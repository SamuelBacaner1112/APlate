// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RebarData.h: interface for the CRebarData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REBARDATA_H__511FA521_97A4_4312_A6B1_F463ED70B0DE__INCLUDED_)
#define AFX_REBARDATA_H__511FA521_97A4_4312_A6B1_F463ED70B0DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBasicIndex;
class CPlateGirderApp;
class CPlateBridgeApp;

class AFX_EXT_CLASS CRebarData
{
public:
	CRebarData();
	virtual ~CRebarData();

	void Serialize(CArchive& ar);
	//슬래브의 피이스
	CDPointArray m_arrSlabSideUp;//슬래브 측면 상부포인트(거더 시.종점, 하중분배가로위치)
	CDPointArray m_arrSlabSideDn;//슬래브 측면 하부포인트(포인트라인 각이 변하는 포인트)
	CDPointArray m_arrSlabFrontCenUp;//슬래브 전면 상부포인트
	CDPointArray m_arrSlabFrontCenDn;//슬래브 전면 하부포인트
	CDPointArray m_arrSlabFrontDanUp;//슬래브 후면 상부포인트
	CDPointArray m_arrSlabFrontDanDn;//슬래브 후면 하부포인트
	CDPointArray m_arrSlabPlan;//슬래브 평면 포인트
	//철근 간격 및 개수
	CDPointArray m_arrRebarSideUp;//슬래브 측면 상부포인트(거더 시.종점, 하중분배가로위치)
	CDPointArray m_arrRebarSideDn;//슬래브 측면 하부포인트(포인트라인 각이 변하는 포인트)
	CDPointArray m_arrRebarFrontCenUp;//슬래브 전면 상부포인트
	CDPointArray m_arrRebarFrontCenDn;//슬래브 전면 하부포인트
	CDPointArray m_arrRebarFrontDanUp;//슬래브 후면 상부포인트
	CDPointArray m_arrRebarFrontDanDn;//슬래브 후면 하부포인트
	//철근물량(길이)	
	double m_dLengthD1;//일반부 상부배력철근
	double m_dLengthD2[2];//단부보강배력철근(상부)-[시점][종점]
	double m_dLengthD3;//일반부 하부배력철근
	double m_dLengthD4[2];//단부보강배력철근(하부)-[시점][종점]
	double m_dLengthD5;//지점부보강배력철근(상부)
	double m_dLengthD6;//지점부보강배력철근(하부)
	double m_dLengthDW[2];//방호벽배력철근-[좌측][우측]
	
	double m_dLength1;//일반부주철근(상부)
	double m_dLength2;//일반부주철근(하부)
	double m_dLength3[2];//캔틸레버부 보강 주철근
	double m_dLength4;//단부주철근(상부)
	double m_dLength5;//단부주철근(하부)
	double m_dLengthW[2];//방호벽주철근-[좌측][우측]
	double m_dLengthG; //스터드 보강철근
	double m_dLengthS1;//간격재1
	double m_dLengthS2;//간격재2
	double m_dHeightS1;//간격재1
	double m_dHeightS2;//간격재2

	//철근물량(수량)
	long   m_nRebarD1;//일반부 상부배력철근
	long   m_nRebarD2[2];//단부보강배력철근(상부)-[시점][종점]
	long   m_nRebarD3;//일반부 하부배력철근
	long   m_nRebarD4[2];//단부보강배력철근(하부)-[시점][종점]
	long   m_nRebarD5;//지점부보강배력철근(상부)
	long   m_nRebarD6;//지점부보강배력철근(하부)
	double m_nRebarDW[2];//방호벽배력철근-[좌측][우측]

	long   m_nRebar1;//일반부주철근(상부)
	long   m_nRebar2;//일반부주철근(하부)
	long   m_nRebar3[2];//캔틸레버부 보강 주철근
	long   m_nRebar4;//단부주철근(상부)
	long   m_nRebar5;//단부주철근(하부)
	long   m_nRebarW[2];//방호벽주철근-[좌측][우측]
	double m_nRebarG; //스터드 보강철근
	double m_nRebarS1H;//횡방향 간격재1
	double m_nRebarS2H;//횡방향 간격재2
	double m_nRebarS1J;//종방향 간격재1
	double m_nRebarS2J;//종방향 간격재2
	//철근상세제원
	long   m_nSpacersuPerM2;//단위면적당(m2)간격재갯수
	long   m_nStrupDia;//전단철근보강재직경
	double m_nSpacerDia;//간격재직경
	double m_dLengthShearEnd;//전단보강철근 양끝길이 

	double m_dDetailL1[2];
	double m_dDetailL2[2];
	double m_dDetailL3[2];
	double m_dDetailL4[2];

	double m_dLengthGuardCTC;//방호벽CTC
	double m_dRadLengthStrup;//전단보강철근휜부분의 반경
	double m_dDiaSpacer;     //간격재지름
	double m_dDiaStrupBogang;//스트럽보강철근지름
	long   m_nPier;          //지점부부모멘트보강철근사용시 피어위치결정 

};

#endif // !defined(AFX_REBARDATA_H__511FA521_97A4_4312_A6B1_F463ED70B0DE__INCLUDED_)
