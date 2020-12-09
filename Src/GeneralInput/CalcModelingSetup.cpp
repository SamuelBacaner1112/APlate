// CalcModelingSetup.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingSetup dialog


CCalcModelingSetup::CCalcModelingSetup(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcModelingSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcModelingSetup)
	m_BonusRate     = 0.0;
	m_JijumDownSize = 0.0;
	m_AddLoad       = 0.0;
	m_TotalWeight   = 0.0;
	m_ModelWeight   = 0.0;
	m_bCheckSettlement = TRUE;
	m_dSteelMass = 0.0;
	m_dAcceleration = 0.0;
	//}}AFX_DATA_INIT
	m_bGL = TRUE;
}


void CCalcModelingSetup::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcModelingSetup)
	DDX_Control(pDX, IDC_COMBO_SETTLE, m_comboSettle);
	DDX_Control(pDX, IDC_RADIO_GIRDER_CEN, m_radioGirderCen);
	DDX_Text(pDX, IDC_MODELINIT_BONUSRATE_EDIT, m_BonusRate);
	DDX_Text(pDX, IDC_MODELINIT_JIJUM_EDIT, m_JijumDownSize);
	DDX_Text(pDX, IDC_MODELINIT_ADDLOAD_EDIT, m_AddLoad);
	DDX_Text(pDX, IDC_MODELINIT_TOTALWEIGHT_EDIT, m_TotalWeight);
	DDX_Text(pDX, IDC_MODELINIT_MODELWEIGHT_EDIT, m_ModelWeight);
	DDX_Control(pDX, IDC_EDIT_STEEL_MASS, m_EditSteelMass);
	DDX_Control(pDX, IDC_MODELINIT_TOTALWEIGHT_EDIT, m_Edit1);
	DDX_Control(pDX, IDC_MODELINIT_MODELWEIGHT_EDIT, m_Edit2);
	DDX_Control(pDX, IDC_MODELINIT_BONUSRATE_EDIT, m_Edit3);
	DDX_Control(pDX, IDC_MODELINIT_ADDLOAD_EDIT, m_Edit4);
	DDX_Control(pDX, IDC_MODELINIT_JIJUM_EDIT, m_Edit5);
	DDX_Check(pDX, IDC_CHECK_SETTLEMENT, m_bCheckSettlement);
	DDX_Text(pDX, IDC_EDIT_STEEL_MASS, m_dSteelMass);
	DDX_Text(pDX, IDC_EDIT_ACCELERATION, m_dAcceleration);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_MAKEMODEL, m_btnMakeModeling);
}

BEGIN_MESSAGE_MAP(CCalcModelingSetup, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcModelingSetup)
	ON_BN_CLICKED(IDC_BUTTON_MAKEMODEL, OnButtonMakemodel)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_CHECK_SETTLEMENT, OnCheckSettlement)
	ON_BN_CLICKED(IDC_RADIO_GIRDER_CEN, OnRadioGirderCen)
	ON_BN_CLICKED(IDC_RADIO_GIRDER_LOWER, OnRadioGirderLower)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTLE, OnSelchangeComboSettle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingSetup message handlers

BOOL CCalcModelingSetup::OnInitDialog() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	CGeneralBaseDlg::OnInitDialog();

	m_dSteelMass = Round(pData->m_TotalWeight/m_dAcceleration,4);

	UpdateData(FALSE);

	return TRUE;  
}

void CCalcModelingSetup::SetDataInit() 
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CCalcData      *pData    = m_pStd->GetDataManage()->GetCalcData();
	CGlobarOption	*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();

	CFemModelingCalc ModelingCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	if(pCalcStd->GetSizeHLLFemData()<=0) return;

	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, FALSE);

	m_ModelWeight		= pData->m_ModelWeight;
	m_BonusRate			= pData->m_BonusRate;
	m_JijumDownSize		= pData->m_JijumDownSize;
	m_AddLoad			= pData->m_AddLoad;
	m_dAcceleration		= pOpt->GetAccelerationOfGravity();	
	m_TotalWeight		= pData->m_TotalWeight;
	m_bCheckSettlement	= pData->m_bCheckSettlement;

	m_radioGirderCen.SetCheck(!pData->IsShoeGirderLower());
	if(pData->m_bCheckSettlement==FALSE)
		m_Edit5.EnableWindow(FALSE);

	m_comboSettle.SetCurSel(pData->m_nModelingSettleMethod);

	UpdateData(FALSE);
}


