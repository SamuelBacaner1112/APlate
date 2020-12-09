// APlateDrawJong.cpp: implementation of the CAPlateDrawJong class.
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

CAPlateDrawJong::CAPlateDrawJong(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawJong::~CAPlateDrawJong()  
{

}

void CAPlateDrawJong::DrawJDanGirderForInput(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	pOptStd->SetEnvType(pDom, HCAD_STLC);

	CDomyun Dom(pDom);

	DrawGirderLineForInput(&Dom, pBxStt, pBxEnd);
	DrawInOutGateBx       (&Dom, pBxStt, pBxEnd, TRUE);
//	DrawVStiffBx		  (&Dom, pBxStt, pBxEnd, 2, -1, TRUE);
//	DrawHStiffBx		  (&Dom, pBxStt, pBxEnd);
//	DrawJiJumVStiffBx	  (&Dom, pBxStt, pBxEnd, 2, -1, TRUE);
//	DrawJackUpBx          (&Dom, pBxStt, pBxEnd, 2, -1, TRUE);
//	DrawFieldSpliceHatchJong(&Dom, pBxStt, pBxEnd, 0, FALSE, TRUE);
	DrawShoeMarkBx		  (&Dom, pBxStt, pBxEnd, pDom->GetScaleDim(), TRUE);	

	*pDom << Dom;
}

void CAPlateDrawJong::DrawGirderLineForInput(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double ExtendLine)
{
	if(pBxStt && pBxEnd && pBxStt->GetStation() >= pBxEnd->GetStation()) return;

	CPlateGirderApp *pGir = (CPlateGirderApp *)pBxStt->GetGirder();
	
	double dDist     = 0;
	double dStaStt   = pBxStt->GetStation();
	double dThick    = 0;
	double dStaCur	 = pBxStt->GetStation();

	CDPoint sttp1, sttp2, endp1, endp2;
	//////////////////////////////////////////////////////////////////////////
	// 상부
	CDPoint sp,ep,jp;
	sp.x = pBxStt->GetStation();
	sp.y = pGir->GetElevationFlangeCamber(dStaCur, dDist) + dThick;
	sttp1 = sp;
	// 시점마감선
	//pDom->LineTo(sp.x,sp.y,sp.x,sp.y-dThick);
	BOOL bSttTaper = FALSE;
	BOOL bEndTaper = FALSE;
	double dSta = 0.0;
	for(dSta=pBxStt->GetStation(); dSta<pBxEnd->GetStation(); dSta+=500)	
	{		
		dStaCur = dSta+ExtendLine;
		ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur, dDist);
		ep.y = pGir->GetElevationFlangeCamber(dStaCur, dDist) + dThick;
		pDom->LineTo(sp,ep);
 		bSttTaper = FALSE;
		bEndTaper = FALSE;		
		sp = ep;		
	}
	dStaCur = pBxEnd->GetStation()+ExtendLine;
	ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur, dDist);
	ep.y = pGir->GetElevationFlangeCamber(dStaCur, dDist) + dThick;
	endp1 = ep;
	pDom->LineTo(sp,ep);	
	// 종점마감선
	//pDom->LineTo(sp.x,sp.y,sp.x,sp.y-dThick);

//	CDPoint xyUp = ep;

	//////////////////////////////////////////////////////////////////////////
	// 하부
	sp.x = pBxStt->GetStation();
	sp.y = pGir->GetElevationFlangeCamber(pBxStt->GetStation(),0,CDPoint(0,1), FALSE) - dThick;
	sttp2 = sp;
	// 시점마감선
	//pDom->LineTo(sp.x,sp.y,sp.x,sp.y+dThick);
	bSttTaper = FALSE;
	bEndTaper = FALSE;
	CDPoint xyDn; 
	long nCount = 0;
	for( dSta=pBxStt->GetStation(); dSta<pBxEnd->GetStation(); dSta+=100)	
	{
		dStaCur = dSta+ExtendLine;
		ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur, dDist);
		ep.y = pGir->GetElevationFlangeCamber(dStaCur, 0, CDPoint(0,1),FALSE) - dThick;

		pDom->LineTo(sp,ep);
 		bSttTaper = FALSE;
		bEndTaper = FALSE;
		sp = ep;
		nCount++;
	}
	dStaCur = pBxEnd->GetStation()+ExtendLine;
	ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur, dDist);
	ep.y = pGir->GetElevationFlangeCamber(dStaCur, 0, CDPoint(0,1),FALSE) - dThick;
	endp2 = ep;
	pDom->LineTo(sp,ep);

	if(!pBxStt->IsState(BX_STT_GIRDER | BX_END_GIRDER)) pDom->LineTo(sttp1.x,sttp1.y,sttp1.x,sttp2.y);
	if(!pBxEnd->IsState(BX_STT_GIRDER | BX_END_GIRDER)) pDom->LineTo(endp1.x,endp1.y,endp1.x,endp2.y);
	// 종점마감선
	//pDom->LineTo(sp.x,sp.y,sp.x,sp.y+dThick);

//	pDom->CutLightningAnyDir(xyUp, xyDn, pDom->Always(2));

}

void CAPlateDrawJong::DrawGirderLineBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine, double ExtendLine, double dFlangeThickScalce, double dGirderHeightScale, BOOL bBindHide)
{
	if(pBxStt && pBxEnd && pBxStt->GetStation() >= pBxEnd->GetStation()) return;

	CARoadOptionStd *pOptStd= m_pDataMng->GetOptionStd();
	BOOL bIsTU = m_pDataMng->GetBridge()->IsTUGir();
	
	pOptStd->SetEnvLType(pDom, bIsTU ? LT_DOT_LINE : LT_SOLID);
	DrawGirderLineUpperBx(pDom, pBxStt, pBxEnd, FALSE, ExtendLine, dFlangeThickScalce, dGirderHeightScale);		// 상판 기준선
	pOptStd->SetEnvLType(pDom, LT_SOLID);
	DrawGirderLineLowerBx(pDom, pBxStt, pBxEnd, FALSE, ExtendLine, dFlangeThickScalce, dGirderHeightScale);		// 하판 기준선

	if(m_pDataMng->GetBridge()->IsTUGir())
		DrawBindLineBx(pDom, pBxStt, pBxEnd, bBindHide);

	if(nLine!=2) return;
	pOptStd->SetEnvLType(pDom, bIsTU ? LT_DOT_LINE : LT_SOLID);
	DrawGirderLineUpperBx(pDom, pBxStt, pBxEnd, TRUE, ExtendLine, dFlangeThickScalce, dGirderHeightScale);		// 상판 두께선
	pOptStd->SetEnvLType(pDom, LT_SOLID);
	DrawGirderLineLowerBx(pDom, pBxStt, pBxEnd, TRUE, ExtendLine, dFlangeThickScalce, dGirderHeightScale);		// 하판 두께선	
}

void CAPlateDrawJong::DrawBindLineBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bBindHide)
{
	if(pBxStt && pBxEnd && pBxStt->GetStation() >= pBxEnd->GetStation()) return;
	CPlateBridgeApp	*pDB	= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	= (CPlateGirderApp *)pBxStt->GetGirder();
	CARoadOptionStd *pOptStd= m_pDataMng->GetOptionStd();

	CPlateBxFinder Finder(pGir);		
	CPlateBasicIndex *pBx	= Finder.GetBxFirst(BX_UPPER, pBxStt, pBxEnd, TRUE, TRUE);
	double	dStaStt	= pBxStt->GetStation();

	double	dBindConcDis		= pDB->m_BindConc.m_dDis;
	double	dBindH				= pDB->m_BindConc.m_dHeight;
	double	dBindConcLen_Stt	= pDB->m_dLengthTUBindConc_Stt;
	double	dBindConcLen_End	= pDB->m_dLengthTUBindConc_End;
	double	dBindBottomY		= dBindConcDis;

	pOptStd->SetEnvType(pDom, HCAD_STLC);

	CPlateBasicIndex *pBxPre = pBx;
	pBx = Finder.GetBxNext();
	while(pBx)
	{
		double	dPreSta	= pBxPre->GetStation();
		if(pBxPre->IsState(BX_STT_GIRDER))
		{
			dPreSta -= dBindConcLen_Stt;
			dStaStt	= dPreSta;
		}
		double	dPreX	= dStaStt + pGir->GetLengthDimType(dStaStt, dPreSta);
		double	dPreY	= pGir->GetElevationFlangeCamber(dPreSta);

		double	dCurSta	= pBx->GetStation();
		if(pBx->IsState(BX_END_GIRDER))
			dCurSta += dBindConcLen_End;
		double	dCurX	= dStaStt + pGir->GetLengthDimType(dStaStt, dCurSta);
		double	dCurY	= pGir->GetElevationFlangeCamber(dCurSta);

		if(bBindHide)
			pOptStd->SetEnvLType(pDom, LT_DOT_LINE);
		pDom->LineTo(dPreX, dPreY+(dBindH-dBindConcDis), dCurX, dCurY+(dBindH-dBindConcDis));
		pOptStd->SetEnvLType(pDom, LT_SOLID);
		pDom->LineTo(dPreX, dPreY-dBindBottomY, dCurX, dCurY-dBindBottomY);

		if(pBxPre->IsState(BX_STT_GIRDER))
			pDom->LineTo(dPreX, dPreY+(dBindH-dBindConcDis), dPreX, dPreY-dBindBottomY);
		else if(pBx->IsState(BX_END_GIRDER))
			pDom->LineTo(dCurX, dCurY+(dBindH-dBindConcDis), dCurX, dCurY-dBindBottomY);

		pBxPre	= pBx;
		pBx = Finder.GetBxNext();
	}
}

//기능 : 측면도에서 상판 그리기(상판 하면, 상면)
void CAPlateDrawJong::DrawGirderLineUpperBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bThickLine, double ExtendLine, double dFlangeThickScalce, double dGirderHeightScale)
{	
	if(pBxStt && pBxEnd && pBxStt->GetStation() >= pBxEnd->GetStation()) return;  	
	CGlobarOption   *pGlopt  = m_pDataMng->GetGlobalOption();
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = 0;
	__int64 BxFlagState = 0;
	BxFlag = BX_UPPER;
	BxFlagState = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE | BX_SPLICE;
		
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, TRUE);

	double dDist     = 0;
	double dStaStt   = pBxStt->GetStation();
	double dThick    = bThickLine ? pBx->GetFactChain(G_F_U)->m_dFactChainThick * (pGlopt->GetDesignHeigtBaseUpper()==0 ? -1 : 1) : 0;
	
	if(bThickLine) dThick *= dFlangeThickScalce;
	
	double dThickPre = dThick;

	double dStaCur	 = pBx->GetStation();
	BOOL bBogus = FALSE;

	CDPoint sp,ep,jp;
	sp.x = dStaCur;
	sp.y = pGir->GetElevationFlangeCamber(dStaCur) + dThick;
	// 시점마감선
	pDom->LineTo(sp.x,sp.y,sp.x,sp.y-dThick);
	BOOL bSttTaper = FALSE;
	BOOL bEndTaper = FALSE;
	while(pBx)
	{
		dStaCur = pBx->GetStation()+ExtendLine;
		ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur, dDist);
		ep.y = pGir->GetElevationFlangeCamber(dStaCur) + dThick;
		if(bBogus && bThickLine)
			DrawTaperLine(pDom,sp,ep,TRUE,bSttTaper,bEndTaper);
		else
			pDom->LineTo(sp,ep);
 		bSttTaper = FALSE;
		bEndTaper = FALSE;
		if(bThickLine)
		{
			double dThickComp = dThick;
			dThick = pBx->GetFactChain(G_F_U)->m_dFactChainThick * (pGlopt->GetDesignHeigtBaseUpper()==0 ? -1 : 1) ;
			if(bThickLine)
				if(bThickLine) dThick *= dFlangeThickScalce;
			// 가선처리 (ONLY FOR SHOP)
			if(bBogus && bThickLine)
			{
				if(dThick>dThickPre) 
				{
					bSttTaper = TRUE;
				}
				// 다음 공장이음을 찾아서 bEndTaper SETTING
				CPlateBasicIndex *pBxFact = Finder.GetBxFixNext();
				if(pBxFact && pBxFact->IsState(BxFlagState))
				{
					double dThickNext = pBxFact->GetFactChain(G_F_U)->m_dFactChainThick;
					if(dThickNext<dThick)
						bEndTaper = TRUE;
				}
			}
			jp.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur, dDist);
			jp.y = pGir->GetElevationFlangeCamber(dStaCur);

			if(dThickComp!=dThick)
			{
				double deltaY = pGlopt->GetDesignHeigtBaseUpper()==0 ? (bBogus ? max(dThick,dThickComp) : min(dThick,dThickComp)) : (bBogus ? min(dThick,dThickComp) : max(dThick,dThickComp));
				pDom->LineTo(jp,CDPoint(jp.x,jp.y+deltaY));
			}
			// ep를 두께차이만큼 조정..
			ep.y += (dThick - dThickComp);
			dThickPre = pBx->GetFactChain(G_F_U)->m_dFactChainThick;
			if(bThickLine)
				if(bThickLine) dThickPre *= dFlangeThickScalce;
		}
		sp = ep;
		pBx = Finder.GetBxNext();
	}
	// 종점마감선
	pDom->LineTo(sp.x,sp.y,sp.x,sp.y-dThick);
}


void CAPlateDrawJong::DrawGirderLineLowerBx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bThickLine, double ExtendLine, double dFlangeThickScalce, double dGirderHeightScale)
{	
	if(pBxStt && pBxEnd && pBxStt->GetStation() >= pBxEnd->GetStation()) return;  		
	CGlobarOption   *pGlopt  = m_pDataMng->GetGlobalOption();
	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_LOWER;
	__int64 BxFlagState = BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE | BX_SPLICE;	
		
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	double dDist     = 0;
	double dStaStt   = pBxStt->GetStation();
	double dThick    = bThickLine ? pBx->GetFactChain(G_F_L)->m_dFactChainThick * (pGlopt->GetDesignHeigtBaseLower()==0 ? 1 : -1) : 0;
	
	if(bThickLine) dThick *= dFlangeThickScalce;

	double dThickPre = dThick;
	double dStaCur   = pBx->GetStation();
	double	dHeight	 = pGir->GetHeightGirderByStaAng(dStaCur);
	//double dBogusFactor = GetBogusFactor(pBx,G_F_L,nDir,pBxStt,pBxEnd);			
	//BOOL bBogus = pStd->GetBogusLine();

	// 가선처리 (ONLY FOR SHOP)
	//if(bBogus && bThickLine)
	//	dThick = pStd->GetBogusThick()*dBogusFactor;
	CDPoint sp,ep,jp;
	
	sp.x = dStaCur;
	sp.y = pGir->GetElevationFlangeCamber(dStaCur,0,CDPoint(0,1), FALSE) - dThick - dHeight*(dGirderHeightScale-1);
	// 시점마감선
	pDomP->LineTo(sp.x,sp.y,sp.x,sp.y+dThick);
	BOOL bSttTaper = FALSE;
	BOOL bEndTaper = FALSE;
	
	while(pBx)
	{
		dStaCur		= pBx->GetStation()+ExtendLine;
		//단면요약도 시종점 탭 부분 사선으로 되는 것 방지하기 위해...070308...KB....
		dHeight		= pGir->GetHeightGirderByStaAng(pBxStt->GetStation());
		ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur, dDist);
		ep.y = pGir->GetElevationFlangeCamber(dStaCur, 0, CDPoint(0,1),FALSE) - dThick - dHeight*(dGirderHeightScale-1);
//		if(/*bBogus && * /bThickLine)
//			DrawTaperLine(pDomP,sp,ep,FALSE,bSttTaper,bEndTaper);
//		else
			pDomP->LineTo(sp,ep);
 		bSttTaper = FALSE;
		bEndTaper = FALSE;
		if(bThickLine && pBx->GetFactChain(G_F_L)!=NULL)
		{
			double dThickComp = dThick;
			dThick = pBx->GetFactChain(G_F_L)->m_dFactChainThick * (pGlopt->GetDesignHeigtBaseLower()==0 ? 1 : -1);
			if(bThickLine)
				if(bThickLine) dThick *= dFlangeThickScalce;
			// 가선처리 (ONLY FOR SHOP)
			if(/*bBogus &&*/ bThickLine)
			{
				if(dThick>dThickPre) 
				{
//					dBogusFactor += pStd->GetBogusRate();
					bSttTaper = TRUE;
				}
//				if(dThick<dThickPre) 
//					dBogusFactor -= pStd->GetBogusRate();
				// 다음 공장이음을 찾아서 bEndTaper SETTING
				CPlateBasicIndex *pBxFact = Finder.GetBxFixNext();
				if(pBxFact && pBxFact->IsState(BxFlagState))
				{
					if(pBxFact->GetFactChain(G_F_L)!=NULL)
					{
						double dThickNext = pBxFact->GetFactChain(G_F_L)->m_dFactChainThick;
						if(dThickNext<dThick)
							bEndTaper = TRUE;
					}
				}
				//dThick = pStd->GetBogusThick()*dBogusFactor;
			}
			//
			double	dHeightGirder	= pGir->GetHeightGirderByStaAng(pBxStt->GetStation());
			jp.x = ep.x;
			jp.y = pGir->GetElevationFlangeCamber(dStaCur, 0, CDPoint(0,1),FALSE)-dHeightGirder*(dGirderHeightScale-1);
			if(dThickComp!=dThick)
			{
//				double deltaY = pGlopt->GetDesignHeigtBaseLower()==0 ? min(dThick,dThickComp) : max(dThick,dThickComp);
				//double deltaY = /*bBogus ? min(dThick,dThickComp) : */max(dThick,dThickComp);
				long	nHeightBaseLo		= pGlopt->GetDesignHeigtBaseLower();
				double	dThickFlangeLo		= max(pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick, pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick)*dFlangeThickScalce;
				if(nHeightBaseLo==1)	dThickFlangeLo *= -1;
				
				pDomP->LineTo(jp,CDPoint(jp.x,jp.y-dThickFlangeLo));
			}
			// ep를 두께차이만큼 조정..
			ep.y -= (dThick - dThickComp);
			dThickPre = pBx->GetFactChain(G_F_L)->m_dFactChainThick;
			if(bThickLine)
				if(bThickLine) dThickPre *= dFlangeThickScalce;
		}
		sp = ep;
		pBx = Finder.GetBxNext();
	}
	// 종점마감선
	pDomP->LineTo(sp.x,sp.y,sp.x,sp.y+dThick);

}

// 슬래브두께는 기본두께로 그림 (실제의 두께가아님)
void CAPlateDrawJong::DrawPaveLine(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDir)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();		
	CGlobarOption   *pGlopt  = pBridge->m_pDataManage->GetGlobalOption();

	if(pBxStt && pBxEnd && pBxStt->GetStation() >= pBxEnd->GetStation()) return;
	CPlateGirderApp		*pGir = pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);

	__int64	BxFlag = BX_UPPER;
		
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	double dStaStt = pBxStt->GetStation();
	double dStaCur = pBxStt->GetStation();
	double dDist   = 0;
	double dThickPave	   = pBridge->m_dThickPave;
	double dThickSlab	   = pBridge->m_dThickSlabBase;
	double dSttBlockOutHeight = pGlopt->GetSttExpansionJointHeight();
	double dSttBlockOutWidth  = pGlopt->GetSttExpansionJointWidth();
	double dEndBlockOutHeight = pGlopt->GetEndExpansionJointHeight();
	double dEndBlockOutWidth  = pGlopt->GetEndExpansionJointWidth();
	double dBindGap			= pBridge->m_BindConc.m_dHeight-pBridge->m_BindConc.m_dDeep-pBridge->m_BindConc.m_dDis;

	CDPoint sp,ep;
	sp.x = dStaCur;
	sp.y = pGir->GetElevationFlange(dStaCur) 
		 + pGir->GetHeightFlangeUpperByTaper(dStaCur)+dThickSlab+dThickPave+dBindGap;

	double dTaperH		= pGir->GetHeightFlangeUpperByTaper(dStaCur, 0, CDPoint(0,1), TRUE);
	double dTaperHPre	= dTaperH;
	//

	double dSttX = pGir->GetStationStt()+pBridge->m_dLengthUganStt+dSttBlockOutWidth;
	double dEndX = pGir->GetStationEnd()-pBridge->m_dLengthUganEnd+dEndBlockOutWidth;
	while(pBx)
	{
		// 시점부마감
		if(pBx->IsState(BX_STT_GIRDER))
		{
			double dUgan = pBridge->m_dLengthUganStt;
			double dY;
			dStaCur = pGir->GetStationStt()+dUgan;
			ep.x = pBx->GetStation() - pGir->GetLengthDimType(pBx->GetStation(), dStaCur);
			ep.y = pGir->GetElevationFlangeCamber(pBx->GetStation(),dDist,pBx->GetAngle()) 
				 + pGir->GetHeightFlangeUpperByTaper(pBx->GetStation(),dDist,pBx->GetAngle())+dThickSlab+dThickPave+dBindGap;
			pDomP->LineTo(sp,ep);
//			pDomP->LineTo(sp.x,sp.y-dThickPave,ep.x,ep.y-dThickPave);
			dY = ep.y - pGir->GetHeightFlangeUpperByTaper(pBx->GetStation(),dDist,pBx->GetAngle(), TRUE) - dThickSlab - dThickPave;
			pDomP->LineTo(ep.x,ep.y,ep.x,dY);
			pDomP->LineTo(ep.x,dY,sp.x,dY);
			double slop;
			slop = (ep.x-sp.x) == 0 ? 0 : (ep.y-sp.y)/(ep.x-sp.x);
			pDomP->LineTo(ep.x,ep.y, ep.x+dSttBlockOutWidth, ep.y+dSttBlockOutWidth*slop);
			pDomP->LineTo(ep.x+dSttBlockOutWidth, ep.y+dSttBlockOutWidth*slop, ep.x+dSttBlockOutWidth, ep.y-dSttBlockOutHeight+dSttBlockOutWidth*slop);
			pDomP->LineTo(ep.x+dSttBlockOutWidth, ep.y-dSttBlockOutHeight+dSttBlockOutWidth*slop, ep.x,ep.y-dSttBlockOutHeight);
			pDomP->LineTo(ep.x, ep.y, ep.x, ep.y-dSttBlockOutHeight);

			pDomP->HatchRect(ep.x,ep.y, ep.x+dSttBlockOutWidth, ep.y+dSttBlockOutWidth*slop, 
				ep.x+dSttBlockOutWidth, ep.y-dSttBlockOutHeight+dSttBlockOutWidth*slop, ep.x,ep.y-dSttBlockOutHeight,"ANSI31");
		}
		// 종점부마감
		if(pBx->IsState(BX_END_GIRDER))
		{
			double dUgan = pBridge->m_dLengthUganEnd;
			double dY;
			dStaCur = pGir->GetStationEnd()-dUgan;
			ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur);
			ep.y = pGir->GetElevationFlangeCamber(dStaCur,dDist,pBx->GetAngle()) 
				 + pGir->GetHeightFlangeUpperByTaper(dStaCur,dDist,pBx->GetAngle())+dThickSlab+dThickPave+dBindGap;
			pDomP->LineTo(sp,ep);
//			pDomP->LineTo(sp.x,sp.y-dThickPave,ep.x,ep.y-dThickPave);
			dY = ep.y - pGir->GetHeightFlangeUpperByTaper(pBx->GetStation(),dDist,pBx->GetAngle(), TRUE) - dThickSlab - dThickPave;
			pDomP->LineTo(ep.x,ep.y,ep.x,dY);
			pDomP->LineTo(ep.x,dY,sp.x,dY);
			double slop = (ep.y-sp.y)/(ep.x-sp.x);
			pDomP->LineTo(ep.x,ep.y, ep.x-dEndBlockOutWidth, ep.y-dEndBlockOutWidth*slop);
			pDomP->LineTo(ep.x-dEndBlockOutWidth, ep.y-dEndBlockOutWidth*slop, ep.x-dEndBlockOutWidth, ep.y-dEndBlockOutWidth*slop-dEndBlockOutHeight);
			pDomP->LineTo(ep.x-dEndBlockOutWidth, ep.y-dEndBlockOutWidth*slop-dEndBlockOutHeight, ep.x,ep.y-dEndBlockOutHeight);
			pDomP->LineTo(ep.x,ep.y, ep.x,ep.y-dEndBlockOutHeight);

			pDomP->HatchRect(ep.x,ep.y, ep.x-dEndBlockOutWidth, ep.y-dEndBlockOutWidth*slop, 
				ep.x-dEndBlockOutWidth, ep.y-dEndBlockOutWidth*slop-dEndBlockOutHeight, ep.x,ep.y-dEndBlockOutHeight,"ANSI31");
		}
		dTaperH = pGir->GetHeightFlangeUpperByTaper(pBx->GetStation(),dDist,pBx->GetAngle(),TRUE);
		dStaCur = pBx->GetStation();
		ep.x = dStaStt + pGir->GetLengthDimType(dStaStt, dStaCur);
		ep.y = pGir->GetElevationFlangeCamber(dStaCur,dDist,pBx->GetAngle()) 
			 + pGir->GetHeightFlangeUpperByTaper(dStaCur,dDist,pBx->GetAngle())+dThickSlab+dThickPave+dBindGap;

		pDomP->LineTo(sp,ep);
//		if(!pBx->IsState(BX_STT_GIRDER|BX_END_GIRDER))
		if(dSttX<sp.x && dEndX>ep.x)		pDomP->LineTo(sp.x,sp.y-dThickPave,ep.x,ep.y-dThickPave);
		else if(dSttX>sp.x && dSttX<ep.x)	pDomP->LineTo(dSttX,sp.y-dThickPave,ep.x,ep.y-dThickPave);
		else if(dEndX<ep.x && dEndX>sp.x)	pDomP->LineTo(sp.x,sp.y-dThickPave,dEndX,ep.y-dThickPave);
		pDomP->LineTo(sp.x, sp.y-dTaperHPre-dThickSlab-dThickPave, ep.x, ep.y-dTaperH-dThickSlab-dThickPave);
		dTaperHPre = dTaperH;

		sp = ep;
		pBx = Finder.GetBxNext();
	}
}

void CAPlateDrawJong::DrawInOutGateBx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bFacePlate, long nType, double dFlangeThickScalce, double dGirderHeightScale)
{  	
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = (CPlateGirderApp *)pBxStt->GetGirder();
	CPlateGirderApp *pGirCen = (CPlateGirderApp *)pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	__int64 BxFlag = BX_STT_GIRDER|BX_END_GIRDER;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, TRUE);

	double StdSta = pBxStt->GetStation();
	CDPoint xy;

	CDomyun* pDom = new CDomyun(pDomP);

	while(pBx)
	{
		xy.y = pGir->GetElevationFlangeCamber(pBx->GetStation(),0);
		xy.x = StdSta + pGir->GetLengthDimType(StdSta,pBx->GetStation(),0);

		double W  = pBridge->m_dWidthInOutEnd;
		double C  = pBridge->m_dHeightTaperEnd;
		double E  = pBridge->m_dHeightInOutEnd;
		double R  = pBridge->m_dRadiusInOutEnd;
		double G  = W*(-1.0) + R;
		double H  = pGir->GetHeightGirderByStaAng(pBx->GetStation(),0,CDPoint(0,1),FALSE);//*dGirderHeightScale;
		double D1 = pBridge->m_dHeightInOutLowerEnd;
		double D  = H - E - D1;

		// 시점측에 인입구를 설치하지 않는 경우
		if(bFacePlate==FALSE) 
		{
			pDom->LineTo(xy.x,xy.y,xy.x,xy.y-H);
			pBx = Finder.GetBxNext();
			continue;
		}
		// 종점측에 인입구를 설치하지 않는 경우
		if(bFacePlate==FALSE) 
		{
			pDom->LineTo(xy.x,xy.y,xy.x,xy.y-H);
			pBx = Finder.GetBxNext();
			continue;
		}

		BOOL bCCutting = pBridge->m_dWidthInOutEnd ? TRUE : FALSE;
		if(pBx->IsState(BX_STT_GIRDER)) 
		{
			W = pBridge->m_dWidthInOutStt;
			C = pBridge->m_dHeightTaperStt;
			E = pBridge->m_dHeightInOutStt;
			R = pBridge->m_dRadiusInOutStt;
			D1 = pBridge->m_dHeightInOutLowerStt;
			D  = H - E - D1;							// 출입구상단~거더상단
			G = W - R;
			bCCutting = pBridge->m_dWidthInOutStt ? TRUE : FALSE;
		}
		double dCutLen = bCCutting ? 10 : 0;
		double sW	   = pBx->IsState(BX_STT_GIRDER) ? 1.0 : -1.0;

		pDom->LineTo(xy.x,xy.y ,xy.x ,xy.y-D+dCutLen);
		if(bCCutting)
			pDom->LineTo(xy.x ,xy.y-D+dCutLen, xy.x+dCutLen*sW,xy.y-D);
		if(D!=0 && E!=0 && R!=0) 
		{
			pDom->LineTo(xy.x+dCutLen*sW,xy.y-D	,xy.x+G,xy.y-D);
			pDom->LineTo(xy.x+dCutLen*sW,xy.y-D-E  ,xy.x+G,xy.y-D-E);
			pDom->LineTo(xy.x,xy.y-D-E-dCutLen ,xy.x  ,xy.y-H);
			if(bCCutting)
			{
				pDom->LineTo(xy.x,xy.y-D-E-dCutLen ,xy.x+dCutLen*sW ,xy.y-D-E);
				pDom->LineTo(xy.x,xy.y-D-E,xy.x+dCutLen*sW ,xy.y-D-E);
				pDom->LineTo(xy.x,xy.y-D, xy.x+dCutLen*sW,xy.y-D);
			}
		}
		if(D!=0 && E!=0 && R!=0)
		{
			if(pBx->IsState(BX_STT_GIRDER)) 
			{
				pDom->LineTo(xy.x+G+R,xy.y-D-R ,xy.x+G+R,xy.y-D-E+R);
				pDom->Arc(xy.x+G,xy.y-D-R  ,R,  0,90);	// 기준점,반지름,시작각도,진행각도
				pDom->Arc(xy.x+G,xy.y-D-E+R,R,270,90);
				if(pBridge->m_cFacePlateUse && bFacePlate)
				{
					double T  = 10; // 부재마크 없음
					double R2 = R-T;
					pDom->LineTo(xy.x+G+R-T,xy.y-D-R ,xy.x+G+R-T,xy.y-D-E+R);
					pDom->Arc(xy.x+G,xy.y-D-R  ,R2,  0,90);	// 기준점,반지름,시작각도,진행각도
					pDom->Arc(xy.x+G,xy.y-D-E+R,R2,270,90);
					if(G>0)
					{
						pDom->LineTo(xy.x,xy.y-D-T   ,xy.x+G,xy.y-D-T);
						pDom->LineTo(xy.x,xy.y-D-E+T ,xy.x+G,xy.y-D-E+T);
						pDom->LineTo(xy.x,xy.y-D-T   ,xy.x,xy.y-D);
						pDom->LineTo(xy.x,xy.y-D-E+T ,xy.x,xy.y-D-E);
					}
					else
					{
						pDom->LineTo(xy.x,xy.y-D-T ,xy.x,xy.y-D);
						pDom->LineTo(xy.x,xy.y-D-E+T ,xy.x,xy.y-D-E);
					}
					if(nType & DRT_TEXTJEWON)
					{
						pOptStd->SetEnvType(pDom,HCAD_DIML);
						CDPoint xy1;
						xy1.x = xy.x+G+R2*cos(ConstPi/4);
						xy1.y = xy.y-D-R+R2*cos(ConstPi/4);
						CVector vJewon = pGir->GetJewonTotal(GI_FA,pBx);
						CString str;
						if(nType & DRT_DETMARK) 
						{
							str = "DET #N";
							pDom->GiSiArrowExt(xy1.x,xy1.y,pDom->Always(8),-pDom->GetTextWidth(str),5,str);
						}
						else
						{
							////////////////////
							//str = pOpt->GetJewonString(1,vJewon.x,vJewon.y,vJewon.z);					
							pDom->GiSiArrowExt(xy1.x,xy1.y,pDom->Always(8),-pDom->GetTextWidth(str),5,str);
							xy1.x = xy.x+G+R*cos(ConstPi/4);
							xy1.y = xy.y-D-E+R-R*cos(ConstPi/4);
							str.Format("R=%.0f",R);
							pDom->GiSiArrowExt(xy1.x,xy1.y,pDom->Always(8),-pDom->GetTextWidth(str),3,str);
						}
						//
						xy1.y = xy.y-(D+pDom->Always(5));//cos(ConstPi/4);
						pDom->SetDirection("BOTTOM");
						pDom->DimMoveTo(xy.x,xy1.y,0);
						pDom->DimLineTo(W,0,COMMA(W));
					}					
				}
			}
			else 
			{
				pDom->LineTo(xy.x+G+R*(-1),xy.y-D-R ,xy.x+G+R*(-1),xy.y-D-E+R);
				pDom->Arc(xy.x+G,xy.y-D-R  ,R, 90,90);
				pDom->Arc(xy.x+G,xy.y-D-E+R,R,180,90);
				if(pBridge->m_cFacePlateUse && bFacePlate)
				{
					double T  = 10;
					double R2 = R-T;
					pDom->LineTo(xy.x+G+R*(-1)+T,xy.y-D-R ,xy.x+G+R*(-1)+T,xy.y-D-E+R);
					pDom->Arc(xy.x+G,xy.y-D-R  ,R2, 90,90);
					pDom->Arc(xy.x+G,xy.y-D-E+R,R2,180,90);
					if(G<0)
					{
						pDom->LineTo(xy.x,xy.y-D-T   ,xy.x+G,xy.y-D-T);
						pDom->LineTo(xy.x,xy.y-D-E+T ,xy.x+G,xy.y-D-E+T);
						pDom->LineTo(xy.x,xy.y-D-T   ,xy.x,xy.y-D);
						pDom->LineTo(xy.x,xy.y-D-E+T ,xy.x,xy.y-D-E);
					}
					else
					{
						pDom->LineTo(xy.x,xy.y-D-T   ,xy.x,xy.y-D);
						pDom->LineTo(xy.x,xy.y-D-E+T ,xy.x,xy.y-D-E);
					}
					if(nType & DRT_TEXTJEWON)
					{
						pOptStd->SetEnvType(pDom,HCAD_DIML);
						CDPoint xy1;
						xy1.x = xy.x+G-R2*cos(ConstPi/4);
						xy1.y = xy.y-D-R+R2*cos(ConstPi/4);
						CVector vJewon = pGir->GetJewonTotal(GI_FA,pBx);
						CString str;
						if(nType & DRT_DETMARK) 
						{
							str = "DET #N";
							pDom->GiSiArrowExt(xy1.x,xy1.y,pDom->Always(8),pDom->GetTextWidth(str),7,str);
						}
						else
						{
//							str = pOpt->GetJewonString(1,vJewon.x,vJewon.y,vJewon.z);					
							pDom->GiSiArrowExt(xy1.x,xy1.y,pDom->Always(8),pDom->GetTextWidth(str),7,str);
							xy1.x = xy.x+G-R*cos(ConstPi/4);
							xy1.y = xy.y-D-E+R-R*cos(ConstPi/4);
							str.Format("R=%.0f",R);
							pDom->GiSiArrowExt(xy1.x,xy1.y,pDom->Always(8),pDom->GetTextWidth(str),1,str);
						}
						//
						xy1.y = xy.y-(D+pDom->Always(5));//cos(ConstPi/4);
						pDom->SetDirection("BOTTOM");
						pDom->DimMoveTo(xy.x-W,xy1.y,0);
						pDom->DimLineTo(W,0,COMMA(W));
					}					
				}
			}
		}
		//
		pBx = Finder.GetBxNext();
	}
	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawJong::DrawTaperLine(CDomyun *pDom, CDPoint sp, CDPoint ep, BOOL bUpper, 
									BOOL bSttTaper, BOOL bEndTaper)
{
	CDomyun Dom(pDom);

	CDPoint sp1 = sp;
	CDPoint sp2 = sp;
	CDPoint ep1 = ep;
	CDPoint ep2 = ep;

	double dSlop  = (ep.y-sp.y)/(ep.x-sp.x);
	//////////////////////// 수정요망 ///////////////////////////////
	double dThick = 10;//pStd->GetBogusThick()*pStd->GetBogusRate();
	double dRate  = 5.0; // 1:5
	double sw	  = bUpper ? 1.0 : -1.0;

	////////////
	if(bSttTaper)
	{
		sp1.y = sp.y - dThick*sw;
		sp2.x = sp.x + dThick*dRate;
		sp2.y = sp.y + dThick*dRate*dSlop;
		Dom.LineTo(sp1,sp2);
	}
	if(bEndTaper)
	{
		ep2.y = ep.y - dThick*sw;
		ep1.x = ep.x - dThick*dRate;
		ep1.y = ep.y - dThick*dRate*dSlop;
		Dom.LineTo(ep1,ep2);
	}
	Dom.LineTo(sp2,ep1);

	*pDom << Dom;
}

