// APlateOutCamber.cpp: implementation of the CAPlateOutCamber class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CAMBER			10
#define CAMBER_LEGEND	20
#define CAMBER_GIRDER	30
#define CAMBER_TITLE	40
#define CAMBER_TABLE	50

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutCamber::CAPlateOutCamber()
{

}

CAPlateOutCamber::~CAPlateOutCamber()
{

}

//////////////////////////////////////////////////////////////////////
// Public
//////////////////////////////////////////////////////////////////////

void CAPlateOutCamber::AddOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB		= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageView	= pStd->GetDrawPageMng();
	COptionStd		*pDomOpt	= pStd->GetDomOptionStd();

	long nPage   = pStd->GetCurPage();
	long nSubPage = nPage - m_nSttPage;
	CDomyun* pDom = pStd->GetDomyun(nPage);

	SetPageData();
	double Scale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	pDom->SetScaleDim(Scale);

	// 도각

	pPageView->SetProgressData("도각 출력중...",0);
	//long nSttPage = pDomOpt->m_nDomPageArray.GetAt(PAGE_CAMBER) == 0 ? 0 : pDomOpt->m_nDomPageArray.GetAt(PAGE_CAMBER) + nSubPage - 1;
	pStd->GetDogak()->Add(pDom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(pDom, pDB->m_sNameBridge, GetTitle(), _T(""), FALSE);
	pDom->SetCalcExtRect();

//	EXIT_RTN;
	pPageView->SetProgressData("CAMBER 출력중...",40);	

	CDPoint Base(0, 0);
	DrawAllCamberDesign(pDom, Base);

//	EXIT_RTN;
	
	pPageView->SetProgressData("출력 완료",100);
}

CString CAPlateOutCamber::GetTitle()
{
	if(m_nPage==0)	return _T("");

	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	
	long nPageSu = m_nPage;
	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("솟 음 도 (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("솟 음 도");
	
	return szTitle;
}
//------------------------ 소제목 쓰기 -------------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::AddSmallTitle(CDomyun *pDomP)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
//	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd* pOpt = pStd->m_pOptStd;
	COutMovePoint*	pMove = pStd->GetMovePoint(pStd->GetCurPage());	

	if(pStd->m_pDomOptionStd->m_nCamber_Type==0 && pStd->m_pDomOptionStd->m_bCamber_AllGirderFull)
		return;
	CString sBurf;	//TEMP CSTRING
	CDomyun Dom(pDomP);

	//제목쓰기...
	pOpt->SetEnvType(&Dom,HCAD_SUB_TITLE);
	CString strSmallTitle;
	strSmallTitle.Format("(GIRDER %d)", m_nG+1);
	CDRect DgkRect = pStd->GetDogak()->GetRectDraw(GetScale());
	Dom.TextOut((DgkRect.left+DgkRect.right)/2, Dom.Always(545), strSmallTitle);
	pOpt->SetEnvType(&Dom, HCAD_HIDD);
	Dom.LineTo(Dom.Always(380), Dom.Always(540),Dom.Always(440), Dom.Always(550));

	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(CAMBER_GIRDER));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER_GIRDER,"거더번호");

	*pDomP << Dom;
}

long CAPlateOutCamber::GetPageSu()
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	  *pGir = pDB->GetGirder(0);
	COptionStd	  *pOpt = pStd->m_pDomOptionStd;
	if(pOpt->m_nCamber_UnitEa<1)
		pOpt->m_nCamber_UnitEa = 1;

	long nTotal  = 0;
	long nGirSu = pDB->GetGirdersu();
	switch(pOpt->m_nCamber_DivType)
	{
		case 0:
			nTotal = pDB->m_nQtyJigan;
			m_nPage = nTotal/pOpt->m_nCamber_UnitEa;
			if(nTotal%pOpt->m_nCamber_UnitEa)
				m_nPage++;
			m_nPage *= nGirSu;
			break;
		case 1:
			m_nPage = nGirSu;
			break;
	}

	if(pOpt->m_nCamber_Type==0)
	{
		if(pOpt->m_bCamber_AllGirder && !pOpt->m_bCamber_AllGirderFull)
			m_nPage = pDB->GetGirdersu();
		else
			m_nPage = 1;
	}

	return m_nPage;
}

//---------------------- 기초 자료 초기화 ----------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::SetPageData()
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();		

	long CurPage = pStd->GetCurPage() - m_nSttPage;
	double Scale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	long nUnit = pStd->m_pDomOptionStd->m_nCamber_UnitEa;
	if(nUnit<1)
		nUnit = 1;

	long nPerPage = GetPageSu()/pDB->GetGirdersu();
	if(pOpt->m_nCamber_Type==0)
		m_nG = CurPage-1;
	else
		m_nG = (CurPage-1)/nPerPage;
	CPlateGirderApp*	pGir = (CPlateGirderApp*)pDB->GetGirder(m_nG);
	m_YRatio = pGir->GetMaxCamberInGirder();
	if (m_YRatio > 0.0)
		m_YRatio = Scale * 20 / m_YRatio;

	if(pOpt->m_nCamber_Type==0)
	{
		m_SttSta = pGir->GetStationStt();
		m_EndSta = pGir->GetStationEnd();
	}
	else
	{
		CPlateBasicIndex *pBxStt = NULL, *pBxEnd = NULL;
		CPlateBxFinder Finder(pGir);
		long nTotalsu = 0, nSttNum = 0, nEndNum = 0;
		switch(pOpt->m_nCamber_DivType)
		{
		case 0:
				nTotalsu = pDB->m_nQtyJigan+1;
				nSttNum = ((CurPage-1)*nUnit) - ((nTotalsu-1)*m_nG);
				nEndNum = nSttNum + nUnit;
				pBxStt = pGir->GetBxOnJijum(nSttNum);
				pBxEnd = pGir->GetBxOnJijum(nEndNum);
				if(nEndNum > nTotalsu)
					pBxEnd = Finder.GetBxLast(BX_END_GIRDER);
			break;
		case 1:
			pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
			pBxEnd = Finder.GetBxLast(BX_END_GIRDER);
			break;
		}

		m_SttSta = pBxStt->GetStation();
		m_EndSta = pBxEnd->GetStation();
	}
	double dSttElev = pGir->GetElevationPlanElOnCenLineAct(m_SttSta);
	double dEndElev = pGir->GetElevationPlanElOnCenLineAct(m_EndSta);
	if (dEndElev == dSttElev)
		m_YRatio2 = 0.0;
	else
		m_YRatio2 = Scale * 0.8 / sqrt(fabs(dEndElev - dSttElev));
}

//--------------------------------------------------------------------
double CAPlateOutCamber::GetMaxCamberInGirder()
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp* pGir = pDB->GetGirder(m_nG);

	long nCamber, nCamberSu = pGir->GetQtyNodeCamber(), nType;
	double cam, sta;
	double dMax = 0.0;

	for (nCamber = 0; nCamber < nCamberSu; nCamber++)   {
		sta = pGir->GetCamberByNode(nCamber)->GetCamberStation();
		for (nType = 0; nType <= 10; nType++)   {
			cam = fabs(pGir->GetCamberByStation(sta, nType));
			dMax = max(cam, dMax);
		}
	}
	return dMax;
}

//--------------------------------------------------------------------
void CAPlateOutCamber::DrawAllCamberFact1(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	COutMovePoint*	pMove = pStd->GetMovePoint(pStd->GetCurPage());	

	double HScale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	double VScale = pStd->m_pDomOptionStd->m_dCamber_VScale;
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(HScale);
	// 계산 솟음량
	DrawDWSCamberDiagram(&Dom, Base);
	DrawDWSCamberDim(&Dom, Base);
	if(pStd->m_pDomOptionStd->m_bCamber_Table)
		DrawDWSCamberTable(&Dom, Base);

	Dom.RedrawByHVScale(HScale/VScale, TRUE);
	Dom.SetCalcExtRect();
	CDRect DgkBound = pStd->GetDogak()->GetRectDraw(GetScale());
	CDRect DomBound = Dom.GetExtRect();

	double MoveX = (DgkBound.left+DgkBound.right)/2 - (DomBound.left+DomBound.right)/2;
	double MoveY = DgkBound.bottom - pDomP->Always(180);
	Dom.Move(MoveX, MoveY);
	Dom.Move(pMove->GetPoint(CAMBER));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER,"솟음량 테이블");
	AddSmallTitle(&Dom);

	*pDomP << Dom;
}

//--------------------------------------------------------------------
void CAPlateOutCamber::DrawAllCamberDesign(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint  *pMove  = pStd->GetMovePoint(pStd->GetCurPage());	

	pDomP->SetCalcExtRect();
	CDRect DomRect = pDomP->GetExtRect();
	CDRect rect(0,0,0,0);
	CDomyun Dom(pDomP);
	double HScale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	double VScale = pStd->m_pDomOptionStd->m_dCamber_VScale;
	Dom.SetScaleDim(HScale);

	if(pOpt->m_bCamber_Table)
	{
		DrawCamberDesign(&Dom, Base);
		Dom.RedrawByHVScale(HScale/VScale, TRUE);
		*pDomP << Dom;

		Base.y = Dom.Always(350);
		Base.x = Dom.Always(50);

		if(pOpt->m_bCamber_AllGirder && !pOpt->m_bCamber_AllGirderFull)
			DrawCamberDesignTable(&Dom, Base, m_nG);
		else
		{
			for(long nG=0; nG < pDB->GetGirdersu(); nG++)
			{
				CDPoint xy = DrawCamberDesignTable(&Dom, Base, nG);
				if(!pOpt->m_bCamber_AllGirder && !pOpt->m_bCamber_AllGirderFull)
					break;
				Base = xy;
			}
		}
	}

	if(pStd->m_pDomOptionStd->m_bCamber_Legend)
	{
		Base.x = Dom.Always(650);
		Base.y = Dom.Always(350);
		if(!pStd->m_pDomOptionStd->m_bCamber_Table)
			Base.y = Dom.Always(500);
		DrawCamberDesignLegend(&Dom, Base);
		*pDomP << Dom;
	}

	if(pStd->m_pDomOptionStd->m_bCamber_Note)
	{
		AddSmallTitle(&Dom);
		*pDomP << Dom;
	}

}


