// CalcSecStressPos.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcSecStressPos.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcSecStressPos dialog


CCalcSecStressPos::CCalcSecStressPos(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: CDialog(CCalcSecStressPos::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcSecStressPos)
	m_dCheckPose = 0.0;	
	//}}AFX_DATA_INIT
	m_pStd = pStd;
	m_dLength = 0;
	m_sGirSta = " ";
	m_sElementInfo = " ";
	m_sNodeInfo = " ";
}


void CCalcSecStressPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcSecStressPos)
	DDX_Text(pDX, IDC_EDIT_CHECKPOSITION, m_dCheckPose);
	DDV_MinMaxDouble(pDX, m_dCheckPose, 0., 1.);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcSecStressPos, CDialog)
	//{{AFX_MSG_MAP(CCalcSecStressPos)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcSecStressPos message handlers

BOOL CCalcSecStressPos::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_ELEMENT, m_sElementInfo);
	SetDlgItemText(IDC_NODE, m_sNodeInfo);
	
	CString str;
	str.Format("%.3f", m_dCheckPose);
	SetDlgItemText(IDC_EDIT_CHECKPOSITION,str);
	SetDlgItemText(IDC_NGIR,m_sGirSta);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalcSecStressPos::OnOK() 
{	
	CString str;	
	GetDlgItemText(IDC_EDIT_CHECKPOSITION, str);
	m_dCheckPose = atof(str);
	
	m_dCheckLength = m_dLength * m_dCheckPose;

	CDialog::OnOK();
}

void CCalcSecStressPos::InitDlgData(long nG, double dSta)
{
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFEMManage      *pManage = m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	double dRatio = 0;
	long   nElm           = ModelCalc.GetElemNoAndRatio(nG, dSta, dRatio);
	double dLengthElm     = ModelCalc.GetElemLength(nElm, FALSE);
	double dLengthFromStt = dRatio*dLengthElm;
	
	long nNodeI	  = pManage->GetElement((unsigned short)nElm)->m_nJStt;
	long nNodeJ	  = pManage->GetElement((unsigned short)nElm)->m_nJEnd;

	m_dLength    = dLengthElm;
	m_dCheckPose = dLengthFromStt/dLengthElm;
	m_sGirSta.Format(" %d ", nG+1);
	m_sElementInfo.Format(" %d     (부재 길이 = %.3f m)", nElm+1, m_dLength);
	m_sNodeInfo.Format   (" I_NODE = %d,    J_NODE = %d ", nNodeI+1, nNodeJ+1);
}

