// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ShoeZapyoDlg.h: interface for the CShoeZapyoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOEZAPYODLG_H__EBB93027_D2A4_4C94_AF77_0C5108E39746__INCLUDED_)
#define AFX_SHOEZAPYODLG_H__EBB93027_D2A4_4C94_AF77_0C5108E39746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class AFX_EXT_CLASS CShoeZapyoDlg : public CShBaseDialog  
class AFX_EXT_CLASS CShoeZapyoDlg : public CXTResizeDialog  
{
// Construction
public:
	CShoeZapyoDlg(CWnd*pParent = NULL);   // standard constructor

// Attributes
protected:

public:
	CObject*	m_pDB;				// CPlateBridgeApp
	CObject*	m_pLineInfoApp;		// CLineInfoApp

// Dialog Data
	//{{AFX_DATA(CShoeZapyoDlg)
	enum { IDD = IDD_SHOEZAPYO_DLG };
	CGridCtrl	m_Grid;
	//}}AFX_DATA
	CButton	m_btnOutExcel;
	CButton	m_btnOutPrint;
	CButton	m_btnClose;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShoeZapyoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetGridText();
	void SetGridAlign();
	void SetGrid();

	// Generated message map functions
	//{{AFX_MSG(CShoeZapyoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShoeFileout();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()};

#endif // !defined(AFX_SHOEZAPYODLG_H__EBB93027_D2A4_4C94_AF77_0C5108E39746__INCLUDED_)
