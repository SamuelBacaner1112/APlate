// DrawDBBaseRebarTU.cpp: implementation of the CDrawDBBaseRebarTU class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawDBBaseRebarTU::CDrawDBBaseRebarTU(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CDrawDBBaseRebarTU::~CDrawDBBaseRebarTU()
{

}

void CDrawDBBaseRebarTU::DrawMainRebarTUAtPlan(CDomyun *pDom, long nG)
	{
	CPlateBridgeApp *pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(nG);
	CADeckDataTU	*pDeckTU= m_pDataMng->GetDeckDataTU();
	
	if(!pDB->IsTUGir())	return;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray *pTvArr = &pDeckTU->m_arrTVectorPlan[nG];
	long nSize = pTvArr->GetSize();

	for (long n=0; n<nSize; n++)
	{
		Dom.LineTo(pTvArr->GetAt(n).m_v1, pTvArr->GetAt(n).m_v2);
	}

	*pDom << Dom;
}

void CDrawDBBaseRebarTU::DrawSabogangRebarTUAtPlan(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(nG);
	CADeckDataTU	*pDeckTU= m_pDataMng->GetDeckDataTU();
	
	if(!pDB->IsTUGir())	return;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	for (BOOL bStt=TRUE; bStt>=FALSE; bStt--)
	{
		CDPoint vAng = pGir->GetAAA(bStt?OFF_STT_SLAB:OFF_END_SLAB);
		double	dStndCuttingAngle	= pDB->m_dGirEndStndGuttingAngle;
		BOOL	bCutAngle90		= (vAng.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
		BOOL	bCutAngle90_Inv	= (vAng.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;

		if(bCutAngle90 || bCutAngle90_Inv)
		{
			CTwinVectorArray *pTvArr = bStt ? &pDeckTU->m_arrTVectorPlanSkewStt_Cut[nG][1] : &pDeckTU->m_arrTVectorPlanSkewEnd_Cut[nG][1];
			long nSize = pTvArr->GetSize();
			long n = 0;
			for (n=0; n<nSize; n++)
			{
				Dom.LineTo(pTvArr->GetAt(n).m_v1, pTvArr->GetAt(n).m_v2);
			}

			pTvArr = bStt ? &pDeckTU->m_arrTVectorPlanSkewStt_Cut[nG][0] : &pDeckTU->m_arrTVectorPlanSkewEnd_Cut[nG][0];
			nSize = pTvArr->GetSize();

			for (n=0; n<nSize; n++)
			{
				Dom.LineTo(pTvArr->GetAt(n).m_v1, pTvArr->GetAt(n).m_v2);
			}
		}
		else
		{
			CTwinVectorArray *pTvArr = bStt ? &pDeckTU->m_arrTVectorPlanSkewStt[nG] : &pDeckTU->m_arrTVectorPlanSkewEnd[nG];
			long nSize = pTvArr->GetSize();

			for (long n=0; n<nSize; n++)
			{
				Dom.LineTo(pTvArr->GetAt(n).m_v1, pTvArr->GetAt(n).m_v2);
			}
		}
	}

	*pDom << Dom;

}

void CDrawDBBaseRebarTU::DrawHunchRebarTUAtPlan(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(nG);
	CADeckDataTU	*pDeckTU= m_pDataMng->GetDeckDataTU();
	
	if(!pDB->IsTUGir())	return;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	for (BOOL bStt=TRUE; bStt>=FALSE; bStt--)
	{
		CTwinVectorArray *pTvArr = bStt ? &pDeckTU->m_arrTVectorPlanHunchStt[nG] : &pDeckTU->m_arrTVectorPlanHunchEnd[nG];

		for (long n=0; n<pTvArr->GetSize(); n++)
			Dom.LineTo(pTvArr->GetAt(n).m_v1, pTvArr->GetAt(n).m_v2);
	}

	*pDom << Dom;
}

void CDrawDBBaseRebarTU::DrawBincConcRebarAtJong(CDomyun *pDom, long nG, BOOL bInput)
{
	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CADeckDataTU		*pDeckTU= m_pDataMng->GetDeckDataTU();
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxFirst(BX_END_GIRDER);
	CLineInfo			*pGirLine	= pDeckTU->GetLineInfo(nG);
	CARoadOptionStd		*pOpt		= m_pDataMng->GetOptionStd();

	CRebarPlacing RebarPlace;
	CDrawDBDeck DeckDraw(m_pDataMng);

	// 구속콘크리트 시점, 종점점
	CDPoint vAngSlabStt		= pGir->GetAAA(OFF_STT_SLAB);	// 슬래브시점각도
	CDPoint vAngSlabEnd		= pGir->GetAAA(OFF_END_SLAB);	// 슬래브시점각도

	double	dStaSttBindConc	= pGirLine->GetStationByScInLine(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt, 0, vAngSlabStt);
	double	dStaEndBindConc	= dStaSttBindConc + pGir->GetLengthDimType(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt, pBxEnd->GetStation()+pDB->m_dLengthTUBindConc_End);

	double	dStaSttAnchor	= dStaSttBindConc + pDeckTU->m_dCoverDanbu + pDeckTU->GetLengthSkewTriangle(nG, TRUE);
	
	double	dXSttAnchor	= dStaSttAnchor;
	double	dYSttAnchor	= pGir->GetElevationFlangeCamber(pBxStt->GetStation()) - pDB->m_BindConc.m_dDis;
	
	double	dX	= dXSttAnchor;
	double	dY	= dYSttAnchor + pDeckTU->m_dCoverDn;

	double	dAnchorRebarH	= pDB->m_BindConc.m_dHeight - pDeckTU->m_dCoverDn + pDeckTU->m_dLengthDolchul;
	double	dDanbuRebarH	= pDB->m_BindConc.m_dHeight - pDeckTU->m_dCoverDn - pDeckTU->m_dCoverUp;
	double	dRebarHTemp		= 0;
	
	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_BARC);

	// 스트럽철근
	double	dDia	= pDeckTU->m_dDiaStrup;
	
	Dom.LineTo(dX, dY, dX, dY+dAnchorRebarH);
	DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY), REBARPOS_DOWN, dDia);
	DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY+dDanbuRebarH), REBARPOS_UP, dDia);
	DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY+dDanbuRebarH/2), REBARPOS_NONE, dDia);

	long	nGugansu		= pDeckTU->m_arrCTC[nG].GetSize();
	CDPointArray dArrTemp;
	CDoubleArray dArrCtc;
	BOOL	bDanbuBogang	= FALSE;

	for(long nGugan=0; nGugan<nGugansu; nGugan++)
	{
		dArrTemp.RemoveAll();
		dArrTemp.Add(pDeckTU->m_arrCTC[nG].GetAt(nGugan));
		RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(dArrTemp, dArrCtc, FALSE);
		
		bDanbuBogang	= (dArrCtc.GetAt(0)==pDeckTU->m_dCTCAnchor/2) ? TRUE : FALSE;
		for(long n=0; n<dArrCtc.GetSize(); n++)
		{
			dX	+= dArrCtc.GetAt(n);
			if(bDanbuBogang && (n%2==0))
				dRebarHTemp	= dDanbuRebarH;
			else
				dRebarHTemp	= dAnchorRebarH;

			Dom.LineTo(dX, dY, dX, dY+dRebarHTemp);
			DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY), REBARPOS_DOWN, dDia);
			DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY+dDanbuRebarH), REBARPOS_UP, dDia);
			if(nGugan==nGugansu-1 && n==dArrCtc.GetSize()-1)
				DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY+dDanbuRebarH/2), REBARPOS_NONE, dDia);
		}
	}

	CString	sText	= _T("");
	//////////////////////////////////////////////////////////////////////////	
	// 종방향 철근
	sText.Format("배력철근_상부거더중간G%d", nG+1);
	CDPointArray arrXY	= pDeckTU->GetRebarShapeByJongName(nG, sText);
	long n = 0;
	for(n=0; n<arrXY.GetSize()-1; n++)
	{
		Dom.LineTo(arrXY.GetAt(n), arrXY.GetAt(n+1));
	}

	sText.Format("배력철근_하부거더중간G%d", nG+1);
	arrXY	= pDeckTU->GetRebarShapeByJongName(nG, sText);
	for(n=0; n<arrXY.GetSize()-1; n++)
	{
		Dom.LineTo(arrXY.GetAt(n), arrXY.GetAt(n+1));
	}

	CDPoint xySideRebarStt	= (arrXY.GetAt(0)+arrXY.GetAt(1))/2;
	CDPoint	xySideRebarEnd	= (arrXY.GetAt(3)+arrXY.GetAt(2))/2;
	Dom.LineTo(xySideRebarStt, xySideRebarEnd);

	// 단부보강철근
	dDia	= pDeckTU->m_dDiaDanbu;
	for (BOOL bStt=TRUE; bStt>=FALSE; bStt--)
	{
		CTwinVectorArray *pTvArr = NULL;
		if(bStt)
		{
			pTvArr	= &pDeckTU->m_arrTVectorPlanSkewStt[nG];
			dX		= dStaSttBindConc + pDeckTU->m_dCoverDanbu;
		}
		else
		{
			pTvArr	= &pDeckTU->m_arrTVectorPlanSkewEnd[nG];
			dX		= dStaEndBindConc - pDeckTU->m_dCoverDanbu;
		}
		DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY), REBARPOS_DOWN, dDia);
		DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY+dDanbuRebarH), REBARPOS_UP, dDia);

		long nSize = pTvArr->GetSize();

		for (long n=0; n<nSize; n++)
		{
			dX	+= bStt ? pDeckTU->m_dCTCAnchor/2 : -pDeckTU->m_dCTCAnchor/2;
			DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY), REBARPOS_DOWN, dDia);
			DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY+dDanbuRebarH), REBARPOS_UP, dDia);
		}
	}

	// 단부헌치철근
	dDia = pDeckTU->m_dDiaHunch;
	double dBindGap		= pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDeep-pDB->m_BindConc.m_dDis;
	sText.Format("헌치철근종단_G%d_시점", nG+1);
	arrXY	= pDeckTU->GetRebarShapeByJongName(nG, sText);
	dY	= pGir->GetElevationFlange(pBxStt->GetStation()) + pGir->GetHeightFlangeUpperByTaper(pBxStt->GetStation())+dBindGap;
	dY	-= pGir->GetHeightFlangeUpperByTaper(pBxStt->GetStation(),0,pBxStt->GetAngle(), TRUE);
	dY	+= pDeckTU->m_dCoverHunch;
	long nRebar = 0;
	for(nRebar=0; nRebar<pDeckTU->m_nQtyHunch; nRebar++)
	{
		dX = pDeckTU->GetStaMainRebar(TRUE, nG, FALSE)+pDeckTU->m_dDisHunch*nRebar;
		DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY), REBARPOS_UP, dDia);
	}
	sText.Format("헌치철근종단_G%d_종점", nG+1);
	arrXY	= pDeckTU->GetRebarShapeByJongName(nG, sText);
	dY	= pGir->GetElevationFlange(pBxEnd->GetStation()) + pGir->GetHeightFlangeUpperByTaper(pBxEnd->GetStation())+dBindGap;
	dY	-= pGir->GetHeightFlangeUpperByTaper(pBxEnd->GetStation(),0,pBxEnd->GetAngle(), TRUE);
	dY	+= pDeckTU->m_dCoverHunch;
	for(nRebar=0; nRebar<pDeckTU->m_nQtyHunch; nRebar++)
	{
		dX = pDeckTU->GetStaMainRebar(FALSE, nG, FALSE)-pDeckTU->m_dDisHunch*nRebar;
		DeckDraw.DrawPointRebar(&Dom, CDPoint(dX,dY), REBARPOS_UP, dDia);
	}
	*pDom << Dom;

	if(!bInput)
	{
		double	dCTC	= (pDB->m_BindConc.m_dWidth - pDeckTU->m_dCoverSd*2) / pDeckTU->m_nQtyDivideJong;
		CDPointArray xyArrMark = pDeckTU->GetMarkPosArrTU(nG, &pDeckTU->m_arrTVectorPlan[nG], dCTC/2, 0.5, FALSE);
		long	nSize	= xyArrMark.GetSize();
		long	nTargetRebar = long(nSize*0.2);

		CConcSymbol	SymDom(pOpt);

		// A-A
		double	dX_Target	= dStaSttBindConc + pDeckTU->m_dCoverDanbu;
		if(pDeckTU->m_nQtyHunch>0)	dX_Target += pDeckTU->m_dDisHunch*(pDeckTU->m_nQtyHunch-1);
		else						dX_Target += pDeckTU->m_dRangeDanbuBogang + 2.5*pDeckTU->m_dCTCAnchor;
		double	dY_Target	= dYSttAnchor + pDB->m_BindConc.m_dHeight + 100;
		double	dDis		= (pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDis) + pGir->GetHeightGirderByJijum(0) + 200;
		SymDom.ArrowSection(&Dom, "A", CDPoint(dX_Target,dY_Target), dDis, CDPoint(1, 0));
		
		// B-B
		dX_Target	= dStaSttAnchor + pDeckTU->m_dRangeDanbuBogang + nTargetRebar*pDeckTU->m_dCTCAnchor;
		SymDom.ArrowSection(&Dom, "B", CDPoint(dX_Target,dY_Target), dDis, CDPoint(1, 0));

		*pDom << Dom;
	}
}

