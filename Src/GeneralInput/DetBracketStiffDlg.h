// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETBRACKETSTIFFDLG_H__C597C42E_2DEC_4A4B_A8A5_7609C5D384F4__INCLUDED_)
#define AFX_DETBRACKETSTIFFDLG_H__C597C42E_2DEC_4A4B_A8A5_7609C5D384F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetBracketStiffDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetBracketStiffDlg dialog

class CDetBracketStiffDlg : public TStackedPage
{
// Construction
public:
	CDetBracketStiffDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor
	CGeneralBaseStd *m_pStd;
	CDomyunGLView   *m_pView;

// Dialog Data
	//{{AFX_DATA(CDetBracketStiffDlg)
	enum { IDD = IDD_DET_10_BRACKETSTIFF };
	CXTTabCtrl	m_tabCRType;
	CXTFlatComboBox	m_ctrlBracketType;
	CGridCtrl m_WeldGrid;
	CGridCtrl m_BoltGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetBracketStiffDlg)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void OnPreInitDialog();

private:	
	void SetInitTypeCombo();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetGridTitle();
	void SetGridTitleWeld();
	void SetGridTitleBolt();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetBracketStiffDlg)
	afx_msg void OnButtonRecommend();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboBracketType();
	afx_msg void OnSelchangeTabCrossbeam(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChangedDataWeld(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataBolt(NMHDR* nmgv, LRESULT*);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETBRACKETSTIFFDLG_H__C597C42E_2DEC_4A4B_A8A5_7609C5D384F4__INCLUDED_)
