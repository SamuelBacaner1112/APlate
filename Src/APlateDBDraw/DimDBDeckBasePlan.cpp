// DimDBDeckBasePlan.cpp: implementation of the CDimDBDeckBasePlan class.
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

CDimDBDeckBasePlan::CDimDBDeckBasePlan(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CDimDBDeckBasePlan::~CDimDBDeckBasePlan()
{

}

// bUpper = TRUE -> 0번거더에 치수표기(평면상위)
void CDimDBDeckBasePlan::DimBasePlan(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
										 CDoubleArray &DimDataArray, CString Title, 
										 BOOL bLeft, long nDan, BOOL bSpanArrow)
{
	if(DimDataArray.GetSize()==0)	return;

	CPlateBridgeApp	    *pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGirCen	= pBxStt->GetGirder();
	CPlateGirderApp		*pLGir		= pDB->GetGirder(0);
	CPlateGirderApp		*pRGir		= pDB->GetGirder(pDB->GetGirdersu()-1);
	CADeckData			*pDeckData	= m_pDataMng->GetDeckData();

	CDPoint CLeft[4], CRigh[4];
	pDB->GetXyFrontCantilever(pBxStt, CLeft, TRUE);

	CPlateBasicIndex *pBxSttRigh = pBxStt->GetBxMatchByCrossBeam(pRGir);
	pDB->GetXyFrontCantilever(pBxSttRigh, CRigh, FALSE);
	double LDist = -(CLeft[3].x-CLeft[0].x) - pLGir->GetWidthFlangeUpper(pBxStt)/2 + pDeckData->m_dCoverSd;
	double RDist = (CRigh[0].x-CRigh[3].x) + pRGir->GetWidthFlangeUpper(pBxSttRigh)/2 - pDeckData->m_dCoverSd;
		
	CDimLineDomyun	Dom(pDomP, (CLineInfoApp *)pGirCen->GetLine());

	double dist = 0;	
	double	dStaBase, dStaStt, dStaEnd, dStaNext;
	double dDistEditRebar = bLeft ? pLGir->m_dGirderCenter+LDist : pRGir->m_dGirderCenter+RDist;
	if(bLeft)
	{
		Dom.SetDirection("TOP");
		dStaBase = pLGir->GetStationBySc(pLGir->GetSSS(OFF_STT_SLAB), LDist, pLGir->GetAAA(OFF_STT_SLAB));					
		dStaStt = pLGir->GetStationBySc(pBxStt->GetStation(), LDist, pBxStt->GetAngle());
		dStaEnd = pLGir->GetStationBySc(pBxEnd->GetStation(), LDist, pBxEnd->GetAngle());

		Dom.DimMoveToOnLine(dStaStt, dDistEditRebar, nDan);
	}
	else
	{
		Dom.SetDirection("BOTTOM");
		dStaBase = pLGir->GetStationBySc(pRGir->GetSSS(OFF_STT_SLAB), RDist, pRGir->GetAAA(OFF_STT_SLAB));		
		dStaStt = pLGir->GetStationBySc(pBxStt->GetStation(), pRGir->m_dGirderCenter-pLGir->m_dGirderCenter+RDist, pBxStt->GetAngle());
		dStaEnd = pLGir->GetStationBySc(pBxEnd->GetStation(), pRGir->m_dGirderCenter-pLGir->m_dGirderCenter+RDist, pBxEnd->GetAngle());

		Dom.DimMoveToOnLine(dStaStt, dDistEditRebar, nDan);				
	}

	// 시점 유간	
	if(pBxStt->IsState(BX_STT_JOINT))	
	{		
		Dom.DimLineOverText(COMMA(pDB->m_dLengthUganStt),"",TRUE);
		double dLenUganStt = pGirCen->GetLine()->GetStationByDisLength(dStaBase, pDB->m_dLengthUganStt, dDistEditRebar) - dStaBase;
		Dom.DimLineToOnLine(dLenUganStt, "");	
		//dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, pDB->m_dLengthUganStt, dDistEditRebar);
	}

	long nSize = DimDataArray.GetSize();		
	double dLenDist, dPreDist=nSize>0 ? DimDataArray.GetAt(0) : 0; // 의미없는 큰값
	CString str1=_T(""),str2=_T("");	
	BOOL bUpperPrint = FALSE;
	for(long i=1; i<nSize; i++) 
	{	
		dist = DimDataArray.GetAt(i);		

		if(dPreDist<400 && dist<400 && bUpperPrint)
		{
			str1 = _T("");
			str2.Format("%s",COMMA(dist));				
			bUpperPrint = FALSE;
		}
		else
		{			
			str1.Format("%s",COMMA(dist));
			str2 = _T("");
			bUpperPrint = TRUE;
		}		
				
		dStaNext = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist - dPreDist, dDistEditRebar);
		if(dStaBase < dStaStt && dStaNext > dStaEnd)
		{
			double dLenDist = pGirCen->GetLine()->GetStationByDisLength(dStaStt, dStaEnd-dStaStt, 0) - dStaStt;
			Dom.DimLineToOnLine(dLenDist, str1, str2, DTYPE_STT_ARROW | DTYPE_END_ARROW);				
			break;
		}
		else if(dStaNext > dStaEnd && !pBxEnd->IsState(BX_END_JOINT))
		{
			double dLenDist = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dStaEnd-dStaBase, 0) - dStaBase;
			Dom.DimLineToOnLine(dLenDist, str1, str2, DTYPE_STT_GEN | DTYPE_END_ARROW);				
			break;
		}
		else if(dStaBase < dStaStt && dStaNext >= dStaStt)
		{
			dLenDist = dStaNext-dStaStt;
			Dom.DimLineToOnLine(dLenDist, str1, str2, DTYPE_STT_ARROW | DTYPE_END_GEN);				
			dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist - dPreDist, dDistEditRebar);
		}
		else if(dStaBase < dStaStt )
		{
			dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist - dPreDist, dDistEditRebar);
		}
		else
		{		
			double dLenDist = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist - dPreDist, dDistEditRebar) - dStaBase;
			Dom.DimLineToOnLine(dLenDist, str1, str2);				
			dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist - dPreDist, dDistEditRebar);
		}	
		dPreDist = dist;
	}	
	if(pBxEnd->IsState(BX_END_JOINT))	
	{
		Dom.DimLineOverText(COMMA(pDB->m_dLengthUganEnd),"",FALSE);
		double dLenUganEnd = pGirCen->GetLine()->GetStationByDisLength(dStaBase, pDB->m_dLengthUganEnd, dDistEditRebar) - dStaBase;
		Dom.DimLineToOnLine(dLenUganEnd, "");			
	}	

	*pDomP << Dom;	
}

