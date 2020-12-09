  // APlateOutSplice.cpp: implementation of the CAPlateOutSplice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define	SP_NUMBER			10	//현장이음 번호
#define	SP_UPPER			20	//상판 이음판
#define	SP_UPPER_TABLE		40	//상판 이음판 재료표
#define	SP_WEB				50	//복부판 이음판
#define	SP_WEB_TABLE		70	//복부판 이음판 재료표
#define	SP_LOWER			80	//하판 이음판
#define	SP_LOWER_TABLE		100	//하판 이음판 재료표
#define	SP_LOWER_RIB		110	//Lower RIB 이음판
#define	SP_LOWER_RIB_TABLE	120	//Lower RIB 이음판 재료표
#define	SP_UPPER_RIB		130	//Upper RIB 이음판
#define	SP_UPPER_RIB_TABLE	140	//Upper RIB 이음판 재료표
#define	SP_WEB_BYUNDAN		150
#define	SP_DIAPHRAM_DANMYUN	160	//Diaphram 단면도
#define SP_JEWON			170
#define SP_WEB_TITLE		200

CAPlateOutSplice::CAPlateOutSplice()
{
	m_bNomyun = FALSE;
}


CAPlateOutSplice::~CAPlateOutSplice()
{
	AhPtrListDelete(m_BMList, (CBMRecord *)0);
}

 
void CAPlateOutSplice::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove     = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageView = pStd->GetDrawPageMng();
	COptionStd      *pOpt      = pStd->GetDomOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	m_bNomyun = FALSE;
	long nPage = pStd->m_nCurPage;
	long PrePage   = m_nSttPage; 
	long nSubPage   = nPage - PrePage;
	double Scale = GetScale();

	if(nSubPage>GetPageSu()) return;

	CDomyun *pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);
	CDomyun Dom(pDom);
	// 도각
	pPageView->SetProgressData("도각 출력중...",0);
	CString csTitle = GetTitle();
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_MAIN_TITLE);
	//pStd->GetDogak()->Add(pDom,nPage,Scale,pOpt->m_szDGKName,pOpt->m_sDomNumHead,csTitle,nSttPage);
	pStd->GetDogak()->Add(&Dom,nPage,Scale,pOpt->m_szDGKName,pOpt->m_sHead_DomyunNumber,pOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(pDom, pBridge->m_sNameBridge, csTitle, _T(""), FALSE);

	pStd->AddMaterial(&Dom,Scale);
	pStd->AddJubujebubuje(&Dom,Scale);
	*pDom << Dom;			

	pDom->SetCalcExtRect();
	CDRect	DgkBound = pDom->GetExtRect();
	CDPoint xyMove = CDPoint(Dom.Always(45), DgkBound.Height()-Dom.Always(125));
	CDPoint xyJewon, xyTitle;
	for(long nIdxOnPage=0; nIdxOnPage<=3; nIdxOnPage++)
	{
		CPlateBasicIndex *pBx = GetBxPage(nSubPage, nIdxOnPage);
		if(pBx==NULL)
			continue;
///////////////////////////////////////////////////////////
//상판 이음판이음판
		if(pOpt->m_SP_bTopView)
		{
//			EXIT_RTN;
			pPageView->SetProgressData("상판 이음판 출력중...",nIdxOnPage*25+1);
			AddUpperSplice(&Dom, pBx, nIdxOnPage);

			Dom.SetCalcExtRect();	
			CDRect rc = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rc.left, -rc.top-rc.Height()); 	
			xyJewon = CDPoint(xyMove.x+rc.Width()+Dom.Always(10), xyMove.y-Dom.Always(10));
			xyMove.y -= rc.Height()+Dom.Always(40);
			
			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(SP_UPPER+nIdxOnPage));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_UPPER+nIdxOnPage, "상판");
			*pDom << Dom;

			Dom.SetScaleDim(GetScale());
			Dom.SetCalcExtRect();	

			CDRect rect = Dom.GetExtRect();
			double dScale = pOpt->m_SP_Scale.GetAt(0);
			DrawStd.AddTitleOnDom(&Dom, dScale, rect, "상부 플랜지",TRUE, TRUE, 0);

			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(SP_UPPER+nIdxOnPage+10));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_UPPER+nIdxOnPage+10, "상판 제목");
			*pDom << Dom;
			
			if(pOpt->m_SP_nJewon)
			{
				pPageView->SetProgressData("상판 이음판 제원 출력중...",nIdxOnPage*25+6);
				AddUpperSpliceTable(&Dom, pBx, nIdxOnPage);

				Dom.SetCalcExtRect();	
				CDRect rc = Dom.GetExtRect();
				Dom.Move(xyJewon);
				Dom.Move(-rc.left, -rc.top-rc.Height()); 	
				
				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(SP_JEWON+nIdxOnPage));
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_JEWON+nIdxOnPage, "상판 제원");
				*pDom << Dom;
			}
		}
///////////////////////////////////////////////////////////
//복부판 이음판
		if(pOpt->m_SP_bWebView)
		{
//			EXIT_RTN;
			pPageView->SetProgressData("복부판 이음판 출력중...",nIdxOnPage*25+9);
			AddWebSplice(&Dom, pBx, nIdxOnPage);

			//////////////////////////////////////////////////////////////////////////			
			Dom.SetCalcExtRect();
			CDRect rc = Dom.GetExtRect();
			Dom.Move(-rc.left+xyMove.x, -rc.top-rc.Height()+xyMove.y); 
			Dom.Move(pMove->GetPoint(SP_WEB+nIdxOnPage));

			xyJewon = CDPoint(xyMove.x+rc.Width()+Dom.Always(10), xyMove.y-Dom.Always(30));
			xyMove.y -= rc.Height()+Dom.Always(40);

			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_WEB+nIdxOnPage, "복부판");
			*pDom << Dom;
			
			double dScale = pOpt->m_SP_Scale.GetAt(1);
			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, dScale, Dom.GetExtRect(), "복부판",TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(SP_WEB_TITLE+nIdxOnPage));
			Dom.SetCalcExtRect();
			
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_WEB_TITLE+nIdxOnPage, "복부판 제목");
			*pDom << Dom;

			if(pOpt->m_SP_nJewon)
			{
				pPageView->SetProgressData("복부판 이음판 제원 출력중...",nIdxOnPage*25+14);
				AddWebSpliceTable(&Dom, pBx, nIdxOnPage);

				Dom.SetCalcExtRect();	
				CDRect rc = Dom.GetExtRect();
				Dom.Move(xyJewon);
				Dom.Move(-rc.left, -rc.top-rc.Height()); 	

				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(SP_WEB_TABLE+nIdxOnPage));
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_WEB_TABLE+nIdxOnPage, "복부판 제원");
				*pDom << Dom;
			}
		}
