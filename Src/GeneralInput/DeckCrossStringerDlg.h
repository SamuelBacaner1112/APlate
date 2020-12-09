// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKCROSSSTRINGERDLG_H__C253C5F1_81F5_4A54_8C5A_C12E7B5892C8__INCLUDED_)
#define AFX_DECKCROSSSTRINGERDLG_H__C253C5F1_81F5_4A54_8C5A_C12E7B5892C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckCrossStringerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckCrossStringerDlg dialog

class CDeckCrossStringerDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckCrossStringerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckCrossStringerDlg)
	enum { IDD = IDD_DECK_065_CROSSSTRINGER };
	CXTTabCtrl	m_ctlTab;
	BOOL	m_bMakeC3;
	BOOL	m_bMakeS3;
	BOOL	m_bMakeOB3;
	BOOL	m_bShowSlabHunch;
	//}}AFX_DATA
	CGridCtrl	m_Grid;
	CButton		m_btnAdd;
	CButton		m_btnDel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckCrossStringerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckCrossStringerDlg)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDeleteRebar();
	afx_msg void OnButtonAddRebar();
	afx_msg void OnCheckC3();
	afx_msg void OnCheckS3();
	afx_msg void OnCheckOB3();
	afx_msg void OnCheckShowSlabHunch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL IsValid();
	void OnPreInitDialog();

	void SetGridTitle();
	void SetGridTitleBasic();		// ǥ���Է�
	void SetGridTitleCrSangse();	// ���κ� �����Է�
	void SetGridTitleStSangse();	// ���κ� �����Է�
	void SetGridTitleObkSangse();	// ���κ� �����Է�

	void SetDataDefault();
	
	void SetDataInit();
	void SetDataInitBasic();		// ǥ���Է�
	void SetDataInitCrSangse();		// ���κ� �����Է�
	void SetDataInitStSangse();		// ���κ� �����Է�
	void SetDataInitObkSangse();	// �������κ� �����Է�
	
	void SetDataSave();
	void SetDataSaveBasic();		// ǥ���Է�
	void SetDataSaveCrSangse();		// ���κ� �����Է�
	void SetDataSaveStSangse();		// ���κ� �����Է�
	void SetDataSaveObkSangse();	// �������κ� �����Է�

	void DrawPlanRebarCr(CDomyun* pDom, double dTextRatio);	// ��鵵
	void DrawPlanRebarSt(CDomyun* pDom, double dTextRatio);	// ��鵵
	void DrawPlanRebarObk(CDomyun* pDom, double dTextRatio);// ��鵵
	void DrawSangseRebar(CDomyun* pDom);	// ǥ���Էµ�
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);

	CString GetCurTabName();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKCROSSSTRINGERDLG_H__C253C5F1_81F5_4A54_8C5A_C12E7B5892C8__INCLUDED_)
