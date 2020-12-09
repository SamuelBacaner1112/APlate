// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKGUARDFENCEDLG_H__B326FAC5_489B_44A4_9EFE_A9E3BFD8A3F8__INCLUDED_)
#define AFX_DECKGUARDFENCEDLG_H__B326FAC5_489B_44A4_9EFE_A9E3BFD8A3F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckGuardFenceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckGuardFenceDlg dialog
#include "ShearBarTypeDlg.h"

class AFX_EXT_CLASS CDeckGuardFenceDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckGuardFenceDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDeckGuardFenceDlg)
	enum { IDD = IDD_DECK_10_GUARDFENCE };
	CXTTabCtrl	m_Tab;
	CComboBox	m_comboGuardFence;
	CComboBox	m_comboThird;
	CComboBox	m_comboSecond;
	CComboBox	m_comboFirst;
	BOOL		m_bChkIsRebar;
	CXTFlatEdit	m_dCover;
	CXTFlatEdit	m_dFy;
	BOOL		m_bPlusToSlabRebar;
	CXTFlatEdit	m_dFck;
	//}}AFX_DATA
	CButton	m_btnShear;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckGuardFenceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckGuardFenceDlg)
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnSelchangeComboGuardwall();
	afx_msg void OnCheckIsrebar();
	afx_msg void OnButtonBarplaceType();
	afx_msg void OnSelchangeTabRebar(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckPlusToSlabRebar();
	//}}AFX_MSG
	afx_msg void OnKillfocusEditFck();
	afx_msg void OnKillfocusEditFy();
	afx_msg void OnKillfocusEditCover();
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedFirstGrid(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedSecondGrid(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

//Varialbe
public:
	int m_nTab;
	int m_nShearBarType;
	int m_nGuard;
	CGridCtrl m_GridFirst;
	CGridCtrl m_GridSecond;
	vector<CString> m_vecDia;   //Ã¶±ÙÁ÷°æ 
	vector<int> m_vecPart;		//±¸°£¼ö 
	vector<double>	m_vecCTC;	//Ã¶±ÙÀÇ CTC
	CShearBarTypeDlg m_dlgShearBarType;
	CBitmapButton	m_bitButton;

//Method
public:
	CDPointArray DeleteDimInfo(CDPointArray ptArrDimInfo, int nDelPart);
	CDPointArray AddDimInfo(CDPointArray ptArrDimInfo, int nAddPart);
	double GetDiaByIdx(int nIdx);
	int GetIdxDia(double dDia);
	int GetBarPlacingType(CDPointArray ptArrConc);
	CString GetRebarType();
	int GetIdxCTC(double dCTC);
	CDPointArray ConvertGuardFenceData(CDPoint A[9]);
	void SetDataInitRebarCTC();
	void SetDataInitRebarPart();
	void SetDataInitRebarDia();
	void SetDataDefault();
	void SetDataInit();	
	void SetDataSave();
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	void SetDataSaveShearRebar();
	void SetDataSaveDistRebar();
	void SetDataSaveMainRebar();
	void SetDataInitShearRebar();
	void SetDataInitDistRebar();
	void SetDataInitMainRebar();
	void SetGridTitleShearRebar();
	void SetGridTitleDistRebar();
	void SetGridTitleMainRebar();
	void SetGridTitle();
	void SetDataInitTab();
	void ChangeViewControl();
	void SetDataInitComboGuardFence();
	void SetDataInitComboShearRebar();
	void SetDataInitComboDistRebar();
	void SetDataInitComboMainRebar();
	void SetDataInitCombo();
	void OnPreInitDialog();

	BOOL IsValid();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKGUARDFENCEDLG_H__B326FAC5_489B_44A4_9EFE_A9E3BFD8A3F8__INCLUDED_)
