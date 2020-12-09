// EarthQuackFactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "EarthQuackFactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEarthQuackFactDlg dialog


CEarthQuackFactDlg::CEarthQuackFactDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEarthQuackFactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEarthQuackFactDlg)
	m_nType = -1;
	m_dFact1 = _T("1.00");
	m_dFact2 = _T("1.20");
	m_dFact3 = _T("1.50");
	m_dFact4 = _T("2.00");
	m_dFact5 = _T("0.00");
	//}}AFX_DATA_INIT
}


void CEarthQuackFactDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEarthQuackFactDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	DDX_Text(pDX, IDC_EDIT6, m_dFact1);
	DDX_Text(pDX, IDC_EDIT2, m_dFact2);
	DDX_Text(pDX, IDC_EDIT3, m_dFact3);
	DDX_Text(pDX, IDC_EDIT4, m_dFact4);
	DDX_Text(pDX, IDC_EDIT5, m_dFact5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEarthQuackFactDlg, CDialog)
	//{{AFX_MSG_MAP(CEarthQuackFactDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEarthQuackFactDlg message handlers

BOOL CEarthQuackFactDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	if(m_ReturnValue=="1.00") m_nType=0;
	else if(m_ReturnValue=="1.20") m_nType=1;
	else if(m_ReturnValue=="1.50") m_nType=2;
	else if(m_ReturnValue=="2.00") m_nType=3;
	else m_nType=4;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEarthQuackFactDlg::OnOK() 
{
	UpdateData();
	if(m_nType==0)	m_ReturnValue.Format("%.2f", atof(m_dFact1));
	else if(m_nType==1)	m_ReturnValue.Format("%.2f", atof(m_dFact2));
	else if(m_nType==2)	m_ReturnValue.Format("%.2f", atof(m_dFact3));
	else if(m_nType==3)	m_ReturnValue.Format("%.2f", atof(m_dFact4));
	else if(m_nType==4)	m_ReturnValue.Format("%.2f", atof(m_dFact5));	
	CUserDialog::OnOK();
}
