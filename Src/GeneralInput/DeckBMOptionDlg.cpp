// DeckBMOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckBMOptionDlg dialog


CDeckBMOptionDlg::CDeckBMOptionDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckBMOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckBMOptionDlg)
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}


void CDeckBMOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckBMOptionDlg)
	DDX_Control(pDX, IDC_CHECK_ELECWIREHOLE, m_ElecWireHole);
	DDX_Control(pDX, IDC_CHECK_TIMBER, m_Timber);
	DDX_Control(pDX, IDC_CHECK_WING_WALL, m_WingWall);
// 	DDX_Control(pDX, IDC_BUTTON_ALL_CANCEL, m_AllCancel);
// 	DDX_Control(pDX, IDC_BUTTON_ALL_CHECK, m_AllCheck);
	DDX_Control(pDX, IDC_CHECK_DRAIN_BRIDGESURFACE, m_DrainBridgeSurface);
	DDX_Control(pDX, IDC_CHECK_SHADE, m_Shade);
	DDX_Control(pDX, IDC_CHECK_PAVE, m_Pave);
	DDX_Control(pDX, IDC_CHECK_NOTCH, m_Notch);
	DDX_Control(pDX, IDC_CHECK_WATER_DRAW, m_WaterDraw);
	DDX_Control(pDX, IDC_CHECK_TACK_COATING, m_TackCoating);
	DDX_Control(pDX, IDC_CHECK_STYROFOAM, m_Styrofoam);
	DDX_Control(pDX, IDC_CHECK_STEEL_QUANTITY, m_SteelQuantity);
	DDX_Control(pDX, IDC_CHECK_SPACER, m_Spacer);
	DDX_Control(pDX, IDC_CHECK_SOUND_PROOF, m_SoundProof);
	DDX_Control(pDX, IDC_CHECK_SHRINKAGE_CONCRETE, m_ShrinkageConcrete);
	DDX_Control(pDX, IDC_CHECK_REBAR_MANUFACTURE, m_RebarManufacture);
	DDX_Control(pDX, IDC_CHECK_PARAPET, m_Parapet);
	DDX_Control(pDX, IDC_CHECK_PAINT, m_Paint);
	DDX_Control(pDX, IDC_CHECK_MOLD, m_Mold);
	DDX_Control(pDX, IDC_CHECK_GUARD_FENCE, m_GuardFence);
	DDX_Control(pDX, IDC_CHECK_EXPLAIN_PLATE, m_ExplainPlate);
	DDX_Control(pDX, IDC_CHECK_EXPANSIONJOINT_COVER, m_ExpansionJointCover);
	DDX_Control(pDX, IDC_CHECK_EXPANSIONJOINT, m_ExpansionJoint);
	DDX_Control(pDX, IDC_CHECK_ESTABLISH_TBM, m_EstablishTBM);
	DDX_Control(pDX, IDC_CHECK_DROPPING_PREVENT, m_DroppingPreVent);
	DDX_Control(pDX, IDC_CHECK_DRAIN_ESTABLISH, m_DrainEstablish);
	DDX_Control(pDX, IDC_CHECK_CONCRETE_PLACING, m_ConcretePlacing);
	DDX_Control(pDX, IDC_CHECK_CONCRETE_BUY, m_ConcreteBuy);
	DDX_Control(pDX, IDC_CHECK_BRIDGE_NAMEPLATE, m_BridgeNamePlate);
	DDX_Control(pDX, IDC_CHECK_BRIDGE_NAME, m_BridgeName);
	DDX_Control(pDX, IDC_RICHEDIT_CONTENTS, m_OptContents);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);	// �ݵ�� IDC_GRID �� ����
	DDX_Control(pDX, IDC_BUTTON_OPTION_SAVE, m_btnOptSave);
	DDX_Control(pDX, IDC_BUTTON_OPTION_LOAD, m_btnOptLoad);
	DDX_Control(pDX, IDC_BUTTON_ALL_CHECK, m_AllCheck);
	DDX_Control(pDX, IDC_BUTTON_ALL_CANCEL, m_AllCancel);
}

BEGIN_MESSAGE_MAP(CDeckBMOptionDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckBMOptionDlg)
	ON_BN_CLICKED(IDC_CHECK_CONCRETE_BUY, OnCheckConcreteBuy)
	ON_BN_CLICKED(IDC_CHECK_CONCRETE_PLACING, OnCheckConcretePlacing)
	ON_BN_CLICKED(IDC_CHECK_MOLD, OnCheckMold)
	ON_BN_CLICKED(IDC_CHECK_SPACER, OnCheckSpacer)
	ON_BN_CLICKED(IDC_CHECK_REBAR_MANUFACTURE, OnCheckRebarManufacture)
	ON_BN_CLICKED(IDC_CHECK_PAVE, OnCheckPave)
	ON_BN_CLICKED(IDC_CHECK_TACK_COATING, OnCheckTackCoating)
	ON_BN_CLICKED(IDC_CHECK_DRAIN_BRIDGESURFACE, OnCheckDrainBridgesurface)
	ON_BN_CLICKED(IDC_CHECK_BRIDGE_NAME, OnCheckBridgeName)
	ON_BN_CLICKED(IDC_CHECK_BRIDGE_NAMEPLATE, OnCheckBridgeNameplate)
	ON_BN_CLICKED(IDC_CHECK_EXPLAIN_PLATE, OnCheckExplainPlate)
	ON_BN_CLICKED(IDC_CHECK_ESTABLISH_TBM, OnCheckEstablishTbm)
	ON_BN_CLICKED(IDC_CHECK_STYROFOAM, OnCheckStyrofoam)
	ON_BN_CLICKED(IDC_CHECK_EXPANSIONJOINT, OnCheckExpansionjoint)
	ON_BN_CLICKED(IDC_CHECK_EXPANSIONJOINT_COVER, OnCheckExpansionjointCover)
	ON_BN_CLICKED(IDC_CHECK_SHRINKAGE_CONCRETE, OnCheckShrinkageConcrete)
	ON_BN_CLICKED(IDC_CHECK_WATER_DRAW, OnCheckWaterDraw)
	ON_BN_CLICKED(IDC_CHECK_NOTCH, OnCheckNotch)
	ON_BN_CLICKED(IDC_CHECK_GUARD_FENCE, OnCheckGuardFence)
	ON_BN_CLICKED(IDC_CHECK_SOUND_PROOF, OnCheckSoundProof)
	ON_BN_CLICKED(IDC_CHECK_PARAPET, OnCheckParapet)
	ON_BN_CLICKED(IDC_CHECK_SHADE, OnCheckShade)
	ON_BN_CLICKED(IDC_CHECK_DROPPING_PREVENT, OnCheckDroppingPrevent)
	ON_BN_CLICKED(IDC_CHECK_STEEL_QUANTITY, OnCheckSteelQuantity)
	ON_BN_CLICKED(IDC_CHECK_PAINT, OnCheckPaint)
	ON_BN_CLICKED(IDC_CHECK_DRAIN_ESTABLISH, OnCheckDrainEstablish)
	ON_BN_CLICKED(IDC_BUTTON_ALL_CHECK, OnButtonAllCheck)
	ON_BN_CLICKED(IDC_BUTTON_ALL_CANCEL, OnButtonAllCancel)
	ON_BN_CLICKED(IDC_CHECK_WING_WALL, OnCheckWingWall)
	ON_BN_CLICKED(IDC_CHECK_TIMBER, OnCheckTimber)
	ON_BN_CLICKED(IDC_CHECK_ELECWIREHOLE, OnCheckElecWireHole)
	ON_BN_CLICKED(IDC_BUTTON_OPTION_SAVE, OnButtonOptionSave)
	ON_BN_CLICKED(IDC_BUTTON_OPTION_LOAD, OnButtonOptionLoad)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_BUTTON_PUSH, OnReceive)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckBMOptionDlg message handlers

void CDeckBMOptionDlg::OnPreInitDialog()
{
	CSteelUWeight *pUWeight = m_pStd->m_pDataManage->GetSteelUWeight();		
	m_nSelectedOpt	= 0;
	m_nCols			= 2;
	m_nRows			= 1;

	m_Grid.SetColumnCount(m_nCols);
	m_Grid.SetRowCount(m_nRows);
	m_Grid.SetFiexedRows(1);
	m_Grid.SetFixedCols(1);
	m_Grid.SetTextMatrix(0, 0, _T("�� ��"));
	m_Grid.SetTextMatrix(0, 1, _T("���� ����"));
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 140);
	m_OptContents.SetReadOnly();
	m_OptContents.SetBackgroundColor(FALSE, RGB(230, 255, 255));
	
	
	CString	sText = _T("");
	sText.Format("%s����", pUWeight->GetTypePave());
	m_Pave.SetWindowText(sText);

	SetResize(IDC_RICHEDIT_CONTENTS, SZ_TOP_LEFT, SZ_BOTTOM_CENTER);
	SetResize(IDC_STATIC_OPT_DETAIL, SZ_MIDDLE_CENTER, SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID, SZ_MIDDLE_CENTER, SZ_BOTTOM_RIGHT);

	SetResize(IDC_CHECK_CONCRETE_BUY, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_CONCRETE_PLACING, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_MOLD, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_SPACER, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_REBAR_MANUFACTURE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_PAVE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_TACK_COATING, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_DRAIN_BRIDGESURFACE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_BRIDGE_NAME, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_BRIDGE_NAMEPLATE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_EXPLAIN_PLATE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_ESTABLISH_TBM, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_STYROFOAM, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_EXPANSIONJOINT, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_EXPANSIONJOINT_COVER, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_SHRINKAGE_CONCRETE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_WATER_DRAW, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_NOTCH, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_GUARD_FENCE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_SOUND_PROOF, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_PARAPET, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_SHADE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_DROPPING_PREVENT, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_STEEL_QUANTITY, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_PAINT, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_DRAIN_ESTABLISH, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_WING_WALL, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_TIMBER, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_CHECK_ELECWIREHOLE, SZ_TOP_CENTER, SZ_TOP_CENTER);

	SetResize(IDC_BUTTON_ALL_CHECK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_ALL_CANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_OPTION_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_OPTION_SAVE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}

void CDeckBMOptionDlg::SetGridTitle()
{
	long nRow = 0;
	long nCol = 0;
	for(nRow=0; nRow<m_Grid.GetRowCount(); nRow++)
	{
		for(nCol=0; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			m_Grid.SetItemState(nRow,nCol,0);
			m_Grid.SetCellType(nRow,nCol,CT_EDIT);
			m_Grid.SetTextBkColor(RGBEDITABLE);
			if(nRow != 0 && nCol != 0)
				m_Grid.SetItemBkColour(nRow, nCol, RGBEDITABLE);
		}
	}

	m_Grid.SetColumnCount(m_nCols);
	m_Grid.SetRowCount(m_nRows);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetFiexedRows(1);
	m_Grid.SetFixedCols(1);
	m_Grid.SetTextMatrix(0, 0, _T("�� ��"));
	m_Grid.SetTextMatrix(0, 1, _T("���� ����"));
	for(nRow=1; nRow<m_nRows; nRow++)
	{
		for(nCol=1; nCol<m_nCols; nCol++)
			m_Grid.SetItemBkColour(nRow, nCol, RGB(230, 255, 255));
	}
}

void CDeckBMOptionDlg::SetDataInit()
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	CStringArray	szArray;

	GetBMOptionContents(szArray);
	m_OptContents.SetSel(0, -1);
	m_OptContents.Clear();
	for(long n = 0; n < szArray.GetSize(); n++)
		m_OptContents.ReplaceSel(szArray.GetAt(n));

	m_ConcreteBuy.SetCheck(pBMOpt->m_bConcreteBuy);
	m_ConcreteBuy.Invalidate();
	m_ConcretePlacing.SetCheck(pBMOpt->m_bConcretePlacing);
	m_Mold.SetCheck(pBMOpt->m_bMold);
	m_Spacer.SetCheck(pBMOpt->m_bSpacer);
	m_RebarManufacture.SetCheck(pBMOpt->m_structRebarManufacture.m_bApply);
	m_Pave.SetCheck(pBMOpt->m_structPave.m_bApply);
	m_TackCoating.SetCheck(pBMOpt->m_structTackCoating.m_bApply);
	m_DrainBridgeSurface.SetCheck(pBMOpt->m_bDrainBridgeSurface);
	m_BridgeName.SetCheck(pBMOpt->m_structBridgeName.m_bApply);
	m_BridgeNamePlate.SetCheck(pBMOpt->m_structBridgeNamePlate.m_bApply);
	m_ExplainPlate.SetCheck(pBMOpt->m_structExplainPlate.m_bApply);
	m_EstablishTBM.SetCheck(pBMOpt->m_structEstablishTBM.m_bApply);
	m_Styrofoam.SetCheck(pBMOpt->m_structStyrofoam.m_bApply);
	m_ExpansionJoint.SetCheck(pBMOpt->m_structExpansionJoint.m_bApply);
	m_ExpansionJointCover.SetCheck(pBMOpt->m_structExpansionJointCover.m_bApply);
	m_ShrinkageConcrete.SetCheck(pBMOpt->m_structShrinkageConcrete.m_bApply);
	m_WaterDraw.SetCheck(pBMOpt->m_bWaterDraw);
	m_Notch.SetCheck(pBMOpt->m_structNotch.m_bApply);
	m_GuardFence.SetCheck(pBMOpt->m_structGuardFence.m_bApply);
	m_SoundProof.SetCheck(pBMOpt->m_structSoundProof.m_bApply);
	m_Parapet.SetCheck(pBMOpt->m_structParapet.m_bApply);
	m_Shade.SetCheck(pBMOpt->m_bShade);
	m_DroppingPreVent.SetCheck(pBMOpt->m_structDroppingPrevent.m_bApply);
	m_SteelQuantity.SetCheck(pBMOpt->m_bSteelQuantity);
	m_Paint.SetCheck(pBMOpt->m_bPaint);
	m_DrainEstablish.SetCheck(pBMOpt->m_bDrainEstablish);
	m_WingWall.SetCheck(pBMOpt->m_structWingWall.m_bApply);
	m_Timber.SetCheck(pBMOpt->m_structTimber.m_bApply);
	m_ElecWireHole.SetCheck(pBMOpt->m_structElecWireHole.m_bApply);

	UpdateData(FALSE);
}

void CDeckBMOptionDlg::SetDataSave()
{

}

BOOL CDeckBMOptionDlg::IsLeft(CString szApply)
{
	if(szApply == _T("��������"))		return TRUE;
	else if(szApply == _T("��������"))	return FALSE;
	else return FALSE;
	return FALSE;
}

BOOL CDeckBMOptionDlg::IsApply(CString	szApply)
{
	if(szApply == _T("����") || szApply == _T("��������") || szApply == _T("��������") || szApply == _T("��������") || szApply == _T("��������") || szApply == _T("��������") || szApply == _T("���� m�� ����"))
		return TRUE;
	if(szApply == _T("������") || szApply == _T("����������") || szApply == _T("����������") || szApply == _T("����������") || szApply == _T("����������") || szApply == _T("��������") || szApply == _T("������ ����"))
		return FALSE;
	ASSERT(FALSE);
	return FALSE;
}

CString CDeckBMOptionDlg::IsApply(BOOL bApply)
{
	CString szApply = _T("");
	if(bApply)	szApply = _T("����");
	else		szApply = _T("������");
	return szApply;
}

CString CDeckBMOptionDlg::IsApplyStt(BOOL bApply)
{
	CString szApply = _T("");
	if(bApply)	szApply = _T("��������");
	else		szApply = _T("����������");
	return szApply;
}

CString CDeckBMOptionDlg::IsApplyEnd(BOOL bApply)
{
	CString szApply = _T("");
	if(bApply)	szApply = _T("��������");
	else		szApply = _T("����������");
	return szApply;
}

CString CDeckBMOptionDlg::IsApplyLeft(BOOL bApply)
{
	CString szApply = _T("");
	if(bApply)	szApply = _T("��������");
	else		szApply = _T("����������");
	return szApply;
}

CString CDeckBMOptionDlg::IsApplyRight(BOOL bApply)
{
	CString szApply = _T("");
	if(bApply)	szApply = _T("��������");
	else		szApply = _T("����������");
	return szApply;
}

