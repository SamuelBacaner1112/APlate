// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCSECOUTLINE_H__2B79EAC6_B63B_4896_B807_92C022DE5342__INCLUDED_)
#define AFX_CALCSECOUTLINE_H__2B79EAC6_B63B_4896_B807_92C022DE5342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcSecOutline.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcSecOutline dialog
class AFX_EXT_CLASS CCalcSecOutline : public CGeneralBaseDlg
{
// Construction
public:
	CCalcSecOutline(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcSecOutline)
	enum { IDD = IDD_CALC_SECTION_OUTLINE };
	CXTFlatComboBox	m_Girder;
	CXTFlatEdit		m_ctrlEdit1;
	CXTFlatEdit		m_ctrlEdit2;
	CXTFlatEdit		m_ctrlEdit3;
	CXTFlatEdit		m_ctrlEdit4;
	CXTFlatEdit		m_ctrlEdit5;
	double			m_dMomentY;
	double			m_dHeightY;
	double			m_dFlangeY;
	CGridCtrl		m_Grid;
	CString			m_Static;
	BOOL			m_bPic;
	//}}AFX_DATA
	CButton		m_btnMoment;
	CButton		m_btnCheckSec;
	CButton		m_btnDelete;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcSecOutline)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	long	m_nG;
	long	m_nIdxSelectedSec;

public:
	void SetDataDefault();
	void SetDataSave();
	void SetDataInit();
	void SetGridTitle();
	void DrawInputDomyunView(BOOL bZoomAll);

	void OnPreInitDialog();
	double GetMinSpace();
	double GetMaxSpace();
	void SetLButtonDblClk (UINT nFlags, CPoint uXy);
	void GetStationNearBx(long nG, double &dSta);

	// Generated message map functions
	//{{AFX_MSG(CCalcSecOutline)
	virtual BOOL OnInitDialog();
	afx_msg void OnSSummaryData();
	afx_msg void OnSSummaryDel();
	afx_msg void OnSSummaryPic();
	afx_msg void OnSelchangeSSummaryCombo1();
	afx_msg void OnClose();
	afx_msg void OnButtonSectionCheck();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR*nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCSECOUTLINE_H__2B79EAC6_B63B_4896_B807_92C022DE5342__INCLUDED_)
