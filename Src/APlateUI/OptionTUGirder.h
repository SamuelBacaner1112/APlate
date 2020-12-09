// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONTUGIRDER_H__C947F5CE_3716_425F_8D18_4A7D1BC2E70C__INCLUDED_)
#define AFX_OPTIONTUGIRDER_H__C947F5CE_3716_425F_8D18_4A7D1BC2E70C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionTUGirder.h: header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionTUGirder dialog
#include "resource.h"
class COptionTUGirder : public CPropertyPage  
{
	DECLARE_DYNCREATE(COptionTUGirder)

// Construction
public:
	COptionTUGirder();
	~COptionTUGirder();

	CAPlateDrawStd	*m_pPlateDrawStd;

private:	
	CCustomOptions m_Options;
	
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	void InitGridData(BOOL bDefaultLoad=FALSE);

public:
// Dialog Data
	//{{AFX_DATA(COptionCamber)
	enum { IDD = IDD_OPTIONPAGE_TUGIRDER };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionCamber)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionCamber)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDefault();
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTUGIRDER_H__C947F5CE_3716_425F_8D18_4A7D1BC2E70C__INCLUDED_)
