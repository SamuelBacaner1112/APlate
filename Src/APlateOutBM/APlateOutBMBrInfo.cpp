// APlateOutBMBrInfo.cpp: implementation of the CAPlateOutBMBrInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutBM.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutBMBrInfo::CAPlateOutBMBrInfo(CAPlateOutBMStd *pAPlateOutBMStd)
{
	m_szBridgeName			= _T("");	// 교량파일 이름
	m_szBridgePath			= _T("");	// 교량파일 경로
	m_szBridgeDir			= _T("");	// 교량 진행방향
	m_bOutTotSlab			= FALSE;	// 슬래브수량총괄 출력여부
	m_bOutTotRebar			= TRUE;		// 철근수량총괄 출력여부
	m_bOutRebar				= TRUE;		// 철근 집계표 출력여부
	m_bOutSlab				= TRUE;		// 슬래브 수량 출력여부
	m_bOutCalcFoundation	= TRUE;		// 산출근거
	m_nSpan					= 0;		// 
	m_pAPlateOutBMStd		= pAPlateOutBMStd;
}

CAPlateOutBMBrInfo::~CAPlateOutBMBrInfo()
{

}

void CAPlateOutBMBrInfo::GetSlabHunch(CPlateBasicIndex *pBx, CVectorArray &ptArr)
{
	CPlateBridgeApp	*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pCRGir		= pDB->GetGirder(-1);
	CPlateGirderApp	*pGirL		= pDB->GetGirder(0);	//1번 거더부터 시작함
	CPlateGirderApp	*pGirR		= pDB->GetGirder(pDB->GetGirdersu()-1);	//1번 거더부터 시작함

	BOOL bIsJijum = pBx->IsJijum();//단부이면...

	CDPoint APave[HDANLINESU], ALeft[4], ARigh[4];
	CDPoint xyLineCenter;
	CDPointArray xyArrUpper, xyArrLower;
	pDB->GetXyPaveAct(pBx->GetStation(), APave);	// 슬래브상면의 좌표
	long n = 0; 
	for (n=0; n<HDANLINESU; n++)
		xyArrUpper.Add(APave[n]);
	
	xyLineCenter.x = APave[0].x + fabs(pDB->GetDisSlabLeftDom(pBx));//선형중심에서 좌측슬래브까지의 거리..

	CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pGirL);
	pDB->GetXyFrontCantilever(pBxLeft, ALeft, TRUE);
	CPlateBasicIndex *pBxRigh = pBx->GetBxMatchByCrossBeam(pGirR);
	pDB->GetXyFrontCantilever(pBxRigh, ARigh, FALSE);

	for (n=0; n<2; n++)
		xyArrLower.Add(ALeft[n]);
	for (n=0; n<3; n++)
		xyArrLower.Add(ARigh[n]);	

	CDPoint A[10];
	for(long nG = 0; nG < pDB->GetGirdersu()-1 ; nG++)	
	{
		CPlateGirderApp *pGirCur = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch  = pGirCur->GetBxMatchByDiap(pBx, pGirCur);
		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE);//거더높이옵션 적용...
				
		xyArrLower.Add(A[0]);

		if(bIsJijum)
		{
			xyArrLower.Add(A[3]);
			xyArrLower.Add(A[6]);
		}
		else
		{
			BOOL bIsNull = FALSE;
			for (n=3; n<7; n++)
			{
				bIsNull = A[n].IsNull();
				if (!bIsNull)
					xyArrLower.Add(A[n]);		
			}
		}			
	}

	//선형중심선 찾아서 추가...	
	xyArrLower.RemoveSameValue();	
	xyArrLower.Sort(TRUE, TRUE);

	double  d1 = 0.0;
	double  d2 = 0.0;
	CDPoint xyLower = (LPARAM)0.0;
	for(n=0; n<xyArrLower.GetSize()-1; n++)
	{
		xyLower = xyArrLower[n];
		d1		= xyLower.x;
		xyLower = xyArrLower[n+1];
		d2		= xyLower.x;
		if(xyLineCenter.x == d1 || (xyLineCenter.x > d1 && xyLineCenter.x < d2))
		{
			xyLower = xyArrLower[n];
			xyLineCenter.y = xyLower.y;
			xyArrLower.Add(xyLineCenter);
			break;
		}
		
	}

	xyArrUpper.RemoveSameValue();
	xyArrLower.RemoveSameValue();
	xyArrUpper.Sort(TRUE, TRUE);
	xyArrLower.Sort(TRUE, TRUE);

	xyArrUpper = pDB->m_pDataManage->GetDeckData()->GetCDPointArrByPolyLine(xyArrLower, xyArrUpper, TRUE);

	double dHeight = 0;
	double dThickPave = pDB->GetThickPave();
	for (n=0; n<xyArrUpper.GetSize(); n++)
	{
		xyLower = xyArrLower[n];
		//값에 포장의 두께가 포함되어 있으므로 두께빼줌
		dHeight = ~(xyArrUpper[n]-xyArrLower[n]) - dThickPave;					
		ptArr.Add(CVector(xyLower.x, xyLower.y, dHeight));
	}
}

