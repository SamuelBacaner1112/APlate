// CalcUltimateLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Generalinput.h"
#include "CalcUltimateLoadDlg.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcUltimateLoadDlg dialog


CCalcUltimateLoadDlg::CCalcUltimateLoadDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcUltimateLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcUltimateLoadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Grid.SetDefaultCharacterCount(2);
}


void CCalcUltimateLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcUltimateLoadDlg)
	DDX_Control(pDX, IDC_RICHEDIT_REBAR, m_richeditRebar);
	DDX_Control(pDX, IDC_TAB_REBAR, m_TabRebar);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcUltimateLoadDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcUltimateLoadDlg)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_REBAR, OnSelchangeTabRebar)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcUltimateLoadDlg message handlers
void CCalcUltimateLoadDlg::OnPreInitDialog()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();

	m_sRebar = pData->DESIGN_MATERIAL.m_sSigmaY;

	long	nTab = 0;
	m_TabRebar.InsertItem(nTab++, "좌측 캔틸레버부");
	m_TabRebar.InsertItem(nTab++, "우측 캔틸레버부");
	m_TabRebar.InsertItem(nTab++, "중앙부 바닥판");
	if(pDB->m_nQtyJigan > 1)
		m_TabRebar.InsertItem(nTab++, "중간지점부");
	m_TabRebar.InsertItem(nTab++, "정착부 보강");
	m_TabRebar.SetCurSel(0);
	SetGridTitle();
	SetActiveGrid();

	SetResize(IDC_TAB_REBAR, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_RICHEDIT_REBAR, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_REBAR_TITLE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
}

