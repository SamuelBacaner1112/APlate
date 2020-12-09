// HisEdit.cpp : implementation file
//

#include "stdafx.h"
#include "APlate.h"
#include "HisEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHisEdit

CHisEdit::CHisEdit()
{
}

CHisEdit::~CHisEdit()
{
}


BEGIN_MESSAGE_MAP(CHisEdit, CEdit)
	//{{AFX_MSG_MAP(CHisEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHisEdit message handlers

void CHisEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	LPARAM lp = (LPARAM)nFlags << 16 | nRepCnt;
	::SetFocus(m_hCmdWnd);
	::SendMessage(m_hCmdWnd,WM_CHAR,(TCHAR)nChar,lp);		
	//CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CHisEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 27)
	{
		return TRUE;
	}
	return CEdit::PreTranslateMessage(pMsg);
}

void CHisEdit::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here	
}

void CHisEdit::ScrollAtEnd()
{
	LineScroll(GetLineCount());
}