void CAPlateOutBMBrInfo::DrawSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd	*pOptStd	= m_pAPlateOutBMStd->GetDataManage()->GetOptionStd();
	CVectorArray ptArr;

	GetSlabHunch(pBx, ptArr);
	
	CDomyun Dom(pDom);
	Dom.SetTextHeight(2);

	Dom.SetTextAlignHorz(TA_CENTER);
	CString	sText	= _T("");
	long	n		= 0;

	for(n=0; n<ptArr.GetSize(); n++)
	{						
		Dom.LineTo(ptArr[n].x, ptArr[n].y,ptArr[n].x, ptArr[n].y+ptArr[n].z);		
		sText.Format("H%d", n+1);
		if(n == 0)
			Dom.TextOut(ptArr[n].x-Dom.GetTextWidth(sText), ptArr[n].y+ptArr[n].z/2-Dom.GetTextHeight()/2, sText);
		else
			Dom.TextOut(ptArr[n].x+Dom.GetTextWidth(sText), ptArr[n].y+ptArr[n].z/2-Dom.GetTextHeight()/2, sText);
	}
	*pDom << Dom;

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextHeight(2);
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(ptArr[0].x, ptArr[0].y, 0, "");

	CDPoint xyCurr, xyNext;
	for(n=0; n<ptArr.GetSize()-1; n++)
	{
		xyCurr	= ptArr[n];
		xyNext	= ptArr[n+1];
		double	dDis	= fabs(xyNext.x-xyCurr.x); 
		sText.Format("L%d", n+1);
		Dom.DimLineTo(dDis, 0, sText);
	}	

	*pDom << Dom;
}

void CAPlateOutBMBrInfo::DrawMainDomyun(CDomyun *pDom, long nSectionPos)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateBasicIndex	*pBx		= GetBxSection(nSectionPos);
	CPlateGirderApp		*pGir		= pDB->GetGirder(pBx->GetNumberGirder());

	CAPlateDrawDanmyun DrawDanmyun(m_pAPlateOutBMStd->GetDataManage());
	CDomyun Dom(pDom);

	
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, FALSE, CAPlateDrawCross::UPPER);	// 슬래브 단면
	
	DrawSlabHunch(&Dom, pBx);

	Dom.SetCalcExtRect();
	CDRect	rect;
	rect = Dom.GetExtRect();

	double dTrimHeight	= pGir->GetHeightGirderBySta(pBx->GetStation())*1/10;//pGir->GetHeightBox(pBx->GetStation())*4/5;

 	Dom.CutLightning(rect.left-Dom.Always(2), rect.top+dTrimHeight, rect.right+Dom.Always(2), rect.top+dTrimHeight, FALSE);

	*pDom << Dom;
}

void CAPlateOutBMBrInfo::DrawCrossRebarBindConc(CDomyun *pDom)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CADeckData			*pDeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CDataManage			*pDataMng	= m_pAPlateOutBMStd->GetDataManage();
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);

	CAPlateDrawDanmyun	DrawDanmyun(pDataMng);
	CAPlateDrawStandard	DrawStd(pDataMng);
	CDrawDBBaseRebarTU	DBRebarTU(pDataMng);
	
	BOOL	bShowMark	= FALSE;//pDeckData->m_bShowMark && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag() ? TRUE : FALSE;
	CString	sText		= _T("");

	CDomyun Dom(pDom);
	pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);

	// Draw Bind Concrete & Girder
	double dWebT		= pGir->GetValueBasicDefault("WEB_T");
	double dFlangeTUp	= pGir->GetValueBasicDefault("FLANGE_T");
	double dFlangeTDn	= pGir->GetValueBasicDefault("FLANGE_T");
	double dBindW		= pDB->m_BindConc.m_dWidth;
	double dBindH		= pDB->m_BindConc.m_dHeight;
	double dBindS1		= pDB->m_BindConc.m_dDeep;
	double dBindS2		= pDB->m_BindConc.m_dDis;
	double dSlabLowH	= -pDB->m_dThickSlabBase;	// 슬래브 하면의 높이
	double dFlangH		= dSlabLowH - (dBindH-dBindS1-dBindS2);
	double dGirderCenter= pGir->m_dGirderCenter;
	double dHeight		= pGir->GetHeightGirderByJijum(0);
	double dUWidth		= pGir->GetValueBasicDefault("FLANGE_W");
	double dLWidth		= pGir->GetValueBasicDefault("FLANGE_W");

	Dom.Rectangle(dGirderCenter-dUWidth/2, dFlangH, dGirderCenter+dUWidth/2, dFlangH-dFlangeTUp);
	Dom.Rectangle(dGirderCenter-dWebT/2,   dFlangH-dFlangeTUp, dGirderCenter + dWebT/2, dFlangH-dFlangeTUp-dHeight);
	Dom.Rectangle(dGirderCenter-dLWidth/2, dFlangH-dFlangeTUp-dHeight, dGirderCenter+dLWidth/2, dFlangH-dFlangeTUp-dFlangeTDn-dHeight);

	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dSlabLowH+dBindS1);
	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter-dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter-dBindW/2, dFlangH-dBindS2,	dGirderCenter-dWebT/2,  dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dFlangH-dBindS2,	dGirderCenter+dWebT/2,  dFlangH-dBindS2);

	// Rebar
	pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);

	DBRebarTU.DrawBincConcRebarAtCross(&Dom, pGir->GetBxOnJijum(0), TRUE);
	DBRebarTU.DimBincConcRebarAtCross(&Dom, 0, TRUE);

	*pDom << Dom;
}

CPlateBasicIndex *CAPlateOutBMBrInfo::GetBxSection(long nSectionPos)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateBasicIndex *pBx	= NULL;
	
	if(nSectionPos==SECTION_DANBU_STT)		// 슬래브의 시점위치
	{
		return pDB->GetGirder(-1)->GetBxOnJijum(0);		
	}
	else if(nSectionPos==SECTION_DANBU_END)		// 슬래브의 시점위치
	{
		return pDB->GetGirder(-1)->GetBxOnJijum(pDB->m_nQtyJigan);		
	}
	else if(nSectionPos==SECTION_GEN)//일반부 테이퍼부분 바로 다음 bx찾기..
	{
		CPlateGirderApp *pGir1= pDB->GetGirder(0);
		CPlateBxFinder Finder(pGir1);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		if(pDB->m_dHeightTaperStt>0)
		{
			while (pBx) 
			{				
				//double dHeight = pGir1->GetHeightTaper(pBx->GetStation());
				if(pGir1->IsExistTaperArea(pBx, 0))//테이퍼부에 속하면..				
					pBx = Finder.GetBxNext();
				else	break;				
			}
		}
		else
		{
			pBx	= Finder.GetBxNext(2);
		}

		return pBx;
	}
	if(nSectionPos==SECTION_JIJUM && pDB->m_nQtyJigan>1) 
		return pDB->GetGirder(-1)->GetBxOnJijum(1);

	ASSERT(FALSE);
	return NULL;
}

