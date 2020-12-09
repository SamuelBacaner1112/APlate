// CmdEdit.cpp : implementation file
//

#include "stdafx.h"
#include "APlate.h"
#include "CmdEdit.h"

#include "HisEdit.h"

#include "APlateDoc.h"
#include "APlateView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdEdit

CCmdEdit::CCmdEdit()
{	
	m_nCmdTree = 0;		
	m_sType = TYPE_NORMAL;
	m_CompileStr = _T("");
	m_LButtonCmd = 0;
}

CCmdEdit::~CCmdEdit()
{
}


BEGIN_MESSAGE_MAP(CCmdEdit, CEdit)
	//{{AFX_MSG_MAP(CCmdEdit)
	ON_WM_LBUTTONDOWN()
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)	
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdEdit message handlers

BOOL CCmdEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) 
	{
		AddHistory("* 취소 *",FALSE);
		m_CmdName = "";
		m_nCmdTree = 0;	
		m_LButtonCmd = 0;		
		InitMembers();
		return TRUE;	
	}
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == 8 || pMsg->wParam == 36 || pMsg->wParam == 37 || pMsg->wParam == 38))
	{		
		CString strCmd;
		GetWindowText(strCmd);
		if(m_CmdStr.GetLength() >= strCmd.GetLength()) return TRUE;
		CDC *pDC = GetDC();				
		CSize size;
		GetTextExtentPoint32(pDC->m_hDC,(LPCTSTR)m_CmdStr,m_CmdStr.GetLength(),&size);
		ReleaseDC(pDC);
		CPoint CurPos = GetCaretPos();
		if(CurPos.x<size.cx - m_CmdStr.GetLength() || pMsg->wParam == 36) 
		{			
			SetCaretPos( CPoint(size.cx - m_CmdStr.GetLength() - 1,0) );
			return TRUE;
		}
	}
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)	   
	{
		CPoint CurPos;
		GetCursorPos(&CurPos);			
		ScreenToClient(&CurPos);
			
		CDC *pDC = GetDC();				
		CSize size;
		GetTextExtentPoint32(pDC->m_hDC,(LPCTSTR)m_CmdStr,m_CmdStr.GetLength(),&size);
		ReleaseDC(pDC);		
		if(CurPos.x<size.cx - m_CmdStr.GetLength()) {
			SetCaretPos( CPoint(size.cx - m_CmdStr.GetLength() - 1,0) );
			return TRUE;
		}
	}	
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
	{
		SetChar(pMsg->wParam);				
	}
		
	return CEdit::PreTranslateMessage(pMsg);
}


/////////////////////////////////////////////////////////////////////////////
// CCmdEdit Command

void CCmdEdit::InitMembers(CString str, short sType)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();			
	if(str=="")
	{
		m_CmdStr = _T("명령줄 : ");
		m_bParse = FALSE;
	}
	else
		m_CmdStr = str;
	m_sType = sType;
	m_nHeadNum = m_CmdStr.GetLength();
	SetWindowText(m_CmdStr);
	SendMessage(WM_KEYDOWN,35);
		
	if(pMain != NULL) 
	{
		CMDIChildWnd* pChild=pMain->MDIGetActive();			
		CView* pView = pChild->GetActiveView();
		//pView->SetFocus();
	}
	m_CompileStr = _T("");	
}

