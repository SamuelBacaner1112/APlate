// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_GUARDFENCETYPEDLG_H__AF9FBB10_A9E5_4671_83F3_3C8A5BDA08D8__INCLUDED_)
#define AFX_GUARDFENCETYPEDLG_H__AF9FBB10_A9E5_4671_83F3_3C8A5BDA08D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuardFenceTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuardFenceTypeDlg dialog

class CGuardFenceTypeDlg : public CDialog
{
// Construction
public:
	void SetGuardFenceType(int nType);
	void DrawRebarType();
	CGuardFenceTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGuardFenceTypeDlg)
	enum { IDD = IDD_DIALOG_GUARDFENCE_TYPE };
	int		m_nRadio;
	//}}AFX_DATA
	CButton	m_btnOk;
	CButton	m_btnCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuardFenceTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuardFenceTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetType();
	void SetType(int nType);
	int m_nType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUARDFENCETYPEDLG_H__AF9FBB10_A9E5_4671_83F3_3C8A5BDA08D8__INCLUDED_)
