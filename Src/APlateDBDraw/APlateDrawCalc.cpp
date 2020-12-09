// APlateDrawCalc.cpp: implementation of the CAPlateDrawCalc class.
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

CAPlateDrawCalc::CAPlateDrawCalc(CDataManage *pDataMng)
{
	m_pDataManage		= pDataMng;
	m_nGuardType		= 0;
	m_dDBLoadCar		= 0;
	m_dDBLoadTank		= 0;
	m_dDBLoadTrailer	= 0;
}

CAPlateDrawCalc::~CAPlateDrawCalc()
{

}
/////////////////////////////////////////////////////////////////////////
/// 방호벽그리기
///원점 : 방호벽의 좌측하단점
/////////////////////////////////////////////////////////////////////////
void CAPlateDrawCalc::DrawGuardWall(CDomyun *pDom, CCentSeparation *pGWall, double BX, double BY)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(0);
	CPlateBasicIndex *pBx    = pGir->GetBxOnJijum(0);

	CDomyun Dom(pDom);
	long pos = pGWall->GetPlace();
	long nHDanNode = pDB->GetQtyHDanNode()-1;

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);

	double W1 = pGWall->m_W1;
	double W2 = pGWall->m_W2;
	double W3 = pGWall->m_W3;
	double W4 = pGWall->m_W4;
	double W5 = pGWall->m_W5;
	double H1 = pGWall->m_H1;
	double H2 = pGWall->m_H2;
	double H3 = pGWall->m_H3;
	double LEN = pDB->GetLengthHDanDom(pBx, nHDanNode);
	double dSlopSlab = (A[0].y-A[1].y)/(A[0].x-A[1].x);
	double DiffOut = 0;
	double DiffIn = (W1+W2+W3+W4+W5)*dSlopSlab;

	if(pos==2)	//우측방호벽
	{
		BX = BX+LEN-pGWall->m_D1;
//		W1 = -pGWall->m_W1;
//		W2 = -pGWall->m_W2;
///		W3 = -pGWall->m_W3;
//		W4 = -pGWall->m_W4;
//		W5 = -pGWall->m_W5;
		W1 = -pGWall->m_W5;
		W2 = -pGWall->m_W4;
		W3 = -pGWall->m_W3;
		W4 = -pGWall->m_W2;
		W5 = -pGWall->m_W1;
		DiffIn = 0;
		CCentSeparation	*pSep = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
		dSlopSlab = (A[nHDanNode].y-A[nHDanNode+1].y)/(A[nHDanNode].x-A[nHDanNode+1].x);
		if(pSep)	DiffOut = -(W1+W2+W3+W4+W5)*dSlopSlab;
	}

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
/////////////////////////////////////////////////////////////////////////
///입력화면의 좌측 방호벽그리기
///
/////////////////////////////////////////////////////////////////////////

void CAPlateDrawCalc::DrawGuardWallInputLeft(CDomyun *pDom, CCentSeparation* pSep, double LEN)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();
	CARoadOptionStd *pStd    = m_pDataManage->GetOptionStd();	
	CPlateGirderApp *pGir    = pDB->GetGirder(0);	
	CPlateBasicIndex *pBx    = pGir->GetCrossBxAtMinFlangeWidth();

	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Height   = pSep ? pSep->GetHeight() : 0;
	double Ta       = pDB->m_dThickSlabLeft;
	double Tc       = pDB->m_dThickSlabBase;	
	double Dis      = pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;
	double UpFlangeT= pGir->GetThickFactByStation(pBx->GetStation(), G_F_U);
	double WebT     = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double Term     = pDB->m_dWidthSlabLeft;
	double dWFlange = pGir->GetWidthFlangeUpper(pBx);
	double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double dHunch	= 0;
	double dWidthBindConc	= pDB->m_BindConc.m_dWidth;
	double dHeightBindConc	= pDB->m_BindConc.m_dHeight;
	double dDeepBindConc	= pDB->m_BindConc.m_dDeep; 
	double dDisBindConc		= pDB->m_BindConc.m_dDis;

	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;

	if(pDB->IsUserSlabHunch(TRUE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(0);
	double dThickSlabLeftExt = pDB->m_dThickSlabLeftExt;

	CDomyun Dom(pDom);
	pStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);

	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;

	double Bx = A[0].x, By = A[0].y;		// Slab 끝단 위
	long nTypeHDan	= pDB->GetWallGuardTypeHDan(0);
	if(nTypeHDan == 1 || nTypeHDan == 2)  //방호벽 혹은 중분대..
	{
		DrawGuardWall(&Dom, pSep, Bx+pSep->m_D1, By);
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL	bBangEm			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
	BOOL	bNanGan			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan;
	BOOL	bWalkLoad		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad;
	CDPoint	xyLoadUp		= CDPoint(0,0); 
	CDPoint	xyLoadDn		= CDPoint(0,0); 
	double	dLoadArrowLen	= 120;
	double	dLoadGap		= 50;

	if(m_nGuardType==99)	LEN	= 0;
	
	if(bBangEm)// && m_nGuardType <= 8)		//방음벽
	{
		double BangEmDis = pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;

		xyLoadUp	= CDPoint(Bx+BangEmDis, By+Height+dLoadGap+dLoadArrowLen);
		xyLoadDn	= CDPoint(Bx+BangEmDis, By+Height+dLoadGap);
		DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
	}

	if(bNanGan)// && m_nGuardType <= 8)		//난간
	{
		double NanGanDis = pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;

		xyLoadUp	= CDPoint(Bx+NanGanDis, By+Height+dLoadGap+dLoadArrowLen);
		xyLoadDn	= CDPoint(Bx+NanGanDis, By+Height+dLoadGap);
		DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//부가하중
	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double	dApplyDis = 0, dELoadDis = 0;

	if(pData->m_bExtraLoad)
	{
		for(long i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
		{
			dApplyDis =	pDB->IsTUGir() ? Term-dWidthBindConc/2 : Term-WebT/2-dQuater;
			dELoadDis = pData->m_ELoadDisArr[i];
			if(dELoadDis<dApplyDis && dELoadDis>0)
			{
				DArrELoadDis.Add(dELoadDis);			
				SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
			}
		}

		for(long n = 0; n < DArrELoadDis.GetSize(); n++)	
		{
			if(DArrELoadDis[n]>0 && Term - DArrELoadDis[n] > 0) 
			{
				double ELoadDis		= DArrELoadDis[n];
				double	dHtemp		= (ELoadDis > (pSep?LEN:0)) ? Pavement : Height;

				xyLoadUp	= CDPoint(Bx+ELoadDis, By+dHtemp+dLoadGap+dLoadArrowLen);
				xyLoadDn	= CDPoint(Bx+ELoadDis, By+dHtemp+dLoadGap);
				DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//차량하중 표시-엑셀작업을 위해 추가함.
	double	DBLoadDis	= 0, DBLoadDis_2nd = 0;
	if(!bWalkLoad)
	{
		double	dDis_WtoW	= 1.8;
		if(pData->m_bTank && !pData->m_bTrailer)
			dDis_WtoW -= (dDis_Tank-dDis_Car);
		else if(!pData->m_bTank && pData->m_bTrailer)
			dDis_WtoW -= (dDis_Trailer-dDis_Car);
		else if(pData->m_bTank && pData->m_bTrailer)
		{
			double	dDisTemp = (dDis_Tank>=dDis_Trailer) ? dDis_Tank : dDis_Trailer;
			dDis_WtoW -= (dDisTemp-dDis_Car);
		}

		DBLoadDis	 = frM(dDis_Car)+LEN;
		DBLoadDis_2nd= frM(dDis_Car+1.8)+LEN;
		BOOL	bDBLaod_2nd = DBLoadDis_2nd < Term-Dis-dQuater;

		xyLoadUp	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap+dLoadArrowLen);
		xyLoadDn	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap);
		DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);

		if(pData->m_bTank) //탱크하중 작용시...
		{
			DBLoadDis		  = frM(dDis_Tank)+LEN;

			xyLoadUp	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap+dLoadArrowLen);
			xyLoadDn	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap);
			DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
		}
		if(pData->m_bTrailer) //트레일러하중 작용시...
		{
			DBLoadDis		  = frM(dDis_Trailer)+LEN;

			xyLoadUp	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap+dLoadArrowLen);
			xyLoadDn	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap);
			DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
		}

		// 두번째 바퀴가 캔틸레어에 재하될때
		if(bDBLaod_2nd)
		{
			xyLoadUp	= CDPoint(Bx+DBLoadDis_2nd, By+Pavement+dLoadGap+dLoadArrowLen);
			xyLoadDn	= CDPoint(Bx+DBLoadDis_2nd, By+Pavement+dLoadGap);
			DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 거더
	if(!pDB->IsTUGir())
	{	
		Dom.MoveTo(Bx+Term,			   By-Tc);
		Dom.LineTo(Bx+Term-dWFlange/2, By-Tc);
		Dom.LineTo(Bx+Term-dWFlange/2, By-Tc-UpFlangeT);
		Dom.LineTo(Bx+Term,			   By-Tc-UpFlangeT);

		Dom.LineTo(Bx+Term-WebT/2, By-Tc-UpFlangeT);
		Dom.LineTo(Bx+Term-WebT/2, By-Tc-300);
	
	}
	else
	{
		Dom.MoveTo(Bx+Term,					 By-Tc+dDeepBindConc);

		Dom.LineTo(Bx+Term-dWidthBindConc/2, By-Tc+dDeepBindConc);
		Dom.LineTo(Bx+Term-dWidthBindConc/2, By-Tc+dDeepBindConc-dHeightBindConc);
		Dom.LineTo(Bx+Term-WebT/2,			 By-Tc+dDeepBindConc-dHeightBindConc);

		Dom.MoveTo(Bx+Term,			   By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc);
		Dom.LineTo(Bx+Term-dWFlange/2, By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc);
		Dom.LineTo(Bx+Term-dWFlange/2, By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc+UpFlangeT);
		Dom.LineTo(Bx+Term,			   By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc+UpFlangeT);

		Dom.MoveTo(Bx+Term-WebT/2, By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc);
		Dom.LineTo(Bx+Term-WebT/2, By-Tc+dDeepBindConc-dHeightBindConc-300);
	}

	// 포장 또는 보도
	double	dUpperRoadH	= Pavement;
	double	dBodoLen	= 0;
	if(pSep)
	{
		if((nTypeHDan==0 || nTypeHDan==1 || nTypeHDan==2) && (pDB->GetWallGuardTypeHDan(1)!=3))
		{
			Dom.LineTo(Bx+LEN, By+Pavement, Bx+Term, By+Pavement);
			Dom.LineTo(Bx+LEN, By+Pavement, Bx+LEN, By);
		}
		else if((nTypeHDan==1 || nTypeHDan==2) && (pDB->GetWallGuardTypeHDan(1)==3))
		{
			double	dBodoH			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
			double	dBodoHDanLen	= pDB->GetLengthHDanDom(pBx, 1);
			if(LEN+dBodoHDanLen > Term)
				dBodoLen	= Term-LEN;
			else
				dBodoLen	= dBodoHDanLen;

			double	dT1			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
			double	dT2			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2;
			double	dHeightStt	= dT1!=0 ? dBodoH : 0;
			double	dHeightEnd	= dT1!=0 ? dBodoH+(dT2/dT1-1)*dBodoH*dBodoLen/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);
			
			if(LEN+dBodoHDanLen > Term) 
			{
				Dom.LineTo(Bx+LEN, By+dHeightStt, Bx+Term, By+dHeightEnd);
				Dom.LineTo(Bx+LEN, By+dHeightStt, Bx+LEN, By);
				dUpperRoadH	= dT1!=0 ? dBodoH+(dT2/dT1-1)*dBodoH*(Term-LEN)/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);
			}
			else
			{
				Dom.LineTo(Bx+LEN, By+dHeightStt, Bx+LEN+dBodoLen, By+dHeightEnd);
				Dom.LineTo(Bx+LEN, By+dHeightStt, Bx+LEN, By);
				Dom.LineTo(Bx+LEN+dBodoLen, By+dHeightEnd, Bx+LEN+dBodoLen, By);
				Dom.LineTo(Bx+LEN+dBodoLen, By+Pavement, Bx+Term, By+Pavement);

				if(LEN+dBodoLen >= Term-WebT/2)	
					dUpperRoadH	= dT1!=0 ? dBodoH+(dT2/dT1-1)*dBodoH*(Term-LEN)/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);
				else
					dUpperRoadH	= Pavement;
			}
		}
	}
	else
	{
		Dom.LineTo(Bx, By+Pavement, Bx+Term, By+Pavement);
		Dom.LineTo(Bx, By+Pavement, Bx, By);
	}

	// Slab
	if(!pDB->IsTUGir())
	{
		if(!pDB->IsUserSlabHunch(TRUE, pBx))
		{
			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx+Term,                By);

			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx,                     By-Ta);
			Dom.LineTo(Bx+Term-Dis-dWFlange/2, By-Tc);
			Dom.LineTo(Bx+Term-dWFlange/2,     By-Tc);
		}
		else
		{
			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx+Term,                By);

			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx,                     By-Ta);
			if(Tc == dThickSlabLeftExt)
				Dom.LineTo(Bx+Term-Dis-dWFlange/2-dHunch, By - Tc - (Tc-dThickSlabLeftExt));
			else
				Dom.LineTo(Bx+Term-Dis-dWFlange/2-dHunch, By - Ta - (dThickSlabLeftExt - Ta));
			Dom.LineTo(Bx+Term-Dis-dWFlange/2, By-Tc);
			Dom.LineTo(Bx+Term-dWFlange/2, By-Tc);
		}
	}
	else
	{
		Dom.MoveTo(Bx,					     By);
		Dom.LineTo(Bx+Term,                  By);

		Dom.MoveTo(Bx,					     By);
		Dom.LineTo(Bx,						 By-Ta);
		Dom.LineTo(Bx+Term-dWidthBindConc/2, By-Tc);
	}

	*pDom << Dom;	
}

