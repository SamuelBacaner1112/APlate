// OptionPlanGen.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionPlanGen property page

IMPLEMENT_DYNCREATE(COptionPlanGen, CPropertyPage)

COptionPlanGen::COptionPlanGen() : CPropertyPage(COptionPlanGen::IDD)
{
	//{{AFX_DATA_INIT(COptionPlanGen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionPlanGen::~COptionPlanGen()
{
}

void COptionPlanGen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPlanGen)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPlanGen, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPlanGen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPlanGen message handlers

void COptionPlanGen::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_PlanGen_Plane_bIP		= m_Options.GetOption("PLANE_IP"	);
	pOpt->m_PlanGen_Plane_bChain	= m_Options.GetOption("PLANE_CHAIN"	);
//	pOpt->m_bPlanGen_Plane_Coord	= m_Options.GetOption("PLANE_COORD"	);

	pOpt->m_PlanGen_nDimPyung				= m_Options.GetOption("DIMENTION_PYUNG"			);
	pOpt->m_PlanGen_bMarkDanPyung			= m_Options.GetOption("MARK_DAN_PYUNG"			);
	pOpt->m_PlanGen_bJijumInfoPyung			= m_Options.GetOption("JIJUM_INFO_PYUNG"		);
	pOpt->m_PlanGen_bJijumInfoPyung_Sta		= m_Options.GetOption("JIJUM_INFO_PYUNG_STA"	);
	pOpt->m_PlanGen_bJijumInfoPyung_Mark	= m_Options.GetOption("JIJUM_INFO_PYUNG_MARK"	);
	pOpt->m_PlanGen_bPlaneInfoPyung			= m_Options.GetOption("PLANE_INFO_PYUNG"		);

	pStd->GetDrawPageMng()->SetModified(PAGE_PLANGEN);	
	
	CPropertyPage::OnOK();
}

BOOL COptionPlanGen::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitData();
	InitGridData();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPlanGen::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}

void COptionPlanGen::InitData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();


	BOOL bPlaneIP		= bDefaultLoad ? TRUE : pOpt->m_PlanGen_Plane_bIP			;
	BOOL bPlaneChain	= bDefaultLoad ? TRUE : pOpt->m_PlanGen_Plane_bChain		;
//	BOOL bPlaneCoord	= bDefaultLoad ? TRUE : pOpt->m_PlanGen_bPlane_bCoord		;

	BOOL nDimPyung				= bDefaultLoad ?     0 : pOpt->m_PlanGen_nDimPyung				;
	BOOL bMarkDanPyung			= bDefaultLoad ?  TRUE : pOpt->m_PlanGen_bMarkDanPyung			;
	BOOL bJijumInfoPyung		= bDefaultLoad ?  TRUE : pOpt->m_PlanGen_bJijumInfoPyung		;
	BOOL bJijumInfoPyungSta		= bDefaultLoad ?  TRUE : pOpt->m_PlanGen_bJijumInfoPyung_Sta	;
	BOOL bJijumInfoPyungMark	= bDefaultLoad ?  TRUE : pOpt->m_PlanGen_bJijumInfoPyung_Mark	;
	BOOL bPlaneInfoPyung		= bDefaultLoad ?  TRUE : pOpt->m_PlanGen_bPlaneInfoPyung		;

	// 평면선형도 설정
	m_Options.SetOptionItem		("평면선형 설정", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption		("PLANE_IP"		, (bPlaneIP) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem	("PLANE_IP"		, "\t평면 선형 IP Line 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption		("PLANE_CHAIN"	, (bPlaneChain) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem	("PLANE_CHAIN"	, "\t평면 선형 측점 표기", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
//		m_Options.SetOption		("PLANE_COORD"	, (bPlaneCoord) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//		m_Options.SetOptionItem	("PLANE_COORD"	, "\t좌표 도각선 표기", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem		("평면도 설정"		, IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption		("DIMENTION_PYUNG"	, nDimPyung);
		m_Options.SetOptionItem	("DIMENTION_PYUNG"	, "\t일반도 치수 표현 적용", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem	("DIMENTION_PYUNG"	, "\t일반도 치수 표현 안함", 1, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOption		("MARK_DAN_PYUNG"	, bMarkDanPyung		? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem	("MARK_DAN_PYUNG"	, "\t\t단면 마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption		("JIJUM_INFO_PYUNG"	, bJijumInfoPyung	? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem	("JIJUM_INFO_PYUNG"	, "\t\t지점 정보 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption		("JIJUM_INFO_PYUNG_STA"	, bJijumInfoPyungSta	? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem	("JIJUM_INFO_PYUNG_STA"	, "\t\t\t지점 STATION 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption		("JIJUM_INFO_PYUNG_MARK", bJijumInfoPyungMark	? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem	("JIJUM_INFO_PYUNG_MARK", "\t\t\t지점 마크 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption		("PLANE_INFO_PYUNG"	, bPlaneInfoPyung	? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem	("PLANE_INFO_PYUNG"	, "\t\t선형 정보 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionPlanGen::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(1);						// 설정 값 적용시 삭제, 아래 것 사용.
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);
	m_Grid.SetColumnWidth(0,Rect.Width()*2/3);
	m_Grid.SetColumnWidth(1,Rect.Width()*1/3);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));
	m_Grid.SetTextBkColor(RGB(255,255,255));

	m_Grid.SetTextMatrix(0, 0, "설정항목");

	m_Grid.SetTextMatrix(0, 1, "설정값");

	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionPlanGen::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionPlanGen::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pStd->m_pDomOptionStd->m_nOptionCur = OPTION_PAGE_AROAD_PLANGEN;

	return CPropertyPage::OnSetActive();
}
