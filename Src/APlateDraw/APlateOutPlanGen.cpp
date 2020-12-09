// APlateOutPlanGen.cpp: implementation of the CAPlateOutPlanGen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutPlanGen::CAPlateOutPlanGen()
{

}

CAPlateOutPlanGen::~CAPlateOutPlanGen()
{

}

//////////////////////////////////////////////////////////////////////

void CAPlateOutPlanGen::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// 일반도이전의 도면번호
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	

//	EXIT_RTN;
	pPageMng->SetProgressData("평면도 출력중 ...",50);
	AddPlan(&Dom,nSubPage);
	*pDom << Dom;	

	pPageMng->SetProgressData("일반 평면도 출력완료...",100);
}

long CAPlateOutPlanGen::GetPageSu()
{
	return (m_nPage = 1);
}

double CAPlateOutPlanGen::GetScale()
{
	return 100;
}

CString CAPlateOutPlanGen::GetTitle()
{
	CString csTitle = "일반 평면도";

	return csTitle;
}
void CAPlateOutPlanGen::AddPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COptionStd		*pDomOpt = pStd->m_pDomOptionStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CString str = _T("");
	
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	long nG = 0;
	CPlateBasicIndex *pBxLeftStt = GetPageBxStt(nG);
	CPlateBasicIndex *pBxLeftEnd = GetPageBxEnd(nG);
	nG = pBridge->GetGirdersu()-1;
	CPlateBasicIndex *pBxRightStt = GetPageBxStt(nG);
	CPlateBasicIndex *pBxRightEnd = GetPageBxEnd(nG);
	DrawPyung.DrawSlabByBx(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);

	CPlateBasicIndex *pBxStt = GetPageBxStt(CENTERGIRDER);
	CPlateBasicIndex *pBxEnd = GetPageBxEnd(CENTERGIRDER);	
	DrawPyung.DrawCenterLine(&Dom,-1,pBxStt->GetStation(),pBxEnd->GetStation());		
	for( nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		pBxStt = GetPageBxStt(nG);
		pBxEnd = GetPageBxEnd(nG);	
		DrawPyung.DrawHBracingCLWithenBx(&Dom, pBxStt, pBxEnd);
		DrawPyung.DrawCRBeamCLWithenBx(&Dom, pBxStt, pBxEnd);//가로보 중심선만그림		
	}

	//이음판이 모두 그려져야 제원이 설정됨
	for(nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		pBxStt = GetPageBxStt(nG);
		pBxEnd = GetPageBxEnd(nG);	
		DrawPyung.DrawHBracingSpliceWithenBx(&Dom, pBxStt, pBxEnd);	
		DrawPyung.DrawHBracingBeamWithenBx(&Dom, pBxStt, pBxEnd);	
	}

	for(nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		pBxStt = GetPageBxStt(nG);
		pBxEnd = GetPageBxEnd(nG);	
		DrawPyung.DrawCRBeamVBracingFlangeWithenBx(&Dom, pBxStt, pBxEnd);	
	}


	for(long nSide=-1;nSide<=1; nSide+=2)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);			
		if(nSide==-1) pStd->m_pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);

		long nSttGir = (nSide==0 || nSide==-1) ? 0 : pBridge->GetGirdersu() / 2 + 1;
		long nEttGir = (nSide==0 || nSide== 1) ? pBridge->GetGirdersu() : pBridge->GetGirdersu() / 2;
		
		if(pBridge->GetGirdersu()%2)
		{
			long nG = pBridge->GetGirdersu()/2;
			pBxStt = GetPageBxStt(nG);
			pBxEnd = GetPageBxEnd(nG);	
			CPlateBxFinder	Finder(pBridge->GetGirder(nG));
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | (nSide==-1 ? BX_UPPERFLANGE : BX_LOWERFLANGE),pBxStt,pBxEnd);
			if(pBx != pBxStt)
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), nSide, 0);	
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), nSide);	
			}
			while(Finder.GetBxFixNext())
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), nSide, 0);	
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), nSide);	
				pBx = Finder.GetBxNext();
			}
			if(pBx != pBxEnd)
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), nSide, 0);	
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), nSide);	
			}

			DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_U, -2, TRUE);
			DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_W, 2, TRUE);
			DrawPyung.DrawSpliceCut(&Dom, pBxStt, pBxEnd, 0, TRUE, TRUE);
			DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_L, 2, TRUE);
			DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd, -1, TRUE, FALSE);
			DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd,  1, FALSE, FALSE);
			DrawPyung.DrawCenterLine(&Dom, nG, pBxStt->GetStation(), pBxEnd->GetStation(),FALSE, FALSE, TRUE, FALSE);								
			DrawPyung.DrawVStiffWithenBx(&Dom, pBxStt, pBxEnd);
			DrawPyung.DrawJackUpWithenBx(&Dom, pBxStt, pBxEnd);
			DrawPyung.DrawJiJumStiffWithenBx(&Dom, pBxStt, pBxEnd);
		}

		for(nG = nSttGir; nG < nEttGir; nG++)
		{
			CPlateBxFinder	Finder(pBridge->GetGirder(nG));
			pBxStt = GetPageBxStt(nG);
			pBxEnd = GetPageBxEnd(nG);	
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | (nSide==-1 ? BX_UPPERFLANGE : BX_LOWERFLANGE),pBxStt,pBxEnd);				
			if(pBx != pBxStt)
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), 0, 0);	
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), 0);					
			}
			while(Finder.GetBxFixNext())
			{					
				DrawPyung.DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), 0, 0);	
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), 0);	
				pBx = Finder.GetBxNext();
			}			
			if(pBx != pBxEnd)
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), 0, 0);	
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), 0);	
			}
			DrawPyung.DrawCenterLine(&Dom, nG, pBxStt->GetStation(), pBxEnd->GetStation(),FALSE,FALSE,FALSE,FALSE);		
			DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_U, nSide, TRUE);
			DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_W, 2, TRUE);
			DrawPyung.DrawSpliceCut(&Dom, pBxStt, pBxEnd, 0, TRUE, TRUE);
			DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd, 0, nSide==-1 ? TRUE : FALSE, FALSE);
			DrawPyung.DrawVStiffWithenBx(&Dom, pBxStt, pBxEnd);
			DrawPyung.DrawJackUpWithenBx(&Dom, pBxStt, pBxEnd);
			DrawPyung.DrawJiJumStiffWithenBx(&Dom, pBxStt, pBxEnd);
		}		
	}			 
	// 도면 심볼
	DrawPyung.DrawGirderNumber(&Dom, pBxEnd);
	DimTextMarkPlan(&Dom, nSubPage);
	
	pBxStt = GetPageBxStt(0);
	pBxEnd = GetPageBxEnd(0);	
	
	pOpt->SetEnvType(&Dom,HCAD_DIML);		
	long nMLDanUp = DrawDimPyung.DimPyungTotal(&Dom, pBxStt, pBxEnd, pStd->GetDomOptionStd()->m_GenDim_Plan_Upper, TRUE, TRUE,  "", 2, TRUE);	
	pBxStt = GetPageBxStt(pBridge->GetGirdersu()-1);
	pBxEnd = GetPageBxEnd(pBridge->GetGirdersu()-1);	
	long nMLDanLow = DrawDimPyung.DimPyungTotal(&Dom, pBxStt, pBxEnd, pStd->GetDomOptionStd()->m_GenDim_Plan_Lower, FALSE, TRUE,  "", 0, TRUE);		

	pBxStt = GetPageBxStt(0);
	pBxEnd = GetPageBxEnd(0);	

	DrawDimPyung.DrawDimHDan(&Dom,pBxStt,TRUE);
	DrawDimPyung.DrawDimHDan(&Dom,pBxEnd,FALSE);
	
	// Match Line
	if(pStd->GetDomOptionStd()->m_Gen_bMatchLine)
	{
		long nG = -1;
		CPlateBasicIndex *pBxStt = GetPageBxStt(nG);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG);
		long nRealPage = pStd->GetDomOptionStd()->m_Gen_bDivPlanJong ? nSubPage-(m_nPage/2) : nSubPage;
		if(pStd->GetDomOptionStd()->m_Gen_bPyungJijumStation)	nMLDanUp+=2;

		if(!pBxStt->IsState(BX_STT_GIRDER))
			DimTextPyungMatchLine(&Dom, pBxStt, nRealPage-1, nMLDanUp, nMLDanLow);
		if(!pBxEnd->IsState(BX_END_GIRDER))
			DimTextPyungMatchLine(&Dom, pBxEnd, nRealPage  , nMLDanUp, nMLDanLow);
	}
	
	// 도로중심선 (MAIN 선형)
	CPlateGirderApp *pGir = pBridge->GetGirder(-1);
	pOpt->SetEnvType(pDom,HCAD_CENT);

	double BP = pGir->GetLine()->m_dStationBp;
	double EP = pGir->GetLine()->GetStationEpLine();

	double dStaStt = BP;
	double dStaEnd = EP;

	pGir->GetLine()->DrawHoriAlignLine(pDom, dStaStt, dStaEnd,0);

	pOpt->SetEnvType(pDom, HCAD_DIML);

	if(pDomOpt->m_PlanGen_Plane_bIP)
		pGir->GetLine()->DrawHoriAlignIPLine(pDom, dStaStt, dStaEnd);

	if(pDomOpt->m_PlanGen_Plane_bChain)
	{
		pGir->GetLine()->DrawHoriAlignIPFactor(pDom, dStaStt, dStaEnd);
		pGir->GetLine()->DrawHoriAlignChain(pDom, dStaStt, dStaEnd);
	}

	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		pGir = pBridge->GetGirder(nG);

		if(pBridge->IsExtLine(nG))
		{
			pOpt->SetEnvType(pDom,HCAD_CENT);

			BP = pGir->GetLine()->m_dStationBp;
			EP = pGir->GetLine()->GetStationEpLine();

			dStaStt = BP;
			dStaEnd = EP;

			pGir->GetLine()->DrawHoriAlignLine(pDom, dStaStt, dStaEnd,0);

			pOpt->SetEnvType(pDom, HCAD_DIML);

			if(pDomOpt->m_PlanGen_Plane_bIP)
				pGir->GetLine()->DrawHoriAlignIPLine(pDom, dStaStt, dStaEnd);

			if(pDomOpt->m_PlanGen_Plane_bChain)
			{
				pGir->GetLine()->DrawHoriAlignIPFactor(pDom, dStaStt, dStaEnd);
				pGir->GetLine()->DrawHoriAlignChain(pDom, dStaStt, dStaEnd);
			}
		}
	}
	*pDom << Dom;	
}

