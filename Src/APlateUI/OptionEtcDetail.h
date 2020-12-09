// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONETCDETAIL_H__34AB3922_C969_4F0B_AA2B_ACE6292ABA32__INCLUDED_)
#define AFX_OPTIONETCDETAIL_H__34AB3922_C969_4F0B_AA2B_ACE6292ABA32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionEtcDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionEtcDetail dialog
#include "resource.h"
class COptionEtcDetail : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionEtcDetail)

// Construction
public:
	COptionEtcDetail();
	~COptionEtcDetail();

	CAPlateDrawStd	*m_pPlateDrawStd;

// Dialog Data
	//{{AFX_DATA(COptionEtcDetail)
	enum { IDD = IDD_OPTIONPAGE_ETCDETAIL };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionEtcDetail)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionEtcDetail)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
private:	
	CCustomOptions m_Options;
	
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	void InitGridData(BOOL bDefaultLoad=FALSE);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONETCDETAIL_H__34AB3922_C969_4F0B_AA2B_ACE6292ABA32__INCLUDED_)
