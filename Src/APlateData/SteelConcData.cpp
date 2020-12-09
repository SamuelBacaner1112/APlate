// SteelConcData.cpp: implementation of the CSteelConcData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelConcData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelConcData::CSteelConcData()
{
	m_BasicStrength = 0.0;	//	설계기준 강도
	m_SafetyRate = 0.0;		//	안전율
	m_Alpha = 0.0;			//	선팽창 계수
	m_ElasticModulus = 0.0;	//	탄성 계수
	m_UnitWeight = 0.0;		//	단위 중량
	m_Temperature = 0.0;		//	온도
	m_Epsilon = 0.0;			//	건조수축에에 의한 최종 수축율
	m_Creep1 = 0.0;			//	건조수축에에 의한 크리이프계수1
	m_Creep2 = 0.0;			//	건조수축에에 의한 크리이프계수2
	m_BendingStress = 0.0;	//	허용 휨 압축응력
	m_ShearStress = 0.0;		//	허용 전단응력
}

CSteelConcData::~CSteelConcData()
{

}

void CSteelConcData::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring()) 
	{
		ar << nFlag;
		ar << m_strSymbol;
		ar << m_BasicStrength;
		ar << m_SafetyRate;
		ar << m_Alpha;
		ar << m_ElasticModulus;
		ar << m_UnitWeight;
		ar << m_Temperature;
		ar << m_Epsilon;
		ar << m_Creep1;
		ar << m_Creep2;
		ar << m_BendingStress;
		ar << m_ShearStress;
	}
	else {
		ar >> nFlag;
		ar >> m_strSymbol;
		ar >> m_BasicStrength;
		ar >> m_SafetyRate;
		ar >> m_Alpha;
		ar >> m_ElasticModulus;
		ar >> m_UnitWeight;
		ar >> m_Temperature;
		ar >> m_Epsilon;
		ar >> m_Creep1;
		ar >> m_Creep2;
		ar >> m_BendingStress;
		ar >> m_ShearStress;
	}
}