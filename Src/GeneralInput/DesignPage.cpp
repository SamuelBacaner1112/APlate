// DesignPage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateCalc/APlateCalc.h"
#include "DesignPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignPage dialog
#define	GetStr(bApp)	(bApp ? "����":"�������")
#define	GetApp(sApp)	(sApp=="����" ? 1 : 0)

SHGRIDTITLE CDesignPage::m_gt[ROW_COUNT_ETC] = 
	{	
		{"�� ��",												"%s",	300, DT_LEFT, TRUE},	// COL_TOTAL_TYPE
		{"1.  ��������",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_KIND
		{"2.  �������",										"%s",	300, DT_LEFT, FALSE},	// COL_STEELSTAND
//		{"2.  ��������",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_HYUNGTAI
		{"3.  �������",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_GRADE
//		{"4.  �ؼ����",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_ANALYSIS
		{"4.  �Ŵ� ���� ���� ����",								"%s",	300, DT_LEFT, FALSE},	// COL_STD_GIRHEIGHT_UPPER
		{"5.  ���� �����β� (mm)",								"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_MIN_UP
		{"6.  ���� �����β� (mm)",								"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_MIN_Lo
		{"7.  ������ �����β� (mm)",							"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_MIN_WEB
		{"8.  ���� �β� ���� ��� ����",						"%s",	300, DT_LEFT, FALSE},	// COL_HEU_INTHICK
		{"9.  �������� �ִ��߷� (kN)",							"%.lf",	300, DT_LEFT, FALSE},	// COL_WEIGHT_MAX_CAR
		{"10. �������� �ִ���� (mm)",							"%.lf",	300, DT_LEFT, FALSE},	// COL_LEN_MAX_SPACE
		{"11. �Ŵ����� ��ȣ",									"%s",	300, DT_LEFT, FALSE},	// COL_GIR_STT_NUM
		{"12. �������� ��ȣ",									"%s",	300, DT_LEFT, FALSE},	// COL_SPACE_NUM
		{"13. �߷�,STATION,ELEVATION,��ǥ ����",				"%s",	300, DT_LEFT, FALSE},	// COL_WEIGHT_STA_ELE_CODI
		{"14. ���� �� ���ܱ��� ǥ�� �ڸ���",				"%.0f",	300, DT_LEFT, FALSE},	// COL_JARISU_PYEN_JONG	
		{"15. �ʷ� ���� ġ�� ����",								"%s",	300, DT_LEFT, FALSE},	// COL_STD_FILLET_WELD
		{"16. �ʷ� ���� �ּ� ���� (mm)",						"%.lf",	300, DT_LEFT, FALSE},	// COL_LEN_MIN_FILLET_WELD				
		{"17. ������ ���� ���� �β��� (mm)",					"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_WELD_MUGAISUN		
		{"18. X ���� ���� ���� �β� : �Էµβ� ���� V���� ����","%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_WELD_XLINE
		{"19. ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����",			"%s",	300, DT_LEFT, FALSE},	// COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN
		{"20. ��Ʈ��ġ ������ ���ܸ� Ÿ��",						"%s",	300, DT_LEFT, FALSE},	// COL_BOLT_BACHI_WEBPAN_BYUN_TYPE
		{"21. ��Ʈ��ġ ������ ���κ�",							"%s",	300, DT_LEFT, FALSE},	// COL_BOLT_BACHI_WEBPAN_CROSS
		{"22. ���� ������ ��� ��� ���� (3%)",					"%.lf",	300, DT_LEFT, FALSE},	// COL_STD_WEB_EUPAN_JONG		
		{"23. ���� ������ �Ϻ� ��� ���� (3%)",					"%.lf",	300, DT_LEFT, FALSE},	// COL_STD_WEB_EUPAN_BYUN
		{"24. ������ ������� ����(�ݿø� �ڸ���)",				"%.lf",	300, DT_LEFT, FALSE},	// COL_JARISU_UNIT_KYUNGSA
		{"25. ���κ� ����� ��� Ÿ��",							"%s",	300, DT_LEFT, FALSE},	// COL_TYPE_CROSS_BRAKET			
		{"26. ���κ� ���򺸰��� �������ġ ��ġ",				"%s",	300, DT_LEFT, FALSE},	// COL_SULCHI_CROSS_HSTIFF_BRAKET
		{"27. ����극�̽� ���� ����",							"%s",	300, DT_LEFT, FALSE},	// COL_STD_HBRACING_HG_DIR
		{"28. �ٴ��� �߾Ӻ� ö�� �����",						"%s",	300, DT_LEFT, FALSE},	// COL_CALC_FLOORCENTER_BAR
		{"29. �������",										"%s",	300, DT_LEFT, FALSE},	// COL_EARTHQUACK_GRADE
		{"30. �������� ����",									"%s",	300, DT_LEFT, FALSE},	// COL_EARTHQUACK_ZONE
		{"31. �������� �˻�",									"%s",	300, DT_LEFT, FALSE},	// COL_FASTLINESEARCH
		{"32. �극�̽� ������ ������ ��ġ",						"%s",	300, DT_LEFT, FALSE},	// COL_INSTALLBEAMATCEN
		{"33. ���տ��� : ������ �νĿ� ���� ȯ������",		"%s",	300, DT_LEFT, FALSE},	// COL_ALLOW_CRACK_WIDTH
		{"34. TU�Ŵ��� ���������� ����÷��� �̰� �ɼ�",		"%.lf", 300, DT_LEFT, FALSE}    //COL_INSULATIONVSTIFFUF
		
	};

CDesignPage::CDesignPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignPage)
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_pDesignPageDlg = (CDesignPageDlg*)pParent;
}

void CDesignPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDesignPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignPage)	
	//}}AFX_MSG_MAP
 	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignPage message handlers
void CDesignPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

CString CDesignPage::GetBoltArrage(long nIdx)
{
	if(nIdx==0) return "��Ʈ �յ��ġ";
	if(nIdx==1) return "��Ʈ �Ϲݹ�ġ";
	if(nIdx==2) return "��Ʈ �����ġ";
	return "";
}

long CDesignPage::GetBoltArrage(CString szStr)
{
	if(szStr=="��Ʈ �յ��ġ") return 0;
	if(szStr=="��Ʈ �Ϲݹ�ġ") return 1;
	if(szStr=="��Ʈ �����ġ") return 2;
	ASSERT(TRUE);
	return -1;
}

void CDesignPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;
	 CGlobarOption*   pOpt = m_pDataManage->GetGlobalOption();

	// �׸��� �⺻ ���� 
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(ROW_COUNT_ETC);
	m_Grid.SetColumnCount(2);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);			
	m_Grid.InputShGridTitle(m_gt, ROW_COUNT_ETC, FALSE, FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetColumnWidth(1,250);
	m_Grid.SetRowHeightAll(20);	
	
	if(pOpt->GetDesignBridgeType() != 2 )//TU�Ŵ���
		m_Grid.SetItemState(COL_INSULATIONVSTIFFUF,1,GVIS_READONLY);
	else
		m_Grid.SetItemState(COL_INSULATIONVSTIFFUF,1,GVIS_MODIFIED);

}

void CDesignPage::SetDataDefault()
{
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	long nBridgeType = pOpt->GetDesignBridgeType();
	pOpt->InitDesignBasic();
	pOpt->SetDesignBridgeType(nBridgeType);

	m_EarthQUackGradeDlg.m_nType	= 0;
	m_EarhtQuackDlg.m_nType			= 0;
	pDB->m_bQuickLineSearch			= TRUE;
	pDB->m_sDeungeub				= _T("1 ���");
	//pDB->m_NumHaiseokCombo			= 0;
	pOpt->SetSteelStandardYear(APPLYYEAR2008);
	
	SetDataInit();
	SetDataSave();
}

void CDesignPage::SetDataSave()
{
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();

	//��������	
	pOpt->SetStrDesignCondition(m_Grid.GetTextMatrix(COL_BRIDGE_KIND, 1));	
//	pOpt->SetStrDesignBridgeType(m_Grid.GetTextMatrix(COL_BRIDGE_HYUNGTAI, 1));	
//	pDB->SetBridgeType(pOpt->GetDesignBridgeType());
	if(pOpt->GetDesignBridgeType()!=0)
		pData->DESIGN_MATERIAL.m_dSigmaCK	= 40.0;
	else
		pData->DESIGN_MATERIAL.m_dSigmaCK	= 27.0;
	
	SetBridgeGrade(m_Grid.GetTextMatrix(COL_BRIDGE_GRADE, 1));
	pOpt->SetStrSteelStandardYear(m_Grid.GetTextMatrix(COL_STEELSTAND, 1));
	//SetBridgeAnalysis(m_Grid.GetTextMatrix(COL_BRIDGE_ANALYSIS, 1));

	pOpt->SetDesignMinThickOfUDFlate(m_Grid.GetTextMatrixDouble(COL_THICK_MIN_UP, 1), TRUE);			//���� �����β�
	pOpt->SetDesignMinThickOfUDFlate(m_Grid.GetTextMatrixDouble(COL_THICK_MIN_Lo, 1), FALSE);			//���� �����β�
	pOpt->SetDesignMinThickOfWebFlate(m_Grid.GetTextMatrixDouble(COL_THICK_MIN_WEB, 1));			//������ �����β�
	pOpt->SetDesignAllowDeflection(m_Grid.GetTextMatrix(COL_HEU_INTHICK,1));
	pOpt->SetDesignMaxWeightOfCarLoad(frkN(m_Grid.GetTextMatrixDouble(COL_WEIGHT_MAX_CAR, 1)));		// �������� �ִ��߷�
	pOpt->SetDesignMaxLengthOfFieldSplice(m_Grid.GetTextMatrixDouble(COL_LEN_MAX_SPACE, 1));		// �������� �ִ����
	pOpt->SetStrDesignDimensionOfFilletWeld(m_Grid.GetTextMatrix(COL_STD_FILLET_WELD, 1));			// �ʷ� ���� ġ�� ����
	pOpt->SetDesignMinLenOfFilletWeld(m_Grid.GetTextMatrixDouble(COL_LEN_MIN_FILLET_WELD, 1));		// �ʷ� ���� �ּұ���
	pOpt->SetDesignThickWeldNoCut(m_Grid.GetTextMatrixDouble(COL_THICK_WELD_MUGAISUN, 1));			// 10������ ���� ���� �β���
	pOpt->SetDesignThickXWeldApply(m_Grid.GetTextMatrixDouble(COL_THICK_WELD_XLINE, 1));			// X ���� ���� ���� �β� 
	//��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
	pOpt->SetDesignBoltHoleJigJaegOfHorPlate(m_Grid.GetTextMatrix(COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN, 1) == "������� Ÿ��" ? 0 : 1);

	pOpt->SetDesignBoltHoleArrangeByunSectionType(GetBoltArrage(m_Grid.GetTextMatrix(COL_BOLT_BACHI_WEBPAN_BYUN_TYPE, 1)));	//��Ʈ��ġ ������ ���ܸ� Ÿ��
	pOpt->SetDesignBoltHoleArrangeCrossBeam(GetBoltArrage(m_Grid.GetTextMatrix(COL_BOLT_BACHI_WEBPAN_CROSS, 1)));			//��Ʈ��ġ ������ ���κ�

	pOpt->SetDesignBoltJongVSlopApplyUp(m_Grid.GetTextMatrixDouble(COL_STD_WEB_EUPAN_JONG, 1));		//���� ������ ���� ��� ����
	pOpt->SetDesignBoltJongVSlopApplyDn(m_Grid.GetTextMatrixDouble(COL_STD_WEB_EUPAN_BYUN, 1));		//���� ������ ���ܸ� ��� ����
	pOpt->SetStrDesignCrossBeamBracketType(m_Grid.GetTextMatrix(COL_TYPE_CROSS_BRAKET, 1));	
	pOpt->SetStrDesignCrossBeamHStiffAtBracket(m_Grid.GetTextMatrix(COL_SULCHI_CROSS_HSTIFF_BRAKET, 1));			
	pOpt->SetDesignStartNumberOfGirder(m_Grid.GetTextMatrixLong(COL_GIR_STT_NUM, 1) - 1);
	pOpt->SetDesignStartNumberOfSplice(m_Grid.GetTextMatrixLong(COL_SPACE_NUM, 1) - 1);
	pOpt->SetStrUnitTonfMeter(m_Grid.GetTextMatrix(COL_WEIGHT_STA_ELE_CODI,1));	
	pOpt->SetDesignPointSlope(m_Grid.GetTextMatrixLong(COL_JARISU_PYEN_JONG,1));	
	pOpt->SetDesignByundanVSlopRoundPos((long)m_Grid.GetTextMatrixDouble(COL_JARISU_UNIT_KYUNGSA, 1));	//������ ������� ����	
	//��Ÿ

	///< �����Ŵ� �������κ� ��ġ���� - ����
	pDB->m_nInstallVStiffOnCrossBeam = 0;

	int nUpperLowerVal = 0;
	CString szTemp = m_Grid.GetTextMatrix(COL_STD_GIRHEIGHT_UPPER,1);
	if(szTemp == GH_STR_UILI) nUpperLowerVal = 0;
	if(szTemp == GH_STR_UOLO) nUpperLowerVal = 1;
	if(szTemp == GH_STR_UILO) nUpperLowerVal = 2;
	if(szTemp == GH_STR_UOLI) nUpperLowerVal = 3;
	pOpt->SetDesignHeigtBase(nUpperLowerVal); 
	
	pOpt->SetStrDesignHBracingDirChange(m_Grid.GetTextMatrix(COL_STD_HBRACING_HG_DIR, 1));

	pOpt->SetStrSlabCentDesignMethod(m_Grid.GetTextMatrix(COL_CALC_FLOORCENTER_BAR,1));//�ٴ��� �߾Ӻ� ö�� �����
	if(m_Grid.GetTextMatrix(COL_EARTHQUACK_GRADE, 1)=="����I ���")
	{
		m_EarthQUackGradeDlg.m_nType = 0;
		pOpt->SetDegreeEarthQuake(0);
	}
	else
	{
		m_EarthQUackGradeDlg.m_nType = 1;
		pOpt->SetDegreeEarthQuake(1);
	}
	if(m_Grid.GetTextMatrix(COL_EARTHQUACK_ZONE, 1)=="I ����")
	{
		m_EarhtQuackDlg.m_nType = 0;
		pOpt->SetRegionEarthQuake(0);
	}
	else
	{
		m_EarhtQuackDlg.m_nType = 1;
		pOpt->SetRegionEarthQuake(1);
	}
	
	pDB->m_bQuickLineSearch	= m_Grid.GetTextMatrix(COL_FASTLINESEARCH, 1) == "����" ? TRUE : FALSE;
	pOpt->SetStrInstallBeamAtCenter(m_Grid.GetTextMatrix(COL_INSTALLBEAMATCEN, 1));
	pOpt->SetStrDesignAllowCrackWidthEnv(m_Grid.GetTextMatrix(COL_ALLOW_CRACK_WIDTH, 1));	
	pOpt->SetInsulationVStiffUF(m_Grid.GetTextMatrixDouble(COL_INSULATIONVSTIFFUF,1));
}

void CDesignPage::SetDataInit()
{
	SetGridTitle();

	CGlobarOption	*pOpt = m_pDataManage->GetGlobalOption();
	CPlateBridgeApp	*pDB = m_pDataManage->GetBridge();

	CString str=_T("");	

	m_Grid.SetGridData(m_gt ,COL_TOTAL_TYPE,			1, "������");
	m_Grid.SetGridData(m_gt, COL_BRIDGE_KIND,			1, pOpt->GetStrDesignCondition());	
	m_Grid.SetGridData(m_gt, COL_STEELSTAND,			1, pOpt->GetStrSteelStandardYear());
//	m_Grid.SetGridData(m_gt, COL_BRIDGE_HYUNGTAI,		1, pOpt->GetStrDesignBridgeType());	
	m_Grid.SetGridData(m_gt, COL_BRIDGE_GRADE,			1, GetStrBridgeGrade(pDB->m_sDeungeub));
//	m_Grid.SetGridData(m_gt, COL_BRIDGE_ANALYSIS,		1, GetStrBridgeAnalysis(pDB->m_NumHaiseokCombo));
	m_Grid.SetGridData(m_gt, COL_THICK_MIN_UP,			1, pOpt->GetDesignUDFlangeMinThick(TRUE));
	m_Grid.SetGridData(m_gt, COL_THICK_MIN_Lo,			1, pOpt->GetDesignUDFlangeMinThick(FALSE));
	m_Grid.SetGridData(m_gt, COL_THICK_MIN_WEB,			1, pOpt->GetDesignWebFalngeMinThick());
	m_Grid.SetGridData(m_gt, COL_HEU_INTHICK,			1, GetStr(pOpt->GetDesignAllowDeflection()));
	m_Grid.SetGridData(m_gt, COL_WEIGHT_MAX_CAR,		1, tokN(pOpt->GetDesignMaxWeightOfCarLoad()));
	m_Grid.SetGridData(m_gt, COL_LEN_MAX_SPACE,			1, pOpt->GetDesignMaxLengthOfFieldSplice());	
	m_Grid.SetGridData(m_gt, COL_STD_FILLET_WELD,		1, pOpt->GetStrDesignDimensionOfFilletWeld());			
	m_Grid.SetGridData(m_gt, COL_LEN_MIN_FILLET_WELD,	1, pOpt->GetDesignMinLenOfFilletWeld());
	m_Grid.SetGridData(m_gt, COL_THICK_WELD_MUGAISUN,	1, pOpt->GetDesignThickWeldNoCut());
	m_Grid.SetGridData(m_gt, COL_THICK_WELD_XLINE,		1, pOpt->GetDesignThickXWeldApply());
	m_Grid.SetGridData(m_gt, COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN,	1, pOpt->GetDesignBoltHoleJigJaegOfHorPlate()==0 ? "������� Ÿ��" : "�Ϲ�Ÿ��");			
	m_Grid.SetGridData(m_gt, COL_BOLT_BACHI_WEBPAN_BYUN_TYPE,	1, GetBoltArrage(pOpt->GetDesignBoltHoleArrangeByunSectionType()));
	m_Grid.SetGridData(m_gt, COL_BOLT_BACHI_WEBPAN_CROSS,		1, GetBoltArrage(pOpt->GetDesignBoltHoleArrangeCrossBeam()));	
	m_Grid.SetGridData(m_gt, COL_STD_WEB_EUPAN_JONG,	1, pOpt->GetDesignBoltJongVSlopApplyUp());
	m_Grid.SetGridData(m_gt, COL_STD_WEB_EUPAN_BYUN,	1, pOpt->GetDesignBoltJongVSlopApplyDn());
	m_Grid.SetGridData(m_gt, COL_TYPE_CROSS_BRAKET,		1, pOpt->GetStrDesignCrossBeamBracketType());	
	m_Grid.SetGridData(m_gt, COL_SULCHI_CROSS_HSTIFF_BRAKET,	1, pOpt->GetStrDesignCrossBeamHStiffAtBracket());
	str.Format("%ld",pOpt->GetDesignStartNumberOfGirder() + 1);
	m_Grid.SetGridData(m_gt, COL_GIR_STT_NUM,			1, str);
	str.Format("%ld",pOpt->GetDesignStartNumberOfSplice() + 1);
	m_Grid.SetGridData(m_gt, COL_SPACE_NUM,				1, str);
	m_Grid.SetGridData(m_gt, COL_WEIGHT_STA_ELE_CODI,	1, pOpt->GetStrUnitTonfMeter());
	m_Grid.SetGridData(m_gt, COL_JARISU_PYEN_JONG,		1, pOpt->GetDesignPointSlope());
	m_Grid.SetGridData(m_gt, COL_JARISU_UNIT_KYUNGSA,	1, pOpt->GetDesignByundanVSlopRoundPos());

	CString szTemp = _T("");
	switch(pOpt->GetDesignHeigtBase())
	{
	case 0: szTemp = GH_STR_UILI; break;
	case 1: szTemp = GH_STR_UOLO; break;
	case 2: szTemp = GH_STR_UILO; break;
	case 3: szTemp = GH_STR_UOLI; break;
	}
	m_Grid.SetTextMatrix(COL_STD_GIRHEIGHT_UPPER,	1, szTemp , DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_STD_HBRACING_HG_DIR,	1, pOpt->GetStrDesignHBracingDirChange(),	DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_CALC_FLOORCENTER_BAR,	1, pOpt->GetStrSlabCentDesignMethod(),		DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_EARTHQUACK_GRADE,		1, pOpt->GetStrDegreeEarthQuake(), DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_EARTHQUACK_ZONE,		1, pOpt->GetStrRegionEarthQuake(), DT_LEFT, FALSE);

	// Set UserDialog
	m_GirHeightUserDlg.bIsTU = pDB->IsTUGir();
	m_Grid.SetUserComboDialog(COL_STD_GIRHEIGHT_UPPER,	1,&m_GirHeightUserDlg);
	m_Grid.SetUserComboDialog(COL_EARTHQUACK_GRADE,		1,&m_EarthQUackGradeDlg);
	m_Grid.SetUserComboDialog(COL_EARTHQUACK_ZONE,		1,&m_EarhtQuackDlg);
	m_Grid.SetTextMatrix(COL_FASTLINESEARCH,		1, pDB->m_bQuickLineSearch ? "����" : "�������", DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_FASTLINESEARCH,		1, pDB->m_bQuickLineSearch ? "����" : "�������", DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_INSTALLBEAMATCEN,		1, pOpt->GetStrInstallBeamAtCenter(),		DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_ALLOW_CRACK_WIDTH,		1, pOpt->GetStrDesignAllowCrackWidthEnv(),	DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_INSULATIONVSTIFFUF,    1,"%.lf",pOpt->GetInsulationVStiffUF(),			DT_LEFT, FALSE);
	// �޺��ڽ� ��ġ ����
	m_Grid.SetCellType(COL_STEELSTAND,				1,CT_COMBO);
	m_Grid.SetCellType(COL_BRIDGE_KIND,				1,CT_COMBO);
//	m_Grid.SetCellType(COL_BRIDGE_HYUNGTAI,			1,CT_COMBO);
	m_Grid.SetCellType(COL_BRIDGE_GRADE,			1,CT_COMBO);
//	m_Grid.SetCellType(COL_BRIDGE_ANALYSIS,			1,CT_COMBO);
	m_Grid.SetCellType(COL_STD_FILLET_WELD,			1,CT_COMBO);
	m_Grid.SetCellType(COL_BOLT_BACHI_WEBPAN_CROSS,	1,CT_COMBO);
	m_Grid.SetCellType(COL_TYPE_CROSS_BRAKET,		1,CT_COMBO);	
	m_Grid.SetCellType(COL_WEIGHT_STA_ELE_CODI,		1,CT_COMBO);
	m_Grid.SetCellType(COL_STD_HBRACING_HG_DIR,		1,CT_COMBO);
	m_Grid.SetCellType(COL_CALC_FLOORCENTER_BAR,	1,CT_COMBO);
	m_Grid.SetCellType(COL_HEU_INTHICK,				1,CT_COMBO);
	m_Grid.SetCellType(COL_FASTLINESEARCH,			1,CT_COMBO);
	m_Grid.SetCellType(COL_INSTALLBEAMATCEN,		1,CT_COMBO);
	m_Grid.SetCellType(COL_ALLOW_CRACK_WIDTH,		1,CT_COMBO);
	m_Grid.SetCellType(COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN,1,CT_COMBO);
	m_Grid.SetCellType(COL_BOLT_BACHI_WEBPAN_BYUN_TYPE,	1,CT_COMBO);
	m_Grid.SetCellType(COL_SULCHI_CROSS_HSTIFF_BRAKET,	1,CT_COMBO);
	
	m_Grid.SetItemState(COL_STD_GIRHEIGHT_UPPER, 1, pDB->IsTUGir()?GVIS_READONLY:GVIS_MODIFIED);		
	
	//�������: ���μ������2010 ����ϰ� �Ѵ�.(Lock�� �������)
	//if(!CGeneralBaseStd::IsModuleDesignStd())
	//m_Grid.SetItemState(COL_STEELSTAND, 1, GVIS_READONLY);

	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL CDesignPage::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		switch(next.row)
		{			
		case COL_BRIDGE_KIND:
			strCombo.Add("���α���");
//			strCombo.Add("����ö����");
//			strCombo.Add("��ö����(LS-22)");
//			strCombo.Add("��ö����(LS-18)");
//			strCombo.Add("���ö������");
//			strCombo.Add("����ö����");
			m_Grid.SetComboString(strCombo);
			break;	
// 		case COL_BRIDGE_HYUNGTAI:
// 			strCombo.Add("������");
// 			strCombo.Add("�Ҽ��ְŴ���");
// 			if(pStd->IsModuleTUGirder())
// 				strCombo.Add("Turnover�Ŵ���");
// 			m_Grid.SetComboString(strCombo);
			break;
		case COL_STEELSTAND:
			strCombo.Add("���μ������2008");
			strCombo.Add("���μ������2010");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_BRIDGE_GRADE:
			strCombo.Add("1 ��� (DB-24 �� DL-24 ����)");
			strCombo.Add("2 ��� (DB-18 �� DL-18 ����)");
			strCombo.Add("3 ��� (DB-13.5 �� DL-13.5 ����)");
			m_Grid.SetComboString(strCombo);
			break;
// 		case COL_BRIDGE_ANALYSIS:
// 			strCombo.Add("�ռ���");
// 			strCombo.Add("���ռ���");
// 			m_Grid.SetComboString(strCombo);
// 			break;
		case COL_STD_FILLET_WELD:	
			strCombo.Add("���� ġ�� ����");
			strCombo.Add("�Ŵ��� �ִ� ġ������");		
			strCombo.Add("���� ���Ǻ� �ִ�ġ��");
			strCombo.Add("��ü 8mm ����");
			strCombo.Add("Ȧ���β� �ø�(���� ġ��)");
			strCombo.Add("Ȧ���β� �ø�(�Ŵ��� �ִ�)");
			strCombo.Add("Ȧ���β� �ø�(��������ũ��)");
			m_Grid.SetComboString(strCombo);	
			break;
		case COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN: 
			strCombo.Add("������� Ÿ��");
			strCombo.Add("�Ϲ�Ÿ��");			
			m_Grid.SetComboString(strCombo);
			break;			
		case COL_BOLT_BACHI_WEBPAN_BYUN_TYPE:
		case COL_BOLT_BACHI_WEBPAN_CROSS: 
			strCombo.Add(GetBoltArrage(0));
			strCombo.Add(GetBoltArrage(1));
			strCombo.Add(GetBoltArrage(2));
			m_Grid.SetComboString(strCombo);
			break;
		case COL_TYPE_CROSS_BRAKET: 
			strCombo.Add("�Ϲ�Ÿ��");
			strCombo.Add("����Ÿ��");			
			m_Grid.SetComboString(strCombo);
			break;
		case COL_SULCHI_CROSS_HSTIFF_BRAKET: 
			strCombo.Add("����");
			strCombo.Add("�������");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_WEIGHT_STA_ELE_CODI:			
			strCombo.Add("Kgf, mm");
			strCombo.Add("Tonf, M");
			m_Grid.SetComboString(strCombo);
			break;				
		case COL_STD_HBRACING_HG_DIR:
			strCombo.Add("�������");
			strCombo.Add("�������");
			m_Grid.SetComboString(strCombo);
			break;			
		case COL_CALC_FLOORCENTER_BAR://�ٴ��� �߾Ӻ� ö�� �����
			strCombo.Add("���� �����");
			strCombo.Add("������ �����");
			strCombo.Add("LB-Deck ����");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_HEU_INTHICK:
			strCombo.Add("����");
			strCombo.Add("�������");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_FASTLINESEARCH:
			strCombo.Add("����");
			strCombo.Add("�������");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_INSTALLBEAMATCEN:
			strCombo.Add("����");
			strCombo.Add("�������");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_ALLOW_CRACK_WIDTH:			//������ �νĿ� ���� ȯ������ (���տ���)
			strCombo.Add("������ ȯ��");
			strCombo.Add("�Ϲ� ȯ��");
			strCombo.Add("�νļ� ȯ��");
			strCombo.Add("�ؽ��� �νļ� ȯ��");
			m_Grid.SetComboString(strCombo);
			break;	
//		case COL_GIR_HYUNGAI:
//			strCombo.Add("��������");
//			strCombo.Add("�������");
//			m_Grid.SetComboString(strCombo);
//			break;	
// 		case COL_APP_THICK_GANG:
// 			strCombo.Add("�β��� ���� �ڵ� ��ȯ");
// 			strCombo.Add("�β��� 520 ����");
// 			strCombo.Add("�β��� ���� ��ȯ ����");
// 			m_Grid.SetComboString(strCombo);
// 			break;	
//		case COL_SULCHI_VSTIFF_CROSS: //case COL_SULCHI_HSTIFF:
//			strCombo.Add("��ġ��");
//			strCombo.Add("��ġ����");
//			m_Grid.SetComboString(strCombo);
//			break;			
//		case COL_BOLT_AUTO_CALC:
//			strCombo.Add("����");
//			strCombo.Add("�������");
//			m_Grid.SetComboString(strCombo);
//			break;
		}
	} 

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	return TStackedPage::PreTranslateMessage(pMsg);
}

BOOL CDesignPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();

	return TRUE;
}

void CDesignPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	SetDataSave();
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	
// 	if(nRow == COL_BRIDGE_HYUNGTAI && nCol == 1)
// 	{
// 		// 0 ������, 1 �Ҽ�����, 2 TU�Ŵ�
// 		long nBridgeType = pOpt->GetDesignBridgeType();
// 		m_pDesignPageDlg->ResetDesignPage(nBridgeType);
// 
// 		CString szTemp = "";
// 		long nGHType = 1;	//�Ҽ��ְŴ��� �⺻ Ÿ�� �ι�°��
// 		if (nBridgeType==PLATE_GIR) nGHType = 0;
// 		pOpt->SetDesignHeigtBase(nGHType);
// 
// 		switch(pOpt->GetDesignHeigtBase())
// 		{
// 			case 0: szTemp = GH_STR_UILI; break;
// 			case 1: szTemp = GH_STR_UOLO; break;
// 			case 2: szTemp = GH_STR_UILO; break;
// 			case 3: szTemp = GH_STR_UOLI; break;
// 		}
// 		m_Grid.SetTextMatrix(COL_STD_GIRHEIGHT_UPPER, 1, szTemp , DT_LEFT, FALSE);
// 
// 		if(nBridgeType==2)	//TU
// 		{
// 			// ������ũ��Ʈ
// 			pDB->m_BindConc.m_dWidth	= 2500;
// 			pDB->m_BindConc.m_dHeight	= 200;
// 			pDB->m_BindConc.m_dDeep		= 36;
// 			pDB->m_BindConc.m_dDis		= 100;
// 
// 			// ��������
// 			pDB->m_dLengthSlabGirderStt = 150;
// 			pDB->m_dLengthSlabGirderEnd = 150;
// 			pDB->m_dLengthTUBindConc_Stt= 100;
// 			pDB->m_dLengthTUBindConc_End= 100;
// 
// 			pOpt->SetDesignMaxLengthOfFieldSplice(20000);
// 		}
// 		else
// 		{
// 			// ������ũ��Ʈ
// 			pDB->m_BindConc.m_dWidth	= 0;
// 			pDB->m_BindConc.m_dHeight	= 0;
// 			pDB->m_BindConc.m_dDeep		= 0;
// 			pDB->m_BindConc.m_dDis		= 0;
// 
// 			// ��������
// 			pDB->m_dLengthSlabGirderStt = 50;
// 			pDB->m_dLengthSlabGirderEnd = 50;
// 			pDB->m_dLengthTUBindConc_Stt= 0;
// 			pDB->m_dLengthTUBindConc_End= 0;
// 		}
// 		m_GirHeightUserDlg.bIsTU = pDB->IsTUGir();
// 	}
	if(nRow == COL_INSULATIONVSTIFFUF && nCol == 1)
	{
		pDB->SetInsultionVStiff();		
	}

	if(nRow == COL_STEELSTAND && nCol ==1)
	{
		long nStandYear = pOpt->GetSteelStandardYear(m_Grid.GetTextMatrix(COL_STEELSTAND, 1));

		CSteelSection *pSteelSection = m_pDataManage->GetSteelSection();
		CCalcData *pData = m_pDataManage->GetCalcData();
		
		CDBAllowStressDataLoad	*pDBAllowStressDataLoad = new CDBAllowStressDataLoad(nStandYear, ROADTYPE);
		CGlobarOption			*pOpt = m_pDataManage->GetGlobalOption();
		CStringArray strArrDBSteelCode;
		CStringArray strArrDBSteelCodeAll;

		pDBAllowStressDataLoad->SetApplyYear(nStandYear);
		pDBAllowStressDataLoad->SetBridgeType(ROADTYPE);
		pDBAllowStressDataLoad->GetSteelCode(strArrDBSteelCode);
		pDBAllowStressDataLoad->GetSteelCodeAll(strArrDBSteelCodeAll);
		CString strDBSteelCode	= _T("");
		CString strSecSteelCode	= _T("");
		BOOL bExistCode = FALSE;

		//������ؿ� ���� ����� ������� �⺻�� ����  
		pData->DESIGN_MATERIAL.m_dShearEs	= nStandYear == APPLYYEAR2008 ? 81000.0 : 79000.0;			///< ���� ź�����
		pData->DESIGN_MATERIAL.m_dEst       = nStandYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//����ź�����
		pData->DESIGN_MATERIAL.m_dDummyEst  = nStandYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//����ź�����


		CMap <CString, LPCTSTR, long, long> mapDBSteelCodeAll;
		for(long nIdxAll = 0; nIdxAll < strArrDBSteelCodeAll.GetSize(); nIdxAll++)
			mapDBSteelCodeAll[strArrDBSteelCodeAll.GetAt(nIdxAll)] = 1;

		for(long nIdx = 0; nIdx < pSteelSection->GetSize(); nIdx++)
		{
			bExistCode = FALSE;
			strSecSteelCode = pSteelSection->GetMaterialByMarkBuje(nIdx);
			
			//���������� �ƴ϶��(��Ʈ, ���� ��) �˻����� �ʰ� ���� �׸����� �Ѿ.
			if(mapDBSteelCodeAll[strSecSteelCode] == 0)
				continue;
			for(long nIdx2 = 0; nIdx2 < strArrDBSteelCode.GetSize(); nIdx2++)
			{
				strDBSteelCode = strArrDBSteelCode.GetAt(nIdx2);
				if(strSecSteelCode == strDBSteelCode)
				{
					bExistCode = TRUE;
					break;
				}
			}
			if(!bExistCode)
				break;
		}

		CSteel *pSteelCtl = m_pDataManage->GetSteel();

		if(!bExistCode)
		{
			CString strMsg		 = _T("");
			CString strApplyYear = _T("");

			strMsg.Format("[%s]���� �������� �ʴ� ������ �����մϴ�!\n[���缳��]���� �������� �缳�����ּ���!", pOpt->GetStrSteelStandardYear());
			AfxMessageBox(strMsg, MB_ICONWARNING);
			
			if(pSteelCtl->GetApplyYear() == APPLYYEAR2008)
				strApplyYear = pOpt->GetStrSteelStandardYear(APPLYYEAR2008);
			else
				strApplyYear = pOpt->GetStrSteelStandardYear(APPLYYEAR2010);
			//���� ����������� ������
			m_Grid.SetGridData(m_gt, COL_STEELSTAND,	1, strApplyYear);
			SetDataSave();	
			delete pDBAllowStressDataLoad;
			return;
		}
		BeginWaitCursor();
		pSteelCtl->SetApplyYear(nStandYear);
		TStackedPage *pDesignSteel = m_pDesignPageDlg->GetPage(IDD_DESIGN_STEEL_MEMBER)->GetPage();
		pDesignSteel->SetDataDefault();
		EndWaitCursor();
		delete pDBAllowStressDataLoad;
		SetDataSave();
	}
	SetDataInit();
	return;
}

