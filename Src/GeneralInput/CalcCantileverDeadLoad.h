// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERDEADLOAD_H__C2B32EF6_7CB6_4B3B_9A38_A6CD9DE7B3E6__INCLUDED_)
#define AFX_CALCCANTILEVERDEADLOAD_H__C2B32EF6_7CB6_4B3B_9A38_A6CD9DE7B3E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverDeadLoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverDeadLoad dialog
class CPlateStressOut;
class CCalcCantileverDeadLoad : public CGeneralBaseDlg
{
// Construction
public:
	CCalcCantileverDeadLoad(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcCantileverDeadLoad)
	enum { IDD = IDD_CALC_CANTILEVER_DEADLOAD };
	CXTTabCtrl	m_ctlTab;
	//}}AFX_DATA
	CGridCtrl m_Grid;	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverDeadLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:		
	void SetDataInit();
	void SetDataDefault();	
	void SetDataSave();
	void SetGridTitle();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);

	void OnPreInitDialog();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcCantileverDeadLoad)
	afx_msg void OnSelchangeTabDesign(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERDEADLOAD_H__C2B32EF6_7CB6_4B3B_9A38_A6CD9DE7B3E6__INCLUDED_)
