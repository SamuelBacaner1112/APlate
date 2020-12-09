// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCMODELINGSETUP_H__5AE39E75_F85A_4CCF_AA6C_902A5347FB4E__INCLUDED_)
#define AFX_CALCMODELINGSETUP_H__5AE39E75_F85A_4CCF_AA6C_902A5347FB4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcModelingSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingSetup dialog

class CCalcModelingSetup : public CGeneralBaseDlg
{
// Construction
public:
	CCalcModelingSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcModelingSetup)
	enum { IDD = IDD_CALC_MODELING_SETUP };
	CComboBox	m_comboSettle;
	CButton	m_radioGirderCen;
	double		m_BonusRate;
	double		m_JijumDownSize;
	double		m_AddLoad;
	double		m_TotalWeight;
	double		m_ModelWeight;
	CXTFlatEdit	m_Edit1;
	CXTFlatEdit	m_Edit2;
	CXTFlatEdit	m_Edit3;
	CXTFlatEdit	m_Edit4;
	CXTFlatEdit	m_Edit5;
	CXTFlatEdit	m_Edit6;
	CXTFlatEdit	m_EditSteelMass;
	BOOL	m_bCheckSettlement;
	double	m_dSteelMass;
	double	m_dAcceleration;
	//}}AFX_DATA
	CButton	m_btnMakeModeling;
	CButton	m_btnModelingView;

private:		
	void SetDataInit();
	void SetDataDefault();	
	void SetDataSave();
	void SetGridTitle(){};
	void OnPreInitDialog();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcModelingSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcModelingSetup)
	afx_msg void OnButtonMakemodel();
	afx_msg void OnCheckLoadForce();
	afx_msg void OnButtonRecommend();
	afx_msg void OnCheckSettlement();
	afx_msg void OnRadioGirderCen();
	afx_msg void OnRadioGirderLower();
	afx_msg void OnSelchangeComboSettle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCMODELINGSETUP_H__5AE39E75_F85A_4CCF_AA6C_902A5347FB4E__INCLUDED_)
