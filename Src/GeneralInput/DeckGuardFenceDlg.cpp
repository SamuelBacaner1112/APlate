// DeckGuardFenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "GuardFenceTypeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckGuardFenceDlg dialog


CDeckGuardFenceDlg::CDeckGuardFenceDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckGuardFenceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckGuardFenceDlg)
	m_bChkIsRebar = FALSE;
	m_bPlusToSlabRebar = FALSE;
	//}}AFX_DATA_INIT
	m_nTab = 0;
	m_nGuard = 0;
}


void CDeckGuardFenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckGuardFenceDlg)
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_COMBO_GUARDWALL, m_comboGuardFence);
	DDX_Control(pDX, IDC_COMBO3, m_comboThird);
	DDX_Control(pDX, IDC_COMBO2, m_comboSecond);
	DDX_Control(pDX, IDC_COMBO1, m_comboFirst);
	DDX_Check(pDX, IDC_CHECK_ISREBAR, m_bChkIsRebar);
	DDX_Control(pDX, IDC_EDIT_COVER, m_dCover);
	DDX_Control(pDX, IDC_EDIT_FY, m_dFy);
	DDX_Check(pDX, IDC_CHECK_PLUSTOSLAB, m_bPlusToSlabRebar);
	DDX_Control(pDX, IDC_EDIT_FCK, m_dFck);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_FIRST, m_GridFirst);
	DDX_GridControl(pDX, IDC_GRID_SECOND, m_GridSecond);
	DDX_Control(pDX, IDC_BUTTON_BARPLACE_TYPE, m_btnShear);
}


BEGIN_MESSAGE_MAP(CDeckGuardFenceDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckGuardFenceDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO_GUARDWALL, OnSelchangeComboGuardwall)
	ON_BN_CLICKED(IDC_CHECK_ISREBAR, OnCheckIsrebar)
	ON_BN_CLICKED(IDC_BUTTON_BARPLACE_TYPE, OnButtonBarplaceType)
	ON_BN_CLICKED(IDC_CHECK_PLUSTOSLAB, OnCheckPlusToSlabRebar)
	ON_EN_KILLFOCUS(IDC_EDIT_COVER, OnKillfocusEditCover)
	ON_EN_KILLFOCUS(IDC_EDIT_FY, OnKillfocusEditFy)
	ON_EN_KILLFOCUS(IDC_EDIT_FCK, OnKillfocusEditFck)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTabRebar)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_FIRST, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_SECOND, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_FIRST, OnCellChangedFirstGrid)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_SECOND, OnCellChangedSecondGrid)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckGuardFenceDlg message handlers

void CDeckGuardFenceDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	if(IsUsedFlag())
	{
		CCentSeparation* pCS = pDB->GetGuardFence(m_nGuard);
		CGuardFenceRebar* pGFRebar = pCS->GetRebar();
		long nHDan = pDB->GetNumHDanByGuardFence(pCS);
		CDPoint ptDot[9];
		pDB->GetXyGuardFenceDom(pBx, nHDan, ptDot);
		CDPointArray ptArrConc = ConvertGuardFenceData(ptDot);
		int nType = GetBarPlacingType(ptArrConc);
		pGFRebar->SetTypePlacingRebar(nType);
		pGFRebar->SetConcPosition(ptArrConc);
		pGFRebar->SetDataDefault(TRUE);
	}
	else
	{
		int nSize = pDB->GetGuardFencesu();
		for(int i = 0; i < nSize ; i++)
		{
			CCentSeparation* pCS = pDB->GetGuardFence(i);
			CGuardFenceRebar* pGFRebar = pCS->GetRebar();
			long nHDan = pDB->GetNumHDanByGuardFence(pCS);
			CDPoint ptDot[9];

			pDB->GetXyGuardFenceDom(pBx, nHDan, ptDot);
			CDPointArray ptArrConc = ConvertGuardFenceData(ptDot);
			int nType = GetBarPlacingType(ptArrConc);
			pGFRebar->SetTypePlacingRebar(nType);
			pGFRebar->SetConcPosition(ptArrConc);
			pGFRebar->SetDataDefault(TRUE);
		}
	}


	ChangeViewControl();
}

void CDeckGuardFenceDlg::SetDataInit()
{
	CString				sText		= _T("");
	CADeckData			*pDeckData	= m_pStd->m_pDeckData; 
	CPlateBridgeApp		*pDB		= m_pStd->m_pDataManage->GetBridge();	
	CCentSeparation		*pCS		= pDB->GetGuardFence(m_nGuard);
	CGuardFenceRebar	*pGFBar		= pCS->GetRebar();

	SetDlgItemText(IDC_STATIC_FY_UNIT, "MPa");
	SetDlgItemText(IDC_STATIC_FCK_UNIT, "MPa");


	sText.Format("%g",	pGFBar->GetFy()/10);
	
	m_dFy.SetWindowText(sText);

	sText.Format("%g",	pGFBar->GetFck()/10);
	
	m_dFck.SetWindowText(sText);

	SetGridTitle();

	//피복은 공통이다.

	sText.Format("%g", pGFBar->GetCover());

	m_dCover.SetWindowText(sText);

	pDeckData->m_bPlusToSlabRebar = m_bPlusToSlabRebar;

	if(m_nTab == 0)
		SetDataInitMainRebar();
	else if(m_nTab == 1)
		SetDataInitDistRebar();
	else if(m_nTab == 2)
		SetDataInitShearRebar();

	UpdateData(FALSE);
}

