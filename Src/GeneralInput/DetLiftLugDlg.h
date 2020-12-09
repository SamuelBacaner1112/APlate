// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETLIFTLUGDLG_H__B04F2BDD_7FB2_4359_882C_EF2B9202602A__INCLUDED_)
#define AFX_DETLIFTLUGDLG_H__B04F2BDD_7FB2_4359_882C_EF2B9202602A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetLiftLugDlg.h : header file
//
#define TON_INDEX 4
/////////////////////////////////////////////////////////////////////////////
// CDetLiftLugDlg dialog
class CDetLiftLugDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetLiftLugDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetLiftLugDlg)
		enum { IDD = IDD_DET_11_LIFTLUG };
		CGridCtrl m_Grid;
	//}}AFX_DATA

	//그리드 특성칼럼 ---  칼럼수, 보강판, 편복부판
	enum {COL_COUNT=17, COL_ADDPLATE=8, COL_SIDEPLATE=9};
	//보강판 칼럼 --- 보강판반지름, 두께(L), 두께(R) 
	enum {COL_IR=10, COL_LT=13, COL_RT=14};
	//편복부판 칼렴 --- 두께(L), 두께(R), 폭(L), 폭(R) 
	enum {COL_SA=11, COL_SB=12, COL_LW=15, COL_RW=16};
	//I
	enum {COL_I = 0};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetLiftLugDlg)
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataDefaultAll(int nArrIndex);
	void SetDataSave();
	void SetGridTitle();
	void OnPreInitDialog();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetLiftLugDlg)
		afx_msg void OnButtonRecommend();
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);
	afx_msg LRESULT OnCellChangedRow(WPARAM nmgv, LPARAM);
	afx_msg LRESULT OnInsertedRow(WPARAM nmgv, LPARAM);
	afx_msg LRESULT OnDeletedRow(WPARAM, LPARAM);

protected:
	bool GetStateCombo(int nRow, int nCol);
	void SetDataDefaultInserted(int nArrIndex);
	void SetGridCombo(int nRow);
	void SetEnableGrid(int nRow);	//그리드 활성화 비활성화
	void SetEnableIndex();
protected:
	long	m_nCurIndex	;

	double	m_dI[TON_INDEX]		;
	double	m_dA[TON_INDEX]		;
	double	m_dB[TON_INDEX]		;
	double	m_dC[TON_INDEX]		;
	double	m_dD[TON_INDEX]		;
	double	m_dDia[TON_INDEX]	;
	double	m_dR[TON_INDEX]		;
	double	m_dT[TON_INDEX]		;
	BOOL	m_bStiff[TON_INDEX]	;
	BOOL	m_bWeb[TON_INDEX]	;
	double	m_dIR[TON_INDEX]	;
	double	m_dSA[TON_INDEX]	;
	double	m_dSB[TON_INDEX]	;
	double	m_dLT[TON_INDEX]	;
	double	m_dRT[TON_INDEX]	;
	double	m_dLW[TON_INDEX]	;
	double	m_dRW[TON_INDEX]	;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETLIFTLUGDLG_H__B04F2BDD_7FB2_4359_882C_EF2B9202602A__INCLUDED_)
