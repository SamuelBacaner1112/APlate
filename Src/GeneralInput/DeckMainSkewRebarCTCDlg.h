// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKMAINSKEWREBARCTCDLG_H__C8B44462_B339_4E57_B992_DC3DDDC8553C__INCLUDED_)
#define AFX_DECKMAINSKEWREBARCTCDLG_H__C8B44462_B339_4E57_B992_DC3DDDC8553C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckMainSkewRebarCTCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckMainSkewRebarCTCDlg dialog

class CDeckMainSkewRebarCTCDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckMainSkewRebarCTCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckMainSkewRebarCTCDlg)
	enum { IDD = IDD_DECK_061_MAINSKEWREBAR_CTC };

	CXTFlatEdit		m_dRangeError;
	CXTTabCtrl		m_ctlTab;
	CGridCtrlMng	m_Grid;
	BOOL			m_bShowTotal;
	BOOL			m_bCheckBox;
	int				m_nPlanViewType;
	CXTFlatEdit		m_sRangeSkewRebarStt;
	CXTFlatEdit		m_sRangeSkewRebarEnd;
	//}}AFX_DATA
	CButton	m_btnError;
	CButton	m_btnSameAll;
	CButton	m_btnAdd;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckMainSkewRebarCTCDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckMainSkewRebarCTCDlg)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCalcRealToDim();
	afx_msg void OnButtonSameAll();
	afx_msg void OnCheckShowMainRebarUpDn();
	afx_msg void OnCheckShowMainRebarTotal();
	afx_msg void OnRadioOneUpDnView();
	afx_msg void OnRadioTwoView();
	afx_msg void OnCheckBox();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonAddGugan();
	//}}AFX_MSG
	afx_msg void OnKillfocusEditError();
	afx_msg void OnKillfocusEditRangeSkew();
	afx_msg void OnKillfocusEditRangeSkewEnd();
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);	
	DECLARE_MESSAGE_MAP()

private:
	void InitTabCtrl();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void SetGridData();
	void SetDataDefault();
	void SetDataDefaultRange();
	void SetDataInit();
	void SetDataSave();
	CString GetCurTabName();
	long GetTabCount();	
	CString GetTabTitle(long nIdx);
	BOOL IsValid(); 

	void DrawPlanSlab(CDomyun *pDom, long nType=0);	// nType=-1 : 상면,  nType=0 :상하면동시, nType=1 : 하면 
	void DrawPlanRebar(CDomyun *pDom);
	void DrawJongRebar(CDomyun *pDom);
	
	void InsertCTCByGridCol(long nGridCol);
	void DeleteCTCByGridCol(long nGridCol);

	CDPointArray* GetCTCRebar(long nPosSlab, BOOL bRealCTC, CString szCTC="");
	void SetCTCRebar(CDPointArray &xyArrCTC, long nPosSlab, BOOL bRealCTC, CString szCTC="");
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKMAINSKEWREBARCTCDLG_H__C8B44462_B339_4E57_B992_DC3DDDC8553C__INCLUDED_)
