// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNLVARSTEELPAGE_H__748343C7_890A_4395_B94D_4C9DF6CE2AB8__INCLUDED_)
#define AFX_DESIGNLVARSTEELPAGE_H__748343C7_890A_4395_B94D_4C9DF6CE2AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignLVarSteelPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignLVarSteelPage dialog
class CDataManage;
class CDesignLVarSteelPage : public TStackedPage
{
// Construction
public:
	CDesignLVarSteelPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignLVarSteelPage)
	enum { IDD = IDD_DESIGN_STEEL_L2 };
	CGridCtrl m_Grid;
	CGridCtrl m_Grid_Property;
	//}}AFX_DATA

	CDataManage *m_pDataManage;
	CDomyunView m_Domyun;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignLVarSteelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignLVarSteelPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	long m_nIndex;
	void DrawLVarSteel();
	void SetLVarSteelProperty();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave(){};
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNLVARSTEELPAGE_H__748343C7_890A_4395_B94D_4C9DF6CE2AB8__INCLUDED_)
