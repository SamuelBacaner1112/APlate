// FactJoinPositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "SubRistRefImgDlg.h"
#include "RistModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFactJoinPositionDlg dialog


CFactJoinPositionDlg::CFactJoinPositionDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CFactJoinPositionDlg::IDD, pParent)
{
	m_ID	= CFactJoinPositionDlg::IDD;
	nCurMark = -1;
	m_TabWnd = NULL;

	//{{AFX_DATA_INIT(CFactJoinPositionDlg)
	m_ComboGir = -1; 
	m_bSameApplySameCol = TRUE;
	m_cFactThickFix = FALSE;
	m_nLengthType = 1;
	m_bCheckMove = FALSE;
	m_bCheckSymbol = TRUE;
	//}}AFX_DATA_INIT
	// 정렬을 위한 정의

	m_nPreGridColumnCount = 0;

	//APLATE-709 insertCol이 두번호출된다. 주석처리
	//m_GridU.SetSendMessageToParentOwner(TRUE);
	//m_GridL.SetSendMessageToParentOwner(TRUE);
	//m_GridW.SetSendMessageToParentOwner(TRUE);
}


CFactJoinPositionDlg::~CFactJoinPositionDlg()
{
	if(m_TabWnd->GetSafeHwnd()) m_TabWnd->DestroyWindow();
	delete m_TabWnd;
}

void CFactJoinPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFactJoinPositionDlg)
// 	DDX_Control(pDX, IDC_FACTJOIN_DISTANCE_CENTER, m_Edit1);
// 	DDX_Control(pDX, IDC_FACTJOIN_GIRDERLEN, m_Radio1);
// 	DDX_Control(pDX, IDC_FACTJOIN_BRIDGELEN, m_Radio2);
	DDX_Check(pDX, IDC_FACTJOIN_ALLAPPLY, m_bSameApplySameCol);
	DDX_Check(pDX, IDC_FACTJOIN_THICK, m_cFactThickFix);
	DDX_Control(pDX, IDC_FACTJOIN_MOVEDIS, m_dDisOnCenterFactJoin);
	DDX_Radio(pDX, IDC_FACTJOIN_GIRDERLEN, m_nLengthType);
	DDX_Control(pDX, IDC_FACTJOIN_DISTANCE_CENTER, m_dDisCenterOnFact);
	DDX_Check(pDX, IDC_FACTJOIN_CHECKMOVE, m_bCheckMove);
	DDX_Control(pDX, IDC_EDIT_FLANGE_WIDTH, m_dFlangeSameWidth);
	DDX_Check(pDX, IDC_FACTJOIN_CHECKSYMBOL, m_bCheckSymbol);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FACTJOIN_MOVEPREV, m_btnMoveLeft);
	DDX_Control(pDX, IDC_FACTJOIN_MOVENEXT, m_btnMoveRight);
	DDX_Control(pDX, IDC_FACTJOIN_MOVE1, m_btnMove1);
	DDX_Control(pDX, IDC_FACTJOIN_MOVE2, m_btnMove2);

	DDX_Control(pDX, IDC_FACTJOIN_AUTO_SYMMETRY, m_btnSymmetry);
	DDX_Control(pDX, IDC_BUTTON_FLANGE_WIDTH_SAME, m_btnSameWidth);

	DDX_Control(pDX, IDC_BUTTON_RIST_REF1, m_btnAutoLength);
	DDX_Control(pDX, IDC_BUTTON_RIST_REF2, m_btnAutoThick);
	DDX_Control(pDX, IDC_RIST_RECOMMEND, m_btnAutoConnect);

	DDX_Control(pDX, IDC_DEFAULT_MATERIAL, m_btnMaterial);
	DDX_Control(pDX, IDC_FACTJOIN_ADVICE1, m_btnAdvice1);
	DDX_Control(pDX, IDC_FACTJOIN_ADVICE2, m_btnAdvice2);
	DDX_Control(pDX, IDC_FACTJOIN_ADVICE3, m_btnAdvice3);
	DDX_Control(pDX, IDC_FACTJOIN_ALLEQUAL, m_btnSameAll);
}

BEGIN_MESSAGE_MAP(CFactJoinPositionDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CFactJoinPositionDlg)
	ON_BN_CLICKED(IDC_FACTJOIN_MOVENEXT, OnMoveRight)
	ON_BN_CLICKED(IDC_FACTJOIN_MOVEPREV, OnMoveLeft)	
	ON_BN_CLICKED(IDC_FACTJOIN_AUTOCALCTHICK, OnFactjoinAutocalcthick)
	ON_BN_CLICKED(IDC_FACTJOIN_USERCALC, OnFactjoinUsercalc)
	ON_BN_CLICKED(IDC_FACTJOIN_AUTOREVISION, OnFactjoinAutorevision)		
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FACTJOIN_CHANGETHICK, OnFactjoinChangethick)
	ON_BN_CLICKED(IDC_FACTJOIN_ALLAPPLY, OnSameApplySameCol)	
	ON_BN_CLICKED(IDC_FACTJOIN_ADVICE1, OnSetDataDefault)
	ON_BN_CLICKED(IDC_FACTJOIN_ADVICE2, OnSetDataDefaultSecond)
	ON_BN_CLICKED(IDC_FACTJOIN_ADVICE3, OnSetDataDefaultThird)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnDialogDataSaveAndRedraw)	
	ON_BN_CLICKED(IDC_FACTJOIN_MOVE1, OnMoveByMoment)
	ON_BN_CLICKED(IDC_FACTJOIN_MOVE2, OnMoveByStress)	
	ON_BN_CLICKED(IDC_FACTJOIN_THICK, OnFactjoinThick)
	ON_BN_CLICKED(IDC_FACTJOIN_AUTO_SYMMETRY, OnSetDataSymmetry)
	ON_BN_CLICKED(IDC_FACTJOIN_ALLEQUAL, OnFactjoinAllequal)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_FACTJOIN_BRIDGELEN, OnFactjoinBridgelen)
	ON_BN_CLICKED(IDC_FACTJOIN_GIRDERLEN, OnFactjoinGirderlen)	
	ON_BN_CLICKED(IDC_FACTJOIN_CHECKMOVE, OnFactjoinCheckmove)
	ON_BN_CLICKED(IDC_BUTTON_FLANGE_WIDTH_SAME, OnButtonFlangeWidthSame)
	ON_BN_CLICKED(IDC_FACTJOIN_CHECKSYMBOL, OnFactjoinChecksymbol)
	ON_BN_CLICKED(IDC_RIST_RECOMMEND, OnRistRecommend)
	ON_BN_CLICKED(IDC_BUTTON_RIST_REF1, OnButtonRistRef1)
	ON_BN_CLICKED(IDC_BUTTON_RIST_REF2, OnButtonRistRef2)
	ON_BN_CLICKED(IDC_BUTTON_RIST_REF3, OnButtonRistRef3)	
	ON_BN_CLICKED(IDC_DEFAULT_MATERIAL, OnDefaultMaterial)
	ON_EN_KILLFOCUS(IDC_FACTJOIN_MOVEDIS, OnKillfocusFactjoinMovedis)	
	ON_EN_KILLFOCUS(IDC_FACTJOIN_DISTANCE_CENTER, OnKillfocusFactjoinDistanceCenter)
	//}}AFX_MSG_MAP
	
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_FACTJOIN_UFLANGE_GRID, OnCellChangedUpper)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_FACTJOIN_LFLANGE_GRID, OnCellChangedLower)	
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_FACTJOIN_WEB_GRID, OnCellChangedWeb)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_FACTJOIN_UFLANGE_GRID, OnCellInsertedCol)	
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_FACTJOIN_LFLANGE_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_FACTJOIN_WEB_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_FACTJOIN_UFLANGE_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_FACTJOIN_LFLANGE_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_FACTJOIN_WEB_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_FACTJOIN_UFLANGE_GRID, OnCellDeletedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_FACTJOIN_LFLANGE_GRID, OnCellDeletedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_FACTJOIN_WEB_GRID, OnCellDeletedCol)

	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_FACTJOIN_UFLANGE_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_FACTJOIN_LFLANGE_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_FACTJOIN_WEB_GRID, OnCellChangedData)

	ON_NOTIFY(GVN_CELL_COPIED_DATA, IDC_FACTJOIN_UFLANGE_GRID, OnCellCopiedData)
	ON_NOTIFY(GVN_CELL_COPIED_DATA, IDC_FACTJOIN_LFLANGE_GRID, OnCellCopiedData)
	ON_NOTIFY(GVN_CELL_COPIED_DATA, IDC_FACTJOIN_WEB_GRID, OnCellCopiedData)

	// Tab
	ON_MESSAGE(TCN_XT_SELCHANGING, OnTabChangePre)
	ON_MESSAGE(TCN_XT_SELCHANGE, OnTabChange)

	// USER_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFactJoinPositionDlg message handlers
BOOL CFactJoinPositionDlg::Create(CWnd * pWnd)
{
	return CGeneralBaseDlg::Create(m_ID,pWnd);
}


BOOL CFactJoinPositionDlg::PreTranslateMessage(MSG* pMsg) 
{	
	CGridCtrl *pGrid = GetGridCurrentTab();


	if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == m_Edit3.m_hWnd)
		{
			OnButtonFlangeWidthSame();//플랜지폭원 동일적용 버튼 실행
			return TRUE;
		}
	}

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = pGrid->GetFocusCell();
		CString szSelText = pGrid->GetTextMatrix(next.row,next.col);
		CSteel *pSteel = m_pStd->m_pDataManage->GetSteel();

		next.row--;

		if( ( IsActiveWeb() &&  ( next.row % W_CYCLE == W_SYMBOL ) ) ||// 복부판
			( !IsActiveWeb() &&  ( next.row % UL_CYCLE == UL_SYMBOL ) ) )// 상하부판
		{
			CStringArray strCombo;
			for(int n = 0; n < pSteel->GetSize(); n++)
			{
				strCombo.Add(pSteel->GetSymbol(n));
				pGrid->SetComboString(strCombo);
			}

		}
	}


	if(pGrid->TranslateMsg(pMsg))
		return pGrid->PreTranslateMessage(pMsg);

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}


BOOL CFactJoinPositionDlg::OnInitDialog() 
{
	CString	sText	= _T("");

	sText.Format("%d",	800);

	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_FACTJOING_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_DEFAULT_MATERIAL,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_ADVICE1,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_ADVICE2,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_ADVICE3,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_ALLEQUAL,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_FACTJOIN_THICK,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_1,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_GIRDERLEN,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_BRIDGELEN,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_DISTANCE_CENTER,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_ALLAPPLY,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_CHECKSYMBOL,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_FACTJOIN_CHECKMOVE,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_FACTJOIN_AUTO_SYMMETRY, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_EDIT_FLANGE_WIDTH, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_MM, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FLANGE_WIDTH_SAME, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_GROUP, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_SHARP, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_MINT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_USET, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_RESULT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_GROUP5, SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_RIST, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_RIST_REF1, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_RIST_REF2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RIST_RECOMMEND, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	return TRUE;
}

