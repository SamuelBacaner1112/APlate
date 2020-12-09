// DeckEtcRebarDlg.cpp : implementation file
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
// CDeckEtcRebarDlg dialog


CDeckEtcRebarDlg::CDeckEtcRebarDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckEtcRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckEtcRebarDlg)
	m_bApplyShear = TRUE;
	m_bApplyWaterHole = FALSE;
	//}}AFX_DATA_INIT

	m_nTypeWaterHoleBar = 0;
	m_nTypeWaterHole = 0;
}



void CDeckEtcRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckEtcRebarDlg)
	DDX_Control(pDX, IDC_COMBO_DRAIN_TYPE, m_comboWaterHoleType);
	DDX_Control(pDX, IDC_COMBO_SPACER_DIA, m_comboSpacerDia);
	DDX_Control(pDX, IDC_COMBO_DRAINBAR_DIA, m_comboWaterHoleBarDia);
	DDX_Control(pDX, IDC_COMBO_END_RFBAR_DIA, m_comboEndRFBarDia);
	DDX_Control(pDX, IDC_COMBO_BAR_COUNT, m_comboBarCnt);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboBarType);
	DDX_Control(pDX, IDC_EDIT_MAINBAR, m_nCntSetMainBar);
	DDX_Control(pDX, IDC_EDIT_DISTBAR, m_nCntSetDistBar);
	DDX_Control(pDX, IDC_EDIT_BAR_LENGTH, m_dLenWaterHoleBar);
	DDX_Control(pDX, IDC_EDIT_WATER_S, m_dAnchorLenWaterHoleBar);
	DDX_Control(pDX, IDC_EDIT_BAR_ANGLE, m_dAngleWaterHoleBar);
	DDX_Control(pDX, IDC_EDIT_BAR_SPACE, m_dSpaceWaterHoleBar);
	DDX_Control(pDX, IDC_EDIT_WATER_R, m_dRadiusWaterHoleBar);
	DDX_Control(pDX, IDC_EDIT_SPACER_LEN_A, m_dSpacerLenA);
	DDX_Control(pDX, IDC_EDIT_SPACER_LEN_B, m_dSpacerLenB);
	DDX_Control(pDX, IDC_EDIT_SPACER_LEN_D, m_dSpacerLenD);
	DDX_Control(pDX, IDC_EDIT_SPACER_COUNT, m_nCntSpacerInSquare);
	DDX_Control(pDX, IDC_EDIT_END_RFBAR_LENGTH, m_dLengthEndRFBar);
	DDX_Control(pDX, IDC_EDIT_END_RFBAR_RADIUS, m_dRadiusEndRFBar);
	DDX_Control(pDX, IDC_EDIT_END_RFBAR_ANGLE, m_dAngleEndRFBar);
	DDX_Control(pDX, IDC_EDIT_END_RFBAR_COUNT, m_nCntEndRFBar);
	DDX_Control(pDX, IDC_EDIT_MID_RFBAR_COUNT, m_nCntMidRFBar);
	DDX_Control(pDX, IDC_EDIT_CATCHDRAIN_COUNT, m_nWaterHolesu);
	DDX_Control(pDX, IDC_EDIT_SPACER_MAX_C, m_dSpacerMaxC);
	DDX_Control(pDX, IDC_EDIT_SPACER_MIN_C, m_dSpacerMinC);
	DDX_Control(pDX, IDC_EDIT_CATCHDRAIN_FLOOR, m_nWaterHoleDansu);
	DDX_Control(pDX, IDC_EDIT_CATCHDRAIN_WIDTH, m_dWaterHoleWidth);
	DDX_Control(pDX, IDC_EDIT_CATCHDRAIN_LENGTH, m_dWaterHoleLength);
	DDX_Check(pDX, IDC_CHECK_SHEAR, m_bApplyShear);
	DDX_Control(pDX, IDC_EDIT_YIELD_SHEAR, m_dSigmaYShear);
	DDX_Control(pDX, IDC_EDIT_YIELD_SPACER, m_dSigmaYSpacer);
	DDX_Control(pDX, IDC_EDIT_YIELD_WATER, m_dSigmaYWaterHole);
	DDX_Check(pDX, IDC_CHECK_WATERHOLE, m_bApplyWaterHole);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CDeckEtcRebarDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckEtcRebarDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_BAR_COUNT, OnSelchangeComboBarCount)
	ON_CBN_SELCHANGE(IDC_COMBO_DRAINBAR_DIA, OnSelchangeComboWaterHolebarDia)
	ON_CBN_SELCHANGE(IDC_COMBO_END_RFBAR_DIA, OnSelchangeComboEndRfbarDia)
	ON_CBN_SELCHANGE(IDC_COMBO_SPACER_DIA, OnSelchangeComboSpacerDia)
	ON_CBN_SELCHANGE(IDC_COMBO_DRAIN_TYPE, OnSelchangeComboWaterHoleType)
	ON_BN_CLICKED(IDC_CHECK_SHEAR, OnCheckShear)
	ON_BN_CLICKED(IDC_CHECK_WATERHOLE, OnCheckWaterhole)
	ON_EN_KILLFOCUS(IDC_EDIT_BAR_SPACE, OnKillfocusEditBarSpace)
	ON_EN_KILLFOCUS(IDC_EDIT_BAR_LENGTH, OnKillfocusEditBarLength)
	ON_EN_KILLFOCUS(IDC_EDIT_WATER_R, OnKillfocusEditWaterR)
	ON_EN_KILLFOCUS(IDC_EDIT_WATER_S, OnKillfocusEditWaterS)
	ON_EN_KILLFOCUS(IDC_EDIT_BAR_ANGLE, OnKillfocusEditBarAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_SPACER_LEN_A, OnKillfocusEditSpacerLenA)
	ON_EN_KILLFOCUS(IDC_EDIT_SPACER_LEN_B, OnKillfocusEditSpacerLenB)
	ON_EN_KILLFOCUS(IDC_EDIT_SPACER_LEN_D, OnKillfocusEditSpacerLenD)
	ON_EN_KILLFOCUS(IDC_EDIT_END_RFBAR_ANGLE, OnKillfocusEditEndRfbarAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_END_RFBAR_COUNT, OnKillfocusEditEndRfbarCount)
	ON_EN_KILLFOCUS(IDC_EDIT_END_RFBAR_LENGTH, OnKillfocusEditEndRfbarLength)
	ON_EN_KILLFOCUS(IDC_EDIT_END_RFBAR_RADIUS, OnKillfocusEditEndRfbarRadius)
	ON_EN_KILLFOCUS(IDC_EDIT_MID_RFBAR_COUNT, OnKillfocusEditMidRfbarCount)
	ON_EN_KILLFOCUS(IDC_EDIT_CATCHDRAIN_COUNT, OnKillfocusEditWaterHoleCount)
	ON_EN_KILLFOCUS(IDC_EDIT_SPACER_MAX_C, OnKillfocusEditSpacerMaxC)
	ON_EN_KILLFOCUS(IDC_EDIT_SPACER_MIN_C, OnKillfocusEditSpacerMinC)
	ON_EN_KILLFOCUS(IDC_EDIT_MAINBAR, OnKillfocusEditSpacerMainbar)
	ON_EN_KILLFOCUS(IDC_EDIT_DISTBAR, OnKillfocusEditSpacerDistbar)
	ON_EN_KILLFOCUS(IDC_EDIT_SPACER_COUNT, OnKillfocusEditSpacerCount)
	ON_EN_KILLFOCUS(IDC_EDIT_CATCHDRAIN_FLOOR, OnKillfocusEditWaterHoleFloor)
	ON_EN_KILLFOCUS(IDC_EDIT_CATCHDRAIN_LENGTH, OnKillfocusEditWaterHoleLength)
	ON_EN_KILLFOCUS(IDC_EDIT_CATCHDRAIN_WIDTH, OnKillfocusEditWaterHoleWidth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckEtcRebarDlg message handlers

void CDeckEtcRebarDlg::SetDataDefault()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;
	CString			sText		= _T("");

	pDeckData->m_bInstallShearRebar = (pDB->GetGirder(-1)->m_pSangse->m_StudHori_Type == 1) ? TRUE : FALSE;
	m_bApplyShear					= pDeckData->m_bInstallShearRebar;

	/************************************************************************/
	/* ������ ����ö��                                                      */
	/************************************************************************/
	if(pDeckData->m_bInstallShearRebar)
	{
		//1. �������� 
		sText.Format("%d",10);

		m_nCntEndRFBar.SetWindowText(sText);

		MakeShearBarDan();
		pDeckData->m_ShearRebarDanbu.SetDataDefault(TRUE);

		//2. �߰������� 
		sText.Format("%d", 20);

		m_nCntMidRFBar.SetWindowText(sText);

		MakeShearBarCen();
		pDeckData->m_ShearRebarJijum.SetDataDefault(TRUE);
	}

	/************************************************************************/
	/* ����� ����ö��                                                      */
	/************************************************************************/
	if(pDeckData->m_bInstallWaterHole)
	{
		m_nTypeWaterHole = 1; //�ڽ��� 

		sText.Format("%d",	long(pDeckData->GetLengthSlabTotal(JONGBASEPOS_SLABCENTER)/10000) * 2);
		
		m_nWaterHolesu.SetWindowText(sText);

		m_nWaterHolesu.GetWindowText(sText);
		pDeckData->m_nWaterHolesu = atoi(sText);

		sText.Format("%d", 1);

		m_nWaterHoleDansu.SetWindowText(sText);

		m_nWaterHoleDansu.GetWindowText(sText);

		pDeckData->m_nWaterHoleDansu = atoi(sText);
		pDeckData->m_nTypeWaterHole = 0;		//Round Ÿ��
		m_nTypeWaterHoleBar = pDeckData->m_nTypeWaterHole;
		pDeckData->m_WaterHoleRoundType.SetDataDefault(TRUE);
		pDeckData->m_WaterHoleBoxType.SetDataDefault(TRUE);
	}

	/************************************************************************/
	/* ������                                                               */
	/************************************************************************/
	pDeckData->m_SpacerRebar.SetDataDefault(TRUE);
	//�ִ� 
	double dMax = pDeckData->m_pDB->m_dThickSlabBase + pDeckData->m_pDB->m_dHeightTaperStt
		- pDeckData->m_dCoverUp - pDeckData->m_dCoverDn;
	pDeckData->m_SpacerRebar.SetRebarLength_C(dMax);
	double dMin = pDeckData->m_pDB->m_dThickSlabBase - pDeckData->m_pDB->m_dThickSlabHunch
		- pDeckData->m_dCoverUp - pDeckData->m_dCoverDn;
	pDeckData->m_SpacerRebar.SetRebarLength_E(dMin); 

	ChangeViewControlWaterHoleBar();
	ChangeViewControlWaterHole();
	UpdateData(FALSE);

	// �׺�����
	pDeckData->m_ShearRebarDanbu.SetFy(4000);
	pDeckData->m_ShearRebarJijum.SetFy(4000);
	pDeckData->m_WaterHoleRoundType.SetFy(4000);
	pDeckData->m_WaterHoleBoxType.SetFy(4000);
	pDeckData->m_SpacerRebar.SetFy(4000);
}