void CDeckGuardFenceDlg::SetDataSave()
{
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	if(m_nTab == 0)
		SetDataSaveMainRebar();
	else if(m_nTab == 1)
		SetDataSaveDistRebar();
	else if(m_nTab == 2)
		SetDataSaveShearRebar();

	m_bPlusToSlabRebar = pDeckData->m_bPlusToSlabRebar;

	if(pDeckData->m_bRecalcJewon && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag())
		pDeckData->MakeRebarJewon();
}

void CDeckGuardFenceDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();
	CDrawGuardFenceRebar DrawGFBar(m_pStd->m_pDataManage);
	CAPlateDrawDanmyun DrawDan(m_pStd->m_pDataManage);

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	double Scale = 10;
	pDom->SetScaleDim(Scale);
	CDomyun Dom(pDom);

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	CCentSeparation *pGuard = pDB->GetGuardFence(m_nGuard);
	CGuardFenceRebar* pGFBar = pGuard->GetRebar();
	if(!pGuard) return;
	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);
	//Draw Conc
	DrawDan.DstDrawGuardFence(&Dom, pBx, nHDan);
	//Draw Rebar
	DrawGFBar.DrawRebar(&Dom, m_nGuard);	
	DrawGFBar.DrawGroundLine(&Dom, m_nGuard);

	if(m_nTab == 0)  //주철근 
	{
		DrawGFBar.DimMainRebar(&Dom, m_nGuard);	
	}
	else if(m_nTab == 1)  //배력철근 
	{
		DrawGFBar.DimDistRebar(&Dom, m_nGuard);	
	}
	else if(m_nTab == 2)  //전단철근 
	{
		DrawGFBar.DimShearRebar(&Dom, m_nGuard);	
	}

	DrawGFBar.MarkMainRebar(&Dom, m_nGuard);	
	DrawGFBar.MarkDistRebar(&Dom, m_nGuard);	
	DrawGFBar.MarkShearRebar(&Dom, m_nGuard);


	*pDom << Dom;
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDeckGuardFenceDlg::OnSelchangeCombo1() 
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	if(m_nTab == 0)  //주철근 
	{
		//주철근 CTC를 뜻한다.
		CString strCTC;
		int nIdx = m_comboFirst.GetCurSel();
		m_comboFirst.GetLBText(nIdx, strCTC);
		double dCTC = atoi(strCTC);
		pGFBar->SetMainRebarCTC(dCTC);
		DrawInputDomyunView(TRUE);
	}
	else if(m_nTab == 1) //배력철근 
	{
		//전면 구간수 
		int nCnt = m_comboFirst.GetCurSel() + 1;
		//기존의 구간수와 비교 
		CDPointArray ptArrDim = pGFBar->GetDistBarDimInfo(0);
		int nSize = ptArrDim.GetSize();
		if(nSize < nCnt)  //기존보다 크다면 
		{
			int nSpace = nCnt - nSize;
			ptArrDim = AddDimInfo(ptArrDim, nSpace);
		}
		else if(nSize > nCnt)  //기존보다 작다면.
		{
			int nSpace = nSize - nCnt;
			ptArrDim = DeleteDimInfo(ptArrDim, nSpace);
		}
		else if(nSize == nCnt)
			return;

		pGFBar->SetDistBarDimInfo(0, ptArrDim);
		SetDataInit();
		DrawInputDomyunView(TRUE);
	}
	else if(m_nTab == 2) //전단철근 
	{
		//전단철근 구간수 
		int nCnt = m_comboFirst.GetCurSel() + 1;
		//기존의 구간수와 비교 
		CDPointArray ptArrDim = pGFBar->GetShearBarDimInfo();
		int nSize = ptArrDim.GetSize();
		if(nSize < nCnt)  //기존보다 크다면 
		{
			int nSpace = nCnt - nSize;
			ptArrDim = AddDimInfo(ptArrDim, nSpace);
		}
		else if(nSize > nCnt)  //기존보다 작다면.
		{
			int nSpace = nSize - nCnt;
			ptArrDim = DeleteDimInfo(ptArrDim, nSpace);
		}
		else if(nSize == nCnt)
			return;

		pGFBar->SetShearBarDimInfo(ptArrDim);
		SetDataInit();
		DrawInputDomyunView(TRUE);
	}
}

void CDeckGuardFenceDlg::OnSelchangeCombo2() 
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	if(m_nTab == 1)  //배력철근 
	{
		//후면 구간수 
		int nCnt = m_comboSecond.GetCurSel() + 1;
		//기존의 구간수와 비교 
		CDPointArray ptArrDim = pGFBar->GetDistBarDimInfo(1);
		int nSize = ptArrDim.GetSize();
		if(nSize < nCnt)  //기존보다 크다면 
		{
			int nSpace = nCnt - nSize;
			ptArrDim = AddDimInfo(ptArrDim, nSpace);
		}
		else if(nSize > nCnt)  //기존보다 작다면.
		{
			int nSpace = nSize - nCnt;
			ptArrDim = DeleteDimInfo(ptArrDim, nSpace);
		}
		else if(nSize == nCnt)
			return;

		pGFBar->SetDistBarDimInfo(1, ptArrDim);
		SetDataInit();
		DrawInputDomyunView(TRUE);
	}
	else if(m_nTab == 2)  //전단철근 
	{
		//철근 직경 
		int nIdx = m_comboSecond.GetCurSel();
		double dDia = GetDiaByIdx(nIdx);
		pGFBar->SetShearRebarDia(dDia);
		DrawInputDomyunView(TRUE);
	}
}

