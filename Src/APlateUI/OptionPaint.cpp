// OptionPaint.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionPaint property page

IMPLEMENT_DYNCREATE(COptionPaint, CPropertyPage)

COptionPaint::COptionPaint() : CPropertyPage(COptionPaint::IDD)
{
	//{{AFX_DATA_INIT(COptionPaint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionPaint::~COptionPaint()
{
}

void COptionPaint::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionPaint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionPaint, CPropertyPage)
	//{{AFX_MSG_MAP(COptionPaint)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionPaint message handlers
void COptionPaint::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	//
	m_Options.SetOptionItem("출력도면 구성", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("bGirGen", (bDefaultLoad ? TRUE : pOpt->m_bPaint_GirGen) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("bGirGen", "\t거더 일반", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bGirGenFront", (bDefaultLoad ? TRUE : pOpt->m_bPaint_GirGenFront) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bGirGenFront", "\t\t정면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bGirGenTable", (bDefaultLoad ? TRUE : pOpt->m_bPaint_TableBox) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bGirGenTable", "\t\t테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOption("bSection", (bDefaultLoad ? TRUE : pOpt->m_bPaint_Section) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("bSection", "\t단면도 일반", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bSectionFront", (bDefaultLoad ? TRUE : pOpt->m_bPaint_SectionFront) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bSectionFront", "\t\t정면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bSectionTable", (bDefaultLoad ? TRUE : pOpt->m_bPaint_TableSection) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bSectionTable", "\t\t테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOption("bSplice", (bDefaultLoad ? TRUE : pOpt->m_bPaint_Splice) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("bSplice", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bSpliceUp", (bDefaultLoad ? TRUE : pOpt->m_bPaint_SpliceUp) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bSpliceUp", "\t\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bSpliceWeb", (bDefaultLoad ? TRUE : pOpt->m_bPaint_SpliceWeb) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bSpliceWeb", "\t\t측판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bSpliceDown", (bDefaultLoad ? TRUE : pOpt->m_bPaint_SpliceDown) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bSpliceDown", "\t\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bSpliceTable", (bDefaultLoad ? TRUE : pOpt->m_bPaint_TableSplice) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bSpliceTable", "\t\t테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
/*
		m_Options.SetOption("bStringer", (bDefaultLoad ? TRUE : pOpt->m_bPaint_Stringer) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("bStringer", "\t세로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bStringerFront", (bDefaultLoad ? TRUE : pOpt->m_bPaint_StringerFront) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bStringerFront", "\t\t정면", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bStringerPlanUp", (bDefaultLoad ? TRUE : pOpt->m_bPaint_StringerPlanUp) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bStringerPlanUp", "\t\t평면(상부)", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bStringerPlanDown", (bDefaultLoad ? TRUE : pOpt->m_bPaint_StringerPlanDown) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bStringerPlanDown", "\t\t평면(하부)", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bStringerTable", (bDefaultLoad ? TRUE : pOpt->m_bPaint_TableStringer) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bStringerTable", "\t\t테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
*/
		m_Options.SetOption("bCross", (bDefaultLoad ? TRUE : pOpt->m_bPaint_Cross) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("bCross", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bCrossFront", (bDefaultLoad ? TRUE : pOpt->m_bPaint_CrossFront) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bCrossFront", "\t\t정면", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bCrossPlan", (bDefaultLoad ? TRUE : pOpt->m_bPaint_CrossPlan) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bCrossPlan", "\t\t평면", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("bCrossTable", (bDefaultLoad ? TRUE : pOpt->m_bPaint_TableCross) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("bCrossTable", "\t\t테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();

	m_ListOption.Initialize(NULL);
}

void COptionPaint::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_dPaint_Scale_GirGen = 20;
		pOpt->m_dPaint_Scale_Section = 20;
		pOpt->m_dPaint_Scale_Splice = 20;
		pOpt->m_dPaint_Scale_Stringer = 20;
		pOpt->m_dPaint_Scale_Cross = 20;
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
	m_Grid.SetTextMatrix(1,0,"거더일반",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"단면도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"현장이음",(UINT)DT_LEFT);
	//m_Grid.SetTextMatrix(4,0,"세로보",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"가로보",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dPaint_Scale_GirGen,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dPaint_Scale_Section,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_dPaint_Scale_Splice,(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dPaint_Scale_Stringer,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dPaint_Scale_Cross,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL COptionPaint::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData(FALSE);
	InitGridData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionPaint::OnDestroy() 
{
	CPropertyPage::OnDestroy();		
}

void COptionPaint::OnOK() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_bPaint_GirGen				= m_Options.GetOption("bGirGen");
	pOpt->m_bPaint_GirGenFront			= m_Options.GetOption("bGirGenFront");
	pOpt->m_bPaint_TableBox				= m_Options.GetOption("bGirGenTable");

	pOpt->m_bPaint_Section				= m_Options.GetOption("bSection");
	pOpt->m_bPaint_SectionFront			= m_Options.GetOption("bSectionFront");
	pOpt->m_bPaint_TableSection			= m_Options.GetOption("bSectionTable");

	pOpt->m_bPaint_Splice				= m_Options.GetOption("bSplice");
	pOpt->m_bPaint_SpliceUp				= m_Options.GetOption("bSpliceUp");
	pOpt->m_bPaint_SpliceWeb			= m_Options.GetOption("bSpliceWeb");
	pOpt->m_bPaint_SpliceDown			= m_Options.GetOption("bSpliceDown");
	pOpt->m_bPaint_TableSplice			= m_Options.GetOption("bSpliceTable");
/*
	pOpt->m_bPaint_Stringer				= m_Options.GetOption("bStringer");
	pOpt->m_bPaint_StringerFront		= m_Options.GetOption("bStringerFront");
	pOpt->m_bPaint_StringerPlanUp		= m_Options.GetOption("bStringerPlanUp");	
	pOpt->m_bPaint_StringerPlanDown		= m_Options.GetOption("bStringerPlanDown");
	pOpt->m_bPaint_TableStringer		= m_Options.GetOption("bStringerTable");
*/
	pOpt->m_bPaint_Cross				= m_Options.GetOption("bCross");
	pOpt->m_bPaint_CrossFront			= m_Options.GetOption("bCrossFront");
	pOpt->m_bPaint_CrossPlan			= m_Options.GetOption("bCrossPlan");
	pOpt->m_bPaint_TableCross			= m_Options.GetOption("bCrossTable");
	
	pOpt->m_dPaint_Scale_GirGen			= m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dPaint_Scale_Section		= m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_dPaint_Scale_Splice			= m_Grid.GetTextMatrixDouble(3,1);
//	pOpt->m_dPaint_Scale_Stringer		= m_Grid.GetTextMatrixDouble(5,1);
	pOpt->m_dPaint_Scale_Cross			= m_Grid.GetTextMatrixDouble(4,1);

	pStd->GetDrawPageMng()->SetModified(PAGE_PAINTMAP);	
	CPropertyPage::OnOK();
}

void COptionPaint::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionPaint::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_PAINT;
	
	return CPropertyPage::OnSetActive();
}