void CDeckEtcRebarDlg::SetDataInit()
{
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	SetComboDia();

	SetDataInitWaterHoleBar();		//����� ö�� 
	SetDataInitShearBar();			//��������ö�� 
	SetDataInitSpacer();			//������ 	

	sText.Format("%g",	pDeckData->m_ShearRebarDanbu.GetFy());

	m_dSigmaYShear.SetWindowText(sText);


	sText.Format("%g",	pDeckData->m_WaterHoleRoundType.GetFy());
	m_dSigmaYWaterHole.SetWindowText(sText);

	sText.Format("%g",	pDeckData->m_SpacerRebar.GetFy());
	m_dSigmaYSpacer.SetWindowText(sText);

	//ö���׺������� �����̴�.
	SetDlgItemText(IDC_STATIC_YIELD_GROUP, "�׺�����(MPa)");

	m_dSigmaYShear.GetWindowText(sText);

	sText.Format("%g",	atof(sText) / 10);
	m_dSigmaYShear.SetWindowText(sText);

	m_dSigmaYWaterHole.GetWindowText(sText);
	sText.Format("%g",	atof(sText) / 10);

	m_dSigmaYWaterHole.SetWindowText(sText);

	m_dSigmaYSpacer.GetWindowText(sText);

	sText.Format("%g",	atof(sText) / 10);
	m_dSigmaYSpacer.SetWindowText(sText);

	UpdateData(FALSE);
}

void CDeckEtcRebarDlg::SetDataSave()
{
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	pDeckData->m_bInstallShearRebar = m_bApplyShear;
	pDeckData->m_bInstallWaterHole  = m_bApplyWaterHole;
	
	SetDataSaveShearBar();
	SetDataSaveSpacer();

	/************************************************************************/
	/* ö���� �׺����� ����                                                 */
	/************************************************************************/
	UpdateData(TRUE);

	
	m_dSigmaYShear.GetWindowText(sText);

	sText.Format("%g",	atof(sText)	* 10);
	m_dSigmaYShear.SetWindowText(sText);

	m_dSigmaYWaterHole.GetWindowText(sText);

	sText.Format("%g",	atof(sText) * 10);
	m_dSigmaYWaterHole.SetWindowText(sText);

	m_dSigmaYSpacer.GetWindowText(sText);
	sText.Format("%g",	atof(sText) * 10);
	m_dSigmaYSpacer.SetWindowText(sText);

	m_dSigmaYShear.GetWindowText(sText);

	pDeckData->m_ShearRebarDanbu.SetFy(atof(sText));
	pDeckData->m_ShearRebarJijum.SetFy(atof(sText));

	m_dSigmaYWaterHole.GetWindowText(sText);
	pDeckData->m_WaterHoleRoundType.SetFy(atof(sText));

	m_dSigmaYWaterHole.GetWindowText(sText);

	pDeckData->m_WaterHoleBoxType.SetFy(atof(sText));

	m_dSigmaYSpacer.GetWindowText(sText);
	pDeckData->m_SpacerRebar.SetFy(atof(sText));


	if(pDeckData->m_bRecalcJewon && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag())
		pDeckData->MakeRebarJewon();
}


