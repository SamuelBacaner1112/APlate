// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNCONCRETEPAGE_H__1293B976_E7C8_42D7_88EE_72069428A3CF__INCLUDED_)
#define AFX_DESIGNCONCRETEPAGE_H__1293B976_E7C8_42D7_88EE_72069428A3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignConcretePage.h : header file
//

#define COLUMN_COUNT_CONCRET 12

/////////////////////////////////////////////////////////////////////////////
// CDesignConcretePage dialog
class CDataManage;
class CDesignConcretePage : public TStackedPage
{
// Construction
public:
	CDesignConcretePage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignConcretePage)
	enum { IDD =  IDD_DESIGN_CONCRETE };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignConcretePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT_CONCRET];	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignConcretePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
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

#endif // !defined(AFX_DESIGNCONCRETEPAGE_H__1293B976_E7C8_42D7_88EE_72069428A3CF__INCLUDED_)
