// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RangePress.h: interface for the CRangePress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGEPRESS_H__B7C52781_DE75_4AF1_9F81_D60FA1A9E844__INCLUDED_)
#define AFX_RANGEPRESS_H__B7C52781_DE75_4AF1_9F81_D60FA1A9E844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RANGE_UPPER 0
#define RANGE_LOWER 0

class AFX_EXT_CLASS CRangePressData : public CObject
{
public:
	CRangePressData();
	CRangePressData(CRangePressData *pObj);
	virtual ~CRangePressData();

public:
	BOOL m_bCompress;
	double m_dDistRange;
	
public:
	void Serialize(CArchive& ar);
	CRangePressData& operator=(const CRangePressData& obj);

	BOOL IsCompress() { return m_bCompress; }
};

class AFX_EXT_CLASS CRangePress
{
public:
	CRangePress();
	CRangePress(CRangePress *pObj);
	virtual ~CRangePress();

public:
	CTypedPtrArray <CObArray, CRangePressData *>m_ArrayUpper;
	CTypedPtrArray <CObArray, CRangePressData *>m_ArrayLower;

public:
	void Serialize(CArchive& ar);
	CRangePress& operator=(const CRangePress& obj);

	void MakeArray(long nMake, BOOL bUpper);
	BOOL IsCompress(BOOL bUpper, long nIdx) { return bUpper ? m_ArrayUpper[nIdx]->IsCompress() : m_ArrayLower[nIdx]->IsCompress(); }
	BOOL IsCompressOnBox(BOOL bUpper, CPlateBasicIndex *pBx);
};

#endif // !defined(AFX_RANGEPRESS_H__B7C52781_DE75_4AF1_9F81_D60FA1A9E844__INCLUDED_)
