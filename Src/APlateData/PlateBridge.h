// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Bridge.h: interface for the CPlateBridge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEBRIDGE_H__5F50DD8D_BE7B_4FDB_A781_1D4BA535B029__INCLUDED_)
#define AFX_PLATEBRIDGE_H__5F50DD8D_BE7B_4FDB_A781_1D4BA535B029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		G_F_U				0 	//일반 	상판
#define		G_F_L				1	//일반 	하판
#define		G_W					2	//일반 	복부판
#define		G_H_L				3	//일반 	수평보강재
#define		G_H_R				4	//일반 	수평보강재

#define		V_ST				5	// 수직보강재
#define		V_J_ST				6	// 지점부 수직보강재(지점보강재)

#define		VB_UB				7	// VBracing 상현재
#define		VB_LB				8	// VBracing 하현재
#define		VB_CR				9	// VBracing 사재
#define		VB_GU				10	// VBracing 가세트
#define		VB_BOLT				11	// VBracing 볼트

#define		HB_B				12	// HBracing 빔
#define		HB_GU				13	// HBracing 빔 가세트
#define		HB_BOLT				14	// HBracing 볼트
 
#define		SP_U_U				15	//상부 현장이음	외부 포장면
#define		SP_U_L				16	//상부 현장이음	내측 이음판
#define		SP_U_FILL			17	//상부 현장이음 필러플레이트
#define		SP_U_BOLT			18	//상부 현장이음	볼트&너트
#define		SP_L_L				19	//하부 현장이음	외측 이음판
#define		SP_L_U				20	//하부 현장이음	내측 이음판
#define		SP_L_FILL			21	//하부 현장이음 필러플레이트
#define		SP_L_BOLT			22	//하부 현장이음	볼트&너트
#define		SP_W_MO				23	//복부판 현장이음 모멘트 이음판(외측)
#define		SP_W_SH				24	//복부판 현장이음 전단 이음판(외측)
#define		SP_W_FILL			25	//복부판 현장이음 필러플레이트
#define		SP_W_BOLT			26	//복부판 현장이음 볼트&너트

#define		CP_W				27	//가로보 지점부	복부판
#define		CP_F_U				28	//가로보 지점부	상판
#define		CP_F_L				29	//가로보 지점부	하판
#define		CP_STEEL			30	//가로보 지점부 형강
#define		CP_BR_W				31	//가로보 지점부	브라켓 복부판
#define		CP_BR_F_U			32	//가로보 지점부	브라켓 상판
#define		CP_BR_F_L			33	//가로보 지점부	브라켓 하판
#define		CP_BR_GA			34	//가로보 지점부	브라켓 보강재
#define		CP_BR_GA_FA			35	//가로보 지점부	브라켓 보강판
#define		CP_V				36	//가로보 지점부	수직보강재
#define		CP_H				37	//가로보 지점부	수평보강재
#define		CP_SP_U_U			38	//가로보 지점부	상부 이음판(상면)
#define		CP_SP_U_L			39	//가로보 지점부	상부 이음판(하면)
#define		CP_SP_L_U			40	//가로보 지점부	하부 이음판(상면)
#define		CP_SP_L_L			41	//가로보 지점부	하부 이음판(하면)
#define		CP_SP_W_MO			42	//가로보 지점부	복부판 이음판 모멘트
#define		CP_SP_W_SH			43	//가로보 지점부	복부판 이음판 전단
#define		CP_SP_W_FH			44	//Filler 두께가 9mm가 넘을경우 이음판 추가...
#define		CP_BOLT				45	//가로보 지점부	볼트&너트

