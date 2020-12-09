// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETPLATEWSHIFTDLG_H__F63F6081_77C1_4EA5_9EC4_42755995F170__INCLUDED_)
#define AFX_DETPLATEWSHIFTDLG_H__F63F6081_77C1_4EA5_9EC4_42755995F170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetPlateWShiftDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetPlateWShiftDlg dialog

class CDetPlateWShiftDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetPlateWShiftDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetPlateWShiftDlg)
		enum { IDD = IDD_DET_06_PLATEWIDTHSHIFT };
		CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetPlateWShiftDlg)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void OnPreInitDialog();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);
	void SetGridTitle();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetPlateWShiftDlg)
		afx_msg void OnButtonRecommend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);

protected:
	void SetDataArrIndex();
	void SetDataCurIndex();

	void GetDataArrIndex();
	void GetDataCurIndex();

protected:
	long	m_nCurIndex	;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETPLATEWSHIFTDLG_H__F63F6081_77C1_4EA5_9EC4_42755995F170__INCLUDED_)
