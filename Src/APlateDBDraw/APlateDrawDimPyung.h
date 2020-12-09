// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawDimPyung.h: interface for the CAPlateDrawDimPyung class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWDIMPYUNG_H__69F62C33_B037_43EE_898F_CCE7A56E48BF__INCLUDED_)
#define AFX_APLATEDRAWDIMPYUNG_H__69F62C33_B037_43EE_898F_CCE7A56E48BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPlateBasicIndex;
class AFX_EXT_CLASS CAPlateDrawDimPyung  
{
public:
	CAPlateDrawDimPyung(CDataManage *pDataMng);
	virtual ~CAPlateDrawDimPyung();

private:
	CDataManage *m_pDataMng; 

public:
	long DimPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,
					   __int64 nType, BOOL bUpper, BOOL bStr,  CString csTitle, 
					   long nSttDan=0, BOOL bSpanArrow=FALSE);
	//
	void DrawDimHDan(CDomyun *pDom,  CPlateBasicIndex *pBx, BOOL bLeft, BOOL bSeperate = TRUE, long nType=1, BOOL bDimGirType=TRUE);
	void DrawDimSttEnd(CDomyun *pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper);

	// ½´ÁÂÇ¥ °ü·Ã
	void DimHDanShoeDist(CDomyun *pDomP, long nJijum, long nDimDan=1);

public:
	void GetDimArray(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
			         CDoubleArray &DimDataArray, __int64 nType, BOOL bUpper, BOOL bNextInclude=FALSE);
	void DimBasePlan(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
					 CDoubleArray &DimDataArray, CString Title, 
					 BOOL bUpper, long nDan, BOOL bSpanArrow, long nUsingGirder=-1);
	void DimBasePlanEx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDPointArray &DimDataArray, CString Title, BOOL bUpper, long nDan);
	//
	void GetDimArrayVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDPointArray &DimDataArray, __int64 nType);
	//
	void GetDimArrayTotal(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, __int64 nType);
	void GetDimArrayFactJoin(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, long nPos, __int64 nType, BOOL bIncludeSplice);
	void GetDimArraySplice(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, __int64 nType);
	void GetDimArrayCrossBeam(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, __int64 nType);
	void GetDimArrayTendon(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, __int64 nType, BOOL bNextInclude);
	void GetDimArraySpan(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, __int64 nType, BOOL bNextInclude);
	//	
	void GetDimHdanPoint(CPlateBasicIndex *pBx, double dx[], long nType);
	//
	void DrawJijumStiffDim(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDan);
	void DrawJackupStiffDim(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nDan);
	void DimXyCenter(CDomyun *pDomP);


};

#endif // !defined(AFX_APLATEDRAWDIMPYUNG_H__69F62C33_B037_43EE_898F_CCE7A56E48BF__INCLUDED_)
