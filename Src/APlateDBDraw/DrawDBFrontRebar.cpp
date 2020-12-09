// DrawDBFrontRebar.cpp: implementation of the CDrawDBFrontRebar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDBDraw.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define REBAR_MAIN_LAPJOINT_GAP 15
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawDBFrontRebar::CDrawDBFrontRebar(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CDrawDBFrontRebar::~CDrawDBFrontRebar()
{

}


void CDrawDBFrontRebar::DimMainRebarUp(CDomyun *pDom, long nSection, double dTextRatio, BOOL bDimRangeGir, BOOL bDimRangeCant)
{
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();	
	CPlateBridgeApp		*pDB   = m_pDataMng->GetBridge();
	CPlateGirderApp		*pGirL = pDB->GetGirder(0);
	CPlateGirderApp		*pGirR = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex		*pBx   = pDeckData->GetBxBySection(nSection);

	CString str1,str2;
	double dSpace = pDeckData->m_dCoverSd;
	double dWidth = pDB->GetWidthSlabDom(pBx);
	double disSlabLeft = pDB->GetDisSlabLeftDom(pBx);
	double disSlabRigh = pDB->GetDisSlabRightDom(pBx);
	double WallHeight  = 1500;
	double	SlabMaxY   = max(pDB->GetHeightSlabFromRoadCenterDom(pBx,pDB->GetDisSlabLeftDom(pBx)),
			                 pDB->GetHeightSlabFromRoadCenterDom(pBx,pDB->GetDisSlabRightDom(pBx)))
						     + WallHeight + pDB->m_dThickPave;
	long nDan = 0;

	CDomyun Dom(pDom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	if ( (bDimRangeGir==TRUE || bDimRangeCant==TRUE) && dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);

	Dom.SetDirection("TOP");

	Dom.DimMoveTo(disSlabLeft, SlabMaxY, nDan, COMMA(dSpace));
	Dom.DimLineTo(dSpace, nDan, "");	
	CDPointArray *pPtArr = pDeckData->GetCTCBeryukRebar(nSection, TRUE);
	// ����Ⱦ�ܱ���
	for(long n=0; n<pPtArr->GetSize(); n++)
	{
		CDPoint ptVal = pPtArr->GetAt(n);
		CString szVal, szVal2;
		if(ptVal.x>1)
		{
			szVal.Format("%.0f@%s", ptVal.x, COMMA(ptVal.y));
			szVal2.Format("=%s", COMMA(ptVal.x*ptVal.y));
		}	
		else
			szVal = COMMA(ptVal.y);

		Dom.DimLineToExtend(ptVal.x*ptVal.y,nDan, szVal, szVal2);
	}
	Dom.DimLineToOver(dSpace, nDan, COMMA(dSpace));

	nDan = 1;
	Dom.DimMoveTo(disSlabLeft, SlabMaxY,nDan);
	Dom.DimLineTo(dWidth,nDan,COMMA(dWidth));

	SlabMaxY -= WallHeight;
	nDan = 0;

	CDPoint CLeft[4], CRigh[4];
	pDB->GetXyFrontCantilever(pBx, CLeft, TRUE);
	pDB->GetXyFrontCantilever(pBx, CRigh, FALSE);
	
	double dPos1 = disSlabLeft;
	double dPos2 = dPos1 + (CLeft[3].x-CLeft[0].x) + pDeckData->m_dRangeDistribRebarOnCant;
	double dPos3 = disSlabRigh - (CRigh[0].x-CRigh[3].x) - pDeckData->m_dRangeDistribRebarOnCant;
	double dPos4 = disSlabRigh;

	if(bDimRangeCant)
	{
		
		Dom.DimMoveTo(dPos1, SlabMaxY, nDan);
		Dom.DimLineToExtend(dPos2-dPos1, nDan, COMMA(dPos2-dPos1), "������±ٹ���");
		Dom.DimLineToExtend(dPos3-dPos2, nDan, COMMA(dPos3-dPos2), "�߾ӹ�±ٹ���");
		Dom.DimLineToExtend(dPos4-dPos3, nDan, COMMA(dPos4-dPos3), "������±ٹ���");

		nDan = 1;
		Dom.DimMoveTo(dPos2 - pDeckData->m_dRangeDistribRebarOnCant, SlabMaxY, nDan);
		Dom.DimLineTo(pDeckData->m_dRangeDistribRebarOnCant, nDan, COMMA(pDeckData->m_dRangeDistribRebarOnCant));

		Dom.DimMoveTo(dPos3, SlabMaxY, nDan);
		Dom.DimLineTo(pDeckData->m_dRangeDistribRebarOnCant, nDan, COMMA(pDeckData->m_dRangeDistribRebarOnCant));
	}

	if(bDimRangeGir)
	{
		Dom.SetDirection("BOTTOM");
		nDan = 1;
		for (long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			dPos1 = pDB->GetGirder(nG)->m_dGirderCenter-pDeckData->m_dRangeDistribRebarOnGirder/2;
			Dom.DimMoveTo(dPos1, SlabMaxY, nDan);
			Dom.DimLineToExtend(pDeckData->m_dRangeDistribRebarOnGirder, nDan, COMMA(pDeckData->m_dRangeDistribRebarOnGirder), "�Ŵ�����");
		}
	}

	*pDom << Dom;
}



void CDrawDBFrontRebar::DimMainRebarLo(CDomyun *pDom, long nSection, BOOL bInput, double dTextRatio)
{
	CADeckData *pDeckData = m_pDataMng->GetDeckData();	
	CPlateBridgeApp		*pDB  = m_pDataMng->GetBridge();

	CPlateGirderApp  *pGirStt = pDB->GetGirder(0);
	CPlateGirderApp  *pGirEnd = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBxLeft = pDeckData->GetBxBySection(nSection, 0);
	CPlateBasicIndex *pBxRigh = pDeckData->GetBxBySection(nSection, pDB->GetGirdersu()-1);
	double dLeft = pDB->GetDisSlabLeftDom(pBxLeft);
	double MinY  = min(pGirEnd->GetElevationDom(pBxLeft), pGirStt->GetElevationDom(pBxRigh)) - 1000;
	double dSpace = pDeckData->m_dCoverSd;

	CDomyun Dom(pDom);
	
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	if(bInput && dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);

	CDPointArray *pPtArr = pDeckData->GetCTCBeryukRebar(nSection, FALSE);

	Dom.SetDirection("BOTTOM");	

	long nDan = 0;

	Dom.DimMoveTo(dLeft, MinY, nDan, COMMA(dSpace));
	Dom.DimLineTo(dSpace, nDan, "");	

	for(long n=0; n<pPtArr->GetSize(); n++)
	{
		CDPoint ptVal = pPtArr->GetAt(n);
		CString szVal, szVal2;

		if(ptVal.x>1)
		{
			szVal.Format("%.0f@%s", ptVal.x, COMMA(ptVal.y));
			szVal2.Format("=%s", COMMA(ptVal.x*ptVal.y));
		}	
		else
			szVal = COMMA(ptVal.y);

		Dom.DimLineToExtend(ptVal.x*ptVal.y,nDan, szVal, szVal2);
	}

	Dom.DimLineToOver(dSpace, nDan, COMMA(dSpace));

	*pDom << Dom;
}

void CDrawDBFrontRebar::DimMainRebarRigh(CDomyun *pDom, long nSection, BOOL bInput, double dTextRatio)
{
	CADeckData *pDeckData = m_pDataMng->GetDeckData();	
	CPlateBridgeApp		*pDB  = m_pDataMng->GetBridge();
	
	CPlateBasicIndex *pBx = pDeckData->GetBxBySection(nSection);
	double dSpaceUp = pDeckData->m_dCoverUp;
	double dSpaceDn = pDeckData->m_dCoverDn;

	CDomyun	Dom(pDom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	if(bInput && dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);

	CString str1, str2;
	
	CDPoint A[HDANLINESU], B[50];
	pDB->GetXySlabAct(pBx->GetStation(), A);
	CPlateGirderApp *pGirR = pDB->GetGirder(pDB->GetGirdersu()-1);
	pDB->GetXyFrontCantilever(pBx, B, FALSE);
	
	long nGuganLast = pDB->GetQtyHDanDom(pBx);
	double dHeightTotal = A[nGuganLast].y-B[0].y;
	double dLenMainDolchul = nSection==SECTION_CEN ? max(pDeckData->m_dLengthMainDolchulUp[1],pDeckData->m_dLengthMainDolchulLo[1]) : max(pDeckData->m_dLengthMainDolchulUpDan[1],pDeckData->m_dLengthMainDolchulLoDan[1]);
	double dHeightHunchRebar = dHeightTotal-dSpaceUp-dSpaceDn-dLenMainDolchul;	// ������ �ϸ鳡�ܿ� ��ġ�� ���� ���
	long   nCol   = 0;
	
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(B[0].x, B[0].y, nCol, COMMA(dSpaceDn));		
	Dom.DimLineTo(dSpaceDn, nCol, "");		
	if(dHeightHunchRebar>0)
	{
		Dom.DimLineToExtend(dHeightHunchRebar, nCol, "", COMMA(dHeightHunchRebar));		
		Dom.DimLineTo(dLenMainDolchul, nCol, COMMA(dLenMainDolchul));	
		Dom.DimLineToOver(dSpaceUp, nCol, COMMA(dSpaceUp));		
	}
	else
	{
		Dom.DimLineTo(dHeightTotal-dSpaceUp-dSpaceDn, nCol, COMMA(dHeightTotal-dSpaceUp-dSpaceDn));	
		Dom.DimLineToOver(dSpaceUp, nCol, COMMA(dSpaceUp));		
	}
	
	nCol = 1;
	Dom.DimMoveTo(B[0].x, B[0].y, nCol, "");
	Dom.DimLineTo(dHeightTotal, nCol, COMMA(dHeightTotal));

	if(!bInput)
	{	
		CPtrList *pList = Dom.GetObjectList(ETT_TEXT);
		POSITION pos = pList->GetHeadPosition();
		while(pos)
		{
			CObText *pText = (CObText *)pList->GetNext(pos);
			pText->m_TextStyle.Angle += 180;		
		}
	}
	
	*pDom << Dom;	
}

void CDrawDBFrontRebar::DimMainRebarLeft(CDomyun *pDom, long nSection, BOOL bInput, double dTextRatio)
{
	CADeckData *pDeckData = m_pDataMng->GetDeckData();	
	CPlateBridgeApp		*pDB  = m_pDataMng->GetBridge();
	CPlateBasicIndex		*pBx  = pDeckData->GetBxBySection(nSection);
	double dSpaceUp = pDeckData->m_dCoverUp;
	double dSpaceDn = pDeckData->m_dCoverDn;

	CDomyun	Dom(pDom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	if(bInput && dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);

	CString str1, str2;
	
	CDPoint A[HDANLINESU], B[50];
	pDB->GetXySlabAct(pBx->GetStation(), A);
	CPlateGirderApp *pGirL = pDB->GetGirder(0);
	pDB->GetXyFrontCantilever(pBx, B, TRUE);
	
	double dHeightTotal = A[0].y-B[0].y;
	double dLenMainDolchul = nSection==SECTION_CEN ? max(pDeckData->m_dLengthMainDolchulUp[0],pDeckData->m_dLengthMainDolchulLo[0]) : max(pDeckData->m_dLengthMainDolchulUpDan[0],pDeckData->m_dLengthMainDolchulLoDan[0]);
	double dHeightHunchRebar = dHeightTotal-dSpaceUp-dSpaceDn-dLenMainDolchul;	// ������ �ϸ鳡�ܿ� ��ġ�� ���� ���
	long   nCol   = 0;
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(B[0].x, B[0].y, nCol, COMMA(dSpaceDn));		
	Dom.DimLineTo(dSpaceDn, nCol, "");		
	if(dHeightHunchRebar>0)
	{
		Dom.DimLineToExtend(dHeightHunchRebar, nCol, "", COMMA(dHeightHunchRebar));		
		Dom.DimLineTo(dLenMainDolchul, nCol, COMMA(dLenMainDolchul));	
		Dom.DimLineToOver(dSpaceUp, nCol, COMMA(dSpaceUp));		
	}
	else
	{
		Dom.DimLineTo(dHeightTotal-dSpaceUp-dSpaceDn, nCol, COMMA(dHeightTotal-dSpaceUp-dSpaceDn));	
		Dom.DimLineToOver(dSpaceUp, nCol, COMMA(dSpaceUp));		
	}

	nCol = 1;
	Dom.DimMoveTo(B[0].x, B[0].y, nCol, "");
	Dom.DimLineTo(dHeightTotal, nCol, COMMA(dHeightTotal));
	
	if(!bInput)
	{	
		CPtrList *pList = Dom.GetObjectList(ETT_TEXT);
		POSITION pos = pList->GetHeadPosition();
		while(pos)
		{
			CObText *pText = (CObText *)pList->GetNext(pos);
			pText->m_TextStyle.Angle += 180;		
		}
	}
	
	*pDom << Dom;	
}

void CDrawDBFrontRebar::DrawMainRebarUp(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, BOOL bDrawJoint, long nShape)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();	
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();	
	CDrawDBDeck DeckDraw(m_pDataMng);
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();

	//////////////////////////////////////////////////////////////////////////
	// ��ö��                                                               
	vector<REBAR_SHAPE> vRebarShape = pDeckData->GetNewRebarShapeArrBySection(nSection, nCycle, TRUE);

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_BARC);
	
	DeckDraw.DrawRebarShapeArr(&Dom, vRebarShape, bInput, FALSE, nShape);
	//////////////////////////////////////////////////////////////////////////
	// ��ö���� ������ ǥ��													
	pOpt->SetEnvType(&Dom,HCAD_BARA);

	CDPointArray xyArrDim, xyArrStt, xyArrEnd;
	if(vRebarShape.size() > 2)
	{
		if(vRebarShape[0].m_bValid && vRebarShape[1].m_bValid)
		{
			xyArrStt = vRebarShape[0].m_dArrShape;
			xyArrDim.Add(xyArrStt[xyArrStt.GetUpperBound()]);
		}
		for(long n=1; n<(long)vRebarShape.size()-1; n++)
		{
			BOOL bValidPre = vRebarShape[n-1].m_bValid;
			BOOL bValidCur = vRebarShape[n].m_bValid;
			BOOL bValidPost = vRebarShape[n+1].m_bValid;
			CDPointArray xyArrRebar = vRebarShape[n].m_dArrShape;
			CDPoint xyStt = xyArrRebar[0];
			CDPoint xyEnd = xyArrRebar[xyArrRebar.GetUpperBound()];
			if(bValidPre && bValidCur)
				xyArrDim.Add(xyStt);
			if(bValidPost && bValidCur)
				xyArrDim.Add(xyEnd);
		}
		if(vRebarShape[vRebarShape.size()-2].m_bValid && vRebarShape[vRebarShape.size()-1].m_bValid)
		{
			xyArrEnd = vRebarShape[vRebarShape.size()-1].m_dArrShape;
			xyArrDim.Add(xyArrEnd[xyArrEnd.GetUpperBound()]);
		}
	}
	else 
	{
		for(long n=0; n<(long)vRebarShape.size(); n++)
		{
			CDPointArray xyArrRebar = vRebarShape[n].m_dArrShape;
			BOOL bValidCur = vRebarShape[n].m_bValid;
			if(bValidCur)
				xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		}
	}

	if(xyArrDim.GetSize() > 1 && bDrawJoint)
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


void CDrawDBFrontRebar::DrawMainRebarLo(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, BOOL bDrawJoint, long nShape)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();	
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();	
	CDrawDBDeck DeckDraw(m_pDataMng);
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();

	//////////////////////////////////////////////////////////////////////////
	// ��ö��                                                               
	vector<REBAR_SHAPE> vRebarShape = pDeckData->GetNewRebarShapeArrBySection(nSection, nCycle, FALSE);

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_BARC);
	
	DeckDraw.DrawRebarShapeArr(&Dom, vRebarShape, bInput, TRUE, nShape);
	
	//////////////////////////////////////////////////////////////////////////
	// ��ö���� ������ ǥ��													
	if( (nSection==SECTION_CEN || nSection==SECTION_PIER) && pDeckData->IsBridgeHasSlabHunch())	
	{
		*pDom << Dom;
		return;
	}

	pOpt->SetEnvType(&Dom,HCAD_BARA);

	CDPointArray xyArrDim, xyArrStt, xyArrEnd;
	if(vRebarShape.size() > 2)
	{
		if(vRebarShape[0].m_bValid && vRebarShape[1].m_bValid)
		{
			xyArrStt = vRebarShape[0].m_dArrShape;
			xyArrDim.Add(xyArrStt[xyArrStt.GetUpperBound()]);
		}
		for(long n=1; n<(long)vRebarShape.size()-1; n++)
		{
			BOOL bValidPre = vRebarShape[n-1].m_bValid;
			BOOL bValidCur = vRebarShape[n].m_bValid;
			BOOL bValidPost = vRebarShape[n+1].m_bValid;
			CDPointArray xyArrRebar = vRebarShape[n].m_dArrShape;
			CDPoint xyStt = xyArrRebar[0];
			CDPoint xyEnd = xyArrRebar[xyArrRebar.GetUpperBound()];
			if(bValidPre && bValidCur)
				xyArrDim.Add(xyStt);
			if(bValidPost && bValidCur)
				xyArrDim.Add(xyEnd);
		}
		if(vRebarShape[vRebarShape.size()-2].m_bValid && vRebarShape[vRebarShape.size()-1].m_bValid)
		{
			xyArrEnd = vRebarShape[vRebarShape.size()-1].m_dArrShape;
			xyArrDim.Add(xyArrEnd[xyArrEnd.GetUpperBound()]);
		}
	}
	else 
	{
		for(long n=0; n<(long)vRebarShape.size(); n++)
		{
			CDPointArray xyArrRebar = vRebarShape[n].m_dArrShape;
			BOOL bValidCur = vRebarShape[n].m_bValid;
			if(bValidCur)
				xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		}
	}

	if(xyArrDim.GetSize() > 1 && bDrawJoint)
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
				xyArrDim[n].y -= REBAR_MAIN_LAPJOINT_GAP;
				xyArrDim[n+1].y -= REBAR_MAIN_LAPJOINT_GAP;
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



void CDrawDBFrontRebar::DrawMainRebar(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, BOOL bDrawJoint, long nShape)
{
	CDomyun Dom(pDom);

	DrawMainRebarUp(&Dom, nSection, nCycle, bInput, bDrawJoint, -1);		
	DrawMainRebarLo(&Dom, nSection, nCycle, bInput, bDrawJoint, nShape);		

	*pDom << Dom;
}

void CDrawDBFrontRebar::DrawMainRebarShear(CDomyun *pDom, long nSection)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBasicIndex		*pBxMatch	= pDeckData->GetBxBySection(nSection, 0);
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
	CDomyun			Dom(pDom);
	CConcSymbol		SymDom(pOpt);
	CRebarPlacing RebarPlacing;

	CDPointArray	BaseArrUp;
	CDPointArray	BaseArrLo;
	CDPointArray	xyArrPointUp;		// ��� ��ö��
	CDPointArray	xyArrPointLo;		// �Ϻ� ��ö��

	//////////////////////////////////////////////////////////////////////////
	// ��� ��ö�� Array ���ϱ�
	BaseArrUp	 = pDeckData->GetXyArrGuideLineForMainRebar(nSection, TRUE);
	xyArrPointUp = pDeckData->MakePointRebarArr(&BaseArrUp, pDeckData->GetCTCBeryukRebar(nSection, TRUE), FALSE, TRUE);
	//////////////////////////////////////////////////////////////////////////
	// �Ϻ� ��ö�� Array ���ϱ�
	BaseArrLo	 = pDeckData->GetXyArrGuideLineForMainRebar(nSection, FALSE);
	xyArrPointLo = pDeckData->MakePointRebarArr(&BaseArrLo, pDeckData->GetCTCBeryukRebar(nSection, FALSE), FALSE, FALSE);
	//////////////////////////////////////////////////////////////////////////
	// ������ ö��
	CDPointArray xyArrUpLo;
	long n = 0;
	for(n=0; n<xyArrPointUp.GetSize(); n++)
		xyArrUpLo.Add(CDPoint(xyArrPointUp[n].x, 0));
	for(n=0; n<xyArrPointLo.GetSize(); n++)
		xyArrUpLo.Add(CDPoint(xyArrPointLo[n].x, 0));

	RebarPlacing.RemoveSamePoint(xyArrUpLo, TRUE, 10);
	xyArrUpLo.Sort();
	CDPointArray xyArrUpLoModify;
	long nCnt = long(pDeckData->m_SpacerRebar.GetCntDistRebarSet()/2);
	if(nCnt==0)		return;

	for(n=0; n<xyArrUpLo.GetSize(); n+=nCnt)
	{
		if(n!=0 && n!=xyArrUpLo.GetUpperBound())
			xyArrUpLoModify.Add(xyArrUpLo[n]);
	}

	CDPointArray xyArrShearUp = pDeckData->GetCDPointArrByPolyLine(xyArrUpLoModify, BaseArrUp, TRUE);
	CDPointArray xyArrShearLo = pDeckData->GetCDPointArrByPolyLine(xyArrUpLoModify, BaseArrLo, TRUE);

	pOpt->SetEnvType(&Dom, HCAD_BNTB);
	for(n=0; n<xyArrShearUp.GetSize(); n++)
	{	
		if(n%2==0)			
			pOpt->SetEnvLType(&Dom, LT_SOLID);
		else
			pOpt->SetEnvLType(&Dom, LT_HIDDEN2_LINE);
		Dom.LineTo(xyArrShearUp[n], xyArrShearLo[n]);
	}

	*pDom << Dom;
}




void CDrawDBFrontRebar::DrawMarkShearRebar(CDomyun *pDom, long nSection, BOOL bMirrorPos)
{
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBasicIndex		*pBxMatch	= pDeckData->GetBxBySection(nSection, 0);
	
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
	CDomyun			Dom(pDom);
	CConcSymbol		SymDom(pOpt);
	CRebarPlacing RebarPlacing;

	CDPointArray	BaseArrUp;
	CDPointArray	BaseArrLo;
	CDPointArray	xyArrPointUp;		// ��� ��ö��
	CDPointArray	xyArrPointLo;		// �Ϻ� ��ö��

	pOpt->SetEnvType(&Dom,HCAD_DIML);
	//////////////////////////////////////////////////////////////////////////
	// ��� ��ö�� Array ���ϱ�
	BaseArrUp	 = pDeckData->GetXyArrGuideLineForMainRebar(nSection, TRUE);
	xyArrPointUp = pDeckData->MakePointRebarArr(&BaseArrUp, pDeckData->GetCTCBeryukRebar(nSection, TRUE), FALSE, TRUE);
	//////////////////////////////////////////////////////////////////////////
	// �Ϻ� ��ö�� Array ���ϱ�
	BaseArrLo	 = pDeckData->GetXyArrGuideLineForMainRebar(nSection, FALSE);
	xyArrPointLo = pDeckData->MakePointRebarArr(&BaseArrLo, pDeckData->GetCTCBeryukRebar(nSection, FALSE), FALSE, FALSE);
	//////////////////////////////////////////////////////////////////////////
	// ������ ö��
	CDPointArray xyArrUpLo;
	long n = 0;
	for(n=0; n<xyArrPointUp.GetSize(); n++)
		xyArrUpLo.Add(CDPoint(xyArrPointUp[n].x, 0));
	for(n=0; n<xyArrPointLo.GetSize(); n++)
		xyArrUpLo.Add(CDPoint(xyArrPointLo[n].x, 0));

	RebarPlacing.RemoveSamePoint(xyArrUpLo, TRUE, 10);
	xyArrUpLo.Sort();
	CDPointArray xyArrUpLoModify;
	long nCnt = long(pDeckData->m_SpacerRebar.GetCntDistRebarSet()/2);
	if(nCnt==0)		return;

	for(n=0; n<xyArrUpLo.GetSize(); n+=nCnt)
	{
		if(n!=0 && n!=xyArrUpLo.GetUpperBound())
			xyArrUpLoModify.Add(xyArrUpLo[n]);
	}

	CDPointArray xyArrShearUp = pDeckData->GetCDPointArrByPolyLine(xyArrUpLoModify, BaseArrUp, TRUE);
	CDPointArray xyArrShearLo = pDeckData->GetCDPointArrByPolyLine(xyArrUpLoModify, BaseArrLo, TRUE);

	CMarkDomyun mark(&Dom, pOpt);
	CTwinVectorArray	tvArr;
	CTwinVectorArray	tvSup;
	CTwinVector			tv;
	CDPointArray		xyArray;
	CRebar	*pRB			= NULL;

	long	nBeryukCycle	= pDeckData->m_stBeryukCycleLeft.size();
	if(nBeryukCycle==0)		return;
	
	long	nSttIdx = 0;
	long	nEndIdx = (long)(xyArrShearUp.GetSize()/3);
	if(bMirrorPos)
	{
		// ���ܺ���ö��
		for(long i = nEndIdx-1; i >= nSttIdx; i--)
		{
			tv.m_v1	= xyArrShearUp.GetAt(i);
			tv.m_v2	= xyArrShearLo.GetAt(i);
			if(bMirrorPos)
			{
				tv.m_v1.x = -tv.m_v1.x;
				tv.m_v2.x = -tv.m_v2.x;
			}
			tvArr.Add(tv);
		}
	}
	else
	{
		// ���ܺ���ö��
		for(long i = nSttIdx; i < nEndIdx; i++)
		{
			tv.m_v1	= xyArrShearUp.GetAt(i);
			tv.m_v2	= xyArrShearLo.GetAt(i);
			if(bMirrorPos)
			{
				tv.m_v1.x = -tv.m_v1.x;
				tv.m_v2.x = -tv.m_v2.x;
			}
			tvArr.Add(tv);
		}
	}


	tv.m_v1	= xyArrShearUp.GetAt(0);
	tv.m_v2	= xyArrShearUp.GetAt(xyArrShearUp.GetSize()-1);
	if(bMirrorPos)
	{
		tv.m_v1	= xyArrShearUp.GetAt(xyArrShearUp.GetSize()-1);
		tv.m_v2	= xyArrShearUp.GetAt(0);
		tv.m_v1.x = -tv.m_v1.x;
		tv.m_v2.x = -tv.m_v2.x;
	}
	tvSup.Add(tv);

	tv.m_v1	= xyArrShearLo.GetAt(0);
	tv.m_v2	= xyArrShearLo.GetAt(xyArrShearLo.GetSize()-1);
	if(bMirrorPos)
	{
		tv.m_v1	= xyArrShearLo.GetAt(xyArrShearLo.GetSize()-1);
		tv.m_v2	= xyArrShearLo.GetAt(0);
		tv.m_v1.x = -tv.m_v1.x;
		tv.m_v2.x = -tv.m_v2.x;
	}
	tvSup.Add(tv);

	pRB = pDeckData->GetRebarByName("������");
	if(pRB)
	{
		mark.SetRebar(tvArr, tvSup);
		mark.m_sMark	= pRB->m_sMark;
		mark.m_sDia		= pRB->GetStringDia();
		mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_dPosScaleSpace			= 0.25;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_dAddHeight				= Dom.Always(5);
		mark.MarkRebar();
	}

	*pDom << Dom;
}



// ��ö�� ������ ġ���� 
void CDrawDBFrontRebar::DimMainRebarJolib(CDomyun *pDom, long nSection, long nCycle, BOOL bInput, double dTextRatio)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();	
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();

	BOOL   bSkewSection = nSection==SECTION_SKEW_STT||nSection==SECTION_SKEW_END ? TRUE : FALSE;
	CDPoint vAngSec = CDPoint(0,1);	// Ⱦ�ܸ鿡�� ��
	if(nSection==SECTION_SKEW_STT)
		vAngSec = pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	else if(nSection==SECTION_SKEW_END)
		vAngSec = pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	double dRatio = fabs(vAngSec.y);

	CDomyun Dom(pDom);

	CString sText, sVariable;
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	if(dTextRatio!=1.0)
		Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);
	
	//////////////////////////////////////////////////////////////////////////
	// ����ö��                                                             
	//////////////////////////////////////////////////////////////////////////
	DimMainRebarLeft(&Dom, nSection, bInput);
	DimMainRebarRigh(&Dom, nSection, bInput);

	//////////////////////////////////////////////////////////////////////////
	// ���ö��                                                             
	//////////////////////////////////////////////////////////////////////////
	vector<REBAR_SHAPE> vRebarShape = pDeckData->GetNewRebarShapeArrBySection(nSection, nCycle, TRUE);
	long nShapeSu = vRebarShape.size();

	CDPointArray xyArrStt = vRebarShape.at(0).m_dArrShape;
	CDPointArray xyArrEnd = vRebarShape.at(nShapeSu-1).m_dArrShape;

	//////////////////////////////////////////////////////////////////////////
	// ������ ġ���� ������ġ
	CDPointArray xyArrDim;
	xyArrDim.Add(xyArrStt[0]);	// ����

	double dYMax = xyArrStt[0].y;
	long nShape = 0;
	for(nShape=0; nShape<nShapeSu; nShape++)
	{
		REBAR_SHAPE vShape = vRebarShape.at(nShape);
		if(vShape.m_bValid==FALSE)		continue;

		CDPointArray xyArrRebar = vShape.m_dArrShape;
		if(nShape==0 || nShape==nShapeSu-1)
			xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		else
		{
			xyArrDim.Add(xyArrRebar[0]);
			xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		}

		for (long m=0; m<xyArrRebar.GetSize(); m++)
			dYMax = max(dYMax, xyArrRebar[m].y);
	}
	xyArrDim.Add(xyArrEnd[0]);	// ����	
	xyArrDim.Sort(TRUE);		// 2�� ġ������ ���

	REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleBySection(nSection, nCycle);
	//////////////////////////////////////////////////////////////////////////
	// 1��, 2��, 3�� ġ����
	Dom.SetDirection("TOP");
	long nCol = 1;
	CDoubleArray arrJoint = pRebarCycle->m_dArrJointUp;
	if(xyArrDim.GetSize() > 2 && arrJoint.GetSize()>0)	// ������ ���� ���
	{
		//////////////////////////////////////////////////////////////////////////
		// 1�� ��������ġ
		if(bInput)
		{
			Dom.DimMoveTo(xyArrStt[0].x, dYMax, nCol);
			double dSumJoint = 0;
			for (long n=0; n<=arrJoint.GetSize(); n++)
			{
				sText.Format("U%d-%d", n+1, nCycle+1);
				double dJointPos = n==arrJoint.GetSize() ? (xyArrEnd[0].x-xyArrStt[0].x-dSumJoint) : arrJoint[n];				

				if(bSkewSection)	sVariable.Format("%s(%s)", COMMA(dJointPos), COMMA(dJointPos/dRatio));
				else				sVariable.Format("%s", COMMA(dJointPos));
				if(bInput)
					Dom.DimLineToExtend(dJointPos, nCol, sText, sVariable);
				else
					Dom.DimLineTo(dJointPos, nCol, sVariable);
				if(n<arrJoint.GetSize())
					dSumJoint += arrJoint[n];
			}

			nCol++;
		}
		//////////////////////////////////////////////////////////////////////////
		// 2�� ������ǥ������
		for(long n=0; n<xyArrDim.GetSize(); n++)
		{
			if(n==0)	
				Dom.DimMoveTo(xyArrDim[n].x, dYMax, nCol);
			else	
			{
				if(bSkewSection)	sVariable.Format("%s(%s)", COMMA(xyArrDim[n].x-xyArrDim[n-1].x), COMMA((xyArrDim[n].x-xyArrDim[n-1].x)/dRatio));
				else				sVariable.Format("%s", COMMA(xyArrDim[n].x-xyArrDim[n-1].x));
				Dom.DimLineTo(xyArrDim[n].x-xyArrDim[n-1].x, nCol, sVariable);
			}
		}
		nCol++;
	}
	//////////////////////////////////////////////////////////////////////////
	// ��üġ��
	CDPoint xyStt, xyEnd;
	if(vRebarShape.size()>1)
	{
		xyStt = xyArrStt[1];
		xyEnd = xyArrEnd[1];
	}
	else
	{
		xyStt = xyArrStt[1];
		xyEnd = xyArrStt[xyArrStt.GetUpperBound()];
	}

	Dom.DimMoveTo(xyStt.x, dYMax, nCol);
	if(bSkewSection)	sVariable.Format("%s(%s)", COMMA(xyEnd.x-xyStt.x), COMMA((xyEnd.x-xyStt.x)/dRatio));	
	else				sVariable.Format("%s", COMMA(xyEnd.x-xyStt.x));		
	Dom.DimLineTo(xyEnd.x-xyStt.x, nCol, sVariable);


	//////////////////////////////////////////////////////////////////////////
	// �Ϻ�ö��                                                             
	//////////////////////////////////////////////////////////////////////////
	
	vector<REBAR_SHAPE> vRebarShapeDn = pDeckData->GetNewRebarShapeArrBySection(nSection, nCycle, FALSE);
	nShapeSu = vRebarShapeDn.size();
	if(nShapeSu ==0 || ((nSection==SECTION_CEN || nSection==SECTION_PIER) && pDeckData->IsBridgeHasSlabHunch()))	
	{
		*pDom << Dom;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ ġ���� ������ġ
	xyArrStt = vRebarShapeDn.at(0).m_dArrShape;
	xyArrEnd = vRebarShapeDn.at(nShapeSu-1).m_dArrShape;

	xyArrDim.RemoveAll();
	xyArrDim.Add(xyArrStt[0]);

	double dYMin = xyArrStt[0].y;
	for(nShape=0; nShape<nShapeSu; nShape++)
	{
		REBAR_SHAPE vShape = vRebarShapeDn.at(nShape);
		if(vShape.m_bValid==FALSE)		continue;

		CDPointArray xyArrRebar = vShape.m_dArrShape;
		if(nShape==0 || nShape==nShapeSu-1)
			xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		else
		{
			xyArrDim.Add(xyArrRebar[0]);
			xyArrDim.Add(xyArrRebar[xyArrRebar.GetUpperBound()]);
		}

		for (long m=0; m<xyArrRebar.GetSize(); m++)
			dYMin = min(dYMin, xyArrRebar[m].y);
	}

	xyArrDim.Add(xyArrEnd[0]);
	xyArrDim.Sort(TRUE);		// 2�� ġ������ ���
	
	arrJoint = pRebarCycle->m_dArrJointDn;
	nCol = 1;
	Dom.SetDirection("BOTTOM");
	if(xyArrDim.GetSize() > 2 && arrJoint.GetSize()>0)
	{
		//////////////////////////////////////////////////////////////////////////
		// 1�� ������ġ(D1, D2, ...)
		if(bInput)
		{
			Dom.DimMoveTo(xyArrStt[0].x, dYMin, nCol);
			double dSumJoint = 0;
			for (long n=0; n<=arrJoint.GetSize(); n++)
			{
				sText.Format("D%d-%d", n+1, nCycle+1);
				double dJointPos = n==arrJoint.GetSize() ? (xyArrEnd[0].x-xyArrStt[0].x-dSumJoint) : arrJoint[n];				

				if(bSkewSection)	sVariable.Format("%s(%s)", COMMA(dJointPos), COMMA(dJointPos/dRatio));
				else				sVariable.Format("%s", COMMA(dJointPos));
				if(bInput)
					Dom.DimLineToExtend(dJointPos, nCol, sText, sVariable);
				else
					Dom.DimLineTo(dJointPos, nCol, sVariable);
				if(n<arrJoint.GetSize())
					dSumJoint += arrJoint[n];
			}
			nCol++;
		}
		//////////////////////////////////////////////////////////////////////////
		// 2�� ������ǥ������
		for(long n=0; n<xyArrDim.GetSize(); n++)
		{
			if(n==0)	
				Dom.DimMoveTo(xyArrDim[n].x, dYMin, nCol);
			else	
			{
				if(bSkewSection)	sVariable.Format("%s(%s)", COMMA(xyArrDim[n].x-xyArrDim[n-1].x), COMMA((xyArrDim[n].x-xyArrDim[n-1].x)/dRatio));
				else				sVariable.Format("%s", COMMA(xyArrDim[n].x-xyArrDim[n-1].x));
				Dom.DimLineTo(xyArrDim[n].x-xyArrDim[n-1].x, nCol, sVariable);
			}
		}
		nCol++;
	}

	// ��üġ��
	if(vRebarShapeDn.size()>1)
	{
		xyStt = xyArrStt[1];
		xyEnd = xyArrEnd[1];
	}
	else
	{
		xyStt = xyArrStt[1];
		xyEnd = xyArrStt[xyArrStt.GetUpperBound()];
	}

	Dom.DimMoveTo(xyStt.x, dYMin, nCol);
	if(bSkewSection)	sVariable.Format("%s(%s)", COMMA(xyEnd.x-xyStt.x), COMMA((xyEnd.x-xyStt.x)/dRatio));	
	else				sVariable.Format("%s", COMMA(xyEnd.x-xyStt.x));		
	Dom.DimLineTo(xyEnd.x-xyStt.x, nCol, sVariable);

	*pDom << Dom;	
}

