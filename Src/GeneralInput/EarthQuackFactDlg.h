// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EARTHQUACKFACTDLG_H__D66CE46D_C5EF_438D_AAED_FB770E2E80CA__INCLUDED_)
#define AFX_EARTHQUACKFACTDLG_H__D66CE46D_C5EF_438D_AAED_FB770E2E80CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EarthQuackFactDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEarthQuackFactDlg dialog

class CEarthQuackFactDlg : public CUserDialog
{
// Construction
public:
	CEarthQuackFactDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEarthQuackFactDlg)
	enum { IDD = IDD_DIALOG_EARTHFACT };
		int		m_nType;
	CString	m_dFact1;
	CString	m_dFact2;
	CString	m_dFact3;
	CString	m_dFact4;
	CString	m_dFact5;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEarthQuackFactDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEarthQuackFactDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUACKFACTDLG_H__D66CE46D_C5EF_438D_AAED_FB770E2E80CA__INCLUDED_)
