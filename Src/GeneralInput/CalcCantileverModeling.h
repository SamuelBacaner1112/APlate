// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERMODELING_H__3887F3EB_341A_415C_892E_4BBD4B19B760__INCLUDED_)
#define AFX_CALCCANTILEVERMODELING_H__3887F3EB_341A_415C_892E_4BBD4B19B760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverModeling.h : header file
//
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverModeling dialog

class CCalcCantileverModeling : public CGeneralBaseDlg
{
// Construction
public:
	CCalcCantileverModeling(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcCantileverModeling)
	enum { IDD = IDD_CALC_CANTILEVER_MODELING };
	//}}AFX_DATA
	CXTTabCtrl	m_ctlTab;
	CButton		m_btnDelLoad;
	CButton		m_btnAddLoad;
	CButton		m_ApplyForce;

	int				m_nLoadCase;
	long			m_nSelectElem;
	CGridCtrlMng	m_Grid;
	CBOOLArray		m_bCheckArr;

public:
	BOOL IsValid();
	void SetDataInit();
	void SetDataDefault();
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);

private:
	void SetDataSave();
	void OnPreInitDialog();
	void InsertTabItem(long nItem, CString szLoadCase);

	CString GetTabName();
	long	GetLoadCase(CString szLoadCase);
	void	InitTabCtrl();

	long GetForceIndex(long nElemCur, long nCurRow, BOOL bTrap);
	void DeleteForcePoint(CLoadForce *pForce, long nIndex, BOOL bDelete);
	void DeleteForceTrap(CLoadForce *pForce, long nIndex, BOOL bDelete);
	void CheckLoadApply(CBOOLArray& bCheckArr);
	void UpdateComboData();

	enum { PREDEAD, POSTDEAD, LIVE,	WIND, CENTRI, CRASH, };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverModeling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcCantileverModeling)
	//}}AFX_MSG
	afx_msg void OnRadioPreLoad();
	afx_msg void OnRadioPostLoad();
	afx_msg void OnRadioLiveLoad();
	afx_msg void OnRadioWindLoad();
	afx_msg void OnRadioCrashLoad();
	afx_msg void OnRadioCentrifugalLoad();
	afx_msg void OnButtonAddload();
	afx_msg void OnButtonDelload();
	afx_msg void OnCheckApplyForce();
 	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
 	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
 	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERMODELING_H__3887F3EB_341A_415C_892E_4BBD4B19B760__INCLUDED_)
