// OptionLineInfo.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionLineInfo property page

IMPLEMENT_DYNCREATE(COptionLineInfo, CPropertyPage)

COptionLineInfo::COptionLineInfo() : CPropertyPage(COptionLineInfo::IDD)
{
	//{{AFX_DATA_INIT(COptionLineInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionLineInfo::~COptionLineInfo()
{
}

void COptionLineInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionLineInfo)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionLineInfo, CPropertyPage)
	//{{AFX_MSG_MAP(COptionLineInfo)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionLineInfo message handlers

void COptionLineInfo::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_bLineInfo_PlanInfoTable    = m_Options.GetOption("PlanInfoTable");
	pOpt->m_bLineInfo_PlanInfoTable_IP = m_Options.GetOption("PlanInfoTableIP");
	pOpt->m_bLineInfo_PlanGrid         = m_Options.GetOption("PlanGrid");

	// 종단면도 설정
	pOpt->m_bLineInfo_JongDanLine          = m_Options.GetOption("JongDanLine");
	pOpt->m_bLineInfo_JongDanPlanInfoTable = m_Options.GetOption("JongDanPlanInfoTable");

	// 편경사도 설정
	pOpt->m_bLineInfo_PyungubaePlanChainUnit = m_Options.GetOption("PyungubaePlanChainUnit");
	pOpt->m_bLineInfo_PyungubaePlanLeft      = m_Options.GetOption("PyungubaePlanLeft");
	pOpt->m_bLineInfo_PyungubaePlanRight     = m_Options.GetOption("PyungubaePlanRight");
	pOpt->m_bLineInfo_PyungubaePlanInfoLeft  = m_Options.GetOption("PyungubaePlanInfoLeft");
	pOpt->m_bLineInfo_PyungubaePlanInfoRight = m_Options.GetOption("PyungubaePlanInfoRight");
	pOpt->m_bLineInfo_PyungubaePlanLineInfo  = m_Options.GetOption("PyungubaePlanLineInfo");
	pOpt->m_bLineInfo_PyungubaePlanInfoTable = m_Options.GetOption("PyungubaePlanInfoTable");

	pOpt->m_dLineInfo_PlaneScale			= m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dLineInfo_JongDanHScale			= m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_dLineInfo_JongDanVScale			= m_Grid.GetTextMatrixDouble(3,1);
	pOpt->m_dLineInfo_PyungubaeScale		= m_Grid.GetTextMatrixDouble(4,1);
	pOpt->m_dLineInfo_PyungubaeRatio		= m_Grid.GetTextMatrixDouble(5,1);
	
	pStd->GetDrawPageMng()->SetModified(PAGE_LINEINFO);	
	
	CPropertyPage::OnOK();
}

void COptionLineInfo::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL COptionLineInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitData();
	InitGridData();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COptionLineInfo::OnSetActive() 
{
	CAPlateDrawStd *pStd = m_pPlateDrawStd;

	pStd->m_pDomOptionStd->m_nOptionCur = OPTION_PAGE_AROAD_LINEINFO;
	
	return CPropertyPage::OnSetActive();
}

void COptionLineInfo::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitData(TRUE);
	InitGridData(TRUE);
}

void COptionLineInfo::InitData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	// 평면선형도 설정
	m_Options.SetOptionItem("평면 선형 제원도 설정", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("PlanInfoTable", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PlanInfoTable) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PlanInfoTable", "\t평면 선형 제원 성과표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PlanInfoTableIP", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PlanInfoTable_IP) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PlanInfoTableIP", "\tIP제원 성과표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PlanGrid", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PlanGrid) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PlanGrid", "\t도각좌표 표기", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	// 종단면도 설정
	m_Options.SetOptionItem("종단 선형 제원도 설정", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("JongDanLine", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_JongDanLine) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JongDanLine", "\t종단 선형 계획선 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JongDanPlanInfoTable", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_JongDanPlanInfoTable) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JongDanPlanInfoTable", "\t종단 선형 측점별 양식 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	// 편경사도 설정
	m_Options.SetOptionItem("편경사 제원도 설정", IDB_ITEMS, IDB_ITEMS);
	
	m_Options.SetOption("PyungubaePlanChainUnit", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PyungubaePlanChainUnit) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PyungubaePlanChainUnit", "\t측점 표기", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PyungubaePlanLeft", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PyungubaePlanLeft) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PyungubaePlanLeft", "\t좌측 편경사도 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PyungubaePlanRight", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PyungubaePlanRight) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PyungubaePlanRight", "\t우측 편경사도 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PyungubaePlanInfoLeft", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PyungubaePlanInfoLeft) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PyungubaePlanInfoLeft", "\t좌측 편경사 정보 표기", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PyungubaePlanInfoRight", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PyungubaePlanInfoRight) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PyungubaePlanInfoRight", "\t우측 편경사 정보 표기", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PyungubaePlanLineInfo", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PyungubaePlanLineInfo) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PyungubaePlanLineInfo", "\t평면 선형 정보 표기", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("PyungubaePlanInfoTable", (bDefaultLoad ? TRUE : pOpt->m_bLineInfo_PyungubaePlanInfoTable) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("PyungubaePlanInfoTable", "\t평면 측점별 양식 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionLineInfo::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_dLineInfo_PlaneScale = 1200;
		pOpt->m_dLineInfo_JongDanHScale = 1200;
		pOpt->m_dLineInfo_JongDanVScale = 200;
		pOpt->m_dLineInfo_PyungubaeScale = 1200;
		pOpt->m_dLineInfo_PyungubaeRatio = 600;
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
	m_Grid.SetTextMatrix(1,0,"평면선형 제원도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"종단선형 제원도(수평)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"종단선형 제원도(수직)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"편경사 제원도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"편경사 수직비율",(UINT)DT_LEFT);	
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dLineInfo_PlaneScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dLineInfo_JongDanHScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_dLineInfo_JongDanVScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dLineInfo_PyungubaeScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.1f",pOpt->m_dLineInfo_PyungubaeRatio,(UINT)DT_RIGHT);	
	
	m_Grid.SetRedraw(TRUE,TRUE);
}
