// APlateOutTUGirder.cpp: implementation of the CAPlateOutTUGirder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
#include "../PlateGangje\Gangje.h"
#include "../APlateDBDraw/DimDomyunEx.h"

#define PLAN			10
#define JONG			20
#define STEEL_TABLE		30
#define DETAIL_TITLE	40
#define DETAIL_A		50
#define DETAIL_B		60
#define DETAIL_STEEL	100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutTUGirder::CAPlateOutTUGirder()
{

}

CAPlateOutTUGirder::~CAPlateOutTUGirder()
{

}

void CAPlateOutTUGirder::DrawOnePage()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB		= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove	    = pStd->GetMovePoint(pStd->m_nCurPage);	
	CARoadOptionStd *pOptStd	= pStd->GetDataManage()->GetOptionStd();	

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	
	long	nPage		= pStd->m_nCurPage;
	long	nPrePage	= m_nSttPage;
	long	nSubPage	= nPage - nPrePage;

	double	dScale		= GetScale();
	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);
	CDomyun Dom(pDom);
	
	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
//	long nSttPage = pDomOpt->m_nDomPageArray.GetAt(PAGE_TUGIRDER) == TRUE ? 0 : pDomOpt->m_nDomPageArray.GetAt(PAGE_TUGIRDER) + nSubPage - 1;	
	pStd->GetDogak()->Add(&Dom,nPage,dScale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pDB->m_sNameBridge, GetTitle(), _T(""),FALSE);
	pStd->AddMaterial(&Dom,dScale);
	pStd->AddJubujebubuje(&Dom,dScale);
	Dom.SetCalcExtRect();
	CDRect rectDgk = Dom.GetExtRect();
	*pDom << Dom;

	CDRect rectDom;
	double dMoveX = 0;
	double dMoveY = 0;
	if(nSubPage == pDB->GetQtyGirder()+1)
	{
		// 시공순서도
		InsertDomyun(&Dom, nSubPage);
		*pDom << Dom;
	}
	if(nSubPage < pDB->GetQtyGirder()+1)
	{
		if(pStd->m_pDataManage->GetDeckDataTU()->m_arrRebarJewonTU.GetSize() < 1) return;
		// 평면도 ////////////////////////////////////////////////////////////////////////
		if(pDomOpt->m_bTU_Plan)
		{
			pPageMng->SetProgressData("평면도 출력중...", 20);
			AddPlan(&Dom, nSubPage);
			Dom.SetScaleDim(pDom->GetScaleDim());

			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();			

			dMoveX = (rectDgk.Width()-rectDom.Width())*0.5 + rectDom.Width()/2;			
			Dom.Move(dMoveX, rectDgk.Height()-Dom.Always(100));
			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();
			dMoveY = rectDom.top;
			// 도면 기준점 영역이동
			Dom.Move(pMove->GetPoint(PLAN));	
				
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), PLAN, "평면도");
			*pDom << Dom;

			DrawStd.AddTitleOnDom(&Dom, GetScale(), rectDom, "평면도", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(PLAN+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(), PLAN+1, "평면도 제목");

			*pDom << Dom;
		}
		
		// 종단면도 //////////////////////////////////////////////////////////////////////
		if(pDomOpt->m_bTU_Jong)
		{
			pPageMng->SetProgressData("종단면도 출력중...", 40);
			AddJong(&Dom, nSubPage);
			Dom.SetScaleDim(pDom->GetScaleDim());

			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();

			Dom.Move(0, -rectDom.top);
			Dom.Move(dMoveX, dMoveY-rectDom.Height()-Dom.Always(30));
			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();

			// 도면 기준점 영역이동
			Dom.Move(pMove->GetPoint(JONG));	
			Dom.SetCalcExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), JONG, "종단면도");
			*pDom << Dom;

			DrawStd.AddTitleOnDom(&Dom, GetScale(), rectDom, "종단면도", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(JONG+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(), JONG+1, "종단면도 제목");

			*pDom << Dom;
		}
		
		// 단면 상세도 ////////////////////////////////////////////////////////////////////////		
		if(pDomOpt->m_bTU_Detail)
		{
			pPageMng->SetProgressData("단면 상세도 A-A 출력중...", 50);
			// 단면 A-A ///////////////////////////////////////////////////////////////////////
			AddDetail(&Dom, nSubPage, TRUE);
			Dom.SetScaleDim(pDom->GetScaleDim());

			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();

			Dom.Move(Dom.Always(100)-rectDom.Width()/2, Dom.Always(100));
			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();
			// 도면 기준점 영역이동
			Dom.Move(pMove->GetPoint(DETAIL_A));	
			Dom.SetCalcExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), DETAIL_A, "A - A");
			*pDom << Dom;

			DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dTUScale_Detail, rectDom, "A - A", TRUE, FALSE, 0);
			Dom.Move(pMove->GetPoint(DETAIL_A+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(), DETAIL_A+1, "A - A 제목");

			*pDom << Dom;

			// 단면 B-B ///////////////////////////////////////////////////////////////////////
			pPageMng->SetProgressData("단면 상세도 B-B 출력중...", 60);
			AddDetail(&Dom, nSubPage, FALSE);
			Dom.SetScaleDim(pDom->GetScaleDim());
			
			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();

			Dom.Move(Dom.Always(120)+rectDom.Width()/2, Dom.Always(100));
			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();
			// 도면 기준점 영역이동
			Dom.Move(pMove->GetPoint(DETAIL_B));	
			Dom.SetCalcExtRect();			
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), DETAIL_B, "B - B");
			*pDom << Dom;

			DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dTUScale_Detail, rectDom, "B - B", TRUE, FALSE, 0);
			Dom.Move(pMove->GetPoint(DETAIL_B+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(), DETAIL_B+1, "B - B 제목");

			*pDom << Dom;
			// 단면 제목 //////////////////////////////////////////////////////////////////////
			rectDom.bottom = rectDom.bottom;
			rectDom.left -= (rectDom.Width() + Dom.Always(10));
			DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dTUScale_Detail, rectDom, "단면 상세도", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(DETAIL_TITLE+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(), DETAIL_TITLE+1, "단면 상세도 제목");

			*pDom << Dom;
		}

		// 철근 상세도 ////////////////////////////////////////////////////////////////////
		if(pDomOpt->m_bTU_DetailSteel)
		{
			pPageMng->SetProgressData("철근 상세도 출력중...", 70);
			AddDetailSteel(&Dom, nSubPage);		
			*pDom << Dom;
		}				
		
		// 철근 재료표 ////////////////////////////////////////////////////////////////////
		if(pDomOpt->m_bTU_TableSteel)
		{
			pPageMng->SetProgressData("철근 재료표 출력중...", 90);
			AddTableSteel(&Dom, nSubPage);
			Dom.SetScaleDim(pDom->GetScaleDim());

			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();

			Dom.Move(rectDgk.Width()-Dom.Always(30), Dom.Always(125));
			Dom.SetCalcExtRect();
			rectDom = Dom.GetExtRect();
			// 도면 기준점 영역이동
			Dom.Move(pMove->GetPoint(STEEL_TABLE));	
			Dom.SetCalcExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), STEEL_TABLE, "철근 재료표");
			*pDom << Dom;

			DrawStd.AddTitleOnDom(&Dom, 0, rectDom, "철근 재료표", TRUE, FALSE, 0);
			Dom.Move(pMove->GetPoint(STEEL_TABLE+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(), STEEL_TABLE+1, "철근 재료표 제목");

			*pDom << Dom;
		}		
	}
	
	pPageMng->SetProgressData("출력완료",100);
}

