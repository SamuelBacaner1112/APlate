// CalcSectionConceptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcSectionConceptDlg dialog

CCalcSectionConceptDlg::CCalcSectionConceptDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcSectionConceptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcSectionConceptDlg)
	m_bWind = FALSE;
	m_nHLLExePos = -1;
	m_bNoDisByNoGuard = FALSE;
	//}}AFX_DATA_INIT
	m_sSelectHLNum = _T("");
	//m_sSelectStation = _T("");
}

void CCalcSectionConceptDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcSectionConceptDlg)
	DDX_Check(pDX, IDC_F_CONCEPT_CHECK4, m_bWind);
	DDX_Radio(pDX, IDC_F_CONCEPT_RADIO1, m_nHLLExePos);
	DDX_Control(pDX, IDC_DB_DIS_EDIT, m_dDBDisFromEdge);
	DDX_GridControl(pDX, IDC_GRID1, m_Grid);
	DDX_Check(pDX, IDC_NODIST_ONNOGUARD, m_bNoDisByNoGuard);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCalcSectionConceptDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcSectionConceptDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID1, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcSectionConceptDlg message handlers

BOOL CCalcSectionConceptDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CCalcSectionConceptDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcSectionConceptDlg::OnPreInitDialog() 
{
	SetResize(IDC_STATIC6,			SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_F_CONCEPT_RADIO1,	SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_F_CONCEPT_RADIO2,	SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_STATIC8,			SZ_TOP_LEFT,	SZ_TOP_RIGHT);
	SetResize(IDC_STATIC3,			SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_DB_DIS_EDIT,		SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_STATIC4,			SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_NODIST_ONNOGUARD,	SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_STATIC5,			SZ_TOP_LEFT,	SZ_TOP_LEFT);
	SetResize(IDC_STATIC9,			SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID1,			SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);

	SetDlgItemControl();

	// 활하중 모델링 기준위치 선택
	InitSelectLiveHL();
}

