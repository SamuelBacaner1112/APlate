// CalcFrameComboLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLOOR_LOAD_DEAD					"DEAD"
#define FLOOR_LOAD_PREDEAD				"PREDEAD"
#define FLOOR_LOAD_POSTDEAD				"POSTDEAD"

#define FLOOR_LOAD_SLAB_STR				"슬래브"
#define FLOOR_LOAD_SLAB_TU_STR			"구속콘크리트"
#define FLOOR_LOAD_PAVE_STR 			"포장하중"
#define FLOOR_LOAD_GUARD_STR 			"방호벽자중"
#define FLOOR_LOAD_EXTRA_STR			"EXTRA"
#define FLOOR_LOAD_PEOPLE_STR			"PEOPLE"
#define FLOOR_LOAD_LIVE_DB_STR			"DB"
#define FLOOR_LOAD_LIVE_DL_STR 			"DL"
#define FLOOR_LOAD_LIVE_TANK_STR		"TANK"
#define FLOOR_LOAD_LIVE_TRAILER_STR		"TRAIL"
#define EPSILON							0.0000000001

/////////////////////////////////////////////////////////////////////////////
// CCalcFrameComboLoadDlg dialog
CCalcFrameComboLoadDlg::CCalcFrameComboLoadDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcFrameComboLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcFrameComboLoadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSelectedCombo = 0;
}


void CCalcFrameComboLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcFrameComboLoadDlg)
		DDX_Control(pDX, IDC_TAB_LOAD, m_ctlTab);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_COMOB,	m_GridCombo);
	DDX_GridControl(pDX, IDC_GRID_LOAD,		m_GridLoad);
	DDX_GridControl(pDX, IDC_GRID_STDLOAD,	m_GridStd);
	
	DDX_Control(pDX, IDC_BUTTON_ADD_COMBO,		m_btnAddCombo);
	DDX_Control(pDX, IDC_BUTTON_DELETE_COMBO,	m_btnDelCombo);
	DDX_Control(pDX, IDC_BUTTON_UPPER,			m_btnUpper);
	DDX_Control(pDX, IDC_BUTTON_LOWER,			m_btnLower);
	DDX_Control(pDX, IDC_BUTTON_INSERTCOMBO,	m_btnInsertCombo);
	DDX_Control(pDX, IDC_BUTTON_REMOVECOMBO,	m_btnRemoveCombo);
	
}


BEGIN_MESSAGE_MAP(CCalcFrameComboLoadDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcFrameComboLoadDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD_COMBO, OnButtonAddCombo)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_COMBO, OnButtonDeleteCombo)
	ON_BN_CLICKED(IDC_BUTTON_INSERTCOMBO, OnButtonInsertcombo)
	ON_BN_CLICKED(IDC_BUTTON_REMOVECOMBO, OnButtonRemovecombo)
	ON_BN_CLICKED(IDC_BUTTON_UPPER, OnButtonUpper)
	ON_BN_CLICKED(IDC_BUTTON_LOWER, OnButtonLower)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOAD, OnTabSelchange)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID_COMOB,  OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_LOAD,	 OnCellChangedDataLoad)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_COMOB, OnCellChangedDataCombo)
END_MESSAGE_MAP()

void CCalcFrameComboLoadDlg::OnPreInitDialog()
{
	m_ctlTab.DeleteAllItems();

	m_ctlTab.InsertItem(0,	"합성전 사하중");
	m_ctlTab.InsertItem(1,	"합성후 사하중");
	m_ctlTab.InsertItem(2,	"활하중");
	m_ctlTab.InsertItem(3,	"지점침하");
	m_ctlTab.SetCurSel(2);

	SetResize(IDC_TAB_LOAD,				SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_COMOB,			SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_LOAD,			SZ_TOP_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID_STDLOAD,			SZ_MIDDLE_RIGHT,SZ_BOTTOM_RIGHT);


	SetResize(IDC_BUTTON_UPPER,			SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON_LOWER,			SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON_INSERTCOMBO,	SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON_REMOVECOMBO,	SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);

	SetResize(IDC_BUTTON_ADD_COMBO,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE_COMBO,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
}

BOOL CCalcFrameComboLoadDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	return TRUE;  
}

void CCalcFrameComboLoadDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun* pDom = m_pView->GetDomyun();
	pDom->ClearEtt();
		
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcFrameComboLoadDlg::SetGridTitleCombo()
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	long			nCurSel		= m_ctlTab.GetCurSel();

	nCurSel = GetnTypeAnalysis(nCurSel);
	
	CFEMManage      *pFEM = pCalcStd->GetModelingFemData(nCurSel);
	
	long	nColSu	=	5;
	long	nRowSu	=	pFEM->m_ComboList.GetCount() == 0 ? 1 : pFEM->m_ComboList.GetCount()+1;
	
	GridCommon(&m_GridCombo, nRowSu, nColSu);
	
	m_GridCombo.SetColumnWidthAll(40);
	m_GridCombo.SetColumnWidth(1, 80);
	m_GridCombo.SetColumnWidth(2, 80);
	m_GridCombo.SetColumnWidth(3, 280);
	m_GridCombo.SetRowHeightAll(20);
	
	m_GridCombo.SetTextMatrix(0, 0, "순번");
	m_GridCombo.SetTextMatrix(0, 1, "조합명");
	m_GridCombo.SetTextMatrix(0, 2, "조합");
	m_GridCombo.SetTextMatrix(0, 3, "설명");
	m_GridCombo.SetTextMatrix(0, 4, "적용");
	
	if(pFEM->m_ComboList.GetCount() == 0)	return;
	
	for(long nRow=1; nRow<nRowSu; nRow++)
	{
		for(long nCol=1; nCol<nColSu; nCol++)
		{
			if(nCol==2)			m_GridCombo.SetCellType(nRow, nCol, CT_COMBO);
			else if(nCol==4)	m_GridCombo.SetCellType(nRow, nCol, CT_CHECKBOX);
		}
	}

	m_GridCombo.SetRedraw(TRUE,TRUE);
}

void CCalcFrameComboLoadDlg::SetGridTitleLoad()
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	long			nCurSel		= m_ctlTab.GetCurSel();

	nCurSel = GetnTypeAnalysis(nCurSel);
	
	CFEMManage	*pFEM = pCalcStd->GetModelingFemData(nCurSel);
	CString		szText		= _T("");
	
	POSITION	pos		=	pFEM->m_ComboList.GetHeadPosition();
	CCombo		*pComb	=	NULL;
	long		nSize	=	0;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb)	nSize = 0;
	else		nSize = pComb->m_szLoadName.GetSize();

	long	nRowSu	=	nSize == 0 ? 1 : nSize+1;
	long	nColSu	=	3;
	
	GridCommon(&m_GridLoad, nRowSu, nColSu);

	m_GridLoad.SetColumnWidthAll(40);
	m_GridLoad.SetColumnWidth(1, 100);
	m_GridLoad.SetColumnWidth(2, 100);
	m_GridLoad.SetRowHeightAll(20);
	
	m_GridLoad.SetTextMatrix(0, 0, "순번");
	m_GridLoad.SetTextMatrix(0, 1, "하중명");
	m_GridLoad.SetTextMatrix(0, 2, "가중치");
	
	if(pFEM->m_ComboList.GetCount()==0)	return;
	
	m_GridLoad.SetRedraw(TRUE,TRUE);
	m_GridLoad.ExpandColumnsToFit();

}

