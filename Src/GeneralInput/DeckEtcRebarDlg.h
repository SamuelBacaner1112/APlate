// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKETCREBARDLG_H__3B9DA8C3_DBD6_48E9_BE24_7BD87F2745FE__INCLUDED_)
#define AFX_DECKETCREBARDLG_H__3B9DA8C3_DBD6_48E9_BE24_7BD87F2745FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckEtcRebarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckEtcRebarDlg dialog

class AFX_EXT_CLASS CDeckEtcRebarDlg : public CGeneralBaseDlg
{
private:
	int m_nTypeWaterHoleBar;				//배수구 철근타입 
	CDoubleArray m_dArrDia;

// Construction
public:
	CDeckEtcRebarDlg(CWnd* pParent = NULL);   // standard constructor

//Method
public:
	int m_nTypeWaterHole;
	
	void ChangeViewControlWaterHole();
	void ChangeViewControlWaterHoleBar();
	void ChangeViewControlShearBar();

	void MakeShearBarCen();
	void MakeShearBarDan();
	//Combo Init
	void SetComboBarCount();
	void SetComboBarType();
	void SetComboDia();
	void SetComboDiaWaterHole();
	void SetComboDiaShearBar();
	void SetComboDiaSpacer();

	//Data Save
	void SetDataSave();
	void SetDataSaveSpacer();
	void SetDataSaveWaterHoleBar();
	void SetDataSaveShearBar();

	//Data Init
	void SetDataInit();
	void SetDataInitWaterHoleBar();
	void SetDataInitShearBar();
	void SetDataInitSpacer();

	BOOL IsValid();
	void OnPreInitDialog();
	void SetDataDefault();
	void DrawInputDomyunView(BOOL bZoomAll);

// Dialog Data
	//{{AFX_DATA(CDeckEtcRebarDlg)
	enum { IDD = IDD_DECK_08_ETCREBAR };
	CComboBox	m_comboWaterHoleType;
	CComboBox	m_comboSpacerDia;
	CComboBox	m_comboWaterHoleBarDia;
	CComboBox	m_comboEndRFBarDia;
	CComboBox	m_comboBarCnt;
	CComboBox	m_comboBarType;
	CXTFlatEdit	m_nCntSetMainBar;
	CXTFlatEdit	m_nCntSetDistBar;
	CXTFlatEdit	m_dLenWaterHoleBar;
	CXTFlatEdit	m_dAnchorLenWaterHoleBar;
	CXTFlatEdit	m_dAngleWaterHoleBar;
	CXTFlatEdit	m_dSpaceWaterHoleBar;
	CXTFlatEdit	m_dRadiusWaterHoleBar;
	CXTFlatEdit	m_dSpacerLenA;
	CXTFlatEdit	m_dSpacerLenB;
	CXTFlatEdit	m_dSpacerLenD;
	CXTFlatEdit	m_nCntSpacerInSquare;
	CXTFlatEdit	m_dLengthEndRFBar;
	CXTFlatEdit	m_dRadiusEndRFBar;
	CXTFlatEdit	m_dAngleEndRFBar;
	CXTFlatEdit	m_nCntEndRFBar;
	CXTFlatEdit	m_nCntMidRFBar;
	CXTFlatEdit	m_nWaterHolesu;
	CXTFlatEdit	m_dSpacerMaxC;
	CXTFlatEdit	m_dSpacerMinC;
	CXTFlatEdit	m_nWaterHoleDansu;
	CXTFlatEdit	m_dWaterHoleWidth;
	CXTFlatEdit	m_dWaterHoleLength;
	BOOL		m_bApplyShear;
	CXTFlatEdit	m_dSigmaYShear;
	CXTFlatEdit	m_dSigmaYSpacer;
	CXTFlatEdit	m_dSigmaYWaterHole;
	BOOL	m_bApplyWaterHole;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckEtcRebarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDeckEtcRebarDlg)
	afx_msg void OnSelchangeComboWaterHoleType();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnSelchangeComboBarCount();
	afx_msg void OnSelchangeComboWaterHolebarDia();
	afx_msg void OnSelchangeComboEndRfbarDia();
	afx_msg void OnSelchangeComboSpacerDia();
	afx_msg void OnCheckShear();
	afx_msg void OnCheckWaterhole();
	//}}AFX_MSG
	afx_msg void OnKillfocusEditBarSpace();
	afx_msg void OnKillfocusEditBarLength();
	afx_msg void OnKillfocusEditWaterR();
	afx_msg void OnKillfocusEditWaterS();
	afx_msg void OnKillfocusEditBarAngle();
	afx_msg void OnKillfocusEditSpacerLenA();
	afx_msg void OnKillfocusEditSpacerLenB();
	afx_msg void OnKillfocusEditSpacerLenD();
	afx_msg void OnKillfocusEditEndRfbarAngle();
	afx_msg void OnKillfocusEditEndRfbarCount();
	afx_msg void OnKillfocusEditEndRfbarLength();
	afx_msg void OnKillfocusEditEndRfbarRadius();
	afx_msg void OnKillfocusEditMidRfbarCount();
	afx_msg void OnKillfocusEditWaterHoleCount();
	afx_msg void OnKillfocusEditSpacerMaxC();
	afx_msg void OnKillfocusEditSpacerMinC();
	afx_msg void OnKillfocusEditSpacerMainbar();
	afx_msg void OnKillfocusEditSpacerDistbar();
	afx_msg void OnKillfocusEditSpacerCount();
	afx_msg void OnKillfocusEditWaterHoleFloor();
	afx_msg void OnKillfocusEditWaterHoleLength();
	afx_msg void OnKillfocusEditWaterHoleWidth();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKETCREBARDLG_H__3B9DA8C3_DBD6_48E9_BE24_7BD87F2745FE__INCLUDED_)
