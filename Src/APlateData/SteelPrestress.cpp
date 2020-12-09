// SteelPrestress.cpp: implementation of the CSteelPrestress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelPrestress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DATASU 11
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSteelPrestressDataPlate::CSteelPrestressDataPlate()
{

}

CSteelPrestressDataPlate::~CSteelPrestressDataPlate()
{

}

void CSteelPrestressDataPlate::SerializeOld(CArchive& ar)
{
//	long nFlag = 2;	//APlate2010
//	long nFlag = 3;	// #28172 PS강재제원 관련
	long nFlag = 4;	// Engin 의 CSteelPrestress 사용에 따른 변경
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_szStandard;			///< 관련규격
		ar << m_szSymbol;			///< 기호
		ar << m_szName;				///< 호칭
		ar << m_dDiameter;			///< 지름
		ar << m_dTensionLoad;		///< 인장하중
		ar << m_dArea;				///< 단면적(mm2)
		ar << m_dElongation;		///< 연신율(%)
		ar << m_dRelaxationNor;		///< 릴렉세이션(%) - Normal
		ar << m_dRelaxationLow;		///< 릴렉세이션(%) - Low
		ar << m_dUnitWeight;		///< 단위무게
		ar << m_dLoadOfElongation;	///< 연신율에 대한 하중
		ar << m_szDivision;			///< 구분
	}
	else
	{
		ar >> nFlag;
		ar >> m_szStandard;			///< 관련규격
		ar >> m_szSymbol;			///< 기호
		ar >> m_szName;				///< 호칭
		if(nFlag >= 1  && nFlag < 4)
		{
			double dRemovedValue;
			ar >> dRemovedValue;		//< 항복강도
			ar >> dRemovedValue;		//< 극한강도	
		}
		ar >> m_dDiameter;			///< 지름
		ar >> m_dTensionLoad;		///< 인장하중
		ar >> m_dArea;				///< 단면적(mm2)
		ar >> m_dElongation;		///< 연신율(%)
		ar >> m_dRelaxationNor;		///< 릴렉세이션(%) - Normal
		ar >> m_dRelaxationLow;		///< 릴렉세이션(%) - Low
		ar >> m_dUnitWeight;		///< 단위무게
		ar >> m_dLoadOfElongation;	///< 연신율에 대한 하중
		ar >> m_szDivision;			///< 구분
	}
}

CSteelPrestressPlate::CSteelPrestressPlate()
{
	SetDataInitGangsun();
}

CSteelPrestressPlate::~CSteelPrestressPlate()
{
	RemoveAllGangsunData();
}

void CSteelPrestressPlate::SerializeOld(CArchive& ar)
{
	long nSize = 0;
	if(ar.IsStoring()) 
	{
		nSize = m_PreStressArr.GetSize();
		ar << nSize;
		for(long n = 0 ; n < nSize ; n++)
		{
			CSteelPrestressDataPlate *pObj = (CSteelPrestressDataPlate*)m_PreStressArr[n];
			pObj->SerializeOld(ar);
		}
	}
	else 
	{
		RemoveAllGangsunData();
		ar >> nSize;
		for(long n = 0 ; n < nSize ; n++)
		{
			CSteelPrestressDataPlate *pObj = new CSteelPrestressDataPlate;
			pObj->SerializeOld(ar);
			m_PreStressArr.Add(pObj);
		}
	}
}