void CDeckGuardFenceDlg::OnSelchangeCombo3() 
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	if(m_nTab == 1)  //배력철근 
	{
		//철근직경 
		int    nIdx = m_comboThird.GetCurSel();
		double dDia = GetDiaByIdx(nIdx);
		pGFBar->SetDistRebarDia(dDia);
		DrawInputDomyunView(TRUE);
	}
}

void CDeckGuardFenceDlg::OnSelchangeComboGuardwall() 
{
	// TODO: Add your control notification handler code here
	m_nGuard = m_comboGuardFence.GetCurSel();
	SetDataInit();
	ChangeViewControl();
	DrawInputDomyunView(TRUE);
}

void CDeckGuardFenceDlg::OnCheckIsrebar() 
{
	// TODO: Add your control notification handler code here
	
}

void CDeckGuardFenceDlg::OnButtonBarplaceType() 
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();
	CCentSeparation *pGuard = pDB->GetGuardFence(m_nGuard);
	CGuardFenceRebar* pGFBar = pGuard->GetRebar();

	int nType = pGFBar->GetTypePlacingRebar();
	CGuardFenceTypeDlg dlg;
	dlg.SetType(nType);

	if(dlg.DoModal() == IDOK)
	{
		nType = dlg.GetType();
		pGFBar->SetTypePlacingRebar(nType);
		pGFBar->SetDataDefault(TRUE);
		SetDataInitRebarDia();
		SetDataInit();
		DrawInputDomyunView(TRUE);
		ChangeViewControl();
	}
}

void CDeckGuardFenceDlg::OnPreInitDialog()
{
	SetDataInitTab();
	//데이터 설정
	SetDataInitRebarDia();
	SetDataInitRebarPart();
	SetDataInitRebarCTC();

	SetDataInitComboGuardFence();
	SetDataInitCombo();
	ChangeViewControl();
}

void CDeckGuardFenceDlg::SetDataInitCombo()
{
	if(m_nTab == 0)  //주철근 
		SetDataInitComboMainRebar();
	else if(m_nTab == 1)  //배력철근 
		SetDataInitComboDistRebar();
	else if(m_nTab == 2)  //전단철근 
		SetDataInitComboShearRebar();

	UpdateData(FALSE);
}

void CDeckGuardFenceDlg::SetDataInitComboMainRebar()
{
	m_comboFirst.ResetContent();
	m_comboSecond.ResetContent();
	m_comboThird.ResetContent();

	for(int i = 0; i <(long) m_vecCTC.size(); i++)
	{
		double dCTC = m_vecCTC[i];
		CString str;
		str.Format("%0.0f", dCTC);
		m_comboFirst.AddString(str);		
	}

	m_comboFirst.SetCurSel(0);
}

void CDeckGuardFenceDlg::SetDataInitComboDistRebar()
{
	m_comboFirst.ResetContent();
	m_comboSecond.ResetContent();
	m_comboThird.ResetContent();

	int i = 0;
	//좌,우측 구간수 
	for(i = 0; i < (long)m_vecPart.size(); i++)
	{
		CString str;
		str.Format("%d", i+1);
		m_comboFirst.AddString(str);		
		m_comboSecond.AddString(str);		
	}

	int nSize = m_vecDia.size();
	m_comboFirst.SetCurSel(0);		
	m_comboSecond.SetCurSel(0);		

	//철근 직경 
	for(i = 0; i < nSize; i++)
	{
		CString str = m_vecDia[i];
		m_comboThird.AddString(str);		
	}

	m_comboThird.SetCurSel(0);
}

void CDeckGuardFenceDlg::SetDataInitComboShearRebar()
{
	m_comboFirst.ResetContent();
	m_comboSecond.ResetContent();
	m_comboThird.ResetContent();
	int i = 0;
	for(i = 0; i < (long)m_vecPart.size(); i++)
	{
		CString str;
		str.Format("%d", i + 1);
		m_comboFirst.AddString(str);		
	}

	for(i = 0; i < (long)m_vecDia.size(); i++)
	{
		CString str = m_vecDia[i];
		m_comboSecond.AddString(str);				
	}

	m_comboFirst.SetCurSel(0);
	m_comboSecond.SetCurSel(0);
}

void CDeckGuardFenceDlg::SetDataInitComboGuardFence()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CString str;

	int nSize = pDB->GetGuardFencesu();
	m_comboGuardFence.ResetContent();
	for(int i = 0; i < nSize; i++)
	{
		str.Format("방호벽 %d", i+1);
		m_comboGuardFence.AddString(str);
	}

	if(m_nGuard < 0)
		m_nGuard = 0;
	m_comboGuardFence.SetCurSel(m_nGuard);
}

