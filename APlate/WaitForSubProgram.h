// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_WAITFORSUBPROGRAM_H__869E2DE2_FE2C_470C_90FB_DFE7DC1F11A7__INCLUDED_)
#define AFX_WAITFORSUBPROGRAM_H__869E2DE2_FE2C_470C_90FB_DFE7DC1F11A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitForSubProgram.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaitForSubProgram dialog

#include "resource.h"

class CWaitForSubProgram : public CDialog
{
// Construction
public:
	CWaitForSubProgram(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaitForSubProgram)
	enum { IDD = IDD_DIALOG_WAITFORALINE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitForSubProgram)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
		BOOL m_bCancel;
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitForSubProgram)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITFORSUBPROGRAM_H__869E2DE2_FE2C_470C_90FB_DFE7DC1F11A7__INCLUDED_)
