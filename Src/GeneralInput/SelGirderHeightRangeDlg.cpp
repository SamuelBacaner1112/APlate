// SelGirderHeightRangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "SelGirderHeightRangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelGirderHeightRangeDlg dialog


CSelGirderHeightRangeDlg::CSelGirderHeightRangeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CSelGirderHeightRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelGirderHeightRangeDlg)
	m_dGirderHeight = 0.0;
	m_szRecommendResult = _T("");
	//}}AFX_DATA_INIT
}


void CSelGirderHeightRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelGirderHeightRangeDlg)
	DDX_Text(pDX, IDC_EDIT_SELGIRDERHEIGHT, m_dGirderHeight);
	DDX_Text(pDX, IDC_STATIC_GHTEXT, m_szRecommendResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelGirderHeightRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelGirderHeightRangeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelGirderHeightRangeDlg message handlers

BOOL CSelGirderHeightRangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	double dMaxJigan = 0;
	for(long n=0; n<m_pBridge->m_nQtyJigan; n++)
	{
		dMaxJigan = max(dMaxJigan, toM(Round(m_pBridge->m_dLengthJigan[n],-3)));
	}
	if(m_bSM520)	
		m_szRecommendResult.Format("SM 520C-TMC : L/16 ~ L/22 = %.3f/16 ~ %.3f/22 = %.3f m ~ %.3f m", dMaxJigan, dMaxJigan, dMaxJigan/16, dMaxJigan/22);
	else
		m_szRecommendResult.Format("SM 570-TMC : L/16 ~ L/25 = %.3f/16 ~ %.3f/25 = %.3f m ~ %.3f m", dMaxJigan, dMaxJigan, dMaxJigan/16, dMaxJigan/25);

	m_dGirderHeight = atof(m_ReturnValue);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelGirderHeightRangeDlg::OnOK() 
{
	UpdateData();

	m_ReturnValue.Format("%g",m_dGirderHeight);
	
	CDialog::OnOK();
}
