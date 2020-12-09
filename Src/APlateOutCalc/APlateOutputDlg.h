// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_APLATEOUTPUTDLG_H__3AB42FE5_148B_4318_B19D_5E8569F3DAEF__INCLUDED_)
#define AFX_APLATEOUTPUTDLG_H__3AB42FE5_148B_4318_B19D_5E8569F3DAEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// APlateOutputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutputDlg dialog
class CAPlateOutCalcStd;
#include "resource.h"

class CAPlateOutputDlg : public CDialog
{
// Construction
public:
	CAPlateOutputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAPlateOutputDlg();
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetDlgEnable(BOOL bEnable);
	long GetCurXLFile()			const	{return m_nFlagXLFileOut; }
	long GetCurXLCheckFile()	const	{return m_nFlagXLCheckFileOut; }
	void SaveFileXLFormat();				// ������꼭
	void SaveFileXLCheckFormat();			// üũ����Ʈ
	void InitListBox();
	void SetWnd(CWnd* pWnd) { m_pWnd = pWnd;};
	void SetXLOutput();
	long GetPageSize()			const	{ return m_nPageArr.GetSize();}
	long m_nType;	//0:1������ 1:��������� 2:������� / check: 3:1������ 4:��������� 5:�������
	CDWordArray		m_nPageArr;
	BOOL	m_bPic;
	BOOL	m_bOneFiles;
	CCustomOptions	m_Options;
	CString			m_szFileName;
	void AllCheckCalcSheet(BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CAPlateOutputDlg)
	enum { IDD = IDD_DIALOG_APLATEOUTXL };
	CButton	m_buttonOpt;
	CProgressCtrl	m_ctrlProgress2;
	CProgressCtrl	m_ctrlProgress1;
	CListCtrl	m_ctrlList;
	CTabCtrl	m_ctrlTab;
	CCustomOptionsTreeCtrl	m_ListOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPlateOutputDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd*			m_pWnd;
	long			m_nFlagXLFileOut;
	long			m_nFlagXLCheckFileOut;
	CString			m_sXLSaveFile;			// ������꼭 XL�� ������ �̸�
	CString			m_sXLCheckSaveFile;		// ���丮��Ʈ XL�� ������ �̸�
	CXTPMarqueeCtrl	*m_pProgress;
	// Generated message map functions
	//{{AFX_MSG(CAPlateOutputDlg)
	afx_msg void OnSelchangeTabOutxl(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnMakeXLFile();
	afx_msg void OnButtonOut();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnButtonFileOpen();
	afx_msg void OnButtonDomyunOpen();
	afx_msg void OnButtonAllcheck();
	afx_msg void OnButtonAlluncheck();
	afx_msg void OnCheckPic();
	afx_msg void OnButtonOpenFolder();
	//}}AFX_MSG
	afx_msg LRESULT OnReceive(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnReceiveCheck(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
public:
	UINT OnMakeAnalysisCal();
	void InitTreeData();
	void SaveTreeData();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLATEOUTPUTDLG_H__3AB42FE5_148B_4318_B19D_5E8569F3DAEF__INCLUDED_)
