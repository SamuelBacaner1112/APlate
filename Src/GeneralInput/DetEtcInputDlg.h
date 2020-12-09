// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETETCINPUTDLG_H__0C2D0189_C369_4560_A476_BE44F281D21F__INCLUDED_)
#define AFX_DETETCINPUTDLG_H__0C2D0189_C369_4560_A476_BE44F281D21F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetEtcInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetEtcInputDlg dialog
#include "StackDialog.h"
class CDataManage;

//class AFX_EXT_CLASS CDetEtcInputDlg : public TStackedDialog
class AFX_EXT_CLASS CDetEtcInputDlg : public TStackedDialog
{
// Construction
public:
	CDetEtcInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual TStackedPage *CreatePage(UINT nId);
	virtual void OnPageChanged(UINT nId, BOOL bActivated);

// Dialog Data
	//{{AFX_DATA(CDetEtcInputDlg)
	enum { IDD = IDD_DET_01_ECT_INPUT };
	CTreeCtrl	m_TreeMenu;
	//}}AFX_DATA
	CButton m_btnSameAll;
	CButton m_btnSameEnd;
	CButton m_btnSameJijum;
	CButton m_btnInputDetail;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetEtcInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void MovePageWindow();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetEtcInputDlg)
	afx_msg void OnSelchangedTreeMenu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonApplyIdentityAllgirder();
	afx_msg void OnButtonApplyIdentityPart();
	afx_msg void OnButtonApplyIdentityAlljijum();
	afx_msg void OnButtonApplyCompleteinputAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void OnPreInitDialog();
	void SetPageInit(long nID);
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void OnButtonNext();
	void OnButtonPrev();
	void ResetDesignPage(long nBridgeType);

	CStringArray m_MenuStr;
	CWordArray m_nIdArray;
	CImageList m_TreeImageList;
	void ShowHideItem(long nId);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETETCINPUTDLG_H__0C2D0189_C369_4560_A476_BE44F281D21F__INCLUDED_)
