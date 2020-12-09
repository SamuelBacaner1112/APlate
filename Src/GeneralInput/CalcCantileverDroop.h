// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERDROOP_H__277D30A1_4D46_4EC0_B02C_A9BDF0615039__INCLUDED_)
#define AFX_CALCCANTILEVERDROOP_H__277D30A1_4D46_4EC0_B02C_A9BDF0615039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverDroop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverDroop dialog

class CCalcCantileverDroop : public CGeneralBaseDlg
{
// Construction
public:
	CCalcCantileverDroop(CWnd* pParent = NULL);   // standard constructor

	void OnPreInitDialog();
	void DrawInputDomyunView(BOOL bZoomAll);
	void SetDataDefault();
	void SetDataInit();
	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CCalcCantileverDroop)
	enum { IDD = IDD_CALC_CANTILEVER_DROOP };
	CShRichEdit	m_richeditDroop;
	CXTTabCtrl	m_TabDroop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverDroop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcCantileverDroop)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabDroop(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERDROOP_H__277D30A1_4D46_4EC0_B02C_A9BDF0615039__INCLUDED_)
