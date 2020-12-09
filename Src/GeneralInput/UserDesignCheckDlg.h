// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_USERDESIGNCHECKDLG_H__E5EAD046_2D75_4D27_A623_FBC77F9E95F8__INCLUDED_)
#define AFX_USERDESIGNCHECKDLG_H__E5EAD046_2D75_4D27_A623_FBC77F9E95F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserDesignCheckDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserDesignCheckDlg dialog
class CUserDesignCheckDlg : public CXTResizeDialog
{
// Construction
public:
	CUserDesignCheckDlg(CXLControl *pXL, CString szTitle=_T(""), CWnd* pParent = NULL);   // standard constructor
	void SetXLRange(long nSttRow, long nSttCol, long nEndRow, long nEndCol);
	
	CXLControl *m_pXL;
	CString	m_szTitle;
	CString m_szPathName;

	long	m_nSttRow;
	long	m_nSttCol;
	long	m_nEndRow;
	long	m_nEndCol;

// Dialog Data
	//{{AFX_DATA(CUserDesignCheckDlg)
	enum { IDD = IDD_DIALOG_USER_DESIGN_CHECK };
	CShRichEdit	m_RichEditCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserDesignCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserDesignCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFilesave();
	afx_msg void OnClose();
	afx_msg void OnButtonFileopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERDESIGNCHECKDLG_H__E5EAD046_2D75_4D27_A623_FBC77F9E95F8__INCLUDED_)
