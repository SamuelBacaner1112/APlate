// OptionTUGirder.cpp: implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTUGirder property page

IMPLEMENT_DYNCREATE(COptionTUGirder, CPropertyPage)

COptionTUGirder::COptionTUGirder() : CPropertyPage(COptionTUGirder::IDD)
{
	//{{AFX_DATA_INIT(COptionTUGirder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionTUGirder::~COptionTUGirder()
{
}

void COptionTUGirder::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionTUGirder)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionTUGirder, CPropertyPage)
	//{{AFX_MSG_MAP(COptionTUGirder)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTUGirder message handlers
int COptionTUGirder::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	return 0;
}

void COptionTUGirder::OnOK() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_bTU_Plan			= m_Options.GetOption("PLAN");
	pOpt->m_bTU_Jong			= m_Options.GetOption("JONG");
	pOpt->m_bTU_Detail			= m_Options.GetOption("DETAIL");
	pOpt->m_bTU_DetailSteel		= m_Options.GetOption("DETAILSTEEL");
	pOpt->m_bTU_TableSteel		= m_Options.GetOption("TABLESTEEL");
	pOpt->m_nTU_Trim			= m_Options.GetOption("TRIMTYPE");

	pOpt->m_dTUScale			= m_Grid.GetTextMatrixDouble(1,1);
//	pOpt->m_dTUScale_Plan		= m_Grid.GetTextMatrixDouble(2,1);
//	pOpt->m_dTUScale_Jong		= m_Grid.GetTextMatrixDouble(3,1);
	pOpt->m_dTUScale_Detail		= m_Grid.GetTextMatrixLong(2,1);

	pStd->GetDrawPageMng()->SetModified(PAGE_TUGIRDER);

	CPropertyPage::OnOK();
}

BOOL COptionTUGirder::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitTreeData();
	InitGridData();	
	return TRUE;
}

void COptionTUGirder::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}

void COptionTUGirder::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	long nTrimType = bDefaultLoad ? 0 : pOpt->m_nTU_Trim;	// Trim 여부

	m_Options.SetOptionItem("도면 표현", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("PLAN", (bDefaultLoad ? TRUE : pOpt->m_bTU_Plan) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PLAN", "\t평면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("TRIMTYPE", nTrimType);
		m_Options.SetOptionItem("TRIMTYPE","\t\t배근도 Trim구간 적용", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("TRIMTYPE","\t\t배근도 Trim구간 미적용", 1, IDB_RADIOON, IDB_RADIOOFF);
	m_Options.SetOption("JONG", (bDefaultLoad ? TRUE : pOpt->m_bTU_Jong) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG", "\t종단면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("DETAIL", (bDefaultLoad ? TRUE : pOpt->m_bTU_Detail) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("DETAIL", "\t단면 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("DETAILSTEEL", (bDefaultLoad ? TRUE : pOpt->m_bTU_DetailSteel) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("DETAILSTEEL", "\t철근 상세도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("TABLESTEEL", (bDefaultLoad ? TRUE : pOpt->m_bTU_TableSteel) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("TABLESTEEL", "\t철근 재료표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionTUGirder::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{		
		pOpt->m_dTUScale		= 50;
//		pOpt->m_dTUScale_Plan	= 50;
//		pOpt->m_dTUScale_Jong	= 50;
		pOpt->m_dTUScale_Detail	= 20;
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(3);
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
	m_Grid.SetTextMatrix(1,0,"TurnOver 거더 배근도",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(2,0,"평면도",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(3,0,"종단면도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(2,0,"단면 상세도",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dTUScale,(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dTUScale_Plan,(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_dTUScale_Jong,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.0f",pOpt->m_dTUScale_Detail,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionTUGirder::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}
BOOL COptionTUGirder::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_TU;
	
	return CPropertyPage::OnSetActive();
}
