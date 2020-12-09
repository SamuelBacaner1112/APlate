// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCSECSTRESSPOS_H__6E757153_0B91_4C39_A757_C7C41BBB0223__INCLUDED_)
#define AFX_CALCSECSTRESSPOS_H__6E757153_0B91_4C39_A757_C7C41BBB0223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcSecStressPos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcSecStressPos dialog
class CCalcSecStressPos : public CDialog
{
// Construction
public:
	CCalcSecStressPos(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor

	void InitDlgData(long nG, double dSta);
	double m_dLength;
	double m_dCheckLength;
	CString m_sElementInfo;
	CString m_sNodeInfo;
	CString m_sGirSta;

	CGeneralBaseStd *m_pStd;

// Dialog Data
	//{{AFX_DATA(CCalcSecStressPos)
	enum { IDD = IDD_CALC_SECTION_STRESS_POS };
	double	m_dCheckPose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcSecStressPos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcSecStressPos)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCSECSTRESSPOS_H__6E757153_0B91_4C39_A757_C7C41BBB0223__INCLUDED_)
