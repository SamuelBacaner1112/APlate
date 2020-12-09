// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERREBARDLG_H__25E5E40F_D01C_4D53_AC22_E661BF0FAD8A__INCLUDED_)
#define AFX_CALCCANTILEVERREBARDLG_H__25E5E40F_D01C_4D53_AC22_E661BF0FAD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverRebarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverRebarDlg dialog

class CCalcCantileverRebarDlg : public CGeneralBaseDlg
{
// Construction
public:	
	CCalcCantileverRebarDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_sRebar;

	void OnPreInitDialog();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void SetDataDefault();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	long GetRebarDiaIdx(CString szDia);
	CString GetRebar(long nIdx);
	CString GetRebar(CString szDia);
	CString GetCheck(BOOL bCheck);
	CString GetTitle();
	void SetActiveGrid();
	BOOL IsValid();
// Dialog Data
	//{{AFX_DATA(CCalcCantileverRebarDlg)
	enum { IDD = IDD_CALC_CANTILEVER_REBAR };
	CXTTabCtrl		m_TabRebar;
	CRichEditCtrl	m_richeditRebar;
	CGridCtrl		m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverRebarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcCantileverRebarDlg)
	afx_msg void OnSelchangeTabRebar(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedRow(NMHDR*nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
private:
	void reCalculate();

	CHARFORMAT m_cfDefault;
	CHARFORMAT m_cfNG;

	void SetRichEditRebarDesignCheck(enum FloorType eType, CRichEditCtrl &RichEdit);
	FloorType GetPosition(CString strTabName);
	void SetTextFont();
	void parse();
public:
	afx_msg void OnSelchangeRebarCombo();
	CComboBox m_comboRebar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERREBARDLG_H__25E5E40F_D01C_4D53_AC22_E661BF0FAD8A__INCLUDED_)