void CCalcUltimateLoadDlg::SetGridTitle()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();

	long	nQtyJigan	= pDB->m_nQtyJigan;
	long	nRows		= 30;
	long	nCols		= 6;

	if(nQtyJigan > 1)	nRows += 4;
	long nRow = 0;
	for(nRow=0; nRow<nRows; nRow++)
		for(long nCol=0; nCol<nCols; nCol++)
			m_Grid.SetCellType(nRow, nCol, CT_EDIT);

	m_Grid.SetCols(nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(47);

	m_Grid.SetTextMatrix(0, 0, "< 좌측 캔틸레버부 >");	m_Grid.SetMergeCol(0, 0, 5);
	m_Grid.SetTextMatrix(1, 0, "배근위치");				m_Grid.SetMergeCol(1, 0, 2);
	m_Grid.SetTextMatrix(1, 3, "간격");
	m_Grid.SetTextMatrix(1, 4, "① 직경");
	m_Grid.SetTextMatrix(1, 5, "② 직경");
	m_Grid.SetTextMatrix(1, 6, "판정");
	m_Grid.SetTextMatrix(2, 0, "일반부");				m_Grid.SetMergeRow(2, 4, 0);
	m_Grid.SetTextMatrix(2, 1, "주철근");				m_Grid.SetMergeRow(2, 3, 1);
	m_Grid.SetTextMatrix(2, 2, "인장");
	m_Grid.SetTextMatrix(3, 2, "압축");
	m_Grid.SetTextMatrix(4, 1, "배력철근");				m_Grid.SetMergeCol(4, 1, 2);
	m_Grid.SetTextMatrix(5, 0, "단부");					m_Grid.SetMergeRow(5, 6, 0);
	m_Grid.SetTextMatrix(5, 1, "주철근");				m_Grid.SetMergeCol(5, 1, 2);
	m_Grid.SetTextMatrix(6, 1, "배력철근");				m_Grid.SetMergeCol(6, 1, 2);

	m_Grid.SetCellType(2, 4, CT_COMBO);					m_Grid.SetCellType(2, 5, CT_COMBO);
	m_Grid.SetCellType(3, 4, CT_COMBO);					m_Grid.SetCellType(3, 5, CT_COMBO);
	m_Grid.SetCellType(4, 4, CT_COMBO);
	m_Grid.SetCellType(5, 4, CT_COMBO);					m_Grid.SetCellType(5, 5, CT_COMBO);
	m_Grid.SetCellType(6, 3, CT_COMBO);
	m_Grid.SetCellType(6, 4, CT_COMBO);

	m_Grid.SetTextMatrix(7, 0, "< 우측 캔틸레버부 >");	m_Grid.SetMergeCol(7, 0, 5);
	m_Grid.SetTextMatrix(8, 0, "배근위치");				m_Grid.SetMergeCol(8, 0, 2);
	m_Grid.SetTextMatrix(8, 3, "간격");
	m_Grid.SetTextMatrix(8, 4, "① 직경");
	m_Grid.SetTextMatrix(8, 5, "② 직경");
	m_Grid.SetTextMatrix(8, 6, "판정");
	m_Grid.SetTextMatrix(9, 0, "일반부");				m_Grid.SetMergeRow(9, 11, 0);
	m_Grid.SetTextMatrix(9, 1, "주철근");				m_Grid.SetMergeRow(9, 10, 1);
	m_Grid.SetTextMatrix(9, 2, "인장");
	m_Grid.SetTextMatrix(10, 2, "압축");
	m_Grid.SetTextMatrix(11, 1, "배력철근");			m_Grid.SetMergeCol(11, 1, 2);
	m_Grid.SetTextMatrix(12, 0, "단부");				m_Grid.SetMergeRow(12, 13, 0);
	m_Grid.SetTextMatrix(12, 1, "주철근");				m_Grid.SetMergeCol(12, 1, 2);
	m_Grid.SetTextMatrix(13, 1, "배력철근");			m_Grid.SetMergeCol(13, 1, 2);
	m_Grid.SetCellType(9, 4, CT_COMBO);					m_Grid.SetCellType(9, 5, CT_COMBO);
	m_Grid.SetCellType(10, 4, CT_COMBO);				m_Grid.SetCellType(10, 5, CT_COMBO);
	m_Grid.SetCellType(11, 4, CT_COMBO);
	m_Grid.SetCellType(12, 4, CT_COMBO);				m_Grid.SetCellType(12, 5, CT_COMBO);
	m_Grid.SetCellType(13, 3, CT_COMBO);
	m_Grid.SetCellType(13, 4, CT_COMBO);

	m_Grid.SetTextMatrix(14, 0, "< 중앙부 바닥판 >");	m_Grid.SetMergeCol(14, 0, 5);
	m_Grid.SetTextMatrix(15, 0, "배근위치");			m_Grid.SetMergeCol(15, 0, 2);
	m_Grid.SetTextMatrix(15, 3, "간격");
	m_Grid.SetTextMatrix(15, 4, "① 직경");
	m_Grid.SetTextMatrix(15, 5, "② 직경");
	m_Grid.SetTextMatrix(15, 6, "판정");
	m_Grid.SetTextMatrix(16, 0, "일반부");				m_Grid.SetMergeRow(16, 18, 0);
	m_Grid.SetTextMatrix(16, 1, "주철근");				m_Grid.SetMergeRow(16, 17, 1);
	m_Grid.SetTextMatrix(16, 2, "인장");
	m_Grid.SetTextMatrix(17, 2, "압축");
	m_Grid.SetTextMatrix(18, 1, "배력철근");			m_Grid.SetMergeCol(18, 1, 2);
	m_Grid.SetTextMatrix(19, 0, "단부");				m_Grid.SetMergeRow(19, 20, 0);
	m_Grid.SetTextMatrix(19, 1, "주철근");				m_Grid.SetMergeCol(19, 1, 2);
	m_Grid.SetTextMatrix(20, 1, "배력철근");			m_Grid.SetMergeCol(20, 1, 2);
	m_Grid.SetCellType(16, 4, CT_COMBO);				m_Grid.SetCellType(16, 5, CT_COMBO);
	m_Grid.SetCellType(17, 4, CT_COMBO);				m_Grid.SetCellType(17, 5, CT_COMBO);
	m_Grid.SetCellType(18, 4, CT_COMBO);
	m_Grid.SetCellType(19, 4, CT_COMBO);				m_Grid.SetCellType(19, 5, CT_COMBO);
	m_Grid.SetCellType(20, 3, CT_COMBO);
	m_Grid.SetCellType(20, 4, CT_COMBO);

	nRow = 21;
	if(nQtyJigan>1)
	{
		m_Grid.SetTextMatrix(nRow, 0, "< 중간지점부 >");	m_Grid.SetMergeCol(nRow, 0, 5);	nRow++;
		m_Grid.SetTextMatrix(nRow, 0, "배근위치");			m_Grid.SetMergeCol(nRow, 0, 2);
		m_Grid.SetTextMatrix(nRow, 3, "간격");
		m_Grid.SetTextMatrix(nRow, 4, "① 직경");
		m_Grid.SetTextMatrix(nRow, 5, "② 직경");
		m_Grid.SetTextMatrix(nRow, 6, "판정");				nRow++;
		m_Grid.SetTextMatrix(nRow, 0, "상면철근");			m_Grid.SetMergeCol(nRow, 0, 2);
		m_Grid.SetMergeRow(nRow, nRow+1, 5);				m_Grid.SetCellType(nRow, 4, CT_COMBO);	nRow++;
		m_Grid.SetTextMatrix(nRow, 0, "하면철근");			m_Grid.SetMergeCol(nRow, 0, 2);
		m_Grid.SetCellType(nRow,4, CT_COMBO);				nRow++;
	}

	m_Grid.SetTextMatrix(nRow, 0, "< 정착부 보강 >");	m_Grid.SetMergeCol(nRow++, 0, 5);
	m_Grid.SetTextMatrix(nRow, 0, "배근위치");			m_Grid.SetMergeCol(nRow, 0, 2);
	m_Grid.SetTextMatrix(nRow, 3, "철근수");
	m_Grid.SetTextMatrix(nRow, 4, "① 직경");
	m_Grid.SetTextMatrix(nRow, 5, "② 직경");
	m_Grid.SetTextMatrix(nRow++, 6, "판정");
	m_Grid.SetTextMatrix(nRow, 0, "수평철근T1");	m_Grid.SetMergeCol(nRow, 0, 2);	m_Grid.SetCellType(nRow++, 4, CT_COMBO);
	m_Grid.SetTextMatrix(nRow, 0, "수평철근T2");	m_Grid.SetMergeCol(nRow, 0, 2);	m_Grid.SetCellType(nRow++, 4, CT_COMBO);

	m_Grid.SetTextMatrix(nRow, 0, "정착판당 정착구");	m_Grid.SetMergeCol(nRow, 0, 2); m_Grid.SetTextMatrix(nRow, 4, "EA");
	m_Grid.SetItemState(nRow++, 3, GVIS_READONLY);

	m_Grid.SetTextMatrix(nRow, 0, "정착판 종류");		m_Grid.SetMergeCol(nRow, 0, 2); m_Grid.SetCellType(nRow, 3, CT_COMBO);
	m_Grid.SetItemState(nRow, 3, GVIS_READONLY);
	m_Grid.SetMergeCol(nRow++, 3, 5);
	
	m_Grid.SetTextMatrix(nRow, 0, "정착판폭원");	m_Grid.SetMergeCol(nRow, 0, 2);
	m_Grid.SetTextMatrix(nRow++, 4, "mm");
	m_Grid.SetTextMatrix(nRow, 0, "정착판높이");	m_Grid.SetMergeCol(nRow, 0, 2);
	m_Grid.SetTextMatrix(nRow++, 4, "mm");
	m_Grid.SetTextMatrix(nRow, 0, "철근간격");	    m_Grid.SetMergeCol(nRow, 0, 2);
	m_Grid.SetTextMatrix(nRow++, 4, "mm");

	m_Grid.SetRedraw(TRUE,TRUE);
	m_Grid.ExpandColumnsToFit();
}

