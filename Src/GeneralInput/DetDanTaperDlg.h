// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETDANTAPERDLG_H__C846FAFD_7936_4A7B_8741_75BE0513FDF0__INCLUDED_)
#define AFX_DETDANTAPERDLG_H__C846FAFD_7936_4A7B_8741_75BE0513FDF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetDanTaperDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetDanTaperDlg dialog

class CDetDanTaperDlg : public TStackedPage
{
// Construction
public:
	CDetDanTaperDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor
	CGeneralBaseStd *m_pStd;

// Dialog Data
	//{{AFX_DATA(CDetDanTaperDlg)
		enum { IDD = IDD_DET_05_DANMYUNSHIFTTAPER };
		CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetDanTaperDlg)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetGridTitle();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void OnCursorChange(long nRow, long nCol);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetDanTaperDlg)
		afx_msg void OnButtonRecommend();
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);


protected:
	long	m_nCurIndex	;

	double	m_dRatioW;
	double	m_dRatioH;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETDANTAPERDLG_H__C846FAFD_7936_4A7B_8741_75BE0513FDF0__INCLUDED_)
