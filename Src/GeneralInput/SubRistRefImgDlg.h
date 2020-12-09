// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SUBRISTREFIMGDLG_H__E3FDF612_2844_462A_B275_21AE8FF08BF1__INCLUDED_)
#define AFX_SUBRISTREFIMGDLG_H__E3FDF612_2844_462A_B275_21AE8FF08BF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubRistRefImgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubRistRefImgDlg dialog

class CSubRistRefImgDlg : public CDialog
{
// Construction
public:
	CSubRistRefImgDlg(long nType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSubRistRefImgDlg)
	enum { IDD = IDD_DIALOG_RIST_REFIMG };
	CTabCtrl	m_ctlTab;
	//}}AFX_DATA
	CGridCtrl m_Grid;
	long m_nType;
	CButton m_btnCancel;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubRistRefImgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubRistRefImgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabReftype(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
		
public:
	void SetGridTitleFlangeWidth();
	void SetGridTitleFlangeLength();
	void SetGridTitleFlangeThick();
	void SetDataInit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBRISTREFIMGDLG_H__E3FDF612_2844_462A_B275_21AE8FF08BF1__INCLUDED_)