void CCalcFrameComboLoadDlg::SetGridTitleStd()
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	long			nCurSel		= m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);

	CFEMManage  *pFEM	=	pCalcStd->GetModelingFemData(nCurSel);
	long		nColSu	=	3;
	long		nRowSu	=	0;

	for(long nLC = 0; nLC < pFEM->m_szLCName.GetSize(); nLC++)
	{
		if(pFEM->m_szLCName[nLC].Find("MAX") > -1 || pFEM->m_szLCName[nLC].Find("MIN") > -1)
			continue;
		nRowSu++;
	}
	nRowSu += m_nSelectedCombo + 1 + pFEM->m_MovingLoadList.GetCount();
				
	GridCommon(&m_GridStd, nRowSu, nColSu);
	
	m_GridStd.SetColumnWidthAll(40);
	m_GridStd.SetColumnWidth(1, 80);
	m_GridStd.SetColumnWidth(2, 120);
	m_GridStd.SetRowHeightAll(20);
	
	m_GridStd.SetTextMatrix(0, 0, "순번");
	m_GridStd.SetTextMatrix(0, 1, "하중명");
	m_GridStd.SetTextMatrix(0, 2, "설명");
	
	m_GridStd.SetRedraw(TRUE,TRUE);
	m_GridStd.ExpandColumnsToFit();
}

void CCalcFrameComboLoadDlg::GridCommon(CGridCtrl *pGrid, long nQtyRow, long nQtyCol)
{
	pGrid->DeleteAllItems();
	
	pGrid->EnableReturnPass(TRUE);
	pGrid->EnableInsertCol(FALSE);
	pGrid->EnableInsertRow(FALSE);
	pGrid->SetTextBkColor(RGBEDITABLE);	
	pGrid->SetRowResize(TRUE);
	pGrid->SetColumnResize(TRUE);
	pGrid->SetGridLines(GVL_BOTH);
	pGrid->SetFixedRowCount(1);
	pGrid->SetFixedColumnCount(1);
	pGrid->SetListMode(FALSE);
	pGrid->EnableTitleTips(FALSE);
	
	pGrid->SetRowCount(nQtyRow);
	pGrid->SetColumnCount(nQtyCol);
	
	pGrid->SetRedraw(TRUE,TRUE);
}

void CCalcFrameComboLoadDlg::SetDataInit()
{	
	SetGridTitleCombo();
	SetDataInitCombo();
	
	SetGridTitleLoad();
	SetDataInitLoad();

	SetGridTitleStd();
	SetDataInitStd();
}

void CCalcFrameComboLoadDlg::RemoveLCMinMaxData()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);
				
	CFEMManage	*pFEM =	pCalcStd->GetModelingFemData(nCurSel);
	CString		szText		=	_T("");

	for(long nLC = 0; nLC < pFEM->m_szLCName.GetSize(); nLC++)
	{
		if((pFEM->m_szLCName[nLC].Find("MAX") > -1) || (pFEM->m_szLCName[nLC].Find("MIN") > -1))
		{
			pFEM->m_szLCName.RemoveAt(nLC);
			if(pFEM->m_szLCComment.GetSize() > nLC)
				pFEM->m_szLCComment.RemoveAt(nLC);
		}	
	}

}

void CCalcFrameComboLoadDlg::SetDataDefault()
{
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);
	
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	ModelCalc.MakeFrameModeling();
	
}

void CCalcFrameComboLoadDlg::SetDataInitCombo()
{
 	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);

	CFEMManage	*pFEM =	pCalcStd->GetModelingFemData(nCurSel);

	long	nRow	=	1;
	CString	szText	=	_T("");

	if(pFEM->m_ComboList.GetCount()==0)	return;
	
	POSITION pos = pFEM->m_ComboList.GetHeadPosition();
			 
	while(pos)
	{
		CCombo *pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		szText.Format("%d", nRow);
		
		m_GridCombo.SetTextMatrix(nRow, 0, szText);
		m_GridCombo.SetTextMatrix(nRow, 1, pComb->m_szName);
		m_GridCombo.SetTextMatrix(nRow, 2, pComb->m_nEnvType ==0 ? "ADD" : "ENVE");
		m_GridCombo.SetTextMatrix(nRow, 3, pComb->m_szDescription);
		m_GridCombo.SetCheck(nRow, 4,pComb->m_bApply);

		nRow++;
	}

	m_GridCombo.SetRedraw(TRUE, TRUE);
}

