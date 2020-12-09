// DeckMainRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "GeneralBaseStd.h"
#include "../APlateDBDraw/APlateDBDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CDeckMainRebarDlg dialog

CDeckMainRebarDlg::CDeckMainRebarDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckMainRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckMainRebarDlg)
	m_bCheckRailType = FALSE;
	m_bShowBox = FALSE;
	m_szMainShape = _T("");
	m_szMainCenterShape = _T("");
	m_szSettleOption = _T("");
	m_szMainCycle = _T("");
	m_szDivideRebar = _T("");
	//}}AFX_DATA_INIT
}


void CDeckMainRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckMainRebarDlg)
	DDX_Control(pDX, IDC_COMBO_VIEW_CYCLE, m_comboViewCycle);
	DDX_Check(pDX, IDC_CHECK_MAIN_LOWER_LEVEL, m_bCheckRailType);
	DDX_Check(pDX, IDC_CHECK_VIEW_BOX, m_bShowBox);
	DDX_Text(pDX, IDC_STATIC_MAIN_SHAPE, m_szMainShape);
	DDX_Text(pDX, IDC_STATIC_MAINCEN_SHAPE, m_szMainCenterShape);
	DDX_Text(pDX, IDC_STATIC_SETTLE_OPTION, m_szSettleOption);
	DDX_Text(pDX, IDC_STATIC_MAIN_CYCLE, m_szMainCycle);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Text(pDX, IDC_STATIC_DIVIDEREBAR, m_szDivideRebar);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_BUTTON_MAIN_SHAPE, m_btnMainBar);
	DDX_Control(pDX, IDC_BUTTON_MAIN_CYCLE, m_btnMainCycle);
	DDX_Control(pDX, IDC_BUTTON_MAIN_CENTER_SHAPE, m_btnMainCenter);
	DDX_Control(pDX, IDC_BUTTON_SETTLE_OPTION, m_btnSettleOption);
	DDX_Control(pDX, IDC_BUTTON_DIVIDEREBAR, m_btnDivideRebar);
	DDX_Control(pDX, IDC_BUTTON_MIRROR_JOINT, m_btnReversePos);
}

BEGIN_MESSAGE_MAP(CDeckMainRebarDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckMainRebarDlg)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)	
 	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(IDC_BUTTON_MIRROR_JOINT, OnButtonMirrorJoint)
	ON_BN_CLICKED(IDC_CHECK_VIEW_BOX, OnCheckViewBox)
	ON_BN_CLICKED(IDC_CHECK_MAIN_LOWER_LEVEL, OnCheckMainLowerLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW_CYCLE, OnSelchangeComboViewCycle)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_SHAPE, OnButtonMainShape)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CENTER_SHAPE, OnButtonMainCenterShape)
	ON_BN_CLICKED(IDC_BUTTON_SETTLE_OPTION, OnButtonSettleOption)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CYCLE, OnButtonMainCycle)
	ON_BN_CLICKED(IDC_BUTTON_DIVIDEREBAR, OnButtonDivideRebar)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)	
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckMainRebarDlg message handlers
void CDeckMainRebarDlg::OnPreInitDialog()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	InitTabCtrl();
	InitComboCycle();
	
	m_DetailMainUpperDlg.m_pDeckData = pDeckData;
	m_Grid.InitGrid(toKgPCm2(pDeckData->m_SigmaY));

	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MAIN_SHAPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_MAIN_CYCLE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_MAIN_CENTER_SHAPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_SETTLE_OPTION, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_DIVIDEREBAR, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_VIEW_BOX, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_MAIN_LOWER_LEVEL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MAIN_SHAPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_MAIN_CYCLE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_MAINCEN_SHAPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SETTLE_OPTION, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_DIVIDEREBAR, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_VIEW_CYCLE, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_MIRROR_JOINT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}



void CDeckMainRebarDlg::InitTabCtrl()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_ctlTab.DeleteAllItems();

	long nTab = 0;

	if(pDeckData->IsSameCycleDanGen())		// 단부, 일반부가 동일할 경우
	{
		m_ctlTab.InsertItem(nTab++,"단부/일반부");
	}
	else
	{
		m_ctlTab.InsertItem(nTab++,"단부");
		m_ctlTab.InsertItem(nTab++,"일반부");
	}
	if(pDB->m_nQtyJigan>1)
		m_ctlTab.InsertItem(nTab++,"지점부");

	if(pDeckData->IsSkewStt())
		m_ctlTab.InsertItem(nTab++,"사보강부(시점)");
	if(pDeckData->IsSkewEnd())
		m_ctlTab.InsertItem(nTab++,"사보강부(종점)");

	m_ctlTab.SetCurSel(0);	
}



