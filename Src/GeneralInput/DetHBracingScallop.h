// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETHBRACINGSCALLOP_H__F0A2C05E_E161_403C_A139_7E497D01C7BA__INCLUDED_)
#define AFX_DETHBRACINGSCALLOP_H__F0A2C05E_E161_403C_A139_7E497D01C7BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetHBracingScallop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetHBracingScallop dialog

class CDetHBracingScallop : public TStackedPage
{
// Construction
public:
	CDetHBracingScallop(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor
	CGeneralBaseStd *m_pStd;
	CDomyunGLView	*m_pView;				///< GL µµ¸é ºä

// Dialog Data
	//{{AFX_DATA(CDetHBracingScallop)
	enum { IDD = IDD_DET_HBRACING_SCALLOP };
	CGridCtrl m_Grid_HVracing;
	CGridCtrl m_Grid_Scallop;
	CGridCtrl m_Grid_FieldScallop;
	BOOL m_bApplyDefault;
	//}}AFX_DATA
	long m_nCol;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetHBracingScallop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:	
//	void OnScallopType();
	void OnPreInitDialog();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetGridTitle();
	void SetGridTitle_HVracing();
	void SetGridTitle_Scallop();
	void SetGridTitle_FieldScallop();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);

	void DrawChamfer(CDomyun *pDomP);
	void DimChamfer(CDomyun *pDomP);
	void DrawFieldWeld(CDomyun *pDomP);
	void DimFieldWeld(CDomyun *pDomP);

	//BOOL IsValid();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetHBracingScallop)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckApplyDefaultSlot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnCellChanged_HVracing(WPARAM nmgv,LPARAM);
	afx_msg void OnCellChangedData_HVracing(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedCol_HVracing(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData_SCallop(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData_FieldSCallop(NMHDR* nmgv, LRESULT*);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETHBRACINGSCALLOP_H__F0A2C05E_E161_403C_A139_7E497D01C7BA__INCLUDED_)