// x값은 스테이션이 아니고 거리값....
// @표시를 포함하는 치수 : 수직보강재, 다이아프램(?)
void CDimDBDeckBasePlan::DimBasePlanEx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
								   CDPointArray &DimDataArray, CDoubleArray &DimRealDist, CString Title, 
								   BOOL bUpper, BOOL bLeft, long nDan, BOOL bSpanArrow)
{
	if(DimDataArray.GetSize()==0)	return;

	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CPlateBridgeApp	    *pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp		*pGirCen	= pDB->GetGirder(-1);
	CPlateGirderApp		*pLGir		= pDB->GetGirder(0);
	CPlateGirderApp		*pRGir		= pDB->GetGirder(pDB->GetGirdersu()-1);

	CDPoint	vAngStt  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd  = pGirCen->GetAAA(OFF_END_SLAB);
	CDPoint CLeft[4], CRigh[4];
	pDB->GetXyFrontCantilever(pBxStt, CLeft, TRUE);

	CPlateBasicIndex *pBxSttRigh = pBxStt->GetBxMatchByCrossBeam(pRGir);
	pDB->GetXyFrontCantilever(pBxSttRigh, CRigh, FALSE);
	
	double	LDist = -(CLeft[3].x-CLeft[0].x) - pLGir->GetWidthFlangeUpper(pBxStt)/2 + pDeckData->m_dCoverSd;
	double	RDist =  (CRigh[0].x-CRigh[3].x) + pRGir->GetWidthFlangeUpper(pBxSttRigh->GetBxMatchByCrossBeam(pRGir))/2 - pDeckData->m_dCoverSd;
	double	dStaBase, dStaStt, dStaEnd, dStaNext;
	
	CDimLineDomyun	Dom(pDomP, (CLineInfoApp *)pGirCen->GetLine());			
	double dDistEditRebar = bLeft ? pLGir->m_dGirderCenter+LDist : pRGir->m_dGirderCenter+RDist;
	if(bLeft)
	{
		Dom.SetDirection("TOP");
		dStaBase = pLGir->GetStationBySc(pLGir->GetSSS(OFF_STT_SLAB), LDist, pLGir->GetAAA(OFF_STT_SLAB));					
		dStaStt  = pLGir->GetStationBySc(pBxStt->GetStation(), LDist, pBxStt->GetAngle());
		dStaEnd  = pLGir->GetStationBySc(pBxEnd->GetStation(), LDist, pBxEnd->GetAngle());

		Dom.DimMoveToOnLine(dStaStt, dDistEditRebar, nDan);
	}
	else
	{
		Dom.SetDirection("BOTTOM");
		dStaBase = pLGir->GetStationBySc(pRGir->GetSSS(OFF_STT_SLAB), RDist, pRGir->GetAAA(OFF_STT_SLAB));		
		dStaStt  = pLGir->GetStationBySc(pBxStt->GetStation(), pRGir->m_dGirderCenter-pLGir->m_dGirderCenter+RDist, pBxStt->GetAngle());
		dStaEnd = pLGir->GetStationBySc(pBxEnd->GetStation(), pRGir->m_dGirderCenter-pLGir->m_dGirderCenter+RDist, pBxEnd->GetAngle());

		Dom.DimMoveToOnLine(dStaStt, dDistEditRebar, nDan);				
	}
	
	// 시점 유간	
	if(pBxStt->IsState(BX_STT_JOINT))	
	{		
		double dLenUganStt = pDeckData->GetWidthUgan(TRUE, bLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);
		Dom.DimLineOverText(COMMA(dLenUganStt),"",TRUE);
		dLenUganStt = pGirCen->GetLine()->GetStationByDisLength(dStaBase,dLenUganStt, dDistEditRebar) - dStaBase;
		Dom.DimLineToOnLine(dLenUganStt, "");	
		//dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dLenUganStt, dDistEditRebar);
	}

	long nSize = DimDataArray.GetSize();
	CDPoint dp;
	double dist, dLenDist, dPreDist=10000; // 의미없는 큰값
	CString str1=_T(""),str2=_T("");	
	BOOL bUpperPrint = FALSE;
	for(long i=0; i<nSize; i++) 
	{	
		dp = DimDataArray.GetAt(i);
		dist = DimRealDist[i];

		if(dp.x>1)
		{	
			if(dist<3500 && dPreDist>400)
			{
				str1.Format("%s@%s",COMMA(dp.x),COMMA(dp.y));
				str2.Format("=%s",COMMA(dp.x * dp.y));
			}
			else if(dist<3500)
			{
				str1 = _T("");
				str2.Format("%s@%s=%s",COMMA(dp.x),COMMA(dp.y),COMMA(dp.x * dp.y));
			}
			else
			{
				str1.Format("%s@%s=%s",COMMA(dp.x),COMMA(dp.y),COMMA(dp.x * dp.y));
				str2 = _T("");
			}
			dPreDist = 10000;
		}
		else
		{
			if(dPreDist<400 && dp.y<400 && bUpperPrint)
			{
				str1 = _T("");
				str2.Format("%s",COMMA(dp.y));				
				bUpperPrint = FALSE;
			}
			else
			{			
				str1.Format("%s",COMMA(dp.y));
				str2 = _T("");
				bUpperPrint = TRUE;
			}
			dPreDist = dp.y;
		}
		
		dStaNext = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist, dDistEditRebar);
		if(dStaBase < dStaStt && dStaNext > dStaEnd)
		{
			double dLenDist = pGirCen->GetLine()->GetStationByDisLength(dStaStt, dStaEnd-dStaStt, 0) - dStaStt;
			Dom.DimLineToOnLine(dLenDist, str1, str2, DTYPE_STT_ARROW | DTYPE_END_ARROW);				
			break;
		}
		if(dStaNext > dStaEnd && !pBxEnd->IsState(BX_END_JOINT))
		{
			dLenDist = dStaEnd-dStaBase;
			Dom.DimLineToOnLine(dLenDist, str1, str2, DTYPE_STT_GEN | DTYPE_END_ARROW);				
			break;
		}	
		else if(dStaBase < dStaStt && dStaNext >= dStaStt)
		{
			dLenDist = dStaNext-dStaStt;
			Dom.DimLineToOnLine(dLenDist, str1, str2, DTYPE_STT_ARROW | DTYPE_END_GEN);				
			dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist, dDistEditRebar);
		}
		else if(dStaBase < dStaStt )
		{
			dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist, dDistEditRebar);
		}
		else
		{		
			dLenDist = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist, dDistEditRebar) - dStaBase;
			Dom.DimLineToOnLine(dLenDist, str1, str2);				
			dStaBase = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dist, dDistEditRebar);
		}							
	}	
	
	if(pBxEnd->IsState(BX_END_JOINT))	
	{
		double dLenUganEnd = pDeckData->GetWidthUgan(FALSE, bLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);
		Dom.DimLineOverText(COMMA(dLenUganEnd),"",FALSE);
		dLenUganEnd = pGirCen->GetLine()->GetStationByDisLength(dStaBase, dLenUganEnd, dDistEditRebar) - dStaBase;
		Dom.DimLineToOnLine(dLenUganEnd, "");			
	}	
 
	*pDomP << Dom;
}

