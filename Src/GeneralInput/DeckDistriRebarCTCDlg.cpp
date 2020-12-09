// DeckDistriRebarCTCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDistriRebarCTCDlg dialog


CDeckDistriRebarCTCDlg::CDeckDistriRebarCTCDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckDistriRebarCTCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDistriRebarCTCDlg)
	m_szDistriDiaRangeOnCant = _T("");
	m_szGirderCenRange = _T("");
	m_szDistriRebarDupliRange = _T("");
	//}}AFX_DATA_INIT
}


void CDeckDistriRebarCTCDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDistriRebarCTCDlg)	
	DDX_Control(pDX, IDC_COMBO_EXTRA, m_comboExtra);
	DDX_Control(pDX, IDC_TAB_POSITION, m_ctlTab);
	DDX_Text(pDX, IDC_EDIT_RANGE_CANTDIA, m_szDistriDiaRangeOnCant);
	DDX_Text(pDX, IDC_EDIT_RANGE_GIRDER, m_szGirderCenRange);
	DDX_Text(pDX, IDC_EDIT_DUPLI_RANGE, m_szDistriRebarDupliRange);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_UPPER, m_GridUp);
	DDX_GridControl(pDX, IDC_GRID_LOWER, m_GridDn);

	DDX_Control(pDX, ID_BUTTON_CTC_RECALC, m_btnReCalc);
	DDX_Control(pDX, IDC_BUTTON_SAMECTC_CURTAB, m_btnCurSameCTC);
	DDX_Control(pDX, IDC_BUTTON_SAMECTC_UPDN, m_btnUpDnSameCTC);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDelete);
}

