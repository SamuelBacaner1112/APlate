// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawDimJong.h: interface for the CAPlateDrawDimJong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWDIMJONG_H__F3893659_BB62_4BD0_AF8E_DBC7F4527E04__INCLUDED_)
#define AFX_APLATEDRAWDIMJONG_H__F3893659_BB62_4BD0_AF8E_DBC7F4527E04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPlateBasicIndex;
class AFX_EXT_CLASS CAPlateDrawDimJong  
{
public:
	CAPlateDrawDimJong(CDataManage *pDataMng);
	virtual ~CAPlateDrawDimJong();

private:
	CDataManage *m_pDataMng;

public:
	long DimJongTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 nType, BOOL bUppeer, BOOL str, long nDimDan);

	void DimByunDanmyunJong(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nTab, long nType);
	void DimBaseJong(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &DimDataArray, CString Title, BOOL bUpper, long nDan);
	void DimBaseJongEx(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDPointArray &DimDataArray, CString Title, BOOL bUpper, long nDan);

	void DimDanmyunCheckMark(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void GetDanmyunCheckStation(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CDoubleArray &dStaArr);
};

#endif // !defined(AFX_APLATEDRAWDIMJONG_H__F3893659_BB62_4BD0_AF8E_DBC7F4527E04__INCLUDED_)