void CDimDBDeckBasePlan::DimBasePlanExSabogang(CDomyun *pDomP, CPlateBasicIndex *pBx, CDPointArray &DimDataArray, BOOL bLeft, BOOL bUpper)
{
	if(DimDataArray.GetSize()==0)	return;

	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	CPlateBridgeApp	    *pDB  = m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir = pDB->GetGirder(-1);
	CPlateGirderApp		*pLGir= pDB->GetGirder(0);
	CPlateGirderApp		*pRGir= pDB->GetGirder(pDB->GetGirdersu()-1);

	long nDan = 0;
	CDPoint CLeft[4], CRigh[4];
	pDB->GetXyFrontCantilever(pBx, CLeft, TRUE);
	pDB->GetXyFrontCantilever(pBx, CRigh, FALSE);
	double LDist = -(CLeft[3].x-CLeft[0].x) - pLGir->GetWidthFlangeUpper(pBx)/2 + pDeckData->m_dCoverSd;
	double RDist = (CRigh[0].x-CRigh[3].x) + pRGir->GetWidthFlangeUpper(pBx->GetBxMatchByCrossBeam(pRGir))/2 - pDeckData->m_dCoverSd;
	
	CDimLineDomyun	Dom(pDomP, (CLineInfoApp *)pGir->GetLine());		
	double dStaBase = pBx->GetStation();

	if(bLeft)
	{
		Dom.SetDirection("TOP");
		dStaBase = pLGir->GetStationBySc(dStaBase, LDist, pBx->GetAngle());
		Dom.DimMoveToOnLine(dStaBase, pLGir->m_dGirderCenter+LDist, nDan);		
	}
	else
	{
		Dom.SetDirection("BOTTOM");
		dStaBase = pLGir->GetStationBySc(dStaBase, pRGir->m_dGirderCenter-pLGir->m_dGirderCenter+RDist, pBx->GetAngle());
		Dom.DimMoveToOnLine(dStaBase, pRGir->m_dGirderCenter+RDist, nDan);				
	}
	
	// 시점 유간
	double dSide;
	double dDistEditRebar = bLeft ? pLGir->m_dGirderCenter+LDist : pRGir->m_dGirderCenter+RDist;
	if(pBx->IsState(BX_STT_JOINT))	
	{		
		double dLenUganStt = pGir->GetLine()->GetStationByDisLength(dStaBase, pDB->m_dLengthUganStt, dDistEditRebar) - dStaBase;		
		dStaBase = pGir->GetLine()->GetStationByDisLength(dStaBase, pDB->m_dLengthUganStt, dDistEditRebar);
		dSide = 1;
	}
	if(pBx->IsState(BX_END_JOINT))	
	{
		double dLenUganEnd = pGir->GetLine()->GetStationByDisLength(dStaBase, pDB->m_dLengthUganEnd, dDistEditRebar) - dStaBase;		
		dStaBase = pGir->GetLine()->GetStationByDisLength(dStaBase, -pDB->m_dLengthUganEnd, dDistEditRebar);
		dSide = -1;
	}			

	if(bLeft)	
		Dom.DimMoveToOnLine(dStaBase,pLGir->m_dGirderCenter+LDist,0);			
	else
		Dom.DimMoveToOnLine(dStaBase,pRGir->m_dGirderCenter+RDist,0);					

	long nSize = DimDataArray.GetSize();
	CDPoint dp;
	double dist, dPreDist=10000; // 의미없는 큰값
	CString str1=_T(""),str2=_T("");	
	BOOL bUpperPrint = FALSE;
	for(long i=0; i<nSize; i++) 
	{	
		dp = DimDataArray.GetAt(i);
		dist = dp.x * dp.y;	
		if(dp.x>1)
		{	
			if(dist<3500)
			{
				str1.Format("%s@%s",COMMA(dp.x),COMMA(dp.y));
				str2.Format("=%s",COMMA(dp.x * dp.y));
			}
			else
			{
				str1.Format("%s@%s=%s",COMMA(dp.x),COMMA(dp.y),COMMA(dp.x * dp.y));
				str2 = _T("");
			}
			dPreDist = 10000;
		}
		else
		{
			if(dPreDist<400 && dp.y<400 && bUpperPrint)
			{
				str1 = _T("");
				str2.Format("%s",COMMA(dp.y));				
				bUpperPrint = FALSE;
			}
			else
			{			
				str1.Format("%s",COMMA(dp.y));
				str2 = _T("");
				bUpperPrint = TRUE;
			}
			dPreDist = dp.y;
		}
		double dLenDist = pGir->GetLine()->GetStationByDisLength(dStaBase, dist, dDistEditRebar) - dStaBase;
		Dom.DimLineToOnLine(dLenDist * dSide, str1, str2);				
					
		dStaBase = pGir->GetLine()->GetStationByDisLength(dStaBase, dist * dSide, dDistEditRebar);		
	}				
	*pDomP << Dom;
}

