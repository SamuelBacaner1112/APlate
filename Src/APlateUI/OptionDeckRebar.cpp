// OptionDeckRebar.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDeckRebar dialog

IMPLEMENT_DYNCREATE(COptionDeckRebar, CPropertyPage)

COptionDeckRebar::COptionDeckRebar() : CPropertyPage(COptionDeckRebar::IDD)
{
	//{{AFX_DATA_INIT(COptionDeckRebar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionDeckRebar::~COptionDeckRebar()
{
}

void COptionDeckRebar::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDeckRebar)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDeckRebar, CPropertyPage)
	//{{AFX_MSG_MAP(COptionDeckRebar)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDeckRebar message handlers

BOOL COptionDeckRebar::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitTreeData();
	InitGridData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionDeckRebar::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	
	if(bDefaultLoad)
	{
		pOpt->m_dDeck_ScaleRebarPlan					= 50;			// 슬래브 평면도 스케일
		pOpt->m_dDeck_ScaleRebarJong					= 50;			// 슬래브 종단면도 스케일
		pOpt->m_dDeck_ScaleRebarFront					= 50;			// 슬래브 횡단면도 스케일
		pOpt->m_dDeck_ScaleRebarSangseDanbu				= 30;			// 슬래브 상세도-단부상세도 스케일
		pOpt->m_dDeck_ScaleRebarSangseDanmyunAA			= 30;			// 슬래브 상세도-단면A-A 스케일
		pOpt->m_dDeck_ScaleRebarSangseWaterReinforce	= 20;			// 배수구 보강 상세도
		pOpt->m_dDeck_ScaleRebarSangseCentReinforce		= 80;			// 중앙지점부 보강철근 이음 상세도
		pOpt->m_dDEck_ScaleRebarSangseDoubleChain		= 80;			// 철근 겹이음 상세 스케일
		pOpt->m_dDeck_ScaleSangseA						= 15;			// 슬래브 상세도-A 스케일
		pOpt->m_dDeck_ScaleSangseChairBlock				= 80;			// 슬래브 상세도-조립철근 및 chair block상세 스케일
		pOpt->m_dDeck_SacleSangseShearReinforce			= 50;			// 전단보강철근 상세
		pOpt->m_dDeck_ScaleSangseShearArray				= 20;			// 슬래브 상세도-조립철근 배치상세 스케일
		pOpt->m_dDeck_ScaleSangseBlockOut				= 80;			// BLOCKOUT 상세
		pOpt->m_dDeck_ScaleSangseCrossBeam				= 20;			// 가로보 상세
		pOpt->m_dDeck_ScaleSangseStringer				= 20;			// 세로보 상세
		pOpt->m_dDeck_ScaleRebarMain					= 50;			// 주철근 조립도 스케일
		pOpt->m_dDeck_MainRebarCycle					= 3;			// 주철근 조립도 사이클수
		pOpt->m_dDeck_ScaleRebarBelyck					= 30;			// 배력철근 조립도 스케일
		pOpt->m_dDeck_BelyckCycle						= 3;			// 배력철근 조립도 사이클수
		pOpt->m_dDeck_SpaceBetweenJewon					= 1000;			// 철근 제원간격
		pOpt->m_dDeck_DatasuOnRebarTable				= 70;			// 테이블당 데이터수
		pOpt->m_dDeck_TypeRebarFrontLeft				= 0;			// 슬래브 횡단면도 타입(기본값 지점부)
		pOpt->m_dDeck_TypeRebarFrontRigh				= 2;			// 슬래브 횡단면도 타입(기본값 없음)
		pOpt->m_dDeck_ScaleGuardRebar					= 10;			// 방호벽 스케일
		pOpt->m_nDeck_JongDrawPos						= 0;			// 슬래브 종단면도 배근 형상(기본값 슬래브중심)
		pOpt->m_dWidthRebarTableRect					= 4000;			// 상세박스 폭
		pOpt->m_dHeightRebarTableRect					= 4000;			// 상세박스 높이
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(28);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);				
	m_Grid.SetColumnWidth(0,Rect.Width()*2/3);
	m_Grid.SetColumnWidth(1,Rect.Width()*1/3-20);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));	
	m_Grid.SetTextBkColor(RGB(255,255,255));

	m_Grid.SetTextMatrix(0,0,	"설정항목");
	m_Grid.SetTextMatrix(1,0,	"평면도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,	"종단면도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(3,0,	"종단면배근 형상",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(4,0,	"횡단면도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,	"횡단면도 위치(좌측)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(6,0,	"횡단면도 위치(우측)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(7,0,	"주철근 조립도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(8,0,	"배력철근 조립도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(9,0,	"슬래브 단부 상세도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(10,0,	"단면A-A 상세도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(11,0,	"배수구 보강 상세도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(12,0,	"중앙 지점부 보강철근 이음 상세도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(13,0,	"철근 겹이음 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(14,0,	"상세도 A",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(15,0,	"SPACE-BAR 및 CHAIR MORTAR BLOCK 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(16,0,	"전단 보강철근 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(17,0,	"조립철근 배치도 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(18,0,	"BLOCK OUT  상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(19,0,	"가로보 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(20,0,	"세로보 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(21,0,	"철근제원 간격",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(22,0,	"테이블당 데이타수",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(23,0,	"방호벽 스케일",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(24,0,	"겹이음장표기(주철근)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(25,0,	"겹이음장표기(배력철근)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(26,0,	"상세박스 폭",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(27,0,	"상세박스 높이",(UINT)DT_LEFT);


	m_Grid.SetTextMatrix(0,1,	"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f", pOpt->m_dDeck_ScaleRebarPlan, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f", pOpt->m_dDeck_ScaleRebarJong, (UINT)DT_RIGHT);
	m_Grid.SetCellType(3, 1, CT_COMBO);
		if(pOpt->m_nDeck_JongDrawPos == 0)
			m_Grid.SetTextMatrix(3, 1, "슬래브중심");
		else if(pOpt->m_nDeck_JongDrawPos == 1)
			m_Grid.SetTextMatrix(3, 1, "슬래브좌측");
		else if(pOpt->m_nDeck_JongDrawPos == 2)
			m_Grid.SetTextMatrix(3, 1, "슬래브우측");
	m_Grid.SetTextMatrix(4,1,"%.1f", pOpt->m_dDeck_ScaleRebarFront, (UINT)DT_RIGHT);
	m_Grid.SetCellType(5, 1, CT_COMBO);
		if(pOpt->m_dDeck_TypeRebarFrontLeft == 0)
			m_Grid.SetTextMatrix(5, 1, "지점부");
		else if(pOpt->m_dDeck_TypeRebarFrontLeft == 1)
			m_Grid.SetTextMatrix(5, 1, "일반부");
		else if(pOpt->m_dDeck_TypeRebarFrontLeft == 2)
			m_Grid.SetTextMatrix(5, 1, "없음");
	m_Grid.SetCellType(6, 1, CT_COMBO);
		if(pOpt->m_dDeck_TypeRebarFrontRigh == 0)
			m_Grid.SetTextMatrix(6, 1, "지점부");
		else if(pOpt->m_dDeck_TypeRebarFrontRigh == 1)
			m_Grid.SetTextMatrix(6, 1, "일반부");
		else
			m_Grid.SetTextMatrix(6, 1, "없음");
	m_Grid.SetTextMatrix(7,1,	"%.1f",	pOpt->m_dDeck_ScaleRebarMain, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(8,1,	"%.1f",	pOpt->m_dDeck_ScaleRebarBelyck, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(9,1,	"%.1f",	pOpt->m_dDeck_ScaleRebarSangseDanbu, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(10,1,	"%.1f",	pOpt->m_dDeck_ScaleRebarSangseDanmyunAA, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(11,1,	"%.1f",	pOpt->m_dDeck_ScaleRebarSangseWaterReinforce, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(12,1,	"%.1f",	pOpt->m_dDeck_ScaleRebarSangseCentReinforce, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(13,1,	"%.1f",	pOpt->m_dDEck_ScaleRebarSangseDoubleChain, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(14,1,	"%.1f",	pOpt->m_dDeck_ScaleSangseA, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(15,1,	"%.1f",	pOpt->m_dDeck_ScaleSangseChairBlock, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(16,1,	"%.1f",	pOpt->m_dDeck_SacleSangseShearReinforce, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(17,1,	"%.1f",	pOpt->m_dDeck_ScaleSangseShearArray, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(18,1,	"%.1f",	pOpt->m_dDeck_ScaleSangseBlockOut, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(19,1,	"%.1f",	pOpt->m_dDeck_ScaleSangseCrossBeam, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(20,1,	"%.1f",	pOpt->m_dDeck_ScaleSangseStringer, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(21,1,	"%.1f",	pOpt->m_dDeck_SpaceBetweenJewon, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(22,1,	"%.0f",	pOpt->m_dDeck_DatasuOnRebarTable, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(23,1,	"%.0f",	pOpt->m_dDeck_ScaleGuardRebar, (UINT)DT_RIGHT);
	m_Grid.SetCellType(24, 1, CT_COMBO);
		if(pOpt->m_bDeck_ShowJointMainRebar)
			m_Grid.SetTextMatrix(24, 1, "적용");
		else
			m_Grid.SetTextMatrix(24, 1, "미적용");
	m_Grid.SetCellType(25, 1, CT_COMBO);
		if(pOpt->m_bDeck_ShowJointBeryukRebar)
			m_Grid.SetTextMatrix(25, 1, "적용");
		else
			m_Grid.SetTextMatrix(25, 1, "미적용");

		m_Grid.SetTextMatrix(26,1,	"%.0f",	pOpt->m_dWidthRebarTableRect, (UINT)DT_RIGHT);
		m_Grid.SetTextMatrix(27,1,	"%.0f",	pOpt->m_dHeightRebarTableRect, (UINT)DT_RIGHT);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionDeckRebar::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bPlanView				= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_PlanView;
	BOOL bJongView				= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_JongView;
	BOOL bFrontView				= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_FrontView;
	BOOL bFrontViewCenter		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_FrontViewCenter;
	BOOL bFrontViewJijum		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_FrontViewJijum;

	BOOL bSangse				= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_Sangse;					
	BOOL bSangseDanbu			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseDanbu;
	BOOL bSangseDanmyunAA		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseDanmyunAA;
	BOOL bSangseWaterReinforce	= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseWaterReinforce;
	BOOL bSangseCentReinforce	= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseCentReinforce;
	BOOL bSangseDoubleChain		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseDoubleChain;
	BOOL bSangseA				= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseA;
	BOOL bSangseChairBlock		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseChairBlock;
	BOOL bSangseShearReinforce	= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseShearReinforce;
	BOOL bSangseShearArray		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseShearArray;
	BOOL bSangseBlockOut		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseBlockOut;
	BOOL bSangseCrossBeam		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseCrossBeam;
	BOOL bSangseStringer		= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseStringer;

	BOOL bMainRebarView			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_MainRebarView;
	BOOL bBelyckView			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_BelyckView;

	BOOL bSangseRebar			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseRebar;
	BOOL bSangseMatrial			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseMatrial;
	BOOL bSangseMatrialTable	= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseMatrialTable;
	
	BOOL bSangseGuard			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_SangseGuard;
	BOOL bGuardSection			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_GuardSection;
	BOOL bGuardDetail			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_GuardDetail;
	BOOL bGuardTable			= bDefaultLoad ? TRUE : pOpt->m_bDeckRebar_GuardTable;
	long nDivType				= bDefaultLoad ? 0	  : pOpt->m_nDeck_Slab_DivType;		// 도면 분할 방식
	long nRebarTableOpt			= bDefaultLoad ? 0	  : pOpt->m_nDeck_Slab_RebarTableOpt;		// 도면 분할 방식
	long nTypeRebarTable		= bDefaultLoad ? 0    : pOpt->m_nTypeRebarTable;  //도면 출력 타입

	m_Options.SetOptionItem("도면 표현", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PLANVIEW", bPlanView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLANVIEW", "\t평면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		
		m_Options.SetOption("JONGVIEW", bJongView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGVIEW", "\t종단면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("FRONTVIEW", bFrontView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FRONTVIEW", "\t횡단면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		
		m_Options.SetOption("MAINREBARVIEW", bMainRebarView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MAINREBARVIEW", "\t주철근 조립도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOption("BELYCK", bBelyckView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BELYCK", "\t배력철근 조립도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOption("SANGVIEW", bSangse ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SANGVIEW", "\t상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("SANGVIEW_DANBU", bSangseDanbu ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_DANBU", "\t\t슬래브 단부 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_DANAA", bSangseDanmyunAA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_DANAA", "\t\t단면A-A 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_WATERREINFORCE", bSangseWaterReinforce ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_WATERREINFORCE", "\t\t배수구 보강 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_DOUBLECHAIN", bSangseDoubleChain ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_DOUBLECHAIN", "\t\t철근 겹이음 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_A", bSangseA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_A", "\t\t상세도'A'", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_CHAIRBLOCK", bSangseChairBlock ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_CHAIRBLOCK", "\t\tSPACE-BAR 및 CHAIR MORTAR BLOCK 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_SHEARREINFORCE", bSangseShearReinforce ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_SHEARREINFORCE", "\t\t전단 보강철근 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_SHEARARRAY", bSangseShearArray ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_SHEARARRAY", "\t\t조립철근 배치 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_BLOCKOUT", bSangseBlockOut ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_BLOCKOUT", "\t\tBLOCK OUT 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_CROSSBEAM", bSangseCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_CROSSBEAM", "\t\t가로보 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGVIEW_STRINGER", bSangseStringer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGVIEW_STRINGER", "\t\t세로보 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		
		m_Options.SetOption("SANGREBAR", bSangseRebar ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SANGREBAR", "\t철근 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("SANGREBAR_MATERIAL", bSangseMatrial ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGREBAR_MATERIAL", "\t\t철근 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGREBAR_MATERIALTABLE", bSangseMatrialTable ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGREBAR_MATERIALTABLE", "\t\t철근 재료표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		//방호벽 
		m_Options.SetOption("SANGGUARD", bSangseGuard ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SANGGUARD", "\t방호벽 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("SANGGUARD_SECTION", bGuardSection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGGUARD_SECTION", "\t\t방호벽 단면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGGUARD_DETAIL", bGuardDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGGUARD_DETAIL", "\t\t방호벽 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

			m_Options.SetOption("SANGGUARD_TABLE", bGuardTable ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SANGGUARD_TABLE", "\t\t방호벽 재료표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("도면분할 방식", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIVTYPE", nDivType);
		m_Options.SetOptionItem("DIVTYPE","\t경간 단위로 분할", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIVTYPE","\t전체 교량을 한장으로", 1, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOptionItem("철근재료표 표현방법", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("REBAR_TABLE_OPT", nRebarTableOpt);
		m_Options.SetOptionItem("REBAR_TABLE_OPT","\t일체로 표현", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("REBAR_TABLE_OPT","\t주철근, 기타철근 구분해서 표현", 1, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOptionItem("철근재료표 출력옵션", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("REBAR_TABLE", nTypeRebarTable);
		m_Options.SetOptionItem("REBAR_TABLE", "\t기본타입", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("REBAR_TABLE", "\t도면간소화 타입", 1, IDB_RADIOON, IDB_RADIOOFF);


	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionDeckRebar::OnOK()
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_bDeckRebar_PlanView				= m_Options.GetOption("PLANVIEW");
	pOpt->m_bDeckRebar_JongView				= m_Options.GetOption("JONGVIEW");
	pOpt->m_bDeckRebar_FrontView			= m_Options.GetOption("FRONTVIEW");
	//상세도
	pOpt->m_bDeckRebar_Sangse				= m_Options.GetOption("SANGVIEW");
	pOpt->m_bDeckRebar_SangseDanbu			= m_Options.GetOption("SANGVIEW_DANBU");
	pOpt->m_bDeckRebar_SangseDanmyunAA		= m_Options.GetOption("SANGVIEW_DANAA");
	pOpt->m_bDeckRebar_SangseWaterReinforce	= m_Options.GetOption("SANGVIEW_WATERREINFORCE");
	pOpt->m_bDeckRebar_SangseCentReinforce	= m_Options.GetOption("SANGVIEW_DOUBLECHAIN");
	pOpt->m_bDeckRebar_SangseA				= m_Options.GetOption("SANGVIEW_A");
	pOpt->m_bDeckRebar_SangseChairBlock		= m_Options.GetOption("SANGVIEW_CHAIRBLOCK");
	pOpt->m_bDeckRebar_SangseShearReinforce	= m_Options.GetOption("SANGVIEW_SHEARREINFORCE");
	pOpt->m_bDeckRebar_SangseShearArray		= m_Options.GetOption("SANGVIEW_SHEARARRAY");
	pOpt->m_bDeckRebar_SangseBlockOut		= m_Options.GetOption("SANGVIEW_BLOCKOUT");
	pOpt->m_bDeckRebar_SangseCrossBeam		= m_Options.GetOption("SANGVIEW_CROSSBEAM");
	pOpt->m_bDeckRebar_SangseStringer		= m_Options.GetOption("SANGVIEW_STRINGER");
	// 
	pOpt->m_bDeckRebar_MainRebarView		= m_Options.GetOption("MAINREBARVIEW");
	pOpt->m_bDeckRebar_BelyckView			= m_Options.GetOption("BELYCK");
	// 철근 상세
	pOpt->m_bDeckRebar_SangseRebar			= m_Options.GetOption("SANGREBAR");
	pOpt->m_bDeckRebar_SangseMatrial		= m_Options.GetOption("SANGREBAR_MATERIAL");
	pOpt->m_bDeckRebar_SangseMatrialTable	= m_Options.GetOption("SANGREBAR_MATERIALTABLE");
	//방호벽 상세 
	pOpt->m_bDeckRebar_SangseGuard			= m_Options.GetOption("SANGGUARD");
	pOpt->m_bDeckRebar_GuardSection			= m_Options.GetOption("SANGGUARD_SECTION");
	pOpt->m_bDeckRebar_GuardDetail			= m_Options.GetOption("SANGGUARD_DETAIL");
	pOpt->m_bDeckRebar_GuardTable			= m_Options.GetOption("SANGGUARD_TABLE");
	pOpt->m_nDeck_Slab_DivType				= m_Options.GetOption("DIVTYPE");			
	pOpt->m_nDeck_Slab_RebarTableOpt		= m_Options.GetOption("REBAR_TABLE_OPT");	
	pOpt->m_nTypeRebarTable					= m_Options.GetOption("REBAR_TABLE");	
	pOpt->m_dDeck_ScaleRebarPlan					= m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dDeck_ScaleRebarJong					= m_Grid.GetTextMatrixDouble(2,1);
	//종단면배근 형상
	CString str = m_Grid.GetTextMatrix(3,1);
	if(str == "슬래브중심")			pOpt->m_nDeck_JongDrawPos = 0;
	else if(str == "슬래브좌측")	pOpt->m_nDeck_JongDrawPos = 1;
	else if(str == "슬래브우측")	pOpt->m_nDeck_JongDrawPos = 2;
	pOpt->m_dDeck_ScaleRebarFront					= m_Grid.GetTextMatrixDouble(4,1);
	//횡단면도 위치
	str = m_Grid.GetTextMatrix(5,1);
	if(str == "지점부")				pOpt->m_dDeck_TypeRebarFrontLeft = 0;
	else if(str == "일반부")		pOpt->m_dDeck_TypeRebarFrontLeft = 1;
	else if(str == "없음")			pOpt->m_dDeck_TypeRebarFrontLeft = 2;
	str = m_Grid.GetTextMatrix(6,1);
	if(str == "지점부")				pOpt->m_dDeck_TypeRebarFrontRigh = 0;
	else if(str == "일반부")		pOpt->m_dDeck_TypeRebarFrontRigh = 1;
	else if(str == "없음")			pOpt->m_dDeck_TypeRebarFrontRigh = 2;
	pOpt->m_dDeck_ScaleRebarMain					= m_Grid.GetTextMatrixDouble(7,1);
	pOpt->m_dDeck_ScaleRebarBelyck					= m_Grid.GetTextMatrixDouble(8,1);
	pOpt->m_dDeck_ScaleRebarSangseDanbu				= m_Grid.GetTextMatrixDouble(9,1);
	pOpt->m_dDeck_ScaleRebarSangseDanmyunAA			= m_Grid.GetTextMatrixDouble(10,1);
	pOpt->m_dDeck_ScaleRebarSangseWaterReinforce	= m_Grid.GetTextMatrixDouble(11,1);
	pOpt->m_dDeck_ScaleRebarSangseCentReinforce		= m_Grid.GetTextMatrixDouble(12,1);
	pOpt->m_dDEck_ScaleRebarSangseDoubleChain		= m_Grid.GetTextMatrixDouble(13,1);
	pOpt->m_dDeck_ScaleSangseA						= m_Grid.GetTextMatrixDouble(14,1);
	pOpt->m_dDeck_ScaleSangseChairBlock				= m_Grid.GetTextMatrixDouble(15,1);
	pOpt->m_dDeck_SacleSangseShearReinforce			= m_Grid.GetTextMatrixDouble(16,1);
	pOpt->m_dDeck_ScaleSangseShearArray				= m_Grid.GetTextMatrixDouble(17,1);
	pOpt->m_dDeck_ScaleSangseBlockOut				= m_Grid.GetTextMatrixDouble(18,1);
	pOpt->m_dDeck_ScaleSangseCrossBeam				= m_Grid.GetTextMatrixDouble(19,1);
	pOpt->m_dDeck_ScaleSangseStringer				= m_Grid.GetTextMatrixDouble(20,1);
	pOpt->m_dDeck_SpaceBetweenJewon					= m_Grid.GetTextMatrixDouble(21,1);
	pOpt->m_dDeck_DatasuOnRebarTable				= m_Grid.GetTextMatrixDouble(22,1);
	pOpt->m_dDeck_ScaleGuardRebar					= m_Grid.GetTextMatrixDouble(23,1);
	str = m_Grid.GetTextMatrix(24,1);
	if(str == "적용")				pOpt->m_bDeck_ShowJointMainRebar = TRUE;
	else if(str == "미적용")		pOpt->m_bDeck_ShowJointMainRebar = FALSE;
	str = m_Grid.GetTextMatrix(25,1);
	if(str == "적용")				pOpt->m_bDeck_ShowJointBeryukRebar = TRUE;
	else if(str == "미적용")		pOpt->m_bDeck_ShowJointBeryukRebar = FALSE;
	pOpt->m_dWidthRebarTableRect					= m_Grid.GetTextMatrixDouble(26,1);
	pOpt->m_dHeightRebarTableRect					= m_Grid.GetTextMatrixDouble(27,1);
	pStd->GetDrawPageMng()->SetModified(PAGE_DECKREBAR);
	CPropertyPage::OnOK();
}

BOOL COptionDeckRebar::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_DECKREBAR;

	return CPropertyPage::OnSetActive();
}

void COptionDeckRebar::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

void COptionDeckRebar::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL COptionDeckRebar::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strComboArrJong, strComboArrFront, strComboArrApply;
	
	strComboArrJong.Add("슬래브중심");
	strComboArrJong.Add("슬래브좌측");
	strComboArrJong.Add("슬래브우측");

	strComboArrFront.Add("지점부");
	strComboArrFront.Add("일반부");
	strComboArrFront.Add("없음");

	strComboArrApply.Add("적용");
	strComboArrApply.Add("미적용");

	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col == 1 && next.row == 3)
			m_Grid.SetComboString(strComboArrJong);		
		if(next.col == 1 && next.row == 5)
			m_Grid.SetComboString(strComboArrFront);		
		if(next.col == 1 && next.row == 6)
			m_Grid.SetComboString(strComboArrFront);		
		if(next.col == 1 && next.row == 24)
			m_Grid.SetComboString(strComboArrApply);		
		if(next.col == 1 && next.row == 25)
			m_Grid.SetComboString(strComboArrApply);		
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

