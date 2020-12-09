// CommandBar.cpp: implementation of the CCommandBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlate.h"
#include "CommandBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandBar::CCommandBar()
{

}

CCommandBar::~CCommandBar()
{

}

BEGIN_MESSAGE_MAP(CCommandBar, CShSizingControlBar)
	//{{AFX_MSG_MAP(CCommandBar)
	ON_WM_CREATE()
	ON_WM_SIZE()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCommandBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CShSizingControlBar::PreTranslateMessage(pMsg);
}

int CCommandBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CShSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject (DEFAULT_GUI_FONT);

	if(!m_HisEdit.Create(WS_BORDER | WS_VSCROLL | WS_VISIBLE  | WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN | ES_LEFT | ES_MULTILINE,
		              CRect(0,0,0,0),this,ID_HISEDIT))
	{
		TRACE0("Failed to create CommandBar tab window\n");
		return -1;      // fail to create
	}
	m_HisEdit.SetFont (&m_Font);	

	if(!m_CmdEdit.Create(WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL|ES_LEFT |WS_CHILD,CRect(0,0,0,0),this,ID_CMDEDIT))
	{
		TRACE0("Failed to create CommandBar tab window\n");
		return -1;      // fail to create
	}

	m_ScrollBar.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ,CRect(0,0,0,0),this,0);
	m_CmdEdit.SetFont (&m_Font);
	m_CmdEdit.InitMembers();
	m_HisEdit.m_hCmdWnd = m_CmdEdit.GetSafeHwnd();
	m_CmdEdit.m_pHisEdit = &m_HisEdit;		

	m_HisEdit.SetWindowText("<<< APlate2003 - HanGil IT>>>\r\n");

	return 0;
}

void CCommandBar::OnSize(UINT nType, int cx, int cy) 
{	
	CShSizingControlBar::OnSize(nType, cx, cy);

    CRect rc;
    GetClientRect(rc);

	m_HisEdit.SetWindowPos(NULL,
            rc.left, rc.top,
            rc.Width(), rc.Height () - 16,
            SWP_NOACTIVATE | SWP_NOZORDER);
	m_CmdEdit.SetWindowPos(NULL,
            rc.left, rc.bottom - 17,
            rc.Width()-35, 17,
            SWP_NOACTIVATE | SWP_NOZORDER);	
	m_ScrollBar.SetWindowPos(NULL,
			rc.right-35, rc.bottom - 17,
            35, 17,
            SWP_NOACTIVATE | SWP_NOZORDER);	
		
/*
	AdjusrHorzScroll (m_wndOutputBuild);
	AdjusrHorzScroll (m_wndOutputDebug);
	AdjusrHorzScroll (m_wndOutputFind1);
	AdjusrHorzScroll (m_wndOutputFind2);
	AdjusrHorzScroll (m_wndOutputSQL);
	*/
}

void CCommandBar::SetChar(UINT nChar) 
{
	// TODO: Add your message handler code here and/or call default	
	m_CmdEdit.SetChar(nChar);
}