// ���κ� ��ġ�� �ܸ� ��
// dDotRebar : ��ö��ǥ�� ����
void CDrawDBFrontRebar::DrawSangseCrossBeam(CDomyun *pDom)
{
	CADeckData			*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();	
	CDrawDBDeck			DeckDraw(m_pDataMng);
	CARoadOptionStd		*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();

	if(pDB->GetGirdersu() < 2)		return;

	CConcSymbol	SymDom(pOpt);
	
	CDomyun Dom(pDom);
	
	long nViewAddLen = 700;
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);

	long m_nG = 0;
//	if(pDB->GetGirdersu() >=2)	m_nG = pDB->GetGirdersu() - 2;

	CPlateBasicIndex *pBx = pDB->GetGirder(m_nG)->GetBxCrossBeamHasHunch();
	if(pBx==NULL)	return;

	CPlateCrossBeam  *pC  = pBx->GetCrossBeam();
	/************************************************************************/
	/* end                                                                  */
	/************************************************************************/
	// 
	CDPoint A[47];
	CDPoint uXy2(0,1);
	CDPoint xyResultFir(0,0), xyResultSec(0,0);

	if(pC)
	{
		pDB->GetGirder(m_nG)->GetHunchXyDom(pBx, A);
		GetXyMatchLineAndLine(A[4], A[5], A[3], uXy2, xyResultFir);
	}

	double dChgSlabFirH = pC ? xyResultFir.y-A[3].y : 0;
	double dChgSlabSecH = 0;
