// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DECKREBARINFODLG_H__EEAB0964_3565_4A55_8F29_6F1B35135A28__INCLUDED_)
#define AFX_DECKREBARINFODLG_H__EEAB0964_3565_4A55_8F29_6F1B35135A28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckRebarInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeckRebarInfoDlg dialog

class AFX_EXT_CLASS CDeckRebarInfoDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDeckRebarInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeckRebarInfoDlg();

// Dialog Data
	//{{AFX_DATA(CDeckRebarInfoDlg)
	enum { IDD = IDD_DECK_02_REBARINFO };
	CGridCtrlMng m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckRebarInfoDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CButton	m_btnSameAll;
	CButton	m_btnReport;


protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckRebarInfoDlg)	
	afx_msg void OnButtonApplySameLRC();
	afx_msg void OnButtonApplyAtCalcData();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);	
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

// Implementation
public:
	BOOL IsValid();
	void InitTabCtrl();
	
	void OnPreInitDialog();
	
	void SetGridData();

	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	void InsertDomyun(long nCRow, long nCCol);

	void ConvertFloorDataToRebarData(); 

	long GetRebarIdx(double dDia);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKREBARINFODLG_H__EEAB0964_3565_4A55_8F29_6F1B35135A28__INCLUDED_)
