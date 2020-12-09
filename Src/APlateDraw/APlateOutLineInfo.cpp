// APlateOutLineInfo.cpp: implementation of the CAPlateLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LINEINFO_PLANE           10
#define LINEINFO_PLANE_TABLE     20
#define LINEINFO_PLANE_TABLE_IP  30
#define LINEINFO_JONGDAN         80
#define LINEINFO_PYUNGUBAE       90

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutLineInfo::CAPlateOutLineInfo()
{

}

CAPlateOutLineInfo::~CAPlateOutLineInfo()
{

}
 
void CAPlateOutLineInfo::DrawOnePage()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();
	COptionStd		*pOpt		= pStd->m_pDomOptionStd;

	long nPage    = pStd->m_nCurPage;
	long nPrePage = m_nSttPage;
	long nSubPage = nPage - nPrePage;

	long nLineSu        = pLineApp->GetLinesu();
	long nLineInfoPage  = nLineSu + 1;
	long nJongDanPage   = nLineSu + nLineInfoPage;
	long nPyungubaePage = nLineSu + nJongDanPage ;

	double Scale  = GetScale();
	
	CDomyun *pDomP = pStd->GetDomyun(nPage);
	pDomP->SetScaleDim(Scale);

	CDomyun *pDom  = new CDomyun(pDomP);

	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
	pStd->GetDogak()->Add(pDom,nPage,Scale,pStd->m_pDomOptionStd->m_szDGKName,pOpt->m_sHead_DomyunNumber,pOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(pDom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);

	if(m_nPage == 3)
	{
		if(nSubPage == 1)
		{
			AddLineInfoDiagram(pDom, 0);
			AddLineInfoTable(pDom, 0);
			AddLineInfoTableIP(pDom, 0);
		}

		if(nSubPage == 2)
			AddJongdanDiagram(pDom, 0);

		if(nSubPage == 3)
			AddPyungubaeDiagram(pDom, 0);
	}
	else
	{
		if(nSubPage <= nLineInfoPage)
		{
			AddLineInfoDiagram(pDom, nSubPage - 1);

			if(nSubPage - 1 != 0)
			{
				AddLineInfoTable(pDom, nSubPage - 2);
				AddLineInfoTableIP(pDom, nSubPage - 2);
			}
		}

		if(nSubPage > nLineInfoPage && nSubPage <= nJongDanPage)
			AddJongdanDiagram(pDom, nSubPage - nLineInfoPage - 1);

		if(nSubPage > nJongDanPage  && nSubPage <= nPyungubaePage)
			AddPyungubaeDiagram(pDom, nSubPage - nJongDanPage - 1);
	}

	pPageMng->SetProgressData("출력완료",100);	

	*pDomP << *pDom;
	delete pDom;
}

long CAPlateOutLineInfo::GetPageSu()
{
	CAPlateDrawStd* pStd  = (CAPlateDrawStd*)m_pStd;
	COptionStd*    pOpt  = pStd->m_pDomOptionStd;
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();

	long nLineSu = pLineApp->GetLinesu();

	m_nPage = (nLineSu * 3);

	if(nLineSu > 1)
		m_nPage = (nLineSu * 3) + 1;

	return m_nPage;
}

double CAPlateOutLineInfo::GetScale()
{
 	CAPlateDrawStd* pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd*    pOpt = pStd->m_pDomOptionStd;
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();

	long nSubPage = pStd->m_nCurPage - m_nSttPage;

	long nLineSu        = pLineApp->GetLinesu();
	long nLineInfoPage  = nLineSu + 1;
	long nJongDanPage   = nLineSu + nLineInfoPage;
	long nPyungubaePage = nLineSu + nJongDanPage ;

	double dScale  = 300;

	if(m_nPage == 3)
	{
		if(nSubPage == 1) dScale = pOpt->m_dLineInfo_PlaneScale;
		if(nSubPage == 2) dScale = pOpt->m_dLineInfo_JongDanHScale;
		if(nSubPage == 3) dScale = pOpt->m_dLineInfo_PyungubaeScale;
	}
	else
	{
		if(nSubPage <= nLineInfoPage)
			dScale = pOpt->m_dLineInfo_PlaneScale;

		if(nSubPage > nLineInfoPage && nSubPage <= nJongDanPage)
			dScale = pOpt->m_dLineInfo_JongDanHScale;

		if(nSubPage > nJongDanPage  && nSubPage <= nPyungubaePage)
			dScale = pOpt->m_dLineInfo_PyungubaeScale;
	}

	return dScale;
}

