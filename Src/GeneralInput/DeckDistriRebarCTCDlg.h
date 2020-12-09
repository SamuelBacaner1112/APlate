// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDISTRIREBARCTCDLG_H__BD530E42_0792_43BB_8F01_2B47E008BA2D__INCLUDED_)
#define AFX_DECKDISTRIREBARCTCDLG_H__BD530E42_0792_43BB_8F01_2B47E008BA2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDistriRebarCTCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDistriRebarCTCDlg dialog

class CDeckDistriRebarCTCDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckDistriRebarCTCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDistriRebarCTCDlg)
	enum { IDD = IDD_DECK_04_DISTRI_REABR_CTC };
	CComboBox	m_comboExtra;
	CXTTabCtrl	m_ctlTab;	
	CString	m_szDistriDiaRangeOnCant;
	CString	m_szGirderCenRange;
	CString	m_szDistriRebarDupliRange;
	//}}AFX_DATA
	CGridCtrlMng m_GridUp;
	CGridCtrlMng m_GridDn;

	CButton	m_btnReCalc;
	CButton	m_btnCurSameCTC;
	CButton	m_btnUpDnSameCTC;
	CButton	m_btnAdd;
	CButton	m_btnDelete;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDistriRebarCTCDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDeckDistriRebarCTCDlg)
	afx_msg void OnSelchangeTabPosition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabPosition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboExtra();
	afx_msg void OnButtonCtcRecalc();
	afx_msg void OnButtonSameCTCByCurtab();
	afx_msg void OnButtonSameCTCByUpdn();
	afx_msg void OnButtonAddCTC();
	afx_msg void OnButtonDeleteCTC();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	afx_msg void OnCellChangedDataUp(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedUp(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedDataDn(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedDn(NMHDR*nmgv,LRESULT*);

	afx_msg void OnCellInsertedColUp(NMHDR*nmgv,LRESULT*);	
	afx_msg void OnCellInsertedColDn(NMHDR*nmgv,LRESULT*);	
	afx_msg void OnCellDeletedColUp(NMHDR*nmgv,LRESULT*);	
	afx_msg void OnCellDeletedColDn(NMHDR*nmgv,LRESULT*);	

	DECLARE_MESSAGE_MAP()

protected:
	void MakeDimArrAtDanJijum(CDPointArray &xyArrDim, BOOL bUpper);
	void MakeDimArrAtCenter();
	double MakeDimArrAtCenterUp(CDPointArray &xyArrDim);	
	void MakeDimArrAtCenterDn(CDPointArray &xyArrDim, double dExtLength);	
	
public:
	void InitTabCtrl();
	void OnPreInitDialog();
	void SetGridDataUp();
	void SetGridDataDn();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	long GetSectionIDByCurTab();
	long GetSectionIDByTabName(CString sTab);
	CString GetCurTabName();
	CString GetTabName(long nTab);
	void InsertCTCByGridCol(long nGridCol, BOOL bUpper);
	void DeleteCTCByGridCol(long nGridCol, BOOL bUpper);
	void OnCursorChange(long nRow, long nCol);

	BOOL IsValid();

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDISTRIREBARCTCDLG_H__BD530E42_0792_43BB_8F01_2B47E008BA2D__INCLUDED_)
