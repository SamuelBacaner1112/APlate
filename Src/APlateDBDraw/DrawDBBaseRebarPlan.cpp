// DrawDBBaseRebarPlan.cpp: implementation of the DrawDBBaseRebarPlan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDBDraw.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawDBBaseRebarPlan::CDrawDBBaseRebarPlan(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}


CDrawDBBaseRebarPlan::~CDrawDBBaseRebarPlan()
{

}

BOOL CDrawDBBaseRebarPlan::IsLeftOfJijumLine(CPlateBasicIndex *pBx, CDPoint ptStt, CDPoint ptEnd, BOOL bStt)
{	
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBxFinder Finder(pGir);
	if(pBx->IsJijum() && pGir->GetNumberJijumByBx(pBx)==0)
	{		
		pBx = Finder.GetBxFirst(BX_STT_JOINT);
	}
	if(pBx->IsJijum() && pGir->GetNumberJijumByBx(pBx)==pDB->m_nQtyJigan)
	{
		pBx = Finder.GetBxFirst(BX_END_JOINT);
	}

	double LDist	  = pDB->GetDisSlabLeftAct(pBx->GetStation());
	double RDist	  = pDB->GetDisSlabRighAct(pBx->GetStation());	
	
	double sta1 = pGir->GetStationBySc(pBx->GetStation(),LDist-pGir->m_dGirderCenter,pBx->GetAngle());
	double sta2 = pGir->GetStationBySc(pBx->GetStation(),RDist-pGir->m_dGirderCenter,pBx->GetAngle());
	CDPoint xy1  = pGir->GetXyGirderDis(sta1,LDist-pGir->m_dGirderCenter);
	CDPoint xy2  = pGir->GetXyGirderDis(sta2,RDist-pGir->m_dGirderCenter);	

	if(bStt)
	{
		if(GetDirectionRotateByPoint(xy1, ptStt, xy2)==-1 || GetDirectionRotateByPoint(xy1, ptEnd, xy2)==-1) return FALSE;
		return TRUE;
	}
	else
	{
		if(GetDirectionRotateByPoint(xy1, ptStt, xy2)==1 || GetDirectionRotateByPoint(xy1, ptEnd, xy2)==1) return TRUE;
		return FALSE;
	}

	return FALSE;
}

void CDrawDBBaseRebarPlan::TrimByJijumBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bStt)
{
	CPlateBridgeApp	*pDB = m_pDataMng->GetBridge();

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBxFinder Finder(pGir);
	if(pBx->IsJijum() && pGir->GetNumberJijumByBx(pBx)==0)
	{		
		pBx = Finder.GetBxFirst(BX_STT_JOINT);
	}
	if(pBx->IsJijum() && pGir->GetNumberJijumByBx(pBx)==pDB->m_nQtyJigan)
	{
		pBx = Finder.GetBxFirst(BX_END_JOINT);
	}

	double LDist	  = pDB->GetDisSlabLeftAct(pBx->GetStation());
	double RDist	  = pDB->GetDisSlabRighAct(pBx->GetStation());	
	
	double sta1 = pGir->GetStationBySc(pBx->GetStation(),LDist-pGir->m_dGirderCenter,pBx->GetAngle());
	double sta2 = pGir->GetStationBySc(pBx->GetStation(),RDist-pGir->m_dGirderCenter,pBx->GetAngle());
	CDPoint xy1  = pGir->GetXyGirderDis(sta1,LDist-pGir->m_dGirderCenter);
	CDPoint xy2  = pGir->GetXyGirderDis(sta2,RDist-pGir->m_dGirderCenter);	
	pDom->TrimByLineOnSide(xy1, xy2, bStt ? 1 : -1);	
}



// 가로보 보강철근 그리기
// pBx : 가로보 위치
void CDrawDBBaseRebarPlan::DrawBogangRebarAtCrossBeamBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bDrawAlways)
{	
	if(!pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))	return;

	CPlateBridgeApp	*pDB	= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt	= m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CPlateCrossBeam	*pC		 = pBx->GetSection()->GetCrossBeam();
	CPlateGirderApp	*pGir	 = pBx->GetGirder();
	CPlateGirderApp	*pGirSec = pGir->m_pRight;
	CPlateBasicIndex*pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);
	CLineInfo		*pLine   = pDB->GetGirder(-1)->GetLine();

	if(!pGir->IsCrossBeamBxHasHunch(pBx))	return;
	
	CDPoint xy1 = pGir->GetXyPlanSlabCrossbeamBase(pBx, TRUE);
	CDPoint xy2 = pGir->GetXyPlanSlabCrossbeamBase(pBx, FALSE);
	CDPoint xyc = (xy1 + xy2) / 2;	

	double dRebarLenC1 = pDeckData->GetLengthCrossBeamHunchRebar(pBx, FALSE);
	double dRebarLenC3 = pDeckData->GetLengthCrossBeamHunchRebar(pBx, TRUE);

	CDoubleArray dArr;	
	pDeckData->GetBoganRebarArrAtCrossBeam(dArr, pBx);

	CDPoint xyCur, vAngCross;
	double dStaCur = 0, dStaLeft = 0;
	double dLLenC1 = (nSide==-1 || nSide==0) ? dRebarLenC1/2 : 0;
	double dRLenC1 = (nSide== 1 || nSide==0) ? dRebarLenC1/2 : 0;
	double dLLenC3 = (nSide==-1 || nSide==0) ? dRebarLenC3/2 : 0;
	double dRLenC3 = (nSide== 1 || nSide==0) ? dRebarLenC3/2 : 0;
	double dOffsetDist = 15;// C1, C3구분을 위한 이격거리

	CDomyun Dom(pDom);		

	for(long n=0; n<dArr.GetSize(); n++)
	{
		double dDist = dArr.GetAt(n);
	
		if(bDrawAlways || pDeckData->IsDrawMainRebarArea(xy1, xy2))
		{			
			dStaLeft  = pBxSec->GetStation();
			vAngCross = pBxSec->GetAngle();
			xyCur	  = pGir->GetXyGirderDis(dStaLeft, dDist-pGir->m_dGirderCenter, vAngCross);										
			pLine->GetStationMatchVerticalByXy(xyCur, dStaCur);
			
			// C3(가로보 상면 보강철근)
			if(pDeckData->m_bInstallCrossBeamUpperBogangRebar)
			{			
				pOpt->SetEnvType(&Dom,HCAD_BARC);
				pLine->DrawLS(&Dom, dStaCur-dLLenC3,dStaCur+dRLenC3, dDist+dOffsetDist);				
			}

			// C1(가로보 헌치 보강철근)
			pOpt->SetEnvLType(&Dom,LT_HIDDEN2_LINE);
			pLine->DrawLS(&Dom, dStaCur-dLLenC1, dStaCur-pC->m_dM_CR+pDeckData->m_dCoverSd,dDist);
			pOpt->SetEnvLType(&Dom,LT_SOLID);
			pLine->DrawLS(&Dom, dStaCur-pC->m_dM_CR+pDeckData->m_dCoverSd, dStaCur+pC->m_dM_CR-pDeckData->m_dCoverSd, dDist);
			pOpt->SetEnvLType(&Dom,LT_HIDDEN2_LINE);
			pLine->DrawLS(&Dom, dStaCur+pC->m_dM_CR-pDeckData->m_dCoverSd, dStaCur+dRLenC1,dDist);
		}				
	}
	
	// C2(가로보 하면 보강철근)
	CVector vVecLine = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dLengthC2 = pGir->GetDisFrontPlateWebToWeb(pBx) - pGir->GetLengthSharpOnBx(pBx, TRUE) - pGir->m_pRight->GetLengthSharpOnBx(pBx, TRUE)
					  -pGir->GetThickFactByStation(pBx->GetStation(), G_W)-pGir->m_pRight->GetThickFactByStation(pBx->GetStation(), G_W)
					  +pDeckData->m_dLengthCrossBeamBogangLower*2;	
	double dWidth = pC->m_dM_CR + 50/*Hunch*/-pDeckData->m_dCoverSd;
	pOpt->SetEnvLType(&Dom,LT_SOLID);
	if(bDrawAlways)
	{
		CDPoint vVec = (xy2-xy1).Unit();
		CDPoint pt1 = xyc - vVec*dLengthC2/2 - vVecLine*dWidth/pBx->GetAngle().y;
		CDPoint pt2 = xyc + vVec*dLengthC2/2 - vVecLine*dWidth/pBx->GetAngle().y;
		CDPoint pt3 = xyc - vVec*dLengthC2/2 + vVecLine*dWidth/pBx->GetAngle().y;
		CDPoint pt4 = xyc + vVec*dLengthC2/2 + vVecLine*dWidth/pBx->GetAngle().y;

		Dom.LineTo(pt1,pt2);
		Dom.LineTo(pt3,pt4);
	}

	*pDom << Dom;
}

// 스트링거 헌치하단 보강철근 그리기
// pBx : 스트링거 위치
void CDrawDBBaseRebarPlan::DrawBogangRebarAtStringerBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bDrawAlways)
{	
/*	CPlateBridgeApp	*pDB	 = m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt	 = m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CPlateCrossBeam	*pC		 = pBx->GetCrossBeam();
	CPlateGirderApp		*pGir	 = pBx->GetGirder();
	CPlateGirderApp		*pGirSec = pGir->m_pRight;
	CPlateBasicIndex	*pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);
	CLineInfo			*pLine   = pDB->GetGirder(-1)->GetLine();

	if(pDeckData->m_arrStringer.GetSize() <=0)	return;

	CDomyun Dom(pDom);		

	BOOL bMatchStt = FALSE, bMatchEnd = FALSE;

	for(long nS=0; nS<pC->m_nQtyStringer_CR; nS++)
	{
		CStringer *pStr = pBx->GetStringer(nS);

		long nG = pBx->GetNumberGirder();
		long nC = pC->GetSelfCount();
		for (long nStg=0; nStg<pDeckData->m_arrStringer.GetSize(); nStg++)
		{
			if(pDeckData->m_arrStringer[nStg] == CVector(nG, nC, nS))
				break;
		}

		if (pStr==NULL)		continue;

		CDPoint xy1 = pGir->GetXyPlanSlabStgBase(pBx, nS, TRUE);
		CPlateBasicIndex *pBxNext = pGir->GetBxByCrossNo(pGir->GetNumberCrossBeamByBx(pBx)+1);
		CDPoint xy2 = pGir->GetXyPlanSlabStgBase(pBxNext, nS, FALSE);

		pDeckData->IsStringerMatchAtTaper(pStr, bMatchStt, bMatchEnd, xy1, xy2);

		CDPoint vVec = (xy2 - xy1).Unit();
		CDPoint xyc = (xy1 + xy2) / 2;	
		//////////////////////////////////////////////////////////////////////////
		// 헌치 보강철근
		double dLength = ~(xy1 - xy2);
		double dRebarLen = pDeckData->GetLengthStringerHunchRebar(pBx, nS, FALSE);
		
		CDPoint ptData = pDeckData->m_arrCTCBogangRebarAtStringer.GetAt(nStg);
		CDPoint xyBase = xyc - vVec * (ptData.x * ptData.y)/2, xyLeftStt, xyLeftEnd, xyRightStt, xyRightEnd;
				
		for(long n=0; n<ptData.x; n++)
		{
			if(nSide==-1 && n>ptData.x/2) break;
			if(nSide== 1 && n<ptData.x/2)
			{
				xyBase += vVec * ptData.y;
				continue;
			}
			xyLeftEnd	= xyBase + vVec.Rotate90() * dRebarLen/2;
			xyLeftStt	= xyBase + vVec.Rotate90() * (pStr->m_uM - pDeckData->m_dCoverSd);			
			xyRightStt	= xyBase + vVec.RotateInv90() * (pStr->m_uM - pDeckData->m_dCoverSd);
			xyRightEnd	= xyBase + vVec.RotateInv90() * dRebarLen/2;
			
			pOpt->SetEnvLType(&Dom,LT_HIDDEN2_LINE);
			Dom.LineTo(xyLeftEnd, xyLeftStt);
			Dom.LineTo(xyRightStt, xyRightEnd);
			pOpt->SetEnvLType(&Dom,LT_SOLID);
			Dom.LineTo(xyLeftStt, xyRightStt);
			xyBase += vVec * ptData.y;
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 상부 보강철근
		double dOffsetDist = 15;// S1, S3구분을 위한 이격거리
		dRebarLen = pDeckData->m_dLengthStringerBogang;
		dRebarLen = pDeckData->GetLengthStringerHunchRebar(pBx, nS, TRUE);
		xyBase = xyc - vVec * (ptData.x * ptData.y)/2, xyLeftStt, xyLeftEnd, xyRightStt, xyRightEnd;
		if(pDeckData->m_bInstallStringerUpperBogangRebar)
		{
			for(long n=0; n<ptData.x; n++)
			{
				if(nSide==-1 && n>ptData.x/2) break;
				if(nSide== 1 && n<ptData.x/2)
				{
					xyBase += vVec * ptData.y;
					continue;
				}
				xyLeftEnd	= xyBase + vVec*dOffsetDist + vVec.Rotate90()*dRebarLen/2;
				xyLeftStt	= xyBase + vVec*dOffsetDist + vVec.Rotate90()*(pStr->m_uM - pDeckData->m_dCoverSd);			
				xyRightStt	= xyBase + vVec*dOffsetDist + vVec.RotateInv90()*(pStr->m_uM - pDeckData->m_dCoverSd);
				xyRightEnd	= xyBase + vVec*dOffsetDist + vVec.RotateInv90()*dRebarLen/2;
				
				pOpt->SetEnvLType(&Dom,LT_SOLID);
				Dom.LineTo(xyLeftEnd, xyLeftStt);
				Dom.LineTo(xyRightStt, xyRightEnd);
				Dom.LineTo(xyLeftStt, xyRightStt);

				xyBase += vVec * ptData.y;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//교축방향 철근
		double	dSettleStt = pDeckData->GetValueSettle(toKgPCm2(pDeckData->m_SigmaCK), toKgPCm2(pDeckData->m_SigmaY), GetRebarSizeByIndex(pDeckData->m_dDiaStringer2), JOINT_TENS_NORMAL_B, toCm(pDeckData->m_dCoverDn), toCm(pStr->m_uM*2));
		double	dSettleEnd = pDeckData->GetValueSettle(toKgPCm2(pDeckData->m_SigmaCK), toKgPCm2(pDeckData->m_SigmaY), GetRebarSizeByIndex(pDeckData->m_dDiaStringer2), JOINT_TENS_NORMAL_B, toCm(pDeckData->m_dCoverDn), toCm(pStr->m_uM*2));
		CDPoint pt1, pt2, pt3, pt4;
		double dWidthHalf = pStr->m_dM + 50 - pDeckData->m_dCoverSd;
		pt1 = xy1 - vVec * dSettleStt - vVec.Rotate90()*dWidthHalf;
		pt2 = xy2 + vVec * dSettleEnd - vVec.Rotate90()*dWidthHalf;
		pt3 = xy1 - vVec * dSettleStt + vVec.Rotate90()*dWidthHalf;
		pt4 = xy2 + vVec * dSettleEnd + vVec.Rotate90()*dWidthHalf;
		Dom.LineTo(pt1, pt2);
		Dom.LineTo(pt3, pt4);
	}

	*pDom << Dom;	*/
}



