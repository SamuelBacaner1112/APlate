// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHORTCUTPANESTART_H__8CFFCC61_BCFF_4214_812D_25A70ACBF9EC__INCLUDED_)
#define AFX_SHORTCUTPANESTART_H__8CFFCC61_BCFF_4214_812D_25A70ACBF9EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortcutPaneStart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneStart window

class CShortcutPaneStart : public CXTPShortcutBarPane
{
// Construction
public:
	CShortcutPaneStart();

// Attributes
public:
	CXTPTaskPanel m_wndTaskPanel;
	CImageList m_ilTaskIcons;

// Operations
public:
	void	InitMRUList();
	BOOL	Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortcutPaneStart)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShortcutPaneStart();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShortcutPaneStart)
	afx_msg LRESULT OnTaskPanelNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTCUTPANESTART_H__8CFFCC61_BCFF_4214_812D_25A70ACBF9EC__INCLUDED_)
