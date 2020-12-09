// DrawGuardFenceRebar.cpp: implementation of the CDrawGuardFenceRebar class.
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

CDrawGuardFenceRebar::CDrawGuardFenceRebar(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{
	
}

CDrawGuardFenceRebar::~CDrawGuardFenceRebar()
{

}

void CDrawGuardFenceRebar::DrawRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	
	DrawMainRebar(&Dom, nGuard);
	DrawDistRebar(&Dom, nGuard);
	DrawShearRebar(&Dom, nGuard);
	*pDom << Dom;
}


void CDrawGuardFenceRebar::DrawMainRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);
	pGFBar->DrawMainRebar(&Dom);
	*pDom << Dom;
}

//타입별로 배력철근을 그릴때..
void CDrawGuardFenceRebar::DrawDistRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	int nType = pGFBar->GetTypePlacingRebar();
	if(nType == 1)
	{
		CDPointArray ptArrLeft, ptArrRight;
		pGFBar->GetPosDistRebarType1(ptArrLeft, ptArrRight);
		DrawDistRebar(&Dom, ptArrLeft);
		DrawDistRebar(&Dom, ptArrRight);
	}
	else if(nType == 2)
	{
		CDPointArray ptArrLeft, ptArrRight;
		pGFBar->GetPosDistRebarType2(ptArrLeft, ptArrRight);
		DrawDistRebar(&Dom, ptArrLeft);
		DrawDistRebar(&Dom, ptArrRight);
	}
	else if(nType == 4)
	{
		vector<CDPointArray> vecDistBar;
		vecDistBar = pGFBar->GetPosDistRebarType4();
		int nSize = vecDistBar.size();
		for(int i = 0; i < nSize; i++)
		{
			CDPointArray ptArrPos = vecDistBar[i];
			DrawDistRebar(&Dom, ptArrPos);
		}
	}
	else if(nType == 5)
	{
		CDPointArray ptArrLeft, ptArrRight;
		pGFBar->GetPosDistRebarType5(ptArrLeft, ptArrRight);
		DrawDistRebar(&Dom, ptArrLeft);
		DrawDistRebar(&Dom, ptArrRight);
	}
	else if(nType == 6)
	{
		CDPointArray ptArrLeft, ptArrRight;
		pGFBar->GetPosDistRebarType6(ptArrLeft, ptArrRight);
		DrawDistRebar(&Dom, ptArrLeft);
		DrawDistRebar(&Dom, ptArrRight);
	}
	else if(nType == 7)
	{
		CDPointArray ptArrLeft, ptArrRight;
		pGFBar->GetPosDistRebarType7(ptArrLeft, ptArrRight);
		DrawDistRebar(&Dom, ptArrLeft);
		DrawDistRebar(&Dom, ptArrRight);
	}

	*pDom << Dom;

}

void CDrawGuardFenceRebar::DrawShearRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARA);

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	pGFBar->DrawShearRebar(&Dom);

	*pDom << Dom;
}

void CDrawGuardFenceRebar::DimRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	pGFBar->DimConc(&Dom);
	*pDom << Dom;
}


void CDrawGuardFenceRebar::DrawDistRebar(CDomyun *pDom, CDPointArray ptArrPos)
{
	CDomyun Dom(pDom);

	for(int i = 0; i < ptArrPos.GetSize(); i++)
	{
		CDPoint ptDot = ptArrPos.GetAt(i);
		CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, ptDot.x, ptDot.y, REBAR_SYMBOL_RADIUS);
	}

	*pDom << Dom;
}


void CDrawGuardFenceRebar::MarkRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);

	MarkMainRebar(&Dom, nGuard);
	MarkDistRebar(&Dom, nGuard);
	MarkShearRebar(&Dom, nGuard);

	*pDom << Dom;
}


void CDrawGuardFenceRebar::MarkMainRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	int nType = pGFBar->GetTypePlacingRebar();
	if(nType == 1)
		MarkMainRebarType1(&Dom, nGuard);
	else if(nType == 2)
		MarkMainRebarType2(&Dom, nGuard);
	else if(nType == 3)
		MarkMainRebarType3(&Dom, nGuard);
	else if(nType == 4)
		MarkMainRebarType4(&Dom, nGuard);
	else if(nType == 5)
		MarkMainRebarType5(&Dom, nGuard);
	else if(nType == 6)
		MarkMainRebarType6(&Dom, nGuard);
	else if(nType == 7)
		MarkMainRebarType7(&Dom, nGuard);

	*pDom << Dom;
}


