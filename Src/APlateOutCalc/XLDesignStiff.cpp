// XLDesignStiff.cpp: implementation of the CXLDesignStiff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcStd.h"
#include "XLDesignStiff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

CXLDesignStiff::CXLDesignStiff(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
}

CXLDesignStiff::~CXLDesignStiff()
{

}

///< 복부판 보강재 검토 - 지점부, 일반부
///< 일반부 단경간 - 최대정모멘트(전체거더)
///<        다경간 - 최대정모멘트(전체거던)
///< 지점부 단경간 - 교대위치중 최대 반력 지점
///<        다경간 - 1. 교대위치중 최대 반력 지점
///<                 2. 교각위치중 최대 반력 지점
void CXLDesignStiff::WebStiff(CStringArray &strArrUseSheet)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CAPlateOutExcel		*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	
	if(pOpt->GetDesignVStiffInstall()!=0 || pBridgeApp->GetGirder(0)->GetQtyVStiffSectionTotal() == 0)//0이면 설치...1이면 미설치...
	{
		pOut->ClearCellRef();
		pAPlateOutXL->SetProgress("6.1 WEB 보강재", pAPlateOutXL->GetProgress()+5);
		pAPlateOutXL->NewSheet(pXL, "6.1 WEB 보강재", 5);
		pOut->SetXL(pXL, "6. 보강재의 설계",			m_nSttRow, "A", "A", 2, BLACK, 11, TRUE);
		pOut->SetXL(pXL, "설치된 수직 보강재가 없습니다.", m_nSttRow, "B", "B");
		
		return;
	}

	long	nGPos		= -1;
	long	nGNegAbut	= -1;
	long	nGNegPier	= -1;
	double	dStaPos     = -9999999999999.0;
	double  dStaNegAbut = -9999999999999.0;
	double  dStaNegPier = -9999999999999.0;
	CPlateBasicIndex *pBxPos = NULL;
	CPlateBasicIndex *pBxNegAbut = NULL;
	CPlateBasicIndex *pBxNegPier = NULL;
	// 일반부 검토시 위치
	BOOL	bUpperPos	= StressCalc.GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nGPos, dStaPos, 1) > 0.0;		///< 정모멘트가 가장 큰곳(일반부검토시)
	if (nGPos < 0 || dStaPos < 0)
	{
		nGPos   = 0;
		dStaPos = (pBridgeApp->GetGirder(nGPos)->GetStationStt() + pBridgeApp->GetGirder(nGPos)->GetStationEnd()) / 2;
	}
	pBxPos = pBridgeApp->GetGirder(nGPos)->GetBxByStationDir(dStaPos, 0, BX_PLATEHSTIFF);
	// 지점부 검토 위치
	BOOL	bUpperNegAbut = FALSE;
	BOOL	bUpperNegPier = FALSE;
	BOOL	bExistAbut	= FALSE;
	BOOL	bExistPier	= FALSE;

	if(pBridgeApp->m_nQtyJigan > 1)	// 2경간 이상
	{
		bUpperNegAbut = TRUE;
		bUpperNegPier = FALSE;
		// 교대부
		CJoint *pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, FALSE); 
		if(pJ)
		{
			bExistAbut	= TRUE;
			nGNegAbut  = ModelCalc.GetGirderNoByJointID(pJ->m_nIdx);
			dStaNegAbut= pJ->m_vPointReal.x;
			
		}
		// 교각부	
		pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, TRUE); 
		if(pJ)
		{
			bExistPier = TRUE;
			nGNegPier  = ModelCalc.GetGirderNoByJointID(pJ->m_nIdx);
			dStaNegPier= pJ->m_vPointReal.x;
		}
		if (nGNegAbut < 0 || dStaNegAbut < 0)
		{
			nGNegAbut   = 0;
			dStaNegAbut = (pBridgeApp->GetGirder(nGNegAbut)->GetStationStt() + pBridgeApp->GetGirder(nGNegAbut)->GetStationEnd()) / 2;
		}
		if (nGNegPier < 0 || dStaNegPier < 0)
		{
			nGNegPier   = 0;
			dStaNegPier = (pBridgeApp->GetGirder(nGNegPier)->GetStationStt() + pBridgeApp->GetGirder(nGNegPier)->GetStationEnd()) / 2;
		}
		pBxNegAbut = pBridgeApp->GetGirder(nGNegAbut)->GetBxByStationDir(dStaNegAbut, 0, BX_PLATEHSTIFF);
		pBxNegPier = pBridgeApp->GetGirder(nGNegPier)->GetBxByStationDir(dStaNegPier, 0, BX_PLATEHSTIFF);
	}
	else	// 단경간
	{
		bUpperNegAbut = TRUE;
		// 교대부
		CJoint *pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, FALSE); 
		if(pJ)
		{
			nGNegAbut  = ModelCalc.GetGirderNoByJointID(pJ->m_nIdx);
			dStaNegAbut= pJ->m_vPointReal.x;
			bExistAbut = TRUE;
		}
		if (nGNegAbut < 0 || dStaNegAbut < 0)
		{
			nGNegAbut   = 0;
			dStaNegAbut = (pBridgeApp->GetGirder(nGNegAbut)->GetStationStt() + pBridgeApp->GetGirder(nGNegAbut)->GetStationEnd()) / 2;
		}
		pBxNegAbut = pBridgeApp->GetGirder(nGNegAbut)->GetBxByStationDir(dStaNegAbut, 0, BX_PLATEHSTIFF);
	}

	m_nSttRow = 1;
	///< 일반부
	pOut->ClearCellRef();
	pAPlateOutXL->SetProgress("6.1 WEB 보강재 (일반부)", pAPlateOutXL->GetProgress()+5);
	pAPlateOutXL->NewSheet(pXL, "6.1 WEB 보강재 (일반부)", 5);
	pOut->SetXL(pXL, "6. 보강재의 설계",			m_nSttRow, "A", "A", 2, BLACK, 11, TRUE);
	pOut->SetXL(pXL, "6.1 WEB 보강재 (일반부)",		m_nSttRow, "B", "B", 1, BLACK, 9);
	WebStiff(pBxPos, bUpperPos);
	strArrUseSheet.Add("6.1 WEB 보강재 (일반부)");
	
	///< 지점부
	m_nSttRow =1;
	if(bExistAbut)
	{
		pOut->ClearCellRef();
		pAPlateOutXL->SetProgress("6.1 WEB 보강재 (지점부-교대)", pAPlateOutXL->GetProgress()+5);
		pAPlateOutXL->NewSheet(pXL, "6.1 WEB 보강재 (지점부-교대)", 5);
		pOut->SetXL(pXL, "6.1 WEB 보강재 (지점부-교대)",		m_nSttRow, "B", "B", 1, BLACK, 9);
		WebStiff(pBxNegAbut, bUpperNegAbut);
		strArrUseSheet.Add("6.1 WEB 보강재 (지점부-교대)");
	}
	
//	if(pBridgeApp->m_nQtyJigan > 1)	//다경간
	if(pBridgeApp->m_nQtyJigan > 1 && bExistPier)	//다경간
	{
		pOut->ClearCellRef();
		pAPlateOutXL->SetProgress("6.1 WEB 보강재 (지점부-교각)", pAPlateOutXL->GetProgress()+5);
		pAPlateOutXL->NewSheet(pXL, "6.1 WEB 보강재 (지점부-교각)", 5);
		m_nSttRow =1;
		pOut->SetXL(pXL, "6.1 WEB 보강재 (지점부-교각)",		m_nSttRow, "B", "B", 1, BLACK, 9);
		WebStiff(pBxNegPier, bUpperNegPier);		
		strArrUseSheet.Add("6.1 WEB 보강재 (지점부-교각)");
	}
}