void CCalcFrameComboLoadDlg::SetDataInitStd()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);
				
	CFEMManage	*pFEM	=	pCalcStd->GetModelingFemData(nCurSel);
	CString		szText	=	_T("");
	CString		szComb	=	_T("");
	long		nRow	=	1;
	
	POSITION pos ;
		
	nRow = 1;
	
	for(long nLC = 0; nLC < pFEM->m_szLCName.GetSize(); nLC++)
	{
		if(pFEM->m_szLCName[nLC].Find("MAX") > -1 || pFEM->m_szLCName[nLC].Find("MIN") > -1)
			continue;

		szText.Format("%d", nRow);
		szComb = pFEM->m_szLCName[nLC];
		
		m_GridStd.SetTextMatrix(nRow, 0, szText);
		m_GridStd.SetTextMatrix(nRow, 1, pFEM->m_szLCName[nLC]);
		
		if(nLC < pFEM->m_szLCComment.GetSize())
			m_GridStd.SetTextMatrix(nRow, 2, pFEM->m_szLCComment[nLC]);

		m_GridStd.SetItemState(nRow, 1, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 1, RGBREADONLY);
		m_GridStd.SetItemState(nRow, 2, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 2, RGBREADONLY);
		
		nRow++;
	}

	pos = pFEM->m_MovingLoadList.GetHeadPosition();

	while(pos)
	{
		CMovingLoad *pMove = (CMovingLoad*) pFEM->m_MovingLoadList.GetNext(pos);	
		if(!pMove) return;

		szText.Format("%d", nRow);
			
		m_GridStd.SetTextMatrix(nRow, 0, szText);
		m_GridStd.SetTextMatrix(nRow, 1, pMove->m_szName);
		m_GridStd.SetTextMatrix(nRow, 2, "");
			
		m_GridStd.SetItemState(nRow, 1, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 1, RGBREADONLY);
		m_GridStd.SetItemState(nRow, 2, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 2, RGBREADONLY);
			
		nRow++;
	}
	if(pFEM->m_ComboList.GetCount() <= 0 )  return;

	pos = pFEM->m_ComboList.GetHeadPosition();

	for(long nComb = 0; nComb < m_nSelectedCombo; nComb++)
	{	
		CCombo *pComb = (CCombo *)pFEM->m_ComboList.GetNext(pos);
		if(!pComb) continue;

		szText.Format("%d", nRow);

		m_GridStd.SetTextMatrix(nRow,	0,	szText);
		m_GridStd.SetTextMatrix(nRow,	1,	pComb->m_szName);
		m_GridStd.SetTextMatrix(nRow,	2, pComb->m_szDescription);

		m_GridStd.SetItemState(nRow,	1,	GVIS_READONLY); m_GridStd.SetItemBkColour(nRow,	1,	RGBREADONLY);
		m_GridStd.SetItemState(nRow,	2,	GVIS_READONLY); m_GridStd.SetItemBkColour(nRow,	2,	RGBREADONLY);

		nRow++;	
	}

	for (long n=nRow; n<m_GridStd.GetRowCount(); n++)
		m_GridStd.DeleteRow(n--);

	m_GridStd.SetRedraw(TRUE, TRUE);

}
void CCalcFrameComboLoadDlg::SetDataInitLoad()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);	
	
	CFEMManage      *pFEM =	pCalcStd->GetModelingFemData(nCurSel);

	long		nRow	=	1;
	long		nSize	=	0;
	CString		szText	=	_T("");
	POSITION	pos		=	pFEM->m_ComboList.GetHeadPosition();
	CCombo		*pComb	=	NULL;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb)	nSize = 0;
	else		nSize = pComb->m_szLoadName.GetSize();

	for(long n=0; n<nSize; n++)
	{
		szText.Format("%d", nRow);
		m_GridLoad.SetTextMatrix(nRow, 0, szText);
		szText.Format("%s", pComb->m_szLoadName[n]);
		m_GridLoad.SetTextMatrix(nRow, 1, szText);
		szText.Format("%.3f", pComb->m_dSafe[n]);
		m_GridLoad.SetTextMatrix(nRow, 2, szText);
		
		m_GridLoad.SetItemState(nRow, 1, GVIS_READONLY);
		m_GridLoad.SetItemBkColour(nRow, 1, RGBREADONLY);
		nRow++;
	}

	m_GridLoad.SetRedraw(TRUE, TRUE);
}

