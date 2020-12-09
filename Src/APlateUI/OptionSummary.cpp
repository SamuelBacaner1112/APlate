// OptionSummary.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSummary property page

IMPLEMENT_DYNCREATE(COptionSummary, CPropertyPage)

COptionSummary::COptionSummary() : CPropertyPage(COptionSummary::IDD)
{
	//{{AFX_DATA_INIT(COptionSummary)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionSummary::~COptionSummary()
{
}

void COptionSummary::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionSummary)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionSummary, CPropertyPage)
	//{{AFX_MSG_MAP(COptionSummary)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSummary message handlers
void COptionSummary::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	long nDivPage	= bDefaultLoad ? 3	: pOpt->m_nSum_DivType;	

	m_Options.SetOptionItem("도면분할 방식", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIV_DOM", nDivPage);
		m_Options.SetOptionItem("DIV_DOM","\t경간 단위로 분할", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t현장이음 단위로 분할", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t가로보 개수로 분할", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t전체 교량을 한장으로", 3, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);		
}

void COptionSummary::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_dSum_Scale = 150;
		pOpt->m_dSum_MomentY = 5;
		pOpt->m_dSum_HeightY = 1.8;
		pOpt->m_dSum_FlangeY = 25;
		pOpt->m_nSum_DivNum = 1;
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
	m_Grid.SetTextMatrix(1,0,"요약도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"모멘트크기",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"거더 높이",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"플랜지두께",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"출력단위 개수",(UINT)DT_LEFT);	
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dSum_Scale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dSum_MomentY,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_dSum_HeightY,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dSum_FlangeY,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.0f",pOpt->m_nSum_DivNum,(UINT)DT_RIGHT);	
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL COptionSummary::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData(FALSE);
	InitGridData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionSummary::OnDestroy() 
{
	CPropertyPage::OnDestroy();		
}

void COptionSummary::OnOK() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_nSum_DivType	= m_Options.GetOption("DIV_DOM");
	
	pOpt->m_dSum_Scale = m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dSum_MomentY = m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_dSum_HeightY = m_Grid.GetTextMatrixDouble(3,1);
	pOpt->m_dSum_FlangeY = m_Grid.GetTextMatrixDouble(4,1);
	pOpt->m_nSum_DivNum = m_Grid.GetTextMatrixLong(5,1);
	pStd->GetDrawPageMng()->SetModified(PAGE_SUMMARY);

	CPropertyPage::OnOK();	
}

void COptionSummary::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionSummary::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_SUMMARY;
	
	return CPropertyPage::OnSetActive();
}