void CAPlateOutBMBrInfo::DrawSlabHunchGen(CDomyun *pDom)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CARoadOptionStd		*pOptStd	= m_pAPlateOutBMStd->GetDataManage()->GetOptionStd();	
	CAPlateDrawPyung	DBPlan(m_pAPlateOutBMStd->GetDataManage());
	CPlateBasicIndex	*pBxStt		= NULL;
	CPlateBasicIndex	*pBxEnd		= NULL;
	CPlateBasicIndex	*pBxLeftStt	= NULL;
	CPlateBasicIndex	*pBxLeftEnd	= NULL;
	CPlateBasicIndex	*pBxRighStt	= NULL;
	CPlateBasicIndex	*pBxRighEnd	= NULL;
	CPlateGirderApp		*pGir		= NULL;

	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	long nG = 0;
	CPlateBxFinder	FinderLeft(pDB->GetGirder(nG));
	pBxLeftStt = FinderLeft.GetBxFirst(BX_STT_GIRDER);
	pBxLeftEnd = FinderLeft.GetBxFirst(BX_END_GIRDER);

	nG = pDB->GetGirdersu()-1;
	CPlateBxFinder	FinderRigh(pDB->GetGirder(nG));
	pBxRighStt = FinderRigh.GetBxFirst(BX_STT_GIRDER);
	pBxRighEnd = FinderRigh.GetBxFirst(BX_END_GIRDER);

	DBPlan.DrawSlabTotal(&Dom);
	DBPlan.DrawTaperLinePyung(&Dom);		// Taper 선 그리기
	DBPlan.DrawCRBeamCLTotalGirder(&Dom);	//중심선만그림

	pGir	= pDB->GetGirder(0);
	CPlateBxFinder Finder(pGir);
	pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	double rotAng  = DBPlan.GetRotateAnglePyung(pBxStt,pBxEnd);		
	Dom.Rotate(rotAng,0,0);

	*pDom << Dom;
}

// 슬래브 횡단면도 그리기
void CAPlateOutBMBrInfo::DrawSlabFrontGen(CDomyun *pDom)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CARoadOptionStd		*pOptStd	= m_pAPlateOutBMStd->GetDataManage()->GetOptionStd();

	CAPlateDrawDanmyun DrawDanmyun(m_pAPlateOutBMStd->GetDataManage());
	CPlateBasicIndex	*pBx		= GetBxSection(SECTION_GEN);
	CDomyun	Dom(pDom);
	
	// 단면그림
	DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, FALSE);
	DrawDanmyun.DrawGuardWall     (&Dom, pBx);
	DrawDanmyun.DrawPierPlace     (&Dom, pBx, FALSE);
	
	// 횡단면도 치수선
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	DrawDanmyun.DimSlabUpper(&Dom, pBx, FALSE);

	*pDom << Dom;
}

// 거더 폭이 다른경우 일단 2가지 폭만 지원한다.....
void CAPlateOutBMBrInfo::GetGirderWidth(long &nGirdersu1, double &dGirderWidth1, long &nGirdersu2, double &dGirderWidth2)
{

}

// 캔틸레버부 폭
double CAPlateOutBMBrInfo::GetCantileverWidth(BOOL bLeft)
{
// 	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
// 	long nG = 0;
// 	if(bLeft)	nG = 0;
// 	else		nG = pDB->GetGirdersu()-1;
// 	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
// 	CPlateBxFinder	Finder(pGir);
// 	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
// 
// 	double	dWidth	= pDB->GetWidSlabUpFlangeWeb(pBx, bLeft);
// 
// 	return dWidth;

	return 0;
}

