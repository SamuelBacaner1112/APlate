// CalcLossOfPrestress.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcLossOfPrestress.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcLossOfPrestress dialog


CCalcLossOfPrestress::CCalcLossOfPrestress(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcLossOfPrestress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcLossOfPrestress)
	m_dCTC		= 0.0;
	m_nQtyPs	= 0;
	//}}AFX_DATA_INIT
}


void CCalcLossOfPrestress::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcLossOfPrestress)
	DDX_Control(pDX, IDC_RICHEDIT_LOSS_OF_PRESTRESS, m_richeditPrestress);
	DDX_Control(pDX, IDC_TAB_PRESTRESS, m_tabPrestress);
	DDX_Text(pDX, IDC_EDIT_CTC, m_dCTC);
	DDX_Text(pDX, IDC_EDIT_QTY_PS, m_nQtyPs);
	DDX_Control(pDX, IDC_EDIT_CTC, m_ctrlEdit1);
	DDX_Control(pDX, IDC_EDIT_QTY_PS, m_ctrlEdit2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcLossOfPrestress, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcLossOfPrestress)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PRESTRESS, OnSelchangeTabPrestress)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcLossOfPrestress message handlers

BOOL CCalcLossOfPrestress::OnInitDialog() 
{
	m_nIndex = 0;

	CGeneralBaseDlg::OnInitDialog();

	m_tabPrestress.InsertItem(0,"ÁÂÃø ÄµÆ¿·¹¹öºÎ");
	m_tabPrestress.InsertItem(1,"¿ìÃø ÄµÆ¿·¹¹öºÎ");
	m_tabPrestress.InsertItem(2,"Áß¾ÓºÎ ¹Ù´ÚÆÇ");


	SetResize(IDC_TAB_PRESTRESS, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_RICHEDIT_LOSS_OF_PRESTRESS, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_GROUP1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_CTC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC18, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_QTY_PS, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC19, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	return TRUE;
}

void CCalcLossOfPrestress::SetDataInit()
{
	CDataManage	*pDataManage	= m_pStd->GetDataManage();
	CCalcData	*pCalcData		= pDataManage->GetCalcData();
	CPlateStressOut	StressOut(m_pStd);
	
	m_dCTC		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	m_nQtyPs	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	
	m_richeditPrestress.SetSel(0, -1);
	m_richeditPrestress.ReplaceSel("");
	m_richeditPrestress.SetReadOnly(TRUE);
	StressOut.SlabSectionPrestress(m_nIndex, m_richeditPrestress);
	m_richeditPrestress.LineScroll(-m_richeditPrestress.GetFirstVisibleLine());
	UpdateData(FALSE);
}

void CCalcLossOfPrestress::SetDataSave()
{
	UpdateData(TRUE);
	CDataManage	*pDataManage	= m_pStd->GetDataManage();
	CCalcData	*pCalcData		= pDataManage->GetCalcData();

	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC = m_dCTC;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty = m_nQtyPs;

	pDataManage->GetBridge()->SetCalcJongTendon();
}

void CCalcLossOfPrestress::SetDataDefault()
{
	CDataManage	*pDataManage	= m_pStd->GetDataManage();
	CCalcData	*pCalcData		= pDataManage->GetCalcData();

	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC = 500.0;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty = 4;

	UpdateData(FALSE);
}

BOOL CCalcLossOfPrestress::IsValid() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if(pBridge->m_nSlabTensionSize==0)	// ¼Ò¼öÁÖÇü
		return FALSE;
	else
		return TRUE;
}

void CCalcLossOfPrestress::OnSelchangeTabPrestress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	CDataManage	*pDataManage	= m_pStd->GetDataManage();
	CCalcData	*pCalcData		= pDataManage->GetCalcData();

	m_nIndex = m_tabPrestress.GetCurSel();

	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC = m_dCTC;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty = m_nQtyPs;
	SetDataInit();
	SetDataSave();
}

void CCalcLossOfPrestress::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}
