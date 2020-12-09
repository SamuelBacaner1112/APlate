// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_VSTIFFSTDINPUTDLG_H__B2E27A01_7EE1_4153_B03E_4DF4F16A7620__INCLUDED_)
#define AFX_VSTIFFSTDINPUTDLG_H__B2E27A01_7EE1_4153_B03E_4DF4F16A7620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VStiffStdInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVStiffStdInputDlg dialog

class CVStiffStdInputDlg : public CDialog
{
// Construction
public:
	CVStiffStdInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVStiffStdInputDlg)
	enum { IDD = IDD_STANDARD_VSTIFF };
	CGridCtrl m_Grid;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CDomyunView m_Domyun;
	CGeneralBaseStd		*m_pStd;
	long			     m_nG;

	CButton	m_btnRecom;
	CButton	m_btnOk;
	CButton	m_btnCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVStiffStdInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawInputGenType(CDomyun *pDom);
	void DrawInputNegaType(CDomyun *pDom);
	void SetGridTitle();


private:
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	double	m_dA_p;
	double	m_dB_p;
	double	m_dC_p;
	double	m_dT_p;
	double	m_dC_n;
	double	m_dT_n;
protected:

	// Generated message map functions
	//{{AFX_MSG(CVStiffStdInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRecommend();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSTIFFSTDINPUTDLG_H__B2E27A01_7EE1_4153_B03E_4DF4F16A7620__INCLUDED_)
