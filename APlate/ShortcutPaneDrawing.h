// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHORTCUTPANEDRAWING_H__BA84C8E4_7162_49A7_8126_2375984B335D__INCLUDED_)
#define AFX_SHORTCUTPANEDRAWING_H__BA84C8E4_7162_49A7_8126_2375984B335D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortcutPaneDrawing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneDrawing window

class CShortcutPaneDrawing : public CXTPShortcutBarPane
{
// Construction
public:
	CShortcutPaneDrawing();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortcutPaneDrawing)
	//}}AFX_VIRTUAL

// Implementation
public:
	CXTTreeCtrl m_wndTreeDrawing;
	CImageList m_ImageList;

	void InitTree();
	void SetItemPositionUpdate(long nCurPage);
	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);
	virtual ~CShortcutPaneDrawing();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShortcutPaneDrawing)
	afx_msg void OnSelchangedTreeDrawing(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTCUTPANEDRAWING_H__BA84C8E4_7162_49A7_8126_2375984B335D__INCLUDED_)
