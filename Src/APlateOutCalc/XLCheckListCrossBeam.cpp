// XLCheckListCrossBeam.cpp: implementation of the CXLCheckListCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplateoutcalc.h"
#include "APlateOutCalcStd.h"
#include "XLCheckListCrossBeam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLCheckListCrossBeam::CXLCheckListCrossBeam(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_pAPlateOutCalcStd	= pStd;
}

CXLCheckListCrossBeam::~CXLCheckListCrossBeam()
{

}


void CXLCheckListCrossBeam::CrossBeam()
{
	CXLControl			*pXL		= m_pXL;
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	

	m_nSttRow			= 1;	
	pXL->SetCellWidth(0, 255, 1);//
	
	pOut->SetXL(pXL, "구조계산 CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "9. 가로보 검토",	m_nSttRow, "A", "A",1, 1, 9,TRUE);	


	CrossBeamGeneral(1);//사용강종 및 최소두께 검토
	SetLines(NEXT);

	CrossBeamGeneral(2);//응력검토
	SetLines(NEXT);
	CrossBeamGeneral(3);//3) 현장이음 검토
	SetLines(NEXT);
	CrossBeamGeneral(4);//4) 수직보강재
	SetLines(NEXT);
	CrossBeamGeneral(5);//5) 수평보강재
	SetLines(NEXT);		

}

void CXLCheckListCrossBeam::CrossBeamHorVerBracing()
{
	CXLControl			*pXL		= m_pXL;
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	

	m_nSttRow			= 1;	
	pXL->SetCellWidth(0, 255, 1);//
	
	pOut->SetXL(pXL, "구조계산 CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "9. 브레이싱 검토",	m_nSttRow, "A", "A",1, 1, 9,TRUE);	
	
	CorssBeamBracing(6);//수직브레이싱....
	SetLines(NEXT);
	CorssBeamBracing(7);//수직브레이싱...
	SetLines(NEXT);
	HorBracing(8);//수평브레이싱..
	SetLines(NEXT);

}

void CXLCheckListCrossBeam::CrossBeamGeneral(int nSeq)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	BOOL bOnlyFirst = TRUE;
		
	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++)//nG < 1;nG++)// 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		CString sGubun;

		int nCrossCount = 1;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();			
			if(pSec)
			{	
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				
				// 0=V형수직브레이싱, 1=역V형수직브레이싱, 2= 가로보
				if(pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
				{	
					if (bOnlyFirst)
					{
						CheckListTitle(nSeq);
						bOnlyFirst = FALSE;
					}

					if(nCrossCount < 10)
						sGubun.Format("%dCR0%d", nG+1,nCrossCount);
					else
						sGubun.Format("%dCR%d", nG+1,nCrossCount);

					CrossBeamGeneralDetail(nSeq, pBx, sGubun);					
				}
			}	
			nCrossCount++;
			pBx = Finder.GetBxNext();
		}	
	}

	if(bOnlyFirst && nSeq == 1)
	{
		pOut->SetXL(pXL, "가로보 타입이 없습니다. ", m_nSttRow, "A", "A");		
	}
}

