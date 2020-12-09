// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETSTAGINGRINGDLG_H__FECB8769_4085_441C_99C2_8F2BD0B405F2__INCLUDED_)
#define AFX_DETSTAGINGRINGDLG_H__FECB8769_4085_441C_99C2_8F2BD0B405F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetStagingRingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetStagingRingDlg dialog

class CDetStagingRingDlg : public TStackedPage
{
// Construction
public:
	CDetStagingRingDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor
	CGeneralBaseStd *m_pStd;

// Dialog Data
	//{{AFX_DATA(CDetStagingRingDlg)
	enum { IDD = IDD_DET_07_STAGINGRING };
	CComboBox	m_ComboStagingring;
	CGridCtrl	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetStagingRingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetGridTitle();
	void OnPreInitDialog();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);
	long m_nType;
	// Grid ¼³Á¤


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetStagingRingDlg)
	afx_msg void OnButtonRecommend();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboStagingring();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);

protected:
	long	m_nCurIndex	;

	double	m_dA;
	double	m_dB;
	double	m_dC;
	double	m_dD;
	double	m_dE;
	double	m_dF;
	double	m_dW;
	double	m_dT;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETSTAGINGRINGDLG_H__FECB8769_4085_441C_99C2_8F2BD0B405F2__INCLUDED_)
