// RebarData.cpp: implementation of the CRebarData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RebarData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRebarData::CRebarData()
{
	//철근물량(길이)	
	m_dLengthD1    = 0;//상부배력철근
	m_dLengthD2[0] = 0;//하부배력철근
	m_dLengthD2[1] = 0;//단부보강배력철근(상부)-[시점][종점]
	m_dLengthD3    = 0;//단부보강배력철근(상부)-[시점][종점]
	m_dLengthD4[0] = 0;//단부보강배력철근(하부)-[시점][종점]
	m_dLengthD4[1] = 0;//단부보강배력철근(하부)-[시점][종점]
	m_dLengthD5    = 0;
	m_dLengthD6    = 0;
	m_dLengthDW[0] = 0;//방호벽배력철근-[좌측][우측]
	m_dLengthDW[1] = 0;//방호벽배력철근-[좌측][우측]
	
	m_dLength1    = 0;//일반부주철근(상부)
	m_dLength2    = 0;//일반부주철근(하부)
	m_dLength3[0] = 0;//캔틸레버부 보강 주철근
	m_dLength3[1] = 0;//캔틸레버부 보강 주철근
	m_dLength4    = 0;//단부주철근(상부)
	m_dLength5    = 0;//단부주철근(하부)
	m_dLengthW[0] = 0;//방호벽주철근-[좌측][우측]
	m_dLengthW[1] = 0;//방호벽주철근-[좌측][우측]
	m_dLengthG    = 0; //스터드 보강철근
	m_dLengthS1   = 0;//간격재1
	m_dLengthS2   = 0;//간격재2
	m_dHeightS1   = 0;//간격재1
	m_dHeightS2   = 0;//간격재2
	//철근물량(수량)
	m_nRebarD1    = 0;//일반부주철근(상부)
	m_nRebarD2[0] = 0;//일반부주철근(하부)
	m_nRebarD2[1] = 0;//단부주철근(상부)
	m_nRebarD3    = 0;//단부주철근(상부)
	m_nRebarD4[0] = 0;//단부주철근(하부)
	m_nRebarD4[1] = 0;//단부주철근(하부)
	m_nRebarD5	  = 0;//단부주철근(하부)
	m_nRebarD6	  = 0;//단부주철근(하부)
	m_nRebarDW[0] = 0;//방호벽배력철근-[좌측][우측]
	m_nRebarDW[1] = 0;//방호벽배력철근-[좌측][우측]

	m_nRebar1    = 0;//일반부주철근(상부)
	m_nRebar2    = 0;//일반부주철근(하부)
	m_nRebar3[0] = 0;//캔틸레버부 보강 주철근
	m_nRebar3[1] = 0;//캔틸레버부 보강 주철근
	m_nRebar4    = 0;//단부주철근(상부)
	m_nRebar5    = 0;//단부주철근(하부)
	m_nRebarW[0] = 0;//방호벽주철근-[좌측][우측]
	m_nRebarW[1] = 0;//방호벽주철근-[좌측][우측]
	m_nRebarG    = 0; //스터드 보강철근
	m_nRebarS1H  = 0;//간격재1
	m_nRebarS2H  = 0;//간격재2
	m_nRebarS1J  = 0;//간격재1
	m_nRebarS2J  = 0;//간격재2
	//철근상세제원
	m_nSpacersuPerM2 = 4;
	m_nStrupDia      = 4;
	m_dDetailL1[0]   = 0;
	m_dDetailL1[1]   = 0;
	m_dDetailL2[0]   = 0;
	m_dDetailL2[1]   = 0;
	m_dDetailL3[0]   = 0;
	m_dDetailL3[1]   = 0;
	m_dDetailL4[0]   = 0;
	m_dDetailL4[1]   = 0;

	m_dLengthShearEnd = 900;
	m_dLengthGuardCTC = 200;
	m_nSpacerDia      = 2;
	m_dRadLengthStrup = 100;
	m_dDiaSpacer      = 0;     //간격재지름
	m_dDiaStrupBogang = 0;//스트럽보강철근지름
	m_nPier           = 0;
}

