// DimDBDeckBaseJong.cpp: implementation of the CDimDBDeckBaseJong class.
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

CDimDBDeckBaseJong::CDimDBDeckBaseJong(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CDimDBDeckBaseJong::~CDimDBDeckBaseJong()
{

}

void CDimDBDeckBaseJong::DimJongTotal(CDomyun *pDomP, long nJigan, __int64 nType, long nJongBasePos, BOOL bFullDimBridge)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CDimDBDeckBasePlan  DimPlan(m_pDataMng);

	if(nType & DIM_DECK_MAINREBAR_UPPER)
	{		
		CDPointArray DataArrayReal, DataArrayForDim;
		CString sTitle = "MAIN REBAR";
		DataArrayReal   = pDeckData->GetDimTotalAtJongBasePos(TRUE, nJongBasePos, TRUE);
		DataArrayForDim = pDeckData->GetDimTotalAtJongBasePos(TRUE, nJongBasePos, FALSE);
		CDoubleArray DimDistReal = DimPlan.GetDistArrReal(DataArrayReal, DataArrayForDim);
		DimBaseJong(pDomP, nJigan, nJongBasePos, DataArrayForDim, DimDistReal, sTitle, TRUE, 1, bFullDimBridge);		
	}	
	if(nType & DIM_DECK_MAINREBAR_LOWER)
	{		
		CDPointArray DataArrayReal, DataArrayForDim;
		CString sTitle = "MAIN REBAR";
		DataArrayReal   = pDeckData->GetDimTotalAtJongBasePos(FALSE, nJongBasePos, TRUE);
		DataArrayForDim = pDeckData->GetDimTotalAtJongBasePos(FALSE, nJongBasePos, FALSE);
		CDoubleArray DimDistReal = DimPlan.GetDistArrReal(DataArrayReal, DataArrayForDim);
		DimBaseJong(pDomP, nJigan, nJongBasePos, DataArrayForDim, DimDistReal, sTitle, FALSE, 1, bFullDimBridge);		
	}	
}


void CDimDBDeckBaseJong::DimJongDanbuDetail(CDomyun *pDomP, BOOL bSttSlab, __int64 nType, long nJongBasePos)
{
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CDimDBDeckBasePlan  DimPlan(m_pDataMng);

	if(nType & DIM_DECK_MAINREBAR_UPPER)
	{		
		CDPointArray DataArrayReal, DataArrayForDim;
		CString sTitle = "MAIN REBAR";
		DataArrayReal   = pDeckData->GetDimTotalAtJongBasePos(TRUE, nJongBasePos, TRUE);
		DataArrayForDim = pDeckData->GetDimTotalAtJongBasePos(TRUE, nJongBasePos, FALSE);
		CDoubleArray DimDistReal = DimPlan.GetDistArrReal(DataArrayReal, DataArrayForDim);
		DimBaseJongDanbuDetail(pDomP, bSttSlab, nJongBasePos, DataArrayForDim, DimDistReal, sTitle, TRUE, 1);		
	}	
	if(nType & DIM_DECK_MAINREBAR_LOWER)
	{		
		CDPointArray DataArrayReal, DataArrayForDim;
		CString sTitle = "MAIN REBAR";
		DataArrayReal   = pDeckData->GetDimTotalAtJongBasePos(FALSE, nJongBasePos, TRUE);
		DataArrayForDim = pDeckData->GetDimTotalAtJongBasePos(FALSE, nJongBasePos, FALSE);
		CDoubleArray DimDistReal = DimPlan.GetDistArrReal(DataArrayReal, DataArrayForDim);
		DimBaseJongDanbuDetail(pDomP, bSttSlab, nJongBasePos, DataArrayForDim, DimDistReal, sTitle, FALSE, 1);		
	}	
}