void CXLCheckListCrossBeam::CrossBeamGeneralDetail(int nSeq, CPlateBasicIndex *pBx, CString sGubun)
{
	CAPlateCalcStd			*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl				*pXL			= m_pXL;
	CAPlateOutCalcXL		*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CPlateCrossBeam			*pCrossBeam	= pBx->GetCrossBeam();
	CDesignCrossBeam		CrossBeam(pCalcStd, pBx);
	CDesignEndCrossBeam		*pEndCr			= NULL;
	CDesignCenterCrossBeam	*pCenCr			= NULL;	

	if(pBx->GetSection()->IsenType(SECTION_A))
		pEndCr	= (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
	else
		pCenCr	= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();

	CString sText = _T("");
	
	switch(nSeq)
	{
		case 1:
		{		
			CString sMaterial	= _T("");
			long nQtyHorStiff	= 0;
			double dWebMinThick = 0;
			double dUpMinThick	= 0;
			double dLoMinThick	= 0;
			double dWebUseThick	= 0;
			double dUpUseThick	= 0;
			double dLoUseThick	= 0;

			if(pEndCr)
			{
				sMaterial		= pEndCr->m_pJewonFlangeLo->m_strMaterial;
				nQtyHorStiff	= pEndCr->m_nQtyHorStiff;
				dWebUseThick	= pEndCr->m_pJewonWeb->m_dWidth;
				dWebMinThick	= pEndCr->m_strtMinThick.m_dThickWebReq;
				dUpUseThick		= pEndCr->m_pJewonFlangeUp->m_dHeight;
				dUpMinThick		= pEndCr->m_strtMinThick.m_dMinThickFlangeUp;
				dLoUseThick		= pEndCr->m_pJewonFlangeLo->m_dHeight;
				dLoMinThick		= pEndCr->m_strtMinThick.m_dMinThickFlangeLo;						
			}
			if(pCenCr)
			{
				sMaterial		= pCenCr->m_pJewonFlangeLo->m_strMaterial;
				nQtyHorStiff	= pCenCr->m_nQtyHorStiff;
				dWebUseThick	= pCenCr->m_pJewonWeb->m_dWidth;
				dWebMinThick	= pCenCr->m_strtMinThick.m_dThickWebReq;
				dUpUseThick		= pCenCr->m_pJewonFlangeUp->m_dHeight;
				dUpMinThick		= pCenCr->m_strtMinThick.m_dMinThickFlangeUp;
				dLoUseThick		= pCenCr->m_pJewonFlangeLo->m_dHeight;
				dLoMinThick		= pCenCr->m_strtMinThick.m_dMinThickFlangeLo;
			}
			
				
			//실제 엑셀에 쓰기...
			pOut->SetLineBoxText(pXL, sGubun,		m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, sMaterial,	m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, nQtyHorStiff,	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "", 0);					
			pOut->SetLineBoxText(pXL, dWebMinThick, m_nSttRow, "M", "O",   1, BLACK, 9, FALSE, TA_CENTER, "Is1", 1);
			pOut->SetLineBoxText(pXL, dWebUseThick, m_nSttRow, "P", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Is2", 1);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is1"), pOut->GetCellRef("Is2"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dUpMinThick,	m_nSttRow, "U", "W",   1, BLACK, 9, FALSE, TA_CENTER, "Is3", 1);
			pOut->SetLineBoxText(pXL, dUpUseThick,	m_nSttRow, "X", "Z",   1, BLACK, 9, FALSE, TA_CENTER, "Is4", 1);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is3"), pOut->GetCellRef("Is4"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AA", "AB", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dLoMinThick,	m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER, "Is5", 1);
			pOut->SetLineBoxText(pXL, dLoUseThick,	m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "Is6", 1);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is5"), pOut->GetCellRef("Is6"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);	
			pOut->SetLineBoxText(pXL, "",			m_nSttRow, "AK", "AP", 1, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			break;
		}
		case 2://응력검토
		{
			double dCompGen[6];
			double dCompWind[6];
			double dCompEarthQuake[6];
			double dCompFrameLoad[6];
			double dMaxStress[6];
			for (int n=0; n<6; n++)
			{
				dCompGen[n]			= 0;	
				dCompWind[n]		= 0;
				dCompEarthQuake[n]	= 0;
				dCompFrameLoad[n]	= 0;
				dMaxStress[n]		= 0;
			}
			double dMoment				= 0;//작용모멘트
			double dShearForce			= 0;//작용전단력
			double dShearStress			= 0;//전단작용응력
			double dAllowShearStress	= 0;//허용전단응력
			double dBendStressFu		= 0;//휨응력상연(작용)
			double dAllowBendStressFu	= 0;//휩응력상연(허용)
			double dBendStressFl		= 0;//휨응력하연(작용)
			double dAllowBendStressFl	= 0;//휩응력하연(허용)
			
			if(pEndCr)
			{
				dAllowBendStressFu		= pEndCr->m_strtSection.m_dFca;				
				dAllowBendStressFl		= pEndCr->m_strtSection.m_dSigmaca;
				dAllowShearStress		= pEndCr->m_strtSection.m_dSheara;
				long n = 0;
				for(n=0; n<6;n++)
				{
					dCompGen[n]			= pEndCr->m_strtFrameLoadComp.m_dCompGen[n];
					dCompWind[n]		= pEndCr->m_strtFrameLoadComp.m_dCompWind[n];
					dCompEarthQuake[n]	= pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[n];
					dCompFrameLoad[n]	= pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[n];
				}
				
				for (n=0; n<6; n++)
				{
					dMaxStress[n]	= max((max(dCompGen[n],dCompWind[n])),(max(dCompEarthQuake[n],dCompFrameLoad[n])));
				}
				dMoment			= tokNM(dMaxStress[0]);
				dShearForce		= tokN(dMaxStress[1]);
				dBendStressFu	= dMaxStress[3];
				dBendStressFl	= dMaxStress[4];
				dShearStress	= dMaxStress[5];
			}
			if(pCenCr)
			{
				dAllowBendStressFu	= pCenCr->m_strtSection.m_dFca;				
				dAllowShearStress	= pCenCr->m_strtSection.m_dSheara;
				dMoment				= tokNM(max(fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), 
											  fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment)));
				dShearForce			= tokN(max(fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), 
											  fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinShear)));
				dShearStress		= pCenCr->m_strtSection.m_dShear;
				dAllowShearStress	= pCenCr->m_strtSection.m_dSheara;
				dBendStressFu		= pCenCr->m_strtSection.m_dFu;
				dAllowBendStressFu	= pCenCr->m_strtSection.m_dFca;
				dBendStressFl		= pCenCr->m_strtSection.m_dFl;
				dAllowBendStressFl	= pCenCr->m_strtSection.m_dSigmaca;
				
			}

			pOut->SetLineBoxText(pXL, sGubun,				m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dMoment,				m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dShearForce,			m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER);			
			pOut->SetLineBoxText(pXL, dShearStress,			m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER, "Is1" , 3);
			pOut->SetLineBoxText(pXL, dAllowShearStress,	m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER, "Is2" , 3);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is1"), pOut->GetCellRef("Is2"));
			pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dBendStressFu,		m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER, "Is3" , 3);
			pOut->SetLineBoxText(pXL, dAllowBendStressFu,	m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER, "Is4" , 3);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is3"), pOut->GetCellRef("Is4"));
			pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dBendStressFl,		m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "Is5" , 3);
			pOut->SetLineBoxText(pXL, dAllowBendStressFl,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "Is6" , 3);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is5"), pOut->GetCellRef("Is6"));
			pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		}
		case 3://3) 현장이음 검토
		{					
			CDesignCrossBeamJoint	Joint(pCalcStd, pBx);
			CDesignFlangeJoint		*pUpper		= Joint.m_pUpper;
			CDesignFlangeJoint		*pLower		= Joint.m_pLower;
			CDesignWebJoint			*pWebLeft	= Joint.m_pWebLeft;
			CDesignWebJoint			*pWebRight	= Joint.m_pWebRight;

			//복부판..좌측 우측 경우에 따라...
			BOOL bIsLeftCon			= TRUE;//사용이면 2 사용안함 이면 3
			BOOL bIsRightCon		= TRUE;

			if (pWebLeft->m_nType == 2)
				bIsLeftCon = TRUE;
			else
				bIsLeftCon = FALSE;

			if (pWebRight->m_nType == 2)
				bIsRightCon = TRUE;
			else
				bIsRightCon = FALSE;

			//거셋유무에 따라...
			long   nIsGuesset			= pUpper->m_nType;
			double dReqBoltEaFu			= 0;//필요볼트수 상부
			double dUseBoltEaFu			= 0;//사용볼트수 
			double dApplyLoadFu			= 0;//적용하중
			double dAllowLoadFu			= 0;//허용하중
			double dReqAreaFu			= 0;//필요단면적
			double dUseAreaFu			= 0;//사용단면적
			double dJApplyStressFu		= 0;//이음판작용응력
			double dJAllowStressFu		= 0;//이음판허용응력

			double dReqBoltEaFl			= 0;//필요볼트수 하부
			double dUseBoltEaFl			= 0;//사용볼트수 
			double dApplyLoadFl			= 0;//적용하중
			double dAllowLoadFl			= 0;//허용하중
			double dReqAreaFl			= 0;//필요단면적
			double dUseAreaFl			= 0;//사용단면적
			double dJApplyStressFl		= 0;//이음판작용응력
			double dJAllowStressFl		= 0;//이음판허용응력

			double dIApplyStressFl		= 0;//인장부 작용응력
			double dIAllowStressFl		= 0;//인장부 허용응력
			
			if(nIsGuesset == 0)//거셋방식일때
			{
				if(bIsLeftCon || bIsRightCon)//이음판이 좌우측중 하나라도 있으면...
				{
					dReqBoltEaFu		= pUpper->m_strtGusset.m_dBoltReq;									//필요볼트수 상부
					dUseBoltEaFu		= pUpper->m_strtGusset.m_dBoltUse;									//사용볼트수 
					dApplyLoadFu		= pUpper->m_strtGusset.m_dAxialLoad/dUseBoltEaFu;					//적용하중/EA
					dAllowLoadFu		= pUpper->m_strtGusset.m_dAllowBolt;								//허용하중/EA
					dReqAreaFu			= pUpper->m_strtGusset.m_dAreaFlange;								//필요단면적
					dUseAreaFu			= pUpper->m_strtGusset.m_dSumAs;									//사용단면적				
					dJApplyStressFu		= pUpper->m_strtGusset.m_dConStress;								//이음판작용응력
					dJAllowStressFu		= pUpper->m_strtGusset.m_dAllowStress;								//이음판허용응력 				

					dReqBoltEaFl		= pLower->m_strtGusset.m_dBoltReq;									//필요볼트수 하부
					dUseBoltEaFl		= pLower->m_strtGusset.m_dBoltUse;									//사용볼트수 
					dApplyLoadFl		= pLower->m_strtGusset.m_dAxialLoad/dUseBoltEaFu;					//적용하중
					dAllowLoadFl		= pLower->m_strtGusset.m_dAllowBolt;								//허용하중
					dReqAreaFl			= pLower->m_strtGusset.m_dAreaFlange;								//필요단면적
					dUseAreaFl			= pLower->m_strtGusset.m_dSumAs;									//사용단면적	
					dJApplyStressFl		= pLower->m_strtGusset.m_dConStress;								//이음판작용응력
					dJAllowStressFl		= pLower->m_strtGusset.m_dAllowStress;								//이음판허용응력				

					dIApplyStressFl		= 0;	//인장부 작용응력
					dIAllowStressFl		= 0;						//인장부 허용응력
				}				

				sText.Format("%s (컨넥션플레이트방식)", sGubun);
				pOut->SetXL(pXL, sText,						m_nSttRow, "B", "B",   1, BLACK, 9, FALSE, TA_LEFT);
			}
			else//기존방식일때
			{	
				if(bIsLeftCon || bIsRightCon)//이음판이 좌우측중 하나라도 있으면...
				{
					dReqBoltEaFu		= pUpper->m_strtPlate.m_dQtyBoltReq;	//필요볼트수 상부
					dUseBoltEaFu		= pUpper->m_strtPlate.m_dQtyBoltUse;	//사용볼트수 
					dApplyLoadFu		= pUpper->m_strtPlate.m_dRho;			//적용하중
					dAllowLoadFu		= pUpper->m_strtPlate.m_dAllowBolt*2;	//허용하중
					dReqAreaFu			= pUpper->m_strtPlate.m_dAf;			//필요단면적
					dUseAreaFu			= pUpper->m_strtPlate.m_dAsn;			//사용단면적				
					dJApplyStressFu		= pUpper->m_strtPlate.m_dDesignStress;	//이음판작용응력
					dJAllowStressFu		= pUpper->m_strtPlate.m_dAllowStress;	//이음판허용응력				

					dReqBoltEaFl		= pLower->m_strtPlate.m_dQtyBoltReq;	//필요볼트수 하부
					dUseBoltEaFl		= pLower->m_strtPlate.m_dQtyBoltUse;	//사용볼트수 
					dApplyLoadFl		= pLower->m_strtPlate.m_dRho;			//적용하중
					dAllowLoadFl		= pLower->m_strtPlate.m_dAllowBolt*2;	//허용하중
					dReqAreaFl			= pLower->m_strtPlate.m_dAf;			//필요단면적
					dUseAreaFl			= pLower->m_strtPlate.m_dAsn;			//사용단면적	
					dJApplyStressFl		= pLower->m_strtPlate.m_dDesignStress;	//이음판작용응력
					dJAllowStressFl		= pLower->m_strtPlate.m_dAllowStress;	//이음판허용응력				

					dIApplyStressFl		= pLower->m_strtPlate.m_dStressFlange;	//인장부 작용응력
					dIAllowStressFl		= dJAllowStressFl;						//인장부 허용응력
				}

				pOut->SetXL(pXL, sGubun,					m_nSttRow, "B", "B",   1, BLACK, 9, FALSE, TA_LEFT);		
			}

			pOut->SetLineBoxText(pXL, "구 분",						m_nSttRow, "B", "H",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "상부 플랜지",				m_nSttRow, "I", "N",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "하부 플랜지",				m_nSttRow, "O", "T",   2, BLACK, 9, FALSE, TA_CENTER);
			SetLines(2);
			pOut->SetLineBoxText(pXL, "볼 트",						m_nSttRow, "B", "D",   5, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "필요볼트수",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);

			pOut->SetLineBoxText(pXL, GetStrValue(dReqBoltEaFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu1", 1);
			pOut->SetLineBoxText(pXL, "개",							m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dReqBoltEaFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl1", 1);
			pOut->SetLineBoxText(pXL, "개",							m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "사용볼트수",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseBoltEaFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu2", 1);
			pOut->SetLineBoxText(pXL, "개",							m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseBoltEaFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl2", 1);
			pOut->SetLineBoxText(pXL, "개",							m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "작용하중/EA",				m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dApplyLoadFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu3", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dApplyLoadFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl3", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "허용하중/EA",				m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dAllowLoadFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu4", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dAllowLoadFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl4", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "판정",						m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);

			if(bIsLeftCon || bIsRightCon)			
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("Fu1"),
				pOut->GetCellRef("Fu2"), pOut->GetCellRef("Fu3"), pOut->GetCellRef("Fu4"));
			else
				sText = "-";			
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);

			if(bIsLeftCon || bIsRightCon)
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("Fl1"),
					pOut->GetCellRef("Fl2"), pOut->GetCellRef("Fl3"), pOut->GetCellRef("Fl4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "O", "T",   1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "이음판",						m_nSttRow, "B", "D",   5, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "필요단면적",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dReqAreaFu),		m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu1", 0);
			pOut->SetLineBoxText(pXL, "mm²",						 m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dReqAreaFl),		m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl1", 0);
			pOut->SetLineBoxText(pXL, "mm²",						 m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "사용단면적",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseAreaFu),		m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu2", 0);
			pOut->SetLineBoxText(pXL, "mm²",						 m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseAreaFl),		m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl2", 0);
			pOut->SetLineBoxText(pXL, "mm²",						 m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "작용응력",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJApplyStressFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu3", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJApplyStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl3", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "허용응력",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJAllowStressFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu4", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJAllowStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl4", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "판정",						m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			if(bIsLeftCon || bIsRightCon)
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("JFu1"),
					pOut->GetCellRef("JFu2"), pOut->GetCellRef("JFu3"), pOut->GetCellRef("JFu4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			if(bIsLeftCon || bIsRightCon)
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("JFl1"),
					pOut->GetCellRef("JFl2"), pOut->GetCellRef("JFl3"), pOut->GetCellRef("JFl4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "O", "T",   1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "",							m_nSttRow, "B", "D",   3, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "인장부",								m_nSttRow, "B", "D",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "작용응력",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "-",							m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetLineBoxText(pXL, GetStrValue(dIApplyStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "IFl3", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetXL(pXL, "모재응력",							m_nSttRow, "B", "D",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용응력",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "-",							m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetLineBoxText(pXL, GetStrValue(dIAllowStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "IFl4", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetXL(pXL, "검 토",								m_nSttRow, "B", "D",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",						m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "-",							m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			if(bIsLeftCon || bIsRightCon && (dIApplyStressFl != 0 && dIAllowStressFl != 0 ))
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")",
					pOut->GetCellRef("IFl3"), pOut->GetCellRef("IFl4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "O", "T",   1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(-14);
			
			//복부판..좌측 우측 경우에 따라...

			double dMaxBendShearW	= 0;//최대휨전단력 좌측이음판
			double dDirectShearW	= 0;//직접전단력
			double dMulShearW		= 0;//합성전단력
			double dAllowShearW		= 0;//허용전단력
			double dReqAreaW		= 0;//필요단면적 좌측
			double dUseAreaW		= 0;//사용단면적
			double dAppBendStressW	= 0;//작용휨응력좌측
			double dAllBendStressW	= 0;//허용휨응력
			double dAppShearStressW	= 0;//작용전단응력
			double dAllShearStressW	= 0;//허용전단응력

			double dReqBoltEaW		= 0;//필요볼트수
			double dUseBoleEaW		= 0;//사용볼트수
			
			pOut->SetLineBoxText(pXL, "복부판",			m_nSttRow, "U", "AP",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);

			if (bIsLeftCon)//왼쪽이 이음판이면...
			{		
				dMaxBendShearW		= pWebLeft->m_strtPlate.m_dRhom;				//최대휨전단력 이음판			
				dDirectShearW		= pWebLeft->m_strtPlate.m_dRhos;				//직접전단력
				dMulShearW			= pWebLeft->m_strtPlate.m_dRho;					//합성전단력
				dAllowShearW		= pWebLeft->m_strtPlate.m_dAllowBolt*2;			//허용전단력
				dReqAreaW			= pWebLeft->m_strtPlate.m_dReqArea;				//필요단면적 좌측
				dUseAreaW			= pWebLeft->m_strtPlate.m_dSumAs;				//사용단면적
				dAppBendStressW		= pWebLeft->m_strtPlate.m_dFMax;				//작용휨응력좌측
				dAllBendStressW		= pWebLeft->m_strtPlate.m_dAllowTension;		//허용휨응력
				dAppShearStressW	= pWebLeft->m_strtPlate.m_dApplyLoad;			//작용전단응력
				dAllShearStressW	= pWebLeft->m_strtPlate.m_dAllowShear;			//허용전단응력

				pOut->SetLineBoxText(pXL, "좌측 이음판",	m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "최대휨전단력",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMaxBendShearW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "직접 전단력",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dDirectShearW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "합성 전단력",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "허용 전단력",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dReqAreaW,		m_nSttRow, "U", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL5", 1);
				pOut->SetLineBoxText(pXL, "mm²",			 m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dUseAreaW,		m_nSttRow, "U", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL6", 1);
				pOut->SetLineBoxText(pXL, "mm²",			 m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAppBendStressW,	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER, "WL7", 1);
				pOut->SetLineBoxText(pXL, dAppShearStressW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL8", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAllBendStressW,	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER, "WL9", 1);
				pOut->SetLineBoxText(pXL, dAllShearStressW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL10", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL7"), pOut->GetCellRef("WL9"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL8"), pOut->GetCellRef("WL10"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "Z", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "휨응력",			m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, "전단응력",		m_nSttRow, "Z", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				SetLines(-13);
			}
			else//이음판 없는 경우
			{
				dReqBoltEaW		= pWebLeft->m_strtGusset.m_dBoltReq;					//필요볼트수
				dUseBoleEaW		= pWebLeft->m_strtGusset.m_dBoltUse;					//사용볼트수
				dMulShearW		= pWebLeft->m_strtGusset.m_dApplyLoad / dUseBoleEaW;	//합성전단력
				dAllowShearW	= pWebLeft->m_strtGusset.m_dAllowBolt;					//허용전단력

				pOut->SetLineBoxText(pXL, "좌측 이음판",	m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "필요 볼트수",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqBoltEaW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "개",				m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "사용 볼트수",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dUseBoleEaW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "개",				m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "합성 전단력",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "허용 전단력",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				SetLines(-13);
			}

			if (bIsRightCon)//오른쪽 이음판이면...
			{
				dMaxBendShearW		= pWebRight->m_strtPlate.m_dRhom;			//최대휨전단력 이음판			
				dDirectShearW		= pWebRight->m_strtPlate.m_dRhos;			//직접전단력
				dMulShearW			= pWebRight->m_strtPlate.m_dRho;			//합성전단력
				dAllowShearW		= pWebRight->m_strtPlate.m_dAllowBolt*2;	//허용전단력
				dReqAreaW			= pWebRight->m_strtPlate.m_dReqArea;		//필요단면적 좌측
				dUseAreaW			= pWebRight->m_strtPlate.m_dSumAs;			//사용단면적
				dAppBendStressW		= pWebRight->m_strtPlate.m_dFMax;			//작용휨응력좌측
				dAllBendStressW		= pWebRight->m_strtPlate.m_dAllowTension;	//허용휨응력
				dAppShearStressW	= pWebRight->m_strtPlate.m_dApplyLoad;		//작용전단응력
				dAllShearStressW	= pWebRight->m_strtPlate.m_dAllowShear;		//허용전단응력

				pOut->SetLineBoxText(pXL, "우측 이음판",	m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "최대휨전단력",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMaxBendShearW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "직접 전단력",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dDirectShearW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "합성 전단력",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "허용 전단력",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dReqAreaW,		m_nSttRow, "AF", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL5", 1);
				pOut->SetLineBoxText(pXL, "mm²",			 m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dUseAreaW,		m_nSttRow, "AF", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL6", 1);
				pOut->SetLineBoxText(pXL, "mm²",			 m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAppBendStressW,	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "WL7", 1);
				pOut->SetLineBoxText(pXL, dAppShearStressW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL8", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAllBendStressW,	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "WL9", 1);
				pOut->SetLineBoxText(pXL, dAllShearStressW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL10", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL7"), pOut->GetCellRef("WL9"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL8"), pOut->GetCellRef("WL10"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AK", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "휨응력",			m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, "전단응력",		m_nSttRow, "AK", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(3);						
			}
			else//이음판 없는 경우
			{
				dReqBoltEaW		= pWebRight->m_strtGusset.m_dBoltReq;					//필요볼트수
				dUseBoleEaW		= pWebRight->m_strtGusset.m_dBoltUse;					//사용볼트수
				dMulShearW		= pWebRight->m_strtGusset.m_dApplyLoad / dUseBoleEaW;	//합성전단력
				dAllowShearW	= pWebRight->m_strtGusset.m_dAllowBolt;					//허용전단력

				pOut->SetLineBoxText(pXL, "우측 이음판",	m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "필요 볼트수",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqBoltEaW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "개",				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "사용 볼트수",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dUseBoleEaW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "개",				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "합성 전단력",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "허용 전단력",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);						
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);	
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(3);			
			}
			break;
		}
		case 4://수직보강재
		{		
			BOOL bIsVStiff = FALSE;//수직보강재가 필요한지 여부...
			double dMaxGapRateV		= 1.5;//간격비 최대
			double dUseGapRateV		= 0;
			double dReqWidthMinV	= 0;
			double dUseWidthMinV	= 0;
			double dReqThickMinV	= 0;
			double dUseThickMinV	= 0;
			double dReqStrengthV	= 0;
			double dUseStrengthV	= 0;

			if(pEndCr)
			{
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(nQtyVerStiff !=0 && pEndCr->m_pVerStiff->m_bIsNeedVerStiff)
				{
					bIsVStiff = TRUE;				
					dUseGapRateV	= pEndCr->m_pVerStiff->m_dRateSpaceHeight;
					dReqWidthMinV	= pEndCr->m_pVerStiff->m_dBvsMin;
					dUseWidthMinV	= pEndCr->m_pVerStiff->m_dBvs;
					dReqThickMinV	= pEndCr->m_pVerStiff->m_dTvsMin;
					dUseThickMinV	= pEndCr->m_pVerStiff->m_dTvs;
					dReqStrengthV	= pEndCr->m_pVerStiff->m_dIReq;
					dUseStrengthV	= pEndCr->m_pVerStiff->m_dIUse;
				}
			}
			if(pCenCr)
			{
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(nQtyVerStiff !=0 && pCenCr->m_pVerStiff->m_bIsNeedVerStiff)
				{
					bIsVStiff = TRUE;				
					dUseGapRateV	= pCenCr->m_pVerStiff->m_dRateSpaceHeight;
					dReqWidthMinV	= pCenCr->m_pVerStiff->m_dBvsMin;
					dUseWidthMinV	= pCenCr->m_pVerStiff->m_dBvs;
					dReqThickMinV	= pCenCr->m_pVerStiff->m_dTvsMin;
					dUseThickMinV	= pCenCr->m_pVerStiff->m_dTvs;
					dReqStrengthV	= pCenCr->m_pVerStiff->m_dIReq;
					dUseStrengthV	= pCenCr->m_pVerStiff->m_dIUse;
				}

			}
			
			if (bIsVStiff)
			{						
				pOut->SetLineBoxText(pXL, sGubun,					m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMaxGapRateV,				m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER, "V1", 1);
				pOut->SetLineBoxText(pXL, dUseGapRateV,				m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER, "V2", 3);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V1"), pOut->GetCellRef("V2"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqWidthMinV,			m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER, "V3", 1);
				pOut->SetLineBoxText(pXL, dUseWidthMinV,			m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER, "V4", 1);
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V3"), pOut->GetCellRef("V4"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqThickMinV,			m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER, "V5", 1);
				pOut->SetLineBoxText(pXL, dUseThickMinV,			m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER, "V6", 1);
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V5"), pOut->GetCellRef("V6"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqStrengthV,			m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "V7", 0);
				pOut->SetLineBoxText(pXL, dUseStrengthV,			m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "V8", 0);
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V7"), pOut->GetCellRef("V8"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				
			}
			else
			{
				
			}
			break;
		}
		case 5://수평보강재
		{
			BOOL   bIsHStriff		= FALSE;//수평보강재가 필요한지 여부
			double dReqThickMinH	= 0;
			double dUseThickMinH	= 0;
			double dReqStrengthH	= 0;
			double dUseStrnghthH	= 0;
			double dFitPositionH	= 0;
			double dSetPoistionH	= 0;

			if(pEndCr)
			{			
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(pEndCr->m_nQtyHorStiff > 0 && (nQtyVerStiff !=0 && pEndCr->m_pVerStiff->m_bIsNeedVerStiff))
				{
					bIsHStriff = TRUE;				
					dReqThickMinH	= pEndCr->m_pHorStiff->m_dThsMin;
					dUseThickMinH	= pEndCr->m_pHorStiff->m_dThs;
					dReqStrengthH	= pEndCr->m_pHorStiff->m_dIReq;
					dUseStrnghthH	= pEndCr->m_pHorStiff->m_dIUse;
					dFitPositionH	= 0;
					dSetPoistionH	= 0;				
				}
			}
			if(pCenCr)
			{
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(pCenCr->m_nQtyHorStiff > 0 && (nQtyVerStiff !=0 && pCenCr->m_pVerStiff->m_bIsNeedVerStiff))
				{
					bIsHStriff = TRUE;				
					dReqThickMinH	= pCenCr->m_pHorStiff->m_dThsMin;
					dUseThickMinH	= pCenCr->m_pHorStiff->m_dThs;
					dReqStrengthH	= pCenCr->m_pHorStiff->m_dIReq;
					dUseStrnghthH	= pCenCr->m_pHorStiff->m_dIUse;
					dFitPositionH	= 0;
					dSetPoistionH	= 0;				
				}
			}

			if(bIsHStriff)
			{
				pOut->SetLineBoxText(pXL, sGubun,					m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqThickMinH,			m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER, "H1", 1);
				pOut->SetLineBoxText(pXL, dUseThickMinH,			m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER, "H2", 1);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("H1"), pOut->GetCellRef("H2"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqStrengthH,			m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER, "H3", 0);
				pOut->SetLineBoxText(pXL, dUseStrnghthH,			m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER, "H4", 0);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("H3"), pOut->GetCellRef("H4"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dFitPositionH,			m_nSttRow, "W", "AA",  1, BLACK, 9, FALSE, TA_CENTER, "H5", 0);
				pOut->SetLineBoxText(pXL, dSetPoistionH,			m_nSttRow, "AB", "AF", 1, BLACK, 9, FALSE, TA_CENTER, "H6", 0);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("H5"), pOut->GetCellRef("H6"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AG", "AH", 1, BLACK, 9, FALSE, TA_CENTER);	
				pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AI", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}
			break;
		}
	}//end switch
}

void CXLCheckListCrossBeam::CheckListTitle(int nSeq)
{
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	switch(nSeq)
	{
		case 1://사용강종 및 최소두께 검토
			pOut->SetXL(pXL, "가) 가로보 ", m_nSttRow, "A", "A");
			pOut->SetXL(pXL, "1) 사용강종 및 최소두께 검토", m_nSttRow, "B", "B");
				
			pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "강 종",					m_nSttRow, "E", "H",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "L",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "복부판 두께(mm)",		m_nSttRow, "M", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "상부플랜지 두께(mm)",	m_nSttRow, "U", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "하부플랜지 두께(mm)",	m_nSttRow, "AC", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "비 고",					m_nSttRow, "AK", "AP", 2, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetXL(pXL,"수평보강재",						m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL,"사용단수",							m_nSttRow, "I", "L",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "최소",					m_nSttRow, "M", "O",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "P", "R",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정", 					m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "최소", 					m_nSttRow, "U", "W",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용", 					m_nSttRow, "X", "Z",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정", 					m_nSttRow, "AA", "AB", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "최소", 					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용", 					m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정", 					m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			break;
		case 2://응력검토
			pOut->SetXL(pXL, "2) 응력 검토", m_nSttRow, "B", "B");

			pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "E", "H",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "L",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "전단응력 (MPa)", 		m_nSttRow, "M", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "휨 응 력 (MPa)", 		m_nSttRow, "W", "AP",  1, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetXL(pXL, "작용모멘트",						m_nSttRow, "E", "H",   0, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetXL(pXL, "작용전단력",						m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL,"(kN·m)",					 		 m_nSttRow, "E", "H",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL,"(kN)",								m_nSttRow, "I", "L",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "작용응력",				m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용응력",				m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "상연응력",				m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용응력",				m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "하연응력",				m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용응력",				m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		case 3:
			pOut->SetXL(pXL, "3) 현장이음 검토", m_nSttRow, "B", "B");
			break;
		case 4://수직보강재
			pOut->SetXL(pXL, "4) 수직보강재", m_nSttRow, "B", "B");
			pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "간격비(a/b)",			m_nSttRow, "E", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "보강재의 최소폭(mm)",	m_nSttRow, "M", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "보강재의 최소두께(mm)",	m_nSttRow, "W", "AF",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "강 성(mm⁴)", 			m_nSttRow, "AG", "AP", 1, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "최대",					m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "필요",					m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "필요",					m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "필요",					m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		case 5://수평보강재
			pOut->SetXL(pXL, "5) 수평보강재",					m_nSttRow, "B", "B", 0);
			pOut->SetXL(pXL, "수평보강재의 위치는 압축측으로부터의 거리 (mm)", 
																m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_LEFT);		
			pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "보강재의 최소두께(mm)",	m_nSttRow, "E", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "강 성(mm⁴)",			m_nSttRow, "M", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "설치위치의 적정성(mm)",	m_nSttRow, "W", "AH",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "비고", 					m_nSttRow, "AI", "AP", 2, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "필요",					m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "필요",					m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "적정위치",				m_nSttRow, "W", "AA",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "설치위치",				m_nSttRow, "AB", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AG", "AH", 1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			break;
		case 6://수직브레이싱...
			pOut->SetXL(pXL, "가) 수직브레이싱 ", m_nSttRow, "A", "A");
			pOut->SetXL(pXL, "1) 사용강종 및 응력 검토", m_nSttRow, "B", "B");
				
			pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "B", "F",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "강 종",					m_nSttRow, "G", "I",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "강 재 제 원",			m_nSttRow, "J", "P",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "세장비(≤150)",			 m_nSttRow, "Q", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "응력 검토 (MPa)",		m_nSttRow, "W", "AH",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "거셋트연결(MPa)",		m_nSttRow, "AI", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AO", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "ℓ/γmin",				  m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "압축",					m_nSttRow, "W", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용",					m_nSttRow, "Z", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "인장",					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용",					m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "작용",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용",					m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		case 7://거세트 플레이트의 검토
			pOut->SetXL(pXL, "2) 거셋트 플레이트의 검토", m_nSttRow, "B", "B");
			
			pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "B", "I",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "거셋트판 응력(MPa)",		m_nSttRow, "J", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "볼트수 (개)",			m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "용접부 전단응력 (MPa)",  m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "작용응력",				m_nSttRow, "J", "M",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용응력",				m_nSttRow, "N", "Q",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "R", "T",   1, BLACK, 9, FALSE, TA_CENTER);				
			pOut->SetLineBoxText(pXL, "필요수",					m_nSttRow, "U", "X",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용수",					m_nSttRow, "Y", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "작용응력",				m_nSttRow, "AF", "AI", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용응력",				m_nSttRow, "AJ", "AM", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AN", "AP", 1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			break;
		case 8://수평브레이싱..
			pOut->SetXL(pXL, "나) 수평브레이싱 ", m_nSttRow, "A", "A");
			pOut->SetXL(pXL, "1) 사용강종 및 응력 검토", m_nSttRow, "B", "B");
				
			pOut->SetLineBoxText(pXL, "위 치",					m_nSttRow, "B", "F",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "강 종",					m_nSttRow, "G", "I",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "강 재 제 원",			m_nSttRow, "J", "P",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "세장비(≤150)",			 m_nSttRow, "Q", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "응력 검토 (MPa)",		m_nSttRow, "W", "AH",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "연결부볼트(개)",			m_nSttRow, "AI", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "AO", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "ℓ/γmin",				  m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "판정",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "압축",					m_nSttRow, "W", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용",					m_nSttRow, "Z", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "인장",					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "허용",					m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "작용",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "사용",					m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
	}//end switch	
}

void CXLCheckListCrossBeam::CorssBeamBracing(int nSeq)
{
	CPlateBridgeApp		*pDB = m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	BOOL bOnlyFirst = TRUE;	
		
	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++)//1;nG++)//
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		CString sGubun;

		int nCrossCount = 1;
		
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();			
			if(pSec)
			{	
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				
				// 0=V형수직브레이싱, 1=역V형수직브레이싱, 2= 가로보
				if(pSec->GetenDetType()==SECDET_VBRACING) // 브레이싱 타입..
				{		
					if (bOnlyFirst)
					{
						CheckListTitle(nSeq);
						bOnlyFirst = FALSE;
					}

					if(nCrossCount < 10)
						sGubun.Format("%dBR0%d", nG+1,nCrossCount);
					else
						sGubun.Format("%dBR%d", nG+1,nCrossCount);
					
					CrossBeamBracingDetail(nSeq, pBx, sGubun);					
				}
			}	
			nCrossCount++;
			pBx = Finder.GetBxNext();
		}	
	}

	if(bOnlyFirst && nSeq == 6)
	{
		pOut->SetXL(pXL, "수직 브레이싱 타입이 없습니다. ", m_nSttRow, "A", "A");		
	}

}

void CXLCheckListCrossBeam::CrossBeamBracingDetail(int nSeq, CPlateBasicIndex *pBx, CString sGubun)
{
	CAPlateCalcStd			*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl				*pXL			= m_pXL;
	CAPlateOutCalcXL		*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	
	CDesignCrossBeam		CrossBeam(pCalcStd, pBx);
	CDesignVBracing			*pVBracing		= NULL;
	CDesignInverseVBracing	*pInVBr			= NULL;
	
	CString sText = _T("");

	if(pBx->GetSection()->IsenType(SECTION_A))
		pInVBr		= (CDesignInverseVBracing*)CrossBeam.GetDesignCrossBeam();
	else
		pVBracing	= (CDesignVBracing*)CrossBeam.GetDesignCrossBeam();

	CString sType[3];
	CString sMaterial[3];
	CString sJeWon[3];				//강재제원
	double  dSlenderness[3];		//세장비
	double  dCompressStress[3];		//압축응력
	double  dComAllowStress[3];		//압축허용
	double  dTensionStress[3];		//인장응력
	double  dTenAllowStress[3];		//인장허용
	double  dGussetStress[3];		//거셋작용
	double  dGuAllowStress[3];		//거셋허용

	double  dReqBoltEA[3];			//필요볼트수
	double  dUseBoltEa[3];			//사용볼트수
	double  dWeldStress[3];			//용접부작용응력
	double  dWeldAllowStress[3];	//용접부허용응력
	
	for (int n=0; n<3; n++)			//초기화
	{
		sMaterial[n]		= "";
		sJeWon[n]			= "";
		dSlenderness[n]		= 0;
		dCompressStress[n]	= 0;	
		dComAllowStress[n]	= 0;	
		dTensionStress[n]	= 0;	
		dTenAllowStress[n]	= 0;	
		dGussetStress[n]	= 0;	
		dGuAllowStress[n]	= 0;
		
		dReqBoltEA[n]		= 0;		
		dUseBoltEa[n]		= 0;		
		dWeldStress[n]		= 0;		
		dWeldAllowStress[n]	= 0;
	}
	
	switch(nSeq)
	{
		case 6://수직브레이싱..사용 강종및 응력검토.
		{			
			sType[0]="상현";
			sType[1]="사재";
			sType[2]="하현";
			
			if(pVBracing)
			{
				//상현재
				int n = 0;
				sMaterial[n]		= pVBracing->m_strtChordUp.m_strMaterial;
				sJeWon[n]			= pVBracing->m_strtChordUp.m_sMaterialType;
				dSlenderness[n]		= pVBracing->m_strtChordUp.m_dSlendernessRatio;
				dCompressStress[n]	= pVBracing->m_strtChordUp.m_dFc;
				dComAllowStress[n]	= pVBracing->m_strtChordUp.m_dFcaApo;
				dTensionStress[n]	= 0;
				dTenAllowStress[n]	= 0;	
				dGussetStress[n]	= pVBracing->m_strtChordUp.m_dShear/2;
				dGuAllowStress[n]	= pVBracing->m_strtChordUp.m_dAllowShear;

				//사재
				n++;
				sMaterial[n]		= pVBracing->m_strtDiagonal.m_strMaterial;
				sJeWon[n]			= pVBracing->m_strtDiagonal.m_sMaterialType;
				dSlenderness[n]		= pVBracing->m_strtDiagonal.m_dSlendernessRatio;
				dCompressStress[n]	= pVBracing->m_strtDiagonal.m_dFc;
				dComAllowStress[n]	= pVBracing->m_strtDiagonal.m_dFcaApo;
				dTensionStress[n]	= pVBracing->m_strtDiagonal.m_dFt;
				dTenAllowStress[n]	= pVBracing->m_strtDiagonal.m_dFta;
				dGussetStress[n]	= pVBracing->m_strtDiagonal.m_dShear/2;
				dGuAllowStress[n]	= pVBracing->m_strtDiagonal.m_dAllowShear;

				//하현재
				n++;
				sMaterial[n]		= pVBracing->m_strtChordLo.m_strMaterial;
				sJeWon[n]			= pVBracing->m_strtChordLo.m_sMaterialType;
				dSlenderness[n]		= pVBracing->m_strtChordLo.m_dSlendernessRatio;
				dCompressStress[n]	= pVBracing->m_strtChordLo.m_dFc;
				dComAllowStress[n]	= pVBracing->m_strtChordLo.m_dFcaApo;
				dTensionStress[n]	= 0;
				dTenAllowStress[n]	= 0;	
				dGussetStress[n]	= pVBracing->m_strtChordLo.m_dShear/2;
				dGuAllowStress[n]	= pVBracing->m_strtChordLo.m_dAllowShear;
			}
			if(pInVBr)
			{
				//상현재
				int n=0;
				sMaterial[n]		= pInVBr->m_strtChordUp.m_strMaterial;
				sJeWon[n]			= pInVBr->m_strtChordUp.m_sMaterialType;
				dSlenderness[n]		= pInVBr->m_strtChordUp.m_dSlendernessRatio;
				dCompressStress[n]	= pInVBr->m_strtChordUp.m_dFc;
				dComAllowStress[n]	= pInVBr->m_strtChordUp.m_dFcaApo;
				dTensionStress[n]	= pInVBr->m_strtChordUp.m_dF;
				dTenAllowStress[n]	= pInVBr->m_strtChordUp.m_dFa;
				dGussetStress[n]	= pInVBr->m_strtChordUp.m_dShear;
				dGuAllowStress[n]	= pInVBr->m_strtChordUp.m_dAllowShear;

				//사재
				n++;
				sMaterial[n]		= pInVBr->m_strtDiagonal.m_strMaterial;
				sJeWon[n]			= pInVBr->m_strtDiagonal.m_sMaterialType;
				dSlenderness[n]		= pInVBr->m_strtDiagonal.m_dSlendernessRatio;
				dCompressStress[n]	= pInVBr->m_strtDiagonal.m_dFc;
				dComAllowStress[n]	= pInVBr->m_strtDiagonal.m_dFcaApo;
				dTensionStress[n]	= pInVBr->m_strtDiagonal.m_dFt;
				dTenAllowStress[n]	= pInVBr->m_strtDiagonal.m_dFta;
				dGussetStress[n]	= pInVBr->m_strtDiagonal.m_dShear;
				dGuAllowStress[n]	= pInVBr->m_strtDiagonal.m_dAllowShear;

				//하현재
				n++;
				sMaterial[n]		= pInVBr->m_strtChordLo.m_strMaterial;
				sJeWon[n]			= pInVBr->m_strtChordLo.m_sMaterialType;
				dSlenderness[n]		= pInVBr->m_strtChordLo.m_dSlendernessRatio;
				dCompressStress[n]	= pInVBr->m_strtChordLo.m_dFc;
				dComAllowStress[n]	= pInVBr->m_strtChordLo.m_dFcaApo;
				dTensionStress[n]	= pInVBr->m_strtChordLo.m_dFt;
				dTenAllowStress[n]	= pInVBr->m_strtChordLo.m_dFta;
				dGussetStress[n]	= pInVBr->m_strtChordLo.m_dShear;
				dGuAllowStress[n]	= pInVBr->m_strtChordLo.m_dAllowShear;
				
			}
			//실제 엑셀에 쓰기...
			pOut->SetLineBoxText(pXL, sGubun,					m_nSttRow, "B", "D",   3, BLACK, 9, FALSE, TA_CENTER);
			for(long n=0; n<3; n++)
			{
				pOut->SetLineBoxText(pXL, sType[n],				m_nSttRow, "E",  "F",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, sMaterial[n],			m_nSttRow, "G",  "I",  1, BLACK, 9, FALSE, TA_CENTER);					
				pOut->SetLineBoxText(pXL, sJeWon[n],			m_nSttRow, "J",  "P",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dSlenderness[n],		m_nSttRow, "Q",  "T",  1, BLACK, 9, FALSE, TA_CENTER, "F1", 1);
				sText.Format("=IF((%s<=150),\"O.K\",\"N.G\")",pOut->GetCellRef("F1"));
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "U",  "V",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dCompressStress[n],	m_nSttRow, "W",  "Y",  1, BLACK, 9, FALSE, TA_CENTER, "F2", 1);
				pOut->SetLineBoxText(pXL, dComAllowStress[n],	m_nSttRow, "Z", "AB", 1, BLACK, 9, FALSE, TA_CENTER, "F3", 1);			
				pOut->SetLineBoxText(pXL, dTensionStress[n],	m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER, "F4", 1);
				pOut->SetLineBoxText(pXL, dTenAllowStress[n],	m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "F5", 1);
				pOut->SetLineBoxText(pXL, dGussetStress[n],		m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER, "F6", 1);
				pOut->SetLineBoxText(pXL, dGuAllowStress[n],	m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "F7", 1);
				sText.Format("=IF(AND((%s<=%s),(%s<=%s),(%s<=%s)),\"O.K\",\"N.G\")", 
					pOut->GetCellRef("F2"),pOut->GetCellRef("F3"),pOut->GetCellRef("F4"),
					pOut->GetCellRef("F5"),pOut->GetCellRef("F6"),pOut->GetCellRef("F7"));
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}

			break;
		}
		case 7://2) 거세트 플레이트의 검토		
		{
			if(pVBracing)
			{
				sType[0]="상현재/사재";
				sType[1]="하현재";
				sType[2]="사재/하현재";

				//상현재/사재
				int n=0;										
				dGussetStress[n]	= pVBracing->m_strtGussetUp.m_dStress;
				dGuAllowStress[n]	= pVBracing->m_strtGussetUp.m_dAllowStress;
				dReqBoltEA[n]		= pVBracing->m_strtGussetUp.m_dQtyBoltReq;
				dUseBoltEa[n]		= pVBracing->m_strtGussetUp.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;

				//하현재
				n++;
				dGussetStress[n]	= pVBracing->m_strtGussetLo.m_dStress;
				dGuAllowStress[n]	= pVBracing->m_strtGussetLo.m_dAllowStress;
				dReqBoltEA[n]		= pVBracing->m_strtGussetLo.m_dQtyBoltReq;
				dUseBoltEa[n]		= pVBracing->m_strtGussetLo.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;

				//사재/하현재
				n++;
				dGussetStress[n]	= pVBracing->m_strtGussetDiagonal.m_dShear;
				dGuAllowStress[n]	= pVBracing->m_strtGussetDiagonal.m_dAllowShear;
				dReqBoltEA[n]		= 0;
				dUseBoltEa[n]		= 0;
				dWeldStress[n]		= pVBracing->m_strtGussetDiagonal.m_dShearWeld;
				dWeldAllowStress[n]	= pVBracing->m_strtGussetDiagonal.m_dAllowShearWeld;			
			}
			if(pInVBr)
			{
				sType[0]="상현재";
				sType[1]="사재/상현재";
				sType[2]="하현재/사재";

				//상현재
				int n=0;										
				dGussetStress[n]	= pInVBr->m_strtGussetUp.m_dStress;
				dGuAllowStress[n]	= pInVBr->m_strtGussetUp.m_dAllowStress;
				dReqBoltEA[n]		= pInVBr->m_strtGussetUp.m_dQtyBoltReq;
				dUseBoltEa[n]		= pInVBr->m_strtGussetUp.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;

				//사재/상현재
				n++;

				//Guesset Plate의 응력검토시 작용응력은 [응력/허용응력] [전단응력/허용전단응력] 중 
				//큰값을 표기한다. (060913일자...이종원차장 감수)
				double dF_Rate = pInVBr->m_strtGussetDiagonal.m_dAllowStress/pInVBr->m_strtGussetDiagonal.m_dStress;
				double dV_Rate = pInVBr->m_strtGussetDiagonal.m_dAllowShear/pInVBr->m_strtGussetDiagonal.m_dShearGusset; 

				if (dF_Rate > dV_Rate)
				{
					dGussetStress[n]	= pInVBr->m_strtGussetDiagonal.m_dStress;
					dGuAllowStress[n]	= pInVBr->m_strtGussetDiagonal.m_dAllowStress;
				}
				else
				{
					dGussetStress[n]	= pInVBr->m_strtGussetDiagonal.m_dShearGusset;
					dGuAllowStress[n]	= pInVBr->m_strtGussetDiagonal.m_dAllowShear;
				}						
				
				dReqBoltEA[n]		= 0;
				dUseBoltEa[n]		= 0;
				dWeldStress[n]		= pInVBr->m_strtGussetDiagonal.m_dShearWeld;
				dWeldAllowStress[n]	= pInVBr->m_strtGussetDiagonal.m_dAllowShearWeld;	

				//하현재/사재
				n++;
				dGussetStress[n]	= pInVBr->m_strtGussetLo.m_dStress;
				dGuAllowStress[n]	= pInVBr->m_strtGussetLo.m_dAllowStress;
				dReqBoltEA[n]		= pInVBr->m_strtGussetLo.m_dQtyBoltReq;
				dUseBoltEa[n]		= pInVBr->m_strtGussetLo.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;
				
			}
			//실제 엑셀에 쓰기...
			pOut->SetLineBoxText(pXL, sGubun,								m_nSttRow, "B", "D",   3, BLACK, 9, FALSE, TA_CENTER);
			for(long n=0; n<3; n++)
			{
				pOut->SetLineBoxText(pXL, sType[n],							m_nSttRow, "E",  "I",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dGussetStress[n],					m_nSttRow, "J",  "M",  1, BLACK, 9, FALSE, TA_CENTER, "G1", 3);
				pOut->SetLineBoxText(pXL, dGuAllowStress[n],				m_nSttRow, "N",  "Q",  1, BLACK, 9, FALSE, TA_CENTER, "G2", 3);
				sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")",pOut->GetCellRef("G1"), pOut->GetCellRef("G2"));
				pOut->SetLineBoxText(pXL, sText,							m_nSttRow, "R",  "T",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, GetStrValue(dReqBoltEA[n]),		m_nSttRow, "U",  "X",  1, BLACK, 9, FALSE, TA_CENTER, "G3", 1);
				pOut->SetLineBoxText(pXL, GetStrValue(dUseBoltEa[n]),		m_nSttRow, "Y",  "AB", 1, BLACK, 9, FALSE, TA_CENTER, "G4", 1);
				if (dReqBoltEA[n] ==0)
					sText = "-";
				else
					sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")",
						pOut->GetCellRef("G3"), pOut->GetCellRef("G4"));
				pOut->SetLineBoxText(pXL, sText,							m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, GetStrValue(dWeldStress[n]),		m_nSttRow, "AF", "AI", 1, BLACK, 9, FALSE, TA_CENTER, "G5", 3);
				pOut->SetLineBoxText(pXL, GetStrValue(dWeldAllowStress[n]),	m_nSttRow, "AJ", "AM", 1, BLACK, 9, FALSE, TA_CENTER, "G6", 3);
				if (dWeldStress[n] ==0)
					sText = "-";
				else
					sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")",
						pOut->GetCellRef("G5"), pOut->GetCellRef("G6"));
				pOut->SetLineBoxText(pXL, sText,							m_nSttRow, "AN", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}

			break;
		}
	}
}

void CXLCheckListCrossBeam::HorBracing(int nSeq)
{
	CPlateBridgeApp		*pDB = m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	BOOL bOnlyFirst = TRUE;
	BOOL bOutValid = FALSE;
	for(long nG = 0 ; nG < pDB->GetGirdersu() - 1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);	
		
		for(long nBr=0; nBr<pGir->m_BracingArr.GetSize() ; nBr++)
		{
			if (bOnlyFirst == TRUE)
			{
				CheckListTitle(nSeq);
				bOnlyFirst = FALSE;
			} 
			
			if(HorBracingDetail(nSeq, nG, nBr) == TRUE)
			{
				bOutValid = TRUE;
			}
		}		
	}

	if(bOutValid == FALSE)
	{
		pXL->DeleteRowLine(m_nSttRow - 6, m_nSttRow - 1);	//수평 브레이싱 테이블 타이틀을 삭제함.
		pOut->SetXL(pXL, "수평 브레이싱 타입이 없습니다. ", m_nSttRow, "A", "A");		
		pXL->DeleteRowLineEnd();
	}

}

BOOL CXLCheckListCrossBeam::HorBracingDetail(int nSeq, long nG, long nBr)
{
	CPlateBridgeApp			*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd			*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl				*pXL			= m_pXL;
	CAPlateOutCalcXL		*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CBracing				*pHBracing		= pDB->GetGirder(nG)->GetBracingInTotal(nBr);
	long nCur = nBr;
	BOOL bOutValid = FALSE;
	BOOL bContinue = TRUE;

	if(pHBracing == NULL) 
		return bOutValid;

	CPlateBasicIndex *pBx = pHBracing ? pHBracing->GetBx() : NULL;
	
	if(pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2) == NULL) 
		return bOutValid;

	CSteelMaterialData	*pMatData	= pHBracing->GetSteelMaterialData();

	if(pMatData->m_dH == 0 || pMatData->m_dB == 0 || pMatData->m_dT1 == 0)
	{
		while (pBx)
		{
			pHBracing = pDB->GetGirder(nG)->GetBracingInTotal(++nCur);
			if (pHBracing == NULL)
			{
				bContinue=FALSE;
				break;
			}

			pBx = pHBracing->GetBx();
			pMatData	= pHBracing->GetSteelMaterialData();
			if (pMatData->m_dH && pMatData->m_dB && pMatData->m_dT1) 
			{
				bOutValid = TRUE;
				break;
			}
		}

		if (bContinue == FALSE)  
		{
			bOutValid = FALSE;
			return bOutValid;
		}
	}

	CDesignHorBracing	HorBracing(pCalcStd, nG, nCur);
	CString sText = _T("");
	CString sMaterial		= HorBracing.m_strMaterial;
	CString sJeWon			= HorBracing.m_strMaterialType;		//강재제원
	double  dSlenderness	= HorBracing.m_dSlendernessRatio;	//세장비
	double  dCompressStress	= HorBracing.m_dFc;					//압축응력
	double  dComAllowStress	= HorBracing.m_dFcaApo;				//압축허용
	double  dTensionStress	= HorBracing.m_dFt;					//인장응력
	double  dTenAllowStress = HorBracing.m_dFta;				//인장허용
	double  dReqBoltEA		= HorBracing.m_dQtyBoltReq;			//필요볼트수
	double  dUseBoltEA		= HorBracing.m_dQtyBoltUse;			//사용볼트수
	
	sText.Format("G%d-%02d", nG+1, nBr+1);
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "B",   "D", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "하현",				m_nSttRow, "E",   "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, sMaterial,			m_nSttRow, "G",   "I", 1, BLACK, 9, FALSE, TA_CENTER);					
	pOut->SetLineBoxText(pXL, sJeWon,				m_nSttRow, "J",   "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, dSlenderness,			m_nSttRow, "Q",   "T", 1, BLACK, 9, FALSE, TA_CENTER, "F1", 1);
	sText.Format("=IF((%s<=150),\"O.K\",\"N.G\")",pOut->GetCellRef("F1"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "U",   "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, dCompressStress,		m_nSttRow, "W",   "Y", 1, BLACK, 9, FALSE, TA_CENTER, "F2", 1);
	pOut->SetLineBoxText(pXL, dComAllowStress,		m_nSttRow, "Z",  "AB", 1, BLACK, 9, FALSE, TA_CENTER, "F3", 1);			
	pOut->SetLineBoxText(pXL, dTensionStress,		m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER, "F4", 1);
	pOut->SetLineBoxText(pXL, dTenAllowStress,		m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "F5", 1);
	pOut->SetLineBoxText(pXL, dReqBoltEA,			m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER, "F6", 1);
	pOut->SetLineBoxText(pXL, dUseBoltEA,			m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "F7", 1);
	sText.Format("=IF(AND((%s<=%s),(%s<=%s),(%s<=%s)),\"O.K\",\"N.G\")", 
		pOut->GetCellRef("F2"),pOut->GetCellRef("F3"),pOut->GetCellRef("F4"),
		pOut->GetCellRef("F5"),pOut->GetCellRef("F6"),pOut->GetCellRef("F7"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	
	return bOutValid;
}

void CXLCheckListCrossBeam::CrossBeamJewon()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CGeneralBaseStd		*pInStd			= m_pAPlateOutCalcStd->GetGeneralInputStd();
	
	pOut->SetXL(pXL, "1) 사용강종 및 최소두께 검토", m_nSttRow, "B", "B");

	long nQtyCr, SectionType,nTypeRow;
	CString sTypeName, sTypeName2;
	CString sText;

	for (long nType = 0 ; nType < 3; nType++)
	{
		switch(nType)
		{
		case  0:				
				SectionType = SECTION_A;
				sTypeName =  "A";
				break;

		case  1:				
				SectionType = SECTION_P;
				sTypeName =  "P";
				break;
//		case  2:				
//				SectionType = SECTION_B;
//				sTypeName =  "B";
//				break;
		case  2:				
				SectionType = SECTION_C;
				sTypeName =  "C";
				break;				
		}

		nTypeRow = m_nSttRow;
		sText.Format("<%s Type - 제작타입>",sTypeName);
		pOut->SetXL(pXL, sText,  m_nSttRow, "B", "B");
		pOut->SetLineBoxText(pXL, "구 분",		m_nSttRow, "B", "E", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "강 종",		m_nSttRow, "F", "I", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",			m_nSttRow, "J", "M", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "복부판 두께(mm)", m_nSttRow, "N", "U", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "상부플랜지 두께(mm)", m_nSttRow, "V", "AC", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "하부플랜지 두께(mm)", m_nSttRow, "AD", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비 고", m_nSttRow, "AL", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
		//SetLines(NEXT);
		pOut->SetXL(pXL,"수평보강재",		m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"사용단수",			m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "최소", m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "사용", m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판정", m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "최소", m_nSttRow, "v", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "사용", m_nSttRow, "Y", "AA", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판정", m_nSttRow, "AB", "AC", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "최소", m_nSttRow, "AD", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "사용", m_nSttRow, "AG", "AI", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판정", m_nSttRow, "AJ", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		
		SetLines(NEXT);
		
		for(long nG=0; nG < pBridgeApp->GetGirdersu()-1; nG++)
		{
			CPlateGirderApp	*pGir	= pBridgeApp->GetGirder(nG);
			
			switch(nType)
			{
			case  0:
					nQtyCr	= pGir->GetQtyCrossAType();
				    break;
			case  1:
					nQtyCr	= pGir->GetQtyCrossPType();
				    break;
//			case  2:
//					nQtyCr	= pGir->GetQtyCrossBType();	   
//				    break;
			case  2:
					nQtyCr	= pGir->GetQtyCrossCType();
				    break;				
			}			

			for(long nCr=0; nCr<nQtyCr; nCr++)
			{
				CPlateBasicIndex	*pBx = pGir->GetBxBySectionType(SectionType, nCr);
				CPlateStressOut	pStressOut(pInStd);
 
				CStringArray Arr;
				CMap <CString, LPCTSTR, double, double> Map;
				pStressOut.SectionCheckOutput(Arr, pBx, Map);
				
				if (nCr == 0)
				{
					sTypeName2 = RetCrossBeamType(Map["MType"], pBx);					
				}				

				sText.Format("G%d-G%d-%s%d", nG+1, nG+2, sTypeName, nCr+1);
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER);

				//Map["JuType"]
				CString sJuType    = pBridgeApp->GetMaterial(CP_F_U, pBx);
				pOut->SetLineBoxText(pXL, sJuType,			m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, Map["HStiff"],	m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER,"",0);
				pOut->SetLineBoxText(pXL, Map["Twmin"],		m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER,"TW");
				pOut->SetLineBoxText(pXL, Map["Tw"],		m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER,"TWMIN");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("TW"), pOut->GetCellRef("TWMIN"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Tfumin"],	m_nSttRow, "V", "X", 1, BLACK, 9, FALSE, TA_CENTER,"TFU");
				pOut->SetLineBoxText(pXL, Map["Tfu"],		m_nSttRow, "Y", "AA", 1, BLACK, 9, FALSE, TA_CENTER,"TFUMIN");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("TFU"), pOut->GetCellRef("TFUMIN"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AB", "AC", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Tflmin"],	m_nSttRow, "AD", "AF", 1, BLACK, 9, FALSE, TA_CENTER,"TFL");
				pOut->SetLineBoxText(pXL, Map["Tfl"],		m_nSttRow, "AG", "AI", 1, BLACK, 9, FALSE, TA_CENTER,"TFLMIN");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("TFL"), pOut->GetCellRef("TFLMIN"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AJ", "AK", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, "",			m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}			
		}//end for nG
		sText.Format("<%s Type - %s>",sTypeName, sTypeName2);
		pOut->SetXL(pXL, sText,  nTypeRow, "B", "B");
		SetLines(NEXT);
	}//end for nType
	
}

void CXLCheckListCrossBeam::CrossBeamMoment()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CGeneralBaseStd	*pInStd			= m_pAPlateOutCalcStd->GetGeneralInputStd();
	
	pOut->SetXL(pXL, "2) 응력 검토", m_nSttRow, "B", "B");

	long nQtyCr, SectionType,nTypeRow;
	CString sTypeName, sTypeName2;
	CString sText;

	for (long nType = 0 ; nType < 3; nType++)
	{
		switch(nType)
		{
		case  0:				
				SectionType = SECTION_A;
				sTypeName =  "A";
				break;

		case  1:				
				SectionType = SECTION_P;
				sTypeName =  "P";
				break;
//		case  2:				
//				SectionType = SECTION_B;
//				sTypeName =  "B";
//				break;
		case  2:				
				SectionType = SECTION_C;
				sTypeName =  "C";
				break;				
		}

		nTypeRow = m_nSttRow;
		sText.Format("<%s Type - 제작타입>",sTypeName);
		pOut->SetXL(pXL, sText,  m_nSttRow, "B", "B");
		pOut->SetLineBoxText(pXL, "구 분",			m_nSttRow, "B", "E", 3, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "F", "I", 3, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "J", "M", 3, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "전단응력 (MPa)", m_nSttRow, "N", "V", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "휨 응 력 (MPa)", m_nSttRow, "W", "AN", 1, BLACK, 9, FALSE, TA_CENTER);		
		pOut->SetLineBoxText(pXL, "비고",			m_nSttRow, "AO", "AP", 3, BLACK, 9, FALSE, TA_CENTER);
		//SetLines(NEXT);
		pOut->SetXL(pXL,"작  용",					m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"작  용",					m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"모멘트",					m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"전단력",					m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "N", "P", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "Q", "S", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판 정",			m_nSttRow, "T", "V", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "W", "Y", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "Z", "AB", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판 정",			m_nSttRow, "AC", "AE", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AH", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AI", "AK", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "판 정",			m_nSttRow, "AL", "AN", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"작 용",					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"허 용",					m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"상 연",					m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"허 용",					m_nSttRow, "Z", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"하 연",					m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"허 용",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL,"(kN·m)",					 m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"(kN)",						m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"응 력",					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"응 력",					m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"응 력",					m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"응 력",					m_nSttRow, "Z", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"응 력",					m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"응 력",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		//SetLines(NEXT);
		
		for(long nG=0; nG < pBridgeApp->GetGirdersu()-1; nG++)
		{
			CPlateGirderApp	*pGir	= pBridgeApp->GetGirder(nG);
			
			switch(nType)
			{
			case  0:
					nQtyCr	= pGir->GetQtyCrossAType();
				    break;
			case  1:
					nQtyCr	= pGir->GetQtyCrossPType();
				    break;
//			case  2:
//					nQtyCr	= pGir->GetQtyCrossBType();	   
//				    break;
			case  2:
					nQtyCr	= pGir->GetQtyCrossCType();
				    break;				
			}			

			for(long nCr=0; nCr<nQtyCr; nCr++)
			{
				CPlateBasicIndex	*pBx = pGir->GetBxBySectionType(SectionType, nCr);
				CPlateStressOut	pStressOut(pInStd);
 
				CStringArray Arr;
				CMap <CString, LPCTSTR, double, double> Map;
				pStressOut.SectionCheckOutput(Arr, pBx, Map);
				
				if (nCr == 0)
				{
					sTypeName2 = RetCrossBeamType(Map["MType"], pBx);					
				}				

				sText.Format("G%d-G%d-%s%d", nG+1, nG+2, sTypeName, nCr+1);
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER);

				//Map["JuType"]
				//CString sJuType    = pBridgeApp->GetMaterial(CP_F_U, pBx);
				pOut->SetLineBoxText(pXL, Map["Mmu"],		m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, Map["Msu"],		m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, Map["Mv"],		m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER,"MV");
				pOut->SetLineBoxText(pXL, Map["Mva"],		m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER,"MVA");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("MV"), pOut->GetCellRef("MVA"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "T", "V", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Mfu"],		m_nSttRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER,"MFU");
				pOut->SetLineBoxText(pXL, Map["Mfca"],		m_nSttRow, "Z", "AB", 1, BLACK, 9, FALSE, TA_CENTER,"MFCA");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("MFU"), pOut->GetCellRef("MFCA"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Mfl"],		m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER,"MFL");
				pOut->SetLineBoxText(pXL, Map["Mfta"],		m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER,"MFTA");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("MFL"), pOut->GetCellRef("MFTA"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}			
		}//end for nG
		sText.Format("<%s Type - %s>",sTypeName, sTypeName2);
		pOut->SetXL(pXL, sText,  nTypeRow, "B", "B");
		SetLines(NEXT);
	}//end for nType
}

CString CXLCheckListCrossBeam::RetCrossBeamType(double sType, CPlateBasicIndex *pBx)
{
	if (sType == 2)
	{
		switch (pBx->GetSection()->GetCrossBeam()->GetType())
		{
			case CR_TYPE_HSTEEL:
				return "형강타입";
			case CR_TYPE_MANUFACTURE:
				return "제작 타입";
			case CR_TYPE_VSTIFF:
				return "수직보강재연결";
		}
	}
	else
	{
		switch (pBx->GetSection()->GetVBracing()->GetType())
		{
			case TYPE_VBRACING:
				return "브레이싱(V형)";
			case TYPE_INVVBRACING:
				return "브레이싱(역V형)";			
		}		
	}
	return "";
}

void CXLCheckListCrossBeam::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}

CString CXLCheckListCrossBeam::GetStrValue(double dValue)
{
	CString str;
	if(dValue == 0)	
		str.Format("'-");
	else 
		str.Format("%.3f", dValue);
	return str;
}