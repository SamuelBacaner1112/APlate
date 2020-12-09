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
	if(m_nType == 0)// 직경  
	{
		m_dAddRateDia[0] = 1.03;
		m_dAddRateDia[1] = 1.06;
		m_dBoundaryDia = 19;
	}
	else			// 주철근여부	
	{
		m_dAddRateMain[0] = 1.06;
		m_dAddRateMain[1] = 1.03;
		m_dBoundaryDia = 0;
	}
}


void CAddRate::Serialize(CArchive& ar)
{
	int nFlag = 2;		//2005년 8월 1일 1로 수정 

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


//철근직경을 넣었을때 해당하는 할증률을 리턴.
double CAddRate::GetAddRateFromDia(double dDia)
{
	if(dDia <= m_dBoundaryDia) //이하면
		return m_dAddRateDia[0];
	else  //초과이면..
		return m_dAddRateDia[1];

}


//주철근인지 아닌지로 할증률을 리턴 
double CAddRate::GetAddRateFromMainBar(BOOL bMainBar)
{
	if(bMainBar) // 주철근이면. 
		return m_dAddRateMain[0];
	else		 // 기타철근이면.
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