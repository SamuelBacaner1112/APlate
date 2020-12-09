// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ADeckData.h: interface for the CADeckData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADECKDATA_H__D5E29F58_6D9A_484D_99E9_80429B01650A__INCLUDED_)
#define AFX_ADECKDATA_H__D5E29F58_6D9A_484D_99E9_80429B01650A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 철근배근위치
#define POS_SLAB_LEFT			0
#define POS_SLAB_RIGH			1
#define POS_SLAB_CENTER			2
#define POS_SLAB_TOTAL			3 
// 주철근 간격
#define CTC_MAIN_UPPER_LEFT		0
#define CTC_MAIN_UPPER_RIGHT	1
#define CTC_MAIN_UPPER_CENTER	2
#define CTC_MAIN_LOWER_LEFT		3
#define CTC_MAIN_LOWER_RIGHT	4
#define CTC_MAIN_LOWER_CENTER	5
#define CTC_MAIN_SIZE			6
// 배력철근 간격
#define CTC_BERYUK_UPPER		0
#define CTC_BERYUK_LOWER		1
#define CTC_BERYUK_SIZE			2
// 사보강 철근 간격
#define CTC_SABOGANG_UPPER_STT 0
#define CTC_SABOGANG_UPPER_END 1
#define CTC_SABOGANG_LOWER_STT 2
#define CTC_SABOGANG_LOWER_END 3
#define CTC_SABOGANG_SIZE	   4
// 사각부 철근 간격(평면)
#define CTC_SKEWMAIN_UPPER_LEFT_STT 0
#define CTC_SKEWMAIN_UPPER_LEFT_END 1
#define CTC_SKEWMAIN_LOWER_LEFT_STT 2
#define CTC_SKEWMAIN_LOWER_LEFT_END 3
#define CTC_SKEWMAIN_UPPER_RIGH_STT 4
#define CTC_SKEWMAIN_UPPER_RIGH_END 5
#define CTC_SKEWMAIN_LOWER_RIGH_STT 6
#define CTC_SKEWMAIN_LOWER_RIGH_END 7
#define CTC_SKEWMAIN_UPPER_CENTER_STT 8
#define CTC_SKEWMAIN_UPPER_CENTER_END 9
#define CTC_SKEWMAIN_LOWER_CENTER_STT 10
#define CTC_SKEWMAIN_LOWER_CENTER_END 11
#define CTC_SKEWMAIN_SIZE	 	    12
// 사각부 철근 간격(종단)
#define CTC_SKEWJONG_UPPER_CENTER_STT 0
#define CTC_SKEWJONG_UPPER_CENTER_END 1
#define CTC_SKEWJONG_LOWER_CENTER_STT 2
#define CTC_SKEWJONG_LOWER_CENTER_END 3
#define CTC_SKEWJONG_SIZE	 	    4
// 주철근 기준위치(종단)
#define JONGBASEPOS_SLABCENTER 0	// 슬래브중심
#define JONGBASEPOS_SLABLEFT   1	// 슬래브좌측
#define JONGBASEPOS_SLABRIGHT  2	// 슬래브우측
#define JONGBASEPOS_SLABLC	   3	// 슬래브좌측과 중앙부 경계	
#define JONGBASEPOS_SLABRC	   4	// 슬래브우측과 중앙부 경계	
#define JONGBASEPOS_ONCB	   5	// 거더0~거더1사이 가로보 중앙위치의 슬래브 단면
// 배력철근 위치(종단)
#define JONGPOS_SLABSTTEND 0	// 슬래브 단부
#define JONGPOS_SLABCENTER 1	// 슬래브 중심
#define JONGPOS_SLABJIJUM  2	// 슬래브 지점부
// 단위중량 및 단면적
#define BASIC_WEIGHT_UNIT	0
#define BASIC_AREA_UNIT		1
#define BASIC_DIAMEATER		2
#define BASIC_GIRTH			3
// 주철근 그리기 옵션(평면)
#define DRAW_LEFTSIDE		-1
#define DRAW_RIGHTSIDE		 1
#define DRAW_SIDEALL		 0
// 횡단면 배근위치
#define SECTION_DAN			0	// 슬래브 시점(주,배)
#define SECTION_CEN			1	// 슬래브 중앙부(주,배)
#define SECTION_SKEW_STT	2	// 사각부 시점(주)
#define SECTION_SKEW_END	3	// 사각부 종점(주)
#define SECTION_PIER		4	// 슬래브 지점부(배)
#define SECTION_TOTAL		5	

// 입력에 대한 정보(사이클별)
typedef struct _REBAR_CYCLE
{
	CDoubleArray m_dArrJointUp;		// 상부 겹이음 중심위치	: (주철근, 배력철근)
	CDoubleArray m_dArrJointDn;		// 하부 겹이음 중심위치	: (주철근, 배력철근)		
	CDoubleArray m_dArrDiaUp;		// 상부 철근직경		: (주철근, 배력철근)
	CDoubleArray m_dArrDiaDn;		// 하부 철근직경		: (주철근, 배력철근)
	CDoubleArray m_dArrValidUp;		// 0 : 삭제 1: 존재		: (주철근, 배력철근)
	CDoubleArray m_dArrValidDn;		// 0 : 삭제 1: 존재		: (주철근, 배력철근)

} REBAR_CYCLE;

// 철근형태에 대한 정보(사이클별)
// CDPoint x: 기준점으로부터 이격거리, y: 이음길이
typedef struct _REBAR_SHAPE
{
	CDPointArray m_dArrShape;		// 형태
	CDPointArray m_dArrInJoint;		// 내부이음	
	double		 m_dDia;			// 직경
	CString		 m_sName;			// 명칭
	BOOL		 m_bValid;			// 0 : 삭제 1: 존재
	long		 m_nCycle;			// 현재 사이클
	BOOL		 m_bUpper;			// 상부/하부 철근
} REBAR_SHAPE;


// 철근형태에 대한 정보(사이클별)
// CDPoint x: 기준점으로부터 이격거리, y: 이음길이
typedef struct _REBAR_PLAN
{
	long m_nPos;	// -1:단부시점,  0:일반부,  1:단부종점
	long m_nCycle;
	long m_nShape;
	CString m_sName;
	CTwinVector m_tVector;
} REBAR_PLAN;


class CPlateBasicIndex;
class CBoxOpenBar;
class CRoundOpenBar;
class CSupportRFBarSet;
class CAddRate;
class CBMOption;
class CDataManage;

class AFX_EXT_CLASS	CADeckData : public CBasicConcInfo  
{
// Constructor
public:
	CADeckData(CDataManage* pManage = NULL);
	virtual ~CADeckData();

// Attributes	
public:
	CDataManage* m_pDataManage;
	CPlateBridgeApp* m_pDB;	
	CBMOption	m_BMOption;

	enum { SP_LEFT=0, SP_RIGHT=1, SP_BOTH=2 };

	///////////////////////////////////////////////////////////////////////////////////////////
	// 기본데이터
	///////////////////////////////////////////////////////////////////////////////////////////
	double m_dCoverUp;					// 콘크리트피복두께(상부)
	double m_dCoverDn;					// 콘크리트피복두께(하부)
	double m_dCoverSd;					// 콘크리트피복두께(측면)
	long   m_nSelectRebarAddRate;		// 철근 할증율 산정 방식

	long   m_nRoundRebarConnect;
	long   m_nTypeBMApply;				// 수량산출 적용기준
	BOOL   m_bIncludeSumZero;			// 수량집계에 빈수량(zero) 포함
	double m_dLengthRebarMax;
	BOOL   m_bShowMark;					// 배근입력화면에서 철근마크보기		
	BOOL   m_bRecalcJewon;				// 배근입력[적용]시 제원재계산
	BOOL   m_bCheckSinchukDetail;
	BOOL   m_bCheckSinchukSimple;
	double m_SigmaCK;					// 콘크리트 설계 강도  ARoadDesignData Dll의 CFStipData의 값을 복사해서 사용함.
	double m_SigmaY;					// 철근의 응력
	double m_SigmaSA;					// 철근의 허용응력


