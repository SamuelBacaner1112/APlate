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
	m_BasicStrength = 0.0;	//	������� ����
	m_SafetyRate = 0.0;		//	������
	m_Alpha = 0.0;			//	����â ���
	m_ElasticModulus = 0.0;	//	ź�� ���
	m_UnitWeight = 0.0;		//	���� �߷�
	m_Temperature = 0.0;		//	�µ�
	m_Epsilon = 0.0;			//	�������࿡�� ���� ���� ������
	m_Creep1 = 0.0;			//	�������࿡�� ���� ũ���������1
	m_Creep2 = 0.0;			//	�������࿡�� ���� ũ���������2
	m_BendingStress = 0.0;	//	��� �� ��������
	m_ShearStress = 0.0;		//	��� ��������
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