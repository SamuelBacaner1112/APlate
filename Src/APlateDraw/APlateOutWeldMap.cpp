           // APlateOutWeldMap.cpp: implementation of the CAPlateOutWeldMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define JONG		10
#define JONG_TITLE	50
#define DANMYUN		200
#define MARK_SYSTEM	300
#define KEY_PLAN	400

#define DET_SCALE_SUMARY_HOR	1
#define DET_SCALE_SUMARY_VER	2
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutWeldMap::CAPlateOutWeldMap()
{

}

CAPlateOutWeldMap::~CAPlateOutWeldMap()
{

}
 
void CAPlateOutWeldMap::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->GetCurPage());
	CPlateGirderApp *pGir		= pBridge->GetGirder(-1);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nPage    = pStd->GetCurPage();
	long SubPage = nPage - m_nSttPage;
	double Scale  = GetScale();
	
	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	

	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
	//long nSttPage = pDomOpt->m_nDomPageArray.GetAt(PAGE_WELDMAP) == TRUE ? 0 : pDomOpt->m_nDomPageArray.GetAt(PAGE_WELDMAP) + SubPage - 1;	
	//pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sDomNumHead,"",nSttPage);
	pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);	
	pStd->AddMaterial(&Dom,Scale);
	pStd->AddJubujebubuje(&Dom,Scale);
	Dom.SetCalcExtRect();
	CDRect dgkRect = Dom.GetExtRect(), DomBound;
	*pDom << Dom;			

	CPlateBasicIndex *pBxStt = NULL, *pBxEnd = NULL;
	CPlateBasicIndex *pBxSttSangse = NULL, *pBxEndSangse = NULL;
	long nSubPage = pStd->m_nCurPage - m_nSttPage - 1;
//	EXIT_RTN;
	pPageMng->SetProgressData("측면도 출력중...",40);
	double MoveX=Dom.Always(100), MoveY=0;
	CDPoint xyMove = CDPoint(Dom.Always(100), dgkRect.Height()-Dom.Always(60));
	
	CDRect rectBase;
	for (long nBlock = 0; nBlock < pDomOpt->m_nWeldDivBlock; nBlock++)
	{
		if(GetPageIndex(nSubPage, nBlock, pBxStt,pBxEnd) == 0)
			break;
		if(nBlock==0)	pBxSttSangse = pBxStt;
		AddJong(&Dom, MoveX, MoveY+Dom.Always(450)-Dom.Always(150)*nBlock, pBxStt, pBxEnd);
		pBxEndSangse = pBxEnd;
		Dom.Move(pMove->GetPoint(JONG+nBlock));
		Dom.SetCalcExtRect();		
		CDRect rect = Dom.GetExtRect();
		if(nBlock==0) rectBase = rect;
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),JONG+nBlock,"측 면 도");
		*pDom << Dom;
	}

	DrawStd.AddTitleOnDom(&Dom, GetScale(), rectBase, "측 면 도",TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(JONG_TITLE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),JONG_TITLE,"측 면 도 제목");
	*pDom << Dom;

/*
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	*pDom << Dom;

	DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "측 면 도",TRUE, TRUE, 0);	

	Dom.SetCalcExtRect();		
	Dom.Move(pMove->GetPoint(JONG+1));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),JONG+1,"측면도 제목");
	*pDom << Dom;
*/
	if(pDomOpt->m_bWeldTypicalSection)
	{
//		EXIT_RTN;			
		pPageMng->SetProgressData("단면도 출력중...",50);						
		AddDanmyun(&Dom,Dom.Always(650),Dom.Always(475));
		*pDom << Dom;
	}

	if(pDomOpt->m_bWeldKeyPlan)
	{
//		EXIT_RTN;			
		pPageMng->SetProgressData("요약도 출력중...",80);						
		AddKeyPan(&Dom,Dom.Always(550),Dom.Always(100), nSubPage);
		*pDom << Dom;
	}
	
	if(pDomOpt->m_bWeldMarkSystem)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("MARK SYSTEM 출력중...",90);
		AddMarkSystem(&Dom, Dom.Always(630), Dom.Always(300));
		*pDom << Dom;
	}
	pPageMng->SetProgressData("출력 완료...",100);
}

long CAPlateOutWeldMap::GetPageSu()
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd		*pDomOpt  = pStd->m_pDomOptionStd;	
	CPlateBasicIndex *pBxStt = NULL, *pBxEnd = NULL;
	int nBlock = 0, nSubPage = 0, nTotalBlock = 0;
	do
	{
		for(nBlock = 0; nBlock < pDomOpt->m_nWeldDivBlock; nBlock++)	//한뷰에 표현될 블록수만큼 반복문
		{
			if(GetPageIndex(nSubPage, nBlock, pBxStt, pBxEnd) == 0)	
				break;		//마지막 블록인 상황	
			nTotalBlock++;			//전체 블록수 증가
		}
		nSubPage++;		//페이지수 증가
	}
	while(nBlock == pDomOpt->m_nWeldDivBlock);	//마지막 블록인 상황으로 인해 루프 탈출

	m_nPage = (nTotalBlock - 1) / pDomOpt->m_nWeldDivBlock + 1;
	return m_nPage;
}

double CAPlateOutWeldMap::GetScale()
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd		*pDomOpt	= pStd->m_pDomOptionStd;
	
	m_Scale = pDomOpt->m_nWeldDanmyunScale;
	return m_Scale;
}

CString CAPlateOutWeldMap::GetTitle()
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	CString szTitle;

	long nPageSu = m_nPage;
	if(nPageSu>1)
		szTitle.Format("용접 시험도 (%d/%d)",pStd->GetCurPage()-m_nSttPage,nPageSu);
	else
		szTitle.Format("용접 시험도");

	return szTitle;	
}

