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
	m_dRebarDiameter = obj.m_dRebarDiameter;		//철근직경
	m_dRebarLength_A = obj.m_dRebarLength_A;		//간격제 제원중 A의 길이
	m_dRebarLength_B = obj.m_dRebarLength_B;		//간격제 제원중 B의 길이
	m_dRebarLength_C = obj.m_dRebarLength_C;		//간격제 제원중 C의 길이
	m_dRebarLength_D = obj.m_dRebarLength_D;		//간격제 제원중 D의 길이
	m_nRebarCalcType = obj.m_nRebarCalcType;		//철근제원 산출 방법 종류 0:배치간격, 1:1개의 간격....
	m_nRebarCntInSquare = obj.m_nRebarCntInSquare;	//1m^2 당 간격제 개수 
	m_nCntMainRebarSet = obj.m_nCntMainRebarSet;	//1개의 철근이 감당하는 주철근 간격 
	m_nCntDistRebarSet = obj.m_nCntDistRebarSet;	//1개의 철근이 감당하는 배력철근 간격 
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
		ar << m_dRebarDiameter;			//철근직경
		ar << m_dRebarLength_A;			//간격제 제원중 A의 길이
		ar << m_dRebarLength_B;			//간격제 제원중 B의 길이
		ar << m_dRebarLength_C;			//간격제 제원중 C의 길이
		ar << m_dRebarLength_D;			//간격제 제원중 D의 길이
		ar << m_nRebarCalcType;			//철근제원 산출 방법 종류 0:배치간격, 1:1개의 간격....
		ar << m_nRebarCntInSquare;		//1m^2당 간격제의 개수
		ar << m_nCntMainRebarSet;		//1개의 철근이 감당하는 주철근 간격 
		ar << m_nCntDistRebarSet;		//1개의 철근이 감당하는 배력철근 간격 
		ar << m_dRebarLength_E;			//간격제 제원중 D의 길이
		ar << m_dFy;					//철근 항복 강도 
	}
	else
	{
		ar >> nFlag;
		ar >> m_dRebarDiameter;			//철근직경
		ar >> m_dRebarLength_A;			//간격제 제원중 A의 길이
		ar >> m_dRebarLength_B;			//간격제 제원중 B의 길이
		ar >> m_dRebarLength_C;			//간격제 제원중 C의 길이
		ar >> m_dRebarLength_D;			//간격제 제원중 D의 길이
		ar >> m_nRebarCalcType;			//철근제원 산출 방법 종류 0:배치간격, 1:1개의 간격....
		ar >> m_nRebarCntInSquare;		//1m^2당 간격제의 개수
		ar >> m_nCntMainRebarSet;		//1개의 철근이 감당하는 주철근 간격 
		ar >> m_nCntDistRebarSet;		//1개의 철근이 감당하는 배력철근 간격 
		ar >> m_dRebarLength_E;
		ar >> m_dFy;
	}
}