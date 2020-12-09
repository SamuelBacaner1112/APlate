// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKBMOPTIONDLG_H__1D13BA55_A6C6_416E_AF43_E897B80522E2__INCLUDED_)
#define AFX_DECKBMOPTIONDLG_H__1D13BA55_A6C6_416E_AF43_E897B80522E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckBMOptionDlg.h : header file
//
#include "ShCheckButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDeckBMOptionDlg dialog
 
class CDeckBMOptionDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckBMOptionDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL	IsValid();
	void	OnPreInitDialog();
	void	SetGridTitle();
	void	SetDataInit();
	void	SetDataSave();
	void	SetDataDefault();
	void	GetBMOptionContents(CStringArray &Arr);
	void	DrawInputDomyunView(BOOL bZoomAll=TRUE);

protected:
	CGridCtrl	m_Grid;
	long		m_nCols;
	long		m_nRows;
	long		m_nSelectedOpt;
	void		GetComboRebarDia(CStringArray &strComboArr);
	void		OnGridRedraw();
	BOOL		IsApply(CString szApply);
	CString		IsApply(BOOL bApply);
	CString		IsApplyStt(BOOL bApply);
	CString		IsApplyEnd(BOOL bApply);
	CString		IsApplyLeft(BOOL bApply);
	CString		IsApplyRight(BOOL bApply);
	BOOL		IsLeft(CString szApply);
	void		SetOptionAll(BOOL bApply);
// Dialog Data
	//{{AFX_DATA(CDeckBMOptionDlg)
	enum { IDD = IDD_DECK_11_BM_OPTION };
	CShCheckButton	m_ElecWireHole;
	CShCheckButton	m_Timber;
	CShCheckButton	m_WingWall;
// 	CShCheckButton	m_AllCancel;
// 	CShCheckButton	m_AllCheck;
	CShCheckButton	m_DrainBridgeSurface;
	CShCheckButton	m_Shade;
	CShCheckButton	m_Pave;
	CShCheckButton	m_Notch;
	CShCheckButton	m_WaterDraw;
	CShCheckButton	m_TackCoating;
	CShCheckButton	m_Styrofoam;
	CShCheckButton	m_SteelQuantity;
	CShCheckButton	m_Spacer;
	CShCheckButton	m_SoundProof;
	CShCheckButton	m_ShrinkageConcrete;
	CShCheckButton	m_RebarManufacture;
	CShCheckButton	m_Parapet;
	CShCheckButton	m_Paint;
	CShCheckButton	m_Mold;
	CShCheckButton	m_GuardFence;
	CShCheckButton	m_ExplainPlate;
	CShCheckButton	m_ExpansionJointCover;
	CShCheckButton	m_ExpansionJoint;
	CShCheckButton	m_EstablishTBM;
	CShCheckButton	m_DroppingPreVent;
	CShCheckButton	m_DrainEstablish;
	CShCheckButton	m_ConcretePlacing;
	CShCheckButton	m_ConcreteBuy;
	CShCheckButton	m_BridgeNamePlate;
	CShCheckButton	m_BridgeName;
	CRichEditCtrl	m_OptContents;
	//}}AFX_DATA
	CButton	m_btnOptSave;
	CButton	m_btnOptLoad;
	CButton	m_AllCheck;
	CButton	m_AllCancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckBMOptionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckBMOptionDlg)
	afx_msg void OnCheckConcreteBuy();
	afx_msg void OnCheckConcretePlacing();
	afx_msg void OnCheckMold();
	afx_msg void OnCheckSpacer();
	afx_msg void OnCheckRebarManufacture();
	afx_msg void OnCheckPave();
	afx_msg void OnCheckTackCoating();
	afx_msg void OnCheckDrainBridgesurface();
	afx_msg void OnCheckBridgeName();
	afx_msg void OnCheckBridgeNameplate();
	afx_msg void OnCheckExplainPlate();
	afx_msg void OnCheckEstablishTbm();
	afx_msg void OnCheckStyrofoam();
	afx_msg void OnCheckExpansionjoint();
	afx_msg void OnCheckExpansionjointCover();
	afx_msg void OnCheckShrinkageConcrete();
	afx_msg void OnCheckWaterDraw();
	afx_msg void OnCheckNotch();
	afx_msg void OnCheckGuardFence();
	afx_msg void OnCheckSoundProof();
	afx_msg void OnCheckParapet();
	afx_msg void OnCheckShade();
	afx_msg void OnCheckDroppingPrevent();
	afx_msg void OnCheckSteelQuantity();
	afx_msg void OnCheckPaint();
	afx_msg void OnCheckDrainEstablish();
	afx_msg void OnButtonAllCheck();
	afx_msg void OnButtonAllCancel();
	afx_msg void OnCheckWingWall();
	afx_msg void OnCheckTimber();
	afx_msg void OnCheckElecWireHole();
	afx_msg void OnButtonOptionSave();
	afx_msg void OnButtonOptionLoad();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg LRESULT OnReceive(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKBMOPTIONDLG_H__1D13BA55_A6C6_416E_AF43_E897B80522E2__INCLUDED_)
