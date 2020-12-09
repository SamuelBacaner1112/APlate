// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNSTEELDIMPAGE_H__D5503725_33A8_430E_BB90_80B863D01589__INCLUDED_)
#define AFX_DESIGNSTEELDIMPAGE_H__D5503725_33A8_430E_BB90_80B863D01589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignSteelDimPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignSteelDimPage dialog
//#define COLUMN_COUNT_STEEL 32
#define COLUMN_COUNT_STEEL 43

class CDataManage;
class CDesignSteelDimPage : public TStackedPage
{
// Construction
public:
	CDesignSteelDimPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignSteelDimPage)
	enum { IDD = IDD_DESIGN_STEEL_MEMBER };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignSteelDimPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	//static SHGRIDTITLE m_gt[COLUMN_COUNT_STEEL];	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignSteelDimPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	void SetDataDefault();
	void	SetGridTitle();
	void	SetDataInit();
	void	SetDataSave();	
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNSTEELDIMPAGE_H__D5503725_33A8_430E_BB90_80B863D01589__INCLUDED_)