void CDeckMainRebarDlg::InitComboCycle()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	m_comboViewCycle.ResetContent();

	long nSection = GetSectionIDByCurTab();
	long nCyclesu = pDeckData->GetRebarCycleSuBySection(nSection);

	for (long n=0; n<nCyclesu; n++)
	{
		m_comboViewCycle.AddString(COMMA(n+1));
	}
	m_comboViewCycle.AddString("ALL");
	m_comboViewCycle.SetCurSel(m_comboViewCycle.GetCount()-1);
}



// 횡단면은 주철근의 배근방향과 일치되게 보여줌
void CDeckMainRebarDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDrawDBFrontRebar   DBFrontRebar(m_pStd->m_pDataManage);
	CAPlateDrawDanmyun  DrawDanmyun(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	BOOL bShowMark = pDeckData->m_bShowMark && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag() ? TRUE : FALSE;

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	CDomyun Dom(pDom);
	CString sz("");

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetScaleDim(50);
	
	long nSection    = GetSectionIDByCurTab();
	long nCycleTotal = pDeckData->GetRebarCycleSuBySection(nSection);
	long nCycleStt   = m_comboViewCycle.GetCurSel();
	long nCycleEnd   = m_comboViewCycle.GetCurSel()+1;
	long nShape      = -1;

	if(GetCurTabName()=="일반부" || GetCurTabName()=="지점부")
	{
		long nCurCol = m_Grid.GetFocusCell().col;
		if(m_Grid.GetTextMatrix(1, nCurCol) == "하면")
		{
			nShape = atol(m_Grid.GetTextMatrix(2, nCurCol))-1;
		}
	}
	
	if (m_comboViewCycle.GetCurSel()<0 || m_comboViewCycle.GetCurSel()==m_comboViewCycle.GetCount()-1)
	{
		nCycleStt = 0;
		nCycleEnd = nCycleTotal;
	}

	if(pDeckData->IsCycleValid(nSection)==FALSE)	return;

	CPlateBasicIndex *pBx = pDeckData->GetBxBySection(nSection);

	double dMoveY = 0;
	for(long nCycle=nCycleStt; nCycle < nCycleEnd; nCycle++)
	{
		// 일반
		DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, FALSE, 2, m_bShowBox);

		// 배근
		DBFrontRebar.DrawMainRebar(&Dom, nSection, nCycle, TRUE, TRUE, nShape);
		DBFrontRebar.DimMainRebarJolib(&Dom, nSection, nCycle, TRUE, m_dScaleDim/100*1.5);
		if(bShowMark)	// 철근마크 보이기
			DBFrontRebar.DrawMarkMainRebar(&Dom, nSection, nCycle);

		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		CString sz;
		sz.Format("%d - CYCLE", nCycle+1);
		DrawStd.AddTitleOnDom(&Dom, 50, rect, sz, TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
		dMoveY -= rect.Height()*1.7;
		Dom.Move(0, dMoveY);

		*pDom << Dom;
	}
	
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}


void CDeckMainRebarDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetFocus();	
	SetDataInit();

	UpdateData();

	m_Grid.SetFocusCell(1, 1);

	GetDlgItem(IDC_STATIC6)->GetActiveWindow();
}

void CDeckMainRebarDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetDataSave();
}


void CDeckMainRebarDlg::SetGridData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;	

	long nSection = GetSectionIDByCurTab();
	long nCyclesu = pDeckData->GetRebarCycleSuBySection(nSection);

	m_Grid.SetGridDefault(6, 5, 3, 1);

	m_Grid.AddCell(0, 0, "Cycle");
	m_Grid.AddCell(1, 0, "상/하");
	m_Grid.AddCell(2, 0, "구간");
	m_Grid.AddCell(3, 0, "길이");
	m_Grid.AddCell(4, 0, "직경");
	m_Grid.AddCell(5, 0, "유/무");

	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	long nLine = 0;	
	long nCol  = 1;

	for (long nCycle=0; nCycle<nCyclesu; nCycle++)
	{
		REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);

		long nJointSizeUp = pRebarCycle->m_dArrJointUp.GetSize();
		long nJointSizeDn = pRebarCycle->m_dArrJointDn.GetSize();
		long nJoint		  = 0;
		for(nJoint=0; nJoint<=nJointSizeUp; nJoint++)
		{
			m_Grid.AddCell(0, nCol, COMMA(nCycle+1)+"Cycle");
			m_Grid.AddCell(1, nCol, "상면");
			m_Grid.AddCell(2, nCol, COMMA(nJoint+1));

			if(nJoint==nJointSizeUp)
			{
				m_Grid.AddCell(3, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);	m_Grid.SetCellDec(CELL_DEC_0); // 길이	
			}
			else
			{
				if(pRebarCycle->m_dArrJointUp[nJoint]==0)
					m_Grid.AddCell(3, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);	// 길이
				else
				{
					m_Grid.AddCell(3, nCol, &pRebarCycle->m_dArrJointUp, nJoint, TRUE);	
					m_Grid.SetCellDec(CELL_DEC_0);	// 길이
				}
			}
		
			m_Grid.AddCell(4, nCol, &pRebarCycle->m_dArrDiaUp, nJoint, nFormat, nLine, "", CELL_TYPE_DIA);			// 직경
			m_Grid.AddCell(5, nCol, &pRebarCycle->m_dArrValidUp, nJoint, nFormat, nLine, "", CELL_TYPE_CHECK);		// 유/무
			nCol++;
		}
		
		for(nJoint=0; nJoint<=nJointSizeDn; nJoint++)
		{
			m_Grid.AddCell(0, nCol, COMMA(nCycle+1)+"Cycle");
			m_Grid.AddCell(1, nCol, "하면");
			m_Grid.AddCell(2, nCol, COMMA(nJoint+1));

			if(nJoint==nJointSizeDn)
			{
				m_Grid.AddCell(3, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);	m_Grid.SetCellDec(CELL_DEC_0);// 길이
			}
			else
			{
				if(pRebarCycle->m_dArrJointDn[nJoint]==0)
					m_Grid.AddCell(3, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
				else
				{
					m_Grid.AddCell(3, nCol, &pRebarCycle->m_dArrJointDn, nJoint, TRUE);	
					m_Grid.SetCellDec(CELL_DEC_0);	// 길이
				}
			}

			m_Grid.AddCell(4, nCol, &pRebarCycle->m_dArrDiaDn, nJoint, nFormat, nLine, "", CELL_TYPE_DIA);				// 직경
			m_Grid.AddCell(5, nCol, &pRebarCycle->m_dArrValidDn, nJoint, nFormat, nLine, "", CELL_TYPE_CHECK);		// 유/무
			nCol++;
		}
	}
}