void CDrawDBBaseRebarTU::DimMainRebarTUAtPlan(CDomyun *pDom, long nG, BOOL bInput)
{
	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CADeckDataTU		*pDeckTU= m_pDataMng->GetDeckDataTU();
	CADeckData			*pDeck  = m_pDataMng->GetDeckData();
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxFirst(BX_END_GIRDER);

	CRebarPlacing RebarPlace;
	CDrawDBDeck DeckDraw(m_pDataMng);

	// 구속콘크리트 시점, 종점점
//	double	dStaSttBindConc	= pDeckTU->GetStaAtGir(pBxStt->GetStation(), CDPoint(0,1), nG) - pDB->m_dLengthTUBindConc_Stt;
//	double	dStaSttBindConc	= pBxStt->GetStation() - pDB->m_dLengthTUBindConc_Stt;
	double	dStaSttBindConc	= pDeckTU->GetStaMainRebar(TRUE, nG, TRUE);
	
	CDimLineDomyun	Dom(pDom, (CLineInfoApp *)pDeckTU->GetLineInfo(nG));
	
	CDPointArray DimDataArray;

	DimDataArray.Add(CDPoint(1, pDeckTU->m_dCoverDanbu));
	DimDataArray.Add(CDPoint(1, pDeckTU->GetLengthSkewTriangle(nG, TRUE)));

	long	nGugansu	= pDeckTU->m_arrCTC[nG].GetSize();
	for(long nGugan=0; nGugan<nGugansu; nGugan++)
	{
		DimDataArray.Add(pDeckTU->m_arrCTC[nG].GetAt(nGugan));
	}

	DimDataArray.Add(CDPoint(1, pDeckTU->GetLengthSkewTriangle(nG, FALSE)));
	DimDataArray.Add(CDPoint(1, pDeckTU->m_dCoverDanbu));
	

	long	nSize	= DimDataArray.GetSize();
	double	dist	= 0, dPreDist=10000; // 의미없는 큰값
	CDPoint dp(0,0);
	CString str1	= _T(""), str2=_T("");	
	double	dLengthTotal	= 0;
	BOOL bUpperPrint = FALSE;

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("TOP");
	
	Dom.DimMoveToOnLine(dStaSttBindConc, -pDB->m_BindConc.m_dWidth/2, 0);
	
	for(long i=0; i<nSize; i++) 
	{	
		dp = DimDataArray.GetAt(i);
		dist = dp.x * dp.y;

		if(dp.x>1)
		{	
			if(dist<3500)
			{
				str1.Format("%s@%s",COMMA(dp.x),COMMA(dp.y));
				str2.Format("=%s",COMMA(dp.x * dp.y));
			}
			else
			{
				str1.Format("%s@%s=%s",COMMA(dp.x),COMMA(dp.y),COMMA(dp.x * dp.y));
				str2 = _T("");
			}
			dPreDist = 10000;
		}
		else
		{
			if(dPreDist<400 && dp.y<400 && bUpperPrint)
			{
				str1 = _T("");
				str2.Format("%s",COMMA(dp.y));				
				bUpperPrint = FALSE;
			}
			else
			{			
				str1.Format("%s",COMMA(dp.y));
				str2 = _T("");
				bUpperPrint = TRUE;
			}
			dPreDist = dp.y;
		}

		if(i==0)
		{
			Dom.DimLineOverText(COMMA(pDeckTU->m_dCoverDanbu),"",TRUE);
			Dom.DimLineToOnLine(pDeckTU->m_dCoverDanbu, "");
		}
		else if(i==nSize-1)
		{
			Dom.DimLineOverText(COMMA(pDeckTU->m_dCoverDanbu),"",FALSE);
			Dom.DimLineToOnLine(pDeckTU->m_dCoverDanbu, "");
		}
		else
			Dom.DimLineToOnLine(dist, str1, str2);
		dLengthTotal	+= dp.x * dp.y;
	}

	Dom.DimMoveToOnLine(dStaSttBindConc, -pDB->m_BindConc.m_dWidth/2, 1);
	Dom.DimLineToOnLine(dLengthTotal, COMMA(dLengthTotal), "");
	
	DrawUpDownLine(&Dom, nG, bInput);

	if((bInput && pDeck->m_bShowMark) || !bInput)
		DrawMark(&Dom, nG, DRAW_PLAN);

	*pDom << Dom;

	//////////////////////////////////////////////////////////////////////////
	// 측면치수
	CDimDomyunEx DomSide(pDB, pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&DomSide, HCAD_DIML);

	BOOL	bMark	= ((bInput && pDeck->m_bShowMark) || !bInput) ? TRUE : FALSE;
	long	nDimDan = bMark ? 2 : 1;
	CDPoint vAngSlabStt		= pGir->GetAAA(OFF_STT_SLAB);	// 슬래브시점각도
	CDPoint vAngSlabEnd		= pGir->GetAAA(OFF_END_SLAB);	// 슬래브시점각도
	double	dLevel	= DomSide.GetDimLevelDis();
	double	dExo	= DomSide.GetDimExo();
	double	Dist	= DomSide.Always(dExo) + DomSide.Always(dLevel)*nDimDan;
	double	staLeft	= pGir->GetStationBySc(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt, -pDB->m_BindConc.m_dWidth/2, vAngSlabStt);
	double	staRigh	= pGir->GetStationBySc(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt,  pDB->m_BindConc.m_dWidth/2, vAngSlabStt);
	double	staDim	= MIN(staLeft, staRigh);
	double	dCTC	= (pDB->m_BindConc.m_dWidth-pDeckTU->m_dCoverSd*2) / pDeckTU->m_nQtyDivideJong;
	
	str1.Format("%s@%s=%s",COMMA(pDeckTU->m_nQtyDivideJong),COMMA(dCTC),COMMA(pDB->m_BindConc.m_dWidth-pDeckTU->m_dCoverSd*2));
	
	// 좌측
	DomSide.SetDirection("LEFT");
	
	//////////////////////////////////////////////////////////////////////////
	DomSide.DimBaseCurvePoint(-Dist, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nDimDan);
	DomSide.DimCurveMoveTo(staDim, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nG, FALSE, COMMA(pDeckTU->m_dCoverSd), 0, FALSE, TRUE);
	//	이 윗부분이 없으면 치수선중 하나가 빠짐.... 당황스러움
	DomSide.DimBaseCurvePoint(-Dist, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nDimDan);
	DomSide.DimCurveMoveTo(staDim, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nG);

	DomSide.DimCurveLineTo(staDim, -pDeckTU->m_dCoverSd, "", "", nG, FALSE, TRUE, TRUE);
	DomSide.DimCurveLineTo(staDim, -(pDB->m_BindConc.m_dWidth-pDeckTU->m_dCoverSd*2), str1, "", nG);
	DomSide.DimCurveLineTo(staDim, -pDeckTU->m_dCoverSd, COMMA(pDeckTU->m_dCoverSd), "", nG, FALSE, TRUE, TRUE);

	nDimDan++;

	Dist	= DomSide.Always(dExo) + DomSide.Always(dLevel)*nDimDan;
	DomSide.DimBaseCurvePoint(-Dist, pGir->m_dGirderCenter-pDB->m_BindConc.m_dWidth/2, nDimDan);
	DomSide.DimCurveMoveTo(staDim, pGir->m_dGirderCenter-pDB->m_BindConc.m_dWidth/2, nG);

	DomSide.DimCurveLineTo(staDim, pDB->m_BindConc.m_dWidth, COMMA(pDB->m_BindConc.m_dWidth), "", nG);

	// 우측
	staLeft	= pGir->GetStationBySc(pBxEnd->GetStation()+pDB->m_dLengthTUBindConc_End, -pDB->m_BindConc.m_dWidth/2, vAngSlabEnd);
	staRigh	= pGir->GetStationBySc(pBxEnd->GetStation()+pDB->m_dLengthTUBindConc_End,  pDB->m_BindConc.m_dWidth/2, vAngSlabEnd);
	staDim	= MAX(staLeft, staRigh);
	nDimDan = bMark ? 2 : 1;
	Dist	= DomSide.Always(dExo) + DomSide.Always(dLevel)*nDimDan;

	DomSide.SetDirection("RIGHT");
	
	DomSide.DimBaseCurvePoint(Dist, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nDimDan);
	DomSide.DimCurveMoveTo(staDim, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nG, FALSE, COMMA(pDeckTU->m_dCoverSd), 0, FALSE, TRUE);
	// 여기도 마찬가지
	DomSide.DimBaseCurvePoint(Dist, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nDimDan);
	DomSide.DimCurveMoveTo(staDim, pGir->m_dGirderCenter+pDB->m_BindConc.m_dWidth/2, nG);

	DomSide.DimCurveLineTo(staDim, -pDeckTU->m_dCoverSd, "", "", nG, FALSE, TRUE, TRUE);
	DomSide.DimCurveLineTo(staDim, -(pDB->m_BindConc.m_dWidth-pDeckTU->m_dCoverSd*2), str1, "", nG);
	DomSide.DimCurveLineTo(staDim, -pDeckTU->m_dCoverSd, COMMA(pDeckTU->m_dCoverSd), "", nG, FALSE, TRUE, TRUE);
	nDimDan++;

	Dist	= DomSide.Always(dExo) + DomSide.Always(dLevel)*nDimDan;
	DomSide.DimBaseCurvePoint(Dist, pGir->m_dGirderCenter-pDB->m_BindConc.m_dWidth/2, nDimDan);
	DomSide.DimCurveMoveTo(staDim, pGir->m_dGirderCenter-pDB->m_BindConc.m_dWidth/2, nG);

	DomSide.DimCurveLineTo(staDim, pDB->m_BindConc.m_dWidth, COMMA(pDB->m_BindConc.m_dWidth), "", nG);

	*pDom << DomSide;
}

