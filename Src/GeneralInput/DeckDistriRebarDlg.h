// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKDISTRIREBARDLG_H__2C06E542_DC2B_4300_B755_30A5C9C342CE__INCLUDED_)
#define AFX_DECKDISTRIREBARDLG_H__2C06E542_DC2B_4300_B755_30A5C9C342CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDistriRebarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckDistriRebarDlg dialog

class CDeckDistriRebarDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckDistriRebarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data 
	//{{AFX_DATA(CDeckDistriRebarDlg)
	enum { IDD = IDD_DECK_05_DISTRI_REBAR };
	CXTTabCtrl	m_ctrlTab;
	CComboBox	m_ctrlComboCycle;
	CComboBox	m_ctrlComboDanBogangArrangeMethod;
	CComboBox	m_ctrlComboUseJujangRebar;
	CComboBox	m_ctrlComboJijumBogangArrange;
	CGridCtrlMng m_Grid;
	CString	m_szScale;
	CString	m_szA1;
	CString	m_szA2;
	CString	m_szB1;
	CString	m_szB2;
	CString	m_szC1;
	CString	m_szC2;
	BOOL	m_bShowDia;
	//}}AFX_DATA
	CButton	m_btnSameTap;
	CButton	m_btnReCalc;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDistriRebarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDistriRebarDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboDanBogangArrangeMethod();
	afx_msg void OnSelchangeComboUseJujangRebar();
	afx_msg void OnSelchangeComboJijumMainStyle();
	afx_msg void OnSelchangeComboViewCycle();
	afx_msg void OnButtonSameByCurTap();
	afx_msg void OnButtonRecalcShape();
	afx_msg void OnCheckShowDia();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);		
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
	
	int	m_nScaleGrade;

public:
	BOOL m_bResetCombo;
	BOOL IsValid();
	void InitTabCtrl();
	void InitComboCycle();
	void OnPreInitDialog();
	void SetGridData();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	long GetJongBasePosByCurTab();
	CString GetCurTabName();
	void SetViewControl();
	void OnCursorChange(long nRow, long nCol);
	long GetTabCount();	
	CString GetTabTitle(long nIdx);
	long GetCurGirderNo();

	void InsertJointByGridCol(long nGridCol);
	void DeleteJointByGridCol(long nGridCol);
	void ReMakeCycleByChangedData(long nJongBasePos, long nCycle, long nShape, BOOL bUpper, double dNewLength);

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	void DrawMomentDiagmDeadLoad(CDomyun *pDom, long nG, CDoubleArray &dArrX);

	CDoubleArray GetArrJongMomentPos(long nG);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDISTRIREBARDLG_H__2C06E542_DC2B_4300_B755_30A5C9C342CE__INCLUDED_)