void CAPlateOutCamber::DrawCamberDesign(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp     *pGir   = pDB->GetGirder(m_nG);
	COutMovePoint  *pMove  = pStd->GetMovePoint(pStd->GetCurPage());	
//	CDrawDBGen     *pDBGen = pStd->m_pDrawDBStandard->GetDBGen();
	CAPlateDrawCamber DrawCamber(pStd->GetDataManage());	
	CDomyun Dom(pDomP);
	CDPoint BasePoint, BriPoint, Rate;
	Rate = DrawCamber.GetDrawRate(&Dom, &BriPoint, m_nG);
	double nScale = pDomP->GetScaleDim();
	Dom.SetScaleDim(nScale*Rate.x);
	////////////////////
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CString str;

	BasePoint.x = 0;
	BasePoint.y = 200;
	DrawCamber.AllDrawCamber(&Dom, m_nG, FALSE);

	// 삼각형
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_MIS2);	
	long	gigan,gigansu = pDB->m_nQtyJigan;
	double	sta = 0;

	for(gigan = 0; gigan <= gigansu; gigan++)
	{
		sta = pGir->GetStationOnJijum(gigan);
		Dom.Triangle(BasePoint.x+(sta)*Rate.x,BasePoint.y,120*Rate.x);
	}

	/////////////////
	double VScale = pStd->m_pDomOptionStd->m_dCamber_VScale;
	Dom.RedrawByScale(nScale/(nScale*Rate.x));
	Dom.RedrawByHVScale(nScale/VScale, TRUE);

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	CDRect DgkRect = pStd->GetDogak()->GetRectDraw(GetScale());;
	Dom.Move((DgkRect.left+DgkRect.right)/2 - (rect.left+rect.right)/2, DgkRect.bottom - rect.Height()-nScale*100);
	Dom.Move(pMove->GetPoint(CAMBER));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER,"솟음량");
	*pDomP << Dom;
}

CDPoint CAPlateOutCamber::DrawCamberDesignTable(CDomyun *pDomP, CDPoint Base, long nG)
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
	COptionStd		*pOpt	= pStd->GetDomOptionStd();
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->GetCurPage());

	CDomyun TDom(pDomP);
	CDomyun Dom(pDomP);
	CVectorArray staArr;
	staArr.RemoveAll();
	GetCamberStation(staArr, 3, nG);
	CDRect rect(0,0,0,0);
	CDPoint BasePoint = Base;
	long nSize = staArr.GetSize();
	long nUnit = pOpt->m_nCamber_TableColSu;
	if(nUnit < 1)	return BasePoint;
	long nTable = nSize/nUnit;
	if(nSize%nUnit)	nTable++;

	double dColWidth  = Dom.Always(20);
	double dRowHeight = Dom.Always(8)*pOpt->m_dCamber_TableHScale;

	CStringArray szArrTitle;
	szArrTitle.Add("(1) 종단곡선에 의한 솟음량");
	szArrTitle.Add("(2) 편경사에 의한 솟음량");
	szArrTitle.Add("(3) 강재 자중에 의한 솟음량");
	if(pDB->IsTUGir())
	{
		szArrTitle.Add("(4) 구속콘크리트자중에 의한 솟음량");
		szArrTitle.Add("(5) 180˚Turnning에 의한 솟음량");
		szArrTitle.Add("(6) 합성전 1차사하중에 의한 솟음량");
		szArrTitle.Add("(7) 합성후 2차사하중에 의한 솟음량");
		szArrTitle.Add("(8) 공장제작시 ((3)~(7)의 합계)");
		szArrTitle.Add("(9) 현장거치시 ((8)-(3)-(4)-(5)");
		szArrTitle.Add("(10) 1차사하중 재하시((9)-(6))");
		szArrTitle.Add("(11) 2차사하중 재하시((10)-(7))");
	}
	else
	{
		szArrTitle.Add("(4) 합성전 1차사하중에 의한 솟음량");
		szArrTitle.Add("(5) 합성후 2차사하중에 의한 솟음량");
		szArrTitle.Add("(6) 공장 제작시((3)-(5)의 합계)");
		szArrTitle.Add("(7) 현장 거치시((6)-(3))");
		szArrTitle.Add("(8) 1차사하중 재하시((7)-(4))");
		szArrTitle.Add("(9) 2차사하중 재하시((8)-(5))");
	}

	long nRow = szArrTitle.GetSize()+1;
	long nCol = nUnit;
	long nUnitMM = pDB->m_cCamberUnitToMM;
	double	dUnit	= nUnitMM ? 1 : 0.1;

	for(long nT = 0; nT < nTable ; nT++)
	{
		if((nT+1)*nUnit > nSize)
			nCol = nUnit - ((nT+1)*nUnit - nSize);
		CGridDomyunEx GDom(pDomP);
		pStd->m_pOptStd->SetEnvType(&GDom, HCAD_TBL1);
		GDom.SetBasePoint(BasePoint);
		GDom.SetColumnCount(nCol+1); //Title 열 추가
		GDom.SetRowCount(nRow);
		GDom.SetRowHeightAll(dRowHeight);
		GDom.SetColumnWidthAll(dColWidth);
		GDom.SetColumnWidth(0, pDomP->Always(70));

		GDom.SetTextMatrix(0,0, "구        분");
		long i = 0;
		for(i=1; i<nRow; i++)
			GDom.SetTextMatrix(i, 0, szArrTitle.GetAt(i-1), GRID_ALIGN_LEFT|GRID_ALIGN_VCENTER);

		double sta;
		long nCnt;
		long nIdx = nT*nUnit;
		for (i = 1; i < nCol+1; i++)
		{
			nCnt = 0;
			CVector data = staArr.GetAt(nIdx);
			sta  = data.x;
			CString str;
			str.Format("%d", (nT*nUnit)+i);
			GDom.SetTextMatrix(nCnt, i, str);
			//ⓞ종단 곡선에 의한 솟음량
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			//①편경사에 의한 솟음량
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			//②강재 자중에 의한 솟음량
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			if(pDB->IsTUGir())
			{
				// 구속콘크리트자중에 의한 솟음량
				str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
				GDom.SetTextMatrix(nCnt, i, str);
				// 180도 터닝에 의한 솟음량
				str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
				GDom.SetTextMatrix(nCnt, i, str);
			}
		
			//③합성전 1차사하중에 의한 솟음량
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			//④합성후 2차사하중에 의한 솟음량
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			// 1차 사하중 제하시
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			// 2차 사하중 제하시
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			// 1차 사하중 제하시
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			// 2차 사하중 제하시
			str.Format("%.3f", pGir->GetCamberByStation(sta, nCnt++) * dUnit);
			GDom.SetTextMatrix(nCnt, i, str);
			nIdx++;
		}
		GDom.Draw(FALSE);
		pStd->m_pOptStd->SetEnvType(&GDom, HCAD_TBL2);
		GDom.DrawOutLine();

		BasePoint.y  = BasePoint.y - dRowHeight*nRow - Dom.Always(10);
		GDom.SetCalcExtRect();
		rect = GDom.GetExtRect();
		CString str = nUnitMM ? "단위(mm)" : "단위(cm)";

		double x = rect.right - GDom.GetTextWidth(str) - GDom.Always(1);
		double y = rect.bottom + GDom.GetTextHeight()/2 + GDom.Always(1);
		GDom.TextOut(x, y, str);
		Dom << GDom;

		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CAMBER_TABLE+/*(nG+1)+*/(nT+1)*10*nT));
		str.Format("솟음량 테이블 %d", /*(nG+1)+*/(nT+1)*10*nT);
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER_TABLE+/*(nG+1)+*/(nT+1)*10*nT,str);
		TDom << Dom;

		if(nT==0 && pStd->m_pDomOptionStd->m_bCamber_AllGirderFull)
		{
			TDom.SetCalcExtRect();
			rect = TDom.GetExtRect();
			pStd->m_pOptStd->SetEnvType(&Dom, HCAD_SUB_TITLE);
			str.Format("(GIRDER %d)", nG+1);
			Dom.TextOut((rect.left+rect.right)/2, rect.bottom+Dom.Always(3), str);

			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(CAMBER_TITLE+nG+1));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER_TITLE+nG+1, str);
			TDom << Dom;
		}
	}

	if(pStd->m_pDomOptionStd->m_bCamber_AllGirderFull && nG==0)
	{
		TDom.SetCalcExtRect();
		rect = TDom.GetExtRect();
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_SUB_TITLE);
		Dom.TextOut((rect.left+rect.right)/2, rect.bottom+Dom.Always(5), "솟 음 량");
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CAMBER_TITLE));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER_TITLE, "솟음량 테이블 제목");
		TDom << Dom;
	}
	*pDomP << TDom;
	BasePoint.y  = BasePoint.y - Dom.Always(10);
	return BasePoint;
}