BEGIN_MESSAGE_MAP(CDeckDistriRebarCTCDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckDistriRebarCTCDlg)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_POSITION, OnSelchangeTabPosition)
 	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_POSITION, OnSelchangingTabPosition)
	ON_CBN_SELCHANGE(IDC_COMBO_EXTRA, OnSelchangeComboExtra)
	ON_BN_CLICKED(ID_BUTTON_CTC_RECALC, OnButtonCtcRecalc)
	ON_BN_CLICKED(IDC_BUTTON_SAMECTC_CURTAB, OnButtonSameCTCByCurtab)
	ON_BN_CLICKED(IDC_BUTTON_SAMECTC_UPDN, OnButtonSameCTCByUpdn)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAddCTC)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDeleteCTC)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_UPPER, OnCellChangedDataUp)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_UPPER, OnCellChangedUp)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_LOWER, OnCellChangedDataDn)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_LOWER, OnCellChangedDn)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID_UPPER, OnCellInsertedColUp)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID_UPPER, OnCellDeletedColUp)	
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID_LOWER, OnCellInsertedColDn)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID_LOWER, OnCellDeletedColDn)	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDistriRebarCTCDlg message handlers
void CDeckDistriRebarCTCDlg::OnPreInitDialog()
{
	InitTabCtrl();
	
	SetResize(IDC_STATIC_GROUP, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC10, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_EXTRA, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_GROUP1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC11, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_RANGE_CANTDIA, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC14, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC12, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_RANGE_GIRDER, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC15, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC13, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_DUPLI_RANGE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC16, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(ID_BUTTON_CTC_RECALC, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_SAMECTC_CURTAB, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_SAMECTC_UPDN, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_ADD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DEL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_TAB_POSITION, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_UPPER, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID_LOWER, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
}

void CDeckDistriRebarCTCDlg::InitTabCtrl()
{
	CPlateBridgeApp *pDB	= m_pStd->m_pDataManage->GetBridge();

	m_ctlTab.DeleteAllItems();

	m_ctlTab.InsertItem(0,"단부");
	m_ctlTab.InsertItem(1,"일반부");
	if(pDB->m_nQtyJigan > 1)
		m_ctlTab.InsertItem(2,"지점부");

	m_ctlTab.SetCurSel(0);	
}


BOOL CDeckDistriRebarCTCDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_GridUp.GetFocusCell().col == m_GridUp.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
		
		if(m_GridDn.GetFocusCell().col == m_GridDn.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;

	}
	
	m_GridUp.ViewGridCellForCombo(pMsg);
	m_GridDn.ViewGridCellForCombo(pMsg);
	
	if(pMsg->hwnd == m_GridUp.GetSafeHwnd() && pMsg->message == WM_RBUTTONDOWN)
	{		
		CCellID cellID = m_GridUp.GetFocusCell();				
		CString szComp = m_GridUp.GetTextMatrix(0, cellID.col);
		if(szComp=="TOTAL" || szComp=="배근범위" || szComp=="오차" )		
			m_GridUp.EnableInsertCol(FALSE);					
		else		
			m_GridUp.EnableInsertCol(TRUE);					
	}	

	if(pMsg->hwnd == m_GridDn.GetSafeHwnd() && pMsg->message == WM_RBUTTONDOWN)
	{		
		CCellID cellID = m_GridDn.GetFocusCell();				
		CString szComp = m_GridDn.GetTextMatrix(0, cellID.col);
		if(szComp=="TOTAL" || szComp=="배근범위" || szComp=="오차" )		
			m_GridDn.EnableInsertCol(FALSE);					
		else		
			m_GridDn.EnableInsertCol(TRUE);					
	}	

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}



void CDeckDistriRebarCTCDlg::OnCellChangedUp(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	m_GridDn.ResetSelection();

	OnCursorChange(nRow, nCol);	
}


void CDeckDistriRebarCTCDlg::OnCellChangedDn(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;
	
	m_GridUp.ResetSelection();

	OnCursorChange(nRow, nCol);	
}



void CDeckDistriRebarCTCDlg::OnCellChangedDataUp(NMHDR*nmgv,LRESULT*)
{
	SetDataSave();

	DrawInputDomyunView(FALSE);
}



void CDeckDistriRebarCTCDlg::OnCellChangedDataDn(NMHDR*nmgv,LRESULT*)
{
	SetDataSave();
	
	DrawInputDomyunView(FALSE);
}




void CDeckDistriRebarCTCDlg::OnCellInsertedColUp(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nCol = pGridView->iColumn;

	InsertCTCByGridCol(nCol, TRUE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;

}


void CDeckDistriRebarCTCDlg::OnCellInsertedColDn(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nCol = pGridView->iColumn;

	InsertCTCByGridCol(nCol, FALSE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}



//삭제
void CDeckDistriRebarCTCDlg::OnCellDeletedColUp(NMHDR*nmgv,LRESULT*)
{			
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	 int nCol = pGridView->iColumn;

	DeleteCTCByGridCol(nCol, TRUE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}


//삭제
void CDeckDistriRebarCTCDlg::OnCellDeletedColDn(NMHDR*nmgv,LRESULT*)
{			
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nCol = pGridView->iColumn;

	DeleteCTCByGridCol(nCol, FALSE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}


void CDeckDistriRebarCTCDlg::InsertCTCByGridCol(long nGridCol, BOOL bUpper)
{
	long nSection = GetSectionIDByCurTab();

	CADeckData *pDeckData  = m_pStd->m_pDeckData;
	CDPointArray *pArrCTC = pDeckData->GetCTCBeryukRebar(nSection, bUpper);

	CDPoint xy(0, 0);

	long   nRebarsu = long(pArrCTC->GetAt(nGridCol-1).x);
	double dCTC     = pArrCTC->GetAt(nGridCol-1).y;

	pArrCTC->GetAt(nGridCol-1).x = long(nRebarsu/2);
	pArrCTC->GetAt(nGridCol-1).y = dCTC;
	pArrCTC->InsertAt(nGridCol-1, CDPoint(nRebarsu-long(nRebarsu/2), dCTC));
}



void CDeckDistriRebarCTCDlg::DeleteCTCByGridCol(long nGridCol, BOOL bUpper)
{
	long nSection = GetSectionIDByCurTab();

	CADeckData *pDeckData  = m_pStd->m_pDeckData;
	CDPointArray *pArrCTC = pDeckData->GetCTCBeryukRebar(nSection, bUpper);

	long nGugan = pArrCTC->GetSize();
	
	if(nGugan==1)	// 이음이 하나일때
	{
		pArrCTC->RemoveAt(nGridCol-1);
	}
	else if(nGugan>1 && nGridCol==nGugan)	// 마지막 이음위치
	{
		long   nRebarsu1 = long(pArrCTC->GetAt(nGridCol-1).x);
		double dCTC1     = pArrCTC->GetAt(nGridCol-1).y;
		long   nRebarsu2 = long(pArrCTC->GetAt(nGridCol-2).x);
		double dCTC2     = pArrCTC->GetAt(nGridCol-2).y;

		pArrCTC->RemoveAt(nGridCol-1);
		pArrCTC->SetAt(nGridCol-2, CDPoint((nRebarsu1*dCTC1)/dCTC2 + nRebarsu2, dCTC2));
	}
	else			// 이음이 둘이상일 경우
	{
		long   nRebarsu1 = long(pArrCTC->GetAt(nGridCol-1).x);
		double dCTC1     = pArrCTC->GetAt(nGridCol-1).y;
		long   nRebarsu2 = long(pArrCTC->GetAt(nGridCol).x);
		double dCTC2     = pArrCTC->GetAt(nGridCol).y;
		
		pArrCTC->RemoveAt(nGridCol-1);
		pArrCTC->SetAt(nGridCol-1, CDPoint((nRebarsu1*dCTC1)/dCTC2 + nRebarsu2, dCTC2));
	}

}



void CDeckDistriRebarCTCDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CAPlateDrawDanmyun  DBDanmyun(m_pStd->m_pDataManage);
	CDrawDBFrontRebar   DBFrontRebar(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);
	CADeckData			*pDeckData  = m_pStd->m_pDeckData;

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	CDomyun Dom(pDom);

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetScaleDim(50);
	
	long   nSection  = GetSectionIDByCurTab();
	CPlateBasicIndex *pBx = pDeckData->GetBxBySection(nSection);
	BOOL bShowMark   = pDeckData->m_bShowMark && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag() ? TRUE : FALSE;
	long nCycle = 0;

	DBDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, FALSE);

	DBFrontRebar.DrawMainRebar(&Dom, nSection, nCycle, TRUE, TRUE);
	DBFrontRebar.DrawMainRebarShear(&Dom, nSection);
	DBFrontRebar.DrawBeryukRebar(&Dom, nSection);

	DBFrontRebar.DimMainRebarUp(&Dom, nSection, m_dScaleDim/100*1.5, m_nTab==1 ? TRUE : FALSE, TRUE);
	DBFrontRebar.DimMainRebarLo(&Dom, nSection, TRUE, m_dScaleDim/100*1.5);
	DBFrontRebar.DimMainRebarLeft(&Dom, nSection, TRUE, m_dScaleDim/100*1.5);
	DBFrontRebar.DimMainRebarRigh(&Dom, nSection, TRUE, m_dScaleDim/100*1.5);
	
	if(bShowMark)
	{
		DBFrontRebar.DrawMarkMainRebar(&Dom, nSection, nCycle);
		DBFrontRebar.DrawMarkBeryukRebar(&Dom, nSection);
		DBFrontRebar.DrawMarkShearRebar(&Dom, nSection);
	}
	
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	DrawStd.AddTitleOnDom(&Dom, 80, rect, GetCurTabName(), TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);

	*pDom << Dom;
	
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}


void CDeckDistriRebarCTCDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	structGridCell *pCellUp	= m_GridUp.GetCurrentCell();
	BOOL bFocusUp = pCellUp!=NULL ? TRUE : FALSE;
	
	CGridCtrl *pGrid = bFocusUp ? &m_GridUp : &m_GridDn;
	
	CString sText1 = pGrid->GetTextMatrix(1, nCol);		
	CString sText2 = pGrid->GetTextMatrix(2, nCol);		
	CString sText3 = sText1+"@"+sText2;
	
	m_pView->GetDomyun()->STMakeCursor(sText3);
}


void CDeckDistriRebarCTCDlg::SetDataSave() 
{
	CADeckData *pDeckData  = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	
	m_GridUp.UpdateData(TRUE);
	m_GridDn.UpdateData(TRUE);

	pDeckData->m_nOptBeryukRebarArrange = m_comboExtra.GetCurSel();
	m_comboExtra.SetCurSel(pDeckData->m_nOptBeryukRebarArrange);
	pDeckData->m_dRangeDistribRebarDup = atof(m_szDistriRebarDupliRange);
	pDeckData->m_dRangeDistribRebarOnCant = atof(m_szDistriDiaRangeOnCant);
	pDeckData->m_dRangeDistribRebarOnGirder = atof(m_szGirderCenRange);					// 거더중앙부 배력철근 범위

	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetBxOnJijum(0);
	CDPoint CLeft[4], CRigh[4];
	CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(0));
	CPlateBasicIndex *pBxRigh = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(pDB->GetGirdersu()-2));

	pDB->GetXyFrontCantilever(pBxLeft, CLeft, TRUE);
	pDB->GetXyFrontCantilever(pBxRigh, CRigh, FALSE);

	pDeckData->m_dWidthSlabCantLeftForBeryuk = ~(CLeft[0]-CLeft[3])+pDeckData->m_dRangeDistribRebarOnCant;
	pDeckData->m_dWidthSlabCantRighForBeryuk = ~(CRigh[0]-CRigh[3])+pDeckData->m_dRangeDistribRebarOnCant;

	UpdateData(TRUE);
}