#define		CG_W				46	//가로보 일반부	복부판
#define		CG_F_U				47	//가로보 일반부	상판
#define		CG_F_L				48	//가로보 일반부	하판
#define		CG_STEEL			49	//가로보 일반부 형강
#define		CG_BR_W				50	//가로보 일반부	브라켓 복부판
#define		CG_BR_F_U			51	//가로보 일반부	브라켓 상판
#define		CG_BR_F_L			52	//가로보 일반부	브라켓 하판
#define		CG_BR_GA			53	//가로보 일반부	브라켓 보강재
#define		CG_BR_GA_FA			54	//가로보 일반부	브라켓 보강판
#define		CG_V				55	//가로보 일반부	수직보강재
#define		CG_H				56	//가로보 일반부	수평보강재
#define		CG_SP_U_U			57	//가로보 일반부	상부 이음판(상면)
#define		CG_SP_U_L			58	//가로보 일반부	상부 이음판(하면)
#define		CG_SP_L_U			59	//가로보 일반부	하부 이음판(상면)
#define		CG_SP_L_L			60	//가로보 일반부	하부 이음판(하면)
#define		CG_SP_W_MO			61	//가로보 일반부	복부판 이음판	모멘트
#define		CG_SP_W_SH			62	//가로보 일반부	복부판 이음판 전단
#define		CG_SP_W_FH			63	//Filler 두께가 9mm가 넘을경우 이음판 추가...
#define		CG_BOLT				64	//가로보 일반부	복부판 볼트&너트

#define		ST_W				65	//세로보	복부판
#define		ST_F_U				66	//세로보	상판
#define		ST_F_L				67	//세로보	하판
#define		ST_BR_W				68	//세로보	브라켓 복부판
#define		ST_BR_F_U			69	//세로보	브라켓 상판
#define		ST_BR_F_L			70	//세로보	브라켓 하판
#define		ST_BR_GA			71	//세로보	브라켓 보강재
#define		ST_BR_GA_FA			72	//세로보	브라켓 보강판
#define		ST_V				73	//세로보	수직보강재
#define		ST_H				74	//세로보	수평보강재
#define		ST_SP_U_U			75	//세로보	상부 이음판(상면)
#define		ST_SP_U_L			76	//세로보	상부 이음판(하면)
#define		ST_SP_L_U			77	//세로보	하부 이음판(상면)
#define		ST_SP_L_L			78	//세로보	하부 이음판(하면)
#define		ST_SP_W				79  //세로보	복부판 이음판
#define		ST_BOLT				80	//세로보	볼트&너트

#define		WI_P_W				81	//외측가로보 지점부	복부판
#define		WI_P_F_U			82	//외측가로보 지점부	상판
#define		WI_P_F_L			83	//외측가로보 지점부	하판
#define		WI_P_BR_W			84	//외측가로보 지점부	브라켓 복부판
#define		WI_P_BR_F_U			85  //외측가로보 지점부	브라켓 상판
#define		WI_P_BR_F_L			86	//외측가로보 지점부	브라켓 하판
#define		WI_P_BR_GA			87	//외측가로보 지점부	브라켓 보강재
#define		WI_P_BR_GA_FA		88	//외측가로보 지점부	브라켓 보강판
#define		WI_P_V				89	//외측가로보 지점부	수직보강재
#define		WI_P_H				90	//외측가로보 지점부	수평보강재
#define		WI_P_SP_U_U			91	//외측가로보 지점부	상부 이음판(상면)
#define		WI_P_SP_U_L			92	//외측가로보 지점부	상부 이음판(하면)
#define		WI_P_SP_L_U			93	//외측가로보 지점부	하부 이음판(상면)
#define		WI_P_SP_L_L			94	//외측가로보 지점부	하부 이음판(하면)
#define		WI_P_SP_W			95	//외측가로보 지점부	복부판 이음판
#define		WI_P_BOLT			96	//외측가로보 지점부	볼트&너트

#define		WI_G_W				97	//외측가로보 일반부	복부판
#define		WI_G_F_U			98	//외측가로보 일반부	상판
#define		WI_G_F_L			99	//외측가로보 일반부	하판
#define		WI_G_BR_W			100	//외측가로보 일반부	브라켓 복부판
#define		WI_G_BR_F_U			101	//외측가로보 일반부	브라켓 상판
#define		WI_G_BR_F_L			102	//외측가로보 일반부	브라켓 하판
#define		WI_G_BR_GA			103	//외측가로보 일반부	브라켓 보강재
#define		WI_G_BR_GA_FA		104 //외측가로보 일반부	브라켓 보강판
#define		WI_G_V				105	//외측가로보 일반부	수직보강재
#define		WI_G_H				106 //외측가로보 일반부	수평보강재
#define		WI_G_SP_U_U			107 //외측가로보 일반부	상부 이음판(상면)
#define		WI_G_SP_U_L			108	//외측가로보 일반부	상부 이음판(하면)
#define		WI_G_SP_L_U			109	//외측가로보 일반부	하부 이음판(상면)
#define		WI_G_SP_L_L			110	//외측가로보 일반부	하부 이음판(하면)
#define		WI_G_SP_W			111	//외측가로보 일반부	복부판 이음판
#define		WI_G_BOLT			112	//외측가로보 일반부	볼트&너트

