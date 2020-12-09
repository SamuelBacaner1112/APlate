// APlateOutPaintMap.cpp: implementation of the CAPlateOutPaintMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DanmyunGen		10
#define Danmyun			20
#define Splice			30
#define CrossBeam		40

#define TABLE_GEN		1
#define TABLE_CROSS		2
#define TABLE_SEC		3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutPaintMap::CAPlateOutPaintMap()
{

}

CAPlateOutPaintMap::~CAPlateOutPaintMap()
{

}

void CAPlateOutPaintMap::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
		
	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// 일반도이전의 도면번호
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	
	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
	pStd->GetDogak()->Add(&Dom,nPage,m_Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);
	CDRect dgkRect = Dom.GetExtRect(), DomBound;

	switch(nSubPage)
	{
		case 1:
			if(pDomOpt->m_bPaint_GirGen)
				AddGeneral(&Dom);
			if(pDomOpt->m_bPaint_Section)
				AddDanmyun(&Dom);
			break;
		case 2:
			if(pDomOpt->m_bPaint_Splice)
				AddSplice(&Dom);
			if(pDomOpt->m_bPaint_Cross)
				AddCrossBeam(&Dom);
			break;
	}

	*pDom << Dom;

	pPageMng->SetProgressData("출력완료",100);
}

long CAPlateOutPaintMap::GetPageSu()
{
	m_nPage = 2;
	return m_nPage;
}

double CAPlateOutPaintMap::GetScale()
{
	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;
	
	m_Scale = pOpt->m_dPaint_Scale_GirGen;
	return m_Scale;
	//return 25;
}

CString CAPlateOutPaintMap::GetTitle()
{
 	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nPageSu = m_nPage;
	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("도 장 상 세 도 (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("도 장 상 세 도");
//	csTitle.Format("도 장 상 세 도 (%d)",nSubPage);

	return szTitle;
}

//도장상세 일반도...
void CAPlateOutPaintMap::AddGeneral(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);	

	double dyPos = 450;
	double dxPos = 100;
	double Scale = pOpt->m_dPaint_Scale_GirGen;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);
	// 일반
	if(pOpt->m_bPaint_GirGen)
	{
		pPageMng->SetProgressData("정면도(일반) 출력중...",30);
		DrawGeneral(&Dom,pBx);
		Dom.SetCalcExtRect();
		CDPoint Base(0,0);
		CDRect rect = Dom.GetExtRect();
		Base.x = rect.left;
		Base.y = rect.top-pDom->Always(5);
		DimTextNoteGen(&Dom,Base, TABLE_GEN);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+140),pDom->Always(dyPos));
		rect = Dom.GetExtRect();
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen,"정면도(일반)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "정 면 도",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen+1,"정면도(일반) 제목");
		*pDom << Dom;
	}
	// 표
	if(pOpt->m_bPaint_TableBox)
	{
		pPageMng->SetProgressData("도장산출근거(일반) 출력중...",50);
		Dom.SetScaleDim(GetScale());
		DrawTableGen(&Dom);
		Dom.Move(pDom->Always(dxPos+400),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen+2,"도장산출근거(일반)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		Dom.SetTextAlignHorz(TA_LEFT);
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "도장 산출 근거 : 일반",TRUE,FALSE,0);
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen+3,"도장산출근거(일반) 제목");
		*pDom << Dom;
	}	
}