void CDrawDBBaseRebarTU::DrawBincConcRebarAtCross(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDrawAnchor)
{
	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir	 = pBx->GetGirder();
	CADeckDataTU		*pDeckTU = m_pDataMng->GetDeckDataTU();
	
	CRebarPlacing RebarPlace;
	CDrawDBDeck DeckDraw(m_pDataMng);

	double	dBindW		= pDB->m_BindConc.m_dWidth;
	double	dBindH		= pDB->m_BindConc.m_dHeight;
	double	dBindS1		= pDB->m_BindConc.m_dDeep;
	double	dBindS2		= pDB->m_BindConc.m_dDis;
	double	dSlabLowH	= -pDB->m_dThickSlabBase;		// 슬래브 하면의 높이
		
	double	dWebT			= pGir->GetValueBasicDefault("WEB_T");
	double	dGirderCenter	= pGir->m_dGirderCenter;
	double	dSta			= pBx->GetStation();

	long	nQtyDivideJong	= pDeckTU->m_nQtyDivideJong;
	long	nG = pBx->GetNumberGirder();

	// 구속콘크리트 시점, 종점점
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	// 스트럽철근
	CString	sText	= _T("");
	sText.Format("스트럽일반_G%d", nG+1);
	if(pBx->GetStation() < pGir->GetStationStt()+pDB->m_dLengthSlabGirderStt)
		sText.Format("스트럽단부_G%d", nG+1);
	if(pBx->GetStation() > pGir->GetStationEnd()-pDB->m_dLengthSlabGirderEnd)
		sText.Format("스트럽단부_G%d", nG+1);
	
	CDPointArray arrXY	= pDeckTU->GetRebarShapeByStrupName(nG, sText);
	long n = 0;
	for(n=0; n<arrXY.GetSize()-1; n++)
		Dom.LineTo(arrXY.GetAt(n), arrXY.GetAt(n+1));

	// 앵커철근
	if(bDrawAnchor)
	{
		for(long nAnchor=0; nAnchor<pDeckTU->m_nQtyAnchor; nAnchor++)
		{
			sText.Format("앵커철근_G%d_%d", nG+1, nAnchor+1);
			arrXY	= pDeckTU->GetRebarShapeByStrupName(nG, sText);
			for(n=0; n<arrXY.GetSize()-1; n++)
			{
				Dom.LineTo(arrXY.GetAt(n), arrXY.GetAt(n+1));
			}
		}
	}
	
	// 헌치철근
	BOOL bHunch = FALSE;
	double dHunchSttStt = pGir->GetSSS(OFF_STT_GIRDER);
	double dHenchSttEnd = dHunchSttStt + pDeckTU->m_nQtyHunch*pDeckTU->m_dDisHunch + pDeckTU->m_dCoverSd;
	double dHunchEndStt = pGir->GetSSS(OFF_END_GIRDER);
	double dHenchEndEnd = dHunchEndStt - pDeckTU->m_nQtyHunch*pDeckTU->m_dDisHunch - pDeckTU->m_dCoverSd;
	if(dSta >= dHunchSttStt && dSta <= dHenchSttEnd)	bHunch = TRUE;
	if(dSta >= dHenchEndEnd && dSta <= dHunchEndStt)	bHunch = TRUE;
	
	arrXY.RemoveAll();
	arrXY.Add(CDPoint(dGirderCenter-dBindW/2-pDeckTU->m_dOutLength, dSlabLowH+dBindS1-dBindH+pDeckTU->m_dCoverHunch));
	arrXY.Add(CDPoint(dGirderCenter-dWebT/2, dSlabLowH+dBindS1-dBindH+pDeckTU->m_dCoverHunch));
	arrXY.Add(CDPoint(dGirderCenter+dWebT/2, dSlabLowH+dBindS1-dBindH+pDeckTU->m_dCoverHunch));
	arrXY.Add(CDPoint(dGirderCenter+dBindW/2+pDeckTU->m_dOutLength, dSlabLowH+dBindS1-dBindH+pDeckTU->m_dCoverHunch));
	if(bDrawAnchor && bHunch)
	{
		Dom.LineTo(arrXY.GetAt(0), arrXY.GetAt(1));
		Dom.LineTo(arrXY.GetAt(2), arrXY.GetAt(3));
	}

	//////////////////////////////////////////////////////////////////////////
	// 종방향 철근
	CDPoint	vX(1,0), vY(0,1);
	
	// 모서리 철근
	CDPoint	xyPos(dGirderCenter-dBindW/2+pDeckTU->m_dCoverSd, dSlabLowH+dBindS1-pDeckTU->m_dCoverUp);
	double	dDia	= pDeckTU->m_dDiaEdge;
		
	DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_DOWN_RIGHT, dDia);

	xyPos	+= vX*(dBindW-pDeckTU->m_dCoverSd*2);
	DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_DOWN_LEFT, dDia);

	xyPos	-= vY*(dBindH-pDeckTU->m_dCoverUp-pDeckTU->m_dCoverDn);
	DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_UP_LEFT, dDia);

	xyPos	-= vX*(dBindW-pDeckTU->m_dCoverSd*2);
	DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_UP_RIGHT, dDia);
	
	// 측면 철근
	dDia	= pDeckTU->m_dDiaSide;
	xyPos	= CDPoint(dGirderCenter-dBindW/2+pDeckTU->m_dCoverSd, dSlabLowH+dBindS1-dBindH/2);
	DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_RIGHT, dDia);
	xyPos	+= vX*(dBindW-pDeckTU->m_dCoverSd*2);
	DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_LEFT, dDia);

	// 상부 철근
	double	dCTC	= (dBindW-pDeckTU->m_dCoverSd*2)/nQtyDivideJong;
	dDia	= pDeckTU->m_dDiaUpper;
	xyPos	= CDPoint(dGirderCenter-dBindW/2+pDeckTU->m_dCoverSd, dSlabLowH+dBindS1-pDeckTU->m_dCoverUp);
	for(n=0; n<nQtyDivideJong-1; n++)
	{
		xyPos	+= vX*dCTC;
		DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_DOWN, dDia);
	}

	// 하부 철근
	xyPos	= CDPoint(dGirderCenter-dBindW/2+pDeckTU->m_dCoverSd, dSlabLowH+dBindS1-dBindH+pDeckTU->m_dCoverDn);
	dDia	= pDeckTU->m_dDiaLower;
	for(n=0; n<nQtyDivideJong-1; n++)
	{
		xyPos	+= vX*dCTC;

		// 짝수구간일 경우 web으로 인해 중앙철근 배근 불가능
		if(nQtyDivideJong%2==0 && n==nQtyDivideJong/2.-1)	continue;
		DeckDraw.DrawPointRebar(&Dom, xyPos, REBARPOS_UP, dDia);
	}

	*pDom << Dom;
}