//	if(pBx->IsStringer())
//	{
//		GetXyMatchSegAndLine(A[10], A[11], A[12], uXy2, xyResultSec);
//		dChgSlabSecH = xyResultSec.y-A[12].y;
//	}
//	else
	{
		GetXyMatchLineAndLine(A[4], A[5], A[6], uXy2, xyResultSec);
		dChgSlabSecH = xyResultSec.y-A[6].y;
	}

	/************************************************************************/
	/* ������ �� ����                                                       */
	/************************************************************************/
	// ����� ��ȭ ���� ����
	double dChgSlabFirW		= dChgSlabFirH * 3;
	double dChgSlabSecW		= dChgSlabSecH * 3;
	double dChgSlabAverW	= fabs((dChgSlabFirW + dChgSlabSecW) / 2);
	double dChgSlabAverH	= fabs((dChgSlabFirH + dChgSlabSecH) / 2);
	double dSlabW			= pDB->m_dWidthHunch;
	double dCrossUW			= pC ? pC->m_uM_CR : 0;
	double dWidthBottom		= (dSlabW + dCrossUW)*2;	// �¿���ġ �߾ӱ�����

	// ������� ���� ����
	double dPave	 = pDB->GetThickPave();
	double dSlabH	 = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
	double dSlabAllW = dCrossUW+dSlabW+dChgSlabAverW;	//��ü �������� ��

	// ������ �׸���
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);

	Dom.MoveTo(0,0);
	Dom.LineToEx(-dCrossUW,0);
	Dom.LineToEx(-dSlabW,0);
	Dom.LineToEx(-dChgSlabAverW, dChgSlabAverH);
	Dom.LineToEx(-nViewAddLen, 0);

	Dom.MoveTo(0,0);
	Dom.LineToEx(dCrossUW,0);
	Dom.LineToEx(dSlabW, 0);
	Dom.LineToEx(dChgSlabAverW, dChgSlabAverH);
	Dom.LineToEx(nViewAddLen, 0);

	// ����
	m_pDataMng->GetOptionStd()->SetEnvLType(&Dom, LT_SOLID);

	double dPaveTopH = dChgSlabAverH + dSlabH + dPave;
	double dPaveBotH = dChgSlabAverH + dSlabH;
	Dom.Solid(-dSlabAllW-nViewAddLen, dPaveTopH, dSlabAllW+nViewAddLen, dPaveTopH, 
		dSlabAllW+nViewAddLen, dPaveBotH, -dSlabAllW-nViewAddLen, dPaveBotH);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CUTL);
	// ���ܸ�
	Dom.CutLightning(-dSlabAllW-nViewAddLen, dChgSlabAverH-Dom.Always(1),
		-dSlabAllW-nViewAddLen, dChgSlabAverH+dSlabH+dPave+Dom.Always(1));
	Dom.CutLightning(dSlabAllW+nViewAddLen, dChgSlabAverH-Dom.Always(1),
		dSlabAllW+nViewAddLen, dChgSlabAverH+dSlabH+dPave+Dom.Always(1));

	/************************************************************************/
	/* ���κ� ����ö��, ���ö�� ǥ��                                       */
	/************************************************************************/
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);
	
	// �Ϻι��ö��
	CDPoint xyStt(-dSlabAllW-nViewAddLen, dChgSlabAverH+pDeckData->m_dCoverDn);
	CDPoint xyEnd( dSlabAllW+nViewAddLen, dChgSlabAverH+pDeckData->m_dCoverDn);
	Dom.LineTo(xyStt, xyEnd);
	// ��ι��ö��
	xyStt.y = xyEnd.y += (dSlabH-pDeckData->m_dCoverUp-pDeckData->m_dCoverDn); 
	Dom.LineTo(xyStt, xyEnd);
	double dUpperRebarEle = xyEnd.y; 
	
	// ���κ� ����ö�� : C1
	double dChgRebarAverW;			// �¿��� ����ö���� �� -----> ����� ���
	//	Left
	CDPoint uLineLeft(-dChgSlabAverW, dChgSlabAverH);
	CDPoint xyLeftRebar(-dCrossUW-dSlabW, pDeckData->m_dCoverDn);
	GetXyMatchSegAndLine(xyStt, xyEnd, xyLeftRebar, uLineLeft, xyResultFir);
	Dom.LineTo(xyResultFir, xyLeftRebar);
	dChgRebarAverW = xyLeftRebar.x - xyResultFir.x;
	CDPoint xyC1Result;				// C1 ǥ�� ����Ʈ
	CDPoint xyC1Temp(-dCrossUW-dSlabW-dChgSlabAverW*1.5, 0);
	GetXyMatchSegAndLine(xyResultFir, xyLeftRebar, xyC1Temp, uXy2, xyC1Result);
	CDPoint xyC3Result(-dCrossUW-dSlabW-dChgSlabAverW*1.5, dUpperRebarEle);
	//	Right
	CDPoint uLineRight(dChgSlabAverW, dChgSlabAverH);
	CDPoint xyRightRebar(dCrossUW+dSlabW, pDeckData->m_dCoverDn);
	GetXyMatchSegAndLine(xyStt, xyEnd, xyRightRebar, uLineRight, xyResultSec);
	Dom.LineTo(xyRightRebar, xyResultSec);
	//	Center
	Dom.LineTo(xyLeftRebar, xyRightRebar);
	//	��ġö�� : C2
	DeckDraw.DrawPointRebar(&Dom, xyLeftRebar, REBARPOS_UP);
	DeckDraw.DrawPointRebar(&Dom, xyRightRebar, REBARPOS_UP);

	/************************************************************************/
	/* ��ö��(Dot Type) ǥ��                                                */
	/************************************************************************/
	double dMainRebarCTC = pDeckData->m_dGenMainRebarCTC[2]/2;
	DeckDraw.DrawPointRebarByLine(&Dom, xyStt, xyEnd, dMainRebarCTC, REBARPOS_UP, 1);
	xyStt.y = xyEnd.y -= (dSlabH-pDeckData->m_dCoverUp-pDeckData->m_dCoverDn); 
	DeckDraw.DrawPointRebarByLine(&Dom, xyStt, xyEnd, dMainRebarCTC, REBARPOS_DOWN, 1);

	/************************************************************************/
	/* ġ����                                                               */
	/************************************************************************/
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	// ū���� ������ ����
	double dChgRebarFirW = dChgRebarAverW - dChgSlabAverW + dChgSlabFirW;
	double dChgRebarSecW = dChgRebarAverW - dChgSlabAverW + dChgSlabSecW;

	GetSwapByMin(dChgRebarFirW, dChgRebarSecW);
	GetSwapByMin(dChgSlabFirH, dChgSlabSecH);

	// ������ �Ϲ� ����
	long nCol = 0;
	CString str;
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-dSlabAllW, dUpperRebarEle+pDeckData->m_dCoverUp+dPave, nCol);
	if(dChgSlabFirW == dChgSlabSecW)
		str.Format("%s", COMMA(dChgSlabFirW));
	else
		str.Format("%s~%s",COMMA(dChgSlabFirW), COMMA(dChgSlabSecW));
	Dom.DimLineTo(dChgSlabAverW, nCol, str);
	Dom.DimLineToExtend(dSlabW, nCol, "", COMMA(dSlabW));
	Dom.DimLineTo(dCrossUW, nCol, COMMA(dCrossUW));
	Dom.DimLineTo(dCrossUW, nCol, COMMA(dCrossUW));
	Dom.DimLineToExtend(dSlabW, nCol, "", COMMA(dSlabW));
	Dom.DimLineTo(dChgSlabAverW, nCol, str);

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dSlabAllW+nViewAddLen, 0, nCol);
	if(dChgSlabFirH == dChgSlabSecH)
		str.Format("%s", COMMA(dChgSlabFirH));
	else
		str.Format("%s~%s",COMMA(dChgSlabFirH), COMMA(dChgSlabSecH));
	Dom.DimLineTo(dChgSlabAverH, nCol, str);
	Dom.DimLineToExtend(dSlabH, nCol,  "", COMMA(dSlabH));
	Dom.DimLineTo(dPave, nCol, COMMA(dPave));

	// ������ ��� ����
	nCol = 2;
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(-dCrossUW-dSlabW-dChgRebarAverW, 0, nCol);
	if(dChgRebarFirW == dChgRebarSecW)
		str.Format("%s", COMMA(dChgRebarFirW));
	else
		str.Format("%s~%s",COMMA(dChgRebarFirW), COMMA(dChgRebarSecW));
	Dom.DimLineTo(dChgRebarAverW, nCol, str);
	Dom.DimLineToExtend(dWidthBottom, nCol, "", COMMA(dWidthBottom));
	Dom.DimLineTo(dChgRebarAverW, nCol, str);
	
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(-dSlabAllW-nViewAddLen, 0, 0);
	if(dChgSlabFirH == dChgSlabSecH)
		str.Format("%s", COMMA(dChgSlabFirH));
	else
		str.Format("%s~%s",COMMA(dChgSlabFirH), COMMA(dChgSlabSecH));
	Dom.DimLineTo(dChgSlabAverH, 0, str);
	Dom.DimLineToExtend(dSlabH, 0, "", COMMA(dSlabH));
	Dom.DimLineToOver(dPave, 0, COMMA(dPave));

	// C1, C2, C3 ���ü�
	CString sText = _T("");
	CRebar *pRB		= pDeckData->GetRebarByName("���κ���ġ����");
	if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyC1Result, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);

	pRB		= pDeckData->GetRebarByName("���κ��ϸ麸��");
	if(pRB)		SymDom.RebarMarkPointLower(&Dom, xyRightRebar+CDPoint(0,1)*REBAR_SYMBOL_RADIUS, dWidthBottom, 0, pRB->m_sMark, pRB->GetStringDia(), FALSE, 0);

	if(pDeckData->m_bInstallCrossBeamUpperBogangRebar)
	{
		pRB		= pDeckData->GetRebarByName("���κ���麸��");
		if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyC3Result, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
	}

	if(dPave>0)
	{
		Dom.SetDirection("TOP");
		m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
		Dom.GiSiArrowExtend(dSlabAllW+nViewAddLen-Dom.Always(3), dChgSlabAverH+dSlabH+dPave,Dom.Always(10),Dom.Always(20),2,_T("����"),_T("T = "+COMMA(dPave)+"mm"));		
	}

	*pDom << Dom;
}

