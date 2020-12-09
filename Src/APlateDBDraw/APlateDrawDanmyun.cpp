// APlateDrawDanmyun.cpp: implementation of the CAPlateDrawDanmyun class.
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
CString szPos[] = 
{ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };

CAPlateDrawDanmyun::CAPlateDrawDanmyun(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawDanmyun::~CAPlateDrawDanmyun()
{

} 

void CAPlateDrawDanmyun::DrawHDanDimForInput(CDomyun *pDom, long nJijum)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	Dom.SetDirection("BOTTOM");

	double dLeftX   = pDB->GetSlabLeft()->m_dGirderCenter;
	double dRightX  = pDB->GetSlabRight()->m_dGirderCenter;
	double dLengthY = 0;//pDB->GetGirder(0)->GetHeightGirderByJijum(nJijum);
	long	nG	= 0;
	for(nG=0; nG<pDB->GetQtyGirder(); nG++)
		dLengthY = max(dLengthY, pDB->GetGirder(nG)->GetHeightGirderByJijum(nJijum));

	if(dLengthY==0)
		dLengthY = pDB->m_dLengthTotalSlab/pDB->m_nQtyJigan/22+Dom.Always(4); 

	Dom.DimMoveTo(dLeftX, -dLengthY,0);
	double dLen = pDB->GetGirder(0)->m_dGirderCenter - dLeftX;
	Dom.DimLineTo(dLen,0,COMMA(dLen));
	
	for(nG=0;nG<pDB->GetGirdersu()-1;nG++)
	{
		double dGirderCenterPrev = pDB->GetGirder(nG)->m_dGirderCenter;
		double dGirderCenterNext = pDB->GetGirder(nG+1)->m_dGirderCenter;
		dLen = dGirderCenterNext - dGirderCenterPrev;
		Dom.DimLineTo(dLen,0,COMMA(dLen));
	}

	dLen = dRightX - pDB->GetGirder(pDB->GetGirdersu()-1)->m_dGirderCenter;
	Dom.DimLineTo(dLen,0,COMMA(dLen));

	Dom.DimMoveTo(dLeftX,-dLengthY,1);
	Dom.DimLineTo(fabs(dRightX-dLeftX), 1,COMMA(dRightX-dLeftX));

	Dom.SetDirection("TOP");
	Dom.SetDimLevelDis(6);
	CString szText;	
	long nDan=1;
	double dSlabCenter = pDB->GetSlabLeft()->m_dGirderCenter;						
	Dom.DimMoveTo(0,0,nDan);
	Dom.DimLineToExtend(dSlabCenter,nDan++,"SL", COMMA(dSlabCenter));
	for(nG=0;nG<pDB->GetGirdersu();nG++)
	{
		szText.Format("L%d",nG+1);
		double dGirderCenter = pDB->GetGirder(nG)->m_dGirderCenter;						
		if(dGirderCenter !=0)
		{
			Dom.DimMoveTo(0,0,nDan);
			Dom.DimLineToExtend(dGirderCenter,nDan++,szText, COMMA(dGirderCenter));
		}
	}
	dSlabCenter = pDB->GetSlabRight()->m_dGirderCenter;						
	Dom.DimMoveTo(0,0,nDan);
	Dom.DimLineToExtend(dSlabCenter,nDan++,"SR", COMMA(dSlabCenter));
	
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawHDanJewonDimForInput(CDomyun *pDom, long nG, long nJijum, CString strJijum)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();	
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nJijum);	
		
	double dFlangeTUpBase = m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper()==0 ? pDB->GetGirder(0)->GetValueBasicDefault("FLANGE_T") : 0;
	double dFlangeTDnBase = m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower()==1 ? pDB->GetGirder(0)->GetValueBasicDefault("FLANGE_T") : 0;	
	BOOL   bExistFact = pDB->GetGirder(0)->GetFactChain(G_F_U, 0) ? TRUE : FALSE;

	if(bExistFact && pBx)
	{
		dFlangeTUpBase = m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper()==0 ? pGir->GetThickFactByStation(pGir->GetBxOnJijum(nJijum)->GetStation(), G_F_U) : 0;
		dFlangeTDnBase = m_pDataMng->GetGlobalOption()->GetDesignHeigtBaseLower()==1 ? pGir->GetThickFactByStation(pGir->GetBxOnJijum(nJijum)->GetStation(), G_F_L) : 0;
	}
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	double dGirderCenter = pGir->m_dGirderCenter;
	double dHeightGir    = pGir->GetHeightGirderByJijum(nJijum);	
	double dTSlab   = 300;
	
	double dBindH  = pDB->m_BindConc.m_dHeight;
	double dBindS1 = pDB->m_BindConc.m_dDeep;
	double dBindS2 = pDB->m_BindConc.m_dDis;

	Dom.SetDirection("RIGHT");
	Dom.SetDimObq(30);
	Dom.SetDimExo(0);
	Dom.DimMoveTo(dGirderCenter, -dTSlab-dHeightGir-dFlangeTUpBase-dFlangeTDnBase-(dBindH-dBindS1-dBindS2), 0);
	Dom.DimLineToExtend(dHeightGir+dFlangeTUpBase+dFlangeTDnBase, 0, strJijum, COMMA(pGir->GetHeightGirderByJijum(nJijum,FALSE)));
	
	long nGsu = pDB->GetGirdersu();
	for(long a=0; a<nGsu; a++)
	{
		dGirderCenter = pDB->GetGirder(a)->m_dGirderCenter;
		CString str = "";
		str.Format("G%d", a+1);
		Dom.TextCircle(dGirderCenter, Dom.Always(3), str, 0, TRUE, TRUE);
	}
	
	*pDom << Dom;
}

//////////////////////////////////////////////////////////////////////
// 내부용 함수
//////////////////////////////////////////////////////////////////////
void CAPlateDrawDanmyun::DrawHDanBaseLineForInput(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CLineInfoApp *pLineApp = m_pDataMng->GetLineInfoApp();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	Dom.SetDirection("TOP");

	double dBaseX = 0, dHeight = Dom.Always(10);
	long nLineSu = pLineApp->GetLinesu();
	for(long nG=0;nG<pBridge->GetGirdersu();nG++)
	{	
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);

		pOptStd->SetEnvType(&Dom, HCAD_CENT);
		Dom.LineTo(dBaseX,dHeight,dBaseX,dHeight-3000);
		if(nLineSu > 0)
		{			
			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			if(pGir->m_dGirderCenter != 0)
			{
				Dom.GiSiArrow(dBaseX,dHeight,Dom.GetTextWidth("도로 중심(+)")*2,Dom.Always(1),0,"도로 중심(+)");
				Dom.GiSiArrow(dBaseX,dHeight,-Dom.GetTextWidth("도로 중심(-)")*2,Dom.Always(1),4,"도로 중심(-)");
			}
			Dom.TextOut(dBaseX,dHeight+Dom.GetTextHeight(), pLineApp->GetLine(pGir->m_nLineNum)->GetNameLineEach());
		}
	}
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawHDanSlabForInput(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	double dLeftX  = pDB->GetSlabLeft()->m_dGirderCenter;
	double dRightX = pDB->GetSlabRight()->m_dGirderCenter;
	double dSlabThick = pDB->m_dThickSlabBase;

	Dom.Rectangle(dLeftX, -300+dSlabThick, dRightX, -300);

	// 구속콘크리트로 인해 잘리는 슬래브 하면 표현
	if(pDB->IsTUGir())
	{
		CDRect rectBind;
		for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
		{
			double dGirderCenter = pDB->GetGirder(nG)->m_dGirderCenter;
			rectBind.left	= dGirderCenter - pDB->m_BindConc.m_dWidth/2;
			rectBind.right	= dGirderCenter + pDB->m_BindConc.m_dWidth/2;
			rectBind.top	= -290;
			rectBind.bottom	= -310;

			Dom.TrimByRect(rectBind);
		}		
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimBindConcJewonForInput(CDomyun *pDom, long nJijum)
{
	CPlateBridgeApp  *pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd	 *pOptStd	= m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetTextHeight(Dom.Always(100));
	Dom.SetDimLevelDis(Dom.Always(150));
	Dom.SetDimVLen(Dom.Always(150));
	Dom.SetDimExo(Dom.Always(30));
	Dom.SetArrowHeadSize(Dom.Always(40));

	long nG = pDB->GetQtyGirder() > 2 ? 1 : 0;
	CPlateGirderApp  *pGir	= pDB->GetGirder(nG);
	CPlateBasicIndex *pBx	= pGir->GetBxOnJijum(nJijum);

	double dFlangeWUp		= pBx ? pGir->GetWidthOnStation(pBx->GetStation(), TRUE) : pGir->GetValueBasicDefault("FLANGE_W");
	double dGirderCenter	= pGir->m_dGirderCenter;
	double dSlabLowH		= -300;	// 슬래브 하면의 높이
	double dBindW	= pDB->m_BindConc.m_dWidth;
	double dBindH	= pDB->m_BindConc.m_dHeight;
	double dBindS1	= pDB->m_BindConc.m_dDeep;
	double dBindS2	= pDB->m_BindConc.m_dDis;	
	double dFlangH	= dSlabLowH - (dBindH-dBindS1-dBindS2);
	double dHGir	= pGir->GetHeightGirderByJijum(0,FALSE)+dBindH-dBindS2;

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dGirderCenter - dBindW/2, dSlabLowH+pDB->m_dThickSlabBase, 0);
	Dom.DimLineTo(dBindW, 0, "b");

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(dGirderCenter - dBindW/2 - Dom.Always(400), dFlangH-dBindS2+dBindH, 0);
	Dom.DimLineTo(-dHGir, 0, "H'");
	Dom.DimMoveTo(dGirderCenter - dBindW/2 - Dom.Always(400), dSlabLowH+pDB->m_dThickSlabBase, 1);
	Dom.DimLineTo(-dHGir-pDB->m_dThickSlabBase+dBindS1, 1, "H");

	Dom.SetDimObq(30);
	Dom.DimMoveTo(dGirderCenter - dBindW/4, dFlangH-dBindS2, 0);
	Dom.DimLineTo(dBindH, 0, "h");

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dGirderCenter + dBindW/2, dSlabLowH, 0, "S1");
	Dom.DimLineTo(dBindS1, 0, "");	
	Dom.DimMoveTo(dGirderCenter + dFlangeWUp/2, dFlangH-dBindS2, 0, "S2");
	Dom.DimLineTo(dBindS2, 0, "");

	Dom.SetDimObq(0);
	Dom.DimMoveTo(dGirderCenter + dBindW/2 + Dom.Always(400), dSlabLowH, 0);
	Dom.DimLineTo(pDB->m_dThickSlabBase, 0, "T");

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawHDanGirderForInput(CDomyun *pDom, long nJijum, BOOL bRealHeight)
{
	CPlateBridgeApp *pDB	 = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption   *pGlopt	 = m_pDataMng->GetGlobalOption();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	long nG = pDB->GetQtyGirder() > 2 ? 1 : 0;
	CPlateGirderApp *pGir = pDB->GetGirder(nG);

	double dWebT      = pGir->GetValueBasicDefault("WEB_T");
	double dFlangeTUp = pGir->GetValueBasicDefault("FLANGE_T");
	double dFlangeTDn = pGir->GetValueBasicDefault("FLANGE_T");
	double dBindW  = pDB->m_BindConc.m_dWidth;
	double dBindH  = pDB->m_BindConc.m_dHeight;
	double dBindS1 = pDB->m_BindConc.m_dDeep;
	double dBindS2 = pDB->m_BindConc.m_dDis;
	double dSlabLowH = -300;	// 슬래브 하면의 높이
	double dFlangH = dSlabLowH - (dBindH-dBindS1-dBindS2);
	BOOL   bExistFact = pGir->GetFactChain(G_F_U, 0) ? TRUE : FALSE;
	
	for(nG=0;nG<pDB->GetGirdersu();nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nJijum);
		if(bExistFact && pBx)
		{
			dWebT      = pGir->GetThickFactByStation(pGir->GetBxOnJijum(nJijum)->GetStation(), G_W);
			dFlangeTUp = pGir->GetThickFactByStation(pGir->GetBxOnJijum(nJijum)->GetStation(), G_F_U);
			dFlangeTDn = pGir->GetThickFactByStation(pGir->GetBxOnJijum(nJijum)->GetStation(), G_F_L);
		}

		double dGirderCenter = pGir->m_dGirderCenter;
		double dHeight = pGir->GetHeightGirderByJijum(nJijum, TRUE);
		if(!bRealHeight)	dHeight	= 2300.0;
		double dUWidth = pBx ? pGir->GetWidthOnStation(pBx->GetStation(), TRUE) : pGir->GetValueBasicDefault("FLANGE_W");
		double dLWidth = pBx ? pGir->GetWidthOnStation(pBx->GetStation(), FALSE): pGir->GetValueBasicDefault("FLANGE_W");

		Dom.Rectangle(dGirderCenter - dUWidth/2, dFlangH, dGirderCenter + dUWidth/2, dFlangH-dFlangeTUp);
		Dom.Rectangle(dGirderCenter - dWebT/2,  dFlangH-dFlangeTUp, dGirderCenter + dWebT/2, dFlangH-dFlangeTUp-dHeight);
		Dom.Rectangle(dGirderCenter - dLWidth/2, dFlangH-dFlangeTUp-dHeight, dGirderCenter + dLWidth/2, dFlangH-dFlangeTUp-dFlangeTDn-dHeight);

		// 구속콘크리트
		if(pDB->IsTUGir())
		{
			Dom.LineTo(dGirderCenter - dBindW/2, dSlabLowH+dBindS1, dGirderCenter + dBindW/2, dSlabLowH+dBindS1);
			Dom.LineTo(dGirderCenter - dBindW/2, dSlabLowH+dBindS1, dGirderCenter - dBindW/2, dFlangH-dBindS2);
			Dom.LineTo(dGirderCenter + dBindW/2, dSlabLowH+dBindS1, dGirderCenter + dBindW/2, dFlangH-dBindS2);
			Dom.LineTo(dGirderCenter - dBindW/2, dFlangH-dBindS2,	dGirderCenter - dWebT/2,  dFlangH-dBindS2);
			Dom.LineTo(dGirderCenter + dBindW/2, dFlangH-dBindS2,	dGirderCenter + dWebT/2,  dFlangH-dBindS2);
		}		
	}

	if(pGlopt->GetDesignHeigtBaseUpper()==1)
		Dom.Move(0, dFlangeTUp);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawGirderAllOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	for(long nG=0;nG<pBridge->GetGirdersu();nG++)
	{
		CPlateGirderApp *pGirMatch = pBridge->GetGirder(nG);	
		CSection *pSec = pBx->GetSection();
		CPlateBasicIndex *pBxMatch = NULL;
		if(pSec->IsenDetType(SECDET_CROSSBEAM) || pSec->IsenDetType(SECDET_VBRACING))
			pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGirMatch);
		else
			pBxMatch = pGir->GetBxMatchByVStiff(pBx, pGirMatch);
		if(pGir == pGirMatch) pBxMatch = pBx;
		if(pBxMatch)
			DrawGirderOnBx(&Dom, pBxMatch);
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawGirderOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bPaintMap, long nMode)
{		
	CPlateGirderApp *pGir    = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDPoint B[10]; // 거더의 포인트

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	pGir->GetHunchXyDom(pBx,B);
	CDPoint ptMid = (B[1] + B[2])/2;		
	double dHeight = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dUWidth = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);
	double dLWidth = pGir->GetWidthOnStation(pBx->GetStation(),FALSE);
	double dUThick = pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	double dLThick = pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double dWThick = pBx->GetFactChain(G_W)->m_dFactChainThick;

	double x[12];
	double y[12];

	x[0] = ptMid.x;
	y[0] = ptMid.y;
	x[1] = ptMid.x - dUWidth/2;
	y[1] = ptMid.y + dUThick;
	x[2] = ptMid.x + dUWidth/2;
	y[2] = ptMid.y;
	x[3] = ptMid.x - dWThick/2;
	y[3] = ptMid.y;
	x[4] = ptMid.x + dWThick/2;
	y[4] = ptMid.y - dHeight;
	x[5] = ptMid.x - dLWidth/2;
	y[5] = ptMid.y - dLThick - dHeight;
	x[6] = ptMid.x + dLWidth/2;
	y[6] = ptMid.y - dHeight;

	// 상부 플랜지
	Dom.MoveTo(ptMid.x-dUWidth/2, ptMid.y);
	Dom.LineTo(ptMid.x+dUWidth/2, ptMid.y);
	Dom.LineTo(ptMid.x+dUWidth/2, ptMid.y+dUThick);
	Dom.LineTo(ptMid.x-dUWidth/2, ptMid.y+dUThick);
	Dom.LineTo(ptMid.x-dUWidth/2, ptMid.y);
	// 복부
	Dom.MoveTo(ptMid.x-dWThick/2, ptMid.y);
	Dom.LineTo(ptMid.x-dWThick/2, ptMid.y-dHeight);
	Dom.LineTo(ptMid.x+dWThick/2, ptMid.y-dHeight);
	Dom.LineTo(ptMid.x+dWThick/2, ptMid.y);
	Dom.LineTo(ptMid.x-dWThick/2, ptMid.y);
	// 하부 플랜지
	Dom.MoveTo(ptMid.x-dLWidth/2, ptMid.y-dHeight);
	Dom.LineTo(ptMid.x+dLWidth/2, ptMid.y-dHeight);
	Dom.LineTo(ptMid.x+dLWidth/2, ptMid.y-dHeight-dLThick);
	Dom.LineTo(ptMid.x-dLWidth/2, ptMid.y-dHeight-dLThick);
	Dom.LineTo(ptMid.x-dLWidth/2, ptMid.y-dHeight);

//	Dom.Rectangle(x[1], y[1], x[2], y[2]);
//	Dom.Rectangle(x[3], y[3], x[4], y[4]);
//	Dom.Rectangle(x[5], y[5], x[6], y[6]);			

	if(bPaintMap)//도장상세도에서 그리는거면...
	{
		double px[4];
		double py[4];

		x[7] = x[5] + (dLWidth/4);
		y[7] = y[5];
		x[8] = x[6] - (dLWidth/4);
		y[8] = y[5] - (dLThick*3);

		x[9] = x[5] + (dLWidth/2);
		y[9] = y[8];
		x[10]= x[9] - (dLThick*2);
		y[10]= y[9] - (dLThick*2);
		x[11]= x[9] + (dLThick*2);
		y[11]= y[10];

		Dom.Rectangle(x[7], y[7], x[8], y[8]);
		Dom.Solid(x[9], y[9], x[10], y[10], x[11], y[11], x[9], y[9]);	
		
		if(nMode ==0)
		{
			px[0] = x[1];
			py[0] = y[2] - (dHeight/2) + (dHeight/7);
			
			Dom.Rectangle(px[0], py[0], x[3], py[0]-dUThick);
			Dom.Rectangle(x[3]+dWThick, py[0], x[2], py[0]-dUThick);

			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(x[1], y[1], 0);
			Dom.DimLineTo(dUWidth, 0, "WU");
			Dom.DimMoveTo(x[3], y[3]- (dHeight/3), 0);
			Dom.DimLineToOver(dWThick, 0, "T");

			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(x[1],y[4], 0);
			Dom.DimLineTo(dHeight, 0, "H");

			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(x[5], y[5], 0);
			Dom.DimLineTo(dLWidth, 0, "WL");
			Dom.DimMoveTo(x[3]+dWThick, py[0]-dUThick, 0);
			Dom.DimLineTo((dUWidth/2)-dWThick, 0, "WH");
			
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(x[2],y[2], 0);
			Dom.DimLineToOver(dUThick, 0, "TU");
			Dom.DimMoveTo(x[6],y[5], 0);
			Dom.DimLineToOver(dLThick, 0, "TL");
			Dom.DimMoveTo(x[2],py[0]-dUThick, 0);
			Dom.DimLineToOver(dUThick, 0, "TH");

			Dom.SetDirection("TOP");
			CString	str1 = "SOLE PLATE";
			CString	str2 = "WP*T*LP";
			double gisilen = Dom.GetTextWidth(str1);
			Dom.GiSiArrowExtend(x[8], y[7], gisilen, gisilen, 1, str1, str2);
		}

		if(nMode == 1)
		{
			double dGap			= (dUWidth/2)/3;
			double dWebTHalf	= dWThick/2;

			Dom.LineTo(x[1]+dGap,y[2],x[1]+dGap,y[4]);
			Dom.LineTo(x[2]-dGap,y[2],x[2]-dGap,y[4]);
			Dom.Arc(x[3],y[3],(dWebTHalf),180,90);
			Dom.Arc(x[3]+dWThick,y[3],(dWebTHalf),0,-90);
			Dom.Arc(x[3],y[4],(dWebTHalf),180,-90);
			Dom.Arc(x[3]+dWThick,y[4],(dWebTHalf),0,90);			
		}

		if(nMode == 2)
		{
			double dGap			= (dUWidth/2)/3;
			double dWebTHalf	= dWThick/2;
			double dHV			= (dUWidth/2)-dGap-dWThick;

			Dom.LineTo(x[1]+dGap,y[2],x[1]+dGap,y[4]+dHV);
			Dom.LineTo(x[1]+dGap+dHV,y[4]);
			Dom.LineTo(x[2]-dGap,y[2],x[2]-dGap,y[4]+dHV);
			Dom.LineTo(x[2]-dGap-dHV,y[4]);

			Dom.Arc(x[3],y[3],(dWebTHalf),180,90);
			Dom.Arc(x[3]+dWThick,y[3],(dWebTHalf),0,-90);			

			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(x[2]-dGap,y[4], 0);
			Dom.DimLineTo(dHV, 0, "H2");
		}

		if(nMode == 1 || nMode == 2)
		{
			double dGap			= (dUWidth/2)/3;
			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(x[1],y[4], 0);
			Dom.DimLineTo(dHeight, 0, "HV");
			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(x[1]+dGap, y[2]-(dHeight/3), 0);
			Dom.DimLineTo(x[3]-x[1]-dGap, 0, "WV");
			Dom.DimMoveTo(x[3]+dWThick, y[2]-(dHeight/3), 0);
			Dom.DimLineTo(x[3]-x[1]-dGap, 0, "WV");
		}		
	}

	*pDom << Dom;
}

/************************
A[1]┌────────┐A[2]
A[0]└───A[4]───┘A[3]
*************************/
void CAPlateDrawDanmyun::DrawGirderBindConc(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bRebar)
{	
	CPlateGirderApp *pGir	 = pBx->GetGirder();
	CPlateBridgeApp *pDB	 = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CADeckDataTU	*pDeckDataTU = m_pDataMng->GetDeckDataTU();
	CDrawDBBaseRebarTU	DBRebarTU(m_pDataMng);

	if(!pDB->IsTUGir()) return;

	CDPoint A[10], vX(1,0), vY(0,1), xyStt, xyEnd;
	pGir->GetHunchXyDom(pBx, A, FALSE, FALSE, TRUE);
	A[3] = A[2] - CDPoint(0,1)*pDB->m_BindConc.m_dHeight;
	A[0].x = A[1].x;	A[0].y = A[3].y;
	A[2].x = A[3].x;	A[2].y = A[1].y;
	A[4] = (A[0] + A[3])/2;
	double dWebT	= pBx->GetFactChain(G_W)->m_dFactChainThick;	// 복부판

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	Dom.LineTo(A[0], A[4]-vX*dWebT/2);
	Dom.LineTo(A[3], A[4]+vX*dWebT/2);	
	Dom.LineTo(A[0], A[1]);
	Dom.LineTo(A[1], A[2]);
	Dom.LineTo(A[2], A[3]);
	*pDom << Dom;

	if(!bRebar) return;
	if(pDeckDataTU->m_stStrupShapeTU.size() < 1) return;
	
	DBRebarTU.DrawBincConcRebarAtCross(&Dom, pBx, TRUE);
	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	Dom.Move(0, -rectDom.top+A[4].y+pDeckDataTU->m_dCoverDn);
	*pDom << Dom;	
}

void CAPlateDrawDanmyun::DimSlabUpper(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CCentSeparation *pGWallL = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
	CCentSeparation *pGWallR = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pBridge->GetXyPaveUpperDom(pBx, A);//

	CDPoint B[HDANLINESU]; // 포장상단의 포인트
	pBridge->GetXyPaveDom(pBx, B);//

	double dTPave = pBridge->GetThickPave();
	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= dTPave;

	for(long cnt2=0; cnt2<HDANLINESU; cnt2++)
		B[cnt2].y -= dTPave;

	CDomyun Dom(pDom);
	if(bInput)	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	else		pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CString str1, str2;

	//////////////////////////////////////////////////////////////////////////////////
	CPlateGirderApp	 *pGirL	= pBridge->GetGirder(0);
	CPlateGirderApp	 *pGirR	= pBridge->GetGirder(pBridge->GetQtyGirder()-1);
	CPlateBasicIndex *pBxL	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirL);
	CPlateBasicIndex *pBxR	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateBxFinder FinderL(pGirL);
	CPlateBxFinder FinderR(pGirR);
	if(pBx->IsState(BX_STT_SLAB))
	{
		pBxL = FinderL.GetBxFirst(BX_STT_SLAB);
		pBxR = FinderR.GetBxFirst(BX_STT_SLAB);
	}
	if(pBx->IsState(BX_END_SLAB))
	{
		pBxL = FinderL.GetBxFirst(BX_END_SLAB);
		pBxR = FinderR.GetBxFirst(BX_END_SLAB);
	}
	double dHTaper	= pBx->GetGirder()->GetHeightTaper(pBx->GetStation());
	double dHTaperL = pBxL ? pGirL->GetHeightTaper(pBxL->GetStation()) : dHTaper;
	double dHTaperR = pBxR ? pGirR->GetHeightTaper(pBxR->GetStation()) : dHTaperL;
	double dTSlabLeft = pBridge->GetSlabThick(CPlateBridgeApp::SLAB_LEFT)  + dHTaperL;
	double dTSlabRigh = pBridge->GetSlabThick(CPlateBridgeApp::SLAB_RIGHT) + dHTaperR;
	
	//도로중심선이 슬래브우측에 있을 경우
	double dLen = fabs(pBridge->GetGirder(0)->m_dGirderCenter - pBridge->GetWidthSlabLeft());
	Dom.DimMoveTo(A[0].x,0, 0);
	Dom.DimLineTo(dLen, 0, COMMA(dLen));	
	
	//좌측면 슬래브 치수	
	if(bInput)
	{	
		Dom.SetDirection("LEFT");
		str1 = COMMA(dTSlabLeft);
		Dom.DimMoveTo(A[0].x, A[0].y - dTSlabLeft, 0, "", TRUE);
		Dom.DimLineToExtend(dTSlabLeft, 0, "T1", str1);
	}

	//우측면 슬래브 치수
	if(bInput)
	{	
		Dom.SetDirection("RIGHT");
		str1 = COMMA(dTSlabRigh);
		Dom.DimMoveTo(B[pBridge->GetQtyHDanDom(pBx)].x, B[pBridge->GetQtyHDanDom(pBx)].y - dTSlabRigh, 0, "", TRUE);
		Dom.DimLineToExtend(dTSlabRigh, 0, "T2", str1);
	}

	//슬래브 치수
	Dom.SetDirection("TOP");
	Dom.SetDimExo(10);
	double pointY;
	double dis = 1080;
	if(pGWallL && !pGWallR)
	{
		double disL = pGWallL->m_H1+pGWallL->m_H2+pGWallL->m_H3;
		pointY = A[0].y;
		dis = disL;
	}
	if(pGWallR && !pGWallL)
	{
		double disR = pGWallR->m_H1+pGWallR->m_H2+pGWallR->m_H3;
		pointY = A[pBridge->GetQtyHDanNode()].y;
		dis = disR;
	}
	if(pGWallL && pGWallR)
	{
		double disL = pGWallL->m_H1+pGWallL->m_H2+pGWallL->m_H3;
		double disR = pGWallR->m_H1+pGWallR->m_H2+pGWallR->m_H3;
		dis = (disL > disR) ? disL : disR;
		pointY = (A[0].y > A[pBridge->GetQtyHDanNode()].y) ? A[0].y : A[pBridge->GetQtyHDanNode()].y;
	}

	if(!pGWallL && !pGWallR)
	{
		pointY = 0;
	}
	Dom.DimMoveTo(A[0].x, pointY+dis, 0, "", TRUE);
	for(long n = 0; n < pBridge->GetQtyHDanNode(); n++)
	{
		str1 = COMMA(A[n+1].x - A[n].x);
		str2.Format("L%d", n + 1);
		if(bInput)	Dom.DimLineToExtend(A[n+1].x - A[n].x, 0, str2, str1);
		else		Dom.DimLineTo(A[n+1].x - A[n].x, 0, str1);
		if(n == pBridge->GetQtyHDanNode()-1)
		{
			Dom.SetDimExo(10);
			str1 = COMMA(A[n+1].x - A[0].x);
			Dom.DimMoveTo(A[0].x, pointY+dis, 1, "", TRUE);
			Dom.DimLineTo(A[n+1].x - A[0].x, 1, str1);
		}
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimSlabUserHunch(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	CDPoint B[10]; // 거더의 포인트

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("BOTTOM");

	pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetHunchXyDom(pBx,B);
	double disR = B[2].y;
	pBridge->GetGirder(0)->GetHunchXyDom(pBx,B);
	double disL = B[1].y;
	double dis  = (disL > disR) ? disR : disL;

	pBridge->GetXyPaveUpperDom(pBx, A);	
	double dBaseX = A[0].x;

	CString szText;

	Dom.DimMoveTo(dBaseX, dis, 0, "", TRUE);
	szText = "G1L";
	pBridge->GetGirder(0)->GetHunchXyDom(pBx,B);
	double dTLen = B[0].x-dBaseX-pBridge->m_dHunchArray.GetAt(0);
	Dom.DimLineTo(dTLen, 0, COMMA(dTLen));
	Dom.DimLineToExtend(pBridge->m_dHunchArray.GetAt(0), 0, szText, COMMA(pBridge->m_dHunchArray.GetAt(0)));
	dBaseX += dTLen + pBridge->m_dHunchArray.GetAt(0);

	for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{
		pBridge->GetGirder(nG)->GetHunchXyDom(pBx,B);
		long nStt = (nG==0) ? 0 : 4;
		for(long n=nStt; n<10; n++)
		{	
			if(B[n].x-dBaseX > 0 )
			{				
				if(n==4)
				{					
					szText.Format("G%dR",nG+1);
					Dom.DimLineToExtend(B[n].x-dBaseX, 0, szText, COMMA(B[n].x-dBaseX));
				}
				else if(n==6)
				{					
					szText.Format("G%dL",nG+2);
					Dom.DimLineToExtend(B[n].x-dBaseX, 0, szText, COMMA(B[n].x-dBaseX));
				}
				else
				{
					//if(B[n].x-dBaseX < 100)		Dom.DimLineToExtend(B[n].x-dBaseX, 0, "", COMMA(B[n].x-dBaseX));
					//else							Dom.DimLineTo(B[n].x-dBaseX, 0, COMMA(B[n].x-dBaseX));
					if(B[n].x-dBaseX > 100)
						Dom.DimLineTo(B[n].x-dBaseX, 0, COMMA(B[n].x-dBaseX));
					else
						Dom.DimLineTo(B[n].x-dBaseX, 0, "");
				}
				dBaseX = B[n].x;
			}			
		}
	}	
	szText.Format("G%dR",pBridge->GetGirdersu());
	Dom.DimLineToExtend(pBridge->m_dHunchArray.GetAt(pBridge->GetGirdersu()*2-1), 0, szText, COMMA(pBridge->m_dHunchArray.GetAt(pBridge->GetGirdersu()*2-1)));
	dBaseX += pBridge->m_dHunchArray.GetAt(pBridge->GetGirdersu()*2-1);	
	Dom.DimLineTo(A[pBridge->GetQtyHDanNode()].x-dBaseX, 0, COMMA(A[pBridge->GetQtyHDanNode()].x-dBaseX));
	
	Dom.SetDirection("RIGHT");
	Dom.SetDimExo(0);
	Dom.SetDimObq(45);
		
	double a =pBridge->m_dHunchArray[pBridge->m_dHunchArray.GetUpperBound()];
	CDPoint ptRight = CDPoint(B[9].x + pBridge->m_dHunchArray[pBridge->m_dHunchArray.GetUpperBound()], pBridge->GetElevationPaveDomByDis(pBx, B[9].x + pBridge->m_dHunchArray[pBridge->m_dHunchArray.GetUpperBound()]) - pBridge->m_dThickPave - pBridge->m_dThickSlabRightExt);
	Dom.DimMoveTo(ptRight.x, ptRight.y, 0);
	Dom.DimLineToExtend(pBridge->m_dThickSlabRightExt,0,"H2",COMMA(pBridge->m_dThickSlabRightExt));

	pBx = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(0),0);
	if(pBx)
	{
		pBridge->GetGirder(0)->GetHunchXyDom(pBx,B);
		CDPoint ptLeft = CDPoint(B[0].x - pBridge->m_dHunchArray[0], pBridge->GetElevationPaveDomByDis(pBx, B[0].x - pBridge->m_dHunchArray[0]) - pBridge->m_dThickPave - pBridge->m_dThickSlabLeftExt);
		Dom.DimMoveTo(ptLeft.x, ptLeft.y, 0);
		Dom.DimLineToExtend(pBridge->m_dThickSlabLeftExt,0,"H1",COMMA(pBridge->m_dThickSlabLeftExt));
	}


	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	CDPoint B[10]; // 거더의 포인트

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("BOTTOM");
	// dis와 dHeight를 구하기 위한 코드 ----------------------------------------
	double dHeightL = pBridge->GetGirder(0)->GetHeightGirderByStaAng(pBx->GetStation());
	double dHeightR = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetHeightGirderByStaAng(pBx->GetStation());
	double dHeight  = (dHeightL > dHeightR) ? dHeightL : dHeightR;

	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(pBridge->GetGirdersu()-1));
	pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetHunchXyDom(pBxMatch, B);

	double disR = B[2].y;

	pBxMatch = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(0));
	pBridge->GetGirder(0)->GetHunchXyDom(pBx,B);

	double disL = B[1].y;
	double dis  = (disL > disR) ? disR : disL;
	if(pBridge->IsTUGir())
		dis = A[0].y - pBridge->m_BindConc.m_dHeight;
	// -------------------------------------------------------------------------
	pBridge->GetXyPaveUpperDom(pBx, A);

	double dGirCenFir = (B[1].x + B[2].x)/2;
	
	Dom.DimMoveTo(A[0].x, dis-dHeight, 0, "", TRUE);
	Dom.DimLineTo(dGirCenFir-A[0].x, 0, COMMA(dGirCenFir-A[0].x));
	for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{
		pBx = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(nG));
		pBridge->GetGirder(nG)->GetHunchXyDom(pBx,B);
		double dGirCenSec = (B[7].x + B[8].x)/2;
		Dom.DimLineTo(dGirCenSec-dGirCenFir, 0, COMMA(dGirCenSec-dGirCenFir));
		dGirCenFir = dGirCenSec;
	}

	Dom.DimLineTo(A[pBridge->GetQtyHDanNode()].x-dGirCenFir, 0, COMMA(A[pBridge->GetQtyHDanNode()].x-dGirCenFir));
	
	*pDom << Dom;
}

