// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_WELDOUTASSEMBLY_H__F2B413FB_D88F_4F28_AD30_94102D1AB133__INCLUDED_)
#define AFX_WELDOUTASSEMBLY_H__F2B413FB_D88F_4F28_AD30_94102D1AB133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CuttingOuWeld.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCuttingOuWeld dialog

#include "resource.h"
 
class CWeldOutSheet;
class CGridCtrl;
 
class AFX_EXT_CLASS CWeldOutAssembly : public CPropertyPage,
									    public CThreadWork
{
	DECLARE_DYNCREATE(CWeldOutAssembly)

// Construction
public:
	CWeldOutAssembly();
	~CWeldOutAssembly();

	CWeldOutSheet* m_pWeldOutSheet;
	CGridCtrl*	   m_pGrid;
	long m_nMakeFlag; // Property Page ±¸ºÐ

	void DrawGrid();
	void InitGrid();
	UINT DoWork();

// Dialog Data
	//{{AFX_DATA(CWeldOutAssembly)
	enum { IDD = IDD_DET_WELD_ASSEMBLY };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWeldOutAssembly)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWeldOutAssembly)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_sTotalRecord;
	void InitGridFirst(long nRows, long nCols);
	void InitGridTitle(long nCols, char *Title[]);
	//
	void DrawGridSang();
	void DrawGridAssembly();
	void DrawGridAssemblySang();
	void DrawGridAssemblyTotal();
	void DrawGridWeldType();
	void DrawGridWeldThick();
	//
	void InitGridSang();
	void InitGridAssembly();
	void InitGridAssemblySang();
	void InitGridAssemblyTotal();
	void InitGridWeldType();
	void InitGridWeldThick();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELDOUTASSEMBLY_H__F2B413FB_D88F_4F28_AD30_94102D1AB133__INCLUDED_)