// bRealDim=TRUE : 실제 철근이 배치되는 CTC를 적용
// bRealDim=FALSE: 치수용에 배치되는 CTC를 적용
void CDimDBDeckBasePlan::GetDimArrayMainRebar(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
										CDPointArray &DimDataArray,BOOL bUpper, BOOL bLeft, BOOL bRealDim)
{
	CADeckData	*pDeck = m_pDataMng->GetDeckData();
	
	CDPointArray *pArr;
	if(bRealDim)
	{
		if(bUpper)	pArr = &pDeck->m_arrCTCMainRebar[bLeft ? CTC_MAIN_UPPER_LEFT : CTC_MAIN_UPPER_RIGHT];
		else		pArr = &pDeck->m_arrCTCMainRebar[bLeft ? CTC_MAIN_LOWER_LEFT : CTC_MAIN_LOWER_RIGHT];
	}
	else
	{
		if(bUpper)	pArr = &pDeck->m_arrCTCMainRebarForDim[bLeft ? CTC_MAIN_UPPER_LEFT : CTC_MAIN_UPPER_RIGHT];
		else		pArr = &pDeck->m_arrCTCMainRebarForDim[bLeft ? CTC_MAIN_LOWER_LEFT : CTC_MAIN_LOWER_RIGHT];
	}


	double dStaBridgeStt = m_pDataMng->GetBridge()->m_dStationBridgeStt + m_pDataMng->GetBridge()->m_dLengthUganStt;
	double dExpWidthStt  =  pDeck->GetWidthBlockOut(TRUE, bLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);
	double dExpWidthEnd  = pDeck->GetWidthBlockOut(FALSE, bLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);
	if(pDeck->m_bInstallMainRebarAtBlockOut)
	{
		dExpWidthStt = 0;
		dExpWidthEnd = 0;
	}

	double dL2 = 0;
	double dL1 = pDeck->GetLengthMainRebarToCover(TRUE, bLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT, bUpper, dL2);

	//if(bUpper)	DimDataArray.Add(CDPoint(1,dExpWidthStt));		// 신축이음부
	DimDataArray.Add(CDPoint(1,dExpWidthStt));		// 신축이음부
	if(dL2==0)	DimDataArray.Add(CDPoint(1, pDeck->m_dCoverSd));			// 피복
	if(dL1>0)	DimDataArray.Add(CDPoint(1, dL2>0 ? dL1+pDeck->m_dCoverSd : dL1));							// ~사각부주철근까지

	CDPointArray *pArrSkew;
	if(bRealDim)
	{
		if(bUpper)	pArrSkew = &pDeck->m_arrCTCMainRebarAtSkew[bLeft?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_RIGH_STT];
		else		pArrSkew = &pDeck->m_arrCTCMainRebarAtSkew[bLeft?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_RIGH_STT];
	}
	else
	{
		if(bUpper)	pArrSkew = &pDeck->m_arrCTCMainRebarAtSkewForDim[bLeft?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_RIGH_STT];
		else		pArrSkew = &pDeck->m_arrCTCMainRebarAtSkewForDim[bLeft?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_RIGH_STT];
	}
	if(dL2>0)
	{
		for(long n=0; n<pArrSkew->GetSize(); n++)
		{
			CDPoint xyDim = pArrSkew->GetAt(n);
			DimDataArray.Add(xyDim);								// 사각부주철근~주철근까지
		}
	}
	//}

	// 주철근
	for(long n=0; n<pArr->GetSize(); n++)
	{
		CDPoint ptData = pArr->GetAt(n);
		DimDataArray.Add(ptData);		
	}

	if(pBxEnd->IsState(BX_END_SLAB | BX_END_JOINT))
	{		
		double dL2 = 0;
		double dL1 = pDeck->GetLengthMainRebarToCover(FALSE, bLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT, bUpper, dL2);

		CDPointArray *pArrSkew;

		if(bRealDim)
		{
			if(bUpper)	pArrSkew = &pDeck->m_arrCTCMainRebarAtSkew[bLeft?CTC_SKEWMAIN_UPPER_LEFT_END:CTC_SKEWMAIN_UPPER_RIGH_END];
			else		pArrSkew = &pDeck->m_arrCTCMainRebarAtSkew[bLeft?CTC_SKEWMAIN_LOWER_LEFT_END:CTC_SKEWMAIN_LOWER_RIGH_END];
		}
		else
		{
			if(bUpper)	pArrSkew = &pDeck->m_arrCTCMainRebarAtSkewForDim[bLeft?CTC_SKEWMAIN_UPPER_LEFT_END:CTC_SKEWMAIN_UPPER_RIGH_END];
			else		pArrSkew = &pDeck->m_arrCTCMainRebarAtSkewForDim[bLeft?CTC_SKEWMAIN_LOWER_LEFT_END:CTC_SKEWMAIN_LOWER_RIGH_END];
		}
		if(dL2>0)
		{
			for(long n=0; n<pArrSkew->GetSize(); n++)
			{
				CDPoint xyDim = pArrSkew->GetAt(n);
				DimDataArray.Add(xyDim);								// 주철근~사각부주철근까지
			}
		}

		if(dL1>0)	DimDataArray.Add(CDPoint(1, dL2>0 ? dL1+pDeck->m_dCoverSd : dL1));							// ~사각부주철근까지
		if(dL2==0)	DimDataArray.Add(CDPoint(1, pDeck->m_dCoverSd));			// 피복
		//if(bUpper)	DimDataArray.Add(CDPoint(1, dExpWidthEnd));		
		DimDataArray.Add(CDPoint(1, dExpWidthEnd));		
	}
}

