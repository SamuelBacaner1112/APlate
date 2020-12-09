// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//
#if !defined(AFX_CALCSECTIONCONCEPTDLG_H__F43F9495_D7F0_4A3B_A1EA_B416C1FE4D57__INCLUDED_)
#define AFX_CALCSECTIONCONCEPTDLG_H__F43F9495_D7F0_4A3B_A1EA_B416C1FE4D57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcSectionConceptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcSectionConceptDlg dialog

class CCalcSectionConceptDlg : public CGeneralBaseDlg
{
// Construction
public:
	CCalcSectionConceptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcSectionConceptDlg)
	enum { IDD = IDD_CALC_SECTIONCONCEPT };

	BOOL		m_bWind;
	int			m_nHLLExePos;
	int			m_nLiveLoadType;
	CXTFlatEdit	m_dDBDLWidth;
	CXTFlatEdit	m_dDBDisFromEdge;
	BOOL		m_bNoDisByNoGuard;

	//}}AFX_DATA

	CGridCtrl	m_Grid;
	CDataManage *m_pDataManage;
	CString		m_sSelectHLNum;
	//CString		m_sSelectStation;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcSectionConceptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcSectionConceptDlg)
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
private:
	void InitSelectLiveHL();

public:
	void DrawInputDomyunView(BOOL bZoomAll);
	void OnPreInitDialog();
	void SetDataGridTitle();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDlgItemControl();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCSECTIONCONCEPTDLG_H__F43F9495_D7F0_4A3B_A1EA_B416C1FE4D57__INCLUDED_)
