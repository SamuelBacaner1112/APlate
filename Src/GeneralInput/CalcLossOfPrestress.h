// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCLOSSOFPRESTRESS_H__030F0A41_E22D_482C_9CDE_0622482DC65B__INCLUDED_)
#define AFX_CALCLOSSOFPRESTRESS_H__030F0A41_E22D_482C_9CDE_0622482DC65B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcLossOfPrestress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcLossOfPrestress dialog

class CCalcLossOfPrestress : public CGeneralBaseDlg
{
// Construction
public:
	CCalcLossOfPrestress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcLossOfPrestress)
	enum { IDD = IDD_CALC_LOSS_OF_PRESTRESS };
	CShRichEdit	m_richeditPrestress;
	CXTTabCtrl	m_tabPrestress;
	CXTFlatEdit	m_ctrlEdit1;
	CXTFlatEdit	m_ctrlEdit2;
	double		m_dCTC;
	long		m_nQtyPs;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcLossOfPrestress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetGridTitle();
	void SetDataSave();
	void SetDataInit();
	void SetDataDefault();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	BOOL IsValid();
	long m_nIndex;
	// Generated message map functions
	//{{AFX_MSG(CCalcLossOfPrestress)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabPrestress(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCLOSSOFPRESTRESS_H__030F0A41_E22D_482C_9CDE_0622482DC65B__INCLUDED_)
