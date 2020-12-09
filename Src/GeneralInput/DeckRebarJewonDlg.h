// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKREBARJEWONDLG_H__3281E59C_0A49_4248_AA31_E799C3440B78__INCLUDED_)
#define AFX_DECKREBARJEWONDLG_H__3281E59C_0A49_4248_AA31_E799C3440B78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckRebarJewonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckRebarJewonDlg dialog
class CDeckRebarJewonDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckRebarJewonDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDeckRebarJewonDlg)
	enum { IDD = IDD_DECK_09_REBARJEWON };
	CSliderCtrl	m_sliderScale;
	CString		m_strScale;
	CComboBox	m_ctrlComboADDRate;
	//}}AFX_DATA

	CXTTabCtrl	m_ctlTab;
	CGridCtrl	m_Grid;
	CRebarTableDlg	m_RebarTableDlg;
	BOOL		m_bLockAll;

	double	m_dScaleHV;
	long	m_nTab;

	CButton	m_btnAdd;
	CButton	m_btnDel;
	CButton	m_batnRatio;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckRebarJewonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckRebarJewonDlg)
	afx_msg void OnRebarAddRate();
	afx_msg void OnRemoveRebar();
	afx_msg void OnAddRebar();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboAddRate();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	BOOL IsValid();
	void OnPreInitDialog();
	void SetGridTitle();
	void SetDataInit();
	void SetDataInitSubRebar(CRebar *pRB, long nRow);
	void SetTextTotalWeight();
	void SetDataDefault();
	void SetDataSave();

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	void DrawGuardFence(CDomyun* pDom);
	void DrawCrStgDetail(CDomyun* pDom);
	void DrawEtcRebar(CDomyun* pDom);
	void DrawJDan(CDomyun* pDom);
	void DrawHDan(CDomyun* pDom);
	void DrawRebarDetail(CDomyun *pDom);
	void DrawTUGirder(CDomyun *pDom);

	vector<CRebar*> GetRebarData();
	void OnCursorChange(long nRow,long nCol);	
	CString GetCurTabName();
	long GetRebarType();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKREBARJEWONDLG_H__3281E59C_0A49_4248_AA31_E799C3440B78__INCLUDED_)
