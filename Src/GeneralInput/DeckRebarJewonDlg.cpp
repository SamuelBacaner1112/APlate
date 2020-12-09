// DeckRebarJewonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "RebarAddRateDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAKE_NULL if(str=="0") str=_T("")

/////////////////////////////////////////////////////////////////////////////
// CDeckRebarJewonDlg dialog
CDeckRebarJewonDlg::CDeckRebarJewonDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckRebarJewonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckRebarJewonDlg)
	m_strScale = _T("");
	//}}AFX_DATA_INIT
	m_dScaleHV  = 3;
	m_nTab      = 0;
	m_bLockAll  = FALSE;
	m_RebarTableDlg.m_nLastNumRebar = 85;
}



void CDeckRebarJewonDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckRebarJewonDlg)
	DDX_Control(pDX, IDC_SLIDER_SCALE, m_sliderScale);
	DDX_Text(pDX, IDC_STATIC_DATA_SCALE, m_strScale);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Control(pDX, IDC_COMBO_ADDRATE, m_ctrlComboADDRate);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_ADDREBAR, m_btnAdd);
	DDX_Control(pDX, IDC_REMOVEREBAR, m_btnDel);
	DDX_Control(pDX, IDC_REBAR_ADDRATE, m_batnRatio);
}

BEGIN_MESSAGE_MAP(CDeckRebarJewonDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckRebarJewonDlg)
	ON_BN_CLICKED(IDC_REBAR_ADDRATE, OnRebarAddRate)
	ON_BN_CLICKED(IDC_REMOVEREBAR, OnRemoveRebar)
	ON_BN_CLICKED(IDC_ADDREBAR, OnAddRebar)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)	
 	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRATE, OnSelchangeComboAddRate)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckRebarJewonDlg message handlers
void CDeckRebarJewonDlg::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	DrawInputDomyunView();

	return;
}

void CDeckRebarJewonDlg::OnPreInitDialog()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_ctlTab.DeleteAllItems();

	long nTab = 0;
	m_ctlTab.InsertItem(nTab++, "주철근");
	m_ctlTab.InsertItem(nTab++, "배력철근");
	m_ctlTab.InsertItem(nTab++, "기타철근");
	if((pDeckData->IsBridgeHasCrossBeam() || pDeckData->IsBridgeHasOutBracket() )&& pDB->m_dThickSlabHunch>0)
		m_ctlTab.InsertItem(nTab++, "보강철근");
	m_ctlTab.InsertItem(nTab++, "방호벽철근");
	if(pDB->IsTUGir())
		m_ctlTab.InsertItem(nTab++, "구속콘크리트철근");

	if(m_nTab >= 0)
		m_ctlTab.SetCurSel(m_nTab);
	
	if(GetCurTabName()=="배력철근") 
	{
		GetDlgItem(IDC_SLIDER_SCALE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DATA_SCALE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SLIDER_SCALE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DATA_SCALE)->ShowWindow(SW_HIDE);
	}

	m_sliderScale.SetRange(0, 20);
	m_strScale.Format("1 / %.2f", double(m_dScaleHV)/20);

	SetResize(IDC_STATIC_ACCOUNT, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_ADDREBAR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_REMOVEREBAR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_REBAR_ADDRATE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_RATIO, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_ADDRATE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SLIDER_SCALE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_DATA_SCALE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);


	UpdateData(FALSE);
}


void CDeckRebarJewonDlg::SetGridTitle()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	vector<CRebar*> vecRebar;

	long nColsu = 23;
	vecRebar = GetRebarData();
	long nRowsu = vecRebar.size()+1;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetColumnCount(nColsu);
	m_Grid.SetRowCount(nRowsu);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(3);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(45);
	m_Grid.SetColumnWidth(0,40);
	m_Grid.SetColumnWidth(1,50);
	m_Grid.SetColumnWidth(2,200);
	m_Grid.SetTextBkColor(RGBEDITABLE);

	CString str = _T("");
	long nCol = 0;
//	m_Grid.SetCheck(0, nCol, m_bLockAll);
	m_Grid.SetTextMatrix(0,nCol++,"잠금");
	long nRow = 0;
	for(nRow=1; nRow < m_Grid.GetRowCount(); nRow++)
		m_Grid.SetCellType(nRow, 0, CT_CHECKBOX);
	m_Grid.SetTextMatrix(0,nCol++,"주철근");	
	for(nRow=1; nRow < m_Grid.GetRowCount(); nRow++)
		m_Grid.SetCellType(nRow, 1, CT_CHECKBOX);

	m_Grid.SetTextMatrix(0,nCol++,"구분");
	m_Grid.SetTextMatrix(0,nCol++,"기호");
	m_Grid.SetTextMatrix(0,nCol++,"Fy");
	m_Grid.SetTextMatrix(0,nCol++,"직경");
	m_Grid.SetTextMatrix(0,nCol++,"A");
	m_Grid.SetTextMatrix(0,nCol++,"B");
	m_Grid.SetTextMatrix(0,nCol++,"C");
	m_Grid.SetTextMatrix(0,nCol++,"D");
	m_Grid.SetTextMatrix(0,nCol++,"E");
	m_Grid.SetTextMatrix(0,nCol++,"R");
	m_Grid.SetTextMatrix(0,nCol++,"X1");
	m_Grid.SetTextMatrix(0,nCol++,"X2");
	m_Grid.SetTextMatrix(0,nCol++,"이음");
	m_Grid.SetTextMatrix(0,nCol++,"이음수");
	m_Grid.SetTextMatrix(0,nCol++,"상세");	
	long nRowSu = m_Grid.GetRowCount();
	for(nRow=1; nRow < nRowSu; nRow++)
		m_Grid.SetUserComboDialog(nRow, 16, &m_RebarTableDlg);
	
	m_Grid.SetTextMatrix(0,nCol++,"개수");
	m_Grid.SetTextMatrix(0,nCol++,"할증");
	m_Grid.SetTextMatrix(0,nCol++,"a");
	m_Grid.SetTextMatrix(0,nCol++,"b");
	m_Grid.SetTextMatrix(0,nCol++,"c");
	m_Grid.SetTextMatrix(0,nCol++,"d");	// 20

}