void CAPlateOutTUGirder::InsertDomyun(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	= pStd->GetDataManage()->GetBridge();

	CString csSection       = "Afx Directory";
	CString csWorkItem		= "Fixed Directory";
	CString szPath = AfxGetApp()->GetProfileString(csSection, csWorkItem);

	CString szName = _T("");
	if(nSubPage == pDB->GetQtyGirder()+1)
		szName.Format("%s\\BlockDomyun\\TU_Sequence.dxf",szPath);

	CDomyun Dom(pDom);
	
	if(szName == _T(""))	return;

	Dom.DxfIn(szName);

	if(nSubPage == pDB->GetQtyGirder()+1)
	{
		Dom.RedrawByScale(3);
		Dom.Move(Dom.Always(-10), Dom.Always(-20));
	}
	
	*pDom << Dom;
}

void CAPlateOutTUGirder::AddPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pDomOpt		= pStd->GetDomOptionStd();
	CDataManage		*pDataMng		= pStd->GetDataManage();
	CPlateBridgeApp	*pDB			= pDataMng->GetBridge();
	CPlateGirderApp	*pGir			= pDB->GetGirder(nSubPage-1);
	
	CAPlateDrawPyung	DBPlan(pDataMng);
	CDrawDBBaseRebarTU	DBRebarTU(pDataMng);

	CDomyun Dom(pDom);
	CDomyun DomTemp(pDom);
	Dom.SetScaleDim(GetScale());
	DomTemp.SetScaleDim(GetScale());

	__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag);

	long nG = nSubPage-1;
	CDRect rectDom, rectTrim;
	
	DBRebarTU.DrawMainRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawSabogangRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawBeryukRebarAtPlan(&Dom, nG);
	DBRebarTU.DrawHunchRebarTUAtPlan(&Dom, nG);

	if(pDomOpt->m_nTU_Trim == 0)
	{
		DBRebarTU.DrawTrimLine(&Dom, nG, 1);

		pStd->m_pOptStd->SetEnvType(&DomTemp,HCAD_DIML);
		
		DBRebarTU.DrawMainRebarTUAtPlan(&DomTemp, nG);
		DBRebarTU.DrawSabogangRebarTUAtPlan(&DomTemp, nG);
		DBRebarTU.DrawHunchRebarTUAtPlan(&Dom, nG);
		DBRebarTU.DrawBeryukRebarAtPlan(&DomTemp, nG);
		DBRebarTU.DrawTrimLine(&DomTemp, nG, -1);
		DBRebarTU.DrawTrimLine(&DomTemp, nG, 0);

		Dom << DomTemp;		
	}

	while(Finder.GetBxFixNext())
	{
		DBPlan.DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0, 0);	
		pBx = Finder.GetBxNext();
	}

	Dom.SetCalcExtRect();
	rectDom = Dom.GetExtRect();
	DBRebarTU.DrawMark(&Dom, nG, DRAW_PLAN);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);

	DBRebarTU.DimMainRebarTUAtPlan(&Dom, nG, FALSE);
	DBRebarTU.DimSabogangRebarTUAtPlan(&Dom, nG);

	if(CAPlateDrawPyung::IsInputDomRotate())
	{
		CDPoint xyStt = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB));
		CDPoint xyEnd = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB));
		CDPoint vDir  = (xyEnd-xyStt).Unit();

		vDir.MirrorVert();
		Dom.Rotate((xyStt+xyEnd)/2,vDir);
	}
	
