// DrawDBBaseRebarJong.cpp: implementation of the CDrawDBBaseRebarJong class.
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

CDrawDBBaseRebarJong::CDrawDBBaseRebarJong(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CDrawDBBaseRebarJong::~CDrawDBBaseRebarJong()
{

}


void CDrawDBBaseRebarJong::DrawBeryukRebarAtJong(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput)
{
	CDomyun Dom(pDom);

	DrawJDanSlab(&Dom, nJongBasePos);
	DrawBeryukRebarUp(&Dom, nJongBasePos, nCycle, bInput);
	DrawBeryukRebarDn(&Dom, nJongBasePos, nCycle, bInput);
	
	*pDom << Dom;
}



void CDrawDBBaseRebarJong::DrawBeryukRebarUp(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CDrawDBDeck DeckDraw(m_pDataMng);

	CDomyun Dom(pDom);
	
	//////////////////////////////////////////////////////////////////////////
	// 배력철근                                                             
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);
	vector<REBAR_SHAPE> vRebarShape = pDeckData->GetNewRebarShapeArrByJongBasePos(nJongBasePos, nCycle, TRUE);
	if(vRebarShape.size() < 1)	
	{
		*pDom << Dom;
		return;
	}

	DeckDraw.DrawRebarShapeArr(&Dom, vRebarShape, bInput, FALSE, -1);
	
	//////////////////////////////////////////////////////////////////////////
	// 배력철근 이음장 표시 
	// 1. 지점부 위치의 가상 겹이음은 제외함
	// 2. 폐합이 아닌경우 제외함
	BOOL bInstallRebarCen = pDeckData->IsInstallBeryukRebarAtCenter(nCycle);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_BARA);

	CDPointArray xyArrDim, xyArrStt, xyArrEnd;
	if (bInstallRebarCen)
	{
		for(long nShape=0; nShape<(long)vRebarShape.size(); nShape++)
		{
			CDPointArray xyArrRebar = vRebarShape[nShape].m_dArrShape;

			if (nShape==0 || nShape==vRebarShape.size()-1)
				xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
			else
			{
				xyArrDim.Add(xyArrRebar[0]);
				xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
			}

		}

	}

	xyArrDim.Sort(TRUE);
	
	if(xyArrDim.GetSize() > 1)
	{
		for(long n=0; n<xyArrDim.GetSize(); n+=2)
		{
			long nCheck = (n/2) % 2;
			if(bInput)
			{
				xyArrDim[n].y += (nCheck==0?1:-1)*REBAR_MAIN_LAPJOINT_GAP;
				xyArrDim[n+1].y += (nCheck==0?1:-1)*REBAR_MAIN_LAPJOINT_GAP;
			}
			else
			{
				xyArrDim[n].y += REBAR_MAIN_LAPJOINT_GAP;
				xyArrDim[n+1].y += REBAR_MAIN_LAPJOINT_GAP;
			}
			if(bInput)
			{
				Dom.SetLineColor(7);
				Dom.SetPLineThick(20);
				Dom.PMoveTo(xyArrDim[n].x, xyArrDim[n].y, 3);
				Dom.PLineTo(xyArrDim[n+1].x, xyArrDim[n+1].y, 3);
				Dom.PLineEnd();
			}
			else
				Dom.LineTo(xyArrDim[n], xyArrDim[n+1]);
		}
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarJong::DrawBeryukRebarDn(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);
	CDrawDBDeck DeckDraw(m_pDataMng);

	CDomyun Dom(pDom);
	
	//////////////////////////////////////////////////////////////////////////
	// 배력철근                                                             
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);
	vector<REBAR_SHAPE> vRebarShape = pDeckData->GetNewRebarShapeArrByJongBasePos(nJongBasePos, nCycle, FALSE);
	if(vRebarShape.size() < 1)	
	{
		*pDom << Dom;
		return;
	}

	DeckDraw.DrawRebarShapeArr(&Dom, vRebarShape, bInput, FALSE, -1);

	//////////////////////////////////////////////////////////////////////////
	// 배력철근 이음장 표시			
	// 1. 지점부 위치의 가상 겹이음은 제외함
	// 2. 폐합이 아닌경우 제외함											
	BOOL bInstallRebarCen = pDeckData->IsInstallBeryukRebarAtCenter(nCycle);
	BOOL bLowerStt2 = pDeckData->IsInstallBeryukRebarAtDanHunch(nJongBasePos, TRUE, nCycle);
	BOOL bLowerEnd2 = pDeckData->IsInstallBeryukRebarAtDanHunch(nJongBasePos, FALSE, nCycle);
	CDPointArray xyArrDim, xyArrStt, xyArrEnd;
	if (bInstallRebarCen)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ** 주의 ** : 테이퍼가 없을경우 시점부하부(1) 철근이 없기 때문에 실제로 고려되는 Shape수를 달리 적용해야한다
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		long nStt = 1;//pDB->m_dHeightTaperStt>0 ? 1 : 0;
		long nEnd = pDB->m_dHeightTaperEnd>0 ? vRebarShape.size()-1 : vRebarShape.size();
		if (!bLowerStt2 || (pDB->m_cSlabThickAddTypeStt==1 && nJongBasePos==JONGBASEPOS_ONCB))
			nStt++;
		if (!bLowerEnd2 || (pDB->m_cSlabThickAddTypeEnd==1 && nJongBasePos==JONGBASEPOS_ONCB))
			nEnd--;

		for(long nShape=nStt; nShape<nEnd; nShape++)
		{
			CDPointArray xyArrRebar = vRebarShape[nShape].m_dArrShape;
			if(xyArrRebar.GetSize()<2)	continue;

			if (nShape==nStt)
				xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
			else if(nShape==nEnd-1)
			{
				xyArrDim.Add(xyArrRebar[0]);
				if(pDB->m_dHeightTaperEnd==0)
					xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
			}
			else
			{
				xyArrDim.Add(xyArrRebar[0]);
				xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
			}
		}
	}

	xyArrDim.Sort(TRUE);

	if(xyArrDim.GetSize() > 1)
	{
		long nArrSize	= pDB->m_dHeightTaperEnd>0 ? xyArrDim.GetSize() : xyArrDim.GetSize()-1;
		for(long n=0; n<nArrSize; n+=2)
		{
			long nCheck = (n/2) % 2;
			if(bInput)
			{
				xyArrDim[n].y += (nCheck==0?1:-1)*REBAR_MAIN_LAPJOINT_GAP;
				xyArrDim[n+1].y += (nCheck==0?1:-1)*REBAR_MAIN_LAPJOINT_GAP;
			}
			else
			{
				xyArrDim[n].y += REBAR_MAIN_LAPJOINT_GAP;
				xyArrDim[n+1].y += REBAR_MAIN_LAPJOINT_GAP;
			}
			if(bInput)
			{
				Dom.SetLineColor(7);
				Dom.SetPLineThick(20);
				Dom.PMoveTo(xyArrDim[n].x, xyArrDim[n].y, 3);
				Dom.PLineTo(xyArrDim[n+1].x, xyArrDim[n+1].y, 3);
				Dom.PLineEnd();
			}
			else
				Dom.LineTo(xyArrDim[n], xyArrDim[n+1]);
		}
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarJong::DrawJDanSlab(CDomyun *pDom, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);
	
	CDomyun Dom(pDom);
	
	//////////////////////////////////////////////////////////////////////////
	//  슬래브단면                                                         
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);	
	CDPointArray xyArrSlab = pDeckData->GetShapeSlabJongBasePos(nJongBasePos);
	CString str = _T("");
	long n = 0;
	for(n=0; n<xyArrSlab.GetSize(); n++)
	{
		if(n==0)	Dom.MoveTo(xyArrSlab[n]);
		else		Dom.LineTo(xyArrSlab[n]);
	}
	Dom.LineTo(xyArrSlab[0]);

	//////////////////////////////////////////////////////////////////////////
	// 신축이음 
	double dPave = pDB->GetThickPave();
	double dExpWStt = pDeckData->GetWidthBlockOut(TRUE, nJongBasePos);
	double dExpWEnd = pDeckData->GetWidthBlockOut(FALSE, nJongBasePos);
	double dExpH = m_pDataMng->GetGlobalOption()->GetSttExpansionJointHeight()-dPave;
	double dLengthSlab = pDeckData->GetLengthSlabTotal(nJongBasePos);

	Dom.Rectangle(0, 0, dExpWStt, -dExpH);
	Dom.Rectangle(dLengthSlab-dExpWEnd, 0, dLengthSlab, -dExpH);
	
	//////////////////////////////////////////////////////////////////////////
	// 지점마크
	double dSta = pGirCen->GetSSS(OFF_STT_SLAB);
	double dDis = 0;	// 거더 중심에서 이격거리
	if(nJongBasePos==JONGBASEPOS_SLABCENTER)	// 교량중심
	{
		if(pDB->GetDisSlabLeftAct(dSta)>0 || pDB->GetDisSlabRighAct(dSta)<0)	// 선형이 슬래브 밖에 있을때 
			dDis = (pDeckData->GetSideBarDistFromCenterGir(TRUE, FALSE) + pDeckData->GetSideBarDistFromCenterGir(TRUE, TRUE))/2;
		else	// 선형이 슬래브 안에 있을때
		{
			double dSlabW = pDB->GetWidthAct(dSta);
			dDis = dSlabW/2 + pDB->GetDisSlabLeftAct(dSta); 
		}
	}
	else if(nJongBasePos==JONGBASEPOS_SLABLEFT)	// 슬래브좌측
		dDis = pDB->GetDisSlabLeftAct(dSta);
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)	// 슬래브우측
		dDis = pDB->GetDisSlabRighAct(dSta);


	double dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB), dDis, pGirCen->GetAAA(OFF_STT_SLAB));

	for(n=0; n<pDB->m_nQtyJigan+1; n++)
	{
		// 지점
		CSymbolDom *pSym = new CSymbolDom(&Dom, m_pDataMng->GetOptionStd());
		pSym->SetScaleDim(40);
	
		CPlateBasicIndex *pBx = pGirCen->GetBxOnJijum(n);
		double dStaJijim = pGirCen->GetStationBySc(pBx->GetStation(), dDis, pBx->GetAngle());
		double dLJijim = pGirCen->GetLengthDimType(dStaStt, dStaJijim, dDis);
		CString strJijum = pDB->m_strJijumName[n];
		m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_STLC);
		pSym->DrawSymbolJijumMark(0,0, strJijum, 0, TRUE); 
		pSym->SetBlockName(strJijum);

		CObInsert *pIns = new CObInsert;		
		pIns->m_sBlockName = strJijum;
		if(pBx->IsDanbuJijum())
			pIns->m_vPoint = CDPoint(dLJijim, -pDB->m_dThickSlabBase-pDB->m_dHeightTaperStt);	
		else
			pIns->m_vPoint = CDPoint(dLJijim, -pDB->m_dThickSlabBase);	

		pIns->m_vSFactor = CVector(1,1,1);
		Dom.AddBlockAndInsert(pSym, pIns);
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarJong::DrawMainRebarAtJong(CDomyun *pDom, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);
	CDrawDBDeck DeckDraw(m_pDataMng);

	double dCoverUp           = pDeckData->m_dCoverUp;            
	double dCoverSd           = pDeckData->m_dCoverSd;
	double dThickPave	      = pDB->GetThickPave();
	double dSttBlockOutHeight = m_pDataMng->GetGlobalOption()->GetSttExpansionJointHeight()-dThickPave;
	double dSttBlockOutWidth  = pDeckData->GetWidthBlockOut(TRUE, nJongBasePos);
	double dEndBlockOutHeight = m_pDataMng->GetGlobalOption()->GetEndExpansionJointHeight()-dThickPave;
	double dEndBlockOutWidth  = pDeckData->GetWidthBlockOut(FALSE, nJongBasePos);
	if(pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth  = 0;
		dEndBlockOutWidth  = 0;
	}
	double dSlabTotalL		  = pDeckData->GetLengthSlabTotal(nJongBasePos);
	
	CDPoint vX(1,0), vY(0,1), xyStt, xyEnd, xyDim;
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);
		
	if(pDB->m_dHeightTaperStt > 0 && !pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		//신축이음부 아래 조금 있는 단부 상면주철근 /////////////////////////////////////////////////////
		//시점
		xyStt = vX*dCoverSd-vY*(dSttBlockOutHeight+dCoverUp);
		DeckDraw.DrawPointRebar(&Dom, xyStt, REBARPOS_UP);
		for (long n=0; n<pDeckData->m_arrCTCMainRebarUnderSinchuk.GetSize(); n++)
		{
			xyDim = pDeckData->m_arrCTCMainRebarUnderSinchuk[n];
			for (long nn=0; nn<xyDim.x; nn++)
			{
				xyStt += vX*xyDim.y;
				DeckDraw.DrawPointRebar(&Dom, xyStt, REBARPOS_UP);
			}
		}
	}
	if(pDB->m_dHeightTaperEnd > 0 && !pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		//종점
		xyStt = vX*(dSlabTotalL-dCoverSd) - vY*(dEndBlockOutHeight+dCoverUp);
		DeckDraw.DrawPointRebar(&Dom, xyStt, REBARPOS_UP);
		for (long n=0; n<pDeckData->m_arrCTCMainRebarUnderSinchuk.GetSize(); n++)
		{
			xyDim = pDeckData->m_arrCTCMainRebarUnderSinchuk[n];
			for (long nn=0; nn<xyDim.x; nn++)
			{
				xyStt -= vX*xyDim.y;
				DeckDraw.DrawPointRebar(&Dom, xyStt, REBARPOS_UP);
			}
		}
	}
	//상면 주철근 /////////////////////////////////////////////////////////////////////////////
	xyStt = vX*(dCoverSd+dSttBlockOutWidth) - vY*dCoverUp;
	CDoubleArray dArrDim = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE);
	DeckDraw.DrawPointRebar(&Dom, xyStt, REBARPOS_UP);
	long n = 0;
	for (n=0; n<dArrDim.GetSize(); n++)
	{
		xyStt = CDPoint(dArrDim[n], xyStt.y);
		DeckDraw.DrawPointRebar(&Dom, xyStt, REBARPOS_UP);
	}

	//하면주철근////////////////////////////////////////////////////////////////////////////// 
	dArrDim = pDeckData->GetRebarPosAtJongBasePos(FALSE, nJongBasePos, TRUE);
	CDPointArray xyArrGuide = pDeckData->GetXyArrGuideLineForBeryukRebar(nJongBasePos, FALSE);
	CDPointArray xyArrRebar = pDeckData->GetCDPointArrByDoubleLine(dArrDim, xyArrGuide);
	for(n=0; n<xyArrRebar.GetSize(); n++)
		DeckDraw.DrawPointRebar(&Dom, xyArrRebar.GetAt(n), REBARPOS_DOWN);

	// 단부 헌치 정착장 주철근//////////////////////////////////////////////////////////////// 
	if (pDeckData->m_bInstallDanbuTaperMainRebar)
	{
		CDPointArray xyArrStt = pDeckData->GetXyArrRebarJongPosAtDanHunch(TRUE, nJongBasePos);
		CDPointArray xyArrEnd = pDeckData->GetXyArrRebarJongPosAtDanHunch(FALSE, nJongBasePos);
		for(n=0; n<xyArrStt.GetSize(); n++)
			DeckDraw.DrawPointRebar(&Dom, xyArrStt.GetAt(n), REBARPOS_UP);
		for(n=0; n<xyArrEnd.GetSize(); n++)
			DeckDraw.DrawPointRebar(&Dom, xyArrEnd.GetAt(n), REBARPOS_UP);

	}

	*pDom << Dom;	
}