///////////////////////////////////////////////////////////
// 하판 이음판
		if(pOpt->m_SP_bBottomView)
		{
//			EXIT_RTN;
			pPageView->SetProgressData("하판 이음판 출력중...",nIdxOnPage*25+17);
			AddLowerSplice(&Dom, pBx, nIdxOnPage);
			
			Dom.SetCalcExtRect();	
			CDRect rc = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rc.left, -rc.top-rc.Height()); 	
			xyJewon = CDPoint(xyMove.x+rc.Width()+Dom.Always(10), xyMove.y-Dom.Always(10));
			xyTitle.x = xyMove.x-rc.left;
			xyMove = CDPoint(xyMove.x+rc.Width()+Dom.Always(25), DgkBound.Height()-Dom.Always(125));

			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(SP_LOWER+nIdxOnPage));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_LOWER+nIdxOnPage, "하판");
			*pDom << Dom;

			Dom.SetScaleDim(GetScale());
			Dom.SetCalcExtRect();	

			CDRect rect = Dom.GetExtRect();
			double dScale = pOpt->m_SP_Scale.GetAt(2);
			DrawStd.AddTitleOnDom(&Dom, dScale, rect, "하부 플랜지",TRUE, TRUE, 0);

			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(SP_LOWER+nIdxOnPage));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_LOWER+nIdxOnPage+10, "하판 제목");
			*pDom << Dom;

			if(pOpt->m_SP_nJewon)
			{
				pPageView->SetProgressData("하판 이음판 제원 출력중...",nIdxOnPage*25+22);
				AddLowerSpliceTable(&Dom, pBx, nIdxOnPage);

				Dom.SetCalcExtRect();	
				CDRect rc = Dom.GetExtRect();
				Dom.Move(xyJewon);
				Dom.Move(-rc.left, -rc.top-rc.Height()); 	
				xyMove = CDPoint(xyJewon.x+rc.Width()+Dom.Always(25), DgkBound.Height()-Dom.Always(125));

				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(SP_LOWER_TABLE+nIdxOnPage));
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_LOWER_TABLE+nIdxOnPage, "하판 제원");
				*pDom << Dom;
			}
		}
//SPLICE TITLE
//		if(pOpt->m_bSpliceNumber)
		{
//			EXIT_RTN;
			pPageView->SetProgressData("현장이음 번호 출력중...",nIdxOnPage*25+25);
			AddSpTiTle(&Dom, nSubPage, nIdxOnPage);
			Dom.Move(xyTitle.x, Dom.Always(520));

			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(SP_NUMBER+nIdxOnPage));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_NUMBER+nIdxOnPage, "현장이음 번호");
			*pDom << Dom;
		}

///////////////////////////////////////////////////////////
// 상판 재료표 ////////////////////////
/*		if(pOpt->m_bSpliceBillofMatrial && pOpt->m_bBillofMatrialType != 0)
		{
			EXIT_RTN;
			pPageView->SetProgressData("상판 이음판 재료표 출력중...",30);
			AddUpperSpliceTable(&Dom, pBx, nIdxOnPage);
			*pDom << Dom;
		}
///////////////////////////////////////////////////////////
// 복부판 재료표 ////////////////////////
		if(pOpt->m_bSpliceBillofMatrial && pOpt->m_bBillofMatrialType != 0)
		{
			EXIT_RTN;
			pPageView->SetProgressData("복부판 이음판 재료표 출력중...",50);
			AddWebSpliceTable(&Dom, pBx, nIdxOnPage);
			*pDom << Dom;
		}
///////////////////////////////////////////////////////////
// 하판 재료표 ////////////////////////
		if(pOpt->m_bSpliceBillofMatrial && pOpt->m_bBillofMatrialType != 0)
		{
			EXIT_RTN;
			pPageView->SetProgressData("하판 이음판 재료표 출력중...",70);
			AddLowerSpliceTable(&Dom, pBx, nIdxOnPage);
			*pDom << Dom;
		}*/
	}
///////////////////////////////////////////////////////////
/*	if(pOpt->m_bSplicenote)
	{
		// 노트 출력
		MoveX =  m_DgkBound.Width() - Dom.Always(180);
		MoveY =  Dom.Always(160);
		pPageView->AddNotePage(MoveX,MoveY,pOpt->m_sNote.GetAt(PAGE_FIELD_SPLICE));	
	}
*/
	if(pOpt->m_SP_nJewon==0 || pOpt->m_SP_nJewon==2)
	{
		pPageView->SetProgressData("현장이음 재료표 출력중...",90);	
		CPlateBasicIndex *pBx1 = GetBxPage(nSubPage, 0);
		CPlateBasicIndex *pBx2 = GetBxPage(nSubPage, 1);
		AddMaterialTable(&Dom, nSubPage);
		Dom.SetCalcExtRect();
		CDRect DomBound = Dom.GetExtRect();
		CDRect DgkBound = pStd->GetDogak()->GetRectDraw(Scale);
		double MoveX = DgkBound.right - DomBound.Width();
		double MoveY = DgkBound.bottom - DomBound.bottom - Dom.Always(50);
		Dom.Move(MoveX, MoveY);

		Dom.Move(pMove->GetPoint(SP_UPPER_TABLE));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), SP_UPPER_TABLE, "재료표");
		*pDom << Dom;
	}

	pPageView->SetProgressData("출력 완료...",100);
}


void CAPlateOutSplice::AddUpperSplice(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);

	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp *pGir		= (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice    *pSp     = (CPlateSplice*)pBx->GetSplice();
	COptionStd  *pOptS = pStd->m_pDomOptionStd;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());
	CDomyun Dom(pDom);
	double dScale = pOptS->m_SP_Scale.GetAt(0);
	Dom.SetScaleDim(dScale);
	DrawPyung.DrawUpperFlangeSplice(&Dom, pBx, 0, 0, FALSE, FALSE, TRUE, FALSE);