void CDeckEtcRebarDlg::OnPreInitDialog()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	m_bApplyShear = pDeckData->m_bInstallShearRebar;
	m_bApplyWaterHole  = pDeckData->m_bInstallWaterHole;

	//ö������ ���� 
	SetComboBarType();
	SetComboBarCount();
	ChangeViewControlShearBar();
	ChangeViewControlWaterHoleBar();
	ChangeViewControlWaterHole();

	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_MIDDLE_CENTER);
	SetResize(IDC_CHECK_SHEAR, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC7, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC8, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_END_RFBAR_DIA, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_END_RFBAR_RADIUS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_END_RFBAR_LENGTH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_END_RFBAR_ANGLE, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC9, SZ_MIDDLE_LEFT, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC10, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC11, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC12, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC13, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_END_RFBAR_COUNT, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_MID_RFBAR_COUNT, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_STATIC14, SZ_MIDDLE_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC15, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_DRAIN_WIDTH, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_CATCHDRAIN_COUNT, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_CATCHDRAIN_WIDTH, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC16, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC17, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC18, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_DRAIN_LENGTH, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_COMBO_DRAIN_TYPE, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_CATCHDRAIN_LENGTH, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_DRAIN_UNIT, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_STATIC_YIELD_GROUP, SZ_MIDDLE_LEFT, SZ_BOTTOM_CENTER);
	SetResize(IDC_STATIC_Y1, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_Y2, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_Y3, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_YIELD_SHEAR, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_YIELD_SPACER, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_EDIT_YIELD_WATER, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_STATIC19, SZ_TOP_CENTER, SZ_MIDDLE_RIGHT);
	SetResize(IDC_STATIC20, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC_MAINBAR, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC_DISTBAR, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC21, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_COMBO_SPACER_DIA, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_MAINBAR, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_DISTBAR, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_SPACER_COUNT, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC22, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC23, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC_SPACER_COUNT, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC24, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC25, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC26, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC27, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC28, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_SPACER_LEN_A, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_SPACER_LEN_B, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_SPACER_MAX_C, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_SPACER_MIN_C, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_SPACER_LEN_D, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC29, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC30, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC31, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC32, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC33, SZ_TOP_CENTER, SZ_TOP_CENTER);

	SetResize(IDC_STATIC34, SZ_MIDDLE_CENTER, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_WATERHOLE, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC35, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC36, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC37, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_COMBO_TYPE, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_COMBO_DRAINBAR_DIA, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_EDIT_CATCHDRAIN_FLOOR, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC38, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC39, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC40, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC_RADIUS, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_EDIT_BAR_SPACE, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_COMBO_BAR_COUNT, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_EDIT_WATER_R, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC41, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC42, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC_RADIUS_UNIT, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC_LENGTH, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC_ANCHORAGE, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC_ANGLE, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_EDIT_BAR_LENGTH, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_EDIT_WATER_S, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_EDIT_BAR_ANGLE, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC43, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC_ANCHOR_UNIT, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_STATIC44, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);

	UpdateData(FALSE);
}


void CDeckEtcRebarDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData			*pDeckData		= m_pStd->m_pDeckData;	
	CDrawOpenRebar		DrawOpenRebar(m_pStd->m_pDataManage);
	CDrawSpacer			DrawSpacer(m_pStd->m_pDataManage);
	CDrawSupportRFBar	DrawShearBar(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);

	CDomyun *pDom = m_pView->GetDomyun();
	double dScale = 40;
	pDom->SetScaleDim(dScale);
	CDomyun Dom(pDom);
	CDRect rect;
	pDom->ClearEtt(TRUE);	
	/************************************************************************/
	/* ���� ����ö��                                                        */
	/************************************************************************/
	if(pDeckData->m_bInstallShearRebar)
	{
		double dDisDetail = 1500;

		if(pDeckData->m_ShearRebarDanbu.GetSize() > 0)
		{
			//1. �������� 
			DrawShearBar.DrawRebar(&Dom, 0);
			DrawShearBar.DimRebar(&Dom, 0);
			DrawShearBar.MarkRebar(&Dom, 0);
			DrawShearBar.DrawDetailCircle(&Dom, 0);
			DrawShearBar.DrawDetailCircleText(&Dom, 0, "�� A");
			DrawShearBar.DrawDetail(&Dom, 0, dDisDetail);
			
			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			DrawStd.AddTitleOnDom(&Dom, dScale, rect, "�������� ����ö��", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
			Dom.Move(Dom.Always(50), Dom.Always(150));
			*pDom << Dom;
		}

		//2. �߰������� 
		if(pDB->m_nQtyJigan > 1 && pDeckData->m_ShearRebarJijum.GetSize() > 0)
		{
			DrawShearBar.DrawRebar(&Dom, 1);
			DrawShearBar.DimRebar(&Dom, 1);
			DrawShearBar.MarkRebar(&Dom, 1);
			DrawShearBar.DrawDetailCircle(&Dom, 1);
			DrawShearBar.DrawDetailCircleText(&Dom, 1, "�� B");
			DrawShearBar.DrawDetail(&Dom, 1, dDisDetail);

			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			DrawStd.AddTitleOnDom(&Dom, dScale, rect, "�߰������� ����ö��", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
			Dom.Move(Dom.Always(200), Dom.Always(150));
			*pDom << Dom;
		}
	}

	/************************************************************************/
	/* ����� ����ö��                                                      */
	/************************************************************************/
	if(pDeckData->m_bInstallWaterHole)
	{
		DrawOpenRebar.SetType(pDeckData->m_nTypeWaterHole);
		DrawOpenRebar.DrawOpen(&Dom);
		DrawOpenRebar.DrawRebar(&Dom);
		DrawOpenRebar.DimRebar(&Dom);
		DrawOpenRebar.MarkRebar(&Dom);

		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();
		DrawStd.AddTitleOnDom(&Dom, dScale, rect, "����� ����ö��", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
		Dom.Move(Dom.Always(80), Dom.Always(30));
		*pDom << Dom;
	}

	/************************************************************************/
	/* ������                                                               */
	/************************************************************************/
	//1. ���鵵 
	dScale	= 20;
	Dom.SetScaleDim(dScale);
	DrawSpacer.DrawFrontRebar(&Dom);
	DrawSpacer.DimFrontRebar(&Dom, TRUE);
	DrawSpacer.MarkRebar(&Dom);
	Dom.RedrawByScale(pDom->GetScaleDim()/dScale);//Ȯ��
	Dom.Move(Dom.Always(400), Dom.Always(40));
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawStd.AddTitleOnDom(&Dom, dScale, rect, "������", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
	*pDom << Dom;		
	
	//2. ��鵵 
	Dom.SetScaleDim(dScale);
	DrawSpacer.DrawPlanRebar(&Dom);
	DrawSpacer.DimPlanRebar(&Dom, TRUE);
	Dom.RedrawByScale(pDom->GetScaleDim()/dScale);//Ȯ��
	Dom.Move(Dom.Always(400), Dom.Always(20));
	*pDom << Dom;

	//3. �����Է� 
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawSpacer.DrawPlaceRebar(&Dom);
	Dom.Move(Dom.Always(300), Dom.Always(40));
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}


BOOL CDeckEtcRebarDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}

//������ ������ ���� 
void CDeckEtcRebarDlg::SetDataInitSpacer()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	//1. ���漳�� 
	int nIdx = GetRebarIndexBySizeARoad(pDeckData->m_SpacerRebar.GetRebarDiameter());
	m_comboSpacerDia.SetCurSel(nIdx);  

	CString sText = _T("");

	
	sText.Format("%d",	pDeckData->m_SpacerRebar.GetRebarCntInSquare());

	m_nCntSpacerInSquare.SetWindowText(sText);

	sText.Format("%d", pDeckData->m_SpacerRebar.GetCntMainRebarSet());
	m_nCntSetMainBar.SetWindowText(sText);

	sText.Format("%d", pDeckData->m_SpacerRebar.GetCntDistRebarSet());
	m_nCntSetDistBar.SetWindowText(sText);

	//2. ö�ٱ��� ���� 
	
	sText.Format("%g",	pDeckData->m_SpacerRebar.GetRebarLength_A());
	
	m_dSpacerLenA.SetWindowText(sText);

	sText.Format("%g",	pDeckData->m_SpacerRebar.GetRebarLength_B());

	m_dSpacerLenB.SetWindowText(sText);

	sText.Format("%g", pDeckData->m_SpacerRebar.GetRebarLength_C());
	m_dSpacerMaxC.SetWindowText(sText);

	sText.Format("%g",	pDeckData->m_SpacerRebar.GetRebarLength_E());
	m_dSpacerMinC.SetWindowText(sText);

	sText.Format("%g",	pDeckData->m_SpacerRebar.GetRebarLength_D());
	m_dSpacerLenD.SetWindowText(sText);

	//���� ���� 
	double dMain = (pDeckData->m_dGenMainRebarCTC[0]/2)*(pDeckData->m_SpacerRebar.GetCntMainRebarSet());	
	double dDist = (pDeckData->m_dGenSupportRebarCTC[0])*(pDeckData->m_SpacerRebar.GetCntDistRebarSet());
	double dTotal = Round(dMain*dDist/1000000, 3);
	CString str;
	str.Format("��/%0.3f��", dTotal);
	SetDlgItemText(IDC_STATIC_SPACER_COUNT, str);

	UpdateData(FALSE);
}


//Spacer �κ� ���� 
void CDeckEtcRebarDlg::SetDataSaveSpacer()
{
	UpdateData(TRUE);
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	CString strDia;
	int nIdx = 	m_comboSpacerDia.GetCurSel();	
	m_comboSpacerDia.GetLBText(nIdx, strDia);
	double dDia = atof(strDia.Right(2));
	pDeckData->m_SpacerRebar.SetRebarDiameter(dDia);

}


//���� ���� ö�� 
void CDeckEtcRebarDlg::SetDataInitShearBar()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	CString strDia	= _T("");
	CString sText	= _T("");
	int		nIdx	= 0;
	double	dDia	= 0;

	/************************************************************************/
	/* ���ܺ���ö��                                                         */
	/************************************************************************/
	//1. ���� 
	dDia = pDeckData->m_ShearRebarDanbu.GetDiaRFBar();
	nIdx = GetRebarIndexBySizeARoad(dDia);
	m_comboEndRFBarDia.SetCurSel(nIdx);
	//2. ��ü����

	sText.Format("%g",	pDeckData->m_ShearRebarDanbu.GetLenRFBar());

	m_dLengthEndRFBar.SetWindowText(sText);

	//3. ������ 

	sText.Format("%g",	pDeckData->m_ShearRebarDanbu.GetArcRadiusRFBar());
	m_dRadiusEndRFBar.SetWindowText(sText);

	//4. ����
	double dAng = pDeckData->m_ShearRebarDanbu.GetArcAngleRFBar();
	
	sText.Format("%g", 2*(90 - dAng/2));

	m_dAngleEndRFBar.SetWindowText(sText);

	//5. ����
	//5-1 �������� 
	sText.Format("%d",	pDeckData->m_ShearRebarDanbu.GetSize());
	
	m_nCntEndRFBar.SetWindowText(sText);

	//5-2 �߰������� 
	sText.Format("%d",	pDeckData->m_ShearRebarJijum.GetSize());

	m_nCntMidRFBar.SetWindowText(sText);

	UpdateData(FALSE);
}


