// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNCSTEELPAGE_H__61AC39A3_CDF1_4D0D_90DF_5ECC2A9CF944__INCLUDED_)
#define AFX_DESIGNCSTEELPAGE_H__61AC39A3_CDF1_4D0D_90DF_5ECC2A9CF944__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignCSteelPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignCSteelPage dialog
class CDataManage;
class CDesignCSteelPage : public TStackedPage
{
// Construction
public:
	CDesignCSteelPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignCSteelPage)
	enum { IDD = IDD_DESIGN_STEEL_C };
	CGridCtrl m_Grid;
	CGridCtrl m_Grid_Property;
	//}}AFX_DATA
	CDataManage *m_pDataManage;
	CDomyunView m_Domyun;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignCSteelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignCSteelPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	long m_nIndex;
	void DrawCSteel();
	void SetCSteelProperty();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave(){};
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNCSTEELPAGE_H__61AC39A3_CDF1_4D0D_90DF_5ECC2A9CF944__INCLUDED_)
