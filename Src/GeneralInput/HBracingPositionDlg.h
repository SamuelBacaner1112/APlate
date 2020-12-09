// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_HBRACINGPOSITIONDLG_H__F1C63304_A7A2_47DD_8C0D_C4C0BD335900__INCLUDED_)
#define AFX_HBRACINGPOSITIONDLG_H__F1C63304_A7A2_47DD_8C0D_C4C0BD335900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HBracingPositionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHBracingPositionDlg dialog

class CHBracingPositionDlg : public CGeneralBaseDlg
{
// Construction
public:
	CHBracingPositionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHBracingPositionDlg)
	enum { IDD = IDD_GEN_15_HBRACINGPOSITION };
	CGridCtrl	m_Grid;
	CGridCtrl	m_GridStd;	
	//CGridCtrl m_GridStd;
	int			m_nBaseHeightPosHBraceSplice;
	CXTFlatEdit	m_sBaseHeightFixPosHBraceSplice;
	CXTFlatEdit	m_sBaseHeightRatioPosHBraceSplice;
	//}}AFX_DATA
	CButton m_btnRemove;
	CButton m_btnSameAll;
	CButton m_btnAddSteel;
	CButton m_btnDelSteel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHBracingPositionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void SetDataInit();
	void SetDataInitStdSteel();
	void SetDataDefault();
	void SetDataSave();
	void SetDataBx();	
	void OnPreInitDialog();
	void SetDataCalcHBracing(CPlateBasicIndex *pBx);
	void SetDataSaveSteelMaterial(CBracing *pBracing, long nRow, long nCol);

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	long GetHBracingPosition(CPlateBasicIndex *pBx);

	// Grid
	void RemoveHBracing(long nType, CPlateBasicIndex *pBx);
	void SetGridTitle();
	void SetGridTitleStdSteel();
	void ClearAll();	
	void DeleteNullColumn();
	void GetSteelType(CString szTypeName, long &nSteelType, long &nJewonIdx, BOOL bSteel);
	void SetDataSaveStandardSteel();
	void SetHBracingEle();

	enum {POS_STT_END, POS_CEN_JIJUM, POS_GENERAL};
	
// Implementation
protected:
	CImageList m_ImageList;
	// Generated message map functions
	//{{AFX_MSG(CHBracingPositionDlg)	
	afx_msg void OnCellChanged(NMHDR* nmgv,LRESULT*);
	afx_msg void OnButtonRemoveAllHBracing();
	afx_msg void OnButtonSametype();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonPrev();
	afx_msg void OnDestroy();
	afx_msg void OnRadioBaseposUpperGirderLower();
	afx_msg void OnRadioBaseposLowerGirderUpper();
	afx_msg void OnRadioHeightByRato();
	afx_msg void OnButtonAddSteel();
	afx_msg void OnButtonDelSteel();
	//}}AFX_MSG
	//afx_msg void OnCellChangedDataGridStdSteel(WPARAM nmgv, LPARAM);
	afx_msg void OnKillfocusEditBaseHeight();
	afx_msg void OnKillfocusEditBaseRatio();
	afx_msg LRESULT OnHBracingInputClose(WPARAM wp, LPARAM lp);
	afx_msg void OnCellChangedDataGridStdSteel(NMHDR* nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HBRACINGPOSITIONDLG_H__F1C63304_A7A2_47DD_8C0D_C4C0BD335900__INCLUDED_)