void CDeckMainRebarDlg::SetDataDefault() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;	
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	// Cycle 및 형태생성
	SetDataDefaultOpt();

	if(IsUsedFlag()==FALSE)
	{
		long nSectionEnd = pDB->m_nQtyJigan==1 ? SECTION_SKEW_END : SECTION_PIER;
		for (long nSection=0; nSection<=nSectionEnd; nSection++)
		{
			pDeckData->MakeCycleMainRebar(nSection);
			pDeckData->MakeShapeMainRebar(nSection);
		}
	}
	else
	{
		long nSection = GetSectionIDByCurTab();
		pDeckData->MakeCycleMainRebar(nSection);
		pDeckData->MakeShapeMainRebar(nSection);
	}
	
	InitComboCycle();

	m_pStd->m_pDoc->SetModifiedFlag();
}



void CDeckMainRebarDlg::SetDataDefaultOpt() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	if(IsUsedFlag()==FALSE)
	{
		pDeckData->m_nDetailMainUpper = 0;
		pDeckData->m_nDetailMainLowerCenter = 1;
		pDeckData->m_dRangeMainRebarFromFlangeStt = 600;	
		pDeckData->m_nDetailSettlement = 0;	
	}

	//////////////////////////////////////////////////////////////////////////////////
	CPlateGirderApp	 *pGirL	= pDB->GetGirder(0);
	CPlateGirderApp	 *pGirR	= pDB->GetGirder(pDB->GetQtyGirder()-1);
	CPlateBxFinder FinderL(pGirL);
	CPlateBxFinder FinderR(pGirR);

	CPlateBasicIndex *pBxL	= FinderL.GetBxFirst(BX_STT_SLAB);
	CPlateBasicIndex *pBxR	= FinderR.GetBxFirst(BX_STT_SLAB);
	double dHTaperL = pGirL->GetHeightTaper(pBxL->GetStation());
	double dHTaperR = pGirR->GetHeightTaper(pBxR->GetStation());

	double dLenCenterSideL = pDB->m_dThickSlabLeft - pDeckData->m_dCoverUp - pDeckData->m_dCoverDn;
	double dLenDanbuSideL  = pDB->m_dThickSlabLeft - pDeckData->m_dCoverUp - pDeckData->m_dCoverDn + dHTaperL/*pDB->m_dHeightTaperStt*/;
	double dLenCenterSideR = pDB->m_dThickSlabRight - pDeckData->m_dCoverUp - pDeckData->m_dCoverDn;
	double dLenDanbuSideR  = pDB->m_dThickSlabRight - pDeckData->m_dCoverUp - pDeckData->m_dCoverDn + dHTaperR/*pDB->m_dHeightTaperStt*/;

	if(pDeckData->m_nDetailMainUpper==0)	// 상부직선타입
	{
		pDeckData->m_dLengthMainDolchulUp[0]    = 0;
		pDeckData->m_dLengthMainDolchulUpDan[0] = 0;
		pDeckData->m_dLengthMainDolchulUp[1]    = 0;
		pDeckData->m_dLengthMainDolchulUpDan[1] = 0;
		pDeckData->m_dLengthMainDolchulLo[0]	= dLenCenterSideL;
		pDeckData->m_dLengthMainDolchulLoDan[0]	= dLenDanbuSideL; 
		pDeckData->m_dLengthMainDolchulLo[1]	= dLenCenterSideR;
		pDeckData->m_dLengthMainDolchulLoDan[1]	= dLenDanbuSideR;
	}
	else
	{
		pDeckData->m_dLengthMainDolchulUp[0]    = dLenCenterSideL;
		pDeckData->m_dLengthMainDolchulUpDan[0] = dLenDanbuSideL; 
		pDeckData->m_dLengthMainDolchulUp[1]    = dLenCenterSideR;
		pDeckData->m_dLengthMainDolchulUpDan[1] = dLenDanbuSideR;
		pDeckData->m_dLengthMainDolchulLo[0]	= 0;
		pDeckData->m_dLengthMainDolchulLoDan[0]	= 0; 
		pDeckData->m_dLengthMainDolchulLo[1]	= 0; 
		pDeckData->m_dLengthMainDolchulLoDan[1]	= 0; 
	}
}