void CDimDBDeckBasePlan::GetDimArraySabogangRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray, BOOL bUpper)
{
	CADeckData	*pDeck = m_pDataMng->GetDeckData();

	long nIdx = 0;
	if(pBx->IsState(BX_STT_SLAB | BX_STT_JOINT))	
		nIdx = (bUpper) ? CTC_SABOGANG_UPPER_STT : CTC_SABOGANG_LOWER_STT;	
	else
		nIdx = (bUpper) ? CTC_SABOGANG_UPPER_END : CTC_SABOGANG_LOWER_END;	

	double dStaBridgeStt = m_pDataMng->GetBridge()->m_dStationBridgeStt + m_pDataMng->GetBridge()->m_dLengthUganStt;
	double dExpWidthStt = bUpper ? pDeck->GetWidthBlockOut(TRUE, bUpper?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT) : 0;
	double dExpWidthEnd = bUpper ? pDeck->GetWidthBlockOut(FALSE, bUpper?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT) : 0;

	if(pBx->IsState(BX_STT_SLAB | BX_STT_JOINT))
	{
		if(bUpper)	DimDataArray.Add(CDPoint(1,dExpWidthStt));
		DimDataArray.Add(CDPoint(1,pDeck->m_dCoverSd));
	}
	if(pBx->IsState(BX_END_SLAB | BX_END_JOINT))
	{		
		if(bUpper)	DimDataArray.Add(CDPoint(1,dExpWidthEnd));		
		DimDataArray.Add(CDPoint(1,pDeck->m_dCoverSd));		
	}

	CDPointArray *pArr = &pDeck->m_arrCTCSabogangRebar[nIdx];
	for(long n=0; n<pArr->GetSize(); n++)
	{
		CDPoint ptData = pArr->GetAt(n);
		DimDataArray.Add(ptData);
	}	
}