void CAPlateOutWeldMap::AddJong(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COptionStd		*pDomOpt	= pStd->m_pDomOptionStd;	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);		

	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	CPlateGirderApp	*pGir	= pBxStt->GetGirder();	
	pStd->m_pDomOptionStd->m_bWeldCamberLevel ? pGir->SetEleLevel(TRUE) : pGir->SetEleLevel(FALSE);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CAPlateDrawJong DrawJong(pStd->GetDataManage());	
	CAPlateDrawDetailDim  DetailDim(pStd,&Dom,pBxStt, pBxEnd);

	long nDir   = 0;
	long nLine  = 2;//pOpt->m_Gen_bBujeThick ? 2 : 0;
	CHAR cWeldType = 'A';

	//DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBxEnd, 0);
	//DrawJong.DrawPaveLine(&Dom,pBxStt,pBxEnd,nDir);
	DrawJong.DrawGirderLineBx(&Dom, pBxStt, pBxEnd, nLine);
//	DrawJong.DrawFactJongBx(&Dom,pBxStt,pBxEnd);
	DrawJong.DrawHGuessetBx(&Dom, pBxStt, pBxEnd);
	DrawJong.DrawCrossBeamBrkFlangeBx(&Dom, pBxStt, pBxEnd);
	DrawJong.DrawJackUpBx(&Dom,pBxStt,pBxEnd,nLine);
	DrawJong.DrawJiJumVStiffBx(&Dom,pBxStt,pBxEnd,nLine);
	DrawJong.DrawInOutGateBx(&Dom,pBxStt,pBxEnd, TRUE);
	DrawJong.DrawFieldSpliceHatchJong(&Dom,pBxStt,pBxEnd,nDir,FALSE,TRUE);
	DrawJong.DrawShoeMarkBx(&Dom,pBxStt,pBxEnd,m_Scale);
//	DrawJong.DrawStudBx(&Dom, pBxStt, pBxEnd, 0);
	if(!pDomOpt->m_bWeldLineType)
		pOpt->SetEnvType(&Dom, HCAD_STLH);
	DrawJong.DrawHStiffBx(&Dom,pBxStt,pBxEnd,nLine);
	DrawJong.DrawVStiffBx(&Dom,pBxStt,pBxEnd,nLine);

	// 치수 기입
	DimJongAllWeldMap(&Dom,pBxStt, pBxEnd,nDir,TRUE);
	DimJongAllWeldMap(&Dom,pBxStt, pBxEnd,nDir,FALSE);
	// Cutting 기호
	if(pDomOpt->m_nWeldMark==0)
	{
		DrawCuttingMarkJong(&Dom,pBxStt,pBxEnd,nDir,TRUE);
		DrawCuttingMarkJong(&Dom,pBxStt,pBxEnd,nDir,FALSE);
	}
	if(pDomOpt->m_nWeldMark==1)
	{
		DrawJong.AddWeldJongFlange(&Dom,pBxStt,pBxEnd,0,nDir, cWeldType, TRUE/*bGrindUp*/, TRUE/*bGrindDn*/);		
//		DrawCuttingGihoJong(p, pBxStt, pBxEnd, TRUE);
//		DrawCuttingGihoJong(p, pBxStt, pBxEnd, FALSE);
	}
/*	DrawJong.AddWeldJong(&Dom,pBxStt,pBxEnd,0,nDir,	FALSE);	
	DrawWebJewon(&Dom,pBxStt,pBxEnd,-1);
	DrawAbutPier(&Dom,pBxStt,pBxEnd);

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	DrawJong.DrawHStiffBxJewon(&Dom,pBxStt,pBxEnd,-1,FALSE);
	
	DetailDim.SetDirDimension(nDir);
	DetailDim.SetJongDimDan(2,TRUE);
	DetailDim.DimTotal(&Dom,pDomOpt->m_DetailDim_Jong_Upper,TRUE,FALSE,FALSE,FALSE);
	DetailDim.SetJongDimDan(3,FALSE);
	DetailDim.DimTotal(&Dom,pDomOpt->m_DetailDim_Jong_Lower,FALSE,FALSE,FALSE,FALSE);	

	DetailDim.DimJongSttEndSideBx(&Dom,pBxStt,pBxEnd);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER);
	if(pBx!=pBxStt)
		DetailDim.DimDetailSttJong(&Dom,pBxStt,pBxEnd,FALSE,FALSE,nDir,FALSE);

	pBx = Finder.GetBxFirst(BX_END_GIRDER);
	if(pBx!=pBxEnd)
		DetailDim.DimDetailEndJong(&Dom, pBxStt, pBxEnd,FALSE,FALSE,nDir);	

	DetailDim.DimDetailMLMarkJong(&Dom,pStd->m_nCurPage - m_nSttPage - 1, nDir);	
*/

	// 도면 회전각도
	CDPoint MoveXY;
	MoveXY.x = -pBxStt->GetStation();
	MoveXY.y = -pGir->GetElevationFlangeCamber(pBxStt->GetStation());
	Dom.Move(MoveXY);
	
	Dom.Move(xDis, yDis);

	//Dom.SetCalcExtRect();	
	//Dom.Move(pMove->GetPoint(JONG));
	//pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	//pMove->AddMPE(&Dom,Dom.GetExtRect(),JONG,"측면도");
	*pDom << Dom;
/*
	// 도면 기준점 영역이동	
	Dom.Move(pMove->GetPoint(WEB));	

	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();	
	// 도면 기준점 영역등록		
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),WEB,"측면도");	
	*pDom << Dom;
	
//	DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "측 면 도", TRUE, TRUE, 0, FALSE, "", HCAD_SUB_TITLE, Dom.Always(15));
	Dom.Move(pMove->GetPoint(WEB+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),WEB+1,"측면도 제목");
*/
}