BOOL CFactJoinPositionDlg::SetGridTitle(CGridCtrl* pGrid)
{
	long r, c;
	CString cs;

	long nRow = pGrid->GetRowCount();
	long nCol = pGrid->GetColumnCount();

	pGrid->SetShowComboDownImage(FALSE);
	pGrid->EnableReturnPass(TRUE);
	pGrid->SetEditable(TRUE);
	pGrid->SetListMode(FALSE);
	pGrid->EnableDragAndDrop(TRUE);
	pGrid->EnableInsertRow(FALSE);	
	pGrid->EnableInsertCol(TRUE);	
	pGrid->EnableTitleTips(FALSE);
	pGrid->SetTextBkColor(RGBEDITABLE);	
	pGrid->SetFixedRowCount(1);	
	pGrid->SetFixedColumnCount(1);	

	pGrid->SetColumnWidthAll(65);
	pGrid->SetTextMatrix(0, 0, _T("구 분"));

	pGrid->SetRowHeightAll(20);
	pGrid->SetColumnWidth(0,80);
	pGrid->SetRowHeight(0, 20);

	for(c=1; c<nCol; c++)   
	{
		cs.Format("%d구간",c);
		pGrid->SetTextMatrix(0,c,cs);
	}

	if(IsActiveWeb(pGrid))	// 복부
	{
		for(r = 1; r < nRow; r+=W_CYCLE) 
		{
			long nG = (r-W_CYCLE-1) / W_CYCLE;
			CString sGir("CR");
			if(r > 1)
				sGir.Format("G%d", nG+1+m_pStd->GetBridge()->GetNumberStartGirder());

			cs.Format("길이(%s)", sGir);
			pGrid->SetTextMatrix(r+W_LEN,0,cs);
			cs.Format("두께(%s)", sGir);
			pGrid->SetTextMatrix(r+W_THICK,0,cs);
			cs.Format("변경강종(%s)", sGir);
			pGrid->SetTextMatrix(r+W_SYMBOL,0,cs);
			cs.Format("이동(%s)", sGir);
			pGrid->SetTextMatrix(r+W_MOVEDIS,0,cs);

				
			long nLastCol = pGrid->GetValidCol(r, FALSE);
			for (c = 1; c <= nLastCol; c++) 
			{
				pGrid->SetItemState(r, c, pGrid->GetItemState(r, c) & ~GVIS_READONLY);
				pGrid->SetItemBkColour(r, c, RGBEDITABLE);
				pGrid->SetItemState(r+W_MOVEDIS, c, pGrid->GetItemState(r+W_MOVEDIS,c) | GVIS_READONLY);
				pGrid->SetItemBkColour(r+W_MOVEDIS, c, RGBREADONLY);
			}
			pGrid->SetItemState(r, nLastCol, pGrid->GetItemState(r,nLastCol) | GVIS_READONLY);
			pGrid->SetItemBkColour(r, nLastCol, RGBREADONLY);
			if(!m_bCheckMove)
				pGrid->SetRowHeight(r+W_MOVEDIS, 0);
			if(!m_bCheckSymbol)
				pGrid->SetRowHeight(r+W_SYMBOL, 0);

			

			for(int n = 1; n < nCol; n++)
			{
				pGrid->SetCellType(r+W_SYMBOL,n,CT_COMBO);
				pGrid->SetItemFormat(r+W_SYMBOL, n, (UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			}

		}
	}
	else	// 상부, 하부
	{
		for(r=1; r<nRow; r+=UL_CYCLE) 
		{
			long nG = (r-UL_CYCLE-1) / UL_CYCLE;
			CString sGir("CR");
			if(r > 1)
				sGir.Format("G%d", nG+1+m_pStd->GetBridge()->GetNumberStartGirder());

				cs.Format("길이(%s)", sGir);
				pGrid->SetTextMatrix(r+UL_LEN,0,cs);
				cs.Format("두께(%s)", sGir);
				pGrid->SetTextMatrix(r+UL_THICK,0,cs);
				cs.Format("폭  (%s)", sGir);
				pGrid->SetTextMatrix(r+UL_WIDTH,0,cs);
				cs.Format("변경강종(%s)", sGir);
				pGrid->SetTextMatrix(r+UL_SYMBOL,0,cs);
				cs.Format("이동(%s)", sGir);
				pGrid->SetTextMatrix(r+UL_MOVEDIS,0,cs);


			long nLastCol = pGrid->GetValidCol(r, FALSE);
			for (c = 1; c <= nLastCol; c++) 
			{
				pGrid->SetItemState(r, c, pGrid->GetItemState(r, c) & ~GVIS_READONLY);
				pGrid->SetItemBkColour(r, c, RGBEDITABLE);
				pGrid->SetItemState(r+UL_MOVEDIS, c, pGrid->GetItemState(r+UL_MOVEDIS,c) | GVIS_READONLY);
				pGrid->SetItemBkColour(r+UL_MOVEDIS, c, RGBREADONLY);
			}
			pGrid->SetItemState(r, nLastCol, pGrid->GetItemState(r,nLastCol) | GVIS_READONLY);
			pGrid->SetItemBkColour(r, nLastCol, RGBREADONLY);
			if (!m_bCheckMove)
				pGrid->SetRowHeight(r+UL_MOVEDIS, 0);
			if(!m_bCheckSymbol)
				pGrid->SetRowHeight(r+UL_SYMBOL, 0);
			for(int n = 1; n <= nLastCol; n++)
			{
				pGrid->SetCellType(r+UL_SYMBOL,n,CT_COMBO);
				pGrid->SetItemFormat(r+UL_SYMBOL, n, (UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			}
		}
	}

	return TRUE;
}

void CFactJoinPositionDlg::SetDataInit()
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CString			sText	= _T("");
	
	m_bSameApplySameCol		= (BOOL)pDB->m_cSameApplySameColFactJoin;

	sText.Format("%g", pDB->m_dDisOnCenterFactJoin);

	m_dDisOnCenterFactJoin.SetWindowText(sText);

	m_cFactThickFix			= pDB->m_cFactThickFix;

	sText.Format("%g",	pDB->m_dSameFlangeWidth);
	
	m_dFlangeSameWidth.SetWindowText(sText);

	UpdateData(FALSE);

// 	GetDlgItem(IDC_STATIC_GROUP5)->ShowWindow(pDB->GetBridgeType()==PLATE_GIR ? SW_HIDE : SW_SHOW);	
// 	GetDlgItem(IDC_RIST_RECOMMEND)->ShowWindow(pDB->GetBridgeType()==PLATE_GIR ? SW_HIDE : SW_SHOW);	
// 	GetDlgItem(IDC_BUTTON_RIST_REF1)->ShowWindow(pDB->GetBridgeType()==PLATE_GIR ? SW_HIDE : SW_SHOW);	
// 	GetDlgItem(IDC_BUTTON_RIST_REF2)->ShowWindow(pDB->GetBridgeType()==PLATE_GIR ? SW_HIDE : SW_SHOW);	
// 	GetDlgItem(IDC_STATIC_RIST)->ShowWindow(pDB->GetBridgeType()==PLATE_GIR ? SW_HIDE : SW_SHOW);	

// #0031825 Rist 와 계약종료로 해당 기능 감춤
	GetDlgItem(IDC_STATIC_GROUP5)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_RIST_RECOMMEND)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_BUTTON_RIST_REF1)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_BUTTON_RIST_REF2)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_STATIC_RIST)->ShowWindow(SW_HIDE);

	m_nLengthType = m_pStd->GetBridge()->m_nLengthTypeOnFact;

	sText.Format("%g",	m_pStd->GetBridge()->m_dDisCenterOnFact);

	m_dDisCenterOnFact.SetWindowText(sText);

	long nGsu = m_pStd->GetBridge()->GetGirdersu();
	CGridCtrl *pGridU = GetGridByFactType(G_F_U);
	CGridCtrl *pGridL = GetGridByFactType(G_F_L);
	CGridCtrl *pGridW = GetGridByFactType(G_W);

	pGridU->SetRowCount((nGsu+1)*UL_CYCLE + 1);
	pGridL->SetRowCount((nGsu+1)*UL_CYCLE + 1);
	pGridW->SetRowCount((nGsu+1)*W_CYCLE + 1);
	for(long nG = -1 ; nG < nGsu ; nG++)
	{
		DataInitFact(G_F_U, nG);
		DataInitFact(G_F_L, nG);
		DataInitFact(G_W, nG);
	}
	SetGridTitle(pGridU);
	SetGridTitle(pGridL);
	SetGridTitle(pGridW);	

	m_GridU.SetInsertThisOnly(!m_bSameApplySameCol);
	m_GridU.SetDeleteThisOnly(!m_bSameApplySameCol);
	m_GridL.SetInsertThisOnly(!m_bSameApplySameCol);
	m_GridL.SetDeleteThisOnly(!m_bSameApplySameCol);
	m_GridW.SetInsertThisOnly(!m_bSameApplySameCol);
	m_GridW.SetDeleteThisOnly(!m_bSameApplySameCol);	
}

CGridCtrl *CFactJoinPositionDlg::GetGridByFactType(long nMarkBujeFact)
{
	switch(nMarkBujeFact) 
	{
	case G_F_U:	return &m_GridU;
	case G_F_L:	return &m_GridL;
	}
	return &m_GridW;
}

void CFactJoinPositionDlg::InputGrid(long nMarkBujeFact, long nG, CDoubleArray &DArStaTotal, BOOL isRist)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CGridCtrl *pGrid = GetGridByFactType(nMarkBujeFact);
	CSteelSection *pSteelSection = m_pStd->m_pDataManage->GetSteelSection();
	long nFactIdxsu, nFactIdx;
	long nRow;
	CString str;
	double sta;
	double term = 0.0;
	double thick = 24;
	double width = 800;

// 6월 10일 발표용 TU 권고안 (by. 이종원부장님)
	if(pDB->IsTUGir())
	{
		if(nMarkBujeFact==G_F_L)thick = 55;
		else					thick = 15;			
	}
			
