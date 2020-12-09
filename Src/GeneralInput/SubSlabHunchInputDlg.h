// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SUBSLABHUNCHINPUTDLG_H__D254722C_F939_4505_B373_3A5F512A1265__INCLUDED_)
#define AFX_SUBSLABHUNCHINPUTDLG_H__D254722C_F939_4505_B373_3A5F512A1265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubSlabHunchInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubSlabHunchInputDlg dialog
class CGeneralBaseStd;
class CPlateBridgeApp;
class CSubSlabHunchInputDlg : public CXTResizeDialog
{
// Construction
public:
	CSubSlabHunchInputDlg(CGeneralBaseStd *pStd, CPlateBridgeApp *pBridge, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSubSlabHunchInputDlg)
	enum { IDD = IDD_SUBINPUT_HUNCHDLG };
	CXTFlatEdit	m_dBaseLeftHeight;
	CXTFlatEdit	m_dBaseRightHeight;
	CXTFlatEdit	m_ctrlEdit1;
	CXTFlatEdit	m_ctrlEdit2;
	//}}AFX_DATA
	CButton	m_btnApply;
	CButton	m_btnOK;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubSlabHunchInputDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CGeneralBaseStd *m_pStd;
	CPlateBridgeApp *m_pBridge;

	CDomyunView m_DomView;
	CGridCtrl m_Grid;

public:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void SetDataInit();
	void SetGridTitle();
	void SetDataSave();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubSlabHunchInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnKillfocusEditBaserheight();
	afx_msg void OnKillfocusEditBaselheight();
	afx_msg void OnSetfocusEditBaselheight();
	afx_msg void OnSetfocusEditBaserheight();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBSLABHUNCHINPUTDLG_H__D254722C_F939_4505_B373_3A5F512A1265__INCLUDED_)