//텍스트 없이 센터라인만 그리기 추가....070329...KB.....
void CAPlateDrawDanmyun::DimSlabCenterLine(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDimText)
{
	long nG		= -1;
	
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double	sta  = pBx->GetStation();
	double  dEle = pBridge->GetGirder(nG)->GetLine()->GetElevationPlanEl(sta);
	CString str1, str2;
	str1.Format("STA. %.3lf", toM(sta));
	str2.Format("EL.  %.3lf", toM(dEle));

	CDomyun Dom(pDom);

	Dom.SetDirection("TOP");
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	double SlabLeftX  = pBridge->GetDisSlabLeftDom(pBx);
	double WallHeight = 1500;
	double SlabMaxY   = max(pBridge->GetHeightSlabFromRoadCenterDom(pBx,pBridge->GetDisSlabLeftDom(pBx)),
			                pBridge->GetHeightSlabFromRoadCenterDom(pBx,pBridge->GetDisSlabRightDom(pBx)))
						   + WallHeight + Dom.Always(3);;
	double SlabToCenter = pBridge->GetDisRoadCenToSlabCen(pBx);
	double SlabT		= pBridge->m_dThickSlabBase;
	double gHeight		= pBridge->GetGirder(nG)->GetHeightGirderByJijum(0) + SlabT - Dom.Always(3);
	double SlabMinY		= min(pBridge->GetHeightSlabFromRoadCenterDom(pBx,pBridge->GetDisSlabLeftDom(pBx)),
			                  pBridge->GetHeightSlabFromRoadCenterDom(pBx,pBridge->GetDisSlabRightDom(pBx)))
						      - gHeight;

	if(bDimText)
	{
		Dom.TextOut(0,SlabMaxY-Dom.Always(2.5),"도로중심");
		double dTextLen = max(Dom.GetTextWidth(str1,1),Dom.GetTextWidth(str2,1));
		if(SlabLeftX < 0)	Dom.GiSiArrowExtend(0, 0, 700, dTextLen, 1, str1, str2);
		else				Dom.GiSiArrowExtend(0, 0, 700,-dTextLen, 3, str1, str2);
	}
	
	pOptStd->SetEnvType(&Dom,HCAD_CENT);
	Dom.LineTo(0,SlabMaxY,0,SlabMinY);
	Dom.LineTo(-1000, 0, 1000, 0);

	if(bDimText)
	{
		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		Dom.TextOut(SlabToCenter,SlabMaxY+Dom.Always(0.5),"교량중심");
		pOptStd->SetEnvType(&Dom,HCAD_CENT);
		Dom.LineTo(SlabToCenter,SlabMaxY+Dom.Always(3),
						SlabToCenter,SlabMinY);
	}

	///도로중심에서 교량중심(슬래브중심)까지의 이격거리
	if(fabs(SlabToCenter) > 0.001 && bDimText)
	{
		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(0, SlabMaxY-Dom.Always(24),1);
		Dom.DimLineTo(SlabToCenter,1,COMMA(SlabToCenter));
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimSlabHunchWidth(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	if(pBridge->IsTUGir()) return;

	long nG = pBx->GetNumberGirder();

	CDPoint A[10];
	pBridge->GetGirder(nG)->GetHunchXyDom(pBx, A);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDimExo(0);
	Dom.SetStartPointExtend(TRUE);
	Dom.SetDirection("BOTTOM");	
	Dom.DimMoveTo(A[6].x, A[6].y, 0);
	Dom.DimLineToOverExtend(pBridge->m_dWidthHunch,0, "W1", COMMA(pBridge->m_dWidthHunch));

	*pDom << Dom;	
}

void CAPlateDrawDanmyun::DimSlabBaseThick(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);

	double sta			 = pBx->GetStation();
	double dThickLeft    = pGir->GetThickSlabPave(sta,-pGir->GetLengthSharpOnBx(pBx, TRUE)/2,pBx->GetAngleSection());
	double dThickRight   = pGir->GetThickSlabPave(sta, pGir->GetLengthSharpOnBx(pBx, TRUE)/2,pBx->GetAngleSection());
	double dBaseThickPos = pBridge->m_dHunchDisForBaseThick;

	CDPoint A[10];
 	pGir->GetHunchXyDom(pBx, A, FALSE);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDimExo(0);
	Dom.SetStartPointExtend(TRUE);
	Dom.SetDimObq(45);

	double dWGir	 = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWMaxGir  = pGir->GetMaxWidthFlange(TRUE);
	double dWidthGap = (dWMaxGir -dWGir)/2; // 최대플랜지 폭과 현재 플랜지폭 차의 반 
		
	if(dThickLeft<dThickRight)
	{
		double dDis		= pBridge->m_BindConc.m_dHeight - pBridge->m_BindConc.m_dDeep - pBridge->m_BindConc.m_dDis;
		double dBaseY	= pBridge->IsTUGir() ? (A[1].y+dDis) : (A[0].y);
		double dThick	= pBridge->GetThickFrPaveByXy(pBx, CDPoint(A[1].x-dBaseThickPos-dWidthGap, dBaseY)) - pBridge->m_dThickPave;
		

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(A[1].x-dBaseThickPos-dWidthGap, dBaseY,0);
		Dom.DimLineToExtend(dThick/*pBridge->m_dThickSlabBase*/,0,"T",COMMA(dThick/*pBridge->m_dThickSlabBase)*/));
		Dom.DimLineToOverExtend(pBridge->m_dThickPave,0,"A",COMMA(pBridge->m_dThickPave));
		// C 치수
		Dom.SetDimObq(0);
		// W 치수

		if(!pBridge->IsTUGir())
		{
			if(dBaseThickPos>0)  
			{
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(A[1].x-dWidthGap, A[1].y,0);
				Dom.DimLineToExtend(-dBaseThickPos,0,"W",COMMA(dBaseThickPos));
			}
			if(dBaseThickPos<0)  
			{
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(A[1].x-dWidthGap, A[1].y,0);
				Dom.DimLineToExtend(-dBaseThickPos,0,"W",COMMA(fabs(dBaseThickPos)));
			}
		}
		
		//
		Dom.LineTo(A[1].x-dBaseThickPos-dWidthGap, dBaseY
			              ,A[1].x-dBaseThickPos-dWidthGap, dBaseY+dThick);
	}
	else
	{
		double dThick = pBridge->GetThickFrPaveByXy(pBx, CDPoint(A[8].x+dBaseThickPos+dWidthGap, A[8].y)) - pBridge->m_dThickPave;

		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(A[8].x+dBaseThickPos+dWidthGap, A[8].y,0);
		Dom.DimLineToExtend(dThick/*pBridge->m_dThickSlabBase*/,0,"T",COMMA(dThick/*pBridge->m_dThickSlabBase*/));
		Dom.DimLineToOverExtend(pBridge->m_dThickPave,0,"A",COMMA(pBridge->m_dThickPave));
		// C 치수
		Dom.SetDimObq(0);
		// W 치수
		if(pBridge->IsTUGir())
		{
			if(dBaseThickPos>0)  
			{
				Dom.SetDirection("TOP");
				Dom.DimMoveTo(A[8].x+dWidthGap, A[8].y,0);
				Dom.DimLineToExtend(dBaseThickPos,0,"W",COMMA(dBaseThickPos));
			}
			if(dBaseThickPos<0)  
			{
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(A[8].x+dWidthGap, A[8].y,0);
				Dom.DimLineToExtend(dBaseThickPos,0,"W",COMMA(fabs(dBaseThickPos)));
			}
		}
		
		//
		Dom.LineTo(A[8].x+dBaseThickPos+dWidthGap, A[8].y
			              ,A[8].x+dBaseThickPos+dWidthGap, A[8].y+dThick);
	}
	Dom.SetDimObq(0);

	*pDom << Dom;	
}



// 횡단 슬래브 상면과 옆면을 그림(보도까지)
void CAPlateDrawDanmyun::DrawSlabUpper(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bWalkRoad, BOOL bPave)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	
	CDPoint A[HDANLINESU], B[HDANLINESU]; // 포장상단의 포인트
	pBridge->GetXyPaveDom(pBx, A);//
	pBridge->GetXyPaveUpperDom(pBx, B);//

	double dTPave = pBridge->GetThickPave();
	for(long cnt=0; cnt<HDANLINESU; cnt++)
	{
		A[cnt].y -= dTPave;
		B[cnt].y -= dTPave;
	}

	CDomyun Dom(pDom);

	Dom.MoveTo(A[0]);
	long n = 0;
	for(n = 1; n < pBridge->GetQtyHDanDom(pBx)+1; n++)
		Dom.LineTo(A[n]);

	if(bPave && dTPave > 0 )
	{
		long QtyLine = long(dTPave / 15.0);		
		double dSpacePaveLine =  dTPave/(QtyLine-1);
		for(long nL=0; nL<QtyLine; nL++) 
		{
			Dom.MoveTo(A[0].x, A[0].y + dSpacePaveLine*(nL));

			for(long nHDanDom = 0; nHDanDom < pBridge->GetQtyHDanDom(pBx); nHDanDom++)
			{
				long nInputNode = pBridge->GetNumInputNodedeDom(pBx, nHDanDom);
				BOOL bDrawAscon = !pBridge->IsTypeGuardFenceDom(nInputNode) && pBridge->GetWallGuardTypeHDan(nInputNode)!=HT_PEOPLEROAD;
				if(bDrawAscon)		//방호벽이 없을 경우
					Dom.LineTo(A[nHDanDom+1].x, A[nHDanDom+1].y + dSpacePaveLine*(nL));
				else
					Dom.MoveTo(A[nHDanDom+1].x, A[nHDanDom+1].y + dSpacePaveLine*(nL));
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
		
	// 보도를 횡단면도에 표시
	// 슬래브 제원 입력에서는 보도에 관한 제원을 입력 받기전이므로 보도를 표시하지 않음
	//		슬래브 제원 입력 Dlg : bWalkRoad = FALSE
	//		나머지 횡단면도      : bWalkRoad = TRUE (default)
	if(bWalkRoad)
	{
		CCalcData *pCalcData = m_pDataMng->GetCalcData();

		long nTotalSize	=	pBridge->GetQtyHDanNode();
		long nHalfSize	=	(nTotalSize%2 == 0) ? nTotalSize/2 : (nTotalSize+1)/2;
		// Left
		for(n = 0; n < nHalfSize; n++)
		{
			if(pBridge->GetWallGuardTypeHDan(n) == 3)
			{
				double dLWalkRoadH	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
				Dom.MoveTo(B[n].x,		B[n].y);
				Dom.LineTo(B[n].x,		B[n].y+dLWalkRoadH);
				Dom.LineTo(B[n+1].x,	B[n+1].y+dLWalkRoadH);
				Dom.LineTo(B[n+1].x,	B[n+1].y);
			}
		}
		// Right
		for(n = nHalfSize; n<nTotalSize; n++)
		{
			if(pBridge->GetWallGuardTypeHDan(n) == 3)
			{
				double dRWalkRoadH	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
				Dom.MoveTo(B[n].x,		B[n].y);
				Dom.LineTo(B[n].x,		B[n].y+dRWalkRoadH);
				Dom.LineTo(B[n+1].x,	B[n+1].y+dRWalkRoadH);
				Dom.LineTo(B[n+1].x,	B[n+1].y);
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////
	CPlateGirderApp	 *pGirL	= pBridge->GetGirder(0);
	CPlateGirderApp	 *pGirR	= pBridge->GetGirder(pBridge->GetQtyGirder()-1);
	CPlateBasicIndex *pBxL	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirL);
	CPlateBasicIndex *pBxR	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateBxFinder FinderL(pGirL);
	CPlateBxFinder FinderR(pGirR);
	if(pBx->IsState(BX_STT_SLAB))
	{
		pBxL = FinderL.GetBxFirst(BX_STT_SLAB);
		pBxR = FinderR.GetBxFirst(BX_STT_SLAB);
	}
	if(pBx->IsState(BX_END_SLAB))
	{
		pBxL = FinderL.GetBxFirst(BX_END_SLAB);
		pBxR = FinderR.GetBxFirst(BX_END_SLAB);
	}
	double dHTaper	= pBx->GetGirder()->GetHeightTaper(pBx->GetStation());
	double dHTaperL = pBxL ? pGirL->GetHeightTaper(pBxL->GetStation()) : dHTaper;
	double dHTaperR = pBxR ? pGirR->GetHeightTaper(pBxR->GetStation()) : dHTaperL;
	double dTSlabLeft = pBridge->GetSlabThick(CPlateBridgeApp::SLAB_LEFT);
	double dTSlabRigh = pBridge->GetSlabThick(CPlateBridgeApp::SLAB_RIGHT);

	//좌측면 슬래브 그리기
	Dom.MoveTo(A[0]);
	Dom.LineTo(A[0].x, A[0].y - dTSlabLeft - dHTaperL);

	//우측면 슬래브 그리기
	long nHDansu = pBridge->GetQtyHDanDom(pBx);
	Dom.MoveTo(A[nHDansu]);
	Dom.LineTo(A[nHDansu].x, A[nHDansu].y - dTSlabRigh - dHTaperR);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawGuardWall(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	//CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CDomyun Dom(pDom);

	for(long n=0; n < pBridge->GetQtyHDanNode(); n++)
	{
		if(pBridge->IsTypeGuardFenceDom(n))  //방호벽이면
		{
			DstDrawGuardFence(&Dom, pBx, n, FALSE);
		}
	}

	*pDom << Dom;	
}

void CAPlateDrawDanmyun::DrawSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp  *pBridge	= m_pDataMng->GetBridge();	
	CPlateGirderApp  *pGirMatch	= NULL;	
	CPlateBasicIndex *pBxMatch	= NULL;
	
	CDomyun Dom(pDom);

	for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{
		pGirMatch = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGirMatch);
		pBxMatch = NULL;
		
		if(pBx->GetSection())
		{
			if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) || pBx->GetSection()->IsenDetType(SECDET_VBRACING))
				pBxMatch = pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirMatch);
			else
				pBxMatch = pBx->GetGirder()->GetBxMatchByVStiff(pBx, pGirMatch);
		}
		else if(pBx->IsState(BX_STT_SLAB))
			pBxMatch = Finder.GetBxFirst(BX_STT_SLAB);
		else if(pBx->IsState(BX_END_SLAB))
			pBxMatch = Finder.GetBxFirst(BX_END_SLAB);
		
		if(pBxMatch)
			DrawSlabHunchOnBx(&Dom, pBxMatch);
	}

	*pDom << Dom;
}

// 현재 BX의 슬래브 횡단 헌치를 그림( nG==0: 좌측캔틸레버 포함, nG==pBridge->GetGirdersu()-2: 우측캔틸레버 포함)
void CAPlateDrawDanmyun::DrawSlabHunchOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp		*pDB	= m_pDataMng->GetBridge();	
	CPlateGirderApp		*pGir	= (CPlateGirderApp	*)pBx->GetGirder();
	CPlateGirderApp		*pGirR	= pGir->m_pRight;
	CPlateBasicIndex	*pBxR	= pGirR ? pGirR->GetBxByStation(pBx->GetStation()) : NULL;
	CSection			*pSec	= pBx->GetSection();
	CPlateBxFinder Finder(pGirR);

	if(pSec && pGirR)
	{
		if(pSec->IsenDetType(SECDET_CROSSBEAM) || pSec->IsenDetType(SECDET_VBRACING))
			pBxR = ((CPlateBasicIndex *)pBx)->GetBxMatchByCrossBeam(pGirR);
		else if(pSec->IsenDetType(SECDET_VSITFF))
			pBxR = ((CPlateBasicIndex *)pBx)->GetBxMatchByVStiff(pGirR);
	}
	else if(pBx->IsState(BX_STT_SLAB) && pGirR)
		pBxR = Finder.GetBxFirst(BX_STT_SLAB);
	else if(pBx->IsState(BX_END_SLAB) && pGirR)
		pBxR = Finder.GetBxFirst(BX_END_SLAB);

	long nG = pBx->GetNumberGirder();

	CDomyun Dom(pDom);
	CDPoint A[HDANLINESU]; // 포장상단의 포인트	
	CDPoint B[10]; // 슬래브 헌치부의  포인트
	CDPoint vX(1,0), vY(0,1);

	pDB->GetXyPaveDom(pBx, A);
	pGir->GetHunchXyDom(pBx,B, FALSE, FALSE, pDB->IsTUGir());
	
	double dHTaperL		= pGir->GetHeightTaper(pBx->GetStation());
	double dHTaperR		= pBxR ? pGirR->GetHeightTaper(pBxR->GetStation()) : dHTaperL;
	double dTPave		= pDB->GetThickPave();
	double dTSlabLeft	= pDB->GetSlabThick(CPlateBridgeApp::SLAB_LEFT);
	double dTSlabRigh	= pDB->GetSlabThick(CPlateBridgeApp::SLAB_RIGHT);
	long   nHDansu		= pDB->GetQtyHDanDom(pBx);//GetQtyHDanNode();

	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= dTPave;

	A[0].y -= (dTSlabLeft + dHTaperL);			// 슬래브의 아래부분위치
	A[nHDansu].y -=  (dTSlabRigh + dHTaperR);
	
	if(nG==0)						Dom.LineTo(B[0], A[0]);
	if(nG==pDB->GetGirdersu()-2)	Dom.LineTo(B[9], A[nHDansu]);

	Dom.MoveTo(B[0]);	
	for(long i=1; i<10; i++)
		Dom.LineTo(B[i]);
	
	*pDom << Dom;	
}

void CAPlateDrawDanmyun::DrawInputSlabDimDanmyun(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	DimSlabUpper(pDom, pBx);
	DimSlabHunch(pDom, pBx);
	DimSlabCenterLine(pDom, pBx);
	DimSlabHunchWidth(pDom, pBx);
	DimSlabBaseThick(pDom, pBx);
}

// 횡단의 슬래브와 거더단면을 그림
void CAPlateDrawDanmyun::DrawDanmyunOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bWalkRoad, BOOL bPave, long nDrawingType/*=2*/, BOOL bGirder)
{		
	CPlateBridgeApp	 *pBridge   = m_pDataMng->GetBridge();
	CPlateGirderApp  *pGirMatch = NULL;
	CPlateBasicIndex *pBxMatch  = NULL;

	CAPlateDrawCross DrawCross(m_pDataMng);
	
	if(pBx==NULL)	return;
	
	if(pBx->GetSection() || pBx->IsState(BX_STT_SLAB|BX_END_SLAB))
	{
		DrawSlabUpper(pDom, pBx, bWalkRoad, bPave);
		DrawSlabHunch(pDom, pBx);
	}

	if(bGirder)
	{
		for(long nG=0; nG<pBridge->GetGirdersu();nG++)
		{
			pGirMatch = pBridge->GetGirder(nG);	
			CPlateBxFinder Finder(pGirMatch);
			pBxMatch = NULL;

			if(pBx->GetSection())
			{
				if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) || pBx->GetSection()->IsenDetType(SECDET_VBRACING))
					pBxMatch = pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirMatch);
				else
					pBxMatch = pBx->GetGirder()->GetBxMatchByVStiff(pBx, pGirMatch);
			}
			else if(pBx->IsState(BX_STT_SLAB))
				pBxMatch = Finder.GetBxFirst(BX_STT_SLAB);
			else if(pBx->IsState(BX_END_SLAB))
				pBxMatch = Finder.GetBxFirst(BX_END_SLAB);
							
			if(pBxMatch)
				DrawCross.DrawGirderFront(pDom, pBxMatch, nDrawingType);
		}
	}
}

// 횡단의 슬래브, 거더단면, 가로보단면생성
void CAPlateDrawDanmyun::DrawDanmyunEttOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon, BOOL bGirderMark, BOOL bMarkPosUp)
{		
	DrawDanmyunOnBx(pDom,pBx);
	DrawCRBeamFrontAllOnBx(pDom, pBx, bJewon, bGirderMark, bMarkPosUp);
}

///////////////////////////////////////////////////////////////////////////////
// 방호벽
void CAPlateDrawDanmyun::DrawInputGuardFence(CDomyun* pDom, long nArrIndex, BOOL bIsInput)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CCentSeparation * pGWall = pBridge->GetGuardWall(nArrIndex);
	
	if(pGWall==NULL)	return;

	CDomyun Dom(pDom);

	long nHDanNode = pBridge->GetQtyHDanNode()-1;
	//
	double BX = 0;
	double BY = 0;
	double LEN = 0;
	double Width = pGWall->GetWidth();
	double D1 = pGWall->m_D1;
	double Co = 0;
	double Scale = 20;
//	Dom.SetScaleDim(Scale);

	pOptStd->SetEnvType(&Dom,HCAD_STLC);	
	long   pos = pGWall->GetPlace();

	if(pGWall->GetWidth() !=0)
	{
		BOOL bLeft = TRUE;
		if(pos==2) 
		{
			LEN = pBridge->GetLengthHDanDom(pBx, nHDanNode);
			Co = Width-(LEN-D1);
			bLeft = FALSE;
		}

		DrawGuardWall(&Dom,pGWall,BX+Co,BY);
		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		DimGuardWallTop(&Dom,pGWall,BX,BY,TRUE,Scale);

		if(pos==1) BX = -pGWall->m_D1;
		DimGuardWallSide(&Dom,pGWall,BX,BY,bLeft,TRUE,Scale);
	}

	//// SLAB 그리기
	double Len    = 0;
	double SlabT1 = pBridge->m_dThickSlabLeft;
	double SlabT2 = pBridge->m_dThickSlabBase;
	double SlabT3 = pBridge->m_dThickSlabRight;
	double dTPave  =	pBridge->m_dThickPave;
	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pBridge->GetXyPaveUpperDom(pBx, A);
	if(pos==0)	 // LEFT
	{
		double dSlopSlab  = (A[0].y-A[1].y)/(A[0].x-A[1].x);
		nHDanNode = 0;
		Len = pBridge->GetLengthHDanDom(pBx, nHDanNode)+500;
		pOptStd->SetEnvType(&Dom,HCAD_STLC);
		Dom.LineTo(-D1, -D1*dSlopSlab, Len, Len*dSlopSlab);
		Dom.LineTo(-D1, -D1*dSlopSlab, -D1, -SlabT1);
		Dom.LineTo(-D1, -SlabT1, Len, -SlabT2+Len*dSlopSlab);
		pOptStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightning(Len, -SlabT2-50+Len*dSlopSlab, Len, dTPave+50+Len*dSlopSlab);
		pOptStd->SetEnvType(&Dom,HCAD_HATH);
		Dom.Solid(Width,  Width*dSlopSlab, Len,     Len*dSlopSlab,
			        Len, dTPave+Len*dSlopSlab, Width, dTPave+Width*dSlopSlab);
	}
	if(pos==1)	 // CENTER
	{
		for(long n=0; n < pBridge->GetQtyHDanNode(); n++)
		{
		//	if(pBridge->GetWallGuardTypeHDan(pBx, n) == 1)  //방호벽이면
			if(pBridge->GetWallGuardTypeHDan(n) == 1)  //방호벽이면
			{
				nHDanNode = n;
				if(n != 0) break;
			}
		}

		double dSlopSlab  = (A[nHDanNode].y-A[nHDanNode+1].y)/(A[nHDanNode].x-A[nHDanNode+1].x);
		Len = pBridge->GetLengthHDanDom(pBx, nHDanNode);
		pOptStd->SetEnvType(&Dom,HCAD_STLC);
		Dom.LineTo(-D1, 0,   Len-D1, (Len-D1)*dSlopSlab);
		Dom.LineTo(-D1, -SlabT1,  Len-D1, -SlabT1);

		pOptStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightning(-D1,-SlabT1-50, -D1,dTPave+50);
		Dom.CutLightning( Len-D1,-SlabT1-50, Len-D1,dTPave+50+(Len-D1)*dSlopSlab);
		pOptStd->SetEnvType(&Dom,HCAD_HATH);

		Dom.Solid(Width,    0, Len-D1, 0,
			        Len-D1, dTPave, Width, dTPave);

		Dom.Solid(-D1,  0,  0, 0,
			        0, dTPave,  -D1, dTPave);
	}
	if(pos==2)	 // RIGHT
	{
		double dSlopSlab  = (A[nHDanNode].y-A[nHDanNode+1].y)/(A[nHDanNode].x-A[nHDanNode+1].x);
		nHDanNode = pBridge->GetQtyHDanNode()-1;
		Len = pBridge->GetLengthHDanDom(pBx, nHDanNode)+500;
		pOptStd->SetEnvType(&Dom,HCAD_STLC);
		double DiffSlab = (Width+D1)*dSlopSlab;
		Dom.LineTo(Width+D1,     DiffSlab,         -Len, -Len*dSlopSlab);
		Dom.LineTo(Width+D1,     DiffSlab, Width+D1, -SlabT3+DiffSlab);
		Dom.LineTo(Width+D1, -SlabT3+DiffSlab,          -Len, -SlabT2-Len*dSlopSlab);
		pOptStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightning(-Len, -SlabT2-50-Len*dSlopSlab, -Len, dTPave+50-Len*dSlopSlab);
		pOptStd->SetEnvType(&Dom,HCAD_HATH);
		Dom.Solid(0, 0, -Len, -Len*dSlopSlab, -Len, dTPave-Len*dSlopSlab, 0, dTPave);
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawVStiffAllOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp	*pGir	= (CPlateGirderApp	*)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	for(long nG=0;nG<pBridge->GetGirdersu();nG++)
	{
		CPlateGirderApp *pGirMatch = pBridge->GetGirder(nG);	
		CSection *pSec = pBx->GetSection();
		CPlateBasicIndex *pBxMatch = NULL;
		if(pSec->IsenDetType(SECDET_CROSSBEAM) || pSec->IsenDetType(SECDET_VBRACING))
			pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGirMatch);
		else
			pBxMatch = pGir->GetBxMatchByVStiff(pBx, pGirMatch);
		if(pGir == pGirMatch) pBxMatch = pBx;
		if(pBxMatch)
			DrawVStiffOnBx(&Dom, pBxMatch);
	}

	*pDom << Dom;
}
void CAPlateDrawDanmyun::DrawCRBeamFrontAllOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon, BOOL bGirderMark, BOOL bMarkPosUp)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp	*pGir    = pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CAPlateDrawCross DrawCross(m_pDataMng);
	long nG = 0;
	for(nG=0;nG<pBridge->GetGirdersu()-1;nG++)
	{
		CPlateGirderApp *pGirMatch = pBridge->GetGirder(nG);	
		CSection *pSec = pBx->GetSection();
		CPlateBasicIndex *pBxMatch = NULL;
		if(pSec)
		{
			if(pSec->IsenDetType(SECDET_CROSSBEAM) || pSec->IsenDetType(SECDET_VBRACING))
				pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGirMatch);
			else
				pBxMatch = pGir->GetBxMatchByVStiff(pBx, pGirMatch);
			if(pGir == pGirMatch) pBxMatch = pBx;

			CPlateBasicIndex *pBxMatchMatch = pGir->GetBxMatchByCrossBeam(pBx, pGirMatch->m_pRight);
			if(pBxMatchMatch)
				DrawCross.DrawCRBeamTotalFrontOnBx(&Dom, pBxMatch, FALSE, bJewon);
		}
	}

	if(bGirderMark==FALSE)
	{
		*pDom << Dom;
		return;
	}

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CString cs;
	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);	
		CDPoint A[HDANLINESU];
		pGir->GetHunchXyDom(pBx, A);
		long nJijum  = pBx->GetNumberJijum();
		CShoe *pShoe = pGir->GetShoe(nJijum);
		double dHeightTotalShoe = pShoe->GetHeightTotalShoe();	
		double dThickFlangeLo = pGir->GetThickFactByStation(pBx->GetStation(), G_F_L);
		double dHeightGir     = pGir->GetHeightGirderByStaAng(pBx->GetStation());

		CDPoint xyCen = (A[1]+A[2])/2;
		if(bMarkPosUp)
		{
			if(pBx->IsJijum())
				xyCen.y += (pBx->IsDanbuJijumStt() ? pBridge->m_dHeightTaperStt : pBridge->m_dHeightTaperEnd) 
					       + pBridge->m_dThickSlabBase + pBridge->m_dThickSlabHunch + pBridge->m_dThickPave + Dom.Always(5);
			else
				xyCen.y += pBridge->m_dThickSlabBase + pBridge->m_dThickSlabHunch + pBridge->m_dThickPave + Dom.Always(5);
		}
		else
		{
			if(pBx->IsJijum())
			{
				xyCen.y -= dHeightGir + dThickFlangeLo + dHeightTotalShoe;
				//xyCen.y += (Dom.GetTextHeight()/2)*1.6;
			}
			else
				xyCen.y -= dHeightGir + dThickFlangeLo + Dom.Always(5);
		}
		cs.Format("G%d", nG+1);
		Dom.TextCircle(xyCen.x, xyCen.y, cs, 0, FALSE, TRUE);
	}
	
	*pDom << Dom;
}
void CAPlateDrawDanmyun::DrawJackUpStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd	*pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	 = (CPlateGirderApp	*)pBx->GetGirder();
	
	long nJ = pGir->GetNumberJijumByBx(pBx);
	CJackupStiff *pJackUp = pGir->GetJackupByJijum(nJ, 0);
	if(!pJackUp) return;

	CDPoint B[10]; // 거더의 포인트
	pGir->GetHunchXyDom(pBx,B);
	
	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint	ptMid			= (B[1] + B[2])/2;	
	double	dLWidth			= pJackUp->m_vJewon[0].x;		///< 좌측 수직보강재 폭
	double	dRWidth			= pJackUp->m_vJewon[1].x;		///< 우측 수직보강재 폭
	double	dVG				= pJackUp->m_dVG;		
	double	dVL				= pJackUp->m_dVL;
	double  dH1L			= pJackUp->m_vJewon[0].z;
	double	WebT			= pBx->GetFactChain(G_W)->m_dFactChainThick;
	
	CDPoint vX(1,0), vY(0,1);
	
	Dom.LineTo(ptMid - vX*(WebT/2) - vY*dH1L, ptMid - vX*(dLWidth + WebT/2) - vY*dH1L);	
	Dom.LineTo(ptMid - vX*(dLWidth + WebT/2) - vY*((dH1L-dVL)+(dLWidth-dVG)), ptMid - vX*(dLWidth + WebT/2) - vY*dH1L);
	Dom.LineTo(ptMid - vX*(dLWidth + WebT/2) - vY*((dH1L-dVL)+(dLWidth-dVG)), ptMid - vX*(dVG + WebT/2) - vY*(dH1L-dVL));
	Dom.LineTo(ptMid - vX*(dVG + WebT/2) - vY*(dH1L-dVL), ptMid - vX*(WebT/2) - vY*(dH1L-dVL));
	
	Dom.LineTo(ptMid + vX*(WebT/2) - vY*dH1L, ptMid + vX*(dRWidth + WebT/2) - vY*dH1L);
	Dom.LineTo(ptMid + vX*(dRWidth + WebT/2) - vY*((dH1L-dVL)+(dRWidth-dVG)), ptMid + vX*(dRWidth + WebT/2) - vY*dH1L);
	Dom.LineTo(ptMid + vX*(dRWidth + WebT/2) - vY*((dH1L-dVL)+(dRWidth-dVG)), ptMid + vX*(dVG + WebT/2) - vY*(dH1L-dVL));
	Dom.LineTo(ptMid + vX*(dVG + WebT/2) - vY*(dH1L-dVL), ptMid + vX*(WebT/2) - vY*(dH1L-dVL));
	
	*pDom << Dom;	
}

void CAPlateDrawDanmyun::DimJackUpStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd	*pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	 = (CPlateGirderApp	*)pBx->GetGirder();
	CJackupStiff *pJackUp = pGir->GetJackupByJijum(pBx->GetNumberJijum(), 0);	
	if(!pJackUp) return;

	CDPoint B[10]; // 거더의 포인트
	pGir->GetHunchXyDom(pBx,B);
	
	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint	ptMid			= (B[1] + B[2])/2;	
	double	dLWidth			= pJackUp->m_vJewon[0].x;		///< 좌측 수직보강재 폭
	double	dRWidth			= pJackUp->m_vJewon[1].x;		///< 우측 수직보강재 폭
	double	dVG				= pJackUp->m_dVG;		
	double	dVL				= pJackUp->m_dVL;
	double  dH1L			= pJackUp->m_vJewon[0].z;
	double	WebT			= pBx->GetFactChain(G_W)->m_dFactChainThick;
	
	CDPoint vX(1,0), vY(0,1);
	CDPoint ptBase = ptMid - vX*(WebT/2) - vY*dH1L;	
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(ptBase.x, ptBase.y,1);	
	Dom.DimLineToExtend(dVL,1,"VL",COMMA(dVL));
	Dom.DimLineTo(dH1L-dVL,1,COMMA(dH1L-dVL));
	Dom.DimMoveTo(ptBase.x, ptBase.y,2);	
	Dom.DimLineTo(dH1L,2,COMMA(dH1L));

	ptBase = ptMid - vX*(dLWidth+WebT/2) - vY*dH1L;		
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(ptBase.x, ptBase.y,0);
	Dom.DimLineToExtend(dLWidth, 0, "WL", COMMA(dLWidth));
	Dom.DimLineTo(WebT, 0, COMMA(WebT));
	Dom.DimLineToExtend(dRWidth, 0, "WR", COMMA(dRWidth));

	ptBase = ptMid - vX*(dVG+WebT/2) - vY*(dH1L-dVL);		
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(ptBase.x, ptBase.y,0);
	Dom.DimLineToExtend(dVG, 0, "VG", COMMA(dVG));
	
	*pDom << Dom;	
}

void CAPlateDrawDanmyun::DrawSpliceOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd	*pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	 = (CPlateGirderApp	*)pBx->GetGirder();
	CPlateSplice	*pSp	 = pBx->GetSplice();
	CAPlateDrawJong DrawJong(m_pDataMng);

	if(!pSp)	return;

	CDPoint A[10]; // 거더의 포인트
	pGir->GetHunchXyDom(pBx,A);

	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint	ptMid = (A[1] + A[2])/2;
	double	dWebT = pBx->GetFactChain(G_W)->m_dFactChainThick;

	if(pSp->m_mT>0)	// 상,하 모멘트 이음판
	{
		for(long n=0; n<2; n++)
		{
			CPlatePieceObject pObj(pGir);
			pObj.m_nMarkBuje = SP_W_MO;
			pObj.m_nRow = n;
			pObj.m_nCol = n==0 ? 1 : 0;
			pSp->CreatePlatePieceObj(&pObj,FALSE, FALSE, 0, TRUE, 0);

			if(pObj.GetPoint(0).y == pObj.GetPoint(1).y) continue;

			Dom.MoveTo(ptMid.x+dWebT/2, ptMid.y+pObj.GetPoint(0).y);
			Dom.LineToEx(pSp->m_mT, 0);
			Dom.LineToEx(0, pObj.GetPoint(1).y - pObj.GetPoint(0).y);
			Dom.LineToEx(-pSp->m_mT, 0);

			Dom.MoveTo(ptMid.x-dWebT/2, ptMid.y+pObj.GetPoint(0).y);
			Dom.LineToEx(-pSp->m_mT, 0);
			Dom.LineToEx(0, pObj.GetPoint(1).y - pObj.GetPoint(0).y);
			Dom.LineToEx(pSp->m_mT, 0);	
		}			
	}

	CPlatePieceObject pObj(pGir);
	pObj.m_nMarkBuje = SP_W_SH;
	pObj.m_nCol =0;
	pSp->CreatePlatePieceObj(&pObj,FALSE, FALSE, 0, TRUE, 0);

	Dom.MoveTo(ptMid.x+dWebT/2, ptMid.y+pObj.GetPoint(0).y);
	Dom.LineToEx(pSp->m_sT, 0);
	Dom.LineToEx(0, pObj.GetPoint(1).y - pObj.GetPoint(0).y);
	Dom.LineToEx(-pSp->m_sT, 0);

	Dom.MoveTo(ptMid.x-dWebT/2, ptMid.y+pObj.GetPoint(0).y);
	Dom.LineToEx(-pSp->m_sT, 0);
	Dom.LineToEx(0, pObj.GetPoint(1).y - pObj.GetPoint(0).y);
	Dom.LineToEx(pSp->m_sT, 0);	
		
	*pDom << Dom;
}