void CDeckMainRebarDlg::SetDataInit() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;	
	
	SetGridData();

	m_Grid.UpdateData(FALSE);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.EnableInsertCol();
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.MergeGrid(0, 1, 0, m_Grid.GetColumnCount());
	
	m_bCheckRailType = pDeckData->m_bMainRebarLowerLevel;

	m_szMainShape = pDeckData->m_nDetailMainUpper==0 ? "상부직선타입" : "상부ㄱ타입";
	m_szMainCenterShape = pDeckData->m_nDetailMainLowerCenter==0 ? "분리배치" : (pDeckData->m_nDetailMainLowerCenter==1 ? "중앙부배치" : "전체배치");
	long nCyclesu = pDeckData->GetRebarCycleSuBySection(GetSectionIDByCurTab());
	m_szMainCycle.Format("%dCycle", nCyclesu);
	if(pDeckData->m_nDetailSettlement==0)
		m_szSettleOption = "헌치 상단부 기준";
	else if(pDeckData->m_nDetailSettlement==1)
		m_szSettleOption = "헌치 하단부 기준";
	else if(pDeckData->m_nDetailSettlement==2)
		m_szSettleOption = "복부판 내측기준";
	m_szDivideRebar = pDeckData->m_bDivideRebarAtCant ? "분리" : "미분리";

	if(GetCurTabName()=="일반부" || GetCurTabName()=="지점부")
	{
		GetDlgItem(IDC_BUTTON_MAIN_CENTER_SHAPE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MAINCEN_SHAPE)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SETTLE_OPTION)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_SETTLE_OPTION)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DIVIDEREBAR)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_DIVIDEREBAR)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MAIN_CENTER_SHAPE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MAINCEN_SHAPE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SETTLE_OPTION)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_SETTLE_OPTION)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DIVIDEREBAR)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DIVIDEREBAR)->ShowWindow(FALSE);
	}


	UpdateData(FALSE);
}



void CDeckMainRebarDlg::SetDataSave() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_Grid.UpdateData(TRUE);
	
	pDeckData->m_bMainRebarLowerLevel = m_bCheckRailType;
}




BOOL CDeckMainRebarDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
		
	}
	
	m_Grid.ViewGridCellForCombo(pMsg);

	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_RBUTTONDOWN)
	{		
		CCellID cellID = m_Grid.GetFocusCell();				
		if(m_Grid.GetTextMatrix(cellID.row,cellID.col)=="-")		
			m_Grid.EnableInsertCol(FALSE);					
		else		
			m_Grid.EnableInsertCol(TRUE);					
	}	

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}



void CDeckMainRebarDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	CString sText = m_Grid.GetTextMatrix(pGridView->iRow, 0);
	SetDataSave();
	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nSection = GetSectionIDByCurTab();
	if(sText=="직경")
		pDeckData->MakeCycleMainRebarReCalc(nSection);
	pDeckData->MakeShapeMainRebar(nSection);

	SetDataInit();
	
	DrawInputDomyunView(FALSE);
}

void CDeckMainRebarDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;
	
	CString sz  = m_Grid.GetTextMatrix(0, pGridView->iColumn);
	sz = sz.GetAt(0);
	long nCycle = atol(sz)-1;

	//m_comboViewCycle.SetCurSel(nCycle);

	if (!IsInitDialog())
		DrawInputDomyunView();
}




