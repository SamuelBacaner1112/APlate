// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DimDBDeckBaseJong.h: interface for the CDimDBDeckBaseJong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIMDBDECKBASEJONG_H__A6CF088B_03C7_4872_B963_E9B2BD545A73__INCLUDED_)
#define AFX_DIMDBDECKBASEJONG_H__A6CF088B_03C7_4872_B963_E9B2BD545A73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage; 
class AFX_EXT_CLASS CDimDBDeckBaseJong 
{
public:
	CDimDBDeckBaseJong(CDataManage *pDataMng);
	virtual ~CDimDBDeckBaseJong();
	
	CDataManage *m_pDataMng;

public:
	void DimJongTotal(CDomyun *pDomP, long nJigan, __int64 nType, long nJongBasePos, BOOL bFullDimBridge);
	void DimBaseJong(CDomyun *pDomP, long nJigan, long nJongBasePos, CDPointArray& DimDataArrayForDim, CDoubleArray& DimDistReal, CString Title, BOOL bUpper, long nDan, BOOL bFullDimBridge);
	
	void DimJongDanbuDetail(CDomyun *pDomP, BOOL bSttSlab, __int64 nType, long nJongBasePos);
	void DimBaseJongDanbuDetail(CDomyun *pDomP, BOOL bSttSlab, long nJongBasePos, CDPointArray& DimDataArrayForDim, CDoubleArray& DimDistReal, CString Title, BOOL bUpper, long nDan);

};
#endif // !defined(AFX_DIMDBDECKBASEJONG_H__A6CF088B_03C7_4872_B963_E9B2BD545A73__INCLUDED_)