void CDrawDBBaseRebarJong::DrawShearRebarAtJong(CDomyun *pDom, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CDrawDBDeck DeckDraw(m_pDataMng);

	CDomyun Dom(pDom);
	CDoubleArray dArrDimUpMain = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE, 0);
	CDPointArray xyArrGuide = pDeckData->GetXyArrGuideLineForBeryukRebar(nJongBasePos, FALSE);
	CDPointArray xyArrPointUp;
	long n = 0;
	for (n=0; n<dArrDimUpMain.GetSize(); n++)	
		xyArrPointUp.Add(CDPoint(dArrDimUpMain[n], -pDeckData->m_dCoverUp));
	CDPointArray xyArrPointLo = pDeckData->GetCDPointArrByDoubleLine(dArrDimUpMain, xyArrGuide);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BNTB);
	long nCount = long(pDeckData->m_SpacerRebar.GetCntMainRebarSet()/2);
	if(nCount==0)	return;

	long nDot = 0;
	for(n=0; n<xyArrPointUp.GetSize(); n+=nCount)
	{	
		if(nDot%2==0)			
			m_pDataMng->GetOptionStd()->SetEnvLType(&Dom, LT_SOLID);
		else
			m_pDataMng->GetOptionStd()->SetEnvLType(&Dom, LT_HIDDEN2_LINE);
		Dom.LineTo(xyArrPointUp[n], xyArrPointLo[n]);
		nDot++;
	}

	*pDom << Dom;
}


void CDrawDBBaseRebarJong::DimMainRebarAtJong(CDomyun *pDom, long nJongBasePos, BOOL bUpper)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);
	CDimDBDeckBasePlan DimPlan(m_pDataMng);
	
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);	
	
	CString sText;
	CDPointArray xyArrDimReal   = pDeckData->GetDimTotalAtJongBasePos(bUpper?TRUE:FALSE, nJongBasePos, TRUE);
	CDPointArray xyArrDimForDim = pDeckData->GetDimTotalAtJongBasePos(bUpper?TRUE:FALSE, nJongBasePos, FALSE);
	CDoubleArray DimDistReal    = DimPlan.GetDistArrReal(xyArrDimReal, xyArrDimForDim);

	CHgBaseDrawStd baseDraw;
	CStringArray sArrText, sArrUnderText;
	CString sDir= bUpper?_T("TOP"):_T("BOTTOM");

	for(long n=0; n<xyArrDimForDim.GetSize(); n++)
	{
		CDPoint xyDim = xyArrDimForDim.GetAt(n);

		if(xyDim.x==1)	sText.Format("%.0f", xyDim.y);
		else			sText.Format("%.0f@%.0f", xyDim.x, xyDim.y);
		
		sArrText.Add(sText);
	}
	baseDraw.AutoDimCont(&Dom, CDPoint(0, bUpper?0:-pDB->m_dThickSlabBase-pDB->m_dHeightTaperStt), DimDistReal, sArrText, sArrUnderText, 1, sDir, -1, TRUE, FALSE, FALSE);

	*pDom << Dom;
}