BOOL CCmdEdit::CmdParse()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CMDIChildWnd* pChild=pMain->MDIGetActive();	
	CAPlateDoc* pDoc = (CAPlateDoc*)pChild->GetActiveDocument();
	CString strCmd;

	CString Command[] = { "ZOOM", "PAN", "REDRAW", "GO", "OPEN", "SAVE", "QSAVE", "CLOSE", "HELP", "DIST",
						  "NEAR", "END", "PERPENDICULAR", "PROPERTY", "UCS", "VIEWRES", "CEN", "DDEDIT",
						  "MOVE", "PROGRAM", "MID", "NODE", "MOVEDATACHECK", "SAVEMARK", "LOADMARK",
						  "WRITECHECKMARK",
						};
	WPARAM CmdId[] = {0, ID_ZOOM_MOVE, ID_PAGE_REDRAW, 0, ID_FILE_OPEN, ID_FILE_SAVE_AS, 
					  ID_FILE_SAVE,ID_FILE_CLOSE, 0, ID_CALC_DIST, ID_BUTTON_NEAR ,ID_BUTTON_ENDPOINT, 
					  ID_BUTTON_PERPOINT, ID_OBJ_INFO, ID_UCS, 0 , ID_BUTTON_CENPOINT, ID_TEXT_EDIT,
					  ID_OBJ_MOVE, -1, ID_BUTTON_MIDPOINT, ID_BUTTON_NODE, /*ID_MOVEDATA_CHECK*/0,
					  /*ID_SAVE_MARK*/0, /*ID_LOAD_MARK*/0,/*ID_WRITECHECK_MARK*/0};	

	long nSize = sizeof(CmdId) / sizeof(WPARAM);
	GetWindowText(strCmd);
	if(strCmd.GetLength() < m_nHeadNum) return FALSE;
	strCmd.MakeUpper();
	strCmd = strCmd.Mid(m_nHeadNum);

	strCmd.TrimLeft();

	if(ErrorCmd()) return FALSE;
	if(strCmd == "" && m_sType == TYPE_NORMAL) {
		//OnHelpApp();		
		return FALSE;
	}

	for(int i=0;i<nSize;i++) {
		if(Command[i].Left(strCmd.GetLength()) == strCmd)
		{
			if(pDoc->m_bBrpFileOpen==0 && Command[i] != "OPEN" && Command[i] != "HELP") 
			{
				InitMembers();
				AddHistory("파일부터 오픈하세요.",FALSE);	
				return TRUE;
			}
			CString Str = "";
			if(CmdId[i]==0) {				
				if(Command[i] == "HELP" && m_sType == TYPE_NORMAL) {
					OnHelpApp();
					return FALSE;
				}
				AddHistory(Command[i],FALSE);
				m_nCmdTree++;
				m_bParse = TRUE;
				if(i==0)  Str = _T("명령줄 : (Window, All, Dynamic, Extend, Previous, In, Out) [Window] ? ");			
				if(i==3)  Str = _T("명령줄 : 이동할 페이지 입력 : ");			
				if(i==15) Str = _T("명령줄 : ViewRes 입력 : ");			

				m_CmdName = Command[i];
				m_nHeadNum = Str.GetLength();				
				InitMembers(Str);

				if(i==0) m_LButtonCmd = ID_ZOOM_WINDOW;
				return TRUE;
			}
			if(CmdId[i]==-1)
			{				
				AddHistory(Command[i],FALSE);
				InitMembers(Str);
				return TRUE;
			}
			if(CmdId[i]) {
				AddHistory(Command[i],FALSE);
				pMain->PostMessage(WM_COMMAND,CmdId[i],0);
				InitMembers();
				m_CmdName = "";
				m_nCmdTree = 0;	
				return FALSE;
			}			
		}
	}	
	InitMembers();
	m_CmdName = "";
	m_nCmdTree = 0;	
	AddHistory("존재하지 않는 명령입니다.",FALSE);	
	//SetFocus();
	return TRUE;
}

BOOL CCmdEdit::CmdParse2()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();	
	CMDIChildWnd* pChild=pMain->MDIGetActive();	
	CAPlateDoc* pDoc = (CAPlateDoc*)pChild->GetActiveDocument();
	CString strCmd;
	CString Command1[] = {
			"WINDOW", "ALL", "DYNAMIC", "EXTEND", "PREVIOUS", "IN", "OUT"
			};

	GetWindowText(strCmd);
	if(strCmd.GetLength() < m_nHeadNum) return FALSE;
	strCmd.MakeUpper();
	strCmd = strCmd.Mid(m_nHeadNum);

	strCmd.TrimLeft();

	if(m_CmdName == "ZOOM") 
	{
		for(int i=0;i<7;i++) {
			if(Command1[i].Left(strCmd.GetLength()) == strCmd)
			{				
				AddHistory(Command1[i],FALSE);
				InitMembers();				
				m_CmdName = "";
				m_nCmdTree = 0;				
				if(i==0) pMain->PostMessage(WM_COMMAND,ID_ZOOM_WINDOW,0);
				if(i==1) pMain->PostMessage(WM_COMMAND,ID_ZOOM_ALL,0);
				if(i==2) pMain->PostMessage(WM_COMMAND,ID_ZOOM_DYNAMIC,0);
				if(i==3) pMain->PostMessage(WM_COMMAND,ID_ZOOM_ALL,0);
				if(i==4) pMain->PostMessage(WM_COMMAND,ID_ZOOM_PREVIOUS,0);
				if(i==5) pMain->PostMessage(WM_COMMAND,ID_ZOOM_IN,0);
				if(i==6) pMain->PostMessage(WM_COMMAND,ID_ZOOM_OUT,0);							
				m_bParse = FALSE;
				return FALSE;
			}			
		}
		return TRUE;
	}

	if(m_CmdName == "VIEWRES")
	{
	}

	InitMembers();
	m_bParse = FALSE;
	m_CmdName = "";
	m_nCmdTree = 0;		
	return FALSE;
}

