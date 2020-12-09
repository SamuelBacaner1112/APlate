// RangePress.cpp: implementation of the CRangePress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RangePress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRangePressData::CRangePressData()
{
	m_bCompress = TRUE;
	m_dDistRange = 0;
}

CRangePressData::CRangePressData(CRangePressData *pObj)
{
	*this = *pObj;			
}

CRangePressData::~CRangePressData()
{
}

void CRangePressData::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_bCompress;		
		ar << m_dDistRange;		
	}
	else
	{
		ar >> nFlag;
		ar >> m_bCompress;		
		ar >> m_dDistRange;		 
	}
}

CRangePressData& CRangePressData::operator=(const CRangePressData& obj)
{
	m_bCompress = obj.m_bCompress;
	m_dDistRange = obj.m_dDistRange;

	return *this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRangePress::CRangePress()
{	
}

CRangePress::CRangePress(CRangePress *pObj)
{
	*this = *pObj;			
}

CRangePress::~CRangePress()
{
	AhTPADelete(&m_ArrayUpper, (CRangePressData*)0);
	AhTPADelete(&m_ArrayLower, (CRangePressData*)0);
}

void CRangePress::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		AhTPASerial(ar, &m_ArrayUpper, (CRangePressData*)0);		
		AhTPASerial(ar, &m_ArrayLower, (CRangePressData*)0);		
	}
	else
	{
		ar >> nFlag;		
		AhTPASerial(ar, &m_ArrayUpper, (CRangePressData*)0);		
		AhTPASerial(ar, &m_ArrayLower, (CRangePressData*)0);		
	}
}

CRangePress& CRangePress::operator=(const CRangePress& obj)
{
	AhTPACopy (&m_ArrayUpper,  &obj.m_ArrayUpper,  (CRangePressData*) 0);	
	AhTPACopy (&m_ArrayLower,  &obj.m_ArrayLower,  (CRangePressData*) 0);	
	return *this;
}

void CRangePress::MakeArray(long nMake, BOOL bUpper)
{
	AhTPAMake(nMake, bUpper ? &m_ArrayUpper : &m_ArrayLower, (CRangePressData*) 0);
}

BOOL CRangePress::IsCompressOnBox(BOOL bUpper, CPlateBasicIndex *pBx)
{
	CTypedPtrArray <CObArray, CRangePressData *> *pArr = bUpper ? &m_ArrayUpper : &m_ArrayLower;
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);	

	double dSta = pBxStt->GetStation();
	for(long n=0; n<pArr->GetSize(); n++)
	{
		dSta += pArr->GetAt(n)->m_dDistRange;
		if(EQUAL(pBx->GetStation(),dSta,EQUAL_VALUE_PLATE))
			continue;
		if(pBx->GetStation() < dSta)
			return pArr->GetAt(n)->m_bCompress;

	}
	return TRUE;
}