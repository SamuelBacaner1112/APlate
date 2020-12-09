// DetSlabAnchor.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetSlabAnchor.h"

#include "../APlateDBDraw/APlateDBDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetSlabAnchor dialog


CDetSlabAnchor::CDetSlabAnchor(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetSlabAnchor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetSlabAnchor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStd = pStd;
}


void CDetSlabAnchor::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetSlabAnchor)
	DDX_GridControl(pDX, IDC_GRID_ANCHOR, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetSlabAnchor, TStackedPage)
	//{{AFX_MSG_MAP(CDetSlabAnchor)

	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_ANCHOR, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_ANCHOR, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID_ANCHOR, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetSlabAnchor message handlers
void CDetSlabAnchor::OnPreInitDialog()
{
	SetResize(IDC_GRID_ANCHOR, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDetSlabAnchor::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDetSlabAnchor::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CARoadOptionStd		*pOptStd	= m_pStd->GetDataManage()->GetOptionStd();
	CDomyun				*pDom		= m_pStd->GetDataManage()->GetDomyun();
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	CPlateBasicIndex	*pBx		= pGir->GetBxOnJijum(0);

	pDom->ClearEtt(TRUE);

	CDPoint	ptStt(0, 0), ptEnd(0, 0);
	double	dExt	= 270;
	CDRect  RectAnchor, RectStud, RectSQ, BoundRect;
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CAPlateDrawDanmyun DrawDanmyun(m_pStd->GetDataManage());
	CAPlateDrawEtc DrawEtc(m_pStd->GetDataManage());

	// 슬래브 앵커
	DrawEtc.DrawInputSlabAnchor(&Dom);
	Dom.SetCalcExtRect();
	RectAnchor = Dom.GetExtRect();
	Dom.Move(0, RectAnchor.Height()*0.2);
	*pDom << Dom;
	
	// 전단연결재
	Dom.SetScaleDim(10);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	DrawDanmyun.DrawGirderOnBx(&Dom, pBx);
	DrawDanmyun.DrawFrontUpperStudOrAnchorBx(&Dom, pBx, 1);
	Dom.SetCalcExtRect();
	RectStud = Dom.GetExtRect();
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DrawDanmyun.DrawDimFrontUpperStudOrAnchorBx(&Dom, pBx, 1);
	ptStt = CDPoint(RectStud.left-dExt, RectStud.bottom-RectStud.Height()/2);
	ptEnd = CDPoint(RectStud.right+dExt, RectStud.bottom-RectStud.Height()/2);
	Dom.TrimByLineOnSide(ptStt, ptEnd, 1, TRUE);
	Dom.CutLightningAnyDir(ptStt, ptEnd, 10);

	Dom.SetCalcExtRect();
	RectStud = Dom.GetExtRect();
	Dom.Move(-RectStud.left+RectStud.Width()/2, -RectStud.bottom+RectStud.Height()/2);
	*pDom << Dom;

	if(pDB->IsTUGir())
	{
		// 각형 전단연결재
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		DrawDanmyun.DrawGirderOnBx(&Dom, pBx);
		DrawDanmyun.DrawFrontUpperStudOrAnchorBx(&Dom, pBx, 2);
		Dom.SetCalcExtRect();
		RectSQ = Dom.GetExtRect();
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		DrawDanmyun.DrawDimFrontUpperStudOrAnchorBx(&Dom, pBx, 2);

		Dom.TrimByLineOnSide(ptStt, ptEnd, 1, TRUE);
		Dom.CutLightningAnyDir(ptStt, ptEnd, 10);

		Dom.SetCalcExtRect();
		RectSQ = Dom.GetExtRect();
		Dom.Move(-RectStud.left+RectStud.Width()/2, -RectStud.bottom+RectStud.Height()/2);
		Dom.Move(Dom.Always(100), 0);
		*pDom << Dom;
	}

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();
}

void CDetSlabAnchor::SetDataDefault()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();

	double	dDis	= 90.0;
	if(pDB->IsPlateGir()) dDis = 60.0;	 // 판형교

	for(long nG=-1; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
		CPlateSangSeDo	*pSang	= pGir->m_pSangse;

		pSang->m_SlabAnchor_A	= 283;
		pSang->m_SlabAnchor_B	= 100;
		pSang->m_SlabAnchor_C	= 50;
		pSang->m_SlabAnchor_T	= 19;
		pSang->m_SlabAnchorType	= 1;

		double dFlangW = pGir->GetWidthFlangeUpper(pGir->GetBxOnJijum(0));
		long nQty = (long)dFlangW/(long)dDis;

		pSang->m_StudHori_H		= 140;
		pSang->m_StudHori_E		= 10;
		pSang->m_StudHori_D		= 22;
		pSang->m_StudHori_Qty	= min(5, nQty);
		for(long n=0; n<STUD_HORI_SU; n++)
			pSang->m_StudHori_Dis[n] = dDis;

		pSang->m_StudHoriSQ_H	= 19;
		pSang->m_StudHoriSQ_W	= 19;
		pSang->m_StudHoriSQ_L	= 250;
	}

}

