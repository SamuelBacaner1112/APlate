// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DIALOGINSTALLBERYUKATBLOCKOUT_H__BC916834_AFFC_407A_AA10_ABF65BAFD8DF__INCLUDED_)
#define AFX_DIALOGINSTALLBERYUKATBLOCKOUT_H__BC916834_AFFC_407A_AA10_ABF65BAFD8DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogInstallBeryukAtBlockout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInstallBeryukAtBlockout dialog
#include "Resource.h"
class CDialogInstallBeryukAtBlockout : public CUserDialog
{
// Construction
public:
	CDialogInstallBeryukAtBlockout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogInstallBeryukAtBlockout)
	enum { IDD = IDD_DIALOG_INSTALL_BERYUK };
	int		m_nInstallBeryukAtBlockout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogInstallBeryukAtBlockout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogInstallBeryukAtBlockout)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGINSTALLBERYUKATBLOCKOUT_H__BC916834_AFFC_407A_AA10_ABF65BAFD8DF__INCLUDED_)