#define		EN_W				113	//외측빔 	복부판
#define		EN_F_U				114	//외측빔 	상판
#define		EN_F_L				115	//외측빔 	하판
#define		EN_BR_W				116	//외측빔 	브라켓 복부판
#define		EN_BR_F_U			117	//외측빔 	브라켓 상판
#define		EN_BR_F_L			118	//외측빔 	브라켓 하판
#define		EN_V				119 //외측빔 	수직보강재
#define		EN_H				120	//외측빔 	수평보강재
#define		EN_SP_U_U			121	//외측빔 	상부 이음판(상면)
#define		EN_SP_U_L			122	//외측빔 	상부 이음판(하면)
#define		EN_SP_L_U			123	//외측빔 	하부 이음판(상면)
#define		EN_SP_L_L			124	//외측빔 	하부 이음판(하면)
#define		EN_SP_W				125	//외측빔 	복부판 이음판
#define		EN_BOLT				126	//외측빔 	볼트&너트

#define		GI_SB				127	//기타부재	전단연결재
#define		GI_SQ				128	//기타부재	전단연결재 각형
#define		GI_SA				129	//기타부재	SLAB ANCHOR
#define		GI_SO				130	//기타부재	솔플레이트 
#define		GI_FA				131	//기타부재	FACE PLATE
#define		GI_LP				132	//기타부재	들고리
#define		GI_LP_R				133	//들고리 링
#define		GI_LP_PL			134	//들고리 보강재
#define		GI_JA_L				135	//기타부재	잭업보강재 좌측
#define		GI_JA_R				136	//기타부재	잭업보강재 우측
#define		GI_JA_SA			137	//기타부재	잭업 솔플레이트
#define		GI_ZST				138	//기타부재	낙교방지책
#define		GI_HG				139	//기타부재	유지보수용 동바리

#define		TOTAL_PIECE_VALUE	140	//총 개수

// ======================= 용접에 대한 부분 정의 =============================
#define		WELD_FIELDU			200
#define		WELD_FIELDL			201
#define		WELD_FIELDWEB		202
#define		TOTAL_WELD_VALUE	3
// ===================== 평면도면용 복부판 현장이음&필러 =====================
#define		SP_W_PLAN			1000	//복부판 현장이음 평면
#define		SP_W_PLAN_F			1001	//복부판 현장이음 평면필러

const long 	KYUNGGANSUMAX		= 40;					// 최대 지간수
const long	JIJUMSUMAX			= KYUNGGANSUMAX+1;		// 최대 지점수
const long  MAXSTRING			= 5;					// 최대 스트링거수

//////////////////////////////////////////////////////////////////////////
// static const
static const long STT_POS = 0;
static const long END_POS = 1;
static const long CENTERGIRDER = -1;

const long EQUAL_VALUE_PLATE = 6;

//////////////////////////////////////////////////////////////////////////
// class CPlateBridge
#define  GIRDERSUMAX    20
#define  JACKUPSUMAX    6
#define  HDANLINESU		50
#define  MAX_TENSION	40

enum { SP_WEB_FL, SP_WEB_MO, SP_WEB_FLMO };
enum { PLATE_GIR, SOSU_GIR, TU_GIR };
enum { LEFT, RIGHT, COUNT };
enum { UP, DOWN };
enum { FRM_UPPER=0, FRM_LOWER=1, FRM_WEB=2};

struct _TENSION {
	double m_dL;
	double m_dH;
	double m_dR;
	long m_nType;
};

