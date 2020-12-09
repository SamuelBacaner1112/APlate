// OptionEtcDetail.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionEtcDetail property page

IMPLEMENT_DYNCREATE(COptionEtcDetail, CPropertyPage)

COptionEtcDetail::COptionEtcDetail() : CPropertyPage(COptionEtcDetail::IDD)
{
	//{{AFX_DATA_INIT(COptionEtcDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionEtcDetail::~COptionEtcDetail()
{
}

void COptionEtcDetail::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionEtcDetail)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionEtcDetail, CPropertyPage)
	//{{AFX_MSG_MAP(COptionEtcDetail)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionEtcDetail message handlers

int COptionEtcDetail::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void COptionEtcDetail::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}

BOOL COptionEtcDetail::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd *pStd = m_pPlateDrawStd;

	pStd->m_pDomOptionStd->m_nOptionCur = OPTION_PAGE_AROAD_ETCDETAIL;
	
	return CPropertyPage::OnSetActive();
}

BOOL COptionEtcDetail::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionEtcDetail::OnDefault()
{
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

void COptionEtcDetail::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void COptionEtcDetail::InitTreeData(BOOL bDefaultLoad)
{	
}

void COptionEtcDetail::InitGridData(BOOL bDefaultLoad)
{
}