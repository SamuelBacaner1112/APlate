// XLCheckListStud.cpp: implementation of the CXLCheckListStud class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplateoutcalc.h"
#include "XLCheckListStud.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLCheckListStud::CXLCheckListStud(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL = pAPlateOutCalcXL;
}

CXLCheckListStud::~CXLCheckListStud()
{

}

void CXLCheckListStud::CheckStud()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);	///< 합성후 고정하중
	CPlateDesigCalc		DesignCalc(pDataManage, pCalcStd);

	m_nSttRow = 1;
	CString	sText		= _T("");
	long	nGsu		= pBridgeApp->GetGirdersu();
	long	nSelectnG	= 0;
	double	dSlabWidth	= 0;
	double	dMaxWidth	= 0;
	
	// 슬래브 폭을 비교하여 가장 큰 거더만 선택
	for(long nG = 0; nG < nGsu; nG++)
	{
		dSlabWidth = pModelCalc->GetMaxSlabWidth(nG, 0);

		if(nG==0) 
		{
			dMaxWidth = dSlabWidth;
			nSelectnG = nG;
		}
		if(dSlabWidth > dMaxWidth)
		{
			dMaxWidth = dSlabWidth;
			nSelectnG = nG;
		}
	}

	long nSttJoint, nEndJoint;
	pModelCalc->GetJointRangeByGirder(nSelectnG, nSttJoint, nEndJoint);	

	pOut->SetXL(pXL, "구조계산 CHECK LIST",	m_nSttRow, "A", "A", 1, BLACK, 11, TRUE);
	pOut->SetXL(pXL, "10.전단연결재 검토",	m_nSttRow, "A", "A", 1, BLACK, 9, FALSE);
	pOut->SetXL(pXL, " 가) 위치별 배치간격 검토 ",	m_nSttRow, "A", "A");

	pOut->SetLineBoxText(pXL, "절점",		m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "하    중     조     합      ( N/mm )",		m_nSttRow, "E","AC", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow,"AD","AG", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "소요간격",			m_nSttRow,"AD","AG", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "배치간격",	m_nSttRow,"AH","AK", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판    정",	m_nSttRow,"AL","AP", 2, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "Sv",			m_nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Ss",			m_nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "St",			m_nSttRow, "O", "S", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Sv+St",		m_nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Ss+St",		m_nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "do=Qa'/S ",			m_nSttRow,"AD","AG", 1, BLACK, 9, FALSE, TA_CENTER);

	CString sSv, sSs, sSt, sSvSt, sSsSt, sUsePitch;
	CString	sSttCell, sEndCell;
	
	for(long nJoint = nSttJoint; nJoint < nEndJoint+1; nJoint++)
	{
		if(m_pAPlateOutCalcStd->GetAPlateOutExcelCheck()->m_bAbort) return;
		
		CPlateBasicIndex *pBx = pCalcStd->GetBxByJoint(pFEMManage->GetJoint((unsigned short)nJoint));

		CMap <CString, LPCTSTR, double, double> Map;
		DesignCalc.GetMapStud(nJoint, Map);
		BOOL isEnd = FALSE;

		if (nJoint == nEndJoint)
			isEnd = TRUE;

		double	dQa	= Map["Qa"];
		double	dSv	= Map["Sv"];
		double	dSs	= Map["Ss"];
		double	dSt	= Map["St"];
		double	dDistance	= (pModelCalc->GetUseBasePitchByStation(nSelectnG, pBx->GetStation(),isEnd));
		
		// 절점 번호
		sText.Format("%d", nJoint+1);
		pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

		if(nJoint == nSttJoint || nJoint == nEndJoint)
		{
			sText.Format("%lf", dSv);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER, "Sv");
			sText.Format("%lf", dSs);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER, "Ss");
			sText.Format("%lf", dSt);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "O", "S", 1, BLACK, 9, FALSE, TA_CENTER, "St");
			sText.Format("=%s+%s", pOut->GetCellRef("Sv"), pOut->GetCellRef("St"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER, "Sv_St");
			sText.Format("=%s+%s", pOut->GetCellRef("Ss"), pOut->GetCellRef("St"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER, "Ss_St");
			sText.Format("=%.3f/MAX(%s, %s)", dQa, pOut->GetCellRef("Sv_St"), pOut->GetCellRef("Ss_St"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AD","AG", 1, BLACK, 9, FALSE, TA_CENTER, "REQ_SPACE", 1);
			sText.Format("%lf", dDistance);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AH","AK", 1, BLACK, 9, FALSE, TA_CENTER, "USE_SPACE", 1);
			sText.Format("=IF(%s>=%s, \"O.K\", \"N.G\")", pOut->GetCellRef("REQ_SPACE"), pOut->GetCellRef("USE_SPACE"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AL","AP", 1, BLACK, 9, FALSE, TA_CENTER);

			if(nJoint == nSttJoint)	sSttCell.Format("AH%d", m_nSttRow);
			if(nJoint == nEndJoint)	sEndCell.Format("AH%d", m_nSttRow);
		}
		else
		{
			sText.Format("%lf", dSv);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER, "Sv");
			pOut->SetLineBoxText(pXL, 0.0,			m_nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER, "Ss");
			pOut->SetLineBoxText(pXL, 0.0,			m_nSttRow, "O", "S", 1, BLACK, 9, FALSE, TA_CENTER, "St");
			sText.Format("=%s+%s", pOut->GetCellRef("Sv"), pOut->GetCellRef("St"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER, "Sv_St");
			sText.Format("=%s+%s", pOut->GetCellRef("Ss"), pOut->GetCellRef("St"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER, "Ss_St");
			sText.Format("=%.3f/MAX(%s, %s)", dQa, pOut->GetCellRef("Sv_St"), pOut->GetCellRef("Ss_St"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AD","AG", 1, BLACK, 9, FALSE, TA_CENTER, "REQ_SPACE", 1);
			sText.Format("%lf", dDistance);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AH","AK", 1, BLACK, 9, FALSE, TA_CENTER, "USE_SPACE", 1);
			sText.Format("=IF(%s>=%s, \"O.K\", \"N.G\")", pOut->GetCellRef("REQ_SPACE"), pOut->GetCellRef("USE_SPACE"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AL","AP", 1, BLACK, 9, FALSE, TA_CENTER);
		}

		m_nSttRow++;
	}

	m_nSttRow++;
	
	CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nSelectnG);
	CPlateBasicIndex	*pBx	= pCalcStd->GetBxByJoint(pFEMManage->GetJoint((unsigned short)nSttJoint));

	double	dD	= (pGir->m_pSangse->m_StudHori_D);
	double	dL2	= (pGir->m_pSangse->m_StudHori_Dis[1]);
	//070226...일자 수정...KB...
	double	dThickHunch	= pBridgeApp->m_dThickSlabHunch;
	double	dSlabThick	= pBridgeApp->m_dThickSlabBase-dThickHunch;
	double  dALLOW_CENT_MAX = MIN(3*dSlabThick, 600);//교축방향 간격 최대...

	double dL1_N = 0;//pGir->m_pSangse->m_StudHori_Dis[0];
	long	nQtyStud	= pGir->m_pSangse->m_StudHori_Qty;
	for(long nStud=0; nStud<nQtyStud-1; nStud++)
		dL1_N += pGir->m_pSangse->m_StudHori_Dis[nStud];	
	double dDist	= (pGir->GetWidthFlangeUpper(pBx)-dL1_N)/2.0;

	pOut->SetXL(pXL, " 나) 배치간격규정 검토",	m_nSttRow, "A", "A", 1);	
	pOut->SetLineBoxText(pXL, "교축방향 간격 (mm)",	m_nSttRow, "B", "U", 1, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetLineBoxText(pXL, "교축직각방향 간격 (mm)",	m_nSttRow, "V","AP", 1, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "최대",		m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용",		m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",		m_nSttRow, "J", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "최소",		m_nSttRow, "L", "O", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용",		m_nSttRow, "P", "S", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",		m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "최소",		m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용",		m_nSttRow, "Z","AC", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",		m_nSttRow,"AD","AE", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "최소연단",	m_nSttRow,"AF","AI", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용",		m_nSttRow,"AJ","AM", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",		m_nSttRow,"AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow++;
	sText.Format("%lf", dALLOW_CENT_MAX);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "ALLOW_CENT_MAX", 1);
	sText.Format("=MAX(%s:%s)", sSttCell, sEndCell);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER, "USE_CENT_MAX", 1);
	sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("USE_CENT_MAX"), pOut->GetCellRef("ALLOW_CENT_MAX"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "J", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	if(5.0*dD > 100.0)
		sText.Format("%.3f", 5.0*dD);
	else
		sText.Format("%.3f", 100.0);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "L", "O", 1, BLACK, 9, FALSE, TA_CENTER, "ALLOW_CENT_MIN", 1);
	sText.Format("=MIN(%s:%s)", sSttCell, sEndCell);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "P", "S", 1, BLACK, 9, FALSE, TA_CENTER, "USE_CENT_MIN", 1);
	sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("USE_CENT_MIN"), pOut->GetCellRef("ALLOW_CENT_MIN"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", dD+30.0);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "ALLOW_RA", 1);
	sText.Format("%lf", dL2);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "Z","AC", 1, BLACK, 9, FALSE, TA_CENTER, "USE_RA", 1);
	sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("ALLOW_RA"), pOut->GetCellRef("USE_RA"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AD","AE", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%lf", 25.0);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AF","AI", 1, BLACK, 9, FALSE, TA_CENTER, "ALLOW_MIN_Y", 1);
	sText.Format("%.3f", dDist-dD/2.0);
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AJ","AM", 1, BLACK, 9, FALSE, TA_CENTER, "USE_Y", 1);
	sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("ALLOW_MIN_Y"), pOut->GetCellRef("USE_Y"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow,"AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);		
}
