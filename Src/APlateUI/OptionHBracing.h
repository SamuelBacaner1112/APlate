// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONHBRACING_H__E28D6CC1_DBE9_43E2_9285_3954D6CF3AD7__INCLUDED_)
#define AFX_OPTIONHBRACING_H__E28D6CC1_DBE9_43E2_9285_3954D6CF3AD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionHBracing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionHBracing dialog
#include "resource.h"
class COptionHBracing : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionHBracing)

// Construction
public:
	COptionHBracing();
	~COptionHBracing();

// Attributes
public:
	CAPlateDrawStd	*m_pPlateDrawStd;
private:
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;

public:
// Dialog Data
	//{{AFX_DATA(COptionHBracing)
	enum { IDD = IDD_OPTIONPAGE_HBRACING };
	CGridCtrl m_Grid;
	CCustomOptionsTreeCtrl	m_ListOption;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionHBracing)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionHBracing)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONHBRACING_H__E28D6CC1_DBE9_43E2_9285_3954D6CF3AD7__INCLUDED_)