// 스트링거 헌치하단 보강철근 그리기
// pBx : 스트링거 위치
void CDrawDBBaseRebarPlan::DrawBogangRebarAtStringer(CDomyun *pDom, long nStg, long nSide, BOOL bDrawAlways)
{	
/*	CPlateBridgeApp	*pDB	 = m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt	 = m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();

	CVector vStg = pDeckData->m_arrStringer[nStg];
	long nCurStg = long(vStg.z);
	CPlateBasicIndex *pBx	 = pDB->GetGirder(long(vStg.x))->GetCrossBeamByCrossNo(long(vStg.y))->GetBx();
	CStringer		 *pStg	 = pBx->GetStringer(nCurStg);
	
	if(pBx==NULL)	return;
	
	CPlateGirderApp		*pGir	 = pBx->GetGirder();

	CDomyun Dom(pDom);		
	pOpt->SetEnvType(&Dom,HCAD_BARC);

	CDPoint xyStt = pGir->GetXyPlanSlabStgBase(pBx, nCurStg, TRUE);
	CPlateBasicIndex *pBxNext = pGir->GetBxByCrossNo(pGir->GetNumberCrossBeamByBx(pBx)+1);
	CDPoint xyEnd = pGir->GetXyPlanSlabStgBase(pBxNext, nCurStg, FALSE);

	BOOL bMatchStt = FALSE, bMatchEnd = FALSE;	// 스트링거가 테이퍼에 접촉하는지 검사..
	CDPoint xyMatchStt, xyMatchEnd;
	pDeckData->IsStringerMatchAtTaper(pStg, bMatchStt, bMatchEnd, xyMatchStt, xyMatchEnd);
	if(bMatchStt)		xyStt = xyMatchStt;
	if(bMatchEnd)		xyEnd = xyMatchEnd;

	CDPoint vVec  = (xyEnd - xyStt).Unit();
	CDPoint xyCen = (xyStt + xyEnd) / 2;	
	//////////////////////////////////////////////////////////////////////////
	// 헌치 보강철근
	double dLength = ~(xyStt - xyEnd);
	double dRebarLen = pDeckData->GetLengthStringerHunchRebar(pBx, nCurStg, FALSE);
	
	CDPoint ptData = pDeckData->m_arrCTCBogangRebarAtStringer.GetAt(nStg);
	CDPoint xyBase = xyCen - vVec * (ptData.x * ptData.y)/2, xyLeftStt, xyLeftEnd, xyRightStt, xyRightEnd;
			
	for(long n=0; n<=ptData.x; n++)
	{
		if(nSide==-1 && n>ptData.x/2) break;
		if(nSide== 1 && n<ptData.x/2)
		{
			xyBase += vVec * ptData.y;
			continue;
		}
		xyLeftEnd	= xyBase + vVec.Rotate90() * dRebarLen/2;
		xyLeftStt	= xyBase + vVec.Rotate90() * (pStg->m_uM - pDeckData->m_dCoverSd);			
		xyRightStt	= xyBase + vVec.RotateInv90() * (pStg->m_uM - pDeckData->m_dCoverSd);
		xyRightEnd	= xyBase + vVec.RotateInv90() * dRebarLen/2;
		
		pOpt->SetEnvLType(&Dom,LT_HIDDEN2_LINE);
		Dom.LineTo(xyLeftEnd, xyLeftStt);
		Dom.LineTo(xyRightStt, xyRightEnd);
		pOpt->SetEnvLType(&Dom,LT_SOLID);
		Dom.LineTo(xyLeftStt, xyRightStt);
		xyBase += vVec * ptData.y;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 상부 보강철근
	double dOffsetDist = 15;// S1, S3구분을 위한 이격거리
	dRebarLen = pDeckData->m_dLengthStringerBogang;
	dRebarLen = pDeckData->GetLengthStringerHunchRebar(pBx, nCurStg, TRUE);
	xyBase = xyCen - vVec * (ptData.x * ptData.y)/2, xyLeftStt, xyLeftEnd, xyRightStt, xyRightEnd;
	
	if(pDeckData->m_bInstallStringerUpperBogangRebar)
	{
		for(long n=0; n<ptData.x; n++)
		{
			if(nSide==-1 && n>ptData.x/2) break;
			if(nSide== 1 && n<ptData.x/2)
			{
				xyBase += vVec * ptData.y;
				continue;
			}
			xyLeftEnd	= xyBase + vVec*dOffsetDist + vVec.Rotate90()*dRebarLen/2;
			xyLeftStt	= xyBase + vVec*dOffsetDist + vVec.Rotate90()*(pStg->m_uM - pDeckData->m_dCoverSd);			
			xyRightStt	= xyBase + vVec*dOffsetDist + vVec.RotateInv90()*(pStg->m_uM - pDeckData->m_dCoverSd);
			xyRightEnd	= xyBase + vVec*dOffsetDist + vVec.RotateInv90()*dRebarLen/2;
			
			pOpt->SetEnvLType(&Dom,LT_SOLID);
			Dom.LineTo(xyLeftEnd, xyLeftStt);
			Dom.LineTo(xyRightStt, xyRightEnd);
			Dom.LineTo(xyLeftStt, xyRightStt);

			xyBase += vVec * ptData.y;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//교축방향 철근
	double	dSettleStt = pDeckData->GetValueSettle(toKgPCm2(pDeckData->m_SigmaCK), toKgPCm2(pDeckData->m_SigmaY), GetRebarSizeByIndex(pDeckData->m_dDiaStringer2), JOINT_TENS_NORMAL_B, toCm(pDeckData->m_dCoverDn), toCm(pStg->m_uM*2));
	double	dSettleEnd = pDeckData->GetValueSettle(toKgPCm2(pDeckData->m_SigmaCK), toKgPCm2(pDeckData->m_SigmaY), GetRebarSizeByIndex(pDeckData->m_dDiaStringer2), JOINT_TENS_NORMAL_B, toCm(pDeckData->m_dCoverDn), toCm(pStg->m_uM*2));
	
	CDPoint pt1, pt2, pt3, pt4;
	double dWidthHalf = pStg->m_dM + 50 - pDeckData->m_dCoverSd;
	pt1 = xyStt - vVec * dSettleStt - vVec.Rotate90()*dWidthHalf;
	pt2 = xyEnd + vVec * dSettleEnd - vVec.Rotate90()*dWidthHalf;
	pt3 = xyStt - vVec * dSettleStt + vVec.Rotate90()*dWidthHalf;
	pt4 = xyEnd + vVec * dSettleEnd + vVec.Rotate90()*dWidthHalf;
	Dom.LineTo(pt1, pt2);
	Dom.LineTo(pt3, pt4);

	*pDom << Dom;	*/
}



// 가로보 헌치하단 보강철근 치수선 그리기
// pBx : 가로보 위치
void CDrawDBBaseRebarPlan::DimBogangRebarAtCrossBeamBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bDrawAlways)
{	
	CPlateBridgeApp	*pDB	 = m_pDataMng->GetBridge();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CPlateGirderApp	 *pGir	  = pBx->GetGirder();
	CPlateGirderApp	 *pGirSec = pGir->m_pRight;
	CPlateBasicIndex *pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);
	CLineInfo		 *pLine   = pDB->GetGirder(-1)->GetLine();

	long nCount = 0;
	CDPoint ptData = pDeckData->GetCTCRebarAtCB(pBx, nCount);

	CDPoint xy1 = pGir->GetXyPlanSlabCrossbeamBase(pBx, TRUE);
	CDPoint xy2 = pGir->GetXyPlanSlabCrossbeamBase(pBx, FALSE);
	double dRebarLen = pDeckData->GetLengthCrossBeamHunchRebar(pBx, TRUE);	// C3철근 길이

	CDomyun Dom(pDom);		
	Dom.SetDirection("BOTTOM");
	Dom.SetDimExo(3);
	CDoubleArray dArr;	
	pDeckData->GetBoganRebarArrAtCrossBeam(dArr, pBx);
	CDPoint xyCurStt, xyCurEnd, ptStt, ptEnd;
	double dStaCenStt = 0, dStaCenEnd = 0, dStaCur = 0;
	
	double dDistStt = dArr.GetAt(0);
	double dDistEnd = dArr.GetAt(dArr.GetUpperBound());
	if(bDrawAlways || pDeckData->IsDrawMainRebarArea(xy1,xy2))
	{			
		double sta	 = pBxSec->GetStation();
		CDPoint vAngCross = pBxSec->GetAngle();
		xyCurStt = pGir->GetXyGirderDis(sta, dDistStt-pGir->m_dGirderCenter, vAngCross);
		xyCurEnd = pGir->GetXyGirderDis(sta, dDistEnd-pGir->m_dGirderCenter, vAngCross);
		pLine->GetStationMatchVerticalByXy(xyCurStt, dStaCenStt);		
		pLine->GetStationMatchVerticalByXy(xyCurEnd, dStaCenEnd);		
		dStaCur = min(dStaCenStt-dRebarLen/2, dStaCenEnd-dRebarLen/2);

		CDPoint ptStt = pLine->GetXyLineDis(dStaCur,dDistStt);							
		CDPoint ptEnd = pLine->GetXyLineDis(dStaCur,dDistEnd);			

		CString szDim, szPos;
		szDim.Format("%.0f@%.0f=%s",ptData.x,ptData.y,COMMA(ptData.x*ptData.y));
		szPos.Format("C%d",nCount+1);
		Dom.DimMoveTo(ptStt.x, ptStt.y,0);
		Dom.DimLineToExtend(ptData.x*ptData.y,0,szDim,szPos);
		Dom.Rotate(ptStt, (ptEnd-ptStt).Unit());
	}					
	*pDom << Dom;
}



void CDrawDBBaseRebarPlan::DimBogangRebarAtStringer(CDomyun *pDom, long nStg, long nSide, BOOL bDrawAlways)
{
/*	CPlateBridgeApp	*pDB	 = m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt	 = m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();

	if(pDeckData->m_arrStringer.GetSize() <=0)	return;

	CDPoint xyDim= pDeckData->m_arrCTCBogangRebarAtStringer[nStg];
	CVector vStg = pDeckData->m_arrStringer[nStg];

	long nCurStg = long(vStg.z);
	CPlateBasicIndex		*pBx	 = pDB->GetGirder(vStg.x)->GetCrossBeamByCrossNo(vStg.y)->GetBx();
	CStringer		*pStg	 = pBx->GetStringer(nCurStg);
	CPlateGirderApp		*pGir	 = pBx->GetGirder();
	CLineInfo		*pLine   = pDB->GetGirder(-1)->GetLine();

	CDPoint xyStt = pGir->GetXyPlanSlabStgBase(pBx, nCurStg, TRUE);
	CPlateBasicIndex *pBxNext = pGir->GetBxByCrossNo(pGir->GetNumberCrossBeamByBx(pBx)+1);
	CDPoint xyEnd = pGir->GetXyPlanSlabStgBase(pBxNext, nCurStg, FALSE);

	BOOL bMatchStt = FALSE, bMatchEnd = FALSE;	// 스트링거가 테이퍼에 접촉하는지 검사..
	CDPoint xyMatchStt, xyMatchEnd;
	pDeckData->IsStringerMatchAtTaper(pStg, bMatchStt, bMatchEnd, xyMatchStt, xyMatchEnd);
	if(bMatchStt)		xyStt = xyMatchStt;
	if(bMatchEnd)		xyEnd = xyMatchEnd;

	CDPoint vVec  = (xyEnd - xyStt).Unit();
	CDPoint xyCen = (xyStt + xyEnd) / 2;	

	CDomyun Dom(pDom);

	double dRebarLen = pDeckData->GetLengthStringerHunchRebar(pBx, pStg->m_nStg, FALSE);
	CDPoint xySttDim = xyCen + vVec.Rotate90()*1200 - vVec*(xyDim.x*xyDim.y)/2;
	CString szDim, szPos;
	szDim.Format("%.0f@%.0f=%s",xyDim.x,xyDim.y,COMMA(xyDim.x*xyDim.y));
	szPos.Format("S%d", nStg+1);
	Dom.DimMoveTo(xySttDim.x, xySttDim.y, 0);
	Dom.DimLineToExtend(xyDim.x*xyDim.y, 0, szDim,szPos);

	Dom.Rotate(xySttDim, (xyEnd-xyStt).Unit());
		
	*pDom << Dom;*/
}



// 가로보 헌치하단 보강철근 치수선 그리기
// pBx : 가로보 위치
void CDrawDBBaseRebarPlan::DimBogangRebarAtOutBracketBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft, long nSide, BOOL bDrawAlways)
{	
/*	CPlateBridgeApp	*pDB	 = m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt	 = m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CBracket		*pOutBrk = pBx->GetBracket(bLeft);
	CLineInfo		*pLine   = pDB->GetGirder(-1)->GetLine();
	CPlateGirderApp	*pGir	 = pBx->GetGirder();

	long nOutBkIdx = pOutBrk->GetSelfCount();
	if(!bLeft)
		nOutBkIdx += pDB->GetLBracketsu();

	CDPoint ptData = pDeckData->m_arrCTCBogangRebarAtOutBrk.GetAt(nOutBkIdx);

	CDPoint xy1 = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, TRUE);
	CDPoint xy2 = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, FALSE);
	double dRebarLen = pDeckData->GetLengthOutBrkHunchRebar(pBx, TRUE, bLeft);	// C3철근 길이

	CDomyun Dom(pDom);		
	Dom.SetDirection("BOTTOM");
	Dom.SetDimExo(3);
	CDoubleArray dArr;	
	pDeckData->GetBoganRebarArrAtOutBrk(dArr, pBx, bLeft);
	CDPoint xyCurStt, xyCurEnd, ptStt, ptEnd;
	double dStaCenStt = 0, dStaCenEnd = 0, dStaCur = 0;
	
	double dDistStt = dArr.GetAt(0);
	double dDistEnd = dArr.GetAt(dArr.GetUpperBound());
	if(bDrawAlways || pDeckData->IsDrawMainRebarArea(xy1,xy2))
	{			
		double  sta	      = pBx->GetStation();
		CDPoint vAngCross = pBx->GetAngle();
		xyCurStt = pGir->GetXyGirderDis(sta, dDistStt-pGir->m_dGirderCenter, vAngCross);
		xyCurEnd = pGir->GetXyGirderDis(sta, dDistEnd-pGir->m_dGirderCenter, vAngCross);
		pLine->GetStationMatchVerticalByXy(xyCurStt, dStaCenStt);		
		pLine->GetStationMatchVerticalByXy(xyCurEnd, dStaCenEnd);		
		dStaCur = min(dStaCenStt-dRebarLen/2, dStaCenEnd-dRebarLen/2);

		CDPoint ptStt = pLine->GetXyLineDis(dStaCur,dDistStt);							
		CDPoint ptEnd = pLine->GetXyLineDis(dStaCur,dDistEnd);			

		CString szDim, szPos;
		szDim.Format("%.0f@%.0f=%s",ptData.x,ptData.y,COMMA(ptData.x*ptData.y));
		szPos.Format("OB%d",nOutBkIdx+1);
		Dom.DimMoveTo(ptStt.x, ptStt.y,0);
		Dom.DimLineToExtend(ptData.x*ptData.y,0,szDim,szPos);
		Dom.Rotate(ptStt, (ptEnd-ptStt).Unit());
	}					
	*pDom << Dom;*/
}


// 가로보 헌치하단 보강철근 그리기
void CDrawDBBaseRebarPlan::DrawBogangRebarAtCrossBeam(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp	*pDB	= m_pDataMng->GetBridge();	
	
	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++) 
	{
		if(nG>pDB->GetGirdersu()/2-1) continue;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);					
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
		long nFlag = 0;
		while(pBx)
		{			
			if(pBx->GetCrossBeam() && pGir->GetHeightTaper(pBx->GetStation())==0)
			{
				if(pBx==pBxEnd)			nFlag = -1;
				else if(pBx==pBxStt)	nFlag = 1;
				else					nFlag = 0;
				DrawBogangRebarAtCrossBeamBx(pDom, pBx, nFlag);
			}
			pBx = Finder.GetBxNext();
		}					
	}	
}






// 가로보 헌치하단 보강철근 그리기
void CDrawDBBaseRebarPlan::DrawBogangRebarAtOutBrk(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
/*	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();	
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	
	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++) 
	{
		if(nG>pDB->GetGirdersu()/2-1) continue;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);					
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
		long nFlag = 0;
		while(pBx)
		{			
			if(pBx->GetCrossBeam() && pGir->GetHeightTaper(pBx->GetStation())==0)
			{
				if(pBx==pBxEnd)			nFlag = -1;
				else if(pBx==pBxStt)	nFlag = 1;
				else					nFlag = 0;
				DrawBogangRebarAtCrossBeamBx(pDom, pBx, nFlag);
			}
			pBx = Finder.GetBxNext();
		}					
	}	*/
}


// 가로보 보강철근 그리기
// pBx : 가로보 위치
void CDrawDBBaseRebarPlan::DrawBogangRebarAtOutBrkBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft, long nSide, BOOL bDrawAlways)
{	
/*	CPlateBridgeApp		*pDB	 = m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt	 = m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CBracket		*pBrk	 = pBx->GetBracket(bLeft);
	CPlateGirderApp		*pGir	 = pBx->GetGirder();
	CLineInfo		*pLine   = pDB->GetGirder(-1)->GetLine();

	double dRebarLenOB1 = pDeckData->GetLengthOutBrkHunchRebar(pBx, FALSE, bLeft);
	double dRebarLenOB3 = pDeckData->GetLengthOutBrkHunchRebar(pBx, TRUE, bLeft);

	CDoubleArray dArr;	
	pDeckData->GetBoganRebarArrAtOutBrk(dArr, pBx, bLeft);

	if (dArr.GetSize()<2)	return;	

	CDPoint xyCur, vAngCross;
	double dStaCur = 0, dStaLeft = 0;
	double dLLenOB1 = (nSide==-1 || nSide==0) ? dRebarLenOB1/2 : 0;
	double dRLenOB1 = (nSide== 1 || nSide==0) ? dRebarLenOB1/2 : 0;
	double dLLenOB3 = (nSide==-1 || nSide==0) ? dRebarLenOB3/2 : 0;
	double dRLenOB3 = (nSide== 1 || nSide==0) ? dRebarLenOB3/2 : 0;
	double dOffsetDist = 15;// OB1, OB3구분을 위한 이격거리
	CDPoint xy1 = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, TRUE);
	CDPoint xy2 = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, FALSE);
	CDPoint xyc = (xy1 + xy2) / 2;	

	CDomyun Dom(pDom);		

	for(long n=0; n<dArr.GetSize(); n++)
	{
		double dDist = dArr.GetAt(n);
	
		if(bDrawAlways || pDeckData->IsDrawMainRebarArea(xy1, xy2))
		{			
			dStaLeft  = pBx->GetStation();
			vAngCross = pBx->GetAngle(); 
			xyCur	  = pGir->GetXyGirderDis(dStaLeft, dDist-pGir->m_dGirderCenter, vAngCross);										
			pLine->GetStationMatchVerticalByXy(xyCur, dStaCur);
			
			// OB3(외측가로보 상면 보강철근)
			if(pDeckData->m_bInstallOutBrkUpperBogangRebar)
			{			
				pOpt->SetEnvType(&Dom,HCAD_BARC);
				pLine->DrawLS(&Dom, dStaCur-dLLenOB3,dStaCur+dRLenOB3, dDist+dOffsetDist);				
			}

			// OB1(외측가로보 헌치 보강철근)
			pOpt->SetEnvLType(&Dom,LT_HIDDEN2_LINE);
			pLine->DrawLS(&Dom, dStaCur-dLLenOB1, dStaCur-pBrk->m_dM_EB+pDeckData->m_dCoverSd,dDist);
			pOpt->SetEnvLType(&Dom,LT_SOLID);
			pLine->DrawLS(&Dom, dStaCur-pBrk->m_dM_EB+pDeckData->m_dCoverSd, dStaCur+pBrk->m_dM_EB-pDeckData->m_dCoverSd, dDist);
			pOpt->SetEnvLType(&Dom,LT_HIDDEN2_LINE);
			pLine->DrawLS(&Dom, dStaCur+pBrk->m_dM_EB-pDeckData->m_dCoverSd, dStaCur+dRLenOB1,dDist);
		}				
	}
	
	// OB2(외측가로보 하면 보강철근)
	CVector vVecLine = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dLengthOB2 = ~(xy1-xy2)+pDeckData->m_dLengthOutBrkBogangLower*2;
	double dWidth = pBrk->m_uM_EB + pDB->m_dHunchWidth - pDeckData->m_dCoverSd;
	pOpt->SetEnvLType(&Dom,LT_SOLID);
	if(bDrawAlways)
	{
		CDPoint vVec = (xy2-xy1).Unit();
		CDPoint pt1 = xyc - vVec*dLengthOB2/2 - vVecLine*dWidth/pBx->GetAngle().y;
		CDPoint pt2 = xyc + vVec*dLengthOB2/2 - vVecLine*dWidth/pBx->GetAngle().y;
		CDPoint pt3 = xyc - vVec*dLengthOB2/2 + vVecLine*dWidth/pBx->GetAngle().y;
		CDPoint pt4 = xyc + vVec*dLengthOB2/2 + vVecLine*dWidth/pBx->GetAngle().y;

		Dom.LineTo(pt1,pt2);
		Dom.LineTo(pt3,pt4);
	}

	*pDom << Dom;*/
}