void CCalcFrameComboLoadDlg::SetDataSave()
{
	// Combination Data Check
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long nFemSize = m_ctlTab.GetItemCount();

	for (long nFem=0; nFem<nFemSize; nFem++)
	{
		CFEMManage* pFem = pCalcStd->GetModelingFemData(nFem);		
		for (long nCombo=0; nCombo<pFem->m_ComboList.GetCount(); nCombo++)
		{
			POSITION pos = pFem->m_ComboList.FindIndex(nCombo);
			CCombo* pComb= (CCombo*)pFem->m_ComboList.GetAt(pos);
			long nLoadSize= pComb->m_szLoadName.GetSize();

			if (nLoadSize<1)
			{
				CString szCombo = pComb->m_szName;
				CString szMessage = _T("");
				szMessage.Format("Load Combination - %s \n 포함된 하중이 없습니다. 삭제하시겠습니까? ", szCombo);
				if (IDOK==MessageBox((LPCSTR)szMessage, "삭제 확인", MB_OKCANCEL | MB_ICONWARNING))
				{
					pFem->m_ComboList.RemoveAt(pos);
					nFem--;
				}
			}
		}
	}
}

void CCalcFrameComboLoadDlg::OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nSelectedCombo	= 0;
	
	SetDataInit();
}

BOOL CCalcFrameComboLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID CellCombo	= m_GridCombo.GetFocusCell();
		if(CellCombo.col==2)
		{
			strCombo.Add("ADD");
			strCombo.Add("ENVE");

			m_GridCombo.SetComboString(strCombo);
		}
	}

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CCalcFrameComboLoadDlg::OnButtonAddCombo() 
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();

	nCurSel = GetnTypeAnalysis(nCurSel);	
	
	CFEMManage      *pFEM =	pCalcStd->GetModelingFemData(nCurSel);
	CString			sText		=	_T("");

	sText.Format("USER%d",pFEM->m_ComboList.GetCount()+1);

	CCombo *pComb = new CCombo(sText, CCombo::ENEV, 1, TRUE);

	pFEM->m_ComboList.AddTail(pComb);

	SetDataInit();	
}

