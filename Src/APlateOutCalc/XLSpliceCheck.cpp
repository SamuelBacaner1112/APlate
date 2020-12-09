// XLSpliceCheck.cpp: implementation of the CXLSpliceCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLSpliceCheck::CXLSpliceCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
}

CXLSpliceCheck::~CXLSpliceCheck()
{

}
////////////////////////////////////////////////////////////////////////////////////////
///5.이음부의 설계
///    _SP = 전체, _CP = 압축, _TS = 인장
////////////////////////////////////////////////////////////////////////////////////////
void CXLSpliceCheck::SpliceCheck()
{
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CCalcData           *pData          = pDataManage->GetCalcData(); 
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CSteelBolt          *pSteelBolt     = m_pAPlateOutCalcStd->GetDataManage()->GetSteelBolt();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CString	sText;
	m_nSttRow = 1;
	pOut->SetXL(pXL, "5.이음부의 설계",       m_nSttRow, "A", "A", 1, BLACK);
	pOut->SetXL(pXL, " 5.1  현장이음의 설계", m_nSttRow, "A", "A", 1, BLACK);
	
	long	nQtySp	= pData->m_ArrSecSpDesign.GetSize();
	long	nG		= 0;
	double	dSta	= 0;

	for(long nSp=0; nSp<nQtySp; nSp++)
	{
		if(pOutExcel->m_bAbort)	return;
		pOut->ClearCellRef();
		
		nG   = (long)pData->m_ArrSecSpDesign.GetAt(nSp).x;
		dSta = pData->m_ArrSecSpDesign.GetAt(nSp).y;

		CPlateGirderApp  *pGir = pBridgeApp->GetGirder(nG);
		CPlateBasicIndex *pBx  = pGir->GetBxByStation(dSta, BX_SPLICE);
		CPlateSplice     *pSp  = pBx->GetSplice();
		pModelCalc->GetSectionCoefficient(pBx, TRUE);
		
		BOOL bHybrid	= pBx->IsHybrid();
		long nSpsu		= pGir->GetQtySplice(CPlateGirder::SPLICE);

		if (nSpsu < 1) 
		{			
			sText.Format(" 현장 이음이 없습니다.");
			pOut->SetXL(pXL, sText, m_nSttRow, "B", "B", 1, BLACK, 9, TRUE);
			
			break;//모두 현장 용접일 경우....070213...KB...
		}

		if(pSp->m_bSpliceType==FALSE)
			continue;

		pStressCalc->GetSectionStress(nG, dSta);

		double dMs = pStressCalc->GetMapValueSecStress("MomentPreDead");
		double dMv = pStressCalc->GetMapValueSecStress("MomentPostDead")+pStressCalc->GetMapValueSecStress("MomentLive")+pStressCalc->GetMapValueSecStress("MomentWind");
		double dSs = fabs(pStressCalc->GetMapValueSecStress("ShearPreDead"));
		double dSv = fabs(pStressCalc->GetMapValueSecStress("ShearPostDead")+pStressCalc->GetMapValueSecStress("ShearLive")+pStressCalc->GetMapValueSecStress("ShearWind"));
		double dMt = fabs(pStressCalc->GetMapValueSecStress("TorsionSum"));

		double Max_up=0, Max_lo=0, Max_upA=0, Max_loA=0, Tau_up=0, Tau_lo=0, Tau_upA=0, Tau_loA=0;
		pStressCalc->GetFlangeStress(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA);

		BOOL	bPositive	= pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);
		CString szTemp		= _T("");
		// 단면번호
		CString sCnt = pBx->GetSplice()->GetMarkstring();
		sCnt = sCnt.Mid(2);

		szTemp.Format("%s) SPLICE - %s", sCnt, sCnt);	
		//szTemp.Format("%d) SPLICE - %d", nSp+1, nSp+1);	
		pOut->SetXL(pXL, szTemp,                          m_nSttRow, "B", "E", 0, RED);	///< 1) SPLICE - 1
		// 절점번호
		double	dRatio			= 0;
		long	nElem			= pModelCalc->GetElemNoAndRatio(nG, dSta, dRatio);
		double	dLengthElm		= pModelCalc->GetElemLength(nElem, FALSE);
		double	dLengthFromStt	= dRatio*dLengthElm;
		CElement *pElm	= pFEMManage->GetElement((unsigned short)nElem);

		szTemp.Format("거더 %d , 절점 %d 부터 %.3f m", nG+1, pElm->m_nJStt+1, dLengthFromStt);
		pOut->SetXL(pXL, szTemp, m_nSttRow, "H", "H", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);

		// 단면가정 및 설계조건
		pOut->SetXL(pXL, "합성전 휨모멘트        : Ms =",	m_nSttRow, "B", "I", 0);
		sText.Format("%.6f", tokNM(dMs));
		pOut->SetXL(pXL, sText,								m_nSttRow, "J", "M", 0, RED, 9, FALSE, TA_CENTER, "Ms_SP"); 
		pOut->SetXL(pXL, "kN·m",							m_nSttRow, "N", "O"); 

		pOut->SetXL(pXL, "합성후 휨모멘트        : Mv =",	m_nSttRow, "B", "I", 0); 
		sText.Format("%.6f", tokNM(dMv));
		pOut->SetXL(pXL, sText,							m_nSttRow, "J", "M", 0, RED, 9, FALSE, TA_CENTER, "Mv_SP");  
		pOut->SetXL(pXL, "kN·m",							m_nSttRow, "N", "O");

		pOut->SetXL(pXL, "합성전 전단력          : Ss =",	m_nSttRow, "B", "I", 0); 
		sText.Format("%.6f", tokN(dSs));
		pOut->SetXL(pXL, sText,								m_nSttRow, "J", "M", 0, RED, 9, FALSE, TA_CENTER, "Ss_SP");  
		pOut->SetXL(pXL, "kN",								m_nSttRow, "N", "O");

		pOut->SetXL(pXL, "합성후 전단력          : Sv =",	m_nSttRow, "B", "I", 0); 
		sText.Format("%.6f", tokN(dSv));
		pOut->SetXL(pXL, sText,								m_nSttRow, "J", "M", 0, RED, 9, FALSE, TA_CENTER, "Sv_SP"); 
		pOut->SetXL(pXL, "kN",								m_nSttRow, "N", "O");

		pOut->SetXL(pXL, "총 비틀림 모멘트       : Mt =",	m_nSttRow, "B", "I", 0); 
		sText.Format("%.6f", tokNM(dMt));
		pOut->SetXL(pXL, sText,								m_nSttRow, "J", "M", 0, RED, 9, FALSE, TA_CENTER, "Mt_SP");  
		pOut->SetXL(pXL, "kN·m",						  m_nSttRow, "N", "O");

		pOut->SetXL(pXL, "상부 FLANGE의응력      : fu =", m_nSttRow, "B", "I", 0); 
		pOut->SetXL(pXL, Max_up,						  m_nSttRow, "J", "M", 0, RED, 9, FALSE, TA_CENTER, "fu_SP", 1);  
		pOut->SetXL(pXL, "MPa",							  m_nSttRow, "N", "O");

		pOut->SetXL(pXL, "하부 FLANGE의응력      : fl =", m_nSttRow, "B", "I", 0); 
		pOut->SetXL(pXL, Max_lo,						  m_nSttRow, "J", "M", 0, RED, 9, FALSE, TA_CENTER, "fl_SP", 1);  
		pOut->SetXL(pXL, "Mpa",					  m_nSttRow, "N", "O");

		pOut->SetXL(pXL, "볼트 : H.T.B"                 , m_nSttRow, "B", "D", 0);
		CString szBoltJewon = "";
		CString szIndex     = "";
		CString szBoltDia   = pSp->m_ZzsBoltSize;						///<  M22  (Bolt 지름)
		CString szBoltMat   = pBridgeApp->GetMaterial(bPositive?SP_U_BOLT:SP_L_BOLT);	///<  F10T (Bolt 종류)--->압축부
		szBoltJewon.Format("(%s %s)", szBoltDia, szBoltMat);			///<  볼트제원(M22, F10T)
		szIndex.Format("%s %s", szBoltMat, szBoltDia);
		long   nBoltIndex  = pSteelBolt->GetIndex(szIndex);
		double dForceAllow = pSteelBolt->GetForceAllow(nBoltIndex);
		double dHybridCoffie = pStressCalc->GetMapValueSecStress("HyR_Pos");	// 합성후 하이브리드 계수 R

		///< (M22 F10T)
		CString	szBoltDiaLeft;
		CString	szBoltDiaRight;
		szBoltDiaLeft = szBoltDia.Left(1);
		szBoltDiaRight = szBoltDia.Right(2);
		szTemp.Format("( %s", szBoltDiaLeft);
		pOut->SetXL(pXL, szTemp,						m_nSttRow, "F", "F", 0, RED,   9, FALSE, TA_CENTER, "", 0);
		szTemp.Format("%s", szBoltDiaRight);
		pOut->SetXL(pXL, szTemp,						m_nSttRow, "G", "G", 0, RED,   9, FALSE, TA_CENTER, "BoltSize", 0);
		szTemp.Format("%s )", szBoltMat);
		pOut->SetXL(pXL, szTemp,						m_nSttRow, "H", "I", 0, RED,   9, FALSE, TA_CENTER, "", 0);
		///< ρa = 4800×2면마찰=9600
		pOut->SetXL(pXL, "ρa ="                        , m_nSttRow, "J", "K", 0);
		pOut->SetXL(pXL, dForceAllow                    , m_nSttRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "faBolt_SP", 0);	///< 1볼트마찰면 허용력
		pOut->SetXL(pXL, "×"                           , m_nSttRow, "N", "N", 0);
		pOut->SetXL(pXL, "2면마찰"                      , m_nSttRow, "O", "P", 0);  ///< 마찰면수
		pOut->SetXL(pXL, "="                            , m_nSttRow, "Q", "Q", 0);
		pOut->SetXL(pXL, dForceAllow*2                  , m_nSttRow, "R", "T", 0, RED, 9, FALSE, TA_CENTER, "faBoltTotal_SP", 0);  ///< 총마찰면 볼트허용력
		pOut->SetXL(pXL, "N"							, m_nSttRow, "U", "V", 1);
		if(bHybrid)
		{
			pOut->SetXL(pXL, "합성후 하이브리드 계수 : R ="	, m_nSttRow, "B", "B", 0);
			pOut->SetXL(pXL, dHybridCoffie					, m_nSttRow, "J", "M", 2, RED, 9, FALSE, TA_CENTER, "Hy_R", 3);		
		}
		else
			SetLines(1);		

		if(pOutExcel->m_bAbort)	return;
		sText.Format("<SPLICE - %d> (1) 압축부 플랜지", nSp+1);
		pOutExcel->SetProgress(sText, pOutExcel->GetProgress()+3);
		SpliceCheckCompressPart(pBx);	///< 압축부

		if(pOutExcel->m_bAbort)	return;
		sText.Format("<SPLICE - %d> (2) 인장부 플랜지", nSp+1);
		pOutExcel->SetProgress(sText, pOutExcel->GetProgress()+3);
		SpliceCheckTensionPart (pBx);	///< 인장부
		
		if(pOutExcel->m_bAbort)	return;
		sText.Format("<SPLICE - %d> (3) WEB 연결부의 계산", nSp+1);
		pOutExcel->SetProgress(sText, pOutExcel->GetProgress()+3);
		SpliceCheckWebPart1	   (pBx);	///< 복부 이음판1(볼트의 응력계산부분)
		if(pOutExcel->m_bAbort)	return;
		SpliceCheckWebPart2	   (pBx);	///< 복부 이음판2(이음판의 응력계산부분)
		if(pOutExcel->m_bAbort)	return;
		SetLines(NEXT);		
	}
	pXL->SetPrintArea(pXL->GetCellStr(0, 0), pXL->GetCellStr(m_nSttRow, 26));
}

