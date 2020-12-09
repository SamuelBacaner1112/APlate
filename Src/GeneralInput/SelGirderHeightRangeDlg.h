// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SELGIRDERHEIGHTRANGEDLG_H__9DC4BE12_12AB_4D8A_99F1_B917DCFBA8FF__INCLUDED_)
#define AFX_SELGIRDERHEIGHTRANGEDLG_H__9DC4BE12_12AB_4D8A_99F1_B917DCFBA8FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelGirderHeightRangeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelGirderHeightRangeDlg dialog
class CPlateBridgeApp;
class CSelGirderHeightRangeDlg : public CUserDialog
{
// Construction
public:
	CSelGirderHeightRangeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelGirderHeightRangeDlg)
	enum { IDD = IDD_DIALOG_SUB_GIRDER_REF };
	double	m_dGirderHeight;
	CString	m_szRecommendResult;
	//}}AFX_DATA
	BOOL m_bSM520;
	CPlateBridgeApp *m_pBridge;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelGirderHeightRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelGirderHeightRangeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELGIRDERHEIGHTRANGEDLG_H__9DC4BE12_12AB_4D8A_99F1_B917DCFBA8FF__INCLUDED_)
