// SubInputTensionBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"
#include "SubInputTensionBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubInputTensionBarDlg dialog


CSubInputTensionBarDlg::CSubInputTensionBarDlg(CGeneralBaseStd *pStd, CPlateBridgeApp *pBridge, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CSubInputTensionBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubInputTensionBarDlg)
	m_nSize      = 0;
	m_dGinJangL  = 0.0;
	m_dJungChakL = 0.0;
	m_dSecMove_L = 0.0;
	m_dDuctOffset_L = 0.0;
	m_dSecMoveEnd_L = 0.0;
	//}}AFX_DATA_INIT
	m_pStd = pStd;
	m_pBridge = pBridge;
	m_DomView.m_bEnablePopupMenu = TRUE;	
}

void CSubInputTensionBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubInputTensionBarDlg)
	DDX_Control(pDX, IDC_COMBO_FIX, m_comboFix);
	DDX_Control(pDX, IDC_COMBO_FIX2, m_comboModelFix);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_nSize);
	DDX_Control(pDX, IDC_EDIT_BASELHEIGHT, m_dLHeight);
	DDX_Control(pDX, IDC_EDIT_BASERHEIGHT, m_dRHeight);
	DDX_Text(pDX, IDC_EDIT_GINJANG_L, m_dGinJangL);
	DDX_Text(pDX, IDC_EDIT_JUNGCHAK_L, m_dJungChakL);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ctrlComboBox1);
	//DDX_Control(pDX, IDC_EDIT_BASELHEIGHT, m_ctrlEdit1);
	//DDX_Control(pDX, IDC_EDIT_BASERHEIGHT, m_ctrlEdit2);
	DDX_Control(pDX, IDC_EDIT_GINJANG_L, m_ctrlEdit3);
	DDX_Control(pDX, IDC_EDIT_JUNGCHAK_L, m_ctrlEdit4);
	DDX_Control(pDX, IDC_EDIT_SECMOVE_L, m_ctrlEdit5);
	DDX_Control(pDX, IDC_EDIT_DUCTOFFSET_L, m_ctrlEdit6);
	DDX_Control(pDX, IDC_EDIT_SECMOVEEND_L, m_ctrlEdit7);
	DDX_Text(pDX, IDC_EDIT_SECMOVE_L, m_dSecMove_L);
	DDX_Text(pDX, IDC_EDIT_DUCTOFFSET_L, m_dDuctOffset_L);
	DDX_Text(pDX, IDC_EDIT_SECMOVEEND_L, m_dSecMoveEnd_L);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_DomControl(pDX, IDC_DOMYUNVIEW, m_DomView);
	DDX_Control(pDX, ID_BUTTON_RECOMMEND, m_btnRecommend);
	DDX_Control(pDX, ID_BUTTON_DXFOUT, m_btnDXFOut);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);
	DDX_Control(pDX, IDOK, m_btnOK);
}

BEGIN_MESSAGE_MAP(CSubInputTensionBarDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CSubInputTensionBarDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, OnSelchangeComboSize)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(ID_BUTTON_DXFOUT, OnButtonDxfout)
	ON_CBN_SELCHANGE(IDC_COMBO_FIX, OnSelchangeComboFix)
	ON_CBN_SELCHANGE(IDC_COMBO_FIX2, OnSelchangeComboFixDom)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_COL, IDC_GRID, OnCellChangedCol)
	ON_EN_KILLFOCUS(IDC_EDIT_BASELHEIGHT, OnKillfocusEditBaselheight)
	ON_EN_KILLFOCUS(IDC_EDIT_BASERHEIGHT, OnKillfocusEditBaserheight)
	ON_EN_SETFOCUS(IDC_EDIT_BASELHEIGHT, OnSetfocusEditBaselheight)
	ON_EN_SETFOCUS(IDC_EDIT_BASERHEIGHT, OnSetfocusEditBaserheight)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubInputTensionBarDlg message handlers

BOOL CSubInputTensionBarDlg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	m_DomView.GetDomyun()->SetCWnd(&m_DomView);
	
	m_ImageList.Create(IDB_BITMAP_CURVETYPE, 20, 1, RGB(255,255,255));
	m_Grid.SetUserImageList(&m_ImageList);

	SetDataInit();
	DrawInputDomyunView();

	SetResize(IDC_STATIC_GBOX, SZ_MIDDLE_LEFT, SZ_MIDDLE_RIGHT);