void CDrawGuardFenceRebar::MarkDistRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	strSym = pGFBar->GetTextMark();
	int nCnt = pGFBar->GetCntMainRebar();
	int nPos = pGFBar->GetDirection();
	nCnt++;

	int nType = pGFBar->GetTypePlacingRebar();
	if(nType == 1 || nType == 2 || nType == 5 || nType == 6 || nType == 7)
	{
		CDPointArray ptArrFront, ptArrBack;
		CDPoint ptStt, ptEnd, ptMark, ptMid;
		if(nType == 1)
			pGFBar->GetPosDistRebarType1(ptArrFront, ptArrBack);
		else if(nType == 2)
			pGFBar->GetPosDistRebarType2(ptArrFront, ptArrBack);
		else if(nType == 5)
			pGFBar->GetPosDistRebarType5(ptArrFront, ptArrBack);
		else if(nType == 6)
			pGFBar->GetPosDistRebarType6(ptArrFront, ptArrBack);
		else if(nType == 7)
			pGFBar->GetPosDistRebarType7(ptArrFront, ptArrBack);

		if(ptArrBack.GetSize() < 2)	return;
		
		strDia = pGFBar->GetTextDistBarDia();
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		ptStt = ptArrBack.GetAt(0);
		ptEnd = ptArrBack.GetAt(1);
		ptMid = (ptStt + ptEnd)/2;
		double dDis = ~(ptStt - ptEnd);
		if(nPos == 0)	//왼쪽으로 본다.
		{
			ptMid.x += dDis/2;
			Sym.RebarMarkLineSide(&Dom, ptMid, strMark, strDia, FALSE, dDis/5, FALSE);
		}
		else if(nPos == 1)	//오른쪽으로 본다.
		{
			ptMid.x -= dDis/2;
			Sym.RebarMarkLineSide(&Dom, ptMid, strMark, strDia, TRUE, dDis/5, FALSE);
		}

		Dom.LineTo(ptStt, ptMid);
		Dom.LineTo(ptEnd, ptMid);
	}
	else if(nType == 4)
	{
		vector<CDPointArray> vecDist = pGFBar->GetPosDistRebarType4();
		CDPointArray ptArrLeft = vecDist[0];
		if(ptArrLeft.GetSize() < 2)		return;

		strDia = pGFBar->GetTextDistBarDia();
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		CDPoint ptStt = ptArrLeft.GetAt(0);
		CDPoint ptEnd = ptArrLeft.GetAt(1);
		CDPoint ptMid = (ptStt + ptEnd)/2;
		double dDis = ~(ptStt - ptEnd);
		ptMid.x -= dDis/2;
		Dom.LineTo(ptStt, ptMid);
		Dom.LineTo(ptEnd, ptMid);
		Sym.RebarMarkLineSide(&Dom, ptMid, strMark, strDia, TRUE, dDis/5, FALSE);
	}

	*pDom << Dom;
}


void CDrawGuardFenceRebar::MarkShearRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	strSym = pGFBar->GetTextMark();
	int nType = pGFBar->GetTypePlacingRebar();
	int nCnt = pGFBar->GetCntMainRebar() + 1;
	nCnt++;


	if(nType == 1 || nType == 2 || nType == 5 || nType == 6 || nType == 7)
	{
		CDPointArray ptArrBar;

		if(nType == 1)
			ptArrBar = pGFBar->GetPosShearRebarType1();
		else if(nType == 2)
			ptArrBar = pGFBar->GetPosShearRebarType2();
		else if(nType == 5)
			ptArrBar = pGFBar->GetPosShearRebarType5();
		else if(nType == 6)
			ptArrBar = pGFBar->GetPosShearRebarType6();
		else if(nType == 7)
			ptArrBar = pGFBar->GetPosShearRebarType7();


		for(int i = 0; i < ptArrBar.GetSize() ; i+=2)
		{
			CDPoint ptStt = ptArrBar.GetAt(i);
			CDPoint ptEnd = ptArrBar.GetAt(i+1);
			CDPoint ptCen = (ptStt + ptEnd)/2;
			strDia = pGFBar->GetTextShearBarDia();
			strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
			Sym.RebarMarkLineLower(&Dom, ptCen, strMark, strDia, FALSE, Dom.Always(12));
		}
	}

	*pDom << Dom;
}

void CDrawGuardFenceRebar::DimMainRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	pGFBar->DimMainRebar(&Dom);
	*pDom << Dom;
}

void CDrawGuardFenceRebar::DimDistRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	pGFBar->DimDistBar(&Dom);
	*pDom << Dom;
}