void CCalcUltimateLoadDlg::SetDataInit()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CCalcData		*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());
	CPlateStressOut	StressOut(m_pStd);

	CString	strTitle	= GetTitle();
	long	nQtyJigan	= pDB->m_nQtyJigan;
	long	nRow		= 2;
	m_richeditRebar.SetSel(0, -1);
	m_richeditRebar.ReplaceSel("");
	m_richeditRebar.SetReadOnly(TRUE);

	// 캔틸레버부 좌우측
	for(long nIdx=0; nIdx<3; nIdx++)
	{
		m_Grid.SetTextMatrix(nRow,		3,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_dGenReinTCTC);					// 일반부 주철근 인장 C.T.C 
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinTDiaIdx_1Cy));	// 일반부 주철근 인장 직경 1-Cycle
		m_Grid.SetTextMatrix(nRow,		5,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinTDiaIdx_2Cy));	// 일반부 주철근 인장 직경 2-Cycle
		nRow++;

		m_Grid.SetTextMatrix(nRow,		3,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_dGenReinCCTC);					// 일반부 주철근 압축 C.T.C 
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinCDiaIdx_1Cy));	// 일반부 주철근 압축 직경 1-Cycle
		m_Grid.SetTextMatrix(nRow,		5,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinCDiaIdx_2Cy));	// 일반부 주철근 압축 직경 2-Cycle
		nRow++;

		m_Grid.SetTextMatrix(nRow,		3,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB);					// 일반부 배력철근 C.T.C
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo3));		// 일반부 배력철근 직경
		m_Grid.SetTextMatrix(nRow,		5,			_T("-"));
		nRow++;

		m_Grid.SetTextMatrix(nRow,		3,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc2);					// 단부 주철근 C.T.C
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2));		// 단부 주철근 직경 1-Cycle
		m_Grid.SetTextMatrix(nRow,		5,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2_2Cy));	// 단부 주철근 직경 2-Cycle
		nRow++;

		m_Grid.SetTextMatrix(nRow,		3,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB1);				// 단부 배력철근 C.T.C
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo4));		// 단부 배력철근 직경
		m_Grid.SetTextMatrix(nRow,		5,			_T("-"));
		nRow+=3;
	}

	if(nQtyJigan>1)
	{
		m_Grid.SetTextMatrix(nRow,		3,	"%.0f",	pCalcData->m_Ctc1);
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->m_ComboInnerJijumSlab1));
		m_Grid.SetTextMatrix(nRow,		5,			_T("-"));
		m_Grid.SetTextMatrix(nRow++,	6,			"");
		m_Grid.SetTextMatrix(nRow,		3,	"%.0f",	pCalcData->m_Ctc2);
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->m_ComboInnerJijumSlab2));
		m_Grid.SetTextMatrix(nRow,		5,			_T("-"));
		nRow+=3;
	}

	//수평철근T1
	m_Grid.SetTextMatrixFormat(nRow,	3,	"%.d",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2);
	m_Grid.SetTextMatrix(nRow,			4,	GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2));
	m_Grid.SetTextMatrix(nRow++,		5,	_T("-"));	
	//수평철근T2
	m_Grid.SetTextMatrixFormat(nRow,	3,	"%.d",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2);
	m_Grid.SetTextMatrix(nRow,			4,	GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerRein2));
	m_Grid.SetTextMatrix(nRow++,		5,	_T("-"));
	//정착판당 정착구
	m_Grid.SetTextMatrixFormat(nRow++,	3,	"%.d",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nAnchorageEA);
	//정착판 종류
	m_Grid.SetTextMatrix(nRow++,	3,	pCalcData->GetFixingPlateType(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nFixingPlateType));

	m_Grid.SetTextMatrixFormat(nRow++,	3,	"%.1f",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth);
	m_Grid.SetTextMatrixFormat(nRow++,	3,	"%.1f",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight);
	m_Grid.SetTextMatrixFormat(nRow++,	3,	"%.1f",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dReinCTC);

	if(strTitle=="중앙부 바닥판")			StressOut.Output_UltimateLoadCheck(FLOOR_MID, m_richeditRebar);
	else if(strTitle=="좌측 캔틸레버부")	StressOut.Output_UltimateLoadCheck(FLOOR_LEFT, m_richeditRebar);
	else if(strTitle=="우측 캔틸레버부")	StressOut.Output_UltimateLoadCheck(FLOOR_RIGHT, m_richeditRebar);
	else if(strTitle=="중간지점부")			StressOut.Output_InnerFloor(m_richeditRebar);
	else if(strTitle=="정착부 보강")		StressOut.Output_FixingPlate(m_richeditRebar);

	m_richeditRebar.LineScroll(-m_richeditRebar.GetFirstVisibleLine());
	DrawInputDomyunView(TRUE);
}

