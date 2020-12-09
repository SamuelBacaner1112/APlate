// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DIALOGINSTALLMAINATBLOCKOUT_H__268812E7_770D_40C7_9E16_C51349EDCDBD__INCLUDED_)
#define AFX_DIALOGINSTALLMAINATBLOCKOUT_H__268812E7_770D_40C7_9E16_C51349EDCDBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogInstallMainAtBlockout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInstallMainAtBlockout dialog
#include "Resource.h" 
class AFX_EXT_CLASS CDialogInstallMainAtBlockout : public CUserDialog
{
// Construction
public:
	CDialogInstallMainAtBlockout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogInstallMainAtBlockout)
	enum { IDD = IDD_DIALOG_INSTALL_MAIN };
	int		m_nInstallMainAtBlockout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogInstallMainAtBlockout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogInstallMainAtBlockout)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGINSTALLMAINATBLOCKOUT_H__268812E7_770D_40C7_9E16_C51349EDCDBD__INCLUDED_)