void CAPlateDrawCalc::DrawFloorLoadInputLeftDim(CDomyun *pDom, CCentSeparation* pSep, double LEN)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();
	CPlateGirderApp *pGir    = pDB->GetGirder(0);
	CPlateBasicIndex *pBx    = pGir->GetCrossBxAtMinFlangeWidth();

	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;

	CDomyun Dom(pDom);

	CDPoint A[HDANLINESU], B[10]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);

	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;
	
	pGir->GetHunchXyDom(pBx, B);

	double Bx	= A[0].x, By = A[0].y;			// Slab 끝단 위
	double D1	= pSep ? pSep->m_D1 : 0;
	double W1	= pSep ? pSep->m_W1 : 0;
	double W2	= pSep ? pSep->m_W2 : 0;
	double W3	= pSep ? pSep->m_W3 : 0;
	double W4	= pSep ? pSep->m_W4 : 0;
	double W5	= pSep ? pSep->m_W5 : 0;
	double Width= pSep ? pSep->GetWidth() : 0;
	double Height	= pSep ? pSep->GetHeight() : 0;
	double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double dWFlange = pGir->GetWidthFlangeUpper(pBx);
	double dHunch	= 0;
	if(pDB->IsUserSlabHunch(TRUE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(0);
	double dThickSlabLeftExt = pDB->m_dThickSlabLeftExt;

	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Ta       = pDB->m_dThickSlabLeft;
	double Tc       = pDB->m_dThickSlabBase;	
	double Dis      = pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;
	double WebT     = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double Term     = pDB->m_dWidthSlabLeft;
	
	double dWidthBindConc	= pDB->m_BindConc.m_dWidth;
	
	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double	dApplyDis	= 0;
	double	dELoadDis	= 0;
	
	for(long i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
	{
		dApplyDis = pDB->IsTUGir() ? Term-dWidthBindConc/2 : Term-WebT/2-dQuater;
		dELoadDis = pData->m_ELoadDisArr[i];
		if(dELoadDis<dApplyDis && dELoadDis>0)
		{
			DArrELoadDis.Add(dELoadDis);			
			SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
		}
	}
	AscendSort(DArrELoadDis, SArrELoadTitle);

	BOOL	bBangEm		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
	BOOL	bNanGan		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan;
	BOOL	bWalkLoad	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad;
	BOOL	bAddLoad	=  SArrELoadTitle.GetSize()>0 ? TRUE : FALSE;

	double	dDimGap		= (bBangEm || bNanGan || bAddLoad) ? 170 : 0;
	long	nDimCol		= 0;
	
	if(m_nGuardType==99)	LEN	= 0;

	Dom.SetDirection("TOP");

	if(bBangEm)
	{
		double	BangEmDis	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
		
		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(BangEmDis, nDimCol, COMMA(fabs(BangEmDis)));
		Dom.TextOut(Bx+BangEmDis, By+Height+dDimGap, "방음벽하중");

		nDimCol++;
	}
	if(bNanGan)
	{
		double	NanGanDis	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;

		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(NanGanDis, nDimCol, COMMA(NanGanDis));
		Dom.TextOut( Bx+NanGanDis, By+Height+dDimGap, "난간하중");

		nDimCol++;
	}

	double	Temp	= 0;
	BOOL	bUsed	= FALSE;
	for(long n = 0; n < DArrELoadDis.GetSize(); n++)
	{
		if(pData->m_bExtraLoad) 
		{
			double ELoadDis		= DArrELoadDis[n];
			CString LoadTitle	= SArrELoadTitle[n];
			double	dHtemp		= (ELoadDis > LEN) ? Pavement : Height;

			if(n==0)
				Dom.DimMoveToExt( Bx+Temp, By+Height+dDimGap, nDimCol, "부가하중", "");
			else
				Dom.DimMoveTo( Bx+Temp, By+Height+dDimGap, nDimCol);
			Dom.DimLineTo(ELoadDis-Temp, nDimCol, COMMA(ELoadDis-Temp));
			Temp = ELoadDis;
			Dom.TextOut(Bx+ELoadDis, By+dHtemp+dDimGap, LoadTitle);
			
			bUsed	= TRUE;
		}
	}
	if(bUsed)	nDimCol++;

	if(m_nGuardType != 99)
	{
		Dom.DimMoveTo( Bx, By+Height+dDimGap, nDimCol);

		if(D1!=0)
			Dom.DimLineTo( D1, nDimCol, COMMA(D1));
		if(W1!=0)	
			Dom.DimLineTo( W1, nDimCol, COMMA(W1));
		if(W2!=0)		
			Dom.DimLineTo( W2, nDimCol, COMMA(W2));
		if(W3!=0)		
			Dom.DimLineTo( W3, nDimCol, COMMA(W3));
		if(W4!=0)		
			Dom.DimLineTo( W4, nDimCol, COMMA(W4));
		if(W5!=0)		
			Dom.DimLineTo( W5, nDimCol, COMMA(W5));
	}

	if(!bWalkLoad)
	{
		double	dDis_WtoW	= 1.8;
		if(pData->m_bTank && !pData->m_bTrailer)
			dDis_WtoW -= (dDis_Tank-dDis_Car);
		else if(!pData->m_bTank && pData->m_bTrailer)
			dDis_WtoW -= (dDis_Trailer-dDis_Car);
		else if(pData->m_bTank && pData->m_bTrailer)
		{
			double	dDisTemp = (dDis_Tank>=dDis_Trailer) ? dDis_Tank : dDis_Trailer;
			dDis_WtoW -= (dDisTemp-dDis_Car);
		}
			
		double	DBLoadDis	= frM(dDis_Car);
		double	DBLoadDis_2nd	= frM(dDis_Car+1.8);
		BOOL	bDBLoad_2nd	= DBLoadDis_2nd < Term-Dis-dQuater-LEN;
		
		Dom.DimMoveTo(Bx+LEN, By+Pavement+170, 0);
		Dom.DimLineTo(DBLoadDis, 0, COMMA(DBLoadDis)); 
		Dom.TextOut(Bx+LEN+DBLoadDis, By+Pavement+170, bDBLoad_2nd?"Pr1":"Pr");

		double	dDimDis = Bx+LEN+DBLoadDis;
		DBLoadDis = (dDis_Trailer-dDis_Car)*1000;
		if(pData->m_bTank)
		{			
			DBLoadDis		= (dDis_Tank-dDis_Car)*1000;
			
			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(DBLoadDis)); 
			Dom.TextOut(dDimDis+DBLoadDis, By+Pavement+170, "Tk");
			
			dDimDis = dDimDis+DBLoadDis;
			DBLoadDis = (dDis_Trailer-dDis_Tank)*1000; 
		}
		if(pData->m_bTrailer)
		{			
			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(DBLoadDis)); 
			Dom.TextOut(DBLoadDis+dDimDis, By+Pavement+170, "Tr");
		}

		if(bDBLoad_2nd)
		{
			Dom.DimLineTo(frM(dDis_WtoW), 0, COMMA(frM(dDis_WtoW))); 
			Dom.TextOut(Bx+LEN+DBLoadDis_2nd, By+Pavement+170, "Pr2");
		}
	}
	
	CString bodo1,bodo2;
	double	dBodoLen	= 0;
	double	dT1			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
	bodo1.Format("%.3f kN/m2", tokNPM2(dT1));
	
	if(pSep && bWalkLoad)
	{
		double	dBodoH	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
		double	dT2		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2;
		double	dT2_	= 0;
		double	dBodoHDanLen	= pDB->GetLengthHDanDom(pBx,1);
		if(LEN+dBodoHDanLen > Term-dQuater)
		{
			dBodoLen	= Term-dQuater-LEN;
			dT2_		= dT2 + fabs(dT2-dT1)*dBodoLen/dBodoHDanLen;
		}
		else
		{
			dBodoLen	= dBodoHDanLen;
			dT2_		= dT2;
		}

		double	dHeightStt	= dT1!=0 ? dBodoH : 0;
		double	dHeightEnd	= dT1!=0 ? dBodoH + (dT2/dT1-1)*dBodoH*dBodoLen/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);
		
//		bodo2.Format("%.3f kN/m2", dHeightStt!=0 ? tokNPM2(dT1*(dHeightEnd/dHeightStt)) : 0);	
		bodo2.Format("%.3f kN/m2", tokNPM2(dT2_));
		
		Dom.GiSiArrowExtend(Bx+LEN, By+dHeightStt, Dom.Always(10), Dom.Always(20), 1, _T("보도하중 T1"), bodo1);	
		Dom.GiSiArrowExtend(Bx+LEN+dBodoLen, By+dHeightEnd, Dom.Always(10), Dom.Always(30), 1, _T("보도하중 T2'"), bodo2);			
	}			

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo( Bx, By-Tc, 0 );
	if(!pDB->IsTUGir())
	{
		if(pDB->m_bUserSlabHunch)
		{
			Dom.DimLineTo(Term-dWFlange/2-Dis-dHunch, 0, COMMA(Term-dWFlange/2-Dis-dHunch) );
			Dom.DimLineTo(dHunch, 0, Comma(dHunch));
		}
		else
		{
			Dom.DimLineTo( Term-dWFlange/2-Dis, 0, COMMA(Term-dWFlange/2-Dis));
		}
		Dom.DimLineToExtend( Dis, 0, "", COMMA( Dis ) );
		Dom.DimLineTo( dQuater, 0, COMMA(dQuater));
		Dom.DimLineTo( dQuater, 0, COMMA(dQuater));
		Dom.DimMoveTo( Bx, By-Tc, 1 );
		Dom.DimLineTo( Term-WebT/2, 1,COMMA(Term-WebT/2));
	}
	else
	{
		Dom.DimLineTo( Term-dWidthBindConc/2, 0, COMMA(Term-dWidthBindConc/2));
		Dom.DimLineTo( dWidthBindConc/2-WebT/2, 0, COMMA(dWidthBindConc/2-WebT/2));
		Dom.DimMoveTo( Bx, By-Tc, 1 );
		Dom.DimLineTo( Term-WebT/2, 1,COMMA(Term-WebT/2));
	}

	// 
	Dom.CutLightning(Bx+Term, By+200, Bx+Term, By-Tc-200, TRUE, 0.75);
	if(!pDB->IsTUGir())
		Dom.LineTo(Bx+Term-WebT/2-dQuater, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH:By+Pavement
			, Bx+Term-WebT/2-dQuater, By-Tc);
	else
		Dom.LineTo(Bx+Term-dWidthBindConc/2, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH:By+Pavement
			, Bx+Term-dWidthBindConc/2, By-Tc);
		
	Dom.SetDirection("LEFT");
	
	if(pDB->m_bUserSlabHunch)
	{
		Dom.DimMoveTo(Bx, By-Tc, 0);
		if(Tc>dThickSlabLeftExt)
			Dom.DimLineToExtend(Tc-dThickSlabLeftExt, 0, "", Comma(Tc-dThickSlabLeftExt));
		if(Ta<dThickSlabLeftExt)
			Dom.DimLineTo(dThickSlabLeftExt-Ta, 0, Comma(dThickSlabLeftExt-Ta));
		Dom.DimLineTo(Ta, 0, COMMA(Ta));
	}
	else
	{
		Dom.DimMoveTo( Bx, By-Tc , 0);
		Dom.DimLineTo( Tc-Ta, 0, COMMA(Tc-Ta));
		Dom.DimLineTo( Ta, 0, COMMA(Ta));
	}

	if(bWalkLoad)
		Dom.DimLineToExtend(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH, 0, COMMA(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH), "H1");
	else
		Dom.DimLineTo( Pavement, 0, COMMA(Pavement));	

	*pDom << Dom;
}

void CAPlateDrawCalc::DrawFloorLoadInputRightDim(CDomyun *pDom, CCentSeparation* pSep, double LEN)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CPlateGirderApp  *pGir    = pDB->GetGirder(pDB->GetGirdersu()-1);	
	CPlateBasicIndex *pBx     = pGir->GetCrossBxAtMinFlangeWidth();
	
	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;
	
	long Size = pDB->GetQtyHDanNode();

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);

	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;

	double Bx	= A[Size].x, By = A[Size].y;
	double D1	= pSep ? -pSep->m_D1 : 0;
	double W1	= pSep ? -pSep->m_W1 : 0;
	double W2	= pSep ? -pSep->m_W2 : 0;
	double W3	= pSep ? -pSep->m_W3 : 0;
	double W4	= pSep ? -pSep->m_W4 : 0;
	double W5	= pSep ? -pSep->m_W5 : 0;
	double Width= pSep ? pSep->GetWidth() : 0;
	double D2	= LEN - Width;
	double Height = pSep ? pSep->GetHeight() : 0;
	
	D1 = -D2;

	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Ta       = pDB->m_dThickSlabRight;
	double Tc       = pDB->m_dThickSlabBase;	
	double Dis      = pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;
	double WebT     = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dWFlange = pGir->GetWidthFlangeUpper(pBx);
	double Term     = pDB->m_dWidthSlabRight;
	double dHunch	= 0;
	if(pDB->IsUserSlabHunch(FALSE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(pDB->m_dHunchArray.GetUpperBound());
	double dThickSlabRightExt = pDB->m_dThickSlabRightExt;
	double dQuater = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	CDPoint B[10];
	pGir->GetHunchXyDom(pBx,B);

	double dWidthBindConc	= pDB->m_BindConc.m_dWidth;
	
	CDomyun Dom(pDom);

	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double	dApplyDis	= 0;
	double	dELoadDis	= 0;
	double	dBridgeWidth = pDB->m_dBridgeWidth;
	long	i = 0;
	for(i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
	{
		dApplyDis	= pDB->IsTUGir() ? dBridgeWidth-(Term-dWidthBindConc/2) : dBridgeWidth-(Term-dQuater-WebT/2);
		dELoadDis	= pData->m_ELoadDisArr[i];
		if(dApplyDis<dELoadDis && dELoadDis>0)
		{
			DArrELoadDis.Add(dELoadDis);			
			SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
		}
	}
	AscendSort(DArrELoadDis, SArrELoadTitle);

	double TotalHLen = 0;
	for(i=0;i<pDB->GetQtyHDanNode();i++)
		TotalHLen += pDB->GetLengthHDanDom(pBx, i);

	BOOL	bBangEm		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;
	BOOL	bNanGan		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan;
	BOOL	bWalkLoad	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad;
	BOOL	bAddLoad	=  SArrELoadTitle.GetSize()>0 ? TRUE : FALSE;

	double	dDimGap		= (bBangEm || bNanGan || bAddLoad) ? 170 : 0;
	long	nDimCol		= 0;

	if(m_nGuardType==99)	LEN	= 0;

	Dom.SetDirection("TOP");

	if(bBangEm)
	{
		double	BangEmDis	= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;

		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(BangEmDis, nDimCol, COMMA(fabs(BangEmDis)));
		Dom.TextOut(Bx+BangEmDis, By+Height+dDimGap, "방음벽하중");

		nDimCol++;
	}
	if(bNanGan)
	{
		double	NanGanDis	= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;

		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(NanGanDis, nDimCol, COMMA(fabs(NanGanDis)));
		Dom.TextOut( Bx+NanGanDis, By+Height+dDimGap, "난간하중");

		nDimCol++;
	}

	double	Temp	= 0;
	BOOL	bUsed	= FALSE;
	for(long k = DArrELoadDis.GetSize() - 1; k >= 0; k--)
	{
		if(pData->m_bExtraLoad)
		{
			double ELoadDis		= -(TotalHLen-DArrELoadDis[k]);
			CString LoadTitle	= SArrELoadTitle[k];
			double	dHtemp		= (-ELoadDis > (pSep?LEN:0)) ? Pavement : Height;

			Dom.DimMoveTo( Bx+Temp, By+Height+dDimGap, nDimCol);
			Dom.DimLineTo(ELoadDis-Temp, nDimCol, COMMA(fabs(ELoadDis-Temp)));
			Temp = ELoadDis;
			Dom.TextOut(Bx+ELoadDis, By+dHtemp+dDimGap, LoadTitle);

			bUsed	= TRUE;
		}
	}
	if(bUsed)	nDimCol++;

	if(m_nGuardType != 99)
	{
		Dom.DimMoveTo( Bx, By+Height+dDimGap, nDimCol);

		if(D1!=0)
			Dom.DimLineTo( D1, nDimCol, COMMA(-D1));
		if(W1!=0)	
			Dom.DimLineTo( W1, nDimCol, COMMA(-W1));
		if(W2!=0)		
			Dom.DimLineTo( W2, nDimCol, COMMA(-W2));
		if(W3!=0)		
			Dom.DimLineTo( W3, nDimCol, COMMA(-W3));
		if(W4!=0)		
			Dom.DimLineTo( W4, nDimCol, COMMA(-W4));
		if(W5!=0)		
			Dom.DimLineTo( W5, nDimCol, COMMA(-W5));
	}

	if(!bWalkLoad)
	{
		double	dDis_WtoW	= 1.8;
		if(pData->m_bTank && !pData->m_bTrailer)
			dDis_WtoW -= (dDis_Tank-dDis_Car);
		else if(!pData->m_bTank && pData->m_bTrailer)
			dDis_WtoW -= (dDis_Trailer-dDis_Car);
		else if(pData->m_bTank && pData->m_bTrailer)
		{
			double	dDisTemp = (dDis_Tank>=dDis_Trailer) ? dDis_Tank : dDis_Trailer;
			dDis_WtoW -= (dDisTemp-dDis_Car);
		}

		double	DBLoadDis	= -frM(dDis_Car);
		double	DBLoadDis_2nd	= -frM(dDis_Car+dDis_WtoW);
		BOOL	bDBLoad_2nd	= ABS(DBLoadDis_2nd) < ABS(Term-Dis-dQuater-LEN);

		Dom.DimMoveTo(Bx-LEN, By+Pavement+170, 0);
		Dom.DimLineTo(DBLoadDis, 0, COMMA(ABS(DBLoadDis))); 
		Dom.TextOut(Bx-LEN+DBLoadDis, By+Pavement+170, bDBLoad_2nd?"Pr1":"Pr");

		double	dDimDis = Bx-LEN+DBLoadDis;
		DBLoadDis = -(dDis_Trailer-dDis_Car)*1000;
		if(pData->m_bTank)
		{			
			DBLoadDis		= -(dDis_Tank-dDis_Car)*1000;

			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(ABS(DBLoadDis))); 
			Dom.TextOut(dDimDis+DBLoadDis, By+Pavement+170, "Tk");

			dDimDis = dDimDis+DBLoadDis;
			DBLoadDis = -(dDis_Trailer-dDis_Tank)*1000; 
		}
		if(pData->m_bTrailer)
		{			
			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(ABS(DBLoadDis))); 
			Dom.TextOut(DBLoadDis+dDimDis, By+Pavement+170, "Tr");
		}

		if(bDBLoad_2nd)
		{
			Dom.DimLineTo(-frM(dDis_WtoW), 0, COMMA(frM(dDis_WtoW))); 
			Dom.TextOut(Bx-LEN+DBLoadDis_2nd, By+Pavement+170, "Pr2");
		}
	}
	
	if(pSep && bWalkLoad)
	{
		CString bodo1,bodo2;
		double	dBodoLen	= 0;
		double	dT1		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2;
		double	dT2		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double	dBodoH	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
		double	dBodoHDanLen	= pDB->GetLengthHDanDom(pBx, Size-2);
		if(LEN+dBodoHDanLen > Term+dQuater)		dBodoLen = Term+dQuater-LEN;
		else									dBodoLen = dBodoHDanLen;

		double	dHeightStt	= dT1!=0 ? dBodoH : 0;
		double	dHeightEnd	= dT1!=0 ? dBodoH + (dT2/dT1-1)*dBodoH*dBodoLen/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);

		bodo1.Format("%.3f kN/m2", tokNPM2(dT1));
		bodo2.Format("%.3f kN/m2", tokNPM2(dT2));

		Dom.GiSiArrowExtend(Bx-LEN, By+dHeightStt, Dom.Always(10), -Dom.Always(20), 3, _T("보도하중 T2"), bodo1);	
		Dom.GiSiArrowExtend(Bx-LEN-dBodoLen, By+dHeightEnd, Dom.Always(10), -Dom.Always(30), 3, _T("보도하중 T1"), bodo2);
	}

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo( Bx, By-Tc, 0 );
	if(!pDB->IsTUGir())
	{
		if(pDB->m_bUserSlabHunch)
		{
			Dom.DimLineTo(-Term+dWFlange/2+Dis+dHunch, 0, COMMA(Term-dWFlange/2-Dis-dHunch) );
			Dom.DimLineTo(-dHunch, 0, Comma(dHunch));
		}
		else
		{
			Dom.DimLineTo( -Term+dWFlange/2+Dis, 0, COMMA(Term-dWFlange/2-Dis));
		}
		Dom.DimLineToExtend( -Dis, 0, "", COMMA( Dis ) );
		Dom.DimLineTo( -dQuater, 0, COMMA(dQuater));
		Dom.DimLineTo( -dQuater, 0, COMMA(dQuater));
		Dom.DimMoveTo( Bx, By-Tc, 1 );
		Dom.DimLineTo( -Term+WebT/2, 1,COMMA(Term-WebT/2));
	}
	else
	{
		Dom.DimLineTo( -Term+dWidthBindConc/2, 0, COMMA(Term-dWidthBindConc/2));
		Dom.DimLineTo( -dWidthBindConc/2+WebT/2, 0, COMMA(dWidthBindConc/2-WebT/2));
		Dom.DimMoveTo( Bx, By-Tc, 1 );
		Dom.DimLineTo( -Term+WebT/2, 1, COMMA(Term-WebT/2));
	}

	Dom.CutLightning(Bx-Term, By+200, Bx-Term, By-Tc-200, TRUE, 0.75);
	if(!pDB->IsTUGir())
		Dom.LineTo(Bx-Term+WebT/2+dQuater, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH:By+Pavement
					, Bx-Term+WebT/2+dQuater, By-Tc);
	else
		Dom.LineTo(Bx-Term+dWidthBindConc/2, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH:By+Pavement
					, Bx-Term+dWidthBindConc/2, By-Tc);
	
	Dom.SetDirection("RIGHT");
	
	if(pDB->m_bUserSlabHunch)
	{
		Dom.DimMoveTo(Bx, By-Tc, 0);
		if(Tc>dThickSlabRightExt)
			Dom.DimLineToExtend(Tc-dThickSlabRightExt, 0, "", Comma(Tc-dThickSlabRightExt));
		if(Ta<dThickSlabRightExt)
			Dom.DimLineTo(dThickSlabRightExt-Ta, 0, Comma(dThickSlabRightExt-Ta));
		Dom.DimLineTo(Ta, 0, COMMA(Ta));
	}
	else
	{
		Dom.DimMoveTo( Bx, By-Tc, 0 );
		Dom.DimLineTo( Tc-Ta, 0, COMMA(Tc-Ta));
		Dom.DimLineTo( Ta, 0, COMMA(Ta));
	}
	if(bWalkLoad)
		Dom.DimLineToExtend(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH, 0, COMMA(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH), "H1");
	else
		Dom.DimLineTo( Pavement, 0, COMMA(Pavement));	

	*pDom << Dom;	
}

