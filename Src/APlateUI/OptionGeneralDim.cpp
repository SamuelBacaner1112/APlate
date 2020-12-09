// OptionGeneralDim.cpp : implementation file
//


#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionGeneralDim property page

IMPLEMENT_DYNCREATE(COptionGeneralDim, CPropertyPage)

COptionGeneralDim::COptionGeneralDim() : CPropertyPage(COptionGeneralDim::IDD)
{
	//{{AFX_DATA_INIT(COptionGeneralDim)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionGeneralDim::~COptionGeneralDim()
{
}

void COptionGeneralDim::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionGeneralDim)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_OPTION_TREE_RIGHT, m_OptionTreeRight);
	DDX_Control(pDX, IDC_OPTION_TREE_LEFT, m_OptionTreeLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionGeneralDim, CPropertyPage)
	//{{AFX_MSG_MAP(COptionGeneralDim)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
//	ON_BN_CLICKED(IDC_DEFAULT_1, OnDefault1)
//	ON_BN_CLICKED(IDC_DEFAULT_2, OnDefault2)
//	ON_BN_CLICKED(IDC_DEFAULT_3, OnDefault3)
//	ON_BN_CLICKED(IDC_DEFAULT_4, OnDefault4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionGeneralDim message handlers
void COptionGeneralDim::InitData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	m_OptionTreeLeft.m_pOptions  = &m_PyungOptions;
	m_OptionTreeRight.m_pOptions = &m_JongOptions;
	m_OptionTreeLeft.m_pOptions->m_Items.RemoveAll();
	m_OptionTreeRight.m_pOptions->m_Items.RemoveAll();

	/////////////////////////////////////////////////////////////////////
	// 평면도 상부 
	__int64 DimFlag = pOpt->m_GenDim_Plan_Upper;
	if(bDefaultLoad)
		DimFlag = DIM_BASE_JOINUPPER|DIM_BASE_SPLICE|DIM_BASE_TOTAL;
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

	if(  DimFlag & DIM_BASE_WELDSTUD)   bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF)		bVStiff = TRUE;	
	if(  DimFlag & DIM_BASE_DIAP)		bDiaphragm = TRUE; 
	if(  DimFlag & DIM_BASE_JOINUPPER)	bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER)	bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB)	bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE)		bSplice = TRUE;
	if(  DimFlag & DIM_BASE_FACTUPPER)	bFactUpper = TRUE;
	if(  DimFlag & DIM_BASE_FACTLOWER)	bFactLower = TRUE;
	if(  DimFlag & DIM_BASE_FACTWEB)	bFactWeb = TRUE;	
	if(  DimFlag & DIM_BASE_BYUN)		bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN)		bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL)		bTotal = TRUE;

	m_PyungOptions.SetOptionItem("평면도 상부", IDB_ITEMS, IDB_ITEMS);
		m_PyungOptions.SetOption("TOP_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_VSTIFF"  , "\t수직보강재" , CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_JOINUPPER", "\t상판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_JOINLOWER", "\t하판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_JOINWEB", "\t복부판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_FACTUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_FACTLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_FACTWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("TOP_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("TOP_TOTAL", "\t전체길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	/////////////////////////////////////////////////////////////////////
	// 평면도 하부 
	DimFlag = pOpt->m_GenDim_Plan_Lower;
	if(bDefaultLoad)
		DimFlag = DIM_BASE_JOINLOWER|DIM_BASE_DIAP|DIM_BASE_SPLICE;

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

	if(  DimFlag & DIM_BASE_WELDSTUD)   bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF)		bVStiff = TRUE;	
	if(  DimFlag & DIM_BASE_DIAP)		bDiaphragm = TRUE; 
	if(  DimFlag & DIM_BASE_JOINUPPER)	bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER)	bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB)	bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE)		bSplice = TRUE;
	if(  DimFlag & DIM_BASE_FACTUPPER)	bFactUpper = TRUE;
	if(  DimFlag & DIM_BASE_FACTLOWER)	bFactLower = TRUE;
	if(  DimFlag & DIM_BASE_FACTWEB)	bFactWeb = TRUE;	
	if(  DimFlag & DIM_BASE_BYUN)		bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN)		bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL)		bTotal = TRUE;

	m_PyungOptions.SetOptionItem("평면도 하부", IDB_ITEMS, IDB_ITEMS);
		m_PyungOptions.SetOption("BOT_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_JOINUPPER", "\t상판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_JOINLOWER", "\t하판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_JOINWEB", "\t복부판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_FACTUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_FACTLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_FACTWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_PyungOptions.SetOption("BOT_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_PyungOptions.SetOptionItem("BOT_TOTAL", "\t전체길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_PyungOptions.BeginTrans();
	m_OptionTreeLeft.Initialize(NULL);	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 측면도 상부 
	DimFlag = pOpt->m_GenDim_Jong_Upper;
	if(bDefaultLoad)
		DimFlag = DIM_BASE_VSTIFF|DIM_BASE_JOINWEB|DIM_BASE_TOTAL;
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

	if(  DimFlag & DIM_BASE_WELDSTUD)   bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF)		bVStiff = TRUE;	
	if(  DimFlag & DIM_BASE_DIAP)		bDiaphragm = TRUE; 
	if(  DimFlag & DIM_BASE_JOINUPPER)	bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER)	bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB)	bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE)		bSplice = TRUE;
	if(  DimFlag & DIM_BASE_FACTUPPER)	bFactUpper = TRUE;
	if(  DimFlag & DIM_BASE_FACTLOWER)	bFactLower = TRUE;
	if(  DimFlag & DIM_BASE_FACTWEB)	bFactWeb = TRUE;	
	if(  DimFlag & DIM_BASE_BYUN)		bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN)		bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL)		bTotal = TRUE;

	m_JongOptions.SetOptionItem("측면도 상부", IDB_ITEMS, IDB_ITEMS);
		m_JongOptions.SetOption("TOP_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_JOINUPPER", "\t상판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_JOINLOWER", "\t하판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_JOINWEB", "\t복부판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_FACTUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_FACTLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_FACTWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("TOP_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("TOP_TOTAL", "\t전체길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	/////////////////////////////////////////////////////////////////////
	// 측면도 하부 
	DimFlag = pOpt->m_GenDim_Jong_Lower;
	if(bDefaultLoad)
		DimFlag = DIM_BASE_BYUN|DIM_BASE_SPAN;
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

	if(  DimFlag & DIM_BASE_WELDSTUD)   bWeldStud = TRUE;
	if(  DimFlag & DIM_BASE_VSTIFF)		bVStiff = TRUE;	
	if(  DimFlag & DIM_BASE_DIAP)		bDiaphragm = TRUE; 
	if(  DimFlag & DIM_BASE_JOINUPPER)	bJoinUpper = TRUE;
	if(  DimFlag & DIM_BASE_JOINLOWER)	bJoinLower = TRUE;
	if(  DimFlag & DIM_BASE_JOINWEB)	bJoinWeb = TRUE;
	if(  DimFlag & DIM_BASE_SPLICE)		bSplice = TRUE;
	if(  DimFlag & DIM_BASE_FACTUPPER)	bFactUpper = TRUE;
	if(  DimFlag & DIM_BASE_FACTLOWER)	bFactLower = TRUE;
	if(  DimFlag & DIM_BASE_FACTWEB)	bFactWeb = TRUE;	
	if(  DimFlag & DIM_BASE_BYUN)		bByunDan = TRUE;
	if(  DimFlag & DIM_BASE_SPAN)		bSpan = TRUE;
	if(  DimFlag & DIM_BASE_TOTAL)		bTotal = TRUE;

	m_JongOptions.SetOptionItem("측면도 하부", IDB_ITEMS, IDB_ITEMS);
		m_JongOptions.SetOption("BOT_WELDSTUD", bWeldStud ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_WELDSTUD", "\t전단 연결재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_VSTIFF", "\t수직보강재", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_DIAP", bDiaphragm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_DIAP", "\t가로보", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_JOINUPPER", bJoinUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_JOINUPPER", "\t상판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_JOINLOWER", bJoinLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_JOINLOWER", "\t하판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_JOINWEB", bJoinWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_JOINWEB", "\t복부판&현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_SPLICE", "\t현장이음", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_FACTUPPER", bFactUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_FACTUPPER", "\t상판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_FACTLOWER", bFactLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_FACTLOWER", "\t하판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_FACTWEB", bFactWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_FACTWEB", "\t복부판", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_BYUN", bByunDan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_BYUN", "\t변단면 제원치수", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_SPAN", bSpan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_SPAN", "\t지간구성", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_JongOptions.SetOption("BOT_TOTAL", bTotal ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_JongOptions.SetOptionItem("BOT_TOTAL", "\t전체길이", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_JongOptions.BeginTrans();
	m_OptionTreeRight.Initialize(NULL);	
}

BOOL COptionGeneralDim::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
		
	InitData();
	return TRUE;
}

void COptionGeneralDim::OnOK() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_GenDim_Plan_Upper = 0x0000;

	if(m_PyungOptions.GetOption("TOP_WELDSTUD"))		pOpt->m_GenDim_Plan_Upper |= DIM_BASE_WELDSTUD;
	if(m_PyungOptions.GetOption("TOP_VSTIFF"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_VSTIFF;
	if(m_PyungOptions.GetOption("TOP_DIAP"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_DIAP;
	if(m_PyungOptions.GetOption("TOP_SPLICE"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_SPLICE;
	if(m_PyungOptions.GetOption("TOP_FACTUPPER"))		pOpt->m_GenDim_Plan_Upper |=DIM_BASE_FACTUPPER;
	if(m_PyungOptions.GetOption("TOP_FACTLOWER"))		pOpt->m_GenDim_Plan_Upper |=DIM_BASE_FACTLOWER;
	if(m_PyungOptions.GetOption("TOP_FACTWEB"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_FACTWEB;
	if(m_PyungOptions.GetOption("TOP_JOINUPPER"))		pOpt->m_GenDim_Plan_Upper |=DIM_BASE_JOINUPPER;
	if(m_PyungOptions.GetOption("TOP_JOINLOWER"))		pOpt->m_GenDim_Plan_Upper |=DIM_BASE_JOINLOWER;
	if(m_PyungOptions.GetOption("TOP_JOINWEB"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_JOINWEB;
	if(m_PyungOptions.GetOption("TOP_BYUN"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_BYUN;	
	if(m_PyungOptions.GetOption("TOP_SPAN"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_SPAN;
	if(m_PyungOptions.GetOption("TOP_TOTAL"))			pOpt->m_GenDim_Plan_Upper |=DIM_BASE_TOTAL;

	pOpt->m_GenDim_Plan_Lower = 0x0000;

	if(m_PyungOptions.GetOption("BOT_WELDSTUD"))		pOpt->m_GenDim_Plan_Lower |=DIM_BASE_WELDSTUD;
	if(m_PyungOptions.GetOption("BOT_VSTIFF"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_VSTIFF;
	if(m_PyungOptions.GetOption("BOT_DIAP"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_DIAP;
	if(m_PyungOptions.GetOption("BOT_SPLICE"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_SPLICE;
	if(m_PyungOptions.GetOption("BOT_FACTUPPER"))		pOpt->m_GenDim_Plan_Lower |=DIM_BASE_FACTUPPER;
	if(m_PyungOptions.GetOption("BOT_FACTLOWER"))		pOpt->m_GenDim_Plan_Lower |=DIM_BASE_FACTLOWER;
	if(m_PyungOptions.GetOption("BOT_FACTWEB"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_FACTWEB;
	if(m_PyungOptions.GetOption("BOT_JOINUPPER"))		pOpt->m_GenDim_Plan_Lower |=DIM_BASE_JOINUPPER;
	if(m_PyungOptions.GetOption("BOT_JOINLOWER"))		pOpt->m_GenDim_Plan_Lower |=DIM_BASE_JOINLOWER;
	if(m_PyungOptions.GetOption("BOT_JOINWEB"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_JOINWEB;
	if(m_PyungOptions.GetOption("BOT_BYUN"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_BYUN;	
	if(m_PyungOptions.GetOption("BOT_SPAN"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_SPAN;
	if(m_PyungOptions.GetOption("BOT_TOTAL"))			pOpt->m_GenDim_Plan_Lower |=DIM_BASE_TOTAL;

	pOpt->m_GenDim_Jong_Upper = 0x0000;

	if(m_JongOptions.GetOption("TOP_WELDSTUD"))			pOpt->m_GenDim_Jong_Upper |=DIM_BASE_WELDSTUD;
	if(m_JongOptions.GetOption("TOP_VSTIFF"))			pOpt->m_GenDim_Jong_Upper |=DIM_BASE_VSTIFF;
	if(m_JongOptions.GetOption("TOP_DIAP"))				pOpt->m_GenDim_Jong_Upper |=DIM_BASE_DIAP;
	if(m_JongOptions.GetOption("TOP_SPLICE"))			pOpt->m_GenDim_Jong_Upper |=DIM_BASE_SPLICE;
	if(m_JongOptions.GetOption("TOP_FACTUPPER"))		pOpt->m_GenDim_Jong_Upper |=DIM_BASE_FACTUPPER;
	if(m_JongOptions.GetOption("TOP_FACTLOWER"))		pOpt->m_GenDim_Jong_Upper |=DIM_BASE_FACTLOWER;
	if(m_JongOptions.GetOption("TOP_FACTWEB"))			pOpt->m_GenDim_Jong_Upper |=DIM_BASE_FACTWEB;
	if(m_JongOptions.GetOption("TOP_JOINUPPER"))		pOpt->m_GenDim_Jong_Upper |=DIM_BASE_JOINUPPER;
	if(m_JongOptions.GetOption("TOP_JOINLOWER"))		pOpt->m_GenDim_Jong_Upper |=DIM_BASE_JOINLOWER;
	if(m_JongOptions.GetOption("TOP_JOINWEB"))			pOpt->m_GenDim_Jong_Upper |=DIM_BASE_JOINWEB;
	if(m_JongOptions.GetOption("TOP_BYUN"))				pOpt->m_GenDim_Jong_Upper |=DIM_BASE_BYUN;	
	if(m_JongOptions.GetOption("TOP_SPAN"))				pOpt->m_GenDim_Jong_Upper |=DIM_BASE_SPAN;
	if(m_JongOptions.GetOption("TOP_TOTAL"))			pOpt->m_GenDim_Jong_Upper |=DIM_BASE_TOTAL;

	pOpt->m_GenDim_Jong_Lower = 0x0000;

	if(m_JongOptions.GetOption("BOT_WELDSTUD"))			pOpt->m_GenDim_Jong_Lower |=DIM_BASE_WELDSTUD;
	if(m_JongOptions.GetOption("BOT_VSTIFF"))			pOpt->m_GenDim_Jong_Lower |=DIM_BASE_VSTIFF;
	if(m_JongOptions.GetOption("BOT_DIAP"))				pOpt->m_GenDim_Jong_Lower |=DIM_BASE_DIAP;
	if(m_JongOptions.GetOption("BOT_SPLICE"))			pOpt->m_GenDim_Jong_Lower |=DIM_BASE_SPLICE;
	if(m_JongOptions.GetOption("BOT_FACTUPPER"))		pOpt->m_GenDim_Jong_Lower |=DIM_BASE_FACTUPPER;
	if(m_JongOptions.GetOption("BOT_FACTLOWER"))		pOpt->m_GenDim_Jong_Lower |=DIM_BASE_FACTLOWER;
	if(m_JongOptions.GetOption("BOT_FACTWEB"))			pOpt->m_GenDim_Jong_Lower |=DIM_BASE_FACTWEB;
	if(m_JongOptions.GetOption("BOT_JOINUPPER"))		pOpt->m_GenDim_Jong_Lower |=DIM_BASE_JOINUPPER;
	if(m_JongOptions.GetOption("BOT_JOINLOWER"))		pOpt->m_GenDim_Jong_Lower |=DIM_BASE_JOINLOWER;
	if(m_JongOptions.GetOption("BOT_JOINWEB"))			pOpt->m_GenDim_Jong_Lower |=DIM_BASE_JOINWEB;
	if(m_JongOptions.GetOption("BOT_BYUN"))				pOpt->m_GenDim_Jong_Lower |=DIM_BASE_BYUN;	
	if(m_JongOptions.GetOption("BOT_SPAN"))				pOpt->m_GenDim_Jong_Lower |=DIM_BASE_SPAN;
	if(m_JongOptions.GetOption("BOT_TOTAL"))			pOpt->m_GenDim_Jong_Lower |=DIM_BASE_TOTAL;

	pStd->GetDrawPageMng()->SetModified(PAGE_GENERAL);	

	CPropertyPage::OnOK();
}

// OptionStd의 초기값과 일치시켜야함....
void COptionGeneralDim::OnDefault()
{
	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData(TRUE);
}


BOOL COptionGeneralDim::OnSetActive() 
{
	CAPlateDrawStd *pStd = m_pPlateDrawStd;

	pStd->m_pDomOptionStd->m_nOptionCur = OPTION_PAGE_AROAD_GENERALDIM;
	
	return CPropertyPage::OnSetActive();
}

// 직선직교 권고안
void COptionGeneralDim::OnDefault1() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_GenDim_Plan_Upper = DIM_BASE_JOINUPPER|DIM_BASE_SPLICE|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Plan_Lower = DIM_BASE_JOINLOWER|DIM_BASE_DIAP|DIM_BASE_SPLICE;
	pOpt->m_GenDim_Jong_Upper = DIM_BASE_VSTIFF|DIM_BASE_JOINWEB|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Jong_Lower = DIM_BASE_BYUN|DIM_BASE_SPAN;

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();

	InitData();
}

// 곡선직교 권고안
void COptionGeneralDim::OnDefault2() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_GenDim_Plan_Upper = DIM_BASE_JOINUPPER|DIM_BASE_SPLICE|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Plan_Lower = DIM_BASE_JOINLOWER|DIM_BASE_DIAP|DIM_BASE_SPLICE;
	pOpt->m_GenDim_Jong_Upper = DIM_BASE_VSTIFF|DIM_BASE_JOINWEB|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Jong_Lower = DIM_BASE_BYUN|DIM_BASE_SPAN;

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData();
}

// 직선사교 권고안
void COptionGeneralDim::OnDefault3() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_GenDim_Plan_Upper = DIM_BASE_JOINUPPER|DIM_BASE_SPLICE|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Plan_Lower = DIM_BASE_JOINLOWER|DIM_BASE_DIAP|DIM_BASE_SPLICE;
	pOpt->m_GenDim_Jong_Upper = DIM_BASE_VSTIFF|DIM_BASE_JOINWEB|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Jong_Lower = DIM_BASE_BYUN|DIM_BASE_SPAN;

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData();
}

// 곡선사교 권고안
void COptionGeneralDim::OnDefault4() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;

	pOpt->m_GenDim_Plan_Upper = DIM_BASE_JOINUPPER|DIM_BASE_SPLICE|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Plan_Lower = DIM_BASE_JOINLOWER|DIM_BASE_DIAP|DIM_BASE_SPLICE;
	pOpt->m_GenDim_Jong_Upper = DIM_BASE_VSTIFF|DIM_BASE_JOINWEB|DIM_BASE_TOTAL;
	pOpt->m_GenDim_Jong_Lower = DIM_BASE_BYUN|DIM_BASE_SPAN;

	m_OptionTreeLeft.DeleteAllItems();
	m_OptionTreeRight.DeleteAllItems();
	InitData();
}
