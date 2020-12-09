// XLDesignStud.cpp: implementation of the CXLDesignStud class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcStd.h"
#include "XLDesignStud.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

CXLDesignStud::CXLDesignStud(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
}

CXLDesignStud::~CXLDesignStud()
{

}

void CXLDesignStud::SquareSHConnector()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CPlateGirderApp		*pGir			= NULL;
	CPlateBasicIndex	*pBx			= NULL;

	m_nSttRow = 4;

	double dSta			= 0;
	double dSlabWidth	= 0;
	double dMaxWidth	= 0;
	long	nSelectnG	= 0;
	CString sText		= _T("");
	long nG = 0;
	for(nG = 0; nG < pDB->GetQtyGirder(); nG++)
	{
		dSlabWidth = pModelCalc->GetMaxSlabWidth(nG, 0);

		if(dSlabWidth > dMaxWidth)
		{
			dMaxWidth = dSlabWidth;
			nSelectnG = nG;
		}
	}
	nG		= nSelectnG;
	pGir	= pDB->GetGirder(nG);
	dSta	= pGir->GetBxOnJijum(0)->GetStation() + pDB->m_dLengthJigan[0]/5;

	pBx		= pGir->GetBxByStation(dSta);	
	dSta	= pBx->GetStation();

	CPlateSangSeDo	*pSang = pGir->m_pSangse;
	
	for(long n=0; n<2; n++)
	{
		if(n!=0)
			dSta = pGir->GetBxOnJijum(0)->GetStation();

		pModelCalc->GetSectionCoefficient(nG, dSta);
		pStressCalc->GetSectionStress(nG, dSta);

		// phase2 ~ phase4
		double	dApNc	= pModelCalc->GetMapValueSecCo("ApNc");			// 구속콘크리트의 환산단면적
		double	dYcu2	= pStressCalc->GetMapValueSecStress("Yvcu2");	// 중립축에서 구속콘크리트 상부까지 거리
		double	dYcl2	= pStressCalc->GetMapValueSecStress("Yvcl2");	// 중립축에서 구속콘크리트 하부까지 거리
		double	dYcm2	= (dYcu2+dYcl2)/2;								// 중립축에서 구속콘크리트 도심까지 거리

		// phase5 ~ phase6
		double	dYcu3	= pStressCalc->GetMapValueSecStress("Yvcu");	// 중립축에서 구속콘크리트 상부까지 거리
		double	dYcl3	= pStressCalc->GetMapValueSecStress("Yvcl");	// 중립축에서 구속콘크리트 하부까지 거리
		double	dYcm3	= (dYcu3+dYcl3)/2;								// 중립축에서 구속콘크리트 도심까지 거리
		double	dApNd	= pModelCalc->GetMapValueSecCo("ApNd");			// 바닥판의 환산단면적
		double	dYdu3	= pStressCalc->GetMapValueSecStress("Yvdu");	// 중립축에서 바닥판 상부까지 거리
		double	dYdl3	= pStressCalc->GetMapValueSecStress("Yvdl");	// 중립축에서 바닥판 하부까지 거리
		double	dYdm3	= (dYdu3+dYdl3)/2;								// 중립축에서 바닥판 도심까지 거리
		
		//TurnOver거더교 일 때는 구속 콘크리트 합성단면의 값을 출력함. LSH 200903
		double	dI33	= pDB->IsTUGir() ? pModelCalc->GetMapValueSecCo("Ivc33") : pModelCalc->GetMapValueSecCo("I33");	// 바닥판 합성전

		double	dIvd33	= pModelCalc->GetMapValueSecCo("Ivd33");		// 바닥판 합성후
		// 각 경우별 전단력
		double	dSph2	= tokN(fabs(pStressCalc->GetMapValueSecStress("Sph2")));
		double	dSph3	= tokN(fabs(pStressCalc->GetMapValueSecStress("Sph3")));
		double	dSph4	= tokN(fabs(pStressCalc->GetMapValueSecStress("Sph4")));
		double	dSph5	= tokN(fabs(pStressCalc->GetMapValueSecStress("Sph5")));
		double	dSph6	= tokN(fabs(pStressCalc->GetMapValueSecStress("Sph6_1")));

		double	dFck	= pModelCalc->GetMapValueSecCo("BindFck") * 10;	// CGS

		double	dFU_W	= pModelCalc->GetMapValueSecCo("FLANGE_W_UP");	// 상부플랜지 폭
		double	dFU_T	= pModelCalc->GetMapValueSecCo("FLANGE_T_UP");	// 상부플랜지 두께
		double	dSqSH_W	= pSang->m_StudHoriSQ_L;						// 각형 전단연결재 길이
		double	dSqSH_H	= pSang->m_StudHoriSQ_H;						// 각형 전단연결재 두께

		double	dDis	= dSta - pGir->GetStationStt();
		double	dDisStud= 0;	

		for(long nStud=0; nStud<STUD_JONG_SU; nStud++)
		{
			dDisStud += pSang->m_StudJong_StudDis[nStud]*pSang->m_StudJong_Studsu[nStud];
			if(dDisStud > dDis)
			{
				dDisStud = pSang->m_StudJong_StudDis[nStud];
				break;
			}
		}

		CDPoint A[10];
 		pGir->GetHunchXyDom(pBx, A, FALSE, FALSE, TRUE);
		double	dH1		= pModelCalc->GetMapValueSecCo("BindConThick") - pModelCalc->GetMapValueSecCo("BindConDis");
		double	dH2		= pGir->GetElevationPaveAct(pBx->GetStation(), 0) - A[2].y;
		
		if(n==0)
		{
			sText.Format("일반부 (지점에서 %.fm 떨어진 위치)", toM(dDis));
			pOut->SetXL(pXL, sText,		m_nSttRow, "B", "B", 0);
		}		
		m_nSttRow += 12;

		pOut->SetXL(pXL, dApNc,		m_nSttRow, "S", "V", 1, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dYcm2,		m_nSttRow, "S", "V", 4, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dYcm3,		m_nSttRow, "S", "V", 1, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dApNd,		m_nSttRow, "S", "V", 1, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dYdm3,		m_nSttRow, "S", "V", 4, RED, 9, FALSE, TA_CENTER, "", 0);

		pOut->SetXL(pXL, dI33,		m_nSttRow, "M", "R", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dSph2,		m_nSttRow, "S", "X", 1, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dSph3,		m_nSttRow, "S", "X", 1, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dSph4,		m_nSttRow, "S", "X", 1, RED, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, dIvd33,	m_nSttRow, "M", "R", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dSph5,		m_nSttRow, "S", "X", 1, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dSph6,		m_nSttRow, "S", "X", 5, RED, 9, FALSE, TA_CENTER);

		if(n!=0)	m_nSttRow++;

		// 상부플랜지 허용 부착강도
		pOut->SetXL(pXL, dFck,		m_nSttRow, "I", "J", 3, RED, 9, FALSE, TA_CENTER, "", 0);
		
		pOut->SetXL(pXL, dFU_W,		m_nSttRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dFU_T,		m_nSttRow, "I", "J",24, RED, 9, FALSE, TA_CENTER, "", 0);

		pOut->SetXL(pXL, dSqSH_W,	m_nSttRow, "J", "K", 1, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dSqSH_H,	m_nSttRow, "M", "N", 9, RED, 9, FALSE, TA_CENTER, "", 0);

		pOut->SetXL(pXL, dH1,		m_nSttRow, "J", "M", 2, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dH2,		m_nSttRow, "J", "M",22, RED, 9, FALSE, TA_CENTER);

		// 전단연결재의 배치 간격
		pOut->SetXL(pXL, dDisStud,	m_nSttRow, "Y","AD", 8, RED, 9, FALSE, TA_CENTER, "", 0);
	}
}

void CXLDesignStud::HorSHSteel()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();
	CCalcData			*pData			= pDataManage->GetCalcData();
	CADeckDataTU		*pDeckDataTU	= pDataManage->GetDeckDataTU();	
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;	
	CXLControl			*pXL			= m_pXL;
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	
	double dSta			= 0;
	double dSlabWidth	= 0;
	double dMaxWidth	= 0;
	long	nSelectnG	= 0;
	CString sText		= _T("");
	long	nG	= 0;
	for(nG = 0; nG < pDB->GetQtyGirder(); nG++)
	{
		dSlabWidth = pModelCalc->GetMaxSlabWidth(nG, 0);

		if(dSlabWidth > dMaxWidth)
		{
			dMaxWidth = dSlabWidth;
			nSelectnG = nG;
		}
	}
	nG		= nSelectnG;
	pGir	= pDB->GetGirder(nG);
	dSta	= pGir->GetBxOnJijum(0)->GetStation();

	pModelCalc->GetSectionCoefficient(nG, dSta);
	pStressCalc->GetSectionStress(nG, dSta);

	double dSph1	= tokN(fabs(pStressCalc->GetMapValueSecStress("Sph1")));
	double dApNd	= pModelCalc->GetMapValueSecCo("ApNd");					// 바닥판의 환산단면적
	double dI		= pModelCalc->GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);				// 바닥판 합성단면의 2차 모멘트
	double dB		= pModelCalc->GetMapValueSecCo("BindConWidth");			// 결합면의 폭(구속콘크리트 폭)
	double dFy		= pData->DESIGN_MATERIAL.m_dSigmaY;						// 철근 항복강도
	double dYdu3	= pStressCalc->GetMapValueSecStress("Yvdu");			// 중립축에서 바닥판 상부까지 거리
	double dYdl3	= pStressCalc->GetMapValueSecStress("Yvdl");			// 중립축에서 바닥판 하부까지 거리
	double dYdm3	= (dYdu3+dYdl3)/2;										// 중립축에서 바닥판 도심까지 거리
	double dQ		= dApNd*dYdm3;											// 단면 1차 모멘트

	long   nS		= (long)pDeckDataTU->m_dCTCAnchor;	// 앵커철근간격
	double dDia		= pDeckDataTU->m_dDiaAnchor;	// 앵커철근직경
	double dARea	= pCalcFloor->AreaReturn((long)dDia);	// 앵커철근면적

	m_nSttRow = 3;
	pOut->SetXL(pXL, dSph1, m_nSttRow, "E", "I", 7, RED, 9, FALSE, TA_CENTER);
	
	sText.Format("%f", dQ);
	pXL->SetXL(m_nSttRow, 7, TA_CENTER, sText);	m_nSttRow++;
	sText.Format("%f", dI);
	pXL->SetXL(m_nSttRow, 7, TA_CENTER, sText);	m_nSttRow++;
	sText.Format("%f", dB);
	pXL->SetXL(m_nSttRow, 7, TA_CENTER, sText);	m_nSttRow++;
	sText.Format("%d", nS);
	pXL->SetXL(m_nSttRow, 7, TA_CENTER, sText);

	m_nSttRow += 7;
	pOut->SetXL(pXL, dFy,	m_nSttRow, "E", "G", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dARea,	m_nSttRow, "E", "G", 3, RED, 9, FALSE, TA_CENTER, "", 1);
}