void CDeckEtcRebarDlg::SetDataSaveShearBar()
{
	UpdateData(TRUE);

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	if(!pDeckData->m_bInstallShearRebar)
		return;

	/************************************************************************/
	/* ��������                                                            */
	/************************************************************************/
	//1. ��ü����

	m_dLengthEndRFBar.GetWindowText(sText);

	pDeckData->m_ShearRebarDanbu.SetLenRFBar(atof(sText));

	//2. ������ 
	m_dRadiusEndRFBar.GetWindowText(sText);
	pDeckData->m_ShearRebarDanbu.SetArcRadiusRFBar(atof(sText));

	//3. ����

	m_dAngleEndRFBar.GetWindowText(sText);

	double dAng = 2*(90 - atof(sText)/2);
	pDeckData->m_ShearRebarDanbu.SetArcAngleRFBar(dAng);

	/************************************************************************/
	/* �߰�������                                                          */
	/************************************************************************/
	//1. ��ü����
	
	m_dLengthEndRFBar.GetWindowText(sText);

	pDeckData->m_ShearRebarJijum.SetLenRFBar(atof(sText));
	
	//2. ������ 
	m_dRadiusEndRFBar.GetWindowText(sText);
	pDeckData->m_ShearRebarJijum.SetArcRadiusRFBar(atof(sText));

	//3. ����
	pDeckData->m_ShearRebarJijum.SetArcAngleRFBar(dAng);
}


void CDeckEtcRebarDlg::OnSelchangeComboType() 
{
	UpdateData();
	// TODO: Add your control notification handler code here
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_nTypeWaterHoleBar = m_comboBarType.GetCurSel();
	pDeckData->m_nTypeWaterHole = m_nTypeWaterHoleBar;

	ChangeViewControlWaterHole();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}


void CDeckEtcRebarDlg::ChangeViewControlWaterHoleBar()
{
	if(m_bApplyWaterHole)
	{
		GetDlgItem(IDC_EDIT_WATER_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BAR_ANGLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WATER_S)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WATER_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BAR_ANGLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WATER_S)->EnableWindow(TRUE);

		if(m_nTypeWaterHoleBar == 0)	//������ ö�� 
		{
			GetDlgItem(IDC_EDIT_WATER_R)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_BAR_ANGLE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_WATER_S)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_STATIC_ANCHORAGE, "��������");
			GetDlgItem(IDC_STATIC_ANCHOR_UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_RADIUS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_RADIUS_UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ANGLE)->ShowWindow(SW_SHOW);
		}
		else if(m_nTypeWaterHoleBar == 1)  //�ڽ��� ö�� 
		{
			GetDlgItem(IDC_EDIT_WATER_R)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_BAR_ANGLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_WATER_S)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_STATIC_ANCHORAGE, "�Ǻ��β�(P)");
			GetDlgItem(IDC_STATIC_ANCHOR_UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_RADIUS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_RADIUS_UNIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_ANGLE)->ShowWindow(SW_HIDE);
		}

	}
	else
	{
		GetDlgItem(IDC_EDIT_WATER_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BAR_ANGLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WATER_S)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WATER_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BAR_ANGLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WATER_S)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}