// 상부 값이 0이면 상부플랜지에, 하부 값이 0이면 하부플랜지에 접함.
// 수직보강재 자체의 길이(높이)는 GetVStiffHeight(bLeft) 로 구한다.
// dLWidth, dRWidth 값이 있을때만 그린다.
void CAPlateDrawDanmyun::DrawVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd	*pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	 = (CPlateGirderApp	*)pBx->GetGirder();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CSection		*pSec	 = pBx->GetSection();
	CVStiff			*pVStiff = pBx->IsJijum() ? pBx->GetJijumStiff() : pSec->GetVStiff();
	CPlateCrossBeam *pCL = NULL, *pCR = NULL;
	CVBracing		*pVrL = NULL, *pVrR = NULL;
	CPlateBasicIndex*pBxCheck	= NULL;

	if(!pVStiff) return;

	// pCL : 이전 거더의 가로보 우측, pCR : 현재 거더의 가로보 좌측
	CDPoint vGir		= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dCrossAng	= fabs(pBx->GetAngleSectionCrossVBracing().RotateInv(vGir).y);
	double dCrossAngN	= fabs(pBx->GetAngleSectionCrossVBracing().RotateInv(vGir).y);
	if(pBx->GetBxMatchByCrossBeam(pGir->m_pLeft))
	{
		pCL		= pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetSection()->GetCrossBeam();
		pVrL	= pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetSection()->GetVBracing();

		BOOL bTRUE = FALSE;
		if(pCL && pCL->GetBx()->GetBxMatchByCrossBeam(pGir))
		{
			pBxCheck = pCL->GetBx()->GetBxMatchByCrossBeam(pGir);
			if(pBxCheck == pBx)	bTRUE	= TRUE;
			else				pCL		= NULL;
		}
		if(pVrL && pVrL->GetBx()->GetBxMatchByCrossBeam(pGir))
		{
			pBxCheck = pVrL->GetBx()->GetBxMatchByCrossBeam(pGir);
			if(pBxCheck == pBx)	bTRUE	= TRUE;
			else				pVrL	= NULL;
		}
		if(bTRUE)
		{
			vGir	= pGir->m_pLeft->GetLine()->GetAngleAzimuth(pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetStation());
			dCrossAng	= fabs(pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetAngleSectionCrossVBracing().RotateInv(vGir).y);
		}		
	}
	pCR		= pSec->GetCrossBeam();
	pVrR	= pSec->GetVBracing();
	if(pSec->IsenDetType(SECDET_VBRACING))
		dCrossAng = dCrossAngN = 1;
	

	CDPoint B[10]; // 거더의 포인트
	pGir->GetHunchXyDom(pBx,B);
	
	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint vX(1,0), vY(0,1);

	double	dHeight			= pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double	dLWidth			= pVStiff->m_dWidth[0]*dCrossAng;		///< 좌측 수직보강재 폭
	double	dRWidth			= pVStiff->m_dWidth[1]*dCrossAngN;		///< 우측 수직보강재 폭
	CDPoint	ptMidUp			= (B[1] + B[2])/2;
	CDPoint	ptMidLo			= ptMidUp - vY*dHeight;

	double	dHLUp			= pVStiff->m_dH[LEFT][UP];
	double	dHLLo			= pVStiff->m_dH[LEFT][DOWN];
	double	dHRUp			= pVStiff->m_dH[RIGHT][UP];
	double	dHRLo			= pVStiff->m_dH[RIGHT][DOWN];
	double	dHLeft			= pVStiff->GetVStiffHeight(TRUE, dHeight);
	double	dHRight			= pVStiff->GetVStiffHeight(FALSE, dHeight);
	double	dLG				= pVStiff->m_dGap[LEFT];
	double	dRG				= pVStiff->m_dGap[RIGHT];

	double	WebT			= pBx->GetFactChain(G_W)->m_dFactChainThick;
	double	dWidthUp		= pGir->GetWidthFlangeUpper(pBx);
	double	dWidthLo		= pGir->GetWidthFlangeLower(pBx);
	double	scallopR		= 35.0;

	double	dRad			= 3.1415926535/180.0;
	double	dLUCompDis		= (dWidthUp - WebT)/2 - dLWidth;		// 좌측 수직보강재 폭과 상부 플랜지폭의 비교
	double	dRUCompDis		= (dWidthUp - WebT)/2 - dRWidth;		// 우측 수직보강재 폭과 상부 플랜지폭의 비교
	double	dLLCompDis		= (dWidthLo - WebT)/2 - dLWidth;		// 좌측 수직보강재 폭과 하부 플랜지폭의 비교
	double	dRLCompDis		= (dWidthLo - WebT)/2 - dRWidth;		// 우측 수직보강재 폭과 하부 플랜지폭의 비교
	double	dLUCutH			= tan(45.0*dRad)*(abs(dLUCompDis)+15);	// 좌측 상부의 절단 높이 계산
	double	dRUCutH			= tan(45.0*dRad)*(abs(dRUCompDis)+15);	// 우측 상부의 절단 높이 계산
	double	dLLCutH			= tan(45.0*dRad)*(abs(dLLCompDis)+15);	// 좌측 하부의 절단 높이 계산
	double	dRLCutH			= tan(45.0*dRad)*(abs(dRLCompDis)+15);	// 우측 하부의 절단 높이 계산
	long	nLCuttingType	= 0;									// 0 : 절단안함	1 : 좌측상부	2 : 좌측하부	3 : 좌측상하부
	long	nRCuttingType	= 0;									// 0 : 절단안함	1 : 우측상부	2 : 우측하부	3 : 우측상하부

	if((dLUCompDis < 0 && !pBridge->IsTUGir()) || (dHLUp==0 && dLWidth > (dWidthUp-WebT)/2))
		nLCuttingType += 1;
	if(dHLLo >0 || dLLCompDis < 0)
		nLCuttingType += 2;

	if((dRUCompDis < 0 && !pBridge->IsTUGir()) || (dHRUp==0 && dRWidth > (dWidthUp-WebT)/2))
		nRCuttingType += 1;
	if(dHRLo >0 || dRLCompDis < 0)
		nRCuttingType += 2;

	CDPointArray xyL, xyR;
	CDPoint xy;

	// Left
	if(dLWidth != 0)
	{
		if(dHLUp==0 && pCL && (pCL->GetType() == CR_TYPE_HSTEEL || (pCL->m_dRH1 > scallopR)))
			Dom.Arc(ptMidUp-vX*(WebT/2), scallopR, 180,  90);
		else if(dHLUp==0 && pVrL)
			Dom.Arc(ptMidUp-vX*(WebT/2), scallopR, 180,  90);
		else if(dHLUp==0 && !pCL && !pVrL)
			Dom.Arc(ptMidUp-vX*(WebT/2), scallopR, 180,  90);
		if(dHLLo==0)	Dom.Arc(ptMidLo-vX*(WebT/2), scallopR, 180, -90);
	}

	if(pCL && pCL->GetType() == CR_TYPE_MANUFACTURE && dLWidth!=0)
	{
		DrawManufactureVStiffOnBx(&Dom, pBx, TRUE);
	}
	else if(dLWidth!=0)
	{
		if(dLWidth < (dWidthLo-WebT)/2 || dHLLo > 0)
			dLLCutH = max(dLWidth-dLG, 0);

		switch(nLCuttingType)
		{
		case 0:	// 절단안함
			xy = ptMidUp-vX*(WebT/2)-vY*dHLUp;
			xyL.Add(xy);
			xy -= vX*dLWidth;
			xyL.Add(xy);
			xy -= vY*dHLeft;
			xyL.Add(xy);
			xy += vX*dLWidth;
			xyL.Add(xy);
			break;
		case 1:	// 상부 모따기
		case 3:	// 상하부 모따기
			xy = ptMidUp-vX*(WebT/2);
			xyL.Add(xy);
			xy += vX*(dLUCutH - dLWidth);
			xyL.Add(xy);
			xy -= vX*dLUCutH + vY*dLUCutH;
			xyL.Add(xy);
			dHLeft = dHLeft-dLUCutH;

		case 2:	// 하부 모따기
			if(nLCuttingType == 2)
			{
				xy = ptMidUp-vX*(WebT/2)-vY*dHLUp;
				xyL.Add(xy);
				xy -= vX*dLWidth;
				xyL.Add(xy);
			}
			
			if(nLCuttingType == 1)
			{
				xy -= vY*dHLeft;
				xyL.Add(xy);
				xy += vX*dLWidth;
				xyL.Add(xy);
				break;
			}
			
			xy -= vY*(dHLeft-dLLCutH);
			xyL.Add(xy);
			xy += vX*dLLCutH - vY*dLLCutH;
			xyL.Add(xy);
			xy += vX*(dLWidth-dLLCutH);
			xyL.Add(xy);

			break;
		}

		Dom.MoveTo(xyL[0]);
		for(long n=1; n<xyL.GetSize(); n++)
			Dom.LineTo(xyL.GetAt(n));
	}

	// Right
	if( dRWidth != 0)
	{	
		if(dHRUp==0 && pCR && (pCR->GetType() == CR_TYPE_HSTEEL || (pCR->m_dLH1 > scallopR)))
			Dom.Arc(ptMidUp+vX*(WebT/2), scallopR, 270, 90);
		else if(dHRUp==0 && pVrR)
			Dom.Arc(ptMidUp+vX*(WebT/2), scallopR, 270, 90);
		else if(dHRUp==0 && !pCR && !pVrR)
			Dom.Arc(ptMidUp+vX*(WebT/2), scallopR, 270, 90);
		if(dHRLo==0)	Dom.Arc(ptMidLo+vX*(WebT/2), scallopR,   0, 90);
	}

	if(pCR && pCR->GetType() == CR_TYPE_MANUFACTURE && dRWidth!=0 && pBx->GetNumberGirder()!=pBridge->GetQtyGirder()-1)
	{
		DrawManufactureVStiffOnBx(&Dom, pBx, FALSE);
	}
	else if(dRWidth!=0)
	{
		if(dRWidth < (dWidthLo-WebT)/2 || dHRLo > 0)
			dRLCutH = max(dRWidth-dRG, 0);

		switch(nRCuttingType)
		{
		case 0:	// 절단안함
			xy = ptMidUp+vX*(WebT/2)-vY*dHRUp;
			xyR.Add(xy);
			xy += vX*dRWidth;
			xyR.Add(xy);
			xy -= vY*dHRight;
			xyR.Add(xy);
			xy -= vX*dRWidth;
			xyR.Add(xy);
			break;
		case 1:	// 상부 모따기
		case 3:	// 상하부 모따기
			xy = ptMidUp+vX*(WebT/2);
			xyR.Add(xy);
			xy += vX*(dRWidth-dRUCutH);
			xyR.Add(xy);
			xy += vX*dRUCutH - vY*dRUCutH;
			xyR.Add(xy);
			dHRight = dHRight-dRUCutH;

		case 2:	// 하부 모따기
			if(nRCuttingType == 2)
			{
				xy = ptMidUp+vX*(WebT/2)-vY*dHRUp;
				xyR.Add(xy);
				xy += vX*dRWidth;
				xyR.Add(xy);
			}
			
			if(nRCuttingType == 1)
			{
				xy -= vY*dHRight;
				xyR.Add(xy);
				xy -= vX*dRWidth;
				xyR.Add(xy);
				break;
			}

			xy -= vY*(dHRight-dRLCutH);
			xyR.Add(xy);	xyR.Add(xy);
			xy -= vX*dRLCutH + vY*dRLCutH;
			xyR.Add(xy);	xyR.Add(xy);
			xy -= vX*(dRWidth-dRLCutH);
			xyR.Add(xy);
			break;
		}

		Dom.MoveTo(xyR[0]);
		for(long n=1; n<xyR.GetSize(); n++)
			Dom.LineTo(xyR.GetAt(n));
	}

	*pDom << Dom;
}

// 제작타입의 수직보강재 - 가로보가 있고, 제작타입임이 확인된 상태
void  CAPlateDrawDanmyun::DrawManufactureVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	CARoadOptionStd	*pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	 = pBx->GetGirder();
	CSection		*pSec	 = pBx->GetSection();

	// pCL : 거더좌측과 연결된 가로보, pCR : 거더우측과 연결된 가로보
	CPlateCrossBeam *pCL	= bLeft ? pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetSection()->GetCrossBeam() : NULL;
	CPlateCrossBeam *pCR	= pSec->GetCrossBeam();
	CPlateCrossBeam	*pC		= bLeft ? pCL : pCR;
	CVStiff			*pVStiff = pBx->IsJijum() ? pBx->GetJijumStiff() : pSec->GetVStiff();

	CDPoint vX(1,0), vY(0,1), A[10], B[10]; // A:가로보, B:거더의 포인트
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx,B);

	long	nCol	= bLeft ? 0 : 1;
	double	dHeight	= pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double	dWidth	= pVStiff->m_dWidth[nCol];					// 수직보강재 폭
	double	dHUp	= pVStiff->m_dH[nCol][UP];
	double	dHLo	= pVStiff->m_dH[nCol][DOWN];
	double	dWebT	= pBx->GetFactChain(G_W)->m_dFactChainThick;
	double	dFUpW	= pBx->GetFactChain(G_F_U)->m_dWidth;		// 상부플랜지 폭
	double	dFLoW	= pBx->GetFactChain(G_F_L)->m_dWidth;		// 하부플랜지 폭
	double	dGap	= 15;
	double	dCrH	= bLeft ? pC->m_dRH1 : pC->m_dLH1;			// 상부플랜지 ~ 가로보 상부 이격거리
	double	dCutUp	= dWidth - ((dFUpW-dWebT)/2-dGap);			// 상부 컷팅 폭
	double	dCutLo	= dWidth - ((dFLoW-dWebT)/2-dGap);			// 상부 컷팅 폭
	CDPoint	ptMidUp	= (B[1] + B[2])/2;
	CDPoint	ptMidLo	= ptMidUp - vY*dHeight;
	CDPoint	vAng	= pC->GetVectorCross();
	if(vAng.x < 0){ vAng.x*=-1; vAng.y*=-1; }
	double	dAngle	= vAng.GetAngleDegree();
	double	dR		= 35.0;

	if(bLeft)
	{
		vX = CDPoint(-1, 0);		
		vAng.Rotate(CDPoint(-1,0));
	}

	if(dHUp > 0)	dCutUp = 0;
	if(dCutUp < 0)	dCutUp = 0;
	if(dCutLo < 0)	dCutLo = 0;
	if(dAngle > 90)	dAngle -= 360;

	CDPoint	xy, xyMatch, xyBase, xyScallopCen;
	CDPointArray xyArr;
	
	if(bLeft)	GetXyMatchLineAndLine(A[5], vAng, ptMidUp+vX*(dWebT/2), vY, xyBase);
	else		GetXyMatchLineAndLine(A[2], vAng, ptMidUp+vX*(dWebT/2), vY, xyBase);

	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	// 상부
	if(dCrH > dHUp)
	{
		xy = ptMidUp + vX*(dWebT/2) - vY*dHUp;
		xyArr.Add(xy);
		xy += vX*(dWidth-dCutUp);
		xyArr.Add(xy);
		xy += vX*dCutUp - vY*dCutUp;

		GetXyMatchLineAndLine(xyBase+vY*pC->m_T2_CR, vAng, xy, vY, xyMatch);
		if(xyMatch.y > xy.y)
		{
			GetXyMatchLineAndLine(xyBase+vY*pC->m_T2_CR, vAng, xy, CDPoint(bLeft?1:-1,1), xy);
			xyArr.Add(xy);
		}
		else
		{
			xyArr.Add(xy);
			xyArr.Add(xyMatch);
		}		
		
		xy = xyBase + vY*pC->m_T2_CR;
		xyScallopCen = xy;
		if(dCrH > dR)
			xy -= vAng*dR*(bLeft?1:-1);
		xyArr.Add(xy);

		for(long n=0; n<xyArr.GetSize()-1; n++)
			Dom.LineTo(xyArr.GetAt(n), xyArr.GetAt(n+1));

		if(dCrH > dR)
		{
			if(bLeft)	Dom.Arc(xyScallopCen, dR, 90,	  90+dAngle);
			else		Dom.Arc(xyScallopCen, dR, dAngle, 90-dAngle);
		}
	}

	xyArr.RemoveAll();
	// 하부

	CDPoint xyBaseCr = bLeft ? A[5] : A[2];
	if(xyBase.y-pC->m_H_CR > ptMidLo.y && !pC->m_cGasset_CR)
	{
		xy = ptMidLo + vX*(dWebT/2) + vY*dHLo;
		xyArr.Add(xy);
		xy += vX*(dWidth-dCutLo);
		xyArr.Add(xy);
		xy += vX*dCutLo + vY*dCutLo;

		GetXyMatchLineAndLine(xyBaseCr-vY*(pC->m_H_CR+pC->m_T1_CR), vAng, xy, vY, xyMatch);
		if(xyMatch.y < xy.y)
		{
			GetXyMatchLineAndLine(xyBaseCr-vY*(pC->m_H_CR+pC->m_T1_CR), vAng, xy, CDPoint(bLeft?-1:1,1), xy);
			xyArr.Add(xy);
		}
		else
		{
			xyArr.Add(xy);
			xyArr.Add(xyMatch);
		}
		
		xy = xyBase-vY*(pC->m_H_CR+pC->m_T1_CR);
		xyScallopCen = xy;
		if(xyScallopCen.y > ptMidLo.y+dR)
			xy -= vAng*dR*(bLeft?1:-1);
		xyArr.Add(xy);

		for(long n=0; n<xyArr.GetSize()-1; n++)
			Dom.LineTo(xyArr.GetAt(n), xyArr.GetAt(n+1));

		if(xyScallopCen.y > ptMidLo.y+dR)
		{
			if(bLeft)	Dom.Arc(xyScallopCen, dR, 180+dAngle, 90-dAngle);
			else		Dom.Arc(xyScallopCen, dR, 270,		  90+dAngle);
		}		
	}

	*pDom << Dom;	
}
void CAPlateDrawDanmyun::DimVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	 = (CPlateGirderApp	*)pBx->GetGirder();
	CSection		*pSec	 = pBx->GetSection();

	CVStiff			*pVStiff = pBx->IsJijum() ? pBx->GetJijumStiff() :  pSec->GetVStiff();

	if(!pVStiff) return;

	double dHeight = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	CDPoint vX(1,0), vY(0,1);
	CDPoint B[10];						// 거더의 포인트
	pGir->GetHunchXyDom(pBx,B);
	CDPoint ptMidUp = (B[1] + B[2])/2;
	CDPoint	ptMidLo = ptMidUp - vY*dHeight;	
	
	CDomyun Dom(pDom);	
	Dom.SetScaleDim(20);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	
	double W  = pVStiff->m_dWidth[0];
	double V  = pVStiff->m_dGap[0];	
	double W2 = pVStiff->m_dWidth[1];
	double V2 = pVStiff->m_dGap[1];	

	double	dHLUp			= pVStiff->m_dH[LEFT][UP];
	double	dHLLo			= pVStiff->m_dH[LEFT][DOWN];
	double	dHRUp			= pVStiff->m_dH[RIGHT][UP];
	double	dHRLo			= pVStiff->m_dH[RIGHT][DOWN];
	double	dHLeft			= pVStiff->GetVStiffHeight(TRUE, dHeight);
	double	dHRight			= pVStiff->GetVStiffHeight(FALSE, dHeight);
	double	dLG				= pVStiff->m_dGap[LEFT];
	double	dRG				= pVStiff->m_dGap[RIGHT];

	long nDan = 0;
	double WebT   = pBx->GetFactChain(G_W)->m_dFactChainThick;
	
	if(W != 0)
	{
		Dom.SetDirection("LEFT");

		Dom.DimMoveTo(ptMidLo.x-W-WebT/2, ptMidLo.y, nDan);
		Dom.DimLineTo(dHLLo, nDan, bInput ? "LH" : COMMA(dHLLo));
		Dom.DimLineTo(dHLeft, nDan, COMMA(dHLeft));
		Dom.DimLineTo(dHLUp, nDan, COMMA(dHLUp));

		Dom.SetDirection("TOP");
		Dom.DimMoveTo(ptMidUp.x-WebT/2, ptMidUp.y, nDan);
		Dom.DimLineTo(-W, nDan, bInput ? "LW" : COMMA(W));

		if(bInput)
			Dom.TextOut(ptMidUp.x-W-WebT/2-100, ptMidUp.y-200, "LT");

		if(dLG>0 && dLG<W && dHLLo>0)
		{
			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(ptMidLo.x-WebT/2, ptMidLo.y, nDan, bInput ? "LG" : COMMA(V));
			Dom.DimLineTo(-V, nDan, "");

			Dom.SetDirection("TOP");
			CDPoint cirP = ptMidLo+vY*dHLLo-vX*(WebT/2+V);
			double R = 1;//sqrt(2*(W-V)*(W-V))*2/3;
//			double R = ((W-V)/2+dHeight-H1L)/cos(ToRadian(45))/2;
			Dom.DimArcMoveTo(cirP.x,cirP.y,R,cirP.x-R*sin(ToRadian(45)),cirP.y+R*sin(ToRadian(45)),nDan);
			Dom.DimArcLineTo(cirP.x-R,cirP.y,nDan,"45");
		}
	}

	if(W2 != 0)
	{
		Dom.SetDirection("RIGHT");

		Dom.DimMoveTo(ptMidLo.x+W2+WebT/2, ptMidLo.y, nDan);
		Dom.DimLineTo(dHRLo, nDan, bInput ? "RH" : COMMA(dHRLo));
		Dom.DimLineTo(dHRight, nDan, COMMA(dHRight));
		Dom.DimLineTo(dHRUp, nDan, COMMA(dHRUp));

		Dom.DimMoveTo(ptMidLo.x+W2+WebT/2, ptMidLo.y, nDan+1);
		Dom.DimLineTo(dHeight, nDan+1, COMMA(dHeight));


		Dom.SetDirection("TOP");
		Dom.DimMoveTo(ptMidUp.x+WebT/2, ptMidUp.y, nDan);
		Dom.DimLineTo(W2, nDan, bInput ? "RW" : COMMA(W2));
		
		if(bInput)
			Dom.TextOut(ptMidUp.x+W2+WebT/2+100, ptMidUp.y-200, "RT");

		if(dRG>0 && dRG<W2 && dHRLo>0)
		{
			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(ptMidLo.x+WebT/2, ptMidLo.y, nDan);
			Dom.DimLineToOver(V2, nDan, bInput ? "RG" : COMMA(V2));

			Dom.SetDirection("TOP");
			CDPoint cirP = ptMidLo+vY*dHRLo+vX*(WebT/2+V2);
			double R = 1;//sqrt(2*(W2-V2)*(W2-V2))*2/3;
//			double R = ((W2-V2)/2+dHeight-H1R)/cos(ToRadian(45))/2;
			Dom.DimArcMoveTo(cirP.x,cirP.y,R,cirP.x+R,cirP.y,nDan);
			Dom.DimArcLineTo(cirP.x+R*sin(ToRadian(45)),cirP.y+R*sin(ToRadian(45)),nDan,"45");			
		}
	}
	*pDom << Dom;	
}

// 전체거더의 높이를 표현하는것이 아님
// 횡단면도에서 한개의 치수선으로 전체 거더 높이를 표현할때만 사용할것
void CAPlateDrawDanmyun::DimHeightGirderAll(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeftDim, BOOL bReverse, BOOL bBindDim)
{
	CPlateBridgeApp *pDB		= m_pDataMng->GetBridge();
	
	double dMaxH, dMinH;
	for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp	 *pGirCur	= pDB->GetGirder(nG);
		CPlateBasicIndex *pBxCur	= pGirCur->GetBxMatchByCrossBeam(pBx, pGirCur);

		double dSta = pBxCur->GetStation();
		double dHeightGir = pGirCur->GetHeightGirderBySta(dSta, FALSE);

		if(pBxCur->IsDanbuJijumStt())		dHeightGir -= pDB->m_dHeightTaperStt;
		else if(pBxCur->IsDanbuJijumEnd())	dHeightGir -= pDB->m_dHeightTaperEnd;

		if(nG == 0)
			dMaxH = dMinH = dHeightGir;

		dMaxH = max(dMaxH, dHeightGir);
		dMinH = min(dMinH, dHeightGir);
	}

	if(dMaxH == dMinH)
		DimHeightGirder(pDom, pBx, bLeftDim, bReverse, bBindDim);
	else
		DimHeightGirder(pDom, pBx, bLeftDim, bReverse, bBindDim, dMaxH, dMinH);	
}

void CAPlateDrawDanmyun::DimHeightGirder(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeftDim, BOOL bReverse, BOOL bBindDim, double dMaxH, double dMinH)
{
	CPlateBridgeApp *pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir		= pBx->GetGirder();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	
	CDimDomyunEx DimDom(pDom);
	pOptStd->SetEnvType(&DimDom,HCAD_DIML);
	if(bLeftDim)	DimDom.SetDirection("LEFT");
	else			DimDom.SetDirection("RIGHT");

	double dGirCenter		= pGir->m_dGirderCenter;	
	double dSta				= pBx->GetStation();
	double dHeightGir		= pGir->GetHeightGirderBySta(dSta, FALSE);

	if(pBx->IsDanbuJijumStt())		dHeightGir -= pDB->m_dHeightTaperStt;
	else if(pBx->IsDanbuJijumEnd())	dHeightGir -= pDB->m_dHeightTaperEnd;

	double dYSttPos = pGir->GetElevationDomForExtLine(pBx) - dHeightGir;

	CString szText = _T("");
	if(dMaxH != -1)	szText.Format("%s ~ %s", COMMA(dMinH), COMMA(dMaxH));
	else			szText.Format("%s", COMMA(dHeightGir));
	DimDom.DimMoveTo(dGirCenter, dYSttPos, 1);
	DimDom.DimLineTo(dGirCenter, dHeightGir, szText);

	if(bBindDim && pDB->IsTUGir())
	{
		long nG = pDB->GetQtyGirder() > 2 ? 1 : 0;
		pGir = pDB->GetGirder(nG);
		pBx  = pGir->GetBxMatchByCrossBeam(pBx, pGir);
		double dBindW = pDB->m_BindConc.m_dWidth;

		DimDom.CircleArrow(pGir->m_dGirderCenter-dBindW/2, pGir->GetElevationDomForExtLine(pBx),
			DimDom.Always(5), -DimDom.Always(20), 5, "구속콘크리트", "");	
	}

	// Text Rotate
	CPtrList *pListText = DimDom.GetObjectList(ETT_TEXT);
	POSITION pos = pListText->GetHeadPosition();
	while(pos)
	{
		CObText *pObj = (CObText *)pListText->GetNext(pos);
		pObj->m_TextStyle.Angle += (bReverse ? 180 : 0);
	}
	
	*pDom << DimDom;
}

void CAPlateDrawDanmyun::DrawGuardWall(CDomyun *pDom, CCentSeparation *pGWall, 
									   double BX, double BY)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	CDomyun Dom(pDom);
	long pos = pGWall->GetPlace();
	long nHDanNode = pBridge->GetQtyHDanNode()-1;

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pBridge->GetXyPaveDom(pBx, A);

	double W1 = pGWall->m_W1;
	double W2 = pGWall->m_W2;
	double W3 = pGWall->m_W3;
	double W4 = pGWall->m_W4;
	double W5 = pGWall->m_W5;
	double H1 = pGWall->m_H1;
	double H2 = pGWall->m_H2;
	double H3 = pGWall->m_H3;
	double dSlopSlab = (A[0].y-A[1].y)/(A[0].x-A[1].x);
	double DiffOut = 0;
	
	if(pos==2)	//우측방호벽
	{
		dSlopSlab = (A[nHDanNode+1].y-A[nHDanNode+2].y)/(A[nHDanNode+1].x-A[nHDanNode+2].x);
		if(A[nHDanNode+1] == A[nHDanNode+2])
			dSlopSlab = 0;
	}

	double DiffIn = (W1+W2+W3+W4+W5)*dSlopSlab;


	Dom.MoveTo( 0, DiffOut);
	Dom.LineTo( 0,H1);
	Dom.LineTo(W1,H1+H2);
	Dom.LineTo(W1+W2   ,H1+H2+H3);
	Dom.LineTo(W1+W2+W3,H1+H2+H3);
	Dom.LineTo(W1+W2+W3+W4,H1+H2);
	Dom.LineTo(W1+W2+W3+W4+W5,H1);
	Dom.LineTo(W1+W2+W3+W4+W5, DiffIn);

	Dom.Move(BX,BY);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimGuardWallSide(CDomyun *pDom, CCentSeparation *pGWall, double BX, 
										  double BY,  BOOL bLeft, BOOL bInput, double DimScale)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetScaleDim(DimScale);

	double H1 = pGWall->m_H1;
	double H2 = pGWall->m_H2;
	double H3 = pGWall->m_H3;

	if(bLeft) {
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(0,0,0);
	}
	else {
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(pGWall->GetWidth(),0,0);
	}
	if(!bInput) {
		Dom.DimLineTo(H1,0,COMMA(H1));
		if(H2>0) Dom.DimLineTo(H2,0,COMMA(H2));
		if(H3>0) Dom.DimLineTo(H3,0,COMMA(H3));
		Dom.DimMoveTo(bLeft? 0 : pGWall->GetWidth(), 0, 1);
		Dom.DimLineTo(H1+H2+H3, 1, COMMA(H1+H2+H3));
	}
	else {
		Dom.DimLineTo(H1,0,"H1");
		if(H2>0) Dom.DimLineTo(H2,0,"H2");
		if(H3>0) Dom.DimLineTo(H3,0,"H3");
		if(bLeft)
			Dom.DimMoveTo(0,0,1);
		else
			Dom.DimMoveTo(pGWall->GetWidth(),0,1);
		Dom.DimLineTo(H1+H2+H3,1,COMMA(H1+H2+H3));
	}

	Dom.Move(BX,BY);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimGuardWallTop(CDomyun *pDom, CCentSeparation *pGWall, 
										 double BX, double BY, BOOL bInput,  double DimScale)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetScaleDim(DimScale);

	long   pos = pGWall->GetPlace();

	double W1 = pGWall->m_W1;
	double W2 = pGWall->m_W2;
	double W3 = pGWall->m_W3;
	double W4 = pGWall->m_W4;
	double W5 = pGWall->m_W5;

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(0,pGWall->GetHeight(),0);

	if(pos==2)	//우측방호벽
	{
		W1 = -pGWall->m_W1;
		W2 = -pGWall->m_W2;
		W3 = -pGWall->m_W3;
		W4 = -pGWall->m_W4;
		W5 = -pGWall->m_W5;
		Dom.DimMoveTo(pGWall->GetWidth(),pGWall->GetHeight(),0);
	}

	if(!bInput) {
		Dom.DimLineTo(W1,0,COMMA(fabs(W1)));
		Dom.DimLineTo(W2,0,COMMA(fabs(W2)));
		Dom.DimLineTo(W3,0,COMMA(fabs(W3)));
		Dom.DimLineTo(W4,0,COMMA(fabs(W4)));
		Dom.DimLineTo(W5,0,COMMA(fabs(W5)));
		if(pos==2)
			Dom.DimMoveTo(pGWall->GetWidth(), pGWall->GetHeight(), 1);
		else
			Dom.DimMoveTo(0,pGWall->GetHeight(),1);
		Dom.DimLineTo(W1+W2+W3+W4+W5, 1, COMMA(fabs(W1+W2+W3+W4+W5)));

	}
	else {
		Dom.DimLineTo(W1,0,"W1");
		Dom.DimLineTo(W2,0,"W2");
		Dom.DimLineTo(W3,0,"W3");
		Dom.DimLineTo(W4,0,"W4");
		Dom.DimLineTo(W5,0,"W5");
		if(pos==0 || pos==1) {
			Dom.DimMoveTo(0,pGWall->GetHeight(),1);
			Dom.DimLineTo(W1+W2+W3+W4+W5,1,COMMA(fabs(W1+W2+W3+W4+W5)));
		}
		else if(pos==2) {
			Dom.DimMoveTo(pGWall->GetWidth(),pGWall->GetHeight(),1);
			Dom.DimLineTo(W1+W2+W3+W4+W5,1,COMMA(fabs(W1+W2+W3+W4+W5)));
		}
		// D1치수
		if(pGWall->m_D1 > 0) {
			if(pos==0 || pos==1) {
				Dom.DimMoveTo(-pGWall->m_D1,pGWall->GetHeight(),0,"D1");
				Dom.DimLineTo(pGWall->m_D1,0,"");
			}
			else if(pos==2) {
				Dom.DimMoveTo(pGWall->GetWidth(),pGWall->GetHeight(),0);
				Dom.DimLineToOver(pGWall->m_D1,0,"D1");
			}
		}
	}

	Dom.Move(BX,BY);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawInputSolePlate(CDomyun *pDom, long nRow, long nGirder, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp    *pGir = pBridge->GetGirder(nGirder);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBasicIndex         *pBx = pGir->GetBxOnJijum(nJijum);

	pDom->SetScaleDim(20);

	CDRect Bound, DomArea; // DomArea 개별영역의 크기
	pOptStd->SetEnvType(pDom,HCAD_SMALL_TITLE);
	CDomyun Dom(pDom);

	// SOLE PLATE 상세
	DrawSolePlateDetail(&Dom, pBx, FALSE, nGirder, nJijum);
	Dom.SetCalcExtRect();
	DomArea = Dom.GetExtRect();
	Dom.TextOut((DomArea.left+DomArea.right)/2
		          ,DomArea.bottom+Dom.GetTextHeight(),"솔플레이트");
	*pDom << Dom;

	// 슈박스 상세
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawShoeBoxDetail(&Dom, 0, 0, 1.0, TRUE, 0, nGirder, nJijum);
	Dom.SetCalcExtRect();
	DomArea = Dom.GetExtRect();
	pOptStd->SetEnvType(&Dom,HCAD_SMALL_TITLE);
	Dom.TextOut((DomArea.left+DomArea.right)/2
		          ,DomArea.bottom+Dom.GetTextHeight()+Dom.Always(10),"SHOE BLOCK 상세");
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DimShoeBoxDetail(&Dom, 0, 0, TRUE, nGirder, nJijum);
	Dom.SetCalcExtRect();
	Bound = Dom.GetExtRect();
	Dom.SetCalcExtRect();
	DomArea = Dom.GetExtRect();
	Dom.Move(Bound.right-DomArea.left+Dom.Always(25)
 	         ,(Bound.top-Bound.bottom)/2 - (DomArea.top-DomArea.bottom)/2);
	*pDom << Dom;
	/*
	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	Dom.LineTo(Bound.left-Bound.Width()*0.2,
				Bound.top-Bound.Height()*0.5,
				Bound.right+Bound.Width()*0.3,
				Bound.bottom+Bound.Height()*0.05);
	*pDom << Dom;

	Dom.SetCalcExtRect();
	CDRect BoundRect = Dom.GetExtRect();	
	pOptStd->SetEnvType(&Dom,HCAD_HIDD);
	Dom.LineTo(0, BoundRect.top-BoundRect.Height()/2, 
		         0, BoundRect.bottom+BoundRect.Height()/7);

	Dom.Invalidate();
	*pDom << Dom;
	*/
}


