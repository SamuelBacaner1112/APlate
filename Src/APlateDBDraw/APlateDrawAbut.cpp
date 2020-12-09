// APlateDrawAbut.cpp: implementation of the CAPlateDrawAbut class.
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

CAPlateDrawAbut::CAPlateDrawAbut(CDataManage *pDataMng)
{
	m_pDataMng = pDataMng;
}

CAPlateDrawAbut::~CAPlateDrawAbut()
{

}
void CAPlateDrawAbut::DrawAbutOrPier(CDomyun *pDom, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(pDom,HCAD_SMALL_TITLE);
	
	if(pBridge->IsAbutOnJiJum(nJijum))	
	{
		DrawAbut(&Dom, pBridge->m_PierInfo + nJijum);
	}
	else 
	{
		DrawPier(&Dom, pBridge->m_PierInfo + nJijum);
	}
	*pDom << Dom;
}

void CAPlateDrawAbut::DrawAbut(CDomyun *pDom, CPierInfo *pPier)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);

	Dom.SetScaleDim(75);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);	

	// 교대  단면도
	double dT1 = pPier->m_daT1;
	double dT2 = pPier->m_daT2;
	double dT3 = pPier->m_daT3;
	double dT4 = pPier->m_daT4;
	double dH1 = pPier->m_daH1;
	double dH2 = pPier->m_daH2;
	double dH3 = pPier->m_daH3;
	double dH4 = pPier->m_daH4;
	double dH5 = pPier->m_daH5;
	double dH6 = pPier->m_daH6;
	double dH7 = pPier->m_daH7;
	double dH8 = pPier->m_daH8;
	double dB1 = pPier->m_daB1;
	double dB2 = pPier->m_daB2;
	double dB3 = pPier->m_daB3;
	double dK1 = pPier->m_daK1;
	double dK2 = pPier->m_daK2;
	double dD = dT4 + dT2 + dT1 - dT3 - dB2;

	double dTotH = dH1 + dH2 + dH3 + dH4;
	double dH9 = dTotH - dH6 - dH7 - dH8;

	double dW = pPier->m_daW;
	double dW1 = pPier->m_daW1;
	double dW2 = pPier->m_daW2;
	double dW3 = dTotH - dW1 - dW2;
	double dW4 = dW - (dB1 - dD) - dT1 - dT2;	

	double dX = dK2, dY = dK1;

	// 단면도
	// 기초
	Dom.Rectangle(0,0,dB1+dB2+dB3+dK2*2,dK1);	
	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX,dY+=dH5);
	Dom.LineTo(dX+=dB1,dY);
	Dom.LineTo(dX,dY+=dH9);
	Dom.LineTo(dX-=dD,dY+=dH8);
	Dom.LineTo(dX,dY+=dH7);
	Dom.LineTo(dX+=dT1,dY);
	Dom.LineTo(dX,dY+=dH6);
	Dom.LineTo(dX+=dT2,dY);
	Dom.LineTo(dX,dY-=dH1);
	Dom.LineTo(dX-=dT3,dY-=dH2);
	Dom.LineTo(dX,dY-=dH3);
	Dom.LineTo(dX+=dT4,dY);
	Dom.LineTo(dX,dY-=dH4);
	Dom.LineTo(dX+=dB3,dY);
	Dom.LineTo(dX,dY-=dH5);

	dX = dB1 - dD + dT1 + dT2 + dK2;
	dY = dTotH + dH5 + dK1;

	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX-=dW,dY);
	Dom.LineTo(dX,dY-=dW1);
	Dom.LineTo(dX+=dW4,dY-=dW2);
	Dom.LineTo(dX,dY-=dW3);

	dX = dB1 - dD + dT1 + dT2 + dK2;
	dY = dTotH + dH5 + dK1;

	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dX,dY,0);
	Dom.DimLineTo(-dT2,0,"T2");	
	Dom.DimLineTo(-dT1,0,"T1");	
	Dom.DimLineTo(-(dW-dT1-dT2),0,COMMA(dW-dT1-dT2));	

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(dB1 - dD + dT1 + dT2 + dK2,dTotH + dH5 + dK1 - dH1 - dH2,0);
	Dom.DimLineTo(-dT3,0,"T3");

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dB1 - dD + dT1 + dT2 + dK2 - dT3,dTotH + dH5 + dK1 - dH1 - dH2 - dH3,0);
	Dom.DimLineTo(dT4,0,"T4");				

	Dom.DimMoveTo(dX,dY,1);
	Dom.DimLineTo(-dW,1,"W");	
	
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dX,dY,0);
	Dom.DimLineTo(-dH1,0,"H1");	
	Dom.DimLineTo(-dH2,0,"H2");	
	Dom.DimLineTo(-dH3,0,"H3");	

	Dom.DimMoveTo(dK2*2 + dB1 + dB2 + dB3,0,0);
	Dom.DimLineTo(dK1,0,"K1");	
	Dom.DimLineTo(dH5,0,"H5");	
	Dom.DimLineTo(dH4,0,"H4");	

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dK2,dH5+dK1,0);
	Dom.DimLineTo(dB1,0,"B1");
	Dom.DimLineTo(dB2,0,"B2");
	Dom.DimLineTo(dB3,0,"B3");

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(0,0,0);
	Dom.DimLineTo(dK2,0,"K2");
	Dom.DimLineTo(dB1+dB2+dB3,0,COMMA(dB1+dB2+dB3));
	Dom.DimLineTo(dK2,0,"K2");

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(dB1 - dD,dTotH + dH5 + dK1,0);
	Dom.DimLineTo(-dH6,0,"H6");
	Dom.DimLineTo(-dH7,0,"H7");
	Dom.DimLineTo(-dH8,0,"H8");
	Dom.DimLineTo(-dH9,0,COMMA(dH9));
	
	Dom.DimMoveTo(dB1 - dD + dT1 + dT2 + dK2 - dW,dTotH + dH5 + dK1,0);
	Dom.DimLineTo(-dW1,0,"W1");
	Dom.DimLineTo(-dW2,0,"W2");
	Dom.DimLineTo(-dW3,0,COMMA(dW3));	
	
	*pDom << Dom;
}

