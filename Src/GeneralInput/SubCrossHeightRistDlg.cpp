// SubCrossHeightRistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "SubCrossHeightRistDlg.h"
#include "DetCrossBeamTypeDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubCrossHeightRistDlg dialog


CSubCrossHeightRistDlg::CSubCrossHeightRistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubCrossHeightRistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubCrossHeightRistDlg)
	m_dDanJijum = 70.0;
	m_dCenterJijum = 70.0;
	m_dGen = 35.0;
	//}}AFX_DATA_INIT
}


void CSubCrossHeightRistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubCrossHeightRistDlg)
	DDX_Text(pDX, IDC_EDIT_CROSSPER_DANJIJUM, m_dDanJijum);
	DDX_Text(pDX, IDC_EDIT_CROSSPER_CENTERJIJUM, m_dCenterJijum);
	DDX_Text(pDX, IDC_EDIT_CROSSPER_GEN, m_dGen);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_CROSSH_RECOMMEND, m_btnBase);
	DDX_Control(pDX, IDC_BUTTON_CROSSH_APPLYALL, m_btnAll);
	DDX_Control(pDX, IDC_BUTTON_CROSSH_APPLY, m_btnApply);
}


BEGIN_MESSAGE_MAP(CSubCrossHeightRistDlg, CDialog)
	//{{AFX_MSG_MAP(CSubCrossHeightRistDlg)
	ON_BN_CLICKED(IDC_BUTTON_CROSSH_RECOMMEND, OnButtonCrosshRecommend)
	ON_BN_CLICKED(IDC_BUTTON_CROSSH_APPLYALL, OnButtonCrosshApplyall)
	ON_BN_CLICKED(IDC_BUTTON_CROSSH_APPLY, OnButtonCrosshApply)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubCrossHeightRistDlg message handlers
void CSubCrossHeightRistDlg::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{	 
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	if(nState == WA_INACTIVE) 
	{
		ShowWindow(SW_HIDE);
	}
}

void CSubCrossHeightRistDlg::OnButtonCrosshRecommend() 
{
	m_dDanJijum = 70.0;
	m_dCenterJijum = 70.0;
	m_dGen = 35.0;

	UpdateData(FALSE);	
}

void CSubCrossHeightRistDlg::OnButtonCrosshApplyall() 
{
	UpdateData(TRUE);
	ShowWindow(SW_HIDE);
	m_pCRBeamDlg->OnRistRecommendOnRatio(TRUE, m_dDanJijum/100, m_dGen/100, m_dCenterJijum/100);
}

void CSubCrossHeightRistDlg::OnButtonCrosshApply() 
{
	UpdateData(TRUE);
	ShowWindow(SW_HIDE);	
	m_pCRBeamDlg->OnRistRecommendOnRatio(FALSE, m_dDanJijum/100, m_dGen/100, m_dCenterJijum/100);
}

BOOL CSubCrossHeightRistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;
}
