// OptionWeldMap.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

/////////////////////////////////////////////////////////////////////////////
// COptionWeldMap property page

IMPLEMENT_DYNCREATE(COptionWeldMap, CPropertyPage)

COptionWeldMap::COptionWeldMap() : CPropertyPage(COptionWeldMap::IDD)
{
	//{{AFX_DATA_INIT(COptionWeldMap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionWeldMap::~COptionWeldMap()
{ 
}

void COptionWeldMap::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionWeldMap)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionWeldMap, CPropertyPage)
	//{{AFX_MSG_MAP(COptionWeldMap)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionWeldMap message handlers

void COptionWeldMap::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	pOpt->m_bWeldLeftGirder		= m_Options.GetOption("m_bWeldLeftGirder");
	pOpt->m_bWeldCamberLevel		= m_Options.GetOption("m_bWeldCamberLevel");
	pOpt->m_bWeldDivDomyun		= m_Options.GetOption("m_bWeldDivDomyun");
	pOpt->m_bWeldKeyPlan		= m_Options.GetOption("KEYPLAN");
	pOpt->m_bWeldKeyPlanSpliceMark = m_Options.GetOption("SPLICE");
	pOpt->m_bWeldKeyPlanJijumMark = m_Options.GetOption("JIJUMMARK");
	pOpt->m_bWeldKeyPlanCrossBeam = m_Options.GetOption("CROSS");
	pOpt->m_bWeldKeyPlanDimension = m_Options.GetOption("KEYDIMENSION");
	pOpt->m_bWeldKeyPlanDirection = m_Options.GetOption("DIRECTION");
	pOpt->m_bWeldKeyPlanOneLine = m_Options.GetOption("KEYPLAN_ONELINE");
	pOpt->m_bWeldKeyPlanRoVer = m_Options.GetOption("KEYPLAN_ROVER");	

	pOpt->m_bWeldTypicalSection	= m_Options.GetOption("m_bWeldTypicalSection");
//	pOpt->m_bWeldNote			= m_Options.GetOption("m_bWeldNote");
	pOpt->m_bWeldMarkSystem		= m_Options.GetOption("m_bWeldMarkSystem");
	pOpt->m_bWeldLineType		= m_Options.GetOption("m_bWeldLineType");
	pOpt->m_bWeldDimText		= m_Options.GetOption("m_bWeldDimText");
	pOpt->m_nWeldMark			= m_Options.GetOption("m_nWeldMark");

	pOpt->m_nWeldDanmyunScale = m_Grid.GetTextMatrixLong(1,1);
//	pOpt->m_nWeldSangseScale = m_Grid.GetTextMatrixLong(2,1);
	pOpt->m_nWeldTypicalScale = m_Grid.GetTextMatrixLong(2,1);
	pOpt->m_nWeldKeyPlanHor = m_Grid.GetTextMatrixLong(3,1);
	pOpt->m_nWeldKeyPlanVer = m_Grid.GetTextMatrixLong(4,1);
	pOpt->m_nWeldDivBlock = m_Grid.GetTextMatrixLong(5,1);
	pOpt->m_nWeldDivDomyun = m_Grid.GetTextMatrixLong(6,1);
	
	pStd->GetDrawPageMng()->SetModified(PAGE_WELDMAP);
//	pStd->GetDrawPageMng()->m_bPageInit = FALSE;

	CPropertyPage::OnOK();
}

BOOL COptionWeldMap::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitData();
	InitGridData();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionWeldMap::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}