//	Dom.RedrawByScale(GetScale()/pDomOpt->m_dTUScale_Plan);	
	Dom.SetCalcExtRect();
	rectDom = Dom.GetExtRect();
	Dom.Move(-(rectDom.right-rectDom.Width()/2), -rectDom.bottom);

	*pDom << Dom;
}

void CAPlateOutTUGirder::AddJong(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pDomOpt		= pStd->GetDomOptionStd();
	CDataManage		*pDataMng		= pStd->GetDataManage();
	CARoadOptionStd *pOptStd		= pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB			= pDataMng->GetBridge();
	CPlateGirderApp	*pGir			= pDB->GetGirder(nSubPage-1);	
	
	CDrawDBBaseRebarTU	DBRebarTU(pDataMng);
	CAPlateDrawJong		DrawJong(pDataMng);

	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxFirst(BX_END_GIRDER);
	
	CDomyun Dom(pDom);
	Dom.SetScaleDim(GetScale());
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	long nG = nSubPage-1;
	BOOL	bEleLevelCur	= pGir->GetEleLevel(); 

	DBRebarTU.DrawBincConcRebarAtJong(&Dom, nG, !pDomOpt->m_bTU_Detail);
	DBRebarTU.DimBincConcRebarAtJong(&Dom, nG, FALSE);
	
	if(pDomOpt->m_nTU_Trim == 0)
		DBRebarTU.DrawTrimLineJong(&Dom, nG);
	
	pGir->SetEleLevel(TRUE);		// 배근도에서는 Level 상태로 그림
	DrawJong.DrawGirderLineBx(&Dom, pBxStt, pBxEnd);
	DrawJong.DrawInOutGateBx(&Dom, pBxStt, pBxEnd);
	DrawJong.DrawJiJumVStiffBx(&Dom, pBxStt, pBxEnd);

	pGir->SetEleLevel(bEleLevelCur);

	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();	

//	Dom.RedrawByScale(GetScale()/pDomOpt->m_dTUScale_Jong);	
	Dom.SetCalcExtRect();
	rectDom = Dom.GetExtRect();

	Dom.Move(-(rectDom.right-rectDom.Width()/2), -Dom.GetExtRect().bottom);
	
	*pDom << Dom;
}

