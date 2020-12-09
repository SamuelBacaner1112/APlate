// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETSLABANCHOR_H__BFADD0D8_9619_481C_A317_0B851D0E1E30__INCLUDED_)
#define AFX_DETSLABANCHOR_H__BFADD0D8_9619_481C_A317_0B851D0E1E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetSlabAnchor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetSlabAnchor dialog

class CDetSlabAnchor : public TStackedPage
{
// Construction
public:
	CDetSlabAnchor(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor
	CGeneralBaseStd *m_pStd;

// Dialog Data
	//{{AFX_DATA(CDetSlabAnchor)
	enum { IDD = IDD_DET_08_SLABANCHOR };
	CGridCtrl		m_Grid;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetSlabAnchor)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetSlabAnchor)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRow	(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETSLABANCHOR_H__BFADD0D8_9619_481C_A317_0B851D0E1E30__INCLUDED_)
