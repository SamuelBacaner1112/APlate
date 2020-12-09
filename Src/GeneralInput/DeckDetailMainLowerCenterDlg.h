// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDETAILMAINLOWERCENTERDLG_H__76430E07_ADDC_4DE5_B96C_03CCA8BC97C4__INCLUDED_)
#define AFX_DECKDETAILMAINLOWERCENTERDLG_H__76430E07_ADDC_4DE5_B96C_03CCA8BC97C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDetailMainLowerCenterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainLowerCenterDlg dialog

class CADeckData;
class CDeckDetailMainLowerCenterDlg : public CUserDialog
{
// Construction
public:
	CDeckDetailMainLowerCenterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDetailMainLowerCenterDlg)
	enum { IDD = IDD_DECKDETAIL_MAINLOWERCENTER };
	int		m_nType;
	//}}AFX_DATA
	CADeckData *m_pDeckData;
	CButton		m_btnOk;
	CButton		m_btnCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDetailMainLowerCenterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDetailMainLowerCenterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDETAILMAINLOWERCENTERDLG_H__76430E07_ADDC_4DE5_B96C_03CCA8BC97C4__INCLUDED_)