//����� 
void CDeckEtcRebarDlg::SetDataInitWaterHoleBar()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CString		sText		= _T("");

	/************************************************************************/
	/* ����� ö��                                                          */
	/************************************************************************/
	m_comboBarType.SetCurSel(m_nTypeWaterHoleBar);

	sText.Format("%d",	pDeckData->m_nWaterHolesu);
	m_nWaterHolesu.SetWindowText(sText);

	sText.Format("%d",	pDeckData->m_nWaterHoleDansu);
	m_nWaterHoleDansu.SetWindowText(sText);

	if(m_nTypeWaterHoleBar == 0)  //��� ö�� 
	{
		m_comboWaterHoleType.SetCurSel(pDeckData->m_WaterHoleRoundType.GetOpenType());
		if(m_nTypeWaterHole == 0)  //����
		{
			sText.Format("%g",	pDeckData->m_WaterHoleRoundType.GetOpenRadius());
			m_dWaterHoleWidth.SetWindowText(sText);
		}
		else
		{
			sText.Format("%g",	pDeckData->m_WaterHoleRoundType.GetWidthOpen());
			m_dWaterHoleWidth.SetWindowText(sText);
			
			sText.Format("%g",	pDeckData->m_WaterHoleRoundType.GetLengthOpen());
			m_dWaterHoleLength.SetWindowText(sText);
		}

		int nIdx = GetRebarIndexBySizeARoad(pDeckData->m_WaterHoleRoundType.GetDiaRebar());
		m_comboWaterHoleBarDia.SetCurSel(nIdx);  //����
		m_comboBarCnt.SetCurSel(pDeckData->m_WaterHoleRoundType.GetCntRebar()-1);   //ö�� ���� 

		sText.Format("%g",pDeckData->m_WaterHoleRoundType.GetSpaceRebar());

		m_dSpaceWaterHoleBar.SetWindowText(sText);			//ö�� ���� 

		double dAng = pDeckData->m_WaterHoleRoundType.GetAngle();				//Arc�� ���� 

		sText.Format("%g", 2 * (90 - dAng / 2));
		m_dAngleWaterHoleBar.SetWindowText(sText);

		sText.Format("%g",	pDeckData->m_WaterHoleRoundType.GetLengthAnchorage());
		m_dAnchorLenWaterHoleBar.SetWindowText(sText);	//ö�� ���� ���� 

		sText.Format("%g",	pDeckData->m_WaterHoleRoundType.GetRadius());

		m_dRadiusWaterHoleBar.SetWindowText(sText);		//ö�� ARC�κ� ������

		sText.Format("%g", pDeckData->m_WaterHoleRoundType.GetLengthRebar());
		m_dLenWaterHoleBar.SetWindowText(sText);		//ö�� ��ü ���� 
	}
	else //�ڽ��� ö�� 
	{
		m_comboWaterHoleType.SetCurSel(pDeckData->m_WaterHoleBoxType.GetOpenType());
		if(m_nTypeWaterHole == 0)  //����
		{
			sText.Format("%g",	pDeckData->m_WaterHoleBoxType.GetOpenRadius());
			m_dWaterHoleWidth.SetWindowText(sText);
		}
		else
		{
			sText.Format("%g",	pDeckData->m_WaterHoleBoxType.GetWidthOpen());
			m_dWaterHoleWidth.SetWindowText(sText);
			
			sText.Format("%g",	pDeckData->m_WaterHoleBoxType.GetLengthOpen());
			m_dWaterHoleLength.SetWindowText(sText);
		}

		int nIdx = GetRebarIndexBySizeARoad(pDeckData->m_WaterHoleBoxType.GetDiaRebar());
		m_comboWaterHoleBarDia.SetCurSel(nIdx);
		m_comboBarCnt.SetCurSel(pDeckData->m_WaterHoleBoxType.GetCntRebar()-1);  //ö�� ���� 


		sText.Format("%g", pDeckData->m_WaterHoleBoxType.GetSpaceRebar());

		m_dSpaceWaterHoleBar.SetWindowText(sText);   //ö�� ���� 

		sText.Format("%g",	pDeckData->m_WaterHoleBoxType.GetLengthRebar());

		m_dLenWaterHoleBar.SetWindowText(sText);

		sText.Format("%g",	pDeckData->m_WaterHoleBoxType.GetCover());

		m_dAnchorLenWaterHoleBar.SetWindowText(sText);  //�Ǻ��β� 
	}

	ChangeViewControlWaterHoleBar();
	UpdateData(FALSE);
}



void CDeckEtcRebarDlg::SetComboDia()
{
	m_dArrDia.RemoveAll();

	m_dArrDia.Add(13);
	m_dArrDia.Add(16);
	m_dArrDia.Add(19);
	m_dArrDia.Add(22);
	m_dArrDia.Add(25);
	m_dArrDia.Add(29);
	m_dArrDia.Add(32);
	m_dArrDia.Add(35);

	SetComboDiaWaterHole();
	SetComboDiaSpacer();
	SetComboDiaShearBar();
}


void CDeckEtcRebarDlg::SetDataSaveWaterHoleBar()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	if(!pDeckData->m_bInstallWaterHole)
		return;
	CString strDia	= _T("");
	CString sText	= _T("");
	/************************************************************************/
	/* ö��                                                                 */
	/************************************************************************/
	int nIdx = 	m_comboWaterHoleBarDia.GetCurSel();	
	m_comboWaterHoleBarDia.GetLBText(nIdx, strDia);
	double dDia = atof(strDia.Right(2));

	if(m_nTypeWaterHoleBar == 0)  //��� ö�� 
	{
		pDeckData->m_WaterHoleRoundType.SetDiaRebar(dDia);
		int nCntBar = m_comboBarCnt.GetCurSel();
		pDeckData->m_WaterHoleRoundType.SetCntRebar(nCntBar+1);				//ö�� ���� 

		m_dSpaceWaterHoleBar.GetWindowText(sText);

		pDeckData->m_WaterHoleRoundType.SetSpaceRebar(atof(sText));	//ö�� ���� 
		
		m_dAnchorLenWaterHoleBar.GetWindowText(sText);

		pDeckData->m_WaterHoleRoundType.SetLengthAnchorage(atof(sText));  //ö���� ������

		m_dRadiusWaterHoleBar.GetWindowText(sText);
		pDeckData->m_WaterHoleRoundType.SetLengthRebar(atof(sText)); //������ 

		m_dAngleWaterHoleBar.GetWindowText(sText);

		double dAng = 2*(90 - atof(sText) / 2);
		pDeckData->m_WaterHoleRoundType.SetAngle(dAng);			//ö���� ���� 

		m_dLenWaterHoleBar.GetWindowText(sText);
		pDeckData->m_WaterHoleRoundType.SetLengthRebar(atof(sText));
	}
	else //�ڽ��� ö�� 
	{
		pDeckData->m_WaterHoleBoxType.SetDiaRebar(dDia);
		int nCntBar = m_comboBarCnt.GetCurSel();
		pDeckData->m_WaterHoleBoxType.SetCntRebar(nCntBar+1);				//ö�� ���� 

		m_dSpaceWaterHoleBar.GetWindowText(sText);

		pDeckData->m_WaterHoleBoxType.SetSpaceRebar(atof(sText));	//ö�� ���� 

		m_dLenWaterHoleBar.GetWindowText(sText);
		pDeckData->m_WaterHoleBoxType.SetLengthRebar(atof(sText));

		m_dAnchorLenWaterHoleBar.GetWindowText(sText);

		pDeckData->m_WaterHoleBoxType.SetCover(atof(sText));
	}
}

//ö�ٰ��� 
void CDeckEtcRebarDlg::OnSelchangeComboBarCount() 
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	// TODO: Add your control notification handler code here
	if(m_nTypeWaterHoleBar == 0)  //��� ö�� 
	{
		int nCntBar = m_comboBarCnt.GetCurSel();
		pDeckData->m_WaterHoleRoundType.SetCntRebar(nCntBar+1);				//ö�� ���� 
	}
	else //�ڽ��� ö�� 
	{
		int nCntBar = m_comboBarCnt.GetCurSel();
		pDeckData->m_WaterHoleBoxType.SetCntRebar(nCntBar+1);				//ö�� ���� 
	}

	DrawInputDomyunView(TRUE);
}