void CDrawDBBaseRebarTU::DimSabogangRebarTUAtPlan(CDomyun *pDom, long nG, BOOL bInput)
{
	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CADeckData			*pDeck		= m_pDataMng->GetDeckData();
	CADeckDataTU		*pDeckTU	= m_pDataMng->GetDeckDataTU();
	CLineInfo			*pLineGir	= pDeckTU->GetLineInfo(nG);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt		= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd		= Finder.GetBxFirst(BX_END_GIRDER);

	CRebarPlacing RebarPlace;
	CDrawDBDeck DeckDraw(m_pDataMng);

	double	dStaStt	= pDeckTU->GetStaAtGir(pBxStt->GetStation(), CDPoint(0,1), nG) - pDB->m_dLengthTUBindConc_Stt + pDeckTU->m_dCoverDanbu;
	double	dStaEnd = pDeckTU->GetStaAtGir(pBxEnd->GetStation(), CDPoint(0,1), nG) + pDB->m_dLengthTUBindConc_End - pDeckTU->m_dCoverDanbu;

	double	dDis			= pDB->m_BindConc.m_dWidth/2-pDeckTU->m_dCoverSd;
	double	dStaSttBindConc	= pGir->GetStationBySc(dStaStt, dDis, pGir->GetAAA(OFF_STT_SLAB));
	double	dStaEndBindConc	= pGir->GetStationBySc(dStaEnd, dDis, pGir->GetAAA(OFF_END_SLAB));
	
	CString	sText	= _T("");

	CDimLineDomyun	Dom(pDom, (CLineInfoApp *)pDeckTU->GetLineInfo(nG));

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("BOTTOM");

	if(pDeck->IsSkewStt())
	{
		Dom.DimMoveToOnLine(dStaSttBindConc, pDB->m_BindConc.m_dWidth/2, 0);	

		sText.Format("%s@%s=%s", COMMA(pDeckTU->m_nQtySabogang-1), COMMA(pDeckTU->m_dCTCAnchor/2), COMMA((pDeckTU->m_nQtySabogang-1)*pDeckTU->m_dCTCAnchor/2));
		Dom.DimLineToOnLine((pDeckTU->m_nQtySabogang-1)*pDeckTU->m_dCTCAnchor/2, sText, "");	
	}

	if(pDeck->IsSkewEnd())
	{
		Dom.DimMoveToOnLine(dStaEndBindConc, pDB->m_BindConc.m_dWidth/2, 0);	

		sText.Format("%s@%s=%s", COMMA(pDeckTU->m_nQtySabogang-1), COMMA(pDeckTU->m_dCTCAnchor/2), COMMA((pDeckTU->m_nQtySabogang-1)*pDeckTU->m_dCTCAnchor/2));
		Dom.DimLineToOnLine(-(pDeckTU->m_nQtySabogang-1)*pDeckTU->m_dCTCAnchor/2, sText, "");	
	}
	
	*pDom << Dom;
	
	
}

void CDrawDBBaseRebarTU::DimBincConcRebarAtJong(CDomyun *pDom, long nG, BOOL bInput)
{
	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CADeckData			*pDeck	= m_pDataMng->GetDeckData();
	CADeckDataTU		*pDeckTU= m_pDataMng->GetDeckDataTU();
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxFirst(BX_END_GIRDER);
	CLineInfo			*pGirLine	= pDeckTU->GetLineInfo(nG);

	CRebarPlacing RebarPlace;
	CDrawDBDeck DeckDraw(m_pDataMng);

	BOOL	bMark	= ((bInput && pDeck->m_bShowMark) || !bInput) ? TRUE : FALSE;
	long	nCol	= bMark ? 1 : 0;

	// 구속콘크리트 시점, 종점점
	CDPoint vAngSlabStt		= pGir->GetAAA(OFF_STT_SLAB);	// 슬래브시점각도
	CDPoint vAngSlabEnd		= pGir->GetAAA(OFF_END_SLAB);	// 슬래브시점각도

	double	dStaSttBindConc	= pGirLine->GetStationByScInLine(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt, 0, vAngSlabStt);
	double	dStaEndBindConc	= dStaSttBindConc + pGir->GetLengthDimType(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt, pBxEnd->GetStation()+pDB->m_dLengthTUBindConc_End);

	double	dStaSttAnchor	= dStaSttBindConc + pDeckTU->m_dCoverDanbu + pDeckTU->GetLengthSkewTriangle(nG, TRUE);
	
	double	dXSttAnchor	= dStaSttAnchor;
	double	dYSttAnchor	= pGir->GetElevationFlangeCamber(pBxStt->GetStation());
	
	double	dY	= dYSttAnchor +	(pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDis);

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	CDPointArray DimDataArray;

	DimDataArray.Add(CDPoint(1, pDeckTU->m_dCoverDanbu));
	DimDataArray.Add(CDPoint(1, pDeckTU->GetLengthSkewTriangle(nG, TRUE)));

	long	nGugansu	= pDeckTU->m_arrCTC[nG].GetSize();
	for(long nGugan=0; nGugan<nGugansu; nGugan++)
	{
		DimDataArray.Add(pDeckTU->m_arrCTC[nG].GetAt(nGugan));
	}

	DimDataArray.Add(CDPoint(1, pDeckTU->GetLengthSkewTriangle(nG, FALSE)));
	DimDataArray.Add(CDPoint(1, pDeckTU->m_dCoverDanbu));

	Dom.SetDirection("TOP");
	Dom.DimMoveToExt(dStaSttBindConc, dY, nCol, COMMA(pDeckTU->m_dCoverDanbu));

	long	nSize	= DimDataArray.GetSize();
	double	dist	= 0, dPreDist=10000; // 의미없는 큰값
	CDPoint dp(0,0);
	CString str1	= _T(""), str2=_T("");	
	BOOL bUpperPrint = FALSE;
	double	distTotal	= 0;

	for(long i=0; i<nSize; i++) 
	{	
		dp = DimDataArray.GetAt(i);
		dist = dp.x * dp.y;

		if(dp.x>1)
		{	
			if(dist<3500)
			{
				str1.Format("%s@%s",COMMA(dp.x),COMMA(dp.y));
				str2.Format("=%s",COMMA(dp.x * dp.y));
			}
			else
			{
				str1.Format("%s@%s=%s",COMMA(dp.x),COMMA(dp.y),COMMA(dp.x * dp.y));
				str2 = _T("");
			}
			dPreDist = 10000;
		}
		else
		{
			if(dPreDist<400 && dp.y<400 && bUpperPrint)
			{
				str1 = _T("");
				str2.Format("%s",COMMA(dp.y));				
				bUpperPrint = FALSE;
			}
			else
			{			
				str1.Format("%s",COMMA(dp.y));
				str2 = _T("");
				bUpperPrint = TRUE;
			}
			dPreDist = dp.y;
		}

		if(i==0)
			Dom.DimLineToExtend(dist, nCol, "", "");
		else if(i==nSize-1)
			Dom.DimLineToOver(dist, nCol, bUpperPrint?str1:str2);
		else
			Dom.DimLineToExtend(dist, nCol, str1, str2);

		distTotal	+= dist;
	}

	nCol++;
	Dom.DimMoveTo(dStaSttBindConc, dY, nCol);
	Dom.DimLineTo(distTotal, nCol, COMMA(distTotal));

	Dom.SetDirection("LEFT");
	nCol	= bMark ? 1 : 0;
	Dom.DimMoveToExt(dStaSttBindConc, dY-pDB->m_BindConc.m_dHeight, nCol, COMMA(pDeckTU->m_dCoverUp));
	Dom.DimLineTo(pDeckTU->m_dCoverUp, nCol, "");
	dist	= pDB->m_BindConc.m_dHeight - pDeckTU->m_dCoverUp - pDeckTU->m_dCoverDn;
	Dom.DimLineTo(dist, nCol, COMMA(dist));
	Dom.DimLineToOver(pDeckTU->m_dCoverDn, nCol, COMMA(pDeckTU->m_dCoverDn));

	nCol++;
	Dom.DimMoveTo(dStaSttBindConc, dY-pDB->m_BindConc.m_dHeight, nCol);
	Dom.DimLineTo(pDB->m_BindConc.m_dHeight, nCol, COMMA(pDB->m_BindConc.m_dHeight));

	Dom.SetDirection("RIGHT");
	nCol	= bMark ? 1 : 0;
	Dom.DimMoveToExt(dStaEndBindConc, dY-pDB->m_BindConc.m_dHeight, nCol, COMMA(pDeckTU->m_dCoverUp));
	Dom.DimLineTo(pDeckTU->m_dCoverUp, nCol, "");
	dist	= pDB->m_BindConc.m_dHeight - pDeckTU->m_dCoverUp - pDeckTU->m_dCoverDn;
	Dom.DimLineTo(dist, nCol, COMMA(dist));
	Dom.DimLineToOver(pDeckTU->m_dCoverDn, nCol, COMMA(pDeckTU->m_dCoverDn));

	nCol++;
	Dom.DimMoveTo(dStaEndBindConc, dY-pDB->m_BindConc.m_dHeight, nCol);
	Dom.DimLineTo(pDB->m_BindConc.m_dHeight, nCol, COMMA(pDB->m_BindConc.m_dHeight));

	if(bMark)
		DrawMark(&Dom, nG, DRAW_JONG);
	
	*pDom << Dom;
}

