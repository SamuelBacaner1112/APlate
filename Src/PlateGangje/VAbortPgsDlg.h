// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// VAbortPgsDlg.h: interface for the CVAbortPgsDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VABORTPGSDLG_H__35D3C0C5_9B82_11D3_AFE7_00105A75D74A__INCLUDED_)
#define AFX_VABORTPGSDLG_H__35D3C0C5_9B82_11D3_AFE7_00105A75D74A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WM_USER_DXFSAVE			(WM_APP + 1000)
#define WM_USER_CURDOMYUN		(WM_APP + 1001)

#define WM_USER_MAKEGANGJE		(WM_APP + 1003)
//#define WM_USER_MAKEGANGJEPGS	(WM_APP + 1004) 이 message대신에 GoProgress함수로 대치
#define WM_USER_MAKEGANGJEGRID	(WM_APP + 1005)
#define WM_USER_MAKEGANGJECOMMA	(WM_APP + 1006)
#define WM_USER_MAKEANALYSISCAL	(WM_APP + 1007)	// 계산서 richtxt
#define WM_USER_MAKEANALYSIS_XL	(WM_APP + 1008)	// 계산서 xls
#define WM_USER_MULTI			(WM_APP + 1009)	// 

#define WM_USER_RUNHLOAD		(WM_APP + 2001) // 구조계산 횡분배 실행
#define WM_USER_RUNAUTOCALC		(WM_APP + 2002) // 두께 자동 계산(공장이음)

#include "ThreadJob.h"


class AFX_EXT_CLASS CVAbortPgsDlg  : public CThreadJob
{
// Constructor
protected:
	CVAbortPgsDlg(long nIDD,CWnd*pParent)
		: CThreadJob(nIDD, pParent) {}

	virtual ~CVAbortPgsDlg() {}

// Attributes
public:
	CWnd*	m_pTextWnd;
	CWnd*	m_pTextAllWnd;

// Implementation
public:
	virtual void OnOK() {}
	virtual void SetOneProgress() {}
	virtual CProgressCtrl* GetPgsAll()	const { return NULL;}
	virtual CProgressCtrl* GetPgsCur()	const { return NULL;}
	virtual void SetCurText(LPCTSTR lpszString) {}
	virtual void InitData(CString const & sTitle,CString const & sTitlePgs,
		BOOL* bAbort,CWnd* pMsgWnd,long WParam,long LParam,BOOL bSamePgsCurAll) {}
	virtual void SetPosAll(int nPos,BOOL bAbsPos) {}
	virtual void SetPosCur(int nPos) {}
	virtual void SetRangeAll(int nStt,int nEnd) {}
	virtual void SetPaneText(LPCTSTR lpstr) {}
};

#endif // !defined(AFX_VABORTPGSDLG_H__35D3C0C5_9B82_11D3_AFE7_00105A75D74A__INCLUDED_)