void CCalcSectionConceptDlg::SetDataDefault()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	
	// UNIT kN m
	pData->m_TankDis    = 0.600;
	pData->m_TankWidth  = 2.600;
	pData->m_TrailDis   = 0.600;
	pData->m_TrailWidth = 2.225;
	
	pData->m_TankWeight = 544.2;
	pData->m_TankEdit1  = 4.500;
	pData->m_TankEdit2  = 0.700;	
	pData->m_TankEdit3  = 1.900;	
	pData->m_TankImpact	= 15;	
	
	pData->m_TrailEdit1	= 3.600;
	pData->m_TrailEdit2	= 1.500;
	pData->m_TrailEdit3	= 4.500;
	pData->m_TrailEdit4	= 1.200;
	pData->m_TrailEdit5	= 0.975;
	pData->m_TrailEdit6	= 1.250;	
	pData->m_TrailImpact= 15;	
	
	pData->m_TrailP1    = 40.815;
	pData->m_TrailP2    = 81.630;
	pData->m_TrailP3    = 58.955;

	pData->m_bNoDistOnNoGuard	= FALSE;
	pData->m_HLType				= 0;
	pData->m_DBFromEdge			= 0.600;
	pData->m_bWind				= FALSE;
	

	SetDataInit();
}
void CCalcSectionConceptDlg::SetDataGridTitle()
{
	long	nRows	= 21;
	long	nCols	= 5;
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 120);
	m_Grid.SetColumnWidth(2, 80);
	m_Grid.SetColumnWidth(3, 60);
	m_Grid.SetColumnWidth(4, 400);
	for(long nRow=0;nRow<nRows;nRow++)
	{
		m_Grid.SetCellFixType(nRow, 0, GVN_ONECELL_COLFIX);
		m_Grid.SetCellFixType(nRow, 1, GVN_ONECELL_COLFIX);
		m_Grid.SetItemState(nRow, 4, GVIS_READONLY);
	}
	
	m_Grid.SetMergeRow(0, 20, 4);
	CBitmap *pBitmapTank	= new CBitmap;
	pBitmapTank->LoadBitmap(IDB_TANK);
	m_Grid.AddBitmap(0,4,pBitmapTank);
	m_Grid.SetTextMatrix(0,	0,	"< 탱크 궤도 하중 >");	m_Grid.SetMergeCol(0, 0, 3);
	m_Grid.SetTextMatrix(1, 0,	"연석으로부터의 거리");	m_Grid.SetMergeCol(1, 0, 1);
	m_Grid.SetTextMatrix(1, 3,	"m");					m_Grid.SetItemState(1, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(2, 0,	"활하중의 충격계수");	m_Grid.SetMergeCol(2, 0, 1);
	m_Grid.SetTextMatrix(2, 3,	"%적용");				m_Grid.SetItemState(2, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(3, 0,	"탱크 총 중량");		m_Grid.SetMergeCol(3, 0, 1);
	m_Grid.SetTextMatrix(3, 3,	"kN");					m_Grid.SetItemState(3, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(4, 0,	"작용하중위치");		m_Grid.SetMergeRow(4, 7, 0);
	m_Grid.SetTextMatrix(4, 1,	"궤도접지길이(L1)");
	m_Grid.SetTextMatrix(4, 3,	"m");					m_Grid.SetItemState(4, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(5, 1,	"궤도폭(B1)");
	m_Grid.SetTextMatrix(5, 3,	"m");					m_Grid.SetItemState(5, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(6, 1,	"궤도내측간거리(B2)");
	m_Grid.SetTextMatrix(6, 3,	"m");					m_Grid.SetItemState(6, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(7, 1,	"하중간거리(Ptk)");
	m_Grid.SetTextMatrix(7, 3,	"m");					m_Grid.SetItemState(7, 3, GVIS_READONLY);
	
	m_Grid.SetTextMatrix(8, 0,	"< 탱크 트레일러 하중 >");	m_Grid.SetMergeCol(8, 0, 3);
	m_Grid.SetTextMatrix(9, 0,	"연석으로부터의 거리");	m_Grid.SetMergeCol(9, 0, 1);
	m_Grid.SetTextMatrix(9, 3,	"m");					m_Grid.SetItemState(9, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(10, 0, "활하중의 충격계수");	m_Grid.SetMergeCol(10, 0, 1);
	m_Grid.SetTextMatrix(10, 3, "%적용");				m_Grid.SetItemState(10, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(11, 0, "차축중량");			m_Grid.SetMergeRow(11, 13, 0);
	m_Grid.SetTextMatrix(11, 1, "P1");
	m_Grid.SetTextMatrix(11, 3, "kN");					m_Grid.SetItemState(11, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(12, 1, "P2");
	m_Grid.SetTextMatrix(12, 3, "kN");					m_Grid.SetItemState(12, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(13, 1, "P3");
	m_Grid.SetTextMatrix(13, 3, "kN");					m_Grid.SetItemState(13, 3, GVIS_READONLY);
	
	m_Grid.SetTextMatrix(14, 0,	"작용하중위치");		m_Grid.SetMergeRow(14, 20, 0);
	m_Grid.SetTextMatrix(14, 1, "차축간거리(L1)");
	m_Grid.SetTextMatrix(14, 3, "m");					m_Grid.SetItemState(14, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(15, 1, "차축간거리(L2)");
	m_Grid.SetTextMatrix(15, 3, "m");					m_Grid.SetItemState(15, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(16, 1, "차축간거리(L3)");
	m_Grid.SetTextMatrix(16, 3, "m");					m_Grid.SetItemState(16, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(17, 1, "차축간거리(L4)");
	m_Grid.SetTextMatrix(17, 3, "m");					m_Grid.SetItemState(17, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(18, 1, "바퀴폭(B1)");
	m_Grid.SetTextMatrix(18, 3, "m");					m_Grid.SetItemState(18, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(19, 1, "바퀴내측간거리(B2)");
	m_Grid.SetTextMatrix(19, 3, "m");					m_Grid.SetItemState(19, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(20, 1, "하중간거리(Ptr)");
	m_Grid.SetTextMatrix(20, 3, "m");					m_Grid.SetItemState(20, 3, GVIS_READONLY);
	m_Grid.SetRedraw(TRUE, TRUE);

}
void CCalcSectionConceptDlg::SetDataInit()
{
	CCalcData	*pData = m_pStd->GetDataManage()->GetCalcData();
	CFemHLLCalc FemHLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	//Grid
	SetDataGridTitle();
	
	m_Grid.SetTextMatrix(1,		2,	"%.3f",	pData->m_TankDis);
	m_Grid.SetTextMatrix(2,		2,	"%.3f", pData->m_TankImpact);
	m_Grid.SetTextMatrix(3,		2,	"%.3f", pData->m_TankWeight);
	m_Grid.SetTextMatrix(4,		2,	"%.3f", pData->m_TankEdit1);
	m_Grid.SetTextMatrix(5,		2,	"%.3f", pData->m_TankEdit2);
	m_Grid.SetTextMatrix(6,		2,	"%.3f", pData->m_TankEdit3);
	m_Grid.SetTextMatrix(7,		2,	"%.3f", pData->m_TankWidth);	m_Grid.SetItemState(7, 2, GVIS_READONLY);
	m_Grid.SetTextMatrix(9,		2,	"%.3f", pData->m_TrailDis);
	m_Grid.SetTextMatrix(10,	2,	"%.3f", pData->m_TrailImpact);
	m_Grid.SetTextMatrix(11,	2,	"%.3f", pData->m_TrailP1);
	m_Grid.SetTextMatrix(12,	2,	"%.3f", pData->m_TrailP2);
	m_Grid.SetTextMatrix(13,	2,	"%.3f", pData->m_TrailP3);
	m_Grid.SetTextMatrix(14,	2,	"%.3f", pData->m_TrailEdit1);
	m_Grid.SetTextMatrix(15,	2,	"%.3f", pData->m_TrailEdit2);
	m_Grid.SetTextMatrix(16,	2,	"%.3f", pData->m_TrailEdit3);
	m_Grid.SetTextMatrix(17,	2,	"%.3f", pData->m_TrailEdit4);
	m_Grid.SetTextMatrix(18,	2,	"%.3f", pData->m_TrailEdit5);
	m_Grid.SetTextMatrix(19,	2,	"%.3f", pData->m_TrailEdit6);
	m_Grid.SetTextMatrix(20,	2,	"%.3f", pData->m_TrailWidth);	m_Grid.SetItemState(20, 2, GVIS_READONLY);

	m_Grid.SetRedraw(TRUE, TRUE);
	
	//Control
	
	//횡분배 실행 위치
	m_nHLLExePos = pData->m_HLType;

	if(m_nHLLExePos<0) m_nHLLExePos = 0;
	
	//풍하중
	if(!pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && !pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm)
	{
		pData->m_bWind = FALSE;
		GetDlgItem(IDC_F_CONCEPT_CHECK4)->EnableWindow(FALSE);
	}
	else 
		GetDlgItem(IDC_F_CONCEPT_CHECK4)->EnableWindow(TRUE);

	m_bWind = pData->m_bWind;
	
	//활하중 재하 방법

	// 활하중 재하 방법 결정
	if(pData->m_nHLiveLoadType == -1)
	{
		FemHLLCalc.CalcHLiveLoadType();	
	}

	CString sText = _T("");

	// 연석거리
	sText.Format("%g", pData->m_DBFromEdge);
	m_dDBDisFromEdge.SetWindowText(sText);

	UpdateData(FALSE);
}
void CCalcSectionConceptDlg::SetDataSave()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	
	//Grid
	pData->m_TankDis	= m_Grid.GetTextMatrixDouble(1,	2);
	pData->m_TankImpact	= m_Grid.GetTextMatrixDouble(2, 2);
	pData->m_TankWeight	= m_Grid.GetTextMatrixDouble(3, 2);
	pData->m_TankEdit1	= m_Grid.GetTextMatrixDouble(4, 2);
	pData->m_TankEdit2	= m_Grid.GetTextMatrixDouble(5, 2);
	pData->m_TankEdit3	= m_Grid.GetTextMatrixDouble(6, 2);
	pData->m_TankWidth	= m_Grid.GetTextMatrixDouble(7, 2);
	pData->m_TrailDis	= m_Grid.GetTextMatrixDouble(9, 2);
	pData->m_TrailImpact= m_Grid.GetTextMatrixDouble(10, 2);
	pData->m_TrailP1	= m_Grid.GetTextMatrixDouble(11, 2);
	pData->m_TrailP2	= m_Grid.GetTextMatrixDouble(12, 2);
	pData->m_TrailP3	= m_Grid.GetTextMatrixDouble(13, 2);
	pData->m_TrailEdit1	= m_Grid.GetTextMatrixDouble(14, 2);
	pData->m_TrailEdit2	= m_Grid.GetTextMatrixDouble(15, 2);
	pData->m_TrailEdit3	= m_Grid.GetTextMatrixDouble(16, 2);
	pData->m_TrailEdit4	= m_Grid.GetTextMatrixDouble(17, 2);
	pData->m_TrailEdit5	= m_Grid.GetTextMatrixDouble(18, 2);
	pData->m_TrailEdit6	= m_Grid.GetTextMatrixDouble(19, 2);
	pData->m_TrailWidth	= m_Grid.GetTextMatrixDouble(20, 2);

	//횡분배 실행 위치
	if(m_nHLLExePos<0) m_nHLLExePos=0;
	pData->m_HLType     = m_nHLLExePos;
	
	//풍하중
	pData->m_bWind = m_bWind;
	
	CString sText = _T("");

	// 연석거리
	m_dDBDisFromEdge.GetWindowText(sText);

	pData->m_DBFromEdge = atof(sText);
}

void CCalcSectionConceptDlg::InitSelectLiveHL()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	
	m_sSelectHLNum.Format("%d", pData->m_CurLiveHL + 1);	
	
	//long nOrgHL = pData->m_CurLiveHL;
	
	//m_sSelectStation = GetStationForStringOut(pData->m_dStationHLLPos[nOrgHL], 3, TRUE);	
	
	UpdateData(FALSE);	
}

void CCalcSectionConceptDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCalcData	*pData = m_pStd->GetDataManage()->GetCalcData();
	CFemHLLCalc FemHLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	

	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	int nRow = pGridView->iRow;

	if (nRow==5 || nRow==6)
	{
		double d1 = m_Grid.GetTextMatrixDouble(5, 2);
		double d2 = m_Grid.GetTextMatrixDouble(6, 2);
		pData->m_TankWidth = d1 + d2;
		m_Grid.SetTextMatrix(7,	2,	"%.3f", pData->m_TankWidth);
	}

	if (nRow==18 || nRow==19)
	{
		double d1 = m_Grid.GetTextMatrixDouble(18, 2);
		double d2 = m_Grid.GetTextMatrixDouble(19, 2);
		pData->m_TrailWidth = d1 + d2;
		m_Grid.SetTextMatrix(20,	2,	"%.3f", pData->m_TrailWidth);
	}

	SetDataSave();
	SetDataInit();
}

void CCalcSectionConceptDlg::SetDlgItemControl()
{
	CDataManage *pManage = m_pStd->GetDataManage();
	CGlobarOption *pOpt	= pManage->GetGlobalOption();
	CCalcData *pData = pManage->GetCalcData();

	// TU 거더일때에는 모든 가로보 위치 검토 비활성화
	long nBridgeType = pOpt->GetDesignBridgeType();

	BOOL bEnable = TRUE;
	if(nBridgeType == 2)
	{
		bEnable = FALSE;
		m_nHLLExePos = pData->m_HLType = 0;
	}

	GetDlgItem(IDC_F_CONCEPT_RADIO2)->EnableWindow(bEnable);
}
