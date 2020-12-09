// APlateDrawStandard.cpp: implementation of the CAPlateDrawStandard class.
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

CAPlateDrawStandard::CAPlateDrawStandard(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawStandard::~CAPlateDrawStandard()
{

}

void CAPlateDrawStandard::AddTitleOnDom(CDomyun *pDomP, double Scale, CDRect DomBound, CString sTitle, BOOL bUpperPos, BOOL bDrawScale, int nQtyLine, BOOL bLeftRotate, CString sTitleDn, long sTitleDnLayerType, double dVerSpace)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CAPlateSymbolDom sDom(pDomP,m_pDataMng);

	CDPoint xy;
	CDRect rect = DomBound;
	if(rect.bottom==0 &&  rect.top==0)
	{
		pDomP->SetCalcExtRect();
		rect = pDomP->GetExtRect();
	}

	pOptStd->SetEnvType(&sDom,HCAD_SUB_TITLE);
//	double dScaleSpace = sDom.GetTextWidth(sTitle)/2-sDom.GetTextWidth(" ");
	xy.x = (rect.left+rect.right)/2;
	xy.y = bUpperPos ? rect.bottom+sDom.Always(10) : rect.top-sDom.Always(20);
	xy.y -= dVerSpace;

	sDom.DrawTitleOnTable(xy.x, xy.y, sTitle, HCAD_SUB_TITLE, TRUE, nQtyLine);
	if (!sTitleDn.IsEmpty())
	{
		pOptStd->SetEnvType(&sDom,sTitleDnLayerType);
		xy.x = (rect.left+rect.right)/2;
		xy.y -= sDom.GetTextHeight() - sDom.Always(2);
		sDom.TextOut(xy, sTitleDn);
	}
	BOOL bRightAlign = FALSE;
	if(bDrawScale)
	{		
  		CString str = _T("");
		str.Format("S = 1 : %s",COMMA(Scale));
		pOptStd->SetEnvType(&sDom,HCAD_SMALL_TITLE);
		sDom.SetTextAlignHorz(bRightAlign?TA_RIGHT:TA_LEFT);
		if(bRightAlign)
			xy.x = rect.right;
		else
			xy.x = (rect.left+rect.right)/2;

		sDom.TextOut(xy,str);
	}

	CDPoint Bxy;
	Bxy.x = (rect.left+rect.right)/2;
	Bxy.y = (rect.bottom+rect.top)/2;
	if (bLeftRotate)
	{
		sDom.Rotate(ConstPi/2, Bxy.x, Bxy.y);
		if (bUpperPos)
			sDom.Move(rect.Height()/2 - rect.Width()/2, 0);
		else
			sDom.Move(-rect.Height()/2 + rect.Width()/2, 0);
	}
	

	*pDomP << sDom;	
}

void CAPlateDrawStandard::AddTitleOnDom(CDomyun *pDomP,  double Scale, double dx, double dy, CString sTitle, BOOL bDrawScale, int nQtyLine, BOOL bLeftRotate)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CAPlateSymbolDom sDom(pDomP,m_pDataMng);

	CDPoint xy;

	pOptStd->SetEnvType(&sDom,HCAD_SUB_TITLE);
	xy.x = dx;
	xy.y = dy;
	sDom.DrawTitleOnTable(xy.x, xy.y, sTitle, HCAD_SUB_TITLE, TRUE, nQtyLine);

	if(bDrawScale)
	{		
  		CString str = _T("");
		str.Format("S = 1 : %s",COMMA(Scale));
		pOptStd->SetEnvType(&sDom,HCAD_SMALL_TITLE);
		sDom.SetCalcExtRect();
		CDRect rect = sDom.GetExtRect();
		sDom.SetTextAlignHorz(TA_LEFT);
		xy.x = dx;
		xy.y = rect.top-sDom.GetTextHeight();
		sDom.TextOut(xy,str);
	}

	if (bLeftRotate)
		sDom.Rotate(ConstPi/2, dx, dy);

	*pDomP << sDom;	
}