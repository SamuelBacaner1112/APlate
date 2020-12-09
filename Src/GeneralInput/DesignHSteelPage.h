// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNHSTEELPAGE_H__8027DE68_CB26_4B6A_9883_95B4C52178A5__INCLUDED_)
#define AFX_DESIGNHSTEELPAGE_H__8027DE68_CB26_4B6A_9883_95B4C52178A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignHSteelPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignHSteelPage dialog
class CDataManage;
class CDesignHSteelPage : public TStackedPage
{
// Construction
public:
	CDesignHSteelPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignHSteelPage)
	enum { IDD = IDD_DESIGN_STEEL_H };
	CGridCtrl m_Grid;
	CGridCtrl m_Grid_Property;
	//}}AFX_DATA

	CDataManage *m_pDataManage;
	CDomyunView m_Domyun;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignHSteelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignHSteelPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	long m_nIndex;
	void DrawHSteel();
	void SetHSteelProperty();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave(){};
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNHSTEELPAGE_H__8027DE68_CB26_4B6A_9883_95B4C52178A5__INCLUDED_)
