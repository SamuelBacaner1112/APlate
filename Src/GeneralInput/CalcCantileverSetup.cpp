// CalcCantileverSetup.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverSetup dialog


CCalcCantileverSetup::CCalcCantileverSetup(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcCantileverSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDrawType	= 0;

	m_GridGen.SetDefaultCharacterCount(2);
	m_GridLoad.SetDefaultCharacterCount(2);
	m_GridAdd.SetDefaultCharacterCount(2);
	m_GridWalk.SetDefaultCharacterCount(2);
}


void CCalcCantileverSetup::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverSetup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_GEN, m_GridGen);
	DDX_GridControl(pDX, IDC_GRID_LOAD, m_GridLoad);
	DDX_GridControl(pDX, IDC_GRID_ADD, m_GridAdd);
	DDX_GridControl(pDX, IDC_GRID_WALKLOAD, m_GridWalk);
}

BEGIN_MESSAGE_MAP(CCalcCantileverSetup, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverSetup)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,	IDC_GRID_GEN, OnCellChangedDataGen)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,	IDC_GRID_LOAD, OnCellChangedDataLoad)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,	IDC_GRID_WALKLOAD, OnCellChangedDataWalkLoad)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,	IDC_GRID_ADD, OnCellChangedDataAdditiveLoad)
	ON_NOTIFY(GVN_CELL_BUTTONCLICK,		IDC_GRID_ADD, OnCellButtonClickAdditiveLoad)
	ON_NOTIFY(GVN_CELL_CHANGED,			IDC_GRID_LOAD, OnCellChangeLoad)
	ON_NOTIFY(GVN_CELL_CHANGED,			IDC_GRID_WALKLOAD, OnCellChangeWalkLoad)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverSetup message handlers

void CCalcCantileverSetup::OnPreInitDialog()
{
	SetResize(IDC_GRID_GEN, SZ_TOP_LEFT, SZ_MIDDLE_CENTER);
	SetResize(IDC_GRID_LOAD, SZ_TOP_CENTER, SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID_ADD, SZ_MIDDLE_LEFT, SZ_BOTTOM_CENTER);
	SetResize(IDC_GRID_WALKLOAD, SZ_MIDDLE_CENTER, SZ_BOTTOM_RIGHT);
}

void CCalcCantileverSetup::SetGridTitle()
{
	SetGridTitleGen();
	SetGridTitleLoad();
	SetGridTitleAdd();
	SetGridTitleWalkLoad();
}

void CCalcCantileverSetup::	SetGridTitleGen()
{
	long nRowCnt = 11;
	long nColCnt = 4;
	m_GridGen.ResetAttrib();
	m_GridGen.SetRowCount(nRowCnt);
	m_GridGen.SetColumnCount(nColCnt);
	m_GridGen.SetEditable(TRUE);
	m_GridGen.SetListMode(FALSE);
	m_GridGen.EnableDragAndDrop(TRUE);
	m_GridGen.EnableInsertRow(FALSE);
	m_GridGen.EnableInsertCol(FALSE);	
	m_GridGen.SetTextBkColor(RGB(225, 250, 250));
	m_GridGen.EnableReturnPass(TRUE);
	m_GridGen.SetRowHeightAll(22);
	m_GridGen.SetColumnWidth(0,120);
	m_GridGen.SetColumnWidth(1,100);
	m_GridGen.SetColumnWidth(2,100);
	m_GridGen.SetColumnWidth(3,100);

	for(long n=0; n<nRowCnt; n++)
		m_GridGen.SetCellFixType(n, 0, GVN_ONECELL_COLFIX);

	long nRow = 0;
	m_GridGen.SetTextMatrix(nRow, 0, "무근콘크리트 단위중량적용");	m_GridGen.SetMergeCol(nRow, 0, 1);
	m_GridGen.SetTextMatrix(nRow, 2, "방호벽");						m_GridGen.SetCellType(nRow, 2, CT_CHECKBOX);
	m_GridGen.SetTextMatrix(nRow, 3, "중분대");						m_GridGen.SetCellType(nRow, 3, CT_CHECKBOX);

	nRow++;
	m_GridGen.SetTextMatrix(nRow, 0, "특수하중 적용 (바닥판, 주형해석)");	m_GridGen.SetMergeCol(nRow, 0, 1);
	m_GridGen.SetTextMatrix(nRow, 2, "탱크");							m_GridGen.SetCellType(nRow, 2, CT_CHECKBOX);
	m_GridGen.SetTextMatrix(nRow, 3, "트레일러");						m_GridGen.SetCellType(nRow, 3, CT_CHECKBOX);

	nRow++;
	m_GridGen.SetTextMatrix(nRow, 0, "설계속도 (km/hr)");	m_GridGen.SetMergeCol(nRow, 0, 1);	m_GridGen.SetMergeCol(nRow, 2, 3);

	nRow++;
	m_GridGen.SetTextMatrix(nRow, 0, "풍하중 강도(kN/m²) 도.설.기 2.1.11");	m_GridGen.SetMergeCol(nRow, 0, 1);	m_GridGen.SetMergeCol(nRow, 2, 3);

	nRow++;
	m_GridGen.SetTextMatrix(nRow, 0, "원심하중반경 (m)");		m_GridGen.SetMergeRow(nRow, nRow+1, 0);
	m_GridGen.SetTextMatrix(nRow, 1, "좌측");					m_GridGen.SetCellFixType(nRow, 1, GVN_ONECELL_ROWFIX);
	m_GridGen.SetTextMatrix(nRow, 2, "중앙");					m_GridGen.SetCellFixType(nRow, 2, GVN_ONECELL_ROWFIX);
	m_GridGen.SetTextMatrix(nRow, 3, "우측");					m_GridGen.SetCellFixType(nRow, 3, GVN_ONECELL_ROWFIX);

	nRow += 2;
	m_GridGen.SetTextMatrix(nRow, 0, "철근피복두께 (mm)");		m_GridGen.SetMergeRow(nRow, nRow+1, 0);
	m_GridGen.SetTextMatrix(nRow, 1, "상면");					m_GridGen.SetCellFixType(nRow, 1, GVN_ONECELL_ROWFIX);
	m_GridGen.SetTextMatrix(nRow, 2, "하면");					m_GridGen.SetCellFixType(nRow, 2, GVN_ONECELL_ROWFIX);
	m_GridGen.SetTextMatrix(nRow, 3, "측면");					m_GridGen.SetCellFixType(nRow, 3, GVN_ONECELL_ROWFIX);

	nRow += 2;
	m_GridGen.SetTextMatrix(nRow, 0, "활하중 재하방법"); 		m_GridGen.SetMergeRow(nRow, nRow+2, 0);
	m_GridGen.SetTextMatrix(nRow, 1, "표준점유효폭(3m)마다 연속으로 재하");	
	m_GridGen.SetCellType(nRow, 1, CT_CHECKBOX);	
	m_GridGen.SetMergeCol(nRow, 1, 3);

	nRow++;
	m_GridGen.SetTextMatrix(nRow, 1, "설계차로폭을 고려하여 재하");		
	m_GridGen.SetCellType(nRow, 1, CT_CHECKBOX);	
	m_GridGen.SetMergeCol(nRow, 1, 3);

	nRow++;
	m_GridGen.SetTextMatrix(nRow, 1, "설계차로폭 사용자 정의 (m) ");			
	m_GridGen.SetCellType(nRow, 1, CT_CHECKBOX);	
	m_GridGen.SetMergeCol(nRow, 1, 2);

	m_GridGen.SetRedraw(TRUE, TRUE);
}