//	1구간짜리 TU기본 권고안
//	if(pDB->IsTUGir() && nMarkBujeFact==G_F_U)
//		width = 300;

	if(isRist)//공장이음 자동설정 [복부] 리스트 권고안이면...061227...KB
	{
		CRistModule RistModule;
		BOOL bSM520 = pDB->GetMaterial(G_F_U) == "SM520C-TMC";	
		double dMaxJigan = RistModule.GetLengthMaxJigan(pDB->m_nQtyJigan, pDB->m_dLengthJigan);
		thick = RistModule.GetWebThick(dMaxJigan, pDB->m_nQtyJigan, 0, 0, bSM520);		
	}
	CString strSymbol;
		strSymbol = pSteelSection->GetMaterialByMarkBuje(nMarkBujeFact);

	if(nG==-1)
		m_nPreGridColumnCount = pGrid->GetColumnCount();
	nFactIdxsu = DArStaTotal.GetSize();
	pGrid->SetColumnCount(nFactIdxsu + 1);

	if(nMarkBujeFact==G_W)	nRow = (nG+1)*W_CYCLE+1;
	else					nRow = (nG+1)*UL_CYCLE+1;

	for(nFactIdx = 0 ; nFactIdx < nFactIdxsu ; nFactIdx++)
	{
		sta = DArStaTotal[nFactIdx];
		if(nMarkBujeFact == G_F_L && !m_pStd->GetBridge()->m_cFactThickFix) // 두께,폭고정 안하면 상부 두께 가져옴
		{
			if(!pDB->IsTUGir())
			{
				width = atof(m_GridU.GetItemText(nRow+UL_WIDTH, nFactIdx+1));
				thick = atof(m_GridU.GetItemText(nRow+UL_THICK, nFactIdx+1));
				strSymbol =m_GridU.GetItemText(nRow+UL_SYMBOL, nFactIdx+1);
			}
		}
		
		SetMatrixByStation(pGrid, nRow, nFactIdx+1, sta);
		if((m_pStd->GetBridge()->m_cFactThickFix && nFactIdx > m_nPreGridColumnCount-2) || !m_pStd->GetBridge()->m_cFactThickFix)
			pGrid->SetTextMatrix(nRow+UL_THICK,nFactIdx+1, "%.0lf", thick);		

		if(nMarkBujeFact==G_W)
		{
			pGrid->SetTextMatrix(nRow+W_MOVEDIS,nFactIdx+1, "%.0lf", term);
			pGrid->SetTextMatrix(nRow+W_SYMBOL,nFactIdx+1, strSymbol,(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		}
		else
		{
			pGrid->SetTextMatrix(nRow+UL_WIDTH,nFactIdx+1, "%.0lf", width);		
			pGrid->SetTextMatrix(nRow+UL_MOVEDIS,nFactIdx+1, "%.0lf", term);
			pGrid->SetTextMatrix(nRow+UL_SYMBOL,nFactIdx+1, strSymbol,(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
		}

// 6월 10일 발표용 TU 권고안 (by. 이종원부장님)
		if(pDB->IsTUGir())
		{
			double dW = 0;
			if(nMarkBujeFact==G_F_U)
			{
				dW = (nFactIdx==RoundUp(nFactIdxsu/2, 0))? 700 : 400;
				pGrid->SetTextMatrix(nRow+UL_WIDTH,nFactIdx+1, "%.0lf", dW);
			}
			else if(nMarkBujeFact==G_F_L)
			{
				dW = (nFactIdx==RoundUp(nFactIdxsu/2, 0))? 1200 : 1100;
				if(nFactIdx==0 || nFactIdx==nFactIdxsu-1)
					dW = 800;
				pGrid->SetTextMatrix(nRow+UL_WIDTH,nFactIdx+1, "%.0lf", dW);
			}			
		}		
	}
	
	//이 밑의 구간은 어떤 이유로 만들었는지 알수가 없음.. 061227...KB
	for( ; nFactIdx < pGrid->GetColumnCount()-1; nFactIdx++)
	{
		pGrid->SetTextMatrix(nRow,nFactIdx+1, "");
		pGrid->SetTextMatrix(nRow+UL_THICK,nFactIdx+1, "");
		if(nMarkBujeFact==G_W)
			pGrid->SetTextMatrix(nRow+W_MOVEDIS,nFactIdx+1, "");
		else
			pGrid->SetTextMatrix(nRow+UL_MOVEDIS,nFactIdx+1, "");
	}
	
	pGrid->SetRedraw(TRUE,TRUE);
}

//기능 : 데이터를 그리드에 세팅함.
void CFactJoinPositionDlg::DataInitFact(long nMarkBujeFact, long nG)
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir  = pDB->GetGirder(nG);
	CGridCtrl       *pGrid = GetGridByFactType(nMarkBujeFact);

	double thick = 0;
	double width = 0;
	double stamid= 0;
	CString strSymbol;
	//	strSymbol = pSteelSection->GetMaterialByMarkBuje(nMarkBujeFact);

	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER;

	if(nMarkBujeFact == G_F_U) // 상부 
		BxFlag |= BX_UPPERFLANGE;
	else if(nMarkBujeFact == G_F_L) // 하부
		BxFlag |= BX_LOWERFLANGE;
	else // 복부판
		BxFlag |= BX_WEB;

	CPlateBxFinder Finder(pGir);
	Finder.GetBxFirst(BxFlag);
	long nFactsu = Finder.GetCountBx() - 1;

	if (nG == -1)
//	if (nG == -1 || nFactsu + 1 > pGrid->GetColumnCount())
		pGrid->SetColumnCount(nFactsu + 1);

	long nFact = 1;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
	while (pBx = Finder.GetBxNext()) 
	{
		if (pBx->IsState(BX_SPLICE) && pBx->GetFactChain(nMarkBujeFact,TRUE) == pBx->GetFactChain(nMarkBujeFact,FALSE))
			continue;

		stamid = pGir->GetStationByOneStepMid(pBx->GetStation(), 0);
		if (pBx->IsState(BX_END_GIRDER))
			stamid = pBx->GetStation();
		CPlateFactChain *pFact = pBx->GetFactChain(nMarkBujeFact, FALSE);
		//CPlateFactChain *pFact = pGir->GetFactChain(nMarkBujeFact, nFact-1);
		if (pFact)
		{
			thick = pFact->m_dFactChainThick;
			width = pFact->m_dWidth;
			strSymbol = pFact->m_strSymbol;
			
			
		}
		else  // 이 경우는 공장이음이 거더에 없을때 생김.
		{
//			if(m_pStd->GetBridge()->GetBridgeType()==PLATE_GIR)
//			{
//				thick = 12.0;
//				width = 400;
//			}
//			else
			{
				thick = 24;
				width = 800;
			}
			if(pDB->IsTUGir() && nMarkBujeFact==G_F_U)
				width = 300;
		}

		if(nMarkBujeFact==G_F_U || nMarkBujeFact==G_F_L)
		{
			SetMatrixByStation(pGrid, (nG+1)*UL_CYCLE+1+UL_LEN, nFact, pBx->GetStation());// 길이 
			pGrid->SetTextMatrix((nG+1)*UL_CYCLE+1+UL_THICK, nFact, "%.0f", thick);			// 두께
			pGrid->SetTextMatrix((nG+1)*UL_CYCLE+1+UL_WIDTH, nFact, "%.0f", width);			// 폭
			pGrid->SetTextMatrix((nG+1)*UL_CYCLE+1+UL_SYMBOL, nFact, strSymbol,(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);				// 재질
			SetMatrixByStation(pGrid, (nG+1)*UL_CYCLE+1+UL_MOVEDIS, nFact, stamid);			// 이동
		}
		else
		{
			SetMatrixByStation(pGrid, (nG+1)*W_CYCLE+1+W_LEN, nFact, pBx->GetStation());// 길이 
			pGrid->SetTextMatrix((nG+1)*W_CYCLE+1+W_THICK, nFact, "%.0f", thick);			// 두께
			pGrid->SetTextMatrix((nG+1)*W_CYCLE+1+W_SYMBOL, nFact, strSymbol,(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);				// 재질
			SetMatrixByStation(pGrid, (nG+1)*W_CYCLE+1+W_MOVEDIS, nFact, stamid);			// 이동
		}
		nFact++;

	}

	pGrid->SetRedraw(TRUE,TRUE);
}



BOOL CFactJoinPositionDlg::ContinueMsg()
{
	int ret;
	ret = MessageBox(	" 기존의 공장이음 길이가 수정됩니다.\n\n      계속 하시겠습니까? ",
						" 공장이음", MB_YESNO);
	if(ret == IDYES)	return TRUE;
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
void CFactJoinPositionDlg::MoveLeftRight(long nDir)
{
	CGridCtrl *pGrid = GetGridCurrentTab();
	if(!pGrid)  return;

	long nRow = pGrid->GetFocusCell().row;
	long nCol = pGrid->GetFocusCell().col;
	long nCols = pGrid->GetColumnCount();
	long nKindOfRowTitle = nRow % UL_CYCLE;
	if(IsActiveWeb(pGrid))//복부판
		nKindOfRowTitle = nRow % W_CYCLE;

	if(nRow < 0 || nCol < 0) return;
	if(IsActiveWeb(pGrid))
	{
		if (nKindOfRowTitle == 1+W_LEN && nCol >= nCols-1)
			return;
	}
	else
	{
		if (nKindOfRowTitle == 1+UL_LEN && nCol >= nCols-1)
			return;
	}

	if(IsActiveWeb(pGrid))
	{
		for(long nG = -1; nG < m_pStd->GetBridge()->GetGirdersu(); nG++)
		{
			long nCurG = (nRow-1) / W_CYCLE - 1;
			if (!m_bSameApplySameCol && nG != nCurG)
				continue;

			long r = nG * W_CYCLE + W_CYCLE+1;
			if(nKindOfRowTitle == 1+W_THICK) // 두께
			{
				double thick = max(atof(pGrid->GetItemText(r+W_THICK,nCol)) + 2*nDir, 0);
				pGrid->SetTextMatrix(r+W_THICK,nCol,"%.0f",thick);
			}
			else  
			{
				CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
				double dSta = GetStationMatrix(pGrid, r, nCol);
				double dNextSta = GetStationMatrix(pGrid, r, nCol+1);
				double dMoveLen;
				if(nKindOfRowTitle == 1+W_LEN)  // 길이
					dMoveLen = pGir->GetStationByOneStepMid(dSta, nDir) - pGir->GetStationByOneStepMid(dSta, 0);
				else  // 이동
					dMoveLen = nDir * 50;
				SetMatrixByStation(pGrid, r, nCol, dSta + dMoveLen);
				SetMatrixByStation(pGrid, r+W_MOVEDIS, nCol, pGir->GetStationByOneStepMid(dSta + dMoveLen, 0));
				SetMatrixByStation(pGrid, r, nCol+1, dNextSta);
			}
		}
	}
	else
	{
		for(long nG = -1; nG < m_pStd->GetBridge()->GetGirdersu(); nG++)
		{
			long nCurG = (nRow-1) / UL_CYCLE - 1;
			if (!m_bSameApplySameCol && nG != nCurG)
				continue;

			long r = nG * UL_CYCLE + UL_CYCLE+1;
			if(nKindOfRowTitle == 1+UL_THICK) // 두께
			{
				double thick = max(atof(pGrid->GetItemText(r+UL_THICK,nCol)) + 2*nDir, 0);
				pGrid->SetTextMatrix(r+UL_THICK,nCol,"%.0f",thick);
			}
			else if(nKindOfRowTitle == 1+UL_WIDTH) // 폭
			{
				double width = max(atof(pGrid->GetItemText(r+UL_WIDTH,nCol)) + 50*nDir, 0);
				pGrid->SetTextMatrix(r+UL_WIDTH,nCol,"%.0f",width);
			}
			else
			{
				CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
				double dSta = GetStationMatrix(pGrid, r, nCol);
				double dNextSta = GetStationMatrix(pGrid, r, nCol+1);
				double dMoveLen;
				if(nKindOfRowTitle == 1+UL_LEN)  // 길이
					dMoveLen = pGir->GetStationByOneStepMid(dSta, nDir) - pGir->GetStationByOneStepMid(dSta, 0);
				else  // 이동
					dMoveLen = nDir * 50;
				SetMatrixByStation(pGrid, r, nCol, dSta + dMoveLen);
				SetMatrixByStation(pGrid, r+UL_MOVEDIS, nCol, pGir->GetStationByOneStepMid(dSta + dMoveLen, 0));
				SetMatrixByStation(pGrid, r, nCol+1, dNextSta);
			}
		}
	}

	pGrid->SetFocus();
	pGrid->SetRedraw(TRUE,TRUE);
}

void CFactJoinPositionDlg::OnMoveLeft() 
{
	MoveLeftRight(-1);
	DrawInputDomyunView(FALSE);
}

void CFactJoinPositionDlg::OnMoveRight() 
{
	MoveLeftRight(1);
	DrawInputDomyunView(FALSE);
}

void CFactJoinPositionDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	if(IsActiveWeb())	//복부
		nRow -= (nRow - 1) % W_CYCLE;
	else
		nRow -= (nRow - 1) % UL_CYCLE;

	CGridCtrl *pGrid = GetGridCurrentTab();
	ASSERT(pGrid);

	if(IsActiveWeb(pGrid))	//복부
	{
		for (long row = 1; row < pGrid->GetRowCount(); row += W_CYCLE)
		{
			if (!(m_bSameApplySameCol || row == nRow))
				continue;
			if (!m_bSameApplySameCol)   {
				for (long n = 0; n < W_CYCLE; n++)
					if (row + n != GridView->iRow)
						pGrid->InsertColumn("", DT_CENTER|DT_VCENTER|DT_SINGLELINE, nCol, row + n);			
			}
			long nG = row/W_CYCLE - 1;
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
			double sta = GetStationMatrix(pGrid, row, nCol);
			double staNext = GetStationMatrix(pGrid, row, nCol+1);
			double staNew = pGir->GetStationByOneStepMid((sta + staNext) / 2, 0);
			SetMatrixByStation(pGrid, row, nCol, staNew);
			SetMatrixByStation(pGrid, row, nCol+1, staNext);
			pGrid->SetTextMatrix(row+W_THICK, nCol, pGrid->GetTextMatrix(row+W_THICK, nCol+1));
			pGrid->SetTextMatrix(row+W_SYMBOL, nCol, pGrid->GetTextMatrix(row+W_SYMBOL, nCol+1),(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			pGrid->SetTextMatrix(row+W_MOVEDIS, nCol, "0");
		}
	}
	else
	{
		for (long row = 1; row < pGrid->GetRowCount(); row += UL_CYCLE)
		{
			if (!(m_bSameApplySameCol || row == nRow))
				continue;
			if (!m_bSameApplySameCol)
			{
				for (long n = 0; n < UL_CYCLE; n++)
				{
					if (row + n != GridView->iRow)
						pGrid->InsertColumn("", DT_CENTER|DT_VCENTER|DT_SINGLELINE, nCol, row + n);
				}
			}
			long nG = row/UL_CYCLE - 1;
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
			double sta = GetStationMatrix(pGrid, row, nCol);
			double staNext = GetStationMatrix(pGrid, row, nCol+1);
			double staNew = pGir->GetStationByOneStepMid((sta + staNext) / 2, 0);
			SetMatrixByStation(pGrid, row, nCol, staNew);
			SetMatrixByStation(pGrid, row, nCol+1, staNext);
			pGrid->SetTextMatrix(row+UL_THICK, nCol, pGrid->GetTextMatrix(row+UL_THICK, nCol+1));
			pGrid->SetTextMatrix(row+UL_WIDTH, nCol, pGrid->GetTextMatrix(row+UL_WIDTH, nCol+1));
			pGrid->SetTextMatrix(row+UL_SYMBOL, nCol, pGrid->GetTextMatrix(row+UL_SYMBOL, nCol+1),(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			pGrid->SetTextMatrix(row+UL_MOVEDIS, nCol, "0");
		}
	}
	SetGridTitle(pGrid);
	pGrid->SetFocus();
	SetDataSave();
}

void CFactJoinPositionDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	CGridCtrl *pGrid = GetGridCurrentTab();	
	ASSERT(pGrid);

	long nRows = pGrid->GetRowCount();
	long nCols = pGrid->GetColumnCount();	
		
	if(nCols <= 2 || nCol == 0 || nCol == nCols-1) return;

	if(IsActiveWeb(pGrid))	//복부
	{
		for(long row = 1; row < nRows; row+=W_CYCLE)
		{
			if (!(m_bSameApplySameCol || row == nRow))
				continue;
			double dist1 = pGrid->GetTextMatrixDouble(row, nCol);
			double dist2 = pGrid->GetTextMatrixDouble(row, nCol + 1);
			pGrid->SetTextMatrix(row, nCol+1, "%.0f", dist1 + dist2);
			if (!m_bSameApplySameCol)   {
				for (long n = 0; n < W_CYCLE; n++)
					if (row + n != GridView->iRow)
						pGrid->DeleteColumn(nCol, row + n);
			}
		}	
	}
	else	// 상,하부
	{
		for(long row = 1; row < nRows; row+=UL_CYCLE)
		{
			if (!(m_bSameApplySameCol || row == nRow))
				continue;
			double dist1 = pGrid->GetTextMatrixDouble(row, nCol);
			double dist2 = pGrid->GetTextMatrixDouble(row, nCol + 1);
			pGrid->SetTextMatrix(row, nCol+1, "%.0f", dist1 + dist2);
			if (!m_bSameApplySameCol)   {
				for (long n = 0; n < UL_CYCLE; n++)
					if (row + n != GridView->iRow)
						pGrid->DeleteColumn(nCol, row + n);
			}
		}
	}
}

void CFactJoinPositionDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
    int nCol = GridView->iColumn;

	CGridCtrl *pGrid = GetGridCurrentTab();	
	ASSERT(pGrid);

	// 구간 Title 다시 정렬하기
	CString cs;
	for(long c=nCol; c<pGrid->GetColumnCount(); c++)
	{
		cs.Format("%d구간",c);
		pGrid->SetTextMatrix(0,c,cs);
	}
	
	pGrid->SetFocus();	
	SetDataSave();
}

void CFactJoinPositionDlg::OnFactjoinThick() 
// 두께고정
{
	UpdateData(TRUE);
	m_pStd->GetBridge()->m_cFactThickFix = m_cFactThickFix;
}

void CFactJoinPositionDlg::OnMoveByMoment() 
// 이동 1
{
	CString	sText	= _T("");

	m_dDisOnCenterFactJoin.GetWindowText(sText);

	MoveByMomentStress(atof(sText));

	UpdateData();

	BeginWaitCursor();
	DataSave(m_TabWnd->GetCurSel());
	//DataSave(1);
	//DataSave(2);
	
	SetDataInit();
	DrawInputDomyunView(TRUE);
	EndWaitCursor();
}

void CFactJoinPositionDlg::OnMoveByStress() 
// 이동 2
{	
	CString	sText	= _T("");

	m_dDisOnCenterFactJoin.GetWindowText(sText);
	
	MoveByMomentStress(-atof(sText));

	UpdateData();

	BeginWaitCursor();
	DataSave(0);
	DataSave(1);
	DataSave(2);

	SetDataInit();
	DrawInputDomyunView(TRUE);
	EndWaitCursor();
}

void CFactJoinPositionDlg::MoveByMomentStress(double dTerm) 
{	
	CGridCtrl *pGrid = GetGridCurrentTab();	
	ASSERT(pGrid);

	long nG;
	long nGsu = m_pStd->GetBridge()->GetGirdersu();

	if( m_bSameApplySameCol )
	{
		for(nG = -1; nG < nGsu; nG++)
		{
			SetFactTerm(G_F_U, nG,-dTerm);
			SetFactTerm(G_F_L, nG, dTerm);
			SetFactTerm(G_W, nG, 0);
		}
	}
	else
	{
		if(pGrid->GetRowCount() == m_GridW.GetRowCount())
			nG = pGrid->GetFocusCell().row/W_CYCLE-1;
		else
			nG = pGrid->GetFocusCell().row/UL_CYCLE-1;
		
		SetFactTerm(G_F_U, nG, -dTerm);
		SetFactTerm(G_F_L, nG, dTerm);
		SetFactTerm(G_W, nG, 0);
	}

	BeginWaitCursor();
	DrawInputDomyunView(FALSE);
	EndWaitCursor();

	m_GridU.Invalidate();
	m_GridL.Invalidate();
	m_GridW.Invalidate();
}

// 공장이음 이동거리 Setting
void CFactJoinPositionDlg::SetFactTerm(long nMarkBujeFact, long nG, double dBaseTerm)
{
	CGridCtrl *pGrid = GetGridByFactType(nMarkBujeFact);
	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
	long nRow = W_CYCLE * nG + W_CYCLE+1;
	if(nMarkBujeFact != G_W)
		nRow = UL_CYCLE * nG + UL_CYCLE+1;
	long nFact = 1;

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = 0;
	if (nMarkBujeFact == G_F_U)
		BxFlag = BX_UPPERFLANGE;
	if (nMarkBujeFact == G_F_L)
		BxFlag = BX_LOWERFLANGE;
	if (nMarkBujeFact == G_W)
		BxFlag = BX_WEB;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
	while (pBx)
	{
		//if (!pBx->IsState(BX_SPLICE))
		{
			double dTerm = GetDirInKyunggan(nG, pBx->GetStation()) * dBaseTerm;
			double staMid = pGir->GetStationByOneStepMid(pBx->GetStation(), 0);
			double sta = staMid + dTerm;
			SetMatrixByStation(pGrid, nRow, nFact, sta);
			if(nMarkBujeFact == G_W)
				SetMatrixByStation(pGrid, nRow+W_MOVEDIS, nFact, staMid);
			else
				SetMatrixByStation(pGrid, nRow+UL_MOVEDIS, nFact, staMid);

			nFact++;
		}
		pBx = Finder.GetBxNext();
	}

}

void CFactJoinPositionDlg::OnKillfocusFactjoinMovedis()
{
	CString	sText	= _T("");

	UpdateData(TRUE);

	m_dDisOnCenterFactJoin.GetWindowText(sText);

	m_pStd->GetBridge()->m_dDisOnCenterFactJoin = atof(sText);

}

void CFactJoinPositionDlg::SetDataDefault() 
{
	CString	sText	= _T("");

	m_nLengthType		= m_pStd->GetBridge()->m_nLengthTypeOnFact;

	sText.Format("%g",	m_pStd->GetBridge()->m_dDisCenterOnFact);

	m_dDisCenterOnFact.SetWindowText(sText);

	m_pStd->GetBridge()->m_dDisOnCenterFactJoin = 250;

	sText.Format("%g",	m_pStd->GetBridge()->m_dDisOnCenterFactJoin);

	m_dDisOnCenterFactJoin.SetWindowText(sText);

	UpdateData(FALSE);

	BeginWaitCursor();
	for (long nMarkBujeFact = 0; nMarkBujeFact <= 2; nMarkBujeFact++)
	{
		SetDataDefaultByBuje(nMarkBujeFact, 1);
		DataSave(nMarkBujeFact);
	}

	EndWaitCursor();		
}

void CFactJoinPositionDlg::SetDataDefaultByBuje(long nMarkBujeFact, long nAdvice, BOOL isRist)
{
	long nG;
	long nGsu = m_pStd->GetBridge()->GetGirdersu();
	CDoubleArray DArStaTotal; // 공장이음 위치 Station(전체)	

	CGridCtrl *pGrid = GetGridByFactType(nMarkBujeFact);
	if(nMarkBujeFact==G_W)
		pGrid->SetRowCount(W_CYCLE*(nGsu+1)+1);
	else
		pGrid->SetRowCount(UL_CYCLE*(nGsu+1)+1);

	for(nG = -1 ; nG < nGsu ; nG++)
	{
		DArStaTotal.RemoveAll();
		InitializeFact(nMarkBujeFact, nG, DArStaTotal, nAdvice);
		InputGrid(nMarkBujeFact, nG, DArStaTotal, isRist);
	}
	SetGridTitle(pGrid);
	pGrid->DeleteUnusedColumn();
	m_pStd->m_pDoc->SetModifiedFlag(TRUE);
}

//권고안1
void CFactJoinPositionDlg::OnSetDataDefault() 
{
	if(!ContinueMsg()) return;

	BeginWaitCursor();
	long nTab = m_TabWnd->GetCurFocus();
	SetDataDefaultByBuje(nTab, 1);

	DataSave(nTab);
	DrawInputDomyunView(TRUE);
	SetDataInit();
	EndWaitCursor();
}
//권고안2
void CFactJoinPositionDlg::OnSetDataDefaultSecond() 
{
	if(!ContinueMsg()) return;

	BeginWaitCursor();
	long nTab = m_TabWnd->GetCurSel();
	SetDataDefaultByBuje(nTab, 2);

	DataSave(nTab);
	DrawInputDomyunView(TRUE);
	EndWaitCursor();
}
//권고안3
void CFactJoinPositionDlg::OnSetDataDefaultThird() 
{
	if(!ContinueMsg()) return;

	BeginWaitCursor();
	long nTab = m_TabWnd->GetCurSel();
	SetDataDefaultByBuje(nTab, 3);

	DataSave(nTab);
	DrawInputDomyunView(TRUE);
	EndWaitCursor();
}


void CFactJoinPositionDlg::OnFactjoinAutocalcthick() 
{
	AfxMessageBox("현재 이 기능은 보완작업 중 입니다. \n\n 다음 버젼에서 완벽하게 지원하도록 하겠습니다.", MB_OK|MB_ICONINFORMATION);
	return;

}

void CFactJoinPositionDlg::OnFactjoinAutorevision() 
{
	AfxMessageBox("현재 이 기능은 보완작업 중 입니다. \n\n 다음 버젼에서 완벽하게 지원하도록 하겠습니다.", MB_OK|MB_ICONINFORMATION);
	return;	
	

}

void CFactJoinPositionDlg::OnFactjoinUsercalc() 
{

}

void CFactJoinPositionDlg::OnFactjoinChangethick() 
{
	UpdateData(TRUE);
}

void CFactJoinPositionDlg::OnSameApplySameCol() 
{
	UpdateData();

	m_GridU.SetInsertThisOnly(!m_bSameApplySameCol);
	m_GridU.SetDeleteThisOnly(!m_bSameApplySameCol);
	m_GridL.SetInsertThisOnly(!m_bSameApplySameCol);
	m_GridL.SetDeleteThisOnly(!m_bSameApplySameCol);
	m_GridW.SetInsertThisOnly(!m_bSameApplySameCol);
	m_GridW.SetDeleteThisOnly(!m_bSameApplySameCol);
}

void CFactJoinPositionDlg::SetDataSave()
{
	DataSave(m_TabWnd->GetCurSel());


}

void CFactJoinPositionDlg::OnDialogDataSaveAndRedraw()
{
	UpdateData();

	DataSave(m_TabWnd->GetCurSel());
	
	SetDataInit();
	CheckWarn();
	DrawInputDomyunView(TRUE);
}

void CFactJoinPositionDlg::OnDestroy()
{
	CGeneralBaseDlg::OnDestroy();	
	m_pView->GetDomyun()->DeleteCursor();	
}

void CFactJoinPositionDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		

	CDomyun Dom(pDom);	
	if(m_nTab==0) 
	{
		CAPlateDrawPyung Draw(m_pStd->GetDataManage());
		Draw.DrawPyung(&Dom, 5, m_TabWnd->GetCurSel()==1 ? FALSE : TRUE);	
	}
	else
	{
		CAPlateDrawJong Draw(m_pStd->GetDataManage());
		Draw.DrawJongGirderTotal(&Dom);
	}

	*pDom << Dom;	

//	if(m_nTab == 0)
		RowColChangeFactjoinGrid(nCurMark, 0);

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CFactJoinPositionDlg::RowColChangeFactjoinGrid(long nMarkBujeFact, NMHDR* nmgv)
{	
	if(nMarkBujeFact < 0) return;
	if(nMarkBujeFact==G_W && m_nTab!=1)	return;
	if(nMarkBujeFact!=G_W && m_nTab==1)	return;
	
	m_pView->GetDomyun()->ClearEtt(FALSE);

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow, nCol;
	if(GridView)
	{
		nRow = GridView->iRow;
		nCol = GridView->iColumn;
	}
	else
	{
		nRow = GetGridCurrentTab()->GetFocusCell().row;
		nCol = GetGridCurrentTab()->GetFocusCell().col;
	}

	CGridCtrl *pGrid = GetGridByFactType(nMarkBujeFact);
	long nCurGir = (nRow-1)/W_CYCLE-1;
	if(nMarkBujeFact != G_W)
		nCurGir = (nRow-1)/UL_CYCLE-1;

	CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nCurGir);

	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER;
	if(nMarkBujeFact == G_F_U) // 상부 
		BxFlag |= BX_UPPERFLANGE;
	else if(nMarkBujeFact == G_F_L) // 하부
		BxFlag |= BX_LOWERFLANGE;
	else // 복부판
		BxFlag |= BX_WEB;

	CPlateBxFinder Finder(pGir);
	Finder.GetBxFirst(BxFlag);
	long nFactsu = Finder.GetCountBx() - 1;

	if (nCol > nFactsu) return;

	nRow = (nCurGir+1)*W_CYCLE+1;
	if(nMarkBujeFact!=G_W)
		nRow = (nCurGir+1)*UL_CYCLE+1;

	double presta, sta;
	presta = GetStationMatrix(pGrid, nRow, nCol - 1);
	sta    = GetStationMatrix(pGrid, nRow, nCol);

	double width = 0;
	if (nMarkBujeFact == G_F_L)
		width = pGir->GetWidthOnStation(sta, FALSE) + 200;
	
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(FALSE);
	pDom->FixEtt(TRUE);

	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CAPlateDrawJong DrawJong(m_pStd->GetDataManage());

	if(m_bSameApplySameCol)
	{		
		pDom->DeleteCursor();
		pDom->MakeCursor(0,0,0);
		for(long nG=-1;nG<m_pStd->GetBridge()->GetGirdersu();nG++)
		{
			if(nMarkBujeFact != G_W && nG==-1)	continue;
			long row = (nG+1)*W_CYCLE+1;
			if(nMarkBujeFact!=G_W)
				row = (nG+1)*UL_CYCLE+1;
			presta = GetStationMatrix(pGrid, row, nCol-1);
			sta = GetStationMatrix(pGrid, row, nCol);
			if(nMarkBujeFact != G_W)	Draw.SetCursorPosition(nG, presta, sta, pDom,TRUE, 40, 2, 0);
			else						DrawJong.SetCursorPosition(nG, presta, sta, pDom,TRUE, 40, 2, 0);
		}
	}
	else
	{
		if(nMarkBujeFact != G_W)	Draw.SetCursorPosition(nCurGir, presta, sta, pDom,TRUE, 40, 2, 0);	
		else						DrawJong.SetCursorPosition(nCurGir, presta, sta, pDom,TRUE, 40, 2, 0);
	}

	nCurMark = nMarkBujeFact;
}

void CFactJoinPositionDlg::OnCellChangedUpper(NMHDR* nmgv, LRESULT*)
{	
	if(m_nTab == 0)
		RowColChangeFactjoinGrid(G_F_U, nmgv);	

	// 플랜지 자유돌출폭에 공장이음두께 검토
	long nG = SelectedGirder(m_GridU.GetFocusCell().row, 5);

	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
	CPlateFactChain *pFactChain = pGir->GetFactChain(G_F_U, m_GridU.GetFocusCell().col-1);
	if(pFactChain==NULL)	return;
	
	double dFlSharp = pGir->GetLengthSharpOnBx(pFactChain->GetBx(), TRUE);

	FlangeThickCheck(pFactChain, dFlSharp);
}

void CFactJoinPositionDlg::OnCellChangedLower(NMHDR* nmgv, LRESULT*)
{
	if(m_nTab == 0)
		RowColChangeFactjoinGrid(G_F_L, nmgv);	

	// 플랜지 자유돌출폭에 공장이음두께 검토
	long nG = SelectedGirder(m_GridL.GetFocusCell().row, 5);

	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
	CPlateFactChain *pFactChain = pGir->GetFactChain(G_F_L, m_GridL.GetFocusCell().col-1);
	if(pFactChain==NULL)	return;
	
	double dFlSharp = pGir->GetLengthSharpOnBx(pFactChain->GetBx(), FALSE);
	FlangeThickCheck(pFactChain, dFlSharp);
}

void CFactJoinPositionDlg::OnCellChangedWeb(NMHDR* nmgv, LRESULT*)
{
	if(m_nTab == 1)
		RowColChangeFactjoinGrid(G_W, nmgv);
	CCellID next	= m_GridW.GetFocusCell();
	long	nRow	= 1;
	long	nG		= SelectedGirder(m_GridW.GetFocusCell().row, 4);
	
	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);

	double	dSttSta = pGir->GetSSS(OFF_STT_GIRDER);
	double	dEndSta	= 0;

	for(long nCol=1; nCol<next.col;nCol++)
	{
		dSttSta += m_GridW.GetTextMatrixDouble(nRow, nCol);
	}

	dEndSta = dSttSta + m_GridW.GetTextMatrixDouble(nRow, next.col);
	WebThickCheck(nG, dSttSta, dEndSta);
}

//기능 : [자동 대칭]
void CFactJoinPositionDlg::OnSetDataSymmetry()
{			
	CGridCtrl *pGrid = GetGridCurrentTab();
	ASSERT(pGrid);
	
	long nG = pGrid->GetFocusCell().row/UL_CYCLE-1;
	if(IsActiveWeb(pGrid))
		nG = pGrid->GetFocusCell().row/W_CYCLE-1;

	long nGsu = m_pStd->GetBridge()->GetGirdersu();
	long nTab = m_TabWnd->GetCurSel();
	DataSave(nTab);

	if(ContinueMsg() == FALSE ) return;
	if(m_bSameApplySameCol)
		for(nG = -1 ; nG < nGsu ; nG++)
			SetDataSymmetryByBuje(nTab, nG);
	else
		SetDataSymmetryByBuje(nTab, nG);

	pGrid->DeleteUnusedColumn();
	SetGridTitle(pGrid);	
	pGrid->Invalidate();
	

	BeginWaitCursor();
	DataSave(nTab);	
	DrawInputDomyunView(FALSE);
	EndWaitCursor();		
}

void CFactJoinPositionDlg::SetDataSymmetryByBuje(long nMarkBujeFact, long nG)
{
	CGridCtrl *pGrid = GetGridByFactType(nMarkBujeFact);
	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);	

	long nCols = pGrid->GetCols();
	long nRow = W_CYCLE * nG + W_CYCLE+1;
	if(nMarkBujeFact != G_W)
		nRow = UL_CYCLE * nG + UL_CYCLE+1;

	long nPier=1;
	double sta, staMid, staSym;
	double SttSta = pGir->GetSSS(OFF_STT_GIRDER);
	double EndSta = pGir->GetSSS(OFF_END_GIRDER);
	double HalfSta = (SttSta + EndSta) / 2.0;  // 교량 교대부 사이 중간 == 대칭점(기준점)
	CDoubleArray StaArr;	
	long nCenCol = 0;
	for (nCenCol = 1; nCenCol < nCols; nCenCol++)
	{
		sta = GetStationMatrix(pGrid, nRow, nCenCol);
		if(sta > pGir->GetBxOnJijum(nPier)->GetStation())
		{						
			if(nPier >= m_pStd->GetBridge()->m_nQtyJigan) break;
			SttSta = pGir->GetBxOnJijum(nPier)->GetStation();
			EndSta = pGir->GetBxOnJijum(m_pStd->GetBridge()->m_nQtyJigan-nPier)->GetStation();
			HalfSta = (SttSta + EndSta) / 2.0;
			++nPier;
		}
		if (sta == HalfSta)   {  // 이런 경우는 진짜 드물거야...
			nCenCol++;
			break;
		}
		if (sta > HalfSta)
			break;
		staMid = pGir->GetStationByOneStepMid(sta, 0);
		staSym = pGir->GetStationByOneStepMid(HalfSta + (HalfSta - staMid), 0);  // 이동전의 상태임.
		staSym -= sta - staMid;  // 이동량 보정.
		StaArr.Add(staSym);
	}
	long nNewCols = nCenCol + StaArr.GetSize() + 1;
	//if (pGrid->GetCols() < nNewCols)
		pGrid->SetColumnCount(nNewCols);

	long nSymCol = StaArr.GetSize();
	for ( ; nCenCol < nNewCols - 1; nCenCol++)
	{
		staSym = StaArr.GetAt(nSymCol - 1);
		SetMatrixByStation(pGrid, nRow, nCenCol, staSym);
		if(nMarkBujeFact == G_W)
			SetMatrixByStation(pGrid, nRow+W_MOVEDIS, nCenCol, pGir->GetStationByOneStepMid(staSym, 0));
		else
			SetMatrixByStation(pGrid, nRow+UL_MOVEDIS, nCenCol, pGir->GetStationByOneStepMid(staSym, 0));

		pGrid->SetTextMatrix(nRow+UL_THICK, nCenCol + 1, pGrid->GetItemText(nRow+UL_THICK, nSymCol));
		pGrid->SetTextMatrix(nRow+UL_WIDTH, nCenCol + 1, pGrid->GetItemText(nRow+UL_WIDTH, nSymCol));
		pGrid->SetTextMatrix(nRow+UL_SYMBOL, nCenCol + 1, pGrid->GetItemText(nRow+UL_SYMBOL, nSymCol));
		
		nSymCol--;
	}
	// 마지막 구간 저장
	SetMatrixByStation(pGrid, nRow, nCenCol, pGir->GetSSS(OFF_END_GIRDER));
	
	if(nMarkBujeFact == G_W)
		pGrid->SetTextMatrix(nRow+W_MOVEDIS, nCenCol, "%.0f", 0.0);
	else
		pGrid->SetTextMatrix(nRow+UL_MOVEDIS, nCenCol, "%.0f", 0.0);

	// 뒤에 남은 Data 삭제.
	for (nCenCol++; nCenCol < pGrid->GetCols(); nCenCol++)
	{
		pGrid->SetTextMatrix (nRow+UL_LEN, nCenCol, "");
		pGrid->SetTextMatrix (nRow+UL_THICK, nCenCol, "");
		pGrid->SetTextMatrix (nRow+UL_WIDTH, nCenCol, "");
		pGrid->SetTextMatrix (nRow+UL_SYMBOL, nCenCol, "");
		if(nMarkBujeFact != G_W)
			pGrid->SetTextMatrix (nRow+UL_MOVEDIS, nCenCol, "");
		
	}
}


//기능 : [모든 거더 동일적용]
void CFactJoinPositionDlg::OnFactjoinAllequal() 
{	
	SetFactEqual(m_TabWnd->GetCurSel());
	DataSave(m_TabWnd->GetCurSel());	
	DrawInputDomyunView(FALSE);
}


//기능 : [모든 거더 동일적용]-상.하.복부판
void CFactJoinPositionDlg::SetFactEqual(long nMarkBujeFact)
{
	CGridCtrl *pGrid = GetGridByFactType(nMarkBujeFact);
	long nCurRow = pGrid->GetFocusCell().row;
	if(nCurRow<=0) 
		nCurRow = 1;
	if(nMarkBujeFact==G_W)
		nCurRow -= (nCurRow - 1) % W_CYCLE;
	else
		nCurRow -= (nCurRow - 1) % UL_CYCLE;


	int nGirNum = nMarkBujeFact==G_W ? nCurRow/W_CYCLE : nCurRow/UL_CYCLE;
	CString str;
	str.Format("  %d번 거더 기준으로 공장이음을 배치합니다. \n    계속하시겠습니까?", nGirNum);
	if (IDCANCEL == MessageBox(str, "모든 거더 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	long nRows = pGrid->GetRows();
	long nCols = pGrid->GetCols();
	long nEndCol = pGrid->GetValidCol(nCurRow, FALSE);
	CPlateGirderApp *pCurGir = m_pStd->GetBridge()->GetGirder(nCurRow/W_CYCLE-1);
	if(nMarkBujeFact != G_W)
		pCurGir = m_pStd->GetBridge()->GetGirder(nCurRow/UL_CYCLE-1);

	if(nMarkBujeFact==G_W)
	{
		for (long nRow = 1; nRow < nRows; nRow += W_CYCLE)   {
			if (nRow == nCurRow)
				continue;
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nRow/W_CYCLE-1);
			long nJijum = 0;
			double staCurStt = pCurGir->GetSSS(OFF_STT_GIRDER);
			double staCurEnd = staCurStt;
			double staStt = pGir->GetSSS(OFF_STT_GIRDER);
			double staEnd = staStt;
			long nCol = 0;
			for (nCol = 1; nCol < nEndCol; nCol++)
			{
				double staCur = GetStationMatrix(pGrid, nCurRow, nCol);
				double staCurMid = pCurGir->GetStationByOneStepMid(staCur, 0);
				if (staCur > staCurEnd)   {
					nJijum++;
					staCurStt = staCurEnd;
					staCurEnd = pCurGir->GetStationOnJijum(nJijum);
					staStt = staEnd;
					staEnd = pGir->GetStationOnJijum(nJijum);
				}
				double staMid = (staCurMid - staCurStt) / (staCurEnd - staCurStt) * (staEnd - staStt) + staStt;
				staMid = pGir->GetStationByOneStepMid(staMid, 0);
				double sta = staMid + (staCur - staCurMid);
				SetMatrixByStation(pGrid, nRow, nCol, sta);
				SetMatrixByStation(pGrid, nRow+W_MOVEDIS, nCol, staMid);
				pGrid->SetTextMatrix(nRow+W_THICK, nCol, pGrid->GetTextMatrix(nCurRow+W_THICK, nCol));
				pGrid->SetTextMatrix(nRow+W_SYMBOL, nCol, pGrid->GetTextMatrix(nCurRow+W_SYMBOL, nCol),(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			}
			SetMatrixByStation(pGrid, nRow, nCol, pGir->GetSSS(OFF_END_GIRDER));
			pGrid->SetTextMatrix(nRow+W_MOVEDIS, nCol, "0");
			pGrid->SetTextMatrix(nRow+W_THICK, nCol, pGrid->GetTextMatrix(nCurRow+W_THICK, nCol));
			pGrid->SetTextMatrix(nRow+W_SYMBOL, nCol, pGrid->GetTextMatrix(nCurRow+W_SYMBOL, nCol),(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			for (nCol++ ; nCol < nCols; nCol++)   
			{
				pGrid->SetTextMatrix(nRow+W_LEN, nCol, "");
				pGrid->SetTextMatrix(nRow+W_THICK, nCol, "");
				pGrid->SetTextMatrix(nRow+W_SYMBOL, nCol, "",(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
				pGrid->SetTextMatrix(nRow+W_MOVEDIS, nCol, "");
			}
		}
	}
	else
	{
		for (long nRow = 1; nRow < nRows; nRow += UL_CYCLE)   {
			if (nRow == nCurRow)
				continue;
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nRow/UL_CYCLE-1);
			long nJijum = 0;
			double staCurStt = pCurGir->GetSSS(OFF_STT_GIRDER);
			double staCurEnd = staCurStt;
			double staStt = pGir->GetSSS(OFF_STT_GIRDER);
			double staEnd = staStt;
			long nCol = 0;
			for (nCol = 1; nCol < nEndCol; nCol++)
			{
				double staCur = GetStationMatrix(pGrid, nCurRow, nCol);
				double staCurMid = pCurGir->GetStationByOneStepMid(staCur, 0);
				if (staCur > staCurEnd)   {
					nJijum++;
					staCurStt = staCurEnd;
					staCurEnd = pCurGir->GetStationOnJijum(nJijum);
					staStt = staEnd;
					staEnd = pGir->GetStationOnJijum(nJijum);
				}
				double staMid = (staCurMid - staCurStt) / (staCurEnd - staCurStt) * (staEnd - staStt) + staStt;
				staMid = pGir->GetStationByOneStepMid(staMid, 0);
				double sta = staMid + (staCur - staCurMid);
				SetMatrixByStation(pGrid, nRow+UL_LEN, nCol, sta);
				SetMatrixByStation(pGrid, nRow+UL_MOVEDIS, nCol, staMid);
				pGrid->SetTextMatrix(nRow+UL_THICK, nCol, pGrid->GetTextMatrix(nCurRow+UL_THICK, nCol));
				pGrid->SetTextMatrix(nRow+UL_WIDTH, nCol, pGrid->GetTextMatrix(nCurRow+UL_WIDTH, nCol));
				pGrid->SetTextMatrix(nRow+UL_SYMBOL, nCol, pGrid->GetTextMatrix(nCurRow+UL_SYMBOL, nCol),(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			}
			SetMatrixByStation(pGrid, nRow, nCol, pGir->GetSSS(OFF_END_GIRDER));
			pGrid->SetTextMatrix(nRow+UL_MOVEDIS, nCol, "0");
			pGrid->SetTextMatrix(nRow+UL_THICK, nCol, pGrid->GetTextMatrix(nCurRow+UL_THICK, nCol));
			pGrid->SetTextMatrix(nRow+UL_WIDTH, nCol, pGrid->GetTextMatrix(nCurRow+UL_WIDTH, nCol));
			pGrid->SetTextMatrix(nRow+UL_SYMBOL, nCol, pGrid->GetTextMatrix(nCurRow+UL_SYMBOL, nCol),(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			nCol++;
			for ( ; nCol < nCols; nCol++)   
			{
				pGrid->SetTextMatrix(nRow+UL_LEN, nCol, "");
				pGrid->SetTextMatrix(nRow+UL_THICK, nCol, "");
				pGrid->SetTextMatrix(nRow+UL_WIDTH, nCol, "");
				pGrid->SetTextMatrix(nRow+UL_SYMBOL, nCol, "",(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
				pGrid->SetTextMatrix(nRow+UL_MOVEDIS, nCol, "");
			}
		}
	}

	pGrid->DeleteUnusedColumn();
	SetGridTitle(pGrid);
	DataSave(m_TabWnd->GetCurSel());	
	DrawInputDomyunView(FALSE);
}


///////////////////////////////////////////////////////////////////
// Dialog 권고안을 위한 함수 II  2001.05.11
void CFactJoinPositionDlg::InitializeFact(long nMarkBujeFact, long nG, CDoubleArray &DArStaTotal, long nAdvice)
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(nG);

	if (nMarkBujeFact == G_F_U || nMarkBujeFact == G_F_L)  // 상하판
	{
		CPlateBxFinder Finder(pGir);
		__int64 BxFlag = 0;
		if (nMarkBujeFact == G_F_U)
			BxFlag = BX_SPLICE;
		if (nMarkBujeFact == G_F_L)
			BxFlag = BX_UPPERFLANGE;

		if(!pDB->IsTUGir())
		{
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
			while (pBx)
			{
				if (nMarkBujeFact == G_F_U || !pBx->IsState(BX_SPLICE))
				{
					double staBase = pBx->GetStation();
					long nMove = GetDirInKyunggan(nG, staBase) * nAdvice;
					if (nMarkBujeFact == G_F_L && nAdvice == 3)
						nMove = 0;
					double staFact = pGir->GetStationByOneStepMid(staBase, nMove);
					DArStaTotal.Add(staFact);
				}
				pBx = Finder.GetBxNext();
			}
		}
		else
		{
// 6월 10일 발표용 TU 권고안 (by. 이종원부장님)
			CPlateBasicIndex *pBxEnd	= Finder.GetBxLast(BxFlag);
			CPlateBasicIndex *pBxStt	= Finder.GetBxFirst(BxFlag);
			CPlateBasicIndex *pBx		= pBxStt;
			
			while (pBx)
			{
				if (nMarkBujeFact == G_F_U || nMarkBujeFact == G_F_L || !pBx->IsState(BX_SPLICE))
				{
					double staBase = pBx->GetStation();
					long nMove = GetDirInKyunggan(nG, staBase) * nAdvice;
					if (nMarkBujeFact == G_F_L && nAdvice == 3)
						nMove = 0;					
					double staFact = pGir->GetStationByOneStepMid(staBase, 0);

					if((pBx!=pBxStt && Finder.GetBxFixPrev() == pBxStt) || (pBx!=pBxEnd && Finder.GetBxFixNext() == pBxEnd))
						staFact -= nMove*1000;
					DArStaTotal.Add(staFact);
				}
				pBx = Finder.GetBxNext();
			}
		}
		
	}
	else  // 복부판 공장이음
	{
		if (nAdvice > 1)   {
			for (long nK = 0; nK < m_pStd->GetBridge()->m_nQtyJigan - 1; nK++)
			{
				double staBase = pGir->GetStationOnJijum(nK+1);
				double staStt = pGir->GetStationByOneStepMid(staBase - 4375, 0);
				double staEnd = pGir->GetStationByOneStepMid(staBase + 4375, 0);
				if (nAdvice == 2)   {
					staStt = GetOptimizeSta(nG, staStt,  1, BX_SPLICE | BX_UPPERFLANGE | BX_LOWERFLANGE);
					staEnd = GetOptimizeSta(nG, staEnd, -1, BX_SPLICE | BX_UPPERFLANGE | BX_LOWERFLANGE);
				}
				if (staStt < staBase && staEnd > staBase)
				{
					DArStaTotal.Add(staStt);
					DArStaTotal.Add(staEnd);
				}
			}
		}
	}
	DArStaTotal.Add(pGir->GetSSS(OFF_END_GIRDER));

}

// 현재 스테이션에 위치한 경간에서 최대 정모멘트 기준으로 앞뒤 여부를 구함.
// -1 : 앞쪽,  1 : 뒤쪽
long CFactJoinPositionDlg::GetDirInKyunggan(long nG, double sta)
{
	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
	long nK = pGir->GetNumberJiganByStation(sta);
	double staStt = pGir->GetStationOnJijum(nK);
	double staEnd = pGir->GetStationOnJijum(nK+1);
	double Ratio = 0.5;
	if (m_pStd->GetBridge()->m_nQtyJigan > 1)   {
		if (nK == 0)  // 첫경간
			Ratio = 3.0/7.0;
		if (nK+1 == m_pStd->GetBridge()->m_nQtyJigan)  // 끝경간
			Ratio = 4.0/7.0;
	}
	double staMax = staStt + (staEnd - staStt) * Ratio;
	if (sta < staMax)
		return -1;
	return 1;
}


// 기존 현장이음과 공장이음을 검사하여 중복되지않는 단면사이 구간의 Station을 리턴
// Sta		: 오리지날 Sta
// nDir		: -1 -> 앞쪽으로 찾음
//			   1 -> 뒤쪽으로 찾음
//			   0 -> 앞이나 뒤쪽중 가까운 쪽으로 찾음
// BxFlag	: 중복 피할 종류 지정
double CFactJoinPositionDlg::GetOptimizeSta(long nG, double sta, long nDir, __int64 BxFlag)
{
	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
	double staNext;
	sta = pGir->GetStationByOneStepMid(sta, 0);

	if (nDir != 0)
	{
		while (pGir->IsBxObjectInMidByStation(sta, BxFlag))   {
			staNext = pGir->GetStationByOneStepMid(sta, nDir);
			if (sta == staNext)  // 거더 시종점인 경우
				break;
			sta = staNext;
		}
		return sta;
	}

	double staStt = sta, staEnd = sta;
	while (pGir->IsBxObjectInMidByStation(staStt, BxFlag))   {
		staNext = pGir->GetStationByOneStepMid(staStt, -1);
		if (staStt == staNext)  // 거더 시점인 경우
			break;
		staStt = staNext;
	}
	while (pGir->IsBxObjectInMidByStation(staEnd, BxFlag))   {
		staNext = pGir->GetStationByOneStepMid(staEnd, 1);
		if (staEnd == staNext)  // 거더 종점인 경우
			break;
		staEnd = staNext;
	}

	if (sta - staStt < staEnd - sta)
		return staStt;
	return staEnd;
}

void CFactJoinPositionDlg::DataSave(long nMarkBujeFact /* = -1*/)
{	
	CString	sText	= _T("");

	UpdateData(TRUE);

	m_pStd->GetBridge()->m_nLengthTypeOnFact = m_nLengthType;

	m_dDisCenterOnFact.GetWindowText(sText);
	m_pStd->GetBridge()->m_dDisCenterOnFact = atof(sText);

	m_pStd->GetBridge()->m_cSameApplySameColFactJoin = (BYTE)m_bSameApplySameCol;
	
	m_dDisOnCenterFactJoin.GetWindowText(sText);
	m_pStd->GetBridge()->m_dDisOnCenterFactJoin = atof(sText);

	m_dFlangeSameWidth.GetWindowText(sText);
	m_pStd->GetBridge()->m_dSameFlangeWidth     = atof(sText);

	CPlateGirderApp* pGir;
	long nG;

	/////////////////////////////////////////////////////////////
	// 상판
	long nCols = m_GridU.GetColumnCount();
	long nFactsu = 0;
	double length=0, thick=0, width=0;
	CString strSymbol;

	if(nMarkBujeFact == -1 || nMarkBujeFact == G_F_U)
	{
		for(nG = -1; nG < m_pStd->GetBridge()->GetGirdersu(); nG++)
		{
			pGir = m_pStd->GetBridge()->GetGirder(nG);
			nFactsu = 0;
			long nCol = 0;
			for(nCol = 1; nCol < nCols; nCol++)
			{
				if(atof(m_GridU.GetItemText(nG*UL_CYCLE+UL_CYCLE+1, nCol)) > 0)	
					nFactsu++;
			}
			
			pGir->MakeFactChain(nFactsu,G_F_U);
			for(nCol = 1; nCol < nFactsu+1; nCol++)
			{
				length = atof(m_GridU.GetItemText(nG*UL_CYCLE+UL_CYCLE+1+UL_LEN, nCol));
				thick  = atof(m_GridU.GetItemText(nG*UL_CYCLE+UL_CYCLE+1+UL_THICK, nCol));
				width  = atof(m_GridU.GetItemText(nG*UL_CYCLE+UL_CYCLE+1+UL_WIDTH, nCol));
				strSymbol  = m_GridU.GetTextMatrix(nG*UL_CYCLE+UL_CYCLE+1+UL_SYMBOL, nCol);
				if(length > 0)
				{
					CPlateFactChain *pFact = pGir->GetFactChain(G_F_U, nCol-1);
					pFact->m_dFactChainThick = thick;
					pFact->m_dWidth          = width;
					pFact->m_strSymbol       = strSymbol;
				}
			}

		}
		SetBxFact(G_F_U);
	}



	//////////////////////////////////////////////////////////
	// 하판
	if(nMarkBujeFact == -1 || nMarkBujeFact == G_F_L)
	{
		nCols = m_GridL.GetColumnCount();
		for(nG = -1; nG < m_pStd->GetBridge()->GetGirdersu(); nG++)
		{
			pGir = m_pStd->GetBridge()->GetGirder(nG);
			nFactsu = 0;
			long nCol = 0;
			for(nCol = 1; nCol < nCols; nCol++)
			{
				if(atof(m_GridL.GetItemText(nG*UL_CYCLE+UL_CYCLE+1, nCol)) > 0)	
					nFactsu++;
			}

			pGir->MakeFactChain(nFactsu,G_F_L);
			for(nCol = 1; nCol< nFactsu+1; nCol++)
			{
				length = atof(m_GridL.GetItemText(nG*UL_CYCLE+UL_CYCLE+1+UL_LEN, nCol));
				thick  = atof(m_GridL.GetItemText(nG*UL_CYCLE+UL_CYCLE+1+UL_THICK, nCol));
				width  = atof(m_GridL.GetItemText(nG*UL_CYCLE+UL_CYCLE+1+UL_WIDTH, nCol));
				strSymbol  = m_GridL.GetTextMatrix(nG*UL_CYCLE+UL_CYCLE+1+UL_SYMBOL, nCol);
				if(length > 0)
				{
					CPlateFactChain *pFact = pGir->GetFactChain(G_F_L, nCol-1);
					pFact->m_dFactChainThick = thick;
					pFact->m_dWidth          = width;
					pFact->m_strSymbol       = strSymbol;
				}
			}
		}
		SetBxFact(G_F_L);
	}


	/////////////////////////////////////////////////////////////
	// 복부판
	if(nMarkBujeFact == -1 || nMarkBujeFact == G_W)
	{
		nCols = m_GridW.GetColumnCount();
		for(nG = -1; nG < m_pStd->GetBridge()->GetGirdersu(); nG++)
		{
			pGir = m_pStd->GetBridge()->GetGirder(nG);
			nFactsu = 0;
			long nCol = 0;
			for(nCol = 1; nCol < nCols; nCol++)
				if(atof(m_GridW.GetItemText(nG*4+5, nCol)) > 0)	nFactsu++;


			pGir->MakeFactChain(nFactsu,G_W);
			double length, thick;
			for(nCol = 1; nCol < nFactsu+1; nCol++)
			{
				length = atof(m_GridW.GetItemText(nG*W_CYCLE+W_CYCLE+1+W_LEN, nCol));
				thick  = atof(m_GridW.GetItemText(nG*W_CYCLE+W_CYCLE+1+W_THICK, nCol));
				strSymbol  = m_GridW.GetTextMatrix(nG*W_CYCLE+W_CYCLE+1+W_SYMBOL, nCol);
				if(length > 0)
				{
					CPlateFactChain *pFact = pGir->GetFactChain(G_W, nCol-1);
					pFact->m_dFactChainThick = thick;
					pFact->m_strSymbol       = strSymbol;
				}
			}
		}
		SetBxFact(G_W);
	}

	m_pStd->m_pDoc->SetModifiedFlag(TRUE);


	for(nG = -1; nG < m_pStd->GetBridge()->GetGirdersu(); nG++)
	{
		m_pStd->GetBridge()->GetGirder(nG)->SetPOSAndEachPointer();
	}
}

void CFactJoinPositionDlg::SetBxFact(long nMarkBujeFact)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPtrList NList;
	CGridCtrl* pGrid;
	long nLinkBx       = 0;
	__int64	bxFlag     = 0;
	switch( nMarkBujeFact )
	{
	case 0 :
			pGrid = &m_GridU; 
			bxFlag = BX_UPPERFLANGE; 
			nLinkBx = CPlateBxObject::LK_FACTCHAIN_UPPER; 
			break;
	case 1 :
			pGrid = &m_GridL; 
			bxFlag = BX_LOWERFLANGE;  
			nLinkBx = CPlateBxObject::LK_FACTCHAIN_LOWER; 
			break;
	case 2 :
			pGrid = &m_GridW; 
			bxFlag = BX_WEB;  
			nLinkBx = CPlateBxObject::LK_FACTCHAIN_WEB; 
			break;	
	}

	long nCols = pGrid->GetColumnCount()-1;	// 그리드 타이틀 제외
	long nG = 0;
	long nRow = 0;
	for(nG = -1, nRow=1; nG < pDB->GetGirdersu(); nG++)		
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		long nJointsu = 0;
		double dis = 0;	
		double sta = pDB->GetGirder(nG)->GetSSS(OFF_STT_GIRDER);		

		for(long nCol = 1; nCol < nCols; nCol++)
		{
			if(pGrid->GetTextMatrixDouble(nRow, nCol) > 0)
			{
				nJointsu++;
				dis  = pGrid->GetTextMatrixDouble(nRow, nCol);
				if(dis <= 0)	continue;
				sta += dis;
				
				CPlateBasicIndex* p = pGir->newBx();
				p->SetState(bxFlag,TRUE);
				p->SetStation(sta);
				NList.AddTail(p);
			}
		}
		CDWordArray LKExceptionArr;
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_UPPER);
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_LOWER);
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_WEB);
		pGir->PressBxList(NList, bxFlag, &LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);	
		// nCol+1을 한 이유는 FactChain은 구간이기 때문에
		pGir->MakeFactChain(nCols, nMarkBujeFact);	
		pGir->SetLKPOS(nLinkBx);
		// 기본값을 설정해줌
		for(long n=0; n<nCols; n++)
		{
//			if(pDB->GetBridgeType()==PLATE_GIR)
//			{
//				pGir->GetFactChain(nMarkBujeFact,n)->m_dFactChainThick = 12;  
//				pGir->GetFactChain(nMarkBujeFact,n)->m_dWidth		   = 400;
//			}
//			else
			{
				if(pDB->IsTUGir() && nMarkBujeFact==G_F_U)
				{
					pGir->GetFactChain(nMarkBujeFact,n)->m_dFactChainThick = 24;  
					pGir->GetFactChain(nMarkBujeFact,n)->m_dWidth		   = 300;
				}
				else
				{
					pGir->GetFactChain(nMarkBujeFact,n)->m_dFactChainThick = 24;  
					pGir->GetFactChain(nMarkBujeFact,n)->m_dWidth		   = 800;
				}
			}
		}

		if(IsActiveWeb(pGrid))  nRow+=W_CYCLE;
		else				  nRow+=UL_CYCLE;
	}	

	// FactChain을 설정	
	for(nG=-1, nRow=1; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);		
		for(long nCol=1; nCol<pGrid->GetColumnCount(); nCol++)
		{		
			if(nMarkBujeFact==2)
			{
				pGir->GetFactChain(nMarkBujeFact,nCol-1)->m_dFactChainThick = pGrid->GetTextMatrixDouble(nRow+1,nCol);				
			}
			else	// 길이는 Bx에서 설정함.
			{
				pGir->GetFactChain(nMarkBujeFact,nCol-1)->m_dFactChainThick    = pGrid->GetTextMatrixDouble(nRow+1,nCol);// 내측판 두께
				pGir->GetFactChain(nMarkBujeFact,nCol-1)->m_dWidth             = pGrid->GetTextMatrixDouble(nRow+2,nCol);// 내측판 폭
			}
		}						
		if(IsActiveWeb(pGrid))  nRow+=W_CYCLE;
		else				  nRow+=UL_CYCLE;
	}	

	
	m_pStd->m_pDoc->SetModifiedFlag(TRUE);

}

int CFactJoinPositionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGeneralBaseDlg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rectClient, TabRect;
	GetClientRect(rectClient);
	rectClient.left		+= 0;
	rectClient.right	-= 140;
	rectClient.top		+= 25;
	rectClient.bottom	-= 60;
	m_TabWnd = new CXTTabCtrl;
	if (!m_TabWnd->Create (WS_CHILD | WS_VISIBLE, 
		rectClient, this, IDC_FACTJOING_TAB))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	m_TabWnd->GetClientRect(&TabRect);

	m_GridU.Create(TabRect, m_TabWnd, IDC_FACTJOIN_UFLANGE_GRID, WS_CHILD|WS_VISIBLE);
	m_GridL.Create(TabRect, m_TabWnd, IDC_FACTJOIN_LFLANGE_GRID, WS_CHILD|WS_VISIBLE);
	m_GridW.Create(TabRect, m_TabWnd, IDC_FACTJOIN_WEB_GRID, WS_CHILD|WS_VISIBLE);

	m_TabWnd->AddControl("상판", &m_GridU);
	m_TabWnd->AddControl("하판", &m_GridL);
	m_TabWnd->AddControl("복부판공장이음", &m_GridW);

	return 0;
}

CGridCtrl *CFactJoinPositionDlg::GetGridCurrentTab()
{
	switch(m_TabWnd->GetCurSel()) 
	{
	case 0:	return &m_GridU;
	case 1:	return &m_GridL;
	case 2:	return &m_GridW;
	}
	return NULL;
}

void CFactJoinPositionDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	CGridCtrl *pGrid = GetGridCurrentTab();
	ASSERT(*pGrid);
	long nRows = pGrid->GetRowCount();

	if(IsActiveWeb(pGrid))		// 복부
	{
		if (nRow % W_CYCLE == 1+W_LEN)  
		{  // 길이
			double staNew = GetStationMatrix(pGrid, nRow, nCol);
			pGrid->SetTextMatrix(nRow, nCol, pGrid->GetLastChangeData());
			double staDif = staNew - GetStationMatrix(pGrid, nRow, nCol);

			for (long r = 1; r < nRows; r+=W_CYCLE)   
			{
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				pGrid->SetTextMatrix(r, nCol, pGrid->GetLastChangeData());
				double sta = GetStationMatrix(pGrid, r, nCol) + staDif;
				double staNext = GetStationMatrix(pGrid, r, nCol+1);
				SetMatrixByStation(pGrid, r, nCol, sta);
				SetMatrixByStation(pGrid, r, nCol+1, staNext);
				CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(r/W_CYCLE-1);
				SetMatrixByStation(pGrid, r+W_MOVEDIS, nCol, pGir->GetStationByOneStepMid(sta, 0));
			}
		}
		if (nRow % W_CYCLE == 1+W_THICK)   
		{  // 두께
			double thickNew = pGrid->GetTextMatrixDouble(nRow,nCol);
			pGrid->SetTextMatrix(nRow, nCol, pGrid->GetLastChangeData());
			double thickDif = thickNew - pGrid->GetTextMatrixDouble(nRow,nCol);

			for (long r = 1+W_THICK; r < nRows; r+=W_CYCLE)   
			{
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				double thick = pGrid->GetTextMatrixDouble(r,nCol) + thickDif;
				thick = max(thick, 2);
				pGrid->SetTextMatrix(r,nCol,"%.lf",thick);
			}
		}
		if (nRow % W_CYCLE == 1+W_SYMBOL)   
		{  // 변경강종
			CString sSym = pGrid->GetTextMatrix(nRow,nCol);

			for (long r = 1+W_SYMBOL; r < nRows; r+=W_CYCLE)   
			{
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				pGrid->SetTextMatrix(r,nCol,sSym,(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			}

			//강종바꾸면 복부판 두께검토내용도 바뀌어야 하므로...
			SetDataSave();
			CCellID next	= pGrid->GetFocusCell();
			long	nRow	= 1;
			long	nG		= SelectedGirder(pGrid->GetFocusCell().row, 4);
			
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);

			double	dSttSta = pGir->GetSSS(OFF_STT_GIRDER);
			double	dEndSta	= 0;

			for(long nCol=1; nCol<next.col;nCol++)
			{
				dSttSta += m_GridW.GetTextMatrixDouble(nRow, nCol);
			}

			dEndSta = dSttSta + m_GridW.GetTextMatrixDouble(nRow, next.col);
			WebThickCheck(nG, dSttSta, dEndSta);
			
		}
	}
	else	// 상, 하판
	{
		if (nRow % UL_CYCLE == 1+UL_LEN)   
		{  // 길이
			double staNew = GetStationMatrix(pGrid, nRow, nCol);
			pGrid->SetTextMatrix(nRow, nCol, pGrid->GetLastChangeData());
			double staDif = staNew - GetStationMatrix(pGrid, nRow, nCol);

			for (long r = 1; r < nRows; r+=UL_CYCLE)   {
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				double sta = GetStationMatrix(pGrid, r, nCol) + staDif;
				double staNext = GetStationMatrix(pGrid, r, nCol+1);
				SetMatrixByStation(pGrid, r, nCol, sta);
				SetMatrixByStation(pGrid, r, nCol+1, staNext);
				CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(r/UL_CYCLE-1);
				SetMatrixByStation(pGrid, r+UL_MOVEDIS, nCol, pGir->GetStationByOneStepMid(sta, 0));
			}
		}
		if (nRow % UL_CYCLE == 1+UL_THICK)   
		{  // 두께
			double thickNew = pGrid->GetTextMatrixDouble(nRow,nCol);
			pGrid->SetTextMatrix(nRow, nCol, pGrid->GetLastChangeData());
			double thickDif = thickNew - pGrid->GetTextMatrixDouble(nRow,nCol);

			for (long r = 1+UL_THICK; r < nRows; r+=UL_CYCLE)   {
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				double thick = pGrid->GetTextMatrixDouble(r,nCol) + thickDif;
				thick = max(thick, 2);
				pGrid->SetTextMatrix(r,nCol,"%.lf",thick);
			}
		}
		if (nRow % UL_CYCLE == 1+UL_WIDTH)  
		{  // 폭
			double widthNew = pGrid->GetTextMatrixDouble(nRow,nCol);
			pGrid->SetTextMatrix(nRow, nCol, pGrid->GetLastChangeData());
			double widthDif = widthNew - pGrid->GetTextMatrixDouble(nRow,nCol);

			for (long r = 1+UL_WIDTH; r < nRows; r+=UL_CYCLE)   {
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				double width = pGrid->GetTextMatrixDouble(r,nCol) + widthDif;
				width = max(width, 2);
				pGrid->SetTextMatrix(r,nCol,"%.lf",width);
			}
		}
		if (nRow % UL_CYCLE == 1+UL_SYMBOL)   
		{  // 변경강종
			CString sSym = pGrid->GetTextMatrix(nRow,nCol);

			for (long r = 1+UL_SYMBOL; r < nRows; r+=UL_CYCLE)   
			{
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				pGrid->SetTextMatrix(r,nCol,sSym,(UINT)DT_VCENTER|DT_SINGLELINE|DT_LEFT);
			}
		}
	}

	pGrid->Invalidate();
	
	SetDataSave();//061010플랜지두께검토부분이 해당위치 클릭시 연동될수 있게 추가..PKB
}

void CFactJoinPositionDlg::OnCellCopiedData(NMHDR* nmgv, LRESULT*)
{
	long nTab = m_TabWnd->GetCurSel();
	DataSave(nTab);
	SetDataInit();	
	DrawInputDomyunView(FALSE);
}

void CFactJoinPositionDlg::OnFactjoinCheckmove() 
{
	UpdateData();
	SetGridTitle(&m_GridU);
	SetGridTitle(&m_GridL);
	SetGridTitle(&m_GridW);
}

void CFactJoinPositionDlg::OnFactjoinBridgelen() 
{
	long	nOldType	= m_nLengthType;
	CString	sText		= _T("");
	
	m_dDisCenterOnFact.GetWindowText(sText);

	double dOldDisCenter = atof(sText);
	UpdateData();
	SetGridDataByChangeLengthType(&m_GridU, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_GridL, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_GridW, nOldType, dOldDisCenter);
}

void CFactJoinPositionDlg::OnFactjoinGirderlen() 
{
	long	nOldType	= m_nLengthType;
	CString	sText		= _T("");
	
	m_dDisCenterOnFact.GetWindowText(sText);

	double dOldDisCenter = atof(sText);

	UpdateData();
	SetGridDataByChangeLengthType(&m_GridU, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_GridL, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_GridW, nOldType, dOldDisCenter);
}

void CFactJoinPositionDlg::OnKillfocusFactjoinDistanceCenter()
{
	long	nOldType	= m_nLengthType;
	CString	sText		= _T("");
	
	m_dDisCenterOnFact.GetWindowText(sText);
	
	double dOldDisCenter = atof(sText);

	UpdateData();
	SetGridDataByChangeLengthType(&m_GridU, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_GridL, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_GridW, nOldType, dOldDisCenter);
}

void CFactJoinPositionDlg::SetGridDataByChangeLengthType(CGridCtrl *pGrid, int nOldType, double dOldDisCenter)
{
	long nRows = pGrid->GetRows();
	long nCols = pGrid->GetCols();

	if(IsActiveWeb(pGrid))
	{
		for (long nRow = 1; nRow < nRows; nRow += W_CYCLE)   
		{
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nRow/W_CYCLE - 1);
			double staStt = pGir->GetSSS(OFF_STT_GIRDER);
			double sta, lenSum = 0;
			long nEndCol = pGrid->GetValidCol(nRow, FALSE);
			for (long nCol = 1; nCol < nCols; nCol++)   
			{
				lenSum += pGrid->GetTextMatrixDouble(nRow, nCol);
				if(nCol == nCols-1) 
				{
					sta = pGir->GetSSS(OFF_END_SHOE);
				}
				else
				{
					if (nOldType == 0)
						sta = pGir->GetStationByActLength(staStt, lenSum);
					else
						sta = pGir->GetLine()->GetStationByDisLength(staStt, lenSum, dOldDisCenter);
				}
				SetMatrixByStation(pGrid, nRow, nCol, sta);
				if (nCol < nEndCol)
					SetMatrixByStation(pGrid, nRow + W_MOVEDIS, nCol, pGir->GetStationByOneStepMid(sta, 0));
				else
					pGrid->SetTextMatrix(nRow + W_MOVEDIS, nCol, "0");
			}
		}
	}
	else
	{
		for (long nRow = 1; nRow < nRows; nRow += UL_CYCLE)   
		{
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nRow/UL_CYCLE - 1);
			double staStt = pGir->GetSSS(OFF_STT_GIRDER);
			double sta, lenSum = 0;
			long nEndCol = pGrid->GetValidCol(nRow, FALSE);
			for (long nCol = 1; nCol < nCols; nCol++)   {
				lenSum += pGrid->GetTextMatrixDouble(nRow, nCol);
				if(nCol == nCols-1) 
				{
					sta = pGir->GetSSS(OFF_END_SHOE);
				}
				else
				{
					if (nOldType == 0)
						sta = pGir->GetStationByActLength(staStt, lenSum);
					else
						sta = pGir->GetLine()->GetStationByDisLength(staStt, lenSum, dOldDisCenter);
				}
				SetMatrixByStation(pGrid, nRow, nCol, sta);
				if (nCol < nEndCol)
					SetMatrixByStation(pGrid, nRow + UL_MOVEDIS, nCol, pGir->GetStationByOneStepMid(sta, 0));
				else
					pGrid->SetTextMatrix(nRow + UL_MOVEDIS, nCol, "0");
			}
		}
	}
	pGrid->Invalidate();
}

double CFactJoinPositionDlg::GetStationMatrix(CGridCtrl *pGrid, long nRow, long nCol)
{
	double lenSum = 0;
	for(long c = 1; c <= nCol; c++)
		lenSum += pGrid->GetTextMatrixDouble(nRow, c);

	CString	sText	= _T("");
	
	m_dDisCenterOnFact.GetWindowText(sText);

	double	sta		= m_pStd->GetBridge()->GetStationByLengthInGrid(FALSE, nRow/W_CYCLE-1, lenSum, m_nLengthType, atof(sText));
	if(!IsActiveWeb(pGrid))
		sta = m_pStd->GetBridge()->GetStationByLengthInGrid(FALSE, nRow/UL_CYCLE-1, lenSum, m_nLengthType, atof(sText));

	return sta;
}

void CFactJoinPositionDlg::SetMatrixByStation(CGridCtrl *pGrid, long nRow, long nCol, double sta)
{
	double	lenSum = 0, len = 0;
	CString	sText	= _T("");

	m_dDisCenterOnFact.GetWindowText(sText);
	
	double dDisCenterOnFact = atof(sText);

	if(IsActiveWeb(pGrid))	//복부
	{
		//nRow -= W_CYCLE;

		if (nRow % W_CYCLE == W_LEN+1)   
		{  // 길이
			for(long c = 1; c < nCol; c++)
				lenSum += pGrid->GetTextMatrixDouble(nRow, c);
			len = m_pStd->GetBridge()->GetLengthByStationInGrid(FALSE, nRow/W_CYCLE-1, sta, m_nLengthType, dDisCenterOnFact) - lenSum;
		}
		if (nRow % W_CYCLE == (W_MOVEDIS+1)%W_CYCLE)  
		{  // 이동
			for(long c = 1; c <= nCol; c++)
				lenSum += pGrid->GetTextMatrixDouble(nRow-W_MOVEDIS, c);
			len = -(m_pStd->GetBridge()->GetLengthByStationInGrid(FALSE, nRow/W_CYCLE-2, sta, m_nLengthType, dDisCenterOnFact) - lenSum);
			if (fabs(len) <= 1)
				len = 0;
		}
	}
	else	// 상,하부
	{
		//nRow -= UL_CYCLE;

		if (nRow % UL_CYCLE == UL_LEN+1)   
		{  // 길이
			for(long c = 1; c < nCol; c++)
				lenSum += pGrid->GetTextMatrixDouble(nRow, c);
			len = m_pStd->GetBridge()->GetLengthByStationInGrid(FALSE, nRow/UL_CYCLE-1, sta, m_nLengthType, dDisCenterOnFact) - lenSum;
		}
		if (nRow % UL_CYCLE == (UL_MOVEDIS+1)%UL_CYCLE)  
		{  // 이동
			for(long c = 1; c <= nCol; c++)
				lenSum += pGrid->GetTextMatrixDouble(nRow-UL_MOVEDIS, c);
			len = -(m_pStd->GetBridge()->GetLengthByStationInGrid(FALSE, nRow/UL_CYCLE-2, sta, m_nLengthType, dDisCenterOnFact) - lenSum);
			if (fabs(len) <= 1)
				len = 0;
		}

	}
	pGrid->SetTextMatrix(nRow, nCol, "%.0f", len);
}



void CFactJoinPositionDlg::OnEndCalc()
{
	SetDataInit();
}

LRESULT CFactJoinPositionDlg::OnTabChangePre(WPARAM iTab, LPARAM)
{
	if(iTab>=0 && iTab<3)
		DataSave((long)iTab);
	return 0;
}

LRESULT CFactJoinPositionDlg::OnTabChange(WPARAM iTab, LPARAM)
{
	int nTab = (int)iTab;
	if(nTab < 0) return 0;
	//DataSave((long)iTab);
	
	if(iTab==2) m_nTab = 1;
	else		m_nTab = 0;

	nCurMark = iTab;
	DrawInputDomyunView(TRUE);

	//탭이 바뀌면 플랜지 두께 체크 해줘야 함...
	CPlateGirderApp *pGir = NULL;
	CPlateFactChain *pFactChain = NULL;
	BOOL	bUpper	= TRUE;
	BOOL	bEnable	= TRUE;
	if(iTab==2)	bEnable = FALSE;
	long	nG		= 0;
	long	nCurCol = 1;
	
	switch(iTab)
	{
		case 0 ://활성화
			bUpper = TRUE;
			nG = m_GridU.GetFocusCell().row;

			if (nG != -1)//선택된 항목이 있을때...없으면 무조건..1,1열로..
			{
				nG = SelectedGirder(m_GridU.GetFocusCell().row, 5);
				nCurCol = m_GridU.GetFocusCell().col;
			}			
			pGir = m_pStd->GetBridge()->GetGirder(nG);			
			pFactChain = pGir->GetFactChain(G_F_U, nCurCol-1);			
			break;
		case 1 ://활성화...
			bUpper = FALSE;
			nG = m_GridL.GetFocusCell().row;

			if (nG != -1)//선택된 항목이 있을때...없으면 무조건..1,1열로..
			{
				nG = SelectedGirder(m_GridL.GetFocusCell().row, 5);
				nCurCol = m_GridU.GetFocusCell().col;
			}			
			pGir = m_pStd->GetBridge()->GetGirder(nG);
			pFactChain = pGir->GetFactChain(G_F_L, nCurCol-1);
			break;
		case 2://복부판...
			long	nRow	= 1;			
			nG = m_GridU.GetFocusCell().row;

			if (nG != -1)//선택된 항목이 있을때...없으면 무조건..1,1열로..
			{
				nG = SelectedGirder(m_GridW.GetFocusCell().row, 4);
				nCurCol = m_GridW.GetFocusCell().col;
			}
						
			pGir = m_pStd->GetBridge()->GetGirder(nG);

			double	dSttSta = pGir->GetSSS(OFF_STT_GIRDER);
			double	dEndSta	= 0;

			for(long nCol=1; nCol<nCurCol;nCol++)
			{
				dSttSta += m_GridW.GetTextMatrixDouble(nRow, nCol);
			}

			dEndSta = dSttSta + m_GridW.GetTextMatrixDouble(nRow, nCurCol);
			WebThickCheck(nG, dSttSta, dEndSta);

			break;
	}

	if (bEnable)
	{
		if(pFactChain==NULL) return 0;
		double dFlSharp = pGir->GetLengthSharpOnBx(pFactChain->GetBx(), bUpper);
		FlangeThickCheck(pFactChain, dFlSharp);	
	}

	GetDlgItem(IDC_BUTTON_FLANGE_WIDTH_SAME)->EnableWindow(bEnable);
/*	
	GetDlgItem(IDC_STATIC_GROUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SHARP)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_2)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MINT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_USET)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_RESULT)->EnableWindow(bEnable);

*/		
	return 0;
}

void CFactJoinPositionDlg::CheckWarn()
{
	CEnvRegist reg;	
	if(!reg.LoadEnvInt(CEnvRegist::SHOW_WARNMSG)) return;

	CGlobarOption *pGlopt = m_pStd->GetDataManage()->GetGlobalOption();
	BOOL	bDesignAllowDeflection	= pGlopt->GetDesignAllowDeflection();
	
	CString szErrMsg;
	// 상하부 플랜지 체크
	for(long nF = 0; nF<2; nF++)
	{
		long nMark = (nF==0) ? G_F_U : G_F_L;
		__int64 BxFlag = (nF==0) ? BX_UPPERFLANGE : BX_LOWERFLANGE;
		double dThickMin = pGlopt->GetDesignUDFlangeMinThick(nF==0?TRUE:FALSE);
		for(long nG=0; nG<m_pStd->GetBridge()->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);			
		
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);			
			while (Finder.GetBxFixNext())
			{
				CPlateFactChain *pPreFact = pBx->GetFactChain(nMark,FALSE);
				CPlateFactChain *pNexFact = pBx->GetFactChain(nMark,TRUE);
				double dSub = fabs(pNexFact->m_dFactChainThick - pPreFact->m_dFactChainThick);
//				if(dSub > dThickMin)
				if(bDesignAllowDeflection && dSub>min(pNexFact->m_dFactChainThick, pPreFact->m_dFactChainThick))
				{
					CString szMsg;
//					szMsg.Format("Sta. %.3f에서 두께가 %.1f 차이가 나서 허용범위 %.1f를 벗어남\r\n",
//								  pBx->GetStation(), dSub, dThickMin);
					szMsg.Format("Sta. %.3f에서 판두께의 차이가 두꺼운쪽 부재 두께의 1/2을 넘음. \r\n",
						pBx->GetStation());
					szErrMsg += ((nF==0) ? "상부 플랜지\r\n" : "하부 플랜지\r\n") + szMsg;
				}
				pBx = Finder.GetBxNext();
			}
		}
	}

	if(!szErrMsg.IsEmpty()) 
	{		
		CWarnDlg Dlg;
		Dlg.m_szTitle = "입력 부적절 오류 경고 !";
		Dlg.m_szMsg = szErrMsg;
		Dlg.DoModal();		
	}
}

long CFactJoinPositionDlg::GetTabCount()
{
	return 2;
}

CString CFactJoinPositionDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0) return "평면도";
	return "종단면도";
}

void CFactJoinPositionDlg::OnButtonFlangeWidthSame() 
{
	UpdateData(TRUE); 
	SetDataSave();
	
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CString			sText	= _T("");
		
	m_dFlangeSameWidth.GetWindowText(sText);
	
	pDB->m_dSameFlangeWidth = atof(sText);

	__int64	bxFlag = 0;
	switch( m_TabWnd->GetCurSel() )
	{
	case 0 :bxFlag = BX_UPPERFLANGE | BX_STT_GIRDER | BX_END_GIRDER; 				
			break;
	case 1 :bxFlag = BX_LOWERFLANGE | BX_STT_GIRDER | BX_END_GIRDER;  				
			break;
	case 2 :bxFlag = BX_WEB         | BX_STT_GIRDER | BX_END_GIRDER;  				
			break;	
	}
	long nG = 0;
	for(nG=-1; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag);							
		while(pBx)
		{								
			CPlateFactChain *pFactChain = pBx->GetFactChain(m_TabWnd->GetCurSel());

			pFactChain->m_dWidth	    = pDB->m_dSameFlangeWidth;

			pBx = Finder.GetBxNext();
		}
	
	}

	SetDataInit();
	DataSave();
	DrawInputDomyunView(FALSE);	

	//플랜지 폭원 동일적용 버튼 누른후 계산 되게..
	CPlateGirderApp *pGir = NULL;
	CPlateFactChain *pFactChain = NULL;
	BOOL bUpper = TRUE;

	switch( m_TabWnd->GetCurSel() )
	{
		case 0 :
			bUpper = TRUE;
			nG = m_GridU.GetFocusCell().row/4-1;
			pGir = m_pStd->GetBridge()->GetGirder(nG);
			pFactChain = pGir->GetFactChain(G_F_U, m_GridU.GetFocusCell().col-1);			
			break;
		case 1 :
			bUpper = FALSE;
			nG = m_GridL.GetFocusCell().row/4-1;
			pGir = m_pStd->GetBridge()->GetGirder(nG);
			pFactChain = pGir->GetFactChain(G_F_L, m_GridL.GetFocusCell().col-1);
			break;
		default:
			return;
	}
		
	if(pFactChain==NULL)	return;
	
	double dFlSharp = pGir->GetLengthSharpOnBx(pFactChain->GetBx(), bUpper);
	FlangeThickCheck(pFactChain, dFlSharp);	
}