void CAPlateDrawDanmyun::DrawSolePlateDetail(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bGen, long nGirder, long nJijum)
{
	CDomyun		Dom(pDom);
	// DRAWING 중심 : 솔플레이트 중심
	DrawSolePlatePlanDetail(&Dom, nGirder, nJijum);
	DimSolePlatePlan(&Dom, bGen, nGirder, nJijum);
	Dom.SetCalcExtRect();
	double moveY = Dom.GetExtHeight()/2;
	*pDom << Dom;

	// DRAWING 중심 : 솔플레이트 중심하단
	DrawSolePlateSideDetail(&Dom, pBx, nGirder, nJijum);
	Dom.SetCalcExtRect();
	moveY += Dom.GetExtHeight();
	DimSolePlateSide(&Dom, pBx, bGen, nGirder, nJijum);
	Dom.Move(0,-(moveY+Dom.Always(5)));		
	*pDom << Dom;		
}
void CAPlateDrawDanmyun::DrawSolePlatePlanDetail(CDomyun *pDom,  long nGirder, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp    *pGir = pBridge->GetGirder(nGirder);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();


	double W  = pGir->GetShoeByJijumNo(nJijum)->m_dW_SolePlate;
	double W2 = pGir->GetShoeByJijumNo(nJijum)->m_dW2_SolePlate;
	double W3 = pGir->GetShoeByJijumNo(nJijum)->m_dW3_SolePlate;
	double H  = pGir->GetShoeByJijumNo(nJijum)->m_dH_SolePlate;
	double H2 = pGir->GetShoeByJijumNo(nJijum)->m_dH2_SolePlate;
	double H3 = pGir->GetShoeByJijumNo(nJijum)->m_dH3_SolePlate;
	long   nDir     = pGir->GetShoeByJijumNo(nJijum)->m_cDirectionShoe;
	double HoleSize = pGir->GetShoeByJijumNo(nJijum)->m_dDia_SolePlateHole;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	// SOLE PLATE
	if(nDir==4) 
	{
		W /= sqrt(2.0);
		W2 /= sqrt(2.0);
		H /= sqrt(2.0);
		H2 /= sqrt(2.0);
	}

	Dom.Rectangle(-W/2-W2,-H/2-H2,W/2+W2,H/2+H2);
	Dom.MoveTo(-W/2, H/2);
	Dom.LineTo( W/2, H/2);
	Dom.LineTo( W/2,-H/2);
	Dom.LineTo(-W/2,-H/2);
	Dom.LineTo(-W/2, H/2);
	if(nDir==4) Dom.Rotate(ConstPi/4,0,0);
	
	Dom.Circle(-W3/2,H3/2,HoleSize/2);
	Dom.Circle(+W3/2,H3/2,HoleSize/2);
	Dom.Circle(-W3/2,-H3/2,HoleSize/2);
	Dom.Circle(+W3/2,-H3/2,HoleSize/2);
	
	*pDom << Dom;
}
void CAPlateDrawDanmyun::DimSolePlatePlan(CDomyun *pDom, BOOL bGen, long nGirder, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp    *pGir = pBridge->GetGirder(nGirder);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double W  = pGir->GetShoeByJijumNo(nJijum)->m_dW_SolePlate;
	double W2 = pGir->GetShoeByJijumNo(nJijum)->m_dW2_SolePlate;
	double W3 = pGir->GetShoeByJijumNo(nJijum)->m_dW3_SolePlate;
	double H  = pGir->GetShoeByJijumNo(nJijum)->m_dH_SolePlate;
	double H2 = pGir->GetShoeByJijumNo(nJijum)->m_dH2_SolePlate;
	double H3 = pGir->GetShoeByJijumNo(nJijum)->m_dH3_SolePlate;
	// DRAWING 중심 : 솔플레이트 중심
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-W/2-W2,H/2+H2,0);
	if(bGen)
	{
		Dom.DimLineTo(W2,0,COMMA(W2));
		Dom.DimLineTo(W,0,COMMA(W));
		Dom.DimLineTo(W2,0,COMMA(W2));
	}
	else
	{
		Dom.DimLineTo(W2,0,"W2");
		Dom.DimLineTo(W,0,"W");
		Dom.DimLineTo(W2,0,"W2");
	}

	Dom.DimMoveTo(-W3/2,H3/2,0);
	if(bGen)	
		Dom.DimLineTo(W3,0,COMMA(W3));
	else
		Dom.DimLineTo(W3,0,"W3");	

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(W/2+W2,-H/2-H2,0);
	
	if(bGen)
	{
		Dom.DimLineTo(H2,0,COMMA(H2));
		Dom.DimLineTo(H,0,COMMA(H));
		Dom.DimLineTo(H2,0,COMMA(H2));
	}
	else
	{
		Dom.DimLineTo(H2,0,"H2");
		Dom.DimLineTo(H,0,"H");
		Dom.DimLineTo(H2,0,"H2");
	}	
	
	Dom.DimMoveTo(W3/2,-H3/2,0);
	if(bGen)	
		Dom.DimLineTo(H3,0,COMMA(H3));
	else
		Dom.DimLineTo(H3,0,"H3");	

	Dom.SetDirection("TOP");
	Dom.GiSiArrow(-W/4, 0, W/2, 0, 0,"교축방향");
	
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawSolePlateSideDetail(CDomyun *pDom, CPlateBasicIndex *pBx, long nGirder, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp    *pGir = pBridge->GetGirder(nGirder);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double   W  = pGir->GetShoeByJijumNo(nJijum)->m_dW_SolePlate;
	double   W2 = pGir->GetShoeByJijumNo(nJijum)->m_dW2_SolePlate;
	double    T = pGir->GetShoeByJijumNo(nJijum)->m_dThick_SolePlate;
	double slop = pGir->GetGiulJongDanShoe(pBx);

	W += W2 * 2;			

	double T1 = T - W/2*slop;
	double T2 = T + W/2*slop;

	if(pGir->GetShoeByJijumNo(nJijum)->m_bLevelSolePlate)
	{
		T1 = T;
		T2 = T;
	}
	if(pGir->GetShoeByJijumNo(nJijum)->m_bLevelSolePlate)
	{
		T1 = T;
		T2 = T;
	}

	CDomyun Dom(pDom);
	Dom.SetScaleDim(Dom.GetScaleDim());
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	// SOLE PLATE
	Dom.LineTo(-W/2, 0,  W/2,  0);
	Dom.LineTo(-W/2, 0, -W/2, T1);
	Dom.LineTo( W/2, 0,  W/2, T2);
	Dom.LineTo(-W/2,T1,  W/2, T2);	

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimSolePlateSide(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bGen,  long nGirder, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp    *pGir = pBridge->GetGirder(nGirder);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double W  = pGir->GetShoeByJijumNo(nJijum)->m_dW_SolePlate;
	double W2 = pGir->GetShoeByJijumNo(nJijum)->m_dW2_SolePlate;
	double  T = pGir->GetShoeByJijumNo(nJijum)->m_dThick_SolePlate;

	W += W2 * 2;

	// DRAWING 중심 : 솔플레이트 중심하단
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	double slop = pGir->GetGiulJongDanShoe(pBx);
	double T1 = T - W/2*slop;
	double T2 = T + W/2*slop;

	if(pGir->GetShoeByJijumNo(nJijum)->m_bLevelSolePlate)
	{
		T1 = T;
		T2 = T;
	}
	if(pGir->GetShoeByJijumNo(nJijum)->m_bLevelSolePlate)
	{
		T1 = T;
		T2 = T;
	}


	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(-W/2,0,0);
	Dom.DimLineTo(W,0,COMMA(W));

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(-W/2,0,0);
	if(bGen)
		Dom.DimLineToOver( T1,0,COMMA(RoundUp(T1,0)));
	else
		Dom.DimLineToOver( T1,0,"T1");

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo( W/2,0,0);
	if(bGen)
		Dom.DimLineToOver( T2,0,COMMA(RoundUp(T2,0)));
	else
		Dom.DimLineToOver( T2,0,"T2");

	Dom.SetDimExo(0);
	Dom.SetDimObq(-45);
	Dom.DimMoveTo(0,0,0);
	if(bGen)
		Dom.DimLineTo( T,0,COMMA(T));
	else
		Dom.DimLineTo( T,0,"T");
	*pDom << Dom;

	if(pGir->GetShoeByJijumNo(nJijum)->m_bLevelSolePlate != TRUE)
	{	
		// 종경사 표시
		Dom.SetDirection("TOP");
		CString str;
		if(bGen)
			str.Format("S = %.4f(%%)",slop);
		else
			str.Format("S(%%)",slop);
		Dom.GiSiArrow(-W/4, T*1.5, W/2, 0, 0,str);
		Dom.Rotate(atan(slop),0,T*1.5);
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawShoeBoxDetail(CDomyun *pDom, double BX, double BY, 
								double dFactorThick, BOOL bLongitude, double dBogus,  long nGirder, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp    *pGir = pBridge->GetGirder(nGirder);
	CPlateBasicIndex         *pBx = pGir->GetBxOnJijum(nJijum);
	CDPoint sp,ep;
	CShoe *pShoe = pGir->GetShoeByJijumNo(nJijum);
	double sH1	= dFactorThick*pShoe->m_dThick_SolePlate;
	double sH2	= dFactorThick*pShoe->m_dThick_Shoe;
	double sH3	= dFactorThick*pShoe->m_dThick_Mortar;	// 몰탈의 높이(1:1로 적용), 폭도 동일
	double sH4	= pShoe->m_dThick_Block;
	double H	= pShoe->m_dH_SolePlate;
	double H2	= pShoe->m_dH2_SolePlate;
	double W	= pShoe->m_dW_SolePlate;
	double W2	= pShoe->m_dW2_SolePlate;

	double dWidthPlate= bLongitude ? W + W2*2 : H + H2*2;
	double dWidthShoe = bLongitude ? W  : H;
	double blockW1	  = bLongitude ? pShoe->m_dW1_Mortal : pShoe->m_dW3_Mortal;
	double blockW2	  = bLongitude ? pShoe->m_dW2_Mortal : pShoe->m_dW3_Mortal;
	
	if(dBogus != 0) dBogus = pGir->GetThickJewon(G_F_L, pBx) - dBogus;
	CDomyun Dom(pDom);

	// 슈 블럭
	sp.x = -blockW1-sH3*2-dWidthShoe/2;	
	sp.y = 0;	
	ep.x = sH3*2+blockW2+dWidthShoe/2;	
	ep.y = sH4 + dBogus;
	Dom.Rectangle(sp,ep);

	// 몰탈
	sp.x = -sH3*2-dWidthShoe/2;	
	sp.y = sH4 + dBogus;
	ep.x = sH3*2+dWidthShoe/2;
	ep.y = sH4 + dBogus;
	Dom.MoveTo(sp);
	Dom.LineTo(sp.x+sH3,sp.y+sH3);
	Dom.LineTo(ep.x-sH3,ep.y+sH3);
	Dom.LineTo(ep);
	Dom.LineTo(sp);
	
	// 슈 
	sp.x = -dWidthShoe/2;
	sp.y = sH4 + dBogus + sH3;
	ep.x = dWidthShoe/2;
	ep.y = sH4 + dBogus + sH3 + sH2/2;
	Dom.Rectangle(sp,ep);
//	pDST->DstHatchInRect(pDom,sp,ep,30);
	CDRect rect;
	rect.left   = sp.x;
	rect.right  = ep.x;
	rect.bottom = sp.y;
	rect.top    = ep.y;
	Dom.HatchRect(rect, "ANSI31", Dom.GetScaleDim());
	//
	sp.x = -dWidthShoe/2+30;
	sp.y = sH4 + dBogus + sH3 + sH2/2;
	ep.x = dWidthShoe/2-30;
	ep.y = sH4 + dBogus + sH3 + sH2*3/4;
	Dom.Rectangle(sp,ep);
	//
	sp.x = -dWidthShoe/2;
	sp.y = sH4 + dBogus + sH3 + sH2*3/4;
	ep.x = dWidthShoe/2;
	ep.y = sH4 + dBogus + sH3 + sH2;
	Dom.Rectangle(sp,ep);
//	pDST->DstHatchInRect(pDom,sp,ep,30);
	rect.left   = sp.x;
	rect.right  = ep.x;
	rect.bottom = sp.y;
	rect.top    = ep.y;
	Dom.HatchRect(rect, "ANSI31", Dom.GetScaleDim());

	// 솔플레이트		
	double JSlop = pGir->GetGiulJongDanShoe(pBx);			//종단 경사
	double T1	 = bLongitude ? sH1 - JSlop*dWidthPlate/2 : sH1;//솔플레이트 좌측면 두께
	double T2	 = bLongitude ? sH1 + JSlop*dWidthPlate/2 : sH1;//솔플레이트 우측면 두께
	
	sp.x = -dWidthPlate/2;
	sp.y = sH4 + dBogus + sH3 + sH2;
	ep.x = dWidthPlate/2;
	ep.y = sH4 + dBogus + sH3 + sH2;
	Dom.MoveTo(sp);
	Dom.LineTo(sp.x,sp.y+T1);
	Dom.LineTo(ep.x,ep.y+T2);
	Dom.LineTo(ep);
	Dom.LineTo(sp);

	Dom.Move(BX,BY);
	*pDom << Dom;

}
void CAPlateDrawDanmyun::DimShoeBoxDetail(CDomyun *pDom, double BX, double BY, BOOL bInput,  long nGirder, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp    *pGir = pBridge->GetGirder(nGirder);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CShoe *pShoe = pGir->GetShoeByJijumNo(nJijum);

	double sH1	= pShoe->m_dThick_SolePlate;
	double sH2	= pShoe->m_dThick_Shoe;
	double sH3	= pShoe->m_dThick_Mortar;	// 몰탈의 높이(1:1로 적용), 폭도 동일
	double sH4	= pShoe->m_dThick_Block;
	double H	= pShoe->m_dH_SolePlate;
	double H2	= pShoe->m_dH2_SolePlate;
	double W	= pShoe->m_dW_SolePlate;
	double W2	= pShoe->m_dW2_SolePlate;

	BOOL bLongitude = TRUE;
	double dWidthShoe = bLongitude ? W  : H;
	double blockW1	  = bLongitude ? pShoe->m_dW1_Mortal : pShoe->m_dW3_Mortal;
	double blockW2	  = bLongitude ? pShoe->m_dW2_Mortal : pShoe->m_dW3_Mortal;

	CDomyun Dom(pDom);
	Dom.SetScaleDim(Dom.GetScaleDim());	
	
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	if(bInput)
	{
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(-sH3*2-dWidthShoe/2-blockW1,sH4,0);
		Dom.DimLineTo(blockW1,0,"mW1");

		Dom.DimMoveTo(sH3*2+dWidthShoe/2,sH4,0);
		Dom.DimLineTo(blockW2,0,"mW2");
	}

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(-sH3-dWidthShoe/2-blockW1,0,0);
	Dom.DimLineTo(sH4,0,"sH4");
	Dom.DimLineTo(sH3,0,"sH3");
	Dom.DimLineToExtend(sH2,0,"","sH2");
	Dom.DimLineTo(sH1,0,"sH1");

	if(bInput) 
	{
		Dom.DimMoveTo(-sH3-dWidthShoe/2-blockW1,0,1);
		Dom.DimLineTo(sH1+sH2+sH3+sH4,1,COMMA(sH1+sH2+sH3+sH4));
	}

	if(!bInput) 
	{
		Dom.SetDirection("TOP");
		Dom.GiSiArrowExtend(dWidthShoe/2,sH4+sH3
						   ,Dom.Always(10),Dom.Always(25),1, 
						   "SHOE EL","(무수축몰탈 상단)");
		Dom.GiSiArrowExtend(sH3+blockW2+dWidthShoe/2+Dom.Always(5), 0
						   ,Dom.Always(10),Dom.Always(25),2, 
						   "교좌면 EL","(하부구조물 상단)");
	}

	Dom.SetTextAlignHorz(TA_LEFT);
	double TextHeight = Dom.GetTextHeight();
	Dom.TextOut(-sH3-dWidthShoe/2,-TextHeight*2.0,"sH1 : SOLE PLATE의 두께 (SHOE CENTER 기준)");
	Dom.TextOut(-sH3-dWidthShoe/2,-TextHeight*4.0,"sH2 : 교좌장치의 높이");
	Dom.TextOut(-sH3-dWidthShoe/2,-TextHeight*6.0,"sH3 : 무수축 몰탈의 높이");
	Dom.TextOut(-sH3-dWidthShoe/2,-TextHeight*8.0,"sH4 : SHOE BLOCK의 높이");

	Dom.Move(BX,BY);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawInputScallop(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	double R = pBridge->GetGirder(-1)->m_pSangse->m_Scallop_R1;
	CDomyun Dom(pDom);
	DrawScallopGen(&Dom, R);
	DimScallopGen(&Dom, R);

	*pDom << Dom;	
}

void CAPlateDrawDanmyun::DrawScallopGen(CDomyun *pDom, double R)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBx    = pGir->GetBxOnJijum(0);
	
	double T1 = 12;
	double T2 = 24;
	double height = pGir->GetHeightGirderByJijum(0)/2;
	double Length = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);

	CDomyun Dom(pDom);
	Dom.SetScaleDim(Dom.GetScaleDim());
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	Dom.MoveTo(0,0);
	Dom.LineToEx(-T1/2, 0);
	Dom.LineToEx(0, height);
	Dom.MoveTo(0,0);
	Dom.LineToEx(T1/2, 0);
	Dom.LineToEx(0, height);

	Dom.Rectangle(-Length, 0, Length, - T2);
//
//	Dom.MoveTo(-Length*2/3, 0);
//	Dom.LineToEx(Length - T1/2, 0);
//	Dom.LineToEx(0, height);
	
	Dom.MoveTo(-Length*3/4, 0);
	Dom.LineToEx(0, height);
//
//	Dom.MoveTo(Length, 0);
//	Dom.LineToEx(-Length + T1/2, 0);
//	Dom.LineToEx(0, height);
	
	Dom.MoveTo(Length*3/4, 0);
	Dom.LineToEx(0, height);

	Dom.Arc(-T1/2, 0, R, 90, 90);
	Dom.Arc( T1/2, 0, R, 0, 90);
	
	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning( -Length, height,  Length, height,FALSE,1);


	*pDom << Dom;

}

void CAPlateDrawDanmyun::DimScallopGen(CDomyun *pDom, double R)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBx    = pGir->GetBxOnJijum(0);
	
	double T1 = 12;
	double T2 = 24;
	double height = pGir->GetHeightGirderByJijum(0)/2;
	double Length = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);

	CDomyun Dom(pDom);
	Dom.SetScaleDim(Dom.GetScaleDim());
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CString str;
	str = "보강재 (t = 두께)";
	double len = Dom.GetTextWidth(str);
	Dom.CircleArrow(Length/2, height/2, len, len, 0,str,"");
	*pDom << Dom;

	CDPoint xy, vDir;
	vDir = CDPoint(1,1);
	str = "  R  ";
	len = Dom.GetTextWidth(str);
	xy = CDPoint(T1/2, 0) + R * vDir.Unit();
	Dom.GiSiArrowExtend(xy.x, xy.y, len, len, 0,str, "");
	Dom.Rotate(xy, vDir);
	
	CString str1, str2;
	str1 = " Web ";
	str2 = " Flange ";
	double len1 = Dom.GetTextWidth(str1);
	double len2 = Dom.GetTextWidth(str2);
	Dom.GiSiArrowExt(T1/2, height/2, len1, len1, 1, str1);
	Dom.GiSiArrowExt(Length/2, -T2, len1, len2, 7, str2);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawInputHStiff(CDomyun *pDom, CPlateBasicIndex *pBxSp, CPlateBasicIndex *pBxSc, double scale, bool bDrawDetailDlg)
{
	CDomyun Dom(pDom);

	DrawHStiff_withSplice(&Dom, pBxSp, scale);
	
	Dom.Move(0,600);

	DrawHStiff_withVStiff(&Dom, pBxSc, scale);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawHStiff_withSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double scale, bool bDrawDetailDlg)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGirCen = pBridge->GetGirder(-1);
	CPlateGirderApp *pGir    = pBx ? (CPlateGirderApp*)pBx->GetGirder() : pGirCen;	
	CPlateSplice    *pSp     = pBx ? pBx->GetSplice() : NULL;

	double SpliceT = 10;
	double SpliceW = 505;
	double mA = 40;
	double mC = 85;
	if(pSp)
	{
		mA = pSp->m_mA == 0 ? 40 : pSp->m_mA;
		mC = pSp->m_mC == 0 ? 85 : pSp->m_mC;
		SpliceT  = (pSp->m_mB * pSp->m_mC) == 0 ? 10 : pSp->m_mT;	
		SpliceW  = (pSp->m_mB * pSp->m_mC) == 0 ? 505 : (pSp->m_mA + pSp->m_mB * pSp->m_mC + pSp->m_mC1/2) * 2;
	}

	CDomyun Dom(pDom);

	// 이음판 제원
	

	// 수평보강재 폭과 두께.
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxHs  = Finder.GetBxLast(BX_PLATEHSTIFF,NULL,pBx);
	double HT   = 12;
	double HW   = 140;
	CWebHStiff  *pHs = pBxHs->GetWebHStiff();
	if (pHs)
	{
		if(pHs->m_dThickHStiff[0] > 0)
		{
			HT   = pHs->m_dThickHStiff[0]; 
			HW   = pHs->m_dWidthHStiff[0];
		}
	}

	double S    = pGirCen->m_pSangse->m_HStiff_Splice_B;	// 수평보강재와 수직보강재의 간격 
	double D    = pGirCen->m_pSangse->m_HStiff_VStiff_D;	// 복부판측 직선길이
	double WebT = pBxHs->GetFactChain(2)->m_dFactChainThick;

	// 상세 DRAWING의 기준점은 수직보강재의 상단 중심(복부판 접촉면)
	double Len  = HW*2+SpliceW; // 각각FIX
	double wing = Dom.Always(5);
	double Ang  = 45;
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.LineTo(-Len,WebT ,Len,WebT);	// Web PALTE
	Dom.LineTo(-Len,   0 ,Len,   0);
	Dom.Rectangle(-SpliceW/2, SpliceT+WebT ,SpliceW/2, WebT);	// SPLICE PLATE
	Dom.Rectangle(-SpliceW/2,-SpliceT ,SpliceW/2, 0);

	///////////
	Dom.LineTo(-Len,-HW, -(SpliceW/2+S+(HW-D)),-HW);	// 수평보강재(좌)
	Dom.LineTo(-(SpliceW/2+S+(HW-D)),-HW,
		         -(SpliceW/2+S),-D); 
	Dom.LineTo(-(SpliceW/2+S), 0, -(SpliceW/2+S),-D); 
	Dom.LineTo(Len,-HW, (SpliceW/2+S+(HW-D)),-HW);	// 수평보강재(우)
	Dom.LineTo((SpliceW/2+S+(HW-D)),-HW,
		         (SpliceW/2+S),-D); 
	Dom.LineTo((SpliceW/2+S), 0, (SpliceW/2+S),-D);

	// 볼트그리기 : 평면도 상세와 일치하여야함
	// 6개를 그리는 것으로 가정하여 돌린다.............
	double sx	 = -SpliceW/2+mA;		//-200
	double boltT =   18;
	double boltL =   65;
	double boltHeadW = 30;
	double boltHeadT = 10;
	pOptStd->SetEnvType(&Dom,HCAD_STLS);

	long n = 0;
	for(n=0; n<3; n++) {
		if(n>0) sx += mC;		//75
		pOptStd->SetEnvType(&Dom,HCAD_STLS);
		Dom.Rectangle(sx-boltHeadW/2,SpliceT+WebT,sx+boltHeadW/2,SpliceT+WebT+boltHeadT);
		Dom.Rectangle(sx-boltT/2,SpliceT+WebT,sx+boltT/2,-boltL);
		Dom.Rectangle(sx-boltHeadW/2,-SpliceT,sx+boltHeadW/2,-SpliceT-boltHeadT);
		pOptStd->SetEnvType(&Dom,HCAD_CENT);
		Dom.LineTo(sx,SpliceT+WebT+boltHeadT*2,sx,-boltL-boltHeadT);
	}
	sx = SpliceW/2-mA;		//200
	for(n=0; n<3; n++) {
		if(n>0) sx -= mC;		//75
		pOptStd->SetEnvType(&Dom,HCAD_STLS);
		Dom.Rectangle(sx-boltHeadW/2,SpliceT+WebT,sx+boltHeadW/2,SpliceT+WebT+boltHeadT);
		Dom.Rectangle(sx-boltT/2,SpliceT+WebT,sx+boltT/2,-boltL);
		Dom.Rectangle(sx-boltHeadW/2,-SpliceT,sx+boltHeadW/2,-SpliceT-boltHeadT);
		pOptStd->SetEnvType(&Dom,HCAD_CENT);
		Dom.LineTo(sx,SpliceT+WebT+boltHeadT*2,sx,-boltL-boltHeadT);
	}

	// 절단선
	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(-Len, -HW-wing, -Len, WebT+wing);
	Dom.CutLightning( Len, -HW-wing,  Len, WebT+wing);
	////////////////////////////////////////////////////////////////////
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(scale);
	CString	str = "수평보강재";
	double gisilen = Dom.GetTextWidth(str);
	Dom.SetDirection("TOP");
	Dom.GiSiArrowExtend(-Len/2, -WebT,gisilen, -gisilen,3, str, _T(""));
	str = "복부 이음판";
	gisilen = Dom.GetTextWidth(str);
	Dom.GiSiArrowExtend(SpliceW/2-Dom.Always(1), WebT+SpliceT, gisilen, gisilen,1, str, _T(""));
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(-(SpliceW/2+S),0,0);

	if(bDrawDetailDlg)
		Dom.DimLineToOverExtend(S,0,"S",COMMA(S));
	else
		Dom.DimLineToOver(S,0,COMMA(S));

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawHStiff_withVStiff(CDomyun *pDom, CPlateBasicIndex *pBx, double scale, bool bDrawDetailDlg)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGirCen    = pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);

	// 수직보강재 제원
	double VT	= 12;
	double VW	= 150;
	// 수평 부재 폭과 두께
	double HT   = 12;
	double HW   = 130;
	// 현장이음 폭
	double SpliceW  = 505;
	// 상세 DRAWING의 기준점은 수직보강재의 상단 중심(복부판 접촉면)
	double B    = pGirCen->m_pSangse->m_HStiff_VStiff_B;	// 수평보강재와 수직보강재의 간격 
	double D    = pGirCen->m_pSangse->m_HStiff_VStiff_D;	// 복부판측 직선길이
	double WebT = pBx->GetFactChain(2)->m_dFactChainThick;
	
	CPlateBxFinder Finder(pBridge->GetGirder(0));
	CPlateBasicIndex *pBxTemp	= Finder.GetBxLast(BX_PLATEHSTIFF,NULL,pBx);

	CWebHStiff  *pHs = pBxTemp->GetWebHStiff();
	if (pHs)
	{
		if(pHs->m_dThickHStiff[0] > 0)
		{
			HT   = pHs->m_dThickHStiff[0]; 
			HW   = pHs->m_dWidthHStiff[0];
		}
	}

	pBxTemp = Finder.GetBxFirst(BX_SPLICE);
	CPlateSplice	 *pSp	= pBxTemp ? pBx->GetSplice() : NULL;
	if(pSp)
		SpliceW =  (pSp->m_mB * pSp->m_mC) == 0 ? 505 : (pSp->m_mA + pSp->m_mB * pSp->m_mC + pSp->m_mC1/2) * 2;	

	double Len  = bDrawDetailDlg ? HW*2+SpliceW : 350 ;	

	double wing = Dom.Always(5);
	double Ang  = 45;
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.LineTo(-Len,WebT ,Len,WebT);	// Web PALTE
	Dom.LineTo(-Len,   0 ,Len,   0);
	Dom.Rectangle(-VT/2,0,VT/2,-VW);	// 수직보강재
	Dom.LineTo(-Len,-HW, -(VT/2+B+(HW-D)),-HW);	// 수평보강재(좌)
	Dom.LineTo(-(VT/2+B+(HW-D)),-HW,
		         -(VT/2+B),-D); 
	Dom.LineTo(-(VT/2+B), 0, -(VT/2+B),-D); 
	Dom.LineTo(Len,-HW, (VT/2+B+(HW-D)),-HW);	// 수평보강재(우)
	Dom.LineTo((VT/2+B+(HW-D)),-HW,
		         (VT/2+B),-D); 
	Dom.LineTo((VT/2+B), 0, (VT/2+B),-D);
	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(-Len, -HW-wing, -Len, WebT+wing);	// 절단선
	Dom.CutLightning( Len, -HW-wing,  Len, WebT+wing);	// 절단선

	////////////////////////////////////////////////////////////////////
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(scale);
	if (bDrawDetailDlg)
	{
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(VT/2+B,-D,0);
		Dom.DimLineToOverExtend(D,0,"D",COMMA(D));

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(VT/2,-D,0);
		Dom.DimLineToExtend(B,0,"B",COMMA(B));
	}
	else
	{
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(-VT/2-B,WebT,0,COMMA(B));
		Dom.DimLineTo(B,0,"");
		Dom.DimLineTo(VT,0,COMMA(VT));
		Dom.DimLineToOver(B,0,COMMA(B));

		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(Len,-HW,0);
		Dom.DimLineTo(HW-D,0,COMMA(HW-D));
		Dom.DimLineToOver(D,0,COMMA(D));
		Dom.DimMoveTo(Len,-HW,1);
		Dom.DimLineTo(HW,1,COMMA(HW));

		Dom.SetDirection("TOP");
		CDPoint xy, xy1;
		xy = CDPoint(-VT/2-B, -D);
		xy1 = CDPoint(-VT/2-B-(HW-D),-HW);
		double R = HW-D;//~(xy - xy1);
		Dom.DimArcMoveTo(xy.x,xy.y,R,xy1.x,xy1.y,1);
		Dom.DimArcLineTo(xy.x, xy.y - (HW-D),1,COMMA(Ang));
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xy.x,xy.y - (HW-D),1);
		Dom.DimLineTo(VT+B*2,1,COMMA(VT+B*2));
		Dom.SetDirection("TOP");
		xy1 = CDPoint(VT/2+B, -D);
		xy = CDPoint(VT/2+B+(HW-D),-HW);
		R = HW-D;//~(xy - xy1);
		Dom.DimArcMoveTo(xy1.x,xy1.y,R,xy1.x,xy1.y-(HW-D),1);
		Dom.DimArcLineTo(xy.x, xy.y,1,COMMA(Ang));
	}
	CString	str = "수평보강재";
	double gisilen = Dom.GetTextWidth(str);
	Dom.GiSiArrowExtend(-Len/2, -WebT, gisilen, -gisilen, 3,str, _T(""));
	str = "수직보강재";
	gisilen = Dom.GetTextWidth(str);
	Dom.GiSiArrowExtend(0, -WebT,	gisilen, gisilen, 1, str, _T(""));

	*pDom << Dom;
}


void CAPlateDrawDanmyun::DrawInputDanTaper(CDomyun *pDom, double dScale)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	double t1	 = 12;
	double t2	 = 18;

	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawFactDetail_VType(&Dom, t1, t2, dScale, TRUE, 2, 2, 60, FALSE, TRUE);

	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	CString str;
	double H = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double B = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	Dom.SetTextAngle(ToDegree(atan(H/B)));
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.SetDirection("TOP");
	str = " 공장이음위치 ";
	Dom.GiSiArrowExtend(0,0,Dom.Always(15),-Dom.GetTextWidth(str),6,str,"");

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawFactDetail_VType(CDomyun *pDom,double T1,double T2, double Scale
									, long nUpDown,double termW, double termH,double AngleV, 
									BOOL bCenter, BOOL bDimDraw, BOOL bShop, BOOL bDim)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	// 기본 두께 확대
	long nS = 2;
	T1 *= nS;
	T2 *= nS;
	termW *= nS;
	termH *= nS;

	// DRAWING의 기준은 이음면의 중심하단,
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	double minT = min(T1,T2);
	double maxT = max(T1,T2);

	// 용접될 부재의 길이
	double shortLen = 50 ;	
	double longLen = 125;

	// 경사결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;

	// 플레이트,플레이트
	double weldLen = termW/2+(minT-termH)*tan(ToRadian(AngleV/2));
	double slopLen = (maxT-minT)/slop;						

	if (slopLen+weldLen >= longLen)
		longLen = weldLen+slopLen+20;
	CDPoint xp = CDPoint(weldLen,minT);						// (4)
	CDPoint slopP = CDPoint(weldLen+slopLen, maxT);			// (0)
	Dom.LineTo(-shortLen,0, longLen,0);					// (1)
	Dom.LineTo(-shortLen,minT, -weldLen,minT);			// (2)
	Dom.LineTo(slopP.x,slopP.y, longLen,maxT);			// (3)
	Dom.LineTo(-termW/2,termH, -weldLen,minT);		// 작은쪽 V(5)
	Dom.LineTo( termW/2,termH, xp.x,xp.y);			// 큰쪽 V(6)
	Dom.LineTo( xp.x,xp.y, slopP.x,slopP.y);   		// Taper(7)
	Dom.LineTo(-termW/2,0,-termW/2,termH);
	Dom.LineTo( termW/2,0, termW/2,termH);

	// 좌우측 CUTTING (Arc/2*2)
	Dom.Arc(-shortLen,minT,   -shortLen-minT/10, minT*3/4, -shortLen,minT/2); // LEFT
	Dom.Arc( -shortLen,0, -shortLen+minT/10, minT*1/4, -shortLen,minT/2);
	Dom.Arc(longLen,maxT,   longLen-minT/10, maxT*3/4, longLen,maxT/2); // RIGHT
	Dom.Arc(longLen,0, longLen+maxT/10, maxT*1/4, longLen,maxT/2);

	// 다듬질면 SOLID
	pOptStd->SetEnvType(&Dom,HCAD_HATH);
	// SOLID HATCH DATA
	CDPoint xy[7];
	xy[0].x = -termW/2;		xy[0].y = 0;
	xy[1].x =  termW/2;		xy[1].y = 0;
	xy[2].x =  termW/2;		xy[2].y = termH;
	xy[3].x =  xp.x;		xy[3].y = xp.y;
	xy[4].x = -xp.x;		xy[4].y = xp.y;
	xy[5].x = -termW/2;		xy[5].y = termH;
	GetXyMatchSegAndSeg( xy[5], xy[4], xy[2], xy[3], xy[6]);
	CDPoint cRadius = xy[3] - xy[6];
	double Radius = ~(cRadius);

	// 다듬질면 ARC
	Dom.Arc(xy[6].x, xy[6].y, Radius, AngleV, AngleV);

	// SOLID HATCH
	CDDWordArray BoundArea;
	BoundArea.Add(0);	BoundArea.Add(xy[4].x);	BoundArea.Add(xy[4].y);	BoundArea.Add(xy[3].x); BoundArea.Add(xy[3].y);

	BoundArea.Add(1);	BoundArea.Add(xy[6].x); BoundArea.Add(xy[6].y);	BoundArea.Add(Radius);
	BoundArea.Add(AngleV); BoundArea.Add(AngleV*2);	//시작각, 끝각
	BoundArea.Add(1);
	Dom.Hatch(&BoundArea,"ANSI31", Dom.GetScaleDim());

	//Dom.Solid(-termW/2,0, termW/2,0,  termW/2,termH, -termW/2,termH);
	Dom.HatchRect(-termW/2,0, termW/2,0,  termW/2,termH, -termW/2,termH,"ANSI31", Dom.GetScaleDim());
	//Dom.Solid(-weldLen,minT, -termW/2,termH, termW/2,termH, weldLen,minT);
	Dom.HatchRect(-weldLen,minT, -termW/2,termH, termW/2,termH, weldLen,minT,"ANSI31", Dom.GetScaleDim());	
 
	// 방향별,상하별로 조정(MIRROR)
	if(nUpDown==-1) {
		Dom.Mirror(0,FALSE);		// 상하로 MIRROR
		if(T1>T2)
			Dom.Mirror(0,TRUE);	// 좌우로 MIRROR
	}
	else {
		if(T1>T2)	
			Dom.Mirror(0,TRUE);	// 좌우로 MIRROR
	}

	/////////////////////////////
	if(bDim)
	{
		DimFactDetail_VType(&Dom,T1,T2,Scale,nUpDown,termW,termH,AngleV,bCenter, bDimDraw, bShop);
	}

	*pDom << Dom;
}
void CAPlateDrawDanmyun::DimFactDetail_VType(CDomyun *pDom,double T1,double T2, double Scale, long nUpDown
					  ,double termW, double termH,double AngleV, BOOL bCenter, BOOL bDimDraw, BOOL bShop)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDimDomyunEx DimDom(pDom);
	pOptStd->SetEnvType(&DimDom, HCAD_DIML);
	BOOL bMirror = T1 > T2 ? TRUE : FALSE;
	long nS = 2;

	// DRAWING의 기준은 이음면의 중심하단,
	double minT = min(T1,T2);
	double maxT = max(T1,T2);
	// 용접될 부재의 길이
	double shortLen = 50;	
	double longLen = 125;

	// 경사결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;

	// 플레이트
	double weldLen = termW/2+(minT-termH)*tan(ToRadian(AngleV/2));
	double slopLen = (maxT-minT)/slop;	
	
	if (slopLen+weldLen >= longLen)
		longLen = weldLen+slopLen+20;
	CDPoint xp = CDPoint(weldLen,minT);						// (4)
	CDPoint slopP = CDPoint(bMirror? -weldLen-slopLen : weldLen+slopLen, maxT);			// (0)

	// 좌우 치수
	double valLen = 0;
	// SHORT LEN
	CDPoint sp = CDPoint(bMirror ? shortLen : -shortLen,0);
	DimDom.SetDirection(bMirror? "RIGHT" : "LEFT");
	valLen = minT;

	if(!bDimDraw)
	{
		DimDom.DimMoveTo(sp,1);
		DimDom.DimLineTo(sp.x, valLen,COMMA(valLen/nS));
		if (bShop && nUpDown!=0)
		{
			if(bMirror)
			{
				sp.x = sp.x+(DimDom.GetDimExo()+DimDom.GetDimVLen())*DimDom.GetScaleDim();
				double TextLength = DimDom.Always(12);
				double Dis = -TextLength*2;
				DimDom.LineTo(sp.x, sp.y, sp.x, sp.y+Dis);
				DimDom.SetTextAngle(90);
				Dis = -TextLength;
				DimDom.TextOut(sp.x-DimDom.Always(1), sp.y+Dis, "(BOX INSIDE)");
			}
		}
	}

	// LONG LEN
	sp = CDPoint(bMirror? -longLen : longLen,0);
	DimDom.SetDirection(bMirror ? "LEFT" : "RIGHT");
	if(!bDimDraw)
	{
		DimDom.DimMoveTo(sp,1);
		DimDom.DimLineTo(termW/2,termH,COMMA(termH/nS));
	}
	DimDom.DimMoveTo(sp.x,maxT,1);
	DimDom.DimLineTo(xp.x, -(maxT-minT), bDimDraw ? "H" : COMMA((maxT-minT)/nS));
	if(!bDimDraw)
	{
		DimDom.DimMoveTo(sp, 2);
		DimDom.DimLineTo(sp.x, maxT, COMMA(maxT/nS));
		if (bShop && nUpDown!=0)
		{
			if(!bMirror)
			{
				sp.x = sp.x+(DimDom.GetDimExo()+DimDom.GetDimVLen()+DimDom.GetDimLevelDis())*DimDom.GetScaleDim();
				double TextLength = DimDom.Always(12);
				double Dis = -TextLength*2;
				DimDom.LineTo(sp.x, sp.y, sp.x, sp.y+Dis);
				DimDom.SetTextAngle(90);
				Dis = -TextLength;
				DimDom.TextOut(sp.x-DimDom.Always(1), sp.y+Dis, "(BOX INSIDE)");
			}
		}
	}

	if(nUpDown==-1) DimDom.Mirror(0,FALSE);
	*pDom << DimDom;

	////////////// MIRROR 영향 ///////////////
	DimDom.SetTextAngle(0);
	CString str = _T("");
	if(nUpDown==-1)
	{
		sp = CDPoint(-termW/2,-termH);
		DimDom.SetDirection("TOP");
		DimDom.DimMoveTo(sp.x,sp.y,1,FALSE,COMMA(termW/nS));
		DimDom.DimLineTo(termW,sp.y,"");
		// ARC DIMMENSION
		GetXyMatchSegAndSeg( CDPoint(+termW/2,-termH), CDPoint(xp.x,-xp.y),
			               CDPoint(-termW/2,-termH), CDPoint(-termW/2-(minT-termH)*tan(ToRadian(AngleV/2)),-minT),
						   sp);	// sp = 원의 중심
		double circleR = (minT+sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		str.Format("%.0f",AngleV);	
		DimDom.DimArcMoveTo(sp.x,sp.y,circleR,-circleR*sin(ToRadian(AngleV/2)),-minT,0);
		DimDom.DimArcLineTo(circleR*sin(ToRadian(AngleV/2)),-minT,0,str+"%%d");

		// TAPER
		DimDom.SetDirection("BOTTOM");
		DimDom.DimMoveTo(slopP.x,slopP.y-maxT*2, 2);
		DimDom.DimLineTo(bMirror? slopLen : -slopLen, xp.y-minT*2, COMMA(slopLen/2));
	}

	else
	{
		sp = CDPoint(-termW/2,termH);
		DimDom.SetDirection("BOTTOM");
		if(!bDimDraw)
		{
			DimDom.DimMoveTo(sp.x,sp.y,1,FALSE,COMMA(termW/nS));
			DimDom.DimLineTo(termW,sp.y,"");
		}
		// ARC DIMMENSION
		DimDom.SetDirection("TOP");
		GetXyMatchSegAndSeg( CDPoint(+termW/2,termH), xp,
			               CDPoint(-termW/2,termH), CDPoint(-termW/2-(minT-termH)*tan(ToRadian(AngleV/2)),minT),
						   sp);	// sp = 원의 중심
		double circleR = (minT-sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		str.Format("%.0f",AngleV);
		if(!bDimDraw)
		{
			DimDom.DimArcMoveTo(sp.x,sp.y,circleR,circleR*sin(ToRadian(AngleV/2)),minT,0);
			DimDom.DimArcLineTo(-circleR*sin(ToRadian(AngleV/2)),minT,0,str+"%%d");
		}

		// TAPER
		DimDom.DimMoveTo(slopP, 2);
		DimDom.DimLineTo(bMirror? slopLen : -slopLen, xp.y, bDimDraw ? "W" : COMMA(slopLen/2));
	}

	// TAPER
	str.Format("1 : %.1f",1/slop);
	double TextAngle = 90;
	if(nUpDown==-1) {
		if(T1<T2) {
			TextAngle = -ToDegree(atan(slop));
			DimDom.SetTextAngle(TextAngle);
			DimDom.TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2,str);
		}
		else {
			TextAngle = ToDegree(atan(slop));
			DimDom.SetTextAngle(TextAngle);
			DimDom.TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2,str);
		}
	}
	else {
		if(T1<T2) {
			TextAngle = ToDegree(atan(slop));
			DimDom.SetTextAngle(TextAngle);
			DimDom.TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
		else {
			TextAngle = -ToDegree(atan(slop));
			DimDom.SetTextAngle(TextAngle);
			DimDom.TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
	}
	*pDom << DimDom;
}



void CAPlateDrawDanmyun::DrawInputStagingRing(CDomyun *pDom, int m_nType, BOOL bDim)
{
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp  *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBx     = pGir->GetBxOnJijum(0);
	CSection		 *pSec	  = pBx->GetSection();
	CVStiff			 *pVStiff = pSec->GetVStiff();

	CDPoint A[41];
	pGir->GetHunchXyDom(pBx,A);  
	//실제 치수보다 3배 키움
	double uWidth = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE)*3;
	double webT    = pBx->GetFactChain(G_W)->m_dFactChainThick*3;
	double UFT	   = pBx->GetFactChain(G_F_U)->m_dFactChainThick*3;
	
	CDomyun Dom(pDom);
//	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	// 상판
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.LineTo(-uWidth/2,   0, uWidth/2,   0);
	Dom.LineTo(-uWidth/2, UFT, uWidth/2, UFT);
	Dom.LineTo( uWidth/2,   0, uWidth/2, UFT);
	Dom.LineTo(-uWidth/2,   0,-uWidth/2, UFT);
 	// 복부판
	Dom.LineTo(-webT/2, 0, -webT/2,-400*3);
	Dom.LineTo( webT/2, 0,  webT/2,-400*3);
	//수직보강재
/*  생략요청에 의해.... 070125....KB...
	Dom.MoveTo(webT/2+VStiffW, 0);
	Dom.LineTo(webT/2+VStiffW, -400*3);
*/
	*pDom << Dom;
	
	if(m_nType) 
	{
		DrawFrontCarryRingType1Bx(&Dom,pBx,TRUE,TRUE);
	//	Dom.Move(-A[2].x,-A[2].y);
		Dom.Move(-webT/2,0);
		*pDom << Dom;
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		DimFlangeType(&Dom, bDim);
		Dom.Move(-webT/2,0);
		*pDom << Dom;
	}
	else 
	{
		DrawFrontCarryRingType2Bx(&Dom,pBx,TRUE,TRUE);
	//	Dom.Move(-A[2].x,-A[2].y);
		Dom.Move(-webT/2,0);
		*pDom << Dom;
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		DimIlbanType(&Dom, bDim);
		Dom.Move(-webT/2,0);
		*pDom << Dom;
	}
}

