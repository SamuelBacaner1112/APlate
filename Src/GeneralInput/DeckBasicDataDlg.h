// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKBASICDATADLG_H__1EB529BE_BBF1_4325_8E72_CFDBD548FCFC__INCLUDED_)
#define AFX_DECKBASICDATADLG_H__1EB529BE_BBF1_4325_8E72_CFDBD548FCFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckBasicDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckBasicDataDlg dialog
#include "DialogInstallMainAtBlockout.h"
#include "DialogInstallBeryukAtBlockout.h"

class CDeckBasicDataDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckBasicDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeckBasicDataDlg();

// Dialog Data
	//{{AFX_DATA(CDeckBasicDataDlg)
	enum { IDD = IDD_DECK_01_BASICDATA };
	//}}AFX_DATA
	CGridCtrlMng	m_Grid;
	CXTTabCtrl		m_Tab;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckBasicDataDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CButton	m_btnOptOpen;
	CButton	m_btnOptSave;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckBasicDataDlg)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionOpen();
	afx_msg void OnOptionsave();
	afx_msg void OnAllRecommend();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChanged(NMHDR*nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()


private:
	CDialogInstallMainAtBlockout	m_DlgInstallMainAtBlockout;
	CDialogInstallBeryukAtBlockout	m_DlgInstallBeryukAtBlockout;

private:
	double GetValueJoint(long nIdxDia, long nIdxType);
	double GetValueSettle(long nIdxDia, long nIdxType);

	void SetGridDataFactor();
	void SetGridDataJoint();
	void SetGridDataSettle();
	void SetGridDataWeight();
	void SetGridDataRebarFactor();

	void SetDataDefaultFactor(); 
	void SetDataDefaultJoint(); 
	void SetDataDefaultSettle(); 
	void SetDataDefaultWeight(); 
	void SetDataDefaultRebarFactor();
	
	CString GetCurTabName();
	double m_dRebarSpace;
	double m_dCover;

public:
	BOOL IsValid();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetDataInit();
	void SetGridData();

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKBASICDATADLG_H__1EB529BE_BBF1_4325_8E72_CFDBD548FCFC__INCLUDED_)