//치수선
	if(pBx == NULL) return;

	double mA  = pSp->m_mA;
	double mB  = pSp->m_mB;
	double mC  = pSp->m_mC;
	double mC1 = pSp->m_mC1;

	double sA  = pSp->m_sA;
	double sB  = pSp->m_sB;
	double sC  = pSp->m_sC;
	double sC1 = pSp->m_sC1;

	double Width	= (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2 + pSp->m_uC1;
	const	double	UWidth= pSp->m_uA+pSp->m_uB*pSp->m_uC+pSp->m_uC1+pSp->m_uB*pSp->m_uC+pSp->m_uA;	//	상부 현장이음 폭
	const	double	DWidth= pSp->m_dA+pSp->m_dB*pSp->m_dC+pSp->m_dC1+pSp->m_dB*pSp->m_dC+pSp->m_dA; //	하부 현장이음 폭
	const	double	MWidth= (mA+mB*mC+mC1/2)*2;	//	모멘트 플레이트
	const	double	SWidth= (sA+sB*sC+sC1/2)*2;	//	전단   플레이트
	double Gap;	//플레이트와 플레이트 사이의 간격계산에 사용
//	double HGap;	//볼트와 플레이트 사아의 간격계산에 사용(첫번째)
	double Half;	//기준 플레이트(여기서는 SWidth)와의 차이(X좌표로)

	//	상판
	//	가장 긴 X좌표를 구함.
	double MaxX;
	MaxX	= MWidth;// = SWidth
	if(MaxX < UWidth)	MaxX = UWidth;
	if(MaxX < DWidth)	MaxX = DWidth;
	if(MaxX < (SWidth))	MaxX = SWidth;
	Half	= MaxX/2;

	double dWGirPre		= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double dWGirNex		= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double dWidthGir	= min(dWGirPre, dWGirNex);
	double sWidth		= (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1;//	스플라이스 폭
	//도면설정
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	//우측 치수선
	Dom.SetDirection("RIGHT");
	long nDimDan =0;
	double dS = (pSp->m_uW - pSp->m_uE * pSp->m_uF)/2;
	double dEF = dWidthGir - (dS + pSp->m_uE * pSp->m_uF) * 2;
	CString str1, str2;
	if(pSp->m_uE>1)
	{
		str1.Format("%d@%.f", pSp->m_uE, pSp->m_uF);
		str2.Format("=%.f", pSp->m_uE*pSp->m_uF);
	}
	else
	{
		str1.Format("%.f", pSp->m_uF);
		str2 = "";
	}

	Dom.DimMoveTo(Half+Dom.Always(7), -dWidthGir, nDimDan, COMMA(dS));
	Dom.DimLineTo(dS, nDimDan, "", 1);
	Dom.DimLineToExtend(pSp->m_uE*pSp->m_uF, nDimDan, str1, str2);
//	Dom.SetDimTvp(tvp);
	Dom.DimLineTo(dEF, nDimDan, COMMA(dEF));
//	Dom.SetDimTvp(tvpCur);
	Dom.DimLineToExtend(pSp->m_uE*pSp->m_uF, nDimDan, str1, str2);
	Dom.DimLineToOver(dS, nDimDan, COMMA(dS), TRUE);

	nDimDan = 1;
	Dom.DimMoveTo(Half+Dom.Always(8), -dWidthGir, nDimDan, "", FALSE);
	Dom.DimLineTo(dWidthGir, nDimDan, COMMA(dWidthGir));

	//하부치수선
	Dom.SetDirection("BOTTOM");
	nDimDan =0;
	str1.Format("%d@%.f", pSp->m_uB, pSp->m_uC);
	str2.Format("=%.f", pSp->m_uB*pSp->m_uC);
	Dom.DimMoveTo(-sWidth/2, -dWidthGir - Dom.Always(3), nDimDan, COMMA(pSp->m_uA));
	Dom.DimLineTo(pSp->m_uA, nDimDan, "", TRUE);
	Dom.DimLineToExtend(pSp->m_uB*pSp->m_uC, nDimDan, str1, str2);
//	Dom.SetDimTvp(tvp);
	Dom.DimLineTo(pSp->m_uC1, nDimDan, COMMA(pSp->m_uC1), 1);
//	Dom.SetDimTvp(tvpCur);
	Dom.DimLineToExtend(pSp->m_uB*pSp->m_uC, nDimDan, str1, str2);
	Dom.DimLineToOver(pSp->m_uA, nDimDan, COMMA(pSp->m_uA));
	nDimDan =1;
	Dom.DimMoveTo(-sWidth/2, -dWidthGir - Dom.Always(4), nDimDan, "");
	Dom.DimLineTo(pSp->m_uA*2+pSp->m_uB*pSp->m_uC*2+pSp->m_uC1, nDimDan, COMMA(pSp->m_uA*2+pSp->m_uB*pSp->m_uC*2+pSp->m_uC1));

	//좌측치수선
	Dom.SetDirection("LEFT");
	nDimDan = 0;
	Gap = dWidthGir - pSp->m_uW * 2;
	Dom.DimMoveTo(-Half-Dom.Always(7), -dWidthGir, nDimDan, "", FALSE);
	Dom.DimLineTo(pSp->m_uW, nDimDan, COMMA(pSp->m_uW));
	Dom.DimLineToExtend(Gap, nDimDan, "", COMMA(Gap));
	Dom.DimLineTo(pSp->m_uW, nDimDan, COMMA(pSp->m_uW));
	//상부

	Dom.RedrawByScale(GetScale()/dScale);
	*pDom << Dom;
}

long CAPlateOutSplice::GetBoltSu(CPlateBasicIndex *pBx, long nUpDown)
{
	CPlateSplice *pSp = pBx->GetSplice();
	if(pSp==NULL) return 0;

	if(nUpDown==1) // 상판
	{
		long ColBoltSu = (pSp->m_uB + 1) * 2;
		long RowBoltSu = (pSp->m_uE + 1) * 2;

		return RowBoltSu*ColBoltSu;
	}
	else if(nUpDown==0) // 복부판
	{
		long MomentBolt = pSp->m_nWebType!=SP_WEB_FL ? ((pSp->m_mE + 1) * (pSp->m_mB + 1) * 4) : 0;
		long SHBolt     = (pSp->m_sE + pSp->m_sE1*2 + 1) * (pSp->m_sB + 1) * 2;
		return MomentBolt+SHBolt;
	}
	else if(nUpDown==-1) // 하판
	{
		long ColBoltSu = (pSp->m_dB + 1)*2;
		long RowBoltSu = (pSp->m_dE + 1)*2;

		return RowBoltSu*ColBoltSu;
	}
	else return 0;

}

void CAPlateOutSplice::AddUpperSpliceTable(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);

	CDomyun Dom(pDom);
	double dScale = 20;
	Dom.SetScaleDim(dScale);
	DrawTopFlangeTable(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/dScale);
	*pDom << Dom;
}

void CAPlateOutSplice::AddWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd    *pOptS = pStd->m_pDomOptionStd;

	double dScale = pOptS->m_SP_Scale.GetAt(1);
	CDomyun Dom(pDom);

	DrawWebSpliceBx(&Dom, pBx, TRUE);
	Dom.SetScaleDim(dScale);
	DimWebSpliceBx(&Dom, pBx, TRUE);

	Dom.RedrawByScale(GetScale()/dScale);
	*pDom << Dom;
}

void CAPlateOutSplice::AddWebSpliceTable(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);

	CDomyun Dom(pDom);
	double dScale = 20;
	Dom.SetScaleDim(dScale);
	DrawWebTable(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/dScale);
	*pDom << Dom;
}

void CAPlateOutSplice::AddLowerSplice(CDomyun *pDom,  CPlateBasicIndex *pBx, long nIdxOnPage)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);

	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp *pGir		= (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice    *pSp     = (CPlateSplice*)pBx->GetSplice();
	COptionStd    *pOptS = pStd->m_pDomOptionStd;

	if(pBx == NULL) return;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());
	
	CDomyun Dom(pDom);
	double dScale = pOptS->m_SP_Scale.GetAt(2);
	Dom.SetScaleDim(dScale);
	DrawPyung.DrawLowerFlangeSplice(&Dom, pBx, 0, 0, FALSE, FALSE, TRUE, FALSE);