void CXLDesignStiff::WebStiff(CPlateBasicIndex *pBx, BOOL bUpper)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CPlateGirderApp		*pGir			= pBx->GetGirder();
	CVStiff				*pVStiff		= pGir->GetBxByStation(pBx->GetStation(), BX_VSTIFF)->GetSection()->GetVStiff();
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CSteel				*pSteelCtl		= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();
	
	CString	sText, sLeft, sNum, sRight;

	CWebHStiff *pHStiff = pBx->GetWebHStiff();
	long	nNr			= max(pHStiff->GetTotalDansu(bUpper, TRUE),pHStiff->GetTotalDansu(bUpper, FALSE));
	long	nG			= pBx->GetNumberGirder();
	double	dSta		= pBx->GetStation();
	double	dMax_up		= 0;
	double	dMax_lo		= 0;
	double	dMax_upA	= 0;
	double	dMax_loA	= 0;
	double	dTau_up		= 0;
	double	dTau_lo		= 0;
	double	dTau_upA	= 0;
	double	dTau_loA	= 0;
	double	dVStiffW	= max(pVStiff->m_dWidth[0], pVStiff->m_dWidth[1]);
	double	dVStiffT	= max(pVStiff->m_dThick[0], pVStiff->m_dThick[1]);
	
	double	dGapJijum	= 0.0;
	
	if(pBx->IsJijum())
	{
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBxStiff	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF, pBx, NULL, FALSE);
		if(!pBxStiff)		pBxStiff	= Finder.GetBxLast(BX_CROSSBEAM_VBRACING | BX_VSTIFF, pBx, NULL, FALSE);
		dGapJijum = fabs(pBx->GetStation()-pBxStiff->GetStation());
	}
	
	double	dGap		= pBx->IsJijum()?  dGapJijum : pBridgeApp->m_dLengthBaseVStiff;
	double	dVStiffH	= pGir->GetHeightGirderByStaAng(dSta);
	double	dTw			= pGir->GetThickJewon(G_W, pBx);
	double	dALLOWA_B	= 3.0;

	if(pBx->IsJijum()) dALLOWA_B = 1.5;
	
	sLeft  = pBridgeApp->GetMaterialNumber(CP_V, -1);
	sNum   = pBridgeApp->GetMaterialNumber(CP_V,  0);
	sRight = pBridgeApp->GetMaterialNumber(CP_V,  1);
	
	CString sWebLeft, sWebRight;
	CCalcGeneral	*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	pCalcGeneral->GetMaterial(pBx, G_W, sWebLeft, sWebRight);
	CString sWebMaterial = sWebLeft;// + sWebRight;//복부판 사용강종...

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sWebLeft + sWebRight)];
	CHGAllowStress		AllowSpace(pSteel);	// 수직보강재를 생략할 수 있는 플랜지 순간격의 최대치

	double	dAllowSpace	= AllowSpace.GetAllowStress(ALLOW_VSTIFFNOT, 0);
	StressCalc.GetFlangeStress(nG, dSta, dMax_up, dMax_lo, dMax_upA, dMax_loA, dTau_up, dTau_lo, dTau_upA, dTau_loA);	

	if(sNum == "490" && sRight == "Y") sNum = "490Y";

	pOut->SetXL(pXL, "가) 수직보강재의 설계",		m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "(",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sLeft,							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sNum,							m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER, "STEELTYPE", 0);
	pOut->SetXL(pXL, sRight,						m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "사용 )",						m_nSttRow, "N", "N", 1);

	pOut->SetXL(pXL, "수평보강재 사용 단수",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, nNr,							m_nSttRow, "M", "M", 1, RED,   9, FALSE, TA_CENTER, "N", 0);

	pOut->SetXL(pXL, "복부판의 연섬유 압축응력",	m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	if(bUpper)	sText.Format("=%.3f", dMax_up);
	else		sText.Format("=%.3f", dMax_lo);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, RED,   9, FALSE, TA_CENTER, "F");
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "P", "P", 0);

	pOut->SetXL(pXL, "복부판 사용강종 :",			m_nSttRow, "R", "R", 0);
	pOut->SetXL(pXL, sWebMaterial,					m_nSttRow, "W", "Z", 1, RED,   9, FALSE, TA_CENTER, "WSTEELTYPE" , 0);

	pOut->SetXL(pXL, "복부판의 전단응력",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "υ",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	if(bUpper)	sText.Format("=%.3f", dTau_up);
	else		sText.Format("=%.3f", dTau_lo);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, RED,   9, FALSE, TA_CENTER, "NU");
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "P", "P", 1);

	pOut->SetXL(pXL, "사용 수직보강재의 폭",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "bvs",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f", dVStiffW);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "BVS", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "O", "O", 1);

	pOut->SetXL(pXL, "사용 수직보강재의 두께",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "tvs",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f", dVStiffT);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "TVS", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "O", "O", 1);

	pOut->SetXL(pXL, "수직보강재의 간격",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "a",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f", dGap);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "A", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "O", "O", 1);

	pOut->SetXL(pXL, "상하 양플랜지의 순간격",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "b",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f", dVStiffH);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "B", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "O", "O", 1);

	pOut->SetXL(pXL, "복부판의 두께",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "t",							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f", dTw);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "TW", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "O", "O", 2);

	SetLines(2);
	
	pOut->SetXL(pXL, dAllowSpace,					m_nSttRow, "D", "E", 0, RED, 9, FALSE, TA_CENTER, "COEFF1", 0);
	pOut->SetXL(pXL, "×",							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "t",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s,3)", pOut->GetCellRef("COEFF1"), pOut->GetCellRef("TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ALLOW_TERM", 2);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("ALLOW_TERM"), pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "b",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	
	SetLines(-2);
	pOut->SetXL(pXL, "·수직보강재의 사용여부 검토",m_nSttRow, "C", "C", 1);
	sText.Format("=\"수직보강재를 생략할 수 있는 플랜지 순간격 : \"&%s&\"t\"", pOut->GetCellRef("COEFF1"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "D", "D", 2);
	
	sText.Format("=IF(%s<%s, \"∴ 수직보강재필요!\", \"∴ 수직보강재필요없음!\")", pOut->GetCellRef("ALLOW_TERM"), pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "D", "D", 1);

	pOut->SetXL(pXL, "·수직보강재의 간격검토",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "( ☞ 도.설.기 3.8.5.1 )",		m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "a/b",							m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("A"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "A_COEFF", 1);
	pOut->SetXL(pXL, "/",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "B_COEFF", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("A_COEFF"), pOut->GetCellRef("B_COEFF"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "A_B");
	pOut->SetXL(pXL, dALLOWA_B,						m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "ALLOW_A_B", 1);
	sText.Format("=IF(%s<=%s,\"≤\",\"＞\")", pOut->GetCellRef("A_B"), pOut->GetCellRef("ALLOW_A_B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "O", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<=%s,\"... O.K\",\"... N.G\")", pOut->GetCellRef("A_B"), pOut->GetCellRef("ALLOW_A_B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Y", "Y", 1, RED);
	pOut->SetXL(pXL, "·수평보강재",				m_nSttRow, "C", "C", 0);
	sText.Format("=%s", pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER, "N_COEFF", 0);
	pOut->SetXL(pXL, "단을 사용하고",				m_nSttRow, "G", "G", 0);
	pOut->SetXL(pXL, "a/b",							m_nSttRow, "K", "K", 0);
	sText.Format("=IF(AND(%s=0,%s>1),\"＞ 1\",IF(AND(%s=0,%s<=1),\"≤ 1\",IF(AND(%s=1,%s>0.8),\"＞ 0.80\",IF(AND(%s=1,%s<=0.8),\"≤ 0.80\",IF(AND(%s=2,%s>0.64),\"＞ 0.64\",IF(AND(%s=2,%s<=0.64),\"≤ 0.64\"))))))",
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, "이므로",						m_nSttRow, "O", "O", 1);
	sText.Format("=IF(AND(%s=0,%s>1),\"(b/100t)⁴[(f/365)²+ { υ / 81+61(b/a)²}²]\",IF(AND(%s=0,%s<=1),\"(b/100t)⁴[(f/365)²+ { υ / 61+81(b/a)²}²]\",IF(AND(%s=1,%s>0.8),\"(b/100t)⁴[(f/950)²+ { υ / 127+61(b/a)²}²]\",IF(AND(%s=1,%s<=0.8),\"(b/100t)⁴[(f/950)²+ { υ / 95+81(b/a)²}²]\",IF(AND(%s=2,%s>0.64),\"(b/100t)⁴[(f/3150)²+ { υ / 197+61(b/a)²}²]\",IF(AND(%s=2,%s<=0.64),\"(b/100t)⁴[(f/3150)²+ { υ / 148+81(b/a)²}²]\"))))))",
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(IF(AND(%s=0,%s>1),((%s/(100*%s))^4)*(((%s/365)^2)+((%s/(81+61*(%s/%s)^2))^2)),IF(AND(%s=0,%s<=1),((%s/(100*%s))^4)*(((%s/365)^2)+((%s/(61+81*(%s/%s)^2))^2)),IF(AND(%s=1,%s>0.8),((%s/(100*%s))^4)*(((%s/950)^2)+((%s/(127+61*(%s/%s)^2))^2)),IF(AND(%s=1,%s<=0.8),((%s/(100*%s))^4)*(((%s/950)^2)+((%s/(95+81*(%s/%s)^2))^2)),IF(AND(%s=2,%s>0.64),((%s/(100*%s))^4)*(((%s/3150)^2)+((%s/(197+61*(%s/%s)^2))^2)),IF(AND(%s=2,%s<=0.64),((%s/(100*%s))^4)*(((%s/3150)^2)+((%s/(148+81*(%s/%s)^2))^2)))))))),3)",
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("B"),pOut->GetCellRef("TW"),
				pOut->GetCellRef("F"),pOut->GetCellRef("NU"),pOut->GetCellRef("B"), pOut->GetCellRef("A"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("B"),pOut->GetCellRef("TW"),
				pOut->GetCellRef("F"),pOut->GetCellRef("NU"),pOut->GetCellRef("B"),pOut->GetCellRef("A"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("B"),pOut->GetCellRef("TW"),
				pOut->GetCellRef("F"),pOut->GetCellRef("NU"),pOut->GetCellRef("B"),pOut->GetCellRef("A"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("B"),pOut->GetCellRef("TW"),
				pOut->GetCellRef("F"),pOut->GetCellRef("NU"),pOut->GetCellRef("B"),pOut->GetCellRef("A"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("B"),pOut->GetCellRef("TW"),
				pOut->GetCellRef("F"),pOut->GetCellRef("NU"),pOut->GetCellRef("B"),pOut->GetCellRef("A"),
				pOut->GetCellRef("N_COEFF"),pOut->GetCellRef("A_B"),pOut->GetCellRef("B"),pOut->GetCellRef("TW"),
				pOut->GetCellRef("F"),pOut->GetCellRef("NU"),pOut->GetCellRef("B"),pOut->GetCellRef("A"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "HSTIFF_COEFF1");
	pOut->SetXL(pXL, 1.0,							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER, "HSTIFF_COEFF2", 0);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("HSTIFF_COEFF1"), pOut->GetCellRef("HSTIFF_COEFF2"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("HSTIFF_COEFF1"), pOut->GetCellRef("HSTIFF_COEFF2"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Y", "Y", 1, RED);

	pOut->SetXL(pXL, "·수직보강재의 강도 검토",	m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "( ☞ 도.설.기 3.8.5.2 )",		m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "γ",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "8.0 × ( b / a )²",			m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 8.0,							m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "GAMMA_COEFF1");
	pOut->SetXL(pXL, "×",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "GAMMA_COEFF2", 0);
	pOut->SetXL(pXL, "/",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("A"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "GAMMA_COEFF3", 1);
	pOut->SetXL(pXL, ")²",							m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*(%s/%s)^2", pOut->GetCellRef("GAMMA_COEFF1"), pOut->GetCellRef("GAMMA_COEFF2"), pOut->GetCellRef("GAMMA_COEFF3"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER, "GAMMA");

	pOut->SetLineBoxText(pXL, "I(req)",				m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "bt³",				m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "× γ",				m_nSttRow, "I", "J", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("B"));
	pOut->SetUnderLineText(pXL, sText,				m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "I_REQ_COEFF1", 0);
	pOut->SetUnderLineText(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("TW"));
	pOut->SetUnderLineText(pXL, sText,				m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "I_REQ_COEFF2", 1);
	pOut->SetUnderLineText(pXL, "³",				m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "×",					m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("GAMMA"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "S", "U", 2, BLACK, 9, FALSE, TA_CENTER, "I_REQ_COEFF3", 3, FALSE);
	SetLines(NEXT);
	pOut->SetXL(pXL, 10.92,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "I_REQ_COEFF4", 2);
	sText.Format("=%s", pOut->GetCellRef("I_REQ_COEFF4"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "I_REQ_COEFF5", 2);
	SetLines(-1);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "V", "V", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=ROUND((%s*%s^3)/%s*%s,3)", pOut->GetCellRef("I_REQ_COEFF1"), pOut->GetCellRef("I_REQ_COEFF2"), pOut->GetCellRef("I_REQ_COEFF5"), pOut->GetCellRef("I_REQ_COEFF3"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "W", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "I_REQ", 1, FALSE);
	pOut->SetLineBoxText(pXL, "mm⁴",				m_nSttRow, "Z", "Z", 2, BLACK, 9, FALSE, TA_LEFT, "", 0, FALSE);
	SetLines(2);

	pOut->SetXL(pXL, "I(used)",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "⅓",							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("TVS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "I_USED_COEFF1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BVS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "I_USED_COEFF2", 1);
	pOut->SetXL(pXL, "³",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(1/3*%s*%s^3,3)", pOut->GetCellRef("I_USED_COEFF1"), pOut->GetCellRef("I_USED_COEFF2"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "I_USED", 1);
	pOut->SetXL(pXL, "mm⁴",						m_nSttRow, "R", "R", 0);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("I_USED"), pOut->GetCellRef("I_REQ"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "I(req)",						m_nSttRow, "U", "U", 0);
	sText.Format("=IF(%s>%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("I_USED"), pOut->GetCellRef("I_REQ"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Y", "Y", 1, RED);

	pOut->SetXL(pXL, "·수직보강재 돌출각의 최소폭",m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "bmin",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "B_MIN_COEFF1", 0);
	pOut->SetXL(pXL, "/",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 30.0,							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "B_MIN_COEFF2", 0);
	pOut->SetXL(pXL, "+",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 50.0,							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER, "B_MIN_COEFF3", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s/%s+%s,3)", pOut->GetCellRef("B_MIN_COEFF1"), pOut->GetCellRef("B_MIN_COEFF2"), pOut->GetCellRef("B_MIN_COEFF3"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "B_MIN");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, "bvs",							m_nSttRow, "S", "S", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BVS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER, "BVS_1", 1);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("B_MIN"), pOut->GetCellRef("BVS_1"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "R", "R", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "X", "X", 0);
	sText.Format("=IF(%s<%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("B_MIN"), pOut->GetCellRef("BVS_1"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Y", "Y", 1, RED);

	pOut->SetXL(pXL, "·돌출각의 최소폭",			m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "tmin",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BVS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "T_MIN_COEFF1", 0);
	pOut->SetXL(pXL, "/",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 13.0,							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "T_MIN_COEFF2", 0);

	pOut->SetXL(pXL, "=",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s/%s,3)", pOut->GetCellRef("T_MIN_COEFF1"), pOut->GetCellRef("T_MIN_COEFF2"), pOut->GetCellRef("T_MIN_COEFF3"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "T_MIN");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, "tvs",							m_nSttRow, "S", "S", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("TVS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER, "TVS_2", 1);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("T_MIN"), pOut->GetCellRef("TVS_2"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "R", "R", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "X", "X", 0);
	sText.Format("=IF(%s<%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("T_MIN"), pOut->GetCellRef("TVS_2"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Y", "Y", 2, RED);

///< 나. 수평보강재의 설계
	if(nNr==0) return;
	sLeft  = pBridgeApp->GetMaterialNumber(G_H_L, -1);
	sNum   = pBridgeApp->GetMaterialNumber(G_H_L,  0);
	sRight = pBridgeApp->GetMaterialNumber(G_H_L,  1);
	double	dHStiff_B = max(pBx->GetWebHStiff()->m_dWidthHStiff[0], pBx->GetWebHStiff()->m_dWidthHStiff[1]);
	double	dHStiff_T = max(pBx->GetWebHStiff()->m_dThickHStiff[0], pBx->GetWebHStiff()->m_dThickHStiff[1]);

	pOut->SetXL(pXL, "나) 수평보강재의 설계",		m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "(",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sLeft,							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sNum,							m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, sRight,						m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "사용 )",						m_nSttRow, "N", "N", 1);

	pOut->SetXL(pXL, "수직보강재의 간격",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "a",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("A"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "H_A", 1);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "N", "N", 1);

	pOut->SetXL(pXL, "상하 양플랜지의 순간격",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "b",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "H_B", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "N", "N", 1);

	pOut->SetXL(pXL, "사용 수평보강재의 폭",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "bhs",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f", dHStiff_B);
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "BHS", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "N", "N", 1);

	pOut->SetXL(pXL, "사용 수평보강재의 두께",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ths",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f", dHStiff_T);
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "THS", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "N", "N", 0);
	
	pOut->SetXL(pXL, "tmin",						m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetXL(pXL, "bhs",							m_nSttRow, "S", "S", 0, BLACK, 9);
	pOut->SetXL(pXL, "/",							m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "16",							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER, "" , 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("=%s/16",pOut->GetCellRef("BHS"));

	pOut->SetXL(pXL, sText,							m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_CENTER, "TMIN", 1);

	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("THS"), pOut->GetCellRef("TMIN"));

	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "O", 0, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "mm",							m_nSttRow, "Y",	"Y", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=IF(%s>%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("THS"), pOut->GetCellRef("TMIN"));

	pOut->SetXL(pXL, sText,							m_nSttRow, "Z", "AA", 1, RED, 9, FALSE, TA_CENTER);
	
	pOut->SetXL(pXL, "·수평보강재의 강도 검토",	m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "( ☞ 도.설.기 3.8.6.1 )",		m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "γ",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "30.0 × ( a / b )",			m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 30.0,							m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "H_GAMMA_COEFF1");
	pOut->SetXL(pXL, "×",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("H_A"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "H_GAMMA_COEFF2", 0);
	pOut->SetXL(pXL, "/",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("H_B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "H_GAMMA_COEFF3", 1);
	pOut->SetXL(pXL, ")²",							m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*(%s/%s)^2,3)", pOut->GetCellRef("H_GAMMA_COEFF1"), pOut->GetCellRef("H_GAMMA_COEFF2"), pOut->GetCellRef("H_GAMMA_COEFF3"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER, "H_GAMMA");

	pOut->SetLineBoxText(pXL, "I(req)",				m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "bt³",				m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "× γ",				m_nSttRow, "I", "J", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("H_B"));
	pOut->SetUnderLineText(pXL, sText,				m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "H_I_REQ_COEFF1", 0);
	pOut->SetUnderLineText(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("TW"));
	pOut->SetUnderLineText(pXL, sText,				m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "H_I_REQ_COEFF2", 1);
	pOut->SetUnderLineText(pXL, "³",				m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "×",					m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("H_GAMMA"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "S", "U", 2, BLACK, 9, FALSE, TA_CENTER, "H_I_REQ_COEFF3", 3, FALSE);
	SetLines(NEXT);
	pOut->SetXL(pXL, 10.92,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "H_I_REQ_COEFF4", 2);
	sText.Format("=%s", pOut->GetCellRef("H_I_REQ_COEFF4"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "H_I_REQ_COEFF5", 2);
	SetLines(-1);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "V", "V", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=ROUND((%s*%s^3)/%s*%s,3)", pOut->GetCellRef("H_I_REQ_COEFF1"), pOut->GetCellRef("H_I_REQ_COEFF2"), pOut->GetCellRef("H_I_REQ_COEFF5"), pOut->GetCellRef("H_I_REQ_COEFF3"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "W", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "H_I_REQ", 1, FALSE);
	pOut->SetLineBoxText(pXL, "mm⁴",				m_nSttRow, "Z", "Z", 2, BLACK, 9, FALSE, TA_LEFT, "", 0, FALSE);
	SetLines(2);

	pOut->SetXL(pXL, "I(used)",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "⅓",							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("THS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "H_I_USED_COEFF1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BHS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "H_I_USED_COEFF2", 1);
	pOut->SetXL(pXL, "³",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(1/3*%s*%s^3,3)", pOut->GetCellRef("H_I_USED_COEFF1"), pOut->GetCellRef("H_I_USED_COEFF2"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "H_I_USED", 1);
	pOut->SetXL(pXL, "mm⁴",						m_nSttRow, "R", "R", 0);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("H_I_USED"), pOut->GetCellRef("H_I_REQ"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "I(req)",						m_nSttRow, "U", "U", 0);
	sText.Format("=IF(%s>%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("H_I_USED"), pOut->GetCellRef("H_I_REQ"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Y", "Y", 1, RED);

	pOut->SetXL(pXL, "·수평보강재의 설치위치",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "( ☞ 도.설.기 3.8.6.1 )",		m_nSttRow, "U", "U");

	pOut->SetXL(pXL, "압축측 플랜지로 부터",		m_nSttRow, "D", "D", 0);
	sText.Format("=IF(%s=1, \"0.20b = 0.20 × \"&%s&\" = \"&0.2*%s&\" mm\",\"0.14b 및 0.36b  즉, \"&0.14*%s&\" mm 및 \"&0.36*%s&\" mm\")",
					pOut->GetCellRef("N"),pOut->GetCellRef("H_B"),pOut->GetCellRef("H_B"),pOut->GetCellRef("H_B"),pOut->GetCellRef("H_B"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "J", "J", 1);


}
///< 지점부 보강재 검토 - 교대부, 교각부
void CXLDesignStiff::JijumSupport(CStringArray &strArrUseSheet, BOOL bIsPier)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CXLWeldingCheck		*pWeldingCheck	= m_pAPlateOutCalcStd->GetWeldingCheck();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFEMManage			*pManage = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CSteel				*pSteelCtl	=  pDataManage->GetSteel();

	m_nSttRow = 1;
	pOut->ClearCellRef();
	CString	sText, sLeft, sRight;
	double	dMaxReaction			= pWeldingCheck->GetMaxReaction(bIsPier);
	long	nMaxJoint				= pWeldingCheck->GetMaxJoint(bIsPier);
	BOOL	bExistAbut				= pWeldingCheck->ExistAbut();
	BOOL	bExistPier				= pWeldingCheck->ExistPier();
	if(bIsPier)
	{
		if(bExistPier)	strArrUseSheet.Add("6.2 지점부 보강재(교각부)");
		else			return;
	}
	else
	{
		if(bExistAbut)	strArrUseSheet.Add("6.2 지점부 보강재(교대부)");
		else			return;
	}
	CPlateBasicIndex	*pBx		= pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nMaxJoint));
	CPlateGirderApp		*pGir		= pBx->GetGirder();	
	CVStiff				*pVStiff	= pGir->GetBxByStation(pBx->GetStation(), BX_JIJUMSTIFF)->GetJijumStiff();
	long	nJijum		= pGir->GetNumberJijumByBx(pBx);
	double	dSta	= pBx->GetStation();
	double	dWidth	= max(pVStiff->m_dWidth[0], pVStiff->m_dWidth[1]);
	double	dThick	= max(pVStiff->m_dThick[0], pVStiff->m_dThick[1]);
	double	dHGir	= pGir->GetHeightGirderByStaAng(dSta);
	double	dHeight	= max(pVStiff->GetVStiffHeight(TRUE, dHGir), pVStiff->GetVStiffHeight(FALSE, dHGir));
	double	dTw		= pGir->GetThickJewon(G_W, pBx);
	long	nQty	= pBridgeApp->GetQtyJijumStiff(nJijum);//pGir->GetQtyRowJijumVStiffByBx(pBx);
	long	nG		= pBx->GetNumberGirder();
	BOOL	bHapSung = FALSE;
	double	dGap	= nQty < 2 ? 0 : ABS(pBridgeApp->m_dJiJumTerm[nJijum][1] - pBridgeApp->m_dJiJumTerm[nJijum][0]);
	if(pBridgeApp->GetNumHaiseokCombo()==0) bHapSung = TRUE;
	StressCalc.GetSectionStress(nG, dSta, bHapSung);

	CDesignJijumStiff   pJStiff(pCalcStd, pBx, nMaxJoint);

	CCalcGeneral	*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	pCalcGeneral->GetMaterial(pBx, V_J_ST, sLeft, sRight);
	// 허용응력 //////////////////////////////////////////////////////////////	
	double	dLengthBuckling	= pJStiff.m_strJijumStiff.m_dBuckLing_le;
	double	dTurningRadius	= pJStiff.m_strJijumStiff.m_dDan_r;
	
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeft + sRight)];
	CHGAllowStress	AllowBucklingUnConsider(pSteel);
	CHGAllowStress	AllowBucklingCoeff(pSteel);
	CHGAllowStress	AllowBuckling(pSteel);
	CHGAllowStress	AllowTension(pSteel);

	AllowBucklingUnConsider.SetAllowBucklingUnconsider(dLengthBuckling, dTurningRadius);
	AllowBuckling.SetAllowBuckling(dThick, dWidth);
	double	dAllowConsider	= AllowBucklingUnConsider.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, dThick);
	double	dAllowCoeff		= AllowBucklingCoeff.GetAllowStress(ALLOW_BUCKLINGCOEFF, dThick);
	double	dAllowBuckling	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dThick);
	double	dAllowTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dThick);
	//////////////////////////////////////////////////////////////////////////	

	if(!bIsPier)
	{
		if(pBridgeApp->m_nQtyJigan > 2)
			pOut->SetXL(pXL, "6.4 지점부 보강재",			m_nSttRow, "B", "B", 1);
		else
			pOut->SetXL(pXL, "6.3 지점부 보강재",			m_nSttRow, "B", "B", 1);
		pOut->SetXL(pXL, "가) 단지점부",m_nSttRow, "B", "B", 1);
	}
	else
	{
		pOut->SetXL(pXL, "나) 중간지점부",m_nSttRow, "B", "B", 1);
	}
	
	if(nQty > 0 && nQty < 5)
	{
		sText.Format("보강재_%d단", nQty);
		pOut->InsertImage(pXL, sText, m_nSttRow, "H");
		SetLines(8);
	}
	
	pOut->SetXL(pXL, "사용강종",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sLeft,							m_nSttRow, "I", "K", 0, RED,   9, FALSE, TA_CENTER, "USESTEEL", 0);
	pOut->SetXL(pXL, sRight,						m_nSttRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "최대지점반력",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Rmax",						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dMaxReaction,					m_nSttRow, "L", "N", 0, RED,   9, FALSE, TA_CENTER, "RMAX");
	pOut->SetXL(pXL, "kN",							m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, "( 절점",						m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, (double)nMaxJoint + 1,			m_nSttRow, "S", "S", 0, RED,   9, FALSE, TA_CENTER, "J_MAX", 0);
	pOut->SetXL(pXL, ")",							m_nSttRow, "T", "T", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "보강재의 폭",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "b",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dWidth,						m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "B", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "보강재의 길이",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "l",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dHeight,						m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "L", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "보강재의 두께",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dThick,						m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "TS", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 0);
	pOut->SetXL(pXL, "tsmin = b /",					m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, 16.0,							m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "TS_COEFF", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("B"), pOut->GetCellRef("TS_COEFF"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "TS_MIN");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("TS"), pOut->GetCellRef("TS_MIN"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "N", "N", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("TS"), pOut->GetCellRef("TS_MIN"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "X", "X", 1, RED);

	pOut->SetXL(pXL, "보강재의 설치간격",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "d",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dGap,							m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "D", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 0);
	pOut->SetXL(pXL, "'( ☞ 도.설.기 3.8.7.2 )",	m_nSttRow, "U", "U", 1);

	pOut->SetXL(pXL, "보강재의 사용열수",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, nQty,							m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "N", 0);
	pOut->SetXL(pXL, "EA",							m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "복부의 두께",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "tw",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTw,							m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "TW", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "d",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("D"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "STIFF_TERM");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "24·tw",						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=24*%s", pOut->GetCellRef("TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "24TW", 1);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "이므로",						m_nSttRow, "R", "R", 0);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("STIFF_TERM"), pOut->GetCellRef("24TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "J", "J", 1, RED,   9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "단면유효폭",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "Be",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>=%s,\"2·12tw + \"&%s-1&\"·d\",\"2·12tw + \"&%s-1&\"·24 tw\")", pOut->GetCellRef("24TW"), pOut->GetCellRef("STIFF_TERM"), pOut->GetCellRef("N"), pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>=%s,2*12*%s+(%s-1)*%s,2*12*%s+(%s-1)*24*%s)",
					pOut->GetCellRef("24TW"),pOut->GetCellRef("STIFF_TERM"),pOut->GetCellRef("TW"),pOut->GetCellRef("N"),
					pOut->GetCellRef("STIFF_TERM"),pOut->GetCellRef("TW"),pOut->GetCellRef("N"),pOut->GetCellRef("TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "BE");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "Q", "Q", 1);

	pOut->SetXL(pXL, "보강재의 단면적",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "As",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("B")); 
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "AS_COEFF1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("TS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "AS_COEFF2", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*2", pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER, "AS_COEFF3", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s*%s,3)", pOut->GetCellRef("AS_COEFF1"), pOut->GetCellRef("AS_COEFF2"), pOut->GetCellRef("AS_COEFF3"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "AS");
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "T", "T", 1);

	pOut->SetXL(pXL, "유효단면적",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "Ae",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BE"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "AE_COEFF1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "AE_COEFF2", 1);
	pOut->SetXL(pXL, "+",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "AE_COEFF3", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s+%s,3)", pOut->GetCellRef("AE_COEFF1"), pOut->GetCellRef("AE_COEFF2"), pOut->GetCellRef("AE_COEFF3"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "AE", 1);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "T", "T", 0);
	pOut->SetXL(pXL, "1.7As =",						m_nSttRow, "V", "V", 0);
	sText.Format("=1.7*%s", pOut->GetCellRef("AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "1.7AS", 1);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "Z", "Z", 0);

	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("AE"), pOut->GetCellRef("1.7AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "U", "U", 1, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "따라서, 유효단면적",			m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "Ae",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=MIN(%s,%s)", pOut->GetCellRef("AE"), pOut->GetCellRef("1.7AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "USE_AE", 1);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "'( ☞ 도.설.기 3.8.7.1 )",	m_nSttRow, "U", "U", 1);

	pOut->SetXL(pXL, "복부폭 중심선 둘레의 단면2차모멘트",m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "I",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\"³/ 12  +  { \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\"³/ 12  +  \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" × \"&ROUND((%s+%s)/2,3)&\"²} × \"&ROUND(%s*2,3)",
					pOut->GetCellRef("BE"), pOut->GetCellRef("TW"), pOut->GetCellRef("TS"), pOut->GetCellRef("B"),
					pOut->GetCellRef("TS"), pOut->GetCellRef("B"), pOut->GetCellRef("B"), pOut->GetCellRef("TW"), pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND((%s*%s^3/12)+((%s*%s^3/12)+(%s*%s*((%s+%s)/2)^2))*%s*2,3)",
					pOut->GetCellRef("BE"), pOut->GetCellRef("TW"), pOut->GetCellRef("TS"), pOut->GetCellRef("B"),
					pOut->GetCellRef("TS"), pOut->GetCellRef("B"), pOut->GetCellRef("B"), pOut->GetCellRef("TW"), pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "I",1);
	pOut->SetXL(pXL, "mm⁴",						m_nSttRow, "J", "J", 1);

	pOut->SetXL(pXL, "단면2차반경",					m_nSttRow, "C", "C", 0);
	pOut->SetLineBoxText(pXL, "r",					m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "√",					m_nSttRow, "I", "I", 2, BLACK,22, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "I",				m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=ROUND(SQRT(%s/%s),3)", pOut->GetCellRef("I"), pOut->GetCellRef("USE_AE"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "M", "O", 2, BLACK, 9, FALSE, TA_CENTER, "R", 3, FALSE);
	pOut->SetLineBoxText(pXL, "mm",					m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(NEXT);
	pOut->SetXL(pXL, "Ae",							m_nSttRow, "J", "K", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "유효좌굴길이",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "le",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "½ × l",						m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(1/2*%s,3)", pOut->GetCellRef("L"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "LE", 1);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "P", "P", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "허용축방향 압축응력",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "'( ☞ 도.설.기 3.3.2.1 )",	m_nSttRow, "U", "U", 1);
	pOut->SetXL(pXL, "le/r",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s/%s,3)", pOut->GetCellRef("LE"), pOut->GetCellRef("R"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "LE_R");
	pOut->SetXL(pXL, "⇒",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "fcag",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAllowConsider,				m_nSttRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "FCAG", 1);
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 1);
	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "국부좌굴을 고려하지 않은 허용축방향 압축응력",m_nSttRow, "M", "M", 1);

	sText.Format("=IF((%s/%s)>16,\"강재두께 확인 요망\",IF((%s/%s)<=%g,\"b ≤ \"&%g&\"/ t \",\"b/16 ≤ t < b/\"&%g&\"\"))",
				pOut->GetCellRef("B"), pOut->GetCellRef("TS"), pOut->GetCellRef("B"), pOut->GetCellRef("TS"), dAllowCoeff, dAllowCoeff, dAllowCoeff);
	pOut->SetXL(pXL, sText,							m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "⇒",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "fcal",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAllowBuckling,				m_nSttRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "FCAL", 1);
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 1);
	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "국부좌굴에 대한 허용응력",	m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "fcao",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAllowTension,					m_nSttRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "FCAO", 1);
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 1);
	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "국부좌굴을 고려하지 않은 최대 허용축방향 압축응력",	m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "fca",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "fcag · fcal  /  fcao",		m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s/%s,3)", pOut->GetCellRef("FCAG"), pOut->GetCellRef("FCAL"), pOut->GetCellRef("FCAO"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "FCA", 1);
	pOut->SetXL(pXL, "MPa",					m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, "'( ☞ 도.설.기 3.3.2.1 )",	m_nSttRow, "U", "U", 1);

	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "허용축방향 압축응력",			m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "fc",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Rmax / Ae",					m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*1000/%s,3)", pOut->GetCellRef("RMAX"), pOut->GetCellRef("USE_AE"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "FC");
	pOut->SetXL(pXL, "MPa",					m_nSttRow, "M", "M", 0);
	pOut->SetXL(pXL, "fca",							m_nSttRow, "Q", "Q", 0);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("FC"), pOut->GetCellRef("FCA"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("FC"), pOut->GetCellRef("FCA"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "X", "X", 1, RED);

}
///< 잭업 보강재 검토 - 교대부, 교각부
void CXLDesignStiff::JackUpSupport(CStringArray &strArrUseSheet, BOOL bIsPier)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CXLWeldingCheck		*pWeldingCheck	= m_pAPlateOutCalcStd->GetWeldingCheck();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage			*pManage = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CSteel				*pSteelCtl		= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();

	m_nSttRow = 1;
	pOut->ClearCellRef();
	CString	sText, sLeft, sNum, sRight;
	long	nMaxJoint				= pWeldingCheck->GetMaxJoint(bIsPier);
	BOOL	bExistAbut				= pWeldingCheck->ExistAbut();
	BOOL	bExistPier				= pWeldingCheck->ExistPier();
	if(bIsPier)
	{
		if(bExistPier)	strArrUseSheet.Add("6.3 JackUp 보강재(교각부)");
		else			return;
	}
	else
	{
		if(bExistAbut)	strArrUseSheet.Add("6.3 JackUp 보강재(교대부)");
		else			return;
	}
	CPlateBasicIndex	*pBx		= pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nMaxJoint));
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	long	nJijum		= pGir->GetNumberJijumByBx(pBx);
	CJackupStiff		*pJackUp	= pGir->GetJackupByJijum(nJijum, 0);
	if(pJackUp == NULL) return;

	double	dWidthL	= pJackUp->GetWidth(TRUE);
	double	dWidthR	= pJackUp->GetWidth(FALSE);
	double	dThick	= pJackUp->GetThick(TRUE);
	double	dHeight	= pJackUp->GetHeight(TRUE);
	double	dTw		= pGir->GetThickJewon(G_W, pBx);
	long	nQty   	    = pGir->GetQtyRowJackupByBx(pBx);
	long	nQtyOneShoe = GetJackupBuildsuByOneShoe(pBx);
	double	dRd		= pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(nMaxJoint)->m_vPreDeadSteel[JOINT_REACTION].z+
					  pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(nMaxJoint)->m_vPreDeadSlab[JOINT_REACTION].z+
					  pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(nMaxJoint)->m_vPostDead[JOINT_REACTION].z;
	double	dRl		= fabs(pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(nMaxJoint)->m_vLiveMax[JOINT_REACTION].z);
	double	dGap	= (bIsPier ? nQty - 1 <2 : nQty < 2) ? 0 : ABS(pBridgeApp->m_dJackUpTerm[nJijum][1]-pBridgeApp->m_dJackUpTerm[nJijum][0]);

	CDesignJijumStiff   pJStiff(pCalcStd, pBx, nMaxJoint);
	
	CCalcGeneral	*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	pCalcGeneral->GetMaterial(pBx, V_J_ST, sLeft, sRight);

	// 허용응력 //////////////////////////////////////////////////////////////	
	double	dLengthBuckling	= pJStiff.m_strJackupStiff.m_dBuckLing_le;
	double	dTurningRadius	= pJStiff.m_strJackupStiff.m_dDan_r;

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeft + sRight)];
	CHGAllowStress	AllowBucklingUnConsider(pSteel);	// 국부좌굴을 고려하지 않은 허용압축응력
	CHGAllowStress	AllowBucklingCoeff(pSteel);			// 자유돌출판의 국부좌굴에 대한 허용응력 계수
	CHGAllowStress	AllowBuckling(pSteel);						// 자유돌출판의 국부좌굴에 대한 허용응력
	CHGAllowStress	AllowTension(pSteel);					// 허용인장응력

	AllowBucklingUnConsider.SetAllowBucklingUnconsider(dLengthBuckling, dTurningRadius);
	AllowBuckling.SetAllowBuckling(dThick, dWidthL);
	double	dAllowConsider	= AllowBucklingUnConsider.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, dThick);
	double	dAllowCoeff		= AllowBucklingCoeff.GetAllowStress(ALLOW_BUCKLINGCOEFF, dThick);
	double	dAllowBuckling	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dThick);
	double	dAllowTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dThick);
	//////////////////////////////////////////////////////////////////////////	

	if(!bIsPier)
	{
		if(pBridgeApp->m_nQtyJigan > 2)
			pOut->SetXL(pXL, "6.5 JACK UP용 보강재",		m_nSttRow, "B", "B", 1);
		else
			pOut->SetXL(pXL, "6.4 JACK UP용 보강재",		m_nSttRow, "B", "B", 1);
		pOut->SetXL(pXL, "가) 단지점부",m_nSttRow, "B", "B", 1);
	}
	else
	{
		pOut->SetXL(pXL, "나) 중간지점부",m_nSttRow, "B", "B", 1);
	}

	if(nQty > 0 && nQty < 5)
	{
		sText.Format("보강재_%d단", bIsPier ? nQty - 1 : nQty);
		pOut->InsertImage(pXL, sText, m_nSttRow, "H");
		SetLines(8);
	}
	
	pOut->SetXL(pXL, "사용강종",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sLeft,							m_nSttRow, "I", "K", 0, RED,   9, FALSE, TA_CENTER, "USESTEEL", 0);
	pOut->SetXL(pXL, sRight,						m_nSttRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "최대지점반력",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Rd",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dRd,							m_nSttRow, "K", "M", 0, RED,   9, FALSE, TA_CENTER, "RD");
	pOut->SetXL(pXL, "kN",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ",",							m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Rl",							m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dRl,							m_nSttRow, "R", "T", 0, RED, 9, FALSE, TA_CENTER, "RL");
	pOut->SetXL(pXL, "kN",							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( 절점",						m_nSttRow, "V", "V", 0);
	pOut->SetXL(pXL, (double)nMaxJoint + 1,			m_nSttRow, "X", "X", 0, RED,   9, FALSE, TA_CENTER, "J_MAX", 0);
	pOut->SetXL(pXL, ")",							m_nSttRow, "Y", "Y", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "보강재의 폭",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "b1",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dWidthL,						m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "B1", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 0);

	pOut->SetXL(pXL, "b1",							m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dWidthR,						m_nSttRow, "Q", "R", 0, RED,   9, FALSE, TA_CENTER, "B2", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "S", "S", 1);

	pOut->SetXL(pXL, "보강재의 길이",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "l",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dHeight,						m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "L", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "보강재의 두께",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dThick,						m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "TS", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 0);
	pOut->SetXL(pXL, "tsmin = b2 /",				m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, 16.0,							m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "TS_COEFF", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("B2"), pOut->GetCellRef("TS_COEFF"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "TS_MIN");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("TS"), pOut->GetCellRef("TS_MIN"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "N", "N", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("TS"), pOut->GetCellRef("TS_MIN"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "X", "X", 1, RED);

	pOut->SetXL(pXL, "보강재의 설치간격",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "d",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dGap,							m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "D", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 0);
	pOut->SetXL(pXL, "'( ☞ 도.설.기 3.8.7.2 )",	m_nSttRow, "U", "U", 1);

	pOut->SetXL(pXL, "보강재의 사용열수",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, bIsPier ? nQty - 1 : nQty,		m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "N", 0);
	pOut->SetXL(pXL, "EA",							m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "Web의 두께",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, ":",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "tw",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTw,							m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "TW", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "SHOE 1개소당 JACK UP장치 설치개소 :",m_nSttRow, "C", "C", 0);
	sText.Format("=%d", nQtyOneShoe);
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER, "JACKUP_SU", 0);
	pOut->SetXL(pXL, "개소",						m_nSttRow, "M", "M", 1);
	pOut->SetXL(pXL, "※ 교좌받침 교체를위한 보강부재의설계시 설계반력은 평상시 지점반력을",m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "   할증한값(Rd+1.5Rl)으로 검토한다.       ☞(강.상.세 2006 3.4.3)", m_nSttRow, "D", "D", 1);

	pOut->SetXL(pXL, "적용하중",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "'P",							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("RD"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "P_COEFF1");
	pOut->SetXL(pXL, "+",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 1.5,							m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "P_COEFF2", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("RL"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "P_COEFF3");
	pOut->SetXL(pXL, ")",							m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "/",							m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=%s", pOut->GetCellRef("JACKUP_SU"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER, "P_COEFF4", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s+%s*%s)/%s", pOut->GetCellRef("P_COEFF1"), pOut->GetCellRef("P_COEFF2"), pOut->GetCellRef("P_COEFF3"), pOut->GetCellRef("P_COEFF4"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "P");
	pOut->SetXL(pXL, "kN",							m_nSttRow, "Z", "Z", 1);

	pOut->SetXL(pXL, "단면유효폭",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "d",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("D"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "STIFF_TERM", 0);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "24·tw",						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=24*%s", pOut->GetCellRef("TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "24TW", 1);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "이므로",						m_nSttRow, "R", "R", 0);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("STIFF_TERM"), pOut->GetCellRef("24TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "J", "J", 1, RED,   9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Be",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>=%s,\"2·12tw + \"&%s-1&\"·d\",\"2·12tw + \"&%s-1&\"·24 tw\")", pOut->GetCellRef("24TW"), pOut->GetCellRef("STIFF_TERM"), pOut->GetCellRef("N"), pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>=%s,2*12*%s+(%s-1)*%s,2*12*%s+(%s-1)*24*%s)",
					pOut->GetCellRef("24TW"),pOut->GetCellRef("STIFF_TERM"),pOut->GetCellRef("TW"),pOut->GetCellRef("N"),
					pOut->GetCellRef("STIFF_TERM"),pOut->GetCellRef("TW"),pOut->GetCellRef("N"),pOut->GetCellRef("TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "BE");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "Q", "Q", 1);

	pOut->SetXL(pXL, "보강재의 단면적",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "As",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" ×\"&ROUND(%s,3)&\" + \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)",
				pOut->GetCellRef("B1"), pOut->GetCellRef("TS"), pOut->GetCellRef("N"), pOut->GetCellRef("B2"), pOut->GetCellRef("TS"), pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s*%s+%s*%s*%s,3)", pOut->GetCellRef("B1"), pOut->GetCellRef("TS"), pOut->GetCellRef("N"), pOut->GetCellRef("B2"), pOut->GetCellRef("TS"), pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "AS");
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "T", "T", 1);

	pOut->SetXL(pXL, "유효단면적",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "Ae",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BE"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "AE_COEFF1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("TW"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "AE_COEFF2", 1);
	pOut->SetXL(pXL, "+",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "AE_COEFF3", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s+%s,3)", pOut->GetCellRef("AE_COEFF1"), pOut->GetCellRef("AE_COEFF2"), pOut->GetCellRef("AE_COEFF3"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "AE", 1);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "T", "T", 0);
	pOut->SetXL(pXL, "1.7As =",						m_nSttRow, "V", "V", 0);
	sText.Format("=1.7*%s", pOut->GetCellRef("AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "1.7AS", 1);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "Z", "Z", 0);

	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("AE"), pOut->GetCellRef("1.7AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "U", "U", 1, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "따라서, 유효단면적",			m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "Ae",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=MIN(%s,%s)", pOut->GetCellRef("AE"), pOut->GetCellRef("1.7AS"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "USE_AE", 1);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "'( ☞ 도.설.기 3.8.7.1 )",	m_nSttRow, "U", "U", 1);

	pOut->SetXL(pXL, "중립축의 위치",				m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "Yo",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Σ(A·y) / ΣAe",				m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"{ \"&ROUND(%s,3)&\" × ( \"&ROUND(%s*%s,3)&\" × \"&ROUND(%s/2,3)&\" ＋ \"&ROUND(%s*%s,3)&\" × \"&ROUND(%s+%s+%s/2,3)&\" ) ＋ \"&ROUND(%s*%s,3)&\" × \"&ROUND(%s+%s/2,3)&\" } / \"&ROUND(%s,3)",
					pOut->GetCellRef("N"), pOut->GetCellRef("B2"), pOut->GetCellRef("TS"), pOut->GetCellRef("B2"), pOut->GetCellRef("B1"),
					pOut->GetCellRef("TS"),pOut->GetCellRef("B2"), pOut->GetCellRef("TW"), pOut->GetCellRef("B1"), pOut->GetCellRef("BE"),
					pOut->GetCellRef("TW"),pOut->GetCellRef("B2"), pOut->GetCellRef("TW"), pOut->GetCellRef("USE_AE"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND((%s*(%s*%s*%s/2+%s*%s*(%s+%s+%s/2))+%s*%s*(%s+%s/2))/%s,3)",
					pOut->GetCellRef("N"), pOut->GetCellRef("B2"), pOut->GetCellRef("TS"), pOut->GetCellRef("B2"), pOut->GetCellRef("B1"),
					pOut->GetCellRef("TS"),pOut->GetCellRef("B2"), pOut->GetCellRef("TW"), pOut->GetCellRef("B1"), pOut->GetCellRef("BE"),
					pOut->GetCellRef("TW"),pOut->GetCellRef("B2"), pOut->GetCellRef("TW"), pOut->GetCellRef("USE_AE"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "YO");
	pOut->SetXL(pXL, "mm",							m_nSttRow, "I", "I", 1);

	pOut->SetXL(pXL, "중립축 둘레의 단면2차모멘트",	m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "I",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\"³/ 12 + \"&ROUND(%s*%s,3)&\" × \"&ROUND(ABS(%s+%s/2-%s),3)",
					pOut->GetCellRef("BE"),pOut->GetCellRef("TW"),pOut->GetCellRef("BE"),pOut->GetCellRef("TW"),pOut->GetCellRef("B2"),
					pOut->GetCellRef("TW"),pOut->GetCellRef("YO"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "F", 1);

	sText.Format("=\"+ ( \"&ROUND(%s,3)&\" ×\"&ROUND(%s,3)&\" ³/ 12 + \"&ROUND(%s*%s,3)&\" ×\"&ROUND(ABS(%s/2-%s),3)&\"²＋ \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" ³/ 12 + \"&ROUND(%s*%s,3)&\" × \"&ROUND(ABS(%s+%s+%s/2-%s),3)&\"²) ×\"&ROUND(%s,3)",
					pOut->GetCellRef("TS"),pOut->GetCellRef("B2"),pOut->GetCellRef("TS"),pOut->GetCellRef("B2"),pOut->GetCellRef("B2"),
					pOut->GetCellRef("YO"),pOut->GetCellRef("TS"),pOut->GetCellRef("B1"),pOut->GetCellRef("B1"),pOut->GetCellRef("TS"),
					pOut->GetCellRef("B2"),pOut->GetCellRef("TW"),pOut->GetCellRef("B1"),pOut->GetCellRef("YO"),pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s^3/12+%s*%s*(ABS(%s+%s/2-%s))^2+(%s*%s^3/12+%s*%s^3/12+%s*%s*(ABS(%s/2-%s))^2+%s*%s*(ABS(%s+%s+%s/2-%s))^2)*%s,3)",
					pOut->GetCellRef("BE"),pOut->GetCellRef("TW"),pOut->GetCellRef("BE"),pOut->GetCellRef("TW"),pOut->GetCellRef("B2"),
					pOut->GetCellRef("TW"),pOut->GetCellRef("YO"),pOut->GetCellRef("TS"),pOut->GetCellRef("B2"),pOut->GetCellRef("TS"),
					pOut->GetCellRef("B2"),pOut->GetCellRef("B2"),pOut->GetCellRef("YO"),pOut->GetCellRef("TS"),pOut->GetCellRef("B1"),
					pOut->GetCellRef("B1"),pOut->GetCellRef("TS"),pOut->GetCellRef("B2"),pOut->GetCellRef("TW"),pOut->GetCellRef("B1"),pOut->GetCellRef("YO"),pOut->GetCellRef("N"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "I");
	pOut->SetXL(pXL, "mm⁴",						m_nSttRow, "J", "J", 1);

	pOut->SetXL(pXL, "단면2차반경",					m_nSttRow, "C", "C", 0);
	pOut->SetLineBoxText(pXL, "r",					m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "√",					m_nSttRow, "I", "I", 2, BLACK,22, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "I",				m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=ROUND(SQRT(%s/%s),3)", pOut->GetCellRef("I"), pOut->GetCellRef("USE_AE"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "M", "O", 2, BLACK, 9, FALSE, TA_CENTER, "R", 3, FALSE);
	pOut->SetLineBoxText(pXL, "mm",					m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(NEXT);
	pOut->SetXL(pXL, "Ae",							m_nSttRow, "J", "K", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "유효좌굴길이",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "le",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "½ × l",						m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(1/2*%s,3)", pOut->GetCellRef("L"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "LE", 1);
	pOut->SetXL(pXL, "mm",							m_nSttRow, "P", "P", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "허용축방향 압축응력 (25% 할증)",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "'( ☞ 도.설.기 3.3.2.1 )",	m_nSttRow, "U", "U", 1);
	pOut->SetXL(pXL, "le/r",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s/%s,3)", pOut->GetCellRef("LE"), pOut->GetCellRef("R"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "LE_R");
	pOut->SetXL(pXL, "⇒",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "fcag",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=%.3f * 1.25", dAllowConsider);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "FCAG", 1);
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 1);
	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "국부좌굴을 고려하지 않은 허용축방향 압축응력",m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "fcal",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f * 1.25", dAllowBuckling);
	pOut->SetXL(pXL, sText,				m_nSttRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "FCAL", 1);
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 1);
	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "국부좌굴에 대한 허용응력",	m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "fcao",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.3f * 1.25", dAllowTension);
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "FCAO", 1);
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 1);
	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "국부좌굴을 고려하지 않은 최대 허용축방향 압축응력",	m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "fca",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "fcag · fcal  /  fcao",		m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s/%s,3)", pOut->GetCellRef("FCAG"), pOut->GetCellRef("FCAL"), pOut->GetCellRef("FCAO"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "FCA", 1);
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 1);
	pOut->SetXL(pXL, ":",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "허용축방향 압축응력",			m_nSttRow, "M", "M", 1);

	pOut->SetXL(pXL, "fc",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Rmax / Ae",					m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*1000/%s,3)", pOut->GetCellRef("P"), pOut->GetCellRef("USE_AE"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "FC");
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "M", "M", 0);
	pOut->SetXL(pXL, "fca",							m_nSttRow, "Q", "Q", 0);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("FC"), pOut->GetCellRef("FCA"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("FC"), pOut->GetCellRef("FCA"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "X", "X", 1, RED);
}

///< SHOE 1개소당 JACK UP장치 설치개소수 구하기
long CXLDesignStiff::GetJackupBuildsuByOneShoe(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CPlateGirderApp		*pGir			= pBx->GetGirder();

	long	nJijum		= pGir->GetNumberJijumByBx(pBx);
	long	nGaesu		= 0;	
	double	dJackupDis	= 0;
	BOOL	bPlus		= FALSE;
	BOOL	bMinus		= FALSE;

	for(long nJackIdx = 0; nJackIdx < JACKUPSUMAX; nJackIdx++)
	{
		dJackupDis = pBridgeApp->m_dJackUpTerm[nJijum][nJackIdx];
		if(dJackupDis == 0) break;
		if(dJackupDis > 0)  bPlus = TRUE;
		else				bMinus = TRUE;
	}

	if(bPlus)	nGaesu++;
	if(bMinus)	nGaesu++;

	return nGaesu;
}

void CXLDesignStiff::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}
///////////////////////////////////////////////////////////////////////////////////
//∫∴∵∮∑∏ㄷ ΑΒΓΔΕΖΗαβγδεζηΣθιΙκλμνξοπρf υφχψω
//	´≤＞＜≠±≤≥∞⌒×㎝⁴ ㎤ ㎠ ㎝ kgf/㎠ t·m  ¹²³⁴₁₂₃₄
//  ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩ ⑪ ⑫ ⑬ ⑭ ⑮ √½⅓㉮㉯㉰㉱㉲㉳㉴
//  ━━━━━━━━━━━━━  ⇒ ⊙
//  ┏ ┗ ┛ ┓┃━ ┣ ┫ⅰ ⅱ ⅲ ⅳ ⅴ ⅵ ⅶ ⅷ ⅸ ⅹ Ⅰ Ⅱ Ⅲ Ⅳ Ⅳ Ⅴ Ⅵ
///////////////////////////////////////////////////////////////////////////////////// 