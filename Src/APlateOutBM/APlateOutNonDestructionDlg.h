// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_APLATEOUTNONDESTRUCTIONDLG_H__004DE063_2B15_4788_AF84_0D7775EB4A76__INCLUDED_)
#define AFX_APLATEOUTNONDESTRUCTIONDLG_H__004DE063_2B15_4788_AF84_0D7775EB4A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// APlateOutNonDestructionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutNonDestructionDlg dialog

class CAPlateOutBMStd;

class CAPlateOutNonDestructionDlg : public CDialog
{
// Construction
public:
	CAPlateOutNonDestructionDlg(CAPlateOutBMStd *pBMStd, CWnd* pParent = NULL);   // standard constructor

	CAPlateOutBMStd		*m_pBMStd;

	void SetWnd(CWnd* pWnd) { m_pWnd = pWnd;};
// Dialog Data
	//{{AFX_DATA(CAPlateOutNonDestructionDlg)
	enum { IDD = IDD_DIALOG_APLATE_NONDES };
	CProgressCtrl	m_ctrlProgress;
	CString	m_szProgressText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPlateOutNonDestructionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pWnd;
	// Generated message map functions
	//{{AFX_MSG(CAPlateOutNonDestructionDlg)
	afx_msg void OnButtonNonDesOut();
	afx_msg void OnButtonNonDesFileOpen();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnReceive(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLATEOUTNONDESTRUCTIONDLG_H__004DE063_2B15_4788_AF84_0D7775EB4A76__INCLUDED_)