void CCalcUltimateLoadDlg::SetDataSave()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CCalcData		*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor		*pCalcFloor	= m_pStd->m_pCalcStd->m_pCalcFloor;

	CString	sText		= _T("");
	long	nRow		= 2;
	long	nQtyJigan	= pDB->m_nQtyJigan;

	for(long nIdx=0; nIdx<3; nIdx++)
	{
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_b				= 1000;
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_dGenReinTCTC		= m_Grid.GetTextMatrixDouble(nRow, 3);			// 일반부 주철근 인장 C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinTDia_1Cy	= GetRebar(m_Grid.GetTextMatrix(nRow, 4));		// 일반부 주철근 인장 직경 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinTDia_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 5));	// 일반부 주철근 인장 직경 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinTDiaIdx_1Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinTDia_1Cy);
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinTDiaIdx_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinTDia_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_dGenReinCCTC		= m_Grid.GetTextMatrixDouble(nRow, 3);			// 일반부 주철근 압축 C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinCDia_1Cy	= GetRebar(m_Grid.GetTextMatrix(nRow, 4));		// 일반부 주철근 압축 직경 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinCDia_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 5));	// 일반부 주철근 압축 직경 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinCDiaIdx_1Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinCDia_1Cy);
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinCDiaIdx_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinCDia_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB		= m_Grid.GetTextMatrixDouble(nRow, 3);			// 일반부 배력철근 C.T.C
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH3		= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// 일반부 배력철근 직경
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo3		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH3);

		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc2		= m_Grid.GetTextMatrixDouble(nRow, 3);			// 단부 주철근 C.T.C
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2		= GetRebar(m_Grid.GetTextMatrix(nRow, 4));		// 단부 주철근 직경 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 5));	// 단부 주철근 직경 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2);
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB1		= m_Grid.GetTextMatrixDouble(nRow, 3);			// 단부 배력철근 C.T.C
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH4		= GetRebar(m_Grid.GetTextMatrix(nRow, 4));		// 단부 배력철근 직경
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo4		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH4);

		nRow+=3;
	}

	if(nQtyJigan>1)
	{
		pCalcData->m_Ctc1					= m_Grid.GetTextMatrixDouble(nRow, 3);		// 철근간격1
		pCalcData->m_CaseH1					= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));// 철근치수1
		pCalcData->m_ComboInnerJijumSlab1	= GetRebarDiaIdx(pCalcData->m_CaseH1);		// 철근 호칭콤보1
		pCalcData->m_dia1					= atof(pCalcData->m_CaseH1);				// 철근지름1

		pCalcData->m_Ctc2					= m_Grid.GetTextMatrixDouble(nRow, 3);		// 철근지름2
		pCalcData->m_CaseH2					= GetRebar(m_Grid.GetTextMatrix(nRow, 4));	// 철근치수2
		pCalcData->m_ComboInnerJijumSlab2	= GetRebarDiaIdx(pCalcData->m_CaseH2);		// 철근 호칭콤보2
		pCalcData->m_dia2					= atof(pCalcData->m_CaseH2);				// 철근지름2
		pCalcFloor->SetInnerJijumRebar();
		nRow+=3;
	}

	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2		= m_Grid.GetTextMatrixLong(nRow, 3);
	sText = GetRebar(m_Grid.GetTextMatrix(nRow, 4));
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerRein2			= GetRebarDiaIdx(sText);	
	nRow++;

	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2		= m_Grid.GetTextMatrixLong(nRow, 3);
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH2				= GetRebar(m_Grid.GetTextMatrix(nRow, 4));
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2			= GetRebarDiaIdx(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH2);	
	nRow++;
	
	//정착판당 정착구
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nAnchorageEA		= m_Grid.GetTextMatrixLong(nRow, 3);
	nRow++;
	//정착판 종류
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nFixingPlateType	= pCalcData->GetFixingPlateType(m_Grid.GetTextMatrix(nRow, 3));
	nRow++;

	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth		= m_Grid.GetTextMatrixDouble(nRow++,3);
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight	= m_Grid.GetTextMatrixDouble(nRow++,3);
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dReinCTC			= m_Grid.GetTextMatrixDouble(nRow++,3);	
}

