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
#define	GetStr(bApp)	(bApp ? "적용":"적용안함")
#define	GetApp(sApp)	(sApp=="적용" ? 1 : 0)

SHGRIDTITLE CDesignPage::m_gt[ROW_COUNT_ETC] = 
	{	
		{"종 류",												"%s",	300, DT_LEFT, TRUE},	// COL_TOTAL_TYPE
		{"1.  교량종류",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_KIND
		{"2.  강재기준",										"%s",	300, DT_LEFT, FALSE},	// COL_STEELSTAND
//		{"2.  교량형식",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_HYUNGTAI
		{"3.  교량등급",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_GRADE
//		{"4.  해석방법",										"%s",	300, DT_LEFT, FALSE},	// COL_BRIDGE_ANALYSIS
		{"4.  거더 높이 기준 설정",								"%s",	300, DT_LEFT, FALSE},	// COL_STD_GIRHEIGHT_UPPER
		{"5.  상판 최저두께 (mm)",								"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_MIN_UP
		{"6.  하판 최저두께 (mm)",								"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_MIN_Lo
		{"7.  복부판 최저두께 (mm)",							"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_MIN_WEB
		{"8.  인접 두께 편차 허용 검토",						"%s",	300, DT_LEFT, FALSE},	// COL_HEU_INTHICK
		{"9.  차량적재 최대중량 (kN)",							"%.lf",	300, DT_LEFT, FALSE},	// COL_WEIGHT_MAX_CAR
		{"10. 현장이음 최대길이 (mm)",							"%.lf",	300, DT_LEFT, FALSE},	// COL_LEN_MAX_SPACE
		{"11. 거더시작 번호",									"%s",	300, DT_LEFT, FALSE},	// COL_GIR_STT_NUM
		{"12. 현장이음 번호",									"%s",	300, DT_LEFT, FALSE},	// COL_SPACE_NUM
		{"13. 중량,STATION,ELEVATION,좌표 단위",				"%s",	300, DT_LEFT, FALSE},	// COL_WEIGHT_STA_ELE_CODI
		{"14. 편경사 및 종단기울기 표기 자리수",				"%.0f",	300, DT_LEFT, FALSE},	// COL_JARISU_PYEN_JONG	
		{"15. 필렛 용접 치수 기준",								"%s",	300, DT_LEFT, FALSE},	// COL_STD_FILLET_WELD
		{"16. 필렛 용접 최소 길이 (mm)",						"%.lf",	300, DT_LEFT, FALSE},	// COL_LEN_MIN_FILLET_WELD				
		{"17. 무개선 용접 적용 두께차 (mm)",					"%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_WELD_MUGAISUN		
		{"18. X 개선 용접 적용 두께 : 입력두께 이하 V용접 적용","%.lf",	300, DT_LEFT, FALSE},	// COL_THICK_WELD_XLINE
		{"19. 볼트배치 지그재그 타입 가로보 상.하판",			"%s",	300, DT_LEFT, FALSE},	// COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN
		{"20. 볼트배치 복부판 변단면 타입",						"%s",	300, DT_LEFT, FALSE},	// COL_BOLT_BACHI_WEBPAN_BYUN_TYPE
		{"21. 볼트배치 복부판 가로보",							"%s",	300, DT_LEFT, FALSE},	// COL_BOLT_BACHI_WEBPAN_CROSS
		{"22. 복부 이음판 상부 경사 기준 (3%)",					"%.lf",	300, DT_LEFT, FALSE},	// COL_STD_WEB_EUPAN_JONG		
		{"23. 복부 이음판 하부 경사 기준 (3%)",					"%.lf",	300, DT_LEFT, FALSE},	// COL_STD_WEB_EUPAN_BYUN
		{"24. 이음판 경사적용 단위(반올림 자리수)",				"%.lf",	300, DT_LEFT, FALSE},	// COL_JARISU_UNIT_KYUNGSA
		{"25. 가로보 브라켓 경사 타입",							"%s",	300, DT_LEFT, FALSE},	// COL_TYPE_CROSS_BRAKET			
		{"26. 가로보 수평보강재 브라켓위치 설치",				"%s",	300, DT_LEFT, FALSE},	// COL_SULCHI_CROSS_HSTIFF_BRAKET
		{"27. 수평브레이싱 형강 방향",							"%s",	300, DT_LEFT, FALSE},	// COL_STD_HBRACING_HG_DIR
		{"28. 바닥판 중앙부 철근 계산방법",						"%s",	300, DT_LEFT, FALSE},	// COL_CALC_FLOORCENTER_BAR
		{"29. 내진등급",										"%s",	300, DT_LEFT, FALSE},	// COL_EARTHQUACK_GRADE
		{"30. 지진구역 구분",									"%s",	300, DT_LEFT, FALSE},	// COL_EARTHQUACK_ZONE
		{"31. 빠른선형 검색",									"%s",	300, DT_LEFT, FALSE},	// COL_FASTLINESEARCH
		{"32. 브레이싱 골조선 도심축 설치",						"%s",	300, DT_LEFT, FALSE},	// COL_INSTALLBEAMATCEN
		{"33. 허용균열폭 : 강재의 부식에 대한 환경조건",		"%s",	300, DT_LEFT, FALSE},	// COL_ALLOW_CRACK_WIDTH
		{"34. TU거더교 수직보강재 상부플랜지 이격 옵션",		"%.lf", 300, DT_LEFT, FALSE}    //COL_INSULATIONVSTIFFUF
		
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
	if(nIdx==0) return "볼트 균등배치";
	if(nIdx==1) return "볼트 일반배치";
	if(nIdx==2) return "볼트 수평배치";
	return "";
}

long CDesignPage::GetBoltArrage(CString szStr)
{
	if(szStr=="볼트 균등배치") return 0;
	if(szStr=="볼트 일반배치") return 1;
	if(szStr=="볼트 수평배치") return 2;
	ASSERT(TRUE);
	return -1;
}

void CDesignPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;
	 CGlobarOption*   pOpt = m_pDataManage->GetGlobalOption();

	// 그리드 기본 셋팅 
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(ROW_COUNT_ETC);
	m_Grid.SetColumnCount(2);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);			
	m_Grid.InputShGridTitle(m_gt, ROW_COUNT_ETC, FALSE, FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetColumnWidth(1,250);
	m_Grid.SetRowHeightAll(20);	
	
	if(pOpt->GetDesignBridgeType() != 2 )//TU거더교
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
	pDB->m_sDeungeub				= _T("1 등급");
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

	//교량형태	
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

	pOpt->SetDesignMinThickOfUDFlate(m_Grid.GetTextMatrixDouble(COL_THICK_MIN_UP, 1), TRUE);			//상판 최저두께
	pOpt->SetDesignMinThickOfUDFlate(m_Grid.GetTextMatrixDouble(COL_THICK_MIN_Lo, 1), FALSE);			//하판 최저두께
	pOpt->SetDesignMinThickOfWebFlate(m_Grid.GetTextMatrixDouble(COL_THICK_MIN_WEB, 1));			//복부판 최저두께
	pOpt->SetDesignAllowDeflection(m_Grid.GetTextMatrix(COL_HEU_INTHICK,1));
	pOpt->SetDesignMaxWeightOfCarLoad(frkN(m_Grid.GetTextMatrixDouble(COL_WEIGHT_MAX_CAR, 1)));		// 차량적재 최대중량
	pOpt->SetDesignMaxLengthOfFieldSplice(m_Grid.GetTextMatrixDouble(COL_LEN_MAX_SPACE, 1));		// 현장이음 최대길이
	pOpt->SetStrDesignDimensionOfFilletWeld(m_Grid.GetTextMatrix(COL_STD_FILLET_WELD, 1));			// 필렛 용접 치수 기준
	pOpt->SetDesignMinLenOfFilletWeld(m_Grid.GetTextMatrixDouble(COL_LEN_MIN_FILLET_WELD, 1));		// 필렛 용접 최소길이
	pOpt->SetDesignThickWeldNoCut(m_Grid.GetTextMatrixDouble(COL_THICK_WELD_MUGAISUN, 1));			// 10무개선 용접 적용 두께차
	pOpt->SetDesignThickXWeldApply(m_Grid.GetTextMatrixDouble(COL_THICK_WELD_XLINE, 1));			// X 개선 용접 적용 두께 
	//볼트배치 지그재그 타입 가로보 상.하판
	pOpt->SetDesignBoltHoleJigJaegOfHorPlate(m_Grid.GetTextMatrix(COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN, 1) == "지그재그 타입" ? 0 : 1);

	pOpt->SetDesignBoltHoleArrangeByunSectionType(GetBoltArrage(m_Grid.GetTextMatrix(COL_BOLT_BACHI_WEBPAN_BYUN_TYPE, 1)));	//볼트배치 복부판 변단면 타입
	pOpt->SetDesignBoltHoleArrangeCrossBeam(GetBoltArrage(m_Grid.GetTextMatrix(COL_BOLT_BACHI_WEBPAN_CROSS, 1)));			//볼트배치 복부판 가로보

	pOpt->SetDesignBoltJongVSlopApplyUp(m_Grid.GetTextMatrixDouble(COL_STD_WEB_EUPAN_JONG, 1));		//복부 이음판 종단 경사 기준
	pOpt->SetDesignBoltJongVSlopApplyDn(m_Grid.GetTextMatrixDouble(COL_STD_WEB_EUPAN_BYUN, 1));		//복부 이음판 변단면 경사 기준
	pOpt->SetStrDesignCrossBeamBracketType(m_Grid.GetTextMatrix(COL_TYPE_CROSS_BRAKET, 1));	
	pOpt->SetStrDesignCrossBeamHStiffAtBracket(m_Grid.GetTextMatrix(COL_SULCHI_CROSS_HSTIFF_BRAKET, 1));			
	pOpt->SetDesignStartNumberOfGirder(m_Grid.GetTextMatrixLong(COL_GIR_STT_NUM, 1) - 1);
	pOpt->SetDesignStartNumberOfSplice(m_Grid.GetTextMatrixLong(COL_SPACE_NUM, 1) - 1);
	pOpt->SetStrUnitTonfMeter(m_Grid.GetTextMatrix(COL_WEIGHT_STA_ELE_CODI,1));	
	pOpt->SetDesignPointSlope(m_Grid.GetTextMatrixLong(COL_JARISU_PYEN_JONG,1));	
	pOpt->SetDesignByundanVSlopRoundPos((long)m_Grid.GetTextMatrixDouble(COL_JARISU_UNIT_KYUNGSA, 1));	//이음판 경사적용 단위	
	//기타

	///< 외측거더 외측가로보 설치여부 - 안함
	pDB->m_nInstallVStiffOnCrossBeam = 0;

	int nUpperLowerVal = 0;
	CString szTemp = m_Grid.GetTextMatrix(COL_STD_GIRHEIGHT_UPPER,1);
	if(szTemp == GH_STR_UILI) nUpperLowerVal = 0;
	if(szTemp == GH_STR_UOLO) nUpperLowerVal = 1;
	if(szTemp == GH_STR_UILO) nUpperLowerVal = 2;
	if(szTemp == GH_STR_UOLI) nUpperLowerVal = 3;
	pOpt->SetDesignHeigtBase(nUpperLowerVal); 
	
	pOpt->SetStrDesignHBracingDirChange(m_Grid.GetTextMatrix(COL_STD_HBRACING_HG_DIR, 1));

	pOpt->SetStrSlabCentDesignMethod(m_Grid.GetTextMatrix(COL_CALC_FLOORCENTER_BAR,1));//바닥판 중앙부 철근 계산방법
	if(m_Grid.GetTextMatrix(COL_EARTHQUACK_GRADE, 1)=="내진I 등급")
	{
		m_EarthQUackGradeDlg.m_nType = 0;
		pOpt->SetDegreeEarthQuake(0);
	}
	else
	{
		m_EarthQUackGradeDlg.m_nType = 1;
		pOpt->SetDegreeEarthQuake(1);
	}
	if(m_Grid.GetTextMatrix(COL_EARTHQUACK_ZONE, 1)=="I 구역")
	{
		m_EarhtQuackDlg.m_nType = 0;
		pOpt->SetRegionEarthQuake(0);
	}
	else
	{
		m_EarhtQuackDlg.m_nType = 1;
		pOpt->SetRegionEarthQuake(1);
	}
	
	pDB->m_bQuickLineSearch	= m_Grid.GetTextMatrix(COL_FASTLINESEARCH, 1) == "적용" ? TRUE : FALSE;
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

	m_Grid.SetGridData(m_gt ,COL_TOTAL_TYPE,			1, "설정값");
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
	m_Grid.SetGridData(m_gt, COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN,	1, pOpt->GetDesignBoltHoleJigJaegOfHorPlate()==0 ? "지그재그 타입" : "일반타입");			
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
	m_Grid.SetTextMatrix(COL_FASTLINESEARCH,		1, pDB->m_bQuickLineSearch ? "적용" : "적용안함", DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_FASTLINESEARCH,		1, pDB->m_bQuickLineSearch ? "적용" : "적용안함", DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_INSTALLBEAMATCEN,		1, pOpt->GetStrInstallBeamAtCenter(),		DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_ALLOW_CRACK_WIDTH,		1, pOpt->GetStrDesignAllowCrackWidthEnv(),	DT_LEFT, FALSE);
	m_Grid.SetTextMatrix(COL_INSULATIONVSTIFFUF,    1,"%.lf",pOpt->GetInsulationVStiffUF(),			DT_LEFT, FALSE);
	// 콤보박스 위치 설정
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
	
	//강재기준: 도로설계기준2010 사용하게 한다.(Lock에 상관없이)
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
			strCombo.Add("도로교량");
//			strCombo.Add("지하철교량");
//			strCombo.Add("국철교량(LS-22)");
//			strCombo.Add("국철교량(LS-18)");
//			strCombo.Add("고속철도교량");
//			strCombo.Add("경전철교량");
			m_Grid.SetComboString(strCombo);
			break;	
// 		case COL_BRIDGE_HYUNGTAI:
// 			strCombo.Add("판형교");
// 			strCombo.Add("소수주거더교");
// 			if(pStd->IsModuleTUGirder())
// 				strCombo.Add("Turnover거더교");
// 			m_Grid.SetComboString(strCombo);
			break;
		case COL_STEELSTAND:
			strCombo.Add("도로설계기준2008");
			strCombo.Add("도로설계기준2010");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_BRIDGE_GRADE:
			strCombo.Add("1 등급 (DB-24 및 DL-24 적용)");
			strCombo.Add("2 등급 (DB-18 및 DL-18 적용)");
			strCombo.Add("3 등급 (DB-13.5 및 DL-13.5 적용)");
			m_Grid.SetComboString(strCombo);
			break;
// 		case COL_BRIDGE_ANALYSIS:
// 			strCombo.Add("합성형");
// 			strCombo.Add("비합성형");
// 			m_Grid.SetComboString(strCombo);
// 			break;
		case COL_STD_FILLET_WELD:	
			strCombo.Add("실제 치수 적용");
			strCombo.Add("거더별 최대 치수적용");		
			strCombo.Add("상하 측판별 최대치수");
			strCombo.Add("전체 8mm 적용");
			strCombo.Add("홀수두께 올림(실제 치수)");
			strCombo.Add("홀수두께 올림(거더별 최대)");
			strCombo.Add("홀수두께 올림(중조립마크별)");
			m_Grid.SetComboString(strCombo);	
			break;
		case COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN: 
			strCombo.Add("지그재그 타입");
			strCombo.Add("일반타입");			
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
			strCombo.Add("일반타입");
			strCombo.Add("레벨타입");			
			m_Grid.SetComboString(strCombo);
			break;
		case COL_SULCHI_CROSS_HSTIFF_BRAKET: 
			strCombo.Add("적용");
			strCombo.Add("적용안함");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_WEIGHT_STA_ELE_CODI:			
			strCombo.Add("Kgf, mm");
			strCombo.Add("Tonf, M");
			m_Grid.SetComboString(strCombo);
			break;				
		case COL_STD_HBRACING_HG_DIR:
			strCombo.Add("하향기준");
			strCombo.Add("상향기준");
			m_Grid.SetComboString(strCombo);
			break;			
		case COL_CALC_FLOORCENTER_BAR://바닥판 중앙부 철근 계산방법
			strCombo.Add("강도 설계법");
			strCombo.Add("경험적 설계법");
			strCombo.Add("LB-Deck 공법");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_HEU_INTHICK:
			strCombo.Add("적용");
			strCombo.Add("적용안함");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_FASTLINESEARCH:
			strCombo.Add("적용");
			strCombo.Add("적용안함");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_INSTALLBEAMATCEN:
			strCombo.Add("적용");
			strCombo.Add("적용안함");
			m_Grid.SetComboString(strCombo);
			break;
		case COL_ALLOW_CRACK_WIDTH:			//강재의 부식에 대한 환경조건 (허용균열폭)
			strCombo.Add("건조한 환경");
			strCombo.Add("일반 환경");
			strCombo.Add("부식성 환경");
			strCombo.Add("극심한 부식성 환경");
			m_Grid.SetComboString(strCombo);
			break;	
//		case COL_GIR_HYUNGAI:
//			strCombo.Add("현장이음");
//			strCombo.Add("현장용접");
//			m_Grid.SetComboString(strCombo);
//			break;	
// 		case COL_APP_THICK_GANG:
// 			strCombo.Add("두께별 강종 자동 변환");
// 			strCombo.Add("두께별 520 적용");
// 			strCombo.Add("두께별 강종 변환 없음");
// 			m_Grid.SetComboString(strCombo);
// 			break;	
//		case COL_SULCHI_VSTIFF_CROSS: //case COL_SULCHI_HSTIFF:
//			strCombo.Add("설치함");
//			strCombo.Add("설치안함");
//			m_Grid.SetComboString(strCombo);
//			break;			
//		case COL_BOLT_AUTO_CALC:
//			strCombo.Add("적용");
//			strCombo.Add("적용안함");
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
// 		// 0 판형교, 1 소수주형, 2 TU거더
// 		long nBridgeType = pOpt->GetDesignBridgeType();
// 		m_pDesignPageDlg->ResetDesignPage(nBridgeType);
// 
// 		CString szTemp = "";
// 		long nGHType = 1;	//소수주거더교 기본 타입 두번째꺼
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
// 			// 구속콘크리트
// 			pDB->m_BindConc.m_dWidth	= 2500;
// 			pDB->m_BindConc.m_dHeight	= 200;
// 			pDB->m_BindConc.m_dDeep		= 36;
// 			pDB->m_BindConc.m_dDis		= 100;
// 
// 			// 지간구성
// 			pDB->m_dLengthSlabGirderStt = 150;
// 			pDB->m_dLengthSlabGirderEnd = 150;
// 			pDB->m_dLengthTUBindConc_Stt= 100;
// 			pDB->m_dLengthTUBindConc_End= 100;
// 
// 			pOpt->SetDesignMaxLengthOfFieldSplice(20000);
// 		}
// 		else
// 		{
// 			// 구속콘크리트
// 			pDB->m_BindConc.m_dWidth	= 0;
// 			pDB->m_BindConc.m_dHeight	= 0;
// 			pDB->m_BindConc.m_dDeep		= 0;
// 			pDB->m_BindConc.m_dDis		= 0;
// 
// 			// 지간구성
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

		//강재기준에 따른 재료의 물리상수 기본값 변경  
		pData->DESIGN_MATERIAL.m_dShearEs	= nStandYear == APPLYYEAR2008 ? 81000.0 : 79000.0;			///< 전단 탄성계수
		pData->DESIGN_MATERIAL.m_dEst       = nStandYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//강재탄성계수
		pData->DESIGN_MATERIAL.m_dDummyEst  = nStandYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//강재탄성계수


		CMap <CString, LPCTSTR, long, long> mapDBSteelCodeAll;
		for(long nIdxAll = 0; nIdxAll < strArrDBSteelCodeAll.GetSize(); nIdxAll++)
			mapDBSteelCodeAll[strArrDBSteelCodeAll.GetAt(nIdxAll)] = 1;

		for(long nIdx = 0; nIdx < pSteelSection->GetSize(); nIdx++)
		{
			bExistCode = FALSE;
			strSecSteelCode = pSteelSection->GetMaterialByMarkBuje(nIdx);
			
			//강재제원이 아니라면(볼트, 봉강 등) 검사하지 않고 다음 항목으로 넘어감.
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

			strMsg.Format("[%s]에서 지원하지 않는 강종이 존재합니다!\n[부재설정]에서 재질값을 재설정해주세요!", pOpt->GetStrSteelStandardYear());
			AfxMessageBox(strMsg, MB_ICONWARNING);
			
			if(pSteelCtl->GetApplyYear() == APPLYYEAR2008)
				strApplyYear = pOpt->GetStrSteelStandardYear(APPLYYEAR2008);
			else
				strApplyYear = pOpt->GetStrSteelStandardYear(APPLYYEAR2010);
			//기존 설계기준으로 설정함
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
// 	if(szStr == _T("합성형"))
// 		pDB->m_NumHaiseokCombo = 0;
// 	else
// 		pDB->m_NumHaiseokCombo = 1;
}

CString CDesignPage::GetStrBridgeAnalysis(long nType)
{
	if(nType == 0)
		return _T("합성형");
	else
		return _T("비합성형");
}

void CDesignPage::SetBridgeGrade(CString szStr)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CCalcData       *pData	= m_pDataManage->GetCalcData();
	CSteelGrade     *pGrade = m_pDataManage->GetSteelGrade();

	CString szText = _T("");

	if(szStr.Find("1") == 0)
		szText.Format("1 등급");
	else if(szStr.Find("2") == 0)
		szText.Format("2 등급");
	else
		szText.Format("3 등급");

	pDB->m_sDeungeub = szText;
	
	// 등급에 따른 하중저장
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
		szText = _T("1 등급 (DB-24 및 DL-24 적용)");
	else if(szStr.GetAt(0) == '2')
		szText = _T("2 등급 (DB-18 및 DL-18 적용)");
	else
		szText = _T("3 등급 (DB-13.5 및 DL-13.5 적용)");

	return szText;
}
