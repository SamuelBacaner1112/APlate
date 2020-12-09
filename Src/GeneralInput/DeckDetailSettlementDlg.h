// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDETAILSETTLEMENTDLG_H__131A28D2_31F9_4F09_8F71_4B1CDD8B08E6__INCLUDED_)
#define AFX_DECKDETAILSETTLEMENTDLG_H__131A28D2_31F9_4F09_8F71_4B1CDD8B08E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDetailSettlementDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailSettlementDlg dialog

class CDeckDetailSettlementDlg : public CUserDialog
{
// Construction
public:
	CDeckDetailSettlementDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDetailSettlementDlg)
	enum { IDD = IDD_DECKDETAIL_SETTLEMENT };
	int		m_nType;
	//}}AFX_DATA
	CADeckData	*m_pDeckData;
	CButton		m_btnOk;
	CButton		m_btnCancel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDetailSettlementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDetailSettlementDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDETAILSETTLEMENTDLG_H__131A28D2_31F9_4F09_8F71_4B1CDD8B08E6__INCLUDED_)
