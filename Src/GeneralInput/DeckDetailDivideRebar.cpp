// DeckDetailDivideRebar.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DeckDetailDivideRebar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailDivideRebar dialog


CDeckDetailDivideRebar::CDeckDetailDivideRebar(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDeckDetailDivideRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDetailDivideRebar)
	m_nDivideRebar = 0;
	//}}AFX_DATA_INIT
}


void CDeckDetailDivideRebar::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDetailDivideRebar)
	DDX_Radio(pDX, IDC_RADIO1, m_nDivideRebar);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDeckDetailDivideRebar, CDialog)
	//{{AFX_MSG_MAP(CDeckDetailDivideRebar)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioDivide)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioNoDivide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDetailDivideRebar message handlers

void CDeckDetailDivideRebar::OnRadioDivide() 
{
	UpdateData();
	CUserDialog::OnInitDialog();

	
}

void CDeckDetailDivideRebar::OnRadioNoDivide() 
{
	UpdateData();
	CUserDialog::OnInitDialog();
	
}


void CDeckDetailDivideRebar::OnOK() 
{
	UpdateData();

	m_pDeckData->m_bDivideRebarAtCant = m_nDivideRebar==0 ? TRUE : FALSE;
	if(!m_pDeckData->m_pDB->m_bUserSlabHunch && m_pDeckData->m_bDivideRebarAtCant)
	{
		AfxMessageBox("캔틸레버부에 슬래브 헌치가 없기 때문에 분리할수 없습니다.");
		m_pDeckData->m_bDivideRebarAtCant = FALSE;
	}

	
	if(m_nDivideRebar==0)
		m_ReturnValue = "분리";
	else if(m_nDivideRebar==1)
		m_ReturnValue = "미분리";

	CUserDialog::OnOK();
}

BOOL CDeckDetailDivideRebar::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	m_nDivideRebar = m_pDeckData->m_bDivideRebarAtCant ? 0 : 1;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}