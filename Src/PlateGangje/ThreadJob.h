// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ThreadJob.h: interface for the CThreadJob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADJOB_H__2C1B9F81_D0A7_11D3_AFE7_00105A75D74A__INCLUDED_)
#define AFX_THREADJOB_H__2C1B9F81_D0A7_11D3_AFE7_00105A75D74A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CThreadJob  : public CDialog
{
public:
//	CThreadJob();
	CThreadJob(long nIDD,CWnd*pParent)
		: CDialog(nIDD, pParent) { m_pThread = NULL; m_uErr = 0;m_bAbort=0;}
	virtual ~CThreadJob()
	{
		if(m_pThread)
		{
			TRACE("CThreadJob: *** Warning:deleteing active thread! ***\n");
			delete m_pThread;
		}
	}



// Attributes
private:
	CWinThread* m_pThread;	// 실행 쓰레드
protected:
	HWND m_hWndOwner;	// 소요 윈도우의 CWnd가 아닌 HWND
	UINT m_ucMsg;		// Onprogress를 위한 되부름 메세지
	UINT m_uErr;		// 쓰레드 오류코드
public:
	UINT m_bAbort;		// 중지할 것인지 결정하는 프래그:DoWork에서 검사해야함


// Implememtation
private:
	static UINT ThreadProc(LPVOID pObj);
protected:
	void OnProgress(WPARAM wp = 0, LPARAM lp = 0);
	virtual UINT DoWork() = 0;
public:
	virtual BOOL Begin(CWnd* pWndOwner=NULL,UINT ucMsg=0);
	virtual void Abort(BOOL bAbort=TRUE);

//	DECLARE_DYNAMIC(CThreadJob)
};



inline UINT CThreadJob::ThreadProc(LPVOID pObj)
{
	CThreadJob* pJob = (CThreadJob*)pObj;
//	ASSERT_KINDOF(CThreadJob,pJob);
	pJob->m_uErr = pJob->DoWork();
	pJob->m_pThread = NULL;
	return pJob->m_uErr;
}
 
inline BOOL CThreadJob::Begin(CWnd* pWndOwner,UINT ucMsg)
{
	m_hWndOwner = pWndOwner->GetSafeHwnd();
	m_ucMsg = ucMsg;
	m_bAbort = FALSE;
	m_uErr = 0;
	m_pThread = AfxBeginThread(ThreadProc,this);
	return m_pThread != NULL;
}

inline void CThreadJob::Abort(BOOL bAbort)
{
	m_bAbort = bAbort;
}

inline void CThreadJob::OnProgress(WPARAM wp, LPARAM lp)
{
	if(m_hWndOwner && m_ucMsg)
		::PostMessage(m_hWndOwner,m_ucMsg,wp,lp);
}



#endif // !defined(AFX_THREADJOB_H__2C1B9F81_D0A7_11D3_AFE7_00105A75D74A__INCLUDED_)