// ���κ� ��ġ�� �ܸ� ��
// ���κ��� ������ ���� : ���κ��� ������ ��� ���κ� �׸�
BOOL CDrawDBFrontRebar::DrawSangseStringer(CDomyun *pDom)
{
//	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
//	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();	
//	CDrawDBDeck DeckDraw(m_pDataMng);
//	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
//
//	if(pDB->GetGirdersu() < 2)		return FALSE;
//
//	CConcSymbol	SymDom(pOpt);
//
//	CDomyun Dom(pDom);
//
//	long nViewAddLen = 700;
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_STLC);
//
//	long m_nG = 0;
//	CPlateGirderApp *pGir = pDB->GetGirder(m_nG);
//	CPlateBxFinder Finder(pGir);
//	CPlateBasicIndex *pBx = pDeckData->GetBxBySection(SECTION_CEN, m_nG); 
//	CStringer *pS = NULL;
//	while(pBx)
//	{     
//		if(pBx->IsStringer())
//			break;
//		else
//			pBx = Finder.GetBxNext();
//	}
//	BOOL bIsStringer = TRUE;
//	if(pBx==NULL)
//	{
//		bIsStringer = FALSE;
//		return bIsStringer;
//	}
//	pS = pBx->GetStringer(0);
//	
//	// ������ ��ȭ
//	CDPoint A[47];
//	pGir->GetHunchXyDom(pBx, A);
//
//	// �� �������� �������� ���� ��
//	CDPoint uXy2(0,1);
//
//	CDPoint xyResultFir, xyResultSec;
//
//	GetXyMatchSegAndLine(A[10], A[11], A[12], uXy2, xyResultFir);
//
//	double dChgSlabFirH = xyResultFir.y - A[12].y;
//	double dChgSlabSecH = 0;
//	GetXyMatchSegAndLine(A[15], A[26], A[14], uXy2, xyResultSec);
//	dChgSlabSecH = xyResultSec.y - A[14].y;
//
//	/************************************************************************/
//	/* ������ �� ����                                                       */
//	/************************************************************************/
//	// ����� ��ȭ ���� ����
//	double dChgSlabFirW		= dChgSlabFirH * 3;
//	double dChgSlabSecW		= dChgSlabSecH * 3;
//	double dChgSlabAverW	= (dChgSlabFirW + dChgSlabSecW) / 2;
//	double dChgSlabAverH	= (dChgSlabFirH + dChgSlabSecH) / 2;
//	double dSlabW			= pDB->m_dWidthHunch;
//	double dStringerUW		= pS->m_uM;
//	double dWidthBottom		= (dSlabW + dStringerUW)*2;	// �¿���ġ �߾ӱ�����
//
//	// ������� ���� ����
//	double dPave = pDB->GetThickPave();
//	double dSlabH = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
//	double dSlabAllW = dStringerUW+dSlabW+dChgSlabAverW;	//��ü �������� ��
//
//		// ������ �׸���
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);
//
//	Dom.MoveTo(0,0);
//	Dom.LineTo(-dStringerUW, 0);
//	Dom.LineToEx(-dSlabW,0);
//	Dom.LineToEx(-dChgSlabAverW, dChgSlabAverH);
//	Dom.LineToEx(-nViewAddLen, 0);
//
//	Dom.MoveTo(0, 0);
//	Dom.LineTo(dStringerUW, 0);
//	Dom.LineToEx(dSlabW, 0);
//	Dom.LineToEx(dChgSlabAverW, dChgSlabAverH);
//	Dom.LineToEx(nViewAddLen, 0);
//	
//	// ����
//	m_pDataMng->GetOptionStd()->SetEnvLType(&Dom, LT_SOLID);
//	
//	double dPaveTopH = dChgSlabAverH + dSlabH + dPave;
//	double dPaveBotH = dChgSlabAverH + dSlabH;
//	Dom.Solid(-dSlabAllW-nViewAddLen, dPaveTopH, dSlabAllW+nViewAddLen, dPaveTopH, 
//		dSlabAllW+nViewAddLen, dPaveBotH, -dSlabAllW-nViewAddLen, dPaveBotH);
//
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CUTL);
//	// ���ܸ�
//	Dom.CutLightning(-dSlabAllW-nViewAddLen, dChgSlabAverH-Dom.Always(1),
//		-dSlabAllW-nViewAddLen, dChgSlabAverH+dSlabH+dPave+Dom.Always(1));
//	Dom.CutLightning(dSlabAllW+nViewAddLen, dChgSlabAverH-Dom.Always(1),
//		dSlabAllW+nViewAddLen, dChgSlabAverH+dSlabH+dPave+Dom.Always(1));
//
//	/************************************************************************/
//	/* ���κ� ����ö��, ��ö�� ǥ��                                       */
//	/************************************************************************/
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);
//	
//	// �Ϻ���ö��
//	CDPoint xyStt(-dSlabAllW-nViewAddLen, dChgSlabAverH+pDeckData->m_dCoverDn);
//	CDPoint xyEnd( dSlabAllW+nViewAddLen, dChgSlabAverH+pDeckData->m_dCoverDn);
//	Dom.LineTo(xyStt, xyEnd);
//	// �����ö��
//	xyStt.y = xyEnd.y += (dSlabH-pDeckData->m_dCoverUp-pDeckData->m_dCoverDn); 
//	Dom.LineTo(xyStt, xyEnd);
//	double dUpperRebarEle = xyEnd.y; 
//	
//	// ���κ� ����ö�� : S1
//	double dChgRebarAverW;			// �¿��� ����ö���� �� -----> ����� ���
//	//	Left
//	CDPoint uLineLeft(-dChgSlabAverW, dChgSlabAverH);
//	CDPoint xyLeftRebar(-dStringerUW-dSlabW, pDeckData->m_dCoverDn);
//	GetXyMatchSegAndLine(xyStt, xyEnd, xyLeftRebar, uLineLeft, xyResultFir);
//	Dom.LineTo(xyResultFir, xyLeftRebar);
//	dChgRebarAverW = xyLeftRebar.x - xyResultFir.x;
//	CDPoint xyS1Result;				// S1 ǥ�� ����Ʈ
//	CDPoint xyS1Temp(-dStringerUW-dSlabW-dChgSlabAverW*1.5, 0);
//	GetXyMatchSegAndLine(xyResultFir, xyLeftRebar, xyS1Temp, uXy2, xyS1Result);
//	CDPoint xyS3Result(-dStringerUW-dSlabW-dChgSlabAverW*1.5, dUpperRebarEle);
//	//	Right
//	CDPoint uLineRight(dChgSlabAverW, dChgSlabAverH);
//	CDPoint xyRightRebar(dStringerUW+dSlabW, pDeckData->m_dCoverDn);
//	GetXyMatchSegAndLine(xyStt, xyEnd, xyRightRebar, uLineRight, xyResultSec);
//	Dom.LineTo(xyRightRebar, xyResultSec);
//	//	Center
//	Dom.LineTo(xyLeftRebar, xyRightRebar);
//	//	��ġö�� : S2
//	DeckDraw.DrawPointRebar(&Dom, xyLeftRebar, REBARPOS_UP);
//	DeckDraw.DrawPointRebar(&Dom, xyRightRebar, REBARPOS_UP);
//
//	/************************************************************************/
//	/* ���ö��(Dot Type) ǥ��                                                */
//	/************************************************************************/
//	double dSupportRebarCTC = pDeckData->m_dGenSupportRebarCTC[2];
//	DeckDraw.DrawPointRebarByLine(&Dom, xyStt, xyEnd, dSupportRebarCTC, REBARPOS_UP, 1);
//	xyStt.y = xyEnd.y -= (dSlabH-pDeckData->m_dCoverUp-pDeckData->m_dCoverDn); 
//	DeckDraw.DrawPointRebarByLine(&Dom, xyStt, xyEnd, dSupportRebarCTC, REBARPOS_DOWN, 1);
//
//	/************************************************************************/
//	/* ġ����                                                               */
//	/************************************************************************/
//	CString str;
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
//
//	// ū���� ������ ����
//	double dChgRebarFirW = dChgRebarAverW - dChgSlabAverW + dChgSlabFirW;
//	double dChgRebarSecW = dChgRebarAverW - dChgSlabAverW + dChgSlabSecW;
//
//	GetSwapByMin(dChgRebarFirW, dChgRebarSecW);
//	GetSwapByMin(dChgSlabFirH, dChgSlabSecH);
//
//	// ������ �Ϲ� ����
//	Dom.SetDirection("TOP");
//	long nCol = 0;
//	Dom.DimMoveTo(-dSlabAllW, dUpperRebarEle+pDeckData->m_dCoverUp+dPave, nCol);
//	if(dChgSlabFirW == dChgSlabSecW)
//		str.Format("%s", COMMA(dChgSlabFirW));
//	else
//		str.Format("%s~%s", COMMA(dChgSlabFirW), COMMA(dChgSlabSecW));
//	Dom.DimLineTo(dChgSlabAverW, nCol, str);
//	Dom.DimLineToExtend(dSlabW, nCol, "", COMMA(dSlabW));
//	Dom.DimLineTo(dStringerUW, nCol, COMMA(dStringerUW));
//	Dom.DimLineTo(dStringerUW, nCol, COMMA(dStringerUW));
//	Dom.DimLineToExtend(dSlabW, nCol, "", COMMA(dSlabW));
//	Dom.DimLineTo(dChgSlabAverW, nCol, str);
//
//	Dom.SetDirection("RIGHT");
//	Dom.DimMoveTo(dSlabAllW+nViewAddLen, 0, nCol);
//	if(dChgSlabFirH == dChgSlabSecH)
//		str.Format("%s", COMMA(dChgSlabFirH));
//	else
//		str.Format("%s~%s", COMMA(dChgSlabFirH), COMMA(dChgSlabSecH));
//	Dom.DimLineTo(dChgSlabAverH, nCol, str);
//	Dom.DimLineToExtend(dSlabH, nCol, "", COMMA(dSlabH));
//	Dom.DimLineTo(dPave, nCol, COMMA(dPave));
//
//	// ������ ��� ����
//	nCol = 2;
//	Dom.SetDirection("BOTTOM");
//	Dom.DimMoveTo(-dStringerUW-dSlabW-dChgRebarAverW, 0, nCol);
//	if(dChgRebarFirW == dChgRebarSecW)
//		str.Format("%s", COMMA(dChgRebarFirW));
//	else
//		str.Format("%s~%s", COMMA(dChgRebarFirW), COMMA(dChgRebarSecW));
//	Dom.DimLineTo(dChgRebarAverW, nCol, str);
//	Dom.DimLineToExtend(dWidthBottom, nCol, "", COMMA(dWidthBottom));
//	Dom.DimLineTo(dChgRebarAverW, nCol, str);
//
//	Dom.SetDirection("LEFT");
//	Dom.DimMoveTo(-dSlabAllW-nViewAddLen, 0, 0);
//	if(dChgSlabFirH == dChgSlabSecH)
//		str.Format("%s", COMMA(dChgSlabFirH));
//	else
//		str.Format("%s~%s", COMMA(dChgSlabFirH), COMMA(dChgSlabSecH));
//	Dom.DimLineTo(dChgSlabAverH, 0, str);
//	Dom.DimLineToExtend(dSlabH, 0, "", COMMA(dSlabH));
//	Dom.DimLineToOver(dPave, 0, COMMA(dPave));
//
//	// S1, S2, S3 ���ü�
//	CRebar *pRB		= pDeckData->GetRebarByName("���κ���ġ����");
//	if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyS1Result, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
//
//	pRB		= pDeckData->GetRebarByName("���κ��ϸ麸��");
//	if(pRB)	SymDom.RebarMarkPointLower(&Dom, xyRightRebar+CDPoint(0,1)*REBAR_SYMBOL_RADIUS, dWidthBottom, 0, pRB->m_sMark, pRB->GetStringDia(), FALSE, 0);
//	if(pDeckData->m_bInstallStringerUpperBogangRebar)
//	{
//		pRB		= pDeckData->GetRebarByName("���κ���麸��");
//		if(pRB)	SymDom.RebarMarkLineUpperEx(&Dom, xyS3Result, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
//	}
//	
//	if(dPave>0)
//	{
//		Dom.SetDirection("TOP");
//		m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
//		Dom.GiSiArrowExtend(dSlabAllW+nViewAddLen-Dom.Always(3), dChgSlabAverH+dSlabH+dPave,Dom.Always(10),Dom.Always(20),2,_T("����"),_T("T = "+COMMA(dPave)+" mm"));		
//	}                                          
//	*pDom << Dom;
//
//	return bIsStringer;

	return FALSE;
}


