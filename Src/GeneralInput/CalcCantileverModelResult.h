// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCCANTILEVERMODELRESULT_H__D60F6559_FD9B_4488_9185_CB0957E90BED__INCLUDED_)
#define AFX_CALCCANTILEVERMODELRESULT_H__D60F6559_FD9B_4488_9185_CB0957E90BED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcCantileverModelResult.h : header file
//

class CFileProgressDlg;
/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverModelResult dialog

class CCalcCantileverModelResult : public CGeneralBaseDlg
{
// Construction
public:
	CCalcCantileverModelResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcCantileverModelResult)
	enum { IDD = IDD_CALC_CANTILEVER_MODELRESULT };
	//}}AFX_DATA
	CXTColorPicker	m_btnBkChange;
	CButton		m_btnSaveImage;
	CButton		m_btnFileRead;
	CButton		m_btnMakeData;
	CButton		m_btnMidasRun;
	CButton		m_btnViewCalc;
	CSliderCtrl		m_ctlSlider;
	CSliderCtrl		m_ctlDispSlider;
	CXTFlatComboBox	m_ComboLoadCombo;
	CXTFlatComboBox	m_ComboLoadCase;

	BOOL			m_bCheckLoadType;
	BOOL			m_bCheckViewJoint;
	BOOL			m_bChceckViewElement;
	BOOL			m_bCheckViewOutValue;
	BOOL			m_bCheckViewRestraint;
	int				m_nRadioForce;
	int				m_nRadioView;
	double			m_dTextSizeJoint;
	double			m_dTextSizeDataValue;
	double			m_dTextSizeElement;
	CString			m_szPath;
	CFileProgressDlg* m_pProgressDlg;

protected:
	CFEMManage		m_FEMTotal;
	char			m_cAxis;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcCantileverModelResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL IsValid();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void SetDataSave();
	void SetDataDefault();
	void SetDataInit();

	void SetLoadComboCombo();
	void SetLoadCaseCombo();
	void SetFileName();
	BOOL FileExist(LPCTSTR lpPathName);

	// Generated message map functions	
	//{{AFX_MSG(CCalcCantileverModelResult)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnButtonBackGroundColr();
	afx_msg void OnButtonSaveImageFile();
	afx_msg void OnSelchangeComboLoadcase();
	afx_msg void OnCheckLoadtype();
	afx_msg void OnSelchangeComboLoadtype();

	afx_msg void OnCheck_Shear22();
	afx_msg void OnCheck_Moment22();
	afx_msg void OnCheck_AxialForce();
	afx_msg void OnCheck_Displace();
	afx_msg void OnCheck_Shear33();
	afx_msg void OnCheck_Moment33();
	afx_msg void OnCheck_AxialTorsion();
	afx_msg void OnCheck_Reaction();
	afx_msg void OnCheck_NodeNum();
	afx_msg void OnCheck_MemberNum();
	afx_msg void OnCheck_Boundary();
	afx_msg void OnCheck_ResultValue();
	afx_msg void OnCheck_Plan_YZ();
	afx_msg void OnCheck_Plan_XZ();
	afx_msg void OnCheck_Plan_XY();
	afx_msg void OnCheck_Plan_3D();

	afx_msg void OnButtonReadOutputFile();
	afx_msg void OnButtonMidasFileMake();
	afx_msg void OnButtonMiaasRun();
	afx_msg void OnButtonResultViewTable();
	afx_msg LRESULT OnRunThread(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
	
private:
	void OnPreInitDialog();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCCANTILEVERMODELRESULT_H__D60F6559_FD9B_4488_9185_CB0957E90BED__INCLUDED_)
