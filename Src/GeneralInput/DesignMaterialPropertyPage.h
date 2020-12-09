// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNMATERIALPROPERTYPAGE_H__9C84AD4A_0BE5_4958_8BAF_8E8ECFA46EB6__INCLUDED_)
#define AFX_DESIGNMATERIALPROPERTYPAGE_H__9C84AD4A_0BE5_4958_8BAF_8E8ECFA46EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignMaterialPropertyPage.h : header file
//

#include "StackDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDesignMaterialPropertyPage dialog
class CDataManage;
// 재료물리상수
class CDesignMaterialPropertyPage : public TStackedPage
{
// Construction
public:
	CDesignMaterialPropertyPage(CDataManage *pMng, CWnd* pParent = NULL);    // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignMaterialPropertyPage)
	enum { IDD = IDD_DESIGN_MATERIAL_SET };
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignMaterialPropertyPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	double GetConcreteEcByFck(double dFck);
	double GetConcreteFckByEc(double dEc);

	// Generated message map functions
	//{{AFX_MSG(CDesignMaterialPropertyPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void SetConcreteEcSigmaCKByFck(double dFck);
	void SetConcreteEcSigmaCKByFci(double dFci);
	void SetConcreteEcSigmaCKByFck2(double dFck);
	void OnPreInitDialog();
	void SetUnit();

protected:
	CDesignPageDlg *m_pDesignPageDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNMATERIALPROPERTYPAGE_H__9C84AD4A_0BE5_4958_8BAF_8E8ECFA46EB6__INCLUDED_)
