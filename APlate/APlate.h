// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlate.h : main header file for the APLATE application
//

#if !defined(AFX_APLATE_H__E42BB9D9_9395_4BAF_8E15_F2737A8090F8__INCLUDED_)
#define AFX_APLATE_H__E42BB9D9_9395_4BAF_8E15_F2737A8090F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAPlateApp:
// See APlate.cpp for the implementation of this class
//

// VERSION, BUILDNUMBER 정보는 include폴더에 versionInfo.h에 들어있다. 
// StdAfx.h 에서 include하고 있다. 
#ifndef VERSION
#define VERSION _T("1.0.1")
#endif

#ifndef BUILDNUMBER
#define BUILDNUMBER _T("100")
#endif

#define BUILDCOUNT		1
#define REVISIONCOUNT	0

#define MENUVERSION		19
#define OVER_VERSION	"이 파일은 상위버전에서 작성되었습니다. \n업그레이드를 하신 후 다시 읽으시기 바랍니다."

static const double BRPFILEVER	= 2006.081006;			// 버전.년월일 형식으로
static const long	BRPBUILDVER	= 1;
static const long	BRPHEADERBUFSIZE = 512;

extern const TCHAR szAPlateClass[];
extern const CString szAPlateName;

class CAPlateApp : public CMSDIWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CAPlateApp();
	virtual ~CAPlateApp();
	
	BOOL			m_bFirFileOpen;	
	
protected :	
	CString		m_strFixedDir;	
	CMultiDocTemplate* m_pDocTemplate;	

// Operations
public:	
	void	SetRegFileExt(CString strValue[4], long nIcon=0, BOOL bOpen=TRUE);
	BOOL	IsRegFileExt() const;
	void	CutLastBackSlash(CString& cs);	//
	void	SetWorkDir(CString strDir);
	void	SetFixDirectory();
	CString GetWorkDir();
	CString GetFixedDir();
	CString GetMRUName(UINT nIndex, BOOL bFullPath);	
	UINT	GetMRUListSize();	
	void DeleteWmfFile();
	CString GetStringVersion();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPlateApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	

// Implementation
	//{{AFX_MSG(CAPlateApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreLoadState();

};

inline CString CAPlateApp::GetFixedDir()
{
	CString strSection = _T("Afx Directory");
	CString strFixedItem = _T("Fixed Directory");
	
	return GetProfileString(strSection, strFixedItem);
}

inline CString CAPlateApp::GetWorkDir()
{
	CString strSection = _T("Afx Directory");
	CString strWorkItem	= _T("Work Directory");
	
	return GetProfileString(strSection, strWorkItem);
}

inline void CAPlateApp::SetWorkDir(CString strDir)
{
	CWinApp* pApp = AfxGetApp();
	CString strSection = _T("Afx Directory");
	CString strWorkItem = _T("Work Directory");
	
	pApp->WriteProfileString(strSection, strWorkItem, strDir);	
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLATE_H__E42BB9D9_9395_4BAF_8E15_F2737A8090F8__INCLUDED_)