void CDrawDBBaseRebarTU::DimBincConcRebarAtCross(CDomyun *pDom, long nG, BOOL bInput, BOOL bAnchor, BOOL bDanbuJewon)
{
	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CADeckData			*pDeck	= m_pDataMng->GetDeckData();
	CADeckDataTU		*pDeckTU= m_pDataMng->GetDeckDataTU();
	
	CRebarPlacing RebarPlace;
	CDrawDBDeck DeckDraw(m_pDataMng);

	double	dBindW		= pDB->m_BindConc.m_dWidth;
	double	dBindH		= pDB->m_BindConc.m_dHeight;
	double	dBindS1		= pDB->m_BindConc.m_dDeep;
	double	dSlabLowH	= -pDB->m_dThickSlabBase;		// 슬래브 하면의 높이
	double	dGirderCenter	= pGir->m_dGirderCenter;
	
	long	nQtyDivideJong	= pDeckTU->m_nQtyDivideJong;

	double	dCoverSd	= pDeckTU->m_dCoverSd;
	double	dCoverUp	= pDeckTU->m_dCoverUp;
	double	dCoverDn	= pDeckTU->m_dCoverDn;
	double	dAnchorW	= pDeckTU->m_dWidthAnchor;
	double	dAnchorDis	= pDeckTU->m_dDisAnchor;
	long	nQtyAnc		= pDeckTU->m_nQtyAnchor;
	double	dDimY		= dSlabLowH+dBindS1;	

	BOOL	bMark	= ((bInput && pDeck->m_bShowMark) || !bInput) ? TRUE : FALSE;
	CString	sText1	= _T(""), sText2 = _T("");
	
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	CDoubleArray	arrDim;

	Dom.SetDirection("TOP");
	long	nCol	= bMark ? 1 : 0;
	if(bAnchor)
	{
		double dPreX = dGirderCenter-(dAnchorDis*(nQtyAnc-1)+dAnchorW*nQtyAnc)/2;
		Dom.DimMoveTo(dGirderCenter-dBindW/2, dDimY, nCol);
		Dom.DimLineToExtend(dPreX-(dGirderCenter-dBindW/2), nCol, bInput?"":COMMA(dPreX-(dGirderCenter-dBindW/2)), bInput?COMMA(dPreX-(dGirderCenter-dBindW/2)):"");
		
		for(long nAnc=0; nAnc<nQtyAnc; nAnc++)
		{
			Dom.DimLineToExtend(dAnchorW, nCol, bInput?"Aw":COMMA(dAnchorW), bInput?COMMA(dAnchorW):"");
			if(nAnc<nQtyAnc-1)
				Dom.DimLineToExtend(dAnchorDis, nCol, bInput?"Ad":COMMA(dAnchorDis), bInput?COMMA(dAnchorDis):"");
			
			dPreX += dAnchorDis+dAnchorW;
		}
		Dom.DimLineToExtend(dGirderCenter+dBindW/2-(dPreX-dAnchorDis), nCol, bInput?"":COMMA(dGirderCenter+dBindW/2-(dPreX-dAnchorDis)), bInput?COMMA(dGirderCenter+dBindW/2-(dPreX-dAnchorDis)):"");
	}
	else
	{
		sText1.Format("%s@%s", COMMA(nQtyDivideJong), COMMA((dBindW-dCoverSd*2)/nQtyDivideJong));
		sText2.Format("=%s", COMMA(dBindW-dCoverSd*2));
		Dom.DimMoveTo(dGirderCenter-dBindW/2, dDimY, nCol);
		Dom.DimLineTo(pDeckTU->m_dCoverSd, nCol, COMMA(pDeckTU->m_dCoverSd));
		Dom.DimLineToExtend(dBindW-dCoverSd*2, nCol, sText1, sText2);
		Dom.DimLineTo(pDeckTU->m_dCoverSd, nCol, COMMA(pDeckTU->m_dCoverSd));		
	}
	nCol++;

	Dom.DimMoveTo(dGirderCenter-dBindW/2, dDimY, nCol);
	Dom.DimLineTo(dBindW, nCol, COMMA(dBindW));

	if(bInput)
	{
		Dom.SetDirection("BOTTOM");
		nCol	= bMark ? 1 : 0;
		sText1.Format("E@%s", COMMA((dBindW-dCoverSd*2)/nQtyDivideJong));
		sText2.Format("%s@%s=%s", COMMA(nQtyDivideJong), COMMA((dBindW-dCoverSd*2)/nQtyDivideJong), COMMA(dBindW-dCoverSd*2));	
		Dom.DimMoveTo(dGirderCenter-dBindW/2-pDeckTU->m_dOutLength, dSlabLowH+dBindS1-dBindH, nCol);
		Dom.DimLineToExtend(pDeckTU->m_dOutLength, nCol, "Hd", COMMA(pDeckTU->m_dOutLength));
		Dom.DimLineToExtend(dCoverSd, nCol, "Sc", COMMA(dCoverSd));
		Dom.DimLineToExtend(dBindW-dCoverSd*2, nCol, sText1, sText2);
		Dom.DimLineToExtend(dCoverSd, nCol, "Sc", COMMA(dCoverSd));
		Dom.DimLineToExtend(pDeckTU->m_dOutLength, nCol, "Hd", COMMA(pDeckTU->m_dOutLength));
	}

	Dom.SetDirection("LEFT");
	nCol	= bMark ? 1 : 0;
	Dom.DimMoveToExt(dGirderCenter-dBindW/2, dSlabLowH+dBindS1-dBindH, nCol, bInput?"Dc":COMMA(dCoverDn), bInput?COMMA(dCoverDn):"");
	Dom.DimLineTo(dCoverDn, nCol, "");
	Dom.DimLineTo((dBindH-dCoverUp-dCoverDn), nCol, COMMA(dBindH-dCoverUp-dCoverDn));
	Dom.DimLineToOverExtend(dCoverUp, nCol, bInput?"Uc":COMMA(dCoverUp), bInput?COMMA(dCoverUp):"");
	
	nCol++;

	Dom.DimMoveTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1-dBindH, nCol);
	Dom.DimLineTo(dBindH, nCol, COMMA(dBindH));

	if(bAnchor)
	{
		Dom.SetDirection("RIGHT");
		nCol	= bMark ? 1 : 0;
		Dom.DimMoveToExt(dGirderCenter+dBindW/2, dSlabLowH+dBindS1-dBindH, nCol, bInput?"Hc":COMMA(pDeckTU->m_dCoverHunch), bInput?COMMA(pDeckTU->m_dCoverHunch):"");
		Dom.DimLineTo(pDeckTU->m_dCoverHunch, nCol, "");
		Dom.DimLineToExtend(dBindH-pDeckTU->m_dCoverHunch, nCol, bInput?"":COMMA(dBindH-pDeckTU->m_dCoverHunch),bInput? COMMA(dBindH-pDeckTU->m_dCoverHunch):"");
		Dom.DimLineToOverExtend(pDeckTU->m_dLengthDolchul, nCol, bInput?"Ah":COMMA(pDeckTU->m_dLengthDolchul), bInput?COMMA(pDeckTU->m_dLengthDolchul):"");
	}

	if(bMark)
		DrawMark(&Dom, nG, DRAW_CROSS, bAnchor, bDanbuJewon);

	if(!bInput && !bDanbuJewon)
	{
		CDPointArray	arrWeld;
		double	dWebT	= pDeckTU->GetMaxWebT(nG);
		CDPoint	xyLeft(dGirderCenter-dWebT/2, dSlabLowH+dBindS1-dBindH+dCoverDn);
		CDPoint	xyRight(dGirderCenter+dWebT/2, dSlabLowH+dBindS1-dBindH+dCoverDn);
		double	dBasicL	= 200;
		CDPoint	vX(1,0), vY(0,1);

		arrWeld.Add(xyLeft);
		arrWeld.Add(xyLeft-vX*dBasicL-vY*dBasicL);
		arrWeld.Add(xyRight+vX*dBasicL*2-vY*dBasicL*2.5);
		arrWeld.Add(xyRight);
		for(long n=0; n<arrWeld.GetSize()-1; n++)
			Dom.LineTo(arrWeld.GetAt(n), arrWeld.GetAt(n+1));

		CDPoint	xyTop(arrWeld.GetAt(arrWeld.GetSize()-2)+vY*dBasicL);
		Dom.LineTo(arrWeld.GetAt(arrWeld.GetSize()-2), xyTop);
		Dom.Solid(xyTop.x, xyTop.y, xyTop.x, xyTop.y-dBasicL/3, xyTop.x+dBasicL/4, xyTop.y-dBasicL/3, xyTop.x, xyTop.y);

		CDPoint xyWeldL, xyWeldR, vWeld;
		vWeld = (-vX-vY).Unit();
		xyWeldL = xyLeft + vWeld*Dom.Always(3) + vWeld.RotateInv90()*Dom.Always(0.5);
		xyWeldR = xyLeft + vWeld*Dom.Always(3) + vWeld.Rotate90()*Dom.Always(0.5);
		Dom.Solid(xyLeft.x, xyLeft.y, xyWeldL.x, xyWeldL.y, xyWeldR.x, xyWeldR.y, xyLeft.x, xyLeft.y);

		vWeld = (vX*2-vY*2.5).Unit();
		xyWeldL = xyRight + vWeld*Dom.Always(3) + vWeld.RotateInv90()*Dom.Always(0.5);
		xyWeldR = xyRight + vWeld*Dom.Always(3) + vWeld.Rotate90()*Dom.Always(0.5);
		Dom.Solid(xyRight.x, xyRight.y, xyWeldL.x, xyWeldL.y, xyWeldR.x, xyWeldR.y, xyRight.x, xyRight.y);
	}

	*pDom << Dom;
}


