// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_REBARADDRATEDLG_H__B9ABC018_62B1_4467_814B_05BA973F94FE__INCLUDED_)
#define AFX_REBARADDRATEDLG_H__B9ABC018_62B1_4467_814B_05BA973F94FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarAddRateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarAddRateDlg dialog
class CAddRate;
#include "Resource.h"

class CRebarAddRateDlg : public CDialog
{
// Construction
public:
	CRebarAddRateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarAddRateDlg)
	enum { IDD = IDD_DIALOG_ADDRATE };
	CComboBox	m_comboDia;
	double	m_dRate1;
	double	m_dRate2;
	//}}AFX_DATA
	CButton	m_btnRecommend;
	CButton	m_btnOk;
	CButton	m_btnCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarAddRateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarAddRateDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDia();
	afx_msg void OnButtonRecommend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStringArray m_strArrDia;
	CAddRate m_AddRate;
	int m_nCurSelDia;

public:
	CAddRate GetAddRate();
	void SetAddRate(CAddRate AddRate);
	int GetDiaIdx(double dDia);
	void SetStringArrDia();
	void SetComboCurDia();		//콤보박스를 현재 경계 직경으로 설정한다.
	double GetCurSelDia();
	void SetBoundaryDia(double dDia);
	void SetStaticName();
	void SetDataInit();
	double GetBoundaryDia();
	void SetDataInitComboDia();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARADDRATEDLG_H__B9ABC018_62B1_4467_814B_05BA973F94FE__INCLUDED_)
