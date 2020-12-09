// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNGRADEPAGE_H__F55A9A0D_6FD6_4598_B0A7_7381E1E24074__INCLUDED_)
#define AFX_DESIGNGRADEPAGE_H__F55A9A0D_6FD6_4598_B0A7_7381E1E24074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignGradePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignGradePage dialog
#define COLUMN_COUNT_GRADE 8

class CDataManage;
class CDesignGradePage : public TStackedPage
{
// Construction
public:
	CDesignGradePage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignGradePage)
	enum { IDD = IDD_DESIGN_GRADE_LOAD };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignGradePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT_GRADE];

protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignGradePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
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

#endif // !defined(AFX_DESIGNGRADEPAGE_H__F55A9A0D_6FD6_4598_B0A7_7381E1E24074__INCLUDED_)
