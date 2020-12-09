// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_STARTPOINTJEWONDLG_H__0B32D1F4_9FF7_4D44_BC59_77A64DE8436F__INCLUDED_)
#define AFX_STARTPOINTJEWONDLG_H__0B32D1F4_9FF7_4D44_BC59_77A64DE8436F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartPointJewonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartPointJewonDlg dialog

class CStartPointJewonDlg : public CGeneralBaseDlg
{
// Construction
public:
	CStartPointJewonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStartPointJewonDlg)
	enum { IDD = IDD_GEN_11_STARTENDPOINTJEWON };
	CGridCtrl m_Grid;
	int		m_nTaperType;
	int		m_nStiffType;
	BOOL	m_bManhole;
	BOOL	m_bStiffPlate;	
	BOOL	m_bDuplicate;
	//}}AFX_DATA

	long m_nCurPos;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartPointJewonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void TaperCalc(long col);
	void SetDlgItemEnable();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);
	void ResetGrid();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartPointJewonDlg)
	afx_msg void OnCheckManhole();
	afx_msg void OnCheckStiffplate();
	afx_msg void OnRadioTaper1();
	afx_msg void OnRadioTaper2();
	afx_msg void OnRadioStiff();
	afx_msg void OnRadioStiff2();
	afx_msg void OnDuplicate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTPOINTJEWONDLG_H__0B32D1F4_9FF7_4D44_BC59_77A64DE8436F__INCLUDED_)
