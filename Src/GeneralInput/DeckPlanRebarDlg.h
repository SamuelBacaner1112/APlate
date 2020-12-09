// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKPLANREBARDLG_H__387F8EEC_9E56_444A_A9FD_311FD4BA460E__INCLUDED_)
#define AFX_DECKPLANREBARDLG_H__387F8EEC_9E56_444A_A9FD_311FD4BA460E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckPlanRebarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckPlanRebarDlg dialog
class AFX_EXT_CLASS CDeckPlanRebarDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckPlanRebarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckPlanRebarDlg)
	enum { IDD = IDD_DECK_07_PLANREBAR };
	CGridCtrlMng	m_Grid;
	CXTFlatEdit		m_dTrimRatio;
	BOOL			m_bShowSlabHunch;
	CXTFlatEdit		m_szDiaCB;
	CXTFlatEdit		m_szDiaCant;
	CXTFlatEdit		m_szDiaDan;
	CXTFlatEdit		m_szDiaOutBk;
	CXTFlatEdit		m_szDiaStringer;
	int				m_nCurSangsePos;
	//}}AFX_DATA
	CButton	m_btnVerGirder;
	CButton	m_btnSideGirder;
	CButton	m_btnHorGirder;
	CButton	m_btnDanbu;
	CButton	m_btnCantil;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckPlanRebarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckPlanRebarDlg)
	afx_msg void OnCheckShowSlabhunch();
	afx_msg void OnRadioCant();
	afx_msg void OnRadioCB();
	afx_msg void OnRadioDan();
	afx_msg void OnRadioOutBk();
	afx_msg void OnRadioSt();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnKillfocusEditCB();
	afx_msg void OnKillfocusEditCant();
	afx_msg void OnKillfocusEditDan();
	afx_msg void OnKillfocusEditOutbk();
	afx_msg void OnKillfocusEditSt();
	afx_msg void OnKillfocusEditTrim();
	DECLARE_MESSAGE_MAP()
		
public:
	BOOL IsValid();
	void OnPreInitDialog();
	void SetPosition(CPoint ptPoint);
	CDPoint FindXyPosByIndexCB(long nG, long nCB);
	CDPoint FindXyPosByIndexST(long nG, long nCB);
	CDPoint FindXyPosByIndexOBK(BOOL bLeftSlab, long nObk);
	void SetGridData();
	
	void SetDataDefault();
	void SetDataDefaultByTrimRatio();
	void SetDataInit();
	void SetDataSave();

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);

	long GetTabCount();
	CString GetTabTitle(long nIdx);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKPLANREBARDLG_H__387F8EEC_9E56_444A_A9FD_311FD4BA460E__INCLUDED_)
