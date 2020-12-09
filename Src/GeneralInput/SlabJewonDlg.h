// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SLABJEWONDLG_H__85B24936_C9EF_4E46_B04C_D96358C5F670__INCLUDED_)
#define AFX_SLABJEWONDLG_H__85B24936_C9EF_4E46_B04C_D96358C5F670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlabJewonDlg.h : header file
//
#include "ShCheckButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSlabJewonDlg dialog

class CSlabJewonDlg : public CGeneralBaseDlg
{
// Construction
public:
	CSlabJewonDlg(CWnd* pParent = NULL);   // standard constructor

protected:
	static SHGRIDTITLE m_gt[9];
	long m_nQtyHDanNode;

	CStringArray 	m_strLevel;
	CStringArray 	m_strType;
	CStringArray 	m_strSwitch;
// Dialog Data
	//{{AFX_DATA(CSlabJewonDlg)
	enum { IDD = IDD_GEN_20_SLABJEWON };
	CShCheckButton	m_SlabHunch;
	CShCheckButton	m_Tendon;
	CComboBox	m_ctlCombo;
	CGridCtrl	m_Grid;
	CGridCtrl	m_GridT;
	double m_dWidthGuardFenceL;
	double m_dWidthGuardFenceR;
	double m_dWidthMarginalStripeL;
	double m_dWidthMarginalStripeR;
	double m_dWidthShoulderL;
	double m_dWidthShoulderR;
	double m_dWidthLane;
	BOOL	m_bLevelCenter;
	//}}AFX_DATA
	CButton m_btnSlabStruct;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlabJewonDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataSave();
	void SetGridTitle();
	void SetDataDefault();
	void SetDataSaveGuard();
	BOOL IsFocusItem(CString szTitle);
	void SetFocusItem(CString szTitle);
	void SetGridFocusCell(long nRow, long nCol);

	BOOL IsChangeSlabDataBeforeSave();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void SetDataDefaultGuard(long nGuard);
	void OnPreInitDialog();
	void OnCheckTendon();

protected:
	void SetInitPositionCombo();
	CPlateBasicIndex* GetPosBx();
	void SetGridCellInsertedCol(int nCol);
	long GetStringValue(const CString &str) const;
	void OnButtonNext();
	long GetTabCount() { return 3; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "플랜지폭" : (nIdx==1 ? "최대플랜지폭위치" : "최소플랜지폭위치"); }

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSlabJewonDlg)
	afx_msg void OnLevelRoadcenter();
	afx_msg void OnButtonSlabhunch();
	afx_msg void OnButtonTendon();
	afx_msg void OnSelchangeComboPos();
	afx_msg void OnButtonSlabStructure();
	//}}AFX_MSG
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCursorChange(long nRow, long nCol);
	afx_msg void OnCellChangedDataGridT(NMHDR* nmgv, LRESULT*);
	afx_msg void STMakeCursor(NMHDR* nmgv, LRESULT*);
	afx_msg LRESULT OnReceive(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLABJEWONDLG_H__85B24936_C9EF_4E46_B04C_D96358C5F670__INCLUDED_)
