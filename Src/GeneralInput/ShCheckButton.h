// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHCHECKBUTTON_H__71E4C575_9C12_4CC8_B59C_2A1193506C86__INCLUDED_)
#define AFX_SHCHECKBUTTON_H__71E4C575_9C12_4CC8_B59C_2A1193506C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShCheckButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShCheckButton window
#include "resource.h"
#define WM_USER_BUTTON_PUSH	(WM_APP + 10000)

class CShCheckButton : public CButton
{
// Construction
public:
	CShCheckButton();

// Attributes 
public:
	BOOL		m_bCaptured;	
	BOOL		m_bOnOff;
	BOOL		m_bCheck;
	CBitmap		m_btnCheckOn;
	CBitmap		m_btnCheckOff;
	void SetCheck(BOOL bCheck);
	BOOL GetCheck()	{ return m_bCheck;}
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShCheckButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShCheckButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShCheckButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHCHECKBUTTON_H__71E4C575_9C12_4CC8_B59C_2A1193506C86__INCLUDED_)