void CCalcUltimateLoadDlg::SetDataDefault()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CCalcData		*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor		*pCalcFloor	= m_pStd->m_pCalcStd->m_pCalcFloor;
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());

	long	nPos	= m_TabRebar.GetCurSel();
	long	nStt	= IsUsedFlag() ? nPos : 0;
	long	nEnd	= IsUsedFlag() ? nPos+1 : FLOOR_SIZE;
	CMap <CString, LPCTSTR, double, double> Map;

	if((IsUsedFlag() && nPos<3) || !IsUsedFlag())
	{
		for(long nIdx=nStt; nIdx<nEnd; nIdx++)
		{
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_b				= 1000;
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_dGenReinTCTC		= 250.0;	// 일반부 주철근 C.T.C 
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinTDia_1Cy	= _T("19");	// 일반부 주철근 직경 1-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinTDia_2Cy	= _T("19");	// 일반부 주철근 직경 2-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinTDiaIdx_1Cy	= 2;		// 일반부 주철근Index 1-Cycle 
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinTDiaIdx_2Cy	= 2;		// 일반부 주철근Index 2-Cycle 

			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_dGenReinCCTC		= 250.0;	// 일반부 주철근 C.T.C 
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinCDia_1Cy	= _T("19");	// 일반부 주철근 직경 1-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_strGenReinCDia_2Cy	= _T("19");	// 일반부 주철근 직경 2-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinCDiaIdx_1Cy	= 2;		// 일반부 주철근Index 1-Cycle 
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_nGenReinCDiaIdx_2Cy	= 2;		// 일반부 주철근Index 2-Cycle 

			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB		= 125.0;		// 일반부 배력철근 C.T.C
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH3		= _T("22");		// 일반부 배력철근 직경
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo3		= 3;			// 일반부 배력철근 Index

			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc2		= 250;			// 단부 주철근 C.T.C
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2		= _T("16");		// 단부 주철근 직경 1-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2_2Cy	= _T("16");		// 단부 주철근 직경 2-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2		= 1;			// 단부 주철근 직경 Index
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2_2Cy	= 1;			// 단부 주철근 직경 Index

			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB1		= 125.0;			// 단부 배력철근 C.T.C
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH4		= _T("19");		// 단부 배력철근 직경
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo4		= 2;			// 단부 배력철근 Index

			pCalcFloor->SetCantileverUltimateLoad(nIdx, Map);
		}
	}

	if((IsUsedFlag() && nPos == 3) || !IsUsedFlag())
	{
		if(pDB->m_nQtyJigan>1)
		{
			pCalcData->m_dia1					= 25.0;		// 철근지름1
			pCalcData->m_dia2					= 25.0;		// 철근지름2
			pCalcData->m_Ctc1					= 125.0;	// 철근간격1
			pCalcData->m_Ctc2					= 125.0;	// 철근지름2
			pCalcData->m_CaseH1					= _T("25");	// 철근치수1
			pCalcData->m_CaseH2					= _T("25");	// 철근치수2
			pCalcData->m_ComboInnerJijumSlab1	= 4;		// 철근 호칭콤보1
			pCalcData->m_ComboInnerJijumSlab2	= 4;		// 철근 호칭콤보2
			pCalcFloor->SetInnerJijumRebar();
		}
	}

	//정착부 보강설계
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1		= 4;		//< 수평 정착부 철근수
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2		= 4;		//< 수평 정착부 철근수
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nAnchorageEA		= 4;		//< 정착판당 정착구
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2		= 4;		//< 수직 정착부 철근수
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein1			= 0;		//< 수평 정착부 철근 combo
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2			= 0;		//< 수평 정착부 보강 철근 combo
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerRein2			= 0;		//< 수직 정착부 보강 철근 combo
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nFixingPlateType	= 0;		//< 정착판의 종류
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth		= 230.0;	//정착판 폭원
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight	= 90.0;		//정착판 높이
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dReinCTC			= 70.0;		//철근간격
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH1				= GetRebar(GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein1));
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH2				= GetRebar(GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2));

	pCalcFloor->SetAnchorageRebar();
	
	SetGridTitle();
}

void CCalcUltimateLoadDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CString csSection       = "Afx Directory";
	CString csWorkItem		= "Fixed Directory";
	CString szPath = AfxGetApp()->GetProfileString(csSection, csWorkItem);
	
	CString szName;	
	szName.Format("%s\\BlockDomyun\\Fixing.dxf",szPath);	

	CString	szTabTitle	= GetTitle();

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);

	if(szTabTitle=="정착부 보강")
	{
		Dom.DxfIn(szName);
		Dom.Move(Dom.Always(0), Dom.Always(0));
	}

	*pDom << Dom;
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

long CCalcUltimateLoadDlg::GetRebarDiaIdx(CString szDia)
{
	if(szDia == _T("13"))	return 0;
	if(szDia == _T("16"))	return 1;
	if(szDia == _T("19"))	return 2;
	if(szDia == _T("22"))	return 3;
	if(szDia == _T("25"))	return 4;
	if(szDia == _T("29"))	return 5;
	if(szDia == _T("32"))	return 6;
	if(szDia == _T("35"))	return 7;

	return -1;
}

CString CCalcUltimateLoadDlg::GetRebar(long nIdx)
{
	if(nIdx == 0)	return _T(m_sRebar+"13");
	if(nIdx == 1)	return _T(m_sRebar+"16");
	if(nIdx == 2)	return _T(m_sRebar+"19");
	if(nIdx == 3)	return _T(m_sRebar+"22");
	if(nIdx == 4)	return _T(m_sRebar+"25");
	if(nIdx == 5)	return _T(m_sRebar+"29");
	if(nIdx == 6)	return _T(m_sRebar+"32");
	if(nIdx == 7)	return _T(m_sRebar+"35");

	return _T("");
}

CString CCalcUltimateLoadDlg::GetRebar(CString szDia)
{
	if(szDia == _T(m_sRebar+"13"))	return _T("13");
	if(szDia == _T(m_sRebar+"16"))	return _T("16");
	if(szDia == _T(m_sRebar+"19"))	return _T("19");
	if(szDia == _T(m_sRebar+"22"))	return _T("22");
	if(szDia == _T(m_sRebar+"25"))	return _T("25");
	if(szDia == _T(m_sRebar+"29"))	return _T("29");
	if(szDia == _T(m_sRebar+"32"))	return _T("32");
	if(szDia == _T(m_sRebar+"35"))	return _T("35");

	return _T("");
}

CString CCalcUltimateLoadDlg::GetCheck(BOOL bCheck)
{
	if(bCheck)	return _T("O.K");
	else		return _T("N.G");

	return _T("");
}

