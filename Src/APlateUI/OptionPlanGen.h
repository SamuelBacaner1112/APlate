// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONPLANGEN_H__F9DC1AAA_3F34_48F8_8046_C544A126C837__INCLUDED_)
#define AFX_OPTIONPLANGEN_H__F9DC1AAA_3F34_48F8_8046_C544A126C837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionPlanGen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionPlanGen dialog
#include "resource.h"

class COptionPlanGen : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionPlanGen)

// Construction
public:
	COptionPlanGen();
	~COptionPlanGen();
	
	CAPlateDrawStd	*m_pPlateDrawStd;

// Dialog Data
	//{{AFX_DATA(COptionPlanGen)
	enum { IDD = IDD_OPTIONPAGE_PLANGEN };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPlanGen)
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
	//{{AFX_MSG(COptionPlanGen)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONPLANGEN_H__F9DC1AAA_3F34_48F8_8046_C544A126C837__INCLUDED_)