void CDrawGuardFenceRebar::DimShearRebar(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	pGFBar->DimShearRebar(&Dom);
	*pDom << Dom;
}


//번 타입 철근 Mark
void CDrawGuardFenceRebar::MarkMainRebarType1(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	CDPoint ptStt, ptEnd, ptMark;
	int nCnt = 1;
	strSym = pGFBar->GetTextMark();

	CDPointArray ptArrBar = pGFBar->GetPosMainRebarType1();
	int nPos = pGFBar->GetDirection();

	/************************************************************************/
	/* 1번 철근                                                             */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(0);
	ptStt = ptArrBar.GetAt(1);
	ptEnd = ptArrBar.GetAt(2);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);


	/************************************************************************/
	/* 2번 철근                                                             */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(1);
	ptStt = ptArrBar.GetAt(4);
	ptEnd = ptArrBar.GetAt(5);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineUpper(&Dom, ptMark, strMark, strDia, TRUE);
	else if(nPos == 1)
		Sym.RebarMarkLineUpper(&Dom, ptMark, strMark, strDia, FALSE);

	
	/************************************************************************/
	/* 3번 철근                                                             */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(2);
	ptStt = ptArrBar.GetAt(5);
	ptEnd = ptArrBar.GetAt(6);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);

	*pDom << Dom;
}


void CDrawGuardFenceRebar::MarkMainRebarType2(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia1, strDia2, strDia3, strMark, strSym;
	CDPoint ptStt, ptEnd, ptMark;
	int nCnt = 1;
	strSym = pGFBar->GetTextMark();

	vector<CDPointArray> vecBar = pGFBar->GetPosMainRebarType2();
	CDPointArray ptArrBar[3];
	strDia1 = pGFBar->GetTextMainBarDia(0);
	strDia2 = pGFBar->GetTextMainBarDia(1);
	strDia3 = pGFBar->GetTextMainBarDia(2);

	for(int i = 0; i < 3; i++)
		ptArrBar[i] = vecBar[i];

	int nPos = pGFBar->GetDirection();
	if(nPos == 0)  //왼쪽으로 본다.
	{
		//Front
		ptStt = ptArrBar[0].GetAt(1);
		ptEnd = ptArrBar[0].GetAt(2);
		ptMark = (ptStt + ptEnd)/2;
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia1, TRUE);

		//Center
		ptStt = ptArrBar[1].GetAt(0);
		ptEnd = ptArrBar[1].GetAt(1);
		ptMark = (ptStt + ptEnd)/2;
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia2, TRUE);

		//Back
		ptStt = ptArrBar[2].GetAt(1);
		ptEnd = ptArrBar[2].GetAt(2);
		ptMark = (ptStt + ptEnd)/2;
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia3, FALSE);
	}
	else if(nPos == 1)  //오른쪽으로 본다.
	{
		//Front
		ptStt = ptArrBar[0].GetAt(1);
		ptEnd = ptArrBar[0].GetAt(2);
		ptMark = (ptStt + ptEnd)/2;
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia1, FALSE);

		//Center
		ptStt = ptArrBar[1].GetAt(0);
		ptEnd = ptArrBar[1].GetAt(1);
		ptMark = (ptStt + ptEnd)/2;
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia2, FALSE);

		//Back
		ptStt = ptArrBar[2].GetAt(1);
		ptEnd = ptArrBar[2].GetAt(2);
		ptMark = (ptStt + ptEnd)/2;
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia3, TRUE);
	}

	*pDom << Dom;
}

void CDrawGuardFenceRebar::MarkMainRebarType3(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	CDPoint ptStt, ptEnd, ptMark;
	int nCnt = 1;
	strSym = pGFBar->GetTextMark();

	CDPointArray ptArrBar = pGFBar->GetPosMainRebarType3();
	strDia = pGFBar->GetTextMainBarDia(0);
	ptStt = ptArrBar.GetAt(0);
	ptEnd = ptArrBar.GetAt(1);
	double dSpace = ~(ptStt - ptEnd);
	ptMark.x = ptStt.x;
	ptMark.y = ptEnd.y + dSpace/3;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);

	*pDom << Dom;
}

void CDrawGuardFenceRebar::MarkMainRebarType4(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	CDPoint ptStt, ptEnd, ptMark;
	int nCnt = 1;
	strSym = pGFBar->GetTextMark();

	CDPointArray ptArrBar = pGFBar->GetPosMainRebarType4();
	int nSize = ptArrBar.GetSize();
	if(nSize > 0)
	{
		strDia = pGFBar->GetTextMainBarDia(0);
		ptStt = ptArrBar.GetAt(2);
		ptEnd = ptArrBar.GetAt(3);
		ptMark = (ptStt + ptEnd)/2;
		strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);
	}

	*pDom << Dom;
}