	///////////////////////////////////////////////////////////////////////////////////////////
	// 기타철근                                                             
	///////////////////////////////////////////////////////////////////////////////////////////
	CBoxOpenBar			m_WaterHoleBoxType;		// Box형철근  개구부 Class
	CRoundOpenBar		m_WaterHoleRoundType;	// Round형철근  개구부 Class
	CSpacer				m_SpacerRebar;			// Spacer Class
	CSupportRFBarSet	m_ShearRebarDanbu;		// 단부 보강철근 Class 
	CSupportRFBarSet	m_ShearRebarJijum;		// 지점부 보강철근 Class
	long				m_nTypeWaterHole;		// 배수구철근 타입
	long				m_nWaterHolesu;			// 배수구 개수
	long				m_nWaterHoleDansu;		// 배수구 단수 
	BOOL				m_bInstallShearRebar;	// 전단보강철근  넣을 것인지 아닌지..
	BOOL				m_bInstallWaterHole;	// 배수구보강철근 넣을 것인지 아닌지..
	///////////////////////////////////////////////////////////////////////////////////////////
	// 철근의 직경
	///////////////////////////////////////////////////////////////////////////////////////////
	// 상부	
	double m_dDiaMainRebar_Cent[POS_SLAB_TOTAL];			// 상부 일반부 주철근 Dia 
	double m_dDiaMainRebar_Dan[POS_SLAB_TOTAL];				// 상부 단부 주철근 Dia 
	double m_dDiaMainRebar_Cent2nd[POS_SLAB_TOTAL];			// 상부 일반부 주철근 Dia  -2Cycle
	double m_dDiaMainRebar_Dan2nd[POS_SLAB_TOTAL];			// 상부 단부 주철근 Dia 	-2Cycle
	double m_dDiaBeryukRebar_Cent[POS_SLAB_TOTAL];			// 상부 일반부 배력철근 Dia 
	double m_dDiaBeryukRebar_Dan[POS_SLAB_TOTAL];			// 상부 단부 배력철근 Dia 
	// 하부
	double m_dDiaMainRebarLower_Cent[POS_SLAB_TOTAL];		// 하부 일반부 주철근 Dia 
	double m_dDiaMainRebarLower_Dan[POS_SLAB_TOTAL];		// 하부 단부 주철근 Dia 
	double m_dDiaMainRebarLower_Cent2nd[POS_SLAB_TOTAL];
	double m_dDiaMainRebarLower_Dan2nd[POS_SLAB_TOTAL];
	double m_dDiaBeryukRebarLower_Cent[POS_SLAB_TOTAL];		// 하부 일반부 배력철근 Dia 
	double m_dDiaBeryukRebarLower_Dan[POS_SLAB_TOTAL];		// 하부 단부 배력철근 Dia 
	// 가로보 및 세로보 보강 철근
	double m_dDiaCrossBeam1;								// 가로보 헌치부철근
	double m_dDiaCrossBeam2;								// 가로보 하면 철근
	double m_dDiaCrossBeam3;								// 가로보 상면 철근
	double m_dDiaStringer1;								// 세로보 헌치부 철근
	double m_dDiaStringer2;								// 세로보 하면 철근
	double m_dDiaStringer3;								// 세로보 상면 철근
	double m_dDiaOutBrk1;								// 외측가로보 헌치부 철근
	double m_dDiaOutBrk2;								// 외측가로보 하면 철근
	double m_dDiaOutBrk3;								// 외측가로보 상면 철근
	// 기타
	double m_dDiaSabogangUp;							// 사보강철근 상부 직경 
	double m_dDiaSabogangDn;							// 사보강철근 하부 직경
	double m_dDiaDanbuHunch;							// 단부 헌치 정착장 위치 주철근 Dia 
	// 지점부 바닥판
	double m_dDiaJijumRebarUp[POS_SLAB_TOTAL];			// 지점부 상부 교축방향철근 Dia 
	double m_dDiaJijumRebarDn[POS_SLAB_TOTAL];			// 지점부 하부 교축방향철근 Dia 
	// 등방배근
	double m_dDiaDMainRebarUpper_Cent;					// 주철근 상면 Dia 
	double m_dDiaDMainRebarLower_Cent;					// 주철근 하면 Dia 
	double m_dDiaDBeryukRebarUpper;						// 배력철근 상면 Dia 
	double m_dDiaDBeryukRebarLower;						// 배력철근 상면 Dia 
	double m_dDiaDMainRebar_Dan;						// 단부보강 주철근 Dia 
	double m_dDiaDBeryukRebar_Dan;						// 단부보강 배력철근 Dia 

	///////////////////////////////////////////////////////////////////////////////////////////
	// 배근 기본간격
	///////////////////////////////////////////////////////////////////////////////////////////
	double m_dGenMainRebarCTC[POS_SLAB_TOTAL];			// 상부 일반부 주철근 CTC
	double m_dGenSupportRebarCTC[POS_SLAB_TOTAL];		// 상부 일반부 배력철근 CTC
	double m_dEndMainRebarCTC[POS_SLAB_TOTAL];			// 상부 단부 주철근 CTC
	double m_dEndSupportRebarCTC[POS_SLAB_TOTAL];		// 상부 단부 배력철근 CTC

	double m_dGenMainRebarCTCLower[POS_SLAB_TOTAL];		// 하부 일반부 주철근 CTC
	double m_dGenSupportRebarCTCLower[POS_SLAB_TOTAL];	// 하부 일반부 배력철근 CTC
	double m_dEndMainRebarCTCLower[POS_SLAB_TOTAL];		// 하부 단부 주철근 CTC
	double m_dEndSupportRebarCTCLower[POS_SLAB_TOTAL];	// 하부 단부 배력철근 CTC

	double m_dJijumRebarCTCUp[POS_SLAB_TOTAL];			// 지점부 1단 교축방향철근 CTC
	double m_dJijumRebarCTCDn[POS_SLAB_TOTAL];			// 지점부 2단 교축방향철근 CTC

	double m_dSabogangRebarCTCUp;						// 사보강철근(상부) CTC 	
	double m_dSabogangRebarCTCDn;						// 사보강철근(하부) CTC 	
	
	// 등방배근 철근
	double	m_dDMainRebarCTCUpper;					// 주철근 상면 간격
	double	m_dDMainRebarCTCLower;					// 주철근 하면 간격
	double	m_dDBeryukRebarCTCUpper;				// 배력철근 상면 간격
	double	m_dDBeryukRebarCTCLower;				// 배력철근 상면 간격
	double	m_dDDanBogangMainCTC;					// 단부보강 주철근 간격
	double	m_dDDanBogangBeryukCTC;					// 단부보강 배력철근 간격
	
	double m_dErrorRangeCTC;					    	// 주철근 구간 오차범위
	double m_dTrimRatio;								// 트림비율		

	///////////////////////////////////////////////////////////////////////////////////////////
	// 배근 실제간격
	///////////////////////////////////////////////////////////////////////////////////////////
	CDPointArray m_arrCTCBeryukRebarDan[CTC_BERYUK_SIZE];// 배력철근 간격 : 단부
	CDPointArray m_arrCTCBeryukRebarCen[CTC_BERYUK_SIZE];// 배력철근 간격 : 중앙부
	CDPointArray m_arrCTCBeryukRebarJijum[CTC_BERYUK_SIZE];	// 배력철근 간격 : 지점부
	CDoubleArray m_arrVariableBeryukRebar[CTC_BERYUK_SIZE];	// 배력철근 간격
	
