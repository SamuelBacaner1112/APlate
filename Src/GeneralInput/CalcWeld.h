// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCWELD_H__11B01DE9_8D5F_4D47_AE98_D6DED0473E47__INCLUDED_)
#define AFX_CALCWELD_H__11B01DE9_8D5F_4D47_AE98_D6DED0473E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcWeld.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcWeld dialog

class CCalcWeld : public CGeneralBaseDlg
{
// Construction
public:
	CCalcWeld(CWnd* pParent = NULL);   // standard constructor

	void Output_SectionSpec(long nType, CShRichEdit &RichEdit);	
// Dialog Data
	//{{AFX_DATA(CCalcWeld)
	enum { IDD = IDD_CALC_WELD };
	CShRichEdit	m_richWeld;		
	CXTFlatComboBox	m_ComboBox1;
	//}}AFX_DATA
	CButton	m_btnOutText;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcWeld)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
// Implementation
protected:
	void SetDataInit();
	void SetDataSave();
	void SetDataDefault();
	void OnPreInitDialog();
	void EditValueOut(int nID, double dValue, char *pFormat);
	void StaticTextOut(int nID, CString &sText);
	void GetJijumVStiffBxByMaxReaction(BOOL bPier);

	double	m_dMaxReaction;
	long	m_nMaxJoint;
	CTypedPtrArray < CObArray, CPlateBasicIndex* > m_ArrBxAbut;
	CTypedPtrArray < CObArray, CPlateBasicIndex* > m_ArrBxPier;

	// Generated message map functions
	//{{AFX_MSG(CCalcWeld)
	afx_msg void OnSelchangeWeldCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCWELD_H__11B01DE9_8D5F_4D47_AE98_D6DED0473E47__INCLUDED_)
