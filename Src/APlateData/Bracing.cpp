// Bracing.cpp: implementation of the CBracing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "Bracing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBracing::CBracing()
{
	m_nType = BRACTYPE_NONE;
	m_dLengthBeam[0] = 0;
	m_dLengthBeam[1] = 0;
	m_pSteelMaterialData = new CSteelMaterialData;
	m_nStdSteelIdx	= 0;
}

CBracing::~CBracing()
{
	delete	m_pSteelMaterialData;
}

void CBracing::SerializeForOld(CArchive &ar)
{
	CPlateBxObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_nType;
		ar << m_dLengthBeam[0];
		ar << m_dLengthBeam[1];
	}
	else
	{
		ar >> m_nType;
		ar >> m_dLengthBeam[0];
		ar >> m_dLengthBeam[1];
	}
}

void CBracing::Serialize(CArchive &ar)
{
	long nFlag = 1;

	CPlateBxObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nType;
		ar << m_dLengthBeam[0];
		ar << m_dLengthBeam[1];
		m_pSteelMaterialData->Serialize(ar);
		ar << m_nStdSteelIdx;
	}
	else
	{
		ar >> nFlag;
		ar >> m_nType;
		ar >> m_dLengthBeam[0];
		ar >> m_dLengthBeam[1];
		m_pSteelMaterialData->Serialize(ar);
		if(nFlag>0)
			ar >> m_nStdSteelIdx;
	}
}


//현재는 수평브레이싱이 하단에만 있다고 가정하고 있음.
//nPos = 0  거더방향으로 첫번째
//nPos = 1  거더방향으로 두번째
double CBracing::GetBeamJewon(long nPos, BOOL bUpper, long nJewon)
{
	double	dValue	= 0;

//	long nSteelType = pMat->GetSteelType(GetBx(), nType);
//	long nJewonType = pMat->GetJewonType(GetBx(), nType);
	switch(nJewon)
	{
		case 0 :
			dValue	= m_pSteelMaterialData->m_dH;// pMat->GetMatData(nSteelType, nJewonType)->m_dH;
			break;
		case 1 :
			dValue	= m_pSteelMaterialData->m_dB;//pMat->GetMatData(nSteelType, nJewonType)->m_dB;
			break;
		case 2 :
			dValue	= m_pSteelMaterialData->m_dT1;//pMat->GetMatData(nSteelType, nJewonType)->m_dT1;
			break;
		case 3 :
			dValue	= m_pSteelMaterialData->m_dT2;//pMat->GetMatData(nSteelType, nJewonType)->m_dT2;
			break;
		case 4 :
			dValue	= m_pSteelMaterialData->m_dR1;//pMat->GetMatData(nSteelType, nJewonType)->m_dR1;
			break;
		case 5 :
			dValue	= m_pSteelMaterialData->m_dR2;//pMat->GetMatData(nSteelType, nJewonType)->m_dR2;
			break;
		case 6 :
		{
			if(nPos==0)
				return m_dLengthBeam[0];
			else
				return m_dLengthBeam[1];
		}
	}
	return dValue;
}