//치수선	

	double mA  = pSp->m_mA;
	double mB  = pSp->m_mB;
	double mC  = pSp->m_mC;
	double mC1 = pSp->m_mC1;

	double sA  = pSp->m_sA;
	double sB  = pSp->m_sB;
	double sC  = pSp->m_sC;
	double sC1 = pSp->m_sC1;

	double Width	= (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2 + pSp->m_uC1;
	const	double	UWidth= pSp->m_uA+pSp->m_uB*pSp->m_uC+pSp->m_uC1+pSp->m_uB*pSp->m_uC+pSp->m_uA;	//	상부 현장이음 폭
	const	double	DWidth= pSp->m_dA+pSp->m_dB*pSp->m_dC+pSp->m_dC1+pSp->m_dB*pSp->m_dC+pSp->m_dA; //	하부 현장이음 폭
	const	double	MWidth= (mA+mB*mC+mC1/2)*2;	//	모멘트 플레이트
	const	double	SWidth= (sA+sB*sC+sC1/2)*2;	//	전단   플레이트
	double Gap;	//플레이트와 플레이트 사이의 간격계산에 사용
//	double HGap;	//볼트와 플레이트 사아의 간격계산에 사용(첫번째)
	double Half;	//기준 플레이트(여기서는 SWidth)와의 차이(X좌표로)

	//	상판
	//	가장 긴 X좌표를 구함.
	double MaxX;
	MaxX	= MWidth;// = SWidth
	if(MaxX < UWidth)	MaxX = UWidth;
	if(MaxX < DWidth)	MaxX = DWidth;
	if(MaxX < (SWidth))	MaxX = SWidth;
	Half	= MaxX/2;

	double dWGirPre		= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double dWGirNex		= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double dWidthGir	= min(dWGirPre, dWGirNex);
	double sWidth		= (pSp->m_dA+pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1;//	스플라이스 폭
	//도면설정
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetScaleDim(dScale);
	//우측치수선
	Dom.SetDirection("RIGHT");
	long nDimDan =0;
	double dS = (pSp->m_dW - pSp->m_dE * pSp->m_dF)/2;
	double dEF = dWidthGir - (dS + pSp->m_dE * pSp->m_dF) * 2;
	CString str1, str2;
	if(pSp->m_dE>1)
	{
		str1.Format("%d@%.f", pSp->m_dE, pSp->m_dF);
		str2.Format("=%.f", pSp->m_dE*pSp->m_dF);
	}
	else
	{
		str1.Format("%.f", pSp->m_dF);
		str2 = "";
	}

	Dom.DimMoveTo(Half+Dom.Always(7), -dWidthGir, nDimDan, COMMA(dS));
	Dom.DimLineTo(dS, nDimDan, "", 1);
	Dom.DimLineToExtend(pSp->m_dE*pSp->m_dF, nDimDan, str1, str2);
//	Dom.SetDimTvp(tvp);
	Dom.DimLineTo(dEF, nDimDan, COMMA(dEF));
//	Dom.SetDimTvp(tvpCur);
	Dom.DimLineToExtend(pSp->m_dE*pSp->m_dF, nDimDan, str1, str2);
	Dom.DimLineToOver(dS, nDimDan, COMMA(dS), TRUE);

	nDimDan = 1;
	Dom.DimMoveTo(Half+Dom.Always(8), -dWidthGir, nDimDan, "", FALSE);
	Dom.DimLineTo(dWidthGir, nDimDan, COMMA(dWidthGir));

	//하부치수선
	Dom.SetDirection("BOTTOM");
	str1.Format("%d@%.f", pSp->m_dB, pSp->m_dC);
	str2.Format("=%.f", pSp->m_dB*pSp->m_dC);
	nDimDan =0;
	Dom.DimMoveTo(-sWidth/2, -dWidthGir - Dom.Always(3), nDimDan, COMMA(pSp->m_dA));
	Dom.DimLineTo(pSp->m_dA, nDimDan, "", TRUE);
	Dom.DimLineToExtend(pSp->m_dB*pSp->m_dC, nDimDan, str1, str2);
//	Dom.SetDimTvp(tvp);
	Dom.DimLineTo(pSp->m_dC1, nDimDan, COMMA(pSp->m_dC1), 1);
//	Dom.SetDimTvp(tvpCur);
	Dom.DimLineToExtend(pSp->m_dB*pSp->m_dC, nDimDan, str1, str2);
	Dom.DimLineToOver(pSp->m_dA, nDimDan, COMMA(pSp->m_dA));
	nDimDan =1;
	Dom.DimMoveTo(-sWidth/2, -dWidthGir - Dom.Always(4), nDimDan, "");
	Dom.DimLineTo(pSp->m_dA*2+pSp->m_dB*pSp->m_dC*2+pSp->m_dC1, nDimDan,
		COMMA(pSp->m_dA*2+pSp->m_dB*pSp->m_dC*2+pSp->m_dC1));

	//좌측치수선
	Dom.SetDirection("LEFT");
	nDimDan = 0;
	Gap = dWidthGir - pSp->m_dW * 2;
	Dom.DimMoveTo(-Half-Dom.Always(7), -dWidthGir, nDimDan, "", FALSE);
	Dom.DimLineTo(pSp->m_dW, nDimDan, COMMA(pSp->m_dW));
	Dom.DimLineToExtend(Gap, nDimDan, "", COMMA(Gap));
	Dom.DimLineTo(pSp->m_dW, nDimDan, COMMA(pSp->m_dW));
	//상부

	Dom.RedrawByScale(GetScale()/dScale);
	*pDom << Dom;
}


void CAPlateOutSplice::AddLowerSpliceTable(CDomyun *pDom, CPlateBasicIndex *pBx, long nIdxOnPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);

	CDomyun Dom(pDom);
	double dScale = 20;
	Dom.SetScaleDim(dScale);
	DrawBottomFlangeTable(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/dScale);
	*pDom << Dom;
}

// 복부 현장이음
// Drawing 원점 : 플랜지의 이음면 상단(상판 하면)
void CAPlateOutSplice::DrawWebSpliceBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp *pGir		= (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice			*pSp		= pBx->GetSplice();

	double dLU	= (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2 + pSp->m_uC1;
	double dLD	= (pSp->m_dA+pSp->m_dB*pSp->m_dC)*2 + pSp->m_dC1;
	double length = max(dLU,dLD);
	double dGirHeight = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	CDomyun Dom(pDom);

	CDPoint xyUpStt(length/2 + Dom.Always(5), 0);
	CDPoint xyUpEnd(-length/2 - Dom.Always(5), 0);
	CDPoint xyLoStt(length/2 + Dom.Always(5), -dGirHeight);
	CDPoint xyLoEnd(-length/2 - Dom.Always(5), -dGirHeight);
	//
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(xyLoStt.x, xyLoStt.y-Dom.Always(4.5), xyUpStt.x, xyUpStt.y+Dom.Always(4.5), TRUE, 1.5);
	Dom.CutLightning(xyLoEnd.x, xyLoEnd.y-Dom.Always(4.5), xyUpEnd.x, xyUpEnd.y+Dom.Always(4.5), TRUE, 1.5);
	//
	CAPlateDrawJong Draw(pBridge->m_pDataManage);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	Draw.DrawWebSplice(&Dom, pBx, 0);
	pOpt->SetEnvType(&Dom,HCAD_STLC);
	Draw.DrawFlangeLineWeb(&Dom,pBx);

	double dTU = pGir->GetThickFactByStation(pBx->GetStation(), 0);
	double dTD = pGir->GetThickFactByStation(pBx->GetStation(), 1);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLS);

	long i = 0;
	//상판 볼트 중심선
	for(i=0; i<=pSp->m_uB; i++)
	{
		Dom.MoveTo(-i * pSp->m_uC - pSp->m_uC1/2, pSp->m_uT + dTU + Dom.Always(0.5));
		Dom.LineTo(-i * pSp->m_uC - pSp->m_uC1/2, -pSp->m_uT - Dom.Always(0.5));
		Dom.MoveTo(i * pSp->m_uC + pSp->m_uC1/2, pSp->m_uT + dTU + Dom.Always(0.5));
		Dom.LineTo(i * pSp->m_uC + pSp->m_uC1/2, -pSp->m_uT - Dom.Always(0.5));
	}
	//하판 볼트 중심선
	for(i=0; i<=pSp->m_dB; i++)
	{
		Dom.MoveTo(-i * pSp->m_dC - pSp->m_dC1/2, -dGirHeight + pSp->m_dT + Dom.Always(0.5));
		Dom.LineTo(-i * pSp->m_dC - pSp->m_dC1/2, -dGirHeight - pSp->m_dT - dTD - Dom.Always(0.5));
		Dom.MoveTo(i * pSp->m_dC + pSp->m_dC1/2, -dGirHeight + pSp->m_dT + Dom.Always(0.5));
		Dom.LineTo(i * pSp->m_dC + pSp->m_dC1/2, -dGirHeight - pSp->m_dT - dTD - Dom.Always(0.5));
	}
	*pDom << Dom;
}


