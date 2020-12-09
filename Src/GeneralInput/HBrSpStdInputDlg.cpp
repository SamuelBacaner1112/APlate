// HBrSpStdInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHBrSpStdInputDlg dialog


CHBrSpStdInputDlg::CHBrSpStdInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHBrSpStdInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHBrSpStdInputDlg)
	m_sBL = _T("");
	m_sGw = _T("");
	m_sSA = _T("");
	m_sSB = _T("");
	m_sSC = _T("");
	m_sSD = _T("");
	m_sSE = _T("");
	m_sStep1 = _T("");
	m_sStep2 = _T("");
	m_sStep3 = _T("");
	m_sStep4 = _T("");
	m_sStep5 = _T("");
	m_sW = _T("");
	m_sW1 = _T("");
	//}}AFX_DATA_INIT
}


void CHBrSpStdInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHBrSpStdInputDlg)
	DDX_Control(pDX, IDC_COMBO_BD, m_comboBoltDia);
	DDX_Text(pDX, IDC_EDIT_BL, m_sBL);
	DDX_Text(pDX, IDC_EDIT_GW, m_sGw);
	DDX_Text(pDX, IDC_EDIT_SA, m_sSA);
	DDX_Text(pDX, IDC_EDIT_SB, m_sSB);
	DDX_Text(pDX, IDC_EDIT_SC, m_sSC);
	DDX_Text(pDX, IDC_EDIT_SD, m_sSD);
	DDX_Text(pDX, IDC_EDIT_SE, m_sSE);
	DDX_Text(pDX, IDC_EDIT_STEP1, m_sStep1);
	DDX_Text(pDX, IDC_EDIT_STEP2, m_sStep2);
	DDX_Text(pDX, IDC_EDIT_STEP3, m_sStep3);
	DDX_Text(pDX, IDC_EDIT_STEP4, m_sStep4);
	DDX_Text(pDX, IDC_EDIT_STEP5, m_sStep5);
	DDX_Text(pDX, IDC_EDIT_W, m_sW);
	DDX_Text(pDX, IDC_EDIT_W1, m_sW1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHBrSpStdInputDlg, CDialog)
	//{{AFX_MSG_MAP(CHBrSpStdInputDlg)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_CBN_SELCHANGE(IDC_COMBO_BD, OnSelchangeComboDia)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHBrSpStdInputDlg message handlers
BOOL CHBrSpStdInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CPlateBridgeApp	*pDB  = m_pStd->GetBridge();
	CHBracingGusset *pBrGu = pDB->GetGirder(0)->GetHBracingGussetInTotal(0);
	CSection        *pSecBr= pBrGu->GetBxBracing()->GetSection();
	CBracing		*pBracing	= pSecBr->GetBx()->GetBracing();
	long        nSteelType = pBracing->GetSteelMaterialData()->m_nType;//pMat->GetSteelType(pBrGu->GetBxBracing(), CSteelMaterial::BUJE_LHBEAM);
	// L형강, 부등변L형강, ㄷ형강일때 볼트열 간격수
	if(nSteelType==CSteelMaterialData::LSTEEL || nSteelType==CSteelMaterialData::LVALSTEEL)
		pDB->m_dHBuSE  = 0;	

	SetDataInit();

	return TRUE;
}

void CHBrSpStdInputDlg::OnButtonRecommend() 
{
	SetDataDefault();
	SetDataInit();
}