void CDeckMainRebarDlg::OnButtonMirrorJoint() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	//
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell==NULL)
	{
		AfxMessageBox("반전시킬 구간을 선택하세요");
		return;
	}
	
	long nCol      = pCell ? pCell->nCol : 0;
	long nSection  = GetSectionIDByCurTab();
	CString sText1 = m_Grid.GetTextMatrix(0, nCol);		// 횡단/종단
	CString sText2 = m_Grid.GetTextMatrix(1, nCol);		// 시점/종점
	CString sText3 = m_Grid.GetTextMatrix(2, nCol);		// 좌/우
	CString ch = sText1.GetAt(0);
	long nCycle = atol(ch)-1;
	if(nCycle<0)	nCycle = 0;
	BOOL bUpper = sText2=="상면" ? TRUE : FALSE;

	//
	CDPoint vAng = CDPoint(0,1);
	CDPoint vAngSec = CDPoint(0,1);
	if(nSection==SECTION_SKEW_STT)
		vAngSec = pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	else if(nSection==SECTION_SKEW_END)
		vAngSec = pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	double dSta	    = pDeckData->GetStationBySection(nSection);
	double dSlabW   = pDB->GetWidthAct(dSta)/fabs(vAngSec.y) - (pDeckData->m_dCoverSd/fabs(vAng.y))*2;
	//
	REBAR_CYCLE  *pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);
	CDoubleArray *pArrJoint = bUpper ? &pRebarCycle->m_dArrJointUp : &pRebarCycle->m_dArrJointDn;
	// 마지막 구간을 계산하여 넣어줌.
	double dLenSum  = 0;
	long n = 0;
	for (n=0; n<pArrJoint->GetSize(); n++)
		dLenSum += pArrJoint->GetAt(n);
	double dLenLast = dSlabW-dLenSum;

	CDoubleArray arrTemp = pArrJoint;
	CDoubleArray arrTempInv; 

	arrTemp.Add(dLenLast);

	//
	for (n=arrTemp.GetSize()-1; n>=0; n--)
		arrTempInv.Add(arrTemp[n]);
	arrTempInv.RemoveAt(arrTempInv.GetUpperBound());

	for (n=0; n<arrTempInv.GetSize(); n++)
		pArrJoint->GetAt(n) = arrTempInv[n];
	
	SetDataInit();

	pDeckData->MakeShapeMainRebar(nSection); 
	
	DrawInputDomyunView(TRUE);		
}

//추가
void CDeckMainRebarDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nRow = pGridView->iRow;
    int nCol = pGridView->iColumn;

	InsertJointByGridCol(nCol);

	SetDataInit();
	SetDataSave();

	CADeckData *pDeckData = m_pStd->m_pDeckData;	
	long nSection = GetSectionIDByCurTab();
	pDeckData->MakeShapeMainRebar(nSection); 

	DrawInputDomyunView(FALSE);
}



//삭제
void CDeckMainRebarDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nRow = pGridView->iRow;
    int nCol = pGridView->iColumn;	

	DeleteJointByGridCol();

	SetDataInit();
	SetDataSave();

	CADeckData *pDeckData = m_pStd->m_pDeckData;	
	long nSection = GetSectionIDByCurTab();
	pDeckData->MakeShapeMainRebar(nSection); 

	DrawInputDomyunView(FALSE);
}



//삭제
void CDeckMainRebarDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{			
	long nSection = GetSectionIDByCurTab();

	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nCol = pGridView->iColumn;

	CString sText1 = m_Grid.GetTextMatrix(0, nCol);		// 횡단/종단
	CString sText2 = m_Grid.GetTextMatrix(1, nCol);		// 시점/종점
	CString sText3 = m_Grid.GetTextMatrix(2, nCol);		// 좌/우

	CString ch;
	if(sText1.GetLength()>0)	ch = sText1.GetAt(0);
	else						ch = "0";

	m_nCycleClick = atol(ch)-1;
	
	m_bUpperClick = sText2=="상면" ? TRUE : FALSE;
	
	if(sText3.GetLength()>0)	ch = sText3.GetAt(0);
	else						ch = "0";

	m_nJointClick= atol(ch)-1;
}


void CDeckMainRebarDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	CString sText1 = m_Grid.GetTextMatrix(0, nCol);		// 횡단/종단
	CString sText2 = m_Grid.GetTextMatrix(1, nCol);		// 시점/종점
	CString sText3 = m_Grid.GetTextMatrix(2, nCol);		// 좌/우

	CString ch = sText1.GetAt(0);
	long nCycle = atol(ch);
	BOOL bUpper = sText2=="상면" ? TRUE : FALSE;
	if(sText3.GetLength()>0)	ch = sText3.GetAt(0);
	else						ch = "0";
	long nJoint = atol(ch);

	CString szMatch;
	szMatch = (bUpper?"U":"D") + COMMA(nJoint) + "-" + COMMA(nCycle);
	m_pView->GetDomyun()->STMakeCursor(szMatch);
}


