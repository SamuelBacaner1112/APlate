// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OptionPaint.h: interface for the COptionPaint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONPAINT_H__18162A08_7474_4289_987E_C313CE6AEA8D__INCLUDED_)
#define AFX_OPTIONPAINT_H__18162A08_7474_4289_987E_C313CE6AEA8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"


class COptionPaint : public CPropertyPage  
{
	DECLARE_DYNCREATE(COptionPaint)

// Construction
public:
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);

	CCustomOptions m_Options;	

	COptionPaint();
	~COptionPaint();

	CAPlateDrawStd	*m_pPlateDrawStd;

// Dialog Data
	//{{AFX_DATA(COptionPaint)
	enum { IDD = IDD_OPTIONPAGE_PAINT };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionPaint)
	public:		
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionPaint)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OPTIONPAINT_H__18162A08_7474_4289_987E_C313CE6AEA8D__INCLUDED_)
