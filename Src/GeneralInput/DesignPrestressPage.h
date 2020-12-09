// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNPRESTRESSPAGE_H__5A6798BA_B888_4FF0_AA80_16B9A52BECF1__INCLUDED_)
#define AFX_DESIGNPRESTRESSPAGE_H__5A6798BA_B888_4FF0_AA80_16B9A52BECF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignPrestressPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPage dialog
#define COLUMN_COUNT_PS 13
#define ROW_COUNT_PS 18

class AFX_EXT_CLASS CDesignPrestressPage : public TStackedPage
{
// Construction
public:
	CDesignPrestressPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignPrestressPage)
	enum { IDD = IDD_DESIGN_PS_PROPERTY };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignPrestressPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT_PS];
	// Generated message map functions
	//{{AFX_MSG(CDesignPrestressPage)
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNPRESTRESSPAGE_H__5A6798BA_B888_4FF0_AA80_16B9A52BECF1__INCLUDED_)