void CDrawDBBaseRebarPlan::DrawMainRebar(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nDrawSide, BOOL bDrawAlways, BOOL bDrawJoint)
{	
	CADeckData		*pDeckData= m_pDataMng->GetDeckData();

	CDomyun Dom(pDom);

	vector <REBAR_PLAN>* vPlan = pDeckData->GetMainRebarArr( bUpper, nDrawSide);

	CTwinVectorArray tvArr;

	for(long n=0; n<(long)vPlan->size(); n++)
	{
		REBAR_PLAN rebarPlan = vPlan->at(n);
		
		if(bDrawAlways || (!IsLeftOfJijumLine(pBxStt, rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2, TRUE) && 
			IsLeftOfJijumLine(pBxEnd, rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2, FALSE)))
		{
			if(bDrawAlways || pDeckData->IsDrawMainRebarArea(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2))
			{
				Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
				tvArr.Add(rebarPlan.m_tVector);
			}
		}
	}

	if(bDrawJoint)	DrawJoint(&Dom, vPlan, pBxStt, pBxEnd, TRUE);

	TrimByJijumBx(&Dom, pBxStt,TRUE);
	TrimByJijumBx(&Dom, pBxEnd,FALSE);
	
	*pDom << Dom;
}


// 신축이음부 아래 주철근 그리기
void CDrawDBBaseRebarPlan::DrawMainRebarUnderSinchuk(CDomyun *pDom, BOOL bStt, long nDrawSide, BOOL bDrawAlways)
{
	CADeckData *pDeckData	= m_pDataMng->GetDeckData();
	
	CDomyun Dom(pDom);	

	vector <REBAR_PLAN>* vPlan = pDeckData->GetMainRebarArrUnderSinChuk(bStt, nDrawSide);
	if(vPlan==NULL)		return;

	for(long n=0; n<(long)vPlan->size(); n++)
	{
		REBAR_PLAN rebarPlan = vPlan->at(n);
		
		if(pDeckData->IsDrawMainRebarArea(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2) || bDrawAlways)
		{
			Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
		}
	}
	
	*pDom << Dom;
}



// 사보강부 주철근 배열 그리기
void CDrawDBBaseRebarPlan::DrawMainRebarAtSkew(CDomyun *pDom, BOOL bUpper, long nDrawSide, BOOL bStt, BOOL bEnd, BOOL bDrawAlways,BOOL bTrimRebar)
{
	CADeckData *pDeckData	= m_pDataMng->GetDeckData();
	
	CDomyun Dom(pDom);	

	if(bStt)
	{
		vector <REBAR_PLAN>* vPlan = pDeckData->GetMainRebarArrAtSkew(TRUE, bUpper, nDrawSide);
		if(vPlan==NULL)		return;

		for(long n=0; n<(long)vPlan->size(); n++)
		{
			REBAR_PLAN rebarPlan = vPlan->at(n);
			
			if(bTrimRebar == FALSE)
			{
				Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
			}
			else if(pDeckData->IsDrawMainRebarArea(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2) || bDrawAlways)
			{
				Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
			}
		}
	}

	if(bEnd)
	{
		vector <REBAR_PLAN>* vPlan = pDeckData->GetMainRebarArrAtSkew(FALSE, bUpper, nDrawSide);
		if(vPlan==NULL)		return;

		for(long n=0; n<(long)vPlan->size(); n++)
		{
			REBAR_PLAN rebarPlan = vPlan->at(n);
			
			if(bTrimRebar == FALSE)
			{
				Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
			}
			else if(pDeckData->IsDrawMainRebarArea(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2) || bDrawAlways)
			{
				Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
			}
		}
	}
	
	*pDom << Dom;
}




// 사보강철근 구하기
void CDrawDBBaseRebarPlan::DrawSabogangRebar(CDomyun *pDom, BOOL bUpper, long nDrawSide, BOOL bStt, BOOL bEnd, BOOL bDrawAlways)
{
	CADeckData *pDeckData	= m_pDataMng->GetDeckData();
	
	CDomyun Dom(pDom);	

	if(bStt)
	{
		vector <REBAR_PLAN>* vPlan = pDeckData->GetSabogangArr(TRUE, bUpper, nDrawSide);
		if(vPlan==NULL)		return;

		for(long n=0; n<(long)vPlan->size(); n++)
		{
			REBAR_PLAN rebarPlan = vPlan->at(n);
			long nShape = rebarPlan.m_nShape;
			
			Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
		}
	}

	if(bEnd)
	{
		vector <REBAR_PLAN>* vPlan = pDeckData->GetSabogangArr(FALSE, bUpper, nDrawSide);
		if(vPlan==NULL)		return;

		for(long n=0; n<(long)vPlan->size(); n++)
		{
			REBAR_PLAN rebarPlan = vPlan->at(n);
			long nShape = rebarPlan.m_nShape;
			
			Dom.LineTo(rebarPlan.m_tVector.m_v1, rebarPlan.m_tVector.m_v2);
		}

	}
	*pDom << Dom;
}


BOOL CDrawDBBaseRebarPlan::IsInDrawPostion(double dStaStt, double dStaEnd)
{
	CPlateBridgeApp	*pDB  = m_pDataMng->GetBridge();	
	CADeckData *pDeckData	= m_pDataMng->GetDeckData();

	double dStaBridgeStt = pDB->m_dStationBridgeStt + pDB->m_dLengthUganStt;
	double dLenStt=0, dLenEnd=0;
	for(long n=0; n<pDeckData->m_arrTrimRange.GetSize()-1; n++)
	{		
		if(n%2==1)
		{
			dLenStt += pDeckData->m_arrTrimRange.GetAt(n);
			continue;
		}
		dLenEnd = dLenStt + pDeckData->m_arrTrimRange.GetAt(n+1);				
		if(dStaStt < dStaBridgeStt+dLenStt && dStaEnd > dStaBridgeStt+dLenStt ||
		   dStaStt < dStaBridgeStt+dLenEnd && dStaEnd < dStaBridgeStt+dLenEnd) return TRUE;		

		dLenStt += pDeckData->m_arrTrimRange.GetAt(n);
	}		

	if(dStaStt > dStaBridgeStt+dLenStt) return TRUE;
	return FALSE;
}



void CDrawDBBaseRebarPlan::DrawBeryukRebarOnCenter(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nDrawSide)
{	
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= pDB->GetGirder(-1);
	CLineInfo		*pLine	    = pDB->m_pLine;
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();

	CDomyun Dom(pDom);		

	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	CPlateBasicIndex *pBxSttJijum = NULL;
	CPlateBasicIndex *pBxEndJijum = NULL;

	while (pBx)
	{	
		if(pBx->IsJijum())
		{		
			if(pBx==pBxEnd)
				break;
			
			long nJijum = pBx->GetGirder()->GetNumberJijumByBx(pBx);				 
			pBxSttJijum = pBx;
			if(nJijum == pDB->m_nQtyJigan)
				break;

			pBxEndJijum = pBx->GetGirder()->GetBxOnJijum(nJijum+1);
			CVectorArray vArr;
			pDeckData->GetBeryukRebarDistOnCenter(&vArr, pBxSttJijum, pBxEndJijum, bUpper, nDrawSide);
			
			for(long n=0; n<vArr.GetSize(); n++) 
				pLine->DrawLS(&Dom, vArr[n].x, vArr[n].y, vArr[n].z);			
		}
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}

void CDrawDBBaseRebarPlan::DrawBeryukRebarOnJijum(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nDrawSide)
{	
	CPlateBridgeApp		*pDB   = m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir  = pDB->GetGirder(-1);
	CLineInfo		*pLine = pDB->m_pLine;
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	
	CVectorArray vArr;
	CDomyun Dom(pDom);	
	
	long nLRType = 0;
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	while (pBx)
	{	
		if(pBx->IsJijum())
		{		
			if(pBx == pBxStt)		nLRType =  1;
			else if(pBx == pBxEnd)	nLRType = -1;
			else					nLRType =  0;
	
			long nJijim = pBxStt->GetGirder()->GetNumberJijumByBx(pBx);			
			CVectorArray vArrJijum;
			pDeckData->GetBeryukRebarDistOnJijum(&vArrJijum, nJijim, bUpper ? 0 : 1, nDrawSide, nLRType);
			for(long n=0; n<vArrJijum.GetSize(); n++) 
				pLine->DrawLS(&Dom, vArrJijum[n].x, vArrJijum[n].y, vArrJijum[n].z);			

		}
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}
 

void CDrawDBBaseRebarPlan::DrawBeryukRebarUnderSinchuk(CDomyun *pDom, BOOL bSttSlab, long nDrawSide)
{	
	CPlateBridgeApp		*pDB	    = m_pDataMng->GetBridge();
	CLineInfo		*pLine		= pDB->m_pLine;
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();

	CVectorArray vArr;
	CDomyun Dom(pDom);	

	CVectorArray vArrJijum;
	long nJijim = bSttSlab ? 0 : pDB->m_nQtyJigan;
	pDeckData->GetBeryukRebarDistOnJijum(&vArrJijum, nJijim, 2, nDrawSide);

	for(long n=0; n<vArrJijum.GetSize(); n++) 
	{
		if(IsInDrawPostion(vArrJijum[n].x, vArrJijum[n].y))
			pLine->DrawLS(&Dom, vArrJijum[n].x, vArrJijum[n].y, vArrJijum[n].z);			
	}

	*pDom << Dom;
}


void CDrawDBBaseRebarPlan::DrawTrimLine(CDomyun *pDom, CPtrArray &ObArcArr, CPtrArray &ObDeleteCircleArc, CPtrArray &ObCircleArrCB, CPtrArray &ObCircleArrST, CPtrArray &ObCircleArrOBK, CPtrArray &ObCircleArrDan, CPtrArray &ObCircleArrCant, CDPointArray &xyArrCB, CDPointArray &xyArrST, CDPointArray &xyArrOBK, CVectorArray &xyArrDan, CVectorArray &xyArrCant, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= m_pDataMng->GetBridge()->GetGirder(-1);
	CLineInfoApp    *pLineApp   = (CLineInfoApp*)pDB->m_pLine;
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();	

	double dStaBridgeStt = pDB->m_dStationBridgeStt + pDB->m_dLengthUganStt;

	CDomyun Dom(pDom), TempDom(pDom);
	double dStation = dStaBridgeStt;
	long n = 0;
	for(n=0; n<pDeckData->m_arrTrimRange.GetSize()-1; n++)
	{		
		dStation += pDeckData->m_arrTrimRange.GetAt(n);				
		double DistLeft  = pDB->GetDisSlabLeftAct(dStation) - pDom->Always(20);
		double DistRight = pDB->GetDisSlabRighAct(dStation) + pDom->Always(20);

		if(pBxStt && pBxEnd)
		{
			if(dStation < pBxStt->GetStation() || dStation > pBxEnd->GetStation()) continue;
		}
		CDPoint xy1  = pLineApp->GetXyLineDis(dStation, DistLeft);
		CDPoint xy2  = pLineApp->GetXyLineDis(dStation, DistRight);
		CDPoint vVec = (xy2-xy1).Unit();
		CDPoint xyc  = xy1 + ~(xy2-xy1)/2 * vVec + ((n%2) ? vVec.RotateInv90() : vVec.Rotate90()) * 1000;

		CDPoint xy1Temp  = xy1 + ((n%2) ? vVec.RotateInv90() : vVec.Rotate90()) * 900;
		CDPoint xy2Temp  = xy2 + ((n%2) ? vVec.RotateInv90() : vVec.Rotate90()) * 900;		
		CDPoint xycTemp = xy1Temp + ~(xy2Temp-xy1Temp)/2 * vVec + ((n%2) ? vVec.RotateInv90() : vVec.Rotate90()) * 100;
		Dom.Arc(xy1,xyc,xy2);

		pOpt->SetEnvType(&TempDom,HCAD_HIDD);
		TempDom.Arc(xy1Temp, xycTemp, xy2Temp);
		ObArcArr.Add(Dom.GetLastObject());
		Dom << TempDom;
		ObDeleteCircleArc.Add(TempDom.GetLastObject());
	}		
	// 슬래브 시종점 횡단라인.
	CDPoint AStt[4], AEnd[4];
	pDB->GetXySss(OFF_STT_SLAB, AStt);
	pDB->GetXySss(OFF_END_SLAB, AEnd);
	//
	CRebarPlacing RebarPlace;
	double dExt = 1000;
	double dStaStt = 0;
	double dStaEnd = 0;
	if(pBxStt)	dStaStt = pBxStt->GetStation() - (pBxStt->IsState(BX_STT_JOINT) ? dExt : 0);
	else		dStaStt = pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB)-dExt;
	if(pBxEnd)	dStaEnd = pBxEnd->GetStation() + (pBxEnd->IsState(BX_END_JOINT) ? dExt : 0);
	else		dStaEnd = pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB)+dExt;
	CDPoint vAngStt = pBxStt ? pBxStt->GetAngle() : pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint vAngEnd = pBxEnd ? pBxEnd->GetAngle() : pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);;
	CDPoint xySttLeft = pDB->m_pLine->GetXyLineDisByAng(dStaStt, pDB->GetDisSlabLeftAct(dStaStt)-2000, vAngStt);
	CDPoint xySttRigh = pDB->m_pLine->GetXyLineDisByAng(dStaStt, pDB->GetDisSlabRighAct(dStaStt)+2000, vAngStt);
	CDPoint xyEndLeft = pDB->m_pLine->GetXyLineDisByAng(dStaEnd, pDB->GetDisSlabLeftAct(dStaEnd)-2000, vAngEnd);
	CDPoint xyEndRigh = pDB->m_pLine->GetXyLineDisByAng(dStaEnd, pDB->GetDisSlabRighAct(dStaEnd)+2000, vAngEnd);
	
	CDPointArray xyArrPoly;
	xyArrPoly.Add(xySttLeft);
	xyArrPoly.Add(xySttRigh);
	xyArrPoly.Add(xyEndRigh);
	xyArrPoly.Add(xyEndLeft);
	xyArrPoly.Add(xySttLeft);
	
	// 가로보 상세원 정의
	if(pDeckData->IsBridgeHasCrossBeam() && pDB->m_dThickSlabHunch > 0 )
	{
		for(n=0; n<pDeckData->m_arrSangsePosCB.GetSize(); n++)
		{
			CDPoint xyPosCB = pDeckData->m_arrSangsePosCB[n];
			long nG  = (long)pDeckData->m_arrSangseIndexCB.GetAt(n).x;
			long nCB = (long)pDeckData->m_arrSangseIndexCB.GetAt(n).y;
			CDPoint xyResult1, xyResult2;

			BOOL bCheckIn = RebarPlace.CheckXyByPolygon(xyPosCB, xyArrPoly, TRUE);
			if(!bCheckIn)	continue;

			if( nCB == 0 || nCB == pDB->GetGirder(nG)->GetQtyCrossBeamVBracing()-1)	// 단부(시/종점) 가로보 일경우
			{
				if (nCB == 0)
				{
					GetXyMatchSegAndCircle(AStt[0], AStt[3], xyPosCB,  pDeckData->m_dDiaSangsePosCB/2, xyResult1, xyResult2);
					CDPoint xyMid = xyPosCB + (AStt[0]-AStt[3]).Unit().RotateInv90()*pDeckData->m_dDiaSangsePosCB/2;
					Dom.Arc(xyResult1, xyMid, xyResult2);
				}
				else
				{
					GetXyMatchSegAndCircle(AEnd[0], AEnd[3], xyPosCB,  pDeckData->m_dDiaSangsePosCB/2, xyResult1, xyResult2);
					CDPoint xyMid = xyPosCB + (AEnd[0]-AEnd[3]).Unit().Rotate90()*pDeckData->m_dDiaSangsePosCB/2;
					Dom.Arc(xyResult1, xyMid, xyResult2);
				}
			}
			else	// 일반부 가로보 일경우
			{
				Dom.Circle(pDeckData->m_arrSangsePosCB[n], pDeckData->m_dDiaSangsePosCB/2);
			}

			ObCircleArrCB.Add(Dom.GetLastObject());
			xyArrCB.Add(pDeckData->m_arrSangseIndexCB[n]);
		}
	}
	// 세로보 상세원 정의
	if(pDeckData->IsBridgeHasStringer())
	{
		for(n=0; n<pDeckData->m_arrSangsePosST.GetSize(); n++)
		{
			CDPoint xyPosST = pDeckData->m_arrSangsePosST[n];

			BOOL bCheckIn = RebarPlace.CheckXyByPolygon(xyPosST, xyArrPoly, TRUE);
			if(!bCheckIn)	continue;

			Dom.Circle(pDeckData->m_arrSangsePosST[n], pDeckData->m_dDiaSangsePosST/2);

			ObCircleArrST.Add(Dom.GetLastObject());
			xyArrST.Add(pDeckData->m_arrSangseIndexST[n]);
		}
	}
	// 외측가로보 상세원 정의
	if(pDeckData->IsBridgeHasOutBracket() && pDB->m_dThickSlabHunch > 0 )
	{
		for(n=0; n<pDeckData->m_arrSangsePosOBK.GetSize(); n++)
		{
			CDPoint xyPosOBK = pDeckData->m_arrSangsePosOBK[n];

			BOOL bCheckIn = RebarPlace.CheckXyByPolygon(xyPosOBK, xyArrPoly, TRUE);
			if(!bCheckIn)	continue;

			Dom.Circle(pDeckData->m_arrSangsePosOBK[n], pDeckData->m_dDiaSangsePosOBK/2);

			ObCircleArrOBK.Add(Dom.GetLastObject());
			xyArrOBK.Add(pDeckData->m_arrSangseIndexOBK[n]);
		}
	}
	// 단부 상세원 정의
	for(n=0; n<pDeckData->m_arrSangsePosDan.GetSize(); n++)
	{
		CVector vecPosDan = pDeckData->m_arrSangsePosDan[n];
		CDPoint xyPosDan = pDeckData->m_arrSangsePosDan[n];
		BOOL    bSttSlab = vecPosDan.z==-1 ? TRUE : FALSE;
		CDPoint xyResult1, xyResult2;

		BOOL bCheckIn = RebarPlace.CheckXyByPolygon(xyPosDan, xyArrPoly, TRUE);
		if(!bCheckIn)	continue;

		if (bSttSlab)
		{
			GetXyMatchSegAndCircle(AStt[0], AStt[3], xyPosDan,  pDeckData->m_dDiaSangsePosDan/2, xyResult1, xyResult2);
			if(!xyResult1.IsNull() && !xyResult2.IsNull())
			{
				CDPoint xyMid = xyPosDan + (AStt[0]-AStt[3]).Unit().RotateInv90()*pDeckData->m_dDiaSangsePosDan/2;
				Dom.Arc(xyResult1, xyMid, xyResult2);
			}
		}
		else
		{
			GetXyMatchSegAndCircle(AEnd[0], AEnd[3], xyPosDan,  pDeckData->m_dDiaSangsePosDan/2, xyResult1, xyResult2);
			if(!xyResult1.IsNull() && !xyResult2.IsNull())
			{
				CDPoint xyMid = xyPosDan + (AEnd[0]-AEnd[3]).Unit().Rotate90()*pDeckData->m_dDiaSangsePosDan/2;
				Dom.Arc(xyResult1, xyMid, xyResult2);
			}
		}

		ObCircleArrDan.Add(Dom.GetLastObject());
		xyArrDan.Add(pDeckData->m_arrSangsePosDan[n]);
	}
	// 캔틸레버부 상세원 (선형을 변환)
	CTwinVectorArray tvArr;
	CTwinVector tv;
	CDPointArray xyArrMatch;
	CVectorArray vecArr;
	double dStaPick = 0;	//  현재 위치에서의 스테이션

	for(n=0; n<pDeckData->m_arrSangsePosCant.GetSize(); n++)
	{
		CVector PosCant   = pDeckData->m_arrSangsePosCant[n];
		BOOL    bLeftSlab = PosCant.z == -1 ? TRUE : FALSE;
		CDPoint xyPosCant = CDPoint(PosCant.x, PosCant.y);
		pLineApp->GetStationMatchVerticalByXy(xyPosCant, dStaPick);
		CDPoint vLine = pLineApp->GetAngleAzimuth(dStaPick);

		BOOL bCheckIn = RebarPlace.CheckXyByPolygon(xyPosCant, xyArrPoly, TRUE);
		if(!bCheckIn)	continue;
		
		if(bLeftSlab)
		{
			tv.m_v1   = xyPosCant + vLine.RotateInv90()*pDeckData->m_dDiaSangsePosCant/2;
			tv.m_v2   = xyPosCant + vLine.Rotate(CDPoint(0.1, -1).Unit())*pDeckData->m_dDiaSangsePosCant/2;
		}
		else
		{
			tv.m_v1   = xyPosCant + vLine.Rotate90()*pDeckData->m_dDiaSangsePosCant/2;
			tv.m_v2   = xyPosCant + vLine.Rotate(CDPoint(-0.1, 1).Unit())*pDeckData->m_dDiaSangsePosCant/2;
		}
		tv.m_v1.z = -1;
		tv.m_v2.z = -pDeckData->m_dDiaSangsePosCant/2;

		double dDist = bLeftSlab ? pDB->GetDisSlabLeftAct(dStaPick) - 300 : pDB->GetDisSlabRighAct(dStaPick) + 300;
		CRebarPlacing RebarPlace;
		tvArr.AddFromLineInfo(pLineApp, pGirCen->GetStationOnJijum(0), pGirCen->GetStationOnJijum(pDB->m_nQtyJigan), dDist);
		tvArr.GetToVectorArray(vecArr);
		RebarPlace.GetXyMatchTvArrAndTv(tvArr, tv, xyArrMatch, FALSE);

		// xyArrMatch(교차점)을 선형방향으로 순서변경
		double dStaMatch1 = 0, dStaMatch2 = 0;
		CDPoint xyMatchStt, xyMatchEnd;
		if(xyArrMatch.GetSize()>1)
		{
			pLineApp->GetStationMatchVerticalByXy(xyArrMatch[0], dStaMatch1);
			pLineApp->GetStationMatchVerticalByXy(xyArrMatch[1], dStaMatch2);
			if(dStaMatch1<dStaMatch2)
			{
				xyMatchStt = xyArrMatch[0];
				xyMatchEnd = xyArrMatch[1];
			}
			else
			{
				xyMatchStt = xyArrMatch[1];
				xyMatchEnd = xyArrMatch[0];
			}

			if(( PosCant.z == -1 || PosCant.z == 1) && xyArrMatch.GetSize() > 1)	// 좌측:-1, 우측:1
			{
				if (PosCant.z == -1)
				{
					CDPoint xyMid = xyPosCant + (xyMatchEnd-xyMatchStt).Unit().RotateInv90()*pDeckData->m_dDiaSangsePosCant/2;
					Dom.Arc(xyMatchStt, xyMid, xyMatchEnd);
				}
				else
				{
					CDPoint xyMid = xyPosCant + (xyMatchEnd-xyMatchStt).Unit().Rotate90()*pDeckData->m_dDiaSangsePosCant/2;
					Dom.Arc(xyMatchStt, xyMid, xyMatchEnd);
				}
			}
			ObCircleArrCant.Add(Dom.GetLastObject());
			xyArrCant.Add(pDeckData->m_arrSangsePosCant[n]);
		}
	}	
	*pDom << Dom;
}



