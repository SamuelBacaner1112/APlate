// DeckDetailMainLowerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DeckDetailMainLowerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainLowerDlg dialog


CDeckDetailMainLowerDlg::CDeckDetailMainLowerDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDeckDetailMainLowerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDetailMainLowerDlg)
	m_nType = 0;
	
	m_dLengthDolchulLoDanLeft = 0;
	m_dLengthDolchulLoDanRigh = 0;
	m_dLengthDolchulLoGenLeft = 0;
	m_dLengthDolchulLoGenRigh = 0;

	//}}AFX_DATA_INIT
}


void CDeckDetailMainLowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDetailMainLowerDlg)
	DDX_Control(pDX, IDC_EDIT_DOLCHUL_LO_DAN_LEFT, m_ctrlLengthDolchulLoDanLeft);
	DDX_Control(pDX, IDC_EDIT_DOLCHUL_LO_DAN_RIGH, m_ctrlLengthDolchulLoDanRigh);
	DDX_Control(pDX, IDC_EDIT_DOLCHUL_LO_GEN_LEFT, m_ctrlLengthDolchulLoGenLeft);
	DDX_Control(pDX, IDC_EDIT_DOLCHUL_LO_GEN_RIGH, m_ctrlLengthDolchulLoGenRigh);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	DDX_Text(pDX, IDC_EDIT_DOLCHUL_LO_DAN_LEFT, m_dLengthDolchulLoDanLeft);
	DDX_Text(pDX, IDC_EDIT_DOLCHUL_LO_DAN_RIGH, m_dLengthDolchulLoDanRigh);
	DDX_Text(pDX, IDC_EDIT_DOLCHUL_LO_GEN_LEFT, m_dLengthDolchulLoGenLeft);
	DDX_Text(pDX, IDC_EDIT_DOLCHUL_LO_GEN_RIGH, m_dLengthDolchulLoGenRigh);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDeckDetailMainLowerDlg, CDialog)
	//{{AFX_MSG_MAP(CDeckDetailMainLowerDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioJiksunType)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioAngleType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailMainLowerDlg message handlers
BOOL CDeckDetailMainLowerDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	m_dLengthDolchulLoDanLeft = Round(m_pDeckData->m_dLengthMainDolchulLoDan[0], 0);
	m_dLengthDolchulLoDanRigh = Round(m_pDeckData->m_dLengthMainDolchulLoDan[1], 0);
	m_dLengthDolchulLoGenLeft = Round(m_pDeckData->m_dLengthMainDolchulLo[0], 0);
	m_dLengthDolchulLoGenRigh = Round(m_pDeckData->m_dLengthMainDolchulLo[1], 0);
	
	if(	m_ReturnValue== "직선타입") 
	{
		m_nType=0;
		m_ctrlLengthDolchulLoDanLeft.EnableWindow(FALSE);
		m_ctrlLengthDolchulLoDanRigh.EnableWindow(FALSE);
		m_ctrlLengthDolchulLoGenLeft.EnableWindow(FALSE);
		m_ctrlLengthDolchulLoGenRigh.EnableWindow(FALSE);
	}
	else if(m_ReturnValue== "ㄱ타입") 
	{
		m_ctrlLengthDolchulLoDanLeft.EnableWindow(TRUE);
		m_ctrlLengthDolchulLoDanRigh.EnableWindow(TRUE);
		m_ctrlLengthDolchulLoGenLeft.EnableWindow(TRUE);
		m_ctrlLengthDolchulLoGenRigh.EnableWindow(TRUE);
		m_nType=1;
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeckDetailMainLowerDlg::OnOK() 
{
	UpdateData();

	m_pDeckData->m_dLengthMainDolchulLoDan[0] = m_dLengthDolchulLoDanLeft;
	m_pDeckData->m_dLengthMainDolchulLoDan[1] = m_dLengthDolchulLoDanRigh;
	m_pDeckData->m_dLengthMainDolchulLo[0]    = m_dLengthDolchulLoGenLeft; 
	m_pDeckData->m_dLengthMainDolchulLo[1]    = m_dLengthDolchulLoGenRigh; 
	
	if(m_nType==0)
		m_ReturnValue = "직선타입";
	else if(m_nType==1)
		m_ReturnValue = "ㄱ타입";

	CUserDialog::OnOK();
}


void CDeckDetailMainLowerDlg::OnRadioJiksunType() 
{
	m_ctrlLengthDolchulLoDanLeft.EnableWindow(FALSE);
	m_ctrlLengthDolchulLoDanRigh.EnableWindow(FALSE);
	m_ctrlLengthDolchulLoGenLeft.EnableWindow(FALSE);
	m_ctrlLengthDolchulLoGenRigh.EnableWindow(FALSE);

}

void CDeckDetailMainLowerDlg::OnRadioAngleType() 
{
	m_ctrlLengthDolchulLoDanLeft.EnableWindow(TRUE);
	m_ctrlLengthDolchulLoDanRigh.EnableWindow(TRUE);
	m_ctrlLengthDolchulLoGenLeft.EnableWindow(TRUE);
	m_ctrlLengthDolchulLoGenRigh.EnableWindow(TRUE);
}
