// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_APLATEOUTBMDLG_H__0BE4A68D_2D25_46DE_9B00_B9B184FB0948__INCLUDED_)
#define AFX_APLATEOUTBMDLG_H__0BE4A68D_2D25_46DE_9B00_B9B184FB0948__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// APlateOutBMDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutBMDlg dialog

#include "resource.h"

class CAPlateOutBMStd;

class CAPlateOutBMDlg : public CDialog
{
// Construction
public:
	CAPlateOutBMDlg(CAPlateOutBMStd *pBMStd, CWnd* pParent = NULL);   // standard constructor

	CAPlateOutBMStd		*m_pBMStd;
	long				m_nTotBridge;		// 총 출력 교량 수
	
	
	void SetDataSave();
	void SetWnd(CWnd* pWnd) { m_pWnd = pWnd;};

// Dialog Data
	//{{AFX_DATA(CAPlateOutBMDlg)
	enum { IDD = IDD_DIALOG_APLATE_OUTBM };
	CCustomOptionsTreeCtrl	m_ctrlTreeBMOut;
	CProgressCtrl	m_ctrlProgress;
	CString	m_szProgressText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPlateOutBMDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CWnd* m_pWnd;
	CCustomOptions	m_Options;
	
	void InitTreeData(BOOL bCheckAll);
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAPlateOutBMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBmout();
	afx_msg void OnButtonFileOpen();
	afx_msg void OnButtonAllPrintout();
	afx_msg void OnButtonOpenDomyun();
	afx_msg void OnButtonAllOutCancel();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnReceive(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLATEOUTBMDLG_H__0BE4A68D_2D25_46DE_9B00_B9B184FB0948__INCLUDED_)