void CDetSlabAnchor::SetGridTitle()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);

	long	nQtyStud	= pGir->m_pSangse->m_StudHori_Qty;
	long	nCol		= 8+pGir->m_pSangse->m_StudHori_Qty;
	long	nRow		= 3;

	if(pDB->IsTUGir())	nCol += 3;

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetFixedRowCount(2);

	m_Grid.SetEditable(TRUE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));

	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 80);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(5, 80);

	m_Grid.SetTextMatrix(0, 0, "슬래브 앵커");	m_Grid.SetMergeCol(0, 0, 4);
	m_Grid.SetTextMatrix(0, 5, "전단연결재");	m_Grid.SetMergeCol(0, 5, 7+nQtyStud);
	if(pDB->IsTUGir())
	{
		m_Grid.SetTextMatrix(0, 8+nQtyStud, "각형 전단연결재");
		m_Grid.SetMergeCol(0, 8+nQtyStud, 10+nQtyStud);
	}
	// 슬래브 앵커
	m_Grid.SetTextMatrix(1, 0, "직경");
	m_Grid.SetTextMatrix(1, 1, "설치방법");
	m_Grid.SetTextMatrix(1, 2,"A");
	m_Grid.SetTextMatrix(1, 3,"B");
	m_Grid.SetTextMatrix(1, 4,"C");
	
	// 전단연결재
	m_Grid.SetTextMatrix(1, 5, "직경");
	m_Grid.SetTextMatrix(1, 6, "수량");
	m_Grid.SetTextMatrix(1, 7, "H");
	m_Grid.SetTextMatrix(1, 8, "E");

	CString	szText	= _T("");
	for(long n=0; n<nQtyStud; n++)
	{
		szText.Format("L%d", n+1);
		m_Grid.SetTextMatrix(1, 9+n, szText);
	}
	if(pDB->IsTUGir())
	{
		m_Grid.SetTextMatrix(1, 8+nQtyStud, "H");
		m_Grid.SetTextMatrix(1, 9+nQtyStud, "W");
		m_Grid.SetTextMatrix(1, 10+nQtyStud, "L");
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetSlabAnchor::SetDataInit()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateSangSeDo	*pSang  = pGir->m_pSangse;
	UpdateData(FALSE);

	long	nCol		= 0;
	long	nRow		= 2;
	CString	szText		= _T("");
	SetGridTitle();

	// 슬래브 앵커
	szText.Format("D%.0f", pSang->m_SlabAnchor_T);
	m_Grid.SetTextMatrix(nRow, nCol, szText);					m_Grid.SetCellType(nRow, nCol++, CT_COMBO);
	szText.Format("TYPE %d", (long)pSang->m_SlabAnchorType+1);
	m_Grid.SetTextMatrix(nRow, nCol, szText);					m_Grid.SetCellType(nRow, nCol++, CT_COMBO);
	m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_SlabAnchor_A);
	m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_SlabAnchor_B);
	m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_SlabAnchor_C);
	// 전단연결재
	szText.Format("D%.0f", pSang->m_StudHori_D);
	m_Grid.SetTextMatrix(nRow, nCol, szText);					m_Grid.SetCellType(nRow, nCol++, CT_COMBO);
	m_Grid.SetTextMatrix(nRow, nCol++, "%g", (double)pSang->m_StudHori_Qty);
	m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_StudHori_H);
	m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_StudHori_E);
	for(long n=0; n<pSang->m_StudHori_Qty-1; n++)
	{
		m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_StudHori_Dis[n]);
	}
	if(pDB->IsTUGir())
	{
		m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_StudHoriSQ_H);
		m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_StudHoriSQ_W);
		m_Grid.SetTextMatrix(nRow, nCol++, "%g", pSang->m_StudHoriSQ_L);
	}

	m_Grid.Invalidate();
}