CRebarData::~CRebarData()
{

}
void CRebarData::Serialize(CArchive& ar)
{	
	long nFlag = 0;
	m_arrSlabSideUp.Serialize(ar);//슬래브 측면 상부포인트(거더 시.종점, 하중분배가로위치)
	m_arrSlabSideDn.Serialize(ar);//슬래브 측면 하부포인트(포인트라인 각이 변하는 포인트)
	m_arrSlabFrontCenUp.Serialize(ar);//슬래브 전면 상부포인트
	m_arrSlabFrontCenDn.Serialize(ar);//슬래브 전면 하부포인트
	m_arrSlabFrontDanUp.Serialize(ar);//슬래브 후면 상부포인트
	m_arrSlabFrontDanDn.Serialize(ar);//슬래브 후면 하부포인트
	m_arrSlabPlan.Serialize(ar);//슬래브 평면 포인트

	m_arrRebarSideUp.Serialize(ar);//슬래브 측면 상부포인트(거더 시.종점, 하중분배가로위치)
	m_arrRebarSideDn.Serialize(ar);//슬래브 측면 하부포인트(포인트라인 각이 변하는 포인트)
	m_arrRebarFrontCenUp.Serialize(ar);//슬래브 전면 상부포인트
	m_arrRebarFrontCenDn.Serialize(ar);//슬래브 전면 하부포인트
	m_arrRebarFrontDanUp.Serialize(ar);//슬래브 후면 상부포인트
	m_arrRebarFrontDanDn.Serialize(ar);//슬래브 후면 하부포인트

	if(ar.IsStoring()) 
	{
		//철근물량(길이)	
		ar << m_dLengthD1;
		ar << m_dLengthD2[0];
		ar << m_dLengthD2[1];
		ar << m_dLengthD3;
		ar << m_dLengthD4[0];
		ar << m_dLengthD4[1];
		ar << m_dLengthD5;
		ar << m_dLengthD6;
		ar << m_dLengthDW[0];
		ar << m_dLengthDW[1];
		
		ar << m_dLength1;
		ar << m_dLength2;
		ar << m_dLength3[0];
		ar << m_dLength3[1];
		ar << m_dLength4;   
		ar << m_dLength5;   
		ar << m_dLengthW[0];
		ar << m_dLengthW[1];
		ar << m_dLengthG; 
		ar << m_dLengthS1;
		ar << m_dLengthS2;
		ar << m_dHeightS1;
		ar << m_dHeightS2;
		//철근물량(수량)
		ar << m_nRebarD1;
		ar << m_nRebarD2[0];
		ar << m_nRebarD2[1];
		ar << m_nRebarD3;
		ar << m_nRebarD4[0];
		ar << m_nRebarD4[1];
		ar << m_nRebarD5;
		ar << m_nRebarD6;
		ar << m_nRebarDW[0];
		ar << m_nRebarDW[1];

		ar << m_nRebar1;
		ar << m_nRebar2;
		ar << m_nRebar3[0];
		ar << m_nRebar3[1];
		ar << m_nRebar4;
		ar << m_nRebar5;
		ar << m_nRebarW[0];
		ar << m_nRebarW[1];
		ar << m_nRebarG; 
		ar << m_nRebarS1H;
		ar << m_nRebarS2H;
		ar << m_nRebarS1J;
		ar << m_nRebarS2J;
		ar << m_nSpacersuPerM2;
		ar << m_nStrupDia;
		ar << m_dLengthShearEnd;
		ar << m_dLengthGuardCTC;
		ar << m_nSpacerDia;
		ar << m_dRadLengthStrup;
		ar << m_dDiaSpacer;     //간격재지름
		ar << m_dDiaStrupBogang;//스트럽보강철근지름
		ar << m_dDetailL1[0];
		ar << m_dDetailL1[1];
		ar << m_dDetailL2[0];
		ar << m_dDetailL2[1];
		ar << m_dDetailL3[0];
		ar << m_dDetailL3[1];
		ar << m_dDetailL4[0];
		ar << m_dDetailL4[1];
		ar << m_nPier;
	}
	else
	{
		//철근물량(길이)	
		ar >> m_dLengthD1;
		ar >> m_dLengthD2[0];
		ar >> m_dLengthD2[1];
		ar >> m_dLengthD3;
		ar >> m_dLengthD4[0];
		ar >> m_dLengthD4[1];
		ar >> m_dLengthD5;
		ar >> m_dLengthD6;
		ar >> m_dLengthDW[0];
		ar >> m_dLengthDW[1];
		
		ar >> m_dLength1;
		ar >> m_dLength2;
		ar >> m_dLength3[0];
		ar >> m_dLength3[1];
		ar >> m_dLength4;   
		ar >> m_dLength5;   
		ar >> m_dLengthW[0];
		ar >> m_dLengthW[1];
		ar >> m_dLengthG; 
		ar >> m_dLengthS1;
		ar >> m_dLengthS2;
		ar >> m_dHeightS1;
		ar >> m_dHeightS2;
		//철근물량(수량)
		ar >> m_nRebarD1;
		ar >> m_nRebarD2[0];
		ar >> m_nRebarD2[1];
		ar >> m_nRebarD3;
		ar >> m_nRebarD4[0];
		ar >> m_nRebarD4[1];
		ar >> m_nRebarD5;
		ar >> m_nRebarD6;
		ar >> m_nRebarDW[0];
		ar >> m_nRebarDW[1];

		ar >> m_nRebar1;
		ar >> m_nRebar2;
		ar >> m_nRebar3[0];
		ar >> m_nRebar3[1];
		ar >> m_nRebar4;
		ar >> m_nRebar5;
		ar >> m_nRebarW[0];
		ar >> m_nRebarW[1];
		ar >> m_nRebarG; 
		ar >> m_nRebarS1H;
		ar >> m_nRebarS2H;
		ar >> m_nRebarS1J;
		ar >> m_nRebarS2J;
		ar >> m_nSpacersuPerM2;
		ar >> m_nStrupDia;
		ar >> m_dLengthShearEnd;
		ar >> m_dLengthGuardCTC;
		ar >> m_nSpacerDia;
		ar >> m_dRadLengthStrup;
		ar >> m_dDiaSpacer;     //간격재지름
		ar >> m_dDiaStrupBogang;//스트럽보강철근지름
		ar >> m_dDetailL1[0];
		ar >> m_dDetailL1[1];
		ar >> m_dDetailL2[0];
		ar >> m_dDetailL2[1];
		ar >> m_dDetailL3[0];
		ar >> m_dDetailL3[1];
		ar >> m_dDetailL4[0];
		ar >> m_dDetailL4[1];
		ar >> m_nPier;
	}
}
