// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_AFX_DESIGNSCALLOPPAGE_H__64358203_6257_4CBE_87C9_F294A96F1265__INCLUDED_)
#define AFX_AFX_DESIGNSCALLOPPAGE_H__64358203_6257_4CBE_87C9_F294A96F1265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignScallopPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignScallopPage dialog
#define COLUMN_COUNT_SCALLOP 7

class CDataManage;
class CDesignScallopPage : public TStackedPage
{
// Construction
public:
	CDesignScallopPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignScallopPage)
	enum { IDD = IDD_DESIGN_SCALLUP_SET };
	CGridCtrl m_Grid;		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignScallopPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT_SCALLOP];	
	CStringArray	m_CutType;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignScallopPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
public:
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNSCALLOPPAGE_H__64358203_6257_4CBE_87C9_F294A96F1265__INCLUDED_)