long CDeckMainRebarDlg::GetFocusColByMultiString(CString szTitle)
{
	long nCycle = atol(CString(szTitle.GetAt(3)));
	long nJoint = atol(CString(szTitle.GetAt(1)));
	BOOL bUpper = CString(szTitle.GetAt(0))=="U" ? TRUE : FALSE;

	CDWordArray dArr;
	for (long nCol=0; nCol<m_Grid.GetColumnCount(); nCol++)
	{
		CString szMatch = COMMA(nCycle)+"Cycle";
		if(m_Grid.GetTextMatrix(0, nCol)==szMatch)
			dArr.Add(nCol);
	}
		
	CDWordArray dArr2;
	for (long nCol2=0; nCol2<dArr.GetSize(); nCol2++)
	{
		CString szMatch = bUpper ? "상면" : "하면";
		if(m_Grid.GetTextMatrix(1, dArr[nCol2])==szMatch)
			dArr2.Add(dArr[nCol2]);
	}

	for (long nCol3=0; nCol3<dArr2.GetSize(); nCol3++)
	{
		CString szMatch = COMMA(nJoint);
		if(m_Grid.GetTextMatrix(2, dArr2[nCol3])==szMatch)
			return dArr2[nCol3];
	}

	return 0;
}



void CDeckMainRebarDlg::InsertJointByGridCol(long nGridCol)
{
	long nSection = GetSectionIDByCurTab();

	CADeckData *pDeckData = m_pStd->m_pDeckData;	
	
	CDPoint xy(0, 0);
	CString sText1 = m_Grid.GetTextMatrix(0, nGridCol+1);		// Cycle
	CString sText2 = m_Grid.GetTextMatrix(1, nGridCol+1);		// 상/하면
	CString sText3 = m_Grid.GetTextMatrix(2, nGridCol+1);		// 구간

	CString ch;
	if(sText1.GetLength()>0)	ch = sText1.GetAt(0);
	else						ch = "0";

	long nCycle = atol(ch)-1;
	BOOL bUpper = sText2=="상면" ? TRUE : FALSE;
	if(sText3.GetLength()>0)	ch = sText3.GetAt(0);
	else						ch = "0";

	long nJoint = atol(ch)-1;

	REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);

	if(bUpper)
	{
		double dLength = pRebarCycle->m_dArrJointUp[nJoint];
		double dDia    = pRebarCycle->m_dArrDiaUp[nJoint];

		pRebarCycle->m_dArrJointUp[nJoint] = dLength/2;
		pRebarCycle->m_dArrJointUp.InsertAt(nJoint, dLength/2);
		pRebarCycle->m_dArrDiaUp.InsertAt(nJoint, dDia);
		pRebarCycle->m_dArrValidUp.InsertAt(nJoint, TRUE);
	}
	else
	{
		double dLength = pRebarCycle->m_dArrJointDn[nJoint];
		double dDia    = pRebarCycle->m_dArrDiaDn[nJoint];

		pRebarCycle->m_dArrJointDn[nJoint] = dLength/2;
		pRebarCycle->m_dArrJointDn.InsertAt(nJoint, dLength/2);
		pRebarCycle->m_dArrDiaDn.InsertAt(nJoint, dDia);
		pRebarCycle->m_dArrValidDn.InsertAt(nJoint, TRUE);
	}
}



void CDeckMainRebarDlg::DeleteJointByGridCol()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;	
	
	long nSection = GetSectionIDByCurTab();
	REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, m_nCycleClick);

	CDPoint xy(0, 0);

	long nSizeUp = pRebarCycle->m_dArrValidUp.GetSize();
	long nSizeDn = pRebarCycle->m_dArrValidDn.GetSize();
	
	double dDis1 = 0;
	double dDis2 = 0;

	if(m_bUpperClick)	
	{
		if(nSizeUp==2)	// 이음이 하나일때
		{
			pRebarCycle->m_dArrJointUp.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrDiaUp.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrValidUp.RemoveAt(m_nJointClick);
		}
		else if(nSizeUp>2 && m_nJointClick+1==nSizeUp-1)	// 마지막 이음위치
		{
			dDis1	= pRebarCycle->m_dArrJointUp.GetAt(m_nJointClick);
			dDis2	= pRebarCycle->m_dArrJointUp.GetAt(m_nJointClick-1);

			pRebarCycle->m_dArrJointUp.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrDiaUp.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrValidUp.RemoveAt(m_nJointClick);

			pRebarCycle->m_dArrJointUp[m_nJointClick-1] = dDis1+dDis2;
		}
		else			// 이음이 둘이상일 경우
		{
			dDis1	= pRebarCycle->m_dArrJointUp.GetAt(m_nJointClick);
			dDis2	= pRebarCycle->m_dArrJointUp.GetAt(m_nJointClick+1);

			pRebarCycle->m_dArrJointUp.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrDiaUp.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrValidUp.RemoveAt(m_nJointClick);

			pRebarCycle->m_dArrJointUp[m_nJointClick] = dDis1+dDis2;
		}
	}
	else
	{
		if(nSizeDn==2)
		{
			pRebarCycle->m_dArrJointDn.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrDiaDn.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrValidDn.RemoveAt(m_nJointClick);
		}
		else if(nSizeDn>2 && m_nJointClick+1==nSizeDn-1)
		{
			dDis1	= pRebarCycle->m_dArrJointDn.GetAt(m_nJointClick);
			dDis2	= pRebarCycle->m_dArrJointDn.GetAt(m_nJointClick-1);

			pRebarCycle->m_dArrJointDn.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrDiaDn.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrValidDn.RemoveAt(m_nJointClick);

			pRebarCycle->m_dArrJointDn[m_nJointClick-1] = dDis1+dDis2;
		}
		else
		{
			dDis1	= pRebarCycle->m_dArrJointDn.GetAt(m_nJointClick);
			dDis2	= pRebarCycle->m_dArrJointDn.GetAt(m_nJointClick+1);

			pRebarCycle->m_dArrJointDn.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrDiaDn.RemoveAt(m_nJointClick);
			pRebarCycle->m_dArrValidDn.RemoveAt(m_nJointClick);

			pRebarCycle->m_dArrJointDn[m_nJointClick] = dDis1+dDis2;
		}
	}
}