void CAPlateOutTUGirder::AddDetail(CDomyun *pDom, long nSubPage, BOOL bAnchor)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pDomOpt		= pStd->GetDomOptionStd();
	CDataManage		*pDataMng		= pStd->GetDataManage();
	CARoadOptionStd *pOptStd		= pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB			= pDataMng->GetBridge();
	CPlateGirderApp	*pGir			= pDB->GetGirder(nSubPage-1);
	CPlateBasicIndex*pBx			= pGir->GetBxOnJijum(0);
	
	CDrawDBBaseRebarTU	DBRebarTU(pDataMng);

	CDomyun Dom(pDom);
	Dom.SetScaleDim(pDomOpt->m_dTUScale_Detail);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	long nG = nSubPage-1;

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
	double dHeight		= pGir->GetHeightGirderByJijum(0)/2;
	double dUWidth		= pGir->GetWidthJewon(G_F_U, pBx);
	double dLWidth		= pGir->GetWidthJewon(G_F_L, pBx);

	Dom.Rectangle(dGirderCenter-dUWidth/2, dFlangH, dGirderCenter+dUWidth/2, dFlangH-dFlangeTUp);
	Dom.Rectangle(dGirderCenter-dWebT/2,   dFlangH-dFlangeTUp, dGirderCenter + dWebT/2, dFlangH-dFlangeTUp-dHeight);
	Dom.Rectangle(dGirderCenter-dLWidth/2, dFlangH-dFlangeTUp-dHeight, dGirderCenter+dLWidth/2, dFlangH-dFlangeTUp-dFlangeTDn-dHeight);

	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dSlabLowH+dBindS1);
	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter-dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter-dBindW/2, dFlangH-dBindS2,	dGirderCenter-dWebT/2,  dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dFlangH-dBindS2,	dGirderCenter+dWebT/2,  dFlangH-dBindS2);

	// Rebar
	pOptStd->SetEnvType(&Dom,HCAD_BARC);

	DBRebarTU.DrawBincConcRebarAtCross(&Dom, pBx, bAnchor);
	DBRebarTU.DimBincConcRebarAtCross(&Dom, nG, FALSE, bAnchor);

	Dom.RedrawByScale(GetScale()/pDomOpt->m_dTUScale_Detail);

	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	Dom.Move(-rectDom.left, -rectDom.top);

	*pDom << Dom;
}