	CDPointArray m_arrCTCMainRebar[CTC_MAIN_SIZE];			// 기준 종단면 주철근간격
	CDPointArray m_arrCTCMainRebarForDim[CTC_MAIN_SIZE];	// 기준 종단면 주철근간격(치수선용)
	CDPointArray m_arrCTCSabogangRebar[CTC_SABOGANG_SIZE];	// 사보강철근 간격	
	CDPointArray m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_SIZE];// 사보강부의 주철근 간격
	CDPointArray m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_SIZE];// 사보강부의 주철근 간격(치수선용)
	CDPointArray m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_SIZE];// 사각부 주철근 간격-종단
	CDPointArray m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_SIZE];// 사각부 주철근 간격(치수선용)-종단
	CDPointArray m_arrCTCMainRebarUnderSinchuk;				// 신축이음부 바로 아래의 주철근 간격
	CDPointArray m_arrCTCBogangRebarAtCrossBeam;			// 가로보보강철근의 간격
	CDPointArray m_arrIndexBogangRebarAtCrossBeam;			// 가로보보강철근(nG, nCB)
	CDPointArray m_arrCTCBogangRebarAtStringer;				// 세로보보강철근의 간격
	CDPointArray m_arrCTCBogangRebarAtOutBrk;				// 외측가로보보강철근의 간격
	CDPointArray m_arrCTCMainRebarDanHunch;					// 단부 헌치 정착장 간격
	CDPoint		 m_CTCBasicCrossBeam;						// 가로보보강철근의 간격시 표준치수
	CDPoint		 m_CTCBasicStringer;						// 세로보보강철근의 간격시 표준치수
	CDPoint		 m_CTCBasicOutBrk;							// 외측가로보보강철근의 간격시 표준치수
	CVectorArray m_arrStringer;								// 세로보보강 위치(x:거더, y:가로보, z:세로보)

	//////////////////////////////////////////////////////////////////////////
	// 배치범위
	//////////////////////////////////////////////////////////////////////////
	double m_dRangeDanbuStt;		// 단부 시점 주철근 배치 범위
	double m_dRangeDanbuEnd;		// 단부 종점 주철근 배치 범위

	//////////////////////////////////////////////////////////////////////////
	// 배근사이클
	//////////////////////////////////////////////////////////////////////////
	vector <REBAR_CYCLE> m_stMainCycleDan;		// 단부 주철근 cycle에 대한 정보
	vector <REBAR_CYCLE> m_stMainCycleCen;		// 일반부 주철근 cycle에 대한 정보
	vector <REBAR_CYCLE> m_stMainCyclePier;			// 지점부 주철근 cycle에 대한 정보
	vector <REBAR_CYCLE> m_stMainCycleSabogangStt;	// 사보강 주철근 cycle에 대한 정보
	vector <REBAR_CYCLE> m_stMainCycleSabogangEnd;	// 사보강 주철근 cycle에 대한 정보
	
	vector <REBAR_CYCLE> m_stBeryukCycleLeft;		// 배력철근 cycle에 대한 정보(슬래브좌측)	
	vector <REBAR_CYCLE> m_stBeryukCycleRigh;		// 배력철근 cycle에 대한 정보(슬래브우측)	
	vector <REBAR_CYCLE> m_stBeryukCycleCent;		// 배력철근 cycle에 대한 정보(슬래브중앙)
	vector <REBAR_CYCLE> m_stBeryukCycleLC;			// 배력철근 cycle에 대한 정보(슬래브좌중측)
	vector <REBAR_CYCLE> m_stBeryukCycleRC;			// 배력철근 cycle에 대한 정보(슬래브우중측)
	vector <REBAR_CYCLE> m_stBeryukCycleOnCB;		// 배력철근 cycle에 대한 정보(거더1~거더2 가로보의 중앙단면)

	//////////////////////////////////////////////////////////////////////////
	// 철근형태에 대한 정보 -    사이클=>Shape
	//////////////////////////////////////////////////////////////////////////
	vector<vector<REBAR_SHAPE> > m_stMainShapeDan;	// 슬래브 시점 주철근 Shape에 대한 정보
	vector<vector<REBAR_SHAPE> > m_stMainShapeCen;	// 일반부 주철근 Shape에 대한 정보
	vector<vector<REBAR_SHAPE> > m_stMainShapePier;		// 지점부 주철근 Shape에 대한 정보
	vector<vector<REBAR_SHAPE> > m_stMainShapeSkewStt;	// 시점 사보강 주철근 Shape에 대한 정보
	vector<vector<REBAR_SHAPE> > m_stMainShapeSkewEnd;	// 종점 사보강 주철근 Shape에 대한 정보
	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeLeft;	// 배력철근 Shape에 대한 정보(슬래브좌측)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeRigh;	// 배력철근 Shape에 대한 정보(슬래브우측)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeCent;	// 배력철근 Shape에 대한 정보(슬래브중앙)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeLC;		// 배력철근 Shape에 대한 정보(슬래브좌중측)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeRC;		// 배력철근 Shape에 대한 정보(슬래브우중측)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeOnCB;	// 배력철근 Shape에 대한 정보(거더1~거더2 가로보의 중앙단면)

	///////////////////////////////////////////////////////////////////////////////////////////
	// 배근방법옵션
	///////////////////////////////////////////////////////////////////////////////////////////
	long m_nTypeDanbuTaperBeryukRebar;					// 단부 Taper 위치 배력철근 형상
	BOOL m_bInstallDanbuTaperMainRebar;					// 단부의 Taper 위치 주철근 적용
	BOOL m_bInstallMainRebarAtBlockOut;					// 무수축콘크리트 주철근 설치
	BOOL m_bInstallBeryukRebarAtBlockOut;				// 무수축콘크리트 배력철근 설치
	long m_nJongCtcCalcPos;								// 종단면 철근CTC 기준위치
	long m_nJongDrawPos;								// 종단면생성위치
	long m_nJijumBogangArrange;							// [다경간]지점보강 주철근 배치
	long m_nOptMainRebarArrange;						// [사교시]주철근 배치
	long m_nOptSabogangRebarRange;						// [사교시]사보강철근 배치범위옵션
	long m_nOptMainRebarAtSkewRange;					// [사교시]사각부 주철근 배치범위옵션
	long m_nOptBeryukRebarArrange;						// 배력철근 짜투리 치수 옵션
	double m_dRangeSabogangRebarStt;					// [사교시]사보강철근 배치범위(시점부)
	double m_dRangeMainRebarAtSkewStt;					// [사교시]사각부 주철근 배치범위(종점부)
	double m_dRangeSabogangRebarEnd;					// [사교시]사보강철근 배치범위(시점부)
	double m_dRangeMainRebarAtSkewEnd;					// [사교시]사각부 주철근 배치범위(종점부)
	long  m_nUseJujangRebar;							// 주장율 철근 배근 옵션 0: 총철근량의 1/3을 순지간의 1/16까지 연장배근  1 : 총철근량의 1/2을 순지간의 1/16까지 연장배근
	int	  m_nTypeGuardCalc;								//방호벽 제원 계산 방법.
	long m_nDetailMainUpper;							// 상부주철근 배치 옵션 (0 : 직선타입 1 : ㄱ타입)
	long m_nDetailMainLowerCenter;						// 중앙부 하부 주철근 배치 옵션 (0 : 분리배치 1 : 중앙부배치 2 : 전체배치) 
	BOOL m_bInstallCrossBeamUpperBogangRebar;			// 가로보 상면보강철근 설치 옵션(직선철근)
	BOOL m_bInstallStringerUpperBogangRebar;			// 세로보 상면보강철근 설치 옵션(직선철근)
	BOOL m_bInstallOutBrkUpperBogangRebar;				// 외측가로보 상면보강철근 설치 옵션(직선철근)	
	long m_nDetailSettlement;							// 정착장 설치옵션		(0 : // 헌치 상단부 기준 1: // 헌치 하단부 기준)
	long m_nSpaceTrimTypeMain;							// 주철근 짜투리 치수 옵션 <사용안함>
	long m_nCTCOptionGen;								// 배력철근 일반부 CTC 옵션(확폭시)
	long m_nCTCOptionPier;								// 배력철근 지점부 CTC 옵션(확폭시)
	BOOL m_bApplyBeryukCTC;								// 배력철근CTC범위 적용(강도설계법형태)
	long m_nMainCyclesu;								// 주철근 사이클수
	BOOL m_bMainRebarLowerLevel;						// 주철근 하부 철근 Level적용(철도교시)
	BOOL m_bPlusToSlabRebar;
	BOOL m_bDivideRebarAtCant;							// 캔틸레버부에 헌치 있을 경우 주철근을 분리할지 여부
	//////////////////////////////////////////////////////////////////////////
	// 배근길이변수
	//////////////////////////////////////////////////////////////////////////
	double m_dLengthMainDolchulUp[2];			// 단부 상부주철근이 ㄱ타입일 경우 돌출길이( 0 : 좌측, 1 : 우측)
	double m_dLengthMainDolchulLo[2];			// 단부 하부주철근이 ㄱ타입일 경우 돌출길이
	double m_dLengthMainDolchulUpDan[2];		// 단부 상부주철근이 ㄱ타입일 경우 돌출길이
	double m_dLengthMainDolchulLoDan[2];		// 단부 하부주철근이 ㄱ타입일 경우 돌출길이
	double m_dRangeDistribRebarOnCant;			// 캔틸레버부 배력근 범위(Sharp끝단에서 거더안쪽으로 범위)
	double m_dWidthSlabCantLeftForBeryuk;		// 좌측 캔틸레버부 배력근 범위(슬래브좌측끝단에서 범위)
	double m_dWidthSlabCantRighForBeryuk;		// 우측 캔틸레버부 배력근 범위(슬래브우측끝단에서 범위)
	double m_dRangeDistribRebarOnGirder;		// 거더부 배력근 범위
	double m_dLengthCrossBeamBogang;			// 가로보상부보강철근의 길이
	double m_dLengthCrossBeamBogangLower;		// 가로보하부보강철근의 이격길이(브라켓플랜지 끝단~)
	double m_dLengthStringerBogang;				// 세로보하면보강철근의 길이
	double m_dLengthOutBrkBogang;				// 외측가로보하면보강철근의 길이
	double m_dLengthOutBrkBogangLower;			// 외측가로보하부보강철근의 정착길이
	double m_dRangeMainRebarFromFlangeStt;		// 캔틸레버부 주철근의 플랜지 끝단에서 중심방향으로 돌출거리
	double m_dLengthBeryukSttA;					// 배력철근 시점부 돌출길이
	double m_dLengthBeryukSttB;					// 배력철근 시점부 헌치부 돌출길이1
	double m_dLengthBeryukSttC;					// 배력철근 시점부 헌치부 돌출길이2
	double m_dLengthBeryukEndA;					// 배력철근 종점부 돌출길이
	double m_dLengthBeryukEndB;					// 배력철근 종점부 헌치부 돌출길이1
	double m_dLengthBeryukEndC;					// 배력철근 종점부 헌치부 돌출길이2
	double m_dRangeDistribRebarDup;				// 배력근이 겹치는 범위

	//////////////////////////////////////////////////////////////////////////
	// 철근제원
	//////////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CRebar*>	m_arrRebarJewon;		// 재료표용 철근리스트
	CSafetyTypedPtrArray <CObArray, CRebar*>	m_arrRebarJewonGuard;	// 재료표용 철근리스트
	long		m_nAddRebarsu;					// 추가철근 개수 
	CAddRate	m_AddRateRebar;					// 철근 할증률 

	//////////////////////////////////////////////////////////////////////////
	// 철근마크
	//////////////////////////////////////////////////////////////////////////
	vector<pair<CString, CDPointArray > >		m_arrMarkPosPlan[20];	// 철근마크(지간별 : 최대 지간 20개)
	long   m_nRebarNo;							// 마크번호 생성시 사용함..

	//////////////////////////////////////////////////////////////////////////
	// 평면도 Trim영역 결정
	//////////////////////////////////////////////////////////////////////////
	CDoubleArray m_arrTrimRange;			// 배근도 Trim 구간에 대한 설정 - 홀수열 : 그리기, 짝수열 : Trim	
	CDPointArray m_arrSangsePosCB;			// 가로보 배근상세		- x,y(평면위치)
	CDPointArray m_arrSangsePosST;			// 세로보 배근상세		- x,y(평면위치)
	CDPointArray m_arrSangsePosOBK;			// 외측가로보 배근상세  - x,y(평면위치)
	CVectorArray m_arrSangsePosDan;			// 단부 배근상세 그리는 위치       - x,y(평면위치), z:-1(시점), 1(종점)
	CVectorArray m_arrSangsePosCant;		// 캔틸레버부 배근상세 그리는 위치 - x,y(평면위치), z:-1(좌측), 1(우측)
	CDPointArray m_arrSangseIndexCB;		// 가로보 배근상세		- x:거더번호, y:가로보번호
	CDPointArray m_arrSangseIndexST;		// 세로보 배근상세		- x:거더번호, y:가로보번호
	CDPointArray m_arrSangseIndexOBK;		// 외측가로보 배근상세  - x:-1(좌측), 1(우측) y:외측가로보번호
	double       m_dDiaSangsePosCB;			// 가로보 상세원의 직경
	double       m_dDiaSangsePosST;			// 세로보 상세원의 직경
	double       m_dDiaSangsePosOBK;		// 외측가로보 상세원의 직경
	double       m_dDiaSangsePosDan;		// 단부 상세원의 직경
	double       m_dDiaSangsePosCant;		// 캔틸레버 상세원의 직경
	
	//////////////////////////////////////////////////////////////////////////
	// 평면배근실제위치 [ 0=상부, 1=하부 ]
	//////////////////////////////////////////////////////////////////////////
	vector<REBAR_PLAN> m_vPlanMainRebar[2];				// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanMainRebarLeft[2];			// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarRigh[2];			// DRAW_RIGH_SIDE

	vector<REBAR_PLAN> m_vPlanMainRebarSkewStt[2];		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanMainRebarSkewLeftStt[2];	// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarSkewRighStt[2];	// DRAW_RIGH_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarSkewEnd[2];		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanMainRebarSkewLeftEnd[2];	// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarSkewRighEnd[2];	// DRAW_RIGH_SIDE

	vector<REBAR_PLAN> m_vPlanSabogangStt[2];			// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanSabogangLeftStt[2];		// DRAW_LEFT_SIDE	
	vector<REBAR_PLAN> m_vPlanSabogangRighStt[2];		// DRAW_RIGH_SIDE
	vector<REBAR_PLAN> m_vPlanSabogangEnd[2];			// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanSabogangLeftEnd[2];		// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanSabogangRighEnd[2];		// DRAW_RIGH_SIDE

	vector<REBAR_PLAN> m_vPlanUnderSinchukStt;		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanUnderSinchukLeftStt;	// DRAW_LEFT_SIDE	
	vector<REBAR_PLAN> m_vPlanUnderSinchukRighStt;	// DRAW_RIGH_SIDE
	vector<REBAR_PLAN> m_vPlanUnderSinchukEnd;		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanUnderSinchukLeftEnd;	// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanUnderSinchukRighEnd;	// DRAW_RIGH_SIDE
	
	//////////////////////////////////////////////////////////////////////////
	// LB DECK
	//////////////////////////////////////////////////////////////////////////
	double m_dHeightLatticeBarDanbu;		// 단부 Lattice Bar 높이
	double m_dHeightLatticeBarGen;			// 일반부 Lattice Bar 높이
	long m_nOptionLBDeckTensionRebarCTC;	// 인장철근 CTC 옵션
	long m_nOptionLBDeckLatticeRebarCTC;	// Lattice Bar CTC 옵션
	double m_dLengthLBDeckDanbuSettle;		// 단부 가로보상면과 패널이 겹치는 범위

	CProgressCtrl m_progressCtrl;
	CStatusBar*	  m_pWndStatusBar;

