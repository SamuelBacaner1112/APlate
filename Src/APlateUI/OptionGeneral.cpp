// OptionGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionGeneral property page

IMPLEMENT_DYNCREATE(COptionGeneral, CPropertyPage)

COptionGeneral::COptionGeneral() : CPropertyPage(COptionGeneral::IDD)
{
	//{{AFX_DATA_INIT(COptionGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionGeneral::~COptionGeneral()
{
}

void COptionGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionGeneral)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);	
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(COptionGeneral)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionGeneral message handlers

BOOL COptionGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	InitTreeData(FALSE);
	InitGridData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionGeneral::InitTreeData(BOOL bDefaultLoad)
{		
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bDivPlanJong		= bDefaultLoad ? FALSE : pOpt->m_Gen_bDivPlanJong;
	BOOL bBujeThick			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bBujeThick;
	BOOL bPlanView			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bPlanView;
	BOOL bJongView			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJongView;
	BOOL bFrontView			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bFrontView;
	BOOL bKeyPlan			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlan;
	BOOL bLegend			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bLegend;
	BOOL bBasicNote			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bBasicNote;
	BOOL bForceText			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bForceText;
	BOOL bForceTextLow		= bDefaultLoad ? TRUE  : pOpt->m_Gen_bForceTextLow;
	BOOL bMatchLine			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bMatchLine;	
	BOOL bJongCenterGir		= bDefaultLoad ? FALSE : pOpt->m_Gen_bJongCenterGir;
	BOOL bJongGenGir		= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJongGenGir;
	BOOL bKeyPlanSpliceMark	= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlanSpliceMark;
	BOOL bKeyPlanJijumMark	= bDefaultLoad ? TRUE : pOpt->m_Gen_bKeyPlanJijumMark;
	BOOL bKeyPlanCrossBeam	= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlanCrossBeam;
	BOOL bKeyPlanDimension	= bDefaultLoad ? TRUE : pOpt->m_Gen_bKeyPlanDimension;
	BOOL bKeyPlanOneLine	= bDefaultLoad ? TRUE : pOpt->m_Gen_bKeyPlanOneLine;
	BOOL bKeyPlanRoVer		= bDefaultLoad ? FALSE : pOpt->m_Gen_bKeyPlanRoVer;	
	BOOL bKeyPlanDirection	= bDefaultLoad ? TRUE  : pOpt->m_Gen_bKeyPlanDirection;
	BOOL bJongLevel			= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJongLevel;	
	BOOL bTableVertical		= bDefaultLoad ? FALSE : pOpt->m_Gen_bVerticalNote;
	BOOL bJijumSymbol		= bDefaultLoad ? TRUE  : pOpt->m_Gen_bJijumSymbol;
	BOOL bJijumSimple	    = bDefaultLoad ? TRUE  : pOpt->m_Gen_bJijumSimple;		
	BOOL bMarkCross			= bDefaultLoad ? FALSE : pOpt->m_Gen_bMarkCorss;
	BOOL bMarkSringer		= bDefaultLoad ? FALSE : pOpt->m_Gen_bMarkStringer;
	BOOL bTextStationPyung  = bDefaultLoad ?  TRUE : pOpt->m_Gen_bPyungJijumStation;
	BOOL bTextStationJong   = bDefaultLoad ?  TRUE : pOpt->m_Gen_bJongJijumStation;
	BOOL bPyungLineInfo		= bDefaultLoad ?  TRUE : pOpt->m_Gen_bPyungLineInfo;
	BOOL bByundan			= bDefaultLoad ?  TRUE : pOpt->m_Gen_bByundan;
	long nOutGirder			= bDefaultLoad ? 0	   : pOpt->m_Gen_nOutGirder;
	long nStyleDanmyunType	= bDefaultLoad ? 0	   : pOpt->m_Gen_nStyleDanmyunType;
	long nPyungDim			= bDefaultLoad ? 1	   : pOpt->m_Gen_nPyungDim;
	long nDivPage			= bDefaultLoad ? 0	   : pOpt->m_Gen_nDivPage;	
	BOOL bDimCenterGirder   = bDefaultLoad ? FALSE : pOpt->m_Gen_bDimCenterGirder;	
	BOOL bDimNormalGirder   = bDefaultLoad ? TRUE  : pOpt->m_Gen_bDimNormalGirder;		
	BOOL bAllHalf		    = bDefaultLoad ? FALSE : pOpt->m_Gen_bPyungAllHalf;	
	BOOL bVerTitleFront	    = bDefaultLoad ? TRUE  : pOpt->m_Gen_bVerTitleFront;	

	m_Options.SetOptionItem("���� ��ü", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIV_PLANJONG", bDivPlanJong ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("DIV_PLANJONG", "\t��鵵�� ���鵵�� �и� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("BUJETHICK", bBujeThick ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BUJETHICK", "\t���� �β� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);


	m_Options.SetOptionItem("���� ǥ��", IDB_ITEMS, IDB_ITEMS);		
		m_Options.SetOption("PLANVIEW", bPlanView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);

		m_Options.SetOptionItem("PLANVIEW", "\t��鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("STYLEDANMYUN_TYPE", nStyleDanmyunType);
			m_Options.SetOptionItem("STYLEDANMYUN_TYPE", "\t\t�ܸ�Ÿ�� ǥ�� ��ü�Ŵ�", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("STYLEDANMYUN_TYPE", "\t\t�ܸ�Ÿ�� ǥ�� 1�� �Ŵ���", 1, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("STYLEDANMYUN_TYPE", "\t\t�ܸ�Ÿ�� ǥ�� ����", 2, IDB_RADIOON, IDB_RADIOOFF);

		m_Options.SetOption("JONGVIEW", bJongView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGVIEW", "\t���鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("JONGCENTERGIR", bJongCenterGir ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JONGCENTERGIR", "\t\t�����߽� �Ŵ� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			//
			m_Options.SetOption("JONGGENGIRDER", bJongGenGir ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JONGGENGIRDER", "\t\t�Ϲ� �Ŵ� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("JONGGIRDER", nOutGirder);
				m_Options.SetOptionItem("JONGGIRDER", "\t\t\t�����߽� �Ŵ� ���", 0, IDB_RADIOON, IDB_RADIOOFF);
				m_Options.SetOptionItem("JONGGIRDER", "\t\t\t��ü �Ŵ� ���", 1, IDB_RADIOON, IDB_RADIOOFF);

		m_Options.SetOption("KEYPLAN", bKeyPlan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("KEYPLAN", "\t��൵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("SPLICE", bKeyPlanSpliceMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SPLICE", "\t\t�������� ��ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("JIJUMMARK", bKeyPlanJijumMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JIJUMMARK", "\t\t������ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("CROSS", bKeyPlanCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("CROSS", "\t\t���κ� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYDIMENSION", bKeyPlanDimension ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYDIMENSION", "\t\tġ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("DIRECTION", bKeyPlanDirection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("DIRECTION", "\t\t������� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ONELINE", bKeyPlanOneLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ONELINE", "\t\t�����Ѽ����� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ROVER", bKeyPlanRoVer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ROVER", "\t\t��൵ ���� ȸ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);			

		m_Options.SetOption("FRONTVIEW", bFrontView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FRONTVIEW", "\tȾ�ܸ鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("VERTITLEFRONT", bVerTitleFront ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VERTITLEFRONT", "\t\t������ �������� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("LEGEND", bLegend ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("LEGEND", "\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		
		m_Options.SetOption("BASICNOTE", bBasicNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BASICNOTE", "\t�⺻��Ʈ", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("NOTETABLE_VERT", bTableVertical ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("NOTETABLE_VERT", "\t\t�������� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			
		m_Options.SetOption("MATCHLINE", bMatchLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MATCHLINE", "\t��ġ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	


	m_Options.SetOptionItem("��鵵 ����", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PYUNG_DIM_CR", bDimCenterGirder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PYUNG_DIM_CR", "\t�����߽� ġ�� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PYUNG_DIM_NORMAL", bDimNormalGirder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PYUNG_DIM_NORMAL", "\t�ϹݰŴ� ġ�� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PYUNG_DIM", nPyungDim);
		m_Options.SetOptionItem("PYUNG_DIM","\t\t��ü�Ŵ� ġ�� ǥ��", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("PYUNG_DIM","\t\t�����Ŵ� ġ�� ǥ��", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOption("PYUNG_ALLHALF", bAllHalf ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PYUNG_ALLHALF", "\t���ϸ� �Ŵ� �߾ӱ������� �и�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("MARK_CROSS", bMarkCross ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MARK_CROSS", "\t���κ� �ܸ� ��ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("MARK_STRINGER", bMarkSringer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MARK_STRINGER", "\t���κ� �ܸ� ��ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("TEXT_STATION_PYUNG", bTextStationPyung ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("TEXT_STATION_PYUNG", "\t���� STATION ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_LINEINFO", bPyungLineInfo ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_LINEINFO", "\t�������� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("���鵵 ����", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("JONGLEVEL", bJongLevel ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGLEVEL", "\t���鵵 LEVEL�� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("TEXT_STATION_JONG", bTextStationJong ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("TEXT_STATION_JONG", "\t���� STATION ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("JIJUMSYMBOL", bJijumSymbol ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JIJUMSYMBOL", "\t������ġ SYMBOL ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("JIJUMSIMPLE", bJijumSimple ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JIJUMSIMPLE", "\t���� ǥ���� ��ġ���� ��ռ� ����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("BYUNDAN", bByundan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BYUNDAN", "\t���ܸ� ����� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("FORCETEXT", bForceText ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FORCETEXT", "\t���±��� ���� ��� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("FORCETEXTLOW", bForceTextLow ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FORCETEXTLOW", "\t���±��� ���� �Ϻ� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem("������� ���", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIV_DOM", nDivPage);
		m_Options.SetOptionItem("DIV_DOM","\t�氣 ������ ����", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t�������� ������ ����", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t���κ� ������ ����", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t��ü ������ ��������", 3, IDB_RADIOON, IDB_RADIOOFF);
		
	///// ��鵵 ���� ǥ�� ���� 
	DWORD DBuje		= bDefaultLoad ? DRAW_PLAN_ALL : pOpt->m_Gen_PyungBuje;
	BOOL bWeb		= (DBuje & DRAW_PLAN_WEB)		? TRUE : FALSE;
	BOOL bSplice	= (DBuje & DRAW_PLAN_SPLICE)    ? TRUE : FALSE;
	BOOL bFact		= (DBuje & DRAW_PLAN_FACT)		? TRUE : FALSE;
	BOOL bCrossBeam	= (DBuje & DRAW_PLAN_CROSSBEAM) ? TRUE : FALSE;
	BOOL bStringer	= (DBuje & DRAW_PLAN_STRINGER)  ? TRUE : FALSE;
	BOOL bBracket	= (DBuje & DRAW_PLAN_BRACKET)   ? TRUE : FALSE;
	BOOL bBrBeam	= (DBuje & DRAW_PLAN_BRBEAM)    ? TRUE : FALSE;
	BOOL bSolePlate	= (DBuje & DRAW_PLAN_SOLEPLATE) ? TRUE : FALSE;
	BOOL bJackUp	= (DBuje & DRAW_PLAN_JACKUP)    ? TRUE : FALSE;
	BOOL bSlab		= (DBuje & DRAW_PLAN_SLAB)		? TRUE : FALSE;

	m_Options.SetOptionItem("��鵵 ���� ǥ��", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PLAN_WEB", bWeb ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_WEB", "\t������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_JACKUP", bJackUp ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_JACKUP", "\t���������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_SPLICE", "\t��������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_FACT", bFact ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_FACT", "\t��������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_STRINGER", bStringer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_STRINGER", "\t���κ�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_CROSSBEAM", bCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_CROSSBEAM", "\t���κ�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_BRACKET", bBracket ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_BRACKET", "\t�������κ�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_BRBEAM", bBrBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_BRBEAM", "\t������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_SOLEPLATE", bSolePlate ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_SOLEPLATE", "\t���÷���Ʈ", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("PLAN_SLAB", bSlab ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLAN_SLAB", "\t������ �ܰ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

///// ���鵵 ���� ǥ�� ���� 
	BOOL bVStiff, bHStiff;
	DBuje		= bDefaultLoad ? DRAW_JONG_ALL : pOpt->m_Gen_JongBuje;
	bVStiff		= (DBuje & DRAW_JONG_VSTIFF)	? TRUE : FALSE;
	bSplice		= (DBuje & DRAW_JONG_SPLICE)	? TRUE : FALSE;
	bFact		= (DBuje & DRAW_JONG_FACT)		? TRUE : FALSE;
	bSolePlate	= (DBuje & DRAW_JONG_SOLEPLATE)	? TRUE : FALSE;
	bJackUp		= (DBuje & DRAW_JONG_JACKUP)	? TRUE : FALSE;
	bHStiff		= (DBuje & DRAW_JONG_HSTIFF)	? TRUE : FALSE;
	bSlab		= (DBuje & DRAW_JONG_SLAB)		? TRUE : FALSE;

	m_Options.SetOptionItem("���鵵 ���� ǥ��", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("JONG_HSTIFF", bHStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_HSTIFF", "\t���򺸰���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_VSTIFF", bVStiff ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_VSTIFF", "\t����������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_SPLICE", bSplice ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_SPLICE", "\t��������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_FACT", bFact ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_FACT", "\t��������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_SOLEPLATE", bSolePlate ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_SOLEPLATE", "\t���÷���Ʈ", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_JACKUP", bJackUp ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_JACKUP", "\t���������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOption("JONG_SLAB", bSlab ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem("JONG_SLAB", "\t������ �ܰ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	// ����ǥ��
	DWORD DBujeInfo			= bDefaultLoad ? BUJEINFO__ALL : pOpt->m_Gen_BujeInfo;
	BOOL bJewonFlangeBuje	= (DBujeInfo & BUJEINFO_FLANGE_NAME)	? TRUE : FALSE;
	BOOL bJewonFlangeMat	= (DBujeInfo & BUJEINFO_FLANGE_MAT)		? TRUE : FALSE;
	BOOL bJewonWebBuje		= (DBujeInfo & BUJEINFO_WEB_NAME)		? TRUE : FALSE;
	BOOL bJewonWebMat		= (DBujeInfo & BUJEINFO_WEB_MAT)		? TRUE : FALSE;
	BOOL bJewonHStiffBuje	= (DBujeInfo & BUJEINFO_HSTIFF_NAME)	? TRUE : FALSE;
	BOOL bJewonHStiffMat	= (DBujeInfo & BUJEINFO_HSTIFF_MAT)		? TRUE : FALSE;
	BOOL bJewonVStiffBuje	= (DBujeInfo & BUJEINFO_VSTIFF_NAME)	? TRUE : FALSE;
	BOOL bJewonVStiffMat	= (DBujeInfo & BUJEINFO_VSTIFF_MAT)		? TRUE : FALSE;
	BOOL bJewonJackUpBuje	= (DBujeInfo & BUJEINFO_JACKUP_NAME)	? TRUE : FALSE;
	BOOL bJewonJackUpMat	= (DBujeInfo & BUJEINFO_JACKUP_MAT)		? TRUE : FALSE;

	m_Options.SetOptionItem("����ǥ��", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOptionItem("\t������", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("FLANGEBUJE", bJewonFlangeBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("FLANGEBUJE", "\t\t�����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("FLANGEMAT", bJewonFlangeMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("FLANGEMAT", "\t\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t������", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("WEBBUJE", bJewonWebBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("WEBBUJE", "\t\t�����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("WEBMAT", bJewonWebMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("WEBMAT", "\t\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t���򺸰���", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("HSTIFFBUJE", bJewonHStiffBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HSTIFFBUJE", "\t\t�����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("HSTIFFMAT", bJewonHStiffMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("HSTIFFMAT", "\t\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t����������", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("VSTIFFBUJE", bJewonVStiffBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VSTIFFBUJE", "\t\t�����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VSTIFFMAT", bJewonVStiffMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VSTIFFMAT", "\t\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t���������", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("JACKUPBUJE", bJewonJackUpBuje ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JACKUPBUJE", "\t\t�����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("JACKUPMAT", bJewonJackUpMat ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JACKUPMAT", "\t\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionGeneral::InitGridData(BOOL bDefaultLoad)
{		
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_Gen_Scale.GetAt(0) = 100;
		pOpt->m_Gen_Scale.GetAt(1) = 50;
		pOpt->m_Gen_Scale.GetAt(2) = 800;
		pOpt->m_Gen_Scale.GetAt(3) = 800;
		pOpt->m_Gen_nUnitEaDraw = 1;	
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

	m_Grid.SetTextMatrix(0,0,"�����׸�");
	m_Grid.SetTextMatrix(1,0,"�Ŵ� �Ϲݵ�",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"�Ŵ� �ܸ鵵",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(3,0,"��൵(����)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"��൵(����)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"��´��� ����",(UINT)DT_LEFT);
	
	m_Grid.SetTextMatrix(0,1,"������");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_Gen_Scale.GetAt(0),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_Gen_Scale.GetAt(1),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_Gen_Scale.GetAt(2),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_Gen_Scale.GetAt(3),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.0f",pOpt->m_Gen_nUnitEaDraw,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);	
}

void COptionGeneral::OnOK() 
{	
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	pOpt->m_Gen_bDivPlanJong		= m_Options.GetOption("DIV_PLANJONG")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bBujeThick			= m_Options.GetOption("BUJETHICK")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bPlanView			= m_Options.GetOption("PLANVIEW")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_nStyleDanmyunType	= m_Options.GetOption("STYLEDANMYUN_TYPE");
	pOpt->m_Gen_bJongView			= m_Options.GetOption("JONGVIEW")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bJongCenterGir		= m_Options.GetOption("JONGCENTERGIR")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJongGenGir			= m_Options.GetOption("JONGGENGIRDER")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_nOutGirder			= m_Options.GetOption("JONGGIRDER");

	pOpt->m_Gen_bFrontView  		= m_Options.GetOption("FRONTVIEW")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bVerTitleFront 		= m_Options.GetOption("VERTITLEFRONT")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlan			= m_Options.GetOption("KEYPLAN")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanSpliceMark	= m_Options.GetOption("SPLICE")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanJijumMark	= m_Options.GetOption("JIJUMMARK")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanCrossBeam	= m_Options.GetOption("CROSS")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanDimension	= m_Options.GetOption("KEYDIMENSION")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanDirection	= m_Options.GetOption("DIRECTION")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanOneLine		= m_Options.GetOption("KEYPLAN_ONELINE")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bKeyPlanRoVer		= m_Options.GetOption("KEYPLAN_ROVER")==-1 ? TRUE : FALSE;	


	pOpt->m_Gen_bLegend				= m_Options.GetOption("LEGEND")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bBasicNote			= m_Options.GetOption("BASICNOTE")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bVerticalNote		= m_Options.GetOption("NOTETABLE_VERT")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJongLevel		    = m_Options.GetOption("JONGLEVEL")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJijumSymbol	    = m_Options.GetOption("JIJUMSYMBOL")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJijumSimple	    = m_Options.GetOption("JIJUMSIMPLE")==-1 ? TRUE : FALSE;		
	pOpt->m_Gen_bMarkCorss		    = m_Options.GetOption("MARK_CROSS")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bMarkStringer		= m_Options.GetOption("MARK_STRINGER")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bPyungJijumStation	= m_Options.GetOption("TEXT_STATION_PYUNG")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bPyungLineInfo		= m_Options.GetOption("PLAN_LINEINFO")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bJongJijumStation	= m_Options.GetOption("TEXT_STATION_JONG")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bByundan			= m_Options.GetOption("BYUNDAN")==-1 ? TRUE : FALSE;

	pOpt->m_Gen_bForceText			= m_Options.GetOption("FORCETEXT")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bForceTextLow		= m_Options.GetOption("FORCETEXTLOW")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_bMatchLine			= m_Options.GetOption("MATCHLINE")==-1 ? TRUE : FALSE;
	pOpt->m_Gen_nPyungDim			= m_Options.GetOption("PYUNG_DIM");
	pOpt->m_Gen_nDivPage			= m_Options.GetOption("DIV_DOM");
	pOpt->m_Gen_bDimCenterGirder	= m_Options.GetOption("PYUNG_DIM_CR")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bDimNormalGirder	= m_Options.GetOption("PYUNG_DIM_NORMAL")==-1 ? TRUE : FALSE;	
	pOpt->m_Gen_bPyungAllHalf		= m_Options.GetOption("PYUNG_ALLHALF")==-1 ? TRUE : FALSE;

	// �������� �ɼ�
	DWORD dOptJweon = 0;
	if(m_Options.GetOption("FLANGEBUJE"))		dOptJweon |= BUJEINFO_FLANGE_NAME;
	if(m_Options.GetOption("FLANGEMAT"))		dOptJweon |= BUJEINFO_FLANGE_MAT;
	if(m_Options.GetOption("WEBBUJE"))			dOptJweon |= BUJEINFO_WEB_NAME;
	if(m_Options.GetOption("WEBMAT"))			dOptJweon |= BUJEINFO_WEB_MAT;
	if(m_Options.GetOption("HSTIFFBUJE"))		dOptJweon |= BUJEINFO_HSTIFF_NAME;
	if(m_Options.GetOption("HSTIFFMAT"))		dOptJweon |= BUJEINFO_HSTIFF_MAT;
	if(m_Options.GetOption("VSTIFFBUJE"))		dOptJweon |= BUJEINFO_VSTIFF_NAME;
	if(m_Options.GetOption("VSTIFFMAT"))		dOptJweon |= BUJEINFO_VSTIFF_MAT;
	if(m_Options.GetOption("JACKUPBUJE"))		dOptJweon |= BUJEINFO_JACKUP_NAME;
	if(m_Options.GetOption("JACKUPMAT"))		dOptJweon |= BUJEINFO_JACKUP_MAT;
	pOpt->m_Gen_BujeInfo = dOptJweon;

	// ��鵵 �ɼ�
	DWORD dOptPlan = 0;
	if(m_Options.GetOption("PLAN_WEB"))			dOptPlan |= DRAW_PLAN_WEB;
	if(m_Options.GetOption("PLAN_SPLICE"))		dOptPlan |= DRAW_PLAN_SPLICE;
	if(m_Options.GetOption("PLAN_FACT"))		dOptPlan |= DRAW_PLAN_FACT;
	if(m_Options.GetOption("PLAN_CROSSBEAM"))	dOptPlan |= DRAW_PLAN_CROSSBEAM;
	if(m_Options.GetOption("PLAN_STRINGER"))	dOptPlan |= DRAW_PLAN_STRINGER;
	if(m_Options.GetOption("PLAN_BRACKET"))		dOptPlan |= DRAW_PLAN_BRACKET;
	if(m_Options.GetOption("PLAN_BRBEAM"))		dOptPlan |= DRAW_PLAN_BRBEAM;
	if(m_Options.GetOption("PLAN_SOLEPLATE"))	dOptPlan |= DRAW_PLAN_SOLEPLATE;
	if(m_Options.GetOption("PLAN_JACKUP"))		dOptPlan |= DRAW_PLAN_JACKUP;
	if(m_Options.GetOption("PLAN_SLAB"))		dOptPlan |= DRAW_PLAN_SLAB;
	pOpt->m_Gen_PyungBuje = dOptPlan;
	
	// ���鵵 �ɼ�
	DWORD dOptJong = 0;
	if(m_Options.GetOption("JONG_VSTIFF"))		dOptJong |= DRAW_JONG_VSTIFF;
	if(m_Options.GetOption("JONG_HSTIFF"))		dOptJong |= DRAW_JONG_HSTIFF;
	if(m_Options.GetOption("JONG_SPLICE"))		dOptJong |= DRAW_JONG_SPLICE;
	if(m_Options.GetOption("JONG_FACT"))		dOptJong |= DRAW_JONG_FACT;
	if(m_Options.GetOption("JONG_SOLEPLATE"))	dOptJong |= DRAW_JONG_SOLEPLATE;
	if(m_Options.GetOption("JONG_JACKUP"))		dOptJong |= DRAW_JONG_JACKUP;
	if(m_Options.GetOption("JONG_SLAB"))		dOptJong |= DRAW_JONG_SLAB;
	pOpt->m_Gen_JongBuje = dOptJong;

	double scale = 0;
	for(long i=1; i<pOpt->m_Gen_Scale.GetSize()+1; i++)
	{
		scale = m_Grid.GetTextMatrixDouble(i,1);
		pOpt->m_Gen_Scale.SetAt(i-1, scale);
	}
	pOpt->m_Gen_nUnitEaDraw = m_Grid.GetTextMatrixLong(5,1);
	
	pStd->GetDrawPageMng()->SetModified(PAGE_GENERAL);

	CPropertyPage::OnOK();
}

// OptionStd�� �ʱⰪ�� ��ġ���Ѿ���....
void COptionGeneral::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionGeneral::OnSetActive() 
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;
	
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_GENERAL;

	return CPropertyPage::OnSetActive();
}

