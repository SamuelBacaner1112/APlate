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
		{"�ܸ� ����1",	"%s",	90,	DT_CENTER,	TRUE },
		{"�����1",		"%s",	90,	DT_CENTER,	TRUE},
		{"�ܸ� ����2",	"%s",	90,	DT_CENTER,	TRUE },
		{"�����2",		"%s",	90,	DT_CENTER,	TRUE},
		{"���� ����",	"%s",	80,	DT_LEFT,	FALSE},
		{"���� ����",	"%s",	100,	DT_LEFT,	FALSE},
		{"���� ���",	"%s",	100,	DT_LEFT,	FALSE},	
	};

CDesignWeldPage::CDesignWeldPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignWeldPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignWeldPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WeldType.Add("�ʷ� ����");
	m_WeldType.Add("�´�� ����");
	m_WeldType.Add("T�� Ȩ����");	
	m_WeldType.Add("�𼭸� ����");
	m_WeldType.Add("�÷��� ����");
	m_WeldType.Add("���� ����");

	m_ImproveType.Add("I�� ����");
	m_ImproveType.Add("L�� ����");
	m_ImproveType.Add("K�� ����");
	m_ImproveType.Add("V�� ����");
	m_ImproveType.Add("X�� ����");
	m_ImproveType.Add("�β��� ���� V�� �Ǵ� X�� ����");	
	m_ImproveType.Add("L��+�ʷ�����");
	m_ImproveType.Add("K��+�ʷ�����");
	m_ImproveType.Add("�������� ����");

	m_LayType.Add("������");
	m_LayType.Add("������");
	m_LayType.Add("���+��ŷ����");
	m_LayType.Add("�µѷ�����");

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

	// �׸��� �⺻ ���� 
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
