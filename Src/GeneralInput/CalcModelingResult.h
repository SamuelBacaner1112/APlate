// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCMODELINGRESULT_H__3FA3E288_0005_42E9_A84C_45AF5124D24D__INCLUDED_)
#define AFX_CALCMODELINGRESULT_H__3FA3E288_0005_42E9_A84C_45AF5124D24D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcModelingResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingResult dialog
#include "ResultDlg.h"
class AFX_EXT_CLASS CCalcModelingResult : public CGeneralBaseDlg
{
// Construction
public:
	CCalcModelingResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcModelingResult)
	enum { IDD = IDD_CALC_MODELING_RESULT };
	CXTColorPicker	m_btnBkChange;
	CSliderCtrl	m_ctrlSliderDispScale;
	CSliderCtrl	m_ctrlSliderAlpha;
	CSliderCtrl	m_ctrlSliderScale;
	CString		m_sSapInputFileName;
	CXTFlatEdit	m_ctrlEdit1;	
	BOOL		m_bShowJoint;
	BOOL		m_bShowElement;
	BOOL		m_bShowRestraint;
	BOOL		m_bShowResultData;
	double	m_dTextSizeJoint;
	double	m_dTextSizeDataValue;
	double	m_dTextSizeElement;
	int		m_nSolveType;
	int		m_nRadioForce;
	//}}AFX_DATA
	CResultDlg m_CalcDlg;
	CGridCtrl m_Grid;

	CXTFlatComboBox	m_comboResultType;
	CXTFlatComboBox	m_comboLoad;
	CXTFlatComboBox	m_comboVersion;
	CXTFlatComboBox	m_comboLoadType;
	CButton	m_btnSaveImage;
	CButton	m_btnLoadApply;
	CButton	m_btnSection;
	CButton	m_btnMemberForce;
	CButton	m_btnReaction;
	CButton	m_btnRizing;
	CButton	m_btnSapRun;
	CButton	m_btnSapInput;
	CButton	m_btnFileRead;
	CButton	m_btnFileDelete;
	CButton	m_btnBoundary;
	CButton	m_btnElement;
	CLongArray	m_ResultIdx;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcModelingResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCalcModelingResult)
	afx_msg void OnSelchangeLoadCase();
	afx_msg void OnSelchangeElemForce();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonMakeSapS2k();
	afx_msg void OnButtonAutoRun();
	afx_msg long OnCalcOK(WPARAM, LPARAM);
	afx_msg void OnFilePrintElemForce();
	afx_msg void OnFilePrintReaction();
	afx_msg void OnFilePrintCamber();
	afx_msg void OnShowJointNo();
	afx_msg void OnShowElemNo();
	afx_msg void OnShowRestraint();
	afx_msg void OnShowData();
	afx_msg void OnButtonSapAdd();
	afx_msg void OnButtonSapDelete();
	afx_msg void OnRadioShear22();
	afx_msg void OnRadioMoment22();
	afx_msg void OnRadioAxialForce();
	afx_msg void OnRadioDisplacement();
	afx_msg void OnRadioShear33();
	afx_msg void OnRadioMoment33();
	afx_msg void OnRadioAxialTorsion();
	afx_msg void OnRadioReaction();
	afx_msg void OnCheckLoadforce();
	afx_msg void OnSelchangeComboLoadType();
	afx_msg void OnRadioViewYz();
	afx_msg void OnRadioViewXz();
	afx_msg void OnRadioViewXy();
	afx_msg void OnRadioView3d();
	afx_msg void OnSelEndOkBackClr();
	afx_msg void OnButtonSaveImagefile();
	afx_msg void OnButtonSectionCoeffDlg();
	afx_msg void OnButtonJointViewDlg();
	afx_msg void OnSelchangeComboSapVersion();
	afx_msg void OnSelchangeComboResultType();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

protected:
	CFEMManage	m_FEMTotal;
	BOOL	m_bCheckLoadType;
	long m_nFileNo;
	char m_cAxis;
	void DrawInputDomyunView(BOOL bZoomAll);

	void MakeTotalData();
	void OnPreInitDialog();
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault();	
	void SetDataSave();
	void SetComboData(long nFilePos);
	void OnButtonPrev();
	void SetFileName();
	void SetLoadTypeCombo();

	void GetDirectoryName(CString &strDirName, const CString& strPathName);
	void PathToPathNum(CString & path,CString csExt, long Num) const;

public:
	BOOL ReadOutFile(CString szFile, BOOL bMessage=TRUE);
	CString GetLoadName(long nAnalysisType);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCMODELINGRESULT_H__3FA3E288_0005_42E9_A84C_45AF5124D24D__INCLUDED_)