void CAPlateDrawDanmyun::DrawFrontCarryRingType1Bx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, BOOL bDetail)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CPlateGirderApp *pGirCen = pBridge->GetGirder(-1);
	
	double R = pGirCen->m_pSangse->m_Dongbari_A*3;
	double B = pGirCen->m_pSangse->m_Dongbari_B*3;
	double C = pGirCen->m_pSangse->m_Dongbari_C*3;
	double W = pGirCen->m_pSangse->m_Dongbari_W*3;

	CDPoint A[41];
	pGir->GetHunchXyDom(pBx,A);

	CDPoint vAng	= CDPoint(0,1);
	double  sw		= -1.0;
	CDomyun Dom(pDom);
	// 접촉점의 상단을 (0,0)으로 그림
	if(bUpper)
	{
		Dom.MoveTo(0,0);
		Dom.LineToEx(-W,0);
		Dom.LineToEx(0,-B+C);
		Dom.LineToEx(C,-C);
		Dom.LineToEx((W-C),0);
		Dom.Arc(0,0,R,180,90);
		// 이동
	//	CDPoint mp = bLeft ? A[2] : A[7];
	//	Dom.Move(mp.x+WebT*sw,mp.y);
	}
	else
	{
		Dom.MoveTo(0,0);
		Dom.LineToEx(-W,0);
		Dom.LineToEx(0,B-C);
		Dom.LineToEx(C,C);
		Dom.LineToEx((W-C-B*sw),0);
		Dom.Arc(0,0,R,90,90);
		// 이동
	//	CDPoint mp = bLeft ? A[2] : A[7];
	//	Dom.Move(mp.x+WebT*sw, mp.y-gHeight);
	}

	*pDom << Dom;

}
void CAPlateDrawDanmyun::DimFlangeType(CDomyun *pDom, BOOL bDim)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CPlateSangSeDo       *pSang   = pBridge->GetGirder(-1)->m_pSangse;

	double A = pSang->m_Dongbari_A*3;
	double B = pSang->m_Dongbari_B*3;
	double C = pSang->m_Dongbari_C*3;
	double W = pSang->m_Dongbari_W*3;
	double T  = pSang->m_Dongbari_T;

	CDomyun Dom(pDom);
	if(bDim)
	{
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(-W,0,1);
		Dom.DimLineToExtend(W,1,"W",COMMA(W/3));
		
		Dom.DimMoveTo(-A,0,0);
		Dom.DimLineToExtend(A,0,"A",COMMA(A/3));

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(-W,-B,0);
		Dom.DimLineToExtend(B,0,"B",COMMA(B/3));

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(-W,-B,0);
		Dom.DimLineToExtend(C,0,"C",COMMA(C/3));

		Dom.TextOut(-W + (W-A)/2, -B/2, "T");	
	}
	else
	{
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(-W,0,1);
		Dom.DimLineTo(W,1,COMMA(W/3));
		
		Dom.DimMoveTo(-A,0,0);
		Dom.DimLineTo(A,0,COMMA(A/3));

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(-W,-B,0);
		Dom.DimLineTo(B,0,COMMA(B/3));

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(-W,-B,0);
		Dom.DimLineTo(C,0,COMMA(C/3));

		Dom.GiSiArrowExt(-W/2, -B, Dom.Always(8), Dom.Always(30), 7,
			"1-PL "+COMMA(W/3)+"X"+COMMA(T)+"X"+COMMA(B/3));
	}

	*pDom << Dom;
}
void CAPlateDrawDanmyun::DrawFrontCarryRingType2Bx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, BOOL bDetail)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CPlateGirderApp *pGirCen    = pBridge->GetGirder(-1);
	
	BOOL bShop = FALSE;
	CDPoint A[41];
	//bShop ? pGir->GetXyHunchShop(pBx,A) : pGir->GetHunchXyDom(pBx,A);
	pGir->GetHunchXyDom(pBx,A);

	CDPoint vAng	= bShop ? pBx->GetAngle() : CDPoint(0,1);
	
	CPlateSangSeDo *pSang = pGirCen->m_pSangse;
	double dA  = pSang->m_Dongbari_A*3;
	double  B  = pSang->m_Dongbari_B*3;
	double  C  = pSang->m_Dongbari_C*3;
	double  D  = pSang->m_Dongbari_D*3;
	double  E  = pSang->m_Dongbari_E*3;
	double  F  = pSang->m_Dongbari_F*3;
	double  W  = pSang->m_Dongbari_W*3;
	double sw = -1;
	CDomyun Dom(pDom);
	// 접촉점의 상단을 (0,0)으로 그림
	Dom.MoveTo(0,0);
	Dom.LineToEx((W-C)*sw,0);
	Dom.LineToEx(C*sw,-C);
	Dom.LineToEx(0,(-E+C)*2);
	Dom.LineToEx(-C*sw,-C);
	Dom.LineToEx(-(W-C)*sw,0);
	//
	Dom.Arc(dA*sw,-E+B/2,D/2,  0,180);
	Dom.Arc(dA*sw,-E-B/2,D/2,180,180);
	Dom.LineTo( (dA+D/2)*sw, -E+B/2, (dA+D/2)*sw, -E-B/2);
	Dom.LineTo( (dA-D/2)*sw, -E+B/2, (dA-D/2)*sw, -E-B/2);
	// 이동
/*	CDPoint mp = bLeft ? A[2] : A[7];
	if(!bUpper)
		mp = A[2] - (gHeight-F*2-E*2);
	Dom.Move(mp.x+WebT*sw,mp.y);*/
	Dom.Move(0,-F);

	*pDom << Dom;


}
void CAPlateDrawDanmyun::DimIlbanType(CDomyun *pDom, BOOL bDim)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CPlateGirderApp *pGirCen    = pBridge->GetGirder(-1);
	CPlateSangSeDo	    *pSang   = pGirCen->m_pSangse;

	CDomyun Dom(pDom);
	double  A  = pSang->m_Dongbari_A*3;
	double  B  = pSang->m_Dongbari_B*3;
	double  C  = pSang->m_Dongbari_C*3;
	double  D  = pSang->m_Dongbari_D*3;
	double  E  = pSang->m_Dongbari_E*3;
	double  F  = pSang->m_Dongbari_F*3;
	double  W  = pSang->m_Dongbari_W*3;
	double  T  = pSang->m_Dongbari_T;

	if(bDim)
	{
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(-W,-F-E,0);
		Dom.DimLineToExtend(E,0,"E",COMMA(E/3));
		Dom.DimLineToExtend(F,0,"F",COMMA(F/3));

		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(-A+D/2,-F-(E+B/2),0);
		Dom.DimLineToExtend(B,0,"B",COMMA(B/3));

		Dom.SetDirection("TOP");
		Dom.DimMoveTo(-A-D/2,-F-(E-B/2),0);
		Dom.DimLineToExtend(D,0,"D",COMMA(D/3));

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(-W,-F-(2*E),0);
		Dom.DimLineToExtend(C,0,"C",COMMA(C/3));
			
		Dom.DimMoveTo(-A,-F-(2*E),0);
		Dom.DimLineToExtend(A,0,"A",COMMA(A/3));

		Dom.DimMoveTo(-W,-F-(2*E),1);
		Dom.DimLineToExtend(W,1,"W",COMMA(W/3));

		Dom.TextOut(-W + C/2,-E-F,"T");	
	}
	else
	{
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(-W,-F-E,0);
		Dom.DimLineTo(E,0,COMMA(E/3));
		Dom.DimLineTo(F,0,COMMA(F/3));

		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(-A+D/2,-F-(E+B/2),0);
		Dom.DimLineTo(B,0,COMMA(B/3));

		Dom.SetDirection("TOP");
		Dom.DimMoveTo(-A-D/2,-F-(E-B/2),0);
		Dom.DimLineTo(D,0,COMMA(D/3));

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(-W,-F-(2*E),0);
		Dom.DimLineTo(C,0,COMMA(C/3));
			
		Dom.DimMoveTo(-A,-F-(2*E),0);
		Dom.DimLineTo(A,0,COMMA(A/3));

		Dom.DimMoveTo(-W,-F-(2*E),1);
		Dom.DimLineTo(W,1,COMMA(W/3));

		Dom.GiSiArrowExt(-W, -F-E*2+C, Dom.Always(8), -Dom.Always(30), 5,
			"1-PL "+COMMA(W/3)+"X"+COMMA(T)+"X"+COMMA(E*2/3));
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawInputBracket(CDomyun *pDom, double dRound, double dChip, double R, double C)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double D1 =   0;
	double D2 = 400;
	double W1 = 250;
	double W2 = 150;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawGassetPlatePlan(&Dom,0,0,D1,D2,W1,W2,R,C,FALSE,TRUE);
	DrawGassetPlatePlan(&Dom,0,0,D1,D2,W1,W2,R,C,FALSE,FALSE);
	Dom.LineTo(-300,0,300,0);
	pOptStd->SetEnvType(&Dom,HCAD_STLH);
	Dom.LineTo(-5,0,-5,-400);
	Dom.LineTo(+5,0,+5,-400);
	Dom.Rotate(ConstPi/2,0,0);

	// HCAD_DIML
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetScaleDim(15);
	Dom.SetDirection("BOTTOM");
	if(C>0)
	{
		Dom.DimMoveTo(0,-W1,0);
		Dom.DimLineToOverExtend(C,0,"C",COMMA(C));
	}
	if(R>0)
	{
		CDPoint xy4 = CPlateCrossBeam::GetGassetPlateVertex(4,0,0,D1,D2,W1,W2,R,C,FALSE,TRUE,CDPoint(0,1),CDPoint(0,1));
		CDPoint xy5 = CPlateCrossBeam::GetGassetPlateVertex(5,0,0,D1,D2,W1,W2,R,C,FALSE,TRUE,CDPoint(0,1),CDPoint(0,1));
		CDPoint xy6 = CPlateCrossBeam::GetGassetPlateVertex(6,0,0,D1,D2,W1,W2,R,C,FALSE,TRUE,CDPoint(0,1),CDPoint(0,1));
		double startAng = (xy4-xy6).GetAngleDegree();
 		double beta		= ToDegree(GetAngleRadianBetweenFrPlusToMinus(xy4,xy6,xy5));

		CDPoint xy;
		xy.x = xy6.x + R*cos(ToRadian(startAng+beta/2));
		xy.y = xy6.y + R*sin(ToRadian(startAng+beta/2));

		CString str;
		Dom.GiSiArrowExtend(xy.x,xy.y,Dom.Always(10),Dom.Always(10),1,"R",COMMA(R));
	}

	*pDom << Dom;
}
void CAPlateDrawDanmyun::DrawGassetPlatePlan(CDomyun *pDom, double BX, double BY, double W1, double W2, double dis1, double dis2, 
							double R, double C, BOOL bLeft, BOOL bFront, CDPoint vAng/*=CDPoint(0,1)*/, CDPoint vRotate/*=CDPoint(1,0)*/, 
							BOOL bRoundOnly/*=FALSE*/, double dWeld/*=0*/)
{
	CDomyun Dom(pDom);

	CDPoint sp[8];
	for (long nPoint = 0; nPoint < 7; nPoint++)
		sp[nPoint] = CPlateCrossBeam::GetGassetPlateVertex(nPoint, BX, BY, W1, W2, dis1, dis2, R, C, bLeft, bFront, vAng, vRotate);

	if (R > 0)   {
		double startAng = (sp[4]-sp[6]).GetAngleDegree();
 		double beta	 = ToDegree(GetAngleRadianBetweenFrPlusToMinus(sp[4], sp[6], sp[5]));

		Dom.LineTo(sp[1], sp[5]);
		Dom.LineTo(sp[2], sp[4]);
		Dom.Arc(sp[6].x, sp[6].y, R, startAng, beta);
	}
	else   {
		if (dWeld > 0)   {
			CDPoint p1, p2, p3, p4, vX(1, 0), vY(0, 1);
			p1 = sp[2];
			p2 = p1 + vX * dWeld;
			p3 = p2 + vY * dWeld;
			p4 = p3 - vX * dWeld;
			Dom.Solid(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
		}
		Dom.LineTo(sp[1], sp[4]);
		Dom.LineTo(sp[4], sp[2]);
	}

	if(!bRoundOnly)
		Dom.LineTo(sp[0], sp[1]);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawInputBracketStiff(CDomyun *pDom, CPlateBasicIndex *pBx)
{
//	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	
	DrawBoxPart(&Dom, pBx);
	DrawGassetPlate(&Dom, pBx);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawInputBStiffWithBolt(CDomyun *pDom, CPlateBasicIndex *pBx)
{
//	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	
	DrawBoxPartWithBolt(&Dom, pBx);
	DrawGassetPlateWithBolt(&Dom, pBx);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawBoxPart(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir = pBridge->GetGirder(0);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	double Hgt = 530;
	double Wid = 400;
	double Srp = 232;
	double T   = 14;
//	long nType = pBridge->m_nGussetStiff_Type;

/*
	CPlateBasicIndex *pBx = NULL;
	if(nType==0)//A
		pBx = pGir->GetBxBySectionType(SECTION_A, 0);
	else if(nType==1)//P
		pBx = pGir->GetBxBySectionType(SECTION_P, 0);
	else if(nType==2)//C
		pBx = pGir->GetBxBySectionType(SECTION_C, 0);
*/

	double S1 = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S1);
	double GassetT = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_T);
	double dB = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_DB);
	double S2 = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S2);
	double S3 = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S3);
	double uB = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_UB);
	double W = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_W);
	double sT = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_ST);
	double dD = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_DD);
	double dE = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_DE);
	double uD = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_UD);
	double uE = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_UE);
	double CC = 10;
	
	// 하판
	Dom.Rectangle(-Srp+T, 0, Srp,-T-6);
	// 복부판
	Dom.LineTo( 0,0,  0,Hgt+100);
	Dom.LineTo( T,0,  T,Hgt+100);
	// 하단판
	int nGen = (int)pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_GEN);
	if(nGen==0)
	{
		Dom.LineTo(  T, Hgt,   Wid, Hgt);
		Dom.LineTo(  T, Hgt+T, Wid, Hgt+T);
		Dom.LineTo(Wid, Hgt+T, Wid, Hgt);
	}
	else
	{
		S2 = 0;
		S3 = 0;
		uB = 0;
		Dom.LineTo(  T, Hgt,   Wid+uE+50, Hgt);
		Dom.LineTo(  T, Hgt+T, Wid+uE+50, Hgt+T);
	}
	// 가세트	
	CDPoint	Sp1(Srp - S1,0);
	CDPoint Sp2(Wid-S2,Hgt-S3);
	Dom.LineTo(Wid-S2,Hgt,Sp2.x,Sp2.y);
	Dom.MoveTo(Sp2);
	if(S1 == 0)
	{
		Dom.LineTo(Sp1.x+CC, Sp1.y+CC);
		Dom.LineTo(Sp1.x-CC, Sp1.y);
		Sp1.x += CC;
		Sp1.y += CC;
	}
	else
		Dom.LineTo(Sp1);
	//보강판 (보강판의 두께와 폭이 0 일 때 그리지 않는다.)
	CDPoint Ang = (Sp2 - Sp1).Unit();
	CDPoint Sp3 = Sp1 + dB*Ang;
	CDPoint Sp4 = Sp3 + GassetT*Ang.RotateInv90().Unit();
	CDPoint Sp5 = Sp2 - uB*Ang;
	CDPoint Sp6 = Sp5 + GassetT*Ang.RotateInv90().Unit();
	CDPoint xy0, xy1;
	GetXyMatchLineAndLine(Sp6, Ang, CDPoint(0,Hgt), CDPoint(1,0), xy0);
	GetXyMatchLineAndLine(Sp6, Ang, CDPoint(0,Hgt-dE), CDPoint(1,0), xy1);

	if(nGen==1)
		Sp6 = xy0;
	if(GassetT != 0 && W !=0) 
	{
		Dom.LineTo(Sp3,Sp4);
		Dom.LineTo(Sp5,Sp6);
		Dom.LineTo(Sp4,Sp6);
	}
	//부보강판
	if(nGen==1)
	{
		Dom.MoveTo(xy1);
		Dom.LineTo(xy1.x+dD, xy1.y);
		Dom.LineTo(xy0.x+uE, xy0.y-uD);
		Dom.LineTo(xy0.x+uE, xy0.y);
		//스켈럽
		Dom.Arc(Wid, Hgt, 35, 180, Ang.GetAngleDegree());
		Dom.Arc(xy0, 35, 0, Ang.GetAngleDegree()-180);
	}
	*pDom << Dom;

///////////////////////////////////////////////////////////////
//치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(10);

	Dom.SetDirection("TOP");
	if(nGen==1)
	{
		Dom.DimMoveTo(xy0.x, Hgt, 0);
		Dom.DimLineToExtend(uE, 0, "uE", COMMA(uE));
	}
	*pDom << Dom;

	Dom.SetDimExo(1);
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(Srp - S1,-T,0);
	Dom.DimLineToOverExtend(S1,0,"S1",COMMA(S1));
	if(nGen==0)
	{
		Dom.DimMoveTo(Wid-S2,Hgt - S3,0);
		Dom.DimLineToOverExtend(S2,0,"S2",COMMA(S2));
	}
	else
	{
		Dom.DimMoveTo(xy1.x, xy1.y, 0);
		Dom.DimLineToOverExtend(dD, 0, "dD", COMMA(dD));
		Dom.GiSiArrowExtend(xy0.x+uE/3, xy1.y+dE*2/3, Dom.Always(7), Dom.Always(5), 7, "sT", COMMA(sT));
	}
	*pDom << Dom;
	
	Dom.SetDirection("RIGHT");
	if(nGen==0)
	{
		Dom.DimMoveTo(Wid,Hgt-S3,0);
		Dom.DimLineToOverExtend(S3,0,"S3",COMMA(S3));
	}
	else
	{
		Dom.DimMoveTo(xy0.x+uE, Hgt-uD, 0);
		Dom.DimLineToOverExtend(uD, 0, "uD", COMMA(uD));
	}
	*pDom << Dom;

	Dom.SetDirection("LEFT");
	if(nGen==1)
	{
		Dom.DimMoveTo(xy1.x, Hgt-dE, 0);
		Dom.DimLineToExtend(dE, 0, "dE", COMMA(dE));
	}
	*pDom << Dom;

	if(GassetT != 0 && W !=0) 
	{
		Dom.SetDirection("BOTTOM");
		Dom.SetStartPointExtend(TRUE);
		Dom.DimMoveTo(Sp3.x,Sp3.y,0);
		Dom.DimLineToOverExtend(GassetT,0,"T",COMMA(GassetT),1);
	}
	double Ang2 = atan(Ang.y/Ang.x);
	Dom.Rotate(-ToRadian(90)+Ang2,Sp3.x,Sp3.y);

	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(-Srp+T,Hgt+100,Srp,Hgt+100,FALSE,1);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawBoxPartWithBolt(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir = pBridge->GetGirder(0);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	double Hgt = 530;
	double Wid = 400;
	double Srp = 232;
	double T   = 14;
	double VStiffWid = 150;
	double distH = 50;

	double S1 = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S1);
	double S2 = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S2);
	double S3 = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S3);
	double dB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
	double dH = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	double dR = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_R);
	double GassetT = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_T);
	double ddB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_DB);
	double uB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_UB);
	double W = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_W);
	double sT = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_ST);
	double dD = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_DD);
	double dE = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_DE);
	double uD = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_UD);
	double uE = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_UE);
	
	// 하판
	Dom.Rectangle(-Srp+T, -50, Srp,-T-56);
	// 복부판
	Dom.LineTo( 0, -50, 0, Hgt+50);
	Dom.LineTo( T, -50, T, Hgt+50);
	// 수직보강재
	Dom.LineTo(VStiffWid+T, -50, VStiffWid+T, distH-50);
	pOptStd->SetEnvType(&Dom, HCAD_STLH);
	Dom.LineTo(VStiffWid+T, distH-50, VStiffWid+T, Hgt-dH+distH-50);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.LineTo(VStiffWid+T, Hgt-dH+distH-50, T+VStiffWid,Hgt+50);
	// 하단판
	int nGen = (int)pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_GEN);
	if(nGen==0)
	{
		Dom.LineTo(  T, Hgt,   Wid, Hgt);
		Dom.LineTo(  T, Hgt+T, Wid, Hgt+T);
		Dom.LineTo(Wid, Hgt+T, Wid, Hgt);
	}
	else
	{
		S2 = 0;
		S3 = 0;
		uB = 0;
		Dom.LineTo(  T, Hgt,   Wid+uE+50, Hgt);
		Dom.LineTo(  T, Hgt+T, Wid+uE+50, Hgt+T);
	}
	// 가세트	
	CDPoint	Sp1(VStiffWid+T+S1, 0);	
	CDPoint Sp2(Wid-S2, Hgt-S3);
	Dom.LineTo(Wid-S2, Hgt, Sp2.x, Sp2.y);
	Dom.MoveTo(Sp2);
	Dom.LineTo(Sp1);
	Dom.LineTo(VStiffWid/3.0+T, 0);
	Dom.LineTo(VStiffWid/3.0+T, Hgt-dH);
	Dom.LineTo(Wid/2.0+dB-dR, Hgt-dH);
	Dom.Arc(Wid/2.0+dB-dR, Hgt-dH+dR, dR, -90, 90);
	Dom.LineTo(Wid/2.0+dB, Hgt-dH+dR, Wid/2.0+dB, Hgt);
	//보강판 (보강판의 두께와 폭이 0 일 때 그리지 않는다.)
	CDPoint Ang = (Sp2 - Sp1).Unit();
	CDPoint Sp3 = Sp1 + ddB*Ang;
	CDPoint Sp4 = Sp3 + GassetT*Ang.RotateInv90().Unit();
	CDPoint Sp5 = Sp2 - uB*Ang;
	CDPoint Sp6 = Sp5 + GassetT*Ang.RotateInv90().Unit();
	CDPoint xy0, xy1;
	GetXyMatchLineAndLine(Sp6, Ang, CDPoint(0,Hgt), CDPoint(1,0), xy0);
	GetXyMatchLineAndLine(Sp6, Ang, CDPoint(0,Hgt-dE), CDPoint(1,0), xy1);

	if(nGen==1)
		Sp6 = xy0;
	if(GassetT != 0 && W !=0) 
	{
		Dom.LineTo(Sp3,Sp4);
		Dom.LineTo(Sp5,Sp6);
		Dom.LineTo(Sp4,Sp6);
	}
	//부보강판
	if(nGen==1)
	{
		Dom.MoveTo(xy1);
		Dom.LineTo(xy1.x+dD, xy1.y);
		Dom.LineTo(xy0.x+uE, xy0.y-uD);
		Dom.LineTo(xy0.x+uE, xy0.y);
		//스켈럽
		Dom.Arc(Wid, Hgt, 35, 180, Ang.GetAngleDegree());
		Dom.Arc(xy0, 35, 0, Ang.GetAngleDegree()-180);
	}
	*pDom << Dom;

///////////////////////////////////////////////////////////////
//치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(10);

	Dom.SetDirection("TOP");
	if(nGen==1)
	{
		Dom.DimMoveTo(xy0.x, Hgt, 0);
		Dom.DimLineToExtend(uE, 0, "uE", COMMA(uE));
	}
	*pDom << Dom;

	Dom.SetDimExo(1);
	Dom.SetDirection("BOTTOM");

	Dom.DimMoveTo(VStiffWid+T,-T,1);
	Dom.DimLineToOverExtend(S1,1,"S1",COMMA(S1));

	if(nGen==0)
	{
		Dom.DimMoveTo(Wid-S2,Hgt - S3,0);
		Dom.DimLineToOverExtend(S2,0,"S2",COMMA(S2));
	}
	else
	{
		Dom.DimMoveTo(xy1.x, xy1.y, 0);
		Dom.DimLineToOverExtend(dD, 0, "dD", COMMA(dD));
		Dom.GiSiArrowExtend(xy0.x+uE/3, xy1.y+dE*2/3, Dom.Always(7), Dom.Always(5), 7, "sT", COMMA(sT));
	}

	Dom.DimMoveTo(Wid/2.0, Hgt-dH, 0);
	Dom.DimLineToOverExtend(dB, 0, "B", COMMA(dB));

	Dom.GiSiArrowExtend(Wid/2.0+dB-dR*(1-sin(ToRadian(45))),Hgt-dH+dR*(1-cos(ToRadian(45))),
		Dom.Always(5),Dom.Always(5),7,"R", COMMA(dR));
	*pDom << Dom;
	
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(Wid/2.0+dB, Hgt-dH, 0);
	Dom.DimLineToOverExtend(dH, 0, "H", COMMA(dH));

	if(nGen==0)
	{
		Dom.DimMoveTo(Wid,Hgt-S3,0);
		Dom.DimLineToOverExtend(S3,0,"S3",COMMA(S3));
	}
	else
	{
		Dom.SetDimVLen(7);
		Dom.DimMoveTo(xy0.x+uE, Hgt-uD, 0);
		Dom.DimLineToOverExtend(uD, 0, "uD", COMMA(uD));
		Dom.SetDimLevelDis(7);
		Dom.DimMoveTo(xy0.x+uE, Hgt-dE, 1);
		Dom.DimLineToExtend(dE, 1, "dE", COMMA(dE));
	}
	*pDom << Dom;

	if(GassetT != 0 && W !=0) 
	{
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(Sp1.x,Sp1.y,0);
		Dom.DimLineToOverExtend(GassetT,0,"T",COMMA(GassetT),1);
	}
	double Ang2 = atan(Ang.y/Ang.x);
	Dom.Rotate(-ToRadian(90)+Ang2,Sp1.x,Sp1.y);

	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(-Srp+T,Hgt+50,Srp,Hgt+50,FALSE,1);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawGassetPlate(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double T   = 12;
	double dS3 = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S3);
	double GassetT = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_T);
	double W = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_W);
	double dA = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_DA);
	double dB = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_DB);
	double dC = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_DC);
	double uA = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_UA);
	double uB = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_UB);
	double uC = pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_UC);

	double BX = 900;
	
	Dom.LineTo(BX,0,BX,dB);
	Dom.LineTo(BX,0,BX+T,0);
	Dom.LineTo(BX+T,0,BX+T,dB);

	int nGen = (int)pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_GEN);
	double Hgt;
	if(nGen==0)
		Hgt = 530 - dS3;
	else
		Hgt = 530;
	if(GassetT != 0 && W !=0 ) //(보강판의 두께와 폭이 0 일 때  보강재를 그리지 않는다.) 
	{
		Dom.LineTo((BX - (dC-T/2)),dB,(BX + (dC+T/2)),dB);

		if(nGen==0)
		{
			Dom.LineTo((BX - (uC-T/2)),Hgt-uB,(BX + (uC+T/2)),Hgt-uB);
			Dom.LineTo((BX - (dC-T/2)),dB, (BX - (W/2 -T/2)),(dB+dA));
			Dom.LineTo((BX - (W/2 -T/2)),(dB+dA),(BX - (W/2 -T/2)),Hgt-(uA+uB));
			Dom.LineTo((BX - (W/2 -T/2)),Hgt-(uA+uB),(BX - (uC-T/2)),Hgt-uB);
			
			Dom.LineTo((BX + (uC+T/2)),Hgt-uB,(BX + (W/2 +T/2)),Hgt-(uA+uB));
			Dom.LineTo((BX + (W/2 +T/2)),Hgt-(uA+uB),(BX + (W/2 +T/2)),(dA+dB));
			Dom.LineTo((BX + (W/2 +T/2)),(dA+dB),(BX + (dC+T/2)),dB);
		}
		else
		{
			Dom.LineTo((BX - (W-T)/2),Hgt,(BX + (W+T)/2),Hgt);
			Dom.LineTo((BX - (W-T)/2),Hgt,(BX - (dC-T/2)),dB);
			Dom.LineTo((BX + (W+T)/2),Hgt,(BX + (dC+T/2)),dB);
		}
	}	
	
	Dom.LineTo(BX,Hgt,BX,Hgt-uB);
	Dom.LineTo(BX,Hgt,BX+T,Hgt);
	Dom.LineTo(BX+T,Hgt,BX+T,Hgt-uB);

	if(GassetT != 0 && W !=0 ) //(보강판의 두께와 폭이 0 이 아닐 때 실선.) 
	{
		pOptStd->SetEnvType(&Dom, HCAD_STLH);
	}
	Dom.LineTo(BX,dB,BX,Hgt-uB);
	Dom.LineTo(BX+T,dB,BX+T,Hgt-uB);

	*pDom << Dom;
////////////////////////////////////////////////////////////////////////
//치수선(보강판의 두께와 폭이 0 일 때  치수선을 그리지 않는다.)
	if(GassetT != 0 && W !=0 ) 
	{
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetScaleDim(10);
		Dom.SetDimExo(1);

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(BX-dC+T/2,dB,0);
		Dom.DimLineToOverExtend(dC,0,"dC",COMMA(dC));

		if(nGen==0)
		{
			Dom.DimMoveTo(BX+(-W/2 + T/2),dB,1);
			Dom.DimLineToExtend(W,1,"W",COMMA(W));
		}

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(BX,0,0);
		Dom.DimLineToExtend(dB,0,"dB",COMMA(dB));
		if(nGen==0)
		{
			Dom.DimLineToExtend(dA,0,"dA",COMMA(dA));
		}

		if(nGen==0)
		{
			Dom.DimMoveTo(BX-uC,Hgt-(uA+uB),0);
			Dom.DimLineToExtend(uA,0,"uA",COMMA(uA));
			Dom.DimLineToExtend(uB,0,"uB",COMMA(uB));
		}

		Dom.SetDirection("TOP");
		if(nGen==0)
		{
			Dom.DimMoveTo(BX+T/2,Hgt,0);
			Dom.DimLineToOverExtend(uC,0,"uC",COMMA(uC));
		}
		else
		{
			Dom.DimMoveTo(BX+(T-W)/2,Hgt,0);
			Dom.DimLineToOverExtend(W,0,"Lower FL. Width","");
		}

		*pDom << Dom;
	}
}

void CAPlateDrawDanmyun::DrawGassetPlateWithBolt(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(0);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double T   = 12;
	double dS3 = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S3);
	double GassetT = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_T);
	double W = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_W);
	double dA = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_DA);
	double dB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_DB);
	double dC = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_DC);
	double uA = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_UA);
	double uB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_UB);
	double uC = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_UC);

	double BX = 900;
	
	Dom.LineTo(BX,0,BX,dB);
	Dom.LineTo(BX,0,BX+T,0);
	Dom.LineTo(BX+T,0,BX+T,dB);

	int nGen = (int)pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_GEN);
	double Hgt;
	if(nGen==0)
		Hgt = 530 - dS3;
	else
		Hgt = 530;
	if(GassetT != 0 && W !=0 ) //(보강판의 두께와 폭이 0 일 때  보강재를 그리지 않는다.) 
	{
		Dom.LineTo((BX - (dC-T/2)),dB,(BX + (dC+T/2)),dB);

		if(nGen==0)
		{
			Dom.LineTo((BX - (uC-T/2)),Hgt-uB,(BX + (uC+T/2)),Hgt-uB);
			Dom.LineTo((BX - (dC-T/2)),dB, (BX - (W/2 -T/2)),(dB+dA));
			Dom.LineTo((BX - (W/2 -T/2)),(dB+dA),(BX - (W/2 -T/2)),Hgt-(uA+uB));
			Dom.LineTo((BX - (W/2 -T/2)),Hgt-(uA+uB),(BX - (uC-T/2)),Hgt-uB);
			
			Dom.LineTo((BX + (uC+T/2)),Hgt-uB,(BX + (W/2 +T/2)),Hgt-(uA+uB));
			Dom.LineTo((BX + (W/2 +T/2)),Hgt-(uA+uB),(BX + (W/2 +T/2)),(dA+dB));
			Dom.LineTo((BX + (W/2 +T/2)),(dA+dB),(BX + (dC+T/2)),dB);
		}
		else
		{
			Dom.LineTo((BX - (W-T)/2),Hgt,(BX + (W+T)/2),Hgt);
			Dom.LineTo((BX - (W-T)/2),Hgt,(BX - (dC-T/2)),dB);
			Dom.LineTo((BX + (W+T)/2),Hgt,(BX + (dC+T/2)),dB);
		}
	}	
	
	Dom.LineTo(BX,Hgt,BX,Hgt-uB);
	Dom.LineTo(BX,Hgt,BX+T,Hgt);
	Dom.LineTo(BX+T,Hgt,BX+T,Hgt-uB);

	if(GassetT != 0 && W !=0 ) //(보강판의 두께와 폭이 0 이 아닐 때 실선.) 
	{
		pOptStd->SetEnvType(&Dom, HCAD_STLH);
	}
	Dom.LineTo(BX,dB,BX,Hgt-uB);
	Dom.LineTo(BX+T,dB,BX+T,Hgt-uB);

	*pDom << Dom;
////////////////////////////////////////////////////////////////////////
//치수선(보강판의 두께와 폭이 0 일 때  치수선을 그리지 않는다.)
	if(GassetT != 0 && W !=0 ) 
	{
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetScaleDim(10);
		Dom.SetDimExo(1);

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(BX-dC+T/2,dB,0);
		Dom.DimLineToOverExtend(dC,0,"dC",COMMA(dC));

		if(nGen==0)
		{
			Dom.DimMoveTo(BX+(-W/2 + T/2),dB,1);
			Dom.DimLineToExtend(W,1,"W",COMMA(W));
		}

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(BX,0,0);
		Dom.DimLineToExtend(dB,0,"dB",COMMA(dB));
		if(nGen==0)
		{
			Dom.DimLineToExtend(dA,0,"dA",COMMA(dA));
		}

		if(nGen==0)
		{
			Dom.DimMoveTo(BX-uC,Hgt-(uA+uB),0);
			Dom.DimLineToExtend(uA,0,"uA",COMMA(uA));
			Dom.DimLineToExtend(uB,0,"uB",COMMA(uB));
		}

		Dom.SetDirection("TOP");
		if(nGen==0)
		{
			Dom.DimMoveTo(BX+T/2,Hgt,0);
			Dom.DimLineToOverExtend(uC,0,"uC",COMMA(uC));
		}
		else
		{
			Dom.DimMoveTo(BX+(T-W)/2,Hgt,0);
			Dom.DimLineToOverExtend(W,0,"Lower FL. Width","");
		}

		*pDom << Dom;
	}
}

void CAPlateDrawDanmyun::DrawInputLiftLug(CDomyun *pDom, long nRow, long nCol, bool bAddPlate, bool bSidePlate)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CDomyun Dom(pDom);

	DrawLiftLugFrontView(&Dom,nRow,nCol, bAddPlate, bSidePlate);
	Dom.Move(-200,0);
	*pDom << Dom;

	DrawLiftLugSidePlateView(&Dom,nRow, bAddPlate, bSidePlate);
	Dom.Move(200,0);
	*pDom << Dom;
}
 
