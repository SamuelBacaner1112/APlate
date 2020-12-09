// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNSTEELMATERIALPAGE_H__6057DCAF_2738_4981_966D_4A567604A405__INCLUDED_)
#define AFX_DESIGNSTEELMATERIALPAGE_H__6057DCAF_2738_4981_966D_4A567604A405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignSteelMaterialPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignSteelMaterialPage dialog
#define COLUMN_COUNT_SECTION 6

class CDataManage;
class CDBAllowStressDataLoad;

//설계조건
//부재설정
class CDesignSteelMaterialPage : public TStackedPage
{
// Construction
public:
	CDesignSteelMaterialPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignSteelMaterialPage)
	enum { IDD = IDD_DESIGN_MEMEBER_SET };
	BOOL	m_bSameApply;
	//}}AFX_DATA

	CDataManage *m_pDataManage;
	CGridCtrl m_Grid;
	BOOL	  m_bChange;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDesignSteelMaterialPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignSteelMaterialPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSameapply();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()

// Implementation
private:
	static SHGRIDTITLE m_gt[COLUMN_COUNT_SECTION];	
	CStringArray m_strComboBuje;
	CStringArray m_strComboMat;

public:
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetFactDataInit();//공장이음이 생성되어 있으면 다시 초기화...
	void SetDataSave();
	void OnButtonF10T();
	void OnButtonF13T();
	BOOL InsertSymbol(CStringArray* pStrArr, CString pKey);
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNSTEELMATERIALPAGE_H__6057DCAF_2738_4981_966D_4A567604A405__INCLUDED_)
