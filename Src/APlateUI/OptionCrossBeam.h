// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OptionCrossBeam.h: interface for the COptionCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONCROSSBEAM_H__92B04616_28BD_4177_8CA4_7045E957134F__INCLUDED_)
#define AFX_OPTIONCROSSBEAM_H__92B04616_28BD_4177_8CA4_7045E957134F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"

class COptionCrossBeam : public CPropertyPage  
{
	DECLARE_DYNCREATE(COptionCrossBeam)

// Construction
public:
	COptionCrossBeam();
	~COptionCrossBeam();

	CAPlateDrawStd	*m_pPlateDrawStd;
public:
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	CCustomOptions m_Options;
	


// Dialog Data
	//{{AFX_DATA(COptionCrossBeam)
	enum { IDD = IDD_OPTIONPAGE_PLATECROSSBEAM };
	CGridCtrl m_Grid;
	CCustomOptionsTreeCtrl	m_ListOption;		
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionCrossBeam)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionCrossBeam)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

#endif // !defined(AFX_OPTIONCROSSBEAM_H__92B04616_28BD_4177_8CA4_7045E957134F__INCLUDED_)
