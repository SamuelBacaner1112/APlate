// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNPRESTRESSPROPERTYPAGE_H__97CD2488_A7B4_4261_9C22_4786F97B1903__INCLUDED_)
#define AFX_DESIGNPRESTRESSPROPERTYPAGE_H__97CD2488_A7B4_4261_9C22_4786F97B1903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignPrestressPropertyPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPropertyPage dialog
#define ROW 18
#define COL  4

class CDesignPrestressPropertyPage : public TStackedPage
{
// Construction
public:
	CDesignPrestressPropertyPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor
	CDataManage *m_pDataManage;

// Dialog Data
	//{{AFX_DATA(CDesignPrestressPropertyPage)
	enum { IDD = IDD_DESIGN_PS_MEMBER };
	CGridCtrl m_Grid;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignPrestressPropertyPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	static SHGRIDTITLE m_gt[COL];
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignPrestressPropertyPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
public:
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void OnPreInitDialog();
protected:
	CString GetStrDuctType(long nType);
	long GetLondDuctType(CString strDuctType);
	double GetCurvatureFriction(long nIndex);
	double GetWobbleFriction(long nIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNPRESTRESSPROPERTYPAGE_H__97CD2488_A7B4_4261_9C22_4786F97B1903__INCLUDED_)
