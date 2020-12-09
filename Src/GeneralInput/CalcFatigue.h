// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCFATIGUE_H__BF85D12D_D4E1_480C_914E_BB42ACD7718C__INCLUDED_)
#define AFX_CALCFATIGUE_H__BF85D12D_D4E1_480C_914E_BB42ACD7718C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcFatigue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcFatigue dialog

class CCalcFatigue : public CGeneralBaseDlg
{
// Construction
public:
	CCalcFatigue(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcFatigue)
	enum { IDD = IDD_CALC_FATIGUE };
	CXTFlatEdit		m_ctrlEdit1;
	CXTFlatEdit		m_ctrlEdit2;
	CXTFlatEdit		m_ctrlEdit3;
	CGridCtrl	m_Grid1;
	CGridCtrl	m_Grid2;
	CGridCtrl	m_Grid3;
	int		m_nRoadType;
	int		m_nStruType;
	CString	m_sCount1;
	CString	m_sCount2;
	CString	m_sCount3;
	int		m_nCheckType;
	//}}AFX_DATA
	CXTFlatComboBox	m_cbST;
	CXTFlatComboBox	m_cbRT;
	CXTFlatComboBox m_ComboBox1;
	CButton	m_btnOutText;
	CButton	m_btnOutExcel;
	CButton	m_btnPrint;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcFatigue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetGridTitle();
	void SetGridTitleMoment();
	void SetGridTitleShear();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void SetDataInitMoment();
	void SetDataInitShear();
	void SetComboData();
	void OnPreInitDialog();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	// Generated message map functions
	//{{AFX_MSG(CCalcFatigue)
	afx_msg void OnSelchangeStressCombo1();
	afx_msg void OnSelchangeStressCombo2();
	afx_msg void OnSelchangeStressCombo3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCFATIGUE_H__BF85D12D_D4E1_480C_914E_BB42ACD7718C__INCLUDED_)