// 	SetResize(IDC_STATIC_1, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
// 	SetResize(IDC_STATIC_9, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
// 	SetResize(IDC_COMBO_SIZE, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	
	return TRUE;
}

void CSubInputTensionBarDlg::SetDataDefault()
{	
	m_pBridge->m_dHeightLeftTension  = m_pBridge->m_dThickSlabLeft/2;
	m_pBridge->m_dHeightRightTension = m_pBridge->m_dThickSlabRight/2;
	m_pBridge->m_dLengthGinjangdan   = 200;
	m_pBridge->m_dLengthJungchakdan  = 200;
	m_pBridge->m_nSlabTensionSize    = 1+(m_pBridge->GetGirdersu()-1)*2;
	m_pBridge->m_nTendonFix          = 0;	// 슬래브좌측
	m_pBridge->m_nTendonDomFix		 = 0;	// 슬래브좌측
	m_pBridge->m_dLengthSecMove		 = m_pBridge->GetLengthHDanDom(NULL, 0);
	m_pBridge->m_dLengthDuctOffset	 = 600;
	m_pBridge->m_dLengthSecMoveEnd   = 110;

	for(long n=0; n<m_pBridge->m_nSlabTensionSize; n++)
	{
		if(n%2==0) 
		{
			m_pBridge->m_SlabTension[n].m_dH = 120;
			m_pBridge->m_SlabTension[n].m_dR = 10000;
		}
		else
		{
			m_pBridge->m_SlabTension[n].m_dH = m_pBridge->m_dThickSlabBase - 100 - m_pBridge->m_dThickSlabHunch;
			m_pBridge->m_SlabTension[n].m_dR = 15000;
			/*
			CPlateBxFinder Finder(m_pBridge->GetGirder(-1));
			CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);	
			while(pBx->IsJijum()) pBx = Finder.GetBxNext();
			if(pBx) 
				m_pBridge->m_SlabTension[n].m_dH = m_pBridge->GetThickFrPaveByXy(pBx, CDPoint(m_pBridge->GetGirder(n-1)->m_dGirderCenter - dLeftX,0));
			else
				m_pBridge->m_SlabTension[n].m_dH = 250;
			*/
		}
/* 070412일자 수정...KB...
		if(n==0) 
			m_pBridge->m_SlabTension[n].m_dL = m_pBridge->GetGirder(0)->m_dGirderCenter - dLeftX;
		else if(n==m_pBridge->m_nSlabTensionSize-1)
			m_pBridge->m_SlabTension[n].m_dL = fabs(m_pBridge->GetGirder(n-1)->m_dGirderCenter - m_pBridge->GetGirder(n-2)->m_dGirderCenter)/2;
			//m_pBridge->m_SlabTension[n].m_dL = dRightX - m_pBridge->GetGirder(m_pBridge->GetGirdersu()-1)->m_dGirderCenter;
		else
			m_pBridge->m_SlabTension[n].m_dL = fabs(m_pBridge->GetGirder(n)->m_dGirderCenter - m_pBridge->GetGirder(n-1)->m_dGirderCenter)/2;

		//m_pBridge->m_SlabTension[n].m_dR = (n%2) ? 15000 : 2000;		
*/
	}

	CPlateBasicIndex *pBx		= m_pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	CPlateBasicIndex *pBxMatch	= pBx->GetBxMatchByCrossBeam(m_pBridge->GetGirder(0));
	long nGsu = m_pBridge->GetGirdersu();
	double Len = 0;
	m_pBridge->m_SlabTension[0].m_dL = m_pBridge->GetWidthSlabLeft();
	
	long nK = 1;
		
	for(long nG=0; nG < nGsu-1; nG++) 
	{
		pBxMatch = pBx->GetBxMatchByCrossBeam(m_pBridge->GetGirder(nG));		
		Len      = m_pBridge->GetGirder(nG)->GetDisGirderToGirder(pBxMatch);
		m_pBridge->m_SlabTension[nK++].m_dL = Len/2;
		m_pBridge->m_SlabTension[nK++].m_dL = Len/2;
	}		
	
	
}