struct _BINDCONC {
	double m_dWidth;	// 폭
	double m_dHeight;	// 높이
	double m_dDeep;		// 매입깊이
	double m_dDis;		// 이격거리
};

class CLineInfo;
class CPlateGirderApp;
class CCentSeparation;
class CPierInfo;

//typedef CTypedPtrArray <CObArray, CPlateGirderApp*> CPlateGirderArray;

//거더, 방호벽, 피어 ---- 생성
class AFX_EXT_CLASS  CPlateBridge : public CObject 
{
// Constructor
public:
	CPlateBridge(CLineInfo* pLine = NULL);
	virtual ~CPlateBridge();

// Attributes
protected:	
	CPlateGirderApp	*m_pCRGirder;				// 가상 중심거더
	CTypedPtrArray <CObArray, CPlateGirderApp*>   m_pGirder;				// 거더 배열 객체
	CTypedPtrArray <CObArray, CCentSeparation*>   m_pGuardWall;//방호벽 

	long	m_nQtyHDanNode						;	// 슬래브 제원
	double	m_dLengthHDan[HDANLINESU]			;	// 차선 간격(좌측)		
	long	m_nValueTypeHDan[HDANLINESU][3]		;	// 종류

public:
	long	GetGuardWallSu() const { return m_pGuardWall.GetSize(); }
	long	GetGirdersu()    const { return m_pGirder.GetSize();}
	long	GetNumHaiseokCombo() 
	{
		//무조건 합성형으로(비합성 지원안함) 
		m_NumHaiseokCombo = 0; 
		return m_NumHaiseokCombo;
	}
	
	CCentSeparation *GetGuardWall(long nGuard) const;
	CCentSeparation *GetCurGuardWall(long nPlace) const;	// 좌, 우, 중앙부 방호벽
	BOOL	IsTypeGuardFenceDom(long nHDan) const;
	long	GetGuardFenceType(long nHDan) const; 
	long	GetValueTypeHDan(long nHDan, long nTypeHDan) const; 
	long	GetNumGuardFenceFrLeft(long nHDan) const;

	CPlateGirderApp* GetGirder(long nG)		   const
	{
		if(nG < 0 || nG >= m_pGirder.GetSize()) return m_pCRGirder;	
		return m_pGirder[nG];
	}

	void SetNumHaiseokCombo(long nNumber) {m_NumHaiseokCombo = nNumber; }
public:
	CLineInfo *m_pLine;
	CPierInfo *m_PierInfo;

	CString	m_sNameBridge					;	//[기본 입력]
	CString	m_sNameOwner					;	// 발주처
	CString	m_sNameConstructor				;	// 시공사		
	CString	m_sNameAssembler				;	// 제작사
	CString	m_sNameSite						;	// 현장명
	CString	m_sNameEngineering				;	// 설계사
	BOOL    m_bUseLine						;   // 선형사용유무
	double  m_dBridgeWidth					;	// 교폭
	double  m_dDisLineToBridgeCen			;	// 선형에서 슬래브중심까지의 거리
	double  m_dGirderSpace					;	// 거더간격
	long    m_nLineType                     ;   // 선형타입(곡선, 직선)

	CString	m_sDeungeub						;	//설계등급
	long	m_nTypeAbutOrPier[2]			;	//시종점부 교대 or교각		  
	double	m_dStationBridgeStt				;	//교량시점 스테이션
	double	m_dLengthTotalSlab				;	//슬래브 길이
	long	m_nQtyJigan						;	//지간수
	double	m_dWidthSlabLeft				;	//좌측캔틸레버폭
	double	m_dWidthSlabRight				;	//우측캔틸레버폭
	CDoubleArray m_dHunchArray				;	//슬래브 헌치 저장	
	CDPointArray m_arrTendon		        ;	// x: Tendon 위치 스테이션 (선형중심), y: 고정단 위치 0=좌측, 1=우측 2=지그재그
	CLongArray m_arrTendonDom				;	//도면에서의 강선 배치 형태 정보
	long    m_nTendonFix					;   // 0 : 슬래브좌측, 1 : 슬래브우측
	long	m_nTendonDomFix					;	// 0 : 슬래브좌측, 1 : 슬래브우측, 2 : 지그재그