// ���κ� ��ġ�� �ܸ� ��
// dDotRebar : ��ö��ǥ�� ����
void CDrawDBFrontRebar::DrawSangseOutBracket(CDomyun *pDom)
{
//	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
//	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();	
//	CDrawDBDeck DeckDraw(m_pDataMng);
//	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
//	
//	CConcSymbol		SymDom(pOpt);
//	CDomyun Dom(pDom);
//
//	long nViewAddLen = 700;
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
//
//	BOOL bLeft = TRUE;
//	if (pDB->GetLBracketsu()==0)	bLeft = FALSE;
//	
//	long m_nG = bLeft?0:pDB->GetGirdersu()-1;
//	CPlateBasicIndex	*pBx = pDeckData->GetBxBySection(SECTION_CEN, m_nG);
//
//	CBracket *pBrk = NULL;
//	CPlateGirderApp *pGir = pDB->GetGirder(m_nG);
//	CPlateBxFinder Finder(pGir);
//	while(pBx)
//	{
//		if(pBx->GetBracket(bLeft) == NULL)
//			pBx = Finder.GetBxNext();
//		else
//			break;
//	}
//
//	if(pBx->GetBracket(bLeft)==NULL)	return;
//	
//	pBrk = pBx->GetBracket(bLeft);
//	// 
//	CDPoint A[10];
//	CDPoint uXy2(0,1);
//	CDPoint xyResultFir(0,0), xyResultSec(0,0);
//
//	if(pBrk)
//	{
//		pBx = pBrk->GetBx();
//		pDB->GetXyFrontCantilever(pBx, A, bLeft);
//		GetXyMatchSegAndLine(A[3], A[4], A[0], uXy2, xyResultFir);
//	}
//
//	double dChgSlabFirH = pBrk ? xyResultFir.y-A[0].y : 0;
//	double dChgSlabSecH = 0;
//	GetXyMatchSegAndLine(A[3], A[4], A[5], uXy2, xyResultSec);
//	dChgSlabSecH = xyResultSec.y-A[5].y;
//
//	/************************************************************************/
//	/* ������ �� ����                                                       */
//	/************************************************************************/
//	// ����� ��ȭ ���� ����
//	double dChgSlabFirW		= dChgSlabFirH * 3;
//	double dChgSlabSecW		= dChgSlabSecH * 3;
//	double dChgSlabAverW	= (dChgSlabFirW + dChgSlabSecW) / 2;
//	double dChgSlabAverH	= (dChgSlabFirH + dChgSlabSecH) / 2;
//	double dSlabW			= pDB->m_dWidthHunch;
//	double dCrossUW			= pBrk ? pBrk->m_uM_EB : 0;
//	double dWidthBottom		= (dSlabW + dCrossUW)*2;	// �¿���ġ �߾ӱ�����
//
//	// ������� ���� ����
//	double dPave  = pDB->GetThickPave();
//	double dSlabH = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
//	double dSlabAllW = dCrossUW+dSlabW+dChgSlabAverW;	//��ü �������� ��
//
//	// ������ �׸���
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);
//
//	Dom.MoveTo(0,0);
//	Dom.LineToEx(-dCrossUW,0);
//	Dom.LineToEx(-dSlabW,0);
//	Dom.LineToEx(-dChgSlabAverW, dChgSlabAverH);
//	Dom.LineToEx(-nViewAddLen, 0);
//
//	Dom.MoveTo(0,0);
//	Dom.LineToEx(dCrossUW,0);
//	Dom.LineToEx(dSlabW, 0);
//	Dom.LineToEx(dChgSlabAverW, dChgSlabAverH);
//	Dom.LineToEx(nViewAddLen, 0);
//
//	// ����
//	m_pDataMng->GetOptionStd()->SetEnvLType(&Dom, LT_SOLID);
//
//	double dPaveTopH = dChgSlabAverH + dSlabH + dPave;
//	double dPaveBotH = dChgSlabAverH + dSlabH;
//	Dom.Solid(-dSlabAllW-nViewAddLen, dPaveTopH, dSlabAllW+nViewAddLen, dPaveTopH, 
//		dSlabAllW+nViewAddLen, dPaveBotH, -dSlabAllW-nViewAddLen, dPaveBotH);
//
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CUTL);
//	// ���ܸ�
//	Dom.CutLightning(-dSlabAllW-nViewAddLen, dChgSlabAverH-Dom.Always(1),
//		-dSlabAllW-nViewAddLen, dChgSlabAverH+dSlabH+dPave+Dom.Always(1));
//	Dom.CutLightning(dSlabAllW+nViewAddLen, dChgSlabAverH-Dom.Always(1),
//		dSlabAllW+nViewAddLen, dChgSlabAverH+dSlabH+dPave+Dom.Always(1));
//
//	/************************************************************************/
//	/* ���κ� ����ö��, ���ö�� ǥ��                                       */
//	/************************************************************************/
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);
//	
//	// �Ϻι��ö��
//	CDPoint xyStt(-dSlabAllW-nViewAddLen, dChgSlabAverH+pDeckData->m_dCoverDn);
//	CDPoint xyEnd( dSlabAllW+nViewAddLen, dChgSlabAverH+pDeckData->m_dCoverDn);
//	Dom.LineTo(xyStt, xyEnd);
//	// ��ι��ö��
//	xyStt.y = xyEnd.y += (dSlabH-pDeckData->m_dCoverUp-pDeckData->m_dCoverDn); 
//	Dom.LineTo(xyStt, xyEnd);
//	double dUpperRebarEle = xyEnd.y; 
//	
//	// ���κ� ����ö�� : C1
//	double dChgRebarAverW;			// �¿��� ����ö���� �� -----> ����� ���
//	//	Left
//	CDPoint uLineLeft(-dChgSlabAverW, dChgSlabAverH);
//	CDPoint xyLeftRebar(-dCrossUW-dSlabW, pDeckData->m_dCoverDn);
//	GetXyMatchSegAndLine(xyStt, xyEnd, xyLeftRebar, uLineLeft, xyResultFir);
//	Dom.LineTo(xyResultFir, xyLeftRebar);
//	dChgRebarAverW = xyLeftRebar.x - xyResultFir.x;
//	CDPoint xyC1Result;				// C1 ǥ�� ����Ʈ
//	CDPoint xyC1Temp(-dCrossUW-dSlabW-dChgSlabAverW*1.5, 0);
//	GetXyMatchSegAndLine(xyResultFir, xyLeftRebar, xyC1Temp, uXy2, xyC1Result);
//	CDPoint xyC3Result(-dCrossUW-dSlabW-dChgSlabAverW*1.5, dUpperRebarEle);
//	//	Right
//	CDPoint uLineRight(dChgSlabAverW, dChgSlabAverH);
//	CDPoint xyRightRebar(dCrossUW+dSlabW, pDeckData->m_dCoverDn);
//	GetXyMatchSegAndLine(xyStt, xyEnd, xyRightRebar, uLineRight, xyResultSec);
//	Dom.LineTo(xyRightRebar, xyResultSec);
//	//	Center
//	Dom.LineTo(xyLeftRebar, xyRightRebar);
//	//	��ġö�� : C2
//	DeckDraw.DrawPointRebar(&Dom, xyLeftRebar, REBARPOS_UP);
//	DeckDraw.DrawPointRebar(&Dom, xyRightRebar, REBARPOS_UP);
//
//	/************************************************************************/
//	/* ��ö��(Dot Type) ǥ��                                                */
//	/************************************************************************/
//	double dMainRebarCTC = pDeckData->m_dGenMainRebarCTC[2]/2;
//	DeckDraw.DrawPointRebarByLine(&Dom, xyStt, xyEnd, dMainRebarCTC, REBARPOS_UP, 1);
//	xyStt.y = xyEnd.y -= (dSlabH-pDeckData->m_dCoverUp-pDeckData->m_dCoverDn); 
//	DeckDraw.DrawPointRebarByLine(&Dom, xyStt, xyEnd, dMainRebarCTC, REBARPOS_DOWN, 1);
//
//	/************************************************************************/
//	/* ġ����                                                               */
//	/************************************************************************/
//	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
//
//	// ū���� ������ ����
//	double dChgRebarFirW = dChgRebarAverW - dChgSlabAverW + dChgSlabFirW;
//	double dChgRebarSecW = dChgRebarAverW - dChgSlabAverW + dChgSlabSecW;
//
//	GetSwapByMin(dChgRebarFirW, dChgRebarSecW);
//	GetSwapByMin(dChgSlabFirH, dChgSlabSecH);
//
//	// ������ �Ϲ� ����
//	long nCol = 0;
//	CString str;
//	Dom.SetDirection("TOP");
//	Dom.DimMoveTo(-dSlabAllW, dUpperRebarEle+pDeckData->m_dCoverUp+dPave, nCol);
//	if(dChgSlabFirW == dChgSlabSecW)
//		str.Format("%s", COMMA(dChgSlabFirW));
//	else
//		str.Format("%s~%s",COMMA(dChgSlabFirW), COMMA(dChgSlabSecW));
//	Dom.DimLineTo(dChgSlabAverW, nCol, str);
//	Dom.DimLineToExtend(dSlabW, nCol, "", COMMA(dSlabW));
//	Dom.DimLineTo(dCrossUW, nCol, COMMA(dCrossUW));
//	Dom.DimLineTo(dCrossUW, nCol, COMMA(dCrossUW));
//	Dom.DimLineToExtend(dSlabW, nCol, "", COMMA(dSlabW));
//	Dom.DimLineTo(dChgSlabAverW, nCol, str);
//
//	Dom.SetDirection("RIGHT");
//	Dom.DimMoveTo(dSlabAllW+nViewAddLen, 0, nCol);
//	if(dChgSlabFirH == dChgSlabSecH)
//		str.Format("%s", COMMA(dChgSlabFirH));
//	else
//		str.Format("%s~%s",COMMA(dChgSlabFirH), COMMA(dChgSlabSecH));
//	Dom.DimLineTo(dChgSlabAverH, nCol, str);
//	Dom.DimLineToExtend(dSlabH, nCol,  "", COMMA(dSlabH));
//	Dom.DimLineTo(dPave, nCol, COMMA(dPave));
//
//	// ������ ��� ����
//	nCol = 2;
//	Dom.SetDirection("BOTTOM");
//	Dom.DimMoveTo(-dCrossUW-dSlabW-dChgRebarAverW, 0, nCol);
//	if(dChgRebarFirW == dChgRebarSecW)
//		str.Format("%s", COMMA(dChgRebarFirW));
//	else
//		str.Format("%s~%s",COMMA(dChgRebarFirW), COMMA(dChgRebarSecW));
//	Dom.DimLineTo(dChgRebarAverW, nCol, str);
//	Dom.DimLineToExtend(dWidthBottom, nCol, "", COMMA(dWidthBottom));
//	Dom.DimLineTo(dChgRebarAverW, nCol, str);
//	
//	Dom.SetDirection("LEFT");
//	Dom.DimMoveTo(-dSlabAllW-nViewAddLen, 0, 0);
//	if(dChgSlabFirH == dChgSlabSecH)
//		str.Format("%s", COMMA(dChgSlabFirH));
//	else
//		str.Format("%s~%s",COMMA(dChgSlabFirH), COMMA(dChgSlabSecH));
//	Dom.DimLineTo(dChgSlabAverH, 0, str);
//	Dom.DimLineToExtend(dSlabH, 0, "", COMMA(dSlabH));
//	Dom.DimLineToOver(dPave, 0, COMMA(dPave));
//
//	// C1, C2, C3 ���ü�
//	CString sText = _T("");
//	CRebar *pRB		= pDeckData->GetRebarByName("�������κ���ġ����");
//	if(pRB)		SymDom.RebarMarkLineLowerEx(&Dom, xyC1Result, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
//
//	pRB		= pDeckData->GetRebarByName("�������κ��ϸ麸��");
//	if(pRB)		SymDom.RebarMarkPointLower(&Dom, xyRightRebar+CDPoint(0,1)*REBAR_SYMBOL_RADIUS, dWidthBottom, 0, pRB->m_sMark, pRB->GetStringDia(), FALSE, 0);
//
//	if(pDeckData->m_bInstallCrossBeamUpperBogangRebar)
//	{
//		pRB		= pDeckData->GetRebarByName("�������κ���麸��");
//		if(pRB)		SymDom.RebarMarkLineUpperEx(&Dom, xyC3Result, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
//	}
//
//	if(dPave>0)
//	{
//		Dom.SetDirection("TOP");
//		m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
//		Dom.GiSiArrowExtend(dSlabAllW+nViewAddLen-Dom.Always(3), dChgSlabAverH+dSlabH+dPave,Dom.Always(10),Dom.Always(20),2,_T("����"),_T("T = "+COMMA(dPave)+"mm"));		
//	}
//
//	*pDom << Dom;
}


