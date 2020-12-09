// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCFLOORSLABSTRESS_H__CB0943CC_FB1E_4B75_A35B_0592BEF064B7__INCLUDED_)
#define AFX_CALCFLOORSLABSTRESS_H__CB0943CC_FB1E_4B75_A35B_0592BEF064B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcFloorSlabStress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcFloorSlabStress dialog

class CCalcFloorSlabStress : public CGeneralBaseDlg
{
// Construction
public:
	CCalcFloorSlabStress(CWnd* pParent = NULL);   // standard constructor
	void OnPreInitDialog();
	void SetDataSave();
	void SetDataInit();
	void SetDataDefault();
	BOOL IsValid();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
// Dialog Data
	//{{AFX_DATA(CCalcFloorSlabStress)
	enum { IDD = IDD_CALC_FLOORSLAB_STRESS };
	CShRichEdit	m_richeditSlabStress;
	CXTTabCtrl	m_tabFloorSlab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcFloorSlabStress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCalcFloorSlabStress)
	afx_msg void OnSelchangeTabFloorslabStress(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCFLOORSLABSTRESS_H__CB0943CC_FB1E_4B75_A35B_0592BEF064B7__INCLUDED_)
