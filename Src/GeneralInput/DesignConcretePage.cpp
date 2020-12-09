// DesignConcretePage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DesignConcretePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignConcretePage dialog
SHGRIDTITLE CDesignConcretePage::m_gt[COLUMN_COUNT_CONCRET] = 
	{	
		{"\n����",							 "%s",		 50,	DT_CENTER,	TRUE}, // 0
		{"������ذ���\n\n(MPa)",			 "%-.1lf",	110,	DT_RIGHT,	FALSE},// 1
		{"\n������",						 "%-.2lf",	 55,	DT_RIGHT,	FALSE},// 2
		{"\n����â���",					 "%-.7lf",	 80,	DT_RIGHT,	FALSE},// 3
		{"ź�����\n\n(MPa)",				 "%-.lf",	 60,	DT_RIGHT,	FALSE},// 4	
		{"���� �߷�\n\n(kN/��)",			 "%-.1lf",	 80,	DT_RIGHT,	FALSE},// 5
		{"�µ�\n\n��",						 "%-.1lf",	 35,	DT_RIGHT,	FALSE},// 6
		{"\n���� ����� ",					 "%-.6lf",	 90,	DT_RIGHT,	FALSE},// 7
		{"\nũ���������1",					 "%-.2lf",	110,	DT_RIGHT,	FALSE},// 8
		{"\nũ���������2",					 "%-.2lf",	110,	DT_RIGHT,	FALSE},// 9
		{"��� �ھ�������\n\n(MPa)",		 "%-.1lf",	120,	DT_RIGHT,	FALSE},// 10
		{"��� ��������\n\n(MPa)",			 "%-.1lf",	100,	DT_RIGHT,	FALSE},// 11
	};

CDesignConcretePage::CDesignConcretePage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignConcretePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignConcretePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDataManage = pMng;
}


void CDesignConcretePage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignConcretePage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignConcretePage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignConcretePage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignConcretePage message handlers
void CDesignConcretePage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignConcretePage::SetDataInit()
{
	SetGridTitle();

	CSteelConc *pSteelConc = m_pDataManage->GetSteelConc();

	long	size = pSteelConc->GetSize();
	if(size < 1) return;
		
	m_Grid.SetRowCount(size+1);
	m_Grid.SetRowHeightAll(20);				
	m_Grid.SetRowHeight(0, 40);		

	for(long nRow = 1; nRow < size+1 ; nRow++) 
	{
		m_Grid.SetGridData(m_gt, nRow, 0, pSteelConc->GetSymbol(nRow-1));		
		m_Grid.SetGridData(m_gt, nRow, 1, pSteelConc->GetBasicStrength(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 2, pSteelConc->GetSafetyRate(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 3, pSteelConc->GetAlpha(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 4, pSteelConc->GetElasticModulus(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 5, tokNPM3(pSteelConc->GetUnitWeight(nRow-1)));
		m_Grid.SetGridData(m_gt, nRow, 6, pSteelConc->GetTemperature(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 7, pSteelConc->GetEpsilon(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 8, pSteelConc->GetCreep1(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 9, pSteelConc->GetCreep2(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 10,pSteelConc->GetBendingStress(nRow-1));
		m_Grid.SetGridData(m_gt, nRow, 11,pSteelConc->GetShearStress(nRow-1));	
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignConcretePage::SetGridTitle()
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
	m_Grid.SetColumnCount(COLUMN_COUNT_CONCRET);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);			
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT_CONCRET);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL CDesignConcretePage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignConcretePage::SetDataSave()
{
	CSteelConc *pSteelConc = m_pDataManage->GetSteelConc();
	long	nSize = pSteelConc->GetSize();

	for(long nRow = 1; nRow < nSize+1 ; nRow++) 
	{
		
		pSteelConc->SetBasicStrength(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 1));
		pSteelConc->SetSafetyRate(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 2));
		pSteelConc->SetAlpha(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 3));
		pSteelConc->SetElasticModulus(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 4));
		pSteelConc->SetUnitWeight(nRow-1, frkNPM3(m_Grid.GetTextMatrixDouble(nRow, 5)));
		pSteelConc->SetTemperature(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 6));
		pSteelConc->SetEpsilon(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 7));
		pSteelConc->SetCreep1(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 8));
		pSteelConc->SetCreep2(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 9));
		pSteelConc->SetBendingStress(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 10));
		pSteelConc->SetShearStress(nRow-1, m_Grid.GetTextMatrixDouble(nRow, 11));
	}
}

void CDesignConcretePage::SetDataDefault()
{
	CSteelConc *pSteelConc = m_pDataManage->GetSteelConc();
	pSteelConc->SetDataInit();
	SetDataInit();
}