CString CAPlateOutLineInfo::GetTitle()
{
 	CAPlateDrawStd* pStd  = (CAPlateDrawStd*)m_pStd;
	COptionStd*    pOpt  = pStd->m_pDomOptionStd;
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();

	long nSubPage = pStd->m_nCurPage - m_nSttPage;

	long nLineSu        = pLineApp->GetLinesu();
	long nLineInfoPage  = nLineSu + 1;
	long nJongDanPage   = nLineSu + nLineInfoPage;
	long nPyungubaePage = nLineSu + nJongDanPage ;

	CString csTitle = "선형제원도";

	if(m_nPage == 3)
	{
		if(nSubPage == 1) csTitle = "평 면 선 형 제 원 도";
		if(nSubPage == 2) csTitle = "종 단 선 형 제 원 도";
		if(nSubPage == 3) csTitle = "편 경 사 제 원 도"   ;
	}
	else
	{
		if(nSubPage <= nLineInfoPage)
		{
			if(nSubPage == 1)
				csTitle = "평 면 선 형 제 원 도 ( 전 체 )";
			else
				csTitle.Format("평 면 선 형 제 원 도 ( %d )", nSubPage - 1 + (pOpt->m_nDomTitleArray.GetAt(PAGE_LINEINFO) - 1));
		}

		if(nSubPage > nLineInfoPage && nSubPage <= nJongDanPage)
			csTitle.Format("종 단 선 형 제 원 도 ( %d )", nSubPage - nLineInfoPage + (pOpt->m_nDomTitleArray.GetAt(PAGE_LINEINFO) - 1));

		if(nSubPage > nJongDanPage  && nSubPage <= nPyungubaePage)
			csTitle.Format("편 경 사 제 원 도 ( %d )", nSubPage - nJongDanPage + (pOpt->m_nDomTitleArray.GetAt(PAGE_LINEINFO) - 1));
	}

	return csTitle;
}

void CAPlateOutLineInfo::AddLineInfoDiagram(CDomyun *pDom, long nLine)
{
 	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->m_nCurPage);
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();
	CLineInfo		*pLine	= pLineApp->GetLine(nLine);

	CDomyun Dom(pDom);

	BOOL bTotal = TRUE;

	pDom->SetCalcExtRect();
	Dom.SetCalcExtRect();

	CAPlateDrawLineInfo DrawLine(pStd->GetDataManage());

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	DrawLine.DrawLineInfoPlan(&Dom, nLine, bTotal);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DrawLine.DrawLineInfoPlanChainUnit(&Dom, nLine, bTotal);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DrawLine.DrawLineInfoPlanInfo(&Dom, nLine, bTotal);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DrawLine.DrawLineInfoPlanInfoIP(&Dom, nLine, bTotal);

	if(pStd->m_pDomOptionStd->m_bLineInfo_PlanGrid)
	{
		double dUnit = pLine->m_dUnitCoordinate;

		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
		DrawLine.DrawLineInfoCoordinate(&Dom, nLine, dUnit);
	}

	CDRect Main, Item;
	Main = pDom->GetExtRect();
	Item = Dom.GetExtRect();

	CDPoint pXY, tXY;

	pXY.x = Item.Width() /2.0;
	pXY.y = Item.Height()/2.0;

	tXY.x = Main.Width() /2.0;
	tXY.y = Main.Height()/2.0;

	Dom.Move(Main.left - Item.left, Main.top - Item.top);
	Dom.Move(tXY.x - pXY.x, tXY.y - pXY.y);
	Dom.Move(pMove->GetPoint(LINEINFO_PLANE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_PLANE, "평면선형 제원");

	*pDom << Dom;

	//Title
	CString sSubTitle;

	if(m_nPage != 3 && nLine == 0)
		sSubTitle = "전 체 선 형";
	else
	{
		if(nLine == 0)	nLine = 1;

		pLine = pLineApp->GetLine(nLine-1);
		sSubTitle.Format("%s", pLine->GetNameLineEach());
	}

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), sSubTitle,TRUE, FALSE, 0);	
	Dom.Move(pMove->GetPoint(LINEINFO_PLANE+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_PLANE+1,"평면선형 제원 제목");

	*pDom << Dom;
}

