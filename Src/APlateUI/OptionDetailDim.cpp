// OptionDetailDim.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDetailDim property page

IMPLEMENT_DYNCREATE(COptionDetailDim, CPropertyPage)

COptionDetailDim::COptionDetailDim() : CPropertyPage(COptionDetailDim::IDD)
{
	//{{AFX_DATA_INIT(COptionDetailDim)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionDetailDim::~COptionDetailDim()
{
}

void COptionDetailDim::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDetailDim)
	DDX_Control(pDX, IDC_OPTION_TREE_RIGHT, m_OptionTreeRight);
	DDX_Control(pDX, IDC_OPTION_TREE_LEFT, m_OptionTreeLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDetailDim, CPropertyPage)
	//{{AFX_MSG_MAP(COptionDetailDim)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDetailDim message handlers
void COptionDetailDim::InitData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	m_OptionTreeLeft.m_pOptions  = &m_PyungOptions;
	m_OptionTreeRight.m_pOptions = &m_JongOptions;
	m_OptionTreeLeft.m_pOptions->m_Items.RemoveAll();
	m_OptionTreeRight.m_pOptions->m_Items.RemoveAll();

	/////////////////////////////////////////////////////////////////////
	// 평면도 상부 
	__int64 DimFlag = pOpt->m_DetailDim_Plan_Upper;
	BOOL bWeldStud	= FALSE;
	BOOL bVStiff	= FALSE;
	BOOL bDiaphragm	= FALSE;
	BOOL bJoinUpper	= FALSE;
	BOOL bJoinLower	= FALSE;
	BOOL bJoinWeb	= FALSE;
	BOOL bSplice	= FALSE;
	BOOL bFactUpper	= FALSE;
	BOOL bFactLower	= FALSE;
	BOOL bFactWeb	= FALSE;
	BOOL bByunDan	= FALSE;
	BOOL bSpan		= FALSE;
	BOOL bTotal		= FALSE;
	if(  DimFlag & DIM_BASE_WELDSTUD) bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF)	  bVStiff = TRUE;	
	if(  DimFlag & DIM_BASE_DIAP) bDiaphragm = TRUE;
	if(  DimFlag & DIM_BASE_JOINUPPER) bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER) bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB) bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE) bSplice = TRUE; //
	if(  DimFlag & DIM_BASE_FACTUPPER) bFactUpper = TRUE; //
	if(  DimFlag & DIM_BASE_FACTLOWER) bFactLower = TRUE;
	if(  DimFlag & DIM_BASE_FACTWEB) bFactWeb = TRUE;
	if(  DimFlag & DIM_BASE_BYUN) bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN) bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL) bTotal = TRUE; //

	if(bDefaultLoad)
	{
		bWeldStud	= FALSE;
		bVStiff		= FALSE;
		bDiaphragm	= FALSE;
		bJoinUpper	= FALSE;
		bJoinLower	= FALSE;
		bJoinWeb	= FALSE;
		bSplice		= TRUE;
		bFactUpper	= TRUE;
		bFactLower	= FALSE;
		bFactWeb	= FALSE;
		bByunDan	= TRUE;
		bSpan		= FALSE;
		bTotal		= TRUE;
	}
	m_PyungOptions.SetOptionItem("평면도 상부", IDB_ITEMS, IDB_ITEMS);
		m_PyungOptions.SetOption("TOP_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_JOINUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_JOINLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_JOINWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_FACTUPPER", "\t상부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_FACTLOWER", "\t하부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_FACTWEB", "\t복부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_TOTAL", "\t총길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	/////////////////////////////////////////////////////////////////////
	// 평면도 하부 
	DimFlag = pOpt->m_DetailDim_Plan_Lower;
	bWeldStud	= FALSE;
	bVStiff	= FALSE;
	bDiaphragm	= FALSE;
	bJoinUpper	= FALSE;
	bJoinLower	= FALSE;
	bJoinWeb	= FALSE;
	bSplice	= FALSE;
	bFactUpper	= FALSE;
	bFactLower	= FALSE;
	bFactWeb	= FALSE;
	bByunDan	= FALSE;
	bSpan		= FALSE;
	bTotal		= FALSE;
	if(  DimFlag & DIM_BASE_WELDSTUD) bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF) bVStiff = TRUE;
	if(  DimFlag & DIM_BASE_DIAP) bDiaphragm = TRUE; //
	if(  DimFlag & DIM_BASE_JOINUPPER) bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER) bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB) bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE) bSplice = TRUE; 
	if(  DimFlag & DIM_BASE_FACTUPPER) bFactUpper = TRUE;
	if(  DimFlag & DIM_BASE_FACTLOWER) bFactLower = TRUE; //
	if(  DimFlag & DIM_BASE_FACTWEB) bFactWeb = TRUE;
	if(  DimFlag & DIM_BASE_BYUN) bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN) bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL) bTotal = TRUE; //

	if(bDefaultLoad)
	{
		bWeldStud	= FALSE;
		bVStiff		= FALSE;
		bDiaphragm	= FALSE;
		bJoinUpper	= FALSE;
		bJoinLower	= FALSE;
		bJoinWeb	= FALSE;
		bSplice		= FALSE;
		bFactUpper	= FALSE;
		bFactLower	= FALSE;
		bFactWeb	= TRUE;
		bByunDan	= FALSE;
		bSpan		= FALSE;
		bTotal		= TRUE;
	}

	m_PyungOptions.SetOptionItem("평면도 하부", IDB_ITEMS, IDB_ITEMS);
		m_PyungOptions.SetOption("BOT_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_JOINUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_JOINLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_JOINWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_FACTUPPER", "\t상부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_FACTLOWER", "\t하부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_FACTWEB", "\t복부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_TOTAL", "\t총길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_PyungOptions.BeginTrans();
	m_OptionTreeLeft.Initialize(NULL);	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 측면도 상부 
	DimFlag = pOpt->m_DetailDim_Jong_Upper;
	bWeldStud	= FALSE;
	bVStiff	= FALSE;
	bDiaphragm	= FALSE;
	bJoinUpper	= FALSE;
	bJoinLower	= FALSE;
	bJoinWeb	= FALSE;
	bSplice	= FALSE;
	bFactUpper	= FALSE;
	bFactLower	= FALSE;
	bFactWeb	= FALSE;
	bByunDan	= FALSE;
	bSpan		= FALSE;
	bTotal		= FALSE;

	if(  DimFlag & DIM_BASE_WELDSTUD) bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF) bVStiff = TRUE;
	if(  DimFlag & DIM_BASE_DIAP) bDiaphragm = TRUE; //
	if(  DimFlag & DIM_BASE_JOINUPPER) bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER) bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB) bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE) bSplice = TRUE; 
	if(  DimFlag & DIM_BASE_FACTUPPER) bFactUpper = TRUE;
	if(  DimFlag & DIM_BASE_FACTLOWER) bFactLower = TRUE; //
	if(  DimFlag & DIM_BASE_FACTWEB) bFactWeb = TRUE;
	if(  DimFlag & DIM_BASE_BYUN) bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN) bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL) bTotal = TRUE; //

	if(bDefaultLoad)
	{
		bWeldStud	= TRUE;
		bVStiff		= TRUE;
		bDiaphragm	= FALSE;
		bJoinUpper	= FALSE;
		bJoinLower	= FALSE;
		bJoinWeb	= FALSE;
		bSplice		= FALSE;
		bFactUpper	= FALSE;
		bFactLower	= FALSE;
		bFactWeb	= TRUE;
		bByunDan	= FALSE;
		bSpan		= FALSE;
		bTotal		= TRUE;
	}

	m_JongOptions.SetOptionItem("측면도 상부", IDB_ITEMS, IDB_ITEMS);
		m_JongOptions.SetOption("TOP_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_JOINUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_JOINLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_JOINWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_FACTUPPER", "\t상부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_FACTLOWER", "\t하부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_FACTWEB", "\t복부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_TOTAL", "\t총길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	/////////////////////////////////////////////////////////////////////
	// 측면도 하부 
	DimFlag = pOpt->m_DetailDim_Jong_Lower;
	bWeldStud	= FALSE;
	bVStiff		= FALSE;
	bDiaphragm	= FALSE;
	bJoinUpper	= FALSE;
	bJoinLower	= FALSE;
	bJoinWeb	= FALSE;
	bSplice		= FALSE;
	bFactUpper	= FALSE;
	bFactLower	= FALSE;
	bFactWeb	= FALSE;
	bByunDan	= FALSE;
	bSpan		= FALSE;
	bTotal		= FALSE;

	if(  DimFlag & DIM_BASE_WELDSTUD) bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF) bVStiff = TRUE;
	if(  DimFlag & DIM_BASE_DIAP) bDiaphragm = TRUE; //
	if(  DimFlag & DIM_BASE_JOINUPPER) bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER) bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB) bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE) bSplice = TRUE; 
	if(  DimFlag & DIM_BASE_FACTUPPER) bFactUpper = TRUE;
	if(  DimFlag & DIM_BASE_FACTLOWER) bFactLower = TRUE; //
	if(  DimFlag & DIM_BASE_FACTWEB) bFactWeb = TRUE;
	if(  DimFlag & DIM_BASE_BYUN) bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN) bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL) bTotal = TRUE; //

	if(bDefaultLoad)
	{
		bWeldStud	= FALSE;
		bVStiff		= FALSE;
		bDiaphragm	= FALSE;
		bJoinUpper	= FALSE;
		bJoinLower	= FALSE;
		bJoinWeb	= FALSE;
		bSplice		= FALSE;
		bFactUpper	= FALSE;
		bFactLower	= FALSE;
		bFactWeb	= FALSE;
		bByunDan	= FALSE;
		bSpan		= FALSE;
		bTotal		= FALSE;
	}

	m_JongOptions.SetOptionItem("측면도 하부", IDB_ITEMS, IDB_ITEMS);
		m_JongOptions.SetOption("BOT_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_JOINUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_JOINLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_JOINWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_FACTUPPER", "\t상부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_FACTLOWER", "\t하부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_FACTWEB", "\t복부공장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_TOTAL", "\t총길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_JongOptions.BeginTrans();
	m_OptionTreeRight.Initialize(NULL);	
}

