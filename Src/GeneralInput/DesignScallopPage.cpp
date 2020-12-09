// DesignScallopPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignScallopPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignScallopPage dialog
SHGRIDTITLE CDesignScallopPage::m_gt[COLUMN_COUNT_SCALLOP] = 
	{
		{"단면 형식1",	"%s",	100,	DT_CENTER,	TRUE },
		{"부재명1",		"%s",	90,	DT_CENTER,	TRUE},
		{"단면 형식2",	"%s",	100,	DT_CENTER,	TRUE },
		{"부재명2",		"%s",	90,	DT_CENTER,	TRUE},
		{"단면 형식3",	"%s",	90,	DT_CENTER,	TRUE },
		{"부재명3",		"%s",	90,	DT_CENTER,	TRUE},
		{"스캘럽 형태",	"%s",	80,	DT_LEFT,	FALSE}		
	};

CDesignScallopPage::CDesignScallopPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignScallopPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignScallopPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CutType.Add("스캘럽");
	m_CutType.Add("채움");
	m_CutType.Add("모따기");
	m_CutType.Add("없음");
			
	m_pDataManage = pMng;
}


void CDesignScallopPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignScallopPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignScallopPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignScallopPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignScallopPage message handlers
void CDesignScallopPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDesignScallopPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignScallopPage::SetDataDefault()
{
	m_pDataManage->GetSteelScallop()->DataInit();	
    SetDataInit();
}

void CDesignScallopPage::SetDataInit()
{
	SetGridTitle();
	
	CSteelScallop *pSteelScallop = m_pDataManage->GetSteelScallop();

	long size = pSteelScallop->m_SectionArr.GetSize();
	if(size < 1) return;
	
	CString	str	= "", strType	= "";	
	
	m_Grid.SetRowCount(size+1);
	m_Grid.SetRowHeightAll(20);				
	m_Grid.SetRowHeight(0, 20);	

	for(long nRow = 1; nRow < size+1 ; nRow++) 
	{
		CSteelScallopData* pSW = pSteelScallop->m_SectionArr[nRow-1];
		m_Grid.SetGridData(m_gt, nRow, 0,pSW->m_strType1);
		m_Grid.SetGridData(m_gt, nRow, 1,pSW->m_strName1);
		m_Grid.SetGridData(m_gt, nRow, 2,pSW->m_strType2);
		m_Grid.SetGridData(m_gt, nRow, 3,pSW->m_strName2);
		m_Grid.SetGridData(m_gt, nRow, 4,pSW->m_strType3);
		m_Grid.SetGridData(m_gt, nRow, 5,pSW->m_strName3);
		m_Grid.SetGridData(m_gt, nRow, 6,pSteelScallop->GetCutTypeByIndex(pSW->m_nCutType));
		m_Grid.SetCellType(nRow, 6, CT_COMBO);	
	}

	for(long n=21; n<=size/*43*/; n++)
	{
		if(!(n > 45 && n <48))
			m_Grid.SetRowHeight(n, 0);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignScallopPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(COLUMN_COUNT_SCALLOP);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(6);			
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT_SCALLOP, FALSE);	
	m_Grid.SetComboString(m_CutType);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignScallopPage::SetDataSave()
{
	CSteelScallop *pSteelScallop = m_pDataManage->GetSteelScallop();
	long cRow = m_Grid.GetRowCount();
	for(long n = 1; n < cRow ; n++) 
	{
		CSteelScallopData* pSW = pSteelScallop->m_SectionArr[n-1];
		pSW->m_strType1 = m_Grid.GetItemText(n, 0);
		pSW->m_strName1 = m_Grid.GetItemText(n, 1);
		pSW->m_strType2 = m_Grid.GetItemText(n, 2);
		pSW->m_strName2 = m_Grid.GetItemText(n, 3);
		pSW->m_strType3 = m_Grid.GetItemText(n, 4);
		pSW->m_strName3 = m_Grid.GetItemText(n, 5);
		pSW->m_nCutType = pSteelScallop->GetItemValue(m_Grid.GetItemText(n, 6));
	}
}

void CDesignScallopPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}