BOOL CDetSlabAnchor::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strComboArr, strTypeCombo;
	strComboArr.Add("D10");
	strComboArr.Add("D13");
	strComboArr.Add("D16");
	strComboArr.Add("D19");
	strComboArr.Add("D22");
	strComboArr.Add("D25");
	strComboArr.Add("D29");
	strComboArr.Add("D32");
	strComboArr.Add("D35");

	strTypeCombo.Add("TYPE1");
	strTypeCombo.Add("TYPE2");
	strTypeCombo.Add("TYPE3");
	strTypeCombo.Add("TYPE4");

	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col == 0 || next.col == 5) 	m_Grid.SetComboString(strComboArr);
		if(next.col == 1)					m_Grid.SetComboString(strTypeCombo);
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	
	return TStackedPage::PreTranslateMessage(pMsg);
}

void CDetSlabAnchor::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	DrawInputDomyunView();
}

void CDetSlabAnchor::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	//다이알로그에 있는 콤보박스의 값을 바꿀 때 
	SetDataSave();
	SetDataInit();
	m_Grid.SetRedraw(TRUE,TRUE);

	DrawInputDomyunView();
}


void CDetSlabAnchor::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nCol = GridView->iColumn;

	DrawInputDomyunView(TRUE);
	m_pStd->GetDataManage()->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}


void CDetSlabAnchor::SetDataSave()
{
	CPlateBridgeApp* pDB = m_pStd->GetBridge();
	long nCol = 0;
	long nRow = 2;
	long nQtyStud = 0;
	CString str;

	for(long nG=-1; nG<pDB->GetGirdersu(); nG++) 
	{
		nCol = 0;
		CPlateSangSeDo *pSang  = pDB->GetGirder(nG)->m_pSangse;

		str = m_Grid.GetItemText(nRow,nCol++);
		str = str.Right(str.GetLength()-1);// D19이면 Right(3-1) D를 뺀 나머지 숫자의 길이
		pSang->m_SlabAnchor_T = atof(str);

		str = m_Grid.GetItemText(nRow,nCol++);
		str = str.Right(1);								 // Type1 ~ Type4
		pSang->m_SlabAnchorType = (BYTE)(atoi(str)-1);

		pSang->m_SlabAnchor_A = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pSang->m_SlabAnchor_B = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pSang->m_SlabAnchor_C = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		
		str = m_Grid.GetTextMatrix(nRow,nCol++);
		str = str.Right(str.GetLength()-1);// D19이면 Right(3-1) D를 뺀 나머지 숫자의 길이
		pSang->m_StudHori_D = atof(str);

		nQtyStud = pSang->m_StudHori_Qty;
		pSang->m_StudHori_Qty = m_Grid.GetTextMatrixLong(nRow, nCol++);
		pSang->m_StudHori_H = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pSang->m_StudHori_E = m_Grid.GetTextMatrixDouble(nRow, nCol++);

		for(long n=0; n<pSang->m_StudHori_Qty; n++)
		{
			pSang->m_StudHori_Dis[n] = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		}
		if(pDB->IsTUGir())
		{
			pSang->m_StudHoriSQ_H = m_Grid.GetTextMatrixDouble(nRow, 8+nQtyStud);
			pSang->m_StudHoriSQ_W = m_Grid.GetTextMatrixDouble(nRow, 9+nQtyStud);
			pSang->m_StudHoriSQ_L = m_Grid.GetTextMatrixDouble(nRow, 10+nQtyStud);
		}
	}
}
