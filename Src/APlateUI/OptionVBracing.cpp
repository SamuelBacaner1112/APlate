// OptionVBracing.cpp: implementation of the COptionVBracing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(COptionVBracing, CPropertyPage)

COptionVBracing::COptionVBracing() : CPropertyPage(COptionVBracing::IDD)
{
	//{{AFX_DATA_INIT(COptionVBracing)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionVBracing::~COptionVBracing()
{

}

void COptionVBracing::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionVBracing)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionVBracing, CPropertyPage)
	//{{AFX_MSG_MAP(COptionVBracing)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionVBracing message handlers
void COptionVBracing::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bVBracing_Summary				= bDefaultLoad ? TRUE : pOpt->m_VBracing_Summary; //
	BOOL bVBracing_Pyung				= bDefaultLoad ? TRUE : pOpt->m_VBracing_Pyung; //
	BOOL bVBracing_Table				= bDefaultLoad ? TRUE : pOpt->m_VBracing_Table; //
	BOOL bVBracing_Plan					= bDefaultLoad ? TRUE : pOpt->m_VBracing_Plan; //
	BOOL bVBracing_Front				= bDefaultLoad ? TRUE : pOpt->m_VBracing_Front; //
	BOOL bVBracing_Detail				= bDefaultLoad ? TRUE : pOpt->m_VBracing_Detail; //
	BOOL bVBracing_AnchorBarDetail		= bDefaultLoad ? TRUE : pOpt->m_VBracing_AnchorBarDetail; //
	BOOL bVBracing_BM					= bDefaultLoad ? TRUE : pOpt->m_VBracing_BM; //
	BOOL bVBracing_AllGirder			= bDefaultLoad ? FALSE : pOpt->m_VBracing_AllGirder; //
	BOOL bVBracing_KeyPlan				= bDefaultLoad ? TRUE : pOpt->m_VBracing_KeyPlan; //
	BOOL bVBracing_KeyPlanRoVer			= bDefaultLoad ? FALSE : pOpt->m_VBracing_KeyPlanRoVer; //
	BOOL bVBracing_KeyPlanSpliceMark	= bDefaultLoad ? TRUE  : pOpt->m_VBracing_KeyPlanSpliceMark;
	BOOL bVBracing_KeyPlanJijumMark		= bDefaultLoad ? TRUE  : pOpt->m_VBracing_KeyPlanJijumMark;
	BOOL bVBracing_KeyPlanCrossBeam		= bDefaultLoad ? TRUE  : pOpt->m_VBracing_KeyPlanCrossBeam;
	BOOL bVBracing_KeyPlanDimension		= bDefaultLoad ? TRUE  : pOpt->m_VBracing_KeyPlanDimension;
	BOOL bVBracing_KeyPlanDirection		= bDefaultLoad ? TRUE  : pOpt->m_VBracing_KeyPlanDirection;
	BOOL bVBracing_KeyPlanOneLine		= bDefaultLoad ? TRUE  : pOpt->m_VBracing_KeyPlanOneLine;
	BOOL bVBracing_Note					= bDefaultLoad ? FALSE : pOpt->m_VBracing_Note;

	BOOL VBracing_bJiJumMark			= bDefaultLoad ? TRUE : pOpt->m_VBracing_bJiJumMark; //
	BOOL VBracing_bDimension			= bDefaultLoad ? TRUE : pOpt->m_VBracing_bDimension; //
	BOOL VBracing_bDirection			= bDefaultLoad ? TRUE : pOpt->m_VBracing_bDirection; //
	BOOL VBracing_bOneline				= bDefaultLoad ? TRUE : pOpt->m_VBracing_bOneline; //
	BOOL VBracing_bLineInfo				= bDefaultLoad ? TRUE : pOpt->m_VBracing_bLineInfo;	//

	//
	m_Options.SetOptionItem("출력도면 구성", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("VBracing_Front", bVBracing_Front ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_Front", "\t정면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("VBracing_Plan", bVBracing_Plan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_Plan", "\t평면도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("VBracing_Detail", bVBracing_Detail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_Detail", "\t수직브레이싱 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("VBracing_AnchorBarDetail", bVBracing_AnchorBarDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_AnchorBarDetail", "\t슬래브 앵커 상세", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("VBracing_BM", bVBracing_BM ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_BM", "\t재료표", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("VBracing_Note", bVBracing_Note ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_Note", "\t기본 노트", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("출력 옵션", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("VBracing_AllGirder", bVBracing_AllGirder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_AllGirder", "\t전체 거더 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("VBracing_KeyPlan", bVBracing_KeyPlan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VBracing_KeyPlan", "\t요약도 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_SPLICE", bVBracing_KeyPlanSpliceMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_SPLICE", "\t\t현장이음 마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_JIJUMMARK", bVBracing_KeyPlanJijumMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_JIJUMMARK", "\t\t지점마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_CROSS", bVBracing_KeyPlanCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_CROSS", "\t\t가로보 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_KEYDIMENSION", bVBracing_KeyPlanDimension ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_KEYDIMENSION", "\t\t치수선 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_DIRECTION", bVBracing_KeyPlanDirection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_DIRECTION", "\t\t진행방향 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_KEYPLAN_ONELINE", bVBracing_KeyPlanOneLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_KEYPLAN_ONELINE", "\t\t부재한선으로 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_KEYPLAN_ROVER", bVBracing_KeyPlanRoVer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_KEYPLAN_ROVER", "\t\t요약도 수직 회전", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);			

		m_Options.SetOptionItem("수직브레이싱 치수표 출력", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("VBracing_Summary", bVBracing_Summary ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_Summary", "\t수직브레이싱 요약 치수도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("VBracing_Pyung", bVBracing_Pyung ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem("VBracing_Pyung", "\t\t위치도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("VBracing_Table", bVBracing_Table ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem("VBracing_Table", "\t\t치수테이블", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOptionItem("치수표 출력옵션", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("VBracing_bJiJumMark", VBracing_bJiJumMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_bJiJumMark", "\t지점마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_bDimension", VBracing_bDimension ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_bDimension", "\t치수선 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_bDirection", VBracing_bDirection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_bDirection", "\t진행방향 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_bOneline", VBracing_bOneline ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_bOneline", "\t부재 한선으로 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VBracing_bLineInfo", VBracing_bLineInfo ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VBracing_bLineInfo", "\t선형정보 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();

	m_ListOption.Initialize(NULL);
}

void COptionVBracing::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		// Add the Properties
		long i=0;
		pOpt->m_VBracing_Scale.RemoveAll();
		pOpt->m_VBracing_Scale.Add(300);
		pOpt->m_VBracing_Scale.Add(300);
		pOpt->m_VBracing_Scale.Add(25);// 정면도 
		pOpt->m_VBracing_Scale.Add(25);// 평면도
		pOpt->m_VBracing_Scale.Add(10);// 수직브레이싱 상세
		pOpt->m_VBracing_Scale.Add(10);// 앵커바 
		pOpt->m_VBracing_Scale.Add(500);// 요약도 수평 
		pOpt->m_VBracing_Scale.Add(500);// 요약도 수직
		pOpt->m_VBracing_nRowSu = 20;// 테이블 줄 수				

	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(10);
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
	m_Grid.SetTextMatrix(1,0,"수직브레이싱 치수표 평면도 수평",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"수직브레이싱 치수표 평면도 수직",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"정면도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"평면도",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"수직브레이싱 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(6,0,"슬래브 앵커 상세",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(7,0,"KEY PLAN : 수평",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(8,0,"KEY PLAN : 수직",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(9,0,"수직브레이싱 치수표 테이블당 열수",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"설정값");
	for(long i=0; i<pOpt->m_VBracing_Scale.GetSize(); i++)
		m_Grid.SetTextMatrix(i+1,1,"%.1f",pOpt->m_VBracing_Scale.GetAt(i),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(9,1,"%.0f",pOpt->m_VBracing_nRowSu,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);	
}

BOOL COptionVBracing::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData(FALSE);
	InitGridData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionVBracing::OnDestroy() 
{
	CPropertyPage::OnDestroy();			
}

void COptionVBracing::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_VBracing_Summary			= m_Options.GetOption("VBracing_Summary");
	pOpt->m_VBracing_Pyung				= m_Options.GetOption("VBracing_Pyung");
	pOpt->m_VBracing_Table				= m_Options.GetOption("VBracing_Table");
	pOpt->m_VBracing_Plan				= m_Options.GetOption("VBracing_Plan");
	pOpt->m_VBracing_Front				= m_Options.GetOption("VBracing_Front");
	pOpt->m_VBracing_Detail				= m_Options.GetOption("VBracing_Detail"); 
	pOpt->m_VBracing_AnchorBarDetail	= m_Options.GetOption("VBracing_AnchorBarDetail");
	pOpt->m_VBracing_BM					= m_Options.GetOption("VBracing_BM");	
	pOpt->m_VBracing_AllGirder			= m_Options.GetOption("VBracing_AllGirder");
	pOpt->m_VBracing_KeyPlanRoVer		= m_Options.GetOption("VBracing_KEYPLAN_ROVER");

	pOpt->m_VBracing_KeyPlanSpliceMark	= m_Options.GetOption("VBracing_SPLICE");
	pOpt->m_VBracing_KeyPlanJijumMark	= m_Options.GetOption("VBracing_JIJUMMARK");
	pOpt->m_VBracing_KeyPlanCrossBeam	= m_Options.GetOption("VBracing_CROSS");
	pOpt->m_VBracing_KeyPlanDimension	= m_Options.GetOption("VBracing_KEYDIMENSION");
	pOpt->m_VBracing_KeyPlanDirection	= m_Options.GetOption("VBracing_DIRECTION");
	pOpt->m_VBracing_KeyPlanOneLine		= m_Options.GetOption("VBracing_KEYPLAN_ONELINE");
	pOpt->m_VBracing_Note				= m_Options.GetOption("VBracing_Note");
	
	pOpt->m_VBracing_bJiJumMark			= m_Options.GetOption("VBracing_bJiJumMark");
	pOpt->m_VBracing_bDimension			= m_Options.GetOption("VBracing_bDimension");
	pOpt->m_VBracing_bDirection			= m_Options.GetOption("VBracing_bDirection");
	pOpt->m_VBracing_bOneline			= m_Options.GetOption("VBracing_bOneline");
	pOpt->m_VBracing_bLineInfo			= m_Options.GetOption("VBracing_bLineInfo");


	double Scale = 0;
	long nSize = pOpt->m_VBracing_Scale.GetSize();
	pOpt->m_VBracing_Scale.RemoveAll();
	for(long i=1; i<nSize+1; i++)
	{
		Scale = m_Grid.GetTextMatrixDouble(i,1);
		pOpt->m_VBracing_Scale.Add(Scale);
	}
	pOpt->m_VBracing_nRowSu = m_Grid.GetTextMatrixLong(9,1);
	pStd->GetDrawPageMng()->SetModified(PAGE_VSDETAIL);
	CPropertyPage::OnOK();
}

void COptionVBracing::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionVBracing::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pStd->m_pDomOptionStd->m_nOptionCur = OPTION_PAGE_AROAD_VBRACING;

	return CPropertyPage::OnSetActive();
}