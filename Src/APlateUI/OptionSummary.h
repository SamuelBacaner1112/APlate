// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONSUMMARY_H__E4EE542C_28F6_494A_B3BA_E3C3D8088392__INCLUDED_)
#define AFX_OPTIONSUMMARY_H__E4EE542C_28F6_494A_B3BA_E3C3D8088392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSummary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionSummary dialog

class COptionSummary : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionSummary)

// Construction
public:
	void InitGridData(BOOL bDefaultLoad=FALSE);
	void InitTreeData(BOOL bDefaultLoad=FALSE);

	CCustomOptions m_Options;	

	COptionSummary();
	~COptionSummary();

	CAPlateDrawStd	*m_pPlateDrawStd;
	

// Dialog Data
	//{{AFX_DATA(COptionSummary)
	enum { IDD = IDD_OPTIONPAGE_SUMMARY };
	CCustomOptionsTreeCtrl	m_ListOption;
	CGridCtrl m_Grid;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionSummary)
	public:		
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionSummary)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSUMMARY_H__E4EE542C_28F6_494A_B3BA_E3C3D8088392__INCLUDED_)
