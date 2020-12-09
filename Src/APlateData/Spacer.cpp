// Spacer.cpp: implementation of the CSpacer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spacer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpacer::CSpacer()
{
	m_dRebarDiameter = 0;	
	m_dRebarLength_A = 0;	
	m_dRebarLength_B = 0;	
	m_dRebarLength_C = 0;	
	m_dRebarLength_D = 0;	
	m_nRebarCalcType = 0;	
	m_nRebarCntInSquare = 0;	
	m_nCntMainRebarSet = 0;	
	m_nCntDistRebarSet = 0;	
	m_dFy = 0;
}

CSpacer::~CSpacer()
{

}

CSpacer::CSpacer(const CSpacer& obj)
{
	m_dRebarDiameter = obj.m_dRebarDiameter;		//ö������
	m_dRebarLength_A = obj.m_dRebarLength_A;		//������ ������ A�� ����
	m_dRebarLength_B = obj.m_dRebarLength_B;		//������ ������ B�� ����
	m_dRebarLength_C = obj.m_dRebarLength_C;		//������ ������ C�� ����
	m_dRebarLength_D = obj.m_dRebarLength_D;		//������ ������ D�� ����
	m_nRebarCalcType = obj.m_nRebarCalcType;		//ö������ ���� ��� ���� 0:��ġ����, 1:1���� ����....
	m_nRebarCntInSquare = obj.m_nRebarCntInSquare;	//1m^2 �� ������ ���� 
	m_nCntMainRebarSet = obj.m_nCntMainRebarSet;	//1���� ö���� �����ϴ� ��ö�� ���� 
	m_nCntDistRebarSet = obj.m_nCntDistRebarSet;	//1���� ö���� �����ϴ� ���ö�� ���� 
	m_dFy = obj.m_dFy;
}


void CSpacer::SetDataDefault(BOOL bRecommend)
{
	if(bRecommend)
	{
		m_dRebarDiameter = 16;	
		m_dRebarLength_A = 100;
		m_dRebarLength_B = 200;
		m_dRebarLength_C = 200;	
		m_dRebarLength_D = 100;	
		m_dRebarLength_E = 100;	
		m_nRebarCalcType = 0;	
		m_nRebarCntInSquare = 2;	
		m_nCntMainRebarSet = 8;	
		m_nCntDistRebarSet = 8;		
	}
}


void CSpacer::Serialize(CArchive& ar)
{
	long nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_dRebarDiameter;			//ö������
		ar << m_dRebarLength_A;			//������ ������ A�� ����
		ar << m_dRebarLength_B;			//������ ������ B�� ����
		ar << m_dRebarLength_C;			//������ ������ C�� ����
		ar << m_dRebarLength_D;			//������ ������ D�� ����
		ar << m_nRebarCalcType;			//ö������ ���� ��� ���� 0:��ġ����, 1:1���� ����....
		ar << m_nRebarCntInSquare;		//1m^2�� �������� ����
		ar << m_nCntMainRebarSet;		//1���� ö���� �����ϴ� ��ö�� ���� 
		ar << m_nCntDistRebarSet;		//1���� ö���� �����ϴ� ���ö�� ���� 
		ar << m_dRebarLength_E;			//������ ������ D�� ����
		ar << m_dFy;					//ö�� �׺� ���� 
	}
	else
	{
		ar >> nFlag;
		ar >> m_dRebarDiameter;			//ö������
		ar >> m_dRebarLength_A;			//������ ������ A�� ����
		ar >> m_dRebarLength_B;			//������ ������ B�� ����
		ar >> m_dRebarLength_C;			//������ ������ C�� ����
		ar >> m_dRebarLength_D;			//������ ������ D�� ����
		ar >> m_nRebarCalcType;			//ö������ ���� ��� ���� 0:��ġ����, 1:1���� ����....
		ar >> m_nRebarCntInSquare;		//1m^2�� �������� ����
		ar >> m_nCntMainRebarSet;		//1���� ö���� �����ϴ� ��ö�� ���� 
		ar >> m_nCntDistRebarSet;		//1���� ö���� �����ϴ� ���ö�� ���� 
		ar >> m_dRebarLength_E;
		ar >> m_dFy;
	}
}