void CAPlateDrawJong::DrawFacePlateStt(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	double dT = 10;
	double dW = pDB->m_dWidthInOutStt;
	double dH = pDB->m_dHeightInOutStt;
	double dR = pDB->m_dRadiusInOutStt;
	double dGap   = dW <= pDB->m_dRadiusInOutStt ? 0 : dT;
	double dSlabT		= pDB->m_dThickSlabBase;
	double dPaveT		= pDB->m_dThickPave;
	double dHeightInOut = dSlabT + dPaveT + pDB->m_dHeightTaperStt + pGir->GetHeightInOutUpper(TRUE, FALSE);
	double dLengthGirder = pDB->m_dLengthUganStt + pDB->m_dLengthSlabGirderStt;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	if( pDB->m_cFacePlateRoundType )
	{
		if(pDB->m_cFacePlateUse)
		{
			Dom.LineTo(dGap, 0, dGap, -dT);
			Dom.LineTo(dW-dR-dT, -dT) ;
			Dom.Arc(dW-dR-dT, -(dR+dT), dR,0,90);
			Dom.LineTo(dW-dT, -(dR+dT),dW-dT, -dH+dR+dT) ;

			Dom.Arc(dW-(dR+dT), -dH +(dR+dT),dR,270,90);
			Dom.LineTo(dW-(dR+dT), -dH+dT, dGap, -dH+dT);
			Dom.LineTo(dGap, -dH);
		}
	}
	else
	{
		if(pDB->m_cFacePlateUse)
		{
			dT *= 2;
			Dom.MoveTo(dGap, dT);
			Dom.LineTo(dW+dT, dT);
			Dom.LineTo(dW+dT, -(dH + dT));
			Dom.LineTo(dGap, -(dH + dT));
		}
	}
	
	Dom.Move(dLengthGirder,-dHeightInOut-pDB->m_BindConc.m_dHeight+pDB->m_BindConc.m_dDeep+pDB->m_BindConc.m_dDis);

	*pDom << Dom;		
}

void CAPlateDrawJong::DrawFacePlateEnd(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir = pDB->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double dT = 10;
	double dW = pDB->m_dWidthInOutEnd;
	double dH = pDB->m_dHeightInOutEnd;
	double dR = pDB->m_dRadiusInOutEnd;
	double dGap   = dW <= pDB->m_dRadiusInOutEnd ? 0 : dT;
	double dSlabT		= pDB->m_dThickSlabBase;
	double dPaveT		= pDB->m_dThickPave;
	double dHeightInOut = dSlabT + dPaveT + pDB->m_dHeightTaperEnd + pGir->GetHeightInOutUpper(FALSE, FALSE);
	double dLengthGirder = pDB->m_dLengthUganEnd+pDB->m_dLengthSlabGirderEnd;
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	if( pDB->m_cFacePlateRoundType )
	{
		if(pDB->m_cFacePlateUse)
		{
			Dom.LineTo(-dGap, 0, -dGap, -dT) ;
			Dom.LineTo(-dW+dR+dT, -dT) ;
			Dom.Arc(-dW+dR+dT, -(dR+dT), dR,90,90);
			Dom.LineTo(-dW+dT, -(dR+dT),-dW+dT, -dH+dR+dT) ;

			Dom.Arc(-dW+(dR+dT), -dH +(dR+dT),dR,270,-90);
			Dom.LineTo(-dW+(dR+dT), -dH+dT, -dGap, -dH+dT);
			Dom.LineTo(-dGap, -dH);
		}
	}
	else
	{
		if(pDB->m_cFacePlateUse)
		{
			dT *= 2;
			Dom.MoveTo(-dGap, dT);
			Dom.LineTo(-dW-dT, dT) ;
			Dom.LineTo(-dW-dT, -(dH + dT)) ;
			Dom.LineTo(-dGap, -(dH + dT));
		}
	}
	
	Dom.Move(-dLengthGirder,-dHeightInOut-pDB->m_BindConc.m_dHeight+pDB->m_BindConc.m_dDeep+pDB->m_BindConc.m_dDis);

	*pDom << Dom;	
}

void CAPlateDrawJong::DrawJongSttEnd(CDomyun *pDom, BOOL bStt)
{
	CPlateBridgeApp *pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir		= pDB->GetGirder(-1);
	CGlobarOption   *pGlopt		= m_pDataMng->GetGlobalOption();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();	

	double dH = bStt ? pGlopt->GetSttExpansionJointHeight()	: pGlopt->GetEndExpansionJointHeight();
	double dW = bStt ? pGlopt->GetSttExpansionJointWidth()	: pGlopt->GetEndExpansionJointWidth();
	
	double dHeightG			= bStt ? pGir->GetHeightGirderByJijum(0, FALSE) : pGir->GetHeightGirderByJijum(pDB->m_nQtyJigan, FALSE);
	double dSlabT			= pDB->m_dThickSlabBase;
	double dPaveT			= pDB->m_dThickPave;
	double dThickShoe		= 200;
	double d100				= 100;
	double d500				= 500;
	double dHeightTaper		= bStt ? pDB->m_dHeightTaperStt : pDB->m_dHeightTaperEnd;
	double dLengthUgan		= bStt ? pDB->m_dLengthUganStt  : pDB->m_dLengthUganEnd;
	double dLengthGirder	= dLengthUgan	+ (bStt ? pDB->m_dLengthSlabGirderStt   : pDB->m_dLengthSlabGirderEnd);
	double dLengthShoe		= dLengthGirder + (bStt ? pDB->m_dLengthGirderShoeStt   : pDB->m_dLengthGirderShoeEnd);
	double dLengthHunch		= dLengthShoe	+ (bStt ? pDB->m_dLengthShoeToTaperStt  : pDB->m_dLengthShoeToTaperEnd);
	double dLengthHunchEnd	= dLengthHunch	+ (bStt ? pDB->m_dLengthTaperToTaperStt : pDB->m_dLengthTaperToTaperEnd);
	double dBindEnd			= dLengthGirder - (bStt ? pDB->m_dLengthTUBindConc_Stt	: pDB->m_dLengthTUBindConc_End);
	double dSlabBottom		= -dPaveT-dSlabT;
	double dHunchBottom		= dSlabBottom-dHeightTaper;
	double dBindTop			= pDB->IsTUGir() ? dHunchBottom+pDB->m_BindConc.m_dDeep+dHeightTaper : dHunchBottom+pDB->m_BindConc.m_dDeep ;
	double dBindH			= pDB->m_BindConc.m_dHeight;
	double dBindDis			= pDB->m_BindConc.m_dDis;
	double dGirderTop		= dBindTop-dBindH+dBindDis;
	double dHeight			= dHeightG + dSlabT + dPaveT + (dBindH-dBindDis-pDB->m_BindConc.m_dDeep);
	double dInOutUpper		= pGir->GetHeightInOutUpper(bStt, FALSE);
	double dEndPointX		= dLengthHunchEnd + d500;
	double dWidthInOut		= bStt ? pDB->m_dWidthInOutStt : pDB->m_dWidthInOutEnd;
	double dRadInOut		= bStt ? pDB->m_dRadiusInOutStt : pDB->m_dRadiusInOutEnd;
	double dManholeH		= bStt ? pDB->m_dHeightInOutStt	: pDB->m_dHeightInOutEnd;
	double dLoInOut			= bStt ? pDB->m_dHeightInOutLowerStt : pDB->m_dHeightInOutLowerEnd;

	CDomyun Dom(pDom);
	if(!bStt)
	{
		dW				*= -1;
		dLengthUgan		*= -1;
		dLengthGirder	*= -1;
		dLengthShoe		*= -1;
		dLengthHunch	*= -1;
		dLengthHunchEnd *= -1;
		dBindEnd	*= -1;
		d100			*= -1;
		d500			*= -1;
		dEndPointX		*= -1;
		dWidthInOut		*= -1;
		dRadInOut		*= -1;
	}

	pOptStd->SetEnvType(&Dom, HCAD_STLC);	
	Dom.MoveTo(-d500,0);
	Dom.LineTo(0,0);
	Dom.LineTo(0,-(dHeight+dThickShoe));
	Dom.LineTo(dLengthHunch,-(dHeight+dThickShoe));
	Dom.LineTo(dLengthShoe-d100,-(dHeight+dThickShoe),dLengthShoe,-(dHeight));
	Dom.LineTo(dLengthShoe+d100,-(dHeight+dThickShoe));

	// 슬래브
	Dom.LineTo(dLengthUgan, 0, dEndPointX,0);
	Dom.LineTo(dLengthUgan, 0, dLengthUgan, dHunchBottom);
	Dom.LineTo(dLengthHunch, dHunchBottom);
	Dom.LineTo(dLengthHunchEnd, dSlabBottom);
 	Dom.LineTo(dEndPointX, dSlabBottom);

	Dom.RectSolid(dLengthUgan, 0, dLengthUgan+dW, -dH, 10, 0);
	Dom.LineTo(dLengthUgan+dW, -dPaveT, dEndPointX, -dPaveT);

	// 구속콘크리트
	if(pDB->IsTUGir())
	{
		Dom.LineTo(dEndPointX, dBindTop, dBindEnd, dBindTop);
		Dom.LineTo(dBindEnd, dBindTop-dBindH);
		Dom.LineTo(dEndPointX, dBindTop-dBindH);
		
		Dom.LineTo(dLengthGirder, dGirderTop, dEndPointX, dGirderTop);
	}

	// 거더
	Dom.LineTo(dLengthShoe, dGirderTop, dLengthShoe, -dHeight);
	Dom.LineTo(dLengthGirder,dGirderTop,dLengthGirder,dGirderTop-dInOutUpper);

	// 맨홀
	if(pDB->m_cFaceManhole)
	{
		Dom.LineTo(dLengthGirder+dWidthInOut-dRadInOut,dGirderTop-dInOutUpper);
		Dom.Arc(dLengthGirder+dWidthInOut-dRadInOut, dGirderTop-dInOutUpper-fabs(dRadInOut), fabs(dRadInOut),bStt?0:90,90);
		Dom.LineTo(dLengthGirder+dWidthInOut,dGirderTop-dInOutUpper-fabs(dRadInOut), dLengthGirder+dWidthInOut,dGirderTop-dInOutUpper-dManholeH+fabs(dRadInOut));
		Dom.Arc(dLengthGirder+dWidthInOut-dRadInOut,-dHeight+dLoInOut+fabs(dRadInOut),fabs(dRadInOut),bStt?270:180,90);
		Dom.LineTo(dLengthGirder+dWidthInOut-dRadInOut, -dHeight+dLoInOut, dLengthGirder, -dHeight+dLoInOut);
	}
	Dom.LineTo(dLengthGirder, -dHeight);
	Dom.LineTo(dEndPointX, -dHeight);

	// 보강판
	if(pDB->m_cFaceManhole && pDB->m_cFacePlateUse && bStt)
		DrawFacePlateStt(&Dom);
	else if(pDB->m_cFaceManhole && pDB->m_cFacePlateUse)
		DrawFacePlateEnd(&Dom);

	*pDom << Dom;
}

void CAPlateDrawJong::DimJongSttEnd(CDomyun *pDom, BOOL bStt)
{
	CPlateBridgeApp *pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir		= pDB->GetGirder(-1);
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CGlobarOption   *pGlopt		= m_pDataMng->GetGlobalOption();
	
	double dW				= bStt ? pGlopt->GetSttExpansionJointWidth()	: pGlopt->GetEndExpansionJointWidth();
	double dHeightG			= bStt ? pGir->GetHeightGirderByJijum(0, FALSE) : pGir->GetHeightGirderByJijum(pDB->m_nQtyJigan, FALSE);
	double dSlabT			= pDB->m_dThickSlabBase;
	double dPaveT			= pDB->m_dThickPave;
	double dHeightTaper		= bStt ? pDB->m_dHeightTaperStt : pDB->m_dHeightTaperEnd;
	double dSlabBottom		= -dPaveT-dSlabT;
	double dHunchBottom		= dSlabBottom-dHeightTaper;
	double dLengthShoeToTaper = bStt? pDB->m_dLengthShoeToTaperStt : pDB->m_dLengthShoeToTaperEnd;
	double dLengthTaperToTaper = bStt? pDB->m_dLengthTaperToTaperStt : pDB->m_dLengthTaperToTaperEnd;
	double dLengthUgan		= bStt ? pDB->m_dLengthUganStt  : pDB->m_dLengthUganEnd;
	double dLengthGirder	= dLengthUgan	+ (bStt ? pDB->m_dLengthSlabGirderStt   : pDB->m_dLengthSlabGirderEnd);
	double dLengthShoe		= dLengthGirder + (bStt ? pDB->m_dLengthGirderShoeStt   : pDB->m_dLengthGirderShoeEnd);
	double dLengthHunch		= dLengthShoe	+ dLengthShoeToTaper;
	double dLengthHunchEnd	= dLengthHunch	+ (bStt ? pDB->m_dLengthTaperToTaperStt : pDB->m_dLengthTaperToTaperEnd);	
	double dExpJointHeight	= bStt ? pGlopt->GetSttExpansionJointHeight() : pGlopt->GetEndExpansionJointHeight();
	double dExpJointWidth	= bStt ? pGlopt->GetSttExpansionJointWidth()  : pGlopt->GetEndExpansionJointWidth();
	double dBindEnd			= dLengthGirder - (bStt ? pDB->m_dLengthTUBindConc_Stt	: pDB->m_dLengthTUBindConc_End);
	double dBindTop			= -dSlabT-dPaveT+pDB->m_BindConc.m_dDeep;
	double dBindH			= pDB->m_BindConc.m_dHeight;
	double dBindDis			= pDB->m_BindConc.m_dDis;
	double dGirBottom		= dBindTop-dBindH+dBindDis-dHeightG;

	CString szText, str;
	szText.Format("(슬래브 중심에서의 거리 = %.0f)", dLengthShoeToTaper);

	if(!bStt)
	{
		dW				*= -1;
		dLengthUgan		*= -1;
		dLengthGirder	*= -1;
		dLengthShoe		*= -1;
		dLengthHunchEnd *= -1;
		dBindEnd	*= -1;
		dLengthShoeToTaper*= -1;
		dLengthTaperToTaper *= -1;
		dExpJointWidth *= -1;
	}

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(30);

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(0,0,0);
	Dom.DimLineTo(dLengthUgan,0,COMMA(fabs(dLengthUgan)));
	Dom.DimLineTo(dLengthShoe - dLengthUgan,0,COMMA(fabs(dLengthShoe - dLengthUgan)));
	Dom.DimLineToExtend(dLengthShoeToTaper,0,"A",szText);
	Dom.DimLineTo(dLengthTaperToTaper,0,"B");
	Dom.DimMoveTo(0,0,1);
	Dom.DimLineTo(dLengthShoe+dLengthShoeToTaper,1,COMMA(fabs(dLengthShoe+dLengthShoeToTaper)));

	Dom.SetDirection("BOTTOM");
	if(pDB->m_cFaceManhole && bStt)
	{
		Dom.DimMoveTo(dLengthGirder,-(dPaveT+dSlabT+dHeightTaper+pGir->GetHeightInOutUpper(bStt)+400),0);
		Dom.DimLineTo(pDB->m_dWidthInOutStt,0,"G");
		Dom.GiSiArrowExt(dLengthGirder+pDB->m_dWidthInOutStt-pDB->m_dRadiusInOutStt*0.5,
			dGirBottom+pDB->m_dHeightInOutLowerStt+pDB->m_dHeightInOutStt-pDB->m_dRadiusInOutStt*0.5,pDB->m_dRadiusInOutStt,-150,5,"R");
	}
	if(pDB->m_cFaceManhole && !bStt)
	{
		Dom.DimMoveTo(dLengthGirder,-(dPaveT+dSlabT+dHeightTaper+pGir->GetHeightInOutUpper(bStt)+400),0);
		Dom.DimLineTo(-pDB->m_dWidthInOutEnd,0,"G");
		Dom.GiSiArrowExt(dLengthGirder-pDB->m_dWidthInOutEnd+pDB->m_dRadiusInOutEnd*.5,
			dGirBottom+pDB->m_dHeightInOutLowerEnd+pDB->m_dHeightInOutEnd-pDB->m_dRadiusInOutEnd*0.5,pDB->m_dRadiusInOutEnd,150,7,"R");
	}

	Dom.SetDirection(bStt ? "LEFT" : "RIGHT");

	if(pDB->IsTUGir())
	{
		double dE = -1*(bStt?pDB->m_dHeightInOutStt:pDB->m_dHeightInOutEnd);
		double dD = -1*(bStt?pDB->m_dHeightInOutLowerStt:pDB->m_dHeightInOutLowerEnd);
		//Col 0
		Dom.DimMoveTo(dLengthGirder, dHunchBottom+dHeightTaper, 0);
		if(dHeightTaper > 0)
			Dom.DimLineTo(-dHeightTaper,0,"C");	
		
		Dom.DimLineTo(dGirBottom-dHunchBottom,0,COMMA(fabs(dGirBottom-dHunchBottom)));
		//Col 1
		szText.Format("%g", dHeightG+dD+dE);
		Dom.DimMoveTo(dLengthGirder, dBindTop-dBindH+dBindDis, 1);
		Dom.DimLineTo(-dHeightG-dD-dE,1, szText);
		Dom.DimLineTo(dE,1,"E");
		Dom.DimLineTo(dD,1,"D");		
	}
	else
	{
		Dom.DimMoveTo(dLengthGirder, dBindTop-dBindH+dBindDis, 0);

		if(dHeightTaper > 0)  
			Dom.DimLineTo(-dHeightTaper,0,"C");
		
		szText.Format("%g", pGir->GetHeightInOutUpper(bStt,FALSE));
		Dom.DimLineTo(-pGir->GetHeightInOutUpper(bStt,FALSE),0, szText);
		Dom.DimLineTo(-1*(bStt?pDB->m_dHeightInOutStt:pDB->m_dHeightInOutEnd),0,"E");
		Dom.DimLineTo(-1*(bStt?pDB->m_dHeightInOutLowerStt:pDB->m_dHeightInOutLowerEnd),0,"D");
	}
	
	Dom.SetDirection("TOP");
	szText.Format("%.f×%.f", fabs(dExpJointWidth), dExpJointHeight);
	Dom.GiSiArrowExtend(dLengthUgan+dExpJointWidth, -dExpJointHeight,
		Dom.Always(10+(pDB->IsTUGir()?10:0)), (bStt?1:-1)*Dom.Always(20), bStt?7:5, szText, "");

	if(pDB->IsTUGir())
	{
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dLengthGirder, dBindTop, 0);
		Dom.DimLineTo(dBindEnd-dLengthGirder, 0, COMMA(fabs(dBindEnd-dLengthGirder)));
		Dom.DimLineToOver(dLengthUgan-dBindEnd, 0, COMMA(fabs(dLengthUgan-dBindEnd)), bStt?2:1);

	}
	
	*pDom << Dom;
}

void CAPlateDrawJong::DimJongStud(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	 = pBxStt->GetGirder();
	CPlateSangSeDo	*pSang	 = pGir->m_pSangse;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pGlopt  = m_pDataMng->GetGlobalOption();

	double length  = 0;
	double dStaStt = pBxStt->GetStation();
	double dStaEnd = dStaStt+pGir->GetLine()->GetLengthByDis(dStaStt, pBxEnd->GetStation(), pGir->m_dGirderCenter);
	double dStaCur = dStaStt;
	double dStaNext= dStaCur;
	BOOL   bPreUp	= FALSE;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("TOP");
	Dom.SetScaleDim(50);
	Dom.DimMoveTo(pBxStt->GetStation(),	pGir->GetElevationFlangeCamber(dStaStt, 0)+100, 0);

	CString str = _T("");
	double dSumDis = 0;
	for (long i = 0; i < STUD_JONG_SU; i++)
	{
		double	dDis = pSang->m_StudJong_StudDis[i];
		long	nQty = pSang->m_StudJong_Studsu[i];
		if(dDis==0 && nQty==0) break;
		
		dSumDis += dDis*nQty;
		//////////////////////////////////////////////////////////////////////////		
		dStaNext = dStaStt+pGir->GetLine()->GetLengthByDis(dStaStt, dStaStt+dSumDis, pGir->m_dGirderCenter);
		//////////////////////////////////////////////////////////////////////////
		
		if((dStaStt <= dStaCur && dStaCur <= dStaEnd) || dStaNext>dStaEnd)
		{
			dStaNext = min(dStaNext, dStaEnd);
			length = dStaNext-dStaCur;
			if(pBridge->m_nLengthTypeOnStud==0)
				dDis = length/nQty;			

			str.Format("%s", pGlopt->GetStrRepeat(nQty, dDis));

			if(length < Dom.GetTextWidth(str)*1.1 && i!=0)
			{
				long	nPreN = pGir->m_pSangse->m_StudJong_Studsu[i-1];
				double	dPreLength = pGir->m_pSangse->m_StudJong_StudDis[i-1] * nPreN;
				CString szPreStr = _T("");
				szPreStr.Format("%s", pGlopt->GetStrRepeat(nPreN, pGir->m_pSangse->m_StudJong_StudDis[i-1]));

				if(dPreLength < Dom.GetTextWidth(szPreStr)*1.1 && bPreUp)
				{
					bPreUp = FALSE;
					Dom.DimLineToExtend(length, 0, "", str);
				}
				else
				{
					bPreUp = TRUE;
					Dom.DimLineToExtend(length, 0, str, "");
				}
			}
			else
			{
				bPreUp = TRUE;
				Dom.DimLineToExtend(length, 0, str, "");
			}
		}
		dStaCur = dStaNext;
	}

/*
	for (long i = 0; i < m_dArrStud.GetSize(); i++)
	{
		long n = pGir->m_pSangse->m_StudJong_Studsu[i];
		dLenReal = pGir->GetLengthDimType(dSta, dSta+pGir->m_pSangse->m_StudJong_StudDis[i]);
		length = dLenReal * n;
		if (length < 1)	break;
		dSta += length;

		if((dStaStt <= dSta && dSta <= dStaEnd) || dSta>dStaEnd)
		{
			CPlateBasicIndex *pBx = pGir->GetBxByStation(dSta);
			if(!pBx)	break;

			if(dSta>dStaEnd)
				dSta = dStaEnd;
			
			CString str = _T("");

			str.Format("%s", pGlopt->GetStrRepeat(n, m_dArrStud.GetAt(i)));

			if(length < Dom.GetTextWidth(str)*1.1 && i!=0)
			{
				long	nPreN = pGir->m_pSangse->m_StudJong_Studsu[i-1];
				double	dPreLength = pGir->m_pSangse->m_StudJong_StudDis[i-1] * nPreN;
				CString szPreStr = _T("");
				szPreStr.Format("%s", pGlopt->GetStrRepeat(nPreN, pGir->m_pSangse->m_StudJong_StudDis[i-1]));

				if(dPreLength < Dom.GetTextWidth(szPreStr)*1.1 && bPreUp)
				{
					bPreUp = FALSE;
					Dom.DimLineToExtend(length, 0, "", str);
				}
				else
				{
					bPreUp = TRUE;
					Dom.DimLineToExtend(length, 0, str, "");
				}
			}
			else
			{
				bPreUp = TRUE;
				Dom.DimLineToExtend(length, 0, str, "");
			}
		}
	}
	*/
	*pDom << Dom;
}

void CAPlateDrawJong::DrawInputDetSpliceWeb(CDomyun *pDom, long nG, long nSp, BOOL bTitle/*=TRUE*/, BOOL bPaintMap/*=FALSE*/)
{
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp  *pGir	  = pBridge->GetGirder(nG);
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBx     = pGir->GetBxBySpliceNoSPType(nSp);
	
	if(pBx==NULL)		return;

	CPlateSplice	 *pSp     = pBx->GetSplice();

	CDomyun Dom(pDom);
	DrawWebSplice(&Dom, pBx);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawFlangeLineWeb(&Dom,pBx);
	DimWebSplice(&Dom, pBx, bTitle , FALSE, bPaintMap);

	*pDom << Dom;
}

void CAPlateDrawJong::DrawWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, long nShape, BOOL bBoltLine)
{
	CPlateSplice *pSp = pBx->GetSplice();
	if(pSp==NULL)	return;
	
	CDomyun Dom(pDom);

	// 모멘트 이음판(상)
	if(pSp->m_mT>0 && pSp->m_nWebType!=SP_WEB_FLMO)
		DrawWebSpliceMoment(&Dom, pBx, 0, 0, nShape, bBoltLine, TRUE);
	// 전단 이음판
	DrawWebSpliceShear(&Dom,pBx, 0, 0, nShape, bBoltLine);
	// 모멘트 이음판(하)
	if(pSp->m_mT>0 && pSp->m_nWebType!=SP_WEB_FLMO)
		DrawWebSpliceMoment(&Dom, pBx, 0, 0, nShape, bBoltLine, FALSE);

	Dom.Move(BX,BY);
	*pDom << Dom;
}

// 원점 : 이음판 좌측 상단점
void CAPlateDrawJong::DrawWebSpliceShear(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, long nShape, BOOL bBoltLine)
{
	CPlateGirderApp	*pGir = (CPlateGirderApp*) pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateSplice *pSp = pBx->GetSplice();

	if(pSp==NULL)	return;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	CPlatePieceObject pObj(pGir);
	pObj.m_nMarkBuje = SP_W_SH;
	pObj.m_nCol = 0;
	pSp->CreatePlatePieceObj(&pObj, TRUE, bBoltLine, 0, TRUE, nShape);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	Dom.Move(BX, BY);
	*pDom << Dom;

	BOOL bNextFiller = TRUE;
	if(!pBx->IsFiller(BX_WEB, bNextFiller))	return;

	CDPoint xyCU, xyCD;
	CDPointArray xyArr;
	long nSize = pObj.GetObjSize();
	if(nSize==5)	// 통판형, 모멘트 분리형
	{
		CAPlateDrawEtc DrawEtc(m_pDataMng);
		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObj);
	}
	else			// 모멘트 일체형
	{
		pOptStd->SetEnvType(&Dom,HCAD_STLC, LT_HIDDEN_LINE);

		xyCU = (pObj.GetPoint(2)+pObj.GetPoint(3))/2;
		xyCD = (pObj.GetPoint(9)+pObj.GetPoint(8))/2;

		if(bNextFiller)
		{
			xyArr.Add(xyCU);
			xyArr.Add(pObj.GetPoint(3));
			xyArr.Add(pObj.GetPoint(4));
			xyArr.Add(pObj.GetPoint(5));
			xyArr.Add(pObj.GetPoint(6));
			xyArr.Add(pObj.GetPoint(7));
			xyArr.Add(pObj.GetPoint(8));
			xyArr.Add(xyCD);
			xyArr.Add(xyCU);
		}
		else
		{
			xyArr.Add(pObj.GetPoint(2));
			xyArr.Add(xyCU);
			xyArr.Add(xyCD);
			xyArr.Add(pObj.GetPoint(9));
			xyArr.Add(pObj.GetPoint(10));
			xyArr.Add(pObj.GetPoint(11));
			xyArr.Add(pObj.GetPoint(0));
			xyArr.Add(pObj.GetPoint(1));
			xyArr.Add(pObj.GetPoint(2));	
		}
		
		Dom.HatchingByCDPointArray(xyArr, 45, 30, TRUE);
	}	
	
	Dom.Move(BX, BY);
	*pDom << Dom;
}

void CAPlateDrawJong::DrawWebSpliceMoment(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, long nShape, BOOL bBoltLine, BOOL bUpper)
{
	CPlateGirderApp	*pGir = (CPlateGirderApp*) pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateSplice *pSp = pBx->GetSplice();

	if(pSp==NULL)	return;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	CPlatePieceObject pObj(pGir);
	pObj.m_nMarkBuje = SP_W_MO;
	pObj.m_nRow = bUpper ? 0 : 1;
	pSp->CreatePlatePieceObj(&pObj,TRUE, bBoltLine, 0, TRUE, nShape);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	Dom.Move(BX, BY);
	*pDom << Dom;

	CAPlateDrawEtc DrawEtc(m_pDataMng);
	DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObj);
		
	*pDom << Dom;
}

