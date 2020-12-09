// DesignGradePage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignGradePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignGradePage dialog
SHGRIDTITLE CDesignGradePage::m_gt[COLUMN_COUNT_GRADE] = 
	{			
		{"\n등급",							"%s",		 70,	DT_LEFT, TRUE}, // 0
		{"하중\n\n(kN)",					"%.3lf",	 70,	DT_RIGHT, FALSE},
		{"총 중량\n\n(kN)",					"%.3lf",	 80,	DT_RIGHT, FALSE},
		{"전륜하중\n\n(kN)",				"%.3lf",	 90,	DT_RIGHT, FALSE},
		{"후륜하중\n\n(kN)",				"%.3lf",	 90,	DT_RIGHT, FALSE},
		{"등분포하중\n\n(kN/m)",			"%.3lf",	100,	DT_RIGHT, FALSE},
		{"집중하중\n모멘트 계산시\n(kN)",	"%.3lf",	120,	DT_RIGHT, FALSE},
		{"집중하중\n전단력 계산시\n(kN)",	"%.3lf",	120,	DT_RIGHT, FALSE},
	};

CDesignGradePage::CDesignGradePage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignGradePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignGradePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignGradePage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignGradePage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignGradePage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignGradePage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignGradePage message handlers
void CDesignGradePage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignGradePage::SetDataInit()
{
	SetGridTitle();

	CSteelGrade *pSteelGrade = m_pDataManage->GetSteelGrade();

	long size = pSteelGrade->GetSize();
	if(size < 1) return;	
	
	m_Grid.SetRowCount(size+1);
	m_Grid.SetRowHeightAll(20);				
	m_Grid.SetRowHeight(0, 40);	

	for(long nRow = 1; nRow < size+1 ; nRow++) 
	{
		m_Grid.SetGridData(m_gt, nRow, 0, pSteelGrade->GetGrade(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 1, tokN(pSteelGrade->GetDB(nRow-1)));
		m_Grid.SetGridData(m_gt, nRow, 2, tokN(pSteelGrade->GetDBPt(nRow-1)));
		m_Grid.SetGridData(m_gt, nRow, 3, tokN(pSteelGrade->GetDBPf(nRow-1)));
		m_Grid.SetGridData(m_gt, nRow, 4, tokN(pSteelGrade->GetDBPr(nRow-1)));
		m_Grid.SetGridData(m_gt, nRow, 5, tokNPM(pSteelGrade->GetDL(nRow-1)));
		m_Grid.SetGridData(m_gt, nRow, 6, tokN(pSteelGrade->GetDLPm(nRow-1)));
		m_Grid.SetGridData(m_gt, nRow, 7, tokN(pSteelGrade->GetDLPs(nRow-1)));
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignGradePage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(COLUMN_COUNT_GRADE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);			
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT_GRADE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL CDesignGradePage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignGradePage::SetDataDefault()
{
	CSteelGrade *pSteelGrade = m_pDataManage->GetSteelGrade();
	pSteelGrade->SetDataInit();
	SetDataInit();
}

void CDesignGradePage::SetDataSave()
{
	CSteelGrade *pSteelGrade = m_pDataManage->GetSteelGrade();
	long	nSize	= pSteelGrade->GetSize();
	long	nCol	= 1;

	for(long nRow = 1; nRow < nSize+1 ; nRow++) 
	{
		nCol	= 1;
		pSteelGrade->SetDB(nRow-1,		frkN(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
		pSteelGrade->SetDBPt(nRow-1,	frkN(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
		pSteelGrade->SetDBPf(nRow-1,	frkN(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
		pSteelGrade->SetDBPr(nRow-1,	frkN(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
		pSteelGrade->SetDL(nRow-1,		frkNPM(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
		pSteelGrade->SetDLPm(nRow-1,	frkN(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
		pSteelGrade->SetDLPs(nRow-1,	frkN(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
	}
}

void CDesignGradePage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}