void CDeckBMOptionDlg::OnGridRedraw()
{
	switch(m_nSelectedOpt)
	{
	case BMOPT_CONCRETE_BYE :
		OnCheckConcreteBuy();
		break;
	case BMOPT_CONCRETE_PLACING :
		OnCheckConcretePlacing();
		break;
	case BMOPT_MOLD :
		OnCheckMold();
		break;
	case BMOPT_SPACER :
		OnCheckSpacer();
		break;
	case BMOPT_REBAR_MANUFACTURE :
		OnCheckRebarManufacture();
		break;
	case BMOPT_PAVE :
		OnCheckPave();
		break;
	case BMOPT_TACKCOATING :
		OnCheckTackCoating();
		break;
	case BMOPT_DRAINBRIDGESURFACE :
		OnCheckDrainBridgesurface();
		break;
	case BMOPT_BRIDGENAME :
		OnCheckBridgeName();
		break;
	case BMOPT_BRIDGENAME_PLATE :
		OnCheckBridgeNameplate();
		break;
	case BMOPT_EXPLAIN_PLATE :
		OnCheckExplainPlate();
		break;
	case BMOPT_ESTABLISH_TBM :
		OnCheckEstablishTbm();
		break;
	case BMOPT_STYROFOAM :
		OnCheckStyrofoam();
		break;
	case BMOPT_EXPANSIONJOINT :
		OnCheckExpansionjoint();
		break;
	case BMOPT_EXPANSIONJOINT_COVER :
		OnCheckExpansionjointCover();
		break;
	case BMOPT_SHRINKAGE_CONCRETE :
		OnCheckShrinkageConcrete();
		break;
	case BMOPT_WATER_DRAW :
		OnCheckWaterDraw();
		break;
	case BMOPT_NOTCH :
		OnCheckNotch();
		break;
	case BMOPT_GUARD_FENCE :
		OnCheckGuardFence();
		break;
	case BMOPT_SOUND_PROOF :
		OnCheckSoundProof();
		break;
	case BMOPT_PARAPET :
		OnCheckParapet();
		break;
	case BMOPT_SHADE :
		OnCheckShade();
		break;
	case BMOPT_DROPPING_PREVENT :
		OnCheckDroppingPrevent();
		break;
	case BMOPT_STEEL_QUANTITY :
		OnCheckSteelQuantity();
		break;
	case BMOPT_PAINT :
		OnCheckPaint();
		break;
	case BMOPT_DRAIN_ESTABLISH :
		OnCheckDrainEstablish();
		break;
	case BMOPT_WING_WALL :
		OnCheckWingWall();
		break;
	case BMOPT_TIMBER :
		OnCheckTimber();
		break;
	case BMOPT_ELECWIREHOLE :
		OnCheckElecWireHole();
		break;
	}
}

void CDeckBMOptionDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB		= m_pStd->m_pDataManage->GetBridge();	
	CADeckData		*pADeckData	= m_pStd->m_pDeckData;
	CBMOption		*pBMOpt		= &pADeckData->m_BMOption;	
	CCalcData		*pData		= m_pStd->m_pDataManage->GetCalcData();	
	CCalcFloor		CalcFloor(m_pStd->m_pDataManage);
	CCentSeparation	*pLCen		= NULL;
	CCentSeparation	*pRCen		= NULL;
	double dGuardHeightL = 0, dGuardHeightR = 0;
	if(pDB->GetGuardFencesu() > 0)
	{
		pLCen = pDB->GetGuardFence(0);
		pRCen = pDB->GetGuardFence(pDB->GetGuardWallSu()-1);

		dGuardHeightL = pLCen->GetHeight();
		dGuardHeightR = pRCen->GetHeight();
	}
	pBMOpt->SetDataDefault();

	double	dWingLeftStt	= pBMOpt->m_structWingWall.m_dLengthLeftStt;
	double	dWingLeftEnd	= pBMOpt->m_structWingWall.m_dLengthLeftEnd;
	double	dWingRighStt	= pBMOpt->m_structWingWall.m_dLengthRighStt;
	double	dWingRighEnd	= pBMOpt->m_structWingWall.m_dLengthRighEnd;
	pBMOpt->m_structParapet.m_dGuardLength[0]	= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT) + dWingLeftStt + dWingLeftEnd;
	pBMOpt->m_structParapet.m_dGuardLength[1]	= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT) + dWingRighStt + dWingRighEnd;
	pBMOpt->m_structParapet.m_dCurbLength		= (pBMOpt->m_structParapet.m_dGuardLength[0] + pBMOpt->m_structParapet.m_dGuardLength[1])/2;

	double dLHeight	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight;
	double dRHeight	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight;

	long nGuardTypeL = CalcFloor.GetGuardWallType(FLOOR_LEFT);
	long nGuardTypeR = CalcFloor.GetGuardWallType(FLOOR_RIGHT);
	BOOL bBangEmExistL = (pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && nGuardTypeL<=8) ? TRUE : FALSE;
	BOOL bBangEmExistR = (pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && nGuardTypeR<=8) ? TRUE : FALSE;
		 
	if(bBangEmExistL)
		pBMOpt->m_structSoundProof.m_dLHeight	= frM(dLHeight - toM(dGuardHeightL));
	if(bBangEmExistR)
		pBMOpt->m_structSoundProof.m_dRHeight	= frM(dRHeight - toM(dGuardHeightR));

	OnGridRedraw();
}

void CDeckBMOptionDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom	= m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}
BOOL CDeckBMOptionDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	BOOL bReturn = CGeneralBaseDlg::PreTranslateMessage(pMsg);
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	CStringArray strCombo;

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		switch(m_nSelectedOpt)
		{
		case BMOPT_CONCRETE_BYE :
		case BMOPT_CONCRETE_PLACING :
		case BMOPT_MOLD :
		case BMOPT_SPACER :
		case BMOPT_PAVE :
		case BMOPT_TACKCOATING :
		case BMOPT_DRAINBRIDGESURFACE :
		case BMOPT_SHRINKAGE_CONCRETE :
		case BMOPT_WATER_DRAW :
		case BMOPT_GUARD_FENCE :
		case BMOPT_SOUND_PROOF :
		case BMOPT_SHADE :
		case BMOPT_STEEL_QUANTITY :
		case BMOPT_PAINT :
		case BMOPT_BRIDGENAME :
		case BMOPT_BRIDGENAME_PLATE :
		case BMOPT_EXPLAIN_PLATE :
		case BMOPT_ESTABLISH_TBM :
		case BMOPT_EXPANSIONJOINT_COVER :
		case BMOPT_PARAPET :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			break;
		case BMOPT_STYROFOAM :
			if(next.col == 1)
			{
				if(next.row == 1 && next.col == 1)
				{
					strCombo.Add("����");
					strCombo.Add("������");
					m_Grid.SetComboString(strCombo);
				}
				else if(next.row == 2 && next.col == 1)
				{
					strCombo.Add("��������");
					strCombo.Add("��������");
					m_Grid.SetComboString(strCombo);				
				}
				else if(next.row ==3 && next.col == 1)
				{
					strCombo.Add("10");
					strCombo.Add("20");
					m_Grid.SetComboString(strCombo);
				}
			}
			break;
		case BMOPT_EXPANSIONJOINT :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			else if(next.row == 2 && next.col == 1)
			{
//				strCombo.Add("���� m�� ����");
				strCombo.Add("������ ����");
				m_Grid.SetComboString(strCombo);			
			}
			else if(next.row == 4 || next.row == 5 || next.row == 6 || next.row == 8)
			{
				if(next.col == 1)
					GetComboRebarDia(strCombo);
				m_Grid.SetComboString(strCombo);
			}
			break;
		case BMOPT_NOTCH :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			else if(next.row == 2 && next.col == 1)
			{
				strCombo.Add("��������");
				strCombo.Add("��������");
				strCombo.Add("��������");
				m_Grid.SetComboString(strCombo);
			}
			break;
		case BMOPT_WING_WALL :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);			
			}
			break;
		case BMOPT_DRAIN_ESTABLISH :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			else if(next.row == 2 && next.col == 1)
			{
				strCombo.Add("��õ��");
				strCombo.Add("������");
				strCombo.Add("��õ��������");
				m_Grid.SetComboString(strCombo);
			}
			else if(next.row == 3 && next.col == 1)
			{
				if(pBMOpt->m_nDrainEstablishType == WALKWAY || pBMOpt->m_nDrainEstablishType == RIVERWALKWAY)
				{
					strCombo.Add("TYPE 1 (�Ǳ���)");
					strCombo.Add("TYPE 2");
					m_Grid.SetComboString(strCombo);
				}
			}
			break;
		case BMOPT_TIMBER :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			else if(next.row == 2 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("����");
				m_Grid.SetComboString(strCombo);			
			}
			else if(next.row == 3 && next.col == 1)
			{
				strCombo.Add("������");
				strCombo.Add("��������");
				strCombo.Add("��������");
				strCombo.Add("��������");
				m_Grid.SetComboString(strCombo);
			}
			break;
		case BMOPT_REBAR_MANUFACTURE :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			else if((next.row == 2 || next.row == 3 || next.row == 4) && next.col == 1)
			{
				strCombo.Add("�ſ캹��");
				strCombo.Add("����");
				strCombo.Add("����");
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			break;
		case BMOPT_DROPPING_PREVENT :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);
			}
			else if(next.row == 2 && next.col == 1)
			{
				strCombo.Add("��������");
				strCombo.Add("�����������");
				m_Grid.SetComboString(strCombo);			
			}
			break;
		case BMOPT_ELECWIREHOLE :
			if(next.row == 1 && next.col == 1)
			{
				strCombo.Add("����");
				strCombo.Add("������");
				m_Grid.SetComboString(strCombo);			
			}
			break;
		}
	}
	return bReturn;
}

void CDeckBMOptionDlg::GetComboRebarDia(CStringArray &strComboArr)
{
	CADeckData			*pADeckData	= m_pStd->m_pDeckData;
	
	double	dSigmaY	= toKgPCm2(pADeckData->m_SigmaY);
	CString szFy;
	szFy = GetCharRebarMark(dSigmaY);

	strComboArr.Add(szFy+"13");
	strComboArr.Add(szFy+"16");
	strComboArr.Add(szFy+"19");
	strComboArr.Add(szFy+"22");
	strComboArr.Add(szFy+"25");
	strComboArr.Add(szFy+"29");
	strComboArr.Add(szFy+"32");
	strComboArr.Add(szFy+"35");	
}