void CDrawDBBaseRebarJong::DimMainRebarAtJongSd(CDomyun *pDom, long nJongBasePos, BOOL bSttSlab, BOOL bIsSlabCommon, BOOL bIsText, BOOL bDimTop)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);
 
	double dThickPave			= pDB->GetThickPave();
	double dThickSlab			= pDB->m_dThickSlabBase;
	double dHeightSlabStt		= dThickSlab+pDB->m_dHeightTaperStt;		
	double dHeightSlabEnd		= dThickSlab+pDB->m_dHeightTaperEnd;		
	double dSttBlockOutHeight	= m_pDataMng->GetGlobalOption()->GetSttExpansionJointHeight()-dThickPave;
	double dSttBlockOutWidth	= pDeckData->GetWidthBlockOut(TRUE, nJongBasePos);
	double dEndBlockOutHeight	= m_pDataMng->GetGlobalOption()->GetEndExpansionJointHeight()-dThickPave;
	double dCoverUp				= pDeckData->m_dCoverUp;             
	double dCoverDn				= pDeckData->m_dCoverDn;
	double dLengthBridge		= pDeckData->GetLengthSlabTotal(nJongBasePos);

	CDPoint vX(1,0), vY(0,1), xyTemp, xyResult, xyStt, xyEnd;
	CDPoint xyOriginStt = CDPoint(0,-dHeightSlabStt);
	CDPoint	xyOriginSttUp = CDPoint(0,0);
	CString str="";

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	long nCol = 1;
	if(bSttSlab)
	{
		if(bIsSlabCommon)
		{
			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyOriginStt.x, xyOriginStt.y, nCol);
			Dom.DimLineTo(dHeightSlabStt, nCol, COMMA(dHeightSlabStt));
			Dom.DimLineToOver(dThickPave, nCol, COMMA(dThickPave), 1);

			Dom.DimMoveTo(xyOriginStt.x, xyOriginStt.y, nCol+1);
			Dom.DimLineTo(dHeightSlabStt+dThickPave, nCol+1, COMMA(dHeightSlabStt+dThickPave));

			if(bDimTop)
			{
				Dom.SetDirection("TOP");
				Dom.DimMoveTo(xyOriginSttUp.x, xyOriginSttUp.y+dThickPave, nCol, "");
				double dDisSlabSttToCr = pDB->m_dLengthSlabGirderStt+pDB->m_dLengthGirderShoeStt;
				Dom.DimLineTo(dDisSlabSttToCr, nCol, COMMA(dDisSlabSttToCr));
				double dDis = ~(pDeckData->m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].GetAt(1) - pDeckData->m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].GetAt(0));
				Dom.DimLineTo(dDis-dDisSlabSttToCr, nCol, COMMA(dDis-dDisSlabSttToCr));
				Dom.DimLineTo(pDB->m_dWidthHunch*5, nCol, COMMA(pDB->m_dWidthHunch*5));
			
			}
			if(bIsText)
			{

				Dom.SetDirection("TOP");
				Dom.GiSiArrowExtend(xyOriginStt.x + dSttBlockOutWidth/2, xyOriginStt.y + dHeightSlabStt + dSttBlockOutHeight/2,
									Dom.Always(10), -Dom.Always(35), 3, "무수축 콘크리트", "");

				str.Format("T = %.f mm", dThickPave);
				Dom.GiSiArrowExtend(xyOriginStt.x + dSttBlockOutWidth + 500, xyOriginStt.y + dHeightSlabStt + dSttBlockOutHeight/2,
									Dom.Always(5), Dom.Always(35), 1, "교면 포장", str);

				Dom.GiSiArrowExtend(pGirCen->GetBxOnJijum(0)->GetStation(), xyOriginStt.y - dHeightSlabStt/3, Dom.Always(10), Dom.Always(30), 0, "단지점 횡형", "");

			}
		}
		else
		{
			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyOriginStt.x, xyOriginStt.y, nCol, COMMA(dCoverDn));
			Dom.DimLineTo(dCoverDn, nCol, "");
			Dom.DimLineTo(dHeightSlabStt-(dCoverUp+dCoverDn+dSttBlockOutHeight), nCol, COMMA(dHeightSlabStt-(dCoverUp+dCoverDn+dSttBlockOutHeight)));
			Dom.DimLineToExtend(dCoverUp, nCol, "", COMMA(dCoverUp));
			Dom.DimLineTo(dSttBlockOutHeight-dCoverUp, nCol, _T(""));
			Dom.DimLineToOver(dCoverUp, nCol, COMMA(dCoverUp));
		}
	}
	else
	{
		if(bIsSlabCommon)
		{
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xyOriginStt.x, xyOriginStt.y, nCol);
			Dom.DimLineTo(dHeightSlabEnd, nCol, COMMA(dHeightSlabEnd));
			Dom.DimLineToOver(dThickPave, nCol, COMMA(dThickPave));

			Dom.DimMoveTo(xyOriginStt.x, xyOriginStt.y, nCol+1);
			Dom.DimLineTo(dHeightSlabStt+dThickPave, nCol+1, COMMA(dHeightSlabStt+dThickPave));

		}
		else
		{
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(dLengthBridge, -dHeightSlabEnd, nCol, COMMA(dCoverDn));
			Dom.DimLineTo(dCoverDn, nCol, "");
			Dom.DimLineTo(dHeightSlabEnd-(dCoverUp+dCoverDn+dEndBlockOutHeight), nCol, COMMA(dHeightSlabEnd-(dCoverUp+dCoverDn+dEndBlockOutHeight)));
			Dom.DimLineToExtend(dCoverUp, nCol, "", COMMA(dCoverUp));
			Dom.DimLineTo(dSttBlockOutHeight-dCoverUp, nCol, _T(""));
			Dom.DimLineToOver(dCoverUp, nCol, COMMA(dCoverUp));
		}
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarJong::DimBeryukRebarAtJong(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput, double dTextRatio)
{
	DimBeryukRebarAtJongUp(pDom, nJongBasePos, nCycle, bInput, dTextRatio);
	DimBeryukRebarAtJongDn(pDom, nJongBasePos, nCycle, bInput, dTextRatio);
}

void CDrawDBBaseRebarJong::DimBeryukRebarAtJongUp(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput, double dTextRatio)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	if(dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);

	Dom.SetDirection("TOP");

	//////////////////////////////////////////////////////////////////////////
	// 상부철근                                                             
	//////////////////////////////////////////////////////////////////////////
	double dCoverSd           = pDeckData->m_dCoverSd;
	double dSttBlockOutWidth  = pDeckData->GetWidthBlockOut(TRUE, nJongBasePos);
	double dEndBlockOutWidth  = pDeckData->GetWidthBlockOut(FALSE, nJongBasePos);
	if(pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth  = 0;
		dEndBlockOutWidth  = 0;
	}
	
	vector<REBAR_SHAPE> vRebarShape = pDeckData->GetNewRebarShapeArrByJongBasePos(nJongBasePos, nCycle, TRUE);
	long nShapeSu = vRebarShape.size();
	CDPointArray xyArrStt = vRebarShape.at(0).m_dArrShape;
	CDPointArray xyArrEnd = vRebarShape.at(nShapeSu-1).m_dArrShape;
	CString sText, sVariable;
	REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos, nCycle);
	
	double dYMax = 0;
	// 2 단치수선에서 사용함.
	CDPointArray xyArrDim;	
	xyArrDim.Add(xyArrStt[0]);
	for(long nShape=0; nShape<nShapeSu; nShape++)
	{
		REBAR_SHAPE vShape = vRebarShape.at(nShape);
		if(vShape.m_bValid==FALSE)		continue;

		CDPointArray xyArrRebar = vShape.m_dArrShape;
		CString szShape = vShape.m_sName;
	
		if (nShape==0 || nShape==vRebarShape.size()-1)
			xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		else
		{
			xyArrDim.Add(xyArrRebar[0]);
			xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		}

		for (long m=0; m<xyArrRebar.GetSize(); m++)
			dYMax = max(dYMax, xyArrRebar[m].y);
	}
	xyArrDim.Add(xyArrEnd[0]);		
	xyArrDim.Sort(TRUE);					

	Dom.SetDirection("TOP");
	
	//////////////////////////////////////////////////////////////////////////
	// 1단 이음위치(U1, U2, ...)
	long nCol = 0;
	CDoubleArray arrJoint = pRebarCycle->m_dArrJointUp;

	if(bInput)
	{
		Dom.DimMoveTo(xyArrStt[0].x-dCoverSd-dSttBlockOutWidth, dYMax, nCol, "");
		Dom.DimLineTo(dSttBlockOutWidth, nCol, COMMA(dSttBlockOutWidth));
		Dom.DimLineToExtend(dCoverSd, nCol, "", COMMA(dCoverSd));

		for (long n=0; n<arrJoint.GetSize(); n++)
		{
			sText.Format("U%d-%d", n+1, nCycle+1);
			sVariable.Format("%s", COMMA(arrJoint[n]));
			if(bInput)
				Dom.DimLineToExtend(arrJoint[n], nCol, sText, sVariable);
			else
				Dom.DimLineTo(arrJoint[n], nCol, sVariable);
		}
		Dom.DimLineToExtend(dCoverSd, nCol, "", COMMA(dCoverSd));
		Dom.DimLineTo(dEndBlockOutWidth, nCol, COMMA(dEndBlockOutWidth));
		nCol++;
	}
	//////////////////////////////////////////////////////////////////////////
	// 2단 이음장표현포함
	for(long n=0; n<xyArrDim.GetSize(); n++)
	{
		if(n==0)	
		{
			Dom.DimMoveTo(xyArrDim[n].x-dCoverSd-dSttBlockOutWidth, dYMax, nCol);
			Dom.DimLineTo(dCoverSd+dSttBlockOutWidth, nCol, COMMA(dCoverSd+dSttBlockOutWidth));
		}
		else	
		{
			sVariable.Format("%s", COMMA(xyArrDim[n].x-xyArrDim[n-1].x));
			Dom.DimLineTo(xyArrDim[n].x-xyArrDim[n-1].x, nCol, sVariable);
		}
	}
	Dom.DimLineTo(dCoverSd+dEndBlockOutWidth, nCol, COMMA(dCoverSd+dEndBlockOutWidth));
	nCol++;
	//////////////////////////////////////////////////////////////////////////
	// 전체치수
	if(xyArrEnd[1].x-xyArrStt[0].x > 0)
	{
		Dom.DimMoveTo(xyArrStt[0].x-dCoverSd-dSttBlockOutWidth, dYMax, nCol);
		sVariable.Format("%s", COMMA(xyArrEnd[1].x-xyArrStt[0].x+dCoverSd*2+dSttBlockOutWidth+dEndBlockOutWidth));		
		Dom.DimLineTo(xyArrEnd[1].x-xyArrStt[0].x+dCoverSd*2+dSttBlockOutWidth+dEndBlockOutWidth, nCol, sVariable);
	}

	*pDom << Dom;	
}

void CDrawDBBaseRebarJong::DimBeryukRebarAtJongDn(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput, double dTextRatio)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("BOTTOM");
	if(bInput && dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);

	double	dLengthSlab	= pDeckData->GetLengthSlabTotal(nJongBasePos);
	double	dCoverSdStt	= pDeckData->GetLengthCoverSd(TRUE);
	double	dCoverSdEnd	= pDeckData->GetLengthCoverSd(FALSE);
	
	vector<REBAR_SHAPE> vRebarShape = pDeckData->GetNewRebarShapeArrByJongBasePos(nJongBasePos, nCycle, FALSE);
	long nShapeSu = vRebarShape.size();
	CString sText, sVariable;
	REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos, nCycle);

	CString	str = _T("");
	CDoubleArray arrDim;
	CDoubleArray arrDimInput;
	double dYMin = 10000000;

	arrDim.Add(0);
	arrDim.Add(dCoverSdStt);
	arrDim.Add(dLengthSlab-dCoverSdEnd);
	arrDim.Add(dLengthSlab);

	long nVaildSize	= pDB->m_dHeightTaperEnd>0 ? nShapeSu-1 : nShapeSu;

	for(long nShape=1; nShape<nVaildSize; nShape++)
	{
		REBAR_SHAPE vShape = vRebarShape.at(nShape);
		if(vShape.m_bValid==FALSE)		continue;

		//2009.03.31 LSH 추가
		if(!vShape.m_dArrShape.GetSize()) continue;

		CDPointArray xyArrRebar = vShape.m_dArrShape;
		CString szShape = vShape.m_sName;
		
		arrDim.Add(xyArrRebar[0].x);
		arrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()].x);

		for (long m=0; m<xyArrRebar.GetSize(); m++)
			dYMin = min(dYMin, xyArrRebar[m].y);
	}
	
	arrDim.Sort(TRUE);							

	CHgBaseDrawStd baseDraw;
	CString sArrText, sArrUnderText;
	CString sDir= _T("BOTTOM");

	long nCol = 1;
	CDoubleArray arrJoint = pRebarCycle->m_dArrJointDn;
	CDPointArray xyArrGuideDn = pDeckData->GetXyArrGuideLineForBeryukRebar(nJongBasePos, FALSE);

	if(bInput)
	{
		Dom.DimMoveTo(0, xyArrGuideDn[0].y, nCol, "");
		Dom.DimLineTo(dCoverSdStt, nCol, COMMA(dCoverSdStt));

		for (long n=0; n<arrJoint.GetSize(); n++)
		{
			sText.Format("D%d-%d", n+1, nCycle+1);
			sVariable.Format("%s", COMMA(arrJoint[n]));
			if(bInput)
				Dom.DimLineToExtend(arrJoint[n], nCol, sText, sVariable);
			else
				Dom.DimLineTo(arrJoint[n], nCol, sVariable);
		}
		Dom.DimLineTo(dCoverSdEnd, nCol, COMMA(dCoverSdEnd));
	}

	nCol = 2;
	Dom.DimMoveToExt(0, xyArrGuideDn[0].y, nCol, COMMA(arrDim[1]-arrDim[0]));
	for(long nDim=0; nDim<arrDim.GetSize()-1; nDim++)
	{
		if(nDim==0)
			Dom.DimLineTo(arrDim[nDim+1]-arrDim[nDim], nCol, "");
		else
		{
			if (nDim%2)
				Dom.DimLineTo(arrDim[nDim+1]-arrDim[nDim], nCol, COMMA(arrDim[nDim+1]-arrDim[nDim]));
			else
				Dom.DimLineToExtend(arrDim[nDim+1]-arrDim[nDim], nCol, "", COMMA(arrDim[nDim+1]-arrDim[nDim]));
		}
	}
	
	
	*pDom << Dom;	
}