void CDrawDBFrontRebar::DrawBeryukRebar(CDomyun *pDom, long nSection)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();	
	CPlateBasicIndex		*pBx	    = pDeckData->GetBxBySection(nSection);
	CDrawDBDeck DeckDraw(m_pDataMng);
	CPlateGirderApp		*pGir		= NULL;

	CDomyun Dom(pDom);
	//////////////////////////////////////////////////////////////////////////
	// ���
	CDPointArray BaseArr	= pDeckData->GetXyArrGuideLineForMainRebar(nSection, TRUE);
	CDPointArray xyArrPoint = pDeckData->MakePointRebarArr(&BaseArr, pDeckData->GetCTCBeryukRebar(nSection, TRUE), TRUE, TRUE);
	long n = 0;
	for (n=0; n<xyArrPoint.GetSize(); n++)
		DeckDraw.DrawPointRebar(&Dom, xyArrPoint[n], REBARPOS_NONE);

	//////////////////////////////////////////////////////////////////////////
	// �ϸ�
	BaseArr	= pDeckData->GetXyArrGuideLineForMainRebar(nSection, FALSE);
	xyArrPoint = pDeckData->MakePointRebarArr(&BaseArr, pDeckData->GetCTCBeryukRebar(nSection, FALSE), TRUE, FALSE);
	for ( n=0; n<xyArrPoint.GetSize(); n++)
		DeckDraw.DrawPointRebar(&Dom, xyArrPoint[n], REBARPOS_NONE);

	//////////////////////////////////////////////////////////////////////////
	// ���κ� ��� ö��	
	if(pDeckData->IsBridgeHasStringer() && (nSection == SECTION_CEN || nSection == SECTION_PIER))	// �Ϲݺ�, ������
	{
		CDPoint A[47];
		CPlateBasicIndex *pBxSec = NULL;
		CDPointArray xyArrStg;
		for(long g = 0;g < pDB->GetGirdersu();g++)
		{
			pGir   = pDB->GetGirder(g);
			pBxSec = pBx->GetBxMatchByCrossBeam(pGir);
			pGir->GetHunchXyDom(pBxSec, A);

			for(long n=0; n<29; n++)		
			{
				if(n==12 || n==14 || n==17 || n==19 || n==22 || n==24)
					xyArrStg.Add(CDPoint(A[n].x, A[n].y+pDeckData->m_dCoverDn));
			}
		}

		for( n=0; n<xyArrStg.GetSize(); n++)
		{
			if(n==0)
				DeckDraw.DrawPointRebar(&Dom, xyArrStg[n], REBARPOS_UP_RIGHT);
			else if(n==xyArrPoint.GetSize()-1)
				DeckDraw.DrawPointRebar(&Dom, xyArrStg[n], REBARPOS_UP_LEFT);
			else
				DeckDraw.DrawPointRebar(&Dom, xyArrStg[n], REBARPOS_UP);
		}
	}

	*pDom << Dom;
}