void CCmdEdit::OnLButtonDown(UINT nFlags, CPoint uXy) 
{
	// TODO: Add your message handler code here and/or call default		
	CEdit::OnLButtonDown(nFlags, uXy);
}

void CCmdEdit::OnSetfocus() 
{
	// TODO: Add your control notification handler code here
//	SendMessage(WM_KEYDOWN,35);
}

void CCmdEdit::SetChar(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default	
	if(nChar == VK_ESCAPE) 
	{
		AddHistory("* 취소 *",FALSE);
		m_CmdName = "";
		m_nCmdTree = 0;
		m_LButtonCmd = 0;		
		InitMembers();
		return;	
	}
	if(nChar == 8 || nChar == 36 || nChar == 37)
	{
		CString str;
		GetWindowText(str);
		if(m_CmdStr.GetLength() >= str.GetLength()) return;	
	}	
	if(nChar == VK_RETURN || nChar == ' ')
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();	
		CMDIChildWnd* pChild=pMain->MDIGetActive();	
		CAPlateView* pView = (CAPlateView*)pChild->GetActiveView();
		CDrawTool* pTool = pView->FindTool(pView->GetDrawShape());
		CString strCmd;
		GetWindowText(strCmd);
		if(!m_bParse) CmdParse();		
		else		  CmdParse2();							
		return;
	}	
	/*
	if(GetFocus() != this)
	{
		SetFocus();				
	}*/		
	SendMessage(WM_CHAR,nChar);
}

void CCmdEdit::AddHistory(CString Str,BOOL bNew)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	CString strText;
	strText = m_CmdStr + Str + "\r\n";
	
	if(bNew)
	{
		m_pHisEdit->SetWindowText(strText);
		((CHisEdit *)m_pHisEdit)->ScrollAtEnd();	
	}
	else
		pMain->AddHisData(strText);	
}

void CCmdEdit::OnHelpApp()
{
	AddHistory("HELP",FALSE);
	::MessageBox( AfxGetMainWnd()->GetSafeHwnd(),					
					"<Help> - 현재 도움말 보기\n"
					"<Zoom> - 화면 확대 및 축소 메뉴\n"
					"<Pan> - 하면 이동하기\n"
					"<Redraw> - 현재화면 새로 그리기\n"
					"<Go> - 지정한 페이지도 이동하기\n"
					"<Open> - BRI 파일 열기\n"
					"<Save> - BRI 파일 다른이름으로 저장하기\n"
					"<Qsave> - BRI 파일 같은이름으로 저장하기\n"
					"<Close> - 파일을 닫고 종료하기\n\n"
					"<Dist> - 두점의 거리를 구함\n"
					"<Near> - 가장 가까운 점 찾기\n"
					"<End> - 끝점 찾기\n"
					"<Perpendicular> - 직교점 찾기\n"					
					"<Property> - 객체 정보 얻기\n\n"
					"* 각 항목의 첫글자부터 일치하는순으로\n"
					"  검색합니다.",					
					"============<명령어 도움말>============",
					0
				 );
	
	InitMembers();	
}

void CCmdEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_RETURN || nChar == ' ')
	{
		SetChar(nChar);				
	}	
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CCmdEdit::ErrorCmd()
{
	switch(m_sType)
	{
	case TYPE_OBJ:
		AddHistory("\r\n잘못된 객체 선택입니다.",FALSE);		
		return TRUE;
	}
	return FALSE;
}

BOOL CCmdEdit::OnLButtonCmd()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();			
	if(m_LButtonCmd!=0)
	{
		AddHistory("",FALSE);
		pMain->SendMessage(WM_COMMAND,m_LButtonCmd,0);		
		m_LButtonCmd = 0;
		m_nCmdTree = 0;
		InitMembers();
		return TRUE;
	}
	return FALSE;
}