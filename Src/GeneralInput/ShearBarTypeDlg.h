// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHEARBARTYPEDLG_H__5E4EE761_4053_4A2B_980F_669D81ACC95D__INCLUDED_)
#define AFX_SHEARBARTYPEDLG_H__5E4EE761_4053_4A2B_980F_669D81ACC95D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShearBarTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShearBarTypeDlg dialog

class CShearBarTypeDlg : public CUserDialog
{
private:
	int m_nType;

// Construction
public:
	CShearBarTypeDlg(CWnd* pParent = NULL);   // standard constructor

//Method
public:
	int GetType();
	void SetType(int nType);


// Dialog Data
	//{{AFX_DATA(CShearBarTypeDlg)
	enum { IDD = IDD_DIALOG_SHEARBAR_TYPE };
	int		m_nRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShearBarTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShearBarTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEARBARTYPEDLG_H__5E4EE761_4053_4A2B_980F_669D81ACC95D__INCLUDED_)
