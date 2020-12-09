// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_JACKUPSOLEPLATEDLG_H__CCD6E7AA_5733_4E35_BF07_3AE25EB3A9EF__INCLUDED_)
#define AFX_JACKUPSOLEPLATEDLG_H__CCD6E7AA_5733_4E35_BF07_3AE25EB3A9EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JackupSolePlateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJackupSolePlateDlg dialog

class CJackupSolePlateDlg : public CDialog
{
// Construction
public:
	CJackupSolePlateDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJackupSolePlateDlg)
	enum { IDD = IDD_DIALOG_JACKUPSOLEPLATE };
	CGridCtrl m_Grid;
	BOOL	m_bUseJackupSolePlate;
	//}}AFX_DATA
	CButton	m_btnDefault;
	CButton	m_btnApply;

  CGeneralBaseStd	*m_pStd;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJackupSolePlateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDataNotUse();

	double GetStationMatrix(long nRow, long nCol, long nG);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	// Grid ¼³Á¤
	void SetGridTitle();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJackupSolePlateDlg)
	afx_msg void OnCheckUseJackupSoleplate();
	afx_msg void OnButtonRecommend();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCellChangedData	(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDataChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JACKUPSOLEPLATEDLG_H__CCD6E7AA_5733_4E35_BF07_3AE25EB3A9EF__INCLUDED_)