//ö�ٰ��� 
void CDeckEtcRebarDlg::OnKillfocusEditBarSpace()
{

	// TODO: Add your control notification handler code here
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	UpdateData(TRUE);

	m_dSpaceWaterHoleBar.GetWindowText(sText);

	if(m_nTypeWaterHoleBar == 0)  //��� ö�� 
		pDeckData->m_WaterHoleRoundType.SetSpaceRebar(atof(sText));	//ö�� ���� 
	else //�ڽ��� ö�� 
		pDeckData->m_WaterHoleBoxType.SetSpaceRebar(atof(sText));	//ö�� ���� 

	DrawInputDomyunView(TRUE);
}


//��ü ö�� ���� 
void CDeckEtcRebarDlg::OnKillfocusEditBarLength()
{

	// TODO: Add your control notification handler code here
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CString		sText		= _T("");

	UpdateData(TRUE);

	m_dLenWaterHoleBar.GetWindowText(sText);

	if(m_nTypeWaterHoleBar == 0)  //��� ö�� 
		pDeckData->m_WaterHoleRoundType.SetLengthRebar(atof(sText)); //ö�� ���� 
	else //�ڽ��� ö�� 
		pDeckData->m_WaterHoleBoxType.SetLengthRebar(atof(sText));	//ö�� ���� 

	DrawInputDomyunView(TRUE);
}

//������ 
void CDeckEtcRebarDlg::OnKillfocusEditWaterR()
{
	// TODO: Add your control notification handler code here
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	UpdateData(TRUE);

	m_dRadiusWaterHoleBar.GetWindowText(sText);

	pDeckData->m_WaterHoleRoundType.SetRadius(atof(sText)); //������ 

	DrawInputDomyunView(TRUE);	
}


//��������  or �Ǻ��β� 
void CDeckEtcRebarDlg::OnKillfocusEditWaterS()
{
	// TODO: Add your control notification handler code here
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	UpdateData(TRUE);

	m_dAnchorLenWaterHoleBar.GetWindowText(sText);

	if(m_nTypeWaterHoleBar == 0)  //Round �� 
		pDeckData->m_WaterHoleRoundType.SetLengthAnchorage(atof(sText)); //������ 
	else
		pDeckData->m_WaterHoleBoxType.SetCover(atof(sText)); //������ 

	DrawInputDomyunView(TRUE);	
}

void CDeckEtcRebarDlg::SetComboBarType()
{
	m_comboBarType.ResetContent();
	//����� ö�� Ÿ�� 
	m_comboBarType.AddString("A Type");
	m_comboBarType.AddString("B Type");
	//����� Ÿ�� 
	m_comboWaterHoleType.AddString("����");
	m_comboWaterHoleType.AddString("�簢��");

}

void CDeckEtcRebarDlg::SetComboBarCount()
{
	m_comboBarCnt.ResetContent();
	CString str;

	for(int i = 0; i < 10; i++)
	{
		str.Format("%d",i+1);
		m_comboBarCnt.AddString(str);
	}
}


//����� ���� ö�� ���� �Է� 
void CDeckEtcRebarDlg::OnKillfocusEditBarAngle()
{
	// TODO: Add your control notification handler code here
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	UpdateData(TRUE);

	m_dAngleWaterHoleBar.GetWindowText(sText);

	double dAng = 2 * (90 - atof(sText) / 2);

	pDeckData->m_WaterHoleRoundType.SetAngle(dAng); //���� 

	DrawInputDomyunView(TRUE);		
}

void CDeckEtcRebarDlg::OnKillfocusEditSpacerLenA()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dSpacerLenA.GetWindowText(sText);
	pDeckData->m_SpacerRebar.SetRebarLength_A(atof(sText));

	DrawInputDomyunView(TRUE);
}


void CDeckEtcRebarDlg::OnKillfocusEditSpacerLenB()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CString		sText		= _T("");

	m_dSpacerLenB.GetWindowText(sText);
	pDeckData->m_SpacerRebar.SetRebarLength_B(atof(sText));

	DrawInputDomyunView(TRUE);
	
}

void CDeckEtcRebarDlg::OnKillfocusEditSpacerLenD()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dSpacerLenD.GetWindowText(sText);
	pDeckData->m_SpacerRebar.SetRebarLength_D(atof(sText));

	DrawInputDomyunView(TRUE);
}

void CDeckEtcRebarDlg::OnKillfocusEditEndRfbarAngle()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dAngleEndRFBar.GetWindowText(sText);

	double		dAng		= 2*(90 - atof(sText)/2);

	pDeckData->m_ShearRebarDanbu.SetArcAngleRFBar(dAng);
	pDeckData->m_ShearRebarJijum.SetArcAngleRFBar(dAng);

	DrawInputDomyunView(TRUE);
}

//���ܺ���ö�� �������� ���� 
void CDeckEtcRebarDlg::OnKillfocusEditEndRfbarCount()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MakeShearBarDan();

	CString sText =_T("");
	
	m_nCntEndRFBar.GetWindowText(sText);

	if(atoi(sText) > 0)
	{
		//������ �ٲٸ� ��� ���� �ʱ�ȭ �ȴ�.
		//���� ���� ����� ���� �ٽ� ���� ������Ѿ� �Ѵ�.
		CString		sText		= _T("");
		CADeckData	*pDeckData	= m_pStd->m_pDeckData;

		m_dLengthEndRFBar.GetWindowText(sText);

		pDeckData->m_ShearRebarDanbu.SetLenRFBar(atof(sText));

		m_dAngleEndRFBar.GetWindowText(sText);

		double dAng = 2*(90 - atof(sText)/2);
		pDeckData->m_ShearRebarDanbu.SetArcAngleRFBar(dAng);

		m_dRadiusEndRFBar.GetWindowText(sText);
		pDeckData->m_ShearRebarDanbu.SetArcRadiusRFBar(atof(sText));

		CString strDia;
		int nIdx = 	m_comboEndRFBarDia.GetCurSel();	
		m_comboEndRFBarDia.GetLBText(nIdx, strDia);
		double dDia = atof(strDia.Right(2));
		pDeckData->m_ShearRebarDanbu.SetDiaRFBar(dDia);
	}

	DrawInputDomyunView(TRUE);
}

void CDeckEtcRebarDlg::OnKillfocusEditEndRfbarLength()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dLengthEndRFBar.GetWindowText(sText);

	pDeckData->m_ShearRebarDanbu.SetLenRFBar(atof(sText));
 	pDeckData->m_ShearRebarJijum.SetLenRFBar(atof(sText));	

	DrawInputDomyunView(TRUE);
}

void CDeckEtcRebarDlg::OnKillfocusEditEndRfbarRadius()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CString		sText		=_T("");

	m_dRadiusEndRFBar.GetWindowText(sText);

	pDeckData->m_ShearRebarDanbu.SetArcRadiusRFBar(atof(sText));
	pDeckData->m_ShearRebarJijum.SetArcRadiusRFBar(atof(sText));

	DrawInputDomyunView(TRUE);
}


void CDeckEtcRebarDlg::OnKillfocusEditMidRfbarCount()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MakeShearBarCen();
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	m_nCntMidRFBar.GetWindowText(sText);

	if(atoi(sText) > 0)
	{

		m_dLengthEndRFBar.GetWindowText(sText);
		pDeckData->m_ShearRebarJijum.SetLenRFBar(atof(sText));
		
		m_dAngleEndRFBar.GetWindowText(sText);

		double dAng = 2*(90 - atof(sText)/2);
		pDeckData->m_ShearRebarJijum.SetArcAngleRFBar(dAng);

		m_dRadiusEndRFBar.GetWindowText(sText);

		pDeckData->m_ShearRebarJijum.SetArcRadiusRFBar(atof(sText));	

		CString strDia;
		int nIdx = 	m_comboEndRFBarDia.GetCurSel();	
		m_comboEndRFBarDia.GetLBText(nIdx, strDia);
		double dDia = atof(strDia.Right(2));
		pDeckData->m_ShearRebarJijum.SetDiaRFBar(dDia);
	}

	DrawInputDomyunView(TRUE);
}


