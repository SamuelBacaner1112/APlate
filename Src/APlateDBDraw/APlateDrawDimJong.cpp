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
		CString title = "����������";
		DimBaseJongEx(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}	
	
	if(nType & DIM_BASE_DIAP)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_DIAP, TRUE);
		CString title = "���κ�";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_SPLICE)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_SPLICE, TRUE);
		CString title = "��������";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_FACTUPPER)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_FACTUPPER, TRUE);
		CString title = "����";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_FACTLOWER)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_FACTLOWER, TRUE);
		CString title = "����";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}

	if(nType & DIM_BASE_FACTWEB)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_FACTWEB, TRUE);
		CString title = "������";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}
	
	if(nType & DIM_BASE_SPAN)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_SPAN, TRUE);
		CString title = "�氣����";
		DimBaseJong(pDomP, pBxStt, pBxEnd, DataArray, title, TRUE, nDimDan++);
	}
	if(nType & DIM_BASE_TOTAL)
	{
		CDoubleArray DataArray;
		DimPyung.GetDimArray(pBxStt, pBxEnd, DataArray, DIM_BASE_TOTAL, TRUE);
		CString title = "�ѱ���";
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

	// ó��ġ��
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

	// ó��ġ��
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
	
	// ������ġ��
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

// pGir->GetBxByStation(dStaArr.GetAt(n)) != dStArr.GetAt(n) �� ��� �Ϲݺ�
void CAPlateDrawDimJong::GetDanmyunCheckStation(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &dStaArr)
{
	CPlateGirderApp *pGir 	 = pBxStt->GetGirder();
	CPlateBasicIndex*pBx	 = NULL;

	__int64 BxFlag;
	CPlateBxFinder Finder(pGir);

	for(long nIdx=0; nIdx<6; nIdx++)
	{
		switch(nIdx)	// ����������, ���������, ����������, ���κ�, ��������, �Ϲݺ�
		{
			case 0:	BxFlag = BX_JIJUM;				break;
			case 1: BxFlag = BX_JACKUP;				break;
			case 2: BxFlag = BX_VSTIFF;				break;
			case 3:
			case 5: BxFlag = BX_CROSSBEAM_VBRACING;	break;	// �Ϲݺ� - ���κ��� ���������� ����
			case 4: BxFlag = BX_SPLICE;				break;
		}
		
		pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);

		if(nIdx != 5 && !pBx) continue;				// �Ϲݺΰ� �ƴѵ� Bx�� ������ �Ѿ

		if(nIdx == 3 && pBx->IsJijum())				// ã�� ���κ��� �����ϰ�� ���� ���κ� ã��
		{
			pBx = Finder.GetBxNext();
			if(pBx)	dStaArr.Add(pBx->GetStation());	// ���� ���κ��� ������ �Ѿ
			continue;
		}

		if(nIdx != 5)								// �Ϲݺΰ� �ƴѵ� Bx�� ������ ����
		{
			dStaArr.Add(pBx->GetStation());
			continue;
		}

		// �Ϲݺ� ��ġã��
		double dStaCross = pBx ? pBx->GetStation() : 0;
		pBx = Finder.GetBxFirst(BX_VSTIFF, pBxStt, pBxEnd);
		double dStaStiff = pBx ? pBx->GetStation() : 0;

		double dStaEnd = max(dStaCross, dStaStiff);	// ���ʿ� �ִ� ���� ��ġ ���� (�ϳ��� ������� �ش� ���� ��ġ)

		double dSta;
		if(dStaCross==0 && dStaStiff==0)			// ���κ��� ���������� ��� ������� ������ �߾��� �Ϲݺη� ����
			dSta = (pBxStt->GetStation()+pBxEnd->GetStation())/2;
		else if(dStaCross!=0 && dStaStiff!=0)		// ���κ��� ���������� ��� ������� �߾��� �Ϲݺη� ����
			dSta = (dStaCross+dStaStiff)/2;
		else if(dStaEnd+200 > pBxEnd->GetStation())	// �ϳ��� ������� +200�� �ϵ�, ������ ��� ���
			dSta = dStaEnd-200;
		else
			dSta = dStaEnd+200;

		// ������ġ�� ��ġ���� Ȯ��
		pBx = pGir->GetBxByStation(dSta);
		double dStaBx = pBx->GetStation();	// ��ó�� �ִ� ������ ��ġ�� ����

		if(pBx->IsState(BX_SPLICE))			// ��ó�� ���簡 ���������� ��� ��ĥ �� ����
		{
			CPlateSplice *pSp = pBx->GetSplice();
			// ���������� ������ ���� �ְų�, ���� ��ġ �ڿ� �������
			if(pBx->GetStation() >= pBxEnd->GetStation() || dSta < pBx->GetStation())
				dSta = pBx->GetStation() - pSp->m_dW;
			else
				dSta = pBx->GetStation() + pSp->m_dW;
		}

		if(dStaBx == dSta)
			dSta -= 100;					// ã�� �Ϲݺο� ���簡 ���� ���

		// ������ ��� ��� - ���� ����������, ������ ����. �������� �Ÿ��� �ּ� 300�� �ȴٰ� ������
		if(dSta < pBxStt->GetStation())	
			dSta += 300;

		dStaArr.Add(dSta);
	}

	dStaArr.Sort();	// ����
	dStaArr.RemoveSameValue();	// ��ġ�� ������� ������
}
