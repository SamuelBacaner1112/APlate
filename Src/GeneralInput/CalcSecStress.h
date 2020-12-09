// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCSECSTRESS_H__E53C9C15_0962_4F7C_BA4E_9AE5C797CEA4__INCLUDED_)
#define AFX_CALCSECSTRESS_H__E53C9C15_0962_4F7C_BA4E_9AE5C797CEA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcSecStress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcSecStress dialog

class CCalcSecStress : public CGeneralBaseDlg
{
// Construction
public:
	CCalcSecStress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcSecStress)
	enum { IDD = IDD_CALC_SECTION_STRESS };
	CButton		m_btnCalcSec;
	BOOL		m_bBuckling;
	BOOL		m_bRenderingStress;
	int			m_nViewStress;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcSecStress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DrawInputDomyunView(BOOL bZoomAll);
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void OnPreInitDialog();
	void MakeSectionStressFrame();

	// Generated message map functions
	//{{AFX_MSG(CCalcSecStress)
	afx_msg void OnRadioDanStress();
	afx_msg void OnRadioLowerCompStress();
	afx_msg void OnRadioLowerStress();
	afx_msg void OnRadioShearStress();
	afx_msg void OnRadioUpperCompStress();
	afx_msg void OnRadioUpperStress();
	afx_msg void OnButtonCalcStress();
	afx_msg void OnCheckViewStress();
	afx_msg void OnCheckBuckling();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCSECSTRESS_H__E53C9C15_0962_4F7C_BA4E_9AE5C797CEA4__INCLUDED_)