void CDeckGuardFenceDlg::ChangeViewControl()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	int nType = pGFBar->GetTypePlacingRebar();

	if(m_nTab == 0) //주철근 
	{
		GetDlgItem(IDC_STATIC_FIRST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_THIRD)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC_FIRST, "주철근CTC");
		GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO3)->ShowWindow(SW_HIDE);
		m_GridFirst.ShowWindow(SW_SHOW);
		m_GridSecond.ShowWindow(SW_SHOW);
	}
	else if(m_nTab == 1)  //배력철근 
	{
		if(nType == 2 || nType == 4)
		{
			GetDlgItem(IDC_STATIC_FIRST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_THIRD)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_STATIC_THIRD, "철근직경");
			SetDlgItemText(IDC_STATIC_FIRST, "측면구간수");
			SetDlgItemText(IDC_STATIC_SECOND, "상부구간수");
			GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_SHOW);
			m_GridSecond.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO3)->ShowWindow(SW_SHOW);
		}
		else if(nType == 3)
		{
			GetDlgItem(IDC_STATIC_FIRST)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_THIRD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO3)->ShowWindow(SW_HIDE);
			m_GridFirst.ShowWindow(SW_HIDE);
			m_GridSecond.ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_FIRST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_THIRD)->ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_STATIC_THIRD, "철근직경");
			SetDlgItemText(IDC_STATIC_FIRST, "전면구간수");
			SetDlgItemText(IDC_STATIC_SECOND, "후면구간수");
			GetDlgItem(IDC_COMBO2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_HIDE);
			m_GridSecond.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO3)->ShowWindow(SW_SHOW);
		}
	}
	else if(m_nTab == 2)  //전단철근 
	{
		if(nType == 3 || nType == 4)  //3,4 타입은 전단철근이 없다.
		{
			GetDlgItem(IDC_STATIC_FIRST)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_THIRD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO3)->ShowWindow(SW_HIDE);
			m_GridFirst.ShowWindow(SW_HIDE);
			m_GridSecond.ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_FIRST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SECOND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_THIRD)->ShowWindow(SW_HIDE);
			SetDlgItemText(IDC_STATIC_FIRST, "전단구간수");
			SetDlgItemText(IDC_STATIC_SECOND, "철근직경");
			GetDlgItem(IDC_COMBO2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO3)->ShowWindow(SW_HIDE);
			m_GridSecond.ShowWindow(SW_SHOW);
		}
	}
}

void CDeckGuardFenceDlg::SetDataInitTab()
{
	m_Tab.DeleteAllItems();
	
	int nTab = 0;
	m_Tab.InsertItem(nTab++, "주철근");
	m_Tab.InsertItem(nTab++, "배력철근");
	m_Tab.InsertItem(nTab++, "전단철근");
	m_Tab.SetCurSel(m_nTab);
}

void CDeckGuardFenceDlg::OnSelchangeTabRebar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_Tab.SetFocus();
	m_nTab = m_Tab.GetCurSel();
	ChangeViewControl();
	SetDataInitCombo();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CDeckGuardFenceDlg::SetGridTitle()
{
	if(m_nTab == 0)
		SetGridTitleMainRebar();
	else if(m_nTab == 1)
		SetGridTitleDistRebar();
	else if(m_nTab == 2)
		SetGridTitleShearRebar();
}

void CDeckGuardFenceDlg::SetGridTitleMainRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	int nType = pGFBar->GetTypePlacingRebar();
	int nCol = 0;
	CString str, strMark;
	strMark = "G";
	/************************************************************************/
	/* 철근직경                                                             */
	/************************************************************************/
	m_GridFirst.DeleteAllItems();
	m_GridFirst.EnableReturnPass();
	m_GridFirst.EnableInsertRow(FALSE);
	m_GridFirst.EnableInsertCol(TRUE);
	m_GridFirst.SetTextBkColor(RGBEDITABLE);	
	m_GridFirst.SetFixedColumnCount(1);	
	m_GridFirst.SetFixedRowCount(1);	
	m_GridFirst.SetRowCount(2);
	nCol = pGFBar->GetCntMainRebar();
	m_GridFirst.SetColumnCount(nCol+1);
	m_GridFirst.SetTextMatrix(0, 0, "구분");
	m_GridFirst.SetTextMatrix(1, 0, "직경");
	int i = 0;
	for(i = 0; i < nCol; i++)
	{
		str.Format("%s%d-%d", strMark, m_nGuard+1, i+1);
		m_GridFirst.SetTextMatrix(0, i+1, str);
	}
	for(i = 0; i < nCol; i++)
		m_GridFirst.SetCellType(1, i+1, CT_COMBO);

	/************************************************************************/
	/* 하부철근 길이                                                        */
	/************************************************************************/
	m_GridSecond.DeleteAllItems();
	m_GridSecond.EnableReturnPass();
	m_GridSecond.EnableInsertRow(FALSE);
	m_GridSecond.EnableInsertCol(TRUE);
	m_GridSecond.SetTextBkColor(RGBEDITABLE);
	m_GridSecond.SetFixedColumnCount(1);	
	m_GridSecond.SetFixedRowCount(1);	
	m_GridSecond.SetRowCount(2);
	if(nType != 3)
		nCol = pGFBar->GetCntRebarHeight() + 2;
	else
		nCol = pGFBar->GetCntRebarHeight() + 1;
	m_GridSecond.SetColumnCount(nCol);
	m_GridSecond.SetTextMatrix(0, 0, "구분");
	m_GridSecond.SetTextMatrix(1, 0, "길이");

	int nCnt = 1;
	if(nType != 3)
		m_GridSecond.SetTextMatrix(0, nCnt++, "LW");

	nCol = pGFBar->GetCntRebarHeight();
	for(i = 0; i < nCol; i++)
		m_GridSecond.SetTextMatrixFormat(0, nCnt++, "H%d", (long)(i+1));

}