CPlateBasicIndex* CAPlateOutPlanGen::GetPageBxStt(long nG)
{	 
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);

	CPlateBasicIndex *pBx = NULL;
	CPlateBxFinder Finder(pGir);
	pBx = Finder.GetBxFirst(BX_STT_GIRDER);
	
	return pBx;
}


CPlateBasicIndex* CAPlateOutPlanGen::GetPageBxEnd(long nG)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);

	CPlateBasicIndex *pBx = NULL;
	CPlateBxFinder Finder(pGir);
	pBx = Finder.GetBxLast(BX_END_GIRDER);
	
	return pBx;
}

void CAPlateOutPlanGen::DimTextMarkPlan(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	
	CDomyun Dom(pDomP);

	long nUpDown   = 1;
	long nGirdersu = pBridge->GetGirdersu();

	for (long nG=0; nG < nGirdersu ; nG++)
	{
		CPlateBasicIndex *pBxStt = GetPageBxStt(nG);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG);

		if(nGirdersu%2 ==0)	
		{
			if(nG<=(nGirdersu/2-1)) nUpDown=1;
			else nUpDown=0; 
		}
		if(nGirdersu%2==1)
		{
			if(nG < (nGirdersu/2))	nUpDown=1; 
			else if (nG == (nGirdersu/2)) 
				nUpDown=-1;
			else nUpDown=0;
		}		
		DimTextPyungTotal(&Dom,pBxStt,pBxEnd,nUpDown);
	}

	*pDomP << Dom;	
}

