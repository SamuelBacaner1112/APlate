// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONTENDON_H__CDE21766_EBB6_423D_89C6_E1B84DD9BCB8__INCLUDED_)
#define AFX_OPTIONTENDON_H__CDE21766_EBB6_423D_89C6_E1B84DD9BCB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTendon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionTendon dialog
#include "resource.h"
class COptionTendon : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionTendon)

// Construction
public:
	COptionTendon();
	~COptionTendon();
	CAPlateDrawStd	*m_pPlateDrawStd;

private:	
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;

// Dialog Data
	//{{AFX_DATA(COptionTendon)
	enum { IDD = IDD_OPTIONPAGE_TENDON };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionTendon)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionTendon)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTENDON_H__CDE21766_EBB6_423D_89C6_E1B84DD9BCB8__INCLUDED_)
