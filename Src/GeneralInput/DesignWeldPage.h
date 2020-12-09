// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNWELDPAGE_H__3EBC3DD3_45C1_4D6B_95AC_444C4A01D00A__INCLUDED_)
#define AFX_DESIGNWELDPAGE_H__3EBC3DD3_45C1_4D6B_95AC_444C4A01D00A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignWeldPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignWeldPage dialog
#define COLUMN_COUNT_WELD 7

class CDataManage;
class CDesignWeldPage : public TStackedPage
{
// Construction
public:
	CDesignWeldPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignWeldPage)
	enum { IDD = IDD_DESIGN_WELD_SET };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignWeldPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT_WELD];	
	CStringArray	m_WeldType;
	CStringArray	m_ImproveType;
	CStringArray	m_LayType;

protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignWeldPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	void SetDataSave();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNWELDPAGE_H__3EBC3DD3_45C1_4D6B_95AC_444C4A01D00A__INCLUDED_)