void CDimDBDeckBaseJong::DimBaseJongDanbuDetail(CDomyun *pDomP, BOOL bSttSlab, long nJongBasePos, CDPointArray& DimDataArrayForDim, CDoubleArray& DimDistReal, CString Title, BOOL bUpper, long nDan)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();
	CDimDBDeckBasePlan  DimPlan(m_pDataMng);

	CDomyun Dom(pDomP);

	double dPosStt = 0, dPosEnd = 0;
	double dExtL	  = 2000;
	double dSlabTotal = pDeckData->GetLengthSlabTotal(nJongBasePos);

	if(bSttSlab)
	{
		dPosStt = 0;
		dPosEnd = pDeckData->GetLengthSlabToTaper(bSttSlab, nJongBasePos) + pDB->m_dLengthTaperToTaperStt + dExtL;
	}
	else
	{
		dPosStt = dSlabTotal - pDeckData->GetLengthSlabToTaper(bSttSlab, nJongBasePos) - pDB->m_dLengthTaperToTaperEnd - dExtL;
		dPosEnd = dSlabTotal;
	}

	// 해당지간의 치수와 치수선 길이를 계산
	// 치수범위의시종점중 하나라도 지간범위에 있으면 치수가 지간에 있는걸로 간주함.
	CDPointArray DimArrAtJigan;
	double dSum    = 0;
	double dSttDim = 0;
	double dEndDim = 0;
	double dGuganL = 0;
	long   n = 0;
	for (n=0; n<DimDataArrayForDim.GetSize(); n++)
	{
		CDPoint xyDim = DimDataArrayForDim[n];
		dGuganL = (xyDim.x*xyDim.y);
		if(dGuganL<=0)	continue;
		
		dSttDim = dSum;
		dSum += dGuganL;
		dEndDim = dSum;
		if( (dPosStt <= dSttDim && dSttDim <= dPosEnd) || (dPosStt <= dEndDim && dEndDim <= dPosEnd) )
			DimArrAtJigan.Add(xyDim);
	}

	BOOL bIncludePosStt = FALSE;
	BOOL bIncludePosEnd = FALSE;
	CDoubleArray dArrDistAccumTot, dArrDistAccumJigan;
	
	dArrDistAccumTot.Add(0);
	dSum = 0;
	for ( n=0; n<DimDistReal.GetSize(); n++)	// 간격
	{
		if(DimDistReal[n] <= 0)		continue;
		dSum += DimDistReal[n];
		dArrDistAccumTot.Add(dSum);
	}
	
	for ( n=0; n<dArrDistAccumTot.GetSize(); n++)
	{
		if(dPosStt <= dArrDistAccumTot[n] && dArrDistAccumTot[n] <= dPosEnd) 
		{
			dArrDistAccumJigan.Add(dArrDistAccumTot[n]);
			if(dPosStt==dArrDistAccumTot[n])		bIncludePosStt = TRUE;
			if(dPosEnd==dArrDistAccumTot[n])		bIncludePosEnd = TRUE;
		}
	}
	if(bIncludePosStt==FALSE)		dArrDistAccumJigan.Add(dPosStt);
	if(bIncludePosEnd==FALSE)		dArrDistAccumJigan.Add(dPosEnd);

	dArrDistAccumJigan.Sort();
	
	CDoubleArray dArrGuganW;
	double dSumGugan = 0;
	for (n=0; n<dArrDistAccumJigan.GetSize()-1; n++)
	{
		double dDis = dArrDistAccumJigan[n+1]-dArrDistAccumJigan[n];
		dArrGuganW.Add(dDis);
		dSumGugan+=dDis;
	}

	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString strUp, strDn, sDir = bUpper ? _T("TOP") : _T("BOTTOM");
	CHgBaseDrawStd baseDraw;

	for ( n=0; n<DimArrAtJigan.GetSize(); n++)
	{
		long nCount = (long)DimArrAtJigan[n].x;
		double dLen = DimArrAtJigan[n].y;
		if(nCount > 1)
		{
			strUp.Format("%d@%s=%s", nCount, COMMA(dLen), COMMA(dLen*nCount));
			strDn = "";
			if(Dom.GetTextWidth(strUp) > dLen*nCount)
			{
				strUp.Format("%d@%s", nCount, COMMA(dLen));
				strDn.Format("=%s", COMMA(dLen*nCount));
			}
		}
		else
		{
			strUp = COMMA(dLen);
			strDn = "";
		}

		sArrText.Add(strUp);
		sArrUnderText.Add(strDn);
	}
	
	baseDraw.AutoDimCont(&Dom, CDPoint(dPosStt, bUpper?0:-pDB->m_dThickSlabBase-pDB->m_dHeightTaperStt), dArrGuganW, sArrText, sArrUnderText, nDan, sDir, -1, FALSE, FALSE, FALSE);

	CDoubleArray dArrTemp;
	dArrTemp.Add(dSumGugan);
	sArrText.RemoveAll();
	sArrUnderText.RemoveAll();
	sArrText.Add(COMMA(dSlabTotal));
	baseDraw.AutoDimCont(&Dom, CDPoint(dPosStt, bUpper?0:-pDB->m_dThickSlabBase-pDB->m_dHeightTaperStt), dArrTemp, sArrText, sArrUnderText, nDan+1, sDir, -1, FALSE, FALSE, FALSE);

	*pDomP << Dom;
}



