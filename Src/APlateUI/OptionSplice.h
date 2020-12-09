// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONSPLICE_H__D2E83DFB_BE2B_43E4_92A1_6B142B205698__INCLUDED_)
#define AFX_OPTIONSPLICE_H__D2E83DFB_BE2B_43E4_92A1_6B142B205698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSplice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionSplice dialog
#include "resource.h"
class COptionSplice : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionSplice)

// Construction
public:
	COptionSplice();
	~COptionSplice();

	CAPlateDrawStd	*m_pPlateDrawStd;

// Dialog Data
	//{{AFX_DATA(COptionSplice)
	enum { IDD = IDD_OPTIONPAGE_SPLICE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	CGridCtrl m_Grid;
	CCustomOptionsTreeCtrl	m_ListOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionSplice)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionSplice)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	void InitGridData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSPLICE_H__D2E83DFB_BE2B_43E4_92A1_6B142B205698__INCLUDED_)
