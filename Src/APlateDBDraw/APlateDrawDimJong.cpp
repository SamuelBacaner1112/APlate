// APlateDrawDimJong.cpp: implementation of the CAPlateDrawDimJong class.
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

CAPlateDrawDimJong::CAPlateDrawDimJong(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawDimJong::~CAPlateDrawDimJong()
{

}

long CAPlateDrawDimJong::DimJongTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 nType, BOOL bUppeer, BOOL str, long nDimDan)
{   	
	CAPlateDrawDimPyung DimPyung(m_pDataMng);	

	if(nType & DIM_BASE_VSTIFF)
	{
		CDPointArray DataArray;
		DimPyung.GetDimArrayVStiff(pBxStt, pBxEnd, DataArray,nType);
		CString title = "수직보강재";
		DimBaseJongEx(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}	
	
	if(nType & DIM_BASE_DIAP)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_DIAP, TRUE);
		CString title = "가로보";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_SPLICE)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_SPLICE, TRUE);
		CString title = "현장이음";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_FACTUPPER)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_FACTUPPER, TRUE);
		CString title = "상판";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_FACTLOWER)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_FACTLOWER, TRUE);
		CString title = "하판";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_FACTWEB)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_FACTWEB, TRUE);
		CString title = "복부판";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}
	
	if(nType & DIM_BASE_SPAN)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_SPAN, TRUE);
		CString title = "경간길이";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}
	if(nType & DIM_BASE_TOTAL)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_TOTAL, TRUE);
		CString title = "총길이";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}	
	return nDimDan;
}

void CAPlateDrawDimJong::DimBaseJongEx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDPointArray &DimDataArray, CString Title, BOOL bUpper, long nDan)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	if(DimDataArray.GetSize() < 2)	return;  	
	CPlateGirderApp		*pGir = pBxStt->GetGirder();

	CDimDomyunEx	*pDom = new CDimDomyunEx(pBridge,pDomP);

	double yStd = 0;
	double sta1 = pBxStt->GetStation();
	double sta2 = pBxStt->GetStation();
	double dist = 0;
	double nLen = pDom->GetTextWidth("TOTAL LENGTH",8);
	double xLen = 0;
	CDPoint dp(0,0);

	if(bUpper)
	{
		if(pGir->GetEleLevel())
			yStd = 0;
		else 
			yStd = pGir->GetLine()->GetElevationPlanEl(sta1);
		
		pDom->SetDirection("TOP");
	}
	else
	{
		yStd = pGir->GetElevationFlange(pBxEnd->GetStation(),0,CDPoint(0,1),FALSE);
		pDom->SetDirection("BOTTOM");
	}

	// 처음치수
//	if(pBxStt->GetStation()>sta1)	sta1 = pBxStt->GetStation();
//	double height1 = pGir->GetLine()->GetElevationPlanEl(pBxStt->GetStation());
//	double height2 = pGir->GetLine()->GetElevationPlanEl(pBxEnd->GetStation());
//	if(height1 < height2)
//		nDan += (long)(height1 - height2);

	pDom->DimMoveTo(sta1, yStd, nDan, FALSE, Title, nLen);

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
		sta2 = sta1 + dist;
		if(!pGir->GetEleLevel())
			yStd = pGir->GetLine()->GetElevationPlanEl(sta2);
		xLen = pGir->GetLengthDimType(sta1,sta2);
		pDom->DimLineTo(xLen,yStd,str1,str2);
		sta1 = sta2;
	}

	*pDomP << *pDom;
	delete pDom;

}

void CAPlateDrawDimJong::DimBaseJong(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, CString Title, BOOL bUpper, long nDan)
{
	if(DimDataArray.GetSize() < 2)	return;

  	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir = pBxStt->GetGirder();

	CDimDomyunEx	*pDom = new CDimDomyunEx(pBridge,pDomP);

	double yStd = 0;
	double sta1 = DimDataArray.GetAt(0);
	double sta2 = 0;
	double nLen = pDom->GetTextWidth("TOTAL LENGTH",8);
	double xLen = 0;
	double dis  = 0;
	
	if(bUpper)
	{
		if(pGir->GetEleLevel())
			yStd = 0;
		else
			yStd = pGir->GetLine()->GetElevationPlanEl(pBxStt->GetStation());
		pDom->SetDirection("TOP");
	}
	else
	{
		yStd = pGir->GetElevationFlange(pBxEnd->GetStation(),0,CDPoint(0,1),FALSE);
		pDom->SetDirection("BOTTOM");
	}

	// 처음치수
	long nSize = DimDataArray.GetSize();
	if(pBxStt->GetStation() > sta1)	sta1 = pBxStt->GetStation();
//	if(pBxStt->GetStation()>sta1)	sta1 = pBxStt->GetStation();
//	double height1 = pGir->GetLine()->GetElevationPlanEl(pBxStt->GetStation());
///	double height2 = pGir->GetLine()->GetElevationPlanEl(pBxEnd->GetStation());
//	if(height1 < height2)
//		nDan += (long)(height1 - height2);

	pDom->DimMoveTo(sta1, yStd, nDan, FALSE, Title, nLen);
	long i = 0;
	for(i=1; i<nSize-1; i++) 
	{
		sta2 = DimDataArray.GetAt(i);
		if(!pGir->GetEleLevel())
			yStd = pGir->GetLine ()->GetElevationPlanEl (sta2);
		xLen = pGir->GetLengthDimType(sta1,sta2,dis);
		pDom->DimLineTo(xLen, yStd, COMMA(sta2-sta1));

		sta1 = sta2;
	}
	
	// 마지막치수
	sta2 = DimDataArray.GetAt(i);
	if(pBxEnd->GetStation()<sta2)	sta2 = pBxEnd->GetStation();
	xLen = pGir->GetLengthDimType(sta1,sta2,dis);
	pDom->DimLineTo(xLen, yStd, COMMA(sta2-sta1));
	
	*pDomP << *pDom;
	delete pDom;

}