void CDrawDBBaseRebarPlan::DrawSangseStudBagang(CDomyun *pDom, long nJigan)
{
	CPlateBridgeApp		*pDB	   = m_pDataMng->GetBridge();
	CADeckData			*pDeckData = m_pDataMng->GetDeckData();
	CPlateGirderApp		*pGirCen   = pDB->GetGirder(-1);

	if(!pDeckData->m_bInstallShearRebar)	return;

	double DistLeft		 = pDeckData->GetDistByJongBase(JONGBASEPOS_SLABLEFT);
	double DistRight	 = pDeckData->GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
	double dStaSlabStt  = pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
	double dRadTrim		= pDB->GetWidthAct(dStaSlabStt)/10;
	
	CDomyun Dom(pDom);
	
	double dStaL = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB),DistLeft,pGirCen->GetAAA(OFF_STT_SLAB));
	double dStaR = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB),DistRight,pGirCen->GetAAA(OFF_STT_SLAB));
	CDPoint xy1  = pGirCen->GetXyGirderDis(dStaL, DistLeft);
	CDPoint xy2  = pGirCen->GetXyGirderDis(dStaR, DistRight);
	CDPoint xyCutStt = xy1 + (xy2-xy1).Unit() * ~(xy2-xy1) * 3/4;
	
	DrawStudBogangRebarByJigan(&Dom, nJigan);

	dStaL = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB),DistLeft,pGirCen->GetAAA(OFF_END_SLAB));
	dStaR = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB),DistRight,pGirCen->GetAAA(OFF_END_SLAB));
	xy1  = pGirCen->GetXyGirderDis(dStaL, DistLeft);
	xy2  = pGirCen->GetXyGirderDis(dStaR, DistRight);
	CDPoint xyCutEnd = xy1 + (xy2-xy1).Unit() * ~(xy2-xy1) * 3/4;

	if(pDB->m_dHeightTaperStt > 0)
	{
		Dom.TrimByCircle2(xyCutStt, dRadTrim, FALSE);		
	}
	if(pDB->m_dHeightTaperEnd > 0)
	{
		Dom.TrimByCircle2(xyCutEnd, dRadTrim, FALSE);		
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarPlan::DrawSangseUnderSinchuk(CDomyun *pDom, BOOL bSttSlab, long nDrawSide)
{
	CPlateBridgeApp		*pDB	   = m_pDataMng->GetBridge();
	CPlateGirderApp		*pGirLeft  = pDB->GetGirder(0);
	CPlateGirderApp		*pGirRight = pDB->GetGirder(pDB->GetGirdersu()-1);

	
	CDomyun Dom(pDom);

	if(bSttSlab)
	{
		DrawBeryukRebarUnderSinchuk(&Dom, bSttSlab, nDrawSide);
		DrawMainRebarUnderSinchuk(&Dom, bSttSlab, nDrawSide);	
	}
	else
	{
		DrawBeryukRebarUnderSinchuk(&Dom, bSttSlab, nDrawSide);
		DrawMainRebarUnderSinchuk(&Dom, bSttSlab, nDrawSide);	
	}

	*pDom << Dom;
}


void CDrawDBBaseRebarPlan::RemoveByLineFence(CDomyun *pDom, long nIndex)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	
	CPlateGirderApp *pGirCen = pDB->GetGirder(-1);
	double DistLeft		 = pDeckData->GetDistByJongBase(JONGBASEPOS_SLABLEFT);
	double DistRight	 = pDeckData->GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
	double dStaBridgeStt = pDB->m_dStationBridgeStt + pDB->m_dLengthUganStt;
	double dStaBridgeEnd = dStaBridgeStt + pDB->m_dLengthTotalSlab;
	
	double dStation = dStaBridgeStt;
	for(long n=0; n<pDeckData->m_arrTrimRange.GetSize()-1; n++)
	{		
		dStation += pDeckData->m_arrTrimRange.GetAt(n);
		CDPoint xy1  = pGirCen->GetXyGirderDis(dStation, DistLeft);
		CDPoint xy2  = pGirCen->GetXyGirderDis(dStation, DistRight);
		CDPoint vVec = (xy2-xy1).Unit();
		CDPoint xyc = xy1 + ~(xy2-xy1)/2 * vVec + ((n%2) ? vVec.RotateInv90() : vVec.Rotate90()) * 1000;
		if(nIndex==n)
		{		
			if(n%2)
			{
				CDPoint xyStt1 = pGirCen->GetXyGirderDis(dStaBridgeStt, DistLeft);
				CDPoint xyStt2 = pGirCen->GetXyGirderDis(dStaBridgeStt, DistRight);
				CDPoint vVecStt = (xyStt2-xyStt1).Unit();
				CDPoint xycStt = xyStt1 + ~(xyStt2-xyStt1)/2 * vVecStt;				
#ifdef _DEBUG
				pDom->LineTo(xyc, xycStt);
#endif

				pDom->RemoveByLineFence(xyc, xycStt);			
			}
			else
			{			
				CDPoint xyEnd1 = pGirCen->GetXyGirderDis(dStaBridgeEnd, DistLeft);
				CDPoint xyEnd2 = pGirCen->GetXyGirderDis(dStaBridgeEnd, DistRight);
				CDPoint vVecEnd = (xyEnd2-xyEnd1).Unit();
				CDPoint xycEnd = xyEnd1 + ~(xyEnd2-xyEnd1)/2 * vVecEnd;			
#ifdef _DEBUG
				pDom->LineTo(xyc, xycEnd);
#endif
				pDom->RemoveByLineFence(xyc, xycEnd);						
			}		
		}
	}		
}


void CDrawDBBaseRebarPlan::DrawMark(CDomyun *pDom, long nJigan)
{
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CPlateBridgeApp *pDB 	   = m_pDataMng->GetBridge();
	
	pDeckData->MakeRebarMarkPosPlan(nJigan, pDom->GetScaleDim());

	// 상부
	if(pDeckData->IsSameMainRebarDanGen(TRUE))
		DrawMarkMainNoDiv(pDom, nJigan, TRUE);
	else
		DrawMarkMainDiv(pDom, nJigan, TRUE);

	// 하부
	BOOL bLeftBrk = pDeckData->IsBridgeHasOutBracket(SECTION_DAN, TRUE);
	BOOL bRighBrk = pDeckData->IsBridgeHasOutBracket(SECTION_DAN, FALSE);
	BOOL bNoBrk   = !bLeftBrk&&!bRighBrk ? TRUE : FALSE;
	BOOL bNoHunch = pDeckData->IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&pDB->GetGirdersu()==1) ? TRUE : FALSE;
	BOOL bTaperDiv= pDB->m_dHeightTaperStt>0 || (pDB->m_dHeightTaperStt==0&&!pDeckData->IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;

	if( bNoHunch && !bTaperDiv)	
		DrawMarkMainNoDiv(pDom, nJigan, FALSE);
	else
		DrawMarkMainDiv(pDom, nJigan, FALSE);

	DrawMarkMainLowerEtc(pDom, nJigan);
	DrawMarkMainSkew(pDom, nJigan, TRUE);
	DrawMarkMainSkew(pDom, nJigan, FALSE);
	DrawMarkBeryuk(pDom, nJigan);
}



void CDrawDBBaseRebarPlan::DrawMarkSangseDanbu(CDomyun *pDom, long nJigan)
{
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB   = m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt	   = m_pDataMng->GetOptionStd();
	CRebar			*pRB	   = NULL;

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CRebarPlacing	RebarPlacing;

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText, szRebar;
	CDPointArray	xyArr, xyArrTemp;
	double dLengthExt = 0;
	
	for (long n=0; n<pDeckData->m_arrSangsePosDan.GetSize(); n++)
	{
		CDPoint xyCen = CDPoint(pDeckData->m_arrSangsePosDan[n]);
		
		if(!pDeckData->IsPointInJigan(xyCen, nJigan))	continue;	// 현재 상세 위치가 현재 지간에 있는지 검사..
		BOOL bStt = pDeckData->m_arrSangsePosDan[n].z==-1 ? TRUE: FALSE;		// 상세위치가 슬래브 좌측이면 하면철근을 표현함

		pRB  = pDeckData->GetRebarByName("배력단부하면1");
		szText.Format("배력단부하면1-%d", n);
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
		CDPoint vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(pDB->GetGirder(-1)->GetSSS(bStt?OFF_STT_SLAB:OFF_END_SLAB));
		if (xyArr.GetSize()>0)
		{
			xyArrTemp.RemoveAll();
			xyArrTemp.Add(xyArr.GetAt(0));
			xyArrTemp.Add(xyArr.GetAt(0)+vGir*100);
			if(bStt)
				dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, TRUE);
			else
				dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, FALSE);
		}
		else	
			dLengthExt = 0;

		if(pRB)	DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;
		long nArrID = long(xyArr.GetSize()/2);
		if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, bStt?6:7, dLengthExt, nArrID, FALSE);
		*pDom << Dom;
	}
	

}


void CDrawDBBaseRebarPlan::DrawMarkSangseCant(CDomyun *pDom, long nJigan)
{
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CARoadOptionStd	*pOpt	   = m_pDataMng->GetOptionStd();
	CRebar			*pRB	   = NULL;

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CRebarPlacing	RebarPlacing;

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText, szRebar;
	CDPointArray	xyArr, xyArrTot;
	double dLengthExt = 0;

	for (long n=0; n<pDeckData->m_arrSangsePosCant.GetSize(); n++)
	{
		CDPoint xyCen = CDPoint(pDeckData->m_arrSangsePosCant[n]);
		if(!pDeckData->IsPointInJigan(xyCen, nJigan))	continue;	// 현재 상세 위치가 현재 지간에 있는지 검사..

		BOOL bUpperRebar = pDeckData->m_arrSangsePosCant[n].z==-1 ? FALSE: TRUE;
	
		long nSection = 0;
		if(pDeckData->IsSameMainRebarDanGen())	nSection = SECTION_DAN;
		else									nSection = SECTION_CEN;				

		long nCyclesu = pDeckData->GetRebarCycleSuBySection(nSection);
		long nCycle	= 0;
		for (nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			szText.Format("캔틸레버상세%d_주철근%dCycle", n+1, nCycle+1);
			CDPointArray xyArrTemp = pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			for (long nTemp=0; nTemp<xyArrTemp.GetSize(); nTemp++)
			{
				xyArrTot.Add(xyArrTemp[nTemp]);
			}
		}
		xyArrTot.Sort();
		
		for ( nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			szText.Format("캔틸레버상세%d_주철근%dCycle", n+1, nCycle+1);
			REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);
			long nShapesu = bUpperRebar ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();
			long nShape   = pDeckData->m_arrSangsePosCant[n].z==-1 ? 0 : nShapesu-1;

			if(pDeckData->IsSameMainRebarDanGen())
			{
				BOOL bOneCycle = pDeckData->m_stMainCycleDan.size()==1 ? TRUE : FALSE;

				if(bOneCycle)		szRebar.Format(bUpperRebar ? "단부상면%d" : "단부하면%d", nShape+1);
				else				szRebar.Format(bUpperRebar ? "단부상면%d-%dCycle" : "단부하면%d-%dCycle", nShape+1, nCycle+1);
			}
			else
			{
				BOOL bOneCycle = pDeckData->m_stMainCycleCen.size()==1 ? TRUE : FALSE;

				if(bOneCycle)	
				{
					if(bUpperRebar)
						szRebar.Format("일반부상면%d", nShape+1);
					else
						szRebar.Format("일반부외측거더하면좌측");
				}
				else
				{
					if(bUpperRebar)
						szRebar.Format("일반부상면%d-%dCycle", nShape+1, nCycle+1);
					else
						szRebar.Format("일반부외측거더하면좌측");
				}
			}

			pRB  = pDeckData->GetRebarByName(szRebar);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			if(pRB)	DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), bUpperRebar?FALSE:TRUE);
			if(pRB)	DrawRebarMark2(&Dom, xyArr, xyArrTot, pRB->m_sMark, pRB->GetStringDia(), bUpperRebar?5:4, FALSE, dLengthExt, long(xyArr.GetSize()/4)+nCycle*2, TRUE);
			
			*pDom << Dom;
		}
	}

}


