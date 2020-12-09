// DetPlateWShiftDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetPlateWShiftDlg.h"

#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetPlateWShiftDlg dialog


CDetPlateWShiftDlg::CDetPlateWShiftDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetPlateWShiftDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetPlateWShiftDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurIndex = 1;
}


void CDetPlateWShiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetPlateWShiftDlg)
		DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetPlateWShiftDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetPlateWShiftDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED		, IDC_GRID, OnCellChanged		)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetPlateWShiftDlg message handlers
void CDetPlateWShiftDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDetPlateWShiftDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	Draw.DrawInputPlateWShift(&Dom, 2.0);
	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetPlateWShiftDlg::SetGridTitle()
{
	long nColCount = 3;
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

	char *szColArr[] = { "구분", "W", "H" };
	char *szRowArr[] = { "구분", "비율" };

	for(long nCol = 0; nCol < nColCount; nCol++) 
		m_Grid.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid.SetRedraw(TRUE,TRUE);
}
	
void CDetPlateWShiftDlg::SetDataInit()
{
	SetGridTitle();

	long nRow = 1;
	CString str(_T(""));

	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	
	str.Format("%g", pBridge->m_HTaper_W);	m_Grid.SetTextMatrix(nRow, 1, str);
	str.Format("%g", pBridge->m_HTaper_H);	m_Grid.SetTextMatrix(nRow, 2, str);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetPlateWShiftDlg::SetDataDefault()
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	pBridge->m_HTaper_W = 2.5;
	pBridge->m_HTaper_H = 1;
}

void CDetPlateWShiftDlg::SetDataSave()
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	pBridge->m_HTaper_W = atof(m_Grid.GetTextMatrix(1, 1));
	pBridge->m_HTaper_H = atof(m_Grid.GetTextMatrix(1, 2));

}

///////////////////////////////////////////////////////////////////////////////
void CDetPlateWShiftDlg::OnButtonRecommend()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "상하판 너비 변화부 상세 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	SetDataDefault();
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

///////////////////////////////////////////////////////////////////////////////

void CDetPlateWShiftDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));

	DrawInputDomyunView(TRUE);
}

void CDetPlateWShiftDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CGeneralBaseDlg::OnCellChangedData(nmgv, 0);

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetPlateWShiftDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

}

///////////////////////////////////////////////////////////////////////////////
