// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawDBBaseRebarJong.h: interface for the CDrawDBBaseRebarJong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDBBASEREBARJONG_H__651F4F79_0E35_4FA2_8DE2_0C3E5F2E32EA__INCLUDED_)
#define AFX_DRAWDBBASEREBARJONG_H__651F4F79_0E35_4FA2_8DE2_0C3E5F2E32EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage; 
class AFX_EXT_CLASS CDrawDBBaseRebarJong  
{
public:
	CDrawDBBaseRebarJong(CDataManage *pDataMng);
	virtual ~CDrawDBBaseRebarJong();

	CDataManage *m_pDataMng;

public:
	// Ã¶±Ù
	void DrawMainRebarAtJong(CDomyun *pDom, long nJongBasePos);
	void DrawShearRebarAtJong(CDomyun *pDom, long nJongBasePos);
	void DrawBeryukRebarAtJong(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput);	
	void DrawBeryukRebarUp(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput);
	void DrawBeryukRebarDn(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput);
	void DrawJDanSlab(CDomyun *pDom, long nJongBasePos);
	
	// ¹è·ÂÃ¶±Ù Ä¡¼ö
	void DimBeryukRebarAtJong(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput, double dTextRatio=1.0);
	void DimBeryukRebarAtJongUp(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput, double dTextRatio=1.0);
	void DimBeryukRebarAtJongDn(CDomyun *pDom, long nJongBasePos, long nCycle, BOOL bInput, double dTextRatio=1.0);
	void DimBeryukRebarAtJongVariable(CDomyun *pDom, long nCycle, BOOL bInput, double dTextRatio=1.0);
	// ÁÖÃ¶±Ù Ä¡¼ö
	void DimMainRebarAtJong(CDomyun *pDom, long nJongBasePos, BOOL bUpper);
	void DimMainRebarAtJongSd(CDomyun *pDom, long nJongBasePos, BOOL bSttSlab, BOOL bIsSlabCommon=FALSE, BOOL bIsText=FALSE, BOOL bDimTop=FALSE);
	// ¸¶Å©
	void DrawMarkMainRebarAtJong(CDomyun *pDom, long nJigan, long nJongBasePos);
	void DrawMarkBeryukRebarAtJong(CDomyun *pDom, long nCycle, long nJigan, long nJongBasePos);
	void DrawMarkBeryukRebarAtJongTotal(CDomyun *pDom, long nCycle, long nJongBasePos);
	void DrawMarkShearRebar(CDomyun *pDom, long nJigan, long nJongBasePos);

};

#endif // !defined(AFX_DRAWDBBASEREBARJONG_H__651F4F79_0E35_4FA2_8DE2_0C3E5F2E32EA__INCLUDED_)