void CDeckBMOptionDlg::GetBMOptionContents(CStringArray &Arr)
{
	CPlateBridgeApp		*pDB		= m_pStd->m_pDataManage->GetBridge();	
	CGlobarOption		*pGlopt		= m_pStd->m_pDataManage->GetGlobalOption();
	CSteelUWeight		*pUWeight	= m_pStd->m_pDataManage->GetSteelUWeight();
	CADeckData			*pADeckData	= m_pStd->m_pDeckData;
	CCalcData			*pData		= m_pStd->m_pDataManage->GetCalcData();	
	CCalcFloor			CalcFloor(m_pStd->m_pDataManage);
	CBMOption			*pBMOpt		= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	double	dSigmaY	= toKgPCm2(pADeckData->m_SigmaY);
		
	sText.Format("\n");																			Arr.Add(sText);
	sText.Format("  �� ���� ��� �ɼ� ���� ���� �� \n\n");										Arr.Add(sText);
	sText.Format("      1. ��ũ��Ʈ ����  :    %s\n", IsApply(pBMOpt->m_bConcreteBuy));			Arr.Add(sText);
	sText.Format("      2. ��ũ��Ʈ Ÿ��  :    %s\n", IsApply(pBMOpt->m_bConcretePlacing));		Arr.Add(sText);
	sText.Format("      3. �� Ǫ ��          :    %s\n", IsApply(pBMOpt->m_bMold));				Arr.Add(sText);
	sText.Format("      4. �����̼�         :    %s\n", IsApply(pBMOpt->m_bSpacer));				Arr.Add(sText);
	sText.Format("      5. ö�ٰ��� ����  :    %s\n", IsApply(pBMOpt->m_structRebarManufacture.m_bApply));		Arr.Add(sText);
	if(pBMOpt->m_structRebarManufacture.m_bApply)
	{
		CString szSpace = pDB->IsTUGir() ? _T("      ") : _T("");
		sText.Format("                    - �� �� ��%s  : %s \n", szSpace, pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nSlabType));
		Arr.Add(sText);
		if(pDB->GetGuardWallSu() > 0)
		{
			sText.Format("                    - �� ȣ ��%s  : %s \n", szSpace, pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nGuardFenceType));
			Arr.Add(sText);
		}

		if(pBMOpt->m_structExpansionJoint.m_bApply)
		{
			sText.Format("                    - ��������%s : %s \n", szSpace, pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nExpansionJointType));
			Arr.Add(sText);
		}

		if(pDB->IsTUGir())
		{
			sText.Format("                    - ������ũ��Ʈ : %s \n", pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nBindConcType));
			Arr.Add(sText);
		}
	}

	// ����
	if(pBMOpt->m_structPave.m_bApply)	sText.Format("      6. %s ����  :    %.0f mm\n", pUWeight->GetTypePave(), pBMOpt->m_structPave.m_dPaveThick);
	else								sText.Format("      6. %s ����  :    %s\n", pUWeight->GetTypePave(), IsApply(pBMOpt->m_structPave.m_bApply));
	Arr.Add(sText);

	if(pBMOpt->m_structTackCoating.m_bApply)	sText.Format("      7. �� �� ��          :    %.1f ��\n", pBMOpt->m_structTackCoating.m_dTackCoating);
	else										sText.Format("      7. �� �� ��          :    %s\n", IsApply(pBMOpt->m_structTackCoating.m_bApply));
	Arr.Add(sText);

	sText.Format("      8. ������         :    %s\n", IsApply(pBMOpt->m_bDrainBridgeSurface));				Arr.Add(sText);

	sText.Format("      9. �� �� ��          :    %s\n", IsApply(pBMOpt->m_structBridgeName.m_bApply));			Arr.Add(sText);
	if(pBMOpt->m_structBridgeName.m_bApply)
	{
		sText.Format("                    - ��ġ���� :    %d  EA\n", pBMOpt->m_structBridgeName.m_nQty);		Arr.Add(sText);
	}

	sText.Format("     10. �� �� ��          :    %s\n", IsApply(pBMOpt->m_structBridgeNamePlate.m_bApply));	Arr.Add(sText);
	if(pBMOpt->m_structBridgeNamePlate.m_bApply)
	{
		sText.Format("                    - ��ġ���� :    %d  EA\n", pBMOpt->m_structBridgeNamePlate.m_nQty);	Arr.Add(sText);
	}

	sText.Format("     11. �� �� ��          :    %s\n", IsApply(pBMOpt->m_structExplainPlate.m_bApply));		Arr.Add(sText);
	if(pBMOpt->m_structExplainPlate.m_bApply)
	{
		sText.Format("                    - ��ġ���� :    %d  EA\n", pBMOpt->m_structExplainPlate.m_nQty);		Arr.Add(sText);
	}

	sText.Format("     12. TBM ��ġ       :    %s\n", IsApply(pBMOpt->m_structEstablishTBM.m_bApply));			Arr.Add(sText);
	if(pBMOpt->m_structEstablishTBM.m_bApply)
	{
		sText.Format("                    - ��ġ���� :    %d  EA\n", pBMOpt->m_structEstablishTBM.m_nQty);		Arr.Add(sText);

	}

	if(pBMOpt->m_structStyrofoam.m_bApply)
	{
		if(pBMOpt->m_structStyrofoam.m_bLeft)
			sText.Format("     13. ��Ƽ����         :   ��������      %.0f mm\n", pBMOpt->m_structStyrofoam.m_dThick);
		else
			sText.Format("     13. ��Ƽ����         :   ��������      %.0f mm\n", pBMOpt->m_structStyrofoam.m_dThick);
	}
	else
		sText.Format("     13. ��Ƽ����         :    %s\n", IsApply(pBMOpt->m_structStyrofoam.m_bApply));
	Arr.Add(sText);	

	sText.Format("     14. ��������         :    %s\n", IsApply(pBMOpt->m_structExpansionJoint.m_bApply));	Arr.Add(sText);
	if(pBMOpt->m_structExpansionJoint.m_bApply)
	{
		if(pBMOpt->m_structExpansionJoint.m_bUnitLength)
		{
			sText.Format("                    - �������� :    ���� m�� ���� (Kgf/m)\n");
			Arr.Add(sText);
			sText.Format("                    - ���� :    %s                         - ���� : %s\n", pBMOpt->m_structExpansionJoint.m_szSttName, pBMOpt->m_structExpansionJoint.m_szEndName);
			Arr.Add(sText);
			sText.Format("                                  %s         %.3f Kgf/m                   %s        %.3f Kgf/m\n",
					pBMOpt->GetExpansionJointDia(TRUE, 0, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dSttWeight[0]),
					pBMOpt->GetExpansionJointDia(FALSE, 0, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dEndWeight[0]));		Arr.Add(sText);
			sText.Format("                                  %s         %.3f Kgf/m                   %s        %.3f Kgf/m\n",
					pBMOpt->GetExpansionJointDia(TRUE, 1, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dSttWeight[1]),
					pBMOpt->GetExpansionJointDia(FALSE, 1, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dEndWeight[1]));		Arr.Add(sText);
		}
		else
		{
			sText.Format("                    - �������� :    ������ ����   (tonf)\n");
			Arr.Add(sText);
			sText.Format("                    - ���� :    %s                         - ���� : %s\n", pBMOpt->m_structExpansionJoint.m_szSttName, pBMOpt->m_structExpansionJoint.m_szEndName);
			Arr.Add(sText);
			sText.Format("                                  %s         %.3f tonf                   %s        %.3f tonf\n",
					pBMOpt->GetExpansionJointDia(TRUE, 0, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dSttWeight[0]),
					pBMOpt->GetExpansionJointDia(FALSE, 0, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dEndWeight[0]));		Arr.Add(sText);
			sText.Format("                                  %s         %.3f tonf                   %s        %.3f tonf\n",
					pBMOpt->GetExpansionJointDia(TRUE, 1, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dSttWeight[1]),
					pBMOpt->GetExpansionJointDia(FALSE, 1, dSigmaY), toTon(pBMOpt->m_structExpansionJoint.m_dEndWeight[1]));		Arr.Add(sText);
		}

		sText.Format("                    - ����ġ�� : %.0f  mm\n", pBMOpt->m_structExpansionJoint.m_dMargin);			Arr.Add(sText);
		sText.Format("                    - ����ġ���� ���� : %d  EA\n", pBMOpt->m_structExpansionJoint.m_nMarginsu);	Arr.Add(sText);
	}

	sText.Format("     15. ������������   :    %s\n", IsApply(pBMOpt->m_structExpansionJointCover.m_bApply));			Arr.Add(sText);
	if(pBMOpt->m_structExpansionJointCover.m_bApply)
	{
		for(long n=0; n<7; n++)
		{
			sText.Format("           - %s ���� : %.3f m       %d  EA\n", pBMOpt->GetExpansionJointCoverPos(n),
				toM(pBMOpt->m_structExpansionJointCover.m_dHeight[n]), pBMOpt->m_structExpansionJointCover.m_nQty[n]);
			Arr.Add(sText);
		}
	}

	double	dExpJointSttHeight	= pGlopt->GetSttExpansionJointHeight();
	double	dExpJointSttWidth	= pGlopt->GetSttExpansionJointWidth();
	double	dExpJointEndHeight	= pGlopt->GetEndExpansionJointHeight();
	double	dExpJointEndWidth	= pGlopt->GetEndExpansionJointWidth();
	sText.Format("     16. ������ ��ũ��Ʈ :    %s\n", IsApply(pBMOpt->m_structShrinkageConcrete.m_bApply));			Arr.Add(sText);
	if(pBMOpt->m_structShrinkageConcrete.m_bApply)
	{
		sText.Format("            - �������� ���� : %.0f �� %.0f    %d EA\n", dExpJointSttWidth, dExpJointSttHeight, pBMOpt->m_structShrinkageConcrete.m_nSttQty);	Arr.Add(sText);
		sText.Format("            - �������� ���� : %.0f �� %.0f    %d EA\n", dExpJointEndWidth, dExpJointEndHeight, pBMOpt->m_structShrinkageConcrete.m_nEndQty);	Arr.Add(sText);
	}

	sText.Format("     17. ���� ������     :    %s\n", IsApply(pBMOpt->m_bWaterDraw));			Arr.Add(sText);

	sText.Format("     18. NOTCH          :    %s\n", IsApply(pBMOpt->m_structNotch.m_bApply));	Arr.Add(sText);
	if(pBMOpt->m_structNotch.m_bApply)
	{
		sText.Format("            - ������ġ      :    %s\n", pBMOpt->GetNotchPos(pBMOpt->m_structNotch.m_nApplyPos));		Arr.Add(sText);
		sText.Format("            - NOTCH ����    :    %s\n", pBMOpt->m_structNotch.m_szNotchType);		Arr.Add(sText);
	}

	if(pBMOpt->m_structGuardFence.m_bApply)	sText.Format("     19. �����潺         :    %.3f m\n", toM(pBMOpt->m_structGuardFence.m_dHeight));
	else									sText.Format("     19. �����潺         :    %s\n", IsApply(pBMOpt->m_structGuardFence.m_bApply));
	Arr.Add(sText);

	if(pBMOpt->m_structSoundProof.m_bApply)	
	{
		//������ �ֳ� ���� üũ....070111....KB...
		long nGuardTypeL = CalcFloor.GetGuardWallType(FLOOR_LEFT);
		long nGuardTypeR = CalcFloor.GetGuardWallType(FLOOR_RIGHT);
		BOOL bBangEmExistL = (pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && nGuardTypeL<=8) ? TRUE : FALSE;
		BOOL bBangEmExistR = (pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && nGuardTypeR<=8) ? TRUE : FALSE;
		 
		if(bBangEmExistL && bBangEmExistR)
			sText.Format("     20. �����ǳ�         :    %.3f m (��)	%.3f m (��)\n", toM(pBMOpt->m_structSoundProof.m_dLHeight), toM(pBMOpt->m_structSoundProof.m_dRHeight));
		else if(bBangEmExistL && !bBangEmExistR)
			sText.Format("     20. �����ǳ�         :    %.3f m (��)\n", toM(pBMOpt->m_structSoundProof.m_dLHeight));
		else if(!bBangEmExistL && bBangEmExistR)
			sText.Format("     20. �����ǳ�         :    %.3f m (��)\n", toM(pBMOpt->m_structSoundProof.m_dRHeight));
	}
	else									sText.Format("     20. �����ǳ�         :    %s\n", IsApply(pBMOpt->m_structSoundProof.m_bApply));
	Arr.Add(sText);

	sText.Format("     21. ��  ��             :    %s\n", IsApply(pBMOpt->m_structParapet.m_bApply));			Arr.Add(sText);
	if(pBMOpt->m_structParapet.m_bApply)
	{
		sText.Format("       - %s     :    %s    %s    %.3f m\n",
			pBMOpt->GetParapetType(DRIVEWAY), IsApplyLeft(pBMOpt->m_structParapet.m_bLeft[DRIVEWAY]),
			IsApplyRight(pBMOpt->m_structParapet.m_bRight[DRIVEWAY]), toM(pBMOpt->m_structParapet.m_dHeight[DRIVEWAY]));	Arr.Add(sText);	
		sText.Format("       - %s     :    %s    %s    %.3f m\n",
			pBMOpt->GetParapetType(FOOTWAY), IsApplyLeft(pBMOpt->m_structParapet.m_bLeft[FOOTWAY]),
			IsApplyRight(pBMOpt->m_structParapet.m_bRight[FOOTWAY]), toM(pBMOpt->m_structParapet.m_dHeight[FOOTWAY]));		Arr.Add(sText);	
		sText.Format("       - %s  :    %s    %s    %.3f m\n",
			pBMOpt->GetParapetType(DRIVE_FOOT_BOUNDARY), IsApplyLeft(pBMOpt->m_structParapet.m_bLeft[DRIVE_FOOT_BOUNDARY]),
			IsApplyRight(pBMOpt->m_structParapet.m_bRight[DRIVE_FOOT_BOUNDARY]), toM(pBMOpt->m_structParapet.m_dHeight[DRIVE_FOOT_BOUNDARY]));Arr.Add(sText);
	}

	sText.Format("     22. ������	      :    %s\n", IsApply(pBMOpt->m_bShade));							Arr.Add(sText);
	sText.Format("     23. ���Ϲ� ������  :    %s\n", IsApply(pBMOpt->m_structDroppingPrevent.m_bApply));	Arr.Add(sText);
	if(pBMOpt->m_structDroppingPrevent.m_bApply)
	{
		sText.Format("              - �� �� �� ġ : %s", pBMOpt->GetDroppingPrevent(pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal));Arr.Add(sText);
		sText.Format("              - ���������� : %.3f m\n", toM(pBMOpt->m_structDroppingPrevent.m_dHorSpaceHeight));Arr.Add(sText);
		if(!pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal)
		{
			sText.Format("              - ������������ : %.3f m\n", toM(pBMOpt->m_structDroppingPrevent.m_dVerSpaceHeight));
			Arr.Add(sText);
		}
	}

	sText.Format("     24. �������         :    %s\n", IsApply(pBMOpt->m_bSteelQuantity));					Arr.Add(sText);
	sText.Format("     25. ��������         :    %s\n", IsApply(pBMOpt->m_bPaint));							Arr.Add(sText);
	sText.Format("     26. ����ü�         :    %s\n", IsApply(pBMOpt->m_bDrainEstablish));	Arr.Add(sText);
	if(pBMOpt->m_bDrainEstablish && (pBMOpt->m_nDrainEstablishType == RIVER || pBMOpt->m_nDrainEstablishType == RIVERWALKWAY))
	{
		sText.Format("             �� ��   õ   ��\n");	Arr.Add(sText);
		sText.Format("              - ��   ��   �� (�����η��� Plate) : %d  EA\n", pBMOpt->m_structRiverDrainEstablish.m_nWaterCollect);	Arr.Add(sText);
		sText.Format("              - ��   ��   �� (�����η��� ����)  : %.3f  m\n", toM(pBMOpt->m_structRiverDrainEstablish.m_dDrain));			Arr.Add(sText);
	}
	if(pBMOpt->m_bDrainEstablish && (pBMOpt->m_nDrainEstablishType == WALKWAY || pBMOpt->m_nDrainEstablishType == RIVERWALKWAY))
	{
		if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)
		{
			sText.Format("             �� ��   ��   �� TYPE 1 (�Ǳ���)\n");	Arr.Add(sText);
			sText.Format("              - ��   ��   ��    : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);			Arr.Add(sText);
			sText.Format("              - ����������    : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinWaterCollect);		Arr.Add(sText);
			sText.Format("              - ��        ��    : %.3f  m\n", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));	Arr.Add(sText);
			sText.Format("              - ��        ��    : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);			Arr.Add(sText);
			sText.Format("              - ��   ��   ��    : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nJoint);					Arr.Add(sText);
		}
		else
		{
			sText.Format("             �� ��   ��   �� TYPE 2\n");	Arr.Add(sText);
			sText.Format("              - ��  ��  �� (�����η��� ����) : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);			Arr.Add(sText);
			sText.Format("              - ��  ��  �� (�����η��� ����) : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nDrain);					Arr.Add(sText);
			sText.Format("              - �������� (8"" �� 10"")     : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinDrain);				Arr.Add(sText);
			sText.Format("              - ��      �� (150A)            : %.3f  m\n", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));	Arr.Add(sText);
			sText.Format("              - ��      �� (200A)            : %.3f  m\n", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[1]));	Arr.Add(sText);
			sText.Format("              - ��      �� (150A)            : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);			Arr.Add(sText);
			sText.Format("              - ��      �� (200A)            : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[1]);			Arr.Add(sText);
			sText.Format("              - û  ��  �� (150A)            : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[0]);			Arr.Add(sText);
			sText.Format("              - û  ��  �� (200A)            : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[1]);			Arr.Add(sText);
			sText.Format("              - ��ΰ����� (150A)         : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[0]);			Arr.Add(sText);
			sText.Format("              - ��ΰ����� (200A)         : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[1]);			Arr.Add(sText);
			sText.Format("              - ħ  ��  ��                      : %d  EA\n", pBMOpt->m_structWalkWayDrainEstablish.m_nCollecting);				Arr.Add(sText);
		}
	}

	sText.Format("     27. �� �� ��          :    %s\n", IsApply(pBMOpt->m_structWingWall.m_bApply));	Arr.Add(sText);
	if(pBMOpt->m_structWingWall.m_bApply)
	{
		sText.Format("             - ���� => ���� : %.3f m    ���� : %.3f m\n", toM(pBMOpt->m_structWingWall.m_dLengthLeftStt), toM(pBMOpt->m_structWingWall.m_dLengthLeftEnd));Arr.Add(sText);
		sText.Format("             - ���� => ���� : %.3f m    ���� : %.3f m\n", toM(pBMOpt->m_structWingWall.m_dLengthRighStt), toM(pBMOpt->m_structWingWall.m_dLengthRighEnd));Arr.Add(sText);
	}

	sText.Format("     28. �� �� ��          :    %s\n", IsApply(pBMOpt->m_structTimber.m_bApply));	Arr.Add(sText);
	if(pBMOpt->m_structTimber.m_bApply)
	{
		sText.Format("             - �� �� ��      ����    :   %s\n", pBMOpt->GetTimberQuality(pBMOpt->m_structTimber.m_nQuality));Arr.Add(sText);
		sText.Format("             - ��ũ�Ǵϼ�   ��ġ   :   %s\n", pBMOpt->GetPosDeckFinisher(pBMOpt->m_structTimber.m_nPosDeckFinisher));Arr.Add(sText);
		sText.Format("             - ��ũ�Ǵϼ� �۾���  :   %.3f  m\n", toM(pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth));Arr.Add(sText);
	}

	sText.Format("     29. �� �� ��          :    %s\n", IsApply(pBMOpt->m_structElecWireHole.m_bApply));	Arr.Add(sText);
	if(pBMOpt->m_structElecWireHole.m_bApply)
	{
		sText.Format("             - ��100  =>   ����   :   %d EA	����   :   %d EA\n", pBMOpt->m_structElecWireHole.m_nEA100[0], pBMOpt->m_structElecWireHole.m_nEA100[1]);Arr.Add(sText);
		sText.Format("             - ��125  =>   ����   :   %d EA	����   :   %d EA\n", pBMOpt->m_structElecWireHole.m_nEA125[0], pBMOpt->m_structElecWireHole.m_nEA125[1]);Arr.Add(sText);
		sText.Format("             - ��150  =>   ����   :   %d EA	����   :   %d EA\n", pBMOpt->m_structElecWireHole.m_nEA150[0], pBMOpt->m_structElecWireHole.m_nEA150[1]);Arr.Add(sText);
	}

}

void CDeckBMOptionDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData		*pADeckData	= m_pStd->m_pDeckData;
	CBMOption		*pBMOpt		= &pADeckData->m_BMOption;	

	if(m_nSelectedOpt == BMOPT_CONCRETE_BYE)		pBMOpt->m_bConcreteBuy		= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_CONCRETE_PLACING)	pBMOpt->m_bConcretePlacing	= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_MOLD)				pBMOpt->m_bMold				= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_SPACER)				pBMOpt->m_bSpacer			= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_REBAR_MANUFACTURE)
	{
		pBMOpt->m_structRebarManufacture.m_bApply		= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structRebarManufacture.m_nSlabType	= pBMOpt->GetIdxRebarManufactureType(m_Grid.GetTextMatrix(2, 1));
		long nRow = 3;
		if(pDB->GetGuardWallSu() > 0)
			pBMOpt->m_structRebarManufacture.m_nGuardFenceType	= pBMOpt->GetIdxRebarManufactureType(m_Grid.GetTextMatrix(nRow++, 1));
		if(pBMOpt->m_structExpansionJoint.m_bApply)
			pBMOpt->m_structRebarManufacture.m_nExpansionJointType	= pBMOpt->GetIdxRebarManufactureType(m_Grid.GetTextMatrix(nRow++, 1));
		if(pDB->IsTUGir())
			pBMOpt->m_structRebarManufacture.m_nBindConcType	= pBMOpt->GetIdxRebarManufactureType(m_Grid.GetTextMatrix(nRow++, 1));
		OnCheckRebarManufacture();
	}

	if(m_nSelectedOpt == BMOPT_PAVE)
	{
		pBMOpt->m_structPave.m_bApply					= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structPave.m_dPaveThick				= m_Grid.GetTextMatrixDouble(1, 2);
		OnCheckPave();
	}
	if(m_nSelectedOpt == BMOPT_TACKCOATING)
	{
		pBMOpt->m_structTackCoating.m_bApply			= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structTackCoating.m_dTackCoating		= m_Grid.GetTextMatrixDouble(1, 2);
		OnCheckTackCoating();
	}
	if(m_nSelectedOpt == BMOPT_DRAINBRIDGESURFACE)
		pBMOpt->m_bDrainBridgeSurface					= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_BRIDGENAME)
	{
		pBMOpt->m_structBridgeName.m_bApply				= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structBridgeName.m_nQty				= m_Grid.GetTextMatrixLong(2, 1);
		OnCheckBridgeName();
	}
	if(m_nSelectedOpt == BMOPT_BRIDGENAME_PLATE)
	{
		pBMOpt->m_structBridgeNamePlate.m_bApply		= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structBridgeNamePlate.m_nQty			= m_Grid.GetTextMatrixLong(2, 1);
		OnCheckBridgeNameplate();
	}
	if(m_nSelectedOpt == BMOPT_EXPLAIN_PLATE)
	{
		pBMOpt->m_structExplainPlate.m_bApply			= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structExplainPlate.m_nQty				= m_Grid.GetTextMatrixLong(2, 1);
		OnCheckExplainPlate();
	}
	if(m_nSelectedOpt == BMOPT_ESTABLISH_TBM)
	{
		pBMOpt->m_structEstablishTBM.m_bApply			= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structEstablishTBM.m_nQty				= m_Grid.GetTextMatrixLong(2, 1);
		OnCheckEstablishTbm();
	}
	if(m_nSelectedOpt == BMOPT_STYROFOAM)
	{
		pBMOpt->m_structStyrofoam.m_bApply				= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structStyrofoam.m_bLeft				= IsLeft(m_Grid.GetTextMatrix(2, 1));
		pBMOpt->m_structStyrofoam.m_dThick				= m_Grid.GetTextMatrixDouble(3, 1);
		OnCheckStyrofoam();
	}
	if(m_nSelectedOpt == BMOPT_EXPANSIONJOINT)
	{
		pBMOpt->m_structExpansionJoint.m_bApply			= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structExpansionJoint.m_bUnitLength	= IsApply(m_Grid.GetTextMatrix(2, 1));
		pBMOpt->m_structExpansionJoint.m_szSttName		= m_Grid.GetTextMatrix(3, 1);
		pBMOpt->m_structExpansionJoint.m_nSttDiaIdx[0]	= pBMOpt->GetRebarDiaIndex(m_Grid.GetTextMatrix(4, 1));
		pBMOpt->m_structExpansionJoint.m_dSttWeight[0]	= pBMOpt->m_structExpansionJoint.m_bUnitLength ? m_Grid.GetTextMatrixDouble(4, 2) : frTon(m_Grid.GetTextMatrixDouble(4, 2));
		pBMOpt->m_structExpansionJoint.m_nSttDiaIdx[1]	= pBMOpt->GetRebarDiaIndex(m_Grid.GetTextMatrix(5, 1));
		pBMOpt->m_structExpansionJoint.m_dSttWeight[1]	= pBMOpt->m_structExpansionJoint.m_bUnitLength ? m_Grid.GetTextMatrixDouble(5, 2) : frTon(m_Grid.GetTextMatrixDouble(5, 2));

		pBMOpt->m_structExpansionJoint.m_szEndName		= m_Grid.GetTextMatrix(6, 1);
		pBMOpt->m_structExpansionJoint.m_nEndDiaIdx[0]	= pBMOpt->GetRebarDiaIndex(m_Grid.GetTextMatrix(7, 1));
		pBMOpt->m_structExpansionJoint.m_dEndWeight[0]	= pBMOpt->m_structExpansionJoint.m_bUnitLength ? m_Grid.GetTextMatrixDouble(7, 2) : frTon(m_Grid.GetTextMatrixDouble(7, 2));
		pBMOpt->m_structExpansionJoint.m_nEndDiaIdx[1]	= pBMOpt->GetRebarDiaIndex(m_Grid.GetTextMatrix(8, 1));
		pBMOpt->m_structExpansionJoint.m_dEndWeight[1]	= pBMOpt->m_structExpansionJoint.m_bUnitLength ? m_Grid.GetTextMatrixDouble(8, 2) : frTon(m_Grid.GetTextMatrixDouble(8, 2));
		pBMOpt->m_structExpansionJoint.m_dMargin		= m_Grid.GetTextMatrixDouble(9, 1);
		pBMOpt->m_structExpansionJoint.m_nMarginsu		= m_Grid.GetTextMatrixLong(10, 1);
		OnCheckExpansionjoint();
	}
	if(m_nSelectedOpt == BMOPT_EXPANSIONJOINT_COVER)
	{
		pBMOpt->m_structExpansionJointCover.m_bApply			= IsApply(m_Grid.GetTextMatrix(1, 1));

		for(long nRow = 2; nRow < 9; nRow++)
		{
			pBMOpt->m_structExpansionJointCover.m_dHeight[nRow-2]	= frM(m_Grid.GetTextMatrixDouble(nRow, 1));
			pBMOpt->m_structExpansionJointCover.m_nQty[nRow-2]		= m_Grid.GetTextMatrixLong(nRow, 3);
		}

		OnCheckExpansionjointCover(); 
	}
	if(m_nSelectedOpt == BMOPT_SHRINKAGE_CONCRETE)
	{
		pBMOpt->m_structShrinkageConcrete.m_bApply	= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structShrinkageConcrete.m_nSttQty	= m_Grid.GetTextMatrixLong(2, 2);
		pBMOpt->m_structShrinkageConcrete.m_nEndQty	= m_Grid.GetTextMatrixLong(3, 2);
		OnCheckShrinkageConcrete();
	}

	if(m_nSelectedOpt == BMOPT_WATER_DRAW)			pBMOpt->m_bWaterDraw	= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_NOTCH)
	{
		pBMOpt->m_structNotch.m_bApply					= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structNotch.m_nApplyPos				= pBMOpt->GetNotchPos(m_Grid.GetTextMatrix(2, 1));
		pBMOpt->m_structNotch.m_szNotchType				= m_Grid.GetTextMatrix(3, 1);
		OnCheckNotch();
	}

	if(m_nSelectedOpt == BMOPT_GUARD_FENCE)
	{
		pBMOpt->m_structGuardFence.m_bApply				= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structGuardFence.m_dHeight			= frM(m_Grid.GetTextMatrixDouble(1, 2));
		OnCheckGuardFence();
	}
	if(m_nSelectedOpt == BMOPT_SOUND_PROOF)
	{
		pBMOpt->m_structSoundProof.m_bApply				= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structSoundProof.m_dLHeight			= frM(m_Grid.GetTextMatrixDouble(2, 1));
		pBMOpt->m_structSoundProof.m_dRHeight			= frM(m_Grid.GetTextMatrixDouble(3, 1));
		OnCheckSoundProof();
	}
	if(m_nSelectedOpt == BMOPT_PARAPET)
	{
		pBMOpt->m_structParapet.m_bApply				= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structParapet.m_dGuardLength[0]		= frM(m_Grid.GetTextMatrixDouble(2, 2));
		pBMOpt->m_structParapet.m_dGuardLength[1]		= frM(m_Grid.GetTextMatrixDouble(3, 2));
		pBMOpt->m_structParapet.m_dCurbLength			= frM(m_Grid.GetTextMatrixDouble(4, 2));
		pBMOpt->m_structParapet.m_nCurbQty				= m_Grid.GetTextMatrixLong(5, 2);

		OnCheckParapet();
	}
	if(m_nSelectedOpt == BMOPT_SHADE)				pBMOpt->m_bShade			= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_DROPPING_PREVENT)
	{
		pBMOpt->m_structDroppingPrevent.m_bApply	= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal	= pBMOpt->GetDroppingPrevent(m_Grid.GetTextMatrix(2, 1));
		pBMOpt->m_structDroppingPrevent.m_dHorSpaceHeight	= frM(m_Grid.GetTextMatrixDouble(3, 1));
		if(!pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal)
			pBMOpt->m_structDroppingPrevent.m_dVerSpaceHeight = frM(m_Grid.GetTextMatrixDouble(4, 1));
		OnCheckDroppingPrevent();
	}
	if(m_nSelectedOpt == BMOPT_STEEL_QUANTITY)		pBMOpt->m_bSteelQuantity	= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_PAINT)				pBMOpt->m_bPaint			= IsApply(m_Grid.GetTextMatrix(1, 1));
	if(m_nSelectedOpt == BMOPT_DRAIN_ESTABLISH)
	{
		pBMOpt->m_bDrainEstablish		= IsApply(m_Grid.GetTextMatrix(1, 1));
		if(pBMOpt->m_nDrainEstablishType == RIVER)
		{
			pBMOpt->m_structRiverDrainEstablish.m_nWaterCollect			= m_Grid.GetTextMatrixLong(3, 2);		// ������
			pBMOpt->m_structRiverDrainEstablish.m_dDrain				= frM(m_Grid.GetTextMatrixDouble(4, 2));		// �����		
		}
		else if(pBMOpt->m_nDrainEstablishType == WALKWAY || pBMOpt->m_nDrainEstablishType == RIVERWALKWAY)
		{
			if(m_Grid.GetTextMatrix(3, 1) == _T("TYPE 1 (�Ǳ���)"))
				pBMOpt->m_structWalkWayDrainEstablish.m_nType = 0;
			else
				pBMOpt->m_structWalkWayDrainEstablish.m_nType = 1;
			if(pBMOpt->m_nDrainEstablishType == WALKWAY && pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)
			{
				pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect		= m_Grid.GetTextMatrixLong(4, 2);			// ������
				pBMOpt->m_structWalkWayDrainEstablish.m_nJoinWaterCollect	= m_Grid.GetTextMatrixLong(5, 2);			// ����������
				pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]	= frM(m_Grid.GetTextMatrixDouble(6, 2));	// ����
				pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]		= m_Grid.GetTextMatrixLong(7, 2);	// ���
				pBMOpt->m_structWalkWayDrainEstablish.m_nJoint				= m_Grid.GetTextMatrixLong(8, 2);			// �����
			}
			else if(pBMOpt->m_nDrainEstablishType == WALKWAY && pBMOpt->m_structWalkWayDrainEstablish.m_nType == 1)
			{
				pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect		= m_Grid.GetTextMatrixLong(4, 2);			// ������
				pBMOpt->m_structWalkWayDrainEstablish.m_nDrain				= m_Grid.GetTextMatrixLong(5, 2);			// �����
				pBMOpt->m_structWalkWayDrainEstablish.m_nJoinDrain			= m_Grid.GetTextMatrixLong(6, 2);			// ��������
				pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]	= frM(m_Grid.GetTextMatrixDouble(7, 2));	// ����
				pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[1]	= frM(m_Grid.GetTextMatrixDouble(8, 2));	// ����
				pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]		= m_Grid.GetTextMatrixLong(9, 2);			// ���
				pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[1]		= m_Grid.GetTextMatrixLong(10, 2);			// ���
				pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[0]		= m_Grid.GetTextMatrixLong(11, 2);			// û�ұ�
				pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[1]		= m_Grid.GetTextMatrixLong(12, 2);			// û�ұ�
				pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[0]		= m_Grid.GetTextMatrixLong(13, 2);			// ��ΰ�����
				pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[1]		= m_Grid.GetTextMatrixLong(14, 2);			// ��ΰ�����
				pBMOpt->m_structWalkWayDrainEstablish.m_nCollecting			= m_Grid.GetTextMatrixLong(15, 2);			// ħ����
			}
			else if(pBMOpt->m_nDrainEstablishType == RIVERWALKWAY)
			{
				pBMOpt->m_structRiverDrainEstablish.m_nWaterCollect		= m_Grid.GetTextMatrixLong(4, 2);				// ������
				pBMOpt->m_structRiverDrainEstablish.m_dDrain			= frM(m_Grid.GetTextMatrixDouble(5, 2));		// �����
				if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)
				{
					pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect		= m_Grid.GetTextMatrixLong(6, 2);		// ������
					pBMOpt->m_structWalkWayDrainEstablish.m_nJoinWaterCollect	= m_Grid.GetTextMatrixLong(7, 2);		// ����������
					pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]	= frM(m_Grid.GetTextMatrixDouble(8, 2));// ����
					pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]		= m_Grid.GetTextMatrixLong(9, 2);		// ���
					pBMOpt->m_structWalkWayDrainEstablish.m_nJoint				= m_Grid.GetTextMatrixLong(10, 2);		// �����				
				}
				else
				{
					pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect		= m_Grid.GetTextMatrixLong(6, 2);		// ������
					pBMOpt->m_structWalkWayDrainEstablish.m_nDrain				= m_Grid.GetTextMatrixLong(7, 2);		// �����
					pBMOpt->m_structWalkWayDrainEstablish.m_nJoinDrain			= m_Grid.GetTextMatrixLong(8, 2);		// ��������
					pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]	= frM(m_Grid.GetTextMatrixDouble(9, 2));// ����
					pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[1]	= frM(m_Grid.GetTextMatrixDouble(10, 2));// ����
					pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]		= m_Grid.GetTextMatrixLong(11, 2);		// ���
					pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[1]		= m_Grid.GetTextMatrixLong(12, 2);		// ���
					pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[0]		= m_Grid.GetTextMatrixLong(13, 2);		// û�ұ�
					pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[1]		= m_Grid.GetTextMatrixLong(14, 2);		// û�ұ�
					pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[0]		= m_Grid.GetTextMatrixLong(15, 2);		// ��ΰ�����
					pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[1]		= m_Grid.GetTextMatrixLong(16, 2);		// ��ΰ�����
					pBMOpt->m_structWalkWayDrainEstablish.m_nCollecting			= m_Grid.GetTextMatrixLong(17, 2);		// ħ����
				}
			}
		}
		pBMOpt->m_nDrainEstablishType	= pBMOpt->GetIdxDrainEstablishType(m_Grid.GetTextMatrix(2, 1));
		OnCheckDrainEstablish();
	}
	if(m_nSelectedOpt == BMOPT_WING_WALL)
	{
		pBMOpt->m_structWingWall.m_bApply				= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structWingWall.m_dLengthLeftStt		= frM(m_Grid.GetTextMatrixDouble(2, 2));
		pBMOpt->m_structWingWall.m_dLengthLeftEnd		= frM(m_Grid.GetTextMatrixDouble(3, 2));
		pBMOpt->m_structWingWall.m_dLengthRighStt		= frM(m_Grid.GetTextMatrixDouble(4, 2));
		pBMOpt->m_structWingWall.m_dLengthRighEnd		= frM(m_Grid.GetTextMatrixDouble(5, 2));
		OnCheckWingWall();
	}
	if(m_nSelectedOpt == BMOPT_TIMBER)
	{
		pBMOpt->m_structTimber.m_bApply					= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structTimber.m_nQuality				= pBMOpt->GetIdxTimberQuality(m_Grid.GetTextMatrix(2, 1));
		pBMOpt->m_structTimber.m_nPosDeckFinisher		= pBMOpt->GetIdxDeckFinisher(m_Grid.GetTextMatrix(3, 1));
		pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth	= frM(m_Grid.GetTextMatrixDouble(4, 1));
		OnCheckTimber();
	}
	if(m_nSelectedOpt == BMOPT_ELECWIREHOLE)
	{
		pBMOpt->m_structElecWireHole.m_bApply			= IsApply(m_Grid.GetTextMatrix(1, 1));
		pBMOpt->m_structElecWireHole.m_nEA100[0]		= m_Grid.GetTextMatrixLong(2, 2);
		pBMOpt->m_structElecWireHole.m_nEA100[1]		= m_Grid.GetTextMatrixLong(3, 2);
		pBMOpt->m_structElecWireHole.m_nEA125[0]		= m_Grid.GetTextMatrixLong(4, 2);
		pBMOpt->m_structElecWireHole.m_nEA125[1]		= m_Grid.GetTextMatrixLong(5, 2);
		pBMOpt->m_structElecWireHole.m_nEA150[0]		= m_Grid.GetTextMatrixLong(6, 2);
		pBMOpt->m_structElecWireHole.m_nEA150[1]		= m_Grid.GetTextMatrixLong(7, 2);
		OnCheckElecWireHole();
	}

	SetDataInit();
}