CPlateBasicIndex* CAPlateOutWeldMap::GetBxSttPage(long nSubPage)
{	 
 	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	COptionStd		*pOpt  = pStd->m_pDomOptionStd;	
	CPlateBasicIndex		*pBxRet = NULL;

	long nType = pOpt->m_Det_nDivPage;
	long nCrossNum = pOpt->m_Det_nUnitEaDraw;
		
	if(nType==0)
	{
		long nG = nSubPage / pBridge->m_nQtyJigan;
		long nPier = nSubPage % pBridge->m_nQtyJigan - 1;
		if(nPier==0) 
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			return Finder.GetBxFirst(BX_STT_GIRDER);
		}
		return pBridge->GetGirder(nG)->GetBxOnJijum(nPier);		
	}
	else if(nType==1)
	{
		long nPage = 1;
		for(long nG=0;nG<pBridge->GetGirdersu();nG++)
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE | BX_STT_GIRDER);
			while(pBx)
			{
				if(nSubPage == nPage) return pBx;
				pBx = Finder.GetBxNext();
				nPage++;
			}
		}		
	}
	else if(nType==2)
	{
		long nAddNum = 0, nPreNum = 0;
		for(long nGir=0;nGir<pBridge->GetGirdersu();nGir++)
		{
			CPlateGirderApp *pGir= pBridge->GetGirder(nGir);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING);		
			long nCross  = (long)Finder.GetCountBx()-2;
			long nCount = (long)((nCross-1)/(nCrossNum-1));
			if((nCross-1)%(nCrossNum-1)) nCount++;
			nAddNum += nCount;
			if(nAddNum > nSubPage-1)				
			{			
				if(nSubPage-1 == nPreNum)
					pBxRet = pBx;
				else
					pBxRet = Finder.GetBxNext(nCrossNum + (nSubPage-nPreNum-2)*(nCrossNum-1));
				if(pBxRet!=NULL) break;
			}
			nPreNum = nAddNum;
		}
		return pBxRet;
	}
	else
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nSubPage));
		return Finder.GetBxFirst(BX_STT_GIRDER);
	}	
	return NULL;
}

CPlateBasicIndex* CAPlateOutWeldMap::GetBxEndPage(long nSubPage)
{	
 	CAPlateDrawStd *pStd	 = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	COptionStd *pOpt		 = pStd->m_pDomOptionStd;	
	CPlateBasicIndex *pBxRet = NULL;

	long nType = pOpt->m_Det_nDivPage;
	long nCrossNum = pOpt->m_Det_nUnitEaDraw;
		
	if(nType == 0)
	{
		long nG = nSubPage / pBridge->m_nQtyJigan;
		long nPier = nSubPage % pBridge->m_nQtyJigan;
		if(nPier==pBridge->m_nQtyJigan) 
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			return Finder.GetBxFirst(BX_END_GIRDER);
		}
		return pBridge->GetGirder(nG)->GetBxOnJijum(nPier);		
	}
	else if(nType == 1)
	{
		long nPage = 1;
		for(long nG=0;nG<pBridge->GetGirdersu();nG++)
		{		
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE | BX_END_GIRDER);
			while(pBx)
			{
				if(nSubPage == nPage) return pBx;
				pBx = Finder.GetBxNext();
				nPage++;
			}
		}
	}
	else if(nType == 2)
	{
		long nAddNum = 0, nPreNum = 0;
		long nGir = 0;
		for(nGir=0;nGir<pBridge->GetGirdersu();nGir++)
		{
			CPlateGirderApp		*pGir= pBridge->GetGirder(nGir);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING);
			long nCross  = (long)Finder.GetCountBx()-2;
			long nCount = (long)((nCross-1)/(nCrossNum-1));
			if((nCross-1)%(nCrossNum-1)) nCount++;
			nAddNum += nCount;
			if(nAddNum > nSubPage-1)					
			{
				if(nSubPage-1 == nPreNum)
					pBxRet = Finder.GetBxNext(nCrossNum);
				else
					pBxRet = Finder.GetBxNext(nCrossNum + (nSubPage-nPreNum-2)*(nCrossNum-1) + nCrossNum);	
				break;
			}
			nPreNum = nAddNum;
		}
		if(!pBxRet)
		{
			CPlateGirderApp *pGir= pBridge->GetGirder(nGir);
			CPlateBxFinder Finder(pGir);	
			pBxRet = Finder.GetBxLast(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING);
		}
//		if(pBxRet->IsAbutEnd())
//		{
//			CPlateGirderApp *pGir= pBridge->GetGirder(nGir);
//			CPlateBxFinder Finder(pGir);	
//			pBxRet = Finder.GetBxFirst(BX_END_GIRDER);
//		}
		return pBxRet;
	}	
	else
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nSubPage));
		return Finder.GetBxFirst(BX_END_GIRDER);
	}	
	return NULL;
}

