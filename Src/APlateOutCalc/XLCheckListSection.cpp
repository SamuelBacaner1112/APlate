// XLCheckListSection.cpp: implementation of the CXLCheckListSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplateoutcalc.h"
#include "XLCheckListSection.h"
#include "APlateOutCalcStd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLCheckListSection::CXLCheckListSection(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_pAPlateOutCalcStd	= pStd;
}

CXLCheckListSection::~CXLCheckListSection()
{

}

void CXLCheckListSection::SectionSummary()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CAPlateOutputDlg	*pDlg			= m_pAPlateOutCalcStd->GetAPlateOutputDlg();

	int	nPic	= pDlg->m_buttonOpt.GetCheck();
	pDlg->m_buttonOpt.SetCheck(1);
	pXL->SetPageDirection(TRUE);

	m_nSttRow = 1;	
	pOut->SetXL(pXL, "구조계산 CHECK LIST",		m_nSttRow, "A", "A", 1, BLACK, 9, TRUE);
	pOut->SetXL(pXL, "5.단면 요약도",			m_nSttRow, "A", "A", 1, BLACK, 9, TRUE);
	
	CString sText;
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		sText.Format("< 거더 %d >", nG+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "C", "F");

		CDomyun	Dom;
		Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom.ClearEtt(TRUE);
		StressCalc.OutputOfMomentDiagram(&Dom, nG, 2.0, 200.0, NULL, NULL, 0, TRUE);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 0, 10, 64, 40, TRUE, TRUE);	
		SetLines(2);
		
		CDomyun	Dom2;
		Dom2.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom2.ClearEtt(TRUE);
		StressCalc.OutputOfSectionSummary(&Dom2, nG, 1.8, 2.5, 200.0);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom2, m_nSttRow, 0, 10, 64, 40, TRUE, TRUE);	
		SetLines(2);
	}
	pDlg->m_buttonOpt.SetCheck(nPic);
}