CString CCalcUltimateLoadDlg::GetTitle()
{
	CString	szTabTitle	= _T("");
	TCITEM	TabCtrlItem;
	TabCtrlItem.mask		= TCIF_TEXT;
	TabCtrlItem.pszText		= szTabTitle.GetBufferSetLength(255);
	TabCtrlItem.cchTextMax	= 255;
	m_TabRebar.GetItem(m_TabRebar.GetCurSel(), &TabCtrlItem);

	return szTabTitle;
}

void CCalcUltimateLoadDlg::SetActiveGrid()
{
	long	nRowsu		= m_Grid.GetRowCount();
	long	nColsu		= m_Grid.GetColumnCount();
	long	nAddRow		= 0;
	long	nFloorIdx	= 0;
	CString	szText		= _T("");
	CString	szTabTitle	= GetTitle();

	if(szTabTitle=="중앙부 바닥판")			nFloorIdx = FLOOR_MID;
	else if(szTabTitle=="좌측 캔틸레버부")	nFloorIdx = FLOOR_LEFT;
	else if(szTabTitle=="우측 캔틸레버부")	nFloorIdx = FLOOR_RIGHT;
	else									nFloorIdx = -1;
	long nRow = 0;
	for(nRow=0; nRow<nRowsu; nRow++)
	{
		szText.Format("%s", m_Grid.GetTextMatrix(nRow, 0));
		if(nFloorIdx==FLOOR_LEFT && szText == _T("< 좌측 캔틸레버부 >"))
		{
			nAddRow	= 6;
			break;
		}
		if(nFloorIdx==FLOOR_RIGHT && szText == _T("< 우측 캔틸레버부 >"))
		{
			nAddRow	= 6;
			break;
		}
		if(nFloorIdx==FLOOR_MID && szText == _T("< 중앙부 바닥판 >"))
		{
			nAddRow	= 6;
			break;
		}
		if(szTabTitle=="중간지점부" && szText == _T("< 중간지점부 >"))
		{
			nAddRow	= 3;
			break;
		}
		if(szTabTitle=="정착부 보강" && szText == _T("< 정착부 보강 >"))
		{
			nAddRow	= 8;
			break;
		}
	}
	long nR = 0;
	for(nR=0; nR<nRowsu; nR++)
	{
		for(long nCol=3; nCol<nColsu; nCol++)
		{
			m_Grid.SetItemBkColour(nR, nCol, RGB(255,255,200));
			m_Grid.SetItemState(nR, nCol, GVIS_READONLY);
		}
	}
	for(long nCol=3; nCol<nColsu; nCol++)
		m_Grid.SetItemBkColour(nRow+1, nCol, RGB(255,255,255));

	for(nR=nRow+2; nR<=nRow+nAddRow; nR++)
	{
		for(long nCol=3; nCol<nColsu; nCol++)
		{
			m_Grid.SetItemBkColour(nR, nCol, RGB(255,255,255));
			m_Grid.SetItemState(nR, nCol, GVIS_MODIFIED);
		}			
	}

	for(nR=0; nR<nRowsu; nR++)
	{
		m_Grid.SetCellFixType(nR, 0, GVN_ONECELL_ROWFIX);
		m_Grid.SetCellFixType(nR, 1, GVN_ONECELL_ROWFIX);
		m_Grid.SetCellFixType(nR, 2, GVN_ONECELL_ROWFIX);
	}
	if(szTabTitle=="정착부 보강" && szText == _T("< 정착부 보강 >"))
	{
		//정착판당 정착구
		m_Grid.SetItemState(nRow+4, 3, GVIS_READONLY);
		//정착판 종류
		m_Grid.SetItemState(nRow+5, 3, GVIS_READONLY);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL CCalcUltimateLoadDlg::IsValid()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	return pBridge->m_bTendonInputComplete;
}

void CCalcUltimateLoadDlg::OnCellChangedData(NMHDR*nmgv,LRESULT*)
{
	CCalcFloor	*pCalcFloor	= m_pStd->m_pCalcStd->m_pCalcFloor;
	
	CString	szTabTitle	= GetTitle();
	CMap<CString, LPCTSTR, double, double> Map;

	SetDataSave();
	if(szTabTitle=="좌측 캔틸레버부")					pCalcFloor->SetCantileverUltimateLoad(FLOOR_LEFT, Map);
	else if(szTabTitle=="우측 캔틸레버부")				pCalcFloor->SetCantileverUltimateLoad(FLOOR_RIGHT, Map);
	else if(szTabTitle=="중앙부 바닥판")				pCalcFloor->SetCantileverUltimateLoad(FLOOR_MID, Map);
	else if(szTabTitle=="중간지점부")					pCalcFloor->SetInnerJijumRebar();
	else if(szTabTitle=="정착부 보강")					pCalcFloor->SetAnchorageRebar();
	SetDataInit();
}

void CCalcUltimateLoadDlg::OnCellChangedRow(NMHDR*nmgv,LRESULT*)
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

    long	nRow	= GridView->iRow;
	long	nSide	= m_TabRebar.GetCurSel();
	long	nTab	= 0;
	BOOL	bChange	= FALSE;

	if(nRow>=0 && nRow<=6)		nTab = 0;	// 좌측 캔틸레버부
	if(nRow>=7 && nRow<=13)		nTab = 1;	// 우측 캔틸레버부
	if(nRow>=14 && nRow<=20)	nTab = 2;	// 중앙부

	if(pDB->m_nQtyJigan>1)
	{
		if(nRow>=21 && nRow<=24)	nTab = 3;	// 중간지점부
		if(nRow>=25 && nRow<=33)	nTab = 4;	// 정착부 보강
	}
	else
	{
		if(nRow>=21 && nRow<=30)	nTab = 3;	// 정착부 보강
	}

	if(nSide != nTab)	bChange	= TRUE;
	else				bChange	= FALSE;
	if(bChange)
	{
		m_TabRebar.SetCurSel(nTab);
		SetActiveGrid();
		SetDataInit();
	}
}

