// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKMAINREBARDLG_H__BB1E6759_7C9A_4CA9_80D0_E0F94CFEEA82__INCLUDED_)
#define AFX_DECKMAINREBARDLG_H__BB1E6759_7C9A_4CA9_80D0_E0F94CFEEA82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckMainRebarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckMainRebarDlg dialog
#include "DeckDetailMainUpperDlg.h"
#include "DeckDetailMainLowerCenterDlg.h"
#include "DeckDetailSettlementDlg.h"
#include "DeckDetailCyclesuDlg.h"
#include "DeckDetailDivideRebar.h"


class CDeckMainRebarDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckMainRebarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckMainRebarDlg)
	enum { IDD = IDD_DECK_03_MAINREINFORCE };
	CComboBox	m_comboViewCycle;
	BOOL	m_bCheckRailType;
	BOOL	m_bShowBox;
	CString	m_szMainShape;
	CString	m_szMainCenterShape;
	CString	m_szSettleOption;
	CString	m_szMainCycle;
	CString	m_szDivideRebar;
	//}}AFX_DATA
	CXTTabCtrl	m_ctlTab;	

	CGridCtrlMng m_Grid;						

	CDeckDetailMainUpperDlg			m_DetailMainUpperDlg;
	CDeckDetailMainLowerCenterDlg	m_DetailMainLowerCenterDlg;
	CDeckDetailSettlementDlg		m_DetailSettlementDlg;
	CDeckDetailCyclesuDlg			m_DetailCyclesuDlg;
	CDeckDetailDivideRebar			m_DetailDivideRebar;

	long m_nJointClick;
	long m_nCycleClick;
	BOOL m_bUpperClick;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckMainRebarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CButton	m_btnMainBar;
	CButton	m_btnMainCycle;
	CButton	m_btnMainCenter;
	CButton	m_btnSettleOption;
	CButton	m_btnDivideRebar;
	CButton	m_btnReversePos;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckMainRebarDlg)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckMark();
	afx_msg void OnViewBox();
	afx_msg void OnButtonMirrorJoint();
	afx_msg void OnCheckViewBox();
	afx_msg void OnCheckMainLowerLevel();
	afx_msg void OnSelchangeComboViewCycle();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonMainShape();
	afx_msg void OnButtonMainCenterShape();
	afx_msg void OnButtonSettleOption();
	afx_msg void OnButtonMainCycle();
	afx_msg void OnButtonDivideRebar();
	//}}AFX_MSG
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);	
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);	

	DECLARE_MESSAGE_MAP()

public:
	BOOL IsValid();

	void InitTabCtrl();
	void InitComboCycle();
	void OnPreInitDialog();
	void SetGridData();

	void SetDataDefault();
	void SetDataDefaultJoint();
	void SetDataDefaultOpt();
	
	void SetDataInit();
	void SetDataSave();
	
	CString GetCurTabName();
	CString GetTabName(long nTab);
	long GetSectionIDByCurTab();
	long GetSectionIDByTabName(CString sTab);

	void SetFocusItem(CString szTitle);
	BOOL IsFocusItem(CString& szTitle);	
	long GetFocusColByMultiString(CString szTitle);
	
	void InsertJointByGridCol(long nGridCol);
	void DeleteJointByGridCol();

	void OnCursorChange(long nRow,long nCol);	
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKMAINREBARDLG_H__BB1E6759_7C9A_4CA9_80D0_E0F94CFEEA82__INCLUDED_)
