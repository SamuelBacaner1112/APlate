// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONDETAIL_H__4AD1D55B_A6A0_4511_A032_91E4EF78C47B__INCLUDED_)
#define AFX_OPTIONDETAIL_H__4AD1D55B_A6A0_4511_A032_91E4EF78C47B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionDetail dialog
#include "resource.h"
class COptionDetail : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionDetail)

// Construction
public:
	COptionDetail();
	~COptionDetail();

// Attributes
public:
	CAPlateDrawStd	*m_pPlateDrawStd;

private:
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;

// Dialog Data
	//{{AFX_DATA(COptionDetail)
	enum { IDD = IDD_OPTIONPAGE_DETAIL };
	CGridCtrl m_Grid;
	CCustomOptionsTreeCtrl	m_ListOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionDetail)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDETAIL_H__4AD1D55B_A6A0_4511_A032_91E4EF78C47B__INCLUDED_)
