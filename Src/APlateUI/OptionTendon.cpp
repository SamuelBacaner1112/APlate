// OptionTendon.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTendon property page

IMPLEMENT_DYNCREATE(COptionTendon, CPropertyPage)

COptionTendon::COptionTendon() : CPropertyPage(COptionTendon::IDD)
{
	//{{AFX_DATA_INIT(COptionTendon)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionTendon::~COptionTendon()
{
}

void COptionTendon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionTendon)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionTendon, CPropertyPage)
	//{{AFX_MSG_MAP(COptionTendon)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTendon message handlers
BOOL COptionTendon::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData(FALSE);
	InitGridData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionTendon::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bPlanView				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_PlanView;				// 평면도
	BOOL bSectionAA				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_SectionAA;			// 거더부 단면 상세
	long nDivType				=	bDefaultLoad ? 0	: pOpt->m_nTendon_DivType;				// 도면 분할 방식
	BOOL bMatchLine				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_MatchLine;			// 매치라인
//	BOOL bLineInfo				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_bPyungLineInfo;		// 선형정보

//	BOOL bDimsionTable			=	bDefaultLoad ? TRUE : pOpt->m_bSlab_DimTable;		// 치수변화 테이블
//	long nDimsionTableType		=	bDefaultLoad ? 1	: pOpt->m_nSlab_DimTableType;	// 치수변화 테이블 타입 
//	BOOL bDimsionTableFirst		=	bDefaultLoad ? FALSE : pOpt->m_bSlab_DimTableFirstPage;	// 첫페이지만 표시

	m_Options.SetOptionItem("도면 표현", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PLANVIEW", bPlanView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLANVIEW", "\t평면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("SECTIONAA", bSectionAA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SECTIONAA", "\t단면A-A 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("MATCHLINE", bMatchLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MATCHLINE", "\t매치라인 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

//		m_Options.SetOption("DIMTABLE", bDimsionTable? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//		m_Options.SetOptionItem("DIMTABLE", "\t치수변화 테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem("도면분할 방식", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIVTYPE", nDivType);
		m_Options.SetOptionItem("DIVTYPE","\t경간 단위로 분할", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIVTYPE","\t전체 교량을 한장으로", 3, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionTendon::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_dTendon_ScaleGen       = 100;	
		pOpt->m_dTendon_ScaleSectionAA = 30;
		pOpt->m_nTendon_DivNum         = 1;		
//		pOpt->m_dSlab_DimTableScale = 100;
//		pOpt->m_dSlab_DimTableHeightFactor = 1;
//		pOpt->m_dSlab_DimTableWidhtFactor = 1;
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(4);
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
	m_Grid.SetTextMatrix(1,0,"평면도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"단면 A-A상세",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(3,0,"출력단위 개수",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(3,0,"변화치수 테이블",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(4,0,"변화치수 테이블 높이계수",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(5,0,"변화치수 테이블 폭계수",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dTendon_ScaleGen,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dTendon_ScaleSectionAA,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.0f",pOpt->m_nSlab_DivNum,(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_dSlab_DimTableScale,(UINT)DT_RIGHT);	
//	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dSlab_DimTableHeightFactor,(UINT)DT_RIGHT);	
//	m_Grid.SetTextMatrix(5,1,"%.1f",pOpt->m_dSlab_DimTableWidhtFactor,(UINT)DT_RIGHT);	
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionTendon::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}

void COptionTendon::OnOK() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_bTendon_PlanView			= m_Options.GetOption("PLANVIEW");	// 평면도
	pOpt->m_bTendon_SectionAA			= m_Options.GetOption("SECTIONAA");		// 거더부 단면 상세
	pOpt->m_nTendon_DivType				= m_Options.GetOption("DIVTYPE");		// 도면 분할 방식
	pOpt->m_bTendon_MatchLine			= m_Options.GetOption("MATCHLINE");
//	pOpt->m_bSlab_DimTable				= m_Options.GetOption("DIMTABLE");				// 치수변화 테이블

	pOpt->m_dTendon_ScaleGen			= m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dTendon_ScaleSectionAA		= m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_nTendon_DivNum              = m_Grid.GetTextMatrixLong(3,1);

//	pOpt->m_dSlab_DimTableScale			= m_Grid.GetTextMatrixDouble(3,1);
//	pOpt->m_dSlab_DimTableHeightFactor	= m_Grid.GetTextMatrixDouble(4,1);
//	pOpt->m_dSlab_DimTableWidhtFactor	= m_Grid.GetTextMatrixDouble(5,1);

	pStd->GetDrawPageMng()->SetModified(PAGE_TENDON);

	CPropertyPage::OnOK();
}

BOOL COptionTendon::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_TENDON;
	
	return CPropertyPage::OnSetActive();
}


// OptionStd의 초기값과 일치시켜야함....
void COptionTendon::OnDefault()
{
	m_ListOption.DeleteAllItems();

	InitTreeData(TRUE);
	InitGridData(TRUE);
}