void CDeckGuardFenceDlg::SetGridTitleDistRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	int nCol = 0;
	CString strType1, strType2, strType;
	int nType = pGFBar->GetTypePlacingRebar();
	if(nType == 1 || nType == 5 || nType == 6 || nType == 7)  //1번철근 이면..
	{
		strType1 = "B";  //후면
		strType2 = "";
	}
	else if(nType == 2)  //2번철근 이면..
	{
		strType1 = "F";  //전면 
		strType2 = "B";  //후면 
	}
	else if(nType == 4)  //4번철근 이면..
	{
		strType1 = "B";  //측면 
		strType2 = "U";  //상부 
	}

	/************************************************************************/
	/* 전면 / 측면 배력철근                                                 */
	/************************************************************************/
	nCol = pGFBar->GetDistBarDimInfo(0).GetSize();
	m_GridFirst.DeleteAllItems();
	m_GridFirst.EnableReturnPass();
	m_GridFirst.EnableInsertRow(FALSE);
	m_GridFirst.EnableInsertCol(TRUE);
	m_GridFirst.SetTextBkColor(RGBEDITABLE);	
	m_GridFirst.SetRowCount(3);
	m_GridFirst.SetColumnCount(nCol + 1);
	m_GridFirst.SetFixedColumnCount(1);	
	m_GridFirst.SetFixedRowCount(1);	
	m_GridFirst.SetTextMatrix(0, 0, "구분");
	m_GridFirst.SetTextMatrix(1, 0, "개수");
	m_GridFirst.SetTextMatrix(2, 0, "간격");
	int i = 0;
	for(i = 0; i < nCol; i++)
	{
		strType.Format("%s%d", strType1, (long)(i+1));
		m_GridFirst.SetTextMatrix(0, i+1, strType);
	}

	/************************************************************************/
	/* 후면 / 상부 배력철근                                                 */
	/************************************************************************/
	nCol = pGFBar->GetDistBarDimInfo(1).GetSize();
	m_GridSecond.DeleteAllItems();
	m_GridSecond.EnableReturnPass();
	m_GridSecond.EnableInsertRow(FALSE);
	m_GridSecond.EnableInsertCol(TRUE);
	m_GridSecond.SetTextBkColor(RGBEDITABLE);	
	m_GridSecond.SetRowCount(3);
	m_GridSecond.SetColumnCount(nCol + 1);
	m_GridSecond.SetFixedColumnCount(1);	
	m_GridSecond.SetFixedRowCount(1);	
	m_GridSecond.SetTextMatrix(0, 0, "구분");
	m_GridSecond.SetTextMatrix(1, 0, "개수");
	m_GridSecond.SetTextMatrix(2, 0, "간격");

	for(i = 0; i < nCol; i++)
	{
		strType.Format("%s%d", strType2, (long)(i+1));
		m_GridSecond.SetTextMatrix(0, i+1, strType);
	}
}

void CDeckGuardFenceDlg::SetGridTitleShearRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	int nCol = 0;

	/************************************************************************/
	/* 철근                                                                 */
	/************************************************************************/
	CDPointArray ptArrDim = pGFBar->GetShearBarDimInfo();
	nCol = ptArrDim.GetSize();
	m_GridFirst.DeleteAllItems();
	m_GridFirst.EnableReturnPass();
	m_GridFirst.EnableInsertRow(FALSE);
	m_GridFirst.EnableInsertCol(TRUE);
	m_GridFirst.SetTextBkColor(RGBEDITABLE);
	m_GridFirst.SetRowCount(3);
	m_GridFirst.SetColumnCount(nCol + 1);
	m_GridFirst.SetFixedColumnCount(1);	
	m_GridFirst.SetFixedRowCount(1);	
	m_GridFirst.SetTextMatrix(0, 0, "구분");
	m_GridFirst.SetTextMatrix(1, 0, "개수");
	m_GridFirst.SetTextMatrix(2, 0, "간격");
	for(int i = 0; i < nCol; i++)
		m_GridFirst.SetTextMatrixFormat(0, i+1, "S%d", (long)(i+1));

	/************************************************************************/
	/* 주철근 개수 / 전단철근 종류                                          */
	/************************************************************************/
	m_GridSecond.DeleteAllItems();
	m_GridSecond.EnableReturnPass();
	m_GridSecond.EnableInsertRow(FALSE);
	m_GridSecond.EnableInsertCol(TRUE);
	m_GridSecond.SetTextBkColor(RGBEDITABLE);
	m_GridSecond.SetRowCount(2);
	m_GridSecond.SetColumnCount(2);
	m_GridSecond.SetFixedRowCount(1);	
	m_GridSecond.SetTextMatrix(0, 0, "주철근간격 개수");
	m_GridSecond.SetTextMatrix(0, 1, "전단철근 종류");
	m_GridSecond.SetColumnWidth(0, 100);
	m_GridSecond.SetColumnWidth(1, 100);
	m_GridSecond.SetUserComboDialog(1, 1, &m_dlgShearBarType);
}

void CDeckGuardFenceDlg::SetDataInitMainRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	int nType = pGFBar->GetTypePlacingRebar();
	CString strDia;
	//1. 주철근 CTC
	double dCTC = pGFBar->GetMainRebarCTC();
	int nComboIdx = GetIdxCTC(dCTC);
	m_comboFirst.SetCurSel(nComboIdx);
	UpdateData(FALSE);

	/************************************************************************/
	/* 주철근 직경                                                          */
	/************************************************************************/
	int nCntBar = pGFBar->GetCntMainRebar();
	for(int i = 0; i < nCntBar; i++)
	{
		strDia = pGFBar->GetTextMainBarDia(i);
		m_GridFirst.SetTextMatrix(1, i+1, strDia);
	}
	
	/************************************************************************/
	/* 하부철근 길이                                                        */
	/************************************************************************/
	int nCol = 1;
	if(nType != 3)
	{
		double dWidth = pGFBar->GetWidthMainBarLower();
		m_GridSecond.SetTextMatrixFormat(1, nCol++, "%0.0f", dWidth);
	}

	int nCntHeight = pGFBar->GetCntRebarHeight();
	if(nCntHeight > 0)
	{
		double dHeight = pGFBar->GetHeightMainBarLower();
		m_GridSecond.SetTextMatrixFormat(1, nCol++, "%0.0f", dHeight);

		if(nCntHeight > 1)
		{
			dHeight = pGFBar->GetHeightMainBarUpper();
			m_GridSecond.SetTextMatrixFormat(1, nCol++, "%0.0f", dHeight);
		}
	}

}

