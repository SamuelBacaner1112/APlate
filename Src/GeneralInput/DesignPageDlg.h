// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNPAGEDLG_H__96A94857_D563_4626_8534_B5A22ECA6FB9__INCLUDED_)
#define AFX_DESIGNPAGEDLG_H__96A94857_D563_4626_8534_B5A22ECA6FB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignPageDlg.h : header file
//
//#include "resource.h"
#include "StackDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CDesignPageDlg dialog

// class AFX_EXT_CLASS CDesignPageDlg : public TStackedDialog

class AFX_EXT_CLASS CDesignPageDlg : public TStackedDialog
{
// Construction
public:
	CDesignPageDlg(CWnd* pParent = NULL);   // standard constructor	
	virtual TStackedPage *CreatePage(UINT nId);
	virtual void OnPageChanged(UINT nId, BOOL bActivated);
	CGeneralBaseStd* GetGeneralInputStd()	{ return m_pStd; }

// Dialog Data
	//{{AFX_DATA(CDesignPageDlg)
	enum { IDD = IDD_DIALOG_MAINDLG };
	CStatic		m_PlaceHolder;

	//}}AFX_DATA
	CButton			m_btnOptSave;
	CButton			m_btnOptOpen;
	CButton			m_btnF10T;
	CButton			m_btnF13T;
	CComboBoxEx		m_ComboLengthCalc;
	CXTTreeCtrl		m_ctlMenuTree;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void MovePageWindow();

	void SetShowButton(BOOL bShow = FALSE);
	void SetShowEdit(BOOL bShow = FALSE);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignPageDlg)
	afx_msg void OnSelchangedTreeMenu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonRecommendEachDlg();
	afx_msg void OnButtonOpenopt();
	afx_msg void OnButtonSaveopt();
	afx_msg void OnButtonF10T();
	afx_msg void OnButtonF13t();
	afx_msg void OnSelchangeComboBoltlength();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void ResetDesignPage(long nBridgeType);
	void OnPreInitDialog();


	CStringArray m_MenuStr;
	CWordArray m_nIdArray;
	CImageList m_TreeImageList;

	CMenu	m_MenuMakeButton;
	long GetBridgeType() { return m_nBridgeType;}

// 	UINT	m_nPlaceholderID;
// 	TStackedPageHelperList m_PageList;
// 	TStackedPageHelper *GetPage(void);
// 	TStackedPageHelper *GetPage(UINT nId);
// 	TStackedPageHelper *AddPage(UINT nId, DWORD dwHelpId = 0L, DWORD dwData1 = 0, DWORD dwData2 = 0);
// 	
// 	void SetPage(void);
// 	void SetPage(UINT nId);
// 	void activatePage(TStackedPageHelper *pPage);
// 	void deactivatePage(TStackedPageHelper *pPage);



protected:
	long m_nBridgeType;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNPAGEDLG_H__96A94857_D563_4626_8534_B5A22ECA6FB9__INCLUDED_)