void CAPlateDrawDimJong::DimByunDanmyunJong(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nTab, long nType)
{
	if(pBxStt && pBxEnd && pBxStt->GetStation() >= pBxEnd->GetStation()) return;  	

	CPlateGirderApp	*pGir    = (CPlateGirderApp*)pBxStt->GetGirder();		
  	CPlateBridgeApp *pDB	 = m_pDataMng->GetBridge();

	long	nJijum	= pBxStt->GetNumberJijum();
	double dSttShoe = nJijum == -1 ? pDB->m_dLengthGirderShoeStt : 0;
	double dEndShoe	= (nJijum == pDB->m_nQtyJigan-1 || pDB->m_nQtyJigan == 1) ? pDB->m_dLengthGirderShoeEnd : 0;

	double dH1       = pBxStt->GetHeightGirderByBx(FALSE);
	double dH3       = pBxEnd->GetHeightGirderByBx(FALSE);
	double dMinus    = fabs(dH1-dH3);
	double dSttJijum = pBxStt->GetStation() + dSttShoe;
	double dEndJijum = pBxEnd->GetStation() - dEndShoe;
	double dStaStt   = pBxStt->GetStation();
	double dH2, dL1, dL2, dL3, dL4, dL5;

	CDPoint sp, ep, cp;
	sp.x = dSttJijum;
	sp.y = pGir->GetElevationFlange(dSttJijum, 0, CDPoint(0,1), FALSE);
	ep.x = dEndJijum;
	ep.y = pGir->GetElevationFlange(dEndJijum, 0, CDPoint(0,1), FALSE);
	double dLenJigan = ABS(sp.x-ep.x);
	pDomP->SetDimExo(30);
	switch(nType)
	{
	default :
		dL1 = dLenJigan;
		
		pDomP->SetDirection("BOTTOM");
		pDomP->DimMoveTo(dSttJijum, sp.y-dMinus-pDomP->Always(3), 0);
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum, dSttJijum+dL1, 0), 0, "L1");
		
		pDomP->SetDirection("LEFT");
		pDomP->DimMoveTo(sp.x-pDB->m_dLengthGirderShoeStt, sp.y, 0);
		pDomP->DimLineTo(dH1, 0, "H1");

		pDomP->SetDirection("RIGHT");
		pDomP->DimMoveTo(ep.x+pDB->m_dLengthGirderShoeEnd, ep.y, 0);
		pDomP->DimLineTo(dH3, 0, "H3");
		break;

	case 1:
		dH2 = pDB->m_dHeightByun[nTab];
		dL1 = pDB->m_dLenLeftByun[nTab];
		dL2 = pDB->m_dLenLeftCurve[nTab];
		dL4 = pDB->m_dLenRightCurve[nTab];
		dL5 = pDB->m_dLenRightByun[nTab];
		dL3 = dLenJigan - (dL1+dL2+dL4+dL5);
		cp.x = dStaStt+pGir->GetLengthDimType(dStaStt,dSttJijum+dL1+dL2+dL3/2,0);
		cp.y = pGir->GetElevationFlange(dSttJijum+dL1+dL2+dL3/2,0,CDPoint(0,1), FALSE);

		pDomP->SetDirection("BOTTOM");
		pDomP->DimMoveTo(sp.x, sp.y-dMinus-pDomP->Always(3), 0);
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum,dSttJijum+dL1,0), 0, "L1");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1,dSttJijum+dL1+dL2,0), 0, "L2");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1+dL2,dSttJijum+dL1+dL2+dL3,0), 0, "L3");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1+dL2+dL3,dSttJijum+dL1+dL2+dL3+dL4,0), 0, "L4");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1+dL2+dL3+dL4,dSttJijum+dL1+dL2+dL3+dL4+dL5,0), 0, "L5");
		
		pDomP->SetDirection("LEFT");
		pDomP->DimMoveTo(sp.x-pDB->m_dLengthGirderShoeStt, sp.y, 0);
		pDomP->DimLineTo(dH1, 0, "H1");
		pDomP->DimMoveTo(cp.x, cp.y, 0);
		pDomP->DimLineTo(dH2, 0, "H2");

		pDomP->SetDirection("RIGHT");
		pDomP->DimMoveTo(ep.x+pDB->m_dLengthGirderShoeEnd, ep.y, 0);
		pDomP->DimLineTo(dH3, 0, "H3");
		break;

	case 2:
		dL2 = pDB->m_dLenRightCurve[nTab];
		dL3 = pDB->m_dLenRightByun[nTab];
		dL1 = dLenJigan - (dL2+dL3);

		pDomP->SetDirection("BOTTOM");
		pDomP->DimMoveTo(sp.x, sp.y-dMinus-pDomP->Always(3), 0);
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum,dSttJijum+dL1,0), 0, "L1");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1,dSttJijum+dL1+dL2,0), 0, "L2");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1+dL2,dSttJijum+dL1+dL2+dL3,0), 0, "L3");
		
		pDomP->SetDirection("LEFT");
		pDomP->DimMoveTo(sp.x-pDB->m_dLengthGirderShoeStt, sp.y, 0);
		pDomP->DimLineTo(dH1, 0, "H1");

		pDomP->SetDirection("RIGHT");
		pDomP->DimMoveTo(ep.x+pDB->m_dLengthGirderShoeEnd, ep.y, 0);
		pDomP->DimLineTo(dH3, 0, "H3");
		break;
		
	case 3:
		dL1 = pDB->m_dLenLeftByun[nTab];
		dL2 = pDB->m_dLenLeftCurve[nTab];
		dL3 = dLenJigan - (dL1+dL2);

		pDomP->SetDirection("BOTTOM");
		pDomP->DimMoveTo(sp.x, sp.y-dMinus-pDomP->Always(3), 0);
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum,dSttJijum+dL1,0), 0, "L1");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1,dSttJijum+dL1+dL2,0), 0, "L2");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1+dL2,dSttJijum+dL1+dL2+dL3,0), 0, "L3");
		
		pDomP->SetDirection("LEFT");
		pDomP->DimMoveTo(sp.x-pDB->m_dLengthGirderShoeStt, sp.y, 0);
		pDomP->DimLineTo(dH1, 0, "H1");

		pDomP->SetDirection("RIGHT");
		pDomP->DimMoveTo(ep.x+pDB->m_dLengthGirderShoeEnd, ep.y, 0);
		pDomP->DimLineTo(dH3, 0, "H3");
		break;
		
	case 4:
		dH2 = pDB->m_dHeightByun[nTab];
		dL1 = pDB->m_dLenLeftByun[nTab];
		dL3 = pDB->m_dLenRightByun[nTab];
		dL2 = dLenJigan - (dL1+dL3);
		cp.x = dStaStt+pGir->GetLengthDimType(dStaStt,dSttJijum+dL1+dL2/2,0);
		cp.y = pGir->GetElevationFlange(dSttJijum+dL1+dL2/2,0,CDPoint(0,1), FALSE);

		pDomP->SetDirection("BOTTOM");
		pDomP->DimMoveTo(dSttJijum, sp.y-dMinus-pDomP->Always(3), 0);
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum,dSttJijum+dL1,0), 0, "L1");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1,dSttJijum+dL1+dL2,0), 0, "L2");
		pDomP->DimLineTo(pGir->GetLengthDimType(dSttJijum+dL1+dL2,dSttJijum+dL1+dL2+dL3,0), 0, "L3");
		
		pDomP->SetDirection("LEFT");
		pDomP->DimMoveTo(sp.x-pDB->m_dLengthGirderShoeStt, sp.y, 0);
		pDomP->DimLineTo(dH1, 0, "H1");
		pDomP->DimMoveTo(cp.x, cp.y, 0);
		pDomP->DimLineTo(dH2, 0, "H2");

		pDomP->SetDirection("RIGHT");
		pDomP->DimMoveTo(ep.x+pDB->m_dLengthGirderShoeEnd, ep.y, 0);
		pDomP->DimLineTo(dH3, 0, "H3");
		break;
	}
}

