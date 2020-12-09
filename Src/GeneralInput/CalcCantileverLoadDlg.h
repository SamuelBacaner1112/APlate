// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERLOADDLG_H__CB74B3D9_5081_4A78_92CE_7BE7D94A986B__INCLUDED_)
#define AFX_CALCCANTILEVERLOADDLG_H__CB74B3D9_5081_4A78_92CE_7BE7D94A986B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverLoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverLoadDlg dialog

class CCalcCantileverLoadDlg : public CGeneralBaseDlg
{
// Construction
public:
	CCalcCantileverLoadDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL IsValid();
	void DrawInputDomyunView(BOOL bZoomAll);
	void SetDataDefault();
	void SetDataInit();
	void OnPreInitDialog();
	void reCalculate();
	void SetDataSave();

// Dialog Data
	//{{AFX_DATA(CCalcCantileverLoadDlg)
	enum { IDD = IDD_CALC_CANTILEVER_LOAD };
	CShRichEdit	m_richeditLoad;
	CXTTabCtrl	m_TabPos;
	//}}AFX_DATA
	BOOL m_bTendon;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcCantileverLoadDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelchangeTabLoadPosition(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERLOADDLG_H__CB74B3D9_5081_4A78_92CE_7BE7D94A986B__INCLUDED_)
