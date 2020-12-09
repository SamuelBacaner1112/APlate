// DetLiftLugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetLiftLugDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetLiftLugDlg dialog


CDetLiftLugDlg::CDetLiftLugDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetLiftLugDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetLiftLugDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurIndex = 1;
}


void CDetLiftLugDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetLiftLugDlg)
		DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetLiftLugDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetLiftLugDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED		, IDC_GRID, OnCellChanged		)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetLiftLugDlg message handlers
void CDetLiftLugDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDetLiftLugDlg::OnInitDialog() 
{	
	CGeneralBaseDlg::OnInitDialog();
	
	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDetLiftLugDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		
	pDom->SetScaleDim(5);

	long nRow = m_nCurIndex-1;
	long nCol = 0;
	if(nRow<0) nRow = 0;
	
	bool bSidePlate = GetStateCombo(nRow+1, COL_SIDEPLATE);
	bool bAddPlate  = GetStateCombo(nRow+1, COL_ADDPLATE);

	CDomyun Dom(pDom);
	CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	Draw.DrawInputLiftLug(&Dom, nRow, nCol, bAddPlate, bSidePlate);
	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetLiftLugDlg::SetGridTitle()
{
	CPlateBridgeApp *pBridge    = m_pStd->GetBridge();
	CPlateGirderApp *pGir       = pBridge->GetGirder(-1);
	CString strTitle[COL_COUNT] = {"구분","A","B","C","D","d","R","T",
		"보강판","복부판", "iR","sA","sB","lT","rT","lW","rW"};
	int nRows = pGir->m_pSangse->m_nTonIndex + 1;		//제목 때문
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(COL_COUNT);
	m_Grid.SetFixedRowCount();

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable();
	m_Grid.EnableInsertRow();
	m_Grid.EnableInsertCol(FALSE);

//	m_Grid.SetTextBkColor(RGBEDITABLE);
	m_Grid.SetColumnWidthAll(36);
	m_Grid.SetColumnWidth(0, 67);
	m_Grid.SetColumnWidth(COL_ADDPLATE, 60);
	m_Grid.SetColumnWidth(COL_SIDEPLATE, 60);
	m_Grid.SetMaxRowCount(5);

	m_Grid.SetRowHeightAll(20);

	for(int i=0; i<COL_COUNT; i++)
		m_Grid.SetTextMatrix(0, i, strTitle[i]);
}
	
void CDetLiftLugDlg::SetDataInit()
{
	SetGridTitle();

	CPlateBridgeApp *pBridge    = m_pStd->GetBridge();
	CPlateGirderApp *pGir       = pBridge->GetGirder(-1);
	CString str[COL_COUNT];
	int nTonIndex = pGir->m_pSangse->m_nTonIndex;

	for(int i=0; i<nTonIndex; i++)
	{
		if(i<nTonIndex-1)
			str[0].Format("%2.0f톤 이하",  pGir->m_pSangse->m_LiftLug_I[i]);
		else
			str[0].Format("%2.0f톤 초과",  pGir->m_pSangse->m_LiftLug_I[i]);
		str[1].Format("%g",  pGir->m_pSangse->m_LiftLug_A[i]);
		str[2].Format("%g",  pGir->m_pSangse->m_LiftLug_B[i]);
		str[3].Format("%g",  pGir->m_pSangse->m_LiftLug_C[i]);
		str[4].Format("%g",  pGir->m_pSangse->m_LiftLug_D[i]);
		str[5].Format("%g",  pGir->m_pSangse->m_LiftLug_d[i]);
		str[6].Format("%g",  pGir->m_pSangse->m_LiftLug_R[i]);
		str[7].Format("%g",  pGir->m_pSangse->m_LiftLug_T[i]);
		str[8] = pGir->m_pSangse->m_LiftLug_bAddPlate[i] ? "있음" : "없음";
		str[9] = pGir->m_pSangse->m_LiftLug_bSidePlate[i]? "있음" : "없음";
		str[10].Format("%g", pGir->m_pSangse->m_LiftLug_iR[i]);
		str[11].Format("%g", pGir->m_pSangse->m_LiftLug_sA[i]);
		str[12].Format("%g", pGir->m_pSangse->m_LiftLug_sB[i]);
		str[13].Format("%g", pGir->m_pSangse->m_LiftLug_lT[i]);
		str[14].Format("%g", pGir->m_pSangse->m_LiftLug_rT[i]);
		str[15].Format("%g", pGir->m_pSangse->m_LiftLug_lW[i]);
		str[16].Format("%g", pGir->m_pSangse->m_LiftLug_rW[i]);

		for(int nIndex = 0; nIndex < COL_COUNT; nIndex++)
			m_Grid.SetTextMatrix(i+1,nIndex,str[nIndex]);

		SetGridCombo(i+1);
		
		SetEnableGrid(i+1);
	}
//	m_Grid.SetFocusCell(nTonIndex,0);
	m_Grid.SetRedraw(TRUE, TRUE);
}
void CDetLiftLugDlg::SetDataDefault()
{
	SetDataDefaultAll(2);
}

void CDetLiftLugDlg::SetDataDefaultAll(int nArrIndex)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);

	pGir->m_pSangse->m_nTonIndex = nArrIndex + 1;
	////////////////////////////////////////////////////////
	pGir->m_pSangse->m_LiftLug_I[nArrIndex-2] =  20;
	pGir->m_pSangse->m_LiftLug_A[nArrIndex-2] = 125;
	pGir->m_pSangse->m_LiftLug_B[nArrIndex-2] = 125;
	pGir->m_pSangse->m_LiftLug_C[nArrIndex-2] = 60;
	pGir->m_pSangse->m_LiftLug_D[nArrIndex-2] = 110;
	pGir->m_pSangse->m_LiftLug_d[nArrIndex-2] = 46;
	pGir->m_pSangse->m_LiftLug_R[nArrIndex-2] = 90;
	pGir->m_pSangse->m_LiftLug_T[nArrIndex-2] = 12;
	pGir->m_pSangse->m_LiftLug_bAddPlate[nArrIndex-2] = FALSE;
	pGir->m_pSangse->m_LiftLug_bSidePlate[nArrIndex-2]= FALSE;
	pGir->m_pSangse->m_LiftLug_iR[nArrIndex-2] = 60;
	pGir->m_pSangse->m_LiftLug_sA[nArrIndex-2] = 16;
	pGir->m_pSangse->m_LiftLug_sB[nArrIndex-2] = 16;
	pGir->m_pSangse->m_LiftLug_lT[nArrIndex-2] = 16;
	pGir->m_pSangse->m_LiftLug_rT[nArrIndex-2] = 16;
	pGir->m_pSangse->m_LiftLug_lW[nArrIndex-2] = 75;
	pGir->m_pSangse->m_LiftLug_rW[nArrIndex-2] = 75;
	////////////////////////////////////////////////////////
	pGir->m_pSangse->m_LiftLug_I[nArrIndex-1] =  30;
	pGir->m_pSangse->m_LiftLug_A[nArrIndex-1] = 180;
	pGir->m_pSangse->m_LiftLug_B[nArrIndex-1] = 180;
	pGir->m_pSangse->m_LiftLug_C[nArrIndex-1] =  60;
	pGir->m_pSangse->m_LiftLug_D[nArrIndex-1] = 120;
	pGir->m_pSangse->m_LiftLug_d[nArrIndex-1] = 46;
	pGir->m_pSangse->m_LiftLug_R[nArrIndex-1] = 75;
	pGir->m_pSangse->m_LiftLug_T[nArrIndex-1] = 16;
	pGir->m_pSangse->m_LiftLug_bAddPlate[nArrIndex-1] = FALSE;
	pGir->m_pSangse->m_LiftLug_bSidePlate[nArrIndex-1]= FALSE;
	pGir->m_pSangse->m_LiftLug_iR[nArrIndex-1] = 60;
	pGir->m_pSangse->m_LiftLug_sA[nArrIndex-1] = 16;
	pGir->m_pSangse->m_LiftLug_sB[nArrIndex-1] = 16;
	pGir->m_pSangse->m_LiftLug_lT[nArrIndex-1] = 16;
	pGir->m_pSangse->m_LiftLug_rT[nArrIndex-1] = 16;
	pGir->m_pSangse->m_LiftLug_lW[nArrIndex-1] = 75;
	pGir->m_pSangse->m_LiftLug_rW[nArrIndex-1] = 75;
	////////////////////////////////////////////////////////
	pGir->m_pSangse->m_LiftLug_I[nArrIndex] =  30;
	pGir->m_pSangse->m_LiftLug_A[nArrIndex] = 250;
	pGir->m_pSangse->m_LiftLug_B[nArrIndex] = 250;
	pGir->m_pSangse->m_LiftLug_C[nArrIndex] =  60;
	pGir->m_pSangse->m_LiftLug_D[nArrIndex] = 140;
	pGir->m_pSangse->m_LiftLug_d[nArrIndex] = 46;
	pGir->m_pSangse->m_LiftLug_R[nArrIndex] = 75;
	pGir->m_pSangse->m_LiftLug_T[nArrIndex] = 18;
	pGir->m_pSangse->m_LiftLug_bAddPlate[nArrIndex] = TRUE;
	pGir->m_pSangse->m_LiftLug_bSidePlate[nArrIndex]= TRUE;
	pGir->m_pSangse->m_LiftLug_iR[nArrIndex] = 60;
	pGir->m_pSangse->m_LiftLug_sA[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_sB[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_lT[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_rT[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_lW[nArrIndex] = 75;
	pGir->m_pSangse->m_LiftLug_rW[nArrIndex] = 75;

}

void CDetLiftLugDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for (long i=-1; i<pBridge->GetGirdersu(); i++)
 	{
		CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
		CString str;
		int nRow = 0;

		pGir->m_pSangse->m_nTonIndex = m_Grid.GetRowCount() - 1;

		for(int i=0; i <pGir->m_pSangse->m_nTonIndex ; i++)
		{
			nRow = i + 1;
			pGir->m_pSangse->m_LiftLug_I[i] = atof(m_Grid.GetTextMatrix(nRow, 0));
			pGir->m_pSangse->m_LiftLug_A[i] = atof(m_Grid.GetTextMatrix(nRow, 1));
			pGir->m_pSangse->m_LiftLug_B[i] = atof(m_Grid.GetTextMatrix(nRow, 2));
			pGir->m_pSangse->m_LiftLug_C[i] = atof(m_Grid.GetTextMatrix(nRow, 3));
			pGir->m_pSangse->m_LiftLug_D[i] = atof(m_Grid.GetTextMatrix(nRow, 4));
			pGir->m_pSangse->m_LiftLug_d[i] = atof(m_Grid.GetTextMatrix(nRow, 5));
			pGir->m_pSangse->m_LiftLug_R[i] = atof(m_Grid.GetTextMatrix(nRow, 6));
			pGir->m_pSangse->m_LiftLug_T[i] = atof(m_Grid.GetTextMatrix(nRow, 7));
			if(!GetStateCombo(nRow, COL_ADDPLATE))
				pGir->m_pSangse->m_LiftLug_bAddPlate[i] = FALSE;
			else
				pGir->m_pSangse->m_LiftLug_bAddPlate[i] = TRUE;
			if(!GetStateCombo(nRow, COL_SIDEPLATE))
				pGir->m_pSangse->m_LiftLug_bSidePlate[i] = FALSE;
			else
				pGir->m_pSangse->m_LiftLug_bSidePlate[i] = TRUE;
			pGir->m_pSangse->m_LiftLug_iR[i] = atof(m_Grid.GetTextMatrix(nRow, 10));
			pGir->m_pSangse->m_LiftLug_sA[i] = atof(m_Grid.GetTextMatrix(nRow, 11));
			pGir->m_pSangse->m_LiftLug_sB[i] = atof(m_Grid.GetTextMatrix(nRow, 12));
			pGir->m_pSangse->m_LiftLug_lT[i] = atof(m_Grid.GetTextMatrix(nRow, 13));
			pGir->m_pSangse->m_LiftLug_rT[i] = atof(m_Grid.GetTextMatrix(nRow, 14));
			pGir->m_pSangse->m_LiftLug_lW[i] = atof(m_Grid.GetTextMatrix(nRow, 15));
			pGir->m_pSangse->m_LiftLug_rW[i] = atof(m_Grid.GetTextMatrix(nRow, 16));

			SetEnableGrid(nRow);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void CDetLiftLugDlg::OnButtonRecommend()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "브라켓 보강판 부착 상세 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	SetDataDefault();
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

///////////////////////////////////////////////////////////////////////////////

BOOL CDetLiftLugDlg::PreTranslateMessage(MSG* pMsg)
{
	CStringArray szArr;

	szArr.Add("없음");
	szArr.Add("있음");

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
	{
		char ClassName[30+1];
		::GetClassName(pMsg->hwnd,ClassName,30);
		if(strcmp(ClassName,"Edit")) return TRUE;
	}

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		if(m_Grid.GetFocusCell().col==m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col == 8) 	m_Grid.SetComboString(szArr);
		if(next.col == 9) 	m_Grid.SetComboString(szArr);
	}

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CDetLiftLugDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));

	DrawInputDomyunView(TRUE);
}

void CDetLiftLugDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetLiftLugDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

}



//------------------------> 그리드줄변경시  <----------------------------
//----------------------------------------------------------------------
LRESULT CDetLiftLugDlg::OnCellChangedRow(WPARAM nmgv, LPARAM)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));

	DrawInputDomyunView(TRUE);

	return 0;
}