void CDrawDBBaseRebarTU::DrawBeryukRebarAtPlan(CDomyun *pDom, long nG)
{
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CADeckDataTU		*pDeckTU	= m_pDataMng->GetDeckDataTU();
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt		= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd		= Finder.GetBxFirst(BX_END_GIRDER);
	CLineInfo			*pLineGir	= pDeckTU->GetLineInfo(nG);

	CDomyun	Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	CDPoint vAngStt		= pGir->GetAAA(OFF_STT_SLAB);	// 슬래브시점각도
	CDPoint vAngEnd		= pGir->GetAAA(OFF_END_SLAB);	// 슬래브시점각도

	double dStaSttGir = pDeckTU->GetStaMainRebar(TRUE, nG, FALSE);
	double dStaEndGir = pDeckTU->GetStaMainRebar(FALSE, nG, FALSE);
	
	double	dStndCuttingAngle	= pDB->m_dGirEndStndGuttingAngle;
	BOOL	bSttCutAngle90	= (vAngStt.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90	= (vAngEnd.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bSttCutAngle90_Inv	= (vAngStt.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90_Inv	= (vAngEnd.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;

	double	dDistLeft	= -(pDB->m_BindConc.m_dWidth/2 - pDeckTU->m_dCoverSd);
	double	dDistRight	=  (pDB->m_BindConc.m_dWidth/2 - pDeckTU->m_dCoverSd);

	double	dSpace		= (pDB->m_BindConc.m_dWidth-pDeckTU->m_dCoverSd*2)/pDeckTU->m_nQtyDivideJong;
	double	dDisRebar	= dDistLeft; 
	CTwinVectorArray tvArr;
	CTwinVector tvLine;
	CDPoint	xyStt(0,0), xyEnd(0,0);
	long n = 0;
	for(n=0; n<=pDeckTU->m_nQtyDivideJong; n++)
	{
		if(n!=0)	dDisRebar	+= dSpace;
		xyStt	= pLineGir->GetXyLineDisByAng(dStaSttGir, dDisRebar, vAngStt);
		xyEnd	= pLineGir->GetXyLineDisByAng(dStaEndGir, dDisRebar, vAngEnd);

		tvLine.m_v1	= xyStt;
		tvLine.m_v2	= xyEnd;
		tvArr.Add(tvLine);
	}

	CDPoint xySttUp		= pGir->GetXyGirderDis(dStaSttGir, dDistLeft, bSttCutAngle90_Inv? CDPoint(0,1) : vAngStt);
	CDPoint xySttMid	= pGir->GetXyGirderDis(dStaSttGir, 0, vAngStt);
	CDPoint xySttDn		= pGir->GetXyGirderDis(dStaSttGir, dDistRight, bSttCutAngle90? CDPoint(0,1) : vAngStt);
	CDPoint xyEndUp		= pGir->GetXyGirderDis(dStaEndGir, dDistLeft, bEndCutAngle90? CDPoint(0,1) : vAngEnd);
	CDPoint xyEndMid	= pGir->GetXyGirderDis(dStaEndGir, 0, vAngEnd);
	CDPoint xyEndDn		= pGir->GetXyGirderDis(dStaEndGir, dDistRight, bEndCutAngle90_Inv? CDPoint(0,1) : vAngEnd);

	// 모따기 기준선분
	CDPoint	xyStt_S1(0,0), xyStt_S2(0,0), xyEnd_S1(0,0), xyEnd_S2(0,0), xyResult(0,0);
	if(bSttCutAngle90)
	{
		xyStt_S1	= xySttMid;
		xyStt_S2	= xySttDn;
	}
	if(bSttCutAngle90_Inv)
	{
		xyStt_S1	= xySttUp;
		xyStt_S2	= xySttMid;
	}

	if(bEndCutAngle90)
	{
		xyEnd_S1	= xyEndUp;
		xyEnd_S2	= xyEndMid;
	}
	if(bEndCutAngle90_Inv)
	{
		xyEnd_S1	= xyEndMid;
		xyEnd_S2	= xyEndDn;
	}

	BOOL	bExist	= FALSE;
	for(n=0; n<tvArr.GetSize(); n++)
	{
		CTwinVector Tvec = tvArr.GetAt(n);
		bExist	= GetXyMatchSegAndSeg(Tvec.m_v1, Tvec.m_v2, xyStt_S1, xyStt_S2, xyResult);
		if(bExist && (bSttCutAngle90 || bSttCutAngle90_Inv))	
			tvArr.GetAt(n).m_v1 = xyResult;
		
		bExist	= GetXyMatchSegAndSeg(Tvec.m_v1, Tvec.m_v2, xyEnd_S1, xyEnd_S2, xyResult);
		if(bExist && (bEndCutAngle90 || bEndCutAngle90_Inv))	
			tvArr.GetAt(n).m_v2 = xyResult;
	}


	dDisRebar	= dDistLeft;
	for(n=0; n<tvArr.GetSize(); n++)
	{
		CTwinVector Tvec = tvArr.GetAt(n);
		if(n!=0)	dDisRebar	+= dSpace;
		pLineGir->DrawLS(&Dom, Tvec.m_v1.x, Tvec.m_v2.x, dDisRebar);
	}
//	Dom.LineTo(xyStt_S1, xyStt_S2);
//	Dom.LineTo(xyEnd_S1, xyEnd_S2);

	*pDom << Dom;
}


void CDrawDBBaseRebarTU::DrawUpDownLine(CDomyun *pDomP, long nG, BOOL bInput)
{
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CADeckDataTU		*pDeckTU	= m_pDataMng->GetDeckDataTU();
	CADeckData			*pDeck		= m_pDataMng->GetDeckData();
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt		= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd		= Finder.GetBxFirst(BX_END_GIRDER);
	CLineInfo			*pCenLine	= pDeckTU->GetLineInfo(-1);

	if(pDB->IsExtLine(-1))  return;

	CDomyun Dom(pDomP);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_CENT);

	double distToLine = pGir->m_dGirderCenter;//pDeckData->GetDistByJongBase(JONGBASEPOS_SLABCENTER);
	double wing		  = ((bInput && pDeck->m_bShowMark) || !bInput) ? 1000 : 500;

	double	sttStation		= pBxStt->GetStation() - pDB->m_dLengthTUBindConc_Stt + pDeckTU->m_dCoverDanbu - wing*1.5;
	double	endStation		= pBxEnd->GetStation() + pDB->m_dLengthTUBindConc_End - pDeckTU->m_dCoverDanbu + wing;
	double	dTextStation	= (pBxEnd->GetStation() - pBxStt->GetStation())*0.4;
 
	pCenLine->DrawLS(&Dom,sttStation,endStation+wing/3,distToLine);
	*pDomP << Dom;
	//
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCADEX_GISI);
	CString strUp  = " 상 면";
	CString strLow = "하 면 ";
	double  Len	   = Dom.GetTextWidth(strLow,1);
	double  Height = Dom.GetTextHeight();

	CDPoint cp, vAng;

	if(!bInput)
	{
		vAng	= pCenLine->GetAngleAzimuth(sttStation+dTextStation-Dom.Always(10));
		cp		= pCenLine->GetXyLineDis(sttStation+dTextStation-Dom.Always(10), distToLine-Height*2.5);
		Dom.TextCircle(cp.x, cp.y, "상 면");
		Dom.Rotate(cp, vAng);
		*pDomP << Dom;

		vAng	= pCenLine->GetAngleAzimuth(sttStation+dTextStation+Dom.Always(10));
		cp		= pCenLine->GetXyLineDis(sttStation+dTextStation+Dom.Always(10), distToLine+Height*2.5);
		Dom.TextCircle(cp.x, cp.y, "하 면");
		Dom.Rotate(cp, vAng);
		*pDomP << Dom;

		vAng	= pCenLine->GetAngleAzimuth(endStation-dTextStation+Dom.Always(10));
		cp		= pCenLine->GetXyLineDis(endStation-dTextStation+Dom.Always(10), distToLine-Height*2.5);
		Dom.TextCircle(cp.x, cp.y, "상 면");
		Dom.Rotate(cp, vAng);
		*pDomP << Dom;

		vAng	= pCenLine->GetAngleAzimuth(endStation-dTextStation-Dom.Always(10));
		cp		= pCenLine->GetXyLineDis(endStation-dTextStation-Dom.Always(10), distToLine+Height*2.5);
		Dom.TextCircle(cp.x, cp.y, "하 면");
		Dom.Rotate(cp, vAng);
		*pDomP << Dom;
	}
	

	//
	cp = pCenLine->GetXyLineDis(sttStation+Height*2,distToLine);

	Dom.SetTextAngle(pCenLine->GetAngleAzimuth(sttStation).GetAngleDegree()+90);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(cp,strUp);
	Dom.SetTextAlignHorz(TA_RIGHT);
	Dom.TextOut(cp,strLow);
	*pDomP << Dom;
	//
	cp = pCenLine->GetXyLineDis(sttStation+Height*2.1,distToLine);
	Dom.Circle(cp.x,cp.y,Dom.Always(0.5),TRUE);
	Dom.GiSiArrow(cp.x,cp.y,Dom.Always(0),Len,0,"");
	Dom.Rotate(pCenLine->GetAngleAzimuth(sttStation).Rotate90().GetAngleRadian(),cp.x,cp.y);
	*pDomP << Dom;
	//
	Dom.GiSiArrow(cp.x,cp.y,Dom.Always(0),Len,0,"");
	Dom.Rotate(pCenLine->GetAngleAzimuth(sttStation).RotateInv90().GetAngleRadian(),cp.x,cp.y);
	*pDomP << Dom;
}

void CDrawDBBaseRebarTU::DrawMark(CDomyun *pDom, long nG, long nDrawPos, BOOL bAnchor/*TRUE*/, BOOL bDanbuJewon/*FALSE*/)
{
	CADeckDataTU	*pDeckDataTU= m_pDataMng->GetDeckDataTU();
	
	pDeckDataTU->m_arrMarkPosTU[nG].clear();

	if(nDrawPos==DRAW_PLAN)
	{
		pDeckDataTU->MakeRebarMarkPosPlanTU(nG, pDom->GetScaleDim());
		DrawMarkPlan(pDom, nG);
	}
	else if(nDrawPos==DRAW_JONG)
	{
		pDeckDataTU->MakeRebarMarkPosJongTU(nG, pDom->GetScaleDim());
		DrawMarkJong(pDom, nG);
	}
	else if(nDrawPos==DRAW_CROSS)
	{
		pDeckDataTU->MakeRebarMarkPosCrossTU(nG, pDom->GetScaleDim());
		DrawMarkCross(pDom, nG, bAnchor, bDanbuJewon);
	}
}

void CDrawDBBaseRebarTU::DrawMarkPlan(CDomyun *pDom, long nG)
{
	CADeckDataTU	*pDeckDataTU= m_pDataMng->GetDeckDataTU();
	CPlateBridgeApp *pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CRebar			*pRB		= NULL;

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pDataMng);

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText = _T(""), szRebar = _T("");
	CDPointArray	xyArr;
	
	double	dCTC		= (pDB->m_BindConc.m_dWidth - pDeckDataTU->m_dCoverSd*2) / pDeckDataTU->m_nQtyDivideJong;
	double	dExtendL	= 0;

	//////////////////////////////////////////////////////////////////////////
	// 스트럽
	//	일반부
	szText.Format("스트럽일반_G%d_시점하", nG+1);
	szRebar.Format("스트럽일반_G%d", nG+1);

	dExtendL	= dCTC/2 + pDeckDataTU->m_dCoverSd + 600;
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 5, FALSE, dExtendL, -1, TRUE);

	szText.Format("스트럽일반_G%d_종점하", nG+1);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 5, FALSE, dExtendL, xyArr.GetSize()-2, FALSE);

	szText.Format("스트럽일반_G%d_시점상", nG+1);
	
	dExtendL	= dCTC/2 + pDeckDataTU->m_dCoverSd + 200;
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 4, FALSE, dExtendL, -1, TRUE);

	szText.Format("스트럽일반_G%d_종점상", nG+1);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 4, FALSE, dExtendL, xyArr.GetSize()-2, FALSE);
	*pDom << Dom;

	// 단부
	CDPoint vAngStt = pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint vAngEnd = pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);
	double	dStndCuttingAngle	= pDB->m_dGirEndStndGuttingAngle;
	BOOL	bSttCutAngle90		= (vAngStt.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90		= (vAngEnd.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bSttCutAngle90_Inv	= (vAngStt.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90_Inv	= (vAngEnd.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
	//=====================================================
	//	시점
	if((bSttCutAngle90 || bSttCutAngle90_Inv))
	{
		szText.Format("스트럽단부_G%d_시점1", nG+1);
		pRB  = pDeckDataTU->GetRebarByName(szText);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)
		{
			CDomyun	Dom1(pDom);
			pOpt->SetEnvType(&Dom1,HCAD_DIML);
			dExtendL	= pDeckDataTU->m_dCoverDanbu + 200;
			if(bSttCutAngle90)	SymDom.RebarMarkLineUpper(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE, dExtendL);
			else				SymDom.RebarMarkLineUpper(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), TRUE, dExtendL);
			Dom1.Rotate(xyArr.GetAt(0), bSttCutAngle90?CDPoint(0,1):CDPoint(0,1));
			*pDom << Dom1;
		}

		szText.Format("스트럽단부_G%d_시점2", nG+1);
		pRB  = pDeckDataTU->GetRebarByName(szText);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)
		{
			CDomyun	Dom1(pDom);
			pOpt->SetEnvType(&Dom1,HCAD_DIML);
			dExtendL	= pDeckDataTU->m_dCoverDanbu + 200;
			if(bSttCutAngle90)	SymDom.RebarMarkLineUpper(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), TRUE, dExtendL);
			else				SymDom.RebarMarkLineUpper(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE, dExtendL);
			Dom1.Rotate(xyArr.GetAt(0), bSttCutAngle90?CDPoint(0,1):CDPoint(0,1));
			*pDom << Dom1;
		}

	}
	else
	{
		szText.Format("스트럽단부_G%d_시점", nG+1);
		pRB  = pDeckDataTU->GetRebarByName(szText);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)
		{
			CDomyun	Dom1(pDom);
			pOpt->SetEnvType(&Dom1,HCAD_DIML);
			dExtendL	= pDeckDataTU->m_dCoverDanbu + 200;
			SymDom.RebarMarkLineUpper(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE, dExtendL);
			Dom1.Rotate(xyArr.GetAt(0), CDPoint(0,1));
			*pDom << Dom1;
		}
	}
	//=====================================================
	//	종점
	if((bEndCutAngle90 || bEndCutAngle90_Inv))
	{
		szText.Format("스트럽단부_G%d_종점1", nG+1);
		pRB  = pDeckDataTU->GetRebarByName(szText);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)
		{
			CDomyun	Dom1(pDom);
			pOpt->SetEnvType(&Dom1,HCAD_DIML);
			dExtendL	= pDeckDataTU->m_dCoverDanbu + 600;
			if(bEndCutAngle90)	SymDom.RebarMarkLineLower(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), TRUE, dExtendL);
			else				SymDom.RebarMarkLineLower(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE, dExtendL);
			Dom1.Rotate(xyArr.GetAt(0), bEndCutAngle90?CDPoint(0,1):CDPoint(0,1));
			*pDom << Dom1;
		}

		szText.Format("스트럽단부_G%d_종점2", nG+1);
		pRB  = pDeckDataTU->GetRebarByName(szText);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)
		{
			CDomyun	Dom1(pDom);
			pOpt->SetEnvType(&Dom1,HCAD_DIML);
			dExtendL	= pDeckDataTU->m_dCoverDanbu + 600;
			if(bEndCutAngle90)	SymDom.RebarMarkLineLower(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE, dExtendL);
			else				SymDom.RebarMarkLineLower(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), TRUE, dExtendL);
			Dom1.Rotate(xyArr.GetAt(0), bEndCutAngle90?CDPoint(0,1):CDPoint(0,1));
			*pDom << Dom1;
		}
	}
	else
	{
		szText.Format("스트럽단부_G%d_종점", nG+1);
		pRB  = pDeckDataTU->GetRebarByName(szText);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)
		{
			CDomyun	Dom1(pDom);
			pOpt->SetEnvType(&Dom1,HCAD_DIML);
			dExtendL	= pDeckDataTU->m_dCoverDanbu + 500;
			SymDom.RebarMarkLineLower(&Dom1, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE, dExtendL);
			Dom1.Rotate(xyArr.GetAt(0), CDPoint(0,1));
			*pDom << Dom1;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 구속콘크리트 헌치부 철근
	//=====================================================
	//	시점 
	szText.Format("헌치철근시점_G%d", nG+1);
	pRB  = pDeckDataTU->GetRebarByName(szText);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, FALSE, 200, -1, TRUE);
	*pDom << Dom;
	//=====================================================
	//	종점 
	szText.Format("헌치철근종점_G%d", nG+1);
	pRB  = pDeckDataTU->GetRebarByName(szText);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, TRUE, 200, -1, FALSE);
	*pDom << Dom;
	
	//////////////////////////////////////////////////////////////////////////
	// 사보강철근
	//=====================================================
	//	시점 
	if((bSttCutAngle90 || bSttCutAngle90_Inv))
	{
		szText.Format("사보강철근_G%d_시점1", nG+1);
		szRebar.Format("사보강철근시점1_G%d", nG+1);

		pRB  = pDeckDataTU->GetRebarByName(szRebar);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
		if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, FALSE, 200, -1, TRUE);

		szText.Format("사보강철근_G%d_시점2", nG+1);
		szRebar.Format("사보강철근시점2_G%d", nG+1);

		pRB  = pDeckDataTU->GetRebarByName(szRebar);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
		if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, FALSE, 400, -1, TRUE);
		*pDom << Dom;
	}
	else
	{
		szText.Format("사보강철근_G%d_시점", nG+1);
		szRebar.Format("사보강철근시점_G%d", nG+1);

		pRB  = pDeckDataTU->GetRebarByName(szRebar);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
		if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 2, FALSE, 200, -1, TRUE);
		*pDom << Dom;
	}
	//=====================================================
	//	종점
	if((bEndCutAngle90 || bEndCutAngle90_Inv))
	{
		szText.Format("사보강철근_G%d_종점1", nG+1);
		szRebar.Format("사보강철근종점1_G%d", nG+1);

		pRB  = pDeckDataTU->GetRebarByName(szRebar);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
		if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, FALSE, 200);

		szText.Format("사보강철근_G%d_종점2", nG+1);
		szRebar.Format("사보강철근종점2_G%d", nG+1);

		pRB  = pDeckDataTU->GetRebarByName(szRebar);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
		if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, FALSE, 400);
		*pDom << Dom;
	}
	else
	{
		szText.Format("사보강철근_G%d_종점", nG+1);
		szRebar.Format("사보강철근종점_G%d", nG+1);

		pRB  = pDeckDataTU->GetRebarByName(szRebar);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
		if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 3, FALSE, 200, -1, FALSE);
		*pDom << Dom;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 상하부철근
	//	하부
	szText.Format("하부철근_G%d_시점", nG+1);
	szRebar.Format("하면철근_G%d", nG+1);

	dExtendL	= dCTC + pDeckDataTU->m_dCoverSd + 600;
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, dExtendL);

	szText.Format("하부철근_G%d_종점", nG+1);
	
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, dExtendL);

	//	상부
	dExtendL	= dCTC + pDeckDataTU->m_dCoverSd + 200;
	szText.Format("상부철근_G%d_시점", nG+1);
	szRebar.Format("상면철근_G%d", nG+1);

	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, dExtendL);

	szText.Format("상부철근_G%d_종점", nG+1);
	
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, dExtendL);
	*pDom << Dom;

	//////////////////////////////////////////////////////////////////////////
	// 모서리철근
	dExtendL	= pDeckDataTU->m_dCoverSd + 200;
	szText.Format("모서리철근_G%d_시점", nG+1);
	szRebar.Format("모서리철근_G%d", nG+1);

	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, dExtendL);

	szText.Format("모서리철근_G%d_종점", nG+1);
	
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, dExtendL);
	*pDom << Dom;
}

