// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDETAILMAINUPPERDLG_H__5FB0348C_E697_4CFE_8943_692585720134__INCLUDED_)
#define AFX_DECKDETAILMAINUPPERDLG_H__5FB0348C_E697_4CFE_8943_692585720134__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDetailMainUpperDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainUpperDlg dialog

class CADeckData;
class CDeckDetailMainUpperDlg : public CUserDialog
{
// Construction
public:
	CDeckDetailMainUpperDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDetailMainUpperDlg)
	enum { IDD = IDD_DECKDETAIL_MAINUPPER };
	int		m_nType;
	//}}AFX_DATA
	CADeckData *m_pDeckData;
	CButton		m_btnOk;
	CButton		m_btnCancel;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDetailMainUpperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDetailMainUpperDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDETAILMAINUPPERDLG_H__5FB0348C_E697_4CFE_8943_692585720134__INCLUDED_)