// 단부, 일반부 주철근 상이 할 경우
void CDrawDBBaseRebarPlan::DrawMarkMainDiv(CDomyun *pDom, long nJigan, BOOL bUpper)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CRebar			*pRB		= NULL;

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CRebarPlacing	RebarPlacing;

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText, szRebar;
	CDPointArray	xyArr;
	double dLengthExt = 0;

	//////////////////////////////////////////////////////////////////////////
	// 주철근 - 상면
	BOOL bOneCycle = pDeckData->m_stMainCycleDan.size()==1 ? TRUE : FALSE;
	BOOL bOneJigan = pDB->m_nQtyJigan > 1 ? FALSE : TRUE;
	long nCyclesu = pDeckData->m_stMainCycleDan.size();

	if(bOneJigan)	
	{
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			// 시점단부
			REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(SECTION_DAN, nCycle);
			long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();
			long nShape = 0;
			for (nShape=0; nShape<nShapesu; nShape++)
			{
				BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
				if(bValid==FALSE)
					continue;
			
				if(bUpper)		szText.Format("단부상면STT%d-%dCycle", nShape+1, nCycle+1);
				else			szText.Format("단부하면STT%d-%dCycle", nShape+1, nCycle+1);
				
				if(bOneCycle)		szRebar.Format(bUpper ? "단부상면%d" : "단부하면%d", nShape+1);
				else				szRebar.Format(bUpper ? "단부상면%d-%dCycle" : "단부하면%d-%dCycle", nShape+1, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				if(pRB)	DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;
				
				if(xyArr.GetSize()>0)
					dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), TRUE, TRUE);
				if(pRB)	DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, TRUE, dLengthExt, -1, TRUE);
				*pDom << Dom;
			}				
			// 종점단부
			pRebarCycle = pDeckData->GetRebarCycleBySection(SECTION_DAN, nCycle);
			nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

			for ( nShape=0; nShape<nShapesu; nShape++)
			{
				BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
				if(bValid==FALSE)
					continue;
				
				if(bUpper)		szText.Format("단부상면END%d-%dCycle", nShape+1, nCycle+1);
				else			szText.Format("단부하면END%d-%dCycle", nShape+1, nCycle+1);
				
				if(bOneCycle)		szRebar.Format(bUpper ? "단부상면%d" : "단부하면%d", nShape+1);
				else				szRebar.Format(bUpper ? "단부상면%d-%dCycle" : "단부하면%d-%dCycle", nShape+1, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				if(pRB)	DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;
	
				if(xyArr.GetSize()>0)
					dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), FALSE, FALSE);
				if(pRB)	DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, TRUE);
				*pDom << Dom;
			}
		}
	}
	else
	{
		if(nJigan==0 || nJigan==pDB->m_nQtyJigan-1)	// 시작, 끝지간에서만...
		{
			for(long nCycle=0; nCycle<nCyclesu; nCycle++)
			{
				REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(nJigan==0?SECTION_DAN:SECTION_DAN, nCycle);
				long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

				for (long nShape=0; nShape<nShapesu; nShape++)
				{
					BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
					if(bValid==FALSE)
						continue;

					if(bUpper)			szText.Format("단부상면%d-%dCycle", nShape+1, nCycle+1);
					else				szText.Format("단부하면%d-%dCycle", nShape+1, nCycle+1);	
					
					pRB  = pDeckData->GetRebarByName(szText);
					xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
					if(pRB)	DrawRebarMarkLine(&Dom, xyArr);
					*pDom << Dom;
					
					if (nJigan==0)	// 시점
					{
						if(xyArr.GetSize()>0)
								dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), TRUE, TRUE);
						if(pRB)	DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, TRUE, dLengthExt, -1, TRUE);
						*pDom << Dom;

					}
					else	// 종점
					{
						if(xyArr.GetSize()>0)
							dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), FALSE, FALSE);
						if(pRB)	DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, TRUE);
						*pDom << Dom;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//  일반부 주철근
	bOneCycle = pDeckData->m_stMainCycleCen.size()==1 ? TRUE : FALSE;
	BOOL bLeftBrk = pDeckData->IsBridgeHasOutBracket(SECTION_CEN, TRUE);
	BOOL bRighBrk = pDeckData->IsBridgeHasOutBracket(SECTION_CEN, FALSE);
	BOOL bNoBrk   = !bLeftBrk&&!bRighBrk ? TRUE : FALSE;
	BOOL bNoHunch = pDeckData->IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&pDB->GetGirdersu()==1) ? TRUE : FALSE;
	BOOL bTaperDiv= pDB->m_dHeightTaperStt>0 || (pDB->m_dHeightTaperStt==0&&!pDeckData->IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;

	if(bUpper || !bUpper&&(bNoHunch && bTaperDiv))
	{
		for(long nCycle=0; nCycle<(long)pDeckData->m_stMainCycleCen.size(); nCycle++)
		{
			REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(SECTION_CEN, nCycle);
			long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();
			for (long nShape=0; nShape<nShapesu; nShape++)
			{
				BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
				if(bValid==FALSE)
					continue;

				szText.Format("일반부%s면STT%d-%dCycle", bUpper?"상":"하", nShape+1, nCycle+1);

				if(bOneCycle)		szRebar.Format("일반부%s면%d", bUpper?"상":"하", nShape+1);
				else				szRebar.Format("일반부%s면%d-%dCycle", bUpper?"상":"하", nShape+1, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				if(pRB)	DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;

				if(xyArr.GetSize()>0)
					dLengthExt = pDeckData->GetLengthExtendToSlabCenFrPoint(xyArr, nJigan, Dom.Always(5), FALSE, TRUE);

				if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, FALSE, dLengthExt, -1, TRUE);
				*pDom << Dom;

				szText.Format("일반부%s면END%d-%dCycle", bUpper?"상":"하", nShape+1, nCycle+1);

				if(bOneCycle)		szRebar.Format("일반부%s면%d", bUpper?"상":"하", nShape+1);
				else				szRebar.Format("일반부%s면%d-%dCycle", bUpper?"상":"하", nShape+1, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				if(pRB)	DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;

				if(xyArr.GetSize()>0)
					dLengthExt = pDeckData->GetLengthExtendToSlabCenFrPoint(xyArr, nJigan, Dom.Always(5), TRUE, FALSE);

				if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, FALSE, dLengthExt, -1, TRUE);
				*pDom << Dom;
			}
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 일반부외측거더하면
		for(long nCycle=0; nCycle<(long)pDeckData->m_stMainCycleCen.size(); nCycle++)
		{
			for (long nSttEnd=0; nSttEnd<2; nSttEnd++)
			{
				if (nSttEnd==0)		szText.Format("일반부외측거더하면STT");
				else				szText.Format("일반부외측거더하면END");

				pRB  = pDeckData->GetRebarByName(pDB->GetGirdersu()==1?"일반부외측거더하면좌측":"일반부외측거더하면우측");
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;
				if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), nSttEnd==0?3:2, FALSE, Dom.Always(5), -1, TRUE);
				*pDom << Dom;
			}
		}
	}
}



void CDrawDBBaseRebarPlan::DrawMarkMainNoDiv(CDomyun *pDom, long nJigan, BOOL bUpper)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CRebar			*pRB		= NULL;

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CRebarPlacing	RebarPlacing;

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText, szRebar;
	CDPointArray	xyArr;
	double dLengthExt = 0;

	//////////////////////////////////////////////////////////////////////////
	// 주철근 - 상면
	long nSection  = SECTION_DAN;	// 철근유무만 판단.
	long nCyclesu  = pDeckData->GetRebarCycleSuBySection(nSection);
	BOOL bOneCycle = pDeckData->m_stMainCycleDan.size()==1 ? TRUE : FALSE;
	
	for(long nCycle=0; nCycle<nCyclesu; nCycle++)
	{
		REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);
		long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

		for (long nShape=0; nShape<nShapesu; nShape++)
		{
			BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
			if(bValid==FALSE)
				continue;

			if(bUpper)		szText.Format("전체상면STT%d-%dCycle", nShape+1, nCycle+1);
			else			szText.Format("전체하면STT%d-%dCycle", nShape+1, nCycle+1);

			if(bOneCycle)		szRebar.Format(bUpper ? "단부상면%d" : "단부하면%d", nShape+1);
			else				szRebar.Format(bUpper ? "단부상면%d-%dCycle" : "단부하면%d-%dCycle", nShape+1, nCycle+1);

			pRB  = pDeckData->GetRebarByName(szRebar);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;

			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabCenFrPoint(xyArr, nJigan, Dom.Always(5), FALSE, TRUE);

			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, TRUE);
			*pDom << Dom;
			
			if(bUpper)		szText.Format("전체상면END%d-%dCycle", nShape+1, nCycle+1);
			else			szText.Format("전체하면END%d-%dCycle", nShape+1, nCycle+1);

			if(bOneCycle)		szRebar.Format(bUpper ? "단부상면%d" : "단부하면%d", nShape+1);
			else				szRebar.Format(bUpper ? "단부상면%d-%dCycle" : "단부하면%d-%dCycle", nShape+1, nCycle+1);

			pRB  = pDeckData->GetRebarByName(szRebar);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;

			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabCenFrPoint(xyArr, nJigan, Dom.Always(5), TRUE, FALSE);
			
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, TRUE, dLengthExt, -1, TRUE);
			*pDom << Dom;

		}
	}
}



void CDrawDBBaseRebarPlan::DrawMarkMainLowerEtc(CDomyun *pDom, long nJigan)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CRebar			*pRB		= NULL;
	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CRebarPlacing	RebarPlacing;

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText, szRebar;
	CDPointArray	xyArr;
	
	//////////////////////////////////////////////////////////////////////////
	// 주철근 - 상면
	long nLoopStt = 0;
	long nLoopEnd = 2;
	
	//////////////////////////////////////////////////////////////////////////
	// 일반부중앙거더하면
	if(pDB->GetGirdersu() > 2)
	{
		for (long nSttEnd=nLoopStt; nSttEnd<nLoopEnd; nSttEnd++)
		{
			if (nSttEnd==0)		szText.Format("일반부중앙거더하면STT");
			else				szText.Format("일반부중앙거더하면END");		
			
			pRB  = pDeckData->GetRebarByName("일반부중앙거더하면");
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), nSttEnd==0?3:2, FALSE, Dom.Always(5), -1, TRUE);
			*pDom << Dom;
		}
	}
	// 일반부 중앙가로보부 하면
	if(pDB->GetGirdersu() > 1)
	{
		for (long nSttEnd=nLoopStt; nSttEnd<nLoopEnd; nSttEnd++)
		{
			if (nSttEnd==0)		szText.Format("일반부중앙가로보부하면STT");
			else				szText.Format("일반부중앙가로보부하면END");		
			
			pRB  = pDeckData->GetRebarByName("일반부중앙가로보부하면");
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), nSttEnd==0?3:2, FALSE, Dom.Always(5), -1, TRUE);
			*pDom << Dom;
		}
	}

/*	CPlateBasicIndex *pBx	= pDB->GetGirder(0)->GetBxOnJijum(0);
	CPlateBasicIndex *pBxL	= pBx->GetBxMatchByCrossBeam(pDB->GetGirder(0));
	CPlateBasicIndex *pBxR	= pBx->GetBxMatchByCrossBeam(pDB->GetGirder(pDB->GetGirdersu()-1));
	CBracket	*pBrkL	= pBxL->GetBracket(TRUE); 
	CBracket	*pBrkR	= pBxR->GetBracket(FALSE); 
	
	//////////////////////////////////////////////////////////////////////////
	// 외측가로보 상부, 외측빔 헌치보강(양측, 우측만 존재시..) 좌측만 존재시는 추후 작업
	if ((pBrkL && pBrkR) || (pBrkL==NULL && pBrkR))
	{
		for (long nSttEnd=nLoopStt; nSttEnd<nLoopEnd; nSttEnd++)
		{
			if (nSttEnd==0)
			{
				if(pBrkL && pBrkR)		szText.Format("외측가로보상부STT");
				else					szText.Format("외측가로보상부(우)STT");
			}
			else
			{
				if(pBrkL && pBrkR)		szText.Format("외측가로보상부END");
				else					szText.Format("외측가로보상부(우)END");
			}

			pRB  = pDeckData->GetRebarByName("일반부외측가로보상부");
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), nSttEnd==0?3:2, FALSE, Dom.Always(5), -1, TRUE);
			*pDom << Dom;

			if (nSttEnd==0)
			{
				if(pBrkL && pBrkR)		szText.Format("외측빔헌치보강STT");
				else					szText.Format("외측빔헌치보강(우)STT");
			}
			else
			{
				if(pBrkL && pBrkR)		szText.Format("외측빔헌치보강END");
				else					szText.Format("외측빔헌치보강(우)END");
			}

			pRB  = pDeckData->GetRebarByName("일반부외측빔헌치보강");
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), nSttEnd==0?3:2, FALSE, Dom.Always(5), -1, TRUE);
			*pDom << Dom;
		}
	}*/
}



void CDrawDBBaseRebarPlan::DrawMarkMainSkew(CDomyun *pDom, long nJigan, BOOL bUpper)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CRebar			*pRB		= NULL;
	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CRebarPlacing	RebarPlacing;

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText, szRebar;
	CDPointArray	xyArr;
	double dLengthExt = 0;
	BOOL bOneCycle = pDeckData->m_stMainCycleDan.size()==1 ? TRUE : FALSE;
	long nSection  = nJigan==0 ? SECTION_SKEW_STT : SECTION_SKEW_END;
	
	//////////////////////////////////////////////////////////////////////////
	// 사보강철근
	//////////////////////////////////////////////////////////////////////////
	if(nJigan==0 || nJigan==pDB->m_nQtyJigan-1)
	{
		CDPoint	vAngStt = pDB->GetGirder(-1)->GetAngleStt();
		CDPoint	vAngEnd = pDB->GetGirder(-1)->GetAngleEnd();

		for(long nCycle=0; nCycle<(long)pDeckData->m_stMainCycleSabogangStt.size(); nCycle++)
		{
			if(nJigan==0)
			{
				REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);
				long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

				for (long nShape=0; nShape<nShapesu; nShape++)
				{
					BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
					if(bValid==FALSE)
						continue;

					if(bOneCycle)		szText.Format("시점사보강상면%d", nShape+1);
					else				szText.Format("시점사보강상면%d-%dCycle", nShape+1, nCycle+1);
					pRB  = pDeckData->GetRebarByName(szText);
					xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
					DrawRebarMarkLine(&Dom, xyArr);
					*pDom << Dom;

					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), TRUE, TRUE);
					if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, TRUE, dLengthExt, -1, TRUE);
					*pDom << Dom;

					if(bOneCycle)		szText.Format("시점사보강하면%d", nShape+1);
					else				szText.Format("시점사보강하면%d-%dCycle", nShape+1, nCycle+1);
					pRB  = pDeckData->GetRebarByName(szText);
					xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
					DrawRebarMarkLine(&Dom, xyArr);
					*pDom << Dom;

					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), TRUE, TRUE);
					if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, TRUE, dLengthExt, -1, TRUE);
					*pDom << Dom;
				}
			}
			
			if(pDB->m_nQtyJigan==1 || nJigan == pDB->m_nQtyJigan-1)
			{
				vector<pair<CString, CDPointArray> > vArrMain;
				REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);
				long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

				for (long nShape=0; nShape<nShapesu; nShape++)
				{
					BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
					if(bValid==FALSE)
						continue;

					if(bOneCycle)		szText.Format("종점사보강상면%d", nShape+1);
					else				szText.Format("종점사보강상면%d-%dCycle", nShape+1, nCycle+1);
					pRB  = pDeckData->GetRebarByName(szText);
					xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
					DrawRebarMarkLine(&Dom, xyArr);
					*pDom << Dom;

					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), TRUE, FALSE);
					if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, FALSE);
					*pDom << Dom;

					if(bOneCycle)		szText.Format("종점사보강하면%d", nShape+1);
					else				szText.Format("종점사보강하면%d-%dCycle", nShape+1, nCycle+1);
					pRB  = pDeckData->GetRebarByName(szText);
					xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
					DrawRebarMarkLine(&Dom, xyArr);
					*pDom << Dom;

					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), TRUE, FALSE);
					if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, FALSE);
					*pDom << Dom;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 사각부주철근
	//////////////////////////////////////////////////////////////////////////
	BOOL bAllSpan = nJigan==pDB->m_nQtyJigan-1? TRUE : FALSE;

	CPlateGirderApp *pGirCen = pDB->GetGirder(-1);
	for (long nStt =0; nStt <= iEND; nStt++)
	{
		BOOL bSttSlab = nStt == iSTT? TRUE :FALSE;
		long nCyclesu = pDeckData->GetRebarCycleSuBySection(SECTION_DAN);

		if(nJigan == 0 && bSttSlab == FALSE && bAllSpan == FALSE) continue;
		if(nJigan == pDB->m_nQtyJigan-1 && bSttSlab && bAllSpan == FALSE) continue;	

		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			//사각부 주철근
			REBAR_CYCLE *pRebarCycle = pDeckData->GetRebarCycleBySection(SECTION_DAN, nCycle);
			long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();
			long nShape	  = 0;
			for (nShape=0; nShape<nShapesu; nShape++)
			{
				BOOL bValid = (BOOL)(bUpper ? pRebarCycle->m_dArrValidUp[nShape] : pRebarCycle->m_dArrValidDn[nShape]);
				if(bValid==FALSE)
					continue;

				if(bUpper)
					bSttSlab? szText.Format("시점사각주철근상면%d-%dCycle", nShape+1, nCycle+1) : szText.Format("종점사각주철근상면%d-%dCycle", nShape+1, nCycle+1);
				else			
					bSttSlab? szText.Format("시점사각주철근하면%d-%dCycle", nShape+1, nCycle+1) : szText.Format("종점사각주철근하면%d-%dCycle", nShape+1, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szText);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				
				if(pRB && xyArr.GetSize()>0)
				{
					dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), FALSE , bSttSlab? TRUE: FALSE);
					DrawRebarMarkLine(&Dom, xyArr);
					if(bSttSlab)
						DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2 , TRUE, dLengthExt);
					else
						DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, TRUE);
				}					

				*pDom << Dom;
			}							
		}
	}


	/*
	CPlateGirderApp *pGirCen = pDB->GetGirder(-1);

	if(nJigan==0)
	{
		szText.Format("시점사각주철근상면");
		pRB  = pDeckData->GetRebarByName(szText);
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;
		
		if(xyArr.GetSize()>0)
			dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), FALSE, TRUE);
		if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, TRUE, dLengthExt);
		*pDom << Dom;

		szText.Format("시점사각주철근하면");
		pRB  = pDeckData->GetRebarByName(szText);
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;

		if(xyArr.GetSize()>0)
			dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), FALSE, TRUE);
		if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, TRUE, dLengthExt);
		*pDom << Dom;
	}
	if(pDB->m_nQtyJigan==1 || nJigan == pDB->m_nQtyJigan-1)
	{
		szText.Format("종점사각주철근상면");
		pRB  = pDeckData->GetRebarByName(szText);
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;
		
		if(xyArr.GetSize()>0)
			dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), FALSE, FALSE);
		if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, TRUE);
		
		*pDom << Dom;

		szText.Format("종점사각주철근하면");
		pRB  = pDeckData->GetRebarByName(szText);
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;

		if(xyArr.GetSize()>0)
			dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArr, Dom.Always(5), FALSE, FALSE);
		if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, dLengthExt, -1, TRUE);
		*pDom << Dom;
	}
	*/
}