void CAPlateDrawJong::DrawFlangeLineWeb(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bFlange)
{
	CPlateGirderApp	*pGir    = (CPlateGirderApp*) pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pOpt	 = m_pDataMng->GetGlobalOption();
	CPlateSplice	*pSp     = pBx->GetSplice();	
	if(pSp==NULL)	return;

	// 거더의 경사
	double staCen   = pBx->GetStation();
	double JongSlop = pGir->GetGiulWebSplice(pBx,TRUE);
	double BottSlop = pGir->GetGiulWebSplice(pBx,FALSE);
	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;
	double dUpperT	= max(upperTp, upperTn);
	double dLowerT	= max(lowerTp, lowerTn);
	double dLengthWeb  = pGir->GetHeightGirderByStaAng(staCen, 0, pBx->GetAngle(), TRUE);	// Pre 기준 웹길이
	if(!pOpt->GetDesignHeigtBaseUpper())
		dLengthWeb -= max(0, upperTn-upperTp);
	if(pOpt->GetDesignHeigtBaseLower())
		dLengthWeb -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이

	CDomyun Dom(pDom);
	CDomyun DomTemp(&Dom);
	double dLU = (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2 + pSp->m_uC1;
	double dLD = (pSp->m_dA+pSp->m_dB*pSp->m_dC)*2 + pSp->m_dC1;
	double dLW = (pSp->m_sA+pSp->m_sB*pSp->m_sC)*2 + pSp->m_sC1;
	double dLM = (pSp->m_mA+pSp->m_mB*pSp->m_mC)*2 + pSp->m_mC1;
	double dL = max(max(dLU, dLD),max(dLW,dLM));
	double Length = dL+pSp->m_dC1*2;/*dL*1.25*/
	double sX = -Length/2;
	double sY = sX * JongSlop + (!pOpt->GetDesignHeigtBaseUpper()? dUpperT : 0);
	// 상판

	if(bFlange)
	{
		long nDir = pOpt->GetDesignHeigtBaseUpper()?-1:1;
		DomTemp.LineTo(sX,sY, sX+Length, sY+Length*JongSlop);
		DomTemp.LineTo(sX,sY-upperTp*nDir, sX+Length/2, sY+Length/2*JongSlop-upperTp*nDir);
		DomTemp.LineTo(sX+Length/2, sY+Length/2*JongSlop-upperTn*nDir, sX+Length, sY+Length*JongSlop-upperTn*nDir);
	}
	
	// 하판
	sY = -dLengthWeb -Length/2*BottSlop - (pOpt->GetDesignHeigtBaseLower() ? dLowerT : 0);
	if(bFlange)
	{
		long nDir = pOpt->GetDesignHeigtBaseLower()?1:-1;
		DomTemp.LineTo(sX,sY, sX+Length, sY+Length*BottSlop);
		DomTemp.LineTo(sX,sY+lowerTp*nDir, sX+Length/2, sY+Length/2*BottSlop+lowerTp*nDir);
		DomTemp.LineTo(sX+Length/2, sY+Length/2*BottSlop+lowerTn*nDir, sX+Length, sY+Length*BottSlop+lowerTn*nDir);
	}

	DomTemp.Move(BX,BY);
	Dom << DomTemp;

	//상하 이음판
	double UpperSpliceT = pSp->m_uT;
	double UpperSpliceW = pSp->m_uA + pSp->m_uB * pSp->m_uC + pSp->m_uC1/2;	//이음판 너비의 절반 
	double LowerSpliceT = pSp->m_dT;
	double LowerSpliceW = pSp->m_dA + pSp->m_dB * pSp->m_dC + pSp->m_dC1/2;	//이음판 너비의 절반 

	BOOL bFillerNext = TRUE;
	
	if (UpperSpliceT !=0)
	{
		//상
		double Ang		= atan(JongSlop);
		double term		= -upperTp/cos(Ang)*JongSlop;
		double dFillerT	= fabs(upperTn-upperTp);
		double dBaseY	= pOpt->GetDesignHeigtBaseUpper() ? min(upperTp, upperTn) : 0;
		DomTemp.Rectangle(-UpperSpliceW,dUpperT,UpperSpliceW, dUpperT+UpperSpliceT);	//상상
		DomTemp.Rectangle(-UpperSpliceW,0, UpperSpliceW, -UpperSpliceT);				//상하
		if(pBx->IsFiller(BX_UPPERFLANGE, bFillerNext))
		{			
			if(bFillerNext)
			{
				DomTemp.HatchRect(0, dBaseY+dFillerT,  UpperSpliceW, dBaseY+dFillerT,  UpperSpliceW, dBaseY, 0, dBaseY, "ANSI31");
				DomTemp.Rectangle(0, dBaseY+dFillerT, UpperSpliceW, dBaseY);
			}
			else
			{
				DomTemp.HatchRect(0, dBaseY+dFillerT, -UpperSpliceW, dBaseY+dFillerT, -UpperSpliceW, dBaseY, 0, dBaseY, "ANSI31");
				DomTemp.Rectangle(-UpperSpliceW, dBaseY+dFillerT, 0, dBaseY);
			}
		}
		DomTemp.Move(BX+term,BY+term*JongSlop);
		DomTemp.Rotate(Ang, BX+term, BY+term*JongSlop+upperTn);
		Dom << DomTemp;
	}

	if (LowerSpliceT !=0)
	{
		double Ang		= atan(BottSlop);
		double dFillerT	= fabs(lowerTn-lowerTp);
		double termN	= (dLowerT/2-dFillerT)/cos(Ang)*BottSlop;
		double termP	= (dLowerT/2)/cos(Ang)*BottSlop;
		double dBaseY	= pOpt->GetDesignHeigtBaseLower() ? -dLengthWeb : -dLengthWeb-min(lowerTp,lowerTn);

		DomTemp.Rectangle(-LowerSpliceW, -dLengthWeb-dLowerT, LowerSpliceW, -dLengthWeb-dLowerT-LowerSpliceT);	// 하하
		DomTemp.Rectangle(-LowerSpliceW, -dLengthWeb,LowerSpliceW, -dLengthWeb+LowerSpliceT);					// 하상

		if(pBx->IsFiller(BX_LOWERFLANGE, bFillerNext))
		{
			if(bFillerNext)
			{
				double term = Ang < 0 ? termP : termN;
				DomTemp.HatchRect(-term, dBaseY,  LowerSpliceW, dBaseY,  LowerSpliceW, dBaseY-dFillerT, -term, dBaseY-dFillerT, "ANSI31");
				DomTemp.Rectangle(-term, dBaseY, LowerSpliceW, dBaseY-dFillerT);
			}
			else
			{
				double term = Ang > 0 ? termP : termN;
				DomTemp.HatchRect(-term, dBaseY, -LowerSpliceW, dBaseY, -LowerSpliceW, dBaseY-dFillerT, -term, dBaseY-dFillerT, "ANSI31");
				DomTemp.Rectangle(-LowerSpliceW, dBaseY, -term, dBaseY-dFillerT);
			}
		}

		DomTemp.Move(BX, BY);
		DomTemp.Rotate(Ang, BX, BY -dLengthWeb - dLowerT/2);
		Dom << DomTemp;
	}
	
	// 거더이음면 중심선 (점선)
	DomTemp.LineTo(0, dUpperT+UpperSpliceT, 0,-dLengthWeb-dLowerT-LowerSpliceT);
	
	DomTemp.Move(BX,BY);
	Dom << DomTemp;

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	double dWidth = max(dLM, dLW);
	if(dL > dWidth + pSp->m_sF*4)
	{
		CDPoint xyCen = rectDom.CenterPoint();
		
		rectDom.left	= xyCen.x - dWidth/2 - pSp->m_sF*2;
		rectDom.right	= xyCen.x + dWidth/2 + pSp->m_sF*2;
		rectDom.bottom	+= 1;
		rectDom.top		-= 1;

		Dom.TrimByRect(rectDom, FALSE);
	}

	*pDom << Dom;	
}

void CAPlateDrawJong::DimWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bTitle, BOOL bExcel, BOOL bPaintMap)
{
	double DimScale = 20;	// Scale
	CPlateGirderApp	*pGir    = (CPlateGirderApp*) pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pOpt	 = m_pDataMng->GetGlobalOption();
	CPlateSplice    *pSp     = pBx ? pBx->GetSplice() : NULL;

	if(pSp==NULL)	return;

	double JongSlop = pGir->GetGiulWebSplice(pBx,TRUE);
	double BottSlop = pGir->GetGiulWebSplice(pBx,FALSE);
	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;
	double dLowerT	= max(lowerTp, lowerTn);
	double dFillerU	= fabs(upperTp-upperTn);
	double dFillerL	= fabs(lowerTp-lowerTn);
	double dLengthWeb  = pGir->GetHeightGirderByStaAng(pBx->GetStation(), 0, pBx->GetAngle(), TRUE);	// Pre 기준 웹길이
	if(!pOpt->GetDesignHeigtBaseUpper())
		dLengthWeb -= max(0, upperTn-upperTp);
	if(pOpt->GetDesignHeigtBaseLower())
		dLengthWeb -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이
	BOOL bNextFillerU	= TRUE;
	BOOL bNextFillerL	= TRUE;
	BOOL bFillerU		= pBx->IsFiller(BX_UPPERFLANGE, bNextFillerU);
	BOOL bFillerL		= pBx->IsFiller(BX_LOWERFLANGE, bNextFillerL);

	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetScaleDim(DimScale);
	if(bExcel)	Dom.SetTextHeight(Dom.Always(0.2));

	CString szText	= _T("");
	double dDis		= 0;
	double dWidthM	= (pSp->m_mA+pSp->m_mB*pSp->m_mC)*2+pSp->m_mC1;
	double dWidthS	= (pSp->m_sA+pSp->m_sB*pSp->m_sC)*2+pSp->m_sC1;
	double dWidthU	= (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1;
	double dWidthD	= (pSp->m_dA+pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1;
	double dMaxW	= max(max(dWidthM,dWidthS),max(dWidthU,dWidthD)) + 50;	// 50:여유분
	if(dMaxW > max(dWidthM,dWidthS) + pSp->m_sF*4)
		dMaxW = max(dWidthM,dWidthS)/2 + pSp->m_sF*2 + Dom.Always(5);

	long nCol = 0;
	if(bPaintMap)
	{
		szText = "W";
		Dom.SetDirection("TOP");
		if(pSp->m_nWebType!=SP_WEB_FL)
		{
			szText = "W1";
			Dom.DimMoveTo(-dWidthM/2, 0, nCol);
			Dom.DimLineTo(dWidthM, nCol, szText);

			Dom.DimMoveTo(-dWidthM/2, 0, nCol+1);
			Dom.DimLineTo(dWidthM, nCol+1, "W");

			szText = "W2";			
			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(-dWidthS/2, -dLengthWeb, nCol);
			Dom.DimLineTo(dWidthS, nCol, szText);
		}
		else
		{
			Dom.DimMoveTo(-dWidthS/2, 0, nCol);
			Dom.DimLineTo(dWidthS, nCol, szText);
		}
	}
	else
	{
		if(pSp->m_nWebType!=SP_WEB_FL)
		{
			Dom.SetDirection("TOP");
			dDis = pSp->m_mB*pSp->m_mC;
			Dom.DimMoveTo(-dWidthM/2, 0,	nCol,		bExcel ? COMMA(pSp->m_mA)	: "mA");
			Dom.DimLineTo(pSp->m_mA,		nCol, "");
			Dom.DimLineTo(dDis,				nCol,		bExcel ? COMMA(dDis)		: "mB@mC");
			Dom.DimLineToExtend(pSp->m_mC1,	nCol, "",	bExcel ? COMMA(pSp->m_mC1)	: "mC1");
		}

		Dom.SetDirection("BOTTOM");
		dDis = pSp->m_sB*pSp->m_sC;
		Dom.DimMoveTo(-dWidthS/2, -dLengthWeb,	nCol,		bExcel ? COMMA(pSp->m_sA)	: "sA");
		Dom.DimLineTo(pSp->m_sA,				nCol, "");
		Dom.DimLineTo(dDis,						nCol,		bExcel ? COMMA(dDis)		: "sB@sC");
		Dom.DimLineToExtend(pSp->m_mC1,			nCol, "",	bExcel ? COMMA(pSp->m_sC1)	: "sC1");
	}
	
	//////////////////////////////////////////////////////////////////////////
	

	Dom.SetDirection("LEFT");
	if(bPaintMap || bExcel)
	{
		// bPaintMap 에는 Left가 없음
		// bExcel 에는 마킹치수가 없음
	}
	else if(pSp->m_nWebType!=SP_WEB_FL)
	{
		dDis = (dLengthWeb-pSp->m_mW*2-pSp->m_mG*2-pSp->m_sW)/2;
		Dom.DimMoveTo(-dMaxW/2, -dLengthWeb, nCol, "mG");
		Dom.DimLineTo(pSp->m_mG, nCol, "");
		Dom.DimLineTo(pSp->m_mW, nCol, "mW");
		if(pSp->m_nWebType==SP_WEB_MO)
			Dom.DimLineToExtend(dDis, nCol, "", COMMA(dDis));
		Dom.DimLineTo(pSp->m_sW, nCol, "sW");
		if(pSp->m_nWebType==SP_WEB_MO)
			Dom.DimLineToExtend(dDis, nCol, "", COMMA(dDis));
		Dom.DimLineTo(pSp->m_mW, nCol, "mW");
		Dom.DimLineToOver(pSp->m_mG, nCol, "mG");
		nCol++;
	}
	else
	{
		dDis = (dLengthWeb-pSp->m_sW)/2;
		Dom.DimMoveTo(-dMaxW/2, -dLengthWeb, nCol);
		Dom.DimLineTo(dDis, nCol, COMMA(dDis));
		Dom.DimLineTo(pSp->m_sW, nCol, "sW");
		Dom.DimLineTo(dDis, nCol, COMMA(dDis));
		nCol++;
	}	
	
	if(!bPaintMap)
	{
		if(bFillerL && !bNextFillerL && !pOpt->GetDesignHeigtBaseLower())	dDis = dFillerL;
		else																dDis = lowerTp;
		Dom.DimMoveTo(-dMaxW/2, -dLengthWeb-dLowerT, nCol, COMMA(dDis));
		Dom.DimLineTo(dDis, nCol, "");
		if(bFillerL && !bNextFillerL && !pOpt->GetDesignHeigtBaseLower())	dDis = lowerTp;
		else																dDis = dFillerL;
		if(bFillerL && !bNextFillerL)
			Dom.DimLineToExtend(dDis, nCol, "", COMMA(dDis));
		if(pSp->m_nWebType!=SP_WEB_FL)
		{
			dDis = (dLengthWeb-pSp->m_mW*2-pSp->m_mG*2-pSp->m_sW)/2;
			Dom.DimLineTo(pSp->m_mG,		nCol, COMMA(pSp->m_mG));
			Dom.DimLineTo(pSp->m_mW,		nCol, COMMA(pSp->m_mW));
			if(pSp->m_nWebType==SP_WEB_MO)
				Dom.DimLineToExtend(dDis,	nCol, "", COMMA(dDis));
			Dom.DimLineTo(pSp->m_sW,		nCol, COMMA(pSp->m_sW));
			if(pSp->m_nWebType==SP_WEB_MO)
				Dom.DimLineToExtend(dDis,	nCol, "", COMMA(dDis));
			Dom.DimLineTo(pSp->m_mW,		nCol, COMMA(pSp->m_mW));
			Dom.DimLineTo(pSp->m_mG,		nCol, COMMA(pSp->m_mG));
		}
		else
		{
			dDis = (dLengthWeb-pSp->m_sW)/2;
			Dom.DimMoveTo(-dMaxW/2, -dLengthWeb, nCol);
			Dom.DimLineTo(dDis,			nCol, COMMA(dDis));
			Dom.DimLineTo(pSp->m_sW,	nCol, COMMA(pSp->m_sW));
			Dom.DimLineTo(dDis,			nCol, COMMA(dDis));
		}
		if(bFillerU && !bNextFillerU && !pOpt->GetDesignHeigtBaseUpper())	dDis = dFillerU;
		else																dDis = upperTp;
		if(bFillerU && !bNextFillerU)	Dom.DimLineToExtend(dDis,	nCol, "", COMMA(dDis));
		else							Dom.DimLineToOver(dDis,		nCol, COMMA(dDis));
		if(bFillerU && !bNextFillerU && !pOpt->GetDesignHeigtBaseUpper())	dDis = upperTp;
		else																dDis = dFillerU;
		if(bFillerU && !bNextFillerU)	Dom.DimLineToOver(dDis,		nCol, COMMA(dDis));
	}

	//////////////////////////////////////////////////////////////////////////	

	nCol = 0;
	Dom.SetDirection("RIGHT");
	if(bExcel)
	{
		// bExcel 에는 마킹치수가 없음
	}
	else if(bPaintMap)
	{
		if(pSp->m_nWebType==SP_WEB_FL)
		{
			dDis = (dLengthWeb-pSp->m_sW)/2;
			Dom.DimMoveTo(dMaxW/2, -dLengthWeb+dDis, nCol);
			Dom.DimLineTo(pSp->m_sW, nCol, "L");
		}
		else
		{
			dDis = (dLengthWeb-pSp->m_mW*2-pSp->m_mG*2-pSp->m_sW)/2;
			Dom.DimMoveTo(dMaxW/2, -pSp->m_mG, nCol);
			Dom.DimLineTo(-pSp->m_mW, nCol, "L1");
			if(pSp->m_nWebType==SP_WEB_MO)
				Dom.DimMoveTo(dMaxW/2, -pSp->m_mG-pSp->m_mW-dDis, nCol);
			Dom.DimLineTo(-pSp->m_sW, nCol, "L2");
			
			Dom.DimMoveTo(dMaxW/2, -pSp->m_mG, nCol+1);
			dDis = dDis*2+pSp->m_mW*2+pSp->m_sW;
			Dom.DimLineTo(-dDis, nCol+1, "L");
		}
	}
	else if(pSp->m_nWebType!=SP_WEB_FL)
	{
		dDis = (dLengthWeb-pSp->m_mE*pSp->m_mF*2-pSp->m_mD*2-pSp->m_sW)/2;
		Dom.DimMoveTo(dMaxW/2, -dLengthWeb, nCol, "mD");
		Dom.DimLineTo(pSp->m_mD, nCol, "");
		Dom.DimLineTo(pSp->m_mE*pSp->m_mF, nCol, "mE@mF");
		if(pSp->m_nWebType==SP_WEB_MO)
		{
			Dom.DimLineToExtend(dDis, nCol, "", COMMA(dDis));
			Dom.DimLineTo(pSp->m_sW, nCol, "sW");
			Dom.DimLineToExtend(dDis, nCol, "", COMMA(dDis));
		}
		else
		{
			dDis = pSp->m_sE*pSp->m_sF;
			Dom.DimLineToExtend(pSp->m_sF,	nCol, "", COMMA(pSp->m_sF));
			Dom.DimLineTo(dDis,				nCol, "sE@sF");
			Dom.DimLineToExtend(pSp->m_sF,	nCol, "", COMMA(pSp->m_sF));
		}
		Dom.DimLineTo(pSp->m_mE*pSp->m_mF, nCol, "mE@mF");
		Dom.DimLineToOver(pSp->m_mD, nCol, "mD");
		nCol++;
	}
	else
	{
		dDis = (dLengthWeb-pSp->m_sE*pSp->m_sF-pSp->m_sE1*pSp->m_sF1*2)/2;
		Dom.DimMoveTo(dMaxW/2, -dLengthWeb, nCol, COMMA(dDis));
		Dom.DimLineTo(dDis, nCol, "");
		if(pSp->m_sE1*pSp->m_sF1 > 0)
			Dom.DimLineTo(pSp->m_sE1*pSp->m_sF1, nCol, "sE1@sF1");
		Dom.DimLineTo(pSp->m_sE*pSp->m_sF, nCol, "sE@sF");
		if(pSp->m_sE1*pSp->m_sF1 > 0)
			Dom.DimLineTo(pSp->m_sE1*pSp->m_sF1, nCol, "sE1@sF1");
		Dom.DimLineToOver(dDis, nCol, COMMA(dDis));
		nCol++;
	}
	
	if(!bPaintMap)
	{
		if(bFillerL && bNextFillerL && !pOpt->GetDesignHeigtBaseLower())	dDis = dFillerL;
		else																dDis = lowerTn;
		Dom.DimMoveTo(dMaxW/2, -dLengthWeb-dLowerT, nCol, COMMA(dDis));
		Dom.DimLineTo(dDis, nCol, "");
		if(bFillerL && bNextFillerL && !pOpt->GetDesignHeigtBaseLower())	dDis = lowerTn;
		else																dDis = dFillerL;
		if(bFillerL && bNextFillerL)
			Dom.DimLineToExtend(dDis, nCol, "", COMMA(dDis));
		if(pSp->m_nWebType!=SP_WEB_FL)
		{
			dDis = (dLengthWeb-pSp->m_mE*pSp->m_mF*2-pSp->m_mD*2-pSp->m_sW+pSp->m_sA*2)/2;
			Dom.DimLineTo(pSp->m_mD,			nCol, COMMA(pSp->m_mD));
			Dom.DimLineTo(pSp->m_mE*pSp->m_mF,	nCol, COMMA(pSp->m_mE*pSp->m_mF));
			if(pSp->m_nWebType==SP_WEB_MO)
			{
				Dom.DimLineToExtend(dDis,		nCol, "", COMMA(dDis));
				Dom.DimLineTo(pSp->m_sW-pSp->m_sA*2,nCol, COMMA(pSp->m_sW-pSp->m_sA*2));
				Dom.DimLineToExtend(dDis,		nCol, "", COMMA(dDis));
			}
			else
			{
				dDis = dLengthWeb - pSp->m_mD*2 - pSp->m_mE*pSp->m_mF*2;
				Dom.DimLineToExtend(dDis,		nCol, "", COMMA(dDis));
			}
				
			Dom.DimLineTo(pSp->m_mE*pSp->m_mF,	nCol, COMMA(pSp->m_mE*pSp->m_mF));
			Dom.DimLineTo(pSp->m_mD,			nCol, COMMA(pSp->m_mD));
		}
		else
		{
			double dDis2 = pSp->m_sE*pSp->m_sF + pSp->m_sE1*pSp->m_sF1*2;
			dDis = (dLengthWeb-dDis2)/2;
			Dom.DimMoveTo(dMaxW/2, -dLengthWeb, nCol);
			Dom.DimLineTo(dDis,			nCol, COMMA(dDis));
			Dom.DimLineTo(dDis2,		nCol, COMMA(dDis2));
			Dom.DimLineTo(dDis,			nCol, COMMA(dDis));
		}
		if(bFillerU && bNextFillerU && !pOpt->GetDesignHeigtBaseUpper())	dDis = dFillerU;
		else																dDis = upperTn;
		if(bFillerU && bNextFillerU)	Dom.DimLineToExtend(dDis,	nCol, "", COMMA(dDis));
		else							Dom.DimLineToOver(dDis,		nCol, COMMA(dDis));
		if(bFillerU && bNextFillerU && !pOpt->GetDesignHeigtBaseUpper())	dDis = upperTn;
		else																dDis = dFillerU;
		if(bFillerU && bNextFillerU)	Dom.DimLineToOver(dDis,		nCol, COMMA(dDis));
	}
	*pDom << Dom;
	if(bPaintMap)	return;

	Dom.SetDirection("RIGHT");
	if (pSp->m_uT !=0)
	{
		double Ang = atan(JongSlop);
		Dom.DimMoveTo(min(dMaxW, dWidthU)/3, 0, 0);
		Dom.DimLineTo(-pSp->m_uT, 0, COMMA(pSp->m_uT));		
		Dom.Rotate(Ang, 0, 0);
		*pDom << Dom;
	}
	if (pSp->m_dT !=0)
	{
		double Ang = atan(BottSlop);
		Dom.DimMoveTo(min(dMaxW, dWidthU)/3, -dLengthWeb, 0);
		Dom.DimLineTo(pSp->m_dT, 0, COMMA(pSp->m_dT));
		Dom.Rotate(Ang, 0, -dLengthWeb);
		*pDom << Dom;
	}

	if(bTitle)
	{
		Dom.SetDirection("TOP");
		dDis = Dom.Always(8);
		if(pSp->m_nWebType!=SP_WEB_FL)
			dDis += Dom.Always(Dom.GetDimVLen())*2;
		Dom.TextOut(0, dDis, _T("복부판 현장이음"));
	}
	*pDom << Dom;
}

//입력 : 
//		 pBxStt
//		 pBxEnd
//		 bText		수평보강재 제원, 마크 표현
//기능 : pBxStt pBxEnd 수평보강재 도면 출력
void CAPlateDrawJong::DrawHStiffBx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine, long nJewonType, BOOL bBoth)
{	
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	
	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_PLATEHSTIFF;
		
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, FALSE);
	double StdSta = pBxStt->GetStation();
	BOOL bWidthSmall[WEBHSTIFFSUMAX];	
	for(long i=0; i<WEBHSTIFFSUMAX; i++) bWidthSmall[i] = FALSE;

	if(!pBx->GetWebHStiff()) return;

	while(pBx)
	{	
		CWebHStiff *pHs	=	pBx->GetWebHStiff();
		// 수직보강재 설치위치에 따라 나눈 이유는 수평보강재 때문이다.
		
		if(pHs->m_nPos == 0 || pHs->m_nPos == 2)
			DrawHStiffOneBx(pDomP, StdSta, pBx, Finder.GetBxFixNext(), 1, bWidthSmall, nLine, nJewonType, bBoth);
		else
			DrawHStiffOneBx(pDomP, StdSta, pBx, Finder.GetBxFixNext(), 0, bWidthSmall, nLine, nJewonType, bBoth);

		//LSH_ZZ 2009.08.12 수평보강재 위치가 수직보강재 위치를 따라가지 않음.
		/*if(pBx->GetGirder()->m_nInstallVStiff==0 || pBx->GetGirder()->m_nInstallVStiff==2)	//양측
			DrawHStiffOneBx(pDomP, StdSta, pBx, Finder.GetBxFixNext(), 1, bWidthSmall, nLine, nJewonType, bBoth);
		else
			DrawHStiffOneBx(pDomP, StdSta, pBx, Finder.GetBxFixNext(), 0, bWidthSmall, nLine, nJewonType, bBoth);
		*/
		pBx = Finder.GetBxNext();
	}
}

//입력 : 
//		 StdSta	기준 스테이션 (현재 도면의 시작 스테이션)
//		 pHs	해당 수평보강재
//		 bText  마크 표현 여부
//기능 : pHs 수평보강재 그리기(1단에서 6단까지 모두 그림)
void CAPlateDrawJong::DrawHStiffOneBx(CDomyun* pDomP, double StdSta, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, long nCol,
									  BOOL *bWidthSmall, long nLine, long nJewonType, BOOL bBoth)
{  	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir    = pBx->GetGirder();
	CWebHStiff		*pHs     = pBx->GetWebHStiff();

	if(!pHs) return;
	
	double XL	  = 0;
	double XR     = 0;
	double T	  = pHs->m_dThickHStiff[nCol];
	double SttSta = 0;
	double EndSta = 0;
	double SttPos = 0;
	double EndPos = 0;
	double HsLen  = 0;
	CString csMark, csStr;

	if(nLine==0)  T = 0; 

	CDomyun* p = new CDomyun(pDomP);	

	DIMTYPE nDimType=DIM_BASIC;
	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		pOptStd->SetEnvType(p,HCAD_STLC, (pHs->m_nPos == WEB_SIDE_BOTH || pHs->m_nPos == WEB_SIDE_RIGHT) ? LT_SOLID : LT_DASHED_LINE);
		BOOL bOverText = FALSE;
		if(pHs->IsEmpty(nDan, nCol==0?TRUE:FALSE))	continue;
		SttSta = pHs->m_dStationStt[nCol][nDan];
		EndSta = pHs->m_dStationEnd[nCol][nDan];
		SttPos = pGir->GetElevationFlangeCamber(SttSta,0) - pHs->GetHeightHStiffOnStt(nDan, nCol==0?TRUE:FALSE, TRUE);
		EndPos = pGir->GetElevationFlangeCamber(EndSta,0) - pHs->GetHeightHStiffOnStt(nDan, nCol==0?TRUE:FALSE, FALSE);
		HsLen  = pGir->GetLengthDimType(SttSta,EndSta,0);
		XL = StdSta + pGir->GetLengthDimType(StdSta,SttSta,0);
		XR = XL + HsLen;	
		
		if(nLine!= 1)
			p->LineTo(XL,SttPos+T/2, XR,EndPos+T/2); // 윗선
		if(nLine!=-1 && T>0)
			p->LineTo(XL,SttPos-T/2, XR,EndPos-T/2); // 아랫선
		if(nLine== 2 && T>0)
		{
			p->LineTo(XL,SttPos+T/2, XL,SttPos-T/2); // 좌측
			p->LineTo(XR,EndPos+T/2, XR,EndPos-T/2); // 우측
		}		
		
		if(nJewonType>=0) 
		{	
			long nMarkBuje = nCol==0 ? G_H_L : G_H_R;
			double HStiffAng = pHs->GetAngleRadianHStiff(nDan, nCol==0?TRUE:FALSE);	
			pOptStd->SetEnvType(p,HCAD_DIML);	
			p->SetTextAngle(ToDegree(HStiffAng));								
				
			if(nJewonType>4)
			{
				CString szJewon;
				szJewon.Format("T=%.0f", T);
				p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), szJewon);													
			}
			else
			{							
				long nG = pBx->GetNumberGirder();
				//csMark = pBridge->GetMarkBuje(nMarkBuje, pBx, nDan, 0);
				CVector vS= pGir->GetJewonTotal(nMarkBuje, pBx, nDan, 0);			
				csStr = pOptStd->GetJewonString(1,vS.x,vS.y,vS.z,"PL");						
				
				// 반대편용			
				CPlateBasicIndex *pBxSide = nCol==0 ? pBx->GetBxRight() : pBx->GetBxLeft();
				CWebHStiff *pHsSide  = pBxSide->GetWebHStiff();
				CString csMark2 = "", csStr2 = "";
			/*	if(!pHsSide->IsEmpty(nDan, bLeft))
				{
					nMarkBuje = bLeft ? G_H_L : G_H_R;							
					//csMark2 = "(" + pBridge->GetMarkBuje(nMarkBuje, pBxSide, nDan, 0) + ")";						
					vS= pGir->GetJewonTotal(nMarkBuje, pBxSide, nDan, 0);			
					csStr2 = "(" + pOptStd->GetJewonString(1,vS.x,vS.y,vS.z,"PL") + ")";						
				}			
				*/
				switch(nJewonType)
				{
				case 0:
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), csStr);									
					if(bBoth) p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight() + p->Always(1)) / cos(HStiffAng), csStr2);				
					break;
				case 1:
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), csMark);					
					if(bBoth) p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight() + p->Always(1)) / cos(HStiffAng), csMark2);				
					break;
				case 2:
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), csStr + "-" + csMark);					
					if(bBoth) p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight() + p->Always(1)) / cos(HStiffAng), csStr2 + "-" + csMark2);				
					break;
				case 3:
					if(bBoth)
					{
						p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), csStr + csStr2);					
						p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight() + p->Always(1)) / cos(HStiffAng), "-" + csMark + csMark2);				
					}
					else
					{
						p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), csStr);					
						p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight() + p->Always(1)) / cos(HStiffAng), "-" + csMark);				
					}
					break;
				}			
				// 주석문과 이하문은 자동 조정을 위해서 일단 남겨둠
				if(p->GetTextWidth(csMark) > HsLen && !bWidthSmall[nDan]) 
					bWidthSmall[nDan] = TRUE;
				else 
					bWidthSmall[nDan] = FALSE;			
			}
		}		
	}
	*pDomP << *p;
	delete p;
}

void CAPlateDrawJong::DrawHStiffBxJewon(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLRDim)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	
	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_PLATEHSTIFF;
		
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, FALSE);
	double StdSta = pBxStt->GetStation();
	BOOL bWidthSmall[WEBHSTIFFSUMAX];	
	for(long i=0; i<WEBHSTIFFSUMAX; i++) bWidthSmall[i] = FALSE;

	if(!pBx->GetWebHStiff()) return;

	while(pBx)
	{		
		// 수직보강재 설치위치에 따라 나눈 이유는 수평보강재 때문이다.
		CWebHStiff *pHs = pBx->GetWebHStiff();
	
		if(pBx->GetGirder()->m_nInstallVStiff==0 || pBx->GetGirder()->m_nInstallVStiff==2)	//양측
			DrawHStiffOneBxJewon(pDomP, StdSta, pBx, Finder.GetBxFixPrev(), Finder.GetBxFixNext(), 1, bWidthSmall, bLRDim);
		else
			DrawHStiffOneBxJewon(pDomP, StdSta, pBx, Finder.GetBxFixPrev(), Finder.GetBxFixNext(), 0, bWidthSmall, bLRDim);

		pBx = Finder.GetBxNext();
	}
}

