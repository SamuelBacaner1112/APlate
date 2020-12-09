// APlateDrawDimPyung.cpp: implementation of the CAPlateDrawDimPyung class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDBDraw.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const long MaxGIRDER =  20;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateDrawDimPyung::CAPlateDrawDimPyung(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawDimPyung::~CAPlateDrawDimPyung()
{

}

// nType==0 : 상하면 nType==1 : 상면 nType==2 : 하면
void CAPlateDrawDimPyung::GetDimHdanPoint(CPlateBasicIndex *pBx, double dx[], long nType)
{
	if(pBx==NULL)	return;
 	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	
	// 횡단면도 포인트 구하기
	long GirderSu = pBridge->GetGirdersu();
	long cnt=0;
	long n = 0;
	for(n=0; n<MaxGIRDER*3; n++)
		dx[n] = 0;

	// 외측가로보,세로보
	// 구간별 거리 저장
	double dLeftSlabCen  = pBridge->GetSlab(CPlateBridgeApp::SLAB_LEFT)->m_dGirderCenter;
	double dRightSlabCen = pBridge->GetSlab(CPlateBridgeApp::SLAB_RIGHT)->m_dGirderCenter;

	if(nType==0)
	{	
		pGir = pBridge->GetGirder(0); 
		dx[cnt++] = pGir->m_dGirderCenter - dLeftSlabCen - pGir->GetWidthFlangeUpper(pBx)/2;
		for(n=0; n<GirderSu; n++) 
		{
			pGir = pBridge->GetGirder(n);
			if(n<GirderSu/2)
				dx[cnt++] = pGir->GetWidthFlangeUpper(pBx);
			else if(n>GirderSu/2)
				dx[cnt++] = pGir->GetWidthFlangeLower(pBx);
			else
			{
				if(GirderSu%2==0)
					dx[cnt++] = pGir->GetWidthFlangeLower(pBx);
				else
					dx[cnt++] = (pGir->GetWidthFlangeUpper(pBx) + pGir->GetWidthFlangeLower(pBx))/2;
			}

			if(n < GirderSu-1)
			{
				if(n<GirderSu/2)
				{
					if(n==GirderSu/2-1 && GirderSu%2==0)
						dx[cnt++] = fabs(pGir->m_pRight->m_dGirderCenter - pGir->m_dGirderCenter - pGir->m_pRight->GetWidthFlangeUpper(pBx)/2 - pGir->GetWidthFlangeLower(pBx)/2);
					else
						dx[cnt++] = fabs(pGir->m_pRight->m_dGirderCenter - pGir->m_dGirderCenter - pGir->m_pRight->GetWidthFlangeUpper(pBx)/2 - pGir->GetWidthFlangeUpper(pBx)/2);
				}
				else if(n>GirderSu/2)
					dx[cnt++] = fabs(pGir->m_pRight->m_dGirderCenter - pGir->m_dGirderCenter - pGir->m_pRight->GetWidthFlangeLower(pBx)/2 - pGir->GetWidthFlangeLower(pBx)/2);
				else				
					dx[cnt++] = fabs(pGir->m_pRight->m_dGirderCenter - pGir->m_dGirderCenter - pGir->m_pRight->GetWidthFlangeLower(pBx)/2 - pGir->GetWidthFlangeLower(pBx)/2);
			}
		}
		dx[cnt++] = dRightSlabCen - pGir->m_dGirderCenter - pGir->GetWidthFlangeLower(pBx)/2;
	}
	if(nType==1)
	{	
		pGir = pBridge->GetGirder(0); 
		dx[cnt++] = pGir->m_dGirderCenter - dLeftSlabCen - pGir->GetWidthFlangeUpper(pBx)/2;
		for(n=0; n<GirderSu; n++) 
		{
			pGir = pBridge->GetGirder(n);
			dx[cnt++] = pGir->GetWidthFlangeUpper(pBx);
			if(n < GirderSu-1)
				dx[cnt++] = fabs(pGir->m_pRight->m_dGirderCenter - pGir->m_dGirderCenter - pGir->m_pRight->GetWidthFlangeUpper(pBx)/2 - pGir->GetWidthFlangeUpper(pBx)/2);
		}
		dx[cnt++] = dRightSlabCen - pGir->m_dGirderCenter - pGir->GetWidthFlangeUpper(pBx)/2;
	}
	else if(nType==2)
	{	
		pGir = pBridge->GetGirder(0); 
		dx[cnt++] = pGir->m_dGirderCenter - dLeftSlabCen - pGir->GetWidthFlangeLower(pBx)/2;
		for(n=0; n<GirderSu; n++) 
		{
			pGir = pBridge->GetGirder(n);
			dx[cnt++] = pGir->GetWidthFlangeLower(pBx);
			if(n < GirderSu-1)
				dx[cnt++] = fabs(pGir->m_pRight->m_dGirderCenter - pGir->m_dGirderCenter - pGir->m_pRight->GetWidthFlangeLower(pBx)/2 - pGir->GetWidthFlangeLower(pBx)/2);
		}
		dx[cnt++] = dRightSlabCen - pGir->m_dGirderCenter - pGir->GetWidthFlangeLower(pBx)/2;
	}
}