// 배력철근 가변치수 표시
void CDrawDBBaseRebarJong::DimBeryukRebarAtJongVariable(CDomyun *pDom, long nCycle, BOOL bInput, double dTextRatio)
{
/*	
	CPlateBridgeApp		*pDB		= pStd->GetBridge();
	CADeckData		*pDeckData	= pStd->GetDeckData();
	
	CDoubleArray arrDimCenterUp,	arrDimCenterDn;
	CDoubleArray arrDimLeftUp,		arrDimLeftDn;
	CDoubleArray arrDimRightUp,		arrDimRightDn;
	CString str = _T("");

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("TOP");
	if(bInput && dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);	
	
	// 1=중앙부, 2=좌측, 3=우측 
	long nJongBasePos = 1;	
	// 상부치수
	arrDimCenterUp	= pDeckData->GetSlabRebarVariableArr(nCycle, nJongBasePos, TRUE);	
	nJongBasePos	= 2;	
	arrDimLeftUp	= pDeckData->GetSlabRebarVariableArr(nCycle, nJongBasePos, TRUE);	
	nJongBasePos	= 3;	
	arrDimRightUp	= pDeckData->GetSlabRebarVariableArr(nCycle, nJongBasePos, TRUE);	

	long nCol = 1;
	Dom.DimMoveTo(0, 0, nCol);
	if(bInput)
	{
		for(long nDim=0; nDim<arrDimCenterUp.GetSize()-1; nDim++)
		{
			if(nDim == 0 || nDim == arrDimCenterUp.GetSize()-2)
			{
				if((arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]) - (arrDimRightUp[nDim+1]-arrDimRightUp[nDim]) == 0)
					str.Format("%s", Comma(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]));
				else
					str.Format("%s~%s", Comma(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]), Comma(arrDimRightUp[nDim+1]-arrDimRightUp[nDim]));
				Dom.DimLineToExtend(arrDimCenterUp[nDim+1]-arrDimCenterUp[nDim], nCol, _T(""), str);
			}
			else
			{
				if((arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]) - (arrDimRightUp[nDim+1]-arrDimRightUp[nDim]) == 0)
					str.Format("%s", Comma(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]));
				else
					str.Format("%s~%s", Comma(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]), Comma(arrDimRightUp[nDim+1]-arrDimRightUp[nDim]));
				Dom.DimLineTo(arrDimCenterUp[nDim+1]-arrDimCenterUp[nDim], nCol, str);
			}
		}
	}
	else
	{
		for(long nDim=0; nDim<arrDimCenterUp.GetSize()-1; nDim++)
		{
			if(nDim == 0 || nDim == arrDimCenterUp.GetSize()-2)
			{
				if((arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]) - (arrDimRightUp[nDim+1]-arrDimRightUp[nDim]) == 0)
					str.Format("%s", COMMA(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]));
				else
					str.Format("%s~%s", COMMA(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]), COMMA(arrDimRightUp[nDim+1]-arrDimRightUp[nDim]));
				Dom.DimLineToExtend(arrDimCenterUp[nDim+1]-arrDimCenterUp[nDim], nCol, _T(""), str);
			}
			else
			{
				if((arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]) - (arrDimRightUp[nDim+1]-arrDimRightUp[nDim]) == 0)
					str.Format("%s", COMMA(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]));
				else
					str.Format("%s~%s", COMMA(arrDimLeftUp[nDim+1]-arrDimLeftUp[nDim]), COMMA(arrDimRightUp[nDim+1]-arrDimRightUp[nDim]));
				Dom.DimLineTo(arrDimCenterUp[nDim+1]-arrDimCenterUp[nDim], nCol, str);
			}
		}
	}
	// 하부치수
	Dom.SetDirection("BOTTOM");

	nJongBasePos	= 1;	
	arrDimCenterDn	= pDeckData->GetSlabRebarVariableArr(nCycle, nJongBasePos, FALSE);	
	nJongBasePos	= 2;	
	arrDimLeftDn	= pDeckData->GetSlabRebarVariableArr(nCycle, nJongBasePos, FALSE);	
	nJongBasePos	= 3;	
	arrDimRightDn	= pDeckData->GetSlabRebarVariableArr(nCycle, nJongBasePos, FALSE);	

	Dom.DimMoveTo(0, -(pDB->m_dThickSlabBase+pDB->m_dHeightTaperStt), nCol);
	if(bInput)
	{
		for(long nDim=0; nDim<arrDimCenterDn.GetSize()-1; nDim++)
		{
			if(nDim == 0 || nDim == arrDimCenterDn.GetSize()-2)
			{
				if((arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]) - (arrDimRightDn[nDim+1]-arrDimRightDn[nDim]) == 0)
					str.Format("%s", Comma(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]));
				else
					str.Format("%s~%s", Comma(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]), Comma(arrDimRightDn[nDim+1]-arrDimRightDn[nDim]));
				Dom.DimLineToExtend(arrDimCenterDn[nDim+1]-arrDimCenterDn[nDim], nCol, _T(""), str);
			}
			else
			{
				if((arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]) - (arrDimRightDn[nDim+1]-arrDimRightDn[nDim]) == 0)
					str.Format("%s", Comma(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]));
				else
					str.Format("%s~%s", Comma(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]), Comma(arrDimRightDn[nDim+1]-arrDimRightDn[nDim]));
				Dom.DimLineTo(arrDimCenterDn[nDim+1]-arrDimCenterDn[nDim], nCol, str);
			}
		}
	}
	else
	{
		for(long nDim=0; nDim<arrDimCenterDn.GetSize()-1; nDim++)
		{
			if(nDim == 0 || nDim == arrDimCenterDn.GetSize()-2)
			{
				if((arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]) - (arrDimRightDn[nDim+1]-arrDimRightDn[nDim]) == 0)
					str.Format("%s", COMMA(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]));
				else
					str.Format("%s~%s", COMMA(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]), COMMA(arrDimRightDn[nDim+1]-arrDimRightDn[nDim]));
				Dom.DimLineToExtend(arrDimCenterDn[nDim+1]-arrDimCenterDn[nDim], nCol, _T(""), str);
			}
			else
			{
				if((arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]) - (arrDimRightDn[nDim+1]-arrDimRightDn[nDim]) == 0)
					str.Format("%s", COMMA(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]));
				else
					str.Format("%s~%s", COMMA(arrDimLeftDn[nDim+1]-arrDimLeftDn[nDim]), COMMA(arrDimRightDn[nDim+1]-arrDimRightDn[nDim]));
				Dom.DimLineTo(arrDimCenterDn[nDim+1]-arrDimCenterDn[nDim], nCol, str);
			}
		}
	}
	*pDom << Dom;	*/
}