void CDrawDBBaseRebarPlan::DrawMarkBeryuk(CDomyun *pDom, long nJigan)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CRebar			*pRB		= NULL;
	CDomyun			Dom(pDom);

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText, szRebar;
	CDPointArray	xyArr, xyArrTemp;
	double dLengthExt = 0;
	//////////////////////////////////////////////////////////////////////////////
	// 배력단부
	//////////////////////////////////////////////////////////////////////////////
	BOOL bOneCycle = pDeckData->m_stBeryukCycleLeft.size()==1 ? TRUE : FALSE;
	CDPoint vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(pDB->GetGirder(-1)->GetBxOnJijum(0)->GetStation());
	long nBeryukCycleSize = pDeckData->m_stBeryukCycleLeft.size() > 2 ? 2 : pDeckData->m_stBeryukCycleLeft.size();
	double dSta = 0;

	//////////////////////////////////////////////////////////////////////////////
	// 시점부
	//////////////////////////////////////////////////////////////////////////////
	long nRebarSuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT, TRUE, TRUE);
	long nRebarSuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, TRUE, TRUE);
	long nRebarSuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH, TRUE, TRUE);
	long nCycle = 0;
	if (pDB->m_nQtyJigan==1 || (pDB->m_nQtyJigan>1 && nJigan==0))	
	{
		for(nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			for (long nLRC=0; nLRC<2; nLRC++)
			{
				long   nPos = nLRC==0 ? POS_SLAB_LEFT : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";
    
				if(bOneCycle)		szRebar.Format("배력단부상면%s", szPos);
				else				szRebar.Format("배력단부상면%s%dCycle", szPos, nCycle+1);
				if(bOneCycle)		szText.Format("배력단부상면%sSTT", szPos);
				else				szText.Format("배력단부상면%sSTT%dCycle", szPos, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;
		
				if(nLRC==1 && (nRebarSuL>0 || nRebarSuR>0))
				{
					long nArrID = long(xyArr.GetSize()/2);

					if(xyArr.GetSize()>0)
					{
						xyArrTemp.RemoveAll();
						xyArrTemp.Add(xyArr.GetAt(nArrID));
						pDB->m_pLine->GetStationMatchVerticalByXy(xyArr.GetAt(nArrID), dSta);
						vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(dSta);
						xyArrTemp.Add(xyArr.GetAt(nArrID)+vGir*100);
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, TRUE);
					}
					else dLengthExt = 0;

					if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 6, dLengthExt, nArrID, FALSE);
				}
				else
				{
					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(5), TRUE);

					if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 0, dLengthExt, -1, TRUE);
				}
				*pDom << Dom;
			}
		}

		nRebarSuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT, TRUE, FALSE);
		nRebarSuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, TRUE, FALSE);
		nRebarSuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH, TRUE, FALSE);

		pRB  = pDeckData->GetRebarByName("배력단부하면1");
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "배력단부하면1STT");
		if (xyArr.GetSize()>0)
		{
			xyArrTemp.RemoveAll();
			xyArrTemp.Add(xyArr.GetAt(0));
			xyArrTemp.Add(xyArr.GetAt(0)+pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB))*100);
			dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, TRUE);
		}
		else	dLengthExt = 0;
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;

		long nArrID = long(xyArr.GetSize()/2);
		if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 6, dLengthExt, nArrID, FALSE);
		*pDom << Dom;
		
		for( nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			for (long nLRC=0; nLRC<2; nLRC++)
			{
				long   nPos = nLRC==0 ? POS_SLAB_LEFT : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";

				if(bOneCycle)		szRebar.Format("배력단부하면2%sCycle", szPos);
				else				szRebar.Format("배력단부하면2%s-%dCycle", szPos, nCycle+1);
				if(bOneCycle)		szText.Format("배력단부하면2%sSTTCycle", szPos);
				else				szText.Format("배력단부하면2%sSTT-%dCycle", szPos, nCycle+1);
				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;

				if(nLRC==1 && (nRebarSuL>0 || nRebarSuR>0))
				{
					long nArrID = long(xyArr.GetSize()/2);

					if(xyArr.GetSize()>0)
					{
						xyArrTemp.RemoveAll();
						xyArrTemp.Add(xyArr.GetAt(nArrID));
						pDB->m_pLine->GetStationMatchVerticalByXy(xyArr.GetAt(nArrID), dSta);
						vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(dSta);
						xyArrTemp.Add(xyArr.GetAt(nArrID)+vGir*100);
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, TRUE);
					}
					else dLengthExt = 0;

					if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 6, dLengthExt, nArrID, FALSE);
				}
				else
				{
					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), FALSE);

					if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(),  1, FALSE, dLengthExt, -1, TRUE);
				}
				*pDom << Dom;
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// 종점부
	//////////////////////////////////////////////////////////////////////////////
	vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(pDB->GetGirder(-1)->GetBxOnJijum(pDB->m_nQtyJigan)->GetStation());
	if (pDB->m_nQtyJigan==1 || (pDB->m_nQtyJigan>1 && nJigan==pDB->m_nQtyJigan-1))	// 종점부 
	{
		nRebarSuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT, TRUE, TRUE);
		nRebarSuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, TRUE, TRUE);
		nRebarSuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH, TRUE, TRUE);
		for(nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			for (long nLRC=0; nLRC<2; nLRC++)
			{
				long   nPos = nLRC==0 ? POS_SLAB_LEFT : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";
				
				if(bOneCycle)		szRebar.Format("배력단부상면%s", szPos);
				else				szRebar.Format("배력단부상면%s%dCycle", szPos, nCycle+1);
				if(bOneCycle)		szText.Format("배력단부상면%sEND", szPos);
				else				szText.Format("배력단부상면%sEND%dCycle", szPos, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;

				if(nLRC==1 && (nRebarSuL>0 || nRebarSuR>0))
				{
					long nArrID = long(xyArr.GetSize()/2);

					if(xyArr.GetSize()>0)
					{
						xyArrTemp.RemoveAll();
						xyArrTemp.Add(xyArr.GetAt(nArrID));
						pDB->m_pLine->GetStationMatchVerticalByXy(xyArr.GetAt(nArrID), dSta);
						vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(dSta);

						xyArrTemp.Add(xyArr.GetAt(nArrID) - vGir*100);
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, FALSE);
					}
					else dLengthExt = 0;

					if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 7, dLengthExt, nArrID, FALSE);
				}
				else
				{
					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(5), TRUE);

					if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 0, dLengthExt, -1, TRUE);
				}
				*pDom << Dom;
			}
		}
			
		//////////////////////////////////////////////////////////////////////////
		nRebarSuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT, TRUE, FALSE);
		nRebarSuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, TRUE, FALSE);
		nRebarSuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH, TRUE, FALSE);
		
		pRB  = pDeckData->GetRebarByName("배력단부하면1");
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "배력단부하면1END");
		if (xyArr.GetSize()>0)
		{
			xyArrTemp.RemoveAll();
			xyArrTemp.Add(xyArr.GetAt(0));
			xyArrTemp.Add(xyArr.GetAt(0)+pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB))*100);
			dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, FALSE);
		}
		else	dLengthExt = 0;
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;
		
		long nArrID = long(xyArr.GetSize()/2);
		if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 7, dLengthExt, nArrID, TRUE);
		*pDom << Dom;

		//////////////////////////////////////////////////////////////////////////
		for( nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			for (long nLRC=0; nLRC<2; nLRC++)
			{
				long   nPos = nLRC==0 ? POS_SLAB_RIGH : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_LEFT);
				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";

				if(bOneCycle)		szRebar.Format("배력단부하면2%sCycle", szPos);
				else				szRebar.Format("배력단부하면2%s-%dCycle", szPos, nCycle+1);
				if(bOneCycle)		szText.Format("배력단부하면2%sENDCycle", szPos);
				else				szText.Format("배력단부하면2%sEND-%dCycle", szPos, nCycle+1);

				pRB  = pDeckData->GetRebarByName(szRebar);
				xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
				DrawRebarMarkLine(&Dom, xyArr);
				*pDom << Dom;

				if(nLRC==1 && (nRebarSuL>0 || nRebarSuR>0))
				{
					long nArrID = long(xyArr.GetSize()/2);

					if(xyArr.GetSize()>0)
					{
						xyArrTemp.RemoveAll();
						xyArrTemp.Add(xyArr.GetAt(nArrID));
						pDB->m_pLine->GetStationMatchVerticalByXy(xyArr.GetAt(nArrID), dSta);
						vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(dSta);

						xyArrTemp.Add(xyArr.GetAt(nArrID) - vGir*100);
						dLengthExt = pDeckData->GetLengthExtendToSlabSttEndFrPoint(xyArrTemp, Dom.Always(5), TRUE, FALSE);
					}
					else dLengthExt = 0;

					if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, 7, dLengthExt, nArrID, FALSE);
				}
				else
				{
					if(xyArr.GetSize()>0)
						dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), FALSE);

					if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(),  1, FALSE, dLengthExt, -1, TRUE);
				}
				*pDom << Dom;
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// 배력일반부
	//////////////////////////////////////////////////////////////////////////////
	bOneCycle		  = pDeckData->m_stBeryukCycleLeft.size()==1 ? TRUE : FALSE;
	long nRealCyclesu = pDeckData->GetsuRealCycle(JONGBASEPOS_SLABCENTER, TRUE);
	long nBesu		  = pDeckData->GetBesu(JONGBASEPOS_SLABCENTER, TRUE);
	CPtrArray ObArcArray, ObArcForDeleteArray, ObCircleArrayCB, ObCircleArrayST, ObCircleArrayObk, ObCircleArrayDan, ObCircleArrayCant;
	CDPointArray xyArrCB, xyArrST, xyArrOBK;
	CVectorArray xyArrDan, xyArrCant;

	CDomyun	DomTemp(pDom);
	DrawTrimLine(&DomTemp, ObArcArray, ObArcForDeleteArray, ObCircleArrayCB, ObCircleArrayST, ObCircleArrayObk, ObCircleArrayDan, ObCircleArrayCant, xyArrCB, xyArrST, xyArrOBK, xyArrDan, xyArrCant);

	CDPoint xyResult1, xyResult2;
	
	nRebarSuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT, FALSE, TRUE);
	nRebarSuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, FALSE, TRUE);
	nRebarSuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH, FALSE, TRUE);

	for(nCycle=0; nCycle<nRealCyclesu; nCycle++)
	{
		long nCycleCur = nCycle*nBesu;
		for (long nLRC=0; nLRC<2; nLRC++)
		{
			long   nPos = nLRC==0 ? POS_SLAB_LEFT : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
			CString szPos("");
			if(nPos==POS_SLAB_LEFT)			szPos = "L";
			else if(nPos==POS_SLAB_RIGH)	szPos = "R";
			else							szPos = "";

			if(bOneCycle)		szText.Format("배력일반부상면%s%d", szPos, nJigan+1);
			else				szText.Format("배력일반부상면%s%d-%dCycle", szPos, nJigan+1, nCycleCur+1);

			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;

			if(nLRC==1 && (nRebarSuL>0 || nRebarSuR>0))
			{
				long nArrID = long(xyArr.GetSize()/2);

				if(xyArr.GetSize()>0)
				{
					xyArrTemp.RemoveAll();
					xyArrTemp.Add(xyArr.GetAt(nArrID));
					pDB->m_pLine->GetStationMatchVerticalByXy(xyArr.GetAt(nArrID), dSta);
					vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(dSta);

					xyArrTemp.Add(xyArr.GetAt(nArrID) + vGir*(nJigan==0?1:-1)*100);
					
					CObArc *pArc = nJigan==0 ? (CObArc *)ObArcArray.GetAt(0) : (CObArc *)ObArcArray.GetAt(ObArcArray.GetUpperBound());
					GetXyMatchLineAndArc(xyArrTemp[0], (xyArrTemp[1]-xyArrTemp[0]).Unit(), pArc->m_SttPoint, pArc->m_Radius, pArc->GetStartAngle(FALSE), pArc->GetEndAngle(FALSE), xyResult1, xyResult2);
					dLengthExt = xyResult1.x==0 ? ~(xyResult2-xyArrTemp[0]) : ~(xyResult1-xyArrTemp[0]);
					if(dLengthExt > 50000)
						dLengthExt = 0;
				}
				else dLengthExt = 0;
			

				if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, nJigan==0 ? 7 : 6, dLengthExt, nArrID, FALSE);
			}
			else
			{
				if(xyArr.GetSize()>0)
					dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(5), TRUE);

				if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, dLengthExt, -1, TRUE);
			}
			*pDom << Dom;
		}
	}

	nRebarSuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT, FALSE, FALSE);
	nRebarSuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, FALSE, FALSE);
	nRebarSuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH, FALSE, FALSE);
		
	nRealCyclesu = pDeckData->GetsuRealCycle(JONGBASEPOS_SLABCENTER, FALSE);
	nBesu = pDeckData->GetBesu(JONGBASEPOS_SLABCENTER, FALSE);
	for( nCycle=0; nCycle<nRealCyclesu; nCycle++)
	{
		long nCycleCur = nCycle*nBesu;
		for (long nLRC=0; nLRC<2; nLRC++)
		{
			long   nPos = nLRC==0 ? POS_SLAB_RIGH : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_LEFT);
			CString szPos("");
			if(nPos==POS_SLAB_LEFT)			szPos = "L";
			else if(nPos==POS_SLAB_RIGH)	szPos = "R";
			else							szPos = "";

			if(bOneCycle)		szText.Format("배력일반부하면%s%d", szPos, nJigan+1);
			else				szText.Format("배력일반부하면%s%d-%dCycle", szPos, nJigan+1, nCycleCur+1);

			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			
			if(nLRC==1 && (nRebarSuL>0 || nRebarSuR>0))
			{
				long nArrID = long(xyArr.GetSize()/2);

				if(xyArr.GetSize()>0)
				{
					xyArrTemp.RemoveAll();
					xyArrTemp.Add(xyArr.GetAt(nArrID));
					pDB->m_pLine->GetStationMatchVerticalByXy(xyArr.GetAt(nArrID), dSta);
					vGir = pDB->GetGirder(-1)->GetLine()->GetAngleAzimuth(dSta);
					xyArrTemp.Add(xyArr.GetAt(nArrID)+vGir*(nJigan==0?1:-1)*100);
					CObArc *pArc = nJigan==0 ? (CObArc *)ObArcArray.GetAt(0) : (CObArc *)ObArcArray.GetAt(ObArcArray.GetUpperBound());
					GetXyMatchLineAndArc(xyArrTemp[0], (xyArrTemp[1]-xyArrTemp[0]).Unit(), pArc->m_SttPoint, pArc->m_Radius, pArc->GetStartAngle(FALSE), pArc->GetEndAngle(FALSE), xyResult1, xyResult2);
					dLengthExt = xyResult1.x==0 ? ~(xyResult2-xyArrTemp[0]) : ~(xyResult1-xyArrTemp[0]);
					if(dLengthExt > 50000)
						dLengthExt = 0;
				}
				else dLengthExt = 0;

				if(pRB)		DrawRebarMarkByAngle(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), vGir, nJigan==0 ? 7 : 6, dLengthExt, nArrID, FALSE);
			}
			else
			{
				if(xyArr.GetSize()>0)
					dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), FALSE);

				if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, dLengthExt, -1, TRUE);
			}
			*pDom << Dom;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// 배력지점부
	//////////////////////////////////////////////////////////////////////////////
	if(pDB->m_nQtyJigan < 2)	return;
	
	bOneCycle = pDeckData->m_stBeryukCycleLeft.size()==1 ? TRUE : FALSE;
	nBeryukCycleSize = pDeckData->m_stBeryukCycleLeft.size() > 2 ? 2 : pDeckData->m_stBeryukCycleLeft.size();
	if(nJigan==0)
	{
		for(nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			if(bOneCycle)		szText.Format("배력지점부상면%d", nJigan+1);
			else				szText.Format("배력지점부상면%d-%dCycle", nJigan+1, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(5), TRUE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, TRUE, dLengthExt, -1, TRUE);
			*pDom << Dom;

			//////////////////////////////////////////////////////////////////////////
			if(bOneCycle)		szText.Format("배력지점부하면%d", nJigan+1);
			else				szText.Format("배력지점부하면%d-%dCycle", nJigan+1, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), FALSE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, dLengthExt, -1, TRUE);
			*pDom << Dom;
		}
	}
	else if(nJigan==pDB->m_nQtyJigan-1)
	{
		for(nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			if(bOneCycle)		szText.Format("배력지점부상면%d", nJigan);
			else				szText.Format("배력지점부상면%d-%dCycle", nJigan, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(5), TRUE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, TRUE, dLengthExt, -1, TRUE);
			*pDom << Dom;

			//////////////////////////////////////////////////////////////////////////		
			if(bOneCycle)		szText.Format("배력지점부하면%d", nJigan);
			else				szText.Format("배력지점부하면%d-%dCycle", nJigan, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), FALSE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, dLengthExt, -1, TRUE);
			*pDom << Dom;
		}

	}
	else
	{
		for(nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			if(bOneCycle)		szText.Format("배력지점부상면%d", nJigan);
			else				szText.Format("배력지점부상면%d-%dCycle", nJigan, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(5), TRUE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, TRUE, dLengthExt, -1, TRUE);
			*pDom << Dom;
			
			//////////////////////////////////////////////////////////////////////////
			if(bOneCycle)		szText.Format("배력지점부하면%d", nJigan);
			else				szText.Format("배력지점부하면%d-%dCycle", nJigan, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), FALSE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, dLengthExt, -1, TRUE);
			*pDom << Dom;
		}
		
		for(nCycle=0; nCycle<nBeryukCycleSize; nCycle++)
		{
			if(bOneCycle)		szText.Format("배력지점부상면%d", nJigan+1);
			else				szText.Format("배력지점부상면%d-%dCycle", nJigan+1, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(5), TRUE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, TRUE, dLengthExt, -1, TRUE);
			*pDom << Dom;

			//////////////////////////////////////////////////////////////////////////
			if(bOneCycle)		szText.Format("배력지점부하면%d", nJigan+1);
			else				szText.Format("배력지점부하면%d-%dCycle", nJigan+1, nCycle+1);
			pRB  = pDeckData->GetRebarByName(szText);
			xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, szText);
			DrawRebarMarkLine(&Dom, xyArr);
			*pDom << Dom;
			if(xyArr.GetSize()>0)
				dLengthExt = pDeckData->GetLengthExtendToSlabLeftRighFrPoint(xyArr[0], Dom.Always(10), FALSE);
			if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, dLengthExt, -1, TRUE);
			*pDom << Dom;
		}
	}

}