void CAPlateDrawDimPyung::DrawDimHDan(CDomyun *pDom,  CPlateBasicIndex *pBx, BOOL bLeft, BOOL bSeperate, long nType, BOOL bDimGirType)
{	
	if(pBx==NULL)	return;
 	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CSlabApp *pSlabL = pBridge->GetSlab(CPlateBridgeApp::SLAB_LEFT);
	CSlabApp *pSlabR = pBridge->GetSlab(CPlateBridgeApp::SLAB_RIGHT);
	CPlateGirderApp	*pGir    = (CPlateGirderApp	*)pBx->GetGirder();
	CLineInfo		*pLine	 = NULL;
	
	long nG = pBx->GetNumberGirder();

	// 횡단면도 포인트 구하기
	double dx[MaxGIRDER*3];
	GetDimHdanPoint(pBx,dx,nType); //좌~우

	CDimDomyunEx Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	// 기준점 구하기
	// pBx->GetAngleSection()은 지점위치가 아닌 곳에서 정상작동.
	double UD = 1.0;	// 치수기입기준이 0번거더(-1 : 최우측거더)
	double dStaCenter = pBridge->GetGirder(-1)->GetStationBySc(pBx->GetStation(), 0, CDPoint(0,0));
	double dStaL      = pBridge->GetGirder(-1)->GetStationBySc(dStaCenter, pSlabL->m_dGirderCenter, pBx->GetAngle());	
	double dStaR      = pBridge->GetGirder(-1)->GetStationBySc(dStaCenter, pSlabR->m_dGirderCenter, pBx->GetAngle());	
	CDPoint dp = pSlabL->GetXyGirderDis(bLeft ? min(dStaL, dStaR) : max(dStaL, dStaR), 0 ,pBx->GetAngle());	
	
	if(nG==pBridge->GetGirdersu()-1)	
		UD=-1.0;
	pGir  = pBridge->GetGirder(nG);
	pLine = pGir->GetLine();

	bLeft ? Dom.SetDirection("LEFT") : Dom.SetDirection("RIGHT");

	double endX = 0;
	long DimDan = bLeft ? 2 : 2;
	// 1단치수 : 거더치수
	Dom.DimMoveTo(dp.x,dp.y,DimDan);
	long nCount=1;

	if(bDimGirType)
	{
		for(long n=0; n<MaxGIRDER*3; n++)
		{
			if(dx[n]==0)	
				break;
			if(!bSeperate)
			{
				if(dx[n] == dx[n+1]) ++nCount;
				else
				{
					if(nCount > 1)
					{
						CString szDim;
						szDim.Format("%d@%s=%s",nCount, COMMA(dx[n]), COMMA(dx[n]*nCount));
						Dom.DimLineTo(dp.x,-dx[n]*UD*nCount,szDim);
					}
					else
						Dom.DimLineTo(dp.x,-dx[n]*UD,COMMA(dx[n]));

					nCount = 1;
				}
			}
			else
			{
				if(n%2==0)	Dom.DimLineTo(dp.x,-dx[n]*UD,COMMA(dx[n]));
				else		Dom.DimLineTo(dp.x,-dx[n]*UD,"",COMMA(dx[n]));
			}
			endX += dx[n];
		}
	}
	else
	{
		CPlateBasicIndex *pBxMatch = NULL;
		double	Len	= pBridge->GetWidthSlabLeft();
		Dom.DimLineTo(dp.x,-Len,COMMA(Len));

		for(long nG=0; nG < pBridge->GetGirdersu()-1; nG++) 
		{
			pBxMatch = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(nG));
			Len      = pBridge->GetGirder(nG)->GetDisGirderToGirder(pBxMatch);
			
			Dom.DimLineTo(dp.x,-Len,COMMA(Len));
		}

		for(long n=0; n<MaxGIRDER*3; n++)
		{
			if(dx[n]==0)	break;
			endX += dx[n];
		}

		Len	= pBridge->GetWidthSlabRight();
		Dom.DimLineTo(dp.x,-Len,COMMA(Len));
	}
	
	// 2단치수 : 도로중심,거더전폭
	DimDan++;
	Dom.DimMoveTo(dp.x,dp.y,DimDan);
	Dom.DimLineTo(dp.x,-endX*UD,COMMA(endX));

	// 0번거더 기준
	Dom.Rotate(pLine->GetAngleAzimuth(pBx->GetStation()).GetAngleRadian(),dp.x,dp.y);

	*pDom << Dom;
}

