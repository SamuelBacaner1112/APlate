// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_TOTALVIEWDLG_H__57933A76_400C_4297_A6EF_775B500FAF35__INCLUDED_)
#define AFX_TOTALVIEWDLG_H__57933A76_400C_4297_A6EF_775B500FAF35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TotalViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTotalViewDlg dialog
class CDomyunView;
#define WM_GET_NEARBX	WM_USER+277
#include "resource.h"

class CTotalViewDlg : public CDialog
{
// Construction
public:
	CTotalViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTotalViewDlg)
	enum { IDD = IDD_DIALOG_TOTALVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CGeneralBaseStd *m_pStd;
	CDomyunView m_DomView;
	__int64 m_nFlag;
	CPoint		m_ptClick;
	long		m_nClickCircleRadius;
	BOOL		m_bClickCircle;
	void Clear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTotalViewDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTotalViewDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();		
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOTALVIEWDLG_H__57933A76_400C_4297_A6EF_775B500FAF35__INCLUDED_)
