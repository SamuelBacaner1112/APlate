// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONGEN_H__B17D0E56_CBE6_4AE5_B158_C1E7E94349BB__INCLUDED_)
#define AFX_OPTIONGEN_H__B17D0E56_CBE6_4AE5_B158_C1E7E94349BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionGen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionGen dialog
#include "resource.h"
class COptionGen : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionGen)

// Construction
public:
	COptionGen();
	~COptionGen();

	CAPlateDrawStd	*m_pPlateDrawStd;

private:	
	void InitGridData(BOOL bDefaultLoad = FALSE);
	void InitGridDataPage(BOOL bDefaultLoad = FALSE);
	void InitGridDataTitle(BOOL bDefaultLoad = FALSE);	
	CCustomOptions m_Options;	

public:
// Dialog Data
	//{{AFX_DATA(COptionGen)
	enum { IDD = IDD_OPTIONPAGE_APLATEGEN };
	CGridCtrl m_Grid;
	CGridCtrl m_GridPage;
	CGridCtrl m_GridTitle;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionGen)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionGen)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONGEN_H__B17D0E56_CBE6_4AE5_B158_C1E7E94349BB__INCLUDED_)