void CAPlateDrawDimJong::DimDanmyunCheckMark(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();		
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir 	 = pBxStt->GetGirder();

	CDomyun Dom(pDom);
	CConcSymbol	SymDom(pOptStd);

	CDoubleArray dStaArr;
	GetDanmyunCheckStation(pBxStt, pBxEnd, dStaArr);

	CDPoint xy;
	char	sc		= 'A';	
	double	dStdSta = pBxStt->GetStation();
	double	dDis	= 0;
	for(long n=0; n<dStaArr.GetSize(); n++)
	{
		xy.x = pGir->GetLengthDimType(dStdSta, dStaArr.GetAt(n)) + dStdSta;
		xy.y = pGir->GetElevationFlangeCamber(dStaArr.GetAt(n)) + pDB->m_BindConc.m_dHeight;
		dDis = (pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDis) + pGir->GetHeightGirderByJijum(0) + 200;
		SymDom.ArrowSection(&Dom, sc, xy, dDis, CDPoint(1, 0));
		sc++;
	}

	*pDom << Dom;	
}

// pGir->GetBxByStation(dStaArr.GetAt(n)) != dStArr.GetAt(n) 일 경우 일반부
void CAPlateDrawDimJong::GetDanmyunCheckStation(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &dStaArr)
{
	CPlateGirderApp *pGir 	 = pBxStt->GetGirder();
	CPlateBasicIndex*pBx	 = NULL;

	__int64 BxFlag;
	CPlateBxFinder Finder(pGir);

	for(long nIdx=0; nIdx<6; nIdx++)
	{
		switch(nIdx)	// 지점보강재, 잭업보강재, 수직보강재, 가로보, 현장이음, 일반부
		{
			case 0:	BxFlag = BX_JIJUM;				break;
			case 1: BxFlag = BX_JACKUP;				break;
			case 2: BxFlag = BX_VSTIFF;				break;
			case 3:
			case 5: BxFlag = BX_CROSSBEAM_VBRACING;	break;	// 일반부 - 가로보와 수직보강재 사이
			case 4: BxFlag = BX_SPLICE;				break;
		}
		
		pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);

		if(nIdx != 5 && !pBx) continue;				// 일반부가 아닌데 Bx가 없으면 넘어감

		if(nIdx == 3 && pBx->IsJijum())				// 찾은 가로보가 지점일경우 다음 가로보 찾음
		{
			pBx = Finder.GetBxNext();
			if(pBx)	dStaArr.Add(pBx->GetStation());	// 다음 가로보가 없으면 넘어감
			continue;
		}

		if(nIdx != 5)								// 일반부가 아닌데 Bx가 있으면 저장
		{
			dStaArr.Add(pBx->GetStation());
			continue;
		}

		// 일반부 위치찾기
		double dStaCross = pBx ? pBx->GetStation() : 0;
		pBx = Finder.GetBxFirst(BX_VSTIFF, pBxStt, pBxEnd);
		double dStaStiff = pBx ? pBx->GetStation() : 0;

		double dStaEnd = max(dStaCross, dStaStiff);	// 뒤쪽에 있는 부재 위치 저장 (하나만 있을경우 해당 부재 위치)

		double dSta;
		if(dStaCross==0 && dStaStiff==0)			// 가로보와 수직보강재 모두 없을경우 구간의 중앙을 일반부로 잡음
			dSta = (pBxStt->GetStation()+pBxEnd->GetStation())/2;
		else if(dStaCross!=0 && dStaStiff!=0)		// 가로보와 수직보강재 모두 있을경우 중앙을 일반부로 잡음
			dSta = (dStaCross+dStaStiff)/2;
		else if(dStaEnd+200 > pBxEnd->GetStation())	// 하나만 있을경우 +200을 하되, 구간을 벗어난 경우
			dSta = dStaEnd-200;
		else
			dSta = dStaEnd+200;

		// 부재위치와 겹치는지 확인
		pBx = pGir->GetBxByStation(dSta);
		double dStaBx = pBx->GetStation();	// 근처에 있는 부재의 위치를 잡음

		if(pBx->IsState(BX_SPLICE))			// 근처의 부재가 현장이음일 경우 겹칠 수 있음
		{
			CPlateSplice *pSp = pBx->GetSplice();
			// 현장이음이 구간의 끝에 있거나, 현재 위치 뒤에 있을경우
			if(pBx->GetStation() >= pBxEnd->GetStation() || dSta < pBx->GetStation())
				dSta = pBx->GetStation() - pSp->m_dW;
			else
				dSta = pBx->GetStation() + pSp->m_dW;
		}

		if(dStaBx == dSta)
			dSta -= 100;					// 찾은 일반부에 부재가 있을 경우

		// 구간을 벗어난 경우 - 거의 없을테지만, 만일을 위해. 시종점의 거리가 최소 300은 된다고 가정함
		if(dSta < pBxStt->GetStation())	
			dSta += 300;

		dStaArr.Add(dSta);
	}

	dStaArr.Sort();	// 정렬
	dStaArr.RemoveSameValue();	// 위치가 같은경우 삭제함
}
