// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONSHOE_H__C012D9AD_2EBD_4004_8828_446DD6FD39F9__INCLUDED_)
#define AFX_OPTIONSHOE_H__C012D9AD_2EBD_4004_8828_446DD6FD39F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionShoe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionShoe dialog
#include "resource.h"
class COptionShoe : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionShoe)

// Construction
public:
	COptionShoe();
	~COptionShoe();

	CAPlateDrawStd	*m_pPlateDrawStd;
private:	
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;

// Dialog Data
	//{{AFX_DATA(COptionShoe)
	enum { IDD = IDD_OPTIONPAGE_SHOE };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionShoe)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionShoe)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSHOE_H__C012D9AD_2EBD_4004_8828_446DD6FD39F9__INCLUDED_)
