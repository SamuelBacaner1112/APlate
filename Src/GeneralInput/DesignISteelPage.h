// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNISTEELPAGE_H__ADE21897_0B30_43EC_ADCB_781F2698B5FF__INCLUDED_)
#define AFX_DESIGNISTEELPAGE_H__ADE21897_0B30_43EC_ADCB_781F2698B5FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignISteelPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignISteelPage dialog
class CDataManage;
class CDesignISteelPage : public TStackedPage
{
// Construction
public:
	CDesignISteelPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignISteelPage)
	enum { IDD = IDD_DESIGN_STEEL_I };
	CGridCtrl m_Grid;
	CGridCtrl m_Grid_Property;
	//}}AFX_DATA

	CDataManage *m_pDataManage;
	CDomyunView m_Domyun;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignISteelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignISteelPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	long m_nIndex;
	void DrawISteel();
	void SetISteelProperty();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave(){};
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNISTEELPAGE_H__ADE21897_0B30_43EC_ADCB_781F2698B5FF__INCLUDED_)