//------------------------> 그리드 추가시  <----------------------------
//----------------------------------------------------------------------
LRESULT CDetLiftLugDlg::OnInsertedRow(WPARAM nmgv, LPARAM)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow + 1;    	
	SetDataDefaultInserted(nRow-1);
	SetDataInit();
	DrawInputDomyunView(TRUE);
	m_Grid.SetEdgeDelete(TRUE);

	return 0;
}

//------------------------> 그리드 삭제시  <----------------------------
//----------------------------------------------------------------------
LRESULT CDetLiftLugDlg::OnDeletedRow(WPARAM, LPARAM)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	pGir->m_pSangse->m_nTonIndex--;
	SetDataInit();
	if(m_Grid.GetRowCount() == 2)
		m_Grid.SetEdgeDelete(FALSE);
	SetEnableIndex();
	return 0;
}

//--------------> 보강판, 편복부판 적용에 따른 그리드 활성화 <------------
//----------------------------------------------------------------------
void CDetLiftLugDlg::SetEnableGrid(int nRow)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);

	BOOL bAddPlate = pGir->m_pSangse->m_LiftLug_bAddPlate[nRow-1];
	BOOL bSidePlate= pGir->m_pSangse->m_LiftLug_bSidePlate[nRow-1];

	if(!bAddPlate)	//보강판 적용시
	{
		m_Grid.SetItemState(nRow, COL_IR, m_Grid.GetItemState(nRow,COL_IR) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_IR, RGBREADONLY);
		m_Grid.SetItemState(nRow, COL_LT, m_Grid.GetItemState(nRow,COL_LT) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_LT, RGBREADONLY);
		m_Grid.SetItemState(nRow, COL_RT, m_Grid.GetItemState(nRow,COL_RT) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_RT, RGBREADONLY);
	}
	else
	{
		m_Grid.SetItemState(nRow, COL_IR, m_Grid.GetItemState(nRow,COL_IR) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_IR, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, COL_LT, m_Grid.GetItemState(nRow,COL_LT) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_LT, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, COL_RT, m_Grid.GetItemState(nRow,COL_RT) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_RT, RGB(225, 250, 250));
	}
	
	if(!bSidePlate)	//편복부판 적용시
	{
		m_Grid.SetItemState(nRow, COL_SA, m_Grid.GetItemState(nRow,COL_SA) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_SA, RGBREADONLY);
		m_Grid.SetItemState(nRow, COL_SB, m_Grid.GetItemState(nRow,COL_SB) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_SB, RGBREADONLY);
		m_Grid.SetItemState(nRow, COL_LW, m_Grid.GetItemState(nRow,COL_LW) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_LW, RGBREADONLY);
		m_Grid.SetItemState(nRow, COL_RW, m_Grid.GetItemState(nRow,COL_RW) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_RW, RGBREADONLY);
	}
	else
	{
		m_Grid.SetItemState(nRow, COL_SA, m_Grid.GetItemState(nRow,COL_SA) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_SA, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, COL_SB, m_Grid.GetItemState(nRow,COL_SB) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_SB, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, COL_LW, m_Grid.GetItemState(nRow,COL_LW) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_LW, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, COL_RW, m_Grid.GetItemState(nRow,COL_RW) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, COL_RW, RGB(225, 250, 250));
	}

	SetEnableIndex();
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDetLiftLugDlg::SetEnableIndex()
{
//	m_Grid.SetItemState(1, COL_I, m_Grid.GetItemState(1,COL_I) | GVIS_READONLY);
//	m_Grid.SetItemBkColour(1, COL_I, RGBREADONLY);

	for(int i=2; i<m_Grid.GetRowCount(); i++)
	{
		m_Grid.SetItemState(i, COL_I, m_Grid.GetItemState(i,COL_I) &~ GVIS_READONLY);
		m_Grid.SetItemBkColour(i, COL_I, RGB(225, 250, 250));
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}
void CDetLiftLugDlg::SetGridCombo(int nRow)
{
	CStringArray aStrCombo;
	aStrCombo.Add("없음");
	aStrCombo.Add("있음");
	m_Grid.SetComboString(aStrCombo);
	m_Grid.SetCellType(nRow, COL_ADDPLATE, CT_COMBO);
	m_Grid.SetCellType(nRow, COL_SIDEPLATE, CT_COMBO);
}


bool CDetLiftLugDlg::GetStateCombo(int nRow, int nCol)
{
	if(m_Grid.GetTextMatrix(nRow, nCol) == "없음")
		return FALSE;
	else
		return TRUE;
}

void CDetLiftLugDlg::SetDataDefaultInserted(int nArrIndex)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);

	pGir->m_pSangse->m_nTonIndex = nArrIndex + 1;

	pGir->m_pSangse->m_LiftLug_I[nArrIndex] = 0;
	pGir->m_pSangse->m_LiftLug_A[nArrIndex] = 180;
	pGir->m_pSangse->m_LiftLug_B[nArrIndex] = 180;
	pGir->m_pSangse->m_LiftLug_C[nArrIndex] = 60;
	pGir->m_pSangse->m_LiftLug_D[nArrIndex] = 120;
	pGir->m_pSangse->m_LiftLug_d[nArrIndex] = 46;
	pGir->m_pSangse->m_LiftLug_R[nArrIndex] = 75;
	pGir->m_pSangse->m_LiftLug_T[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_bAddPlate[nArrIndex] = FALSE;
	pGir->m_pSangse->m_LiftLug_bSidePlate[nArrIndex]   = FALSE;
	pGir->m_pSangse->m_LiftLug_iR[nArrIndex] = 60;
	pGir->m_pSangse->m_LiftLug_sA[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_sB[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_lT[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_rT[nArrIndex] = 16;
	pGir->m_pSangse->m_LiftLug_lW[nArrIndex] = 75;
	pGir->m_pSangse->m_LiftLug_rW[nArrIndex] = 75;
}
