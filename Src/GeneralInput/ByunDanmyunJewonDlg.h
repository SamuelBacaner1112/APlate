// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BYUNDANMYUNJEWONDLG_H__0F4DFAD0_B689_4423_B2F5_C0E9922DA222__INCLUDED_)
#define AFX_BYUNDANMYUNJEWONDLG_H__0F4DFAD0_B689_4423_B2F5_C0E9922DA222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ByunDanmyunJewonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CByunDanmyunJewonDlg dialog

class CByunDanmyunJewonDlg : public CGeneralBaseDlg
{
// Construction
public:
	CByunDanmyunJewonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CByunDanmyunJewonDlg)
	enum { IDD = IDD_GEN_12_BYUNDANMYUNJEWON };
	CXTTabCtrl	m_ctlTab;
	CXTPButton		m_btnType4;
	CXTPButton		m_btnType3;
	CXTPButton		m_btnType2;
	CXTPButton		m_btnType1;
	CGridCtrl	m_Grid;
	BOOL	m_bUseByunDan;
	CString	m_szArcInfo;
	int		m_nArcType;
	//}}AFX_DATA
	CBitmap m_bitmap[4];
	long m_nTab;
	long m_nByunDanType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CByunDanmyunJewonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataDefaultByStyleByun();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);	
	void OnCellChangedData(NMHDR* nmgv,LRESULT*);

	void SetGridTitle();
	void OnPreInitDialog();
	void OnCursorChange(long nRow, long nCol);

	//
	void CheckSelectedButton();
	void EnableByunDan(long nTab);
	void CalcArcInfo();
	void InKyungTotal();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CByunDanmyunJewonDlg)
	afx_msg void OnSelchangeTabSel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckUsebyun();
	afx_msg void OnButtonType1();
	afx_msg void OnButtonType2();
	afx_msg void OnButtonType3();
	afx_msg void OnButtonType4();
	afx_msg void OnRadioCircletype1();
	afx_msg void OnRadioCircletype2();
	afx_msg void OnRadioCircletype3();
	afx_msg void OnButtonRecommend();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BYUNDANMYUNJEWONDLG_H__0F4DFAD0_B689_4423_B2F5_C0E9922DA222__INCLUDED_)
