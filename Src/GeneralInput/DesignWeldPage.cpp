// DesignWeldPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignWeldPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignWeldPage dialog
SHGRIDTITLE CDesignWeldPage::m_gt[COLUMN_COUNT_WELD] = 
	{
		{"단면 형식1",	"%s",	90,	DT_CENTER,	TRUE },
		{"부재명1",		"%s",	90,	DT_CENTER,	TRUE},
		{"단면 형식2",	"%s",	90,	DT_CENTER,	TRUE },
		{"부재명2",		"%s",	90,	DT_CENTER,	TRUE},
		{"용접 형태",	"%s",	80,	DT_LEFT,	FALSE},
		{"개선 형태",	"%s",	100,	DT_LEFT,	FALSE},
		{"적층 방법",	"%s",	100,	DT_LEFT,	FALSE},	
	};

CDesignWeldPage::CDesignWeldPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignWeldPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignWeldPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WeldType.Add("필렛 용접");
	m_WeldType.Add("맞대기 용접");
	m_WeldType.Add("T형 홈용접");	
	m_WeldType.Add("모서리 용접");
	m_WeldType.Add("플래어 용접");
	m_WeldType.Add("압접 용접");

	m_ImproveType.Add("I형 개선");
	m_ImproveType.Add("L형 개선");
	m_ImproveType.Add("K형 개선");
	m_ImproveType.Add("V형 개선");
	m_ImproveType.Add("X형 개선");
	m_ImproveType.Add("두께에 따라 V형 또는 X형 개선");	
	m_ImproveType.Add("L형+필렛용접");
	m_ImproveType.Add("K형+필렛용접");
	m_ImproveType.Add("개선하지 않음");

	m_LayType.Add("편면용접");
	m_LayType.Add("양면용접");
	m_LayType.Add("편면+백킹강판");
	m_LayType.Add("온둘레용접");

	m_pDataManage = pMng;
}


void CDesignWeldPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignWeldPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignWeldPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignWeldPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignWeldPage message handlers
void CDesignWeldPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDesignWeldPage::PreTranslateMessage(MSG* pMsg)
{	
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col == 4) 	m_Grid.SetComboString(m_WeldType);		
		if(next.col == 5) 	m_Grid.SetComboString(m_ImproveType);		
		if(next.col == 6) 	m_Grid.SetComboString(m_LayType);		
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return TStackedPage::PreTranslateMessage(pMsg);
}

void CDesignWeldPage::SetDataDefault()
{
	m_pDataManage->GetSteelWeld()->DataInit();
	SetDataInit();
}

void CDesignWeldPage::SetDataInit()
{
	SetGridTitle();
	
	CSteelWeld *pSteelWeld = m_pDataManage->GetSteelWeld();
	
	long size = pSteelWeld->m_SectionArr.GetSize();
	if(size < 1) return;
	
	CString	str	= "", strType	= "";	
	
	m_Grid.SetRowCount(size+1);
	m_Grid.SetRowHeightAll(20);
	
	for(long nRow = 1; nRow < size+1 ; nRow++) 
	{		
		CSteelWeldData* pBW = pSteelWeld->m_SectionArr[nRow-1];
		m_Grid.SetGridData(m_gt, nRow, 0,pBW->m_strType1);
		m_Grid.SetGridData(m_gt, nRow, 1,pBW->m_strName1);
		m_Grid.SetGridData(m_gt, nRow, 2,pBW->m_strType2);
		m_Grid.SetGridData(m_gt, nRow, 3,pBW->m_strName2);
		m_Grid.SetGridData(m_gt, nRow, 4,pSteelWeld->GetWeldTypeByIndex(pBW->m_nWeldType));
		m_Grid.SetGridData(m_gt, nRow, 5,pSteelWeld->GetImproveTypeByIndex(pBW->m_nImproveType));
		m_Grid.SetGridData(m_gt, nRow, 6,pSteelWeld->GetLayTypeByIndex(pBW->m_nLayType));
		m_Grid.SetCellType(nRow, 4, CT_COMBO);
		m_Grid.SetCellType(nRow, 5, CT_COMBO);
		m_Grid.SetCellType(nRow, 6, CT_COMBO);		
	}
	
	for(long n=54; n<=114; n++)
	{
		m_Grid.SetRowHeight(n, 0);		
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignWeldPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	// 그리드 기본 셋팅 
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(COLUMN_COUNT_WELD);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(4);			
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT_WELD, FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetColumnWidth(1, 120);

	m_Grid.SetRedraw(TRUE,TRUE);

}

BOOL CDesignWeldPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignWeldPage::SetDataSave()
{
	CSteelWeld *pSteelWeld = m_pDataManage->GetSteelWeld();
	long cRow = m_Grid.GetRowCount();
	for(long n = 1; n < cRow ; n++) 
	{
		CSteelWeldData* pBW = pSteelWeld->m_SectionArr[n-1];
		pBW->m_strType1 = m_Grid.GetItemText(n, 0);
		pBW->m_strName1 = m_Grid.GetItemText(n, 1);
		pBW->m_strType2 = m_Grid.GetItemText(n, 2);
		pBW->m_strName2 = m_Grid.GetItemText(n, 3);
		pBW->m_nWeldType = pSteelWeld->GetItemValue(m_Grid.GetItemText(n, 4));
		pBW->m_nImproveType = pSteelWeld->GetItemValue(m_Grid.GetItemText(n, 5));
		pBW->m_nLayType = pSteelWeld->GetItemValue(m_Grid.GetItemText(n, 6));
	}
}

void CDesignWeldPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}
