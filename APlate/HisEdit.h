// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_HISEDIT_H__761EF4C2_7BBB_40D8_BFB4_0D9E19125477__INCLUDED_)
#define AFX_HISEDIT_H__761EF4C2_7BBB_40D8_BFB4_0D9E19125477__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HisEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHisEdit window

class CHisEdit : public CEdit
{
// Construction
public:
	CHisEdit();

// Attributes
public:
	HWND m_hCmdWnd;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHisEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHisEdit();

	// Generated message map functions
public:
	void ScrollAtEnd();

protected:
	//{{AFX_MSG(CHisEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISEDIT_H__761EF4C2_7BBB_40D8_BFB4_0D9E19125477__INCLUDED_)