	long	m_nSlabTensionSize				;	// 배치구간 개수
	struct _TENSION m_SlabTension[MAX_TENSION];	// 배치구간 최대 40개 (20거더 기준 39개 확인)
	double	m_dHeightLeftTension			;	//강선배치 좌측기준
	double	m_dHeightRightTension			;	//강선배치 우측기준
	double  m_dLengthGinjangdan				;	// 긴장단 길이
	double  m_dLengthJungchakdan			;	// 정착단 길이
	double  m_dLengthSecMove				;	// 고정단 단부 이격거리..070208일자 추가.....KB...
	double  m_dLengthSecMoveEnd				;	// 긴장단 단부 이격거리..070208일자 추가.....KB...
	double  m_dLengthDuctOffset				;   // 고정단 덕트 옵셋거리..

	double	m_dRoundCrossUpper				;	//가로보 Rounding 처리
	double	m_dRoundChipCrossUpper			;
	double	m_dRoundCrossLower				;
	double	m_dRoundChipCrossLower			;
	double	m_dRoundStringerUpper			;
	double	m_dRoundChipStringerUpper		;	//세로보상부틈새간격
	double	m_dRoundStringerLower			;
	double	m_dRoundChipStringerLower		;	//세로보하부틈새간격

	BYTE	m_cBracketForm					;	// 외측가로보 유무
	BYTE	m_cBracketType					;	// 외측가로보 설치( 0 : 다이아프램 위치,1 : 수직보강재 2번째 )
	BYTE	m_bLeftBracket					;	// 좌측 외측가로보 유무
	BYTE	m_bRightBracket					;	// 우측 외측가로보 유무
	BYTE	m_cBrBeamType					;	// 외측빔 종류
	BYTE	m_bBracketBeamSpliceApply		;	// 외측빔 현장이음 분할 변수
	double	m_dRoundBracketUpper			;
	double	m_dRoundChipBracketUpper		;
	double	m_dRoundBracketLower			;
	double	m_dRoundChipBracketLower		;

	struct	_BINDCONC m_BindConc			;	//구속콘크리트
	CString m_strJijumName[JIJUMSUMAX]		;	//지점이름(단면제원구성에서 저장)
	double	m_dLengthJigan[KYUNGGANSUMAX]	;	//지간길이
	double	m_dLengthUganStt				;	//시점 유간 거리
	double	m_dLengthSlabGirderStt			;	//시점슬래브와 거더끝단과의 거리
	double	m_dLengthGirderShoeStt			;	//거더 끝단에서 슈까지의 거리
	double	m_dLengthGirderShoeEnd			;	//
	double	m_dLengthSlabGirderEnd			;	//종점슬래브와 거더종점과의 간격
	double	m_dLengthUganEnd				;	//종점 유간 거리
	CDPoint m_vAngJijum[JIJUMSUMAX]			;	//지점각도

	BYTE	m_cOffSetStt					;	//시점 옵셋
	BYTE	m_cOffSetEnd					;	//종점 옵셋
	double	m_dDisExtLineSttLeft			;	//확폭일 경우에만 사용
	double	m_dDisExtLineEndLeft			;	//확폭일 경우에만 사용
	double	m_dDisExtLineSttRight			;	//확폭일 경우에만 사용
	double	m_dDisExtLineEndRight			;	//확폭일 경우에만 사용
	BYTE	m_cDirShoe						;	//슈 각도->접선, 각도에 직각, 현방향, 고정슈방향
	BYTE	m_cDirVStiffJijum				;	//지점보강재 각도->접선, 각도에 직각, 현방향, 고정슈방향
	
	double	m_dLengthShoeToTaperStt			;	//슈에서 테이퍼시작부까지[시점제원 입력]
	double	m_dLengthTaperToTaperStt		;	//테이퍼 길이
	double	m_dHeightTaperStt				;	//테이퍼 높이
	double	m_dLengthShoeToTaperEnd			;	//슈에서 테이퍼시작부까지[종점제원 입력]
	double	m_dLengthTaperToTaperEnd		;	//테이퍼 길이
	double	m_dHeightTaperEnd				;	//테이퍼 높이
	double  m_HTaper_W						;	
	double  m_HTaper_H						;