void CAPlateOutLineInfo::AddLineInfoTable(CDomyun *pDom, long nLine)
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd *)m_pStd;
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->m_nCurPage);

	CDomyun Dom(pDom);

	pDom->SetCalcExtRect();
	Dom.SetCalcExtRect();

	CDRect Main, Item;
	Main = pDom->GetExtRect();
	Item = Dom.GetExtRect();

	CDPoint TableBase;
	TableBase.x = Main.right;
	TableBase.y = Main.bottom;

	CAPlateDrawLineInfo DrawLine(pStd->GetDataManage());
	
	if(pStd->m_pDomOptionStd->m_bLineInfo_PlanInfoTable) // 평면 제원 성과표
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
		DrawLine.DrawLineInfoTable(&Dom, nLine, TableBase);
	}

	CDPoint pXY;

	pXY.x = Item.right  - Dom.Always(255);
	pXY.y = Item.bottom - Dom.Always(25);

	Dom.Move(pXY.x, pXY.y);
	Dom.Move(pMove->GetPoint(LINEINFO_PLANE_TABLE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_PLANE_TABLE,"평면제원 성과표");

	*pDom << Dom;

	// Title
	CString sSubTitle = "평 면 제 원 성 과 표";

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), sSubTitle,TRUE, FALSE, 0);	
	Dom.Move(pMove->GetPoint(LINEINFO_PLANE_TABLE+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_PLANE_TABLE+1,"평면제원 성과표 제목");

	*pDom << Dom;
}

void CAPlateOutLineInfo::AddLineInfoTableIP(CDomyun *pDom, long nLine)
{
 	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->m_nCurPage);
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();
	CLineInfo		*pLine	= pLineApp->GetLine(nLine);

	CDomyun Dom(pDom);

	pDom->SetCalcExtRect();
	Dom.SetCalcExtRect();

	if(pStd->m_pDomOptionStd->m_bLineInfo_PlanInfoTable_IP) // IP   제원 성과표
	{
		long nTotalIP = pLine->GetQtyIp();

		CDRect Main, Item;
		Main = pDom->GetExtRect();
		Item = Dom.GetExtRect();

		CDPoint TableBase;
		TableBase.x = Main.left;
		TableBase.y = Main.top;

		CDPoint pXY;

		pXY.x = Item.left + Dom.Always(5);
		pXY.y = Item.top  + Dom.Always(90);

		CAPlateDrawLineInfo DrawLine(pStd->GetDataManage());

		for(long nIP = 0; nIP < nTotalIP; nIP++)
		{
			CLineSplit* pLineSplit = pLine->GetLineSplitByIp(nIP);

			BOOL bClothoid = (pLineSplit->m_dA1 == 0 && pLineSplit->m_dA2 == 0) ? FALSE : TRUE;

			pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
			DrawLine.DrawLineInfoTableIP(&Dom, nLine, nIP, TableBase);

			Dom.Move(pXY.x, pXY.y);
			Dom.Move(pMove->GetPoint(LINEINFO_PLANE_TABLE_IP + nIP));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), LINEINFO_PLANE_TABLE_IP + nIP, "IP제원 TABLE");

			*pDom << Dom;

			// Title
			if(nIP == 0)
			{
				CString sSubTitle = "I P 제 원 T A B L E";

				CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
				DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), sSubTitle,TRUE, FALSE, 0);	
				Dom.Move(pMove->GetPoint(LINEINFO_PLANE_TABLE_IP+nTotalIP));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_PLANE_TABLE_IP+nTotalIP,"IP제원 TABLE 제목");

				*pDom << Dom;
			}

			if(!bClothoid)	pXY.x += Dom.Always(105);
			else			pXY.x += Dom.Always(235);
		}
	}
}

