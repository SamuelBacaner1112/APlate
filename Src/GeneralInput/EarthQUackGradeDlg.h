// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EARTHQUACKGRADEDLG_H__0D3D2F8C_4D0E_4DB8_8448_B06F7C8715A7__INCLUDED_)
#define AFX_EARTHQUACKGRADEDLG_H__0D3D2F8C_4D0E_4DB8_8448_B06F7C8715A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EarthQUackGradeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEarthQUackGradeDlg dialog

class CEarthQUackGradeDlg : public CUserDialog
{
// Construction
public:
	CEarthQUackGradeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEarthQUackGradeDlg)
	enum { IDD = IDD_DIALOG_EARTHQUACKGRADE };
	int		m_nType;
	//}}AFX_DATA
	double m_dRiskCoefficient;		///< 위험도 계수

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEarthQUackGradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEarthQUackGradeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUACKGRADEDLG_H__0D3D2F8C_4D0E_4DB8_8448_B06F7C8715A7__INCLUDED_)