void CDrawDBBaseRebarTU::DrawMarkJong(CDomyun *pDom, long nG)
{
	CADeckDataTU	*pDeckDataTU= m_pDataMng->GetDeckDataTU();
	CPlateBridgeApp *pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CRebar			*pRB		= NULL;

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pDataMng);

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText = _T(""), szRebar = _T("");
	CDPointArray	xyArr;
	double dLengthExt = 0;

	//////////////////////////////////////////////////////////////////////////
	// 일반부 스트럽 철근
	szText.Format("스트럽일반_G%d_시점", nG+1);
	szRebar.Format("스트럽일반_G%d", nG+1);

	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkPoint(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, pDeckDataTU->m_dCoverUp+400);

	szText.Format("스트럽일반_G%d_종점", nG+1);
	xyArr.RemoveAll();
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkPoint(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, pDeckDataTU->m_dCoverUp+400);
	*pDom << Dom;

	//////////////////////////////////////////////////////////////////////////
	// 헌치
	szText.Format("헌치철근종단_G%d_시점", nG+1);
	szRebar.Format("헌치철근시점_G%d", nG+1);
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkPoint(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE);

	szText.Format("헌치철근종단_G%d_종점", nG+1);
	szRebar.Format("헌치철근종점_G%d", nG+1);
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkPoint(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE);