void CCalcCantileverSetup::SetGridTitleLoad()
{
	long nRow = 8;
	long nCol = 5;
	m_GridLoad.ResetAttrib();
	m_GridLoad.SetRowCount(nRow);
	m_GridLoad.SetColumnCount(nCol);
	m_GridLoad.SetEditable(TRUE);
	m_GridLoad.SetListMode(FALSE);
	m_GridLoad.EnableDragAndDrop(TRUE);
	m_GridLoad.EnableInsertRow(FALSE);
	m_GridLoad.EnableInsertCol(FALSE);	
	m_GridLoad.SetTextBkColor(RGB(225, 250, 250));
	m_GridLoad.EnableReturnPass(TRUE);
	m_GridLoad.SetRowHeightAll(22);
	m_GridLoad.SetColumnWidth(0,90);
	m_GridLoad.SetColumnWidth(1,70);
	m_GridLoad.SetColumnWidth(2,70);
	m_GridLoad.SetColumnWidth(3,60);	
	m_GridLoad.SetColumnWidth(4,120);	

	for(long n=0;n<nRow;n++)
		m_GridLoad.SetCellFixType(n, 0, GVN_ONECELL_COLFIX);
	
	m_GridLoad.SetCellFixType(0, 3, GVN_ONECELL_ROWFIX);	m_GridLoad.SetCellFixType(0, 4, GVN_ONECELL_ROWFIX);
	m_GridLoad.SetCellFixType(4, 3, GVN_ONECELL_ROWFIX);	m_GridLoad.SetCellFixType(4, 4, GVN_ONECELL_ROWFIX);

	m_GridLoad.SetTextMatrix(0, 0, "방음벽");
	m_GridLoad.SetTextMatrix(0, 1, "좌측");	m_GridLoad.SetCellType(0, 1, CT_CHECKBOX); m_GridLoad.SetCellFixType(0, 1, GVN_ONECELL_ROWFIX);
	m_GridLoad.SetTextMatrix(0, 2, "우측");	m_GridLoad.SetCellType(0, 2, CT_CHECKBOX); m_GridLoad.SetCellFixType(0, 2, GVN_ONECELL_ROWFIX);
	m_GridLoad.SetTextMatrix(0, 3, "단위");
	m_GridLoad.SetTextMatrix(0, 4, "비고");
	m_GridLoad.SetTextMatrix(1, 0, "자중");	m_GridLoad.SetTextMatrix(1, 3, "kN");
	m_GridLoad.SetTextMatrix(2, 0, "높이");	m_GridLoad.SetTextMatrix(2, 3, "m");	m_GridLoad.SetTextMatrix(2, 4, "방호벽포함");
	m_GridLoad.SetTextMatrix(3, 0, "재하거리");m_GridLoad.SetTextMatrix(3, 3, "m");	m_GridLoad.SetTextMatrix(3, 4, "양단부로부터거리");
	m_GridLoad.SetTextMatrix(4, 0, "난간");
	m_GridLoad.SetTextMatrix(4, 1, "좌측");	m_GridLoad.SetCellType(4, 1, CT_CHECKBOX); m_GridLoad.SetCellFixType(4, 1, GVN_ONECELL_ROWFIX);
	m_GridLoad.SetTextMatrix(4, 2, "우측");	m_GridLoad.SetCellType(4, 2, CT_CHECKBOX); m_GridLoad.SetCellFixType(4, 2, GVN_ONECELL_ROWFIX);
	m_GridLoad.SetTextMatrix(4, 3, "단위");
	m_GridLoad.SetTextMatrix(4, 4, "비고");
	m_GridLoad.SetTextMatrix(5, 0, "자중");	m_GridLoad.SetTextMatrix(5, 3, "kN");
	m_GridLoad.SetTextMatrix(6, 0, "높이");	m_GridLoad.SetTextMatrix(6, 3, "m");	m_GridLoad.SetTextMatrix(6, 4, "방호벽포함");
	m_GridLoad.SetTextMatrix(7, 0, "재하거리");m_GridLoad.SetTextMatrix(7, 3, "m");	m_GridLoad.SetTextMatrix(7, 4, "양단부로부터거리");

	m_GridLoad.SetRedraw(TRUE, TRUE);


	for(long nR=1; nR<nRow;nR++)
	{
		m_GridLoad.SetItemState(nR, 3, GVIS_READONLY);
		m_GridLoad.SetItemState(nR, 4, GVIS_READONLY);
	}
}

