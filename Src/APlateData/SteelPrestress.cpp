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
//	long nFlag = 3;	// #28172 PS�������� ����
	long nFlag = 4;	// Engin �� CSteelPrestress ��뿡 ���� ����
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_szStandard;			///< ���ñ԰�
		ar << m_szSymbol;			///< ��ȣ
		ar << m_szName;				///< ȣĪ
		ar << m_dDiameter;			///< ����
		ar << m_dTensionLoad;		///< ��������
		ar << m_dArea;				///< �ܸ���(mm2)
		ar << m_dElongation;		///< ������(%)
		ar << m_dRelaxationNor;		///< �������̼�(%) - Normal
		ar << m_dRelaxationLow;		///< �������̼�(%) - Low
		ar << m_dUnitWeight;		///< ��������
		ar << m_dLoadOfElongation;	///< �������� ���� ����
		ar << m_szDivision;			///< ����
	}
	else
	{
		ar >> nFlag;
		ar >> m_szStandard;			///< ���ñ԰�
		ar >> m_szSymbol;			///< ��ȣ
		ar >> m_szName;				///< ȣĪ
		if(nFlag >= 1  && nFlag < 4)
		{
			double dRemovedValue;
			ar >> dRemovedValue;		//< �׺�����
			ar >> dRemovedValue;		//< ���Ѱ���	
		}
		ar >> m_dDiameter;			///< ����
		ar >> m_dTensionLoad;		///< ��������
		ar >> m_dArea;				///< �ܸ���(mm2)
		ar >> m_dElongation;		///< ������(%)
		ar >> m_dRelaxationNor;		///< �������̼�(%) - Normal
		ar >> m_dRelaxationLow;		///< �������̼�(%) - Low
		ar >> m_dUnitWeight;		///< ��������
		ar >> m_dLoadOfElongation;	///< �������� ���� ����
		ar >> m_szDivision;			///< ����
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