void CAPlateOutCamber::DrawCamberDesignLegend(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	COptionStd		*pOpt	= pStd->GetDomOptionStd();
	COutMovePoint  *pMove  = pStd->GetMovePoint(pStd->GetCurPage());	
	CDomyun Dom(pDomP);
	CGridDomyunEx GDom(pDomP);
	pStd->m_pOptStd->SetEnvType(&GDom, HCAD_TBL1);
	GDom.SetBasePoint(CDPoint(0,0));

	CStringArray szArrTitle;
	szArrTitle.Add("(1) 종단곡선에 의한 솟음량");
	szArrTitle.Add("(2) 편경사에 의한 솟음량");
	szArrTitle.Add("(3) 강재 자중에 의한 솟음량");
	if(pDB->IsTUGir())
	{
		szArrTitle.Add("(4) 구속콘크리트자중에 의한 솟음량");
		szArrTitle.Add("(5) 180˚Turnning에 의한 솟음량");
		szArrTitle.Add("(6) 합성전 1차사하중에 의한 솟음량");
		szArrTitle.Add("(7) 합성후 2차사하중에 의한 솟음량");
		szArrTitle.Add("(8) 공장제작시 ((3)~(7)의 합계)");
		szArrTitle.Add("(9) 현장거치시 ((8)-(3)-(4)-(5)");
		szArrTitle.Add("(10) 1차사하중 재하시((9)-(6))");
		szArrTitle.Add("(11) 2차사하중 재하시((10)-(7))");
	}
	else
	{
		szArrTitle.Add("(4) 합성전 1차사하중에 의한 솟음량");
		szArrTitle.Add("(5) 합성후 2차사하중에 의한 솟음량");
		szArrTitle.Add("(6) 공장 제작시((3)-(5)의 합계)");
		szArrTitle.Add("(7) 현장 거치시((6)-(3))");
		szArrTitle.Add("(8) 1차사하중 재하시((7)-(4))");
		szArrTitle.Add("(9) 2차사하중 재하시((8)-(5))");
	}

	long nCol = 3;
	long nRow = szArrTitle.GetSize()+1;
	GDom.SetColumnCount(nCol);
	GDom.SetRowCount(nRow);
	double ColWidth[]  = {GDom.Always(30),GDom.Always(30),GDom.Always(70)};
	double RowHeight = GDom.Always(8)*pOpt->m_dCamber_TableHScale;
	long	i = 0;
	for(i=0; i<nCol; i++)
		GDom.SetColumnWidth(i,ColWidth[i]);
	GDom.SetRowHeightAll(RowHeight);
	GDom.SetTextMatrix(0,0,"범        례");
	GDom.SetTextMatrix(1,0,"계산 솟음량");
	GDom.SetMergeCol(0,0,2);
	if(pDB->IsTUGir())
	{
		GDom.SetMergeRow(1,7,0);
		GDom.SetMergeRow(8,11,0);
		GDom.SetTextMatrix(8,0,"시공 단계별\n솟음량");
	}
	else
	{
		GDom.SetMergeRow(1,5,0);
		GDom.SetMergeRow(6,9,0);
		GDom.SetTextMatrix(6,0,"시공 단계별\n솟음량");
	}
	for(i=0; i<nRow-1; i++)
		GDom.SetTextMatrix(i+1, 2, szArrTitle.GetAt(i), GRID_ALIGN_LEFT|GRID_ALIGN_VCENTER);
	GDom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&GDom, HCAD_TBL2);
	GDom.DrawOutLine();
	Dom << GDom;

	double nLineWidth = GDom.Always(20);
	CDPoint cp(0,0);
	cp.x = ColWidth[0]+GDom.Always(5);
	cp.y = -RowHeight*3/2;
	// 종단곡선에 의한 조정량
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1,LT_DASHDOTDOT_LINE,ECC_JONGDAN);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	// 편경사에 의한 솟음량
	cp.y -= RowHeight;
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1,LT_SOLID,ECC_DEFAULT1);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	// 강재 자중에 의한 솟음량
	cp.y -= RowHeight;
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1,LT_DOT_LINE,ECC_ONLYSTEEL);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	if(pDB->IsTUGir())
	{
		// 구속콘크리트자중에 의한 솟음량
		cp.y -= RowHeight;
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1, LT_DOT_LINE, ECC_CONF);
		Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

		// 180˚Turnning에 의한 솟음량
		cp.y -= RowHeight;
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1, LT_DOT_LINE, ECC_TURNING);
		Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);
	}

	// 합성전 1차하사중에 의한 솟음량
	cp.y -= RowHeight;
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1,LT_DASH_LINE,ECC_STEEL);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	// 합성후 2차사하중에 의한 솟음량
	cp.y -= RowHeight;
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1,LT_DASHDOT_LINE,ECC_VIVID);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);
	// 공장 제작시
	cp.y -= RowHeight;
	Dom.SetLineColor(ECC_FACT);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	// 현장 거치시
	cp.y -= RowHeight;
	Dom.SetLineColor(ECC_FIELD);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	// 1차사하중 재하시
	cp.y -= RowHeight;
	Dom.SetLineColor(ECC_REMOVESTEEL);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	// 2차사하중 재하시
	cp.y -= RowHeight;
	Dom.SetLineColor(ECC_REMOVEVIVID);
	Dom.LineTo(cp.x, cp.y, cp.x+nLineWidth, cp.y);

	Dom.Move(Base.x, Base.y);
	Dom.Move(pMove->GetPoint(CAMBER_LEGEND));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER_LEGEND,"범례");

	*pDomP << Dom;

}

void CAPlateOutCamber::DrawAllCamberFact2(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	COutMovePoint*	pMove = pStd->GetMovePoint(pStd->GetCurPage());	

	CDomyun Dom(pDomP);
	double HScale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	double VScale = pStd->m_pDomOptionStd->m_dCamber_VScale;
	Dom.SetScaleDim(HScale);

	// 계산 솟음량
	DrawDMCamberUppdrDim(&Dom, Base);
	if(pStd->m_pDomOptionStd->m_bCamber_Table)
		DrawDMCamberTable(&Dom, Base);

	Dom.RedrawByHVScale(HScale/VScale, TRUE);

	Dom.SetCalcExtRect();
	CDRect DgkBound = pStd->GetDogak()->GetRectDraw(GetScale());
	CDRect DomBound = Dom.GetExtRect();

	double MoveX = (DgkBound.left+DgkBound.right)/2 - (DomBound.left+DomBound.right)/2;
	double MoveY = DgkBound.bottom - pDomP->Always(100);
	Dom.Move(MoveX, MoveY);
	DomBound = Dom.GetExtRect();
	Dom.Move(pMove->GetPoint(CAMBER));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER,"솟음량 테이블");
	*pDomP << Dom;

	AddSmallTitle(&Dom);
	*pDomP << Dom;
	//범례추가하기
	if(pStd->m_pDomOptionStd->m_bCamber_Legend)
	{
		AddBumlaeMark(&Dom, Base);
		//내용을 이동
		MoveX = DomBound.right + pDomP->Always(30);
		MoveY = DgkBound.bottom - pDomP->Always(300);
		Dom.Move(MoveX, MoveY);
		Dom.Move(pMove->GetPoint(CAMBER_LEGEND));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CAMBER_LEGEND,"범례");
	}
	*pDomP << Dom;
}

// nSectionType ? 0:둘다 1:다이아프램 2:현장이음을 리턴 3:다이어 프램만 리턴
// staArr[].x : 스테이션
// staArr[].y : 모든 캠버량
// staArr[].z : 앞 지간과의 캠버 기준 높이차
void CAPlateOutCamber::GetCamberStation(CVectorArray &staArr, long nSectionType, long nG)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp     *pGir = pDB->GetGirder(nG);

	//기준선 경사 구하기
//	double dSttElev, dEndElev, dSlop;
	if(pOpt->m_nCamber_Type==0)
	{
		m_SttSta = pGir->GetStationStt();
		m_EndSta = pGir->GetStationEnd();
	}
//	dSttElev = pGir->GetElevationPlanElOnCenLineAct(m_SttSta);
//	dEndElev = pGir->GetElevationPlanElOnCenLineAct(m_EndSta);
//	dSlop = (dEndElev - dSttElev) / (m_EndSta - m_SttSta);

	CVector data;
	staArr.RemoveAll();
/*
	if(pOpt->m_nCamber_Type == 0)
	{
		CPlateBxFinder Finder(pGir);
		__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER;
		if(nSectionType == 3)
			BxFlag |= BX_CROSSBEAM_VBRACING;
		else
		{
			if (nSectionType != 1)
				BxFlag |= BX_SPLICE;
			if (nSectionType != 2)
				BxFlag |= BX_CROSSBEAM_VBRACING;
		}


		CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);

		//스테이션 추가
		while (pBx)   {
			double sta = pBx->GetStation();
			if (m_SttSta <= sta && sta <= m_EndSta)   {
				data.x = pBx->GetStation();	
				staArr.Add(data);	//스테이션을 추가한다
			}
			pBx = Finder.GetBxNext();
		}
	}

	else if(pOpt->m_nCamber_Type == 1 || pOpt->m_nCamber_Type == 2)*/
//	{
		////////////////////////////////////////////////BY CMS 2001. 11. 21
		long nCamb = pGir->GetQtyNodeCamber();
		for(long nSu = 0; nSu < nCamb; nSu++)
		{
			double dSta = pGir->GetCamberByNode(nSu)->GetCamberStation();
			if(nSu == nCamb-1)
				dSta  = pGir->GetStationOnJijum(pDB->m_nQtyJigan);
			if(m_SttSta <= dSta && dSta <= m_EndSta)
			{
				data.x = dSta;	
				staArr.Add(data);	//스테이션을 추가한다
			}
		}
		////////////////////////////////////////////////////////////////////
//	}
/*
	if(pOpt->m_nCamber_Type != 2)
	{
		if (m_SttSta < staArr.GetAt(0).x)   {
			data.x = m_SttSta;
			staArr.InsertAt(0, data);
		}
		if (staArr.GetAt(staArr.GetUpperBound()).x < m_EndSta)   {
			data.x = m_EndSta;
			staArr.Add(data);
		}

		double sta, camber;

		for (long nIdx = 0; nIdx < staArr.GetSize(); nIdx++)   {
			data = staArr.GetAt(nIdx);
			sta  = data.x;
			camber  = pGir->GetCamberByStation(sta, 0);	//ⓞ종단 곡선에 의한 조정량
			camber += pGir->GetCamberByStation(sta, 1);	//①편경사에 의한 조정량
			camber += pGir->GetCamberByStation(sta, 2);	//②강재 자중에 의한 솟음량
			camber += pGir->GetCamberByStation(sta, 3);	//③합성전 1차사하중에 의한 솟음량
			camber += pGir->GetCamberByStation(sta, 4);	//④합성후 2차사하중에 의한 솟음량
			camber += pGir->GetCamberByStation(sta, 5);	//⑨이상 : User입력값
			camber += pGir->GetCamberByStation(sta, 6);
			data.y = camber * m_YRatio;
			data.z = (sta - m_SttSta) * dSlop * m_YRatio2;
			staArr.SetAt(nIdx, data);
		}
	}*/
}