void CAPlateOutTUGirder::AddDetailSteel(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove			= pStd->GetMovePoint(pStd->m_nCurPage);
	CDataManage		*pDataMng		= pStd->GetDataManage();
	COptionStd      *pDomOpt		= pStd->GetDomOptionStd();
	CARoadOptionStd *pOptStd		= pDataMng->GetOptionStd();
	CADeckData		*pDeckData		= pDataMng->GetDeckData();
	CADeckDataTU	*pDeckDataTU	= pDataMng->GetDeckDataTU();
	CRebar			*pRB			= NULL;

	CDomyun Dom(pDom);
	CConcSymbol symDB(pOptStd);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	
	long	nG		= nSubPage-1;
	long	nCount	= 0;
	double	dMoveX	= 0, dMoveY = 0;
	double	dBaseX	= pDom->Always(450);
	double	dBaseY	= pDom->Always(215);
	double	dJewonY	= 0;

	CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar = new CSafetyTypedPtrArray<CObArray, CRebar*>;
	CString sText;
	long	n	=0;
	for(n=0; n<pDeckDataTU->m_arrRebarJewonTU.GetSize(); n++)
	{
		pRB = pDeckDataTU->m_arrRebarJewonTU.GetAt(n);
		long nFind = pRB->m_sDescription.Find("_G");
		sText = pRB->m_sDescription.Mid(nFind+2, 2);
		nFind = (long)atof(sText);
		if(nFind-1 == nG)
		{
			CRebar *pRBNew = new CRebar;
			*pRBNew = *pRB;
			pArrRebar->Add(pRBNew);
		}
	}
	pDeckData->ShellSortRebarList(pArrRebar, &CRebar::CompareForTable);

	// 동일타입끼리 정렬함
	long nSize= pArrRebar->GetSize();
	long nIdx = 1;
	for(n=0; n<nSize-2; n++)
	{
		CRebar *pRBCur	= pArrRebar->GetAt(n);
		
		for(long k=n+1; k<nSize; k++)
		{
			CRebar *pRBNext = pArrRebar->GetAt(k);

			if(pRBCur->m_nTypeRebar == pRBNext->m_nTypeRebar)
			{
				CRebar *pRBNew = new CRebar;
				*pRBNew = *pRBNext;
				delete pRBNext;
				pArrRebar->RemoveAt(k);
				pArrRebar->InsertAt(n+nIdx, pRBNew);
				nIdx++;
				continue;
			}
		}
		nIdx = 1;
	}

	CDRect rectDom;
	double dMaxH = 0;
	for(n=0; n <nSize; n++)
	{
		pRB = pArrRebar->GetAt(n);

		if(n==0 || pRB->m_nTypeRebar != pArrRebar->GetAt(n-1)->m_nTypeRebar)
		{
			symDB.RebarDetailBasic(&Dom, pRB->m_nTypeRebar, CDPoint(0,dJewonY), pRB->m_nJoint>0);
			dJewonY -= Dom.Always(20);
		}

		symDB.DrawJewonBar(&Dom, pRB, CDPoint(-Dom.Always(symDB.m_pStd->m_DimSet.m_dRebarLineWidth)/2,dJewonY));
		
		Dom.SetCalcExtRect();
		rectDom = Dom.GetExtRect();

		dJewonY -= Dom.Always(10);

		if(n!=nSize-1 && pRB->m_nTypeRebar == pArrRebar->GetAt(n+1)->m_nTypeRebar) continue;
		if(nCount%2 != 1)	dMaxH = rectDom.Height();
		else				dMaxH = max(dMaxH, rectDom.Height());
		
		Dom.Move(-rectDom.right+rectDom.Width()/2 , -rectDom.bottom);

		dMoveX = (rectDom.Width()+Dom.Always(10)) * (nCount%2);		
		Dom.Move(dMoveX + dBaseX, dMoveY + dBaseY);
		dMoveY -= nCount%2 != 1 ? 0 : (dMaxH+Dom.Always(10));

		Dom.Move(pMove->GetPoint(DETAIL_STEEL+nCount*10));		

		Dom.SetCalcExtRect();
		rectDom = Dom.GetExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), DETAIL_STEEL+nCount*10, "철근 상세도" + COMMA(nCount+1));

		nCount++;
		dJewonY = -Dom.Always(20);
		
		*pDom << Dom;
	}

	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	Dom.LineTo(dBaseX, dBaseY, dBaseX+rectDom.Width()+Dom.Always(10), dBaseY);
	Dom.SetCalcExtRect();
	rectDom = Dom.GetExtRect();
	*pDom << Dom;

	DrawStd.AddTitleOnDom(&Dom, 0, rectDom, "철근 상세도", TRUE, FALSE, 0);
	Dom.Move(pMove->GetPoint(DETAIL_STEEL+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(), DETAIL_STEEL+1, "철근 상세도 제목");
	*pDom << Dom;

	// 그리는동안 생성한 RebarData 삭제
	for(n=0; n<pArrRebar->GetSize(); n++)
	{
		pRB = pArrRebar->GetAt(n);
		delete pRB;
	}
}