void CDimDBDeckBasePlan::GetDimArraySpanSlab(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									 CDoubleArray &DimDataArray, __int64 nType, BOOL bNextInclude)
{
	CPlateGirderApp		*pGir = pBxStt->GetGirder();

	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBxFinder Finder(m_pDataMng->GetBridge()->GetGirder(-1));
	DimDataArray.Add(Finder.GetBxFirst(BX_STT_SLAB)->GetStation());
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	while(pBx) 
	{
		if( pBx->IsJijum() && 
			pBx->GetGirder()->GetNumberJijumByBx(pBx) != 0 &&
			pBx->GetGirder()->GetNumberJijumByBx(pBx) != m_pDataMng->GetBridge()->m_nQtyJigan)
			DimDataArray.Add(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}
	DimDataArray.Add(Finder.GetBxFirst(BX_END_SLAB)->GetStation());	
}

void CDimDBDeckBasePlan::GetDimArraySinchukRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray)
{
	CADeckData	*pDeck = m_pDataMng->GetDeckData();

	BOOL bStt = pBx->IsState(BX_STT_SLAB | BX_STT_JOINT) ? TRUE : FALSE;
	CDPointArray *pArr = &pDeck->m_arrCTCMainRebarUnderSinchuk;
	double dStaBridgeStt = m_pDataMng->GetBridge()->m_dStationBridgeStt + m_pDataMng->GetBridge()->m_dLengthUganStt;	
	DimDataArray.Add(CDPoint(1,pDeck->m_dCoverSd));		
	
	for(long n=0; n<pArr->GetSize(); n++)
	{
		CDPoint ptData = pArr->GetAt(n);
		
		DimDataArray.Add(ptData);
	}
}

void CDimDBDeckBasePlan::GetDimArrayTotal(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
											CDoubleArray &DimDataArray, __int64 nType)
{
	CPlateBridgeApp	    *pDB  = m_pDataMng->GetBridge();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();
	double dCoverSd = pDeckData->m_dCoverSd;

	CPlateGirderApp		*pGirCen = pDB->GetGirder(-1);
	double LDist = pDeckData->GetSideBarDistFromCenterGir(TRUE, TRUE);
	double RDist = pDeckData->GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dStaStt, dStaEnd;

	if(nType & DIM_DECK_TOTAL_LEFT)
	{	
		dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB), LDist, pGirCen->GetAAA(OFF_STT_SLAB));
		dStaEnd = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB), LDist, pGirCen->GetAAA(OFF_END_SLAB));
		DimDataArray.Add(0);		
		DimDataArray.Add(pGirCen->GetLengthDimType(dStaStt, dStaEnd, LDist, TRUE));
	}
	else if(nType & DIM_DECK_TOTAL_RIGHT)
	{
		dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB), RDist, pGirCen->GetAAA(OFF_STT_SLAB));
		dStaEnd = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB), RDist, pGirCen->GetAAA(OFF_END_SLAB));
		DimDataArray.Add(0);		
		DimDataArray.Add(pGirCen->GetLengthDimType(dStaStt, dStaEnd, RDist, TRUE));
	}
}

void CDimDBDeckBasePlan::DimPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,
								   __int64 nType, BOOL bLeft, BOOL bStr,  CString csTitle, 
								   long nSttDan, BOOL bSpanArrow)