// Implementation
public:
	void Serialize(CArchive& ar);
	void SerializePlan(CArchive& ar);

	//////////////////////////////////////////////////////////////////////////
	// 철근형태에 대한 정보생성
	//////////////////////////////////////////////////////////////////////////
	void MakeShapeMainRebar(long nSection);
	void MakeShapeMainRebarBySlabOutLine(long nSection, long nCycle, BOOL bUpper, vector<REBAR_SHAPE>& vRebarShape);
	void MakeShapeMainRebarLowerHasSlabHunch(long nSection, long nCycle, vector<REBAR_SHAPE>& vRebarShape);

	void MakeShapeMainRebarAtUpperGirderWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	void MakeShapeMainRebarAtUpperCbStgWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	void MakeShapeMainRebarAtUpperOutBkWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape);

	void AddRebarShapeToRebarShape(vector<REBAR_SHAPE>& vRebarShapeOrigin, vector<REBAR_SHAPE>& vRebarShapeToAdd);

	void MakeShapeBeryukRebar();
	void MakeShapeBeryukRebarUp(long nJongBasePos, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	void MakeShapeBeryukRebarDn(long nJongBasePos, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	//////////////////////////////////////////////////////////////////////////
	// 배력철근CTC 대한 정보생성
	//////////////////////////////////////////////////////////////////////////
	void MakeDimArrAtSection(long nSection, CDPointArray &xyArrDim, BOOL bUpper);
	void MakeDimArrAtSectionTU(long nSection, CDPointArray &xyArrDim, BOOL bUpper);	// TU거더의 배력철근 (현재 단부 하부만 생성함 081113)
	void MakeDimArrAtDanbu();	
	void MakeDimArrAtJijum();
	void MakeDimArrAtCenterStrength();		// 강도 설계법
	void MakeDimArrAtCenterExperience();	// 경험적 설계법
	double MakeDimArrAtCenterStrengthUp(long nSection, CDPointArray &xyArrDim);
	void MakeDimArrAtCenterStrengthDn(long nSection, CDPointArray &xyArrDim, double dExtLength);

	//////////////////////////////////////////////////////////////////////////
	// 철근제원에 대한 정보 계산
	//////////////////////////////////////////////////////////////////////////
	void MakeRebarJewon();					// 철근제원생성
	void MakeRebarJewon_Main_Danbu(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);
	void MakeRebarJewon_Main_GenNoHunch(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);
	void MakeRebarJewon_Main_GenLowerAtHunch(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);
	void MakeRebarJewon_Beryuk(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, long nCycle);// 철근제원생성 - 배력철근
	void MakeRebarJewon_Skew(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bSttSlab);				// 철근제원생성 - 사보강철근
	void MakeRebarJewon_SkewMainRebar(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bSttSlab);		// 철근제원생성 - 사각부 주철근
	void MakeRebarJewon_CrossBeam(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);		// 철근제원생성 - 가로보 보강철근(C1, C2)
	void MakeRebarJewon_Stringer(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);			// 철근제원생성 - 세로보 보강철근(S1, S2)
	void MakeRebarJewon_OutBrk(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);			// 철근제원생성 - 외측가로보 보강철근(OB1, OB2, OB3)
	void MakeRebarJewon_Etc(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);				// 철근제원생성 - 간격재, 전단보강철근, 배수구 보강철근
	void MakeRebarJewon_GuardFence(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bCalcMeter = FALSE);	// 철근제원생성 - 방호벽
	CRebar* GetRebarByName(CString szName);	// 철근의 m_dScription을 이용하여 철근을 구함

	//////////////////////////////////////////////////////////////////////////
	// 철근마크에 대한 정보 계산
	//////////////////////////////////////////////////////////////////////////
	void MakeRebarMarkPosPlan(long nJigan, double dScale);
	void MakeRebarMarkPosPlanMain(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos);
	void MakeRebarMarkPosPlanMainNoDiv(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos);	
	void MakeRebarMarkPosPlanMainDiv(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos);	
	void MakeRebarMarkPosPlanMainLowerEtc(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);	
	void MakeRebarMarkPosPlanSkew(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);		// 평면배근마크(사각부주철근, 사보강철근)
	void MakeRebarMarkPosPlanBeryukDanAtSkew(vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bSttSlab, BOOL bUpper, double dScale);	// 평면배근마크(배력철근)
	void MakeRebarMarkPosPlanBeryukDanAtNoSkew(vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bSttSlab, BOOL bUpper, double dScale);	// 평면배근마크(배력철근)
	void MakeRebarMarkPosPlanBeryukGen(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bUpper, double dScale);	// 평면배근마크(배력철근)
	void MakeRebarMarkPosPlanBeryukJijum(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bUpper, double dScale);	// 평면배근마크(배력철근)
	void MakeRebarMarkPosPlanBeryukSub(vector < pair<CString, CDPointArray > > &vMarkPos, CDPoint xySttRebar, CDPoint vRebar, CDPointArray xyArrDimMark, CString szMark, long nCycle, BOOL bUpper, double dScale);
	void MakeRebarMarkPosPlanCross(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, double dScale);		// 평면배근마크(가로보철근)	
	void MakeRebarMarkPosPlanStringer(long nJigan,  vector < pair<CString, CDPointArray > > &vMarkPos, double dScale);	// 평면배근마크(세로보철근)
	void MakeRebarMarkPosPlanStringerSub(vector < pair<CString, CDPointArray > > &vMarkPos, long nSangseST, long nStgAtTotal, double dScale);
	void MakeRebarMarkPosPlanOutBrk(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, double dScale);
	void MakeRebarMarkPosPlanSangseDanbu(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);
	void MakeRebarMarkPosPlanSangseCant(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);

	//////////////////////////////////////////////////////////////////////////
	// 철근이음장 위치
	//////////////////////////////////////////////////////////////////////////
	void MakeCycleMainRebar(long nSection);	
	void MakeCycleMainRebarReCalc(long nSection);		
	void MakeCycleBeryukRebar(CDoubleArray arrMomentLeft, CDoubleArray arrMomentRigh);	
	void MakeCycleBeryukRebar_LC_RC();	
	void MakeCycleMainRebar4Cy(long nSection);		
	void MakeCycleMainRebar1Cy(long nSection);		

	//////////////////////////////////////////////////////////////////////////
	// 철근간격 계산함수
	//////////////////////////////////////////////////////////////////////////
	CDPointArray CalcCTCMainRebarBase(long nGugansu, long nGugan, double dBaseLen);
	void CalcCTCMainRebar();
	void CalcCTCMainRebarBySlabHDanPos();	// 도로중심, 슬래브중심 주철근 간격계산
	void CalcCTCMainRebarByMaxRadius();		// 곡률반경이 큰쪽 주철근 간격계산
	void CalcCTCMainRebarForDim();			// 주철근 치수선계산
	void CalcCTCMainRebarAtSkewForDim();	// 사각부 철근간격계산
	void CalcCTCMainRebarAtSinchuk(BOOL bStt, CDPointArray &xyDimLeft, CDPointArray &xyDimRigh);
	void CalcCTCSabogangRebar();			// 사보강철근간격계산
	void CalcCTCMainRebarAtSkew();			// 사보강부 주철근 간격계산
	void CalcCTCSinchukRebar();				// 신축이음부 하부 주철근 간격계산
	void CalcCTCBogangRebarAtCrossBeam();	// 가로보 보강철근 간격 계산
	void CalcCTCBogangRebarAtStringer();	// 세로보 보강철근 간격 계산
	void CalcCTCBogangRebarAtOutBrk();		// 외측가로보 보강철근 간격 계산
	void CalcCTCDanHunchRebar();			// 단부 헌치 정착장	
	
	void CalcCTCMainRebarBySlabCentCTCBySlabUpper();	// 상부 중앙부 주철근간격기준 좌우측 주철근 간격 계산
	void CalcCTCMainRebarBySlabLeftRighCTCBySlabUpper(BOOL bLeftBase);	// 좌우측 주철근 간격기준 중앙부 주철근 간격계산

	//////////////////////////////////////////////////////////////////////////
	// 평면도 주철근 생성
	//////////////////////////////////////////////////////////////////////////
	void MakePlanMainRebar();
	void MakePlanMainRebarSkew();
	void MakePlanSabogang();
	void MakePlanUnderSinchuk();
	// 배력철근 거리(일반)	/ 배력철근 거리(중간지점부)==>  nRebarPos=0 : 상부, nRebarPos=1 : 하부 nRebarPos=2 : 신축이음부 하면
	void GetBeryukRebarDistOnCenter(CVectorArray *pArr, CPlateBasicIndex *pBxSttJijum, CPlateBasicIndex *pBxEndJijum, BOOL bUpper, long nDrawSide=DRAW_SIDEALL);
	void GetBeryukRebarDistOnJijum(CVectorArray *pArr, long nJijum, long nRebarPos, long nDrawSide, long nLRType=0);
	// 철근마크 위치를 구함(일반형태)
	CDPointArray GetMarkPosArr(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, double dRatio, BOOL bUpper);
	CDPointArray GetMarkPosArrSkewMain(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, BOOL bUpper, double dRatio = 0.5);
	// 철근마크 위치를 구함(사각부철근)
	CDPointArray GetMarkPosArrAtMainSkew(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, BOOL bUpper, BOOL bFromStt);
	CDPointArray GetMarkPosArrAtSangCant(CTwinVectorArray *tvArrPlane, double dLenToNearMidCTC, BOOL bUpper);
	CDPointArray GetMarkPosArrByLenFrStt(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, double dLenFrStt, BOOL bUpper);

	// 주철근 배열 구하기
	vector<REBAR_PLAN>* GetMainRebarArr	  (BOOL bUpper, long nDrawSide);
	// 사보강부 주철근 배열 구하기
	vector<REBAR_PLAN>* GetMainRebarArrAtSkew(BOOL bSttSlab, BOOL bUpper, long nDrawSide);
	// 사보강철근 배열 구하기
	vector<REBAR_PLAN>* GetSabogangArr		  (BOOL bSttSlab, BOOL bUpper, long nDrawSide);
	// 신축이음부 아래 주철근 구하기
	vector<REBAR_PLAN>* GetMainRebarArrUnderSinChuk(BOOL bSttSlab, long nDrawSide);
	// 가로보 헌치하단 보강 철근 배열(V형)
	vector<REBAR_PLAN>* GetBoganRebarArrAtCrossBeam(CDoubleArray &dDistArr, CPlateBasicIndex *pBx);
	// 외측가로보 헌치하단 보강 철근 배열(V형)
	void GetBoganRebarArrAtOutBrk(CDoubleArray &dDistArr, CPlateBasicIndex *pBx, BOOL bLeft);
	//////////////////////////////////////////////////////////////////////////
	// 공통계산함수
	//////////////////////////////////////////////////////////////////////////
	// 동일 선상에 있는 벡터제거
	void EraseSameVector(CDPointArray& xyArr);				
	// 철근의 CTC, 간격을 이용하여 길이를 구함.
	double  GetRebarLengthByPointArray(CDPointArray xyArr);		
	// 포인트배열을  폴리라인에 쏴서 교차점구함
	CDPointArray GetCDPointArrByPolyLine(CDPointArray xyArrOrigin, CDPointArray xyArrPoly, BOOL bVert);	
	// CDoubleArray을  폴리라인에 쏴서 교차점구함
	CDPointArray GetCDPointArrByDoubleLine(CDoubleArray dArrOrigin, CDPointArray xyArrPoly);
	// 철근생성시 길이 및 이음장 설정함.
	void SetLengthRebar(CRebar *pRB, long nJointType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);
	// 재료표 생성시 철근 Sorting
	void ShellSortRebarList(CTypedPtrArray <CObArray, CRebar*> *pArr, int (CRebar::*fcmp)(const CRebar*) const) const;
	// 철근의 단위중량 구하기
	double GetValueBasic(long nDia, long nType);
	// 기준선과 철근의 CTC를 이용하여 점철근의 위치를 구하기	
	CDPointArray MakePointRebarArr(CDPointArray *parrBase, CDPointArray *parrDot, BOOL bMove, BOOL bUpper);
	// xyArrDim을 이용하여 철근수를 구함
	long GetRebarSu(CDPointArray xyArrDim);
	// xyArrDim을 이용하여 철근수를 구함(dRange : 시작범위)
	long GetRebarSuAtRange(CDPointArray xyArrDim, double dRange);
	// CTC배열을 기준으로 시점에서 dOffsetStt만큼 떨어진 위치에서 CTC중간점이 가장 근접한 위치를 찾음.
	double GetLengthByCTCAndRebarsu(CDPointArray xyArrDim);
	// xyArrDim을 이용하여 길이를 구함.
	double GetPointByOffsetAndCTC(CDPointArray xyArrDim, double dOffsetStt);
	// CTC배열을 Cycle별로 구함
	CDPointArray GetMarkPosArrByCycle(CDPointArray xyArrMark, long nCycle, long nCyclesu);
	// 철근 추가
	BOOL AddRebar(int nType);
	// 철근 삭제
	BOOL DeleteRebar(CString strMark);
	// 철근 찾기
	int SearchRebar(CString strMark, CRebar* pRB);
	// 포인트배열중 원안에 있는것만...
	void GetPointArrInCircle(CDPointArray& xyArr, CDPoint xyCen, double dRad);
	// 좌측, 우측 철근배근간격을 이용하여 중앙의 철근배근간격을 계산함(중심위치)
	CDPointArray GetDimArrCenByLeftRight(CDPointArray &xyArrDimLeft, CDPointArray &xyArrDimRigh);
	// 보간함수
	double Bogan(double x1, double x2, double y1, double y2, double x);
	// 주철근상부 간격을 하부간격으로 결정시 사용
	void CalcCTCChildByCTCParent(CDPointArray &xyArrCTCParent, CDPointArray &xyArrCTCChild, double dOffsetStt, double dOffsetEnd);
	// 전체 마크에서 사이클별로 마크 추출
	CDPointArray GetDimArrByCycle(CDPointArray xyArrMark, long nCycle, long nCyclesu);
	//////////////////////////////////////////////////////////////////////////
	// 계산함수
	//////////////////////////////////////////////////////////////////////////
	// 단면별 종단위치에 따른 슬래브의 단면점 (좌측/상단이 원점)
	CDPointArray GetShapeSlabJongBasePos(long nJongBasePos);
	CDPointArray GetShapeSlabOnCB(long nG);
	// 단면별 횡단위치에 따른 사용철근 직경
	double GetRebarDiaBySection(long nSection, long nHDanPos, long nCycle, BOOL bUpper);
	// 단면별 종단위치에 따른 사용철근 직경
	double GetRebarDiaByJongBasePos(long nJongPos, long nJongBasePos, BOOL bUpper);
	// 횡단위치에서 사이클이 존재하는지?
	BOOL IsCycleValid(long nSection);
	// 종단위치에서 사이클이 존재하는지?
	BOOL IsCycleValidJong(long nJongBasePos);
	// 횡단위치에서 철근형태가 존재하는지?
	BOOL IsShapeValid(long nSection);
	// 종단위치에서 철근형태가 존재하는지?
	BOOL IsShapeValidJong(long nJongBasePos);
	// 주철근의 시작스테이션(신충이음부 및 피복은 고려안함)
	double GetSttStationMainRebar(long nPos, BOOL bUpper);
	// 단부와 일반부가 동일한 사이클인가?
	BOOL IsSameCycleDanGen();
	// 좌측, 우측, 중앙부 주철근이 동일한 철근인가?
	BOOL IsSameMainRebarLRC(BOOL bUpper, BOOL bDanbu);
	// 단부, 일반부가 동일한 철근인가?
	BOOL IsSameMainRebarDanGen(BOOL bUpper);
	BOOL IsSameMainRebarDanGen();
	BOOL IsSameMainRebarShapesuDanGen();
	// 슬래브좌측, 우측, 중앙부의 주철근 CTC 동일한가?
	BOOL IsSameMainRebarCTC(BOOL bDanbu, BOOL bUpper);
	// 종단기준위치의 슬래브의길이
	double GetLengthSlabTotal(long nJongBasePos);
	double GetLengthSlabTotalByDist(double dDist);
	// nJigan의 슬래브 길이
	double GetLengthSlab(long nJigan, long nJongBasePos, BOOL bConsiderCoverSd=FALSE);
	double GetLengthSlabByDist(long nJigan, double dDist, BOOL bConsiderCoverSd=FALSE);
	// nJigan까지의 슬래브 길이
	double GetLengthSlabAccuml(long nJiganTo, long nJongBasePos, BOOL bConsiderCoverSd=FALSE);
	double GetLengthSlabAccumlByDist(long nJiganTo, double dDist, BOOL bConsiderCoverSd=FALSE);
	// 마크위치를 구함
	CDPointArray GetArrMarkPosPlanByName(long nJigan, CString szName);
	// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray GetStationArrForMainRebarGugan(long nPosType=0);
	// 단부철근의 갯수를 구함.
	long GetRebarSuAtDanbu(BOOL bUpper, BOOL bStt);
	// 기준 CTC를 이용하여 원하는 위치의 CTC를 구함 (복합선형에서 사용함)
	CDPointArray GetDimArrAtMultiCurve(double dStaStt, CDPointArray xyArrDimSource, double dDistSource, long nCalcPos, BOOL bPositiveDir=TRUE);
	// 해당Section에서 주철근 조립단면 가이드라인을 생성
	CDPointArray GetXyArrGuideLineForMainRebar(long nSection, BOOL bUpper);
	// JongBasePos에서 배력철근 조립단면 가이드라인을 생성
	CDPointArray GetXyArrGuideLineForBeryukRebar(long nJongBasePos, BOOL bUpper);
	CDPointArray GetXyArrGuideLineForBeryukRebarOnCB(long nG, BOOL bUpper);
	// 외측가로보가 있나?
	BOOL IsBridgeHasOutBracket();
	BOOL IsBridgeHasOutBracket(long nSection, BOOL bLeft);
	// 슬래브의 헌치가 있나?
	BOOL IsBridgeHasSlabHunch();
	// 곡선교?
	BOOL IsBridgeHasCurveLine();
	// 가로보 있나?
	BOOL IsBridgeHasCrossBeam();
	BOOL IsBridgeHasCrossBeamHunch();
	// 세로보 있나? 
	BOOL IsBridgeHasStringer();
	// 사교인가 ? 
	BOOL IsSkew();
	// 슬래브 시점이 사교?
	BOOL IsSkewStt();
	// 슬래브 종점이 사교?
	BOOL IsSkewEnd();
	// 주철근에서 피복까지의 거리
	double GetLengthMainRebarToCover(BOOL bStt, long nJongBasePos, BOOL bUpper, double &dLengthMainRebarAtSkew, BOOL bNotBlock = FALSE);
	// 주철근이 시작되는 스테이션
	double GetStationMainRebarPos(BOOL bSttMainRebar, long nPosType=0) const;
	// 선형부터 슬래브 주철근 좌측, 우측 까지의 이격거리(수직)
	double GetSideBarDistFromCenterGir(long nSection, BOOL bLeft);
	// 라인이 현재 그리기 영역에 있는지 검사
	BOOL IsDrawMainRebarArea(CDPoint pt1, CDPoint pt2);
	// 포안트가 현재 그리기 영역에 있는지 검사
	BOOL IsDrawArea(CDPoint pt1);
	// 지간별 철근마크 위치를 구함
	CDPointArray ModifyMarkPosArrByJigan(long nJigan, CDPointArray xyArrMark, CTwinVectorArray *tvArrRebar);
	// Trim되어서 중간이 비어있는 xyArrMarkPos를 xyArrMarkPosStt와 xyArrMarkPosEnd로 분할함. 
	void DivideMarkPosAtJigan(CDPointArray* xyArrMarkPos, CDPointArray* xyArrMarkPosStt, CDPointArray* xyArrMarkPosEnd, long nJigan);
	// 슬래브의 면적
	double GetAreaSlab();
	// 지점위치에서 지점보강재 배근옵션에 따라 지점보강재 좌,우측 길이를 계산.
	void GetArrJointDisAtJijum(long nPier, CDPoint vAngStt, CDPoint vAngEnd, double dDisStt, double dDisEnd, double *dArrJoint);
	// 마크위치에서 슬래브 시점부나 종점부까지의거리를 계산하여 리턴함
	double GetLengthExtendToSlabSttEndFrPoint(CDPointArray xyArrMark, double dExtPlus, BOOL bFromSttMark, BOOL bSttSlabLine);
	// 마크위치에서 슬래브 중심트림구간 시점부나 종점부까지의 거리를 계산하여 리턴함
	double GetLengthExtendToSlabCenFrPoint(CDPointArray xyArrMark, long nJigan, double dExtPlus, BOOL bFromSttMark, BOOL bSttSlabCenLine);
	// 마크위치에서 슬래브 좌측끝단, 우측끝단까지의거리를 계산하여 리턴함
	double GetLengthExtendToSlabLeftRighFrPoint(CDPoint xyPoint, double dExtPlus, BOOL bLeftSlab);
	// 슬래브 횡단의 임의의 점에서 주철근의 방향벡터
	CDPoint GetSlopeAtHDanByPos(CDPointArray xyArrHDan, CDPoint xyPos);
	// 슬래브 횡단에서 위치별로 이음장 길이를 자동으로 구해줌
	double GetJointLengthAtHDan(long nSection, BOOL bUpper, long nHDanPos);
	// 슬래브 종단에서 위치별로 이음장 길이를 자동으로 구해줌
	double GetJointLengthAtJDan(long nJongPos, BOOL bUpper, long nJongBasePos);
	// xyArrPoly을 따라 가면서 길이의 중간위치점을 구함
	CDPoint GetMidPointAtPolyLine(CDPointArray xyArrPoly);
	// 철근제원의 총합을 구한다.
	double GetRebarWeightTotal(BOOL bMainRebar, BOOL bGuardRebar);
	// 종단면에서의 해당 지간의 시작점, 끝점을 리턴한다. 
	BOOL GetPosJiganAtJong(int nJigan, double& dStt, double& dEnd, long nJongBasePos);
	// 철근수를 사이클별로 나눈다.	
	double GetRebarsuAtCycle(long nRebarsu, long nCycle, long nTotalCycle);
	// 실제 철근이 있는 배력사이클 수를 구함.
	long GetsuRealCycle(long nJongBasePos, BOOL bUpper);
	// 단경간 = 일반부/단부, 다경간 = 일반부/지점부 배수구하기.
	long GetBesu(long nJongBasePos, BOOL bUpper);
	// 평면도에서 tVector의 중심위치가 횡단방향에서 위치하는 비율을 계산함.
	double GetRatioOfMidPosRebarAtHDan(CTwinVector& tVector, double dSta, BOOL bUpper, double &dRatioPre);
	// 배력철근의 CTC를 구함.
	CDPointArray *GetCTCBeryukRebar(long nSection, BOOL bUpper);	
	// 횡단위치에서 배력철근의 CTC 저장함
	void SetCTCBeryukRebar(CDPointArray& xyArrDim, long nSection, BOOL bUpper);
	// 종단의 전체 주철근 및 사보강 철근 배치간격을 계산
	CDoubleArray GetRebarPosAtJongBasePos(BOOL bUpper, long nJongBasePos, BOOL bRealPos, long nRangeRebar=0);
	// 종단면 기준위치에서 철근배근 치수
	CDPointArray GetDimTotalAtJongBasePos(BOOL bUpper, long nJongBasePos, BOOL bRealDim);
	// 신축이음부의 폭(종단기준위치에서 실제 길이)
	double GetWidthBlockOut(BOOL bSttSlab, long nJongBasePos);
	// 유간의 폭
	double GetWidthUgan(BOOL bSttSlab, long nJongBasePos);
	// 측면피복두께
	double GetCoverSdToModify(BOOL bSttSlab, BOOL bLeftCalcPos);
	// 신축이음부에서 측면피복두께
	double GetCoverSdToModifyAtExp(BOOL bSttSlab, BOOL bLeftCalcPos, CDPoint xyDimStt);
	// 캔틸레버부 직경적용 철근의 개수
	long GetsuBeryukAtCantilever(long nPos, BOOL bDanbu, BOOL bUpper);
	// 횡단위치의 캔틸레버부철근 타입
	long GetTypeBeryukAtCantilever(BOOL bDanbu, BOOL bUpper);
	// 종단위치에서 배력철근이 설치되는 길이
	double GetLengthRebarRangeAtJongPos(long nJongBasePos, BOOL bUpper);
	/// 횡단위치에서 스테이션
	double GetStationBySection(long nSection);
	// 횡단위치에서 철근사이클을 제거
	void RemoveRebarCycleBySection(long nSection);
	// 횡단위치에 철근사이클추가
	void AddRebarCycleBySection(long nSection, REBAR_CYCLE stRebarCycle);
	// 횡단위치에서 겹이음수
	long GetSizeHDanJointBySection(long nSection, long nCycle, BOOL bUpper);
	// 횡단위치에서 사이클가져오기
	REBAR_CYCLE* GetRebarCycleBySection(long nSection, long nCycle);
	// 횡단위치에서 사이클가져오기
	vector<REBAR_CYCLE>* GetRebarCycleBySection(long nSection);
	// 횡단위치의 사이클수
	long GetRebarCycleSuBySection(long nSection);
	// 횡단위치의 철근형태제거
	void RemoveRebarShapeBySection(long nSection);
	// 종단위치의 철근형태제거
	void RemoveRebarShapeByJongBasePos(long nJongBasePos);
	// 횡단위치에 철근형태추가
	void AddRebarShapeArrAtSection(long nSection, vector<REBAR_SHAPE> vRebarShape);
	// 종단위치에 철근형태추가
	void AddRebarShapeArrAtJongBasePos(long nJongBasePos, vector<REBAR_SHAPE> vRebarShape);
	// 종단위치에서 철근사이클을 제거
	void RemoveRebarCycleByJongBasePos(long nJongBasePos);
	// 종단위치에 철근사이클추가
	void AddRebarCycleByJongBasePos(long nJongBasePos, REBAR_CYCLE stRebarCycle);
	// 종단위치에 철근사이클추가
	void AddRebarCycleByJongBasePos(long nJongBasePos, CDoubleArray& arrUpLeftLong, CDoubleArray& arrDnLeftLong
									, CDoubleArray& dArrDiaUpLeft, CDoubleArray& dArrDiaDnLeft);
	// 종단위치에서 사이클가져오기
	REBAR_CYCLE* GetRebarCycleByJongBasePos(long nJongBasePos, long nCycle);
	// 종단위치에서 사이클가져오기
	vector<REBAR_CYCLE>* GetRebarCycleByJongBasePos(long nJongBasePos);
	// 종단위치의 사이클수
	long GetRebarCycleSuByJongBasePos(long nJongBasePos);
	// 횡단위치에서 사이클별 철근형태
	vector<REBAR_SHAPE>* GetRebarShapeArrBySection(long nSection, long nCycle);
	// 종단위치에서 사이클별 철근형태
	vector<REBAR_SHAPE>* GetRebarShapeArrByJongBasePos(long nJongBasePos, long nCycle);
	// 횡단위치에서 사이클별 철근형태를 상하부 구분해서 새로 만듬
	vector<REBAR_SHAPE> GetNewRebarShapeArrBySection(long nSection, long nCycle, BOOL bUpper);
	// 종단위치에서 사이클별 철근형태를 상하부 구분해서 새로 만듬
	vector<REBAR_SHAPE> GetNewRebarShapeArrByJongBasePos(long nJongBasePos, long nCycle, BOOL bUpper);
	// 종단위치에서 사이클별 철근형태를 가져옴
	CDPointArray GetRebarShapeByJongBasePos(long nJongBasePos, long nCycle, CString szName);	
	// 주철근 돌출길이
	double GetLengthMainDolchul(long nSection, BOOL bUpper, BOOL bLeft);
	// 겹이음길이 계산을 위한 주철근의 기본간격
	double GetCTCMainForJoint(BOOL bUpper, long nHDanPos, long nSection);
	// 겹이음길이 계산을 위한 배력철근의 기본간격
	double GetCTCBeryukForJoint(BOOL bUpper, long nHDanPos, long nJongBasePos);
	// vCycleSource를 vCycleTarget에 dRatio만큼 이음길이를 변화시켜 대입.
	void SetCycleToCycle(vector<REBAR_CYCLE> &vCycleSource, vector<REBAR_CYCLE> &vCycleTarget, double dRatio=1); 
	// arrCTCSource를 arrCTCTarget에 dRatio만큼 CTC를 변화시켜 대입.
	void SetCTCArrToCTCArr(CDPointArray &arrCTCSource, CDPointArray &arrCTCTarget, double dRatio=1); 
	// REBAR_PLAN 배열중에서 nCycle이고 nShape인  CTwinVector배열을 구함 
	void GetTwinVectorArrByCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycle, long nShape);
	void GetTwinVectorArrAtRebarPlan(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn);
	void GetTwinVectorArrByCycleAndName(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycle, CString sName);
	void GetTwinVectorArrByMultiCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycleStt, long nCycleEnd, long nShape);
	void GetTwinVectorArrByMultiCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycleStt, long nCycleEnd, CString sName);
	// 최대 CTC를 구함
	double GetMaxCTC(CDPointArray &xyArrDim);
	// xyArrPoint중에서 짝수이고 nCount수 만큼 생성(마크틱위치생성시 사용함)
	CDPointArray ModifyPointByCount(CDPointArray &xyArrPoint, long nCount);
	// 확폭교량에서 배력철근 CTC의 기준이 되는 시종점위치
	void GetXyBasePointPlanEx(CDPoint Base[9], CDPoint Real[4], BOOL bIncludeCover, BOOL bUpper, BOOL bIncludeSttEndCover=FALSE);
	// 종단기준위치에서 종단방향으로 측면피복의 실제길이(확폭고려됨)
	double GetLengthCoverSd(BOOL bSttSlab, long nJongBasePos=JONGBASEPOS_SLABCENTER);	
	// 중앙부 배력철근 간격생성시 사용함(확폭에서만)
	void ModifyDimArr(CDPointArray &arrDim);
	// 단부 하부 배력철근2를 설치하나?
	BOOL IsInstallBeryukRebarAtDanHunch(long nJongBasePos, BOOL bSttSlab, long nCycle);
	// 단면별 해당 Bx
	CPlateBasicIndex* GetBxBySection(long nSection, long nG=-1);		
	// 슬래브의시작부 ~ 테이퍼 시작부	
	double GetLengthSlabToTaper(BOOL bSttSlab, long nJongBasePos);
	double GetLengthSlabToTaperByDist(BOOL bSttSlab, double dDist);
	// 거더상의 지간길이
	double GetLengthJiganByGirder(long nG, long nJigan);
	// 철근의 CTC를 이용하여 철근의 갯수구하기
	long GetSuRebarArray(CDPointArray *xyArr, BOOL bIncludeCover);
	// 슬래브 평면에서 세로보의 길이를 구함
