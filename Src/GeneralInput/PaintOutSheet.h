// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PaintOutSheet.h: interface for the CPaintOutSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAINTOUTSHEET_H__5FD31207_A98A_4BA1_B040_647282B3403C__INCLUDED_)
#define AFX_PAINTOUTSHEET_H__5FD31207_A98A_4BA1_B040_647282B3403C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPaintOutAssembly;
class CXLCell;

class AFX_EXT_CLASS CPaintOutSheet : public CPropertySheet  
{
	DECLARE_DYNAMIC(CPaintOutSheet)
 
// Construction
public:
	CPaintOutSheet();

// Attributes
public:
	CDataManage    *m_pDataManage;
	CFont			m_SheetFont;
	CXTPButton		m_MakeButton;
	CXTPButton		m_SaveButton;
	CXTPButton		m_PrintButton;
	CMenu			m_MenuMakeButton;
	CButton			m_CloseButton;

	CProgressCtrl	m_Progress;
	CStatic			m_ProgressText;
	CStatic			m_CountText;

	CPaintOutAssembly *m_pPaintSang;
	CPaintOutAssembly *m_pPaintBujeSang;
//	CPaintOutAssembly *m_pPaintBujeTotal;
//	CPaintOutAssembly *m_pPaintMatAndThick;
//	CPaintOutAssembly *m_pPaintBoltSang;
	CPaintOutAssembly *m_pPaintBoltTotal;
	CPaintOutAssembly *m_pPaintTotalPage;

	//
	BOOL	m_bNoSize;
	CString	m_sXLFileName;
	long	m_nMakePaintFlag;
	long    m_nARoadFlag;
	DIMTYPE m_nDimType;

// Operations
public:
	BOOL Create(CWnd* pParentWnd, long nMakeFlag, long nARoadFlag, DIMTYPE nDimType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaintOutSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaintOutSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaintOutSheet)
	afx_msg void OnButtonMakeCreatorDropDown();
	afx_msg void OnButtonSaveDropDown();
	afx_msg void OnButtonPrintDropDown();
	afx_msg void OnButtonMakeCreator();
	afx_msg void OnButtonMakeFileOpenXL();
	afx_msg void OnButtonFileSaveXL();
	afx_msg void OnButtonFileSaveAllXL();
	afx_msg void OnButtonFileOutFileOpenXL();
	afx_msg void OnButtonPrintPreview();
	afx_msg void OnButtonPrintThis();
	afx_msg void OnButtonClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnProgress(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
	//
	void Start(long nMakeType);
	void Finish();
};


#endif // !defined(AFX_PAINTOUTSHEET_H__5FD31207_A98A_4BA1_B040_647282B3403C__INCLUDED_)
