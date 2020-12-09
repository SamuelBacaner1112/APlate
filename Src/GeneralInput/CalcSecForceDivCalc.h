// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCSECFORCEDIVCALC_H__8090D032_27A9_4FC9_8F48_11429B1B5ECB__INCLUDED_)
#define AFX_CALCSECFORCEDIVCALC_H__8090D032_27A9_4FC9_8F48_11429B1B5ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcSecForceDivCalc.h : header file
//
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CCalcSecForceDivCalc dialog
class CCalcSecForceDivCalc : public CGeneralBaseDlg
{	
// Construction
public:
	CCalcSecForceDivCalc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcSecForceDivCalc)
	enum { IDD = IDD_CALC_SECFORCE_DIV_CALC };
 	CXTTabCtrl		m_ctlTab;
	//}}AFX_DATA
 	CGridCtrlMng	m_Grid;
	CXTFlatComboBox	m_ctrlHLLStation;
	CButton		m_btnDelLoad;
	CButton		m_btnAddLoad;
	CButton		m_ApplyForce;
	CButton		m_btnLiveLoadPre;

	int		m_nHLoadExePos;
	int		m_nLoadCase;
	long	m_nSelectElem;
	BOOL	m_bSetHLLCurSel;
	CBOOLArray	m_bCheckArr;

public:
	vector <vector<double> > m_LoadPoint;	// 치수표현을 위한 배열
	vector <vector<double> > m_LoadPoint_Dead;

	void SetDataInit();
	void SetDataDefault();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

private:
	void SetDataSave();
	void OnPreInitDialog();
	void InitTabCtrl();
	void InsertTabItem(long nItem, CString szLoadCase);
	BOOL ExistLoadCase(CString szLoadCase);
	void SetLoadName();
	void SetComboData();
	CString GetTabName();
	long GetForceIndex(long nElemCur, long nCurRow, BOOL bTrap);
	void DeleteForcePoint(CLoadForce *pForce, long nIndex, BOOL bDelete);
	void DeleteForceTrap(CLoadForce *pForce, long nIndex, BOOL bDelete);

	enum {PREDEAD, POSTDEAD, LIVE};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcSecForceDivCalc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcSecForceDivCalc)
	afx_msg void OnLiveloadpreSelect();
	//}}AFX_MSG
	afx_msg void OnRadioPredead();
	afx_msg void OnRadioPostdead();
	afx_msg void OnRadioLive();
	afx_msg void OnButtonAddload();
	afx_msg void OnButtonDelload();
	afx_msg void OnCheckApplyForce();
	afx_msg void OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeComboSpan();
	afx_msg void OnDeltaposSpinSpan(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCSECFORCEDIVCALC_H__8090D032_27A9_4FC9_8F48_11429B1B5ECB__INCLUDED_)