//����� ö�� ���� 
void CDeckEtcRebarDlg::OnSelchangeComboWaterHolebarDia() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CString strDia;
	int nIdx = m_comboWaterHoleBarDia.GetCurSel();
	m_comboWaterHoleBarDia.GetLBText(nIdx, strDia);
	double dDia = (double)atoi(strDia.Right(2));
	if(m_nTypeWaterHoleBar == 0)
		pDeckData->m_WaterHoleRoundType.SetDiaRebar(dDia);
	else
		pDeckData->m_WaterHoleBoxType.SetDiaRebar(dDia);

	CRebar* pRB = pDeckData->GetRebarByName("���������ö��");
	if(pRB)	pRB->m_dDia = dDia;

	DrawInputDomyunView(TRUE);	
}

void CDeckEtcRebarDlg::OnSelchangeComboEndRfbarDia() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CString strDia;
	int nIdx = 	m_comboEndRFBarDia.GetCurSel();	
	m_comboEndRFBarDia.GetLBText(nIdx, strDia);
	double dDia = atof(strDia.Right(2));
	pDeckData->m_ShearRebarDanbu.SetDiaRFBar(dDia);
	pDeckData->m_ShearRebarJijum.SetDiaRFBar(dDia);

	CRebar* pRB = pDeckData->GetRebarByName("���ܺ���ö��");
	if(pRB)	pRB->m_dDia = dDia;

	DrawInputDomyunView(TRUE);	
}



void CDeckEtcRebarDlg::OnSelchangeComboSpacerDia() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	CString strDia;
	int nIdx = 	m_comboSpacerDia.GetCurSel();	
	m_comboSpacerDia.GetLBText(nIdx, strDia);
	double dDia = atof(strDia.Right(2));
	pDeckData->m_SpacerRebar.SetRebarDiameter(dDia);

	CRebar* pRB = pDeckData->GetRebarByName("������");
	if(pRB)	pRB->m_dDia = dDia;

	DrawInputDomyunView(TRUE);		
}


void CDeckEtcRebarDlg::MakeShearBarDan()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	pDeckData->m_ShearRebarDanbu.RemoveRebarPos();  //ö����ġ ������ �����Ѵ�.
	
	CDPointArray	ptArrStud	= pDeckData->GetPosFirstStud();
	double			dSpace		= Round(pDeckData->m_pDB->GetGirder(0)->m_pSangse->m_StudJong_MinDis, 1);
	CString			sText		= _T("");

	m_nCntEndRFBar.GetWindowText(sText);

	for(int i = 0 ; i < atoi(sText); i++)
	{
		CSupportRFBar rfBar;
		
		for(int j = 0; j < ptArrStud.GetSize(); j++)
		{
			CDPoint ptStud = ptArrStud.GetAt(j);
			ptStud.x += dSpace;
			ptArrStud.SetAt(j, ptStud);
		}

		rfBar.SetPosStud(ptArrStud);
		pDeckData->m_ShearRebarDanbu.AddRFBar(rfBar);
	}	
}

//�߰� ����ö�ٸ� ���� �����.
void CDeckEtcRebarDlg::MakeShearBarCen()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	pDeckData->m_ShearRebarJijum.RemoveRebarPos();  //��Ʈ�� Ŭ�������� ���ܺ���ö�� ��ü�� ��� �����.
	
	CDPointArray	ptArrStud	= pDeckData->GetPosFirstStud();
	double			dSpace		= Round(pDeckData->m_pDB->GetGirder(0)->m_pSangse->m_StudJong_MinDis, 1);
	CString			sText		= _T("");

	m_nCntMidRFBar.GetWindowText(sText);

	for(int i = 0 ; i < atoi(sText); i++)
	{
		CSupportRFBar rfBar;
		
		for(int j = 0; j < ptArrStud.GetSize(); j++)
		{
			CDPoint ptStud = ptArrStud.GetAt(j);
			ptStud.x += dSpace;
			ptArrStud.SetAt(j, ptStud);
		}

		rfBar.SetPosStud(ptArrStud);
		pDeckData->m_ShearRebarJijum.AddRFBar(rfBar);
	}	
}


void CDeckEtcRebarDlg::OnKillfocusEditWaterHoleCount()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;	

	m_nWaterHolesu.GetWindowText(sText);
	pDeckData->m_nWaterHolesu = atoi(sText);
}

void CDeckEtcRebarDlg::OnKillfocusEditSpacerMaxC()
{
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	CString		sText		= _T("");
 	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dSpacerMaxC.GetWindowText(sText);

 	pDeckData->m_SpacerRebar.SetRebarLength_C(atoi(sText));
 	DrawInputDomyunView(TRUE);	
}

void CDeckEtcRebarDlg::OnKillfocusEditSpacerMinC()
{
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	CString		sText		= _T("");
 	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	m_dSpacerMinC.GetWindowText(sText);

 	pDeckData->m_SpacerRebar.SetRebarLength_E(atof(sText));
 	DrawInputDomyunView(TRUE);	
}

//��ö�� ���� 
void CDeckEtcRebarDlg::OnKillfocusEditSpacerMainbar()
{
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	
	CString		sText		= _T("");
 	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_nCntSetMainBar.GetWindowText(sText);
	pDeckData->m_SpacerRebar.SetCntMainRebarSet(atoi(sText));	

	SetDataInitSpacer();
 	DrawInputDomyunView(TRUE);		
}

//���ö�� ���� 
void CDeckEtcRebarDlg::OnKillfocusEditSpacerDistbar()
{
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	CString		sText		= _T("");
 	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_nCntSetDistBar.GetWindowText(sText);
	pDeckData->m_SpacerRebar.SetCntDistRebarSet(atoi(sText));

	SetDataInitSpacer();
 	DrawInputDomyunView(TRUE);		
}

void CDeckEtcRebarDlg::OnKillfocusEditSpacerCount()
{
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	CString		sText		= _T("");
 	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_nCntSpacerInSquare.GetWindowText(sText);
	pDeckData->m_SpacerRebar.SetRebarCntInSquare(atoi(sText));	

	DrawInputDomyunView(TRUE);
}

void CDeckEtcRebarDlg::ChangeViewControlShearBar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
 	CADeckData *pDeckData = m_pStd->m_pDeckData;
	if(pDeckData->m_bInstallShearRebar)//���ܺ���ö�� ����
	{
		GetDlgItem(IDC_COMBO_END_RFBAR_DIA)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_END_RFBAR_LENGTH)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_END_RFBAR_RADIUS)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_END_RFBAR_ANGLE)->EnableWindow(SW_SHOW);
		//�������� 
		GetDlgItem(IDC_EDIT_END_RFBAR_COUNT)->EnableWindow(SW_SHOW);
		//�߰������� 
		if(pDB->m_nQtyJigan < 2)
			GetDlgItem(IDC_EDIT_MID_RFBAR_COUNT)->EnableWindow(SW_HIDE);
		else
			GetDlgItem(IDC_EDIT_MID_RFBAR_COUNT)->EnableWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_COMBO_END_RFBAR_DIA)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_END_RFBAR_LENGTH)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_END_RFBAR_RADIUS)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_END_RFBAR_ANGLE)->EnableWindow(SW_HIDE);
		//�������� 
		GetDlgItem(IDC_EDIT_END_RFBAR_COUNT)->EnableWindow(SW_HIDE);
		//�߰������� 
		GetDlgItem(IDC_EDIT_MID_RFBAR_COUNT)->EnableWindow(SW_HIDE);
	}
}

