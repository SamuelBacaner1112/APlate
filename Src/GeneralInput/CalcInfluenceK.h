// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCINFLUENCEK_H__31171EDE_4DC9_4590_94AF_E0E26EB702AA__INCLUDED_)
#define AFX_CALCINFLUENCEK_H__31171EDE_4DC9_4590_94AF_E0E26EB702AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcInfluenceK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcInfluenceK dialog
#include "Resource.h"
#include "ResultDlg.h"
class CCalcInfluenceK : public CGeneralBaseDlg
{
// Construction
public:
	CCalcInfluenceK(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcInfluenceK)
	enum { IDD = IDD_CALC_INF_K };
	CString		m_sKFileName;
	CXTFlatEdit	m_ctrlEdit1;
	//}}AFX_DATA
	CGridCtrl m_Grid;
	long m_nGir;

	CButton	m_btnReCalcCoeff;
	CButton	m__btnMakeInput;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcInfluenceK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetGridTitle();
	void SetDataSave();
	void SetDataInit();
	void SetDataDefault(){};
	void SetFileName();


	void DrawInputDomyunView(BOOL bZoomAll);
	void DrawInfK(CDomyun* pDom, long nG, BOOL bExcel);
	void PathToPathNum(CString & path,CString csExt, long Num) const;
	void OnPreInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CCalcInfluenceK)
	afx_msg void OnButtonMakeS2k();
	afx_msg void OnRecalcInfluence();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR*nmgv,LRESULT*);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCINFLUENCEK_H__31171EDE_4DC9_4590_94AF_E0E26EB702AA__INCLUDED_)
