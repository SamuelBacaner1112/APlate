// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SUBCROSSHEIGHTRISTDLG_H__FCE9B5C5_C296_43CB_B531_4D4A2FA07454__INCLUDED_)
#define AFX_SUBCROSSHEIGHTRISTDLG_H__FCE9B5C5_C296_43CB_B531_4D4A2FA07454__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubCrossHeightRistDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubCrossHeightRistDlg dialog
class CDetCrossBeamTypeDlg;
class CSubCrossHeightRistDlg : public CDialog
{
// Construction
public:
	CSubCrossHeightRistDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSubCrossHeightRistDlg)
	enum { IDD = IDD_DIALOG_SUB_CROSSBEAM_RIST };
	double	m_dDanJijum;
	double	m_dCenterJijum;
	double	m_dGen;
	//}}AFX_DATA
	CButton m_btnBase;
	CButton m_btnAll;
	CButton m_btnApply;

	CDetCrossBeamTypeDlg	*m_pCRBeamDlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubCrossHeightRistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	//}}AFX_VIRTUAL
	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubCrossHeightRistDlg)
	afx_msg void OnButtonCrosshRecommend();
	afx_msg void OnButtonCrosshApplyall();
	afx_msg void OnButtonCrosshApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBCROSSHEIGHTRISTDLG_H__FCE9B5C5_C296_43CB_B531_4D4A2FA07454__INCLUDED_)