BOOL COptionDetailDim::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
		
	InitData(FALSE);
	return TRUE;
}

void COptionDetailDim::OnOK() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_DetailDim_Plan_Upper = 0x0000;
	
	if(m_PyungOptions.GetOption("TOP_WELDSTUD"))		pOpt->m_DetailDim_Plan_Upper |= DIM_BASE_WELDSTUD;
	if(m_PyungOptions.GetOption("TOP_VSTIFF"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_VSTIFF;
	if(m_PyungOptions.GetOption("TOP_DIAP"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_DIAP;
	if(m_PyungOptions.GetOption("TOP_SPLICE"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_SPLICE;
	if(m_PyungOptions.GetOption("TOP_FACTUPPER"))		pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_FACTUPPER;
	if(m_PyungOptions.GetOption("TOP_FACTLOWER"))		pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_FACTLOWER;
	if(m_PyungOptions.GetOption("TOP_FACTWEB"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_FACTWEB;
	if(m_PyungOptions.GetOption("TOP_JOINUPPER"))		pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_JOINUPPER;
	if(m_PyungOptions.GetOption("TOP_JOINLOWER"))		pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_JOINLOWER;
	if(m_PyungOptions.GetOption("TOP_JOINWEB"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_JOINWEB;
	if(m_PyungOptions.GetOption("TOP_BYUN"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_BYUN;
	if(m_PyungOptions.GetOption("TOP_SPAN"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_SPAN;
	if(m_PyungOptions.GetOption("TOP_TOTAL"))			pOpt->m_DetailDim_Plan_Upper |=DIM_BASE_TOTAL;

	pOpt->m_DetailDim_Plan_Lower = 0x0000;

	if(m_PyungOptions.GetOption("BOT_WELDSTUD"))		pOpt->m_DetailDim_Plan_Lower |= DIM_BASE_WELDSTUD;
	if(m_PyungOptions.GetOption("BOT_VSTIFF"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_VSTIFF;
	if(m_PyungOptions.GetOption("BOT_DIAP"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_DIAP;
	if(m_PyungOptions.GetOption("BOT_SPLICE"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_SPLICE;
	if(m_PyungOptions.GetOption("BOT_FACTUPPER"))		pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_FACTUPPER;
	if(m_PyungOptions.GetOption("BOT_FACTLOWER"))		pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_FACTLOWER;
	if(m_PyungOptions.GetOption("BOT_FACTWEB"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_FACTWEB;
	if(m_PyungOptions.GetOption("BOT_JOINUPPER"))		pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_JOINUPPER;
	if(m_PyungOptions.GetOption("BOT_JOINLOWER"))		pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_JOINLOWER;
	if(m_PyungOptions.GetOption("BOT_JOINWEB"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_JOINWEB;
	if(m_PyungOptions.GetOption("BOT_BYUN"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_BYUN;
	if(m_PyungOptions.GetOption("BOT_SPAN"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_SPAN;
	if(m_PyungOptions.GetOption("BOT_TOTAL"))			pOpt->m_DetailDim_Plan_Lower |=DIM_BASE_TOTAL;

	pOpt->m_DetailDim_Jong_Upper = 0x0000;
	
	if(m_JongOptions.GetOption("TOP_WELDSTUD"))			pOpt->m_DetailDim_Jong_Upper |= DIM_BASE_WELDSTUD;
	if(m_JongOptions.GetOption("TOP_VSTIFF"))			pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_VSTIFF;
	if(m_JongOptions.GetOption("TOP_DIAP"))				pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_DIAP;
	if(m_JongOptions.GetOption("TOP_SPLICE"))			pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_SPLICE;
	if(m_JongOptions.GetOption("TOP_FACTUPPER"))		pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_FACTUPPER;
	if(m_JongOptions.GetOption("TOP_FACTLOWER"))		pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_FACTLOWER;
	if(m_JongOptions.GetOption("TOP_FACTWEB"))			pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_FACTWEB;
	if(m_JongOptions.GetOption("TOP_JOINUPPER"))		pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_JOINUPPER;
	if(m_JongOptions.GetOption("TOP_JOINLOWER"))		pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_JOINLOWER;
	if(m_JongOptions.GetOption("TOP_JOINWEB"))			pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_JOINWEB;
	if(m_JongOptions.GetOption("TOP_BYUN"))				pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_BYUN;
	if(m_JongOptions.GetOption("TOP_SPAN"))				pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_SPAN;
	if(m_JongOptions.GetOption("TOP_TOTAL"))			pOpt->m_DetailDim_Jong_Upper |=DIM_BASE_TOTAL;

	pOpt->m_DetailDim_Jong_Lower = 0x0000;

	if(m_JongOptions.GetOption("BOT_WELDSTUD"))			pOpt->m_DetailDim_Jong_Lower |= DIM_BASE_WELDSTUD;
	if(m_JongOptions.GetOption("BOT_VSTIFF"))			pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_VSTIFF;
	if(m_JongOptions.GetOption("BOT_DIAP"))				pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_DIAP;
	if(m_JongOptions.GetOption("BOT_SPLICE"))			pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_SPLICE;
	if(m_JongOptions.GetOption("BOT_FACTUPPER"))		pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_FACTUPPER;
	if(m_JongOptions.GetOption("BOT_FACTLOWER"))		pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_FACTLOWER;
	if(m_JongOptions.GetOption("BOT_FACTWEB"))			pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_FACTWEB;
	if(m_JongOptions.GetOption("BOT_JOINUPPER"))		pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_JOINUPPER;
	if(m_JongOptions.GetOption("BOT_JOINLOWER"))		pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_JOINLOWER;
	if(m_JongOptions.GetOption("BOT_JOINWEB"))			pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_JOINWEB;
	if(m_JongOptions.GetOption("BOT_BYUN"))				pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_BYUN;
	if(m_JongOptions.GetOption("BOT_SPAN"))				pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_SPAN;
	if(m_JongOptions.GetOption("BOT_TOTAL"))			pOpt->m_DetailDim_Jong_Lower |=DIM_BASE_TOTAL;

	pStd->GetDrawPageMng()->SetModified(PAGE_DETAIL);	

	CPropertyPage::OnOK();
}

void COptionDetailDim::OnDefault()
{
	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData(TRUE);
}

BOOL COptionDetailDim::OnSetActive() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_DETAILDIM;
	
	return CPropertyPage::OnSetActive();
}

void COptionDetailDim::OnDefault1() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_DetailDim_Plan_Upper = DIM_BASE_VSTIFF    |DIM_BASE_FACTUPPER | DIM_BASE_SPLICE | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Plan_Lower = DIM_BASE_DIAP      | DIM_BASE_FACTLOWER;
	pOpt->m_DetailDim_Jong_Upper = DIM_BASE_WELDSTUD  | DIM_BASE_VSTIFF | DIM_BASE_FACTWEB | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Jong_Lower = DIM_BASE_BYUN; 

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData();
}

void COptionDetailDim::OnDefault2() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_DetailDim_Plan_Upper = DIM_BASE_VSTIFF  | DIM_BASE_FACTUPPER | DIM_BASE_SPLICE | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Plan_Lower = DIM_BASE_VSTIFF | DIM_BASE_DIAP | DIM_BASE_FACTLOWER | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Jong_Upper = DIM_BASE_WELDSTUD  | DIM_BASE_VSTIFF | DIM_BASE_FACTWEB | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Jong_Lower = DIM_BASE_BYUN; 

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData();
}

void COptionDetailDim::OnDefault3() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_DetailDim_Plan_Upper = DIM_BASE_VSTIFF    |DIM_BASE_FACTUPPER | DIM_BASE_SPLICE | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Plan_Lower = DIM_BASE_DIAP      | DIM_BASE_FACTLOWER;
	pOpt->m_DetailDim_Jong_Upper = DIM_BASE_WELDSTUD  | DIM_BASE_VSTIFF | DIM_BASE_FACTWEB | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Jong_Lower = DIM_BASE_BYUN; 

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData();
}

void COptionDetailDim::OnDefault4() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;

	pOpt->m_DetailDim_Plan_Upper = DIM_BASE_VSTIFF  | DIM_BASE_FACTUPPER | DIM_BASE_SPLICE | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Plan_Lower = DIM_BASE_VSTIFF | DIM_BASE_DIAP | DIM_BASE_FACTLOWER | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Jong_Upper = DIM_BASE_WELDSTUD  | DIM_BASE_VSTIFF | DIM_BASE_FACTWEB | DIM_BASE_TOTAL;
	pOpt->m_DetailDim_Jong_Lower = DIM_BASE_BYUN; 

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData();
}