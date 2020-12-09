// DialogInstallBeryukAtBlockout.cpp : implementation file
//

#include "stdafx.h"
#include "DialogInstallBeryukAtBlockout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogInstallBeryukAtBlockout dialog


CDialogInstallBeryukAtBlockout::CDialogInstallBeryukAtBlockout(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDialogInstallBeryukAtBlockout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInstallBeryukAtBlockout)
	m_nInstallBeryukAtBlockout = -1;
	//}}AFX_DATA_INIT
}


void CDialogInstallBeryukAtBlockout::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInstallBeryukAtBlockout)
	DDX_Radio(pDX, IDC_RADIO_INSTALL, m_nInstallBeryukAtBlockout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogInstallBeryukAtBlockout, CDialog)
	//{{AFX_MSG_MAP(CDialogInstallBeryukAtBlockout)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogInstallBeryukAtBlockout message handlers

void CDialogInstallBeryukAtBlockout::OnOK() 
{
	UpdateData();
	if(m_nInstallBeryukAtBlockout==0)		m_ReturnValue = "설치";
	else if(m_nInstallBeryukAtBlockout==1)	m_ReturnValue = "미설치";

	
	CUserDialog::OnOK();
}

BOOL CDialogInstallBeryukAtBlockout::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	if(m_ReturnValue== "설치") 						m_nInstallBeryukAtBlockout=0;
	else if(m_ReturnValue== "미설치") 				m_nInstallBeryukAtBlockout=1;		

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
