// DrawSupportRFBar.cpp: implementation of the CDrawSupportRFBar class.
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

CDrawSupportRFBar::CDrawSupportRFBar(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CDrawSupportRFBar::~CDrawSupportRFBar()
{

}


void CDrawSupportRFBar::DrawRebar(CDomyun* pDom, int nType)
{
	CSupportRFBarSet* pSptRFBar = NULL;

	if(nType == 0)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarDanbu;
	else if(nType == 1)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarJijum;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARA);
	pSptRFBar->DrawRebar(&Dom);

	*pDom << Dom;
}


void CDrawSupportRFBar::DimRebar(CDomyun* pDom, int nType)
{
	CSupportRFBarSet* pSptRFBar = NULL;

	if(nType == 0)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarDanbu;
	else if(nType == 1)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarJijum;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	pSptRFBar->DimRebar(&Dom);

	*pDom << Dom;
}


void CDrawSupportRFBar::DrawDetailCircle(CDomyun* pDom, int nType)
{
	CSupportRFBarSet* pSptRFBar = NULL;
	if(nType == 0)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarDanbu;
	else if(nType == 1)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarJijum;
	
	CDomyun Dom(pDom);

	int nSize = pSptRFBar->GetSize();
	if(nSize <= 0)
		return;

	CSupportRFBar* rfBar = pSptRFBar->GetAt(nSize - 1);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DETL);
	rfBar->DrawDetailCircle(&Dom);
	*pDom << Dom;
}


void CDrawSupportRFBar::MarkRebar(CDomyun* pDom, int nType)
{
	CADeckData* pDeckData = m_pDataMng->GetDeckData();
	CSupportRFBarSet* pSptRFBar = NULL;

	if(nType == 0)  //단지점부 
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarDanbu;
	else if(nType == 1)  //중간지점부 
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarJijum;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	CConcSymbol Sym(m_pDataMng->GetOptionStd());
	CDPointArray ptArrLine;

	int nSize = pSptRFBar->GetSize();
	int i = 0;
	for(i = 0 ; i < nSize; i++)
	{
		CSupportRFBar* pBar = pSptRFBar->GetAt(i);
		CDPointArray ptArrRight = pBar->GetPosRebar(FALSE);
		ptArrLine.Add(ptArrRight.GetAt(0));
		ptArrLine.Add(ptArrRight.GetAt(1));
	}

	CDPoint ptStt, ptEnd, ptMark;
	CDPointArray ptArrMark;

	nSize = ptArrLine.GetSize();
	for(i = 0; i < nSize; i+=2)
	{
		ptStt = ptArrLine.GetAt(i);
		ptEnd = ptArrLine.GetAt(i+1);
		ptMark = (ptStt + ptEnd)/2;
		ptArrMark.Add(ptMark);
		Sym.RebarCountMarkOne(&Dom, ptMark);
	}

	CString strDia;
	strDia = pSptRFBar->GetTextRebarDia();
	nSize = ptArrMark.GetSize();
	CRebar* pRB = pDeckData->GetRebarByName("전단보강철근");
	if(nSize > 0)
	{
		ptStt = ptArrMark.GetAt(0);
		ptEnd = ptArrMark.GetAt(nSize - 1);
		Dom.LineTo(ptStt, ptEnd);
		if(pRB)
			Sym.RebarMarkLineSide(&Dom, ptStt, pRB->m_sMark, pRB->GetStringDia(), TRUE);
		else
			Sym.RebarMarkLineSide(&Dom, ptStt, "E2", strDia, TRUE);
	}

	*pDom << Dom;
}


void CDrawSupportRFBar::DrawDetail(CDomyun *pDom, int nType, double dDis)
{
	CSupportRFBarSet* pSptRFBar = NULL;
	
	if(nType == 0)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarDanbu;
	else if(nType == 1)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarJijum;

	CDomyun Dom(pDom);

	int nSize = pSptRFBar->GetSize();
	if(nSize <= 0)	return;

	CSupportRFBar* rfBar = pSptRFBar->GetAt(nSize - 1);

	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARA);
	rfBar->DrawDetail(&Dom, dDis);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_STLC);
	rfBar->DrawStud(&Dom, 10, dDis);  //10은 Stud지름 나중에 실지름으로 수정
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	rfBar->MarkStud(&Dom, dDis);

	*pDom << Dom;
}


void CDrawSupportRFBar::DrawDetailCircleText(CDomyun *pDom, int nType, CString strText)
{
	CSupportRFBarSet* pSptRFBar = NULL;
	if(nType == 0)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarDanbu;
	else if(nType == 1)
		pSptRFBar = &m_pDataMng->GetDeckData()->m_ShearRebarJijum;

	CDomyun Dom(pDom);
	int nSize = pSptRFBar->GetSize();
	if(nSize <= 0)
		return;

	CSupportRFBar* rfBar = pSptRFBar->GetAt(nSize - 1);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	rfBar->DrawDetailCircleText(&Dom, strText);
	
	*pDom << Dom;
}