//가로보 헌치 상면 접합 최대/최소 길이...070315...KB...
//1. 가로보헌치상면접합 적용인 가로보
//2. 시/종점 헌치부 Station사이에 위치한 가로보
//3. 수직브레이싱 제외
//4. 슬레브상세입력에서 W,W1 제외한 길이...
//5. 헌치부 갯수등 같이 구하자...
void CAPlateOutBMBrInfo::GetCrossBeamHunch(double &dMaxLength, double &dMinLength, long &nQtyHunch, 
										double &dMinHeight, double &dMaxHeight, double &dLowWidth)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CDPoint A[10]; // 거더의 포인트
	double dHunchLength = 0;
	double dHunchMaxHeight = 0;
	double dHunchMinHeight = 0;
	double dHunchLowWidth = 0;

	dMaxLength = 0;
	dMinLength = 1000000.0;
	dMaxHeight = 0;
	dMinHeight = 1000000.0;
	nQtyHunch  = 0;
	dLowWidth  = 0;

	//시/종점 헌치부 Station사이에 위치한 가로보

	double dHunchStt = 0;
 	double dHunchEnd = 0;
		
	for(long nG = 0; nG < pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		//CPlateBxFinder		FinderStt(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		//CPlateBasicIndex	*pBxStt	= FinderStt.GetBxFirst(BX_STT_SLAB);
		pBx	= Finder.GetBxNext();

		dHunchStt = pGir->GetSSS(OFF_STT_HUNCHEND);// 
		dHunchEnd = pGir->GetSSS(OFF_END_HUNCHEND);// 
	
		while(pBx)
		{			
			//가로보 타입만...브레이싱타입 제외되고 나옴..
			CPlateCrossBeam		*pC	= pBx->GetCrossBeam();
			if(!pC)//Null이면 
			{
				pBx = Finder.GetBxNext();
				continue;
			}			

			//가로보헌치상면접합 적용인 가로보
			if(!pC->m_bHunchTopConnect) 
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			//시/종점 헌치부 Station사이에 위치한 가로보
			

			double dSta = pBx->GetStation();

			if(dSta <= dHunchStt || dSta >= dHunchEnd)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			pDB->GetGirder(nG)->GetHunchXyDom(pBx,A);

			dHunchLength = A[6].x - A[3].x;
			dHunchMaxHeight = max(A[4].y - A[3].y, A[5].y - A[6].y);
			dHunchMinHeight = min(A[4].y - A[3].y, A[5].y - A[6].y);

			dHunchLowWidth += A[3].x - A[0].x;

			if(dHunchLength > dMaxLength)
				dMaxLength = dHunchLength;

			if(dHunchLength < dMinLength)
				dMinLength = dHunchLength;

			if(dHunchMaxHeight > dMaxHeight)
				dMaxHeight = dHunchMaxHeight;

			if(dHunchMinHeight < dMinHeight)
				dMinHeight = dHunchMinHeight;

			nQtyHunch++;

			pBx = Finder.GetBxNext();			
		}
	}

	if (nQtyHunch ==0)
	{
		dMinLength = 0;
		dMaxLength = 0;
		dMaxHeight = 0;
		dMinHeight = 0;
		dLowWidth  = 0;
	}
	else
		dLowWidth = dHunchLowWidth/nQtyHunch;//평균값을 구하기 위해...	
}

double CAPlateOutBMBrInfo::GetGirderHeight(BOOL bMax)
{		
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();	

	// 간단히 입력한것을 이용함
	double	dHeight	= bMax ? 0 : 9999999.9;
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{	
		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		for(long nJ=0; nJ<pDB->m_nQtyJigan+1; nJ++)
		{		
			if(bMax)	dHeight = max(dHeight, pGir->GetHeightGirderByJijum(nJ,FALSE));
			else		dHeight = min(dHeight, pGir->GetHeightGirderByJijum(nJ,FALSE));			
		}
	}

	return  dHeight;
}

// x : 좌측 y : 우측
void CAPlateOutBMBrInfo::GetSlabVerticalDimensions(CVectorArray &ptArr)
{
	CADeckData		*pDeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CPlateBridgeApp *pDB		= m_pAPlateOutBMStd->GetBridgeApp();

	CDPoint xyDim;

	xyDim.x = pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABLEFT);
	xyDim.y = pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABRIGHT);
	ptArr.Add(xyDim);

	xyDim.x = pDB->m_dHeightTaperStt*5;
	xyDim.y = pDB->m_dHeightTaperStt*5;
	ptArr.Add(xyDim);

	double dLengthCentL = pDeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT) - pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABLEFT) - pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABLEFT) - pDB->m_dHeightTaperStt*5 - pDB->m_dHeightTaperEnd*5;
	double dLengthCentR = pDeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT) - pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABRIGHT) - pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABRIGHT) - pDB->m_dHeightTaperStt*5 - pDB->m_dHeightTaperEnd*5;

	xyDim.x = dLengthCentL;
	xyDim.y = dLengthCentR;
	ptArr.Add(xyDim);

	xyDim.x = pDB->m_dHeightTaperEnd*5;
	xyDim.y = pDB->m_dHeightTaperEnd*5;
	ptArr.Add(xyDim);

	xyDim.x = pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABLEFT);
	xyDim.y = pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABRIGHT);
	ptArr.Add(xyDim);

}


void CAPlateOutBMBrInfo::DrawSlabDongbari(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CDataManage			*pDataMng	= m_pAPlateOutBMStd->GetDataManage();
	CARoadOptionStd		*pOptStd	= pDataMng->GetOptionStd();
	CADeckData			*pDeckData	= pDataMng->GetDeckData();
	CBMOption			*pBMOpt		= &pDeckData->m_BMOption;
	
	CAPlateDrawDanmyun DrawDanmyun(pDataMng);	

	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);
	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetTextHeight(2);

	DrawDanmyun.DrawGuardWall(&Dom, pBx);	// 방호벽
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, TRUE, TRUE, CAPlateDrawCross::NOTDRAWVSTIFF);	// 슬래브 단면
	DrawDanmyun.DimSlabCenterLine(&Dom, pBx, FALSE);
	//DrawDanmyun.DrawSlabDosang(&Dom,pBx);	
	
	long dFlangeTUpBase = pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper();		
	long nGirderCnt		= pDB->GetGirdersu();
	long nG				= 0;
	CDPointArray xyArrHatchCent, xyArrHatchLeft, xyArrHatchRigh;
	CDPointArray xyArrUpper, xyArrLower;
	double dHatchW    = 100;
	double dWFinisher = pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth;
	double dUpperT		= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	double dLowerT		= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double dBindH		= pDB->m_BindConc.m_dHeight;
	double dBindW		= pDB->m_BindConc.m_dWidth;
	
	//각거더 내부 포인트 찾기..	
	CDPoint A[10];	
	CDPoint aLeft, aRigh;
	
 	for (nG=0; nG<nGirderCnt; nG++)
	{
		CPlateGirderApp *pGirCur = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch  = pGirCur->GetBxMatchByDiap(pBx, pGirCur);
		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE, FALSE, pDB->IsTUGir());//거더높이옵션 적용...
		
		double dWFlangeUp	= pGirCur->GetWidthFlangeUpper(pBx);//상부플랜지넓이..
		double dWFlangeLo	= pGirCur->GetWidthFlangeLower(pBx);//하부플랜지넓이..
		double dHeight		= pGirCur->GetHeightGirderByStaAng(pBx->GetStation());//복부판의 높이...			
		double dWebT		= pGirCur->GetThickFactByStation(pBx->GetStation(), G_W);//복부판 두께...
		double dGapUp		= (dWFlangeUp-dWebT)/2;
		double dGapLo		= (dWFlangeLo-dWebT)/2;

		if(dFlangeTUpBase ==1) dUpperT = 0;