void COptionWeldMap::InitData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();
	
	m_Options.SetOptionItem("출력 옵션", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("m_bWeldLeftGirder", (bDefaultLoad ? TRUE : pOpt->m_bWeldLeftGirder));
	m_Options.SetOptionItem("m_bWeldLeftGirder", "\t중앙 거더 출력", 1, IDB_RADIOON, IDB_RADIOOFF);	
	m_Options.SetOptionItem("m_bWeldLeftGirder", "\t전체 거더 출력",0, IDB_RADIOON, IDB_RADIOOFF);		
	m_Options.SetOption("m_bWeldCamberLevel", (bDefaultLoad ? TRUE : pOpt->m_bWeldCamberLevel) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);		
	m_Options.SetOptionItem("m_bWeldCamberLevel", "\t측면도 레벨 적용", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
	m_Options.SetOptionItem("도면분할 방식", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("m_bWeldDivDomyun", (bDefaultLoad ? TRUE : pOpt->m_bWeldDivDomyun));
	m_Options.SetOptionItem("m_bWeldDivDomyun", "\t현장이음 개수", 1, IDB_RADIOON, IDB_RADIOOFF);	
	m_Options.SetOptionItem("m_bWeldDivDomyun", "\t분할하지 않음",0, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOptionItem("상세 출력", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("KEYPLAN", (bDefaultLoad ? TRUE : pOpt->m_bWeldKeyPlan) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("KEYPLAN", "\t요약도", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("SPLICE", (bDefaultLoad ? TRUE : pOpt->m_bWeldKeyPlanSpliceMark) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SPLICE", "\t\t현장이음 마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("JIJUMMARK", (bDefaultLoad ? TRUE : pOpt->m_bWeldKeyPlanJijumMark) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JIJUMMARK", "\t\t지점마크 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("CROSS", (bDefaultLoad ? TRUE : pOpt->m_bWeldKeyPlanCrossBeam) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("CROSS", "\t\t가로보 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYDIMENSION", (bDefaultLoad ? TRUE : pOpt->m_bWeldKeyPlanDimension) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYDIMENSION", "\t\t치수선 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("DIRECTION", (bDefaultLoad ? TRUE : pOpt->m_bWeldKeyPlanDirection) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("DIRECTION", "\t\t진행방향 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ONELINE", (bDefaultLoad ? TRUE : pOpt->m_bWeldKeyPlanOneLine) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ONELINE", "\t\t부재한선으로 표현", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ROVER", (bDefaultLoad ? FALSE : pOpt->m_bWeldKeyPlanRoVer) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ROVER", "\t\t요약도 수직 회전", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("m_bWeldTypicalSection", (bDefaultLoad ? TRUE : pOpt->m_bWeldTypicalSection) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("m_bWeldTypicalSection", "\t단면도 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
//	m_Options.SetOption("m_bWeldNote", (bDefaultLoad ? TRUE : pOpt->m_bWeldNote) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//	m_Options.SetOptionItem("m_bWeldNote", "\tNOTE 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("m_bWeldMarkSystem", (bDefaultLoad ? TRUE : pOpt->m_bWeldMarkSystem) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("m_bWeldMarkSystem", "\tMarkSystem 출력", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem("수직,수평보강재 출력 옵션", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("m_bWeldLineType", (bDefaultLoad ? TRUE : pOpt->m_bWeldLineType));
	m_Options.SetOptionItem("m_bWeldLineType", "\t단선으로 출력", 1, IDB_RADIOON, IDB_RADIOOFF);	
	m_Options.SetOptionItem("m_bWeldLineType", "\t2점 쇄선으로 출력",0, IDB_RADIOON, IDB_RADIOOFF);		

	m_Options.SetOptionItem("치수 문자 출력 옵션", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("m_bWeldDimText", (bDefaultLoad ? TRUE : pOpt->m_bWeldDimText));
	m_Options.SetOptionItem("m_bWeldDimText", "\t두께 출력", 1, IDB_RADIOON, IDB_RADIOOFF);	
	m_Options.SetOptionItem("m_bWeldDimText", "\t길이와 두께 출력",0, IDB_RADIOON, IDB_RADIOOFF);		

	m_Options.SetOptionItem("용접 마크 출력 옵션", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("m_nWeldMark", (bDefaultLoad ? 0 : pOpt->m_nWeldMark));
	m_Options.SetOptionItem("m_nWeldMark", "\t마크 출력", 0, IDB_RADIOON, IDB_RADIOOFF);	
	m_Options.SetOptionItem("m_nWeldMark", "\t기호 출력",1, IDB_RADIOON, IDB_RADIOOFF);
//	m_Options.SetOptionItem("m_nWeldMark", "\t그림과 기호 출력",2, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionWeldMap::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{	
		pOpt->m_nWeldDanmyunScale = 100;
		pOpt->m_nWeldSangseScale = 3;
		pOpt->m_nWeldTypicalScale = 20;
		pOpt->m_nWeldKeyPlanHor = 500;
		pOpt->m_nWeldKeyPlanVer = 500;
		pOpt->m_nWeldDivBlock = 3;
		pOpt->m_nWeldDivDomyun = 4;
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(7);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);				
	m_Grid.SetColumnWidth(0,(long)(Rect.Width()*2.0/3.0));
	m_Grid.SetColumnWidth(1,(long)(Rect.Width()*1.0/3.0));
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));	
	m_Grid.SetTextBkColor(RGB(255,255,255));

	m_Grid.SetTextMatrix(0,0,"설정항목");	
	m_Grid.SetTextMatrix(1,0,"종단 상세도",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(2,0,"용접 상세도",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"TYPICAL SECTION",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(3,0,"KEY PLAN : 수평",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"KEY PLAN : 수직",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"페이지당 거더라인 개수",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(6,0,"출력단위 현장이음 개수",(UINT)DT_LEFT);

	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrixFormat(1,1,"%d",pOpt->m_nWeldDanmyunScale,(UINT)DT_RIGHT);	
//	m_Grid.SetTextMatrixFormat(2,1,"%d",pOpt->m_nWeldSangseScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrixFormat(2,1,"%d",pOpt->m_nWeldTypicalScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrixFormat(3,1,"%d",pOpt->m_nWeldKeyPlanHor,(UINT)DT_RIGHT);		
	m_Grid.SetTextMatrixFormat(4,1,"%d",pOpt->m_nWeldKeyPlanVer,(UINT)DT_RIGHT);		
	m_Grid.SetTextMatrixFormat(5,1,"%d",pOpt->m_nWeldDivBlock,(UINT)DT_RIGHT);		
	m_Grid.SetTextMatrixFormat(6,1,"%d",pOpt->m_nWeldDivDomyun,(UINT)DT_RIGHT);			

	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionWeldMap::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionWeldMap::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_WELDMAP;
	
	return CPropertyPage::OnSetActive();
}