//도장상세 단면도...
void CAPlateOutPaintMap::AddDanmyun(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);	

	double dyPos = 200;
	double dxPos = 150;
	double Scale = pOpt->m_dPaint_Scale_Section;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);
	// 일반
	if(pOpt->m_bPaint_Section)
	{
		pPageMng->SetProgressData("정면도(단면) 출력중...",70);
		DrawSection(&Dom,pBx, 1);		
		Dom.SetCalcExtRect();
		CDPoint Base(0,0);
		CDRect rect = Dom.GetExtRect();
		Base.x = rect.left;
		Base.y = rect.top-pDom->Always(5);
		DimTextNoteGen(&Dom,Base, TABLE_SEC);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos),pDom->Always(dyPos));
		rect = Dom.GetExtRect();
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun,"정면도(단면)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "정 면 도",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+1,"정면도(단면) 제목");
		*pDom << Dom;

		Dom.SetScaleDim(Scale);
		DrawSection(&Dom,pBx, 2);		
		Dom.SetCalcExtRect();	
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+200),pDom->Always(dyPos));
		rect = Dom.GetExtRect();
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+2,"정면도(단면)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "정 면 도",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+3,"정면도(단면) 제목");
		*pDom << Dom;
	}
	// 표
	if(pOpt->m_bPaint_TableSection)
	{
		pPageMng->SetProgressData("도장산출근거(단면) 출력중...",90);
		Dom.SetScaleDim(GetScale());
		DrawTableSec(&Dom);
		Dom.Move(pDom->Always(dxPos+350),pDom->Always(dyPos+5));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+4,"도장산출근거(단면)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		Dom.SetTextAlignHorz(TA_LEFT);
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "도장 산출 근거 : 단면",TRUE,FALSE,0);
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+5));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+5,"도장산출근거(단면) 제목");
		*pDom << Dom;
	}

}

//도장상세 현장이음...
void CAPlateOutPaintMap::AddSplice(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CPlateBasicIndex	*pBx = pGir->GetBxBySpliceNoSPType(0);

	//모두 현장 용접일 경우....070320...KB...
	if (pGir->GetQtySplice(CPlateGirder::SPLICE) < 1) return;

	double dyPos = 480;
	double dxPos = 100;

	double Scale = pOpt->m_dPaint_Scale_Splice;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);
	if(pOpt->m_bPaint_SpliceUp)
	{
		pPageMng->SetProgressData("상판이음판 출력중...",60);
		DrawFlangeSplice(&Dom,pBx,TRUE);		
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos-20),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice,"상판이음판");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "상  판",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+1,"상판이음판 제목");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_SpliceWeb)
	{
		pPageMng->SetProgressData("측판이음판 출력중...",70);
		DrawWebSplice(&Dom,pBx);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+100),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+2,"측판이음판");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "측  판",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+3,"측판이음판 제목");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_SpliceDown)
	{
		pPageMng->SetProgressData("하판이음판 출력중...",80);
		DrawFlangeSplice(&Dom,pBx,FALSE);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+220),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice+4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+4,"하판이음판");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "하  판",TRUE,TRUE,0);
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+5));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+5,"하판이음판 제목");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_TableSplice)
	{
		pPageMng->SetProgressData("도장산출근거(현장이음) 출력중...",90);
		Dom.SetScaleDim(GetScale());
		DrawTableSplice(&Dom);
		Dom.Move(pDom->Always(500),pDom->Always(dyPos+25));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice+6));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+6,"도장산출근거(현장이음)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "도장 산출 근거 : 현장이음",TRUE,FALSE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+7));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+7,"도장산출근거(현장이음) 제목");
		*pDom << Dom;
	}

}