	double	m_dHeightInOutStt				;	//내부출입구 높이
	double	m_dWidthInOutStt				;	//내부출입구 깊이
	double	m_dRadiusInOutStt				;	//내부출입구 round 반지름
	double	m_dHeightInOutLowerStt			;	//내부출입구 하단 높이
	double	m_dHeightInOutEnd				;	//내부출입구 높이
	double	m_dWidthInOutEnd				;	//내부출입구 깊이
	double	m_dRadiusInOutEnd				;	//내부출입구 round 반지름
	double	m_dHeightInOutLowerEnd			;	//내부출입구 하단 높이

	BYTE	m_cTaperAngleTypeStt			;	//테이퍼 각도->직각, 옵셋, 선형과 평행
	BYTE	m_cTaperAngleTypeEnd			;	//테이퍼 각도->직각, 옵셋, 선형과 평행

	BYTE	m_cFacePlateRoundType			;	//페이스 플레이트 라운드처리
	BYTE	m_cFacePlateUse					;	//페이스 플레이트 설치여부
	BYTE	m_cFaceManhole					;	//맨홀설치 여부
	
	BYTE	m_cByunDan						;	//[변단면 제원]
	long	m_nStyleByun[KYUNGGANSUMAX]		;	//[변단면 입력] 변단변 스타일
	long	m_nByunCurveType[KYUNGGANSUMAX]	;	//원, 2차 포물, n차 포물
	double	m_dHeightByun[KYUNGGANSUMAX]	;
	double	m_dLenLeftByun[KYUNGGANSUMAX]	;
	double	m_dLenRightByun[KYUNGGANSUMAX]	;
	double	m_dLenLeftCurve[KYUNGGANSUMAX]	;
	double	m_dLenRightCurve[KYUNGGANSUMAX]	;

	CMatrixStrGrid m_CrossAngleGrid			;	//[다이아프램 위치 입력]
	double	m_dLengthBaseCrossBeam			;	//기준 길이 위치 

	double	m_dDisCenterOnCrBeam			;	//교량 중심 설계
	double	m_dDisCenterOnVst				;
	double	m_dDisCenterOnJRib				;
	double	m_dDisCenterOnSplice			;
	double	m_dDisCenterOnFact				;
	double	m_dDisCenterOnCamber			;
	double	m_dDisCenterOnStud				;
	double	m_dDisCenterOnHStiff			;

	int		m_nLengthTypeOnCrBeam			;
	int		m_nLengthTypeOnVst				;
	int		m_nLengthTypeOnJRib				;
	int		m_nLengthTypeOnSplice			;
	int		m_nLengthTypeOnFact				;
	int		m_nLengthTypeOnCamber			;
	int		m_nLengthTypeOnStud				;
	int		m_nLengthTypeOnHStiff			;

	double m_dMoveTermOnSplice;

	double  m_dJiJumTerm[JIJUMSUMAX][JACKUPSUMAX];  // 지점보강재 간격 (상대거리임.)
	long	m_nQtyJijumStiff[JIJUMSUMAX];
	CMatrixStrGrid m_VStiffGrid				;	//[수직보강재 위치입력]
	BOOL	m_bInputAngleVStiff				;	//수직보강재 각도 입력 보기
	long	m_nCrossArrangeType				;	//가로보 배치형태
	double	m_dLengthBaseVStiff				;
	long	m_nHStiffDefault				;	//[수평보강재 위치입력]
	BYTE	m_cHStiffDimRound				;
	BYTE	m_cHStiffDimRoundValue			;	// 5mm or 10mm
	double	m_dHStiffLengthMin				;
	double	m_dHStiffTRibApply				;	// 수평보강재와 횡리브 간격조정
	double  m_dHStiffCheckLength			;	// 수평보강재 교번구간 체크시 기준 길이...
	BYTE	m_cHStiffHeightOffset			;
	double	m_dHStiffHeight1				;
	double	m_dHStiffHeight2				;
	double	m_dHStiffHeight3				;
	double	m_dHStiffMinLenJackup			;
	BYTE	m_cHStiffJackupVStiff			;
	BYTE	m_cFactThickFix					;	// 공장이음 두께 고정
	double	m_dDisOnCenterFactJoin			;	// 공장이음 이동
	BYTE	m_cSpliceSimpleType				;	//[현장이음 상세입력]