// 수직보강재의 설치기준과 수평보강재의 설치기준은 같기 때문에
// 수직보강재 양측, 우측 설치시 우측수평보강재의 제원을 보여주고
// 수직보강재 좌측 설치시 좌측수평보강재의 제원을 보여줌.
void CAPlateDrawJong::DrawHStiffOneBxJewon(CDomyun* pDomP, double StdSta, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxPrev,  CPlateBasicIndex *pBxNext, long nCol, BOOL *bWidthSmall, BOOL bLRDim)									  
{
	CARoadOptionStd *pOpt	 = m_pDataMng->GetOptionStd();	
	CPlateGirderApp	*pGir    = pBx->GetGirder();
	CWebHStiff		*pHs     = pBx->GetWebHStiff();
	if(!pHs) return;

	double XL	  = 0;
	double XR     = 0;
	double SttSta = 0;
	double EndSta = 0;
	double SttPos = 0;
	double EndPos = 0;
	double HsLen  = 0;
	CString csStr = _T("");
	// 수직보강재의 설치기준과 수평보강재의 설치기준은 같기 때문에 수직보강재 양측설치시 수평보강재의 갯수는 2개로 함
	

	long nHStiffsu = pHs->m_nPos == 0 ? 2 : 1;
	//long nHStiffsu = pGir->m_nInstallVStiff==0 ? 2 : 1;
	long nMarkBuje = nCol==0 ?  G_H_L : G_H_R;

	CDomyun* p = new CDomyun(pDomP);	
	DIMTYPE nDimType=DIM_BASIC;	
	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{		
		if(pHs->IsEmpty(nDan, nCol==0?TRUE:FALSE))	continue;

		SttSta = pHs->m_dStationStt[nCol][nDan];
		EndSta = pHs->m_dStationEnd[nCol][nDan];
		SttPos = pGir->GetElevationFlangeCamber(SttSta,0) - pHs->GetHeightHStiffOnStt(nDan,  nCol==0?TRUE:FALSE, TRUE);
		EndPos = pGir->GetElevationFlangeCamber(EndSta,0) - pHs->GetHeightHStiffOnStt(nDan,  nCol==0?TRUE:FALSE, FALSE);
		HsLen  = pGir->GetLengthDimType(SttSta,EndSta,0);
		XL = StdSta + pGir->GetLengthDimType(StdSta,SttSta,0);
		XR = XL + HsLen;

		CVector vJ		= pGir->GetJewonTotal(nMarkBuje, pBx,nDan,0,DIM_BASIC);
		CVector vJPrev	= pBxPrev ? pGir->GetJewonTotal(nMarkBuje,pBxPrev,nDan,0,DIM_BASIC) : CVector(0,0,0);
		csStr = (pBx != pBxPrev &&
			     pOpt->GetJewonString(nHStiffsu,vJ.x,vJ.y,vJ.z,"PL")==pOpt->GetJewonString(nHStiffsu,vJPrev.x,vJPrev.y,vJPrev.z,"PL")) ? 
				 "DO" : 
		         pOpt->GetJewonString(nHStiffsu,vJ.x,vJ.y,vJ.z,"PL");
		if(p->GetTextWidth(csStr) > HsLen) 
		{
			csStr = pOpt->GetJewonString(nHStiffsu,vJ.x,vJ.y,vJ.z,"PL");			
		}
		
		double HStiffAng = pHs->GetAngleRadianHStiff(nDan, nCol==0?TRUE:FALSE);
		pOpt->SetEnvType(p,HCAD_DIML);	
		p->SetTextAngle(ToDegree(HStiffAng));								
		if(csStr=="DO")
			p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), csStr);					
		else
		{				
			CString csStrLeft = _T(""), csStrRight = _T("");
			CVector vJLeft  = pGir->GetJewonTotal(G_H_L,pBx,nDan,0,DIM_BASIC);
			CVector vJRight = pGir->GetJewonTotal(G_H_R,pBx,nDan,0,DIM_BASIC);
			if(HsLen > p->GetTextWidth(csStr))
			{
				p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), "H-STIFF");
				csStrLeft  = pOpt->GetJewonString(nHStiffsu,vJLeft.x,vJLeft.y,vJLeft.z,"PL");			
				csStrRight = pOpt->GetJewonString(nHStiffsu,vJRight.x,vJRight.y,vJRight.z,"PL");			
				if(bLRDim) // 좌우제원을 모두 표기
				{
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight()*1 + p->Always(1)) / cos(HStiffAng), csStrLeft);
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight()+p->Always(1))*2 / cos(HStiffAng), "("+csStrRight+")");
				}
				else
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight() + p->Always(1)) / cos(HStiffAng), csStr);
			}
			else
			{				
				csStr = COMMA(vJ.z);
				csStrLeft  = COMMA(vJLeft.z);
				csStrRight = COMMA(vJRight.z);
				p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 + p->Always(1) / cos(HStiffAng), "PL");				
				if(bLRDim)
				{
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight()+p->Always(1)) / cos(HStiffAng), csStrLeft);
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight()+p->Always(1))*2 / cos(HStiffAng), "("+csStrRight+")");
				}
				else
					p->TextOut(XL + (XR - XL) / 2, SttPos + (EndPos - SttPos) / 2 - (p->GetTextHeight() + p->Always(1)) / cos(HStiffAng), csStr);
			}
		}
		if(p->GetTextWidth(csStr) > HsLen && !bWidthSmall[nDan]) 
			bWidthSmall[nDan] = TRUE;
		else 
			bWidthSmall[nDan] = FALSE;					
	}
	*pDomP << *p;
	delete p;
}

// 수직보강재 양측, 우측 설치시 우측의 제원을 보여주고
// 수직보강재 좌측       설치시 좌측의 제원을 보여줌.
void CAPlateDrawJong::DrawVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, long nCol, long nLine, long nJewonType)
{
	if(pBx->IsJijum())	return;
	CGlobarOption   *pGlopt  = m_pDataMng->GetGlobalOption();
	CARoadOptionStd *pOpt 	 = m_pDataMng->GetOptionStd();	
	CPlateGirderApp	*pGir    = pBx->GetGirder();
	
	double dSta = pBx->GetStation();
	
	// 수직보강재 양측설치시 수평보강재의 갯수는 2개로 함(단, 수직보강재 위치인 경우만--가로보위치는 따로 계산함)
	long nCount = (pGir->m_nInstallVStiff == 0) ? 2 : 1;

	double vT, vW, vHUp, vHLo, vH;

	CSection	*pSec		=	pBx->GetSection();
	CVStiff		*pVStiff	=	NULL; 

	if(pSec->IsenDetType(SECDET_CROSSBEAM))		// 가로보 일경우 복부판의 폭기준으로 그림
	{
		CPlateCrossBeam *pCross = pSec->GetCrossBeam();
		double			dHGir	= pGir->GetHeightGirderByStaAng(dSta);

		pVStiff = pCross->m_pVStiff;
		vT		= pVStiff->m_dThick[nCol];
		vW		= pVStiff->m_dWidth[nCol];	

		if(vT==0)
		{
			vT = pCross->m_T3_CR;
			vW = 1;
		}

		vH		= pVStiff->GetVStiffHeight(nCol == 0 ? TRUE : FALSE, dHGir);
		vHUp	= pVStiff->m_dH[nCol][UP];
		vHLo	= pVStiff->m_dH[nCol][DOWN];

	}
	else if(pSec->IsenDetType(SECDET_VBRACING))	// 가로보 일경우 복부판의 폭기준으로 그림
	{
		CVBracing *pVr = pSec->GetVBracing();
		double dHGir = pGir->GetHeightGirderByStaAng(dSta);
		pVStiff = pVr->m_pVStiff;
		vT = pVStiff->m_dThick[nCol];
		vW = pVStiff->m_dWidth[nCol];	
		vH	 = pVStiff->GetVStiffHeight(nCol==0?TRUE:FALSE, dHGir);
		vHUp = pVStiff->m_dH[nCol][UP];
		vHLo = pVStiff->m_dH[nCol][DOWN];
	}
	else
	{
		if(pSec->GetVStiff())
		{
			double dHGir = pGir->GetHeightGirderByStaAng(dSta);
			pVStiff = pSec->GetVStiff();
			vT = pVStiff->m_dThick[nCol];
			vW = pVStiff->m_dWidth[nCol];	
			vH	 = pVStiff->GetVStiffHeight(nCol==0?TRUE:FALSE, dHGir);
			vHUp = pVStiff->m_dH[nCol==0?LEFT:RIGHT][0];
			vHLo = pVStiff->m_dH[nCol==0?LEFT:RIGHT][1];
		}
	}

	if(!pVStiff) return;

	BOOL bVstiffExist = (vT && vW) ? TRUE : FALSE;
	if(nLine==0)	vT = 0; 

	double dThick    = pGlopt->GetDesignHeigtBaseUpper()==0 ? pBx->GetFactChain(G_F_U)->m_dFactChainThick : 0;
	// 수직부재 그리기
	double XStt = -vT/2;
	double XEnd =  vT/2;
	double YStt = pGir->GetElevationFlangeCamber(dSta-vT/2) - dThick;
	double YCen = pGir->GetElevationFlangeCamber(dSta) - dThick;
	double YEnd = pGir->GetElevationFlangeCamber(dSta+vT/2) - dThick;
	double HStt = pGir->GetHeightGirderByStaAng(dSta-vT/2);
	double HCen = pGir->GetHeightGirderByStaAng(dSta);
	double HEnd = pGir->GetHeightGirderByStaAng(dSta+vT/2);

	CAPlateSymbolDom Dom(pDom,m_pDataMng);			
	if(bVstiffExist)
	{
		if(nLine!=1)
		{
			Dom.LineTo(XStt,YStt-vHUp, XStt,YStt-vHUp-vH);
			if(vHLo>0)
				Dom.LineTo(XStt,YStt-HStt+vHLo, XStt,YStt-HStt+vHLo);
		}
		if(nLine!=1 && vT>0)
		{
			Dom.LineTo(XEnd,YEnd-vHUp, XEnd,YEnd-vHUp-vH);
			if(vHLo>0)
				Dom.LineTo(XEnd,YEnd-HEnd+vHLo, XEnd, YEnd-HEnd+vHLo);
		}
		if(vH>0)	Dom.LineTo(XStt,YStt-vHUp-vH,	XEnd,YEnd-vHUp-vH);
		if(vHLo>0)	Dom.LineTo(XStt,YStt-HStt+vHLo, XEnd,YEnd-HEnd+vHLo);
		if(vHUp>0)	Dom.LineTo(XStt,YStt-vHUp,		XEnd,YEnd-vHUp);
	}

	///////////////////////////////////////////////////// By Shin
	CString szH = "";
	if(pGir->IsByunByStation(dSta))
		szH.Format("(%.0f)",pBx->GetHeightGirderByBx());

	CWebHStiff *pHStiff = pBx->GetWebHStiff();
	double dUpSpace = 0;
	double dDnSpace = 0;
	double dAddSpace = 0;
	BOOL bLeft = nCol==0 ? TRUE : FALSE;
	if(pHStiff) 
	{
		long nUDan = pHStiff->GetUpperDan(bLeft);
		long nLDan = pHStiff->GetLowerDan(bLeft);
		dUpSpace =  nUDan == 0 ? 0 : pHStiff->GetHeightHStiffOnStt(nUDan-1,bLeft,TRUE);						
		dDnSpace =	pHStiff->GetHeightHStiffOnStt(nLDan+2,bLeft, TRUE);
		if(nLDan!=0) dDnSpace = HCen - dDnSpace;
		dAddSpace = dUpSpace - dDnSpace;
	}

	if(bVstiffExist && nJewonType>=0)
	{
		pOpt->SetEnvType(&Dom,HCAD_DIML);		
		double cx1, cx2;
		Dom.SetTextAngle(90);
		cx1 = XStt - Dom.Always(Dom.GetDimTvp());
		cx2 = XStt + vT + Dom.GetTextHeight() + Dom.Always(Dom.GetDimTvp());
		
		CVector vJ, vJ2;
		CString szJewon, szText, szJewon2, szText2;
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{
			nCount = 1;
			if(vH==HCen)	// 수직보강재와 복부높이가 같을 경우(딱 붙을 경우)
			{
				if(pSec->IsenDetType(SECDET_CROSSBEAM))		szText.Format("CROSSBEAM PL");
				else										szText.Format("VBRACING PL");

				vJ = CVector(0,0,0);
			}
			else
			{
				if(vH>0)
				{
					szText.Format("%d-CROSSBEAM PL",nCount);
					vJ = pGir->GetJewonTotal(V_ST,pBx,0,0,DIM_BASIC);	
					if(vJ==CVector(0,0,0))	vJ = pGir->GetJewonTotal(V_ST,pBx,0,1,DIM_BASIC);	

				}
				if(vHLo>0)
				{
					szText2.Format("%d-CROSSBEAM PL",nCount);
					vJ2 = pGir->GetJewonTotal(V_ST,pBx,1,0,DIM_BASIC);	
					if(vJ2==CVector(0,0,0))	vJ2 = pGir->GetJewonTotal(V_ST,pBx,1,0,DIM_BASIC);	
				}
			}
		}
		else
		{
			if(vH==HCen)
			{
				szText.Format("%d-VSTIFF PL",nCount);
				vJ = pGir->GetJewonTotal(V_ST,pBx,0,0,DIM_BASIC);	
				if(vJ==CVector(0,0,0))	vJ = pGir->GetJewonTotal(V_ST,pBx,0,1,DIM_BASIC);	
			}
			else
			{
				if(vH>0)
				{
					szText.Format("%d-VSTIFF PL",nCount);
					vJ = pGir->GetJewonTotal(V_ST,pBx,0,0,DIM_BASIC);	
					if(vJ==CVector(0,0,0))	vJ = pGir->GetJewonTotal(V_ST,pBx,0,1,DIM_BASIC);	
				}
				if(vHLo>0)
				{
					szText2.Format("%d-VSTIFF PL",nCount);
					vJ2 = pGir->GetJewonTotal(V_ST,pBx,1,0,DIM_BASIC);	
					if(vJ2==CVector(0,0,0))	vJ2 = pGir->GetJewonTotal(V_ST,pBx,1,1,DIM_BASIC);	
				}
			}
		}

		if(nJewonType>4)
		{
			if(!vJ.IsNull()) 
				szJewon.Format("%.0f-%.0f-%.0f",vJ.y,vJ.x,vJ.z);
			Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);			
		}
		else
		{
			CString szMark="";				
			long nG = pBx->GetNumberGirder();

			if(!vJ.IsNull()) 
			{
				szJewon = pOpt->GetJewonString(0,vJ.x,vJ.y,vJ.z,"",FALSE);					
				szJewon2 = pOpt->GetJewonString(0,vJ2.x,vJ2.y,vJ2.z,"",FALSE);					
			}		
			
			switch(nJewonType)
			{
			case 0:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 1:
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 2:
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 3:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);
				break;
			case 4:			
				{
					if(vH==HCen)
					{
							Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szText+" "+szJewon);
					}
					else
					{
						if(vH>0)
						{
							Dom.TextOut(cx1,YCen-vHUp-vH/2,szText+" "+szJewon);
						}
						if(vHLo>0)
						{
							Dom.TextOut(cx1,YCen-HCen+vHLo/2, szText2+" "+szJewon2);
						}
					}
				}
			}		
		}
	}	

	double Xp = dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation(), 0);
	Dom.Move(Xp, 0);

	*pDom << Dom;	
}

void CAPlateDrawJong::DrawVStiffBx(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine, long nJewonType, BOOL bApplyHiddenLine)
{		
	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();	

	__int64 BxFlag = BX_VSTIFF | BX_CROSSBEAM_VBRACING;

	CDomyun				Dom(pDom);	
	CPlateBxFinder		Finder(pGir);

	CPlateBasicIndex	*pBx	= (CPlateBasicIndex*)Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	
	while(pBx)
	{	
		// 수직보강재는 항상 실선이다. (이유 : 종단 그리는 기준이 수직보강재 설치방향이기 때문)
		if(pBx->IsState(BX_VSTIFF))
		{
			CVStiff *pVStiff = pBx->GetSection()->GetVStiff();

			if(pBx->GetGirder()->m_nInstallVStiff==0 || pBx->GetGirder()->m_nInstallVStiff==2)	//양측, 우측
			{
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				DrawVStiffOnBx(&Dom, pBx, pBxStt->GetStation(), 1, nLine, nJewonType);		
			}
			else	// 좌측									
			{
				pOptStd->SetEnvType(&Dom, HCAD_STLH);
				DrawVStiffOnBx(&Dom, pBx, pBxStt->GetStation(), 0, nLine, nJewonType);	
			}
		}
		else
		{
			CVStiff *pVStiff = NULL;

			if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			{
				CPlateCrossBeam *pCross = pBx->GetSection()->GetCrossBeam();
				if(pCross==NULL)	break;

				pVStiff = pCross->m_pVStiff;
			}
			else
			{
				CVBracing *pVr = pBx->GetSection()->GetVBracing();
				if(pVr==NULL)	break;

				pVStiff = pVr->m_pVStiff;
			}

			double dWL	=	pVStiff->m_dWidth[0];
			double dWR	=	pVStiff->m_dWidth[1];

			// 지점보강재가 양측에 있을 경우는 우측면을 그리고 
			// 중요사항 : 보강재의방향이 수평보강재의 설치방향과 다른 경우는 LT_DOT_LINE으로 그림.
			if((dWL>0 && dWR>0) || (dWL==0 && dWR>0))	// 우측 보강재 그림
			{
				if(dWL>0 && dWR>0)	// 둘다 있을 경우
					pOptStd->SetEnvType(&Dom, HCAD_STLC);
				else				// 우측에만 있을 경우
				{
					if(pBx->GetGirder()->m_nInstallVStiff==1)	// 좌측
					{
						if(bApplyHiddenLine)	pOptStd->SetEnvType(&Dom, HCAD_STLH);
						else					pOptStd->SetEnvType(&Dom, HCAD_STLC);
					}
					else										// 우측	
						pOptStd->SetEnvType(&Dom, HCAD_STLC);
				}

				DrawVStiffOnBx(&Dom, pBx, pBxStt->GetStation(), 1, nLine, nJewonType);		// 우측그림
			}
			else if(dWL>0 && dWR==0)					// 좌측 보강재 그림
			{
				if(pBx->GetGirder()->m_nInstallVStiff==2)	//우측
				{
					if(bApplyHiddenLine)	pOptStd->SetEnvType(&Dom, HCAD_STLH);
					else					pOptStd->SetEnvType(&Dom, HCAD_STLC);
				}
				else
					pOptStd->SetEnvType(&Dom, HCAD_STLH);
				
				DrawVStiffOnBx(&Dom, pBx, pBxStt->GetStation(), 0, nLine, nJewonType);		// 좌측그림
			}
		}

		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}

	*pDom << Dom;
}

// 수직보강재 양측, 우측 설치시 우측의 제원을 보여주고
// 수직보강재 좌측       설치시 좌측의 제원을 보여줌.
void CAPlateDrawJong::DrawVStiffBxJewon(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nCol, BOOL bJewonLeftRigh)
{
	CARoadOptionStd *pOpt 	 = m_pDataMng->GetOptionStd();	
	CPlateGirderApp	*pGir    = pBxStt->GetGirder();
	
	CPlateBxFinder Finder(pGir);
	
	CDomyun Dom(pDomP);
	pOpt->SetEnvType(&Dom, HCAD_DIML);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JACKUP | BX_JIJUMSTIFF| BX_CROSSBEAM_VBRACING | BX_VSTIFF, pBxStt, pBxEnd);
	BOOL bJackupDraw = FALSE;
	BOOL bVStiffDraw = FALSE;
	BOOL bLeft = nCol==0?TRUE:FALSE;

	while(pBx)
	{
		double YCen = pGir->GetElevationFlangeCamber(pBx->GetStation());
		double HCen = pGir->GetHeightGirderByStaAng(pBx->GetStation());

		CWebHStiff *pHStiff = pBx->GetWebHStiff();		
		double dUpSpace = 0;
		double dDnSpace = 0;
		double dAddSpace = 0;
		if(pHStiff) 
		{
			long nUDan = pHStiff->GetUpperDan(bLeft);
			long nLDan = pHStiff->GetLowerDan(bLeft);
			dUpSpace =  nUDan == 0 ? 0 : pHStiff->GetHeightHStiffOnStt(nUDan-1, bLeft, TRUE);						
			dDnSpace =	pHStiff->GetHeightHStiffOnStt(2+nLDan, bLeft, TRUE);
			if(nLDan!=0) dDnSpace = HCen - dDnSpace;
			dAddSpace = dUpSpace - dDnSpace;						
		}

		if(pBx->IsState(BX_JACKUP))		// 잭업 보강재 제원 nCol에 관계없음.
		{	
			if(!bJackupDraw)
			{
				CPlateBasicIndex *pBxJackup = pBx;
				double dWLeft  = pBxJackup->GetJackupStiff()->GetWidth(TRUE);
				double dWRight = pBxJackup->GetJackupStiff()->GetWidth(FALSE);
				if(dWLeft==0 && dWRight==0)		break;
				// 잭업이 [양측, 우측]에 있으면 [우측 제원]을 [좌측]에만 있으면 [좌측제원]..
				long nMarkBuje = 0;
				long nColJack  = 0;
				long nJacksu   = 0;
				if((dWLeft>0 && dWRight>0) || (dWLeft==0 && dWRight>0))	{	nMarkBuje = GI_JA_R;	nColJack = 1;	}
				else													{	nMarkBuje = GI_JA_L;	nColJack = 0;	}	
				if((dWLeft>0 && dWRight>0))		nJacksu = 2;
				else							nJacksu = 1;

				// 잭업제원은 좌우측을 부재마크로 구별하기 때문에 nCol은 의미없음.
				CVector vJ = pGir->GetJewonTotal(nMarkBuje, pBxJackup, 0, 0, DIM_BASIC);		
				double dLen = pGir->GetLengthDimType(pBxStt->GetStation(), pBx->GetStation());
				double cx1 = pBxStt->GetStation() + dLen - vJ.y - Dom.Always(Dom.GetDimTvp());
				double cx2 = pBxStt->GetStation() + dLen + vJ.y + Dom.GetTextHeight() + Dom.Always(Dom.GetDimTvp());		

				Dom.SetTextAngle(90);
				Dom.TextOut(cx1, YCen-((pBxJackup->GetJackupStiff()->m_dVL==0 ? HCen : HCen + (pBx->GetJackupStiff()->GetHeight(TRUE) - pBxJackup->GetJackupStiff()->m_dVL))+dAddSpace)/2, "잭업보강재");
				Dom.TextOut(cx2, YCen-((pBxJackup->GetJackupStiff()->m_dVL==0 ? HCen : HCen + (pBx->GetJackupStiff()->GetHeight(TRUE) - pBxJackup->GetJackupStiff()->m_dVL))+dAddSpace)/2, pOpt->GetJewonString(nJacksu,vJ.x,vJ.y,vJ.z));
				bJackupDraw = TRUE;
			}
		}
		else if(pBx->IsState(BX_JIJUMSTIFF))// 지점 보강재 제원 
		{
			if(!bVStiffDraw)
			{
				CPlateBasicIndex	*pBxJijumV	=	pBx;
				double				dWLeft		=	pBxJijumV->GetJijumStiff()->m_dWidth[0];
				double				dWRight		=	pBxJijumV->GetJijumStiff()->m_dWidth[1];

				if(dWLeft==0 && dWRight==0)		break;
		
				long nMarkBuje = V_J_ST;
				long nColJijum = 0;
				long nJVStiff  = 0;

				if((dWLeft>0 && dWRight>0) || (dWLeft==0 && dWRight>0))		nColJijum = 1;
				else														nColJijum = 0;
				
				if((dWLeft>0 && dWRight>0))		nJVStiff = 2;
				else							nJVStiff = 1;
				
				CVector	vJ		= pGir->GetJewonTotal(nMarkBuje, pBxJijumV, 0, nColJijum, DIM_BASIC);
				double	dLen	= pGir->GetLengthDimType(pBxStt->GetStation(), pBx->GetStation());
				double	cx1		= pBxStt->GetStation() + dLen - vJ.y - Dom.Always(Dom.GetDimTvp());
				double	cx2		= pBxStt->GetStation() + dLen + vJ.y + Dom.GetTextHeight() + Dom.Always(Dom.GetDimTvp());		

				Dom.SetTextAngle(90);

				Dom.TextOut(cx1, YCen-(HCen+dAddSpace)/2, "지점보강재");		
				Dom.TextOut(cx2, YCen-(HCen+dAddSpace)/2, pOpt->GetJewonString(nJVStiff,vJ.x,vJ.y,vJ.z));

				bVStiffDraw = TRUE;
			}
		}
		else	// 수직 보강재, 가로보 연결 수직보강재 제원 
		{
			CVector vJ;

			// 수직보강재 양측설치시 수직보강재 갯수는 2개로 함(단, 수직보강재 위치인 경우만--가로보위치는 따로 계산함)
			long nCount = 0;

			if (pBx->IsState(BX_CROSSBEAM_VBRACING))
			{
				nCount = 1;// 수동으로 계산할것.....

				if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
				{
					CPlateCrossBeam *pC = pBx->GetCrossBeam();

					if(pC->GetType()==CR_TYPE_HSTEEL)	// H형강타입
					{
						if(pC->m_pVStiff->m_dWidth[0]>0 && pC->m_pVStiff->m_dWidth[1]>0)			nCount=2;
						else if(pC->m_pVStiff->m_dWidth[0]==0 && pC->m_pVStiff->m_dWidth[1]==0)		nCount=0;
						else																		nCount=1;		
					
						double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());

						vJ.x = pC->m_pVStiff->m_dWidth[nCol];
						vJ.y = pC->m_pVStiff->m_dThick[nCol];
						vJ.z = pC->m_pVStiff->GetVStiffHeight(nCol==0?TRUE:FALSE, dHGir);
					}
					else
						vJ = CVector(0,0,0);
				}
				else 
				{
					CVBracing *pVr = pBx->GetSection()->GetVBracing();

					if(pVr->m_pVStiff->m_dWidth[0] > 0 && pVr->m_pVStiff->m_dWidth[1] > 0)			nCount=2;
					else if(pVr->m_pVStiff->m_dWidth[0] == 0 && pVr->m_pVStiff->m_dWidth[1] == 0)	nCount=0;
					else																			nCount=1;		

					double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());

					vJ.x = pVr->m_pVStiff->m_dWidth[nCol];
					vJ.y = pVr->m_pVStiff->m_dThick[nCol];
					vJ.z = pVr->m_pVStiff->GetVStiffHeight(nCol==0?TRUE:FALSE, dHGir);
				}
			}
			else
			{
				nCount	=	(pGir->m_nInstallVStiff == 0) ? 2 : 1;
				vJ		=	pGir->GetJewonTotal(V_ST, pBx, 0, nCol, DIM_BASIC);
			}

			double dLen = pGir->GetLengthDimType(pBxStt->GetStation(), pBx->GetStation());
			double cx1 = pBxStt->GetStation() + dLen - vJ.y - Dom.Always(Dom.GetDimTvp());
			double cx2 = pBxStt->GetStation() + dLen + vJ.y + Dom.GetTextHeight() + Dom.Always(Dom.GetDimTvp());		

			Dom.SetTextAngle(90);
			// 공장제작타입 가로보에서는 "CROSS BEAM PL."로 표시함
			if(pBx->IsState(BX_CROSSBEAM_VBRACING))	
			{
				if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
				{
					CPlateCrossBeam *pC = pBx->GetCrossBeam();
					if(pC->GetType()==CR_TYPE_HSTEEL)	// H형강타입
					{
						Dom.TextOut(cx1, YCen-(HCen+dAddSpace)/2, "V-STIFF");		
						Dom.TextOut(cx2, YCen-(HCen+dAddSpace)/2, pOpt->GetJewonString(nCount,vJ.x,vJ.y,vJ.z));
						
						//양측경우 좌, 우의 값이 틀리면 조건에 따라서 
						//좌,우 양측 제원을 표시함.
						if(pC->m_pVStiff->m_dWidth[0] > 0 && pC->m_pVStiff->m_dWidth[1] > 0)
						{
							if((pC->m_pVStiff->m_dWidth[0] != pC->m_pVStiff->m_dWidth[1]) || (pC->m_pVStiff->m_dThick[0] != pC->m_pVStiff->m_dThick[1]) || bJewonLeftRigh)
							{
								double	dHGir	=	pGir->GetHeightGirderByStaAng(pBx->GetStation());
								
								vJ.x = pC->m_pVStiff->m_dWidth[abs(nCol-1)];
								vJ.y = pC->m_pVStiff->m_dThick[abs(nCol-1)];
								vJ.z = pC->m_pVStiff->GetVStiffHeight(abs(nCol-1)==0?TRUE:FALSE, dHGir);

								Dom.TextOut(cx2+Dom.GetTextHeight()+Dom.Always(0.5), YCen-(HCen+dAddSpace)/2, "(" + pOpt->GetJewonString(nCount,vJ.x,vJ.y,vJ.z) + ")");
							}
						}
					}
					else
						Dom.TextOut(cx1, YCen-(HCen+dAddSpace)/2, "CROSS BEAM");	
				}
				else
				{
					CVBracing *pVr = pBx->GetSection()->GetVBracing();

					Dom.TextOut(cx1, YCen-(HCen+dAddSpace)/2, "V-STIFF");		
					Dom.TextOut(cx2, YCen-(HCen+dAddSpace)/2, pOpt->GetJewonString(nCount,vJ.x,vJ.y,vJ.z));
					
					//양측경우 좌, 우의 값이 틀리면 조건에 따라서 
					//좌,우 양측 제원을 표시함.
					if(pVr->m_pVStiff->m_dWidth[0] > 0 && pVr->m_pVStiff->m_dWidth[1] > 0)
					{
						if((pVr->m_pVStiff->m_dWidth[0] != pVr->m_pVStiff->m_dWidth[1]) || (pVr->m_pVStiff->m_dThick[0] != pVr->m_pVStiff->m_dThick[1]) || bJewonLeftRigh)
						{
							double	dHGir	=	pGir->GetHeightGirderByStaAng(pBx->GetStation());
							
							vJ.x = pVr->m_pVStiff->m_dWidth[abs(nCol-1)];
							vJ.y = pVr->m_pVStiff->m_dThick[abs(nCol-1)];
							vJ.z = pVr->m_pVStiff->GetVStiffHeight(abs(nCol-1)==0?TRUE:FALSE, dHGir);

							Dom.TextOut(cx2+Dom.GetTextHeight()+Dom.Always(0.5), YCen-(HCen+dAddSpace)/2, "(" + pOpt->GetJewonString(nCount,vJ.x,vJ.y,vJ.z) + ")");
						}
					}
				}
			}
			else		
			{
				
				CVStiff *pVStiff = pBx->GetSection()->GetVStiff();
				Dom.TextOut(cx1, YCen-(HCen+dAddSpace)/2,"V-STIFF");		
				Dom.TextOut(cx2, YCen-(HCen+dAddSpace)/2, pOpt->GetJewonString(nCount,vJ.x,vJ.y,vJ.z));
				
				//양측경우 좌, 우의 값이 틀리면 조건에 따라서 
				//좌,우 양측 제원을 표시함.
				if(pVStiff->m_dWidth[0] > 0 && pVStiff->m_dWidth[1] > 0)
				{
					if((pVStiff->m_dWidth[0] != pVStiff->m_dWidth[1]) || (pVStiff->m_dThick[0] != pVStiff->m_dThick[1]) || bJewonLeftRigh)
					{
						double	dHGir	=	pGir->GetHeightGirderByStaAng(pBx->GetStation());
						
						vJ.x = pVStiff->m_dWidth[abs(nCol-1)];
						vJ.y = pVStiff->m_dThick[abs(nCol-1)];
						vJ.z = pVStiff->GetVStiffHeight(abs(nCol-1)==0?TRUE:FALSE, dHGir);

						Dom.TextOut(cx2+Dom.GetTextHeight()+Dom.Always(0.5), YCen-(HCen+dAddSpace)/2, "(" + pOpt->GetJewonString(nCount,vJ.x,vJ.y,vJ.z) + ")");

					}
				}
			}
		}
		
		pBx = Finder.GetBxNext();
	}
	*pDomP << Dom;
}

void CAPlateDrawJong::DrawJackUpOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, long nCol, long nLine, long nJewonType)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CGlobarOption   *pGlopt  = m_pDataMng->GetGlobalOption();
	CARoadOptionStd *pOpt	 = m_pDataMng->GetOptionStd();	
	CPlateGirderApp	*pGir    = pBx->GetGirder();
	BOOL bLeftHStiff = nCol==0 ? TRUE : FALSE;
	
	double dSta = pBx->GetStation();
	long   nJ   = pGir->GetNumberJijumByBx(pBx);
	CJackupStiff *pJackUp = pGir->GetJackupByJijum(nJ, 0);
	double vT = pJackUp->m_vJewon[nCol].y;
	double vW = pJackUp->m_vJewon[nCol].x;

	BOOL bVstiffExist = (vT && vW) ? TRUE : FALSE;

	if(nLine==0) vT = 0; 

	double dThick    = pGlopt->GetDesignHeigtBaseUpper()==0  && pBx->GetFactChain(G_F_U) ? pBx->GetFactChain(G_F_U)->m_dFactChainThick : 0;
	// 수직부재 그리기
	double XStt = -vT/2;
	double XEnd =  vT/2;
	double YStt = pGir->GetElevationFlangeCamber(dSta-vT/2)-dThick;
	double YCen = pGir->GetElevationFlangeCamber(dSta)-dThick;
	double YEnd = pGir->GetElevationFlangeCamber(dSta+vT/2)-dThick;
	double HStt = pJackUp->m_dVL == 0 ? pGir->GetHeightGirderByStaAng(dSta-vT/2) : pJackUp->m_dVL;
	double HCen = pJackUp->m_dVL == 0 ? pGir->GetHeightGirderByStaAng(dSta) : pJackUp->m_dVL;
	double HEnd = pJackUp->m_dVL == 0 ? pGir->GetHeightGirderByStaAng(dSta+vT/2) : pJackUp->m_dVL;

	CAPlateSymbolDom Dom(pDom,m_pDataMng);			
	if(bVstiffExist)
	{
		if(nLine!=1)
			Dom.LineTo(XStt,YStt-pGir->GetHeightGirderByStaAng(dSta-vT/2), XStt,YStt-pGir->GetHeightGirderByStaAng(dSta-vT/2)+HStt);
		if(nLine!=1 && vT>0)
			Dom.LineTo(XEnd,YEnd-pGir->GetHeightGirderByStaAng(dSta+vT/2), XEnd,YEnd-pGir->GetHeightGirderByStaAng(dSta+vT/2)+HEnd);
		if(pJackUp->m_dVL!=0)
			Dom.LineTo(XStt,YStt-pGir->GetHeightGirderByStaAng(dSta-vT/2)+HStt, XEnd,YEnd-pGir->GetHeightGirderByStaAng(dSta+vT/2)+HEnd);
	}

	CString szH = "";
	if(pGir->IsByunByStation(dSta))
		szH.Format("(%.0f)",pBx->GetHeightGirderByBx());

	CWebHStiff *pHStiff = pBx->GetWebHStiff();
	double dUpSpace = 0;
	double dDnSpace = 0;
	double dAddSpace = 0;
	if(pHStiff) 
	{
		long nUDan = pHStiff->GetUpperDan(bLeftHStiff);
		long nLDan = pHStiff->GetLowerDan(bLeftHStiff);
		dUpSpace =  nUDan == 0 ? 0 : pHStiff->GetHeightHStiffOnStt(nUDan-1, bLeftHStiff, TRUE);						
		dDnSpace =	pHStiff->GetHeightHStiffOnStt(nLDan+2, bLeftHStiff, TRUE);
		if(nLDan!=0) dDnSpace = HCen - dDnSpace;
		dAddSpace = dUpSpace - dDnSpace;
	}
	if(bVstiffExist && nJewonType>=0)
	{
		pOpt->SetEnvType(&Dom,HCAD_DIML);		
		double cx1, cx2;
		double cy1;
		Dom.SetTextAngle(90);
		cx1 = XStt - Dom.Always(Dom.GetDimTvp());
		cx2 = XStt + vT + Dom.GetTextHeight() + Dom.Always(Dom.GetDimTvp());
		cy1 = YStt-pGir->GetHeightGirderByStaAng(dSta-vT/2)+(HStt/2);
		
		CString szJewon;
		CVector vJ = pGir->GetJewonTotal(GI_JA_L, pBx, 0, 0);	
		if(vJ==CVector(0,0,0))
			vJ = pGir->GetJewonTotal(GI_JA_L, pBx, 0, 1);	

		if(nJewonType>4)
		{
			szJewon.Format("%.0f-%.0f-%.0f",vJ.y,vJ.x,vJ.z);
			Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);			
		}
		else
		{
			CString szMark="";		
			long nG = pBx->GetNumberGirder();
			szMark	= pBridge->GetMarkBuje(GI_JA_L,pBx,0);									
			szJewon = pOpt->GetJewonString(1,vJ.x,vJ.y,vJ.z,"PL",FALSE);					
			
			switch(nJewonType)
			{
			case 0:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 1:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szMark);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 2:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon + "-" + szMark);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 3:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,"-" + szMark+szH);
				break;
			case 4:				
				Dom.TextOut(cx1,cy1,"2-JACK UP 보강재 "+szJewon);
				//Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,"2-JACK UP 보강재 "+szJewon);
			}		
		}
	}	

	double Xp = dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation());
	Dom.Move(Xp, 0);
	*pDom << Dom;	
}

