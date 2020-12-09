// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DANMYUNTYPEDLG_H__BA448DD6_5CB4_4E25_B6D9_376BEEB75813__INCLUDED_)
#define AFX_DANMYUNTYPEDLG_H__BA448DD6_5CB4_4E25_B6D9_376BEEB75813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DanmyunTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDanmyunTypeDlg dialog

class CDanmyunTypeDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDanmyunTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDanmyunTypeDlg)
	enum { IDD = IDD_GEN_17_DANMYUNTYPE };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CButton m_btnSameAll;
	CButton m_btnCtype;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDanmyunTypeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void SetDanmyunTypeByJijum(CPlateBasicIndex* pBxPrevJijum, CPlateBasicIndex* pBxJijum);
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void OnPreInitDialog();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	long GetTabCount() { return 2; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "평면도" : "종단면도"; }	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDanmyunTypeDlg)
	afx_msg void OnDanmyuntypeAllgirgerSame();
	afx_msg void OnDanmyuntypeAllgirgerCtype();
	//}}AFX_MSG
	afx_msg void OnCellChanged(NMHDR*nmgv,LRESULT*);
	afx_msg void OnCellChangedData(NMHDR*nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DANMYUNTYPEDLG_H__BA448DD6_5CB4_4E25_B6D9_376BEEB75813__INCLUDED_)