//도장상세 가로보..
void CAPlateOutPaintMap::AddCrossBeam(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	while(pBx)
	{
		CPlateCrossBeam	*pC	= pBx->GetCrossBeam();
		if(pC)
		{
			break;
		}
		pBx = Finder.GetBxNext();
	}

	if(pBx==NULL)
		return;

	double dyPos = 260;
	double dxPos = 60;

	double Scale = pOpt->m_dPaint_Scale_Cross;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	if(pOpt->m_bPaint_CrossPlan)
	{
		pPageMng->SetProgressData("가로보 평면도 출력중...",70);
		DrawCrossBeamPlan(&Dom,pBx);		
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam,"가로보 평면도");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "평면도", TRUE, TRUE, 0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+1,"가로보 평면도 제목");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_CrossFront)
	{
		pPageMng->SetProgressData("가로보 정면도 출력중...",80);
		DrawCrossBeamFront(&Dom,pBx);		
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos),pDom->Always(dyPos-100));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+2,"가로보 정면도");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "정면도", TRUE, TRUE, 0);				
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+3,"가로보 정면도 제목");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_TableCross)
	{
		pPageMng->SetProgressData("가로보 도장산출근거 출력중...",90);
		Dom.SetScaleDim(GetScale());
		DrawTableCROSSBEAM(&Dom);
		Dom.SetCalcExtRect();
		CDPoint Base(0,0);
		CDRect rect = Dom.GetExtRect();
		Base.x = rect.left;
		Base.y = rect.top-pDom->Always(5);
		DimTextNoteGen(&Dom,Base, TABLE_CROSS);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(500),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+4,"가로보 도장산출근거");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "도장 산출 근거 : 가로보", TRUE, FALSE, 0);			
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+5));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+5,"가로보 도장산출근거 제목");
		*pDom << Dom;
	}
}

void CAPlateOutPaintMap::DrawCrossBeamPlan(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;	
	CAPlateDrawCross DrawCross(pStd->GetDataManage());
	
	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);	
	DrawCross.DrawInputCRBeamPlan(&Dom, pBx, TRUE);	

	*pDom << Dom;	
}

void CAPlateOutPaintMap::DrawCrossBeamFront(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawCross DrawCross(pStd->GetDataManage());		

	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
		
	DrawCross.DrawInputCRBeamFront(&Dom, pBx, TRUE);

	*pDom << Dom;	
}

void CAPlateOutPaintMap::InsertDomyun(CDomyun *pDom, long nSubPage)
{
	CString csSection       = "Afx Directory";
	CString csWorkItem		= "Fixed Directory";
	CString szPath = AfxGetApp()->GetProfileString(csSection, csWorkItem);
	
	CString szName;	
	szName.Format("%s\\BlockDomyun\\Painter_Detail(%d).dxf",szPath,nSubPage);	

	CDomyun Dom(pDom);
	
	Dom.DxfIn(szName);
	Dom.Move(Dom.Always(80), Dom.Always(80));
	*pDom << Dom;
}

void CAPlateOutPaintMap::DrawFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper)
{
	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;		
	CAPlateDrawPyung	DrawPyung(pStd->GetDataManage());

	CDomyun Dom(pDom);
	if(bUpper)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawUpperFlangeSplice(&Dom, pBx, 0, 0, FALSE, FALSE, TRUE);
	}
	else
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLH);
		DrawPyung.DrawLowerFlangeSplice(&Dom, pBx, 0, 0, FALSE, FALSE, TRUE);
	}
		
	*pDom << Dom;
}

void CAPlateOutPaintMap::DrawWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawJong		DrawJong(pStd->GetDataManage());
	
	CDomyun Dom(pDom);
	DrawJong.DrawInputDetSpliceWeb(&Dom, 0, 0, FALSE, TRUE);
	
	*pDom << Dom;
}

void CAPlateOutPaintMap::DrawGeneral(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp		*pDB	= pStd->m_pDB;
	CPlateGirderApp		*pGir	= pBx->GetGirder();	

	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());
	DrawDanmyun.DrawGirderOnBx(&Dom, pBx, TRUE);
	DrawDanmyun.DrawFrontUpperStudOrAnchorBx(&Dom, pBx, pBx->GetGirder()->m_pSangse->m_StudHori_Type);
	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	Dom.Move(-A[2].x, -A[2].y);
	*pDom << Dom;	
}
void CAPlateOutPaintMap::DrawSection(CDomyun *pDom, CPlateBasicIndex *pBx, long nMode)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp		*pDB	= pStd->m_pDB;
	CPlateGirderApp		*pGir	= pBx->GetGirder();	

	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());
	DrawDanmyun.DrawGirderOnBx(&Dom, pBx, TRUE, nMode);//수직보강재 그리기...	
	DrawDanmyun.DrawFrontUpperStudOrAnchorBx(&Dom, pBx, pBx->GetGirder()->m_pSangse->m_StudHori_Type);
	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	Dom.Move(-A[2].x, -A[2].y);
	*pDom << Dom;	
}