	// [슬래브 제원]
	double	m_dThickSlabLeft				;	// 슬래브 좌측 두께
	double	m_dThickSlabBase				;	// 슬래브 기준 두께	
	double	m_dThickSlabRight				;	// 슬래브 우측 두께
	double	m_dThickSlabHunch				;	// 헌치 높이
	double  m_dSlopHunch					;   // 헌치 경사
	double	m_dThickPave					;	// 포장 두께
	double	m_dWidthHunch					;	// 슬래브 돌출 길이
	double	m_dHunchDisForBaseThick			;	// 기준 두께 위치
	BYTE	m_cLevelRoadCenterToSlabEdge	;	// 도로중심간격 레벨적용, 도로중심이 슬래브 좌.우측을 벗어났을 경우
	double  m_dHeightDosang					;   // 철도교 도상높이	
	double	m_dMortalProtected				;	// 보호 몰탈 두께
	double  m_dThickSlabLeftExt				;	// 좌측 두께 For 소수주형
	double  m_dThickSlabRightExt			;	// 우측 두께 For 소수주형

	BYTE	m_cCamberUnitToMM				;	//[캠버 입력]
	double  m_dJackUpTerm[JIJUMSUMAX][JACKUPSUMAX];  // 잭업보강재 간격 (상대거리임.)	
	struct _JACKUP_SOLEPLATE_
	{
		double m_dL;
		double m_dD;
		double m_dH;
		double m_dW;
		double m_dT;
	} m_structJackupJewon[JIJUMSUMAX][2];

	
	BYTE	m_cSameApplySameColCross		;//동일구간 동일적용
	BYTE	m_cSameApplySameColVStiff		;//동일구간 동일적용
	BYTE	m_cSameApplySameColJRib			;//동일구간 동일적용
	BYTE	m_cSameApplySameColDanmyun		;//동일구간 동일적용
	BYTE	m_cSameApplySameColSplice		;//동일구간 동일적용
	BYTE	m_cSameApplySameColFactJoin		;//동일구간 동일적용
	BYTE	m_cSameApplySameColOthers		;//동일구간 동일적용
	BYTE	m_cSameApplySameColHStiff		;//동일구간 동일적용
	BYTE	m_cSameApplySameColStud			;//동일구간 동일적용

	BYTE	m_cFactoryType;					//제작솟음 자동계산 종류 0 : 최대값 기준 1 : 일정비율
	double  m_dMaxFactoryValue;				//최대값 기준 값
	double	m_dRateFactoryValue;			//일정비율 값
	//브라켓보강판상세
	int m_nGussetStiff_Gen[3][2];	//[Type][Weld:Bolt]
	double m_dGS_T[3][2];
	double m_dGS_S1[3][2];
	double m_dGS_S2[3][2];
	double m_dGS_S3[3][2];
	double m_dGS_W[3][2];
	double m_dGS_dA[3][2];
	double m_dGS_Gen_dB[3][2];
	double m_dGS_Gen_dC[3][2];
	double m_dGS_Trap_dB[3][2];
	double m_dGS_Trap_dC[3][2];
	double m_dGS_uA[3][2];
	double m_dGS_uB[3][2];
	double m_dGS_uC[3][2];
	//슬로트
	double m_dSlotA[3];
	double m_dSlotB[3];
	double m_dSlotR[3];
	double m_dSlotR2[3];
	BOOL m_bApplyDefaultSlot;

	double  m_FieldScallop_BaseT			;   //현장 용접 기준 t
	double  m_FieldScallop_R1				;   //현장 용접 시 복부판t < 기준t
	double  m_FieldScallop_R2				;   //현장 용접 시 복부판t >= 기준t
	//
	double m_dGS_Bolt_B[3];			//볼트 이음 형태
	double m_dGS_Bolt_H[3];
	double m_dGS_Bolt_R[3];

