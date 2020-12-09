// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DimDBDeckBasePlan.h: interface for the CDimDBDeckBasePlan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIMDBDECKBASEPLAN_H__B5AF7FCE_FEA4_4331_BDE3_C54E19A490AC__INCLUDED_)
#define AFX_DIMDBDECKBASEPLAN_H__B5AF7FCE_FEA4_4331_BDE3_C54E19A490AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define DIMDECK_BASEHDAN_LOWER			0x000001
#define DIMDECK_BASEHDAN_UPPER			0x000002
#define DIMDECK_BASEHDAN_TOTAL			0x000004

const __int64 DIM_DECK_MAINREBAR_UPPER		= 0x0000000000000001;
const __int64 DIM_DECK_MAINREBAR_LOWER		= 0x0000000000000002;
const __int64 DIM_DECK_SPAN_CENTER			= 0x0000000000000004;

const __int64 DIM_DECK_SKEW_STTUPPER		= 0x0000000000000008;
const __int64 DIM_DECK_SKEW_ENDUPPER		= 0x0000000000000010;
const __int64 DIM_DECK_SKEW_STTLOWER		= 0x0000000000000020;
const __int64 DIM_DECK_SKEW_ENDLOWER		= 0x0000000000000040;

const __int64 DIM_DECK_MAINREBAR_SINCHUK_STT= 0x0000000000000800;
const __int64 DIM_DECK_MAINREBAR_SINCHUK_END= 0x0000000000001000;

const __int64 DIM_DECK_TOTAL_CENTER			= 0x0000000000002000;
const __int64 DIM_DECK_TOTAL_LEFT			= 0x0000000000004000;
const __int64 DIM_DECK_TOTAL_RIGHT			= 0x0000000000008000;
const __int64 DIM_DECK_TOTAL_LR				= 0x0000000000010000;

enum { MBAR_START, MBAR_MIDDLE, MBAR_END, MBAR_ALL};


class CDataManage; 
class AFX_EXT_CLASS CDimDBDeckBasePlan  
{
public:
	CDimDBDeckBasePlan(CDataManage *pDataMng);
	virtual ~CDimDBDeckBasePlan();
	
	CDataManage *m_pDataMng;
	
protected:	
	void GetDimArraySabogangRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray, BOOL bUpper);
	void GetDimArraySinchukRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray);
	void GetDimArrayHDanUpperRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray);
	void GetDimArrayHDanLowerRebar(CPlateBasicIndex *pBx, CDPointArray &DimDataArray);
	void GetDimArrayHDanTotal(CPlateBasicIndex *pBx, CDPointArray &DimDataArray);
	void DimBaseHDanPyung(CDomyun *pDomP, CPlateBasicIndex *pBx, CDPointArray &DimDataArray, CString szMainTitle, BOOL bLeft, long nDan);
	void DimBasePlan(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, CString Title, BOOL bLeft, long nDan, BOOL bSpanArrow);
	void DimBasePlanEx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDPointArray &DimDataArray, CDoubleArray &DimRealDist, CString Title, BOOL bUpper, BOOL bLeft, long nDan, BOOL bSpanArrow=FALSE);
	void DimBasePlanExSabogang(CDomyun *pDomP, CPlateBasicIndex *pBx, CDPointArray &DimDataArray, BOOL bLeft, BOOL bUpper);

public:
	void GetDimArrayMainRebar(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDPointArray &DimDataArray,BOOL bUpper, BOOL bLeft, BOOL bRealDim);
	void GetDimArraySpanSlab(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, __int64 nType, BOOL bNextInclude);
	void GetDimArrayTotal(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, __int64 nType);

	void DimPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 nType, BOOL bLeft, BOOL bStr, CString csTitle, long nSttDan=0, BOOL bSpanArrow=FALSE);
	void DimPyungHDan(CDomyun *pDomP, CPlateBasicIndex *pBx, DWORD nType, BOOL bLeft, long nSttDan=0, BOOL bSlab=TRUE, BOOL bOnlyUpper=FALSE);	
	CDoubleArray GetDistArrReal(CDPointArray &DimDataArrayReal, CDPointArray &DimDataArrayForDim);
};

#endif // !defined(AFX_DIMDBDECKBASEPLAN_H__B5AF7FCE_FEA4_4331_BDE3_C54E19A490AC__INCLUDED_)