void CSubInputTensionBarDlg::SetDataInit()
{	
	if(m_pBridge->m_nSlabTensionSize==0) SetDataDefault();	

	CString				sText	= _T("");
	CPlateBasicIndex	*pBx	= m_pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();

	m_pBridge->SetTendonArcType(pBx);

	m_nSize    = m_pBridge->m_nSlabTensionSize;

	sText.Format("%g",	m_pBridge->m_dHeightLeftTension);
	m_dLHeight.SetWindowText(sText);

	sText.Format("%g",	m_pBridge->m_dHeightRightTension);
	m_dRHeight.SetWindowText(sText);

	m_dGinJangL= m_pBridge->m_dLengthGinjangdan;
	m_dJungChakL= m_pBridge->m_dLengthJungchakdan;
	m_dSecMove_L = m_pBridge->m_dLengthSecMove;
	m_dDuctOffset_L = m_pBridge->m_dLengthDuctOffset;
	m_dSecMoveEnd_L = m_pBridge->m_dLengthSecMoveEnd;
	m_comboFix.SetCurSel(m_pBridge->m_nTendonFix);
	m_comboModelFix.SetCurSel(m_pBridge->m_nTendonDomFix);

	UpdateData(FALSE);	

	SetGridTitle();	
	
	for(long n=1; n<m_nSize+1; n++)
	{
		m_Grid.SetTextMatrixFormat(1,n,"%.3f",toM(m_pBridge->m_SlabTension[n-1].m_dL));
		m_Grid.SetTextMatrixFormat(2,n,"%.3f",toM(m_pBridge->m_SlabTension[n-1].m_dH));
		m_Grid.SetTextMatrixFormat(3,n,"%.3f",toM(m_pBridge->m_SlabTension[n-1].m_dR));
		m_Grid.SetUserImage(4,n,(long)m_pBridge->m_SlabTension[n-1].m_nType);	
		m_Grid.SetItemState(4, n, GVIS_READONLY);
	}

	CCalcFloor CalcFloor(m_pStd->GetDataManage());
	CalcFloor.CalcPrestressTotAngle(0);
}

void CSubInputTensionBarDlg::SetGridTitle()
{
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(5);
	m_Grid.SetColumnCount(m_nSize+1);

	m_Grid.SetRowHeightAll(1);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(0,150);

	// 상부 타이틀
	for(long nCol = 0; nCol < m_nSize; nCol++) 
	{
		CString szText;
		szText.Format("변곡점%d", nCol+1);
		m_Grid.SetTextMatrix(0, nCol+1, szText);		
		m_Grid.SetCellType(4,nCol+1, CT_USERIMAGE);
	}
	m_Grid.SetTextMatrix(0, 0, "변곡점");
	m_Grid.SetTextMatrix(1, 0, "변곡점거리(L, m)");
	m_Grid.SetTextMatrix(2, 0, "기준높이(H, m)");
	m_Grid.SetTextMatrix(3, 0, "곡선반경(R, m)");
	m_Grid.SetTextMatrix(4, 0, "곡선형태(S)");	

	m_Grid.ExpandRowsToFit();
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CSubInputTensionBarDlg::SetDataSave()
{		
	CString	sText	= _T("");

	UpdateData();

	m_pBridge->m_nSlabTensionSize    = m_Grid.GetColumnCount()-1;	

	m_dLHeight.GetWindowText(sText);
	m_pBridge->m_dHeightLeftTension  = atof(sText);

	m_dRHeight.GetWindowText(sText);
	m_pBridge->m_dHeightRightTension = atof(sText);

	m_pBridge->m_dLengthGinjangdan   = m_dGinJangL;
	m_pBridge->m_dLengthJungchakdan  = m_dJungChakL;
	m_pBridge->m_nTendonFix          = m_comboFix.GetCurSel();	
	m_pBridge->m_nTendonDomFix       = m_comboModelFix.GetCurSel();
	m_pBridge->m_dLengthSecMove		 = m_dSecMove_L;
	m_pBridge->m_dLengthDuctOffset	 = m_dDuctOffset_L;
	m_pBridge->m_dLengthSecMoveEnd   = m_dSecMoveEnd_L;

	for(long n=1; n<m_Grid.GetColumnCount(); n++)
	{
		m_pBridge->m_SlabTension[n-1].m_dL = frM(m_Grid.GetTextMatrixDouble(1,n));
		m_pBridge->m_SlabTension[n-1].m_dH = frM(m_Grid.GetTextMatrixDouble(2,n));
		m_pBridge->m_SlabTension[n-1].m_dR = frM(m_Grid.GetTextMatrixDouble(3,n));
		m_pBridge->m_SlabTension[n-1].m_nType = m_Grid.GetUserImage(4,n);
	}
	CPlateBasicIndex *pBx = m_pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	
	m_pBridge->SetTendonArcType(pBx);
}

void CSubInputTensionBarDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd		*pOpt = m_pStd->GetDataManage()->GetOptionStd();
	CAPlateDrawDanmyun	DrawDanmyun(m_pStd->GetDataManage());
	CAPlateDrawCross	DrawCross(m_pStd->GetDataManage());

	CDomyun *pDom = m_DomView.GetDomyun();	
	pDom->ClearEtt(TRUE);	
	
	CDomyun Dom(pDom);	
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	
	
	CPlateBasicIndex *pBx = m_pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	DrawDanmyun.DrawGuardWall(&Dom, pBx);	// 방호벽
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, TRUE, CAPlateDrawCross::NOTDRAWVSTIFF);	// 지점부 슬래브 단면
	DrawDanmyun.DrawTensionBar(&Dom, pBx, TRUE);	// 지점부 슬래브 단면
	
	///< 치수선그리기
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 75*(dDiagonalL/10000);
	Dom.SetScaleDim(dScale);
	
	DrawDanmyun.DimTensionBarOnLineInfo(&Dom, pBx, TRUE);	//직선
	DrawDanmyun.DimTensionBar(&Dom, pBx, TRUE);				//변곡선
	DrawDanmyun.DimTypicalSectionBottom(&Dom, pBx);
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
	
	*pDom << Dom;
	
	m_DomView.RedrawAll();
	if(bZoomAll) m_DomView.ZoomAll();
	else		 m_DomView.RedrawAll();
}

