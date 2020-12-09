// DrawSpacer.cpp: implementation of the CDrawSpacer class.
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

CDrawSpacer::CDrawSpacer(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CDrawSpacer::~CDrawSpacer()
{

}


//정면도 
void CDrawSpacer::DrawFrontRebar(CDomyun *pDom)
{
	CSpacer* pSpacer = &m_pDataMng->GetDeckData()->m_SpacerRebar;

	CDomyun Dom(pDom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BNTB);

	double dLenA = pSpacer->GetRebarLength_A();
	double dLenB = pSpacer->GetRebarLength_B();
	double dLenC = pSpacer->GetRebarLength_C();	
	double dLenD = pSpacer->GetRebarLength_D();

	CDPoint ptStt, ptEnd, ptDot;
	ptDot = ptStt;
	Dom.MoveTo(ptDot);
	ptDot.x -= dLenA;
	ptDot.y = dLenC;
	Dom.LineTo(ptDot);
	ptDot.x += dLenA*2 + dLenB;
	ptDot.y = dLenC;
	Dom.LineTo(ptDot);
	ptDot.x -= dLenA;
	ptDot.y = ptStt.y;
	Dom.LineTo(ptDot);
	ptEnd = ptDot;

	//Draw D
	CDPoint ptAngle = ToDPointFrDegree(125);
	ptDot = ptStt + ptAngle.Unit()*dLenD;
	Dom.LineTo(ptStt, ptDot);
	ptAngle = ToDPointFrDegree(315);
	ptDot = ptEnd + ptAngle.Unit()*dLenD;
	Dom.LineTo(ptEnd, ptDot);

	*pDom << Dom;

}


//평면도 
void CDrawSpacer::DrawPlanRebar(CDomyun *pDom)
{
	CSpacer* pSpacer = &m_pDataMng->GetDeckData()->m_SpacerRebar;
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BNTB);

	double dLenA = pSpacer->GetRebarLength_A();
	double dLenB = pSpacer->GetRebarLength_B();
	double dLenD = pSpacer->GetRebarLength_D();
	
	CDPoint ptStt, ptEnd;
	ptStt.x = -dLenA;
	ptStt.y = 0;
	ptEnd.x = dLenB + dLenA;
	ptEnd.y = 0;
	Dom.LineTo(ptStt, ptEnd);
	ptStt.x = 0;
	ptStt.y = 0;
	ptEnd.x = 0;
	ptEnd.y = dLenD;
	Dom.LineTo(ptStt, ptEnd);
	ptStt.x = dLenB;
	ptStt.y = 0;
	ptEnd.x = dLenB;
	ptEnd.y = -dLenD;
	Dom.LineTo(ptStt, ptEnd);

	*pDom << Dom;
}


//정면도 치수선 
void CDrawSpacer::DimFrontRebar(CDomyun *pDom, BOOL bInput)
{
	CSpacer* pSpacer = &m_pDataMng->GetDeckData()->m_SpacerRebar;
	CDomyun Dom(pDom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	double dLen = 0;
	double dLenA = pSpacer->GetRebarLength_A();
	double dLenB = pSpacer->GetRebarLength_B();
	double dLenC = pSpacer->GetRebarLength_C();	
	int nCol = 0;
	CString str, str2;

	/************************************************************************/
	/* 상부                                                                 */
	/************************************************************************/
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-dLenA, dLenC, nCol);
	str.Format("%0.0f", dLenA);

	if(bInput == TRUE)		str2 = "A";
	else		str2 = "";
	Dom.DimLineToExtend(dLenA, nCol, str, str2);

	str.Format("%0.0f", dLenB);
	if(bInput == TRUE)		str2 = "B";
	else		str2 = "";
	Dom.DimLineToExtend(dLenB, nCol, str, str2);

	if(bInput == TRUE)		str2 = "A";
	else		str2 = "";
	str.Format("%0.0f", dLenA);
	Dom.DimLineToExtend(dLenA, nCol, str, str2);

	nCol++;
	Dom.DimMoveTo(-dLenA, dLenC, nCol);
	dLen = dLenA*2 + dLenB;
	str.Format("%0.0f", dLen);
	Dom.DimLineTo(dLen, nCol, str);

	*pDom << Dom;


	/************************************************************************/
	/* 오른쪽                                                               */
	/************************************************************************/
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dLenA + dLenB, 0, 0);
	str = "VAR";
	if(bInput == TRUE)  //입력창
	{
		str2 = "C";
		Dom.DimLineToExtend(dLenC, 0, str, str2);
	}
	else  //도면 
	{
		CDPoint ptA(dLenB, 0);
		CDPoint ptB(dLenB + dLenA, dLenC);
		CDPoint ptAng = ToDPointFrDegree((ptB - ptA).Unit().GetAngleDegree() - 90);
		str2 = "";
		double dVal = pow(dLenA, 2) + pow(dLenC, 2);
		double dLen = sqrt(dVal);
		Dom.DimLineToExtend(dLen, 0, str, str2);
		Dom.Rotate(CDPoint(dLenA + dLenB, 0), ptAng);
	}

	*pDom << Dom;
}