void CAPlateDrawDanmyun::DrawLiftLugFrontView(CDomyun *pDom, long nRow, long nCol, bool bAddPlate, bool bSidePlate)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	
	CPlatePieceObject pObj(pGir,GI_LP,NULL,nRow);
	pGir->m_pSangse->CreatePieceObjLiftLug(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, FALSE);

	*pDom << Dom;

	// ADD PLATE
	if(bAddPlate)
	{
		nCol = 1;
		CPlatePieceObject pObjAdd(pGir,GI_LP,NULL,nRow,nCol);
		//CREATE
		pGir->m_pSangse->CreatePieceObjLiftLug(&pObjAdd);
		pObjAdd.DrawPieceOutline(&Dom, pOptStd, FALSE);
		//DRAW DIM
		DimAddPlate(&Dom, &pObjAdd);
		//MOVE
		CDPoint cen(pObjAdd.GetChild(0)->GetPoint(1));
		Dom.Move(0,cen.y);
		*pDom << Dom;
	}

	// SIDE PLATE
	if(bSidePlate)
	{
		double A = pGir->m_pSangse->m_LiftLug_A[nRow];
		double B = pGir->m_pSangse->m_LiftLug_B[nRow];
		double sA = pGir->m_pSangse->m_LiftLug_sA[nRow];
		double sB = pGir->m_pSangse->m_LiftLug_sB[nRow];
		double C = pGir->m_pSangse->m_LiftLug_C[nRow];
		
		//LEFT
		Dom.MoveTo(-A,0);
		Dom.LineTo(-A-sA,0);
		Dom.LineTo(-A-sA,C);
		Dom.LineTo(-A,C);

		//RIGHT
		Dom.MoveTo(B,0);
		Dom.LineTo(B+sB,0);
		Dom.LineTo(B+sB,C);
		Dom.LineTo(B,C);

		//DIM SET
		CDimDomyunEx Dom(pDom);
		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		int nDan = 1;
		Dom.SetDirection("BOTTOM");
		CDPoint xy;

		//DIM LEFT
		xy.x = -A;	xy.y = 0;
		Dom.DimMoveTo(xy,nDan);
		xy.x = -sA;
		Dom.DimLineTo(xy.x, xy.y, "sA");

		//DIM RIGHT
		xy.x = B;	xy.y = 0;
		Dom.DimMoveTo(xy,nDan);
		xy.x = sB;
		Dom.DimLineTo(xy.x, xy.y, "sB");

		*pDom << Dom;
	}

	DimFrontView(&Dom, &pObj);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawLiftLugSidePlateView(CDomyun *pDom, long nRow, bool bAddPlate, bool bSidePlate)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	if(bSidePlate)
	{
		CPlatePieceObject pObjSide(pGir,GI_LP,NULL,nRow,2);
		pGir->m_pSangse->CreatePieceObjLiftLug(&pObjSide);
		pObjSide.DrawPieceOutline(&Dom,pOptStd, FALSE);
		DimSideViewSidePlate(&Dom, &pObjSide);
	}
	DrawSideMainPlateView(&Dom, nRow, bAddPlate, bSidePlate);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimFrontView(CDomyun *pDom, CPlatePieceObject *pObj)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDimDomyunEx Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	CDPoint xyBase(pObj->GetPoint(5));
	CDPoint xy;
	int nDan = 1;

	//DOWN
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyBase,nDan);
	xy = pObj->GetPoint(6);		Dom.DimLineTo(xy.x, xy.y, "A");
	Dom.DimMoveTo(xyBase,nDan);
	xy = pObj->GetPoint(4);		Dom.DimLineTo(xy.x, xy.y, "B");

	//RIGHT
	Dom.SetDirection("RIGHT");
	xyBase = pObj->GetPoint(4);	Dom.DimMoveTo(xyBase,nDan);
	xy = pObj->GetPoint(3);		Dom.DimLineTo(xy.x, xy.y, "C");

	nDan = 2;					Dom.DimMoveTo(xyBase,nDan);
	if(xy != pObj->GetChild(0)->GetPoint(0))
		xy = pObj->GetChild(0)->GetPoint(0);	Dom.DimLineTo(xyBase.x, xy.y, "D");

	//RADIUS
	CDPoint vDir(-1,1);		vDir = vDir.Unit();
	CDPoint R(pObj->GetChild(0)->GetPoint(1));
	CDPoint cen(pObj->GetChild(0)->GetPoint(0));
	xy = cen + vDir*R.y;

	Dom.SetDirection("TOP");
	CString str = "R";
	double len = Dom.GetTextWidth(str, 3);
	Dom.GiSiArrowExt(xy.x, xy.y, len, -len/2, 3, str);

	//DIAMETER
	nDan = 1;
	CDPoint d(pObj->GetChild(0)->GetPoint(2));
	Dom.DimMoveTo(cen.x-d.x/2, cen.y, nDan);
	Dom.DimLineTo(d.x, cen.y, "d");

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimSideViewSidePlate(CDomyun *pDom, CPlatePieceObject *pObjSide)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDimDomyunEx Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("BOTTOM");

	CDPoint xy, xybase(0,0);
	
	Dom.DimMoveTo(xybase, 2);
	xy = pObjSide->GetPoint(3);
	Dom.DimLineTo(xy.x, xy.y, "lW");

	xy = pObjSide->GetPoint(2);
	Dom.DimMoveTo(xybase, 2);
	Dom.DimLineTo(xy.x, xy.y, "rW");
	
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimAddPlate(CDomyun *pDom,CPlatePieceObject *pObjAdd)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDimDomyunEx Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);


	//VECTOR
	CDPoint vDir(1,1);		vDir = vDir.Unit();
	//RADIUS
	CDPoint iR(pObjAdd->GetChild(0)->GetPoint(2));
	//CENTER
	CDPoint cen(pObjAdd->GetPoint(0));
	//XY POINT
	CDPoint xy = cen + vDir*iR.y;

	CString str = "iR";
	double len = Dom.GetTextWidth(str, 3);
	Dom.GiSiArrowExt(xy.x, xy.y, len, len/2, 1, str);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawSideMainPlateView(CDomyun *pDom, long nRow,  bool bAddPlate ,bool bSidePlate)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	if(nRow < 0) nRow = 0;
	double d = pGir->m_pSangse->m_LiftLug_d[nRow];		//구멍 지름
	double C = pGir->m_pSangse->m_LiftLug_C[nRow];
	double D = pGir->m_pSangse->m_LiftLug_D[nRow];		//구멍까지 높이
	double R  = pGir->m_pSangse->m_LiftLug_R[nRow];		//외각 반지름
	double T = pGir->m_pSangse->m_LiftLug_T[nRow];		
	double H = D + R;
	double iR = pGir->m_pSangse->m_LiftLug_iR[nRow];
	double lT = pGir->m_pSangse->m_LiftLug_lT[nRow];
	double rT = pGir->m_pSangse->m_LiftLug_rT[nRow];
	double uH = H - ( R - iR );
	double dH = uH  - iR * 2;

	CDPoint xy;
	pOptStd->SetEnvLType(&Dom, LT_DOT_LINE);
	xy.x = -T/2;	xy.y = D-d/2;	Dom.MoveTo(xy);
	xy.x =  T/2;	xy.y = D-d/2;	Dom.LineTo(xy);
	xy.x = -T/2;	xy.y = D+d/2;	Dom.MoveTo(xy);
	xy.x =  T/2;	xy.y = D+d/2;	Dom.LineTo(xy);

	if(bSidePlate)
	{
		pOptStd->SetEnvLType(&Dom, LT_DOT_LINE);
		//CEN HIDDEN PART
		xy.x = -T/2;	xy.y = 0;	Dom.MoveTo(xy);
		xy.x = -T/2;	xy.y = C;	Dom.LineTo(xy);
		xy.x =  T/2;	xy.y = C;	Dom.LineTo(xy);
		xy.x =	T/2;	xy.y = 0;	Dom.LineTo(xy);
		pOptStd->SetEnvLType(&Dom, LT_SOLID);
		xy.x = -T/2;	xy.y = C;	Dom.MoveTo(xy);
		xy.x = -T/2;	xy.y = H;	Dom.LineTo(xy);
		xy.x =  T/2;	xy.y = H;	Dom.LineTo(xy);
		xy.x =	T/2;	xy.y = C;	Dom.LineTo(xy);
	}
	else
	{
		pOptStd->SetEnvLType(&Dom, LT_SOLID);
		CDPoint xy;
		xy.x = -T/2;	xy.y = 0;	Dom.MoveTo(xy);
						xy.y = H;	Dom.LineTo(xy);
		xy.x = T/2;					Dom.LineTo(xy);
						xy.y = 0;	Dom.LineTo(xy);
		xy.x = -T/2;				Dom.LineTo(xy);
	}
	*pDom << Dom;
	//치수선 그리기
	CDimDomyunEx DomDim(pDom);
	pOptStd->SetEnvType(&DomDim,HCAD_DIML);

	int nDan = 1;
	DomDim.SetDirection("BOTTOM");
	xy.x = -T/2;	xy.y = 0;
	DomDim.DimMoveTo(xy,nDan);
	xy.x = T;
	DomDim.DimLineTo(xy.x, xy.y, "T");
	
	//편복부판
	if(bAddPlate)
	{
		pOptStd->SetEnvLType(&DomDim, LT_DOT_LINE);
		xy.x =  T/2;	xy.y = D-d/2;	DomDim.MoveTo(xy);
		xy.x +=  rT;	xy.y = D-d/2;	DomDim.LineTo(xy);
		xy.x =  T/2;	xy.y = D+d/2;	DomDim.MoveTo(xy);
		xy.x +=  rT;	xy.y = D+d/2;	DomDim.LineTo(xy);

		xy.x = -T/2;	xy.y = D-d/2;	DomDim.MoveTo(xy);
		xy.x -=  lT;	xy.y = D-d/2;	DomDim.LineTo(xy);
		xy.x = -T/2;	xy.y = D+d/2;	DomDim.MoveTo(xy);
		xy.x -=  lT;	xy.y = D+d/2;	DomDim.LineTo(xy);
		if(!bSidePlate && dH >= C)
		{
			//RIGHT
			pOptStd->SetEnvLType(&DomDim, LT_SOLID);
			xy.x = T/2;		xy.y = dH;	DomDim.MoveTo(xy);
			xy.x += rT;					DomDim.LineTo(xy);
							xy.y = uH;	DomDim.LineTo(xy);
			xy.x -= rT;					DomDim.LineTo(xy);

			//LEFT
			xy.x = -T/2;	xy.y = dH;	DomDim.MoveTo(xy);
			xy.x -= lT;					DomDim.LineTo(xy);
							xy.y = uH;	DomDim.LineTo(xy);
			xy.x += lT;					DomDim.LineTo(xy);
		}
		else
		{
			//RIGHT
			pOptStd->SetEnvLType(&DomDim, LT_DOT_LINE);
			xy.x = T/2;		xy.y = dH;	DomDim.MoveTo(xy);
			xy.x += rT;		xy.y = dH;	DomDim.LineTo(xy);
							xy.y = C;	DomDim.LineTo(xy);
			pOptStd->SetEnvLType(&DomDim, LT_SOLID);
							xy.y = uH;	DomDim.LineTo(xy);				
			xy.x -= rT;					DomDim.LineTo(xy);

			//LEFT
			pOptStd->SetEnvLType(&DomDim, LT_DOT_LINE);
			xy.x = -T/2;	xy.y = dH;	DomDim.MoveTo(xy);
			xy.x -= lT;		xy.y = dH;	DomDim.LineTo(xy);
							xy.y = C;	DomDim.LineTo(xy);
			pOptStd->SetEnvLType(&DomDim, LT_SOLID);
							xy.y = uH;	DomDim.LineTo(xy);
			xy.x += lT;					DomDim.LineTo(xy);
		}
		//RIGHT DIM
		nDan = 0;
		xy.x = T/2;		xy.y = 0;
		DomDim.DimMoveTo(xy,nDan);
		xy.x = rT;
		DomDim.DimLineTo(xy.x, xy.y, "rT");

		//LEFT DIM
		xy.x = -T/2;
		DomDim.DimMoveTo(xy,nDan);
		xy.x = -lT;
		DomDim.DimLineTo(xy.x, xy.y, "lT");
	}
	*pDom << DomDim;
}

//Jackup soleplate

void CAPlateDrawDanmyun::DrawJSolePlate(CDomyun *pDom, long nGirder, long nJijum, long nPos)
{
	CPlateBridgeApp	*pBridge    = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirder	= pBridge->GetGirder(nGirder);

	double dSta		= pGirder->GetStationOnJijum(nJijum);
	double dVSlop	= pGirder->GetGiulJongDanOnlyLine(dSta);
	double dBaseX = 0;
	double dBaseY = 0;

	CDomyun Dom(pDom);
	DrawJSolePlatePlane(&Dom, dBaseX, dBaseY);

	dBaseX = 1250;

	DrawJSolePlateSide(&Dom, dVSlop, dBaseX, dBaseY);

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimJSolePlate(CDomyun *pDom, long nGirder, long nJijum, long nPos)
{
	CPlateBridgeApp	*pBridge    = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirder	= pBridge->GetGirder(nGirder);

	double dSta		= pGirder->GetStationOnJijum(nJijum);
	double dVSlop	= pGirder->GetGiulJongDanOnlyLine(dSta);
	double dBaseX = 0;
	double dBaseY = 0;

	CDomyun Dom(pDom);
	DimJSolePlatePlane(&Dom, dBaseX, dBaseY);

	dBaseX = 1250;

	DimJSolePlateSide(&Dom, dVSlop, dBaseX, dBaseY);

	*pDom << Dom;
}

// 잭업 솔플레이트 상세
void CAPlateDrawDanmyun::DrawJSolePlatePlane(CDomyun *pDom, double dBaseX, double dBaseY)
{
/*	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double dW = pJSolePlate->m_dW		;	// 종방향 길이
	double dH = pJSolePlate->m_dH		;	// 교축직각 길이

	Dom.Rectangle(-dW / 2, -dH / 2, dW / 2, dH / 2);

	Dom.Move(dBaseX, dBaseY);

	*pDom << Dom;*/
}

void CAPlateDrawDanmyun::DrawJSolePlateSide(CDomyun *pDom, double dVSlop, double dBaseX, double dBaseY)
{
/*	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double	dW	   = pJSolePlate->m_dW	 ;	// 종방향 길이
	double	dT1	   = pJSolePlate->m_dT1	 ;	// 좌측부 두께
	double	dT	   = pJSolePlate->m_dT	 ;	// 중앙부 두께
	double	dT2	   = pJSolePlate->m_dT2	 ;	// 우측부 두께

	// SOLE PLATE
	Dom.LineTo(-dW / 2,   0,  dW / 2,   0);
	Dom.LineTo(-dW / 2,   0, -dW / 2, dT1);
	Dom.LineTo( dW / 2,   0,  dW / 2, dT2);
	Dom.LineTo(-dW / 2, dT1,  dW / 2, dT2);	

	Dom.Move(dBaseX, dBaseY);

	*pDom << Dom;*/
}

void CAPlateDrawDanmyun::DimJSolePlatePlane(CDomyun *pDom, double dBaseX, double dBaseY)
{
/*	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double dW = pJSolePlate->m_dW;	// 종방향 길이
	double dH = pJSolePlate->m_dH;	// 교축직각 길이

	Dom.SetDirection("TOP");

	Dom.DimMoveTo(-dW / 2, dH / 2, 0);

	Dom.DimLineTo(dW, 0, "W");

	Dom.SetDirection("RIGHT");

	Dom.DimMoveTo(dW/2, -dH / 2, 0);

	Dom.DimLineTo(dH, 0, "H");

	Dom.SetDirection("TOP");

	Dom.GiSiArrow(-dW / 4, 0, dW / 2, 0, 0, "교축방향");

	Dom.Move(dBaseX, dBaseY);

	*pDom << Dom;*/
}

void CAPlateDrawDanmyun::DimJSolePlateSide(CDomyun *pDom, double dVSlop, double dBaseX, double dBaseY)
{
/*	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double	dW		= pJSolePlate->m_dW		;	// 종방향 길이
	double	dT1		= pJSolePlate->m_dT1	;	// 좌측부 두께
	double	dT		= pJSolePlate->m_dT		;	// 중앙부 두께
	double	dT2		= pJSolePlate->m_dT2	;	// 우측부 두께
	BYTE	bLevel	= pJSolePlate->m_bLevel	;	// 레벨 적용

	Dom.SetDirection("BOTTOM");

	Dom.DimMoveTo(-dW / 2, 0, 0);

	Dom.DimLineTo(dW, 0, "W");

	Dom.SetDirection("LEFT");

	Dom.DimMoveTo(-dW / 2, 0, 0);

	Dom.DimLineTo(dT1, 0, "T1");

	Dom.SetDirection("RIGHT");

	Dom.DimMoveTo(dW / 2, 0, 0);

	Dom.DimLineTo(dT2, 0, "T2");

	Dom.SetDimExo(0);
	Dom.SetDimObq(-45);
	Dom.DimMoveTo(0, 0, 0);

	Dom.DimLineTo(dT, 0, "T");

	Dom.Move(dBaseX, dBaseY);

	*pDom << Dom;

	// 종구배 표시
	if(bLevel)
	{
		Dom.SetDirection("TOP");

		CString str;

			str.Format("LEVEL", dVSlop);

		Dom.GiSiArrow(-dW / 4, dT * 1.5, dW / 2, 0, 0, str);
	}
	else
	{
		Dom.SetDirection("TOP");

		CString str;

		str.Format("S = %.4f%%", dVSlop);

		Dom.GiSiArrow(-dW / 4, dT * 1.5, dW / 2, 0, 0, str);
		Dom.Rotate(atan(dVSlop), 0, dT * 1.5);
	}

	Dom.Move(dBaseX, dBaseY);

	*pDom << Dom;*/
}
//상하판너비 변화부상세
void CAPlateDrawDanmyun::DrawInputPlateWShift(CDomyun *pDom, double dScale)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();

	double dW1	  = 20;
	double dW2	  = 30;

	BOOL bUpper	  =  TRUE;
	BOOL bCenter  = FALSE;
	BOOL bDimDraw =  TRUE;

	CDomyun Dom(pDom);
	double dRatioH = pBridge->m_HTaper_H;
	double dRatioW = pBridge->m_HTaper_W;
	DrawPlateWShift(&Dom, dW1, dW2, dScale, bUpper, bCenter, dRatioH, dRatioW);
	DimPlateWShift(&Dom, dW1, dW2, dScale, bUpper, bCenter, bDimDraw, dRatioH, dRatioW);
	*pDom << Dom;
}


void CAPlateDrawDanmyun::DrawPlateWShift(CDomyun *pDom, double dW1, double dW2, double dScale, BOOL bUpper, BOOL bCenter, double dRatioH, double dRatioW)
{
	CARoadOptionStd *pOptStd		= m_pDataMng->GetOptionStd();
	CPlateBridge			*pBridge		= m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetScaleDim(dScale);

	double dScale2 = 1.5;

	dW1 *= dScale2;
	dW2 *= dScale2;

	double dW_Min = min(dW1, dW2);
	double dW_Max = max(dW1, dW2);

	// 접합될 부재의 길이
	double dL_Min = 100;
	double dL_Max = 100;

	// 경사결정
	double dSlop	= 0.1; // 1:10

	if(dRatioH && dRatioW) dSlop = dRatioH / dRatioW;

	// 플레이트,플레이트
	double dL_Slop = (dW_Max/2 - dW_Min/2) / dSlop;

	CDPoint pBase = CDPoint(      0, dW_Min/2);			// (1)
	CDPoint pSlop = CDPoint(dL_Slop, dW_Max/2);			// (2)

	Dom.LineTo(-dL_Min, dW_Min/2,       0, dW_Min/2);	// (0)
	Dom.LineTo(pBase.x,  pBase.y, pSlop.x,  pSlop.y);	// (3) : Taper
	Dom.LineTo(pSlop.x,  pSlop.y,  dL_Max, dW_Max/2);	// (4)

	pBase.y -= dW_Min;
	pSlop.y -= dW_Max;

	Dom.LineTo(-dL_Min, -dW_Min/2,       0, -dW_Min/2);	// (0)
	Dom.LineTo(pBase.x,   pBase.y, pSlop.x,   pSlop.y);	// (3) : Taper
	Dom.LineTo(pSlop.x,   pSlop.y,  dL_Max, -dW_Max/2);	// (4)

	// 좌우측 CUTTING (Arc/2*2)
	Dom.Arc(-dL_Min, dW_Min/2, -dL_Min - (dW_Min/2.0) / 10,  (dW_Min/2.0) / 2, -dL_Min,           0); // LEFT
	Dom.Arc(-dL_Min,        0, -dL_Min + (dW_Min/2.0) / 10, -(dW_Min/2.0) / 2, -dL_Min, -dW_Min / 2);
	Dom.Arc( dL_Max, dW_Max/2,  dL_Max - (dW_Max/2.0) / 10,  (dW_Max/2.0) / 2,  dL_Max,           0); // RIGHT
	Dom.Arc( dL_Max,        0,  dL_Max + (dW_Max/2.0) / 10, -(dW_Max/2.0) / 2,  dL_Max, -dW_Max / 2);

	// 방향별로 조정(MIRROR)
	if(dW1 > dW2)	Dom.Mirror(0, TRUE);	// 좌우로 MIRROR

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimPlateWShift(CDomyun *pDom, double dW1, double dW2, double dScale, BOOL bUpper, BOOL bCenter, BOOL bDimDraw, double dRatioH, double dRatioW)
{
	CARoadOptionStd *pOptStd		= m_pDataMng->GetOptionStd();
	CPlateBridge			*pBridge		= m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);

	BOOL bMirror = (dW1 > dW2) ? TRUE : FALSE;

	double dScale2 = 1.5;

	dW1 *= dScale2;
	dW2 *= dScale2;

	// DRAWING의 기준은 이음면의 중심하단,
	double dW_Min = min(dW1, dW2);
	double dW_Max = max(dW1, dW2);

	// 용접될 부재의 길이
	double dL_Min = 100;
	double dL_Max = 100;

	double dSlop = 0.1; // 1:5

	if(dRatioH && dRatioW) dSlop = dRatioH / dRatioW;

	// 플레이트
	double dL_Slop = (dW_Max - dW_Min) / 2 / dSlop;

	CDPoint pXY_Point = CDPoint(0, dW_Min / 2);											// (4)
	CDPoint pXY_Slop  = CDPoint(bMirror ? -dL_Slop : dL_Slop, dW_Max / 2);	// (0)

	double dW	   = 0;	// 좌우 치수
	double dL_Text = 0;
	double dL_Dim  = 0;

	Dom.SetScaleDim(dScale);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	// SHORT LENGTH
	CDPoint pXY = CDPoint(bMirror ? dL_Min : -dL_Min, 0);
	Dom.SetDirection(bMirror? "RIGHT" : "LEFT");

	dW = dW_Min;

	if(!bDimDraw)
	{
		Dom.DimMoveTo(pXY.x, pXY.y, 1);
		Dom.DimLineTo(pXY.x, (long)dW, COMMA(dW / dScale2));

		if(bMirror)
		{
			pXY.x = pXY.x + (Dom.GetDimExo() + Dom.GetDimVLen()) * Dom.GetScaleDim();

			dL_Text = Dom.Always(12);
			dL_Dim  = dL_Text * -1 * 2;

			Dom.LineTo(pXY.x, pXY.y, pXY.x, pXY.y + dL_Dim);
			Dom.SetTextAngle(90);

			dL_Dim = -dL_Text;
			Dom.TextOut(pXY.x - Dom.Always(1), pXY.y + dL_Dim, "(BOX INSIDE)");
		}
	}

	// LONG LENGTH
	pXY = CDPoint(bMirror ? -dL_Max : dL_Max, 0);

	Dom.SetDirection(bMirror ? "LEFT" : "RIGHT");

	Dom.DimMoveTo(pXY.x, dW_Min / 2, 0);
	Dom.DimLineTo((dW_Max - dW_Min)/2, 0, bDimDraw ? "H1" : COMMA((dW_Max - dW_Min) / 2 / dScale2));

	if(!bDimDraw)
	{
		Dom.DimMoveTo(pXY.x, pXY.y, 2);
		Dom.DimLineTo(pXY.x, (long)dW_Max, COMMA(dW_Max / dScale2));

		if(!bMirror)
		{
			pXY.x = pXY.x + (Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis()) * Dom.GetScaleDim();

			dL_Text = Dom.Always(12);
			dL_Dim  = dL_Text * -1 * 2;

			Dom.LineTo(pXY.x, pXY.y, pXY.x, pXY.y + dL_Dim);
			Dom.SetTextAngle(90);

			dL_Dim = -dL_Text;
			Dom.TextOut(pXY.x - Dom.Always(1), pXY.y + dL_Dim, "(BOX INSIDE)");
		}
	}

	if(!bUpper) Dom.Mirror(0,FALSE);

	*pDom << Dom;

	Dom.SetScaleDim(dScale);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	////////////// MIRROR 영향 ///////////////
	Dom.SetTextAngle(0);

	CString str(_T(""));

	if(bUpper)
	{
		// TAPER
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(pXY_Slop.x, pXY_Slop.y, 0);
		Dom.DimLineTo(bMirror ? dL_Slop : -dL_Slop, 0, bDimDraw ? "W1" : COMMA(dL_Slop / 2));
	}
	else
	{
		// TAPER
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(pXY_Slop.x, pXY_Slop.y - dW_Max * 2, 2);
		Dom.DimLineTo(bMirror ? dL_Slop : -dL_Slop, (long)(pXY_Point.y - dW_Min * 2), COMMA(dL_Slop / 2));
	}

	// TAPER
	str.Format("1 : %.1f", 1 / dSlop);

	double dAngle_Text = 90;

	if(bUpper)
	{
		if(dW1 < dW2)
		{
			dAngle_Text = ToDegree(atan(dSlop));

			Dom.SetTextAngle(dAngle_Text);
			Dom.TextOut((pXY_Point.x + (dW_Max - dW_Min) / 2 / dSlop) / 2, (pXY_Point.y + dW_Max / 2) / 2, str);
		}
		else
		{
			dAngle_Text = -ToDegree(atan(dSlop));

			Dom.SetTextAngle(dAngle_Text);
			Dom.TextOut((-pXY_Point.x - (dW_Max - dW_Min) / 2 / dSlop) / 2, (pXY_Point.y + dW_Max / 2) / 2, str);
		}
	}
	else
	{
		if(dW1 < dW2)
		{
			dAngle_Text = -ToDegree(atan(dSlop));

			Dom.SetTextAngle(dAngle_Text);
			Dom.TextOut((pXY_Point.x + (dW_Max - dW_Min) / 2 / dSlop) / 2, (pXY_Point.y + dW_Max / 2) / 2 - dW_Max / 2 * 2, str);
		}
		else
		{
			dAngle_Text = ToDegree(atan(dSlop));

			Dom.SetTextAngle(dAngle_Text);
			Dom.TextOut((-pXY_Point.x - (dW_Max - dW_Min) / 2 / dSlop) / 2, (pXY_Point.y + dW_Max / 2) / 2 - dW_Max / 2 * 2, str);
		}
	}

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimTypicalSectionUpper(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL)	return;
	CARoadOptionStd *pOptStd		= m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge		= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir			= pBridge->GetGirder(0);

	CDomyun Dom(pDom);
	
	////////////////////////////////////////////
	// 상면 치수선 : 거더 배치
	Dom.SetDirection("TOP");
	
	long nG=0, cnt=0;
	long GirderSu = pBridge->GetGirdersu();
	const long MaxGIRDER = 20;
	double dx[MaxGIRDER*2+2] = {0};	// Max Girder = 10EA +2(Bracket)
	CString str;

	// 좌측 슬래브 끝
	dx[0] = pBridge->GetSlabLeft()->m_dGirderCenter;//pBridge->GetDisSlabLeftDom(pBx);

	// BOX POINT (좌우측 WEB)
	cnt = 1;
	for(nG=0; nG<GirderSu; nG++) {
		pGir = pBridge->GetGirder(nG);
		dx[cnt+0] = pGir->m_dGirderCenter;
		dx[cnt+1] = pGir->m_dGirderCenter;
		cnt += 2;
	}

	CDPoint B[HDANLINESU], vX(1,0), vY(0,1);
	pBridge->GetXyPaveUpperDom(pBx, B);

	// 우측 슬래브 끝
	dx[cnt] = pBridge->GetSlabRight()->m_dGirderCenter;//pBridge->GetDisSlabRightDom(pBx);

	double dy = B[0].y;
	dy = max(dy,B[pBridge->GetQtyHDanNode()].y);

	//////////////////////////////////////////////////////////////////////////////////////////
	// LEVEL 치수
	long DimDan = 1;	
	for(long n=0; n<pBridge->GetQtyHDanNode(); n++)
	{
	//	if(pBridge->GetTypeLevelHDan(pBx,n))		//Level이 아닐때
		if(pBridge->GetTypeLevelHDan(n))		//Level이 아닐때
		{
			Dom.DimMoveTo(B[n].x,0,DimDan);
			Dom.DimLineToExtend(B[n+1].x-B[n].x,DimDan,COMMA(B[n+1].x-B[n].x),"(LEVEL)");
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// 1단치수 
	DimDan = 2;
	Dom.DimMoveTo(dx[0],0,DimDan);
	Dom.DimLineTo(dx[1]-dx[0],DimDan,COMMA(dx[1]-dx[0]));
	cnt = 1;
	for(; cnt < GirderSu*2; cnt++) {
		Dom.DimLineTo(dx[cnt+1]-dx[cnt],DimDan,COMMA(dx[cnt+1]-dx[cnt]));
	}
	Dom.DimLineTo(dx[cnt+1]-dx[cnt],DimDan,COMMA(dx[cnt+1]-dx[cnt]));

	//////////////////////////////////////////////////////////////////////////////////////////
	// 2단치수
	long DimCase = 0;
	if(dx[GirderSu*2+1] <= 0)	DimCase = 1;	
	if(dx[0] >= 0)				DimCase = 2;

	DimDan++;
	switch(DimCase) {
		case 0:	// 도로중심이 거더사이에 있는 경우
			Dom.DimMoveTo(dx[0],0,DimDan);
			Dom.DimLineTo(fabs(dx[0]),DimDan,COMMA(fabs(dx[0])));
			Dom.DimLineTo(dx[GirderSu*2+1],DimDan,COMMA(dx[GirderSu*2+1]));
			break;
		case 1:	// 거더가 모두 도로 좌측에 있는 경우
			Dom.DimMoveTo(dx[0],0,DimDan);	
			Dom.DimLineTo(dx[GirderSu*2+1]-dx[0],DimDan,COMMA(dx[GirderSu*2+1]-dx[0]));
			if(dx[GirderSu*2-1] < 0) 
				Dom.DimLineTo(fabs(dx[GirderSu*2+1]),DimDan,COMMA(fabs(dx[GirderSu*2+1])));
			break;
		case 2:	// 거더가 모두 도로 우측에 있는 경우
			Dom.DimMoveTo(0,0,DimDan);
			if(dx[0] >0) 
				Dom.DimLineTo(dx[0],DimDan,COMMA(dx[0]));
			Dom.DimLineTo(dx[GirderSu*2+1]-dx[0],DimDan,COMMA(dx[GirderSu*2+1]-dx[0]));
			break;
		default: 
			break;
	}

	//도로중심선표현 
	pOptStd->SetEnvType(&Dom, HCAD_CENT);	

	CPlateGirderApp	 *pGirCen	= pBridge->GetGirder(-1);
	CPlateBasicIndex *pBxMatch	= pBx->GetBxMatchByCrossBeam(pGirCen);
	pGirCen->GetHunchXyDom(pBxMatch, B);

	double dX = pGirCen->m_dGirderCenter;
	double dY = B[0].y - pGirCen->GetHeightGirderByStaAng(pBx->GetStation());
	double dThickSlab = pBridge->m_dThickPave+pBridge->m_dThickSlabBase+pBridge->m_dThickSlabHunch;

	CDPoint xyStt = CDPoint(dX, dY);
	CDPoint xyEnd = CDPoint(dX, dY+dThickSlab+pGirCen->GetHeightGirderByStaAng(pBx->GetStation())+Dom.Always(10));
	if(pBx->IsAbut())
		xyEnd = CDPoint(dX, dY+dThickSlab+pGirCen->GetHeightGirderByStaAng(pBx->GetStation())+pBridge->m_dHeightTaperStt+Dom.Always(10));
	Dom.LineTo(xyStt, xyEnd);
	//CL of Road
	Dom.SetTextAlignHorz(TA_LEFT);

	Dom.TextOut(xyEnd, "C");
	Dom.TextOut(xyEnd+vX*Dom.GetTextHeight()/2-vY*Dom.GetTextHeight()/2, "L OF ROAD");

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimTypicalSectionBottom(CDomyun *pDomP, CPlateBasicIndex *pBx, long dAddLen, BOOL bApplyMirror)
{
	if(pBx==NULL)	return;

 	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirCen = pBridge->GetGirder(-1);

	long nGsu = pBridge->GetGirdersu();

	CDimDomyunEx Dom(pDomP);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);	

	Dom.SetDirection("BOTTOM");
	long DimDan = 2;

	CDPoint B[10];
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(0));
	pBridge->GetGirder(0)->GetHunchXyDom(pBxMatch, B);

	// 0번 거더
	double x   = pBridge->GetSlabLeft()->m_dGirderCenter;
	double y   = B[0].y - pBxMatch->GetHeightGirderByBx()*0.5;
	double Len = pBridge->GetWidthSlabLeft();
	CDPoint vX(1,0), vY(0,1);
	double PierHeight = dAddLen;

	if(pBx->IsAbut())	y -= PierHeight;

	Dom.DimMoveTo(x,   y, DimDan);
	Dom.DimLineTo(Len, y, COMMA(Len));

	for(long nG=0; nG < nGsu-1; nG++) 
	{
		pBxMatch = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(nG));
		pBridge->GetGirder(nG)->GetHunchXyDom(pBxMatch, B);
		Len      = pBridge->GetGirder(nG)->GetDisGirderToGirder(pBxMatch);
		y	     = B[0].y - pBxMatch->GetHeightGirderByBx();
		if(pBxMatch->IsAbut())
			y  = B[0].y - pBxMatch->GetHeightGirderByBx() - PierHeight;

		Dom.DimLineTo(Len,y,COMMA(Len));
	}

	Dom.DimLineTo(pBridge->GetWidthSlabRight(), y, COMMA(pBridge->GetWidthSlabRight()));

	//도로중심선표현 
	pOptStd->SetEnvType(&Dom, HCAD_CENT);	

	pBxMatch = pBx->GetBxMatchByCrossBeam(pGirCen);
	pGirCen->GetHunchXyDom(pBxMatch, B);

	double dX = pGirCen->m_dGirderCenter;
	double dY = B[0].y - pGirCen->GetHeightGirderByStaAng(pBx->GetStation());
	double dThickSlab = pBridge->m_dThickPave+pBridge->m_dThickSlabBase+pBridge->m_dThickSlabHunch;

	CDPoint xyStt = CDPoint(dX, dY);
	CDPoint xyEnd = CDPoint(dX, dY+dThickSlab+pGirCen->GetHeightGirderByStaAng(pBx->GetStation())+Dom.Always(10));
	if(pBx->IsAbut())
		xyEnd = CDPoint(dX, dY+dThickSlab+pGirCen->GetHeightGirderByStaAng(pBx->GetStation())+pBridge->m_dHeightTaperStt+Dom.Always(10));
	Dom.LineTo(xyStt, xyEnd);
	//CL of Road
	Dom.SetTextAlignHorz(TA_LEFT);
	if(bApplyMirror)
	{
		Dom.TextOut(xyEnd-vX*Dom.GetTextHeight(), "C");
		Dom.TextOut(xyEnd-vX*(Dom.GetTextHeight()/2+Dom.GetTextWidth("L OF ROAD"))-vY*Dom.GetTextHeight()/2, "L OF ROAD");
	}
	else
	{
		Dom.TextOut(xyEnd, "C");
		Dom.TextOut(xyEnd+vX*Dom.GetTextHeight()/2-vY*Dom.GetTextHeight()/2, "L OF ROAD");
	}
	*pDomP << Dom;
}	
	

void CAPlateDrawDanmyun::DimTypicalSectionSlabThick(CDomyun *pDomP,CPlateBasicIndex *pBx)
{
}

void CAPlateDrawDanmyun::DimFrontHDanExtBottom(CDomyun *pDomP, CPlateBasicIndex *pBx, long nSttDan)
{
}

void CAPlateDrawDanmyun::DimFrontHDanExtTop(CDomyun *pDom, CPlateBasicIndex *pBx, long nSttDan)
{/*
  	CARoadOptionStd *pOptStd		= m_pDataMng->GetOptionStd();
	CPlateBridge			*pBridge		= m_pDataMng->GetBridge();
	CPlateGirder			*pGirder		= pBridge->GetGirder(-1);

	CString str1,str2;
	double	sta  = pBx->GetStation();


	CDPoint A[HDANLINESU];
	pBridge->GetXyPaveUpperDom(pBx, A);

	double dWidth   = pBridge->GetWidthSlabDom(pBx);
	double SlabMaxY = max(pBridge->GetHeightSlabFromRoadCenterDom(pBx,pBridge->GetDisSlabLeftDom(pBx)),
			              pBridge->GetHeightSlabFromRoadCenterDom(pBx,pBridge->GetDisSlabRightDom(pBx)));
	double Dist = 0;

	CDomyun Dom(pDom);
	CDoubleArray dArr;
	CDimDBBasePlan *pBridgeDimPlan = pStd->GetDimDBBasePlan();
	pDBDimPlan->GetDimArrayHDanCenterRoad(pBx,dArr);
	Dom.DimMoveTo(dArr.GetAt(0), SlabMaxY,nSttDan);
	for(long n=1; n<dArr.GetSize(); n++)
	{
		Dist = dArr.GetAt(n) - dArr.GetAt(n-1);
		if (!EQUAL(Dist, 0, 0.001))
			Dom.DimLineTo(Dist,nSttDan,COMMA(fabs(Dist)));
	}

	Dom.DimMoveTo(A[0].x, SlabMaxY,++nSttDan);
	for(n = 0; n < pDB->GetQtyHDanNode(); n++)
	{
		Dist = pDB->GetLengthHDanDom(pBx,n);
		Dom.DimLineTo(Dist,nSttDan,COMMA(fabs(Dist)));
	}

	Dom.DimMoveTo(A[0].x, SlabMaxY,++nSttDan);
	Dom.DimLineTo(dWidth,nSttDan,COMMA(dWidth));

	*pDom << Dom;
*/
}