if(pDB->IsTUGir())
		{
			dGapUp = (dBindW-dWebT)/2;
			aLeft = CDPoint(A[1].x+dGapUp, A[2].y-dBindH);
			aRigh = CDPoint(A[2].x-dGapUp, A[2].y-dBindH);
		}
		else
		{
			aLeft = CDPoint(A[1].x+dGapUp, A[1].y-dUpperT);
			aRigh = CDPoint(A[2].x-dGapUp, A[2].y-dUpperT);
		}

		xyArrUpper.Add(A[0]);
		if(pDB->IsTUGir())
		{			
			xyArrUpper.Add(CDPoint(A[1].x, A[2].y-dBindH));
			xyArrUpper.Add(aLeft);
			xyArrUpper.Add(aLeft);
		}
		else
		{
			xyArrUpper.Add(A[1]);
			xyArrUpper.Add(CDPoint(A[1].x, aLeft.y));
			xyArrUpper.Add(aLeft);
		}

		xyArrUpper.Add(aRigh);
		if(pDB->IsTUGir())
		{
			xyArrUpper.Add(CDPoint(A[2].x, aRigh.y));
			xyArrUpper.Add(CDPoint(A[2].x, aRigh.y));
			xyArrUpper.Add(A[3]);
		}
		else
		{
			xyArrUpper.Add(CDPoint(A[2].x, aRigh.y));
			xyArrUpper.Add(A[2]);
			xyArrUpper.Add(A[3]);
		}		

		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE);
		aLeft.y = A[1].y-dUpperT;
		aRigh.y = A[2].y-dUpperT;
		//하부 플랜지 좌우 포인트...
		xyArrLower.Add(CDPoint(aLeft.x-dGapLo, aLeft.y-dHeight));
		xyArrLower.Add(CDPoint(aLeft.x, aLeft.y-dHeight));
		xyArrLower.Add(CDPoint(aRigh.x, aLeft.y-dHeight));
		xyArrLower.Add(CDPoint(aRigh.x+dGapLo, aLeft.y-dHeight));
	}

	long nCntUp = 0;
	long nCntLo = 0;
	int i = 0;
	for (nG=0; nG<nGirderCnt-1; nG++)
	{
		xyArrHatchCent.RemoveAll();

		nCntUp = nG*8+4;
		nCntLo = nG*4+2+3;
								
		for (i=0; i<8; i++)
			xyArrHatchCent.Add(xyArrUpper[nCntUp++]);

		for (i=0; i<4; i++)
			xyArrHatchCent.Add(xyArrLower[nCntLo--]);

		//xyArrHatchCent.RemoveSameValue();//여기서는 굳이 할필요 없음....
		
		nCntUp = nG*8+4;
		nCntLo = nG*4+2+3;

		xyArrHatchCent.Add(xyArrUpper[nCntUp]);//다각형 첫점으로 막아야지...
		
		Dom.SetDirection("TOP");
		Dom.HatchingByCDPointArray(xyArrHatchCent, 45, dHatchW, TRUE);
		
		//Dom.LineTo(xyArrLower[nCntLo+1], xyArrLower[nCntLo+2]);

		CDPoint xyGisi = (xyArrLower[nCntLo]+xyArrLower[nCntLo-2])/2;
 		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y+200, Dom.Always(10), Dom.Always(20), 7, "동바리(목재)");
	}
	
	//좌/우측 슬래부 동바리...

	// 데크피니셔위치		0 : 미적용  1 : 좌측적용   2 : 우측적용   3 : 양측적용
	long nDFPos = pBMOpt->m_structTimber.m_nPosDeckFinisher;	
	CDPoint APave[HDANLINESU], ALeft[4], ARigh[4];
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyLeftIn, xyLeftOut;//  = A[9];
	CDPoint xyRighIn, xyRighOut;// = A[9] - vX*dWFinisher;

	CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(0));
	pDB->GetXyFrontCantilever(pBxLeft, ALeft, TRUE);
	CPlateBasicIndex *pBxRigh = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(nGirderCnt-1));
	pDB->GetXyFrontCantilever(pBxRigh, ARigh, FALSE);

	xyArrHatchLeft.RemoveAll();
	xyArrHatchRigh.RemoveAll();
	
	//슬래브헌치 적용/비적용시 같은 좌표 하나 더 들어감...
	//왼쪽
	for (i=0; i<3; i++)
		xyArrHatchLeft.Add(ALeft[i]);
	for(i=1; i<4; i++)
		xyArrHatchLeft.Add(xyArrUpper[i]);
	for(i=1; i>-1; i--)
		xyArrHatchLeft.Add(xyArrLower[i]);

	xyArrHatchLeft.Add(xyArrLower[0] - vY*dLowerT);//CDPoint(xyArrLower[0].x, xyArrLower[0].y - dLowerT));

	xyLeftIn = ALeft[0];
	xyLeftOut = ALeft[0] - vX*dWFinisher;

	//밑에서 사용해야 하므로...먼저 계산...
	xyRighIn = ARigh[0];
	xyRighOut = ARigh[0] + vX*dWFinisher;
	
	//왼쪽..데크피니셔적용..
	if(nDFPos ==1 || nDFPos ==3)//왼쪽적용이나 양쪽적용이면...
	{
		xyArrHatchLeft.Add(xyLeftOut);//다각형 첫점으로 막아야지...

		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyLeftOut.x, xyLeftOut.y, 0);
		Dom.DimLineTo(dWFinisher, 0, COMMA(dWFinisher));

		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyRighIn.x+dWFinisher, xyArrLower[xyArrLower.GetUpperBound()].y-dLowerT, 0);
		CString sText;
		sText.Format("%s ~ %s", COMMA(xyRighIn.y-xyArrLower[xyArrLower.GetUpperBound()].y-pDB->m_dHeightTaperStt), 
			COMMA(xyRighIn.y-xyArrLower[xyArrLower.GetUpperBound()].y));
		Dom.DimLineTo(xyRighIn.y-xyArrLower[xyArrLower.GetUpperBound()].y, 0, sText);

		Dom.SetDirection("TOP");
 		CDPoint xyGisi = xyArrLower[0];
 		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y+200, Dom.Always(10), -Dom.Always(20), 5, "데크피니셔");
	}
	
	xyArrHatchLeft.RemoveSameValue();//
	xyArrHatchLeft.Add(xyLeftIn);//다각형 첫점으로 막아야지...	
		
	Dom.SetDirection("TOP");
	Dom.HatchingByCDPointArray(xyArrHatchLeft, 45, dHatchW, TRUE);
	
	//슬래브헌치 적용/비적용시 같은 좌표 하나 더 들어감...
	//오른쪽...마지막 하부거더부터 시작...했더니 방향이 안맞네..끙...슬래브 맨끝에서 시작..도 안되네..쩝...거더 오른쪽 시작점에서...	

	nCntLo = xyArrLower.GetUpperBound();//(nGirderCnt-1)*4+7;//하부 거더 맨 마지막 배열...
	nCntUp = (nGirderCnt-1)*8+4;;//상부..마지막...
	
	for (i=nCntUp; i<nCntUp+3; i++)
		xyArrHatchRigh.Add(xyArrUpper[i]);
	for(i=2; i>-1; i--)
		xyArrHatchRigh.Add(ARigh[i]);	

	//오른쪽..데크피니셔적용..
	if(nDFPos ==2 || nDFPos ==3)//오른쪽 적용이나 양쪽적용이면...
	{
		xyArrHatchRigh.Add(xyRighOut);//다각형 첫점으로 막아야지...

		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyRighIn.x, xyRighIn.y, 0);
		Dom.DimLineTo(dWFinisher, 0, COMMA(dWFinisher));

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(xyLeftIn.x-dWFinisher, xyArrLower[xyArrLower.GetUpperBound()].y-dLowerT, 0);
		CString sText;
		sText.Format("%s ~ %s", COMMA(xyLeftIn.y-xyArrLower[xyArrLower.GetUpperBound()].y-pDB->m_dHeightTaperStt), 
			COMMA(xyLeftIn.y-xyArrLower[xyArrLower.GetUpperBound()].y));
		Dom.DimLineTo(xyLeftIn.y-xyArrLower[xyArrLower.GetUpperBound()].y, 0, sText);

		Dom.SetDirection("TOP");
 		CDPoint xyGisi = xyArrLower[xyArrLower.GetUpperBound()];
 		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y+200, Dom.Always(10), Dom.Always(20), 7, "데크피니셔");
	}
		
	CDPoint xyLastPt = xyArrLower[nCntLo] - vY*dLowerT;//CDPoint(xyArrLower[nCntLo].x, xyArrLower[nCntLo].y - dLowerT);
	xyArrHatchRigh.Add(xyLastPt);
	xyArrHatchRigh.Add(xyArrLower[nCntLo]);
	xyArrHatchRigh.Add(xyArrLower[nCntLo-1]);
	
	xyArrHatchRigh.RemoveSameValue();//
	xyArrHatchRigh.Add(xyArrUpper[nCntUp]);//다각형 첫점으로 막아야지...
			
	Dom.SetDirection("TOP");
	Dom.HatchingByCDPointArray(xyArrHatchRigh, 45, dHatchW, TRUE);

	*pDom << Dom;

	if(!pDB->IsTUGir())	return;	
	for(nG=0; nG<nGirderCnt; nG++)
	{
		CPlateGirderApp *pGirCur = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch  = pGirCur->GetBxMatchByDiap(pBx, pGirCur);
		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE, FALSE, TRUE);

		double dDeep = pDB->m_BindConc.m_dDeep;
		CDRect rectTrim(A[2].x-dBindH, A[2].y-dBindH+0.0001, A[2].x-0.0001, A[2].y-dDeep);
		pDom->TrimByRect(rectTrim);
	}
 		