void CAPlateOutWeldMap::AddKeyPan(CDomyun *pDom, double xDis, double yDis, long nSubPage)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint*	pMove		= pStd->GetMovePoint(pStd->GetCurPage());	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	long GirderSu = pBridge->GetGirdersu();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CDomyun Dom(pDom);	
	
	double dKeyScaleHor = pDomOpt->m_nWeldKeyPlanHor;
	double dKeyScaleVer = pDomOpt->m_nWeldKeyPlanVer;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	BOOL bSpliceMark = pDomOpt->m_bWeldKeyPlanSpliceMark;
	BOOL bJijumMark	 = pDomOpt->m_bWeldKeyPlanJijumMark;
	BOOL bCross		 = pDomOpt->m_bWeldKeyPlanCrossBeam;
	BOOL bDimension	 = pDomOpt->m_bWeldKeyPlanDimension;
	BOOL bDirection	 = pDomOpt->m_bWeldKeyPlanDirection;
	BOOL bOneLine	 = pDomOpt->m_bWeldKeyPlanOneLine;
	// 진행방향
	CString sBridgeDir	= pDomOpt->m_sTextBridgeDirection;
	CString sCounterDir = pDomOpt->m_sTextCounterDirection;

	Dom.SetScaleDim(dKeyScaleHor);

	CPlateBasicIndex *pBxStt = NULL;
	CPlateBasicIndex *pBxEnd = NULL;
	// KEY PLAN 그리기
	DrawPyung.AddKeyPlan(&Dom,pBxStt,pBxEnd,dKeyScaleHor,FALSE, bSpliceMark,bJijumMark, bDirection,FALSE,bCross,bOneLine,-1,-1,FALSE,FALSE,sBridgeDir,sCounterDir);	

	// HATCH 넣기
	for(long nG=0; nG<GirderSu; nG++) 
	{
		for(int nBlockSu = 0; nBlockSu<pDomOpt->m_nWeldDivBlock; nBlockSu++)
		{
			GetPageIndex(nSubPage, nBlockSu, pBxStt, pBxEnd, TRUE);
			DrawPyung.DrawHatchInRect(&Dom,pBxStt,pBxEnd, FALSE);
		}
	}

	// 치수선	
	if(bDimension)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
		CPlateBxFinder Finder(pBridge->GetGirder(-1));
		//
		__int64 nTypeUpper  = DIM_BASE_SPAN;
		long  nSttDan = 1;
		DrawDimPyung.DimPyungTotal(&Dom,Finder.GetBxFirst(BX_STT_GIRDER),
			 Finder.GetBxLast(BX_END_GIRDER),nTypeUpper,FALSE,FALSE,"",nSttDan);
	}

	// 도면 이동 및 회전, 스케일조정
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER);
	CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();

	ASSERT( pBx || pBxNext);
	double rotAng = DrawPyung.GetRotateAnglePyung(pBx,pBxNext);
	CDPoint MoveXY = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	Dom.Move(-MoveXY);	// 도로중심 (0,0)
	Dom.Rotate(rotAng,0,0);	

	BOOL bRotate = pDomOpt->m_bWeldKeyPlanRoVer;
	if (bRotate)
		Dom.Rotate(ConstPi/2, 0,0);

	Dom.SetCalcExtRect();	
	CDRect rectOutLine = Dom.GetExtRect();
/*	//if (pOption->m_KeyPlanbOutLine)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
		rectOutLine.NormalizeRect();
		rectOutLine.left -= Dom.Always(10);
		//if (pOption->m_Det_bKeyPlanRoVer)
		//	rectOutLine.left -= Dom.Always(40);
		rectOutLine.top -= Dom.Always(10);
		rectOutLine.right += Dom.Always(10);
		//if (pOption->m_Det_bKeyPlanRoVer)
		//	rectOutLine.bottom += Dom.Always(10);
		//else
			rectOutLine.bottom += Dom.Always(30);
		Dom.Rectangle(rectOutLine);
	}
*/
	Dom.RedrawByHVScale(dKeyScaleHor/dKeyScaleVer,FALSE,TRUE);
	Dom.RedrawByScale(m_Scale/dKeyScaleHor);	
	
	// 최초 기준점 이동
	Dom.Move(xDis,yDis);

	Dom.SetCalcExtRect();	
	Dom.Move(pMove->GetPoint(KEY_PLAN));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(),KEY_PLAN,"Key Plan");	
	*pDom << Dom;

//	pOpt->SetEnvType(&Dom,HCAD_SUB_TITLE);
//	pStd->AddTitleOnDom(&Dom, "요 약 도");

	// TITLE
	Dom.SetScaleDim(GetScale());
	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();
	rect.bottom -=Dom.Always(30);
	DrawStd.AddTitleOnDom(&Dom, dKeyScaleHor, rect, "요 약 도",TRUE, TRUE, 0, FALSE);

	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(KEY_PLAN+1));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),KEY_PLAN+1,"요약도 제목");
	*pDom << Dom;
}