void CFactJoinPositionDlg::OnFactjoinChecksymbol() 
{
	UpdateData();
	SetGridTitle(&m_GridU);
	SetGridTitle(&m_GridL);
	SetGridTitle(&m_GridW);
}

void CFactJoinPositionDlg::OnRistRecommend() 
{		
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CSteelSection *pSteelSection = m_pStd->m_pDataManage->GetSteelSection();

	UpdateData();

	CString szMaterialU = pDB->GetMaterial(G_F_U);
	CString szMaterialL = pDB->GetMaterial(G_F_L);
	if((szMaterialU!="SM520C-TMC" && szMaterialU!="SM570-TMC") || (szMaterialL!="SM520C-TMC" && szMaterialL!="SM570-TMC"))
	{
		AfxMessageBox("상하판 강재재질이 SM520C-TMC 또는 SM570-TMC일때만 지원됩니다.",MB_ICONWARNING);
		return;
	}

	if(!ContinueMsg()) return;

	BeginWaitCursor();

	CRistModule RistModule;
	
	long nMarkBujeFact=m_TabWnd->GetCurSel() == 0 ? G_F_U : m_TabWnd->GetCurSel() == 1 ? G_F_L : G_W;
	/////////////////////////////////////////////////////////////
	// 상판
	BOOL bSM520 = pDB->GetMaterial(G_F_U) == "SM520C-TMC";	
	double dMaxJigan = RistModule.GetLengthMaxJigan(pDB->m_nQtyJigan, pDB->m_dLengthJigan);
	//long nDivid = RistModule.GetDividCount(dMaxJigan, pDB->m_nQtyJigan, bSM520);
	double length;
	BOOL bUpper = FALSE;
	
	CPtrList NList;	
	long nLinkBx       = 0;
	__int64	bxFlag     = 0;
	switch( nMarkBujeFact )
	{
	case 0 :
			bxFlag = BX_UPPERFLANGE; 
			nLinkBx = CPlateBxObject::LK_FACTCHAIN_UPPER; 
			bUpper = TRUE;
			break;
	case 1 :
			bxFlag = BX_LOWERFLANGE;  
			nLinkBx = CPlateBxObject::LK_FACTCHAIN_LOWER; 
			bUpper = FALSE;
			break;
	case 2 :			
			bxFlag = BX_WEB;  
			nLinkBx = CPlateBxObject::LK_FACTCHAIN_WEB; 
			break;	
	}

	if (nMarkBujeFact == G_W)//복부판일경우 한구간만....061227...KB
	{
		//BeginWaitCursor();
		long nTab = G_W;//m_TabWnd->GetCurSel();
		SetDataDefaultByBuje(nTab, 1, TRUE);

		DataSave(nTab);
		DrawInputDomyunView(TRUE);
		EndWaitCursor();
		return;
	}

	long nDivid=0;
	for(long nG = -1; nG < pDB->GetGirdersu(); nG++)		
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);				
		double sta = pDB->GetGirder(nG)->GetSSS(OFF_STT_GIRDER);				
		double dSttSta, dEndSta, dLength;

		long nStt=0;
		long nJ = 0;
		for(nJ = 0; nJ < pDB->m_nQtyJigan; nJ++)
		{
			dSttSta = nJ==0 ? pDB->GetGirder(nG)->GetSSS(OFF_STT_GIRDER) : pDB->GetGirder(nG)->GetStationOnJijum(nJ);
			dEndSta = nJ==pDB->m_nQtyJigan-1 ? pDB->GetGirder(nG)->GetSSS(OFF_END_GIRDER) : pDB->GetGirder(nG)->GetStationOnJijum(nJ+1);
			dLength = dEndSta - dSttSta;			
			long nCount = (nJ==0 || nJ==pDB->m_nQtyJigan-1) ? 5 : 7;			
			if(nJ==pDB->m_nQtyJigan-1) --nCount;
			for(long nS=nStt; nS<nCount; nS++)
			{			
				double dRatio = RistModule.GetRatio(dMaxJigan, pDB->m_nQtyJigan, nJ, nS, bSM520);
				length = dLength * dRatio;
				sta += length;
				
				if((nJ!=pDB->m_nQtyJigan-1 && nS==nCount-1) || (nJ>0 && nJ<pDB->m_nQtyJigan-1 && (nS==0 || nS==nCount-1)))
				{
					double dRatio2 = RistModule.GetRatio(dMaxJigan, pDB->m_nQtyJigan, nJ+1, 0, bSM520);
					dSttSta = pDB->GetGirder(nG)->GetStationOnJijum(nJ+1);
					dEndSta = nJ+1==pDB->m_nQtyJigan-1 ? pDB->GetGirder(nG)->GetSSS(OFF_END_GIRDER) : pDB->GetGirder(nG)->GetStationOnJijum(nJ+2);
					dLength = dEndSta - dSttSta;			
					length = dLength * dRatio2;
					sta += length;
				}
				CPlateBasicIndex* p = pGir->newBx();
				p->SetState(bxFlag,TRUE);
				p->SetStation(sta);
				NList.AddTail(p);					
				++nDivid;		
				nStt=1;
			}
		}
		CDWordArray LKExceptionArr;
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_UPPER);
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_LOWER);
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_WEB);
		pGir->PressBxList(NList, bxFlag, &LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);	
		// nCol+1을 한 이유는 FactChain은 구간이기 때문에
		pGir->MakeFactChain(nDivid+1, nMarkBujeFact);	
		pGir->SetLKPOS(nLinkBx);
		// 기본값을 설정해줌
		long nFact=0;
		nStt = 0;
		for(nJ = 0; nJ < pDB->m_nQtyJigan; nJ++)
		{
			long nCount = (nJ==0 || nJ==pDB->m_nQtyJigan-1) ? 5 : 7;
			//if(nJ==pDB->m_nQtyJigan-1) --nCount;
			for(long nS=nStt; nS<nCount; nS++)
			{
				/*  복부인경우 위에서 처리하고 return...
				if(nMarkBujeFact == G_W)
					pGir->GetFactChain(nMarkBujeFact,nFact)->m_dFactChainThick = RistModule.GetWebThick(dMaxJigan, pDB->m_nQtyJigan, nJ, nS, bSM520);  					
				else  */

				//플렌지 두께가 NG 인 경우 두께 + 2 해줌...만족할때까지...070109 ..KB				
				CPlateFactChain *pFactChain = pGir->GetFactChain(nMarkBujeFact,nFact);

				double dFCThick = RistModule.GetFlangeThick(dMaxJigan, pDB->m_nQtyJigan, nJ, nS, bSM520, nMarkBujeFact == G_F_U ? TRUE : FALSE);  
				double dFCWidth = RistModule.GetFlangeWidth(pGir->GetHeightGirderByJijum(nJ,FALSE), dMaxJigan, pDB->m_nQtyJigan, nJ, nS, bSM520, nMarkBujeFact == G_F_U ? TRUE : FALSE);				

				pFactChain->m_dWidth			= dFCWidth;				
				pFactChain->m_strSymbol			= pSteelSection->GetMaterialByMarkBuje(nMarkBujeFact);
				//먼저 width 를 입력하고 자유돌출폭 계산을 해야 한다.
				double dFlSharp = pGir->GetLengthSharpOnBx(pFactChain->GetBx(), bUpper);//자유돌출폭...
				
				while(dFlSharp/16 > dFCThick)
				{
					dFCThick += 2;
				}								
				pFactChain->m_dFactChainThick	= dFCThick;				
				
				++nFact;
				nStt=1;				
			}
		}		
	}		
	
	m_pStd->m_pDoc->SetModifiedFlag(TRUE);
	
	// ===============================================================================

	CString sText	= _T("");

	SetDataInit();	
	
	m_dDisOnCenterFactJoin.GetWindowText(sText);

	MoveByMomentStress(atof(sText));//0);//070129일자 수정..KB...

	DataSave(m_TabWnd->GetCurSel());
	SetDataInit();		
	DrawInputDomyunView(TRUE);
	EndWaitCursor();	
}

