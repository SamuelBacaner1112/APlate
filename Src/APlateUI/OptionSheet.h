// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONSHEET_H__51738A75_F2B3_4399_BF79_6DB10D0F8E80__INCLUDED_)
#define AFX_OPTIONSHEET_H__51738A75_F2B3_4399_BF79_6DB10D0F8E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionSheet
#include "OptionGen.h"
#include "OptionGeneral.h"
#include "OptionGeneralDim.h"
#include "OptionDetail.h"
#include "OptionDetailDim.h"
#include "OptionSplice.h"
#include "OptionCrossBeam.h"
#include "OptionHBracing.h"
#include "OptionVBracing.h"
#include "OptionBMPage.h"
#include "OptionShoe.h"
#include "OptionCamber.h"

#include "OptionEtcDetail.h"
#include "OptionWeldMap.h"
#include "OptionPlanGen.h"
#include "OptionLineInfo.h"
#include "OptionPaint.h"
#include "OptionSlabGen.h"
#include "OptionTendon.h"
#include "OptionDeckRebar.h"
#include "OptionSummary.h"
#include "OptionTUGirder.h"

class COptionStd;
class CAPlateDrawStd;
class AFX_EXT_CLASS COptionSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionSheet)

// Construction
public:
	COptionSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
private:
	CButton m_buttonLoad;
	CButton m_buttonSave;
	CButton m_buttonDefault;
	BOOL	m_bSetOk;

// Attributes
public:	
	//CDocument *m_pDoc;
	CAPlateDrawStd		*m_pPlateDrawStd;

	COptionGen			m_PageGen;
	COptionGeneral		m_PageGeneral;
	COptionGeneralDim	m_PageGeneralDim;
	COptionDetail		m_PageDetail;
	COptionDetailDim	m_PageDetailDim;
	COptionSplice		m_PageSplice;
	COptionCrossBeam    m_PageCrossBeam;
	COptionHBracing		m_PageHVracing;
	COptionVBracing     m_PageVBracing;	
	COptionBMPage		m_OptionBMPage;
	COptionShoe			m_OptionShoe;
	COptionCamber		m_OptionCamber;
	COptionEtcDetail	m_OptionEtcDetail;
	COptionWeldMap		m_OptionWeldMap;
	COptionPlanGen		m_OptionPlanGen;
	COptionLineInfo		m_OptionLineInfo;
	COptionPaint        m_OptionPaint;
	COptionSlabGen		m_OptionSlabGen;
	COptionTendon		m_OptionTendon;
	COptionDeckRebar	m_OptionDeckRebar;
	COptionSummary		m_OptionSummary;
	COptionTUGirder		m_OptionTUGirder;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionSheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();	
	afx_msg void OnLoad();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSHEET_H__51738A75_F2B3_4399_BF79_6DB10D0F8E80__INCLUDED_)