{	
	if(nType & DIM_DECK_MAINREBAR_UPPER)
	{		
		CDPointArray DataArrayReal, DataArrayForDim;
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0) sTitle = "MAIN REBAR";
		GetDimArrayMainRebar(pBxStt, pBxEnd, DataArrayReal, TRUE, bLeft, TRUE);
		GetDimArrayMainRebar(pBxStt, pBxEnd, DataArrayForDim, TRUE, bLeft, FALSE);
		CDoubleArray DimDistReal = GetDistArrReal(DataArrayReal, DataArrayForDim);
		DimBasePlanEx(pDomP,pBxStt,pBxEnd,DataArrayForDim,DimDistReal,sTitle,TRUE,bLeft,nSttDan,TRUE);		
		++nSttDan;
	}	
	if(nType & DIM_DECK_MAINREBAR_LOWER)
	{	
		CDPointArray DataArrayReal, DataArrayForDim;
		CString sTitle = csTitle;
		if(bStr && sTitle.GetLength()==0) sTitle = "MAIN REBAR";
		GetDimArrayMainRebar(pBxStt, pBxEnd, DataArrayReal, FALSE, bLeft, TRUE);
		GetDimArrayMainRebar(pBxStt, pBxEnd, DataArrayForDim, FALSE, bLeft, FALSE);
		CDoubleArray DimDistReal = GetDistArrReal(DataArrayReal, DataArrayForDim);
		DimBasePlanEx(pDomP,pBxStt,pBxEnd,DataArrayForDim,DimDistReal,sTitle,FALSE,bLeft,nSttDan,TRUE);		
		++nSttDan;
	}	
	if(nType & DIM_DECK_SPAN_CENTER)
	{	
		CDoubleArray DataArray;
		CString sTitle = csTitle;		
		GetDimArraySpanSlab(pBxStt, pBxEnd, DataArray, nType, TRUE);
		if(bStr && sTitle.GetLength()==0) sTitle = "TOTAL LENGTH";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bLeft,nSttDan,TRUE);		
		nSttDan++;
	}
	if(nType & DIM_DECK_TOTAL_LEFT)
	{	
		CDoubleArray DataArray;
		CString sTitle = csTitle;		
		GetDimArrayTotal(pBxStt,pBxEnd,DataArray,nType);
		if(bStr && sTitle.GetLength()==0) sTitle = "TOTAL LENGTH";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bLeft,nSttDan,TRUE);		
		nSttDan++;
	}	
	if(nType & DIM_DECK_TOTAL_RIGHT)
	{	
		CDoubleArray DataArray;
		CString sTitle = csTitle;		
		GetDimArrayTotal(pBxStt,pBxEnd,DataArray,nType);
		if(bStr && sTitle.GetLength()==0) sTitle = "TOTAL LENGTH";
		DimBasePlan(pDomP,pBxStt,pBxEnd,DataArray,sTitle,bLeft,nSttDan,TRUE);		
		nSttDan++;
	}	
	if(nType & DIM_DECK_SKEW_STTUPPER)
	{
		CDPointArray DataArray;
		GetDimArraySabogangRebar(pBxStt, DataArray, TRUE);				
		DimBasePlanExSabogang(pDomP,pBxStt,DataArray,bLeft, TRUE);				
	}
	if(nType & DIM_DECK_SKEW_STTLOWER)
	{
		CDPointArray DataArray;
		GetDimArraySabogangRebar(pBxStt, DataArray, FALSE);				
		DimBasePlanExSabogang(pDomP,pBxStt,DataArray,bLeft, FALSE);				
	}
	if(nType & DIM_DECK_SKEW_ENDUPPER)
	{
		CDPointArray DataArray;
		GetDimArraySabogangRebar(pBxEnd, DataArray, TRUE);				
		DimBasePlanExSabogang(pDomP,pBxEnd,DataArray,bLeft, TRUE);				
	}
	if(nType & DIM_DECK_SKEW_ENDLOWER)
	{
		CDPointArray DataArray;
		GetDimArraySabogangRebar(pBxEnd, DataArray, FALSE);				
		DimBasePlanExSabogang(pDomP,pBxEnd,DataArray,bLeft, FALSE);				
	}
	if(nType & DIM_DECK_MAINREBAR_SINCHUK_STT)
	{
		CDPointArray DataArray;
		GetDimArraySinchukRebar(pBxStt, DataArray);				
		DimBasePlanExSabogang(pDomP,pBxStt,DataArray,bLeft, FALSE);				
	}	
	if(nType & DIM_DECK_MAINREBAR_SINCHUK_END)
	{
		CDPointArray DataArray;
		GetDimArraySinchukRebar(pBxEnd, DataArray);				
		DimBasePlanExSabogang(pDomP,pBxEnd,DataArray,bLeft, FALSE);				
	}
}

void CDimDBDeckBasePlan::DimBaseHDanPyung(CDomyun *pDomP, CPlateBasicIndex *pBx, CDPointArray &DimDataArray, CString szMainTitle, BOOL bLeft, long nDan)
{
	if(DimDataArray.GetSize()<1) return;

	CADeckData			*pDeckData = m_pDataMng->GetDeckData();
	CPlateBridgeApp	    *pDB  = m_pDataMng->GetBridge();
	CPlateGirderApp		*pGir = pDB->GetGirder(-1);
	CPlateBasicIndex	*pBxCen = pBx->GetBxMatchByCrossBeam(pGir);

	CDimDomyunEx *pDom = new CDimDomyunEx(m_pDataMng->GetBridge(),pDomP);
	
	long nSize  = DimDataArray.GetSize();
	bLeft ? pDom->SetDirection("LEFT") : pDom->SetDirection("RIGHT");

	CDPoint vAng    = pBx->GetAngle();
	double staCen   = pGir->GetStationBySc(pBx->GetStation(),-pBx->GetGirder()->m_dGirderCenter,pBx->GetAngle());
	double dLeft    = m_pDataMng->GetBridge()->GetDisSlabLeftDom(pBx);
	double dRight   = m_pDataMng->GetBridge()->GetDisSlabRightDom(pBx);
	double staLeft  = pGir->GetStationBySc(staCen,dLeft,vAng);
	double staRigh  = pGir->GetStationBySc(staCen,dRight,vAng);
	double staDim   = bLeft ? min(staLeft,staRigh) : max(staLeft,staRigh);
	double dLevel   = pDom->GetDimLevelDis();
	double dExo	    = pDom->GetDimExo();
	double Dist		= pDom->Always(dExo) + pDom->Always(dLevel)*nDan;
	double sw		= bLeft ? -1.0 : 1.0;
	double dCoverSd = pDeckData->m_dCoverSd;
	
	CString szTitle = COMMA(dCoverSd) + szMainTitle;
	// TITLE
	pDom->DimBaseCurvePoint(Dist*sw, dLeft);
	if(szMainTitle.IsEmpty())	
		pDom->DimCurveMoveTo(staDim,dLeft,-1);			
	else
	{	
		pDom->DimCurveMoveTo(staDim,dLeft,-1,FALSE,szTitle,0,FALSE,FALSE,TRUE);
		pDom->DimCurveLineTo(staDim,dCoverSd,"","",-1);
	}

	CString szData1, szData2;
	double dCount, dCTC;
	for(long i=0; i<nSize; i++)
	{
		dCount	= DimDataArray.GetAt(i).x;
		dCTC	= DimDataArray.GetAt(i).y;
		if(dCount==1)	
		{ 
			if((i==0 || i==nSize-1) && !szMainTitle.IsEmpty())
			{
				szData1 = "";
				szData2 = COMMA(dCTC); 
			}	
			else
			{
				szData1 = COMMA(dCTC); 
				szData2 = ""; 				
			}
		}
		else if(dCount*dCTC>2000)
		{
			szData1.Format("%.0f@%s=%s",dCount,COMMA(dCTC),COMMA(dCount*dCTC));
			szData2 = "";
		}
		else
		{
			szData1.Format("%.0f@%s",dCount,COMMA(dCTC));
			szData2.Format("=%s",COMMA(dCount*dCTC));
		}
		pDom->DimCurveLineTo(staDim,dCount*dCTC,szData1,szData2,-1,FALSE,TRUE);
	}
	if(!szMainTitle.IsEmpty())		
		pDom->DimCurveLineTo(staDim,dCoverSd,COMMA(dCoverSd),"",-1,FALSE,FALSE,TRUE);

	*pDomP << *pDom;
	delete pDom;
}

