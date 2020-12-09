// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONGENERALDIM_H__D2E56679_1E69_40D4_8A9F_6EDFC15FE579__INCLUDED_)
#define AFX_OPTIONGENERALDIM_H__D2E56679_1E69_40D4_8A9F_6EDFC15FE579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionGeneralDim.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionGeneralDim dialog
#include "resource.h"
class COptionGeneralDim : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionGeneralDim)

// Construction
public:
	COptionGeneralDim();
	~COptionGeneralDim();

	CAPlateDrawStd	*m_pPlateDrawStd;

private:	
	CCustomOptions m_PyungOptions;
	CCustomOptions m_JongOptions;	

public:
	void InitData(BOOL bDefaultLoad=FALSE);

// Dialog Data
	//{{AFX_DATA(COptionGeneralDim)
	enum { IDD = IDD_OPTIONPAGE_GENERAL_DIM };
	CCustomOptionsTreeCtrl	m_OptionTreeLeft;
	CCustomOptionsTreeCtrl	m_OptionTreeRight;	
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionGeneralDim)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionGeneralDim)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	afx_msg void OnDefault1();
	afx_msg void OnDefault2();
	afx_msg void OnDefault3();
	afx_msg void OnDefault4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONGENERALDIM_H__D2E56679_1E69_40D4_8A9F_6EDFC15FE579__INCLUDED_)