void CAPlateOutPlanGen::DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDanUp, long nDanLow)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	CPlateGirderApp		*pGir		= pBx->GetGirder();	

	CSymbolDom SymDom(pDom, pStd->m_pOptStd);
	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
	CString str;
	double sta   = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();
	CDPoint xy1(0,0),xy2(0,0);
	
	double Dis	   = pBridge->GetDisSlabLeftDom(pBx);
	double Scale   = SymDom.GetScaleDim();
	double dExo	   = SymDom.GetDimExo();
	double vLen	   = SymDom.GetDimVLen();
	double dLevel  = SymDom.GetDimLevelDis();
	double distExo = (dExo+vLen+dLevel*(nDanUp))*Scale;
	double TextHeight = SymDom.GetTextHeight()*2.2;

	xy1 = pGir->GetXyGirderDis(sta, Dis-distExo, vAng);
	str.Format("%s%d","M.L", nSubPage);
	CDPoint rAng = pGir->GetLine()->GetAngleAzimuth(sta);
	SymDom.DrawSymbolHexagonStr(xy1,rAng,str);

	distExo -= TextHeight/2;
	xy1 = pGir->GetXyGirderDis(sta, Dis-distExo, vAng);
	SymDom.MoveTo(xy1);
	Dis		= pBridge->GetDisSlabRightDom(pBx);
	distExo = (dExo+vLen+dLevel*(nDanLow))*Scale;
	xy2		= pGir->GetXyGirderDis(sta, Dis+distExo, vAng);
	SymDom.LineTo(xy2);

	*pDom << SymDom;
}