// ��ũ��Ʈ ����
void CDeckBMOptionDlg::OnCheckConcreteBuy() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_CONCRETE_BYE;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bConcreteBuy));
	m_ConcreteBuy.SetCheck(pBMOpt->m_bConcreteBuy);
	m_ConcreteBuy.Invalidate();
	m_Grid.SetRedraw(TRUE,TRUE);
}

// ��ũ��Ʈ Ÿ��
void CDeckBMOptionDlg::OnCheckConcretePlacing() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_CONCRETE_PLACING;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bConcretePlacing));
	m_ConcretePlacing.SetCheck(pBMOpt->m_bConcretePlacing);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ��Ǫ��
void CDeckBMOptionDlg::OnCheckMold() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_MOLD;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bMold));
	m_Mold.SetCheck(pBMOpt->m_bMold);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// �����̼�
void CDeckBMOptionDlg::OnCheckSpacer() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_SPACER;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bSpacer));
	m_Spacer.SetCheck(pBMOpt->m_bSpacer);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ö�ٰ�������
void CDeckBMOptionDlg::OnCheckRebarManufacture() 
{
	CPlateBridgeApp *pDB		= m_pStd->m_pDataManage->GetBridge();	
	CADeckData		*pADeckData	= m_pStd->m_pDeckData;
	CBMOption		*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_REBAR_MANUFACTURE;
	m_nCols			= 2;
	m_nRows			= 3;
	BOOL	bGuard	= FALSE;
	BOOL	bExp	= FALSE;
	BOOL	bTUGir	= pDB->IsTUGir();
	if(pDB->GetGuardWallSu()>0)					bGuard	= TRUE;
	if(pBMOpt->m_structExpansionJoint.m_bApply)	bExp	= TRUE;
	if(bGuard)	m_nRows++;
	if(bExp)	m_nRows++;
	if(bTUGir)	m_nRows++;

	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetCellType(2, 1, CT_COMBO);

	long nRow = 3;
	if(bGuard)	m_Grid.SetCellType(nRow++, 1, CT_COMBO);
	if(bExp)	m_Grid.SetCellType(nRow++, 1, CT_COMBO);
	if(bTUGir)	m_Grid.SetCellType(nRow++, 1, CT_COMBO);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structRebarManufacture.m_bApply));
	m_Grid.SetTextMatrix(2, 0, "�� �� ��");
	nRow = 3;
	if(bGuard)	m_Grid.SetTextMatrix(nRow++, 0, "�� ȣ ��");
	if(bExp)	m_Grid.SetTextMatrix(nRow++, 0, "��������");
	if(bTUGir)	m_Grid.SetTextMatrix(nRow++, 0, "������ũ��Ʈ");

	m_Grid.SetTextMatrix(2, 1, pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nSlabType));
	nRow = 3;
	if(bGuard)
		m_Grid.SetTextMatrix(nRow++, 1, pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nGuardFenceType));
	if(bExp)
		m_Grid.SetTextMatrix(nRow++, 1, pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nExpansionJointType));
	if(bTUGir)
		m_Grid.SetTextMatrix(nRow++, 1, pBMOpt->GetRebarManufactureType(pBMOpt->m_structRebarManufacture.m_nBindConcType));

	if(!pBMOpt->m_structRebarManufacture.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);
		m_Grid.SetItemBkColour(2, 1, RGBREADONLY);

		nRow = 3;
		if(bGuard)
		{
			m_Grid.SetItemState(nRow, 1, m_Grid.GetItemState(nRow, 1) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow++, 1, RGBREADONLY);
		}
		if(bExp)
		{
			m_Grid.SetItemState(nRow, 1, m_Grid.GetItemState(nRow, 1) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow++, 1, RGBREADONLY);
		}
		if(bTUGir)
		{
			m_Grid.SetItemState(nRow, 1, m_Grid.GetItemState(nRow, 1) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow++, 1, RGBREADONLY);
		}
	}

	m_RebarManufacture.SetCheck(pBMOpt->m_structRebarManufacture.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ����
void CDeckBMOptionDlg::OnCheckPave() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_PAVE;
	m_nCols			= 4;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 70);
	m_Grid.SetColumnWidth(2, 70);
	m_Grid.SetColumnWidth(3, 70);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(0, 2, "�β�");
	m_Grid.SetTextMatrix(0, 3, "����");
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structPave.m_bApply));
	m_Grid.SetTextMatrix(1, 2, "%.0f", pBMOpt->m_structPave.m_dPaveThick);
	m_Grid.SetTextMatrix(1, 3, "mm");
	if(!pBMOpt->m_structPave.m_bApply)
	{
		m_Grid.SetItemState(1, 2, m_Grid.GetItemState(1, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(1, 2, RGBREADONLY);
		m_Grid.SetItemState(1, 3, m_Grid.GetItemState(1, 3) | GVIS_READONLY);	m_Grid.SetItemBkColour(1, 3, RGBREADONLY);
	}
	m_Pave.SetCheck(pBMOpt->m_structPave.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ������
void CDeckBMOptionDlg::OnCheckTackCoating() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_TACKCOATING;
	m_nCols			= 4;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 70);
	m_Grid.SetColumnWidth(2, 70);
	m_Grid.SetColumnWidth(3, 70);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(0, 2, "����");
	m_Grid.SetTextMatrix(0, 3, "����");
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structTackCoating.m_bApply));
	m_Grid.SetTextMatrix(1, 2, "%.1f", pBMOpt->m_structTackCoating.m_dTackCoating);
	m_Grid.SetTextMatrix(1, 3, "��");

	if(!pBMOpt->m_structTackCoating.m_bApply)
	{
		m_Grid.SetItemState(1, 2, m_Grid.GetItemState(1, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(1, 2, RGBREADONLY);
		m_Grid.SetItemState(1, 3, m_Grid.GetItemState(1, 3) | GVIS_READONLY);	m_Grid.SetItemBkColour(1, 3, RGBREADONLY);
	}

	m_TackCoating.SetCheck(pBMOpt->m_structTackCoating.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ������
void CDeckBMOptionDlg::OnCheckDrainBridgesurface() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_DRAINBRIDGESURFACE;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bDrainBridgeSurface));
	m_DrainBridgeSurface.SetCheck(pBMOpt->m_bDrainBridgeSurface);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ������
void CDeckBMOptionDlg::OnCheckBridgeName() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	m_nSelectedOpt	= BMOPT_BRIDGENAME;
	m_nCols			= 3;
	m_nRows			= 3;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 180);
	m_Grid.SetColumnWidth(2, 60);

	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structBridgeName.m_bApply));

	m_Grid.SetTextMatrix(2, 0, "��ġ����");
	sText.Format("%d", pBMOpt->m_structBridgeName.m_nQty);
	m_Grid.SetTextMatrix(2, 1, sText);
	m_Grid.SetTextMatrix(2, 2, "EA");

	if(!pBMOpt->m_structBridgeName.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(2, 2, m_Grid.GetItemState(2, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 2, RGBREADONLY);
	}
	m_BridgeName.SetCheck(pBMOpt->m_structBridgeName.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ������
void CDeckBMOptionDlg::OnCheckBridgeNameplate() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	m_nSelectedOpt	= BMOPT_BRIDGENAME_PLATE;
	m_nCols			= 3;
	m_nRows			= 3;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 180);
	m_Grid.SetColumnWidth(2, 60);

	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structBridgeNamePlate.m_bApply));
	m_Grid.SetTextMatrix(2, 0, "��ġ����");
	sText.Format("%d", pBMOpt->m_structBridgeNamePlate.m_nQty);
	m_Grid.SetTextMatrix(2, 1, sText);
	m_Grid.SetTextMatrix(2, 2, "EA");

	if(!pBMOpt->m_structBridgeNamePlate.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(2, 2, m_Grid.GetItemState(2, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 2, RGBREADONLY);
	}
	m_BridgeNamePlate.SetCheck(pBMOpt->m_structBridgeNamePlate.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ������
void CDeckBMOptionDlg::OnCheckExplainPlate() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	m_nSelectedOpt	= BMOPT_EXPLAIN_PLATE;
	m_nCols			= 3;
	m_nRows			= 3;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 180);
	m_Grid.SetColumnWidth(2, 60);

	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structExplainPlate.m_bApply));
	m_Grid.SetTextMatrix(2, 0, "��ġ����");
	sText.Format("%d", pBMOpt->m_structExplainPlate.m_nQty);
	m_Grid.SetTextMatrix(2, 1, sText);
	m_Grid.SetTextMatrix(2, 2, "EA");

	if(!pBMOpt->m_structExplainPlate.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(2, 2, m_Grid.GetItemState(2, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 2, RGBREADONLY);
	}
	m_ExplainPlate.SetCheck(pBMOpt->m_structExplainPlate.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// TBM ��ġ
void CDeckBMOptionDlg::OnCheckEstablishTbm() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	m_nSelectedOpt	= BMOPT_ESTABLISH_TBM;
	m_nCols			= 3;
	m_nRows			= 3;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 180);
	m_Grid.SetColumnWidth(2, 60);

	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structEstablishTBM.m_bApply));
	m_Grid.SetTextMatrix(2, 0, "��ġ����");
	sText.Format("%d", pBMOpt->m_structEstablishTBM.m_nQty);
	m_Grid.SetTextMatrix(2, 1, sText);
	m_Grid.SetTextMatrix(2, 2, "EA");

	if(!pBMOpt->m_structEstablishTBM.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(2, 2, m_Grid.GetItemState(2, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 2, RGBREADONLY);
	}
	m_EstablishTBM.SetCheck(pBMOpt->m_structEstablishTBM.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ��Ƽ����
void CDeckBMOptionDlg::OnCheckStyrofoam() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_STYROFOAM;
	m_nCols			= 3;
	m_nRows			= 4;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 140);
	m_Grid.SetColumnWidth(2, 70);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetCellType(2, 1, CT_COMBO);
	m_Grid.SetCellType(3, 1, CT_COMBO);
	m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetMergeCol(2, 1, 2);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(2, 0, "��     ġ");
	m_Grid.SetTextMatrix(3, 0, "��     ��");
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structStyrofoam.m_bApply));
	if(pBMOpt->m_structStyrofoam.m_bLeft)	m_Grid.SetTextMatrix(2, 1, "��������");
	else									m_Grid.SetTextMatrix(2, 1, "��������");
	m_Grid.SetTextMatrix(3, 1, "%.0f", pBMOpt->m_structStyrofoam.m_dThick);
	m_Grid.SetTextMatrix(3, 2, "mm");

	if(!pBMOpt->m_structStyrofoam.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemState(3, 2, m_Grid.GetItemState(3, 2) | GVIS_READONLY);		m_Grid.SetItemBkColour(3, 2, RGBREADONLY);
	}

	m_Styrofoam.SetCheck(pBMOpt->m_structStyrofoam.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ��������
void CDeckBMOptionDlg::OnCheckExpansionjoint() 
{
	CADeckData		*pADeckData	= m_pStd->m_pDeckData;
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CBMOption		*pBMOpt		= &pADeckData->m_BMOption;

	BOOL bWalkRoad	= FALSE;
	for(long nHDan=0; nHDan<pDB->GetQtyHDanNode(); nHDan++)
	{
		if(pDB->GetValueTypeHDan(nHDan, 1)==3)	bWalkRoad	= TRUE;
	}

	m_nSelectedOpt	= BMOPT_EXPANSIONJOINT;
	m_nCols			= 4;
	m_nRows			= 12;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 85);
	m_Grid.SetColumnWidth(2, 85);
	m_Grid.SetColumnWidth(3, 85);
	m_Grid.SetMergeCol(0, 1, 3);
	m_Grid.SetMergeCol(1, 1, 3);
	m_Grid.SetMergeCol(2, 1, 3);
	m_Grid.SetMergeCol(3, 1, 3);
	m_Grid.SetMergeRow(4, 6, 0);
	m_Grid.SetMergeRow(7, 9, 0);
	m_Grid.SetMergeCol(4, 1, 3);
	m_Grid.SetMergeCol(7, 1, 3);
	m_Grid.SetMergeCol(10, 1, 2);
	m_Grid.SetMergeCol(11, 1, 2);
	
	m_Grid.SetCellType(1, 1, CT_COMBO);
//	m_Grid.SetCellType(2, 1, CT_COMBO);
	m_Grid.SetCellType(3, 1, CT_COMBO);
	m_Grid.SetCellType(5, 1, CT_COMBO);
	m_Grid.SetCellType(6, 1, CT_COMBO);
	m_Grid.SetCellType(8, 1, CT_COMBO);
	m_Grid.SetCellType(9, 1, CT_COMBO);

	m_Grid.SetTextMatrix(2, 0, "��������");
	m_Grid.SetTextMatrix(3, 0, "������������");
	m_Grid.SetTextMatrix(4, 0, "����");
	m_Grid.SetTextMatrix(7, 0, "����");
	m_Grid.SetTextMatrix(10, 0, "����ġ��");
	m_Grid.SetTextMatrix(11, 0, "����ġ������");

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structExpansionJoint.m_bApply));
	if(pBMOpt->m_structExpansionJoint.m_bUnitLength)	m_Grid.SetTextMatrix(2, 1, "���� m�� ����");
	else												m_Grid.SetTextMatrix(2, 1, "������ ����");

	if(!bWalkRoad)
	{
		pBMOpt->m_structExpansionJoint.m_bIncludeWalkLoad	= FALSE;
		m_Grid.SetRowHeight(3, 0);
	}
	m_Grid.SetTextMatrix(3, 1, IsApply(pBMOpt->m_structExpansionJoint.m_bIncludeWalkLoad));
	m_Grid.SetTextMatrix(4, 1, pBMOpt->m_structExpansionJoint.m_szSttName);
	m_Grid.SetTextMatrix(5, 1, pBMOpt->GetRebarDia(pBMOpt->m_structExpansionJoint.m_nSttDiaIdx[0], toKgPCm2(pADeckData->m_SigmaY)));
	if(pBMOpt->m_structExpansionJoint.m_bUnitLength)
	{
		m_Grid.SetTextMatrix(5, 2, "%.3f", pBMOpt->m_structExpansionJoint.m_dSttWeight[0]);
		m_Grid.SetTextMatrix(5, 3, "Kgf/m");
	}
	else
	{
		m_Grid.SetTextMatrix(5, 2, "%.3f", toTon(pBMOpt->m_structExpansionJoint.m_dSttWeight[0]));
		m_Grid.SetTextMatrix(5, 3, "tonf");
	}

	m_Grid.SetTextMatrix(6, 1, pBMOpt->GetRebarDia(pBMOpt->m_structExpansionJoint.m_nSttDiaIdx[1], toKgPCm2(pADeckData->m_SigmaY)));
	if(pBMOpt->m_structExpansionJoint.m_bUnitLength)
	{
		m_Grid.SetTextMatrix(6, 2, "%.3f", pBMOpt->m_structExpansionJoint.m_dSttWeight[1]);
		m_Grid.SetTextMatrix(6, 3, "Kgf/m");
	}
	else
	{
		m_Grid.SetTextMatrix(6, 2, "%.3f", toTon(pBMOpt->m_structExpansionJoint.m_dSttWeight[1]));
		m_Grid.SetTextMatrix(6, 3, "tonf");
	}

	m_Grid.SetTextMatrix(7, 1, pBMOpt->m_structExpansionJoint.m_szEndName);
	m_Grid.SetTextMatrix(8, 1, pBMOpt->GetRebarDia(pBMOpt->m_structExpansionJoint.m_nEndDiaIdx[0], toKgPCm2(pADeckData->m_SigmaY)));
	if(pBMOpt->m_structExpansionJoint.m_bUnitLength)
	{
		m_Grid.SetTextMatrix(8, 2, "%.3f", pBMOpt->m_structExpansionJoint.m_dEndWeight[0]);
		m_Grid.SetTextMatrix(8, 3, "Kgf/m");
	}
	else
	{
		m_Grid.SetTextMatrix(8, 2, "%.3f", toTon(pBMOpt->m_structExpansionJoint.m_dEndWeight[0]));
		m_Grid.SetTextMatrix(8, 3, "tonf");
	}

	m_Grid.SetTextMatrix(9, 1, pBMOpt->GetRebarDia(pBMOpt->m_structExpansionJoint.m_nEndDiaIdx[1], toKgPCm2(pADeckData->m_SigmaY)));
	if(pBMOpt->m_structExpansionJoint.m_bUnitLength)
	{
		m_Grid.SetTextMatrix(9, 2, "%.3f", pBMOpt->m_structExpansionJoint.m_dEndWeight[1]);
		m_Grid.SetTextMatrix(9, 3, "Kgf/m");
	}
	else
	{
		m_Grid.SetTextMatrix(9, 2, "%.3f", toTon(pBMOpt->m_structExpansionJoint.m_dEndWeight[1]));
		m_Grid.SetTextMatrix(9, 3, "tonf");
	}

	m_Grid.SetTextMatrix(10, 1, "%.0f", pBMOpt->m_structExpansionJoint.m_dMargin);
	m_Grid.SetTextMatrix(10, 3, "mm");
	CString	sText;
	sText.Format("%d", pBMOpt->m_structExpansionJoint.m_nMarginsu);
	m_Grid.SetTextMatrix(11, 1, sText);
	m_Grid.SetTextMatrix(11, 3, "EA");

	if(!pBMOpt->m_structExpansionJoint.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemState(4, 1, m_Grid.GetItemState(4, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(4, 1, RGBREADONLY);
		m_Grid.SetItemState(5, 1, m_Grid.GetItemState(5, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(5, 1, RGBREADONLY);
		m_Grid.SetItemState(5, 2, m_Grid.GetItemState(5, 2) | GVIS_READONLY);		m_Grid.SetItemBkColour(5, 2, RGBREADONLY);
		m_Grid.SetItemState(6, 1, m_Grid.GetItemState(6, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(6, 1, RGBREADONLY);
		m_Grid.SetItemState(6, 2, m_Grid.GetItemState(6, 2) | GVIS_READONLY);		m_Grid.SetItemBkColour(6, 2, RGBREADONLY);

		m_Grid.SetItemState(7, 1, m_Grid.GetItemState(7, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(7, 1, RGBREADONLY);
		m_Grid.SetItemState(8, 1, m_Grid.GetItemState(8, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(8, 1, RGBREADONLY);
		m_Grid.SetItemState(8, 2, m_Grid.GetItemState(8, 2) | GVIS_READONLY);		m_Grid.SetItemBkColour(8, 2, RGBREADONLY);
		m_Grid.SetItemState(9, 1, m_Grid.GetItemState(9, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(9, 1, RGBREADONLY);
		m_Grid.SetItemState(9, 2, m_Grid.GetItemState(9, 2) | GVIS_READONLY);		m_Grid.SetItemBkColour(9, 2, RGBREADONLY);

		m_Grid.SetItemState(5, 3, m_Grid.GetItemState(5, 3) | GVIS_READONLY);		m_Grid.SetItemBkColour(5, 3, RGBREADONLY);
		m_Grid.SetItemState(6, 3, m_Grid.GetItemState(6, 3) | GVIS_READONLY);		m_Grid.SetItemBkColour(6, 3, RGBREADONLY);
		m_Grid.SetItemState(8, 3, m_Grid.GetItemState(8, 3) | GVIS_READONLY);		m_Grid.SetItemBkColour(8, 3, RGBREADONLY);
		m_Grid.SetItemState(9, 3, m_Grid.GetItemState(9, 3) | GVIS_READONLY);		m_Grid.SetItemBkColour(9, 3, RGBREADONLY);

		m_Grid.SetItemState(10, 1, m_Grid.GetItemState(10, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(10, 1, RGBREADONLY);
		m_Grid.SetItemState(10, 3, m_Grid.GetItemState(10, 3) | GVIS_READONLY);		m_Grid.SetItemBkColour(10, 3, RGBREADONLY);
		m_Grid.SetItemState(11, 1, m_Grid.GetItemState(11, 1) | GVIS_READONLY);		m_Grid.SetItemBkColour(11, 1, RGBREADONLY);
		m_Grid.SetItemState(11, 3, m_Grid.GetItemState(11, 3) | GVIS_READONLY);		m_Grid.SetItemBkColour(11, 3, RGBREADONLY);
	}

	m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);
	m_ExpansionJoint.SetCheck(pBMOpt->m_structExpansionJoint.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// �������� ����
void CDeckBMOptionDlg::OnCheckExpansionjointCover() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	m_nSelectedOpt	= BMOPT_EXPANSIONJOINT_COVER;
	m_nCols			= 5;
	m_nRows			= 9;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 90);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(2, 80);
	m_Grid.SetColumnWidth(3, 50);
	m_Grid.SetColumnWidth(4, 50);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetMergeCol(0, 1, 4);
	m_Grid.SetMergeCol(1, 1, 4);
	m_Grid.SetCellType(1, 1, CT_COMBO);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structExpansionJointCover.m_bApply));

	m_Grid.SetTextMatrix(2, 0, pBMOpt->GetExpansionJointCoverPos(GENERAL_GUARDFENCE));
	m_Grid.SetTextMatrix(3, 0, pBMOpt->GetExpansionJointCoverPos(ABSOLUTE_GUARDFENCE));
	m_Grid.SetTextMatrix(4, 0, pBMOpt->GetExpansionJointCoverPos(GENERAL_SOUNDPROOF));
	m_Grid.SetTextMatrix(5, 0, pBMOpt->GetExpansionJointCoverPos(ABSOLUTE_SOUNDPROOF));
	m_Grid.SetTextMatrix(6, 0, pBMOpt->GetExpansionJointCoverPos(CENTER_GUARDFENCE));
	m_Grid.SetTextMatrix(7, 0, pBMOpt->GetExpansionJointCoverPos(PARAPET));
	m_Grid.SetTextMatrix(8, 0, pBMOpt->GetExpansionJointCoverPos(CURB));

	for(long nRow = 2; nRow < 9; nRow++)
	{
		m_Grid.SetTextMatrix(nRow, 1, "%.3f", toM(pBMOpt->m_structExpansionJointCover.m_dHeight[nRow-2]));
		m_Grid.SetTextMatrix(nRow, 2, "m");
		sText.Format("%d", pBMOpt->m_structExpansionJointCover.m_nQty[nRow-2]);
		m_Grid.SetTextMatrix(nRow, 3, sText);
		m_Grid.SetTextMatrix(nRow, 4, "EA");
	}

	if(!pBMOpt->m_structExpansionJointCover.m_bApply)
	{
		for(long nRow = 2; nRow < 9; nRow++)
		{
			for(long nCol = 1; nCol < 5; nCol++)
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
			}
		}
	}

	m_ExpansionJointCover.SetCheck(pBMOpt->m_structExpansionJointCover.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}

// ������ ��ũ��Ʈ
void CDeckBMOptionDlg::OnCheckShrinkageConcrete() 
{
	CADeckData		*pADeckData	= m_pStd->m_pDeckData;
	CBMOption		*pBMOpt		= &pADeckData->m_BMOption;
	CGlobarOption	*pGlopt		= m_pStd->m_pDataManage->GetGlobalOption();	

	CString	sText				= _T("");
	double	dExpJointSttHeight	= pGlopt->GetSttExpansionJointHeight();
	double	dExpJointSttWidth	= pGlopt->GetSttExpansionJointWidth();
	double	dExpJointEndHeight	= pGlopt->GetEndExpansionJointHeight();
	double	dExpJointEndWidth	= pGlopt->GetEndExpansionJointWidth();

	m_nSelectedOpt	= BMOPT_SHRINKAGE_CONCRETE;
	m_nCols			= 4;
	m_nRows			= 4;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 80);
	m_Grid.SetColumnWidth(3, 80);
	m_Grid.SetMergeCol(0, 1, 3);
	m_Grid.SetMergeCol(1, 1, 3);

	m_Grid.SetTextMatrix(2, 0, "�������� ����");
	m_Grid.SetTextMatrix(3, 0, "�������� ����");

	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structShrinkageConcrete.m_bApply));
	sText.Format("%.0f �� %.0f", dExpJointSttWidth, dExpJointSttHeight);
	m_Grid.SetTextMatrix(2, 1, sText);
	sText.Format("%d", pBMOpt->m_structShrinkageConcrete.m_nSttQty);
	m_Grid.SetTextMatrix(2, 2, sText);
	m_Grid.SetTextMatrix(2, 3, "EA");

	sText.Format("%.0f �� %.0f", dExpJointEndWidth, dExpJointEndHeight);
	m_Grid.SetTextMatrix(3, 1, sText);
	sText.Format("%d", pBMOpt->m_structShrinkageConcrete.m_nSttQty);
	m_Grid.SetTextMatrix(3, 2, sText);
	m_Grid.SetTextMatrix(3, 3, "EA");

	if(!pBMOpt->m_structShrinkageConcrete.m_bApply)
	{
	
	}

	m_ShrinkageConcrete.SetCheck(pBMOpt->m_structShrinkageConcrete.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ���� ������
void CDeckBMOptionDlg::OnCheckWaterDraw() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_WATER_DRAW;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bWaterDraw));
	m_WaterDraw.SetCheck(pBMOpt->m_bWaterDraw);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// NOTCH
void CDeckBMOptionDlg::OnCheckNotch() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_NOTCH;
	m_nCols			= 2;
	m_nRows			= 4;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetMergeRow(1, 2, 0);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetCellType(2, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structNotch.m_bApply));
	m_Grid.SetTextMatrix(2, 1, pBMOpt->GetNotchPos(pBMOpt->m_structNotch.m_nApplyPos));
	m_Grid.SetTextMatrix(3, 0, "NOTCH ����");
	m_Grid.SetTextMatrix(3, 1, pBMOpt->m_structNotch.m_szNotchType);

	if(!pBMOpt->m_structNotch.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
	}
	m_Notch.SetCheck(pBMOpt->m_structNotch.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// �����潺
void CDeckBMOptionDlg::OnCheckGuardFence() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_GUARD_FENCE;
	m_nCols			= 4;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 70);
	m_Grid.SetColumnWidth(2, 70);
	m_Grid.SetColumnWidth(3, 70);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(0, 2, "����");
	m_Grid.SetTextMatrix(0, 3, "����");
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structGuardFence.m_bApply));
	m_Grid.SetTextMatrix(1, 2, "%.1f", toM(pBMOpt->m_structGuardFence.m_dHeight));
	m_Grid.SetTextMatrix(1, 3, "m");
	if(!pBMOpt->m_structGuardFence.m_bApply)
	{
		m_Grid.SetItemState(1, 2, m_Grid.GetItemState(1, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(1, 2, RGBREADONLY);
		m_Grid.SetItemState(1, 3, m_Grid.GetItemState(1, 3) | GVIS_READONLY);	m_Grid.SetItemBkColour(1, 3, RGBREADONLY);
	}
	m_GuardFence.SetCheck(pBMOpt->m_structGuardFence.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// �����ǳ�
void CDeckBMOptionDlg::OnCheckSoundProof() 
{
	CADeckData		*pADeckData	= m_pStd->m_pDeckData;
	CBMOption		*pBMOpt		= &pADeckData->m_BMOption;
	CCalcData		*pData		= m_pStd->m_pDataManage->GetCalcData();	
	CCalcFloor		CalcFloor(m_pStd->m_pDataManage);

	m_nSelectedOpt	= BMOPT_SOUND_PROOF;
	m_nCols			= 3;
	m_nRows			= 4;
	
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 180);
	m_Grid.SetColumnWidth(2, 60);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structSoundProof.m_bApply));
	m_Grid.SetMergeCol(0, 1, 2, TRUE);
	m_Grid.SetMergeCol(1, 1, 2, TRUE);

	long nGuardTypeL = CalcFloor.GetGuardWallType(FLOOR_LEFT);
	long nGuardTypeR = CalcFloor.GetGuardWallType(FLOOR_RIGHT);
	BOOL bBangEmExistL = (pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && nGuardTypeL<=8) ? TRUE : FALSE;
	BOOL bBangEmExistR = (pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && nGuardTypeR<=8) ? TRUE : FALSE;

	if(bBangEmExistL)
	{
		m_Grid.SetTextMatrix(2, 0, "����(��)");
		m_Grid.SetTextMatrix(2, 1, "%.3f", toM(pBMOpt->m_structSoundProof.m_dLHeight));
		m_Grid.SetTextMatrix(2, 2, "m");
		if(!pBMOpt->m_structSoundProof.m_bApply)
		{
			m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
			m_Grid.SetItemState(2, 2, m_Grid.GetItemState(2, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 2, RGBREADONLY);
		}
	}
	else
		m_Grid.SetRowHeight(2, 0);

	if(bBangEmExistR)
	{
		m_Grid.SetTextMatrix(3, 0, "����(��)");
		m_Grid.SetTextMatrix(3, 1, "%.3f", toM(pBMOpt->m_structSoundProof.m_dRHeight));
		m_Grid.SetTextMatrix(3, 2, "m");
		if(!pBMOpt->m_structSoundProof.m_bApply)
		{
			m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
			m_Grid.SetItemState(3, 2, m_Grid.GetItemState(3, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(3, 2, RGBREADONLY);
		}
	}
	else
		m_Grid.SetRowHeight(3, 0);
	m_SoundProof.SetCheck(pBMOpt->m_structSoundProof.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ����
void CDeckBMOptionDlg::OnCheckParapet() 
{
	CADeckData		*pADeckData	= m_pStd->m_pDeckData;
	CBMOption		*pBMOpt		= &pADeckData->m_BMOption;
	CCalcData		*pData		= m_pStd->m_pDataManage->GetCalcData();	
	CCalcFloor		CalcFloor(m_pStd->m_pDataManage);

	CString	sText	= _T("");
	m_nSelectedOpt	= BMOPT_PARAPET;
	m_nCols			= 4;
	m_nRows			= 6;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 120);
	m_Grid.SetColumnWidth(2, 120);
	m_Grid.SetColumnWidth(3, 70);
	m_Grid.SetMergeCol(0, 1, 3);
	m_Grid.SetMergeCol(1, 1, 3);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetMergeRow(2, 3, 0);
	m_Grid.SetMergeRow(4, 5, 0);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structParapet.m_bApply));
	m_Grid.SetTextMatrix(2, 0, "��ȣ����");
	m_Grid.SetTextMatrix(2, 1, "��������");
	m_Grid.SetTextMatrix(3, 1, "��������");
	m_Grid.SetTextMatrix(4, 0, "������");
	m_Grid.SetTextMatrix(4, 1, "��������");
	m_Grid.SetTextMatrix(5, 1, "���밹��");

	m_Grid.SetTextMatrix(2, 2, "%.3f", toM(pBMOpt->m_structParapet.m_dGuardLength[0]));
	m_Grid.SetTextMatrix(2, 3, "m");
	m_Grid.SetTextMatrix(3, 2, "%.3f", toM(pBMOpt->m_structParapet.m_dGuardLength[1]));
	m_Grid.SetTextMatrix(3, 3, "m");
	m_Grid.SetTextMatrix(4, 2, "%.3f", toM(pBMOpt->m_structParapet.m_dCurbLength));
	m_Grid.SetTextMatrix(4, 3, "m");
	sText.Format("%d", pBMOpt->m_structParapet.m_nCurbQty);
	m_Grid.SetTextMatrix(5, 2, sText);
	m_Grid.SetTextMatrix(5, 3, "EA");

	long nGuardTypeL = CalcFloor.GetGuardWallType(FLOOR_LEFT);
	long nGuardTypeR = CalcFloor.GetGuardWallType(FLOOR_RIGHT);

	if(!pBMOpt->m_structParapet.m_bApply)
	{
		for(long nRow = 2; nRow < 6; nRow++)
		{
			for(long nCol = 1; nCol < 4; nCol++)
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
			}
		}
	}

	m_Parapet.SetCheck(pBMOpt->m_structParapet.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ������
void CDeckBMOptionDlg::OnCheckShade() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_SHADE;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bShade));
	m_Shade.SetCheck(pBMOpt->m_bShade);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ���Ϲ� ������
void CDeckBMOptionDlg::OnCheckDroppingPrevent() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_DROPPING_PREVENT;
	m_nCols			= 3;
	m_nRows			= 5;
	if(pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal)	m_nRows = 4;

	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 180);
	m_Grid.SetColumnWidth(2, 60);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetCellType(2, 1, CT_COMBO);
	m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetMergeCol(2, 1, 2);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structDroppingPrevent.m_bApply));
	m_Grid.SetTextMatrix(2, 0, "���� ����");
	m_Grid.SetTextMatrix(2, 1, pBMOpt->GetDroppingPrevent(pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal));
	m_Grid.SetTextMatrix(3, 0, "���� ������");
	m_Grid.SetTextMatrix(3, 1, "%.3f", toM(pBMOpt->m_structDroppingPrevent.m_dHorSpaceHeight));
	m_Grid.SetTextMatrix(3, 2, "m");
	if(!pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal)
	{
		m_Grid.SetTextMatrix(4, 0, "���� ������");
		m_Grid.SetTextMatrix(4, 1, "%.3f", toM(pBMOpt->m_structDroppingPrevent.m_dVerSpaceHeight));
		m_Grid.SetTextMatrix(4, 2, "m");
	}

	if(!pBMOpt->m_structDroppingPrevent.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemState(3, 2, m_Grid.GetItemState(3, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(3, 2, RGBREADONLY);
		if(!pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal)
		{
			m_Grid.SetItemState(4, 1, m_Grid.GetItemState(4, 1) | GVIS_READONLY);	m_Grid.SetItemBkColour(4, 1, RGBREADONLY);
			m_Grid.SetItemState(4, 2, m_Grid.GetItemState(4, 2) | GVIS_READONLY);	m_Grid.SetItemBkColour(4, 2, RGBREADONLY);
		}
	}
	m_DroppingPreVent.SetCheck(pBMOpt->m_structDroppingPrevent.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// �������
void CDeckBMOptionDlg::OnCheckSteelQuantity() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_STEEL_QUANTITY;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bSteelQuantity));
	m_SteelQuantity.SetCheck(pBMOpt->m_bSteelQuantity);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ��������
void CDeckBMOptionDlg::OnCheckPaint() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_PAINT;
	m_nCols			= 2;
	m_nRows			= 2;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 235);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bPaint));
	m_Paint.SetCheck(pBMOpt->m_bPaint);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ����ü�
