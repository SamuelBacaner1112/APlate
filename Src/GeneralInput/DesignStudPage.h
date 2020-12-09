// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNSTUDPAGE_H__4A2E65F2_3927_4B5F_9520_F2389B3F473C__INCLUDED_)
#define AFX_DESIGNSTUDPAGE_H__4A2E65F2_3927_4B5F_9520_F2389B3F473C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignStudPage.h : header file
//

//////////////////////////////////////////////////////////////////////
// CDesignStudPage dialog
class CDataManage;
class CDesignStudPage : public TStackedPage
{
// Construction
public:
	CDesignStudPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignStudPage)
	enum { IDD = IDD_DESIGN_STUD };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignStudPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignStudPage)
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

#endif // !defined(AFX_DESIGNSTUDPAGE_H__4A2E65F2_3927_4B5F_9520_F2389B3F473C__INCLUDED_)
