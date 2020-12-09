// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_HDANDLG_H__7D91A01F_0A6C_4654_B391_A601ED5CAF8A__INCLUDED_)
#define AFX_HDANDLG_H__7D91A01F_0A6C_4654_B391_A601ED5CAF8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HDanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHDanDlg dialog
// 횡단구성입력

class CHDanDlg : public CGeneralBaseDlg
{
// Construction
public:
	CHDanDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHDanDlg)
	enum { IDD = IDD_GEN_03_HDAN };
	CXTFlatEdit	m_ctlMoveDist;
	CXTFlatComboBox	m_ComboGirsu;
	CGridCtrl		m_Grid;
	CGridCtrl		m_GridN;
	int m_nGirderSu;
	long m_nCurGirder;
	long m_nCurJijum;
	double	m_dMoveDist;
	BOOL	m_bCheckJijumSame;
	//}}AFX_DATA
	CButton	m_btnRecommend;
	CButton	m_btnOffset;
	CButton	m_btnMove;
	CButton	m_btnSameAll;
	CButton	m_btnConc;
	CButton	m_btnCalc;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHDanDlg)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support		
	//}}AFX_VIRTUAL

private:
	//void CalcGirderPosAuto();
	//void CalcGirderPosAutoByCantilever();
	CSelGirderHeightRangeDlg m_UserDlg;
	CBitmap	m_BitMap;

	void SetGridTitle();
	void SetDataDefault();
	void SetDataSave();
	void SetDataBx();
	BOOL IsFocusItem(CString szTitle);
	void SetFocusItem(CString szTitle);
	void OnCursorChange(long nRow, long nCol);

	long GetTabCount();	
	CString GetTabTitle(long nIdx);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHDanDlg)
	afx_msg void OnSelchangeHdanGirdersu();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCalcAuto();	
	afx_msg void OnButtonRecommend();
	afx_msg void OnButtonOffsetonl1();
	afx_msg void OnButtonOffsetonl2();
	afx_msg void OnAllGirderSame();
	afx_msg void OnRistRecommend();
	afx_msg void OnBindConc();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedGridN(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataGridN(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void SetDataInit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HDANDLG_H__7D91A01F_0A6C_4654_B391_A601ED5CAF8A__INCLUDED_)