void CDesignPage::SetBridgeAnalysis(CString szStr)
{
// 	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
// 
// 	if(szStr == _T("�ռ���"))
// 		pDB->m_NumHaiseokCombo = 0;
// 	else
// 		pDB->m_NumHaiseokCombo = 1;
}

CString CDesignPage::GetStrBridgeAnalysis(long nType)
{
	if(nType == 0)
		return _T("�ռ���");
	else
		return _T("���ռ���");
}

void CDesignPage::SetBridgeGrade(CString szStr)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CCalcData       *pData	= m_pDataManage->GetCalcData();
	CSteelGrade     *pGrade = m_pDataManage->GetSteelGrade();

	CString szText = _T("");

	if(szStr.Find("1") == 0)
		szText.Format("1 ���");
	else if(szStr.Find("2") == 0)
		szText.Format("2 ���");
	else
		szText.Format("3 ���");

	pDB->m_sDeungeub = szText;
	
	// ��޿� ���� ��������
	long nCombo = atol(pDB->m_sDeungeub)-1;
	if(nCombo< 0 || nCombo>2) nCombo= 0;
	pData->DESIGN_CONDITION.m_dPf		= pGrade->GetDBPf(nCombo);
	pData->DESIGN_CONDITION.m_dPr		= pGrade->GetDBPr(nCombo);
	pData->DESIGN_CONDITION.m_dW		= pGrade->GetDL(nCombo);
	pData->DESIGN_CONDITION.m_dPm		= pGrade->GetDLPm(nCombo);
	pData->DESIGN_CONDITION.m_dPs		= pGrade->GetDLPs(nCombo);
	pData->DESIGN_CONDITION.m_szBridgeGrade = pDB->m_sDeungeub;
}

CString CDesignPage::GetStrBridgeGrade(CString szStr)
{
	CString szText = _T("");

	if(szStr.GetAt(0) == '1')
		szText = _T("1 ��� (DB-24 �� DL-24 ����)");
	else if(szStr.GetAt(0) == '2')
		szText = _T("2 ��� (DB-18 �� DL-18 ����)");
	else
		szText = _T("3 ��� (DB-13.5 �� DL-13.5 ����)");

	return szText;
}
