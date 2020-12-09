// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONDETAILDIM_H__7BB12FAB_15F4_4D1F_A34F_6C04386D5517__INCLUDED_)
#define AFX_OPTIONDETAILDIM_H__7BB12FAB_15F4_4D1F_A34F_6C04386D5517__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDetailDim.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionDetailDim dialog
#include "resource.h"
class COptionDetailDim : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionDetailDim)

// Construction
public:
	COptionDetailDim();
	~COptionDetailDim();

	CAPlateDrawStd	*m_pPlateDrawStd;

private:	
	CCustomOptions m_PyungOptions;
	CCustomOptions m_JongOptions;	

public:
	void InitData(BOOL bDefaultLoad=FALSE);

// Dialog Data
	//{{AFX_DATA(COptionDetailDim)
	enum { IDD = IDD_OPTIONPAGE_DETAIL_DIM };
	CCustomOptionsTreeCtrl	m_OptionTreeLeft;
	CCustomOptionsTreeCtrl	m_OptionTreeRight;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionDetailDim)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionDetailDim)
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

#endif // !defined(AFX_OPTIONDETAILDIM_H__7BB12FAB_15F4_4D1F_A34F_6C04386D5517__INCLUDED_)