void CDeckDistriRebarCTCDlg::SetGridDataUp() 
{
	CPlateBridgeApp *pDB		  = m_pStd->m_pDataManage->GetBridge();	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nSection = GetSectionIDByCurTab();
	
	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	long nLine = 0;	

	CDPointArray* pArrCTC = pDeckData->GetCTCBeryukRebar(nSection, TRUE);
	long nGuganSu = pArrCTC->GetSize();
	if (nGuganSu==0)	return;

	m_GridUp.SetGridDefault(3, nGuganSu+4, 1, 1);
	// 제목
	long nCol = 0;
	for (nCol=0; nCol<nGuganSu+4; nCol++)
	{
		if(nCol==0)						m_GridUp.AddCell(0, nCol, "구간");
		else if(nCol==nGuganSu+1)		m_GridUp.AddCell(0, nCol, "TOTAL");
		else if(nCol==nGuganSu+2)		m_GridUp.AddCell(0, nCol, "배근범위");
		else if(nCol==nGuganSu+3)		m_GridUp.AddCell(0, nCol, "오차");
		else							m_GridUp.AddCell(0, nCol, Comma(nCol));
	}
	// 개수
	double dTotalL = 0;
	double dTotalN = 0;
	for (nCol=0; nCol<nGuganSu+1; nCol++)
	{
		if(nCol==0)	
		{
			m_GridUp.AddCell(1, nCol, "개수");
			m_GridUp.AddCell(2, nCol, "간격");
		}
		else
		{
			CDPoint xyCTC = pArrCTC->GetAt(nCol-1);
			m_GridUp.AddCell(1, nCol, pArrCTC, nCol-1, TRUE, nFormat, nLine);	m_GridUp.SetCellDec(CELL_DEC_AUTO);
			m_GridUp.AddCell(2, nCol, pArrCTC, nCol-1, FALSE, nFormat, nLine);	m_GridUp.SetCellDec(CELL_DEC_AUTO);
			dTotalL += xyCTC.x * xyCTC.y;
			dTotalN += xyCTC.x;
		}
	}
	double dSta = pDeckData->GetStationBySection(nSection);
	double dWidthSlab = pDB->GetWidthAct(dSta);

	// 합계 (TOTAL, 배근범위, 오차)
	for (nCol=nGuganSu+1; nCol<nGuganSu+4; nCol++)
	{
		if(nCol==nGuganSu+1)
		{
			sz.Format("%.0lf", dTotalN);
			m_GridUp.AddCell(1, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
			sz.Format("%.2lf", dTotalL);
			m_GridUp.AddCell(2, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
		}
		else if(nCol==nGuganSu+2)
		{
			m_GridUp.AddCell(1, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
			sz.Format("%.2lf", dWidthSlab-pDeckData->m_dCoverSd*2);
			m_GridUp.AddCell(2, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
		}
		else if(nCol==nGuganSu+3)
		{
			m_GridUp.AddCell(1, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
			sz.Format("%.2lf", dTotalL-(dWidthSlab-pDeckData->m_dCoverSd*2));
			m_GridUp.AddCell(2, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
		}
	}

}


void CDeckDistriRebarCTCDlg::SetGridDataDn() 
{
	CPlateBridgeApp *pDB		  = m_pStd->m_pDataManage->GetBridge();	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nSection = GetSectionIDByCurTab();
	
	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	long nLine = 0;	

	CDPointArray* pArrCTC = pDeckData->GetCTCBeryukRebar(nSection, FALSE);
	long nGuganSu = pArrCTC->GetSize();
	if (nGuganSu==0)	return;

	m_GridDn.SetGridDefault(3, nGuganSu+4, 1, 1);
	
	// 제목
	long nCol = 0;
	for (nCol=0; nCol<nGuganSu+4; nCol++)
	{
		if(nCol==0)						m_GridDn.AddCell(0, nCol, "구간");
		else if(nCol==nGuganSu+1)		m_GridDn.AddCell(0, nCol, "TOTAL");
		else if(nCol==nGuganSu+2)		m_GridDn.AddCell(0, nCol, "배근범위");
		else if(nCol==nGuganSu+3)		m_GridDn.AddCell(0, nCol, "오차");
		else							m_GridDn.AddCell(0, nCol, Comma(nCol));
	}
	// 개수
	double dTotalL = 0;
	double dTotalN = 0;
	for (nCol=0; nCol<nGuganSu+1; nCol++)
	{
		if(nCol==0)	
		{
			m_GridDn.AddCell(1, nCol, "개수");
			m_GridDn.AddCell(2, nCol, "간격");
		}
		else
		{
			m_GridDn.AddCell(1, nCol, pArrCTC, nCol-1, TRUE, nFormat, nLine);	m_GridDn.SetCellDec(CELL_DEC_AUTO);
			m_GridDn.AddCell(2, nCol, pArrCTC, nCol-1, FALSE, nFormat, nLine);	m_GridDn.SetCellDec(CELL_DEC_AUTO);
			CDPoint xyCTC = pArrCTC->GetAt(nCol-1);
			dTotalL += xyCTC.x * xyCTC.y;
			dTotalN += xyCTC.x;
		}
	}
	double dSta = pDeckData->GetStationBySection(nSection);
	double dWidthSlab = pDB->GetWidthAct(dSta);

	// 합계 (TOTAL, 배근범위, 오차)
	for (nCol=nGuganSu+1; nCol<nGuganSu+4; nCol++)
	{
		if(nCol==nGuganSu+1)
		{
			sz.Format("%.0lf", dTotalN);
			m_GridDn.AddCell(1, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
			sz.Format("%.2lf", dTotalL);
			m_GridDn.AddCell(2, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
		}
		else if(nCol==nGuganSu+2)
		{
			m_GridDn.AddCell(1, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
			sz.Format("%.2lf", dWidthSlab-pDeckData->m_dCoverSd*2);
			m_GridDn.AddCell(2, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
		}
		else if(nCol==nGuganSu+3)
		{
			m_GridDn.AddCell(1, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
			sz.Format("%.2lf", dTotalL-(dWidthSlab-pDeckData->m_dCoverSd*2));
			m_GridDn.AddCell(2, nCol, sz, DT_CENTER|DT_VCENTER, CELL_READONLY);
		}
	}
}



void CDeckDistriRebarCTCDlg::SetDataDefault() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CGlobarOption *pGlopt = m_pStd->GetDataManage()->GetGlobalOption();

	if(!IsUsedFlag())
	{	
		pDeckData->m_dRangeDistribRebarOnCant   = 1000;	// 캔틸레버부 배력근 범위
		pDeckData->m_dRangeDistribRebarOnGirder = 1000;	// 거더부 배력근 범위
		pDeckData->m_dRangeDistribRebarDup		= 200;	// 배력은이 겹치는 범위	
		pDeckData->m_nOptBeryukRebarArrange	    = CADeckData::SP_BOTH;				// 짜투리 치수범위

		pDeckData->MakeDimArrAtDanbu();
		
		if(pGlopt->GetSlabCentDesignMethod()==1)	// 경험적
			pDeckData->MakeDimArrAtCenterExperience();
		else
			pDeckData->MakeDimArrAtCenterStrength();

		if(m_pStd->m_pDataManage->GetBridge()->m_nQtyJigan > 1)
			pDeckData->MakeDimArrAtJijum();
	}
	else
	{
		if(GetCurTabName()=="단부")		
			pDeckData->MakeDimArrAtDanbu();
		else if(GetCurTabName()=="일반부")	
		{
			if(pGlopt->GetSlabCentDesignMethod()==1)	// 경험적
				pDeckData->MakeDimArrAtCenterExperience();
			else
				pDeckData->MakeDimArrAtCenterStrength();
		}
		else
			pDeckData->MakeDimArrAtJijum();
	}
}




void CDeckDistriRebarCTCDlg::SetDataInit() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nSection = GetSectionIDByCurTab();
	CDPointArray* pArrCTCUp = pDeckData->GetCTCBeryukRebar(nSection, TRUE);
	CDPointArray* pArrCTCDn = pDeckData->GetCTCBeryukRebar(nSection, FALSE);

	if(pArrCTCUp && pArrCTCUp->GetSize()>0)
	{
		SetGridDataUp();
		
		m_GridUp.UpdateData(FALSE);
		m_GridUp.SetColumnWidthAll(50);		
		m_GridUp.SetColumnWidth(0, 100);
		m_GridUp.SetColumnWidth(m_GridUp.GetColumnCount()-1, 50);
		m_GridUp.SetColumnWidth(m_GridUp.GetColumnCount()-2, 70);
		m_GridUp.SetColumnWidth(m_GridUp.GetColumnCount()-3, 70);
		m_GridUp.EnableInsertCol();
		m_GridUp.EnableInsertRow(FALSE);
		m_GridUp.SetRedraw(TRUE, TRUE);
		m_GridUp.SetFocusCell(1, 1);
	}

	if (pArrCTCDn && pArrCTCDn->GetSize())
	{
		SetGridDataDn();
		
		m_GridDn.UpdateData(FALSE);
		m_GridDn.SetColumnWidthAll(50);		
		m_GridDn.SetColumnWidth(0, 100);
		m_GridDn.SetColumnWidth(m_GridDn.GetColumnCount()-1, 50);
		m_GridDn.SetColumnWidth(m_GridDn.GetColumnCount()-2, 70);
		m_GridDn.SetColumnWidth(m_GridDn.GetColumnCount()-3, 70);
		m_GridDn.EnableInsertCol();
		m_GridDn.EnableInsertRow(FALSE);
		m_GridDn.SetRedraw(TRUE, TRUE);
	}

	
	m_comboExtra.SetCurSel(pDeckData->m_nOptBeryukRebarArrange);
	m_szDistriRebarDupliRange.Format("%.0lf", pDeckData->m_dRangeDistribRebarDup);	// 배력철근 겹침범위
	m_szDistriDiaRangeOnCant.Format("%.0lf", pDeckData->m_dRangeDistribRebarOnCant);// 캔틸레버부 배력철근 직경 적용범위
	m_szGirderCenRange.Format("%.0lf", pDeckData->m_dRangeDistribRebarOnGirder);	// 거더중앙부 배력철근 범위

	if(GetCurTabName()=="일반부")
	{
		GetDlgItem(IDC_STATIC12)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC13)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_DUPLI_RANGE)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_RANGE_GIRDER)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC15)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC16)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC12)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC13)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_DUPLI_RANGE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_RANGE_GIRDER)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC15)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC16)->ShowWindow(FALSE);
	}

	UpdateData(FALSE);
}