// WEB 이음판 치수선
void CAPlateOutSplice::DimWebSpliceBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(pBx == NULL)	return;

	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateGirderApp *pGir		= (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice			*pSp		= pBx->GetSplice();

	CDomyun Dom(pDom);	
	pOpt->SetEnvType(&Dom,HCAD_DIML);

	double gHeight     = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dLengthWeb  = gHeight;
	double	Tu	  = pGir->GetThickJewon(G_F_U,pBx);
	double	Tl	  = pGir->GetThickJewon(G_F_L,pBx);

	CDPoint	Base;	// Base Point로 사용
	Base.x = Base.y = 0;

	BOOL bMoment = TRUE;	// 모멘트이음판이 없으면 FALSE
	if(pSp->m_mW*pSp->m_mE*pSp->m_mF*pSp->m_mB*pSp->m_mC == 0)	bMoment = FALSE;

	double mA  = pSp->m_mA;
	double mB  = pSp->m_mB;
	double mC  = pSp->m_mC;
	double mC1 = pSp->m_mC1;
	double mG  = pSp->m_mG;
	double mD  = pSp->m_mD;
	double mE  = pSp->m_mE;
	double mF  = pSp->m_mF;
	double mW  = pSp->m_mW;

	double sA  = pSp->m_sA;
	double sB  = pSp->m_sB;
	double sC  = pSp->m_sC;
	double sC1 = pSp->m_sC1;
	double sE  = pSp->m_sE;
	double sF  = pSp->m_sF;
	double sE1 = pSp->m_sE1;
	double sF1 = pSp->m_sF1;
	double sW  = pSp->m_sW;
	
	double Width	= (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2 + pSp->m_uC1;
	const	double	UWidth= pSp->m_uA+pSp->m_uB*pSp->m_uC+pSp->m_uC1+pSp->m_uB*pSp->m_uC+pSp->m_uA;	//	상부 현장이음 폭
	const	double	DWidth= pSp->m_dA+pSp->m_dB*pSp->m_dC+pSp->m_dC1+pSp->m_dB*pSp->m_dC+pSp->m_dA; //	하부 현장이음 폭
	const	double	MWidth= (mA+mB*mC+mC1/2)*2;	//	모멘트 플레이트
	const	double	SWidth= (sA+sB*sC+sC1/2)*2;	//	전단   플레이트
	double Gap	;	//플레이트와 플레이트 사이의 간격계산에 사용
	double HGap;	//볼트와 플레이트 사아의 간격계산에 사용(첫번째)
	double Half;	//기준 플레이트(여기서는 SWidth)와의 차이(X좌표로)
	double DimOff = 250;

	//	상판
	//	가장 긴 X좌표를 구함.
	double MaxX;
	MaxX	= MWidth;// = SWidth
	if(MaxX < UWidth)	MaxX = UWidth;
	if(MaxX < DWidth)	MaxX = DWidth;
	if(MaxX < (SWidth))	MaxX = SWidth;

	Dom.SetDirection("LEFT");
	Half	= MaxX/2;
	//	치수선 Level : 0
	Dom.DimMoveTo(-Half-Dom.Always(7), -dLengthWeb, 0, COMMA(mG));

	if(bMoment) 
	{
		Dom.DimLineTo(mG, 0, _T(""));
		if(mW != 0)	Dom.DimLineTo(mW, 0, COMMA(mW));
		Gap = (dLengthWeb-(mG+mW+sW+mW+mG))/2;
		Dom.DimLineToExtend(Gap, 0, _T(""), COMMA(Gap));
		Dom.DimLineTo(sW, 0, COMMA(sW));
		Gap = (dLengthWeb-(mG+mW+sW+mW+mG))/2;
		Dom.DimLineToExtend(Gap, 0, _T(""), COMMA(Gap));
		if(mW != 0)	Dom.DimLineTo(mW, 0, COMMA(mW));
		Dom.DimLineToOver(mG, 0, COMMA(mG));
	}
	// 모멘트 이음판 치수선 : 없을때 수정
	// REV_HA : 2000.8.3
	else 
	{
		Gap = (dLengthWeb-sW)/2;
		Dom.DimLineTo(Gap, 0, COMMA(Gap));
		Dom.DimLineTo(sW, 0, COMMA(sW));
		Gap = (dLengthWeb-sW)/2;
		Dom.DimLineTo(Gap, 0, COMMA(Gap));
	}

	///////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("RIGHT");
	//	치수선 Level : 0
	CString str1, str2;
	if(bMoment) 
	{
		Dom.DimMoveTo(Half+Dom.Always(7), -dLengthWeb, 0, COMMA(mD));

		Dom.DimLineTo(mD, 0, _T(""));

		str1.Format("%.f@%.f", mE, mF);
		str2.Format("=%.f", mE*mF);
		Dom.DimLineToExtend(mE*mF, 0, str1, str2);
		
		Gap		= (dLengthWeb-(mG+mW+sW+mW+mG))/2;
		HGap	= mW-(mD-mG+mE*mF) + Gap + (sW-sE*sF-sE1*sF1*2)/2;
//		Dom.SetDimTvp(tvp);
		Dom.DimLineTo(HGap, 0, COMMA(HGap));
//		Dom.SetDimTvp(tvpCur);
		
		str1.Format("%.f@%.f=%.f", sE1, sF1, sE1*sF1);
		Dom.DimLineTo(sE1*sF1, 0, str1);
		str1.Format("%.f@%.f=%.f", sE, sF, sE*sF);
		Dom.DimLineTo(sE*sF, 0, str1);
		str1.Format("%.f@%.f=%.f", sE1, sF1, sE1*sF1);
		Dom.DimLineTo(sE1*sF1, 0, str1);
		
		Gap		= (dLengthWeb-(mG+mW+sW+mW+mG))/2;
		HGap	= (sW-sE*sF-sE1*sF1*2)/2 + Gap + mW-(mD-mG+mE*mF);
//		Dom.SetDimTvp(tvp);
		Dom.DimLineTo(HGap, 0, COMMA(HGap));
//		Dom.SetDimTvp(tvpCur);

		str1.Format("%.f@%.f", mE, mF);
		str2.Format("=%.f", mE*mF);
		Dom.DimLineToExtend(mE*mF, 0, str1, str2);
		Dom.DimLineToOver(mD, 0, COMMA(mD));
	}
	// 모멘트 이음판 치수선 : 없을때 수정
	// REV_HA : 2000.8.3
	else 
	{
//		Dom.DimMoveTo(Half+Dom.Always(7), -Tu-dLengthWeb, 0);
		Dom.DimMoveTo(Half+Dom.Always(7), -dLengthWeb, 0);
		Gap		= (dLengthWeb-sE*sF-sE1*sF1*2)/2;
		Dom.DimLineTo(Gap, 0, COMMA(Gap));

		str1.Format("%.f@%.f=%.f", sE1, sF1, sE1*sF1);
		Dom.DimLineTo(sE1*sF1, 0, str1);
		str1.Format("%.f@%.f=%.f", sE, sF, sE*sF);
		Dom.DimLineTo(sE*sF, 0, str1);
		str1.Format("%.f@%.f=%.f", sE1, sF1, sE1*sF1);
		Dom.DimLineTo(sE1*sF1, 0, str1);
		Dom.DimLineTo(Gap, 0, COMMA(Gap));	
	}

	//	치수선 Level : 1
	Dom.DimMoveTo(Half+Dom.Always(8), -dLengthWeb-Tl, 1, COMMA(Tl));
	Dom.DimLineTo(Tl, 1, _T(""));
	Dom.DimLineTo(dLengthWeb, 1, COMMA(dLengthWeb));
	Dom.DimLineToOver(Tu, 1, COMMA(Tu));
	///////////////////////////////////////////////////////////////////////////
	// REV_HA : 2000.8.3
	// 모멘트 이음판 치수선 : 없을때 삭제
	if(bMoment) 
	{
		Dom.SetDirection("TOP");
		Half	= MWidth/2;
		str1.Format("%.f@%.f", mB, mC);
		str2.Format("=%.f", mB*mC);

		Dom.DimMoveTo(-Half, Tu+DimOff/5, 0, COMMA(mA));
		Dom.DimLineTo(mA, 0, _T(""));
		Dom.DimLineToExtend(mB*mC, 0, str1, str2);
//		Dom.SetDimTvp(tvp);
		Dom.DimLineTo(mC1,0, COMMA(mC1));
//		Dom.SetDimTvp(tvpCur);
		Dom.DimLineToExtend(mB*mC, 0, str1, str2);
		Dom.DimLineToOver(mA, 0, COMMA(mA));

		Dom.DimMoveTo(-Half, Tu+DimOff/5, 1);
		Dom.DimLineTo((mA+mB*mC)*2+mC1, 1, COMMA((mA+mB*mC)*2+mC1));
	}
	///////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("BOTTOM");
	Half	= SWidth/2;
	if(sB>1)
	{
		str1.Format("%.f@%.f", sB, sC);
		str2.Format("=%.f", sB*sC);
	}
	else
	{
		str1.Format("%.f", sC);
		str2 = "";
	}
	//치수선 레벨 0
	Dom.DimMoveTo(-Half, -dLengthWeb-Tl-DimOff/5, 0, COMMA(sA));
	Dom.DimLineTo(sA , 0, _T(""));
	Dom.DimLineToExtend(sB*sC, 0, str1, str2);
//	Dom.SetDimTvp(tvp);
	Dom.DimLineTo(sC1, 0, COMMA(sC1));
//	Dom.SetDimTvp(tvpCur);
	Dom.DimLineToExtend(sB*sC,	 0, str1, str2);
	Dom.DimLineToOver(sA, 0, COMMA(sA));
	//치수선 레벨 1
	Dom.DimMoveTo(-Half, -dLengthWeb-Tl-DimOff/5-Dom.Always(1), 1, "");
	Dom.DimLineTo(sA*2+sB*sC*2+sC1, 1, COMMA(sA*2+sB*sC*2+sC1));
	*pDom << Dom;
}


long CAPlateOutSplice::GetPageSu()
{
  	MakeSpliceList();
	m_nPage	= 0;
	long nSplicePerPage = 4;
	long nSpliceTotal = m_SpList.GetSize();

	if(nSpliceTotal%nSplicePerPage == 0)	
		m_nPage = nSpliceTotal/nSplicePerPage;
	else 
		m_nPage = nSpliceTotal/nSplicePerPage+1;

	return m_nPage;	
}

void CAPlateOutSplice::DrawTopFlangeTable(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd  *pStd = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateSplice    *pSp  = (CPlateSplice*)pBx->GetSplice();
	CSteelSection   *pSteelSec	= pStd->m_pDataManage->GetSteelSection();
	CGlobarOption	*pOpt		= pStd->m_pDataManage->GetGlobalOption();

	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	long nRows = 8;
	long nCols = 1;
	double Height = Dom.Always(5);
	double Width = Dom.Always(40);
	CString str;
	Dom.SetRowCount(nRows);
	Dom.SetColumnCount(nCols);
	Dom.SetColumnWidthAll(Width);
	Dom.SetRowHeightAll(Height);
	Dom.SetBasePoint(CDPoint(0,0));

	CString strPL, strPLMat;

	double dSta		  = pBx->GetStation();
	double dWidth     = pSp->m_uA*2+pSp->m_uB*pSp->m_uC*2+pSp->m_uC1;
	double dHeight1   = min(pGir->GetWidthOnStation(dSta, TRUE, FALSE), pGir->GetWidthOnStation(dSta, TRUE, TRUE));
	double dHeight2   = pSp->m_uW;
	double dT         = pSp->m_uT;
	CString sKindBolt = pSp->m_ZzsBoltSize;
	double dLengthBolt= pSp->m_uL;  //Goh
	BOOL bNextFiller  = TRUE;
	double dFillerT	  = fabs(pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick - pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick);

	long nNumPL1 = 1;
	long nNumPL2 = 2;
	long nNumPLF = pOpt->GetDesignHeigtBaseUpper() ? 1 : 2;
	long nNumBolt = GetBoltSu(pBx, 1);

	long nRow = 0;
	long nAlignL = GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER; 
	long nAlignR = GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER;

	Dom.SetGridLines(GVL_VERT);
	for(long n=0; n<4; n++)
	{
		switch(n)
		{
		case 0:
			if(dWidth==0 || dT==0 || dHeight1==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL1, dWidth, dT, dHeight1);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_U_U) +")";
			break;
		case 1:
			if(dWidth==0 || dT==0 || dHeight2==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL2, dWidth, dT, dHeight2);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_U_L) +")";
			break;
		case 2:
			if(dLengthBolt <= 0)					continue;
			strPL.Format("%d H.T.B - %s x %.f", nNumBolt, sKindBolt, dLengthBolt);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_U_BOLT) +")";
			break;
		case 3:
			if(!pBx->IsFiller(BX_UPPERFLANGE, bNextFiller))	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPLF, dWidth/2, dFillerT, dHeight2);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_U_FILL) +")";
		}

		Dom.SetTextMatrix(nRow++, 0, strPL, nAlignL);
		Dom.SetTextMatrix(nRow++, 0, strPLMat, nAlignR);
	}
	
	Dom.Draw(FALSE);