//--------------------------------------------------------------------
//-------------------- 켐버 라인 그리기(대우) ------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::DrawDWSCamberDiagram(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();

	CDomyun Dom(pDomP);

	CVectorArray staArr;
	GetCamberStation(staArr, DIM_BASIC, m_nG);
	long nCount = staArr.GetSize();
	CVector data;
	CString str;

	// 기준선 그리기
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CENT);
	data = staArr.GetAt(0);
	Dom.MoveTo(data.x, data.z);
	data = staArr.GetAt(staArr.GetUpperBound());
	Dom.LineTo(data.x, data.z);

	long nIdx = 0;
	// 지점 심볼 그리기
	for (nIdx = 0; nIdx < nCount; nIdx++)   {
		data = staArr.GetAt(nIdx);
		if(IsSpot(data))
			Dom.Triangle(data.x, data.z, 8);
	}

	// 캠버 Line 그리기
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	data = staArr.GetAt(0);
	Dom.PMoveTo(data.x, data.z + data.y);
	for (nIdx = 1; nIdx < nCount; nIdx++)   {
		data = staArr.GetAt(nIdx);
		Dom.PLineTo(data.x, data.z + data.y);
	}
	Dom.PLineEnd();

	// 절점 Line 그리기
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLH);
	GetCamberStation(staArr, DIM_CAMBER, m_nG);  // 가로보
	nCount = staArr.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)   {
		data = staArr.GetAt(nIdx);
		Dom.MoveTo(data.x, data.z);
		Dom.LineTo(data.x, data.z + data.y);
	}

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	GetCamberStation(staArr, DIM_RECT, m_nG);  // 현장이음
	nCount = staArr.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)   {
		data = staArr.GetAt(nIdx);
		Dom.MoveTo(data.x, data.z);
		Dom.LineTo(data.x, data.z + data.y);
	}

	// 지조립높이 치수선 심볼 그리기
	DrawYSymbol(&Dom);

	Dom.Move(CDPoint(-m_SttSta, 0) + Base);

	*pDomP << Dom;
}

//------------------------- 심볼 그리기 ------------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::DrawYSymbol(CDomyun *pDomP)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;

	CVectorArray staArr;
	CDPoint xy1, xy2, xy3, xy4, vDir;
	CVector data;
	GetCamberStation(staArr, DIM_BASIC, m_nG);

	data = staArr.GetAt(0);
	xy1 = CDPoint(data.x, data.z);
	data = staArr.GetAt(staArr.GetUpperBound());
	xy2 = CDPoint(data.x, data.z);
	vDir = (xy2 - xy1).Unit();

	data = staArr.GetAt(staArr.GetSize() / 2);
	xy3 = CDPoint(data.x, data.z + data.y);
	GetXyMatchLineAndLine(xy3, vDir.Rotate90(), xy1, vDir, xy4);

	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("LEFT");
	Dom.SetDimObq(60);
	Dom.DimMoveTo(xy3.x, xy3.y, 0);
	Dom.DimLineTo(-~(xy3-xy4), 0, "Y");

	Dom.Rotate(xy3, vDir);

	Dom.MoveTo(xy3);
	Dom.LineTo(xy4);
	double dLen = Dom.Always(3);
	Dom.MoveTo(xy4 + vDir * dLen);
	Dom.LineTo(xy4 + vDir * dLen + vDir.Rotate90() * dLen);
	Dom.LineTo(xy4 + vDir.Rotate90() * dLen);

	*pDomP << Dom;

}

//------------------ 캠버 라인 치수선 그리기(대우)--------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::DrawDWSCamberDim(CDomyun *pDomP, CDPoint Base)
{
	/*
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*		pGir = pDB->GetGirder(m_nG);

	CDimDomyunEx DimDom(pDomP);
	pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_DIML);
	DimDom.SetDirection("TOP");

	double dBound = 3500;
	double presta, len;
	long DimDan, nCount;
	CString str, str2;
	CVectorArray staArr;
	CVector data;

	double Scale = pStd->m_pDomOptionStd->m_dCamber_HScale;

	// nSectionType == 1 : 다이아프램
	// nSectionType == 2 : 현장이음
	for (long nSectionType = 0; nSectionType <= 2; nSectionType++)   {
		GetCamberStation(staArr, nSectionType, m_nG);
		nCount = staArr.GetSize();

		DimDan = long (max(staArr.GetAt(0).z, staArr.GetAt(staArr.GetUpperBound()).z) / (Scale * 10)) + 5; //?
		data = staArr.GetAt(0);
		DimDom.DimBasePoint(data.x, data.z + data.y, DimDan + nSectionType - 1);
		DimDom.DimMoveTo(data.x, data.z + data.y);
		for (long nIdx = 1; nIdx < nCount; nIdx++)   {
			presta = data.x;
			data = staArr.GetAt(nIdx);
			len  = pGir->GetLengthDimType(presta, data.x);
			if (data.x - presta >= dBound)   {
				if (data.x - presta == len)
					str.Format("%.1f", len);
				else
					str.Format("%.1f (%.1f)", data.x - presta, len);
				DimDom.DimLineTo(data.x - presta, data.z + data.y, str);
			}
			else   {
				str.Format("%.1f", data.x - presta);
				if (data.x - presta == len)
					str2.Format("");
				else
					str2.Format("(%.1f)", len);
				DimDom.DimLineTo(data.x - presta, data.z + data.y, str, str2);
			}
		}
	}

	// 전체 치수
	data = staArr.GetAt(0);
	DimDom.DimBasePoint(data.x, data.z + data.y, DimDan + 2);
	DimDom.DimMoveTo(data.x, data.z + data.y);
	presta = data.x;
	data = staArr.GetAt(staArr.GetUpperBound());
	len = pGir->GetLengthDimType(presta, data.x);
	if (data.x - presta == len)
		str.Format("%.1f", len);
	else
		str.Format("%.1f (%.1f)", data.x - presta, len);
	DimDom.DimLineTo(data.x - presta, data.z + data.y, str);

	// 지간번호 마크
	pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_SMALL_TITLE);
	for (long nK = 0; nK <= pDB->m_nQtyJigan; nK++)   {
		double sta = pGir->GetStationOnJijum(nK);
		if (m_SttSta <= sta && sta <= m_EndSta)   
		{
			CSymbolDom	Dom(pDomP, pStd->m_pOptStd);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
			Dom.SetScaleDim(Scale);
			str = pDB->GetMarkSection(SECTION_MAIN,pDB->GetGirder(-1)->GetBxOnJijum(nK));
			CDPoint CP(sta, (DimDan+3)*10 * Scale);
			CP.y += (5*Scale);
			Dom.SymbolTextCircle(CP,str);

			Dom.MoveTo(sta, DimDan*10 * Scale);
			Dom.LineTo(sta, (DimDan+3)*10 * Scale);
			DimDom << Dom;		
		}
	}

	// 현장이음 마크
	pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_MIS2);
	for (long nIdx = 0; nIdx < nCount; nIdx++)   {
		data = staArr.GetAt(nIdx);
		CPlateBasicIndex *pBx = pGir->GetBxByStationDir(data.x, 0, BX_SPLICE | BX_CROSSBEAM_VBRACING);
		if (pBx->IsState(BX_SPLICE))   {
			str.Format("SP%d", pBx->GetSplice()->GetSelfCount() + 1+pDB->GetNumberStartSplice());
			DimDom.TextCircle(data.x, data.z + data.y + Scale * 20, str, 0, TRUE, TRUE, 0);
		}
	}

	DimDom.Move(CDPoint(-m_SttSta, 0) + Base);
	*pDomP << DimDom;
	*/
}

