// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_GIRHEIGHTUSERDLG_H__C3905184_C77B_46D4_9FF5_B6F9CE55791C__INCLUDED_)
#define AFX_GIRHEIGHTUSERDLG_H__C3905184_C77B_46D4_9FF5_B6F9CE55791C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GirHeightUserDlg.h : header file
//


const CString GH_STR_UILI = _T("상판내측, 하판내측 (기본타입)");
const CString GH_STR_UOLO = _T("상판외측, 하판외측 (소수주거더교타입)");
const CString GH_STR_UILO = _T("상판내측, 하판외측");
const CString GH_STR_UOLI = _T("상판외측, 하판내측");
/////////////////////////////////////////////////////////////////////////////
// CGirHeightUserDlg dialog

class CGirHeightUserDlg : public CUserDialog
{
// Construction
public:
	CGirHeightUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGirHeightUserDlg)
	enum { IDD = IDD_BD_USERDLG_GIRHEIGHT };
	CBitmapButton	m_GirHtRadio1;
	CBitmapButton	m_GirHtRadio2;
	CBitmapButton	m_GirHtRadio3;
	CBitmapButton	m_GirHtRadio4;
	int		m_nGirHtRadio;
	//}}AFX_DATA
	BOOL	bIsTU;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGirHeightUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGirHeightUserDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnGirhtRadio1();
	afx_msg void OnGirhtRadio2();
	afx_msg void OnGirhtRadio3();
	afx_msg void OnGirhtRadio4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GIRHEIGHTUSERDLG_H__C3905184_C77B_46D4_9FF5_B6F9CE55791C__INCLUDED_)