void CAPlateOutPaintMap::DimTextNoteGen(CDomyun *pDom, CDPoint Base, long nMode)
{
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TXTG);
	Dom.SetBasePoint(Base);
	long nRow = 1;
	long nCol = 3;
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(5));
	Dom.SetColumnWidthAll(Dom.Always(14));
	Dom.SetColumnWidth(1,Dom.Always(5));
	Dom.SetTextMatrix(0,0,"NOTE)");
	long nAlign = GRID_ALIGN_LEFT|GRID_ALIGN_VCENTER;
	Dom.SetTextMatrix(0,1,"1.",nAlign);	
	switch(nMode)
	{
		case TABLE_GEN:
			Dom.SetTextMatrix(0,2,"전단연결재 : HB=STUD의 높이, D=STUD의 직경",nAlign);	
			break;
		case TABLE_CROSS:
			Dom.SetTextMatrix(0,2,"D : ANCHOR BAR의 직경, HB : ANCHOR BAR의 높이",nAlign);	
			break;
		case TABLE_SEC:
			Dom.SetTextMatrix(0,2,"TV : 수직보강재 두께",nAlign);	
			break;
	}
	
	Dom.SetGridLines(GVL_NONE);

	Dom.Draw();
	*pDom << Dom;	
}

void CAPlateOutPaintMap::DrawTableSec(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 4;
	long nCol = 6;	
	CString sData[] = {
			"수직보강재",  "WV*HV*2+HV*TV",       "WV*HV*2+HV*TV",       "", "", "",
			"수직보강재*", "WV-HV-WV*H2/2+HV*TV", "WV-HV-WV*H2/2+HV*TV", "", "", "",
			"동바리용고리","L*W*2+(L+W*2)*T",     "L*W*2+(L+W*2)*T",     "", "", ""			
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"구분",
							"1) 외부 도장\n  (공 장)",
							"2) 외부 도장\n  (현 장)",
							"3) 연결판 도장\n (공 장)",
							"4) 외부볼트및\n  연결도장(현장)",
							"5) 외부포장면\n  도장(공장)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;

}
void CAPlateOutPaintMap::DrawTableSplice(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 10;
	long nCol = 6;	
	CString sData[] = {
			"상면이음판 (상판)",	"",                "",					"W*L*2+(W+L)*T*2", "",				"",
			"상면이음판 (하판)",	"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"하면 이음판",			"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"모멘트 이음판",		"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"전단 이음판",			"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"전단 이음판 (연결형)",	"",                "-W1*L1*2-W2*L2",	"W*L-(W1-W2)*L2\n+(W1*2-W2+L)*2*T", "W*L+(W+L)*2*T",	"",
			"브레이싱 이음판",		"W*L*2+(W+L)*T*2", "W*L*2+(W+L)*T*2", "", "", "",
			"채움판",				"",                "",                  "",                "(W/2+L)*2*T",     "",
			"채움판 (연결형)",		"",                "",                  "",                "(W+L)*2*T\n+(W1-W2)*T",     ""
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"구분",
							"1) 외부 도장\n  (공 장)",
							"2) 외부 도장\n  (현 장)",
							"3) 연결판 도장\n (공 장)",
							"4) 외부볼트및\n  연결도장(현장)",
							"5) 외부포장면\n  도장(공장)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;

}
void CAPlateOutPaintMap::DrawTableCROSSBEAM(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 17;
	long nCol = 6;	
	CString sData[] = {
			"주플레이트",       "GH1*L*2",                 "GH1*L*2",                 "",                    "",                "",
			"상플레이트",       "GB1*L",                   "GB1*L",                   "",                    "",                "GB1*L+L*T*2",
			"하플레이트",       "GB1*L*2+L*T*2",           "GB1*L*2+L*T*2",           "",                    "",                "",
			"수직보강재",       "GB1*GH1*2+GH1*T",         "GB1*GH1*2+GH1*T",         "",                    "",                "",
			"수평보강재",       "GB1*L*2+L*T",             "GB1*L*2+L*T",             "",                    "",                "",
			"상부이음판(상면)", "",                        "",                        "GB1*L*2+(GB1+L)*T*2", "",                "",
			"하부이음판(하면)", "",                        "-GB1*L",                  "GB1*L*2+(GB1+L)*T*2", "GB1*L+(GB1+L)*T*2", "",
			"하부이음판",       "",                        "-GB1*L",                  "GB1*L*2+(GB1+L)*T*2", "GB1*L+(GB1+L)*T*2", "",
			"웨브이음판",       "",                        "-GB1*L",                  "GB1*L*2+(GB1+L)*T*2", "GB1*L+(GB1+L)*T*2", "",
			"ANCHOR BAR",       "",                        "",                        "",                    "",                "PI*D*HB+PI*(D/2)^2*2",
			"거세트 플레이트",  "GH1*GW2*2",               "GH1*GW2*2",               "",                    "",                "",
			"거세트 상판",      "(GB1+GB2)/2*GW1",         "(GB1+GB2)/2*GW1",         "",                    "",                "(GB1*GB2)/2*GW1+GL1*T*2",
			"거세트 하판",      "(GB1+GB3)/2*GW2+GL2*T*2", "(GB1+GB3)/2*GW2+GL2*T*2", "",                    "",                "",
			"거세트 보강판",    "GH2*GW2*2",               "GH2*GW2*2",               "",                    "",                "",
			"거세트 보강재",    "W*L*2+(W+L)*T*2",         "GB1*L*2+(W+L)*T*2",       "",                    "",                "",
			"거세트 공제",      "GH2*GW3/2*2",             "GH2*GW3/2*2",             "",                    "",                ""			
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"구분",
							"1) 외부 도장\n  (공 장)",
							"2) 외부 도장\n  (현 장)",
							"3) 연결판 도장\n (공 장)",
							"4) 외부볼트및\n  연결도장(현장)",
							"5) 외부포장면\n  도장(공장)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;	
}
void CAPlateOutPaintMap::DrawTableGen(CDomyun *pDom)
{
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 8;
	long nCol = 6;	
	CString sData[] = {
			"상부플랜지",  "(WU-T)*LA",        "(WU-T)*LA",        "", "", "(WU+TU*2)*LA",
			"하부플랜지",  "(WL*2-t+TL*2)*LA", "(WL*2-t+TL*2)*LA", "", "", "",
			"웨브플랜지",  "LA*H*2",           "LA*H*2",           "", "", "",
			"수평보강재",  "WH*LA*2+LA*TH",    "",                 "", "", "",
			"SOLE PLATE",  "WP*LP+(WP+LP)*T*2","WP*LP+(WP+LP)*T*2","", "", "",
			"전단 연결재", "",                 "",                 "", "", "PI*D*HB+PI*(D/2)^2",
			"FACE PLATE",  "W*L*2+(W+L)*T*2",  "W*L*2+(W+L)*T*2",  "", "", ""
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"구분",
							"1) 외부 도장\n  (공 장)",
							"2) 외부 도장\n  (현 장)",
							"3) 연결판 도장\n (공 장)",
							"4) 외부볼트및\n  연결도장(현장)",
							"5) 외부포장면\n  도장(공장)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;
}
void CAPlateOutPaintMap::ReplaceData(CDomyun *pDom)
{

}