//--------------------------------------------------------------------
//------------------- 캠버 테이블 그리기(대우) -----------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::DrawDWSCamberTable(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*		pGir = pDB->GetGirder(m_nG);

	//SET ENV
	CDomyun Dom(pDomP);
	double Scale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	double VScale = pStd->m_pDomOptionStd->m_dCamber_VScale;
	Dom.SetScaleDim(Scale);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	Dom.SetTextWidthFactor(0.8);

	//SET TITLE
	CStringArray titleArr;
	titleArr.Add("계획고\n(PLAN HEIGHT)");
	titleArr.Add("편경사\n(SIDE SLOPE)");
	titleArr.Add("계획고에서\n복부판상단까지 높이");
	titleArr.Add("종곡선 및 편경사에\n의한 솟음");
	titleArr.Add("강재 자중에\n의한 솟음량");
	titleArr.Add("합성전 1차사하중에\n의한 솟음량");
	titleArr.Add("합성후 2차사하중에\n의한 솟음량");
	titleArr.Add("합계");

	//GET DATA
	CVectorArray staArr;
	CVector data;
	GetCamberStation(staArr, 0, m_nG);
	long nCount = staArr.GetSize();

	//SET TABLE DIM
	CString str;
	CDPoint BaseRow, Origin;
	double HeaderH, HeaderW, MiddleDan;
	double HeaderH1 = Scale * 22;	//LARGE HEIGHT
	double HeaderH2 = Scale * 15;	//SMALL HEIGHT
	double HeaderW1 = (Scale * 20)*(VScale/Scale);
	double HeaderW2 = (Scale * 60)*(VScale/Scale);	//LARGE WEIDTH
	double HeaderW3 = (Scale * 40)*(VScale/Scale);
	double Term   = Scale * 20;
	double Margin = Scale * 10;
	double CamberW = (m_EndSta - m_SttSta) + Margin * 2;
	double TextGap = Scale * 3;

	Origin.x = m_SttSta - (Margin + HeaderW2);
	Origin.y = min(staArr.GetAt(0).z, staArr.GetAt(staArr.GetUpperBound()).z) - Term;
	BaseRow = Origin;

	for (long nRow = 0; nRow < titleArr.GetSize(); nRow++)
	{
		// Row 기본값 Setting.
		HeaderH = HeaderH1;
		if (1 <= nRow && nRow <= 9)
			HeaderH = HeaderH2;
		HeaderW = HeaderW2;
		BaseRow.x = Origin.x;
		if (4 <= nRow && nRow <= 7)
		{
			HeaderW = HeaderW3;
			BaseRow.x = Origin.x + (HeaderW2 - HeaderW3);
		}
		BaseRow.y -= HeaderH;

		// 밑줄그림
		if (nRow < 12)
		{
			Dom.MoveTo(BaseRow);
			if (nRow == 7)
				Dom.MoveTo(Origin.x, BaseRow.y);
			Dom.LineTo(BaseRow.x + HeaderW + CamberW, BaseRow.y);
		}
		// Title 출력
		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.SetTextHeight(3.6);
		CString strText = _T("");
		strText = titleArr.GetAt(nRow);
		TextOutMultiLine(&Dom, BaseRow.x + HeaderW / 2, BaseRow.y + HeaderH / 2, strText);
		// 중간 Title 출력
		if (nRow == 3)
			MiddleDan = BaseRow.y;
		if (nRow == 7)
		{
			Dom.MoveTo(BaseRow.x, MiddleDan);
			Dom.LineTo(BaseRow);
			str = "실시설계\n캠버";
			TextOutMultiLine(&Dom, (Origin.x + BaseRow.x) / 2, (MiddleDan + BaseRow.y) / 2, str);
		}

		// 캠버값 출력
		Dom.SetTextAngle(90);
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.SetTextHeight(3.0);
		for (long nIdx = 0; nIdx < nCount; nIdx++)
		{
			data = staArr.GetAt(nIdx);
			double sta = data.x;
			double camber0 = pGir->GetCamberByStation(sta, 1);
			double camber1 = pGir->GetCamberByStation(sta, 0);
			double camber2 = pGir->GetCamberByStation(sta, 2);
			double camber3 = pGir->GetCamberByStation(sta, 3);
			double camber4 = pGir->GetCamberByStation(sta, 4);

			double camber;
			switch (nRow)
			{
			case 0 :	camber = pGir->GetLine()->GetElevationPlanEl(sta);	break;
			case 1 :	camber = pGir->GetPyungubaeCenLine(sta, 0) * 100;	break;
			case 2 :	camber = pGir->GetElevationFlange(sta) - pGir->GetLine()->GetElevationPlanEl(sta);	break;
			case 3 :	camber = camber0 + camber1;	break;
			case 4 :	camber = camber2;	break;
			case 5 :	camber = camber3;	break;
			case 6 :	camber = camber4;	break;
			case 7 :	camber = camber2 + camber3 + camber4;	break;
			}
			if (nRow == 1)
				str.Format("%.1f%%", camber);
			else
				str.Format("%.1f", camber);
			Dom.TextOut(sta + Dom.GetTextHeight() / 2, BaseRow.y + HeaderH - TextGap, str);
		}
	}

//	Dom.SetCalcExtRect();
//	CDRect rect = Dom.GetExtRect();
//	Dom.LineTo(rect.left , rect.bottom, rect.right, rect.bottom);
//	Dom.LineTo(rect.right, rect.bottom, rect.right, rect.top);
//	Dom.LineTo(rect.right, rect.top   , rect.left , rect.top);
//	Dom.LineTo(rect.left , rect.top   , rect.left , rect.bottom);

	// 외곽선 및 기타 선 그림.
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.Rectangle(Origin.x - HeaderW1, BaseRow.y, Origin.x + HeaderW2 + CamberW, Origin.y);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	Dom.LineTo(Origin.x, Origin.y, Origin.x, BaseRow.y);
	Dom.LineTo(Origin.x + HeaderW2, Origin.y, Origin.x + HeaderW2, BaseRow.y);

	Dom.SetTextAngle(0);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextHeight(4);
	Dom.TextOut(Origin.x - HeaderW1 / 2, (Origin.y + BaseRow.y) / 2, "GIRDER");

	CSymbolDom Sym(&Dom,pStd->m_pOptStd);
	pStd->m_pOptStd->SetEnvType(&Sym, HCAD_TBL1);
	Sym.SetScaleDim(Scale);
	str.Format("G%d", m_nG + 1+pDB->GetNumberStartGirder());
	Sym.DrawSymbolGirderMark(&Dom, Origin.x - HeaderW1 / 2, (Origin.y + BaseRow.y) / 2 - Dom.GetTextHeight() * 2, str, 0);
	Dom << Sym;

	// 캠버도와 테이블사이 기준선 그림.
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = pGir->GetBxByStationDir(m_SttSta, 1, BX_CROSSBEAM_VBRACING);
	Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, NULL, pBx);
	long nDiap = Finder.GetCountBx();
	for (long nIdx = 0; nIdx < nCount; nIdx++)   {
		data = staArr.GetAt(nIdx);
		pBx = pGir->GetBxByStationDir(data.x, 0, BX_CROSSBEAM_VBRACING | BX_SPLICE | BX_STT_GIRDER | BX_END_GIRDER);
		if (pBx->IsState(BX_CROSSBEAM_VBRACING))
		{
			Dom.MoveTo(data.x, data.z - Scale * 1);
			Dom.LineTo(data.x, Origin.y + Scale * 13);

			CSymbolDom	SDom(pDomP, pStd->m_pOptStd);
			pStd->m_pOptStd->SetEnvType(&SDom,HCAD_DIML);
			double ScaleTable = pStd->m_pDomOptionStd->m_dCamber_HScale;
			SDom.SetScaleDim(ScaleTable);
			CDPoint CP  = CDPoint(data.x, Origin.y + ScaleTable * 4);
			CP.y += Scale*5;
			str.Format("%d", nDiap++);
			SDom.SymbolTextCircle(CP, str);
			Dom << SDom;

			Dom.MoveTo(data.x, Origin.y + Scale * 4);
			Dom.LineTo(data.x, Origin.y - Scale * 3);
		}
		else   {
			Dom.MoveTo(data.x, data.z - Scale * 1);
			Dom.LineTo(data.x, Origin.y - Scale * 3);
		}
	}

	Dom.Move(CDPoint(-m_SttSta, 0) + Base);
	*pDomP << Dom;

}

//------------------------- 테이블 제목 쓰기 -------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::TextOutMultiLine(CDomyun *pDom, double x, double y, CString &cs, long nVertAlign)
{
	CStringArray DestArr;	
	int pos;
	while ((pos = cs.Find("\n")) >= 0)   {
		DestArr.Add(cs.Left(pos));
		cs = cs.Mid(pos+1);
	}
	DestArr.Add(cs);	// Last String		
	long nRow = DestArr.GetSize();
	double TextHgt = pDom->GetTextHeight();
	double TextGap = TextHgt * 0.5;

	if (nVertAlign == 0)  // Middle Align
		y += (nRow-1) * (TextHgt+TextGap) / 2 - TextHgt / 2;
	if (nVertAlign == 1)  // Top Align
		y -= TextHgt;
	if (nVertAlign == 2)  // Bottom Align
		y += (nRow-1) * (TextHgt+TextGap);

	for (long n = 0; n < nRow; n++)   {
		pDom->TextOut(x, y, DestArr.GetAt(n));
		y -= TextHgt + TextGap;
	}
}