void CCalcCantileverSetup::SetGridTitleAdd()
{
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();

	long nRow = pData->m_ELoadTitleArr.GetSize()+1;
	long nCol = 4;
	m_GridAdd.ResetAttrib();
	m_GridAdd.SetRowCount(nRow);
	m_GridAdd.SetColumnCount(nCol);
	m_GridAdd.SetEditable(TRUE);
	m_GridAdd.SetListMode(FALSE);
	m_GridAdd.EnableDragAndDrop(TRUE);
	m_GridAdd.EnableInsertRow(FALSE);
	m_GridAdd.EnableInsertCol(FALSE);	
	m_GridAdd.SetTextBkColor(RGB(225, 250, 250));
	m_GridAdd.EnableReturnPass(TRUE);
	m_GridAdd.SetRowHeightAll(22);
	m_GridAdd.SetColumnWidth(0,95);
	m_GridAdd.SetColumnWidth(1,130);
	m_GridAdd.SetColumnWidth(2,105);
	m_GridAdd.SetColumnWidth(3,65);
	m_GridAdd.SetTextMatrix(0, 0, "부가하중추가");		m_GridAdd.SetCellType(0, 0, CT_BUTTON);
	m_GridAdd.SetItemBkColour(0, 0, RGB(210,210,210));
	m_GridAdd.SetTextMatrix(0, 1, "좌측끝단부터거리(m)");
	m_GridAdd.SetTextMatrix(0, 2, "하중크기(kN/m)");
	m_GridAdd.SetTextMatrix(0, 3, "하중삭제");	

	for(long n=1;n<nCol;n++)
		m_GridAdd.SetCellFixType(0, n, GVN_ONECELL_ROWFIX);

	m_GridAdd.SetRedraw(TRUE,TRUE);
}

void CCalcCantileverSetup::SetGridTitleWalkLoad()
{
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();

	long nRows = 7;
	long nCols = 5;
	m_GridWalk.ResetAttrib();
	m_GridWalk.SetRowCount(nRows);
	m_GridWalk.SetColumnCount(nCols);
	m_GridWalk.SetEditable(TRUE);
	m_GridWalk.SetListMode(FALSE);
	m_GridWalk.EnableDragAndDrop(TRUE);
	m_GridWalk.EnableInsertRow(FALSE);
	m_GridWalk.EnableInsertCol(FALSE);	
	m_GridWalk.SetTextBkColor(RGB(225, 250, 250));
	m_GridWalk.EnableReturnPass(TRUE);
	m_GridWalk.SetRowHeightAll(22);
	m_GridWalk.SetColumnWidth(0,90);
	m_GridWalk.SetColumnWidth(1,70);
	m_GridWalk.SetColumnWidth(2,70);
	m_GridWalk.SetColumnWidth(3,60);
	m_GridWalk.SetColumnWidth(4,100);
	
	for(long n=0;n<nRows;n++)
		m_GridWalk.SetCellFixType(n, 0, GVN_ONECELL_COLFIX);
	m_GridWalk.SetCellFixType(0, 3, GVN_ONECELL_ROWFIX);
	m_GridWalk.SetCellFixType(0, 4, GVN_ONECELL_ROWFIX);

	m_GridWalk.SetTextMatrix(0, 0, "보도자중");
	m_GridWalk.SetTextMatrix(0, 1, "좌측");	m_GridWalk.SetCellType(0, 1, CT_CHECKBOX);	m_GridWalk.SetCellFixType(0, 1, GVN_ONECELL_ROWFIX);
	m_GridWalk.SetTextMatrix(0, 2, "우측");	m_GridWalk.SetCellType(0, 2, CT_CHECKBOX);	m_GridWalk.SetCellFixType(0, 2, GVN_ONECELL_ROWFIX);
	m_GridWalk.SetTextMatrix(0, 3, "단위");
	m_GridWalk.SetTextMatrix(0, 4, "비고");

	m_GridWalk.SetTextMatrix(1, 0, "T1(좌측)");		m_GridWalk.SetTextMatrix(1, 3, "kN/m²");	m_GridWalk.SetTextMatrix(1, 4, "단위중량×높이");
	m_GridWalk.SetTextMatrix(2, 0, "T2(우측)");		m_GridWalk.SetTextMatrix(2, 3, "kN/m²");	m_GridWalk.SetTextMatrix(2, 4, "단위중량×높이");
	m_GridWalk.SetTextMatrix(3, 0, "H1");		m_GridWalk.SetTextMatrix(3, 3, "m");
	m_GridWalk.SetTextMatrix(4, 0, "수평력");	m_GridWalk.SetMergeCol(4, 1, 2);	m_GridWalk.SetTextMatrix(4, 3, "kN/m");
	m_GridWalk.SetTextMatrix(5, 0, "수직력");	m_GridWalk.SetMergeCol(5, 1, 2);	m_GridWalk.SetTextMatrix(5, 3, "kN/m");

	m_GridWalk.SetTextMatrix(6, 0, "군집하중");
	m_GridWalk.SetTextMatrix(6, 1, "좌측");	m_GridWalk.SetCellType(6, 1, CT_CHECKBOX);
	m_GridWalk.SetTextMatrix(6, 2, "우측");	m_GridWalk.SetCellType(6, 2, CT_CHECKBOX);
	m_GridWalk.SetTextMatrix(6, 4, "활하중");

	BOOL	bLeft	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad;
	BOOL	bRight	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad;
	m_GridWalk.SetItemState(0, 1, bLeft ? GVIS_MODIFIED : GVIS_READONLY);
	m_GridWalk.SetItemState(0, 2, bRight ? GVIS_MODIFIED : GVIS_READONLY);

	m_GridWalk.SetRedraw(TRUE, TRUE);

	for(long nR=1; nR<nRows;nR++)
	{
		m_GridWalk.SetItemState(nR, 3, GVIS_READONLY);
		m_GridWalk.SetItemState(nR, 4, GVIS_READONLY);
	}
}

void CCalcCantileverSetup::SetDataInit()
{
	SetGridTitle();
	SetCellReadOnly(BANGEUM);
	SetCellReadOnly(NANGAN);
	SetCellReadOnly(WALKLOAD);
	SetCellReadOnly(PEOPLELOAD);

	SetDataInitGen();
	SetDataInitLoad();
	SetDataInitAdd();
	SetDataInitWalkLoad();
}

