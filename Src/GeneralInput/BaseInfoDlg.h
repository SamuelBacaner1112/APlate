// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BASEINFODLG_H__7DE62657_E658_48FA_AA91_82CCA1F2830D__INCLUDED_)
#define AFX_BASEINFODLG_H__7DE62657_E658_48FA_AA91_82CCA1F2830D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseInfoDlg dialog

class CBaseInfoDlg : public CGeneralBaseDlg
{
// Construction
public:
	CBaseInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBaseInfoDlg)
	enum { IDD = IDD_GEN_02_BASEINFO };
	CGridCtrl			m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseInfoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void OnPreInitDialog();
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void DrawJongGuBaeDo();
	void DrawJongDan();

	long GetTabCount() { return 2; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "평면도" : "종단면도"; }	
// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CBaseInfoDlg)
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEINFODLG_H__7DE62657_E658_48FA_AA91_82CCA1F2830D__INCLUDED_)