//----------------------- 상단의 거더 치수(대명) ---------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::DrawDMCamberUppdrDim(CDomyun *pDomP, CDPoint Base)
{
	/*
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*		pGir = pDB->GetGirder(m_nG);

	CDimDomyunEx DimDom(pDomP);
	double Scale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_DIML);
	DimDom.SetDirection("TOP");

	double presta = 0, preSameSta = 0, dHeight = 0, sta=0, OrgSta = 0, SumSta = 0;
	long DimDan, nCount;
	CString str,strDimHead;
	CVectorArray staArr;
	CVector data, dataNext;
	long nReSu = 0;
	BOOL bReContinue = FALSE;

	long nSectionType = 3; //다이어 프램 리턴 받기 위해
	GetCamberStation(staArr, nSectionType, m_nG);
	nCount = staArr.GetSize();
	if(staArr.GetAt(0).x==pGir->GetStationStt())
		staArr.RemoveAt(0);
	if(staArr.GetAt(nCount-1).x==pGir->GetStationEnd())
		staArr.RemoveAt(staArr.GetUpperBound());
	nCount = staArr.GetSize();

	DimDan = 3;
	
	for(long nAgain = 0; nAgain < 2; nAgain++)
	{
		if(nAgain == 0)
			strDimHead = "CAMBER POINT";
		else
			strDimHead = "SPAN LENGTH";
		data = staArr.GetAt(0);
		DimDom.DimBasePoint(data.x, data.y, DimDan + nAgain - 1);
		DimDom.DimMoveTo(data.x, data.y,0,FALSE,strDimHead,DimDom.Always(50));

		presta = staArr.GetAt(0).x;
		dHeight = data.y;
		for (long nIdx = 1; nIdx < nCount; nIdx++)
		{
			if(nAgain == 0 || IsSpot(data))
				presta = data.x;

			data = staArr.GetAt(nIdx);
			if(nIdx != nCount-1)
				dataNext = staArr.GetAt(nIdx+1);
			else
				dataNext = CVector(0,0,0);

			if((data.x-presta) == (dataNext.x-data.x) && nAgain == 0)
			{
				if(nReSu == 0)
					preSameSta = presta;
				nReSu++;
			}
			else
			{
				if(nAgain == 1 && !IsSpot(data) && data!=staArr.GetAt(staArr.GetUpperBound()))
					continue;
				if(nReSu == 0)
				{
					str.Format("%.1f", data.x - presta);
					DimDom.DimLineTo((data.x - presta), dHeight, str);
				}
				else
				{
					str.Format("%d@%.1f = %.1f", (nReSu+1), data.x-presta, (nReSu+1)*(data.x-presta));
					DimDom.DimLineTo((data.x - preSameSta), dHeight, str);
				}
				nReSu = 0;
			}
		}
	}

	// 거더 치수
	data = staArr.GetAt(0);
	DimDom.DimBasePoint(data.x, data.y, DimDan + 1);
	DimDom.DimMoveTo(data.x, data.y,0,FALSE,"TOTAL SPAN LENGTH",DimDom.Always(50));
	presta = data.x;
	data = staArr.GetAt(staArr.GetUpperBound());
	str.Format("%.1f", data.x - presta);
	DimDom.DimLineTo((data.x - presta), data.y, str);

	// 지간번호 마크
	pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_DIML);
	for (long nK = 0; nK <= pDB->m_nQtyJigan; nK++)
	{
		presta = sta;
		sta = pGir->GetStationOnJijum(nK);
		if(nK == 0)
		{
			OrgSta = sta;
		}

		if(nK!=0)
			SumSta += (sta-presta);
		if (m_SttSta <= sta && sta <= m_EndSta)
		{
			CSymbolDom	Dom(pDomP, pStd->m_pOptStd);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
			Dom.SetScaleDim(Scale);
			CDPoint CP(OrgSta+SumSta, (DimDan+3)*10 * Scale);
			CP.y += (5*Scale);
			str = pDB->GetMarkSection(SECTION_MAIN,pDB->GetGirder(-1)->GetBxOnJijum(nK));
			Dom.SymbolTextCircle(CP,str);
			Dom.MoveTo(OrgSta+SumSta, (DimDan+1.5)*10 * Scale);
			Dom.LineTo(OrgSta+SumSta, (DimDan+3)*10 * Scale);
			DimDom << Dom;
		}
	}
	DimDom.Move(CDPoint(-m_SttSta, 0) + Base);
	*pDomP << DimDom;
	*/
}