void CDrawDBBaseRebarPlan::DrawMarkCB_ST_OBK(CDomyun *pDom, long nJigan)
{
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CRebar			*pRB		= NULL;
	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CDPointArray	xyArr;
	
	//////////////////////////////////////////////////////////////////////////
	pRB  = pDeckData->GetRebarByName("가로보하면보강");
	xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "가로보하면보강");
	DrawRebarMarkLine(&Dom, xyArr);
	*pDom << Dom;
	if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 4, TRUE, pDeckData->m_dDiaSangsePosCB/4+Dom.Always(3), 0, TRUE);
	*pDom << Dom;
	//////////////////////////////////////////////////////////////////////////
	
	pRB  = pDeckData->GetRebarByName("가로보헌치보강");
	xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "가로보헌치보강");
	DrawRebarMarkLine(&Dom, xyArr);
	*pDom << Dom;
	if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, TRUE, Dom.Always(5));
	*pDom << Dom;
	//////////////////////////////////////////////////////////////////////////
	
	if(pDeckData->m_bInstallCrossBeamUpperBogangRebar)
	{
		pRB  = pDeckData->GetRebarByName("가로보상면보강");
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "가로보상면보강");
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;
		if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, Dom.Always(5));
		*pDom << Dom;
	}
	//////////////////////////////////////////////////////////////////////////
	
	pRB  = pDeckData->GetRebarByName("외측가로보하면보강");
	xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "외측가로보하면보강");
	DrawRebarMarkLine(&Dom, xyArr);
	*pDom << Dom;
	if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 4, TRUE, pDeckData->m_dDiaSangsePosOBK/4+Dom.Always(3), 0, TRUE);
	*pDom << Dom;
	//////////////////////////////////////////////////////////////////////////
	pRB  = pDeckData->GetRebarByName("외측가로보헌치보강");
	xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "외측가로보헌치보강");
	DrawRebarMarkLine(&Dom, xyArr);
	*pDom << Dom;
	if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, TRUE, Dom.Always(5));
	*pDom << Dom;
	
	//////////////////////////////////////////////////////////////////////////
	if(pDeckData->m_bInstallOutBrkUpperBogangRebar)
	{
		pRB  = pDeckData->GetRebarByName("외측가로보상면보강");
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "외측가로보상면보강");
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;
		if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, Dom.Always(5));
		*pDom << Dom;
	}
	
	//////////////////////////////////////////////////////////////////////////
	pRB  = pDeckData->GetRebarByName("세로보하면보강");
	xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "세로보하면보강");
	DrawRebarMarkLine(&Dom, xyArr);
	*pDom << Dom;
	if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 7, FALSE, pDeckData->m_dDiaSangsePosST/4+Dom.Always(3), 0, TRUE);
	*pDom << Dom;
	
	//////////////////////////////////////////////////////////////////////////
	pRB  = pDeckData->GetRebarByName("세로보헌치보강");
	xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "세로보헌치보강");
	DrawRebarMarkLine(&Dom, xyArr);
	*pDom << Dom;
	if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 4, FALSE, pDeckData->m_dDiaSangsePosST/4+Dom.Always(3), long(xyArr.GetSize()/2), TRUE);
	*pDom << Dom;
	//////////////////////////////////////////////////////////////////////////
	if(pDeckData->m_bInstallStringerUpperBogangRebar)
	{
		pRB  = pDeckData->GetRebarByName("세로보상면보강");
		xyArr= pDeckData->GetArrMarkPosPlanByName(nJigan, "세로보상면보강");
		DrawRebarMarkLine(&Dom, xyArr);
		*pDom << Dom;
		if(pRB)		DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 5, FALSE, pDeckData->m_dDiaSangsePosST/4+Dom.Always(8), long(xyArr.GetSize()/2), TRUE);
		*pDom << Dom;
	}
}



void CDrawDBBaseRebarPlan::DrawRebarMarkLine(CDomyun *pDom, CDPointArray xyArr)
{
	if(xyArr.GetSize() < 2)		return;
	
	CARoadOptionStd	*pOpt	   = m_pDataMng->GetOptionStd();
	CDomyun		Dom(pDom);
	CConcSymbol Sym(pOpt);

	double	dMark   = pDom->Always(1);
	CDPoint	xyStt	= CDPoint(0, 0);
	CDPoint	xyEnd	= CDPoint(0, 0);
	CDPoint	vX		= CDPoint(1, 0);
	CDPoint	vY		= CDPoint(0, 1);
	long	nSize	= xyArr.GetSize();
	CDPoint vAng	= (xyArr.GetAt(1)-xyArr.GetAt(0)).Unit().Rotate(CDPoint(-1,-1).Unit());

	double dRotateAngle = 0;
	long n = 0;
	for(n=0; n < nSize; n++)
	{
		xyStt	= xyArr.GetAt(n)+vAng*dMark;
		xyEnd	= xyArr.GetAt(n)-vAng*dMark;
		dRotateAngle = (xyEnd-xyStt).Unit().GetAngleDegree();

		Sym.RebarCountMarkOne(&Dom, xyArr.GetAt(n));
//#ifdef _DEBUG
//		Dom.LineTo(xyStt, xyEnd);
//#else
//		CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGLEATK", xyArr.GetAt(n).x, xyArr.GetAt(n).y, Dom.GetScaleDim(), dRotateAngle);
//#endif
	}
	for(n=0; n < nSize-1; n++)
		Dom.LineTo(xyArr.GetAt(n), xyArr.GetAt(n+1));

	*pDom << Dom;
}


// nDir = 0	: Upper		nDir = 1 : Lower	nDir = 2 : Left		nDir = 3 : Right --- 직선
// nDir = 4 : Upper		nDir = 5 : Lower	nDir = 6 : Left		nDir = 7 : Right --- Array 중간 마크
void CDrawDBBaseRebarPlan::DrawRebarMark(CDomyun *pDom, CDPointArray xyArr, CString szMark, CString szDia, long nDir, BOOL bLeft, double dExtendL, long nArrIdx, BOOL bReverse)
{
	if(xyArr.GetSize() < 2)		return;
	CRebarPlacing RebarPlacing;
	if(bReverse)
		RebarPlacing.ReverseRebar(xyArr);
	
	CObInsert	*pIns = NULL;
	CDomyun		Dom(pDom);

	CDPoint	xyStt	= CDPoint(0, 0);
	CDPoint	xyEnd	= CDPoint(0, 0);
	CDPoint	vX		= CDPoint(1, 0);
	CDPoint	vY		= CDPoint(0, 1);
	CDPoint xyDir	= CDPoint(0, 0);
	CDPoint vAng	= CDPoint(0, 0);
	double	dScale	= Dom.GetScaleDim();
	long	nSize	= xyArr.GetSize();
	long	nMarkLen= szMark.GetLength();

	switch(nDir)
	{
		case 0 :
			xyStt		= xyArr.GetAt(nSize-1);
			xyEnd		= xyArr.GetAt(nSize-1)+vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nSize-1]-xyArr[nSize-2]).Unit();
			vAng = vY.BetweenAngle(xyDir);
			Dom.Rotate(xyArr[nSize-1], vAng);
			break;

		case 1 :
			xyStt		= xyArr.GetAt(nSize-1);
			xyEnd		= xyArr.GetAt(nSize-1)-vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20), xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nSize-1]-xyArr[nSize-2]).Unit();
			vAng = CDPoint(0,-1).BetweenAngle(xyDir);
			Dom.Rotate(xyArr[nSize-1], vAng);
			break;

		case 2 :
			xyStt		= xyArr.GetAt(nSize-1);
			xyEnd		= xyArr.GetAt(nSize-1) - vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns		= CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00,  CDPoint(xyEnd.x-Dom.Always(20), xyEnd.y), dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nSize-1]-xyArr[nSize-2]).Unit();
			vAng = vX.BetweenAngle(xyDir);
			Dom.Rotate(xyArr[nSize-1], -vAng);
			break;

		case 3 :
			xyStt		= xyArr.GetAt(0);
			xyEnd		= xyStt + vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns		= CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00,  xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[0]-xyArr[1]).Unit();
			vAng = xyDir.BetweenAngle(vX);
			Dom.Rotate(-vAng.GetAngleRadian(), xyStt.x, xyStt.y);
			break;

		case 4 :
			if(nArrIdx==-1) nArrIdx=0;
			xyStt		= (xyArr.GetAt(nArrIdx)+xyArr.GetAt(nArrIdx+1))/2;
			xyEnd		= xyStt + vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().RotateInv90();
			vAng  = vY.BetweenAngle(xyDir);
			Dom.Rotate(xyStt, vAng);
			break;

		case 5 :
			if(nArrIdx == -1)	nArrIdx = 0;
			xyStt		= (xyArr.GetAt(nArrIdx)+xyArr.GetAt(nArrIdx+1))/2;
			xyEnd		= xyStt - vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().RotateInv90();
			vAng = vY.BetweenAngle(xyDir);
			Dom.Rotate(xyStt, vAng);
			break;

		case 6 :
			if(nArrIdx==-1) nArrIdx=0;
			xyStt		= (xyArr.GetAt(nArrIdx)+xyArr.GetAt(nArrIdx+1))/2;
			xyEnd		= xyStt - vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().Rotate90();
			vAng  = xyDir.BetweenAngle(-vX);
			Dom.Rotate(-vAng.GetAngleRadian(), xyStt.x, xyStt.y);
			break;

		case 7 :
			if(nArrIdx==-1) nArrIdx=0;
			xyStt		= (xyArr.GetAt(nArrIdx)+xyArr.GetAt(nArrIdx+1))/2;
			xyEnd		= xyStt + vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().Rotate90();
			vAng  = xyDir.BetweenAngle(vX);
			Dom.Rotate(-vAng.GetAngleRadian(), xyStt.x, xyStt.y);
			break;
	}

	*pDom << Dom;
}



// nDir = 0	: Upper		nDir = 1 : Lower	nDir = 2 : Left		nDir = 3 : Right --- 직선
// nDir = 4 : Upper		nDir = 5 : Lower	nDir = 6 : Left		nDir = 7 : Right --- Array 중간 마크
void CDrawDBBaseRebarPlan::DrawRebarMark2(CDomyun *pDom, CDPointArray xyArr, CDPointArray xyArrTot, CString szMark, CString szDia, long nDir, BOOL bLeft, double dExtendL, long nArrIdx, BOOL bReverse)
{
	if(xyArr.GetSize() < 2)		return;
	CRebarPlacing RebarPlacing;
	if(bReverse)
		RebarPlacing.ReverseRebar(xyArr);
	
	CObInsert	*pIns = NULL;
	CDomyun		Dom(pDom);

	CDPoint	xyStt	= CDPoint(0, 0);
	CDPoint	xyEnd	= CDPoint(0, 0);
	CDPoint	vX		= CDPoint(1, 0);
	CDPoint	vY		= CDPoint(0, 1);
	CDPoint xyDir	= CDPoint(0, 0);
	CDPoint vAng	= CDPoint(0, 0);
	double	dScale	= Dom.GetScaleDim();
	long	nSize	= xyArr.GetSize();
	long	nMarkLen= szMark.GetLength();

	switch(nDir)
	{
		case 0 :
			xyStt		= xyArr.GetAt(nSize-1);
			xyEnd		= xyArr.GetAt(nSize-1)+vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nSize-1]-xyArr[nSize-2]).Unit();
			vAng = vY.BetweenAngle(xyDir);
			Dom.Rotate(xyArr[nSize-1], vAng);
			break;

		case 1 :
			xyStt		= xyArr.GetAt(nSize-1);
			xyEnd		= xyArr.GetAt(nSize-1)-vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20), xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nSize-1]-xyArr[nSize-2]).Unit();
			vAng = CDPoint(0,-1).BetweenAngle(xyDir);
			Dom.Rotate(xyArr[nSize-1], vAng);
			break;

		case 2 :
			xyStt		= xyArr.GetAt(nSize-1);
			xyEnd		= xyArr.GetAt(nSize-1) - vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns		= CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00,  CDPoint(xyEnd.x-Dom.Always(20), xyEnd.y), dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nSize-1]-xyArr[nSize-2]).Unit();
			vAng = vX.BetweenAngle(xyDir);
			Dom.Rotate(xyArr[nSize-1], -vAng);
			break;

		case 3 :
			xyStt		= xyArr.GetAt(0);
			xyEnd		= xyStt + vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns		= CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00,  xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[0]-xyArr[1]).Unit();
			vAng = xyDir.BetweenAngle(vX);
			Dom.Rotate(-vAng.GetAngleRadian(), xyStt.x, xyStt.y);
			break;

		case 4 :
			if(nArrIdx==-1) nArrIdx=0;
			xyStt		= (xyArrTot.GetAt(nArrIdx)+xyArrTot.GetAt(nArrIdx+1))/2;
			GetXyMatchSegAndLine(xyArr[0], xyArr[xyArr.GetUpperBound()], xyStt, (xyArrTot.GetAt(nArrIdx)-xyArrTot.GetAt(nArrIdx+1)).Unit().Rotate90(), xyStt);
			xyEnd		= xyStt + vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().RotateInv90();
			vAng  = vY.BetweenAngle(xyDir);
			Dom.Rotate(xyStt, vAng);
			break;

		case 5 :
			if(nArrIdx == -1)	nArrIdx = 0;
			xyStt		= (xyArrTot.GetAt(nArrIdx)+xyArrTot.GetAt(nArrIdx+1))/2;
			GetXyMatchSegAndLine(xyArr[0], xyArr[xyArr.GetUpperBound()], xyStt, (xyArrTot.GetAt(nArrIdx)-xyArrTot.GetAt(nArrIdx+1)).Unit().Rotate90(), xyStt);
			xyEnd		= xyStt - vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			if(bLeft)	pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			else		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().RotateInv90();
			vAng = vY.BetweenAngle(xyDir);
			Dom.Rotate(xyStt, vAng);
			break;

		case 6 :
			if(nArrIdx==-1) nArrIdx=0;
			xyStt		= (xyArrTot.GetAt(nArrIdx)+xyArrTot.GetAt(nArrIdx+1))/2;
			GetXyMatchSegAndLine(xyArr[0], xyArr[xyArr.GetUpperBound()], xyStt, (xyArrTot.GetAt(nArrIdx)-xyArrTot.GetAt(nArrIdx+1)).Unit().Rotate90(), xyStt);
			xyEnd		= xyStt - vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().Rotate90();
			vAng  = xyDir.BetweenAngle(-vX);
			Dom.Rotate(-vAng.GetAngleRadian(), xyStt.x, xyStt.y);
			break;

		case 7 :
			if(nArrIdx==-1) nArrIdx=0;
			xyStt		= (xyArrTot.GetAt(nArrIdx)+xyArrTot.GetAt(nArrIdx+1))/2;
			GetXyMatchSegAndLine(xyArr[0], xyArr[xyArr.GetUpperBound()], xyStt, (xyArrTot.GetAt(nArrIdx)-xyArrTot.GetAt(nArrIdx+1)).Unit().Rotate90(), xyStt);
			xyEnd		= xyStt + vX*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			xyDir = (xyArr[nArrIdx+1] - xyArr[nArrIdx]).Unit().Rotate90();
			vAng  = xyDir.BetweenAngle(vX);
			Dom.Rotate(-vAng.GetAngleRadian(), xyStt.x, xyStt.y);
			break;
	}

	*pDom << Dom;
}




// nDir = 0	: Upper		nDir = 1 : Lower	nDir = 2 : Left		nDir = 3 : Right --- 직선
// nDir = 4 : Upper		nDir = 5 : Lower	nDir = 6 : Left		nDir = 7 : Right --- Array 중간 마크
void CDrawDBBaseRebarPlan::DrawRebarMarkByAngle(CDomyun *pDom, CDPointArray xyArr, CString szMark, CString szDia, CDPoint vAng, long nDir, double dExtendL, long nArrIdx, BOOL bReverse)
{
	if(xyArr.GetSize() < 2)		return;
	CRebarPlacing RebarPlacing;
	if(bReverse)
		RebarPlacing.ReverseRebar(xyArr);
	
	CObInsert	*pIns = NULL;
	CDomyun		Dom(pDom);

	CDPoint	xyStt	= CDPoint(0, 0);
	CDPoint	xyEnd	= CDPoint(0, 0);
	CDPoint xyDir	= CDPoint(0, 0);
	CDPoint	vX		= CDPoint(1, 0);
	CDPoint	vY		= CDPoint(0, 1);
	double	dScale	= Dom.GetScaleDim();
	long	nSize	= xyArr.GetSize();
	long	nMarkLen= szMark.GetLength();
	CDPoint vOrigin = CDPoint(0, 0);

	switch(nDir)
	{
		case 0 :
			xyDir = (xyArr[xyArr.GetUpperBound()]-xyArr[xyArr.GetUpperBound()-1]).Unit();
			xyStt = xyArr.GetAt(nSize-1);
			xyEnd = xyArr.GetAt(nSize-1)+xyDir*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			*pDom << Dom;
			
			if(vAng.GetAngleDegree() <= 180)
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			else
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20),xyEnd.y), dScale);

			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			Dom.Rotate(xyEnd, vAng);
			break;

		case 1 :
			xyDir = (xyArr[xyArr.GetUpperBound()]-xyArr[xyArr.GetUpperBound()-1]).Unit();
			xyStt		= xyArr.GetAt(nSize-1);
			xyEnd		= xyArr.GetAt(nSize-1)-vY*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			*pDom << Dom;

			if(vAng.GetAngleDegree() <= 180)
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			else
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20), xyEnd.y), dScale);

			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			
			Dom.Rotate(xyEnd, vAng);
			break;

		case 6 :
			if(nArrIdx==-1 || nArrIdx < 2) nArrIdx=0;
			xyStt		= (xyArr.GetAt(nArrIdx)+xyArr.GetAt(nArrIdx+1))/2;
			xyEnd		= xyStt - vAng*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			*pDom << Dom;

			if(vAng.GetAngleDegree() <= 90 && vAng.GetAngleDegree() > 270)
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);
			else
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20), xyEnd.y), dScale);

			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			
			Dom.Rotate(xyEnd, vAng);
			break;

		case 7 :
			if(nArrIdx==-1 || nArrIdx < 2) nArrIdx=0;
			xyStt		= (xyArr.GetAt(nArrIdx)+xyArr.GetAt(nArrIdx+1))/2;
			xyEnd		= xyStt + vAng*dExtendL;
			Dom.LineTo(xyStt, xyEnd);
			*pDom << Dom;

			if(vAng.GetAngleDegree() <= 90 && vAng.GetAngleDegree() > 270)
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, CDPoint(xyEnd.x-Dom.Always(20), xyEnd.y), dScale);
			else
				pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, nMarkLen > 3 ? HCAD_E00_KCFIT : HCAD_E00, xyEnd, dScale);

			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", szMark);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_DIA", szDia);
			
			Dom.Rotate(xyEnd, vAng);
			break;
	}

	*pDom << Dom;
}