void CAPlateDrawJong::DrawJackUpBx(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine, long nJewonType, BOOL bApplyHiddenLine)
{		
	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	__int64 BxFlag = BX_JACKUP;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	CDomyun Dom(pDom);	
	
	while(pBx)
	{
		long   nJ   = pGir->GetNumberJijumByBx(pBx);
		CJackupStiff *pJackUp = pGir->GetJackupByJijum(nJ, 0);
		double vWL = pJackUp->m_vJewon[0].x;
		double vWR = pJackUp->m_vJewon[1].x;
		// 지점보강재가 양측에 있을 경우는 우측면을 그리고 
		// 중요사항 : 보강재의방향이 수평보강재의설치방향과 다른 경우는 LT_DOT_LINE으로 그림.
		if((vWL>0 && vWR>0) || (vWL==0 && vWR>0))	// 우측 보강재 그림
		{
			if(vWL>0 && vWR>0)	// 둘다 있을 경우
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
			else				// 우측에만 있을 경우
			{
				if(pBx->GetGirder()->m_nInstallVStiff==1)	// 좌측
				{
					if(bApplyHiddenLine)		pOptStd->SetEnvType(&Dom, HCAD_STLH);
					else						pOptStd->SetEnvType(&Dom, HCAD_STLC);
				}
				else										// 우측	
					pOptStd->SetEnvType(&Dom, HCAD_STLC);
			}
			DrawJackUpOnBx(&Dom, pBx, pBxStt->GetStation(), 1, nLine, nJewonType);		// 우측그림
		}
		else if(vWL>0 && vWR==0)					// 좌측 보강재 그림
		{
			if(pBx->GetGirder()->m_nInstallVStiff==2)	//우측
			{
				if(bApplyHiddenLine)		pOptStd->SetEnvType(&Dom, HCAD_STLH);
				else						pOptStd->SetEnvType(&Dom, HCAD_STLC);
			}
			else
				pOptStd->SetEnvType(&Dom, HCAD_STLC);

			DrawJackUpOnBx(&Dom, pBx, pBxStt->GetStation(), 0, nLine, nJewonType);		// 좌측그림
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::DrawJiJumVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, long nCol, long nLine, long nJewonType)
{
	CGlobarOption   *pGlopt  = m_pDataMng->GetGlobalOption();
	CARoadOptionStd *pOpt	= m_pDataMng->GetOptionStd();	
	CPlateGirderApp	*pGir	= (CPlateGirderApp *)pBx->GetGirder();
	
	double dSta = pBx->GetStation();
	long   nJ   = pGir->GetNumberJijumByBx(pBx);
	CVStiff *pJVStiff = pGir->GetJijumVStiffByJijum(nJ, 0);
	double vT = pJVStiff->m_dThick[nCol];
	double vW = pJVStiff->m_dWidth[nCol];

	BOOL bVstiffExist = (vT && vW) ? TRUE : FALSE;

	if(nLine==0)
		vT = 0; 

	double dThick    = pGlopt->GetDesignHeigtBaseUpper()==0  && pBx->GetFactChain(G_F_U) ? pBx->GetFactChain(G_F_U)->m_dFactChainThick : 0;
	// 수직부재 그리기
	double XStt = -vT/2;
	double XEnd =  vT/2;
	double YStt = pGir->GetElevationFlangeCamber(dSta-vT/2) - dThick;
	double YCen = pGir->GetElevationFlangeCamber(dSta) - dThick;
	double YEnd = pGir->GetElevationFlangeCamber(dSta+vT/2) - dThick;
	double HStt = pGir->GetHeightGirderByStaAng(dSta-vT/2);
	double HCen = pGir->GetHeightGirderByStaAng(dSta);
	double HEnd = pGir->GetHeightGirderByStaAng(dSta+vT/2);
		
	CAPlateSymbolDom Dom(pDom,m_pDataMng);			
	if(bVstiffExist)
	{
		if(nLine!=1)
			Dom.LineTo(XStt,YStt, XStt,YStt-HStt);
		if(nLine!=1 && vT>0)
			Dom.LineTo(XEnd,YEnd, XEnd,YEnd-HEnd);
	}

	///////////////////////////////////////////////////// By Shin
	CString szH = "";
	if(pGir->IsByunByStation(dSta))
		szH.Format("(%.0f)",pBx->GetHeightGirderByBx());
	
	double dAddSpace = 0;
	if(bVstiffExist && nJewonType>=0)
	{
		pOpt->SetEnvType(&Dom,HCAD_DIML);		
		double cx1, cx2;
		Dom.SetTextAngle(90);
		cx1 = XStt - Dom.Always(Dom.GetDimTvp());
		cx2 = XStt + vT + Dom.GetTextHeight() + Dom.Always(Dom.GetDimTvp());
		
		CString szJewon;
		CVector vJ = pGir->GetJewonTotal(V_J_ST, pBx, 0, nCol, DIM_BASIC);	

		if(nJewonType>4)
		{
			if(!vJ.IsNull()) szJewon.Format("%.0f-%.0f-%.0f",vJ.y,vJ.x,vJ.z);
			Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);			
		}
		else
		{
			CString szMark="";		
			long nG = pBx->GetNumberGirder();
			szMark	= "MARK_수정";//pBridge->GetMarkBuje(B_V, pBx, 0,(nDir==-1)?0:1);									
/*
			if(pStd->FindUseMark(szMark,m_sMarkArray)) 
*/
				szJewon = pOpt->GetJewonString(1,vJ.x,vJ.y,vJ.z,"PL",FALSE);
/*			else
			{
				m_sMarkArray.Add(szMark);
				szJewon = pOpt->GetJewonString(1,vJ.x,vJ.y,vJ.z,"PL",FALSE);					
			}		
*/			
			switch(nJewonType)
			{
			case 0:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 1:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szMark);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 2:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon + "-" + szMark);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szH);
				break;
			case 3:
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,szJewon);
				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,"-" + szMark+szH);
				break;
			case 4:				
				Dom.TextOut(cx1,YCen-(HCen+dAddSpace)/2,"지점부 보강재 "+szJewon);
//				Dom.TextOut(cx2,YCen-(HCen+dAddSpace)/2,szJewon);
			}		
		}
	}	

	double Xp = dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation(), 0);
	Dom.Move(Xp, 0);
	*pDom << Dom;	
}

void CAPlateDrawJong::DrawJiJumVStiffBx(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nLine, long nJewonType, BOOL bApplyHiddenLine)
{		
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	__int64 BxFlag = BX_JIJUMSTIFF;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	CDomyun Dom(pDom);	
	while(pBx)
	{
		long   nJ   = pGir->GetNumberJijumByBx(pBx);
		CVStiff *pVStiff = pGir->GetJijumVStiffByJijum(nJ, 0);
		double vWL = pVStiff->m_dWidth[0];
		double vWR = pVStiff->m_dWidth[1];
		// 지점보강재가 양측에 있을 경우는 우측면을 그리고 
		// 중요사항 : 보강재의방향이 수평보강재의설치방향과 다른 경우는 LT_DOT_LINE으로 그림.
		if((vWL>0 && vWR>0) || (vWL==0 && vWR>0))	// 우측 보강재 그림
		{
			if(vWL>0 && vWR>0)	// 둘다 있을 경우
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
			else				// 우측에만 있을 경우
			{
				if(pBx->GetGirder()->m_nInstallVStiff==1)	// 좌측
				{
					if(bApplyHiddenLine)		pOptStd->SetEnvType(&Dom, HCAD_STLH);
					else						pOptStd->SetEnvType(&Dom, HCAD_STLC);
				}
				else										// 우측	
					pOptStd->SetEnvType(&Dom, HCAD_STLC);
			}
			DrawJiJumVStiffOnBx(&Dom, pBx, pBxStt->GetStation(), 1, nLine, nJewonType);		// 우측그림
		}
		else if(vWL>0 && vWR==0)					// 좌측 보강재 그림
		{
			if(pBx->GetGirder()->m_nInstallVStiff==2)	//우측
			{
				if(bApplyHiddenLine)		pOptStd->SetEnvType(&Dom, HCAD_STLH);
				else						pOptStd->SetEnvType(&Dom, HCAD_STLC);
			}
			else
				pOptStd->SetEnvType(&Dom, HCAD_STLC);

			DrawJiJumVStiffOnBx(&Dom, pBx, pBxStt->GetStation(), 0, nLine, nJewonType);		// 좌측그림
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::DrawPier(CDomyun *pDomP, long nPier, long nG, CDPoint MoveXY, BOOL bCut, BOOL bMark, BOOL bSimpleType)
{	
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();		
	BOOL bAbut = FALSE;

	if(nPier==0 && pBridge->m_nTypeAbutOrPier[0]==0) bAbut = TRUE;
	if(nPier==pBridge->m_nQtyJigan && pBridge->m_nTypeAbutOrPier[1]==0) bAbut = TRUE;

	CDomyun *pDom = new CDomyun(pDomP);

	if(bAbut)
	{
		DrawAbutSide(pDom, nPier, nG, bCut, bMark);
	}
	else
		DrawPierSide(pDom, nPier, nG, bCut, bMark, bSimpleType);

	pDom->Move(MoveXY);

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawJong::DrawPierSide(CDomyun *pDomP, long nPier, long nG, BOOL bCut, BOOL bMark, BOOL bSimpleType)
{	
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();		
	CPierInfo		*pInfo = pBridge->m_PierInfo + nPier;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun* pDom = new CDomyun(pDomP);

	double HeightPier = bCut ? pInfo->m_dpH1+pInfo->m_dpH2+1000
	                         : pInfo->m_dpH1+pInfo->m_dpH2+pInfo->m_dpH3+pInfo->m_dpH4;
	double WidthCopp  = pInfo->GetWidth();
	double HeightCopp = pInfo->m_dpH1;
	double HeightHunc = pInfo->m_dpH2;
	double ColumnDia  = pInfo->m_dpD;
	double WidthFoot  = pInfo->m_dpB1+pInfo->m_dpD+pInfo->m_dpB2;
	double HeightFoot = bCut ? 0 : pInfo->m_dpH4;
	CPlateGirderApp	*pGir = pBridge->GetGirder(nG);
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nPier);
	double FlT = pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower()==1) FlT = 0;

	CDPoint xy(0,-FlT);

	if(bSimpleType)
	{
		CDPoint xy1(xy.x-WidthCopp/2, xy.y);
		CDPoint xy2(xy.x+WidthCopp/2, xy.y-HeightCopp-HeightHunc);
		double wing = 500;
		pDom->LineTo(xy1.x, xy2.y, xy1.x, xy1.y);
		pDom->LineTo(xy1.x, xy1.y, xy2.x,xy1.y);
		pDom->LineTo(xy2.x,xy1.y,xy2.x, xy2.y);
		pOptStd->SetEnvType(pDom, HCAD_CUTL);
		pDom->CutLightning(xy1.x-wing, xy2.y, xy2.x+wing, xy2.y ,FALSE);
		pOptStd->SetEnvType(pDom, HCAD_STLC);
	}
	if(!bCut)
	{
		pDom->Rectangle(xy.x-WidthFoot/2,xy.y-HeightPier+HeightFoot,
						xy.x+WidthFoot/2,xy.y-HeightPier);
		pDom->Rectangle(xy.x-WidthFoot/2-pInfo->m_dpK1,xy.y-HeightPier,
						xy.x+WidthFoot/2+pInfo->m_dpK1,xy.y-HeightPier-pInfo->m_dpK2);
	}
	if(!bSimpleType)
	{
		pDom->Rectangle(xy.x-WidthCopp/2,xy.y, 
						xy.x+WidthCopp/2,xy.y-HeightCopp);
		pDom->Rectangle(xy.x-WidthCopp/2,xy.y-HeightCopp,
						xy.x+WidthCopp/2,xy.y-HeightCopp-HeightHunc);
		pDom->Rectangle(xy.x-ColumnDia/2,xy.y-HeightCopp-HeightHunc,
						xy.x+ColumnDia/2,xy.y-HeightPier+HeightFoot);
		// HUNCH
		for(long i=1; i<4; i++)
			pDom->LineTo(xy.x-WidthCopp/2,xy.y-HeightCopp-HeightHunc*i/4,
						 xy.x+WidthCopp/2,xy.y-HeightCopp-HeightHunc*i/4);
		// COLUMN
		pDom->LineTo(xy.x-ColumnDia*0.98/2,xy.y-HeightCopp-HeightHunc,
					 xy.x-ColumnDia*0.98/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x+ColumnDia*0.98/2,xy.y-HeightCopp-HeightHunc,
					 xy.x+ColumnDia*0.98/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x-ColumnDia*0.92/2,xy.y-HeightCopp-HeightHunc,
					 xy.x-ColumnDia*0.92/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x+ColumnDia*0.92/2,xy.y-HeightCopp-HeightHunc,
					 xy.x+ColumnDia*0.92/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x-ColumnDia*0.80/2,xy.y-HeightCopp-HeightHunc,
					 xy.x-ColumnDia*0.80/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x+ColumnDia*0.80/2,xy.y-HeightCopp-HeightHunc,
					 xy.x+ColumnDia*0.80/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x-ColumnDia*0.60/2,xy.y-HeightCopp-HeightHunc,
					 xy.x-ColumnDia*0.60/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x+ColumnDia*0.60/2,xy.y-HeightCopp-HeightHunc,
					 xy.x+ColumnDia*0.60/2,xy.y-HeightPier+HeightFoot);
		pDom->LineTo(xy.x,xy.y-HeightCopp-HeightHunc,
					 xy.x,xy.y-HeightPier+HeightFoot);

	}

	// MOVE,FIX
	BOOL bFix = FALSE;
	for(long n=0; n<pBridge->GetGirdersu(); n++) 
	{
		CPlateGirderApp  *pGir = pBridge->GetGirder(n);
		CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(nPier);
		CShoe *pShoe = pGir->GetShoeByJijumNo(nPier);
		if(pShoe->GetDirectionShoe()==0)	bFix = TRUE;

		if(bFix) break;
	}
	CString str = bFix ? "F" : "M";
	pOptStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextAlignHorz(TA_CENTER);
	pDom->TextCircle(xy.x, xy.y-pDom->GetTextHeight()*1.5, str, -1);

	xy.x += ( WidthCopp/2 + pDom->Always(2) + pDom->GetTextWidth(str) );
	pDom->TextCircle(xy.x, xy.y-pDom->GetTextHeight()*1.5, pBridge->m_strJijumName[nPier], 3);

	*pDomP << *pDom;
	delete pDom;
}


//기능 : 슈표현과 슈마크 호출
//입력 : doulbe ySeperate
//출력 : 베이스 라인으로 부터 이격 거리 비율로 세팅 default =1, 0.5인경우 절반
void CAPlateDrawJong::DrawShoeMarkBx(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double Scale, BOOL bGeneral, double ySeperateRatio, BOOL bHatch/* = FALSE*/)
{
  	CPlateBridgeApp *pBridge	= m_pDataMng->GetBridge();		
	CPlateGirderApp	*pGir		= (CPlateGirderApp *)pBxStt->GetGirder();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CPlateBxFinder	Finder(pGir);
	
	__int64 BxFlag = BX_JIJUM;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);

	CDPoint sp = CDPoint(0,0);

	// 좌우측 보정
	CDPoint vAngle	= CDPoint(0,1);
	double CurSta	= 0.0;
	double sttSta	= pBxStt->GetStation();
	double Tl		= 0.0;

	CDomyun	*p		= new CDomyun(pDom);
	long	nJijum	= 0;

	p->SetScaleDim(Scale);
	
	while(pBx)
	{
		if(pBx->IsJijum())
		{
			vAngle = pBx->GetAngle();
			CurSta = pBx->GetStation();
			// 실제거더의 길이로 X좌표를 산정함(실제의 STATION(cSta)과는 다름)
			if(pBx->GetFactChain(G_F_L))
				Tl   = pBx->GetFactChain(G_F_L)->m_dFactChainThick;
			else
				Tl = 0;
			if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower()==1) Tl=0;
			sp.x = sttSta + pGir->GetLengthDimType(sttSta,CurSta,0);
			sp.y = pGir->GetElevationFlangeCamber(CurSta, 0, vAngle, FALSE)
				 - Tl;
			// SHOE MARK
			double ShoeHeight = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx))->GetHeightTotalShoe();
			//double ShoeHeight = pBx->GetDiaphragm()->m_ShoeFir.GetHeightTotalShoe();
			double L = 500;
			double H = ShoeHeight;
			if (bHatch)
			{
				p->LineTo(sp.x ,sp.y, sp.x+L/2, sp.y-H);
				p->LineTo(sp.x+L/2, sp.y-H, sp.x-L/2, sp.y-H);
				p->LineTo(sp.x ,sp.y, sp.x-L/2, sp.y-H);
				pOptStd->SetEnvType(p,HCAD_HATH);
				p->HatchRect(sp.x ,sp.y, sp.x+L/2, sp.y-H, sp.x-L/2, sp.y-H, sp.x,sp.y,"ANSI31");
			}
			else
				p->Solid(sp.x ,sp.y, sp.x+L/2, sp.y-H, sp.x-L/2,sp.y-H, sp.x, sp.y);

			// SHOE TEXT
			if(bGeneral)
			{
				CString str = pBridge->m_strJijumName[pBx->GetNumberJijum()];
				
				CAPlateSymbolDom Dom(pDom,m_pDataMng);
				pOptStd->SetEnvType(&Dom,HCAD_SYMB);
				sp.y -= Dom.GetTextHeight()*5;
				Dom.TextCircle(sp.x, sp.y, str);
			 	*pDom << Dom;
			}
			nJijum++;
		}
		pBx = Finder.GetBxNext();
	}

 	*pDom << *p;
	delete p;
}

// (0,0)은 교량시종점.
void CAPlateDrawJong::DrawAbutSide(CDomyun *pDomP, long nPier, long nG, BOOL bCut, BOOL bMark)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();		
	CPierInfo		*pInfo = pBridge->m_PierInfo + nPier;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun* pDom = new CDomyun(pDomP);

	BOOL bStt = nPier ? FALSE : TRUE;
	double sw = bStt  ? 1.0 : -1.0;

	// 교대  단면도
	double dT1 = pInfo->m_daT1*sw;
	double dT2 = pInfo->m_daT2*sw;
	double dT3 = pInfo->m_daT3*sw;
	double dT4 = pInfo->m_daT4*sw;
	double dH1 = pInfo->m_daH1;
	double dH2 = pInfo->m_daH2;
	double dH3 = pInfo->m_daH3;
	double dH4 = pInfo->m_daH4;
	double dH5 = pInfo->m_daH5;
	double dH6 = pInfo->m_daH6;
	double dH7 = pInfo->m_daH7;
	double dH8 = pInfo->m_daH8;
	double dB1 = pInfo->m_daB1*sw;
	double dB2 = pInfo->m_daB2*sw;
	double dB3 = pInfo->m_daB3*sw;
	double dK1 = pInfo->m_daK1*sw;
	double dK2 = pInfo->m_daK2*sw;

	// dH3를 다시 구함 : 거더중심을 기준으로 
	CPlateGirderApp	*pGir  = (CPlateGirderApp *)pBridge->GetGirder(nG);
	double dStaCen	   = bStt ? pGir->GetStationStt() : pGir->GetStationEnd();
	double dThickPave  = pBridge->m_dThickPave;
	double dThickSlab  = pBridge->m_dThickSlabBase;
	double dHgtBoxLF   = pGir->GetHeightFlangeUpperByTaper(dStaCen) + pGir->GetHeightGirderByStaAng(dStaCen)
		               + dThickSlab + dThickPave;
	CPlateBasicIndex *pBx   = pGir->GetBxOnJijum(nPier);
	double dHeightShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx))->GetHeightTotalShoe();
	double dBindGap	= pBridge->m_BindConc.m_dHeight-pBridge->m_BindConc.m_dDeep-pBridge->m_BindConc.m_dDis;

	dH3 = dHgtBoxLF + dHeightShoe - (dH1+dH2) + dBindGap;
	dH3 += (m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower()==0 ? 0 : pGir->GetThickJewon(G_F_L,pBx));
	dH3 += (m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper()==1 ? 0 : pGir->GetThickJewon(G_F_U,pBx));
	
	double Base = dB1+dB2+dB3;
	double tipL = 300;
	double dCut = -dH6-dH7-dH8-tipL;	// 절단할 경우의 위치

	CDPoint xy(0,0);

	// 단면도
	pDom->MoveTo(xy);
	xy.y -= dH1;		pDom->LineTo(xy);
	xy.x -= dT3;	
	xy.y -= dH2;		pDom->LineTo(xy);
	xy.y -= dH3;		pDom->LineTo(xy);
	xy.x += dT4;		pDom->LineTo(xy);
	if(bCut)
		pDom->LineTo(xy.x,dCut);
	else
	{
		xy.y -= dH4;	
		pDom->LineTo(xy);
	}

	if(!bCut)
	{
		pDom->MoveTo(xy);
		xy.x += dB3;		pDom->LineTo(xy);
		xy.y -= dH5;		pDom->LineTo(xy);
		xy.x -= Base;		pDom->LineTo(xy);
		xy.y += dH5;		pDom->LineTo(xy);
		xy.x += dB1;		pDom->LineTo(xy);
	}
	else
	{
		xy.x = dT4-dT3-dB2;
		xy.y = dCut;
	}

	pDom->MoveTo(xy);
	if(bCut)
		xy.y += tipL;
	else
		xy.y += (dH1+dH2+dH3+dH4-dH6-dH7-dH8);	pDom->LineTo(xy);
	xy.x -= (dT1+dT2-dT3+dT4-dB2);
	xy.y += dH8;		pDom->LineTo(xy);
	xy.y += dH7;		pDom->LineTo(xy);
	xy.x += dT1;		pDom->LineTo(xy);
	xy.y += dH6;		pDom->LineTo(xy);
	xy.x += dT2;		pDom->LineTo(xy);

	// 바닥
	if(!bCut)
	{
		pDom->MoveTo(xy);
		xy.x = dT4-dT3+dB3+dK2;
		xy.y = -(dH1+dH2+dH3+dH4+dH5);
		pDom->MoveTo(xy);
		xy.x -= (dB1+dB2+dB3+dK2*2);
		pDom->LineTo(xy);
		xy.y -= dK1;
		pDom->LineTo(xy);
		xy.x += (dB1+dB2+dB3+dK2*2);
		pDom->LineTo(xy);
		xy.y += dK1;
		pDom->LineTo(xy);
	}
	else
	{
		pOptStd->SetEnvType(pDom,HCAD_CUTL);
		CDPoint sp,ep;
		sp.x = dT4-dT3+dB3-Base+dB1-tipL;
		sp.y = dCut;
		ep.x = dT4-dT3+tipL;
		ep.y = dCut;
		if(nPier!=0)
		{
			sp.x = dT4-dT3-tipL;
			ep.x = dT4-dT3-dB2+tipL;
		}
		pDom->CutLightning(sp.x,sp.y,ep.x,ep.y,FALSE);
	}

	// MOVE,FIX
	BOOL bFix = FALSE;
	for(long n=0; n<pBridge->GetGirdersu(); n++) 
	{
		CPlateGirderApp  *pGir = pBridge->GetGirder(n);
		CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(nPier);
		CShoe *pShoe = pGir->GetShoeByJijumNo(nPier);
		if(pShoe->GetDirectionShoe()==0)	bFix = TRUE;

		if(bFix) break;
	}
	xy.x =  (dT4 - dT3)/2;
	xy.y = -(dH1+dH2+dH3+pDom->GetTextHeight()*1.5);
	
	CString str = bFix ? "F" : "M";
	pOptStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextAlignHorz(TA_CENTER);
	pDom->TextCircle(xy.x, xy.y, str, -1);

	// A
	if(bMark)
	{
		xy.x =  (dT4 - dT3) + sw*pDom->GetTextHeight()*2;
		pDom->TextCircle(xy.x, xy.y, pBridge->m_strJijumName[nPier], 3);
	}

	*pDomP << *pDom;
	delete pDom;
}


// 기능 : 현장이음  해치
void CAPlateDrawJong::DrawFieldSpliceHatchJong(CDomyun* pDomP,  CPlateBasicIndex *pBxStt, 
									CPlateBasicIndex *pBxEnd , long Dir, BOOL bCenterLineOnly, BOOL bSpliceMark,
									double dFlangeThickScalce, double dGirderHeightScale, BOOL bRealDraw, long nType)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();	
	CGlobarOption	*pOpt	 = m_pDataMng->GetGlobalOption();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_SPLICE;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	double dStaStt = pBxStt->GetStation();

	CDPoint sp,ep;
	double Xpos   = 0; // 도면상의 X좌표
	double CurSta = 0;
	double	dHeightGirder	= 0;
	double	dThickFlangeUp	= 0;
	double	dThickFlangeLo	= 0;
	double	dElevation		= 0;
	long	nHeightBaseUp	= 0;
	long	nHeightBaseLo	= 0;

	BOOL bSpliceType = TRUE; // 현장 이음 / 현장 용접
	
	// 임시 처리//////////////////////////////////
	while(pBx)
	{				
		CDomyun Dom(pDomP);
		CString szLayer = Dom.GetLayerEx();
		nHeightBaseUp	= m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper();
		CurSta			= pBx->GetStation();
		dElevation		= pGir->GetElevationFlangeCamber(CurSta,0);
		dThickFlangeUp	= (nHeightBaseUp==0 ? 0 : pBx->GetFactChain(G_F_U)->m_dFactChainThick);
		Xpos   = dStaStt + pGir->GetLengthDimType(dStaStt,CurSta,0);
		sp.x   = Xpos;
		sp.y   = dElevation + dThickFlangeUp*dFlangeThickScalce;
		
		// 이음 중심선
		nHeightBaseLo	= m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower();
		dHeightGirder	= pGir->GetHeightGirderByStaAng(pBxStt->GetStation());
		dThickFlangeLo	= nHeightBaseLo==0 ? pBx->GetFactChain(G_F_L)->m_dFactChainThick : 0;
		dElevation		= pGir->GetElevationFlangeCamber(CurSta, 0, CDPoint(0,1),FALSE);	
		ep.x = Xpos;
		ep.y = dElevation - dHeightGirder * (dGirderHeightScale - 1) - dThickFlangeLo*dFlangeThickScalce;


		bSpliceType = pBx->GetSplice()->m_bSpliceType;
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		if(bSpliceType)//이음타입..
			Dom.LineTo(sp,ep);
		else if (dFlangeThickScalce == 1 && dGirderHeightScale ==1)		
		{
			//1. 현장용접시 스캘럽인지 아닌지..상판 하판 구분...
			//2. 도면 분할 방식이 현장이음별 인경우 반쪽만...그리기.. 
			//3. 복부 두께와 기준 두께 관계로 R 설정..
			//4. 현재 현장이음은 복부판 두께차가 없다...


			//5. 단면요약도 에서 그려질 경우 스케일 고려 해야 하네...쩝...070309...KB...
			//   일단 스케일이 안들어오면 1,1 이면 그리자....

			long nScallopUpper   = pBridge->GetIndexBaseScallop(G_F_U, G_F_U, G_W, pBx); // 스,채,모,없
			long nScallopLower   = pBridge->GetIndexBaseScallop(G_F_L, G_F_L, G_W, pBx); // 스,채,모,없
			double dR1 = 0;
			double dThick_GW = pBx->GetFactChain(G_W)->m_dFactChainThick;
			double dThick_GFU = pBx->GetFactChain(G_F_U)->m_dFactChainThick;
			double dThick_GFL = pBx->GetFactChain(G_F_L)->m_dFactChainThick;

			double dSttAngU = 180.0;
			double dEndAngU = 180.0;
			double dSttAngL = 0.0;
			double dEndAngL = 180.0;

			if (nType == 1)//도면 분할방식...
			{
				if (pBx == pBxStt)
				{
					dSttAngU = 270.0;
					dEndAngU = 90.0;
					dEndAngL = 90.0;
				}
				
				if (pBx == pBxEnd)
				{					
					dEndAngU = 90.0;
					dSttAngL = 90.0;
					dEndAngL = 90.0;
				}
			}

			CDPoint NSp = sp;
			CDPoint NEp = ep;

			NSp.y = NSp.y - dThick_GFU;
			NEp.y = NEp.y + dThick_GFL;

			if(dThick_GW < pBridge->m_FieldScallop_BaseT)
				dR1 = pBridge->m_FieldScallop_R1;
			else
				dR1 = pBridge->m_FieldScallop_R2;

			CString str = "";
			str.Format("R%.0f", dR1);

			if (nScallopUpper == 0)
			{
				Dom.LineTo(sp,NSp);
				Dom.Arc(NSp,dR1, dSttAngU, dEndAngU);
				NSp.y = NSp.y - dR1;			
			}
			if (nScallopLower == 0)
			{
				Dom.LineTo(NEp,ep);
				Dom.Arc(NEp,dR1, dSttAngL, dEndAngL);
				NEp.y = NEp.y + dR1;				
			}

			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(NSp,NEp);
		}
		else//단면요약도에서 그리는거면..
			Dom.LineTo(sp,ep);

		Dom.SetLayer(szLayer);
		sp.y = pGir->GetElevationFlangeCamber(CurSta) - pGir->GetThickFlangeByBase(CurSta);
		
		long nShape = 0;
		if(bRealDraw)
		{
			if(!bCenterLineOnly && pBx->GetSplice()->m_bSpliceType)
			{
				double dTPre	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
				double dTNext	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
				sp.y = pGir->GetElevationFlangeCamber(CurSta);
				if(!pOpt->GetDesignHeigtBaseUpper())
					sp.y -= max(dTPre,dTNext);
				
				if(pBx==pBxStt)			nShape = 1;
				else if(pBx==pBxEnd)	nShape = -1;
				else					nShape = 0;
				
				DrawWebSplice(&Dom, pBx, sp.x, sp.y, nShape, FALSE);
				DrawFlangeLineWeb(&Dom, pBx, sp.x, sp.y, FALSE);
			}
		}
		else
		{
			CPlateSplice	*pSp	= pBx->GetSplice();

			if(nHeightBaseUp==0)	dThickFlangeUp	= max(pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick, pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick);
			else					dThickFlangeUp	= min(pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick, pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick);
			if(nHeightBaseLo==0)	dThickFlangeLo	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;
			else					dThickFlangeLo	= max(pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick, pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick);

			double	dSpWidth		= (pSp->m_sA + pSp->m_sB*pSp->m_sC + pSp->m_sC1/2)*2;
			double	dSpHeight		= pSp->m_sW;
			double	dHeightGirder	= pGir->GetHeightGirderByStaAng(pBx->GetStation());


			if(pSp->m_bSpliceType)
			{
				if(pSp->m_mT>0)
				{
					double	dMomentWidth	= (pSp->m_mA+pSp->m_mB*pSp->m_mC+pSp->m_mC1/2)*2*dGirderHeightScale;
					sp.y += nHeightBaseUp==0 ? dThickFlangeUp-dThickFlangeUp*dFlangeThickScalce : 0;
					ep.y += dThickFlangeLo*dFlangeThickScalce;
					sp.y -= (dHeightGirder-dSpHeight)/2*dGirderHeightScale;
					ep.y += (dHeightGirder-dSpHeight)/2*dGirderHeightScale;
					
					Dom.Rectangle(sp.x-dMomentWidth/2, sp.y, sp.x+dMomentWidth/2, sp.y+pSp->m_mW*dGirderHeightScale);
					Dom.Rectangle(sp.x-dMomentWidth/2, ep.y, sp.x+dMomentWidth/2, ep.y-pSp->m_mW*dGirderHeightScale);

					sp.x -= dSpWidth/2*dGirderHeightScale;
					ep.x += dSpWidth/2*dGirderHeightScale;
					Dom.Rectangle(sp, ep);
				}
				else
				{
					sp.x -= dSpWidth/2*dGirderHeightScale;
					sp.y += nHeightBaseUp==0 ? dThickFlangeUp-dThickFlangeUp*dFlangeThickScalce : 0;
					ep.x += dSpWidth/2*dGirderHeightScale;
					ep.y += dThickFlangeLo*dFlangeThickScalce;

					sp.y -= (dHeightGirder-dSpHeight)/2*dGirderHeightScale;
					ep.y += (dHeightGirder-dSpHeight)/2*dGirderHeightScale;
					Dom.Rectangle(sp, ep);
				}
			}
		}

		if(bSpliceMark) 
		{
			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			CString szStr = pBx->GetSplice()->GetMarkstring();
			Dom.TextCircle(ep.x,ep.y-Dom.Always(21),szStr,0,TRUE,TRUE,0);		
		}

		pBx  = Finder.GetBxNext();

		*pDomP << Dom;
	}
}

