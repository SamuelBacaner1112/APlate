// DetHStiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetHStiffDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetHStiffDlg dialog


CDetHStiffDlg::CDetHStiffDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetHStiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetHStiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurIndex = 1;
	m_pStd = pStd;
}


void CDetHStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetHStiffDlg)
		DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetHStiffDlg, TStackedPage)
	//{{AFX_MSG_MAP(CDetHStiffDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED		, IDC_GRID, OnCellChanged		)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetHStiffDlg message handlers
void CDetHStiffDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDetHStiffDlg::OnInitDialog() 
{	
	TStackedPage::OnInitDialog();
	
	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDetHStiffDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CPlateGirderApp *pGir    = pBridge->GetGirder(-1); 
	CPlateBxFinder FinderSp(pGir);
	CPlateBasicIndex *pBxSp = FinderSp.GetBxFirst(BX_SPLICE);
	CPlateBxFinder FinderSc(pGir);
	CPlateBasicIndex *pBxSc = FinderSc.GetBxFirst(BX_CROSSBEAM_VBRACING);

	CDomyun *pDom = m_pStd->GetDataManage()->GetDomyun();
	double Scale  = 15;
	pDom->ClearEtt(TRUE);
	pDom->SetScaleDim(Scale);
	
	CDomyun Dom(pDom);
	CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	Draw.DrawInputHStiff(&Dom, pBxSp, pBxSc, Scale);

	*pDom << Dom;

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();
}

void CDetHStiffDlg::SetGridTitle()
{
	long nColCount = 4;
	long nRowCount = 2;

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

	m_Grid.SetColumnWidthAll(50);

	char *szColArr[] = { "구분", "B", "D", "S" };
	char *szRowArr[] = { "구분", "간격" };

	for(long nCol = 0; nCol < nColCount; nCol++) 
		m_Grid.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid.SetRedraw(TRUE,TRUE);
}
	
void CDetHStiffDlg::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp * pGir   = pBridge->GetGirder(-1);
	
	SetGridTitle();

	m_Grid.SetTextMatrix(1, 1, "%g", pGir->m_pSangse->m_HStiff_VStiff_B);
	m_Grid.SetTextMatrix(1, 2, "%g", pGir->m_pSangse->m_HStiff_VStiff_D);
	m_Grid.SetTextMatrix(1, 3, "%g", pGir->m_pSangse->m_HStiff_Splice_B);
}

void CDetHStiffDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp * pGir   = pBridge->GetGirder(-1);
	
	pGir->m_pSangse->m_HStiff_VStiff_B = 35;
	pGir->m_pSangse->m_HStiff_VStiff_D = 10;
	pGir->m_pSangse->m_HStiff_Splice_B = 20;
}

void CDetHStiffDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp * pGir   = pBridge->GetGirder(-1);
	
	pGir->m_pSangse->m_HStiff_VStiff_B = atof(m_Grid.GetTextMatrix(1, 1));
	pGir->m_pSangse->m_HStiff_VStiff_D = atof( m_Grid.GetTextMatrix(1, 2));
	pGir->m_pSangse->m_HStiff_Splice_B = atof(m_Grid.GetTextMatrix(1, 3));

}

///////////////////////////////////////////////////////////////////////////////

void CDetHStiffDlg::OnButtonRecommend()
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

void CDetHStiffDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;

	m_pStd->GetDataManage()->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,nCol));

	DrawInputDomyunView(TRUE);
}

void CDetHStiffDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
//	TStackedPage::OnCellChangedData(nmgv, 0);

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetHStiffDlg::OnCursorChange(long nRow, long nCol)
{
//	TStackedPage::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,nCol));
}

///////////////////////////////////////////////////////////////////////////////
