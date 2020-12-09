// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONWELDMAP_H__F07319C3_4B87_4C88_A3DD_5BE48FBC801A__INCLUDED_)
#define AFX_OPTIONWELDMAP_H__F07319C3_4B87_4C88_A3DD_5BE48FBC801A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionWeldMap.h : header file
//
 
/////////////////////////////////////////////////////////////////////////////
// COptionWeldMap dialog
#include "resource.h"

class COptionWeldMap : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionWeldMap)

// Construction
public:
	COptionWeldMap();
	~COptionWeldMap();
	
	CAPlateDrawStd	*m_pPlateDrawStd;

// Dialog Data
	//{{AFX_DATA(COptionWeldMap)
	enum { IDD = IDD_OPTIONPAGE_WELDMAP };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionWeldMap)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:	
	CCustomOptions m_Options;
	
	void InitData(BOOL bDefaultLoad=FALSE);
	void InitGridData(BOOL bDefaultLoad=FALSE);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionWeldMap)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONWELDMAP_H__F07319C3_4B87_4C88_A3DD_5BE48FBC801A__INCLUDED_)
