// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDETAILCYCLESUDLG_H__07221806_FB0E_4EE1_94B4_ECC29F5F7957__INCLUDED_)
#define AFX_DECKDETAILCYCLESUDLG_H__07221806_FB0E_4EE1_94B4_ECC29F5F7957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDetailCyclesuDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailCyclesuDlg dialog

class CADeckData;
class CDeckDetailCyclesuDlg : public CUserDialog
{
// Construction
public:
	CDeckDetailCyclesuDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDetailCyclesuDlg)
	enum { IDD = IDD_DECKDETAIL_CYCLESU };
	int		m_nCyclesu;
	//}}AFX_DATA
	CADeckData	*m_pDeckData;
	CButton		m_btnOk;
	CButton		m_btnCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDetailCyclesuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDetailCyclesuDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDETAILCYCLESUDLG_H__07221806_FB0E_4EE1_94B4_ECC29F5F7957__INCLUDED_)