void CFactJoinPositionDlg::OnButtonRistRef1() 
{
	CSubRistRefImgDlg Dlg(0);
	Dlg.DoModal();
}

void CFactJoinPositionDlg::OnButtonRistRef2() 
{
	CSubRistRefImgDlg Dlg(1);
	Dlg.DoModal();
}

void CFactJoinPositionDlg::OnButtonRistRef3() 
{
	CSubRistRefImgDlg Dlg(2);
	Dlg.DoModal();
}

void CFactJoinPositionDlg::FlangeThickCheck(CPlateFactChain *pFactChain, double dFlSharp)
{
	CString szTemp;

	szTemp.Format("플랜지두께검토(㎜)");
	GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(szTemp);
	
	szTemp.Format("자유돌출폭: %.2lf mm", dFlSharp);
	GetDlgItem(IDC_STATIC_SHARP)->SetWindowText(szTemp);
	
	szTemp.Format("최소두께");
	GetDlgItem(IDC_STATIC_2)->SetWindowText(szTemp);

	szTemp.Format("%.2lf/16 = %.2lf mm", dFlSharp, dFlSharp/16);
	GetDlgItem(IDC_STATIC_MINT)->SetWindowText(szTemp);

	szTemp.Format("Use : %.2lf mm", pFactChain->m_dFactChainThick);
	GetDlgItem(IDC_STATIC_USET)->SetWindowText(szTemp);

	if(dFlSharp/16 <= pFactChain->m_dFactChainThick)
		szTemp.Format("%.2lf <= %.2lf O.K", dFlSharp/16, pFactChain->m_dFactChainThick);
	else
		szTemp.Format("%.2lf >  %.2lf N.G", dFlSharp/16, pFactChain->m_dFactChainThick);

	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(szTemp);

	UpdateData(FALSE);
}