void CDeckRebarJewonDlg::SetDataDefault()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	if(IsUsedFlag()==FALSE)
	{
		m_dScaleHV = 3;
		pDeckData->m_AddRateRebar.m_nType = 0;
		pDeckData->m_AddRateRebar.SetDataDefault();
	}
	pDeckData->MakeRebarJewon();

	if(pDB->IsTUGir())
		m_pStd->m_pDeckDataTU->MakeRebarJewonTU();
}


void CDeckRebarJewonDlg::SetDataInit()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	SetGridTitle();
	vector<CRebar*> vecRebar = GetRebarData();
	for(long n=0; n < (long)vecRebar.size(); n++)
		SetDataInitSubRebar(vecRebar[n], n+1);

	BOOL bEmptyCol = TRUE;
	for (long nCol=5; nCol<m_Grid.GetColumnCount(); nCol++)
	{
		bEmptyCol = TRUE;
		for (long nRow=1; nRow<m_Grid.GetRowCount(); nRow++)
		{
			if (m_Grid.GetTextMatrix(nRow, nCol) != "")
			{
				bEmptyCol = FALSE;
				break;
			}
		}
		if (bEmptyCol)
		{
			m_Grid.SetColumnWidth(nCol, 0);
		}
	}

	
	SetTextTotalWeight();

	m_ctrlComboADDRate.SetCurSel(pDeckData->m_AddRateRebar.m_nType);
	m_sliderScale.SetPos((int)m_dScaleHV);
	if(GetCurTabName()=="배력철근") 
	{
		GetDlgItem(IDC_SLIDER_SCALE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DATA_SCALE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SLIDER_SCALE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DATA_SCALE)->ShowWindow(SW_HIDE);
	}
}


void CDeckRebarJewonDlg::SetDataSave()
{
	CADeckData		*pDeckData		= m_pStd->m_pDeckData;
	CADeckDataTU	*pDeckDataTU	= m_pStd->m_pDeckDataTU;
	CPlateBridgeApp *pDB			= m_pStd->m_pDataManage->GetBridge();	

	/************************************************************************/
	/* 기존에 있는 철근제원 데이터를 종류별로 삭제한다.                     */
	/************************************************************************/
	if(GetCurTabName() == "방호벽철근")	//방호벽이면 
	{
		AhTPADelete(&pDeckData->m_arrRebarJewonGuard,(CRebar*)0);		
	}
	else if(GetCurTabName() == "구속콘크리트철근")
	{
		AhTPADelete(&pDeckDataTU->m_arrRebarJewonTU,(CRebar*)0);		
	}
	else
	{
		long nSize = pDeckData->m_arrRebarJewon.GetSize();
		if(nSize <= 0)
			return;

		for(long i = nSize - 1; i >= 0; i--)
		{
			CRebar* pRB = pDeckData->m_arrRebarJewon.GetAt(i);
			if(pRB->m_nIdxNumber == GetRebarType())
				pDeckData->m_arrRebarJewon.RemoveAt(i);
		}
	}

	CString str("");
	long nRebar = m_Grid.GetRowCount()-1;
	for(long n = 0; n < nRebar; n++)
	{
		long nCol = 0;
 		CRebar *pRB = new CRebar();

		pRB->m_bLock		= m_Grid.IsCheck(n+1, nCol++);
		pRB->m_bMainRebar   = m_Grid.IsCheck(n+1, nCol++);
		pRB->m_sDescription = m_Grid.GetTextMatrix(n+1, nCol++);
		pRB->m_sMark		= m_Grid.GetTextMatrix(n+1, nCol++);
		pRB->m_Fy			= m_Grid.GetTextMatrixDouble(n+1, nCol++);
		CString sz = m_Grid.GetTextMatrix(n+1, nCol++);
		sz.Delete(0);
		pRB->m_dDia			= atol(sz);
		pRB->m_dLengthMax_A = m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dLengthMax_B = m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dLengthMax_C = m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dLengthMax_D = m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_E			= m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_R			= m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dAngleX1		= m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dAngleX2		= m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dJointLength	= m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_nJoint		= m_Grid.GetTextMatrixLong(n+1, nCol++);
		pRB->m_nTypeRebar   = m_Grid.GetTextMatrixLong(n+1, nCol++);
		if(GetCurTabName() == "방호벽철근")
			pRB->m_dEa		= m_Grid.GetTextMatrixLong(n+1, nCol++);
		else
			pRB->m_nEa		= m_Grid.GetTextMatrixLong(n+1, nCol++);
		pRB->m_dRateAdd		= m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dLengthMin_A = m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dLengthMin_B = m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dLengthMin_C = m_Grid.GetTextMatrixDouble(n+1, nCol++);
		pRB->m_dLengthMin_D = m_Grid.GetTextMatrixDouble(n+1, nCol++);

		pRB->m_nIdxNumber   = GetRebarType();

		if(GetCurTabName() == "방호벽철근")  //방호벽이면 
		{
			CString szRebar = pRB->m_sDescription;
			long nGuard = atol(szRebar.Right(2))-1;
			if(nGuard>=0)	
			{
				CGuardFenceRebar* pGFBar = pDB->GetGuardFence(nGuard)->GetRebar();
				pRB->m_Fy = pGFBar->GetFy();	
				pDeckData->m_arrRebarJewonGuard.Add(pRB);
			}
			else
			{
				pDeckData->m_arrRebarJewonGuard.Add(pRB);
			}
		}
		else  //방호벽이 아니면 ..
		{
			pRB->m_Fy = toKgPCm2(pDeckData->m_SigmaY);
			
			if(GetCurTabName() == "구속콘크리트철근")
				pDeckDataTU->m_arrRebarJewonTU.Add(pRB);
			else
				pDeckData->m_arrRebarJewon.Add(pRB);
		}
	}
}


void CDeckRebarJewonDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	CDomyun Dom(pDom);
	Dom.SetScaleDim(50);

	if(GetCurTabName() == "주철근")
		DrawHDan(&Dom);
	else if(GetCurTabName() == "배력철근")  
		DrawJDan(&Dom);
	else if(GetCurTabName() == "기타철근")  
		DrawEtcRebar(&Dom);
	else if(GetCurTabName() == "보강철근")  
		DrawCrStgDetail(&Dom);
	else if(GetCurTabName() == "방호벽철근") 
		DrawGuardFence(&Dom);
	else if(GetCurTabName() == "구속콘크리트철근")
	{
		Dom.SetScaleDim(25);
		DrawTUGirder(&Dom);
	}

	DrawRebarDetail(&Dom);

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}



void CDeckRebarJewonDlg::DrawRebarDetail(CDomyun *pDom)
{
	CADeckData		*pDeckData		= m_pStd->m_pDeckData;
	CADeckDataTU	*pDeckDataTU	= m_pStd->m_pDeckDataTU;

	if(m_Grid.GetFocusCell().row<1) return;

	long nRow = m_Grid.GetFocusCell().row;
	CString strMark = m_Grid.GetTextMatrix(nRow, 3);
	BOOL bFind = FALSE;

	CRebar *pRB = NULL;
	

	if(GetCurTabName() == "구속콘크리트철근")
	{
		for(long i = 0; i < pDeckDataTU->m_arrRebarJewonTU.GetSize(); i++)
		{
			pRB = pDeckDataTU->m_arrRebarJewonTU[i];
			if(pRB->m_sMark == strMark)
			{
				bFind = TRUE;
				break;
			}
		}
	}
	else
	{
		for(long i = 0; i < pDeckData->m_arrRebarJewon.GetSize(); i++)
		{
			pRB = pDeckData->m_arrRebarJewon[i];
			if(pRB->m_sMark == strMark)
			{
				bFind = TRUE;
				break;
			}
		}

		if(bFind == FALSE)
		{
			for(long i = 0; i < pDeckData->m_arrRebarJewonGuard.GetSize(); i++)
			{
				pRB = pDeckData->m_arrRebarJewonGuard[i];
				if(pRB->m_sMark == strMark)
				{
					bFind = TRUE;
					break;
				}
			}
		}
	}

	if(bFind == FALSE)
		return;
	
	CDomyun Dom(pDom);
	CConcSymbol symDB(m_pStd->m_pDataManage->GetOptionStd());
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);

	if(pRB)
		symDB.RebarDetailBasic(&Dom, pRB->m_nTypeRebar, CDPoint(0,0), pRB->m_nJoint>0);

	if(GetCurTabName() == "보강철근")
		Dom.RedrawByScale(0.5);
	else if(GetCurTabName() == "방호벽철근")
		Dom.RedrawByScale(0.2);

	CDRect rect;
	pDom->SetCalcExtRect();
	rect = pDom->GetExtRect();
	CDPoint ptCen;
	ptCen.x = rect.CenterPoint().x;
	ptCen.y = rect.bottom;
	Dom.Move(ptCen.x, ptCen.y);

	*pDom << Dom;
}


void CDeckRebarJewonDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CADeckData		*pDeckData		= m_pStd->m_pDeckData;
	CADeckDataTU	*pDeckDataTU	= m_pStd->m_pDeckDataTU;

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	long nRow = GridView->iRow;
    long nCol = GridView->iColumn;

	if(nRow==0 && nCol==0)
	{
		if( MessageBox("철근제원 잠금시에는 철근데이터변경시 적용이 되지 않습니다.\n계속 하시겠습니까","철근제원잠금 입력",
			MB_ICONQUESTION|MB_YESNO) == IDNO ) return;
	}


	CString sMark = m_Grid.GetTextMatrix(nRow, 3);
	CRebar *pRB = NULL;
	if(GetCurTabName()=="방호벽철근")
	{
		for (long n=0; n<pDeckData->m_arrRebarJewonGuard.GetSize(); n++)
		{
			pRB = pDeckData->m_arrRebarJewonGuard[n];
			if(sMark==pRB->m_sMark)
			{
				break;
			}
		}
	}
	else if(GetCurTabName()=="구속콘크리트철근")
	{
		for (long n=0; n<pDeckDataTU->m_arrRebarJewonTU.GetSize(); n++)
		{
			pRB = pDeckDataTU->m_arrRebarJewonTU[n];
			if(sMark==pRB->m_sMark)
			{
				break;
			}
		}
	}
	else
	{
		for (long n=0; n<pDeckData->m_arrRebarJewon.GetSize(); n++)
		{
			pRB = pDeckData->m_arrRebarJewon[n];
			if(sMark==pRB->m_sMark)
			{
				break;
			}
		}
	}

	if(pRB)
	{
		long nCol1 = 0;
		pRB->m_bLock		= m_Grid.IsCheck(nRow, nCol1++);
		pRB->m_bMainRebar   = m_Grid.IsCheck(nRow, nCol1++);
		pRB->m_sDescription = m_Grid.GetTextMatrix(nRow, nCol1++);
		pRB->m_sMark		= m_Grid.GetTextMatrix(nRow, nCol1++);
		pRB->m_Fy			= m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		CString sz = m_Grid.GetTextMatrix(nRow, nCol1++);
		sz.Delete(0);
		pRB->m_dDia			= atol(sz);
		pRB->m_dLengthMax_A = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dLengthMax_B = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dLengthMax_C = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dLengthMax_D = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_E			= m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_R			= m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dAngleX1		= m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dAngleX2		= m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dJointLength	= m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_nJoint		= m_Grid.GetTextMatrixLong(nRow, nCol1++);
		pRB->m_nTypeRebar   = m_Grid.GetTextMatrixLong(nRow, nCol1++);
		if(GetCurTabName() == "방호벽철근")
			pRB->m_dEa		= m_Grid.GetTextMatrixLong(nRow, nCol1++);
		else
			pRB->m_nEa		= m_Grid.GetTextMatrixLong(nRow, nCol1++);
		pRB->m_dRateAdd		= m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dLengthMin_A = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dLengthMin_B = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dLengthMin_C = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_dLengthMin_D = m_Grid.GetTextMatrixDouble(nRow, nCol1++);
		pRB->m_nIdxNumber   = GetRebarType();
	}
	
	// 할증율 재설정
	if(nCol==1)
	{
		pRB->m_dRateAdd = pDeckData->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	}

	SetDataInitSubRebar(pRB, nRow);
	SetDataSave();
	DrawInputDomyunView(FALSE);
}


void CDeckRebarJewonDlg::OnRebarAddRate() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	CRebarAddRateDlg dlg;
	dlg.SetAddRate(pDeckData->m_AddRateRebar);

	if(dlg.DoModal() == IDOK)  //저장 
	{
		pDeckData->m_AddRateRebar = dlg.GetAddRate();
		for (long n=0; n<pDeckData->m_arrRebarJewon.GetSize(); n++)
		{
			CRebar *pRB = pDeckData->m_arrRebarJewon[n];
			pRB->m_dRateAdd = pDeckData->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		}

		SetDataInit();		
	}
}


void CDeckRebarJewonDlg::OnRemoveRebar() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nCurRow = m_Grid.GetFocusCell().row;
	if(nCurRow < 0) return;

	CString strMark = m_Grid.GetTextMatrix(nCurRow, 3);
	BOOL bRet = pDeckData->DeleteRebar(strMark);	
	if(bRet == TRUE)
	{
		m_Grid.DeleteRow(nCurRow);
		m_Grid.Invalidate();
	}
	SetDataSave();
}


//철근 추가 
void CDeckRebarJewonDlg::OnAddRebar() 
{
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;
	CADeckDataTU	*pDeckDataTU= m_pStd->m_pDeckDataTU;

	if(GetCurTabName()=="구속콘크리트철근")
		pDeckDataTU->AddRebar();
	else
		pDeckData->AddRebar(GetRebarType());

	SetDataInit();
	
	m_Grid.Invalidate();
}



vector<CRebar*> CDeckRebarJewonDlg::GetRebarData()
{
	CADeckData		*pDeckData		= m_pStd->m_pDeckData;
	CADeckDataTU	*pDeckDataTU	= m_pStd->m_pDeckDataTU;
	vector<CRebar*> vecRebar;

	if(GetCurTabName()=="방호벽철근")
	{
		long nSize = pDeckData->m_arrRebarJewonGuard.GetSize();
		
		for(long n=0; n<nSize; n++)
		{
			CRebar *pRB = pDeckData->m_arrRebarJewonGuard[n];
			vecRebar.push_back(pRB);
		}
	}
	else if(GetCurTabName()=="구속콘크리트철근")
	{
		long nSize = pDeckDataTU->m_arrRebarJewonTU.GetSize();
		
		for(long n=0; n<nSize; n++)
		{
			CRebar *pRB = pDeckDataTU->m_arrRebarJewonTU[n];
			vecRebar.push_back(pRB);
		}
	}
	else 
	{
		long nSize = pDeckData->m_arrRebarJewon.GetSize();
		
		for(long n=0; n<nSize; n++)
		{
			CRebar *pRB = pDeckData->m_arrRebarJewon[n];
			if(pRB->m_nIdxNumber == GetRebarType())
				vecRebar.push_back(pRB);
		}
	}

	return vecRebar;
}


void CDeckRebarJewonDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_Grid.DeleteAllItems();

	SetDataInit();	

	if(GetCurTabName()=="배력철근") 
	{
		GetDlgItem(IDC_SLIDER_SCALE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DATA_SCALE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SLIDER_SCALE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DATA_SCALE)->ShowWindow(SW_HIDE);
	}

	DrawInputDomyunView(TRUE);
}


void CDeckRebarJewonDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetDataSave();
}


BOOL CDeckRebarJewonDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}


void CDeckRebarJewonDlg::SetDataInitSubRebar(CRebar *pRB, long nRow)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	CString str;
	long nCol = 0;
	
	m_Grid.SetCheck(nRow, nCol++, pRB->m_bLock);
	m_Grid.SetCheck(nRow, nCol++, pRB->m_bMainRebar);
	m_Grid.SetTextMatrix(nRow,nCol++, pRB->m_sDescription);
	m_Grid.SetTextMatrix(nRow,nCol++, pRB->m_sMark);
	str.Format("%.0f",pRB->m_Fy);		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++, str,DT_RIGHT,FALSE);
	str = pRB->GetStringDia();
	m_Grid.SetTextMatrix(nRow,nCol++, pRB->GetStringDia());

	str.Format("%.0f",pRB->m_dLengthMax_A);	MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_dLengthMax_B); MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_dLengthMax_C);	MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_dLengthMax_D);	MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_E);			MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_R);			MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
	str.Format("%.0f",pRB->m_dAngleX1);		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
	str.Format("%.0f",pRB->m_dAngleX2);		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
	str.Format("%.0f",pRB->m_dJointLength);	MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
	str.Format("%d",pRB->m_nJoint);			MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
	str.Format("%d",pRB->m_nTypeRebar);		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);

	if(pRB->m_nIdxNumber == 4)	//방호벽이면 
	{
		if(pDeckData->m_nTypeGuardCalc == 0)  //전체길이로 계산 
		{
			str.Format("%0.0f",max(pRB->m_dEa,0));		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
		}
		else if(pDeckData->m_nTypeGuardCalc == 1)  //단위길이로 계산 
		{
			str.Format("%0.3f",max(pRB->m_dEa,0));		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
		}
	}
	else	//일반철근이면 
	{
		str.Format("%d",max(pRB->m_nEa,0));		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
	}

	str.Format("%.2f",pRB->m_dRateAdd);		MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str);
	str.Format("%.0f",pRB->m_dLengthMin_A);	MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_dLengthMin_B); MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_dLengthMin_C);	MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);
	str.Format("%.0f",pRB->m_dLengthMin_D);	MAKE_NULL;	m_Grid.SetTextMatrix(nRow,nCol++,str,DT_RIGHT,FALSE);

	for ( nRow=1; nRow<m_Grid.GetRowCount(); nRow++)
	{
		if(nRow%2)
		{
			for (long nCol=3; nCol<m_Grid.GetColumnCount(); nCol++)
				m_Grid.SetItemBkColour(nRow, nCol, RGB(129, 203, 216));
		}
		else
		{
			for (long nCol=3; nCol<m_Grid.GetColumnCount(); nCol++)
				m_Grid.SetItemBkColour(nRow, nCol, RGBEDITABLE);
		}
	}

	m_Grid.SetRedraw();
}


void CDeckRebarJewonDlg::DrawHDan(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	CAPlateDrawDanmyun DrawDanmyun(m_pStd->m_pDataManage);
	CDrawDBFrontRebar   DBFrontRebar(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);

	vector <REBAR_CYCLE> ST_Cycle;
	long nSection = 0, nCurCycle = 0;
	long nRow = m_Grid.GetFocusCell().row;
	CString str = m_Grid.GetTextMatrix(nRow, 2);

	if(str.Find("단부") != -1)  //단부
		nSection = SECTION_DAN;
	else if(str.Find("일반부") != -1)  //일반부 
		nSection = SECTION_CEN;
	else if(str.Find("시점사보강") != -1)  //사보강 
		nSection = SECTION_SKEW_STT;
	else if(str.Find("종점사보강") != -1)  //사보강 
		nSection = SECTION_SKEW_END;
	else if(str.Find("사각주철근") != -1)  //사각주철근 
		return;

	if(str.Find("Cycle") != -1) //Cycle 찾기 
		nCurCycle = atoi(str.Right(6)) - 1;

	CPlateBasicIndex *pBx = pDeckData->GetBxBySection(nSection);

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);

	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, FALSE);

	DBFrontRebar.DrawMainRebar(&Dom, nSection, nCurCycle, TRUE, FALSE);
	DBFrontRebar.DimMainRebarJolib(&Dom, nSection, nCurCycle, FALSE, m_dScaleDim/100);
	DBFrontRebar.DrawMarkMainRebar(&Dom, nSection, nCurCycle);

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	CString sz;
	sz.Format("%d - CYCLE", nCurCycle+1);
	DrawStd.AddTitleOnDom(&Dom, 80, rect, sz, TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);

	*pDom << Dom;
}