void CAPlateOutPlanGen::DimTextPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();	
	CPlateGirderApp		*pGir		= pBxStt->GetGirder();	
	
	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);

	double LenGisi = Dom.Always(12);
	if(pBxStt->GetNumberGirder()==pBridge->GetGirdersu()-1)
		LenGisi = Dom.Always(6)+pBridge->m_dWidthSlabRight;

	m_JweonInfoStaArr.RemoveAll();

	//현장이음 구간 등록 
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxSplice = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	while(pBxSplice)
	{
		double sta = pBxSplice->GetStation();
		if(sta > pBxEnd->GetStation()) sta = pBxEnd->GetStation(); 
		CPlateSplice	*pSp = pBxSplice->GetSplice();
		double W = (pSp->m_uA + pSp->m_uB * pSp->m_uC + pSp->m_uC1/2) * 2;
		CDPoint staxy;
		staxy.x = pGir->GetStationByActLength(sta, -W/2, 0);
		staxy.y = pGir->GetStationByActLength(sta,  W/2, 0);
		m_JweonInfoStaArr.Add(staxy);
		pBxSplice = Finder.GetBxNext();
	}
//	DimTextPyungManHole(pDom, pBxStt, pBxEnd,nUpper,LenGisi);		//1. 맨홀 
//	SortSta();
//	DimTextPyungWaterOut(pDom, pBxStt, pBxEnd,nUpper,LenGisi);		//2. 결로수 집수구 
//	SortSta();
//	DimTextPyungJRib(pDom, pBxStt, pBxEnd, nUpper,LenGisi);			//3. 종브 
//	SortSta();
//	DimTextPyungCrossBeam(pDom, pBxStt, pBxEnd, nUpper,LenGisi);	//4. 다이아프램 
//	SortSta();
//	if(nUpper==1)
//	{
//		if(bHRibBuje || bHRibMat)
//			DimTextPyungHRib(pDom, pBxStt, pBxEnd, nUpper,LenGisi);	//5. 횡리브 
//	}
//	else
//	{
//		if(bVStiffBuje || bVStiffMat)
			DimTextPyungVStiff(&Dom, pBxStt, pBxEnd, nUpper,LenGisi); //5.  수직보강재
//	}

//	SortSta();
//	DimTextPyungCrossBeam(pDom, pBxStt, pBxEnd,nUpper,LenGisi);		//6. 가로보
//	SortSta();
	DimTextPyungFlange(&Dom, pBxStt, pBxEnd, nUpper,LenGisi);		//7. 상하판 

	*pDomP << Dom;	
}