void CXLCheckListSection::SectionCheck()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CFEMManage			*pFEMManage		= m_pAPlateOutCalcStd->GetAPlateCalcStd()->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemStressCalc		StressCalc(pDataManage, m_pAPlateOutCalcStd->GetAPlateCalcStd());
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CPlateGirderApp		*pGir			= NULL;
	CPlateBasicIndex	*pBx			= NULL;
	
	long	nPosSec		= 0;
	long	nSttRow		= 1;
	long	nG			= 0;
	long	nNodeIdx	= 0;
	long	nSize		= 0;
	long	nCurlW1		= 0, nCurlW2	= 0;
	double	dFlangeRate	= 100.0;
	double	dWebRate	= 100.0;
	double	dSta		= 0;
	double	Max_up		= 0, Max_lo = 0, Max_upA = 0, Max_loA = 0, Tau_up = 0, Tau_lo = 0, Tau_upA = 0, Tau_loA = 0;
	CString sCell1, sCell2, sText, sCellRow;
	
	pOut->SetXL(pXL, "구조계산 CHECK LIST",				nSttRow, "A", "A", 1, BLACK, 9, TRUE);
	pOut->SetXL(pXL, "4.단면별 응력검토결과",			nSttRow, "A", "A", 1, BLACK, 9, TRUE);
	pOut->SetXL(pXL, "작용응력에 대한 허용응력 검토",	nSttRow, "B", "B", 1, RED,   9, FALSE);

	long nGsu = pDB->GetGirdersu();	
	for(nG = 0 ; nG < nGsu ; nG++)
	{
		sText.Format("< Girder - %d >", nG+1);
		pOut->SetXL(pXL, sText,	nSttRow, "B", "B", 1, BLUE,   9, FALSE);

		pOut->SetLineBoxText(pXL, "구분",					nSttRow, "A", "D", 3, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						nSttRow, "E","AH", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "휨 응 력 검 토",					nSttRow, "E","AB", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(",								nSttRow,"AC","AC", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dFlangeRate,						nSttRow,"AD","AE", 0, BLACK, 9, FALSE, TA_CENTER, "FLANGERATE", 0);
		pOut->SetXL(pXL, "%",								nSttRow,"AF","AF", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, ")",								nSttRow,"AG","AG", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						nSttRow,"AI","AW", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "전 단 응 력 검 토",				nSttRow,"AI","AQ", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(",								nSttRow,"AR","AR", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dWebRate,							nSttRow,"AS","AT", 0, BLACK, 9, FALSE, TA_CENTER, "WEBRATE", 0);
		pOut->SetXL(pXL, "%",								nSttRow,"AU","AU", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, ")",								nSttRow,"AV","AV", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "합 성 응 력 검 토",		nSttRow,"AX","BK", 2, BLACK, 9, FALSE, TA_CENTER);
		nSttRow++;
		pOut->SetLineBoxText(pXL, "상부 플랜지",			nSttRow, "E", "S", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "하부 플랜지",			nSttRow, "T","AH", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "복 부 판",				nSttRow,"AI","AW", 1, BLACK, 9, FALSE, TA_CENTER);
		nSttRow++;
		pOut->SetLineBoxText(pXL, "작용응력",				nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "허용응력",				nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비율",					nSttRow, "O", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판정",					nSttRow, "R", "S", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetLineBoxText(pXL, "작용응력",				nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "허용응력",				nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비율",					nSttRow,"AD","AF", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판정",					nSttRow,"AG","AH", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetLineBoxText(pXL, "작용응력",				nSttRow,"AI","AM", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "허용응력",				nSttRow,"AN","AR", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비율",					nSttRow,"AS","AU", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판정",					nSttRow,"AV","AW", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetLineBoxText(pXL, "상연",					nSttRow,"AX","AZ", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비율",					nSttRow,"BA","BC", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "하연",					nSttRow,"BD","BF", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비율",					nSttRow,"BG","BI", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판정",					nSttRow,"BJ","BK", 1, BLACK, 9, FALSE, TA_CENTER);

		nSttRow++;

		nSize	= pFEMManage->GetFrameSectionSize();
		for(long nMat = 0; nMat < nSize; nMat++)
		{
			nCurlW1 = (long)(double(nMat+nG*nSize)/(double(nSize*nGsu))*60.0);
			sText.Format("4. 단면검토 %d(G%d)", nMat+1, nG+1);
			m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(sText, nCurlW1+nCurlW2);

			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort) return;

			dSta		= 0;
			nNodeIdx	= pModelCalc->GetMaxForcePositionOfSection(nMat+1, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
			if(nNodeIdx<0)	continue;
			double	dStaSec	= pModelCalc->GetStationSecJewon(nMat+1, nG, dSta, nPosSec);

			StressCalc.GetFlangeStress(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA, dStaSec, nPosSec);

			sText.Format("단면 %d", nMat+1);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "A", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("%.3f", Max_up);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER, "UP_FLANGE_ACT", 3);

			sText.Format("%.3f", Max_upA);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER, "UP_FLANGE_ALLOW", 3);

			sText.Format("=ABS(%s/%s)*100", pOut->GetCellRef("UP_FLANGE_ACT"), pOut->GetCellRef("UP_FLANGE_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "O", "Q", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=IF(ABS(%s)>ABS(%s)*%s/100,\"N.G\",\"O.K\")", pOut->GetCellRef("UP_FLANGE_ACT"), pOut->GetCellRef("UP_FLANGE_ALLOW"), pOut->GetCellRef("FLANGERATE"));
			if(fabs(Max_up)>fabs(Max_upA)*dFlangeRate/100)
				pOut->SetLineBoxText(pXL, sText,	nSttRow, "R", "S", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,	nSttRow, "R", "S", 1, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%.3f", Max_lo);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER, "LO_FLANGE_ACT", 3);

			sText.Format("%.3f", Max_loA);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER, "LO_FLANGE_ALLOW", 3);

			sText.Format("=ABS(%s/%s)*100", pOut->GetCellRef("LO_FLANGE_ACT"), pOut->GetCellRef("LO_FLANGE_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,	nSttRow,"AD","AF", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=IF(ABS(%s)>ABS(%s)*%s/100,\"N.G\",\"O.K\")", pOut->GetCellRef("LO_FLANGE_ACT"), pOut->GetCellRef("LO_FLANGE_ALLOW"), pOut->GetCellRef("FLANGERATE"));
			if(fabs(Max_lo)>fabs(Max_loA)*dFlangeRate/100)
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AG","AH", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AG","AH", 1, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%.3f", Tau_up);
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AI","AM", 1, BLACK, 9, FALSE, TA_CENTER, "WEB_ACT");

			sText.Format("%.3f", Tau_upA);
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AN","AR", 1, BLACK, 9, FALSE, TA_CENTER, "WEB_ALLOW");

			sText.Format("=ABS(%s/%s)*100", pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AS","AU", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=IF(ABS(%s)>ABS(%s)*%s/100,\"N.G\",\"O.K\")", pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"), pOut->GetCellRef("WEBRATE"));
			if(fabs(Tau_up)>fabs(Tau_upA)*dWebRate/100)
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AV","AW", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AV","AW", 1, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("=(%s/%s)^2+(%s/%s)^2", pOut->GetCellRef("UP_FLANGE_ACT"), pOut->GetCellRef("UP_FLANGE_ALLOW"), pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AX","AZ", 1, BLACK, 9, FALSE, TA_CENTER, "UP_COMPOSE");

			sText.Format("=%s/1.2*100", pOut->GetCellRef("UP_COMPOSE"));
			pOut->SetLineBoxText(pXL, sText,				nSttRow,"BA","BC", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=(%s/%s)^2+(%s/%s)^2", pOut->GetCellRef("LO_FLANGE_ACT"), pOut->GetCellRef("LO_FLANGE_ALLOW"), pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"BD","BF", 1, BLACK, 9, FALSE, TA_CENTER, "LO_COMPOSE");

			sText.Format("=%s/1.2*100", pOut->GetCellRef("LO_COMPOSE"));
			pOut->SetLineBoxText(pXL, sText,				nSttRow, "BG", "BI", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			double Sum_up = (Max_up/Max_upA)*(Max_up/Max_upA)+(Tau_up/Tau_upA)*(Tau_up/Tau_upA);
			double Sum_lo = (Max_lo/Max_loA)*(Max_lo/Max_loA)+(Tau_up/Tau_upA)*(Tau_up/Tau_upA);

			sText.Format("=IF(OR(%s>1.2,%s>1.2),\"N.G\",\"O.K\")", pOut->GetCellRef("UP_COMPOSE"), pOut->GetCellRef("LO_COMPOSE"));
			if(Sum_up>1.2||Sum_lo>1.2)
				pOut->SetLineBoxText(pXL, sText,			nSttRow,"BJ", "BK", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,			nSttRow,"BJ", "BK", 1, BLACK, 9, FALSE, TA_CENTER);

			nSttRow++;

			//}
		}
	
		long nSpsu = 0;
		double Moment = 0;

		pGir = pDB->GetGirder(nG);
		nSpsu = pGir->GetQtySplice();

		for(long i = 0; i < nSpsu; i++)
		{
			nCurlW2 = (long)(double(i+nG*nSpsu)/(double(nSpsu*nGsu))*20.0);
			sText.Format("4. 현장이음검토 %d(G%d)", i+1, nG+1);
			m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(sText, nCurlW1+nCurlW2);

			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort) return;
			CPlateSplice	*pSp	= NULL;

			pSp		= pGir->m_pSplice.GetAt(i);
			pBx		= pSp->GetBx();
			dSta	= pSp->GetBx()->GetStation();			
			Moment = StressCalc.GetElementForce(ELE_MOMENT3, 0, nG, dSta, 4);

			StressCalc.GetFlangeStress(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA);
			
			sText.Format("SP %d", i+1);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "A", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("%.3f", Max_up);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER, "UP_FLANGE_ACT", 3);

			sText.Format("%.3f", Max_upA);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER, "UP_FLANGE_ALLOW", 3);

			sText.Format("=ABS(%s/%s)*100", pOut->GetCellRef("UP_FLANGE_ACT"), pOut->GetCellRef("UP_FLANGE_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "O", "Q", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=IF(ABS(%s)>ABS(%s)*%s/100,\"N.G\",\"O.K\")", pOut->GetCellRef("UP_FLANGE_ACT"), pOut->GetCellRef("UP_FLANGE_ALLOW"), pOut->GetCellRef("FLANGERATE"));
			if(fabs(Max_up)>fabs(Max_upA)*dFlangeRate/100)
				pOut->SetLineBoxText(pXL, sText,	nSttRow, "R", "S", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,	nSttRow, "R", "S", 1, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%.3f", Max_lo);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER, "LO_FLANGE_ACT", 3);

			sText.Format("%.3f", Max_loA);
			pOut->SetLineBoxText(pXL, sText,	nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER, "LO_FLANGE_ALLOW", 3);

			sText.Format("=ABS(%s/%s)*100", pOut->GetCellRef("LO_FLANGE_ACT"), pOut->GetCellRef("LO_FLANGE_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,	nSttRow,"AD","AF", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=IF(ABS(%s)>ABS(%s)*%s/100,\"N.G\",\"O.K\")", pOut->GetCellRef("LO_FLANGE_ACT"), pOut->GetCellRef("LO_FLANGE_ALLOW"), pOut->GetCellRef("FLANGERATE"));
			if(fabs(Max_lo)>fabs(Max_loA)*dFlangeRate/100)
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AG","AH", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AG","AH", 1, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%.3f", Tau_up);
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AI","AM", 1, BLACK, 9, FALSE, TA_CENTER, "WEB_ACT");

			sText.Format("%.3f", Tau_upA);
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AN","AR", 1, BLACK, 9, FALSE, TA_CENTER, "WEB_ALLOW");

			sText.Format("=ABS(%s/%s)*100", pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AS","AU", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=IF(ABS(%s)>ABS(%s)*%s/100,\"N.G\",\"O.K\")", pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"), pOut->GetCellRef("WEBRATE"));
			if(fabs(Tau_up)>fabs(Tau_upA)*dWebRate/100)
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AV","AW", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,	nSttRow,"AV","AW", 1, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("=(%s/%s)^2+(%s/%s)^2", pOut->GetCellRef("UP_FLANGE_ACT"), pOut->GetCellRef("UP_FLANGE_ALLOW"), pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"AX","AZ", 1, BLACK, 9, FALSE, TA_CENTER, "UP_COMPOSE");

			sText.Format("=%s/1.2*100", pOut->GetCellRef("UP_COMPOSE"));
			pOut->SetLineBoxText(pXL, sText,				nSttRow,"BA","BC", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("=(%s/%s)^2+(%s/%s)^2", pOut->GetCellRef("LO_FLANGE_ACT"), pOut->GetCellRef("LO_FLANGE_ALLOW"), pOut->GetCellRef("WEB_ACT"), pOut->GetCellRef("WEB_ALLOW"));
			pOut->SetLineBoxText(pXL, sText,		nSttRow,"BD","BF", 1, BLACK, 9, FALSE, TA_CENTER, "LO_COMPOSE");

			sText.Format("=%s/1.2*100", pOut->GetCellRef("LO_COMPOSE"));
			pOut->SetLineBoxText(pXL, sText,				nSttRow, "BG", "BI", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

			double Sum_up = (Max_up/Max_upA)*(Max_up/Max_upA)+(Tau_up/Tau_upA)*(Tau_up/Tau_upA);
			double Sum_lo = (Max_lo/Max_loA)*(Max_lo/Max_loA)+(Tau_up/Tau_upA)*(Tau_up/Tau_upA);

			sText.Format("=IF(OR(%s>1.2,%s>1.2),\"N.G\",\"O.K\")", pOut->GetCellRef("UP_COMPOSE"), pOut->GetCellRef("LO_COMPOSE"));
			if(Sum_up>1.2||Sum_lo>1.2)
				pOut->SetLineBoxText(pXL, sText,			nSttRow,"BJ", "BK", 1, RED, 9, FALSE, TA_CENTER);
			else
				pOut->SetLineBoxText(pXL, sText,			nSttRow,"BJ", "BK", 1, BLACK, 9, FALSE, TA_CENTER);

			nSttRow++;
		}		
		nSttRow++;
	}
	sText.Format("BK%d", nSttRow+2);
	pXL->SetPrintArea("A1", sText);
	nSttRow++;	
}

void CXLCheckListSection::SectionUltimate()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	

	CString	sCell1	= _T("");
	CString	sCell2	= _T("");
	CString	sText	= _T("");
	long	nSttRow	= 1;

	//------------------------ 극한응력에 대한 허용응력 검토 ------------------------------

	sCell1.Format("B%d", nSttRow);
	pXL->SetXL(sCell1, "극한응력에 대한 허용응력 검토");
	pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

	nSttRow++;

	long nGsu	= pDB->GetGirdersu();
	long nCurlW1 = 0, nCurlW2 = 0;

	for(long nG = 0 ; nG < nGsu ; nG++)
	{
		sText.Format("< Girder - %d >", nG+1);
		pOut->SetXL(pXL, sText,	nSttRow, "B", "B", 1, BLUE,   9, FALSE);

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 3, 1, 2);//top

		sCell1.Format("B%d", nSttRow);
		pXL->SetXL(sCell1, "구분");//구분

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("D%d", nSttRow+1);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("B%d", nSttRow+1);
		pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("E%d", nSttRow+1);
		pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left

		sCell1.Format("E%d", nSttRow);
		pXL->SetXL(sCell1, "바닥판 CON'C 또는 RE-BAR ");//바닥판 CON'C 또는 RE-BAR 

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("N%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("N%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("O%d", nSttRow);
		pXL->SetXL(sCell1, "상부 플랜지");//상부 플랜지

		sCell1.Format("O%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("X%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right복  부  판

		sCell1.Format("Y%d", nSttRow);
		pXL->SetXL(sCell1, "하부 플랜지");//하부 플랜지

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AH%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AH%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AI%d", nSttRow);
		pXL->SetXL(sCell1, "비        고");//비        고

		sCell1.Format("AI%d", nSttRow);
		sCell2.Format("AP%d", nSttRow+1);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AP%d", nSttRow);
		sCell2.Format("AP%d", nSttRow+1);	
		pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		sCell1.Format("E%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

		sCell1.Format("E%d", nSttRow);
		pXL->SetXL(sCell1, "극한응력");//극한응력

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("H%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("H%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("I%d", nSttRow);
		pXL->SetXL(sCell1, "항복응력");//항복응력

		sCell1.Format("I%d", nSttRow);
		sCell2.Format("L%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("L%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("M%d", nSttRow);
		pXL->SetXL(sCell1, "판정");//판정

		sCell1.Format("M%d", nSttRow);
		sCell2.Format("N%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("N%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right


		sCell1.Format("O%d", nSttRow);
		pXL->SetXL(sCell1, "극한응력");//극한응력

		sCell1.Format("O%d", nSttRow);
		sCell2.Format("R%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("R%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("S%d", nSttRow);
		pXL->SetXL(sCell1, "항복응력");//항복응력

		sCell1.Format("S%d", nSttRow);
		sCell2.Format("V%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("V%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("W%d", nSttRow);
		pXL->SetXL(sCell1, "판정");//판정

		sCell1.Format("W%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("X%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right


		sCell1.Format("Y%d", nSttRow);
		pXL->SetXL(sCell1, "극한응력");//극한응력

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AB%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		pXL->SetXL(sCell1, "항복응력");//항복응력

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AF%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AF%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AG%d", nSttRow);
		pXL->SetXL(sCell1, "판정");//판정

		sCell1.Format("AG%d", nSttRow);
		sCell2.Format("AH%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AH%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		sCell1.Format("B%d", nSttRow-1);
		sCell2.Format("AP%d", nSttRow);
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

		nSttRow++;

		nCurlW1	=	(long)(10.0/nGsu*(nG+1));
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4. 단면검토", (80+nCurlW1+nCurlW2));
		SectionUltimate(nSttRow, nG);

		nCurlW2	=	(long)(10.0/nGsu*(nG+1));
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4. 단면검토", (80+nCurlW1+nCurlW2));
		SectionSplice(nSttRow, nG);

		nSttRow++;

	}	
}

void CXLCheckListSection::SectionUltimate(long &nSttRow, long nG)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp(); 
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CXLControl			*pXL			= m_pXL;
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
	CFemStressCalc		StressCalc(pDataManage, m_pAPlateOutCalcStd->GetAPlateCalcStd());
	CPlateGirderApp		*pGir			= NULL;
	CPlateBasicIndex	*pBx			= NULL;
	CSteel				*pSteelCtl		= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();
	CFEMManage			*pFEMManage		= m_pAPlateOutCalcStd->GetAPlateCalcStd()->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CString sCell1, sCell2, sText, sCellRow, strLeft, strRight;

	BOOL bWind = pCalcData->m_bWind;
	long nNodeIdx = 0;
	long nPosSec = 0; 
	double Moment = 0, dSta = 0;

	long	nSize	= pFEMManage->GetFrameSectionSize(); 

	for(long nMat = 0; nMat < nSize; nMat++)
	{
		if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort) return;

		dSta		= 0;
		nNodeIdx = pModelCalc->GetMaxForcePositionOfSection(nMat+1, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
		if(nNodeIdx<0)	continue;

		pGir = pDB->GetGirder(nG);
		pBx	= pGir->GetBxByStationDir(dSta, 0);

		double	dStaSec	= pModelCalc->GetStationSecJewon(nMat+1, nG, dSta, nPosSec);

		StressCalc.GetSectionStress(nG, dSta, TRUE, dStaSec, nPosSec);
		pModelCalc->GetSectionCoefficient(nG, dStaSec, FALSE, 0, nPosSec);

		Moment = StressCalc.GetMapValueSecStress("MomentPreDead");

		BOOL bPLUS_M = StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);

		double Sigma_ckA;
		if(bPLUS_M)	Sigma_ckA = pCalcData->DESIGN_MATERIAL.m_dSigmaCK*3/5 * StressCalc.GetMapValueSecStress("HyR_Pos");
		else		Sigma_ckA = 400 * StressCalc.GetMapValueSecStress("HyR_Pos");

		sCellRow.Format("B%d", nSttRow);
		sCell2.Format("D%d", nSttRow);
		sText.Format("단면 %d", nMat+1);
		pXL->SetXL(sCellRow, sText);
		pXL->SetMergeCell(sCellRow, sCell2);//조합
	
		CalcGeneral.GetMaterial(pBx, G_F_U, strLeft, strRight);
		
		CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
		CHGAllowStress		AllowYieldUp(pSteel);
		CHGAllowStress		AllowYieldDn(pSteel);
		double	dThickFlangeUp	= pModelCalc->m_MapSecCoe["FLANGE_T_UP"];
		double	dThickFlangeLo	= pModelCalc->m_MapSecCoe["FLANGE_T_DN"];
		double Sigma_YieldUp	= AllowYieldUp.GetAllowStress(ALLOW_STDYIELDPOINT_UP, dThickFlangeUp);
		double Sigma_YieldDn	= AllowYieldDn.GetAllowStress(ALLOW_STDYIELDPOINT_LO, dThickFlangeLo);

		double Sigma_Dead,Sigma_Live,Sigma_Sigma,Sigma_Sigma2;
		double Irv = StressCalc.GetMapValueSecStress("Irv");

		double up_f_C		= pModelCalc->m_MapSecCoe["FLANGE_W_UP"];// MapCoeff["up_f_C"];
		double lo_f_C		= pModelCalc->m_MapSecCoe["FLANGE_W_DN"];
		
		// 크리프, 건조수축 포함시
		//바닥판 콘크리트 또는 Rebar
		
		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vcu_l");

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
			Sigma_Sigma=Sigma_Dead*1.3+Sigma_Live*2.15
								+StressCalc.GetMapValueSecStress("Sigma_cu1") 
								+StressCalc.GetMapValueSecStress("Sigma_cu2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
		}
		else           // 부모멘트
		{
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
			Sigma_Sigma=Sigma_Dead*1.3+Sigma_Live*2.15
								+StressCalc.GetMapValueSecStress("Sigma_cu1") 
								+StressCalc.GetMapValueSecStress("Sigma_cu2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
		}

		sCell1.Format("D%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("H%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("H%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("I%d", nSttRow);
		sCell2.Format("L%d", nSttRow);
		sText.Format("%.3f", Sigma_ckA);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("L%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("M%d", nSttRow);
		sCell2.Format("N%d", nSttRow);
		sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma)>fabs(Sigma_ckA))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("N%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		// 크리프, 건조수축 포함시
		//상부 플랜지

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");
// 		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l");

		double Sigma_vsu = StressCalc.GetMapValueSecStress("Sigma_vsu");
		double Sigma_vsu_l = StressCalc.GetMapValueSecStress("Sigma_vsu_l");
		double Alpha = StressCalc.GetMapValueSecStress("Alpha");
		double fw = StressCalc.GetMapValueSecStress("Fw");
		double dLiveUpperAdd = 0;
		if(Alpha < 10 && (Sigma_vsu+Sigma_vsu_l) > 0)
		{
			dLiveUpperAdd += fw;
		}
		else if(Alpha < 10 && (Sigma_vsu+Sigma_vsu_l) < 0)
		{
			dLiveUpperAdd -= fw;
		}

		Sigma_Live = Sigma_vsu_l + dLiveUpperAdd;

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma=(Sigma_vsu+Sigma_Dead)*1.3
								+Sigma_Live*2.15
								+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		else           // 부모멘트
		{
			double dmsc = StressCalc.GetMapValueSecStress("Msc");
			double dIrv = StressCalc.GetMapValueSecStress("Irv");
			double dYvsu = StressCalc.GetMapValueSecStress("Yvsu");
				
			double dTemp1 = Sigma_vsu;
			double dTemp2 = StressCalc.GetMapValueSecStress("Sigma_vsu1");
			double dTemp3 = StressCalc.GetMapValueSecStress("Sigma_vsu2");
			double dTemp4 = StressCalc.GetMapValueSecStress("Sigma_sut");

			Sigma_Sigma = (dTemp1 + Sigma_Dead)*1.3 + Sigma_Live*2.15 + dTemp2 + dTemp3 - fabs(dTemp4);

		}

		sCell1.Format("O%d", nSttRow);
		sCell2.Format("R%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("R%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("S%d", nSttRow);
		sCell2.Format("V%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldUp);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("V%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("W%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma)>fabs(Sigma_YieldUp))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("X%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right
		
		// 크리프, 건조수축 포함시
		// 하부 플랜지

		double Sigma_vsl = StressCalc.GetMapValueSecStress("Sigma_vsl");
		double Sigma_vsl_l = StressCalc.GetMapValueSecStress("Sigma_vsl_l");

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");
// 		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l"); 
		double dLiveAddLower = 0;
		if(Alpha < 10 && (Sigma_vsl+Sigma_vsl_l) > 0)
		{
			dLiveAddLower += fw;
		}
		else if(Alpha < 10 && (Sigma_vsl+Sigma_vsl_l) < 0)
		{
			dLiveAddLower -= fw;
		}
		Sigma_Live = Sigma_vsl_l + dLiveAddLower;

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma=(Sigma_vsl+Sigma_Dead)*1.3
								+Sigma_Live*2.15
								+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
		}
		else           // 부모멘트
		{
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
								+Sigma_Live*2.15
								+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
		}

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AB%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AF%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldDn);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AF%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AG%d", nSttRow);
		sCell2.Format("AH%d", nSttRow);
		sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma)>fabs(Sigma_YieldDn))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("AH%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AI%d", nSttRow);
		pXL->SetXL(sCell1, "(크리프,건조수축 포함시)");
		pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

		sCell1.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		////////////////////////////////////////
		// 크리프, 건조수축 제외시
		// 바닥판 con'c 또는 re-bar

		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vcu_l");

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
			Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
								+fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
		}
		else           // 부모멘트
		{
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
			Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
								-fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
		}

		sCell1.Format("D%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("H%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma2);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("H%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("I%d", nSttRow);
		sCell2.Format("L%d", nSttRow);
		sText.Format("%.3f", Sigma_ckA);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("L%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("M%d", nSttRow);
		sCell2.Format("N%d", nSttRow);
		sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma2)>fabs(Sigma_ckA))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("N%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		// 크리프, 건조수축 제외시
		// 상부 플랜지

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");
// 		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l");
		Sigma_Live = Sigma_vsu_l + dLiveUpperAdd;

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
								+Sigma_Live*2.15
								+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		else           // 부모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
								+Sigma_Live*2.15
								-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}

		sCell1.Format("O%d", nSttRow);
		sCell2.Format("R%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma2);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("R%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("S%d", nSttRow);
		sCell2.Format("V%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldUp);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("V%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("W%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldUp))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("X%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		// 크리프, 건조수축 제외시
		// 하부플랜지

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");
// 		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l"); 
		Sigma_Live = Sigma_vsl_l + dLiveAddLower;

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
								+Sigma_Live*2.15
								-fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
		}
		else           // 부모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
								+Sigma_Live*2.15
								+fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
		}

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma2);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AB%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AF%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldDn);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AF%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AG%d", nSttRow);
		sCell2.Format("AH%d", nSttRow);
		sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldDn))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("AH%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AI%d", nSttRow);
		pXL->SetXL(sCell1, "(크리프,건조수축 제외시)");
		pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

		sCell1.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		//////////////////////////////////////////////////////
		// 활하중 비재하 시
		// 바닥판 con'c 또는 re-bar

		double Sigma_Wind = 0;
		if(bWind)
		{
			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vcu_w");

			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Wind * 1.3
									+StressCalc.GetMapValueSecStress("Sigma_cu1") 
									+StressCalc.GetMapValueSecStress("Sigma_cu2") 
									+fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
			}
			else           // 부모멘트
			{
				Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Wind * 1.3
									+StressCalc.GetMapValueSecStress("Sigma_cu1") 
									+StressCalc.GetMapValueSecStress("Sigma_cu2") 
									-fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
			}

			sCell1.Format("D%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("H%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("H%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("I%d", nSttRow);
			sCell2.Format("L%d", nSttRow);
			sText.Format("%.3f", Sigma_ckA);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("L%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("M%d", nSttRow);
			sCell2.Format("N%d", nSttRow);
			sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_ckA))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("N%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");

			// 활하중 비재하 시
			// 상부 플랜지

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");
			
			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
									+Sigma_Wind*1.3
									+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
									+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}
			else           // 부모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
									+Sigma_Wind*1.3
									+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
									-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}

			sCell1.Format("O%d", nSttRow);
			sCell2.Format("R%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("R%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("S%d", nSttRow);
			sCell2.Format("V%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldUp);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("V%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("W%d", nSttRow);
			sCell2.Format("X%d", nSttRow);
			sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_YieldUp))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("X%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			// 활하중 비재하 시
			// 하부 플랜지
			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsl_w");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");

			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
									+Sigma_Wind*1.3
									+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
									-fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
			}
			else           // 부모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
									+Sigma_Wind*1.3
									+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
									+fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
			}

			sCell1.Format("Y%d", nSttRow);
			sCell2.Format("AB%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AB%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AC%d", nSttRow);
			sCell2.Format("AF%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldDn);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AF%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AG%d", nSttRow);
			sCell2.Format("AH%d", nSttRow);
			sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_YieldDn))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("AH%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "(활하중 비재하시)");
			pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

			/////////////////////////////////////////////////////////
			// 활하중 재하시
			// 바닥판 con's 또는 re-bar

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vcu_w");
			Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vcu_l");

			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Live * 2.15 + Sigma_Wind / 2.0 * 1.3
									+StressCalc.GetMapValueSecStress("Sigma_cu1") 
									+StressCalc.GetMapValueSecStress("Sigma_cu2") 
									+fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
			}
			else           // 부모멘트
			{
				Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Live * 2.15 + Sigma_Wind / 2.0 * 1.3
									+StressCalc.GetMapValueSecStress("Sigma_cu1") 
									+StressCalc.GetMapValueSecStress("Sigma_cu2") 
									-fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
			}

			sCell1.Format("D%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("H%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("H%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("I%d", nSttRow);
			sCell2.Format("L%d", nSttRow);
			sText.Format("%.3f", Sigma_ckA);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("L%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("M%d", nSttRow);
			sCell2.Format("N%d", nSttRow);
			sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_ckA))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("N%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			// 활하중 재하시
			// 상부 플랜지
			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");
			Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");

			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
									+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
									+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
									+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}
			else           // 부모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
									+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
									+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
									-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}

			sCell1.Format("O%d", nSttRow);
			sCell2.Format("R%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma2);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("R%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("S%d", nSttRow);
			sCell2.Format("V%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldUp);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("V%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("W%d", nSttRow);
			sCell2.Format("X%d", nSttRow);
			sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldUp))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("X%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			// 활하중 재하시
			// 하부 플랜지
			Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l");
			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsl_w");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");

			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
									+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
									+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
									-fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
			}
			else           // 부모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
									+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
									+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
									+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
									+fabs(StressCalc.GetMapValueSecStress("Sigma_slt"));
			}

			sCell1.Format("Y%d", nSttRow);
			sCell2.Format("AB%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma2);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AB%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AC%d", nSttRow);
			sCell2.Format("AF%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldDn);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AF%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AG%d", nSttRow);
			sCell2.Format("AH%d", nSttRow);
			sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldDn))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("AH%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "(활하중 재하시)");
			pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;
		}

		sCell2.Format("D%d", nSttRow-1);
		pXL->SetMergeCell(sCellRow, sCell2);

		sCell2.Format("AH%d", nSttRow-1);
		pXL->SetHoriAlign(sCellRow, sCell2, TA_CENTER);

		sCell2.Format("B%d", nSttRow-1);
		pXL->CellLine(sCellRow, sCell2, 1, 1, 2);//Left
		//}
	}	
}

void CXLCheckListSection::SectionSplice(long &nSttRow, long nG)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CFemStressCalc		StressCalc(pDataManage, m_pAPlateOutCalcStd->GetAPlateCalcStd());
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CXLControl			*pXL			= m_pXL;
	CPlateGirderApp		*pGir			= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx			= NULL;
	CSteel				*pSteelCtl		= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
	
	CString sCell1, sCell2, sText, sCellRow, strLeft, strRight;;

	BOOL bWind = pCalcData->m_bWind;
	long	nSpsu	= pGir->GetQtySplice();
	double	Moment	= 0, dSta = 0;

	for(long i = 0; i < nSpsu; i++)
	{
		if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort) return;

		CPlateSplice		*pSp	= NULL;
		pSp	= pGir->m_pSplice[i];
		pBx = pSp->GetBx();
		dSta = pSp->GetBx()->GetStation();
		Moment = StressCalc.GetElementForce(ELE_MOMENT3, 0, nG, dSta, 4);
		//Moment = pDesignCalc->GetFrameElementForce(ELE_MOMENT3, 0, nG, dSta, 4);

		sCellRow.Format("B%d", nSttRow);
		sCell2.Format("D%d", nSttRow);
		sText.Format("SP %d", i+1);
		pXL->SetXL(sCellRow, sText);
		pXL->SetMergeCell(sCellRow, sCell2);//조합
		
//		CMap <CString, LPCTSTR, double, double> StressCalc.m_MapSecStress;
//		pDesignCalc->GetSectionStress(nG, dSta, StressCalc.m_MapSecStress);
		StressCalc.GetSectionStress(nG, dSta, TRUE);		

		Moment = toTonM(StressCalc.GetMapValueSecStress("Ms"));

		BOOL bPLUS_M = StressCalc.IsPositive(ELE_MOMENT3,nG, dSta);

		double Sigma_ckA;
		if(bPLUS_M)	Sigma_ckA = pCalcData->DESIGN_MATERIAL.m_dSigmaCK*3/5 * StressCalc.GetMapValueSecStress("HyR_Pos");
		else		Sigma_ckA = 4000 * StressCalc.GetMapValueSecStress("HyR_Pos");

		CalcGeneral.GetMaterial(pBx, G_F_U, strLeft, strRight);

		CHGBaseSteel		*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
		CHGAllowStress		AllowYieldUp(pSteel);
		CHGAllowStress		AllowYieldDn(pSteel);
	

		double	dThickFlangeUp	= pModelCalc->m_MapSecCoe["FLANGE_T_UP"];
		double	dThickFlangeLo	= pModelCalc->m_MapSecCoe["FLANGE_T_DN"];
		double Sigma_YieldUp	= AllowYieldUp.GetAllowStress(ALLOW_STDYIELDPOINT_UP, dThickFlangeUp);
		double Sigma_YieldDn	= AllowYieldDn.GetAllowStress(ALLOW_STDYIELDPOINT_LO, dThickFlangeLo);

		double Sigma_Dead = 0, Sigma_Live = 0, Sigma_Sigma = 0, Sigma_Sigma2 = 0;
		double Irv = StressCalc.GetMapValueSecStress("Irv");

		/////////////////////////////////////////
		// 크리프 건조수축 포함 시 sp
		// 바닥판 콘크리트 또는 Rebar
		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vcu_l");
		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma=Sigma_Dead*1.3+Sigma_Live*2.15
				+StressCalc.GetMapValueSecStress("Sigma_cu1") 
				+StressCalc.GetMapValueSecStress("Sigma_cu2") 
				+fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
		}
		else           // 부모멘트
		{
			Sigma_Sigma=Sigma_Dead*1.3+Sigma_Live*2.15
				+StressCalc.GetMapValueSecStress("Sigma_cu1") 
				+StressCalc.GetMapValueSecStress("Sigma_cu2") 
				-fabs(StressCalc.GetMapValueSecStress("Sigma_cut"));
		}
		sCell1.Format("D%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("H%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("H%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("I%d", nSttRow);
		sCell2.Format("L%d", nSttRow);
		sText.Format("%.3f", Sigma_ckA);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("L%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("M%d", nSttRow);
		sCell2.Format("N%d", nSttRow);
		sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma)>fabs(Sigma_ckA))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("N%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		// 크리프 건조수축 포함 시 sp 
		// 상부 플랜지

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");
		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l"); 
		
		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
				+Sigma_Live*2.15
				+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
				+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
				+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		else           // 부모멘트
		{
			double dTemp1 = StressCalc.GetMapValueSecStress("Sigma_vsu");
			double dTemp2 = StressCalc.GetMapValueSecStress("Sigma_vsu1");
			double dTemp3 = StressCalc.GetMapValueSecStress("Sigma_vsu2");
			double dTemp4 = StressCalc.GetMapValueSecStress("Sigma_sut");
			
			Sigma_Sigma = (dTemp1 + Sigma_Dead)*1.3 + Sigma_Live*2.15 + dTemp2 + dTemp3 - fabs(dTemp4);
		}

		sCell1.Format("O%d", nSttRow);
		sCell2.Format("R%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("R%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("S%d", nSttRow);
		sCell2.Format("V%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldUp);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("V%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("W%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma)>fabs(Sigma_YieldUp))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("X%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		// 크리프 건조수축 포함 시 sp
		// 하부 플랜지

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");
		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l"); 
		
		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
				+Sigma_Live*2.15
				+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
				+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
				-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}
		else           // 부모멘트
		{
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
				+Sigma_Live*2.15
				+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
				+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
				+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AB%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AF%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldDn);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AF%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AG%d", nSttRow);
		sCell2.Format("AH%d", nSttRow);
		sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma)>fabs(Sigma_YieldDn))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("AH%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AI%d", nSttRow);
		pXL->SetXL(sCell1, "(크리프,건조수축 포함시)");
		pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

		sCell1.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;


		////////////////////////////////////////
		// 크리프, 건조수축 제외 시 sp
		// 바닥판 con'c 또는 rebar

		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vcu_l");

		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
			Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
				+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
		}
		else           // 부모멘트
		{
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
			Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
				-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
		}

		sCell1.Format("D%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("E%d", nSttRow);
		sCell2.Format("H%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma2);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("H%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("I%d", nSttRow);
		sCell2.Format("L%d", nSttRow);
		sText.Format("%.3f", Sigma_ckA);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("L%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("M%d", nSttRow);
		sCell2.Format("N%d", nSttRow);
		sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma2)>fabs(Sigma_ckA))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("N%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right


		// 크리프, 건조수축 제외 시 sp
		// 상부 플랜지

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");
		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l"); 
		
		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
				+Sigma_Live*2.15
				+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		else           // 부모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
				+Sigma_Live*2.15
				-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}

		sCell1.Format("O%d", nSttRow);
		sCell2.Format("R%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma2);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("R%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("S%d", nSttRow);
		sCell2.Format("V%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldUp);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("V%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("W%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldUp))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("X%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		
		// 크리프, 건조수축 제외 시
		// 하부 플랜지

		Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");
		Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l"); 
		
		if(bPLUS_M)    // 정모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
				+Sigma_Live*2.15
				-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}
		else           // 부모멘트
		{
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
				+Sigma_Live*2.15
				+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}


		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_Sigma2);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AB%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AF%d", nSttRow);
		sText.Format("%.3f", Sigma_YieldDn);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//조합

		sCell1.Format("AF%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AG%d", nSttRow);
		sCell2.Format("AH%d", nSttRow);
		sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldDn))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

		sCell1.Format("AH%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("AI%d", nSttRow);
		pXL->SetXL(sCell1, "(크리프,건조수축 제외시)");
		pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

		sCell1.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		/////////////////////////////////////////
		// 활하중 비재하 시 sp
		// 바닥판 con'c 또는 rebar

		double Sigma_Wind = 0;
		if(bWind)
		{
			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vcu_w");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
							
			
			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Wind * 1.3
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
			}
			else           // 부모멘트
			{
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Wind * 1.3
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
			}

			sCell1.Format("D%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("H%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("H%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("I%d", nSttRow);
			sCell2.Format("L%d", nSttRow);
			sText.Format("%.3f", Sigma_ckA);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("L%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("M%d", nSttRow);
			sCell2.Format("N%d", nSttRow);
			sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_ckA))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("N%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");

			
			// 활하중 비재하 시 sp
			// 상부 플랜지

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");
			
			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
					+Sigma_Wind*1.3
					+StressCalc.GetMapValueSecStress("Sigma_vsu1")
					+StressCalc.GetMapValueSecStress("Sigma_vsu2")
					+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}
			else           // 부모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
					+Sigma_Wind*1.3
					+StressCalc.GetMapValueSecStress("Sigma_vsu1")
					+StressCalc.GetMapValueSecStress("Sigma_vsu2")
					-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}

			sCell1.Format("O%d", nSttRow);
			sCell2.Format("R%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("R%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("S%d", nSttRow);
			sCell2.Format("V%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldUp);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("V%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("W%d", nSttRow);
			sCell2.Format("X%d", nSttRow);
			sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_YieldUp))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("X%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsl_w");

			
			// 활하중 비재하 시 sp
			// 하부 플랜지

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsl_w");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");
			
			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
					+Sigma_Wind*1.3
					+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
					+StressCalc.GetMapValueSecStress("Sigma_vsl2")
					-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
			}
			else           // 부모멘트
			{
				Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
					+Sigma_Wind*1.3
					+StressCalc.GetMapValueSecStress("Sigma_vsl1")
					+StressCalc.GetMapValueSecStress("Sigma_vsl2")
					+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
			}

			sCell1.Format("Y%d", nSttRow);
			sCell2.Format("AB%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AB%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AC%d", nSttRow);
			sCell2.Format("AF%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldDn);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AF%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AG%d", nSttRow);
			sCell2.Format("AH%d", nSttRow);
			sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_YieldDn))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("AH%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "(활하중 비재하시)");
			pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

			/////////////////////////////////////////////////////////
			// 활하중 재하시 sp
			// 바닥판 con'c 또는 rebar
			
			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vcu_w");
			Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vcu_l");
			
			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Live * 2.15 + Sigma_Wind / 2.0 * 1.3
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2")
					+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
			}
			else           // 부모멘트
			{
				Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_vcu");
				Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Live * 2.15 + Sigma_Wind / 2.0 * 1.3
					+StressCalc.GetMapValueSecStress("Sigma_cu1")
					+StressCalc.GetMapValueSecStress("Sigma_cu2")
					-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
			}

			sCell1.Format("D%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("H%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("H%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("I%d", nSttRow);
			sCell2.Format("L%d", nSttRow);
			sText.Format("%.3f", Sigma_ckA);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("L%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("M%d", nSttRow);
			sCell2.Format("N%d", nSttRow);
			sText.Format("=IF(ABS(E%d)>ABS(I%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma)>fabs(Sigma_ckA))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("N%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			
			// 활하중 재하시 sp
			// 상부 플랜지

			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");
			Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_su");
			
			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
					+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
					+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
					+StressCalc.GetMapValueSecStress("Sigma_vsu2")
					+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}
			else           // 부모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsu")+Sigma_Dead)*1.3
					+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
					+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
					+StressCalc.GetMapValueSecStress("Sigma_vsu2")
					-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
			}


			sCell1.Format("O%d", nSttRow);
			sCell2.Format("R%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma2);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("R%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("S%d", nSttRow);
			sCell2.Format("V%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldUp);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("V%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("W%d", nSttRow);
			sCell2.Format("X%d", nSttRow);
			sText.Format("=IF(ABS(O%d)>ABS(S%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldUp))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("X%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right


			// 활하중 재하시 sp
			// 하부 플랜지

			Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l");
			Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsl_w");
			Sigma_Dead = StressCalc.GetMapValueSecStress("Sigma_sl");
			
			if(bPLUS_M)    // 정모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
					+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
					+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
					+StressCalc.GetMapValueSecStress("Sigma_vsl2")
					-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
			}
			else           // 부모멘트
			{
				Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_vsl")+Sigma_Dead)*1.3
					+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
					+StressCalc.GetMapValueSecStress("Sigma_vsl1")
					+StressCalc.GetMapValueSecStress("Sigma_vsl2")
					+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
			}
			
			sCell1.Format("Y%d", nSttRow);
			sCell2.Format("AB%d", nSttRow);
			sText.Format("%.3f", Sigma_Sigma2);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AB%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AC%d", nSttRow);
			sCell2.Format("AF%d", nSttRow);
			sText.Format("%.3f", Sigma_YieldDn);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AF%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AG%d", nSttRow);
			sCell2.Format("AH%d", nSttRow);
			sText.Format("=IF(ABS(Y%d)>ABS(AC%d),\"N.G\",\"O.K\")", nSttRow, nSttRow);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			if(fabs(Sigma_Sigma2)>fabs(Sigma_YieldDn))
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);

			sCell1.Format("AH%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "(활하중 재하시)");
			pXL->SetFonts(sCell1, 7, "굴림", 1, FALSE);

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;
		}

		sCell2.Format("D%d", nSttRow-1);
		pXL->SetMergeCell(sCellRow, sCell2);

		sCell2.Format("AH%d", nSttRow-1);
		pXL->SetHoriAlign(sCellRow, sCell2, TA_CENTER);

		sCell2.Format("B%d", nSttRow-1);
		pXL->CellLine(sCellRow, sCell2, 1, 1, 2);//Left
	}	
}

void CXLCheckListSection::Splice()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CFemStressCalc		StressCalc(pDataManage, m_pAPlateOutCalcStd->GetAPlateCalcStd());
	CXLControl			*pXL			= m_pXL;
	CGeneralBaseStd	*pStd			= m_pAPlateOutCalcStd->m_pGenInputStd;
	CPlateGirderApp		*pGir			= NULL;
	CPlateBasicIndex	*pBx			= NULL;
	CPlateStressOut		StressOut(pStd);

	long nSttRow = 1;

	CString sCell1, sCell2, sText, sCellRow, sJuType;

	sCell1.Format("A%d", nSttRow);
	pXL->SetXL(sCell1, "구조계산 CHECK LIST");
	pXL->SetFonts(sCell1, sCell1, 12, "굴림");

	nSttRow++;

	sCell1.Format("A%d", nSttRow);
	pXL->SetXL(sCell1, "6.현장이음검토");
	pXL->SetFonts(sCell1, sCell1, 9, "굴림");

	nSttRow++;

	long	nGsu		= pDB->GetGirdersu();
	long	nG			= 0;
	long	nSpsu		= 0;
	long 	nCurlW		= 0;
	double	dSta		= 0;
	double Max_up = 0, Max_lo = 0, Max_upA = 0, Max_loA = 0, Tau_up = 0, Tau_lo = 0, Tau_upA = 0, Tau_loA = 0;

	//////////////////////////////////////////////////////////////////////////
	// 거더별 모든 현장이음 출력으로 수정 : 2005.07.09 by kwan
	//////////////////////////////////////////////////////////////////////////
	
	for (nG = 0; nG < nGsu; nG++)   
	{
		pGir = pDB->GetGirder(nG);
		nSpsu = pGir->GetQtySplice(CPlateGirder::SPLICE);

		if (nSpsu < 1) 
		{
			sCell1.Format("A%d", nSttRow);
			sText.Format(" 현장 이음이 없습니다.");
			pXL->SetXL(sCell1, sText);
			pXL->SetFonts(sCell1, sCell1, 9, "굴림");

			break;//모두 현장 용접일 경우....070213...KB...
		}

		sCell1.Format("A%d", nSttRow);
		sText.Format(" < Girder %d >", nG+1);
		pXL->SetXL(sCell1, sText);
		pXL->SetFonts(sCell1, sCell1, 9, "굴림");

		nSttRow++;
		
		for(long i = 0; i < nSpsu; i++)
		{
			nCurlW = (long)((double)(i+1+nG*nSpsu)/(double)(nSpsu*nGsu)*100.0);
			sText.Format("6. 현장이음검토(G%d) %d/%d", nG+1, i+1, nSpsu);
			m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(sText, nCurlW);

			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort) return;
			pBx   = pGir->GetBxBySpliceNoSPType(i);
			//CPlateSplice *pSp = pGir->m_pSplice.GetAt(i);
			CPlateSplice *pSp = pBx->GetSplice();
			//pBx = pSp->GetBx();
			dSta = pBx->GetStation();

			StressCalc.GetFlangeStress(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA);

			CStringArray CalcArr;
			CalcArr.Add ("  <  현장 이음의 설계  >\n");						
			CMap<CString, LPCTSTR, double, double> MapSplice;
			StressOut.SpliceCheckOutput(CalcArr, nG, i, MapSplice);

			double Sa=0, Ra=0;
			double Su=0, n   =0,  na    =0, Ass    =0, As    =0, Ss    =0, R =0, Sft=0;		//상판
			double Sl=0, nl  =0,  nal   =0, Assl   =0, Asl   =0, Ssl   =0, Rl=0, Sfb=0;		//하판			
			double /*SaAr[6],*/ Rb=0, Rw=0, Rwh=0, Swu=0, Swl=0;
			double RbMax = 0;//복부판
			
			BOOL bUp = FALSE;
			BOOL bLo = FALSE;
			BOOL bIsPlus = TRUE;
		
			bIsPlus	= (BOOL)MapSplice["ISPLUS_MOMENT"];
			Sa		= MapSplice["Safu"];
			Ra		= MapSplice["BOLT_ALLOW_STRESS"];//MapSplice["Saw"];
					
			BOOL bTensionUp = (Max_up < 0.0);			
			Su		= MapSplice["Su"];
			n		= MapSplice["REQ_BOLTSU_UP"];
			na		= MapSplice["USE_BOLTSU_UP"];
			Ass		= MapSplice["REQ_AS_UP"];
			As		= MapSplice["USE_AS_UP"];
			Ss		= MapSplice["SPLICE_STRESS_UP"];
			R		= MapSplice["BOLT_ACTING_STRESS_UP"];		
			Rb		= MapSplice["RbMax"];		
			if (bTensionUp)   // 부모멘트부이면 상부 모재응력검토함.
			{
				Sft	= MapSplice["인장모재작용응력_UP"];
				bUp = TRUE;
			}					
			BOOL bTensionLo = (Max_lo < 0.0);			
			Sl		= MapSplice["Sl"];
			nl		= MapSplice["REQ_BOLTSU_DN"];
			nal		= MapSplice["USE_BOLTSU_DN"];
			Assl	= MapSplice["REQ_AS_DN"];
			Asl		= MapSplice["USE_AS_DN"];
			Ssl		= MapSplice["SPLICE_STRESS_DN"];
			Rl		= MapSplice["BOLT_ACTING_STRESS_DN"];			
			if (bTensionLo)	// 정모멘트부이면 하부 모재응력검토함.
			{
				Sfb	= MapSplice["인장모재작용응력_DN"];
				bLo		= TRUE;
			}			
			/*
			SaAr[0]		= MapSplice["1ROW_WEB_SHEAR"];
			SaAr[1]		= MapSplice["2ROW_WEB_SHEAR"];
			SaAr[2]		= MapSplice["3ROW_WEB_SHEAR"];
			SaAr[3]		= MapSplice["4ROW_WEB_SHEAR"];
			SaAr[4]		= MapSplice["5ROW_WEB_SHEAR"];
			*/
			//for (int nLine = 0; nLine < 5; nLine++)
			//	Rb = max(SaAr[nLine], Rb);			

			Rwh			= MapSplice["SHEAR_BOLT_STRESS"];
			Rw			= MapSplice["WEB_COMPOSE_STRESS"];
			Swu			= MapSplice["UP_WEB_STRESS"];
			Swl			= MapSplice["DN_WEB_STRESS"];			
		//------------------------------------------------------------------
			sCellRow.Format("B%d", nSttRow);
			sCell2.Format("E%d", nSttRow);
			sText.Format("SP %d (G%d)", i+1, nG+1);
	//		sText.Format("SP %d", i+1);
			pXL->SetXL(sCellRow, sText);
			pXL->SetMergeCell(sCellRow, sCell2);//조합
			pXL->SetFonts(sCellRow, sCellRow, 9, "굴림", 3, FALSE);

			nSttRow++;

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 3, 1, 2);//top
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("B%d", nSttRow);
			pXL->SetXL(sCell1, "구분");

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("B%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left
			
			sCell1.Format("J%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("J%d", nSttRow);
			pXL->SetXL(sCell1, "상부 플랜지");

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("P%d", nSttRow);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			pXL->SetXL(sCell1, "하부 플랜지");

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("W%d", nSttRow);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right			

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->SetXL(sCell1, "복부판");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("D%d", nSttRow+4);
			pXL->SetXL(sCell1, "볼트");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("B%d", nSttRow+4);
			pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left
			
			sCell1.Format("E%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->SetXL(sCell1, "필요볼트수");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.0f", RoundUp(n,0));
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);
			pXL->SetXL(sCell1, "개");//개

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.f", RoundUp(nl,0));
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);
			pXL->SetXL(sCell1, "개");//개

			sCell1.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "최대휨전단력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AI%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AJ%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", Rb);
   			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AN%d", nSttRow);
			sText.Format("%s", "N/EA");
			pXL->SetXL(sCell1, sText);//개

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------
			
			sCell1.Format("E%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->SetXL(sCell1, "사용볼트수");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.f", na);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);
			pXL->SetXL(sCell1, "개");//개

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.f", nal);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);
			pXL->SetXL(sCell1, "개");//개

			sCell1.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "직접전단력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AI%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("AJ%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", Rwh);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AN%d", nSttRow);			
			pXL->SetXL(sCell1, "N/EA");//개

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------

			sCell1.Format("E%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->SetXL(sCell1, "작용하중/EA");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.2f", R);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1, sCell1, "0.0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);			
			pXL->SetXL(sCell1, "N/EA");//개

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.2f", Rl);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1, sCell1, "0.0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);
			sText.Format("%s", "N/EA");
			pXL->SetXL(sCell1, sText);//개

			sCell1.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right
			
			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "합성전단력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("AJ%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", Rw);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AN%d", nSttRow);
			sText.Format("%s", "N/EA");
			pXL->SetXL(sCell1, sText);//개

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------
			sCell1.Format("E%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("E%d", nSttRow);
			pXL->SetXL(sCell1, "허용하중/EA");
			sCell2.Format("I%d", nSttRow);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.2f", Ra);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);
			sText.Format("%s", "N/EA");
			pXL->SetXL(sCell1, sText);

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.2f", Ra);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);
			sText.Format("%s", "N/EA");
			pXL->SetXL(sCell1, sText);//개

			sCell1.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AI%d", nSttRow);
			pXL->SetXL(sCell1, "허용전단력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("AJ%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", Ra);
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AN%d", nSttRow);
			sText.Format("%s", "N/EA");
			pXL->SetXL(sCell1, sText);//개

			sCell1.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;
		//------------------------------------------------------------

			sCell1.Format("E%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->SetXL(sCell1, "판정");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("J%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("P%d", nSttRow);
			sText.Format("=IF(AND(J%d<=J%d,J%d<=J%d),\"O.K\",\"N.G\")", nSttRow-4, nSttRow-3, nSttRow-2, nSttRow-1);
			pXL->SetXL(sCell1, sText);
			if(n>na||R>Ra)
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("W%d", nSttRow);
			sText.Format("=IF(AND(Q%d<=Q%d,Q%d<=Q%d),\"O.K\",\"N.G\")", nSttRow-4, nSttRow-3, nSttRow-2, nSttRow-1);
			pXL->SetXL(sCell1, sText);
			if(nl>nal||Rl>Ra)
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			sText.Format("=IF(AND(AJ%d<=AJ%d,AJ%d<=AJ%d,AJ%d<=AJ%d),\"O.K\",\"N.G\")", nSttRow-4, nSttRow-1, nSttRow-3, nSttRow-1, nSttRow-2, nSttRow-1);
			pXL->SetXL(sCell1, sText);
			if(!(Rb<=Ra && Rwh<=Ra && Rw<=Ra))
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------

		//---------------- 이음판 ----------------------------------

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("D%d", nSttRow+4);
			pXL->SetXL(sCell1, "이음판");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("B%d", nSttRow+4);
			pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left
			
			sCell1.Format("E%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->SetXL(sCell1, "필요단면적");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.2f", (As));
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0.00");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);			
			pXL->SetXL(sCell1, "㎟");//㎠

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.2f", (Asl));
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0.00");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);
			sText.Format("%s", "㎟");
			pXL->SetXL(sCell1, sText);//㎠

			sCell1.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right
			
			double Tw = pDB->GetGirder(nG)->GetThickJewon (G_W, pBx);
			double Height = pDB->GetGirder(nG)->GetHeightGirderByStaAng(dSta);
			double AsWeb = Tw*Height;

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", (AsWeb));
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0.00");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AN%d", nSttRow);			
			pXL->SetXL(sCell1, "㎟");//㎠

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------
			
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetXL(sCell1, "사용단면적");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.2f", (Ass));//toCm2
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0.00");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);			
			pXL->SetXL(sCell1, "㎟");//㎠

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.2f", (Assl));
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0.00");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);			
			pXL->SetXL(sCell1, "㎟");//㎠

			sCell1.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right			

			double Ts1, Ts2, Ws1, Ws2;
			Ts1 = pSp->m_mT;   // 모멘트 이음판 두께
			Ts2 = pSp->m_sT;   // 전단 이음판 두께
			Ws1 = pSp->m_mW;   // 모멘트 이음판 폭
			Ws2 = pSp->m_sW;   // 전단 이음판 폭

			double SPWebAs = (4*(Ts1*Ws1)+2*(Ts2*Ws2));

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", (SPWebAs));
			pXL->SetXL(sCell1, sText);
			pXL->SetNumberFormat(sCell1,sCell1, "0.00");
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->SetMergeCell(sCell1, sCell2);//조합

			sCell1.Format("AN%d", nSttRow);			
			pXL->SetXL(sCell1, "㎟");//㎠

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------


		//------------------------------------------------------------
			
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetXL(sCell1, "작용응력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.2f", Ss);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);
			sCell2.Format("P%d", nSttRow);			
			pXL->SetXL(sCell1, "MPa");
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.2f", Ssl);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);
			sCell2.Format("W%d", nSttRow);			
			pXL->SetXL(sCell1, "MPa");
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", max(Swu,Swl));
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AN%d", nSttRow);			
			pXL->SetXL(sCell1, "MPa");

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------


		//------------------------------------------------------------
			
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetXL(sCell1, "허용응력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("M%d", nSttRow);
			sText.Format("%.2f", Sa);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("N%d", nSttRow);
			sCell2.Format("P%d", nSttRow);			
			pXL->SetXL(sCell1, "MPa");
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("T%d", nSttRow);
			sText.Format("%.2f", Sa);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("U%d", nSttRow);
			sCell2.Format("W%d", nSttRow);			
			pXL->SetXL(sCell1, "MPa");
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AM%d", nSttRow);
			sText.Format("%.2f", Sa);
			pXL->SetXL(sCell1, sText);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("AN%d", nSttRow);			
			pXL->SetXL(sCell1, "MPa");
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;
		//------------------------------------------------------------


		//------------------------------------------------------------
				
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetXL(sCell1, "판정");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			sText.Format("=IF(AND(J%d<=J%d,J%d<=J%d),\"O.K\",\"N.G\")", nSttRow-4, nSttRow-3, nSttRow-2, nSttRow-1);
			pXL->SetXL(sCell1, sText);
			if(As>Ass||Ss>Sa)
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("W%d", nSttRow);
			sText.Format("=IF(AND(Q%d<=Q%d,Q%d<=Q%d),\"O.K\",\"N.G\")", nSttRow-4, nSttRow-3, nSttRow-2, nSttRow-1);
			pXL->SetXL(sCell1, sText);
			if(Asl>Assl||Ssl>Sa)
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			sText.Format("=IF(AND(X%d<=X%d,X%d<=X%d),\"O.K\",\"N.G\")", nSttRow-4, nSttRow-3, nSttRow-2, nSttRow-1);
			pXL->SetXL(sCell1, sText);
			if(AsWeb>SPWebAs||Swu>Sa)
				pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//--------------------------------------------------------

		//-------------- 모재/응력/검토 ---------------------------

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("D%d", nSttRow);
			pXL->SetXL(sCell1, "모재");//구분
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetXL(sCell1, "작용응력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			if(bUp)
			{
				sCell1.Format("J%d", nSttRow);
				sCell2.Format("M%d", nSttRow);
				sText.Format("%.2f", Sft);
				pXL->SetXL(sCell1, sText);
				pXL->SetMergeCell(sCell1, sCell2);//조합
				pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

				sCell1.Format("N%d", nSttRow);				
				pXL->SetXL(sCell1, "MPa");
			}

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			if(bLo)
			{
				sCell1.Format("Q%d", nSttRow);
				sCell2.Format("T%d", nSttRow);
				sText.Format("%.3f", Sfb);
				pXL->SetXL(sCell1, sText);
				pXL->SetMergeCell(sCell1, sCell2);//조합
				pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

				sCell1.Format("U%d", nSttRow);				
				pXL->SetXL(sCell1, "MPa");
			}

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("E%d", nSttRow);
			sCell2.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("D%d", nSttRow);
			pXL->SetXL(sCell1, "응력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetXL(sCell1, "허용응력");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			if(bUp)
			{
				sCell1.Format("J%d", nSttRow);
				sCell2.Format("M%d", nSttRow);
				sText.Format("%.2f", Sa);
				pXL->SetXL(sCell1, sText);
				pXL->SetMergeCell(sCell1, sCell2);//조합
				pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

				sCell1.Format("N%d", nSttRow);				
				pXL->SetXL(sCell1, "MPa");
			}

			sCell1.Format("P%d", nSttRow);
			pXL->CellLine(sCell1, sCell1, 2, 1, 2);//Right

			if(bLo)
			{
				sCell1.Format("Q%d", nSttRow);
				sCell2.Format("T%d", nSttRow);
				sText.Format("%.3f", Sa);
				pXL->SetXL(sCell1, sText);
				pXL->SetMergeCell(sCell1, sCell2);//조합
				pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

				sCell1.Format("U%d", nSttRow);				
				pXL->SetXL(sCell1, "MPa");
			}

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->SetXL(sCell1, "인장플랜지의 모재응력에 대하여 검토");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right


			sCell1.Format("E%d", nSttRow);
			sCell2.Format("W%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow++;

		//------------------------------------------------------------
			sCell1.Format("B%d", nSttRow);
			sCell2.Format("D%d", nSttRow);
			pXL->SetXL(sCell1, "검토");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
			
			sCell1.Format("E%d", nSttRow);
			sCell2.Format("I%d", nSttRow);
			pXL->SetXL(sCell1, "판정");
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("J%d", nSttRow);
			sCell2.Format("P%d", nSttRow);
			if(bUp)
			{
				sText.Format("=IF(J%d<=J%d,\"O.K\",IF(J%d=\"-\",\"\",\"N.G\"))", nSttRow-2, nSttRow-1, nSttRow-2);
				pXL->SetXL(sCell1, sText);
				if(Sft>Sa)
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			}
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("Q%d", nSttRow);
			sCell2.Format("W%d", nSttRow);
			if(bLo)
			{
				sText.Format("=IF(Q%d<=Q%d,\"O.K\",\"N.G\")", nSttRow-2, nSttRow-1);
				pXL->SetXL(sCell1, sText);
				if(Sfb>Sa)
					pXL->SetFonts(sCell1, sCell1, 9, "굴림", 3, FALSE);
			}
			pXL->SetMergeCell(sCell1, sCell2);//조합
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
			pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

			sCell1.Format("X%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

			sCell1.Format("B%d", nSttRow);
			sCell2.Format("AP%d", nSttRow);
			pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

			nSttRow+=2;	
		}	// for loof - Splice
	}	// for loof - girder


	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("6. 현장이음 검토", 100);
	
}

void CXLCheckListSection::Weld()
{
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();

	m_nSttRow			= 1;

	pXL->SetCellWidth(0, 255, 1);//
	
	pOut->SetXL(pXL, "구조계산 CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "7.용접의 적정성 검토",	m_nSttRow, "A", "A",1, 1, 9,TRUE);	

	pOut->SetXL(pXL, "가) 플랜지와 복부판의 용접부 검토", m_nSttRow, "A", "A");	
	SetLines(NEXT);
	
	FilletWeldMeasureCheck();
	WeldStressCheck();
}


///< 필렛용접 치수 검토
void CXLCheckListSection::FilletWeldMeasureCheck()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();

	CString	sText, sCellRef_Tu, sCellRef_Tl, sCellRef_Tw, sCellRef_Smin, sCellRef_Smax, sCellRef_Suse;
	long	nIndex	= 0;
	long	nG		= 0;
	double	dSta	= 0;
	pOut->SetXL(pXL, "1) 필렛용접 치수 검토",				m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "tu : 상부 플랜지 두께",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "tl : 하부 플랜지 두께",				m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, "tw : 복부판 두께 ",					m_nSttRow, "Z", "Z", 0);
	pOut->SetXL(pXL, "(단위 : mm)",							m_nSttRow, "AK", "AK");

	pOut->SetLineBoxText(pXL, "구 분",						m_nSttRow, "B", "E", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "모재의 사용두께",			m_nSttRow, "F", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "상부 플랜지와 복부판",		m_nSttRow, "O", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "하부 플랜지와 복부판",		m_nSttRow, "Z", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "비 고",						m_nSttRow, "AK", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "tu",							m_nSttRow, "F", "H", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "tl",							m_nSttRow, "I", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "tw",							m_nSttRow, "L", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smax",						m_nSttRow, "O", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smin",						m_nSttRow, "R", "T", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Suse",						m_nSttRow, "U", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",						m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smax",						m_nSttRow, "Z", "AB", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smin",						m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Suse",						m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",						m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
	
	SetLines(NEXT);

	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		nG   = -1;
		dSta = 0;
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		long	nSec	= atoi(pFrameSec->m_szName);

		ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_SHEAR_2, CFemModelingCalc::MAX_ABS, FALSE);
		
		long nPosSec = -1;
		double	dStaSec	= ModelCalc.GetStationSecJewon(nSec, nG, dSta, nPosSec);
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dStaSec);

		double	dTu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
		double	dTl	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
		double	dTw	= pBx->GetFactChain(G_W)->m_dFactChainThick;
		double	dSu	= max (RoundUp (sqrt (2 * max (dTu, dTw)), 0), 6.0);
		double	dSl	= max (RoundUp (sqrt (2 * max (dTl, dTw)), 0), 6.0);
		sText.Format("단면 - %d", nIndex+1);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "B", "E", 1, RED,   9, FALSE, TA_CENTER);
		///< tu
		sCellRef_Tu.Format("TU_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTu,						m_nSttRow, "F", "H", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tu, 0);
		///< tl
		sCellRef_Tl.Format("TL_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTl,						m_nSttRow, "I", "K", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tl, 0);
		///< tw
		sCellRef_Tw.Format("TW_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTw,						m_nSttRow, "L", "N", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tw, 0);
		///< Smax - 상부
		sText.Format("=MIN(%s,%s)", pOut->GetCellRef(sCellRef_Tu), pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smax.Format("UPPER_S_MAX_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "O", "Q", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smax,1);
		///< Smin - 상부
		sText.Format("=IF(%s<8,SQRT(2*MAX(%s,%s)),MAX(6,SQRT(2*MAX(%s,%s))))",
					pOut->GetCellRef(sCellRef_Smax),pOut->GetCellRef(sCellRef_Tu),pOut->GetCellRef(sCellRef_Tw),pOut->GetCellRef(sCellRef_Tu),pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smin.Format("UPPER_S_MIN_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "R", "T", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smin,1);
		///< Suse - 상부
		sCellRef_Suse.Format("UPPER_SUSE_UP_%d", nIndex);
		pOut->SetLineBoxText(pXL, dSu,						m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Suse, 0);
		///< 판정
		sText.Format("=IF(AND(%s<=%s,%s>=%s),\"OK\",\"NG\")", pOut->GetCellRef(sCellRef_Smin), pOut->GetCellRef(sCellRef_Suse),
					pOut->GetCellRef(sCellRef_Smax), pOut->GetCellRef(sCellRef_Suse));
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
		///< Smax - 하부
		sText.Format("=MIN(%s,%s)", pOut->GetCellRef(sCellRef_Tl), pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smax.Format("LOWER_S_MAX_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "Z", "AB", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smax,1);
		///< Smin - 하부
		sText.Format("=IF(%s<8,SQRT(2*MAX(%s,%s)),MAX(6,SQRT(2*MAX(%s,%s))))",
					pOut->GetCellRef(sCellRef_Smax),pOut->GetCellRef(sCellRef_Tl),pOut->GetCellRef(sCellRef_Tw),pOut->GetCellRef(sCellRef_Tl),pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smin.Format("LOWER_S_MIN_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smin,1);
		///< Suse - 하부
		sCellRef_Suse.Format("LOWER_SUSE_UP_%d", nIndex);
		pOut->SetLineBoxText(pXL, dSl,						m_nSttRow, "AF", "AH", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Suse, 0);
		///< 판정
		sText.Format("=IF(AND(%s<=%s,%s>=%s),\"OK\",\"NG\")", pOut->GetCellRef(sCellRef_Smin), pOut->GetCellRef(sCellRef_Suse),
					pOut->GetCellRef(sCellRef_Smax), pOut->GetCellRef(sCellRef_Suse));
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AK", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
		nIndex++;
	}

	SetLines(NEXT);
}
///< 용접부의 응력 검토
void CXLCheckListSection::WeldStressCheck()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();


	CString	sText;
	pOut->SetXL(pXL, "2) 용접부의 응력 검토",				m_nSttRow, "B", "B", 1);

	CString	sFlange, sSmax, sTu, sB, sA, sYvsu, sQ, sIv, sS, sSigmaA, sNu, sANu, sJuType;
	long	nIndex			= 0;
	double	dSmax			= 0;
	double	dTu				= 0;
	double	dWf				= 0;
	double	dYvsu			= 0;
	double	dIv				= 0;
	double	dS				= 0;
	double	dTw				= 0;
	double	dANu			= 0;
	
	for(long n=0; n < 2; n++)
	{
		if(n==0)
		{
			pOut->SetXL(pXL, "① 상부 플랜지와 복부판",				m_nSttRow, "B", "B");
			sFlange = "UPPER";
		}
		else
		{
			sFlange = "LOWER";
			pOut->SetXL(pXL, "② 하부 플랜지와 복부판",				m_nSttRow, "B", "B");
		}
		nIndex = 0;
		pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "E", "H", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "J", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "K", "M", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "N", "Q", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "R", "T", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "U", "X", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AC", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AD", "AE", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AF", "AH", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AI", "AK", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AN", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비고",					m_nSttRow,"AO","AP", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Smax",							m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		if(n==0)
		{
			pOut->SetXL(pXL, "tu",								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Bu",								m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "A",								m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Yvsu",							m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		else
		{
			pOut->SetXL(pXL, "tl",								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Bl",								m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "A",								m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Yvsl",							m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		pOut->SetXL(pXL, "Q",								m_nSttRow, "U", "X", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Iv",								m_nSttRow, "Y", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "s",								m_nSttRow, "AD", "AE", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Σa",								m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "υ",								m_nSttRow, "AI", "AK", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "υa",								m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "(kN)",							m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "I", "J", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm²)",							m_nSttRow,  "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm³)",							m_nSttRow, "U", "X", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm⁴)",							m_nSttRow, "Y", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "AD", "AE", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(MPa)",						m_nSttRow, "AI", "AK", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(MPa)",						m_nSttRow, "AL", "AN", 1, BLACK, 8, FALSE, TA_CENTER);

		double dSmaxTmp=0;
		long   nG		= 0;
		double dSta		= 0;
		double dStaSec	= 0;
		POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
		while(pos)
		{
			nG   = -1;
			dSta = 0;
			CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
			if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

			long	nPosSec	= -1;
			long	nSec	= atoi(pFrameSec->m_szName);
			ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_SHEAR_2, CFemModelingCalc::MAX_ABS, FALSE);
			dStaSec	= ModelCalc.GetStationSecJewon(nSec, nG, dSta, nPosSec);

			BOOL bPositiveM = StressCalc.IsPositive(ELE_MOMENT3, nG, dSta); 
			CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
			CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dStaSec);
			ModelCalc.GetSectionCoefficient(nG, dStaSec, FALSE, 0, nPosSec);
			StressCalc.GetSectionStress(nG, dSta, TRUE, dStaSec, nPosSec);

			dSmax		= fabs(StressCalc.GetElementForce(ELE_SHEAR_2, 0, nG, dSta, 4));	///< 최대전단력
			dTw			= pBx->GetFactChain(G_W)->m_dFactChainThick;
			sJuType		= pDB->GetMaterial(G_F_U);
			CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
			CHGAllowStress	AllowStress(pSteel);
			dANu		= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dTw);

			if(n==0)
			{
				if(bPositiveM)
					dYvsu = fabs(ModelCalc.GetMapValueSecCo("Yvsu"));
				else
					dYvsu = fabs(StressCalc.GetMapValueSecStress("Yvsu"));
				dTu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
				dWf	= pGir->GetWidthOnStation(dSta, TRUE);
				if(dSmaxTmp < dSmax)
				{
					dSmaxTmp	= dSmax;
					m_pVStiffBx	= pBx;
					m_nG		= nG;
				}
				m_nSectionSize++;
	
			}
			else
			{
				if(bPositiveM)
					dYvsu = fabs(ModelCalc.GetMapValueSecCo("Yvsl"));
				else
					dYvsu = fabs(StressCalc.GetMapValueSecStress("Yvsl"));
				dTu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
				dWf	= pGir->GetWidthOnStation(dSta, FALSE);
			}

			dS = pDataManage->GetFilletWeldStandard(dTu, dTw);
			
			if(bPositiveM)	dIv = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
			else			dIv = StressCalc.GetMapValueSecStress("Irv");
			CString	sCell1, sCell2;
			sText.Format("단면-%d", nIndex+1);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "B", "D", 1, RED,   9, FALSE, TA_CENTER);
			///< Smax
			sSmax.Format("SMAX_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dSmax,					m_nSttRow, "E", "H", 1, RED,   9, FALSE, TA_CENTER, sSmax, 1);
			///< tu
			sTu.Format("TU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dTu,						m_nSttRow, "I", "J", 1, RED,   9, FALSE, TA_CENTER, sTu, 0);
			///< B
			sB.Format("B_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dWf,						m_nSttRow, "K", "M", 1, RED,   9, FALSE, TA_CENTER, sB, 1);
			///< A
			sText.Format("=ROUND(%s*%s,1)", pOut->GetCellRef(sTu), pOut->GetCellRef(sB));
			sA.Format("A_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "N", "Q", 1, BLACK, 9, FALSE, TA_CENTER, sA, 1);
			///< Yvsu
			sYvsu.Format("YVSU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dYvsu,					m_nSttRow, "R", "T", 1, RED,   9, FALSE, TA_CENTER, sYvsu, 1);
			///< Q
			sCell1.Format("M%d", m_nSttRow);	sCell2.Format("M%d", m_nSttRow);
			sText.Format("=ROUND(%s*(%s-%s),1)", pOut->GetCellRef(sA), pOut->GetCellRef(sYvsu), pOut->GetCellRef(sTu));
			sQ.Format("Q_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "X", 1, BLACK, 9, FALSE, TA_CENTER, sQ, 0);
			pXL->SetNumberFormat(sCell1, sCell2, "0.0E+00");
			///< Iv
			sCell1.Format("O%d", m_nSttRow);	sCell2.Format("Q%d", m_nSttRow);
			sIv.Format("IV_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dIv,						m_nSttRow, "Y", "AC", 1, RED,   9, FALSE, TA_CENTER, sIv, 0);
			pXL->SetNumberFormat(sCell1, sCell2, "0.000E+00");
			///< s
			sS.Format("S_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dS,						m_nSttRow, "AD", "AE", 1, RED,   9, FALSE, TA_CENTER, sS, 0);
			///< Σa
			sText.Format("=ROUND(2*%s/SQRT(2),1)", pOut->GetCellRef(sS));
			sSigmaA.Format("SIGMAA_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER, sSigmaA, 2);
			///< υ
			sText.Format("=ROUND(%s*10^3*%s/(%s*%s),1)", pOut->GetCellRef(sSmax), pOut->GetCellRef(sQ), pOut->GetCellRef(sIv), pOut->GetCellRef(sSigmaA));
			sNu.Format("NU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER, sNu, 1);
			///< υa
			sANu.Format("ANU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dANu,						m_nSttRow, "AL", "AN", 1, RED,   9, FALSE, TA_CENTER, sANu, 0);
			///< 비고
			sText.Format("=IF(%s>%s,\"NG\",\"OK\")", pOut->GetCellRef(sNu), pOut->GetCellRef(sANu));
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow,"AO","AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			nIndex++;
		}
		SetLines(1);
	}

}

void CXLCheckListSection::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}