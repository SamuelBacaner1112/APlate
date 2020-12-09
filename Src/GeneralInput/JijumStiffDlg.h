// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_JIJUMSTIFFDLG_H__C57D8B53_A3FA_44B2_AF7C_F9177459E960__INCLUDED_)
#define AFX_JIJUMSTIFFDLG_H__C57D8B53_A3FA_44B2_AF7C_F9177459E960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JijumStiffDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJijumStiffDlg dialog

class CJijumStiffDlg : public CGeneralBaseDlg
{
// Construction
public:
	CJijumStiffDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJijumStiffDlg)
	enum { IDD = IDD_GEN_09_JIJUMSTIFFPOSITION };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CButton	m_btnAllSame;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJijumStiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDataBx();
	void SetDataJewon();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	// Grid ¼³Á¤
	void SetGridTitle();

	double GetStationMatrix(long nRow, long nCol, long nG);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJijumStiffDlg)
	afx_msg void OnButtonAllsameapply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JIJUMSTIFFDLG_H__C57D8B53_A3FA_44B2_AF7C_F9177459E960__INCLUDED_)
