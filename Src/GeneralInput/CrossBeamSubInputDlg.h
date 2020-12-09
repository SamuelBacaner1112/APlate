// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CROSSBEAMSUBINPUTDLG_H__129F44F2_8FB0_4A5F_B040_1996B9D85E19__INCLUDED_)
#define AFX_CROSSBEAMSUBINPUTDLG_H__129F44F2_8FB0_4A5F_B040_1996B9D85E19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrossBeamSubInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrossBeamSubInputDlg dialog

#define WM_DATA_CHANGED		(WM_APP + 10000)
#define WM_SUBINPUT_CLOSE	(WM_APP + 10001)
#include "resource.h"

class CCrossBeamSubInputDlg : public CDialog
{
// Construction
public:
	CCrossBeamSubInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCrossBeamSubInputDlg)
	enum { IDD = IDD_DIALOG_CROSSBEAM_SUBINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGeneralBaseStd		*m_pStd;
	CGridCtrl			m_Grid;
	CPlateCrossBeam		*m_pCrossBeam;
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	long GetlongValue(CString szValue);
	BOOL GetBoolValue(CString szValue);
	void SetCellReadOnly(long nRow, long nCol, BOOL bReadOnly);
	void RecommendBracketHeight();
	void RecommendMoment();
	void RecommendConnectionPlate();
	void RecommendHunchTopConnect();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrossBeamSubInputDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCrossBeamSubInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCellButtonClickRecommend(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROSSBEAMSUBINPUTDLG_H__129F44F2_8FB0_4A5F_B040_1996B9D85E19__INCLUDED_)