//배력철근 입력창 데이터 
void CDeckGuardFenceDlg::SetDataInitDistRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	int nType = pGFBar->GetTypePlacingRebar();

	//1. 구간수 
	int nSize = pGFBar->GetDistBarDimInfo(0).GetSize();
	m_comboFirst.SetCurSel(nSize - 1);
	if(nType == 2)
	{
		nSize = pGFBar->GetDistBarDimInfo(1).GetSize();
		m_comboSecond.SetCurSel(nSize - 1);
	}

	//2. CTC
	double dDia = pGFBar->GetDistRebarDia();
	int nIdx = GetIdxDia(dDia);
	m_comboThird.SetCurSel(nIdx);

	UpdateData(FALSE);

	//3. Data Setting
	CDPointArray ptArrLeftDim = pGFBar->GetDistBarDimInfo(0);
	int i = 0;
	for(i = 0 ; i < ptArrLeftDim.GetSize(); i++)
	{
		CDPoint ptDim = ptArrLeftDim.GetAt(i);
		m_GridFirst.SetTextMatrixFormat(1, i+1, "%0.0f", ptDim.x);
		m_GridFirst.SetTextMatrixFormat(2, i+1, "%0.0f", ptDim.y);
	}

	CDPointArray ptArrRightDim = pGFBar->GetDistBarDimInfo(1);
	for(i = 0 ; i < ptArrRightDim.GetSize(); i++)
	{
		CDPoint ptDim = ptArrRightDim.GetAt(i);
		m_GridSecond.SetTextMatrixFormat(1, i+1, "%0.0f", ptDim.x);
		m_GridSecond.SetTextMatrixFormat(2, i+1, "%0.0f", ptDim.y);
	}
}

void CDeckGuardFenceDlg::SetDataInitShearRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	//1. 구간수 
	CDPointArray ptArrLeftDim = pGFBar->GetShearBarDimInfo();
	int nCnt = ptArrLeftDim.GetSize();
	m_comboFirst.SetCurSel(nCnt - 1);

	//2. 철근직경 
	double dDia = pGFBar->GetShearRebarDia();
	int nIdx = GetIdxDia(dDia);
	m_comboSecond.SetCurSel(nIdx);
	UpdateData(FALSE);

	//Grid Data
	for(int i = 0 ; i < ptArrLeftDim.GetSize(); i++)
	{
		CDPoint ptDim = ptArrLeftDim.GetAt(i);
		m_GridFirst.SetTextMatrixFormat(1, i+1, "%0.0f", ptDim.x);
		m_GridFirst.SetTextMatrixFormat(2, i+1, "%0.0f", ptDim.y);
	}

	//주철근 간격 묶음 개수 
	int nCntMainBarSet = pGFBar->GetCntMainBarSet();
	CString str;
	str.Format("%d", nCntMainBarSet);
	m_GridSecond.SetTextMatrix(1, 0, str); 
	//전단철근 종류 
	int nType = pGFBar->GetTypeShearBar();
	str.Format("%d", nType);
	m_GridSecond.SetTextMatrix(1, 1, str);
}

void CDeckGuardFenceDlg::SetDataSaveMainRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	int nType = pGFBar->GetTypePlacingRebar();
	CString strCTC, strDia;

	//1. 주철근 직경 //타입에 따라 달라져야 한다.
	int nCntBar = pGFBar->GetCntMainRebar();
	for(int i = 0; i < nCntBar; i++)
	{
		strDia = m_GridFirst.GetTextMatrix(1, i+1);
		double dDia = atoi(strDia.Right(2));
		pGFBar->SetMainRebarDia(i, dDia);
	}

	//2. 하부 철근 길이, 처음엔 Width, 다음엔 Height
	int nCol = 1;
	if(nType != 3)
	{
		double dWidth = m_GridSecond.GetTextMatrixDouble(1, nCol++);
		pGFBar->SetWidthMainBarLower(dWidth);
	}

	int nCntHeight = pGFBar->GetCntRebarHeight();
	if(nCntHeight > 0)
	{
		double dHeight = m_GridSecond.GetTextMatrixDouble(1, nCol++);
		pGFBar->SetHeightMainBarLower(dHeight);

		if(nCntHeight > 1)
		{
			dHeight = m_GridSecond.GetTextMatrixDouble(1, nCol++);
			pGFBar->SetHeightMainBarUpper(dHeight);
		}
	}
}

void CDeckGuardFenceDlg::SetDataSaveDistRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	//전면 
	if(m_GridFirst.IsWindowEnabled() == TRUE)
	{
		CDPointArray ptArrFrontDim;
		int nCol = m_comboFirst.GetCurSel() + 1;
		for(int i = 0 ; i < nCol; i++)
		{
			CDPoint ptDim;
			ptDim.x = m_GridFirst.GetTextMatrixDouble(1, i+1);
			ptDim.y = m_GridFirst.GetTextMatrixDouble(2, i+1);
			ptArrFrontDim.Add(ptDim);
		}
		pGFBar->SetDistBarDimInfo(0, ptArrFrontDim);
	}

	//후면 
	if(m_GridSecond.IsWindowEnabled() == TRUE)
	{
		CDPointArray ptArrBackDim;
		int nCol = m_comboSecond.GetCurSel() + 1;
		for(int i = 0 ; i < nCol; i++)
		{
			CDPoint ptDim;
			ptDim.x = m_GridSecond.GetTextMatrixDouble(1, i+1);
			ptDim.y = m_GridSecond.GetTextMatrixDouble(2, i+1);
			ptArrBackDim.Add(ptDim);
		}
		pGFBar->SetDistBarDimInfo(1, ptArrBackDim);
	}
}