//	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
//	Dom.DrawOutLine();
	*pDom << Dom;

}

void CAPlateOutSplice::DrawBottomFlangeTable(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd  *pStd     = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp *pGir     = (CPlateGirderApp*)pBx->GetGirder();
	CPlateSplice    *pSp      = (CPlateSplice*)pBx->GetSplice();
	CSteelSection   *pSteelSec	= pStd->m_pDataManage->GetSteelSection();
	CGlobarOption	*pOpt		= pStd->m_pDataManage->GetGlobalOption();

	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	long nRows = 8;
	long nCols = 1;
	double Height = Dom.Always(5);
	double Width = Dom.Always(40);
	CString str;
	Dom.SetRowCount(nRows);
	Dom.SetColumnCount(nCols);
	Dom.SetColumnWidthAll(Width);
	Dom.SetRowHeightAll(Height);
	Dom.SetBasePoint(CDPoint(0,0));

	CString strPL, strPLMat;

	double dSta		  = pBx->GetStation();
	double dWidth   = pSp->m_dA*2+pSp->m_dB*pSp->m_dC*2+pSp->m_dC1;
	double dHeight1   = min(pGir->GetWidthOnStation(dSta, FALSE, FALSE), pGir->GetWidthOnStation(dSta, FALSE, TRUE));
	double dHeight2 = pSp->m_dW;
	double dT       = pSp->m_dT;
	CString sKindBolt  = pSp->m_ZzsBoltSize;
	double dLengthBolt = pSp->m_dL;
	BOOL bNextFiller  = TRUE;
	double dFillerT	  = fabs(pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick - pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick);

	long nNumPL1 = 1;
	long nNumPL2 = 2;
	long nNumPLF = !pOpt->GetDesignHeigtBaseLower() ? 1 : 2;
	long nNumBolt = GetBoltSu(pBx, -1);

	long nRow = 0;
	long nAlignL = GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER; 
	long nAlignR = GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER;

	Dom.SetGridLines(GVL_VERT);
	for(long n=0; n<4; n++)
	{
		switch(n)
		{
		case 0:
			if(dWidth==0 || dT==0 || dHeight1==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL1, dWidth, dT, dHeight1);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_L_L) +")";
			break;
		case 1:
			if(dWidth==0 || dT==0 || dHeight2==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL2, dWidth, dT, dHeight2);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_L_U) +")";
			break;
		case 2:
			if(dLengthBolt <= 0)					continue;
			strPL.Format("%d H.T.B - %s x %.f", nNumBolt, sKindBolt, dLengthBolt);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_L_BOLT) +")";
			break;
		case 3:
			if(!pBx->IsFiller(BX_LOWERFLANGE, bNextFiller))	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPLF, dWidth/2, dFillerT, dHeight2);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_L_FILL) +")";
		}

		Dom.SetTextMatrix(nRow++, 0, strPL, nAlignL);
		Dom.SetTextMatrix(nRow++, 0, strPLMat, nAlignR);
	}

	Dom.Draw(FALSE);
