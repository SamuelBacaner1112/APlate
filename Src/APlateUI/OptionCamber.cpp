// OptionCamber.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionCamber property page

IMPLEMENT_DYNCREATE(COptionCamber, CPropertyPage)

COptionCamber::COptionCamber() : CPropertyPage(COptionCamber::IDD)
{
	//{{AFX_DATA_INIT(COptionCamber)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionCamber::~COptionCamber()
{
}

void COptionCamber::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionCamber)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionCamber, CPropertyPage)
	//{{AFX_MSG_MAP(COptionCamber)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionCamber message handlers
int COptionCamber::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	
	
	return 0;
}

void COptionCamber::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class

	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_nCamber_Type			= m_Options.GetOption("TYPE");
	pOpt->m_bCamber_Table			= m_Options.GetOption("TABLE");
	pOpt->m_bCamber_AllGirder		= m_Options.GetOption("ALLGIRDER");
	pOpt->m_bCamber_AllGirderFull	= m_Options.GetOption("ALLGIRDERFULL");
//	pOpt->m_bCamber_Note			= m_Options.GetOption("NOTE");
	pOpt->m_bCamber_Legend			= m_Options.GetOption("LEGEND");
	pOpt->m_nCamber_DivType			= m_Options.GetOption("DIVTYPE");
	
	pOpt->m_dCamber_HScale = m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dCamber_VScale = m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_nCamber_TableColSu = m_Grid.GetTextMatrixLong(3,1);
	pOpt->m_dCamber_TableHScale = m_Grid.GetTextMatrixDouble(4,1);
	pOpt->m_nCamber_UnitEa = m_Grid.GetTextMatrixLong(5,1);
	
	pStd->GetDrawPageMng()->SetModified(PAGE_CAMBER);
//	pStd->GetDrawPageMng()->m_bPageInit = FALSE;

	CPropertyPage::OnOK();
}

BOOL COptionCamber::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitTreeData();
	InitGridData();	
	return TRUE;
}

void COptionCamber::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}

void COptionCamber::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	m_Options.SetOptionItem("도면 타입", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("TYPE", (bDefaultLoad ? 0 : pOpt->m_nCamber_Type));
/*	m_Options.SetOptionItem("TYPE", "\t제작도 타입1", 0, IDB_RADIOON, IDB_RADIOOFF);	
	m_Options.SetOptionItem("TYPE", "\t제작도 타입2", 1, IDB_RADIOON, IDB_RADIOOFF);	*/	
	m_Options.SetOptionItem("TYPE", "\t설계도 타입", 0, IDB_RADIOON, IDB_RADIOOFF);		

	m_Options.SetOptionItem("도면 표현", IDB_ITEMS, IDB_ITEMS);	
	m_Options.SetOption("TABLE", (bDefaultLoad ? TRUE : pOpt->m_bCamber_Table) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("TABLE", "\tCAMBER TABLE", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
//	m_Options.SetOption("NOTE", (bDefaultLoad ? TRUE : pOpt->m_bCamber_Note) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//	m_Options.SetOptionItem("NOTE", "\t노트사항", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
	m_Options.SetOption("LEGEND", (bDefaultLoad ? TRUE : pOpt->m_bCamber_Legend) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("LEGEND", "\t범례", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem("도면분할 방식", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIVTYPE", (bDefaultLoad ? 1	: pOpt->m_nCamber_DivType));
		m_Options.SetOptionItem("DIVTYPE","\t경간 단위로 분할", 0, IDB_RADIOON, IDB_RADIOOFF);
//		m_Options.SetOptionItem("DIVTYPE","\t현장이음 단위로 분할", 1, IDB_RADIOON, IDB_RADIOOFF);
//		m_Options.SetOptionItem("DIVTYPE","\t가로보 개수로 분할", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIVTYPE","\t전체 교량을 한장으로", 1, IDB_RADIOON, IDB_RADIOOFF);
	
	m_Options.SetOptionItem("출력 옵션", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("ALLGIRDER", (bDefaultLoad ? TRUE : pOpt->m_bCamber_AllGirder) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("ALLGIRDER", "\t거더별 전체 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("ALLGIRDERFULL", (bDefaultLoad ? FALSE : pOpt->m_bCamber_AllGirderFull) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("ALLGIRDERFULL", "\t전체거더 한장으로 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionCamber::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{		
		pOpt->m_dCamber_HScale = 200;
		pOpt->m_dCamber_VScale = 200;
		pOpt->m_nCamber_TableColSu = 30;
		pOpt->m_dCamber_TableHScale = 1;
		pOpt->m_nCamber_UnitEa = 1;
	}
	
	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(6);
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
	m_Grid.SetTextMatrix(1,0,"솟음도(수평)",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"솟음도(수직)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"테이블 열 개수",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"테이블 행 높이",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"출력단위 개수",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dCamber_HScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dCamber_VScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.0f",pOpt->m_nCamber_TableColSu,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dCamber_TableHScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.0f",pOpt->m_nCamber_UnitEa,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);	
}

void COptionCamber::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}
BOOL COptionCamber::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_CAMBER;
	
	return CPropertyPage::OnSetActive();
}
