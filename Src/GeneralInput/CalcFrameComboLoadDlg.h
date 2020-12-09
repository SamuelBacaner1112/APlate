// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//
#if !defined(AFX_CALCFRAMECOMBOLOADDLG_H__6E35F26F_05A7_4ECC_89F6_238853E4F320__INCLUDED_)
#define AFX_CALCFRAMECOMBOLOADDLG_H__6E35F26F_05A7_4ECC_89F6_238853E4F320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcFrameComboLoadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcFrameComboLoadDlg dialog
class CCalcFrameComboLoadDlg : public CGeneralBaseDlg
{
// Construction
public:
	CCalcFrameComboLoadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcFrameComboLoadDlg)
	enum { IDD = IDD_CALC_FRAME_COMBO };
	CXTTabCtrl	m_ctlTab;
	//}}AFX_DATA
	CButton	m_btnAddCombo;
	CButton	m_btnDelCombo;
	CButton	m_btnUpper;
	CButton	m_btnLower;
	CButton	m_btnInsertCombo;
	CButton	m_btnRemoveCombo;
	long	m_nSelectedCombo;

protected:
	CGridCtrl	m_GridCombo;
	CGridCtrl	m_GridLoad;
	CGridCtrl	m_GridStd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcFrameComboLoadDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:	
	void OnPreInitDialog();
	
private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	BOOL ErrorMessage(long nErr, long nVal);
	void SetGridTitleStd();
	void SetGridTitleLoad();
	void SetGridTitleCombo();
	void GridCommon(CGridCtrl *pGrid, long nQtyRow, long nQtyCol);
	void SetDataInit();
	void SetDataInitStd();
	void SetDataInitCombo();
	void SetDataInitLoad();
	void SetDataSave();
	void SetDataDefault();	
	BOOL SwapCombo(BOOL bUpper);
	BOOL SwapLoad(CCombo* pCombo, long nIdx1, BOOL bUpper);
	long GetnTypeAnalysis(long nCurSel);
	void RemoveLCMinMaxData();

protected:
	// Generated message map functions
	//{{AFX_MSG(CCalcFrameComboLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAddCombo();
	afx_msg void OnButtonDeleteCombo();
	afx_msg void OnButtonInsertcombo();
	afx_msg void OnButtonRemovecombo();
	afx_msg void OnButtonUpper();
	afx_msg void OnButtonLower();
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataLoad(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataCombo(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCFRAMECOMBOLOADDLG_H__6E35F26F_05A7_4ECC_89F6_238853E4F320__INCLUDED_)