void CAPlateDrawAbut::DrawPier(CDomyun *pDom, CPierInfo *pPier)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
		
	CDomyun Dom(pDom);
	
	Dom.SetScaleDim(75);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	
	double dB1 = pPier->m_dpB1;
	double dB2 = pPier->m_dpB2;
	double dB3 = pPier->m_dpB3;
	double dB4 = pPier->m_dpB4;
	double dD = pPier->m_dpD;
	double dD2_P = pPier->m_dpP;		
	double dD2_N = pPier->m_dpN;		
	double dW = pPier->m_dpW;
	double dH1 = pPier->m_dpH1;
	double dH2 = pPier->m_dpH2;
	double dK1 = pPier->m_dpK1;
	double dK2 = pPier->m_dpK2;
	double dH3 = pPier->m_dpH3;
	double dH4 = pPier->m_dpH4;
	double dR  = pPier->m_dpR;

	double dX = dK1, dY = dK2;

	// 기초 그리기
	Dom.Rectangle(0,0,dB1+dD+dB2+dK1*2,dK2);	
	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX,dY+=dH4);
	Dom.LineTo(dX+=dB1+dB2+dD,dY);
	Dom.LineTo(dX,dY-=dH4);

	dX = dK1 + dB1, dY = dK2 + dH4;
	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX,dY+=dH3);
	Dom.LineTo(dX-=dW/2+dR-dD/2,dY+=dH2);
	Dom.LineTo(dX,dY+=dH1);
	Dom.LineTo(dX+=dW+dR*2,dY);
	Dom.LineTo(dX,dY-=dH1);
	Dom.LineTo(dX-=dW/2+dR-dD/2,dY-=dH2);		
	Dom.LineTo(dX-dD,dY);
	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX,dY-dH3);
	
	if(dR!=0)
	{
		dX = dK1 + (dB1 + dB2 + dD)/2  - dW/2 - dR, dY = dK2 + dH1 + dH2 + dH3 + dH4;
		for(long i=0;i<=5;i++)
		{
			Dom.MoveTo(dX+dR/5*i,dY);
			Dom.LineTo(dX+dR/5*i,dY-dH1);
			Dom.MoveTo(dX+dW+dR*2-dR/5*i,dY);
			Dom.LineTo(dX+dW+dR*2-dR/5*i,dY-dH1);
		}
	}
	
	dX = dK1 + 20000, dY = dK2;
	Dom.Rectangle(20000,0,20000+dB3+dD2_P+dD2_N+dB4+dK1*2,dK2);			
	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX,dY+=dH4);
	Dom.LineTo(dX+=dB3+dB4+dD2_P+dD2_N,dY);
	Dom.LineTo(dX,dY-=dH4);

	if(dR!=0)
	{
		dX = 20000 + dK1 + (dB3+dD2_P+dD2_N+dB4)/2 - (dD2_P+dD2_N)/2, dY = dK2 + dH1 + dH2 + dH3 + dH4;
		for(long i=0;i<=10;i++)
		{
			Dom.MoveTo(dX+(dD2_P+dD2_N)/10*i,dY);
			Dom.LineTo(dX+(dD2_P+dD2_N)/10*i,dY-dH1);
			Dom.MoveTo(dX+dD2_P+dD2_N-(dD2_P+dD2_N)/10*i,dY);
			Dom.LineTo(dX+dD2_P+dD2_N-(dD2_P+dD2_N)/10*i,dY-dH1);
		}
	}
	
	dX = dK1 + dB1 + 20000, dY = dK2 + dH4;
	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX,dY+=dH3+dH1+dH2);			
	dY -= dH4;
	for(long i=0;i<5;i++)
	{
		Dom.LineTo(dX,dY+dH2/4*i,dX+dD2_P+dD2_N,dY+dH2/4*i);
	}
	dY += dH4;
	Dom.MoveTo(dX,dY);
	Dom.LineTo(dX+=dD2_P+dD2_N,dY);
	Dom.LineTo(dX,dY-=dH3+dH1+dH2);

	// 치수기입
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	dX = dK1 + (dB1 + dB2 + dD)/2  - dW/2, dY = dK2 + dH1 + dH2 + dH3 + dH4;
	Dom.SetDirection("TOP");
	if(dR!=0)
	{
		Dom.DimMoveTo(dX,dY,0);		
		Dom.DimLineTo(-dR,0,"X");
	}
	Dom.DimMoveTo(dX,dY,0);		
	Dom.DimLineTo(dW,0,"W");
	if(dR!=0)
	{
		Dom.DimMoveTo(dX+dW,dY,0);		
		Dom.DimLineTo(dR,0,"X");
	}
	
	
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(dX,dY,0);
	Dom.DimLineTo(-dH1,0,"H1");
	Dom.DimLineTo(-dH2,0,"H2");
	Dom.DimLineTo(-dH3,0,"H3");
	Dom.DimLineTo(-dH4,0,"H4");
	Dom.DimLineTo(-dK2,0,"K2");

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(0,0,0);
	Dom.DimLineTo(dK1,0,"K1");
	Dom.DimLineTo(dB1,0,"B1");
	Dom.DimLineTo(dD,0,"D");
	Dom.DimLineTo(dB2,0,"B2");
	Dom.DimLineTo(dK1,0,"K1");

	dX = 20000 + dK1 + (dB3+dD2_P+dD2_N+dB4)/2 - (dD2_P+dD2_N)/2, dY = dK2 + dH1 + dH2 + dH3 + dH4;

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(20000,0,0);
	Dom.DimLineTo(dK1,0,"K1");
	Dom.DimLineTo(dB2,0,"B3");
	Dom.DimLineTo(dD2_P,0,"P");
	Dom.DimLineTo(dD2_N,0,"N");
	Dom.DimLineTo(dB3,0,"B4");
	Dom.DimLineTo(dK1,0,"K1");
	
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dX,dY,0);
	Dom.DimLineTo(dD2_P,0,"P");
	Dom.DimLineTo(dD2_N,0,"N");
	*pDom << Dom;
}