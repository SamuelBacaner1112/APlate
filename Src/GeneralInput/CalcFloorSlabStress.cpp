// CalcFloorSlabStress.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcFloorSlabStress dialog


CCalcFloorSlabStress::CCalcFloorSlabStress(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcFloorSlabStress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcFloorSlabStress)
	//}}AFX_DATA_INIT
}


void CCalcFloorSlabStress::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcFloorSlabStress)
	DDX_Control(pDX, IDC_RICHEDIT_SLAB_STRESS, m_richeditSlabStress);
	DDX_Control(pDX, IDC_TAB_FLOORSLAB_STRESS, m_tabFloorSlab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcFloorSlabStress, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcFloorSlabStress)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FLOORSLAB_STRESS, OnSelchangeTabFloorslabStress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcFloorSlabStress message handlers

void CCalcFloorSlabStress::OnPreInitDialog()
{
	m_tabFloorSlab.InsertItem(0,"좌측 캔틸레버부");
	m_tabFloorSlab.InsertItem(1,"우측 캔틸레버부");
	m_tabFloorSlab.InsertItem(2,"중앙부 바닥판");
	m_tabFloorSlab.SetCurSel(0);

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);
	SetResize(IDC_TAB_FLOORSLAB_STRESS, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_RICHEDIT_SLAB_STRESS, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CCalcFloorSlabStress::SetDataSave()
{

}

void CCalcFloorSlabStress::SetDataInit()
{
	CPlateStressOut	pStressOut(m_pStd);
 
	long	nSide	= m_tabFloorSlab.GetCurSel();

	m_richeditSlabStress.Clear();
	m_richeditSlabStress.SetSel(0, -1);
	m_richeditSlabStress.ReplaceSel("");
	m_richeditSlabStress.SetReadOnly(TRUE);
	pStressOut.SlabPrestresscheck(nSide, m_richeditSlabStress);
	m_richeditSlabStress.LineScroll(-m_richeditSlabStress.GetFirstVisibleLine());
}

void CCalcFloorSlabStress::SetDataDefault()
{

}

void CCalcFloorSlabStress::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcFloorSlabStress::OnSelchangeTabFloorslabStress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataInit();
}


BOOL CCalcFloorSlabStress::IsValid()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if(pBridge->m_nSlabTensionSize==0)	// 소수주형
		return FALSE;
	else
		return TRUE;
}
