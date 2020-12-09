// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ReDir.h: interface for the CReDir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REDIR_H__4D35950B_D0C4_43B2_B627_2BC9A8306D24__INCLUDED_)
#define AFX_REDIR_H__4D35950B_D0C4_43B2_B627_2BC9A8306D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Redirect.h"

#define SET_WINDOW_TEXT   1
#define STDIO_WRITE_TEXT  2
#define STDERR_WRITE_TEXT 3

class CResultDlg;
class AFX_EXT_CLASS CReDir : public CRedirect  
{
public:
	CReDir(CResultDlg *pDlg);
	virtual ~CReDir();
public:
	CResultDlg *m_pDlg;
protected:
	void OnChildWrite(UINT OutputID, LPCSTR lpszOutput);
protected:
	virtual void OnChildStarted(LPCSTR lpszCmdLine);
	virtual void OnChildStdOutWrite(LPCSTR lpszBuffer);
	virtual void OnChildStdErrWrite(LPCSTR lpszBuffer);
	virtual void OnChildTerminate();
};

#endif // !defined(AFX_REDIR_H__4D35950B_D0C4_43B2_B627_2BC9A8306D24__INCLUDED_)