void CSubInputTensionBarDlg::OnOK() 
{
	SetDataSave();
	m_pBridge->SetCalcJongTendon();
	m_pBridge->m_bTendonInputComplete = TRUE;
	
	CXTResizeDialog::OnOK();
}

void CSubInputTensionBarDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CSubInputTensionBarDlg::OnCellChangedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
    int nCol = GridView->iColumn;

	m_DomView.GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

BOOL CSubInputTensionBarDlg::PreTranslateMessage(MSG* pMsg) 
{
	CWnd *pWnd = FromHandle(pMsg->hwnd);
	TCHAR szClass [256];
	::GetClassName (pMsg->hwnd, szClass, 255);
	CString strClass = szClass;
	if(pWnd && strClass=="Edit" && pWnd->GetFocus() != pWnd && pMsg->message == WM_LBUTTONDOWN)
	{	
		CXTResizeDialog::PreTranslateMessage(pMsg);
		((CEdit *)pWnd)->SetSel(0,-1);
		return TRUE;
	}	
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

void CSubInputTensionBarDlg::OnSelchangeComboSize() 
{
	UpdateData();
	m_pBridge->m_nSlabTensionSize = m_nSize;
	
	CPlateBasicIndex *pBx = m_pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();

	m_pBridge->SetTendonArcType(pBx);

	SetDataInit();
	DrawInputDomyunView(TRUE);	
}

void CSubInputTensionBarDlg::OnKillfocusEditBaselheight() 
{
	UpdateData();
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);		
}

void CSubInputTensionBarDlg::OnKillfocusEditBaserheight() 
{
	UpdateData();
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);	
}

void CSubInputTensionBarDlg::OnButtonApply() 
{
	UpdateData();
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);	
}

void CSubInputTensionBarDlg::OnButtonRecommend() 
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "강선배치 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
	return;

	SetDataDefault();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CSubInputTensionBarDlg::OnButtonDxfout() 
{
	m_DomView.GetDomyun()->DxfOut();	
}

void CSubInputTensionBarDlg::OnSetfocusEditBaselheight() 
{
	m_DomView.GetDomyun()->STMakeCursor("H1");	
}

void CSubInputTensionBarDlg::OnSetfocusEditBaserheight() 
{
	m_DomView.GetDomyun()->STMakeCursor("H2");	
}

void CSubInputTensionBarDlg::OnSelchangeComboFix() 
{
	UpdateData(TRUE);

	m_pBridge->m_nTendonFix = m_comboFix.GetCurSel();

	GetDlgItem(IDC_DOMYUNVIEW)->SetFocus();
	DrawInputDomyunView(FALSE);

}
void CSubInputTensionBarDlg::OnSelchangeComboFixDom()
{
	UpdateData(TRUE);
	
	m_pBridge->m_nTendonDomFix = m_comboModelFix.GetCurSel();
	
	GetDlgItem(IDC_DOMYUNVIEW)->SetFocus();
	DrawInputDomyunView(FALSE);
}