void CAPlateOutWeldMap::AddMarkSystem(CDomyun *pDom, double xDis, double yDis)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
//	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint*	pMove		= pStd->GetMovePoint(pStd->GetCurPage());	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CString sBurf;	//TEMP CSTRING
	CDomyun Dom(pDom);
	Dom.SetScaleDim(m_Scale);

	//WRITE CONTENTS
	pOpt->SetEnvType(&Dom,HCAD_NOTE);
	Dom.SetTextAlignHorz(TA_LEFT);
	
	double dHBase = Dom.Always(-8);	//제목으로부터 떨어진 높이
	double dHText = -Dom.GetTextHeight()/2;
	double dHTextAll = -Dom.GetTextHeight()*1.8;
	double dH = dHBase + dHText;
	double dHTextS = dH*1.2;
	double dWText = 0;

	double dXDis = 0;
	double dXLineDis = Dom.Always(-3);
	double dYLineDis = Dom.Always(-10);
	sBurf = "X";
	dWText = Dom.GetTextWidth(sBurf)*0.8;
	Dom.TextOut(dXDis,dHBase, sBurf);
	Dom.LineTo(dXDis, dH, dWText, dH);
	Dom.MoveTo(dWText/2, dH);
	Dom.LineToEx(dXDis, dYLineDis);
	Dom.LineToEx(dXLineDis, 0);
	sBurf = "GIRDER NO. (TRANS.)";
	dWText = Dom.GetTextWidth(sBurf);
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dYLineDis, sBurf);
	sBurf = "(G1 ~ G3)";
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dHTextAll+dYLineDis, sBurf);

	dXDis = Dom.Always(5);
	dXLineDis = Dom.Always(-6);
	dYLineDis = Dom.Always(-25);
	sBurf = "01";
	dWText = Dom.GetTextWidth(sBurf);
	Dom.TextOut(dXDis, dHBase, sBurf);
	Dom.LineTo(dXDis, dH, dXDis+dWText, dH);
	Dom.MoveTo(dXDis+dWText/2, dH);
	Dom.LineToEx(0, dYLineDis);
	Dom.LineToEx(dXLineDis, 0);
	sBurf = "BOX NO. (LONGI.)";
	dWText = Dom.GetTextWidth(sBurf);
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dYLineDis, sBurf);
	sBurf = "(01 ~ 11)";
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dHTextAll+dYLineDis, sBurf);

	dXDis = Dom.Always(11);
	sBurf = "-";
	Dom.TextOut(dXDis, dHBase, sBurf);

	dXDis = Dom.Always(15);
	dXLineDis = Dom.Always(-7);
	dYLineDis = Dom.Always(-40);
	sBurf = "TF";
	dWText = Dom.GetTextWidth(sBurf);
	Dom.TextOut(dXDis, dHBase, sBurf);
	Dom.LineTo(dXDis, dH, dXDis+dWText, dH);
	Dom.MoveTo(dXDis+dWText/2, dH);
	Dom.LineToEx(0, dYLineDis);
	Dom.LineToEx(dXLineDis, 0);
	sBurf = "TF - TOP FLG";
	dWText = Dom.GetTextWidth(sBurf);
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dYLineDis, sBurf);
	sBurf = "BF - BOTTOM FLG";
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dHTextAll+dYLineDis, sBurf);
	sBurf = "LF - LEFT WEB";
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dHTextAll*2+dYLineDis, sBurf);
	sBurf = "RW - RIGHT WEB";
	Dom.TextOut(dXDis+dXLineDis-dWText, dHTextS+dHTextAll*3+dYLineDis, sBurf);
	
	dXDis = Dom.Always(20);
	sBurf = "-";
	Dom.TextOut(dXDis, dHBase, sBurf);
	
	dXDis = Dom.Always(25);
	dXLineDis = Dom.Always(9);
	dYLineDis = Dom.Always(-50);
	sBurf = "R";
	dWText = Dom.GetTextWidth(sBurf)*0.8;
	Dom.TextOut(dXDis, dHBase, sBurf);
	Dom.LineTo(dXDis, dH, dXDis+dWText, dH);
	Dom.MoveTo(dXDis+dWText/2, dH);
	Dom.LineToEx(0, dYLineDis);
	Dom.LineToEx(dXLineDis, 0);
	sBurf = "R - RT";
	Dom.TextOut(dXLineDis+dXDis+Dom.Always(4), dHTextS+dYLineDis, sBurf);

	dXDis = Dom.Always(30);
	sBurf = "-";
	Dom.TextOut(dXDis, dHBase, sBurf);

	dXDis = Dom.Always(35);
	dXLineDis = Dom.Always(7);
	dYLineDis = Dom.Always(-30);
	sBurf = "T";
	dWText = Dom.GetTextWidth(sBurf)*0.8;
	Dom.TextOut(dXDis, dHBase, sBurf);
	Dom.LineTo(dXDis, dH, dXDis+dWText, dH);
	Dom.MoveTo(dXDis+dWText/2, dH);
	Dom.LineToEx(0, dYLineDis);
	Dom.LineToEx(dXLineDis, 0);
	sBurf = "인장 (T)";
	Dom.TextOut(dXLineDis+dXDis+Dom.Always(4), dHTextS+dYLineDis, sBurf);
	sBurf = "압축 (C)";
	Dom.TextOut(dXLineDis+dXDis+Dom.Always(4), dHTextS+dHTextAll+dYLineDis, sBurf);
	sBurf = "교번 (B)";
	Dom.TextOut(dXLineDis+dXDis+Dom.Always(4), dHTextS+dHTextAll*2+dYLineDis, sBurf);
	
	dXDis = Dom.Always(40);
	sBurf = "-";
	Dom.TextOut(dXDis, dHBase, sBurf);
	
	dXDis = Dom.Always(45);
	dXLineDis = Dom.Always(7);
	dYLineDis = Dom.Always(-20);
	sBurf = "01";
	dWText = Dom.GetTextWidth(sBurf)*0.8;
	Dom.TextOut(dXDis, dHBase, sBurf);
	Dom.LineTo(dXDis, dH, dXDis+dWText, dH);
	Dom.MoveTo(dXDis+dWText/2, dH);
	Dom.LineToEx(0, dYLineDis);
	Dom.LineToEx(dXLineDis, 0);
	sBurf = "SERIAL NO";
	Dom.TextOut(dXLineDis+dXDis+Dom.Always(4), dHTextS+dYLineDis, sBurf);

	// WRITE TITLE "MARK SYSTEM"
	pOpt->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	sBurf = "MARK SYSTEM";

	Dom.Move(xDis, yDis);

	// MOVE AND REGIST
	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(MARK_SYSTEM));		
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MARK_SYSTEM,"MARK SYSTEM");
	*pDom << Dom;

	// TITEL
	Dom.SetScaleDim(GetScale());
	Dom.SetCalcExtRect();	
	DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), "MARK SYSTEM",TRUE, FALSE, 0, FALSE);

	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(MARK_SYSTEM+1));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MARK_SYSTEM+1, "MARK SYSTEM 제목");
	*pDom << Dom;
}

