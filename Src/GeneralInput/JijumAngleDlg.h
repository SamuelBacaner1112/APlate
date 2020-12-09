// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_JIJUMANGLEDLG_H__93829A81_F3CE_49D4_9D29_A800235DFCEE__INCLUDED_)
#define AFX_JIJUMANGLEDLG_H__93829A81_F3CE_49D4_9D29_A800235DFCEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JijumAngleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJijumAngleDlg dialog

class CJijumAngleDlg : public CGeneralBaseDlg
{
// Construction 
public:
	CJijumAngleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJijumAngleDlg)
	enum { IDD = IDD_GEN_06_JIJUMANGLE };
	CXTFlatEdit	m_ctrlGirStndCuttingAngle;
	CGridCtrl m_Grid;
	int		m_nShoeDir;	
	BOOL	m_bStartOffset;
	BOOL	m_bEndOffset;
	int		m_nGirEndDir;
	double	m_dGirStndCuttingAngle;
	//}}AFX_DATA
	CButton		m_btnAngRight;		
	CButton		m_btnAngQual;		
	CButton		m_btnAngParallel;		
	CButton		m_btnShoeCordinate;	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJijumAngleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void OnButtonApply();
	void SetDataSave(BOOL bSttAbut = FALSE, BOOL bEndAbut = FALSE);
	void SetDataBx();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	// Grid ¼³d
	void SetGridTitle();

// Implementation
protected:	

	// Generated message map functions
	//{{AFX_MSG(CJijumAngleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSttoffset();
	afx_msg void OnCheckEndoffset();
	afx_msg void OnShoeDir1();
	afx_msg void OnShoeDir2();
	afx_msg void OnShoeDir3();
	afx_msg void OnShoeDir4();
	afx_msg void OnAngleA1Equal();
	afx_msg void OnAngleA1Parallel();
	afx_msg void OnAngleRighTang();
	afx_msg void OnShoeCoordiante();
	afx_msg void OnRadioGirendCtVer();
	afx_msg void OnRadioGirendSaPyung();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JIJUMANGLEDLG_H__93829A81_F3CE_49D4_9D29_A800235DFCEE__INCLUDED_)