void CDeckMainRebarDlg::OnCheckMark() 
{
	UpdateData();

	DrawInputDomyunView(FALSE);
}


void CDeckMainRebarDlg::OnViewBox() 
{
	UpdateData();

	DrawInputDomyunView(FALSE);
}

BOOL CDeckMainRebarDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}



void CDeckMainRebarDlg::SetFocusItem(CString szTitle)
{
	long nRow = 3;
	long nCol = GetFocusColByMultiString(szTitle);

	GetFocus();
	
	m_Grid.SetSelectedRange(-1,-1,-1,-1);
	m_Grid.SetFocusCell(nRow,nCol);					
	m_Grid.SendMessage(WM_KEYDOWN,VK_F2);
}



BOOL CDeckMainRebarDlg::IsFocusItem(CString &szTitle)
{
	CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
	if(pGrid->GetSafeHwnd())
	{
		if (szTitle.Find("U")!=-1 || szTitle.Find("D")!=-1)
		{
			return TRUE;
		}
	}
	return FALSE;
}


long CDeckMainRebarDlg::GetSectionIDByCurTab()
{
	// 단부 : 일반교량 - 슬래브 시점부
	if (GetCurTabName()=="단부" || GetCurTabName()=="단부/일반부")	
	{
		return SECTION_DAN;
	}
	else if (GetCurTabName()=="일반부")	
	{
		return SECTION_CEN;
	}
	else if (GetCurTabName()=="지점부")	
	{
		return SECTION_PIER;
	}
	else if (GetCurTabName()=="사보강부(시점)")	return SECTION_SKEW_STT;
	else if (GetCurTabName()=="사보강부(종점)")	return SECTION_SKEW_END;

	return -1;
}



long CDeckMainRebarDlg::GetSectionIDByTabName(CString sTab)
{
	if (sTab=="단부" || sTab=="단부/일반부")	
	{
		return SECTION_DAN;
	}
	else if (sTab=="일반부")	
	{
		return SECTION_CEN;
	}
	else if (sTab=="지점부")	
	{
		return SECTION_PIER;
	}
	else if (sTab=="사보강부(시점)")	return SECTION_SKEW_STT;
	else if (sTab=="사보강부(종점)")	return SECTION_SKEW_END;

	return -1;
}


CString CDeckMainRebarDlg::GetCurTabName()
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


CString CDeckMainRebarDlg::GetTabName(long nTab)
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



void CDeckMainRebarDlg::OnCheckViewBox() 
{
	UpdateData(TRUE);

	DrawInputDomyunView(FALSE);	
}


void CDeckMainRebarDlg::OnCheckMainLowerLevel() 
{
	UpdateData(TRUE);

	AfxMessageBox("지원예정입니다.");

/*	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	pDeckData->m_bMainRebarLowerLevel = m_bCheckRailType;

	for (long nTab=0; nTab<m_ctlTab.GetItemCount(); nTab++)
	{
		long nSection = GetSectionIDByTabName(GetTabName(nTab));
		pDeckData->MakeShapeMainRebar(nSection);
	}*/
	DrawInputDomyunView(FALSE);	
}


void CDeckMainRebarDlg::OnSelchangeComboViewCycle() 
{
	DrawInputDomyunView(TRUE);	
}


void CDeckMainRebarDlg::OnButtonApply() 
{
	UpdateData();

	SetDataSave();
	SetDataInit();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	
	long nSectionEnd = pDB->m_nQtyJigan==1 ? SECTION_SKEW_END : SECTION_PIER;

	for (long nSection=0; nSection<=nSectionEnd; nSection++)
		pDeckData->MakeShapeMainRebar(nSection); 

	if(pDeckData->m_bRecalcJewon && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag())
		pDeckData->MakeRebarJewon();

	DrawInputDomyunView();		
}


