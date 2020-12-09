// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNFRICTIONCOEFFPAGE_H__0D3AEBF2_989C_4680_A301_A1D6FF588CCD__INCLUDED_)
#define AFX_DESIGNFRICTIONCOEFFPAGE_H__0D3AEBF2_989C_4680_A301_A1D6FF588CCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignFrictionCoeffPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignFrictionCoeffPage dialog
class CDataManage;
#define COLUMN_COUNT	4
#define ROW_COUNT		8
class AFX_EXT_CLASS CDesignFrictionCoeffPage : public TStackedPage
{
// Construction
public:
	CDesignFrictionCoeffPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignFrictionCoeffPage)
	enum { IDD = IDD_DESIGN_FRICTION_COEFF };
		// NOTE: the ClassWizard will add data members here
	CGridCtrl m_Grid;
	//}}AFX_DATA
	CDataManage *m_pDataManage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignFrictionCoeffPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT];
	// Generated message map functions
	//{{AFX_MSG(CDesignFrictionCoeffPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

public:
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void SetDataDefault();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNFRICTIONCOEFFPAGE_H__0D3AEBF2_989C_4680_A301_A1D6FF588CCD__INCLUDED_)
