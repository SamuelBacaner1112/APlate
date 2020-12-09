// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCDISPLACEMENT_H__DCE20459_3E84_454B_AA3D_6E25CC6CDD98__INCLUDED_)
#define AFX_CALCDISPLACEMENT_H__DCE20459_3E84_454B_AA3D_6E25CC6CDD98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcDisplacement.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcDisplacement dialog

class AFX_EXT_CLASS CCalcDisplacement : public CGeneralBaseDlg
{
// Construction
public:
	BOOL IsValid();
	CCalcDisplacement(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcDisplacement)
	enum { IDD = IDD_CALC_DISPLACEMENT };
	CXTFlatEdit		m_ctrlEdit1;
	CXTFlatEdit		m_ctrlEdit2;
	//}}AFX_DATA
	long m_nLoadComb;
	long m_nLoadCase;

	CXTFlatComboBox	m_comboGirder;
	CXTFlatComboBox	m_comboJigan;
	CXTFlatComboBox	m_comboLoadComb;
	CButton	m_btnReadFile;
	CButton	m_btnOutText;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcDisplacement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void DisplacementOutput(CDomyun *pDom, long nG, long nLC);

private:
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
protected:
	void SetComboData();

	void SetComboLoadComb();
	int  GetLoadCase();
	double GetMaxDisplace(long nG, long nK, long nLC);
	void Calc_Displacement();

	// Generated message map functions
	//{{AFX_MSG(CCalcDisplacement)
	afx_msg void OnDisplaceTotext();
	afx_msg void OnSelchangeDisplaceCombo1();
	afx_msg void OnSelchangeDisplaceCombo2();
	afx_msg void OnSelchangeDisplaceCombo3();
	afx_msg void OnSDisplaceReadfile();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCDISPLACEMENT_H__DCE20459_3E84_454B_AA3D_6E25CC6CDD98__INCLUDED_)