void CCalcFrameComboLoadDlg::OnButtonDeleteCombo() 
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	long			nSize		=	(long)0.0;
	CString			szText		=	_T("");
	CString			szText2		=	_T("");
	CStringArray	szArr;

	nCurSel = GetnTypeAnalysis(nCurSel);	
	
	CFEMManage	*pFEM =	pCalcStd->GetModelingFemData(nCurSel);
	POSITION	pos;
	CCombo		*pComb;
	CCombo		*pComb2;
	
	szText	=	m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,1);
	pos		=	pFEM->m_ComboList.GetHeadPosition();

	while(pos)
	{
		pComb2	=	(CCombo*)pFEM->m_ComboList.GetNext(pos);
		if(szText == pComb2->m_szName)
			break;
	}

	pos	=	pFEM->m_ComboList.GetHeadPosition();

	long nCount = 1;

	//현재 조합이 다른 곳에서 참조 되었는지를 체크함.
	while(pos)
	{
		pComb	= (CCombo*)pFEM->m_ComboList.GetNext(pos);

		nCount++;	
		
		if(m_nSelectedCombo > nCount) continue;
		
		nSize	= pComb->m_szLoadName.GetSize();
		
		for(long nLoad = 0; nLoad < nSize; nLoad++)
		{
			BOOL	bExist		=	FALSE;
			CString szTitle1	=	pComb->m_szLoadName[nLoad];
			CString szTitle2	=	pComb2->m_szName;
			CString	szCombo		=	pComb->m_szName;

			if(szTitle1 == szTitle2)
				bExist = TRUE;

			if(bExist)
				szArr.Add(szCombo);
		}
		
	}
	
	if(szArr.GetSize()>0)
	{
		CString	sz = _T("");

		for(long n=0; n<szArr.GetSize(); n++)
		{
			sz.Format("%s, ", szArr.GetAt(n));
			szText2 += sz;
		}
		
		sz.Format("\n%s는 위의 하중조합에서 사용중입니다. 먼저 위의 조합에서 제거해야 합니다.", pComb2->m_szName);
		
		szText2 += sz;

		AfxMessageBox(szText2);

		return;
	}

	if(m_nSelectedCombo!=0)
		m_nSelectedCombo--;

	m_GridCombo.SetFocusCell(m_nSelectedCombo+1, 1);
	
	
	if(ErrorMessage(3, pFEM->m_ComboList.GetCount())) return;

	POSITION PosBack;

	pos = pFEM->m_ComboList.GetHeadPosition();

	while(pos)
	{
		pComb2	=	(CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb2->m_szName)
			break;

		PosBack = pos;
	}
	
	//자료구조의 첫번째 pos는 GetNext실행 후 얻어 올 수 없음.
	if(pFEM->m_ComboList.GetCount()==1 || m_nSelectedCombo==0)
		PosBack = pFEM->m_ComboList.GetHeadPosition();

	if(PosBack == NULL) return;

	pFEM->m_ComboList.RemoveAt(PosBack);
	
	if(m_nSelectedCombo!=0)
		m_nSelectedCombo--;

	m_GridCombo.SetFocusCell(m_nSelectedCombo+1, 1);

	SetDataInit();
}
void CCalcFrameComboLoadDlg::OnCellChangedDataCombo(NMHDR* nmgv, LRESULT*)
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);
	
	CFEMManage	*pFEM =	pCalcStd->GetModelingFemData(nCurSel);
	
	long	nRow	= 1;
	CString	szText	= _T("");
	
	if(pFEM->m_ComboList.GetCount()==0)	return;
	
	POSITION pos = pFEM->m_ComboList.GetHeadPosition();
	
	while(pos)
	{
		CCombo *pComb = (CCombo *)pFEM->m_ComboList.GetNext(pos);
		szText.Format("%d", nRow);
		pComb->m_szName = m_GridCombo.GetTextMatrix(nRow, 1);
		
		if ( m_GridCombo.GetTextMatrix(nRow, 2) == "ADD")
			pComb->m_nEnvType	=	0;
		else
			pComb->m_nEnvType	=	1;

		pComb->m_szDescription	=	m_GridCombo.GetTextMatrix(nRow, 3);
		pComb->m_bApply			=	m_GridCombo.IsCheck(nRow, 4);

		nRow++;
	}

}

void CCalcFrameComboLoadDlg::OnCellChangedDataLoad(NMHDR* nmgv, LRESULT*)
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);
	
	CFEMManage	*pFEM =	pCalcStd->GetModelingFemData(nCurSel);
	CString		szText		=	_T("");
	
	szText = m_GridStd.GetTextMatrix(m_nSelectedCombo+1,1);
	
	POSITION pos	=	pFEM->m_ComboList.GetHeadPosition();
	CCombo *pComb	=	NULL;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb) return;
	
	CCellID CellID = m_GridLoad.GetFocusCell();
	
	pComb->m_dSafe.GetAt(CellID.row-1) = m_GridLoad.GetTextMatrixDouble(CellID.row,	2);
	
}

void CCalcFrameComboLoadDlg::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);

	NM_GRIDVIEW		*GridView	=	(NM_GRIDVIEW *)nmgv;
	int				nRow		=	GridView->iRow;
    
	m_GridLoad.ResetSelection();
	m_GridStd.ResetSelection();

	m_nSelectedCombo = nRow-1;

	SetGridTitleLoad();
	SetGridTitleStd();
	SetDataInitStd();	
	SetDataInitLoad();

	m_GridCombo.SetFocus();
}