void CCalcCantileverSetup::SetDataInitGen()
{
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();

	CString	szText	= _T("");

	long nRow = 0;	// 무근콘크리트 단위중량 적용
	m_GridGen.SetCheck(nRow, 2, pData->m_bMugun);
	m_GridGen.SetCheck(nRow, 3, pData->m_bJungbunMugun);

	nRow++;		// 특수하중 적용 (바닥판, 주형해석)
	m_GridGen.SetCheck(nRow, 2, pData->m_bTank);
	m_GridGen.SetCheck(nRow, 3, pData->m_bTrailer);

	nRow++;		// 설계속도 v
	m_GridGen.SetTextMatrix(nRow, 2, "%.3f", pData->m_dSpeed);

	nRow++;		// 풍하중 강도
	m_GridGen.SetTextMatrix(nRow, 2, "%.3f", pData->m_dWindLoadForce);

	nRow += 2;	// 원심하중반경
	if(pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Cent_R>0.0)
		szText.Format("%.3f", pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Cent_R);
	else
		szText.Format("∞");
	m_GridGen.SetTextMatrix(nRow, 1, szText);
	if(pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R>0.0)
		szText.Format("%.3f", pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R);
	else
		szText.Format("∞");
	m_GridGen.SetTextMatrix(nRow, 2, szText);
	if(pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Cent_R>0.0)
		szText.Format("%.3f", pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Cent_R);
	else
		szText.Format("∞");
	m_GridGen.SetTextMatrix(nRow, 3, szText);

	nRow += 2;	// 철근피복두깨
	m_GridGen.SetTextMatrix(nRow, 1, "%.0f", pData->m_dCoverUp);
	m_GridGen.SetTextMatrix(nRow, 2, "%.0f", pData->m_dCoverDn);
	m_GridGen.SetTextMatrix(nRow, 3, "%.0f", pData->m_dCoverSd);

	BOOL bLiveLoadCheck[3] = { FALSE, FALSE, FALSE };
	bLiveLoadCheck[pData->m_nHLiveLoadType] = TRUE;

	nRow++;
	m_GridGen.SetCheck(nRow, 1, bLiveLoadCheck[0]);
	nRow++;
	m_GridGen.SetCheck(nRow, 1, bLiveLoadCheck[1]);
	nRow++;
	m_GridGen.SetCheck(nRow, 1, bLiveLoadCheck[2]);

	if (!bLiveLoadCheck[2]) m_GridGen.SetItemState(nRow, 3, GVIS_READONLY);
	else m_GridGen.SetItemState(nRow, 3, GVIS_MODIFIED);

	m_GridGen.SetTextMatrix(nRow, 3, "%.3f", pData->m_dUserLaneWidth);
	m_GridGen.SetRedraw(TRUE, TRUE);
}

void CCalcCantileverSetup::SetDataInitLoad()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	
	m_GridLoad.SetCheck(0, 1, pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm);
	m_GridLoad.SetCheck(0, 2, pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm);
	m_GridLoad.SetTextMatrix(1, 1, "%.3f", tokN(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight));
	m_GridLoad.SetTextMatrix(1, 2, "%.3f", tokN(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight));
	m_GridLoad.SetTextMatrix(2, 1, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight));
	m_GridLoad.SetTextMatrix(2, 2, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight));
	m_GridLoad.SetTextMatrix(3, 1, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis));
	m_GridLoad.SetTextMatrix(3, 2, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis));

	m_GridLoad.SetCheck(4, 1, pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan);
	m_GridLoad.SetCheck(4, 2, pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan);
	m_GridLoad.SetTextMatrix(5, 1, "%.3f", tokN(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight));
	m_GridLoad.SetTextMatrix(5, 2, "%.3f", tokN(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight));
	m_GridLoad.SetTextMatrix(6, 1, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight));
	m_GridLoad.SetTextMatrix(6, 2, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanHeight));
	m_GridLoad.SetTextMatrix(7, 1, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis));
	m_GridLoad.SetTextMatrix(7, 2, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis));
	m_GridLoad.SetRedraw(TRUE, TRUE);
}

void CCalcCantileverSetup::SetDataInitAdd()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	for(long nRow=0; nRow<pData->m_ELoadTitleArr.GetSize(); nRow++)
	{
		m_GridAdd.SetTextMatrix(nRow+1, 0, pData->m_ELoadTitleArr.GetAt(nRow));
		m_GridAdd.SetTextMatrix(nRow+1, 1, "%.3f", toM(pData->m_ELoadDisArr.GetAt(nRow)));
		m_GridAdd.SetTextMatrix(nRow+1, 2, "%.3f", tokN(pData->m_ELoadWeightArr.GetAt(nRow)));
		m_GridAdd.SetTextMatrix(nRow+1, 3, "삭제");	m_GridAdd.SetCellType(nRow+1, 3, CT_BUTTON);
	}
	m_GridAdd.SetRedraw(TRUE, TRUE);
}

void CCalcCantileverSetup::SetDataInitWalkLoad()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	m_GridWalk.SetCheck(0, 1, pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad);
	m_GridWalk.SetCheck(0, 2, pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad);
	m_GridWalk.SetTextMatrix(1, 1, "%.3f", tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1));
	m_GridWalk.SetTextMatrix(2, 1, "%.3f", tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2));
	m_GridWalk.SetTextMatrix(3, 1, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH));
	m_GridWalk.SetTextMatrix(4, 1, "%.3f", tokNPM(pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_HoriF));
	m_GridWalk.SetTextMatrix(5, 1, "%.3f", tokNPM(pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_VerF));
	m_GridWalk.SetTextMatrix(1, 2, "%.3f", tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1));
	m_GridWalk.SetTextMatrix(2, 2, "%.3f", tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2));
	m_GridWalk.SetTextMatrix(3, 2, "%.3f", toM(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH));
	m_GridWalk.SetCheck(6, 1, pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bPeopleLoad);
	m_GridWalk.SetCheck(6, 2, pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bPeopleLoad);
	m_GridWalk.SetRedraw(TRUE, TRUE);
}

