// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONGENERAL_H__11C0C07E_E6E4_4135_9CE5_16B659AF39E1__INCLUDED_)
#define AFX_OPTIONGENERAL_H__11C0C07E_E6E4_4135_9CE5_16B659AF39E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionGeneral dialog
#include "resource.h"
class COptionGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionGeneral)

// Construction
public:
	COptionGeneral();
	~COptionGeneral();

// Attributes
public:
	CAPlateDrawStd	*m_pPlateDrawStd;

private:
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;

public:
// Dialog Data
	//{{AFX_DATA(COptionGeneral)
	enum { IDD = IDD_OPTIONPAGE_GENERAL };
	CGridCtrl m_Grid;
	CCustomOptionsTreeCtrl	m_ListOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionGeneral)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONGENERAL_H__11C0C07E_E6E4_4135_9CE5_16B659AF39E1__INCLUDED_)