BOOL CCalcFrameComboLoadDlg::ErrorMessage(long nErr, long nVal)
{
	BOOL bError	= FALSE;
	switch(nErr)
	{
	case 0 :	// 하중조합 선택이 항목이 없는경우
		{
			if(nVal>=0)	bError = FALSE;
			else
			{
				AfxMessageBox("하중조합을 선택하여 주십시요.");
				bError = TRUE;
			}
		}
		break;
	case 1 :	// 추가할 하중 선택이 없는경우
		{
			if(nVal>=0)	bError = FALSE;
			else
			{
				AfxMessageBox("추가할 하중을 선택하여 주십시요.");
				bError = TRUE;
			}
		}
		break;
	case 2 :	// 삭제할 comboLoad를 선택하지 않은경우
		{
			if(nVal>=0)	bError = FALSE;
			else
			{
				AfxMessageBox("제거할 하중을 선택하여 주십시요.");
				bError = TRUE;
			}
		}
		break;
	case 3 :
		{
			if(nVal>0)	bError = FALSE;
			else
			{
				AfxMessageBox("하중조합이 존재하지 않습니다.");
				bError = TRUE;
			}
		}
		break;
	}
	
	return bError;
}

void CCalcFrameComboLoadDlg::OnButtonInsertcombo() 
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);
	
	CFEMManage	*pFEM =	pCalcStd->GetModelingFemData(nCurSel);
	CString		szText		=	_T("");
	
	szText = m_GridStd.GetTextMatrix(m_nSelectedCombo+1,1);
	
	POSITION pos	=	pFEM->m_ComboList.GetHeadPosition();
	CCombo *pComb	=	NULL;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb) return;
	
	CCellID CellID = m_GridStd.GetFocusCell();
	long nRow = CellID.row;
	szText = m_GridStd.GetTextMatrix(nRow,1);
	
	if(szText.GetLength()<=0) return;

	pComb->m_szLoadName.Add(szText);
	pComb->m_dSafe.Add(1.0);
	
	long nIdx = nRow-1;
	long icLoadCase = pFEM->m_szLCName.GetSize();
	long icMovingLoad = pFEM->m_MovingLoadList.GetCount();
	if(nIdx < icLoadCase) //LOAD
	{
		pComb->m_bLive = FALSE;
		pComb->m_nType.Add(0);
	}
	else if(nIdx < icLoadCase+icMovingLoad) //MOVE
	{
		pComb->m_bLive = TRUE;
	}
	else //COMBO
	{
		pComb->m_bLive = FALSE;
		pComb->m_nType.Add(1);
	}
	SetGridTitleLoad();
	SetDataInitLoad();
}

void CCalcFrameComboLoadDlg::OnButtonRemovecombo() 
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);
	
	CFEMManage	*pFEM	=	pCalcStd->GetModelingFemData(nCurSel);
	CCombo		*pComb		=	NULL;
	POSITION	pos			=	pFEM->m_ComboList.GetHeadPosition();
	CString		szText		=	_T("");
	long		nCombo		=	m_nSelectedCombo;
	long		nComboLoad	=	m_GridLoad.GetFocusCell().row-1;

	if(ErrorMessage(0, nCombo))	return;
	if(ErrorMessage(2, nComboLoad))	return;

	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb) return;
	
	CCellID CellID	=	m_GridLoad.GetFocusCell();
	long	nCurRow	=	CellID.row;
	
	pComb->m_szLoadName.RemoveAt(nCurRow-1);
	pComb->m_dSafe.RemoveAt(nCurRow-1);
	
	if(nCurSel != LIVE_LOAD_ANALYSIS)
		pComb->m_nType.RemoveAt(nCurRow-1);

	SetGridTitleLoad();
	SetDataInitLoad();
	
}

