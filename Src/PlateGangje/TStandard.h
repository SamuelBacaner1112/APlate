// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// TStandard.h: interface for the CTStandard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSTANDARD_H__F7C3A3A3_C00C_11D2_95C3_002074115574__INCLUDED_)
#define AFX_TSTANDARD_H__F7C3A3A3_C00C_11D2_95C3_002074115574__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CPlateBridgeApp;
class CEnvironment;
class CLineInfoApp;
class CBaseChief;
class CVAbortPgsDlg;
class CDimTable;
class CBMCreator;
class CPaintCreator;
class CCuttingStandard;
class CWeldStandard;
class CXTPStatusBar;

class AFX_EXT_CLASS CTStandard  : public CObject
{
// Construction
public:
	CTStandard();
	virtual ~CTStandard();

// Attributes
protected:
	HWND			m_hStatusWnd;

public:
	CXTPStatusBar*		m_pWndStatusBar;
	CDocument*		m_pDoc;
	CPlateBridgeApp*		m_pDB;
//	CDrawDBGen*		m_pDDBGen;
	CEnvironment*	m_pEnv;
	CLineInfoApp*	m_pLineApp;		
	CSplitterWnd*	m_pSplit;
	CBaseChief*		m_pBaseChief;
	CBMCreator*	m_pBMStandard;
	CPaintCreator*	m_pPaintCreator;
	CWeldStandard*  m_pWeldStandard;
	CCuttingStandard*	m_pCuttingStandard;

	CVAbortPgsDlg*	m_pAbortPgsDlg;

	// 생성시 초기값 없음
	CProgressCtrl	m_StatusProgress;
	long			m_ProcessPage;
	CWnd*			m_pWnd;

// Implementation
protected:
	CDPoint GetABSub(const CString& cs, long nG,double D) const;
public:
	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE);
	void SetStatusBar(CXTPStatusBar *pStatusBar);
	CString GetContinueType(CString csType,long nG=0) const;
	HWND GetSafePgs() const{return m_StatusProgress.GetSafeHwnd();}	
	void SetRange( int nLower, int nUpper );
 	void DestroyPgs();
	BOOL CreatePgs(int SPos);
	void SetPosPgs(int Pos);
///////////////////////////////////////////////////
// Thread
protected:
	BOOL m_bUseSimpleThread;
public:
	void GoProgress(long lw);
	void BeginSimpleThread();
	void EndSimpleThread();
	BOOL* GetAbort() const;
	BOOL Abort() const;
};

#endif // !defined(AFX_TSTANDARD_H__F7C3A3A3_C00C_11D2_95C3_002074115574__INCLUDED_)