//측면도에 용접기호 생성하기
void CAPlateDrawJong::AddWeldJong(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,double rotAng, BOOL bLeft, BOOL bWeldMarkOne)
{
	CPlateGirderApp *pGir	= pBxStt->GetGirder();
	CARoadOptionStd *pOpt	= m_pDataMng->GetOptionStd();
	CLineInfo*	   pLine	= pGir->GetLine();	
	
	CPlateBxFinder		Finder(pGir);	
	Finder.SetIncludeBothPos(TRUE);
	// 수평보강재 -------------------------------------------------------------------------------------------
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE | BX_WEB | BX_STT_GIRDER | BX_END_GIRDER, pBxStt, pBxEnd), *pBxNext=NULL, *pBxSp = NULL;
	ASSERT(pBx);
	
	CAPlateSymbolDom Dom(pDom, m_pDataMng);			
	pOpt->SetEnvType(&Dom,HCAD_DIML);

	BOOL bSttSplice = FALSE;
	while(Finder.GetBxFixNext())
	{		
		pBxNext = Finder.GetBxFixNext();		
		
		//MAKE JAEWON
		double Sta = (pBx->GetStation() + pBxNext->GetStation()) / 2;
		CPlateBasicIndex *pmidBx = pGir->GetBxByStation(Sta);
		while(pmidBx)
		{
			if(pmidBx->IsState(BX_VSTIFF | BX_CROSSBEAM_VBRACING)) break;
			Sta += 100;
			if(Sta > pBxEnd->GetStation()) break;
			pmidBx = pGir->GetBxByStation(Sta);
			if(pmidBx == pBxEnd) 
			{
				pmidBx = NULL;
				break;
			}
		}
		if(!pmidBx) break;
		Sta = pmidBx->GetStation() + 300;
		if(Sta > pBxEnd->GetStation()) break;
		
		if(Sta > pBxNext->GetStation()-100) Sta = (pBx->GetStation() + pBxNext->GetStation()) / 2;

		double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),pBxEnd->GetStation(), 0);		
		double y1	= pGir->GetElevationFlangeCamber(Sta, 0, CDPoint(0,1), TRUE);
		double y2	= pGir->GetElevationFlangeCamber(Sta, 0, CDPoint(0,1), FALSE);
		
		CWebHStiff *pHStiff = pmidBx->GetWebHStiff(); // 임시적임 nSide);
		if(!pHStiff)
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		long UpperDan = pHStiff->GetUpperDan(bLeft);
		long LowerDan = pHStiff->GetLowerDan(bLeft);
		
		if((UpperDan==0 && LowerDan==0))
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		
		double UpperTerm	= pmidBx->IsCompressByJRib(TRUE)  ? pHStiff->GetHeightHStiffOnStt(UpperDan-1,bLeft,TRUE) : 0;
		double LowerTerm	= pmidBx->IsCompressByJRib(FALSE) ? pHStiff->GetHeightHStiffOnStt(2+LowerDan,bLeft,TRUE) : 0;	
		
		CVector vJ;		
		if(UpperTerm != 0) 
			vJ = pGir->GetJewonTotal(bLeft ? G_H_L : G_H_R, pmidBx,(UpperDan==1) ? 0 : 1,0,DIM_BASIC);
		else if(LowerTerm != 0)
			vJ = pGir->GetJewonTotal(bLeft ? G_H_L : G_H_R, pmidBx,(LowerDan==1) ? 4 : 3,0,DIM_BASIC);
		double HSThick = vJ.y;
		
		double T = HSThick;
//		if(pStd->GetBogusLine())
//		{
//			T = pStd->GetBogusThick();
//		}

		double dHSta;	
		
		if(pmidBx->IsCompressByJRib(TRUE) && UpperDan > 0)
		{
			BOOL bLeft = FALSE;
			dHSta = (pHStiff->m_dStationStt[bLeft ? 0 : 1][UpperDan-1]+pHStiff->m_dStationEnd[bLeft ? 0 : 1][UpperDan-1])/2;
			pBxSp = pGir->GetBxByStation(dHSta, BX_SPLICE);

			double dStaComp = pBxSp ? pBxSp->GetStation() : dHSta;
			if((dHSta - dStaComp) > -1000 && (dHSta - dStaComp) < 0)
				bLeft = TRUE;

			dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dHSta, 0);
			y1	= pGir->GetElevationFlangeCamber(dHSta, 0, CDPoint(0,1), TRUE);			
			bSttSplice = (pBxStt==pmidBx && pmidBx->IsState(BX_SPLICE)) ? TRUE : FALSE;
			Dom.DrawWeldMark(CDPoint(dLen,y1 - UpperTerm - T/2), bLeft ? G_H_L : G_H_R, G_W,pmidBx, -1,6,0,bLeft,FALSE,rotAng,TRUE, "", TRUE, bSttSplice);
		}
		if(pmidBx->IsCompressByJRib(FALSE) && LowerDan > 0)
		{
			BOOL bLeft = FALSE;
			dHSta = (pHStiff->m_dStationStt[bLeft ? 0 : 1][2+LowerDan]+pHStiff->m_dStationEnd[bLeft ? 0 : 1][2+LowerDan])/2;
			pBxSp = pGir->GetBxByStation(dHSta, BX_SPLICE);

			double dStaComp = pBxSp ? pBxSp->GetStation() : dHSta;
			if((dHSta - dStaComp) > -1000 && (dHSta - dStaComp) < 0)
				bLeft = TRUE;
			dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dHSta, 0);			
			y2	= pGir->GetElevationFlangeCamber(dHSta, 0, CDPoint(0,1), TRUE);			
			bSttSplice = (pBxStt==pmidBx && pmidBx->IsState(BX_SPLICE)) ? TRUE : FALSE;
			Dom.DrawWeldMark(CDPoint(dLen,y2 - LowerTerm - T/2),bLeft ? G_H_L : G_H_R, G_W, pmidBx, -1,6,0,bLeft,FALSE,rotAng,TRUE, "", TRUE, bSttSplice);
		}

		if(bWeldMarkOne) break;
		pBx = Finder.GetBxNext();		
	}
	// 상판:복부판 용접 -------------------------------------------------------------------------------------------
	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_UPPERFLANGE, pBxStt, pBxEnd), *pBxNext;
	while(Finder.GetBxFixNext())
	{	
		BOOL bLeft = FALSE;
		pBxNext = Finder.GetBxFixNext();		

		if(pBx == pBxNext) break;		// 같은것 제외
		if(!pBxNext) pBxNext = pBxEnd;	// 끝점 추가

		double Sta = (pBx->GetStation() + pBxNext->GetStation()) /2;
		double PixSta = Sta;
		pBxSp = pGir->GetBxByStation(Sta, BX_SPLICE);

		double dStaComp = pBxSp ? pBxSp->GetStation() : Sta;
		if((Sta - dStaComp) > -600 && (Sta - dStaComp) < 0)
			Sta	-= (600 - fabs(Sta - dStaComp)); 
		else if((Sta - dStaComp) < 600 && (Sta - dStaComp) >= 0)
			Sta	+= (600 - fabs(Sta - dStaComp)); 

/*		double differ = Sta - pGir->GetBxByStation(Sta, BX_UPPERFLANGE)->GetStation();
		if(differ < 0)
		{
			Sta = (pBx->GetStation() + pBxNext->GetStation()) /2;
			if(dStaComp < Sta)	bLeft = FALSE;
			if(dStaComp > Sta)	bLeft = TRUE;
		}
*/
		CPlateBasicIndex *pBx2 = pGir->GetBxByStation(Sta, BX_UPPERFLANGE);
		if(!pBx2) pBx2 = pGir->GetBxByStation(Sta, BX_END_GIRDER);
		double BaseSta = pBx2->GetStation();
		if(BaseSta < PixSta && BaseSta > Sta)
			Sta = Sta + (BaseSta-Sta) + (PixSta-BaseSta)/2;
		else if(BaseSta > PixSta && BaseSta < Sta)
			Sta = Sta + (BaseSta-Sta) + (PixSta-BaseSta)/2;

		if(dStaComp < Sta)	bLeft = FALSE;
		if(dStaComp > Sta)	bLeft = TRUE;

		CPlateBasicIndex *pmidBx = pGir->GetBxByStation(Sta);
		if(fabs(pmidBx->GetStation()-Sta) < 100) Sta += 100;

		CPlateBxFinder Finder2(pGir);
		if(Sta > BaseSta)
		{
			__int64 Flag = BX_END_GIRDER | BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_SPLICE;
			pmidBx = Finder2.GetBxFirst(Flag, pGir->GetBxByStation(Sta, BX_UPPERFLANGE), pBxNext);
		}
		else if(Sta < BaseSta)
		{
			__int64 Flag = BX_STT_GIRDER | BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_SPLICE;
			pmidBx = Finder2.GetBxLast(Flag, pBx,pGir->GetBxByStation(Sta, BX_UPPERFLANGE));
		}

		if(pmidBx)
		{
			double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),Sta, 0);			
			double y1	= pGir->GetElevationFlangeCamber(Sta, 0, CDPoint(0,1), TRUE);		
			if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper()==0) y1 -= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
			bSttSplice = (pBxStt==pmidBx && pmidBx->IsState(BX_SPLICE)) ? TRUE : FALSE;
			Dom.DrawWeldMark(CDPoint(dLen,y1), G_W, G_F_U, pmidBx, -1,6,0,bLeft,FALSE,rotAng, TRUE, "", TRUE, bSttSplice);
		}
		
		if(bWeldMarkOne) break;
		pBx = Finder.GetBxNext();		
	}
	// 상판:하판 용접) -------------------------------------------------------------------------------------------
	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_LOWERFLANGE, pBxStt, pBxEnd), *pBxNext;
	while(Finder.GetBxFixNext())
	{			
		BOOL bLeft = FALSE;
		pBxNext = Finder.GetBxFixNext();		

		if(pBx == pBxNext) break;		// 같은것 제외
		if(!pBxNext) pBxNext = pBxEnd;	// 끝점 추가

		double Sta = (pBx->GetStation() + pBxNext->GetStation()) /2;
		double PixSta = Sta;
		pBxSp = pGir->GetBxByStation(Sta, BX_SPLICE);

		double dStaComp = pBxSp ? pBxSp->GetStation() : Sta;
		if((Sta - dStaComp) > -600 && (Sta - dStaComp) < 0)
			Sta	-= (600 - fabs(Sta - dStaComp)); 
		else if((Sta - dStaComp) < 600 && (Sta - dStaComp) >= 0)
			Sta	+= (600 - fabs(Sta - dStaComp));
/*		double differ = Sta - pGir->GetBxByStation(Sta, BX_LOWERFLANGE)->GetStation();
		if(differ < 0)
		{
			Sta = (pBx->GetStation() + pBxNext->GetStation()) /2;
			if(dStaComp < Sta)	bLeft = FALSE;
			if(dStaComp > Sta)	bLeft = TRUE;
		}
		*/
		CPlateBasicIndex *pBx2 = pGir->GetBxByStation(Sta, BX_LOWERFLANGE);
		if(!pBx2) pBx2 = pGir->GetBxByStation(Sta, BX_END_GIRDER);
		double BaseSta = pBx2->GetStation();
		if(BaseSta < PixSta && BaseSta > Sta)
		{
			Sta = Sta + (BaseSta-Sta) + (PixSta-BaseSta)/2;
		}
		else if(BaseSta > PixSta && BaseSta < Sta)
		{
			Sta = Sta + (BaseSta-Sta) + (PixSta-BaseSta)/2;
		}

		if(dStaComp < Sta)	bLeft = FALSE;
		if(dStaComp > Sta)	bLeft = TRUE;
		CPlateBasicIndex *pmidBx = pGir->GetBxByStation(Sta);
		if(fabs(pmidBx->GetStation()-Sta) < 100) Sta += 100;		

		CPlateBxFinder Finder2(pGir);
		if(Sta > BaseSta)
		{
			__int64 Flag = BX_END_GIRDER | BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_SPLICE;
			pmidBx = pGir->GetBxByStationDir(Sta, 1, Flag);
		}
		else if(Sta < BaseSta)
		{
			__int64 Flag = BX_STT_GIRDER | BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_SPLICE;
			pmidBx = pGir->GetBxByStationDir(Sta, -1, Flag);
		}

		if(pmidBx)
		{
			double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),Sta, 0);
			double y2	= pGir->GetElevationFlangeCamber(Sta, 0, CDPoint(0,1), FALSE);
			if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower()==1) y2 += pBx->GetFactChain(G_F_L)->m_dFactChainThick;
			bSttSplice = (pBxStt==pmidBx && pmidBx->IsState(BX_SPLICE)) ? TRUE : FALSE;
			Dom.DrawWeldMark(CDPoint(dLen,y2), G_W, G_F_L, pmidBx, -1,6,0,bLeft,TRUE,rotAng, TRUE, "", TRUE, bSttSplice);
		}
		
		if(bWeldMarkOne) break;		
		pBx = Finder.GetBxNext();		
	}
	*pDom << Dom;

}

// 공장이음용접
void CAPlateDrawJong::AddWeldJongFlange(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng, long nSide, CHAR &cWeldType, BOOL bGrindUpper, BOOL bGrindLower, BOOL bDetail)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();		
	CPlateGirderApp	*pGir    	= pBxStt->GetGirder();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CLineInfo		*pLine		= pGir->GetLine();	
	CPlateBxFinder	Finder(pGir);
	CAPlateSymbolDom Dom(pDom,m_pDataMng);
	long nHBase = m_pDataMng->GetGlobalOption()->GetDesignHeigtBase();
	double dYGisi = 0; //지시선 Y값...
	BOOL bUpperSide = TRUE; //용접기호 텍스트 미로 여부...True 면 안함...

	CString strText;
	// 상부플랜지 공장이음 용접
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_UPPERFLANGE, pBxStt, pBxEnd);
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;
	CString strSlope;
	strSlope.Format("1 : %.1f",1/slop);

	while(pBx)
	{		
		if(!pBx->GetFactChain(G_F_U,TRUE) || !pBx->GetFactChain(G_F_U,FALSE)) break;
		if(pBx->IsState(BX_SPLICE))
		{
			pBx=Finder.GetBxNext();
			continue;
		}
		double dT1   = pBx->GetFactChain(G_F_U,TRUE)->m_dFactChainThick;
		double dT2   = pBx->GetFactChain(G_F_U,FALSE)->m_dFactChainThick;
		double dSta = pBx->GetStation();
		double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta, 0);
		double dY	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), TRUE);	
		if (bDetail)
		{
			strText.Format("\"%c\"",cWeldType++);
			pOpt->SetEnvType(&Dom,HCAD_SMALL_TITLE);
			Dom.TextOut(dLen,dY + Dom.Always(6), strText);
			pOpt->SetEnvType(&Dom,HCAD_DIML,LT_DASHDOT_LINE);
			Dom.Circle(dLen,dY,Dom.Always(5));
		}
		pOpt->SetEnvType(&Dom,HCAD_DIML);
		BOOL bLeft = FALSE;
		if(dT1 > dT2)	bLeft = TRUE;
		
		dYGisi = dY;
		//거더높이 기준에서 상판의 기준이 내부이면...두께 더해줌...
		if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper()==1)	
		{
  			dY += min(dT1, dT2);
			dYGisi = dY;
		}
		else
		{
			dYGisi -= min(dT1,dT2);
			bUpperSide = FALSE;
		}

		if(dT1 == dT2)	bUpperSide = TRUE;

		Dom.DrawWeldMark(CDPoint(dLen,dY), G_F_U, G_F_U, pBx, -1, 10, 0, bLeft, TRUE, rotAng, FALSE, _T(""), bGrindUpper, FALSE, FALSE, bUpperSide);		
		
		if(dT1 != dT2)
		{
			if(bLeft)
				Dom.GiSiText(dLen,dYGisi,CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==2 ? 1 : -1),strSlope, "", TRUE);		
			else		
				Dom.GiSiText(dLen,dYGisi,ConstPi - CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==2 ? 1 : -1),strSlope, "", FALSE);
		}
		pBx = Finder.GetBxNext();
	}

	// 하부플랜지 공장이음 용접
	bUpperSide = TRUE;
	pBx = Finder.GetBxFirst(BX_LOWERFLANGE,pBxStt,pBxEnd);
	while(pBx)
	{		
		if(!pBx->GetFactChain(G_F_L,TRUE) || !pBx->GetFactChain(G_F_L,FALSE)) break;		
		if(pBx->IsState(BX_SPLICE))
		{
			pBx=Finder.GetBxNext();
			continue;
		}
		double dT1	= pBx->GetFactChain(G_F_L,TRUE)->m_dFactChainThick;
		double dT2	= pBx->GetFactChain(G_F_L,FALSE)->m_dFactChainThick;		
		double dSta	= pBx->GetStation();
		double dLen	= pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta, 0);			
		double dY	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE);		
		if (bDetail)
		{
			strText.Format("\"%c\"",cWeldType++);
			pOpt->SetEnvType(&Dom,HCAD_SMALL_TITLE);
			Dom.TextOut(dLen,dY + Dom.Always(6), strText);
			pOpt->SetEnvType(&Dom,HCAD_DIML,LT_DASHDOT_LINE);
			Dom.Circle(dLen,dY,Dom.Always(5));
		}
		pOpt->SetEnvType(&Dom,HCAD_DIML);
		BOOL bLeft = FALSE;
		if(dT1 > dT2)	bLeft = TRUE;
		
		dYGisi = dY;
		//거더높이 기준에서 하판의 기준이 내부이면...두께 빼줌...
 		if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower()==0)
		{
 			dY -= min(dT1, dT2);				
			dYGisi = dY;
		}
		else
		{
			dYGisi += min(dT1, dT2);
			bUpperSide = FALSE;
		}
		
		if(dT1 == dT2)	bUpperSide = TRUE;

		Dom.DrawWeldMark(CDPoint(dLen,dY), G_F_L, G_F_L, pBx, -1, 10, 0, bLeft, FALSE, rotAng, FALSE, _T(""), bGrindLower, FALSE, FALSE, bUpperSide);

		if(dT1 != dT2)
		{
			if(bLeft)
				Dom.GiSiText(dLen,dYGisi,-CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==3 ? 1 : -1),"",strSlope, TRUE);
			else
				Dom.GiSiText(dLen,dYGisi,ConstPi + CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==3 ? 1 : -1),"", strSlope, FALSE);
		}
		pBx = Finder.GetBxNext();
	}

	// 복부판  공장이음 용접
	pBx = Finder.GetBxFirst(BX_WEB, pBxStt, pBxEnd);
	while(pBx)
	{		
		if(!pBx->GetFactChain(G_W,TRUE) || !pBx->GetFactChain(G_W,FALSE)) break;		
		if(pBx->IsState(BX_SPLICE))
		{
			pBx=Finder.GetBxNext();
			continue;
		}

		double dT1	= pBx->GetFactChain(G_W,TRUE)->m_dFactChainThick;
		double dT2	= pBx->GetFactChain(G_W,FALSE)->m_dFactChainThick;		
		double dSta	= pBx->GetStation();
		double dLen	= pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta, 0);			
		// 복부판의 중간
		double dY	= (pGir->GetElevationFlangeCamber(dSta)+pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE))/2;		
		if (bDetail)
		{
			strText.Format("\"%c\"",cWeldType++);
			pOpt->SetEnvType(&Dom,HCAD_SMALL_TITLE);
			Dom.TextOut(dLen,dY + Dom.Always(6), strText);
			pOpt->SetEnvType(&Dom,HCAD_DIML,LT_DASHDOT_LINE);
			Dom.Circle(dLen,dY,Dom.Always(5));
		}
		pOpt->SetEnvType(&Dom,HCAD_DIML);
		BOOL bLeft = FALSE;
		if(dT1 > dT2)	bLeft = TRUE;
		dY -= min(dT1, dT2);
		Dom.DrawWeldMark(CDPoint(dLen,dY), G_W, G_W, pBx, -1, 6, 0, TRUE, TRUE, rotAng, FALSE, _T(""), TRUE, FALSE, FALSE);

		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}

// 현장용접
void CAPlateDrawJong::AddWeldSplicePos(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng, CHAR &cWeldType, BOOL bGrindUpper, BOOL bGrindLower, BOOL bDetail)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();		
	CPlateGirderApp	*pGir		= pBxStt->GetGirder();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CLineInfo		*pLine		= pGir->GetLine();	
	long nHBase = m_pDataMng->GetGlobalOption()->GetDesignHeigtBase();
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	CAPlateSymbolDom Dom(pDom,m_pDataMng);

	CString strText;
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;
	CString strSlope;
	strSlope.Format("1 : %.1f",1/slop);

	double dCenSta = pBxStt->GetStation() + (pBxEnd->GetStation()-pBxStt->GetStation())/2;

	//복부...
	BOOL bLeft = TRUE;
	long nMarkBuje = G_W;
	long nWeldMarkBuje = WELD_FIELDWEB;	
	while(pBx)
	{		
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)	
		{
			pBx = Finder.GetBxNext();
			continue;
		}

		if(!pBx->GetFactChain(nMarkBuje,TRUE) || !pBx->GetFactChain(nMarkBuje,FALSE)) break;		

		double dT1	= pBx->GetFactChain(nMarkBuje,TRUE)->m_dFactChainThick;
		double dT2	= pBx->GetFactChain(nMarkBuje,FALSE)->m_dFactChainThick;
		double dSta = pBx->GetStation();
		double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta, 0);	
		double dYUp	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), TRUE);				
		double dYLo	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE);		
		double dHGir = dYUp-dYLo;
		double dY	= dYLo + dHGir/2;

		if (bDetail)
		{
			strText.Format("\"%c\"",cWeldType++);
			pOpt->SetEnvType(&Dom,HCAD_SMALL_TITLE);
			Dom.TextOut(dLen,dY + Dom.Always(6), strText);
			pOpt->SetEnvType(&Dom,HCAD_DIML,LT_DASHDOT_LINE);
			Dom.Circle(dLen,dY,Dom.Always(5));
		}

		pOpt->SetEnvType(&Dom,HCAD_DIML);		
				
		if(dSta > dCenSta) bLeft = FALSE;

		dY += min(dT1, dT2);
		long nIdx = pBridge->GetIndexBaseWeld(nWeldMarkBuje,nWeldMarkBuje,pBx);
		Dom.DrawWeldMark(CDPoint(dLen,dY), nMarkBuje, nMarkBuje, pBx, -1, 12, 0, bLeft, TRUE, rotAng, FALSE, _T(""), bGrindUpper,FALSE,TRUE,TRUE,nIdx);
		
		if(dT1 != dT2)
		{
			if(bLeft)
				Dom.GiSiText(dLen,dY,CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==2 ? 1 : -1),strSlope, "", TRUE);		
			else		
				Dom.GiSiText(dLen,dY,ConstPi - CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==2 ? 1 : -1),strSlope, "", FALSE);
		}

		pBx = Finder.GetBxNext();
	}

	// 상부
	pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	bLeft = TRUE;
	nMarkBuje = G_F_U;
	nWeldMarkBuje = WELD_FIELDU;
	while(pBx)
	{		
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)
		{
			pBx = Finder.GetBxNext();	continue;
		}

		if(!pBx->GetFactChain(nMarkBuje, TRUE) || !pBx->GetFactChain(nMarkBuje, FALSE)) break;

		double dT1   = pBx->GetFactChain(G_F_U,TRUE)->m_dFactChainThick;
		double dT2   = pBx->GetFactChain(G_F_U,FALSE)->m_dFactChainThick;
		double dSta		= pBx->GetStation();
		double dStaComp	= pGir->GetBxByStation(dSta, BX_SPLICE)->GetStation();
		double dWidth	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
		double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta, 0);	
		double dYUp	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), TRUE) - pGir->GetThickFlangeByBase(dSta,TRUE);	
		double dY	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), TRUE);

		if (bDetail)
		{
			strText.Format("\"%c\"",cWeldType++);
			pOpt->SetEnvType(&Dom,HCAD_SMALL_TITLE);
			Dom.TextOut(dLen,dY + Dom.Always(6), strText);
			pOpt->SetEnvType(&Dom,HCAD_DIML,LT_DASHDOT_LINE);
			Dom.Circle(dLen,dY,Dom.Always(5));
		}

		pOpt->SetEnvType(&Dom,HCAD_DIML);
				
		if(dSta > dCenSta) bLeft = FALSE;
		//if((dSta - dStaComp) > -1000 && (dSta - dStaComp) < 0)
		//	bLeft = FALSE;
				
		dYUp += min(dT1, dT2);
		long nIdx = pBridge->GetIndexBaseWeld(nWeldMarkBuje,nWeldMarkBuje,pBx);
		Dom.DrawWeldMark(CDPoint(dLen,dYUp), nMarkBuje, nMarkBuje, pBx, -1, 12, 0, bLeft, TRUE, rotAng, FALSE, "", bGrindUpper, FALSE, TRUE,TRUE, nIdx);		

		if(dT1 != dT2)
		{
			if(bLeft)
				Dom.GiSiText(dLen,dY,CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==2 ? 1 : -1),strSlope, "", TRUE);		
			else		
				Dom.GiSiText(dLen,dY,ConstPi - CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==2 ? 1 : -1),strSlope, "", FALSE);
		}
		
		pBx = Finder.GetBxNext();
	}

	pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd); 
	bLeft = TRUE;
	nMarkBuje = G_F_L;
	nWeldMarkBuje = WELD_FIELDL;
	//하부
	while(pBx)
	{		
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)
		{
			pBx = Finder.GetBxNext();	continue;
		}

		if(!pBx->GetFactChain(nMarkBuje, TRUE) || !pBx->GetFactChain(nMarkBuje, FALSE)) break;
		double dT1	= pBx->GetFactChain(G_F_L,TRUE)->m_dFactChainThick;
		double dT2	= pBx->GetFactChain(G_F_L,FALSE)->m_dFactChainThick;
		double dSta		= pBx->GetStation();
		double dStaComp	= pGir->GetBxByStation(dSta, BX_SPLICE)->GetStation();
		double dWidth	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
		double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta, 0);	
		double dYLo	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE) + pGir->GetThickFlangeByBase(dSta,FALSE);		
		double dY	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE);	
		
		if (bDetail)
		{
			strText.Format("\"%c\"",cWeldType++);
			pOpt->SetEnvType(&Dom,HCAD_SMALL_TITLE);
			Dom.TextOut(dLen,dY + Dom.Always(6), strText);
			pOpt->SetEnvType(&Dom,HCAD_DIML,LT_DASHDOT_LINE);
			Dom.Circle(dLen,dY,Dom.Always(5));
		}

		pOpt->SetEnvType(&Dom,HCAD_DIML);
				
		if(dSta > dCenSta) bLeft = FALSE;

		//if((dSta - dStaComp) < 1000 && (dSta - dStaComp) >= 0)
		//	bLeft = TRUE;

		dYLo -= min(dT1, dT2);
		long nIdx = pBridge->GetIndexBaseWeld(nWeldMarkBuje,nWeldMarkBuje,pBx);
		Dom.DrawWeldMark(CDPoint(dLen,dYLo), nMarkBuje, nMarkBuje, pBx, -1, 12, 0, bLeft, FALSE, rotAng, FALSE, "", bGrindLower, FALSE, TRUE, FALSE, nIdx);
		
		if(dT1 != dT2)
		{
			if(bLeft)
				Dom.GiSiText(dLen,dY,-CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==3 ? 1 : -1),"",strSlope, TRUE);
			else
				Dom.GiSiText(dLen,dY,ConstPi + CDPoint(5,1).GetAngleRadian() * (nHBase==0 || nHBase==3 ? 1 : -1),"", strSlope, FALSE);
		}
		
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}