/*
void CAPlateDrawCalc::DrawGuardInputLeftDim(CDomyun *pDom, CCentSeparation* pSep, double LEN)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();
	CARoadOptionStd *pStd    = m_pDataManage->GetOptionStd();
	CPlateGirderApp *pGir    = pDB->GetGirder(0);
	CPlateBasicIndex *pBx    = pGir->GetCrossBxAtMinFlangeWidth();	

	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;
	
	CDomyun Dom(pDom);
	
	CDPoint A[HDANLINESU], B[10]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);

	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;
	
	pGir->GetHunchXyDom(pBx, B);

	double Bx = A[0].x, By = A[0].y;			// Slab 끝단 위
	double D1 = pSep ? pSep->m_D1 : 0;
	double W1 = pSep ? pSep->m_W1 : 0;
	double W2 = pSep ? pSep->m_W2 : 0;
	double W3 = pSep ? pSep->m_W3 : 0;
	double W4 = pSep ? pSep->m_W4 : 0;
	double W5 = pSep ? pSep->m_W5 : 0;
	double H1 = pSep ? pSep->m_H1 : 0;
	double H2 = pSep ? pSep->m_H2 : 0;
	double H3 = pSep ? pSep->m_H3 : 0;	
	double Width = pSep ? pSep->GetWidth() : 0;
	double D2    = LEN - Width - D1;
	double Height= pSep ? pSep->GetHeight() : 0;
	double BH = 0;
	double NH = 0;
	double BWidth	= 100;
	double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double dWFlange = pGir->GetWidthFlangeUpper(pBx);
	double dHunch	= 0;
	if(pDB->IsUserSlabHunch(TRUE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(0);
	double dThickSlabLeftExt = pDB->m_dThickSlabLeftExt;

	if(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && m_nGuardType <= 8)	BH =  170;
	if(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && m_nGuardType <= 8)	NH = 170;

	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Ta       = pDB->m_dThickSlabLeft;
	double Tc       = pDB->m_dThickSlabBase;	
	double Dis      = pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;
	double WebT     = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double Term     = pDB->m_dWidthSlabLeft;
	double UpFlangeT= pGir->GetThickFactByStation(pBx->GetStation(), G_F_U);
	double L1 = 0, L2 = 0, T3 = 0, uM = 0, BrLen = 0;
	double XH1 = 0;
	double XH2 = 0;

	BOOL	bBangEm		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
	BOOL	bNanGan		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan;
	BOOL	bWalkLoad	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad;
	
	Dom.SetDirection("TOP");
	if(m_nGuardType != 99)
	{
		if(bBangEm || bNanGan)
			Dom.DimMoveTo( Bx, By + Height + BH + NH, 0 );
		else
			Dom.DimMoveTo( Bx, By + Height, 0 );

		Dom.DimLineTo( D1, 0, COMMA(D1));
		Dom.DimLineTo( W1, 0, COMMA(W1));
		Dom.DimLineTo( W2, 0, COMMA(W2));
		Dom.DimLineTo( W3, 0, COMMA(W3));
		Dom.DimLineTo( W4, 0, COMMA(W4));
		Dom.DimLineTo( W5, 0, COMMA(W5));

		if(bBangEm || bNanGan)
			Dom.DimMoveTo( Bx, By + Height + BH + NH, 1 );
		else
			Dom.DimMoveTo( Bx, By + Height, 1 );

		Dom.DimLineTo( D1+Width, 1, COMMA(D1+Width) );
	}

	CString bodo1,bodo2;
	double Cy = 0;

	/////////////////////////////////////////////
	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double dCen, dELoadDis;
	long nCount = 0;	

	for(long i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
	{
		dCen = pDB->GetWidthSlabDom(pBx) / 2.0;
		dELoadDis = pData->m_ELoadDisArr[i];
		if(dELoadDis <= dCen)
		{
			DArrELoadDis.Add(dELoadDis);
			SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
		}
	}
	Cy = By-Tc;
	XH1 = By;

	// Slab Area Line
	Dom.MoveTo( Bx, By-Ta );
	Dom.LineTo( Bx+Term-dQuater-WebT/2, By-Ta );//Dom.LineTo( Bx+Term-dQuater-WebT/2, By-Ta );
	Dom.MoveTo( Bx+Term-Dis-dWFlange/2, By-Ta );
	Dom.LineTo( Bx+Term-Dis-dWFlange/2, By-Tc );

	double	dLoad1		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
	bodo1.Format("%.3f kN/㎡", tokNPM2(dLoad1));
	if(bWalkLoad)
	{
		double dBodoH	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
		double dBodoW	= frM(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadWidth);
		double T2	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
		bodo2.Format("%.3f kN/㎡", tokNPM2(T2));

		Dom.GiSiArrowExtend(Bx+D1+Width, By+dBodoH, Dom.Always(10), Dom.Always(20), 1, _T("보도하중"), bodo1);	
		Dom.GiSiArrowExtend(Bx+Term-dQuater-WebT/2, By+dBodoH, Dom.Always(20), Dom.Always(30), 1, _T("검토위치 보도하중"), bodo2);			
	}			

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo( Bx, Cy, 0 );
	if(pDB->IsUserSlabHunch(TRUE, pBx))
	{
		Dom.DimLineTo( Term-Dis-dWFlange/2-dHunch, 0, COMMA(Term-Dis-dWFlange/2-dHunch) );
		Dom.DimLineTo(dHunch, 0, Comma(dHunch));
	}
	else
	{
		Dom.DimLineTo( Term-Dis-dWFlange/2, 0, COMMA(Term-Dis-dWFlange/2));
	}
	Dom.DimLineToExtend( Dis, 0, "", COMMA( Dis ) );
	Dom.DimLineTo( dQuater, 0, COMMA(dQuater));
	Dom.DimLineTo( dQuater+WebT/2, 0, COMMA(dQuater+WebT/2));

	Dom.DimMoveTo( Bx, Cy, 1 );
	Dom.DimLineTo( Term, 1,COMMA(Term));

	Dom.SetDirection("TOP");

	double Temp = 0;
	for(long n = 0; n < DArrELoadDis.GetSize(); n++)	//부가하중
	{
		if(pData->m_bExtraLoad && DArrELoadDis[n]>0 && Term-dQuater-WebT/2- DArrELoadDis[n] > 0) 
		{
			double ELoadDis		= DArrELoadDis[n];
			CString LoadTitle	= SArrELoadTitle[n];

			if(bBangEm || bNanGan)
				Dom.DimMoveTo( Bx+Temp, By + Height + BH + NH, 3 );
			else
				Dom.DimMoveTo( Bx+Temp, By + Height, 3 );

			Dom.DimLineTo(ELoadDis-Temp,3,COMMA(ELoadDis-Temp));

			Temp = ELoadDis;
			
			Dom.TextOut(Bx+ELoadDis, By+Height+170, LoadTitle);
		}
	}

	long nNumber = 0;
	CString szNumber;
	//GuardWall Area Line
	if(m_nGuardType == 1)
	{
		Dom.LineTo(Bx+D1, By+H1+H2, Bx+D1+W3+W4, By+H1+H2);
		Dom.LineTo(Bx+D1+W3, By+H1+H2, Bx+D1+W3, By+H1+H2+H3);
		Dom.LineTo(Bx+D1+W3+W4, By, Bx+D1+W3+W4, By+H1+H2);
		Dom.LineTo(Bx+D1+W3+W4, By+H1, Bx+D1+W3+W4+W5, By+H1);

		Dom.TextCircle(Bx+D1+W3/2,By+H1+H2+H3/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W4/3,By+H1+H2+H3/3, "2", -1);
		Dom.TextCircle(Bx+D1+(W3+W4)/2,By+(H1+H2)/2, "3", -1);
		Dom.TextCircle(Bx+D1+W3+W4+W5/3,By+H1+H2/3, "4", -1);
		Dom.TextCircle(Bx+D1+W3+W4+W5/2,By+H1/2, "5", -1);
		nNumber = 6;
	}
	if(m_nGuardType == 2)
	{
		if(W4>0)
		{
			Dom.LineTo(Bx+D1, By+H1+H2, Bx+D1+W3+W4, By+H1+H2);
			Dom.LineTo(Bx+D1+W3, By+H1+H2, Bx+D1+W3, By+H1+H2+H3);

			Dom.TextCircle(Bx+D1+W3/2,By+H1+H2+H3/2, "1", -1);		nNumber++;
			Dom.TextCircle(Bx+D1+W3+W4/3,By+H1+H2+H3/3, "2", -1);	nNumber++;
			Dom.TextCircle(Bx+D1+(W3+W4)/2,By+(H1+H2)/2, "3", -1);	nNumber++;
		}
		if(W5>0)
		{
			Dom.LineTo(Bx+D1, By+H1, Bx+D1+W3+W5, By+H1);
			Dom.LineTo(Bx+D1+W3, By+H1, Bx+D1+W3, By+H1+H2);

			Dom.TextCircle(Bx+D1+W3/2,By+H1+H2/2, "1", -1);
			Dom.TextCircle(Bx+D1+W3+W5/3,By+H1+H2/3, "2", -1);
			Dom.TextCircle(Bx+D1+(W3+W5)/2,By+H1/2, "3", -1);
		}
		nNumber = 4;
	}
	if(m_nGuardType == 3)
	{
		Dom.LineTo(Bx+D1, By+H1, Bx+D1+W3+W5, By+H1);
		Dom.LineTo(Bx+D1+W3, By+H1, Bx+D1+W3, By+H1+H2);

		Dom.TextCircle(Bx+D1+W3/2,By+H1+(H2+H3)/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W5/3,By+H1+H2/3, "2", -1);
		Dom.TextCircle(Bx+D1+(W3+W5)/2,By+H1/2, "3", -1);
		nNumber = 4;
	}
	if(m_nGuardType == 4)//
	{
		Dom.TextCircle(Bx+D1+Width/2,By+(H1+H2+H3)/2, "1", -1);
		nNumber = 2;
	}
	if(m_nGuardType == 5)
	{
		Dom.LineTo(Bx+D1+W1, By+H1+H2, Bx+D1+W1+W2+W3+W4, By+H1+H2);
		Dom.LineTo(Bx+D1, By+H1, Bx+D1+Width, By+H1);

		Dom.TextCircle(Bx+D1+W1+W2+W3/2,By+H1+H2+H3/2, "1", -1);
		Dom.TextCircle(Bx+D1+W1+W2+W3/2,By+H1+H2/2, "2", -1);
		Dom.TextCircle(Bx+D1+W1+W2+W3/2,By+H1/2, "3", -1);
		nNumber = 4;
	}
	if(m_nGuardType == 6)
	{
		Dom.LineTo(Bx+D1+W3, By, Bx+D1+W3, By+H2);

		Dom.TextCircle(Bx+D1+W3/2,By+(H2+H3)/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W5/3,By+H2/3, "2", -1);
		nNumber = 3;
	}
	if(m_nGuardType == 7)
	{
		Dom.LineTo(Bx+D1+W3, By, Bx+D1+W3, By+H2);

		Dom.TextCircle(Bx+D1+W3/2,By+H2/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W5/3,By+H2/3, "2", -1);
		nNumber = 3;
	}
	if(m_nGuardType == 8)
	{
		Dom.LineTo(Bx+D1+W3, By, Bx+D1+W3, By+H1);

		Dom.TextCircle(Bx+D1+W3/2,By+(H1+H3)/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W5/2,By+H1/2, "2", -1);
		nNumber = 3;
	}
	if(m_nGuardType == 100)
	{
		Dom.TextOut(Bx+Term/2,By+H1/2,"이 방호벽 모양은 지원할 수 없습니다.");
	}
	
	/// Slab Area Line
	if(pDB->IsUserSlabHunch(TRUE, pBx) && m_nGuardType != 100)
	{
		if(nNumber==0) nNumber=1;

		double dTemp = Term - (dQuater+WebT/2) - (dQuater+Dis) - dHunch;
		Dom.LineTo(Bx + dTemp, By-Ta, Bx+dTemp, By-Ta-(dThickSlabLeftExt -Ta));
		if(Tc != dThickSlabLeftExt)
			Dom.LineTo(Bx+dTemp, By-Ta-(dThickSlabLeftExt -Ta), Bx+dTemp+dHunch, By-Ta-(dThickSlabLeftExt -Ta));

		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+Term/2,XH1-Ta/2, szNumber, -1);

		if(dThickSlabLeftExt-Ta>0)
		{		
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+Term-(dQuater+WebT/2)-(dQuater+Dis) - dHunch - dTemp/6, By - Ta - (dThickSlabLeftExt-Ta)/2, szNumber, -1);
		
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+Term-(dQuater+WebT/2)-(dQuater+Dis) - dHunch/2, By - Ta - (dThickSlabLeftExt-Ta)/2, szNumber, -1);
		}

		if(Tc != dThickSlabLeftExt)
		{
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+Term-(dQuater+WebT/2)-(dQuater+Dis) - dHunch/3, By - Tc + (Tc-dThickSlabLeftExt)/2, szNumber, -1);
		}
		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+Term-(dQuater+WebT/2)-(dQuater+Dis)/2, XH1-Tc-(Ta-Tc)/2, szNumber, -1);

		if(Term > (Width+D1))
		{
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+D1+W3+W4+W5+(Term-Width-D1)/2,XH1+Pavement/2, szNumber, -1);
		}

	}
	else
	{
		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+Term/2,XH1-Ta/2, szNumber, -1);

		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+(Term-Dis-dQuater-dWFlange/2-WebT/2)*0.8, XH1-Tc-(Ta-Tc)/2, szNumber, -1);

		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+Term-(dQuater+WebT/2)-(dQuater+Dis)/2, XH1-Tc-(Ta-Tc)/2, szNumber, -1);

		if(Term > (Width+D1))
		{
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+D1+W3+W4+W5+(Term-Width-D1)/2,XH1+Pavement/2, szNumber, -1);
		}
	}
	Dom.SetDirection("LEFT");
	
	double DH = B[9].y-B[8].y;
	if(DH<0)
		DH = 0;

	if(pDB->IsUserSlabHunch(TRUE, pBx))
	{
		Dom.DimMoveTo(Bx, By-Tc-(DH), 0);
		Dom.DimLineToExtend(Tc- dThickSlabLeftExt, 0, "", Comma(Tc-dThickSlabLeftExt));
		if(dThickSlabLeftExt-Ta>0)
			Dom.DimLineTo(ABS(dThickSlabLeftExt-Ta), 0, Comma(ABS(dThickSlabLeftExt-Ta)));
		Dom.DimLineTo(DH, 0, COMMA(DH));
		Dom.DimLineTo(Ta, 0, COMMA(Ta));
	}
	else
	{
		Dom.DimMoveTo(Bx, By-Ta-(DH), 0);
		Dom.DimLineTo(DH, 0, COMMA(DH));
		Dom.DimLineTo(Ta, 0, COMMA(Ta));
	}

	if(bWalkLoad)
		Dom.DimLineTo( pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH, 0, COMMA(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH));
	else
		Dom.DimLineTo( pDB->m_dThickPave, 0, COMMA(pDB->m_dThickPave));		

	Dom.DimMoveTo( Bx, By-Ta-(DH), 1 );
	if(bWalkLoad)
		Dom.DimLineTo( pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH+Ta+(DH), 1, COMMA(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH+Ta+(DH)));
	else
		Dom.DimLineTo( pDB->m_dThickPave+Ta+(DH), 1, COMMA(pDB->m_dThickPave+Ta+(DH)));		

	double NanGanDis	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
	double BangEmDis	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;

	Dom.SetDirection("TOP");

	if(bBangEm && !bNanGan)	//방음벽일 경우
	{
		Dom.DimMoveTo(Bx, By+Height+30,0);
		Dom.DimLineTo(BangEmDis,0,COMMA(fabs(BangEmDis)));
		Dom.TextOut(Bx+BangEmDis+100, By+Height+170, "방음벽하중");
	}

	if(!bBangEm && bNanGan)	//난간일 경우
	{
		Dom.DimMoveTo(Bx, By+Height+30,0);
		Dom.DimLineTo(NanGanDis,0,COMMA(NanGanDis));
		Dom.TextOut( Bx+NanGanDis+100, By+Height+170, "난간하중");
	}
	
	if(!bWalkLoad)
	{
		double dByH	= 0;
		double dDimDis = 0;
		double DBLoadDis		= dDis_Car *1000+LEN;
		if(m_nGuardType == 99)	DBLoadDis		= dDis_Car*1000;

		Dom.DimMoveTo( Bx, By + Height + BH + NH, 2 );	//차량하중표시
		Dom.DimLineTo( DBLoadDis, 2, COMMA(DBLoadDis)); 
		Dom.TextOut(Bx+DBLoadDis, By+Pavement+400, "Pr");

		dByH += 100;
		dDimDis = Bx+DBLoadDis;
		DBLoadDis = (dDis_Trailer-dDis_Car)*1000; 

		if(pData->m_bTank)
		{			
			DBLoadDis		= (dDis_Tank-dDis_Car)*1000;//dDis_Tank *1000+LEN;
			//if(m_nGuardType == 99)	DBLoadDis		= dDis_Tank*1000;

			Dom.DimMoveTo( dDimDis, By + Height + BH + NH, 2 );	//차량하중표시
			Dom.DimLineTo( DBLoadDis, 2, COMMA(DBLoadDis)); 
			Dom.TextOut(dDimDis+DBLoadDis, By+Pavement+400+dByH, "Tk");

			dByH += 100;			
			dDimDis = dDimDis+DBLoadDis;
			DBLoadDis = (dDis_Trailer-dDis_Tank)*1000; 
		}
		if(pData->m_bTrailer)
		{			
			//DBLoadDis		= dDis_Trailer *1000+LEN;
			//if(m_nGuardType == 99)	DBLoadDis		= dDis_Trailer*1000;
			 
			Dom.DimMoveTo( dDimDis, By + Height + BH + NH, 2 );	//차량하중표시
			Dom.DimLineTo( DBLoadDis, 2, COMMA(DBLoadDis)); 
			Dom.TextOut(DBLoadDis+dDimDis, By+Pavement+400+dByH, "Tr");

			dByH += 100;
		}

	}

	*pDom << Dom;	
}
*/

