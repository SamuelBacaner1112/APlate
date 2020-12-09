// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNDEDUCTPAGE_H__EB9B5E47_8EC7_4915_A1C0_C868957DB047__INCLUDED_)
#define AFX_DESIGNDEDUCTPAGE_H__EB9B5E47_8EC7_4915_A1C0_C868957DB047__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignDeductPage.h : header file
//

#define ROW_COUNT_DEDUCT 17

/////////////////////////////////////////////////////////////////////////////
// CDesignDeductPage dialog
class CDataManage;
class CDesignDeductPage : public TStackedPage
{
// Construction
public:
	CDesignDeductPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignDeductPage)
	enum { IDD = IDD_DESIGN_DEDUCT_SET };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;
	CStringArray m_strCombo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignDeductPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignDeductPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

// Implementation
private:
	static SHGRIDTITLE m_gt[ROW_COUNT_DEDUCT];	

public:
	void SetGridTitle();	
	void SetDataInit();	
	void SetDataSave();	
	void SetDataDefault();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNDEDUCTPAGE_H__EB9B5E47_8EC7_4915_A1C0_C868957DB047__INCLUDED_)