	double m_dGS_Sub_sT[3][2];			//부보강판
	double m_dGS_Sub_uD[3][2];
	double m_dGS_Sub_dD[3][2];
	double m_dGS_Sub_uE[3][2];
	double m_dGS_Sub_dE[3][2];
	// 수직보강재 대표단면입력시 필요한 변수
	double	m_dA_p; // 일반부
	double	m_dB_p;
	double	m_dC_p;
	double	m_dT_p;
	double	m_dC_n; // 부모멘트부
	double	m_dT_n;
	long    m_nInstallVStiff; // 양측, 좌측, 우측, 편측

	// 수평브레이싱 이음판 대표단면입력
	double m_dHBuSL;
	double m_dHBuGw;
	double m_dHBuSA;
	double m_dHBuSB;
	double m_dHBuSC;
	double m_dHBuSD;
	double m_dHBuSE;
	double m_dHBuStep1;
	double m_dHBuStep2;
	double m_dHBuStep3;
	double m_dHBuStep4;
	double m_dHBuStep5;
	double m_dHBuW;
	double m_dHBuW1;
	CString m_szHBrSpBoltDia;

	///< 가로보의 수직보강재 설치
	long	m_nInstallVStiffOnCrossBeam; ///< 0 : 설치안함 1:지점부만 설치 2:모든가로보설치
	BOOL    m_bTendonInputComplete;


	// 신종화 추가 ================
	BOOL m_bUserSlabHunch;
	BOOL m_bQuickLineSearch; // 빠른선형 검색
	// ============================

	// 심동윤 추가 ================
	BOOL m_bUseJackupSolePlate;
	// ============================
	double m_dSameFlangeWidth;
	BYTE	m_cSlabThickAddTypeStt			;	// 0 : 없음 1 : 두번째 가로보까지 2 : 단부 이격거리
	BYTE	m_cSlabThickAddTypeEnd			;	// 0 : 없음 1 : 두번째 가로보까지 2 : 단부 이격거리

	BOOL m_bApplyHunchAtCB; // 가로보 헌치적용(거더상판 일치시)   적용/미적용

	// TU거더교 관련 ==============
	double	m_dLengthTUBindConc_Stt;			// 구속콘크리트 시점부 돌출길이
	double	m_dLengthTUBindConc_End;			// 구속콘크리트 종점부 돌출길이

	// 거더단부 모따기 기준각도
	double	m_dGirEndStndGuttingAngle;

	long m_nBaseHeightPosHBraceSplice;
	double m_dBaseHeightFixPosHBraceSplice;
	double m_dBaseHeightRatioPosHBraceSplice;

// Implementation
protected:
	void SetPOSAndEachPointer();
	// 심동윤 추가 ================
//	long m_nBridgeType;				///< 교량 종류 0 : 판형교, 1 : 소수주형, 2: TU거더교

public:
	void Serialize(CArchive& ar);

	BOOL CheckSelfData(BOOL bAssertUse);
	CPlateBridge& operator=(const CPlateBridge& obj);
	void MakeGuardWall(long nMake);
	void MakeGuardFence(long nMake, CStringArray& strName);
	void MakeGirder(long nMake, BOOL bCopy=TRUE);

	BOOL IsExtLine(long nG = -1) const;
	BOOL IsJustModified() const;
	BOOL IsModify() const;
	BOOL IsDataFilled() const;

	void SetJustModified(BOOL bModify);
	void SetLengthHDanByLine(double dHDanLen, long nQtyLine) { m_dLengthHDan[nQtyLine] = dHDanLen; }
	double GetLengthHDanByLine(long nQtyLine) { return m_dLengthHDan[nQtyLine]; }
	long GetNumberGirderNearestToCenterLine();
	void SetGirderSearchIP();
	double	GetLengthBridge() const {return m_dLengthUganStt+m_dLengthTotalSlab + m_dLengthUganEnd;}
	BOOL IsHapsung();
	double GetStdMaxTendonHeight();
private:
	long    m_NumHaiseokCombo;	// 해석방법
};
#endif // !defined(AFX_PLATEBRIDGE_H__5F50DD8D_BE7B_4FDB_A781_1D4BA535B029__INCLUDED_)
