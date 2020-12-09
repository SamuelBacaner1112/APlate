// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNBMPAGE_H__B38C32C0_CA9B_4408_BED3_B309DD884CAB__INCLUDED_)
#define AFX_DESIGNBMPAGE_H__B38C32C0_CA9B_4408_BED3_B309DD884CAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignBMPage.h : header file
//

#define COLUMN_COUNT_BILLENV 9

/////////////////////////////////////////////////////////////////////////////
// CDesignBMPage dialog
class CDataManage;
class CDesignBMPage : public TStackedPage
{
// Construction
public:
	CDesignBMPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignBMPage)
	enum { IDD = IDD_DESIGN_BM_SET };
	CGridCtrl m_Grid;
	//}}AFX_DATA

	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignBMPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignBMPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT_BILLENV];	

public:
	void SetGridTitle();	
	void SetDataInit();	
	void SetDataSave();	
	void SetDataDefault();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNBMPAGE_H__B38C32C0_CA9B_4408_BED3_B309DD884CAB__INCLUDED_)