void CAPlateOutTUGirder::AddTableSteel(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	CDataManage		*pDataMng		= pStd->GetDataManage();
	COptionStd      *pDomOpt		= pStd->GetDomOptionStd(); 
	CARoadOptionStd *pOptStd		= pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB			= pDataMng->GetBridge();
	CADeckData		*pDeckData		= pDataMng->GetDeckData();
	CADeckDataTU	*pDeckDataTU	= pDataMng->GetDeckDataTU();
	CRebar			*pRB			= NULL;

	CHgBaseDrawStd HGDraw;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar = new CSafetyTypedPtrArray<CObArray, CRebar*>;
	long nG		= nSubPage-1;
	CString sText;

	for(long n=0; n<pDeckDataTU->m_arrRebarJewonTU.GetSize(); n++)
	{
		pRB = pDeckDataTU->m_arrRebarJewonTU.GetAt(n);
		long nFind = pRB->m_sDescription.Find("_G");
		sText = pRB->m_sDescription.Mid(nFind+2, 2);
		nFind = (long)atof(sText);
		if(nFind-1 == nG)
			pArrRebar->Add(new CRebar(pRB));
	}
	pDeckData->ShellSortRebarList(pArrRebar, &CRebar::CompareForTable);

	double dLengthSum		= 0;
	double dLengthSumAdd	= 0;
	double dLengthTotal		= 0;	
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;
	long nMaxRow = (long)pDomOpt->m_dDeck_DatasuOnRebarTable;
	double dFy = toKgPCm2(pDeckData->m_SigmaY);
	BOOL bShadeOutLine = TRUE;
	CDPoint xyBase	= CDPoint(0,0);


	HGDraw.DrawRebarTable(&Dom, 0, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, 
		dWeightTotal, dWeightTotalAdd, *pArrRebar,dFy, bShadeOutLine);

	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	Dom.Move(-rectDom.right, -rectDom.top);
	*pDom << Dom;
}

long CAPlateOutTUGirder::GetPageSu()
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	= pStd->GetDataManage()->GetBridge();

	if(pDB->IsTUGir())	m_nPage = pDB->GetQtyGirder()+2;
	else				m_nPage = 0;
	return m_nPage;
}

double CAPlateOutTUGirder::GetScale()
{
	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	 = pStd->m_pDomOptionStd;

	m_Scale = pOpt->m_dTUScale;
	return m_Scale;
}

CString CAPlateOutTUGirder::GetTitle()
{
	CAPlateDrawStd  *pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	= pStd->GetDataManage()->GetBridge();

	CString	szTitle	= _T("");

	if(pStd->m_nCurPage-m_nSttPage<=pDB->GetQtyGirder())
		szTitle.Format("Turnover 거더 배근도 (%d/%d)", pStd->m_nCurPage-m_nSttPage,pDB->GetQtyGirder());
	if(pStd->m_nCurPage-m_nSttPage==pDB->GetQtyGirder()+1)
		szTitle = _T("상부구조 시공 순서도");
	if(pStd->m_nCurPage-m_nSttPage==pDB->GetQtyGirder()+2)
		szTitle = _T("Turnover 거더 전도방지책");

	return szTitle;
}