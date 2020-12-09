// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WeldOutSheet.h: interface for the CWeldOutSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WELDOUTSHEET_H__532CF8AB_1282_4250_BABA_65C8731BFBF1__INCLUDED_)
#define AFX_WELDOUTSHEET_H__532CF8AB_1282_4250_BABA_65C8731BFBF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CWeldOutAssembly;
class CXLCell;

class AFX_EXT_CLASS CWeldOutSheet : public CPropertySheet

{
	DECLARE_DYNAMIC(CWeldOutSheet)

// Construction
public:
	CWeldOutSheet();

// Attributes
public:
	CDataManage		*m_pDataManage;
	CFont			m_SheetFont;
	CXTPButton		m_MakeButton;
	CXTPButton		m_SaveButton;
	CXTPButton		m_PrintButton;
	CMenu			m_MenuMakeButton;
	CButton			m_CloseButton;

	CProgressCtrl	m_Progress;
	CStatic			m_ProgressText;
	CStatic			m_CountText;

	CWeldOutAssembly *m_pSangPage;
	CWeldOutAssembly *m_pAssemblySangPage;
	CWeldOutAssembly *m_pAssemblyPage;
	CWeldOutAssembly *m_pAssemblyTotalPage;
	CWeldOutAssembly *m_pTypePage;
	CWeldOutAssembly *m_pThickPage;
	//
	BOOL	m_bNoSize;
	CString	m_sXLFileName;

	long	m_nMakeWeldFlag;
	long	m_nARoadFlag;	// 0-AROAD,1-ASHOP,2-AMODEL,3-APIECE
	DIMTYPE m_nDimType;
// Operations
public:
	BOOL Create(CWnd* pParentWnd, long nMakeFlag, DIMTYPE nDimType, long nARoadFlag=1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeldOutSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWeldOutSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWeldOutSheet)
	afx_msg void OnButtonMakeCreatorDropDown();
	afx_msg void OnButtonSaveDropDown();
	afx_msg void OnButtonPrintDropDown();
	afx_msg void OnButtonMakeCreator();
	afx_msg void OnButtonMakeFileOpenXL();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonFileSaveXL();
	afx_msg void OnButtonFileSaveAllXL();
	afx_msg void OnButtonFileOutFileOpenXL();
	afx_msg void OnButtonPrintPreview();
	afx_msg void OnButtonPrintThis();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnProgress(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	void Start(long nMakeType);
	void Finish();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELDOUTSHEET_H__532CF8AB_1282_4250_BABA_65C8731BFBF1__INCLUDED_)