void CFactJoinPositionDlg::WebThickCheck(long nG, double dSttSta, double dEndSta)
{	
	CPlateGirderApp	*pGir	= m_pStd->GetBridge()->GetGirder(nG);	
	CPlateDesigCalc	DesignCalc(m_pStd->m_pDataManage, m_pStd->m_pCalcStd);
	CCalcGeneral	CalcGeneral(m_pStd->m_pDataManage);	
	CSteel			*pSteelCtl	= m_pStd->m_pDataManage->GetSteel();

	
	CPlateBasicIndex	*pBx = pGir->GetBxByStation(dSttSta);

	CString szTemp = "";
	CString sLeft = "", sRight = "";
	CalcGeneral.GetMaterial(pBx, G_W, sLeft, sRight);
	
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeft + sRight)];

	CHGAllowStress	AllowMinThick(pSteel);

	double dMaxHeight = DesignCalc.GetMaxGirderHight(pBx, nG, dSttSta, dEndSta);
	double dMinThick0	= AllowMinThick.GetAllowStress(ALLOW_PLATEBRIDGE_WEBMINTHICK, 0);	//강종별 최소두께
	double dMinThick1	= AllowMinThick.GetAllowStress(ALLOW_PLATEBRIDGE_WEBMINTHICK, 1);	//강종별 최소두께
	double dMinThick2	= AllowMinThick.GetAllowStress(ALLOW_PLATEBRIDGE_WEBMINTHICK, 2);	//강종별 최소두께	

	szTemp.Format("복부판두께검토(㎜)");
	GetDlgItem(IDC_STATIC_GROUP)->SetWindowText(szTemp);

	szTemp.Format("비합성(b/강종별)");
	GetDlgItem(IDC_STATIC_SHARP)->SetWindowText(szTemp);
	
	szTemp.Format("(☞ 도.설.기 3.8.4.1)");
	GetDlgItem(IDC_STATIC_2)->SetWindowText(szTemp);

	szTemp.Format("없음:%.0f/%.0f=%.0f mm", dMaxHeight, dMinThick0, (dMaxHeight/dMinThick0));
	GetDlgItem(IDC_STATIC_MINT)->SetWindowText(szTemp);

	szTemp.Format("1  단:%.0f/%.0f=%.0f mm", dMaxHeight, dMinThick1, (dMaxHeight/dMinThick1));
	GetDlgItem(IDC_STATIC_USET)->SetWindowText(szTemp);

	szTemp.Format("2  단:%.0f/%.0f=%.0f mm", dMaxHeight, dMinThick2, (dMaxHeight/dMinThick2));
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(szTemp);

	UpdateData(FALSE);
}

