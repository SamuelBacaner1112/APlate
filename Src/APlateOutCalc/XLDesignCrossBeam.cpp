// XLDesignCrossBeam.cpp: implementation of the CXLDesignCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcStd.h"
#include "XLDesignCrossBeam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

CXLDesignCrossBeam::CXLDesignCrossBeam(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
}

CXLDesignCrossBeam::~CXLDesignCrossBeam()
{
}

void CXLDesignCrossBeam::DesignVBracingOut(CStringArray &strArrUseSheet, long &nIdx)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CFemModelingCalc    ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc      StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString szSheetName		= _T("");
	double  dProgress		= 20/pDB->GetQtyGirder();

	pOut->ClearCellRef();	
	pOutExcel->SetProgress("7. 가로보의 설계", 50);
	StressCalc.SetSectionCrossBeamPos();

	// 단부 (역V형 수직브레이싱)
	pXL->SetActiveSheet("7.1단부수직브레이싱(역V형)");
	BOOL	bFirst			= TRUE;
	long	nRow			= 3;
	long	nG				= 0;
	for(nG=0; nG<pDB->GetQtyGirder()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			pOutExcel->SetProgress("7. 수직브레이싱의 설계", 50+(long)dProgress*(nG+1));
			CSection	*pSec	= pBx->GetSection();
			CVBracing	*pVBr	= pSec->GetVBracing();
			if(pOutExcel->m_bAbort)	return;
			if(pVBr)
			{
				if(pSec->IsenType(SECTION_A) && pVBr->IsSectionCheck())
				{
					if(!bFirst)	pXL->InsertCopyRowLine(3, 282, nRow);
					DesignInverseVBracing(pBx, nRow);
					bFirst = FALSE;
				}
			}
			pBx = Finder.GetBxNext();
		}
	}

	if(!bFirst)
	{
		szSheetName.Format("7.%d 단부수직브레이싱(역V형)", nIdx);
		long nRowTitle = 2;
		pOut->SetXL(pXL, szSheetName, nRowTitle, "B", "B");
		pXL->SetSheetName(szSheetName);
		strArrUseSheet.Add(szSheetName);
		nIdx++;
	}
	pXL->DeleteRowLineEnd();

	// 중간부
	pXL->SetActiveSheet("7.2중간수직브레이싱(V형)");
	bFirst			= TRUE;
	nRow			= 2;
	for(nG=0; nG<pDB->GetQtyGirder()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			pOutExcel->SetProgress("7. 수직브레이싱의 설계", 70+(long)dProgress*(nG+1));
			CSection	*pSec	= pBx->GetSection();
			CVBracing	*pVBr	= pSec->GetVBracing();
			if(pOutExcel->m_bAbort)	return;
			if(pVBr)
			{
				if(!pSec->IsenType(SECTION_A) && pVBr->IsSectionCheck())
				{
					if(!bFirst)	pXL->InsertCopyRowLine(2, 198, nRow);
					DesignVBracing(pBx, nRow);
					bFirst = FALSE;
				}
			}
			pBx = Finder.GetBxNext();
		}
	}

	if(!bFirst)
	{
		szSheetName.Format("7.%d 중간수직브레이싱(V형)", nIdx);
		long nRowTitle = 1;
		pOut->SetXL(pXL, szSheetName, nRowTitle, "B", "B");
		pXL->SetSheetName(szSheetName);
		strArrUseSheet.Add(szSheetName);
		nIdx++;
	}
	pXL->DeleteRowLineEnd();
}

void CXLDesignCrossBeam::DesignCrossBeam(CStringArray &strArrUseSheet, long &nIdx)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CFemModelingCalc    ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc      StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	StressCalc.SetSectionCrossBeamPos();
	pOut->ClearCellRef();
	pOutExcel->SetProgress("7. 가로보의 설계", 10);

	BOOL	bFirst			= TRUE;
	long	nRow			= 3;
	CString szSheetName		= _T("");
	double  dProgress		= 20/pDB->GetQtyGirder();
	
	pXL->SetActiveSheet("7.1상면접합가로보");
	long nG = 0;
	for(nG=0; nG<pDB->GetQtyGirder()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		
		while(pBx)
		{
			pOutExcel->SetProgress("7. 가로보의 설계", 10+(long)dProgress*(nG+1));
			CSection		*pSec	= pBx->GetSection();
			CPlateCrossBeam	*pCr	= pSec->GetCrossBeam();
			if(pOutExcel->m_bAbort)	return;
			if(pCr)
			{
				if(pCr->m_bHunchTopConnect && pCr->IsSectionCheck())
				{
					if(!bFirst)	pXL->InsertCopyRowLine(3, 168, nRow);
					DesignCrossBeamDetail(pBx, nRow);
					bFirst	= FALSE;
				}
			}
			pBx = Finder.GetBxNext();
		}
	}
	if(!bFirst)
	{
		szSheetName.Format("7.%d 가로보", nIdx);
		long nRowTitle = 2;
		pOut->SetXL(pXL, szSheetName, nRowTitle, "B", "B");
		pXL->SetSheetName(szSheetName);
		strArrUseSheet.Add(szSheetName);
		nIdx++;
	}
	pXL->DeleteRowLineEnd();


	bFirst			= TRUE;
	nRow			= 2;
	pOut->ClearCellRef();
	pXL->SetActiveSheet("7.2상면미접합가로보");
	for(nG=0; nG<pDB->GetQtyGirder()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			pOutExcel->SetProgress("7. 가로보의 설계", 30+(long)dProgress*(nG+1));
			CSection		*pSec	= pBx->GetSection();
			CPlateCrossBeam	*pCr	= pSec->GetCrossBeam();
			if(pOutExcel->m_bAbort)	return;
			if(pCr)
			{
				if(!pCr->m_bHunchTopConnect && pCr->IsSectionCheck())
				{
					if(!bFirst)	pXL->InsertCopyRowLine(2, 89, nRow);
					DesignCrossBeamDetail(pBx, nRow);	// 단부가로보(H형강,제작타입)
					bFirst	= FALSE;
					nRow++;
				}
			}
			pBx = Finder.GetBxNext();
		}
	}
	if(!bFirst)
	{
		szSheetName.Format("7.%d 가로보", nIdx);
		long nRowTitle = 1;
		if(nIdx==1)
		{
			pXL->InsertRowLine(1);
			pOut->SetXL(pXL, "7. 브레이싱 및 가로보의 설계", nRowTitle, "A", "A");
		}		
		pOut->SetXL(pXL, szSheetName, nRowTitle, "B", "B");

		pXL->SetSheetName(szSheetName);
		strArrUseSheet.Add(szSheetName);
		nIdx++;
	}
	pXL->DeleteRowLineEnd();
}

