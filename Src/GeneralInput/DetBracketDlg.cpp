// DetBracketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetBracketDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetBracketDlg dialog


CDetBracketDlg::CDetBracketDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetBracketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetBracketDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurIndex = 1;
	m_pStd		= pStd;
}


void CDetBracketDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetBracketDlg)
		DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetBracketDlg, TStackedPage)
	//{{AFX_MSG_MAP(CDetBracketDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED		, IDC_GRID, OnCellChanged		)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetBracketDlg message handlers
void CDetBracketDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDetBracketDlg::OnInitDialog() 
{	
	TStackedPage::OnInitDialog();
	
	return TRUE;
}

void CDetBracketDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pStd->GetDataManage()->GetDomyun();
	
	pDom->ClearEtt(TRUE);

	double dRound	= m_Grid.GetTextMatrixDouble(1, m_nCurIndex);
	double dChip	= m_Grid.GetTextMatrixDouble(2, m_nCurIndex);
	long   nCol     = m_Grid.GetFocusCell().col;
	double R        = atof(m_Grid.GetItemText(1,nCol));
	double C        = atof(m_Grid.GetItemText(2,nCol));

	CDomyun Dom(pDom);
	CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	Draw.DrawInputBracket(&Dom, dRound, dChip, R, C);
	long row = m_Grid.GetFocusCell().row;
	if(row >= 0)
	{
		CString	str = m_Grid.GetItemText(row,0);
		pDom->STMakeCursor(str);
	}

	*pDom << Dom;

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();
}

void CDetBracketDlg::SetGridTitle()
{
	long nColCount = 7;
	long nRowCount = 3;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRowCount);
	m_Grid.SetColumnCount(nColCount);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);	

	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0,  50);
	m_Grid.SetColumnWidth(3,  0);
	m_Grid.SetColumnWidth(4,  0);
	m_Grid.SetColumnWidth(5,  0);
	m_Grid.SetColumnWidth(6,  0);

	char *szColArr[] = { "구분", "가로보(상)", "가로보(하)", "세로보(상)", "세로보(하)", "외측가로보(상)", "외측가로보(하)" };
	char *szRowArr[] = { "구분", "R", "C" };

	for(long nCol = 0; nCol < nColCount; nCol++) 
		m_Grid.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid.SetFocusCell(1, 1);
}
	
void CDetBracketDlg::SetDataInit()
{
	SetGridTitle();

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CString str;
	str.Format("%.0f",pBridge->m_dRoundCrossUpper);
	m_Grid.SetTextMatrix(1,1,str);
	str.Format("%.0f",pBridge->m_dRoundChipCrossUpper);
	m_Grid.SetTextMatrix(2,1,str);
	str.Format("%.0f",pBridge->m_dRoundCrossLower);
	m_Grid.SetTextMatrix(1,2,str);
	str.Format("%.0f",pBridge->m_dRoundChipCrossLower);
	m_Grid.SetTextMatrix(2,2,str);
	//
	str.Format("%.0f",pBridge->m_dRoundStringerUpper);
	m_Grid.SetTextMatrix(1,3,str);
	str.Format("%.0f",pBridge->m_dRoundChipStringerUpper);
	m_Grid.SetTextMatrix(2,3,str);
	str.Format("%.0f",pBridge->m_dRoundStringerLower);
	m_Grid.SetTextMatrix(1,4,str);
	str.Format("%.0f",pBridge->m_dRoundChipStringerLower);
	m_Grid.SetTextMatrix(2,4,str);
	//
	str.Format("%.0f",pBridge->m_dRoundBracketUpper);
	m_Grid.SetTextMatrix(1,5,str);
	str.Format("%.0f",pBridge->m_dRoundChipBracketUpper);
	m_Grid.SetTextMatrix(2,5,str);
	str.Format("%.0f",pBridge->m_dRoundBracketLower);
	m_Grid.SetTextMatrix(1,6,str);
	str.Format("%.0f",pBridge->m_dRoundChipBracketLower);
	m_Grid.SetTextMatrix(2,6,str);
	//
	m_Grid.SetRedraw(TRUE,TRUE);

}

void CDetBracketDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	pBridge->m_dRoundCrossUpper			= 150;
	pBridge->m_dRoundChipCrossUpper		=  15;
	pBridge->m_dRoundCrossLower			= 150;
	pBridge->m_dRoundChipCrossLower		=  15;
	//
	pBridge->m_dRoundStringerUpper		= 150;
	pBridge->m_dRoundChipStringerUpper	=  15;
	pBridge->m_dRoundStringerLower		= 150;
	pBridge->m_dRoundChipStringerLower	=  15;
	//
	pBridge->m_dRoundBracketUpper			= 150;
	pBridge->m_dRoundChipBracketUpper		=  15;
	pBridge->m_dRoundBracketLower			= 150;
	pBridge->m_dRoundChipBracketLower		=  15;
}

void CDetBracketDlg::SetDataSave()
{
	CPlateBridge *pBridge = m_pStd->GetBridge();
	CPlateGirder *pGirder = pBridge->GetGirder(-1);

	pBridge->m_dRoundCrossUpper			= atof(m_Grid.GetItemText(1,1));
	pBridge->m_dRoundChipCrossUpper		= atof(m_Grid.GetItemText(2,1));
	pBridge->m_dRoundCrossLower			= atof(m_Grid.GetItemText(1,2));
	pBridge->m_dRoundChipCrossLower		= atof(m_Grid.GetItemText(2,2));
	//
	pBridge->m_dRoundStringerUpper		= atof(m_Grid.GetItemText(1,3));
	pBridge->m_dRoundChipStringerUpper	= atof(m_Grid.GetItemText(2,3));
	pBridge->m_dRoundStringerLower		= atof(m_Grid.GetItemText(1,4));
	pBridge->m_dRoundChipStringerLower	= atof(m_Grid.GetItemText(2,4));
	//
	pBridge->m_dRoundBracketUpper			= atof(m_Grid.GetItemText(1,5));
	pBridge->m_dRoundChipBracketUpper		= atof(m_Grid.GetItemText(2,5));
	pBridge->m_dRoundBracketLower			= atof(m_Grid.GetItemText(1,6));
	pBridge->m_dRoundChipBracketLower		= atof(m_Grid.GetItemText(2,6));
}

///////////////////////////////////////////////////////////////////////////////

void CDetBracketDlg::OnButtonRecommend()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "브라켓 상,하판 상세 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	SetDataDefault();
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

///////////////////////////////////////////////////////////////////////////////

void CDetBracketDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nCol;
	CString str = m_Grid.GetItemText(nRow, 0);
	m_pStd->GetDataManage()->GetDomyun()->STMakeCursor(str);
	DrawInputDomyunView(TRUE);
}

void CDetBracketDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
//	TStackedPage::OnCellChangedData(nmgv, 0);

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetBracketDlg::OnCursorChange(long nRow, long nCol)
{
//	TStackedPage::OnCursorChange(nRow, nCol);
}

///////////////////////////////////////////////////////////////////////////////