void CDimDBDeckBasePlan::GetDimArrayHDanTotal(CPlateBasicIndex *pBx, CDPointArray &DimDataArray)
{
 	CPlateBridgeApp		*pDB  = m_pDataMng->GetBridge();
	double dDisSlabLeft   = pDB->GetDisSlabLeftDom(pBx);
	double dDisSlabRight  = pDB->GetDisSlabRightDom(pBx);

	DimDataArray.Add(CDPoint(1,dDisSlabRight-dDisSlabLeft));	
}

void CDimDBDeckBasePlan::GetDimArrayHDanLowerRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray)
{
	CADeckData		*pDeck = m_pDataMng->GetDeckData();

	for(long n=0; n<pDeck->m_arrCTCBeryukRebarDan[CTC_BERYUK_LOWER].GetSize(); n++)
		DimDataArray.Add(pDeck->m_arrCTCBeryukRebarDan[CTC_BERYUK_LOWER].GetAt(n));
}

void CDimDBDeckBasePlan::GetDimArrayHDanUpperRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray)
{
	CADeckData		*pDeck = m_pDataMng->GetDeckData();
	
	for(long n=0; n<pDeck->m_arrCTCBeryukRebarDan[CTC_BERYUK_UPPER].GetSize(); n++)
		DimDataArray.Add(pDeck->m_arrCTCBeryukRebarDan[CTC_BERYUK_UPPER].GetAt(n));
}

// 횡단면도 치수선 (DOM을 기준으로.......) : 평면도
void CDimDBDeckBasePlan::DimPyungHDan(CDomyun *pDomP, CPlateBasicIndex *pBx, DWORD nType, BOOL bLeft, long nSttDan, BOOL bSlab, BOOL bOnlyUpper)
{
	long nDimDan = 0;
	if(nType & DIMDECK_BASEHDAN_LOWER)
	{
		CDPointArray DataArray;
		GetDimArrayHDanLowerRebar(pBx,DataArray);
		DimBaseHDanPyung(pDomP,pBx,DataArray,"(하면)",bLeft,nSttDan+nDimDan++);	
	}	
	if(nType & DIMDECK_BASEHDAN_UPPER)
	{
		CDPointArray DataArray;
		GetDimArrayHDanUpperRebar(pBx,DataArray);
		DimBaseHDanPyung(pDomP,pBx,DataArray,"(상면)",bLeft,nSttDan+nDimDan++);	
	}	
	if(nType & DIMDECK_BASEHDAN_TOTAL)
	{	
		CDPointArray DataArray;
		GetDimArrayHDanTotal(pBx,DataArray);
		DimBaseHDanPyung(pDomP,pBx,DataArray,"",bLeft,nSttDan+nDimDan++);	
	}	
}


// 실제 철근간격을 치수용철근간격을 이용하여 거리값으로 변환한다.
// 이유 : 평면치수에서 치수텍스트는 치수용철근간격을 이용하고 길이는 실제길이를 이용함.
CDoubleArray CDimDBDeckBasePlan::GetDistArrReal(CDPointArray &DimDataArrayReal, CDPointArray &DimDataArrayForDim)
{
	CADeckData	*pDeckData = m_pDataMng->GetDeckData();
	CDoubleArray dArrDistReal;
	CDoubleArray dArrDistReturn;
	CRebarPlacing RebarPlace;
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(DimDataArrayReal, dArrDistReal, FALSE);

	long nCount=0;
	for (long n=0; n<DimDataArrayForDim.GetSize(); n++)
	{
		CDPoint xyDim = DimDataArrayForDim[n];
		double dGuganL = 0;
		for (long nn=0; nn<xyDim.x; nn++)
		{
			if(dArrDistReal.GetSize() > nCount)
			{
				dGuganL += dArrDistReal[nCount];
				nCount++;
			}
		}
		dArrDistReturn.Add(dGuganL);
	}
	
	return dArrDistReturn;
}