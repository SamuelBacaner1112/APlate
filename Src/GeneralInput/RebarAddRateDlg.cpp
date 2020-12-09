// RebarAddRateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RebarAddRateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarAddRateDlg dialog


CRebarAddRateDlg::CRebarAddRateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRebarAddRateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarAddRateDlg)
	m_dRate1 = 0.0;
	m_dRate2 = 0.0;
	//}}AFX_DATA_INIT
}


void CRebarAddRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarAddRateDlg)
	DDX_Control(pDX, IDC_COMBO_DIA, m_comboDia);
	DDX_Text(pDX, IDC_EDIT_RATE1, m_dRate1);
	DDX_Text(pDX, IDC_EDIT_RATE2, m_dRate2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_RECOMMEND, m_btnRecommend);
}


BEGIN_MESSAGE_MAP(CRebarAddRateDlg, CDialog)
	//{{AFX_MSG_MAP(CRebarAddRateDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DIA, OnSelchangeComboDia)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarAddRateDlg message handlers

void CRebarAddRateDlg::OnOK() 
{
	UpdateData(TRUE);
	
	if(m_AddRate.m_nType == 0)  //직경으로 설정 
	{
		double dDia = GetCurSelDia();
		m_AddRate.m_dBoundaryDia = dDia;
		m_AddRate.m_dAddRateDia[0] = m_dRate1;
		m_AddRate.m_dAddRateDia[1] = m_dRate2;
	}
	else
	{
		m_AddRate.m_dBoundaryDia = 0;
		m_AddRate.m_dAddRateMain[0] = m_dRate1;
		m_AddRate.m_dAddRateMain[1] = m_dRate2;
	}
	
	CDialog::OnOK();
}


BOOL CRebarAddRateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDataInitComboDia();
	SetDataInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRebarAddRateDlg::SetDataInitComboDia()
{
	m_comboDia.ResetContent();
	SetStringArrDia();

	int nCntString = m_strArrDia.GetSize();
	CString strDia;

	for(int i = 0; i < nCntString; i++)
	{
		strDia = m_strArrDia.GetAt(i);
		m_comboDia.AddString(strDia);
	}

	//제일처음에는 0를 가르키도록 한다.
	m_comboDia.SetCurSel(0);
}


void CRebarAddRateDlg::OnSelchangeComboDia() 
{
	// TODO: Add your control notification handler code here
	m_nCurSelDia = m_comboDia.GetCurSel();
}


double CRebarAddRateDlg::GetCurSelDia()
{
	CString strDia;
	m_comboDia.GetLBText(m_nCurSelDia, strDia);
	if(strDia.GetLength() <= 0)	return 0;
	double dDia = atoi((char*)strDia.Right(2).operator LPCTSTR());
	return dDia;
}	


void CRebarAddRateDlg::SetDataInit()
{
	UpdateData();
	
	SetStaticName();
	SetComboCurDia();

	if(m_AddRate.m_nType==0)
	{
		m_dRate1 = m_AddRate.m_dAddRateDia[0];
		m_dRate2 = m_AddRate.m_dAddRateDia[1];
	}
	else
	{
		m_dRate1 = m_AddRate.m_dAddRateMain[0];
		m_dRate2 = m_AddRate.m_dAddRateMain[1];
	}
	
	UpdateData(FALSE);
}


void CRebarAddRateDlg::SetStaticName()
{
	if(m_AddRate.m_nType == 0)  //직경으로 설정 
	{
		GetDlgItem(IDC_COMBO_DIA)->ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_STATIC_RATETYPE1, "이하철근이면");
		SetDlgItemText(IDC_STATIC_RATETYPE2, "초과철근이면");
	}
	else if(m_AddRate.m_nType == 1)  //주철근으로 설정
	{
		GetDlgItem(IDC_COMBO_DIA)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC_RATETYPE1, "주철근이면");
		SetDlgItemText(IDC_STATIC_RATETYPE2, "기타철근이면");
	}
}


void CRebarAddRateDlg::OnButtonRecommend() 
{
	// TODO: Add your control notification handler code here
	m_AddRate.SetDataDefault();
	SetDataInit();
}


void CRebarAddRateDlg::SetComboCurDia()
{
	if(m_AddRate.m_nType == 0)
	{
		int dDia = (int)m_AddRate.m_dBoundaryDia;
		m_comboDia.SetCurSel(GetDiaIdx(dDia));
	}

	m_nCurSelDia = m_comboDia.GetCurSel(); 
}


void CRebarAddRateDlg::SetStringArrDia()
{
	m_strArrDia.RemoveAll();

	CString szFy;
	szFy = GetCharRebarMark(m_AddRate.m_dFy);
	
	m_strArrDia.Add(szFy+"10");
	m_strArrDia.Add(szFy+"13");
	m_strArrDia.Add(szFy+"16");
	m_strArrDia.Add(szFy+"19");
	m_strArrDia.Add(szFy+"22");
	m_strArrDia.Add(szFy+"25");
	m_strArrDia.Add(szFy+"29");
	m_strArrDia.Add(szFy+"32");
	m_strArrDia.Add(szFy+"35");
}


int CRebarAddRateDlg::GetDiaIdx(double dDia)
{
	int nCntStr = m_strArrDia.GetSize();
	double dTemp = 0;
	CString strDia;

	for(int i = 0; i < nCntStr ; i++)
	{
		strDia = m_strArrDia.GetAt(i);
		dTemp = atoi((char*)strDia.Right(2).operator LPCTSTR());
		if(dTemp == dDia)
			return i;
	}

	return 0;
}

void CRebarAddRateDlg::SetAddRate(CAddRate AddRate)
{
	m_AddRate = AddRate;
}

CAddRate CRebarAddRateDlg::GetAddRate()
{
	return m_AddRate;
}
