// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Redirect.h : header file
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRedirect class
#define BUFFER_SIZE 1024

extern LRESULT CopyData(HWND hWnd, DWORD dwData, HWND hRxWnd, LPCVOID lpData, DWORD cbData);
extern void GetDirectoryName(CString &strDirName, const CString& strPathName);
extern void GetFileName(CString &strFileName, const CString& strPathName);
extern void GetCommandLine(HINSTANCE hInstance, CString &strCmdLine, LPCSTR lpszCmdLine);
extern void SeperateStr(CString &str, CStringArray &rtStr);

class CRedirect
{
// Construction
public:
	CRedirect();
	~CRedirect();

	BOOL StartChildProcess(LPCSTR lpszCmdLine, BOOL bShowChildWindow = FALSE);
	BOOL IsChildRunning() const;
	void TerminateChildProcess();
	void WriteChildStdIn(LPCSTR lpszInput);	

protected:
	HANDLE m_hExitEvent;

	// Child input(stdin) & output(stdout, stderr) pipes
	HANDLE m_hStdIn, m_hStdOut, m_hStdErr;
	// Parent output(stdin) & input(stdout) pipe
	HANDLE m_hStdInWrite, m_hStdOutRead, m_hStdErrRead;
	// stdout, stderr write threads
	HANDLE m_hStdOutThread, m_hStdErrThread;
	// Monitoring thread
	HANDLE m_hProcessThread;
	// Child process handle
	HANDLE m_hChildProcess;

	BOOL m_bWorking;
//	CResultDlg m_result;

	HANDLE PrepAndLaunchRedirectedChild(LPCSTR lpszCmdLine,
		HANDLE hStdOut, HANDLE hStdIn, HANDLE hStdErr,
		BOOL bShowChildWindow);

	static BOOL m_bRunThread;
	static int staticStdOutThread(CRedirect *pRedirect)
		{ return pRedirect->StdOutThread(pRedirect->m_hStdOutRead); }
	static int staticStdErrThread(CRedirect *pRedirect)
		{ return pRedirect->StdErrThread(pRedirect->m_hStdErrRead); }
	static int staticProcessThread(CRedirect *pRedirect)
		{ return pRedirect->ProcessThread(); }
	int StdOutThread(HANDLE hStdOutRead);
	int StdErrThread(HANDLE hStdErrRead);
	int ProcessThread();

public:
	virtual void OnChildStarted(LPCSTR lpszCmdLine) {};
	virtual void OnChildStdOutWrite(LPCSTR lpszOutput) {};
	virtual void OnChildStdErrWrite(LPCSTR lpszOutput) {};
	virtual void OnChildTerminate() {};
};
