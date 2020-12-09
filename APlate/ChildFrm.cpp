// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "APlate.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_MAXIMIZE|FWS_ADDTOTITLE;
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	CMainFrame* pMFrm =(CMainFrame*)GetParentOwner();
	switch (nID & 0xFFF0)
	{
	case SC_CLOSE:
		{			 
			CMSDIWinApp* pApp = (CMSDIWinApp*)AfxGetApp();
			break;
		}
	default:
		CFrameWnd::OnSysCommand(nID, lParam);
	}
} 

BOOL CChildFrame::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{	
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