// bHalf	: 상하로 1/2
// bSttHalf	: 앞뒤로 1/2
void CAPlateDrawDanmyun::DrawPlanSolePlateBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bHalf, BOOL bSttHalf, BOOL bEndHalf)
{	
	CPlateGirderApp	*pGir = (CPlateGirderApp *)pBx->GetGirder();	

	if(!pBx->IsJijum()) return;	
	
	long nJijum = pGir->GetNumberJijumByBx(pBx);
	CShoe *pShoe = pGir->GetShoeByJijumNo(nJijum);

	double W = pShoe->GetWidthTotalSolePlate();//교축
	double H = pShoe->GetHeigthTotalSolePlate();//직각

	CDomyun *pDom = new CDomyun(pDomP);

	CDPoint vAng = pBx->GetAngle();
	CDPoint jAng = vAng.RotateInv90();
	CDPoint xyTL = CDPoint(-W/2, H/2);
	CDPoint xyTR = CDPoint( W/2, H/2);
	CDPoint xyBL = CDPoint(-W/2,-H/2);
	CDPoint xyBR = CDPoint( W/2,-H/2);

	xyTL = xyTL.Rotate(jAng);
	xyTR = xyTR.Rotate(jAng);
	xyBL = xyBL.Rotate(jAng);
	xyBR = xyBR.Rotate(jAng);

	CDPoint xyMidL(0,0),xyMidR(0,0),xyMidT(0,0),xyMidB(0,0);
	GetXyMatchSegAndSeg(CDPoint(0,0),CDPoint( 1,0),xyTR,xyBR,xyMidR);
	GetXyMatchSegAndSeg(CDPoint(0,0),CDPoint(-1,0),xyTL,xyBL,xyMidL);
	GetXyMatchSegAndSeg(CDPoint(0,0),CDPoint(0,0)+1*vAng,xyTL,xyTR,xyMidT);
	GetXyMatchSegAndSeg(CDPoint(0,0),CDPoint(0,0)+1*vAng,xyBL,xyBR,xyMidB);
	if(bHalf) 
	{
		if(bSttHalf)
		{
			pDom->LineTo(xyMidL,xyBL);
			pDom->LineTo(xyBL,xyMidB);
		}
		if(bEndHalf)
		{
			pDom->LineTo(xyMidB,xyBR);
			pDom->LineTo(xyBR,xyMidR);
		}
	}
	else
	{
		if(bSttHalf)
		{
			pDom->LineTo(xyMidT,xyTL);
			pDom->LineTo(xyTL,xyBL);
			pDom->LineTo(xyBL,xyMidB);
		}
		if(bEndHalf)
		{
			pDom->LineTo(xyMidT,xyTR);
			pDom->LineTo(xyTR,xyBR);
			pDom->LineTo(xyBR,xyMidB);
		}
	}

	long nG = pBx->GetNumberGirder();
	CDPoint xyShoe = pGir->GetXyGirderDis(pBx->GetStation(), 0, vAng); 	
	CDPoint lvAng  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());

	pDom->Rotate(CDPoint(0,0),lvAng);
	pDom->Move(xyShoe);

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawDanmyun::DimTextSlopeArrow(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxStt,  CPlateBasicIndex *pBxEnd, BOOL bMirror)
{
	CARoadOptionStd	*pOptStd		= m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge		= m_pDataMng->GetBridge();	
	CSlabApp		*pSlabLeft		= pBridge->GetSlabLeft();
	CSlabApp		*pSlabRight		= pBridge->GetSlabRight();

	CDomyun Dom(pDomP);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	// SLOPE 표시
	long	nRoundSlope	= m_pDataMng->GetGlobalOption()->GetDesignPointSlope();
	BOOL	bVarSlope	= FALSE;//종단방향의 기울기일경우 사용
	CString str1 = _T(""), str2 = _T("");
	
/*	Dom.SetDirection("TOP");

	if(pSlabRight->m_dGirderCenter < pBridge->GetGirder(-1)->m_dGirderCenter) //슬래브가 중심거더 좌측에 있는 경우
	{
		if(pBxStt && pBxEnd)	bVarSlope = pGir->GetLine()->IsSlopVar(pBxStt->GetStation(), pBxEnd->GetStation(), TRUE);
		// 일반적인 경우 dSlope1가 구배로 적용됨(램프구간에서 가변시 제외)	
		dDist  = pBridge->GetDisSlabRightDom(pBx) - dSlabWidth/2;
		// 교량중심을 기준으로 좌(1), 우측(2) 구배
		dEleD1  = pBridge->GetHeightSlabFromRoadCenterDom(pBx, dDist-dGap);
		dSlope1 = pGir->GetLine()->GetPyungubae(dStaCen, dDist-dGap);
		
		dEleD2  = pBridge->GetHeightSlabFromRoadCenterDom(pBx, dDist+dGap);
		dSlope2 = pGir->GetLine()->GetPyungubae(dStaCen, dDist+dGap);
		
		str1    = "S = " + GetStringOutSlope(dSlope1, nRoundSlope) + "%";
		str2    = "S = " + GetStringOutSlope(dSlope2, nRoundSlope) + "%";

		if(bVarSlope) str1.Format("S = VAR%%");

		dLen = Dom.GetTextWidth(str1,2);

		if(dSlope1 == dSlope2) //기울기 표시를 중앙에 하나만 해줌.
		{
			Dom.GiSiArrow(dDist-dLen/2, (dEleD1+dEleD2)/2+dTxtHeight, dLen, 0, 0, str1);
			Dom.Rotate(atan(-dSlope1), dDist, (dEleD1+dEleD2)/2+dTxtHeight);
			Dom.SetTextAngle(ToDegree(atan(dSlope1)));
		}
		else//기울기 표시를 양측에 해줌
		{
			Dom.GiSiArrow(dDist-dGap, dEleD1+dTxtHeight, -dLen, 0, 4, str1);
			Dom.Rotate(atan(-dSlope1), dDist-dLen/2, dEleD1+dTxtHeight);
			*pDomP << Dom;

			Dom.GiSiArrow(dDist+dGap, dEleD2+dTxtHeight,  dLen, 0, 0, str2);
			Dom.Rotate(atan(dSlope2), dDist+dLen/2, dEleD2+dTxtHeight);
			*pDomP << Dom;
		}
		*pDomP << Dom;
	}
	else if(pSlabLeft->m_dGirderCenter > pBridge->GetGirder(-1)->m_dGirderCenter) //슬래브가 중심거더 우측에 있는 경우
	{
		if(pBxStt && pBxEnd)
			bVarSlope = pGir->GetLine()->IsSlopVar(pBxStt->GetStation(), pBxEnd->GetStation(), FALSE);
		// 일반적인 경우 dSlope2가 구배로 적용됨(램프구간에서 가변시 제외)	
		dDist  = pBridge->GetDisSlabLeftDom(pBx) + dSlabWidth/2;
		// 교량중심을 기준으로 좌(1), 우측(2) 구배
		dEleD1  = pBridge->GetHeightSlabFromRoadCenterDom(pBx, dDist-dGap);
		dSlope1 = pGir->GetLine()->GetPyungubae(dStaCen, dDist-dGap);

		dEleD2  = pBridge->GetHeightSlabFromRoadCenterDom(pBx, dDist+dGap);
		dSlope2 = pGir->GetLine()->GetPyungubae(dStaCen, dDist+dGap);
		
		str1    = "S=" + GetStringOutSlope(dSlope1, nRoundSlope) + "%";
		str2    = "S=" + GetStringOutSlope(dSlope2, nRoundSlope) + "%";

		if(bVarSlope) str1.Format("S=VAR%%");

		dLen = Dom.GetTextWidth(str1,2);

		if(dSlope1 == dSlope2) //기울기 표시를 중앙에 하나만 해줌.
		{
			Dom.GiSiArrow(dDist-dLen/2, (dEleD1+dEleD2)/2+dTxtHeight, dLen, 0, 0, str2);
			Dom.Rotate(atan(dSlope2), dDist, (dEleD1+dEleD2)/2+dTxtHeight);
			Dom.SetTextAngle(ToDegree(atan(dSlope2)));
		}
		else//기울기 표시를 양측에 해줌
		{
			Dom.GiSiArrow(dDist-dGap/2, dEleD1+dTxtHeight, -dLen, 0, 4, str1);
			Dom.Rotate(atan(-dSlope1), dDist-dLen/2, dEleD1+dTxtHeight);
			*pDomP << Dom;

			Dom.GiSiArrow(dDist+dGap/2, dEleD2+dTxtHeight,  dLen, 0, 0, str2);
			Dom.Rotate(atan(dSlope2), dDist+dLen/2, dEleD2+dTxtHeight);
			*pDomP << Dom;
		}
		*pDomP << Dom;
	}
	else		// 중심거더가 슬래브 안에 있는 경우
	{
		if(pBxStt && pBxEnd)
			bVarSlope = pGir->GetLine()->IsSlopVar(pBxStt->GetStation(), pBxEnd->GetStation(), FALSE);
		// 일반적인 경우 dSlope2가 구배로 적용됨(램프구간에서 가변시 제외)	
		dDist  = pBridge->GetDisSlabLeftDom(pBx) + dSlabWidth/2 - pBridge->m_dDisLineToBridgeCen;
		// 교량중심을 기준으로 좌(1), 우측(2) 구배
		dEleD1  = pBridge->GetHeightSlabFromRoadCenterDom(pBx, dDist-dGap);
		dSlope1 = pGir->GetLine()->GetPyungubae(dStaCen, dDist-dGap);

		dEleD2  = pBridge->GetHeightSlabFromRoadCenterDom(pBx, dDist+dGap);
		dSlope2 = pGir->GetLine()->GetPyungubae(dStaCen, dDist+dGap);
		
		str1    = "S=" + GetStringOutSlope(dSlope1, nRoundSlope) + "%";
		str2    = "S=" + GetStringOutSlope(dSlope2, nRoundSlope) + "%";

		if(bVarSlope) str1.Format("S=VAR%%");

		dLen = Dom.GetTextWidth(str1,2);

		if(dSlope1 != dSlope2) //기울기 표시를 중앙에 하나만 해줌.
		{
			Dom.GiSiArrow(dDist-dLen/2, (dEleD1+dEleD2)/2+dTxtHeight, dLen, 0, 0, str2);
			Dom.Rotate(atan(dSlope2), dDist, (dEleD1+dEleD2)/2+dTxtHeight);
			Dom.SetTextAngle(ToDegree(atan(dSlope2)));
		}
		else//기울기 표시를 양측에 해줌
		{
			Dom.GiSiArrow(dDist-dGap/2, dEleD1+dTxtHeight, -dLen, 0, 4, str1);
			Dom.Rotate(atan(-dSlope1), dDist-dLen/2, dEleD1+dTxtHeight);
			*pDomP << Dom;

			Dom.GiSiArrow(dDist+dGap/2, dEleD2+dTxtHeight,  dLen, 0, 0, str2);
			Dom.Rotate(atan(dSlope2), dDist+dLen/2, dEleD2+dTxtHeight);
			*pDomP << Dom;
		}
		*pDomP << Dom;
	}
	*pDomP << Dom;*/

	double StaCen  = pBx->GetStation();
	double Slope   = 0; 
	double Dist,EleD,Len;
	CString str = _T("");
	CPlateGirderApp	*pGirCen = pBridge->GetGirder(-1);

	if(pBridge->GetDisSlabLeftDom(pBx)<1000) 
	{
		if(pBxStt && pBxEnd)
			bVarSlope = pGirCen->GetLine()->IsSlopVar(pBxStt->GetStation(), pBxEnd->GetStation(), TRUE);
		Dist  = pBridge->GetDisSlabLeftDom(pBx)/2;
		EleD  = pBridge->GetHeightSlabFromRoadCenterDom(pBx,Dist);
		Slope = pGirCen->GetLine()->GetPyungubae(StaCen,Dist);
		str   = "S = " + GetStringOutSlope(Slope,nRoundSlope) + "%";
		if(bVarSlope) str.Format("S = VAR%%");
		Len = Dom.GetTextWidth(str,4);
		Dom.GiSiArrow(Dist+Len/2,EleD+Dom.GetTextHeight(),-Len,0,4,"");
		Dom.Rotate(atan(-Slope),Dist,EleD+Dom.GetTextHeight());
		Dom.SetTextAngle(ToDegree(atan(-Slope)));
		CDPoint xy = CDPoint(Dist,EleD+Dom.GetTextHeight()+Dom.Always(1));
		CDPoint vDir = CDPoint(1, -Slope).Unit();
		xy = xy.GetXyRotateByBase(CDPoint(Dist,EleD+Dom.GetTextHeight()), vDir);
		Dom.TextOut(xy,str);
		*pDomP << Dom;
	}
	if(pBridge->GetDisSlabRightDom(pBx)>1000) 
	{
		if(pBxStt && pBxEnd)
			bVarSlope = pGirCen->GetLine()->IsSlopVar(pBxStt->GetStation(), pBxEnd->GetStation(), FALSE);
		Dist  = pBridge->GetDisSlabRightDom(pBx)/2;
		EleD  = pBridge->GetHeightSlabFromRoadCenterDom(pBx,Dist);
		Slope = pGirCen->GetLine()->GetPyungubae(StaCen,Dist);
		str   = "S = " + GetStringOutSlope(Slope,nRoundSlope) + "%";
		if(bVarSlope) str.Format("S = VAR%%");
		Len = Dom.GetTextWidth(str,2);
		Dom.GiSiArrow(Dist-Len/2,EleD+Dom.GetTextHeight(),Len,Dom.Always(2),0,"");
		Dom.Rotate(atan(Slope),Dist,EleD+Dom.GetTextHeight());
		Dom.SetTextAngle(ToDegree(atan(Slope)));
		CDPoint xy = CDPoint(Dist,EleD+Dom.GetTextHeight()+Dom.Always(1));
		CDPoint vDir = CDPoint(1, Slope).Unit();
		xy = xy.GetXyRotateByBase(CDPoint(Dist,EleD+Dom.GetTextHeight()), vDir);
		Dom.TextOut(xy,str);
		*pDomP << Dom;
	}

	*pDomP << Dom;

}

// 하부구조물의 교좌면
void CAPlateDrawDanmyun::DrawPierPlace(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bDrawGirderMark)
{
	if(!pBx->IsJijum()) return;

	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();		
	CPlateGirderApp	*pGir		= pDB->GetGirder(0);	// 0번거더부터 시작
	CAPlateDrawSebu DrawSebu(m_pDataMng);

	long	nJijum	   = pGir->GetNumberJijumByBx(pBx);
	double	dSlabLeftX  = pDB->GetSlabLeft()->m_dGirderCenter;		// 도로중심 ~ 슬래브 좌단
	double	dSlabRightX = pDB->GetSlabRight()->m_dGirderCenter;		// 도로중심 ~ 슬래브 우단

	CDPoint sp(0,0),ep(0,0),A[41], B[10];
	CDomyun* pDom = new CDomyun(pDomP);
	/////////////// 교좌면
	CPlateBasicIndex *pBxCur = pBx->GetBxMatchByCrossBeam(pGir);	// 0번 거더
	CShoe *pShoe = pGir->GetShoe(nJijum);

	pGir->GetHunchXyDom(pBxCur, B);
	// dGirH에 하부플랜지 두께가 포함되지 않음
	double dGirEL		= ((B[1]+B[2])/2).y;	// 상부플랜지 하면의 높이
	double dGirH		= pGir->GetHeightGirderByStaAng(pBxCur->GetStation());
	double dFlangLT		= pGir->GetThickJewon(G_F_L, pBxCur);
	double dShoeBottom	= dGirEL - dGirH - dFlangLT - pShoe->GetHeightTotalShoe();
	double dSlabLeftY	= dShoeBottom;
	double dPreX		= dSlabLeftX;
	double dPreY		= dShoeBottom;

	for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		pGir	= pDB->GetGirder(nG);
		pBxCur	= pGir->GetBxOnJijum(nJijum);

		pGir->GetHunchXyDom(pBxCur, B);

		dGirEL	= ((B[1]+B[2])/2).y;	// 슬래브 하면의 높이
		dGirH	= pGir->GetHeightGirderByStaAng(pBxCur->GetStation());
		dFlangLT= pGir->GetThickJewon(G_F_L, pBxCur);

		dShoeBottom = dGirEL - dGirH - dFlangLT - pShoe->GetHeightTotalShoe();
		DrawSebu.DrawShoeBoxDetail(pDom,pBxCur,pGir->m_dGirderCenter,dShoeBottom,1.0,FALSE);

		double dX = dSlabRightX;
		if(nG != pDB->GetQtyGirder()-1)
			dX = pGir->m_dGirderCenter + pGir->GetDisFrontPlateWebToWeb(pBxCur)/2;
		pDom->LineTo(dPreX, dShoeBottom, dX, dShoeBottom);

		if(nG!=0)
			pDom->LineTo(dPreX, dPreY, dPreX, dShoeBottom);

		dPreX = dX;
		dPreY = dShoeBottom;
	}

	double dBottom = min(dPreY, dSlabLeftY) - 1000;
	pDom->LineTo(dSlabLeftX,  dBottom, dSlabLeftX,  dSlabLeftY);
	pDom->LineTo(dSlabRightX, dBottom, dSlabRightX, dPreY);

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawDanmyun::DrawFrontUpperStudOrAnchorBx(CDomyun *pDomP, CPlateBasicIndex *pBx, long nStudType, BOOL bInput)
{	
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
//	long nType = pGir->m_pSangse->m_StudHori_Type;

	CDPoint A[41], vAng;
	pGir->GetHunchXyDom(pBx,A);
	vAng = CDPoint(0,1);

	CDomyun	*pDom = new CDomyun(pDomP);

	double Thick  = pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	// --- 전단연결재 데이타
	double	HeadSide	= 5;	// Head는 Body(CD)보다 10넓다.(양쪽으로 5씩)	
	double CD = pGir->m_pSangse->m_StudHori_D;
	double CE = pGir->m_pSangse->m_StudHori_E;
	double CH = pGir->m_pSangse->m_StudHori_H;
	int     ScQty = pGir->m_pSangse->m_StudHori_Qty;
	
	double dTotalLen = 0;
	double dLen = 0;	
	double dWidth = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);		
	long  i = 0;
	for(i=0;i<ScQty-1;i++) dTotalLen += pGir->m_pSangse->m_StudHori_Dis[i];	
	for(i=0;i<ScQty;i++)
	{
		if(i==0) dLen = (dWidth - dTotalLen) / 2;
		else	 dLen += pGir->m_pSangse->m_StudHori_Dis[i-1]/vAng.y;
		double slop = 0;
		double BX=A[1].x;
		double BY= 0;
		BY = A[2].y + Thick;
			
		if(nStudType==0)
		{
			// 슬래브 앵커
			pDomP->AnchorBarFront(BX+dLen,BY,CD,slop);
		}
		else if(nStudType==1)
		{
			// 전단연결재
			pDom->LineTo(BX-CD/2+dLen, BY,	BX-CD/2+dLen, BY+CH);
			pDom->LineTo(BX+CD/2+dLen, BY,	BX+CD/2+dLen, BY+CH);
			pDom->Rectangle(BX-CD/2+dLen-HeadSide, BY+CH, BX+CD/2+dLen+HeadSide, BY+CH+CE);
		}
		else if(nStudType==2)
		{
			// 각형 전단연결재
			double	BX = A[1].x + dWidth/2;
			double	BY = A[2].y + Thick;
			double	dH = pGir->m_pSangse->m_StudHoriSQ_H;
			double	dL = pGir->m_pSangse->m_StudHoriSQ_L;
			pDom->Rectangle(BX-dL/2, BY+dH, BX+dL/2, BY);
		}
		pDom->Rotate(atan(slop), BX+dLen,BY);
		*pDomP << *pDom;
	}
	delete pDom;
}

void CAPlateDrawDanmyun::DrawDimFrontUpperStudOrAnchorBx(CDomyun *pDomP, CPlateBasicIndex *pBx, long nStudType, BOOL bInput)
{	
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();	

	CDPoint A[41], vAng;
	pGir->GetHunchXyDom(pBx,A);
	vAng = CDPoint(0,1);

	CDomyun	*pDom = new CDomyun(pDomP);	
	double Thick  = pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	// --- 전단연결재 데이타
	double CE = pGir->m_pSangse->m_StudHori_E;
	double CH = pGir->m_pSangse->m_StudHori_H;
	int     ScQty = pGir->m_pSangse->m_StudHori_Qty;
	
	double dTotalLen = 0;
	double dLen = 0;	
	double dWidth = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);		
	long	i = 0;
	if(nStudType==0)
	{		
	}
	else if(nStudType==1)
	{
		for(i=0;i<ScQty-1;i++) dTotalLen += pGir->m_pSangse->m_StudHori_Dis[i];
		pDom->SetDimVLen(7);
		pDom->SetDirection("LEFT");
		pDom->DimMoveTo(A[1].x+(dWidth - dTotalLen) / 2, A[2].y + Thick, 0);	
		pDom->DimLineTo(CH, 0, bInput ? "H" : COMMA(CH));		
		pDom->DimLineTo(CE, 0, bInput ? "E" : COMMA(CE));		

		pDom->SetDirection("TOP");
		dLen = (dWidth - dTotalLen) / 2;
		pDom->DimMoveTo(A[1].x, A[1].y+CE+CH+Thick, 1);
		pDom->DimLineTo(dLen, 1, COMMA(dLen));
		pDom->DimLineTo(dWidth-2*dLen, 1, COMMA(dWidth-2*dLen));
		pDom->DimLineTo(dLen, 1, COMMA(dLen));

			for(i=0;i<ScQty;i++)
			{		
				if(i==0) dLen = (dWidth - dTotalLen) / 2;
				else	 dLen += pGir->m_pSangse->m_StudHori_Dis[i-1]/vAng.y;
				double BX=A[1].x;
				double BY= 0;
				BY = A[2].y + Thick+CE+CH;

				CString szText = COMMA(pGir->m_pSangse->m_StudHori_Dis[i-1]);
				if(bInput) szText.Format("L%d",i);
				if(i==0)
					pDom->DimMoveTo(BX+dLen, BY, 0);
				else if(i%2 == 0)
					pDom->DimLineToExtend(pGir->m_pSangse->m_StudHori_Dis[i-1], 0, "", szText);
				else
					pDom->DimLineTo(pGir->m_pSangse->m_StudHori_Dis[i-1], 0, szText);

				*pDomP << *pDom;
			}
	}
	else if(nStudType==2)
	{
		double	BX = A[1].x + dWidth/2;
		double	BY = A[2].y + Thick;
		double	dH = pGir->m_pSangse->m_StudHoriSQ_H;
		double	dL = pGir->m_pSangse->m_StudHoriSQ_L;

		pDom->SetDirection("RIGHT");
		pDom->DimMoveTo(BX + dL/2, BY, 0);
		pDom->DimLineToExtend(dH, 0, COMMA(dH), "H");

		pDom->SetDirection("TOP");
		pDom->DimMoveTo(BX - dL/2, BY+dH, 0);
		pDom->DimLineToExtend(dL, 0, "L", COMMA(dL));
		*pDomP << *pDom;
	}

	delete pDom;
}

