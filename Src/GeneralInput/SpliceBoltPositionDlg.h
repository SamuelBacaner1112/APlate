// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SPLICEBOLTPOSITIONDLG_H__2504C726_053B_4ABD_AC3B_78492F1F0006__INCLUDED_)
#define AFX_SPLICEBOLTPOSITIONDLG_H__2504C726_053B_4ABD_AC3B_78492F1F0006__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpliceBoltPositionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpliceBoltPositionDlg dialog
#include "resource.h"
class CSpliceBoltPositionDlg : public CDialog
{
// Construction
public:
	CSpliceBoltPositionDlg(CGeneralBaseStd *pStd, long nG, long nSp, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpliceBoltPositionDlg)
	enum { IDD = IDD_SPLICE_BOLTPOS };	
	//}}AFX_DATA
	CGridCtrl m_Grid;
	CDetFieldJoinDlg *m_pParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpliceBoltPositionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpliceBoltPositionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectPos();
	afx_msg void OnAllPos();
	afx_msg void OnRecommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CGeneralBaseStd *m_pStd;
	long m_nG;
	long m_nSp;
	int  m_nSpliceType; //통판형(0),분리형(1),연결형(2) 구분자
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void SetAutoCalcPos(long nG, long nSp);

public:
	double	m_dHorUL;	// 상하판 ↔
	double	m_dVerUL;	// 상하판 ↕
	double	m_dHorMS;	// 모멘트,복부판 ↔	
	double	m_dVerMS;	// 모멘트,복부판 ↕
	double	m_dDan;
	double	m_dCen;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLICEBOLTPOSITIONDLG_H__2504C726_053B_4ABD_AC3B_78492F1F0006__INCLUDED_)
