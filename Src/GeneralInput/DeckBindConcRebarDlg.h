// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKBINDCONCREBARDLG_H__4F3112C0_AA86_4A30_94E6_B60590DD3508__INCLUDED_)
#define AFX_DECKBINDCONCREBARDLG_H__4F3112C0_AA86_4A30_94E6_B60590DD3508__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckBindConcRebarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckBindConcRebarDlg dialog

class CDeckBindConcRebarDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckBindConcRebarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckBindConcRebarDlg)
	enum { IDD = IDD_DECK_063_BINDCONC };
	CGridCtrlMng	m_GridOpt;
	CGridCtrlMng	m_GridCTC;
	//}}AFX_DATA	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckBindConcRebarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckBindConcRebarDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg void OnCellChangedDataCTC(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedDataOpt(NMHDR*nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	BOOL IsValid();
	void OnPreInitDialog();
	void SetGridData();
	void SetGridDataOpt();
	void SetGridDataCTC();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);

	long GetTabCount();	
	CString GetTabTitle(long nIdx);

private:
	void DrawPlanHunchRebarBindConc(CDomyun *pDom);
	void DrawPlanRebarBindConc(CDomyun *pDom);
	void DrawJongRebarBindConc(CDomyun *pDom);
	void DrawCrossRebarBindConc(CDomyun *pDom);

	BOOL IsFocusItem(CString szTitle);
	void SetFocusItem(CString szTitle);
	void OnCursorChange(long nRow, long nCol);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKBINDCONCREBARDLG_H__4F3112C0_AA86_4A30_94E6_B60590DD3508__INCLUDED_)
