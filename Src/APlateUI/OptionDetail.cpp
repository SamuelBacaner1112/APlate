// OptionDetail.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDetail property page

IMPLEMENT_DYNCREATE(COptionDetail, CPropertyPage)

COptionDetail::COptionDetail() : CPropertyPage(COptionDetail::IDD)
{
	//{{AFX_DATA_INIT(COptionDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionDetail::~COptionDetail()
{
}

void COptionDetail::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDetail)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);	
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDetail, CPropertyPage)
	//{{AFX_MSG_MAP(COptionDetail)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDetail message handlers

BOOL COptionDetail::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	InitTreeData(FALSE);
	InitGridData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionDetail::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();
			
	m_Options.SetOptionItem("���� ǥ��", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PLANVIEW", (bDefaultLoad ? TRUE : pOpt->m_Det_bPlanView) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLANVIEW", "\t��鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("JONGVIEW", (bDefaultLoad ? TRUE : pOpt->m_Det_bJongView) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGVIEW", "\t���鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);		
		if(pDB->IsTUGir())
		{
			m_Options.SetOption("DANVIEW", (bDefaultLoad ? TRUE : pOpt->m_Det_bGirderdan) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("DANVIEW", "\t�ܸ鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);							
		}
		m_Options.SetOption("KEYPLAN", (bDefaultLoad ? TRUE : pOpt->m_Det_bKeyPlan) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("KEYPLAN", "\t��൵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("SPLICE", (bDefaultLoad ? TRUE : pOpt->m_Det_bKeyPlanSpliceMark) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("SPLICE", "\t\t�������� ��ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("JIJUMMARK", (bDefaultLoad ? TRUE : pOpt->m_Det_bKeyPlanJijumMark) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("JIJUMMARK", "\t\t������ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("CROSS", (bDefaultLoad ? TRUE : pOpt->m_Det_bKeyPlanCrossBeam) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("CROSS", "\t\t���κ� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYDIMENSION", (bDefaultLoad ? TRUE : pOpt->m_Det_bKeyPlanDimension) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYDIMENSION", "\t\tġ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("DIRECTION", (bDefaultLoad ? TRUE : pOpt->m_Det_bKeyPlanDirection) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("DIRECTION", "\t\t������� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ONELINE", (bDefaultLoad ? TRUE : pOpt->m_Det_bKeyPlanOneLine) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ONELINE", "\t\t�����Ѽ����� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("KEYPLAN_ROVER", (bDefaultLoad ? FALSE : pOpt->m_Det_bKeyPlanRoVer) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("KEYPLAN_ROVER", "\t\t��൵ ���� ȸ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("WELDDET", (bDefaultLoad ? TRUE : pOpt->m_Det_bWeldDetail) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("WELDDET", "\t������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("FIELDWELDDET", (bDefaultLoad ? TRUE : pOpt->m_Det_bFieldWeldDetail) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("FIELDWELDDET", "\t���������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("SOLEPLATE", (bDefaultLoad ? TRUE : pOpt->m_Det_bSolePlateDetail) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SOLEPLATE", "\t���¹�ħ ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("VSTIFF", (bDefaultLoad ? TRUE : pOpt->m_Det_bVStiffDetail) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VSTIFF", "\t���������� ���� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("VSTIFF_ALL", (bDefaultLoad ? FALSE : pOpt->m_Det_bVStiffAll) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("VSTIFF_ALL", "\t\t���������� ��ü ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

		m_Options.SetOption("HSTIFF", (bDefaultLoad ? TRUE : pOpt->m_Det_bHStiffDetail) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("HSTIFF", "\t���򺸰��� ���� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("LEGEND", (bDefaultLoad ? TRUE : pOpt->m_Det_bLegend) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("LEGEND", "\t����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("BASICNOTE", (bDefaultLoad ? TRUE : pOpt->m_Det_bBasicNote) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("BASICNOTE", "\t�⺻��Ʈ", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);				
			m_Options.SetOption("NOTETABLE_VERT", (bDefaultLoad ? FALSE : pOpt->m_Det_bBasicNoteVert) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("NOTETABLE_VERT", "\t\t�������� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("��鵵 ����", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("STTENDSTATIONPYUNG", (bDefaultLoad ? TRUE : pOpt->m_Det_bSttEndStationPyung) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("STTENDSTATIONPYUNG", "\t������ �����̼� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("PIERABUTPYUNG", (bDefaultLoad ? FALSE : pOpt->m_Det_bPierAbutPyung) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PIERABUTPYUNG", "\t����, ���� ���¸� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("LINEINFOPYUNG", (bDefaultLoad ? TRUE : pOpt->m_Det_bLineInfoPyung) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("LINEINFOPYUNG", "\t�������� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("UPWELDGRIND", (bDefaultLoad ? TRUE : pOpt->m_Det_UpperWeldbGrind) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("UPWELDGRIND", "\t����÷��� ���� Gǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("DNWELDGRIND", (bDefaultLoad ? TRUE : pOpt->m_Det_LowerWeldbGrind) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("DNWELDGRIND", "\t�Ϻ��÷��� ���� Gǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		
		m_Options.SetOptionItem("���鵵 ����", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("JONGLEVEL", (bDefaultLoad ? TRUE : pOpt->m_Det_bLevelJong) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("JONGLEVEL", "\t���鵵 LEVEL�� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("SECTIONMARKJONG", (bDefaultLoad ? TRUE : pOpt->m_Det_bSectionMarkJong) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SECTIONMARKJONG", "\t�ܸ鸶ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("SOLEPLATEJONG", (bDefaultLoad ? TRUE : pOpt->m_Det_bSolePlateJong) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SOLEPLATEJONG", "\t���÷���Ʈ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("PIERABUTJONG", (bDefaultLoad ? FALSE : pOpt->m_Det_bPierAbutJong) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PIERABUTJONG", "\t����, ���� ���¸� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOptionItem("VSTIFFLEFTRIGHVIEW", "\t���������� ���� ��(��)ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("VSTIFFLEFTRIGHVIEW", (bDefaultLoad ? FALSE : pOpt->m_Det_bVStiffLeftRight) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		
	m_Options.SetOptionItem("������� ���", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIV_DOM", (bDefaultLoad ? 2 : pOpt->m_Det_nDivPage));
		m_Options.SetOptionItem("DIV_DOM","\t�氣 ������ ����", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t�������� ������ ����", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t���κ� ������ ����", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIV_DOM","\t��ü ������ ��������", 3, IDB_RADIOON, IDB_RADIOOFF);
		
	m_Options.SetOptionItem("����ǥ��", IDB_ITEMS, IDB_ITEMS);		
		m_Options.SetOption("VSTIFFJEWON", (bDefaultLoad ? TRUE : pOpt->m_Det_bVStiffJewon) ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("VSTIFFJEWON", "\t����������&���������", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		
	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionDetail::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;

	long i=0;
	if(pDB->IsTUGir() && pOpt->m_Det_Scale.GetSize() == 8)
		pOpt->m_Det_Scale.Add(30);

	if(bDefaultLoad)
	{
		pOpt->m_Det_Scale.RemoveAll();
		pOpt->m_Det_Scale.Add(30);
		pOpt->m_Det_Scale.Add(500);
		pOpt->m_Det_Scale.Add(500);
		pOpt->m_Det_Scale.Add(3);
		pOpt->m_Det_Scale.Add(20);
		pOpt->m_Det_Scale.Add(10);
		pOpt->m_Det_Scale.Add(10);
		pOpt->m_Det_Scale.Add(10);
		if(pDB->IsTUGir())
			pOpt->m_Det_Scale.Add(30); // TU�ܸ鵵
//		pOpt->m_Det_dWeldWidth = 5;		
		pOpt->m_Det_nUnitEaDraw = pDB->IsTUGir() ? 4 : 2;
		pOpt->m_Det_MaxSebuDetailSu = 0;
	}
	
	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(pOpt->m_Det_Scale.GetSize()+3);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);		
	
	m_Grid.SetColumnWidth(0,Rect.Width()*2/3);
	m_Grid.SetColumnWidth(1,Rect.Width()*1/3);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));	
	m_Grid.SetTextBkColor(RGB(255,255,255));

	long nRow = 0;
	m_Grid.SetTextMatrix(nRow++,0,"�����׸�");
	m_Grid.SetTextMatrix(nRow++,0,"�Ŵ� �󼼵�",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(nRow++,0,"��൵(����)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"��൵(����)",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"�� �� �� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"���¹�ħ ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"���������� ���� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"���򺸰��� ���� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"���򺸰��� ����(Ⱦ����) ��",(UINT)DT_LEFT);
	if(pOpt->m_Det_Scale.GetSize() == nRow)
		m_Grid.SetTextMatrix(nRow++,0,"TU �ܸ鵵",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"���κ� ���� ����",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++,0,"���� �� �ִ� ��� ����",(UINT)DT_LEFT);

	m_Grid.SetTextMatrix(0,1,"������");
	for(nRow = 1; nRow < pOpt->m_Det_Scale.GetSize()+1; nRow++)
		m_Grid.SetTextMatrix(nRow,1,"%.1f",pOpt->m_Det_Scale.GetAt(nRow-1),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++,1,"%.0f",pOpt->m_Det_nUnitEaDraw,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++,1,"%.0f",pOpt->m_Det_MaxSebuDetailSu,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionDetail::OnOK() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;	

	pOpt->m_Det_bPlanView   = m_Options.GetOption("PLANVIEW");
	pOpt->m_Det_bJongView   = m_Options.GetOption("JONGVIEW");
	pOpt->m_Det_bGirderdan	= pDB->IsTUGir() ? m_Options.GetOption("DANVIEW") : FALSE;
	pOpt->m_Det_bKeyPlan	= m_Options.GetOption("KEYPLAN");
		pOpt->m_Det_bKeyPlanSpliceMark  = m_Options.GetOption("SPLICE");
		pOpt->m_Det_bKeyPlanJijumMark   = m_Options.GetOption("JIJUMMARK");
		pOpt->m_Det_bKeyPlanCrossBeam   = m_Options.GetOption("CROSS");
		pOpt->m_Det_bKeyPlanDimension   = m_Options.GetOption("KEYDIMENSION");
		pOpt->m_Det_bKeyPlanDirection	= m_Options.GetOption("DIRECTION");
		pOpt->m_Det_bKeyPlanOneLine		= m_Options.GetOption("KEYPLAN_ONELINE");
		pOpt->m_Det_bKeyPlanRoVer		= m_Options.GetOption("KEYPLAN_ROVER");
	pOpt->m_Det_bWeldDetail			= m_Options.GetOption("WELDDET");
	pOpt->m_Det_bFieldWeldDetail	= m_Options.GetOption("FIELDWELDDET");
	pOpt->m_Det_bSolePlateDetail	= m_Options.GetOption("SOLEPLATE");
	pOpt->m_Det_bVStiffDetail		= m_Options.GetOption("VSTIFF");
		pOpt->m_Det_bVStiffAll			= m_Options.GetOption("VSTIFF_ALL");
	pOpt->m_Det_bHStiffDetail		= m_Options.GetOption("HSTIFF");	
	pOpt->m_Det_bLegend				= m_Options.GetOption("LEGEND");
	pOpt->m_Det_bBasicNote			= m_Options.GetOption("BASICNOTE");
	pOpt->m_Det_bBasicNoteVert		= m_Options.GetOption("NOTETABLE_VERT");
	
	pOpt->m_Det_bSttEndStationPyung		= m_Options.GetOption("STTENDSTATIONPYUNG");
	pOpt->m_Det_bPierAbutPyung			= m_Options.GetOption("PIERABUTPYUNG");
	pOpt->m_Det_bLineInfoPyung			= m_Options.GetOption("LINEINFOPYUNG");

	pOpt->m_Det_bLevelJong				= m_Options.GetOption("JONGLEVEL");
	pOpt->m_Det_bSectionMarkJong		= m_Options.GetOption("SECTIONMARKJONG");		
	pOpt->m_Det_bSolePlateJong			= m_Options.GetOption("SOLEPLATEJONG");
	pOpt->m_Det_bPierAbutJong			= m_Options.GetOption("PIERABUTJONG");
	pOpt->m_Det_bVStiffLeftRight		= m_Options.GetOption("VSTIFFLEFTRIGHVIEW");

	pOpt->m_Det_nDivPage			= m_Options.GetOption("DIV_DOM");			
	pOpt->m_Det_bVStiffJewon		= m_Options.GetOption("VSTIFFJEWON");

	pOpt->m_Det_UpperWeldbGrind		= m_Options.GetOption("UPWELDGRIND");
	pOpt->m_Det_LowerWeldbGrind		= m_Options.GetOption("DNWELDGRIND");

	double scale = 0;
	long nSize = pOpt->m_Det_Scale.GetSize();
	pOpt->m_Det_Scale.RemoveAll();
	long i = 0;
	for(i=1; i<nSize+1; i++)
	{
		scale = m_Grid.GetTextMatrixDouble(i,1);
		pOpt->m_Det_Scale.Add(scale);
	}	
	pOpt->m_Det_nUnitEaDraw   = m_Grid.GetTextMatrixLong(i++,1);
	pOpt->m_Det_MaxSebuDetailSu = m_Grid.GetTextMatrixLong(i,1);

	pStd->GetDrawPageMng()->SetModified(PAGE_DETAIL);	
	CPropertyPage::OnOK();
}

BOOL COptionDetail::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	
	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_DETAIL;

	return CPropertyPage::OnSetActive();
}

void COptionDetail::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);	
}