//	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
//	Dom.DrawOutLine();
	*pDom << Dom;
}

void CAPlateOutSplice::DrawWebTable(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	CPlateSplice *pSp = (CPlateSplice*)pBx->GetSplice();
	CSteelSection *pSteelSec  = pStd->m_pDataManage->GetSteelSection();

	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	long nRows = 10;
	long nCols = 1;
	double Height = Dom.Always(5);
	double Width = Dom.Always(40);
	CString str;
	Dom.SetRowCount(nRows);
	Dom.SetColumnCount(nCols);
	Dom.SetColumnWidthAll(Width);
	Dom.SetRowHeightAll(Height);
	Dom.SetBasePoint(CDPoint(0,0));

	CString strPL, strPLMat;

	double dWidth1	= (pSp->m_mA + pSp->m_mB * pSp->m_mC) * 2 + pSp->m_mC1;
	double dHeight1 = pSp->m_mW;
	double dWidth2	= (pSp->m_sA + pSp->m_sB * pSp->m_sC) * 2 + pSp->m_sC1;
	double dHeight2 = pSp->m_sW;
	double dT1 = pSp->m_mT;
	double dT2 = pSp->m_sT;
	BOOL bNextFiller  = TRUE;
	double dFillerT	  = fabs(pBx->GetFactChain(G_W, FALSE)->m_dFactChainThick - pBx->GetFactChain(G_W, TRUE)->m_dFactChainThick)/2;

	CString sKindBolt = pSp->m_ZzsBoltSize;
	double dLengthBolt = pSp->m_sL;

	long nNumPL1 = 4;
	long nNumPL2 = 2;
	long nNumBolt = GetBoltSu(pBx, 0);

	long nRow = 0;
	long nAlignL = GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER; 
	long nAlignR = GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER;

	Dom.SetGridLines(GVL_VERT);
	for(long n=0; n<5; n++)
	{
		switch(n)
		{
		case 0:
			if(dWidth1==0 || dT1==0 || dHeight1==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL1, dWidth1, dT1, dHeight1);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_W_MO) +")";
			break;
		case 1:
			if(dWidth2==0 || dT2==0 || dHeight2==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL2, dWidth2, dT2, dHeight2);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_W_SH) +")";
			break;
		case 2:
			if(dLengthBolt <= 0)					continue;
			strPL.Format("%d H.T.B - %s x %.f", nNumBolt, sKindBolt, dLengthBolt);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_W_BOLT) +")";
			break;
		case 3:
			if(!pBx->IsFiller(BX_WEB, bNextFiller))	continue;
			if(dWidth1==0 || dT1==0 || dHeight1==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL1, dWidth1/2, dFillerT, dHeight1);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_W_FILL) +")";
			break;
		case 4:
			if(!pBx->IsFiller(BX_WEB, bNextFiller))	continue;
			if(dWidth2==0 || dT2==0 || dHeight2==0)	continue;
			strPL.Format("%d PL - %.f x %.f x %.f", nNumPL2, dWidth2/2, dFillerT, dHeight2);
			strPLMat = "(" + pSteelSec->GetMaterialByMarkBuje(SP_W_FILL) +")";
			break;
		}

		Dom.SetTextMatrix(nRow++, 0, strPL, nAlignL);
		Dom.SetTextMatrix(nRow++, 0, strPLMat, nAlignR);
	}

	Dom.Draw(FALSE);
//	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
//	Dom.DrawOutLine();
	*pDom << Dom;
}

CString CAPlateOutSplice::GetTitle()
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CString szTitle;

	long nPageSu = m_nPage;
	if(nPageSu>1)
		szTitle.Format("현장이음 상세도 (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("현장이음 상세도");

	return szTitle;	

}

void CAPlateOutSplice::AddSpTiTle(CDomyun *pDom, long subPage, long nIdxOnPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBasicIndex   *pBx  = GetBxPage(subPage,nIdxOnPage);
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_SUB_TITLE);
	CString str = DimSpTitle(&Dom,pBx);

	Dom.TextCircle(0,0,str);
	*pDom << Dom;

}

CString CAPlateOutSplice::DimSpTitle(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateSplice			*pSp  = pBx->GetSplice();
	CPlateGirderApp *pGir		= (CPlateGirderApp*) pBx->GetGirder();

	CString sTitle = _T("");
	BOOL AllGirder  = pStd->m_pDomOptionStd->m_SP_bAllGirder;
	double dHeight = pGir->GetHeightGirderByStaAng(pBx->GetStation());

	long nSttGir = AllGirder ? pBx->GetNumberGirder() : 0;
	long nEndGir = AllGirder ? pBx->GetNumberGirder() : pBridge->GetGirdersu()-1;

	for(long nG=nSttGir; nG<= nEndGir; nG++)
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBxSpl = Finder.GetBxFirst(BX_SPLICE);
		CString sGir;
		sGir.Format("G%d:",pBxSpl->GetNumberGirderEx()+1);
		sTitle += sGir;
		long nSplPre = -1;
		long nSplCur = -1;
		long count = 0;
		while(pBxSpl)
		{
			CString sSp;
			if((*pSp==*pBxSpl->GetSplice()) && (dHeight==pGir->GetHeightGirderByStaAng(pBxSpl->GetStation())))
			{
				count++;
				nSplCur = pBxSpl->GetNumberSpliceEx();
				sSp.Format("SP%d", nSplCur+1);
				if(nSplCur!=nSplPre+1 || nSplPre==-1)
					sTitle = sTitle + sSp + ",";
				else
				{
					if(Finder.GetBxFixNext() && *Finder.GetBxFixNext()->GetSplice() == *pSp)
						sTitle = sTitle;
					else
					{
						sTitle = sTitle.Left(sTitle.GetLength()-1);	
						sTitle = sTitle+"~" + sSp+",";
					}
				}
				
				nSplPre = nSplCur;
			}
			pBxSpl = Finder.GetBxNext();
		}
		if(!count)
			sTitle = sTitle.Left(sTitle.GetLength()-sGir.GetLength());
	}
	sTitle = sTitle.Left(sTitle.GetLength()-1);
	
	return sTitle;
}

CPlateBasicIndex* CAPlateOutSplice::GetBxPage(long nSubPage, long nIdxOnPage)
{
	long nSpTotal = (nSubPage-1)*4+nIdxOnPage;

	if(nSpTotal>=m_SpList.GetSize()) 
		return NULL;
	
	CPlateBasicIndex *pBx = m_SpList.GetAt(nSpTotal)->GetBx();
	return pBx;
}