void CAPlateDrawDanmyun::DrawLaneMark(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	

	CPlateBxFinder Finder(pBridge->GetGirder(0));
	if(!pBx) pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pBridge->GetXyPaveUpperDom(pBx, A);

	double dTPave = pBridge->GetThickPave();
	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= dTPave;

	CDomyun Dom(pDom);		
	for(long n = 0; n < pBridge->GetQtyHDanNode(); n++)
	{
		if(pBridge->GetWallGuardTypeHDan(n) == 0)
			Dom.Circle(A[n],100,TRUE);		
	}
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawTensionBar(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput)
{
	CPlateBridgeApp	*pBridge	=	m_pDataMng->GetBridge();	

	if(pBridge->m_nSlabTensionSize==0) return;
	
	double m_dLHeight	=	pBridge->m_dHeightLeftTension;
	double m_dRHeight	=	pBridge->m_dHeightRightTension;

	CDPoint A[HDANLINESU];
	
	pBridge->GetXyPaveDom(pBx, A);	//?
	
	double dLDist	=	pBridge->GetSlabLeft()->m_dGirderCenter;
	double dRDist	=	pBridge->GetSlabRight()->m_dGirderCenter;
		
	double dX1, dX2, dY1, dY2;

	CDPoint ptStt		=	CDPoint(A[0].x, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist) - (pBridge->m_dThickPave + m_dLHeight));
	CDPoint ptSttUnit	=	ptStt;	
	CDPoint ptEnd		=	CDPoint(A[pBridge->GetQtyHDanDom(pBx)].x,pBridge->GetElevationPaveActByDis(pBx->GetStation(), dRDist) - (pBridge->m_dThickPave + m_dRHeight));
	CDPoint ptEndUnit	=	ptEnd;
	
	CDPoint xyResult1,	xyResult2, xyResult3, xyResult4;
	CDPoint ptSttTemp,	ptEndTemp;
	
	CDPoint ptCen1,		ptCen2,	ptCen3;
	CDPoint ptCenter,	ptCenter2;
	CDPoint xyMatch1,	xyMatch2;

	double dDistTotal	=	0;
	double dOffsetL1	=	5;
	double dOffsetL2	=	12.5;

	BOOL bLeftFix = (pBridge->m_nTendonFix != 1) ? TRUE : FALSE;	// 고정단이 좌측&지그재그 : TRUE, 우측 : FALSE
	
	for(long n=0; n<pBridge->m_nSlabTensionSize; n++)
	{
		double	dL	=	pBridge->m_SlabTension[n].m_dL;
		double	dR	=	pBridge->m_SlabTension[n].m_dR;
		double	dH	=	pBridge->m_SlabTension[n].m_dH;
		long	nT	=	pBridge->m_SlabTension[n].m_nType;

		dDistTotal	+=	dL;
		
		if(n==0)	ptCen1	=	ptStt;	
		else		ptCen1	=	ptCen2;

		ptCen2	=	CDPoint(ptStt.x + dDistTotal, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal) - pBridge->m_dThickPave - dH);
		ptCen3	=	CDPoint(ptStt.x + dDistTotal + pBridge->m_SlabTension[n+1].m_dL, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal+pBridge->m_SlabTension[n + 1].m_dL) - pBridge->m_dThickPave - pBridge->m_SlabTension[n + 1].m_dH);
		
		if(n==pBridge->m_nSlabTensionSize - 1)
			ptCen3	=	ptEnd;

		if(bInput) pDom->SetLineColor(1);
		
		CDPoint vDir1	=	(ptCen1 - ptCen2).Unit();
		CDPoint vDir2	=	(ptCen3 - ptCen2).Unit();
		CDPoint vDir3	=	(vDir1 + vDir2).Unit();
		
		//실제 반지름 구하기 - Start
		CDPoint vDir4	= vDir3.BetweenAngle(vDir1);
		double	dRealV	= (360 - vDir4.GetAngleDegree()) - 90;

		dRealV = ToRadian(dRealV);

		double dH1		= tan(dRealV)*dR;
		double dRealR	= sqrt(dH1*dH1 + dR*dR);
		
		//실제 반지름 구하기 - End
		ptCenter	=	ptCen2 + vDir3 * dRealR;	
		
		//변곡점 시점부분
		if(n==0)
		{	
			if(GetTangentCircle(ptStt, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				
				dX1	=	bLeftFix ? pBridge->m_dLengthSecMove : pBridge->m_dLengthSecMoveEnd;
				dX2 =	xyMatch1.x - ptStt.x;
				dY2 =	xyMatch1.y - ptStt.y;

				dY1 = (dX1 / dX2) * dY2;

				ptSttUnit.x	=	ptStt.x + dX1;
				ptSttUnit.y =	ptStt.y + dY1;

				if(bLeftFix)
					ptSttTemp	=	ptSttUnit;
				else
				{
					ptSttTemp.x	=	ptSttUnit.x+140;
					ptSttTemp.y	=	ptSttUnit.y+140*(xyMatch1-ptSttUnit).Unit().y;
				}
				
				pDom->LineTo(ptSttTemp, xyMatch1);			

				if(bInput) pDom->SetLineColor(4);

				if(bLeftFix)	DrawTensionBarStartUnit(pDom, ptSttUnit, TRUE);	// 고정단
				else			DrawTensionBarEndUnit(pDom, ptSttUnit, TRUE);	// 긴장단
												
				//덕트까지 그리기..
				double dLengthDuct = bLeftFix  ? pBridge->m_dLengthDuctOffset : 0;

				dX1	=	dLengthDuct;
				dX2 =	xyMatch1.x - ptSttUnit.x;
				dY2 =	xyMatch1.y - ptSttUnit.y;
				dY1 =	(dX1 / dX2) * dY2;
				
				if(bInput) pDom->SetLineColor(1);

				pDom->LineTo(ptSttTemp.x, ptSttTemp.y + dOffsetL1, ptSttTemp.x + dLengthDuct,ptSttTemp.y + dY1 + dOffsetL1);
				pDom->LineTo(ptSttTemp.x, ptSttTemp.y - dOffsetL1, ptSttTemp.x + dLengthDuct,ptSttTemp.y + dY1 - dOffsetL1);
				
				if(bInput) pDom->SetLineColor(4);

				pDom->LineTo(ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 + dOffsetL2, ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1-dOffsetL2);
				pDom->LineTo(ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 + dOffsetL2, xyMatch1.x, xyMatch1.y + dOffsetL2);
				pDom->LineTo(ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 - dOffsetL2, xyMatch1.x, xyMatch1.y - dOffsetL2);				
			}
		}
		else
		{
			//직선 그리기
			if(GetTangentCircle(xyMatch2, ptCenter, dR, xyResult1, xyResult2)==2)
			{
				if(nT==0)
					xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				else
					xyMatch1 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;

				if(bInput) pDom->SetLineColor(1);

				pDom->LineTo(xyMatch1, xyMatch2);

				if(bInput) pDom->SetLineColor(4);				
				
				pDom->LineTo(xyMatch2.x, xyMatch2.y + dOffsetL2, xyMatch1.x, xyMatch1.y + dOffsetL2);
				pDom->LineTo(xyMatch2.x, xyMatch2.y - dOffsetL2, xyMatch1.x, xyMatch1.y - dOffsetL2);
			}			
		}

		if(n!=pBridge->m_nSlabTensionSize)
		{

			//곡선 그리기
			double	dLength =	~(xyMatch1-ptCen2);
			CDPoint	vAng1	=	(xyMatch1-ptCen2).Unit();
			
			vAng1.MirrorHorz();

			GetTangentCircle(ptCen3, ptCenter, dR, xyResult1, xyResult2);

			if(nT == 0)
				xyMatch2	=	(xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
			else
				xyMatch2	=	(xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
			
			double	dAngle1	=	(xyMatch1 - ptCenter).Unit().GetAngleDegree();
			double	dAngle2	=	(xyMatch2 - ptCenter).Unit().GetAngleDegree() - dAngle1;
			
			if(bInput) pDom->SetLineColor(2);
			
			pDom->Arc(ptCenter, dR, dAngle1, dAngle2);

			if(bInput) pDom->SetLineColor(4);
			
			pDom->Arc(ptCenter.x, ptCenter.y + dOffsetL2, dR, dAngle1, dAngle2);
			pDom->Arc(ptCenter.x, ptCenter.y - dOffsetL2, dR, dAngle1, dAngle2);
			
		}

		if(n == pBridge->m_nSlabTensionSize-1)
		{
			dX1	=	bLeftFix ? pBridge->m_dLengthSecMoveEnd : pBridge->m_dLengthSecMove;
			dX2 =	xyMatch2.x - ptEnd.x;
			dY2 =	xyMatch2.y - ptEnd.y;
			
			dY1 =	(dX1 / dX2) * dY2;
			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;

			//마지막 부분
			if(bLeftFix)
				ptEndTemp	=	ptEndUnit;
			else
			{
				ptEndTemp.x	=	ptEndUnit.x - pBridge->m_dLengthDuctOffset;
				ptEndTemp.y	=	ptEndUnit.y + pBridge->m_dLengthDuctOffset * (xyMatch2 - ptEndUnit).Unit().y;
			} 
			
			if(bLeftFix)	DrawTensionBarEndUnit(pDom,		ptEndUnit, FALSE);	// 긴장단
			else			DrawTensionBarStartUnit(pDom,	ptEndUnit, FALSE);	// 고정단
			
			//140띄어서 마무리...	
			
			dX1	=	bLeftFix ? pBridge->m_dLengthSecMoveEnd + 140 : pBridge->m_dLengthSecMove;
			dX2	=	xyMatch2.x - ptEnd.x;
			dY2	=	xyMatch2.y - ptEnd.y;
			dY1	=	(dX1 / dX2) * dY2;
			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;
			
			if(bInput) pDom->SetLineColor(1);

			pDom->LineTo(xyMatch2, ptEndUnit);
			
			if(!bLeftFix)
			{
				pDom->LineTo(ptEndTemp.x, ptEndTemp.y + dOffsetL1, ptEndUnit.x, ptEndUnit.y + dOffsetL1);
				pDom->LineTo(ptEndTemp.x, ptEndTemp.y - dOffsetL1, ptEndUnit.x, ptEndUnit.y - dOffsetL1);
				if(bInput) pDom->SetLineColor(4);
				pDom->LineTo(ptEndTemp.x, ptEndTemp.y + dOffsetL2, ptEndTemp.x, ptEndTemp.y - dOffsetL2);
			}

			if(bInput) pDom->SetLineColor(4);
			
			pDom->LineTo(xyMatch2.x, xyMatch2.y + dOffsetL2, ptEndTemp.x, ptEndTemp.y + dOffsetL2);
			pDom->LineTo(xyMatch2.x, xyMatch2.y - dOffsetL2, ptEndTemp.x, ptEndTemp.y - dOffsetL2);
			
		}
	}
}

void CAPlateDrawDanmyun::DrawTensionBar_ExcelOut(CDomyun *pDom, CPlateBasicIndex *pBx, CStringArray& szArr, CDPointArray& xyArr, CDoubleArray& AngArr, BOOL bInput)
{
	CPlateBridgeApp	*pBridge	=	m_pDataMng->GetBridge();	
	
	szArr.RemoveAll();
	xyArr.RemoveAll();
	AngArr.RemoveAll();

	if(pBridge->m_nSlabTensionSize==0) return;
	
	double m_dLHeight	=	pBridge->m_dHeightLeftTension;
	double m_dRHeight	=	pBridge->m_dHeightRightTension;

	CDPoint A[HDANLINESU];
	
	pBridge->GetXyPaveDom(pBx, A);	//?

	double dLDist	=	pBridge->GetSlabLeft()->m_dGirderCenter;
	double dRDist	=	pBridge->GetSlabRight()->m_dGirderCenter;
		
	double dX1, dX2, dY1, dY2;

	CDPoint ptStt		=	CDPoint(A[0].x, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist) - (pBridge->m_dThickPave + m_dLHeight));
	CDPoint ptSttUnit	=	ptStt;	
	CDPoint ptEnd		=	CDPoint(A[pBridge->GetQtyHDanDom(pBx)].x,pBridge->GetElevationPaveActByDis(pBx->GetStation(), dRDist) - (pBridge->m_dThickPave + m_dRHeight));
	CDPoint ptEndUnit	=	ptEnd;
	
	CDPoint xyResult1,	xyResult2, xyResult3, xyResult4, xyPoint1, xyPoint2;
	CDPoint ptSttTemp,	ptEndTemp;
	
	CDPoint ptCen1,		ptCen2,	ptCen3;
	CDPoint ptCenter,	ptCenter2;
	CDPoint xyMatch1,	xyMatch2;

	double dDistTotal	=	0;
	double dOffsetL1	=	5;
	double dOffsetL2	=	12.5;

	BOOL bLeftFix = (pBridge->m_nTendonFix != 1) ? TRUE : FALSE;	// 고정단이 좌측&지그재그 : TRUE, 우측 : FALSE

	long nSzPos = 0;
	long n	= 0;
	for(n=0; n<pBridge->m_nSlabTensionSize; n++)
	{
		double	dL	=	pBridge->m_SlabTension[n].m_dL;
		double	dR	=	pBridge->m_SlabTension[n].m_dR;
		double	dH	=	pBridge->m_SlabTension[n].m_dH;
		long	nT	=	pBridge->m_SlabTension[n].m_nType;

		dDistTotal	+=	dL;
		
		if(n==0)	ptCen1	=	ptStt;	
		else		ptCen1	=	ptCen2;

		ptCen2	=	CDPoint(ptStt.x + dDistTotal, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal) - pBridge->m_dThickPave - dH);
		ptCen3	=	CDPoint(ptStt.x + dDistTotal + pBridge->m_SlabTension[n+1].m_dL, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal+pBridge->m_SlabTension[n + 1].m_dL) - pBridge->m_dThickPave - pBridge->m_SlabTension[n + 1].m_dH);
		
		if(n==pBridge->m_nSlabTensionSize - 1)
			ptCen3	=	ptEnd;

		if(bInput) pDom->SetLineColor(1);
		
		CDPoint vDir1	=	(ptCen1 - ptCen2).Unit();
		CDPoint vDir2	=	(ptCen3 - ptCen2).Unit();
		CDPoint vDir3	=	(vDir1 + vDir2).Unit();
		
		//실제 반지름 구하기 - Start
		CDPoint vDir4	= vDir3.BetweenAngle(vDir1);
		double	dRealV	= (360 - vDir4.GetAngleDegree()) - 90;
		
		dRealV = ToRadian(dRealV);
		
		double dH1		= tan(dRealV)*dR;
		double dRealR	= sqrt(dH1*dH1 + dR*dR);
		
		//실제 반지름 구하기 - End
		ptCenter	=	ptCen2 + vDir3 * dRealR;	
		
		//변곡점 시점부분
		if(n==0)
		{	
			if(GetTangentCircle(ptStt, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				
				dX1	=	bLeftFix ? pBridge->m_dLengthSecMove : pBridge->m_dLengthSecMoveEnd;
				dX2 =	xyMatch1.x - ptStt.x;
				dY2 =	xyMatch1.y - ptStt.y;

				dY1 = (dX1 / dX2) * dY2;

				ptSttUnit.x	=	ptStt.x + dX1;
				ptSttUnit.y =	ptStt.y + dY1;

				if(bLeftFix)
					ptSttTemp	=	ptSttUnit;
				else
				{
					ptSttTemp.x	=	ptSttUnit.x+140;
					ptSttTemp.y	=	ptSttUnit.y+140*(xyMatch1-ptSttUnit).Unit().y;
				}
				
				pDom->LineTo(ptSttTemp, xyMatch1);
				
				if(bInput) pDom->SetLineColor(4);

				if(bLeftFix)	DrawTensionBarStartUnit(pDom, ptSttUnit, TRUE);	// 고정단
				else			DrawTensionBarEndUnit(pDom, ptSttUnit, TRUE);	// 긴장단
												
				//덕트까지 그리기..
				double dLengthDuct = bLeftFix  ? pBridge->m_dLengthDuctOffset : 0;

				dX1	=	dLengthDuct;
				dX2 =	xyMatch1.x - ptSttUnit.x;
				dY2 =	xyMatch1.y - ptSttUnit.y;
				dY1 =	(dX1 / dX2) * dY2;
				
				if(bInput) pDom->SetLineColor(1);

				pDom->LineTo(ptSttTemp.x, ptSttTemp.y + dOffsetL1, ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 + dOffsetL1);
				pDom->LineTo(ptSttTemp.x, ptSttTemp.y - dOffsetL1, ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 - dOffsetL1);
				
				if(bInput) pDom->SetLineColor(4);

				pDom->LineTo(ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 + dOffsetL2, ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1-dOffsetL2);
				pDom->LineTo(ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 + dOffsetL2, xyMatch1.x, xyMatch1.y + dOffsetL2);
				pDom->LineTo(ptSttTemp.x + dLengthDuct, ptSttTemp.y + dY1 - dOffsetL2, xyMatch1.x, xyMatch1.y - dOffsetL2);

				///////////////////////////////////////////////////////////////////////////////////
				// Postioin Setting
				szArr.InsertAt(0, szPos[nSzPos++]);
				xyPoint1.x = ptSttTemp.x;
				xyPoint1.y = ptSttTemp.y;
				xyArr.Add(xyPoint1);


				szArr.InsertAt(0, szPos[nSzPos++]);
				xyPoint2.x = ptSttTemp.x + dLengthDuct;
				xyPoint2.y = ptSttTemp.y + dY1;
				xyArr.Add(xyPoint2);

				szArr.InsertAt(0, szPos[nSzPos++]);
				xyPoint1 = xyPoint2;
				xyPoint2 = xyMatch1;
				xyArr.Add(xyPoint2);
				///////////////////////////////////////////////////////////////////////////////////
			}
		}
		else
		{
			//직선 그리기
			if(GetTangentCircle(xyMatch2, ptCenter, dR, xyResult1, xyResult2)==2)
			{
				if(nT==0)
					xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				else
					xyMatch1 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;

				if(bInput) pDom->SetLineColor(1);

				pDom->LineTo(xyMatch1, xyMatch2);

				if(bInput) pDom->SetLineColor(4);				
				
				pDom->LineTo(xyMatch2.x, xyMatch2.y + dOffsetL2, xyMatch1.x, xyMatch1.y + dOffsetL2);
				pDom->LineTo(xyMatch2.x, xyMatch2.y - dOffsetL2, xyMatch1.x, xyMatch1.y - dOffsetL2);

				///////////////////////////////////////////////////////////////////////////////////
				// Postioin Setting
				szArr.InsertAt(0, szPos[nSzPos++]);
				xyPoint1 = xyMatch1;
				xyPoint2 = xyMatch2;
				xyArr.Add(xyPoint1);
				///////////////////////////////////////////////////////////////////////////////////
			}			
		}

		if(n!=pBridge->m_nSlabTensionSize)
		{

			//곡선 그리기
			double	dLength =	~(xyMatch1-ptCen2);
			CDPoint	vAng1	=	(xyMatch1-ptCen2).Unit();
			
			vAng1.MirrorHorz();

			GetTangentCircle(ptCen3, ptCenter, dR, xyResult1, xyResult2);

			if(nT == 0)
				xyMatch2	=	(xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
			else
				xyMatch2	=	(xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
			
			double	dAngle1	=	(xyMatch1 - ptCenter).Unit().GetAngleDegree();
			double	dAngle2	=	(xyMatch2 - ptCenter).Unit().GetAngleDegree() - dAngle1;
			
			if(bInput) pDom->SetLineColor(2);
			
			pDom->Arc(ptCenter, dR, dAngle1, dAngle2);

			if(bInput) pDom->SetLineColor(4);
			
			pDom->Arc(ptCenter.x, ptCenter.y + dOffsetL2, dR, dAngle1, dAngle2);
			pDom->Arc(ptCenter.x, ptCenter.y - dOffsetL2, dR, dAngle1, dAngle2);

			///////////////////////////////////////////////////////////////////////////////////
			// Postioin Setting
			szArr.InsertAt(0, szPos[nSzPos++]);
			xyPoint1 = xyMatch1;
			xyPoint2 = xyMatch2;
			xyArr.Add(xyPoint2);
			///////////////////////////////////////////////////////////////////////////////////
		}

		if(n == pBridge->m_nSlabTensionSize-1)
		{
			dX1	=	bLeftFix ? pBridge->m_dLengthSecMoveEnd : pBridge->m_dLengthSecMove;
			dX2 =	xyMatch2.x - ptEnd.x;
			dY2 =	xyMatch2.y - ptEnd.y;
			
			dY1 =	(dX1 / dX2) * dY2;
			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;

			//마지막 부분
			if(bLeftFix)
				ptEndTemp	=	ptEndUnit;
			else
			{
				ptEndTemp.x	=	ptEndUnit.x - pBridge->m_dLengthDuctOffset;
				ptEndTemp.y	=	ptEndUnit.y + pBridge->m_dLengthDuctOffset * (xyMatch2 - ptEndUnit).Unit().y;
			} 
			
			if(bLeftFix)	DrawTensionBarEndUnit(pDom,		ptEndUnit, FALSE);	// 긴장단
			else			DrawTensionBarStartUnit(pDom,	ptEndUnit, FALSE);	// 고정단
			
			//140띄어서 마무리...	
			
			dX1	=	bLeftFix ? pBridge->m_dLengthSecMoveEnd + 140 : pBridge->m_dLengthSecMove;
			dX2	=	xyMatch2.x - ptEnd.x;
			dY2	=	xyMatch2.y - ptEnd.y;
			dY1	=	(dX1 / dX2) * dY2;
			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;
			
			if(bInput) pDom->SetLineColor(1);

			pDom->LineTo(xyMatch2, ptEndUnit);
			
			if(!bLeftFix)
			{
				pDom->LineTo(ptEndTemp.x, ptEndTemp.y + dOffsetL1, ptEndUnit.x, ptEndUnit.y + dOffsetL1);
				pDom->LineTo(ptEndTemp.x, ptEndTemp.y - dOffsetL1, ptEndUnit.x, ptEndUnit.y - dOffsetL1);
				if(bInput) pDom->SetLineColor(4);
				pDom->LineTo(ptEndTemp.x, ptEndTemp.y + dOffsetL2, ptEndTemp.x, ptEndTemp.y - dOffsetL2);
			}

			if(bInput) pDom->SetLineColor(4);
			
			pDom->LineTo(xyMatch2.x, xyMatch2.y + dOffsetL2, ptEndTemp.x, ptEndTemp.y + dOffsetL2);
			pDom->LineTo(xyMatch2.x, xyMatch2.y - dOffsetL2, ptEndTemp.x, ptEndTemp.y - dOffsetL2);

			///////////////////////////////////////////////////////////////////////////////////
			// Postioin Setting
			szArr.InsertAt(0, szPos[nSzPos++]);
			xyPoint1 = xyMatch2;
			xyPoint2 = ptEndUnit;
			xyArr.Add(xyPoint2);
			///////////////////////////////////////////////////////////////////////////////////			
		}
	}

	// Position Drawing
	pDom->SetTextHeight(200);
	for (n=0; n<xyArr.GetSize(); n++)
	{
		CDPoint xyPos = xyArr.GetAt(n);
		CString cs = szArr.GetAt(n);
		pDom->SetTextColor(3);
		pDom->TextOut(xyPos.x, xyPos.y-pDom->GetTextHeight()*2, cs);	
	}

	// Position Angle
	for (n=0; n<xyArr.GetSize()-1; n++)
	{
		CDPoint xyPos = (xyArr.GetAt(n+1) - xyArr.GetAt(n)).Unit();
		double dAng = (xyPos.GetAngleRadian()>ConstPi/2)? ConstPi*2-xyPos.GetAngleRadian() : xyPos.GetAngleRadian();
		AngArr.Add(dAng);
	}
}

void CAPlateDrawDanmyun::DrawTensionBarStartUnit(CDomyun *pDom, CDPoint ptStt, BOOL bLeft)
{
	double dH1 = 70;
	double dH2 = 15;
	double dH3 = 5;
	double dW1 = 20;
	double dW2 = 40;
	double dW3 = 10;
	
	CDPoint PT = ptStt;

	CDomyun Dom(pDom);
	
	Dom.Rectangle(PT.x, PT.y+dH1, PT.x-dW1, PT.y-dH1);
	Dom.Rectangle(PT.x-dW1, PT.y+dH2, PT.x-dW1-dW2, PT.y-dH2);
	Dom.Rectangle(PT.x-dW1-dW2, PT.y+dH3, PT.x-dW1-dW2-dW3, PT.y-dH3);
	
	Dom.LineTo(PT.x-dW1-dW2-(dW3/2), PT.y, PT.x , PT.y);
	//Dom.LineTo(PT.x-20-40-7, PT.y+5, PT.x-20-40-10, PT.y+2.5);
	//Dom.LineTo(PT.x-20-40, PT.y+2.5, PT.x-20-40-3, PT.y-5);

	if(!bLeft)	Dom.Mirror(PT.x, TRUE);
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DrawTensionBarEndUnit(CDomyun *pDom, CDPoint ptStt, BOOL bLeft)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();	

	CDPoint pt[35];
	long n = 0;
	for (n = 0; n < 32; n++)//초기화..
		pt[n] = ptStt;
	
	pt[1].y = ptStt.y+33;
	pt[2].x = ptStt.x-140;
	pt[2].y = ptStt.y+17.5;
	pt[3].x = pt[2].x;
	pt[3].y = ptStt.y-17.5;
	pt[4].y = ptStt.y-33;

	pt[5].y = ptStt.y+62.5;
	pt[6].x = ptStt.x+14;
	pt[6].y = pt[5].y;
	pt[7].x = pt[6].x;
	pt[7].y = pt[6].y-4;
	pt[8].x = ptStt.x+4;
	pt[8].y = pt[7].y;
	pt[9].x = pt[8].x;
	pt[9].y = ptStt.y-62.5+4;
	pt[10].x = pt[6].x;
	pt[10].y = pt[9].y;
	pt[11].x = pt[10].x;
	pt[11].y = pt[10].y-4;
	pt[12].y = pt[11].y;

	pt[13].x = pt[8].x;
	pt[13].y = pt[8].y-4;
	pt[14].x = pt[13].x+6;
	pt[14].y = pt[13].y;
	pt[15].x = pt[6].x;
	pt[15].y = pt[14].y-4;

	pt[18].x = pt[9].x;
	pt[18].y = pt[9].y+4;

	pt[17].x = pt[18].x+6;
	pt[17].y = pt[18].y;
	pt[16].x = pt[11].x;
	pt[16].y = pt[17].y+4;
	
	pt[19].x = ptStt.x+4;

	pt[20].x = pt[6].x;
	pt[20].y = ptStt.y+43;
	pt[21].x = pt[20].x+47;
	pt[21].y = pt[20].y;
	pt[22].x = pt[21].x;
	pt[22].y = pt[21].y-10;

	pt[23].x = pt[20].x;
	pt[23].y = pt[22].y;
	pt[24].x = pt[22].x;
	pt[24].y = pt[22].y-4;
	pt[25].x = pt[24].x+4;
	pt[25].y = pt[24].y-4;
	
	pt[26].x = pt[19].x+65;
	
	pt[32].x = pt[10].x;
	pt[32].y = ptStt.y-43;
	pt[31].x = pt[32].x+47;
	pt[31].y = pt[32].y;
	pt[30].x = pt[31].x;
	pt[30].y = pt[31].y+10;
	pt[29].x = pt[32].x;
	pt[29].y = pt[30].y;
	pt[28].x = pt[30].x;
	pt[28].y = pt[30].y+4;
	pt[27].x = pt[28].x+4;
	pt[27].y = pt[28].y+4;

	pt[33].x = pt[23].x+10;
	pt[33].y = pt[23].y;
	pt[34].x = pt[33].x;
	pt[34].y = pt[29].y;

	CDomyun Dom(pDom);

	Dom.MoveTo(pt[1]);
	for (n=1; n<=4; n++)
		Dom.LineTo(pt[n]);

	Dom.MoveTo(pt[12]);
	for (n=5; n<=12; n++)
		Dom.LineTo(pt[n]);

	
	Dom.LineTo(pt[13],pt[14]);
	Dom.Arc(pt[14].x, pt[15].y,4.0,0.0,90.0);
	Dom.LineTo(pt[15],pt[16]);
	Dom.Arc(pt[17].x, pt[16].y,4.0,0.0,-90.0);
	Dom.LineTo(pt[17],pt[18]);

	Dom.MoveTo(pt[23]);
	for (n=20; n<=23; n++)
		Dom.LineTo(pt[n]);
	Dom.LineTo(pt[22],pt[24]);

	Dom.Arc(pt[24].x, pt[25].y,4.0,0.0,90.0);
	Dom.LineTo(pt[25],pt[26]);
	Dom.LineTo(pt[26],pt[27]);
	Dom.Arc(pt[28].x, pt[27].y,4.0,0.0,-90.0);	

	Dom.MoveTo(pt[32]);
	for (n=29; n<=32; n++)
		Dom.LineTo(pt[n]);
	Dom.LineTo(pt[28],pt[30]);

	Dom.LineTo(pt[33],pt[24]);
	Dom.LineTo(pt[34],pt[28]);

	Dom.LineTo(pt[19].x, pt[19].y,ptStt.x + pBridge->m_dLengthSecMoveEnd, pt[19].y);

	if(bLeft) Dom.Mirror(ptStt.x, TRUE);
	*pDom << Dom;
}
void CAPlateDrawDanmyun::DimTensionBarOnLineInfo(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput)
{
	CPlateBridgeApp	*pBridge	=	m_pDataMng->GetBridge();	
	CARoadOptionStd	*pOptStd	=	m_pDataMng->GetOptionStd();

	CString szType;
	CDomyun Dom(pDom);

	if(bInput)	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	else		pOptStd->SetEnvType(&Dom, HCAD_DIML);

	if(pBridge->m_nSlabTensionSize==0) return;
	
	Dom.SetDimLevelDis(6);
	Dom.SetDimVLen(6);

	double m_dLHeight	=	pBridge->m_dHeightLeftTension;
	double m_dRHeight	=	pBridge->m_dHeightRightTension;

	CDPoint A[HDANLINESU];

	pBridge->GetXyPaveUpperDom(pBx, A);	

	double	dLDist	=	pBridge->GetSlabLeft()->m_dGirderCenter;
	double	dRDist	=	pBridge->GetSlabRight()->m_dGirderCenter;
	CDPoint	ptStt	=	CDPoint(A[0].x,pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist) - pBridge->m_dThickPave - m_dLHeight);	
	CDPoint ptEnd	=	CDPoint(A[pBridge->GetQtyHDanNode()].x,pBridge->GetElevationPaveActByDis(pBx->GetStation(), dRDist) - pBridge->m_dThickPave - m_dRHeight);	

	CDPoint xyResult1,	xyResult2, xyResult3, xyResult4;
	CDPoint ptCen1,		ptCen2,	ptCen3;
	CDPoint ptNext,		ptNext2;
	CDPoint ptCenter,	ptCenter2;
	CDPoint xyMatch1,	xyMatch2;		

	double			dDistTotal	=	0;	
	CCentSeparation *pCentSep	=	pBridge->GetCurGuardWall(CCentSeparation::LEFT);

	if(pCentSep==NULL)	return;

	double dEleDim = A[0].y-pBridge->m_dThickPave+pCentSep->GetHeight();

	Dom.DimMoveTo(ptStt.x, dEleDim,0);	

	for(long n=0; n<pBridge->m_nSlabTensionSize; n++)
	{
		double	dL	=	pBridge->m_SlabTension[n].m_dL;
		double	dR	=	pBridge->m_SlabTension[n].m_dR;
		double	dH	=	pBridge->m_SlabTension[n].m_dH;
		long	nT	=	pBridge->m_SlabTension[n].m_nType;

		dDistTotal += dL;

		if(n==0)	ptCen1	=	ptStt;	
		else		ptCen1	=	ptCen2;
		
		ptCen2	=	CDPoint(ptStt.x + dDistTotal, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal) - pBridge->m_dThickPave - dH);
		ptCen3	=	CDPoint(ptStt.x + dDistTotal + pBridge->m_SlabTension[n+1].m_dL, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal+pBridge->m_SlabTension[n + 1].m_dL) - pBridge->m_dThickPave - pBridge->m_SlabTension[n + 1].m_dH);
		
		if(n==pBridge->m_nSlabTensionSize - 1)
			ptCen3	=	ptEnd;
		
		CDPoint vDir1	=	(ptCen1 - ptCen2).Unit();
		CDPoint vDir2	=	(ptCen3 - ptCen2).Unit();
		CDPoint vDir3	=	(vDir1 + vDir2).Unit();
		
		//실제 반지름 구하기 - Start
		
		CDPoint vDir4	=	vDir3.BetweenAngle(vDir1);
		double	dAng	=	vDir4.GetAngleDegree();
		
		dAng = 360 - dAng;
		
		double dRealV = dAng - 90;
		
		dRealV = ToRadian(dRealV);
		
		double dH1		=	tan(dRealV)*dR;
		double dRealR	=	sqrt(dH1*dH1 + dR*dR);
		
		//End
		ptCenter	=	ptCen2 + vDir3 * dRealR;	
		if(n==0)
		{	
			if(GetTangentCircle(ptStt, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				
				Dom.DimLineToExtend( fabs(ptStt.x) -fabs(xyMatch1.x), 0, COMMA(fabs(ptStt.x) -fabs(xyMatch1.x)),"직선");
			}
		}			
		else
		{
			if(GetTangentCircle(xyMatch2, ptCenter, dR, xyResult1, xyResult2)==2)
			{
				if(nT==0)
					xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				else
					xyMatch1 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
		
				Dom.DimLineToExtend(xyMatch1.x - xyMatch2.x, 0, COMMA(xyMatch1.x - xyMatch2.x),"직선");
			}
		}
		
		if(n!=pBridge->m_nSlabTensionSize)
		{
			double	dLength =	~(xyMatch1-ptCen2);
			CDPoint	vAng1	=	(xyMatch1-ptCen2).Unit();
			
			vAng1.MirrorHorz();
			
			GetTangentCircle(ptCen3, ptCenter, dR, xyResult1, xyResult2);
			
			if(nT == 0)
				xyMatch2	=	(xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
			else
				xyMatch2	=	(xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
			
			double	dAngle1	=	(xyMatch1 - ptCenter).Unit().GetAngleDegree();
			double	dAngle2	=	(xyMatch2 - ptCenter).Unit().GetAngleDegree() - dAngle1;	

			szType.Format("R=%.1f",toM(dR));
			Dom.DimLineToExtend(xyMatch2.x - xyMatch1.x, 0, COMMA(xyMatch2.x - xyMatch1.x),szType);
		
		}
		if(n== pBridge->m_nSlabTensionSize-1)
		{
			Dom.DimLineToExtend(ptEnd.x - xyMatch2.x, 0, COMMA(ptEnd.x - xyMatch2.x),"직선");				
		}
	
	}
	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimTensionBar(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bInput)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);

	if(bInput)	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	else		pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double dLeftX   = pBridge->GetSlabLeft()->m_dGirderCenter;
	double dRightX  = pBridge->GetSlabRight()->m_dGirderCenter;

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pBridge->GetXyPaveUpperDom(pBx, A);

	Dom.SetDimLevelDis(6);
	// 슬래브 좌우측, 두께및 텐던위치 표시
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(A[0].x, A[0].y-pBridge->m_dThickPave,0);		
	if(bInput)
		Dom.DimLineToExtend(-pBridge->m_dHeightLeftTension, 0, "H1", COMMA(pBridge->m_dHeightLeftTension));
	else
		Dom.DimLineToExtend(-pBridge->m_dHeightLeftTension, 0, "", COMMA(pBridge->m_dHeightLeftTension));

	Dom.DimMoveTo(A[0].x, A[0].y-pBridge->m_dThickPave,1);		
	Dom.DimLineTo(-pBridge->m_dThickSlabLeft, 1, COMMA(pBridge->m_dThickSlabLeft));	

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(A[pBridge->GetQtyHDanNode()].x, A[pBridge->GetQtyHDanNode()].y-pBridge->m_dThickPave,0);		
	if(bInput)
		Dom.DimLineToExtend(-pBridge->m_dHeightRightTension, 0, "H2", COMMA(pBridge->m_dHeightRightTension));
	else
		Dom.DimLineToExtend(-pBridge->m_dHeightRightTension, 0, "", COMMA(pBridge->m_dHeightRightTension));

	Dom.DimMoveTo(A[pBridge->GetQtyHDanNode()].x, A[pBridge->GetQtyHDanNode()].y-pBridge->m_dThickPave,1);		
	Dom.DimLineToExtend(-pBridge->m_dThickSlabRight, 1, "", COMMA(pBridge->m_dThickSlabRight));	
	
	double dExo = Dom.GetDimExo();
	Dom.SetDimExo(0);

	CString szStr;
	double dLen, dTotalLen=0;	
	Dom.SetDimObq(10);
	long n = 0;
	for(n=0; n<pBridge->m_nSlabTensionSize; n++)	
	{
		dLen = pBridge->m_SlabTension[n].m_dL;
		Dom.DimMoveTo(dLeftX + dTotalLen + dLen, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLeftX + dTotalLen + dLen) - pBridge->m_dThickPave,0);
		Dom.DimLineTo(-pBridge->m_SlabTension[n].m_dH,0,COMMA(pBridge->m_SlabTension[n].m_dH));
		dTotalLen += dLen;
	}

	Dom.SetDimObq(0);
	Dom.SetDimExo(dExo);
	// 슬래브 상단 텐던배치 표시
	CCentSeparation *pCentSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
	if(pCentSep==NULL) return;
	double dEleDim = A[0].y-pBridge->m_dThickPave+pCentSep->GetHeight();

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dLeftX, dEleDim, 1);		
	
	dTotalLen=0;
	for(n=0; n<pBridge->m_nSlabTensionSize; n++)	
	{
		szStr.Format("변곡점%d",n+1);
		_TENSION Tension = pBridge->m_SlabTension[n];
		dLen = Tension.m_dL;
		Dom.DimLineToExtend(dLen,1,COMMA(dLen),szStr);
		dTotalLen += dLen;
	}
	Dom.DimLineTo(dRightX-dLeftX-dTotalLen,1,COMMA(dRightX-dLeftX-dTotalLen));

	Dom.DimMoveTo(dLeftX, dEleDim, 2);
	Dom.DimLineTo(dRightX-dLeftX,2,COMMA(dRightX-dLeftX));	

	*pDom << Dom;
}

//방호벽 그리기...
void CAPlateDrawDanmyun::DstDrawGuardFence(CDomyun *pDomP, 
				CPlateBasicIndex *pBx, long nHDan, BOOL bActual/*FALSE*/)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDomP);
	pOptStd->SetEnvType(pDomP,HCAD_CONC);
	CDPoint A[9];
	pBridge->GetXyGuardFenceDom(pBx,nHDan,A,bActual);

	for (long n=0; n<8; n++)
	{
		if(A[n] != A[n+1])
			Dom.LineTo(A[n],A[n+1]);
	}

	*pDomP << Dom;
	//이후는 철도교 추가시...060418
}

void CAPlateDrawDanmyun::DstDimGuardFenceTop(CDomyun *pDomP, 
									CPlateBasicIndex *pBx, long nHDan, BOOL bInput)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	if(!pBridge->IsTypeGuardFenceDom(nHDan))	
		return;
	
	CDomyun *pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom,HCAD_DIML);

	CDPoint A[9];
	pBridge->GetXyGuardFenceDom(pBx, nHDan, A);	
	CDPoint xy = pBridge->GetXyHDanLeftDom(pBx, nHDan);

	pDom->SetDirection("TOP");
		
	double D1 = A[1].x - A[0].x;
	double W1 = A[3].x - A[2].x;
	double W2 = A[4].x - A[3].x;
	double W3 = A[5].x - A[4].x;
	double W4 = A[6].x - A[5].x;
	double W5 = A[7].x - A[6].x;

	if(!bInput) 
	{
		pDom->DimMoveTo(A[1].x, A[4].y, 0);
		pDom->DimLineTo(W1,0,COMMA(fabs(W1)));
		pDom->DimLineTo(W2,0,COMMA(fabs(W2)));
		pDom->DimLineTo(W3,0,COMMA(fabs(W3)));
		pDom->DimLineTo(W4,0,COMMA(fabs(W4)));
		pDom->DimLineTo(W5,0,COMMA(fabs(W5)));
		pDom->DimMoveTo(A[1].x, A[4].y, 1);
		pDom->DimLineTo(W1+W2+W3+W4+W5, 1, COMMA(fabs(W1+W2+W3+W4+W5)));

	}
	else 
	{
		double lenHDan = pBridge->GetLengthHDanDom(pBx, nHDan);
		pDom->LineTo(A[0].x, A[0].y, A[0].x + lenHDan, A[0].y);
		pDom->DimMoveTo(A[0].x, A[4].y, 0);
		pDom->DimLineToExtend(D1,0,"D1",COMMA(D1));
		pDom->DimLineToExtend(W1,0,"W1",COMMA(W1));
		pDom->DimLineToExtend(W2,0,"W2",COMMA(W2));
		pDom->DimLineToExtend(W3,0,"W3",COMMA(W3));
		pDom->DimLineToExtend(W4,0,"W4",COMMA(W4));
		pDom->DimLineToExtend(W5,0,"W5",COMMA(W5));
		
		pDom->DimMoveTo(A[0].x, A[4].y, 1);
		pDom->DimLineTo(D1+W1+W2+W3+W4+W5,1,COMMA(fabs(D1+W1+W2+W3+W4+W5)));
		pDom->DimMoveTo(A[0].x, A[4].y, 2);
		pDom->DimLineToExtend(lenHDan,2,COMMA(lenHDan),"횡단구간길이");
	}

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawDanmyun::DstDimGuardFenceSide(CDomyun *pDomP, CPlateBasicIndex *pBx, long nHDan, BOOL bInput)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	if(!pBridge->IsTypeGuardFenceDom(nHDan))	
		return;

	CDomyun *pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom,HCAD_DIML);

	CDPoint A[9];
	pBridge->GetXyGuardFenceDom(pBx, nHDan, A);

	pDom->SetDirection("LEFT");
	
	double H1 = A[2].y - A[1].y;
	double H2 = A[3].y - A[2].y;
	double H3 = A[4].y - A[3].y;

	if(!bInput) 
	{
		pDom->DimMoveTo(A[1].x, A[1].y, 0);
		pDom->DimLineTo(H1,0,COMMA(fabs(H1)));
		pDom->DimLineTo(H2,0,COMMA(fabs(H2)));
		pDom->DimLineTo(H3,0,COMMA(fabs(H3)));
		pDom->DimMoveTo(A[1].x, A[1].y, 1);
		pDom->DimLineTo(H1+H2+H3, 1, COMMA(H1+H2+H3));

	}
	else 
	{
		pDom->DimMoveTo(A[1].x, A[1].y, 0);
		pDom->DimLineToExtend(H1,0,"H1",COMMA(H1));
		pDom->DimLineToExtend(H2,0,"H2",COMMA(H2));
		pDom->DimLineToExtend(H3,0,"H3",COMMA(H3));
		
		pDom->DimMoveTo(A[1].x, A[1].y, 1);
		pDom->DimLineTo(H1+H2+H3,1,COMMA(H1+H2+H3));
	}

	pDom->SetDirection("RIGHT");
	if(!bInput) 
	{
		pDom->DimMoveTo(A[8].x, A[0].y, 0);
		pDom->DimLineTo(H1,0,COMMA(fabs(H1)));
		pDom->DimLineTo(H2,0,COMMA(fabs(H2)));
		pDom->DimLineTo(H3,0,COMMA(fabs(H3)));
		pDom->DimMoveTo(A[8].x, A[0].y, 1);
		pDom->DimLineTo(H1+H2+H3, 1, COMMA(H1+H2+H3));

	}
	else 
	{
		pDom->DimMoveTo(A[8].x, A[0].y, 0);
		pDom->DimLineToExtend(H1,0,"H1",COMMA(H1));
		pDom->DimLineToExtend(H2,0,"H2",COMMA(H2));
		pDom->DimLineToExtend(H3,0,"H3",COMMA(H3));
		
		pDom->DimMoveTo(A[8].x, A[0].y, 1);
		pDom->DimLineTo(H1+H2+H3,1,COMMA(H1+H2+H3));
	}

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawDanmyun::DimGirderAndSlab(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir		= pBx->GetGirder();

	CDomyun	Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDimExo(0);
	Dom.SetStartPointExtend(TRUE);
	Dom.SetDimObq(45);
	CDPoint sp(0,0);
	long	nCol		= 0;
	double	dSlabLeftX	= pGir->GetDisCenterLineToGirderDom(pBx);
	double	dHTaper		= 0;
	if(pBx->IsDanbuJijumStt())		dHTaper = pDB->m_dHeightTaperStt;
	else if(pBx->IsDanbuJijumEnd())	dHTaper = pDB->m_dHeightTaperEnd;

	double	dGirEL		= pGir->GetElevationDom(pBx) - dHTaper;
	double	dHeightGir	= pGir->GetHeightGirder(pBx, TRUE);
	double	dTu			= pGir->GetThickJewon(G_F_U, pBx);
	double	dTl			= pGir->GetThickJewon(G_F_L, pBx);
	double	dPaveThick	= pDB->m_dThickPave;
	double	dSlabThick	= pGir->GetThickSlab(pBx, 0);

	// 시작점	
	sp.x = dSlabLeftX;
	sp.y = dGirEL - (dHeightGir + dTl) - (pDB->m_BindConc.m_dHeight - pDB->m_BindConc.m_dDeep - pDB->m_BindConc.m_dDis+dTu);

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(sp.x, sp.y, nCol, COMMA(dTl));
	Dom.DimLineTo(dTl, nCol, "");
	Dom.DimLineTo(dHeightGir-(pDB->m_BindConc.m_dDis-dTu), nCol, COMMA(dHeightGir-(pDB->m_BindConc.m_dDis-dTu)));
	Dom.DimLineToExtend(pDB->m_BindConc.m_dHeight, nCol, "", COMMA(pDB->m_BindConc.m_dHeight));
	Dom.DimLineTo(dSlabThick, nCol, COMMA(dSlabThick));
	Dom.DimLineToOver(dPaveThick, nCol, COMMA(dPaveThick));

	*pDom << Dom;
}

void CAPlateDrawDanmyun::DimVStiffDet(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bStiff)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir    = pBx->GetGirder();
	CVStiff			*pVStiff  = pBx->GetSection() ? pBx->GetSection()->GetVStiff() : NULL;
	CJackupStiff	*pJStiff  = pBx->GetJackupStiff();

	if(bStiff && (!pVStiff && !pJStiff)) return;

	CDPoint B[10];
	CDPoint vX(1,0), vY(0,1);

 	pGir->GetHunchXyDom(pBx, B);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	double dHGir         = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dWidthGirU    = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWidthGirD    = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
	double dWVStiffL     = pVStiff ? pVStiff->m_dWidth[0] : 0;
	double dWVStiffR     = pVStiff ? pVStiff->m_dWidth[1] : 0;
	double dTFlangeU     = pGir->GetThickFactByStation(pBx->GetStation(), G_F_U);
	double dTFlangeD     = pGir->GetThickFactByStation(pBx->GetStation(), G_F_L);
	double dTWeb         = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	long   nCol          = 0;
	if(pJStiff)
	{
		dWVStiffL = pJStiff->m_vJewon[0].x;
		dWVStiffR = pJStiff->m_vJewon[1].x;
	}

	CDPoint xyStt;

	// 거더 수직보강재 제원 ========================================
	Dom.SetDirection("BOTTOM");
	CVector vJ(0,0,0);
	if(bStiff && pVStiff)
	{
		vJ = pGir->GetJewonTotal(V_ST, pBx, 0, 0);//우측수직보강재

		if(dWVStiffR > 0 && vJ.z > 0)
		{
			CString szJewon = pOptStd->GetJewonString(0, vJ.x, vJ.y, vJ.z);
			Dom.CircleArrow((B[1].x+B[2].x)/2+dWVStiffR*0.8, B[1].y-dHGir+dWVStiffR*0.3, dWVStiffR*0.3+Dom.Always(7), Dom.GetTextWidth(szJewon), 7, szJewon, "");
		}
	}

	//가로보제원	========================================
	xyStt = (B[1]+B[2])/2 + vX*dWidthGirD/2 - vY*(dTFlangeD+dHGir);	
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyStt.x, xyStt.y ,nCol);
	Dom.DimLineTo(dTFlangeD, nCol, COMMA(dTFlangeD));
	if(pVStiff)
	{
		Dom.DimLineToExtend(pVStiff->m_dH[1][1], nCol, "", COMMA(pVStiff->m_dH[1][1]));
		Dom.DimLineTo(pVStiff->GetVStiffHeight(FALSE), nCol, COMMA(pVStiff->GetVStiffHeight(FALSE)));
		Dom.DimLineToExtend(pVStiff->m_dH[1][0], nCol, "", COMMA(pVStiff->m_dH[1][0]));		
	}
	else if(pJStiff)
	{
		Dom.DimLineTo(pJStiff->m_dVL, nCol, COMMA(pJStiff->m_dVL));
		Dom.DimLineTo(dHGir-pJStiff->m_dVL, nCol, COMMA(dHGir-pJStiff->m_dVL));
	}
	else
		Dom.DimLineTo(dHGir, nCol, COMMA(dHGir));
	Dom.DimLineTo(dTFlangeU, nCol, COMMA(dTFlangeU));

	xyStt = (B[1]+B[2])/2 - vX*dWidthGirU/2;
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "", TRUE);
	Dom.DimLineTo(dWidthGirU, 0, COMMA(dWidthGirU));

	//Bottom Dimension
	xyStt = (B[1]+B[2])/2 - vX*dWidthGirD/2 - vY*(dTFlangeD+dHGir);	
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "", TRUE);
	Dom.DimLineTo(dWidthGirD, 0, COMMA(dWidthGirD));

	// 상부플랜지와의 용접
	CString str = "";
	if(bStiff)
	{
		CDPoint xyWeld = (B[1]+B[2])/2 + vX*(dTWeb/2);
		if(pVStiff && pVStiff->GetVStiffHeight(FALSE) == dHGir)
		{
			Dom.WeldByValue(xyWeld.x, xyWeld.y, 6, 0, FALSE, FALSE);

			double dArcR = dTFlangeU <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
			str.Format("R=%d", (long)dArcR);
			CDPoint xyGisi = B[1]+vX*pGir->GetLengthSharpOnBx(pBx, TRUE) + CDPoint(-1,-1).Unit()*dArcR; 
			Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(4), -Dom.Always(8), 5, str);
		}
		
		// 하부플랜지와의 용접
		xyWeld = (B[1]+B[2])/2 + vX*(dTWeb/2) - vY*dHGir; 
		if(pVStiff || pJStiff)
		{
			Dom.WeldByValue(xyWeld.x, xyWeld.y, 6, 0, FALSE, TRUE);

			double dArcR = dTFlangeD <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
			str.Format("R=%d", (long)dArcR);
			CDPoint xyGisi = (B[1]+B[2])/2 + vX*(-dTWeb/2) - vY*dHGir + CDPoint(-1,1).Unit()*dArcR; 
			Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(4), -Dom.Always(8), 3, str);
		}
	}	

	*pDom << Dom;	
}