long CFactJoinPositionDlg::SelectedGirder(long nFocusCell_Row, long nQ)
{
	double nResidual = nFocusCell_Row % nQ;//나머지
	long nQuotient = nFocusCell_Row / nQ;//몫
	long nG = -1;

	if (nResidual == 0)
		nG = nQuotient - 2;
	else
		nG = nQuotient - 1;	 

	return nG;
}

void CFactJoinPositionDlg::OnDefaultMaterial() 
{
	if(AfxMessageBox("기존에 입력한 강종이 변경됩니다.\n계속하시겠습니까?", MB_YESNO | MB_ICONINFORMATION)== IDNO)
		return;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CSteelSection	*pSteelSection = m_pStd->m_pDataManage->GetSteelSection();

	long nMarkBujeFact = 0;
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER;

	for(long nBuje = 0; nBuje < 3;nBuje++)
	{
		if(nBuje==0)
		{
			nMarkBujeFact = G_F_U;
			BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_UPPERFLANGE;
		}
		else if(nBuje==1)
		{
			nMarkBujeFact = G_F_L;
			BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_LOWERFLANGE;
		}
		else if(nBuje==2)
		{
			nMarkBujeFact = G_W;
			BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_WEB;
		}
		CString szSymbol = pSteelSection->GetMaterialByMarkBuje(nMarkBujeFact);

		for(long nG=-1;nG<pDB->GetGirdersu();nG++)
		{
			CPlateGirderApp* pGir  = pDB->GetGirder(nG);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag);
			while(pBx)
			{
				CPlateFactChain *pFactChain = pBx->GetFactChain(nMarkBujeFact,FALSE);
				if(pFactChain)
					pFactChain->m_strSymbol = szSymbol;
				pBx = Finder.GetBxNext();
			}
		}
	}	
	SetDataInit();
}
