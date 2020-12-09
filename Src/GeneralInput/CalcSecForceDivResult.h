// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCSECFORCEDIVRESULT_H__0154FBAB_5F47_4993_B9A3_153128EB3961__INCLUDED_)
#define AFX_CALCSECFORCEDIVRESULT_H__0154FBAB_5F47_4993_B9A3_153128EB3961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcSecForceDivResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcSecForceDivResult dialog
#include "Resource.h"
#include "ResultDlg.h"
class CCalcSecForceDivResult : public CGeneralBaseDlg
{
// Construction
public:
	CCalcSecForceDivResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcSecForceDivResult)
	enum { IDD = IDD_CALC_SECFORCE_DIV_RESULT };
	CString			m_sHLLFileName;
	CXTFlatComboBox	m_ctlHLCombo;
	CXTFlatEdit		m_flatSavePath;
	//}}AFX_DATA
	
	CGridCtrl m_Grid;
	long m_nCase;
	CResultDlg m_CalcDlg;

	CButton	m_btnSapInput;

	void ReadSapHLLOutputByFolder();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcSecForceDivResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCalcSecForceDivResult)
	afx_msg void OnSelchangeComboHdantype();
	afx_msg void OnButtonMakeSapS2k();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR*nmgv,LRESULT*);

	DECLARE_MESSAGE_MAP()

	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault(){};	
	void SetDataSave();
	void SetPreComboData();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void PathToPathNum(CString & path,CString csExt, long Num) const;
	void SetFileName();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCSECFORCEDIVRESULT_H__0154FBAB_5F47_4993_B9A3_153128EB3961__INCLUDED_)
