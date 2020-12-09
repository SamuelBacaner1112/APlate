// OptionShoe.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionShoe property page

IMPLEMENT_DYNCREATE(COptionShoe, CPropertyPage)

COptionShoe::COptionShoe() : CPropertyPage(COptionShoe::IDD)
{
	//{{AFX_DATA_INIT(COptionShoe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionShoe::~COptionShoe()
{
}

void COptionShoe::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionShoe)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionShoe, CPropertyPage)
	//{{AFX_MSG_MAP(COptionShoe)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionShoe message handlers
BOOL COptionShoe::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData();
	InitGridData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionShoe::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	//
	m_Options.SetOptionItem("출력도면 구성", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOptionItem("\t교량받침 배치도", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("m_Shoe_bShoeCoord", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bShoeCoord) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("m_Shoe_bShoeCoord", "\t\t교량받침 좌표도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("m_Shoe_bTableCoord", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bTableCoord) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("m_Shoe_bTableCoord", "\t\t좌표테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("m_Shoe_bTableShoeBM", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bTableShoeBM) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("m_Shoe_bTableShoeBM", "\t\t교량받침 집계테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("m_Shoe_bTableLegend", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bTableLegend) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("m_Shoe_bTableLegend", "\t\t범례", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);


		m_Options.SetOptionItem("\tELEVATION 산출근거도", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("m_Elev_bTable", (bDefaultLoad ? TRUE : pOpt->m_Elev_bTable) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("m_Elev_bTable", "\t\t산출근거테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("m_Elev_bSection", (bDefaultLoad ? TRUE : pOpt->m_Elev_bSection) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("m_Elev_bSection", "\t\t표준단면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("m_Elev_bShoeDetail", (bDefaultLoad ? TRUE : pOpt->m_Elev_bShoeDetail) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("m_Elev_bShoeDetail", "\t\t교량받침상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("교량받침 배치도 출력 옵션", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("m_Shoe_bCLSlabXY", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bCLSlabXY) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bCLSlabXY", "\t도로중심 슬래브 좌표 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("m_Shoe_bCLSttEndXY", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bCLSttEndXY) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bCLSttEndXY", "\t도로중심 시종점부 좌표 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("m_Shoe_bSlabXY", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bSlabXY) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bSlabXY", "\t슬래브 좌표 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("m_Shoe_bSttEndXY", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bSttEndXY) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bSttEndXY", "\t시종점부 좌표 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("m_Shoe_bLineInfo", (bDefaultLoad ? FALSE : pOpt->m_Shoe_bLineInfo) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bLineInfo", "\t선형정보 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("m_Shoe_bCoordZ", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bCoordZ) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bCoordZ", "\t3차원 좌표 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("m_Shoe_bAngle", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bAngle) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bAngle", "\t방위각 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("교량받침 배치도 치수 옵션", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("m_Shoe_bSpanCenter", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bSpanCenter) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bSpanCenter", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("m_Shoe_bTotalCenter", (bDefaultLoad ? TRUE : pOpt->m_Shoe_bTotalCenter) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("m_Shoe_bTotalCenter", "\t전체길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);


	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionShoe::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_Shoe_dScaleHori = 300;
		pOpt->m_Shoe_dScaleVert = 300;
		pOpt->m_Elev_dScaleTable = 100;
		pOpt->m_Elev_dScaleSection = 50;
		pOpt->m_Elev_dScaleShoeDetail = 10;
		pOpt->m_Shoe_dColumSu = 4;
	}
	
	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(7);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);				
	m_Grid.SetColumnWidth(0,Rect.Width()*2/3);
	m_Grid.SetColumnWidth(1,Rect.Width()*1/3);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));	
	m_Grid.SetTextBkColor(RGB(255,255,255));

	m_Grid.SetTextMatrix(0,0,"설정항목");
	m_Grid.SetTextMatrix(1,0,"슈배치도 수평",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"슈배치도 수직",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"ELEVATION 산출근거 테이블",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"표준단면도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"교량받침상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(6,0,"테이블 출력 지점 개수",(UINT)DT_LEFT);	
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_Shoe_dScaleHori,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_Shoe_dScaleVert,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_Elev_dScaleTable,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_Elev_dScaleSection,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.1f",pOpt->m_Elev_dScaleShoeDetail,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(6,1,"%.0f",pOpt->m_Shoe_dColumSu,(UINT)DT_RIGHT);	
	
	m_Grid.SetRedraw(TRUE,TRUE);	
}

void COptionShoe::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}


void COptionShoe::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	
	pOpt->m_Shoe_bShoeCoord   = m_Options.GetOption("m_Shoe_bShoeCoord");
	pOpt->m_Shoe_bTableCoord  = m_Options.GetOption("m_Shoe_bTableCoord");
	pOpt->m_Shoe_bTableShoeBM = m_Options.GetOption("m_Shoe_bTableShoeBM");
	pOpt->m_Shoe_bTableLegend = m_Options.GetOption("m_Shoe_bTableLegend");
	pOpt->m_Elev_bTable		  = m_Options.GetOption("m_Elev_bTable");	
	pOpt->m_Elev_bSection	  = m_Options.GetOption("m_Elev_bSection");	
	pOpt->m_Elev_bShoeDetail	  = m_Options.GetOption("m_Elev_bShoeDetail");	
	pOpt->m_Shoe_bCoordZ	  = m_Options.GetOption("m_Shoe_bCoordZ");
	pOpt->m_Shoe_bAngle	  = m_Options.GetOption("m_Shoe_bAngle");
	pOpt->m_Shoe_bLineInfo	  = m_Options.GetOption("m_Shoe_bLineInfo");
	pOpt->m_Shoe_bCLSlabXY	  = m_Options.GetOption("m_Shoe_bCLSlabXY");
	pOpt->m_Shoe_bCLSttEndXY	  = m_Options.GetOption("m_Shoe_bCLSttEndXY");
	pOpt->m_Shoe_bSlabXY	  = m_Options.GetOption("m_Shoe_bSlabXY");
	pOpt->m_Shoe_bSttEndXY	  = m_Options.GetOption("m_Shoe_bSttEndXY");
	pOpt->m_Shoe_bSpanCenter	  = m_Options.GetOption("m_Shoe_bSpanCenter");
	pOpt->m_Shoe_bTotalCenter	  = m_Options.GetOption("m_Shoe_bTotalCenter");

	pOpt->m_Shoe_dScaleHori = m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_Shoe_dScaleVert = m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_Elev_dScaleTable = m_Grid.GetTextMatrixDouble(3,1);
	pOpt->m_Elev_dScaleSection = m_Grid.GetTextMatrixDouble(4,1);
	pOpt->m_Elev_dScaleShoeDetail = m_Grid.GetTextMatrixDouble(5,1);
	pOpt->m_Shoe_dColumSu = m_Grid.GetTextMatrixLong(6,1);
	pStd->GetDrawPageMng()->SetModified(PAGE_SHOECOORD);	
	CPropertyPage::OnOK();
}

void COptionShoe::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionShoe::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;

	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_SHOE;
	
	return CPropertyPage::OnSetActive();
}
