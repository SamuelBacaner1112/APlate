// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONCAMBER_H__3CD72B1C_E3D8_4107_9066_7E7D20354348__INCLUDED_)
#define AFX_OPTIONCAMBER_H__3CD72B1C_E3D8_4107_9066_7E7D20354348__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionCamber.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionCamber dialog
#include "resource.h"
class COptionCamber : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionCamber)

// Construction
public:
	COptionCamber();
	~COptionCamber();

	CAPlateDrawStd	*m_pPlateDrawStd;
private:	
	CCustomOptions m_Options;
	
	void InitTreeData(BOOL bDefaultLoad=FALSE);
	void InitGridData(BOOL bDefaultLoad=FALSE);

public:
// Dialog Data
	//{{AFX_DATA(COptionCamber)
	enum { IDD = IDD_OPTIONPAGE_CAMBER };
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

#endif // !defined(AFX_OPTIONCAMBER_H__3CD72B1C_E3D8_4107_9066_7E7D20354348__INCLUDED_)