// 	CHgBaseDrawStd baseDraw;
// 	CStringArray sArrText;
// 	CStringArray sArrUnderText;
// 	CString strUp, strDn, sDir = _T("BOTTOM");
// 	CDoubleArray dArrDimSum, dArrDim;
// 	dArrDimSum.Add(xyArrLowerLeft[0].x);
// 	dArrDimSum.Add(xyArrLowerRigh[xyArrLowerRigh.GetUpperBound()].x);
// 	for ( nG=0; nG<pDB->GetGirdersu(); nG++)
// 	{
// 		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
// 
// 		dArrDimSum.Add(xyArrLowerLeft[nG].x + (pGir->m_dLeftSharp-dArrWebTL[nG]));
// 		dArrDimSum.Add(xyArrLowerRigh[nG].x - (pGir->m_dRighSharp-dArrWebTR[nG]));
// 	}
// 	dArrDimSum.Sort();
// 
// 	for (long n=0; n<dArrDimSum.GetSize()-1; n++)
// 		dArrDim.Add(dArrDimSum[n+1]-dArrDimSum[n]);
// 
// 	baseDraw.AutoDimCont(&Dom, xyArrLowerLeft[0], dArrDim, sArrText, sArrUnderText, 0, sDir, -1, FALSE, FALSE, FALSE);
// 
}