// 가로보복부판과 거더의 복부판의 용접
// 또는 수직브레이싱 위치의 수직보강재와 거더복부판과의 용접 
void CAPlateDrawJong::AddWeldJongCrossBeam(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng)
{
	CPlateGirderApp *pGir		= pBxStt->GetGirder();
	CARoadOptionStd *pOpt		= m_pDataMng->GetOptionStd();
	CLineInfo       *pLine		= pGir->GetLine();	

	CAPlateSymbolDom Dom(pDom,m_pDataMng);			

	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUMSTIFF|BX_JACKUP|BX_CROSSBEAM_VBRACING|BX_SPLICE ,pBxStt, pBxEnd), *pBxSp=NULL;	
	
	CString strText;
	BOOL bDrawSp = TRUE;
	BOOL bDrawCB = TRUE;
	BOOL bDrawJIJUMV = TRUE;
	BOOL bDrawJackUp = TRUE;
	while(pBx)
	{	
		if(pBx->IsState(BX_SPLICE)) bDrawSp = TRUE;
		if(pBx->IsState(BX_CROSSBEAM_VBRACING) && bDrawCB)
		{
			double dSta = pBx->GetStation();
			pBxSp = pGir->GetBxByStation(dSta, BX_SPLICE);
			double dStaComp = pBxSp ? pBxSp->GetStation() : dSta+1001;
			if(fabs(dSta - dStaComp) < 1000)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			double dHGir = pGir->GetHeightGirderBySta(dSta);
			double dY = pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE) + dHGir/4;
			pOpt->SetEnvType(&Dom, HCAD_DIML);	
			if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			{
				CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
				double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(), dSta)-pC->m_T3_CR/2;			
				if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)	// H형강
					Dom.DrawWeldMark(CDPoint(dLen,dY), V_ST, G_W, pBx, -1, 6, 0, TRUE, TRUE, rotAng);
				else	// 제작타입
				{
					if(pBx->IsJijum())
						Dom.DrawWeldMark(CDPoint(dLen,dY), CP_BR_W, G_W, pBx, -1, 6, 0, TRUE, TRUE, rotAng);											
					else
						Dom.DrawWeldMark(CDPoint(dLen,dY), CG_BR_W, G_W, pBx, -1, 6, 0, TRUE, TRUE, rotAng);											
				}
			}
			else if(pBx->GetSection())	// 수직브레이싱
			{
				CVStiff *pVstiff = pBx->GetSection()->GetVBracing()->m_pVStiff;
				double dT   = max(pVstiff->m_dThick[0], pVstiff->m_dThick[1]);
				double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(), dSta)-dT/2;
				Dom.DrawWeldMark(CDPoint(dLen,dY), V_ST, G_W, pBx, -1, 6, 0, FALSE, TRUE, rotAng);
			}
			bDrawCB = FALSE;
		}
		else if(pBx->IsState(BX_JIJUMSTIFF) && bDrawJIJUMV)
		{
			double dSta = pBx->GetStation();
			pBxSp = pGir->GetBxByStation(dSta, BX_SPLICE);
			double dStaComp = pBxSp ? pBxSp->GetStation() : dSta+1001;
			if(fabs(dSta - dStaComp) < 1000)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			double dHGir = pGir->GetHeightGirderBySta(dSta);
			double dY = pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE) + dHGir/5;
			pOpt->SetEnvType(&Dom, HCAD_DIML);	
			CVStiff *pVstiff = pBx->GetJijumStiff();
			double dT   = max(pVstiff->m_dThick[0], pVstiff->m_dThick[1]);
			double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(), dSta)-dT/2;
			Dom.DrawWeldMark(CDPoint(dLen,dY), V_J_ST, G_W, pBx, -1, 6, 0, TRUE, TRUE, rotAng);
			bDrawJIJUMV = FALSE;
		}
		else if(pBx->IsState(BX_JACKUP) && bDrawJackUp)
		{
			double dSta = pBx->GetStation();
			pBxSp = pGir->GetBxByStation(dSta, BX_SPLICE);
			double dStaComp = pBxSp ? pBxSp->GetStation() : dSta+1001;
			if(fabs(dSta - dStaComp) < 1000)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			CJackupStiff *pJackUp = pBx->GetJackupStiff();
			double dHGir = pGir->GetHeightGirderBySta(dSta);
			double dCurHeightJackUp = pJackUp->m_dVL == 0 ? dHGir : pJackUp->m_dVL;
			double dY = pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE) + dCurHeightJackUp/3;
			pOpt->SetEnvType(&Dom, HCAD_DIML);				
			double dT   = max(pJackUp->m_vJewon[0].y, pJackUp->m_vJewon[1].y);
			double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(), dSta)-dT/2;
			Dom.DrawWeldMark(CDPoint(dLen,dY), GI_JA_L, G_W, pBx, -1, 6, 0, FALSE, TRUE, rotAng);
			bDrawJackUp = FALSE;
		}
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::DrawStudBx(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();

	double length, dSta = pGir->GetSSS(OFF_STT_GIRDER);		

	double HeadSide	= 5;	// Head는 Body(CD)보다 10넓다.(양쪽으로 5씩)	
	double CD = pGir->m_pSangse->m_StudHori_D;
	double CE = pGir->m_pSangse->m_StudHori_E;
	double CH = pGir->m_pSangse->m_StudHori_H;
	double SA = pGir->m_pSangse->m_SlabAnchor_A;
	double SB = pGir->m_pSangse->m_SlabAnchor_B;
	double ST = pGir->m_pSangse->m_SlabAnchor_T;

	double dStaStt = pBxStt->GetStation();
	double dStaEnd = pBxEnd->GetStation();
	double dStaPre = dStaStt;

	CDomyun Dom(pDomP);

	for (long i = 0; i < STUD_JONG_SU; i++)
	{
		long n = pGir->m_pSangse->m_StudJong_Studsu[i];		
		length = pGir->m_pSangse->m_StudJong_StudDis[i] * n;
		if (length < 1)
			break;
		dSta += length;
		if(dStaStt <= dSta && dSta <= dStaEnd)
		{
			if (pGir->m_pSangse->m_StudJong_Studsu[i+1] == 0)
				break;					

			CPlateBasicIndex *pBx = pGir->GetBxByStation(dSta);
			if(!pBx) break;

			double dThick = 0;
			if(dSta < pBx->GetStation())
				dThick = pBx->GetFactChain(G_F_U,FALSE)->m_dFactChainThick;
			else
				dThick = pBx->GetFactChain(G_F_U,TRUE)->m_dFactChainThick;
			
			if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper()==0) dThick = 0;
			double dY = pGir->GetElevationFlangeCamber(dSta,0) + dThick;

//			CPlateSpliceBase *pSp = pBx->GetSplice();
//			if (pSp)
//			{
//				double dSpSta = pBx->GetStation();
//				double dSpW	  = pBx->GetSplice()->GetWidByBuje(SP_U_U);
//				if(dSta > dSpSta-dSpW/2 && dSta < dSpSta+dSpW/2)
//					dY += pBx->GetSplice()->GetThkByBuje(SP_U_U);
//			}
			
			double dRealSta = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta,0);
			double dMiddleSta	= dRealSta-(dRealSta-dStaPre)/2;

			if(pGir->m_pSangse->m_StudHori_Type==0)				// 슬래브앵커
			{
				Dom.AnchorBarFront(dRealSta, dY, ST, 0, SA, SB);
//				if(n==2)
//					Dom.AnchorBarFront(dMiddleSta, dY, ST, 0, SA, SB);
			}
			else if(pGir->m_pSangse->m_StudHori_Type==1)		// 스터드
			{
				Dom.LineTo(dRealSta-CD/2, dY,	dRealSta-CD/2, dY+CH);
				Dom.LineTo(dRealSta+CD/2, dY,	dRealSta+CD/2, dY+CH);
				Dom.Rectangle(dRealSta-CD/2-HeadSide, dY+CH, dRealSta+CD/2+HeadSide, dY+CH+CE);
//				if(n==2)
//				{
//					Dom.LineTo(dMiddleSta-CD/2, dY,	dMiddleSta-CD/2, dY+CH);
//					Dom.LineTo(dMiddleSta+CD/2, dY,	dMiddleSta+CD/2, dY+CH);
//					Dom.Rectangle(dMiddleSta-CD/2-HeadSide, dY+CH, dMiddleSta+CD/2+HeadSide, dY+CH+CE);
//				}
			}
			else if(pGir->m_pSangse->m_StudHori_Type==2)		// 각형(TU)
			{
				double SW = pGir->m_pSangse->m_StudHoriSQ_W;
				double SH = pGir->m_pSangse->m_StudHoriSQ_H;
				Dom.Rectangle(dRealSta-SW/2, dY+SH, dRealSta+SW/2, dY);
			}
			dStaPre = dRealSta;

//			Dom.LineTo(dRealSta-CD/2, dY,	dRealSta-CD/2, dY+CH);
//			Dom.LineTo(dRealSta+CD/2, dY,	dRealSta+CD/2, dY+CH);
//			Dom.Rectangle(dRealSta-CD/2-HeadSide, dY+CH, dRealSta+CD/2+HeadSide, dY+CH+CE);
		}		
	}
	*pDomP << Dom;

	if(!pGir->m_pSangse->m_StudJong_SpliceCenter) return;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE,pBxStt,pBxEnd);
	while(pBx)
	{
		double dThick = pGir->GetThickJewon(G_F_U,pBx);
		if(m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper()==0) dThick = 0;
		double dSta   = pBx->GetStation();
		double dY = pGir->GetElevationFlangeCamber(dSta) + dThick;							
		double dRealSta = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(),dSta);
		Dom.LineTo(dRealSta-CD/2, dY,	dRealSta-CD/2, dY+CH);
		Dom.LineTo(dRealSta+CD/2, dY,	dRealSta+CD/2, dY+CH);
		Dom.Rectangle(dRealSta-CD/2-HeadSide, dY+CH, dRealSta+CD/2+HeadSide, dY+CH+CE);

		pBx = Finder.GetBxNext();
	}
	*pDomP << Dom;

}

// pBxStt : 시작BX영역
// pBxEnd : 종료BX영역
// bText  : 공장이음마크(복부판)
// bMark  : 용접시 사용되는 마크
// bSymType : 용접시 사용되는 마크 종류
void CAPlateDrawJong::DrawFactJongBx(CDomyun *pDomP,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									 BOOL bText, BOOL bMark, BOOL bSymType)
{
 	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_WEB, pBxStt, pBxEnd);

	double dStdSta = pBxStt->GetStation();
	double dCurSta = 0;
	double sY1,sY2,sX;

	CDomyun Dom(pDomP);

	while(pBx) 
	{
		if(!pBx->IsState(BX_SPLICE))
		{
			dCurSta = pBx->GetStation();
			sX  = dStdSta + pGir->GetLengthDimType(dStdSta,dCurSta);
			sY1 = pGir->GetElevationFlangeCamber(dCurSta, 0, pBx->GetAngle());
			sY2 = pGir->GetElevationFlangeCamber(dCurSta, 0, pBx->GetAngle(),FALSE);

			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.MoveTo(sX,sY1);		
			Dom.LineTo(sX,sY2);

			CSymbolDom SymDom(pDomP,pOptStd);	
			if(bText)				
			{
				pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
				SymDom.TextCircle(sX, sY2-Dom.Always(12), "WF",0,TRUE,TRUE,0);		
			
				*pDomP << SymDom;
			}

			if(bMark) 
			{				
				if(bSymType)
					SymDom.DrawSymbolCuttingMark(CDPoint(sX, sY1 + Dom.Always(20)),Dom.Always(2.5), 0);									
				else
					SymDom.DrawSymbolCuttingS(sX, (sY1 + sY2) /2, 0);												
			}
		}
		//
		pBx = Finder.GetBxNext();
	}
	*pDomP << Dom;	
}

void CAPlateDrawJong::DrawHGuessetBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(!m_pDataMng->GetBridge()->IsPlateGir())		// 판형교이 아닐 경우
		return;
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	__int64 BxFlag = BX_HBRACINGGUSSET;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	CDomyun Dom(pDom);	
	while(pBx)
	{
		DrawHGuessetOnBx(&Dom, pBx, pBxStt->GetStation());		
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::DrawHGuessetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt)
{
	CPlateGirderApp *pGir	= pBx->GetGirder();
	CHBracingGusset *pBrGu	= pBx->GetHBracingGusset();

	if(!pBrGu->IsReal())	return;

	CDomyun Dom(pDom);

	double dSta = pBx->GetStation();	// 수평브레이싱 이음판의 중심위치 스테이션
	CDPoint xyCen =CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation()),
		pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE) + pBx->GetOffsetFromLowerFlange());
	CDPoint xyDirLowerFL = CDPoint(1, pGir->GetGiulFlangeUpperLower(dSta, 0, FALSE)).Unit();
	CDPoint vX(1,0), vY(0,1);

	if(pBrGu->m_dWidth[0]>0 || pBrGu->m_dWidth[2])
	{
		CSection *pSec = pBx->GetSection();
		if(pSec==NULL)  //BX_HBRACINGGUSSET의 스테이션이 SECTION가 불일치하는 경우도 있음. 미세한 차이이기 때문에 무시가능.
			pSec = pGir->GetBxByStation(pBx->GetStation(), BX_CROSSBEAM_VBRACING | BX_VSTIFF)->GetSection();
		CVStiff *pVStiff = pSec->GetVStiff();
		double dVStiffThick = 0;
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{
			pVStiff = pSec->GetCrossBeam()->m_pVStiff;
			if(max(pVStiff->m_dThick[0], pVStiff->m_dThick[1])==0)
				dVStiffThick = pSec->GetCrossBeam()->m_T3_CR;
		}
		else if(pSec->IsenDetType(SECDET_VBRACING))
		{
			pVStiff = pSec->GetVBracing()->m_pVStiff;
			dVStiffThick = (pVStiff) ? max(pVStiff->m_dThick[0], pVStiff->m_dThick[1]) : 0;
		}

		double dSta = pBx->GetStation();	// 수평브레이싱 이음판의 중심위치 스테이션
		double dL   = max(pBrGu->m_dWidth[0], pBrGu->m_dWidth[2]);		
		CDPoint vX(1,0), vY(0,1);
		CDPoint xyCen =CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation()), pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE) + pBrGu->m_ddH);
		CDPoint xyDirLowerFL = CDPoint(1, pGir->GetGiulFlangeUpperLower(dSta, 0, FALSE)).Unit();
		CDPoint xy1 = xyCen;
		CDPoint xy2 = xyCen-xyDirLowerFL*dL;
		CDPoint xy3 = xyCen-xyDirLowerFL*dL-xyDirLowerFL.Rotate90()*(pBrGu->m_dT);
		CDPoint xy4 = xyCen-vY*(pBrGu->m_dT/fabs(xyDirLowerFL.x));

		Dom.LineTo(xy1, xy2);
		Dom.LineTo(xy2, xy3);
		Dom.LineTo(xy3, xy4);
		
		*pDom << Dom;
	}

	if(pBrGu->m_dWidth[1] > 0 || pBrGu->m_dWidth[3] > 0)		// 기준스테이션 우측
	{
		CSection *pSec = pBx->GetSection();
		if(pSec==NULL)  //BX_HBRACINGGUSSET의 스테이션이 SECTION가 불일치하는 경우도 있음. 미세한 차이이기 때문에 무시가능.
			pSec = pGir->GetBxByStation(pBx->GetStation(), BX_CROSSBEAM_VBRACING | BX_VSTIFF)->GetSection();
		CVStiff *pVStiff = pSec->GetVStiff();
		double dVStiffThick = 0;
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{
			pVStiff = pSec->GetCrossBeam()->m_pVStiff;
			if(max(pVStiff->m_dThick[0], pVStiff->m_dThick[1])==0)
				dVStiffThick = pSec->GetCrossBeam()->m_T3_CR;
		}
		else if(pSec->IsenDetType(SECDET_VBRACING))
		{
			pVStiff = pSec->GetVBracing()->m_pVStiff;
			dVStiffThick = (pVStiff) ? max(pVStiff->m_dThick[0], pVStiff->m_dThick[1]) : 0;
		}

		double dSta = pBx->GetStation();	// 수평브레이싱 이음판의 중심위치 스테이션
		double dL   = max(pBrGu->m_dWidth[1], pBrGu->m_dWidth[3]);	// 큰것
		CDPoint vX(1,0), vY(0,1);
		CDPoint xyCen =CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation()), pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE) + pBrGu->m_ddH);
		CDPoint xyDirLowerFL = CDPoint(1, pGir->GetGiulFlangeUpperLower(dSta, 0, FALSE)).Unit();
		CDPoint xy1 = xyCen;
		CDPoint xy2 = xyCen+xyDirLowerFL*dL;
		CDPoint xy3 = xyCen-xyDirLowerFL.Rotate90()*(pBrGu->m_dT)+xyDirLowerFL*dL;
		CDPoint xy4 = xyCen-vY*(pBrGu->m_dT/fabs(xyDirLowerFL.x));

		Dom.LineTo(xy1, xy2);
		Dom.LineTo(xy2, xy3);
		Dom.LineTo(xy3, xy4);

		*pDom << Dom;
	}
}

void CAPlateDrawJong::DrawCrossBeamBrkFlangeBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);

	CDomyun Dom(pDom);	
	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			DrawCrossBeamBrkFlangeOnBx(&Dom, pBx, pBxStt->GetStation());		
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::DrawCrossBeamBrkFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CPlateCrossBeam *pC      = pBx->GetSection()->GetCrossBeam();
	CHBracingGusset *pBrGu	 = pBx->GetHBracingGusset();
	CPlateCrossBeam *pCPrev  = NULL;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	long nG = pBx->GetNumberGirder();
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyCen; 
	CDPoint xy1, xy2, xy3, xy4;
	double dwFlangeUp = 0;
	double dwFlangeDn = 0;
	double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	CDomyun Dom(pDom);	

	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	if(nG==0)	// 시작거더
	{
		if(pC->m_nLGirSpType && (!pBrGu || !pBrGu->IsReal(1)))
		{
			dwFlangeUp = pC->m_uM_CR+pC->m_uN_CR;
			dwFlangeDn = pC->m_dM_CR+pC->m_dN_CR;
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pC->m_dLH1 - pC->m_T2_CR/2);
			// 상부 브라켓 플랜지
			xy1 = xyCen-vX*dwFlangeUp+vY*(pC->m_T2_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pC->m_T2_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pC->m_T2_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pC->m_T2_CR/2);

			if(pC->m_dLH1>0)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
			// 하부 브라켓 플랜지
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pC->m_dLH1 - pC->m_T2_CR - pC->m_H_CR - pC->m_T1_CR/2);
			xy1 = xyCen-vX*dwFlangeUp+vY*(pC->m_T1_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pC->m_T1_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pC->m_T1_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pC->m_T1_CR/2);
			if(pC->m_H_CR < dHGir)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
		}
	}
	else if(nG==pDB->GetGirdersu()-1)	// 마지막거더
	{
		pCPrev = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetSection()->GetCrossBeam();

		if(pCPrev && pCPrev->m_nRGirSpType && (!pBrGu || !pBrGu->IsReal(0)))
		{
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pCPrev->m_dRH1 - pCPrev->m_T2_CR/2);
			dwFlangeUp = pCPrev->m_uM_CR+pCPrev->m_uN_CR;
			dwFlangeDn = pCPrev->m_dM_CR+pCPrev->m_dN_CR;
			
			xy1 = xyCen-vX*dwFlangeUp+vY*(pCPrev->m_T2_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pCPrev->m_T2_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pCPrev->m_T2_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pCPrev->m_T2_CR/2);
			if(pCPrev->m_dRH1>0)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
			// 하부 브라켓 플랜지
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pCPrev->m_dRH1 - pCPrev->m_T2_CR - pCPrev->m_H_CR - pCPrev->m_T1_CR/2);
			xy1 = xyCen-vX*dwFlangeUp+vY*(pCPrev->m_T1_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pCPrev->m_T1_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pCPrev->m_T1_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pCPrev->m_T1_CR/2);
			if(pCPrev->m_H_CR < dHGir)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
		}
	}
	else if(!pBrGu || !pBrGu->IsReal())	/// 내부거더
	{
		pCPrev = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetSection()->GetCrossBeam();

		if(pCPrev	&& ((pCPrev->m_nRGirSpType==TRUE	&& pC->m_nLGirSpType==TRUE)
					||  (pCPrev->m_nRGirSpType==FALSE	&& pC->m_nLGirSpType==TRUE)))	// pC꺼 그림.
		{
			dwFlangeUp = pC->m_uM_CR+pC->m_uN_CR;
			dwFlangeDn = pC->m_dM_CR+pC->m_dN_CR;
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pC->m_dLH1 - pC->m_T2_CR/2);
			// 상부 브라켓 플랜지
			xy1 = xyCen-vX*dwFlangeUp+vY*(pC->m_T2_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pC->m_T2_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pC->m_T2_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pC->m_T2_CR/2);
			
			if(pC->m_dLH1 > 0)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
			// 하부 브라켓 플랜지
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pC->m_dLH1 - pC->m_T2_CR - pC->m_H_CR - pC->m_T1_CR/2);
			xy1 = xyCen-vX*dwFlangeUp+vY*(pC->m_T1_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pC->m_T1_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pC->m_T1_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pC->m_T1_CR/2);
			if(pC->m_H_CR < dHGir)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
		}
		else if(pCPrev && pCPrev->m_nRGirSpType==TRUE && pC->m_nLGirSpType==FALSE)	// pCPrev꺼 그림.
		{
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pCPrev->m_dRH1 - pCPrev->m_T2_CR/2);
			dwFlangeUp = pCPrev->m_uM_CR+pCPrev->m_uN_CR;
			dwFlangeDn = pCPrev->m_dM_CR+pCPrev->m_dN_CR;
			
			xy1 = xyCen-vX*dwFlangeUp+vY*(pCPrev->m_T2_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pCPrev->m_T2_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pCPrev->m_T2_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pCPrev->m_T2_CR/2);
			if(pCPrev->m_dRH1>0)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
			// 하부 브라켓 플랜지
			xyCen = CDPoint(dStaStt+pGir->GetLengthDimType(dStaStt, pBx->GetStation())
				          , pGir->GetElevationFlangeCamber(pBx->GetStation()) - pCPrev->m_dRH1 - pCPrev->m_T2_CR - pCPrev->m_H_CR - pCPrev->m_T1_CR/2);
			xy1 = xyCen-vX*dwFlangeUp+vY*(pCPrev->m_T1_CR/2);
			xy2 = xyCen+vX*dwFlangeUp+vY*(pCPrev->m_T1_CR/2);
			xy3 = xyCen+vX*dwFlangeUp-vY*(pCPrev->m_T1_CR/2);
			xy4 = xyCen-vX*dwFlangeUp-vY*(pCPrev->m_T1_CR/2);

			if(pCPrev->m_H_CR < dHGir)
			{
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy2, xy3);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy4, xy1);
			}
		}
	}
	*pDom << Dom;

}

void CAPlateDrawJong::DrawJongGirderTotal(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();

	CDomyun Dom(pDom);	
	Dom.SetScaleDim(75);

	long nCount = 0;
	m_SideSttY = 0;

	SetCenterAngle();
	SideCenter(&Dom);
	for(long nG=-1; nG<pDB->GetGirdersu(); nG++)
	{
		if(!DrawGirderNum(nG)) continue;
		if(pDB->m_cByunDan==0)	// 일반단면
			DrawSideAll(&Dom, nG);
		else					// 변단면일경우
			DrawByunSide(&Dom, nG);

		CrossBeamSide(&Dom, nG);		
		TextCrossBeamHeight(&Dom, nG);
		VStiffSide(&Dom, nG);
		TextVStiffHeight(&Dom, nG);
		GirderNumberingSide(&Dom, nG);
		SpliceSide(&Dom, nG);
		FlangeSide(&Dom, nG, TRUE);
		FlangeSide(&Dom, nG, FALSE);
		LeftFactSide(&Dom, nG);
		AbutPierSymbol(&Dom, nG);				
		DimSideLeftWidth(nG, &Dom);
		DimSideRightWidth(nG, &Dom);

		m_SideSttY += GetSideSttY(nG, nCount);
		nCount++;
	}
	m_SW = 0;
	
	*pDom << Dom;
}


void CAPlateDrawJong::DrawJongGirderTotalHStiff(CDomyun *pDom) 
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(75);

	long   cnt = 0;
	m_SideSttY = 0;
	long nLine = 2;
	long nJewon= 5;
	
	for(long nG = 0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

		pGir->SetEleLevel(TRUE);
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		
		DrawGirderLineBx (&Dom, pBxStt, pBxEnd, nLine);
		DrawInOutGateBx  (&Dom, pBxStt, pBxEnd, TRUE);
		DrawFieldSpliceHatchJong(&Dom, pBxStt, pBxEnd, 0, FALSE, TRUE);
		DrawVStiffBx     (&Dom, pBxStt, pBxEnd, nLine, nJewon, TRUE);
		DrawJiJumVStiffBx(&Dom, pBxStt, pBxEnd, nLine, nJewon, TRUE);
		DrawJackUpBx	 (&Dom, pBxStt, pBxEnd, nLine, nJewon, TRUE);
		DrawHStiffBx	 (&Dom, pBxStt, pBxEnd, nLine, nJewon);
		DrawShoeMarkBx	 (&Dom, pBxStt, pBxEnd, 75,  TRUE);	
		DrawCrossBeamBrkFlangeBx (&Dom, pBxStt, pBxEnd);
		DrawHGuessetBx(&Dom, pBxStt, pBxEnd);

		//DrawAlternateSection(&Dom, g, pBxStt, pBxEnd);//교번구간 그리기....
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		AddSttDimJongBx(&Dom,pBxStt);
		AddEndDimJongBx(&Dom,pBxStt,pBxEnd);
		pGir->SetEleLevel(FALSE);

		Dom.Move(0,-GetSideSttY(nG,cnt));
		m_SideSttY += GetSideSttY(nG,cnt);		
		cnt++;
	}
	m_SW = 0;

	*pDom << Dom;
}


// 측면도에 삼각형 그리기
void CAPlateDrawJong::AbutPierSymbol(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_SUB_TITLE);

	double	height1 = 0,height = 0,	sta1 =0;
	CString str;
	height = -(pDB->m_dThickSlabBase+pDB->m_dThickPave)+m_SideSttY;

	for(long k=0; k<= pDB->m_nQtyJigan; k++)
	{
		sta1	= GetLengthSide(pGir->GetStationOnJijum(k), 0, nG);
		str		= pDB->m_strJijumName[k];
		height1 = pGir->GetHeightGirderByJijum(k);
		Dom.Triangle(sta1,height - height1,7);
		Dom.TextCircle(sta1,height-height1-1600,str,4);
	}
	*pDom << Dom;
}

void CAPlateDrawJong::DrawAlternateSection(CDomyun *pDom, long nG, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bPlan)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);	
	CARoadOptionStd	*pOptStd	= m_pDataMng->GetOptionStd();
	CCalcData		*pData		= m_pDataMng->GetCalcData();
	if(!pData->m_bReadFrameModelingOutput) return;//모델링 값이 없으면 리턴...

	double Scale = 75;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	double BaseY = 0.0;
	double staStt = pBxStt->GetStation();
	double staEnd = pBxEnd->GetStation();
	CString cs, cs2;

	double YHeight = 1;   // 거더 높이 확대비율
	
	if (bPlan)//평면도에서 디버그용으로표기하기 위해...
	{
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER);
		CDPoint pt1 = pGir->GetXyGirderDis(pBx->GetStation());//,0,pBx->GetAngleSection());	
		BaseY = pt1.y;
	}
		
	// 교번 구간 표시
	Dom.SetTextAlignHorz(TA_CENTER);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);

	InsertionSort(pData->m_ArrMomentZeroSta);
	for (long nIdx = 0; nIdx < pData->m_ArrMomentZeroSta.GetSize() - 1; nIdx += 2)   
	{
		Dom.SetLineColor(4);
		if (pData->m_ArrMomentZeroSta[nIdx] >= staStt && pData->m_ArrMomentZeroSta[nIdx] <= staEnd)   {
			Dom.MoveTo(pData->m_ArrMomentZeroSta[nIdx], BaseY + 12 * Scale);
			Dom.LineTo(pData->m_ArrMomentZeroSta[nIdx], BaseY - (pGir->GetHeightMaxByJijum() * YHeight + 12 * Scale));
		}
		if (pData->m_ArrMomentZeroSta[nIdx + 1] >= staStt && pData->m_ArrMomentZeroSta[nIdx + 1] <= staEnd)   {
			Dom.MoveTo(pData->m_ArrMomentZeroSta[nIdx + 1], BaseY + 12 * Scale);
			Dom.LineTo(pData->m_ArrMomentZeroSta[nIdx + 1], BaseY - (pGir->GetHeightMaxByJijum() * YHeight + 12 * Scale));
		}
		double dSttTerm = max(staStt, pData->m_ArrMomentZeroSta[nIdx]);
		double dEndTerm = min(staEnd, pData->m_ArrMomentZeroSta[nIdx + 1]);
		if (dSttTerm < dEndTerm)   {
			CDPoint P1((dSttTerm + dEndTerm) * 0.5, BaseY + 14 * Scale);
			Dom.SetTextColor(3);
			Dom.TextOut (P1, "- 교번 구간 -");
		}
	}

	*pDom << Dom;
}

void CAPlateDrawJong::InsertionSort(CDWordArray &Data)
{
	// 추가 정렬
	int i, j, nCount = Data.GetSize();
	for (i = 1; i < nCount; i++)   {
		double temp = Data [i];
		j = i;
		while (j > 0 && Data [j - 1] > temp)   {
			Data.SetAt (j, Data [j - 1]);
			j--;
		}
		Data.SetAt (j, (DWORD)temp);
	}
}

void CAPlateDrawJong::CrossBeamSide(CDomyun *pDom, long nG)	// 측면도에 가로보 그리기
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double	sdiap_u =0, sdiap_l = 0,
			sty	   = pDB->m_dThickSlabBase+pDB->m_dThickPave,
			sta1 = 0, cursta = 0;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		cursta = pBx->GetStation();
		sdiap_u = sty + pGir->GetHeightFlangeUpperByTaper(cursta);
		sdiap_l = sty + pGir->GetHeightFlangeLower(cursta,0,TRUE);
		sta1 = GetLengthSide(cursta,0,nG);
		Dom.LineTo(sta1,-sdiap_u+m_SideSttY,sta1,-sdiap_l+m_SideSttY);
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::TextCrossBeamHeight(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetTextAngle(90);

	double sta = 0;
	CString str;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		sta = pBx->GetStation();
		str.Format("%.0lf",pGir->GetHeightGirderByStaAng(sta));
		sta = GetLengthSide(sta,0,nG);
		Dom.TextOut(sta+300,-pGir->GetHeightGirderByJijum(0,FALSE)/2+m_SideSttY, str);
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			str = "CROSSBEAM";
		else if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
			str = "VBRACING";
		Dom.TextOut(sta-100,-pGir->GetHeightGirderByJijum(0,FALSE)/2+m_SideSttY, str);

		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

// 측면도 화면에 수직보강재그리기
void CAPlateDrawJong::VStiffSide(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetTextAngle(90);

	double	sty	   = pDB->m_dThickSlabBase+pDB->m_dThickPave,
			vstf_u = 0, vstf_l = 0, sta = 0, cursta = 0;
		
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);
	while(pBx)
	{
		cursta = pBx->GetStation();
		vstf_u = sty+pGir->GetHeightFlangeUpperByTaper(cursta);
		vstf_l = sty+pGir->GetHeightFlangeLower(cursta,0,TRUE);
		sta = GetLengthSide(cursta,0,nG);
		Dom.LineTo(sta,-vstf_u+m_SideSttY,sta,-vstf_l+m_SideSttY);
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::TextVStiffHeight(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextAngle(90);

	double sta = 0;
	CString str;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);
	while(pBx)
	{
		sta = pBx->GetStation();
		str.Format("%.0lf",pGir->GetHeightGirderByStaAng(sta));
		if(atof(str) <= 0) break;
		sta = GetLengthSide(sta,0,nG);
		Dom.TextOut(sta+300,-pGir->GetHeightGirderByJijum(0,FALSE)/2+m_SideSttY,str);
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::GirderNumberingSide(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_MIS2);
	
	double sta = 0;
	CString str;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_VSTIFF);
	while(pBx)
	{
		sta = GetLengthSide(pBx->GetStation(),0,nG);
		str = pBx->GetMarkString();

		if(pBx->IsJijum())					pOptStd->SetEnvType(&Dom, HCAD_DIML);
		else if(pBx->IsState(BX_CROSSBEAM_VBRACING))	pOptStd->SetEnvType(&Dom, HCAD_DIML);
		else								pOptStd->SetEnvType(&Dom, HCAD_MIS2);

		if(str!="")
			Dom.TextCircle(sta, m_SideSttY+100, str, -1, TRUE);

		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::FlangeSide(CDomyun *pDom, long nG, BOOL bUpper)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double	sta1 = 0,sta2 = 0,
			sp_y    = -(pDB->m_dThickSlabBase + pDB->m_dThickPave)+m_SideSttY,
			total   = 0, spwidth = 0, height = 0;
	CString str;
	long nSp = 0;
		
	if(bUpper)	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	else		pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE);
	while(pBx)
	{
		total   = pBx->GetStation();
		spwidth = 200;	
		height  = pGir->GetHeightGirderByStaAng(total);
		sta1    = GetLengthSide(total+spwidth/2,0,nG);
		sta2    = GetLengthSide(total-spwidth/2,0,nG);
		if(bUpper)					
		{
			Dom.SetLineColor(3);	// 연두 
			Dom.Solid(sta1,sp_y+160,sta1,sp_y-160,sta2,sp_y-160,sta2,sp_y+160);				
		}
		else		
		{
			Dom.SetLineColor(5);	// 파랑
			Dom.Solid(sta1,sp_y-height+160,sta1,sp_y-height-160,sta2,sp_y-height-160,sta2,sp_y-height+160);		
		}
		
		nSp++;

		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void CAPlateDrawJong::SpliceSide(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double	sta1    = 0, sta2 = 0,
			sp_y    = -(pDB->m_dThickSlabBase+pDB->m_dThickPave)+m_SideSttY,
			total   = 0, spwidth = 0, height = 0,
			height1 = GetMaxHeight(nG);
	CString str;
	long nSp = 0;
		
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	while(pBx)
	{
		total   = pBx->GetStation();
		spwidth = 505;	
		height  = pGir->GetHeightGirderByStaAng(total);
		sta1    = GetLengthSide(total+spwidth/2,0,nG);
		sta2    = GetLengthSide(total-spwidth/2,0,nG);
		if(pBx->GetSplice()->m_bSpliceType)
		{
			Dom.Solid(sta1,sp_y-40,sta1,sp_y-335,sta2,sp_y-335,sta2,sp_y-40);
			Dom.Solid(sta1,sp_y-height+335,sta1,sp_y-height+40,sta2,sp_y-height+40,sta2,sp_y-height+335);
			sta1 = GetLengthSide(total+spwidth/3,0,nG);
			sta2 = GetLengthSide(total-spwidth/3,0,nG);
			Dom.Solid(sta1,sp_y-360,sta1,sp_y-height+360,sta2,sp_y-height+360,sta2,sp_y-360);
		}
		else
		{
			sta1 = GetLengthSide(total,0,nG);
			Dom.MoveTo(sta1, sp_y);
			Dom.LineTo(sta1, sp_y-height);
		}
//		str.Format("SP%d", nSp+1+pDB->GetNumberStartSplice());
		sta1 = GetLengthSide(total,0,nG);
		Dom.TextCircle(sta1, sp_y-height1-700, pBx->GetSplice()->GetMarkstring(), 0, TRUE, TRUE, 0.0);
		
		nSp++;

		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

void  CAPlateDrawJong::LeftFactSide(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double	sty	   = pDB->m_dThickSlabBase+pDB->m_dThickPave,
			vstf_y = 0,cursta = 0,height = 0,sta =0;

	Dom.SetPLineThick(50);
	Dom.SetLineColor(2);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_WEB);
	while(pBx)
	{
		if (!pBx->IsState(BX_SPLICE))
		{
			cursta = pBx->GetStation();
			vstf_y = sty + pGir->GetHeightFlangeUpperByTaper(cursta);
			height = pGir->GetHeightFlangeLower(cursta);
			sta = GetLengthSide(cursta,0,nG);
			Dom.PMoveTo(sta,-vstf_y+m_SideSttY);
			Dom.PLineTo(sta,-vstf_y - height+m_SideSttY);
			Dom.PLineEnd();
		}
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

// 측면도 중심선 그리기
void CAPlateDrawJong::SideCenter(CDomyun *pDom)
{	
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	pOptStd->SetEnvType(pDom, HCAD_HIDD);

	double	sta1 = 0,sta2 = 0,
			stt = GetStationKyungMaxMin(0),
			end = GetStationKyungMaxMin(pDB->m_nQtyJigan, FALSE);
	sta1 = GetLengthSide(stt-10000,0,-1);
	sta2 = GetLengthSide(end+10000,0,-1);
	pDom->LineTo(sta1,0,sta2,0);
}


BOOL CAPlateDrawJong::DrawGirderNum(long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		

	BOOL	bDraw = FALSE;
	bDraw = (-1 <= nG && nG < pDB->GetGirdersu());

	return bDraw;
}

void CAPlateDrawJong::DrawSideAll(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double	sttslab = pGir->GetSSS(OFF_STT_SLAB),
			endslab = pGir->GetSSS(OFF_END_SLAB),
			sttHstt = pGir->GetSSS(OFF_STT_HUNCHSTT),
			sttHend = pGir->GetSSS(OFF_STT_HUNCHEND),
			endHstt = pGir->GetSSS(OFF_END_HUNCHSTT),
			endHend = pGir->GetSSS(OFF_END_HUNCHEND),
			asc  = pDB->m_dThickPave,
			tc   = pDB->m_dThickSlabBase,
			sty  = asc+tc-m_SideSttY,
			stx  = pGir->GetSSS(OFF_STT_GIRDER),
			sta1 = 0,sta2 = 0;
	sta1 = GetLengthSide(sttslab,0,nG);
	sta2 = GetLengthSide(endslab,0,nG);

	Dom.LineTo(sta1,m_SideSttY,sta2,m_SideSttY);
	if(endslab >= endHend)
	{
		Dom.LineTo(sta2, -sty-pDB->m_dHeightTaperEnd);
		sta1 = GetLengthSide(endHstt,0,nG);
		Dom.LineTo(sta1,-sty-pDB->m_dHeightTaperEnd);
		sta2 = GetLengthSide(endHend,0,nG);
	}
	
	Dom.LineTo(sta2,-sty);
	sta1 = GetLengthSide(max(sttslab, sttHend),0,nG);
	Dom.LineTo(sta1,-sty);
	if(sttslab <= sttHend)
	{
		sta2 = GetLengthSide(sttHstt,0,nG);
		Dom.LineTo(sta2,-sty-pDB->m_dHeightTaperStt);
		sta1 = GetLengthSide(sttslab,0,nG);
		Dom.LineTo(sta1,-sty-pDB->m_dHeightTaperStt);
	}	
	Dom.LineTo(sta1,m_SideSttY);
	double dWidthMusucon = 0;
	sta2 = GetLengthSide(sttslab+dWidthMusucon,0,nG);
	double dThickMusucon = 0;
	Dom.Solid(sta1,m_SideSttY,sta2,m_SideSttY,sta2,-dThickMusucon+m_SideSttY,sta1,-dThickMusucon+m_SideSttY);
	sta1 = GetLengthSide(endslab,0,nG);
	sta2 = GetLengthSide(endslab-dWidthMusucon,0,nG);
	Dom.Solid(sta1,m_SideSttY,sta2,m_SideSttY,sta2,-dThickMusucon+m_SideSttY,sta1,-dThickMusucon+m_SideSttY);
	sta1 = GetLengthSide(sttslab+dWidthMusucon,0,nG);
	Dom.LineTo(sta1,-asc+m_SideSttY,sta2,-asc+m_SideSttY);

	if(sttslab <= sttHend)
	{
		sty += pDB->m_dHeightTaperStt;
		sta1 = GetLengthSide(stx,0,nG);
		Dom.LineTo(sta1,-sty,sta1,-sty-pGir->GetHeightInOutUpper(TRUE));
	}
	else
	{
		sta1 = GetLengthSide(stx,0,nG);
		Dom.LineTo(sta1,-sty,sta1,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightTaperStt);
	}
	sta2 = GetLengthSide(stx+pDB->m_dWidthInOutStt-pDB->m_dRadiusInOutStt,0,nG);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE));
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dRadiusInOutStt,pDB->m_dRadiusInOutStt,0,90);
	sta2 = GetLengthSide(stx+pDB->m_dWidthInOutStt,0,nG);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dRadiusInOutStt);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt+(pDB->m_dRadiusInOutStt));
	sta2 = GetLengthSide(stx+pDB->m_dWidthInOutStt-pDB->m_dRadiusInOutStt,0,nG);
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt+(pDB->m_dRadiusInOutStt),
		pDB->m_dRadiusInOutStt,270,90);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt-pDB->m_dHeightInOutLowerStt);

	stx = pGir->GetSSS(OFF_END_GIRDER);
	sty = asc+tc+pDB->m_dHeightTaperEnd-m_SideSttY;
	sta1 = GetLengthSide(stx,0,nG);
	if(endslab >= endHend)	Dom.LineTo(sta1,-sty,sta1,-sty-pGir->GetHeightInOutUpper(FALSE));
	else					Dom.LineTo(sta1,-sty+pDB->m_dHeightTaperEnd,sta1,-sty-pGir->GetHeightInOutUpper(FALSE));
	
	sta2 = GetLengthSide(stx-pDB->m_dWidthInOutEnd+pDB->m_dRadiusInOutEnd,0,nG);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE));
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dRadiusInOutEnd,pDB->m_dRadiusInOutEnd,90,90);
	sta2 = GetLengthSide(stx-pDB->m_dWidthInOutEnd,0,nG);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dRadiusInOutEnd);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd+(pDB->m_dRadiusInOutEnd));
	sta2 = GetLengthSide(stx-pDB->m_dWidthInOutEnd+pDB->m_dRadiusInOutEnd,0,nG);
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd+(pDB->m_dRadiusInOutEnd),
		pDB->m_dRadiusInOutEnd,180,90);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd-pDB->m_dHeightInOutLowerEnd);

	sta2 = GetLengthSide(pGir->GetSSS(OFF_STT_GIRDER),0,nG);
	Dom.LineTo(sta1,-asc-tc-pDB->m_dHeightTaperStt-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt-pDB->m_dHeightInOutLowerStt+m_SideSttY,
				sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd-pDB->m_dHeightInOutLowerEnd);
	
	// 측면도 거더 번호 출력
	CString str;
	if(nG < 0) str = "CR";
	else str.Format("G%d",nG+1+pDB->GetNumberStartGirder());
	pOptStd->SetEnvType(pDom, HCAD_SMALL_TITLE);
	sta1 = GetLengthSide(pGir->GetStationStt()-1000,0,nG);
	Dom.TextCircle(sta1,-sty-SttEndTHeight(nG)*0.5,str,4);
	*pDom << Dom;
}


