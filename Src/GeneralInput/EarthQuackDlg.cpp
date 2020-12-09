// EarthQuackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "EarthQuackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEarthQuackDlg dialog


CEarthQuackDlg::CEarthQuackDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEarthQuackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEarthQuackDlg)
	m_nType = -1;
	//}}AFX_DATA_INIT
	m_dZoneCoefficient = 0.11;
}


void CEarthQuackDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEarthQuackDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEarthQuackDlg, CDialog)
	//{{AFX_MSG_MAP(CEarthQuackDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEarthQuackDlg message handlers

BOOL CEarthQuackDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	if(m_ReturnValue== "I 备开") 
	{
		m_dZoneCoefficient = 0.11;
		m_nType=0;
	}
	else
	{
		m_dZoneCoefficient = 0.07;
		m_nType=1;
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEarthQuackDlg::OnOK() 
{
	UpdateData();
	if(m_nType==0)
	{
		m_dZoneCoefficient = 0.11;
		m_ReturnValue = "I 备开";
	}
	else
	{
		m_dZoneCoefficient = 0.07;
		m_ReturnValue = "II备开";
	}
	CUserDialog::OnOK();
}