void CAPlateDrawCalc::DrawGuardWallInputRight(CDomyun *pDom, CCentSeparation* pSep, double LEN)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CARoadOptionStd  *pStd    = m_pDataManage->GetOptionStd();	
	CPlateGirderApp  *pGir    = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBx     = pGir->GetCrossBxAtMinFlangeWidth();
	
	double D1 = pSep ? -pSep->m_D1 : 0;
	double W1 = pSep ? -pSep->m_W1 : 0;
	double W2 = pSep ? -pSep->m_W2 : 0;
	double W3 = pSep ? -pSep->m_W3 : 0;
	double W4 = pSep ? -pSep->m_W4 : 0;
	double W5 = pSep ? -pSep->m_W5 : 0;
	double D2 = pSep ? LEN - fabs(D1 + W1 + W2 + W3 + W4 + W5) : 0;
	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Height   = pSep ? pSep->GetHeight() : 0;
	double Ta	    = pDB->m_dThickSlabRight;
	double Tc       = pDB->m_dThickSlabBase;	
	double Dis      = -(pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2);
	double UpFlangeT= pGir->GetThickFactByStation(pBx->GetStation(), G_F_U);
	double WebT		= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dWFlange = pGir->GetWidthFlangeUpper(pBx);
	double Term     = -pDB->m_dWidthSlabRight;
	long   Size     = pDB->GetQtyHDanNode();
	double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double dHunch	= 0;
	if(pDB->IsUserSlabHunch(FALSE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(pDB->m_dHunchArray.GetUpperBound());
	double dThickSlabRightExt = pDB->m_dThickSlabRightExt;
	double dWidthBindConc	= pDB->m_BindConc.m_dWidth;
	double dHeightBindConc	= pDB->m_BindConc.m_dHeight;
	double dDeepBindConc	= pDB->m_BindConc.m_dDeep; 
	double dDisBindConc		= pDB->m_BindConc.m_dDis;
	
	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;

	
	D1 = -D2;
	
	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);

	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;

	double Bx=A[Size].x, By = A[Size].y;

	CDomyun Dom(pDom);
	pStd->SetEnvType(&Dom, HCAD_STLC);

	long nTypeHDan	= pDB->GetWallGuardTypeHDan(Size-1);
	if(nTypeHDan == 1 || nTypeHDan == 2)  //방호벽 혹은 중분대..
	{
		DrawGuardWall(&Dom, pSep, Bx-LEN + D1, By);//LEN을 바꾸지 말 것.
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL	bBangEm			= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;
	BOOL	bNangan			= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan;
	BOOL	bWalkLoad		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad;
	CDPoint	xyLoadUp		= CDPoint(0,0); 
	CDPoint	xyLoadDn		= CDPoint(0,0); 
	double	dLoadArrowLen	= 120;
	double	dLoadGap		= 50;

	if(m_nGuardType==99)	LEN	= 0;

	if(bBangEm && m_nGuardType <= 8)		//방음벽
	{
		double BangEmDis		= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;

		xyLoadUp	= CDPoint(Bx+BangEmDis, By+Height+dLoadGap+dLoadArrowLen);
		xyLoadDn	= CDPoint(Bx+BangEmDis, By+Height+dLoadGap);
		DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
	}

	if(bNangan && m_nGuardType <= 8)		//난간
	{
		double NanGanDis		= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;

		xyLoadUp	= CDPoint(Bx+NanGanDis, By+Height+dLoadGap+dLoadArrowLen);
		xyLoadDn	= CDPoint(Bx+NanGanDis, By+Height+dLoadGap);
		DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
	}

	
	double TotalHLen = 0;
	int i = 0;
	for(i=0;i<pDB->GetQtyHDanNode();i++)
		TotalHLen += pDB->GetLengthHDanDom(pBx, i);

	/////////////////////////////////////////////
	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double	dApplyDis = 0, dELoadDis = 0;
	double	dBridgeWidth	= pDB->m_dBridgeWidth;
	
	if(pData->m_bExtraLoad)
	{
		for(i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
		{
			dApplyDis =	pDB->IsTUGir() ? dBridgeWidth - (ABS(Term)-dWidthBindConc/2) : dBridgeWidth - (ABS(Term)-WebT/2-dQuater);
			dELoadDis = pData->m_ELoadDisArr[i];

			if(dApplyDis<dELoadDis && dELoadDis>0 && dELoadDis<dBridgeWidth)
			{
				DArrELoadDis.Add(dELoadDis);			
				SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
			}
		}

		for(long n = DArrELoadDis.GetSize()-1; n>=0; n--)	//부가하중
		{
			double ELoadDis		= -(TotalHLen-DArrELoadDis[n]);
			double	dHtemp		= (-ELoadDis > (pSep?LEN:0)) ? Pavement : Height;

			xyLoadUp	= CDPoint(Bx+ELoadDis, By+dHtemp+dLoadGap+dLoadArrowLen);
			xyLoadDn	= CDPoint(Bx+ELoadDis, By+dHtemp+dLoadGap);
			DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//차량하중 표시-엑셀작업을 위해 추가함.
	double DBLoadDis	= 0, DBLoadDis_2nd = 0;
	if(!bWalkLoad)
	{
		double	dDis_WtoW	= 1.8;
		if(pData->m_bTank && !pData->m_bTrailer)
			dDis_WtoW -= (dDis_Tank-dDis_Car);
		else if(!pData->m_bTank && pData->m_bTrailer)
			dDis_WtoW -= (dDis_Trailer-dDis_Car);
		else if(pData->m_bTank && pData->m_bTrailer)
		{
			double	dDisTemp = (dDis_Tank>=dDis_Trailer) ? dDis_Tank : dDis_Trailer;
			dDis_WtoW -= (dDisTemp-dDis_Car);
		}

		DBLoadDis	 = -frM(dDis_Car)-LEN;
		DBLoadDis_2nd= -frM(dDis_Car+1.8)-LEN;
		BOOL	bDBLaod_2nd = ABS(DBLoadDis_2nd) < ABS(Term-Dis+dQuater);
		
		xyLoadUp	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap+dLoadArrowLen);
		xyLoadDn	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap);
		DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);

		if(pData->m_bTank) //탱크하중 작용시...
		{
			DBLoadDis		  = -dDis_Tank*1000-LEN;
			
			xyLoadUp	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap+dLoadArrowLen);
			xyLoadDn	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap);
			DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);

		}
		if(pData->m_bTrailer) //트레일러하중 작용시...
		{
			DBLoadDis		  = -dDis_Trailer*1000-LEN;
			
			xyLoadUp	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap+dLoadArrowLen);
			xyLoadDn	= CDPoint(Bx+DBLoadDis, By+Pavement+dLoadGap);
			DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
		}

		// 두번째 바퀴가 캔틸레어에 재하될때
		if(bDBLaod_2nd)
		{
			xyLoadUp	= CDPoint(Bx+DBLoadDis_2nd, By+Pavement+dLoadGap+dLoadArrowLen);
			xyLoadDn	= CDPoint(Bx+DBLoadDis_2nd, By+Pavement+dLoadGap);
			DrawLoadArrow(&Dom, xyLoadUp, xyLoadDn);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 거더
	if(!pDB->IsTUGir())
	{	
		Dom.MoveTo(Bx+Term,			   By-Tc);
		Dom.LineTo(Bx+Term+dWFlange/2, By-Tc);
		Dom.LineTo(Bx+Term+dWFlange/2, By-Tc-UpFlangeT);
		Dom.LineTo(Bx+Term,			   By-Tc-UpFlangeT);

		Dom.LineTo(Bx+Term+WebT/2, By-Tc-UpFlangeT);
		Dom.LineTo(Bx+Term+WebT/2, By-Tc-300);
	}
	else
	{
		Dom.MoveTo(Bx+Term,					 By-Tc+dDeepBindConc);
		Dom.LineTo(Bx+Term+dWidthBindConc/2, By-Tc+dDeepBindConc);
		Dom.LineTo(Bx+Term+dWidthBindConc/2, By-Tc+dDeepBindConc-dHeightBindConc);
		Dom.LineTo(Bx+Term+WebT/2,			 By-Tc+dDeepBindConc-dHeightBindConc);

		Dom.MoveTo(Bx+Term,			   By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc);
		Dom.LineTo(Bx+Term+dWFlange/2, By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc);
		Dom.LineTo(Bx+Term+dWFlange/2, By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc+UpFlangeT);
		Dom.LineTo(Bx+Term,			   By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc+UpFlangeT);

		Dom.MoveTo(Bx+Term+WebT/2, By-Tc+dDeepBindConc-dHeightBindConc+dDisBindConc);
		Dom.LineTo(Bx+Term+WebT/2, By-Tc+dDeepBindConc-dHeightBindConc-300);
	}

	// 포장 또는 보도
	double	dUpperRoadH	= Pavement;
	double	dBodoLen	= 0;
	if(pSep)
	{
		if((nTypeHDan ==0 || nTypeHDan==1 || nTypeHDan==2) && (pDB->GetWallGuardTypeHDan(Size-2)!=3))
		{	
			Dom.LineTo(Bx-LEN, By+Pavement, Bx+Term, By+Pavement);
			Dom.LineTo(Bx-LEN, By+Pavement, Bx-LEN, By);
		}
		else if((nTypeHDan==1 || nTypeHDan==2) && (pDB->GetWallGuardTypeHDan(Size-2)==3))
		{
			double dBodoH		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
			double dBodoHDanLen	= pDB->GetLengthHDanDom(pBx, Size-2);
			if(LEN+dBodoHDanLen > ABS(Term))
				dBodoLen	= ABS(Term)-LEN;
			else
				dBodoLen	= dBodoHDanLen;

			double	dT1			= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2;
			double	dT2			= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
			double	dHeightStt	= dT1!=0 ? dBodoH : 0;
			double	dHeightEnd	= dT1!=0 ? dBodoH+(dT2/dT1-1)*dBodoH*dBodoLen/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);

			if(LEN+dBodoHDanLen > ABS(Term)) 
			{
				Dom.LineTo(Bx-LEN, By+dHeightStt, Bx+Term, By+dHeightEnd);
				Dom.LineTo(Bx-LEN, By+dHeightStt, Bx-LEN, By);
				dUpperRoadH	= dT1!=0 ? dBodoH+(dT2/dT1-1)*dBodoH*(ABS(Term)-LEN)/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);
			}
			else
			{
				Dom.LineTo(Bx-LEN, By+dHeightStt, Bx-LEN-dBodoLen, By+dHeightEnd);
				Dom.LineTo(Bx-LEN, By+dHeightStt, Bx-LEN, By);
				Dom.LineTo(Bx-LEN-dBodoLen, By+dHeightEnd, Bx-LEN-dBodoLen, By);
				Dom.LineTo(Bx-LEN-dBodoLen, By+Pavement, Bx+Term, By+Pavement);

				if(LEN+dBodoLen >= ABS(Term)-WebT/2)
					dUpperRoadH	= dT1!=0 ? dBodoH+(dT2/dT1-1)*dBodoH*(ABS(Term)-LEN)/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);
				else				
					dUpperRoadH	= Pavement;
			}
		}
	}
	else
	{
		Dom.LineTo(Bx, By+Pavement, Bx+Term, By+Pavement);
		Dom.LineTo(Bx, By+Pavement, Bx, By);
	}


	// Slab
	if(!pDB->IsTUGir())
	{
		if(!pDB->IsUserSlabHunch(TRUE, pBx))
		{
			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx+Term,                By);

			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx,                     By-Ta);
			Dom.LineTo(Bx+Term-Dis+dWFlange/2, By-Tc);
			Dom.LineTo(Bx+Term+dWFlange/2,     By-Tc);
		}
		else
		{
			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx+Term,                By);

			Dom.MoveTo(Bx,					   By);
			Dom.LineTo(Bx,                     By-Ta);
			if(Tc == dThickSlabRightExt)
				Dom.LineTo(Bx+Term-Dis+dWFlange/2+dHunch, By - Tc - (Tc-dThickSlabRightExt));
			else
				Dom.LineTo(Bx+Term-Dis+dWFlange/2+dHunch, By - Ta - (dThickSlabRightExt - Ta));
			Dom.LineTo(Bx+Term-Dis+dWFlange/2, By-Tc);
			Dom.LineTo(Bx+Term+dWFlange/2, By-Tc);
		}
	}
	else
	{
		Dom.MoveTo(Bx,					     By);
		Dom.LineTo(Bx+Term,                  By);

		Dom.MoveTo(Bx,					     By);
		Dom.LineTo(Bx,						 By-Ta);
		Dom.LineTo(Bx+Term+dWidthBindConc/2, By-Tc);
	}

	*pDom << Dom;	
}

