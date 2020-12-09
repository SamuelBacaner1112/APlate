// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONDECKREBAR_H__CDAB078C_B061_4C14_BB36_13A38892BEB4__INCLUDED_)
#define AFX_OPTIONDECKREBAR_H__CDAB078C_B061_4C14_BB36_13A38892BEB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDeckRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionDeckRebar dialog
#include "resource.h"

class COptionDeckRebar : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionDeckRebar)
// Construction
public:
	COptionDeckRebar();   // standard constructor
	~COptionDeckRebar();

	CAPlateDrawStd	*m_pPlateDrawStd;

private:	
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;
// Dialog Data
	//{{AFX_DATA(COptionDeckRebar)
	enum { IDD = IDD_OPTIONPAGE_DECKREBAR };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDeckRebar)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDeckRebar)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDECKREBAR_H__CDAB078C_B061_4C14_BB36_13A38892BEB4__INCLUDED_)