void CCalcModelingSetup::SetDataDefault()
{
	CAPlateCalcStd  *pCalcStd = m_pStd->m_pCalcStd;

	if(pCalcStd->GetSizeHLLFemData()<=0) return;

	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CFemModelingCalc ModelingCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	ModelingCalc.CalcBonusRate();

	pData->m_JijumDownSize	= 10.0;
	pData->m_AddLoad		= 500;		// N/m2
	pData->m_bCheckSettlement = TRUE;//pBridge->m_nQtyJigan>1 ? TRUE : FALSE;
	pData->m_nModelingSettleMethod	= 1;
	m_JijumDownSize			= 10.0;
	m_AddLoad				= 500;		// N/m2

	ModelingCalc.MakeFrameModeling();
}

void CCalcModelingSetup::SetDataSave()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CGlobarOption *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	
	UpdateData(TRUE);

	pData->m_TotalWeight    = m_TotalWeight;
	pData->m_ModelWeight    = m_ModelWeight;
	pData->m_BonusRate      = m_BonusRate;
	pData->m_JijumDownSize  = m_JijumDownSize;
	pData->m_bCheckSettlement = m_bCheckSettlement;
	pData->m_AddLoad        = m_AddLoad;
	pData->m_nModelingSettleMethod	= m_comboSettle.GetCurSel();
	pOpt->SetAccelerationOfGravity(m_dAcceleration);
}

void CCalcModelingSetup::DrawInputDomyunView(BOOL bZoomAll)
{		
	CFEMManage	*pFEM	= m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CDomyun		*pDom	= m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	m_pStd->m_FEMDraw.ChangeFEM(pFEM);
	m_pStd->m_FEMDraw.InsertDomyunPoint();
	m_pStd->m_FEMDraw.SetRestraintScale(1);
	m_pStd->m_FEMDraw.SetJointSize(0.2);
	m_pStd->m_FEMDraw.SetGraientCountor(TRUE);

	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_NUMBER, TRUE);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_NUMBER, FALSE);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_RENDER_RESTRAINT, TRUE);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE, FALSE);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_LOADCASE, FALSE);

	m_pView->SetAxis('d');
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

//모델링 재생성
void CCalcModelingSetup::OnButtonMakemodel() 
{
	if(MessageBox("기존의 모델링 데이터가 모두 지워집니다.\n 계속 하시겠습니까", "모델링 재생성", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	CFemModelingCalc ModelingCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	ModelingCalc.MakeFrameModeling();

	DrawInputDomyunView(TRUE);
}

void CCalcModelingSetup::OnButtonRecommend() 
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	CCalcData		*pData		= m_pStd->GetDataManage()->GetCalcData();

	CFemModelingCalc ModelingCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	if(pCalcStd->GetSizeHLLFemData()<=0) return;

	m_dSteelMass	= Round(ModelingCalc.CalcBonusRate(),4);
	m_TotalWeight   = pData->m_TotalWeight;
	m_ModelWeight   = pData->m_ModelWeight;
	m_BonusRate     = pData->m_BonusRate;
	m_comboSettle.SetCurSel(pData->m_nModelingSettleMethod);

	UpdateData(FALSE);
	SetDataSave();
}

void CCalcModelingSetup::OnCheckSettlement() 
{
	UpdateData();

	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	pData->m_bCheckSettlement = m_bCheckSettlement;

	if(pData->m_bCheckSettlement)
		m_Edit5.EnableWindow(TRUE);
	else
		m_Edit5.EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CCalcModelingSetup::OnPreInitDialog()
{
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC7, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC8, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_9, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC10, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_11, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_STEEL_MASS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_MODELINIT_TOTALWEIGHT_EDIT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_MODELINIT_MODELWEIGHT_EDIT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_MODELINIT_BONUSRATE_EDIT, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_12, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_13, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_14, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_15, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_16, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_MODELINIT_ADDLOAD_EDIT, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_CHECK_SETTLEMENT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_17, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_18, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_19, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_MODELINIT_JIJUM_EDIT, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_20, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_GIRDER_CEN, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_GIRDER_LOWER, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_MAKEMODEL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_TIP, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_21, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_SETTLE, SZ_TOP_LEFT, SZ_TOP_LEFT);
}

void CCalcModelingSetup::OnRadioGirderCen() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	UpdateData(TRUE);
	pData->SetShoeGirderLower(!m_radioGirderCen.GetCheck());
}

void CCalcModelingSetup::OnRadioGirderLower() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	UpdateData(TRUE);
	pData->SetShoeGirderLower(!m_radioGirderCen.GetCheck());
}

void CCalcModelingSetup::OnSelchangeComboSettle() 
{
	UpdateData(TRUE);
	SetDataSave();
}