//	// 단부 스트럽
//	szText.Format("스트럽단부_G%d_시점", nG+1);
//	szRebar.Format("스트럽단부_G%d", nG+1);
//
//	xyArr.RemoveAll();
//	pRB  = pDeckDataTU->GetRebarByName(szRebar);
//	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
//	if(pRB)	SymDom.RebarMarkLineLower(&Dom, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), TRUE);
//
//	szText.Format("스트럽단부_G%d_종점", nG+1);
//	xyArr.RemoveAll();
//	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
//	if(pRB)	SymDom.RebarMarkLineLower(&Dom, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE);
//	*pDom << Dom;

	// 일반부 측면철근
	szText.Format("측면철근일반_G%d_시점", nG+1);
	szRebar.Format("측면철근일반_G%d", nG+1);

	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkLineUpper(&Dom, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE);

	szText.Format("측면철근일반_G%d_종점", nG+1);
	xyArr.RemoveAll();
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkLineUpper(&Dom, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE);
	*pDom << Dom;

	// 단부 측면철근
	szText.Format("측면철근단부_G%d_시점", nG+1);
	szRebar.Format("측면철근단부_G%d", nG+1);

	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	
	{
		CDomyun	Dom1(pDom);
		pOpt->SetEnvType(&Dom1,HCAD_DIML);
		SymDom.RebarMarkPoint(&Dom1, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, 0);
		Dom1.Rotate(xyArr.GetAt(0), CDPoint(0,1));
		*pDom << Dom1;
	}

	szText.Format("측면철근단부_G%d_종점", nG+1);
	xyArr.RemoveAll();
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	
	{
		CDomyun	Dom1(pDom);
		pOpt->SetEnvType(&Dom1,HCAD_DIML);
		SymDom.RebarMarkPoint(&Dom1, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, 0);
		Dom1.Rotate(xyArr.GetAt(0), CDPoint(0,1));
		*pDom << Dom1;
	}
		
	// 앵커 철근
	szText.Format("앵커철근_G%d_시점", nG+1);
	szRebar.Format("앵커철근_G%d", nG+1);
	
	dLengthExt = (pDB->m_BindConc.m_dHeight-pDeckDataTU->m_dCoverDn-pDeckDataTU->m_dCoverUp)/4 + pDeckDataTU->m_dCoverUp + 150;
	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 4, FALSE, dLengthExt, -1, TRUE);

	szText.Format("앵커철근_G%d_종점", nG+1);
	szRebar.Format("앵커철근_G%d", nG+1);
	
	dLengthExt = (pDB->m_BindConc.m_dHeight-pDeckDataTU->m_dCoverDn-pDeckDataTU->m_dCoverUp)/4 + pDeckDataTU->m_dCoverUp + 150;
	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 4, FALSE, dLengthExt, xyArr.GetSize()-2, FALSE);
	*pDom << Dom;
}

void CDrawDBBaseRebarTU::DrawMarkCross(CDomyun *pDom, long nG, BOOL bAnchor, BOOL bDanbuJewon)
{
	CADeckDataTU	*pDeckDataTU= m_pDataMng->GetDeckDataTU();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CRebar			*pRB		= NULL;
	CRebar			*pRB_Danbu	= NULL;

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	CMarkDomyun		mark(&Dom, pOpt);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pDataMng);

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CString			szText = _T(""), szRebar = _T("");
	CDPointArray	xyArr;
	
	//////////////////////////////////////////////////////////////////////////
	szText.Format("스트럽일반_G%d", nG+1);
	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szText);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)
	{
		if(!bDanbuJewon)
			SymDom.RebarMarkLineLower(&Dom, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), FALSE);
		else
		{
			szText.Format("스트럽단부_G%d", nG+1);
			pRB_Danbu  = pDeckDataTU->GetRebarByName(szText);

			if(pRB_Danbu)	SymDom.RebarMarkLineLower(&Dom, xyArr.GetAt(0), pRB_Danbu->m_sMark, pRB_Danbu->GetStringDia(), FALSE);
		}
	}

	if(bAnchor)
	{
		szRebar.Format("헌치철근시점_G%d", nG+1);
		szText.Format("헌치철근횡단_G%d", nG+1);
		xyArr.RemoveAll();
		pRB  = pDeckDataTU->GetRebarByName(szRebar);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)	SymDom.RebarMarkLineLower(&Dom, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, (BOOL)pDeckDataTU->m_dCoverUp+200);
	}

	szText.Format("상면철근_G%d", nG+1);
	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szText);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkPoint(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, pDeckDataTU->m_dCoverUp+200);

	szText.Format("하면철근_G%d", nG+1);
	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szText);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	if(pRB)	SymDom.RebarMarkPoint(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, pDeckDataTU->m_dCoverUp+200);

	szText.Format("모서리철근_G%d", nG+1);
	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szText);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	Dom.SetDirection("TOP");
	if(pRB)
	{
		CDomyun	Dom1(pDom);
		pOpt->SetEnvType(&Dom1,HCAD_DIML);
		SymDom.RebarMarkPoint(&Dom1, xyArr, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, 0);
		Dom1.Rotate(xyArr.GetAt(0), CDPoint(0,1));
		*pDom << Dom1;
	}

	szText.Format("측면철근_G%d", nG+1);
	szRebar.Format("측면철근일반_G%d", nG+1);
	xyArr.RemoveAll();
	pRB  = pDeckDataTU->GetRebarByName(szRebar);
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
	Dom.SetDirection("TOP");
	if(pRB)
	{
		CDomyun	Dom1(pDom);
		pOpt->SetEnvType(&Dom1,HCAD_DIML);
		if(!bDanbuJewon)
			SymDom.RebarMarkPoint(&Dom1, xyArr, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, 0);
		else
		{
			szRebar.Format("측면철근단부_G%d", nG+1);
			pRB_Danbu  = pDeckDataTU->GetRebarByName(szRebar);

			SymDom.RebarMarkPoint(&Dom1, xyArr, pRB_Danbu->m_sMark, pRB_Danbu->GetStringDia(), 0, FALSE, 0);
		}
		Dom1.Rotate(xyArr.GetAt(0), CDPoint(0,1));
		*pDom << Dom1;
	}

	if(bAnchor)
	{
		szText.Format("앵커철근_G%d", nG+1);
		xyArr.RemoveAll();
		pRB  = pDeckDataTU->GetRebarByName(szText);
		xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, szText);
		if(pRB)	SymDom.RebarMarkLineUpper(&Dom, xyArr.GetAt(0), pRB->m_sMark, pRB->GetStringDia(), TRUE);
	}

	
	*pDom << Dom;
}

// nType 1	: 0.35에서 우측잘라냄
// nType -1 : 0.65에서 좌측잘라냄
// nType 0	: 잘라낸 부분 lightning 표시
void CDrawDBBaseRebarTU::DrawTrimLine(CDomyun *pDom, long nG, long nType)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= m_pDataMng->GetBridge()->GetGirder(nG);
	CLineInfoApp    *pLineApp   = (CLineInfoApp*)pDB->m_pLine;
	
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

	//////////////////////////////////////////////////////////////////////////

	double dBridgeLength = pBxEnd->GetStation() - pBxStt->GetStation();
	double dStaBridgeStt = pBxStt->GetStation();
	double dStation		 = 0;
	double dDistLeft	 = pGir->m_dGirderCenter - pDB->m_BindConc.m_dWidth/2;
	double dDistRight	 = pGir->m_dGirderCenter + pDB->m_BindConc.m_dWidth/2;

	CDPoint xy1, xy2;

	if(nType != -1)
	{
		dStation = dStaBridgeStt+dBridgeLength*0.35;
		xy1 = pLineApp->GetXyLineDis(dStation, dDistLeft);
		xy2 = pLineApp->GetXyLineDis(dStation, dDistRight);

		if(nType == 0)
			pDom->CutLightningAnyDir(xy1, xy2, pDom->Always(10));
	}

	if(nType != 1)
	{
		dStation = dStaBridgeStt+dBridgeLength*0.65;
		xy1 = pLineApp->GetXyLineDis(dStation, dDistLeft);
		xy2 = pLineApp->GetXyLineDis(dStation, dDistRight);

		if(nType == 0)
			pDom->CutLightningAnyDir(xy1, xy2, pDom->Always(10));
	}

	if(nType != 0)
		pDom->TrimByLineOnSide(xy2, xy1, nType, TRUE);
}

void CDrawDBBaseRebarTU::DrawTrimLineJong(CDomyun *pDom, long nG)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= m_pDataMng->GetBridge()->GetGirder(nG);
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

	//////////////////////////////////////////////////////////////////////////

	double dBridgeLength = pBxEnd->GetStation() - pBxStt->GetStation();
	double dStaBridgeStt = pBxStt->GetStation();
	double dSttY		 = pGir->GetElevationFlangeCamber(dStaBridgeStt+dBridgeLength*0.35)+pDB->m_BindConc.m_dHeight;
	double dEndY		 = pGir->GetElevationFlangeCamber(dStaBridgeStt+dBridgeLength*0.65)-pDB->m_BindConc.m_dHeight;

	CDRect rectTrim(dStaBridgeStt+dBridgeLength*0.35, dSttY, dStaBridgeStt+dBridgeLength*0.65, dEndY);

	pDom->TrimByRect(rectTrim, TRUE);

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_DIML);

	Dom.CutLightningAnyDir(rectTrim.TopLeft(), rectTrim.BottomLeft(), 0);
	Dom.CutLightningAnyDir(rectTrim.TopRight(), rectTrim.BottomRight(), 0);

	*pDom << Dom;
}