void CDrawDBBaseRebarPlan::DrawStudBogangRebarByJigan(CDomyun *pDom, long nJigan)
{
	CPlateBridgeApp	*pDB	   = m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARA);
	// 거더가 짝수개이면 절반을 그리고 홀수개이면 중간은 우측만 그림.
	//////////////////////////////////////////////////////////////////////////
	// 단부 스터드보강철근
	if (nJigan==0 || nJigan==pDB->m_nQtyJigan-1)
	{
		if (pDB->GetGirdersu()%2 == 0)
		{
			long nGStt = long(pDB->GetGirdersu()/2);
			for (long nG=nGStt; nG<pDB->GetGirdersu(); nG++)
			{
				if(nJigan==0 || pDB->m_nQtyJigan==1)
				{
					DrawStudBogangRebarAtDanbu(&Dom, nG, TRUE, TRUE);
					DrawStudBogangRebarAtDanbu(&Dom, nG, FALSE, TRUE);
				}
				if(nJigan==pDB->m_nQtyJigan-1 || pDB->m_nQtyJigan==1)
				{
					DrawStudBogangRebarAtDanbu(&Dom, nG, TRUE, FALSE);
					DrawStudBogangRebarAtDanbu(&Dom, nG, FALSE, FALSE);
				}
			}
		}
		else
		{
			long nGStt = long(pDB->GetGirdersu()/2);
			if(nJigan==0 || pDB->m_nQtyJigan==1)
			{
				DrawStudBogangRebarAtDanbu(&Dom, nGStt, FALSE, TRUE);
			}
			if(nJigan==pDB->m_nQtyJigan-1 || pDB->m_nQtyJigan==1)
			{
				DrawStudBogangRebarAtDanbu(&Dom, nGStt, FALSE, FALSE);
			}

			nGStt++;
			
			for (long nG=nGStt; nG<pDB->GetGirdersu(); nG++)
			{
				if(nJigan==0 || pDB->m_nQtyJigan==1)
				{
					DrawStudBogangRebarAtDanbu(&Dom, nG, TRUE, TRUE);
					DrawStudBogangRebarAtDanbu(&Dom, nG, FALSE, TRUE);
				}
				if(nJigan==pDB->m_nQtyJigan-1 || pDB->m_nQtyJigan==1)
				{
					DrawStudBogangRebarAtDanbu(&Dom, nG, TRUE, FALSE);
					DrawStudBogangRebarAtDanbu(&Dom, nG, FALSE, FALSE);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 지점부 스터드보강철근
	if(pDB->m_nQtyJigan==1)	
	{
		*pDom << Dom;
		return;
	}
	
	if (pDB->GetGirdersu()%2 == 0)
	{
		long nGStt = long(pDB->GetGirdersu()/2);
		for (long nG=nGStt; nG<pDB->GetGirdersu(); nG++)
		{
			DrawStudBogangRebarAtCenJijum(&Dom, nG, TRUE, nJigan, TRUE);
			DrawStudBogangRebarAtCenJijum(&Dom, nG, TRUE, nJigan, FALSE);
			DrawStudBogangRebarAtCenJijum(&Dom, nG, FALSE, nJigan, TRUE);
			DrawStudBogangRebarAtCenJijum(&Dom, nG, FALSE, nJigan, FALSE);
		}
	}
	else
	{
		long nGStt = long(pDB->GetGirdersu()/2);
		DrawStudBogangRebarAtCenJijum(&Dom, nGStt, FALSE, nJigan, TRUE);
		DrawStudBogangRebarAtCenJijum(&Dom, nGStt, FALSE, nJigan, FALSE);

		nGStt++;
		
		for (long nG=nGStt; nG<pDB->GetGirdersu(); nG++)
		{
			DrawStudBogangRebarAtCenJijum(&Dom, nG, TRUE, nJigan, TRUE);
			DrawStudBogangRebarAtCenJijum(&Dom, nG, TRUE, nJigan, FALSE);
			DrawStudBogangRebarAtCenJijum(&Dom, nG, FALSE, nJigan, TRUE);
			DrawStudBogangRebarAtCenJijum(&Dom, nG, FALSE, nJigan, FALSE);
		}
	}

	*pDom << Dom;
}


void CDrawDBBaseRebarPlan::DrawStudBogangRebarAtCenJijum(CDomyun *pDom, long nG, BOOL bLeft, long nJigan, BOOL bSttJigan)
{
	CPlateBridgeApp	*pDB	   = m_pDataMng->GetBridge();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	if(!pDeckData->m_bInstallShearRebar)
		return;

	CDomyun Dom(pDom);

	double dDisHStt = pDB->GetGirder(nG)->m_pSangse->m_StudHori_Dis[0];
	CDPointArray xyArrStud = pDeckData->GetPosFirstStud();

	CPlateGirderApp *pGir  = pDB->GetGirder(nG);
	double	dStaJijum = 0;
	CDPoint vAngJijum;
	
	if (bSttJigan)
	{
		dStaJijum = pGir->GetStationOnJijum(nJigan);
		vAngJijum = pGir->GetAngleOnJijum(nJigan);
	}
	else
	{
		dStaJijum = pGir->GetStationOnJijum(nJigan+1);
		vAngJijum = pGir->GetAngleOnJijum(nJigan+1);
	}

	if(pDeckData->m_ShearRebarJijum.GetSize() == 0)		return;

	CDPoint ptStt = pDeckData->m_ShearRebarJijum.GetAt(0)->GetCalcStudCenter();
	CDPoint ptEnd = pDeckData->m_ShearRebarJijum.GetAt(pDeckData->m_ShearRebarJijum.GetSize()-1)->GetCalcStudCenter();
	double  dDis  = ~(ptStt - ptEnd);
	if(dDis==0)		return;

	double dDisJCur = 0;
	double dDisHCur = dDisHStt;

	long nJStudStt = 0;
	long nJStudEnd = 0;

	if (bSttJigan && nJigan>0)
	{
		long nCen = long(pDeckData->m_ShearRebarJijum.GetSize()/2);
		CDPoint ptSttTemp = pDeckData->m_ShearRebarJijum.GetAt(0)->GetCalcStudCenter();
		CDPoint ptEndTemp = pDeckData->m_ShearRebarJijum.GetAt(nCen)->GetCalcStudCenter();
		double  dDisCen  = ~(ptSttTemp - ptEndTemp);
		dDisJCur  = -dDis/2 + dDisCen;
		nJStudStt = long(pDeckData->m_ShearRebarJijum.GetSize()/2);
		nJStudEnd = pDeckData->m_ShearRebarJijum.GetSize();
	}
	else if(bSttJigan==FALSE && nJigan<pDB->m_nQtyJigan-1)
	{
		dDisJCur  = -dDis/2;
		nJStudStt = 0;
		nJStudEnd = long(pDeckData->m_ShearRebarJijum.GetSize()/2);
	}

	CRebarPlacing RebarPlace;
	for (long nJStud = nJStudStt; nJStud < nJStudEnd; nJStud++)
	{
		if(nJStud > nJStudStt)
			dDisJCur += pGir->m_pSangse->m_StudJong_MinDis;

		double dStaStudCur  = pGir->GetStationByActLength(dStaJijum, dDisJCur);
		double dDistStudStt = -pGir->GetWidthFlangeUpper(pGir->GetBxByStation(dStaJijum))/2 - dDisHCur;

		CDPointArray xyArrStudPos; 
		double dDistStudCur = 0;

		for (long n=0; n<xyArrStud.GetSize(); n++)
		{
			dDistStudCur = (dDistStudStt - xyArrStud[n].y);
			xyArrStudPos.Add(pGir->GetXyGirderDis(dStaStudCur, (bLeft?-1:1)*dDistStudCur));
		}

		if (bLeft==FALSE)
			RebarPlace.ReverseRebar(xyArrStudPos);		
	
		CSupportRFBar RfBar;

		RfBar = *pDeckData->m_ShearRebarJijum.GetAt(nJStud);
		RfBar.SetPosStud(xyArrStudPos);
		RfBar.DrawRebar(&Dom);
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarPlan::DrawStudBogangRebarAtDanbu(CDomyun *pDom, long nG, BOOL bLeft, BOOL bSttSlab)
{ 
	CPlateBridgeApp	*pDB	   = m_pDataMng->GetBridge();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	if(!pDeckData->m_bInstallShearRebar)
		return;

	CDomyun Dom(pDom);

	double dDisHStt = pDB->GetGirder(nG)->m_pSangse->m_StudHori_Dis[0];
	double dDisJStt = pDB->GetGirder(nG)->m_pSangse->m_StudJong_BaseDis[0];
	CDPointArray xyArrStud = pDeckData->GetPosFirstStud();

	CPlateGirderApp *pGir  = pDB->GetGirder(nG);
	double dStaGirSttEnd = 0;
	CDPoint vAngSttEnd;
	if(bSttSlab)
	{
		dStaGirSttEnd = pGir->GetSSS(OFF_STT_GIRDER);
		vAngSttEnd = pGir->GetAAA(OFF_STT_GIRDER);
	}
	else
	{
		dStaGirSttEnd = pGir->GetSSS(OFF_END_GIRDER);
		vAngSttEnd = pGir->GetAAA(OFF_END_GIRDER);
	}

	if(pDeckData->m_ShearRebarDanbu.GetSize() == 0)		return;

	double dDisJCur = dDisJStt;
	double dDisHCur = dDisHStt;
	for (long nJStud=0; nJStud<pDeckData->m_ShearRebarDanbu.GetSize(); nJStud++)
	{
		if(nJStud>0)	dDisJCur += pGir->m_pSangse->m_StudJong_MinDis;

		double dStaStudCur  = pGir->GetStationByActLength(dStaGirSttEnd, (bSttSlab?1:-1)*dDisJCur);
		double dDistStudStt = -pGir->GetWidthFlangeUpper(pGir->GetBxByStation(dStaGirSttEnd))/2 - dDisHCur;

		CDPoint vAngStudCur = pGir->GetAngleOffset(dStaGirSttEnd, vAngSttEnd, (bSttSlab?1:-1)*dDisJCur);

		CDPointArray xyArrStudPos; 
		CDPoint xyStt = pGir->GetXyGirderDis(dStaStudCur, (bLeft?-1:1)*dDistStudStt, vAngStudCur);
		xyArrStudPos.Add(xyStt);

		CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(dStaStudCur);
		CDPoint xyStud;
		for (long n=1; n<xyArrStud.GetSize(); n++)
		{
			
			xyStud = xyStt + (bLeft ? vGir.RotateInv90() : vGir.Rotate90()) * xyArrStud[n].y;
			xyArrStudPos.Add(xyStud);
		}
		
		CRebarPlacing rb;
		if (bLeft==FALSE)
			rb.ReverseRebar(xyArrStudPos);	

		CSupportRFBar RfBar;

		RfBar = *pDeckData->m_ShearRebarDanbu.GetAt(nJStud);
		RfBar.SetPosStud(xyArrStudPos);
		RfBar.DrawRebar(&Dom);
	}

	*pDom << Dom;
}


void CDrawDBBaseRebarPlan::DrawJoint(CDomyun *pDom, vector<REBAR_PLAN> *pvPlan, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bMainRebar)
{
	CPlateBridgeApp		*pDB	   = m_pDataMng->GetBridge();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();

	CDomyun Dom(pDom);
	
	CVectorArray xyArrJoint;
	CVector xyStt, xyEnd, xySttMatch, xyEndMatch, vRebar, vJoint, xyResult;
	
	double dExt = 100;
	BOOL bNoBrk   = !pDeckData->IsBridgeHasOutBracket();
	BOOL bNoHunch = pDeckData->IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&pDB->GetGirdersu()==1) ? TRUE : FALSE;
	long n = 0;
	for (n=0; n<(long)pvPlan->size(); n++)
	{
		xyStt = pvPlan->at(n).m_tVector.m_v1;
		xyEnd = pvPlan->at(n).m_tVector.m_v2;
		vRebar= (xyEnd-xyStt).Unit();  
		vJoint= vRebar.Rotate90();
		long m = 0;
		for (m=0; m<(long)pvPlan->size(); m++)
		{
			xySttMatch = pvPlan->at(m).m_tVector.m_v1-vRebar*dExt;
			xyEndMatch = pvPlan->at(m).m_tVector.m_v2+vRebar*dExt;
			long nPos = pvPlan->at(m).m_nPos;
			CString sName = pvPlan->at(m).m_sName;

			if((nPos==0 && !bNoHunch) && sName.Find("상부")==-1)	continue;

			if (n!=m && pvPlan->at(n).m_tVector.m_v1.z==pvPlan->at(m).m_tVector.m_v1.z)
			{
				if(GetXyMatchSegAndLine(xySttMatch, xyEndMatch, xyStt, vJoint, xyResult))
				{
					xyArrJoint.Add(CVector(xyResult.x, xyResult.y, pvPlan->at(m).m_tVector.m_v1.z));
					break;
				}
			}
		}
		for ( m=0; m<(long)pvPlan->size(); m++)
		{
			xySttMatch = pvPlan->at(m).m_tVector.m_v1-vRebar*dExt;
			xyEndMatch = pvPlan->at(m).m_tVector.m_v2+vRebar*dExt;
			long nPos = pvPlan->at(m).m_nPos;
			CString sName = pvPlan->at(m).m_sName;
		
			if((nPos==0 && !bNoHunch) && sName.Find("상부")==-1)	continue;

			if (n!=m && pvPlan->at(n).m_tVector.m_v1.z==pvPlan->at(m).m_tVector.m_v1.z)
			{
				if(GetXyMatchSegAndLine(xySttMatch, xyEndMatch, xyEnd, vJoint, xyResult))
				{
					xyArrJoint.Add(CVector(xyResult.x, xyResult.y, pvPlan->at(m).m_tVector.m_v1.z));
					break;
				}
			}
		}
	}
	
	if(xyArrJoint.GetSize()<2)	
		return;
	
	double dJointExt = 20;

	if(bMainRebar)
	{
		for (n=0; n<xyArrJoint.GetSize()-1; n+=2)
		{
			CVector vecJoint = xyArrJoint[n+1]-xyArrJoint[n];
			vecJoint = vecJoint.Unit();
			CDPoint vAng = vecJoint.Rotate90();

			m_pDataMng->GetOptionStd()->SetEnvType(&Dom ,HCAD_DIML);

			if(!IsLeftOfJijumLine(pBxStt, xyArrJoint[n], xyArrJoint[n+1], TRUE) && 
				IsLeftOfJijumLine(pBxEnd, xyArrJoint[n], xyArrJoint[n+1], FALSE))
			{
				if(pDeckData->IsDrawMainRebarArea(xyArrJoint[n], xyArrJoint[n+1]))
				{
					Dom.MoveTo(xyArrJoint[n]);
					Dom.LineTo(xyArrJoint[n]-vAng.Rotate(CDPoint(1,1).Unit())*dJointExt);
					Dom.LineTo(xyArrJoint[n+1]+vAng.Rotate(CDPoint(-1,1).Unit())*dJointExt);
					Dom.LineTo(xyArrJoint[n+1]);		
				}
			}
		}
	}
	else
	{
		CDPointArray xyArrTemp;	// Dist 종류별...
		for (n=0; n<xyArrJoint.GetSize(); n++)
		{
			CVector vecJoint = xyArrJoint[n];
			xyArrTemp.Add(CDPoint(0, vecJoint.z));
		}
		xyArrTemp.Sort(FALSE);

		CRebarPlacing RebarPlace;
		RebarPlace.RemoveSamePoint(xyArrTemp);	

		// Dist 종류별로 철근분리
		for (n=0; n<xyArrTemp.GetSize(); n++)
		{
			CVectorArray vArr;
			for (long m=0; m<xyArrJoint.GetSize(); m++)
			{
				CDPoint xyTemp = xyArrTemp[n];
				if(xyArrJoint[m].z== xyTemp.y)		// Dist가 동일한 경우
					vArr.Add(xyArrJoint[m]);
			}
			vArr.Sort(0);	// X축으로 Sort함

			for (long nn=0; nn<vArr.GetSize()-1; nn+=2)
			{
				CDPoint vAng = (vArr[nn+1]-vArr[nn]).Unit().Rotate90();

				m_pDataMng->GetOptionStd()->SetEnvType(&Dom ,HCAD_DIML);

				if(!IsLeftOfJijumLine(pBxStt, vArr[nn], vArr[nn+1], TRUE) && 
					IsLeftOfJijumLine(pBxEnd, vArr[nn], vArr[nn+1], FALSE))
				{
					if(pDeckData->IsDrawMainRebarArea(vArr[nn], vArr[nn+1]))
					{
						Dom.MoveTo(vArr[nn]);
						Dom.LineTo(vArr[nn]-vAng.Rotate(CDPoint(1,1).Unit())*dJointExt);
						Dom.LineTo(vArr[nn+1]+vAng.Rotate(CDPoint(-1,1).Unit())*dJointExt);
						Dom.LineTo(vArr[nn+1]);
					}
				}
			}
		}
	}	

  *pDom << Dom;
}



