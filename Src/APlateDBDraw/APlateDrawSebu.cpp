// APlateDrawSebu.cpp: implementation of the CAPlateDrawSebu class.
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

CAPlateDrawSebu::CAPlateDrawSebu(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawSebu::~CAPlateDrawSebu()
{

}

// 솔플레이트 상세 (일반도,세부상세)
void CAPlateDrawSebu::AddSolePlateDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bGen)
{
	CDomyun		Dom(pDomP);	

	// DRAWING 중심 : 솔플레이트 중심
	DrawSolePlatePlanDetail(&Dom, pBx);
	DimSolePlatePlan(&Dom, pBx,bGen);
	Dom.SetCalcExtRect();
	double moveY = Dom.GetExtHeight()/2;
	*pDomP << Dom;

	// DRAWING 중심 : 솔플레이트 중심하단
	DrawSolePlateSideDetail(&Dom, pBx);
	Dom.SetCalcExtRect();
	moveY += Dom.GetExtHeight();
	DimSolePlateSide(&Dom, pBx,bGen);
	Dom.Move(0,-(moveY+Dom.Always(5)));		
	*pDomP << Dom;		
}

// DRAWING 중심 : 솔플레이트 중심
void CAPlateDrawSebu::DrawSolePlatePlanDetail(CDomyun *pDomP, CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBx->GetGirder();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CShoe *pShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx));

	double W = pShoe->m_dW_SolePlate;
	double W2 = pShoe->m_dW2_SolePlate;
	double W3 = pShoe->m_dW3_SolePlate;
	double H = pShoe->m_dH_SolePlate;
	double H2 = pShoe->m_dH2_SolePlate;
	double H3 = pShoe->m_dH3_SolePlate;
	long   nDir = pShoe->m_cDirectionShoe;
	double HoleSize = pShoe->m_dDia_SolePlateHole;

	CDomyun* pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom, HCAD_STLC);

	// SOLE PLATE
	if(nDir==4) 
	{
		W /= sqrt(2.0);
		W2 /= sqrt(2.0);
		H /= sqrt(2.0);
		H2 /= sqrt(2.0);
	}

	pDom->Rectangle(-W/2-W2,-H/2-H2,W/2+W2,H/2+H2);
	pDom->MoveTo(-W/2, H/2);
	pDom->LineTo( W/2, H/2);
	pDom->LineTo( W/2,-H/2);
	pDom->LineTo(-W/2,-H/2);
	pDom->LineTo(-W/2, H/2);
	if(nDir==4) pDom->Rotate(ConstPi/4,0,0);
	
	pDom->Circle(-W3/2,H3/2,HoleSize/2);
	pDom->Circle(+W3/2,H3/2,HoleSize/2);
	pDom->Circle(-W3/2,-H3/2,HoleSize/2);
	pDom->Circle(+W3/2,-H3/2,HoleSize/2);
	
	*pDomP << *pDom;
	delete pDom;
}


// DRAWING 중심 : 솔플레이트 중심하단
void CAPlateDrawSebu::DrawSolePlateSideDetail(CDomyun *pDomP, CPlateBasicIndex *pBx)
{
	CPlateGirderApp	*pGir = (CPlateGirderApp *)pBx->GetGirder();	
	CShoe *pShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx));
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double W = pShoe->m_dW_SolePlate;
	double W2 = pShoe->m_dW2_SolePlate;
	double T = pShoe->m_dThick_SolePlate;
	double slop = pGir->GetGiulJongDanShoe(pBx);

	W += W2 * 2;			

	double T1 = T - W/2*slop;
	double T2 = T + W/2*slop;

	if(pShoe->m_bLevelSolePlate)
	{
		T1 = T;
		T2 = T;
	}
	
	CDomyun* pDom = new CDomyun(pDomP);
	pDom->SetScaleDim(pDomP->GetScaleDim());
	pOptStd->SetEnvType(pDom, HCAD_STLC);

	// SOLE PLATE
	pDom->LineTo(-W/2, 0,  W/2,  0);
	pDom->LineTo(-W/2, 0, -W/2, T1);
	pDom->LineTo( W/2, 0,  W/2, T2);
	pDom->LineTo(-W/2,T1,  W/2, T2);	

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawSebu::DimSolePlatePlan(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bGen)
{		
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBx->GetGirder();		
	CShoe *pShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx));
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double W = pShoe->m_dW_SolePlate;
	double W2 = pShoe->m_dW2_SolePlate;
	double W3 = pShoe->m_dW3_SolePlate;
	double H = pShoe->m_dH_SolePlate;
	double H2 = pShoe->m_dH2_SolePlate;
	double H3 = pShoe->m_dH3_SolePlate;
	
	// DRAWING 중심 : 솔플레이트 중심
	CDomyun* pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom, HCAD_DIML);

	pDom->SetDirection("TOP");
	pDom->DimMoveTo(-W/2-W2,H/2+H2,0);
	if(bGen)
	{
		pDom->DimLineTo(W2,0,COMMA(W2));
		pDom->DimLineTo(W,0,COMMA(W));
		pDom->DimLineTo(W2,0,COMMA(W2));
	}
	else
	{
		pDom->DimLineTo(W2,0,"W2");
		pDom->DimLineTo(W,0,"W");
		pDom->DimLineTo(W2,0,"W2");
	}

	pDom->DimMoveTo(-W3/2,H3/2,0);
	if(bGen)	
		pDom->DimLineTo(W3,0,COMMA(W3));
	else
		pDom->DimLineTo(W3,0,"W3");	

	pDom->SetDirection("RIGHT");
	pDom->DimMoveTo(W/2+W2,-H/2-H2,0);
	
	if(bGen)
	{
		pDom->DimLineTo(H2,0,COMMA(H2));
		pDom->DimLineTo(H,0,COMMA(H));
		pDom->DimLineTo(H2,0,COMMA(H2));
	}
	else
	{
		pDom->DimLineTo(H2,0,"H2");
		pDom->DimLineTo(H,0,"H");
		pDom->DimLineTo(H2,0,"H2");
	}	
	
	pDom->DimMoveTo(W3/2,-H3/2,0);
	if(bGen)	
		pDom->DimLineTo(H3,0,COMMA(H3));
	else
		pDom->DimLineTo(H3,0,"H3");	

	pDom->SetDirection("TOP");
	pDom->GiSiArrow(-W/4, 0, W/2, 0, 0,"교축방향");
	
//	pDom->GiSiArrowExtend(-W3/2-HoleSize/2, H3/2, 50, -300, 4,"Hole Size","Ф = "+COMMA(HoleSize));

	*pDomP << *pDom;
	delete pDom;

}

void CAPlateDrawSebu::DimSolePlateSide(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bGen)
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBx->GetGirder();	
	CShoe *pShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx));
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double W = pShoe->m_dW_SolePlate;
	double W2 = pShoe->m_dW2_SolePlate;
	double T = pShoe->m_dThick_SolePlate;

	W += W2 * 2;

	// DRAWING 중심 : 솔플레이트 중심하단
	CDomyun* pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom, HCAD_DIML);
	double slop = pGir->GetGiulJongDanShoe(pBx);

	double T1 = T - W/2*slop;
	double T2 = T + W/2*slop;

	if(pShoe->m_bLevelSolePlate)
	{
		T1 = T;
		T2 = T;
	}	

	pDom->SetDirection("BOTTOM");
	pDom->DimMoveTo(-W/2,0,0);
	//if(bGen)
	pDom->DimLineTo(W,0,COMMA(W));
	//else
	//	pDom->DimLineTo(W,0,"W");

	pDom->SetDirection("LEFT");
	pDom->DimMoveTo(-W/2,0,0);
	if(bGen)
		pDom->DimLineToOver( T1,0,COMMA(RoundUp(T1,0)));
	else
		pDom->DimLineToOver( T1,0,"T1");

	pDom->SetDirection("RIGHT");
	pDom->DimMoveTo( W/2,0,0);
	if(bGen)
		pDom->DimLineToOver( T2,0,COMMA(RoundUp(T2,0)));
	else
		pDom->DimLineToOver( T2,0,"T2");

	pDom->SetDimExo(0);
	pDom->SetDimObq(-45);
	pDom->DimMoveTo(0,0,0);
	if(bGen)
		pDom->DimLineTo( T,0,COMMA(T));
	else
		pDom->DimLineTo( T,0,"T");
	*pDomP << *pDom;

	if(pShoe->m_bLevelSolePlate != TRUE)
	{	
		// 종경사 표시
		pDom->SetDirection("TOP");
		CString str;
		if(bGen)
			str.Format("S = %.3f%%",slop*100);
		else
			str = "S %";
		pDom->GiSiArrow(-W/4, T*1.5, W/2, 0, 0,str);
		pDom->Rotate(atan(slop),0,T*1.5);
	}

	*pDomP << *pDom;
	delete pDom;
}

///////////////////////////////////////////////////
// 수직보강재 부착상세 : B-TYPE
// DRAWING의 원점은 박스의 내공 우측하단 POINT
void CAPlateDrawSebu::AddVStiffDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX, double BY, double DimScale)
{	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	= (CPlateGirderApp	*)pBx->GetGirder();
	CSection		*pSec	= ((CPlateBasicIndex *)pBx)->GetSection();
	CVStiff			*pVStiff = pSec->GetVStiff();
	
	CDomyun *pDom = new CDomyun(pDomP);	
	pOptStd->SetEnvType(pDom, HCAD_STLC);
	
	double dHGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double W  = pVStiff->m_dWidth[0];
	double V  = pVStiff->m_dGap[0];	
	double W2 = pVStiff->m_dWidth[1];
	double V2 = pVStiff->m_dGap[1];	
	double H1L = pVStiff->GetVStiffHeight(TRUE, dHGir);
	double H1R = pVStiff->GetVStiffHeight(FALSE, dHGir);
	
	double LowerT = pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double WebT   = pBx->GetFactChain(G_W)->m_dFactChainThick;
	double dWidth = pGir->GetWidthOnStation(pBx->GetStation(),FALSE);
	double Len	= 500;	// 박스의 높이(폭) : 절단면까지

	// 하판, 복부판
    pDom->Rectangle(-dWidth/2, 0, dWidth/2, -LowerT);
	pDom->LineTo(-WebT/2, 0, -WebT/2, Len);
	pDom->LineTo( WebT/2, 0,  WebT/2, Len);

	double dHG_L = dHGir - H1L;
    
	// 수직보강재(좌측)
	if(W!=0) // #APLATE-662, 출력 조건을 개선한다.
	{
		if (H1L == dHGir) 
		{
			pDom->LineTo(-W-WebT/2, Len, -W-WebT/2,0);
			double scallopR = 35;
			pDom->Arc(-WebT/2,0,scallopR,180,-90);
		}
		else
		{						
			pDom->LineTo(-W-WebT/2,W-V+dHG_L, -W-WebT/2, Len);
			pDom->LineTo(-W-WebT/2,W-V+dHG_L, -V-WebT/2, (dHG_L));
			pDom->LineTo(-V-WebT/2, (dHG_L),   -WebT/2,  (dHG_L));
		}
	}
	// 수직보강재(우측)
	if(W2!=0) // #APLATE-662, 출력 조건을 개선한다.
	{
		if (H1R == dHGir) 
		{
			pDom->LineTo(W2+WebT/2,Len, W2+WebT/2,0);
			double scallopR = 35;
			pDom->Arc(WebT/2,0,scallopR,90,-90);
		}
		else
		{
			pDom->LineTo(W2+WebT/2, (dHGir-H1R)+W2-V2, W2+WebT/2, Len);
			pDom->LineTo(W2+WebT/2, (dHGir-H1R)+W2-V2, V2+WebT/2, (dHGir-H1R));
			pDom->LineTo(V2+WebT/2, (dHGir-H1R),    WebT/2,    (dHGir-H1R));
		}
	}
	pOptStd->SetEnvType(pDom, HCAD_CUTL);
	double dWMax = max(W,W2);
	pDom->CutLightning(-dWMax-WebT/2-pDom->Always(5), Len, +dWMax+WebT/2+pDom->Always(5), Len, FALSE);

	// 치수선 및 지시선
	pOptStd->SetEnvType(pDom, HCAD_DIML);
	DimVStiffDetail(pDom,pBx,BX,BY,DimScale);

	pDom->Move(BX,BY);
	*pDomP << *pDom;
	delete pDom;

}