void CXLSpliceCheck::SpliceCheckCompressPart(CPlateBasicIndex* pBx)
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CPlateGirderApp     *pGir           = pBx->GetGirder();
	CPlateSplice        *pSp            = pBx->GetSplice();
	CAPlateDrawPyung	DrawPyung(pDataManage);
	CSteel				*pSteelCtl		= pDataManage->GetSteel();

	long   nG        = pBx->GetNumberGirder();
	double dSta      = pBx->GetStation();
	BOOL   bHybrid	 = pBx->IsHybrid();
	BOOL   bPositive = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);
	long   nBujeMark = bPositive ? G_F_U : G_F_L;	///< 압축부 부재마크
	CString szTemp = "";

	pOut->SetXL(pXL, " (1) 압축부 플랜지"  , m_nSttRow, "B", "F", 0);
	if(bPositive)	pOut->SetXL(pXL, "(상부 플랜지의 이음)", m_nSttRow, "H", "M", 1);
	else			pOut->SetXL(pXL, "(하부 플랜지의 이음)", m_nSttRow, "H", "M", 1);

	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	pCalcGeneral->GetMaterial(pBx, nBujeMark, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowComp(pSteel);
	CHGAllowStress	AllowShear(pSteel);
	double dThickFlange = pGir->GetThickFactByStation(dSta, nBujeMark);
	double dWidthFlange = pGir->GetWidthOnStation(dSta, bPositive?TRUE:FALSE);	
	
	double dAllowComp	= AllowComp.GetAllowStress(ALLOW_COMPSTRESS, dThickFlange);
	double dAllowShear	= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, dThickFlange);
	// 필러 존재시 인접플랜지 두께 ///////////////////////////////////////////
	double dThickNear	= dThickFlange;
	BOOL bNextFiller	= TRUE;
	if(pBx->IsFiller(bPositive ? BX_UPPERFLANGE : BX_LOWERFLANGE, bNextFiller))
		dThickNear = pBx->GetFactChain(nBujeMark, TRUE)->m_dFactChainThick;
	//////////////////////////////////////////////////////////////////////////	

	SetLines(4);

	///< 도면 그림 삽입
	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(20);

	//070222일자 수정...KB...
	if(bPositive)	DrawPyung.DrawUpperFlangeSplice(&Dom, pBx, 0, 0, FALSE, TRUE);
	else			DrawPyung.DrawLowerFlangeSplice(&Dom, pBx, 0, 0, FALSE, TRUE);
	
	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 2, 11, FALSE, FALSE);
	SetLines(NEXT);

	pOut->SetXL(pXL, "사용 강종    :",			m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, strLeft,					m_nSttRow, "U", "W", 0, RED,   9, FALSE, TA_LEFT, "STEELTYPE_COMP", 0);
	pOut->SetXL(pXL, strRight,					m_nSttRow, "X", "X", 1, RED);
	if(dThickFlange!=dThickNear)
	{
		pOut->SetXL(pXL, "인접플랜지 두께   =",		m_nSttRow, "Q", "Q", 0);
		pOut->SetXL(pXL, dThickNear,				m_nSttRow, "V", "X", 0, RED,   9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, "mm",						m_nSttRow, "Y", "Y", 1);
	}
	if(bPositive)	pOut->SetXL(pXL, "모재두께     : Tu =",		m_nSttRow, "Q", "Q", 0);
	else			pOut->SetXL(pXL, "모재두께     : Tl =",		m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dThickFlange,				m_nSttRow, "V", "X", 0, RED,   9, FALSE, TA_CENTER, "FlangeT_CP", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "Y", "Y", 1);
	pOut->SetXL(pXL, "모재길이     :  L =",		m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dWidthFlange,				m_nSttRow, "V", "X", 0, RED,   9, FALSE, TA_CENTER, "FlangeW_CP", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "Y", "Y", 1);
	pOut->SetXL(pXL, "허용압축응력 : fa =",		m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dAllowComp,				m_nSttRow, "V", "X", 0, RED, 9, FALSE, TA_CENTER, "ALLOW_COMP", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 1);
	pOut->SetXL(pXL, "허용전단응력 : υa =",	m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dAllowShear,				m_nSttRow, "V", "X", 0, RED, 9, FALSE, TA_CENTER, "ALLOW_SHEAR", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
	m_nSttRow-=9;
	if(dThickFlange!=dThickNear)
		m_nSttRow--;

	pOut->SetXL(pXL, "fs =",					m_nSttRow, "C", "D", 0);
	szTemp.Format("=IF((%s+%s)>0,ABS(%s),ABS(%s))", pOut->GetCellRef("Ms_SP"), pOut->GetCellRef("Mv_SP"), pOut->GetCellRef("fu_SP"), pOut->GetCellRef("fl_SP"));             
	pOut->SetXL(pXL, szTemp,					m_nSttRow, "E", "I", 0, BLACK, 9, FALSE, TA_CENTER, "fs_CP", 1);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "J", "J", 1);

	pOut->SetXL(pXL, "fs' ="                        , m_nSttRow, "C", "D", 0);
	pOut->SetXL(pXL, 0.75                           , m_nSttRow, "E", "F", 0, RED, 9, FALSE, TA_CENTER, "0.75_SP", 2);
	pOut->SetXL(pXL, "×"                           , m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("ALLOW_COMP"));
	pOut->SetXL(pXL, szTemp                         , m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	
	if(bHybrid)
	{
		pOut->SetXL(pXL, "×"                           , m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		
		szTemp.Format("=%s",	pOut->GetCellRef("Hy_R"));

		pOut->SetXL(pXL, szTemp                         , m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	}

	pOut->SetXL(pXL, "=",							m_nSttRow, !bHybrid ? "J" : "M", !bHybrid ? "J" : "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);

	if(bHybrid)
		szTemp.Format("=%s*%s*%s", pOut->GetCellRef("0.75_SP"), pOut->GetCellRef("ALLOW_COMP"),pOut->GetCellRef("Hy_R"));
	else
		szTemp.Format("=%s*%s", pOut->GetCellRef("0.75_SP"), pOut->GetCellRef("ALLOW_COMP"));

	pOut->SetXL(pXL, szTemp,						m_nSttRow, !bHybrid ? "K" : "N", !bHybrid ? "L" : "O", 0, BLACK, 9, FALSE, TA_CENTER, "fs'_SP", 1);

	pOut->SetXL(pXL, "MPa",							m_nSttRow, !bHybrid ? "M" : "P", !bHybrid ? "N" : "Q", 0);
	pOut->SetXL(pXL, "( ☞ 도.설 3.5.1.1 )",		m_nSttRow, !bHybrid ? "P" : "R", !bHybrid ? "P" : "R", 1);

	pOut->SetXL(pXL, "∴ 이음부의 설계응력(fu)는 ",	m_nSttRow, "B", "B", 0);
	szTemp.Format("=MAX(%s,%s)", pOut->GetCellRef("fs_CP"), pOut->GetCellRef("fs'_SP"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "DesignFs_CP", 1);
	pOut->SetXL(pXL, "MPa을 적용한다.",				m_nSttRow, "K", "K", 9);
	if(dThickFlange!=dThickNear)
		m_nSttRow++;

	pOut->SetXL(pXL, "① 필요볼트수의 결정"         , m_nSttRow, "C", "C", 1);
	pOut->SetLineBoxText  (pXL, "n="				, m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetUnderLineText(pXL, "As × fs"			, m_nSttRow, "E", "G", 0);
	pOut->SetLineBoxText  (pXL, "="					, m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_CP"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "J", "J", 0);
	
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_CP"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "M", "M", 0);
	
	szTemp.Format("=%s", pOut->GetCellRef("DesignFs_CP"));
	pOut->SetUnderLineText(pXL, szTemp   			, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetLineBoxText  (pXL, "≒"				, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	
	m_nSttRow++;
	if(bHybrid)
	{
		pOut->SetXL		  (pXL, "R × ρa"			, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		szTemp.Format("=%s", pOut->GetCellRef("Hy_R"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Hy_R_temp", 3);
		pOut->SetXL		  (pXL, "×"				, m_nSttRow, "L", "L", 0);
		szTemp.Format("=%s", pOut->GetCellRef("faBoltTotal_SP"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/(%s*%s),0)", pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("DesignFs_CP"),
			pOut->GetCellRef("Hy_R_temp"), pOut->GetCellRef("faBoltTotal_SP"));
	}
	else
	{
		pOut->SetXL			  (pXL, "ρa"				, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		szTemp.Format("=%s", pOut->GetCellRef("faBoltTotal_SP"));
		pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/%s,0)", pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("DesignFs_CP"), pOut->GetCellRef("faBoltTotal_SP"));
	}	
	
	pOut->SetLineBoxText  (pXL, szTemp				, m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "EA"				, m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetLineBoxText  (pXL, "⇒"				, m_nSttRow, "S", "S", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetLineBoxText  (pXL, "n"					, m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetLineBoxText  (pXL, "="					, m_nSttRow, "U", "U", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	
	long nBoltsu = bPositive ? (pSp->m_uB+1)*2 * (pSp->m_uE+1):(pSp->m_dB+1)*2 * (pSp->m_dE+1);
	pOut->SetLineBoxText  (pXL, double(nBoltsu)		, m_nSttRow, "V", "V", 2, RED,	 9, FALSE, TA_CENTER, "TotalBoltsu_CP", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "EA사용"			, m_nSttRow, "W", "X", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	m_nSttRow++;
	m_nSttRow++;
	
	pOut->SetXL(pXL, "② 이음판의 결정"		        , m_nSttRow, "C", "C", 1);
	pOut->SetLineBoxText  (pXL, "필요단면적 : As ="	, m_nSttRow, "D", "H", 2, BLACK, 9, FALSE, TA_LEFT,   "", 3, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_CP"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "J", "J", 0);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_CP"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "M", "M", 0);
	szTemp.Format("=%s", pOut->GetCellRef("DesignFs_CP"));
	pOut->SetUnderLineText(pXL, szTemp   			, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetLineBoxText  (pXL, "="					, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	m_nSttRow++;
	if(bHybrid)
	{
		szTemp.Format("=%s", pOut->GetCellRef("Hy_R"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Hy_R_temp", 3);
		pOut->SetXL		  (pXL, "×"				, m_nSttRow, "L", "L", 0);
		szTemp.Format("=%s", pOut->GetCellRef("ALLOW_COMP"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/(%s*%s),0)", pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("DesignFs_CP"),
			pOut->GetCellRef("Hy_R_temp"), pOut->GetCellRef("ALLOW_COMP"));
	}
	else
	{
		szTemp.Format("=%s", pOut->GetCellRef("ALLOW_COMP"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/%s,0)", pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("DesignFs_CP"), pOut->GetCellRef("ALLOW_COMP"));
	}
	
	pOut->SetLineBoxText  (pXL, szTemp				, m_nSttRow, "Q", "R", 2, BLACK, 9, FALSE, TA_CENTER, "AreaReqSp_CP", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "mm²"				, m_nSttRow, "S", "S", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);

	m_nSttRow+=2;

	double dWidthSp       = pGir->GetWidthOnStation(dSta, bPositive?TRUE:FALSE);
	double dThickSp       = bPositive ? pSp->GetuT() : pSp->GetdT();
	double dLengthSp      = bPositive ? (pSp->m_uA*2+(pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1) : (pSp->m_dA*2+(pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1);
	double dWidthSpSmall  = bPositive ? pSp->m_uW : pSp->m_dW;
	double dThickSpSmall  = bPositive ? pSp->m_uT : pSp->m_dT;
	double dLengthSpSmall = bPositive ? (pSp->m_uA*2+(pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1) : (pSp->m_dA*2+(pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1);
	///< 외측이음판
	pOut->SetXL			  (pXL, "사용이음판 : As =" , m_nSttRow, "D", "H", 0);
	pOut->SetXL			  (pXL, "1"					, m_nSttRow, "I", "I", 0, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "-"					, m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			  (pXL, "PL"				, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			  (pXL, dWidthSp			, m_nSttRow, "L", "M", 0, RED,	 9, FALSE, TA_CENTER, "WidthSp_CP", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dThickSp			, m_nSttRow, "O", "O", 0, RED,   9, FALSE, TA_CENTER, "ThickSp_CP", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dLengthSp			, m_nSttRow, "Q", "R", 0, RED,   9, FALSE, TA_CENTER, "LengthSp_CP", 0);
	pOut->SetXL			  (pXL, "="					, m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=%s*%s",  pOut->GetCellRef("WidthSp_CP"), pOut->GetCellRef("ThickSp_CP"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "AreaSpBig_CP", 1);
	pOut->SetXL			  (pXL, "mm²"				, m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	///< 내측이음판
	pOut->SetXL			  (pXL, "2"					, m_nSttRow, "I", "I", 0, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "-"					, m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			  (pXL, "PL"				, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			  (pXL, dWidthSpSmall		, m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "WidthSpSmall_CP", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dThickSpSmall		, m_nSttRow, "O", "O", 0, RED,   9, FALSE, TA_CENTER, "ThickSpSmall_CP", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dLengthSpSmall		, m_nSttRow, "Q", "R", 0, RED,   9, FALSE, TA_CENTER, "LengthSpSmall_CP", 0);
	pOut->SetXL			  (pXL, "="					, m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=2*%s*%s",  pOut->GetCellRef("WidthSpSmall_CP"), pOut->GetCellRef("ThickSpSmall_CP"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "AreaSpSmall_CP", 1);
	pOut->SetXL			  (pXL, "mm²"				, m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	///< 합계
	pOut->SetXL			  (pXL, "Σ As ="			, m_nSttRow, "I", "I", 0);
	szTemp.Format("=%s+%s",  pOut->GetCellRef("AreaSpBig_CP"), pOut->GetCellRef("AreaSpSmall_CP"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "AreaSp_CP", 1);
	szTemp.Format("=IF(%s<%s, \"mm ²＜  Asreq'd =\", \"mm² ≥  Asreq'd =\")",  pOut->GetCellRef("AreaSp_CP"), pOut->GetCellRef("AreaReqSp_CP"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "M", "Q", 0, BLACK, 9, FALSE, TA_LEFT);
	szTemp.Format("=%s",  pOut->GetCellRef("AreaReqSp_CP"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=IF(%s<%s, \"mm², ∴ N.G\", \"mm², ∴ O.K\")",  pOut->GetCellRef("AreaSp_CP"), pOut->GetCellRef("AreaReqSp_CP"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "T", "T", 1);

	pOut->SetLineBoxText(pXL, "이음판의응력 : fs =",	m_nSttRow, "D", "H", 2, BLACK, 9, FALSE, TA_LEFT, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "",						m_nSttRow, "I", "Q", 0);

	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_CP"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_CP"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("DesignFs_CP"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM3", 1);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	m_nSttRow++;
	szTemp.Format("=%s", pOut->GetCellRef("AreaSp_CP"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "I", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM4", 2);
	m_nSttRow--;
	szTemp.Format("=ROUND(%s*%s*%s/%s,3)", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"), pOut->GetCellRef("PARAM3"), pOut->GetCellRef("PARAM4"));
	pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, "S", "U", 2, BLACK, 9, FALSE, TA_CENTER, "Sp_Stress_Cp", 3, FALSE);
	pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "V", "V", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	if(!bHybrid)
	{
		szTemp.Format("=IF(%s>%s, \">\", \"≤\")", pOut->GetCellRef("Sp_Stress_Cp"), pOut->GetCellRef("ALLOW_COMP"));
		pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, "W", "W", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	}
	//if(bHybrid)	szTemp.Format("=\"R·fsa, \"&IF(%s>%s*%s, \"N.G\", \"O.K\")", pOut->GetCellRef("Sp_Stress_Cp"), pOut->GetCellRef("ALLOW_COMP"), pOut->GetCellRef("Hy_R"));
	if(bHybrid) szTemp.Format("=IF(%s>%s*%s,\">  R·fsa  ∴ N.G\",\"≤  R·fsa   ∴ O.K\")", pOut->GetCellRef("Sp_Stress_Cp"), pOut->GetCellRef("ALLOW_COMP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=\"fsa, \"&IF(%s>%s, \"∴ N.G\", \"∴ O.K\")", pOut->GetCellRef("Sp_Stress_Cp"), pOut->GetCellRef("ALLOW_COMP"));

	pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, "X", !bHybrid ? "AA" : "AB", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	m_nSttRow+=2;

	///< ③ BOLT의 응력검토(직응력)
	pOut->SetXL			  (pXL, "③ 볼트의 응력검토"	        , m_nSttRow, "C", "C", 1);

	m_nSttRow+=9;
	pOut->SetXL			  (pXL, " Ss : 합성전 전단력"	        , m_nSttRow, "D", "H", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Ss_SP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL			  (pXL, "kN"					        , m_nSttRow, "L", "M", 0);

	pOut->SetXL			  (pXL, " Sv : 합성후 전단력"	        , m_nSttRow, "O", "S", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Sv_SP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL			  (pXL, "kN"					        , m_nSttRow, "W", "X", 1);

	pOut->SetXL			  (pXL, " Qs,Qv : 합성전 및 합성후의 중립축에 대한 접합선외측의 단면1차모멘트", m_nSttRow, "D", "D", 1);
	pOut->SetXL			  (pXL, "Qs = "							, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×"							, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×("							, m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	
	pModelCalc->GetSectionCoefficient(nG, dSta, TRUE);//슬래브 유효폭으로 수정....070223...KB...

	szTemp.Format("=%.1lf", fabs(pModelCalc->GetMapValueSecCo(bPositive?"Ysu":"Ysl")));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "dYQs_CP", 1);
	pOut->SetXL			  (pXL, "-"								, m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s/2", pOut->GetCellRef("FlangeT_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL			  (pXL, ") ="							, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s*%s*(%s-%s/2)", pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("dYQs_CP"), pOut->GetCellRef("FlangeT_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Qs_CP", 1);
	pOut->SetXL			  (pXL, "mm³"							, m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetXL			  (pXL, "Qv = "							, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×"							, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×("							, m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);

	szTemp.Format("=%.1lf", fabs(pModelCalc->GetMapValueSecCo(bPositive?"Yvsu":"Yvsl")));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "dYQv_CP", 1);
	pOut->SetXL			  (pXL, "-"								, m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s/2", pOut->GetCellRef("FlangeT_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL			  (pXL, ") ="							, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s*%s*(%s-%s/2)", pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("dYQv_CP"), pOut->GetCellRef("FlangeT_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Qv_CP", 1);
	pOut->SetXL			  (pXL, "mm³"							, m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, " Is,Iv : 합성전 및 합성후의 중립축에 대한 총단면의 단면2차모멘트" , m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "Is"						, m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",	m_nSttRow,"F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("%.0lf", pModelCalc->GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBx));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "G", "J", 0, RED,   9, FALSE, TA_CENTER, "Is_SP", 0);
	pOut->SetXL(pXL, "mm⁴,"							, m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("%.0lf", pModelCalc->GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS));
	pOut->SetXL(pXL, "Iv"						, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",	m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "P", "S", 0, RED,   9, FALSE, TA_CENTER, "Iv_SP", 0);
	pOut->SetXL(pXL, "mm⁴"							, m_nSttRow, "T", "T", 1);

	pOut->SetXL(pXL, " p : 접합선 방향의 볼트의 평균 핏치(mm) = 플랜지 전폭 / 횡방향 볼트수" , m_nSttRow, "D", "D", 0);
	m_nSttRow++;
	pOut->SetXL(pXL, "="								, m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_CP"));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "/("							, m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("TotalBoltsu_CP"));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "/"								, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	long nQtyBolt = bPositive ? pSp->m_uB+1 : pSp->m_dB+1;
	szTemp.Format("=%d", nQtyBolt);
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "S","S", 0, RED,   9, FALSE, TA_CENTER, "BoltRow_CP", 0);
	pOut->SetXL(pXL, ") ="							, m_nSttRow, "T","T",0, BLACK, 9, FALSE, TA_CENTER);

	szTemp.Format("=%s/(%s/%s)", pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("TotalBoltsu_CP"), pOut->GetCellRef("BoltRow_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "U","V", 0, BLACK, 9, FALSE, TA_CENTER, "PitchAve_CP", 1);
	pOut->SetXL			  (pXL, "mm"							, m_nSttRow, "W","W", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			  (pXL, " n : 접합선직각의 볼트의 갯수" , m_nSttRow, "D", "D", 1);
	m_nSttRow-=18;
	///< ρp =P/n
	pOut->SetXL			  (pXL, "  · 직응력"			        , m_nSttRow, "C", "C", 1);
	pOut->SetLineBoxText  (pXL, "ρp ="							, m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P"						        , m_nSttRow, "F", "F", 1);
	pOut->SetXL			  (pXL, "n"						        , m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	m_nSttRow--; ///< 분모->분자
	///< = (모재플랜지 두께×모재플랜지 폭×설계응력)/사용볼트수
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s",  pOut->GetCellRef("FlangeT_CP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetUnderLineText(pXL, "×"					        , m_nSttRow, "J", "J", 0);
	szTemp.Format("=%s",  pOut->GetCellRef("FlangeW_CP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetUnderLineText(pXL, "×"					        , m_nSttRow, "M", "M", 0);
	szTemp.Format("=%s",  pOut->GetCellRef("DesignFs_CP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "N", "O", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s",  pOut->GetCellRef("TotalBoltsu_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "EA"							, m_nSttRow, "L", "L", 0);
	m_nSttRow--; ///< 분모->분자
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s*%s*%s/%s", pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("FlangeW_CP"), pOut->GetCellRef("DesignFs_CP"), pOut->GetCellRef("TotalBoltsu_CP"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "Q", "S", 2, BLACK, 9, FALSE, TA_CENTER, "RhoP_CP", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "N/EA"						, m_nSttRow, "T", "U", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \" ＞  R·ρa ∴ N.G \", \" ≤ R·ρa ∴ O.K \")", pOut->GetCellRef("RhoP_CP"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \" ＞  ρa ∴ N.G \", \" ≤ ρa ∴ O.K \")", pOut->GetCellRef("RhoP_CP"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "V", "Z", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	m_nSttRow+=2;
	///< ③ BOLT의 응력검토(휨전단응력)
	pOut->SetXL			  (pXL, "  · 휨전단 응력"			    , m_nSttRow, "C", "C", 1);
	///< ρH
	pOut->SetLineBoxText  (pXL, "ρh ="							, m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "("								, m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Ss Qs"					        , m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetLineBoxText  (pXL, "+"								, m_nSttRow, "I", "I", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Sv Qv"					        , m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetLineBoxText  (pXL, ")"								, m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "×"							, m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "p"						        , m_nSttRow, "N", "N", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "Is"					        , m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "Iv"					        , m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "n"						        , m_nSttRow, "N", "N", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);			//next

	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "("								, m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s*1000", pOut->GetCellRef("Ss_SP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "PH_SS", 1);
	pOut->SetUnderLineText(pXL, "×"							, m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Qs_CP"));
	pOut->SetUnderLineText(pXL, szTemp							, m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PH_QS", 1);
	pOut->SetLineBoxText  (pXL, "+"								, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s*1000", pOut->GetCellRef("Sv_SP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PH_SV", 1);
	pOut->SetUnderLineText(pXL, "×"							, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Qv_CP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "PH_QV", 1);
	pOut->SetLineBoxText  (pXL, ")"								, m_nSttRow, "V", "V", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "×"							, m_nSttRow, "W", "W", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("PitchAve_CP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "PH_P", 1);
	pOut->SetUnderLineText(pXL, "mm"						    , m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	m_nSttRow++;

	szTemp.Format("=%s", pOut->GetCellRef("Is_SP"));
	pOut->SetXL			  (pXL, szTemp					        , m_nSttRow, "G", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PH_IS", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Iv_SP"));
	pOut->SetXL			  (pXL, szTemp					        , m_nSttRow, "O", "U", 0, BLACK, 9, FALSE, TA_CENTER, "PH_IV", 0);
	szTemp.Format("=%s", pOut->GetCellRef("BoltRow_CP"));
	pOut->SetXL			  (pXL, szTemp					        , m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "PH_N", 0);
	pOut->SetXL			  (pXL, "EA"					        , m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);

	m_nSttRow++;
	pOut->SetXL  (pXL, " ="										, m_nSttRow, "E","E",0, BLACK, 9, FALSE, TA_CENTER, "");

	szTemp.Format("=(%s*%s/%s+%s*%s/%s)*%s/%s", pOut->GetCellRef("PH_SS"), pOut->GetCellRef("PH_QS"), pOut->GetCellRef("PH_IS"),
														  pOut->GetCellRef("PH_SV"), pOut->GetCellRef("PH_QV"), pOut->GetCellRef("PH_IV"), 
														  pOut->GetCellRef("PH_P"), pOut->GetCellRef("PH_N"));	
	pOut->SetXL(pXL, szTemp									    , m_nSttRow, "F","H",0, BLACK, 9, FALSE, TA_CENTER, "RhoH_CP", 2);
	pOut->SetXL(pXL, "N/EA"								    , m_nSttRow, "I","J",0, BLACK, 9, FALSE, TA_CENTER, "");
	
	m_nSttRow+=10;

	pOut->SetXL			(pXL, "  · 합성응력의 검토"			, m_nSttRow, "C", "C", 1);			 
	pOut->SetXL			(pXL, " ρ = √(ρp²＋ ρh²)  =  √(" , m_nSttRow, "D", "K", 0);			 
	szTemp.Format("=%s", pOut->GetCellRef("RhoP_CP"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);			 			 
	pOut->SetXL			(pXL, "²"							    , m_nSttRow, "O", "O", 0);			 
	pOut->SetXL			(pXL, "＋"							    , m_nSttRow, "P", "P", 0);			 
	szTemp.Format("=%s", pOut->GetCellRef("RhoH_CP"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);			 
	pOut->SetXL			(pXL, "²"							    , m_nSttRow, "T", "T", 0);			 
	pOut->SetXL			(pXL, ")"							   , m_nSttRow, "U", "V", 1);
	pOut->SetXL			(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=SQRT(%s^2+%s^2)", pOut->GetCellRef("RhoP_CP"), pOut->GetCellRef("RhoH_CP"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "CompStress_CP", 1);			 
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"N/EA  ＞  R·ρa , ∴ N.G\",\"N/EA  ≤  R·ρa , ∴ O.K\" ",  pOut->GetCellRef("CompStress_CP"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"N/EA  ＞  ρa , ∴ N.G\",\"N/EA  ≤  ρa , ∴ O.K\" ",  pOut->GetCellRef("CompStress_CP"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "H", "H", 0);
	
	m_nSttRow+=2;
}


void CXLSpliceCheck::SpliceCheckTensionPart(CPlateBasicIndex* pBx)
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CPlateGirderApp     *pGir           = pBx->GetGirder();
	CPlateSplice        *pSp            = pBx->GetSplice();
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CAPlateDrawPyung	DrawPyung(pDataManage);

	long   nG        = pBx->GetNumberGirder();
	double dSta      = pBx->GetStation();
	BOOL   bHybrid	 = pBx->IsHybrid();
	BOOL   bPositive = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);
	long   nBujeMark = bPositive ? G_F_L : G_F_U;	///< 압축부 부재마크
	CString szTemp = "";
	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	pOut->SetXL(pXL, " (2) 인장부 플랜지"  , m_nSttRow, "B", "F", 0);
	if(bPositive)	pOut->SetXL(pXL, "(하부 플랜지의 이음)", m_nSttRow, "H", "M", 5);
	else			pOut->SetXL(pXL, "(상부 플랜지의 이음)", m_nSttRow, "H", "M", 5);

	pCalcGeneral->GetMaterial(pBx, nBujeMark, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowTension(pSteel);
	CHGAllowStress	AllowShear(pSteel);

	///< 도면 그림 삽입
	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(20);
	if(bPositive)	DrawPyung.DrawLowerFlangeSplice(&Dom, pBx, 0, 0, false, true);
	else			DrawPyung.DrawUpperFlangeSplice(&Dom, pBx, 0, 0, false, true);
	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 2, 11, FALSE, FALSE);
	SetLines(NEXT);

	double dThickFlange = pGir->GetThickFactByStation(dSta, nBujeMark);
	double dWidthFlange = pGir->GetWidthOnStation(dSta, bPositive?FALSE:TRUE);
	
	
	double dAllowTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dThickFlange);
	double dAllowShear		= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, dThickFlange);
	// 필러 존재시 인접플랜지 두께 ///////////////////////////////////////////
	double dThickNear	= dThickFlange;
	BOOL bNextFiller	= TRUE;
	if(pBx->IsFiller(bPositive ? BX_LOWERFLANGE : BX_UPPERFLANGE, bNextFiller))
		dThickNear = pBx->GetFactChain(nBujeMark, TRUE)->m_dFactChainThick;
	//////////////////////////////////////////////////////////////////////////

	pOut->SetXL(pXL, "사용 강종    :",			m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, strLeft,					m_nSttRow, "U", "W", 0, RED,   9, FALSE, TA_LEFT, "STEELTYPE_TENSION", 0);
	pOut->SetXL(pXL, strRight,					m_nSttRow, "X", "X", 1, RED);
	if(dThickFlange!=dThickNear)
	{
		pOut->SetXL(pXL, "인접플랜지 두께   =",		m_nSttRow, "Q", "Q", 0);
		pOut->SetXL(pXL, dThickNear,				m_nSttRow, "V", "X", 0, RED,   9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, "mm",						m_nSttRow, "Y", "Y", 1);
	}
	if(bPositive)	pOut->SetXL(pXL, "모재두께     : Tl =",		m_nSttRow, "Q", "Q", 0);
	else			pOut->SetXL(pXL, "모재두께     : Tu =",		m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dThickFlange,				m_nSttRow, "V", "X", 0, RED,   9, FALSE, TA_CENTER, "FlangeT_TS", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "Y", "Y", 1);
	pOut->SetXL(pXL, "모재길이     :  L =",		m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dWidthFlange,				m_nSttRow, "V", "X", 0, RED,   9, FALSE, TA_CENTER, "FlangeW_TS", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "Y", "Y", 1);
	pOut->SetXL(pXL, "허용압축응력 : fa =",		m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dAllowTension,				m_nSttRow, "V", "X", 0, RED, 9, FALSE, TA_CENTER, "ALLOW_TENSION", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 1);
	pOut->SetXL(pXL, "허용전단응력 : υa =",	m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dAllowShear,				m_nSttRow, "V", "X", 0, RED, 9, FALSE, TA_CENTER, "ALLOW_SHEAR", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
	m_nSttRow-=9;
	if(dThickFlange!=dThickNear)
		m_nSttRow--;

	pOut->SetXL(pXL, "fs ="                         , m_nSttRow, "C", "D", 0);
	szTemp.Format("=IF((%s+%s)>0,ABS(%s),ABS(%s))", pOut->GetCellRef("Ms_SP"), pOut->GetCellRef("Mv_SP"), pOut->GetCellRef("fl_SP"), pOut->GetCellRef("fu_SP"));             
	pOut->SetXL(pXL, szTemp                         , m_nSttRow, "E", "I", 0, BLACK, 9, FALSE, TA_CENTER, "fs_TS", 1);
	pOut->SetXL(pXL, "MPa"                     , m_nSttRow, "J", "J", 1);

	pOut->SetXL(pXL, "fs' ="                        , m_nSttRow, "C", "D", 0);
	szTemp.Format("=%s", pOut->GetCellRef("0.75_SP"));
	pOut->SetXL(pXL,szTemp                          , m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, "×"                           , m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("ALLOW_TENSION"));
	pOut->SetXL(pXL, szTemp                         , m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	
	if(bHybrid)
	{
		pOut->SetXL(pXL, "×"                           , m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		szTemp.Format("=%s", pOut->GetCellRef("Hy_R"));
		pOut->SetXL(pXL, szTemp                         , m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	}

	pOut->SetXL(pXL, "=", m_nSttRow, !bHybrid ? "K" : "M", !bHybrid ? "K" : "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);

	if(!bHybrid)
		szTemp.Format("=%s*%s", pOut->GetCellRef("0.75_SP"), pOut->GetCellRef("ALLOW_TENSION"));
	else
		szTemp.Format("=%s*%s*%s", pOut->GetCellRef("0.75_SP"), pOut->GetCellRef("ALLOW_TENSION"), pOut->GetCellRef("Hy_R"));

	pOut->SetXL(pXL, szTemp                         , m_nSttRow, !bHybrid ? "K" : "N", !bHybrid ? "L" : "O", 0, BLACK, 9, FALSE, TA_CENTER, "fs'_SP2", 1);

	pOut->SetXL(pXL, "MPa"                     , m_nSttRow, !bHybrid ? "M" : "P" , !bHybrid ? "N" : "Q", 0);

	pOut->SetXL(pXL, "( ☞ 도.설 3.5.1.1 )"    , m_nSttRow, !bHybrid ?  "P" : "R", !bHybrid ? "P" : "R", 1);

	pOut->SetXL(pXL, "∴ 이음부의 설계응력(fl)는 ", m_nSttRow, "B", "H", 0);
	szTemp.Format("=MAX(%s,%s)", pOut->GetCellRef("fs_TS"), pOut->GetCellRef("fs'_SP2"));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "DesignFs_TS", 1);
	pOut->SetXL(pXL, "MPa을 적용한다."         , m_nSttRow, "K", "K", 9);
	if(dThickFlange!=dThickNear)
		m_nSttRow++;

	pOut->SetXL(pXL, "① 필요볼트수의 결정"         , m_nSttRow, "C", "C", 1);
	pOut->SetLineBoxText  (pXL, "n="				, m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetUnderLineText(pXL, "As × fs"			, m_nSttRow, "E", "G", 0);
	pOut->SetLineBoxText  (pXL, "="					, m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "J", "J", 0);

	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_TS"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "M", "M", 0);
	
	szTemp.Format("=%s", pOut->GetCellRef("DesignFs_TS"));
	pOut->SetUnderLineText(pXL, szTemp   			, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetLineBoxText  (pXL, "≒"				, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	
	m_nSttRow++;
	if(bHybrid)
	{
		pOut->SetXL		  (pXL, "R × ρa"			, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		szTemp.Format("=%s", pOut->GetCellRef("Hy_R"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Hy_R_temp", 3);
		pOut->SetXL		  (pXL, "×"				, m_nSttRow, "L", "L", 0);
		szTemp.Format("=%s", pOut->GetCellRef("faBoltTotal_SP"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/(%s*%s),0)", pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("DesignFs_TS"),
		pOut->GetCellRef("Hy_R_temp"), pOut->GetCellRef("faBoltTotal_SP"));
	}
	else
	{
		pOut->SetXL			  (pXL, "ρa"				, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		szTemp.Format("=%s", pOut->GetCellRef("faBoltTotal_SP"));
		pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/%s,0)", pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("DesignFs_TS"), pOut->GetCellRef("faBoltTotal_SP"));
	}	
	
	pOut->SetLineBoxText  (pXL, szTemp				, m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "EA"				, m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetLineBoxText  (pXL, "⇒"				, m_nSttRow, "S", "S", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetLineBoxText  (pXL, "n"					, m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	pOut->SetLineBoxText  (pXL, "="					, m_nSttRow, "U", "U", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	long nBoltsu = bPositive ? (pSp->m_dB+1)*2 * (pSp->m_dE+1) : (pSp->m_uB+1)*2 * (pSp->m_uE+1);
	pOut->SetLineBoxText  (pXL, double(nBoltsu)		, m_nSttRow, "V", "V", 2, RED,	 9, FALSE, TA_CENTER, "TotalBoltsu_TS", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "EA사용"			, m_nSttRow, "W", "X", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	m_nSttRow++;
	m_nSttRow++;
	
	pOut->SetXL(pXL, "② 이음판의 결정"		        , m_nSttRow, "C", "C", 1);
	pOut->SetLineBoxText  (pXL, "필요단면적 : As ="	, m_nSttRow, "D", "H", 2, BLACK, 9, FALSE, TA_LEFT,   "", 3, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "J", "J", 0);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_TS"));
	pOut->SetUnderLineText(pXL, szTemp				, m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetUnderLineText(pXL, "×"				, m_nSttRow, "M", "M", 0);
	szTemp.Format("=%s", pOut->GetCellRef("DesignFs_TS"));
	pOut->SetUnderLineText(pXL, szTemp   			, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetLineBoxText  (pXL, "="					, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	m_nSttRow++;
	if(bHybrid)
	{
		szTemp.Format("=%s", pOut->GetCellRef("Hy_R"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Hy_R_temp", 3);
		pOut->SetXL		  (pXL, "×"				, m_nSttRow, "L", "L", 0);

		szTemp.Format("=%s", pOut->GetCellRef("ALLOW_TENSION"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/(%s*%s),0)", pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("DesignFs_TS"),
		pOut->GetCellRef("Hy_R_temp"), pOut->GetCellRef("ALLOW_TENSION"));
	}
	else
	{
		szTemp.Format("=%s", pOut->GetCellRef("ALLOW_TENSION"));
		pOut->SetXL		  (pXL, szTemp				, m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		m_nSttRow--;
		szTemp.Format("=ROUNDUP(%s*%s*%s/%s,0)", pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("DesignFs_TS"), pOut->GetCellRef("ALLOW_TENSION"));
	}
	pOut->SetLineBoxText  (pXL, szTemp				, m_nSttRow, "Q", "R", 2, BLACK, 9, FALSE, TA_CENTER, "AreaReqSp_TS", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "mm²"				, m_nSttRow, "S", "S", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);

	m_nSttRow++;
	m_nSttRow++;

	double dWidthSp       = pGir->GetWidthOnStation(dSta, !bPositive);
	double dThickSp       = bPositive ? pSp->GetdT() : pSp->GetuT();
	double dLengthSp      = bPositive ? (pSp->m_dA*2+(pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1) : (pSp->m_uA*2+(pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1);
	double dWidthSpSmall  = bPositive ? pSp->m_dW : pSp->m_uW;
	double dThickSpSmall  = bPositive ? pSp->m_dT : pSp->m_uT;
	double dLengthSpSmall = bPositive ? (pSp->m_dA*2+(pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1) : (pSp->m_uA*2+(pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1);
	///< 외측이음판
	pOut->SetXL			  (pXL, "사용이음판 : As =" , m_nSttRow, "D", "H", 0);
	pOut->SetXL			  (pXL, "1"					, m_nSttRow, "I", "I", 0, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "-"					, m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			  (pXL, "PL"				, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			  (pXL, dWidthSp			, m_nSttRow, "L", "M", 0, RED,	 9, FALSE, TA_CENTER, "WidthSp_TS", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dThickSp			, m_nSttRow, "O", "O", 0, RED,   9, FALSE, TA_CENTER, "ThickSp_TS", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dLengthSp			, m_nSttRow, "Q", "R", 0, RED,   9, FALSE, TA_CENTER, "LengthSp_TS", 0);
	pOut->SetXL			  (pXL, "="					, m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=%s*%s",  pOut->GetCellRef("WidthSp_TS"), pOut->GetCellRef("ThickSp_TS"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "AreaSpBig_TS", 1);
	pOut->SetXL			  (pXL, "mm²"				, m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	///< 내측이음판
	pOut->SetXL			  (pXL, "2"					, m_nSttRow, "I", "I", 0, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "-"					, m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			  (pXL, "PL"				, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			  (pXL, dWidthSpSmall		, m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "WidthSpSmall_TS", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dThickSpSmall		, m_nSttRow, "O", "O", 0, RED,   9, FALSE, TA_CENTER, "ThickSpSmall_TS", 0);
	pOut->SetXL			  (pXL, "×"				, m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, dLengthSpSmall		, m_nSttRow, "Q", "R", 0, RED,   9, FALSE, TA_CENTER, "LengthSpSmall_TS", 0);
	pOut->SetXL			  (pXL, "="					, m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=2*%s*%s",  pOut->GetCellRef("WidthSpSmall_TS"), pOut->GetCellRef("ThickSpSmall_TS"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "AreaSpSmall_TS", 1);
	pOut->SetXL			  (pXL, "mm²"				, m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	///< 합계
	pOut->SetXL			  (pXL, "Σ="				, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s+%s",  pOut->GetCellRef("AreaSpBig_TS"), pOut->GetCellRef("AreaSpSmall_TS"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "AreaSp_TS", 1);
	szTemp.Format("=IF(%s<%s, \"mm ²＜  Asreq'd =\", \"mm² ≥  Asreq'd =\")",  pOut->GetCellRef("AreaSp_TS"), pOut->GetCellRef("AreaReqSp_TS"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "M", "Q", 0, BLACK, 9, FALSE, TA_LEFT);
	szTemp.Format("=%s",  pOut->GetCellRef("AreaReqSp_TS"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=IF(%s<%s, \"mm², ∴ N.G\", \"mm², ∴ O.K\")",  pOut->GetCellRef("AreaSp_TS"), pOut->GetCellRef("AreaReqSp_TS"));
	pOut->SetXL			  (pXL, szTemp				, m_nSttRow, "T", "T", 1);
	
	m_nSttRow+=17;
	pOut->SetXL			  (pXL, " Ss : 합성전 전단력"	        , m_nSttRow, "D", "H", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Ss_SP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL			  (pXL, "kN"					        , m_nSttRow, "L", "M", 0);

	pOut->SetXL			  (pXL, " Sv : 합성후 전단력"	        , m_nSttRow, "O", "S", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Sv_SP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL			  (pXL, "kN"					        , m_nSttRow, "W", "X", 1);

	pOut->SetXL			  (pXL, " Qs,Qv : 합성전 및 합성후의 중립축에 대한 접합선외측의 단면1차모멘트", m_nSttRow, "D", "D", 1);
	pOut->SetXL			  (pXL, "Qs = "							, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×"							, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×("							, m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	
	CString sGetCellRef	= "";
	sGetCellRef.Format("V%d", m_nSttRow+22);

	szTemp.Format("=(%s+%s+%s)-%s", sGetCellRef, pOut->GetCellRef("FlangeT_CP"),pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("dYQs_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "M", "N", 0, BLACK,   9, FALSE, TA_CENTER, "dYQs_TS", 1);
	pOut->SetXL			  (pXL, "-"								, m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s/2", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL			  (pXL, ") ="							, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s*%s*(%s-%s/2)", pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("dYQs_TS"), pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Qs_TS", 1);
	pOut->SetXL			  (pXL, "mm³"							, m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetXL			  (pXL, "Qv = "							, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×"							, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×("							, m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	
	szTemp.Format("=(%s+%s+%s)-%s", sGetCellRef, pOut->GetCellRef("FlangeT_CP"),pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("dYQv_CP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "M", "N", 0, BLACK,   9, FALSE, TA_CENTER, "dYQv_TS", 1);
	pOut->SetXL			  (pXL, "-"								, m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s/2", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL			  (pXL, ") ="							, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s*%s*(%s-%s/2)", pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("dYQv_TS"), pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Qv_TS", 1);
	pOut->SetXL			  (pXL, "mm³"							, m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, " Is,Iv : 합성전 및 합성후의 중립축에 대한 총단면의 단면2차모멘트" , m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "Is"							, m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",	m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("Is_SP"));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "mm⁴,"							, m_nSttRow, "K", "K", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Iv_SP"));
	pOut->SetXL(pXL, "Iv"						, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",	m_nSttRow, "O", "O", 0, BLACK, 9);
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "mm⁴"							, m_nSttRow, "T", "T", 1);

	pOut->SetXL(pXL, " p : 접합선 방향의 볼트의 평균 핏치(mm) = 플랜지 전폭 / 횡방향 볼트수" , m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "="								, m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_TS"));
	pOut->SetXL(pXL, szTemp, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "/("							, m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("TotalBoltsu_TS"));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "BoltRow_TS_Total", 0);
	pOut->SetXL(pXL, "/"								, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	long nQtyBolt = bPositive ? pSp->m_dB+1 : pSp->m_uB+1;
	szTemp.Format("=%d", nQtyBolt);
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "S","S", 0, RED,   9, FALSE, TA_CENTER, "BoltRow_TS", 0);
	pOut->SetXL(pXL, ") ="							, m_nSttRow, "T","T",0, BLACK, 9, FALSE, TA_CENTER);

	szTemp.Format("=%s/(%s/%s)", pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("TotalBoltsu_TS"), pOut->GetCellRef("BoltRow_TS"));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "U","V", 0, BLACK, 9, FALSE, TA_CENTER, "PitchAve_TS", 1);
	pOut->SetXL(pXL, "mm"							, m_nSttRow, "W","W", 1);

	pOut->SetXL			  (pXL, " n : 접합선직각의 볼트의 갯수" , m_nSttRow, "D", "D", 1);
	m_nSttRow-=26;
	pOut->SetXL(pXL, "이음판의 순단면적 : 인장부의 순단면적은 볼트지름을공제", m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "An",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("AreaSp_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "AreaSp_TS1", 1);
	pOut->SetXL(pXL, "-",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=IF(%s<27, %s+3,%s+4 )", pOut->GetCellRef("BoltSize"),pOut->GetCellRef("BoltSize"),pOut->GetCellRef("BoltSize"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "BoltSize1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("ThickSp_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "ThickSp_TS1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s/%s", pOut->GetCellRef("TotalBoltsu_TS"), pOut->GetCellRef("BoltRow_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "TotalBolt", 0);
	pOut->SetXL(pXL, "×",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2.0,							m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "EA",							m_nSttRow, "S", "S", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=ROUND(%s-%s*%s*%s*2,3)", pOut->GetCellRef("AreaSp_TS1"), pOut->GetCellRef("BoltSize1"), pOut->GetCellRef("ThickSp_TS1"), pOut->GetCellRef("TotalBolt"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "AN", 0);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "I", "I", 0);
	szTemp.Format("=IF(%s<%s, \"<\", \"≥\")", pOut->GetCellRef("AN"), pOut->GetCellRef("AreaReqSp_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Asreq'd",						m_nSttRow, "L", "M", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("AreaReqSp_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "mm²",						m_nSttRow, "Q", "Q", 0);

	szTemp.Format("=IF(%s<%s, \"∴ N.G\", \"∴ O.K\")", pOut->GetCellRef("AN"), pOut->GetCellRef("AreaReqSp_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "T", "T", 1);

	pOut->SetLineBoxText(pXL, "이음판의응력 : fs =",	m_nSttRow, "D", "H", 2, BLACK, 9, FALSE, TA_LEFT, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "",						m_nSttRow, "I", "Q", 0);

	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "SP_STRESS_PARAM1", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "SP_STRESS_PARAM2", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("DesignFs_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "SP_STRESS_PARAM3", 1);
	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	m_nSttRow++;
	szTemp.Format("=%s", pOut->GetCellRef("AN"));
	pOut->SetXL(pXL, szTemp,							m_nSttRow, "I", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "SP_STRESS_PARAM4", 2);
	m_nSttRow--;
	szTemp.Format("=ROUND(%s*%s*%s/%s,3)", pOut->GetCellRef("SP_STRESS_PARAM1"), pOut->GetCellRef("SP_STRESS_PARAM2"), pOut->GetCellRef("SP_STRESS_PARAM3"), pOut->GetCellRef("SP_STRESS_PARAM4"));
	pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, "S", "U", 2, BLACK, 9, FALSE, TA_CENTER, "Sp_Stress_TS", 3, FALSE);
	pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "V", "V", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	if(!bHybrid)
	{
		szTemp.Format("=IF(%s>%s, \">\", \"≤\")", pOut->GetCellRef("Sp_Stress_TS"), pOut->GetCellRef("ALLOW_TENSION"));
		pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, "W", "W", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	}
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s,\">  R·fsa  ∴ N.G\",\"≤  R·fsa   ∴ O.K\")", pOut->GetCellRef("Sp_Stress_TS"), pOut->GetCellRef("ALLOW_TENSION"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=\"fsa, \"&IF(%s>%s, \"∴ N.G\", \"∴ O.K\")", pOut->GetCellRef("Sp_Stress_TS"), pOut->GetCellRef("ALLOW_TENSION"));
	
	pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, !bHybrid ? "X" : "W",  "AA", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	m_nSttRow+=3;
	///< ③ BOLT의 응력검토(직응력)
	pOut->SetXL			  (pXL, "③ 볼트의 응력검토"	        , m_nSttRow, "C", "C", 1);
	///< ρp =P/n
	pOut->SetXL			  (pXL, "  · 직응력"			        , m_nSttRow, "C", "C", 1);
	pOut->SetLineBoxText  (pXL, "ρp ="							, m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P"						        , m_nSttRow, "F", "F", 1);
	pOut->SetXL			  (pXL, "n"						        , m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	m_nSttRow--; ///< 분모->분자
	///< = (모재플랜지 두께×모재플랜지 폭×설계응력)/사용볼트수
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s",  pOut->GetCellRef("FlangeT_TS"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetUnderLineText(pXL, "×"					        , m_nSttRow, "J", "J", 0);
	szTemp.Format("=%s",  pOut->GetCellRef("FlangeW_TS"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetUnderLineText(pXL, "×"					        , m_nSttRow, "M", "M", 0);
	szTemp.Format("=%s",  pOut->GetCellRef("DesignFs_TS"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "N", "O", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s",  pOut->GetCellRef("TotalBoltsu_TS"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "EA"							, m_nSttRow, "L", "L", 0);
	m_nSttRow--; ///< 분모->분자
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s*%s*%s/%s", pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("FlangeW_TS"), pOut->GetCellRef("DesignFs_TS"), pOut->GetCellRef("TotalBoltsu_TS"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "Q", "S", 2, BLACK, 9, FALSE, TA_CENTER, "RhoP_TS", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "N/EA"						, m_nSttRow, "T", "U", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \" ＞  R·ρa ∴ N.G \", \" ≤ R·ρa ∴ O.K \")", pOut->GetCellRef("RhoP_TS"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \" ＞  ρa ∴ N.G \", \" ≤ ρa ∴ O.K \")", pOut->GetCellRef("RhoP_TS"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "V", "Z", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	m_nSttRow+=2;
	///< ③ BOLT의 응력검토(휨전단응력)
	pOut->SetXL			  (pXL, "  · 휨전단 응력"			    , m_nSttRow, "C", "C", 1);
	///< ρH
	pOut->SetLineBoxText  (pXL, "ρh ="							, m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "("								, m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Ss Qs"					        , m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetLineBoxText  (pXL, "+"								, m_nSttRow, "I", "I", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Sv Qv"					        , m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetLineBoxText  (pXL, ")"								, m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "×"							, m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "p"						        , m_nSttRow, "N", "N", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "Is"					        , m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "Iv"					        , m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL			  (pXL, "n"						        , m_nSttRow, "N", "N", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);			//next

	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "("								, m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s*1000", pOut->GetCellRef("Ss_SP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "PH_SS", 1);
	pOut->SetUnderLineText(pXL, "×"							, m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Qs_TS"));
	pOut->SetUnderLineText(pXL, szTemp							, m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PH_QS", 1);
	pOut->SetLineBoxText  (pXL, "+"								, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s*1000", pOut->GetCellRef("Sv_SP"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PH_SV", 1);
	pOut->SetUnderLineText(pXL, "×"							, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Qv_TS"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "PH_QV", 1);
	pOut->SetLineBoxText  (pXL, ")"								, m_nSttRow, "V", "V", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "×"							, m_nSttRow, "W", "W", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("PitchAve_TS"));
	pOut->SetUnderLineText(pXL, szTemp					        , m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "PH_P", 1);
	pOut->SetUnderLineText(pXL, "mm"						    , m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	m_nSttRow++;

	szTemp.Format("=%s", pOut->GetCellRef("Is_SP"));
	pOut->SetXL			  (pXL, szTemp					        , m_nSttRow, "G", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PH_IS", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Iv_SP"));
	pOut->SetXL			  (pXL, szTemp					        , m_nSttRow, "O", "U", 0, BLACK, 9, FALSE, TA_CENTER, "PH_IV", 0);
	szTemp.Format("=%s", pOut->GetCellRef("BoltRow_TS"));
	pOut->SetXL			  (pXL, szTemp					        , m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "PH_N", 0);
	pOut->SetXL			  (pXL, "EA"					        , m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);

	m_nSttRow++;
	pOut->SetXL  (pXL, " ="										, m_nSttRow, "E","E",0, BLACK, 9, FALSE, TA_CENTER, "");
	
	szTemp.Format("=(%s*%s/%s+%s*%s/%s)*%s/%s", pOut->GetCellRef("PH_SS"), pOut->GetCellRef("PH_QS"), pOut->GetCellRef("PH_IS"),
														  pOut->GetCellRef("PH_SV"), pOut->GetCellRef("PH_QV"), pOut->GetCellRef("PH_IV"), 
														  pOut->GetCellRef("PH_P"), pOut->GetCellRef("PH_N"));
	pOut->SetXL(pXL, szTemp							    , m_nSttRow, "F","H", 0, BLACK, 9, FALSE, TA_CENTER, "RhoH_TS", 2);
	pOut->SetXL(pXL, "N/EA"						    , m_nSttRow, "I","I", 0);
	
	m_nSttRow+=10;

	pOut->SetXL			(pXL, "  · 합성응력의 검토"			, m_nSttRow, "C", "C", 1);			 
	pOut->SetXL			(pXL, " ρ = √(ρp²＋ ρh²)  =  √(" , m_nSttRow, "D", "K", 0);			 
	szTemp.Format("=%s", pOut->GetCellRef("RhoP_TS"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);			 			 
	pOut->SetXL			(pXL, "²"							    , m_nSttRow, "O", "O", 0);			 
	pOut->SetXL			(pXL, "＋"							    , m_nSttRow, "P", "P", 0);			 

	szTemp.Format("=%s", pOut->GetCellRef("RhoH_TS"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);			 
	pOut->SetXL			(pXL, "²)"							    , m_nSttRow, "T", "T", 1);
	pOut->SetXL			(pXL, "="							    , m_nSttRow, "E", "E", 0);
	szTemp.Format("=SQRT(%s^2+%s^2)", pOut->GetCellRef("RhoP_TS"), pOut->GetCellRef("RhoH_TS"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "CompStress_TS", 1);
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"N/EA  ＞  R·ρa , ∴ N.G\",\"N/EA  ≤  R·ρa , ∴ O.K\" ",  pOut->GetCellRef("CompStress_TS"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"N/EA  ＞  ρa , ∴ N.G\",\"N/EA  ≤  ρa , ∴ O.K\" ",  pOut->GetCellRef("CompStress_TS"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetXL			(pXL, szTemp						    , m_nSttRow, "I", "I",2);	
	pOut->SetXL(pXL, "④ 모재의 응력검토",						m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "플랜지의 총단면적 : As =",				m_nSttRow, "D", "D", 0);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeT_TS"));
	pOut->SetXL(pXL, szTemp,									m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "StressCheck_Param1", 1);
	pOut->SetXL(pXL, "×",										m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("FlangeW_TS"));
	pOut->SetXL(pXL, szTemp,									m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "StressCheck_Param2", 1);
	pOut->SetXL(pXL, "=",										m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s*%s", pOut->GetCellRef("StressCheck_Param1"), pOut->GetCellRef("StressCheck_Param2"));
	pOut->SetXL(pXL, szTemp,									m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "FLANGE_TOT_AS", 1);
	pOut->SetXL(pXL, "mm²",									m_nSttRow, "T", "T", 1);
	pOut->SetXL(pXL, "볼트 구멍을 공제한 플랜지의 순단면적 : Asn =",m_nSttRow, "D", "D", 0);
	
//	=J110&" ×( "&M110&" - "&(G11+3)&" × ("&Q104&" / "&S104&")) ="

	szTemp.Format("=%s&\" ×( \"&%s&\" - \"&IF(%s<27,%s+3,%s+4)&\" × (\"&%s&\" / \"&%s&\")) =\"", pOut->GetCellRef("StressCheck_Param1"), pOut->GetCellRef("StressCheck_Param2"),
				pOut->GetCellRef("BoltSize"),pOut->GetCellRef("BoltSize"),pOut->GetCellRef("BoltSize"), pOut->GetCellRef("BoltRow_TS_Total"), pOut->GetCellRef("BoltRow_TS"));
	pOut->SetXL(pXL, szTemp,									m_nSttRow, "O", "O", 0);
	szTemp.Format("=ROUND(%s*(%s-IF(%s<27,%s+3,%s+4)*%s/%s),3)", pOut->GetCellRef("StressCheck_Param1"), pOut->GetCellRef("StressCheck_Param2"), pOut->GetCellRef("BoltSize"),pOut->GetCellRef("BoltSize"),pOut->GetCellRef("BoltSize"), pOut->GetCellRef("BoltRow_TS_Total"), pOut->GetCellRef("BoltRow_TS"));
	pOut->SetXL(pXL, szTemp,									m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "FLANGE_AS", 1);
	pOut->SetXL(pXL, "mm²",									m_nSttRow, "Z", "Z", 1);

	pOut->SetLineBoxText(pXL, "모재의응력 : fl =",	m_nSttRow, "D", "H", 2, BLACK, 9, FALSE, TA_LEFT, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "",						m_nSttRow, "I", "N", 0);

	szTemp.Format("=%s", pOut->GetCellRef("FLANGE_TOT_AS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "StressCheck_Param3", 1);
	pOut->SetXL(pXL, "×",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("DesignFs_TS"));
	pOut->SetXL(pXL, szTemp,						m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "StressCheck_Param4", 1);

	pOut->SetLineBoxText(pXL, "=",					m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	m_nSttRow++;
	szTemp.Format("=%s", pOut->GetCellRef("FLANGE_AS"));
	pOut->SetXL(pXL, szTemp,							m_nSttRow, "I", "N", 0, BLACK, 9, FALSE, TA_CENTER, "StressCheck_Param5", 2);
	m_nSttRow--;
	szTemp.Format("=ROUND(%s*%s/%s,3)", pOut->GetCellRef("StressCheck_Param3"), pOut->GetCellRef("StressCheck_Param4"), pOut->GetCellRef("StressCheck_Param5"));
	pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, "P", "R", 2, BLACK, 9, FALSE, TA_CENTER, "Sp_Stress_Ts", 3, FALSE);
	pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	if(!bHybrid)
	{
		szTemp.Format("=IF(%s>%s, \">\", \"≤\")", pOut->GetCellRef("Sp_Stress_Ts"), pOut->GetCellRef("ALLOW_TENSION"));
		pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, "U", "U", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	}

	if(bHybrid)	szTemp.Format("=IF(%s>ROUND(%s*%s,3),\">  R·fsa  ∴ N.G\",\"≤  R·fsa   ∴ O.K\")", pOut->GetCellRef("Sp_Stress_Ts"), pOut->GetCellRef("ALLOW_TENSION"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=\"fsa, \"&IF(%s>ROUND(%s,3), \"∴ N.G\", \"∴ O.K\")", pOut->GetCellRef("Sp_Stress_Ts"), pOut->GetCellRef("ALLOW_TENSION"));

	pOut->SetLineBoxText(pXL, szTemp,				m_nSttRow, !bHybrid ? "V" : "U", "Y" , 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	m_nSttRow+=3;
}

void CXLSpliceCheck::SpliceCheckWebPart1(CPlateBasicIndex* pBx)
{
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CPlateGirderApp     *pGir           = pBx->GetGirder();
	CPlateSplice        *pSp            = pBx->GetSplice();
	
	long   nG        = pBx->GetNumberGirder();
	double dSta      = pBx->GetStation();
	BOOL	bHybrid	 = pBx->IsHybrid();
	CString szTemp = "";

	pOut->SetXL(pXL, " (3) WEB 연결부의 계산"		, m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, " ① 볼트의 응력계산"			, m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "  · 휨에 의한 볼트의 응력"	, m_nSttRow, "C", "C", 2);

	///< 도면 그림 삽입
	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(20);
	SpliceWebPlanStree(&Dom, pBx, pGir->GetWidthOnStation(dSta, !pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta)));
	long nRowTemp = m_nSttRow;
	long nRowTemp1= m_nSttRow;
	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 1, 15, TRUE, FALSE);	
	nRowTemp = m_nSttRow-nRowTemp;
	m_nSttRow -= nRowTemp;
	SetLines(2);
	pOut->SetXL(pXL, "거더총높이"					, m_nSttRow, "U", "W", 0);
	szTemp.Format("=%.1lf", pGir->GetHeightGirderByStaAng(dSta));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "X", "Y", 0, RED,  9, FALSE, TA_CENTER, "WEB_H", 1);
	pOut->SetXL(pXL, "mm"							, m_nSttRow, "Z", "Z", 1, BLACK,9, FALSE, TA_CENTER, "", 0);
	
	pOut->SetXL(pXL, "복부판두께"					, m_nSttRow, "U", "W", 0);
	szTemp.Format("=%.1lf", pGir->GetThickFactByStation(dSta, G_W));
	pOut->SetXL(pXL, szTemp							, m_nSttRow, "X", "Y", 0, RED,  9, FALSE, TA_CENTER, "WEB_T", 1);
	pOut->SetXL(pXL, "mm"							, m_nSttRow, "Z", "Z", 1, BLACK,9, FALSE, TA_CENTER, "", 0);
	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_WEB, bNextFiller))
	{
		pOut->SetXL(pXL, "인접복부판 두께"				, m_nSttRow, "T", "W", 0);
		szTemp.Format("=%.1lf", pBx->GetFactChain(G_W)->m_dFactChainThick);
		pOut->SetXL(pXL, szTemp							, m_nSttRow, "X", "Y", 0, RED,  9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, "mm"							, m_nSttRow, "Z", "Z", 0, BLACK,9, FALSE, TA_CENTER, "", 0);
	}
	m_nSttRow	= nRowTemp1+nRowTemp;
	
	CDoubleArray arrBoltPos;
	CWordArray   arrBoltsu;
	long   Nl = 0;
	double Lp = 0;
	double Lb = (pGir->GetHeightGirderByStaAng(dSta) - ((pSp->m_mD + pSp->m_mF * pSp->m_mE)*2 + pSp->m_sF*pSp->m_sE + pSp->m_sF1*pSp->m_sE1*2)) / 2;  // 이음판 1,2 사이의 볼트거리
	double d1;
	double d2;
	BOOL   bMoment =FALSE;
	long   ni;
	double Hw = pBx->GetHeightGirderByBx();

	d1 = (pSp->m_mD + pSp->m_mE * pSp->m_mF) * 2;
	d2 = pSp->m_sE * pSp->m_sF + (pSp->m_sE1 * pSp->m_sF1)*2;			

	for (int i = 0; i < 5; i++)   
	{
		if (i <= pSp->m_mE && pSp->m_mE >0)	 //모멘트 이음판
		{  
			bMoment = TRUE;
			Nl = (int) pSp->m_mB + 1;	// 모멘트 이음판 라인당 볼트수
			
			if (i == 0)					// 모멘트 이음판 첫줄							
				Lp += pSp->m_mD + pSp->m_mF / 2;	
			else if (i == pSp->m_mE)	// 모멘트 이음판 마지막줄				
				Lp += pSp->m_mF / 2 + (Hw - (d1 +d2))/4;	
			else						// 모멘트 이음판 중간줄				
				Lp += pSp->m_mF;					
		}
		else	// 전단 이음판
		{  
			ni = bMoment ? i-pSp->m_mE-1 : i-pSp->m_mE;
			if(pSp->m_sE1 > 0) // sE1 > 0
			{
				if(ni ==0)
					Lp += bMoment ? pSp->m_sF1 /2 + (Hw-(d1+d2))/4 : pSp->m_sF1 / 2 + Lb;
				else if(pSp->m_sE1 > ni)
					Lp += pSp->m_sF1;
				else if(pSp->m_sE1 == ni)
					Lp += pSp->m_sF1/2 +pSp->m_sF/2;
				else
					Lp += pSp->m_sF;
			}
			else // sE1 ==0
			{
				if (ni == 0)	//전단 이음판 첫번째 열
				{
					Lp += bMoment ? (pSp->m_sF /2) + (Hw-(d1 + d2)) /4 : pSp->m_sF / 2 + Lb;
				}
				else if(pSp->m_sE > ni)	// 전단 이음판 중간 열				
					Lp += pSp->m_sF;				
			}	
			Nl = (int) pSp->m_sB + 1;	// 전단 이음판 라인당 볼트수
		}

		arrBoltPos.Add(Lp);
		arrBoltsu.Add((WORD)Nl);
	}

	pOut->SetXL		(pXL, "  중립축에서 연단까지의 거리", m_nSttRow, "C", "J", 0);
	pOut->SetXL		(pXL, "ho ="						, m_nSttRow, "K", "L", 0);
	szTemp.Format	("=%s/2", pOut->GetCellRef("WEB_H"));
	pOut->SetXL		(pXL, szTemp						, m_nSttRow, "M", "N", 0, BLACK,9, FALSE, TA_CENTER, "ho", 1);
	pOut->SetXL		(pXL, "mm"							, m_nSttRow, "O", "O", 1, BLACK,9, FALSE, TA_CENTER, "", 0);
	///< f1 = fu = 1425
	pOut->SetXL		(pXL, "f1 = fu ="					, m_nSttRow, "D", "F", 0);
	szTemp.Format	("=MAX(%s,%s)", pOut->GetCellRef("DesignFs_CP"), pOut->GetCellRef("DesignFs_TS"));
	pOut->SetXL		(pXL, szTemp						, m_nSttRow, "G", "H", 0, BLACK,9, FALSE, TA_CENTER, "f1", 1);
	pOut->SetXL		(pXL, "MPa"							, m_nSttRow, "I", "J", 1, BLACK,9, FALSE, TA_CENTER, "", 1);
	///< f2 =  f1 .(ho-110) / ho
	pOut->SetXL		(pXL, "f2 = f1·(ho -"				, m_nSttRow, "D", "G", 0);
	pOut->SetXL		(pXL, arrBoltPos[0]					, m_nSttRow, "H", "I", 0, RED,  9, FALSE, TA_CENTER, "BoltPos1", 0);
	pOut->SetXL		(pXL, ")/ho ="						, m_nSttRow, "J", "K", 0, BLACK,9, FALSE, TA_CENTER, "", 0);
	szTemp.Format	("=%s*(%s-%s)/%s", pOut->GetCellRef("f1"), pOut->GetCellRef("ho"), pOut->GetCellRef("BoltPos1"), pOut->GetCellRef("ho"));
	pOut->SetXL		(pXL, szTemp						, m_nSttRow, "L", "N", 0, BLACK,9, FALSE, TA_CENTER, "f2", 1);
	pOut->SetXL		(pXL, "MPa"							, m_nSttRow, "O", "O", 1);
	///< f3 =  f1 .(ho-210) / ho
	pOut->SetXL		(pXL, "f3 = f1·(ho -"				, m_nSttRow, "D", "G", 0);
	pOut->SetXL		(pXL, arrBoltPos[1]					, m_nSttRow, "H", "I", 0, RED,  9, FALSE, TA_CENTER, "BoltPos2", 0);
	pOut->SetXL		(pXL, ")/ho ="						, m_nSttRow, "J", "K", 0, BLACK,9, FALSE, TA_CENTER, "", 0);
	szTemp.Format	("=%s*(%s-%s)/%s", pOut->GetCellRef("f1"), pOut->GetCellRef("ho"), pOut->GetCellRef("BoltPos2"), pOut->GetCellRef("ho"));
	pOut->SetXL		(pXL, szTemp						, m_nSttRow, "L", "N", 0, BLACK,9, FALSE, TA_CENTER, "f3", 1);
	pOut->SetXL		(pXL, "MPa"							, m_nSttRow, "O", "O", 1);
	///< f4 =  f1 .(ho-300) / ho
	pOut->SetXL		(pXL, "f4 = f1·(ho -"				, m_nSttRow, "D", "G", 0);
	pOut->SetXL		(pXL, arrBoltPos[2]					, m_nSttRow, "H", "I", 0, RED,  9, FALSE, TA_CENTER, "BoltPos3", 0);
	pOut->SetXL		(pXL, ")/ho ="						, m_nSttRow, "J", "K", 0, BLACK,9, FALSE, TA_CENTER, "", 0);
	szTemp.Format	("=%s*(%s-%s)/%s", pOut->GetCellRef("f1"), pOut->GetCellRef("ho"), pOut->GetCellRef("BoltPos3"), pOut->GetCellRef("ho"));
	pOut->SetXL		(pXL, szTemp						, m_nSttRow, "L", "N", 0, BLACK,9, FALSE, TA_CENTER, "f4", 1);
	pOut->SetXL		(pXL, "MPa"							, m_nSttRow, "O", "O", 1);
	///< f5 =  f1 .(ho-400) / ho
	pOut->SetXL		(pXL, "f5 = f1·(ho -"				, m_nSttRow, "D", "G", 0);
	pOut->SetXL		(pXL, arrBoltPos[3]						, m_nSttRow, "H", "I", 0, RED,  9, FALSE, TA_CENTER, "BoltPos4", 0);
	pOut->SetXL		(pXL, ")/ho ="						, m_nSttRow, "J", "K", 0, BLACK,9, FALSE, TA_CENTER, "", 0);
	szTemp.Format	("=%s*(%s-%s)/%s", pOut->GetCellRef("f1"), pOut->GetCellRef("ho"), pOut->GetCellRef("BoltPos4"), pOut->GetCellRef("ho"));
	pOut->SetXL		(pXL, szTemp						, m_nSttRow, "L", "N", 0, BLACK,9, FALSE, TA_CENTER, "f5", 1);
	pOut->SetXL		(pXL, "MPa"							, m_nSttRow, "O", "O", 1);
	///< f6 =  f1 .(ho-500) / ho
	pOut->SetXL		(pXL, "f6 = f1·(ho -"				, m_nSttRow, "D", "G", 0);
	pOut->SetXL		(pXL, arrBoltPos[4]					, m_nSttRow, "H", "I", 0, RED,  9, FALSE, TA_CENTER, "BoltPos5", 0);
	pOut->SetXL		(pXL, ")/ho ="						, m_nSttRow, "J", "K", 0, BLACK,9, FALSE, TA_CENTER, "", 0);
	szTemp.Format	("=%s*(%s-%s)/%s", pOut->GetCellRef("f1"), pOut->GetCellRef("ho"), pOut->GetCellRef("BoltPos5"), pOut->GetCellRef("ho"));
	pOut->SetXL		(pXL, szTemp						, m_nSttRow, "L", "N", 0, BLACK,9, FALSE, TA_CENTER, "f6", 1);
	pOut->SetXL		(pXL, "MPa"							, m_nSttRow, "O", "O", 1);
	m_nSttRow++;
	///< 제 1 열 : .........
	pOut->SetLineBoxText  (pXL, "제1열 :"				, m_nSttRow, "B", "C", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format		  ("=%s", pOut->GetCellRef("f1")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetUnderLineText(pXL, "+"						, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("f2")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("BoltPos1")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "L", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("WEB_T")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "mm / "					, m_nSttRow, "P", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, arrBoltsu[0]			, m_nSttRow, "R", "R", 2, RED,   9, FALSE, TA_CENTER, "Boltsu1", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "EA = "					, m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	m_nSttRow++;
	pOut->SetXL			  (pXL, "2"						, m_nSttRow, "D", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
	m_nSttRow--;
	szTemp.Format("=(%s+%s)/2*%s*%s/%s", pOut->GetCellRef("f1"), pOut->GetCellRef("f2"), pOut->GetCellRef("BoltPos1"), pOut->GetCellRef("WEB_T"), pOut->GetCellRef("Boltsu1"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "U", "W", 2, BLACK, 9, FALSE, TA_CENTER, "Boltfa1", 1, FALSE);	
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"N/EA ＞R·ρa\", \"N/EA ＜R·ρa\" ", pOut->GetCellRef("Boltfa1"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"N/EA ＞ρa\", \"N/EA ＜ρa\" ", pOut->GetCellRef("Boltfa1"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "X", "AA",2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);	
	m_nSttRow+=2;
	///< 제 2 열 : .........
	pOut->SetLineBoxText  (pXL, "제2열 :"				, m_nSttRow, "B", "C", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format		  ("=%s", pOut->GetCellRef("f2")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetUnderLineText(pXL, "+"						, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("f3")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s-%s", pOut->GetCellRef("BoltPos2"), pOut->GetCellRef("BoltPos1")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "L", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("WEB_T")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "mm / "					, m_nSttRow, "P", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, arrBoltsu[1]			, m_nSttRow, "R", "R", 2, RED,   9, FALSE, TA_CENTER, "Boltsu2", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "EA = "					, m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	m_nSttRow++;
	pOut->SetXL			  (pXL, "2"						, m_nSttRow, "D", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
	m_nSttRow--;
	szTemp.Format("=(%s+%s)/2*(%s-%s)*%s/%s", pOut->GetCellRef("f2"), pOut->GetCellRef("f3"), pOut->GetCellRef("BoltPos2"), pOut->GetCellRef("BoltPos1"), pOut->GetCellRef("WEB_T"), pOut->GetCellRef("Boltsu2"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "U", "W", 2, BLACK, 9, FALSE, TA_CENTER, "Boltfa2", 1, FALSE);	
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"N/EA ＞R·ρa\", \"N/EA ＜R·ρa\" ", pOut->GetCellRef("Boltfa2"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"N/EA ＞ρa\", \"N/EA ＜ρa\" ", pOut->GetCellRef("Boltfa2"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "X", "AA",2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);	
	m_nSttRow+=2;
	///< 제 3 열 : .........
	pOut->SetLineBoxText  (pXL, "제3열 :"				, m_nSttRow, "B", "C", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format		  ("=%s", pOut->GetCellRef("f3")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetUnderLineText(pXL, "+"						, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("f4")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s-%s", pOut->GetCellRef("BoltPos3"), pOut->GetCellRef("BoltPos2")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "L", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("WEB_T")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "mm / "					, m_nSttRow, "P", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, arrBoltsu[2]			, m_nSttRow, "R", "R", 2, RED,   9, FALSE, TA_CENTER, "Boltsu3", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "EA = "					, m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	m_nSttRow++;
	pOut->SetXL			  (pXL, "2"						, m_nSttRow, "D", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
	m_nSttRow--;
	szTemp.Format("=(%s+%s)/2*(%s-%s)*%s/%s", pOut->GetCellRef("f3"), pOut->GetCellRef("f4"), pOut->GetCellRef("BoltPos3"), pOut->GetCellRef("BoltPos2"), pOut->GetCellRef("WEB_T"), pOut->GetCellRef("Boltsu3"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "U", "W", 2, BLACK, 9, FALSE, TA_CENTER, "Boltfa3", 1, FALSE);	
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"N/EA ＞R·ρa\", \"N/EA ＜R·ρa\" ", pOut->GetCellRef("Boltfa3"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"N/EA ＞ρa\", \"N/EA ＜ρa\" ", pOut->GetCellRef("Boltfa3"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "X", "AA",2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);	
	m_nSttRow+=2;
	///< 제 4 열 : .........
	pOut->SetLineBoxText  (pXL, "제4열 :"				, m_nSttRow, "B", "C", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format		  ("=%s", pOut->GetCellRef("f4")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetUnderLineText(pXL, "+"						, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("f5")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s-%s", pOut->GetCellRef("BoltPos4"), pOut->GetCellRef("BoltPos3")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "L", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("WEB_T")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "mm / "					, m_nSttRow, "P", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, arrBoltsu[3]			, m_nSttRow, "R", "R", 2, RED,   9, FALSE, TA_CENTER, "Boltsu4", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "EA = "					, m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	m_nSttRow++;
	pOut->SetXL			  (pXL, "2"						, m_nSttRow, "D", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
	m_nSttRow--;
	szTemp.Format("=(%s+%s)/2*(%s-%s)*%s/%s", pOut->GetCellRef("f4"), pOut->GetCellRef("f5"), pOut->GetCellRef("BoltPos4"), pOut->GetCellRef("BoltPos3"), pOut->GetCellRef("WEB_T"), pOut->GetCellRef("Boltsu4"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "U", "W", 2, BLACK, 9, FALSE, TA_CENTER, "Boltfa4", 1, FALSE);	
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"N/EA ＞R·ρa\", \"N/EA ＜R·ρa\" ", pOut->GetCellRef("Boltfa4"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"N/EA ＞ρa\", \"N/EA ＜ρa\" ", pOut->GetCellRef("Boltfa4"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "X", "AA",2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	m_nSttRow+=2;
	///< 제 5 열 : .........
	pOut->SetLineBoxText  (pXL, "제5열 :"				, m_nSttRow, "B", "C", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format		  ("=%s", pOut->GetCellRef("f5")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetUnderLineText(pXL, "+"						, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("f6")); 
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s-%s", pOut->GetCellRef("BoltPos5"), pOut->GetCellRef("BoltPos4")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "L", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	szTemp.Format		  ("=%s", pOut->GetCellRef("WEB_T")); 
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "mm / "					, m_nSttRow, "P", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, arrBoltsu[4]			, m_nSttRow, "R", "R", 2, RED,   9, FALSE, TA_CENTER, "Boltsu5", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "EA = "					, m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);			
	m_nSttRow++;
	pOut->SetXL			  (pXL, "2"						, m_nSttRow, "D", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
	m_nSttRow--;
	szTemp.Format("=(%s+%s)/2*(%s-%s)*%s/%s", pOut->GetCellRef("f5"), pOut->GetCellRef("f6"), pOut->GetCellRef("BoltPos5"), pOut->GetCellRef("BoltPos4"), pOut->GetCellRef("WEB_T"), pOut->GetCellRef("Boltsu5"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "U", "W", 2, BLACK, 9, FALSE, TA_CENTER, "Boltfa5", 1, FALSE);	
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"N/EA ＞R·ρa\", \"N/EA ＜R·ρa\" ", pOut->GetCellRef("Boltfa5"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"N/EA ＞ρa\", \"N/EA ＜ρa\" ", pOut->GetCellRef("Boltfa5"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "X", "AA",2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	m_nSttRow+=3;
	
	double dWFlangeUp = pGir->GetWidthFlangeUpper(pBx);
	double dTFlangeUp = pGir->GetThickFactByStation(dSta, G_F_U);
	double dHWeb      = pGir->GetHeightGirderByStaAng(dSta);
	double dTWeb      = pGir->GetThickFactByStation(dSta, G_W);
	double dWFlangeDn = pGir->GetWidthFlangeLower(pBx);
	double dTFlangeDn = pGir->GetThickFactByStation(dSta, G_F_L);
	pModelCalc->GetSectionCoefficient(nG, dSta);
	double dK         = pModelCalc->GetMapValueSecCo("K");

	pOut->SetXL			  (pXL, "  · 전단력에 의한 볼트의 응력"		, m_nSttRow, "C", "C", 1);
	pOut->SetXL			  (pXL, "  복부판이 부담하는 전단력 : Ss + Sv"	, m_nSttRow, "C", "C", 1);
	pOut->SetXL			  (pXL, "  비틀림 모멘트에 의한 복부의 전단력   : St = Mt·t / K"	, m_nSttRow, "C", "C", 1);
	pOut->SetXL			  (pXL, "  K = ∑(1/3)·b·t³   ( ☞ 도.설.해 3.8.2.3 )"		, m_nSttRow, "C", "C", 1);
	pOut->SetXL			  (pXL, "="						, m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "("						, m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, dWFlangeUp				, m_nSttRow, "F", "G", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×"					, m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, dTFlangeUp				, m_nSttRow, "I", "J", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "³+"					, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, dHWeb					, m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×"					, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, dTWeb					, m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "³+"					, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, dWFlangeDn				, m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "×"					, m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, dTFlangeDn				, m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL			  (pXL, "³)/3"					, m_nSttRow, "W", "X", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "'=",		m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dK,		m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "K", 1);
	pOut->SetXL(pXL, "mm⁴",	m_nSttRow, "H", "H", 1, BLACK, 9);

	pOut->SetXL			  (pXL, "  따라서, 전단력에 의한 볼트의 응력은"	, m_nSttRow, "C", "C", 1);
	pOut->SetLineBoxText  (pXL, "ρs"					, m_nSttRow, "C", "C", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "="						, m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText  (pXL, "("						, m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);			
	szTemp.Format("=ABS(%s+%s)", pOut->GetCellRef("Ss_SP"), pOut->GetCellRef("Sv_SP"));
	pOut->SetLineBoxText  (pXL, szTemp					, m_nSttRow, "F", "H", 2, BLACK, 9, FALSE, TA_CENTER, "TAU_BOLT_PA1", 1, FALSE);			
	pOut->SetLineBoxText  (pXL, "×"					, m_nSttRow, "I", "I", 2, BLACK, 9, FALSE, TA_CENTER, ""            , 0, FALSE);			
	pOut->SetLineBoxText  (pXL, 1000					, m_nSttRow, "J", "L", 2, BLACK, 9, FALSE, TA_CENTER, "TAU_BOLT_PA2", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "+"						, m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, ""            , 0, FALSE);			
	szTemp.Format("=ABS(%s)", pOut->GetCellRef("Mt_SP"));
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "TAU_BOLT_PA3", 2);			
	pOut->SetUnderLineText(pXL, "×"					, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	pOut->SetUnderLineText(pXL, 1000000					, m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "TAU_BOLT_PA4", 0);			
	pOut->SetUnderLineText(pXL, "×"					, m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);			
	szTemp.Format("=%s", pOut->GetCellRef("WEB_T"));
	pOut->SetUnderLineText(pXL, szTemp					, m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER, "TAU_BOLT_PA5", 2);			
	pOut->SetLineBoxText  (pXL, ")"						, m_nSttRow, "X", "X", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "/"						, m_nSttRow, "Y", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	long nBoltWebHalf = (pSp->m_sE+pSp->m_sE1*2+1)*(pSp->m_sB+1);
	if(pSp->m_nWebType!=0)	nBoltWebHalf += (pSp->m_mE+1)*(pSp->m_mB+1)*2;
	  
	pOut->SetLineBoxText  (pXL, nBoltWebHalf			, m_nSttRow, "Z", "Z", 2, RED,  9, FALSE, TA_CENTER, "TAU_BOLT_PA7", 0, FALSE);			
	pOut->SetLineBoxText  (pXL, "EA"					, m_nSttRow, "AA","AA",2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);			
	m_nSttRow++;///<분모
	szTemp.Format("=%s", pOut->GetCellRef("K"));
	pOut->SetXL			  (pXL, szTemp					, m_nSttRow, "N", "W", 0, BLACK, 9, FALSE, TA_CENTER, "TAU_BOLT_PA6", 0);

	m_nSttRow++;
	pOut->SetXL(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=ROUND((%s*%s+%s*%s*%s/%s)/%s,1)", pOut->GetCellRef("TAU_BOLT_PA1"), pOut->GetCellRef("TAU_BOLT_PA2"), pOut->GetCellRef("TAU_BOLT_PA3"),
													   pOut->GetCellRef("TAU_BOLT_PA4"), pOut->GetCellRef("TAU_BOLT_PA5"), pOut->GetCellRef("TAU_BOLT_PA6"), pOut->GetCellRef("TAU_BOLT_PA7"));
	pOut->SetXL  (pXL, szTemp					, m_nSttRow, "E","G", 0, BLACK, 9, FALSE, TA_CENTER, "TAU_BOLT_SHEAR", 1);
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s,\"N/EA   ＞ R·ρa\", \"N/EA  ＜  R·ρa\"", pOut->GetCellRef("TAU_BOLT_SHEAR"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s,\"N/EA   ＞ ρa\", \"N/EA  ＜  ρa\"", pOut->GetCellRef("TAU_BOLT_SHEAR"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetXL  (pXL, szTemp					, m_nSttRow, "H","H", 2);

	pOut->SetXL			  (pXL, "  · 합성응력의 검토"		 , m_nSttRow, "C", "C", 1);
	pOut->SetXL			  (pXL, " ρ = √ (ρp²＋ρs²) = √( ", m_nSttRow, "C", "J", 0,BLACK, 9, FALSE,TA_RIGHT);	
	szTemp.Format("=ROUND(MAX(%s:%s),1)", pOut->GetCellRef("Boltfa1"), pOut->GetCellRef("Boltfa4"));
	pOut->SetXL			  (pXL, szTemp						 , m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_RIGHT, "COMP_STRESSCHECK1", 1);
	pOut->SetXL			  (pXL, "²"						 , m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL			  (pXL, "+"						 , m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s", pOut->GetCellRef("TAU_BOLT_SHEAR"));
	pOut->SetXL			  (pXL, szTemp						 , m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "COMP_STRESSCHECK2", 1);
	pOut->SetXL			  (pXL, " ²"						 , m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			  (pXL, " )"						 , m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	szTemp.Format("=ROUND(SQRT(%s^2+%s^2),1)", pOut->GetCellRef("COMP_STRESSCHECK1"), pOut->GetCellRef("COMP_STRESSCHECK2")); 
	pOut->SetXL			  (pXL, szTemp						 , m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "COMP_STRESSCHECK", 1);
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s,\"N/EA   ＞ R·ρa, NG\",\"N/EA  ＜  R·ρa, OK\"", pOut->GetCellRef("COMP_STRESSCHECK"), pOut->GetCellRef("faBoltTotal_SP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s,\"N/EA   ＞ ρa, NG\",\"N/EA  ＜  ρa, OK\"", pOut->GetCellRef("COMP_STRESSCHECK"), pOut->GetCellRef("faBoltTotal_SP"));
	pOut->SetXL			  (pXL, szTemp						 , m_nSttRow, "I","I", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);
	
	m_nSttRow++; 
}

void CXLSpliceCheck::SpliceCheckWebPart2(CPlateBasicIndex* pBx)
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CPlateGirderApp     *pGir           = pBx->GetGirder();
	CPlateSplice        *pSp            = pBx->GetSplice();

	double dSta      = pBx->GetStation();
	BOOL	bHybrid	 = pBx->IsHybrid();
	CString	szTemp		= _T("");

	pOut->SetXL(pXL, " ② 이음판의 응력계산"	 			 , m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "  · 복부판의 단면2차모멘트"		  	 , m_nSttRow, "C", "C", 0);
	///< 가상으로 만들어 놓음//////////////////////////////////
	szTemp.Format("=IF((%s+%s)>0,%s,%s)", pOut->GetCellRef("Ms_SP"), pOut->GetCellRef("Mv_SP"), pOut->GetCellRef("dYQs_CP"), pOut->GetCellRef("dYQs_TS"));
	pOut->SetXL(pXL, szTemp									 , m_nSttRow, "M", "N", 0, 2, 9, FALSE, TA_CENTER, "YSU", 1);
	szTemp.Format("=(%s+%s+%s)-%s", pOut->GetCellRef("WEB_H"), pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("YSU"));
	pOut->SetXL(pXL, szTemp									 , m_nSttRow, "O", "P", 0, 2, 9, FALSE, TA_CENTER, "YSL", 1);
	szTemp.Format("=IF((%s+%s)>0,%s,%s)", pOut->GetCellRef("Ms_SP"), pOut->GetCellRef("Mv_SP"), pOut->GetCellRef("dYQv_CP"), pOut->GetCellRef("dYQv_TS)"));
	pOut->SetXL(pXL, szTemp									 , m_nSttRow, "Q", "R", 0, 2, 9, FALSE, TA_CENTER, "YVU", 1);
	szTemp.Format("=(%s+%s+%s)-%s", pOut->GetCellRef("WEB_H"), pOut->GetCellRef("FlangeT_CP"), pOut->GetCellRef("FlangeT_TS"), pOut->GetCellRef("YVU"));
	pOut->SetXL(pXL, szTemp									 , m_nSttRow, "S", "T", 1, 2, 9, FALSE, TA_CENTER, "YVL", 1);
	//////////////////////////////////////////////////////////
	pOut->SetLineBoxText  (pXL, "Isw'="						, m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_T"));
	pOut->SetUnderLineText(pXL, szTemp						, m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "ISW_PARA1", 1);
	pOut->SetUnderLineText(pXL, "×"						, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_H"));
	pOut->SetUnderLineText(pXL, szTemp						, m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_RIGHT,  "ISW_PARA2", 1);
	pOut->SetUnderLineText(pXL, "³"						, m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetLineBoxText  (pXL, "+"							, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_T"));
	pOut->SetLineBoxText  (pXL,szTemp						, m_nSttRow, "O", "P", 2, BLACK, 9, FALSE, TA_CENTER, "ISW_PARA3", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "×"						, m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_H"));
	pOut->SetLineBoxText  (pXL, szTemp						, m_nSttRow, "R", "S", 2, BLACK, 9, FALSE, TA_CENTER, "ISW_PARA4", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "×("						, m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	szTemp.Format("=%s/2", pOut->GetCellRef("WEB_H"));
	pOut->SetLineBoxText  (pXL, szTemp						, m_nSttRow, "U", "V", 2, BLACK, 9, FALSE, TA_CENTER, "ISW_PARA5", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "-"							, m_nSttRow, "W", "W", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("YSL"));
	pOut->SetLineBoxText  (pXL, szTemp						, m_nSttRow, "X", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "ISW_PARA6", 1, FALSE);
	pOut->SetLineBoxText  (pXL, ")²"						, m_nSttRow, "Z", "Z", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	m_nSttRow++;
	pOut->SetXL			  (pXL, 12							, m_nSttRow, "F", "M", 1, BLACK, 9, FALSE, TA_CENTER, "ISW_PARA7", 0);	
	pOut->SetXL(pXL, "="									, m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s*%s^3/%s+%s*%s*(%s-%s)^2", pOut->GetCellRef("ISW_PARA1"), pOut->GetCellRef("ISW_PARA2"), pOut->GetCellRef("ISW_PARA7")
											   , pOut->GetCellRef("ISW_PARA3"), pOut->GetCellRef("ISW_PARA4"), pOut->GetCellRef("ISW_PARA5"), pOut->GetCellRef("ISW_PARA6"));

	pOut->SetXL  (pXL, szTemp							, m_nSttRow, "F","I", 0, BLACK, 9, FALSE, TA_CENTER, "ISW", 1);
	pOut->SetXL  (pXL, "mm⁴"							, m_nSttRow, "J","J", 2);
	
	pOut->SetLineBoxText  (pXL, "Ivw'="						, m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_T"));
	pOut->SetUnderLineText(pXL, szTemp						, m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "IVW_PARA1", 1);
	pOut->SetUnderLineText(pXL, "×"						, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, ""		 , 1);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_H"));
	pOut->SetUnderLineText(pXL, szTemp						, m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_RIGHT,  "IVW_PARA2", 1);
	pOut->SetUnderLineText(pXL, "³"						, m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER, ""		 , 1);
	pOut->SetLineBoxText  (pXL, "+"							, m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, ""		 , 1, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_T"));
	pOut->SetLineBoxText  (pXL, szTemp						, m_nSttRow, "O", "P", 2, BLACK, 9, FALSE, TA_CENTER, "IVW_PARA3", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "×"						, m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, ""		 , 1, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("WEB_H"));
	pOut->SetLineBoxText  (pXL, szTemp						, m_nSttRow, "R", "S", 2, BLACK, 9, FALSE, TA_CENTER, "IVW_PARA4", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "×("						, m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, ""		 , 1, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("YVL"));
	pOut->SetLineBoxText  (pXL, szTemp						, m_nSttRow, "U", "V", 2, BLACK, 9, FALSE, TA_CENTER, "IVW_PARA5", 1, FALSE);
	pOut->SetLineBoxText  (pXL, "-"							, m_nSttRow, "W", "W", 2, BLACK, 9, FALSE, TA_CENTER, ""		 , 1, FALSE);
	szTemp.Format("=%s/2", pOut->GetCellRef("WEB_H"));
	pOut->SetLineBoxText  (pXL, szTemp						, m_nSttRow, "X", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "IVW_PARA6", 1, FALSE);
	pOut->SetLineBoxText  (pXL, ")²"						, m_nSttRow, "Z", "Z", 2, BLACK, 9, FALSE, TA_CENTER, ""		 , 1, FALSE);
	m_nSttRow++;
	pOut->SetXL			  (pXL, 12							, m_nSttRow, "F", "M", 1, BLACK, 9, FALSE, TA_CENTER, "IVW_PARA7", 0);	
	pOut->SetXL(pXL, "="									, m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	szTemp.Format("=%s*%s^3/%s+%s*%s*(%s-%s)^2", pOut->GetCellRef("IVW_PARA1"), pOut->GetCellRef("IVW_PARA2"), pOut->GetCellRef("IVW_PARA7")
											   , pOut->GetCellRef("IVW_PARA3"), pOut->GetCellRef("IVW_PARA4"), pOut->GetCellRef("IVW_PARA5"), pOut->GetCellRef("IVW_PARA6"));
	pOut->SetXL  (pXL, szTemp						, m_nSttRow, "F","I", 0, BLACK, 9, FALSE, TA_CENTER, "IVW", 1);
	pOut->SetXL  (pXL, "mm⁴"						, m_nSttRow, "J","J", 2);

	pOut->SetXL			  (pXL, "  · 복부에 작용하는 모멘트", m_nSttRow, "C", "C", 1);
	m_nSttRow+=5;
	pOut->SetXL			  (pXL, "Msw : 복부에 작용하는 합성전 하중에 의한 모멘트, Ms =", m_nSttRow, "D", "Q", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Ms_SP"));
	pOut->SetXL			  (pXL, szTemp												   , m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	
	pOut->SetXL			  (pXL, "kN·m"											   , m_nSttRow, "V", "V", 1);
	pOut->SetXL			  (pXL, "Mvw : 복부에 작용하는 합성후 하중에 의한 모멘트, Mv =", m_nSttRow, "D", "Q", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Mv_SP"));
	pOut->SetXL			  (pXL, szTemp												   , m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	
	pOut->SetXL			  (pXL, "kN·m"											   , m_nSttRow, "V", "V", 1);
	pOut->SetXL			  (pXL, "Is  : 강재보단면의 단면2차모멘트               , Is =", m_nSttRow, "D", "Q", 0);		
	szTemp.Format("=%s", pOut->GetCellRef("Is_SP"));
	pOut->SetXL			  (pXL, szTemp												   , m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
	pOut->SetXL			  (pXL, "mm⁴"												   , m_nSttRow, "V", "V", 1);
	pOut->SetXL			  (pXL, "Iv  : 합성  단면의 단면2차모멘트               , Iv =", m_nSttRow, "D", "Q", 0);
	szTemp.Format("=%s", pOut->GetCellRef("Iv_SP"));
	pOut->SetXL			  (pXL, szTemp												   , m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
	pOut->SetXL			  (pXL, "m⁴"												   , m_nSttRow, "V", "V", 1);
	m_nSttRow-=9;
	
	pOut->SetLineBoxText  (pXL, "Msw = Ms·"					, m_nSttRow, "D", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				
	pOut->SetUnderLineText(pXL, "Isw'"							, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow++; 
	pOut->SetXL			  (pXL, "Is"							, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow--;
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				

	szTemp.Format("=%s", pOut->GetCellRef("Ms_SP"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "I", "J", 2, BLACK, 9, FALSE, TA_CENTER, "MSW_PARA1", 1, FALSE);				
	pOut->SetLineBoxText  (pXL, "×"							, m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				
	szTemp.Format("=%s", pOut->GetCellRef("ISW"));
	pOut->SetUnderLineText(pXL, szTemp							, m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER, "MSW_PARA2", 0);
	m_nSttRow++; 
	szTemp.Format("=%s", pOut->GetCellRef("Is_SP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER, "MSW_PARA3", 0);
	m_nSttRow--;
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				
	szTemp.Format("=%s*%s/%s", pOut->GetCellRef("MSW_PARA1"), pOut->GetCellRef("MSW_PARA2"), pOut->GetCellRef("MSW_PARA3"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "Q", "R", 2, BLACK, 9, FALSE, TA_CENTER, "MSW", 1, FALSE);				
	pOut->SetLineBoxText  (pXL, "kN·m"						, m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				
	
	m_nSttRow+=2;
	pOut->SetLineBoxText  (pXL, "Mvw = Mv·"					, m_nSttRow, "D", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				
	pOut->SetUnderLineText(pXL, "Isv'"							, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow++; 
	pOut->SetXL			  (pXL, "Iv"							, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow--;
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				

	szTemp.Format("=%s", pOut->GetCellRef("Mv_SP"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "I", "J", 2, BLACK, 9, FALSE, TA_CENTER, "MVW_PARA1", 1, FALSE);				
	pOut->SetLineBoxText  (pXL, "×"							, m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				
	szTemp.Format("=%s", pOut->GetCellRef("IVW"));
	pOut->SetUnderLineText(pXL,szTemp							, m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER, "MVW_PARA2", 0);
	m_nSttRow++; 
	szTemp.Format("=%s", pOut->GetCellRef("Iv_SP"));
	pOut->SetXL			  (pXL, szTemp							, m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER, "MVW_PARA3", 0);
	m_nSttRow--;
	pOut->SetLineBoxText  (pXL, "="								, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);				
	szTemp.Format("=%s*%s/%s", pOut->GetCellRef("MVW_PARA1"), pOut->GetCellRef("MVW_PARA2"), pOut->GetCellRef("MVW_PARA3"));
	pOut->SetLineBoxText  (pXL, szTemp							, m_nSttRow, "Q", "R", 2, BLACK, 9, FALSE, TA_CENTER, "MVW", 1, FALSE);				
	pOut->SetLineBoxText  (pXL, "kN·m"						, m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	
	m_nSttRow+=8;
	pOut->SetXL			  (pXL, "  · 이음판의 단면2차모멘트"	, m_nSttRow, "C", "C", 1);
	///< 박스그리기
	for(long n=0; n<4; n++)
	{
		pOut->SetLineBoxText  (pXL, ""							, m_nSttRow, "D", "H");
		pOut->SetLineBoxText  (pXL, ""							, m_nSttRow, "I", "K");
		pOut->SetLineBoxText  (pXL, ""							, m_nSttRow, "L", "N");
		pOut->SetLineBoxText  (pXL, ""							, m_nSttRow, "O", "Q");
		pOut->SetLineBoxText  (pXL, ""							, m_nSttRow, "R", "U");
		pOut->SetLineBoxText  (pXL, ""							, m_nSttRow, "V", "Y");
		m_nSttRow++;
	}
	m_nSttRow-=4;
	pOut->SetXL				  (pXL, "A"							, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, "Y"							, m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, "AY"						, m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, "AY²"						, m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, "Io"						, m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	
	pOut->SetXL				  (pXL, "4PL"						, m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, pSp->m_mW					, m_nSttRow, "E", "F", 0, RED,   9, FALSE, TA_CENTER, "WEBSP_MW", 0);
	pOut->SetXL				  (pXL, "×"						, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, pSp->m_mT					, m_nSttRow, "H", "H", 1, RED,   9, FALSE, TA_CENTER, "WEBSP_MT", 0);

	pOut->SetXL				  (pXL, "2PL"						, m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, pSp->m_sW					, m_nSttRow, "E", "F", 0, RED,   9, FALSE, TA_CENTER, "WEBSP_SW", 0);
	pOut->SetXL				  (pXL, "×"						, m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, pSp->m_sT					, m_nSttRow, "H", "H", 1, RED,   9, FALSE, TA_CENTER, "WEBSP_ST", 0);

	pOut->SetXL				  (pXL, "Σ"						, m_nSttRow, "D", "H", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	
	m_nSttRow-=3;
	szTemp.Format("=%s*%s*%d",	pOut->GetCellRef("WEBSP_MW"), pOut->GetCellRef("WEBSP_MT"),  4);
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "WEBSP_M_A", 2);
	double dYsp = (pGir->GetHeightGirderByStaAng(dSta)/2-(pSp->m_mG+pSp->m_mW/2));
	pOut->SetXL				  (pXL, dYsp						, m_nSttRow, "L", "N", 0, RED,   9, FALSE, TA_CENTER, "WEBSP_M_Y", 2);
	szTemp.Format("=%s*%s", pOut->GetCellRef("WEBSP_M_A"), pOut->GetCellRef("WEBSP_M_Y"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	szTemp.Format("=%s*%s^2", pOut->GetCellRef("WEBSP_M_A"), pOut->GetCellRef("WEBSP_M_Y"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "WEBSP_M_AY^2", 0);
	szTemp.Format("=%s^3*%s/12*4", pOut->GetCellRef("WEBSP_MW"), pOut->GetCellRef("WEBSP_MT"));
	pOut->SetXL				  (pXL,szTemp						, m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "WEBSP_M_IO", 2);

	szTemp.Format("=%s*%s*%d",	pOut->GetCellRef("WEBSP_SW"), pOut->GetCellRef("WEBSP_ST"),  2);
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "WEBSP_S_A", 2);
	pOut->SetXL				  (pXL, "-"							, m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL				  (pXL, "-"							, m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL				  (pXL, "-"							, m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	szTemp.Format("=%s^3*%s/12*2", pOut->GetCellRef("WEBSP_SW"), pOut->GetCellRef("WEBSP_ST"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "WEBSP_S_IO", 2);

	szTemp.Format("=%s+%s", pOut->GetCellRef("WEBSP_M_A"), pOut->GetCellRef("WEBSP_S_A"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "WEBSP_A", 2);
	pOut->SetXL				  (pXL, ""							, m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL				  (pXL, ""							, m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	szTemp.Format("=%s", pOut->GetCellRef("WEBSP_M_AY^2"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "WEBSP_AY^2", 0);
	szTemp.Format("=%s+%s", pOut->GetCellRef("WEBSP_M_IO"), pOut->GetCellRef("WEBSP_S_IO"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "WEBSP_IO", 2);

	pOut->SetXL				  (pXL, " I = Io ＋ AY²="			, m_nSttRow, "D", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s",	pOut->GetCellRef("WEBSP_IO"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "＋"						, m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s",	pOut->GetCellRef("WEBSP_AY^2"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL				  (pXL, "="							, m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s+%s", pOut->GetCellRef("WEBSP_IO"), pOut->GetCellRef("WEBSP_AY^2"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "WEBSP_I", 1);
	pOut->SetXL				  (pXL, "mm⁴"						, m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);

	pOut->SetXL				  (pXL, "Isw"						, m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "="							, m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("WEBSP_I"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ISW_SP_PARA1", 1);
	pOut->SetXL				  (pXL, "＋"						, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("WEBSP_A"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "ISW_SP_PARA2", 1);
	pOut->SetXL				  (pXL, "×("						, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s/2", pOut->GetCellRef("WEB_H"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "ISW_SP_PARA3", 1);
	pOut->SetXL				  (pXL, "-"							, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("YSL"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "ISW_SP_PARA4", 1);
	pOut->SetXL				  (pXL, ")²="						, m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s+%s*(%s-%s)^2", pOut->GetCellRef("ISW_SP_PARA1"), pOut->GetCellRef("ISW_SP_PARA2")
									, pOut->GetCellRef("ISW_SP_PARA3"), pOut->GetCellRef("ISW_SP_PARA4"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "V", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "ISW_SP", 1);
	pOut->SetXL				  (pXL, "mm⁴"						, m_nSttRow, "Z", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);

	pOut->SetXL				  (pXL, "Ivw"						, m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "="							, m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("WEBSP_I"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER, "IVW_SP_PARA1", 1);
	pOut->SetXL				  (pXL, "＋"						, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("WEBSP_A"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "IVW_SP_PARA2", 1);
	pOut->SetXL				  (pXL, "×("						, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("YVL"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "IVW_SP_PARA3", 1);
	pOut->SetXL				  (pXL, "-"							, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s/2", pOut->GetCellRef("WEB_H"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "IVW_SP_PARA4", 1);
	pOut->SetXL				  (pXL, ")²="						, m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s+%s*(%s-%s)^2", pOut->GetCellRef("IVW_SP_PARA1"), pOut->GetCellRef("IVW_SP_PARA2")
									, pOut->GetCellRef("IVW_SP_PARA3"), pOut->GetCellRef("IVW_SP_PARA4"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "V", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "IVW_SP", 1);
	pOut->SetXL				  (pXL, "mm⁴"						, m_nSttRow, "Z", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);

	pOut->SetXL				  (pXL, "Isw : 강재보단면의 중립축에 대한 이음판의 총단면2차모멘트", m_nSttRow, "D", "D", 1);
	pOut->SetXL				  (pXL, "Ivw : 합성  단면의 중립축에 대한 이음판의 총단면2차모멘트", m_nSttRow, "D", "D", 1);

	m_nSttRow++;
	pOut->SetXL				  (pXL, "  · 이음판의 응력"		, m_nSttRow, "C", "C", 1);
	pOut->SetXL				  (pXL, "Ysu,Ysl : 강재보단면의 중립축에서 이음판 상,하연까지의 거리", m_nSttRow, "C", "C", 1);
	pOut->SetXL				  (pXL, "Ysu ="						, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("YSU"));
	pOut->SetXL				  (pXL,szTemp						, m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "YSU", 1);
	pOut->SetXL				  (pXL, "mm"						, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "Ysl ="						, m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("YSL"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "mm"						, m_nSttRow, "P", "P", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);

	pOut->SetXL				  (pXL, "Yvu,Yvl : 합성  단면의 중립축에서 이음판 상,하연까지의 거리", m_nSttRow, "C", "C", 1);
	pOut->SetXL				  (pXL, "Yvu ="						, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("YVU"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "mm"						, m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "Yvl ="						, m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	szTemp.Format("=%s", pOut->GetCellRef("YVL"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL				  (pXL, "mm"						, m_nSttRow, "P", "P", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);

	pOut->SetXL				  (pXL, "fu : 이음판의 상연응력          fl : 이음판의 하연응력", m_nSttRow, "C", "C", 1);
	///<  fu
	pOut->SetLineBoxText	  (pXL, "fu ="						, m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText    (pXL, "Msw"						, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow++;
	pOut->SetXL				  (pXL, "Isw"						, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow--;
	pOut->SetLineBoxText	  (pXL, "·Ysu +"					, m_nSttRow, "G", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	pOut->SetUnderLineText    (pXL, "Mvw"						, m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow++;
	pOut->SetXL				  (pXL, "Ivw"						, m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow--;
	pOut->SetLineBoxText	  (pXL, "·Yvu"						, m_nSttRow, "K", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	m_nSttRow+=2;
	pOut->SetLineBoxText	  (pXL, "="							, m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("MSW"));
	pOut->SetUnderLineText    (pXL, szTemp						, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA1", 3);
	pOut->SetUnderLineText    (pXL, "×"						, m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText    (pXL, "1000000"					, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA2", 0);
	m_nSttRow++;
	szTemp.Format("=%s", pOut->GetCellRef("ISW_SP"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "E", "K", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA3", 1);
	m_nSttRow--;
	pOut->SetLineBoxText      (pXL, "×"						, m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("YSU"));
	pOut->SetLineBoxText      (pXL, szTemp						, m_nSttRow, "M", "O", 2, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA4", 3, FALSE);

	pOut->SetLineBoxText      (pXL, "+"							, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);

	szTemp.Format("=%s", pOut->GetCellRef("MVW"));
	pOut->SetUnderLineText    (pXL, szTemp						, m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA5", 3);
	pOut->SetUnderLineText    (pXL, "×"						, m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText    (pXL, "1000000"					, m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA6", 0);
	m_nSttRow++;
	szTemp.Format("=%s", pOut->GetCellRef("IVW_SP"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "Q", "W", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA7", 1);
	m_nSttRow--;
	pOut->SetLineBoxText      (pXL, "×"						, m_nSttRow, "X", "X", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("YVU"));
	pOut->SetLineBoxText      (pXL,szTemp						, m_nSttRow, "Y", "AA", 2, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA8", 3, FALSE);

	m_nSttRow+=2;
	pOut->SetXL				  (pXL, "="							, m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	szTemp.Format("=%s*%s/%s*%s", pOut->GetCellRef("SP_FU_PARA1"), pOut->GetCellRef("SP_FU_PARA2"), pOut->GetCellRef("SP_FU_PARA3"),pOut->GetCellRef("SP_FU_PARA4"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA9", 3);
	pOut->SetXL				  (pXL, "+"							, m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	szTemp.Format("=%s*%s/%s*%s", pOut->GetCellRef("SP_FU_PARA5"), pOut->GetCellRef("SP_FU_PARA6"), pOut->GetCellRef("SP_FU_PARA7"),pOut->GetCellRef("SP_FU_PARA8"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU_PARA10", 3);
	pOut->SetXL				  (pXL, "="							, m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	szTemp.Format("=%s+%s", pOut->GetCellRef("SP_FU_PARA9"), pOut->GetCellRef("SP_FU_PARA10"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FU", 3);
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"MPa ＞  R·fsa , ∴ N.G\",\"MPa ＜  R·fsa , ∴ O.K\" ", pOut->GetCellRef("SP_FU"), pOut->GetCellRef("ALLOW_COMP"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"MPa ＞  fsa , ∴ N.G\",\"MPa ＜  fsa , ∴ O.K\" ", pOut->GetCellRef("SP_FU"), pOut->GetCellRef("ALLOW_COMP"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "P", "X", 1);

	///<  fl
	pOut->SetLineBoxText	  (pXL, "fl ="						, m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText    (pXL, "Msw"						, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow++;
	pOut->SetXL				  (pXL, "Isw"						, m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow--;
	pOut->SetLineBoxText	  (pXL, "·Ysl +"					, m_nSttRow, "G", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	pOut->SetUnderLineText    (pXL, "Mvw"						, m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow++;
	pOut->SetXL				  (pXL, "Ivw"						, m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	m_nSttRow--;
	pOut->SetLineBoxText	  (pXL, "·Yvl"						, m_nSttRow, "K", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	m_nSttRow+=2;
	pOut->SetLineBoxText	  (pXL, "="							, m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("MSW"));
	pOut->SetUnderLineText    (pXL, szTemp						, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA1", 3);
	pOut->SetUnderLineText    (pXL, "×"						, m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText    (pXL, "1000000"					, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA2", 0);
	m_nSttRow++;
	szTemp.Format("=%s", pOut->GetCellRef("ISW_SP"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "E", "K", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA3", 1);
	m_nSttRow--;
	pOut->SetLineBoxText      (pXL, "×"						, m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("YSL"));
	pOut->SetLineBoxText      (pXL, szTemp						, m_nSttRow, "M", "O", 2, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA4", 3, FALSE);

	pOut->SetLineBoxText      (pXL, "+"							, m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);

	szTemp.Format("=%s", pOut->GetCellRef("MVW"));
	pOut->SetUnderLineText    (pXL, szTemp						, m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA5", 3);
	pOut->SetUnderLineText    (pXL, "×"						, m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetUnderLineText    (pXL, "1000000"					, m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA6", 0);
	m_nSttRow++;
	szTemp.Format("=%s", pOut->GetCellRef("IVW_SP"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "Q", "W", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA7", 1);
	m_nSttRow--;
	pOut->SetLineBoxText      (pXL, "×"						, m_nSttRow, "X", "X", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);
	szTemp.Format("=%s", pOut->GetCellRef("YVL"));
	pOut->SetLineBoxText      (pXL, szTemp						, m_nSttRow, "Y", "AA", 2, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA8", 3, FALSE);

	m_nSttRow+=2;
	pOut->SetXL				  (pXL, "="							, m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	szTemp.Format("=%s*%s/%s*%s", pOut->GetCellRef("SP_FL_PARA1"), pOut->GetCellRef("SP_FL_PARA2"), pOut->GetCellRef("SP_FL_PARA3"),pOut->GetCellRef("SP_FL_PARA4"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA9", 3);
	pOut->SetXL				  (pXL, "+"							, m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	szTemp.Format("=%s*%s/%s*%s", pOut->GetCellRef("SP_FL_PARA5"), pOut->GetCellRef("SP_FL_PARA6"), pOut->GetCellRef("SP_FL_PARA7"),pOut->GetCellRef("SP_FL_PARA8"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL_PARA10", 3);
	pOut->SetXL				  (pXL, "="							, m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	szTemp.Format("=%s+%s", pOut->GetCellRef("SP_FL_PARA9"), pOut->GetCellRef("SP_FL_PARA10"));
	pOut->SetXL				  (pXL, szTemp						, m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "SP_FL", 3);
	if(bHybrid)	szTemp.Format("=IF(%s>%s*%s, \"MPa ＞  R·fsa , ∴ N.G\",\"MPa ＜  R·fsa , ∴ O.K\" ", pOut->GetCellRef("SP_FL"), pOut->GetCellRef("ALLOW_TENSION"), pOut->GetCellRef("Hy_R"));
	else		szTemp.Format("=IF(%s>%s, \"MPa ＞  fsa , ∴ N.G\",\"MPa ＜  fsa , ∴ O.K\" ", pOut->GetCellRef("SP_FL"), pOut->GetCellRef("ALLOW_TENSION"));
	pOut->SetXL				  (pXL, szTemp					, m_nSttRow, "P", "X", 0);
	m_nSttRow++;
}

void CXLSpliceCheck::SetLines(int nLine)
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
// ※
///////////////////////////////////////////////////////////////////////////////////// 

void CXLSpliceCheck::SpliceWebPlanStree(CDomyun *pDom, CPlateBasicIndex	*pBx, double dWidth)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();
	CPlateGirderApp		*pGir			= pDB->GetGirder(pBx->GetNumberGirder());	
	CPlateSplice		*pSp			= pBx->GetSplice();
	CPlateStressOut		StressOut(m_pAPlateOutCalcStd->GetGeneralInputStd());
	CAPlateDrawJong		DrawJong(pDataManage);

	long nG		= pBx->GetNumberGirder();
	long nSp	= pBx->GetNumberSpliceEx();	

	CDomyun Dom(pDom);
	DrawJong.DrawWebSplice(&Dom, pBx, 0, 0, 0);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawJong.DrawFlangeLineWeb(&Dom,pBx);
	DrawJong.DimWebSplice(&Dom,pBx,FALSE,TRUE);
	Dom.SetCalcExtRect();
	Dom.Move(-Dom.GetExtRect().right, 0);
	*pDom << Dom;

	CStringArray CalcArr;
	CMap <CString, LPCTSTR, double, double> MapSplice;
	StressOut.SpliceCheckOutput(CalcArr, nG, nSp, MapSplice);
	Dom.SetTextHeight(Dom.Always(0.2));
	
	double dSa		= MapSplice["Safu"];				// 허용 합성 전단력
	double dSu		= MapSplice["Su"];	// 이음판 실제 응력
	double Height	= pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	double dBolt[5];
	CString szBolt = _T("");
	
	for(long nCol=0; nCol<5; nCol++)
	{
		szBolt.Format("BOLT_ALLOW_%d", nCol+1);
		dBolt[nCol] = MapSplice[szBolt];	
	}

	double Lp = 0;
	double Lb = (pGir->GetHeightGirderByStaAng(pBx->GetStation()) - ((pSp->m_mD + pSp->m_mF * pSp->m_mE)*2 + pSp->m_sF*pSp->m_sE + pSp->m_sF1*pSp->m_sE1*2)) / 2;  // 이음판 1,2 사이의 볼트거리
	double d1 = (pSp->m_mD + pSp->m_mE * pSp->m_mF) * 2;
	double d2 = pSp->m_sE * pSp->m_sF + (pSp->m_sE1 * pSp->m_sF1)*2;
	BOOL   bMoment =FALSE;
	long   ni;
	double Hw = pBx->GetHeightGirderByBx();
	double Ho = Hw/2;
	
	CString sStress = _T("");
	sStress.Format("%.1f", max(dSa*0.75, dSu)*1000);
	Dom.TextOut(dWidth+0.32*Height, -50, sStress);

	for (long i = 0; i < 5; i++)
	{
		if (i <= pSp->m_mE && pSp->m_mE >0)	 //모멘트 이음판
		{  
			bMoment = TRUE;			
			if (i == 0)					// 모멘트 이음판 첫줄							
				Lp += pSp->m_mD + pSp->m_mF / 2;	
			else if (i == pSp->m_mE)	// 모멘트 이음판 마지막줄				
				Lp += pSp->m_mF / 2 + (Hw - (d1 +d2))/4;	
			else						// 모멘트 이음판 중간줄				
				Lp += pSp->m_mF;					
		}
		else	// 전단 이음판
		{  
			ni = bMoment ? i-pSp->m_mE-1 : i-pSp->m_mE;
			if(pSp->m_sE1 > 0) // sE1 > 0
			{
				if(ni ==0)
					Lp += bMoment ? pSp->m_sF1 /2 + (Hw-(d1+d2))/4 : pSp->m_sF1 / 2 + Lb;
				else if(pSp->m_sE1 > ni)
					Lp += pSp->m_sF1;
				else if(pSp->m_sE1 == ni)
					Lp += pSp->m_sF1/2 +pSp->m_sF/2;
				else
					Lp += pSp->m_sF;
			}
			else // sE1 ==0
			{
				if (ni == 0)	//전단 이음판 첫번째 열
				{
					Lp += bMoment ? (pSp->m_sF /2) + (Hw-(d1 + d2)) /4 : pSp->m_sF / 2 + Lb;
				}
				else if(pSp->m_sE > ni)	// 전단 이음판 중간 열				
					Lp += pSp->m_sF;				
			}	
		}

		if(dBolt[i] < 0)	continue;
		sStress.Format("%.1f",dBolt[i]);
		Dom.LineTo(dWidth, -Lp, dWidth+0.64*(Ho-Lp), -Lp);		
		Dom.TextOut(dWidth+0.64*(Ho-Lp), -Lp-50, sStress);
	}

	Dom.LineTo(dWidth, 0, dWidth, -Height);
	Dom.LineTo(dWidth, 0, dWidth+0.3*Height, 0);
	Dom.LineTo(dWidth, -Height, dWidth-0.3*Height, -Height);
	Dom.LineTo(dWidth+0.3*Height, 0, dWidth-0.3*Height, -Height);

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dWidth+0.3*Height, 0, 0);
	Dom.DimLineTo(-0.5*Height, 0, COMMA(Ho));
	Dom.DimLineTo(-0.5*Height, 0, COMMA(Hw-Ho));
	Dom.DimMoveTo(dWidth+0.3*Height, 0, 1);
	Dom.DimLineTo(-Height, 1, COMMA(Hw));

	*pDom << Dom;
}