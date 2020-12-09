// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BINDCONCJEWONDLG_H__6D556280_77C2_4F85_B4A4_F829399CF675__INCLUDED_)
#define AFX_BINDCONCJEWONDLG_H__6D556280_77C2_4F85_B4A4_F829399CF675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BindConcJewonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBindConcJewonDlg dialog
class CDataManage;
class CBindConcJewonDlg : public CDialog
{
// Construction
public:
	CBindConcJewonDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBindConcJewonDlg)
	enum { IDD = IDD_DIALOG_BINDCONC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGridCtrl m_Grid;
	CDomyunView m_Domyun;
	CGeneralBaseStd	*m_pStd;
	CHDanDlg		*m_pParent;	

	double m_dGirH;
	void SetDataDefault();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBindConcJewonDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBindConcJewonDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnApplyJewon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	void OnDefault();

	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();	
	void DrawBindConc();
	void SetGirderHeight();
	void OnCellChangedData(NMHDR*nmgv,LRESULT*);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINDCONCJEWONDLG_H__6D556280_77C2_4F85_B4A4_F829399CF675__INCLUDED_)