void CDeckMainRebarDlg::OnButtonMainShape() 
{
	UpdateData();

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_DetailMainUpperDlg.m_pDeckData = pDeckData;
	if(m_DetailMainUpperDlg.DoModal()==IDOK)
	{
		if(MessageBox("주철근의 Cycle 및 형태를 재계산합니다.\n 계속 하시겠습니까 ?","확인",MB_ICONQUESTION | MB_YESNO)==IDYES)
		{
			SetDataDefaultOpt();

			long nSectionEnd = m_pStd->GetBridge()->m_nQtyJigan==1 ? SECTION_SKEW_END : SECTION_PIER;
			for (long nSection=0; nSection<=nSectionEnd; nSection++)
			{
				pDeckData->MakeCycleMainRebar(nSection);
				pDeckData->MakeShapeMainRebar(nSection);
			}
			SetDataInit();
			DrawInputDomyunView(FALSE);	
		}
	}
}

void CDeckMainRebarDlg::OnButtonMainCenterShape() 
{
	UpdateData();

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_DetailMainLowerCenterDlg.m_pDeckData = pDeckData;
	if(m_DetailMainLowerCenterDlg.DoModal()==IDOK)
	{
		if(MessageBox("주철근의 Cycle 및 형태를 재계산합니다.\n 계속 하시겠습니까 ?","확인",MB_ICONQUESTION | MB_YESNO)==IDYES)
		{
			SetDataDefaultOpt();

			long nSectionEnd = m_pStd->GetBridge()->m_nQtyJigan==1 ? SECTION_SKEW_END : SECTION_PIER;
			for (long nSection=0; nSection<=nSectionEnd; nSection++)
			{
				pDeckData->MakeCycleMainRebar(nSection);
				pDeckData->MakeShapeMainRebar(nSection);
			}
			SetDataInit();
			InitComboCycle();
			DrawInputDomyunView(FALSE);		
		}
	}
}

void CDeckMainRebarDlg::OnButtonSettleOption() 
{
	UpdateData();

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_DetailSettlementDlg.m_pDeckData = pDeckData;
	if(m_DetailSettlementDlg.DoModal()==IDOK)
	{
		if(MessageBox("주철근의 Cycle 및 형태를 재계산합니다.\n 계속 하시겠습니까 ?","확인",MB_ICONQUESTION | MB_YESNO)==IDYES)
		{
			SetDataDefaultOpt();

			long nSectionEnd = m_pStd->GetBridge()->m_nQtyJigan==1 ? SECTION_SKEW_END : SECTION_PIER;
			for (long nSection=0; nSection<=nSectionEnd; nSection++)
			{
				pDeckData->MakeCycleMainRebar(nSection);
				pDeckData->MakeShapeMainRebar(nSection);
			}

			SetDataInit();
			InitComboCycle();
			DrawInputDomyunView(FALSE);		
		}
	}
}

void CDeckMainRebarDlg::OnButtonMainCycle() 
{
	UpdateData();

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_DetailCyclesuDlg.m_pDeckData = pDeckData;
	if(m_DetailCyclesuDlg.DoModal()==IDOK)
	{
		if(MessageBox("주철근의 Cycle 및 형태를 재계산합니다.\n 계속 하시겠습니까 ?","확인",MB_ICONQUESTION | MB_YESNO)==IDYES)
		{
			SetDataDefaultOpt();

			long nSection = GetSectionIDByCurTab();
				if(pDeckData->m_nMainCyclesu==1)
					pDeckData->MakeCycleMainRebar1Cy(nSection);
				else if(pDeckData->m_nMainCyclesu==2)
					pDeckData->MakeCycleMainRebar(nSection);
				else
					pDeckData->MakeCycleMainRebar4Cy(nSection);
				
				pDeckData->MakeShapeMainRebar(nSection);

			SetDataInit();
			InitComboCycle();
			DrawInputDomyunView(FALSE);		
		}
	}	
}

void CDeckMainRebarDlg::OnButtonDivideRebar() 
{
	UpdateData();

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_DetailDivideRebar.m_pDeckData = pDeckData;
	if(m_DetailDivideRebar.DoModal()==IDOK)
	{
		if(MessageBox("주철근의 Cycle 및 형태를 재계산합니다.\n 계속 하시겠습니까 ?","확인",MB_ICONQUESTION | MB_YESNO)==IDYES)
		{
			long nSectionEnd = m_pStd->GetBridge()->m_nQtyJigan==1 ? SECTION_SKEW_END : SECTION_PIER;
			for (long nSection=0; nSection<=nSectionEnd; nSection++)
			{
				if(pDeckData->m_nMainCyclesu==1)
					pDeckData->MakeCycleMainRebar1Cy(nSection);
				else if(pDeckData->m_nMainCyclesu==2)
					pDeckData->MakeCycleMainRebar(nSection);
				else
					pDeckData->MakeCycleMainRebar4Cy(nSection);
				
				pDeckData->MakeShapeMainRebar(nSection);
			}

			SetDataInit();
			InitComboCycle();
			DrawInputDomyunView(FALSE);		
		}
	}	
}
