// ReDir.cpp: implementation of the CReDir class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultDlg.h"
#include "ReDir.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReDir::CReDir(CResultDlg *pDlg) : m_pDlg(pDlg)
{
	
}

CReDir::~CReDir()
{

}

void CReDir::OnChildStarted(LPCSTR lpszCmdLine)
{		
	CString strText(AfxGetApp()->m_pszAppName);
	strText += " - ";
	strText += lpszCmdLine;
	CopyData(m_pDlg->m_hWnd, SET_WINDOW_TEXT, NULL, "Start", strText.GetLength()+1);	
}

// Send stdout or stderr text to the display window.

void CReDir::OnChildWrite(UINT OutputID, LPCSTR lpszOutput)
{	
//	if (pFrame != NULL)
//	{
		/* Ansi Check ¸·À½
		if (theApp.m_bOemToAnsi)
		{
			int len = strlen(lpszOutput);
			char *lpszBuffer = new CHAR[len+1];
			strcpy(lpszBuffer, lpszOutput);
			::OemToChar(lpszBuffer, lpszBuffer);
			CopyData(pFrame->m_hWnd, OutputID, NULL, lpszBuffer, strlen(lpszBuffer)+1);
			delete [] lpszBuffer;
		}
		else
		*/
	CopyData(m_pDlg->m_hWnd, STDIO_WRITE_TEXT, NULL, lpszOutput, strlen(lpszOutput)+1);
//	}	
}

// Send stdout text to the display window.

void CReDir::OnChildStdOutWrite(LPCSTR lpszBuffer)
{
	OnChildWrite(STDIO_WRITE_TEXT, lpszBuffer);
}

// Send stderr text to the display window.

void CReDir::OnChildStdErrWrite(LPCSTR lpszBuffer)
{
	OnChildWrite(STDERR_WRITE_TEXT, lpszBuffer);
}

// Child process is terminated correctly.

void CReDir::OnChildTerminate()
{
	// Terminate the child process.
	// Send STDIO_CLOSE Command with empty string to call
	// TerminateChildProcess().	
	m_pDlg->AddCommand(STDIO_CLOSE, "");
	
	// Terminate the application or change the window text.
	//CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
	//if (pFrame != NULL)
	//{
	//if (m_pDlg->m_bExitClose)
	//	pFrame->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
	//else if(theApp.m_nStep < STEP3)
	//{
	//	++theApp.m_nStep;
	//	while(IsChildRunning());
	//	AfxGetMainWnd()->SendMessage(WM_NEXT_COMMAND, (WPARAM)theApp.m_nStep, (LPARAM)0);
	//}
	//else if(theApp.m_nStep == STEP6)
	//{
	//	++theApp.m_nStep;
	//	AfxGetMainWnd()->SendMessage(WM_NEXT_COMMAND, (WPARAM)theApp.m_nStep, (LPARAM)0);
	//}
	//else
	//	if (pFrame != NULL)
	CString sz = AfxGetApp()->m_pszAppName;
	CopyData(m_pDlg->m_hWnd, SET_WINDOW_TEXT, NULL, "End"/*AfxGetApp()->m_pszAppName*/, strlen(AfxGetApp()->m_pszAppName)+1);
	//}
}