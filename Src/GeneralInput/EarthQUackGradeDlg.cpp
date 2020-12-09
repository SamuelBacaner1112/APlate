// EarthQUackGradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "EarthQUackGradeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEarthQUackGradeDlg dialog


CEarthQUackGradeDlg::CEarthQUackGradeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEarthQUackGradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEarthQUackGradeDlg)
	m_nType = -1;
	//}}AFX_DATA_INIT
	m_dRiskCoefficient = 1.4;
}


void CEarthQUackGradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEarthQUackGradeDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEarthQUackGradeDlg, CDialog)
	//{{AFX_MSG_MAP(CEarthQUackGradeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEarthQUackGradeDlg message handlers

BOOL CEarthQUackGradeDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	if(m_ReturnValue== "내진I 등급") 
		m_nType=0;
	else if(m_ReturnValue== "내진II등급") 
		m_nType=1;		

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEarthQUackGradeDlg::OnOK() 
{

	UpdateData();
	if(m_nType==0)
	{
		m_dRiskCoefficient = 1.4;
		m_ReturnValue = "내진I 등급";
	}
	else if(m_nType==1)
	{
		m_dRiskCoefficient = 1.0;
		m_ReturnValue = "내진II등급";
	}

	CUserDialog::OnOK();
}