void CCalcCantileverSetup::SetDataSave()
{
	SetDataSaveGen();
	SetDataSaveLoad();
	SetDataSaveWalkLoad();
	SetDataSaveAdditiveLoad();
}

void CCalcCantileverSetup::SetDataSaveGen()
{
	CCalcData	*pData		= m_pStd->GetDataManage()->GetCalcData();
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	long nRow = 0;
	pData->m_bMugun			= m_GridGen.IsCheck(nRow, 2);
	pData->m_bJungbunMugun	= m_GridGen.IsCheck(nRow, 3);

	nRow++;
	pData->m_bTank			= m_GridGen.IsCheck(nRow, 2);
	pData->m_bTrailer		= m_GridGen.IsCheck(nRow, 3);

	nRow++;	// 설계속도
	pData->m_dSpeed			= m_GridGen.GetTextMatrixDouble(nRow, 2);

	nRow++;	// 풍하중 강도
	pData->m_dWindLoadForce	= m_GridGen.GetTextMatrixDouble(nRow, 2);

	nRow += 2;	// 원심하중 반경
	if(m_GridGen.GetTextMatrix(nRow, 1)=="∞")	pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Cent_R	= 0;
	else										pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Cent_R	= m_GridGen.GetTextMatrixDouble(nRow, 1);
	if(m_GridGen.GetTextMatrix(nRow, 2)=="∞")	pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R		= 0;
	else										pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R		= m_GridGen.GetTextMatrixDouble(nRow, 2);
	if(m_GridGen.GetTextMatrix(nRow, 3)=="∞")	pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Cent_R	= 0;
	else										pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Cent_R	= m_GridGen.GetTextMatrixDouble(nRow, 3);

	nRow+=2;
	pData->m_dCoverUp	= m_GridGen.GetTextMatrixDouble(nRow, 1);
	pData->m_dCoverDn	= m_GridGen.GetTextMatrixDouble(nRow, 2);
	pData->m_dCoverSd	= m_GridGen.GetTextMatrixDouble(nRow, 3);

	pDeckData->m_dCoverUp	= pData->m_dCoverUp;
	pDeckData->m_dCoverDn	= pData->m_dCoverDn;
	pDeckData->m_dCoverSd	= pData->m_dCoverSd;

	BOOL bLiveType[3] = { FALSE, FALSE, FALSE };
	nRow++;
	bLiveType[0] = m_GridGen.IsCheck(nRow, 1);
	nRow++;
	bLiveType[1] = m_GridGen.IsCheck(nRow, 1);
	nRow++;
	bLiveType[2] = m_GridGen.IsCheck(nRow, 1);
	
	long nType = (bLiveType[0])? 0 : (bLiveType[1])? 1: 2;
	pData->m_nHLiveLoadType = nType;
	pData->m_dUserLaneWidth = m_GridGen.GetTextMatrixDouble(nRow,3);
}

