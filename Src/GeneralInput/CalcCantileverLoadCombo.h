// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERLOADCOMBO_H__8A18BA55_19EB_47B9_B981_B28FFF24B5AB__INCLUDED_)
#define AFX_CALCCANTILEVERLOADCOMBO_H__8A18BA55_19EB_47B9_B981_B28FFF24B5AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverLoadCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverLoadCombo dialog

class CCalcCantileverLoadCombo : public CGeneralBaseDlg
{
// Construction
public:
	CCalcCantileverLoadCombo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcCantileverLoadCombo)
	enum { IDD = IDD_CALC_CANTILEVER_LOADCOMBO };
	//}}AFX_DATA
	CButton	m_btnAddCombo;
	CButton	m_btnDelCombo;
	CButton	m_btnUpper;
	CButton	m_btnLower;
	CButton	m_btnInsertCombo;
	CButton	m_btnRemoveCombo;
	
	CGridCtrl	m_GridCombo;
	CGridCtrl	m_GridLoad;
	CGridCtrl	m_GridStd;
	long		m_nSelectedCombo;

	vector<BOOL> m_bCurrentLC;
	vector<BOOL> m_bOriginalLC;
	CStringArray m_szNameLC;

	vector<BOOL>	m_Comb_bApply;
	vector<CString>	m_Comb_szName;
	vector<CString>	m_Comb_szDescription;
	vector<long>	m_Comb_nEnvType;
	vector<BOOL>	m_Comb_bLive;

	CLongArray		m_Comb_Idx1;
	CDWordArray		m_Comb_Arr1;
	CLongArray		m_Comb_Idx2;
	CStringArray	m_Comb_Arr2;
	CLongArray		m_Comb_Idx3;
	CDoubleArray	m_Comb_Arr3;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverLoadCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	BOOL IsValid();
	void OnPreInitDialog();

private:
	void SetDataSave();
	void SetDataDefault();
	void SetDataInit();
	void SetDataInitCombo();
	void SetDataInitLoad();
	void SetDataInitStd();
	void SetGridTitleCombo();
	void SetGridTitleLoad();
	void SetGridTitleStd();

	BOOL SwapCombo(BOOL bUpper);
	BOOL SwapLoad(CCombo* pCombo, long nIdx1, BOOL bUpper);
	BOOL ErrorMessage(long nErr, long nVal);

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void GridCommon(CGridCtrl *pGrid, long nQtyRow, long nQtyCol);
	long GetStaticLoadCase();

	void CheckLoadComboApply();
	void CheckLoadCaseApply();
	long GetLoadApplyMemberCount();
	long GetLoadCaseIndex(CString szLoad);
	void SetComboToLoadCaseApply(CCombo* pComb);
	void SetLoadCaseApply();
	void SetOriginalComboData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcCantileverLoadCombo)
	//}}AFX_MSG
	virtual BOOL OnInitDialog();

	afx_msg void OnButtonAddCombo();
	afx_msg void OnButtonDeleteCombo();
	afx_msg void OnButtonUpper();
	afx_msg void OnButtonLower();
	afx_msg void OnButtonInsertcombo();
	afx_msg void OnButtonRemovecombo();

	afx_msg void OnCellChangedRowCombo(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataLoad(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataCombo(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERLOADCOMBO_H__8A18BA55_19EB_47B9_B981_B28FFF24B5AB__INCLUDED_)