// 변단면 일경우 측면도 그리기 
void CAPlateDrawJong::DrawByunSide(CDomyun *pDom, long nG)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CPlateGirderApp *pCG     = pDB->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double	sttslab = pGir->GetSSS(OFF_STT_SLAB),
			endslab = pGir->GetSSS(OFF_END_SLAB),
			sttHstt = pGir->GetSSS(OFF_STT_HUNCHSTT),
			sttHend = pGir->GetSSS(OFF_STT_HUNCHEND),
			endHstt = pGir->GetSSS(OFF_END_HUNCHSTT),
			endHend = pGir->GetSSS(OFF_END_HUNCHEND),
			asc  = pDB->m_dThickPave,
			tc   = pDB->m_dThickSlabBase,
			sty  = asc+tc-m_SideSttY,
			stx  = pGir->GetSSS(OFF_STT_GIRDER),
			sta1 = 0,sta2 = 0;
	sta1 = GetLengthSide(sttslab,0,nG);
	sta2 = GetLengthSide(endslab,0,nG);
	Dom.LineTo(sta1,m_SideSttY,sta2,m_SideSttY);
	Dom.LineTo(sta2, -sty-pDB->m_dHeightTaperEnd);
	sta1 = GetLengthSide(endHstt,0,nG);
	Dom.LineTo(sta1,-sty-pDB->m_dHeightTaperEnd);
	sta2 = GetLengthSide(endHend,0,nG);
	Dom.LineTo(sta2,-sty);
	sta1 = GetLengthSide(sttHend,0,nG);
	Dom.LineTo(sta1,-sty);
	sta2 = GetLengthSide(sttHstt,0,nG);
	Dom.LineTo(sta2,-sty-pDB->m_dHeightTaperStt);
	sta1 = GetLengthSide(sttslab,0,nG);
	Dom.LineTo(sta1,-sty-pDB->m_dHeightTaperStt);
	Dom.LineTo(sta1,m_SideSttY);
	double dWidthMusucon = 0;
	sta2 = GetLengthSide(sttslab+dWidthMusucon,0,nG);
	double dThickMusucon = 0;
	Dom.Solid(sta1,m_SideSttY,sta2,m_SideSttY,sta2,-dThickMusucon+m_SideSttY,sta1,-dThickMusucon+m_SideSttY);
	sta1 = GetLengthSide(endslab,0,nG);
	sta2 = GetLengthSide(endslab-dWidthMusucon,0,nG);
	Dom.Solid(sta1,m_SideSttY,sta2,m_SideSttY,sta2,-dThickMusucon+m_SideSttY,sta1,-dThickMusucon+m_SideSttY);
	sta1 = GetLengthSide(sttslab+dWidthMusucon,0,nG);
	Dom.LineTo(sta1,-asc+m_SideSttY,sta2,-asc+m_SideSttY);

	sty += pDB->m_dHeightTaperStt;
	sta1 = GetLengthSide(stx,0,nG);
	Dom.LineTo(sta1,-sty,sta1,-sty-pGir->GetHeightInOutUpper(TRUE));
	sta2 = GetLengthSide(stx+pDB->m_dWidthInOutStt-pDB->m_dRadiusInOutStt,0,nG);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE));
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dRadiusInOutStt,pDB->m_dRadiusInOutStt,0,90);
	sta2 = GetLengthSide(stx+pDB->m_dWidthInOutStt,0,nG);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dRadiusInOutStt);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt+(pDB->m_dRadiusInOutStt));
	sta2 = GetLengthSide(stx+pDB->m_dWidthInOutStt-pDB->m_dRadiusInOutStt,0,nG);
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt+(pDB->m_dRadiusInOutStt),
		pDB->m_dRadiusInOutStt,270,90);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(TRUE)-pDB->m_dHeightInOutStt-pDB->m_dHeightInOutLowerStt);

	stx = pGir->GetSSS(OFF_END_GIRDER);
	sty = asc+tc+pDB->m_dHeightTaperEnd-m_SideSttY;
	sta1 = GetLengthSide(stx,0,nG);

	Dom.LineTo(sta1,-sty,sta1,-sty-pGir->GetHeightInOutUpper(FALSE));
	sta2 = GetLengthSide(stx-pDB->m_dWidthInOutEnd+pDB->m_dRadiusInOutEnd,0,nG);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE));
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dRadiusInOutEnd,pDB->m_dRadiusInOutEnd,90,90);
	sta2 = GetLengthSide(stx-pDB->m_dWidthInOutEnd,0,nG);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dRadiusInOutEnd);
	Dom.LineTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd+(pDB->m_dRadiusInOutEnd));
	sta2 = GetLengthSide(stx-pDB->m_dWidthInOutEnd+pDB->m_dRadiusInOutEnd,0,nG);
	Dom.Arc(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd+(pDB->m_dRadiusInOutEnd),
		pDB->m_dRadiusInOutEnd,180,90);
	Dom.MoveTo(sta2,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd);
	Dom.LineTo(sta1,-sty-pGir->GetHeightInOutUpper(FALSE)-pDB->m_dHeightInOutEnd-pDB->m_dHeightInOutLowerEnd);

	long	gigansu,gigan = pDB->m_nQtyJigan;
	double	leftcurv,leftlen,rightcurv,rightlen,centerlen,height,
			sttshoe = pGir->GetSSS(OFF_STT_SHOE),
			sumsta = sttshoe,klen =0,lefth,righth,centerh;
	height = pGir->GetHeightGirderByJijum(0);
	stx = pGir->GetSSS(OFF_STT_GIRDER);
	sty = asc + tc-m_SideSttY;
	sta1 = GetLengthSide(stx,0,nG);
	sta2 = GetLengthSide(sumsta,0,nG);
	Dom.LineTo(sta1,-sty-height,sta2,-sty-height);
	Dom.MoveTo(sta2,m_SideSttY);
	for(gigansu = 0;gigansu < gigan;gigansu++)
	{
		klen = pGir->GetStationOnJijum(gigansu + 1) - pGir->GetStationOnJijum(gigansu);
		if(gigansu == 0)
			lefth = pGir->GetHeightGirderByJijum(gigansu);
		else lefth = pGir->GetHeightGirderByJijum(gigansu);
		if(gigansu < gigan-1)
			righth = pGir->GetHeightGirderByJijum(gigansu+1);
		else righth = pGir->GetHeightGirderByJijum(gigan);

		centerh = pDB->m_dHeightByun[gigansu];
		leftlen = pDB->m_dLenLeftByun[gigansu];
		leftcurv = pDB->m_dLenLeftCurve[gigansu];
		rightcurv = pDB->m_dLenRightCurve[gigansu];
		rightlen = pDB->m_dLenRightByun[gigansu];
		centerlen = klen - (leftlen+leftcurv+rightcurv+rightlen);
		sta1 = GetLengthSide(sumsta,0,nG);
		sta2 = GetLengthSide(sumsta+leftlen,0,nG);
		Dom.LineTo(sta1,-sty-lefth,sta2,-sty-lefth);
		sumsta+=leftlen;
		if(leftcurv > 0)
			ByunDanCurveDraw(&Dom,pGir,nG,sumsta,sumsta+leftcurv,-sty-lefth,0);
		sumsta += leftcurv;
		if(centerlen > 0){
			sta1 = GetLengthSide(sumsta,0,nG);
			sta2 = GetLengthSide(sumsta+centerlen,0,nG);
			Dom.LineTo(sta1,-sty-centerh,sta2,-sty-centerh);
		}
		sumsta += centerlen;
		if(rightcurv > 0)
			ByunDanCurveDraw(&Dom,pGir,nG,sumsta,sumsta+rightcurv,-sty-centerh,0);

		sumsta += rightcurv;
		sta1 = GetLengthSide(sumsta,0,nG);
		sta2 = GetLengthSide(sumsta+rightlen,0,nG);
		Dom.LineTo(sta1,-sty-righth,sta2,-sty-righth);
		sumsta += rightlen;
	}
	sta1 = GetLengthSide(sumsta,0,nG);
	sta2 = GetLengthSide(pGir->GetSSS(OFF_END_GIRDER),0,nG);
	Dom.LineTo(sta1,-sty-righth,sta2,-sty-righth);

// 측면도 거더 번호 출력
	CString str;
	if(nG < 0) str = "CR";
	else str.Format("G%d",nG+1+pDB->GetNumberStartGirder());
	pOptStd->SetEnvType(pDom, HCAD_SMALL_TITLE);
	sta1 = GetLengthSide(pGir->GetStationStt()-1000,0,nG);
	Dom.TextCircle(sta1,-sty-SttEndTHeight(nG)*0.5,str,4);
	*pDom << Dom;
}

// 측면도 좌측 치수선
void CAPlateDrawJong::DimSideLeftWidth(long nG, CDomyun *pDom)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("LEFT");

	long	dan = 1;
	double sta1 = GetLengthSide(pGir->GetSSS(OFF_STT_SLAB),0,nG),
			sum  = 0;
	CString str;

	Dom.DimMoveTo(sta1,m_SideSttY,dan);
	str.Format("%G",pDB->m_dThickSlabBase+pDB->m_dThickPave);
	Dom.DimLineTo(-atof(str),dan,str);
	if(pDB->m_dHeightTaperStt > 0)
	{
		str.Format("%G",pDB->m_dHeightTaperStt);
		Dom.DimLineTo(-atof(str),dan,str);
	}
	str.Format("%G",pGir->GetHeightInOutUpper(TRUE));
	Dom.DimLineTo(-atof(str),dan,str);
	str.Format("%G",pDB->m_dHeightInOutStt);
	Dom.DimLineTo(-atof(str),dan,str);
	str.Format("%G",pDB->m_dHeightInOutLowerStt);
	Dom.DimLineTo(-atof(str),dan,str);
	sum  = pDB->m_dThickSlabBase + pDB->m_dThickPave
		   + pDB->m_dHeightTaperStt
		   + pGir->GetHeightInOutUpper(TRUE)
		   + pDB->m_dHeightInOutStt
		   + pDB->m_dHeightInOutLowerStt;
	Dom.DimMoveTo(sta1,m_SideSttY,dan+1);
	str.Format("%G",sum);
	Dom.DimLineTo(-sum,dan+1,str);

	*pDom << Dom;
}

// 측면도 우측 치수선
void CAPlateDrawJong::DimSideRightWidth(long nG, CDomyun *pDom)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("RIGHT");

	long	dan = 1;
	double sta1 = GetLengthSide(pGir->GetSSS(OFF_END_SLAB),0,nG),
			sum  = 0;
	CString str;

	Dom.DimMoveTo(sta1,m_SideSttY,dan);
	str.Format("%G",pDB->m_dThickSlabBase+pDB->m_dThickPave);
	Dom.DimLineTo(-atof(str),dan,str);
	if(pDB->m_dHeightTaperEnd > 0)
	{
		str.Format("%G",pDB->m_dHeightTaperEnd);
		Dom.DimLineTo(-atof(str),dan,str);
	}
	str.Format("%G",pGir->GetHeightInOutUpper(FALSE));
	Dom.DimLineTo(-atof(str),dan,str);
	str.Format("%G",pDB->m_dHeightInOutEnd);
	Dom.DimLineTo(-atof(str),dan,str);
	str.Format("%G",pDB->m_dHeightInOutLowerEnd);
	Dom.DimLineTo(-atof(str),dan,str);
	sum  = pDB->m_dThickSlabBase 
		   + pDB->m_dThickPave
		   + pDB->m_dHeightTaperEnd
		   + pGir->GetHeightInOutUpper(FALSE)
		   + pDB->m_dHeightInOutEnd
		   + pDB->m_dHeightInOutLowerEnd;
	Dom.DimMoveTo(sta1,m_SideSttY,dan+1);
	str.Format("%G",sum);
	Dom.DimLineTo(-sum,dan+1,str);

	*pDom << Dom;
}

double CAPlateDrawJong::GetSideSttY(long nG, long nCount)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		

	double y = GetMaxHeight(nG);
	long dan = 1;
/*	if( m_pEnv->m_bDimDiap   == 1 ) dan++;
	if( m_pEnv->m_bDimSplice == 1 ) dan++;
	if( m_pEnv->m_bDimWFact  == 1 ) dan++;
	if( m_pEnv->m_bDimStud   == 1 ) dan++;
	if( m_pEnv->m_bDimGigan  == 1 ) dan++;
	if( m_pEnv->m_bDimSlab   == 1 ) dan++;
*/	if( pDB->m_cByunDan != 0/* && m_pEnv->m_bDimGigan*/) dan += 2;
	
	y += 1700 * (dan+1);

	return -y;
}

// 화면의 중심에 도면 위치시키기위한 각도 계산
void CAPlateDrawJong::SetCenterAngle()
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();		

	m_SW = 0;
	CPlateGirderApp *pGirCR = pDB->GetGirder(-1);
	m_CenterXY = pGirCR->GetLine()->GetXyLineDis(pGirCR->GetStationStt(),0);
	CDPoint xy = pGirCR->GetLine()->GetXyLineDis(pGirCR->GetStationEnd(),0);

	m_vRotate = (xy - m_CenterXY).Unit(); // 선형 회전각
}

// 측면도 좌표 변환
double CAPlateDrawJong::GetLengthSide(double sta, double width, long nG)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();		

	CDPoint xy  = pDB->GetGirder(nG)->GetXyGirderDis(sta, width);
	double dist = (xy - m_CenterXY) ^ m_vRotate;

	return dist;
}

double CAPlateDrawJong::GetMaxHeight(long nG)
{
	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	double Result = 0,bik = 0;
	for(long n=0; n<pDB->m_nQtyJigan+1; n++)
	{
		bik = pGir->GetHeightGirderByJijum(n);
		if(bik > Result) Result = bik;
	}

	return Result;
}


double CAPlateDrawJong::GetStationKyungMaxMin(long kyung, BOOL bMin)
{
	CPlateBridgeApp *pDB    = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGirCR = pDB->GetGirder(-1);

	if(pDB->m_vAngJijum[kyung] == CDPoint(0,1))
		return pGirCR->GetStationOnJijum(kyung);

	double station, result;
	BOOL bFirst = TRUE;
	for(long g=0; g<pDB->GetGirdersu(); g++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(g);
		if (pGir->GetLine() != pGirCR->GetLine())
			continue;
		station = pGir->GetStationOnJijum(kyung);
		if (bFirst)   {
			result = station;
			bFirst = FALSE;
		}
		if (bMin)
			result = min (result, station);
		else
			result = max (result, station);
	}
	return result;
}

double CAPlateDrawJong::SttEndTHeight(long nG, long pos)
{
	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir = pDB->GetGirder(nG);

	double Result = 0;
	if(pos == 1) // 시점
		 Result = pDB->m_dHeightInOutStt+pGir->GetHeightInOutUpper(TRUE)+pDB->m_dHeightInOutLowerStt;
	else Result = pDB->m_dHeightInOutEnd+pGir->GetHeightInOutUpper(FALSE)+pDB->m_dHeightInOutLowerEnd;
	
	return Result;
}


double CAPlateDrawJong::ByunDanCurveDraw(CDomyun *pDom, CPlateGirderApp* pGir, long nG, double sttsta, double endsta, double height, double width)
{
	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();		

	double	length = (endsta - sttsta)/100,
			sty    = pDB->m_dThickPave + pDB->m_dThickSlabBase - m_SideSttY,
			cursta = sttsta, sta1 = 0;
	sta1 = GetLengthSide(cursta, width, nG);
	pDom->MoveTo(sta1,height);
	for(long n=0; n<100; n++)
	{
		cursta += length;
		height  = pGir->GetHeightFlangeLower(cursta);
		sta1    = GetLengthSide(cursta, width, nG);
		pDom->LineTo(sta1,-sty-height);
	}
	return 0;
}

void CAPlateDrawJong::AddSttDimJongBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir 	 = pBx->GetGirder();
	CLineInfo       *pLine	 = pGir->GetLine();		
	
	if (!pBx->GetWebHStiff()) return;

	double Fixx = pBx->GetStation();	

	CDimDomyunEx Dom(pDom);
	Dom.SetDirection("LEFT");
	Dom.SetDimDir(DIR_NONE);	
	// 수직보강재 설치위치에 따라 나눈 이유는 수평보강재 때문이다.
	BOOL bLeft;
	if(pBx->GetGirder()->m_nInstallVStiff==0 || pBx->GetGirder()->m_nInstallVStiff==2)	//양측
		bLeft = FALSE;
	else
		bLeft = TRUE;

	double Sta = Fixx;	
	double Height   = pGir->GetHeightGirderByStaAng(Sta);	
	long UpperDan	= pBx->GetWebHStiff()->IsEmpty(1, bLeft) ? 1 : 2;
	long LowerDan	= pBx->GetWebHStiff()->IsEmpty(3, bLeft) ? 1 : 2;																	
	double UpperFactor = (UpperDan==1) ? 0.20 : 0.14;
	double LowerFactor = (LowerDan==1) ? 0.20 : 0.14;
	double UpperTerm	= pBx->IsCompressByJRib(TRUE)  ? Height * UpperFactor : 0;
	double LowerTerm	= pBx->IsCompressByJRib(FALSE) ? Height * LowerFactor : 0;
	double UpperTerm2	= (UpperDan==1) ? 0 : Height * 0.22;
	double LowerTerm2	= (LowerDan==1) ? 0 : Height * 0.22;
	double MiddleTerm	= Height;
	
	if(pBx->IsCompressByJRib(TRUE))	MiddleTerm -= UpperTerm + UpperTerm2;
	if(pBx->IsCompressByJRib(FALSE)) MiddleTerm -= LowerTerm + LowerTerm2;

	double y1	= pGir->GetElevationFlangeCamber(Sta) + pGir->GetHeightFlangeUpperByTaper(Sta);
	double xDis = Dom.Always(30);
				
	if(pBx->IsState(BX_STT_GIRDER))
	{
		// 출입구 제원
		double C  = pDB->IsTUGir() ? 0 : pDB->m_dHeightTaperStt;	// EXP. JOINT에의한 단차
		double D  = pGir->GetHeightInOutUpper(TRUE);		// 출입구상단~거더상단
		double E  = pDB->m_dHeightInOutStt;		// 출입구 높이
		
		Dom.DimBasePoint(Fixx - xDis, y1 - C);			
		Dom.DimMoveTo(Fixx,y1 - C);		
		Dom.DimLineTo(Fixx,-D,COMMA(D));
		Dom.DimLineTo(Fixx,-E,COMMA(E));
		Dom.DimLineTo(Fixx,-Height+D+E,COMMA(Height - D - E));	
		xDis += Dom.Always(Dom.GetDimLevelDis());
	}
		
	Dom.DimBasePoint(Fixx - xDis, y1 - pGir->GetHeightFlangeUpperByTaper(Sta));	
	Dom.DimMoveTo(Fixx,y1 - pGir->GetHeightFlangeUpperByTaper(Sta));		
	if(pBx->IsCompressByJRib(TRUE))
	{
		Dom.DimLineTo(Fixx,-UpperTerm,COMMA(UpperTerm));		
		Dom.DimLineTo(Fixx,-UpperTerm2,COMMA(UpperTerm2));
	}
	Dom.DimLineTo(Fixx,-MiddleTerm,COMMA(MiddleTerm));
	
	if(pBx->IsCompressByJRib(FALSE))
	{
		Dom.DimLineTo(Fixx,-LowerTerm2,COMMA(LowerTerm2));
		Dom.DimLineTo(Fixx,-LowerTerm,COMMA(LowerTerm));
	}
	
	xDis += Dom.Always(Dom.GetDimLevelDis());
	Dom.SetDimDir(DIR_LEFT);
	Dom.DimBasePoint(Fixx - xDis, y1);
	if(pBx->IsState(BX_STT_GIRDER))
	{		
		Dom.DimMoveTo(Fixx,y1);
		Dom.DimLineTo(Fixx,-pGir->GetHeightFlangeUpperByTaper(Sta), COMMA(pGir->GetHeightFlangeUpperByTaper(Sta)),"",TRUE);
	}
	else			
		Dom.DimMoveTo(Fixx,y1,0,TRUE);
	
	Dom.SetDimDir(DIR_RIGHT);
	Dom.DimLineTo(Fixx,-Height,COMMA(Height),"",TRUE);

	*pDom << Dom;	   
} 
 
void CAPlateDrawJong::AddEndDimJongBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{	
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CPlateGirderApp *pGir 	 = pBxStt->GetGirder();
	CLineInfo       *pLine	 = pGir->GetLine();		

	CPlateBxFinder Finder(pGir);

	double SttSta,EndSta,DisSta;		

	SttSta = pBxStt->GetStation();
	EndSta = pBxEnd->GetStation();
	DisSta = pGir->GetLengthDimType(SttSta,EndSta, 0);		
	
	double Fixx = SttSta + DisSta;

	CDimDomyunEx Dom(pDom);		
	Dom.SetDirection("RIGHT");
	Dom.SetDimDir(DIR_NONE);
	
	double Sta = Fixx;	
	double Height = pGir->GetHeightGirderByStaAng(EndSta);	
	CPlateBasicIndex *pBx =  NULL;
	CWebHStiff *pWebHStiff = pBxEnd->GetWebHStiff();

	__int64 BxFlag = BX_PLATEHSTIFF;

	if(!pWebHStiff) 
		pBx = Finder.GetBxLast(BxFlag);
	else
		pBx = pBxEnd;

	if (!pBx->GetWebHStiff()) return;

	// 수직보강재 설치위치에 따라 나눈 이유는 수평보강재 때문이다.
	BOOL bLeft;
	if(pBx->GetGirder()->m_nInstallVStiff==0 || pBx->GetGirder()->m_nInstallVStiff==2)	//양측
		bLeft = FALSE;
	else
		bLeft = TRUE;

	long UpperDan	= pBx->GetWebHStiff()->IsEmpty(1, bLeft) ? 1 : 2;
	long LowerDan	= pBx->GetWebHStiff()->IsEmpty(2, bLeft) ? 1 : 2;																
	double UpperFactor = (UpperDan==1) ? 0.20 : 0.14;
	double LowerFactor = (LowerDan==1) ? 0.20 : 0.14;
	double UpperTerm	= pBx->IsCompressByJRib(TRUE)    ? Height * UpperFactor : 0;
	double LowerTerm	= pBx->IsCompressByJRib(FALSE) ? Height * LowerFactor : 0;
	double UpperTerm2	= (UpperDan==1) ? 0 : Height * 0.22;
	double LowerTerm2	= (LowerDan==1) ? 0 : Height * 0.22;
	double MiddleTerm	= Height;
	
	if(pBx->IsCompressByJRib(TRUE))	MiddleTerm -= UpperTerm + UpperTerm2;
	if(pBx->IsCompressByJRib(FALSE)) MiddleTerm -= LowerTerm + LowerTerm2;

	double y1	= pGir->GetElevationFlangeCamber(EndSta) + pGir->GetHeightFlangeUpperByTaper(Sta);
	double xDis = Dom.Always(30);	
	
	if(pBxEnd->IsState(BX_END_GIRDER))
	{
		// 출입구 제원
		double C  = pDB->IsTUGir() ? 0 : pDB->m_dHeightTaperEnd;	// EXP. JOINT에의한 단차
		double D  = pGir->GetHeightInOutUpper(FALSE);		// 출입구상단~거더상단
		double E  = pDB->m_dHeightInOutEnd;		// 출입구 높이
		
		Dom.DimBasePoint(Fixx + xDis, y1 - C);		
		Dom.DimMoveTo(Fixx,y1 - C);		
		Dom.DimLineTo(Fixx,-D,COMMA(D));
		Dom.DimLineTo(Fixx,-E,COMMA(E));		
		Dom.DimLineTo(Fixx,-Height+D+E,COMMA(Height - D - E));	
		xDis += Dom.Always(Dom.GetDimLevelDis());
	}

	Dom.DimBasePoint(Fixx + xDis, y1 - pGir->GetHeightFlangeUpperByTaper(Sta));	
	Dom.DimMoveTo(Fixx,y1 - pGir->GetHeightFlangeUpperByTaper(Sta));
	
	if(pBx->IsCompressByJRib(TRUE))
	{
		Dom.DimLineTo(Fixx, -UpperTerm,COMMA(UpperTerm));		
		Dom.DimLineTo(Fixx, -UpperTerm2,COMMA(UpperTerm2));
	}
	Dom.DimLineTo(Fixx, -MiddleTerm,COMMA(MiddleTerm));
	
	if(pBx->IsCompressByJRib(FALSE))
	{
		Dom.DimLineTo(Fixx, -LowerTerm2,COMMA(LowerTerm2));
		Dom.DimLineTo(Fixx, -LowerTerm,COMMA(LowerTerm));
	}

	xDis += Dom.Always(Dom.GetDimLevelDis());
	Dom.SetDimDir(DIR_LEFT);
	Dom.DimBasePoint(Fixx + xDis, y1);	
	if(pBxEnd->IsState(BX_END_GIRDER))
	{		
		Dom.DimMoveTo(Fixx,y1);	
		Dom.DimLineTo(Fixx,-pGir->GetHeightFlangeUpperByTaper(EndSta),COMMA(pGir->GetHeightFlangeUpperByTaper(EndSta)),"",TRUE);
	}
	else
		Dom.DimMoveTo(Fixx,y1,0,TRUE);	
	
	Dom.SetDimDir(DIR_RIGHT);
	Dom.DimLineTo(Fixx,-Height,COMMA(Height),"",TRUE);			

	*pDom << Dom;	
}

// dLineThick = 선 두께
// nPos       = 값에 무관하게 거더중심.. CAPlateDrawPyung과 맞추기 위해 변수유지
// nLineColor = -1: SetEnvType(&Dom, HCAD_DIML); 색상을 유지..
// nLineColor 이 -1아니면 그 색상을 적용함.
void CAPlateDrawJong::SetCursorPosition(long nG, double staStt, double staEnd, CDomyun *pDom, BOOL bDelCursor, 
										 double dLineThick, long nPos, long nLineColor)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB = m_pDataMng->GetBridge();

	if(pDom == NULL)
		return;
	if(bDelCursor) pDom->DeleteCursor();

	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER|BX_END_GIRDER);
	if(!pBx) return;

	double dHStt	= pDB->m_dHeightTaperStt + pDB->m_dHeightInOutStt + pDB->m_dHeightInOutLowerStt;
	double dHEnd	= pDB->m_dHeightTaperEnd + pDB->m_dHeightInOutEnd + pDB->m_dHeightInOutLowerEnd;
	double dGirH	= pDB->m_dThickSlabBase + pDB->m_dThickPave + pGir->GetHeightInOutUpper(TRUE) + max(dHStt, dHEnd) + 100*2;
	double dSideSttY	= 100;

	//////////////////////////////////////////////////////////////////////////
	SetCenterAngle();
	staStt = GetLengthSide(staStt,0,nG);
	staEnd = GetLengthSide(staEnd,0,nG);
	//////////////////////////////////////////////////////////////////////////	

	CDPoint xy(staStt, dSideSttY);
	pDom->MakeCursor(xy.x, xy.y, 90);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	if(nLineColor!=-1)
		Dom.SetLineColor(nLineColor);
	Dom.SetPLineThick(dLineThick);	

	for(long n=-1; n<nG; n++)
		dSideSttY += GetSideSttY(n, n+1);

	CDPoint p1, p2, p3, p4;
	p1 = CDPoint(staStt, dSideSttY);
	p2 = CDPoint(staStt, dSideSttY-dGirH);
	p3 = CDPoint(staEnd, dSideSttY);
	p4 = CDPoint(staEnd, dSideSttY-dGirH);

	Dom.PMoveTo(p1.x,p1.y,0,TRUE);
	Dom.PLineTo(p2.x,p2.y);
	Dom.PLineTo(p4.x,p4.y);
	Dom.PLineTo(p3.x,p3.y);
	Dom.PLineEnd();

	*pDom << Dom;
	pDom->Invalidate();
}