//	double GetStgLengthAtSlab(CStringer *pStg);
	// 가로보 철근 길이 구하기
	double GetLengthCrossBeamHunchRebar(CPlateBasicIndex *pBx, BOOL bUpper);
	// 세로보 철근길이 구하기
	double GetLengthStringerHunchRebar(CPlateBasicIndex *pBx, long nStg, BOOL bUpper) const;
	// 외측가로보 철근길이 구하기
	double GetLengthOutBrkHunchRebar(CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft);
	// CTC배열을 기준으로 시점에서 dOffsetStt만큼 떨어진 위치에서 CTC중간점이 가장 근접한 위치를 찾음.
	double GetNearstMidPointAtCTCArr(CDPointArray xyArrDim, double dOffsetStt);
	// 평면도에서 철근마크가 위치하는 비율을 계산함.
	double GetRatioOfPlanMark(CDPointArray xyArr, BOOL bUpper, double &dRatioPre);
	//Stud의 제일 처음 위치 리턴(입력용)
	CDPointArray GetPosFirstStud();							
	// PointArray의 간격을 반환한다.
	vector<double> GetLenArrFromLineArr(CDPointArray ptArr);
	// 스트링거가 테이퍼부와 만나는지 검사
//	void IsStringerMatchAtTaper(CStringer *pStg, BOOL &bMatchStt, BOOL &bMatchEnd, CDPoint &xyMatchStt, CDPoint &xyMatchEnd);
	// m_arrStringer에서 해당 Bx의 세로보 위치를 구함.
