// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDETAILMAINLOWERDLG_H__CB1F5CBE_274B_4B20_BA96_0354E16B0E98__INCLUDED_)
#define AFX_DECKDETAILMAINLOWERDLG_H__CB1F5CBE_274B_4B20_BA96_0354E16B0E98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDetailMainLowerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainLowerDlg dialog

class CADeckData;
class CDeckDetailMainLowerDlg : public CUserDialog
{
// Construction
public:
	CDeckDetailMainLowerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDetailMainLowerDlg)
	enum { IDD = IDD_DECKDETAIL_MAINLOWER };

	CEdit	m_ctrlLengthDolchulLoDanLeft;
	CEdit	m_ctrlLengthDolchulLoDanRigh;
	CEdit	m_ctrlLengthDolchulLoGenLeft;
	CEdit	m_ctrlLengthDolchulLoGenRigh;
	int		m_nType;
	double	m_dLengthDolchulLoDanLeft;
	double	m_dLengthDolchulLoDanRigh;
	double	m_dLengthDolchulLoGenLeft;
	double	m_dLengthDolchulLoGenRigh;
	//}}AFX_DATA
	CADeckData *m_pDeckData;
	CButton		m_btnOk;
	CButton		m_btnCancel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDetailMainLowerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDetailMainLowerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioJiksunType();
	afx_msg void OnRadioAngleType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDETAILMAINLOWERDLG_H__CB1F5CBE_274B_4B20_BA96_0354E16B0E98__INCLUDED_)