double CAPlateOutBMBrInfo::GetBridgeLength(long nGuardFence)
{	
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);	
	CADeckData	*pADeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CFemStressCalc  StressCalc(m_pAPlateOutBMStd->GetDataManage(), NULL);
	
	double	dResult		= 0;
	long	nQtyHDan	= pDB->GetQtyHDanDom(pBx);
	long	nSumGuard	= 0;
	long	nHDan		= 0;

	if(nGuardFence!=0)
	{
		for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			long nType = pDB->GetValueTypeHDan(nHDan, 1);
			if(nType == 1 || nType == 2)	 // 방호벽, 중분대
				nSumGuard++;
			if(nSumGuard == nGuardFence)
				break;
		}
	}

	double	dDistGuard		= pDB->GetLengthHDanDom(pBx, nHDan);
	double	dDistL			= pADeckData->GetSideBarDistFromCenterGir(TRUE, TRUE);
	double	dLengthL		= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT);
	double	dDistR			= pADeckData->GetSideBarDistFromCenterGir(TRUE, FALSE);
	double	dLengthR		= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT);
	double	dLengthReturn	= 0;
	dLengthReturn = StressCalc.Bogan(dDistL, dDistR, dLengthL, dLengthR, dDistGuard); 

	return dLengthReturn;
}

double CAPlateOutBMBrInfo::GetLengthGirder(long nG)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);

	double	dstaGirStt	= pGir->GetSSS(OFF_STT_GIRDER);
	double	dstaGirEnd	= pGir->GetSSS(OFF_END_GIRDER);

	return dstaGirEnd - dstaGirStt;
}

CDoubleArray CAPlateOutBMBrInfo::GetSizeConnectPart(long nG, BOOL bUpper)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxLast(BX_END_GIRDER);

	double staGirStt = pGir->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double staGirEnd = pGir->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션	
	double	dThick = 0, sta = 0;
	double	dSizeTen = 0, dSizeCom = 0, dSizeAlter = 0;
	CDoubleArray	arrPartSize;
	arrPartSize.RemoveAll();
	
	if(bUpper)
	{
		// 상판 공장이음 치수선 그림.
		CPlateBasicIndex *pBx		= Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE, pBxStt, pBxEnd);
		double	dPreSta		= staGirStt;
		double	dPreThick	= 0;
		while(pBx)   
		{
			dThick = pGir->GetThickJewon(G_F_U, pBx);
			if (!pBx->IsState(BX_UPPERFLANGE) || dThick != dPreThick)   
			{
				sta = pBx->GetStation();
				
				if (staGirStt < sta && sta < staGirEnd)   
				{
					long nValue	= pBx->GetCompressTypeByComplex();	// 0:상부압축, 1:하부압축, 2:교번

					if(nValue==1)		++dSizeCom;		
					else if(nValue==2)	++dSizeAlter;
					else if(nValue==3)	++dSizeTen;
				}
				
				if (sta >= staGirEnd)
					break;
				dPreSta = sta;
				dPreThick = dThick;
			}
			pBx = Finder.GetBxNext();
		}
	}
	else
	{
		// 하판 공장이음 치수선 그림.
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE, pBxStt, pBxEnd);
		double	dPreSta		= staGirStt;
		double	dPreThick	= 0;
		while(pBx)   
		{
			dThick = pGir->GetThickJewon(G_F_L, pBx);
			if (!pBx->IsState(BX_LOWERFLANGE) || dThick != dPreThick)   
			{
				sta = pBx->GetStation();
				
				if (staGirStt < sta && sta < staGirEnd)   
				{
					long nValue	= pBx->GetCompressTypeByComplex();	// 0:상부압축, 1:하부압축, 2:교번

					if(nValue==1)		++dSizeTen;
					else if(nValue==2)	++dSizeAlter;
					else if(nValue==3)	++dSizeCom;
				}

				if (sta >= staGirEnd)
					break;
				dPreSta = sta;
				dPreThick = dThick;
			}
			pBx = Finder.GetBxNext();
		}
	}

	arrPartSize.Add(dSizeTen);
	arrPartSize.Add(dSizeAlter);
	arrPartSize.Add(dSizeCom);

	return arrPartSize;	
}

CDoubleArray CAPlateOutBMBrInfo::GetLengthWebButtWelding()
{	
	CPlateBridgeApp	*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CWeldStandard	*pWeldStd	= m_pAPlateOutBMStd->GetDataManage()->GetWeldStandard();
	
	CPtrList		*pList		= &pWeldStd->m_ListSang;

	if(pList->GetCount() <= 0) return 0;

	CDoubleArray	arrLengthButtWelding;
	arrLengthButtWelding.RemoveAll();

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		POSITION pos = pList->GetHeadPosition();
		double	dLengthButtWelding	= 0;

		while( pos )
		{	
			CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

			if(pRec->m_nGirder==nG && pRec->GetStrSubEleName()=="복부판" && pRec->m_sWeldType=="맞대기 용접")
				dLengthButtWelding += pRec->m_dLengthWeld;
		}
		arrLengthButtWelding.Add(dLengthButtWelding);
	}

	return arrLengthButtWelding; 
}