void CDeckBMOptionDlg::OnCheckDrainEstablish() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_DRAIN_ESTABLISH;
	CString	sText	= _T("");

	if(!pBMOpt->m_bDrainEstablish)
	{
		m_nCols = 4;
		m_nRows = 2;
		SetGridTitle();
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidth(0, 90);
		m_Grid.SetColumnWidth(1, 120);
		m_Grid.SetCellType(1, 1, CT_COMBO);
		m_Grid.SetMergeCol(0, 1, 3);
		m_Grid.SetMergeCol(1, 1, 3);
		m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
		m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bDrainEstablish));
		m_DrainEstablish.SetCheck(pBMOpt->m_bDrainEstablish);
		m_Grid.SetRedraw(TRUE,TRUE);
		return;
	}
	if(pBMOpt->m_nDrainEstablishType == RIVER)
	{
		m_nCols	= 4;
		m_nRows = 5;
		SetGridTitle();
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidth(0, 90);
		m_Grid.SetColumnWidth(1, 110);
		m_Grid.SetColumnWidth(2, 90);
		m_Grid.SetColumnWidth(3, 60);
		m_Grid.SetCellType(1, 1, CT_COMBO);
		m_Grid.SetCellType(2, 1, CT_COMBO);
		m_Grid.SetMergeCol(0, 1, 3);
		m_Grid.SetMergeCol(1, 1, 3);
		m_Grid.SetMergeCol(2, 1, 3);

		m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
		m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bDrainEstablish));
		m_Grid.SetTextMatrix(2, 0, "Ÿ�� ����");
		m_Grid.SetTextMatrix(2, 1, pBMOpt->GetDrainEstablishType());
		m_Grid.SetTextMatrix(3, 0, "�� �� ��");
		m_Grid.SetTextMatrix(3, 1, "�����η��� Plate");
		sText.Format("%d", pBMOpt->m_structRiverDrainEstablish.m_nWaterCollect);
		m_Grid.SetTextMatrix(3, 2, sText);
		m_Grid.SetTextMatrix(3, 3, "EA");

		m_Grid.SetTextMatrix(4, 0, "�� �� ��");
		m_Grid.SetTextMatrix(4, 1, "�����η��� ����");
		m_Grid.SetTextMatrix(4, 2, "%.3f", toM(pBMOpt->m_structRiverDrainEstablish.m_dDrain));
		m_Grid.SetTextMatrix(4, 3, "m");
	}
	else if(pBMOpt->m_nDrainEstablishType == WALKWAY)
	{
		if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)
		{
			m_nCols	= 4;
			m_nRows = 9;
			SetGridTitle();
			m_Grid.SetRowHeightAll(20);
			m_Grid.SetColumnWidth(0, 90);
			m_Grid.SetColumnWidth(1, 110);
			m_Grid.SetColumnWidth(2, 90);
			m_Grid.SetColumnWidth(3, 60);
			m_Grid.SetCellType(1, 1, CT_COMBO);
			m_Grid.SetCellType(2, 1, CT_COMBO);
			m_Grid.SetCellType(3, 1, CT_COMBO);

			m_Grid.SetMergeCol(0, 1, 3);
			m_Grid.SetMergeCol(1, 1, 3);
			m_Grid.SetMergeCol(2, 1, 3);
			m_Grid.SetMergeCol(3, 1, 3);

			m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
			m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bDrainEstablish));
			m_Grid.SetTextMatrix(2, 0, "Ÿ�� ����");
			m_Grid.SetTextMatrix(2, 1, pBMOpt->GetDrainEstablishType());
			m_Grid.SetTextMatrix(3, 0, "��      ��");
			if(pBMOpt->m_structWalkWayDrainEstablish.m_nType==0)
				sText.Format("TYPE 1 (�Ǳ���)");
			else
				sText.Format("TYPE 2");
			m_Grid.SetTextMatrix(3, 1, sText);

			m_Grid.SetTextMatrix(4, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(4, 1, "-");
			m_Grid.SetItemState(4, 1, m_Grid.GetItemState(4, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);
			m_Grid.SetTextMatrix(4, 2, sText);
			m_Grid.SetTextMatrix(4, 3, "EA");
			m_Grid.SetTextMatrix(5, 0, "����������");
			m_Grid.SetTextMatrix(5, 1, "-");
			m_Grid.SetItemState(5, 1, m_Grid.GetItemState(5, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinWaterCollect);
			m_Grid.SetTextMatrix(5, 2, sText);
			m_Grid.SetTextMatrix(5, 3, "EA");
			m_Grid.SetTextMatrix(6, 0, "��      ��");
			m_Grid.SetTextMatrix(6, 1, "-");
			m_Grid.SetItemState(6, 1, m_Grid.GetItemState(6, 1) | GVIS_READONLY);
			m_Grid.SetTextMatrix(6, 2, "%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));
			m_Grid.SetTextMatrix(6, 3, "m");
			m_Grid.SetTextMatrix(7, 0, "��      ��");
			m_Grid.SetTextMatrix(7, 1, "-");
			m_Grid.SetItemState(7, 1, m_Grid.GetItemState(7, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);
			m_Grid.SetTextMatrix(7, 2, sText);
			m_Grid.SetTextMatrix(7, 3, "EA");
			m_Grid.SetTextMatrix(8, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(8, 1, "-");
			m_Grid.SetItemState(8, 1, m_Grid.GetItemState(8, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoint);
			m_Grid.SetTextMatrix(8, 2, sText);
			m_Grid.SetTextMatrix(8, 3, "EA");
		}
		else if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 1)
		{
			m_nCols	= 4;
			m_nRows = 16;
			SetGridTitle();
			m_Grid.SetRowHeightAll(20);
			m_Grid.SetColumnWidth(0, 90);
			m_Grid.SetColumnWidth(1, 110);
			m_Grid.SetColumnWidth(2, 90);
			m_Grid.SetColumnWidth(3, 60);
			m_Grid.SetCellType(1, 1, CT_COMBO);
			m_Grid.SetCellType(2, 1, CT_COMBO);
			m_Grid.SetCellType(3, 1, CT_COMBO);

			m_Grid.SetMergeCol(0, 1, 3);
			m_Grid.SetMergeCol(1, 1, 3);
			m_Grid.SetMergeCol(2, 1, 3);
			m_Grid.SetMergeCol(3, 1, 3);

			m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
			m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bDrainEstablish));
			m_Grid.SetTextMatrix(2, 0, "Ÿ�� ����");
			m_Grid.SetTextMatrix(2, 1, pBMOpt->GetDrainEstablishType());
			m_Grid.SetTextMatrix(3, 0, "��      ��");
			if(pBMOpt->m_structWalkWayDrainEstablish.m_nType==0)
				sText.Format("TYPE 1 (�Ǳ���)");
			else
				sText.Format("TYPE 2");
			m_Grid.SetTextMatrix(3, 1, sText);
			m_Grid.SetTextMatrix(4, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(4, 1, "�����η��� ����");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);
			m_Grid.SetTextMatrix(4, 2, sText);
			m_Grid.SetTextMatrix(4, 3, "EA");
			m_Grid.SetTextMatrix(5, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(5, 1, "�����η�������");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nDrain);
			m_Grid.SetTextMatrix(5, 2, sText);
			m_Grid.SetTextMatrix(5, 3, "EA");
			m_Grid.SetTextMatrix(6, 0, "��������");
			m_Grid.SetTextMatrix(6, 1, "8""��10""");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinDrain);
			m_Grid.SetTextMatrix(6, 2, sText);
			m_Grid.SetTextMatrix(6, 3, "EA");
			m_Grid.SetTextMatrix(7, 0, "��      ��");
			m_Grid.SetTextMatrix(7, 1, "150A");
			m_Grid.SetTextMatrix(7, 2, "%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));
			m_Grid.SetTextMatrix(7, 3, "m");
			m_Grid.SetTextMatrix(8, 0, "��      ��");
			m_Grid.SetTextMatrix(8, 1, "200A");
			m_Grid.SetTextMatrix(8, 2, "%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[1]));
			m_Grid.SetTextMatrix(8, 3, "m");
			m_Grid.SetTextMatrix(9, 0, "��      ��");
			m_Grid.SetTextMatrix(9, 1, "150A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);
			m_Grid.SetTextMatrix(9, 2, sText);
			m_Grid.SetTextMatrix(9, 3, "EA");
			m_Grid.SetTextMatrix(10, 0, "��      ��");
			m_Grid.SetTextMatrix(10, 1, "200A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[1]);
			m_Grid.SetTextMatrix(10, 2, sText);
			m_Grid.SetTextMatrix(10, 3, "EA");
			m_Grid.SetTextMatrix(11, 0, "û  ��  ��");
			m_Grid.SetTextMatrix(11, 1, "150A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[0]);
			m_Grid.SetTextMatrix(11, 2, sText);
			m_Grid.SetTextMatrix(11, 3, "EA");
			m_Grid.SetTextMatrix(12, 0, "û  ��  ��");
			m_Grid.SetTextMatrix(12, 1, "200A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[1]);
			m_Grid.SetTextMatrix(12, 2, sText);
			m_Grid.SetTextMatrix(12, 3, "EA");
			m_Grid.SetTextMatrix(13, 0, "��ΰ�����");
			m_Grid.SetTextMatrix(13, 1, "150A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[0]);
			m_Grid.SetTextMatrix(13, 2, sText);
			m_Grid.SetTextMatrix(13, 3, "EA");
			m_Grid.SetTextMatrix(14, 0, "��ΰ�����");
			m_Grid.SetTextMatrix(14, 1, "200A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[1]);
			m_Grid.SetTextMatrix(14, 2, sText);
			m_Grid.SetTextMatrix(14, 3, "EA");
			m_Grid.SetTextMatrix(15, 0, "ħ  ��  ��");
			m_Grid.SetTextMatrix(15, 1, "-");
			m_Grid.SetItemState(15, 1, m_Grid.GetItemState(15, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCollecting);
			m_Grid.SetTextMatrix(15, 2, sText);
			m_Grid.SetTextMatrix(15, 3, "EA");
		}
		else
			return;
	}
	else if(pBMOpt->m_nDrainEstablishType == RIVERWALKWAY)
	{

		if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)
		{
			m_nCols	= 4;
			m_nRows = 11;
		}
		else
		{
			m_nCols = 4;
			m_nRows = 18;
		}
		SetGridTitle();
		m_Grid.SetColumnWidth(0, 90);
		m_Grid.SetColumnWidth(1, 110);
		m_Grid.SetColumnWidth(2, 90);
		m_Grid.SetColumnWidth(3, 60);
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetCellType(1, 1, CT_COMBO);
		m_Grid.SetCellType(2, 1, CT_COMBO);
		m_Grid.SetCellType(3, 1, CT_COMBO);

		m_Grid.SetMergeCol(0, 1, m_nCols-1);
		m_Grid.SetMergeCol(1, 1, m_nCols-1);
		m_Grid.SetMergeCol(2, 1, m_nCols-1);
		m_Grid.SetMergeCol(3, 1, m_nCols-1);

		m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
		m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_bDrainEstablish));
		m_Grid.SetTextMatrix(2, 0, "Ÿ�� ����");
		m_Grid.SetTextMatrix(2, 1, pBMOpt->GetDrainEstablishType());
		if(pBMOpt->m_structWalkWayDrainEstablish.m_nType==0)	sText.Format("TYPE 1 (�Ǳ���)");
		else													sText.Format("TYPE 2");
		m_Grid.SetTextMatrix(3, 1, sText);
		m_Grid.SetTextMatrix(4, 0, "�� �� ��");
		m_Grid.SetTextMatrix(4, 1, "�����η��� Plate");
		sText.Format("%d", pBMOpt->m_structRiverDrainEstablish.m_nWaterCollect);
		m_Grid.SetTextMatrix(4, 2, sText);
		m_Grid.SetTextMatrix(4, 3, "EA");

		m_Grid.SetTextMatrix(5, 0, "�� �� ��");
		m_Grid.SetTextMatrix(5, 1, "�����η��� ����");
		m_Grid.SetTextMatrix(5, 2, "%.3f", toM(pBMOpt->m_structRiverDrainEstablish.m_dDrain));
		m_Grid.SetTextMatrix(5, 3, "m");

		if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)
		{
			m_Grid.SetTextMatrix(6, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(6, 1, "-");
			m_Grid.SetItemState(6, 1, m_Grid.GetItemState(6, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);
			m_Grid.SetTextMatrix(6, 2, sText);
			m_Grid.SetTextMatrix(6, 3, "EA");
			m_Grid.SetTextMatrix(7, 0, "����������");
			m_Grid.SetTextMatrix(7, 1, "-");
			m_Grid.SetItemState(7, 1, m_Grid.GetItemState(7, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinWaterCollect);
			m_Grid.SetTextMatrix(7, 2, sText);
			m_Grid.SetTextMatrix(7, 3, "EA");
			m_Grid.SetTextMatrix(8, 0, "��      ��");
			m_Grid.SetTextMatrix(8, 1, "-");
			m_Grid.SetItemState(8, 1, m_Grid.GetItemState(8, 1) | GVIS_READONLY);
			m_Grid.SetTextMatrix(8, 2, "%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));
			m_Grid.SetTextMatrix(8, 3, "m");
			m_Grid.SetTextMatrix(9, 0, "��      ��");
			m_Grid.SetTextMatrix(9, 1, "-");
			m_Grid.SetItemState(9, 1, m_Grid.GetItemState(9, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);
			m_Grid.SetTextMatrix(9, 2, sText);
			m_Grid.SetTextMatrix(9, 3, "EA");
			m_Grid.SetTextMatrix(10, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(10, 1, "-");
			m_Grid.SetItemState(10, 1, m_Grid.GetItemState(10, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoint);
			m_Grid.SetTextMatrix(10, 2, sText);
			m_Grid.SetTextMatrix(10, 3, "EA");

		}
		else
		{
			m_Grid.SetTextMatrix(6, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(6, 1, "�����η��� ����");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);
			m_Grid.SetTextMatrix(6, 2, sText);
			m_Grid.SetTextMatrix(6, 3, "EA");
			m_Grid.SetTextMatrix(7, 0, "��  ��  ��");
			m_Grid.SetTextMatrix(7, 1, "�����η�������");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nDrain);
			m_Grid.SetTextMatrix(7, 2, sText);
			m_Grid.SetTextMatrix(7, 3, "EA");
			m_Grid.SetTextMatrix(8, 0, "��������");
			m_Grid.SetTextMatrix(8, 1, "8""��10""");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinDrain);
			m_Grid.SetTextMatrix(8, 2, sText);
			m_Grid.SetTextMatrix(8, 3, "EA");
			m_Grid.SetTextMatrix(9, 0, "��      ��");
			m_Grid.SetTextMatrix(9, 1, "150A");
			m_Grid.SetTextMatrix(9, 2, "%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));
			m_Grid.SetTextMatrix(9, 3, "m");
			m_Grid.SetTextMatrix(10, 0, "��      ��");
			m_Grid.SetTextMatrix(10, 1, "200A");
			m_Grid.SetTextMatrix(10, 2, "%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[1]));
			m_Grid.SetTextMatrix(10, 3, "m");
			m_Grid.SetTextMatrix(11, 0, "��      ��");
			m_Grid.SetTextMatrix(11, 1, "150A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);
			m_Grid.SetTextMatrix(11, 2, sText);
			m_Grid.SetTextMatrix(11, 3, "EA");
			m_Grid.SetTextMatrix(12, 0, "��      ��");
			m_Grid.SetTextMatrix(12, 1, "200A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[1]);
			m_Grid.SetTextMatrix(12, 2, sText);
			m_Grid.SetTextMatrix(12, 3, "EA");
			m_Grid.SetTextMatrix(13, 0, "û  ��  ��");
			m_Grid.SetTextMatrix(13, 1, "150A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[0]);
			m_Grid.SetTextMatrix(13, 2, sText);
			m_Grid.SetTextMatrix(13, 3, "EA");
			m_Grid.SetTextMatrix(14, 0, "û  ��  ��");
			m_Grid.SetTextMatrix(14, 1, "200A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[1]);
			m_Grid.SetTextMatrix(14, 2, sText);
			m_Grid.SetTextMatrix(14, 3, "EA");
			m_Grid.SetTextMatrix(15, 0, "��ΰ�����");
			m_Grid.SetTextMatrix(15, 1, "150A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[0]);
			m_Grid.SetTextMatrix(15, 2, sText);
			m_Grid.SetTextMatrix(15, 3, "EA");
			m_Grid.SetTextMatrix(16, 0, "��ΰ�����");
			m_Grid.SetTextMatrix(16, 1, "200A");
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[1]);
			m_Grid.SetTextMatrix(16, 2, sText);
			m_Grid.SetTextMatrix(16, 3, "EA");
			m_Grid.SetTextMatrix(17, 0, "ħ  ��  ��");
			m_Grid.SetTextMatrix(17, 1, "-");
			m_Grid.SetItemState(17, 1, m_Grid.GetItemState(17, 1) | GVIS_READONLY);
			sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCollecting);
			m_Grid.SetTextMatrix(17, 2, sText);
			m_Grid.SetTextMatrix(17, 3, "EA");
		}
		long nRow = 0;
		for(nRow = 4; nRow < 6; nRow++)
		{
			for(long nCol = 1; nCol < m_nCols; nCol++)
				m_Grid.SetItemBkColour(nRow, nCol, RGB(255, 255, 200));
		}

		for(nRow = 6; nRow < m_nRows; nRow++)
		{
			for(long nCol = 1; nCol < m_nCols; nCol++)
				m_Grid.SetItemBkColour(nRow, nCol, RGB(200, 255, 255));
		}
	}
	else
		return;

	m_DrainEstablish.SetCheck(pBMOpt->m_bDrainEstablish);
	m_Grid.SetRedraw(TRUE,TRUE);
}
// ������
void CDeckBMOptionDlg::OnCheckWingWall() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_WING_WALL;
	m_nCols			= 4;
	m_nRows			= 6;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 70);
	m_Grid.SetColumnWidth(2, 70);
	m_Grid.SetColumnWidth(3, 70);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(2, 0, "����");
	m_Grid.SetTextMatrix(2, 1, "����");
	m_Grid.SetTextMatrix(3, 1, "����");
	m_Grid.SetTextMatrix(4, 0, "����");
	m_Grid.SetTextMatrix(4, 1, "����");
	m_Grid.SetTextMatrix(5, 1, "����");
	m_Grid.SetMergeRow(2, 3, 0);
	m_Grid.SetMergeRow(4, 5, 0);
	m_Grid.SetMergeCol(0, 1, 3);
	m_Grid.SetMergeCol(1, 1, 3);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structWingWall.m_bApply));
	m_Grid.SetTextMatrix(2, 2, "%.1f", toM(pBMOpt->m_structWingWall.m_dLengthLeftStt));
	m_Grid.SetTextMatrix(2, 3, "m");
	m_Grid.SetTextMatrix(3, 2, "%.1f", toM(pBMOpt->m_structWingWall.m_dLengthLeftEnd));
	m_Grid.SetTextMatrix(3, 3, "m");
	m_Grid.SetTextMatrix(4, 2, "%.1f", toM(pBMOpt->m_structWingWall.m_dLengthRighStt));
	m_Grid.SetTextMatrix(4, 3, "m");
	m_Grid.SetTextMatrix(5, 2, "%.1f", toM(pBMOpt->m_structWingWall.m_dLengthRighEnd));
	m_Grid.SetTextMatrix(5, 3, "m");

	if(!pBMOpt->m_structWingWall.m_bApply)
	{
		m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(4, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(5, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(2, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(3, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(4, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(5, 3, RGBREADONLY);

		m_Grid.SetItemState(2, 2, m_Grid.GetItemState(2, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(2, 2, RGBREADONLY);
		m_Grid.SetItemState(3, 2, m_Grid.GetItemState(3, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(3, 2, RGBREADONLY);
		m_Grid.SetItemState(4, 2, m_Grid.GetItemState(4, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(4, 2, RGBREADONLY);
		m_Grid.SetItemState(5, 2, m_Grid.GetItemState(5, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(5, 2, RGBREADONLY);
	}
	m_Grid.SetItemState(2, 1, m_Grid.GetItemState(1, 2) | GVIS_READONLY);
	m_Grid.SetItemState(3, 1, m_Grid.GetItemState(1, 3) | GVIS_READONLY);
	m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);
	m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);
	m_Grid.SetItemState(4, 1, m_Grid.GetItemState(4, 1) | GVIS_READONLY);
	m_Grid.SetItemState(5, 1, m_Grid.GetItemState(5, 1) | GVIS_READONLY);

	m_Grid.SetItemState(2, 3, m_Grid.GetItemState(2, 1) | GVIS_READONLY);
	m_Grid.SetItemState(3, 3, m_Grid.GetItemState(3, 1) | GVIS_READONLY);
	m_Grid.SetItemState(4, 3, m_Grid.GetItemState(4, 1) | GVIS_READONLY);
	m_Grid.SetItemState(5, 3, m_Grid.GetItemState(5, 1) | GVIS_READONLY);

	m_GuardFence.SetCheck(pBMOpt->m_structWingWall.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
	
}
// ���ٸ�
void CDeckBMOptionDlg::OnCheckTimber() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_TIMBER;
	m_nCols			= 3;
	m_nRows			= 5;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 140);
	m_Grid.SetColumnWidth(2, 80);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetCellType(2, 1, CT_COMBO);
	m_Grid.SetCellType(3, 1, CT_COMBO);
	m_Grid.SetTextMatrix(1, 0, "���ٸ�����");
	m_Grid.SetTextMatrix(2, 0, "���ٸ�����");
	m_Grid.SetTextMatrix(3, 0, "��ũ�Ǵϼ� ��ġ");
	m_Grid.SetTextMatrix(4, 0, "��ũ�Ǵϼ� �۾���");
	m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetMergeCol(2, 1, 2);
	m_Grid.SetMergeCol(3, 1, 2);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structTimber.m_bApply));
	m_Grid.SetTextMatrix(2, 1, pBMOpt->GetTimberQuality(pBMOpt->m_structTimber.m_nQuality));
	m_Grid.SetTextMatrix(3, 1, pBMOpt->GetPosDeckFinisher(pBMOpt->m_structTimber.m_nPosDeckFinisher));
	m_Grid.SetTextMatrix(4, 1, "%.3f", toM(pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth));
	m_Grid.SetTextMatrix(4, 2, "m");

	if(!pBMOpt->m_structTimber.m_bApply)
	{
		m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemState(4, 1, m_Grid.GetItemState(4, 1) | GVIS_READONLY);m_Grid.SetItemBkColour(4, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(4, 2, RGBREADONLY);
	}
	m_Grid.SetItemState(4, 2, m_Grid.GetItemState(4, 2) | GVIS_READONLY);
	m_GuardFence.SetCheck(pBMOpt->m_structTimber.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}


void CDeckBMOptionDlg::OnButtonAllCheck() 
{
	SetOptionAll(TRUE);
	OnGridRedraw();
	SetDataInit();
}

void CDeckBMOptionDlg::OnButtonAllCancel() 
{
	SetOptionAll(FALSE);
	OnGridRedraw();
	SetDataInit();
}

void CDeckBMOptionDlg::SetOptionAll(BOOL bApply)
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	pBMOpt->m_bConcreteBuy							= bApply;
	pBMOpt->m_bConcretePlacing						= bApply;
	pBMOpt->m_bMold									= bApply;
	pBMOpt->m_bSpacer								= bApply;
	pBMOpt->m_structRebarManufacture.m_bApply		= bApply;
	pBMOpt->m_structPave.m_bApply					= bApply;
	pBMOpt->m_structTackCoating.m_bApply			= bApply;
	pBMOpt->m_bDrainBridgeSurface					= bApply;
	pBMOpt->m_structBridgeName.m_bApply				= bApply;
	pBMOpt->m_structBridgeNamePlate.m_bApply		= bApply;
	pBMOpt->m_structExplainPlate.m_bApply			= bApply;
	pBMOpt->m_structEstablishTBM.m_bApply			= bApply;
	pBMOpt->m_structStyrofoam.m_bApply				= bApply;
	pBMOpt->m_structExpansionJoint.m_bApply			= bApply;
	pBMOpt->m_structExpansionJointCover.m_bApply	= bApply;
	pBMOpt->m_structShrinkageConcrete.m_bApply		= bApply;
	pBMOpt->m_bWaterDraw							= bApply;
	pBMOpt->m_structNotch.m_bApply					= bApply;
	pBMOpt->m_structGuardFence.m_bApply				= bApply;
	pBMOpt->m_structSoundProof.m_bApply				= bApply;
	pBMOpt->m_structParapet.m_bApply				= bApply;
	pBMOpt->m_bShade								= bApply;
	pBMOpt->m_structDroppingPrevent.m_bApply		= bApply;
	pBMOpt->m_bSteelQuantity						= bApply;
	pBMOpt->m_bPaint								= bApply;
	pBMOpt->m_bDrainEstablish						= bApply;
	pBMOpt->m_structWingWall.m_bApply				= bApply;
	pBMOpt->m_structTimber.m_bApply					= bApply;
	pBMOpt->m_structElecWireHole.m_bApply			= bApply;
}

LRESULT CDeckBMOptionDlg::OnReceive(WPARAM wp,LPARAM lp)
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	switch(wp)
	{
	case IDC_CHECK_CONCRETE_BUY :
		pBMOpt->m_bConcreteBuy = m_ConcreteBuy.GetCheck();
		OnCheckConcreteBuy();
		break;
	case IDC_CHECK_CONCRETE_PLACING :
		pBMOpt->m_bConcretePlacing = m_ConcretePlacing.GetCheck();
		OnCheckConcretePlacing();
		break;
	case IDC_CHECK_MOLD :
		pBMOpt->m_bMold = m_Mold.GetCheck();
		OnCheckMold();
		break;
	case IDC_CHECK_SPACER :
		pBMOpt->m_bSpacer= m_Spacer.GetCheck();
		OnCheckSpacer();
		break;
	case IDC_CHECK_REBAR_MANUFACTURE :
		pBMOpt->m_structRebarManufacture.m_bApply = m_RebarManufacture.GetCheck();
		OnCheckRebarManufacture();
		break;
	case IDC_CHECK_PAVE :
		pBMOpt->m_structPave.m_bApply = m_Pave.GetCheck();
		OnCheckPave();
		break;
	case IDC_CHECK_TACK_COATING :
		pBMOpt->m_structTackCoating.m_bApply = m_TackCoating.GetCheck();
		OnCheckTackCoating();
		break;
	case IDC_CHECK_DRAIN_BRIDGESURFACE :
		pBMOpt->m_bDrainBridgeSurface = m_DrainBridgeSurface.GetCheck();
		OnCheckDrainBridgesurface();
		break;
	case IDC_CHECK_BRIDGE_NAME :
		pBMOpt->m_structBridgeName.m_bApply = m_BridgeName.GetCheck();
		OnCheckBridgeName();
		break;
	case IDC_CHECK_BRIDGE_NAMEPLATE :
		pBMOpt->m_structBridgeNamePlate.m_bApply = m_BridgeNamePlate.GetCheck();
		OnCheckBridgeNameplate();
		break;
	case IDC_CHECK_EXPLAIN_PLATE :
		pBMOpt->m_structExplainPlate.m_bApply = m_ExplainPlate.GetCheck();
		OnCheckExplainPlate();
		break;
	case IDC_CHECK_ESTABLISH_TBM :
		pBMOpt->m_structEstablishTBM.m_bApply = m_EstablishTBM.GetCheck();
		OnCheckEstablishTbm();
		break;
	case IDC_CHECK_STYROFOAM :
		pBMOpt->m_structStyrofoam.m_bApply = m_Styrofoam.GetCheck();
		OnCheckStyrofoam();
		break;
	case IDC_CHECK_EXPANSIONJOINT :
		pBMOpt->m_structExpansionJoint.m_bApply = m_ExpansionJoint.GetCheck();
		OnCheckExpansionjoint();
		break;
	case IDC_CHECK_EXPANSIONJOINT_COVER :
		pBMOpt->m_structExpansionJointCover.m_bApply = m_ExpansionJointCover.GetCheck();
		OnCheckExpansionjointCover();
		break;
	case IDC_CHECK_SHRINKAGE_CONCRETE :
		pBMOpt->m_structShrinkageConcrete.m_bApply = m_ShrinkageConcrete.GetCheck();
		OnCheckShrinkageConcrete();
		break;
	case IDC_CHECK_WATER_DRAW :
		pBMOpt->m_bWaterDraw = m_WaterDraw.GetCheck();
		OnCheckWaterDraw();
		break;
	case IDC_CHECK_NOTCH :
		pBMOpt->m_structNotch.m_bApply = m_Notch.GetCheck();
		OnCheckNotch();
		break;
	case IDC_CHECK_GUARD_FENCE :
		pBMOpt->m_structGuardFence.m_bApply = m_GuardFence.GetCheck();
		OnCheckGuardFence();
		break;
	case IDC_CHECK_SOUND_PROOF :
		pBMOpt->m_structSoundProof.m_bApply = m_SoundProof.GetCheck();
		OnCheckSoundProof();
		break;
	case IDC_CHECK_PARAPET :
		pBMOpt->m_structParapet.m_bApply = m_Parapet.GetCheck();
		OnCheckParapet();
		break;
	case IDC_CHECK_SHADE :
		pBMOpt->m_bShade = m_Shade.GetCheck();
		OnCheckShade();
		break;
	case IDC_CHECK_DROPPING_PREVENT :
		pBMOpt->m_structDroppingPrevent.m_bApply = m_DroppingPreVent.GetCheck();
		OnCheckDroppingPrevent();
		break;
	case IDC_CHECK_STEEL_QUANTITY :
		pBMOpt->m_bSteelQuantity = m_SteelQuantity.GetCheck();
		OnCheckSteelQuantity();
		break;
	case IDC_CHECK_PAINT :
		pBMOpt->m_bPaint = m_Paint.GetCheck();
		OnCheckPaint();
		break;
	case IDC_CHECK_DRAIN_ESTABLISH :
		pBMOpt->m_bDrainEstablish = m_DrainEstablish.GetCheck();
		OnCheckDrainEstablish();
		break;
	case IDC_CHECK_WING_WALL :
		pBMOpt->m_structWingWall.m_bApply	= m_WingWall.GetCheck();
		OnCheckWingWall();
		break;
	case IDC_CHECK_TIMBER :
		pBMOpt->m_structTimber.m_bApply		= m_Timber.GetCheck();
		OnCheckTimber();
	case IDC_CHECK_ELECWIREHOLE :
		pBMOpt->m_structElecWireHole.m_bApply	= m_ElecWireHole.GetCheck();
		OnCheckElecWireHole();
	}

	SetDataInit();
	return 0;
}

BOOL CDeckBMOptionDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}

void CDeckBMOptionDlg::OnCheckElecWireHole() 
{
	CADeckData	*pADeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pADeckData->m_BMOption;

	m_nSelectedOpt	= BMOPT_ELECWIREHOLE;
	m_nCols			= 4;
	m_nRows			= 8;
	SetGridTitle();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 140);
	m_Grid.SetColumnWidth(1, 70);
	m_Grid.SetColumnWidth(2, 70);
	m_Grid.SetColumnWidth(3, 70);
	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetTextMatrix(2, 0, "��100");
	m_Grid.SetTextMatrix(2, 1, "����");
	m_Grid.SetTextMatrix(3, 1, "����");
	m_Grid.SetTextMatrix(4, 0, "��125");
	m_Grid.SetTextMatrix(4, 1, "����");
	m_Grid.SetTextMatrix(5, 1, "����");
	m_Grid.SetTextMatrix(6, 0, "��150");
	m_Grid.SetTextMatrix(6, 1, "����");
	m_Grid.SetTextMatrix(7, 1, "����");
	m_Grid.SetMergeRow(2, 3, 0);
	m_Grid.SetMergeRow(4, 5, 0);
	m_Grid.SetMergeRow(6, 7, 0);
	m_Grid.SetMergeCol(0, 1, 3);
	m_Grid.SetMergeCol(1, 1, 3);

	m_Grid.SetTextMatrix(1, 0, pBMOpt->GetBMOptItem(m_nSelectedOpt));
	m_Grid.SetTextMatrix(1, 1, IsApply(pBMOpt->m_structElecWireHole.m_bApply));
	m_Grid.SetTextMatrix(2, 2, "%.0f", (double)(pBMOpt->m_structElecWireHole.m_nEA100[0]));
	m_Grid.SetTextMatrix(2, 3, "EA");
	m_Grid.SetTextMatrix(3, 2, "%.0f", (double)(pBMOpt->m_structElecWireHole.m_nEA100[1]));
	m_Grid.SetTextMatrix(3, 3, "EA");
	m_Grid.SetTextMatrix(4, 2, "%.0f", (double)(pBMOpt->m_structElecWireHole.m_nEA125[0]));
	m_Grid.SetTextMatrix(4, 3, "EA");
	m_Grid.SetTextMatrix(5, 2, "%.0f", (double)(pBMOpt->m_structElecWireHole.m_nEA125[1]));
	m_Grid.SetTextMatrix(5, 3, "EA");
	m_Grid.SetTextMatrix(6, 2, "%.0f", (double)(pBMOpt->m_structElecWireHole.m_nEA150[0]));
	m_Grid.SetTextMatrix(6, 3, "EA");
	m_Grid.SetTextMatrix(7, 2, "%.0f", (double)(pBMOpt->m_structElecWireHole.m_nEA150[1]));
	m_Grid.SetTextMatrix(7, 3, "EA");

	if(!pBMOpt->m_structElecWireHole.m_bApply)
	{
		m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(2, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(3, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(4, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(5, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(6, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(7, 1, RGBREADONLY);
		m_Grid.SetItemBkColour(2, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(3, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(4, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(5, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(6, 3, RGBREADONLY);
		m_Grid.SetItemBkColour(7, 3, RGBREADONLY);

		m_Grid.SetItemState(2, 2, m_Grid.GetItemState(2, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(2, 2, RGBREADONLY);
		m_Grid.SetItemState(3, 2, m_Grid.GetItemState(3, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(3, 2, RGBREADONLY);
		m_Grid.SetItemState(4, 2, m_Grid.GetItemState(4, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(4, 2, RGBREADONLY);
		m_Grid.SetItemState(5, 2, m_Grid.GetItemState(5, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(5, 2, RGBREADONLY);
		m_Grid.SetItemState(6, 2, m_Grid.GetItemState(6, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(6, 2, RGBREADONLY);
		m_Grid.SetItemState(7, 2, m_Grid.GetItemState(7, 2) | GVIS_READONLY);m_Grid.SetItemBkColour(7, 2, RGBREADONLY);
	}
	m_Grid.SetItemState(2, 1, m_Grid.GetItemState(1, 2) | GVIS_READONLY);
	m_Grid.SetItemState(3, 1, m_Grid.GetItemState(1, 3) | GVIS_READONLY);
	m_Grid.SetItemState(2, 1, m_Grid.GetItemState(2, 1) | GVIS_READONLY);
	m_Grid.SetItemState(3, 1, m_Grid.GetItemState(3, 1) | GVIS_READONLY);
	m_Grid.SetItemState(4, 1, m_Grid.GetItemState(4, 1) | GVIS_READONLY);
	m_Grid.SetItemState(5, 1, m_Grid.GetItemState(5, 1) | GVIS_READONLY);
	m_Grid.SetItemState(6, 1, m_Grid.GetItemState(6, 1) | GVIS_READONLY);
	m_Grid.SetItemState(7, 1, m_Grid.GetItemState(7, 1) | GVIS_READONLY);

	m_Grid.SetItemState(2, 3, m_Grid.GetItemState(2, 1) | GVIS_READONLY);
	m_Grid.SetItemState(3, 3, m_Grid.GetItemState(3, 1) | GVIS_READONLY);
	m_Grid.SetItemState(4, 3, m_Grid.GetItemState(4, 1) | GVIS_READONLY);
	m_Grid.SetItemState(5, 3, m_Grid.GetItemState(5, 1) | GVIS_READONLY);
	m_Grid.SetItemState(6, 3, m_Grid.GetItemState(6, 1) | GVIS_READONLY);
	m_Grid.SetItemState(7, 3, m_Grid.GetItemState(7, 1) | GVIS_READONLY);

	m_ElecWireHole.SetCheck(pBMOpt->m_structElecWireHole.m_bApply);
	m_Grid.SetRedraw(TRUE,TRUE);
}
void CDeckBMOptionDlg::OnButtonOptionSave() 
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pDeckData->m_BMOption;

	CFileDialog dlg(FALSE, "*.abm",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		              "ARoad ���� �ɼ�����(*.abm)|*.abm||",NULL);

	dlg.m_ofn.lpstrTitle = "�����ɼ� ����";
	if(dlg.DoModal()==IDOK)
	{
		UpdateData();
		CFile file;
		if(!file.Open(dlg.GetPathName(),CFile::modeWrite | CFile::modeCreate)) return;
		CArchive ar(&file,CArchive::store);		
		pBMOpt->Serialize(ar);
		ar.Close();
		file.Close();
	}	
}

void CDeckBMOptionDlg::OnButtonOptionLoad() 
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CBMOption	*pBMOpt		= &pDeckData->m_BMOption;

	CFileDialog dlg(TRUE, "*.abm",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		              "APlate ���� �ɼ�����(*.abm)|*.abm||",NULL);
	dlg.m_ofn.lpstrTitle = "�����ɼ� �ҷ�����";
	if(dlg.DoModal()==IDOK)
	{
		CFile file;
		if(!file.Open(dlg.GetPathName(),CFile::modeRead)) return;
		CArchive ar(&file,CArchive::load);
		pBMOpt->Serialize(ar);
		ar.Close();
		file.Close();
		SetDataInit();
	}
}