//----------------- 테이블 그리기(대명) ------------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::DrawDMCamberTable(CDomyun *pDomP, CDPoint Base)
{
	/*
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*		pGir = pDB->GetGirder(m_nG);
	CARoadOptionStd* pOpt = pStd->m_pOptStd;

	CSymbolDom Dom(pDomP, pOpt);
	double Scale = pStd->m_pDomOptionStd->m_dCamber_HScale;
	double sta = 0;
	//횡스케일 관련변수들
	double PreData, OrgData, SumData = 0;
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);

	CStringArray titleArr;
	SetDMTableTitleArr(titleArr);	//테이블 제목 세팅

	CVectorArray staArr;
	CVector data;
	GetCamberStation(staArr, 3, m_nG);
	long nCount = staArr.GetSize();

	CString str;
	CDPoint BaseRow, Origin, xyStt, xyEnd, xyDiaMark, rAng(0,0);
	//테이블에 들어갈 그래프 시작점과 끝점 
	xyStt.x = staArr.GetAt(1).x;
	xyEnd.x = xyStt.x + (staArr.GetAt(staArr.GetUpperBound()).x-xyStt.x);
	xyDiaMark.x = xyEnd.x + Dom.Always(13.5);
	//.., //LARGE HEIGHT, //SMALL HEIGHT, //TITLE WEIDTH
	double HeaderH, HeaderLargeH=Scale*65, HeaderSmallH=Scale*25, HeaderGraphH=Scale*40, HeaderW=Scale*42
		, HeaderVSmallH=Scale*15;	
	double Term=Scale*14, Margin=Scale*7, CamberW=(m_EndSta-m_SttSta)+Margin*2,TextGap=Scale * 2;

	//SET ORIGIN
	Origin.x = m_SttSta - (Margin + HeaderW);
	Origin.y = staArr.GetAt(0).z - Term;
	BaseRow = Origin;

	CPlateBasicIndex *pBxCro = NULL;
	for (long nRow = 0; nRow < titleArr.GetSize(); nRow++)
	{
		SumData = 0;
		// Row 기본값 Setting.
		HeaderH = HeaderSmallH;
		if(nRow == 0 || nRow == 5)
			HeaderH = HeaderLargeH;
		if(nRow == 4 || nRow == 7)
			HeaderH = HeaderGraphH;
		BaseRow.x = Origin.x;
		BaseRow.y -= HeaderH;

		// 계획고
		if (nRow == 0)
		{
			pStd->m_pOptStd->SetEnvLType(&Dom, LT_DASHDOT_LINE);
			xyStt.y = xyEnd.y = xyDiaMark.y = BaseRow.y;
			Dom.LineTo(xyStt, xyEnd);
			pStd->m_pOptStd->SetEnvLType(&Dom, LT_SOLID);
			Dom.LineTo(xyEnd.x, xyEnd.y, xyEnd.x+Dom.Always(9),xyEnd.y);
			Dom.DrawSymbolDiamondMark(xyDiaMark, rAng, "1");
		}

		if (nRow == 4)	//거더 높이 그래프
		{
			pStd->m_pOptStd->SetEnvLType(&Dom, LT_DOT_LINE);
			BaseRow.y += HeaderH/2;
			xyStt.y = xyEnd.y = xyDiaMark.y = BaseRow.y;			
			Dom.LineTo(xyStt, xyEnd);
			//CAMBER LINE
			pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
			data = staArr.GetAt(0);
			OrgData = data.x;
			Dom.PMoveTo(data.x, data.y+BaseRow.y);
			for (long nLineIdx = 1; nLineIdx < nCount; nLineIdx++)
			{
				PreData = data.x;
				data = staArr.GetAt(nLineIdx);
				SumData += (data.x - PreData);
				data.y -= (pGir->GetCamberByStation(data.x, 0) + pGir->GetCamberByStation(data.x, 1));
				Dom.PLineTo(OrgData + SumData,data.y+BaseRow.y);
			}
			Dom.PLineEnd();
			//그래프를 지시하는 마크
			pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
			Dom.LineTo(xyEnd.x, xyEnd.y, xyEnd.x+Dom.Always(9),xyEnd.y);
			Dom.DrawSymbolDiamondMark(xyDiaMark, rAng, "2");
			Dom.LineTo(xyEnd.x, data.y+BaseRow.y, xyEnd.x+Dom.Always(9),xyEnd.y-Dom.Always(9));
			xyDiaMark.y -= Dom.Always(9);
			Dom.DrawSymbolDiamondMark(xyDiaMark, rAng, "3");
		}
		if(nRow ==7)	//솟음값 그래프
		{
			pStd->m_pOptStd->SetEnvLType(&Dom, LT_SOLID);
			BaseRow.y += HeaderH/2;
			xyStt.y = xyEnd.y = xyDiaMark.y = BaseRow.y;			
			Dom.LineTo(xyStt, xyEnd);
			//CAMBER LINE
			pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
			data = staArr.GetAt(0);
			OrgData = data.x;
			Dom.PMoveTo(data.x, data.y+BaseRow.y);
			for (long nLineIdx = 1; nLineIdx < nCount; nLineIdx++)
			{
				PreData = data.x;
				data = staArr.GetAt(nLineIdx);
				SumData += (data.x - PreData);
				Dom.PLineTo(OrgData+SumData,data.y+BaseRow.y);
			}
			Dom.PLineEnd();
			//그래프를 지시하는 마크
			pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
			Dom.LineTo(xyEnd.x, xyEnd.y, xyEnd.x+Dom.Always(9),xyEnd.y);
			Dom.DrawSymbolDiamondMark(xyDiaMark, rAng, "4");
			Dom.LineTo(xyEnd.x, data.y+BaseRow.y, xyEnd.x+Dom.Always(9),xyEnd.y-Dom.Always(9));
			xyDiaMark.y -= Dom.Always(9);
			Dom.DrawSymbolDiamondMark(xyDiaMark, rAng, "5");
		}
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);

		pStd->m_pOptStd->SetEnvLType(&Dom, LT_SOLID);
		// Title 출력
		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		TextOutMultiLine(&Dom, BaseRow.x + HeaderW / 2, BaseRow.y + HeaderH / 2, titleArr.GetAt(nRow));

		if(nRow == 4 || nRow == 7)
			continue;
		// 캠버값 출력
		Dom.SetTextAngle(90);
		Dom.SetTextAlignHorz(TA_CENTER);
		for (long nIdx = 0; nIdx < nCount; nIdx++)
		{
			PreData = sta;
			data = staArr.GetAt(nIdx);
			sta = data.x;
			if(nIdx == 0)
				OrgData = sta;
			if(nIdx!=0)
				SumData += (sta - PreData);
			double camber0 = pGir->GetCamberByStation(sta, 0);
			double camber1 = pGir->GetCamberByStation(sta, 1);
			double camber2 = pGir->GetCamberByStation(sta, 2);
			double camber3 = pGir->GetCamberByStation(sta, 3);
			double camber4 = pGir->GetCamberByStation(sta, 4);
			double camber5 = pGir->GetCamberByStation(sta, 5);
			double camber6 = pGir->GetCamberByStation(sta, 6);
			double camberJolib = pGir->GetCamberByStation(sta, 7) + camber0 + camber1;
			double camberHyunjang = pGir->GetCamberByStation(sta, 8) + camber0 + camber1;

			double camber;
			switch (nRow)
			{
			case 0 :	camber = sta;	break;
			case 1 :	camber = pGir->GetLine()->GetElevationPlanEl(sta);	break;
			case 2 :	camber = pGir->GetPyungubaeCenLine(sta, 0) * 100;	break;
			case 3 :	camber = pDB->m_dThickSlabBase + pDB->m_dThickPave + pGir->GetHeightFlangeUpperByTaper(sta);
			case 5 :	camber = camber2 + camber3 + camber4;	break;
			case 6 :	camber = pGir->GetElevationFlange(sta);	break;
			default :	camber = 0; break;
			}
			switch(nRow)
			{
			case 0:	str.Format("STA. %.3f", camber); break;
			case 5: str.Format("%.1f=%.1f+%.1f+%.1f",camber,camber2,camber3,camber4);	break;
			case 10:
				if(nIdx == 0)
					str = "START";
				else if(nIdx == nCount-1)
					str = "END";
				else
				{
					pBxCro = pGir->GetBxByCrossNo((long)camber-2);
					if(pBxCro)
						str = pDB->GetMarkSection(SECTION_CR, pBxCro);
					else
						str = _T("");
				}
				break;
			default:	str.Format("%.1f", camber);	break;
			}

			if(nIdx == 1 || nIdx == nCount-1)
				Dom.TextOut(OrgData+SumData + Dom.GetTextHeight()*1.2, BaseRow.y + HeaderH/2 - TextGap, str);
			else
				Dom.TextOut(OrgData+SumData - Dom.GetTextHeight() / 4, BaseRow.y + HeaderH/2 - TextGap, str);
		}
		if (nRow == 10) //마지막
		{
			pStd->m_pOptStd->SetEnvLType(&Dom, LT_SOLID);
			Dom.MoveTo(BaseRow.x, BaseRow.y);
			Dom.LineTo(BaseRow.x + HeaderW + CamberW, BaseRow.y);
			Dom.TextOut(BaseRow.x+Dom.Always(20), BaseRow.y+Dom.Always(2), "BASE LINE");
			Dom.SetTextAngle(90);
		}
	}

	// 상단치수와 테이블사이 절점표시.
	SumData = 0;
	Dom.SetTextAngle(0);
	Dom.SetTextAlignHorz(TA_CENTER);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = pGir->GetBxByStationDir(m_SttSta, 1, BX_CROSSBEAM_VBRACING);
	Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, NULL, pBx);
	long nDiap = Finder.GetCountBx();
	for (long nIdx = 0; nIdx < nCount; nIdx++)
	{
		PreData = data.x;
		data = staArr.GetAt(nIdx);
		if(nIdx == 0)
		{
			OrgData = data.x;
		}
		if(nIdx != 0)	//한칸 진행된 경우...상단의 치수와 관련해서
			SumData += (data.x - PreData);
		

		pBx = pGir->GetBxByStationDir(data.x, 0, BX_CROSSBEAM_VBRACING | BX_SPLICE | BX_STT_GIRDER | BX_END_GIRDER);
		if (pBx->IsState(BX_CROSSBEAM_VBRACING))
		{
			CSymbolDom	SDom(pDomP, pStd->m_pOptStd);
			CDPoint CP  = CDPoint(OrgData+SumData, Origin.y + Scale * 4);
			CP.y += Scale*5;
			pStd->m_pOptStd->SetEnvType(&SDom,HCAD_DIML);
			SDom.SetScaleDim(Scale);
			str.Format("%d", nDiap++);
			SDom.SymbolTextCircle(CP, str);
			SDom.MoveTo(OrgData+SumData, Origin.y + Scale * 2);
			SDom.LineTo(OrgData+SumData, BaseRow.y);
			Dom << SDom;
		}
		else
		{
			Dom.MoveTo(OrgData+SumData, Origin.y + Scale * 2);
			Dom.LineTo(OrgData+SumData, BaseRow.y);
		}
	}

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	// 지점 심볼 그리기
	SumData = 0;
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		PreData = data.x;
		data = staArr.GetAt(nIdx);
		if(nIdx == 0)
		{
			OrgData = data.x;
		}
		if(nIdx!=0)
			SumData += (data.x - PreData);
		if(IsSpot(data))
			Dom.Triangle(OrgData+SumData, BaseRow.y, 8);
	}

	// 현장 이음 치수 그리기
	SumData = 0;
	CDimDomyunEx DimDom(pDomP);
	pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_DIML);
	DimDom.SetDirection("BOTTOM");
	long DimDan = 2;
	staArr.RemoveAll();
	GetCamberStation(staArr, 2, m_nG);
	nCount = staArr.GetSize();
	data = staArr.GetAt(0);
	DimDom.DimBasePoint(data.x, BaseRow.y, DimDan);
	DimDom.DimMoveTo(data.x, BaseRow.y, 0, FALSE, "FIELD SPLICE", DimDom.Always(50));
	OrgData = data.x;
	CPlateBxFinder FinderSp(pGir);
	pBx = FinderSp.GetBxFirst(BX_STT_GIRDER | BX_SPLICE | BX_END_GIRDER, 
							pGir->GetBxByStation(m_SttSta), pGir->GetBxByStation(m_EndSta));
//	CPlateBasicIndex *pBxStt = pGir->GetBxByStation(m_SttSta);
	double dPresta = pGir->GetBxByStation(m_SttSta)->GetStation(), dSpSta = 0;
	if(pBx->GetSplice()){
		dSpSta = pBx->GetStation();
		SumData += (dSpSta - dPresta);
		str.Format("%.1f", dSpSta - dPresta);
		pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_DIML);
		DimDom.DimLineTo((dSpSta - dPresta),BaseRow.y, str);
		pOpt->SetEnvType(&DimDom,HCAD_SYMB); // MARK LAYER
		DimDom.TextCircle(OrgData+SumData,BaseRow.y-DimDom.Always(30),pDB->GetMarkSection(SECTION_SP,pBx),2);
		dPresta = dSpSta;
	}
	while(pBx=FinderSp.GetBxNext())
	{
		dSpSta = pBx->GetStation();
		SumData += (dSpSta - dPresta);
		str.Format("%.1f", dSpSta - dPresta);
		pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_DIML);
		DimDom.DimLineTo((dSpSta - dPresta),BaseRow.y, str);
		pOpt->SetEnvType(&DimDom,HCAD_SYMB); // MARK LAYER
		DimDom.TextCircle(OrgData+SumData,BaseRow.y-DimDom.Always(30),pStd->m_pDB->GetMarkSection(SECTION_SP,pBx),2);
		dPresta = dSpSta;
	}
	pBx = pGir->GetBxByStation(m_EndSta);
	dPresta = dSpSta;
	dSpSta = pBx->GetStation();
	if(!pBx->GetSplice())
	{
		str.Format("%.1f", dSpSta - dPresta);
		if(dSpSta - dPresta==0)
			str="";
		pStd->m_pOptStd->SetEnvType(&DimDom, HCAD_DIML);
		DimDom.DimLineTo((dSpSta - dPresta),BaseRow.y, str);
	}

	//거더치수
	pOpt->SetEnvType(&DimDom,HCAD_DIML); // MARK LAYER
	data = staArr.GetAt(0);
	DimDom.DimBasePoint(data.x, BaseRow.y, DimDan+3);
	DimDom.DimMoveTo(data.x, BaseRow.y);
	pOpt->SetEnvType(&DimDom,HCAD_SYMB);
	if(FinderSp.GetBxFirst(BX_STT_GIRDER) == pGir->GetBxByStation(m_SttSta))
		DimDom.TextCircle(data.x, BaseRow.y-DimDom.Always(60),"START",2);

	double presta = data.x;
	data = staArr.GetAt(staArr.GetUpperBound());
	str.Format("%.1f", data.x - presta);
	if(FinderSp.GetBxFirst(BX_END_GIRDER) == pGir->GetBxByStation(m_EndSta))
		DimDom.TextCircle(OrgData+(data.x-OrgData), BaseRow.y-DimDom.Always(60), " END ",2);
	pOpt->SetEnvType(&DimDom,HCAD_DIML);
	DimDom.DimLineTo((data.x - presta),BaseRow.y,str);
	DimDom.Move(CDPoint(-m_SttSta, 0) + Base);
	Dom.Move(CDPoint(-m_SttSta, 0) + Base);

	*pDomP << DimDom;
	*pDomP << Dom;
	*/
}

//-------------------- 테이블 제목 세팅 ------------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::SetDMTableTitleArr(CStringArray &arrTitle)
{
	arrTitle.Add("STATION");
	arrTitle.Add("계획고");
	arrTitle.Add("편경사");
	arrTitle.Add("CON\'C");
	arrTitle.Add("");
	arrTitle.Add("CAMBER\n=(자중+합성전+합성후)");
	arrTitle.Add("GIRDER EL");
	arrTitle.Add("");
}

