// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONLINEINFO_H__30C2A054_B0AD_487E_90A2_7B2A59177F94__INCLUDED_)
#define AFX_OPTIONLINEINFO_H__30C2A054_B0AD_487E_90A2_7B2A59177F94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionLineInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionLineInfo dialog
#include "resource.h"

class COptionLineInfo : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionLineInfo)

// Construction
public:
	COptionLineInfo();
	~COptionLineInfo();
	
	CAPlateDrawStd	*m_pPlateDrawStd;
	
// Dialog Data
	//{{AFX_DATA(COptionLineInfo)
	enum { IDD = IDD_OPTIONPAGE_LINEINFO };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionLineInfo)
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
	//{{AFX_MSG(COptionLineInfo)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONLINEINFO_H__30C2A054_B0AD_487E_90A2_7B2A59177F94__INCLUDED_)
