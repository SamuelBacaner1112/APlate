// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNCTSTEELPAGE_H__F0C159B2_B869_46B4_A932_86DF6643C66C__INCLUDED_)
#define AFX_DESIGNCTSTEELPAGE_H__F0C159B2_B869_46B4_A932_86DF6643C66C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignCTSteelPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignCTSteelPage dialog
class CDataManage;
class CDesignCTSteelPage : public TStackedPage
{
// Construction
public:
	CDesignCTSteelPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignCTSteelPage)
	enum { IDD = IDD_DESIGN_STEEL_CT };
	CGridCtrl m_Grid;
	CGridCtrl m_Grid_Property;
	//}}AFX_DATA
	
	CDataManage *m_pDataManage;
	CDomyunView m_Domyun;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignCTSteelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignCTSteelPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	long m_nIndex;
	void DrawCTSteel();
	void SetCTSteelProperty();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave(){};
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNCTSTEELPAGE_H__F0C159B2_B869_46B4_A932_86DF6643C66C__INCLUDED_)