void CDimDBDeckBaseJong::DimBaseJong(CDomyun *pDomP, long nJigan, long nJongBasePos, CDPointArray& DimDataArrayForDim, CDoubleArray& DimDistReal, CString Title, BOOL bUpper, long nDan, BOOL bFullDimBridge)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CADeckData		*pDeckData	= m_pDataMng->GetDeckData();

	CDomyun Dom(pDomP);

	double dPosStt = 0, dPosEnd = 0;
	pDeckData->GetPosJiganAtJong(nJigan, dPosStt, dPosEnd, nJongBasePos);

	if(bFullDimBridge)
	{
		double dPosTemp = 0;
		pDeckData->GetPosJiganAtJong(0, dPosStt, dPosTemp, nJongBasePos);
		pDeckData->GetPosJiganAtJong(pDB->m_nQtyJigan-1, dPosTemp, dPosEnd, nJongBasePos);
	}
	double dSlabTotal = pDeckData->GetLengthSlabTotal(nJongBasePos);

	// 해당지간의 치수와 치수선 길이를 계산
	// 치수범위의시종점중 하나라도 지간범위에 있으면 치수가 지간에 있는걸로 간주함.
	CDPointArray DimArrAtJigan;
	double dSum    = 0;
	double dSttDim = 0;
	double dEndDim = 0;
	double dGuganL = 0;
	double dMaxGuganL = 0;
	CDPoint xyDimMax;
	long n = 0;
	for (n=0; n<DimDataArrayForDim.GetSize(); n++)
	{
		CDPoint xyDim = DimDataArrayForDim[n];
		dGuganL = (xyDim.x*xyDim.y);
		if(dMaxGuganL<dGuganL)
		{
			dMaxGuganL = dGuganL;
			xyDimMax = xyDim;
		}
		if(dGuganL<=0)	continue;
		
		dSttDim = dSum;
		dSum += dGuganL;
		dEndDim = dSum;
		if( (dPosStt <= dSttDim && dSttDim <= dPosEnd) || (dPosStt <= dEndDim && dEndDim <= dPosEnd) )
			DimArrAtJigan.Add(xyDim);
	}

	if(DimArrAtJigan.GetSize()==0)
		DimArrAtJigan.Add(xyDimMax);
	
	BOOL bIncludePosStt = FALSE;
	BOOL bIncludePosEnd = FALSE;
	CDoubleArray dArrDistAccumTot, dArrDistAccumJigan;
	
	dArrDistAccumTot.Add(0);
	dSum = 0;
	for ( n=0; n<DimDistReal.GetSize(); n++)	// 간격
	{
		if(DimDistReal[n] <= 0)		continue;
		dSum += DimDistReal[n];
		dArrDistAccumTot.Add(dSum);
	}
	
	for ( n=0; n<dArrDistAccumTot.GetSize(); n++)
	{
		if(dPosStt <= dArrDistAccumTot[n] && dArrDistAccumTot[n] <= dPosEnd) 
		{
			dArrDistAccumJigan.Add(dArrDistAccumTot[n]);
			if(dPosStt==dArrDistAccumTot[n])		bIncludePosStt = TRUE;
			if(dPosEnd==dArrDistAccumTot[n])		bIncludePosEnd = TRUE;
		}
	}
	if(bIncludePosStt==FALSE)		dArrDistAccumJigan.Add(dPosStt);
	if(bIncludePosEnd==FALSE)		dArrDistAccumJigan.Add(dPosEnd);

	dArrDistAccumJigan.Sort();
	
	CDoubleArray dArrGuganW;
	double dSumGugan = 0;
	for (n=0; n<dArrDistAccumJigan.GetSize()-1; n++)
	{
		double dDis = dArrDistAccumJigan[n+1]-dArrDistAccumJigan[n];
		dArrGuganW.Add(dDis);
		dSumGugan+=dDis;
	}

	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString strUp, strDn, sDir = bUpper ? _T("TOP") : _T("BOTTOM");
	CHgBaseDrawStd baseDraw;

	for ( n=0; n<DimArrAtJigan.GetSize(); n++)
	{
		CDPoint xyJigan = DimArrAtJigan[n];
		long nCount = (long)xyJigan.x;
		double dLen = xyJigan.y;
		if(nCount > 1)
		{
			strUp.Format("%d@%s=%s", nCount, COMMA(dLen), COMMA(dLen*nCount));
			strDn = "";
			if(Dom.GetTextWidth(strUp) > dLen*nCount)
			{
				strUp.Format("%d@%s", nCount, COMMA(dLen));
				strDn.Format("=%s", COMMA(dLen*nCount));
			}
			if(Dom.GetTextWidth(strUp) > dLen*nCount)
			{
				strUp = " ";
				strDn.Format("%d@%s=%s", nCount, COMMA(dLen), COMMA(dLen*nCount));			
			}
		}
		else
		{
			if(Dom.GetTextWidth(strUp) > dLen && strUp!=" ")
			{
				strUp = " ";
				strDn = COMMA(dLen);				
			}
			else
			{
				strUp = COMMA(dLen);
				strDn = "";
			}			
		}

		sArrText.Add(strUp);
		sArrUnderText.Add(strDn);
	}
	
	baseDraw.AutoDimCont(&Dom, CDPoint(dPosStt, bUpper?0:-pDB->m_dThickSlabBase-pDB->m_dHeightTaperStt), dArrGuganW, sArrText, sArrUnderText, nDan, sDir, -1, FALSE, FALSE, FALSE);

	CDoubleArray dArrTemp;
	dArrTemp.Add(dSumGugan);
	sArrText.RemoveAll();
	sArrUnderText.RemoveAll();
	sArrText.Add(COMMA(dSlabTotal));
	baseDraw.AutoDimCont(&Dom, CDPoint(dPosStt, bUpper?0:-pDB->m_dThickSlabBase-pDB->m_dHeightTaperStt), dArrTemp, sArrText, sArrUnderText, nDan+1, sDir, -1, FALSE, FALSE, FALSE);

	*pDomP << Dom;
}




