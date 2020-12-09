// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EARTHQUACKDLG_H__823514BB_03B4_44E4_A786_3A28E48F2571__INCLUDED_)
#define AFX_EARTHQUACKDLG_H__823514BB_03B4_44E4_A786_3A28E48F2571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EarthQuackDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEarthQuackDlg dialog

class CEarthQuackDlg : public CUserDialog
{
// Construction
public:
	CEarthQuackDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEarthQuackDlg)
	enum { IDD = IDD_DIALOG_EARTHQUACK };
	int		m_nType;
	//}}AFX_DATA

	double m_dZoneCoefficient;	///< 구역계수
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEarthQuackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEarthQuackDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUACKDLG_H__823514BB_03B4_44E4_A786_3A28E48F2571__INCLUDED_)