/*
void CAPlateDrawCalc::DrawGuardInputRightDim(CDomyun *pDom, CCentSeparation* pSep, double LEN)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CARoadOptionStd  *pStd    = m_pDataManage->GetOptionStd();	
	CPlateGirderApp  *pGir    = pDB->GetGirder(pDB->GetGirdersu()-1);	
	CPlateBasicIndex *pBx     = pGir->GetCrossBxAtMinFlangeWidth();

	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;
	
	long Size = pDB->GetQtyHDanNode();

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);

	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;

	double Bx = A[Size].x, By = A[Size].y;
	double D1 = pSep ? -pSep->m_D1 : 0;
	double W1 = pSep ? -pSep->m_W1 : 0;
	double W2 = pSep ? -pSep->m_W2 : 0;
	double W3 = pSep ? -pSep->m_W3 : 0;
	double W4 = pSep ? -pSep->m_W4 : 0;
	double W5 = pSep ? -pSep->m_W5 : 0;
	double H1 = pSep ? pSep->m_H1 : 0;
	double H2 = pSep ? pSep->m_H2 : 0;
	double H3 = pSep ? pSep->m_H3 : 0;
	double D2 = pSep ? LEN - fabs(D1+W1+W2+W3+W4+W5) : 0;
	double Width = pSep ? pSep->GetWidth() : 0;
	double Height = pSep ? pSep->GetHeight() : 0;
	double BH = 0;
	double NH = 0;
	double BWidth = 100;

	D1 = -D2;

	if(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && m_nGuardType <= 8)	BH =  170;
	if(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && m_nGuardType <= 8)	NH =  170;

	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Ta       = pDB->m_dThickSlabRight;
	double Tc       = pDB->m_dThickSlabBase;	
	double Dis      = pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;
	double UpFlangeT= pGir->GetThickFactByStation(pBx->GetStation(), G_F_U);
	double WebT     = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dWFlange = pGir->GetWidthFlangeUpper(pBx);
	double Term     = pDB->m_dWidthSlabRight;
	double dHunch	= 0;
	if(pDB->IsUserSlabHunch(FALSE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(pDB->m_dHunchArray.GetUpperBound());
	double dThickSlabRightExt = pDB->m_dThickSlabRightExt;
	double L1 = 0, L2 = 0, T3 = 0, uM = 0, BrLen = 0;
	double XH1 = 0;
	double XH2 = 0;
	double dQuater = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;

	BOOL	bBangEm			= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;
	BOOL	bNanGan			= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan;
	BOOL	bWalkLoad		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad;

	CDPoint B[10];
	pGir->GetHunchXyDom(pBx,B);

	CDomyun Dom(pDom);

	Dom.SetDirection("TOP");
	if(m_nGuardType != 99)
	{
		if(bBangEm || bNanGan)
			Dom.DimMoveTo( Bx, By + Height + BH + NH, 0 );
		else
			Dom.DimMoveTo( Bx, By + Height, 0 );

		Dom.DimLineTo( D1, 0, COMMA(fabs(D1)));
		Dom.DimLineTo( W5, 0, COMMA(fabs(W5)));
		Dom.DimLineTo( W4, 0, COMMA(fabs(W4)));
		Dom.DimLineTo( W3, 0, COMMA(fabs(W3)));
		Dom.DimLineTo( W2, 0, COMMA(fabs(W2)));
		Dom.DimLineTo( W1, 0, COMMA(fabs(W1)));

		if(bBangEm || bNanGan)
			Dom.DimMoveTo( Bx, By + Height + BH + NH, 1 );
		else
			Dom.DimMoveTo( Bx, By + Height, 1 );

		Dom.DimLineTo( -Width+D1, 1, COMMA(fabs(-Width+D1)) );
	}

	CString bodo1,bodo2;
	long nSize=pDB->GetQtyHDanNode();
	double Cy = 0;

	double TotalHLen = 0;
	for(int i=0;i<pDB->GetQtyHDanNode();i++)
		TotalHLen += pDB->GetLengthHDanDom(pBx, i);

	/////////////////////////////////////////////
	double bodoH = pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double dCen, dELoadDis;
	long nCount = 0;	
	double	dBridgeWidth = pDB->m_dBridgeWidth;
	for(i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
	{
		dCen = pDB->GetWidthSlabDom(pBx) / 2.0;
		dELoadDis = pData->m_ELoadDisArr[i];

		if((dBridgeWidth-Term-dQuater-WebT/2) < dELoadDis)
		{
			DArrELoadDis.Add(dELoadDis);			
			SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
		}
	}

	Cy = By-Tc;
	XH1 = By;

	// Slab Area Line
	Dom.MoveTo( Bx, By-Ta );
	Dom.LineTo( Bx-Term+dQuater+WebT/2, By-Ta );
	Dom.MoveTo( Bx-Term+Dis+dWFlange/2, By-Ta );
	Dom.LineTo( Bx-Term+Dis+dWFlange/2, By-Tc );

	double	dLoad1		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
	bodo1.Format("%.3f kN/㎡", tokNPM2(dLoad1));
	if(bWalkLoad)
	{
		double dBodoH	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
		double dBodoW	= frM(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadWidth);
		double T2	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		bodo2.Format("%.3f kN/㎡", tokNPM2(T2));

		Dom.GiSiArrowExtend(Bx+D1-Width, By+dBodoH, Dom.Always(10), -Dom.Always(20), 3, _T("보도하중"), bodo1);
		Dom.GiSiArrowExtend(Bx-Term+dQuater+WebT/2, By+dBodoH, Dom.Always(20), -Dom.Always(30), 3, _T("검토위치 보도하중"), bodo2);			
	}		

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo( Bx-Term, Cy, 0 );
	Dom.DimLineTo( (dWFlange/2-WebT/2)/2+WebT/2, 0, COMMA((dWFlange/2-WebT/2)/2+WebT/2));
	Dom.DimLineTo( (dWFlange/2-WebT/2)/2, 0, COMMA((dWFlange/2-WebT/2)/2));
	Dom.DimLineToExtend( Dis, 0, "", COMMA( Dis ) );
	if(pDB->IsUserSlabHunch(FALSE, pBx))
	{
		Dom.DimLineTo(dHunch, 0, Comma(dHunch));
		Dom.DimLineTo( Term-Dis-dWFlange/2-dHunch, 0, COMMA(Term-Dis-dWFlange/2-dHunch) );
	}
	else
		Dom.DimLineTo( Term-Dis-dWFlange/2, 0, COMMA(Term-Dis-dWFlange/2) );

	Dom.DimMoveTo( Bx, Cy, 1 );
	Dom.DimLineTo( -Term, 1,COMMA(Term));

	Dom.SetDirection("TOP");

	double Temp = 0;

	for(long k = DArrELoadDis.GetSize() - 1; k >= 0; k--)	//부가하중
	{
		if(pData->m_bExtraLoad && (dBridgeWidth-Term-dQuater-WebT/2) < DArrELoadDis[k])
		{
			double ELoadDis		= -(TotalHLen-frM(DArrELoadDis[k]));
			CString LoadTitle	= SArrELoadTitle[k];

			if(!(bBangEm && bNanGan))
				Dom.DimMoveTo( Bx+Temp, By + Height + BH + NH, 3 );
			else
				Dom.DimMoveTo( Bx+Temp, By + Height, 3 );

			Dom.DimLineTo(ELoadDis-Temp,3,COMMA(fabs(ELoadDis-Temp)));

			Temp = ELoadDis;
			
			Dom.TextOut(Bx+ELoadDis, By+Height+170, LoadTitle);
		}
	}

	Width = pSep ? -pSep->GetWidth() : 0;	
	Term  = -pDB->m_dWidthSlabRight;

	long nNumber = 0;
	CString szNumber;
	//GuardWall Area Line
	if(m_nGuardType == 1)
	{
		Dom.LineTo(Bx+D1, By+H1+H2, Bx+D1+W3+W2, By+H1+H2);
		Dom.LineTo(Bx+D1+W3, By+H1+H2, Bx+D1+W3, By+H1+H2+H3);
		Dom.LineTo(Bx+D1+W3+W2, By, Bx+D1+W3+W2, By+H1+H2);
		Dom.LineTo(Bx+D1+W3+W2, By+H1, Bx+D1+W3+W2+W1, By+H1);

		Dom.TextCircle(Bx+D1+W3/2,By+H1+H2+H3/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W2/3,By+H1+H2+H3/3, "2", -1);
		Dom.TextCircle(Bx+D1+(W3+W2)/2,By+(H1+H2)/2, "3", -1);
		Dom.TextCircle(Bx+D1+W3+W2+W1/3,By+H1+H2/3, "4", -1);
		Dom.TextCircle(Bx+D1+W3+W2+W1/2,By+H1/2, "5", -1);
		nNumber = 6;
	}
	if(m_nGuardType == 2)
	{
		if(fabs(W4)>0)
		{
			Dom.LineTo(Bx+D1, By+H1+H2, Bx+D1+W3+W2, By+H1+H2);
			Dom.LineTo(Bx+D1+W3, By+H1+H2, Bx+D1+W3, By+H1+H2+H3);

			Dom.TextCircle(Bx+D1+W3/2,By+H1+H2+H3/2, "1", -1);
			Dom.TextCircle(Bx+D1+W3+W2/3,By+H1+H2+H3/3, "2", -1);
			Dom.TextCircle(Bx+D1+(W3+W2)/2,By+(H1+H2)/2, "3", -1);
		}
		if(fabs(W5)>0)
		{
			Dom.LineTo(Bx+D1, By+H1, Bx+D1+W3+W1, By+H1);
			Dom.LineTo(Bx+D1+W3, By+H1, Bx+D1+W3, By+H1+H2);

			Dom.TextCircle(Bx+D1+W3/2,By+H1+H2/2, "1", -1);
			Dom.TextCircle(Bx+D1+W3+W1/3,By+H1+H2/3, "2", -1);
			Dom.TextCircle(Bx+D1+(W3+W1)/2,By+H1/2, "3", -1);
		}
		nNumber = 4;
	}
	if(m_nGuardType == 3)
	{
		Dom.LineTo(Bx+D1, By+H1, Bx+D1+W3+W1, By+H1);
		Dom.LineTo(Bx+D1+W3, By+H1, Bx+D1+W3, By+H1+H2);

		Dom.TextCircle(Bx+D1+W3/2,By+H1+(H2+H3)/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W1/3,By+H1+H2/3, "2", -1);
		Dom.TextCircle(Bx+D1+(W3+W1)/2,By+H1/2, "3", -1);

		nNumber = 4;
	}
	if(m_nGuardType == 4)
	{
		Dom.TextCircle(Bx+D1+Width/2,By+(H1+H2+H3)/2, "1", -1);
		nNumber = 2;
	}
	if(m_nGuardType == 5)
	{
		Dom.LineTo(Bx+D1+W1, By+H1+H2, Bx+D1+W1+W2+W3+W2, By+H1+H2);
		Dom.LineTo(Bx+D1, By+H1, Bx+D1+Width, By+H1);

		Dom.TextCircle(Bx+D1+W1+W2+W3/2,By+H1+H2+H3/2, "1", -1);
		Dom.TextCircle(Bx+D1+W1+W2+W3/2,By+H1+H2/2, "2", -1);
		Dom.TextCircle(Bx+D1+W1+W2+W3/2,By+H1/2, "3", -1);

		nNumber = 4;
	}
	if(m_nGuardType == 6)
	{
		Dom.LineTo(Bx+D1+W3, By, Bx+D1+W3, By+H2);

		Dom.TextCircle(Bx+D1+W3/2,By+(H2+H3)/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W1/3,By+H2/3, "2", -1);
		nNumber = 3;
	}
	if(m_nGuardType == 7)
	{
		Dom.LineTo(Bx+D1+W3, By, Bx+D1+W3, By+H2);

		Dom.TextCircle(Bx+D1+W3/2,By+H2/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W1/3,By+H2/3, "2", -1);
		nNumber = 3;
	}
	if(m_nGuardType == 8)
	{
		Dom.LineTo(Bx+D1+W3, By, Bx+D1+W3, By+H1);

		Dom.TextCircle(Bx+D1+W3/2,By+(H1+H3)/2, "1", -1);
		Dom.TextCircle(Bx+D1+W3+W1/2,By+H1/2, "2", -1);
		nNumber = 3;
	}
	if(m_nGuardType == 100)
	{
		Dom.TextOut(Bx+Term/2,By+(H1+H2+H3)/2,"이 방호벽 모양은 지원할 수 없습니다.");
	}	

	/// Slab Area Line
	if(pDB->IsUserSlabHunch(FALSE, pBx) && m_nGuardType != 100)
	{
		if(nNumber==0) nNumber=1;

		double dTemp = ABS(Term) - (dQuater+WebT/2) - (dQuater+Dis) - dHunch;
		Dom.LineTo(Bx - dTemp, By-Ta, Bx-dTemp, By-Ta-(dThickSlabRightExt -Ta));
		if(Tc != dThickSlabRightExt)
			Dom.LineTo(Bx-dTemp, By-Ta-(dThickSlabRightExt -Ta), Bx-dTemp-dHunch, By-Ta-(dThickSlabRightExt -Ta));

		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+(Term)/2,XH1-Ta/2, szNumber, -1);

		if(dThickSlabRightExt-Ta>0)
		{		
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+Term+(dQuater+WebT/2)+(dQuater+Dis) + dHunch + dTemp/6, By - Ta - (dThickSlabRightExt-Ta)/2, szNumber, -1);

			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+Term+(dQuater+WebT/2)+(dQuater+Dis) + dHunch/2, By - Ta - (dThickSlabRightExt-Ta)/2, szNumber, -1);
		}

		if(Tc != dThickSlabRightExt)
		{
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+Term+(dQuater+WebT/2)+(dQuater+Dis) + dHunch/3, By - Tc + (Tc-dThickSlabRightExt)/2, szNumber, -1);
		}
		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+Term+(dQuater+WebT/2)+(dQuater+Dis)/2, XH1-Tc-(Ta-Tc)/2, szNumber, -1);

		if(fabs(Term) > fabs(Width+D1))
		{
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+D1+W3+W4+W5+(Term-Width-D1)/2,XH1+Pavement/2, szNumber, -1);
		}
	
	}
	else
	{
		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+Term/2,XH1-Ta/2, szNumber, -1);

		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+(Term+Dis+dQuater+dWFlange/2+WebT/2)*0.8, XH1-Tc-(Ta-Tc)/2, szNumber, -1);

		szNumber.Format("%d", nNumber++);
		Dom.TextCircle(Bx+Term+(dQuater+WebT/2)+(dQuater+Dis)/2, XH1-Tc-(Ta-Tc)/2, szNumber, -1);

		if(fabs(Term) > fabs(Width+D1))
		{
			szNumber.Format("%d", nNumber++);
			Dom.TextCircle(Bx+D1+W3+W4+W1+(Term-Width-D1)/2,XH1+Pavement/2, szNumber, -1);
		}	
	}

	Dom.SetDirection("RIGHT");
	
	double DH = B[9].y-B[8].y;
	if(DH<0)
		DH = 0;
	if(pDB->IsUserSlabHunch(FALSE, pBx))
	{
		Dom.DimMoveTo(Bx, By-Tc-(DH), 0);
		Dom.DimLineToExtend(Tc- dThickSlabRightExt, 0, "", Comma(Tc-dThickSlabRightExt));
		Dom.DimLineTo(ABS(dThickSlabRightExt-Ta), 0, Comma(ABS(dThickSlabRightExt-Ta)));
		Dom.DimLineTo(DH, 0, COMMA(DH));
		Dom.DimLineTo(Ta, 0, COMMA(Ta));
	}
	else
	{
		Dom.DimMoveTo( Bx, By-Ta-(DH), 0 );
		Dom.DimLineTo( DH, 0, COMMA(DH));
		Dom.DimLineTo( Ta, 0, COMMA(Ta));
		Dom.DimLineTo( Pavement, 0, COMMA(Pavement));
	}

	if(bWalkLoad)
		Dom.DimLineTo( pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH, 0, COMMA(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH));
	else
		Dom.DimLineTo( Pavement, 0, COMMA(Pavement));		
		
	Dom.DimMoveTo( Bx, By-Ta-(DH), 1 );
	if(bWalkLoad)
		Dom.DimLineTo( pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH+Ta+(DH), 1, COMMA(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH+Ta+(DH)));
	else
		Dom.DimLineTo( Pavement+Ta+(DH), 1, COMMA(Pavement+Ta+(DH)) );

	double NanGanDis	= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
	double BangEmDis	= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;

	Dom.SetDirection("TOP");

	if(bBangEm && !bNanGan)	//방음벽일 경우
	{
		Dom.DimMoveTo(Bx, By+Height+30,0);
		Dom.DimLineTo(BangEmDis,0,COMMA(fabs(BangEmDis)));
		Dom.TextOut(Bx+BangEmDis+100, By+Height+170, "방음벽하중");
	}

	if(!bBangEm && bNanGan)	//난간일 경우
	{
		Dom.DimMoveTo(Bx, By+Height+30,0);
		Dom.DimLineTo(NanGanDis,0,COMMA(fabs(NanGanDis)));
		Dom.TextOut(Bx+NanGanDis+100, By+Height+170, "난간하중");
	}

	if(!bWalkLoad)
	{
		double dByH	= 0;
		double dDimDis = 0;
		double DBLoadDis		= -dDis_Car *1000-LEN;
		if(m_nGuardType == 99)	DBLoadDis		= -dDis_Car*1000;

		Dom.DimMoveTo( Bx, By + Height + BH + NH, 2 );	//차량하중표시
		Dom.DimLineTo( DBLoadDis, 2, COMMA(fabs(DBLoadDis))); 
		Dom.TextOut(Bx+DBLoadDis, By+Pavement+400, "Pr");

		dByH += 100;
		dDimDis = Bx+DBLoadDis;
		DBLoadDis = -(dDis_Trailer-dDis_Car)*1000; 

		if(pData->m_bTank)
		{			
			DBLoadDis		= -(dDis_Tank-dDis_Car)*1000;//dDis_Tank *1000+LEN;
			//if(m_nGuardType == 99)	DBLoadDis		= -dDis_Tank*1000;

			Dom.DimMoveTo( dDimDis, By + Height + BH + NH, 2 );	//차량하중표시
			Dom.DimLineTo( DBLoadDis, 2, COMMA(fabs(DBLoadDis)));  
			Dom.TextOut(dDimDis+DBLoadDis, By+Pavement+400+dByH, "Tank");

			dByH += 100;			
			dDimDis = dDimDis+DBLoadDis;
			DBLoadDis = -(dDis_Trailer-dDis_Tank)*1000; 
		}
		if(pData->m_bTrailer)
		{			
			//DBLoadDis		= dDis_Trailer *1000+LEN;
			//if(m_nGuardType == 99)	DBLoadDis		= -dDis_Trailer*1000;
			 
			Dom.DimMoveTo( dDimDis, By + Height + BH + NH, 2 );	//차량하중표시
			Dom.DimLineTo( DBLoadDis, 2, COMMA(fabs(DBLoadDis)));  
			Dom.TextOut(DBLoadDis+dDimDis, By+Pavement+400+dByH, "Trailer");

			dByH += 100;
		}
	}

	*pDom << Dom;	
}
*/