//종단면도상의 배력철근을 그린다.
void CDeckRebarJewonDlg::DrawJDan(CDomyun *pDom)
{
	CDrawDBBaseRebarJong DBDeckJong(m_pStd->m_pDataManage);
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,MAIN_MEMBER);
	Dom.SetScaleDim(50);

	long nCurCycle = 0;
	long nRow = m_Grid.GetFocusCell().row;
	CString str = m_Grid.GetTextMatrix(nRow, 2);

	if(str.Find("Cycle") != -1)
		nCurCycle = atoi(str.Right(6)) - 1;

	long nJongPos = 0;
	if (str.Find("L") == -1 && str.Find("R") == -1)
		nJongPos = JONGBASEPOS_SLABCENTER;
	else if(str.Find("L") != -1)
		nJongPos = JONGBASEPOS_SLABLEFT;
	else if(str.Find("R") != -1)
		nJongPos = JONGBASEPOS_SLABRIGHT;

	double dMoveY = 0;
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);
	DBDeckJong.DrawBeryukRebarAtJong(&Dom, nJongPos, nCurCycle, TRUE);
	
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	DBDeckJong.DimBeryukRebarAtJong(&Dom, nJongPos, nCurCycle, FALSE, m_dScaleDim/100);
	DBDeckJong.DrawMarkBeryukRebarAtJongTotal(&Dom, nCurCycle, nJongPos);

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	CString szText, szScale;
	szText.Format("%d - CYCLE", nCurCycle+1);
	DrawStd.AddTitleOnDom(&Dom, 80, rect, szText, TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
	dMoveY -= rect.Height()*2;
	Dom.Move(0, dMoveY);
	Dom.RedrawByHVScale(m_dScaleHV/20, TRUE, FALSE);

	*pDom << Dom;
}

