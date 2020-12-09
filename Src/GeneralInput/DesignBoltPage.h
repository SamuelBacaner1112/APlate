// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNBOLTPAGE_H__DA7D32AA_2BB2_44EE_9B0B_F3B0A68EC687__INCLUDED_)
#define AFX_DESIGNBOLTPAGE_H__DA7D32AA_2BB2_44EE_9B0B_F3B0A68EC687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignBoltPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignBoltPage dialog
class CDataManage;
class CDesignBoltPage : public TStackedPage
{
// Construction
public:
	CDesignBoltPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignBoltPage)
	enum { IDD = IDD_DESIGN_BOLT };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignBoltPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignBoltPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellButtonClick(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNBOLTPAGE_H__DA7D32AA_2BB2_44EE_9B0B_F3B0A68EC687__INCLUDED_)
