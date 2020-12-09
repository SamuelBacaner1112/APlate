// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONSLABGEN_H__D03F9151_E424_46A1_AA6E_9A36FBD927E4__INCLUDED_)
#define AFX_OPTIONSLABGEN_H__D03F9151_E424_46A1_AA6E_9A36FBD927E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSlabGen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionSlabGen dialog
#include "resource.h"
class COptionSlabGen : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionSlabGen)

// Construction
public:
	COptionSlabGen();   // standard constructor
	~COptionSlabGen();
	CAPlateDrawStd	*m_pPlateDrawStd;
private:	
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;
	
// Dialog Data
	//{{AFX_DATA(COptionSlabGen)
	enum { IDD = IDD_OPTIONPAGE_SLABGEN };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionSlabGen)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionSlabGen)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSLABGEN_H__D03F9151_E424_46A1_AA6E_9A36FBD927E4__INCLUDED_)