//	long GetStgNoByBx(CPlateBasicIndex *pBx, long nStg);
	// nStgAtTotal을 이용하여 세로보를 구함.
//	CStringer* GetStgByStgNo(long nStgAtTotal);
	// 슬래브 좌/우측 철근 간격을 기준으로 우/좌측 철근간격을 계산한다.
	double GetCTCBySlabLeftRighRatio(BOOL bCalcLeft, CDPointArray &xyArrSourceCTC, double dDataNew, long nGugan, BOOL bIncludeSinchuk);
	// SECTION_CEN 에서 주철근 Shape의 수
	long GetsuMainRebarShapeAtCen();
	CDPointArray GetXyArrSlabEnd(long nG);
	CDPointArray GetXyArrSlabHunch(long nG, CPlateBasicIndex *pBxCB);
	double GetLengthSlabEndToCBCen(long nG, CPlateBasicIndex *pBxCB);
	double GetDistByJongBase(long nJongBasePos);
	CDPointArray GetXyArrPoint(CString sRebarName, vector<REBAR_SHAPE>& vArrShape);
	long GetNoShapeAtHDanShapeArr(CString sRebarName, vector<REBAR_SHAPE>& vArrShape);
	long GetRebarsuAtPlanArr(vector<REBAR_PLAN> *pvPlan, long nCycle, long nShape, long nPos);
	BOOL IsInstallBeryukRebarAtCenter(long nCycle);
	BOOL IsPehapCycle(long nJongBasePos, long nCycle);
	BOOL IsPointInJigan(CDPoint xyCen, long nJigan);
	BOOL IsPointInCircle(CDPoint xyPos, CDPoint xyCen, double dRad);
	long GetXyPosIDWhereInSlabHDan(CDPoint xyPos);
	
	vector<CDoubleArray> GetJiganLenArr();
	CDPointArray GetDanbuJointPos();
	// 단부 헌치 정착장 3~4개 정도 들어가는 주철근 위치
	CDPointArray GetXyArrRebarJongPosAtDanHunch(BOOL bSttSlab, long nJongBasePos);
	CDPointArray GetXyArrJointPosAtJongBasePos(long nJongBasePos, long nCycle, BOOL bUpper);
	CDPoint GetXyPosByTwinVectorAndDist(CTwinVector tVector, double dDist);
	double GetDistByXyPos(CDPoint xyPos);
	double GetLengthBySlabEndRadius(double dStaNear, long nJongBasePosBase, long nJongBasePosTarget, double dBaseLen);
	CDPoint GetCTCRebarAtCB(CPlateBasicIndex *pBx, long &nCount);
};

#endif // !defined(AFX_ADECKDATA_H__D5E29F58_6D9A_484D_99E9_80429B01650A__INCLUDED_)
