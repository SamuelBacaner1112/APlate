// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNLSTEELPAGE_H__83AABD05_3D94_47A8_869E_92F3FB56B439__INCLUDED_)
#define AFX_DESIGNLSTEELPAGE_H__83AABD05_3D94_47A8_869E_92F3FB56B439__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignLSteelPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignLSteelPage dialog
class CDataManage;
class CDesignLSteelPage : public TStackedPage
{
// Construction
public:
	CDesignLSteelPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignLSteelPage)
	enum { IDD = IDD_DESIGN_STEEL_L1 };
	CGridCtrl m_Grid;
	CGridCtrl m_Grid_Property;
	//}}AFX_DATA

	CDataManage *m_pDataManage;
	CDomyunView m_Domyun;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignLSteelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignLSteelPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	long m_nIndex;
	void DrawLSteel();
	void SetLSteelProperty();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave(){};
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNLSTEELPAGE_H__83AABD05_3D94_47A8_869E_92F3FB56B439__INCLUDED_)
