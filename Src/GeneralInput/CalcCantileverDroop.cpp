// CalcCantileverDroop.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverDroop dialog


CCalcCantileverDroop::CCalcCantileverDroop(CWnd* pParent /*=NULL*/)
: CGeneralBaseDlg(CCalcCantileverDroop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverDroop)
	//}}AFX_DATA_INIT
}

void CCalcCantileverDroop::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverDroop)
	DDX_Control(pDX, IDC_RICHEDIT_DROOP, m_richeditDroop);
	DDX_Control(pDX, IDC_TAB_DROOP, m_TabDroop);
	//}}AFX_DATA_MAP
}


//BEGIN_MESSAGE_MAP(CCalcCantileverDroop, CGeneralBaseDlg)
BEGIN_MESSAGE_MAP(CCalcCantileverDroop, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverDroop)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DROOP, OnSelchangeTabDroop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverDroop message handlers
void CCalcCantileverDroop::OnPreInitDialog()
{
	CGlobarOption	*pOpt	= m_pStd->m_pDataManage->GetGlobalOption();
	CCalcGeneral	CalcGeneral(m_pStd->m_pDataManage);

	m_TabDroop.InsertItem(0, "ÁÂÃø ÄµÆ¿·¹¹öºÎ");
	m_TabDroop.InsertItem(1, "¿ìÃø ÄµÆ¿·¹¹öºÎ");

	if(pOpt->GetSlabCentDesignMethod() == 0)
	{
		m_TabDroop.InsertItem(2, "Áß¾ÓºÎ ¹Ù´ÚÆÇ");
	}
	else if(pOpt->GetSlabCentDesignMethod() == 1)
	{
		if(!CalcGeneral.CheckExperienceDesign(TRUE) || !CalcGeneral.CheckExperienceDesign(FALSE))
			m_TabDroop.InsertItem(2, "Áß¾ÓºÎ ¹Ù´ÚÆÇ");
	}

	m_TabDroop.SetCurSel(0);
	m_richeditDroop.SetReadOnly(TRUE);
}

BOOL CCalcCantileverDroop::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_TAB_DROOP, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_RICHEDIT_DROOP, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	return TRUE;
}

void CCalcCantileverDroop::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcCantileverDroop::SetDataInit()
{
	CPlateStressOut	StressOut(m_pStd);

	m_richeditDroop.SetSel(0, -1);
	m_richeditDroop.ReplaceSel("");

	long	nSide	= m_TabDroop.GetCurSel();

	m_richeditDroop.AddText("\n");
	StressOut.Output_DroopReview(nSide, m_richeditDroop);
	m_richeditDroop.AddText("\n\n");
	StressOut.Output_CrackReview(nSide, m_richeditDroop);
	m_richeditDroop.AddText("\n\n");
	m_richeditDroop.LineScroll(-m_richeditDroop.GetFirstVisibleLine());
}


void CCalcCantileverDroop::OnSelchangeTabDroop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	SetDataInit();
}


void CCalcCantileverDroop::SetDataDefault()
{

}

BOOL CCalcCantileverDroop::IsValid()
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	BOOL bValid = pDB->m_bTendonInputComplete;
	if(pDB->m_nSlabTensionSize==0 || pDB->IsPlateGir())
	{
		if(!bValid)
		{
			m_bUsedFlag = FALSE;
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		m_bUsedFlag = FALSE;
		return FALSE;
	}
}