void CDeckGuardFenceDlg::SetDataSaveShearRebar()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	if(m_GridFirst.IsWindowEnabled() == TRUE)
	{
		CDPointArray ptArrDim;
		int nCol = m_comboFirst.GetCurSel() + 1;
		for(int i = 0 ; i < nCol; i++)
		{
			CDPoint ptDim;
			ptDim.x = m_GridFirst.GetTextMatrixDouble(1, i+1);
			ptDim.y = m_GridFirst.GetTextMatrixDouble(2, i+1);
			ptArrDim.Add(ptDim);
		}

		pGFBar->SetShearBarDimInfo(ptArrDim);
	}

	if(m_GridSecond.IsWindowEnabled() == TRUE)
	{
		//주철근 간격 묶음 개수 
		int nCntMainBarSet = (int)m_GridSecond.GetTextMatrixDouble(1, 0); 
		pGFBar->SetCntMainBarSet(nCntMainBarSet);
		//전단철근 종류 
		int nType = m_GridSecond.GetTextMatrixInt(1, 1);
		pGFBar->SetTypeShearBar(nType);
	}
}

void CDeckGuardFenceDlg::SetDataInitRebarDia()
{
	m_vecDia.clear();

	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar* pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	CString strType, strDia;
	strType = pGFBar->GetRebarType();
	
	//철근직경 삽입 
	strDia.Format("%s13", strType);
	m_vecDia.push_back(strDia);
	strDia.Format("%s16", strType);
	m_vecDia.push_back(strDia);
	strDia.Format("%s19", strType);
	m_vecDia.push_back(strDia);
	strDia.Format("%s22", strType);
	m_vecDia.push_back(strDia);
	strDia.Format("%s25", strType);
	m_vecDia.push_back(strDia);
	strDia.Format("%s29", strType);
	m_vecDia.push_back(strDia);
	strDia.Format("%s32", strType);
	m_vecDia.push_back(strDia);
	strDia.Format("%s35", strType);
	m_vecDia.push_back(strDia);
}

void CDeckGuardFenceDlg::SetDataInitRebarPart()
{
	m_vecPart.clear();

	for(int i = 0; i < 10; i++)
		m_vecPart.push_back(i+1);
}

void CDeckGuardFenceDlg::SetDataInitRebarCTC()
{
	m_vecCTC.clear();

	for(int i = 0; i < 10; i++)
	{
		double dCTC = 100 + i*25;
		m_vecCTC.push_back(dCTC);
	}
}

BOOL CDeckGuardFenceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->hwnd == m_GridFirst.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN)
	{
		CGeneralBaseDlg::PreTranslateMessage(pMsg);
		CCellID next = m_GridFirst.GetFocusCell();
		if(m_nTab == 0) //주철근 
		{
			if(next.row == 1)
			{
				CStringArray strArrDia;
				CString strType;

				for(int i = 0; i < (long)m_vecDia.size(); i++)
				{
					CString strDia = m_vecDia[i];
					strArrDia.Add(strDia);
				}

				m_GridFirst.SetComboString(strArrDia);		
			}
		}
		return TRUE;
	}

	if(m_GridFirst.TranslateMsg(pMsg))
		return m_GridFirst.PreTranslateMessage(pMsg);
	
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CDeckGuardFenceDlg::OnKillfocusEditCover() 
{
	CPlateBridgeApp		*pDB	= m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar	*pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();
	CString				sText	= _T("");
	
	UpdateData(TRUE);
	
	double	dCover		= pGFBar->GetCover();

	m_dCover.GetWindowText(sText);

	double	dCover_Cont = atof(sText);

	if(dCover != dCover_Cont && dCover_Cont > 0)
	{
		
		pGFBar->SetCover(dCover_Cont);
		DrawInputDomyunView(TRUE);
	}
}

CDPointArray CDeckGuardFenceDlg::ConvertGuardFenceData(CDPoint A[9])
{
	CADeckData* pDeckData	= m_pStd->m_pDeckData;
	CDPointArray ptArrConc, ptArr;
	list<CDPoint> listPos;
	list<CDPoint>::iterator iterPos, iterPos2;

	for(int i = 1 ; i < 9; i++)
		listPos.push_back(A[i]);
	
	//중복된 값을 제거한다.
	iterPos2 = unique(listPos.begin(), listPos.end());
	for(iterPos = listPos.begin(); iterPos != iterPos2; iterPos++)
	{
		CDPoint ptDot = *iterPos;
		ptArrConc.Add(ptDot);
	}

	//중복된 값은 없지만, 한 직선상에 있는 점은 있을수 있다.
	pDeckData->EraseSameVector(ptArrConc);
	
	return ptArrConc;
	
	CRebarPlacing RebarPlace;
	CDPointArray xyArr;
	for (long n=0; n<9; n++)
		xyArr.Add(A[n]);
	RebarPlace.RemoveSamePoint(xyArr);
	//중복된 값은 없지만, 한 직선상에 있는 점은 있을수 있다.
	pDeckData->EraseSameVector(xyArr);
	
	return xyArr;
}

int CDeckGuardFenceDlg::GetIdxCTC(double dCTC)
{
	int nSize = m_vecCTC.size();
	for(int i = 0; i < nSize; i++)
	{
		if(dCTC == m_vecCTC[i])
			return i;
	}

	return 0;
}

