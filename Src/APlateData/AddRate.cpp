// AddRate.cpp: implementation of the CAddRate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddRate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddRate::CAddRate()
{
	m_nType = 0;
	m_dAddRateDia[0] = 0;
	m_dAddRateDia[1] = 0;
	m_dAddRateMain[0] = 0;
	m_dAddRateMain[1] = 0;
	m_dBoundaryDia = 0;
	m_dFy = 0;
}

CAddRate::CAddRate(const CAddRate& obj)
{
	m_nType = obj.m_nType;
	m_dAddRateDia[0] = obj.m_dAddRateDia[0];
	m_dAddRateDia[1] = obj.m_dAddRateDia[1];
	m_dAddRateMain[0] = obj.m_dAddRateMain[0];
	m_dAddRateMain[1] = obj.m_dAddRateMain[1];
	m_dBoundaryDia = obj.m_dBoundaryDia;
	m_dFy = obj.m_dFy;
}

CAddRate::~CAddRate()
{

}

CAddRate& CAddRate::operator=(const CAddRate& obj)
{
	m_nType = obj.m_nType;
	m_dAddRateDia[0] = obj.m_dAddRateDia[0];
	m_dAddRateDia[1] = obj.m_dAddRateDia[1];
	m_dAddRateMain[0] = obj.m_dAddRateMain[0];
	m_dAddRateMain[1] = obj.m_dAddRateMain[1];
	m_dBoundaryDia = obj.m_dBoundaryDia;
	m_dFy = obj.m_dFy;

	return *this;
}


void CAddRate::SetDataDefault()
{
	if(m_nType == 0)// ����  
	{
		m_dAddRateDia[0] = 1.03;
		m_dAddRateDia[1] = 1.06;
		m_dBoundaryDia = 19;
	}
	else			// ��ö�ٿ���	
	{
		m_dAddRateMain[0] = 1.06;
		m_dAddRateMain[1] = 1.03;
		m_dBoundaryDia = 0;
	}
}


void CAddRate::Serialize(CArchive& ar)
{
	int nFlag = 2;		//2005�� 8�� 1�� 1�� ���� 

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nType;
		ar << m_dAddRateDia[0];
		ar << m_dAddRateDia[1];
		ar << m_dAddRateMain[0];
		ar << m_dAddRateMain[1];
		ar << m_dBoundaryDia;
		ar << m_dFy;
	}
	else 
	{
		ar >> nFlag;
		ar >> m_nType;
		ar >> m_dAddRateDia[0];
		ar >> m_dAddRateDia[1];
		ar >> m_dAddRateMain[0];
		if(nFlag > 1)
		{
			ar >> m_dAddRateMain[1];
			ar >> m_dBoundaryDia;
		}
		
		if(nFlag > 0)
			ar >> m_dFy;
	}
}


//ö�������� �־����� �ش��ϴ� �������� ����.
double CAddRate::GetAddRateFromDia(double dDia)
{
	if(dDia <= m_dBoundaryDia) //���ϸ�
		return m_dAddRateDia[0];
	else  //�ʰ��̸�..
		return m_dAddRateDia[1];

}


//��ö������ �ƴ����� �������� ���� 
double CAddRate::GetAddRateFromMainBar(BOOL bMainBar)
{
	if(bMainBar) // ��ö���̸�. 
		return m_dAddRateMain[0];
	else		 // ��Ÿö���̸�.
		return m_dAddRateMain[1];
}


double CAddRate::GetOneAddRate(double dDia, BOOL bMainBar)
{
	double dAddRate = 0;

	if(m_nType == 0)
		dAddRate = GetAddRateFromDia(dDia);
	else if(m_nType == 1)
		dAddRate = GetAddRateFromMainBar(bMainBar);

	return dAddRate;
}