long CAPlateOutSplice::MakeSpliceList()
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;

	m_SpList.RemoveAll();

	long nGirderSu = pStd->m_pDB->GetGirdersu();
	BOOL AllGirder  = pStd->m_pDomOptionStd->m_SP_bAllGirder;

	double gHeight, gHeightComp;
	for(long nG=0; nG< nGirderSu; nG++)
	{
		CPlateGirderApp *pGir = (CPlateGirderApp*)pStd->m_pDB->GetGirder(nG);
		CPlateBxFinder  Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
		while(pBx)
		{
			CPlateSplice *pSp  = pBx->GetSplice();
			long nSp     = pSp->GetSelfCount();

			CPlateGirderApp	*pGirFir  = (CPlateGirderApp*)pSp->GetGirder();
			gHeight = pGirFir->GetHeightGirderByStaAng(pBx->GetStation());
			long i = 0;
			for(i=0; i<m_SpList.GetSize(); i++)
			{
				CPlateSplice *pSpComp = m_SpList.GetAt(i);
				long nSpComp = pSpComp->GetSelfCount();

				CPlateGirderApp	*pGirComp = (CPlateGirderApp*)pSpComp->GetGirder();
				CPlateBxFinder  FinderComp(pGirComp);
				CPlateBasicIndex *pBxComp = FinderComp.GetBxFirst(BX_SPLICE);
				
				gHeightComp = pGirComp->GetHeightGirderByStaAng(pBxComp->GetStation());

				if((*pSp==*pSpComp) && (gHeight==gHeightComp))
				{
					if (AllGirder)
					{
						long nGFir = pGirFir->GetBxBySpliceNo(nSp)->GetNumberGirder();
						long nGComp = pGirComp->GetBxBySpliceNo(nSpComp)->GetNumberGirder();

						if (nGFir != nGComp)
							continue;
						else
							break;
					}
					else
						break;
				}
			}
			if(i==m_SpList.GetSize() && pSp->IsTrue() && pSp->m_bSpliceType)
				m_SpList.Add(pSp);
			pBx = Finder.GetBxNext();
		}
	}
	
	return m_SpList.GetSize();	

}

double CAPlateOutSplice::GetScale()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt = pStd->m_pDomOptionStd;

	double Scale  = pOpt->m_SP_Scale.GetAt(0);

	return Scale;
}

void CAPlateOutSplice::AddMaterialTable(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;	
	CDataManage		*pData	= pStd->GetDataManage();
	COptionStd		*pOpt	= pStd->GetDomOptionStd();
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->GetCurPage());
	CBMCreator		*pBMStd	= pData->GetBMStandard();
	CGlobarOption	*pGlopt	= pData->GetGlobalOption();
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);

	double dTitleWidth[] = {Dom.Always(30), Dom.Always(35), Dom.Always(18), Dom.Always(18), Dom.Always(18),
						    Dom.Always(18), Dom.Always(22), Dom.Always(24), Dom.Always(22)};

	CPtrList pBMList;
	CPtrList pBMRecordList;
	CPtrList pBxList;
	double	dHeight = 8.0;
	long	nExtRow	= 0;
	long	k		= 0;
	for(k = 0; k < 4; k++)
	{
		CPlateBasicIndex *pBx = GetBxPage(nSubPage, k);
		if(pBx)
		{
			nExtRow += 1;
			pBxList.AddTail(pBx);
		}
		else	break;
		pBMStd->GetBMListByBxAndFlagSang(&pBMList, pBx, pBx, BX_SPLICE);
		POSITION	pos		= pBMList.GetHeadPosition();
		for(long i=0; i<pBMList.GetCount(); i++)
		{
			CBMRecord *pRec = (CBMRecord*)pBMList.GetNext(pos);
			if(pRec->m_nMarkBuje!=SP_W_SH && pRec->m_nQty<1)	continue;
			pBMRecordList.AddTail(pRec);
		}
		pBMList.RemoveAll();
	}

	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetColumnCount(9);	
	Dom.SetRowCount(pBMRecordList.GetCount()+nExtRow+1);
	Dom.SetRowHeightAll(Dom.Always(dHeight));

	for(long i = 0; i < Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i, dTitleWidth[i]);
	
	Dom.SetScaleDim(GetScale());
	Dom.SetMargin(3);
	Dom.SetRowHeight(0,Dom.Always(dHeight*1.5));
	Dom.SetTextMatrix(0, 0, "구분",					GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(0, 1, "부재명",				GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(0, 2, "너비\n(mm)",			GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);		
	Dom.SetTextMatrix(0, 3, "두께\n(mm)",			GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);		
	Dom.SetTextMatrix(0, 4, "길이\n(mm)",			GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);	
	Dom.SetTextMatrix(0, 5, "수량\n(개)",			GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);		
	Dom.SetTextMatrix(0, 6, "단위중량\n(kgf/m3)",	GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(0, 7, "총중량\n(kgf)",		GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(0, 8, "재질",					GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
	long nRoundTot   = pGlopt->GetPointValueOfGangJae();			///< 총중량 강재 반올림 자리수
	long nRoundUnit  = pGlopt->GetPointValueOfUnitGangJae();		///< 단위중량 강재 반올림 자리수

	CString szText;
	long nRowMin = 1;
	CBMRecord *pRecPre = NULL;
	double dTotalWeight = 0;
	long	nIdx = 0;
	POSITION posBx = pBxList.GetHeadPosition();

	for(k =0; k < pBxList.GetCount(); k++)
	{
		CPlateBasicIndex *pBx = (CPlateBasicIndex*)pBxList.GetNext(posBx);
		pBMList.RemoveAll();
		pBMStd->GetBMListByBxAndFlagSang(&pBMList, pBx, pBx, BX_SPLICE);
		POSITION pos = pBMList.GetHeadPosition();
		long i = 0;
		for(i=0;i < pBMList.GetCount();i++)
		{

			if(pos==NULL) continue;
			CBMRecord *pRec = (CBMRecord *)pBMList.GetNext(pos);
			if(pRecPre==NULL) pRecPre = pRec;

			if(pRec->GetStrEleName() == pRecPre->GetStrEleName())
			{
				if(pRec!=pRecPre)
					Dom.SetMergeRow(nRowMin+nIdx,i+nIdx+1, 0);
			}
			else	nRowMin = i+1;

			dTotalWeight += pRec->m_dTWeight;
			Dom.SetTextMatrix(nRowMin+nIdx, 0, pRec->GetStrEleName(),	GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
			Dom.SetTextMatrix(i+1+nIdx, 1, pRec->GetStrSubEleName(),	GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);	
			Dom.SetTextMatrix(i+1+nIdx, 2, pRec->GetStrWidth(),			GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER);	
			Dom.SetTextMatrix(i+1+nIdx, 3, pRec->GetStrThick(),			GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER);
			Dom.SetTextMatrix(i+1+nIdx, 4, pRec->GetStrLength(),		GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER);
			Dom.SetTextMatrix(i+1+nIdx, 5, pRec->GetStrQty(),			GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER);
			Dom.SetTextMatrix(i+1+nIdx, 6, pRec->GetStrUWeight(nRoundUnit),			GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER);
			Dom.SetTextMatrix(i+1+nIdx, 7, pRec->GetStrTWeight(FALSE, nRoundTot),	GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER);
			Dom.SetTextMatrix(i+1+nIdx, 8, pRec->GetStrMaterial(),					GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);

			pRecPre=pRec;
		}
		
		Dom.SetMergeCol(i+nIdx+1, 0, 1);
		szText.Format("총중량 (%s)", DimSpTitle(&Dom, pBx));
		Dom.SetTextMatrix(i+nIdx+1, 0, szText, GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
		szText = GetStringOutValue(dTotalWeight,nRoundTot);
		Dom.SetTextMatrix(i+nIdx+1, 7,szText, GRID_ALIGN_RIGHT | GRID_ALIGN_VCENTER);
		nIdx += pBMList.GetCount()+1;
		nRowMin = 1;
		dTotalWeight=0;
	}
	
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();
	
	AhPtrListDelete(pBMList,(CBMRecord*)0);
	pBMRecordList.RemoveAll();

	*pDom << Dom;
}
