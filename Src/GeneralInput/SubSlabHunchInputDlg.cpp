// SubSlabHunchInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"
#include "SubSlabHunchInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubSlabHunchInputDlg dialog


CSubSlabHunchInputDlg::CSubSlabHunchInputDlg(CGeneralBaseStd *pStd, CPlateBridgeApp *pBridge, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CSubSlabHunchInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubSlabHunchInputDlg)
	//}}AFX_DATA_INIT
	m_pStd = pStd;
	m_pBridge = pBridge;	

	m_DomView.m_bEnablePopupMenu = TRUE;
}


void CSubSlabHunchInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubSlabHunchInputDlg)
	//DDX_Control(pDX, IDC_EDIT_BASELHEIGHT, m_ctrlEdit1);
	//DDX_Control(pDX, IDC_EDIT_BASERHEIGHT, m_ctrlEdit2);
	DDX_Control(pDX, IDC_EDIT_BASELHEIGHT, m_dBaseLeftHeight);
	DDX_Control(pDX, IDC_EDIT_BASERHEIGHT, m_dBaseRightHeight);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_DomControl(pDX, IDC_DOMYUNVIEW, m_DomView);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);
	DDX_Control(pDX, IDOK, m_btnOK);
}


BEGIN_MESSAGE_MAP(CSubSlabHunchInputDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CSubSlabHunchInputDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_EN_KILLFOCUS(IDC_EDIT_BASELHEIGHT, OnKillfocusEditBaselheight)
	ON_EN_KILLFOCUS(IDC_EDIT_BASERHEIGHT, OnKillfocusEditBaserheight)
	ON_EN_SETFOCUS(IDC_EDIT_BASELHEIGHT, OnSetfocusEditBaselheight)
	ON_EN_SETFOCUS(IDC_EDIT_BASERHEIGHT, OnSetfocusEditBaserheight)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_COL, IDC_GRID, OnCellChangedCol)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubSlabHunchInputDlg message handlers

BOOL CSubSlabHunchInputDlg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();		

	m_DomView.GetDomyun()->SetCWnd(&m_DomView);

	SetDataInit();
	DrawInputDomyunView();

	return TRUE;
}

void CSubSlabHunchInputDlg::SetDataInit()
{
	CString	sText	= _T("");

	SetGridTitle();

	sText.Format("%g",	m_pBridge->m_dThickSlabLeftExt);
	m_dBaseLeftHeight.SetWindowText(sText);

	sText.Format("%g",	m_pBridge->m_dThickSlabRightExt);
	m_dBaseRightHeight.SetWindowText(sText);

	UpdateData(FALSE);

	if(m_pBridge->m_dHunchArray.GetSize()==0)
	{
		for (long nCol = 0; nCol < m_pBridge->GetGirdersu()*2; nCol+=2) 
		{
			CString szText;
			m_Grid.SetTextMatrix(1, nCol, "500");
			m_pBridge->m_dHunchArray.Add(500);
			m_Grid.SetTextMatrix(1, nCol+1, "500");			
			m_pBridge->m_dHunchArray.Add(500);
		}		
	}
	else
	{
		for (long nCol = 0; nCol < m_pBridge->GetGirdersu()*2; nCol+=2) 
		{
			CString szText;
			m_Grid.SetTextMatrix(1, nCol, "%g", m_pBridge->m_dHunchArray.GetAt(nCol));
			m_pBridge->m_dHunchArray.Add(m_pBridge->m_dHunchArray.GetAt(nCol));
			m_Grid.SetTextMatrix(1, nCol+1, "%g", m_pBridge->m_dHunchArray.GetAt(nCol+1));			
			m_pBridge->m_dHunchArray.Add(m_pBridge->m_dHunchArray.GetAt(nCol+1));
		}		
	}
}

void CSubSlabHunchInputDlg::SetGridTitle()
{
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(2);
	m_Grid.SetColumnCount(m_pBridge->GetGirdersu()*2);

	m_Grid.SetRowHeightAll(20);
	//m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(60);

	for (long nCol = 0; nCol < m_pBridge->GetGirdersu()*2; nCol+=2) 
	{
		CString szText;
		szText.Format("G%dL", nCol/2 + 1);
		m_Grid.SetTextMatrix(0, nCol, szText);
		szText.Format("G%dR", nCol/2 + 1);
		m_Grid.SetTextMatrix(0, nCol+1, szText);
	}
	m_Grid.ExpandRowsToFit();
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CSubSlabHunchInputDlg::SetDataSave()
{
	CString	sText	= _T("");

	UpdateData();

	m_dBaseLeftHeight.GetWindowText(sText);

	m_pBridge->m_dThickSlabLeftExt = atof(sText);

	m_dBaseRightHeight.GetWindowText(sText);
	m_pBridge->m_dThickSlabRightExt = atof(sText);

	m_pBridge->m_dHunchArray.RemoveAll();
	for (long nCol = 0; nCol < m_Grid.GetColumnCount(); nCol++) 
	{
		m_pBridge->m_dHunchArray.Add(m_Grid.GetTextMatrixDouble(1, nCol));		
	}	
}

void CSubSlabHunchInputDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd *pOpt = m_pStd->GetDataManage()->GetOptionStd();
	CDomyun *pDom = m_DomView.GetDomyun();	
	pDom->ClearEtt(TRUE);	

	CDomyun Dom(pDom);	
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	CAPlateDrawDanmyun DrawDanmyun(m_pStd->GetDataManage());
	CAPlateDrawCross DrawCross(m_pStd->GetDataManage());

	CPlateBasicIndex *pBx = m_pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, CAPlateDrawCross::NOTDRAWVSTIFF);	// 지점부 슬래브 단면
	DrawDanmyun.DrawGuardWall(&Dom, pBx);	// 방호벽
	
	///< 치수선
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 40*(dDiagonalL/8500);
	Dom.SetScaleDim(dScale);
	DrawDanmyun.DimSlabUserHunch(&Dom, pBx);	
	*pDom << Dom;

	if(bZoomAll) m_DomView.ZoomAll();
	else		 m_DomView.RedrawAll();
}

void CSubSlabHunchInputDlg::OnOK() 
{
	SetDataSave();
	
	CXTResizeDialog::OnOK();
}

void CSubSlabHunchInputDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CSubSlabHunchInputDlg::OnCellChangedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
    int nCol = GridView->iColumn;

	m_DomView.GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

BOOL CSubSlabHunchInputDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->hwnd == m_DomView.GetSafeHwnd() && (pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_MBUTTONUP))
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	else if(pMsg->message == WM_MOUSEWHEEL)
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CSubSlabHunchInputDlg::OnSetfocusEditBaselheight() 
{	
	m_DomView.GetDomyun()->STMakeCursor("H1");	
}

void CSubSlabHunchInputDlg::OnKillfocusEditBaselheight() 
{	
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CSubSlabHunchInputDlg::OnSetfocusEditBaserheight() 
{	
	m_DomView.GetDomyun()->STMakeCursor("H2");
}

void CSubSlabHunchInputDlg::OnKillfocusEditBaserheight() 
{	
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CSubSlabHunchInputDlg::OnButtonApply() 
{
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}
