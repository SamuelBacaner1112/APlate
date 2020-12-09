// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDETAILDIVIDEREBAR_H__BBD3A30A_8242_4B62_BFA9_5FB9D76099AB__INCLUDED_)
#define AFX_DECKDETAILDIVIDEREBAR_H__BBD3A30A_8242_4B62_BFA9_5FB9D76099AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDetailDivideRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailDivideRebar dialog

class CADeckData;
class CDeckDetailDivideRebar : public CUserDialog
{
// Construction
public:
	CDeckDetailDivideRebar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDetailDivideRebar)
	enum { IDD = IDD_DECKDETAIL_DIVIDEREBAR };
	int		m_nDivideRebar;
	//}}AFX_DATA
	CADeckData *m_pDeckData;
	CButton		m_btnOk;
	CButton		m_btnCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDetailDivideRebar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDetailDivideRebar)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioDivide();
	afx_msg void OnRadioNoDivide();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDETAILDIVIDEREBAR_H__BBD3A30A_8242_4B62_BFA9_5FB9D76099AB__INCLUDED_)