void CDrawDBFrontRebar::DrawMarkMainRebar(CDomyun *pDom, long nSection, long nCycle, BOOL bMirrorPos)
{
	if(m_pDataMng->GetDeckData()->m_arrRebarJewon.GetSize() == 0)	return;
	
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();	
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
	CRebar			*pRB		= NULL;
	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);

	pOpt->SetEnvType(&Dom, HCAD_DIML);

	CDPointArray xyArr;
	CDPoint xyMark(0,0);
	CString	sText  = _T("");
	long	nSize  = 0;
	long	nShape = 0;

	vector <REBAR_CYCLE>* pvCycle = pDeckData->GetRebarCycleBySection(nSection);
	BOOL	bOneCycle   = pvCycle->size()==1 ? TRUE : FALSE;
	long n = 0;
	if(nSection==SECTION_DAN)	// �ܺ�
	{
		for (long nUpDn=0; nUpDn<2; nUpDn++)
		{
			BOOL bUpper = nUpDn==0 ? TRUE : FALSE;
			vector <REBAR_SHAPE> vShape = pDeckData->GetNewRebarShapeArrBySection(nSection, nCycle, bUpper);
			
			long nRebarsu = vShape.size();
			for (long n=0; n<nRebarsu; n++)
			{
				CDPointArray xyArrRebar = vShape.at(n).m_dArrShape;

				//���
				if (bUpper)
				{
					if(bOneCycle)		pDeckData->IsSkewStt()? sText.Format("�����簢��ö�ٻ��%d", n+1) : sText.Format("�����簢��ö�ٻ��%d", n+1);
					else				pDeckData->IsSkewStt()? sText.Format("�����簢��ö�ٻ��%d-%dCycle", n+1, nCycle+1) : sText.Format("�����簢��ö�ٻ��%d-%dCycle", n+1, nCycle+1);
				}
				else
				{
					if(bOneCycle)		pDeckData->IsSkewStt()? sText.Format("�����簢��ö���ϸ�%d", n+1) : sText.Format("�����簢��ö���ϸ�%d", n+1); 
					else				pDeckData->IsSkewStt()? sText.Format("�����簢��ö���ϸ�%d-%dCycle", n+1, nCycle+1) : sText.Format("�����簢��ö���ϸ�%d-%dCycle", n+1, nCycle+1);
				}

// 				if (bUpper)
// 				{
//  					if(bOneCycle)		sText.Format("�ܺλ��%d", n+1);
//  					else				sText.Format("�ܺλ��%d-%dCycle", n+1, nCycle+1);
// 				}
// 				else
// 				{
// 					if(bOneCycle)		sText.Format("�ܺ��ϸ�%d", n+1);
// 					else				sText.Format("�ܺ��ϸ�%d-%dCycle", n+1, nCycle+1);
// 				}

				pRB		= pDeckData->GetRebarByName(sText);
				xyMark	= pDeckData->GetMidPointAtPolyLine(xyArrRebar);
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB && vShape[n].m_bValid)
				{
					if(nUpDn==0)
						SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
					else
						SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
				}
			}
		}
	}
	else if(nSection==SECTION_CEN || nSection==SECTION_PIER)	// �߾Ӻ�	
	{
		// �����ö��
		if(pDeckData->IsSameMainRebarDanGen(TRUE))	// �ܺο� �߾Ӻ� ö���� ���Ͻÿ��� �ܺ�ö������ �׸�.
		{
			vector <REBAR_SHAPE> vShape = pDeckData->GetNewRebarShapeArrBySection(SECTION_DAN, nCycle, TRUE);
			
			long nRebarsu = vShape.size();
			for (long n=0; n<nRebarsu; n++)
			{
				CDPointArray xyArrRebar = vShape.at(n).m_dArrShape;
				// ���
				if(bOneCycle)		sText.Format("�ܺλ��%d", n+1);
				else				sText.Format("�ܺλ��%d-%dCycle", n+1, nCycle+1);
				pRB		= pDeckData->GetRebarByName(sText);
				xyMark  = pDeckData->GetMidPointAtPolyLine(xyArrRebar);
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB && vShape[n].m_bValid)
					SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}
		else
		{
			vector <REBAR_SHAPE> vShape = pDeckData->GetNewRebarShapeArrBySection(SECTION_CEN, nCycle, TRUE);
			
			long nRebarsu = vShape.size();
			for (long n=0; n<nRebarsu; n++)
			{
				CDPointArray xyArrRebar = vShape.at(n).m_dArrShape;
				//��� �߾Ӻ�
				if(bOneCycle)		sText.Format("�Ϲݺλ��%d", n+1);
				else				sText.Format("�Ϲݺλ��%d-%dCycle", n+1, nCycle+1);
				pRB		= pDeckData->GetRebarByName(sText);
				xyMark  = pDeckData->GetMidPointAtPolyLine(xyArrRebar);
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB && vShape[n].m_bValid)
					SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}
		
		// �ϸ���ö��
		BOOL bLeftBrk = pDeckData->IsBridgeHasOutBracket(nSection, TRUE);
		BOOL bRighBrk = pDeckData->IsBridgeHasOutBracket(nSection, FALSE);
		BOOL bNoBrk   = !bLeftBrk&&!bRighBrk ? TRUE : FALSE;
		BOOL bNoHunch = pDeckData->IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&pDB->GetGirdersu()==1) ? TRUE : FALSE;
		BOOL bTaperDiv= pDB->m_dHeightTaperStt>0 || (pDB->m_dHeightTaperStt==0&&!pDeckData->IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;
		
		vector <REBAR_SHAPE> vShape = pDeckData->GetNewRebarShapeArrBySection(SECTION_CEN, nCycle, FALSE);

		if( bNoHunch && bTaperDiv)
		{
			long nRebarsu = vShape.size();
			for (long n=0; n<nRebarsu; n++)
			{
				CDPointArray xyArrRebar = vShape.at(n).m_dArrShape;
				
				if(bOneCycle)		sText.Format("�Ϲݺ��ϸ�%d", n+1);
				else				sText.Format("�Ϲݺ��ϸ�%d-%dCycle", n+1, nCycle+1);
				pRB	   = pDeckData->GetRebarByName(sText);
				xyMark  = pDeckData->GetMidPointAtPolyLine(xyArrRebar);
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB && vShape[n].m_bValid)
					SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
			}
		}

		if(bLeftBrk && bRighBrk)
		{
			pRB	   = pDeckData->GetRebarByName("�Ϲݺο�������ġ����");
			xyArr  = pDeckData->GetXyArrPoint("��������ġ����(��)", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("��������ġ����(��)", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[2]+xyArr[3])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο������κ����");
			xyArr	= pDeckData->GetXyArrPoint("�������κ����(��)", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�������κ����(��)", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= xyArr[0] + (xyArr[1]-xyArr[0]).Unit() * ~(xyArr[1]-xyArr[0]) * 0.3;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ�����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}
		else if(bLeftBrk==FALSE && bRighBrk)
		{
			pRB		= pDeckData->GetRebarByName("�Ϲݺο�������ġ����(��)");
			xyArr	= pDeckData->GetXyArrPoint("��������ġ����(��)", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("��������ġ����(��)", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[2]+xyArr[3])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο������κ����(��)");
			xyArr	= pDeckData->GetXyArrPoint("�������κ����(��)", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�������κ����(��)", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= xyArr[0] + (xyArr[1]-xyArr[0]).Unit() * ~(xyArr[1]-xyArr[0]) * 0.3;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ�����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}
		else if(bLeftBrk && bRighBrk==FALSE)
		{
			pRB		= pDeckData->GetRebarByName("�Ϲݺο�������ġ����(��)");
			xyArr	= pDeckData->GetXyArrPoint("��������ġ����(��)", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("��������ġ����(��)", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[2]+xyArr[3])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο������κ����(��)");
			xyArr	= pDeckData->GetXyArrPoint("�������κ����(��)", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�������κ����(��)", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= xyArr[0] + (xyArr[1]-xyArr[0]).Unit() * ~(xyArr[1]-xyArr[0]) * 0.3;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ�����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}
		else if(bLeftBrk==FALSE && bRighBrk==FALSE)
		{
			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ�����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}

			pRB		= pDeckData->GetRebarByName("�Ϲݺο����Ŵ��ϸ����");
			xyArr	= pDeckData->GetXyArrPoint("�����Ŵ����", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�����Ŵ����", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}

		if(pDB->GetGirdersu() > 2)
		{
			pRB		= pDeckData->GetRebarByName("�Ϲݺ��߾ӰŴ��ϸ�");
			xyArr	= pDeckData->GetXyArrPoint("�Ϲݺ��߾ӰŴ��ϸ�(1)", vShape);
			nSize	= xyArr.GetSize();
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("�Ϲݺ��߾ӰŴ��ϸ�(1)", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[1]+xyArr[2])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}

		if(pDeckData->IsBridgeHasCrossBeam() && pDB->m_dThickSlabHunch > 0 )
		{
			pRB	= pDeckData->GetRebarByName("�Ϲݺ��߾Ӱ��κ����ϸ�");
			xyArr	= pDeckData->GetXyArrPoint("���κ������ü", vShape);
			nShape = pDeckData->GetNoShapeAtHDanShapeArr("���κ������ü", vShape);
			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
			{
				xyMark	= (xyArr[0]+xyArr[1])/2;
				if(bMirrorPos)	xyMark.x = -xyMark.x;
				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
			}
		}

		// ���κ���ġ����
//		if(pDeckData->IsBridgeHasStringer())
//		{
//			pRB	= pDeckData->GetRebarByName("���κ���ġ����");
//			CPlateBasicIndex *pBx = pDeckData->GetBxBySection(nSection);
//			pBx = pDB->GetGirder(0)->GetBxMatchByCrossBeam(pBx, pDB->GetGirder(0));
//			CPlateCrossBeam	*pC	= pBx->GetCrossBeam();
//			if(pC == NULL)
//				pC = pDB->GetGirder(0)->GetCrossBeamByCrossNo(1);
//
//			long nStg = pC->m_nQtyStringer_CR;
//			if(nStg > 0)	sText.Format("���κ�����(0-%d)", 0);
//			if(nStg > 1)	sText.Format("���κ�����(0-%d)", 1);
//			if(nStg > 2)	sText.Format("���κ�����(0-%d)", 2);
//
//			xyArr	= pDeckData->GetXyArrPoint(sText, vShape);
//			nShape = pDeckData->GetNoShapeAtHDanShapeArr("sText", vShape);
//			if(xyArr.GetSize() > 0 && vShape[nShape].m_bValid)
//			{
//				xyMark	= xyArr[1];
//				if(bMirrorPos)	xyMark.x = -xyMark.x;
//				if(pRB)	SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, TRUE);
//			}
//		}
	}
	else if(nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END)	// �纸�� ö��
	{
		// �纸��ö�� ���
		vector <REBAR_SHAPE> vShape = pDeckData->GetNewRebarShapeArrBySection(nSection, nCycle, TRUE);
		long nRebarsu = vShape.size();
		for (n=0; n<nRebarsu; n++)
		{
			CDPointArray xyArrRebar = vShape.at(n).m_dArrShape;			
			if(nSection==SECTION_SKEW_STT)
			{
				if(bOneCycle)		sText.Format("�����纸�����%d", n+1);
				else				sText.Format("�����纸�����%d-%dCycle", n+1, nCycle+1);
			}
			else
			{
				if(bOneCycle)		sText.Format("�����纸�����%d", n+1);
				else				sText.Format("�����纸�����%d-%dCycle", n+1, nCycle+1);
			}
			pRB		= pDeckData->GetRebarByName(sText);
			xyMark  = pDeckData->GetMidPointAtPolyLine(xyArrRebar);
			if(bMirrorPos)	xyMark.x = -xyMark.x;
			if(pRB && vShape[n].m_bValid)
				SymDom.RebarMarkLineUpperEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}

		// �纸��ö�� �ϸ�
		vShape = pDeckData->GetNewRebarShapeArrBySection(nSection, nCycle, FALSE);

		nRebarsu = vShape.size();
		for ( n=0; n<nRebarsu; n++)
		{
			REBAR_SHAPE Shape = vShape[n];
			CDPointArray xyArrRebar = Shape.m_dArrShape;
			
			if(nSection==SECTION_SKEW_STT)
			{
				if(bOneCycle)		sText.Format("�����纸���ϸ�%d", n+1);
				else				sText.Format("�����纸���ϸ�%d-%dCycle", n+1, nCycle+1);
			}
			else
			{
				if(bOneCycle)		sText.Format("�����纸���ϸ�%d", n+1);
				else				sText.Format("�����纸���ϸ�%d-%dCycle", n+1, nCycle+1);
			}
			pRB		= pDeckData->GetRebarByName(sText);
			xyMark  = pDeckData->GetMidPointAtPolyLine(xyArrRebar);
			if(bMirrorPos)	xyMark.x = -xyMark.x;
			if(pRB && Shape.m_bValid)
				SymDom.RebarMarkLineLowerEx(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), pRB->GetLength(), 0, FALSE);
		}
	}

	
	*pDom << Dom;
}

void CDrawDBFrontRebar::DrawMarkBeryukRebar(CDomyun *pDom, long nSection, BOOL bMirrorPos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CARoadOptionStd	*pOpt		= (CARoadOptionStd*)m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBx = pDeckData->GetBxBySection(nSection);
	
	CConcSymbol		SymDom(pOpt);
	CDomyun			Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);

	BOOL	bOneCycle	= pDeckData->m_stBeryukCycleLeft.size()==1 ? TRUE : FALSE;
	CString	sRebar		= _T("");
	
	CDPointArray xyArrPoint, xyArrTemp, BaseArr;
	long n = 0;
	//////////////////////////////////////////////////////////////////////////
	// ��� ��� ö��
	for (n=0; n<3; n++)		// 0:���� 1:�߾� 2:����
	{
		if (nSection==SECTION_DAN)
		{
			if(n==0)
			{
				if(bOneCycle)	sRebar.Format("��´ܺλ��L");
				else			sRebar.Format("��´ܺλ��L1Cycle");
			}
			else if(n==1)
			{
				if(bOneCycle)	sRebar.Format("��´ܺλ��");
				else			sRebar.Format("��´ܺλ��1Cycle");
			}
			else if(n==2)
			{
				if(bOneCycle)	sRebar.Format("��´ܺλ��R");
				else			sRebar.Format("��´ܺλ��R1Cycle");
			}
		}
		else if(nSection==SECTION_CEN)
		{
			if(n==0)
			{
				if(bOneCycle)	sRebar.Format("����Ϲݺλ��L1");
				else			sRebar.Format("����Ϲݺλ��L1-1Cycle");
			}
			else if(n==1)
			{
				if(bOneCycle)	sRebar.Format("����Ϲݺλ��1");
				else			sRebar.Format("����Ϲݺλ��1-1Cycle");
			}
			else if(n==2)
			{
				if(bOneCycle)	sRebar.Format("����Ϲݺλ��R1");
				else			sRebar.Format("����Ϲݺλ��R1-1Cycle");
			}
		}
		else if(nSection==SECTION_PIER)
		{
			if(bOneCycle)	sRebar.Format("��������λ��1");
			else			sRebar.Format("��������λ��1-1Cycle");
		}

		CRebar *pRB	= pDeckData->GetRebarByName(sRebar);

		BaseArr   = pDeckData->GetXyArrGuideLineForMainRebar(nSection, TRUE);
		xyArrTemp = pDeckData->MakePointRebarArr(&BaseArr, pDeckData->GetCTCBeryukRebar(nSection, TRUE), TRUE, TRUE);
		
		if(xyArrTemp.GetSize()==0)	return;
		
		long nRebarsuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT,   nSection==SECTION_DAN ? TRUE : FALSE, TRUE);
		long nRebarsuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, nSection==SECTION_DAN ? TRUE : FALSE, TRUE);
		long nRebarsuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH,   nSection==SECTION_DAN ? TRUE : FALSE, TRUE);
		if(nSection==SECTION_PIER)	nRebarsuL = 1;	// ������ ���� �ӽ÷� ó����.	

		if(n==0)
		{
			if(nRebarsuL > 0)
			{
				xyArrPoint.Add(xyArrTemp[0]);
				xyArrPoint.Add(xyArrTemp[2]);
				xyArrPoint.Add(xyArrTemp[4]);
			}
		}
		else if(n==1 && nSection!=SECTION_PIER)
		{
			if(nRebarsuC > 0)
			{
				xyArrPoint.Add(xyArrTemp[nRebarsuL]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+2]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+4]);
			}
		}
		else if(n==2 && nSection!=SECTION_PIER)
		{
			if(nRebarsuR > 0)
			{
				xyArrPoint.Add(xyArrTemp[nRebarsuL+nRebarsuC]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+nRebarsuC+2]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+nRebarsuC+4]);
			}
		}

		if (bMirrorPos)
		{
			for(long n=0; n<xyArrPoint.GetSize(); n++)
				xyArrPoint[n].x = -xyArrPoint[n].x;
		}

		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 0, FALSE, 0);
		xyArrPoint.RemoveAll();
	}
	//////////////////////////////////////////////////////////////////////////
	// �ϸ� ��� ö��
	for ( n=0; n<3; n++)		// 0:���� 1:�߾� 2:����
	{
		if (nSection==SECTION_DAN)
		{
			if(n==0)
				sRebar.Format("��´ܺ��ϸ�L1");
			else if(n==1)
				sRebar.Format("��´ܺ��ϸ�1");
			else if(n==2)
				sRebar.Format("��´ܺ��ϸ�R1");
		}
		else if(nSection==SECTION_CEN)
		{
			if(n==0)
			{
				if(bOneCycle)	sRebar.Format("����Ϲݺ��ϸ�L1");
				else			sRebar.Format("����Ϲݺ��ϸ�L1-1Cycle");
			}
			else if(n==1)
			{
				if(bOneCycle)	sRebar.Format("����Ϲݺ��ϸ�1");
				else			sRebar.Format("����Ϲݺ��ϸ�1-1Cycle");
			}
			else if(n==2)
			{
				if(bOneCycle)	sRebar.Format("����Ϲݺ��ϸ�R1");
				else			sRebar.Format("����Ϲݺ��ϸ�R1-1Cycle");
			}
		}
		else if(nSection==SECTION_PIER)
		{
			if(bOneCycle)	sRebar.Format("����������ϸ�1");
			else			sRebar.Format("����������ϸ�1-1Cycle");
		}

		CRebar *pRB	= pDeckData->GetRebarByName(sRebar);

		BaseArr   = pDeckData->GetXyArrGuideLineForMainRebar(nSection, FALSE);
		xyArrTemp = pDeckData->MakePointRebarArr(&BaseArr, pDeckData->GetCTCBeryukRebar(nSection, FALSE), TRUE, FALSE);
		
		if(xyArrTemp.GetSize()==0)	return;

		long nRebarsuL = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_LEFT,   nSection==SECTION_DAN ? TRUE : FALSE, FALSE);
		long nRebarsuC = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_CENTER, nSection==SECTION_DAN ? TRUE : FALSE, FALSE);
		long nRebarsuR = pDeckData->GetsuBeryukAtCantilever(POS_SLAB_RIGH,   nSection==SECTION_DAN ? TRUE : FALSE, FALSE);
		if(nSection==SECTION_PIER)	nRebarsuL = 1;	// ������ ���� �ӽ÷� ó����.	

		if(n==0)
		{
			if(nRebarsuL > 0)
			{
				xyArrPoint.Add(xyArrTemp[0]);
				xyArrPoint.Add(xyArrTemp[2]);
				xyArrPoint.Add(xyArrTemp[4]);
			}
		}
		else if(n==1 && nSection!=SECTION_PIER)
		{
			if(nRebarsuC > 0)
			{
				xyArrPoint.Add(xyArrTemp[nRebarsuL]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+2]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+4]);
			}
		}
		else if(n==2 && nSection!=SECTION_PIER)
		{
			if(nRebarsuR > 0)
			{
				xyArrPoint.Add(xyArrTemp[nRebarsuL+nRebarsuC]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+nRebarsuC+2]);
				xyArrPoint.Add(xyArrTemp[nRebarsuL+nRebarsuC+4]);
			}
		}

		if (bMirrorPos)
		{
			for(long n=0; n<xyArrPoint.GetSize(); n++)
				xyArrPoint[n].x = -xyArrPoint[n].x;
		}

		if(pRB && xyArrPoint.GetSize()>0)		SymDom.RebarMarkPoint(&Dom, xyArrPoint, pRB->m_sMark, pRB->GetStringDia(), 1, FALSE, 0);
		xyArrPoint.RemoveAll();

	}
	*pDom << Dom;
}