void CDeckDistriRebarCTCDlg::OnSelchangeTabPosition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nTab = m_ctlTab.GetCurSel();

	UpdateData();	
	SetDataInit();	
	DrawInputDomyunView(TRUE);
}

void CDeckDistriRebarCTCDlg::OnSelchangingTabPosition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();	
}

BOOL CDeckDistriRebarCTCDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}


CString CDeckDistriRebarCTCDlg::GetCurTabName()
{
	CString sText;
	TCITEM item ;
	char cTableName[256];
	item.mask = TCIF_TEXT;
	item.pszText = cTableName;
	item.cchTextMax = 255;

	m_ctlTab.GetItem(m_ctlTab.GetCurSel(), &item);
	sText = item.pszText;

	return sText;
}



long CDeckDistriRebarCTCDlg::GetSectionIDByCurTab()
{
	// 단부 : 일반교량 - 슬래브 시점부
	//        확푝교량 - 기준이 되는 단면(시점부와 종점부중 교폭이 작은 위치)
	if (GetCurTabName()=="단부")	
	{
		return SECTION_DAN;
	}
	else if (GetCurTabName()=="일반부")	
	{
		return SECTION_CEN;
	}
	else if (GetCurTabName()=="지점부")	
		return SECTION_PIER;

	return -1;
}


