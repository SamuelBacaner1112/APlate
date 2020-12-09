// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCWINDFORCE_H__0F579C74_A33F_4867_B86A_D6D8ACB785F3__INCLUDED_)
#define AFX_CALCWINDFORCE_H__0F579C74_A33F_4867_B86A_D6D8ACB785F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcWindForce.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcWindForce dialog
#include "Resource.h"
#include "ResultDlg.h"
class CCalcWindForce : public CGeneralBaseDlg
{
// Construction
public:
	CCalcWindForce(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcWindForce)
	enum { IDD = IDD_CALC_WINDFORCE };
	CComboBox	m_comboJijumPos;
	CString	m_strStation;
	CString	m_sWindFileName;
	//}}AFX_DATA
	CResultDlg	m_CalcDlg;
	CGridCtrl	m_Grid;
	long		m_nCase;
	CXTTabCtrl	m_TabWind;
	CButton		m_btnSapInput;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcWindForce)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:		
	void SetDataInit();
	void SetDataInitWindLoad();
	void SetDataInitResult();
	void SetDataDefault();	
	void SetDataSave();
	void SetGridTitle();
	void SetGridTitleWindLoad();
	void SetGridTitleResult();
	void SetComboData();
	void OnPreInitDialog();
	void SetFileName();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void PathToPathNum(CString & path,CString csExt, long Num) const;
	void DrawDimJoint(CDomyun *pDom, CFEMManage *pFEM, double dTextHeight);

public:
	BOOL IsValid();

protected:
	enum {PAGE_LOAD, PAGE_RESULT};


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcWindForce)
	afx_msg void OnButtonMakeSapS2k();
	afx_msg void OnSelchangeComboJijum();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedData(NMHDR*nmgv,LRESULT*);
	afx_msg void OnSelchangeTabWind(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCWINDFORCE_H__0F579C74_A33F_4867_B86A_D6D8ACB785F3__INCLUDED_)
