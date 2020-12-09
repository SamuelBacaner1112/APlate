// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_PAINTOUTASSEMBLY_H__B5A09ED5_B73D_4F07_9108_C8DDF16E30D2__INCLUDED_)
#define AFX_PAINTOUTASSEMBLY_H__B5A09ED5_B73D_4F07_9108_C8DDF16E30D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaintOutAssembly.h : header file
//
 
/////////////////////////////////////////////////////////////////////////////
// CPaintOutAssembly dialog
#include "resource.h"
 
class CPaintOutSheet;
class CGridCtrl;
class CXLCell;
 
class AFX_EXT_CLASS CPaintOutAssembly : public CPropertyPage,
									    public CThreadWork
{
	DECLARE_DYNCREATE(CPaintOutAssembly)

// Construction
public:
	void DrawGrid();
	CPaintOutAssembly();
	~CPaintOutAssembly();

	CPaintOutSheet* m_pPaintOutSheet;
	CGridCtrl*		m_pGrid;

	long m_nMakeFlag; // Property Page ±¸ºÐ
	CString m_sTotalRecord;
	UINT DoWork();
	
// Dialog Data
	//{{AFX_DATA(CPaintOutAssembly)
	enum { IDD = IDD_DET_PAINT_ASSEMBLY };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPaintOutAssembly)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPaintOutAssembly)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitGrid();
	//
	void InitGridSang();
	void InitGridBujeSang();
	void InitGridBujeTotal();
	void InitGridAssembly();
	void InitGridAssemblyTotal();
	void InitGridGrandAssembly();
	void InitGridGrandAssemblyTotal();
	void InitGridMatAndThick();
	void InitGridBoltSang();
	void InitGridBoltTotal();
	void InitGridSummary();

	void DrawGridSang();
	void DrawGridBujeSang();
	void DrawGridBujeTotal();
	void DrawGridAssembly();
	void DrawGridAssemblyTotal();
	void DrawGridGrandAssembly();
	void DrawGridGrandAssemblyTotal();
	void DrawGridMatAndThick();
	void DrawGridBoltSang();
	void DrawGridBoltTotal();
	void DrawGridSummary();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAINTOUTASSEMBLY_H__B5A09ED5_B73D_4F07_9108_C8DDF16E30D2__INCLUDED_)
