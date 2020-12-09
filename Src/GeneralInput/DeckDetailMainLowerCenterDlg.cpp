// DeckDetailMainLowerCenterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DeckDetailMainLowerCenterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainLowerCenterDlg dialog


CDeckDetailMainLowerCenterDlg::CDeckDetailMainLowerCenterDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDeckDetailMainLowerCenterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDetailMainLowerCenterDlg)
	m_nType = -1;
	//}}AFX_DATA_INIT
}


void CDeckDetailMainLowerCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDetailMainLowerCenterDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDeckDetailMainLowerCenterDlg, CDialog)
	//{{AFX_MSG_MAP(CDeckDetailMainLowerCenterDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainLowerCenterDlg message handlers
BOOL CDeckDetailMainLowerCenterDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	m_nType = m_pDeckData->m_nDetailMainLowerCenter;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeckDetailMainLowerCenterDlg::OnOK() 
{
	UpdateData();

	m_pDeckData->m_nDetailMainLowerCenter = m_nType;

	if(m_nType==0)
	{
		AfxMessageBox("분리배치는 추후 지원예정입니다.");
		return;
	}

	CUserDialog::OnOK();
}
