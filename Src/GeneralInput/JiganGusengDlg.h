// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_JIGANGUSENGDLG_H__D9914FEE_7865_41E9_9EDD_2BEDFBE5A715__INCLUDED_)
#define AFX_JIGANGUSENGDLG_H__D9914FEE_7865_41E9_9EDD_2BEDFBE5A715__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JiganGusengDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJiganGusengDlg dialog

class CJiganGusengDlg : public CGeneralBaseDlg
{
// Construction
public:
	CJiganGusengDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJiganGusengDlg)
	enum { IDD = IDD_GEN_05_JIGANGUSENG };
	CGridCtrl	m_Grid;
	CGridCtrl	m_GridExp;
	CXTFlatEdit	m_dBindConcLen_Stt;
	CXTFlatEdit	m_dBindConcLen_End;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJiganGusengDlg)  
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CButton		m_btnShoeFix;
	CButton		m_btnRecommend;
	CButton		m_btnExpType;
	CButton		m_btnExpCheck;
	CButton		m_btnAutoCalc;

private:	
	void SetJiganLastLen();
	void SetDataBx();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void OnCursorChange(long nRow, long nCol);
	void OnButtonNext();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);	

	void SetGridTitle();
	void SetSttEndUgan(double shoeX);
	CString GetLineInfo();
	BOOL CheckShoeDirFixExist();
	long	m_nIdxExp;
	double GetDistGirder(long nJijum, BOOL bPre);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJiganGusengDlg)
	afx_msg void OnButtonShoefix();
	afx_msg void OnButtonAutocalc();
	afx_msg void OnButtonExpansionjoitnType();
	afx_msg void OnExpansionjointCheck();
	afx_msg void OnButtonRecommendExpansionJoint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg long OnExpansionRecommend(WPARAM wp, LPARAM lp);
	afx_msg void OnKillfocusEditBindconcStt();
	afx_msg void OnKillfocusEditBindconcEnd();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JIGANGUSENGDLG_H__D9914FEE_7865_41E9_9EDD_2BEDFBE5A715__INCLUDED_)