void CDeckRebarJewonDlg::DrawEtcRebar(CDomyun *pDom)
{
	CADeckData			*pDeckData		= m_pStd->m_pDeckData;	
	CDrawOpenRebar		DrawOpenRebar(m_pStd->m_pDataManage);
	CDrawSpacer			DrawSpacer(m_pStd->m_pDataManage);
	CDrawSupportRFBar	DrawShearBar(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);

	double dScaleDim = 40;
	CDRect rect;
	pDom->SetScaleDim(dScaleDim);
	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScaleDim);

	/************************************************************************/
	/* 전단 보강철근                                                        */
	/************************************************************************/
	if(pDeckData->m_bInstallShearRebar)
	{
		double dDisDetail = 1500;
		//1. 단지점부 
		if(pDeckData->m_ShearRebarDanbu.GetSize() > 0)
		{
			DrawShearBar.DrawRebar(&Dom, 0);
			DrawShearBar.DimRebar(&Dom, 0);
			DrawShearBar.MarkRebar(&Dom, 0);
			DrawShearBar.DrawDetailCircle(&Dom, 0);
			DrawShearBar.DrawDetailCircleText(&Dom, 0, "상세 A");
			DrawShearBar.DrawDetail(&Dom, 0, dDisDetail);
			
			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			DrawStd.AddTitleOnDom(&Dom, dScaleDim, rect, "단지점부 보강철근", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
			Dom.Move(Dom.Always(50), Dom.Always(150));
			*pDom << Dom;
		}

		//2. 중간지점부 
		if(pDeckData->m_ShearRebarJijum.GetSize() > 0)
		{
			DrawShearBar.DrawRebar(&Dom, 1);
			DrawShearBar.DimRebar(&Dom, 1);
			DrawShearBar.MarkRebar(&Dom, 1);
			DrawShearBar.DrawDetailCircle(&Dom, 1);
			DrawShearBar.DrawDetailCircleText(&Dom, 1, "상세 B");
			DrawShearBar.DrawDetail(&Dom, 1, dDisDetail);

			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			DrawStd.AddTitleOnDom(&Dom, dScaleDim, rect, "중간지점부 보강철근", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
			Dom.Move(Dom.Always(200), Dom.Always(150));
			*pDom << Dom;
		}
	}

	/************************************************************************/
	/* 배수구 보강철근                                                      */
	/************************************************************************/
	if(pDeckData->m_bInstallWaterHole)
	{
		DrawOpenRebar.SetType(pDeckData->m_nTypeWaterHole);
		DrawOpenRebar.DrawOpen(&Dom);
		DrawOpenRebar.DrawRebar(&Dom);
		DrawOpenRebar.DimRebar(&Dom);
		DrawOpenRebar.MarkRebar(&Dom);
	}
	
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	DrawStd.AddTitleOnDom(&Dom, dScaleDim, rect, "배수구 보강철근", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
	Dom.Move(Dom.Always(50), Dom.Always(30));
	*pDom << Dom;

	/************************************************************************/
	/* 간격재                                                               */
	/************************************************************************/
	//1. 정면도 
	double dScale	= dScaleDim/2;
	Dom.SetScaleDim(dScale);
	DrawSpacer.DrawFrontRebar(&Dom);
	DrawSpacer.DimFrontRebar(&Dom, FALSE);
	DrawSpacer.MarkRebar(&Dom);
	Dom.RedrawByScale(dScaleDim/dScale);//확대
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.Move(Dom.Always(200), Dom.Always(40));
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawStd.AddTitleOnDom(&Dom, dScaleDim, rect, "간격재", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
	*pDom << Dom;		
	
	//2. 평면도 
	Dom.SetScaleDim(dScale);
	DrawSpacer.DrawPlanRebar(&Dom);
	DrawSpacer.DimPlanRebar(&Dom, FALSE);
	Dom.RedrawByScale(dScaleDim/dScale);//확대
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.Move(Dom.Always(200), Dom.Always(20));
	*pDom << Dom;

	//3. 간격입력 
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawSpacer.DrawPlaceRebar(&Dom);
	Dom.Move(Dom.Always(300), Dom.Always(40));
	*pDom << Dom;
}

void CDeckRebarJewonDlg::DrawCrStgDetail(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CDrawDBFrontRebar   DBFrontRebar(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);
	CADeckData			*pDeckData		= m_pStd->m_pDeckData;

	
	CDomyun Dom(pDom);
	
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetScaleDim(35);
	CDRect rect1, rect2, rect3;
	if(pDeckData->IsBridgeHasCrossBeamHunch() && pDB->m_dThickSlabHunch>0)
	{
		DBFrontRebar.DrawSangseCrossBeam(&Dom);
		Dom.SetCalcExtRect();
		rect1 = Dom.GetExtRect();
		DrawStd.AddTitleOnDom(&Dom, 50, rect1, "가로보 조립도", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
		*pDom << Dom;

//		if(pDeckData->IsBridgeHasStringer())
//		{
//			DBFrontRebar.DrawSangseStringer(&Dom);
//			Dom.SetCalcExtRect();
//			rect2 = Dom.GetExtRect();
//			DrawStd.AddTitleOnDom(&Dom, 50, rect2, "세로보 조립도", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
//			double dMoveX = rect1.Width()*1.5;
//			Dom.Move(dMoveX, 0);
//		
//			*pDom << Dom;
//		}
	}

//	if(pDeckData->IsBridgeHasOutBracket())
//	{
//		DBFrontRebar.DrawSangseOutBracket(&Dom);
//		Dom.SetCalcExtRect();
//		rect3 = Dom.GetExtRect();
//		DrawStd.AddTitleOnDom(&Dom, 50, rect3, "외측가로보 조립도", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
//		double dMoveX = (rect1.Width()+rect2.Width())*1.2;
//		Dom.Move(dMoveX, 0);
//	
//		*pDom << Dom;
//	}
}


void CDeckRebarJewonDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);
	CString str = m_Grid.GetItemText(nRow, 2);
	m_pView->GetDomyun()->STMakeCursor(str);
}




void CDeckRebarJewonDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_dScaleHV = m_sliderScale.GetPos();
	m_strScale.Format("1 / %.2f", double(m_dScaleHV)/20);
	UpdateData(FALSE);
	DrawInputDomyunView(TRUE);
	
	CGeneralBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDeckRebarJewonDlg::SetTextTotalWeight()
{
	CADeckData		*pDeckData		= m_pStd->m_pDeckData;
	CADeckDataTU	*pDeckDataTU	= m_pStd->m_pDeckDataTU;

	CString szTab = GetCurTabName();
	CString szText;
	BOOL bMainRebar  = FALSE;
	BOOL bGuardRebar = FALSE;
	double dSum  = 0;
	double dArea = 0;

	if(szTab=="구속콘크리트철근")
	{
		dSum  = Round(toTon(pDeckDataTU->GetRebarWeightTotal()), 3);
		dArea = Round(pDeckDataTU->GetAreaSlab()/1000000, 3);
	}
	else
	{
		if(szTab=="방호벽철근")
		{
			bMainRebar = FALSE;
			bGuardRebar= TRUE; 
		}
		else
		{
			bMainRebar = TRUE;
			bGuardRebar= FALSE; 
		}

		dSum  = Round(toTon(pDeckData->GetRebarWeightTotal(bMainRebar, bGuardRebar)), 3);
		dArea = Round(pDeckData->GetAreaSlab()/1000000, 3);
	}

	if(szTab=="방호벽철근")		szText.Format("총중량 : %0.3f tonf, 면적당중량 : %0.3f tonf/m²(방호벽만)", dSum, dSum/dArea);
	else						szText.Format("총중량 : %0.3f tonf, 면적당중량 : %0.3f tonf/m²(방호벽제외)", dSum, dSum/dArea);
	SetDlgItemText(IDC_STATIC_ACCOUNT, szText);	

	UpdateData(FALSE);
}

void CDeckRebarJewonDlg::DrawGuardFence(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CDrawGuardFenceRebar DrawGFBar(m_pStd->m_pDataManage);
	CAPlateDrawDanmyun DrawDan(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);

	double Scale = 10;
	Dom.SetScaleDim(Scale);

	long nRow   = m_Grid.GetFocusCell().row;
	if(nRow < 1)	nRow = 1;

	CString szRebar = m_Grid.GetTextMatrix(nRow, 2);
	long nGuard = atol(szRebar.Right(2))-1;

	if(nGuard<0)	return;

	//1. Draw Conc
	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CCentSeparation *pGuard = pDB->GetGuardFence(nGuard);
	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);
	DrawDan.DstDrawGuardFence(&Dom, pBx, nHDan);
	//2. Draw Rebar
	DrawGFBar.DrawRebar(&Dom, nGuard);	
	//3. DimRebar
	DrawGFBar.DimRebar(&Dom, nGuard);	
	//4. MarkRebar
	DrawGFBar.MarkRebar(&Dom, nGuard);	

	*pDom << Dom;
}


void CDeckRebarJewonDlg::DrawTUGirder(CDomyun *pDom)
{
	long	nRow	= m_Grid.GetFocusCell().row;
	CString sMark	= m_Grid.GetTextMatrix(nRow, 3);
	long	nG		= (long)atof(sMark.Right(1))-1;
	if(nG<0)	nG = 0;

	CPlateBridgeApp	*pDB		= m_pStd->m_pDataManage->GetBridge();	
	CADeckDataTU	*pDeckDataTU= m_pStd->m_pDeckDataTU;
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);
	CRebar			*pRB		= NULL;

	for (long n=0; n<pDeckDataTU->m_arrRebarJewonTU.GetSize(); n++)
	{
		pRB = pDeckDataTU->m_arrRebarJewonTU[n];
		if(sMark==pRB->m_sMark)
		{
			break;
		}
	}
	BOOL	bDanbuJewon	= FALSE;
	if(pRB->m_sDescription.Find("단부")!=-1)	bDanbuJewon = TRUE;

	CDrawDBBaseRebarTU	DBRebarTU(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	
	double dWebT		= pDeckDataTU->GetMaxWebT(nG);//pGir->GetValueBasicDefault("WEB_T");
	double dFlangeTUp	= pGir->GetValueBasicDefault("FLANGE_T");
	double dFlangeTDn	= pGir->GetValueBasicDefault("FLANGE_T");
	double dBindW		= pDB->m_BindConc.m_dWidth;
	double dBindH		= pDB->m_BindConc.m_dHeight;
	double dBindS1		= pDB->m_BindConc.m_dDeep;
	double dBindS2		= pDB->m_BindConc.m_dDis;
	double dSlabLowH	= -pDB->m_dThickSlabBase;	// 슬래브 하면의 높이
	double dFlangH		= dSlabLowH - (dBindH-dBindS1-dBindS2);
	double dGirderCenter= pGir->m_dGirderCenter;
	double dHeight		= pGir->GetHeightGirderByJijum(0);
	double dUWidth		= pGir->GetValueBasicDefault("FLANGE_W");
	double dLWidth		= pGir->GetValueBasicDefault("FLANGE_W");

	Dom.Rectangle(dGirderCenter-dUWidth/2, dFlangH, dGirderCenter+dUWidth/2, dFlangH-dFlangeTUp);
	Dom.Rectangle(dGirderCenter-dWebT/2,   dFlangH-dFlangeTUp, dGirderCenter + dWebT/2, dFlangH-dFlangeTUp-dHeight);
	Dom.Rectangle(dGirderCenter-dLWidth/2, dFlangH-dFlangeTUp-dHeight, dGirderCenter+dLWidth/2, dFlangH-dFlangeTUp-dFlangeTDn-dHeight);

	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dSlabLowH+dBindS1);
	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter-dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter-dBindW/2, dFlangH-dBindS2,	dGirderCenter-dWebT/2,  dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dFlangH-dBindS2,	dGirderCenter+dWebT/2,  dFlangH-dBindS2);

	// Rebar
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);

	CPlateBasicIndex *pBx	= pGir->GetBxByStation(pGir->GetStationStt());
	if(!bDanbuJewon)	pBx	= pGir->GetBxOnJijum(0);
	DBRebarTU.DrawBincConcRebarAtCross(&Dom, pBx, !bDanbuJewon);
	DBRebarTU.DimBincConcRebarAtCross(&Dom, nG, FALSE, !bDanbuJewon, bDanbuJewon);

	*pDom << Dom;
}


