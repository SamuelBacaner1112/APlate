// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawSpacer.h: interface for the CDrawSpacer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWSPACER_H__B469E58E_A220_4DF3_86E1_033425AEB001__INCLUDED_)
#define AFX_DRAWSPACER_H__B469E58E_A220_4DF3_86E1_033425AEB001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;

class AFX_EXT_CLASS CDrawSpacer  
{
public:
	CDrawSpacer(CDataManage *pDataMng);
	virtual ~CDrawSpacer();

public:
	CDataManage *m_pDataMng;

public:
	void DimPlaceRebar(CDomyun* pDom);
	CDPointArray GetPosPlanSpacer();
	void DrawPlanRebarEx(CDomyun* pDom, CDPoint ptCen);
	void DrawGridRebar(CDomyun* pDom);
	void DrawPlaceRebar(CDomyun* pDom);
	void MarkRebar(CDomyun* pDom);
	void	DimPlanRebar(CDomyun* pDom, BOOL bInput = FALSE);
	void	DimFrontRebar(CDomyun* pDom, BOOL bInput = FALSE);
	void	DrawPlanRebar(CDomyun* pDom);
	void	DrawFrontRebar(CDomyun* pDom);
};

#endif // !defined(AFX_DRAWSPACER_H__B469E58E_A220_4DF3_86E1_033425AEB001__INCLUDED_)
