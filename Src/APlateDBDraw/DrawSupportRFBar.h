// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawSupportRFBar.h: interface for the CDrawSupportRFBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWSUPPORTRFBAR_H__D0AF3C87_89D3_4D94_A221_19F469AB4EEB__INCLUDED_)
#define AFX_DRAWSUPPORTRFBAR_H__D0AF3C87_89D3_4D94_A221_19F469AB4EEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;

class AFX_EXT_CLASS CDrawSupportRFBar  
{
public:
	CDrawSupportRFBar(CDataManage *pDataMng);
	virtual ~CDrawSupportRFBar();

public:
	CDataManage *m_pDataMng;

public:
	void	DrawDetailCircleText(CDomyun* pDom, int nType, CString strText);
	void	DrawDetailCircle(CDomyun* pDom, int nType);
	void	DrawDetail(CDomyun* pDOm, int nType, double dDis = 0);
	void	DrawRebar(CDomyun* pDom, int nType);
	void	DimRebar(CDomyun* pDom, int nType);
	void	MarkRebar(CDomyun* pDom, int nType);
};

#endif // !defined(AFX_DRAWSUPPORTRFBAR_H__D0AF3C87_89D3_4D94_A221_19F469AB4EEB__INCLUDED_)