void CDeckDistriRebarCTCDlg::OnSelchangeComboExtra() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->m_nOptBeryukRebarArrange = m_comboExtra.GetCurSel();
	
	DrawInputDomyunView(FALSE);
	
}

void CDeckDistriRebarCTCDlg::OnButtonCtcRecalc() 
{
	UpdateData(TRUE);
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CGlobarOption *pGlopt = m_pStd->GetDataManage()->GetGlobalOption();

	pDeckData->m_dRangeDistribRebarDup = atof(m_szDistriRebarDupliRange);
	pDeckData->m_dRangeDistribRebarOnCant = atof(m_szDistriDiaRangeOnCant);
	pDeckData->m_dRangeDistribRebarOnGirder = atof(m_szGirderCenRange);		
	
	if(GetCurTabName()=="단부")		
		pDeckData->MakeDimArrAtDanbu();
	else if(GetCurTabName()=="일반부")	
	{
		if(pGlopt->GetSlabCentDesignMethod()==1)	// 경험적
			pDeckData->MakeDimArrAtCenterExperience();
		else
			pDeckData->MakeDimArrAtCenterStrength();
	}
	else
	{
		if(m_pStd->m_pDataManage->GetBridge()->m_nQtyJigan > 1)
			pDeckData->MakeDimArrAtJijum();
	}

	SetDataInit();

	DrawInputDomyunView(FALSE);
}

