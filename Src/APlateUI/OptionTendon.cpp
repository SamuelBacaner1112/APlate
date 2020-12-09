// OptionTendon.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTendon property page

IMPLEMENT_DYNCREATE(COptionTendon, CPropertyPage)

COptionTendon::COptionTendon() : CPropertyPage(COptionTendon::IDD)
{
	//{{AFX_DATA_INIT(COptionTendon)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionTendon::~COptionTendon()
{
}

void COptionTendon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionTendon)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionTendon, CPropertyPage)
	//{{AFX_MSG_MAP(COptionTendon)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTendon message handlers
BOOL COptionTendon::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData(FALSE);
	InitGridData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionTendon::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bPlanView				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_PlanView;				// ��鵵
	BOOL bSectionAA				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_SectionAA;			// �Ŵ��� �ܸ� ��
	long nDivType				=	bDefaultLoad ? 0	: pOpt->m_nTendon_DivType;				// ���� ���� ���
	BOOL bMatchLine				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_MatchLine;			// ��ġ����
//	BOOL bLineInfo				=	bDefaultLoad ? TRUE : pOpt->m_bTendon_bPyungLineInfo;		// ��������

//	BOOL bDimsionTable			=	bDefaultLoad ? TRUE : pOpt->m_bSlab_DimTable;		// ġ����ȭ ���̺�
//	long nDimsionTableType		=	bDefaultLoad ? 1	: pOpt->m_nSlab_DimTableType;	// ġ����ȭ ���̺� Ÿ�� 
//	BOOL bDimsionTableFirst		=	bDefaultLoad ? FALSE : pOpt->m_bSlab_DimTableFirstPage;	// ù�������� ǥ��

	m_Options.SetOptionItem("���� ǥ��", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("PLANVIEW", bPlanView ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("PLANVIEW", "\t��鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("SECTIONAA", bSectionAA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SECTIONAA", "\t�ܸ�A-A ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOption("MATCHLINE", bMatchLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("MATCHLINE", "\t��ġ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

//		m_Options.SetOption("DIMTABLE", bDimsionTable? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//		m_Options.SetOptionItem("DIMTABLE", "\tġ����ȭ ���̺�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem("������� ���", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("DIVTYPE", nDivType);
		m_Options.SetOptionItem("DIVTYPE","\t�氣 ������ ����", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("DIVTYPE","\t��ü ������ ��������", 3, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);
}

void COptionTendon::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		pOpt->m_dTendon_ScaleGen       = 100;	
		pOpt->m_dTendon_ScaleSectionAA = 30;
		pOpt->m_nTendon_DivNum         = 1;		
//		pOpt->m_dSlab_DimTableScale = 100;
//		pOpt->m_dSlab_DimTableHeightFactor = 1;
//		pOpt->m_dSlab_DimTableWidhtFactor = 1;
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(4);
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
	m_Grid.SetTextMatrix(1,0,"��鵵",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"�ܸ� A-A��",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(3,0,"��´��� ����",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(3,0,"��ȭġ�� ���̺�",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(4,0,"��ȭġ�� ���̺� ���̰��",(UINT)DT_LEFT);
//	m_Grid.SetTextMatrix(5,0,"��ȭġ�� ���̺� �����",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"������");
	m_Grid.SetTextMatrix(1,1,"%.1f",pOpt->m_dTendon_ScaleGen,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2,1,"%.1f",pOpt->m_dTendon_ScaleSectionAA,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3,1,"%.0f",pOpt->m_nSlab_DivNum,(UINT)DT_RIGHT);
//	m_Grid.SetTextMatrix(3,1,"%.1f",pOpt->m_dSlab_DimTableScale,(UINT)DT_RIGHT);	
//	m_Grid.SetTextMatrix(4,1,"%.1f",pOpt->m_dSlab_DimTableHeightFactor,(UINT)DT_RIGHT);	
//	m_Grid.SetTextMatrix(5,1,"%.1f",pOpt->m_dSlab_DimTableWidhtFactor,(UINT)DT_RIGHT);	
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionTendon::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
}

void COptionTendon::OnOK() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_bTendon_PlanView			= m_Options.GetOption("PLANVIEW");	// ��鵵
	pOpt->m_bTendon_SectionAA			= m_Options.GetOption("SECTIONAA");		// �Ŵ��� �ܸ� ��
	pOpt->m_nTendon_DivType				= m_Options.GetOption("DIVTYPE");		// ���� ���� ���
	pOpt->m_bTendon_MatchLine			= m_Options.GetOption("MATCHLINE");
//	pOpt->m_bSlab_DimTable				= m_Options.GetOption("DIMTABLE");				// ġ����ȭ ���̺�

	pOpt->m_dTendon_ScaleGen			= m_Grid.GetTextMatrixDouble(1,1);
	pOpt->m_dTendon_ScaleSectionAA		= m_Grid.GetTextMatrixDouble(2,1);
	pOpt->m_nTendon_DivNum              = m_Grid.GetTextMatrixLong(3,1);

//	pOpt->m_dSlab_DimTableScale			= m_Grid.GetTextMatrixDouble(3,1);
//	pOpt->m_dSlab_DimTableHeightFactor	= m_Grid.GetTextMatrixDouble(4,1);
//	pOpt->m_dSlab_DimTableWidhtFactor	= m_Grid.GetTextMatrixDouble(5,1);

	pStd->GetDrawPageMng()->SetModified(PAGE_TENDON);

	CPropertyPage::OnOK();
}

BOOL COptionTendon::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_TENDON;
	
	return CPropertyPage::OnSetActive();
}


// OptionStd�� �ʱⰪ�� ��ġ���Ѿ���....
void COptionTendon::OnDefault()
{
	m_ListOption.DeleteAllItems();

	InitTreeData(TRUE);
	InitGridData(TRUE);
}
