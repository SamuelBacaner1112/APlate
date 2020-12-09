// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNBASEDLG_H__E298DF86_88D4_48A7_8179_7A4AE7333D2D__INCLUDED_)
#define AFX_DESIGNBASEDLG_H__E298DF86_88D4_48A7_8179_7A4AE7333D2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignBaseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignBaseDlg dialog
// 설계선택사항/PS강재
class CDesignBaseDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDesignBaseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignBaseDlg)
	enum { IDD = IDD_CALC_DESIGNBASE };
	CShRichEdit	m_richeditAllowStress;
	CXTTabCtrl	m_ctlTab;
	//}}AFX_DATA
	CGridCtrl m_Grid;
	

private:	
	void SetShow();
	CString GetLineInfo();

	void SetDataDefaultBridgeJewon();
	void SetDataDefaultDesignCondition();
	void SetDataDefaultDesignRef();
	void SetDataDefaultDesignUnitWeight();

	void SetDataInitDesignBase();
	void SetDataInitDesignCondition();
	void SetDataInitDesignMaterial();
	void SetDataInitDesignUnitWeight();	
	void SetDataInitDesignRef();
	void SetDataInitPrestress();

	void SetGridTitleDesignBase();
	void SetGridTitleDesignCondition();
	void SetGridTitleDesignMaterial();
	void SetGridTitleDesignUnitWeight();
	void SetGridTitleDesignRef();
	void SetGridTitleDesignPrestress();

	void SetDataInit();
	void SetDataDefault();	
	void SetDataSave();
	void SetGridTitle();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);

	void OnPreInitDialog();
	CString GetStrDuctType(long nType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignBaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignBaseDlg)	
	afx_msg void OnSelchangeTabDesign(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNBASEDLG_H__E298DF86_88D4_48A7_8179_7A4AE7333D2D__INCLUDED_)