void CAPlateDrawSebu::DimVStiffDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX, double BY, double DimScale)
{	
	CPlateGirderApp	*pGir	= (CPlateGirderApp	*)pBx->GetGirder();
	CSection		*pSec	= ((CPlateBasicIndex *)pBx)->GetSection();
	CVStiff			*pVStiff = pSec->GetVStiff();

	// B type 수직보강재  DATA
	double dHGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double W  = pVStiff->m_dWidth[0];
	double V  = pVStiff->m_dGap[0];	
	double W2 = pVStiff->m_dWidth[1];
	double V2 = pVStiff->m_dGap[1];	
	double H1L = pVStiff->GetVStiffHeight(TRUE, dHGir);
	double H1R = pVStiff->GetVStiffHeight(FALSE, dHGir);
	
	double WebT   = pBx->GetFactChain(G_W)->m_dFactChainThick;
	double Len	= 500;	// 박스의 높이(폭) : 절단면까지
	CString	str = _T("");

	// 치수선
	if(W!=0)
	{
		pDomP->SetDirection("TOP");
		pDomP->DimMoveTo(-W-WebT/2,Len,0);
		if(dHGir-H1L == 0)
		{
			double scallopR = 35;
			pDomP->DimLineTo(W-scallopR,0,COMMA(W-scallopR));
			pDomP->DimLineTo(scallopR,0,COMMA(scallopR));
		}
		else
		{
			pDomP->DimLineTo(W-V,0,COMMA(W-V));
			pDomP->DimLineToOver(V,0,COMMA(V));
		}
		if(dHGir-H1L > 0) 
		{
			pDomP->SetDirection("LEFT");
			pDomP->DimMoveTo(-W,0,0);
			pDomP->DimLineTo(dHGir-H1L,0,COMMA(dHGir-H1L));
			pDomP->DimLineTo(W-V,0,COMMA(W-V));
		}
		if(V>0 && (dHGir-H1L)>0) {
			pDomP->SetDirection("TOP");
			CDPoint xyCenCir = CDPoint(-V-WebT/2, (dHGir-H1L));	// 45도라는 가정
			double R = 1;//(W-V)*3;
			CDPoint xySttCir = xyCenCir+CDPoint(-1,1).Unit()*R;
			CDPoint xyEndCir = xyCenCir+CDPoint(-1,0).Unit()*R;
			pDomP->DimArcMoveTo(xyCenCir.x, xyCenCir.y, R, xySttCir.x, xySttCir.y, 0);
			pDomP->DimArcLineTo(xyEndCir.x, xyEndCir.y, 0, "45%%d");						
		}
	}

	if(W2!=0 )
	{
		pDomP->SetDirection("TOP");
		pDomP->DimMoveTo(WebT/2,Len,0);
		if(dHGir-H1R ==0)
		{
			double scallopR = 35;
			pDomP->DimLineTo(scallopR,0,COMMA(scallopR));
			pDomP->DimLineTo(W2-scallopR,0,COMMA(W2-scallopR));
		}
		else
		{
			pDomP->DimLineTo(V2,0,COMMA(V2));
			pDomP->DimLineTo(W2-V2,0,COMMA(W2-V2));
		}

		if(dHGir-H1R > 0) 
		{
			pDomP->SetDirection("RIGHT");
			pDomP->DimMoveTo(W2,0,0);
			pDomP->DimLineTo(dHGir-H1R,0,COMMA(dHGir-H1R));
			pDomP->DimLineTo(W2-V2,0,COMMA(W2-V2));
		}
		if(V2>0 && (dHGir-H1R)>0) {
			pDomP->SetDirection("TOP");
			CDPoint xyCenCir = CDPoint(V2+WebT/2, (dHGir-H1R));	// 45도라는 가정
			double R = 1;//(W2-V2)*3;
			CDPoint xySttCir = xyCenCir+CDPoint(1,1).Unit()*R;
			CDPoint xyEndCir = xyCenCir+CDPoint(1,0).Unit()*R;
			pDomP->DimArcMoveTo(xyCenCir.x, xyCenCir.y, R, xyEndCir.x, xyEndCir.y, 0);
			pDomP->DimArcLineTo(xySttCir.x, xySttCir.y, 0, "45%%d");							
		}
	}

	if(W!=0 && W2!=0)
	{
		str = "수직보강재";
		double strLen = pDomP->GetTextWidth(str);
		pDomP->GiSiArrowExt(-W, (dHGir-H1L)+W, strLen/2, -strLen, 3, str);
	}
	else
	{
		if(W!=0)
		{
			str = "수직보강재";
			double strLen = pDomP->GetTextWidth(str);
			pDomP->GiSiArrowExt(-W, (dHGir-H1L)+W, strLen/2, -strLen, 3, str);
		}
		if(W2!=0)
		{
			str = "수직보강재";
			double strLen = pDomP->GetTextWidth(str);
			pDomP->GiSiArrowExt(W, (dHGir-H1R)+W2, strLen/2, -strLen, 3, str);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 공장이음 용접상세
// T1 : 왼쪽 플랜지의 두께
// T2 : 오른쪽 플랜지의 두께
void CAPlateDrawSebu::AddFactDetail(CDomyun *pDomP,double T1,double T2, double Scale
						,long nUpDown ,double termW ,double termH, double termH2,double AngleV, double AngleX, BOOL bCenter, BOOL bShop, BOOL bWeb)
{
	//termW = 2, termH = 2, termH2 = 6, AngleV = 60, AngleX = 90//
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();
	
	CGlobarOption *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	double Xthick = pGlopt->GetDesignThickXWeldApply();

	CDomyun Dom(pDomP);
	
	if(min(T1,T2) > Xthick)
		if(!bWeb/*cWeldTypeName != 'C'*/)
			DrawFactDetail_XType(&Dom, T1, T2, Scale, nUpDown, termW, termH, termH2, AngleV, AngleX, bCenter, bShop);
		else
			DrawFactDetail_XType_CType(&Dom, T1, T2, Scale, nUpDown, termW, termH, termH2, AngleV, AngleX, bCenter, bShop);
	else
	{
		if(!bWeb/*cWeldTypeName != 'C'*/)
			DrawFactDetail_VType(&Dom, T1, T2, Scale, nUpDown, termW, termH, AngleV, bCenter, FALSE, bShop);
		else
			DrawFactDetail_VType_CType(&Dom, T1, T2, Scale, nUpDown, termW, termH, AngleV, bCenter, FALSE, bShop);
	}
	
	*pDomP << Dom;
}


/*                                                     (0)          (3)
                                                           ---------------------
                                           slop        |
											       | 
                 (2)         weldLen   xp      |   (7)
        ---------------------     *   (4) |      slopLen
                             |         |                                    maxT
                              |       |                    
		minT               (5) |     |  (6)                         
							    |   |
								 | |              (1)
 		--------------------------*---------------------------------------------
								(0,0)|
										|
										(8)	|
												|			(9)
												 -------------------------------
	   |----------shortLen--------|---------------------longLen-----------------|
*/

void CAPlateDrawSebu::DrawFactDetail_VType_CType(CDomyun *pDomP,double T1,double T2, double Scale
									, long nUpDown,double termW, double termH,double AngleV, BOOL bCenter, BOOL bDimDraw, BOOL bShop)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(Scale);
	
	// 기본 두께 확대
	long nS = 2;
	T1 *= nS;
	T2 *= nS;
	termW *= nS;
	termH *= nS;
	
	// DRAWING의 기준은 이음면의 중심하단,
	pOptStd->SetEnvType(pDom,HCAD_STLC);
	double minT = min(T1,T2);
	double maxT = max(T1,T2);
	
	// 용접될 부재의 길이
	double shortLen = 50 ;	
	double longLen = 125;
	if(T1==T2) longLen = 100;
	
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
	pDom->LineTo(-shortLen,0, termW/2,0);					// (1)
	pDom->LineTo(-shortLen,minT, -weldLen,minT);			// (2)
	pDom->LineTo(slopP.x,slopP.y, longLen,maxT);			// (3)
	pDom->LineTo(-termW/2,termH, -weldLen,minT);		// 작은쪽 V(5)
	pDom->LineTo( termW/2,termH, xp.x,xp.y);			// 큰쪽 V(6)
	pDom->LineTo( xp.x,xp.y, slopP.x,slopP.y);   		// Taper(7)
	pDom->LineTo(-termW/2,0,-termW/2,termH);
	pDom->LineTo( termW/2,0, termW/2,termH);
	pDom->LineTo(termW/2,0, slopP.x-xp.x, minT - maxT); //(8)
	pDom->LineTo(longLen, minT - maxT);					//(9)

	// 좌우측 CUTTING (Arc/2*2)
	pDom->Arc(-shortLen,minT,   -shortLen-minT/10, minT*3/4, -shortLen,minT/2); // LEFT
	pDom->Arc( -shortLen,0, -shortLen+minT/10, minT*1/4, -shortLen,minT/2);
	pDom->Arc(longLen,maxT,   longLen-minT/10, maxT*3/4, longLen,maxT/2); // RIGHT
	pDom->Arc(longLen,minT -maxT, longLen+maxT/10, maxT*1/4, longLen,maxT/2);
	
	// 다듬질면 SOLID
	pOptStd->SetEnvType(pDom,HCAD_HATH);
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
	pDom->Arc(xy[6].x, xy[6].y, Radius, AngleV, AngleV);
	
	// SOLID HATCH
	CDDWordArray BoundArea;
	BoundArea.Add(0);	BoundArea.Add(xy[4].x);	BoundArea.Add(xy[4].y);	BoundArea.Add(xy[3].x); BoundArea.Add(xy[3].y);
	
	BoundArea.Add(1);	BoundArea.Add(xy[6].x); BoundArea.Add(xy[6].y);	BoundArea.Add(Radius);
	BoundArea.Add(AngleV); BoundArea.Add(AngleV*2);	//시작각, 끝각
	BoundArea.Add(1);
	pDom->Hatch(&BoundArea,"ANSI31", pDomP->GetScaleDim());
	
	//pDom->Solid(-termW/2,0, termW/2,0,  termW/2,termH, -termW/2,termH);
	pDom->HatchRect(-termW/2,0, termW/2,0,  termW/2,termH, -termW/2,termH,"ANSI31", pDomP->GetScaleDim());
	//pDom->Solid(-weldLen,minT, -termW/2,termH, termW/2,termH, weldLen,minT);
	pDom->HatchRect(-weldLen,minT, -termW/2,termH, termW/2,termH, weldLen,minT,"ANSI31", pDomP->GetScaleDim());	
	
	// 방향별,상하별로 조정(MIRROR)
	if(nUpDown==-1) {
		pDom->Mirror(0,FALSE);		// 상하로 MIRROR
		if(T1>T2)
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}
	else {
		if(T1>T2)	
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}
	
	/////////////////////////////
	DimFactDetail_VType(pDom,T1,T2,Scale,nUpDown,termW,termH,AngleV,bCenter, bDimDraw, bShop);

	*pDomP << *pDom;
	delete pDom;
}

/*                                                     (0)          (3)
                                                           ---------------------
                                           slop        |
											       | 
                 (2)         weldLen   xp      |   (7)
        ---------------------     *   (4) |      slopLen
                             |         |                                    maxT
                              |       |                    
		minT               (5) |     |  (6)                         
							    |   |
								 | |              (1)
 		--------------------------*-------------------------------------------
		       shortLen           (0,0)                     longLen
*/

void CAPlateDrawSebu::DrawFactDetail_VType(CDomyun *pDomP,double T1,double T2, double Scale
									, long nUpDown,double termW, double termH,double AngleV, BOOL bCenter, BOOL bDimDraw, BOOL bShop)
{	
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(Scale);

	// 기본 두께 확대
	long nS = 2;
	T1 *= nS;
	T2 *= nS;
	termW *= nS;
	termH *= nS;

	// DRAWING의 기준은 이음면의 중심하단,
	pOptStd->SetEnvType(pDom,HCAD_STLC);
	double minT = min(T1,T2);
	double maxT = max(T1,T2);

	// 용접될 부재의 길이
	double shortLen = 50 ;	
	double longLen = 125;
	if(T1==T2) longLen = 100;

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
	pDom->LineTo(-shortLen,0, longLen,0);					// (1)
	pDom->LineTo(-shortLen,minT, -weldLen,minT);			// (2)
	pDom->LineTo(slopP.x,slopP.y, longLen,maxT);			// (3)
	pDom->LineTo(-termW/2,termH, -weldLen,minT);		// 작은쪽 V(5)
	pDom->LineTo( termW/2,termH, xp.x,xp.y);			// 큰쪽 V(6)
	pDom->LineTo( xp.x,xp.y, slopP.x,slopP.y);   		// Taper(7)
	pDom->LineTo(-termW/2,0,-termW/2,termH);
	pDom->LineTo( termW/2,0, termW/2,termH);

	// 좌우측 CUTTING (Arc/2*2)
	pDom->Arc(-shortLen,minT,   -shortLen-minT/10, minT*3/4, -shortLen,minT/2); // LEFT
	pDom->Arc( -shortLen,0, -shortLen+minT/10, minT*1/4, -shortLen,minT/2);
	pDom->Arc(longLen,maxT,   longLen-minT/10, maxT*3/4, longLen,maxT/2); // RIGHT
	pDom->Arc(longLen,0, longLen+maxT/10, maxT*1/4, longLen,maxT/2);

	// 다듬질면 SOLID
	pOptStd->SetEnvType(pDom,HCAD_HATH);
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
	pDom->Arc(xy[6].x, xy[6].y, Radius, AngleV, AngleV);

	// SOLID HATCH
	CDDWordArray BoundArea;
	BoundArea.Add(0);	BoundArea.Add(xy[4].x);	BoundArea.Add(xy[4].y);	BoundArea.Add(xy[3].x); BoundArea.Add(xy[3].y);

	BoundArea.Add(1);	BoundArea.Add(xy[6].x); BoundArea.Add(xy[6].y);	BoundArea.Add(Radius);
	BoundArea.Add(AngleV); BoundArea.Add(AngleV*2);	//시작각, 끝각
	BoundArea.Add(1);
	pDom->Hatch(&BoundArea,"ANSI31", pDomP->GetScaleDim());

	//pDom->Solid(-termW/2,0, termW/2,0,  termW/2,termH, -termW/2,termH);
	pDom->HatchRect(-termW/2,0, termW/2,0,  termW/2,termH, -termW/2,termH,"ANSI31", pDomP->GetScaleDim());
	//pDom->Solid(-weldLen,minT, -termW/2,termH, termW/2,termH, weldLen,minT);
	pDom->HatchRect(-weldLen,minT, -termW/2,termH, termW/2,termH, weldLen,minT,"ANSI31", pDomP->GetScaleDim());	
 
	// 방향별,상하별로 조정(MIRROR)
	if(nUpDown==-1) {
		pDom->Mirror(0,FALSE);		// 상하로 MIRROR
		if(T1>T2)
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}
	else {
		if(T1>T2)	
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}

	/////////////////////////////
	DimFactDetail_VType(pDom,T1,T2,Scale,nUpDown,termW,termH,AngleV,bCenter, bDimDraw, bShop);

	*pDomP << *pDom;
	delete pDom;
}

/*                                                      (10)                   (11)
                                                           ---------------------
                                           slop        |
											       | 
        (5)              (4) weldLen   xp      |         
        ---------------------     *   (9) |      slopLen
                             |         |                                    maxT
                              |       |                    
		minT                   |     |                              
				           (3)  |13 |  (8)
								 | |                 
                           (2) | 14  | (7)
 		--------------------------*-------------------------------------------
		(0)   shortLen      (1)  (0,0) (6)                 longLen            (12)
*/

void CAPlateDrawSebu::DrawFactDetail_XType_CType(CDomyun *pDomP,double T1,double T2, double Scale
									, long nUpDown,double termW, double termH, double termH2, double AngleV, double AngleX, BOOL bCenter, BOOL bShop)
{	
	double dUpDown= (double)nUpDown;
	if(fabs(dUpDown)!=1)
	{
		DrawFactDetail_XTypeByWeb(pDomP, T1, T2, Scale, nUpDown, termW, termH, termH2, AngleV, AngleX, bCenter, bShop);
		return;
	}
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(Scale);
	long nS = 2;

	T1 *= nS;
	T2 *= nS;
	termW *= nS;
	termH *= nS;
	termH2 *= nS;

	double TotHterm = termH2 + termH;
	// DRAWING의 기준은 이음면의 중심하단,
	pOptStd->SetEnvType(pDom,HCAD_STLC);
	double minT = min(T1,T2);
	double maxT = max(T1,T2);

	// 용접될 부재의 길이
	double shortLen = 50 ;	
	double longLen = 125;
	if(T1==T2) longLen = 100;

	// 경사 결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;

	// 플레이트,플레이트
	double weldVLen = termW/2+(minT-TotHterm)*tan(ToRadian(AngleV/2));
	double weldCLen = termW/2+(maxT-TotHterm)*tan(ToRadian(AngleV/2));	//bCenter = TRUE시 사용
	double weldXLen = termW/2 + termH2 * tan(ToRadian(AngleX/2));
	double slopLen = (maxT-minT)/slop;
	if (slopLen+weldVLen >= longLen)
		longLen = weldVLen+slopLen+20;
	// MAKE POINT
	CDPoint xy[17];
	xy[0]  = CDPoint(-shortLen, 0);				xy[1]  = CDPoint(-weldXLen, 0);
	xy[2]  = CDPoint(-termW/2, termH2);			xy[3]  = CDPoint(-termW/2, TotHterm);
	xy[4]  = CDPoint(-weldVLen, minT);			xy[5]  = CDPoint(-shortLen, minT);
	xy[6]  = CDPoint( weldXLen, 0);				xy[7]  = CDPoint( termW/2, termH2);
	xy[8]  = CDPoint( termW/2, TotHterm);		xy[9]  = CDPoint( weldVLen, minT);
	xy[10] = CDPoint( weldVLen+slopLen, maxT);	xy[11] = CDPoint( longLen, maxT);
	xy[12] = CDPoint( longLen, 0);				

	if(bCenter)
		GetXyMatchSegAndSeg(xy[8],CDPoint(weldCLen,maxT), CDPoint(0,minT),CDPoint(slopLen, maxT), xy[9]);

	// For Arc
	GetXyMatchSegAndSeg(xy[4], xy[3], xy[9], xy[8], xy[13]);
	GetXyMatchSegAndSeg(xy[1], xy[2], xy[6], xy[7], xy[14]);
	double dVRadius = ~(xy[4]-xy[13]);
	double dXRadius = ~(xy[1]-xy[14]);
	xy[15] = CDPoint(0, xy[13].y+dVRadius);
	xy[16] = CDPoint(0, xy[14].y-dXRadius);

	// DRAW
	pDom->LineTo(xy[0], xy[1]);		pDom->LineTo(xy[1], xy[2]);
	pDom->LineTo(xy[2], xy[3]);		pDom->LineTo(xy[3], xy[4]);
	pDom->LineTo(xy[4], xy[5]);		
	pDom->LineTo(xy[6], xy[7]);		pDom->LineTo(xy[7], xy[8]);
	pDom->LineTo(xy[8], xy[9]);		pDom->LineTo(xy[9], xy[10]);
	pDom->LineTo(xy[10], xy[11]);   
	pDom->LineTo(xy[6].x, xy[6].y, xy[6].x + (xy[10].x - xy[9].x), xy[6].y - (xy[10].y - xy[9].y));
	pDom->LineTo(xy[6].x + (xy[10].x - xy[9].x), xy[6].y - (xy[10].y - xy[9].y) , longLen, xy[6].y - (xy[10].y - xy[9].y));
	
	
	// 좌우측 CUTTING (Arc/2*2)
	pDom->Arc(-shortLen,minT,   -shortLen-minT/10, minT*3/4, -shortLen,minT/2); // LEFT
	pDom->Arc( -shortLen,0, -shortLen+minT/10, minT*1/4, -shortLen,minT/2);
	pDom->Arc(longLen,maxT,   longLen-minT/10, maxT*3/4, longLen,maxT/2); // RIGHT
	pDom->Arc(longLen,xy[9].y - xy[10].y, longLen+maxT/10, maxT*1/4, longLen,maxT/2);

	// 다듬질면 ARC
	pDom->Arc(xy[4].x, xy[4].y, xy[15].x, xy[15].y, xy[9].x, xy[9].y);
	pDom->Arc(xy[6].x, xy[6].y, xy[16].x, xy[16].y, xy[1].x, xy[1].y);

	// 다듬질면 SOLID
	pOptStd->SetEnvType(pDom,HCAD_HATH);

	// SOLID HATCH
	CDDWordArray BoundArea;
	BoundArea.Add(0);	BoundArea.Add(xy[4].x);	BoundArea.Add(xy[4].y);	BoundArea.Add(xy[9].x); BoundArea.Add(xy[9].y);

	BoundArea.Add(1);	BoundArea.Add(xy[13].x); BoundArea.Add(xy[13].y);	BoundArea.Add(dVRadius);
	BoundArea.Add(AngleV); BoundArea.Add(AngleV*2);	//시작각, 끝각
	BoundArea.Add(1);
	pDom->Hatch(&BoundArea,"ANSI31", pDomP->GetScaleDim());

	CDDWordArray BoundAreaX;
	BoundAreaX.Add(0);	BoundAreaX.Add(xy[6].x);	BoundAreaX.Add(xy[6].y);	BoundAreaX.Add(xy[1].x); BoundAreaX.Add(xy[1].y);

	BoundAreaX.Add(1);	BoundAreaX.Add(xy[14].x); BoundAreaX.Add(xy[14].y);	BoundAreaX.Add(dXRadius);
	BoundAreaX.Add(225); BoundAreaX.Add(315);//시작각, 끝각
	BoundAreaX.Add(1);
	pDom->Hatch(&BoundAreaX,"ANSI31", pDomP->GetScaleDim());

	pDom->HatchRect(xy[2].x,xy[2].y, xy[7].x,xy[7].y, xy[8].x,xy[8].y, xy[3].x,xy[3].y,"ANSI31", pDomP->GetScaleDim());
	pDom->HatchRect(xy[3].x,xy[3].y, xy[8].x,xy[8].y, xy[9].x,xy[9].y, xy[4].x,xy[4].y,"ANSI31", pDomP->GetScaleDim());
	pDom->HatchRect(xy[1].x,xy[1].y, xy[6].x,xy[6].y, xy[7].x,xy[7].y, xy[2].x,xy[2].y,"ANSI31", pDomP->GetScaleDim());

	// 방향별,상하별로 조정(MIRROR)
	if (nUpDown==-1 )
	{
		pDom->Mirror(0,FALSE);		// 상하로 MIRROR
		if(T1>T2)
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}
	else 
	{
		if(T1>T2)	
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}

	/////////////////////////////
	DimFactDetail_XType(pDom,T1,T2,Scale, nUpDown,termW, termH, termH2, AngleV,AngleX,bCenter, bShop);

	*pDomP << *pDom;
	delete pDom;
}

/*                                                      (10)                   (11)
                                                           ---------------------
                                           slop        |
											       | 
        (5)              (4) weldLen   xp      |         
        ---------------------     *   (9) |      slopLen
                             |         |                                    maxT
                              |       |                    
		minT                   |     |                              
				           (3)  |13 |  (8)
								 | |                 
                           (2) | 14  | (7)
 		--------------------------*-------------------------------------------
		(0)   shortLen      (1)  (0,0) (6)                 longLen            (12)
*/

void CAPlateDrawSebu::DrawFactDetail_XType(CDomyun *pDomP,double T1,double T2, double Scale
									, long nUpDown,double termW, double termH, double termH2, double AngleV, double AngleX, BOOL bCenter, BOOL bShop)
{	
	double dUpDown = (double)nUpDown;
	if(fabs(dUpDown)!=1)
	{
		DrawFactDetail_XTypeByWeb(pDomP, T1, T2, Scale, nUpDown, termW, termH, termH2, AngleV, AngleX, bCenter, bShop);
		return;
	}
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(Scale);
	long nS = 2;

	T1 *= nS;
	T2 *= nS;
	termW *= nS;
	termH *= nS;
	termH2 *= nS;

	double TotHterm = termH2 + termH;
	// DRAWING의 기준은 이음면의 중심하단,
	pOptStd->SetEnvType(pDom,HCAD_STLC);
	double minT = min(T1,T2);
	double maxT = max(T1,T2);

	// 용접될 부재의 길이
	double shortLen = 50 ;	
	double longLen = 125;
	if(T1==T2) longLen = 100;

	// 경사 결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;

	// 플레이트,플레이트
	double weldVLen = termW/2+(minT-TotHterm)*tan(ToRadian(AngleV/2));
	double weldCLen = termW/2+(maxT-TotHterm)*tan(ToRadian(AngleV/2));	//bCenter = TRUE시 사용
	double weldXLen = termW/2 + termH2 * tan(ToRadian(AngleX/2));
	double slopLen = (maxT-minT)/slop;
	if (slopLen+weldVLen >= longLen)
		longLen = weldVLen+slopLen+20;
	// MAKE POINT
	CDPoint xy[17];
	xy[0]  = CDPoint(-shortLen, 0);				xy[1]  = CDPoint(-weldXLen, 0);
	xy[2]  = CDPoint(-termW/2, termH2);			xy[3]  = CDPoint(-termW/2, TotHterm);
	xy[4]  = CDPoint(-weldVLen, minT);			xy[5]  = CDPoint(-shortLen, minT);
	xy[6]  = CDPoint( weldXLen, 0);				xy[7]  = CDPoint( termW/2, termH2);
	xy[8]  = CDPoint( termW/2, TotHterm);		xy[9]  = CDPoint( weldVLen, minT);
	xy[10] = CDPoint( weldVLen+slopLen, maxT);	xy[11] = CDPoint( longLen, maxT);
	xy[12] = CDPoint( longLen, 0);

	if(bCenter)
		GetXyMatchSegAndSeg(xy[8],CDPoint(weldCLen,maxT), CDPoint(0,minT),CDPoint(slopLen, maxT), xy[9]);

	// For Arc
	GetXyMatchSegAndSeg(xy[4], xy[3], xy[9], xy[8], xy[13]);
	GetXyMatchSegAndSeg(xy[1], xy[2], xy[6], xy[7], xy[14]);
	double dVRadius = ~(xy[4]-xy[13]);
	double dXRadius = ~(xy[1]-xy[14]);
	xy[15] = CDPoint(0, xy[13].y+dVRadius);
	xy[16] = CDPoint(0, xy[14].y-dXRadius);

	// DRAW
	pDom->LineTo(xy[0], xy[1]);		pDom->LineTo(xy[1], xy[2]);
	pDom->LineTo(xy[2], xy[3]);		pDom->LineTo(xy[3], xy[4]);
	pDom->LineTo(xy[4], xy[5]);		pDom->LineTo(xy[12], xy[6]);
	pDom->LineTo(xy[6], xy[7]);		pDom->LineTo(xy[7], xy[8]);
	pDom->LineTo(xy[8], xy[9]);		pDom->LineTo(xy[9], xy[10]);
	pDom->LineTo(xy[10], xy[11]);

	// 좌우측 CUTTING (Arc/2*2)
	pDom->Arc(-shortLen,minT,   -shortLen-minT/10, minT*3/4, -shortLen,minT/2); // LEFT
	pDom->Arc( -shortLen,0, -shortLen+minT/10, minT*1/4, -shortLen,minT/2);
	pDom->Arc(longLen,maxT,   longLen-minT/10, maxT*3/4, longLen,maxT/2); // RIGHT
	pDom->Arc(longLen,0, longLen+maxT/10, maxT*1/4, longLen,maxT/2);

	// 다듬질면 ARC
	pDom->Arc(xy[4].x, xy[4].y, xy[15].x, xy[15].y, xy[9].x, xy[9].y);
	pDom->Arc(xy[6].x, xy[6].y, xy[16].x, xy[16].y, xy[1].x, xy[1].y);

	// 다듬질면 SOLID
	pOptStd->SetEnvType(pDom,HCAD_HATH);

	// SOLID HATCH
	CDDWordArray BoundArea;
	BoundArea.Add(0);	BoundArea.Add(xy[4].x);	BoundArea.Add(xy[4].y);	BoundArea.Add(xy[9].x); BoundArea.Add(xy[9].y);

	BoundArea.Add(1);	BoundArea.Add(xy[13].x); BoundArea.Add(xy[13].y);	BoundArea.Add(dVRadius);
	BoundArea.Add(AngleV); BoundArea.Add(AngleV*2);	//시작각, 끝각
	BoundArea.Add(1);
	pDom->Hatch(&BoundArea,"ANSI31", pDomP->GetScaleDim());

	CDDWordArray BoundAreaX;
	BoundAreaX.Add(0);	BoundAreaX.Add(xy[6].x);	BoundAreaX.Add(xy[6].y);	BoundAreaX.Add(xy[1].x); BoundAreaX.Add(xy[1].y);

	BoundAreaX.Add(1);	BoundAreaX.Add(xy[14].x); BoundAreaX.Add(xy[14].y);	BoundAreaX.Add(dXRadius);
	BoundAreaX.Add(225); BoundAreaX.Add(315);//시작각, 끝각
	BoundAreaX.Add(1);
	pDom->Hatch(&BoundAreaX,"ANSI31", pDomP->GetScaleDim());

	pDom->HatchRect(xy[2].x,xy[2].y, xy[7].x,xy[7].y, xy[8].x,xy[8].y, xy[3].x,xy[3].y,"ANSI31", pDomP->GetScaleDim());
	pDom->HatchRect(xy[3].x,xy[3].y, xy[8].x,xy[8].y, xy[9].x,xy[9].y, xy[4].x,xy[4].y,"ANSI31", pDomP->GetScaleDim());
	pDom->HatchRect(xy[1].x,xy[1].y, xy[6].x,xy[6].y, xy[7].x,xy[7].y, xy[2].x,xy[2].y,"ANSI31", pDomP->GetScaleDim());

	// 방향별,상하별로 조정(MIRROR)
	if (nUpDown==-1 )
	{
		pDom->Mirror(0,FALSE);		// 상하로 MIRROR
		if(T1>T2)
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}
	else 
	{
		if(T1>T2)	
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}

	/////////////////////////////
	DimFactDetail_XType(pDom,T1,T2,Scale, nUpDown,termW, termH, termH2, AngleV,AngleX,bCenter, bShop);

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawSebu::DrawFactDetail_XTypeByWeb(CDomyun *pDomP,double T1,double T2, double Scale
									, long nUpDown,double termW, double termH, double termH2, double AngleV, double AngleX, BOOL bCenter, BOOL bShop)
{	
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CGlobarOption *pGlopt		= m_pDataMng->GetGlobalOption();
	double dThickXWeld			= pGlopt->GetDesignThickXWeldApply();

	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(Scale);
	long nS = 2;

	//////////////////////////////////////////////////////////////////////////
	{
		double dMinT	= min(T1,T2);
		long nType		= (dThickXWeld < dMinT) ? XWELD_UF : XWELD_L;
		// CDomyun::FilletText(~)참고
		if(nType==XWELD_UF)
			termH2	= (dMinT-termH)/3;
		else
			termH2	= dMinT-termH;
	}
	//////////////////////////////////////////////////////////////////////////	

	T1 *= nS;
	T2 *= nS;
	termW *= nS;
	termH *= nS;
	termH2 *= nS;

	double TotHterm = termH2 + termH;
	// DRAWING의 기준은 이음면의 중심하단,
	pOptStd->SetEnvType(pDom,HCAD_STLC);
	double minT = min(T1,T2);
	double maxT = max(T1,T2);
	double dGab = (maxT-minT)/2;

	// 용접될 부재의 길이
	double shortLen = 50 ;	
	double longLen = 125;
	if(T1==T2) longLen = 100;

	// 경사 결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = (TaperH && TaperW) ? (TaperH/TaperW) : 0.2; // 1:5

	// 플레이트,플레이트
	double weldVLen = termW/2+(minT-TotHterm)*tan(ToRadian(AngleV/2));
	double weldCLen = termW/2+(maxT-TotHterm)*tan(ToRadian(AngleV/2));	//bCenter = TRUE시 사용
	double slopLen = (maxT-minT)/(slop*2);
	if (slopLen+weldVLen >= longLen)
		longLen = weldVLen+slopLen+20;
	// MAKE POINT
	CDPoint xy[18];
	xy[0]  = CDPoint(-shortLen, 0);					xy[1]  = CDPoint(-weldVLen, 0);
	xy[2]  = CDPoint(-termW/2, termH2);				xy[3]  = CDPoint(-termW/2, TotHterm);
	xy[4]  = CDPoint(-weldVLen, minT);				xy[5]  = CDPoint(-shortLen, minT);
	xy[6]  = CDPoint( weldVLen+slopLen, -dGab);		
	xy[7]  = CDPoint( weldVLen, 0);		xy[8]  = CDPoint( termW/2, termH2);
	xy[9]  = CDPoint( termW/2, TotHterm);			xy[10]  = CDPoint( weldVLen, minT);
	xy[11] = CDPoint( weldVLen+slopLen, maxT-dGab);	xy[12] = CDPoint( longLen, maxT-dGab);
	xy[13] = CDPoint( longLen, -dGab);

	if(bCenter)
		GetXyMatchSegAndSeg(xy[9],CDPoint(weldCLen,maxT), CDPoint(0,minT),CDPoint(slopLen, maxT), xy[10]);

	// For Arc
	GetXyMatchSegAndSeg(xy[4], xy[3], xy[10], xy[9], xy[14]);
	GetXyMatchSegAndSeg(xy[1], xy[2], xy[7], xy[8], xy[15]);
	double dVRadius = ~(xy[4]-xy[14]);
	double dXRadius = ~(xy[1]-xy[15]);
	xy[16] = CDPoint(0, xy[14].y+dVRadius);
	xy[17] = CDPoint(0, xy[15].y-dXRadius);

	long n = 0;
	// DRAW
	for(n=0; n<12; n++)
	{
		if(n==5)
			continue;
		pDom->LineTo(xy[n], xy[n+1]);
	}
	pDom->LineTo(xy[13], xy[6]);

	// 좌우측 CUTTING (Arc/2*2)
	pDom->Arc(-shortLen,minT,   -shortLen-minT/10, minT*3/4, -shortLen,minT/2); // LEFT
	pDom->Arc( -shortLen,0, -shortLen+minT/10, minT*1/4, -shortLen,minT/2);
	pDom->Arc(longLen,maxT-dGab,   longLen-minT/10, maxT*3/4-dGab, longLen,maxT/2-dGab); // RIGHT
	pDom->Arc(longLen,-dGab, longLen+maxT/10, maxT*1/4-dGab, longLen,maxT/2-dGab);

	// 다듬질면 ARC
	pDom->Arc(xy[4].x, xy[4].y, xy[16].x, xy[16].y, xy[10].x, xy[10].y);
	pDom->Arc(xy[7].x, xy[7].y, xy[17].x, xy[17].y, xy[1].x, xy[1].y);

	// 다듬질면 SOLID
	pOptStd->SetEnvType(pDom,HCAD_HATH);

	// SOLID HATCH
	CDDWordArray BoundArea;
	BoundArea.Add(0);	BoundArea.Add(xy[4].x);	BoundArea.Add(xy[4].y);	BoundArea.Add(xy[10].x); BoundArea.Add(xy[10].y);

	BoundArea.Add(1);	BoundArea.Add(xy[14].x); BoundArea.Add(xy[14].y);	BoundArea.Add(dVRadius);
	double dSttAng = (xy[10]-xy[9]).Unit().GetAngleDegree();
	double dEndAng = (xy[4]-xy[3]).Unit().GetAngleDegree();
	BoundArea.Add(dSttAng); BoundArea.Add(dEndAng);//시작각, 끝각
	BoundArea.Add(1);
	pDom->Hatch(&BoundArea,"ANSI31", pDomP->GetScaleDim());

	CDDWordArray BoundAreaX;
	CDPoint xyArcCen(0,0);
	BoundAreaX.Add(0);	BoundAreaX.Add(xy[7].x);	BoundAreaX.Add(xy[7].y);	BoundAreaX.Add(xy[1].x); BoundAreaX.Add(xy[1].y);

	BoundAreaX.Add(1);	BoundAreaX.Add(xy[15].x); BoundAreaX.Add(xy[15].y);	BoundAreaX.Add(dXRadius);
	dSttAng = (xy[1]-xy[2]).Unit().GetAngleDegree();
	dEndAng = (xy[7]-xy[8]).Unit().GetAngleDegree();
	BoundAreaX.Add(dSttAng); BoundAreaX.Add(dEndAng);//시작각, 끝각
	BoundAreaX.Add(1);
	pDom->Hatch(&BoundAreaX,"ANSI31", pDomP->GetScaleDim());

	pDom->HatchRect(xy[2].x,xy[2].y, xy[8].x,xy[8].y, xy[9].x,xy[9].y, xy[3].x,xy[3].y,"ANSI31", pDomP->GetScaleDim());
	pDom->HatchRect(xy[3].x,xy[3].y, xy[9].x,xy[9].y, xy[10].x,xy[10].y, xy[4].x,xy[4].y,"ANSI31", pDomP->GetScaleDim());
	pDom->HatchRect(xy[1].x,xy[1].y, xy[7].x,xy[7].y, xy[8].x,xy[8].y, xy[2].x,xy[2].y,"ANSI31", pDomP->GetScaleDim());

	// 방향별,상하별로 조정(MIRROR)
	if (nUpDown==-1 )
	{
		pDom->Mirror(0,FALSE);		// 상하로 MIRROR
		if(T1>T2)
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}
	else 
	{
		if(T1>T2)	
			pDom->Mirror(0,TRUE);	// 좌우로 MIRROR
	}	

	/////////////////////////////
	DimFactDetail_XTypeByWeb(pDom,T1,T2,Scale, nUpDown,termW, termH, termH2, AngleV,AngleX,bCenter, bShop);

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawSebu::DimFactDetail_VType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
					  ,double termW, double termH,double AngleV, BOOL bCenter, BOOL bDimDraw, BOOL bShop)
{	
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	pOptStd->SetEnvType(pDomP,HCAD_DIML);
	BOOL bMirror = FALSE;
	if(T1 > T2) bMirror = TRUE;
	else if(T1 < T2) bMirror = FALSE;
	long nS = 2;

	CDimDomyunEx *pDom = new CDimDomyunEx(pDomP);

	// DRAWING의 기준은 이음면의 중심하단,
	double minT = min(T1,T2);
	double maxT = max(T1,T2);
	// 용접될 부재의 길이
	double shortLen = 50;	
	double longLen = 125;
	if(T1==T2) longLen = 80;

	// 경사결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;

	// 플레이트
	double weldLen = termW/2+(minT-termH)*tan(ToRadian(AngleV/2));
	double slopLen = (maxT-minT)/slop;	
	
	if(slopLen+weldLen >= longLen && T1!=T2)
		longLen = weldLen+slopLen+20;
	CDPoint xp = CDPoint(weldLen,minT);						// (4)
	CDPoint slopP = CDPoint(bMirror? -weldLen-slopLen : weldLen+slopLen, maxT);			// (0)

	// 좌우 치수
	double valLen = 0;
	// SHORT LEN
	CDPoint sp = CDPoint(bMirror ? shortLen : -shortLen,0);
	pDom->SetDirection(bMirror? "RIGHT" : "LEFT");
	valLen = minT;

	if(!bDimDraw)
	{
		pDom->DimMoveTo(sp,1);
		pDom->DimLineTo(sp.x, nS*2,COMMA(nS));
		pDom->DimLineTo(sp.x, valLen-nS*2,COMMA(valLen/nS-nS));
		if (bShop && nUpDown!=0)
		{
			if(bMirror)
			{
				sp.x = sp.x+(pDom->GetDimExo()+pDom->GetDimVLen())*pDom->GetScaleDim();
				double TextLength = pDom->Always(12);
				double Dis = -TextLength*2;
				pDom->LineTo(sp.x, sp.y, sp.x, sp.y+Dis);
				pDom->SetTextAngle(90);
				Dis = -TextLength;
				pDom->TextOut(sp.x-pDom->Always(1), sp.y+Dis, "(BOX INSIDE)");
			}
		}
	}

	// LONG LEN
	if(T1!=T2)
	{
		sp = CDPoint(bMirror? -longLen : longLen,0);
		pDom->SetDirection(bMirror ? "LEFT" : "RIGHT");
		if(!bDimDraw)
		{
			pDom->DimMoveTo(sp,1);
			pDom->DimLineTo(termW/2,termH,COMMA(termH/nS));
		}

		pDom->DimMoveTo(sp.x,maxT,1);
		pDom->DimLineTo(xp.x, -(maxT-minT), bDimDraw ? "H" : COMMA((maxT-minT)/nS));
	}
	
	if(!bDimDraw)
	{
		if(T1!=T2)
		{
			pDom->DimMoveTo(sp, 2);
			pDom->DimLineTo(sp.x, maxT, COMMA(maxT/nS));
		}
		if (bShop && nUpDown!=0)
		{
			if(!bMirror)
			{
				sp.x = sp.x+(pDom->GetDimExo()+pDom->GetDimVLen()+pDom->GetDimLevelDis())*pDom->GetScaleDim();
				double TextLength = pDom->Always(12);
				double Dis = -TextLength*2;
				pDom->LineTo(sp.x, sp.y, sp.x, sp.y+Dis);
				pDom->SetTextAngle(90);
				Dis = -TextLength;
				pDom->TextOut(sp.x-pDom->Always(1), sp.y+Dis, "(BOX INSIDE)");
			}
		}
	}

	if(nUpDown==-1) pDom->Mirror(0,FALSE);
	*pDomP << *pDom;

	////////////// MIRROR 영향 ///////////////
	pDom->SetTextAngle(0);
	CString str = _T("");
	if(nUpDown==-1)
	{
		sp = CDPoint(-termW/2,-termH);
		pDom->SetDirection("TOP");
		pDom->DimMoveTo(sp.x,sp.y,1,FALSE,COMMA(termW/nS));
		pDom->DimLineTo(termW,sp.y,"");
		// ARC DIMMENSION
		GetXyMatchSegAndSeg( CDPoint(+termW/2,-termH), CDPoint(xp.x,-xp.y),
			               CDPoint(-termW/2,-termH), CDPoint(-termW/2-(minT-termH)*tan(ToRadian(AngleV/2)),-minT),
						   sp);	// sp = 원의 중심
		double circleR = (minT+sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		str.Format("%.0f",AngleV);	
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,-circleR*sin(ToRadian(AngleV/2)),-minT,0);
		pDom->DimArcLineTo(circleR*sin(ToRadian(AngleV/2)),-minT,0,str+"%%d");

		// TAPER
		if(slopLen!=0)
		{
			pDom->SetDirection("BOTTOM");
			pDom->DimMoveTo(slopP.x,slopP.y-maxT*2, 2);
			pDom->DimLineTo(bMirror? slopLen : -slopLen, xp.y-minT*2, COMMA(slopLen/2));
		}
	}
	else
	{
		sp = CDPoint(-termW/2,termH);
		pDom->SetDirection("BOTTOM");
		if(!bDimDraw)
		{
			pDom->DimMoveTo(sp.x,sp.y,1);
			pDom->DimLineTo(termW,sp.y,COMMA(termW/nS));
		}
		// ARC DIMMENSION
		pDom->SetDirection("TOP");
		GetXyMatchSegAndSeg( CDPoint(+termW/2,termH), xp,
			               CDPoint(-termW/2,termH), CDPoint(-termW/2-(minT-termH)*tan(ToRadian(AngleV/2)),minT),
						   sp);	// sp = 원의 중심
		double circleR = (minT-sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		str.Format("%.0f",AngleV);
		if(!bDimDraw)
		{
			pDom->DimArcMoveTo(sp.x,sp.y,circleR,circleR*sin(ToRadian(AngleV/2)),minT,0);
			pDom->DimArcLineTo(-circleR*sin(ToRadian(AngleV/2)),minT,0,str+"%%d");
		}

		// TAPER
		if(slopLen!=0)
		{
			pDom->DimMoveTo(slopP, 2);
			pDom->DimLineTo(bMirror? slopLen : -slopLen, xp.y, bDimDraw ? "W" : COMMA(slopLen/2));
		}
	}

	// TAPER
	str.Format("1 : %.1f",1 / slop);
	double TextAngle = 90;
	if(nUpDown==-1)
	{
		if(T1<T2)
		{
			TextAngle = -ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2,str);
		}
		else if(T1>T2)
		{
			TextAngle = ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2,str);
		}
	}
	else
	{
		if(T1<T2)
		{
			TextAngle = ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
		else if(T1>T2)
		{
			TextAngle = -ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
	}


	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawSebu::DimFactDetail_XType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW, double termH, double termH2,double AngleV, double AngleX, BOOL bCenter, BOOL bShop)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	pOptStd->SetEnvType(pDomP,HCAD_DIML);

	CDimDomyunEx *pDom = new CDimDomyunEx(pDomP);

	BOOL bMirror = FALSE;
	if(T1 > T2) bMirror = TRUE;
	else if(T1 < T2) bMirror = FALSE;

	// DRAWING의 기준은 이음면의 중심하단,
	long nS = 2;
	double minT = min(T1,T2);
	double maxT = max(T1,T2);
	double shortLen = 50;	
	double longLen = 125;
	termH2 = (minT/2-2)/3*2;
	if(T1==T2) longLen = 100;
	double TotHterm = termH + termH2;
	// 경사결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;

	// 플레이트
	double weldLen = termW/2+(minT-TotHterm)*tan(ToRadian(AngleV/2));
	double slopLen = (maxT-minT)/slop;
	if (slopLen+weldLen >= longLen)
		longLen = weldLen+slopLen+20;
	CDPoint xp = CDPoint(weldLen,minT);						// (4)
	CDPoint slopP = CDPoint(bMirror? -weldLen-slopLen : weldLen+slopLen, maxT);			// (0)

	if(bCenter)
	GetXyMatchSegAndSeg( CDPoint(termW/2,TotHterm), CDPoint(termW/2+(maxT-TotHterm)*tan(ToRadian(AngleV/2)), maxT)
							,CDPoint(0, minT)
							,CDPoint((maxT-minT)/slop,maxT)
							,xp);

	double dHbetweenT = bCenter? maxT - xp.y : maxT - minT;

	// SHORT LEN
	CDPoint sp = CDPoint(bMirror ? shortLen : -shortLen,0);
	pDom->SetDirection(bMirror ? "RIGHT" : "LEFT");
	pDom->DimMoveTo(sp,1);
	pDom->DimLineTo(bMirror ? termW/2 : -termW/2, termH2, COMMA(termH2/nS));
	pDom->DimLineTo(bMirror ? termW/2 : -termW/2, termH, COMMA(termH/nS));
	pDom->DimLineTo(sp.x, minT-TotHterm, COMMA(minT/nS-termH/nS-Round(termH2/nS,0)));

	pDom->DimMoveTo(sp,2);
	pDom->DimLineTo(sp.x, minT, COMMA(minT/2));
	if (bShop && nUpDown!=0)
	{
		if(bMirror)
		{
			sp.x = sp.x+(pDom->GetDimExo()+pDom->GetDimVLen()+pDom->GetDimLevelDis())*pDom->GetScaleDim();
			double TextLength = pDom->Always(12);
			double Dis = -TextLength*2;
			pDom->LineTo(sp.x, sp.y, sp.x, sp.y+Dis);
			pDom->SetTextAngle(90);
			Dis = -TextLength;
			pDom->TextOut(sp.x-pDom->Always(1), sp.y+Dis, "(BOX INSIDE)");
		}
	}
	
	if(T1!=T2)
	{
		sp = CDPoint(bMirror? -longLen : longLen, maxT);
		pDom->SetDirection(bMirror? "LEFT" : "RIGHT");
		pDom->DimMoveTo(sp,1);
		pDom->DimLineTo(bMirror ? -xp.x : xp.x, -dHbetweenT, COMMA(dHbetweenT/2));
		pDom->DimMoveTo(sp,2);
		pDom->DimLineTo(sp.x, -maxT, COMMA(maxT/2));
	}
	if (bShop && nUpDown!=0)
	{
		if(!bMirror)
		{
			sp.x = sp.x+(pDom->GetDimExo()+pDom->GetDimVLen()+pDom->GetDimLevelDis())*pDom->GetScaleDim();
			double TextLength = pDom->Always(12);
			double Dis = -TextLength*2;
			pDom->LineTo(sp.x, sp.y, sp.x, sp.y+Dis);
			pDom->SetTextAngle(90);
			Dis = -TextLength;
			pDom->TextOut(sp.x-pDom->Always(1), sp.y+Dis, "(BOX INSIDE)");
		}
	}

	if(nUpDown==-1) pDom->Mirror(0,FALSE);
	*pDomP << *pDom;

	pDom->SetTextAngle(0);
	double circleR = maxT;
	CString str = _T("");
	////////////// MIRROR 영향 ///////////////
	if(nUpDown==-1)
	{
		sp = CDPoint(-termW/2,0);
		pDom->SetDirection("TOP");
		pDom->DimMoveTo(sp,2);
		pDom->DimLineTo(termW,0,COMMA(termW/nS));
	
		// ARC DIMMENSION
		GetXyMatchSegAndSeg( CDPoint(+termW/2,-TotHterm), CDPoint(xp.x,-xp.y),
			               CDPoint(-termW/2,-TotHterm), CDPoint(-termW/2-(minT-TotHterm)*tan(ToRadian(AngleV/2)),-minT),
						   sp);	// sp = 원의 중심
		circleR = -sp.y/cos(ToRadian(AngleX/2));
		str.Format("%.0f",AngleX);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,circleR*sin(ToRadian(AngleX/2)),0,0);
		pDom->DimArcLineTo(-circleR*sin(ToRadian(AngleX/2)),0,0,str+"%%d");
		//
		circleR = (minT+sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		str.Format("%.0f",AngleV);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,-circleR*sin(ToRadian(AngleV/2)),-minT,0);
		pDom->DimArcLineTo(circleR*sin(ToRadian(AngleV/2)),-minT,0,str+"%%d");

		// TAPER
		if(slopLen!=0)
		{
			pDom->SetDirection("BOTTOM");
			pDom->DimMoveTo(slopP.x,slopP.y-maxT*2, 2);
			pDom->DimLineTo(bMirror? slopLen : -slopLen, xp.y-minT*2, COMMA(slopLen/2));
		}
	}
	else {
		sp = CDPoint(-termW/2,0);
		pDom->SetDirection("BOTTOM");
		pDom->DimMoveTo(sp,2);
		pDom->DimLineTo(termW,0,COMMA(termW/nS));

		// ARC DIMMENSION
		GetXyMatchSegAndSeg( CDPoint(+termW/2,TotHterm), xp,
			               CDPoint(-termW/2,TotHterm), CDPoint(-termW/2-(minT-TotHterm)*tan(ToRadian(AngleV/2)),minT),
						   sp);	// sp = 원의 중심
		circleR = (minT-sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		pDom->SetDirection("TOP");
		str.Format("%.0f",AngleV);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,+circleR*sin(ToRadian(AngleV/2)),minT,0);
		pDom->DimArcLineTo(-circleR*sin(ToRadian(AngleV/2)),minT,0,str+"%%d");
		//
		circleR = sp.y/cos(ToRadian(AngleX/2));
		str.Format("%.0f",AngleX);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,-circleR*sin(ToRadian(AngleX/2)),0,0);
		pDom->DimArcLineTo(circleR*sin(ToRadian(AngleX/2)),0,0,str+"%%d");

		// TAPER
		if(slopLen!=0)
		{
			pDom->DimMoveTo(slopP, 2);
			pDom->DimLineTo(bMirror? slopLen : -slopLen, xp.y, COMMA(slopLen/2));
		}
	}


	// TAPER
	str.Format("1 : %.1f",1/slop);
	double TextAngle = 90;
	if(nUpDown==-1)
	{
		if(T1<T2)
		{
			TextAngle = -ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2.1,str);
		}
		else if(T1>T2)
		{
			TextAngle = ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2.1,str);
		}
	}
	else
	{
		if(T1<T2)
		{
			TextAngle = ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
		else if(T1>T2)
		{
			TextAngle = -ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
	}

  
	*pDomP << *pDom;
	delete pDom;

}

void CAPlateDrawSebu::DimFactDetail_XTypeByWeb(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW, double termH, double termH2,double AngleV, double AngleX, BOOL bCenter, BOOL bShop)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CGlobarOption *pGlopt		= m_pDataMng->GetGlobalOption();
	double dThickXWeld			= pGlopt->GetDesignThickXWeldApply();
	
	pOptStd->SetEnvType(pDomP,HCAD_DIML);

	CDimDomyunEx *pDom = new CDimDomyunEx(pDomP);

	BOOL bMirror = FALSE;
	if(T1 > T2) bMirror = TRUE;
	else if(T1 < T2) bMirror = FALSE;

	//////////////////////////////////////////////////////////////////////////
	{
		double dMinT	= min(T1,T2);
		long nType		= (dThickXWeld < dMinT) ? XWELD_UF : XWELD_L;
		// CDomyun::FilletText(~)참고
		if(nType==XWELD_UF)
			termH2	= (dMinT-termH)/3;
		else
			termH2	= dMinT-termH;
	}
	//////////////////////////////////////////////////////////////////////////	

	// DRAWING의 기준은 이음면의 중심하단,
	long nS = 2;
	double minT = min(T1,T2);
	double maxT = max(T1,T2);
	double shortLen = 50;	
	double longLen = 125;
	if(T1==T2) longLen = 100;
	double TotHterm = termH + termH2;
	// 경사결정
	double TaperH = pBridge->GetGirder(-1)->m_pSangse->m_Taper_H;
	double TaperW = pBridge->GetGirder(-1)->m_pSangse->m_Taper_W;
	double slop	  = 0.2; // 1:5
	if(TaperH && TaperW) slop = TaperH/TaperW;

	// 플레이트
	double weldLen = termW/2+(minT-TotHterm)*tan(ToRadian(AngleV/2));
	double slopLen = (maxT-minT)/slop;
	if (slopLen+weldLen >= longLen)
		longLen = weldLen+slopLen+20;
	CDPoint xp = CDPoint(weldLen,minT);						// (4)
	CDPoint slopP = CDPoint(bMirror? -weldLen-slopLen : weldLen+slopLen, maxT);			// (0)

	if(bCenter)
	GetXyMatchSegAndSeg( CDPoint(termW/2,TotHterm), CDPoint(termW/2+(maxT-TotHterm)*tan(ToRadian(AngleV/2)), maxT)
							,CDPoint(0, minT)
							,CDPoint((maxT-minT)/slop,maxT)
							,xp);

	double dHbetweenT = (maxT - minT)/2;

	// SHORT LEN
	CDPoint sp = CDPoint(bMirror ? shortLen : -shortLen,0);
	pDom->SetDirection(bMirror ? "RIGHT" : "LEFT");
	pDom->DimMoveTo(sp,1);
	pDom->DimLineTo(bMirror ? termW/2 : -termW/2, termH2, COMMA(termH2/nS));
	pDom->DimLineTo(bMirror ? termW/2 : -termW/2, termH, COMMA(termH/nS));
	pDom->DimLineTo(bMirror ? termW/2 : -termW/2, minT-TotHterm, COMMA((minT-TotHterm)/nS));

	pDom->DimMoveTo(sp,2);
	pDom->DimLineTo(sp.x, minT, COMMA(minT/2));
	
	if(T1!=T2)
	{
		sp = CDPoint(bMirror? -longLen : longLen, maxT-dHbetweenT);
		pDom->SetDirection(bMirror? "LEFT" : "RIGHT");
		pDom->DimMoveTo(sp,1);
		pDom->DimLineTo(bMirror ? -xp.x : xp.x, -dHbetweenT, COMMA(dHbetweenT/2));
		pDom->DimMoveTo(sp-CDPoint(0, maxT),1);
		pDom->DimLineTo(bMirror ? -xp.x : xp.x, dHbetweenT, COMMA(dHbetweenT/2));
		pDom->DimMoveTo(sp,2);
		pDom->DimLineTo(sp.x, -maxT, COMMA(maxT/2));
	}

	if(nUpDown==-1) pDom->Mirror(0,FALSE);
	*pDomP << *pDom;

	pDom->SetTextAngle(0);
	double circleR = maxT;
	CString str = _T("");
	////////////// MIRROR 영향 ///////////////
	if(nUpDown==-1)
	{
		sp = CDPoint(-termW/2,0);
		pDom->SetDirection("TOP");
		pDom->DimMoveTo(sp,2);
		pDom->DimLineTo(termW,0,COMMA(termW/nS));
	
		// ARC DIMMENSION
		GetXyMatchSegAndSeg( CDPoint(+termW/2,-TotHterm), CDPoint(xp.x,-xp.y),
			               CDPoint(-termW/2,-TotHterm), CDPoint(-termW/2-(minT-TotHterm)*tan(ToRadian(AngleV/2)),-minT),
						   sp);	// sp = 원의 중심
		circleR = -sp.y/cos(ToRadian(AngleX/2));
		str.Format("%.0f",AngleX);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,circleR*sin(ToRadian(AngleX/2)),0,0);
		pDom->DimArcLineTo(-circleR*sin(ToRadian(AngleX/2)),0,0,str+"%%d");
		//
		circleR = (minT+sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		str.Format("%.0f",AngleV);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,-circleR*sin(ToRadian(AngleV/2)),-minT,0);
		pDom->DimArcLineTo(circleR*sin(ToRadian(AngleV/2)),-minT,0,str+"%%d");

		// TAPER
		if(slopLen!=0)
		{
			pDom->SetDirection("BOTTOM");
			pDom->DimMoveTo(slopP.x,slopP.y-maxT*2, 2);
			pDom->DimLineTo(bMirror? slopLen : -slopLen, xp.y-minT*2, COMMA(slopLen/2));
		}
	}
	else {
		sp = CDPoint(-termW/2,0);
		pDom->SetDirection("BOTTOM");
		pDom->DimMoveTo(sp,2);
		pDom->DimLineTo(termW,0,COMMA(termW/nS));

		// ARC DIMMENSION
		GetXyMatchSegAndSeg( CDPoint(+termW/2,TotHterm), xp,
			               CDPoint(-termW/2,TotHterm), CDPoint(-termW/2-(minT-TotHterm)*tan(ToRadian(AngleV/2)),minT),
						   sp);	// sp = 원의 중심
		circleR = (minT-sp.y)/cos(ToRadian(AngleV/2));	// maxT/2는 여유폭
		pDom->SetDirection("TOP");
		str.Format("%.0f",AngleV);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,+circleR*sin(ToRadian(AngleV/2)),minT,0);
		pDom->DimArcLineTo(-circleR*sin(ToRadian(AngleV/2)),minT,0,str+"%%d");
		//
		circleR = sp.y/cos(ToRadian(AngleX/2));
		str.Format("%.0f",AngleX);
		pDom->DimArcMoveTo(sp.x,sp.y,circleR,-circleR*sin(ToRadian(AngleX/2)),0,0);
		pDom->DimArcLineTo(circleR*sin(ToRadian(AngleX/2)),0,0,str+"%%d");

		// TAPER
		if(slopLen!=0)
		{
			pDom->DimMoveTo(slopP, 2);
			pDom->DimLineTo(bMirror? slopLen : -slopLen, xp.y, COMMA(slopLen/2));
		}
	}


	// TAPER
	str.Format("1 : %.1f",1/slop);
	double TextAngle = 90;
	if(nUpDown==-1)
	{
		if(T1<T2)
		{
			TextAngle = -ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2.1,str);
		}
		else if(T1>T2)
		{
			TextAngle = ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2-maxT*2.1,str);
		}
	}
	else
	{
		if(T1<T2)
		{
			TextAngle = ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((xp.x+(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
		else if(T1>T2)
		{
			TextAngle = -ToDegree(atan(slop));
			pDom->SetTextAngle(TextAngle);
			pDom->TextOut((-xp.x-(maxT-minT)/slop)/2,(xp.y+maxT)/2,str);
		}
	}

  
	*pDomP << *pDom;
	delete pDom;

}

///////////////////////////////////////////////////////////
// 수평보강재 부착상세 (새로작성)
// 수직보강재 : 수평보강재
void CAPlateDrawSebu::AddHStiffDetail_WithVStiff(CDomyun *pDomP, CPlateBasicIndex *pBx, double Scale, BOOL bDrawDetailDlg)
{	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBx->GetGirder();	
	CSection		*pSec	 = pBx->GetSection();
	CVStiff			*pVStiff = pSec->GetVStiff();

	CDomyun* pDom = new CDomyun(pDomP);

	// 수직보강재 제원
	double VT  = pVStiff->m_dThick[0] == 0 ? 12 : pVStiff->m_dThick[0];
	double VW  = pVStiff->m_dWidth[0] == 0 ? 150 : pVStiff->m_dWidth[0];

	// 수평 부재 폭과 두께.
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxHs = Finder.GetBxLast(BX_PLATEHSTIFF, NULL, pBx);
	double HT   = 12;
	double HW   = 140;
	CWebHStiff  *pHs = pBxHs->GetWebHStiff();
	if (pHs)
	{
		if(max(pHs->m_dThickHStiff[0], pHs->m_dThickHStiff[1]) > 0)
		{
			HT   = max(pHs->m_dThickHStiff[0], pHs->m_dThickHStiff[1]); 
			HW   = max(pHs->m_dWidthHStiff[0], pHs->m_dWidthHStiff[1]);
		}
	}

	double B    = pBridge->GetGirder(-1)->m_pSangse->m_HStiff_VStiff_B;	// 수평보강재와 수직보강재의 간격 
	double D    = pBridge->GetGirder(-1)->m_pSangse->m_HStiff_VStiff_D;	// 복부판측 직선길이
	double WebT = pBx->GetFactChain(G_W)->m_dFactChainThick;

	// 상세 DRAWING의 기준점은 수직보강재의 상단 중심(복부판 접촉면)
	CPlateBasicIndex	*pBxSp	= Finder.GetBxFirst(BX_SPLICE);
	CPlateSplice		*pSp	= pBxSp ? pBxSp->GetSplice() : NULL;
	double Len  = 0; // 각각FIX
	double SpliceW  = 505;
	if(pSp)
	{
		SpliceW = (pSp->m_mB * pSp->m_mC) == 0 ? 505
			: (pSp->m_mA + pSp->m_mB * pSp->m_mC + pSp->m_mC1/2) * 2;
	}
	
	bDrawDetailDlg ? Len = HW*2+SpliceW : Len = 350 ;

	double wing = pDom->Always(5);
	double Ang  = 45;
	pOptStd->SetEnvType(pDom, HCAD_STLC);
	pDom->LineTo(-Len,WebT ,Len,WebT);	// Web PALTE
	pDom->LineTo(-Len,   0 ,Len,   0);
	pDom->Rectangle(-VT/2,0,VT/2,-VW);	// 수직보강재
	pDom->LineTo(-Len,-HW, -(VT/2+B+(HW-D)),-HW);	// 수평보강재(좌)
	pDom->LineTo(-(VT/2+B+(HW-D)),-HW,
		         -(VT/2+B),-D); 
	pDom->LineTo(-(VT/2+B), 0, -(VT/2+B),-D); 
	pDom->LineTo(Len,-HW, (VT/2+B+(HW-D)),-HW);	// 수평보강재(우)
	pDom->LineTo((VT/2+B+(HW-D)),-HW,
		         (VT/2+B),-D); 
	pDom->LineTo((VT/2+B), 0, (VT/2+B),-D);
	pOptStd->SetEnvType(pDom, HCAD_CUTL);
	pDom->CutLightning(-Len, -HW-wing, -Len, WebT+wing);	// 절단선
	pDom->CutLightning( Len, -HW-wing,  Len, WebT+wing);	// 절단선

	////////////////////////////////////////////////////////////////////
	pOptStd->SetEnvType(pDom, HCAD_DIML);
	pDom->SetScaleDim(Scale);
	if (bDrawDetailDlg)
	{
		pDom->SetDirection("RIGHT");
		pDom->DimMoveTo(VT/2+B,-D,0);
		pDom->DimLineToOverExtend(D,0,"D",COMMA(D));

		pDom->SetDirection("BOTTOM");
		pDom->DimMoveTo(VT/2,-D,0);
		pDom->DimLineToExtend(B,0,"B",COMMA(B));

	}
	else
	{
		pDom->SetDirection("TOP");
		pDom->DimMoveTo(-VT/2-B,WebT,0,COMMA(B));
		pDom->DimLineTo(B,0,"");
		pDom->DimLineTo(VT,0,COMMA(VT));
		pDom->DimLineToOver(B,0,COMMA(B));

		pDom->SetDirection("RIGHT");
		pDom->DimMoveTo(Len,-HW,0);
		pDom->DimLineTo(HW-D,0,COMMA(HW-D));
		pDom->DimLineToOver(D,0,COMMA(D));
		pDom->DimMoveTo(Len,-HW,1);
		pDom->DimLineTo(HW,1,COMMA(HW));

/*		pDom->SetDirection("BOTTOM");
		pDom->DimMoveTo(-VT/2-B-(HW-D),-HW,0);
		pDom->DimLineTo(HW-D,0,COMMA(HW-D));
		pDom->DimLineTo(B+VT+B,0,COMMA(B+VT+B));
		pDom->DimLineTo(HW-D,0,COMMA(HW-D));
*/
		pDom->SetDirection("TOP");
		CDPoint xy, xy1;
		xy = CDPoint(-VT/2-B, -D);
		xy1 = CDPoint(-VT/2-B-(HW-D),-HW);
		double R = HW-D;//~(xy - xy1);
		pDom->DimArcMoveTo(xy.x,xy.y,R,xy1.x,xy1.y,1);
		pDom->DimArcLineTo(xy.x, xy.y - (HW-D),1,COMMA(Ang));
		pDom->SetDirection("BOTTOM");
		pDom->DimMoveTo(xy.x,xy.y - (HW-D),1);
		pDom->DimLineTo(VT+B*2,1,COMMA(VT+B*2));
		pDom->SetDirection("TOP");
		xy1 = CDPoint(VT/2+B, -D);
		xy = CDPoint(VT/2+B+(HW-D),-HW);
		R = HW-D;//~(xy - xy1);
		pDom->DimArcMoveTo(xy1.x,xy1.y,R,xy1.x,xy1.y-(HW-D),1);
		pDom->DimArcLineTo(xy.x, xy.y,1,COMMA(Ang));
	}
	CString	str = "수평보강재";
	double gisilen = pDom->GetTextWidth(str);
	pDom->GiSiArrowExtend(-Len/2, -WebT, gisilen, -gisilen, 3,str, _T(""));
	str = "수직보강재";
	gisilen = pDom->GetTextWidth(str);
	pDom->GiSiArrowExtend(0, -WebT-HW/2,	gisilen, gisilen, 1, str, _T(""));

	*pDomP << *pDom;
	delete pDom;
}

/////////////////////////////////////////////////////////////////////////////////
// 수평보강재 : 횡리브 Rib Bx로 수정 
// nSide = -1 : 좌측  1: 우측 
void CAPlateDrawSebu::AddHStiffDetail_WithHRib(CDomyun *pDomP, CPlateBasicIndex *pBx, long nSide, double DimScale)
{
	/*
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir = pBx->GetGirder();
	CDanmyun		*pDan = pBx->GetDanmyun();

	if(pDan==NULL) return;

	CDomyun* pDom = new CDomyun(pDomP);
	pDom->SetScaleDim(DimScale);

	// HRIB 보강재 제원
	double VT  = pDan->m_uRib.m_W2;
	double VW  = 200;

	// 수평 부재 폭과 두께.
	double HW   = 12;
	double HT   = 140;
	CWebHStiff *pHs = pBx->GetWebHStiff(nSide);
	if (pHs)
	{
		for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
		{
			if(pHs->IsEmpty(nDan))	continue;
			HW  = pHs->m_dWidthHStiff;
			HT  = pHs->m_dThickHStiff;
			break;
		}
	}
	double B    = pBridge->GetGirder(-1)->m_pSangse->m_HStiff_VStiff_B;	// 수평보강재와 수직보강재의 간격 
	double D    = pBridge->GetGirder(-1)->m_pSangse->m_HStiff_VStiff_D;	// 복부판측 직선길이
	long nMarkBuje = nSide == -1 ? G_W : G_W_R;
	double WebT = pBx->GetFactChain(nMarkBuje)->m_dFactChainThick;

	// 상세 DRAWING의 기준점은 수직보강재의 상단 중심(복부판 접촉면)
	double Len  = 350; // 각각FIX
	double wing = pDom->Always(5);
	double Ang  = 45;
	pOptStd->SetEnvType(pDom, HCAD_STLC);
	pDom->LineTo(-Len,WebT ,Len,WebT);	// Web PALTE
	pDom->LineTo(-Len,   0 ,Len,   0);

	pDom->LineTo(-VT/2,0,-VT/2,-VW);	// 횡리브
	pDom->LineTo(   -6,0,   -6,-VW);
	pDom->LineTo(    6,0,    6,-VW);
	pDom->LineTo( VT/2,0, VT/2,-VW);

	pDom->LineTo(-Len,-HW, -(VT/2+B+(HW-D)),-HW);	// 수평보강재(좌)
	pDom->LineTo(-(VT/2+B+(HW-D)),-HW,
		         -(VT/2+B),-D); 
	pDom->LineTo(-(VT/2+B), 0, -(VT/2+B),-D); 
	pDom->LineTo(Len,-HW, (VT/2+B+(HW-D)),-HW);	// 수평보강재(우)
	pDom->LineTo((VT/2+B+(HW-D)),-HW,
		         (VT/2+B),-D); 
	pDom->LineTo((VT/2+B), 0, (VT/2+B),-D);

	pOptStd->SetEnvType(pDom, HCAD_CUTL);
	pDom->CutLightning(-Len, -HW-wing, -Len, WebT+wing);	    // 절단선
	pDom->CutLightning( Len, -HW-wing,  Len, WebT+wing);	    // 절단선
	pDom->CutLightning(-VT/2-wing, -VW, VT/2+wing, -VW,FALSE);	// 절단선

	////////////////////////////////////////////////////////////////////
	pOptStd->SetEnvType(pDom, HCAD_DIML);
	pDom->SetDirection("TOP");
	pDom->DimMoveTo(-VT/2-B,WebT,0,COMMA(B));
	pDom->DimLineTo(B,0,"");
	pDom->DimLineTo(VT,0,COMMA(VT));
	pDom->DimLineToOver(B,0,COMMA(B));

	pDom->SetDirection("RIGHT");
	pDom->DimMoveTo(Len,-HW,0);
	pDom->DimLineTo(HW-D,0,COMMA(HW-D));
	pDom->DimLineToOver(D,0,COMMA(D));
	pDom->DimMoveTo(Len,-HW,1);
	pDom->DimLineTo(HW,1,COMMA(HW));

/*	pDom->SetDirection("BOTTOM");
	pDom->DimMoveTo(-VT/2-B-(HW-D),-HW,0);
	pDom->DimLineTo(HW-D,0,COMMA(HW-D));
	pDom->DimLineTo(B+VT+B,0,COMMA(B+VT+B));
	pDom->DimLineTo(HW-D,0,COMMA(HW-D)); * /

	pDom->SetDirection("TOP");
	CDPoint xy, xy1;
	xy = CDPoint(-VT/2-B, -D);
	xy1 = CDPoint(-VT/2-B-(HW-D),-HW);
	double R = HW-D;//~(xy - xy1);
	pDom->DimArcMoveTo(xy.x,xy.y,R,xy1.x,xy1.y,1);
	pDom->DimArcLineTo(xy.x, xy.y - (HW-D),1,COMMA(Ang));
	pDom->SetDirection("BOTTOM");
	pDom->DimMoveTo(xy.x,xy.y - (HW-D),1);
	pDom->DimLineTo(VT+B*2,1,COMMA(VT+B*2));
	pDom->SetDirection("TOP");
	xy1 = CDPoint(VT/2+B, -D);
	xy = CDPoint(VT/2+B+(HW-D),-HW);
	R = HW-D;//~(xy - xy1);
	pDom->DimArcMoveTo(xy1.x,xy1.y,R,xy1.x,xy1.y-(HW-D),1);
	pDom->DimArcLineTo(xy.x, xy.y,1,COMMA(Ang));

	pDom->GiSiArrowExtend(-Len/2, -pDom->Always(5),
						pDom->Always(13), -pDom->Always(28),
						3, _T("수평보강재"), _T(""));
	pDom->GiSiArrowExtend(pDom->Always(5), -pDom->Always(5),
						pDom->Always(13), pDom->Always(28),
						1, _T("횡 리브"), _T(""));

	*pDomP << *pDom;
	delete pDom;
	*/
}

void CAPlateDrawSebu::AddHStiffDetail_WithSplice(CDomyun *pDomP, CPlateBasicIndex *pBx, double Scale, BOOL bDrawDetailDlg)
{	
	/*
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	= (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	*pGirCen= m_pDataMng->GetBridge()->GetGirder(-1);
	CPlateSplice *pSp		= pBx->GetSplice();
	CDomyun* pDom		= new CDomyun(pDomP);

	// 이음판 제원
	double SpliceT  = (pSp->m_mB * pSp->m_mC) == 0 ? 10 : pSp->m_mT;	
	double SpliceW  = (pSp->m_mB * pSp->m_mC) == 0 ? 505 : 
					  (pSp->m_mA + pSp->m_mB * pSp->m_mC + pSp->m_mC1/2) * 2;

	// 수평보강재 폭과 두께.
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxHs  = Finder.GetBxLast(BX_PLATEHSTIFF_LEFT,NULL,pBx);
	double HT   = 12;
	double HW   = 140;
	CWebHStiff  *pHs = pBxHs->GetWebHStiff(-1);
	if (pHs)
	{
		if(pHs->m_dThickHStiff > 0)
		{
			HT   = pHs->m_dThickHStiff; 
			HW   = pHs->m_dWidthHStiff;
		}
	}

	double S    = pGirCen->m_pSangse->m_HStiff_Splice_B;	// 수평보강재와 수직보강재의 간격 
	double D    = pGirCen->m_pSangse->m_HStiff_VStiff_D;	// 복부판측 직선길이
	double WebT = pBxHs->GetFactChain(3)->m_dFactChainThick;

	// 상세 DRAWING의 기준점은 수직보강재의 상단 중심(복부판 접촉면)
	double Len  = HW*2+SpliceW; // 각각FIX
	double wing = pDom->Always(5);
	double Ang  = 45;
	pOptStd->SetEnvType(pDom, HCAD_STLC);
	pDom->LineTo(-Len,WebT ,Len,WebT);	// Web PALTE
	pDom->LineTo(-Len,   0 ,Len,   0);
	pDom->Rectangle(-SpliceW/2, SpliceT+WebT ,SpliceW/2, WebT);	// SPLICE PLATE
	pDom->Rectangle(-SpliceW/2,-SpliceT ,SpliceW/2, 0);

	///////////
	pDom->LineTo(-Len,-HW, -(SpliceW/2+S+(HW-D)),-HW);	// 수평보강재(좌)
	pDom->LineTo(-(SpliceW/2+S+(HW-D)),-HW,
		         -(SpliceW/2+S),-D); 
	pDom->LineTo(-(SpliceW/2+S), 0, -(SpliceW/2+S),-D); 
	pDom->LineTo(Len,-HW, (SpliceW/2+S+(HW-D)),-HW);	// 수평보강재(우)
	pDom->LineTo((SpliceW/2+S+(HW-D)),-HW,
		         (SpliceW/2+S),-D); 
	pDom->LineTo((SpliceW/2+S), 0, (SpliceW/2+S),-D);

	// 볼트그리기 : 평면도 상세와 일치하여야함
	// 6개를 그리는 것으로 가정하여 돌린다.............
	double mA = pSp->m_mA == 0 ? 40 : pSp->m_mA;
	double mC = pSp->m_mC == 0 ? 85 : pSp->m_mC;
	double sx	 = -SpliceW/2+mA;		//-200
	double boltT =   18;
	double boltL =   65;
	double boltHeadW = 30;
	double boltHeadT = 10;
	pOptStd->SetEnvType(pDom,HCAD_STLS);
	for(long n=0; n<3; n++) {
		if(n>0) sx += mC;		//75
		pOptStd->SetEnvType(pDom,HCAD_STLS);
		pDom->Rectangle(sx-boltHeadW/2,SpliceT+WebT,sx+boltHeadW/2,SpliceT+WebT+boltHeadT);
		pDom->Rectangle(sx-boltT/2,SpliceT+WebT,sx+boltT/2,-boltL);
		pDom->Rectangle(sx-boltHeadW/2,-SpliceT,sx+boltHeadW/2,-SpliceT-boltHeadT);
		pOptStd->SetEnvType(pDom,HCAD_CENT);
		pDom->LineTo(sx,SpliceT+WebT+boltHeadT*2,sx,-boltL-boltHeadT);
	}
	sx = SpliceW/2-mA;		//200
	for(n=0; n<3; n++) {
		if(n>0) sx -= mC;		//75
		pOptStd->SetEnvType(pDom,HCAD_STLS);
		pDom->Rectangle(sx-boltHeadW/2,SpliceT+WebT,sx+boltHeadW/2,SpliceT+WebT+boltHeadT);
		pDom->Rectangle(sx-boltT/2,SpliceT+WebT,sx+boltT/2,-boltL);
		pDom->Rectangle(sx-boltHeadW/2,-SpliceT,sx+boltHeadW/2,-SpliceT-boltHeadT);
		pOptStd->SetEnvType(pDom,HCAD_CENT);
		pDom->LineTo(sx,SpliceT+WebT+boltHeadT*2,sx,-boltL-boltHeadT);
	}

	// 절단선
	pOptStd->SetEnvType(pDom, HCAD_CUTL);
	pDom->CutLightning(-Len, -HW-wing, -Len, WebT+wing);
	pDom->CutLightning( Len, -HW-wing,  Len, WebT+wing);
	////////////////////////////////////////////////////////////////////
	pOptStd->SetEnvType(pDom, HCAD_DIML);
	pDom->SetScaleDim(Scale);
	CString	str = "수평보강재";
	double gisilen = pDom->GetTextWidth(str);
	pDom->GiSiArrowExtend(-Len/2, -WebT,gisilen, -gisilen,3, str, _T(""));
	str = "복부 이음판";
	gisilen = pDom->GetTextWidth(str);
	pDom->GiSiArrowExtend(SpliceW/2-pDom->Always(1), WebT+SpliceT, gisilen, gisilen,1, str, _T(""));
	pDom->SetDirection("BOTTOM");
	pDom->DimMoveTo(-(SpliceW/2+S),0,0);

	if(bDrawDetailDlg)
		pDom->DimLineToOverExtend(S,0,"S",COMMA(S));
	else
		pDom->DimLineToOver(S,0,COMMA(S));

	*pDomP << *pDom;
	delete pDom;
	*/
}

// DRAWING 원점 : 슈블럭의 중심하단 (중심은 슈의 중심을 기준으로 한다)
void CAPlateDrawSebu::DrawShoeBoxDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX, double BY, 
									double dFactorThick, BOOL bLongitude, double dBogus)
{
	CPlateGirderApp *pGir	= (CPlateGirderApp *)pBx->GetGirder();
	CShoe *pShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx));

	CDPoint sp,ep;	 

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
	
	if(dBogus != 0) dBogus = pGir->GetThickJewon(G_F_L,pBx) - dBogus;
	CDomyun* pDom = new CDomyun(pDomP);

	// 슈 블럭
	sp.x = -blockW1-sH3*2-dWidthShoe/2;	
	sp.y = 0;	
	ep.x = sH3*2+blockW2+dWidthShoe/2;	
	ep.y = sH4 + dBogus;
	pDom->Rectangle(sp,ep);

	// 몰탈
	sp.x = -sH3*2-dWidthShoe/2;	
	sp.y = sH4 + dBogus;
	ep.x = sH3*2+dWidthShoe/2;
	ep.y = sH4 + dBogus;
	pDom->MoveTo(sp);
	pDom->LineTo(sp.x+sH3,sp.y+sH3);
	pDom->LineTo(ep.x-sH3,ep.y+sH3);
	pDom->LineTo(ep);
	pDom->LineTo(sp);
	
	// 슈 
	sp.x = -dWidthShoe/2;
	sp.y = sH4 + dBogus + sH3;
	ep.x = dWidthShoe/2;
	ep.y = sH4 + dBogus + sH3 + sH2/2;
	pDom->Rectangle(sp,ep);
//	pDST->DstHatchInRect(pDom,sp,ep,30);
	CDRect rect;
	rect.left   = sp.x;
	rect.right  = ep.x;
	rect.bottom = sp.y;
	rect.top    = ep.y;
	pDom->HatchRect(rect, "ANSI31", pDomP->GetScaleDim());
	//
	sp.x = -dWidthShoe/2+30;
	sp.y = sH4 + dBogus + sH3 + sH2/2;
	ep.x = dWidthShoe/2-30;
	ep.y = sH4 + dBogus + sH3 + sH2*3/4;
	pDom->Rectangle(sp,ep);
	//
	sp.x = -dWidthShoe/2;
	sp.y = sH4 + dBogus + sH3 + sH2*3/4;
	ep.x = dWidthShoe/2;
	ep.y = sH4 + dBogus + sH3 + sH2;
	pDom->Rectangle(sp,ep);
//	pDST->DstHatchInRect(pDom,sp,ep,30);
	rect.left   = sp.x;
	rect.right  = ep.x;
	rect.bottom = sp.y;
	rect.top    = ep.y;
	pDom->HatchRect(rect, "ANSI31", pDomP->GetScaleDim());

	// 솔플레이트		
	double JSlop = pGir->GetGiulJongDanShoe(pBx);			//종단 경사
	double T1	 = bLongitude ? sH1 - JSlop*dWidthPlate/2 : sH1;//솔플레이트 좌측면 두께
	double T2	 = bLongitude ? sH1 + JSlop*dWidthPlate/2 : sH1;//솔플레이트 우측면 두께
	
	sp.x = -dWidthPlate/2;
	sp.y = sH4 + dBogus + sH3 + sH2;
	ep.x = dWidthPlate/2;
	ep.y = sH4 + dBogus + sH3 + sH2;
	pDom->MoveTo(sp);
	pDom->LineTo(sp.x,sp.y+T1);
	pDom->LineTo(ep.x,ep.y+T2);
	pDom->LineTo(ep);
	pDom->LineTo(sp);

	pDom->Move(BX,BY);
	*pDomP << *pDom;
	delete pDom;
}