BOOL CCalcUltimateLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strArrCombo;
	CCalcData	*pCalcData	= m_pStd->GetDataManage()->GetCalcData();

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		long	nRow	= m_Grid.GetFocusCell().row;
		long	nCol	= m_Grid.GetFocusCell().col;
		long	nQtyRow	= m_Grid.GetRowCount();

		if(nRow==nQtyRow-4)
		{
			strArrCombo.Add("VSL type S6-4");
			m_Grid.SetComboString(strArrCombo);
		}
		else if (nRow==6 && nCol==3)
		{
			double dSpaceComb[5] = { 0, 0, 0, 0, 0 };
			double dSpace = pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_CtcB;

			dSpaceComb[0] = dSpace * 0.25;
			dSpaceComb[1] = dSpace * 0.50;
			dSpaceComb[2] = dSpace * 1.00;
			dSpaceComb[3] = dSpace * 2.00;
			dSpaceComb[4] = dSpace * 4.00;

			for (long n=0; n<5; n++)
			{
				CString szVal;
				szVal.Format("%.2f", dSpaceComb[n]);
				strArrCombo.Add(szVal);
			}
			m_Grid.SetComboString(strArrCombo);
		}

		else if (nRow==13 && nCol==3)
		{
			double dSpaceComb[5] = { 0, 0, 0, 0, 0 };
			double dSpace = pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_CtcB;
			dSpaceComb[0] = dSpace * 0.25;
			dSpaceComb[1] = dSpace * 0.50;
			dSpaceComb[2] = dSpace * 1.00;
			dSpaceComb[3] = dSpace * 2.00;
			dSpaceComb[4] = dSpace * 4.00;
			
			for (long n=0; n<5; n++)
			{
				CString szVal;
				szVal.Format("%.2f", dSpaceComb[n]);
				strArrCombo.Add(szVal);
			}
			m_Grid.SetComboString(strArrCombo);
		}
		else if (nRow==20 && nCol==3)
		{
			double dSpaceComb[5] = { 0, 0, 0, 0, 0 };
			double dSpace = pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_CtcB;

			dSpaceComb[0] = dSpace * 0.25;
			dSpaceComb[1] = dSpace * 0.50;
			dSpaceComb[2] = dSpace * 1.00;
			dSpaceComb[3] = dSpace * 2.00;
			dSpaceComb[4] = dSpace * 4.00;
			
			for (long n=0; n<5; n++)
			{
				CString szVal;
				szVal.Format("%.2f", dSpaceComb[n]);
				strArrCombo.Add(szVal);
			}
			m_Grid.SetComboString(strArrCombo);
		}

		else if(m_Grid.GetCellType(nRow, nCol)==CT_COMBO)
		{
			strArrCombo.Add(m_sRebar+"13");
			strArrCombo.Add(m_sRebar+"16");
			strArrCombo.Add(m_sRebar+"19");
			strArrCombo.Add(m_sRebar+"22");
			strArrCombo.Add(m_sRebar+"25");
			strArrCombo.Add(m_sRebar+"29");
			strArrCombo.Add(m_sRebar+"32");
			strArrCombo.Add(m_sRebar+"35");
			m_Grid.SetComboString(strArrCombo);
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CCalcUltimateLoadDlg::OnSelchangeTabRebar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetActiveGrid();
	SetGridTitle();
	SetDataInit();
	//DrawInputDomyunView(TRUE);
}
