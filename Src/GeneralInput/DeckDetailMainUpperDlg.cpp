// DeckDetailMainUpperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DeckDetailMainUpperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainUpperDlg dialog


CDeckDetailMainUpperDlg::CDeckDetailMainUpperDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDeckDetailMainUpperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDetailMainUpperDlg)
	m_nType = 0;
	//}}AFX_DATA_INIT
}


void CDeckDetailMainUpperDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDetailMainUpperDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDeckDetailMainUpperDlg, CDialog)
	//{{AFX_MSG_MAP(CDeckDetailMainUpperDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainUpperDlg message handlers
BOOL CDeckDetailMainUpperDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	m_nType = m_pDeckData->m_nDetailMainUpper;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeckDetailMainUpperDlg::OnOK() 
{
	UpdateData();
	
	m_pDeckData->m_nDetailMainUpper = m_nType;

	CUserDialog::OnOK();
}

