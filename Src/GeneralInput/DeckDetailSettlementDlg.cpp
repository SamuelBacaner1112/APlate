// DeckDetailSettlementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DeckDetailSettlementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailSettlementDlg dialog


CDeckDetailSettlementDlg::CDeckDetailSettlementDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDeckDetailSettlementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDetailSettlementDlg)
	m_nType = -1;
	//}}AFX_DATA_INIT
}


void CDeckDetailSettlementDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDetailSettlementDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDeckDetailSettlementDlg, CDialog)
	//{{AFX_MSG_MAP(CDeckDetailSettlementDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailSettlementDlg message handlers
BOOL CDeckDetailSettlementDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	m_nType = m_pDeckData->m_nDetailSettlement;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeckDetailSettlementDlg::OnOK() 
{
	UpdateData();

	m_pDeckData->m_nDetailSettlement = m_nType;

	CUserDialog::OnOK();
}