void CDeckDistriRebarCTCDlg::OnButtonSameCTCByCurtab() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nSectionCur = GetSectionIDByCurTab();
	CString sTabCur = GetCurTabName();
	
	if( MessageBox(sTabCur+"단면의 배력철근 간격을 기준으로 \n모든 단면의 배력철근 간격을 동일적용하시겠습니까?","배력철근 간격 동일적용",
		MB_YESNO|MB_ICONQUESTION) == IDNO ) return;

	CDPointArray* pArrCTCUpCur = pDeckData->GetCTCBeryukRebar(nSectionCur, TRUE);
	CDPointArray* pArrCTCDnCur = pDeckData->GetCTCBeryukRebar(nSectionCur, FALSE);
	for (long nTab=0; nTab<m_ctlTab.GetItemCount(); nTab++)
	{
		CString sTab = GetTabName(nTab);
		long nSection = GetSectionIDByTabName(sTab);
		if (nSection==nSectionCur)		continue;

		CDPointArray* pArrCTCUp = pDeckData->GetCTCBeryukRebar(nSection, TRUE);
		CDPointArray* pArrCTCDn = pDeckData->GetCTCBeryukRebar(nSection, FALSE);

		*pArrCTCUp = *pArrCTCUpCur;
		*pArrCTCDn = *pArrCTCDnCur;
	}

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);	
}



