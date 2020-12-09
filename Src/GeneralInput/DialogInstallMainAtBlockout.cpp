// DialogInstallMainAtBlockout.cpp : implementation file
//

#include "stdafx.h"
#include "DialogInstallMainAtBlockout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogInstallMainAtBlockout dialog


CDialogInstallMainAtBlockout::CDialogInstallMainAtBlockout(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDialogInstallMainAtBlockout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInstallMainAtBlockout)
	m_nInstallMainAtBlockout = -1;
	//}}AFX_DATA_INIT
}


void CDialogInstallMainAtBlockout::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInstallMainAtBlockout)
	DDX_Radio(pDX, IDC_RADIO_INSTALL, m_nInstallMainAtBlockout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogInstallMainAtBlockout, CDialog)
	//{{AFX_MSG_MAP(CDialogInstallMainAtBlockout)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogInstallMainAtBlockout message handlers

void CDialogInstallMainAtBlockout::OnOK() 
{
	UpdateData();
	if(m_nInstallMainAtBlockout==0)			m_ReturnValue = "설치";
	else if(m_nInstallMainAtBlockout==1)	m_ReturnValue = "미설치";

	
	CUserDialog::OnOK();
}

BOOL CDialogInstallMainAtBlockout::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	if(m_ReturnValue== "설치") 						m_nInstallMainAtBlockout=0;
	else if(m_ReturnValue== "미설치") 				m_nInstallMainAtBlockout=1;		

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
