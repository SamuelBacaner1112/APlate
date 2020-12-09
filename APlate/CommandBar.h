// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// CommandBar.h: interface for the CCommandBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDBAR_H__3812F843_A074_4689_837F_2889093F7D06__INCLUDED_)
#define AFX_COMMANDBAR_H__3812F843_A074_4689_837F_2889093F7D06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdEdit.h"
#include "HisEdit.h"

class CCommandBar : public CShSizingControlBar  
{
public:
	CCommandBar();	

public:
	CHisEdit m_HisEdit;
	CCmdEdit m_CmdEdit;
	CScrollBar m_ScrollBar;
	CFont m_Font;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommandBar();
	void SetChar(UINT nChar);

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_COMMANDBAR_H__3812F843_A074_4689_837F_2889093F7D06__INCLUDED_)
