// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_HBRSPSTDINPUTDLG_H__DC2DC145_B721_4554_9198_6610A4C9D431__INCLUDED_)
#define AFX_HBRSPSTDINPUTDLG_H__DC2DC145_B721_4554_9198_6610A4C9D431__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HBrSpStdInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHBrSpStdInputDlg dialog

class CHBrSpStdInputDlg : public CDialog
{
// Construction
public:
	CHBrSpStdInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHBrSpStdInputDlg)
	enum { IDD = IDD_STANDARD_HBRSPLICE };
	CComboBox	m_comboBoltDia;
	CString	m_sBL;
	CString	m_sGw;
	CString	m_sSA;
	CString	m_sSB;
	CString	m_sSC;
	CString	m_sSD;
	CString	m_sSE;
	CString	m_sStep1;
	CString	m_sStep2;
	CString	m_sStep3;
	CString	m_sStep4;
	CString	m_sStep5;
	CString	m_sW;
	CString	m_sW1;
	//}}AFX_DATA
	CGeneralBaseStd	*m_pStd;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHBrSpStdInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();

protected:

	// Generated message map functions
	//{{AFX_MSG(CHBrSpStdInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRecommend();
	virtual void OnOK();
	afx_msg void OnSelchangeComboDia();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HBRSPSTDINPUTDLG_H__DC2DC145_B721_4554_9198_6610A4C9D431__INCLUDED_)
