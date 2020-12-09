// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNUWEIGHTPAGE_H__931F609E_D98E_4ABF_990B_2AD563EAA9D3__INCLUDED_)
#define AFX_DESIGNUWEIGHTPAGE_H__931F609E_D98E_4ABF_990B_2AD563EAA9D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignUWeightPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignUWeightPage dialog
#define ROW_COUNT_UWEIGHT 10

class CDataManage;
class CDesignUWeightPage : public TStackedPage
{
// Construction
public:
	CDesignUWeightPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignUWeightPage)
	enum { IDD = IDD_DESIGN_UNIT_WEIGHT };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignUWeightPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

private:
	static SHGRIDTITLE m_gt[ROW_COUNT_UWEIGHT];	
//	double& GetData(long row) const;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignUWeightPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	void SetDataSave();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNUWEIGHTPAGE_H__931F609E_D98E_4ABF_990B_2AD563EAA9D3__INCLUDED_)