void CAPlateOutLineInfo::AddJongdanDiagram(CDomyun *pDom, long nLine)
{
 	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->m_nCurPage);
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();
	CLineInfo		*pLine	= pLineApp->GetLine(nLine);

	CDomyun Dom(pDom);

	double dScaleH = pStd->m_pDomOptionStd->m_dLineInfo_JongDanHScale;
	double dScaleV = pStd->m_pDomOptionStd->m_dLineInfo_JongDanVScale;
	double dRatio  = dScaleH / dScaleV;

	double dStaStt = 0, dStaEnd = 0;

	BOOL bUser = FALSE;

	pDom->SetCalcExtRect();
	Dom.SetCalcExtRect();

	CAPlateDrawLineInfo DrawLine(pStd->GetDataManage());

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetLineColor(7);
	DrawLine.DrawJongDanPlan(&Dom, dStaStt, dStaEnd, dRatio, nLine, bUser);

	if(pStd->m_pDomOptionStd->m_bLineInfo_JongDanLine)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetLineColor(5);
		DrawLine.DrawJongDanPlanLine(&Dom, dStaStt, dStaEnd, dRatio, nLine, bUser);
	}

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextColor(4);
	DrawLine.DrawJongDanPlanLineInfo(&Dom, dStaStt, dStaEnd, dRatio, nLine, bUser);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetLineColor(1);
	Dom.SetTextColor(4);
	DrawLine.DrawJongDanPlanInfoVIP(&Dom, dStaStt, dStaEnd, dRatio, nLine, bUser);

	CDRect Main, Item;
	Main = pDom->GetExtRect();
	Item = Dom.GetExtRect();

	CDPoint pXY, tXY;

	pXY.x = Item.Width() /2.0;
	pXY.y = Item.Height()/2.0;

	tXY.x = Main.Width() /2.0;
	tXY.y = Main.Height()/2.0;

	Dom.Move(Main.left - Item.left, Main.top - Item.top);
	Dom.Move(tXY.x - pXY.x, tXY.y - pXY.y);
	Dom.Move(pMove->GetPoint(LINEINFO_JONGDAN));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_JONGDAN,"종단선형 제원");

	*pDom << Dom;

	// Title
	CString sSubTitle;
	sSubTitle.Format("%s", pLine->GetNameLineEach());

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), sSubTitle,TRUE, FALSE, 0);	
	Dom.Move(pMove->GetPoint(LINEINFO_JONGDAN+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_JONGDAN+1,"종단선형 제원 제목");

	*pDom << Dom;
}

