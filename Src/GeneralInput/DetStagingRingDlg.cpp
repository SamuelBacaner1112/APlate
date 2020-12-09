// DetStagingRingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetStagingRingDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetStagingRingDlg dialog


CDetStagingRingDlg::CDetStagingRingDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetStagingRingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetStagingRingDlg)
	m_nType = 0;
	//}}AFX_DATA_INIT
	m_nCurIndex = 1;
	m_pStd		= pStd;
}

void CDetStagingRingDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetStagingRingDlg)
	DDX_Control(pDX, IDC_COMBO_STAGINGRING, m_ComboStagingring);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetStagingRingDlg, TStackedPage)
	//{{AFX_MSG_MAP(CDetStagingRingDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_STAGINGRING, OnSelchangeComboStagingring)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED		, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetStagingRingDlg message handlers
void CDetStagingRingDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_STAGINGRING, SZ_TOP_LEFT, SZ_TOP_LEFT);
}

BOOL CDetStagingRingDlg::OnInitDialog() 
{	
	TStackedPage::OnInitDialog();
	
	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDetStagingRingDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pStd->GetDataManage()->GetDomyun();
	pDom->ClearEtt(TRUE);
	pDom->SetScaleDim(12);

	CDomyun Dom(pDom);
	CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	Draw.DrawInputStagingRing(&Dom, m_nType);
	*pDom << Dom;	

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();
}

void CDetStagingRingDlg::SetGridTitle()
{
	long nColCount = (m_nType) ? 6 : 9;
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

	char *szColArr[] = { "구분", "A", "B", "C", (m_nType) ? "W" : "D", (m_nType) ? "T" : "E", "F", "W", "T" };
	char *szRowArr[] = { "구분", "제원" };

	for(long nCol = 0; nCol < nColCount; nCol++)
		m_Grid.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid.SetRedraw(TRUE,TRUE);
}
	
void CDetStagingRingDlg::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);

	m_nType = pGir->m_pSangse->m_Dongbari_Type;
	SetGridTitle();

	long nRow = 1;
	long nCol = 1;

	CString str(_T(""));
	m_ComboStagingring.SetCurSel(m_nType);
	if(m_nType)// 플랜지 접합타입
	{
		str.Format("%g", pGir->m_pSangse->m_Dongbari_A);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_B);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_C);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_W);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_T);	m_Grid.SetTextMatrix(nRow, nCol++, str);

	}
	else// 일반 타입
	{
		str.Format("%g", pGir->m_pSangse->m_Dongbari_A);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_B);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_C);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_D);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_E);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_F);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_W);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", pGir->m_pSangse->m_Dongbari_T);	m_Grid.SetTextMatrix(nRow, nCol++, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetStagingRingDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	if(m_nType)
	{
		m_dA = pGir->m_pSangse->m_Dongbari_A = 35;
		m_dB = pGir->m_pSangse->m_Dongbari_B = 70;
		m_dC = pGir->m_pSangse->m_Dongbari_C = 20;
		m_dW = pGir->m_pSangse->m_Dongbari_W = 70;
		m_dT = pGir->m_pSangse->m_Dongbari_T = 10;
	}
	else
	{
		m_dA = pGir->m_pSangse->m_Dongbari_A = 50;
		m_dB = pGir->m_pSangse->m_Dongbari_B = 60;
		m_dC = pGir->m_pSangse->m_Dongbari_C = 20;
		m_dD = pGir->m_pSangse->m_Dongbari_D = 30;
		m_dE = pGir->m_pSangse->m_Dongbari_E = 60;
		m_dF = pGir->m_pSangse->m_Dongbari_F = 35;
		m_dW = pGir->m_pSangse->m_Dongbari_W = 90;
		m_dT = pGir->m_pSangse->m_Dongbari_T = 10;
	}
	pGir->m_pSangse->m_Dongbari_Type	= (BYTE)m_nType;
}

void CDetStagingRingDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	long nRow = 1;
	long nCol = 1;

	pGir->m_pSangse->m_Dongbari_Type	= (BYTE)m_nType;
	if(m_nType)
	{
		pGir->m_pSangse->m_Dongbari_A = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_B = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_C = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_W = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_T = m_Grid.GetTextMatrixDouble(nRow, nCol++);

	}
	else
	{
		pGir->m_pSangse->m_Dongbari_A = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_B = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_C = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_D = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_E = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_F = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_W = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pGir->m_pSangse->m_Dongbari_T = m_Grid.GetTextMatrixDouble(nRow, nCol++);
	}
}



void CDetStagingRingDlg::OnButtonRecommend()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "동바리 고리 상세 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	SetDataDefault();
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

void CDetStagingRingDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;
	m_pStd->GetDataManage()->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));

	DrawInputDomyunView(TRUE);
}

void CDetStagingRingDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
//	TStackedPage::OnCellChangedData(nmgv, 0);

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetStagingRingDlg::OnCursorChange(long nRow, long nCol)
{
//	TStackedPage::OnCursorChange(nRow, nCol);

}

void CDetStagingRingDlg::OnSelchangeComboStagingring() 
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();	
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateSangSeDo	*pSang  = pGir->m_pSangse;
	UpdateData(TRUE);

	pSang->m_Dongbari_Type	= m_ComboStagingring.GetCurSel();
	m_nType					= m_ComboStagingring.GetCurSel();
	SetDataInit();
	DrawInputDomyunView();
}