void CXLDesignStud::ShearConnector()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();	
	CAPlateOutExcel		*pOutEXcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	long	nGsu		= pBridgeApp->GetGirdersu();
	long	nSelectnG	= 0;
	long	nSttJoint	= 0;
	long	nEndJoint	= 0;
	double	dSlabWidth	= 0;
	double	dMaxWidth	= 0;

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
	pModelCalc->GetJointRangeByGirder(nSelectnG, nSttJoint, nEndJoint);

	pOut->ClearCellRef();
	m_nSttRow = 1;
	pOut->SetXL(pXL, "8. 전단연결재의 설계",		m_nSttRow, "A", "A", 2, BLACK, 11, TRUE);

	///< (1) 합성후하중에 의한 수평전단력
	pOutEXcel->SetProgress("(1) 합성후하중에 의한 수평전단력", pOutEXcel->GetProgress()+15);
	if(pOutEXcel->m_bAbort)	return;
	HShearByPostLoad(nSelectnG, nSttJoint, nEndJoint);

	///< (2) 건조수축 및 온도차에 의한 수평전단력
	pOutEXcel->SetProgress("(2) 건조수축 및 온도차에 의한 수평전단력", pOutEXcel->GetProgress()+15);
	if(pOutEXcel->m_bAbort)	return;
	HShearOfShrinkage(nSelectnG, nSttJoint, nEndJoint);

	///< (3) STUD의 설계
	pOutEXcel->SetProgress("(3) STUD의 설계", pOutEXcel->GetProgress()+15);
	if(pOutEXcel->m_bAbort)	return;
	DesignOfStud(nSelectnG);

	///< (4) 하중조합 및 STUD 의 배치
	pOutEXcel->SetProgress("4) 하중조합 및 STUD 의 배치", pOutEXcel->GetProgress()+15);
	if(pOutEXcel->m_bAbort)	return;
	LoadComposeStudArrangement(nSelectnG, nSttJoint, nEndJoint);

	///< (5) 전단연결재의 피로설계
	pOutEXcel->SetProgress("(5) 전단연결재의 피로설계", pOutEXcel->GetProgress()+15);
	if(pOutEXcel->m_bAbort)	return;
	FatigueDesignOfStud(nSelectnG, nSttJoint, nEndJoint);

	///< (6) 전단연결재의 극한강도검토
	pOutEXcel->SetProgress("(6) 전단연결재의 극한강도검토", pOutEXcel->GetProgress()+15);
	if(pOutEXcel->m_bAbort)	return;
	UltimateStrengthOfStud(nSelectnG, nSttJoint, nEndJoint);
}
///< (1) 합성후하중에 의한 수평전단력
void CXLDesignStud::HShearByPostLoad(long nSelectnG, long nSttJoint, long nEndJoint)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFemManage		= pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);	///< 합성후 고정하중
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();	
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText;
	
	pOut->SetColsWidth(pXL, 0, 255, 1);

	pOut->SetXL(pXL, "(1) 합성후하중에 의한 수평전단력",m_nSttRow, "B", "B", 0, BLACK, 9);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.9.5.7)",		m_nSttRow,"AE","AE");
	pOut->SetXL(pXL, "※ 휨모멘트의 부호에 관계없이 바닥판 콘크리트의 단면을 유효로하여 계산한다.",m_nSttRow, "C", "C");

	pOut->SetXL(pXL, "Sv",							m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "수평전단력",					m_nSttRow, "W", "W", 1);
	pOut->SetXL(pXL, "Sv",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "V·Qvc / Iv",					m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "Iv",							m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "합성단면2차 모멘트",			m_nSttRow, "W", "W", 1);
	pOut->SetXL(pXL, "V",							m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "합성후하중에 의한 수직전단력(활하중 포함)",m_nSttRow, "W", "W", 1);
	pOut->SetXL(pXL, "Q",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Ac·dvc / n",					m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "Qvc",							m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "합성단면 중립축에 대한 콘크리트의 단면1차 모멘트",m_nSttRow, "W", "W", 1);
	pOut->SetXL(pXL, "dvc",							m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "합성단면의 도심에서 바닥판콘크리트 도심까지의 거리",m_nSttRow, "W", "W", 2);

	pOut->SetXL(pXL, "※ Vd : 합성후 고정하중에의한　수직전단력 , Vl+I : 충격을 포함한 활하중에 의한 수직전단력",m_nSttRow, "D", "D", 1);

	pOut->SetLineBoxText(pXL, "절점",				m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",					m_nSttRow, "E", "G", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "단 면",						m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Ac/n(mm²)",			m_nSttRow, "H", "K", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "dvc(mm)",			m_nSttRow, "L", "O", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Qvc(mm³)",			m_nSttRow, "P", "T", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Iv(mm⁴)",			m_nSttRow, "U", "Y", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Vd(kN)",				m_nSttRow, "Z","AC", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Vl+i(kN)",			m_nSttRow,"AD","AG", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Vd+Vl+I",			m_nSttRow,"AH","AK", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Sv(N/mm)",			m_nSttRow,"AL","AP", 2, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetXL(pXL, "TYPE",						m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER);

	BOOL bIsTU = pBridgeApp->IsTUGir();
	CString	sAcs, sDvc, sQvc, sIv, sVd, sVli, sVdli, sSv;
	for(long nJoint = nSttJoint; nJoint < nEndJoint+1; nJoint++)
	{
		CPlateBasicIndex *pBx = pCalcStd->GetBxByJoint(pFemManage->GetJoint((unsigned short)nJoint));
		double	dSta	= pBx->GetStation();

		pModelCalc->GetSectionCoefficient(nSelectnG, dSta);
		pStressCalc->GetSectionStress(nSelectnG, dSta);
		double	dAcs	= (pModelCalc->GetMapValueSecCo("Acs_SUM"));
		double	dDvc	= (pModelCalc->GetMapValueSecCo(bIsTU ? "Dvc" : "dvc"));
		double	dIv		= pModelCalc->GetMapValueSecCo(bIsTU ? "Ivd33" : "Iv33");//        /pow(10,4);
		double	dVd		= fabs(pStressCalc->GetMapValueSecStress("ShearPostDead"))/1000;
		double	dVli	= fabs(pStressCalc->GetMapValueSecStress("ShearLive"))/1000;
		//double	dAc_n		= (pModelCalc->GetMapValueSecCo("As_SLAB"))/8;

		sText.Format("%d", nJoint+1);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "C", "D", 1, RED,   9, FALSE, TA_CENTER, "", 0);

		sText = pModelCalc->GetSectionNameByBx(pFemManage, pBx, AFTER_COMPOSITE_ANALYSIS);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "E", "G", 1, RED,   9, FALSE, TA_CENTER, "", 0);

		sAcs.Format("ACS_%d", nJoint);
		pOut->SetLineBoxText(pXL, dAcs,					m_nSttRow, "H", "K", 1, RED,   9, FALSE, TA_CENTER, sAcs, 1);

		sDvc.Format("DVC_%d", nJoint);
		pOut->SetLineBoxText(pXL, dDvc,					m_nSttRow, "L", "O", 1, RED,   9, FALSE, TA_CENTER, sDvc, 3);

		sText.Format("=ROUND(%s*%s,3)",pOut->GetCellRef(sAcs), pOut->GetCellRef(sDvc));
		sQvc.Format("QVC_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "P", "T", 1, BLACK, 9, FALSE, TA_CENTER, sQvc, 1);

		sIv.Format("IV_%d", nJoint);
		pOut->SetLineBoxText(pXL, dIv,					m_nSttRow, "U", "Y", 1, RED,   9, FALSE, TA_CENTER, sIv, 1);

		sText.Format("U%d", m_nSttRow);
		pXL->SetNumberFormat(sText, "0.000E+00");

		sVd.Format("VD_%d", nJoint);
		pOut->SetLineBoxText(pXL, dVd,					m_nSttRow, "Z","AC", 1, RED,   9, FALSE, TA_CENTER, sVd);

		sVli.Format("VLI_%d", nJoint);
		pOut->SetLineBoxText(pXL, dVli,					m_nSttRow,"AD","AG", 1, RED,   9, FALSE, TA_CENTER, sVli);

		sText.Format("=ROUND(%s+%s,3)", pOut->GetCellRef(sVd), pOut->GetCellRef(sVli));
		sVdli.Format("VDLI_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AH","AK", 1, BLACK, 9, FALSE, TA_CENTER, sVdli);

		sText.Format("=ROUND(%s*%s/%s*1000,3)", pOut->GetCellRef(sQvc), pOut->GetCellRef(sVdli), pOut->GetCellRef(sIv));
		sSv.Format("SV_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AL","AP", 1, BLACK, 9, FALSE, TA_CENTER, sSv);
		SetLines(NEXT);
	}
	SetLines(NEXT);
}

