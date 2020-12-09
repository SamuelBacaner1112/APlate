// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNSPECIALMOVINGLOAD_H__A6A65F43_21EB_40DE_BCC3_85BA82947B9D__INCLUDED_)
#define AFX_DESIGNSPECIALMOVINGLOAD_H__A6A65F43_21EB_40DE_BCC3_85BA82947B9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignSpecialMovingLoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDesignSpecialMovingLoad dialog
class CDataManage;
class CDesignSpecialMovingLoad : public TStackedPage
{
	//DECLARE_DYNCREATE(CDesignSpecialMovingLoad)

// Construction
public:
	CDesignSpecialMovingLoad(CDataManage *pMng, CWnd* pParent = NULL);
	~CDesignSpecialMovingLoad();

// Dialog Data
	//{{AFX_DATA(CDesignSpecialMovingLoad)
	enum { IDD = IDD_DESIGN_SPECIAL_LOAD };
	//}}AFX_DATA
	CDataManage *m_pDataManage;
	CGridCtrl	m_Grid;
	CBitmap		m_BitTank;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDesignSpecialMovingLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDesignSpecialMovingLoad)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void SetGridTitle();
	void OnPreInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNSPECIALMOVINGLOAD_H__A6A65F43_21EB_40DE_BCC3_85BA82947B9D__INCLUDED_)
