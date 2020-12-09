// OptionSlabGen.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSlabGen dialog

IMPLEMENT_DYNCREATE(COptionSlabGen, CPropertyPage)

COptionSlabGen::COptionSlabGen() : CPropertyPage(COptionSlabGen::IDD)
{
	//{{AFX_DATA_INIT(COptionSlabGen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionSlabGen::~COptionSlabGen()
{
}

void COptionSlabGen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionSlabGen)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionSlabGen, CPropertyPage)
	//{{AFX_MSG_MAP(COptionSlabGen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSlabGen message handlers
BOOL COptionSlabGen::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData(FALSE);
	InitGridData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionSlabGen::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bPlanView				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_PlanView;	// ��鵵
	long nPlanView_DimOpt		=	bDefaultLoad ? 0	: pOpt->m_nSlab_PlanView_DimOpt;	// ��鵵 ġ��ǥ�� �ɼ�
	BOOL bJongView				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_JongView;	// ���鵵
	BOOL bFrontView				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_FrontView; // Ⱦ�ܸ鵵
	BOOL bFrontViewLeft			=	bDefaultLoad ? TRUE : pOpt->m_bSlab_FrontViewLeft; // Ⱦ�ܸ鵵(����)
	BOOL bFrontViewRight		=	bDefaultLoad ? FALSE : pOpt->m_bSlab_FrontViewRight; // Ⱦ�ܸ鵵(����)
	BOOL bFrontDetView			=	bDefaultLoad ? TRUE : pOpt->m_bSlab_FrontDetView; // Ⱦ�ܸ鵵 ��
	BOOL bSectionAA				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_SectionAA;		// �Ŵ��� �ܸ� ��
	BOOL bSectionBB				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_SectionBB;		// �Ŵ��� �ܸ� ��
//	BOOL bSectionCC				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_SectionCC;		// �Ŵ��� �߾Ӵܸ� ��
	BOOL bSectionCC				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_SectionCC;		// ���κ� �ܸ� ��
//	BOOL bSectionEE				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_SectionEE;		// ��Ʈ���� ��
	BOOL bGuideWall				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_GuideWall;			// ��ȣ�� ��
	BOOL bNotch					=	bDefaultLoad ? TRUE : pOpt->m_bSlab_Notch;			// NOTCH ��
	BOOL bKeyPlan				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_KeyPlan;		// ��൵
	BOOL bKeyPlanRoVer			=   bDefaultLoad ? FALSE : pOpt-> m_bSlab_bKeyPlanRoVer;
	long nDivType				=	bDefaultLoad ? 0	: pOpt->m_nSlab_DivType;		// ���� ���� ���
	BOOL bUpperSlabLeftDim		=	bDefaultLoad ? TRUE : pOpt->m_bSlab_UpperSlabLeftDim;	// ���̾�����(��)
	BOOL bUpperJiganLeftDim		=	bDefaultLoad ? TRUE : pOpt->m_bSlab_UpperJiganLeftDim;	// ��� ��������(��)
	BOOL bLowerSlabRightDim		=	bDefaultLoad ? TRUE : pOpt->m_bSlab_LowerSlabRightDim;	// ���̾�����(��)
	BOOL bLowerJiganRightDim	=	bDefaultLoad ? TRUE : pOpt->m_bSlab_LowerJiganRightDim;	// �Ϻ� ��������(��)
	BOOL bMatchLine				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_MatchLine;			// ��ġ����
	BOOL bLineInfo				=	bDefaultLoad ? TRUE : pOpt->m_bSlab_bPyungLineInfo;		// ��������

	BOOL bDimsionTable			=	bDefaultLoad ? TRUE : pOpt->m_bSlab_DimTable;		// ġ����ȭ ���̺�
	long nDimsionTableType		=	bDefaultLoad ? 1	: pOpt->m_nSlab_DimTableType;	// ġ����ȭ ���̺� Ÿ�� 
	BOOL bDimsionTableFirst		=	bDefaultLoad ? FALSE : pOpt->m_bSlab_DimTableFirstPage;	// ù�������� ǥ��
	BOOL bDimsionPyungubae		=	bDefaultLoad ? TRUE : pOpt->m_bSlab_Pyungubae;		// ���� ���뿩��

	BOOL bKeyPlanSpliceMark	= bDefaultLoad ? FALSE  : pOpt->m_bSlab_KeyPlanSpliceMark;	//��൵
	BOOL bKeyPlanJijumMark	= bDefaultLoad ? TRUE : pOpt->m_bSlab_KeyPlanJijumMark;
	BOOL bKeyPlanCrossBeam	= bDefaultLoad ? TRUE  : pOpt->m_bSlab_KeyPlanCrossBeam;
	BOOL bKeyPlanDimension	= bDefaultLoad ? FALSE : pOpt->m_bSlab_KeyPlanDimension;
	BOOL bKeyPlanOneLine	= bDefaultLoad ? FALSE : pOpt->m_bSlab_KeyPlanOneLine;
	BOOL bKeyPlanDirection	= bDefaultLoad ? TRUE  : pOpt->m_bSlab_KeyPlanDirection;

	long nNotchType			= bDefaultLoad ? 0		: pOpt->m_nSlab_NotchType;

	m_Options.SetOptionItem("���� ǥ��", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PLANVIEW", bPlanView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLANVIEW", "\t��鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("PLANVIEW_DIMOPT", nPlanView_DimOpt);
			m_Options.SetOptionItem("PLANVIEW_DIMOPT","\t\t����ġ�� : �Ŵ��� ǥ��", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("PLANVIEW_DIMOPT","\t\t����ġ�� : �Ŵ����� ǥ��", 1, IDB_RADIOON, IDB_RADIOOFF);
		
		m_Options.SetOption("JONGVIEW", bJongView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGVIEW", "\t���鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("FRONTVIEW", bFrontView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FRONTVIEW", "\tȾ�ܸ鵵 ��������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("FRONTVIEWLEFT", bFrontViewLeft ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("FRONTVIEWLEFT", "\t\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("FRONTVIEWRIGHT", bFrontViewRight ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("FRONTVIEWRIGHT", "\t\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOption("FRONTDETVIEW", bFrontDetView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FRONTDETVIEW", "\tȾ�ܸ鵵 ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		
		m_Options.SetOption("SECTIONAA", bSectionAA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SECTIONAA", "\t�Ŵ��� �ܸ� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("SECTIONBB", bSectionBB ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SECTIONBB", "\t�Ŵ��� �ܺ� �ܸ� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

//		m_Options.SetOption("SECTIONCC", bSectionCC ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//		m_Options.SetOptionItem("SECTIONCC", "\t�Ŵ��� �߾� �ܸ� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("SECTIONCC", bSectionCC ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SECTIONCC", "\t���κ� �ܸ� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

//		m_Options.SetOption("SECTIONEE", bSectionEE ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//		m_Options.SetOptionItem("SECTIONEE", "\t���κ� �ܸ� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("GUIDEWALL", bGuideWall ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("GUIDEWALL", "\t��ȣ�� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("NOTCH", bNotch ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("NOTCH", "\tNOTCH ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("NOTCHTYPE", nNotchType);
			m_Options.SetOptionItem("NOTCHTYPE","\t\t�˷�̴���", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("NOTCHTYPE","\t\t��ٸ��� ���", 1, IDB_RADIOON, IDB_RADIOOFF);
		
		m_Options.SetOption("MATCHLINE", bMatchLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MATCHLINE", "\t��ġ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

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
			

		m_Options.SetOption("DIMTABLE", bDimsionTable? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("DIMTABLE", "\tġ����ȭ ���̺�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("DIMTABLETYPE", nDimsionTableType);
			m_Options.SetOptionItem("DIMTABLETYPE","\t\t����,������ ǥ��", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("DIMTABLETYPE","\t\t��� ���κ� ǥ��", 1, IDB_RADIOON, IDB_RADIOOFF);
//			m_Options.SetOption("DIMTABLEFIRSTPAGE", bDimsionTableFirst ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//			m_Options.SetOptionItem("DIMTABLEFIRSTPAGE", "\t\tù ������������ ���̱�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("DIMTABLEPYUNGUBAE", bDimsionPyungubae ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("DIMTABLEPYUNGUBAE", "\t\t���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("��� �ɼ�", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("LINEINFO", bLineInfo ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("LINEINFO", "\t���� ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("������� ���", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIVTYPE", nDivType);
		m_Options.SetOptionItem("DIVTYPE","\t�氣 ������ ����", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIVTYPE","\t�������� ������ ����", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIVTYPE","\t���κ� ������ ����", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIVTYPE","\t��ü ������ ��������", 3, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOptionItem("ġ����", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOptionItem("\t���", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("UPPERSLABLEFTDIM", bUpperSlabLeftDim ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("UPPERSLABLEFTDIM", "\t\t���κ�(��)", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("UPPERJIGANLEFTDIM", bUpperJiganLeftDim ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("UPPERJIGANLEFTDIM", "\t\t��� ���� ����(��)", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOptionItem("\t�Ϻ�", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("LOWERSLABRIGHTDIM", bLowerSlabRightDim ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("LOWERSLABRIGHTDIM", "\t\t���κ�(��)", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("LOWERJIGANRIGHTDIM", bLowerJiganRightDim ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("LOWERJIGANRIGHTDIM", "\t\t�Ϻ� ���� ����(��)", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionSlabGen::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_dSlab_ScaleSlabGen = 100;	
		pOpt->m_dSlab_ScaleFrontDet = 50;	
		pOpt->m_dSlab_ScaleSectionAA = 30;
		pOpt->m_dSlab_ScaleSectionBB = 30;
//		pOpt->m_dSlab_ScaleSectionCC = 30;
		pOpt->m_dSlab_ScaleSectionCC = 30;
//		pOpt->m_dSlab_ScaleSectionEE = 30;
		pOpt->m_dSlab_ScaleGuideWall = 30;
		pOpt->m_dSlab_ScaleNothch = 2;
		pOpt->m_dSlab_ScaleKeyPlanHor = 800;
		pOpt->m_dSlab_ScaleKeyPlanVer = 800;
		pOpt->m_nSlab_DivNum = 1;		
		pOpt->m_dSlab_DimTableScale = 100;
		pOpt->m_dSlab_DimTableHeightFactor = 1;
		pOpt->m_dSlab_DimTableWidhtFactor = 1;
		pOpt->m_nSlab_NotchType				= 0;
		pOpt->m_dSlab_NotchHeight			= 20;
		pOpt->m_dSlab_NotchTopWidth			= 30;
		pOpt->m_dSlab_NotchBotWidth			= 30;
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(17);
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
	m_Grid.SetTextMatrix(1,0,"������ �Ϲݵ�",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"Ⱦ�ܸ鵵 ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"�Ŵ��� �ܸ� ��",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(4,0,"�Ŵ��� �߾� �ܸ� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"���κ� �ܸ� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(6,0,"��ȣ�� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(7,0,"NOTCH ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(8,0,"   - NOTCH ����",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(9,0,"   - NOTCH ��(���)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(10,0,"   - NOTCH ��(�ϴ�)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(11,0,"��൵(����)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(12,0,"��൵(����)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(13,0,"��´��� ����",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(14,0,"��ȭġ�� ���̺�",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(15,0,"��ȭġ�� ���̺� ���̰��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(16,0,"��ȭġ�� ���̺� �����",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"������");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dSlab_ScaleSlabGen,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dSlab_ScaleFrontDet,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_dSlab_ScaleSectionAA,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dSlab_ScaleSectionBB,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5,1,"%.1f",pOpt->m_dSlab_ScaleSectionCC,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(6,1,"%.1f",pOpt->m_dSlab_ScaleGuideWall,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(7,1,"%.1f",pOpt->m_dSlab_ScaleNothch,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(8,1,"%.1f",pOpt->m_dSlab_NotchHeight,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(9,1,"%.1f",pOpt->m_dSlab_NotchTopWidth,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(10,1,"%.1f",pOpt->m_dSlab_NotchBotWidth,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(11,1,"%.1f",pOpt->m_dSlab_ScaleKeyPlanHor,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(12,1,"%.1f",pOpt->m_dSlab_ScaleKeyPlanVer,(UINT)DT_RIGHT);	
	m_Grid.SetTextMatrix(13,1,"%.0f",pOpt->m_nSlab_DivNum,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(14,1,"%.1f",pOpt->m_dSlab_DimTableScale,(UINT)DT_RIGHT);	
	m_Grid.SetTextMatrix(15,1,"%.1f",pOpt->m_dSlab_DimTableHeightFactor,(UINT)DT_RIGHT);	
	m_Grid.SetTextMatrix(16,1,"%.1f",pOpt->m_dSlab_DimTableWidhtFactor,(UINT)DT_RIGHT);	
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionSlabGen::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}

void COptionSlabGen::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_bSlab_PlanView				= m_Options.GetOption("PLANVIEW");		// ��鵵
	pOpt->m_nSlab_PlanView_DimOpt		= m_Options.GetOption("PLANVIEW_DIMOPT");	// ��鵵 ġ���ɼ�
	pOpt->m_bSlab_JongView				= m_Options.GetOption("JONGVIEW");		// ���鵵
	pOpt->m_bSlab_FrontView				= m_Options.GetOption("FRONTVIEW");		// Ⱦ�ܸ鵵(����)
	pOpt->m_bSlab_FrontViewLeft			= m_Options.GetOption("FRONTVIEWLEFT");		// Ⱦ�ܸ鵵(����)
	pOpt->m_bSlab_FrontViewRight		= m_Options.GetOption("FRONTVIEWRIGHT");	// Ⱦ�ܸ鵵(����)
	pOpt->m_bSlab_FrontDetView			= m_Options.GetOption("FRONTDETVIEW");	// Ⱦ�ܸ鵵 ��
	pOpt->m_bSlab_SectionAA				= m_Options.GetOption("SECTIONAA");		// �Ŵ��� �ܸ� ��
	pOpt->m_bSlab_SectionBB				= m_Options.GetOption("SECTIONBB");		// �ڽ��� �ܸ� ��
	pOpt->m_bSlab_SectionCC				= m_Options.GetOption("SECTIONCC");		// ���κ� �ܸ� ��
	pOpt->m_bSlab_GuideWall				= m_Options.GetOption("GUIDEWALL");		// ��ȣ�� ��
	pOpt->m_bSlab_Notch					= m_Options.GetOption("NOTCH");			// NOTCH ��
	pOpt->m_nSlab_NotchType				= m_Options.GetOption("NOTCHTYPE");		// Notch Type
	pOpt->m_bSlab_KeyPlan				= m_Options.GetOption("KEYPLAN");		// ��൵
	pOpt->m_bSlab_bKeyPlanRoVer			= m_Options.GetOption("KEYPLAN_ROVER");	// ��൵
	pOpt->m_nSlab_DivType				= m_Options.GetOption("DIVTYPE");		// ���� ���� ���
	pOpt->m_bSlab_UpperSlabLeftDim		= m_Options.GetOption("UPPERSLABLEFTDIM");		// ���̾�����(��)
	pOpt->m_bSlab_UpperJiganLeftDim		= m_Options.GetOption("UPPERJIGANLEFTDIM");		// ��� ��������(��)
	pOpt->m_bSlab_LowerSlabRightDim		= m_Options.GetOption("LOWERSLABRIGHTDIM");		// ���̾�����(��)
	pOpt->m_bSlab_LowerJiganRightDim	= m_Options.GetOption("LOWERJIGANRIGHTDIM");	// �Ϻ� ��������(��)
	pOpt->m_bSlab_MatchLine				= m_Options.GetOption("MATCHLINE");
	pOpt->m_bSlab_bPyungLineInfo		= m_Options.GetOption("LINEINFO");
	pOpt->m_bSlab_DimTable				= m_Options.GetOption("DIMTABLE");				// ġ����ȭ ���̺�
	pOpt->m_nSlab_DimTableType			= m_Options.GetOption("DIMTABLETYPE");			// ġ����ȭ ���̺�
	pOpt->m_bSlab_Pyungubae				= m_Options.GetOption("DIMTABLEPYUNGUBAE");
	pOpt->m_bSlab_KeyPlanSpliceMark		= m_Options.GetOption("SPLICE");			// ��൵
	pOpt->m_bSlab_KeyPlanJijumMark		= m_Options.GetOption("JIJUMMARK");
	pOpt->m_bSlab_KeyPlanCrossBeam		= m_Options.GetOption("CROSS");
	pOpt->m_bSlab_KeyPlanDimension		= m_Options.GetOption("KEYDIMENSION");
	pOpt->m_bSlab_KeyPlanDirection		= m_Options.GetOption("DIRECTION");
	pOpt->m_bSlab_KeyPlanOneLine		= m_Options.GetOption("KEYPLAN_ONELINE");

	pOpt->m_dSlab_ScaleSlabGen			= m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dSlab_ScaleFrontDet			= m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_dSlab_ScaleSectionAA		= m_Grid.GetTextMatrixDouble(3,1);
	pOpt->m_dSlab_ScaleSectionBB		= m_Grid.GetTextMatrixDouble(4,1);
	pOpt->m_dSlab_ScaleSectionCC		= m_Grid.GetTextMatrixDouble(5,1);
	pOpt->m_dSlab_ScaleGuideWall		= m_Grid.GetTextMatrixDouble(6,1);
	pOpt->m_dSlab_ScaleNothch			= m_Grid.GetTextMatrixDouble(7,1);
	pOpt->m_dSlab_NotchHeight			= m_Grid.GetTextMatrixDouble(8,1);
	pOpt->m_dSlab_NotchTopWidth			= m_Grid.GetTextMatrixDouble(9,1);
	pOpt->m_dSlab_NotchBotWidth			= m_Grid.GetTextMatrixDouble(10,1);
	pOpt->m_dSlab_ScaleKeyPlanHor		= m_Grid.GetTextMatrixDouble(11,1);
	pOpt->m_dSlab_ScaleKeyPlanVer		= m_Grid.GetTextMatrixDouble(12,1);
	pOpt->m_nSlab_DivNum				= m_Grid.GetTextMatrixLong(13,1);
	pOpt->m_dSlab_DimTableScale			= m_Grid.GetTextMatrixDouble(14,1);
	pOpt->m_dSlab_DimTableHeightFactor	= m_Grid.GetTextMatrixDouble(15,1);
	pOpt->m_dSlab_DimTableWidhtFactor	= m_Grid.GetTextMatrixDouble(16,1);

	pStd->GetDrawPageMng()->SetModified(PAGE_SLABGEN);

	CPropertyPage::OnOK();
}

BOOL COptionSlabGen::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;


	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_SLABGEN;
	
	return CPropertyPage::OnSetActive();
}

// OptionStd�� �ʱⰪ�� ��ġ���Ѿ���....
void COptionSlabGen::OnDefault()
{
	m_ListOption.DeleteAllItems();

	InitTreeData(TRUE);
	InitGridData(TRUE);
}

