// FileProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "FileProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileProgressDlg dialog


CFileProgressDlg::CFileProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileProgressDlg)
	//}}AFX_DATA_INIT
	m_nType = 0;
}


void CFileProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileProgressDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS_EX, m_ctlProgress);
}


BEGIN_MESSAGE_MAP(CFileProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CFileProgressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileProgressDlg message handlers

BOOL CFileProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CCalcCantileverModelResult*)m_pParent)->m_pProgressDlg = this;
	CenterWindow();
	ShowWindow(SW_SHOW);
	Sleep(100);
	Begin();
	
	return TRUE;
}

UINT CFileProgressDlg::DoWork()
{
	Sleep(0);
	m_pParent->SendMessage(WM_RUNTHREAD, m_nType);
	PostMessage(WM_CLOSE);
	Sleep(100);
	
	return 0;
}