void CDeckDistriRebarCTCDlg::OnButtonSameCTCByUpdn() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	structGridCell *pCellUp	= m_GridUp.GetCurrentCell();
	BOOL bFocusUp = pCellUp!=NULL ? TRUE : FALSE;

	long nSectionCur = GetSectionIDByCurTab();
	CString sTabCur  = GetCurTabName();
	CString sSource  = bFocusUp ? "상면" : "하면";
	CString sTaget   = bFocusUp ? "하면" : "상면";
	
	if( MessageBox(sTabCur+"단면의"+sSource+"배력철근 간격을 기준으로 \n"+sTaget+ "배력철근 간격을 동일적용하시겠습니까?","배력철근 간격 동일적용",
		MB_YESNO|MB_ICONQUESTION) == IDNO ) return;

	CDPointArray* pArrCTCUpCur = pDeckData->GetCTCBeryukRebar(nSectionCur, TRUE);
	CDPointArray* pArrCTCDnCur = pDeckData->GetCTCBeryukRebar(nSectionCur, FALSE);

	if (bFocusUp)		*pArrCTCDnCur = *pArrCTCUpCur;
	else				*pArrCTCUpCur = *pArrCTCDnCur;
		

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);		
}



void CDeckDistriRebarCTCDlg::OnButtonAddCTC() 
{
	structGridCell *pCellUp	= m_GridUp.GetCurrentCell();
	BOOL bFocusUp = pCellUp!=NULL ? TRUE : FALSE;

	long nCol = bFocusUp ? m_GridUp.GetFocusCell().col : m_GridDn.GetFocusCell().col;

	if(nCol<1)
		nCol = bFocusUp ? m_GridUp.GetColumnCount()-4 : m_GridDn.GetColumnCount()-4;
	
	InsertCTCByGridCol(nCol, bFocusUp);
	

	SetDataInit();
	SetDataSave();

	if(bFocusUp)
		m_GridUp.SetFocusCell(1, m_GridUp.GetColumnCount()-4);
	else
		m_GridDn.SetFocusCell(1, m_GridDn.GetColumnCount()-4);	
}



