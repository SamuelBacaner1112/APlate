// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCMODELINGFRAMEPROPTY_H__614C5519_3978_44E2_8358_0A359FB26107__INCLUDED_)
#define AFX_CALCMODELINGFRAMEPROPTY_H__614C5519_3978_44E2_8358_0A359FB26107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcModelingFramePropty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingFramePropty dialog

class CCalcModelingFramePropty : public CDialog
{
// Construction
public:
	CCalcModelingFramePropty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcModelingFramePropty)
	enum { IDD = IDD_DIALOG_MODELING_FRAME_PROP };
	int		m_nPrePost;
	//}}AFX_DATA
	CGridCtrl m_Grid;
	CDomyunView m_Domyun;
	CGeneralBaseStd	*m_pStd;
	CButton	m_btnOk;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcModelingFramePropty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault(){};

	// Generated message map functions
	//{{AFX_MSG(CCalcModelingFramePropty)
	afx_msg void OnRadioPre();
	afx_msg void OnRadioPost();
	afx_msg void OnRadioPreSteel();
	afx_msg void OnRadioPreConc();
	afx_msg void OnSelchangeComboLoadcase();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCMODELINGFRAMEPROPTY_H__614C5519_3978_44E2_8358_0A359FB26107__INCLUDED_)
