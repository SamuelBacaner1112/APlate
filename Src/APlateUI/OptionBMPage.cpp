// OptionBMPage.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionBMPage dialog
IMPLEMENT_DYNCREATE(COptionBMPage, CPropertyPage)

COptionBMPage::COptionBMPage() : CPropertyPage(COptionBMPage::IDD)	
{
	//{{AFX_DATA_INIT(COptionBMPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionBMPage::~COptionBMPage()
{
}

void COptionBMPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionBMPage)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);	
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionBMPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionBMPage)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionBMPage message handlers
BOOL COptionBMPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	InitTreeData(FALSE);
	InitGridData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionBMPage::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	BOOL bSang		 = (pOpt->m_BM_OutSelect & MAKE_BM_SANG)			? TRUE : FALSE;
	BOOL bStandard	 = (pOpt->m_BM_OutSelect & MAKE_BM_STDANDARD_SANG)	? TRUE : FALSE;
	BOOL bThick		 = (pOpt->m_BM_OutSelect & MAKE_BM_THICK)			? TRUE : FALSE;
	BOOL bCar		 = (pOpt->m_BM_OutSelect & MAKE_BM_CAR)				? TRUE : FALSE;
	BOOL bCarTotal	 = (pOpt->m_BM_OutSelect & MAKE_BM_CARTOTAL)		? TRUE : FALSE;
	BOOL bSection	 = (pOpt->m_BM_OutSelect & MAKE_BM_SECTIONTYPE)		? TRUE : FALSE;
	BOOL bBuje		 = (pOpt->m_BM_OutSelect & MAKE_BM_BUJE)			? TRUE : FALSE;
	BOOL bBujeTotal	 = (pOpt->m_BM_OutSelect & MAKE_BM_BUJE_TOTAL)		? TRUE : FALSE;
	BOOL bGirderTotal= (pOpt->m_BM_OutSelect & MAKE_BM_GIRDER_TOTAL)	? TRUE : FALSE;
	BOOL bSizeTypeTotal= (pOpt->m_BM_OutSelect & MAKE_BM_BUJEKIND_TOTAL)	? TRUE : FALSE;	
	BOOL bDeductSang = (pOpt->m_BM_OutSelect & MAKE_BM_DEDUCT_SANG)		? TRUE : FALSE;
	BOOL bDeductBuje = (pOpt->m_BM_OutSelect & MAKE_BM_DEDUCT_BUJE)		? TRUE : FALSE;
	//
	BOOL bPaintSang	 = (pOpt->m_BM_PaintSelect & PAINTMAKE_SANG)	? TRUE : FALSE;
	BOOL bPaintBolt  = (pOpt->m_BM_PaintSelect & PAINTMAKE_BOLT_TOTAL)	? TRUE : FALSE;
	BOOL bPaintBuje  = (pOpt->m_BM_PaintSelect & PAINTMAKE_BUJE_SANG)	? TRUE : FALSE;
	BOOL bPaintTotal = (pOpt->m_BM_PaintSelect & PAINTMAKE_SUMMARY)	? TRUE : FALSE;

	BOOL bWeldSang  = (pOpt->m_BM_WeldSelect & WELDMAKE_SANG)		? TRUE : FALSE;
	BOOL bWeldType  = (pOpt->m_BM_WeldSelect & WELDMAKE_WELDTYPE)	? TRUE : FALSE;
	BOOL bWeldThick = (pOpt->m_BM_WeldSelect & WELDMAKE_WELDTHICK)	? TRUE : FALSE;

	long nTypeJewonOut = pOpt->m_nTypeJewonOut;

	if(bDefaultLoad)
	{
		bSang		= TRUE;
		bStandard   = TRUE;
		bCar		= TRUE;
		bSection	= TRUE;
		bCarTotal	= TRUE;
		bThick		= TRUE;
		bBuje		= TRUE;
		bBujeTotal	= TRUE;
		bGirderTotal= TRUE;
		bSizeTypeTotal = TRUE;
		bDeductSang	= TRUE;
		bDeductBuje	= TRUE;
		bPaintSang	= TRUE;
		bPaintBuje  = TRUE;
		bPaintTotal = TRUE;
		bPaintBolt  = TRUE;
		bWeldSang   = TRUE;
		bWeldType   = TRUE;
		bWeldThick  = TRUE;
		nTypeJewonOut = 0;
	}

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();
	
	m_Options.SetOptionItem("강재집계표 구성", IDB_ITEMS, IDB_ITEMS);	
	m_Options.SetOption("BM_SANG", bSang ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_SANG", "\t상세집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_STANDARD", bStandard ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_STANDARD", "\t표준집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_THICK", bThick ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_THICK", "\t강종별두께별집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_CAR", bCar ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_CAR", "\t차량적재집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_CARTOTAL", bCarTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_CARTOTAL", "\t차량적재총괄집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_SECTION", bSection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_SECTION", "\t단면타입별집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_BUJE", bBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_BUJE", "\t부재별집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_BUJE_TOTAL", bBujeTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_BUJE_TOTAL", "\t부재별총괄집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_GIRDER_TOTAL", bGirderTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_GIRDER_TOTAL", "\t거더별총괄집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_SIZETYPE_TOTAL", bSizeTypeTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);	
	m_Options.SetOptionItem("BM_SIZETYPE_TOTAL", "\t부재종류별총괄집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);		
	m_Options.SetOption("BM_DEDUCTSANG", bDeductSang ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_DEDUCTSANG", "\t상세별공제집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("BM_DEDUCTBUJE", bDeductBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("BM_DEDUCTBUJE", "\t부재별공제집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("도장집계표 구성", IDB_ITEMS, IDB_ITEMS);	
	m_Options.SetOption("PAINTMAKE_SANG", bPaintSang ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PAINTMAKE_SANG", "\t상세집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PAINTMAKE_SUMMARY", bPaintTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PAINTMAKE_SUMMARY", "\t총괄집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PAINTMAKE_BOLT", bPaintBolt ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PAINTMAKE_BOLT", "\t볼트집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PAINTMAKE_BUJE",  bPaintBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PAINTMAKE_BUJE", "\t부재별집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("용접집계표 구성", IDB_ITEMS, IDB_ITEMS);	
	m_Options.SetOption("WELDMAKE_SANG", bWeldSang ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("WELDMAKE_SANG", "\t상세집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("WELDMAKE_WELDTYPE", bWeldType ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("WELDMAKE_WELDTYPE", "\t단면별집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("WELDMAKE_WELDTHICK", bWeldThick ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("WELDMAKE_WELDTHICK", "\t두께별집계표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("출력옵션", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOptionItem("\t제원출력 방식", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("OUT_JEWON_TYPE", nTypeJewonOut);
		m_Options.SetOptionItem("OUT_JEWON_TYPE", "\t\t두께,너비,길이(T-W-L)", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("OUT_JEWON_TYPE", "\t\t길이,너비,두께(L-W-T)", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("OUT_JEWON_TYPE", "\t\t너비,두께,길이(W-T-L)", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("OUT_JEWON_TYPE", "\t\t길이,두께,너비(L-T-W)", 3, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionBMPage::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_BM_TableSu = 3;
		pOpt->m_BM_LineSu = 50;
		pOpt->m_BM_Paint_TableSu = 2;
		pOpt->m_BM_Paint_LineSu = 50;
		pOpt->m_BM_Weld_TableSu = 2;
		pOpt->m_BM_Weld_LineSu = 50;
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
	m_Grid.SetTextMatrix(1,0,"강재집계표 테이블 수",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"강재집계표 LINE 수",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"도장집계표 테이블 수",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(4,0,"도장집계표 LINE 수",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"용접집계표 테이블 수",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(6,0,"용접집계표 LINE 수",(UINT)DT_LEFT);
	
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.0f",pOpt->m_BM_TableSu,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.0f",pOpt->m_BM_LineSu,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.0f",pOpt->m_BM_Paint_TableSu,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.0f",pOpt->m_BM_Paint_LineSu,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.0f",pOpt->m_BM_Weld_TableSu,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(6,1,"%.0f",pOpt->m_BM_Weld_LineSu,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionBMPage::OnOK() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_BM_OutSelect = 0x0000;
	if(m_Options.GetOption("BM_SANG"))			pOpt->m_BM_OutSelect |= MAKE_BM_SANG;
	if(m_Options.GetOption("BM_STANDARD"))		pOpt->m_BM_OutSelect |= MAKE_BM_STDANDARD_SANG;
	if(m_Options.GetOption("BM_CAR"))			pOpt->m_BM_OutSelect |= MAKE_BM_CAR;
	if(m_Options.GetOption("BM_SECTION"))		pOpt->m_BM_OutSelect |= MAKE_BM_SECTIONTYPE;
	if(m_Options.GetOption("BM_CARTOTAL"))		pOpt->m_BM_OutSelect |= MAKE_BM_CARTOTAL;
	if(m_Options.GetOption("BM_THICK"))			pOpt->m_BM_OutSelect |= MAKE_BM_THICK;
	if(m_Options.GetOption("BM_BUJE"))			pOpt->m_BM_OutSelect |= MAKE_BM_BUJE;
	if(m_Options.GetOption("BM_BUJE_TOTAL"))	pOpt->m_BM_OutSelect |= MAKE_BM_BUJE_TOTAL;
	if(m_Options.GetOption("BM_GIRDER_TOTAL"))	pOpt->m_BM_OutSelect |= MAKE_BM_GIRDER_TOTAL;
	if(m_Options.GetOption("BM_SIZETYPE_TOTAL"))	pOpt->m_BM_OutSelect |= MAKE_BM_BUJEKIND_TOTAL;
	if(m_Options.GetOption("BM_DEDUCTSANG"))	pOpt->m_BM_OutSelect |= MAKE_BM_DEDUCT_SANG;
	if(m_Options.GetOption("BM_DEDUCTBUJE"))	pOpt->m_BM_OutSelect |= MAKE_BM_DEDUCT_BUJE;

	pOpt->m_BM_PaintSelect = 0x0000;
	if(m_Options.GetOption("PAINTMAKE_SANG"))		pOpt->m_BM_PaintSelect |= PAINTMAKE_SANG;
	if(m_Options.GetOption("PAINTMAKE_SUMMARY"))	pOpt->m_BM_PaintSelect |= PAINTMAKE_SUMMARY;
	if(m_Options.GetOption("PAINTMAKE_BOLT"))		pOpt->m_BM_PaintSelect |= PAINTMAKE_BOLT_TOTAL;
	if(m_Options.GetOption("PAINTMAKE_BUJE"))		pOpt->m_BM_PaintSelect |= PAINTMAKE_BUJE_SANG;

	pOpt->m_BM_WeldSelect = 0x0000;
	if(m_Options.GetOption("WELDMAKE_SANG"))		pOpt->m_BM_WeldSelect |= WELDMAKE_SANG;
	if(m_Options.GetOption("WELDMAKE_WELDTYPE"))	pOpt->m_BM_WeldSelect |= WELDMAKE_WELDTYPE;
	if(m_Options.GetOption("WELDMAKE_WELDTHICK"))	pOpt->m_BM_WeldSelect |= WELDMAKE_WELDTHICK;

	pOpt->m_nTypeJewonOut = m_Options.GetOption("OUT_JEWON_TYPE");

	pOpt->m_BM_TableSu		 = m_Grid.GetTextMatrixLong(1,1);
	pOpt->m_BM_LineSu		 = m_Grid.GetTextMatrixLong(2,1);
	pOpt->m_BM_Paint_TableSu = m_Grid.GetTextMatrixLong(3,1);
	pOpt->m_BM_Paint_LineSu  = m_Grid.GetTextMatrixLong(4,1);
	pOpt->m_BM_Weld_TableSu  = m_Grid.GetTextMatrixLong(5,1);
	pOpt->m_BM_Weld_LineSu   = m_Grid.GetTextMatrixLong(6,1);

	pStd->GetDrawPageMng()->SetModified(PAGE_BM);

	CPropertyPage::OnOK();
}

BOOL COptionBMPage::OnSetActive() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;
	
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_BM;
		
	return CPropertyPage::OnSetActive();
}

void COptionBMPage::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitGridData(TRUE);
	InitTreeData(TRUE);
}
