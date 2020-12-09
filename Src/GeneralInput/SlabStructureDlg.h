// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SLABSTRUCTUREDLG_H__9EB9D333_7561_4E7D_8639_8A501BC69DE1__INCLUDED_)
#define AFX_SLABSTRUCTUREDLG_H__9EB9D333_7561_4E7D_8639_8A501BC69DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlabStructureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSlabStructureDlg dialog

class CSlabStructureDlg : public CDialog
{
// Construction
public:
	CSlabStructureDlg(CWnd* pParent = NULL);   // standard constructor

	CDomyunView	m_DomView;
	CGridCtrl	m_Grid;
	CPlateBridgeApp	*m_pDB;
	CDataManage		*m_pDataManage;

	BOOL IsFocusItem(CString &szTitle);
	void SetFocusItem(CString szTitle);
	void SetDataDefault();
	void SetDataDefault1(long nGuard);
	void SetDataDefault2(long nGuard);
	void SetDataDefault3(long nGuard);
	void SetDataDefault4(long nGuard);
	void SetDataDefault5(long nGuard);
	void SetDataDefault6(long nGuard);
	void SetDataSave();
	void SetGridTitle();
	void SetDataInit();
	void DrawInputDomyunView(BOOL bZoomAll=FALSE);

// Dialog Data
	//{{AFX_DATA(CSlabStructureDlg)
	enum { IDD = IDD_DIALOG_SLAB_STRUCTURE };
	//}}AFX_DATA
	CButton	m_btnBaseType;
	CButton	m_btnRectType;
	CButton	m_btnCenterType;
	CButton	m_btnSideType;
	CButton	m_btnSymmetriType;
	CButton	m_btnCenterHalfType;
	CButton	m_btnOK;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlabStructureDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSlabStructureDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnStructureDefault();
	afx_msg void OnGuardwallbangRecommend();
	afx_msg void OnButtonRecommendMedibarrHalf();
	afx_msg void OnButtonRecommendMedibarr();
	afx_msg void OnButtonNangan();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonSymmetry();
	afx_msg void OnButtonGuardwall();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLABSTRUCTUREDLG_H__9EB9D333_7561_4E7D_8639_8A501BC69DE1__INCLUDED_)