//------------------------ 범 례(대명) -------------------------------
//--------------------------------------------------------------------
void CAPlateOutCamber::AddBumlaeMark(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd* pOpt = pStd->m_pOptStd;

	CString sBurf;	//TEMP CSTRING
	CSymbolDom Dom(pDomP, pOpt);

	//제목쓰기..."범례"
	pOpt->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	sBurf = "";

	CString strSmallTitle = "범 례";
	Dom.TextOut(Dom.Always(35), Dom.Always(5), strSmallTitle);
	//////////////////////////////////////////////////////////////////////////////////////////
	//pStd->GetShopOutDC()->DrawTitleOnTable(&Dom,Dom.Always(35),Dom.Always(-5),sBurf,TRUE,0);

	//WRITE CONTENTS
	pOpt->SetEnvType(&Dom,HCAD_NOTE);
	Dom.SetTextAlignHorz(TA_LEFT);
	long nContents = -12; long nBaesu = -10;
	sBurf =	": 계 획 고";
	Dom.TextOut(Dom.Always(40),Dom.Always(nContents), sBurf);
	sBurf = ": ";
	Dom.TextOut(Dom.Always(40),Dom.Always(nContents+nBaesu*1), sBurf);
	double Len = Dom.GetTextWidth("1")*4 + Dom.GetTextWidth(sBurf);
	sBurf = "+ 편경사 - CON'C";
	Dom.TextOut(Dom.Always(40)+Len,Dom.Always(nContents+nBaesu*1), sBurf);

	sBurf = ": GIRDER EL(";
	Dom.TextOut(Dom.Always(40),Dom.Always(nContents+nBaesu*2), sBurf);
	Len = Dom.GetTextWidth("2")*4 + Dom.GetTextWidth(sBurf);
	sBurf = "+ CMABER)";
	Dom.TextOut(Dom.Always(40)+Len,Dom.Always(nContents+nBaesu*2), sBurf);


	sBurf = ": 지점간의 기준선";
	Dom.TextOut(Dom.Always(40),Dom.Always(nContents+nBaesu*3), sBurf);
	sBurf = ": 솟 음 값";
	Dom.TextOut(Dom.Always(40),Dom.Always(nContents+nBaesu*4), sBurf);
	sBurf = "- 공장조립시 솟음 : 지점간을 기준으로 계산했을때의 솟음값";
	Dom.TextOut(Dom.Always(10),Dom.Always(nContents+nBaesu*5), sBurf);
	sBurf = "- 현장거치시 솟음 : 지점상에 강재 조립 설치를 완료한 상태";
	Dom.TextOut(Dom.Always(10),Dom.Always(nContents+nBaesu*6), sBurf);
	sBurf = "     (공장조립시 솟음 - 자중값)";
	Dom.TextOut(Dom.Always(40),Dom.Always(nContents+nBaesu*7), sBurf);
	
	CDPoint xy = Base, sy(0,0),rAng(0,0);	//선그리는 기준, 심볼그리는 기준
	//구분선 길이
	double dGubunLength = 150;
	//선길이, 선간격, 선시작점부터 마크까지 이격 거리
	double dLineLength = 20, dLineDistance = 10, dMarkDistance = 5;	
	
	//범례 위쪽 구분선그리기
//	xy.x -= Dom.Always(120);	xy.y -= Dom.Always(451);
	Dom.MoveTo(xy);		Dom.LineTo(xy.x+Dom.Always(dGubunLength), xy.y);
	//1.계획고
	pOpt->SetEnvLType(&Dom, LT_DASHDOT_LINE);
	xy.x += Dom.Always(15);		xy.y -= Dom.Always(dLineDistance);
	Dom.MoveTo(xy);		Dom.LineTo(xy.x+Dom.Always(dLineLength), xy.y);
	pOpt->SetEnvLType(&Dom, LT_SOLID);
	sy = xy;	sy.x -= Dom.Always(dMarkDistance);
	Dom.DrawSymbolDiamondMark(sy, rAng, "1");
	//2.편경사
	pOpt->SetEnvLType(&Dom, LT_DASH_LINE);
	xy.y -= Dom.Always(dLineDistance);
	Dom.MoveTo(xy);		Dom.LineTo(xy.x+Dom.Always(dLineLength), xy.y);
	pOpt->SetEnvLType(&Dom, LT_SOLID);
	sy.y = xy.y;
	Dom.DrawSymbolDiamondMark(sy, rAng, "2");
	sy.x += Dom.Always(37);
	Dom.DrawSymbolDiamondMark(sy, rAng, "1");
	//3.Girder El
	pOpt->SetEnvLType(&Dom, LT_SOLID);
	xy.y -= Dom.Always(dLineDistance);
	Dom.MoveTo(xy);		Dom.LineTo(xy.x+Dom.Always(dLineLength), xy.y);
	sy = xy;	sy.x -= Dom.Always(dMarkDistance);
	Dom.DrawSymbolDiamondMark(sy, rAng, "3");
	sy.x += Dom.Always(57);
	Dom.DrawSymbolDiamondMark(sy, rAng, "2");
	//4.지점간의 기준선
	xy.y -= Dom.Always(dLineDistance);
	Dom.MoveTo(xy);		Dom.LineTo(xy.x+Dom.Always(dLineLength), xy.y);
	sy = xy;	sy.x -= Dom.Always(dMarkDistance);
	Dom.DrawSymbolDiamondMark(sy, rAng, "4");
	//5.솟음값
	xy.y -= Dom.Always(dLineDistance);
	Dom.MoveTo(xy);		Dom.LineTo(xy.x+Dom.Always(dLineLength), xy.y);
	sy.y = xy.y;
	Dom.DrawSymbolDiamondMark(sy, rAng, "5");
	//범례 아래쪽 구분선그리기
	xy.x -= Dom.Always(15);	xy.y -= Dom.Always(38);
	Dom.MoveTo(xy);		Dom.LineTo(xy.x+Dom.Always(dGubunLength), xy.y);
	
	*pDomP << Dom;
}

void CAPlateOutCamber::DimSlabThick(CDomyun *pDomP,CPlateBasicIndex *pBx)
{
	if(pBx==NULL)	return;
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	 *pGir = pDB->GetGirder(0);//0번거더

	CDomyun* pDom = new CDomyun(pDomP);

	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir);;

	// 치수선
	long DimDan = 0;
	double x,y;
	double LEle,REle,LDis,RDis;
	double Thick = pDB->m_dThickSlabBase + pDB->m_dThickPave;
 
	for(long nG=0; nG<pStd->m_pDB->GetGirdersu(); nG++) {
		pGir   = pDB->GetGirder(nG);
		pBxSec = pBx->GetBxMatchByCrossBeam(pGir);
		LDis   = pGir->m_dGirderCenter + pDB->m_dHunchDisForBaseThick;
		RDis   = pGir->m_dGirderCenter + pDB->m_dHunchDisForBaseThick;
		LEle   = pDB->GetHeightSlabFromRoadCenterDom(pBxSec, LDis);
		REle   = pDB->GetHeightSlabFromRoadCenterDom(pBxSec, RDis);

		x = (LEle<REle) ? LDis : RDis;
		y = min(LEle,REle) - Thick;
		////
		pDom->SetDimExo(0);
		pDom->SetStartPointExtend(TRUE);
		pDom->SetDirection("LEFT");
		pDom->SetDimObq(45);
		pDom->DimMoveTo(x,y,DimDan);
		pDom->DimLineTo(Thick,DimDan,COMMA(Thick));
 		////
		pDom->SetDirection("RIGHT");
		pDom->SetDimObq(0);
		pDom->DimMoveTo(x,y,DimDan);
		pDom->DimLineTo(fabs(y),DimDan,COMMA(fabs(y)));
	}
 
	*pDomP << *pDom;
	delete pDom;
}


void CAPlateOutCamber::DimGirderDist(CDomyun *pDomP,CPlateBasicIndex *pBx)
{
	if(pBx==NULL)	return;
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	 *pGir = pDB->GetGirder(0);//0번거더

	CDimDomyunEx* pDom = new CDimDomyunEx(pDomP);

	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir);;

	// 치수선
	double x,y;
	double LEle,REle,LDis,RDis;
	double Thick = pDB->m_dThickSlabBase + pDB->m_dThickPave;
 
	for(long nG=0; nG<pDB->GetGirdersu(); nG++) {
		pGir   = pDB->GetGirder(nG);
		pBxSec = pBx->GetBxMatchByCrossBeam(pGir);
		LDis   = pGir->m_dGirderCenter -  pDB->m_dHunchDisForBaseThick;
		RDis   = pGir->m_dGirderCenter +  pDB->m_dHunchDisForBaseThick;
		LEle   = pDB->GetHeightSlabFromRoadCenterDom(pBxSec, LDis);
		REle   = pDB->GetHeightSlabFromRoadCenterDom(pBxSec, RDis);

		x = (LEle<REle) ? LDis : RDis;
		y = min(LEle,REle) - Thick;
		////
		pDom->DimMoveTo(0,0,nG+1);
		pDom->DimLineTo(x,y,COMMA(fabs(x)));
	}
 
	*pDomP << *pDom;
	delete pDom;
}

void CAPlateOutCamber::DimTextEtc(CDomyun *pDomP,CPlateBasicIndex *pBx)
{
	if(pBx==NULL)	return;
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	 *pGir = pDB->GetGirder(0);//0번거더

	CDimDomyunEx* pDom = new CDimDomyunEx(pDomP);

	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir);;

	// T.O.W
	double x = pGir->m_dGirderCenter;
	double y = pGir->GetElevationDom(pBxSec);

	double Len = pDom->GetTextWidth("T.O.W");
	pDom->LineTo(x,y, x-Len*1.5,y);
	pDom->TextOut(x-Len,y,"T.O.W");
 
	*pDomP << *pDom;
	delete pDom;
}


//---------------------- 도각 제목 얻기 ------------------------------
//--------------------------------------------------------------------
CString CAPlateOutCamber::GetPageTitle(long nPage, BOOL bShort)
{	
	CString szTitle;

	szTitle.Format("솟 음 도 (%d/%d)",nPage - m_nSttPage,GetPageSu());
	return szTitle;
}

//-------------------- 전체 도각 스케일 얻기 -------------------------
//--------------------------------------------------------------------
double CAPlateOutCamber::GetScale()
{
	return ((CAPlateDrawStd*)m_pStd)->m_pDomOptionStd->m_dCamber_HScale;
}

//-------------------- 지점인지 아닌지 판판 --------------------------
//--------------------------------------------------------------------
BOOL CAPlateOutCamber::IsSpot(CVector vSta)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*		pGir = pDB->GetGirder(m_nG);

	CPlateBasicIndex *pBx = pGir->GetBxByStationDir(vSta.x, 0, BX_SPLICE | BX_CROSSBEAM_VBRACING | BX_STT_GIRDER | BX_END_GIRDER);
	if (pBx->IsState(BX_CROSSBEAM_VBRACING) && pBx->IsJijum())
		return TRUE;
	else
		return FALSE;
}