void CAPlateOutWeldMap::AddDanmyun(CDomyun *pDom, double xDis, double yDis)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir		= pBridge->GetGirder(-1);	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->GetCurPage());
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(0);	
	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	double dThickTop = pGir->GetThickFactByStation(pBx->GetStation(), 0);
	double dThickBot = pGir->GetThickFactByStation(pBx->GetStation(), 1);
	double dThickWeb = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dHeight1   = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dHeight2   = pGir->GetHeightMaxByJijum();
	double dWidthUp  = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWidthLo  = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
	double dTotalH = dThickTop + dHeight1 + dThickBot;

	double dScale = pDomOpt->m_nWeldTypicalScale;

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetScaleDim(dScale);

	Dom.Rectangle(-dWidthUp/2, 0, dWidthUp/2, -dThickTop);	//상판
	Dom.Rectangle(-dThickWeb/2, -dThickTop, dThickWeb/2, -dThickTop-dHeight1);	//복부판
	Dom.Rectangle(-dWidthLo/2, -dThickTop-dHeight1, dWidthLo/2, -dThickTop-dHeight1-dThickBot);	//하판

	pOpt->SetEnvType(&Dom,HCAD_NOTE);
	Dom.TextOut(dWidthUp/2+Dom.Always(10), -dThickTop, "상판");
	Dom.TextOut(dWidthUp/2+Dom.Always(10), -dTotalH/2, "복부판");
	Dom.TextOut(dWidthUp/2+Dom.Always(10), -dTotalH+dThickBot, "하판");

	Dom.SetDimExo(5);
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-dWidthUp/2, 0, 0);
	Dom.DimLineTo(dWidthUp, 0, COMMA(dWidthUp));
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(-dWidthLo/2, -dTotalH, 0);
	Dom.DimLineTo(dWidthLo, 0, COMMA(dWidthLo));
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(-dWidthLo/2, -dTotalH+dThickBot, 0);
	Dom.DimLineTo(dHeight1, 0, COMMA(dHeight1)+" ~ "+COMMA(dHeight2));

	Dom.RedrawByScale(GetScale()/dScale);
	Dom.Move(xDis,yDis);
	
	// MOVE AND REGIST
	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(DANMYUN));		
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),DANMYUN,"단면도");
	*pDom << Dom;

	// TITEL
	Dom.SetScaleDim(GetScale());
	Dom.SetCalcExtRect();	
	DrawStd.AddTitleOnDom(&Dom, dScale, Dom.GetExtRect(), "단면도",TRUE, TRUE, 0, FALSE);
	
	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(DANMYUN+1));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), DANMYUN+1, "단면도 제목");
	*pDom << Dom;
}
//	nSubPage	: 이 도면종류의 선두를 0으로 한 페이지 위치
//	nBlock		: 한 페이지에서 거더 그림 위치
//	return		: 위의 해당 Block에 해당되는 시작 끝 Bx, 그리고 존재유무
long CAPlateOutWeldMap::GetPageIndex(long nSubPage, long nBlock, CPlateBasicIndex *&pBxStt, CPlateBasicIndex *&pBxEnd, BOOL bKeyPlan)
{
	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CPlateGirderApp *pGir = pBridge->GetGirder(-1);
	long nSumBlock = nSubPage * pDomOpt->m_nWeldDivBlock + nBlock;	//총블록수의 합계

	long nDivSu = pDomOpt->m_nWeldDivDomyun;
	long nGSu =  pDomOpt->m_bWeldLeftGirder ? 1 : pBridge->GetGirdersu();	//거더수의 결정
	long nG = 0;

	///////////////////////////////
	// 자동분할
	///////////////////////////////
	// 현장이음 개수기준 분할
	if(pDomOpt->m_bWeldDivDomyun)
	{
		long nCur = 0, nPre = 0;
		for (nG = 0; nG < pBridge->GetGirdersu(); nG++)
		{
			if(bKeyPlan)	// KEY PLAN 인 경우
				pGir = pBridge->GetGirder(nG);
			else if(pDomOpt->m_bWeldLeftGirder)
				pGir = pBridge->GetGirder(-1);	//중앙 거더
			else
				pGir = pBridge->GetGirder(nG);
			nCur += pGir->GetQtySplice() / nDivSu + 1;	//한거더당 나오는 블록수
			if (nCur > nSumBlock)
				break;
			nPre = nCur;
		}
		if (nG >= nGSu)
			return 0;
		long nSp = (nSumBlock - nPre) * nDivSu;
		CPlateBxFinder Finder(pGir);
		Finder.GetBxFirst(BX_STT_GIRDER | BX_SPLICE | BX_END_GIRDER);
		pBxStt = Finder.GetBxNext(nSp);
		pBxEnd = Finder.GetBxNext(nDivSu);
		if (!pBxEnd)
			pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	}
	else	/////////////////////////////////
	{		// 분할하지 않음
		nG = nSumBlock;
		if (nG >= nGSu)
			return 0;

		if(bKeyPlan)
			pGir = pBridge->GetGirder(nG);
		else if(pDomOpt->m_bWeldLeftGirder)
			pGir = pBridge->GetGirder(-1);	//중앙거더
		else
			pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	}
	return 1;
}

