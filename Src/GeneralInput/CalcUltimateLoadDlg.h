// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCULTIMATELOADDLG_H__2384D1EC_27E6_420B_A3F0_1571AD366813__INCLUDED_)
#define AFX_CALCULTIMATELOADDLG_H__2384D1EC_27E6_420B_A3F0_1571AD366813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcUltimateLoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcUltimateLoadDlg dialog

class CCalcUltimateLoadDlg : public CGeneralBaseDlg
{
// Construction
public:
	CCalcUltimateLoadDlg(CWnd* pParent = NULL);   // standard constructor

	void InsertDomyun(CDomyun *pDom);
	void OnPreInitDialog();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void SetDataDefault();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	long GetRebarDiaIdx(CString szDia);
	CString GetRebar(long nIdx);
	CString GetRebar(CString szDia);
	CString GetCheck(BOOL bCheck);
	CString GetTitle();
	void SetActiveGrid();
	BOOL IsValid();

	CString m_sRebar;
// Dialog Data
	//{{AFX_DATA(CCalcUltimateLoadDlg)
	enum { IDD = IDD_CALC_ULTIMATELOAD_DLG };
	CShRichEdit	m_richeditRebar;
	CXTTabCtrl	m_TabRebar;
	CGridCtrl	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcUltimateLoadDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcUltimateLoadDlg)
	afx_msg void OnSelchangeTabRebar(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedRow(NMHDR*nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCULTIMATELOADDLG_H__2384D1EC_27E6_420B_A3F0_1571AD366813__INCLUDED_)