////////////////////////////////////////////////////////////////////////////////////////////
long CAPlateDrawDimPyung::DimPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,
								   __int64 nType, BOOL bUpper, BOOL bStr,  CString csTitle, 
								   long nSttDan, BOOL bSpanArrow)
{	
	if(nType & DIM_BASE_VSTIFF)
	{
		CDPointArray DataArray;
		GetDimArrayVStiff(pBxStt,pBxEnd,DataArray,nType);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "수직보강재";
		DimBasePlanEx(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++);
	}
	
	if(nType & DIM_BASE_DIAP)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_DIAP,bUpper);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "가로보";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,FALSE);
	}
	
	if(nType & DIM_BASE_SPLICE)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_SPLICE,bUpper);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "현장이음";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,FALSE);
	}	
	if(nType & DIM_BASE_FACTUPPER)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_FACTUPPER,bUpper);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "상판";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,FALSE);
	}	
	if(nType & DIM_BASE_FACTLOWER)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_FACTLOWER,bUpper);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "하판";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,FALSE);
	}	
	if(nType & DIM_BASE_FACTWEB)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_FACTWEB,bUpper);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "복부판";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,FALSE);
	}
	
	if(nType & DIM_BASE_TENDON)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_TENDON,bUpper);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "TENDON";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,FALSE);
	}
	
	if(nType & DIM_BASE_SPAN)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_SPAN,bUpper,bSpanArrow);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "경간길이";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,bSpanArrow);
	}	
	if(nType & DIM_BASE_TOTAL)
	{
		CDoubleArray DataArray;
		GetDimArray(pBxStt,pBxEnd,DataArray,DIM_BASE_TOTAL,bUpper);
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0)
			sTitle = "총길이";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bUpper,nSttDan++,FALSE);
	}
	
	return nSttDan;
}

void CAPlateDrawDimPyung::GetDimArray(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
								      CDoubleArray &DimDataArray, __int64 nType, BOOL bUpper, BOOL bNextInclude)
{	
	if(nType & DIM_BASE_DIAP)
	{			
		GetDimArrayCrossBeam(pBxStt,pBxEnd,DimDataArray,nType);
		return;
	}
	if(nType & DIM_BASE_SPLICE)
	{			
		GetDimArraySplice(pBxStt,pBxEnd,DimDataArray,nType);
		return;
	}	
	if(nType & DIM_BASE_FACTUPPER)
	{			
		GetDimArrayFactJoin(pBxStt,pBxEnd,DimDataArray, 0,nType,FALSE);
		return;
	}	
	if(nType & DIM_BASE_FACTLOWER)
	{			
		GetDimArrayFactJoin(pBxStt,pBxEnd,DimDataArray, 1,nType,FALSE);
		return;
	}	
	if(nType & DIM_BASE_FACTWEB)
	{			
		GetDimArrayFactJoin(pBxStt,pBxEnd,DimDataArray, 2,nType,FALSE);
		return;
	}	
	if(nType & DIM_BASE_TENDON)
	{			
		GetDimArrayTendon(pBxStt,pBxEnd,DimDataArray,nType,bNextInclude);
		return;
	}	
	if(nType & DIM_BASE_SPAN)
	{			
		GetDimArraySpan(pBxStt,pBxEnd,DimDataArray,nType,bNextInclude);
		return;
	}
	if(nType & DIM_BASE_TOTAL)
	{			
		GetDimArrayTotal(pBxStt,pBxEnd,DimDataArray,nType);
		return;
	}
}