void CAPlateOutWeldMap::DimJongAllWeldMap(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDir, BOOL bUpper)
{	
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOStd = (CARoadOptionStd*)pStd->m_pOptStd;
//	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	COptionStd *pDomOpt = pStd->GetDomOptionStd();
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBxStt->GetGirder();
	CLineInfo *pLine = pGir->GetLine();	
	CPlateBxFinder Finder(pGir);

	//SET DIM
	CDimDomyunEx Dom(pDom);
	pOStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDimDir(DIR_NONE);
	long nDan = 2;	
	if(!bUpper)
		nDan = 2;
	//SET STT,END POINT
	__int64 BxFlag;
	double SttSta = pBxStt->GetStation();
	double EndSta = pBxEnd->GetStation();					 		
	double SttY, EndY, MidY, y, Length, Height;
//	long nAlwaysHeight = bUpper ? 8 : 18;
//	double Dis = Dom.Always(Dom.GetDimVLen()+Dom.GetDimExo());
	//SET DIM HEIGHT
	Height = 0;//Dom.Always(nAlwaysHeight) + Dom.Always(Dom.GetDimExo());
	CString strT;
	
	//ELEVATION OF STT,END
	SttY = pGir->GetElevationFlangeCamber(SttSta, 0, CDPoint(0,1), bUpper);
	EndY = pGir->GetElevationFlangeCamber(EndSta, 0, CDPoint(0,1), bUpper);
	MidY = (SttY + pGir->GetElevationFlangeCamber(SttSta, 0, CDPoint(0,1), FALSE))/2;
	
	if(bUpper)	//UPPER
	{
		Dom.SetDirection("TOP");			
		y = SttY > EndY ? SttY : EndY;	//SET MORE HIGHER ELEVATION
		Dom.DimBasePoint(SttSta, y+Height);	//SET BASE DIMPOINT
		Dom.DimMoveTo(SttSta, y+Height, nDan, FALSE, "상판 두께", Dom.Always(40), TRUE);
		Dom.TextOut(SttSta+Dom.Always(-30), MidY, "복부판 두께");
	}
	else	//LOWER
	{
		Dom.SetDirection("BOTTOM");			
		y = SttY > EndY ? EndY : SttY;		
		Dom.DimBasePoint(SttSta, y-Height);
		Dom.DimMoveTo(SttSta, y-Height, nDan, FALSE, "하판 두께", Dom.Always(40), TRUE);
	}
	
	//SET BxFlage
	if(bUpper)	BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;
	else		BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE;	

	//SET FINDER
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);	

	//CHECK FOR JUMP BX_SPLICE
	while(pBx)
	{
//		 if(!pBx->IsState(BX_SPLICE)) 
		 {
			//DUE TO DimMoveTo
			if(!pBx->IsState(BX_STT_GIRDER))	//OTHER GIRDER
			{
				CPlateBasicIndex *pPreBx = pBx->GetFactChain(bUpper ? G_F_U : G_F_L,FALSE)->GetBx();		
				Dom.DimMoveTo(SttSta, y,nDan,FALSE,"",0,TRUE);				
				Length = GetDisWebBySta(pBx->GetNumberGirder(),pPreBx->GetStation(),pBx->GetStation(),nDir);
				y = pGir->GetElevationFlangeCamber(pBx->GetStation(), 0, CDPoint(0,1), bUpper);		
				strT.Format("T = %.0f",pBx->GetFactChain(bUpper ? G_F_U : G_F_L,FALSE)->m_dFactChainThick);		
				double AddLen = GetDisWebBySta(pBx->GetNumberGirder(),SttSta,pBx->GetStation(),nDir);
				if(pDomOpt->m_bWeldDimText)
					Dom.DimLineTo(AddLen, y,strT);
				else
					Dom.DimLineTo(AddLen, y,COMMA(Length),strT);
			}
			else
				Dom.DimMoveTo(SttSta, y, nDan);	//STT GIRDER		
			 break;
		 }
		 pBx = Finder.GetBxNext();
	}

	while(Finder.GetBxFixNext())	
	{		
		CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
		Length = GetDisWebBySta(pBx->GetNumberGirder(),pBx->GetStation(),pBxNext->GetStation(),nDir);
		y = pGir->GetElevationFlangeCamber(pBxNext->GetStation(), 0, CDPoint(0,1), bUpper);

		strT.Format("T = %.0f",pGir->GetThickJewon(bUpper ? G_F_U : G_F_L, pBx));
		if(pDomOpt->m_bWeldDimText)
			Dom.DimLineTo(Length, y, strT);
		else
			Dom.DimLineTo(Length, y,COMMA(Length),strT);

		pBx = Finder.GetBxNext();
	}

	if(pBx != pBxEnd && pBx)
	{				
		CPlateBasicIndex *pNextBx = pBx->GetFactChain(bUpper ? G_F_U : G_F_L,TRUE)->GetBxEnd();	
		if(pNextBx)
		{
			Length = GetDisWebBySta(pBx->GetNumberGirder(),pBx->GetStation(),pNextBx->GetStation(),nDir);
			y = pGir->GetElevationFlangeCamber(pBx->GetStation(), 0, CDPoint(0,1), bUpper);

			strT.Format("(T = %.0f)",pGir->GetThickJewon(bUpper ? G_F_U : G_F_L, pBx));	
			double AddLen = GetDisWebBySta(pBx->GetNumberGirder(),pBx->GetStation(),EndSta,nDir);
			if(pDomOpt->m_bWeldDimText)
				Dom.DimLineTo(AddLen, y,strT,"",FALSE,FALSE,0,TRUE);
			else
				Dom.DimLineTo(AddLen, y,COMMA(Length),strT,FALSE,FALSE,0,TRUE);		
		}
	}

	*pDom << Dom;	
}

