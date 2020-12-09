// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONBMPAGE_H__D607AE27_B7B4_4EBF_A05D_86D7FDC19936__INCLUDED_)
#define AFX_OPTIONBMPAGE_H__D607AE27_B7B4_4EBF_A05D_86D7FDC19936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionBMPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionBMPage dialog
#include "resource.h"
class COptionBMPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionBMPage)

// Construction
public:
	COptionBMPage();   // standard constructor
	~COptionBMPage();
	CAPlateDrawStd	*m_pPlateDrawStd;

private:	
	void InitGridData(BOOL bDefaultLoad = FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);	
	CCustomOptions m_Options;	

public:
// Dialog Data
	//{{AFX_DATA(COptionBMPage)
	enum { IDD = IDD_OPTIONPAGE_BM };
	CGridCtrl m_Grid;
	CCustomOptionsTreeCtrl	m_ListOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionBMPage)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionBMPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONBMPAGE_H__D607AE27_B7B4_4EBF_A05D_86D7FDC19936__INCLUDED_)