void CAPlateOutLineInfo::AddPyungubaeDiagram(CDomyun *pDom, long nLine)
{
 	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->m_nCurPage);
	CDataManage		*pDataMng	= pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pDataMng->GetLineInfoApp();
	CLineInfo		*pLine	= pLineApp->GetLine(nLine);

	CDomyun Dom(pDom);

	double dStaStt = 0, dStaEnd = 0;

	double dRatio = frM(pStd->m_pDomOptionStd->m_dLineInfo_PyungubaeRatio);

	int nCase = 0; // 0 : 좌/우측, 1 : 좌측, 2 : 우측

	BOOL bTotal =  TRUE;
	BOOL bUser  = FALSE;
	double dChainUnit = 20;

	pDom->SetCalcExtRect();
	Dom.SetCalcExtRect();

	BOOL bLeft  = pStd->m_pDomOptionStd->m_bLineInfo_PyungubaePlanLeft;
	BOOL bRight = pStd->m_pDomOptionStd->m_bLineInfo_PyungubaePlanRight;

	CAPlateDrawLineInfo DrawLine(pStd->GetDataManage());

	if(bLeft)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
		Dom.SetLineColor(7);
		DrawLine.DrawPyungubaePlan(&Dom, dStaStt, dStaEnd, dRatio, nLine, 1, bUser);
	}

	if(bRight)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLH);
		Dom.SetLineColor(7);
		DrawLine.DrawPyungubaePlan(&Dom, dStaStt, dStaEnd, dRatio, nLine, 2, bUser);
	}

	BOOL bLeftInfo  = pStd->m_pDomOptionStd->m_bLineInfo_PyungubaePlanInfoLeft;
	BOOL bRightInfo = pStd->m_pDomOptionStd->m_bLineInfo_PyungubaePlanInfoRight;

	if(bLeftInfo)
		if(bRightInfo) nCase = 0;
		else           nCase = 1;
	else               nCase = 2;

	if(bLeftInfo || bRightInfo)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetLineColor(1);
		Dom.SetTextColor(4);
		DrawLine.DrawPyungubaePlanInfo(&Dom, dStaStt, dStaEnd, dRatio, nLine, nCase, bTotal, bUser);
	}

	if(pStd->m_pDomOptionStd->m_bLineInfo_PyungubaePlanLineInfo)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetLineColor(1);
		Dom.SetTextColor(4);
		DrawLine.DrawPyungubaeLineInfo(&Dom, dStaStt, dStaEnd, dRatio, nLine, nCase, bTotal, bUser);
	}

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetLineColor(5);
	Dom.SetTextColor(4);
	DrawLine.DrawPyungubaePlanCenterLine(&Dom, dStaStt, dStaEnd, dRatio, nLine, dChainUnit, nCase, bUser);

	if(pStd->m_pDomOptionStd->m_bLineInfo_PyungubaePlanChainUnit)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetLineColor(2);
		Dom.SetTextColor(4);
		DrawLine.DrawPyungubaePlanUnitChain(&Dom, dStaStt, dStaEnd, dRatio, nLine, dChainUnit, nCase, bTotal, bUser);
	}

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetLineColor(1);
	Dom.SetTextColor(4);
	DrawLine.DrawPyungubaeRatioBar(&Dom, dStaStt, dStaEnd, dRatio, nLine, nCase, bTotal, bUser);

	CDRect Main, Item;
	Main = pDom->GetExtRect();
	Item = Dom.GetExtRect();

	CDPoint pXY, tXY;

	pXY.x = Item.Width() /2.0;
	pXY.y = Item.Height()/2.0;

	tXY.x = Main.Width() /2.0;
	tXY.y = Main.Height()/2.0;

	Dom.Move(Main.left - Item.left, Main.top - Item.top);
	Dom.Move(tXY.x - pXY.x, tXY.y - pXY.y);
	Dom.Move(pMove->GetPoint(LINEINFO_PYUNGUBAE));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), LINEINFO_PYUNGUBAE, "편경사 제원");

	*pDom << Dom;

	// Title
	CString sSubTitle;
	sSubTitle.Format("%s", pLine->GetNameLineEach());

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), sSubTitle,TRUE, FALSE, 0);	
	Dom.Move(pMove->GetPoint(LINEINFO_PYUNGUBAE+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),LINEINFO_PYUNGUBAE+1,"편경사 제원 제목");

	*pDom << Dom;
}