void CDeckDistriRebarCTCDlg::OnButtonDeleteCTC() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	structGridCell *pCellUp	= m_GridUp.GetCurrentCell();
	BOOL bFocusUp = pCellUp!=NULL ? TRUE : FALSE;

	long nCol = bFocusUp ? m_GridUp.GetFocusCell().col : m_GridDn.GetFocusCell().col;

	if(nCol<1)
		nCol = bFocusUp ? m_GridUp.GetColumnCount()-4 : m_GridDn.GetColumnCount()-4;

	long nSection = GetSectionIDByCurTab();
	CDPointArray* pArrCTC = pDeckData->GetCTCBeryukRebar(nSection, bFocusUp);
	if(pArrCTC->GetSize()<=1)	return;

	DeleteCTCByGridCol(nCol, bFocusUp);

	SetDataInit();
	SetDataSave();

	if(bFocusUp)
		m_GridUp.SetFocusCell(1, m_GridUp.GetColumnCount()-4);
	else
		m_GridDn.SetFocusCell(1, m_GridDn.GetColumnCount()-4);	
}



long CDeckDistriRebarCTCDlg::GetSectionIDByTabName(CString sTab)
{
	// 단부 : 일반교량 - 슬래브 시점부
	if (sTab=="단부")	
	{
		return SECTION_DAN;
	}
	else if (sTab=="일반부")	
	{
		return SECTION_CEN;
	}
	else if (sTab=="지점부")	return SECTION_PIER;

	return -1;

}



CString CDeckDistriRebarCTCDlg::GetTabName(long nTab)
{
	CString sText;
	TCITEM item ;
	char cTableName[256];
	item.mask = TCIF_TEXT;
	item.pszText = cTableName;
	item.cchTextMax = 255;

	m_ctlTab.GetItem(nTab, &item);
	sText = item.pszText;

	return sText;
	
}

void CDeckDistriRebarCTCDlg::OnButtonApply() 
{
	UpdateData();

	SetDataSave();
	SetDataInit();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	if(pDeckData->m_bRecalcJewon && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag())
		pDeckData->MakeRebarJewon();

	DrawInputDomyunView();		
}
