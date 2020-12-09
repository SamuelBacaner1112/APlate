// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_PAGEVIEWBAR_H__553C8C80_A447_4FFD_AC19_039791FD610B__INCLUDED_)
#define AFX_PAGEVIEWBAR_H__553C8C80_A447_4FFD_AC19_039791FD610B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageViewBar.h : header file
//
#include "XTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPageViewBar dialog
class CAPlateDrawStd;
class CPageViewBar : public CControlBar
{
// Construction
public:
	CPageViewBar();   // standard constructor
	~CPageViewBar();

// Operations
public:
	virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

public:
	CXTabCtrl m_wndTab;
	CFont *m_TabFont;
	CAPlateDrawStd *m_pDrawStd;	
	CImageList m_ImageList;
	BOOL m_bMultiLine;
		
	void SetModelMode(BOOL bModel) { m_bModelMode = bModel; }
	void SetMultiLine(BOOL bMultiLine = TRUE);

private:
	BOOL m_bModelMode;	
	
// Implementation
protected:	
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	// Generated message map functions
	//{{AFX_MSG(CPageViewBar)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);	
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEVIEWBAR_H__553C8C80_A447_4FFD_AC19_039791FD610B__INCLUDED_)