void CDrawGuardFenceRebar::MarkMainRebarType5(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	CDPoint ptStt, ptEnd, ptMark;
	int nCnt = 1;
	strSym = pGFBar->GetTextMark();

	CDPointArray ptArrBar = pGFBar->GetPosMainRebarType5();
	int nPos = pGFBar->GetDirection();

	/************************************************************************/
	/* 1번 철근 (전면부)                                                    */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(0);
	ptStt = ptArrBar.GetAt(2);
	ptEnd = ptArrBar.GetAt(3);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);

	/************************************************************************/
	/* 2번 철근  (상부)                                                     */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(1);
	ptStt = ptArrBar.GetAt(4);
	ptEnd = ptArrBar.GetAt(5);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineUpper(&Dom, ptMark, strMark, strDia, TRUE);
	else if(nPos == 1)
		Sym.RebarMarkLineUpper(&Dom, ptMark, strMark, strDia, FALSE);

	/************************************************************************/
	/* 3번 철근 (후면부)                                                   */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(2);
	ptStt = ptArrBar.GetAt(5);
	ptEnd = ptArrBar.GetAt(6);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);


	/************************************************************************/
	/* 4번 철근 (2사이클)                                                   */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(3);
	ptStt = ptArrBar.GetAt(3);
	ptEnd = ptArrBar.GetAt(8);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);

	*pDom << Dom;
}

void CDrawGuardFenceRebar::MarkMainRebarType6(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	CDPoint ptStt, ptEnd, ptMark;
	int nCnt = 1;
	strSym = pGFBar->GetTextMark();

	CDPointArray ptArrBar = pGFBar->GetPosMainRebarType6();
	int nPos = pGFBar->GetDirection();

	/************************************************************************/
	/* 1번 철근 (전면부)                                                    */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(0);
	ptStt = ptArrBar.GetAt(3);
	ptEnd = ptArrBar.GetAt(4);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);

	/************************************************************************/
	/* 2번 철근  (후면부)                                                   */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(1);
	ptStt = ptArrBar.GetAt(5);
	ptEnd = ptArrBar.GetAt(6);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);

	/************************************************************************/
	/* 3번 철근 (2사이클)                                                   */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(2);
	ptStt = ptArrBar.GetAt(3);
	ptEnd = ptArrBar.GetAt(8);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);

	*pDom << Dom;
}

void CDrawGuardFenceRebar::MarkMainRebarType7(CDomyun *pDom, int nGuard)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	CString strDia, strMark, strSym;
	CDPoint ptStt, ptEnd, ptMark;
	int nCnt = 1;
	strSym = pGFBar->GetTextMark();

	CDPointArray ptArrBar = pGFBar->GetPosMainRebarType7();
	int nPos = pGFBar->GetDirection();

	/************************************************************************/
	/* 1번 철근 (전면부)                                                    */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(0);
	ptStt = ptArrBar.GetAt(3);
	ptEnd = ptArrBar.GetAt(4);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);

	/************************************************************************/
	/* 2번 철근  (후면부)                                                   */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(1);
	ptStt = ptArrBar.GetAt(5);
	ptEnd = ptArrBar.GetAt(6);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);

	/************************************************************************/
	/* 3번 철근 (2사이클)                                                   */
	/************************************************************************/
	strDia = pGFBar->GetTextMainBarDia(2);
	ptStt = ptArrBar.GetAt(3);
	ptEnd = ptArrBar.GetAt(8);
	ptMark = (ptStt + ptEnd)/2;
	strMark.Format("%s%d-%d", strSym, nGuard+1, nCnt++);
	if(nPos == 0)  //왼쪽으로 본다.
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, FALSE);
	else if(nPos == 1)
		Sym.RebarMarkLineSide(&Dom, ptMark, strMark, strDia, TRUE);

	*pDom << Dom;
}


void CDrawGuardFenceRebar::DrawGroundLine(CDomyun *pDom, int nGuard)
{
	pDom->SetLTScale(50);

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DASH);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());

	CGuardFenceRebar* pGFBar = m_pDataMng->GetBridge()->GetGuardFence(nGuard)->GetRebar();
	pGFBar->DrawGoundLine(&Dom);

	*pDom << Dom;
}