CString CDeckGuardFenceDlg::GetRebarType()
{
	CADeckData* pDeckData	= m_pStd->m_pDeckData;
	CString strType;

	strType = GetCharRebarMark(toKgPCm2(pDeckData->m_SigmaY));

	return strType;
}

int CDeckGuardFenceDlg::GetBarPlacingType(CDPointArray ptArrConc)
{
	int nType = 0;
	/************************************************************************/
	/* 콘크리트 모양을 가지고 권장타입을 설정한다.                          */
	/***********************************************************************/
	int nSize = ptArrConc.GetSize();
	if(nSize == 6)
		nType = 1;
	else if(nSize == 8)
		nType = 3;
	else if(nSize == 4)
		nType = 4;

	return nType;
}

void CDeckGuardFenceDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	if(m_nTab == 0)
		SetDataSaveMainRebar();
	else if(m_nTab == 1)
		SetDataSaveDistRebar();
	else if(m_nTab == 2)
		SetDataSaveShearRebar();
	
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

int CDeckGuardFenceDlg::GetIdxDia(double dDia)
{
	int nSize = m_vecDia.size();
	for(int i = 0; i < nSize; i++)
	{
		CString str = m_vecDia[i];
		double dNowDia = atoi(str.Right(2));
		if(dDia == dNowDia)
			return i;
	}

	return -1;
}

//Index로 Dia 찾기 
double CDeckGuardFenceDlg::GetDiaByIdx(int nIdx)
{
	int nSize = m_vecDia.size();
	if(nIdx > nSize || nIdx < 0)
		return 0;

	CString strDia = m_vecDia[nIdx];
	double dDia = atoi(strDia.Right(2));
	return dDia;
}

void CDeckGuardFenceDlg::OnCellChangedFirstGrid(NMHDR* nmgv, LRESULT*)
{
	long col = m_GridFirst.GetFocusCell().col;
	CString	str = m_GridFirst.GetItemText(0,col);
	m_pView->GetDomyun()->STMakeCursor(str);	
}

void CDeckGuardFenceDlg::OnCellChangedSecondGrid(NMHDR* nmgv, LRESULT*)
{
	long col = m_GridSecond.GetFocusCell().col;
	CString	str = m_GridSecond.GetItemText(0,col);
	m_pView->GetDomyun()->STMakeCursor(str);	
}

CDPointArray CDeckGuardFenceDlg::AddDimInfo(CDPointArray ptArrDimInfo, int nAddPart)
{
	CDPoint ptDim;
	for(int i = 0; i < nAddPart; i++)
	{
		ptDim.x = 0;
		ptDim.y = 0;
		ptArrDimInfo.Add(ptDim);
	}

	return ptArrDimInfo;
}

CDPointArray CDeckGuardFenceDlg::DeleteDimInfo(CDPointArray ptArrDimInfo, int nDelPart)
{
	int nSize = ptArrDimInfo.GetSize();
	int nMin = nSize - 1 - nDelPart;
	int nVal = (nMin >= 0) ? nMin : 0;

	for(int i = nSize - 1; i > nVal; i--)
		ptArrDimInfo.RemoveAt(i);

	return ptArrDimInfo;
}

BOOL CDeckGuardFenceDlg::IsValid()
{
	CPlateBridgeApp	*pDB = m_pStd->m_pDataManage->GetBridge();	
	long nSize = pDB->GetGuardFencesu();
	
	return m_pStd->m_bInclude_Module_Deck && nSize > 0;
}

BOOL CDeckGuardFenceDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	SetResize(IDC_COMBO_GUARDWALL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_FIRST, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SECOND, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_THIRD, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_ISREBAR, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BARPLACE_TYPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_COVER, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_COVER, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_UNIT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_FCK, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_FCK, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_FCK_UNIT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_FY, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_FY, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_FY_UNIT, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_CHECK_PLUSTOSLAB, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_FIRST, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID_SECOND, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);		

	return TRUE;
}

void CDeckGuardFenceDlg::OnKillfocusEditFy() 
{
	CString				sText	= _T("");
	CPlateBridgeApp		*pDB	= m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar	*pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	UpdateData(TRUE);

	double dFy = pGFBar->GetFy();
	
	m_dFy.GetWindowText(sText);

	double dFy_Cont = atof(sText);

	if(dFy != dFy_Cont && dFy_Cont > 0)
	{
		sText.Format("%g", dFy_Cont*10);
		
		m_dFy.SetWindowText(sText);

		m_dFy.GetWindowText(sText);

		pGFBar->SetFy(atof(sText));

		SetDataInitRebarDia();
		SetDataInitCombo();
		SetDataInit();
		DrawInputDomyunView(TRUE);
	}	
}

void CDeckGuardFenceDlg::OnCheckPlusToSlabRebar() 
{
	// TODO: Add your control notification handler code here
	
}

void CDeckGuardFenceDlg::OnKillfocusEditFck() 
{
	CString				sText	= _T("");
	CPlateBridgeApp		*pDB	= m_pStd->m_pDataManage->GetBridge();	
	CGuardFenceRebar	*pGFBar = pDB->GetGuardFence(m_nGuard)->GetRebar();

	UpdateData(TRUE);
	double dFck = pGFBar->GetFck();

	m_dFck.GetWindowText(sText);

	double dFck_Cont = atof(sText);

	if(dFck != dFck_Cont && dFck_Cont > 0)
	{
		sText.Format("%g",	dFck_Cont*10);

		m_dFck.SetWindowText(sText);

		m_dFck.GetWindowText(sText);

		pGFBar->SetFck(atof(sText));

		SetDataInitRebarDia();
		SetDataInitCombo();
		SetDataInit();
		DrawInputDomyunView(TRUE);
	}	
}
