// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKMAINREBARCTCDLG_H__C53B1D68_7057_4CF4_BA53_9840F0C021F1__INCLUDED_)
#define AFX_DECKMAINREBARCTCDLG_H__C53B1D68_7057_4CF4_BA53_9840F0C021F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckMainRebarCTCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckMainRebarCTCDlg dialog

class CDeckMainRebarCTCDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckMainRebarCTCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckMainRebarCTCDlg)
	enum { IDD = IDD_DECK_060_MAINREBAR_CTC };
	CXTTabCtrl	 m_ctlTab;
	CGridCtrlMng m_Grid;
	CGridCtrlMng m_GridDim;
	CComboBox	 m_comboMainCTCExtra;	// �ܿ�ġ��
	CComboBox	 m_comboMainCTCBasePos;	// ������ġ
	CXTFlatEdit	 m_dRangeError;			// ���ձ��ر���
	CXTFlatEdit	 m_dRangeDanbuStt;		// �ܺκ�����ö�� ��ġ����(����)
	CXTFlatEdit	 m_dRangeDanbuEnd;		// �ܺκ�����ö�� ��ġ����(����)
	BOOL		 m_bCheckBox;			// ���������κ���
	BOOL		 m_bShowTotal;			// ��üö�ٺ���	
	int			 m_nPlanViewType;		// ��ü��, �и���
	BOOL	m_bCheckAutoCTC;

	//}}AFX_DATA
	CButton	m_btnError;
	CButton	m_btnSameAll;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckMainRebarCTCDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckMainRebarCTCDlg)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboMainCTCBasePos();
	afx_msg void OnSelchangeComboMainCTCExtraOption();
	afx_msg void OnButtonCalcRealToDim();
	afx_msg void OnCheckBox();
	afx_msg void OnCheckShowMainRebarUpDn();
	afx_msg void OnCheckShowMainRebarTotal();
	afx_msg void OnRadioOneUpDnView();
	afx_msg void OnRadioTwoView();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonApply();
	afx_msg void OnButtonSameAll();
	//}}AFX_MSG
	afx_msg void OnKillfocusEditSlabStt();
	afx_msg void OnKillfocusEditSlabEnd();
	afx_msg void OnKillfocusEditError();
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);	
	afx_msg void OnCellChangedDataDim(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertedColDim(NMHDR* nmgv, LRESULT*);	
	afx_msg void OnCellDeletedColDim(NMHDR* nmgv, LRESULT*);	
	DECLARE_MESSAGE_MAP()

private:
	void InitTabCtrl();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void SetGridData();
	void SetGridDataDim();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	CString GetCurTabName();
	long GetTabCount();	
	CString GetTabTitle(long nIdx);

	void DrawPlanSlab(CDomyun *pDom, long nType=0);	// nType=-1 : ���,  nType=0 :���ϸ鵿��, nType=1 : �ϸ� 
	void DrawPlanRebar(CDomyun *pDom);
	void DrawJongRebar(CDomyun *pDom);
	
	void InsertCTCByGridCol(long nGridCol, BOOL bDim);
	void DeleteCTCByGridCol(long nGridCol, BOOL bDim);

	CDPointArray* GetCTCRebar(long nPosSlab, BOOL bRealCTC, CString szCTC="");
	void SetCTCRebar(CDPointArray &xyArrCTC, long nPosSlab, BOOL bRealCTC, CString szCTC="");
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKMAINREBARCTCDLG_H__C53B1D68_7057_4CF4_BA53_9840F0C021F1__INCLUDED_)
