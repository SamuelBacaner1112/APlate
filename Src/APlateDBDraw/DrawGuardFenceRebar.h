// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawGuardFenceRebar.h: interface for the CDrawGuardFenceRebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWGUARDFENCEREBAR_H__A9F3856E_1ABE_4265_9512_FE862C604DE9__INCLUDED_)
#define AFX_DRAWGUARDFENCEREBAR_H__A9F3856E_1ABE_4265_9512_FE862C604DE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;

class AFX_EXT_CLASS CDrawGuardFenceRebar
{
public:
	CDrawGuardFenceRebar(CDataManage *pDataMng);
	virtual ~CDrawGuardFenceRebar();

	CDataManage *m_pDataMng;

public:
	void DrawGroundLine(CDomyun* pDom, int nGuard);
	void MarkMainRebarType7(CDomyun* pDom, int nGuard);
	void MarkMainRebarType6(CDomyun* pDom, int nGuard);
	void MarkMainRebarType5(CDomyun* pDom, int nGuard);
	void MarkMainRebarType4(CDomyun* pDom, int nGuard);
	void MarkMainRebarType3(CDomyun* pDom, int nGuard);
	void MarkMainRebarType2(CDomyun* pDom, int nGuard);
	void MarkMainRebarType1(CDomyun* pDom, int nGuard);
	void DimShearRebar(CDomyun* pDom, int nGuard);
	void DimDistRebar(CDomyun* pDom, int nGuard);
	void DimMainRebar(CDomyun* pDom, int nGuard);
	void MarkShearRebar(CDomyun* pDom, int nGuard);
	void MarkDistRebar(CDomyun* pDom, int nGuard);
	void MarkMainRebar(CDomyun* pDom, int nGuard);
	void MarkRebar(CDomyun* pDom, int nGuard);
	void DrawDistRebar(CDomyun* pDom, CDPointArray ptArrPos);
	void DimRebar(CDomyun* pDom, int nGuard);
	void DrawShearRebar(CDomyun* pDom, int nGuard);
	void DrawDistRebar(CDomyun* pDom, int nGuard);
	void DrawMainRebar(CDomyun* pDom, int nGuard);
	void DrawRebar(CDomyun* pDom, int nGuard);

};

#endif // !defined(AFX_DRAWGUARDFENCEREBAR_H__A9F3856E_1ABE_4265_9512_FE862C604DE9__INCLUDED_)