void CDrawSpacer::DimPlanRebar(CDomyun *pDom, BOOL bInput)
{
	CSpacer* pSpacer = &m_pDataMng->GetDeckData()->m_SpacerRebar;
	CDomyun Dom(pDom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	double dLenA = pSpacer->GetRebarLength_A();
	double dLenB = pSpacer->GetRebarLength_B();
	double dLenD = pSpacer->GetRebarLength_D();
	CString str, str2;

	/************************************************************************/
	/* 오른쪽                                                               */
	/************************************************************************/
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dLenA + dLenB, -dLenD, 0);
	if(bInput == TRUE)		str2 = "D";
	else		str2 = "";
	str.Format("%0.0f", dLenD);
	Dom.DimLineToExtend(dLenD, 0, str, str2);
	Dom.DimLineToExtend(dLenD, 0, str, str2);

	str.Format("%0.0f", dLenD*2);
	Dom.DimMoveTo(dLenA + dLenB, -dLenD, 1);
	Dom.DimLineTo(dLenD*2, 1, str);

	*pDom << Dom;

}


void CDrawSpacer::MarkRebar(CDomyun *pDom)
{
	CADeckData* pDeckData = m_pDataMng->GetDeckData();
	CSpacer* pSpacer = &pDeckData->m_SpacerRebar;
	CDomyun Dom(pDom);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	double dLenA = pSpacer->GetRebarLength_A();
	double dLenC = pSpacer->GetRebarLength_C();	
	
	CDPoint ptStt, ptEnd, ptMark;
	ptStt.x = 0;
	ptStt.y = 0;
	ptEnd.x = -dLenA;
	ptEnd.y = dLenC;
	
	ptMark = (ptStt + ptEnd)/2;
//	CString strDia;
//	strDia.Format("%s%0.0f", pDeckData->GetRebarDiaType(), pSpacer->GetRebarDiameter());

	CRebar *pRB = pDeckData->GetRebarByName("간격재");
	if(pRB)	Sym.RebarMarkLineSide(&Dom, ptMark, pRB->m_sMark, pRB->GetStringDia(), TRUE);
//	else	Sym.RebarMarkLineSide(&Dom, ptMark, "E3", strDia, TRUE);

	*pDom << Dom;
}

//평면도상에 배치되어 있는 간격재를 그린다.
void CDrawSpacer::DrawPlaceRebar(CDomyun *pDom)
{
	CDomyun Dom(pDom);
	DrawGridRebar(&Dom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BNTB);
	CDPointArray ptArr = GetPosPlanSpacer();
	for(int i = 0; i < ptArr.GetSize(); i++)
		DrawPlanRebarEx(&Dom, ptArr.GetAt(i));

	DimPlaceRebar(&Dom);
	*pDom << Dom;
}


void CDrawSpacer::DrawGridRebar(CDomyun *pDom)
{
	CADeckData* pDeckData = m_pDataMng->GetDeckData();
	CSpacer* pSpacer = &pDeckData->m_SpacerRebar;
	CDomyun Dom(pDom);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	int nCntMainBar = pSpacer->GetCntMainRebarSet() + 2;
	int nCntDistBar = pSpacer->GetCntDistRebarSet() + 2;
	double dCTCMainBar = pDeckData->m_dGenMainRebarCTC[0]/2;  //주철근 CTC
	double dCTCDistBar = pDeckData->m_dGenSupportRebarCTC[0];  //배력철근 CTC
	CDPoint ptSttMain, ptEndMain;
	CDPoint ptSttDist, ptEndDist;
	double dSpace = 100;
	int i = 0;
	//주철근을 그린다. 
	for(i = 0; i < nCntMainBar; i++)
	{
		ptSttMain.x = i*dCTCMainBar;
		ptSttMain.y = -dSpace;
		ptEndMain.x = ptSttMain.x;
		ptEndMain.y = nCntDistBar*dCTCDistBar + dSpace;
		Dom.LineTo(ptSttMain, ptEndMain);
	}
	
	//배력철근을 그린다. 
	for(i = 0; i < nCntDistBar; i++)
	{
		ptSttDist.x = -dSpace;
		ptSttDist.y = i*dCTCDistBar;
		ptEndDist.x = (nCntMainBar - 1)*dCTCMainBar + dSpace;
		ptEndDist.y = ptSttDist.y;
		Dom.LineTo(ptSttDist, ptEndDist);
	}

	*pDom << Dom;
}

