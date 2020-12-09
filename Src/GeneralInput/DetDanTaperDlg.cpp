// DetDanTaperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetDanTaperDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetDanTaperDlg dialog


CDetDanTaperDlg::CDetDanTaperDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetDanTaperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetDanTaperDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurIndex = 1;
	m_pStd	= pStd;
}


void CDetDanTaperDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetDanTaperDlg)
		DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetDanTaperDlg, TStackedPage)
	//{{AFX_MSG_MAP(CDetDanTaperDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED		, IDC_GRID, OnCellChanged		)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetDanTaperDlg message handlers
void CDetDanTaperDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDetDanTaperDlg::OnInitDialog() 
{	
	TStackedPage::OnInitDialog();
	
	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDetDanTaperDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pStd->GetDataManage()->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	double	dScale	= 2.0;
	CDRect   BoundRect;
	CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	Draw.DrawInputDanTaper(&Dom, dScale);
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	Dom.Move(BoundRect.left, BoundRect.bottom);
	*pDom << Dom;
	Draw.DrawInputPlateWShift(&Dom, dScale);
	*pDom << Dom;

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();
}

void CDetDanTaperDlg::SetGridTitle()
{
	long nColCount = 5;
	long nRowCount = 2;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
//	m_Grid.SetTextBkColor(RGBEDITABLE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRowCount);
	m_Grid.SetColumnCount(nColCount);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);	

	m_Grid.SetColumnWidthAll(50);

	char *szColArr[] = { "구분", "W", "H", "W1", "H1" };
	char *szRowArr[] = { "구분", "비율" };

	for(long nCol = 0; nCol < nColCount; nCol++) 
		m_Grid.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid.SetRedraw(TRUE,TRUE);
}
	
void CDetDanTaperDlg::SetDataInit()
{
	SetGridTitle();
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	m_Grid.SetTextMatrix(1, 1, "%g", pBridge->GetGirder(-1)->m_pSangse->m_Taper_W);
	m_Grid.SetTextMatrix(1, 2, "%g", pBridge->GetGirder(-1)->m_pSangse->m_Taper_H);
	m_Grid.SetTextMatrix(1, 3, "%g", pBridge->m_HTaper_W);
	m_Grid.SetTextMatrix(1, 4, "%g", pBridge->m_HTaper_H);
}

void CDetDanTaperDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	pBridge->GetGirder(-1)->m_pSangse->m_Taper_W = 2.5;
	pBridge->GetGirder(-1)->m_pSangse->m_Taper_H = 1;
	pBridge->m_HTaper_W = 2.5;
	pBridge->m_HTaper_H = 1;
}

void CDetDanTaperDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	pBridge->GetGirder(-1)->m_pSangse->m_Taper_W = m_Grid.GetTextMatrixDouble(1,1);
	pBridge->GetGirder(-1)->m_pSangse->m_Taper_H = m_Grid.GetTextMatrixDouble(1,2);
	pBridge->m_HTaper_W = m_Grid.GetTextMatrixDouble(1,3);
	pBridge->m_HTaper_H = m_Grid.GetTextMatrixDouble(1,4);
}

///////////////////////////////////////////////////////////////////////////////

void CDetDanTaperDlg::OnButtonRecommend()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "잭업 솔플레이트 상세 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	SetDataDefault();
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

///////////////////////////////////////////////////////////////////////////////

void CDetDanTaperDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;
	m_pStd->GetDataManage()->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));

	DrawInputDomyunView(TRUE);
}

void CDetDanTaperDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
//	TStackedPage::OnCellChangedData(nmgv, 0);

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetDanTaperDlg::OnCursorChange(long nRow, long nCol)
{
//	TStackedPage::OnCursorChange(nRow, nCol);

}

///////////////////////////////////////////////////////////////////////////////