void CDeckEtcRebarDlg::SetComboDiaSpacer()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CString strType, strDia;
	m_comboSpacerDia.ResetContent();	

	strType = GetCharRebarMark(pDeckData->m_SpacerRebar.GetFy());

	for(int i = 0; i < m_dArrDia.GetSize(); i++)
	{
		double dDia = m_dArrDia.GetAt(i);
		strDia.Format("%s%0.0f", strType, dDia);
		m_comboSpacerDia.InsertString(i, strDia);
	}
}


void CDeckEtcRebarDlg::SetComboDiaShearBar()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CString strType, strDia;
	m_comboEndRFBarDia.ResetContent();	

	strType = GetCharRebarMark(pDeckData->m_ShearRebarDanbu.GetFy());

	for(int i = 0; i < m_dArrDia.GetSize(); i++)
	{
		double dDia = m_dArrDia.GetAt(i);
		strDia.Format("%s%0.0f", strType, dDia);
		m_comboEndRFBarDia.InsertString(i, strDia);
	}
}


void CDeckEtcRebarDlg::SetComboDiaWaterHole()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CString strType, strDia;
	m_comboWaterHoleBarDia.ResetContent();	

	strType = GetCharRebarMark(pDeckData->m_WaterHoleRoundType.GetFy());

	for(int i = 0; i < m_dArrDia.GetSize(); i++)
	{
		double dDia = m_dArrDia.GetAt(i);
		strDia.Format("%s%0.0f", strType, dDia);
		m_comboWaterHoleBarDia.InsertString(i, strDia);
	}
}

//�ܼ� 
void CDeckEtcRebarDlg::OnKillfocusEditWaterHoleFloor()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;	
	
	m_nWaterHoleDansu.GetWindowText(sText);
	pDeckData->m_nWaterHoleDansu = atoi(sText);
}

//����� Ÿ�� 
void CDeckEtcRebarDlg::OnSelchangeComboWaterHoleType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CADeckData *pDeckData = m_pStd->m_pDeckData;	
	m_nTypeWaterHole = m_comboWaterHoleType.GetCurSel();
	pDeckData->m_WaterHoleRoundType.SetOpenType(m_nTypeWaterHole);
	pDeckData->m_WaterHoleBoxType.SetOpenType(m_nTypeWaterHole);

	ChangeViewControlWaterHole();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}


//����� ���� ���� 
void CDeckEtcRebarDlg::OnKillfocusEditWaterHoleLength()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;	
	
	m_dWaterHoleLength.GetWindowText(sText);

	if(pDeckData->m_nTypeWaterHole == 0) //���� 
		pDeckData->m_WaterHoleRoundType.SetLengthOpen(atof(sText));
	else //�ڽ� 
		pDeckData->m_WaterHoleBoxType.SetLengthOpen(atof(sText));

	DrawInputDomyunView(TRUE);
}


//����� ���� ���̿� ����Ÿ���϶� ���� 
void CDeckEtcRebarDlg::OnKillfocusEditWaterHoleWidth()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;	

	m_dWaterHoleWidth.GetWindowText(sText);

	if(m_nTypeWaterHole == 0) //������� ���� 
	{
		pDeckData->m_WaterHoleRoundType.SetOpenRadius(atof(sText));
		pDeckData->m_WaterHoleBoxType.SetOpenRadius(atof(sText));
	}
	else //������� �ڽ��� 
	{
		pDeckData->m_WaterHoleRoundType.SetWidthOpen(atof(sText));
		pDeckData->m_WaterHoleBoxType.SetWidthOpen(atof(sText));
	}

	DrawInputDomyunView(TRUE);
}


void CDeckEtcRebarDlg::ChangeViewControlWaterHole()
{
	if(m_bApplyWaterHole)
	{
		GetDlgItem(IDC_EDIT_CATCHDRAIN_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DRAIN_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_YIELD_WATER)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DRAINBAR_DIA)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_FLOOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BAR_SPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_BAR_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BAR_LENGTH)->EnableWindow(TRUE);

		if(m_nTypeWaterHole == 0)	//���� 
		{
			SetDlgItemText(IDC_STATIC_DRAIN_WIDTH, "������");
			GetDlgItem(IDC_STATIC_DRAIN_LENGTH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_CATCHDRAIN_LENGTH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_DRAIN_UNIT)->ShowWindow(SW_HIDE);
		}
		else if(m_nTypeWaterHole == 1)  //�簢��  
		{
			SetDlgItemText(IDC_STATIC_DRAIN_WIDTH, "����");
			GetDlgItem(IDC_STATIC_DRAIN_LENGTH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_CATCHDRAIN_LENGTH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_DRAIN_UNIT)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		GetDlgItem(IDC_EDIT_CATCHDRAIN_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DRAIN_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_YIELD_WATER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DRAINBAR_DIA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CATCHDRAIN_FLOOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BAR_SPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_BAR_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BAR_LENGTH)->EnableWindow(FALSE);
	}

}



void CDeckEtcRebarDlg::OnCheckShear() 
{
	UpdateData(TRUE);

	CADeckData *pDeckData	= m_pStd->m_pDeckData;
	CString		sText		= _T("");

	pDeckData->m_bInstallShearRebar	= m_bApplyShear;
	if(pDeckData->m_bInstallShearRebar)
	{
		//1. �������� 

		sText.Format("%d", 10);

		m_nCntEndRFBar.SetWindowText(sText);

		MakeShearBarDan();
		pDeckData->m_ShearRebarDanbu.SetDataDefault(TRUE);

		//2. �߰������� 

		sText.Format("%d", 20);

		m_nCntMidRFBar.SetWindowText(sText);

		MakeShearBarCen();
		pDeckData->m_ShearRebarJijum.SetDataDefault(TRUE);
	}

	SetDataInit();
	DrawInputDomyunView(TRUE);

	ChangeViewControlShearBar();
}

void CDeckEtcRebarDlg::OnCheckWaterhole() 
{
	UpdateData(TRUE);

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	pDeckData->m_bInstallWaterHole = m_bApplyWaterHole;
	if(pDeckData->m_bInstallWaterHole)
	{
		m_nTypeWaterHole = 1; //�ڽ��� 

		sText.Format("%d",	long(pDeckData->GetLengthSlabTotal(JONGBASEPOS_SLABCENTER)/10000) * 2);
		m_nWaterHolesu.SetWindowText(sText);

		m_nWaterHolesu.GetWindowText(sText);

		pDeckData->m_nWaterHolesu = atoi(sText);

		sText.Format("%d", 1);
		m_nWaterHoleDansu.SetWindowText(sText);

		m_nWaterHoleDansu.GetWindowText(sText);

		pDeckData->m_nWaterHoleDansu = atoi(sText);
		pDeckData->m_nTypeWaterHole = 0;		//Round Ÿ��
		m_nTypeWaterHoleBar = pDeckData->m_nTypeWaterHole;
		pDeckData->m_WaterHoleRoundType.SetDataDefault(TRUE);
		pDeckData->m_WaterHoleBoxType.SetDataDefault(TRUE);
	}

	SetDataInit();
	DrawInputDomyunView(TRUE);

	ChangeViewControlWaterHole();	
	ChangeViewControlWaterHoleBar();	
}
