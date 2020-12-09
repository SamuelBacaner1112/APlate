// GirHeightUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "GirHeightUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGirHeightUserDlg dialog


CGirHeightUserDlg::CGirHeightUserDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CGirHeightUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGirHeightUserDlg)
	m_nGirHtRadio = -1;
	//}}AFX_DATA_INIT
}


void CGirHeightUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGirHeightUserDlg)
	DDX_Control(pDX, IDC_GIRHT_RADIO1, m_GirHtRadio1);
	DDX_Control(pDX, IDC_GIRHT_RADIO2, m_GirHtRadio2);
	DDX_Control(pDX, IDC_GIRHT_RADIO3, m_GirHtRadio3);
	DDX_Control(pDX, IDC_GIRHT_RADIO4, m_GirHtRadio4);
	DDX_Radio(pDX, IDC_GH_RADIO1, m_nGirHtRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGirHeightUserDlg, CDialog)
	//{{AFX_MSG_MAP(CGirHeightUserDlg)
	ON_BN_CLICKED(IDC_GIRHT_RADIO1, OnGirhtRadio1)
	ON_BN_CLICKED(IDC_GIRHT_RADIO2, OnGirhtRadio2)
	ON_BN_CLICKED(IDC_GIRHT_RADIO3, OnGirhtRadio3)
	ON_BN_CLICKED(IDC_GIRHT_RADIO4, OnGirhtRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGirHeightUserDlg message handlers

BOOL CGirHeightUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(bIsTU)
	{
		m_GirHtRadio1.LoadBitmaps(IDB_GIRHT_RAD1_DOWN, IDB_GIRHT_RAD1_DOWN,IDB_GIRHT_RAD1_DOWN,IDB_GIRHT_RAD1_DOWN);
		m_GirHtRadio3.LoadBitmaps(IDB_GIRHT_RAD3_DOWN, IDB_GIRHT_RAD3_DOWN,IDB_GIRHT_RAD3_DOWN,IDB_GIRHT_RAD3_DOWN);
		m_GirHtRadio4.LoadBitmaps(IDB_GIRHT_RAD4_DOWN, IDB_GIRHT_RAD4_DOWN,IDB_GIRHT_RAD4_DOWN,IDB_GIRHT_RAD4_DOWN);
	}
	else
	{
		m_GirHtRadio1.LoadBitmaps(IDB_GIRHT_RAD1, IDB_GIRHT_RAD1_DOWN,IDB_GIRHT_RAD1,IDB_GIRHT_RAD1);		
		m_GirHtRadio3.LoadBitmaps(IDB_GIRHT_RAD3, IDB_GIRHT_RAD3_DOWN,IDB_GIRHT_RAD3,IDB_GIRHT_RAD3);
		m_GirHtRadio4.LoadBitmaps(IDB_GIRHT_RAD4, IDB_GIRHT_RAD4_DOWN,IDB_GIRHT_RAD4,IDB_GIRHT_RAD4);
	}
	m_GirHtRadio2.LoadBitmaps(IDB_GIRHT_RAD2, IDB_GIRHT_RAD2_DOWN,IDB_GIRHT_RAD2,IDB_GIRHT_RAD2);
 
	m_GirHtRadio1.SizeToContent();
	m_GirHtRadio2.SizeToContent();
	m_GirHtRadio3.SizeToContent();
	m_GirHtRadio4.SizeToContent();

	if(m_ReturnValue == GH_STR_UILI)
		m_nGirHtRadio = 0;
	if(m_ReturnValue == GH_STR_UOLO)
		m_nGirHtRadio = 1;
	if(m_ReturnValue == GH_STR_UILO)
		m_nGirHtRadio = 2;
	if(m_ReturnValue == GH_STR_UOLI)
		m_nGirHtRadio = 3;

	m_GirHtRadio1.EnableWindow(!bIsTU);
	m_GirHtRadio3.EnableWindow(!bIsTU);
	m_GirHtRadio4.EnableWindow(!bIsTU);
	GetDlgItem(IDC_GH_RADIO1)->EnableWindow(!bIsTU);
	GetDlgItem(IDC_GH_RADIO3)->EnableWindow(!bIsTU);
	GetDlgItem(IDC_GH_RADIO4)->EnableWindow(!bIsTU);
 
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGirHeightUserDlg::OnOK() 
{
	UpdateData(TRUE);


	switch(m_nGirHtRadio)
	{
	case 0:	m_ReturnValue = GH_STR_UILI; break;
	case 1:	m_ReturnValue = GH_STR_UOLO; break;
	case 2:	m_ReturnValue = GH_STR_UILO; break;
	case 3:	m_ReturnValue = GH_STR_UOLI; break;
	}


	CDialog::OnOK();
}


void CGirHeightUserDlg::OnGirhtRadio1() 
{
	m_nGirHtRadio = 0;
	UpdateData(FALSE);
	
}

void CGirHeightUserDlg::OnGirhtRadio2() 
{
	m_nGirHtRadio = 1;
	UpdateData(FALSE);
	
}

void CGirHeightUserDlg::OnGirhtRadio3() 
{
	m_nGirHtRadio = 2;
	UpdateData(FALSE);
	
}

void CGirHeightUserDlg::OnGirhtRadio4() 
{
	m_nGirHtRadio = 3;
	UpdateData(FALSE);
	
}
