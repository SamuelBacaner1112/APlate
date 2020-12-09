// OptionHBracing.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionHBracing property page

IMPLEMENT_DYNCREATE(COptionHBracing, CPropertyPage)

COptionHBracing::COptionHBracing() : CPropertyPage(COptionHBracing::IDD)
{
	//{{AFX_DATA_INIT(COptionHBracing)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionHBracing::~COptionHBracing()
{
}

void COptionHBracing::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionHBracing)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);	
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionHBracing, CPropertyPage)
	//{{AFX_MSG_MAP(COptionHBracing)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionHBracing message handlers

void COptionHBracing::OnOK() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_HBracing_Summary			= m_Options.GetOption("HBracing_Summary");
	pOpt->m_HBracing_Pyung				= m_Options.GetOption("HBracing_Pyung");
	pOpt->m_HBracing_Table				= m_Options.GetOption("HBracing_Table");
	pOpt->m_HBracing_Plan				= m_Options.GetOption("HBracing_Plan");
//	pOpt->m_HBracing_Front				= m_Options.GetOption("HBracing_Front");
	pOpt->m_HBracing_Detail				= m_Options.GetOption("HBracing_Detail"); 
	pOpt->m_HBracing_BM					= m_Options.GetOption("HBracing_BM");	
	pOpt->m_HBracing_Note				= m_Options.GetOption("HBracing_Note");
	
	pOpt->m_HBracing_bJiJumMark			= m_Options.GetOption("HBracing_bJiJumMark");
	pOpt->m_HBracing_bDimension			= m_Options.GetOption("HBracing_bDimension");
	pOpt->m_HBracing_bDirection			= m_Options.GetOption("HBracing_bDirection");
	pOpt->m_HBracing_bOneline			= m_Options.GetOption("HBracing_bOneline");
	pOpt->m_HBracing_bLineInfo			= m_Options.GetOption("HBracing_bLineInfo");

	double Scale = 0;
	long nSize = pOpt->m_HBracing_Scale.GetSize();
	pOpt->m_HBracing_Scale.RemoveAll();
	for(long i=1; i<nSize+1; i++)
	{
		Scale = m_Grid.GetTextMatrixDouble(i,1);
		pOpt->m_HBracing_Scale.Add(Scale);
	}
	
	CPropertyPage::OnOK();
}

BOOL COptionHBracing::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitTreeData(TRUE);
	InitGridData(TRUE);		

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionHBracing::InitTreeData(BOOL bDefaultLoad)
{	
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bHBracing_Summary				= bDefaultLoad ? TRUE : pOpt->m_HBracing_Summary; //
	BOOL bHBracing_Pyung				= bDefaultLoad ? TRUE : pOpt->m_HBracing_Pyung; //
	BOOL bHBracing_Table				= bDefaultLoad ? TRUE : pOpt->m_HBracing_Table; //
	BOOL bHBracing_Plan					= bDefaultLoad ? TRUE : pOpt->m_HBracing_Plan; //
//	BOOL bHBracing_Front				= bDefaultLoad ? TRUE : pOpt->m_HBracing_Front; //
	BOOL bHBracing_Detail				= bDefaultLoad ? TRUE : pOpt->m_HBracing_Detail; //
	BOOL bHBracing_BM					= bDefaultLoad ? TRUE : pOpt->m_HBracing_BM; //
	BOOL bHBracing_Note					= bDefaultLoad ? TRUE : pOpt->m_HBracing_Note;

	BOOL HBracing_bJiJumMark			= bDefaultLoad ? TRUE : pOpt->m_HBracing_bJiJumMark; //
	BOOL HBracing_bDimension			= bDefaultLoad ? TRUE : pOpt->m_HBracing_bDimension; //
	BOOL HBracing_bDirection			= bDefaultLoad ? TRUE : pOpt->m_HBracing_bDirection; //
	BOOL HBracing_bOneline				= bDefaultLoad ? TRUE : pOpt->m_HBracing_bOneline; //
	BOOL HBracing_bLineInfo				= bDefaultLoad ? TRUE : pOpt->m_HBracing_bLineInfo;	//

	//
	m_Options.SetOptionItem("출력도면 구성", IDB_ITEMS, IDB_ITEMS);	
//		m_Options.SetOption("HBracing_Front", bHBracing_Front ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//		m_Options.SetOptionItem("HBracing_Front", "\t정면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("HBracing_Plan", bHBracing_Plan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("HBracing_Plan", "\t평면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("HBracing_Detail", bHBracing_Detail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("HBracing_Detail", "\t수평브레이싱 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("HBracing_BM", bHBracing_BM ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("HBracing_BM", "\t재료표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("HBracing_Note", bHBracing_Note ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("HBracing_Note", "\t기본 노트", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("수평브레이싱 치수표 출력", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("HBracing_Summary", bHBracing_Summary ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HBracing_Summary", "\t수평브레이싱 요약 치수도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("HBracing_Pyung", bHBracing_Pyung ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem("HBracing_Pyung", "\t\t위치도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("HBracing_Table", bHBracing_Table ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem("HBracing_Table", "\t\t치수테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("치수표 출력옵션", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("HBracing_bJiJumMark", HBracing_bJiJumMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HBracing_bJiJumMark", "\t지점마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("HBracing_bDimension", HBracing_bDimension ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HBracing_bDimension", "\t치수선 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("HBracing_bDirection", HBracing_bDirection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HBracing_bDirection", "\t진행방향 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("HBracing_bOneline", HBracing_bOneline ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HBracing_bOneline", "\t부재 한선으로 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("HBracing_bLineInfo", HBracing_bLineInfo ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HBracing_bLineInfo", "\t선형정보 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();

	m_ListOption.Initialize(NULL);
}

void COptionHBracing::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_HBracing_Scale.GetAt(0) = 50;
		pOpt->m_HBracing_Scale.GetAt(1) = 10;		
		pOpt->m_HBracing_Scale.GetAt(2) = 500;		
		pOpt->m_HBracing_Scale.GetAt(3) = 500;		
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(5);
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
	m_Grid.SetTextMatrix(1,0,"평면도",(UINT)DT_LEFT);		
	m_Grid.SetTextMatrix(2,0,"상세도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"요약도(수평)",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(4,0,"요약도(수직)",(UINT)DT_LEFT);	
	
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_HBracing_Scale.GetAt(0),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_HBracing_Scale.GetAt(1),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_HBracing_Scale.GetAt(2),(UINT)DT_RIGHT);	
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_HBracing_Scale.GetAt(3),(UINT)DT_RIGHT);	
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionHBracing::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitGridData(TRUE);
	InitTreeData(TRUE);
}

BOOL COptionHBracing::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pStd->m_pDomOptionStd->m_nOptionCur = OPTION_PAGE_AROAD_HBRACING;
	
	return CPropertyPage::OnSetActive();
}