///< (2) 건조수축 및 온도차에 의한 수평전단력
void CXLDesignStud::HShearOfShrinkage(long nSelectnG, long nSttJoint, long nEndJoint)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFemManage		= pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);	///< 합성후 고정하중
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText;

	long	nG		= nSelectnG;
	CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
	CPlateBasicIndex	*pBx	= pCalcStd->GetBxByJoint(pFemManage->GetJoint((unsigned short)nSttJoint));
	double	dSta				= pBx->GetStation();

	double	dGirToGir	= toM(pGir->GetDisGirderToGirder(pBx));
	double	dBridgeL	= toM(pBridgeApp->GetLengthBridge());
	
	pModelCalc->GetSectionCoefficient(nG, dSta);

	BOOL bIsTU = pBridgeApp->IsTUGir();
	double	dIs			= (pModelCalc->GetMapValueSecCo("I33"));
	double	dIc			= (pModelCalc->GetMapValueSecCo(bIsTU ? "Id" : "Igc"));
	double	dAs			= (pModelCalc->GetMapValueSecCo("As_SUM"));
	double	dAc			= (pModelCalc->GetMapValueSecCo("As_SLAB"));
	double	dD			= (pModelCalc->GetMapValueSecCo(bIsTU ? "Dd" : "dv"));
	double	dEst		= pCalcData->DESIGN_MATERIAL.m_dEst;
	double	dEpsilonS	= pCalcData->DESIGN_MATERIAL.m_dEpsilonS;
	double	dTemparature = pCalcData->DESIGN_MATERIAL.m_dTemparature;
	double	dAlpha		= pCalcData->DESIGN_MATERIAL.m_dAlpha;
	pOut->SetXL(pXL, "(2) 건조수축및 온도차에 의한 수평전단력",m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.9.5.3)",		m_nSttRow,"AE","AE",2);

	///< 그림삽입
	pOut->InsertImage(pXL, "전단연결재_건조수축", m_nSttRow, "C");
	pOut->SetXL(pXL, "L",								m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow,"AA","AA", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "단순합성형  →  지간",			m_nSttRow,"AB","AB", 1);
	pOut->SetXL(pXL, "연속합성형  →  지간의합계",		m_nSttRow,"AB","AB", 4);

	pOut->SetXL(pXL, "주형의 간격(a)",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dGirToGir,							m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "A", 2);
	pOut->SetXL(pXL, "m",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(시점부)",						m_nSttRow, "Q", "Q", 1, RED);

	pOut->SetXL(pXL, "L / 10",							m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dBridgeL,							m_nSttRow, "G", "J", 0, RED,   9, FALSE, TA_CENTER, "L", 2);
	pOut->SetXL(pXL, "/",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 10.0,								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "L_COEFF1", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s/%s,3)", pOut->GetCellRef("L"), pOut->GetCellRef("L_COEFF1"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "L_10");
	pOut->SetXL(pXL, "m",								m_nSttRow, "S", "S");
	pOut->SetXL(pXL, "∴ 전단분포폭 :",					m_nSttRow, "F", "F", 0);
	sText.Format("=IF(%s<%s,%s,%s)", pOut->GetCellRef("A"), pOut->GetCellRef("L_10"), pOut->GetCellRef("A"), pOut->GetCellRef("L_10"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER, "SHEAR_WIDTH", 2);
	pOut->SetXL(pXL, "m",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(시점부)",						m_nSttRow, "Q", "Q", 1, RED);

	pOut->SetXL(pXL, "* 건조수축에 의한 전단력(GIRDER 계산 단면TYPE - 1 참조)",m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "-  시점부",						m_nSttRow, "Y", "Y", 1, RED);

	pOut->SetXL(pXL, "n₂",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 24.0,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "N2", 0);

	pOut->SetXL(pXL, "Is",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dIs,								m_nSttRow, "X","AC", 0, RED,   9, FALSE, TA_CENTER, "IS", 1);
	pOut->SetXL(pXL, "mm⁴",							m_nSttRow,"AD","AD", 1);

	pOut->SetXL(pXL, "Ac",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAc,								m_nSttRow, "F", "I", 0, RED,   9, FALSE, TA_CENTER, "AC", 1);
	pOut->SetXL(pXL, "mm²",							m_nSttRow, "J", "J", 0);

	pOut->SetXL(pXL, "Ic",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dIc,								m_nSttRow, "X","AC", 0, RED,   9, FALSE, TA_CENTER, "IC", 1);
	pOut->SetXL(pXL, "mm⁴",							m_nSttRow,"AD","AD", 1);

	pOut->SetXL(pXL, "As",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAs,								m_nSttRow, "F", "I", 0, RED,   9, FALSE, TA_CENTER, "AS", 1);
	pOut->SetXL(pXL, "mm²",							m_nSttRow, "J", "J", 1);

	pOut->SetXL(pXL, "d",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dD,								m_nSttRow, "F", "I", 0, RED,   9, FALSE, TA_CENTER, "D", 3);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "J", "J", 0);

	pOut->SetXL(pXL, "Es",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dEst,								m_nSttRow, "X","AB", 0, RED,   9, FALSE, TA_CENTER, "ES", 1);
	pOut->SetXL(pXL, "MPa (강재의 탄성계수)",			m_nSttRow,"AC","AC", 1);

	pOut->SetXL(pXL, "(강재도심과 바닥판 중심간 거리)",	m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "εs",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dEpsilonS,							m_nSttRow, "X","AB", 0, RED,   9, FALSE, TA_CENTER, "EPSILONS", 6);
	pOut->SetXL(pXL, "(콘크리트의 건조수축에 의한 최종수축률)",m_nSttRow,"AC","AC",0);
	SetLines(-2);

	pOut->SetXL(pXL, "P2",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Es·εs·Ac/n₂",					m_nSttRow, "X", "X", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow,"AD","AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s*%s/%s,3)", pOut->GetCellRef("ES"), pOut->GetCellRef("EPSILONS"), pOut->GetCellRef("AC"), pOut->GetCellRef("N2"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AE","AH", 0, BLACK, 9, FALSE, TA_CENTER, "P2", 1);
	pOut->SetXL(pXL, "N",								m_nSttRow,"AI","AI", 1);
	SetLines(2);

	pOut->SetLineBoxText(pXL, "F₂",					m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, 1.0,					m_nSttRow, "F", "O", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetLineBoxText(pXL, 1.0,						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Ac",					m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Ac·d²",				m_nSttRow, "L", "O", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n₂·As",							m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Ic+n₂·Is",						m_nSttRow, "L", "O", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	pOut->SetUnderLineText(pXL, 1.0,					m_nSttRow, "F","AH", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF1", 0);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, 1.0,						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF2", 0, FALSE);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("AC"));
	pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "H", "N", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF3", 1);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "",						m_nSttRow, "P","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("AC"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF4", 1);
	pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X","AA", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF5");
	pOut->SetXL(pXL, "²",								m_nSttRow,"AB","AB", 1);
	sText.Format("=%s", pOut->GetCellRef("N2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF6", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("AS"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF7", 1);
	sText.Format("=%s", pOut->GetCellRef("IC"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Q", "V", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF8", 0);
	pOut->SetXL(pXL, "+",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("N2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF9", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("IS"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AF", 1, BLACK, 9, FALSE, TA_CENTER, "F2_COEFF10", 1);
	SetLines(-3);

	pOut->SetLineBoxText(pXL, "=",						m_nSttRow,"AI","AJ", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=ROUND(%s/(1+%s/(%s*%s)+%s*%s^2/(%s+%s*%s)),3)",
					pOut->GetCellRef("F2_COEFF1"), pOut->GetCellRef("F2_COEFF3"), pOut->GetCellRef("F2_COEFF6"), pOut->GetCellRef("F2_COEFF7"),
					pOut->GetCellRef("F2_COEFF4"), pOut->GetCellRef("F2_COEFF5"), pOut->GetCellRef("F2_COEFF8"), pOut->GetCellRef("F2_COEFF9"), pOut->GetCellRef("F2_COEFF10"));
	pOut->SetLineBoxText(pXL, sText,								m_nSttRow,"AK","AO", 2, BLACK, 9, FALSE, TA_CENTER, "F2", 3, FALSE);
	SetLines(4);

	pOut->SetXL(pXL, "Ncs",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P2·F2",							m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "NCS_COEFF1", 1);
	pOut->SetXL(pXL, "×",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("F2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "NCS_COEFF2", 3);
	pOut->SetXL(pXL, "=",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s,3)", pOut->GetCellRef("NCS_COEFF1"), pOut->GetCellRef("NCS_COEFF2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "T", "X", 0, BLACK, 9, FALSE, TA_CENTER, "NCS");
	pOut->SetXL(pXL, "N",								m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "Ss",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "2·Ncs / a",						m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2.0,								m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER, "SS_COEFF1", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("NCS"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "N", "R", 0, BLACK, 9, FALSE, TA_CENTER, "SS_COEFF2");
	pOut->SetXL(pXL, "/",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*1000", pOut->GetCellRef("SHEAR_WIDTH"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER, "SS_COEFF3");
	pOut->SetXL(pXL, "=",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s/%s,3)", pOut->GetCellRef("SS_COEFF1"), pOut->GetCellRef("SS_COEFF2"), pOut->GetCellRef("SS_COEFF3"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Y","AC", 0, BLACK, 9, FALSE, TA_CENTER, "SS");
	pOut->SetXL(pXL, "N/mm",							m_nSttRow,"AD","AD", 2);
//////
	pOut->SetXL(pXL, "* 온도차에 의한 전단력",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "-  시점부",						m_nSttRow, "Y", "Y", 1, RED);

	pOut->SetXL(pXL, "n",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 8.0,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "T_N", 0);

	pOut->SetXL(pXL, "Is",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("IS"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X","AC", 0, BLACK, 9, FALSE, TA_CENTER, "T_IS", 1);
	pOut->SetXL(pXL, "mm⁴",							m_nSttRow,"AD","AD", 1);

	pOut->SetXL(pXL, "Ac",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("AC"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "T_AC", 1);
	pOut->SetXL(pXL, "mm²",							m_nSttRow, "J", "J", 0);

	pOut->SetXL(pXL, "Ic",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("IC"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X","AC", 0, BLACK, 9, FALSE, TA_CENTER, "T_IC", 1);
	pOut->SetXL(pXL, "mm⁴",							m_nSttRow,"AD","AD", 1);

	pOut->SetXL(pXL, "As",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("AS"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "T_AS", 1);
	pOut->SetXL(pXL, "mm²",							m_nSttRow, "J", "J", 1);

	pOut->SetXL(pXL, "d",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "T_D", 3);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "J", "J", 0);

	pOut->SetXL(pXL, "Es",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("ES"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X","AB", 0, BLACK, 9, FALSE, TA_CENTER, "T_ES", 1);
	pOut->SetXL(pXL, "MPa (강재의 탄성계수)",			m_nSttRow,"AC","AC", 1);

	pOut->SetXL(pXL, "(강재도심과 바닥판 중심간 거리)",	m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "α",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAlpha,							m_nSttRow, "X","AB", 0, BLACK, 9, FALSE, TA_CENTER, "T_ALPHA", 6);
	pOut->SetXL(pXL, "(강재 및 콘크리트의 선팽창계수)",	m_nSttRow,"AC","AC", 1);

	pOut->SetXL(pXL, "Δt",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTemparature,						m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "T_TEMPARATURE", 0);
	pOut->SetXL(pXL, "˚C",								m_nSttRow, "Z", "Z", 0);
	pOut->SetXL(pXL, "( 바닥판 콘크리트와 강재와의 온도차 )",m_nSttRow,"AB","AB", 0);
	SetLines(-3);

	pOut->SetXL(pXL, "P1",								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Es·Δt·Ac/n",					m_nSttRow, "X", "X", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow,"AD","AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s*%s*%s/%s,3)", pOut->GetCellRef("T_ES"), pOut->GetCellRef("T_ALPHA"), pOut->GetCellRef("T_TEMPARATURE"), pOut->GetCellRef("T_AC"), pOut->GetCellRef("T_N"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AE","AH", 0, BLACK, 9, FALSE, TA_CENTER, "P1", 1);
	pOut->SetXL(pXL, "N",								m_nSttRow,"AI","AI", 1);
	SetLines(2);
//////
	pOut->SetLineBoxText(pXL, "F₁",					m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, 1.0,					m_nSttRow, "F", "O", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetLineBoxText(pXL, 1.0,						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Ac",					m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Ac·d²",				m_nSttRow, "L", "O", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n·As",							m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Ic+n·Is",						m_nSttRow, "L", "O", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	pOut->SetUnderLineText(pXL, 1.0,					m_nSttRow, "F","AH", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF1", 0);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, 1.0,						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF2", 0, FALSE);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("T_AC"));
	pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "H", "N", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF3", 1);
	pOut->SetLineBoxText(pXL, "+",						m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "",						m_nSttRow, "P","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("T_AC"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF4", 1);
	pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("T_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X","AA", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF5");
	pOut->SetXL(pXL, "²",								m_nSttRow,"AB","AB", 1);
	sText.Format("=%s", pOut->GetCellRef("T_N"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF6", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("T_AS"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF7", 1);
	sText.Format("=%s", pOut->GetCellRef("T_IC"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Q", "V", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF8", 0);
	pOut->SetXL(pXL, "+",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("T_N"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF9", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("T_IS"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AF", 1, BLACK, 9, FALSE, TA_CENTER, "F1_COEFF10", 1);
	SetLines(-3);

	pOut->SetLineBoxText(pXL, "=",						m_nSttRow,"AI","AJ", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=ROUND(%s/(1+%s/(%s*%s)+%s*%s^2/(%s+%s*%s)),3)",
					pOut->GetCellRef("F1_COEFF1"), pOut->GetCellRef("F1_COEFF3"), pOut->GetCellRef("F1_COEFF6"), pOut->GetCellRef("F1_COEFF7"),
					pOut->GetCellRef("F1_COEFF4"), pOut->GetCellRef("F1_COEFF5"), pOut->GetCellRef("F1_COEFF8"), pOut->GetCellRef("F1_COEFF9"), pOut->GetCellRef("F1_COEFF10"));
	pOut->SetLineBoxText(pXL, sText,								m_nSttRow,"AK","AO", 2, BLACK, 9, FALSE, TA_CENTER, "F1", 3, FALSE);
	SetLines(4);

	pOut->SetXL(pXL, "Ncs",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P1·F1",							m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("P1"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "T_NCS_COEFF1", 1);
	pOut->SetXL(pXL, "×",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("F1"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "T_NCS_COEFF2", 3);
	pOut->SetXL(pXL, "=",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s,3)", pOut->GetCellRef("T_NCS_COEFF1"), pOut->GetCellRef("T_NCS_COEFF2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "T", "X", 0, BLACK, 9, FALSE, TA_CENTER, "T_NCS");
	pOut->SetXL(pXL, "N",								m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "St",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "2·Ncs / a",						m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2.0,								m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER, "T_SS_COEFF1", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("T_NCS"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "N", "R", 0, BLACK, 9, FALSE, TA_CENTER, "T_SS_COEFF2");
	pOut->SetXL(pXL, "/",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*1000", pOut->GetCellRef("SHEAR_WIDTH"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER, "T_SS_COEFF3");
	pOut->SetXL(pXL, "=",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s/%s,3)", pOut->GetCellRef("T_SS_COEFF1"), pOut->GetCellRef("T_SS_COEFF2"), pOut->GetCellRef("T_SS_COEFF3"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Y","AC", 0, BLACK, 9, FALSE, TA_CENTER, "T_ST");
	pOut->SetXL(pXL, "N/mm",							m_nSttRow,"AD","AD", 2);
}
///< (3) STUD의 설계
void CXLDesignStud::DesignOfStud(long nSelectnG)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText;
	long nG = nSelectnG;

	CPlateGirderApp		*pGir			= pBridgeApp->GetGirder(nG);

	double	dStudD		= pGir->m_pSangse->m_StudHori_D;
	double	dStudH		= pGir->m_pSangse->m_StudHori_H+pGir->m_pSangse->m_StudHori_E;
	double	dStudDis	= pGir->m_pSangse->m_StudHori_Dis[1];
	long	nStudQty	= pGir->m_pSangse->m_StudHori_Qty;
	double	dSigmaCK	= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;

	pOut->SetXL(pXL, "(3) STUD의 설계",					m_nSttRow, "B", "B", 2);
	///< 그림삽입
	pOut->InsertImage(pXL, "전단연결재_STUD", m_nSttRow, "C");

	pOut->SetXL(pXL, "s",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dStudDis,							m_nSttRow, "T", "V", 0, RED,   9, FALSE, TA_CENTER, "STUD_S", 0);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "W", "W", 0);
	pOut->SetXL(pXL, "( s : STUD의 횡방향 최소간격 )",	m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "d",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dStudD,							m_nSttRow, "T", "V", 0, RED,   9, FALSE, TA_CENTER, "STUD_D", 0);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "W", "W", 0);
	pOut->SetXL(pXL, "( d : STUD의 직경 )",				m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "H",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dStudH,							m_nSttRow, "T", "V", 0, RED,   9, FALSE, TA_CENTER, "STUD_H", 0);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "W", "W", 0);
	pOut->SetXL(pXL, "( H : STUD의 전 높이 )",			m_nSttRow, "Y", "Y", 4);

	pOut->SetXL(pXL, "* STUD의 전단응력",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.9.5.6)",		m_nSttRow,"AE","AE", 1);
	pOut->SetXL(pXL, "H/d",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_H"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "H/D_COEFF1", 0);
	pOut->SetXL(pXL, "/",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "H/D_COEFF2", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s/%s,3)", pOut->GetCellRef("H/D_COEFF1"), pOut->GetCellRef("H/D_COEFF2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "H/D");
	pOut->SetXL(pXL, 5.5,								m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "A_H/D", 1);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("H/D"), pOut->GetCellRef("A_H/D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "R", "R", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "이므로",							m_nSttRow, "W", "W", 1);

	pOut->SetXL(pXL, "Qa",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>=%s,9.5,1.74", pOut->GetCellRef("H/D"), pOut->GetCellRef("A_H/D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "QA_COEFF1", 2);
	sText.Format("=%s&\"·d²· √fck\"", pOut->GetCellRef("QA_COEFF1"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "×",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Q", "R", 0, BLACK, 9, FALSE, TA_CENTER, "QA_COEFF2", 1);
	pOut->SetXL(pXL, "²",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "√",								m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", dSigmaCK);
	pOut->SetXL(pXL, sText,								m_nSttRow, "V", "X", 0, RED,   9, FALSE, TA_CENTER, "QA_COEFF3", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s^2*SQRT(%s),3)", pOut->GetCellRef("QA_COEFF1"), pOut->GetCellRef("QA_COEFF2"), pOut->GetCellRef("QA_COEFF3"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Z","AD", 0, BLACK, 9, FALSE, TA_CENTER, "STUD_QA");
	pOut->SetXL(pXL, "N/본",							m_nSttRow,"AE","AE", 1);

	pOut->SetXL(pXL, "STUD",							m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, (double)nStudQty,					m_nSttRow, "G", "H", 0, RED,   9, FALSE, TA_CENTER, "STUD_QTY", 0);
	pOut->SetXL(pXL, "본 사용 :",						m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "Qa'",								m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_QTY"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "STUD_QA_T_COEFF1", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_QA"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "STUD_QA_T_COEFF2", 1);
	pOut->SetXL(pXL, "=",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s,3)", pOut->GetCellRef("STUD_QA_T_COEFF1"), pOut->GetCellRef("STUD_QA_T_COEFF2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X","AA", 0, BLACK, 9, FALSE, TA_CENTER, "STUD_QA_T", 1);
	pOut->SetXL(pXL, "N",								m_nSttRow,"AB","AB", 1);

	pOut->SetXL(pXL, "* STUD의 배치간격",				m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "do",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Qa' /  S",						m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "( S : 수평전단력 )",				m_nSttRow, "M", "M", 2);

}
///< (4) 하중조합 및 STUD 의 배치
void CXLDesignStud::LoadComposeStudArrangement(long nSelectnG, long nSttJoint, long nEndJoint)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText, sText1, sText2;
	double	dUsePitch	= 0;
	double	dThickHunch	= pBridgeApp->m_dThickSlabHunch;
	double	dSlabThick	= pBridgeApp->m_dThickSlabBase-dThickHunch;
	
	pOut->SetXL(pXL, "(4) 하중조합 및 STUD의 배치",		m_nSttRow, "B", "B", 1);

	pOut->SetLineBoxText(pXL, "절점",					m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "하  중  조  합   ( N/mm )",m_nSttRow, "E","AC", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AD","AG", 2);
	pOut->SetXL(pXL, "소요간격",						m_nSttRow,"AD","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AH","AK", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "배치간격",						m_nSttRow,"AH","AK", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "비    고",				m_nSttRow,"AL","AO", 2, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "Sv",						m_nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Ss",						m_nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "St",						m_nSttRow, "O", "S", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Sv+St",					m_nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Ss+St",					m_nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "do=Qa'/S ",						m_nSttRow,"AD","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm)",							m_nSttRow,"AH","AK", 1, BLACK, 9, FALSE, TA_CENTER);

	CString sSv, sSs, sSt, sSvSt, sSsSt, sUsePitch;
	BOOL isEnd = FALSE;
	for(long nJoint = nSttJoint; nJoint < nEndJoint+1; nJoint++)
	{
		CPlateBasicIndex *pBx = pCalcStd->GetBxByJoint(pFEMManage->GetJoint((unsigned short)nJoint));

		///< 절점
		sText.Format("%d", nJoint+1);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "C", "D", 1, RED,   9, FALSE, TA_CENTER, "", 0);

		///< Sv
		sSv.Format("SV_%d", nJoint);
		sText.Format("=%s", pOut->GetCellRef(sSv));
		sSv.Format("L_SV_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "E", "I", 1, BLACK, 9, FALSE, TA_CENTER, sSv);

		///< Ss
		if(nJoint == nSttJoint)			sText.Format("=%s", pOut->GetCellRef("SS"));
		else if(nJoint == nEndJoint)
		{
			isEnd = TRUE;
			sSs.Format("L_SS_%d", nSttJoint);
			sText.Format("=%s", pOut->GetCellRef(sSs));
		}
		else	sText.Format("=%.3f", 0.0);
		sSs.Format("L_SS_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "J", "N", 1, BLACK, 9, FALSE, TA_CENTER, sSs);

		///< St
		if(nJoint == nSttJoint)			sText.Format("=%s", pOut->GetCellRef("T_ST"));
		else if(nJoint == nEndJoint)
		{
			sSt.Format("L_ST_%d", nSttJoint);
			sText.Format("=%s", pOut->GetCellRef(sSt));
		}
		else	sText.Format("=%.3f", 0.0);
		sSt.Format("L_ST_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "O", "S", 1, BLACK, 9, FALSE, TA_CENTER, sSt);

		///< Sv+St
		sSvSt.Format("L_SVST_%d", nJoint);
		sText.Format("=%s+%s", pOut->GetCellRef(sSv), pOut->GetCellRef(sSt));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "T", "X", 1, BLACK, 9, FALSE, TA_CENTER, sSvSt);
		///< Ss+St
		sSsSt.Format("L_SSST_%d", nJoint);
		sText.Format("=%s+%s", pOut->GetCellRef(sSs), pOut->GetCellRef(sSt));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "Y","AC", 1, BLACK, 9, FALSE, TA_CENTER, sSsSt);
		///< do
		sText.Format("=%s/(MAX(%s,%s))", pOut->GetCellRef("STUD_QA_T"), pOut->GetCellRef(sSvSt), pOut->GetCellRef(sSsSt));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AD","AG", 1, BLACK, 9, FALSE, TA_CENTER);

		dUsePitch = Round(pModelCalc->GetUseBasePitchByStation(nSelectnG, pBx->GetStation(),isEnd),0);
		sUsePitch.Format("L_USE_PITCH_%d", nJoint);
		pOut->SetLineBoxText(pXL, dUsePitch,			m_nSttRow,"AH","AK", 1, RED,   9, FALSE, TA_CENTER, sUsePitch, 1);
		
		pModelCalc->GetJijumNameOfJoint(nJoint, sText);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AL","AO", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
	}

	SetLines(NEXT);

	pOut->SetXL(pXL, "* STUD 배치 간격 검토",			m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.9.5.4, 3.9.5.5)",	m_nSttRow,"AE","AE", 1);

	pOut->SetXL(pXL, "STUD의 최대간격",					m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "바닥판콘크리트 두께의 3배",		m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, "(",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 3.0,								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_STUD_COEFF1", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dSlabThick,						m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "MAX_STUD_COEFF2", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("MAX_STUD_COEFF1"), pOut->GetCellRef("MAX_STUD_COEFF2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_STUD_COEFF3", 0);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "W", "W", 0);
	pOut->SetXL(pXL, ")",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "이고",							m_nSttRow, "Z", "Z", 0);
	pOut->SetXL(pXL, 600.0,								m_nSttRow,"AB","AC", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_STUD_COEFF4", 0);
	pOut->SetXL(pXL, "mm 이하",							m_nSttRow,"AD","AD", 1);

	pOut->SetXL(pXL, "⇒",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText1.Format("L_USE_PITCH_%d", nSttJoint);
	sText2.Format("L_USE_PITCH_%d", nEndJoint);
	sText.Format("=MAX(%s:%s)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_USE_PITCH", 0);
	pOut->SetXL(pXL, "mm 이하 사용",					m_nSttRow, "H", "H", 0);

	sText.Format("=IF(%s<=MIN(%s,600),\"... O.K\",\"... N.G\")", pOut->GetCellRef("MAX_USE_PITCH"), pOut->GetCellRef("MAX_STUD_COEFF3"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AE","AE", 1, RED);

	pOut->SetXL(pXL, "STUD의 최소간격",					m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "교축방향",						m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, ":",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "5·d",							m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=5*%s", pOut->GetCellRef("STUD_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "5D", 0);
	pOut->SetXL(pXL, "mm or 100 mm",						m_nSttRow, "R", "R", 0);

	pOut->SetXL(pXL, "⇒",									m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=MIN(%s:%s)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "MIX_USE_PITCH", 1);
	pOut->SetXL(pXL, "mm 사용",							m_nSttRow,"AA","AA", 0);

	sText.Format("=IF(OR(%s>=%s,%s>=100),\"... O.K\",\"... N.G\")", pOut->GetCellRef("MIX_USE_PITCH"), pOut->GetCellRef("5D"), pOut->GetCellRef("MIX_USE_PITCH"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AE","AE", 1, RED);

	pOut->SetXL(pXL, "교축직각방향",					m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, ":",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "d+30.0",							m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+30", pOut->GetCellRef("STUD_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "D3", 1);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "R", "R", 0);
	pOut->SetXL(pXL, "⇒",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_S"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_COEFF1", 0);
	pOut->SetXL(pXL, "mm 사용",							m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>=%s,\"... O.K\",\"... N.G\")", pOut->GetCellRef("MIN_COEFF1"), pOut->GetCellRef("D3"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AE","AE", 1, RED);


	CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nSelectnG);
	CPlateBasicIndex	*pBx	= pCalcStd->GetBxByJoint(pFEMManage->GetJoint((unsigned short)nSttJoint));

	//ARoad와 전단연결재 방식이 틀려서 수정한듯...이 방식이 맞음...체크리스트도 수정해야 겠따...070226....KB..
	double dL1 = 0;//pGir->m_pSangse->m_StudHori_Dis[0];
	long	nQtyStud	= pGir->m_pSangse->m_StudHori_Qty;
	for(long nStud=0; nStud<nQtyStud-1; nStud++)
		dL1 += pGir->m_pSangse->m_StudHori_Dis[nStud];	
	double dDist	= (pGir->GetWidthFlangeUpper(pBx)-dL1)/2.0;

	pOut->SetXL(pXL, "최소연단순간격",					m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, ":",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 25,								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_INTERVAL", 1);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dDist,								m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "USE_INTERVAL_COEFF1", 0);
	pOut->SetXL(pXL, "-",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "USE_INTERVAL_COEFF2", 1);
	pOut->SetXL(pXL, "/",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2.0,								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER, "USE_INTERVAL_COEFF3", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s-%s/2,3)", pOut->GetCellRef("USE_INTERVAL_COEFF1"), pOut->GetCellRef("USE_INTERVAL_COEFF2"), pOut->GetCellRef("USE_INTERVAL_COEFF3"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Z","AA", 0, BLACK, 9, FALSE, TA_CENTER, "USE_INTERVAL", 0);
	pOut->SetXL(pXL, "mm",								m_nSttRow,"AB","AB", 0);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("MIN_INTERVAL"), pOut->GetCellRef("USE_INTERVAL"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("MIN_INTERVAL"), pOut->GetCellRef("USE_INTERVAL"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AE","AE", 2, RED);
}

///< (5) 전단연결재의 피로설계
void CXLDesignStud::FatigueDesignOfStud(long nSelectnG, long nSttJoint, long nEndJoint)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CString	sText;
	double	dDBCount	= 0;
	double	dDLCount	= 0;
	if(pCalcData->m_nFatigueRoadType == 0)
	{
		dDBCount = 2000000;
		dDLCount = 500000;
	}
	else
	{
		dDBCount = 500000;
		dDLCount = 100000;
	}
	
	pOut->SetXL(pXL, "(5) 전단연결재의 피로설계",		m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.9.5.9)",		m_nSttRow,"AE","AE");

	pOut->SetXL(pXL, "* 수평전단력 범위",				m_nSttRow, "C", "C", 1);

	pOut->SetLineBoxText(pXL, "Sr",						m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Vr·Q",				m_nSttRow, "G", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "I",								m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-1);
	pOut->SetXL(pXL, "Sr",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "설계대상 단면의 슬래브와 주거더 사이의 수평전단력 범위 (N/mm)", m_nSttRow, "O", "O");
	pOut->SetXL(pXL, "Vr",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "충격을 포함하는 활하중에 대한 전단력의 범위 (N)", m_nSttRow, "P", "P");
	pOut->SetXL(pXL, "Q",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "정모멘트부⇒ 합성단면의 중립축에 대한 콘크리트의 환산 단면1차모멘트(mm³)", m_nSttRow, "O", "O");
	pOut->SetXL(pXL, "부모멘트부⇒ 합성단면의 중립축에 대한 철근의 단면1차모멘트(mm³)", m_nSttRow, "O", "O");
	pOut->SetXL(pXL, "I",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "합성단면면의 단면2차모멘트(mm⁴) - 부모멘트부의 경우 철근 고려", m_nSttRow, "O", "O");

	pOut->SetXL(pXL, "* 허용수평전단력 범위",			m_nSttRow, "C", "C", 3);

	pOut->SetXL(pXL, "Zr",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단연결재의 허용수평전단력 범위 ( N )", m_nSttRow, "O", "O");

	pOut->SetXL(pXL, "d",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "F_D", 1);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, "( STUD의 지름 )",					m_nSttRow, "S", "S", 1);

	pOut->SetXL(pXL, "β",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( 하중 반복횟수 : DB 하중 -",		m_nSttRow, "R", "R", 0);
	sText.Format("%.f이상",dDBCount);
	pOut->SetXL(pXL, sText,								m_nSttRow,"AC","AH", 0, RED,   9, FALSE, TA_CENTER, "DB_COUNT", 0);
	pOut->SetXL(pXL, ")",								m_nSttRow,"AI","AI", 1, BLACK, 9, FALSE, TA_LEFT);

	pOut->SetXL(pXL, "( 하중 반복횟수 : DL 하중 -",		m_nSttRow, "R", "R", 0);
	sText.Format("%.3f",dDLCount);
	pOut->SetXL(pXL, sText,								m_nSttRow,"AC","AH", 0, RED,   9, FALSE, TA_CENTER, "DL_COUNT", 0);
	pOut->SetXL(pXL, ")",								m_nSttRow,"AI","AI", 1, BLACK, 9, FALSE, TA_LEFT);
	SetLines(-2);

	sText.Format("=IF(%s=100000,91,IF(%s=500000,74,IF(%s=2000000,55,IF(%s=\"2000000이상\",39,\"ERROR\"))))",
					pOut->GetCellRef("DB_COUNT"),pOut->GetCellRef("DB_COUNT"),pOut->GetCellRef("DB_COUNT"),pOut->GetCellRef("DB_COUNT"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER, "DB_BETA", 0);

	sText.Format("=IF(%s=100000,91,IF(%s=500000,74,IF(%s=2000000,55,IF(%s=\"2000000이상\",39,\"ERROR\"))))",
					pOut->GetCellRef("DL_COUNT"),pOut->GetCellRef("DL_COUNT"),pOut->GetCellRef("DL_COUNT"),pOut->GetCellRef("DL_COUNT"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "DL_BETA", 0);
	SetLines(-5);

	pOut->SetXL(pXL, "Zr",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "β·d²",							m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "⇒",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=%s", pOut->GetCellRef("DB_BETA"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "DB_ZR_COEFF1", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("F_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "DB_ZR_COEFF2", 1);
	pOut->SetXL(pXL, "²",								m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s^2,3)", pOut->GetCellRef("DB_ZR_COEFF1"), pOut->GetCellRef("DB_ZR_COEFF2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Q", "T", 0, BLACK, 9, FALSE, TA_CENTER, "DB_ZR", 1);
	pOut->SetXL(pXL, "N",								m_nSttRow, "U", "U", 0);
	pOut->SetXL(pXL, "( DB 하중의 경우 )",				m_nSttRow, "W", "W", 1);

	sText.Format("=%s", pOut->GetCellRef("DL_BETA"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "DL_ZR_COEFF1", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("F_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "DL_ZR_COEFF2", 1);
	pOut->SetXL(pXL, "²",								m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s^2,3)", pOut->GetCellRef("DL_ZR_COEFF1"), pOut->GetCellRef("DL_ZR_COEFF2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Q", "T", 0, BLACK, 9, FALSE, TA_CENTER, "DL_ZR", 1);
	pOut->SetXL(pXL, "N",								m_nSttRow, "U", "U", 0);
	pOut->SetXL(pXL, "( DL 하중의 경우 )",				m_nSttRow, "W", "W", 6);

	pOut->SetXL(pXL, "* 피로를 고려한 전단연결재의 필요간격",m_nSttRow, "C", "C", 1);

	pOut->SetLineBoxText(pXL, "do",						m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "ΣZr",					m_nSttRow, "G", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Sr",								m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-1);
	pOut->SetXL(pXL, "do",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단연결재의 간격 ( mm )", m_nSttRow, "O", "O");
	pOut->SetXL(pXL, "ΣZr",							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n·Zr ( N )", m_nSttRow, "O", "O");

	pOut->SetXL(pXL, "n",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "한 단면에 설치된 전단연결재의 총 개수", m_nSttRow, "O", "O", 2);

	pOut->SetXL(pXL, "* 피로설계에 의한 위치별 전단연결재 간격 검토 ( DB하중에 대한 검토 )",m_nSttRow, "B", "B");

	pOut->SetLineBoxText(pXL, "절점",					m_nSttRow, "A", "B", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "I(mm⁴)",				m_nSttRow, "C", "G", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "H", "K", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "O", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "P", "S", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "T", "W", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "X","AA", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "n",						m_nSttRow,"AB","AC", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AD","AG", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AH","AJ", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AK","AM", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "비고",					m_nSttRow,"AN","AP", 2, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Ac/n",							m_nSttRow, "H", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "dvc",								m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Q",								m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Vr",								m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Sr",								m_nSttRow, "X","AA", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ΣZr",							m_nSttRow,"AD","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "소요간격",						m_nSttRow,"AH","AJ", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "배치간격",						m_nSttRow,"AK","AM", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "or Asr",							m_nSttRow, "H", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "or dr",							m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm³)",							m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N)",							m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N/mm)",						m_nSttRow, "X","AA", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N)",							m_nSttRow,"AD","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "do(mm)",							m_nSttRow,"AH","AJ", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm)",							m_nSttRow,"AK","AM", 1, BLACK, 9, FALSE, TA_CENTER);

	long nElemStt = 0, nElemEnd = 0, nElem = 0;
	pModelCalc->GetElemRangeByGirder(nSelectnG, nElemStt, nElemEnd);

	nElem = nElemStt;
	BOOL bIsTU = pBridgeApp->IsTUGir();
	CString	sIv, sAcn, sDvc, sQ, sVr, sSr, sN, sZr, sDo, sPitch;
	for(long nJoint = nSttJoint; nJoint < nEndJoint+1; nJoint++)
	{
		CPlateBasicIndex *pBx = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nJoint));
		double	dSta	= pBx->GetStation();
		BOOL bPositive = pStressCalc->IsPositive(ELE_MOMENT3, nSelectnG, dSta);

		pModelCalc->GetSectionCoefficient(nSelectnG, dSta);
		pStressCalc->GetSectionStress(nSelectnG, dSta);

		double	dIv		= pModelCalc->GetMapValueSecCo(bIsTU ? "Ivd33" : "Iv33");///pow(10,4);
		double	dAcn	= bPositive ? pModelCalc->GetMapValueSecCo("Acs_SUM") : pStressCalc->GetMapValueSecStress("Ar"); //Ac/n or As
		double	dDvc	= bPositive ? pModelCalc->GetMapValueSecCo(bIsTU ? "Dvc" : "dvc") : pStressCalc->GetMapValueSecStress("dr"); //dvc or dr

		double dVrMax(0.0), dVrMin(0.0);
		long nElemPre = (nJoint == nSttJoint) ? -1 : nElem-1;
		long nElemCur = nElem;
		double dVr = 0;
		if(nJoint == nSttJoint)
		{
			dVrMax = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, nElemCur, 0.0);
			dVrMin = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, nElemCur, 0.0);

			dVr = frkN(dVrMax - dVrMin);
		}
		else if(nJoint == nEndJoint)
		{
			dVrMax = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, nElemPre, 1.0);
			dVrMin = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, nElemPre, 1.0);

			dVr = frkN(dVrMax - dVrMin);
		}
		else
		{
			double dVrMaxCur = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, nElemCur, 0.0);
			double dVrMaxPre = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, nElemPre, 1.0);
			double dVrMinCur = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, nElemCur, 0.0);
			double dVrMinPre = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, nElemPre, 1.0);

			dVr = frkN(max(dVrMaxCur - dVrMinCur, dVrMaxPre - dVrMinPre));
		}
		
		nElem++;
				
		///< 절점
		sText.Format("%d", nJoint+1);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "A", "B", 1, RED,   9, FALSE, TA_CENTER, "", 0);
		///< I
		sIv.Format("DB_IV_%d", nJoint);
		pOut->SetLineBoxText(pXL, dIv,					m_nSttRow, "C", "G", 1, RED,   9, FALSE, TA_CENTER, sIv, 1);
		sText.Format("C%d", m_nSttRow);
		pXL->SetNumberFormat(sText, "0.000E+00");
		
		///< Ac/n
		sAcn.Format("DB_ACN_%d", nJoint);
		pOut->SetLineBoxText(pXL, dAcn,					m_nSttRow, "H", "K", 1, RED,   9, FALSE, TA_CENTER, sAcn, 1);
		///< dvc
		sDvc.Format("DB_DVC_%d", nJoint);
		pOut->SetLineBoxText(pXL, dDvc,					m_nSttRow, "L", "O", 1, RED,   9, FALSE, TA_CENTER, sDvc, 3);
		///< Q
		sQ.Format("DB_Q_%d", nJoint);
		sText.Format("=%s*%s", pOut->GetCellRef(sAcn), pOut->GetCellRef(sDvc));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "P", "S", 1, BLACK, 9, FALSE, TA_CENTER, sQ, 0);
		///< Vr
		sVr.Format("DB_VR_%d", nJoint);
		pOut->SetLineBoxText(pXL, dVr,					m_nSttRow, "T", "W", 1, RED,   9, FALSE, TA_CENTER, sVr, 1);
		///< Sr
		sSr.Format("DB_SR_%d", nJoint);
		sText.Format("=ROUND(%s*%s/%s,3)", pOut->GetCellRef(sVr), pOut->GetCellRef(sQ), pOut->GetCellRef(sIv));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "X","AA", 1, BLACK, 9, FALSE, TA_CENTER, sSr, 1);
		///< n
		sN.Format("DB_STUD_%d", nJoint);
		sText.Format("=%s", pOut->GetCellRef("STUD_QTY"));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AB","AC", 1, BLACK, 9, FALSE, TA_CENTER, sN, 0);
		///< ΣZr
		sZr.Format("DB_ZR_%d", nJoint);
		sText.Format("=%s*%s", pOut->GetCellRef("DB_ZR"), pOut->GetCellRef(sN));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AD","AG", 1, BLACK, 9, FALSE, TA_CENTER, sZr, 1);
		///< 소요간격
		sDo.Format("DB_DO_%d", nJoint);
		sText.Format("=%s/%s", pOut->GetCellRef(sZr), pOut->GetCellRef(sSr));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AH","AJ", 1, BLACK, 9, FALSE, TA_CENTER, sDo, 1);

		///< 배치간격
		sPitch.Format("L_USE_PITCH_%d", nJoint);
		sText.Format("=%s", pOut->GetCellRef(sPitch));
		sPitch.Format("DB_USE_PITCH_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AK","AM", 1, BLACK, 9, FALSE, TA_CENTER, sPitch, 1);
		///< 비고
		sText.Format("=IF(%s>%s, \"O.K\", \"N.G\")", pOut->GetCellRef(sDo), pOut->GetCellRef(sPitch));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
	}

	SetLines(NEXT);
	pOut->SetXL(pXL, "* 피로설계에 의한 위치별 전단연결재 간격 검토 ( DL하중에 대한 검토 )",m_nSttRow, "B", "B");
	pOut->SetLineBoxText(pXL, "절점",					m_nSttRow, "A", "B", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "I(mm⁴)",				m_nSttRow, "C", "G", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "H", "K", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "O", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "P", "S", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "T", "W", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "X","AA", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "n",						m_nSttRow,"AB","AC", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AD","AG", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AH","AJ", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow,"AK","AM", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "비고",					m_nSttRow,"AN","AP", 2, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Ac/n",							m_nSttRow, "H", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "dvc",								m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Q",								m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Vr",								m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Sr",								m_nSttRow, "X","AA", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ΣZr",							m_nSttRow,"AD","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "소요간격",						m_nSttRow,"AH","AJ", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "배치간격",						m_nSttRow,"AK","AM", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "or Asr",							m_nSttRow, "H", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "or dr",							m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm³)",							m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N)",							m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N/mm)",						m_nSttRow, "X","AA", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N)",							m_nSttRow,"AD","AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "do(mm)",							m_nSttRow,"AH","AJ", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm)",							m_nSttRow,"AK","AM", 1, BLACK, 9, FALSE, TA_CENTER);
	
	nElem = nElemStt;
	for(long nJoint = nSttJoint; nJoint < nEndJoint+1; nJoint++)
	{
		CPlateBasicIndex *pBx = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nJoint));
		double	dSta	= pBx->GetStation();
		BOOL bPositive = pStressCalc->IsPositive(ELE_MOMENT3, nSelectnG, dSta);

		pModelCalc->GetSectionCoefficient(nSelectnG, dSta);
		pStressCalc->GetSectionStress(nSelectnG, dSta);

		double	dIv		= pModelCalc->GetMapValueSecCo(bIsTU ? "Ivd33" : "Iv33");///pow(10,4);
		double	dAcn	= bPositive ? pModelCalc->GetMapValueSecCo("Acs_SUM") : pStressCalc->GetMapValueSecStress("Ar"); //Ac/n or As
		double	dDvc	= bPositive ? pModelCalc->GetMapValueSecCo(bIsTU ? "Dvc" : "dvc") : pStressCalc->GetMapValueSecStress("dr"); //dvc or dr

		double dVrMax(0.0), dVrMin(0.0);
		long nElemPre = (nJoint == nSttJoint) ? -1 : nElem-1;
		long nElemCur = nElem;	
		double dVr = 0;

		if(nJoint == nSttJoint)
		{
			dVrMax = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, nElemCur, 0.0);
			dVrMin = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, nElemCur, 0.0);

			dVr = frkN(dVrMax - dVrMin);
		}
		else if(nJoint == nEndJoint)
		{
			dVrMax = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, nElemPre, 1.0);
			dVrMin = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, nElemPre, 1.0);

			dVr = frkN(dVrMax - dVrMin);
		}
		else
		{
			double dVrMaxCur = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, nElemCur, 0.0);
			double dVrMaxPre = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, nElemPre, 1.0);
			double dVrMinCur = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, nElemCur, 0.0);
			double dVrMinPre = pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, nElemPre, 1.0);

			dVr = frkN(max(dVrMaxCur - dVrMinCur, dVrMaxPre - dVrMinPre));
		}
		
		nElem++;

		///< 절점
		sText.Format("%d", nJoint+1);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "A", "B", 1, RED,   9, FALSE, TA_CENTER, "", 0);
		///< I
		sIv.Format("DL_IV_%d", nJoint);
		pOut->SetLineBoxText(pXL, dIv,					m_nSttRow, "C", "G", 1, RED,   9, FALSE, TA_CENTER, sIv, 1);
		sText.Format("C%d", m_nSttRow);
		pXL->SetNumberFormat(sText, "0.000E+00");
		
		///< Ac/n
		sAcn.Format("DL_ACN_%d", nJoint);
		pOut->SetLineBoxText(pXL, dAcn,					m_nSttRow, "H", "K", 1, RED,   9, FALSE, TA_CENTER, sAcn, 1);
		///< dvc
		sDvc.Format("DL_DVC_%d", nJoint);
		pOut->SetLineBoxText(pXL, dDvc,					m_nSttRow, "L", "O", 1, RED,   9, FALSE, TA_CENTER, sDvc, 3);
		///< Q
		sQ.Format("DL_Q_%d", nJoint);
		sText.Format("=%s*%s", pOut->GetCellRef(sAcn), pOut->GetCellRef(sDvc));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "P", "S", 1, BLACK, 9, FALSE, TA_CENTER, sQ, 0);
		///< Vr
		sVr.Format("DL_VR_%d", nJoint);
		pOut->SetLineBoxText(pXL, dVr,					m_nSttRow, "T", "W", 1, RED,   9, FALSE, TA_CENTER, sVr, 1);
		///< Sr
		sSr.Format("DL_SR_%d", nJoint);
		sText.Format("=ROUND(%s*%s/%s,3)", pOut->GetCellRef(sVr), pOut->GetCellRef(sQ), pOut->GetCellRef(sIv));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "X","AA", 1, BLACK, 9, FALSE, TA_CENTER, sSr, 1);
		///< n
		sN.Format("DL_STUD_%d", nJoint);
		sText.Format("=%s", pOut->GetCellRef("STUD_QTY"));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AB","AC", 1, BLACK, 9, FALSE, TA_CENTER, sN, 0);
		///< ΣZr
		sZr.Format("DL_ZR_%d", nJoint);
		sText.Format("=%s*%s", pOut->GetCellRef("DL_ZR"), pOut->GetCellRef(sN));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AD","AG", 1, BLACK, 9, FALSE, TA_CENTER, sZr, 1);
		///< 소요간격
		sDo.Format("DL_DO_%d", nJoint);
		sText.Format("=%s/%s", pOut->GetCellRef(sZr), pOut->GetCellRef(sSr));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AH","AJ", 1, BLACK, 9, FALSE, TA_CENTER, sDo, 1);

		///< 배치간격
		sPitch.Format("L_USE_PITCH_%d", nJoint);
		sText.Format("=%s", pOut->GetCellRef(sPitch));
		sPitch.Format("DL_USE_PITCH_%d", nJoint);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AK","AM", 1, BLACK, 9, FALSE, TA_CENTER, sPitch, 1);
		///< 비고
		sText.Format("=IF(%s>%s, \"O.K\", \"N.G\")", pOut->GetCellRef(sDo), pOut->GetCellRef(sPitch));
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow,"AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
	}
	SetLines(NEXT);
}

///< (6) 전단연결재의 극한강도검토
void CXLDesignStud::UltimateStrengthOfStud(long nSelectnG, long nSttJoint, long nEndJoint)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);	

	long	nG							= nSelectnG;
	CPlateGirderApp		*pGir			= pBridgeApp->GetGirder(nG);
	CPlateBasicIndex	*pBx			= pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nSttJoint));
	double	dSta						= pBx->GetStation();

	CString	sText;
	CString sJuType = pBridgeApp->GetMaterial(G_F_U, pBx);
	CString sJu     = sJuType;
	long nSpaceIdx  = sJu.Find(' ');
	if(nSpaceIdx != -1) sJu.Delete(nSpaceIdx);
	double	dFyr	= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dEc		= pCalcData->DESIGN_MATERIAL.m_dEc;
	double	dFu		= 410;///< 전단연결재의 인장강도(MPa)
	double	dFy		= 0;
	double	dThickHunch	= pBridgeApp->m_dThickSlabHunch;
	double	dSlabThick	= (pBridgeApp->m_dThickSlabBase-dThickHunch);//toCm

	pOut->SetXL(pXL, "(6) 전단연결재의 극한강도 검토",	m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.9.5.10)",		m_nSttRow,"AE","AE");

	pOut->SetXL(pXL, "* 전단연결재의 극한강도",			m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "Vu",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "0.4·d²·√fck·Ec",				m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 0.4,								m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "VU_COEFF1", 1);
	pOut->SetXL(pXL, "×",								m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("STUD_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "VU_COEFF2", 0);
	pOut->SetXL(pXL, "²",								m_nSttRow, "K", "K", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "√(",								m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("QA_COEFF3"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "VU_COEFF3", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dEc,								m_nSttRow, "Q", "S", 0, RED,   9, FALSE, TA_CENTER, "VU_COEFF4", 0);
	pOut->SetXL(pXL, ")",								m_nSttRow, "T", "T", 1);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*%s^2*SQRT(%s*%s),3)", pOut->GetCellRef("VU_COEFF1"), pOut->GetCellRef("VU_COEFF2"), pOut->GetCellRef("VU_COEFF3"), pOut->GetCellRef("VU_COEFF4"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "VU", 1);
	pOut->SetXL(pXL, "N",								m_nSttRow, "J", "J", 0);
	SetLines(-2);

	pOut->SetXL(pXL, "Vu",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단연결재의 극한강도 (N) 단, Vu ≤ Asc fu", m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "d",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단연결재의 지름",				m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "fck",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "콘크리트의 28일압축강도(MPa)",m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "Asc fu",							m_nSttRow, "B", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"π/4 ×\"&ROUND(%s,3)&\"²×\"", pOut->GetCellRef("F_D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, dFu,								m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "FU", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(PI()/4*%s^2*%s,3)", pOut->GetCellRef("F_D"), pOut->GetCellRef("FU"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "ASC_FU", 1);
	pOut->SetXL(pXL, "N",								m_nSttRow, "T", "T", 0);

	pOut->SetXL(pXL, "Asc",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단연결재의 단면적(mm²)",m_nSttRow, "Y", "Y", 0);

	SetLines(-1);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("VU"), pOut->GetCellRef("ASC_FU"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "M", "M", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Asc fu",							m_nSttRow, "O", "O", 2);

	sText.Format("=IF(%s>%s,\"⇒ Vu = Asc fu = \"&ROUND(%s,1)&\" N\",\"\")", pOut->GetCellRef("VU"), pOut->GetCellRef("ASC_FU"), pOut->GetCellRef("ASC_FU"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "D", "D", 0);

	pOut->SetXL(pXL, "fu",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단연결재의 인장강도(MPa)",	m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "* 최대 정모멘트부의 슬래브에 작용하는 힘 ( P ) - P₁, P₂중 작은 값을 취한다.",m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "P₁",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "As·fy",							m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "As",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "덮개판을 포함하는 강재 단면의 총면적(mm²)",m_nSttRow, "Y", "Y", 1);
	pOut->SetXL(pXL, "fy",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "사용된 강재의 공칭항복응력",		m_nSttRow, "Y", "Y", 0);
	pOut->SetXL(pXL, dFy,								m_nSttRow,"AJ","AL", 0, RED,   9, FALSE, TA_CENTER, "FY", 0);
	pOut->SetXL(pXL, "(MPa)",						m_nSttRow,"AM","AM", 1);

	pOut->SetXL(pXL, "fck",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "콘크리트의 28일 압축강도(MPa)",m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "P₂",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "0.85·fck·b·ts",				m_nSttRow, "F", "F", 0);

	pOut->SetXL(pXL, "b",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "슬래브의 유효폭 ( mm )",			m_nSttRow, "Y", "Y", 1);

	pOut->SetXL(pXL, "ts",								m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "콘크리트의 슬래브의 두께",		m_nSttRow, "Y", "Y", 0);
	pOut->SetXL(pXL, dSlabThick,						m_nSttRow,"AJ","AL", 0, RED,   9, FALSE, TA_CENTER, "SLAB_THICK", 1);
	pOut->SetXL(pXL, "(mm)",							m_nSttRow,"AM","AM", 1);

	pOut->SetXL(pXL, "* 최대 부모멘트부의 슬래브에 작용하는 힘",m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "P₃",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Asr·fyr",						m_nSttRow, "F", "F", 0);

	pOut->SetXL(pXL, "Asr",								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "지점부 유효슬래브 폭 내에 설치된 종방향 철근의 총면적 (mm²)",m_nSttRow, "Q", "Q", 1);

	pOut->SetXL(pXL, "fyr",								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "철근의 최소 항복응력",			m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, dFyr,								m_nSttRow,"AB","AD", 0, RED,   9, FALSE, TA_CENTER, "FYR", 0);
	pOut->SetXL(pXL, "(MPa)",						m_nSttRow,"AE","AE", 1);

	pOut->SetXL(pXL, "* 전단연결재의 필요갯수",			m_nSttRow, "C", "C");
	pOut->SetLineBoxText(pXL, "N₁",					m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P",					m_nSttRow, "F", "H", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "φVu",							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "N₁",								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "최대 정모멘트부와 인접한 교대 지점부 사이의 전단연결재 필요개수",m_nSttRow, "Q", "Q", 1);

	pOut->SetXL(pXL, "P",								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P₁P₂중 작은값",					m_nSttRow, "Q", "Q", 1);

	pOut->SetLineBoxText(pXL, "N₂",					m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P + P₃",				m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "φ",								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "감소계수 ( 0.85 )",				m_nSttRow, "Q", "Q", 1);

	pOut->SetXL(pXL, "φVu",							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "N₂",								m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "최대 정모멘트부와 인접한 최대부모멘트부 사이의 전단연결재 필요개수",m_nSttRow, "Q", "Q", 2);

	long	nKsu			= pBridgeApp->m_nQtyJigan;
	long	nGuganPlusSu	= 2;
	long	nGuganMinusSu	= (nKsu-1) * 2;
	long	nGuganSu		= nGuganPlusSu + nGuganMinusSu;

	pOut->SetXL(pXL, "* 구간별 전단연결재 필요개수 및 설치개수 검토",m_nSttRow, "C", "C");

	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "A", "E", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "F", "H", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "L", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "M", "P", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Q", "T", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "U", "X", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y","AB", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "필요개수",				m_nSttRow,"AC","AH", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "설치개수",				m_nSttRow,"AI","AL", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "비 고",					m_nSttRow,"AM","AO", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "위 치",							m_nSttRow, "A", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "b",								m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "As",								m_nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Asr",								m_nSttRow, "M", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P₁",								m_nSttRow, "Q", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P₂",								m_nSttRow, "U", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P₃",								m_nSttRow, "Y","AB", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetXL(pXL, "(절점번호)",						m_nSttRow, "A", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm)",							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm²)",							m_nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(mm²)",							m_nSttRow, "M", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N)",							m_nSttRow, "Q", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N)",							m_nSttRow, "U", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(N)",							m_nSttRow, "Y","AB", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "N₁",					m_nSttRow,"AC","AE", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "N₂",					m_nSttRow,"AF","AH", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);

	// 구간별 전단연결재 필요수량 및 설치수량 검토
	// 지간 개수에 의한 구간의 개수 구하기
	// 슈 위치인 절점인덱스 구하기
	CDWordArray arrShoeJointID;
	long i = 0;
	for(i=nSttJoint; i<=nEndJoint; i++)
	{
		CJoint *pJ = pManage->GetJoint((unsigned short)i);
		if(pCalcStd->GetBxByJoint(pJ)->IsJijum())
			arrShoeJointID.Add(i);
		
	}
	////////////////////////////////////////////////////
	// 각 구간을 구성하는 시작 절점번호 구하기
	long nMoMaxNodeNO = 0;
	double MoMax = 0.0,Mo = 0;
	CDWordArray DArPlusMoment;  // 각 지간에서의 최대 모멘트 절점

	for(long nK=0; nK<nKsu; nK++)
	{
		if(nK==0)
		{
			MoMax = 0.0;
			for(i=(long)arrShoeJointID[0]; i<=(long)arrShoeJointID[1]; i++)
			{
				CJoint *pJ = pManage->GetJoint((unsigned short)i);
				nG   = pModelCalc->GetGirderNoByJointID(i);
				pBx  = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)i));
				dSta = pBx->GetStation();
				Mo   = pStressCalc->GetElementForce(ELE_MOMENT3, LC_NEXT, nG, dSta,0)
					  +pStressCalc->GetElementForce(ELE_MOMENT3, LC_LIVE_MAX, nG, dSta,0)
					  +pStressCalc->GetElementForce(ELE_MOMENT3, LC_DISP_MAX, nG, dSta,0);
				
				if(Mo > MoMax)
				{
					MoMax = Mo;
					nMoMaxNodeNO = i+1;
				}
			}
			DArPlusMoment.Add(nMoMaxNodeNO);
		}
		else if(nK==nKsu-1)
		{
			MoMax = 0.0;
			for(i=arrShoeJointID[nK]; i<=nEndJoint; i++)
			{
				nG   = pModelCalc->GetGirderNoByJointID(i);
				pBx  = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)i));
				dSta = pBx->GetStation();
				Mo   = pStressCalc->GetElementForce(ELE_MOMENT3, LC_NEXT, nG, dSta,0)
					  +pStressCalc->GetElementForce(ELE_MOMENT3, LC_LIVE_MAX, nG, dSta,0)
					  +pStressCalc->GetElementForce(ELE_MOMENT3, LC_DISP_MAX, nG, dSta,0);
				if(Mo > MoMax)
				{
					MoMax = Mo;
					nMoMaxNodeNO = i+1;
				}
			}
			DArPlusMoment.Add(nMoMaxNodeNO);
		}
		else
		{
			MoMax = 0.0;
			for(i=(long)arrShoeJointID[nK]; i<=(long)arrShoeJointID[nK+1]; i++)
			{
				nG   = pModelCalc->GetGirderNoByJointID(i);
				pBx  = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)i));
				dSta = pBx->GetStation();
				Mo   = pStressCalc->GetElementForce(ELE_MOMENT3, LC_NEXT, nG, dSta,0)
					  +pStressCalc->GetElementForce(ELE_MOMENT3, LC_LIVE_MAX, nG, dSta,0)
					  +pStressCalc->GetElementForce(ELE_MOMENT3, LC_DISP_MAX, nG, dSta,0);
				if(Mo > MoMax)
				{
					MoMax = Mo;
					nMoMaxNodeNO = i+1;
				}
			}
			DArPlusMoment.Add(nMoMaxNodeNO);
		}
	}

	////////////////////////////////////////////////////
	double SlabEffectWidth = 0, AsPlus = 0, AsMinus = 0, Aru = 0,Arl = 0;
	long nGaesu = 0;
	long nSttNodeNO = 0, nEndNodeNO = 0;
	long nNodeIndex = 0;

	for(long nGugan = 0; nGugan < nGuganSu; nGugan++)
	{
		if(nGugan % 2 == 0)
		{
			nSttNodeNO = arrShoeJointID[(long)(nGugan/2)] + 1;
			nEndNodeNO = DArPlusMoment[(long)(nGugan/2)];
		}
		else
		{
			nSttNodeNO = DArPlusMoment[(long)(nGugan/2)];
			nEndNodeNO = arrShoeJointID[(long)((nGugan-1)/2+1)] + 1;
		}

		if(nGugan % 2 == 0)	nNodeIndex = nEndNodeNO - 1;
		else				nNodeIndex = nSttNodeNO - 1;

		nG  = pModelCalc->GetGirderNoByJointID(nNodeIndex);
		pBx = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nNodeIndex));

		SlabEffectWidth = (pModelCalc->GetEffectWidthSlab(nG, pBx->GetStation()));

		pModelCalc->GetSectionCoefficient(nG, pBx->GetStation());
		AsPlus = (pModelCalc->GetMapValueSecCo("As_SUM"));

		if(nGugan != 0 && nGugan != nGuganSu-1)
		{
			if(nGugan % 2 == 0)	nNodeIndex = nSttNodeNO - 1;
			else				nNodeIndex = nEndNodeNO - 1;

			nG  = pModelCalc->GetGirderNoByJointID(nNodeIndex);
			pBx = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nNodeIndex));
			
			Aru = pCalcData->m_Asuse1*toM(pModelCalc->GetEffectWidthSlab(nG, pBx->GetStation()));
			Arl = pCalcData->m_Asuse2*toM(pModelCalc->GetEffectWidthSlab(nG, pBx->GetStation()));
			AsMinus = Aru + Arl;
		}
		else
			AsMinus = 0.0;

		nGaesu = pModelCalc->GetShearChainSu(nG, nSttNodeNO, nEndNodeNO);

		sText.Format("%d ~ %d", nSttNodeNO, nEndNodeNO);
		pOut->SetLineBoxText(pXL, sText			  , m_nSttRow,"A", "E", 1, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetLineBoxText(pXL, SlabEffectWidth , m_nSttRow,"F", "H", 1, RED, 9, FALSE, TA_CENTER, "b", 1);
		pOut->SetLineBoxText(pXL, AsPlus		  , m_nSttRow,"I", "L", 1, RED, 9, FALSE, TA_CENTER, "As", 1);
		pOut->SetLineBoxText(pXL, AsMinus		  , m_nSttRow,"M", "P", 1, RED, 9, FALSE, TA_CENTER, "As'", 1);
		

		sText.Format("=%s*%s", pOut->GetCellRef("FY"), pOut->GetCellRef("As"));
		pOut->SetLineBoxText(pXL, sText		  	  , m_nSttRow,"Q", "T", 1, BLACK, 9, FALSE, TA_CENTER, "P1", 0);

		sText.Format("=0.85*%s*%s*%s", pOut->GetCellRef("QA_COEFF3"), pOut->GetCellRef("b"), pOut->GetCellRef("SLAB_THICK"));
		pOut->SetLineBoxText(pXL, sText		      , m_nSttRow,"U", "X", 1, BLACK, 9, FALSE, TA_CENTER, "P2", 0);
		
		sText.Format("=%s*%s", pOut->GetCellRef("As'"), pOut->GetCellRef("FYR"));
		pOut->SetLineBoxText(pXL, sText			  , m_nSttRow,"Y","AB", 1, BLACK, 9, FALSE, TA_CENTER, "P3", 0);
		
		sText.Format("=IF(%s=0,MIN(%s,%s)/(0.85*MIN(%s,%s)),\"-\")", pOut->GetCellRef("As'"), pOut->GetCellRef("P1")
											 , pOut->GetCellRef("P2"), pOut->GetCellRef("VU"), pOut->GetCellRef("ASC_FU"));
		pOut->SetLineBoxText(pXL, sText			  , m_nSttRow,"AC","AE", 1, BLACK, 9, FALSE, TA_CENTER, "N1", 0);
		
		sText.Format("=IF(%s=0,\"-\",(MIN(%s,%s)+%s)/(0.85*MIN(%s,%s)))", pOut->GetCellRef("As'"), pOut->GetCellRef("P1")
											 , pOut->GetCellRef("P2"), pOut->GetCellRef("P3"), pOut->GetCellRef("VU"), pOut->GetCellRef("ASC_FU"));
		pOut->SetLineBoxText(pXL, sText			  , m_nSttRow,"AF","AH", 1, BLACK, 9, FALSE, TA_CENTER, "N2", 0);

		pOut->SetLineBoxText(pXL, nGaesu		  , m_nSttRow,"AI","AL", 1, RED, 9, FALSE, TA_CENTER, "설치개수", 0);
		
		sText.Format("=IF(%s=0,IF(%s<=%s,\"O.K.\",\"N.G.\"),IF(%s<=%s,\"O.K.\",\"N.G.\"))", 
					 pOut->GetCellRef("As'"), pOut->GetCellRef("N1"), pOut->GetCellRef("설치개수"), pOut->GetCellRef("N2"), pOut->GetCellRef("설치개수"));
		pOut->SetLineBoxText(pXL, sText			  , m_nSttRow,"AM","AO", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

		m_nSttRow++;
	}
}

void CXLDesignStud::SetLines(int nLine)
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