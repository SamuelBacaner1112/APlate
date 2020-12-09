// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_RESULTDLG_H__B9FC7907_17D6_4E81_99A8_9FF6F3BF24D9__INCLUDED_)
#define AFX_RESULTDLG_H__B9FC7907_17D6_4E81_99A8_9FF6F3BF24D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog
#define STDIO_OPEN  1
#define STDIO_CLOSE 2
#define STDIO_WRITE 3

#define WM_STDIO_COMMAND WM_USER + 33
#define WM_CALC_OK		 WM_USER + 34

#define WM_CALC_END		 WM_USER + 35

#include "Afxmt.h" // For CCriticalSection
#include "resource.h"

class CReDir;
class AFX_EXT_CLASS CResultDlg : public CDialog
{
// Construction
public:
	CResultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultDlg();
	
	CReDir *m_pRedirect;
	CWnd *m_pMainWnd;
	CCriticalSection m_criticalSection;
	CString m_strCmdLine;
	CString m_szBtnText;
	CStringList m_StdioCommand;
	CButton* m_pButton;	
	CStringList m_StrCmdList;
	CString m_szCurrPath;

	BOOL m_bError;
	BOOL m_bWarning;

public:
	void SetBtnText(CString szText) { m_szBtnText = szText; };
	void AddCommand(WPARAM wCommand, LPCSTR lpszCommand);
	void Create(CWnd *pParent, CWnd *pMainWnd=NULL) { m_pMainWnd = pMainWnd;  CDialog::Create(IDD, pParent); }	
// Dialog Data
	//{{AFX_DATA(CResultDlg)
	enum { IDD = IDD_DIALOG_RESULT_PLATE };
	CColorListBox	m_ctlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CResultDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg LRESULT OnStdioCommand(WPARAM wCommand, LPARAM lParam);
	afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTDLG_H__B9FC7907_17D6_4E81_99A8_9FF6F3BF24D9__INCLUDED_)