void CAPlateOutPlanGen::DimTextPyungVStiff(CDomyun *pDomP, CPlateBasicIndex *pBxStt, 
										   CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi)
{
	/*
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
 	CPlateGirderApp		*pGir		= pBxStt->GetGirder();	
	CSteelSection	*pBSC		= pStd->GetDataManage()->GetSteelSection();	

	CPlateBxFinder Finder(pGir);
//	CPlateBasicIndex *pBxStd = Finder.GetBxFirst(BX_STT_GIRDER|BX_SPLICE,pBxStt,pBxEnd,FALSE);
//	if(pBxStd==NULL) return;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF,pBxStt,pBxEnd);
	long nCount = Finder.GetCountBx();
	pBx = Finder.GetBxNext(nCount/2);
	CPlateBasicIndex *pBxold = NULL;
	if(pBx==NULL) return;

	CDomyun Dom(pDomP);

	CString strBuje	= "수직보강재";
	CString strMat	= pBSC->GetMaterialByMarkBuje(B_V);
	double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));

	double sta  = pBx->GetStation();
	double sta2 = pGir->GetStationByActLength(sta, textLen, 0);
	pBxold = pBx;
	while (1)
	{
		if (CheckStation(sta, sta2))
		{
			while(pBx)
			{
				pBx = Finder.GetBxNext();
				if (pBx == NULL)
					break;
				else
					continue;
			}
			if (pBx == NULL)
				break;
			sta = pBx->GetStation();
			sta2 = pGir->GetStationByActLength(sta, textLen, 0);
		}
		else
			break;
	}
	if (pBx==NULL)
		pBx = pBxold;	

	long   nG = pBxStt->GetNumberGirder();
	double dEndDist = pGir->m_dWidth/2+dLenGisi;
	double dist = pGir->m_dWidth/2;
	sta  = pBx->GetStation();
	CDPoint xy1 = pGir->GetXyGirderDis(sta,dist,pBx->GetAngle());
	CDPoint xy2 = pGir->GetXyGirderDis(sta,dEndDist);
	m_JweonInfoStaArr.Add(CDPoint(sta,sta));

	double R = Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
	Dom.Circle(xy1,R,TRUE);
	Dom.LineTo(xy1,xy2);
	*pDomP << Dom;
	//
	strBuje	= "수직보강재";
	strMat	= pBSC->GetMaterialByMarkBuje(B_V);
	TextOutJewonAndMat(&Dom,xy2,strBuje,strMat);
	Dom.Rotate(xy2,pGir->GetLine()->GetAngleAzimuth(sta));

	*pDomP << Dom;
	*/
}