void CHBrSpStdInputDlg::SetDataDefault()
{
	CPlateBridgeApp	*pDB  = m_pStd->GetBridge();

	pDB->m_dHBuSL = 80;
	pDB->m_dHBuGw = 20;
	pDB->m_dHBuSA = 40; 
	pDB->m_dHBuSB = 2;
	pDB->m_dHBuSC = 80;
	pDB->m_dHBuSD = 40;
	pDB->m_dHBuSE = 1;
	pDB->m_dHBuW  = 350;
	pDB->m_dHBuW1 = 100;
	pDB->m_szHBrSpBoltDia = "M22";

	CHBracingGusset *pBrGu = pDB->GetGirder(0)->GetHBracingGussetInTotal(0);
	CSection        *pSecBr= pBrGu->GetBxBracing()->GetSection();
	CBracing		*pBracing	= pSecBr->GetBx()->GetBracing();
	long        nSteelType = pBracing->GetSteelMaterialData()->m_nType;//pMat->GetSteelType(pBrGu->GetBxBracing(), CSteelMaterial::BUJE_LHBEAM);
	// L형강, 부등변L형강, ㄷ형강일때 볼트열 간격수
	if(nSteelType==CSteelMaterialData::LSTEEL || nSteelType==CSteelMaterialData::LVALSTEEL)
		pDB->m_dHBuSE  = 0;	

	pDB->m_dHBuStep1 = 50;
	pDB->m_dHBuStep2 = 20;
	pDB->m_dHBuStep3 = 20;
	pDB->m_dHBuStep4 = 20;
	pDB->m_dHBuStep5 = 120;
}


void CHBrSpStdInputDlg::SetDataInit()
{
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();

	m_sBL.Format("%.0lf", pDB->m_dHBuSL);
	m_sGw.Format("%.0lf", pDB->m_dHBuGw);
	m_sSA.Format("%.0lf", pDB->m_dHBuSA);
	m_sSB.Format("%.0lf", pDB->m_dHBuSB);
	m_sSC.Format("%.0lf", pDB->m_dHBuSC);
	m_sSD.Format("%.0lf", pDB->m_dHBuSD);
	m_sSE.Format("%.0lf", pDB->m_dHBuSE);
	m_sStep1.Format("%.0lf", pDB->m_dHBuStep1);
	m_sStep2.Format("%.0lf", pDB->m_dHBuStep2);
	m_sStep3.Format("%.0lf", pDB->m_dHBuStep3);
	m_sStep4.Format("%.0lf", pDB->m_dHBuStep4);
	m_sStep5.Format("%.0lf", pDB->m_dHBuStep5);
	m_sW.Format("%.0lf", pDB->m_dHBuW);	
	m_sW1.Format("%.0lf", pDB->m_dHBuW1);	

	CString szCombo;
	long n = 0;
	for (n=0; n<m_comboBoltDia.GetCount(); n++)
	{
		m_comboBoltDia.GetLBText(n, szCombo);
		if(szCombo==pDB->m_szHBrSpBoltDia)
			break;
	}
	m_comboBoltDia.SetCurSel(n);

	UpdateData(FALSE);
}


void CHBrSpStdInputDlg::SetDataSave()
{
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();

	pDB->m_dHBuSL = atof(m_sBL);
	pDB->m_dHBuGw = atof(m_sGw);
	pDB->m_dHBuSA = atof(m_sSA); 
	pDB->m_dHBuSB = atof(m_sSB);
	pDB->m_dHBuSC = atof(m_sSC);
	pDB->m_dHBuSD = atof(m_sSD);
	pDB->m_dHBuSE = atof(m_sSE);
	pDB->m_dHBuStep1 = atof(m_sStep1);
	pDB->m_dHBuStep2 = atof(m_sStep2);
	pDB->m_dHBuStep3 = atof(m_sStep3);
	pDB->m_dHBuStep4 = atof(m_sStep4);
	pDB->m_dHBuStep5 = atof(m_sStep5);
	pDB->m_dHBuW	  = atof(m_sW);	
	pDB->m_dHBuW1	  = atof(m_sW1);	

	long nID = m_comboBoltDia.GetCurSel();
	CString szCombo;
	m_comboBoltDia.GetLBText(nID, pDB->m_szHBrSpBoltDia);

}

void CHBrSpStdInputDlg::OnOK() 
{
	UpdateData(TRUE);
	SetDataSave();
	
	CDialog::OnOK();
}

void CHBrSpStdInputDlg::OnSelchangeComboDia() 
{
	// TODO: Add your control notification handler code here
	
}