void CXLDesignCrossBeam::DesignCrossBeamDetail(CPlateBasicIndex *pBx, long &nRow)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();		
	CAPlateCalcStd		*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData	= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CGlobarOption		*pOpt		= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CAPlateOutExcel		*pOutExcel	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CPlateCrossBeam		*pCrossBeam	= pBx->GetCrossBeam();	
	CDesignCrossBeam	CrossBeam(pCalcStd, pBx);
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());	
	CCentSeparation	    *pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CXLControl			*pXL		= m_pXL;
	CSteel				*pSteelCtl	= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();

	CString strLeft, strRight;
	CString	sText			= _T("");
	long	nNumCr			= pBx->GetNumberCrossBeam();
	long	nG				= pBx->GetNumberGirder();
	double	dStation		= pBx->GetStation();
	long	nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
	
	CDesignEndCrossBeam		*pCrEnd	= NULL;
	CDesignCenterCrossBeam	*pCrCen	= NULL;
	BOOL bEnd		= pBx->GetSection()->IsenType(SECTION_A) ? TRUE : FALSE;
	BOOL bConnect	= pCrossBeam->m_bHunchTopConnect;

	double dUpWidth=0, dLoWidth=0, dWebH=0, dWebT=0, dUpT=0, dLoT=0, dSpaceG=0;
	double dPreM=0, dPreS=0, dPreN=0, dPosM=0, dPosS=0, dPosN;
	double dLiveMaxM=0, dLiveMinM=0, dLiveMaxS=0, dLiveMinS=0, dLiveMaxN=0, dLiveMinN=0;
	double dSetMaxM=0, dSetMinM=0, dSetMaxS=0, dSetMinS=0, dSetMaxN=0, dSetMinN=0;
	BOOL	bIsNeedVerStiff	= FALSE;
	CString	szMaterial		= _T("");
	double	dSpaceVStiff=0, dBvs=0, dTvs=0;	
	double	dSpaceHStiff=0, dBhs=0,	dThs=0;
	long	nQtyHStiff=0, nSideVer=0, nSideHor=0;

	// 단부제원
	CString sGradeEarth, sRegionEarth;
	double	dThickSlab=0, dThickPave=0, dUnitPave=0, dUnitConc=0, dPr=0, dDistHVrToEndSlab=0;
	double	dBridgeW=0, dLenSpan=0, dReaction=0;
	long	nGradeEarth=0, nRegionEarth=0;
	double	dHeightGirder=0, dSlabThick=0;
	double	dHeightGuard=0, dHeightGuardFence=0, dHeightSoundProof=0, dHeightHandRail=0;

	dThickSlab			= toM(pGir->GetThickSlab(pBx, 0));						//상판두께
	dThickPave			= toM(pDB->m_dThickPave);								//포장두께
	dUnitPave			= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt);	//아스팔트 단위중량
	dUnitConc			= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun);	//철근콘크리트 단위중량
	dPr					= tokN(pCalcData->DESIGN_CONDITION.m_dPr);				//DB하중

	nGradeEarth			= pOpt->GetIdxDegreeEarthQuake();
	nRegionEarth		= pOpt->GetIdxRegionEarthQuake();
	
	if(nGradeEarth==0)	sGradeEarth = "I";
	else				sGradeEarth = "II";
	if(nRegionEarth==0)	sRegionEarth = "I";
	else				sRegionEarth = "II";

	dHeightGirder		= toM(pBx->GetHeightGirderByBx());	//거더높이
	dSlabThick			= toM(pDB->m_dThickSlabBase);		//슬래브두께
	dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;
	
	if(bEnd)
	{
		pCrEnd = (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
		
		dDistHVrToEndSlab	= toM(pCrEnd->m_strtVerLoad.m_dDistVerBracingToEndSlab);	//수직브레이싱에서 상판단부까지의 거리		
		dBridgeW			= toM(pCrEnd->m_strtHorLoad.m_dWidthBridge);				//교량폭원
		dLenSpan			= toM(pCrEnd->m_strtHorLoad.m_dLengthSpan);					//해당지간장
		dReaction			= tokN(pCrEnd->m_strtEarthQuakeLoad.m_dReaction);		

		dUpWidth		= pCrEnd->m_pJewonFlangeUp->m_dWidth;		// 플랜지폭
		dLoWidth		= pCrEnd->m_pJewonFlangeLo->m_dWidth;		// 플랜지폭
		dWebH			= pCrEnd->m_pJewonWeb->m_dHeight;			// 복부폭
		nQtyHStiff		= pCrEnd->m_nQtyHorStiff;					// 수평보강재 사용단수
		dWebT			= pCrEnd->m_pJewonWeb->m_dWidth;			// 복부두께
		dUpT			= pCrEnd->m_pJewonFlangeUp->m_dHeight;		// 상부두께
		dLoT			= pCrEnd->m_pJewonFlangeLo->m_dHeight;		// 하부두께
		dSpaceG			= toM(pCrEnd->m_strtVerLoad.m_dGirderSpace);// 주형간격
		dPreM			= tokNM(pCrEnd->m_pFrameElementLoad->m_strtMoment.m_dPreDeadLoad);		// 합성전 고정하중 M
		dPreS			= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad);		// 합성전 고정하중 S
		dPreN			= tokN(pCrEnd->m_pFrameElementLoad->m_strtAxial.m_dPreDeadLoad);		// 합성전 고정하중 N
		dPosM			= tokNM(pCrEnd->m_pFrameElementLoad->m_strtMoment.m_dPostDeadLoad);		// 합성후 고정하중 M
		dPosS			= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad);		// 합성후 고정하중 S
		dPosN			= tokN(pCrEnd->m_pFrameElementLoad->m_strtAxial.m_dPostDeadLoad);		// 합성후 고정하중 N
		dLiveMaxM		= tokNM(pCrEnd->m_pFrameElementLoad->m_strtMoment.m_dMaxLiveLoad);		// 활하중 M - Max
		dLiveMinM		= tokNM(pCrEnd->m_pFrameElementLoad->m_strtMoment.m_dMinLiveLoad);		// 활하중 M - Min
		dLiveMaxS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad);		// 활하중 S - Max
		dLiveMinS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad);		// 활하중 S - Min
		dLiveMaxN		= tokN(pCrEnd->m_pFrameElementLoad->m_strtAxial.m_dMaxLiveLoad);		// 활하중 N - Max
		dLiveMinN		= tokN(pCrEnd->m_pFrameElementLoad->m_strtAxial.m_dMinLiveLoad);		// 활하중 N - Min
		dSetMaxM		= tokNM(pCrEnd->m_pFrameElementLoad->m_strtMoment.m_dMaxSettlement);	// 지점침하 M - Max
		dSetMinM		= tokNM(pCrEnd->m_pFrameElementLoad->m_strtMoment.m_dMinSettlement);	// 지점침하 M - Min
		dSetMaxS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement);		// 지점침하 S - Max
		dSetMinS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMinSettlement);		// 지점침하 S - Min
		dSetMaxN		= tokN(pCrEnd->m_pFrameElementLoad->m_strtAxial.m_dMaxSettlement);		// 지점침하 N - Max
		dSetMinN		= tokN(pCrEnd->m_pFrameElementLoad->m_strtAxial.m_dMinSettlement);		// 지점침하 N - Min
		bIsNeedVerStiff	= pCrEnd->m_pVerStiff->m_bIsNeedVerStiff;
		szMaterial		= pCrEnd->m_pVerStiff->m_strMaterial;
		dSpaceVStiff	= pCrEnd->m_pVerStiff->m_dVerStiffSpace;
		dBvs			= pCrEnd->m_pVerStiff->m_dBvs;
		dTvs			= pCrEnd->m_pVerStiff->m_dTvs;
		nSideVer		= pCrEnd->m_pVerStiff->m_nVerSide;
		dSpaceHStiff	= pCrEnd->m_pHorStiff->m_dVerStiffSpace;
		dBhs			= pCrEnd->m_pHorStiff->m_dBhs;
		dThs			= pCrEnd->m_pHorStiff->m_dThs;
		nSideHor		= pCrEnd->m_pHorStiff->m_nHorSide;
	}
	else
	{
		pCrCen = (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();

		long nSpanIdx		= pGir->GetNumberJiganByStation(dStation);

		// 지점이 아닐때 dReaction 이 0으로 들어감.. (아직 정확한 값을 알 수 없음. 수정필요. 080519)
		dDistHVrToEndSlab	= toM(pDB->m_dLengthGirderShoeStt);
		dBridgeW			= toM(pDB->m_dBridgeWidth);
		dLenSpan			= toM(pDB->m_dLengthJigan[nSpanIdx]);	//해당지간장

		dUpWidth		= pCrCen->m_pJewonFlangeUp->m_dWidth;		// 플랜지폭
		dLoWidth		= pCrCen->m_pJewonFlangeLo->m_dWidth;		// 플랜지폭
		dWebH			= pCrCen->m_pJewonWeb->m_dHeight;			// 복부폭
		nQtyHStiff		= pCrCen->m_nQtyHorStiff;					// 수평보강재 사용단수
		dWebT			= pCrCen->m_pJewonWeb->m_dWidth;			// 복부두께
		dUpT			= pCrCen->m_pJewonFlangeUp->m_dHeight;		// 상부두께
		dLoT			= pCrCen->m_pJewonFlangeLo->m_dHeight;		// 하부두께
		dSpaceG			= toM(pCrCen->m_dGirderSpace);				// 주형간격
		dPreM			= tokNM(pCrCen->m_pFrameElementLoad->m_strtMoment.m_dPreDeadLoad);		// 합성전 고정하중 M
		dPreS			= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad);		// 합성전 고정하중 S
		dPreN			= tokN(pCrCen->m_pFrameElementLoad->m_strtAxial.m_dPreDeadLoad);		// 합성전 고정하중 N
		dPosM			= tokNM(pCrCen->m_pFrameElementLoad->m_strtMoment.m_dPostDeadLoad);		// 합성후 고정하중 M
		dPosS			= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad);		// 합성후 고정하중 S
		dPosN			= tokN(pCrCen->m_pFrameElementLoad->m_strtAxial.m_dPostDeadLoad);		// 합성후 고정하중 N
		dLiveMaxM		= tokNM(pCrCen->m_pFrameElementLoad->m_strtMoment.m_dMaxLiveLoad);		// 활하중 M - Max
		dLiveMinM		= tokNM(pCrCen->m_pFrameElementLoad->m_strtMoment.m_dMinLiveLoad);		// 활하중 M - Min
		dLiveMaxS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad);		// 활하중 S - Max
		dLiveMinS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad);		// 활하중 S - Min
		dLiveMaxN		= tokN(pCrCen->m_pFrameElementLoad->m_strtAxial.m_dMaxLiveLoad);		// 활하중 N - Max
		dLiveMinN		= tokN(pCrCen->m_pFrameElementLoad->m_strtAxial.m_dMinLiveLoad);		// 활하중 N - Min
		dSetMaxM		= tokNM(pCrCen->m_pFrameElementLoad->m_strtMoment.m_dMaxSettlement);	// 지점침하 M - Max
		dSetMinM		= tokNM(pCrCen->m_pFrameElementLoad->m_strtMoment.m_dMinSettlement);	// 지점침하 M - Min
		dSetMaxS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement);		// 지점침하 S - Max
		dSetMinS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMinSettlement);		// 지점침하 S - Min
		dSetMaxN		= tokN(pCrCen->m_pFrameElementLoad->m_strtAxial.m_dMaxSettlement);		// 지점침하 N - Max
		dSetMinN		= tokN(pCrCen->m_pFrameElementLoad->m_strtAxial.m_dMinSettlement);		// 지점침하 N - Min
		bIsNeedVerStiff	= pCrCen->m_pVerStiff->m_bIsNeedVerStiff;
		szMaterial		= pCrCen->m_pVerStiff->m_strMaterial;
		dSpaceVStiff	= pCrCen->m_pVerStiff->m_dVerStiffSpace;
		dBvs			= pCrCen->m_pVerStiff->m_dBvs;
		dTvs			= pCrCen->m_pVerStiff->m_dTvs;
		nSideVer		= pCrCen->m_pVerStiff->m_nVerSide;
		dSpaceHStiff	= pCrCen->m_pHorStiff->m_dVerStiffSpace;
		dBhs			= pCrCen->m_pHorStiff->m_dBhs;
		dThs			= pCrCen->m_pHorStiff->m_dThs;
		nSideHor		= pCrCen->m_pHorStiff->m_nHorSide;
	}
		
	CalcGeneral.GetMaterial(pBx, CG_F_U, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowWebMinT(pSteel);	// 복부판 최소두께
	CHGAllowStress	AllowTension(pSteel);	// 허용축방향 인장응력, 허용압축응력

	double dAllowWebMinT	= AllowWebMinT.GetAllowStress(ALLOW_PLATEBRIDGE_WEBMINTHICK, nQtyHStiff);
	double dAllowTensionUp	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dUpT);

	if(bEnd)	sText.Format("< 단부가로보 : G%d - G%d C%d >", nG+1, nG+2, nNumCr+1);
	else		sText.Format("< 하중분배 중간가로보 : G%d - G%d C%d >", nG+1, nG+2, nNumCr+1);
	pOut->SetXL(pXL, sText,				nRow, "B", "B", 2, BLACK, 9);	
	pOut->SetXL(pXL, strLeft,			nRow, "U", "X", 0, BLACK, 9);
	pOut->SetXL(pXL, strRight,			nRow, "Y", "Y", 0, BLACK, 9);
	pOut->SetXL(pXL, dAllowTensionUp,	nRow,"AF","AG", 1, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dUpWidth,			nRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dLoWidth,			nRow,"AF","AG", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dWebH,				nRow, "V", "W", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, nQtyHStiff,		nRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "QTY_HSTIFF", 0);
	pOut->SetXL(pXL, dWebT,				nRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("=IF(%s>=3,\"\",%.f)", pOut->GetCellRef("QTY_HSTIFF"), dAllowWebMinT);
	pOut->SetXL(pXL, sText,				nRow,"AB","AC", 4, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dUpT,				nRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dLoT,				nRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

	if(bConnect)
	{
		nRow+=4;
		pOut->SetXL(pXL, dThickPave,		nRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);//포장두께..
		pOut->SetXL(pXL, dThickSlab,		nRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);//상판두께..
		pOut->SetXL(pXL, dSpaceG,			nRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);//주형간격
		pOut->SetXL(pXL, dDistHVrToEndSlab,	nRow, "R", "T", 5, BLACK, 9, FALSE, TA_CENTER, "", 3);//수직브레이싱에서 상판단부까지의 거리
	}
	else
		pOut->SetXL(pXL, dSpaceG,			nRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);//주형간격	

	nRow+=5;

	if(bConnect)
	{
		pOut->SetXL(pXL, dUnitPave,			nRow, "K", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	//아스팔트 단위중량
		pOut->SetXL(pXL, dUnitConc,			nRow, "K", "M", 7, BLACK, 9, FALSE, TA_CENTER, "", 3);	//철근콘크리트 단위중량.
		pOut->SetXL(pXL, dPr,				nRow, "Y", "Z", 14, BLACK, 9, FALSE, TA_CENTER, "", 0);	//DB하중에 따른 후륜하중 강도입력
		pOut->SetXL(pXL, dBridgeW,			nRow, "I", "K", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	//교량폭원
		pOut->SetXL(pXL, dHeightGirder,		nRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//거더높이
		pOut->SetXL(pXL, dSlabThick,		nRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//슬래브두께
		pOut->SetXL(pXL, toM(dHeightGuardFence+dHeightSoundProof+dHeightHandRail),
											nRow, "Q", "S", 5, BLACK, 9, FALSE, TA_CENTER, "", 3);	//방호벽+방음벽+난간..
		pOut->SetXL(pXL, dLenSpan,			nRow, "L", "N", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	//해당지간장
		pOut->SetXL(pXL, pDB->GetGirdersu(),nRow, "L", "M", 5, BLACK, 9, FALSE, TA_CENTER, "", 0);	//거더수
		pOut->SetXL(pXL, dReaction,			nRow, "P", "S", 2, BLACK, 9, FALSE, TA_CENTER, "", 3);	//A1 또는 A2 등 해당 지점에 작용하는 반력의 합계 
		pOut->SetXL(pXL, sGradeEarth,		nRow, "H", "H", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	//내진등급
		pOut->SetXL(pXL, sRegionEarth,		nRow, "H", "H", 34, BLACK, 9, FALSE, TA_CENTER, "", 3);	//내진구역
	}

	pOut->SetXL(pXL, dPreM,	nRow, "K", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);		//합성전 고정하중 M
	pOut->SetXL(pXL, dPreS,	nRow, "S", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);		//합성전 고정하중 S
	pOut->SetXL(pXL, dPreN,	nRow, "AA", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	//합성전 고정하중 N
	//비합성인 경우 합성후 고정하중 라인 삭제...
	if (pDB->IsHapsung())
	{
		pOut->SetXL(pXL, dPosM,	nRow, "K", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//합성후 고정하중 M
		pOut->SetXL(pXL, dPosS,	nRow, "S", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//합성후 고정하중 S
		pOut->SetXL(pXL, dPosN,	nRow, "AA", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);//합성후 고정하중 N	
	}
	else
	{
		pXL->DeleteRowLine(nRow, nRow);
		nRow+=1;
	}	
	
	pOut->SetXL(pXL, dLiveMaxM,	nRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//활하중 M - Max
	pOut->SetXL(pXL, dLiveMinM,	nRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//활하중 M - Min
	pOut->SetXL(pXL, dLiveMaxS,	nRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//활하중 s - Max
	pOut->SetXL(pXL, dLiveMinS,	nRow, "W", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//활하중 s - Min
	pOut->SetXL(pXL, dLiveMaxN,	nRow, "AA", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);//활하중 n - Max
	pOut->SetXL(pXL, dLiveMinN,	nRow, "AE", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);//활하중 n - Min
	pOut->SetXL(pXL, dSetMaxM,	nRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//지점침하 M - Max
	pOut->SetXL(pXL, dSetMinM,	nRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//지점침하 M - Min
	pOut->SetXL(pXL, dSetMaxS,	nRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//지점침하 s - Max
	pOut->SetXL(pXL, dSetMinS,	nRow, "W", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	//지점침하 s - Min
	pOut->SetXL(pXL, dSetMaxN,	nRow, "AA", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);//지점침하 n - Max
	pOut->SetXL(pXL, dSetMinN,	nRow, "AE", "AH", 15, BLACK, 9, FALSE, TA_CENTER, "", 3);//지점침하 n - Min

	if(pOutExcel->m_bAbort)	return;
	//////////////////////////////////////////////////////////////////////////
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress AllowBuckling(pSteel);		// 자유돌출판의 국부좌굴에 대한 허용응력
	CHGAllowStress AllowBuckCoeff(pSteel);		// 자유돌출판의 국부좌굴에 대한 허용응력계수
	CHGAllowStress AllowBendingComp2(pSteel);	// 허용휨압축응력 3.3.4(b)-1
	CHGAllowStress AllowShearStress(pSteel);	// 허용전단응력
	CHGAllowStress AllowFlangeSpace(pSteel);	// 수직보강재를 생략할 수 있는 플랜지 순간격의 최대치	

	double dCoeff		= AllowBuckCoeff.GetAllowStress(ALLOW_BUCKLINGCOEFF, dLoT);
	double dBuckling	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dLoT);
	double dAllowTensionLo = AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dLoT);
	double dAllowShear	= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dWebT);
	double dFlangeSpace = AllowFlangeSpace.GetAllowStress(ALLOW_VSTIFFNOT, 0);
	pOut->SetXL(pXL, dCoeff,			nRow, "F", "H", 4, RED, 9, FALSE, TA_CENTER, "", 1);

	double dValue1		= atof(pXL->GetXL(nRow-2, 14));
	double dValue2		= atof(pXL->GetXL(nRow-2, 32));
	if(dValue1<=dValue2)
		pXL->DeleteRowLineDirect(nRow, nRow);
	else
	{
		double dAw	= atof(pXL->GetXL(nRow-4, 7));
		double dAc	= atof(pXL->GetXL(nRow-4, 11));
		AllowBendingComp2.SetTypeAllow(ALLOW_BENDINGCOMP_CASE2);
		AllowBendingComp2.SetAllowBendingStress(dAw, dAc, frM(dSpaceG), dUpWidth);
		HGALLOWSTRESS	vtAllowStress = AllowBendingComp2.m_AllowStress[AllowBendingComp2.GetIdxByThick(dUpT)];
		pOut->SetXL(pXL, vtAllowStress.m_dCoeff1,	nRow, "F", "H", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, vtAllowStress.m_dCoeff3,	nRow, "K", "L", 0, RED, 9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, vtAllowStress.m_dCoeff2,	nRow, "R", "S", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	}
	
	pOut->SetXL(pXL, dAllowTensionLo,	nRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dAllowShear,		nRow,"AD","AE",15, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dAllowShear,		nRow, "I", "K", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dFlangeSpace,		nRow, "D", "E", 2, RED, 9, FALSE, TA_CENTER, "", 0);	
	
	//////////////////////////////////////////////////////////////////////////	

	BOOL bChkVerStiff;
	if(nQtyVerStiff !=0 && bIsNeedVerStiff)
	{
		bChkVerStiff = TRUE;
		pOut->SetXL(pXL, szMaterial,	nRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	//수직보강재 재질
		pOut->SetXL(pXL, dSpaceVStiff,	nRow, "N", "P", 3, BLACK, 9, FALSE, TA_CENTER, "", 1);	//수직보강재 간격
		pOut->SetXL(pXL, dBvs,			nRow, "G", "I", 2, BLACK, 9, FALSE, TA_CENTER, "", 1);	//수직보강재 폭
		pOut->SetXL(pXL, dTvs,			nRow, "G", "I", 12, BLACK, 9, FALSE, TA_CENTER, "", 1);	//수직보강재 두깨
		pOut->SetXL(pXL, nSideVer,		nRow, "R", "S", 3, BLACK, 9, FALSE, TA_CENTER, "", 0);	//본			
	}
	else
	{
		//--------수직 보강재...없으면 삭제....
		bChkVerStiff = FALSE;

		if(bConnect)
		{
			pXL->DeleteRowLine(nRow, nRow+19);
			nRow+=20;
		}
		else
		{
			pXL->DeleteRowLine(nRow, nRow+20);
			nRow+=21;
		}		
	}	

	if (nQtyHStiff < 1 || bChkVerStiff != TRUE) //수평보강재 없음... 수직보강재 없으면
	{
		pXL->DeleteRowLine(nRow, nRow+8);
		nRow+=9;
	}
	else
	{
		CalcGeneral.GetMaterial(pBx, CP_H, strLeft, strRight);//
		pOut->SetXL(pXL, strLeft+strRight,						nRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	//수평보강재 재질
		pOut->SetXL(pXL, dSpaceHStiff,		nRow, "R", "T", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);	//수직보강재 간격
		pOut->SetXL(pXL, dBhs,				nRow, "R", "T", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);	//수평보강재 폭
		pOut->SetXL(pXL, dThs,				nRow, "R", "T", 4, BLACK, 9, FALSE, TA_CENTER, "", 1);	//수평보강재 두께
		pOut->SetXL(pXL, nSideHor,			nRow, "Q", "R", 4, BLACK, 9, FALSE, TA_CENTER, "", 3);	//본	
	}
}

void CXLDesignCrossBeam::DesignInverseVBracing(CPlateBasicIndex *pBx, long &nRow)
{
	CPlateBridgeApp			*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd			*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData				*pCalcData	= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CGlobarOption			*pGlobarOpt	= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CAPlateOutExcel			*pOutExcel	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CPlateGirderApp			*pGir		= pBx->GetGirder();
	CCentSeparation			*pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	CXLControl				*pXL		= m_pXL;
	CAPlateOutCalcXL		*pOut		= m_pAPlateOutCalcXL;
	CCalcGeneral			CalcGen(m_pAPlateOutCalcStd->GetDataManage());
	CDesignCrossBeam		CrossBeam(pCalcStd, pBx);
	CDesignInverseVBracing	*pInVBr		= (CDesignInverseVBracing*)CrossBeam.GetDesignCrossBeam();
	CSteel					*pSteelCtl	= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();
	CString	strText				= _T("");
	CString	strLeft				= _T("");
	CString	strRight			= _T("");

	long	nBrNum				= pBx->GetNumberCrossBeam();
	long	nG					= pBx->GetNumberGirder();
	double	dPr					= pCalcData->DESIGN_CONDITION.m_dPr;
	double	dUnitPave			= pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt;
	double	dUnitConc			= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;
	double	dThickSlab			= pGir->GetThickSlab(pBx, 0);
	double	dThickPave			= pDB->m_dThickPave;
	double	dHeightGirder		= pBx->GetHeightGirderByBx();
	double	dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double	dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double	dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double	dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;
	long	nGradeEarth			= pGlobarOpt->GetIdxDegreeEarthQuake();
	long	nRegionEarth		= pGlobarOpt->GetIdxRegionEarthQuake();
	double	dUpT				= pInVBr->m_pChordUp->m_dThick1;
	double	dSdT				= pInVBr->m_pDiagonal->m_dThick1;
	double	dLoT				= pInVBr->m_pChordLo->m_dThick1;
	
//	strText.Format("< 수직브레이싱 거더 : %d     STA : %.0f >", nG, dStation);
	strText.Format("< 수직브레이싱 거더 : G%d - G%d B%d >", nG+1, nG+2, nBrNum + 1);
	pOut->SetXL(pXL, strText,	nRow, "B", "B", 2);
	strText.Format("%.3f", pInVBr->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "Y","AA", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_dGirderSpace);
	// 상현재
	pOut->SetXL(pXL, strText,	nRow, "Y","AA", 7, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_pChordUp->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_pChordUp->m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_pChordUp->m_dThick1);
	pOut->SetXL(pXL, strText,	nRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);

	if(pInVBr->m_pChordUp->m_nType==CSteelMaterialData::CTSTEEL)
	{
		pOut->SetXL(pXL, "CT",	nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",			 nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pInVBr->m_pChordUp->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	else if (pInVBr->m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
	{
		pOut->SetXL(pXL, "ㄷ",	nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",			 nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pInVBr->m_pChordUp->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	strText.Format("%.6f", pInVBr->m_pChordUp->m_dArea);
	pOut->SetXL(pXL, strText,	nRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pChordUp->m_dGammaX);
	pOut->SetXL(pXL, strText,	nRow, "X", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pChordUp->m_dGammaY);
	pOut->SetXL(pXL, strText,	nRow, "AA","AC", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pChordUp->m_dGammaMin);
	pOut->SetXL(pXL, strText,	nRow,"AD","AF", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	// 사재
	strText.Format("%.3f", pInVBr->m_pDiagonal->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_pDiagonal->m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_pDiagonal->m_dThick1);
	pOut->SetXL(pXL, strText,	nRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	if(pInVBr->m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
	{
		pOut->SetXL(pXL, "CT",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pInVBr->m_pDiagonal->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	else if(pInVBr->m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
	{
		pOut->SetXL(pXL, "ㄷ",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pInVBr->m_pDiagonal->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	strText.Format("%.6f", pInVBr->m_pDiagonal->m_dArea);
	pOut->SetXL(pXL, strText,	nRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pDiagonal->m_dGammaX);
	pOut->SetXL(pXL, strText,	nRow, "X", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pDiagonal->m_dGammaY);
	pOut->SetXL(pXL, strText,	nRow, "AA","AC", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pDiagonal->m_dGammaMin);
	pOut->SetXL(pXL, strText,	nRow,"AD","AF", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	// 하현재
	strText.Format("%.3f", pInVBr->m_pChordLo->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_pChordLo->m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_pChordLo->m_dThick1);
	pOut->SetXL(pXL, strText,	nRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	if(pInVBr->m_pChordLo->m_nType==CSteelMaterialData::CTSTEEL)
	{
		pOut->SetXL(pXL, "CT",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pInVBr->m_pChordLo->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	if(pInVBr->m_pChordLo->m_nType==CSteelMaterialData::CSTEEL)
	{
		pOut->SetXL(pXL, "ㄷ",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pInVBr->m_pChordLo->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	strText.Format("%.6f", pInVBr->m_pChordLo->m_dArea);
	pOut->SetXL(pXL, strText,	nRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pChordLo->m_dGammaX);
	pOut->SetXL(pXL, strText,	nRow, "X", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pChordLo->m_dGammaY);
	pOut->SetXL(pXL, strText,	nRow, "AA","AB", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pInVBr->m_pChordLo->m_dGammaMin);
	pOut->SetXL(pXL, strText,	nRow,"AD","AF", 6, RED, 9, FALSE, TA_CENTER, "", 1);
	// (1) 연직하중 - 1) 고정하중
	strText.Format("%.3f", toM(dThickPave));
	pOut->SetXL(pXL, strText,	nRow,"AE","AG", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(dThickSlab));
	pOut->SetXL(pXL, strText,	nRow,"AE","AG", 2, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(pInVBr->m_strtVerLoad.m_dDistVerBracingToEndSlab));
	pOut->SetXL(pXL, strText,	nRow,"AE","AG",12, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", tokNPM3(dUnitPave));
	pOut->SetXL(pXL, strText,	nRow, "K", "M", 1, RED, 9, FALSE, TA_CENTER, "", 2);
	strText.Format("%.3f", tokNPM3(dUnitConc));
	pOut->SetXL(pXL, strText,	nRow, "K", "M", 8, RED, 9, FALSE, TA_CENTER, "", 2);
	// (1) 연직하중 - 2) 활하중
	strText.Format("%.3f", tokN(dPr));
	pOut->SetXL(pXL, strText,	nRow, "K", "L",25, RED, 9, FALSE, TA_CENTER, "", 0);
	// (2) 수평하중 - 1) 풍하중
	strText.Format("%.3f", toM(pInVBr->m_strtHorLoad.m_dWidthBridge));
	pOut->SetXL(pXL, strText,	nRow, "I", "K", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(dHeightGirder));
	pOut->SetXL(pXL, strText,	nRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(dThickSlab));
	pOut->SetXL(pXL, strText,	nRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(dHeightGuardFence+dHeightSoundProof+dHeightHandRail));
	pOut->SetXL(pXL, strText,	nRow, "Q", "S", 5, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(pInVBr->m_strtHorLoad.m_dLengthSpan));
	pOut->SetXL(pXL, strText,	nRow, "K", "N", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%d", pDB->GetGirdersu());
	pOut->SetXL(pXL, strText,	nRow, "L", "L", 6, RED, 9, FALSE, TA_CENTER, "", 0);
	// (2) 수평하중 - 2) 지진하중
	strText.Format("%.6f", tokN(pInVBr->m_strtHorLoad.m_dReaction));
	pOut->SetXL(pXL, strText,	nRow, "P", "S", 2, RED, 9, FALSE, TA_CENTER, "", 3);
	if(nGradeEarth==0)	strText.Format("Ⅰ");
	else				strText.Format("Ⅱ");
	pOut->SetXL(pXL, strText,	nRow, "H", "H", 1, RED, 9, FALSE, TA_CENTER);
	if(nRegionEarth==0)	strText.Format("Ⅰ");
	else				strText.Format("Ⅱ");
	pOut->SetXL(pXL, strText,	nRow, "H", "H",39, RED, 9, FALSE, TA_CENTER);

	if(pOutExcel->m_bAbort)	return;
	// (다) 부재의 설계
	
	CalcGen.GetMaterial(pBx, VB_UB, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress AllowShearStress(pSteel);
	CHGAllowStress AllowTension(pSteel);
	CHGAllowStress AllowBuckUncon(pSteel);
	
	double dShear, dTension;
	
	AllowBuckUncon.SetTypeAllow(ALLOW_BUCKLING_UNCONSIDER);
	HGALLOWSTRESS	vtAllowStressUp = AllowBuckUncon.m_AllowStress[AllowBuckUncon.GetIdxByThick(dUpT)];
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dUpT);
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dUpT);
	pOut->SetXL(pXL, strLeft,	nRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "W", "W", 4, RED, 9, FALSE, TA_CENTER);
	if(pInVBr->m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
		pOut->SetXL(pXL, " Zx   =", nRow, "Q", "Q", 0);
	strText.Format("%.3f", pInVBr->m_pChordUp->m_dZy);
	pOut->SetXL(pXL, strText,	nRow, "T", "W",6, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff1,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff2,	nRow,"AE","AF", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff3,	nRow, "K", "L", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff5,	nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff4,	nRow,"AB","AC", 6, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dTension,	nRow, "G", "I", 5, RED, 9, FALSE, TA_CENTER, "", 1);
	if(pInVBr->m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
		pOut->SetXL(pXL, "f = Ml+i / Zx =",	nRow, "E", "E", 0);
	nRow+=2;
	pOut->SetXL(pXL, dShear,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtChordUp.m_dLengthWeld);
	pOut->SetXL(pXL, strText,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtChordUp.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "S", 7, RED, 9, FALSE, TA_CENTER, "", 1);

	CalcGen.GetMaterial(pBx, VB_CR, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowShearStress.SetBaseSteel(pSteel);
	AllowTension.SetBaseSteel(pSteel);
	
	AllowBuckUncon.SetTypeAllow(ALLOW_BUCKLING_UNCONSIDER);
	HGALLOWSTRESS	vtAllowStressSd = AllowBuckUncon.m_AllowStress[AllowBuckUncon.GetIdxByThick(dSdT)];
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dSdT);
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dSdT);
	pOut->SetXL(pXL, strLeft,	nRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "W", "W", 9, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff1,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff2,	nRow,"AE","AF", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff3,	nRow, "K", "L", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff5,	nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff4,	nRow,"AB","AC", 6, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dTension,	nRow, "H", "J", 7, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dShear,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtDiagonal.m_dLengthWeld);
	pOut->SetXL(pXL, strText,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtDiagonal.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "T", 7, RED, 9, FALSE, TA_CENTER, "", 1);

	CalcGen.GetMaterial(pBx, VB_LB, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowShearStress.SetBaseSteel(pSteel);
	AllowTension.SetBaseSteel(pSteel);

	AllowBuckUncon.SetTypeAllow(ALLOW_BUCKLING_UNCONSIDER);
	HGALLOWSTRESS	vtAllowStressLo = AllowBuckUncon.m_AllowStress[AllowBuckUncon.GetIdxByThick(dLoT)];
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dLoT);
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dLoT);
	pOut->SetXL(pXL, strLeft,	nRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "W", "W", 9, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff1,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff2,	nRow,"AE","AF", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff3,	nRow, "K", "L", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff5,	nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff4,	nRow,"AB","AC", 6, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dTension,	nRow, "H", "J", 7, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dShear,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtChordLo.m_dLengthWeld);
	pOut->SetXL(pXL, strText,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtChordLo.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "T",19, RED, 9, FALSE, TA_CENTER, "", 1);

	// 상현재 연결 (A,C)
	CalcGen.GetMaterial(pBx, VB_GU, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowShearStress.SetBaseSteel(pSteel);
	AllowTension.SetBaseSteel(pSteel);
	
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, pInVBr->m_strtGussetUp.m_dThick);
	strText.Format("%.3f", pInVBr->m_strtGussetUp.m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_strtGussetUp.m_dThick);
	pOut->SetXL(pXL, strText,	nRow, "P", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_strtGussetUp.m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, strLeft,	nRow, "W", "Y", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "Z", "Z", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTension,	nRow, "P", "R", 4, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%s", pInVBr->m_strtGussetUp.m_strBoltMaterial);
	pOut->SetXL(pXL, strText,	nRow, "H", "H", 0, RED);
	strText.Format("%.3f", pInVBr->m_strtGussetUp.m_dDiaBolt);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", tokN(pInVBr->m_strtGussetUp.m_dBoltAllowStress));
	pOut->SetXL(pXL, strText,	nRow, "V", "X", 2, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtGussetUp.m_dQtyBoltUse);
	pOut->SetXL(pXL, strText,	nRow, "Y", "Z", 5, RED, 9, FALSE, TA_CENTER, "", 0);
	// 사재/상현재 연결 (B)
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, pInVBr->m_strtGussetDiagonal.m_dThick);
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, pInVBr->m_strtGussetDiagonal.m_dThick);
	strText.Format("%.3f", pInVBr->m_strtGussetDiagonal.m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_strtGussetDiagonal.m_dThick);
	pOut->SetXL(pXL, strText,	nRow, "P", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_strtGussetDiagonal.m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, strLeft,	nRow, "W", "Y", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "Z", "Z", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTension,	nRow, "P", "R", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dShear,	nRow, "P", "R", 6, RED, 9, FALSE, TA_CENTER, "", 1);
	double dWeldT		= atof(pXL->GetXL(nRow-4, 15));
	double dWeldShear	= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dWeldT);
	pOut->SetXL(pXL, dWeldShear,	nRow, "R", "T", 2, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtGussetDiagonal.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "S",11, RED, 9, FALSE, TA_CENTER, "", 1);
	// 하현재/사재 연결 (D,E)
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, pInVBr->m_strtGussetLo.m_dThick);
	strText.Format("%.3f", pInVBr->m_strtGussetLo.m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_strtGussetLo.m_dThick);
	pOut->SetXL(pXL, strText,	nRow, "P", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pInVBr->m_strtGussetLo.m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, strLeft,	nRow, "W", "Y", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "Z", "Z", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTension,	nRow, "P", "R", 6, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%s", pInVBr->m_strtGussetLo.m_strBoltMaterial);
	pOut->SetXL(pXL, strText,	nRow, "H", "H", 0, RED);
	strText.Format("%.3f", pInVBr->m_strtGussetLo.m_dDiaBolt);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", tokN(pInVBr->m_strtGussetLo.m_dBoltAllowStress));
	pOut->SetXL(pXL, strText,	nRow, "V", "X", 2, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pInVBr->m_strtGussetLo.m_dQtyBoltUse);
	pOut->SetXL(pXL, strText,	nRow, "Y", "Z", 7, RED, 9, FALSE, TA_CENTER, "", 0);
}

void CXLDesignCrossBeam::DesignVBracing(CPlateBasicIndex *pBx, long &nRow)
{
	CPlateBridgeApp			*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd			*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData				*pCalcData	= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CGlobarOption			*pGlobarOpt	= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CAPlateOutExcel			*pOutExcel	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CPlateGirderApp			*pGir		= pBx->GetGirder();
	CCentSeparation			*pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	CXLControl				*pXL		= m_pXL;
	CAPlateOutCalcXL		*pOut		= m_pAPlateOutCalcXL;
	CCalcGeneral			CalcGen(m_pAPlateOutCalcStd->GetDataManage());
	CDesignCrossBeam		CrossBeam(pCalcStd, pBx);
	CDesignVBracing			*pVBr		= (CDesignVBracing*)CrossBeam.GetDesignCrossBeam();
	CSteel					*pSteelCtl	= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();
	CString	strText				= _T("");
	CString	strLeft				= _T("");
	CString	strRight			= _T("");
	long	nBrNum				= pBx->GetNumberCrossBeam();
	long	nG					= pBx->GetNumberGirder();
	double	dHeightGirder		= pBx->GetHeightGirderByBx();
	double	dSlabThick			= pDB->m_dThickSlabBase;
	double	dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double	dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double	dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double	dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;
	long	nGradeEarth			= pGlobarOpt->GetIdxDegreeEarthQuake();
	long	nRegionEarth		= pGlobarOpt->GetIdxRegionEarthQuake();
	double	dUpT				= pVBr->m_pChordUp->m_dThick1;
	double	dSdT				= pVBr->m_pDiagonal->m_dThick1;
	double	dLoT				= pVBr->m_pChordLo->m_dThick1;

//	strText.Format("< 수직브레이싱 거더 : %d     STA : %.0f >", nG, dStation);
	strText.Format("< 수직브레이싱 거더 : G%d - G%d B%d >", nG+1, nG+2, nBrNum);
	pOut->SetXL(pXL, strText,	nRow, "B", "B", 2);
	strText.Format("%.3f", pVBr->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "Y","AA", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_dGirderSpace);
	// 상현재
	pOut->SetXL(pXL, strText,	nRow, "Y","AA", 7, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_pChordUp->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_pChordUp->m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_pChordUp->m_dThick1);
	pOut->SetXL(pXL, strText,	nRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	if(pVBr->m_pChordUp->m_nType==CSteelMaterialData::CTSTEEL)
	{
		pOut->SetXL(pXL, "CT",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pVBr->m_pChordUp->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	else if(pVBr->m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
	{		
		pOut->SetXL(pXL, "ㄷ",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pVBr->m_pChordUp->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	strText.Format("%.6f", pVBr->m_pChordUp->m_dArea);
	pOut->SetXL(pXL, strText,	nRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pChordUp->m_dGammaX);
	pOut->SetXL(pXL, strText,	nRow, "X", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pChordUp->m_dGammaY);
	pOut->SetXL(pXL, strText,	nRow, "AA","AC", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pChordUp->m_dGammaMin);
	pOut->SetXL(pXL, strText,	nRow,"AD","AF", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	// 사재
	strText.Format("%.3f", pVBr->m_pDiagonal->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_pDiagonal->m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_pDiagonal->m_dThick1);
	pOut->SetXL(pXL, strText,	nRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	if(pVBr->m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
	{
		pOut->SetXL(pXL, "CT",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pVBr->m_pDiagonal->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	else if(pVBr->m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
	{
		pOut->SetXL(pXL, "ㄷ",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pVBr->m_pDiagonal->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	strText.Format("%.6f", pVBr->m_pDiagonal->m_dArea);
	pOut->SetXL(pXL, strText,	nRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pDiagonal->m_dGammaX);
	pOut->SetXL(pXL, strText,	nRow, "X", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pDiagonal->m_dGammaY);
	pOut->SetXL(pXL, strText,	nRow, "AA","AC", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pDiagonal->m_dGammaMin);
	pOut->SetXL(pXL, strText,	nRow,"AD","AF", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	// 하현재
	strText.Format("%.3f", pVBr->m_pChordLo->m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_pChordLo->m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_pChordLo->m_dThick1);
	pOut->SetXL(pXL, strText,	nRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	if(pVBr->m_pChordLo->m_nType==CSteelMaterialData::CTSTEEL)
	{
		pOut->SetXL(pXL, "CT",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pVBr->m_pChordLo->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	else if(pVBr->m_pChordLo->m_nType==CSteelMaterialData::CSTEEL)
	{
		pOut->SetXL(pXL, "ㄷ",		nRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",		nRow, "Q", "Q", 0, RED, 9, FALSE, TA_CENTER);
		strText.Format("%.3f", pVBr->m_pChordLo->m_dThick2);
		pOut->SetXL(pXL, strText,	nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	strText.Format("%.6f", pVBr->m_pChordLo->m_dArea);
	pOut->SetXL(pXL, strText,	nRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pChordLo->m_dGammaX);
	pOut->SetXL(pXL, strText,	nRow, "X", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pChordLo->m_dGammaY);
	pOut->SetXL(pXL, strText,	nRow, "AA","AC", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.6f", pVBr->m_pChordLo->m_dGammaMin);
	pOut->SetXL(pXL, strText,	nRow,"AD","AF",11, RED, 9, FALSE, TA_CENTER, "", 1);
	// 가) 하중산정 - 1) 풍하중
	strText.Format("%.3f", toM(pVBr->m_strtWind.m_dWidthBridge));
	pOut->SetXL(pXL, strText,	nRow, "I", "K", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(dHeightGirder));
	pOut->SetXL(pXL, strText,	nRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(dSlabThick));
	pOut->SetXL(pXL, strText,	nRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(dHeightGuardFence+dHeightSoundProof+dHeightHandRail));
	pOut->SetXL(pXL, strText,	nRow, "Q", "S", 5, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.3f", toM(pVBr->m_strtWind.m_dBracingSpace));
	pOut->SetXL(pXL, strText,	nRow, "L", "N", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%d", pDB->GetGirdersu());
	pOut->SetXL(pXL, strText,	nRow, "L", "L", 6, RED, 9, FALSE, TA_CENTER, "", 0);
	// 가) 하중산정 - 1) 지진하중
	strText.Format("%.6f", tokN(pVBr->m_strtEarthQuake.m_dReaction));
	pOut->SetXL(pXL, strText,	nRow, "Q", "T",	0, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.6f", toM(pVBr->m_strtEarthQuake.m_dLengthBridge));
	pOut->SetXL(pXL, strText,	nRow, "W", "Y", 2, RED, 9, FALSE, TA_CENTER, "", 3);
	if(nGradeEarth==0)	strText.Format("Ⅰ");
	else				strText.Format("Ⅱ");
	pOut->SetXL(pXL, strText,	nRow, "H", "H", 1, RED, 9, FALSE, TA_CENTER);
	if(nRegionEarth==0)	strText.Format("Ⅰ");
	else				strText.Format("Ⅱ");
	pOut->SetXL(pXL, strText,	nRow, "H", "H",28, RED, 9, FALSE, TA_CENTER);

	if(pOutExcel->m_bAbort)	return;
	// 다) 부재의 설계
	
	CalcGen.GetMaterial(pBx, VB_UB, strLeft, strRight);

	CHGBaseSteel	*pSteel	 = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowShearStress(pSteel);
	CHGAllowStress	AllowTension(pSteel);
	CHGAllowStress	AllowBuckUncon(pSteel);
	double dShear, dTension;
	AllowBuckUncon.SetTypeAllow(ALLOW_BUCKLING_UNCONSIDER);
	HGALLOWSTRESS	vtAllowStressUp = AllowBuckUncon.m_AllowStress[AllowBuckUncon.GetIdxByThick(dUpT)];
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dUpT);
	pOut->SetXL(pXL, strLeft,	nRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "W", "W", 8, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff1,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff2,	nRow,"AE","AF", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff3,	nRow, "K", "L", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff5,	nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressUp.m_dCoeff4,	nRow,"AB","AC", 9, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dShear,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtChordUp.m_dLengthWeld);
	pOut->SetXL(pXL, strText,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtChordUp.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "S", 7, RED, 9, FALSE, TA_CENTER, "", 1);

	CalcGen.GetMaterial(pBx, VB_CR, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowShearStress.SetBaseSteel(pSteel);
	AllowTension.SetBaseSteel(pSteel);
	AllowBuckUncon.SetBaseSteel(pSteel);
	AllowBuckUncon.SetTypeAllow(ALLOW_BUCKLING_UNCONSIDER);
	HGALLOWSTRESS	vtAllowStressSd = AllowBuckUncon.m_AllowStress[AllowBuckUncon.GetIdxByThick(dSdT)];
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dSdT);
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dSdT);
	pOut->SetXL(pXL, strLeft,	nRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "W", "W", 9, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff1,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff2,	nRow,"AE","AF", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff3,	nRow, "K", "L", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff5,	nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressSd.m_dCoeff4,	nRow,"AB","AC", 6, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dTension,	nRow, "H", "J", 7, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dShear,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtDiagonal.m_dLengthWeld);
	pOut->SetXL(pXL, strText,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtDiagonal.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "S", 7, RED, 9, FALSE, TA_CENTER, "", 1);

	CalcGen.GetMaterial(pBx, VB_LB, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowShearStress.SetBaseSteel(pSteel);
	AllowTension.SetBaseSteel(pSteel);
	AllowBuckUncon.SetBaseSteel(pSteel);
	AllowBuckUncon.SetTypeAllow(ALLOW_BUCKLING_UNCONSIDER);
	HGALLOWSTRESS	vtAllowStressLo = AllowBuckUncon.m_AllowStress[AllowBuckUncon.GetIdxByThick(dLoT)];
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, dLoT);
	pOut->SetXL(pXL, strLeft,	nRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "W", "W", 8, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff1,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff2,	nRow,"AE","AF", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff3,	nRow, "K", "L", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff5,	nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, vtAllowStressLo.m_dCoeff4,	nRow,"AB","AC", 8, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dShear,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtChordLo.m_dLengthWeld);
	pOut->SetXL(pXL, strText,	nRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtChordLo.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "S",19, RED, 9, FALSE, TA_CENTER, "", 1);

	// 1) 상현재/사재 연결 (A,B)
	CalcGen.GetMaterial(pBx, VB_GU, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowShearStress.SetBaseSteel(pSteel);
	AllowTension.SetBaseSteel(pSteel);
	AllowBuckUncon.SetBaseSteel(pSteel);

	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, pVBr->m_strtGussetUp.m_dThick);
	strText.Format("%.3f", pVBr->m_strtGussetUp.m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_strtGussetUp.m_dThick);
	pOut->SetXL(pXL, strText,	nRow, "P", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_strtGussetUp.m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, strLeft,	nRow, "W", "Y", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "Z", "Z", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTension,	nRow, "P", "R", 6, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%s", pVBr->m_strtGussetUp.m_strBoltMaterial);
	pOut->SetXL(pXL, strText,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER);
	strText.Format("%.3f", pVBr->m_strtGussetUp.m_dDiaBolt);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", tokN(pVBr->m_strtGussetUp.m_dBoltAllowStress));
	pOut->SetXL(pXL, strText,	nRow, "V", "X", 2, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtGussetUp.m_dQtyBoltUse);
	pOut->SetXL(pXL, strText,	nRow, "Y", "Z", 5, RED, 9, FALSE, TA_CENTER, "", 0);
	// 2) 하현재 연결 (C,E)
	dTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, pVBr->m_strtGussetLo.m_dThick);
	strText.Format("%.3f", pVBr->m_strtGussetLo.m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_strtGussetLo.m_dThick);
	pOut->SetXL(pXL, strText,	nRow, "P", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_strtGussetLo.m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, strLeft,	nRow, "W", "Y", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "Z", "Z", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTension,	nRow, "P", "R", 4, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%s", pVBr->m_strtGussetLo.m_strBoltMaterial);
	pOut->SetXL(pXL, strText,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER);
	strText.Format("%.3f", pVBr->m_strtGussetLo.m_dDiaBolt);
	pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", tokN(pVBr->m_strtGussetLo.m_dBoltAllowStress));
	pOut->SetXL(pXL, strText,	nRow, "V", "X", 2, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtGussetLo.m_dQtyBoltUse);
	pOut->SetXL(pXL, strText,	nRow, "Y", "Z", 5, RED, 9, FALSE, TA_CENTER, "", 0);
	// 3) 사재/하현재 연결 (D)
	// 강재의 허용전단응력, 용접부의 허용응력 둘다 두께를 동일한곳에서 가져옴 (역v형은 서로 다른데서 가져옴)
	dShear		= AllowShearStress.GetAllowStress(ALLOW_SHEARSTRESS, pVBr->m_strtGussetDiagonal.m_dThick);
	strText.Format("%.3f", pVBr->m_strtGussetDiagonal.m_dHeight);
	pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_strtGussetDiagonal.m_dThick);
	pOut->SetXL(pXL, strText,	nRow, "P", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%.3f", pVBr->m_strtGussetDiagonal.m_dWidth);
	pOut->SetXL(pXL, strText,	nRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, strLeft,	nRow, "W", "Y", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strRight,	nRow, "Z", "Z", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dShear,	nRow, "P", "R", 4, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dShear,	nRow, "R", "T", 2, RED, 9, FALSE, TA_CENTER, "", 1);
	strText.Format("%.3f", pVBr->m_strtGussetDiagonal.m_dThroatDepth);
	pOut->SetXL(pXL, strText,	nRow, "R", "S", 8, RED, 9, FALSE, TA_CENTER, "", 0);
}

void CXLDesignCrossBeam::DesignHorBracing(CStringArray &strArrUseSheet, long &nIdx)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();		
	CAPlateCalcStd		*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CAPlateOutExcel		*pOutExcel	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	CPlateBasicIndex	*pBx		= pGir->GetBracingInTotal(0)->GetBx();
	CXLControl			*pXL		= m_pXL;
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());	
	CDesignHorBracing	HorBracing(pCalcStd, 0, 0);	
	CSteel				*pSteelCtl	= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();

	CString	strText		= _T(""), strLeft = _T(""), strRight = _T("");
	long	nQtyBracing	= pGir->GetQtyBracingReal();
	long	nQtyGir		= pDB->GetGirdersu();
	long	nRow		= 1;
	BOOL	bFirst		= TRUE;
	if(nQtyBracing<1)	return;

	CalcGeneral.GetMaterial(pBx, HB_B, strLeft, strRight);
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress AllowBuckUncon(pSteel);

	pXL->SetActiveSheet("7.3수평브레이싱의설계");
	strText.Format("7.%d 수평브레이싱의설계", nIdx);
	pXL->SetSheetName(strText);
	strArrUseSheet.Add(strText);

	strText.Format("7.%d 수평브레이싱", nIdx);	nIdx++;
	pOut->SetXL(pXL, strText, nRow, "B", "B", 6);
	strText.Format("%.6f", tokNPM(HorBracing.m_dWindLoad));
	pOut->SetXL(pXL, strText,	nRow, "L", "N", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%.6f", tokNPM(HorBracing.m_dEarthQuakeLoad));
	pOut->SetXL(pXL, strText,	nRow, "L", "N",12, RED, 9, FALSE, TA_CENTER, "", 3);

	CPtrArray ptrMaxBracing;
	
	CSteelMaterial	*pMat	= m_pAPlateOutCalcStd->GetDataManage()->GetSteelMaterial();

	for(long nSteelSu = 0; nSteelSu < pMat->m_ptrArrStdSteel.GetSize(); nSteelSu++)
	{
		ptrMaxBracing.Add(new CPtrArray);
	}

	for(long nG = 0; nG < nQtyGir-1; nG++)
	{
		pGir = pDB->GetGirder(nG);

		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBxHBr	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		
		for(long nBr = 0; nBr < pGir->GetQtyBracingAll()-1; nBr++)
		{
			if(pOutExcel->m_bAbort)	return;
			// 마지막을 검사
			
			CPlateBasicIndex	*pBx		= pGir->GetBracingInTotal(nBr)->GetBx();

			if(pBx->GetBracing()->GetType()==CBracing::BRACTYPE_NONE)	continue;

			CDesignHorBracing *pHBr =  new CDesignHorBracing(pCalcStd, nG, nBr);
	
			if(!bFirst)	pXL->InsertCopyRowLine(nRow, nRow, nRow+1);

			nRow++;
			
			long nMatSteelIdx = pBxHBr->GetBracing()->m_nStdSteelIdx;
			
			if(nMatSteelIdx >= 0 )
				((CPtrArray*)ptrMaxBracing[nMatSteelIdx])->Add(pHBr);
			
			strText.Format("G%d D%d", nG+1, nBr+1);
			pOut->SetXL(pXL, strText,	nRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER);

			strText.Format("%g", toM(pHBr->m_dLengthSection));
			pOut->SetXL(pXL, strText,	nRow, "G", "I", 0, RED, 9, FALSE, TA_CENTER, "", 3);

			strText.Format("%g", toM2(pHBr->m_dAreaPositiveInf));
			pOut->SetXL(pXL, strText,	nRow, "J", "L", 0, RED, 9, FALSE, TA_CENTER, "", 3);

			strText.Format("%g", toM2(pHBr->m_dAreaNegativeInf));
			pOut->SetXL(pXL, strText,	nRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);

			strText.Format("%g", ToDegree(pHBr->m_dTheta));
			pOut->SetXL(pXL, strText,	nRow, "S", "U", 0, RED, 9, FALSE, TA_CENTER, "", 3);


			CString sCellRef = _T("");

			sCellRef.Format("G%dD%d_PLUSNW", nG+1, nBr+1);
			strText.Format("=J%d*$AA$7/COS(RADIANS(S%d))", nRow, nRow, nRow);
			pOut->SetXL(pXL, strText,	nRow, "Y", "AB", 0, BLACK, 9, FALSE, TA_CENTER, sCellRef, 3);

			sCellRef.Format("G%dD%d_MINUSNW", nG+1, nBr+1);
			strText.Format("=M%d*$AA$7/COS(RADIANS(S%d))", nRow, nRow);
			pOut->SetXL(pXL, strText,	nRow, "AC", "AF", 0, BLACK, 9, FALSE, TA_CENTER, sCellRef, 3);

			sCellRef.Format("G%dD%d_NE", nG+1, nBr+1);
			strText.Format("=P%d*$AA$8/COS(RADIANS(S%d))", nRow, nRow);
			pOut->SetXL(pXL, strText,	nRow, "AG", "AJ", 0, BLACK, 9, FALSE, TA_CENTER, sCellRef, 3);

			bFirst	= FALSE;
			pBxHBr = Finder.GetBxNext();
			nIdx++;
		}
	}

	nRow+=2;
	BOOL bMaxMultiBracing = FALSE;
	long nBr1 = 0;
	for(nBr1 = 0; nBr1 < ptrMaxBracing.GetSize(); nBr1++)
	{
		double				dMax			= 0.0;
		CDesignHorBracing	*pHorBracingMax	= NULL;
		CPtrArray *pArrMaxBracing = ((CPtrArray*)ptrMaxBracing[nBr1]);
		for(long nBr2 = 0; nBr2 < pArrMaxBracing->GetSize(); nBr2++)
		{
			CDesignHorBracing *pHorBracingTB = (CDesignHorBracing*)pArrMaxBracing->GetAt(nBr2);
			
			double dValue = fabs(pHorBracingTB->m_dAreaPositiveInf + pHorBracingTB->m_dAreaNegativeInf);

			if(dValue > dMax)
			{
				dMax = dValue;
				pHorBracingMax = pHorBracingTB;
			}
		}

		if(pHorBracingMax && bMaxMultiBracing)
		{
			//Copy 명령 
			pXL->InsertCopyRowLine(nRow - 32, nRow - 2, nRow);
		}
		
		if(pHorBracingMax)
		{
			if(!bMaxMultiBracing)
				nRow++;

			BOOL		bCTSteel		= pHorBracingMax->m_pBracingJewon->m_nType==CSteelMaterialData::CTSTEEL ? TRUE : FALSE;

			AllowBuckUncon.SetTypeAllow(ALLOW_BUCKLING_UNCONSIDER);
			HGALLOWSTRESS	vtAllowStressD1 = AllowBuckUncon.m_AllowStress[AllowBuckUncon.GetIdxByThick(pHorBracingMax->m_pBracingJewon->m_dThick1)];

			strText.Format("G%d-D%d 부재", pHorBracingMax->m_nG + 1, pHorBracingMax->m_nIndex + 1);
			pOut->SetXL(pXL, strText, nRow, "D", "I", 0, BLACK);

			strText.Format("%g", toM(pHorBracingMax->m_dLengthElem));
			pOut->SetXL(pXL, strText,	nRow, "N", "P", 1, RED, 9, FALSE, TA_CENTER, "", 3);

			if(bCTSteel)	strText.Format("CT");
			else			strText.Format("L");

			pOut->SetXL(pXL, strText,	nRow, "G", "G", 0, RED, 9);
			
			strText.Format("%g", pHorBracingMax->m_pBracingJewon->m_dHeight);
			pOut->SetXL(pXL, strText,	nRow, "J", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);

			strText.Format("%g", pHorBracingMax->m_pBracingJewon->m_dWidth);
			pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);

			strText.Format("%g", pHorBracingMax->m_pBracingJewon->m_dThick1);
			pOut->SetXL(pXL, strText,	nRow, "P", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 0);

			if(bCTSteel)
			{
				strText.Format("%g", pHorBracingMax->m_pBracingJewon->m_dThick2);
				pOut->SetXL(pXL, strText,	nRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			}
			else
			{	
				strText.Format(" ");
				pOut->SetXL(pXL, strText,	nRow, "R", "R", 0, WHITE, 9, FALSE, TA_LEFT, "", 0);
				pOut->SetXL(pXL, strLeft,	nRow, "S", "T", 0, WHITE, 9, FALSE, TA_RIGHT, "", 0);				
			}

			pOut->SetXL(pXL, strLeft,	nRow, "W", "Y", 0, RED, 9, FALSE, TA_RIGHT, "", 0);
			pOut->SetXL(pXL, strRight,	nRow, "Z", "Z", 0, RED, 9, FALSE, TA_LEFT, "", 0);

			nRow++;

			strText.Format("%g ", pHorBracingMax->m_pBracingJewon->m_dArea);
			pOut->SetXL(pXL, strText,	nRow, "T", "V", 1, RED, 9, FALSE, TA_CENTER, "", 1);

			strText.Format("%g", min(pHorBracingMax->m_pBracingJewon->m_dGammaX,pHorBracingMax->m_pBracingJewon->m_dGammaY));
			pOut->SetXL(pXL, strText,	nRow, "T", "V", 1, RED, 9, FALSE, TA_CENTER, "", 1);

			strText.Format("%g", pHorBracingMax->m_pBracingJewon->m_dGammaX);
			pOut->SetXL(pXL, strText,	nRow, "T", "V", 6, RED, 9, FALSE, TA_CENTER, "", 1);
			
			pOut->SetXL(pXL, vtAllowStressD1.m_dCoeff1,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			pOut->SetXL(pXL, vtAllowStressD1.m_dCoeff2,	nRow,"AE","AF", 1, RED, 9, FALSE, TA_CENTER, "", 0);
			pOut->SetXL(pXL, vtAllowStressD1.m_dCoeff3,	nRow, "K", "L", 1, RED, 9, FALSE, TA_CENTER, "", 1);
			pOut->SetXL(pXL, vtAllowStressD1.m_dCoeff5,	nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			pOut->SetXL(pXL, vtAllowStressD1.m_dCoeff4,	nRow,"AB","AC", 6, RED, 9, FALSE, TA_CENTER, "", 0);
			
			strText.Format("%g", pHorBracingMax->m_dFta);
			pOut->SetXL(pXL, strText,	nRow, "H", "J", 4, RED, 9, FALSE, TA_CENTER, "", 1);

			//ft = N / An

			CString sCellRef_PLUSNW  = _T("");
			CString sCellRef_MINUSNW = _T("");
			CString sCellRef_NE		 = _T("");
			
			strText.Format("G%dD%d_PLUSNW", pHorBracingMax->m_nG + 1, pHorBracingMax->m_nIndex + 1);
			sCellRef_PLUSNW  = pOut->GetCellRef(strText);

			strText.Format("G%dD%d_MINUSNW", pHorBracingMax->m_nG + 1, pHorBracingMax->m_nIndex + 1);
			sCellRef_MINUSNW = pOut->GetCellRef(strText);

			strText.Format("G%dD%d_NE", pHorBracingMax->m_nG + 1, pHorBracingMax->m_nIndex + 1);
			sCellRef_NE		 = pOut->GetCellRef(strText);

			strText.Format("=MAX(ABS(%s),ABS(%s),ABS(%s))", sCellRef_PLUSNW, sCellRef_MINUSNW, sCellRef_NE);
			pOut->SetXL(pXL, strText,	nRow, "K", "N", 5, BLACK, 9, FALSE, TA_CENTER);

			strText.Format("%s", pHorBracingMax->m_strBoltMaterial);
			pOut->SetXL(pXL, strText,	nRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER);

			strText.Format("%g", pHorBracingMax->m_dDiaBolt);
			pOut->SetXL(pXL, strText,	nRow, "L", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);

			strText.Format("%g", tokN(pHorBracingMax->m_dBoltAllowStress));
			pOut->SetXL(pXL, strText,	nRow, "V", "X", 3, RED, 9, FALSE, TA_CENTER, "", 1);

			strText.Format("%g", pHorBracingMax->m_dQtyBoltUse);
			pOut->SetXL(pXL, strText,	nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);

			strText.Format("%d", pHorBracingMax->m_nQtyLine);
			pOut->SetXL(pXL, strText,	nRow, "T", "T", 2, RED, 9, FALSE, TA_CENTER, "", 0);

			bMaxMultiBracing = TRUE;
		}
	}

	//Memory Release	
	for(nBr1 = 0; nBr1 < ptrMaxBracing.GetSize(); nBr1++)
	{
		CPtrArray *pArrMaxBracing = ((CPtrArray*)ptrMaxBracing[nBr1]);
		for(long nBr2 = 0; nBr2 < pArrMaxBracing->GetSize(); nBr2++)
		{
			delete (CDesignHorBracing*)pArrMaxBracing->GetAt(nBr2);
		}
		
		delete (CPtrArray*)ptrMaxBracing[nBr1];
	}

}

void CXLDesignCrossBeam::DesignConnection(long nIdx)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemStressCalc      StressCalc(pDataManage, pCalcStd);
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
		
	CString	sText		= _T("");	
	long	nG			= 0;
	long	nNumCr		= 0;

	m_nSttRow = 1;
	pOut->ClearCellRef();
	pXL->SetCellWidth(0, 255, 1);

	sText.Format("7.%d 가로보의 연결", nIdx);
	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1, BLACK, 9);

	StressCalc.SetSectionCrossBeamPos();

	for(nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CSection		*pSec	= pBx->GetSection();
			if(!pSec)
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			CPlateCrossBeam *pCr	= pSec->GetCrossBeam();
			if(!pCr || !pCr->IsSectionCheck())
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			nG			= pBx->GetNumberGirder();
			nNumCr		= pBx->GetNumberCrossBeam();
			
			if(pSec->IsenType(SECTION_A))	sText.Format("< 단부 가로보 : G%d - G%d C%d >", nG+1, nG+2, nNumCr+1);
			else							sText.Format("< 하중분배 가로보 : G%d - G%d C%d >", nG+1, nG+2, nNumCr+1);
			pOut->SetXL(pXL, sText, m_nSttRow, "B", "B", 1, BLUE, 9, TRUE);
			
			if(pCr->m_nLGirSpType!=0 || pCr->m_nRGirSpType!=0)
			{
				if(pCr->m_bConnectionPlate)//컨낵션 플레이트 방식이면
				{
					if(pOutExcel->m_bAbort)	return;
					ConnectGussetType(pBx, 1, pCr->m_nRadioConnectionPlateInOut);//상부, 내,외측 여부
					if(pOutExcel->m_bAbort)	return;
					ConnectGussetType(pBx, 2, pCr->m_nRadioConnectionPlateInOut);//하부, 내외측적용==0
				}
				else//기존방식이면
				{
					if(pOutExcel->m_bAbort)	return;
					ConnectExistTop(pBx);
					if(pOutExcel->m_bAbort)	return;
					ConnectExistBottom(pBx);
				}
			}

			if(pOutExcel->m_bAbort)	return;
			if(pCr->m_nLGirSpType)	ConnectExistWeb(pBx, 1);	//왼쪽 이음판이 있으면
			else					ConnectGussetWeb(pBx, 1);	//왼쪽 이음판이 없으면

			if(pCr->m_nLGirSpType != pCr->m_nRGirSpType)		// 오른쪽 이음판의 유/무가 왼쪽 이음판과 다르면
			{
				if(pCr->m_nRGirSpType)	ConnectExistWeb(pBx, 2);	//왼쪽이음판이 없고 오른쪽 이음판이 있으면
				else					ConnectGussetWeb(pBx, 2);	//왼쪽이음판이 있고 오른쪽 이음판이 없으면
			}
			
			if(pOutExcel->m_bAbort)	return;
			pBx = Finder.GetBxNext();
		}
	}
}

void CXLDesignCrossBeam::ConnectGussetType(CPlateBasicIndex *pBx, long UNType, long nInOut)//가로보연결 - 거셋방식 상부 플랜지
{
	CAPlateCalcStd			*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CDesignCrossBeamJoint	Joint(pCalcStd, pBx);
	CDesignFlangeJoint		*pUpper		= NULL;
	CAPlateOutCalcXL		*pOut		= m_pAPlateOutCalcXL;
	CXLControl				*pXL		= m_pXL;
	
	CString sTitle = "";

	if(UNType == 1)//상부
	{
		pUpper	= Joint.m_pUpper;
		sTitle = "(1) 상부 플랜지 연결";
	}
	else//하부
	{
		pUpper	= Joint.m_pLower;
		sTitle = "(2) 하부 플랜지 연결";
	}

	CString sText = _T("");
	CString strLeft, strRight;
	double dFa	= pUpper->m_strtGusset.m_dAllowStress;	//허용응력
	double dF	= pUpper->m_strtGusset.m_dStress;		//작용응력

	double dFlangeArea = pUpper->m_strtGusset.m_dAreaFlange;
	CString sBolt1, sBolt2, sBolt4;	
	double dDimCen= pUpper->m_strtGusset.m_dTR;
	sBolt1 = pUpper->m_strtGusset.m_strBoltMaterial;	//"F10T";
	sBolt2 = "M";
	double dDiaBolt  = pUpper->m_strtGusset.m_dDiaBolt;	//"22";
	if (pUpper->m_strtGusset.m_nQtySide == 1)	sBolt4 = "(1면마찰)";
	else										sBolt4 = "(2면마찰)";
	
	double dBoltAllow	= pUpper->m_strtGusset.m_dAllowBolt;
	double dUseBoltEA	= pUpper->m_strtGusset.m_dBoltUse;
	double dUseBoltCol	= pUpper->m_strtGusset.m_nQtyLine;
	double dPanEA1		= pUpper->m_strtGusset.m_nTopConEA;
	double dPanWidth1	= pUpper->m_strtGusset.m_dTopWidth;
	double dPanThick1	= pUpper->m_strtGusset.m_dTopThick;
	double dPanEA2		= pUpper->m_strtGusset.m_nBottomConEA;
	double dPanWidth2	= pUpper->m_strtGusset.m_dBottomWidth;
	double dPanThick2	= pUpper->m_strtGusset.m_dBottomThick;

	strLeft = pUpper->m_strtGusset.m_strMaterial;
	strRight = pUpper->m_strtGusset.m_sRightMaterial;

	pOut->SetXL(pXL, sTitle,					m_nSttRow, "C", "C", 1, BLACK, 9);
	pOut->SetXL(pXL, "허용 응력",				m_nSttRow, "D", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "fa =",					m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, dFa,						m_nSttRow, "K", "N", 0, RED,   9, FALSE, TA_CENTER, "dFa");
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "(",						m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strLeft,					m_nSttRow, "S", "U", 0, RED,   9, FALSE, TA_CENTER, "MatNM");
	pOut->SetXL(pXL, strRight,					m_nSttRow, "V", "V", 0, RED,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ")",						m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetXL(pXL, "작용 응력",				m_nSttRow, "D", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f =", 					m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dF,						m_nSttRow, "K", "N", 0, RED,   9, FALSE, TA_CENTER, "dF");
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);	
	
	//순서상...
	pOut->SetXL(pXL, "0.75*fa =",				m_nSttRow, "T", "W", 0, BLACK, 9);
	sText.Format("=0.75*%s",pOut->GetCellRef("dFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "X", "AA", 0, RED,  9, FALSE, TA_CENTER, "ReFa");
	//이쪽으로 이동...
	sText.Format("=IF(%s>%s,\"＞\",\"＜\")", pOut->GetCellRef("dF"), pOut->GetCellRef("ReFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "R", "R", 0, BLACK, 9);
	
	pOut->SetXL(pXL, "MPa 이므로",				m_nSttRow, "AB", "AF", 1, BLACK, 9);
			
	m_nSttRow++;
	pOut->SetXL(pXL, "(",						m_nSttRow, "I", "I", 0, BLACK, 9);
	pOut->SetXL(pXL, "플랜지의 단면적",			m_nSttRow, "J", "O", 0, BLACK, 9);
	pOut->SetXL(pXL, "Af =",					m_nSttRow, "P", "Q", 0, BLACK, 9);
	pOut->SetXL(pXL, dFlangeArea,				m_nSttRow, "R", "T", 0, RED,   9, FALSE, TA_CENTER, "dFlangeArea", 1);
	pOut->SetXL(pXL, "mm²)",					 m_nSttRow, "U", "W", 0, BLACK, 9);
	m_nSttRow--;

	pOut->SetXL(pXL, "설계 축력",				m_nSttRow, "D", "G", 0, BLACK, 9);
	sText.Format("=IF(%s>%s,\" P = f·Af = \"&%s&\" × \"&%s&\" / 1000 = \"&FIXED(%s*%s/1000,3)&\" kN\",\" P = 0.75 fa·Af = \"&%s&\" × \"&%s&\" / 1000 = \"&FIXED(%s*%s/1000,3)&\" kN\")",
		pOut->GetCellRef("dF"), pOut->GetCellRef("ReFa"), pOut->GetCellRef("dF"), pOut->GetCellRef("dFlangeArea"), 
		pOut->GetCellRef("dF"), pOut->GetCellRef("dFlangeArea"), pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFlangeArea"), 
		pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFlangeArea"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "I", "I", 1, BLACK, 9);

	m_nSttRow++;	
	
	pOut->SetXL(pXL, "사용 볼트",				m_nSttRow, "D", "H", 1, BLACK, 9);
	pOut->SetXL(pXL, sBolt1,					m_nSttRow, "F", "G", 0, RED,   9, FALSE, TA_CENTER, "sBolt1", 0);
	pOut->SetXL(pXL, sBolt2,					m_nSttRow, "I", "I", 0, RED,   9, FALSE, TA_CENTER, "sBolt2", 0);
	pOut->SetXL(pXL, dDiaBolt,					m_nSttRow, "J", "K", 0, RED,   9, FALSE, TA_CENTER, "sBolt3", 0);
	pOut->SetXL(pXL, sBolt4,					m_nSttRow, "L", "O", 0, RED,   9, FALSE, TA_CENTER, "sBolt4", 0);
	pOut->SetXL(pXL, "ρa =",					m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, tokN(dBoltAllow),			m_nSttRow, "R", "T", 0, RED,   9, FALSE, TA_CENTER, "UseBoltLoad", 1);
	pOut->SetXL(pXL, "kN",						m_nSttRow, "U", "U", 1, BLACK, 9, FALSE, TA_CENTER);	

	pOut->SetXL(pXL, "필요 볼트수",				m_nSttRow, "D", "H", 1, BLACK, 9);	
	pOut->SetXL(pXL, "n",						m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P / ρa",					 m_nSttRow, "G", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,%s*%s/1000,%s*%s/1000)", pOut->GetCellRef("dF"), pOut->GetCellRef("ReFa"),
		pOut->GetCellRef("dF"),pOut->GetCellRef("dFlangeArea"),pOut->GetCellRef("ReFa"),pOut->GetCellRef("dFlangeArea"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ReqBolt1", 3);
	pOut->SetXL(pXL, "/",						m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseBoltLoad"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "L", "N", 1, BLACK, 9, FALSE, TA_CENTER, "ReqBolt2", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("ReqBolt1"), pOut->GetCellRef("ReqBolt2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "ReqBolt3", 1);
	pOut->SetXL(pXL, "EA",						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "→",						 m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, dUseBoltEA,				m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "dUseBoltEA", 0);
	pOut->SetXL(pXL, "개 사용",					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(",						m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dUseBoltCol,				m_nSttRow, "R", "R", 0, RED,   9, FALSE, TA_CENTER, "dUseBoltCol", 0);
	pOut->SetXL(pXL, "열 배치)",				m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("ReqBolt3"), pOut->GetCellRef("dUseBoltEA"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "X", "Y", 2, BLACK, 9, FALSE, TA_CENTER);

	if(UNType == 1)//상부
	{		
		pOut->SetXL(pXL, dPanWidth1,				m_nSttRow, "H", "K", 1, BLACK, 9, FALSE, TA_CENTER, "dDimTop", 0);
		pOut->SetXL(pXL, dDimCen,					m_nSttRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "dDimCen", 0);
		sText.Format("=(%s-%s)/2", pOut->GetCellRef("dDimTop"), pOut->GetCellRef("dDimCen"));
		pOut->SetXL(pXL, sText,						m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "dDimLeft", 0);
		sText.Format("=%s", pOut->GetCellRef("dDimLeft"));
		pOut->SetXL(pXL, sText,						m_nSttRow, "K", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

		if (dPanEA2 == 1)
			pOut->InsertImage(pXL, "Guesset_상하부1", m_nSttRow-1, "D");
		else if(dPanEA2 == 2)
			pOut->InsertImage(pXL, "Guesset_상하부3", m_nSttRow-1, "D");
		else
			pOut->InsertImage(pXL, "Guesset_상하부1", m_nSttRow-1, "D");
	}
	else//하부 
	{
		pOut->SetXL(pXL, dPanWidth1*2 + dDimCen,	m_nSttRow, "H", "K", 1, BLACK, 9, FALSE, TA_CENTER, "dDimTop", 0);
		pOut->SetXL(pXL, dDimCen,					m_nSttRow, "I", "J", 0, RED, 9, FALSE, TA_CENTER, "dDimCen", 0);
		sText.Format("=(%s-%s)/2", pOut->GetCellRef("dDimTop"), pOut->GetCellRef("dDimCen"));
		pOut->SetXL(pXL, sText,						m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "dDimLeft", 0);
		sText.Format("=%s", pOut->GetCellRef("dDimLeft"));
		pOut->SetXL(pXL, sText,						m_nSttRow, "K", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

		if (dPanEA1 == 2)
		{
			if (dPanEA2 == 1)
				pOut->InsertImage(pXL, "Guesset_상하부3", m_nSttRow-1, "D");			
			else
				pOut->InsertImage(pXL, "Guesset_상하부2", m_nSttRow-1, "D");
		}
		else
		{
			if (dPanEA2 == 1)
				pOut->InsertImage(pXL, "Guesset_상하부1", m_nSttRow-1, "D");			
			else
				pOut->InsertImage(pXL, "Guesset_상하부2", m_nSttRow-1, "D");
		}
		
	}

	SetLines(9);

	pOut->SetXL(pXL, "연결판 검토",				m_nSttRow, "D", "H", 1, BLACK, 9);
	pOut->SetXL(pXL, "사용연결판",				m_nSttRow, "E", "I", 0, BLACK, 9);

	pOut->SetXL(pXL, dPanEA1,					m_nSttRow, "K", "K", 0, RED,   9, FALSE, TA_CENTER, "dPanEA1", 0);
	pOut->SetXL(pXL, "-PL",						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPanWidth1,				m_nSttRow, "N", "P", 0, RED,   9, FALSE, TA_CENTER, "dPanWidth1", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPanThick1,				m_nSttRow, "R", "S", 1, RED,   9, FALSE, TA_CENTER, "dPanThick1", 0);
	
	//16 line은 연결판이 한쪽에만 설치되는 경우엔 삭제
	pOut->SetXL(pXL, dPanEA2,					m_nSttRow, "K", "K", 0, RED,   9, FALSE, TA_CENTER, "dPanEA2", 0);
	pOut->SetXL(pXL, "-PL",						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPanWidth2,				m_nSttRow, "N", "P", 0, RED,   9, FALSE, TA_CENTER, "dPanWidth2", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPanThick2,				m_nSttRow, "R", "S", 1, RED,   9, FALSE, TA_CENTER, "dPanThick2", 0);
	//=IF(L9="(1면마찰)",K15*N15*R15,K15*N15*R15+K16*N16*R16)
	pOut->SetXL(pXL, "전단면적 Ag =",			m_nSttRow, "H", "M", 0, BLACK, 9, FALSE, TA_CENTER);

	if(UNType == 1)//상부
	{
		sText.Format("=IF(%s=\"(1면마찰)\",%s*%s*%s,%s*%s*%s+%s*%s*%s)", 
			pOut->GetCellRef("sBolt4"), 
			pOut->GetCellRef("dPanEA1"), pOut->GetCellRef("dPanWidth1"), pOut->GetCellRef("dPanThick1"),
			pOut->GetCellRef("dPanEA1"), pOut->GetCellRef("dPanWidth1"), pOut->GetCellRef("dPanThick1"),
			pOut->GetCellRef("dPanEA2"), pOut->GetCellRef("dPanWidth2"), pOut->GetCellRef("dPanThick2"));
		pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "DanAg", 1);
		pOut->SetXL(pXL, "mm²",					 m_nSttRow, "R", "S", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "순단면적 An =",			m_nSttRow, "H", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		if (dPanEA1 == 1 || dPanEA2 == 1)
		{
			sText.Format("=IF(%s=\"(1면마찰)\",%s&\" - ( 25 × \"&%s&\" + \"&%s&\" ) × \"&%s&\" =\" ,%s&\" - { ( 25 × \"&%s&\" + \"&%s&\" ) × \"&%s&\" + 25 × \"&%s&\" × \"&%s&\" } = \")", 
			pOut->GetCellRef("sBolt4"), pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"), pOut->GetCellRef("dPanThick1"),		
			pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"), pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
							
			if (sBolt4 == "(1면마찰)")
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "Y",  0, BLACK, 9, FALSE, TA_LEFT, "",1);			
				sText.Format("=%s-(25*%s+%s)*%s",
					pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"), pOut->GetCellRef("dPanThick1"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "Z",  "AB", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AC", "AD", 1, BLACK, 9, FALSE, TA_LEFT, "",1);

			}
			else
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "AD",  0, BLACK, 9, FALSE, TA_LEFT, "",1);
				sText.Format("=%s-((25*%s+%s)*%s+25*%s*%s)", pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"),
					 pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AH", "AI", 1, BLACK, 9, FALSE, TA_LEFT, "",1);
			}				
		}
		else
		{
			sText.Format("=IF(%s=\"(1면마찰)\",%s&\" -  25 × \"&%s&\" × \"&%s&\" = \",%s&\" - ( 25 × \"&%s&\" × \"&%s&\" + 25 × \"&%s&\" × \"&%s&\" ) = \")",
			pOut->GetCellRef("sBolt4"), pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick1"),		
			pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"), pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
			
			
			if (sBolt4 == "(1면마찰)")
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "Y",  0, BLACK, 9, FALSE, TA_LEFT, "",1);
				sText.Format("=%s-25*%s*%s",pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"), pOut->GetCellRef("dPanThick1"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "Z",  "AB", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AC", "AD", 1, BLACK, 9, FALSE, TA_LEFT, "",1);
			}
			else
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "AD",  0, BLACK, 9, FALSE, TA_LEFT, "",1);
				sText.Format("=%s-(25*%s*%s+25*%s*%s)",pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"), 
					pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AH", "AI", 1, BLACK, 9, FALSE, TA_LEFT, "",1);
			}			
		}
	}
	else//하부면
	{
		sText.Format("=IF(%s=\"(1면마찰)\",%s*%s*%s,%s*%s*%s+%s*%s*%s)", 
			pOut->GetCellRef("sBolt4"), 
			pOut->GetCellRef("dPanEA2"), pOut->GetCellRef("dPanWidth2"), pOut->GetCellRef("dPanThick2"),
			pOut->GetCellRef("dPanEA1"), pOut->GetCellRef("dPanWidth1"), pOut->GetCellRef("dPanThick1"),
			pOut->GetCellRef("dPanEA2"), pOut->GetCellRef("dPanWidth2"), pOut->GetCellRef("dPanThick2"));
		pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "DanAg", 1);
		pOut->SetXL(pXL, "mm²",					 m_nSttRow, "R", "S", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "순단면적 An =",			m_nSttRow, "H", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		if (dPanEA1 == 1 || dPanEA2 == 1)
		{
			sText.Format("=IF(%s=\"(1면마찰)\",%s&\" - ( 25 × \"&%s&\" + \"&%s&\" ) × \"&%s&\" =\" ,%s&\" - { ( 25 × \"&%s&\" + \"&%s&\" ) × \"&%s&\" + 25 × \"&%s&\" × \"&%s&\" } = \")", 
			pOut->GetCellRef("sBolt4"), pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"), pOut->GetCellRef("dPanThick2"),		
			pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"), pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
							
			if (sBolt4 == "(1면마찰)")
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "Y",  0, BLACK, 9, FALSE, TA_LEFT, "",1);			
				sText.Format("=%s-(25*%s+%s)*%s",
					pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"), pOut->GetCellRef("dPanThick2"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "Z",  "AB", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AC", "AD", 1, BLACK, 9, FALSE, TA_LEFT, "",1);

			}
			else
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "AD",  0, BLACK, 9, FALSE, TA_LEFT, "",1);
				sText.Format("=%s-((25*%s+%s)*%s+25*%s*%s)", pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dDimCen"),
					 pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AH", "AI", 1, BLACK, 9, FALSE, TA_LEFT, "",1);
			}				
		}
		else
		{
			sText.Format("=IF(%s=\"(1면마찰)\",%s&\" -  25 × \"&%s&\" × \"&%s&\" = \",%s&\" - ( 25 × \"&%s&\" × \"&%s&\" + 25 × \"&%s&\" × \"&%s&\" ) = \")",
			pOut->GetCellRef("sBolt4"), pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"),		
			pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"), pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
			
			
			if (sBolt4 == "(1면마찰)")
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "Y",  0, BLACK, 9, FALSE, TA_LEFT, "",1);
				sText.Format("=%s-25*%s*%s",pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"), pOut->GetCellRef("dPanThick2"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "Z",  "AB", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AC", "AD", 1, BLACK, 9, FALSE, TA_LEFT, "",1);
			}
			else
			{
				pOut->SetXL(pXL, sText,						m_nSttRow, "N",  "AD",  0, BLACK, 9, FALSE, TA_LEFT, "",1);
				sText.Format("=%s-(25*%s*%s+25*%s*%s)",pOut->GetCellRef("DanAg"), pOut->GetCellRef("dUseBoltCol"), 
					pOut->GetCellRef("dPanThick1"), pOut->GetCellRef("dUseBoltCol"),pOut->GetCellRef("dPanThick2"));
				pOut->SetXL(pXL, sText,						m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_LEFT, "DanAn",1);
				pOut->SetXL(pXL, "mm²",					 m_nSttRow, "AH", "AI", 1, BLACK, 9, FALSE, TA_LEFT, "",1);
			}						
		}
	}

	pOut->SetXL(pXL, "연결판 응력",				m_nSttRow, "D", "H", 0, BLACK, 9);
	pOut->SetXL(pXL, "f",						m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P / An",					m_nSttRow, "L", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("ReqBolt1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER, "Con1", 3);
	pOut->SetXL(pXL, "x  1000  /",				m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("DanAn"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "Con2", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*1000/%s", pOut->GetCellRef("Con1"), pOut->GetCellRef("Con2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "X", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "ConLoad", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "AB", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,\"＞\",\"＜\")", pOut->GetCellRef("ConLoad"), pOut->GetCellRef("dFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AD", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "fa",						m_nSttRow, "AE", "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,\"N.G\",\"O.K\")", pOut->GetCellRef("ConLoad"), pOut->GetCellRef("dFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AG", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

	SetLines(NEXT);
}

void CXLDesignCrossBeam::DrawCrossBeam(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CXLControl			*pXL		= m_pXL;
	
	CAPlateDrawCross DrawCross(m_pAPlateOutCalcStd->GetDataManage());	
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirR   = pGir->m_pRight;
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGirR);
	
	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(30);

	//정면도
	if(bLeft)	DrawCross.DrawGirderFront(&Dom, pBx, CAPlateDrawCross::ALL);
	else		DrawCross.DrawGirderFront(&Dom, pBxNext, CAPlateDrawCross::ALL);

	DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);// 복부판, 상판, 하판
	DrawCross.DrawCRBeamSpliceOnBx(&Dom, pBx);      // 상하플랜지 이음판, 전단이음판, 모멘트이음판
	DrawCross.DrawCRBeamGassetOnBx(&Dom, pBx);      // 가셋트-상하플랜지판, 복부판, 보강판
	DrawCross.DrawCRBeamVStiffnerOnBx(&Dom, pBx);   // 수직보강재
	DrawCross.DrawCRBeamAnchorBar(&Dom, pBx);       // 슬래브앵커
	DrawCross.DrawFrontHStiffBx(&Dom, pBx);		  // 가로보 부착 수평보강재
	
	DrawCross.DrawHStiffFront(&Dom, pBx, TRUE);	  // 거더 부착 수평보강재
	DrawCross.DrawHStiffFront(&Dom, pBx, FALSE);	  // 거더 부착 수평보강재
	DrawCross.DrawHStiffFront(&Dom, pBxNext, TRUE);	  // 수평보강재
	DrawCross.DrawHStiffFront(&Dom, pBxNext, FALSE);	  // 수평보강재
	
	DrawCross.DrawFrontHGussetSplice(&Dom, pBx, TRUE);
	DrawCross.DrawFrontHGussetSplice(&Dom, pBxNext, TRUE);

	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	CDPoint xyTrimStt(0,0), xyTrimEnd(0,0);
	if(bLeft)
	{
		xyTrimStt.x = rectDom.left + rectDom.Width()*0.3;
		xyTrimEnd.x = rectDom.left + rectDom.Width()*0.3;
		xyTrimStt.y = rectDom.top;
		xyTrimEnd.y = rectDom.bottom;

		Dom.TrimByLineOnSide(xyTrimStt, xyTrimEnd, 1, TRUE);
		Dom.CutLightningAnyDir(xyTrimStt, xyTrimEnd, 0);
	}
	else
	{
		xyTrimStt.x = rectDom.left + rectDom.Width()*0.7;
		xyTrimEnd.x = rectDom.left + rectDom.Width()*0.7;
		xyTrimStt.y = rectDom.top;
		xyTrimEnd.y = rectDom.bottom;

		Dom.TrimByLineOnSide(xyTrimStt, xyTrimEnd, -1, TRUE);
		Dom.CutLightningAnyDir(xyTrimStt, xyTrimEnd, 0);
	}

	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 15, 5, FALSE, FALSE);

}

void CXLDesignCrossBeam::ConnectGussetWeb(CPlateBasicIndex *pBx, long LRType)//가로보연결 - 이음판 없는 복부 플랜지
{
	CAPlateCalcStd		*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CDesignCrossBeamJoint	Joint(pCalcStd, pBx);
	CDesignWebJoint		*pWeb		= NULL;
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CXLControl			*pXL		= m_pXL;

	if (LRType == 1)//왼쪽이면
	{
		pWeb = Joint.m_pWebLeft;
	}
	else
	{
		pWeb = Joint.m_pWebRight;
	}

	CString sText = _T("");		
	CString sBolt1, sBolt2;
	
	double dAF = tokN(fabs(pWeb->m_strtGusset.m_dApplyLoad));	// 56.7890;//작용전단력
	double dUseBoltEA = pWeb->m_strtGusset.m_dBoltUse;
	sBolt1 = pWeb->m_strtGusset.m_strBoltMaterial;		//"F10T";
	sBolt2 =  "M";										//pWeb->m_strtGusset.m_dDiaBolt;
	double dDiaBolt		= pWeb->m_strtGusset.m_dDiaBolt;
	double dBoltAllow	= pWeb->m_strtGusset.m_dAllowBolt;

	CPlateCrossBeam *pCr	= pBx->GetSection()->GetCrossBeam();

	if(pCr->m_nLGirSpType==0 && pCr->m_nRGirSpType==0)
		pOut->SetXL(pXL, "(1) 복부판 연결",	m_nSttRow, "C", "C", 1, BLACK, 9);
	else
		pOut->SetXL(pXL, "(3) 복부판 연결",	m_nSttRow, "C", "C", 1, BLACK, 9);
	
	pOut->SetXL(pXL, "작용전단력",				m_nSttRow, "D", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "S=",						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, dAF,						m_nSttRow, "K", "N", 0, RED,   9, FALSE, TA_CENTER, "dAF");
	pOut->SetXL(pXL, "kN",						m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER);

	DrawCrossBeam(pBx, LRType==1?TRUE:FALSE);
//	pOut->InsertImage(pXL, "Guesset_Web", m_nSttRow, "Z");
	pOut->SetXL(pXL, "사용 BOLT",				m_nSttRow, "D", "H", 1, BLACK, 9);
	pOut->SetXL(pXL, sBolt1,					m_nSttRow, "F", "G", 0, RED,   9, FALSE, TA_CENTER, "sBolt1", 0);
	pOut->SetXL(pXL, sBolt2,					m_nSttRow, "I", "I", 0, RED,   9, FALSE, TA_CENTER, "sBolt2", 0);
	pOut->SetXL(pXL, dDiaBolt,					m_nSttRow, "J", "K", 0, RED,   9, FALSE, TA_CENTER, "sBolt3", 0);
	pOut->SetXL(pXL, "(1면마찰)",				m_nSttRow, "L", "O", 0, BLACK, 9, FALSE, TA_CENTER, "sBolt4", 0);
	pOut->SetXL(pXL, "ρa =",					m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, tokN(dBoltAllow),				m_nSttRow, "R", "T", 0, RED,   9, FALSE, TA_CENTER, "UseBolt", 1);	
	pOut->SetXL(pXL, "kN",						m_nSttRow, "U", "U", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "필요 볼트수",				m_nSttRow, "D", "H", 1, BLACK, 9);
	pOut->SetXL(pXL, "n",						m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "S / ρa",					 m_nSttRow, "G", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAF"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ReqBolt1", 3);
	pOut->SetXL(pXL, "/",						m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseBolt"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "L", "M", 1, BLACK, 9, FALSE, TA_CENTER, "ReqBolt2", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("ReqBolt1"), pOut->GetCellRef("ReqBolt2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "ReqBolt3", 1);
	pOut->SetXL(pXL, "EA",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "→",						m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, dUseBoltEA,				m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "dUseBoltEA", 0);
	pOut->SetXL(pXL, "개 사용",					m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("ReqBolt3"), pOut->GetCellRef("dUseBoltEA"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "S", "T", 1, BLACK, 9, FALSE, TA_CENTER);

	SetLines(NEXT);
}

void CXLDesignCrossBeam::ConnectExistTop(CPlateBasicIndex *pBx)//가로보연결 - 기존방식 상부 플랜지
{
	CAPlateCalcStd			*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CDesignCrossBeamJoint	Joint(pCalcStd, pBx);
	CDesignFlangeJoint		*pUpper		= Joint.m_pUpper;
	CAPlateOutCalcXL		*pOut		= m_pAPlateOutCalcXL;
	CXLControl				*pXL		= m_pXL;
	CSteel					*pSteelCtl	= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();
	
	CString sText			= _T("");
	CString sLeftMaterial	= pUpper->m_strtPlate.m_strMaterial;
	double dFu				= pUpper->m_strtPlate.m_dStress; //29.4; //작용응력
	double dL1				= pUpper->m_strtPlate.m_dL1; //115;	
	double dtf				= pUpper->m_strtPlate.m_dThick; //10;
	double dL				= pUpper->m_strtPlate.m_dWidth; //250;
	double dtw				= pUpper->m_strtPlate.m_dTw; //10;
	CString dUseBoltT		= pUpper->m_strtPlate.m_sLeftBoltMaterial;
	double dUseBoltEA		= pUpper->m_strtPlate.m_dQtyBoltUse;//6;
	double dUseBoltCol		= pUpper->m_strtPlate.m_nQtyLine;//3;
	double dBoltMSize		= pUpper->m_strtPlate.m_dDiaBolt;
	double dBoltAllow		= pUpper->m_strtPlate.m_dAllowBolt;
	double dA1				= pUpper->m_strtPlate.m_dA1;
	double dA2				= pUpper->m_strtPlate.m_dA2;
	double dA3				= pUpper->m_strtPlate.m_dA3;
	double dts				= pUpper->m_strtPlate.m_dTsUse;
	double dApplySF			= pUpper->m_strtPlate.m_dShear;
	double dDanMom1			= pUpper->m_strtPlate.m_dQ1;
	double dDanMom2			= pUpper->m_strtPlate.m_dI;
	long   nTopConEA		= pUpper->m_strtPlate.m_nTopConEA;
	long   nBottomConEA		= pUpper->m_strtPlate.m_nBottomConEA;

	pOut->SetXL(pXL, "(1) 상부 플랜지 이음",	m_nSttRow, "C", "C", 1, BLACK, 9);
	pOut->SetXL(pXL, "작용응력 : fu =",			m_nSttRow, "C", "H", 0, BLACK, 9);	
	pOut->SetXL(pXL, dFu,						m_nSttRow, "K", "N", 0, RED,   9, FALSE, TA_CENTER, "dFu");
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, " ( 가로보 계산 참조)",	m_nSttRow, "S", "S", 1, BLACK, 9);
	pOut->SetXL(pXL, "모재의 허용응력 :",		m_nSttRow, "C", "H", 0, BLACK, 9);	
	pOut->SetXL(pXL, "fa =",					m_nSttRow, "J", "J", 0, BLACK, 9);

	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "(",						m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, sLeftMaterial,				m_nSttRow, "V", "Y", 0, RED,   9, FALSE, TA_CENTER, "MatNM");
	pOut->SetXL(pXL, "사용 )",					m_nSttRow, "Z", "AA", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(2);

	pOut->SetXL(pXL, "Af = tf·l =",			m_nSttRow, "K", "K", 0, BLACK, 9);
	pOut->SetXL(pXL, dtf,						m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "dtf", 1);
	pOut->SetXL(pXL, "×",						m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dL,						m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "dL", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("dtf"), pOut->GetCellRef("dL"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "dAf", 1);
	pOut->SetXL(pXL, "mm²",	 				m_nSttRow, "Z", "AA", 1, BLACK, 9, FALSE, TA_CENTER);
	
	SetLines(-4);

//	CString strMaterial = sLeftMaterial;
//	if(strMaterial == "SM400")	strMaterial.Format("%s%s",strMaterial, "B");
	CCalcGeneral	CalcGen(m_pAPlateOutCalcStd->GetDataManage());
	CString	szLeft, szRight;
	if(pBx->IsJijum())	CalcGen.GetMaterial(pBx, CP_W, szLeft, szRight);
	else				CalcGen.GetMaterial(pBx, CG_W, szLeft, szRight);

	CHGBaseSteel		*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft+szRight)];
	CHGAllowStress		AllowTension(pSteel);

	double dTension = AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dtf);
	pOut->SetXL(pXL, dTension,					m_nSttRow, "N", "Q", 1, RED, 9, FALSE, TA_CENTER, "dFa", 0);	
	
	pOut->SetXL(pXL, "0.75 fa =",				m_nSttRow, "J", "J", 0, BLACK, 9);	
	sText.Format("=0.75*%s", pOut->GetCellRef("dFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "ReFa", 3);	
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=IF(%s>%s,\">\",\"<\")", pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFu"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "fu",						m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "이므로",					m_nSttRow, "AA", "AA", 1, BLACK, 9);

	pOut->SetXL(pXL, "fu' =",					m_nSttRow, "K", "K", 0, BLACK, 9);
	sText.Format("=IF(%s>%s,%s,%s)", pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFu"),
		pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFu"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "dFu'", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "를 사용하여 설계",		m_nSttRow, "U", "U", 0, BLACK, 9);	
	pOut->SetXL(pXL, "( ☞ 도.설 3.5.1.1 )",	m_nSttRow, "AC", "AC", 1, BLACK, 9);
	
	pOut->InsertImage(pXL,"CROSSBEAM연결_상부", m_nSttRow+2,"D");

	SetLines(1);
	pOut->SetXL(pXL, "볼트의 필요갯수 산정 ( 2면마찰 )",  m_nSttRow, "T", "T", 1);
	pOut->SetXL(pXL, "H.T.B",					m_nSttRow, "U", "U", 0);
	pOut->SetXL(pXL, dUseBoltT,					m_nSttRow, "Y", "Z", 0, RED, 9, FALSE, TA_CENTER, "dUseBoltT", 0);
	pOut->SetXL(pXL, "T ( M",					m_nSttRow, "AA", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dBoltMSize,				m_nSttRow, "AD", "AE", 0, RED, 9, FALSE, TA_CENTER, "dBoltMSize", 0);
	pOut->SetXL(pXL, ") 사용",					m_nSttRow, "AF", "AF", 1);

	pOut->SetXL(pXL, "⇒ ρa = ",			    m_nSttRow, "U", "U", 0);
	pOut->SetXL(pXL, dBoltAllow,				m_nSttRow, "Y", "AB", 0, RED, 9, FALSE, TA_CENTER, "dPa", 1);
	pOut->SetXL(pXL, "N",					    m_nSttRow, "AC", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( ☞ 도.설 3.3.2.3 )",	m_nSttRow, "AE", "AE", 1, BLACK, 9);

	pOut->SetXL(pXL, "N = Af·f / ( 2·ρa )",	m_nSttRow, "U", "U", 1);
	pOut->SetXL(pXL, "=",					    m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "N1", 1);
	pOut->SetXL(pXL, "×",						 m_nSttRow,"Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dFu'"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AA", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "N2", 1);
	pOut->SetXL(pXL, "/",						m_nSttRow, "AE", "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(",						m_nSttRow, "AF", "AF", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2,							m_nSttRow, "AG", "AG", 0, BLACK, 9, FALSE, TA_CENTER, "N3", 0);
	pOut->SetXL(pXL, "×",						m_nSttRow, "AH", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dPa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AI", "AL", 0, BLACK, 9, FALSE, TA_CENTER, "N4", 1);
	pOut->SetXL(pXL, ")",						m_nSttRow, "AM", "AM", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",					    m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s/(%s*%s)", pOut->GetCellRef("N1"), pOut->GetCellRef("N2"),
		pOut->GetCellRef("N3"), pOut->GetCellRef("N4"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "dReqBoltEA", 1);
	pOut->SetXL(pXL, "개",					    m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "⇒",					    m_nSttRow, "AB", "AB", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, dUseBoltEA,				m_nSttRow, "AC", "AD", 0, RED, 9, FALSE, TA_CENTER, "dUseBoltEA", 0);
	pOut->SetXL(pXL, "개 사용 (",			    m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dUseBoltCol,				m_nSttRow, "AH", "AI", 0, RED, 9, FALSE, TA_CENTER, "dUseBoltCol", 0);
	pOut->SetXL(pXL, "열배치)",					m_nSttRow, "AJ", "AJ", 0);
	sText.Format("=IF(%s>%s,\"N.G\",\"O.K\")", pOut->GetCellRef("dReqBoltEA"), pOut->GetCellRef("dUseBoltEA"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AN", "AO", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "tf =",				    m_nSttRow, "W", "X", 0);
	sText.Format("=%s", pOut->GetCellRef("dtf"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "Usetf", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);

	pOut->SetXL(pXL, "tw =",				    m_nSttRow, "W", "X", 0);
	pOut->SetXL(pXL, dtw,						m_nSttRow, "Y", "AA", 0, RED, 9, FALSE, TA_CENTER, "Usetw", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);

	pOut->SetXL(pXL, "L =",						m_nSttRow, "W", "X", 0);
	sText.Format("=%s", pOut->GetCellRef("dL"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "UseL", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);

	pOut->SetXL(pXL, "L1 =",				    m_nSttRow, "W", "X", 0);	
	pOut->SetXL(pXL, dL1,						m_nSttRow, "Y", "AA", 0, RED, 9, FALSE, TA_CENTER, "UseL1", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);
	
	m_nSttRow++;
	
	pOut->SetXL(pXL, "A1 =",				    m_nSttRow, "W", "X", 0);
	sText.Format("=%s-1", pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "NA0", 0);
	pOut->SetXL(pXL, "@",						m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dA1,						m_nSttRow, "AA", "AC", 0, RED, 9, FALSE, TA_CENTER, "NA1", 0);
	pOut->SetXL(pXL, "A2 =",				    m_nSttRow, "AE", "AF", 0);
	pOut->SetXL(pXL, dA2,						m_nSttRow, "AG", "AI", 0, RED, 9, FALSE, TA_CENTER, "NA2", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AJ", "AK", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "X", "X", 0);
	sText.Format("=%s*%s", pOut->GetCellRef("NA0"), pOut->GetCellRef("NA1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "UseA1", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 0);
	pOut->SetXL(pXL, "A3 =",				    m_nSttRow, "AE", "AF", 0);
	pOut->SetXL(pXL, dA3,						m_nSttRow, "AG", "AI", 0, RED, 9, FALSE, TA_CENTER, "NA3", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AJ", "AK", 1);
	
	SetLines(-3);
	pOut->SetXL(pXL, "A =",						m_nSttRow, "W", "X", 0);
	sText.Format("=(%s+%s)*2+%s", pOut->GetCellRef("NA2"), pOut->GetCellRef("UseA1"),
		pOut->GetCellRef("NA3"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "UseA", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);
	SetLines(2);

	pOut->SetXL(pXL, "SPLICE 두께산정",		m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "Asn =",				m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",					m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "{(",					m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Asn1", 0);
	pOut->SetXL(pXL, "-",					m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<27,%s+3,%s+4)", pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "Asn2", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER, "Asn3", 0);
	pOut->SetXL(pXL, ")   +   (",			m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseL1"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "Asn4", 0);
	pOut->SetXL(pXL, "-",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("Asn2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "Asn5", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s/2", pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AA", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "Asn6", 0);
	pOut->SetXL(pXL, ")",					m_nSttRow, "AB", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "AC", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2,						m_nSttRow, "AD", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "Asn7", 0);
	pOut->SetXL(pXL, "EA)}",				m_nSttRow, "AE", "AE", 1);
	pOut->SetXL(pXL, "=",					m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s-%s*%s)+(%s-%s*%s)*%s", pOut->GetCellRef("Asn1"),
		pOut->GetCellRef("Asn2"), pOut->GetCellRef("Asn3"), pOut->GetCellRef("Asn4"),
		pOut->GetCellRef("Asn5"), pOut->GetCellRef("Asn6"), pOut->GetCellRef("Asn7"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "Asn8", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",					m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "≥",					 m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Af",					m_nSttRow, "O", "O", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "∴",					m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",					m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "≥",					 m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Af",					m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "/",					m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("Asn8"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "=",					m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "Asn9", 1);
	pOut->SetXL(pXL, "/",					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("Asn8"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "Asn10", 1);
	pOut->SetXL(pXL, "=",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("Asn9"), pOut->GetCellRef("Asn10"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "X", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "Asn11", 1);
	pOut->SetXL(pXL, "mm",					m_nSttRow, "AA", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "⇒",					m_nSttRow, "AC", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts =",				m_nSttRow, "AD", "AF", 0, BLUE, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dts,					m_nSttRow, "AG", "AH", 0, RED, 9, FALSE, TA_CENTER, "dts", 0);
	pOut->SetXL(pXL, "mm 사용",				m_nSttRow, "AI", "AK", 0, BLUE, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,\"N.G\",\"O.K\")", pOut->GetCellRef("Asn11"), pOut->GetCellRef("dts"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AN", "AO", 1, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetXL(pXL, "연결판",				m_nSttRow, "C", "C", 1);	
	pOut->SetXL(pXL, nTopConEA,				m_nSttRow, "D", "D", 0, RED,   9, FALSE, TA_CENTER, "nTopConEA", 0);
	pOut->SetXL(pXL, "-",					m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "PL.",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ConO2", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dts"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "ConO3", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseA"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER, "ConO4", 0);	
	pOut->SetXL(pXL, nBottomConEA,			m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "nBottomConEA", 0);
	pOut->SetXL(pXL, "-",					m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "PL.",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseL1"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ConT2", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dts"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "ConT3", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseA"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "ConT4", 0);
	//밑줄 쫙....		
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "D", "P", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "Σ As =",				 m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s*%s*%s+%s*%s*%s)",
		pOut->GetCellRef("nTopConEA"), pOut->GetCellRef("ConO2"), pOut->GetCellRef("ConO3"),
		pOut->GetCellRef("nBottomConEA"), pOut->GetCellRef("ConT2"), pOut->GetCellRef("ConT3"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "As1", 0);
	pOut->SetXL(pXL, "mm2",					m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ">",					m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Af =",				m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER, "As2", 0);
	pOut->SetXL(pXL, "mm2",					m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("As1"), pOut->GetCellRef("As2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AA", "AB", 1, BLUE, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "⊙ 볼트의 응력검토",	 m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "  · 직응력",			 m_nSttRow, "B", "B", 1);
	pOut->SetLineBoxText(pXL, "ρp =",		 m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P",		m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("dtf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "P1", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=%s", pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "P2", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "I", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("=%s", pOut->GetCellRef("dFu'"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "P3", 1);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "P", "S", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);
	//SetLines(1);
	pOut->SetXL(pXL, "n",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dUseBoltEA"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "P4", 0);
	pOut->SetXL(pXL, "EA",					m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-1);
	sText.Format("=%s*%s*%s/%s", pOut->GetCellRef("P1"),
		pOut->GetCellRef("P2"), pOut->GetCellRef("P3"), pOut->GetCellRef("P4"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "U", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "dPP", 1, FALSE);	
	pOut->SetLineBoxText(pXL, "N/EA",		m_nSttRow, "Z", "AB", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=IF(%s>2*%s,\" ＞ 2·ρa    N.G\",\"＜  2·ρa    O.K\")",
		pOut->GetCellRef("dPP"), pOut->GetCellRef("dPa"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AC", "AK", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(3);

	pOut->SetXL(pXL, "  · 휨전단 응력",			 m_nSttRow, "B", "B", 1);
	pOut->SetLineBoxText(pXL, "ρh =",		 m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "S Q",		m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "×",			 m_nSttRow, "H", "I", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "p",		m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(1);
	pOut->SetXL(pXL, "I",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n",					m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(2);

	pOut->SetXL(pXL, "S : 작용전단력",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, dApplySF,				m_nSttRow, "J", "O", 0, RED, 9, FALSE, TA_CENTER, "dApplySF", 3);
	pOut->SetXL(pXL, "kN",					m_nSttRow, "P", "P", 0);
	pOut->SetXL(pXL, "( 가로보 계산 참조)",	m_nSttRow, "S", "S", 1);
	pOut->SetXL(pXL, "Q : 중립축에대한 접합선외측의 단면1차모멘트",	m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "Q =",					m_nSttRow, "D", "E", 0);
	sText.Format("=%s", pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "Q1", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dtf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Q2", 1);
	pOut->SetXL(pXL, "× (",				 m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dDanMom1,				m_nSttRow, "N", "Q", 0, RED, 9, FALSE, TA_CENTER, "dDanMom1", 1);
	pOut->SetXL(pXL, "-",					m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/2", pOut->GetCellRef("Q2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Q3", 1);
	pOut->SetXL(pXL, ") =",					m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s*(%s-%s)", pOut->GetCellRef("Q1"), pOut->GetCellRef("Q2"),
		pOut->GetCellRef("dDanMom1"), pOut->GetCellRef("Q3"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "X", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "dQ", 1);
	pOut->SetXL(pXL, "mm³",				 m_nSttRow, "AB", "AC",1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "I : 중립축에대한 총단면의 단면2차모멘트",	m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "I =",					m_nSttRow, "S", "T", 0);
	pOut->SetXL(pXL, dDanMom2,				m_nSttRow, "U", "Z", 0, RED, 9, FALSE, TA_CENTER, "dDanMom2", 1);
	pOut->SetXL(pXL, "mm⁴ ( 가로보 계산 참조)", m_nSttRow, "AA", "AA", 1);
	pOut->SetXL(pXL, "p : 접합선 방향의 볼트의 평균 핏치(mm) = 플랜지 전폭 / 횡방향 볼트수", 
											m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "=",					m_nSttRow, "AD", "AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_CENTER, "P1", 1);
	pOut->SetXL(pXL, "/",					m_nSttRow, "AH", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AI", "AJ", 0, BLACK, 9, FALSE, TA_CENTER, "P2", 1);
	pOut->SetXL(pXL, "=",					m_nSttRow, "AK", "AK", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("P1"), pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AL", "AN", 0, BLACK, 9, FALSE, TA_CENTER, "dP", 1);
	pOut->SetXL(pXL, "mm",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n : 접합선직각의 볼트의 갯수",	m_nSttRow, "C", "C", 1);

	SetLines(-9);
	sText.Format("=%s*1000", pOut->GetCellRef("dApplySF"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "M", "P", 0, BLACK, 9, FALSE, TA_CENTER, "Ph1", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dQ"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Ph2", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dP"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "Ph3", 1);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "M", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "Z", "Z", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(1);
	sText.Format("=%s", pOut->GetCellRef("dDanMom2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "N", "S", 0, BLACK, 9, FALSE, TA_CENTER, "Ph4", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "Ph5", 1);
	pOut->SetXL(pXL, "EA",					m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-1);
	sText.Format("=%s*%s*%s/%s/%s", pOut->GetCellRef("Ph1"), pOut->GetCellRef("Ph2"),
		pOut->GetCellRef("Ph3"),pOut->GetCellRef("Ph4"),pOut->GetCellRef("Ph5"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AA", "AG", 2, BLACK, 9, FALSE, TA_CENTER, "dPh", 1, FALSE);
	pOut->SetLineBoxText(pXL, "N/EA",		m_nSttRow, "AH", "AI", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	SetLines(10);
	pOut->SetXL(pXL, "  · 합성응력의 검토", m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "ρ =  √(ρp²＋ρh²)  = ",m_nSttRow, "C", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=SQRT(%s^2+%s^2)", pOut->GetCellRef("dPP"), pOut->GetCellRef("dPh"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "K", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "ppph", 1);
	sText.Format("=IF(%s>2*%s,\"N/EA  ＞ 2·ρa    N.G\",\"N/EA ＜  2·ρa    O.K\")", 
		pOut->GetCellRef("ppph"), pOut->GetCellRef("dPa"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "R", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);

	pOut->SetXL(pXL, "⊙ 모재의 응력검토",	m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "플랜지의 총단면적",	m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "As =",				m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s&\" × \"&%s&\" =\"", pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "T", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "Y", "AB", 0, BLACK, 9, FALSE, TA_CENTER, "dAs", 1);
	pOut->SetXL(pXL, "mm²",				 m_nSttRow, "AC", "AD", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "볼트 구멍을 공제한 플랜지의 순단면적  Asn =",	m_nSttRow, "C", "S", 0);	
	sText.Format("=%s&\" ×( \"&%s&\" - \"&IF(%s<27,%s+3,%s+4)&\" ×\"&%s&\" ) =\"", 
		pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"), 
		pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "T", "AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*(%s-IF(%s<27,%s+3,%s+4)*%s)", pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"), 
		pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AE", "AH", 0, BLACK, 9, FALSE, TA_CENTER, "dAsn", 1);
	pOut->SetXL(pXL, "mm²",				 m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "모재의 응력",m_nSttRow, "C", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "fu =",		m_nSttRow, "H", "J", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("dAs"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "fu1", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dFu'"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "fu2", 1);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "K", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);	
	SetLines(1);
	sText.Format("=%s", pOut->GetCellRef("dAsn"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "M", "P", 0, BLACK, 9, FALSE, TA_CENTER, "fu3", 1);
	SetLines(-1);
	pOut->SetLineBoxText(pXL, "=",		    m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	sText.Format("=%s*%s/%s", pOut->GetCellRef("fu1"), pOut->GetCellRef("fu2"), pOut->GetCellRef("fu3"));
	pOut->SetLineBoxText(pXL, sText,	    m_nSttRow, "U", "X", 2, BLACK, 9, FALSE, TA_CENTER, "dmFu", 3, FALSE);
	pOut->SetLineBoxText(pXL, "MPa",	    m_nSttRow, "Y", "AA", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	sText.Format("=IF(%s<=%s,\"≤\",\"＞\")", pOut->GetCellRef("dmFu"), pOut->GetCellRef("dFa"));
	pOut->SetLineBoxText(pXL, sText,	    m_nSttRow, "AC", "AC", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	pOut->SetLineBoxText(pXL, "fa",			m_nSttRow, "AE", "AF", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("dmFu"), pOut->GetCellRef("dFa"));
	pOut->SetLineBoxText(pXL, sText,	    m_nSttRow, "AI", "AJ", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	SetLines(3);
}
void CXLDesignCrossBeam::ConnectExistBottom(CPlateBasicIndex *pBx)//가로보연결 - 기존방식 하부 플랜지
{
	CAPlateCalcStd			*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CDesignCrossBeamJoint	Joint(pCalcStd, pBx);
	CDesignFlangeJoint		*pLower	= Joint.m_pLower;
	CAPlateOutCalcXL		*pOut		= m_pAPlateOutCalcXL;
	CXLControl				*pXL		= m_pXL;
	CSteel					*pSteelCtl	= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();

	CString sText		= _T("");
	CString strLeft		= pLower->m_strtPlate.m_strMaterial;
	double  dFl			= pLower->m_strtPlate.m_dStress;
	double  dL1			= pLower->m_strtPlate.m_dL1;
	double  dtf			= pLower->m_strtPlate.m_dThick;
	double  dL			= pLower->m_strtPlate.m_dWidth;
	double  dtw			= pLower->m_strtPlate.m_dTw;
	CString dUseBoltT	= pLower->m_strtPlate.m_sLeftBoltMaterial;
	double  dUseBoltEA	= pLower->m_strtPlate.m_dQtyBoltUse;
	double  dUseBoltCol	= pLower->m_strtPlate.m_nQtyLine;
	double  dBoltMSize	= pLower->m_strtPlate.m_dDiaBolt;
	double	dBoltAllow	= pLower->m_strtPlate.m_dAllowBolt;
	double  dA1			= pLower->m_strtPlate.m_dA1;
	double  dA2			= pLower->m_strtPlate.m_dA2;
	double  dA3			= pLower->m_strtPlate.m_dA3;
	double  dts			= pLower->m_strtPlate.m_dTsUse;
	double  dApplySF	= pLower->m_strtPlate.m_dShear;
	double  dDanMom1	= pLower->m_strtPlate.m_dQ1;
	double  dDanMom2	= pLower->m_strtPlate.m_dI;
	long    nTopConEA   = pLower->m_strtPlate.m_nTopConEA;
	long    nBottomConEA= pLower->m_strtPlate.m_nBottomConEA;
	

	pOut->SetXL(pXL, "(2) 하부 플랜지 이음",	m_nSttRow, "C", "C", 1, BLACK, 9);
	pOut->SetXL(pXL, "작용응력 : fl =",			m_nSttRow, "C", "H", 0, BLACK, 9);	
	pOut->SetXL(pXL, dFl,						m_nSttRow, "K", "N", 0, RED,   9, FALSE, TA_CENTER, "dFl");
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, " ( 가로보 계산 참조)",	m_nSttRow, "S", "S", 1, BLACK, 9);
	pOut->SetXL(pXL, "모재의 허용응력 :",		m_nSttRow, "C", "H", 0, BLACK, 9);	
	pOut->SetXL(pXL, "fa =",					m_nSttRow, "J", "J", 0, BLACK, 9);	
	
		
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "(",						m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, strLeft,					m_nSttRow, "V", "Y", 0, RED,   9, FALSE, TA_CENTER, "MatNM");
	pOut->SetXL(pXL, "사용 )",					m_nSttRow, "Z", "AA", 1, BLACK, 9, FALSE, TA_CENTER);

	SetLines(2);
	
	pOut->SetXL(pXL, "Af = tf·l =",			 m_nSttRow, "K", "K", 0, BLACK, 9);
	pOut->SetXL(pXL, dtf,						m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "dtf", 1);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dL,						m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "dL", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("dtf"), pOut->GetCellRef("dL"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "dAf", 1);
	pOut->SetXL(pXL, "mm²",	 				 m_nSttRow, "Z", "AA", 1, BLACK, 9, FALSE, TA_CENTER);

	SetLines(-4);

//	CString strMaterial = strLeft;
//	if(strMaterial == "SM400") strMaterial.Format("%s%s", strLeft, "B");
	CCalcGeneral	CalcGen(m_pAPlateOutCalcStd->GetDataManage());
	CString	szLeft, szRight;
	if(pBx->IsJijum())	CalcGen.GetMaterial(pBx, CP_W, szLeft, szRight);
	else				CalcGen.GetMaterial(pBx, CG_W, szLeft, szRight);
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft+szRight)];
	CHGAllowStress	AllowTension(pSteel);

	double dTension = AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dtf);
	pOut->SetXL(pXL, dTension,						m_nSttRow, "N", "Q", 1, RED, 9, FALSE, TA_CENTER, "dFa", 0);

	pOut->SetXL(pXL, "0.75 fa =",				m_nSttRow, "J", "J", 0, BLACK, 9);	
	sText.Format("=0.75*%s", pOut->GetCellRef("dFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "ReFa", 3);	
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=IF(%s>%s,\">\",\"<\")", pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFl"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "fl",						m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "이므로",					m_nSttRow, "AA", "AA", 1, BLACK, 9);

	pOut->SetXL(pXL, "fl' =",					m_nSttRow, "K", "K", 0, BLACK, 9);
	sText.Format("=IF(%s>%s,%s,%s)", pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFl"),
		pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFl"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "dFl'", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "를 사용하여 설계",		m_nSttRow, "U", "U", 0, BLACK, 9);	
	pOut->SetXL(pXL, "( ☞ 도.설 3.5.1.1 )",	m_nSttRow, "AC", "AC", 1, BLACK, 9);
	
	if(nBottomConEA ==1)//하단 브라켓 보강판이 없는 경우는 상부 이음과 같은 형식의 그림 삽입
		pOut->InsertImage(pXL,"CROSSBEAM연결_하부2", m_nSttRow+2,"D");
	else
		pOut->InsertImage(pXL,"CROSSBEAM연결_하부", m_nSttRow+2,"D");

	SetLines(1);

	pOut->SetXL(pXL, "볼트의 필요갯수 산정 ( 2면마찰 )",  m_nSttRow, "T", "T", 1);
	pOut->SetXL(pXL, "H.T.B",					m_nSttRow, "U", "U", 0);
	pOut->SetXL(pXL, dUseBoltT,					m_nSttRow, "Y", "Z", 0, RED, 9, FALSE, TA_CENTER, "dUseBoltT", 0);
	pOut->SetXL(pXL, "T ( M",					m_nSttRow, "AA", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dBoltMSize,				m_nSttRow, "AD", "AE", 0, RED, 9, FALSE, TA_CENTER, "dBoltMSize", 0);
	pOut->SetXL(pXL, ") 사용",					m_nSttRow, "AF", "AF", 1);

	pOut->SetXL(pXL, "⇒ ρa = ",			    m_nSttRow, "U", "U", 0);
	pOut->SetXL(pXL, dBoltAllow,				m_nSttRow, "Y", "AB", 0, RED, 9, FALSE, TA_CENTER, "dPa", 1);
	pOut->SetXL(pXL, "N",					    m_nSttRow, "AC", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( ☞ 도.설 3.3.2.3 )",	m_nSttRow, "AE", "AE", 1, BLACK, 9);

	pOut->SetXL(pXL, "N = Af·f / ( 2·ρa )",	m_nSttRow, "U", "U", 1);
	pOut->SetXL(pXL, "=",					    m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "N1", 1);
	pOut->SetXL(pXL, "×",						m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dFl'"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AA", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "N2", 1);
	pOut->SetXL(pXL, "/",						m_nSttRow, "AE", "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(",						m_nSttRow, "AF", "AF", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2,							m_nSttRow, "AG", "AG", 0, BLACK, 9, FALSE, TA_CENTER, "N3", 0);
	pOut->SetXL(pXL, "×",						m_nSttRow, "AH", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dPa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AI", "AL", 0, BLACK, 9, FALSE, TA_CENTER, "N4", 1);
	pOut->SetXL(pXL, ")",						m_nSttRow, "AM", "AM", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",					    m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s/(%s*%s)", pOut->GetCellRef("N1"), pOut->GetCellRef("N2"),
		pOut->GetCellRef("N3"), pOut->GetCellRef("N4"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "dReqBoltEA", 1);
	pOut->SetXL(pXL, "개",					    m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "⇒",					    m_nSttRow, "AB", "AB", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, dUseBoltEA,				m_nSttRow, "AC", "AD", 0, RED, 9, FALSE, TA_CENTER, "dUseBoltEA", 0);
	pOut->SetXL(pXL, "개 사용 (",			    m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dUseBoltCol,				m_nSttRow, "AH", "AI", 0, RED, 9, FALSE, TA_CENTER, "dUseBoltCol", 0);
	pOut->SetXL(pXL, "열배치)",					m_nSttRow, "AJ", "AJ", 0);
	sText.Format("=IF(%s>%s,\"N.G\",\"O.K\")", pOut->GetCellRef("dReqBoltEA"), pOut->GetCellRef("dUseBoltEA"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AN", "AO", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "tf =",				    m_nSttRow, "W", "X", 0);
	sText.Format("=%s", pOut->GetCellRef("dtf"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "Usetf", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);

	pOut->SetXL(pXL, "tw =",				    m_nSttRow, "W", "X", 0);
	pOut->SetXL(pXL, dtw,						m_nSttRow, "Y", "AA", 0, RED, 9, FALSE, TA_CENTER, "Usetw", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);

	pOut->SetXL(pXL, "L =",						m_nSttRow, "W", "X", 0);
	sText.Format("=%s", pOut->GetCellRef("dL"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "UseL", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);

	pOut->SetXL(pXL, "L1 =",				    m_nSttRow, "W", "X", 0);	
	pOut->SetXL(pXL, dL1,						m_nSttRow, "Y", "AA", 0, RED, 9, FALSE, TA_CENTER, "UseL1", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);
	
	m_nSttRow++;
	
	pOut->SetXL(pXL, "A1 =",				    m_nSttRow, "W", "X", 0);
	sText.Format("=%s-1", pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "NA0", 0);
	pOut->SetXL(pXL, "@",						m_nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dA1,						m_nSttRow, "AA", "AC", 0, RED, 9, FALSE, TA_CENTER, "NA1", 0);
	pOut->SetXL(pXL, "A2 =",				    m_nSttRow, "AE", "AF", 0);
	pOut->SetXL(pXL, dA2,						m_nSttRow, "AG", "AI", 0, RED, 9, FALSE, TA_CENTER, "NA2", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AJ", "AK", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "X", "X", 0);
	sText.Format("=%s*%s", pOut->GetCellRef("NA0"), pOut->GetCellRef("NA1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "UseA1", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 0);
	pOut->SetXL(pXL, "A3 =",				    m_nSttRow, "AE", "AF", 0);
	pOut->SetXL(pXL, dA3,						m_nSttRow, "AG", "AI", 0, RED, 9, FALSE, TA_CENTER, "NA3", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AJ", "AK", 1);
	
	SetLines(-3);
	pOut->SetXL(pXL, "A =",						m_nSttRow, "W", "X", 0);
	sText.Format("=(%s+%s)*2+%s", pOut->GetCellRef("NA2"), pOut->GetCellRef("UseA1"),
		pOut->GetCellRef("NA3"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "UseA", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AB", "AC", 1);
	SetLines(2);

	SetLines(5);
	pOut->SetXL(pXL, "ts =",				m_nSttRow, "AD", "AF", 0, BLUE, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dts,					m_nSttRow, "AG", "AH", 0, RED, 9, FALSE, TA_CENTER, "dts", 0);
	pOut->SetXL(pXL, "mm 사용",				m_nSttRow, "AI", "AK", 1, BLUE, 9, FALSE, TA_CENTER);	

	pOut->SetXL(pXL, "연결판",				m_nSttRow, "C", "C", 1);	
	pOut->SetXL(pXL, nTopConEA,				m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "nTopConEA", 0);
	pOut->SetXL(pXL, "-",					m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "PL.",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=1,%s,%s)", pOut->GetCellRef("nTopConEA"),
		pOut->GetCellRef("UseL"), pOut->GetCellRef("UseL1"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ConO2", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dts"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "ConO3", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseA"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER, "ConO4", 0);	
	pOut->SetXL(pXL, nBottomConEA,			m_nSttRow, "D", "D", 0, RED,   9, FALSE, TA_CENTER, "nBottomConEA", 0);
	pOut->SetXL(pXL, "-",					m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "PL.",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=1,%s,%s)", pOut->GetCellRef("nBottomConEA"),
		pOut->GetCellRef("UseL"), pOut->GetCellRef("UseL1"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ConT2", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dts"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "ConT3", 0);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseA"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "ConT4", 0);
	//밑줄 쫙....		
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "D", "P", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "Σ As =",				 m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s*%s*%s+%s*%s*%s)",
		pOut->GetCellRef("nTopConEA"), pOut->GetCellRef("ConO2"), pOut->GetCellRef("ConO3"),
		pOut->GetCellRef("nBottomConEA"), pOut->GetCellRef("ConT2"), pOut->GetCellRef("ConT3"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "As1", 1);
	pOut->SetXL(pXL, "mm2",					m_nSttRow, "N", "O", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Asn =",				m_nSttRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("As1"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER, "As2", 1);
	pOut->SetXL(pXL, "-",					m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dts"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "As3", 0);	
	pOut->SetXL(pXL, "×",					 m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<27,%s+3,%s+4)", pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "As4", 0);	
	pOut->SetXL(pXL, "×",					 m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s*2", pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "As5", 0);	
	pOut->SetXL(pXL, "EA =",				m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s-%s*%s*%s", pOut->GetCellRef("As2"), 
		pOut->GetCellRef("As3"), pOut->GetCellRef("As4"), pOut->GetCellRef("As5"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER, "As4", 1);	
	pOut->SetXL(pXL, "mm2",					m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,\"＞\",\"＜\")", pOut->GetCellRef("As4"), pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AA", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "Af =",				m_nSttRow, "AC", "AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "mm2",					m_nSttRow, "AH", "AI", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,\"＞\",\"＜\")", pOut->GetCellRef("As4"), pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AK", "AL", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);		

	SetLines(-9);
	pOut->SetXL(pXL, "SPLICE 두께산정",		m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "Asn =",				m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",					m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "{(",					m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=1,\"{( \"&%s&\" - \"&IF(%s<27,%s+3,%s+4)&\" × \"&%s/%s/2&\" ) × 2EA + ( \"&%s&\" - \"&IF(%s<27,%s+3,%s+4)&\" × \"&%s/%s&\" )}\",\"{( \"&%s&\" - \"&IF(%s<27,%s+3,%s+4)&\" × \"&%s/%s/2&\" ) × 4EA }\")",
		pOut->GetCellRef("nBottomConEA"), pOut->GetCellRef("UseL1"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"),
		pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"), pOut->GetCellRef("UseL"),	pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), 
		pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"),
		pOut->GetCellRef("UseL1"), pOut->GetCellRef("dBoltMSize"),	pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_LEFT);	
	pOut->SetXL(pXL, "=",					m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=IF(%s=1,(%s-IF(%s<27,%s+3,%s+4)*%s/%s/2)*2+(%s-IF(%s<27,%s+3,%s+4)*%s/%s),(%s-IF(%s<27,%s+3,%s+4)*%s/%s/2)*4)", 
		pOut->GetCellRef("nBottomConEA"), pOut->GetCellRef("UseL1"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"),
		pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"), pOut->GetCellRef("UseL"),	pOut->GetCellRef("dBoltMSize"),  pOut->GetCellRef("dBoltMSize"),
		pOut->GetCellRef("dBoltMSize"),pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"),	pOut->GetCellRef("UseL1"),
		pOut->GetCellRef("dBoltMSize"),	 pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"),pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "Asn8", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",					m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "≥",					 m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Af",					m_nSttRow, "O", "O", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "∴",					m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",					m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "≥",					 m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Af",					m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "/",					m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("Asn8"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "=",					m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "Asn9", 1);
	pOut->SetXL(pXL, "/",					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("Asn8"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "Asn10", 1);
	pOut->SetXL(pXL, "=",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("Asn9"), pOut->GetCellRef("Asn10"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "X", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "Asn11", 1);
	pOut->SetXL(pXL, "mm",					m_nSttRow, "AA", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "⇒",					m_nSttRow, "AC", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("=IF(%s>%s,\"N.G\",\"O.K\")", pOut->GetCellRef("Asn11"), pOut->GetCellRef("dts"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AN", "AO", 1, BLACK, 9, FALSE, TA_CENTER);

	SetLines(5);
	
	pOut->SetXL(pXL, "⊙ 볼트의 응력검토",	m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "  · 직응력",			m_nSttRow, "B", "B", 1);
	pOut->SetLineBoxText(pXL, "ρp =",		m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P",		m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("dtf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "P1", 0);
	pOut->SetXL(pXL, "×",					m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=%s", pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "P2", 0);
	pOut->SetXL(pXL, "×",					m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "I", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("=%s", pOut->GetCellRef("dFl'"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "P3", 1);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "P", "S", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);
	//SetLines(1);
	pOut->SetXL(pXL, "n",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dUseBoltEA"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "P4", 0);
	pOut->SetXL(pXL, "EA",					m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-1);
	sText.Format("=%s*%s*%s/%s", pOut->GetCellRef("P1"),
		pOut->GetCellRef("P2"), pOut->GetCellRef("P3"), pOut->GetCellRef("P4"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "U", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "dPP", 1, FALSE);	
	pOut->SetLineBoxText(pXL, "N/EA",		m_nSttRow, "Z", "AB", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=IF(%s>2*%s,\" ＞ 2·ρa    N.G\",\"＜  2·ρa    O.K\")",
		pOut->GetCellRef("dPP"), pOut->GetCellRef("dPa"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AC", "AK", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(3);

	pOut->SetXL(pXL, "  · 휨전단 응력",			 m_nSttRow, "B", "B", 1);
	pOut->SetLineBoxText(pXL, "ρh =",		 m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "S Q",		m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "×",			 m_nSttRow, "H", "I", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "p",		m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(1);
	pOut->SetXL(pXL, "I",					m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n",					m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(2);

	pOut->SetXL(pXL, "S : 작용전단력",		m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, dApplySF,				m_nSttRow, "J", "O", 0, RED, 9, FALSE, TA_CENTER, "dApplySF", 3);
	pOut->SetXL(pXL, "kN",					m_nSttRow, "P", "P", 0);
	pOut->SetXL(pXL, "( 가로보 계산 참조)",	m_nSttRow, "S", "S", 1);
	pOut->SetXL(pXL, "Q : 중립축에대한 접합선외측의 단면1차모멘트",	m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "Q =",					m_nSttRow, "D", "E", 0);
	sText.Format("=%s", pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "Q1", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dtf"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Q2", 1);
	pOut->SetXL(pXL, "× (",				 m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dDanMom1,				m_nSttRow, "N", "Q", 0, RED, 9, FALSE, TA_CENTER, "dDanMom1", 1);
	pOut->SetXL(pXL, "-",					m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/2", pOut->GetCellRef("Q2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Q3", 1);
	pOut->SetXL(pXL, ") =",					m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s*(%s-%s)", pOut->GetCellRef("Q1"), pOut->GetCellRef("Q2"),
		pOut->GetCellRef("dDanMom1"), pOut->GetCellRef("Q3"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "X", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "dQ", 1);
	pOut->SetXL(pXL, "mm³",				 m_nSttRow, "AB", "AC",1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "I : 중립축에대한 총단면의 단면2차모멘트",	m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "I =",					m_nSttRow, "S", "T", 0);
	pOut->SetXL(pXL, dDanMom2,				m_nSttRow, "U", "Z", 0, RED, 9, FALSE, TA_CENTER, "dDanMom2", 1);
	pOut->SetXL(pXL, "mm⁴ ( 가로보 계산 참조)", m_nSttRow, "AA", "AA", 1);
	pOut->SetXL(pXL, "p : 접합선 방향의 볼트의 평균 핏치(mm) = 플랜지 전폭 / 횡방향 볼트수", 
											m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "=",					m_nSttRow, "AD", "AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AE", "AG", 0, BLACK, 9, FALSE, TA_CENTER, "P1", 1);
	pOut->SetXL(pXL, "/",					m_nSttRow, "AH", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("dUseBoltEA"), pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AI", "AJ", 0, BLACK, 9, FALSE, TA_CENTER, "P2", 1);
	pOut->SetXL(pXL, "=",					m_nSttRow, "AK", "AK", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("P1"), pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AL", "AN", 0, BLACK, 9, FALSE, TA_CENTER, "dP", 1);
	pOut->SetXL(pXL, "mm",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "n : 접합선직각의 볼트의 갯수",	m_nSttRow, "C", "C", 1);

	SetLines(-9);
	sText.Format("=%s*1000", pOut->GetCellRef("dApplySF"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "M", "P", 0, BLACK, 9, FALSE, TA_CENTER, "Ph1", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dQ"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Ph2", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dP"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "Ph3", 1);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "M", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetLineBoxText(pXL, "=",			m_nSttRow, "Z", "Z", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(1);
	sText.Format("=%s", pOut->GetCellRef("dDanMom2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "N", "S", 0, BLACK, 9, FALSE, TA_CENTER, "Ph4", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dUseBoltCol"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "Ph5", 1);
	pOut->SetXL(pXL, "EA",					m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-1);
	sText.Format("=%s*%s*%s/%s/%s", pOut->GetCellRef("Ph1"), pOut->GetCellRef("Ph2"),
		pOut->GetCellRef("Ph3"),pOut->GetCellRef("Ph4"),pOut->GetCellRef("Ph5"));
	pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AA", "AG", 2, BLACK, 9, FALSE, TA_CENTER, "dPh", 1, FALSE);
	pOut->SetLineBoxText(pXL, "N/EA",		m_nSttRow, "AH", "AI", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	SetLines(10);
	pOut->SetXL(pXL, "  · 합성응력의 검토", m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "ρ =  √(ρp²＋ρh²)  = ",m_nSttRow, "C", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=SQRT(%s^2+%s^2)", pOut->GetCellRef("dPP"), pOut->GetCellRef("dPh"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "K", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "ppph", 1);
	sText.Format("=IF(%s>2*%s,\"N/EA  ＞ 2·ρa    N.G\",\"N/EA ＜  2·ρa    O.K\")", 
		pOut->GetCellRef("ppph"), pOut->GetCellRef("dPa"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "R", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);

	pOut->SetXL(pXL, "⊙ 모재의 응력검토",	m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "플랜지의 총단면적",	m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "As =",				m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s&\" × \"&%s&\" =\"", pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "T", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "Y", "AB", 0, BLACK, 9, FALSE, TA_CENTER, "dAs", 1);
	pOut->SetXL(pXL, "mm²",				 m_nSttRow, "AC", "AD", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "볼트 구멍을 공제한 플랜지의 순단면적  Asn =",	m_nSttRow, "C", "S", 0);	
	sText.Format("=%s&\" ×( \"&%s&\" - \"&IF(%s<27,%s+3,%s+4)&\" ×\"&%s&\" ) =\"", 
		pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"), pOut->GetCellRef("dBoltMSize"),
		pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "T", "AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*(%s-IF(%s<27,%s+3,%s+4)*%s)", pOut->GetCellRef("dtf"), pOut->GetCellRef("UseL"), 
		pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("dBoltMSize"), pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "AE", "AH", 0, BLACK, 9, FALSE, TA_CENTER, "dAsn", 1);
	pOut->SetXL(pXL, "mm²",				 m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "모재의 응력",m_nSttRow, "C", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "fl =",		m_nSttRow, "H", "J", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("dAs"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "fu1", 1);
	pOut->SetXL(pXL, "×",					 m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dFl'"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "fu2", 1);
	pOut->SetUnderLineText(pXL,"",			m_nSttRow, "K", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);	
	SetLines(1);
	sText.Format("=%s", pOut->GetCellRef("dAsn"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "M", "P", 0, BLACK, 9, FALSE, TA_CENTER, "fu3", 1);
	SetLines(-1);
	pOut->SetLineBoxText(pXL, "=",		    m_nSttRow, "T", "T", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	sText.Format("=%s*%s/%s", pOut->GetCellRef("fu1"), pOut->GetCellRef("fu2"), pOut->GetCellRef("fu3"));
	pOut->SetLineBoxText(pXL, sText,	    m_nSttRow, "U", "X", 2, BLACK, 9, FALSE, TA_CENTER, "dmFu", 3, FALSE);
	pOut->SetLineBoxText(pXL, "MPa",	    m_nSttRow, "Y", "AA", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	sText.Format("=IF(%s<=%s,\"≤\",\"＞\")", pOut->GetCellRef("dmFu"), pOut->GetCellRef("dFa"));
	pOut->SetLineBoxText(pXL, sText,	    m_nSttRow, "AC", "AC", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	pOut->SetLineBoxText(pXL, "fa",			m_nSttRow, "AE", "AF", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("dmFu"), pOut->GetCellRef("dFa"));
	pOut->SetLineBoxText(pXL, sText,	    m_nSttRow, "AI", "AJ", 2, BLACK, 9, FALSE, TA_CENTER, "", 1, FALSE);
	SetLines(3);
}
void CXLDesignCrossBeam::ConnectExistWeb(CPlateBasicIndex *pBx, long LRType)//가로보연결 - 기존방식 복부 플랜지
{
	CAPlateCalcStd		*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CDesignCrossBeamJoint	Joint(pCalcStd, pBx);	
	CDesignWebJoint		*pWeb		= NULL;
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CXLControl			*pXL		= m_pXL;	

	CSteel				*pSteelCtl		= m_pAPlateOutCalcStd->GetDataManage()->GetSteel();

	if (LRType == 1)//왼쪽이면
	{
		pWeb = Joint.m_pWebLeft;
	}
	else
	{
		pWeb = Joint.m_pWebRight;
	}

	CString sText		= _T("");
	CString strLeft		= pWeb->m_strtPlate.m_strMaterial;
	
	double dFl			= pWeb->m_strtPlate.m_dStress;
	double dsE			= pWeb->m_strtPlate.m_dsE;
	double dsF			= pWeb->m_strtPlate.m_dsF;
	double dsT			= pWeb->m_strtPlate.m_dTop;
	double dA			= pWeb->m_strtPlate.m_dA;
	double dA2			= pWeb->m_strtPlate.m_dA2;

	double dtw			= pWeb->m_strtPlate.m_dTw;
	long   nB1 			= pWeb->m_strtPlate.m_nB1;
	double dB1			= pWeb->m_strtPlate.m_dB1;
	double dB2			= pWeb->m_strtPlate.m_dB2;
	double dB			= pWeb->m_strtPlate.m_dB;
	double dPa			= pWeb->m_strtPlate.m_dAllowBolt;//볼트의 허용응력
	double dDiaBolt		= pWeb->m_strtPlate.m_dDiaBolt;
	double dTsUse		= pWeb->m_strtPlate.m_dTsUse;
	double dMoment		= pWeb->m_strtPlate.m_dMoment;
	double dI			= pWeb->m_strtPlate.m_dI;
	double dsT1			= pWeb->m_strtPlate.m_dsT1;
	double dsT2			= pWeb->m_strtPlate.m_dsT2;
	double dApplySF		= 0;

	CPlateCrossBeam *pCr	= pBx->GetSection()->GetCrossBeam();

	//////////////////////////////////////////////////////////////////////////	
	CDesignCrossBeam	CrossBeam(pCalcStd, pBx);
	CDesignEndCrossBeam		*pCrEnd	= NULL;
	CDesignCenterCrossBeam	*pCrCen	= NULL;
	BOOL bEnd		= pBx->GetSection()->IsenType(SECTION_A) ? TRUE : FALSE;
	double dPreS=0, dPosS=0, dLiveMaxS=0, dLiveMinS=0, dSetMaxS=0, dSetMinS=0;
	if(bEnd)
	{
		pCrEnd = (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
		dPreS			= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad);		// 합성전 고정하중 S
		dPosS			= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad);		// 합성후 고정하중 S
		dLiveMaxS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad);		// 활하중 S - Max
		dLiveMinS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad);		// 활하중 S - Min
		dSetMaxS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement);		// 지점침하 S - Max
		dSetMinS		= tokN(pCrEnd->m_pFrameElementLoad->m_strtShear.m_dMinSettlement);		// 지점침하 S - Min
	}
	else
	{
		pCrCen = (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();
		dPreS			= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad);		// 합성전 고정하중 S
		dPosS			= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad);		// 합성후 고정하중 S
		dLiveMaxS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad);		// 활하중 S - Max
		dLiveMinS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad);		// 활하중 S - Min
		dSetMaxS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement);		// 지점침하 S - Max
		dSetMinS		= tokN(pCrCen->m_pFrameElementLoad->m_strtShear.m_dMinSettlement);		// 지점침하 S - Min
	}
	dApplySF = dPreS+dPosS+max(dLiveMaxS+dSetMaxS, dLiveMinS+dSetMinS);
	
	//////////////////////////////////////////////////////////////////////////	

	if(pCr->m_nLGirSpType==0 && pCr->m_nRGirSpType==0)
		pOut->SetXL(pXL, "(1) Web의 연결",			m_nSttRow, "B", "B", 1, BLACK, 9);
	else
		pOut->SetXL(pXL, "(3) Web의 연결",			m_nSttRow, "B", "B", 1, BLACK, 9);	

	pOut->InsertImage(pXL,"CROSSBEAM연결_복부", m_nSttRow,"E");

	pOut->SetXL(pXL, dA,						m_nSttRow, "F", "I", 1, RED,   9, FALSE, TA_CENTER, "dA",0);	
	pOut->SetXL(pXL, dA2,						m_nSttRow, "D", "E", 0, RED,   9, FALSE, TA_CENTER, "dA2",0);
	sText.Format("=%s-%s*2", pOut->GetCellRef("dA"), pOut->GetCellRef("dA2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "",0);
	sText.Format("=%s", pOut->GetCellRef("dA2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "",0);

	pOut->SetXL(pXL, dsT,						m_nSttRow, "N", "O", 2, RED, 9, FALSE, TA_CENTER, "dsT",0);	
	pOut->SetXL(pXL, dsF,						m_nSttRow, "M", "N", 1, RED, 9, FALSE, TA_CENTER, "dsF",0);	
	pOut->SetXL(pXL, "@",						m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dsE,						m_nSttRow, "M", "N", 1, RED, 9, FALSE, TA_CENTER, "dsE",0);
	//pOut->SetXL(pXL, "=",						m_nSttRow, "N", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dsT"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	SetLines(-2);
	sText.Format("=%s*%s", pOut->GetCellRef("dsE"), pOut->GetCellRef("dsF"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "dMid", 0);
	sText.Format("=%s+%s*2", pOut->GetCellRef("dMid"), pOut->GetCellRef("dsT"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "dTot", 0);
	SetLines(-2);
	pOut->SetXL(pXL, "ho",						m_nSttRow, "AH", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/2", pOut->GetCellRef("dTot"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AH", "AJ", 0, BLACK, 9, FALSE, TA_CENTER, "dHo", 0);
	
	SetLines(7);
	
	pOut->SetXL(pXL, "tw =",					m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dtw,						m_nSttRow, "F", "G", 0, RED, 9, FALSE, TA_CENTER, "dtw",0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "H", "I", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "B1 =",					m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, nB1,						m_nSttRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "nB1",0);
	pOut->SetXL(pXL, "@",						m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dB1,						m_nSttRow, "H", "I", 0, RED, 9, FALSE, TA_CENTER, "dB1",0);
	pOut->SetXL(pXL, "=",						m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("nB1"), pOut->GetCellRef("dB1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "dUseB1",0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "B2 =",					m_nSttRow, "Q", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dB2,						m_nSttRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "dB2",0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "b =",						m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dB,						m_nSttRow, "AA", "AC", 0, RED, 9, FALSE, TA_CENTER, "dB",0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AD", "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "B =",						m_nSttRow, "AG", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s+%s)*2+%s", 
		pOut->GetCellRef("dUseB1"), pOut->GetCellRef("dB"), pOut->GetCellRef("dB2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AI", "AK", 0, BLACK, 9, FALSE, TA_CENTER, "dUseB",0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);
	
	pOut->SetXL(pXL, "작용응력 : fl =",			m_nSttRow, "C", "H", 0, BLACK, 9);	
	pOut->SetXL(pXL, dFl,						m_nSttRow, "K", "N", 0, RED,   9, FALSE, TA_CENTER, "dFl");
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, " ( 가로보 계산 참조)",	m_nSttRow, "S", "S", 1, BLACK, 9);
	pOut->SetXL(pXL, "모재의 허용응력 :",		m_nSttRow, "C", "H", 0, BLACK, 9);	
	pOut->SetXL(pXL, "fa =",					m_nSttRow, "J", "J", 0, BLACK, 9);	
	
	//sText.Format("");
	//pOut->SetXL(pXL, 210,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "dFa", 3);	
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "(",						m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, strLeft,					m_nSttRow, "V", "Y", 0, RED,   9, FALSE, TA_CENTER, "MatNM");
	pOut->SetXL(pXL, "사용 )",					m_nSttRow, "Z", "AA", 0, BLACK, 9, FALSE, TA_CENTER);

// 	CString strMaterial = strLeft;
// 	if(strLeft.Find("SM400") > -1)	strMaterial.Format("%s%s",strLeft, "B");
	CCalcGeneral	CalcGen(m_pAPlateOutCalcStd->GetDataManage());
	CString	szLeft, szRight;
	if(pBx->IsJijum())	CalcGen.GetMaterial(pBx, CP_W, szLeft, szRight);
	else				CalcGen.GetMaterial(pBx, CG_W, szLeft, szRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft+szRight)];
	CHGAllowStress		AllowTension(pSteel);
	CHGAllowStress		AllowShear(pSteel);

	double dTension = AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dtw);
	pOut->SetXL(pXL, dTension,					m_nSttRow, "N", "Q", 1, RED, 9, FALSE, TA_CENTER, "dFa", 0);

	pOut->SetXL(pXL, "0.75 fa =",				m_nSttRow, "J", "J", 0, BLACK, 9);	
	sText.Format("=0.75*%s", pOut->GetCellRef("dFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "ReFa", 3);	
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=IF(%s>%s,\">\",\"<\")", pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFl"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "fl",						m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "이므로",					m_nSttRow, "AA", "AA", 1, BLACK, 9);

	pOut->SetXL(pXL, "fl' =",					m_nSttRow, "K", "K", 0, BLACK, 9);
	sText.Format("=IF(%s>%s,%s,%s)", pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFl"),
		pOut->GetCellRef("ReFa"), pOut->GetCellRef("dFl"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "dFl'", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "를 사용하여 설계",		m_nSttRow, "U", "U", 0, BLACK, 9);	
	pOut->SetXL(pXL, "( ☞ 도.설 3.5.1.1 )",	m_nSttRow, "AC", "AC", 1, BLACK, 9);	
	
	pOut->SetXL(pXL, "f1=",						m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "fl'",						m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dFl'"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER, "dF1", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f2=",						m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f1·(ho -",				 m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+%s/2", pOut->GetCellRef("dsT"), pOut->GetCellRef("dsF"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "dF2ho", 1);
	pOut->SetXL(pXL, ")/ho",					m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*(%s-%s)/%s", pOut->GetCellRef("dF1"), 
		pOut->GetCellRef("dHo"), pOut->GetCellRef("dF2ho"),pOut->GetCellRef("dHo"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "dF2", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f3=",						m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f1·(ho -",				 m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+%s", pOut->GetCellRef("dF2ho"), pOut->GetCellRef("dsF"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "dF3ho", 1);
	pOut->SetXL(pXL, ")/ho",					m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*(%s-%s)/%s", pOut->GetCellRef("dF1"), 
		pOut->GetCellRef("dHo"), pOut->GetCellRef("dF3ho"),pOut->GetCellRef("dHo"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "dF3", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f4=",						m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "f1·(ho -",				 m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+%s", pOut->GetCellRef("dF3ho"), pOut->GetCellRef("dsF"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "dF4ho", 1);
	pOut->SetXL(pXL, ")/ho",					m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*(%s-%s)/%s", pOut->GetCellRef("dF1"), 
		pOut->GetCellRef("dHo"), pOut->GetCellRef("dF4ho"),pOut->GetCellRef("dHo"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "dF4", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);
	
	SetLines(-20);
	sText.Format("=%s", pOut->GetCellRef("dF1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AB", "AE", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("=%s", pOut->GetCellRef("dF2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "U", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);	
	sText.Format("=%s", pOut->GetCellRef("dF4"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "U", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("=%s", pOut->GetCellRef("dF3"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AA", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	
	SetLines(19);
	pOut->SetXL(pXL, "휨모멘트에 의한 보울트의 작용력 ( 2면마찰 )",		m_nSttRow, "C", "C", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "제1열",					m_nSttRow, "D", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ρm1 =  (",				 m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dF1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "pm1", 3);
	pOut->SetXL(pXL, "+",						m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dF2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "P", "S", 0, BLACK, 9, FALSE, TA_CENTER, "pm2", 3);
	pOut->SetXL(pXL, ")",						m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "× 1/2 ×",				  m_nSttRow, "U", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dtw"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "pm3", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "AA", "AA", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dF2ho"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AB", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "pm4", 1);
	pOut->SetXL(pXL, "/",						m_nSttRow, "AE", "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+1", pOut->GetCellRef("nB1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AF", "AG", 1, BLACK, 9, FALSE, TA_CENTER, "pm5", 0);
	pOut->SetXL(pXL, "=",						m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s+%s)*0.5*%s*%s/%s", pOut->GetCellRef("pm1"),pOut->GetCellRef("pm2"),
		pOut->GetCellRef("pm3"), pOut->GetCellRef("pm4"), pOut->GetCellRef("pm5"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "J", "N", 0, BLACK, 9, FALSE, TA_CENTER, "pm6", 3);
	pOut->SetXL(pXL, "N/EA",					m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "2ρa =",					 m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%.0f*2", dPa);
	pOut->SetXL(pXL, sText,						m_nSttRow, "X", "Z", 0, RED,   9, FALSE, TA_CENTER, "dPaX2", 0);
	sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("pm6"), pOut->GetCellRef("dPaX2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "N/EA",					m_nSttRow, "AA", "AC", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s,\"O.K.\",\"N.G\")", pOut->GetCellRef("pm6"), pOut->GetCellRef("dPaX2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AF", "AJ", 1, BLUE,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단력에 의한 보울트의 작용력 ( 2면전단 )",		m_nSttRow, "C", "C", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "ρs =",					m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	if(pOut->GetCellRef("dApplySF").IsEmpty())	sText.Format("%g", dApplySF);
	else										sText.Format("=%s*1000", pOut->GetCellRef("dApplySF"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "ps1", 3);
	
	pOut->SetXL(pXL, "/",						m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s+1)*(%s+1)", pOut->GetCellRef("dsE"), pOut->GetCellRef("nB1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "K", "L", 0, BLACK,   9, FALSE, TA_CENTER, "ps2", 0);
	pOut->SetXL(pXL, "EA",						m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("ps1"), pOut->GetCellRef("ps2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "P", "T", 0, BLACK,   9, FALSE, TA_CENTER, "ps3", 0);
	pOut->SetXL(pXL, "N/EA",					m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "2ρa =",					 m_nSttRow, "Z", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dPaX2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AC", "AE", 0, BLACK,   9, FALSE, TA_CENTER, "ps4", 0);
	sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("ps3"), pOut->GetCellRef("ps4"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetXL(pXL, "N/EA",					m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s,\"O.K.\",\"N.G\")", pOut->GetCellRef("ps3"), pOut->GetCellRef("ps4"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AJ", "AK", 1, BLUE,   9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "합성응력의 검토",			m_nSttRow, "C", "C", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "ρ =",					 m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "√(ρm²＋ρs²)",			 m_nSttRow, "F", "L", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",						m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=SQRT(%s^2+%s^2)", pOut->GetCellRef("pm6"), pOut->GetCellRef("ps3"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "F", "I", 0, BLACK,   9, FALSE, TA_CENTER, "dp", 3);
	pOut->SetXL(pXL, "N/EA",					m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "2ρa =",					 m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dPaX2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "S", "U", 0, BLACK,   9, FALSE, TA_CENTER, "dp2", 0);
	sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("dp"), pOut->GetCellRef("dp2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);pOut->SetXL(pXL, "N/EA",					m_nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s,\"O.K.\",\"N.G\")", pOut->GetCellRef("dp"), pOut->GetCellRef("dp2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Z", "AA", 1, BLUE,   9, FALSE, TA_CENTER);
	
	pOut->SetXL(pXL, "SPLICE의 응력검토",		m_nSttRow, "C", "C", 1, BLACK, 9, FALSE, TA_LEFT);
	
	pOut->InsertImage(pXL,"CROSSBEAM연결_응력검토", m_nSttRow,"C");	

	pOut->SetXL(pXL, dsT1,						m_nSttRow, "O", "P", 1, RED,   9, FALSE, TA_CENTER, "dsT1", 0);
	pOut->SetXL(pXL, dsT2,						m_nSttRow, "Q", "R", 0, RED,   9, FALSE, TA_CENTER, "dsT2", 0);
	
	pOut->SetXL(pXL, "H = ",					m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dTot"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AA", "AC", 0, BLACK,   9, FALSE, TA_CENTER, "dH", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "h = ",					m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s-%s*2", pOut->GetCellRef("dH"), pOut->GetCellRef("dsT1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AA", "AC", 0, BLACK,   9, FALSE, TA_CENTER, "dh", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("=%s", pOut->GetCellRef("dsF"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "O", "P", 0, BLACK,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "Ysu = ",					m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/2-%s", pOut->GetCellRef("dH"), pOut->GetCellRef("dsT1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AA", "AC", 0, BLACK,   9, FALSE, TA_CENTER, "dYsu", 0);
	pOut->SetXL(pXL, "mm",						m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "@",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dMid"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Q", "S", 0, BLACK,   9, FALSE, TA_CENTER, "", 0);
	sText.Format("=%s", pOut->GetCellRef("dTot"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "T", "V", 1, BLACK,   9, FALSE, TA_CENTER, "", 0);
	
	sText.Format("=%s", pOut->GetCellRef("dsE"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "O", "P", 1, BLACK,   9, FALSE, TA_CENTER, "", 0);
	//pOut->SetXL(pXL, "=",						m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);
	sText.Format("=%s", pOut->GetCellRef("dsT2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Q", "R", 1, BLACK,   9, FALSE, TA_CENTER, "", 0);
	sText.Format("=%s", pOut->GetCellRef("dsT1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "O", "P", 1, BLACK,   9, FALSE, TA_CENTER, "", 0);

	SetLines(1);

	pOut->SetXL(pXL, "Asn = ",					m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "{",						m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dh"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "Asn1", 0);
	pOut->SetXL(pXL, "-",						m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%.0f<27,%.0f+3,%.0f+4", dDiaBolt, dDiaBolt, dDiaBolt);
	pOut->SetXL(pXL, sText,						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "Asn2", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+1", pOut->GetCellRef("dsE"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "Asn3", 0);
	pOut->SetXL(pXL, "}",						m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2,							m_nSttRow, "S", "S", 1, BLACK, 9, FALSE, TA_CENTER, "Asn4", 0);
	pOut->SetXL(pXL, "=",						m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s-%s*%s)*%s", pOut->GetCellRef("Asn1"),
		pOut->GetCellRef("Asn2"), pOut->GetCellRef("Asn3"), pOut->GetCellRef("Asn4"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "dAsn", 1);
	pOut->SetXL(pXL, "ts",						m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "≥",						 m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Aw",						m_nSttRow, "M", "M", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "∴",						m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ts",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "≥",						 m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Aw/",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dAsn"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s/%s", pOut->GetCellRef("dtw"), pOut->GetCellRef("dTot"), pOut->GetCellRef("dAsn"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "Asn5", 1);
	pOut->SetXL(pXL, "mm ⇒  ts =",				m_nSttRow, "S", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dTsUse,					m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "dTsUse", 0);
	pOut->SetXL(pXL, "mm 사용",					m_nSttRow, "AA", "AD", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s, \"N.G\", \"O.K\")", pOut->GetCellRef("Asn5"), pOut->GetCellRef("dTsUse"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AF", "AG", 1, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetXL(pXL, "휨모멘트에 의한 검토",	m_nSttRow, "C", "C", 1, BLACK, 9, FALSE, TA_LEFT);	
	pOut->SetXL(pXL, "작용 모멘트 :",			m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_LEFT);	
	pOut->SetXL(pXL, "M =",						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dMoment,					m_nSttRow, "L", "Q", 0, RED,   9, FALSE, TA_CENTER, "dMoment", 1);
	pOut->SetXL(pXL, "kN·m  ( 가로보 계산참조 )",m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "세로보 전체의 단면2차 모멘트 :",		m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_LEFT);	
	pOut->SetXL(pXL, "I =",						m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dI,						m_nSttRow, "V", "AA", 0, RED, 9, FALSE, TA_CENTER, "dI", 1);
	pOut->SetXL(pXL, "mm4",						m_nSttRow, "AB", "AC", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "복부만의 단면2차 모멘트",		m_nSttRow, "D", "D", 1, BLACK, 9, FALSE, TA_LEFT);	
	pOut->SetXL(pXL, "Iw =",					m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dtw"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "Iw1", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(",						m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dH"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "Iw2", 0);
	pOut->SetXL(pXL, ")³",						 m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "/",						m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 12,						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "=",						m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s^3/12", pOut->GetCellRef("Iw1"), pOut->GetCellRef("Iw2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "R", "W", 0, BLACK, 9, FALSE, TA_CENTER, "dIw", 1);
	pOut->SetXL(pXL, "mm4",						m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "따라서, 복부가 담당하는 휨모멘트는",		m_nSttRow, "D", "D", 1, BLACK, 9, FALSE, TA_LEFT);	
	pOut->SetXL(pXL, "Mw = M·Iw / I =",		m_nSttRow, "E", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dMoment"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "L", "P", 0, BLACK, 9, FALSE, TA_CENTER, "Mw1", 1);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dIw"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "R", "W", 0, BLACK, 9, FALSE, TA_CENTER, "Mw2", 1);
	pOut->SetXL(pXL, "/",						m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dI"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AD", 0, BLACK, 9, FALSE, TA_CENTER, "Mw3", 1);
	pOut->SetXL(pXL, "=",						m_nSttRow, "AE", "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s/%s", pOut->GetCellRef("Mw1"), pOut->GetCellRef("Mw2"), pOut->GetCellRef("Mw3"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AF", "AL", 0, BLACK, 9, FALSE, TA_CENTER, "dMw", 1);
	pOut->SetXL(pXL, "kN·m",					 m_nSttRow, "AM", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "SPLICE의 단면2차 모멘트",		m_nSttRow, "D", "D", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "Is =",					m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 2,							m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=%s", pOut->GetCellRef("dTsUse"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Is1", 0);
	pOut->SetXL(pXL, "(",						m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dh"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "Is2", 0);
	pOut->SetXL(pXL, ")³",						 m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "/",						m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 12,						m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "=",						m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=2*(%s*%s^3/12)", pOut->GetCellRef("Is1"), pOut->GetCellRef("Is2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "U", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "dIs", 1);
	pOut->SetXL(pXL, "mm4",						m_nSttRow, "AA", "AB", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "SPLICE의 최대응력",		m_nSttRow, "D", "D", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "fmax = Mw·Ysu / Is =",	 m_nSttRow, "D", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*10^6", pOut->GetCellRef("dMw"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "M", "S", 0, BLACK, 9, FALSE, TA_CENTER, "fmax1", 1);	
	pOut->SetXL(pXL, "×",						 m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);		
	sText.Format("=%s", pOut->GetCellRef("dYsu"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER, "fmax2", 1);
	pOut->SetXL(pXL, "/",						m_nSttRow, "X", "X", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dIs"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Y", "AD", 1, BLACK, 9, FALSE, TA_CENTER, "fmax3", 0);
	pOut->SetXL(pXL, "=",						m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s/%s", pOut->GetCellRef("fmax1"), pOut->GetCellRef("fmax2"), pOut->GetCellRef("fmax3"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "H", "N", 0, BLACK, 9, FALSE, TA_CENTER, "dfmax", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "<",						m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ρa =",					 m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dFa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_CENTER, "dPa", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")", pOut->GetCellRef("dfmax"), pOut->GetCellRef("dPa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AE", "AF", 1, BLUE, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, 2,							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER, "PL1", 0);
	pOut->SetXL(pXL, "-",						m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "PL.",						m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dh"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "PL2", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("dTsUse"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "PL3", 0);
	pOut->SetXL(pXL, "×",						 m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+100", pOut->GetCellRef("dB2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "P", "Q", 1, BLACK, 9, FALSE, TA_CENTER, "PL4", 0);
	pOut->SetXL(pXL, "Σ As =",					 m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s*%s*%s)", pOut->GetCellRef("PL1"),pOut->GetCellRef("PL2"),pOut->GetCellRef("PL3"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "dAs", 0);
	pOut->SetXL(pXL, "mm2",						m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, "Aw =",					 m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("dtw"),pOut->GetCellRef("dTot"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER, "dAw", 0);
	pOut->SetXL(pXL, "mm2",						m_nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=IF(%s>%s,\"＞\",\"＜\")", pOut->GetCellRef("dAs"), pOut->GetCellRef("dAw"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("dAs"), pOut->GetCellRef("dAw"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "AA", "AB", 1, BLUE, 9, FALSE, TA_CENTER);

	
	pOut->SetXL(pXL, "전단력에 대한 검토",		m_nSttRow, "D", "D", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "υ = S / Asn =",			 m_nSttRow, "E", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("=%s", pOut->GetCellRef("ps1"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "L", "P", 0, BLACK, 9, FALSE, TA_CENTER, "v1", 0);
	pOut->SetXL(pXL, "/",						m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("dAsn"), pOut->GetCellRef("dTsUse"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "v2", 0);
	pOut->SetXL(pXL, "=",						m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/%s", pOut->GetCellRef("v1"), pOut->GetCellRef("v2"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "V", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "dv", 3);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Z", "AA", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "<",						m_nSttRow, "AD", "AD", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "υa =",					 m_nSttRow, "AF", "AG", 0, BLACK, 9, FALSE, TA_CENTER);

	double dShear = AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, dtw);
	pOut->SetXL(pXL, dShear,					m_nSttRow, "AH", "AK", 0, RED, 9, FALSE, TA_CENTER, "dVa", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "AL", "AN", 2, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "합성응력의 검토",			m_nSttRow, "D", "D", 1, BLACK, 9, FALSE, TA_LEFT);

	pOut->SetLineBoxText(pXL, "√((",			 m_nSttRow, "E", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=%s", pOut->GetCellRef("dfmax"));	
	pOut->SetUnderLineText(pXL, sText,			m_nSttRow, "G", "L", 0, BLACK, 9, FALSE, TA_CENTER, "HL1", 3);
	
	pOut->SetLineBoxText(pXL, ")²",			 m_nSttRow, "M", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "+",				m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "(",				m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	sText.Format("=%s", pOut->GetCellRef("dv"));	
	pOut->SetUnderLineText(pXL, sText,			m_nSttRow, "Q", "V", 0, BLACK, 9, FALSE, TA_CENTER, "HL2", 3);
	pOut->SetLineBoxText(pXL, ")²)",			 m_nSttRow, "W", "X", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",				m_nSttRow, "Y", "Y", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(1);
	sText.Format("=%s", pOut->GetCellRef("dPa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "G", "L", 0, BLACK, 9, FALSE, TA_CENTER, "HL3", 0);
	sText.Format("=%s", pOut->GetCellRef("dVa"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "Q", "V", 0, BLACK, 9, FALSE, TA_CENTER, "HL4", 0);
	SetLines(-1);

	sText.Format("=SQRT((%s/%s)^2+(%s/%s)^2)", pOut->GetCellRef("HL1"), 
		pOut->GetCellRef("HL3"), pOut->GetCellRef("HL2"), pOut->GetCellRef("HL4"));
	pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "Z", "AE", 2, BLACK, 9, FALSE, TA_CENTER, "HLoad", 3, FALSE);
	pOut->SetLineBoxText(pXL, "<",				m_nSttRow, "AF", "AF", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, 1.2,				m_nSttRow, "AG", "AH", 2, BLACK, 9, FALSE, TA_CENTER, "HLoad1", 1, FALSE);
	sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")", pOut->GetCellRef("HLoad"), pOut->GetCellRef("HLoad1"));
	pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AJ", "AK", 2, BLUE, 9, FALSE, TA_CENTER, "", 0, FALSE);
	
	SetLines(3);

}

void CXLDesignCrossBeam::SetLines(int nLine)
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
