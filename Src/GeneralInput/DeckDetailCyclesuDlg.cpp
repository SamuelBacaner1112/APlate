// DeckDetailCyclesuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DeckDetailCyclesuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailCyclesuDlg dialog


CDeckDetailCyclesuDlg::CDeckDetailCyclesuDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDeckDetailCyclesuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDetailCyclesuDlg)
	m_nCyclesu = 1;
	//}}AFX_DATA_INIT
}


void CDeckDetailCyclesuDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDetailCyclesuDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nCyclesu);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDeckDetailCyclesuDlg, CDialog)
	//{{AFX_MSG_MAP(CDeckDetailCyclesuDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailCyclesuDlg message handlers
BOOL CDeckDetailCyclesuDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	if(m_pDeckData->m_nMainCyclesu==1)
		m_nCyclesu = 0;
	else if(m_pDeckData->m_nMainCyclesu==2)
		m_nCyclesu = 1;
	else if(m_pDeckData->m_nMainCyclesu==4)
		m_nCyclesu = 2;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeckDetailCyclesuDlg::OnOK() 
{
	UpdateData();

	m_pDeckData->m_nMainCyclesu = m_nCyclesu;
	if(m_pDeckData->m_nMainCyclesu==0)
		m_pDeckData->m_nMainCyclesu = 1;
	else if(m_pDeckData->m_nMainCyclesu==1)
		m_pDeckData->m_nMainCyclesu = 2;
	else if(m_pDeckData->m_nMainCyclesu==2)
		m_pDeckData->m_nMainCyclesu = 4;

	CUserDialog::OnOK();
}
