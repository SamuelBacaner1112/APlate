// OptionGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionGeneral property page

IMPLEMENT_DYNCREATE(COptionGeneral, CPropertyPage)

COptionGeneral::COptionGeneral() : CPropertyPage(COptionGeneral::IDD)
{
	//{{AFX_DATA_INIT(COptionGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionGeneral::~COptionGeneral()
{
}

void COptionGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionGeneral)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);	
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(COptionGeneral)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionGeneral message handlers

BOOL COptionGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	InitTreeData(FALSE);
	InitGridData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionGeneral::InitTreeData(BOOL bDefaultLoad)
{		
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bDivPlanJong		= bDefaultLoad ? FALSE : pOpt->m_Gen_bDivPlanJong;
	BOOL bBujeThick			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bBujeThick;
	BOOL bPlanView			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bPlanView;
	BOOL bJongView			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJongView;
	BOOL bFrontView			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bFrontView;
	BOOL bKeyPlan			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlan;
	BOOL bLegend			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bLegend;
	BOOL bBasicNote			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bBasicNote;
	BOOL bForceText			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bForceText;
	BOOL bForceTextLow		= bDefaultLoad ? TRUE  : pOpt->m_Gen_bForceTextLow;
	BOOL bMatchLine			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bMatchLine;	
	BOOL bJongCenterGir		= bDefaultLoad ? FALSE : pOpt->m_Gen_bJongCenterGir;
	BOOL bJongGenGir		= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJongGenGir;
	BOOL bKeyPlanSpliceMark	= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlanSpliceMark;
	BOOL bKeyPlanJijumMark	= bDefaultLoad ? TRUE : pOpt->m_Gen_bKeyPlanJijumMark;
	BOOL bKeyPlanCrossBeam	= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlanCrossBeam;
	BOOL bKeyPlanDimension	= bDefaultLoad ? TRUE : pOpt->m_Gen_bKeyPlanDimension;
	BOOL bKeyPlanOneLine	= bDefaultLoad ? TRUE : pOpt->m_Gen_bKeyPlanOneLine;
	BOOL bKeyPlanRoVer		= bDefaultLoad ? FALSE : pOpt->m_Gen_bKeyPlanRoVer;	
	BOOL bKeyPlanDirection	= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlanDirection;
	BOOL bJongLevel			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJongLevel;	
	BOOL bTableVertical		= bDefaultLoad ? FALSE : pOpt->m_Gen_bVerticalNote;
	BOOL bJijumSymbol		= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJijumSymbol;
	BOOL bJijumSimple	    = bDefaultLoad ? TRUE  : pOpt->m_Gen_bJijumSimple;		
	BOOL bMarkCross			= bDefaultLoad ? FALSE : pOpt->m_Gen_bMarkCorss;
	BOOL bMarkSringer		= bDefaultLoad ? FALSE : pOpt->m_Gen_bMarkStringer;
	BOOL bTextStationPyung  = bDefaultLoad ?  TRUE : pOpt->m_Gen_bPyungJijumStation;
	BOOL bTextStationJong   = bDefaultLoad ?  TRUE : pOpt->m_Gen_bJongJijumStation;
	BOOL bPyungLineInfo		= bDefaultLoad ?  TRUE : pOpt->m_Gen_bPyungLineInfo;
	BOOL bByundan			= bDefaultLoad ?  TRUE : pOpt->m_Gen_bByundan;
	long nOutGirder			= bDefaultLoad ? 0	   : pOpt->m_Gen_nOutGirder;
	long nStyleDanmyunType	= bDefaultLoad ? 0	   : pOpt->m_Gen_nStyleDanmyunType;
	long nPyungDim			= bDefaultLoad ? 1	   : pOpt->m_Gen_nPyungDim;
	long nDivPage			= bDefaultLoad ? 0	   : pOpt->m_Gen_nDivPage;	
	BOOL bDimCenterGirder   = bDefaultLoad ? FALSE : pOpt->m_Gen_bDimCenterGirder;	
	BOOL bDimNormalGirder   = bDefaultLoad ? TRUE  : pOpt->m_Gen_bDimNormalGirder;		
	BOOL bAllHalf		    = bDefaultLoad ? FALSE : pOpt->m_Gen_bPyungAllHalf;	
	BOOL bVerTitleFront	    = bDefaultLoad ? TRUE  : pOpt->m_Gen_bVerTitleFront;	

	m_Options.SetOptionItem("도면 전체", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIV_PLANJONG", bDivPlanJong ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("DIV_PLANJONG", "\t평면도와 측면도를 분리 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("BUJETHICK", bBujeThick ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BUJETHICK", "\t부재 두께 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);


	m_Options.SetOptionItem("도면 표현", IDB_ITEMS, IDB_ITEMS);		
		m_Options.SetOption("PLANVIEW", bPlanView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);

		m_Options.SetOptionItem("PLANVIEW", "\t평면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("STYLEDANMYUN_TYPE", nStyleDanmyunType);
			m_Options.SetOptionItem("STYLEDANMYUN_TYPE", "\t\t단면타입 표현 전체거더", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("STYLEDANMYUN_TYPE", "\t\t단면타입 표현 1번 거더만", 1, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("STYLEDANMYUN_TYPE", "\t\t단면타입 표현 없음", 2, IDB_RADIOON, IDB_RADIOOFF);

		m_Options.SetOption("JONGVIEW", bJongView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGVIEW", "\t측면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("JONGCENTERGIR", bJongCenterGir ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JONGCENTERGIR", "\t\t도로중심 거더 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			//
			m_Options.SetOption("JONGGENGIRDER", bJongGenGir ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JONGGENGIRDER", "\t\t일반 거더 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("JONGGIRDER", nOutGirder);
				m_Options.SetOptionItem("JONGGIRDER", "\t\t\t교량중심 거더 출력", 0, IDB_RADIOON, IDB_RADIOOFF);
				m_Options.SetOptionItem("JONGGIRDER", "\t\t\t전체 거더 출력", 1, IDB_RADIOON, IDB_RADIOOFF);

		m_Options.SetOption("KEYPLAN", bKeyPlan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("KEYPLAN", "\t요약도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("SPLICE", bKeyPlanSpliceMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SPLICE", "\t\t현장이음 마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("JIJUMMARK", bKeyPlanJijumMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JIJUMMARK", "\t\t지점마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("CROSS", bKeyPlanCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("CROSS", "\t\t가로보 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYDIMENSION", bKeyPlanDimension ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYDIMENSION", "\t\t치수선 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("DIRECTION", bKeyPlanDirection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("DIRECTION", "\t\t진행방향 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ONELINE", bKeyPlanOneLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ONELINE", "\t\t부재한선으로 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ROVER", bKeyPlanRoVer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ROVER", "\t\t요약도 수직 회전", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);			

		m_Options.SetOption("FRONTVIEW", bFrontView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FRONTVIEW", "\t횡단면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("VERTITLEFRONT", bVerTitleFront ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VERTITLEFRONT", "\t\t제목을 수직으로 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("LEGEND", bLegend ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("LEGEND", "\t범례", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		
		m_Options.SetOption("BASICNOTE", bBasicNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BASICNOTE", "\t기본노트", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("NOTETABLE_VERT", bTableVertical ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("NOTETABLE_VERT", "\t\t수직으로 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			
		m_Options.SetOption("MATCHLINE", bMatchLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MATCHLINE", "\t매치라인 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	


	m_Options.SetOptionItem("평면도 설정", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PYUNG_DIM_CR", bDimCenterGirder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PYUNG_DIM_CR", "\t도로중심 치수 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PYUNG_DIM_NORMAL", bDimNormalGirder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PYUNG_DIM_NORMAL", "\t일반거더 치수 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PYUNG_DIM", nPyungDim);
		m_Options.SetOptionItem("PYUNG_DIM","\t\t전체거더 치수 표현", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("PYUNG_DIM","\t\t양측거더 치수 표현", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOption("PYUNG_ALLHALF", bAllHalf ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PYUNG_ALLHALF", "\t상하면 거더 중앙기준으로 분리", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("MARK_CROSS", bMarkCross ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MARK_CROSS", "\t가로보 단면 마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("MARK_STRINGER", bMarkSringer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MARK_STRINGER", "\t세로보 단면 마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("TEXT_STATION_PYUNG", bTextStationPyung ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("TEXT_STATION_PYUNG", "\t지점 STATION 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_LINEINFO", bPyungLineInfo ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_LINEINFO", "\t선형정보 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("측면도 설정", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("JONGLEVEL", bJongLevel ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGLEVEL", "\t측면도 LEVEL로 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("TEXT_STATION_JONG", bTextStationJong ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("TEXT_STATION_JONG", "\t지점 STATION 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("JIJUMSYMBOL", bJijumSymbol ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JIJUMSYMBOL", "\t지점위치 SYMBOL 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("JIJUMSIMPLE", bJijumSimple ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JIJUMSIMPLE", "\t교각 표현시 헌치선과 기둥선 삭제", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("BYUNDAN", bByundan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BYUNDAN", "\t변단면 계수값 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("FORCETEXT", bForceText ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FORCETEXT", "\t응력구간 문자 상부 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("FORCETEXTLOW", bForceTextLow ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FORCETEXTLOW", "\t응력구간 문자 하부 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem("도면분할 방식", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIV_DOM", nDivPage);
		m_Options.SetOptionItem("DIV_DOM","\t경간 단위로 분할", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t현장이음 단위로 분할", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t가로보 개수로 분할", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t전체 교량을 한장으로", 3, IDB_RADIOON, IDB_RADIOOFF);
		
	///// 평면도 부재 표현 선택 
	DWORD DBuje		= bDefaultLoad ? DRAW_PLAN_ALL : pOpt->m_Gen_PyungBuje;
	BOOL bWeb		= (DBuje & DRAW_PLAN_WEB)		? TRUE : FALSE;
	BOOL bSplice	= (DBuje & DRAW_PLAN_SPLICE)    ? TRUE : FALSE;
	BOOL bFact		= (DBuje & DRAW_PLAN_FACT)		? TRUE : FALSE;
	BOOL bCrossBeam	= (DBuje & DRAW_PLAN_CROSSBEAM) ? TRUE : FALSE;
	BOOL bStringer	= (DBuje & DRAW_PLAN_STRINGER)  ? TRUE : FALSE;
	BOOL bBracket	= (DBuje & DRAW_PLAN_BRACKET)   ? TRUE : FALSE;
	BOOL bBrBeam	= (DBuje & DRAW_PLAN_BRBEAM)    ? TRUE : FALSE;
	BOOL bSolePlate	= (DBuje & DRAW_PLAN_SOLEPLATE) ? TRUE : FALSE;
	BOOL bJackUp	= (DBuje & DRAW_PLAN_JACKUP)    ? TRUE : FALSE;
	BOOL bSlab		= (DBuje & DRAW_PLAN_SLAB)		? TRUE : FALSE;

	m_Options.SetOptionItem("평면도 부재 표현", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PLAN_WEB", bWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_WEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_JACKUP", bJackUp ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_JACKUP", "\t잭업보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_FACT", bFact ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_FACT", "\t공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_STRINGER", bStringer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_STRINGER", "\t세로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_CROSSBEAM", bCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_CROSSBEAM", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_BRACKET", bBracket ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_BRACKET", "\t외측가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_BRBEAM", bBrBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_BRBEAM", "\t외측빔", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_SOLEPLATE", bSolePlate ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_SOLEPLATE", "\t솔플레이트", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_SLAB", bSlab ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_SLAB", "\t슬래브 외곽선", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

///// 측면도 부재 표현 선택 
	BOOL bVStiff, bHStiff;
	DBuje		= bDefaultLoad ? DRAW_JONG_ALL : pOpt->m_Gen_JongBuje;
	bVStiff		= (DBuje & DRAW_JONG_VSTIFF)	? TRUE : FALSE;
	bSplice		= (DBuje & DRAW_JONG_SPLICE)	? TRUE : FALSE;
	bFact		= (DBuje & DRAW_JONG_FACT)		? TRUE : FALSE;
	bSolePlate	= (DBuje & DRAW_JONG_SOLEPLATE)	? TRUE : FALSE;
	bJackUp		= (DBuje & DRAW_JONG_JACKUP)	? TRUE : FALSE;
	bHStiff		= (DBuje & DRAW_JONG_HSTIFF)	? TRUE : FALSE;
	bSlab		= (DBuje & DRAW_JONG_SLAB)		? TRUE : FALSE;

	m_Options.SetOptionItem("측면도 부재 표현", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("JONG_HSTIFF", bHStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_HSTIFF", "\t수평보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_FACT", bFact ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_FACT", "\t공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_SOLEPLATE", bSolePlate ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_SOLEPLATE", "\t솔플레이트", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_JACKUP", bJackUp ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_JACKUP", "\t잭업보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_SLAB", bSlab ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_SLAB", "\t슬래브 외곽선", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	// 제원표현
	DWORD DBujeInfo			= bDefaultLoad ? BUJEINFO__ALL : pOpt->m_Gen_BujeInfo;
	BOOL bJewonFlangeBuje	= (DBujeInfo & BUJEINFO_FLANGE_NAME)	? TRUE : FALSE;
	BOOL bJewonFlangeMat	= (DBujeInfo & BUJEINFO_FLANGE_MAT)		? TRUE : FALSE;
	BOOL bJewonWebBuje		= (DBujeInfo & BUJEINFO_WEB_NAME)		? TRUE : FALSE;
	BOOL bJewonWebMat		= (DBujeInfo & BUJEINFO_WEB_MAT)		? TRUE : FALSE;
	BOOL bJewonHStiffBuje	= (DBujeInfo & BUJEINFO_HSTIFF_NAME)	? TRUE : FALSE;
	BOOL bJewonHStiffMat	= (DBujeInfo & BUJEINFO_HSTIFF_MAT)		? TRUE : FALSE;
	BOOL bJewonVStiffBuje	= (DBujeInfo & BUJEINFO_VSTIFF_NAME)	? TRUE : FALSE;
	BOOL bJewonVStiffMat	= (DBujeInfo & BUJEINFO_VSTIFF_MAT)		? TRUE : FALSE;
	BOOL bJewonJackUpBuje	= (DBujeInfo & BUJEINFO_JACKUP_NAME)	? TRUE : FALSE;
	BOOL bJewonJackUpMat	= (DBujeInfo & BUJEINFO_JACKUP_MAT)		? TRUE : FALSE;

	m_Options.SetOptionItem("제원표현", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOptionItem("\t상하판", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("FLANGEBUJE", bJewonFlangeBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("FLANGEBUJE", "\t\t부재명", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("FLANGEMAT", bJewonFlangeMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("FLANGEMAT", "\t\t재질", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t복부판", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("WEBBUJE", bJewonWebBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("WEBBUJE", "\t\t부재명", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("WEBMAT", bJewonWebMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("WEBMAT", "\t\t재질", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t수평보강재", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("HSTIFFBUJE", bJewonHStiffBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HSTIFFBUJE", "\t\t부재명", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("HSTIFFMAT", bJewonHStiffMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HSTIFFMAT", "\t\t재질", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t수직보강재", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("VSTIFFBUJE", bJewonVStiffBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VSTIFFBUJE", "\t\t부재명", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VSTIFFMAT", bJewonVStiffMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VSTIFFMAT", "\t\t재질", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t잭업보강재", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("JACKUPBUJE", bJewonJackUpBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JACKUPBUJE", "\t\t부재명", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("JACKUPMAT", bJewonJackUpMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JACKUPMAT", "\t\t재질", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionGeneral::InitGridData(BOOL bDefaultLoad)
{		
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_Gen_Scale.GetAt(0) = 100;
		pOpt->m_Gen_Scale.GetAt(1) = 50;
		pOpt->m_Gen_Scale.GetAt(2) = 800;
		pOpt->m_Gen_Scale.GetAt(3) = 800;
		pOpt->m_Gen_nUnitEaDraw = 1;	
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(6);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);				
	m_Grid.SetColumnWidth(0,Rect.Width()*2/3);
	m_Grid.SetColumnWidth(1,Rect.Width()*1/3);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));	
	m_Grid.SetTextBkColor(RGB(255,255,255));

	m_Grid.SetTextMatrix(0,0,"설정항목");
	m_Grid.SetTextMatrix(1,0,"거더 일반도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"거더 단면도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(3,0,"요약도(수평)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"요약도(수직)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"출력단위 개수",(UINT)DT_LEFT);
	
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_Gen_Scale.GetAt(0),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_Gen_Scale.GetAt(1),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_Gen_Scale.GetAt(2),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_Gen_Scale.GetAt(3),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.0f",pOpt->m_Gen_nUnitEaDraw,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);	
}

void COptionGeneral::OnOK() 
{	
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_Gen_bDivPlanJong		= m_Options.GetOption("DIV_PLANJONG")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bBujeThick			= m_Options.GetOption("BUJETHICK")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bPlanView			= m_Options.GetOption("PLANVIEW")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_nStyleDanmyunType	= m_Options.GetOption("STYLEDANMYUN_TYPE");
	pOpt->m_Gen_bJongView			= m_Options.GetOption("JONGVIEW")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bJongCenterGir		= m_Options.GetOption("JONGCENTERGIR")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJongGenGir			= m_Options.GetOption("JONGGENGIRDER")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_nOutGirder			= m_Options.GetOption("JONGGIRDER");

	pOpt->m_Gen_bFrontView  		= m_Options.GetOption("FRONTVIEW")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bVerTitleFront 		= m_Options.GetOption("VERTITLEFRONT")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlan			= m_Options.GetOption("KEYPLAN")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanSpliceMark	= m_Options.GetOption("SPLICE")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanJijumMark	= m_Options.GetOption("JIJUMMARK")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanCrossBeam	= m_Options.GetOption("CROSS")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanDimension	= m_Options.GetOption("KEYDIMENSION")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanDirection	= m_Options.GetOption("DIRECTION")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanOneLine		= m_Options.GetOption("KEYPLAN_ONELINE")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanRoVer		= m_Options.GetOption("KEYPLAN_ROVER")==-1 ? TRUE : FALSE;	


	pOpt->m_Gen_bLegend				= m_Options.GetOption("LEGEND")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bBasicNote			= m_Options.GetOption("BASICNOTE")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bVerticalNote		= m_Options.GetOption("NOTETABLE_VERT")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJongLevel		    = m_Options.GetOption("JONGLEVEL")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJijumSymbol	    = m_Options.GetOption("JIJUMSYMBOL")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJijumSimple	    = m_Options.GetOption("JIJUMSIMPLE")==-1 ? TRUE : FALSE;		
	pOpt->m_Gen_bMarkCorss		    = m_Options.GetOption("MARK_CROSS")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bMarkStringer		= m_Options.GetOption("MARK_STRINGER")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bPyungJijumStation	= m_Options.GetOption("TEXT_STATION_PYUNG")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bPyungLineInfo		= m_Options.GetOption("PLAN_LINEINFO")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJongJijumStation	= m_Options.GetOption("TEXT_STATION_JONG")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bByundan			= m_Options.GetOption("BYUNDAN")==-1 ? TRUE : FALSE;

	pOpt->m_Gen_bForceText			= m_Options.GetOption("FORCETEXT")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bForceTextLow		= m_Options.GetOption("FORCETEXTLOW")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bMatchLine			= m_Options.GetOption("MATCHLINE")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_nPyungDim			= m_Options.GetOption("PYUNG_DIM");
	pOpt->m_Gen_nDivPage			= m_Options.GetOption("DIV_DOM");
	pOpt->m_Gen_bDimCenterGirder	= m_Options.GetOption("PYUNG_DIM_CR")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bDimNormalGirder	= m_Options.GetOption("PYUNG_DIM_NORMAL")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bPyungAllHalf		= m_Options.GetOption("PYUNG_ALLHALF")==-1 ? TRUE : FALSE;

	// 부재정보 옵션
	DWORD dOptJweon = 0;
	if(m_Options.GetOption("FLANGEBUJE"))		dOptJweon |= BUJEINFO_FLANGE_NAME;
	if(m_Options.GetOption("FLANGEMAT"))		dOptJweon |= BUJEINFO_FLANGE_MAT;
	if(m_Options.GetOption("WEBBUJE"))			dOptJweon |= BUJEINFO_WEB_NAME;
	if(m_Options.GetOption("WEBMAT"))			dOptJweon |= BUJEINFO_WEB_MAT;
	if(m_Options.GetOption("HSTIFFBUJE"))		dOptJweon |= BUJEINFO_HSTIFF_NAME;
	if(m_Options.GetOption("HSTIFFMAT"))		dOptJweon |= BUJEINFO_HSTIFF_MAT;
	if(m_Options.GetOption("VSTIFFBUJE"))		dOptJweon |= BUJEINFO_VSTIFF_NAME;
	if(m_Options.GetOption("VSTIFFMAT"))		dOptJweon |= BUJEINFO_VSTIFF_MAT;
	if(m_Options.GetOption("JACKUPBUJE"))		dOptJweon |= BUJEINFO_JACKUP_NAME;
	if(m_Options.GetOption("JACKUPMAT"))		dOptJweon |= BUJEINFO_JACKUP_MAT;
	pOpt->m_Gen_BujeInfo = dOptJweon;

	// 평면도 옵션
	DWORD dOptPlan = 0;
	if(m_Options.GetOption("PLAN_WEB"))			dOptPlan |= DRAW_PLAN_WEB;
	if(m_Options.GetOption("PLAN_SPLICE"))		dOptPlan |= DRAW_PLAN_SPLICE;
	if(m_Options.GetOption("PLAN_FACT"))		dOptPlan |= DRAW_PLAN_FACT;
	if(m_Options.GetOption("PLAN_CROSSBEAM"))	dOptPlan |= DRAW_PLAN_CROSSBEAM;
	if(m_Options.GetOption("PLAN_STRINGER"))	dOptPlan |= DRAW_PLAN_STRINGER;
	if(m_Options.GetOption("PLAN_BRACKET"))		dOptPlan |= DRAW_PLAN_BRACKET;
	if(m_Options.GetOption("PLAN_BRBEAM"))		dOptPlan |= DRAW_PLAN_BRBEAM;
	if(m_Options.GetOption("PLAN_SOLEPLATE"))	dOptPlan |= DRAW_PLAN_SOLEPLATE;
	if(m_Options.GetOption("PLAN_JACKUP"))		dOptPlan |= DRAW_PLAN_JACKUP;
	if(m_Options.GetOption("PLAN_SLAB"))		dOptPlan |= DRAW_PLAN_SLAB;
	pOpt->m_Gen_PyungBuje = dOptPlan;
	
	// 측면도 옵션
	DWORD dOptJong = 0;
	if(m_Options.GetOption("JONG_VSTIFF"))		dOptJong |= DRAW_JONG_VSTIFF;
	if(m_Options.GetOption("JONG_HSTIFF"))		dOptJong |= DRAW_JONG_HSTIFF;
	if(m_Options.GetOption("JONG_SPLICE"))		dOptJong |= DRAW_JONG_SPLICE;
	if(m_Options.GetOption("JONG_FACT"))		dOptJong |= DRAW_JONG_FACT;
	if(m_Options.GetOption("JONG_SOLEPLATE"))	dOptJong |= DRAW_JONG_SOLEPLATE;
	if(m_Options.GetOption("JONG_JACKUP"))		dOptJong |= DRAW_JONG_JACKUP;
	if(m_Options.GetOption("JONG_SLAB"))		dOptJong |= DRAW_JONG_SLAB;
	pOpt->m_Gen_JongBuje = dOptJong;

	double scale = 0;
	for(long i=1; i<pOpt->m_Gen_Scale.GetSize()+1; i++)
	{
		scale = m_Grid.GetTextMatrixDouble(i,1);
		pOpt->m_Gen_Scale.SetAt(i-1, scale);
	}
	pOpt->m_Gen_nUnitEaDraw = m_Grid.GetTextMatrixLong(5,1);
	
	pStd->GetDrawPageMng()->SetModified(PAGE_GENERAL);

	CPropertyPage::OnOK();
}

// OptionStd의 초기값과 일치시켜야함....
void COptionGeneral::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionGeneral::OnSetActive() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;
	
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_GENERAL;

	return CPropertyPage::OnSetActive();
}