//평면도를 그린다. (실제 배치 방향)
void CDrawSpacer::DrawPlanRebarEx(CDomyun *pDom, CDPoint ptCen)
{
	CSpacer* pSpacer = &m_pDataMng->GetDeckData()->m_SpacerRebar;
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BNTB);

	double dLenA = pSpacer->GetRebarLength_A();
	double dLenB = pSpacer->GetRebarLength_B();
	double dLenD = pSpacer->GetRebarLength_D();
	
	double dLen = 2*dLenA+dLenB;
	CDPoint ptStt, ptEnd;
	//1. 가운데 
	ptStt.x = ptCen.x;
	ptStt.y = ptCen.y - dLen/2;
	ptEnd.x = ptCen.x;
	ptEnd.y = ptCen.y + dLen/2;
	Dom.LineTo(ptStt, ptEnd);
	//2. 하부 
	ptStt.x = ptCen.x - dLenD;
	ptStt.y = ptCen.y - dLen/2 + dLenA;
	ptEnd.x = ptCen.x;
	ptEnd.y = ptCen.y - dLen/2 + dLenA;
	Dom.LineTo(ptStt, ptEnd);
	//3. 상부 
	ptStt.x = ptCen.x;
	ptStt.y = ptCen.y + dLen/2 - dLenA;
	ptEnd.x = ptCen.x + dLenD;
	ptEnd.y = ptCen.y + dLen/2 - dLenA;
	Dom.LineTo(ptStt, ptEnd);

	*pDom << Dom;
}


CDPointArray CDrawSpacer::GetPosPlanSpacer()
{
	CADeckData* pDeckData = m_pDataMng->GetDeckData();
	CSpacer* pSpacer = &pDeckData->m_SpacerRebar;

	CDPointArray ptArrSpacer;
	int nCntMainBar = pSpacer->GetCntMainRebarSet();
	int nCntDistBar = pSpacer->GetCntDistRebarSet();
	double dCTCMainBar = pDeckData->m_dGenMainRebarCTC[0]/2;  //주철근 CTC
	double dCTCDistBar = pDeckData->m_dGenSupportRebarCTC[0];  //배력철근 CTC
	CDPoint ptSttMain, ptEndMain;
	CDPoint ptSttDist, ptEndDist;
	
	CDPoint ptSpacer[5];
	ptSpacer[0].x = dCTCMainBar/2;
	ptSpacer[0].y = dCTCDistBar;
	ptArrSpacer.Add(ptSpacer[0]);
	ptSpacer[1].x = dCTCMainBar/2;
	ptSpacer[1].y = dCTCDistBar + dCTCDistBar*nCntDistBar;
	ptArrSpacer.Add(ptSpacer[1]);
	ptSpacer[2].x = dCTCMainBar/2 + dCTCMainBar*nCntMainBar;
	ptSpacer[2].y = dCTCDistBar + dCTCDistBar*nCntDistBar;
	ptArrSpacer.Add(ptSpacer[2]);
	ptSpacer[3].x = dCTCMainBar/2 + dCTCMainBar*nCntMainBar;
	ptSpacer[3].y = dCTCDistBar;
	ptArrSpacer.Add(ptSpacer[3]);
	if(pSpacer->GetRebarCntInSquare() > 1)
	{
		ptSpacer[4].x = ((ptSpacer[0] + ptSpacer[3])/2).x;
		ptSpacer[4].y = ((ptSpacer[0] + ptSpacer[1])/2).y;
		ptArrSpacer.Add(ptSpacer[4]);
	}

	return ptArrSpacer;
}


void CDrawSpacer::DimPlaceRebar(CDomyun *pDom)
{
	CADeckData* pDeckData = m_pDataMng->GetDeckData();
	CSpacer* pSpacer = &pDeckData->m_SpacerRebar;
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	CDPointArray ptArr = GetPosPlanSpacer();
	int nSize = ptArr.GetSize();
	if(nSize <= 0)	return;

	CDPoint ptStt = ptArr.GetAt(0);
	CString str;
	
	int nCntMainBar = pSpacer->GetCntMainRebarSet();
	int nCntDistBar = pSpacer->GetCntDistRebarSet();
	double dCTCMainBar = pDeckData->m_dGenMainRebarCTC[0]/2;  //주철근 CTC
	double dCTCDistBar = pDeckData->m_dGenSupportRebarCTC[0];  //배력철근 CTC
	double dLen = 0;

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(0, ptStt.y, 0);
	dLen = nCntDistBar*dCTCDistBar;
	str.Format("%d@%0.0f=%0.0f", nCntDistBar, dCTCDistBar, dLen);
	Dom.DimLineTo(dLen, 0, str);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(ptStt.x, 0, 0);
	dLen = nCntMainBar*dCTCMainBar;
	str.Format("%d@%0.0f=%0.0f", nCntMainBar, dCTCMainBar, dLen);
	Dom.DimLineTo(dLen, 0, str);

	*pDom << Dom;
}