void CAPlateOutPlanGen::DimTextPyungFlange(CDomyun *pDomP, CPlateBasicIndex *pBxStt, 
										   CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();	
	CPlateGirderApp		*pGir		= pBxStt->GetGirder();	
	CSteelSection	*pBSC		= pStd->GetDataManage()->GetSteelSection();	

	BOOL bUpper = (nUpper==1) ? TRUE : FALSE;

	__int64 BxFlag = (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx1 = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	CPlateBasicIndex *pBx2 = pBxEnd;
	if(pBx1==NULL || pBx2==NULL) return;

	CDomyun Dom(pDomP);
	CDPoint xy1,xy2;
	double dist = 0;//GetDisCirclePoint(pBx1,nUpper);
	double sta1 = pBx1->GetStation();
	double sta2 = pBx2->GetStation();
	double staGisi = (sta1+sta2)/2;
	double staGisi2 = 0;

	double dEndDist = dLenGisi;

	CString strBuje	= _T("");
	CString strMat	= _T("");
	if(bUpper) 
	{
		CPlateBasicIndex *pBxS1 = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_UPPERFLANGE,pBx1,pBx2);
		CPlateBasicIndex *pBxS2 = Finder.GetBxNext();
		staGisi = (pBxS1->GetStation()+pBxS2->GetStation())/2;
		strBuje	= "상 판";		
		strMat	= pBSC->GetMaterialByMarkBuje(G_F_U);		

		//표시구간과 겹치는지를 CHECK!!!
		double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));
		staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
		double staGisiold = staGisi;
		double staGisi2old = staGisi2;
		CPlateBasicIndex *pBxS1old = pBxS1;
		CPlateBasicIndex *pBxS2old = pBxS2;
		while (1)
		{
			if (CheckStation(staGisi, staGisi2))
			{
				__int64 BxFlag2 = BX_CROSSBEAM_VBRACING | BX_VSTIFF;
				pBxS1 = pBxS2;
				pBxS2 = Finder.GetBxFirst(BxFlag2, pBxS1, pBx2, FALSE, FALSE);
				if (pBxS1==NULL || pBxS2==NULL)
					break;
				sta1 = pBxS1->GetStation();
				sta2 = pBxS2->GetStation();
				staGisi = (sta1+sta2)/2;
				staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
			}
			else
				break;
		}

		if (pBxS1==NULL || pBxS2==NULL)
		{
			staGisi = staGisiold;
			staGisi2 = staGisi2old;
			pBxS1 = pBxS1old;
			pBxS2 = pBxS2old;
		}
		xy1		= pGir->GetXyGirderDis(staGisi,dist,pBxS1->GetAngle());
		xy2		= pGir->GetXyGirderDis(staGisi,dEndDist);
	}
	else
	{
//		CPlateBasicIndex *pBxS1 = Finder.GetBxLast(BX_CROSSBEAM_VBRACING|BX_VSTIFF,pBx2,pBx1);
//		CPlateBasicIndex *pBxS2 = Finder.GetBxNext();
		CPlateBasicIndex *pBxS1 = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_LOWERFLANGE,pBx1,pBx2);
		CPlateBasicIndex *pBxS2 = Finder.GetBxNext();
		staGisi = (pBxS1->GetStation()+pBxS2->GetStation())/2;
		strBuje	= "하 판";		
		strMat	=  pBSC->GetMaterialByMarkBuje(G_F_L);

		//표시구간과 겹치는지를 CHECK!!!
		double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));
		staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
		double staGisiold = staGisi;
		double staGisi2old = staGisi2;
		CPlateBasicIndex *pBxS1old = pBxS1;
		CPlateBasicIndex *pBxS2old = pBxS2;
		while (1)
		{
			if (CheckStation(staGisi, staGisi2))
			{
				__int64 BxFlag2 = BX_CROSSBEAM_VBRACING | BX_VSTIFF;
				pBxS1 = pBxS2;
				pBxS2 = Finder.GetBxFirst(BxFlag2, pBxS1, pBx2, FALSE, FALSE);
				if (pBxS1==NULL || pBxS2==NULL)
					break;
				sta1 = pBxS1->GetStation();
				sta2 = pBxS2->GetStation();
				staGisi = (sta1+sta2)/2;
				staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
			}
			else
				break;
		}

		if (pBxS1==NULL || pBxS2==NULL)
		{
			staGisi = staGisiold;
			staGisi2 = staGisi2old;
			pBxS1 = pBxS1old;
			pBxS2 = pBxS2old;
		}

		xy1		= pGir->GetXyGirderDis(staGisi,dist,pBxS1->GetAngle());
		xy2		= pGir->GetXyGirderDis(staGisi,dEndDist);
	}	

	double R = Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
	Dom.Circle(xy1,R,TRUE);
	Dom.LineTo(xy1,xy2);
	*pDomP << Dom;
	
	m_JweonInfoStaArr.Add(CDPoint(staGisi, staGisi2));	
	TextOutJewonAndMat(&Dom,xy2,strBuje,strMat);
	Dom.Rotate(xy2,pGir->GetLine()->GetAngleAzimuth(staGisi));

	*pDomP << Dom;
}

BOOL CAPlateOutPlanGen::CheckStation(double sta1, double sta2)
{
	BOOL bCheck = FALSE;
	for (long n=0; n < m_JweonInfoStaArr.GetSize(); n++)
	{
		double dStaPre = m_JweonInfoStaArr[n].x;
		double dStaEnd = m_JweonInfoStaArr[n].y;

		if((dStaPre >= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd >= sta2) || 
		   (dStaPre <= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd >= sta2) ||
		   (dStaPre <= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd <= sta2) ||
		   (dStaPre >= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd <= sta2))
			bCheck = TRUE;
	}

	return bCheck;
}

void CAPlateOutPlanGen::TextOutJewonAndMat(CDomyun *pDomP, CDPoint xy,CString strBuje, CString strMat, BOOL bDirForward)
{
	double dLenStr  = max(pDomP->GetTextWidth(strBuje),pDomP->GetTextWidth(strMat));
	if(!bDirForward) dLenStr *= -1.0;

	pDomP->LineTo(xy.x,xy.y,xy.x+dLenStr,xy.y);
	if(strBuje.GetLength())
		pDomP->TextOut(xy.x+dLenStr/2,xy.y+pDomP->GetTextHeight()*1/4,strBuje);
	if(strMat.GetLength())
	{
		double dY = xy.y-pDomP->GetTextHeight()*5/4;
		if(strBuje.GetLength()==0)
			dY = xy.y+pDomP->GetTextHeight()*1/4;
		pDomP->TextOut(xy.x+dLenStr/2,dY,strMat);
	}
}
