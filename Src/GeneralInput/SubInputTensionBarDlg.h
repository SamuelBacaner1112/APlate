// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SUBINPUTTENSIONBARDLG_H__EA4AC627_480E_4BBB_AD9A_2B912AF08922__INCLUDED_)
#define AFX_SUBINPUTTENSIONBARDLG_H__EA4AC627_480E_4BBB_AD9A_2B912AF08922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubInputTensionBarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubInputTensionBarDlg dialog
class CPlateBridgeApp;
class CCalcFloor;
class CSubInputTensionBarDlg : public CXTResizeDialog
{
// Construction
public:
	CSubInputTensionBarDlg(CGeneralBaseStd *pStd, CPlateBridgeApp *pBridge, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSubInputTensionBarDlg)
	enum { IDD = IDD_SUBINPUT_TENSIONBAR };	
	int				m_nSize;
	CXTFlatEdit		m_dLHeight;
	CXTFlatEdit		m_dRHeight;
	double			m_dGinJangL;
	double			m_dJungChakL;
	CXTFlatEdit		m_ctrlEdit1;
	CXTFlatEdit		m_ctrlEdit2;
	CXTFlatEdit		m_ctrlEdit3;
	CXTFlatEdit		m_ctrlEdit4;
	CXTFlatEdit		m_ctrlEdit5;
	CXTFlatEdit		m_ctrlEdit6;
	CXTFlatEdit		m_ctrlEdit7;
	double	m_dSecMove_L;
	double	m_dDuctOffset_L;
	double	m_dSecMoveEnd_L;
	//}}AFX_DATA
	CGeneralBaseStd *m_pStd;
	CPlateBridgeApp *m_pBridge;

	CDomyunView m_DomView;
	CGridCtrl m_Grid;
	CImageList m_ImageList;

	CXTFlatComboBox	m_comboFix;
	CXTFlatComboBox m_comboModelFix;
	CXTFlatComboBox	m_ctrlComboBox1;
	CButton	m_btnRecommend;
	CButton	m_btnDXFOut;
	CButton	m_btnApply;
	CButton	m_btnOK;

public:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void SetDataDefault();
	void SetDataInit();
	void SetGridTitle();
	void SetDataSave();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubInputTensionBarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubInputTensionBarDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboSize();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonRecommend();
	afx_msg void OnButtonDxfout();
	afx_msg void OnSelchangeComboFix();
	afx_msg void OnSelchangeComboFixDom();
	//}}AFX_MSG
	afx_msg void OnKillfocusEditBaselheight();
	afx_msg void OnKillfocusEditBaserheight();
	afx_msg void OnSetfocusEditBaselheight();
	afx_msg void OnSetfocusEditBaserheight();
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedCol(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBINPUTTENSIONBARDLG_H__EA4AC627_480E_4BBB_AD9A_2B912AF08922__INCLUDED_)