double CAPlateOutWeldMap::GetDisWebBySta(long nG, double SttSta, double EndSta, long nDir)
{
	CAPlateDrawStd	*pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp		*pGir  = pBridge->GetGirder(nG);	
	CLineInfo		*pLine = pGir->GetLine();	


	return pLine->GetLengthByDis(SttSta,EndSta,pGir->m_dGirderCenter);
}

void CAPlateOutWeldMap::DrawCuttingMarkJong(CDomyun *pDom, CPlateBasicIndex *pBxStt,CPlateBasicIndex *pBxEnd,long nDir,BOOL bUpper)
{
	CAPlateDrawStd* pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir  = (CPlateGirderApp*)pBxStt->GetGirder();
	CARoadOptionStd* pOpt = (CARoadOptionStd*)pStd->m_pOptStd;

	//SET FINDER
	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	//SET SymbolDom
	CSymbolDom SyDom(pDom, pOpt);	
	pOpt->SetEnvType(&SyDom,HCAD_SYMB);

	CDPoint StaCur;	//VAR CURTTING MARK OF CENTER

	//SET STT,END POINT
	double SttSta = pBxStt->GetStation();
	double EndSta = pBxEnd->GetStation();
	double SttY, EndY, y, Length;
	//SET MARK DIM
	long nDan = 3;
	if(!bUpper)
		nDan = 3;
	double Height = SyDom.Always(SyDom.GetDimExo()) + SyDom.Always(nDan*SyDom.GetDimVLen());
	double dRadius = SyDom.Always(3);
	
	//ELEVATION OF STT,END
	SttY = pGir->GetElevationFlangeCamber(SttSta, 0, CDPoint(0,1), bUpper);
	EndY = pGir->GetElevationFlangeCamber(EndSta, 0, CDPoint(0,1), bUpper);
	
	//BASE Y  BASE X = SttSta;
	if(bUpper)	//UPPER
	{
		y = SttY > EndY ? SttY : EndY;
		y =  Height + y;
	}
	else	//LOWER
	{
		y = SttY > EndY ? EndY : SttY;		
		y =  -Height + y;
	}

	//CHECK FOR JUMP BX_SPLICE
/*
	while(pBx)
	{
		 if(!pBx->IsState(BX_SPLICE)) break;
		 pBx = Finder.GetBxNext();
	}
*/

	double dTempStore = 0;

	//FIRST
	if(pBx)
	{
		StaCur.x = SttSta + GetDisWebBySta(pBx->GetNumberGirder(), SttSta, pBx->GetStation(), nDir);
		StaCur.y = y;		
		SyDom.DrawSymbolCuttingMark(StaCur, dRadius);

		//MARK LINE AND TEXT
		DrawCuttingLineAndText(pDom, pBx, StaCur, y, bUpper);

		*pDom << SyDom;
		dTempStore = StaCur.x;	//Temp
	}

	//THE OHTER
	while(Finder.GetBxFixNext())
	{
		CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
		Length = GetDisWebBySta(pBx->GetNumberGirder(),pBx->GetStation(),pBxNext->GetStation(),nDir);
		StaCur.x = dTempStore + Length;
		StaCur.y = y;
		SyDom.DrawSymbolCuttingMark(StaCur, dRadius);
		dTempStore = StaCur.x;
		//MARK LINE AND TEXT
		DrawCuttingLineAndText(pDom, pBxNext, StaCur, y, bUpper);
		*pDom << SyDom;
		pBx = Finder.GetBxNext();
	}
}

void CAPlateOutWeldMap::DrawCuttingLineAndText(CDomyun *pDom, CPlateBasicIndex *pBx, CDPoint &StaCur, double y, BOOL bUpper)
{
	CAPlateDrawStd* pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CARoadOptionStd* pOpt = pStd->m_pOptStd;

	CSymbolDom SyDom(pDom, pOpt);	
	pOpt->SetEnvType(&SyDom,HCAD_SYMB);
	SyDom.SetScaleDim(pDom->GetScaleDim());	

	CDPoint MarkLineY(SyDom.Always(0), SyDom.Always(20));	//MARKLINE LENGTH Y
	CDPoint MarkLineX(SyDom.Always(6), SyDom.Always(0));	//MARKLINE LENGTH X
	CDPoint MarkLinePosY(0,0);	//MARKLINE POSITION Y
	CDPoint MarkLinePosX(0,0);	//MARKLINE POSITION X

	StaCur.y = bUpper? y - SyDom.Always(8) : y + SyDom.Always(8);
	if(bUpper)
	{
		MarkLinePosY = StaCur+MarkLineY;
		MarkLinePosX = MarkLinePosY+MarkLineX;
		SyDom.LineTo(StaCur, MarkLinePosY);
		SyDom.LineTo(MarkLinePosY, MarkLinePosX);
	}
	else
	{
		MarkLinePosY = StaCur-MarkLineY;
		MarkLinePosX = MarkLinePosY+MarkLineX;
		SyDom.LineTo(StaCur, MarkLinePosY);
		SyDom.LineTo(MarkLinePosY, MarkLinePosX);
	}

	//SET MARK TEXT
	MarkLinePosX.x += SyDom.Always(16);
	MarkLinePosX.y += SyDom.Always(-1);
	SyDom.SetTextWidthFactor(0.7);
//	SyDom.TextOut(MarkLinePosX, pBridge->GetMarkWeldDetail(bUpper? WELD_FU : WELD_FL, pBx));
	MarkLinePosX.x += SyDom.Always(1);
	MarkLinePosX.y += SyDom.Always(-4);
//	SyDom.TextOut(MarkLinePosX, "("+pBridge->GetMarkWeld(bUpper? WELD_FU : WELD_FL, pBx)+")");
	SyDom.SetTextWidthFactor(1);

	*pDom << SyDom;
}
