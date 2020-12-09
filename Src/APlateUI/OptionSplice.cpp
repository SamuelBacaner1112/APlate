// OptionSplice.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSplice property page

IMPLEMENT_DYNCREATE(COptionSplice, CPropertyPage)

COptionSplice::COptionSplice() : CPropertyPage(COptionSplice::IDD)
{
	//{{AFX_DATA_INIT(COptionSplice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionSplice::~COptionSplice()
{
}

void COptionSplice::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionSplice)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionSplice, CPropertyPage)
	//{{AFX_MSG_MAP(COptionSplice)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSplice message handlers

void COptionSplice::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_SP_bTopView    = m_Options.GetOption("TOP_FLANGE");
	pOpt->m_SP_bWebView    = m_Options.GetOption("WEB_FLANGE");
	pOpt->m_SP_bBottomView = m_Options.GetOption("BOTTOM_FLANGE");
//	pOpt->m_SP_bTopRib	   = m_Options.GetOption("TOP_RIB_FLANGE");
//	pOpt->m_SP_bBottomRib  = m_Options.GetOption("BOTTOM_RIB_FLANGE");
//	pOpt->m_SP_bSangSeRib  = m_Options.GetOption("SANGSE_RIB_FLANGE");
	pOpt->m_SP_nJewon      = m_Options.GetOption("JEWON");
//	pOpt->m_SP_bHort	   = m_Options.GetOption("HORT");
	pOpt->m_SP_bAllGirder = m_Options.GetOption("bAllGirder");

	double Scale = 0;
	
	for(long i=1; i<pOpt->m_SP_Scale.GetSize()+1; i++)
	{
		Scale = m_Grid.GetTextMatrixDouble(i,1);
		pOpt->m_SP_Scale.SetAt(i-1,Scale);
	}
	pStd->GetDrawPageMng()->SetModified(PAGE_SPLICE);	
	
	CPropertyPage::OnOK();
}

BOOL COptionSplice::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData();
	InitGridData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionSplice::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void COptionSplice::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionSplice::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_SPLICE;
	
	return CPropertyPage::OnSetActive();
}

void COptionSplice::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;	
	if(bDefaultLoad)
	{
		long i=0;
		pOpt->m_SP_Scale.SetAt(i++,20);
		pOpt->m_SP_Scale.SetAt(i++,20);
		pOpt->m_SP_Scale.SetAt(i++,20);
		pOpt->m_SP_Scale.SetAt(i++,10);
		pOpt->m_SP_Scale.SetAt(i++,10);
		pOpt->m_SP_Scale.SetAt(i++,5);
	}
	
	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(4);
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
	m_Grid.SetTextMatrix(1,0,"상판 현장이음",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"복부판 현장이음",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"하판 현장이음",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(4,0,"상판 리브 현장이음",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(5,0,"측판 리브 현장이음",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(6,0,"모따기 상세",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_SP_Scale.GetAt(0),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_SP_Scale.GetAt(1),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_SP_Scale.GetAt(2),(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_SP_Scale.GetAt(3),(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(5,1,"%.1f",pOpt->m_SP_Scale.GetAt(4),(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(6,1,"%.1f",pOpt->m_SP_Scale.GetAt(5),(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);	
}

void COptionSplice::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	//
	BOOL bTopView     = bDefaultLoad ? TRUE : pOpt->m_SP_bTopView;
	BOOL bWebView     = bDefaultLoad ? TRUE : pOpt->m_SP_bWebView;
	BOOL bBottomView  = bDefaultLoad ? TRUE : pOpt->m_SP_bBottomView;
//	BOOL bTopRib      = bDefaultLoad ? TRUE : pOpt->m_SP_bTopRib;
//	BOOL bBottomRib   = bDefaultLoad ? TRUE : pOpt->m_SP_bBottomRib;
//	BOOL bSangSeRib   = bDefaultLoad ? TRUE : pOpt->m_SP_bSangSeRib;
//	BOOL bHort		  = bDefaultLoad ? FALSE : pOpt->m_SP_bHort;
	BOOL bAllGirder  = bDefaultLoad ? TRUE : pOpt->m_SP_bAllGirder;

	m_Options.SetOptionItem("출력도면 구성", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("TOP_FLANGE", bTopView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("TOP_FLANGE", "\t상판 현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("WEB_FLANGE", bWebView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("WEB_FLANGE", "\t복부판 현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("BOTTOM_FLANGE", bBottomView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BOTTOM_FLANGE", "\t하판 현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
//		m_Options.SetOption("TOP_RIB_FLANGE", bTopRib ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);	
//		m_Options.SetOptionItem("TOP_RIB_FLANGE", "\t상부 리브 현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
//		m_Options.SetOption("BOTTOM_RIB_FLANGE", bBottomRib ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);	
//		m_Options.SetOptionItem("BOTTOM_RIB_FLANGE", "\t하부 리브 현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
//		m_Options.SetOption("SANGSE_RIB_FLANGE", bSangSeRib ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);	
//		m_Options.SetOptionItem("SANGSE_RIB_FLANGE", "\t모따기 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
	m_Options.SetOptionItem("출력 옵션", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("bAllGirder", bAllGirder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);	
		m_Options.SetOptionItem("bAllGirder", "\t전체거더 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
//		m_Options.SetOption("HORT", bHort ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);	
//		m_Options.SetOptionItem("HORT", "\t플랜지 수평으로", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
	m_Options.SetOptionItem("제원 표현", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("JEWON", bDefaultLoad ? 0 : pOpt->m_SP_nJewon);
		m_Options.SetOptionItem("JEWON", "\t재료표", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("JEWON", "\t제원", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("JEWON", "\t재료표 & 제원", 2, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}
