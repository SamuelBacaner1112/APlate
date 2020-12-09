// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_JOINTRESTRAINTDLG_H__82681FD8_C604_4438_9760_D8E963296CED__INCLUDED_)
#define AFX_JOINTRESTRAINTDLG_H__82681FD8_C604_4438_9760_D8E963296CED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JointRestraintDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJointRestraintDlg dialog

class CJointRestraintDlg : public CDialog
{
// Construction
public:
	CJointRestraintDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJointRestraintDlg)
	enum { IDD = IDD_DIALOG_JOINTRESTRAINT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGridCtrl m_Grid;
	CGeneralBaseStd	*m_pStd;
	CButton	m_btnOk;

protected:
	void SetGridTitle();
	void SetDataInit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJointRestraintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJointRestraintDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOINTRESTRAINTDLG_H__82681FD8_C604_4438_9760_D8E963296CED__INCLUDED_)