//상부/하부  플랜지 폭원 및 모멘트 구간.....
void CAPlateOutBMBrInfo::GetFlangeLengthCompress(BOOL bUpper, CDoubleArray &arrFlangeWidth, 
									CDoubleArray &arrCompress, CDoubleArray &arrSort, double dCompress[3][99])
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp *pGir	= pDB->GetGirder(0);
	CPlateBxFinder Finder(pGir);	

	__int64 BxFlag;
	if (bUpper)	BxFlag = BX_UPPERFLANGE | BX_SPLICE;//상판공장이음, 현장용접
	else		BxFlag = BX_LOWERFLANGE | BX_SPLICE;//하판공장이음..현장용접
	
	double dWidth = 0;	
	long nType = 0;
	BOOL bSpliceType = FALSE;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);

	while(pBx)
	{				
		//현장용접인지 아닌지 체크 요망....
		if (pBx->IsState(BX_SPLICE))
		{
			bSpliceType = pBx->GetSplice()->m_bSpliceType;
			if(bSpliceType) 
			{
				pBx = Finder.GetBxNext();
				continue;//현장이음이므로 통과...
			}
		}
		
		if (bUpper)	dWidth = pGir->GetWidthFlangeUpper(pBx);
		else		dWidth = pGir->GetWidthFlangeLower(pBx);

		arrFlangeWidth.Add(dWidth);
		//리턴 값 1=상부압축, 2=하부압축, 3=교번부 리턴
		nType = pBx->GetCompressTypeByComplex();
		
		arrCompress.Add(nType);
		pBx = Finder.GetBxNext();
	}
	
	arrSort = arrFlangeWidth;
	arrSort.RemoveSameValue();
	arrSort.Sort();

	long dSize = arrSort.GetSize();	
	long nR = 0;
	long nFix = 0;
	//상부 = 0:압축, 1:인장, 2:교번... [99] = dSize만큼만...
	//하부 = 0:인장, 1:압축, 2:교번... [99] = dSize만큼만...
	//dCompress[0][] : 인장부로
	//dCompress[1][] : 교번부로
	//dCompress[2][] : 압축부로
	long nTension = bUpper ? 2 : 1;	// 인장부
	long nComplex = 3;

	long i, j;
	
	//배열 초기화
	for (i=0;i<3;i++)
		for (j=0; j<99;j++)		
			dCompress[i][j] = 0;					
		
	for(i=0; i<arrSort.GetSize(); i++)
		for(j=0; j<arrFlangeWidth.GetSize();j++)
		{
			if(arrSort[i] == arrFlangeWidth[j])
			{
				nR = (long)arrCompress[j];

				if (nR == nTension)
					nFix = 0;//인장부					
				else if (nR == nComplex)
					nFix = 1;//교번부
				else
					nFix = 2;//압축부

				dCompress[nFix][i] = dCompress[nFix][i] + 1;
			}
	}

}

CDoubleArray CAPlateOutBMBrInfo::GetLengthHStiffener()
{
	CPlateBridgeApp	*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CWeldStandard	*pWeldStd	= m_pAPlateOutBMStd->GetDataManage()->GetWeldStandard();

	CPtrList		*pList		= &pWeldStd->m_ListSang;

	if(pList->GetCount() <= 0) return 0;

	CDoubleArray	arrLengthHStiffener;
	arrLengthHStiffener.RemoveAll();

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		POSITION pos = pList->GetHeadPosition();
		double	dLengthHStiffener	= 0;

		while( pos )
		{	
			CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

			if(pRec->m_nGirder==nG && pRec->GetStrEleName()=="일반" && pRec->GetStrSubEleName()=="수평보강재")
				dLengthHStiffener += pRec->m_dLengthWeld;
		}
		arrLengthHStiffener.Add(dLengthHStiffener);
	}

	return arrLengthHStiffener; 
}

//지점보강재 
void CAPlateOutBMBrInfo::GetLengthJijumStiffener(long nG, CDoubleArray &arrLength, CDoubleArray &arrSize)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
	
 	arrLength.RemoveAll();
	arrSize.RemoveAll();

	double dWidth = 0;
	double dLength = 0;
	long nCount = 0;
	
	for(long nJijum = 0; nJijum <= pDB->m_nQtyJigan; nJijum++)
	{
		nCount = 0;
		for (long nJack = 0; nJack < JACKUPSUMAX; nJack++)
		{
			dLength = pDB->m_dJiJumTerm[nJijum][nJack];
			if (dLength != 0) nCount++;			
		}

		CVStiff *pVStiff = pGir->GetJijumVStiffByJijum(nJijum, 0);
		if(pVStiff)
		{		
			arrLength.Add((pVStiff->m_dWidth[0]+pVStiff->m_dWidth[1])*nCount);
			arrSize.Add(nCount*2);	
		}
		else
		{		
			arrLength.Add(0);
			arrSize.Add(0);	
		}
	}
}	

double CAPlateOutBMBrInfo::GetUpperLengthAver(BOOL bPositive)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();

	double	dLengthTot	= 0;
	long	nIndex		= 0;

	for(long nG = 0; nG < pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_UPPERFLANGE | BX_END_GIRDER);

		nIndex	= 0;
		while(pBx)
		{
			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
			double	dStaPre		= pBx->GetStation();
			double	dStaNext	= pBxNext ? pBxNext->GetStation() : pBx->GetStation();
			double	dDist		= dStaNext - dStaPre;
			if(bPositive)
			{
				if(nIndex%2==0)
					dLengthTot += dDist;
			}
			else
			{
				if(nIndex%2!=0)
					dLengthTot += dDist;
			}
			nIndex++;
			pBx = Finder.GetBxNext();
		}
	}

	return dLengthTot/pDB->GetQtyGirder();
}

double CAPlateOutBMBrInfo::GetConfinementLengthAver()
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateBxFinder	Finder(pGir);

	double	dSta1		= Finder.GetBxFirst(BX_STT_SLAB)->GetStation() + (pDB->m_dLengthSlabGirderStt-pDB->m_dLengthTUBindConc_Stt);
	double	dSta2		= Finder.GetBxFirst(BX_END_SLAB)->GetStation() - (pDB->m_dLengthSlabGirderEnd-pDB->m_dLengthTUBindConc_End);
	double	dLength		= pGir->GetLengthDimType(dSta1, dSta2);

	return dLength;
}