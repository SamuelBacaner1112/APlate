// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETHSTIFFDLG_H__F92AFED2_F4BF_4B9C_AB8E_27A841648C33__INCLUDED_)
#define AFX_DETHSTIFFDLG_H__F92AFED2_F4BF_4B9C_AB8E_27A841648C33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetHStiffDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetHStiffDlg dialog

class CDetHStiffDlg : public TStackedPage
{
// Construction
public:
	CDetHStiffDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor
	CGeneralBaseStd *m_pStd;
	CDomyunGLView	*m_pView;				///< GL µµ¸é ºä

// Dialog Data
	//{{AFX_DATA(CDetHStiffDlg)
		enum { IDD = IDD_DET_04_HSTIFF };
		CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetHStiffDlg)
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

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetHStiffDlg)
		afx_msg void OnButtonRecommend();
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);

protected:
	long	m_nCurIndex	;

	double	m_dB;
	double	m_dD;
	double	m_dS;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETHSTIFFDLG_H__F92AFED2_F4BF_4B9C_AB8E_27A841648C33__INCLUDED_)