void CCalcFrameComboLoadDlg::OnButtonUpper() 
{
	if(m_nSelectedCombo < 0 ) return;

	CCellID CellLoad	=	m_GridLoad.GetFocusCell();
	CCellID	CellCombo	=	m_GridCombo.GetFocusCell();

	if(CellLoad.row < 1)	//Combo
	{
		if(SwapCombo(TRUE))
			m_GridCombo.SetFocusCell(CellCombo.row-1, CellCombo.col);
		SetDataInitCombo();
	}
	else	//Load
	{
		
	}
}
BOOL CCalcFrameComboLoadDlg::SwapCombo(BOOL bUpper)
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	long			nCurSel		=	m_ctlTab.GetCurSel();
	
	nCurSel = GetnTypeAnalysis(nCurSel);

	CFEMManage	*pFEM	=	pCalcStd->GetModelingFemData(nCurSel);

	if(bUpper	&& m_nSelectedCombo	< 1)										return FALSE;
	if(!bUpper	&& m_nSelectedCombo	> pFEM->m_ComboList.GetCount() - 2)	return FALSE;	
	
	long	nIdx1		=	m_nSelectedCombo + 1;
	long	nIdx2		=	nIdx1 + (bUpper? -1 : 1);	
	CString	szText		=	_T("");
	CString	szText2		=	_T("");
	CCombo	*pComb		=	NULL;
	CCombo	*pComb2		=	NULL;	
	CCombo	*pComb3		=	NULL;
	POSITION pos		=	pFEM->m_ComboList.GetHeadPosition();
	POSITION posBack	=	NULL;

	szText	=	m_GridCombo.GetTextMatrix(nIdx1,	1);
	
	//bUpper가 true이면 이전의 행을, false이면 다음의 행의 키 값을 가져 옴.
	szText2	=	m_GridCombo.GetTextMatrix(nIdx2,	1);	

	while(pos)
	{
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
	
		if(szText == pComb->m_szName)
			break;
	}
	
	pos = pFEM->m_ComboList.GetHeadPosition();
	
	//Key 값 찾기 작업
	while(pos)
	{
		pComb2 = (CCombo*)pFEM->m_ComboList.GetNext(pos);

		if(szText2 == pComb2->m_szName)
			break;
	}

	if(bUpper)
	{
		for(long n = 0; n < pComb->m_szLoadName.GetSize(); n++)
		{
			if(pComb->m_szLoadName[n] == pComb2->m_szName)
				return FALSE;
		}
	}
	else
	{
		for(long n = 0; n < pComb2->m_szLoadName.GetSize(); n++)
		{
			if(pComb2->m_szLoadName[n] == pComb->m_szName)
				return FALSE;
		}

	}
	
	pos = pFEM->m_ComboList.GetHeadPosition();
	
	//위치 변경 작업
	while(pos)
	{
		posBack = pos;
		pComb3 = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb3->m_szName)
		{
			pFEM->m_ComboList.SetAt(posBack,pComb2);

			if(bUpper)
			{
				pFEM->m_ComboList.GetPrev(posBack);
				pFEM->m_ComboList.SetAt(posBack, pComb);
			}
			else
			{
				pFEM->m_ComboList.GetNext(posBack);
				pFEM->m_ComboList.SetAt(posBack, pComb);
			}
			break;
		}
		
	}

	return TRUE;
}
long CCalcFrameComboLoadDlg::GetnTypeAnalysis(long nCurSel)
{
	CPlateBridgeApp *pDB		=	m_pStd->GetBridge();

	if(nCurSel == 0)
		nCurSel = pDB->IsTUGir() ? BEFORE_TURN_COMPOSITE_ANALYSIS : BEFORE_COMPOSITE_ANALYSIS;
	else
		nCurSel += 1;	
	
	return nCurSel;
}

void CCalcFrameComboLoadDlg::OnButtonLower() 
{
	CCellID CellLoad	=	m_GridLoad.GetFocusCell();
	CCellID	CellCombo	=	m_GridCombo.GetFocusCell();

	if(CellLoad.row<1)	// Combo
	{
		if(SwapCombo(FALSE))
			m_GridCombo.SetFocusCell(CellCombo.row+1, CellCombo.col);
		SetDataInitCombo();
	}
}
BOOL CCalcFrameComboLoadDlg::SwapLoad(CCombo* pCombo, long nIdx1, BOOL bUpper)
{
	if(bUpper	&& nIdx1 < 1)									return FALSE;
	if(!bUpper	&& nIdx1 > pCombo->m_szLoadName.GetSize()-2)	return FALSE;

	return TRUE;
}