void CDrawDBBaseRebarJong::DrawMarkMainRebarAtJong(CDomyun *pDom, long nJigan, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);
	CARoadOptionStd *pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();

 	CRebar			*pRB		= NULL;
	CConcSymbol		SymDom(pOpt);
	
	CDomyun			Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);

	double dCoverUp           = pDeckData->m_dCoverUp;            
	double dCoverSd           = pDeckData->m_dCoverSd;
	double dThickPave	      = pDB->GetThickPave();
	double dSttBlockOutHeight = m_pDataMng->GetGlobalOption()->GetSttExpansionJointHeight()-dThickPave;
	double dSttBlockOutWidth  = pDeckData->GetWidthBlockOut(TRUE, nJongBasePos);
	double dEndBlockOutHeight = m_pDataMng->GetGlobalOption()->GetEndExpansionJointHeight()-dThickPave;
	double dEndBlockOutWidth  = pDeckData->GetWidthBlockOut(FALSE, nJongBasePos);
	if(pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth  = 0;
		dEndBlockOutWidth  = 0;
	}
	double dSlabTotalL		  = pDeckData->GetLengthSlabTotal(nJongBasePos);
	
	CDPoint vX(1,0), vY(0,1), xyStt, xyEnd, xyDim;
	CDPointArray xyArrPoint;
	CString sText;

	//////////////////////////////////////////////////////////////////////////
	//신축이음하면주철근 
	//////////////////////////////////////////////////////////////////////////
	//시점
	long nCount = 0;
	if(nJigan==0 && !pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		xyStt = vX*dCoverSd-vY*(dSttBlockOutHeight+dCoverUp-REBAR_SYMBOL_RADIUS);
		xyArrPoint.Add(xyStt);
		for (long n=0; n<pDeckData->m_arrCTCMainRebarUnderSinchuk.GetSize(); n++)
		{
			xyDim = pDeckData->m_arrCTCMainRebarUnderSinchuk[n];
			for (long nn=0; nn<xyDim.x; nn++)
			{
				xyStt += vX*xyDim.y;
				if(nCount<2)
				{
					xyArrPoint.Add(xyStt);
					nCount++;
				}
			}
		}
		pRB	= pDeckData->GetRebarByName("신축이음하면주철근");
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, TRUE, 0);
		xyArrPoint.RemoveAll();
	}
	
	//종점
	nCount = 0;
	if(nJigan==pDB->m_nQtyJigan-1 && !pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		xyStt = vX*(dSlabTotalL-dCoverSd) - vY*(dEndBlockOutHeight+dCoverUp-REBAR_SYMBOL_RADIUS);
		xyArrPoint.Add(xyStt);
		for (long n=0; n<pDeckData->m_arrCTCMainRebarUnderSinchuk.GetSize(); n++)
		{
			xyDim = pDeckData->m_arrCTCMainRebarUnderSinchuk[n];
			for (long nn=0; nn<xyDim.x; nn++)
			{
				xyStt -= vX*xyDim.y;
				if(nCount<2)
				{
					xyArrPoint.Add(xyStt);
					nCount++;
				}
			}
		}
		pRB	= pDeckData->GetRebarByName("신축이음하면주철근");
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, 0);
		xyArrPoint.RemoveAll();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 마크구역별 마크위치 계산 
	//////////////////////////////////////////////////////////////////////////
	CDoubleArray dArrDimUpSaboStt  = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE, -2);
	CDoubleArray dArrDimLoSaboStt  = pDeckData->GetRebarPosAtJongBasePos(FALSE, nJongBasePos, TRUE, -2);
	CDoubleArray dArrDimUpSkMaStt  = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE, -1);
	CDoubleArray dArrDimLoSkMaStt  = pDeckData->GetRebarPosAtJongBasePos(FALSE, nJongBasePos, TRUE, -1);
	CDoubleArray dArrDimUpMain     = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE, 3);
	CDoubleArray dArrDimLoMain     = pDeckData->GetRebarPosAtJongBasePos(FALSE, nJongBasePos, TRUE, 3);
	CDoubleArray dArrDimUpSkMaEnd  = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE, 1);
	CDoubleArray dArrDimLoSkMaEnd  = pDeckData->GetRebarPosAtJongBasePos(FALSE, nJongBasePos, TRUE, 1);
	CDoubleArray dArrDimUpSaboEnd  = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE, 2);
	CDoubleArray dArrDimLoSaboEnd  = pDeckData->GetRebarPosAtJongBasePos(FALSE, nJongBasePos, TRUE, 2);
	CDPointArray xyArrGuide = pDeckData->GetXyArrGuideLineForBeryukRebar(nJongBasePos, FALSE);
	CDPointArray xyArrPointUpSaboStt, xyArrPointUpSkMaStt, xyArrPointUpMain, xyArrPointUpSkMaEnd, xyArrPointUpSaboEnd;

	long n =0;
	for (n=0; n<dArrDimUpSaboStt.GetSize(); n++)		xyArrPointUpSaboStt.Add(CDPoint(dArrDimUpSaboStt[n], -dCoverUp));
	for (n=0; n<dArrDimUpSkMaStt.GetSize(); n++)		xyArrPointUpSkMaStt.Add(CDPoint(dArrDimUpSkMaStt[n], -dCoverUp));
	for (n=0; n<dArrDimUpMain.GetSize(); n++)			xyArrPointUpMain.Add(CDPoint(dArrDimUpMain[n], -dCoverUp));
	for (n=0; n<dArrDimUpSkMaEnd.GetSize(); n++)		xyArrPointUpSkMaEnd.Add(CDPoint(dArrDimUpSkMaEnd[n], -dCoverUp));
	for (n=0; n<dArrDimUpSaboEnd.GetSize(); n++)		xyArrPointUpSaboEnd.Add(CDPoint(dArrDimUpSaboEnd[n], -dCoverUp));
	CDPointArray xyArrPointLoSaboStt = pDeckData->GetCDPointArrByDoubleLine(dArrDimLoSaboStt, xyArrGuide);
	CDPointArray xyArrPointLoSkMaStt = pDeckData->GetCDPointArrByDoubleLine(dArrDimLoSkMaStt, xyArrGuide);
	CDPointArray xyArrPointLoMain    = pDeckData->GetCDPointArrByDoubleLine(dArrDimLoMain, xyArrGuide);
	CDPointArray xyArrPointLoSkMaEnd = pDeckData->GetCDPointArrByDoubleLine(dArrDimLoSkMaEnd, xyArrGuide);
	CDPointArray xyArrPointLoSaboEnd = pDeckData->GetCDPointArrByDoubleLine(dArrDimLoSaboEnd, xyArrGuide);

	//////////////////////////////////////////////////////////////////////////
	// 상면
	//////////////////////////////////////////////////////////////////////////
	
	// 사보강철근 (시점상부)
	long nCyclesu = pDeckData->m_stMainCycleSabogangStt.size();
	if(xyArrPointUpSaboStt.GetSize() > 0 && nJigan==0 && nCyclesu>0)	
	{
		xyArrPointUpSaboStt.Sort(TRUE, TRUE);
	
		long nSizeMid = long(xyArrPointUpSaboStt.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointUpSaboStt.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointUpSaboStt[n]+vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		BOOL bOneCycle  = nCyclesu==1 ? TRUE : FALSE;
		if(bOneCycle)		sText.Format("시점사보강상면1");
		else				sText.Format("시점사보강상면1-1Cycle");

		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}

	// 사각부주철근 (시점상부)
	nCyclesu = pDeckData->m_stMainCycleDan.size();
	if(xyArrPointUpSkMaStt.GetSize() > 0 && nJigan==0 && nCyclesu>0)	
	{
		xyArrPointUpSkMaStt.Sort(TRUE, TRUE);
	
		long nSizeMid = long(xyArrPointUpSkMaStt.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointUpSkMaStt.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointUpSkMaStt[n]+vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		sText.Format("시점사각주철근상면");
		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}

	// 단부주철근
	if(xyArrPointUpMain.GetSize() > 0 && (nJigan==0||nJigan==pDB->m_nQtyJigan-1))	
	{
		long nCyclesu = pDeckData->m_stMainCycleDan.size();
		if(nJigan==0)	xyArrPointUpMain.Sort(TRUE, TRUE);
		else			xyArrPointUpMain.Sort(TRUE, FALSE);	
	
		nCount = 0;
		for ( n=0; n<xyArrPointUpMain.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointUpMain[n]+vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		if(nCyclesu==1)		sText.Format("단부상면1");
		else				sText.Format("단부상면1-1Cycle");
		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}

	// 일반부상면주철근
	long nRebarDanStt = pDeckData->GetRebarSuAtDanbu(TRUE, TRUE);
	long nRebarDanEnd = pDeckData->GetRebarSuAtDanbu(TRUE, FALSE);
	nCount = 0;
	if(pDeckData->IsSameMainRebarDanGen(TRUE)==FALSE)	// 단부상면과 일반부상면의 철근이 다를 경우
	{
		long nCyclesu  = pDeckData->m_stMainCycleCen.size();
		BOOL bOneCycle = nCyclesu==1 ? TRUE : FALSE;
		if(nJigan==0)						
		{
			xyArrPointUpMain.Sort(TRUE, TRUE);
			for (long n=nRebarDanStt; n<xyArrPointUpMain.GetSize(); n+=nCyclesu)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointUpMain[n]+vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
		}
		else if(nJigan==pDB->m_nQtyJigan-1)	
		{
			xyArrPointUpMain.Sort(TRUE, FALSE);
			for (long n=nRebarDanEnd; n<xyArrPointUpMain.GetSize(); n+=nCyclesu)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointUpMain[n]+vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
		}
		else								// 중간위치에 마크표기
		{
			xyArrPointUpMain.Sort(TRUE, TRUE);
			double dPosStt = 0, dPosEnd = 0;
			CDPointArray xyArrPointJigan;
			pDeckData->GetPosJiganAtJong(nJigan, dPosStt, dPosEnd, nJongBasePos);

			for (n=0; n<xyArrPointUpMain.GetSize(); n++)
			{
				if(dPosStt < xyArrPointUpMain[n].x && xyArrPointUpMain[n].x < dPosEnd)
					xyArrPointJigan.Add(xyArrPointUpMain[n]);
			}
			long nSizeMid = long(xyArrPointJigan.GetSize()/2.2);
			for ( n=nSizeMid; n<xyArrPointUpMain.GetSize(); n+=2)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointJigan[n]+vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}		
		}

		if(bOneCycle)		sText.Format("일반부상면1");
		else				sText.Format("일반부상면1-1Cycle");

		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, nJigan==pDB->m_nQtyJigan-1?TRUE:FALSE, 0);
		xyArrPoint.RemoveAll();
	}
	
	if(pDeckData->IsSameMainRebarDanGen(TRUE) && (nJigan!=0 && nJigan!=pDB->m_nQtyJigan-1))	// 단부상면과 일반부상면의 철근 동일시
	{
		xyArrPointUpMain.Sort(TRUE, TRUE);
		double dPosStt = 0, dPosEnd = 0;
		CDPointArray xyArrPointJigan;
		pDeckData->GetPosJiganAtJong(nJigan, dPosStt, dPosEnd, nJongBasePos);

		for (n=0; n<xyArrPointUpMain.GetSize(); n++)
		{
			if(dPosStt < xyArrPointUpMain[n].x && xyArrPointUpMain[n].x < dPosEnd)
				xyArrPointJigan.Add(xyArrPointUpMain[n]+vY*REBAR_SYMBOL_RADIUS);
		}
		long nSizeMid = long(xyArrPointJigan.GetSize()/2.2);
		for ( n=nSizeMid; n<xyArrPointUpMain.GetSize(); n+=2)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointJigan[n]+vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}		

		if(pDeckData->m_stMainCycleDan.size()==1)		sText.Format("단부상면1");
		else											sText.Format("단부상면1-1Cycle");
		
		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, nJigan==pDB->m_nQtyJigan-1?TRUE:FALSE, 0);
		xyArrPoint.RemoveAll();
	}
	
	// 사각부주철근 (종점상면)
	nCyclesu = pDeckData->m_stMainCycleDan.size();
	if(xyArrPointUpSkMaEnd.GetSize() > 0 && nJigan==pDB->m_nQtyJigan-1 && nCyclesu>0)	
	{
		xyArrPointUpSkMaEnd.Sort(TRUE, FALSE);
	
		long nSizeMid = long(xyArrPointUpSkMaEnd.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointUpSkMaEnd.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointUpSkMaEnd[n]+vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		sText.Format("종점사각주철근상면");
		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}
	
	// 사보강철근 (종점상면)
	nCyclesu = pDeckData->m_stMainCycleSabogangStt.size();
	if(xyArrPointUpSaboEnd.GetSize() > 0 && nJigan==pDB->m_nQtyJigan-1 && nCyclesu>0)	
	{
		xyArrPointUpSaboEnd.Sort(TRUE, FALSE);
	
		long nSizeMid = long(xyArrPointUpSaboEnd.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointUpSaboEnd.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointUpSaboEnd[n]+vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		BOOL bOneCycle  = nCyclesu==1 ? TRUE : FALSE;
		if(bOneCycle)		sText.Format("종점사보강상면1");
		else				sText.Format("종점사보강상면1-1Cycle");

		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}

	//////////////////////////////////////////////////////////////////////////
	// 하면
	//////////////////////////////////////////////////////////////////////////
	
	// 사보강철근 (시점하부)
	nCyclesu = pDeckData->m_stMainCycleSabogangStt.size();
	if(xyArrPointLoSaboStt.GetSize() > 0 && nJigan==0 && nCyclesu>0)	
	{
		xyArrPointLoSaboStt.Sort(TRUE, TRUE);
	
		long nSizeMid = long(xyArrPointLoSaboStt.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointLoSaboStt.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointLoSaboStt[n]-vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		BOOL bOneCycle  = nCyclesu==1 ? TRUE : FALSE;
		if(bOneCycle)		sText.Format("시점사보강하면1");
		else				sText.Format("시점사보강하면1-1Cycle");

		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}

	// 사각부주철근 (시점상부)
	nCyclesu = pDeckData->m_stMainCycleDan.size();
	if(xyArrPointLoSkMaStt.GetSize() > 0 && nJigan==0 && nCyclesu>0)	
	{
		xyArrPointLoSkMaStt.Sort(TRUE, TRUE);
	
		long nSizeMid = long(xyArrPointLoSkMaStt.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointLoSkMaStt.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointLoSkMaStt[n]-vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		sText.Format("시점사각주철근하면");
		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}

	// 단부주철근
	nCyclesu = pDeckData->m_stMainCycleDan.size();
	if(xyArrPointLoMain.GetSize() > 0 && (nJigan==0||nJigan==pDB->m_nQtyJigan-1) && nCyclesu>0 )	
	{
		if(nJigan==0)	xyArrPointLoMain.Sort(TRUE, TRUE);
		else			xyArrPointLoMain.Sort(TRUE, FALSE);	
	
		nCount = 0;
		for ( n=0; n<xyArrPointLoMain.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointLoMain[n]-vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		if(nCyclesu==1)		sText.Format("단부하면1");
		else				sText.Format("단부하면1-1Cycle");
		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, nJigan==0?TRUE:FALSE, 0);
		xyArrPoint.RemoveAll();
	}

	// 일반부하면주철근
	nRebarDanStt = pDeckData->GetRebarSuAtDanbu(TRUE, TRUE);
	nRebarDanEnd = pDeckData->GetRebarSuAtDanbu(TRUE, FALSE);
	nCount = 0;

	BOOL bLeftBrk = pDeckData->IsBridgeHasOutBracket(SECTION_CEN, TRUE);
	BOOL bRighBrk = pDeckData->IsBridgeHasOutBracket(SECTION_CEN, FALSE);
	BOOL bNoBrk   = !bLeftBrk&&!bRighBrk ? TRUE : FALSE;
	BOOL bNoHunch = pDeckData->IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&pDB->GetGirdersu()==1) ? TRUE : FALSE;
	BOOL bTaperDiv= pDB->m_dHeightTaperStt>0 || (pDB->m_dHeightTaperStt==0&&!pDeckData->IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;

	if(bNoHunch==FALSE)
	{
		long nCyclesu = pDeckData->m_stMainCycleCen.size();
		BOOL bOneCycle = nCyclesu==1 ? TRUE : FALSE;
		if(nJigan==0)						// 시작단부주철근 끝나고 3개 지난후 마크
		{
			xyArrPointLoMain.Sort(TRUE, TRUE);
			for (long n=nRebarDanStt+3; n<xyArrPointLoMain.GetSize(); n+=nCyclesu)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointLoMain[n]-vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
		}
		else if(nJigan==pDB->m_nQtyJigan-1)	// 종점단부주철근 끝나고 3개 지난후 마크
		{
			xyArrPointLoMain.Sort(TRUE, FALSE);
			for (long n=nRebarDanEnd+3; n<xyArrPointLoMain.GetSize(); n+=nCyclesu)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointLoMain[n]-vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
		}
		else								// 중간위치에 마크표기
		{
			xyArrPointLoMain.Sort(TRUE, TRUE);
			double dPosStt = 0, dPosEnd = 0;
			CDPointArray xyArrPointJigan;
			pDeckData->GetPosJiganAtJong(nJigan, dPosStt, dPosEnd, nJongBasePos);

			for (n=0; n<xyArrPointLoMain.GetSize(); n++)
			{
				if(dPosStt < xyArrPointLoMain[n].x && xyArrPointLoMain[n].x < dPosEnd)
					xyArrPointJigan.Add(xyArrPointLoMain[n]);
			}
			long nSizeMid = long(xyArrPointJigan.GetSize()/2.2);
			for ( n=nSizeMid; n<xyArrPointLoMain.GetSize(); n+=2)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointJigan[n]-vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}		
		}

		if(nJongBasePos==JONGBASEPOS_SLABLEFT)		sText.Format("일반부외측거더하면좌측");
		else										sText.Format("일반부외측거더하면우측");

		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, nJigan==pDB->m_nQtyJigan-1?TRUE:FALSE, 0);
		xyArrPoint.RemoveAll();
	}
	else if(bNoHunch && bTaperDiv)	// 단부상면과 일반부상면의 철근이 다를 경우
	{
		long nCyclesu = pDeckData->m_stMainCycleCen.size();
		BOOL bOneCycle = nCyclesu==1 ? TRUE : FALSE;
		if(nJigan==0)						// 시작단부주철근 끝나고 3개 지난후 마크
		{
			xyArrPointLoMain.Sort(TRUE, TRUE);
			for (long n=nRebarDanStt+3; n<xyArrPointLoMain.GetSize(); n+=nCyclesu)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointLoMain[n]-vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
		}
		else if(nJigan==pDB->m_nQtyJigan-1)	// 종점단부주철근 끝나고 3개 지난후 마크
		{
			xyArrPointLoMain.Sort(TRUE, FALSE);
			for (long n=nRebarDanEnd+3; n<xyArrPointLoMain.GetSize(); n+=nCyclesu)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointLoMain[n]-vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
		}
		else								// 중간위치에 마크표기
		{
			xyArrPointLoMain.Sort(TRUE, TRUE);
			double dPosStt = 0, dPosEnd = 0;
			CDPointArray xyArrPointJigan;
			pDeckData->GetPosJiganAtJong(nJigan, dPosStt, dPosEnd, nJongBasePos);

			for (n=0; n<xyArrPointLoMain.GetSize(); n++)
			{
				if(dPosStt < xyArrPointLoMain[n].x && xyArrPointLoMain[n].x < dPosEnd)
					xyArrPointJigan.Add(xyArrPointLoMain[n]-vY*REBAR_SYMBOL_RADIUS);
			}
			long nSizeMid = long(xyArrPointJigan.GetSize()/2.2);
			for ( n=nSizeMid; n<xyArrPointLoMain.GetSize(); n+=2)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrPointJigan[n]-vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}		
		}

		if(bOneCycle)		sText.Format("일반부하면1");
		else				sText.Format("일반부하면1-1");

		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, nJigan==pDB->m_nQtyJigan-1?TRUE:FALSE, 0);
		xyArrPoint.RemoveAll();
	}
	
	// 사각부주철근 (종점하부)
	nCyclesu = pDeckData->m_stMainCycleDan.size();
	if(xyArrPointLoSkMaEnd.GetSize() > 0 && nJigan==pDB->m_nQtyJigan-1 && nCyclesu>0)	
	{
		xyArrPointLoSkMaEnd.Sort(TRUE, FALSE);
	
		long nSizeMid = long(xyArrPointLoSkMaEnd.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointLoSkMaEnd.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointLoSkMaEnd[n]-vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		sText.Format("종점사각주철근상면");
		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();
	}
	
	// 사보강철근 (종점하부)
	nCyclesu = pDeckData->m_stMainCycleSabogangStt.size();
	if(xyArrPointLoSaboEnd.GetSize() > 0 && nJigan==pDB->m_nQtyJigan-1 && nCyclesu >0)	
	{
		xyArrPointLoSaboEnd.Sort(TRUE, FALSE);
	
		long nSizeMid = long(xyArrPointLoSaboEnd.GetSize()/2);
		nCount = 0;
		for ( n=nSizeMid; n<xyArrPointLoSaboEnd.GetSize(); n+=nCyclesu)
		{
			if(nCount<3)
			{
				xyArrPoint.Add(xyArrPointLoSaboEnd[n]-vY*REBAR_SYMBOL_RADIUS);
				nCount++;
			}
		}

		BOOL bOneCycle  = nCyclesu==1 ? TRUE : FALSE;
		if(bOneCycle)		sText.Format("종점사보강상면1");
		else				sText.Format("종점사보강상면1-1Cycle");

		pRB	= pDeckData->GetRebarByName(sText);
		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, nJigan==0?FALSE:TRUE, 0);
		xyArrPoint.RemoveAll();

	}
	// 단부 헌치 정착장 주철근//////////////////////////////////////////////////////////////// 
	if (pDeckData->m_bInstallDanbuTaperMainRebar)
	{
		CDPointArray xyArrStt = pDeckData->GetXyArrRebarJongPosAtDanHunch(TRUE, nJongBasePos);
		CDPointArray xyArrEnd = pDeckData->GetXyArrRebarJongPosAtDanHunch(FALSE, nJongBasePos);
		if(nJigan==0)
		{
			nCount = 0;
			for(long n=0; n<xyArrStt.GetSize(); n++)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrStt[n]+vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
			
			pRB	= pDeckData->GetRebarByName("단부헌치부주철근");
			if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, 0);
			xyArrPoint.RemoveAll();
		}
		else if(nJigan==pDB->m_nQtyJigan-1)
		{
			nCount = 0;
			for(long n=0; n<xyArrEnd.GetSize(); n++)
			{
				if(nCount<3)
				{
					xyArrPoint.Add(xyArrEnd[n]+vY*REBAR_SYMBOL_RADIUS);
					nCount++;
				}
			}
			
			pRB	= pDeckData->GetRebarByName("단부헌치부주철근");
			if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, 0);
			xyArrPoint.RemoveAll();
		}
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarJong::DrawMarkBeryukRebarAtJongTotal(CDomyun *pDom, long nCycle, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();	
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
	CRebar			*pRB		= NULL;
	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);

	if(pDeckData->m_arrRebarJewon.GetSize()==0)
		return;

	pOpt->SetEnvType(&Dom, HCAD_DIML);
	CDPointArray xyArr;
	CDPoint	xyMark(0,0);
	CString	sText			= _T("");
	BOOL	bOneCycle		= pDeckData->m_stBeryukCycleLeft.size()==1 ? TRUE : FALSE;

	vector <REBAR_SHAPE>* pvShape = pDeckData->GetRebarShapeArrByJongBasePos(nJongBasePos, nCycle);
	if(pvShape->size() < 1)		return;

	// 시점부 상부
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		if(bOneCycle)	sText.Format("배력단부상면L");
		else			sText.Format("배력단부상면L%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		if(bOneCycle)	sText.Format("배력단부상면R");
		else			sText.Format("배력단부상면R%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		if(bOneCycle)	sText.Format("배력단부상면");
		else			sText.Format("배력단부상면%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_ONCB)
	{
		if(bOneCycle)	sText.Format("배력단부상면L");
		else			sText.Format("배력단부상면L%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	
	if(pRB==NULL)
	{
		if(bOneCycle)	sText.Format("배력단부상면");
		else			sText.Format("배력단부상면%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}

	xyArr	= pDeckData->GetXyArrPoint("시점부상부", *pvShape);
	if(xyArr.GetSize() > 2)
	{
		xyMark	= xyArr[1]+(xyArr[2]-xyArr[1]).Unit()*~(xyArr[2]-xyArr[1])*0.7;
		
		if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}
	// 종점부 상부
	xyArr	= pDeckData->GetXyArrPoint("종점부상부", *pvShape);
	if(xyArr.GetSize() > 2)
	{
		xyMark	= xyArr[1]+(xyArr[2]-xyArr[1]).Unit()*~(xyArr[2]-xyArr[1])*0.7;
		if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}
	long n = 0;
	// 일반부 상부
	for(n=0; n<pDB->m_nQtyJigan; n++)
	{
		if(nJongBasePos==JONGBASEPOS_SLABLEFT)
		{
			if(bOneCycle)	sText.Format("배력일반부상면L%d", n+1);
			else			sText.Format("배력일반부상면L%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
		{
			if(bOneCycle)	sText.Format("배력일반부상면R%d", n+1);
			else			sText.Format("배력일반부상면R%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
		{
			if(bOneCycle)	sText.Format("배력일반부상면%d", n+1);
			else			sText.Format("배력일반부상면%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_ONCB)
		{
			if(bOneCycle)	sText.Format("배력일반부상면L%d", n+1);
			else			sText.Format("배력일반부상면L%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		
		if(pRB==NULL)
		{
			if(bOneCycle)	sText.Format("배력일반부상면%d", n+1);
			else			sText.Format("배력일반부상면%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}

		pRB		= pDeckData->GetRebarByName(sText);
		sText.Format("일반부상부(%d)", n+1);
		xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
		if(xyArr.GetSize() > 1)
		{
			xyMark	= (xyArr[0]+xyArr[1])/2;
			if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}
	// 지점부 상부
	for(n=0; n<pDB->m_nQtyJigan-1;n++)
	{
		if(bOneCycle)	sText.Format("배력지점부상면%d", n+1);
		else			sText.Format("배력지점부상면%d-%dCycle", n+1, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
		sText.Format("지점부상부(%d)", n+1);
		xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
		if(xyArr.GetSize() > 2)
		{
			xyMark	= xyArr[0] + (xyArr[2]-xyArr[0]).Unit()*~(xyArr[2]-xyArr[0])*0.4;
			if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}
	// 시점부 하부  1
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		sText.Format("배력단부하면L1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		sText.Format("배력단부하면R1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		sText.Format("배력단부하면1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_ONCB)
	{
		sText.Format("배력단부하면L1");
		pRB	= pDeckData->GetRebarByName(sText);
	}

	if(pRB==NULL)
	{
		sText.Format("배력단부하면1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	xyArr	= pDeckData->GetXyArrPoint(_T("시점부하부(1)"), *pvShape);
	if(xyArr.GetSize() > 3)
	{
		if(pDeckData->m_bInstallBeryukRebarAtBlockOut)
			xyMark	= (xyArr[1]+xyArr[2])/2;
		else
			xyMark	= (xyArr[2]+xyArr[3])/2;
		if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}
	// 종점부 하부  1
	xyArr	= pDeckData->GetXyArrPoint(_T("종점부하부(1)"), *pvShape);
	if(xyArr.GetSize() > 3)
	{
		if(pDeckData->m_bInstallBeryukRebarAtBlockOut)
			xyMark	= (xyArr[1]+xyArr[2])/2;
		else
			xyMark	= (xyArr[2]+xyArr[3])/2;
		if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}
	// 시점부 하부  2
	if(pDeckData->IsInstallBeryukRebarAtDanHunch(nJongBasePos, TRUE, nCycle))
	{
		if(nJongBasePos==JONGBASEPOS_SLABLEFT)
		{
			if(bOneCycle)	sText.Format("배력단부하면2L");
			else			sText.Format("배력단부하면2L-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
		{
			if(bOneCycle)	sText.Format("배력단부하면2R");
			else			sText.Format("배력단부하면2R-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
		{
			if(bOneCycle)	sText.Format("배력단부하면2");
			else			sText.Format("배력단부하면2-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_ONCB)
		{
			if(bOneCycle)	sText.Format("배력단부하면2L");
			else			sText.Format("배력단부하면2L-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}

		if(pRB==NULL)
		{
			if(bOneCycle)	sText.Format("배력단부하면2");
			else			sText.Format("배력단부하면2-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		xyArr	= pDeckData->GetXyArrPoint(_T("시점부하부(2)"), *pvShape);
		if(xyArr.GetSize() > 1)
		{
			xyMark	= (xyArr[0]+xyArr[1])/2;
			if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}

	// 종점부 하부  2
	if(pDeckData->IsInstallBeryukRebarAtDanHunch(nJongBasePos, FALSE, nCycle))
	{
		xyArr	= pDeckData->GetXyArrPoint(_T("종점부하부(2)"), *pvShape);
		if(xyArr.GetSize() > 1)
		{
			xyMark	= (xyArr[0]+xyArr[1])/2;
			if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}

	// 일반부 하부
	for(n=0; n<pDB->m_nQtyJigan;n++)
	{
		if(nJongBasePos==JONGBASEPOS_SLABLEFT)
		{
			if(bOneCycle)	sText.Format("배력일반부하면L%d", n+1);
			else			sText.Format("배력일반부하면L%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
		{
			if(bOneCycle)	sText.Format("배력일반부하면R%d", n+1);
			else			sText.Format("배력일반부하면R%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
		{
			if(bOneCycle)	sText.Format("배력일반부하면%d", n+1);
			else			sText.Format("배력일반부하면%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_ONCB)
		{
			if(bOneCycle)	sText.Format("배력일반부하면L%d", n+1);
			else			sText.Format("배력일반부하면L%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}

		if(pRB==NULL)
		{
			if(bOneCycle)	sText.Format("배력일반부하면%d", n+1);
			else			sText.Format("배력일반부하면%d-%dCycle", n+1, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		sText.Format("일반부하부(%d)", n+1);
		xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
		if(xyArr.GetSize() > 1)
		{
			xyMark	= (xyArr[0]+xyArr[1])/2;
			if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}

	// 지점부 하부
	for(n=0; n<pDB->m_nQtyJigan-1;n++)
	{
		if(bOneCycle)	sText.Format("배력지점부하면%d", n+1);
		else			sText.Format("배력지점부하면%d-%dCycle", n+1, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
		sText.Format("지점부하부(%d)", n+1);
		xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
		if(xyArr.GetSize() > 2)
		{
			xyMark	= xyArr[0] + (xyArr[2]-xyArr[0]).Unit()*~(xyArr[2]-xyArr[0])*0.4;
			if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}

	*pDom << Dom;
}



void CDrawDBBaseRebarJong::DrawMarkBeryukRebarAtJong(CDomyun *pDom, long nCycle, long nJigan, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();	
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
	CRebar			*pRB		= NULL;
	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);

	if(pDeckData->m_arrRebarJewon.GetSize()==0)
		return;

	pOpt->SetEnvType(&Dom, HCAD_DIML);
	CDPointArray xyArr;
	CDPoint	xyMark(0,0);
	CString	sText			= _T("");
	BOOL	bOneCycle		= pDeckData->m_stBeryukCycleLeft.size()==1 ? TRUE : FALSE;
	vector <REBAR_SHAPE>* pvShape = pDeckData->GetRebarShapeArrByJongBasePos(nJongBasePos, nCycle);
	if(pvShape->size() < 1)		return;

	// 시점부 상부
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		if(bOneCycle)	sText.Format("배력단부상면L");
		else			sText.Format("배력단부상면L%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		if(bOneCycle)	sText.Format("배력단부상면R");
		else			sText.Format("배력단부상면R%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		if(bOneCycle)	sText.Format("배력단부상면");
		else			sText.Format("배력단부상면%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	if(pRB==NULL)
	{
		if(bOneCycle)	sText.Format("배력단부상면");
		else			sText.Format("배력단부상면%dCycle", nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}

	xyArr	= pDeckData->GetXyArrPoint("시점부상부", *pvShape);
	if(xyArr.GetSize() > 2)
	{
		xyMark	= (xyArr[1]+xyArr[2])/2;
		if(pRB && nJigan==0)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}

	// 종점부 상부
	xyArr	= pDeckData->GetXyArrPoint("종점부상부", *pvShape);
	if(xyArr.GetSize() > 2)
	{
		xyMark	= (xyArr[1]+xyArr[2])/2;
		if(pRB && nJigan==pDB->m_nQtyJigan-1)		SymDom.RebarMarkLineUpper(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), (BOOL)pRB->GetLength(), 0, FALSE);
	}

	// 일반부 상부
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		if(bOneCycle)	sText.Format("배력일반부상면L%d", nJigan+1);
		else			sText.Format("배력일반부상면L%d-%dCycle", nJigan+1, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		if(bOneCycle)	sText.Format("배력일반부상면R%d", nJigan+1);
		else			sText.Format("배력일반부상면R%d-%dCycle", nJigan+1, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		if(bOneCycle)	sText.Format("배력일반부상면%d", nJigan+1);
		else			sText.Format("배력일반부상면%d-%dCycle", nJigan+1, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	if(pRB==NULL)
	{
		if(bOneCycle)	sText.Format("배력일반부상면%d", nJigan+1);
		else			sText.Format("배력일반부상면%d-%dCycle", nJigan+1, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
	}
	sText.Format("일반부상부(%d)", nJigan+1);
	xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
	if(xyArr.GetSize() > 1)
	{
		xyMark	= (xyArr[0]+xyArr[1])/2;
		if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}
	// 지점부 상부
	if (nJigan==0 || nJigan==pDB->m_nQtyJigan-1)
	{
		if(bOneCycle)	sText.Format("배력지점부상면%d", nJigan==0 ? nJigan+1 : nJigan);
		else			sText.Format("배력지점부상면%d-%dCycle", nJigan==0 ? nJigan+1 : nJigan, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
		sText.Format("지점부상부(%d)", nJigan==0 ? nJigan+1 : nJigan);
		xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
		if(xyArr.GetSize() > 2)
		{
			if(nJigan==0)
				xyMark	= xyArr[0] + (xyArr[1]-xyArr[0]).Unit()*~(xyArr[1]-xyArr[0])*0.7;
			else
				xyMark	= xyArr[2] + (xyArr[1]-xyArr[2]).Unit()*~(xyArr[1]-xyArr[2])*0.7;
			if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}
	else
	{
		for (long nSttEnd=0; nSttEnd<2; nSttEnd++)
		{
			if(bOneCycle)	sText.Format("배력지점부상면%d", nJigan+nSttEnd);
			else			sText.Format("배력지점부상면%d-%dCycle", nJigan+nSttEnd, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
			sText.Format("지점부상부(%d)", nJigan+nSttEnd);
			xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
			if(xyArr.GetSize() > 2)
			{
				if(nSttEnd==0)
					xyMark	= xyArr[2] + (xyArr[1]-xyArr[2]).Unit()*~(xyArr[1]-xyArr[2])*0.7;
				else
					xyMark	= xyArr[0] + (xyArr[1]-xyArr[0]).Unit()*~(xyArr[1]-xyArr[0])*0.7;
				if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}		
		}
	}

	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		sText.Format("배력단부하면L1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		sText.Format("배력단부하면R1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		sText.Format("배력단부하면1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	if(pRB==NULL)
	{
		sText.Format("배력단부하면1");
		pRB	= pDeckData->GetRebarByName(sText);
	}
	xyArr	= pDeckData->GetXyArrPoint(_T("시점부하부(1)"), *pvShape);
	if(xyArr.GetSize() > 3)
	{
		if(pDeckData->m_bInstallBeryukRebarAtBlockOut)
			xyMark	= (xyArr[1]+xyArr[2])/2;
		else
			xyMark	= (xyArr[2]+xyArr[3])/2;
		if(pRB && nJigan==0)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}

	// 종점부 하부  1
	xyArr	= pDeckData->GetXyArrPoint(_T("종점부하부(1)"), *pvShape);
	if(xyArr.GetSize() > 3)
	{
		if(pDeckData->m_bInstallBeryukRebarAtBlockOut)
			xyMark	= (xyArr[1]+xyArr[2])/2;
		else
			xyMark	= (xyArr[2]+xyArr[3])/2;
		if(pRB && nJigan==pDB->m_nQtyJigan-1)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}
	// 시점부 하부  2
	if(pDeckData->IsInstallBeryukRebarAtDanHunch(nJongBasePos, TRUE, nCycle))
	{
		if(nJongBasePos==JONGBASEPOS_SLABLEFT)
		{
			if(bOneCycle)	sText.Format("배력단부하면2L");
			else			sText.Format("배력단부하면2L-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
		{
			if(bOneCycle)	sText.Format("배력단부하면2R");
			else			sText.Format("배력단부하면2R-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
		{
			if(bOneCycle)	sText.Format("배력단부하면2");
			else			sText.Format("배력단부하면2-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		if(pRB==NULL)
		{
			if(bOneCycle)	sText.Format("배력단부하면2");
			else			sText.Format("배력단부하면2-%dCycle", nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
		}
		
		xyArr	= pDeckData->GetXyArrPoint(_T("시점부하부(2)"), *pvShape);
		if(xyArr.GetSize() > 1)
		{
			xyMark	= (xyArr[0]+xyArr[1])/2;
			if(pRB && nJigan==0)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}

	// 종점부 하부  2
	if(pDeckData->IsInstallBeryukRebarAtDanHunch(nJongBasePos, FALSE, nCycle))
	{
		xyArr	= pDeckData->GetXyArrPoint(_T("종점부하부(2)"), *pvShape);
		if(xyArr.GetSize() > 1)
		{
			xyMark	= (xyArr[0]+xyArr[1])/2;
			if(pRB && nJigan==pDB->m_nQtyJigan-1)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}

	// 일반부 하부
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		if(bOneCycle)	sText.Format("배력일반부하면L%d", nJigan+1);
		else			sText.Format("배력일반부하면L%d-%dCycle", nJigan+1, nCycle+1);
	}
	if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		if(bOneCycle)	sText.Format("배력일반부하면R%d", nJigan+1);
		else			sText.Format("배력일반부하면R%d-%dCycle", nJigan+1, nCycle+1);
	}
	if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		if(bOneCycle)	sText.Format("배력일반부하면%d", nJigan+1);
		else			sText.Format("배력일반부하면%d-%dCycle", nJigan+1, nCycle+1);
	}
	if(pRB==NULL)
	{
		if(bOneCycle)	sText.Format("배력일반부하면%d", nJigan+1);
		else			sText.Format("배력일반부하면%d-%dCycle", nJigan+1, nCycle+1);
	}
	pRB		= pDeckData->GetRebarByName(sText);
	sText.Format("일반부하부(%d)", nJigan+1);
	xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
	if(xyArr.GetSize() > 1)
	{
		xyMark	= (xyArr[0]+xyArr[1])/2;
		if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
	}

	// 지점부 하부
	if (nJigan==0 || nJigan==pDB->m_nQtyJigan-1)
	{
		if(bOneCycle)	sText.Format("배력지점부하면%d", nJigan==0 ? nJigan+1 : nJigan);
		else			sText.Format("배력지점부하면%d-%dCycle", nJigan==0 ? nJigan+1 : nJigan, nCycle+1);
		pRB		= pDeckData->GetRebarByName(sText);
		sText.Format("지점부하부(%d)", nJigan==0 ? nJigan+1 : nJigan);
		xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
		if(xyArr.GetSize() > 2)
		{
			if(nJigan==0)
				xyMark	= xyArr[0] + (xyArr[1]-xyArr[0]).Unit()*~(xyArr[1]-xyArr[0])*0.7;
			else
				xyMark	= xyArr[2] + (xyArr[1]-xyArr[2]).Unit()*~(xyArr[1]-xyArr[2])*0.7;
			if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}
	else
	{
		for (long nSttEnd=0; nSttEnd<2; nSttEnd++)
		{
			if(bOneCycle)	sText.Format("배력지점부하면%d", nJigan+nSttEnd);
			else			sText.Format("배력지점부하면%d-%dCycle", nJigan+nSttEnd, nCycle+1);
			pRB		= pDeckData->GetRebarByName(sText);
			sText.Format("지점부하부(%d)", nJigan+nSttEnd);
			xyArr	= pDeckData->GetXyArrPoint(sText, *pvShape);
			if(xyArr.GetSize() > 2)
			{
				if(nSttEnd==0)
					xyMark	= xyArr[2] + (xyArr[1]-xyArr[2]).Unit()*~(xyArr[1]-xyArr[2])*0.7;
				else
					xyMark	= xyArr[0] + (xyArr[1]-xyArr[0]).Unit()*~(xyArr[1]-xyArr[0])*0.7;
				if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}		
		}
	}


	*pDom << Dom;
}


void CDrawDBBaseRebarJong::DrawMarkShearRebar(CDomyun *pDom, long nJigan, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();	
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGirCen	= pDB->GetGirder(-1);
	CRebar			*pRB		= NULL;

	CDrawDBDeck DeckDraw(m_pDataMng);
	CDrawDBBaseRebarPlan RebarPlan(m_pDataMng);

	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);

	Dom.SetDirection("TOP");
	
	pOpt->SetEnvType(&Dom, HCAD_DIML);

	if(pDeckData->m_arrRebarJewon.GetSize()==0)
		return;

	CDoubleArray dArrDimUpMain = pDeckData->GetRebarPosAtJongBasePos(TRUE, nJongBasePos, TRUE, 0);
	if(pDB->m_nQtyJigan==1)
		dArrDimUpMain.Sort(TRUE);
	else
	{
		if(nJigan==pDB->m_nQtyJigan-1)
			dArrDimUpMain.Sort(FALSE);
		else
			dArrDimUpMain.Sort(TRUE);
	}
	
	CDPointArray xyArrGuide = pDeckData->GetXyArrGuideLineForBeryukRebar(nJongBasePos, FALSE);
	CDPointArray xyArrPointUp;
	long n = 0;
	for (n=0; n<dArrDimUpMain.GetSize(); n++)	
		xyArrPointUp.Add(CDPoint(dArrDimUpMain[n], -pDeckData->m_dCoverUp));
	CDPointArray xyArrPointLo = pDeckData->GetCDPointArrByDoubleLine(dArrDimUpMain, xyArrGuide);

	long nCount = long(pDeckData->m_SpacerRebar.GetCntMainRebarSet()/2);
	if(nCount==0)	return;

	CDPointArray xyArrMark;
	long nSum = 0;
	
	if(nJigan==0 || nJigan==pDB->m_nQtyJigan-1)
	{
		for(n=0; n<xyArrPointUp.GetSize(); n+=nCount)
		{
			if(nSum < 3)
			{
				xyArrMark.Add(xyArrPointUp[n]+CDPoint(0,-1)*(100/*pDB->m_dThickSlabBase*/));
				nSum++;
			}
		}
		pRB = pDeckData->GetRebarByName("간격재");
		
		if(pRB)
		{
			RebarPlan.DrawRebarMarkLine(&Dom, xyArrMark);
			RebarPlan.DrawRebarMark(&Dom, xyArrMark, pRB->m_sMark, pRB->GetStringDia(), nJigan==0?2:3, TRUE, 350, xyArrMark.GetUpperBound(), nJigan==0?TRUE:FALSE);
		}		
	}
	else 
	{
		double dPosStt = 0, dPosEnd = 0;
		CDPointArray xyArrPointJigan;
		pDeckData->GetPosJiganAtJong(nJigan, dPosStt, dPosEnd, nJongBasePos);

		for (n=0; n<xyArrPointUp.GetSize(); n+=nCount)
		{
			if(dPosStt < xyArrPointUp[n].x && xyArrPointUp[n].x < dPosEnd)
				xyArrPointJigan.Add(xyArrPointUp[n]);
		}
		long nSizeMid = long(xyArrPointJigan.GetSize()/3);
		nSum = 0;
		for (n=nSizeMid; n<xyArrPointJigan.GetSize(); n++)
		{
			if(nSum < 3)
			{
				xyArrMark.Add(xyArrPointJigan[n]+CDPoint(0,-1)*(100/*pDB->m_dThickSlabBase*/));
				nSum++;
			}
		}		
		pRB = pDeckData->GetRebarByName("간격재");

		if(pRB)
		{
			RebarPlan.DrawRebarMarkLine(&Dom, xyArrMark);
			RebarPlan.DrawRebarMark(&Dom, xyArrMark, pRB->m_sMark, pRB->GetStringDia(), 5, FALSE, Dom.Always(15), -1, TRUE);
		}		
	}

	
	*pDom << Dom;
}