void CDeckRebarJewonDlg::OnSelchangeComboAddRate() 
{
	CADeckData*	pDeckData	= m_pStd->m_pDeckData;	
	pDeckData->m_AddRateRebar.m_nType = m_ctrlComboADDRate.GetCurSel();
	
	for (long n=0; n<pDeckData->m_arrRebarJewon.GetSize(); n++)
	{
		CRebar *pRB = pDeckData->m_arrRebarJewon[n];
		pRB->m_dRateAdd = pDeckData->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	}

	SetDataInit();
	
}


CString CDeckRebarJewonDlg::GetCurTabName()
{
	CString sText;
	TCITEM item ;
	char cTableName[256];
	item.mask = TCIF_TEXT;
	item.pszText = cTableName;
	item.cchTextMax = 255;

	m_ctlTab.GetItem(m_ctlTab.GetCurSel(), &item);
	sText = item.pszText;

	return sText;
	
}


long CDeckRebarJewonDlg::GetRebarType()
{
	long nType = 0;
	CString szTab = GetCurTabName();
	if(szTab == "주철근")					nType = 0;
	else if(szTab == "배력철근")			nType = 1;
	else if(szTab == "기타철근")			nType = 2;
	else if(szTab == "보강철근")			nType = 3;
	else if(szTab == "방호벽철근")			nType = 4;
	else if(szTab == "구속콘크리트철근")	nType = 5;

	return nType;
}

