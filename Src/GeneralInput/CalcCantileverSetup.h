// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERSETUP_H__CBB35903_66C0_44A4_9BC1_1256AA6C8E74__INCLUDED_)
#define AFX_CALCCANTILEVERSETUP_H__CBB35903_66C0_44A4_9BC1_1256AA6C8E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverSetup dialog

class CCalcCantileverSetup : public CGeneralBaseDlg
{
// Construction
public:
	CCalcCantileverSetup(CWnd* pParent = NULL);   // standard constructor

private:
	CGridCtrl	m_GridGen;
	CGridCtrl	m_GridLoad;
	CGridCtrl	m_GridAdd;
	CGridCtrl	m_GridWalk;
// Dialog Data
	//{{AFX_DATA(CCalcCantileverSetup)
	enum { IDD = IDD_CALC_CANTILEVER_SETUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetDataInit();
	void SetDataSave();
	void SetGridTitle();
	void OnPreInitDialog();
	void SetDataDefault();
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	void SetGridTitleGen();
	void SetGridTitleLoad();
	void SetGridTitleAdd();
	void SetGridTitleWalkLoad();
	void SetDataInitGen();
	void SetDataInitLoad();
	void SetDataInitAdd();
	void SetDataInitWalkLoad();
	void SetDataSaveGen();
	void SetDataSaveLoad();
	void SetDataSaveWalkLoad();
	void SetDataSaveAdditiveLoad();
	void SetCellReadOnly(long nTarget);
	CString	GetApply(BOOL bApply);
	BOOL	GetApply(CString szApply);
	BOOL	IsExistWalkLoad(BOOL bLeft);
	enum { BANGEUM, NANGAN, WALKLOAD, PEOPLELOAD, ADDITIVELOAD };
	long	m_nDrawType;
	// Generated message map functions
	//{{AFX_MSG(CCalcCantileverSetup)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg void OnCellChangedDataLoad(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedDataWalkLoad(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedDataAdditiveLoad(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellButtonClickAdditiveLoad(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangeLoad(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangeWalkLoad(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedDataGen(NMHDR*nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERSETUP_H__CBB35903_66C0_44A4_9BC1_1256AA6C8E74__INCLUDED_)