void CCalcCantileverSetup::SetDataSaveLoad()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	BOOL	bLeft	= m_GridLoad.IsCheck(0, 1);
	BOOL	bRight	= m_GridLoad.IsCheck(0, 2);

	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm			= bLeft;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm			= bRight;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight	= frkN(m_GridLoad.GetTextMatrixDouble(1, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight	= frM(m_GridLoad.GetTextMatrixDouble(2, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis		= frM(m_GridLoad.GetTextMatrixDouble(3, 1));

	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight	= frkN(m_GridLoad.GetTextMatrixDouble(1, 2));
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight	= frM(m_GridLoad.GetTextMatrixDouble(2, 2));
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis		= frM(m_GridLoad.GetTextMatrixDouble(3, 2));

	bLeft	= m_GridLoad.IsCheck(4, 1);
	bRight	= m_GridLoad.IsCheck(4, 2);
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan			= bLeft;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan			= bRight;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight	= frkN(m_GridLoad.GetTextMatrixDouble(5, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight	= frM(m_GridLoad.GetTextMatrixDouble(6, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis		= frM(m_GridLoad.GetTextMatrixDouble(7, 1));

	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight	= frkN(m_GridLoad.GetTextMatrixDouble(5, 2));
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanHeight	= frM(m_GridLoad.GetTextMatrixDouble(6, 2));
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis		= frM(m_GridLoad.GetTextMatrixDouble(7, 2));
}

void CCalcCantileverSetup::SetDataSaveWalkLoad()
{
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir   = NULL;

	BOOL	bLeft	= m_GridWalk.IsCheck(0, 1);
	BOOL	bRight	= m_GridWalk.IsCheck(0, 2);
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad		= bLeft;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1		= frkNPM2(m_GridWalk.GetTextMatrixDouble(1, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2		= frkNPM2(m_GridWalk.GetTextMatrixDouble(2, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH		= frM(m_GridWalk.GetTextMatrixDouble(3, 1));
	pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_HoriF	= frkNPM(m_GridWalk.GetTextMatrixDouble(4, 1));
	pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_VerF	= frkNPM(m_GridWalk.GetTextMatrixDouble(5, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bPeopleLoad		= m_GridWalk.IsCheck(6, 1);

	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad		= bRight;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1		= frkNPM2(m_GridWalk.GetTextMatrixDouble(1, 2));
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2		= frkNPM2(m_GridWalk.GetTextMatrixDouble(2, 2));
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH		= frM(m_GridWalk.GetTextMatrixDouble(3, 2));
	pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_HoriF	= frkNPM(m_GridWalk.GetTextMatrixDouble(4, 1));
	pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_VerF	= frkNPM(m_GridWalk.GetTextMatrixDouble(5, 1));
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bPeopleLoad		= m_GridWalk.IsCheck(6, 2);

	if(bLeft)
	{
		pGir	= pDB->GetGirder(0);
		CPlateBasicIndex *pBx    = pGir->GetCrossBxAtMinFlangeWidth();

		double Term     = pDB->m_dWidthSlabLeft;
		double dLen		= pDB->GetLengthHDanDom(pBx, 0);
		double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;		
		double dWebT    = pGir->GetThickFactByStation(pBx->GetStation(), G_W);

		double dLoad1 = pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
		double dLoad2 = pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2;
		double dBodoMomentL = (Term-dLen-dQuater-dWebT/2);//보도 시작점부터 모멘트 기준선까지의 거리...
		double T2 = dLoad1 + (dBodoMomentL) / pDB->GetLengthHDanDom(pBx, 1) * (dLoad2 - dLoad1);
		
		pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment = T2;
	}
	else
	{
		pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment = 0.0;
	}

	if (bRight)
	{
		pGir    = pDB->GetGirder(pDB->GetGirdersu()-1);		
		CPlateBasicIndex *pBx    = pGir->GetCrossBxAtMinFlangeWidth();

		long nSize		= pDB->GetQtyHDanNode();
		double Term     = pDB->m_dWidthSlabRight;
		double dLen		= pDB->GetLengthHDanDom(pBx, nSize-1);
		double dQuater  = pGir->GetLengthSharpOnBx(pBx, TRUE)/2;		
		double dWebT    = pGir->GetThickFactByStation(pBx->GetStation(), G_W);

		double dLoad1 = pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double dLoad2 = pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2;
		double dBodoMomentL = (Term-dLen-dQuater-dWebT/2);//보도 시작점부터 모멘트 기준선까지의 거리...
		double T2 = dLoad1 + (dBodoMomentL) / pDB->GetLengthHDanDom(pBx, nSize-2) * (dLoad2 - dLoad1);
		
		pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment = T2;
	}
	else
	{
		pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment = 0.0;
	}
}

void CCalcCantileverSetup::SetDataSaveAdditiveLoad()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	BOOL	bExtraLoad	= FALSE;

	for(long nRow=1; nRow<pData->m_ELoadTitleArr.GetSize()+1; nRow++)
	{
		if(m_GridAdd.GetTextMatrix(nRow,0) != _T(""))
		{
			pData->m_ELoadTitleArr.SetAt(nRow-1, m_GridAdd.GetTextMatrix(nRow,0));
			pData->m_ELoadDisArr.SetAt(nRow-1, frM(m_GridAdd.GetTextMatrixDouble(nRow,1)));
			pData->m_ELoadWeightArr.SetAt(nRow-1, frkN(m_GridAdd.GetTextMatrixDouble(nRow,2)));
			bExtraLoad	= TRUE;
		}
	}
	pData->m_bExtraLoad	= bExtraLoad;
}

void CCalcCantileverSetup::SetDataDefault()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CLineInfo *pLine = m_pStd->GetLineInfoApp()->GetLine(0);

	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis    = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight = 0.0;

	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis    = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanHeight = 0.0;
	
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1 = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2 = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH = 0.0;

	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1 = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2 = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment = 0.0;
	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH = 0.0;

	pData->m_dSpeed		= 120;
	pData->m_dWindLoadForce = 3;
	pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Coll_V	= 0.0;	// 사용하지 않음
	pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Coll_V	= 0.0;	// 사용하지 않음

	if (pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R==0)
	{
		long nIPs = pLine->GetQtyIp();

		if (nIPs>0)
		{
			CIPRecord* pIpRec = &pLine->m_pIpRec[0];
			pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R = toM(pIpRec->m_dR_IpRecord);
		}
	}
	pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Cent_R	= pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R;
	pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Cent_R	= pData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R;

	pData->m_dCoverUp	= 60;
	pData->m_dCoverDn	= 40;
	pData->m_dCoverSd	= 50;

	BOOL	bLeftWalkLoad	= IsExistWalkLoad(TRUE);
	BOOL	bRightWalkLoad	= IsExistWalkLoad(FALSE);
	pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad		= bLeftWalkLoad;
	pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_HoriF	= bLeftWalkLoad ? 2.5 : 0;
	pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_VerF	= bLeftWalkLoad ? 1.0 : 0;

	pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad		= bRightWalkLoad;
	pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_HoriF	= bRightWalkLoad ? 2.5 : 0;
	pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_VerF	= bRightWalkLoad ? 1.0 : 0;
}

void CCalcCantileverSetup::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateBasicIndex	*pBx	= pDB->GetGirder(0)->GetCrossBxAtMinFlangeWidth();
	CCentSeparation		*pSep	= NULL;
	CDomyun				*pDom	= m_pView->GetDomyun();
	CARoadOptionStd		*pOptStd= m_pStd->GetDataManage()->GetOptionStd();
	CAPlateDrawCalc		DrawCalc(m_pStd->m_pDataManage);
	CCalcGeneral		CalcGeneral(m_pStd->m_pDataManage);
	CCalcFloor			CalcFloor(m_pStd->GetDataManage());

	pDom->ClearEtt(TRUE);
	double	dLen	= 0;
	double	dDis_Car		= CalcGeneral.GetDBLOAD_Dis_CAR();
	double	dDis_Tank		= CalcGeneral.GetDBLOAD_Dis_Tank();
	double	dDis_Trailer	= CalcGeneral.GetDBLOAD_Dis_Trailer();
	long	nType			= 0;
	DrawCalc.SetDBLoad(dDis_Car, dDis_Tank, dDis_Trailer);
	CDomyun	Dom(pDom);
	Dom.SetScaleDim(20);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	switch(m_nDrawType)
	{
	case FLOOR_LEFT :
		pSep = pDB->GetCurGuardWall(CCentSeparation::LEFT);
		dLen = pDB->GetLengthHDanDom(pBx, 0);
		if(!pSep)	dLen = 0;
		nType	= CalcFloor.GetGuardWallType(FLOOR_LEFT);
		DrawCalc.SetGuardType(nType);
		DrawCalc.DrawGuardWallInputLeft(&Dom, pSep, dLen);
		DrawCalc.DrawFloorLoadInputLeftDim(&Dom, pSep, dLen);
		break;
	case FLOOR_RIGHT :
		pSep = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
		dLen = pDB->GetLengthHDanDom(pBx, pDB->GetQtyHDanNode()-1);
		if(!pSep)	dLen = 0;
		nType	= CalcFloor.GetGuardWallType(FLOOR_RIGHT);
		DrawCalc.SetGuardType(nType);
		DrawCalc.DrawGuardWallInputRight(&Dom, pSep, dLen);
		DrawCalc.DrawFloorLoadInputRightDim(&Dom, pSep, dLen);
		break;
	}

	*pDom<<Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

CString CCalcCantileverSetup::GetApply(BOOL bApply)
{
	if(bApply)	return "적용";
	else		return "미적용";

	return "미적용";
}
BOOL CCalcCantileverSetup::GetApply(CString szApply)
{
	if(szApply=="적용")	return TRUE;
	else				return FALSE;

	return FALSE;
}

BOOL CCalcCantileverSetup::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
//		CCellID next = m_GridGen.GetFocusCell();
//		long nRow = next.row;
//		long nCol = next.col;
	}
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CCalcCantileverSetup::SetCellReadOnly(long nTarget)
{
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();

	BOOL	bLeft	= FALSE;
	BOOL	bRight	= FALSE;
	long	nRow	= 0;

	switch(nTarget)
	{
	case BANGEUM:
		bLeft	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
		bRight	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;
		for(nRow=1; nRow<4; nRow++)
		{
			m_GridLoad.SetItemBkColour(nRow, 1, bLeft ? RGB(225, 250, 250) : RGB(255,255,200));
			m_GridLoad.SetItemState(nRow, 1, bLeft ? GVIS_MODIFIED : GVIS_READONLY);
			m_GridLoad.SetItemBkColour(nRow, 2, bRight ? RGB(225, 250, 250) : RGB(255,255,200));
			m_GridLoad.SetItemState(nRow, 2, bRight ? GVIS_MODIFIED : GVIS_READONLY);
		}
		m_GridLoad.SetRedraw(TRUE, TRUE);
		break;
	case NANGAN:
		bLeft	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan;
		bRight	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan;
		for(nRow=5; nRow<8; nRow++)
		{
			m_GridLoad.SetItemBkColour(nRow, 1, bLeft ? RGB(225, 250, 250) : RGB(255,255,200));
			m_GridLoad.SetItemState(nRow, 1, bLeft ? GVIS_MODIFIED : GVIS_READONLY);
			m_GridLoad.SetItemBkColour(nRow, 2, bRight ? RGB(225, 250, 250) : RGB(255,255,200));
			m_GridLoad.SetItemState(nRow, 2, bRight ? GVIS_MODIFIED : GVIS_READONLY);
		}
		m_GridLoad.SetRedraw(TRUE, TRUE);
		break;
	case WALKLOAD:
		bLeft	= pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad;
		bRight	= pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad;
		for(nRow=1;nRow<4;nRow++)
		{
			m_GridWalk.SetItemBkColour(nRow, 1, bLeft ? RGB(225, 250, 250) : RGB(255,255,200));
			m_GridWalk.SetItemState(nRow, 1, bLeft ? GVIS_MODIFIED : GVIS_READONLY);
			m_GridWalk.SetItemBkColour(nRow, 2, bRight ? RGB(225, 250, 250) : RGB(255,255,200));
			m_GridWalk.SetItemState(nRow, 2, bRight ? GVIS_MODIFIED : GVIS_READONLY);
		}
		if(bLeft || bRight)
		{
			m_GridWalk.SetItemBkColour(4, 1, RGB(225, 250, 250));
			m_GridWalk.SetItemState(4, 1, GVIS_MODIFIED);
			m_GridWalk.SetItemBkColour(5, 1, RGB(225, 250, 250));
			m_GridWalk.SetItemState(5, 1, GVIS_MODIFIED);
		}
		else
		{
			m_GridWalk.SetItemBkColour(4, 1, RGB(255,255,200));
			m_GridWalk.SetItemState(4, 1, GVIS_READONLY);
			m_GridWalk.SetItemBkColour(5, 1, RGB(255,255,200));
			m_GridWalk.SetItemState(5, 1, GVIS_READONLY);
		}
		m_GridWalk.SetItemState(6, 1, bLeft ? GVIS_MODIFIED : GVIS_READONLY);
		m_GridWalk.SetItemState(6, 2, bRight ? GVIS_MODIFIED : GVIS_READONLY);
		m_GridWalk.SetRedraw(TRUE, TRUE);
		break;
	case PEOPLELOAD:
		m_GridWalk.SetRedraw(TRUE, TRUE);
		break;
	case ADDITIVELOAD:
		m_GridAdd.SetRedraw(TRUE,TRUE);
		break;
	}
}

void CCalcCantileverSetup::OnCellChangedDataGen(NMHDR*nmgv,LRESULT*)
{
	CCalcData	*pData		= m_pStd->GetDataManage()->GetCalcData();
	NM_GRIDVIEW	*GridView	= (NM_GRIDVIEW *)nmgv;

	long	nRow	= GridView->iRow;
	long	nCol	= GridView->iColumn;

	if(nRow>7)
	{
		BOOL bCheck[3] = { FALSE, FALSE, FALSE };
		long nIdx = (nRow==8)? 0 : (nRow==9)? 1: 2;
		bCheck[nIdx] = TRUE;
		m_GridGen.SetCheck(8, 1, bCheck[0]);
		m_GridGen.SetCheck(9, 1, bCheck[1]);
		m_GridGen.SetCheck(10, 1, bCheck[2]);

		m_GridGen.SetItemState(10, 3, GVIS_READONLY);
		if(nIdx==2) m_GridGen.SetItemState(10, 3, GVIS_MODIFIED);
	
		m_GridGen.SetRedraw(TRUE, TRUE);

		if(nRow==10 && nCol==3)
		{
			double	dValue	= m_GridGen.GetTextMatrixDouble(nRow, nCol);
			if(dValue < 3.0 || dValue > 3.6)
			{
				AfxMessageBox("설계차로폭은 3m 이상 3.6m 이하로 입력해야 합니다.");
				m_GridGen.SetTextMatrix(nRow, nCol, "%.3f", pData->m_dUserLaneWidth);
			}
		}
		SetDataSaveGen();
		DrawInputDomyunView(FALSE);
	}
}

void CCalcCantileverSetup::OnCellChangedDataLoad(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	long	nRow	= GridView->iRow;
	long	nCol	= GridView->iColumn;

	SetDataSaveLoad();
	SetDataInit();
	DrawInputDomyunView(FALSE);
	if(nRow==0 && (nCol==1 || nCol==2))
		SetCellReadOnly(BANGEUM);
	if(nRow==4 && (nCol==1 || nCol==2))
		SetCellReadOnly(NANGAN);

	nRow = min(nRow+1, m_GridLoad.GetRowCount()-1);
	m_GridLoad.ResetSelection();
	m_GridLoad.SetFocusCell(nRow,nCol-1,FALSE);
}

void CCalcCantileverSetup::OnCellChangedDataWalkLoad(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	long	nRow	= GridView->iRow;
	long	nCol	= GridView->iColumn;

	if(nRow==0)
	{
		if(nCol==1 && !IsExistWalkLoad(TRUE))
		{
			AfxMessageBox("횡단면도에서 좌측 보도를 설치하지 않았습니다.");
			m_GridWalk.SetCheck(nRow, nCol, FALSE);
		}

		if(nCol==2 && !IsExistWalkLoad(FALSE))
		{
			AfxMessageBox("횡단면도에서 우측 보도를 설치하지 않았습니다.");
			m_GridWalk.SetCheck(nRow, nCol, FALSE);
		}
	}
	else if(nRow==6 && nCol==1)
	{
		if(nCol==1 && !IsExistWalkLoad(TRUE))
		{
			AfxMessageBox("횡단면도에서 좌측 보도를 설치하지 않았습니다.");
			m_GridWalk.SetCheck(nRow, nCol, FALSE);
		}
	
		if(nCol==2 && !IsExistWalkLoad(FALSE))
		{
			AfxMessageBox("횡단면도에서 우측 보도를 설치하지 않았습니다.");
			m_GridWalk.SetCheck(nRow, nCol, FALSE);
		}
	}
	SetDataSaveWalkLoad();
	SetDataInit();
	DrawInputDomyunView(FALSE);
	SetCellReadOnly(WALKLOAD);
	nRow = min(nRow+1, m_GridWalk.GetRowCount()-1);
	m_GridWalk.ResetSelection();
	m_GridWalk.SetFocusCell(nRow,nCol-1);
	DrawInputDomyunView(FALSE);
}

void CCalcCantileverSetup::OnCellChangedDataAdditiveLoad(NMHDR*nmgv,LRESULT*)
{
	SetDataSaveAdditiveLoad();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CCalcCantileverSetup::OnCellButtonClickAdditiveLoad(NMHDR*nmgv,LRESULT*)
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	long	nRow	= GridView->iRow;
	long	nCol	= GridView->iColumn;

	if(nRow==0 && nCol==0)
	{
		m_GridAdd.SetRowCount(m_GridAdd.GetRowCount()+1);
		m_GridAdd.SetRowHeightAll(20);
		m_GridAdd.SetRedraw(TRUE,TRUE);
		m_GridAdd.SetTextMatrix(m_GridAdd.GetRowCount()-1, 0, "UNTITLE");
		m_GridAdd.SetTextMatrix(m_GridAdd.GetRowCount()-1, 1, "%.3f", 0.0);
		m_GridAdd.SetTextMatrix(m_GridAdd.GetRowCount()-1, 2, "%.3f", 0.0);
		m_GridAdd.SetCellType(m_GridAdd.GetRowCount()-1, 3, CT_BUTTON);
		m_GridAdd.SetTextMatrix(m_GridAdd.GetRowCount()-1, 3, "삭제");
		pData->m_ELoadTitleArr.SetSize(pData->m_ELoadTitleArr.GetSize()+1);
		pData->m_ELoadDisArr.SetSize(pData->m_ELoadDisArr.GetSize()+1);
		pData->m_ELoadWeightArr.SetSize(pData->m_ELoadWeightArr.GetSize()+1);
		SetDataSaveAdditiveLoad();
		DrawInputDomyunView(FALSE);
	}
	if(nRow>0)
	{
		m_GridAdd.DeleteRow(nRow);	
		m_GridAdd.SetRedraw(TRUE,TRUE);
		pData->m_ELoadTitleArr.SetSize(pData->m_ELoadTitleArr.GetSize()-1);
		pData->m_ELoadDisArr.SetSize(pData->m_ELoadDisArr.GetSize()-1);
		pData->m_ELoadWeightArr.SetSize(pData->m_ELoadWeightArr.GetSize()-1);
		SetDataSaveAdditiveLoad();
		DrawInputDomyunView(FALSE);
	}
}

void CCalcCantileverSetup::OnCellChangeLoad(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	long	nCol	= GridView->iColumn;

	if(nCol==1)
	{
		m_nDrawType = FLOOR_LEFT;
		DrawInputDomyunView(TRUE);
	}
	else if(nCol==2)
	{
		m_nDrawType = FLOOR_RIGHT;
		DrawInputDomyunView(TRUE);
	}
}

void CCalcCantileverSetup::OnCellChangeWalkLoad(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	long	nCol	= GridView->iColumn;

	if(nCol==1)
	{
		m_nDrawType = FLOOR_LEFT;
		DrawInputDomyunView(TRUE);
	}
	else if(nCol==2)
	{
		m_nDrawType = FLOOR_RIGHT;
		DrawInputDomyunView(TRUE);
	}
}

BOOL CCalcCantileverSetup::IsExistWalkLoad(BOOL bLeft)
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(-1);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	long	nType		= 0;
	double	dDistTot	= 0;
	double	dSlabWidth	= pDB->GetWidthSlabDom(pBx);

	for(long nHDan=0; nHDan<pDB->GetQtyHDanNode(); nHDan++)
	{
		nType    = pDB->GetValueTypeHDan(nHDan, 1);
		dDistTot += pDB->GetLengthHDanDom(pBx, nHDan);

		if(nType==HT_PEOPLEROAD && bLeft)
		{
			if(dDistTot <= dSlabWidth/2 || nHDan == 0)
				return TRUE;
		}

		if(dDistTot > dSlabWidth/2 && nType==HT_PEOPLEROAD && !bLeft)
			return TRUE;
	}

	return FALSE;
}
