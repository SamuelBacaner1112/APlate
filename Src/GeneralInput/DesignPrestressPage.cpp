// DesignPrestressPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "../APlateCalc/APlateCalc.h"
#include "DesignPrestressPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPage dialog


CDesignPrestressPage::CDesignPrestressPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignPrestressPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignPrestressPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignPrestressPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignPrestressPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignPrestressPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignPrestressPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPage message handlers
SHGRIDTITLE CDesignPrestressPage::m_gt[COLUMN_COUNT_PS] = 
{

	{"\n구    분\n",							"%s",		80,	DT_RIGHT,	TRUE},
	{"\n관련규격\n",							"%s",		80,	DT_RIGHT,	TRUE},
	{"\n기    호\n",							"%s",		110,DT_RIGHT,	TRUE},
	{"\n호 칭 명\n",							"%s",		120,DT_RIGHT,	TRUE},
	{"\n항복강도\nfpy(MPa)\n",					"%-.lf",	120,DT_RIGHT,	TRUE},
	{"\n인장강도\nfpu(MPa)\n",					"%-.lf",	120,DT_RIGHT,	TRUE},
	{"0.2%영구\n연신율에\n대한하중\n(N)",		"%-.2lf",	90,	DT_RIGHT,	FALSE},
	{"\n인장하중\n(N)",							"%-.2lf",	90,	DT_RIGHT,	TRUE},
	{"\n연 신 율\n(%)",							"%-.2lf",	90,	DT_RIGHT,	TRUE},
	{"\n릴렉세이션율\nNormal(%)",				"%-.2lf",	90,	DT_RIGHT,	FALSE},
	{"\n릴렉세이션율\nLow(%)",					"%-.2lf",	90,	DT_RIGHT,	FALSE},
	{"\n공칭단면적\n(mm²)",					"%-.2lf",	90,	DT_RIGHT,	FALSE},
	{"\n단위무게\n(N/mm)",						"%-.2lf",	90,	DT_RIGHT,	FALSE},
	
};

void CDesignPrestressPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignPrestressPage::SetDataDefault()
{
	CSteelPrestressPlate *pPrestress = m_pDataManage->GetSteelPrestress();
	pPrestress->SetDataInitGangsun();
	SetDataInit();
}
void CDesignPrestressPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	// 그리드 기본 셋팅 
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(ROW_COUNT_PS);
	m_Grid.SetColumnCount(COLUMN_COUNT_PS);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(3);
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT_PS, TRUE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);
	
	//m_Grid.SetColumnCount(11);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 52);

	//항복강도
	m_Grid.SetColumnWidth(4, 70);
	//인장강도
	m_Grid.SetColumnWidth(5, 70);

}
void CDesignPrestressPage::SetDataInit()
{
	CSteelPrestressPlate *pPrestress = m_pDataManage->GetSteelPrestress();
	SetGridTitle();
	long nSize = pPrestress->GetSizePsGangsun();
	m_Grid.SetMergeRow(1, 4, 0);
	m_Grid.SetMergeRow(5, ROW_COUNT_PS-1, 0);
	for(long nRow=1; nRow < nSize+1; nRow++)
	{
		CSteelPrestressData *pPrestressData = pPrestress->GetSteelPrestressData(nRow-1);
		long nCol = 1;
		m_Grid.SetTextMatrix(nRow, 0, pPrestressData->m_szDivision);
		m_Grid.SetTextMatrix(nRow, nCol++, pPrestressData->m_szStandard);
		m_Grid.SetTextMatrix(nRow, nCol++, pPrestressData->m_szSymbol);
		m_Grid.SetTextMatrix(nRow, nCol++, pPrestressData->m_szName);
		
		m_Grid.SetItemState(nRow,	nCol,	GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol,	RGBREADONLY);
		m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", GetValueUnitChange(pPrestressData->GetYieldStrength(), _T("kgf/cm²"), _T("MPa")));

		m_Grid.SetItemState(nRow,	nCol,	GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol,	RGBREADONLY);
		m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", GetValueUnitChange(pPrestressData->GetTensionStrength(), _T("kgf/cm²"), _T("MPa")));

		m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", GetValueUnitChange(pPrestressData->m_dLoadOfElongation, _T("kgf"), _T("N")));
		m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", GetValueUnitChange(pPrestressData->m_dTensionLoad, _T("kgf"), _T("N")));
		m_Grid.SetTextMatrix(nRow, nCol++, "%.1f", pPrestressData->m_dElongation);
		m_Grid.SetTextMatrix(nRow, nCol++, "%.1f", pPrestressData->m_dRelaxationNor);
		m_Grid.SetTextMatrix(nRow, nCol++, "%.1f", pPrestressData->m_dRelaxationLow);
		m_Grid.SetTextMatrix(nRow, nCol++, "%.2f", pPrestressData->m_dArea);
		m_Grid.SetTextMatrix(nRow, nCol++, "%.3f", GetValueUnitChange(pPrestressData->m_dUnitWeight, _T("kgf"), _T("N")));
	}
	m_Grid.SetMergeRow(1, ROW_COUNT_PS-1, 1);
	m_Grid.SetMergeRow(1, 4, 2);
	m_Grid.SetMergeRow(7, 10, 2);
	m_Grid.SetMergeRow(11, 14, 2);
	m_Grid.SetMergeRow(15, ROW_COUNT_PS-1, 2);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignPrestressPage::SetDataSave()
{
	CSteelPrestressPlate *pPrestress = m_pDataManage->GetSteelPrestress();
	long	nSize	= pPrestress->GetSizePsGangsun();
	long	nCol	= 3;

	for(long nRow=1; nRow < nSize+1; nRow++)
	{
		nCol	= 3;
		CSteelPrestressData *pPrestressData = pPrestress->GetSteelPrestressData(nRow-1);
		pPrestressData->m_szName			= m_Grid.GetTextMatrix(nRow, nCol++);
		
		nCol +=2;	//항복강도, 인장강도는 ReadOnly이기 때문에 저장하지 않는다.

		pPrestressData->m_dLoadOfElongation	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pPrestressData->m_dTensionLoad		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pPrestressData->m_dElongation		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pPrestressData->m_dRelaxationNor	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pPrestressData->m_dRelaxationLow	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pPrestressData->m_dArea				= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pPrestressData->m_dUnitWeight		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	}
}

void CDesignPrestressPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}