// bUpper = TRUE -> 0번거더에 치수표기(평면상위)
// nUsingGirder : 김성곤만 사용할것
// nUsingGirder = -1 이면 슬래브를 기준으로 하부 치수선을 그림(종전방식:default)
// nUsingGirder = 0 이면 시작거더를 기준으로 하부 치수선을 그림
// nUsingGirder = 1 이면 마지막거더를 기준으로 하부 치수선을 그림
void CAPlateDrawDimPyung::DimBasePlan(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
										 CDoubleArray &DimDataArray, CString Title, 
										 BOOL bUpper, long nDan, BOOL bSpanArrow,  long nUsingGirder)
{
	if(DimDataArray.GetSize()==0)	return;

 	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBxStt->GetGirder();

	CDimDomyunEx	*pDom = new CDimDomyunEx(pBridge,pDomP);

	long   nG	= pBxStt->GetNumberGirder();
	double yStd = 0;
	double sta1 = DimDataArray.GetAt(0);
	double sta2 = 0;
	double nLen = pDom->GetTextWidth("TOTAL LENGTH",8);
	
	if(bUpper)
	{
		CPlateGirderApp *pGirStd = pBridge->GetGirder(0);
		yStd = pGirStd->m_dGirderCenter;
//		if(pBridge->GetLBracketsu())
//		{
//			CBrBeam *pBeam = pBridge->GetBrBeam(0,TRUE);
//			double dDistBrkCen = pBeam->GetDistFromGirderCenter(TRUE);
//			yStd = pGirStd->m_dGirderCenter + dDistBrkCen - pBeam->m_uM_WB;
//		}
		double Dist = yStd - (nDan+1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		pDom->SetDirection("BOTTOM");
		pDom->DimBaseCurvePoint(sta1, Dist);
	}
	else
	{
		CPlateGirderApp *pGirStd = pBridge->GetGirder(pBridge->GetGirdersu()-1);
		yStd = pBridge->GetDisSlabRightDom(pGirStd->GetBxByStation(pGirStd->GetStationStt(), BX_CROSSBEAM_VBRACING));
		if(nUsingGirder==0)
			yStd -= fabs(pBridge->GetSlabRight()->m_dGirderCenter-pBridge->GetGirder(1)->m_dGirderCenter);
		else if(nUsingGirder==1)
			yStd -= fabs(pBridge->GetSlabRight()->m_dGirderCenter-pBridge->GetGirder(pBridge->GetGirdersu()-1)->m_dGirderCenter);

//		if(pBridge->GetRBracketsu())
//		{
//			CBrBeam *pBeam = pBridge->GetBrBeam(0,FALSE);
//			double dDistBrkCen = pBeam->GetDistFromGirderCenter(FALSE);
//			yStd = pGirStd->m_dGirderCenter + dDistBrkCen + pBeam->m_uM_WB;;
//		}
		double Dist = yStd + (nDan+1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		pDom->SetDirection("TOP");
		pDom->DimBaseCurvePoint(sta1, Dist);
	}

	CString str = _T("");

	// 처음치수
	if(bSpanArrow && pBxStt->GetStation()>sta1)
	{
		pDom->DimCurveMoveTo(pBxStt->GetStation(), yStd, nG, FALSE, Title, nLen,TRUE,TRUE);
		long nJijum = pGir->GetNumberJijumByBx(pBxStt) - 1;
		if(nJijum<0) nJijum = 0;
		CPlateBasicIndex *pBxCen = pBridge->GetGirder(-1)->GetBxOnJijum(nJijum);
		str = "";//pBridge->GetMarkSection(SECTION_MAIN,pBxCen) + " = " + COMMA(pBridge->m_dLengthJigan[nJijum]);
	}
	else
		pDom->DimCurveMoveTo(pBxStt->GetStation(), yStd, nG, FALSE, Title, nLen);

	sta1 = pBxStt->GetStation();

	long nSize = DimDataArray.GetSize();
	long i = 0;
	for(i=1; i<nSize-1; i++) 
	{	
		sta2 = DimDataArray.GetAt(i);
		if(str.GetLength()==0) str = COMMA(sta2-sta1);

		if(sta2-sta1 < 300)
			pDom->DimCurveLineTo(sta2-sta1, yStd, "", str, nG);
		else
			pDom->DimCurveLineTo(sta2-sta1, yStd, str ,"", nG);
		sta1 = sta2;
		str.Empty();
	}
	
	// 마지막치수
	sta2 = DimDataArray.GetAt(i);
	if(bSpanArrow && pBxEnd->GetStation()<sta2)
	{
		long nJijum = pGir->GetNumberJijumByBx(pBxEnd) + 1;
		if(nJijum>pBridge->m_nQtyJigan) nJijum = pBridge->m_nQtyJigan; // ERROR CHECK!!
		//CPlateBasicIndex *pBxCen = pBridge->GetGirder(-1)->GetBxOnJijum(nJijum);
		str = "";//pBridge->GetMarkSection(SECTION_MAIN,pBxCen) + " = " + COMMA(pBridge->m_dLengthJigan[nJijum-1]);
		if(sta2-sta1 < 300)
			pDom->DimCurveLineTo(pBxEnd->GetStation()-sta1, yStd, "", str, nG, FALSE,FALSE,FALSE,0,bSpanArrow);
		else
			pDom->DimCurveLineTo(pBxEnd->GetStation()-sta1, yStd, str ,"",nG,FALSE,FALSE,FALSE,0,bSpanArrow);
	}
	else
	{
		sta2 = pBxEnd->GetStation();
		if(sta2-sta1 < 300)
			pDom->DimCurveLineTo(sta2-sta1, yStd, "", COMMA(sta2-sta1), nG);
		else
			pDom->DimCurveLineTo(sta2-sta1, yStd, COMMA(sta2-sta1) ,"",nG);
	}
	
	*pDomP << *pDom;
	delete pDom;
}


// x값은 스테이션이 아니고 거리값....
// @표시를 포함하는 치수 : 수직보강재, 다이아프램(?)
void CAPlateDrawDimPyung::DimBasePlanEx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
								   CDPointArray &DimDataArray, CString Title, 
								   BOOL bUpper, long nDan)
{
	if(DimDataArray.GetSize()==0)	return;

  	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	
	CDimDomyunEx	*pDom = new CDimDomyunEx(pBridge,pDomP);	

	long   nG	= pBxStt->GetNumberGirder();
	double yStd = 0;
	double sta1 = pBxStt->GetStation();
	double sta2 = pBxStt->GetStation();
	double dist = 0;
	double nLen = pDom->GetTextWidth("TOTAL LENGTH",8);
	CDPoint dp;


	if(bUpper)
	{
		CPlateGirderApp *pGirStd = pBridge->GetGirder(0);
		yStd = pGirStd->m_dGirderCenter;
		double Dist = yStd - (nDan+1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		pDom->SetDirection("BOTTOM");
		pDom->DimBaseCurvePoint(sta1, Dist);
	}
	else
	{
		CPlateGirderApp *pGirStd = pBridge->GetGirder(pBridge->GetGirdersu()-1);
		yStd = pGirStd->m_dGirderCenter;
		double Dist = yStd + (nDan+1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		pDom->SetDirection("TOP");
		pDom->DimBaseCurvePoint(sta1, Dist);
	}

	// 처음치수
	if(pBxStt->GetStation()>sta1)	sta1 = pBxStt->GetStation();
	pDom->DimCurveMoveTo(sta1, yStd, nG, FALSE, Title, nLen);

	long nSize = DimDataArray.GetSize();
	CString str1=_T(""),str2=_T("");
	for(long i=0; i<nSize; i++) 
	{	
		dp = DimDataArray.GetAt(i);
		dist = dp.x * dp.y;
		if(dp.y>1)
		{	
			if(dist<3500)
			{
				str1.Format("%s@%s",COMMA(dp.y),COMMA(dp.x));
				str2.Format("=%s",COMMA(dist));
			}
			else
				str1.Format("%s@%s=%s",COMMA(dp.y),COMMA(dp.x),COMMA(dist));
		}
		else
		{
			str1.Format("%s",COMMA(dp.x));
			str2 = _T("");
		}

		pDom->DimCurveLineTo(dist,yStd,str1,str2,nG);
		sta1 = sta2;
	}

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawDimPyung::GetDimArrayTotal(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
										   CDoubleArray &DimDataArray, __int64 nType)
{
 	DimDataArray.Add(pBxStt->GetStation());
	DimDataArray.Add(pBxEnd->GetStation());
}

// 공장이음+현장이음 or 공장이음
// nPos=0 : 상부
// nPos=1 : 하부
// nPos=2 : 복부
void CAPlateDrawDimPyung::GetDimArrayFactJoin(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
										 CDoubleArray &DimDataArray, long nPos, __int64 nType, BOOL bIncludeSplice)
{
  	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBxStt->GetGirder();

	__int64 BxFlag = 0;
	if(nPos==0)	
		BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_UPPERFLANGE;
	else if(nPos==1)
		BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_LOWERFLANGE;
	else if(nPos==2)
		BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_WEB;

	if(bIncludeSplice) BxFlag |= BX_SPLICE;

	CPlateBxFinder Finder(pGir);
	if(!pBxStt->IsState(BxFlag)) 
	{
		CPlateBasicIndex *pBxSp = Finder.GetBxLast(BxFlag,pBxStt);
		if(pBxSp)
			DimDataArray.Add(pBxSp->GetStation());
	}

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	while(pBx) 
	{
		DimDataArray.Add(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}

	if(!pBxEnd->IsState(BxFlag)) 
	{
		CPlateBasicIndex *pBxSp = Finder.GetBxFirst(BxFlag,pBxEnd);
		if(pBxSp)
			DimDataArray.Add(pBxSp->GetStation());
	}
}

void CAPlateDrawDimPyung::GetDimArraySplice(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
										   CDoubleArray &DimDataArray, __int64 nType)
{
  	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBxStt->GetGirder();

	__int64 BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_SPLICE;
	CPlateBxFinder Finder(pGir);

	if(!pBxStt->IsState(BxFlag)) 
	{
		CPlateBasicIndex *pBxCross = Finder.GetBxLast(BxFlag,pBxStt);
		if(pBxCross)
			DimDataArray.Add(pBxCross->GetStation());
	}

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	while(pBx) 
	{
		DimDataArray.Add(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}

	if(!pBxEnd->IsState(BxFlag)) 
	{
		CPlateBasicIndex *pBxCross = Finder.GetBxFirst(BxFlag,pBxEnd);
		if(pBxCross)
			DimDataArray.Add(pBxCross->GetStation());
	}
}

void CAPlateDrawDimPyung::GetDimArrayVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDPointArray &DimDataArray, __int64 nType)
{
	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBxStt->GetGirder();

	__int64 BxFlagDiap   = BX_CROSSBEAM_VBRACING|BX_STT_GIRDER|BX_END_GIRDER;
	__int64 BxFlagVStiff = BX_CROSSBEAM_VBRACING|BX_STT_GIRDER|BX_END_GIRDER|BX_VSTIFF;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlagDiap,pBxStt,pBxEnd);

	CPlateBxFinder FinderVS(pGir);
	CPlateBasicIndex *pBxVS = FinderVS.GetBxFirst(BxFlagVStiff,pBxStt,pBx);

	double PreDist = 0;
	double CurDist = 0;
	double LastSta = 0;
	CDPoint vPo;
	if(!pBxStt->IsState(BxFlagVStiff))
	{
		vPo.x = pBxVS->GetStation() - pBxStt->GetStation();
		vPo.y = 1;
		DimDataArray.Add(vPo);
		LastSta = pBxVS->GetStation();
	}

	while(pBxVS)
	{
		LastSta = pBxVS->GetStation();
		if(FinderVS.GetBxFixNext()==NULL) break;
		CurDist = FinderVS.GetBxFixNext()->GetStation() - pBxVS->GetStation();
		if(Round(CurDist,1) != Round(PreDist,1))
		{
			vPo.x  = CurDist;
			vPo.y	= 1;
			PreDist = CurDist;
			DimDataArray.Add(vPo);
		}
		else
		{
			long nSize = DimDataArray.GetSize();
			vPo    = DimDataArray.GetAt(nSize-1);
			vPo.y += 1;
			DimDataArray.SetAt(nSize-1,vPo);
		}
		pBxVS = FinderVS.GetBxNext();
	}

	while(pBx) 
	{
		PreDist = 0;
		if(Finder.GetBxFixNext()==NULL) break;
		pBxVS = FinderVS.GetBxFirst(BxFlagVStiff,pBx,Finder.GetBxFixNext());
		while(pBxVS)
		{
			LastSta = pBxVS->GetStation();
			if(FinderVS.GetBxFixNext()==NULL) break;
			CurDist = FinderVS.GetBxFixNext()->GetStation() - pBxVS->GetStation();
			if(CurDist != PreDist)
			{
				vPo.x  = CurDist;
				vPo.y	= 1;
				PreDist = CurDist;
				DimDataArray.Add(vPo);
				PreDist = CurDist;
			}
			else
			{
				long nSize = DimDataArray.GetSize();
				vPo    = DimDataArray.GetAt(nSize-1);
				vPo.y += 1;
				DimDataArray.SetAt(nSize-1,vPo);
			}
			pBxVS = FinderVS.GetBxNext();
		}
		pBx = Finder.GetBxNext();
	}

	pBx   = Finder.GetBxLast(BxFlagDiap,pBxEnd,pBxStt);
	pBxVS = FinderVS.GetBxFirst(BxFlagVStiff,pBx,pBxEnd);
	PreDist = 0;
	while(pBxVS)
	{
		LastSta = pBxVS->GetStation();
		if(FinderVS.GetBxFixNext()==NULL) break;
		CurDist = FinderVS.GetBxFixNext()->GetStation() - pBxVS->GetStation();
		if(CurDist != PreDist)
		{
			vPo.x  = CurDist;
			vPo.y	= 1;
			PreDist = CurDist;
			DimDataArray.Add(vPo);
			PreDist = CurDist;
		}
		else
		{
			long nSize = DimDataArray.GetSize();
			vPo    = DimDataArray.GetAt(nSize-1);
			vPo.y += 1;
			DimDataArray.SetAt(nSize-1,vPo);
		}
		pBxVS = FinderVS.GetBxNext();
	}

	if(!pBxEnd->IsState(BxFlagVStiff))
	{
		vPo.x = pBxEnd->GetStation() - LastSta;
		vPo.y = 1;
		DimDataArray.Add(vPo);
	}
}

void CAPlateDrawDimPyung::GetDimArrayCrossBeam(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									   CDoubleArray &DimDataArray, __int64 nType)
{
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBxStt->GetGirder();  	

	__int64 BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING;
	CPlateBxFinder Finder(pGir);

	if(!pBxStt->IsState(BxFlag)) 
	{
		CPlateBasicIndex *pBxCross = Finder.GetBxLast(BxFlag,pBxStt);
		if(pBxCross)
			DimDataArray.Add(pBxCross->GetStation());
	}

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	while(pBx) 
	{
		DimDataArray.Add(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}

	if(!pBxEnd->IsState(BxFlag)) 
	{
		CPlateBasicIndex *pBxCross = Finder.GetBxFirst(BxFlag,pBxEnd);
		if(pBxCross)
			DimDataArray.Add(pBxCross->GetStation());
	}
}

void CAPlateDrawDimPyung::GetDimArrayTendon(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									 CDoubleArray &DimDataArray, __int64 nType, BOOL bNextInclude)
{

}

void CAPlateDrawDimPyung::GetDimArraySpan(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									 CDoubleArray &DimDataArray, __int64 nType, BOOL bNextInclude)
{
  	CPlateGirderApp *pGir = (CPlateGirderApp *)pBxStt->GetGirder();  	

	__int64 BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING;
	CPlateBxFinder Finder(pGir);

	if(!pBxStt->IsJijum() && !pBxStt->IsState(BX_STT_GIRDER|BX_END_GIRDER))
	{
		BOOL bAddStation = TRUE;
		CPlateBasicIndex *pBxCross = Finder.GetBxLast(BxFlag,pBxStt);
		while(pBxCross) 
		{
			if(pBxCross->IsJijum() || pBxCross->IsState(BX_STT_GIRDER|BX_END_GIRDER))
			{
				DimDataArray.Add(pBxCross->GetStation());
				bAddStation = FALSE;
				break;
			}
			pBxCross = Finder.GetBxNext();
		}
		if(bAddStation)
			DimDataArray.Add(pBxStt->GetStation());
	}

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	while(pBx) 
	{
		if(pBx->IsJijum())
			DimDataArray.Add(pBx->GetStation());
		if(pBx->IsState(BX_STT_GIRDER|BX_END_GIRDER))
			DimDataArray.Add(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}

	if(!pBxEnd->IsJijum() && !pBxEnd->IsState(BX_STT_GIRDER|BX_END_GIRDER))
	{
		BOOL bAddStation = TRUE;
		CPlateBasicIndex *pBxCross = Finder.GetBxFirst(BxFlag,pBxEnd);
		while(pBxCross) 
		{
			if(pBxCross->IsJijum() || pBxCross->IsState(BX_STT_GIRDER|BX_END_GIRDER))
			{
				DimDataArray.Add(pBxCross->GetStation());
				bAddStation = FALSE;
				break;
			}
			pBxCross = Finder.GetBxNext();
		}
		if(bAddStation)
			DimDataArray.Add(pBxEnd->GetStation());
	}
}

void CAPlateDrawDimPyung::DrawDimSttEnd(CDomyun *pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper)
{	
 	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir    = (CPlateGirderApp	*)pBxStt->GetGirder();	
	long nG = pBxStt->GetNumberGirder();

	double dWidthStt = pGir->GetWidthOnStation(pBxStt->GetStation(), bUpper, TRUE);
	double dWidthEnd = pGir->GetWidthOnStation(pBxEnd->GetStation(), bUpper, FALSE);

	CDimDomyunEx	Dom(pBridge,pDom);
	
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double yStd = pGir->m_dGirderCenter - dWidthStt/2;	
	double Dist = Dom.GetDimLevelDis()*Dom.GetScaleDim();
	double dSpace;

	Dom.SetDirection("LEFT");
	if(pBxStt->IsState(BX_SPLICE))	dSpace = Dom.Always(20);
	else									dSpace = Dom.Always(10);
	Dom.DimBaseCurvePoint(pBxStt->GetStation() - dSpace, Dist);
	Dom.DimCurveMoveTo(-Dist, yStd, nG, FALSE);	
	Dom.DimCurveLineTo(-Dist, dWidthStt, COMMA(dWidthStt), "", nG);

	yStd = pGir->m_dGirderCenter - dWidthEnd/2;	
	Dom.SetDirection("RIGHT");
	if(pBxEnd->IsState(BX_SPLICE))	dSpace = Dom.Always(20);
	else									dSpace = Dom.Always(10);
	Dom.DimBaseCurvePoint(pBxEnd->GetStation() + dSpace, Dist);
	Dom.DimCurveMoveTo(Dist, yStd, nG, FALSE);	
	Dom.DimCurveLineTo(Dist, dWidthEnd, COMMA(dWidthEnd), "", nG);

	*pDom << Dom;
}

void CAPlateDrawDimPyung::DrawJijumStiffDim(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDan)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir    = (CPlateGirderApp	*)pBxStt->GetGirder();	
	long nG = pBxStt->GetNumberGirder();
	CDimDomyunEx	Dom(pBridge,pDom);
	CPlateBxFinder Finder(pGir);

	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_JIJUMSTIFF | BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	CDoubleArray dStaArr;
	while(pBx)
	{
		if((pBx->IsState(BX_CROSSBEAM_VBRACING) && pBx->IsJijum()) || pBx->IsState(BX_JIJUMSTIFF))
		{
			dStaArr.Add(pBx->GetStation());
		}
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}

	for(long i=0; i<dStaArr.GetSize(); i++)
	{
		double dWidth = pGir->GetWidthOnStation(dStaArr.GetAt(i),TRUE);
		double yStd = pGir->m_dGirderCenter - dWidth/2;			 
		double Dist = yStd - (nDan+1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		Dom.SetDirection("BOTTOM");
		if(i==0) 
		{
			Dom.DimBaseCurvePoint(dStaArr.GetAt(i), Dist);
			Dom.DimCurveMoveTo(dStaArr.GetAt(i), yStd, nG, FALSE);
		}
		else
		{
			CString str = COMMA(dStaArr.GetAt(i)-dStaArr.GetAt(i-1));			
			Dom.DimCurveLineTo(dStaArr.GetAt(i)-dStaArr.GetAt(i-1),yStd,(i%2) ? str : "",!(i%2) ? str : "",nG);
		}
	}
	*pDom << Dom;
}

void CAPlateDrawDimPyung::DrawJackupStiffDim(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDan)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir    = (CPlateGirderApp	*)pBxStt->GetGirder();	
	long nG = pBxStt->GetNumberGirder();
	CDimDomyunEx	Dom(pBridge,pDom);
	CPlateBxFinder Finder(pGir);

	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_JACKUP | BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	CDoubleArray dStaArr;
	while(pBx)
	{
		if((pBx->IsState(BX_CROSSBEAM_VBRACING) && pBx->IsJijum()) || pBx->IsState(BX_JACKUP))
		{
			dStaArr.Add(pBx->GetStation());
		}
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}

	for(long i=0; i<dStaArr.GetSize(); i++)
	{
		double dWidth = pGir->GetWidthOnStation(dStaArr.GetAt(i),TRUE);
		double yStd = pGir->m_dGirderCenter - dWidth/2;			 
		double Dist = yStd - (nDan+1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		Dom.SetDirection("BOTTOM");
		if(i==0) 
		{
			Dom.DimBaseCurvePoint(dStaArr.GetAt(i), Dist);
			Dom.DimCurveMoveTo(dStaArr.GetAt(i), yStd, nG, FALSE);
		}
		else
		{
			CString str = COMMA(dStaArr.GetAt(i)-dStaArr.GetAt(i-1));			
			Dom.DimCurveLineTo(dStaArr.GetAt(i)-dStaArr.GetAt(i-1),yStd,(i%2) ? str : "",!(i%2) ? str : "",nG);
		}
	}
	*pDom << Dom;
}

void CAPlateDrawDimPyung::DimHDanShoeDist(CDomyun *pDomP, long nJijum, long nDimDan)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	    *pGir  = pBridge->GetGirder(-1);
	CPlateGirderApp	    *pGirL = pBridge->GetGirder(0);
	CPlateGirderApp	    *pGirR = pBridge->GetGirder(pBridge->GetGirdersu()-1);	
	CSlabApp		*pSlabL= pBridge->GetSlab(CPlateBridgeApp::SLAB_LEFT);
	CSlabApp		*pSlabR= pBridge->GetSlab(CPlateBridgeApp::SLAB_RIGHT);

	CDimDomyunEx	Dom(pBridge, pDomP);

	double dLen = 0;
	long nDan   = nDimDan;
	BOOL bLeft  = (nJijum==pBridge->m_nQtyJigan) ? FALSE : TRUE;
	Dom.SetDirection(bLeft ? "LEFT" : "RIGHT");

	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nJijum);
	CDPoint vAng    = pBx->GetAngleSection();
	double staCen   = pBx->GetStation();
	double dLeft    = pSlabL->m_dGirderCenter - pGir->m_dGirderCenter;
	double dRight   = pSlabR->m_dGirderCenter - pGir->m_dGirderCenter;
	double staLeft  = pGir->GetStationBySc(staCen,dLeft,vAng);
	double staRight = pGir->GetStationBySc(staCen,dRight,vAng);
	double staDim   = bLeft ? min(staLeft,staRight) : max(staLeft,staRight);
	double dLevel   = Dom.GetDimLevelDis();
	double dExo	    = Dom.GetDimExo();
	double Dist		= Dom.Always(dExo) + Dom.Always(dLevel)*nDan;
	double sw		= bLeft ? -1.0 : 1.0;

	CPlateBasicIndex *pBxL = pGirL->GetBxOnJijum(nJijum);
	CPlateBasicIndex *pBxR = pGirR->GetBxOnJijum(nJijum);
	CDPoint xyL  = pGirL->GetXyGirderDis(pBxL->GetStation(), pGirL->m_dGirderCenter - pSlabL->m_dGirderCenter ,((CPlateBasicIndex*)pBxL)->GetAngleSection());// 슬래브 좌측 끝단
	CDPoint xyR  = pGirR->GetXyGirderDis(pBxR->GetStation(), pGirR->m_dGirderCenter - pSlabR->m_dGirderCenter, ((CPlateBasicIndex*)pBxR)->GetAngleSection());// 슬래브 우측 끝단
	CDPoint xy1  = xyR;
	CDPoint xy2  = CDPoint(0,0);
	CDPoint vBri = pGir->GetLine()->GetAngleAzimuth(pGir->GetStationStt());
	double  dDir = -1.0;
	double  dLenAdd = 0;

	Dom.DimBaseCurvePoint(Dist*sw, dRight);
	Dom.DimCurveMoveTo(staDim,dRight,-1);
	xy1 = xyR;
	for(long nRow=pBridge->GetGirdersu()-1; nRow>-1; nRow--)
	{		
		CPlateGirderApp *pGirShoe = pBridge->GetGirder(nRow);
		xy2  = pGirShoe->GetXyGirderDis(pBx->GetStation(),0, pBx->GetAngleSection());
		dLen = ~(xy1-xy2);
		Dom.DimCurveLineTo(staDim,dLen*dDir,COMMA(fabs(dLen)),"",-1);
		xy1  = xy2;
		dLenAdd += dLen;
	}
	xy2  = xyL;
	dLen = ~(xy1-xy2);
	Dom.DimCurveLineTo(staDim,dLen*dDir,COMMA(fabs(dLen)),"",-1);

	dLenAdd += dLen;
	// TOTAL 치수
	
	if(pBx->IsAbut())
	{
		Dist = Dom.Always(dExo) + Dom.Always(dLevel)*(nDan+1);
		dLen = dLenAdd;//~(xyL-xyR);
		Dom.DimBaseCurvePoint(Dist*sw, dRight);
		Dom.DimCurveMoveTo(staDim,dRight,-1);
		Dom.DimCurveLineTo(staDim,dLen*dDir,COMMA(fabs(dLen)),"",-1);	
	}

	*pDomP << Dom;	
}


//기준선형의 각 지점 좌표, 스테이션 정보 그리기
void CAPlateDrawDimPyung::DimXyCenter(CDomyun *pDomP)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun *pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom, HCAD_DIML);

	long	Dir = 3;
	double  dTextHeight = pDom->GetTextHeight();
	double  disArrowHeight = 20*pDom->GetScaleDim();
	CString str1,str2;
	pDom->SetTextAlignHorz(TA_RIGHT);

	for(long nJ = 0; nJ < pBridge->m_nQtyJigan+1;nJ++)
	{
		double sta = pGir->GetStationOnJijum(nJ);
		if(nJ == 0)
			sta = pGir->GetStationStt();
		else if(nJ == pBridge->m_nQtyJigan)
			sta = pGir->GetStationEnd();

		CDPoint xy = pGir->GetXyGirderDis(sta);
		str1 = GetStationForStringOutNew(toM(sta), TRUE);
		str2.Format("X = %.6lf  Y = %.6lf", toM(xy.y), toM(xy.x));
		double lenText = -str2.GetLength()*dTextHeight;
		if(nJ==pBridge->m_nQtyJigan)
		{
			Dir = 1;
			lenText *= -1;
		}

		pDom->GiSiArrowExtend(xy.x, xy.y, disArrowHeight, lenText, Dir, str1, str2);
		CDPoint vAng = pGir->GetLine()->GetAngleAzimuth(sta);
		pDom->Rotate(vAng.GetAngleRadian(),xy.x,xy.y);
		*pDomP << *pDom;
	}

	*pDomP << *pDom;
	delete pDom;
}