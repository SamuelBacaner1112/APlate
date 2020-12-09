// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_JACKUPPOSITIONDLG_H__AEFA3900_4496_4FCC_8E7B_2DF280FEDB4D__INCLUDED_)
#define AFX_JACKUPPOSITIONDLG_H__AEFA3900_4496_4FCC_8E7B_2DF280FEDB4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JackupPositionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJackupPositionDlg dialog

class CJackupPositionDlg : public CGeneralBaseDlg
{
// Construction
public:
	CJackupPositionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJackupPositionDlg)
	enum { IDD = IDD_GEN_08_JACKUPPOSITION };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CButton m_ctrlAbutSame;
	CButton m_ctrlPierSame;
	CButton m_btnPosition;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJackupPositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDataBx();
	void SetDataJewon();

	double GetStationMatrix(long nRow, long nCol, long nG);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	// Grid 설정
	void SetGridTitle();
	long GetTabCount();
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "평면도" : "상세도"; }	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJackupPositionDlg)
	afx_msg void OnAllabutSame();
	afx_msg void OnAllpierSame();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonJackupsoleplate();
	afx_msg void OnButtonRecommend();
	//}}AFX_MSG
	afx_msg void OnCellChanged(WPARAM nmgv, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JACKUPPOSITIONDLG_H__AEFA3900_4496_4FCC_8E7B_2DF280FEDB4D__INCLUDED_)