// 확장 스캘럽표현하기
// Length : 부재의 길이 - 복부판의 외측에서 스캘럽ARC의 중심위치 (X좌표거리)
// Thick1 : Length 부재의 두께
// Thick2 : 연결되는 부재의 두께
// WebT   : 수직부재의 두께
// 그리는 원점은 스캘럽의 중심위치(0,0)
// 기울기는 좌에서우로 진행
void CAPlateDrawSebu::DrawScallopExtend(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX, double BY, double ScallopR, 
										   double Length, 
										   double Slop1,  double Slop2, 
										   double Thick1, double Thick2, 
										   double WebT, BOOL bLeft, BOOL bExtend, long nScallop, BOOL bStringer, BOOL bShop)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir = (CPlateGirderApp*)m_pDataMng->GetBridge();
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();
	if(Length<=0 || ScallopR<=0) return;

	CDomyun *pDom = new CDomyun(pDomP);

	double C = 10;
	double sw = bLeft ? 1.0 : -1.0;
	double WeldIntv = 2;
	double Radius = Length*3/2;
	CDPoint vCen(Length/2*sw, 0);
	CDPoint xyResult1(0,0),xyResult2(0,0);
	CDPoint sttCp(0,0), endCp(0,0);

	double SttAng  = 270;
	double AngPlus = ToDegree(atan(Slop2));
	double AngPlus2 = ToDegree(atan(Slop1));
	if(bLeft) SttAng = 180 + AngPlus;

	if (bExtend)
	{
		pDom->LineTo(0, -ScallopR, Length*sw, -ScallopR + Length*sw*Slop1);
		pDom->Arc(0, 0,  ScallopR, SttAng, 90-AngPlus*sw);
	}
	else
	{
		if (nScallop == 0)
		{
			pDom->Arc(Length*sw, Length*sw*Slop1,  ScallopR, 270, 90*-sw+AngPlus2);
			pDom->LineTo(Length*sw-ScallopR*sw, (Length*sw-ScallopR*sw)*Slop1, ScallopR*sw,ScallopR*sw*Slop1);
			pDom->Arc(0, 0,  ScallopR, SttAng, 90-AngPlus*sw);
			pDom->Arc(0, 0,  ScallopR, 270, 90*sw+AngPlus2);
		}
		else if (nScallop == 1 || nScallop == 2)
		{
			pDom->MoveTo(Length*sw - C*sw*cos(ToRadian(AngPlus2)), (Length*sw - C*sw*cos(ToRadian(AngPlus2)))*Slop1);
			pDom->LineTo(Length*sw, -C+Length*sw*Slop1);
			pDom->LineTo(Length*sw-C*sw, (Length*sw-C*sw)*Slop1, ScallopR*sw,ScallopR*sw*Slop1);
			pDom->Arc(0, 0,  ScallopR, SttAng, 90-AngPlus*sw);
			pDom->Arc(0, 0,  ScallopR, 270, 90*sw+AngPlus2);
		}
		else
		{
			pDom->Arc(0, 0,  ScallopR, SttAng, 90-AngPlus*sw);
			pDom->Arc(0, 0,  ScallopR, 270, 90*sw+AngPlus2);
			pDom->LineTo(Length*sw, Length*sw*Slop1, ScallopR*sw,ScallopR*sw*Slop1);
		}
	}

	if(Thick1>0 && Thick2>0 && WebT >0)
	{
		// 상부부재 그리기
/*		if(dLH>0)
		{
			sttCp = CDPoint(-dSharp*-sw,-dLH);
			endCp = CDPoint(Length*-sw,-dLH);
			GetXyMatchSegAngArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);

			pDom->LineTo(sttCp, endCp);
			sttCp = CDPoint(dSharp*-sw,-dLH-Thick2);
			endCp = CDPoint(Length*-sw,-dLH-Thick2);
			GetXyMatchSegAngArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);
			pDom->LineTo(sttCp, endCp);
		//	pDom->LineTo(WeldIntv*-sw,WeldIntv*Slop2*-sw, (WeldIntv+Thick2)*-sw,(WeldIntv+Thick2)*Slop2*-sw+Thick2);
		}
		else*/
		{
			sttCp = CDPoint(WeldIntv*-sw,WeldIntv*Slop2*-sw);
			endCp = CDPoint(Length*-sw,Length*Slop2*-sw);
			GetXyMatchSegAndArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);

			pDom->LineTo(WeldIntv*-sw,WeldIntv*Slop2*-sw, xyResult1.x,xyResult1.y);
			sttCp = CDPoint((WeldIntv+Thick2)*-sw,(WeldIntv+Thick2)*Slop2*-sw+Thick2);
			endCp = CDPoint(Length*-sw,Length*Slop2*-sw+Thick2);
			GetXyMatchSegAndArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);
			pDom->LineTo((WeldIntv+Thick2)*-sw,(WeldIntv+Thick2)*Slop2*-sw+Thick2, xyResult1.x,xyResult1.y);
			pDom->LineTo(WeldIntv*-sw,WeldIntv*Slop2*-sw, (WeldIntv+Thick2)*-sw,(WeldIntv+Thick2)*Slop2*-sw+Thick2);
		}
		//
		if (bShop)
			pOptStd->SetEnvType(pDom,HCAD_MIS1);
		// 상부부재 그리기
		sttCp = CDPoint(0,0);
		endCp = CDPoint(Length*2.0*sw, Slop1*sw*Length*2.0);
		GetXyMatchSegAndArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);
		pDom->LineTo(0,0,      xyResult1.x, xyResult1.y);

		sttCp = CDPoint(0,Thick1);
		endCp = CDPoint(Length*2.0*sw, Slop1*sw*Length*2.0+Thick1);
		GetXyMatchSegAndArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);
		pDom->LineTo(0,Thick1, xyResult1.x, xyResult1.y);
		pDom->LineTo(0,Thick1, 0,0);

		// 수직부재 그리기
		sttCp = CDPoint(Length*sw,Length*Slop1*sw);
		endCp = CDPoint(Length*sw,-Length*0.8);
		GetXyMatchSegAndArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);
		pDom->LineTo( Length*sw,Length*Slop1*sw, 
			          xyResult1.x,xyResult1.y);
		sttCp = CDPoint((Length+WebT)*sw,(Length+WebT)*Slop1*sw);
		endCp = CDPoint((Length+WebT)*sw,-Length*0.8);
		GetXyMatchSegAndArc(sttCp, endCp, vCen, Radius, 170, 200, xyResult1,xyResult2);
		pDom->LineTo((Length+WebT)*sw,(Length+WebT)*Slop1*sw, 
			         xyResult1.x,xyResult1.y);

	}

	if (bShop)
	{
		//CERAMIC BAR
		double CeramicBarW = 40;
		double CeramicBarH = 20;
		pDom->Rectangle(-CeramicBarW/2, -CeramicBarH, CeramicBarW/2, 0);
		pDom->HatchInQuadrAngle(CDPoint(-CeramicBarW/2, 0), CDPoint(CeramicBarW/2,0), CDPoint(-CeramicBarW/2, -CeramicBarH), CDPoint(CeramicBarW/2, -CeramicBarH), 10);
	}

	pOptStd->SetEnvType(pDom,HCAD_MIS1);
	pDom->Arc(Length/2*sw, 0, Radius, 170, 200);
	pDom->Move(BX,BY);
	*pDomP << *pDom;
	delete pDom;
}