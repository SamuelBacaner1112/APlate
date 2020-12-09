// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETBRACKETDLG_H__B88E3853_86D1_4D6D_848E_0B31B6F41FE5__INCLUDED_)
#define AFX_DETBRACKETDLG_H__B88E3853_86D1_4D6D_848E_0B31B6F41FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetBracketDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetBracketDlg dialog

class CDetBracketDlg : public TStackedPage
{
// Construction
public:
	CDetBracketDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor
	CGeneralBaseStd *m_pStd;
// Dialog Data
	//{{AFX_DATA(CDetBracketDlg)
		enum { IDD = IDD_DET_09_BRACKET };
		CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetBracketDlg)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetGridTitle();
	void OnPreInitDialog();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetBracketDlg)
		afx_msg void OnButtonRecommend();
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);

protected:
	long	m_nCurIndex	;
	// Round
	double	m_dR_CRBeamUpper;
	double	m_dR_CRBeamLower;
	double	m_dR_VBeamUpper	;
	double	m_dR_VBeamLower	;
	double	m_dR_WBeamUpper	;
	double	m_dR_WBeamLower	;
	// Chip
	double	m_dC_CRBeamUpper;
	double	m_dC_CRBeamLower;
	double	m_dC_VBeamUpper	;
	double	m_dC_VBeamLower	;
	double	m_dC_WBeamUpper	;
	double	m_dC_WBeamLower	;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETBRACKETDLG_H__B88E3853_86D1_4D6D_848E_0B31B6F41FE5__INCLUDED_)