void CAPlateDrawCalc::DrawGuardDivideDimLeft(CDomyun *pDom, CCentSeparation* pSep, double LEN, BOOL bExcel)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CARoadOptionStd  *pStd    = m_pDataManage->GetOptionStd();	
	CPlateGirderApp  *pGir    = pDB->GetGirder(0);	
	CPlateBasicIndex *pBx     = pGir->GetCrossBxAtMinFlangeWidth();

	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;
	
	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, A);
	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;

	if(m_nGuardType==99)	LEN = 0;

	double Bx=A[0].x, By = A[0].y;			// Slab 끝단 위
	double D1 = pSep ? pSep->m_D1 : 0;
	double W1 = pSep ? pSep->m_W1 : 0;
	double W2 = pSep ? pSep->m_W2 : 0;
	double W3 = pSep ? pSep->m_W3 : 0;
	double W4 = pSep ? pSep->m_W4 : 0;
	double W5 = pSep ? pSep->m_W5 : 0;
	double H1 = pSep ? pSep->m_H1 : 0;
	double H2 = pSep ? pSep->m_H2 : 0;
	double H3 = pSep ? pSep->m_H3 : 0;	
	double Width = pSep ? pSep->GetWidth() : 0;
	double Height = pSep ? pSep->GetHeight() : 0;
		
	double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double dHunch	= 0;
	if(pDB->IsUserSlabHunch(TRUE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(0);
	double dThickSlabLeftExt = pDB->m_dThickSlabLeftExt;

	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Ta		= pDB->m_dThickSlabLeft;
	double Tc		= pDB->m_dThickSlabBase;
	double Dis		= pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;;
	double WebT		= pGir->GetThickJewon(G_W, pBx);
	double Term		= pDB->m_dWidthSlabLeft;
	double dMomentLen	= Term - WebT/2 - dQuater;

	double dWidthBindConc	= pDB->m_BindConc.m_dWidth;
	double dHeightBindConc	= pDB->m_BindConc.m_dHeight;
	
	CDomyun Dom(pDom);

	if(bExcel)	Dom.SetDimExo(5);

	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double	dApplyDis	= 0;
	double	dELoadDis	= 0;
	
	CString	sL[15], sH[15];
	long n = 0;
	for(n=0; n<15; n++)
	{
		sL[n].Format("L%d", n+1);
		sH[n].Format("H%d", n+1);
	}

	for(long i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
	{
		dApplyDis = pDB->IsTUGir() ? Term-dWidthBindConc/2 : Term-WebT/2-dQuater;
		dELoadDis = pData->m_ELoadDisArr[i];
		if(dELoadDis<dApplyDis && dELoadDis>0)
		{
			DArrELoadDis.Add(dELoadDis);			
			SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
		}
	}
	AscendSort(DArrELoadDis, SArrELoadTitle);
	
	BOOL	bBangEm		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
	BOOL	bNanGan		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan;
	BOOL	bWalkLoad	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad;

	BOOL	bAddLoad	=  SArrELoadTitle.GetSize()>0 ? TRUE : FALSE;

	double	dDimGap		= (bBangEm || bNanGan || bAddLoad) ? 170 : 0;
	long	nDimCol		= 0;

	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("TOP");
	
	if(bBangEm)
	{
		double	BangEmDis	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;

		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(BangEmDis, nDimCol, COMMA(fabs(BangEmDis)));
		Dom.TextOut(Bx+BangEmDis, By+Height+dDimGap, "방음벽하중");

		nDimCol++;
	}
	if(bNanGan)
	{
		double	NanGanDis	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;

		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(NanGanDis, nDimCol, COMMA(NanGanDis));
		Dom.TextOut( Bx+NanGanDis, By+Height+dDimGap, "난간하중");

		nDimCol++;
	}

	double	Temp	= 0;
	BOOL	bUsed	= FALSE;
	for(n = 0; n < DArrELoadDis.GetSize(); n++)
	{
		if(pData->m_bExtraLoad) 
		{
			double ELoadDis		= DArrELoadDis[n];
			CString LoadTitle	= SArrELoadTitle[n];
			double	dHtemp		= (ELoadDis > (pSep?LEN:0)) ? Pavement : Height;

			if(!bUsed)
				Dom.DimMoveToExt( Bx+Temp, By+Height+dDimGap, nDimCol, "부가하중", "");
			else
				Dom.DimMoveTo( Bx+Temp, By+Height+dDimGap, nDimCol);
			Dom.DimLineTo(ELoadDis-Temp, nDimCol, COMMA(ELoadDis-Temp));
			Temp = ELoadDis;
			Dom.TextOut(Bx+ELoadDis, By+dHtemp+dDimGap, LoadTitle);
			
			bUsed	= TRUE;
		}
	}
	if(bUsed)	nDimCol++;

	long	nLIdx		= 0;
	double	dDisLenIdx	= 0;
	double  dMomentH	=  0;
	double  dMomentV	=  0;
	double	dDeep		=  0;
	
	dDeep	 = pDB->m_BindConc.m_dDeep;
	dMomentH = dWidthBindConc/4;
	dMomentV = Tc - dDeep;

	if(m_nGuardType==99)	LEN = 0;

	if(m_nGuardType != 99)
	{
		Dom.DimMoveTo( Bx, By+Height+dDimGap, nDimCol);

		if(D1!=0)
			Dom.DimLineToExtend( D1, nDimCol, COMMA(D1), bExcel?sL[nLIdx++]:"");
		if(W1!=0)	
			Dom.DimLineToExtend( W1, nDimCol, COMMA(W1), bExcel?sL[nLIdx++]:"");
		if(W2!=0)		
			Dom.DimLineToExtend( W2, nDimCol, COMMA(W2), bExcel?sL[nLIdx++]:"");
		if(W3!=0)		
			Dom.DimLineToExtend( W3, nDimCol, COMMA(W3), bExcel?sL[nLIdx++]:"");
		if(W4!=0)		
			Dom.DimLineToExtend( W4, nDimCol, COMMA(W4), bExcel?sL[nLIdx++]:"");
		if(W5!=0)		
			Dom.DimLineToExtend( W5, nDimCol, COMMA(W5), bExcel?sL[nLIdx++]:"");

		if(pDB->IsTUGir())
		{
			dDisLenIdx = Term-LEN-dWidthBindConc/2;
			Dom.DimLineToExtend(dDisLenIdx+dMomentH, nDimCol, COMMA(dDisLenIdx+dMomentH), bExcel?sL[nLIdx++]:"");
		}
		else
		{
			dDisLenIdx	= Term-WebT/2-dQuater-LEN;
			Dom.DimLineToExtend(dDisLenIdx, nDimCol, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
		}		
	}
	else
	{
		dDisLenIdx	= pDB->IsTUGir() ? Term-dWidthBindConc/2-LEN : Term-WebT/2-dQuater-LEN;
		if(dDisLenIdx!=0)
		{
			Dom.DimMoveTo(Bx, By+Pavement+170, 1);
			Dom.DimLineToExtend(dDisLenIdx, 1, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
		}
	}

	//////////////////////////////////////////////////////////////////////////

	Dom.SetDirection("BOTTOM");
	if(!pDB->IsTUGir())
	{
		Dom.DimMoveTo( Bx, By-Tc-300, 0 );

		dDisLenIdx	= Term-WebT/2-dQuater*2-Dis-dHunch;
		if(dDisLenIdx!=0)
			Dom.DimLineToExtend(dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");

		dDisLenIdx	= dHunch;
		if(dDisLenIdx!=0)	
			Dom.DimLineToExtend(dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");

		dDisLenIdx	= dQuater+Dis;
		if(dDisLenIdx!=0)
			Dom.DimLineToExtend(dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");

		dDisLenIdx	= dQuater;
		if(dDisLenIdx!=0)
			Dom.DimLineToExtend(dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
	}
	else
	{
		Dom.DimMoveTo( Bx+Term-dWidthBindConc/2, By-Tc, 0 );
		
		dDisLenIdx	= Term-dWidthBindConc/2;
		if(dDisLenIdx!=0)
		{
			Dom.DimLineToExtend(-dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
			Dom.DimMoveTo( Bx+Term-dWidthBindConc/2, By-Tc, 0 );
		}
		Dom.DimLineToExtend(dWidthBindConc/4, 0, COMMA(dWidthBindConc/4), bExcel?sL[nLIdx++]:"");		
	}

	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("LEFT");
	long	nHIdx	= 0;

	Dom.DimMoveTo(Bx, By-Ta, 0);

	double dDisHIdx	= Ta;

	if(dDisHIdx!=0)
		Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
				
	if(m_nGuardType != 99)
	{
		dDisHIdx	= H1;
		if(dDisHIdx!=0)
			Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
		dDisHIdx	= H2;
		if(dDisHIdx!=0)
			Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
		dDisHIdx	= H3;
		if(dDisHIdx!=0)
			Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
	}

	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(Bx+Term, By-Tc, 0);

	dDisHIdx	= Tc;
	if(dDisHIdx!=0)
		Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");

	dDisHIdx	= bWalkLoad ? pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH : Pavement;
	if(dDisHIdx!=0)
		Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
	if(dDeep>0)
	{
		Dom.DimMoveTo(Bx+Term, By-Tc, 1);
		Dom.DimLineTo(dDeep, 1, COMMA(dDeep));
		Dom.DimLineToExtend(Tc-dDeep, 1, COMMA(Tc-dDeep), bExcel?sH[nHIdx++]:"");
	}

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(Bx, By-Ta, 0);

	dDisHIdx	= Tc-dThickSlabLeftExt;
	if(dDisHIdx>0)
		Dom.DimLineToExtend(-dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");


	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("TOP");

	if(!bWalkLoad)
	{
		double	dDis_WtoW	= 1.8;
		if(pData->m_bTank && !pData->m_bTrailer)
			dDis_WtoW -= (dDis_Tank-dDis_Car);
		else if(!pData->m_bTank && pData->m_bTrailer)
			dDis_WtoW -= (dDis_Trailer-dDis_Car);
		else if(pData->m_bTank && pData->m_bTrailer)
		{
			double	dDisTemp = (dDis_Tank>=dDis_Trailer) ? dDis_Tank : dDis_Trailer;
			dDis_WtoW -= (dDisTemp-dDis_Car);
		}

		double	DBLoadDis	= frM(dDis_Car);
		double	DBLoadDis_2nd	= frM(dDis_Car+1.8);
		BOOL	bDBLoad_2nd	= DBLoadDis_2nd < Term-Dis-dQuater-LEN;
			
		Dom.DimMoveTo(Bx+LEN, By+Pavement+170, 0);
		Dom.DimLineTo(DBLoadDis, 0, COMMA(DBLoadDis)); 
		Dom.TextOut(Bx+LEN+DBLoadDis, By+Pavement+170, bDBLoad_2nd?"Pr1":"Pr");

		double	dDimDis = Bx+LEN+DBLoadDis;
		DBLoadDis = (dDis_Trailer-dDis_Car)*1000;
		if(pData->m_bTank)
		{			
			DBLoadDis		= (dDis_Tank-dDis_Car)*1000;

			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(DBLoadDis)); 
			Dom.TextOut(dDimDis+DBLoadDis, By+Pavement+170, "Tk");

			dDimDis = dDimDis+DBLoadDis;
			DBLoadDis = (dDis_Trailer-dDis_Tank)*1000; 
		}
		if(pData->m_bTrailer)
		{			
			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(DBLoadDis)); 
			Dom.TextOut(DBLoadDis+dDimDis, By+Pavement+170, "Tr");
		}

		if(bDBLoad_2nd)
		{
			Dom.DimLineTo(frM(dDis_WtoW), 0, COMMA(frM(dDis_WtoW))); 
			Dom.TextOut(Bx+LEN+DBLoadDis_2nd, By+Pavement+170, "Pr2");
		}
	}
	
	if(pSep && bWalkLoad)
	{
		CString bodo1,bodo2;
		double	dBodoLen	= 0;
		double	dT1			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
		double	dT2			= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2;
		double	dT2_		= 0;
		double	dBodoH		= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
		double	dBodoHDanLen	= pDB->GetLengthHDanDom(pBx,1);
		if(LEN+dBodoHDanLen > Term+dQuater)
		{	
			dBodoLen	= Term+dQuater-LEN;
			dT2_		= dT2 + fabs(dT2-dT1)*dBodoLen/dBodoHDanLen;
		}
		else
		{
			dBodoLen	= dBodoHDanLen;
			dT2_		= dT2;
		}

		double	dHeightStt	= dT1!=0 ? dBodoH : 0;
		double	dHeightEnd	= dT1!=0 ? dBodoH + (dT2/dT1-1)*dBodoH*dBodoLen/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);

		bodo1.Format("%.3f kN/m2", tokNPM2(dT1));
		bodo2.Format("%.3f kN/m2", tokNPM2(dT2_));

		Dom.GiSiArrowExtend(Bx+LEN, By+dHeightStt, Dom.Always(10), Dom.Always(20), 1, _T("보도하중 T1"), bodo1);	
		Dom.GiSiArrowExtend(Bx+LEN+dBodoLen, By+dHeightEnd, Dom.Always(10), Dom.Always(30), 1, _T("보도하중 T2"), bodo2);			
	}

	Dom.CutLightning(Bx+Term, By+200, Bx+Term, By-Tc-200, TRUE, 0.75);

	if(!pDB->IsTUGir())
		Dom.LineTo(Bx+Term-WebT/2-dQuater, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH:By+Pavement, Bx+Term-WebT/2-dQuater, By-Tc);
	else
		Dom.LineTo(Bx+Term-dWidthBindConc/2, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH:By, Bx+Term-dWidthBindConc/2, By-Tc);

	//////////////////////////////////////////////////////////////////////////
	// Divide Area
	CString sNum[20];
	for(n=0; n<20; n++)
		sNum[n].Format("%d", n+1);

	long nA	= 0;
	Bx += D1;
	if(m_nGuardType == 1)
	{
		pDom->LineTo(Bx,		By+H1+H2,	Bx+W3+W4,		By+H1+H2);
		pDom->LineTo(Bx+W3,		By+H1+H2,	Bx+W3,			By+H1+H2+H3);
		pDom->LineTo(Bx+W3+W4,	By,			Bx+W3+W4,		By+H1+H2);
		pDom->LineTo(Bx+W3+W4,	By+H1,		Bx+W3+W4+W5,	By+H1);

		pDom->TextCircle(Bx+W3/2,		By+H1+H2+H3/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W4/3,	By+H1+H2+H3/3,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+(W3+W4)/2,	By+(H1+H2)/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W4+W5/3,	By+H1+H2/3,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W4+W5/2,	By+H1/2,		sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 2)
	{
		pDom->LineTo(Bx+W3,By+H1+H2+H3,Bx+W3,By+H1+H2);
		pDom->LineTo(Bx,By+H1+H2,Bx+W3+W4,By+H1+H2);
		pDom->TextCircle(Bx+W3/2,By+H1+H2+H3/2,sNum[nA++],-1,FALSE,TRUE);
		pDom->TextCircle(Bx+W3+W4/3,By+H1+H2+H3/3,sNum[nA++],-1,FALSE,TRUE);
		pDom->TextCircle(Bx+(W3+W4)/2,By+(H2+H1)/2,sNum[nA++],-1,FALSE,TRUE);
	}
	else if (m_nGuardType == 3)
	{
		pDom->LineTo(Bx,By+H1,Bx+W3+W5,By+H1);
		pDom->LineTo(Bx+W3,By+H1+H2,Bx+W3,By+H1);
		pDom->TextCircle(Bx+W3/2,By+(H1+H2+H3)/2,sNum[nA++],-1,FALSE,TRUE);
		pDom->TextCircle(Bx+W3+W5/3,By+H1+H2/3,sNum[nA++],-1,FALSE,TRUE);
		pDom->TextCircle(Bx+(W3+W5)/2,By+H1/2,sNum[nA++],-1,FALSE,TRUE);
	}
	else if(m_nGuardType == 4)
	{
		pDom->TextCircle(Bx+Width/2,	By+Height/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		
	}
	else if(m_nGuardType == 5)
	{
		pDom->LineTo(Bx+W1,			By+H1+H2,	Bx+W1+W2+W3+W4,		By+H1+H2);
		pDom->LineTo(Bx+W1+W2,		By+H1+H2,	Bx+W1+W2,			By+H1+H2+H3);
		pDom->LineTo(Bx+W1+W2+W3,	By+H1+H2,	Bx+W1+W2+W3,		By+H1+H2+H3);
		pDom->LineTo(Bx+W1,			By+H1,		Bx+W1,				By+H1+H2);
		pDom->LineTo(Bx+W1+W2+W3+W4,By+H1,		Bx+W1+W2+W3+W4,		By+H1+H2);
		pDom->LineTo(Bx,			By+H1,		Bx+W1+W2+W3+W4+W5,	By+H1);
		
		//////////////////////////////////////////////////////////////////////////
		
		pDom->TextCircle(Bx+W1+W2+W3/2,			By+H1+H2+H3/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W1+W2*2/3,			By+H1+H2+H3/3,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W1+W2+W3+W4/3,		By+H1+H2+H3/3,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W1+W2+W3/2,			By+H1+H2/2,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W1*2/3,				By+H1+H2/3,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W1+W2+W3+W4+W5/3,	By+H1+H2/3,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W1+W2+W3/2,			By+H1/2,		sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 6)
	{
		pDom->LineTo(Bx+W3,By,Bx+W3,By+H2+H3);
		pDom->TextCircle(Bx+W3/2,By+(H2+H3)/2,sNum[nA++],-1,FALSE,TRUE);
		pDom->TextCircle(Bx+W3+W5/2,By+H2/3,sNum[nA++],-1,FALSE,TRUE);
	}
	else if(m_nGuardType == 7)
	{
		pDom->LineTo(Bx+W3,By,Bx+W3,By+H2);
		pDom->TextCircle(Bx+W3/2,By+H2/2,sNum[nA++],-1,FALSE,TRUE);
		pDom->TextCircle(Bx+W3+W5/2,By+H2/3,sNum[nA++],-1,FALSE,TRUE);
	}
	else if(m_nGuardType == 8)
	{
		pDom->LineTo(Bx+W3, By, Bx+W3, By+H1);

		pDom->TextCircle(Bx+W3/2,		By+(H1+H3)/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W5/2,	By+H1/2,		sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 100)
	{
		pDom->TextCircle(Bx+Term/2,By+H1/2,"이 방호벽 모양은 지원할 수 없습니다.");
	}

	Bx -= D1;
	double	dDisTemp1=0, dDisTemp2=0, dDisTemp3=0;

	if(!pDB->IsTUGir())
	{
		if(pDB->m_bUserSlabHunch)
		{
			dDisTemp1	= Term - WebT/2 - 2*dQuater - Dis - dHunch;
			dDisTemp2	= Term - WebT/2 - 2*dQuater - Dis;
			dDisTemp3	= Term - WebT/2 - dQuater;

			// Line
			pDom->LineTo(Bx, By-Ta, Bx+dDisTemp3, By-Ta);
			/*if(Tc != dThickSlabLeftExt)
			{
				pDom->LineTo(Bx+dDisTemp2, By-Ta, Bx+dDisTemp2, By-Tc);
				pDom->LineTo(Bx+dDisTemp1, By-dThickSlabLeftExt, Bx+dDisTemp2, By-dThickSlabLeftExt);
				pDom->LineTo(Bx+dDisTemp1, By-Ta, Bx+dDisTemp1, By-dThickSlabLeftExt);
			}
			*/
			//else
			//{
				pDom->LineTo(Bx+dDisTemp1, By-Ta, Bx+dDisTemp1, By-Tc);
			//}

			// Numbering
			pDom->TextCircle(Bx+dDisTemp3/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			/*if(Tc != dThickSlabLeftExt)
			{
				pDom->TextCircle(Bx+dDisTemp1+(dDisTemp2-dDisTemp1)*2/3, By-dThickSlabLeftExt-(Tc-dThickSlabLeftExt)/3, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx+dDisTemp2+(dDisTemp3-dDisTemp2)/2, By-Ta-(Tc-Ta)/2, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx+dDisTemp1*2/3, By-Ta-(Tc-dThickSlabLeftExt)/3, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx+dDisTemp1+(dDisTemp2-dDisTemp1)/2, By-Ta-(Tc-dThickSlabLeftExt)/2, sNum[nA++],	-1,	FALSE,	TRUE);
			}
			*/
			//else
			//{
				pDom->TextCircle(Bx+dDisTemp1*2/3, By-Ta-(Tc-Ta)/3, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx+dDisTemp1+(dDisTemp3-dDisTemp1)/2, By-Ta-(Tc-Ta)/2, sNum[nA++],	-1,	FALSE,	TRUE);
			//}
		}
		else
		{
			dDisTemp1	= Term - WebT/2 - 2*dQuater - Dis;
			dDisTemp2	= Term - WebT/2 - dQuater;

			// Line
			pDom->LineTo(Bx, By-Ta, Bx+dDisTemp2, By-Ta);
			pDom->LineTo(Bx+dDisTemp1, By-Ta, Bx+dDisTemp1, By-Tc);

			// Numbering
			pDom->TextCircle(Bx+dDisTemp2/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx+dDisTemp1*2/3, By-Ta-(Tc-Ta)/3, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx+dDisTemp1+(dDisTemp2-dDisTemp1)/2, By-Ta-(Tc-Ta)/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}			

		dDisTemp1	= Term - LEN - WebT/2 - dQuater;
		if(bWalkLoad)
		{
			double dWalkLoadH	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
			pDom->TextCircle(Bx+LEN+dDisTemp1/2, By+dWalkLoadH/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}
		else
			pDom->TextCircle(Bx+LEN+dDisTemp1/2, By+Pavement/2, sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else
	{
		if(Tc==Ta)
		{
			dDisTemp1	= Term - dWidthBindConc/2;
			pDom->TextCircle(Bx+dDisTemp1/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->LineTo(Bx+dDisTemp1+dMomentH,By,Bx+dDisTemp1+dMomentH,By-dMomentV);
			pDom->TextCircle(Bx+dDisTemp1+dMomentH/2,By-dMomentV/2,sNum[nA++],-1,FALSE,TRUE);

			dDisTemp1	= Term - LEN - dWidthBindConc/4;
			if(bWalkLoad)
			{
				double dWalkLoadH	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
				pDom->TextCircle(Bx+LEN+dDisTemp1/2, By+dWalkLoadH/2, sNum[nA++],	-1,	FALSE,	TRUE);
			}
			else
				pDom->TextCircle(Bx+LEN+dDisTemp1/2, By+Pavement/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}
		else
		{
			dDisTemp1	= Term - dWidthBindConc/2;

			// Line			
			pDom->LineTo(Bx, By-Ta, Bx+dDisTemp1, By-Ta);
			double dLTScale = pDom->GetLTScale();
			pDom->SetLTScale(Dom.Always(5));
			pStd->SetEnvLType(pDom, LT_CENTER_LINE);
			pDom->LineTo(Bx+dDisTemp1+dMomentH,By+Pavement+Dom.Always(5),Bx+dDisTemp1+dMomentH,By-dMomentV-dHeightBindConc-300);
			pStd->SetEnvLType(pDom, LT_SOLID);
			pDom->SetLTScale(dLTScale);
		
			// Numbering
			pDom->TextCircle(Bx+dDisTemp1/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx+dDisTemp1*2/3, By-Ta-(Tc-Ta)/3, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx+dDisTemp1+dMomentH/2,By-dMomentV/2,sNum[nA++],-1,FALSE,TRUE);
			
			dDisTemp1	= Term - LEN - dWidthBindConc/4;
			if(bWalkLoad)
			{
				double dWalkLoadH	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH;
				pDom->TextCircle(Bx+LEN+dDisTemp1/2, By+dWalkLoadH/2, sNum[nA++],	-1,	FALSE,	TRUE);
			}
			else
				pDom->TextCircle(Bx+LEN+dDisTemp1/2, By+Pavement/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}

	}

	*pDom << Dom;
}


void CAPlateDrawCalc::DrawGuardDivideDimRight(CDomyun *pDom, CCentSeparation* pSep, double LEN, BOOL bExcel)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CARoadOptionStd  *pStd    = m_pDataManage->GetOptionStd();	
	CPlateGirderApp  *pGir    = pDB->GetGirder(pDB->GetGirdersu()-1);	
	CPlateBasicIndex *pBx     = pGir->GetCrossBxAtMinFlangeWidth();	

	double	dDis_Car		= m_dDBLoadCar;
	double	dDis_Tank		= m_dDBLoadTank;
	double	dDis_Trailer	= m_dDBLoadTrailer;
	long	Size			= pDB->GetQtyHDanNode();

	CDPoint A[HDANLINESU];
	pDB->GetXyPaveUpperDom(pBx, A);
	for(long cnt=0; cnt<HDANLINESU; cnt++)
		A[cnt].y -= pDB->m_dThickPave;

	double Pavement = pDB->m_dThickPave;		// 포장 두께
	double Bx = A[Size].x, By = A[Size].y;
	double D1 = pSep ? -pSep->m_D1 : 0;
	double W1 = pSep ? -pSep->m_W1 : 0;
	double W2 = pSep ? -pSep->m_W2 : 0;
	double W3 = pSep ? -pSep->m_W3 : 0;
	double W4 = pSep ? -pSep->m_W4 : 0;
	double W5 = pSep ? -pSep->m_W5 : 0;
	double H1 = pSep ? pSep->m_H1 : 0;
	double H2 = pSep ? pSep->m_H2 : 0;
	double H3 = pSep ? pSep->m_H3 : 0;
	double Width = pSep ? pSep->GetWidth() : 0;	
	double Height = pSep ? pSep->GetHeight() : 0;
	double D2 = pSep ? LEN-Width+fabs(D1) : 0;
	
	D1 = -D2;

	double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double dHunch	= 0;
	if(pDB->IsUserSlabHunch(FALSE, pBx))
		dHunch	= pDB->m_dHunchArray.GetAt(pDB->m_dHunchArray.GetUpperBound());
	double dThickSlabRightExt = pDB->m_dThickSlabRightExt;
	
	double Ta	 = pDB->m_dThickSlabRight;
	double Tc	 = pDB->m_dThickSlabBase;
	double Dis	 = pDB->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;;
	double WebT  = pGir->GetThickJewon(G_W, pBx);
	double Term  = pDB->m_dWidthSlabRight;
	
	double dWidthBindConc	= pDB->m_BindConc.m_dWidth;
	double dHeightBindConc	= pDB->m_BindConc.m_dHeight;
	
	CDomyun Dom(pDom);

	if(bExcel)	Dom.SetDimExo(5);

	CDoubleArray DArrELoadDis;
	CStringArray SArrELoadTitle;
	double	dApplyDis	= 0;
	double	dELoadDis	= 0;
	double	dBridgeWidth = pDB->m_dBridgeWidth;
	
	CString	sL[15], sH[15];

	double  dMomentH	=  0;
	double  dMomentV	=  0;
	double	dDeep		=  0;

	dDeep	 = pDB->m_BindConc.m_dDeep;
	dMomentH = dWidthBindConc/4;
	dMomentV = Tc - dDeep;
	
	long  n =0;
	for(n=0; n<15; n++)
	{
		sL[n].Format("L%d", n+1);
		sH[n].Format("H%d", n+1);
	}
	long i = 0;
	for(i = 0; i < pData->m_ELoadTitleArr.GetSize(); i++)
	{
		dApplyDis	= pDB->IsTUGir() ? dBridgeWidth-(Term-dWidthBindConc/2) : dBridgeWidth-(Term-dQuater-WebT/2);
		dELoadDis	= pData->m_ELoadDisArr[i];
		if(dApplyDis<dELoadDis && dELoadDis>0)
		{
			DArrELoadDis.Add(dELoadDis);			
			SArrELoadTitle.Add(pData->m_ELoadTitleArr[i]);
		}
	}
	AscendSort(DArrELoadDis, SArrELoadTitle);

	double TotalHLen = 0;
	for(i=0;i<pDB->GetQtyHDanNode();i++)
		TotalHLen += pDB->GetLengthHDanDom(pBx, i);

	BOOL	bBangEm		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;
	BOOL	bNanGan		= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan;
	BOOL	bWalkLoad	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad;
	BOOL	bAddLoad	=  SArrELoadTitle.GetSize()>0 ? TRUE : FALSE;

	double	dDimGap		= (bBangEm || bNanGan || bAddLoad) ? 170 : 0;
	long	nDimCol		= 0;

	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("TOP");

	if(bBangEm)
	{
		double	BangEmDis	= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;

		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(BangEmDis, nDimCol, COMMA(fabs(BangEmDis)));
		Dom.TextOut(Bx+BangEmDis, By+Height+dDimGap, "방음벽하중");

		nDimCol++;
	}
	if(bNanGan)
	{
		double	NanGanDis	= -pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;

		Dom.DimMoveTo(Bx, By+Height+dDimGap, nDimCol);
		Dom.DimLineTo(NanGanDis, nDimCol, COMMA(fabs(NanGanDis)));
		Dom.TextOut( Bx+NanGanDis, By+Height+dDimGap, "난간하중");


		nDimCol++;
	}

	double	Temp	= 0, dLenSum = 0;
	BOOL	bUsed	= FALSE;
	for(long k = DArrELoadDis.GetSize() - 1; k >= 0; k--)
	{
		if(pData->m_bExtraLoad)
		{
			double ELoadDis		= -(TotalHLen-DArrELoadDis[k]);
			CString LoadTitle	= SArrELoadTitle[k];
			double	dHtemp		= (-ELoadDis > LEN) ? Pavement : Height;

			Dom.DimMoveTo( Bx+Temp, By+Height+dDimGap, nDimCol);
			Dom.DimLineTo(ELoadDis-Temp, nDimCol, COMMA(fabs(ELoadDis-Temp)));
			if(k==0)
				Dom.DimLineToOver(-ELoadDis, nDimCol, "부가하중");
				
			Temp = ELoadDis;
			Dom.TextOut(Bx+ELoadDis, By+dHtemp+dDimGap, LoadTitle);

			bUsed	= TRUE;
			dLenSum	+= ELoadDis;
		}
	}
	if(bUsed)	nDimCol++;

	long	nLIdx		= 0;
	double	dDisLenIdx	= 0; 

	if(m_nGuardType != 99)
	{
		Dom.DimMoveTo( Bx, By+Height+dDimGap, nDimCol);

		if(D1!=0)
			Dom.DimLineToExtend( D1, nDimCol, COMMA(-D1), bExcel?sL[nLIdx++]:"");
		if(W5!=0)	
			Dom.DimLineToExtend( W5, nDimCol, COMMA(-W5), bExcel?sL[nLIdx++]:"");
		if(W4!=0)		
			Dom.DimLineToExtend( W4, nDimCol, COMMA(-W4), bExcel?sL[nLIdx++]:"");
		if(W3!=0)		
			Dom.DimLineToExtend( W3, nDimCol, COMMA(-W3), bExcel?sL[nLIdx++]:"");
		if(W2!=0)		
			Dom.DimLineToExtend( W2, nDimCol, COMMA(-W2), bExcel?sL[nLIdx++]:"");
		if(W1!=0)		
			Dom.DimLineToExtend( W1, nDimCol, COMMA(-W1), bExcel?sL[nLIdx++]:"");

		if(pDB->IsTUGir())
		{
			dDisLenIdx	= Term-LEN-dWidthBindConc/2;
			Dom.DimLineToExtend(-(dDisLenIdx+dMomentH), nDimCol, COMMA(dDisLenIdx+dMomentH), bExcel?sL[nLIdx++]:"");
		}
		else
		{
			dDisLenIdx	= Term-WebT/2-dQuater-LEN;
			Dom.DimLineToExtend(-dDisLenIdx, nDimCol, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
		}
	}
	else
	{
		dDisLenIdx	= pDB->IsTUGir() ? Term-LEN-dWidthBindConc/2 : Term-WebT/2-dQuater-LEN;
		if(dDisLenIdx!=0)
		{
			Dom.DimMoveTo(Bx, By+Pavement+170, 1);
			Dom.DimLineToExtend(-dDisLenIdx, 1, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("BOTTOM");
	if(!pDB->IsTUGir())
	{
		Dom.DimMoveTo( Bx, By-Tc, 0 );

		dDisLenIdx	= Term-WebT/2-dQuater*2-Dis-dHunch;
		if(dDisLenIdx!=0)
			Dom.DimLineToExtend(-dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");

		dDisLenIdx	= dHunch;
		if(dDisLenIdx!=0)	
			Dom.DimLineToExtend(-dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");

		dDisLenIdx	= dQuater+Dis;
		if(dDisLenIdx!=0)
			Dom.DimLineToExtend(-dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
		
		dDisLenIdx	= dQuater;
		if(dDisLenIdx!=0)
			Dom.DimLineToExtend(-dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
	}
	else
	{
		Dom.DimMoveTo( Bx-Term+dWidthBindConc/2, By-Tc, 0 );
		
		dDisLenIdx	= Term-dWidthBindConc/2;
		if(dDisLenIdx!=0)
		{
			Dom.DimLineToExtend(dDisLenIdx, 0, COMMA(dDisLenIdx), bExcel?sL[nLIdx++]:"");
			Dom.DimMoveTo( Bx-Term+dWidthBindConc/2, By-Tc, 0 );
		}
		Dom.DimLineToExtend(-dWidthBindConc/4, 0, COMMA(dWidthBindConc/4), bExcel?sL[nLIdx++]:"");
	}

	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("RIGHT");
	long	nHIdx	= 0;

	Dom.DimMoveTo(Bx, By-Ta, 0);

	double dDisHIdx	= Ta;
	if(dDisHIdx!=0)
		Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");

	if(m_nGuardType != 99)
	{
		dDisHIdx	= H1;
		if(dDisHIdx!=0)
			Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
		dDisHIdx	= H2;
		if(dDisHIdx!=0)
			Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
		dDisHIdx	= H3;
		if(dDisHIdx!=0)
			Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
	}

	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(Bx-Term, By-Tc, 0);

	dDisHIdx	= Tc;
	if(dDisHIdx!=0)
		Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
	dDisHIdx	= bWalkLoad ? pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH : Pavement;
	if(dDisHIdx!=0)
		Dom.DimLineToExtend(dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");
	
	if(dDeep>0)
	{
		Dom.DimMoveTo(Bx-Term, By-Tc, 1);
		Dom.DimLineTo(dDeep, 1, COMMA(dDeep));
		Dom.DimLineToExtend(Tc-dDeep, 1, COMMA(Tc-dDeep), bExcel?sH[nHIdx++]:"");
	}

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(Bx, By-Ta, 0);

	dDisHIdx	= Tc-dThickSlabRightExt;
	if(dDisHIdx>0)
		Dom.DimLineToExtend(-dDisHIdx, 0, COMMA(dDisHIdx), bExcel?sH[nHIdx++]:"");


	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("TOP");

	if(!bWalkLoad)
	{
		double	dDis_WtoW	= 1.8;
		if(pData->m_bTank && !pData->m_bTrailer)
			dDis_WtoW -= (dDis_Tank-dDis_Car);
		else if(!pData->m_bTank && pData->m_bTrailer)
			dDis_WtoW -= (dDis_Trailer-dDis_Car);
		else if(pData->m_bTank && pData->m_bTrailer)
		{
			double	dDisTemp = (dDis_Tank>=dDis_Trailer) ? dDis_Tank : dDis_Trailer;
			dDis_WtoW -= (dDisTemp-dDis_Car);
		}

		double	DBLoadDis	= -frM(dDis_Car);
		double	DBLoadDis_2nd	= -frM(dDis_Car+1.8);
		BOOL	bDBLoad_2nd	= ABS(DBLoadDis_2nd) < ABS(Term-Dis-dQuater-LEN);
		
		Dom.DimMoveTo(Bx-LEN, By+Pavement+170, 0);
		Dom.DimLineTo(DBLoadDis, 0, COMMA(ABS(DBLoadDis))); 
		Dom.TextOut(Bx-LEN+DBLoadDis, By+Pavement+170, bDBLoad_2nd?"Pr1":"Pr");

		double	dDimDis = Bx-LEN+DBLoadDis;
		DBLoadDis = -(dDis_Trailer-dDis_Car)*1000;
		if(pData->m_bTank)
		{			
			DBLoadDis		= -(dDis_Tank-dDis_Car)*1000;

			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(ABS(DBLoadDis))); 
			Dom.TextOut(dDimDis+DBLoadDis, By+Pavement+170, "Tk");

			dDimDis = dDimDis+DBLoadDis;
			DBLoadDis = -(dDis_Trailer-dDis_Tank)*1000; 
		}
		if(pData->m_bTrailer)
		{			
			Dom.DimMoveTo(dDimDis, By+Pavement+170, 0);
			Dom.DimLineTo(DBLoadDis, 0, COMMA(ABS(DBLoadDis))); 
			Dom.TextOut(DBLoadDis+dDimDis, By+Pavement+170, "Tr");
		}

		if(bDBLoad_2nd)
		{
			Dom.DimLineTo(-frM(dDis_WtoW), 0, COMMA(frM(dDis_WtoW))); 
			Dom.TextOut(Bx-LEN+DBLoadDis_2nd, By+Pavement+170, "Pr2");
		}
	}
	
	if(pSep && bWalkLoad)
	{
		CString bodo1,bodo2;
		double	dBodoLen		= 0;
		double	dT1				= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2;
		double	dT2				= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double	dT2_			= 0;
		double	dBodoH			= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
		double	dBodoHDanLen	= pDB->GetLengthHDanDom(pBx, Size-2);
		if(LEN+dBodoHDanLen > Term+dQuater)
		{
			dBodoLen	= Term+dQuater-LEN;
			dT2_		= dT2 + fabs(dT2-dT1)*dBodoLen/dBodoHDanLen;
		}
		else
		{
			dBodoLen	= dBodoHDanLen;
			dT2_		= dT2;
		}

		double	dHeightStt	= dT1!=0 ? dBodoH : 0;
		double	dHeightEnd	= dT1!=0 ? dBodoH + (dT2/dT1-1)*dBodoH*dBodoLen/dBodoHDanLen : (dT2!=0 ? dBodoH : 0);

		bodo1.Format("%.3f kN/m2", tokNPM2(dT1));
		bodo2.Format("%.3f kN/m2", tokNPM2(dT2_));

		Dom.GiSiArrowExtend(Bx-LEN, By+dHeightStt, Dom.Always(10), -Dom.Always(20), 3, _T("보도하중 T2"), bodo1);	
		Dom.GiSiArrowExtend(Bx-LEN-dBodoLen, By+dHeightEnd, Dom.Always(10), -Dom.Always(30), 3, _T("보도하중 T1"), bodo2);			
	}			

	Dom.CutLightning(Bx-Term, By+200, Bx-Term, By-Tc-200, TRUE, 0.75);
	if(pDB->IsTUGir())
		Dom.LineTo(Bx-Term+dWidthBindConc/2, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH:By, Bx-Term+dWidthBindConc/2, By-Tc);
	else
		Dom.LineTo(Bx-Term+WebT/2+dQuater, bWalkLoad?By+pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH:By+Pavement, Bx-Term+WebT/2+dQuater, By-Tc);

	//////////////////////////////////////////////////////////////////////////
	// Divide Area
	CString sNum[20];
	for(n=0; n<20; n++)
		sNum[n].Format("%d", n+1);

	long nA	= 0;
	Bx += D1;
	if(m_nGuardType == 1)
	{
		pDom->LineTo(Bx,		By+H1+H2,	Bx+W3+W2,		By+H1+H2);
		pDom->LineTo(Bx+W3,		By+H1+H2,	Bx+W3,			By+H1+H2+H3);
		pDom->LineTo(Bx+W3+W2,	By,			Bx+W3+W2,		By+H1+H2);
		pDom->LineTo(Bx+W3+W2,	By+H1,		Bx+W3+W2+W1,	By+H1);

		pDom->TextCircle(Bx+W3/2,		By+H1+H2+H3/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W2/3,	By+H1+H2+H3/3,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+(W3+W2)/2,	By+(H1+H2)/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W2+W1/3,	By+H1+H2/3,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W2+W1/2,	By+H1/2,		sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 2)
	{
		pDom->LineTo(Bx+W3,By+H1+H2+H3,Bx+W3,By+H1+H2);
		pDom->LineTo(Bx,By+H1+H2,Bx+W3+W2,By+H1+H2);
		pDom->TextCircle(Bx+W3/2,By+H1+H2+H3/2,sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W2/2,By+H1+H2+H3/3,sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+(W3+W2)/2,By+(H1+H2)/2,sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 3)
	{
		pDom->LineTo(Bx+W3,By+H1+H2,Bx+W3,By+H1);
		pDom->LineTo(Bx+W3+W1,By+H1,Bx,By+H1);
		pDom->TextCircle(Bx+W3/2,By+H1+H2+H3/2,sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W1/3,By+H1+H2/3,sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+(W3+W1)/2,By+H1/2,sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 4)
	{
		pDom->TextCircle(Bx+Width/2,	By+Height/2,	sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 5)
	{
		pDom->LineTo(Bx+W1,			By+H1+H2,	Bx+W5+W4+W3+W2,		By+H1+H2);
		pDom->LineTo(Bx+W5+W4,		By+H1+H2,	Bx+W5+W4,			By+H1+H2+H3);
		pDom->LineTo(Bx+W5+W4+W3,	By+H1+H2,	Bx+W5+W4+W3,		By+H1+H2+H3);
		pDom->LineTo(Bx+W5,			By+H1,		Bx+W5,				By+H1+H2);
		pDom->LineTo(Bx+W5+W4+W3+W2,By+H1,		Bx+W5+W4+W3+W2,		By+H1+H2);
		pDom->LineTo(Bx,			By+H1,		Bx+W5+W4+W3+W2+W1,	By+H1);
		
		//////////////////////////////////////////////////////////////////////////
		
		pDom->TextCircle(Bx+W5+W4+W3/2,			By+H1+H2+H3/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W5+W4*2/3,			By+H1+H2+H3/3,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W5+W4+W3+W2/3,		By+H1+H2+H3/3,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W5+W4+W3/2,			By+H1+H2/2,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W5*2/3,				By+H1+H2/3,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W5+W4+W3+W2+W1/3,	By+H1+H2/3,		sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W1+W2+W3/2,			By+H1/2,		sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 7)
	{
		pDom->LineTo(Bx+W3,By+H2,Bx+W3,By);
		pDom->TextCircle(Bx+W3/2,By+H2/2,sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W1/3,By+H2/3,sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 8)
	{
		pDom->LineTo(Bx+W3, By, Bx+W3, By+H1);

		pDom->TextCircle(Bx+W3/2,		By+(H1+H3)/2,	sNum[nA++],	-1,	FALSE,	TRUE);
		pDom->TextCircle(Bx+W3+W1/2,	By+H1/2,		sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else if(m_nGuardType == 100)
	{
		pDom->TextCircle(Bx+Term/2,By+H1/2,"이 방호벽 모양은 지원할 수 없습니다.");
	}

	Bx -= D1;
	double	dDisTemp1=0, dDisTemp2=0, dDisTemp3=0;

	if(!pDB->IsTUGir())
	{
		if(pDB->m_bUserSlabHunch)
		{
			dDisTemp1	= Term - WebT/2 - 2*dQuater - Dis - dHunch;
			dDisTemp2	= Term - WebT/2 - 2*dQuater - Dis;
			dDisTemp3	= Term - WebT/2 - dQuater;

			// Line
			pDom->LineTo(Bx, By-Ta, Bx-dDisTemp3, By-Ta);
			/*if(Tc != dThickSlabRightExt)
			{
				pDom->LineTo(Bx-dDisTemp2, By-Ta, Bx-dDisTemp2, By-Tc);
				pDom->LineTo(Bx-dDisTemp1, By-dThickSlabRightExt, Bx-dDisTemp2, By-dThickSlabRightExt);
				pDom->LineTo(Bx-dDisTemp1, By-Ta, Bx-dDisTemp1, By-dThickSlabRightExt);
			}
			*/
			//else
			//{
				pDom->LineTo(Bx-dDisTemp1, By-Ta, Bx-dDisTemp1, By-Tc);
			//}

			// Numbering
			pDom->TextCircle(Bx-dDisTemp3/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			/*if(Tc != dThickSlabRightExt)
			{
				pDom->TextCircle(Bx-dDisTemp1-(dDisTemp2-dDisTemp1)*2/3, By-dThickSlabRightExt-(Tc-dThickSlabRightExt)/3, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx-dDisTemp2-(dDisTemp3-dDisTemp2)/2, By-Ta-(Tc-Ta)/2, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx-dDisTemp1*2/3, By-Ta-(Tc-dThickSlabRightExt)/3, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx-dDisTemp1-(dDisTemp2-dDisTemp1)/2, By-Ta-(Tc-dThickSlabRightExt)/2, sNum[nA++],	-1,	FALSE,	TRUE);
			}
			*/
			//else
			//{
				pDom->TextCircle(Bx-dDisTemp1*2/3, By-Ta-(Tc-Ta)/3, sNum[nA++],	-1,	FALSE,	TRUE);
				pDom->TextCircle(Bx-dDisTemp1-(dDisTemp3-dDisTemp1)/2, By-Ta-(Tc-Ta)/2, sNum[nA++],	-1,	FALSE,	TRUE);
			//}
		}
		else
		{
			dDisTemp1	= Term - WebT/2 - 2*dQuater - Dis;
			dDisTemp2	= Term - WebT/2 - dQuater;

			// Line
			pDom->LineTo(Bx, By-Ta, Bx-dDisTemp2, By-Ta);
			pDom->LineTo(Bx-dDisTemp1, By-Ta, Bx-dDisTemp1, By-Tc);

			// Numbering
			pDom->TextCircle(Bx-dDisTemp2/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx-dDisTemp1*2/3, By-Ta-(Tc-Ta)/3, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx-dDisTemp1-(dDisTemp2-dDisTemp1)/2, By-Ta-(Tc-Ta)/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}
	
		dDisTemp1	= Term - LEN - WebT/2 - dQuater;
		if(bWalkLoad)
		{
			double dWalkLoadH	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
			pDom->TextCircle(Bx-LEN-dDisTemp1/2, By+dWalkLoadH/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}
		else
			pDom->TextCircle(Bx-LEN-dDisTemp1/2, By+Pavement/2, sNum[nA++],	-1,	FALSE,	TRUE);
	}
	else
	{
		if(Tc==Ta)
		{
			dDisTemp1	= Term - dWidthBindConc/2;
			pDom->TextCircle(Bx-dDisTemp1/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->LineTo(Bx-dDisTemp1-dMomentH,By,Bx-dDisTemp1-dMomentH,By-dMomentV);
			pDom->TextCircle(Bx-dDisTemp1-dMomentH/2,By-dMomentV/2,sNum[nA++],-1,FALSE,TRUE);
			dDisTemp1	= Term - LEN - dWidthBindConc/2;
			if(bWalkLoad)
			{
				double dWalkLoadH	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
				pDom->TextCircle(Bx-LEN-dDisTemp1/2, By+dWalkLoadH/2, sNum[nA++],	-1,	FALSE,	TRUE);
			}
			else
				pDom->TextCircle(Bx-LEN-dDisTemp1/2, By+Pavement/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}
		else
		{
			dDisTemp1	= Term - dWidthBindConc/2;
			// Line
			pDom->LineTo(Bx, By-Ta, Bx-dDisTemp1, By-Ta);
			double dLTScale = pDom->GetLTScale();
			pDom->SetLTScale(Dom.Always(5));
			pStd->SetEnvLType(pDom, LT_CENTER_LINE);
			pDom->LineTo(Bx-dDisTemp1-dMomentH,By+Pavement+Dom.Always(5),Bx-dDisTemp1-dMomentH,By-dMomentV-dHeightBindConc-300);
			pStd->SetEnvLType(pDom, LT_SOLID);
			pDom->SetLTScale(dLTScale);
			
			// Numbering
			pDom->TextCircle(Bx-dDisTemp1/2, By-Ta/2, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx-dDisTemp1*2/3, By-Ta-(Tc-Ta)/3, sNum[nA++],	-1,	FALSE,	TRUE);
			pDom->TextCircle(Bx-dDisTemp1-dMomentH/2,By-dMomentV/2,sNum[nA++],-1,FALSE,TRUE);

			dDisTemp1	= Term - LEN - dWidthBindConc/2;
			if(bWalkLoad)
			{
				double dWalkLoadH	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH;
				pDom->TextCircle(Bx-LEN-dDisTemp1/2, By+dWalkLoadH/2, sNum[nA++],	-1,	FALSE,	TRUE);
			}
			else
				pDom->TextCircle(Bx-LEN-dDisTemp1/2, By+Pavement/2, sNum[nA++],	-1,	FALSE,	TRUE);
		}
	}

	*pDom << Dom;	
}

void CAPlateDrawCalc::AscendSort(CDoubleArray &DArrWheight, CStringArray &DArrHeight)
{
	long Size = DArrWheight.GetSize();
	double Ascend[100];
	CString sAscend[100];
	long i = 0;
	for(i = 0; i < Size; i++)
	{
		Ascend[i] = DArrWheight.GetAt(i);
		sAscend[i] = DArrHeight[i];
	}
	double Min = Ascend[0];
	double temp = 0;
	CString stemp;
	CString sMin = sAscend[0];
	for(i = 1; i < Size; i++)
	{
		for(long j = i; j < Size; j++)
		{
			if(Min > Ascend[j])
			{
				temp = Ascend[j];
				Ascend[j] = Min;
				Min = temp;

				stemp = sAscend[j];
				sAscend[j] = sMin;
				sMin = stemp;
			}
			else continue;
		}
		Ascend[i-1] = Min;
		Min = Ascend[i];

		sAscend[i-1] = sMin;
		sMin = sAscend[i];
	}
	DArrWheight.RemoveAll();
	for(i = 0; i < Size; i++)
	{
		temp = Ascend[i];
		DArrWheight.Add(temp);
		DArrHeight[i] = sAscend[i];
	}
}

void CAPlateDrawCalc::SetDBLoad(double dDBLoadCar, double dDBLoadTank, double dDBLoadTrailer)
{
	m_dDBLoadCar		= dDBLoadCar;
	m_dDBLoadTank		= dDBLoadTank;
	m_dDBLoadTrailer	= dDBLoadTrailer;
}

void CAPlateDrawCalc::DrawLoadArrow(CDomyun *pDom, CDPoint xyUp, CDPoint xyDn)
{
	CDomyun Dom(pDom);

	Dom.Solid(xyUp.x-5, xyUp.y, xyUp.x+5, xyUp.y, xyDn.x+5, xyDn.y, xyDn.x-5, xyDn.y);
	Dom.Solid(xyDn.x-20, xyDn.y+10, xyDn.x, xyDn.y, xyDn.x+20, xyDn.y+10, xyDn.x, xyDn.y-40);

	*pDom << Dom;
}