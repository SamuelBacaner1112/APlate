// OptionCrossBeam.cpp: implementation of the COptionCrossBeam class.
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
IMPLEMENT_DYNCREATE(COptionCrossBeam, CPropertyPage)

COptionCrossBeam::COptionCrossBeam() : CPropertyPage(COptionCrossBeam::IDD)
{
	//{{AFX_DATA_INIT(COptionCrossBeam)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
} 

COptionCrossBeam::~COptionCrossBeam()
{
}

void COptionCrossBeam::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionCrossBeam)
	DDX_Control(pDX, IDC_OPTION_TREE, m_ListOption);
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionCrossBeam, CPropertyPage)
	//{{AFX_MSG_MAP(COptionCrossBeam)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionCrossBeam message handlers
void COptionCrossBeam::InitTreeData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	BOOL bCross_Summary					= bDefaultLoad ? TRUE : pOpt->m_Cross_Summary;
	BOOL bCross_Pyung					= bDefaultLoad ? TRUE : pOpt->m_Cross_Pyung;
	BOOL bCross_Table					= bDefaultLoad ? TRUE : pOpt->m_Cross_Table;
	BOOL bCross_Plan					= bDefaultLoad ? TRUE : pOpt->m_Cross_Plan;
	//
	BOOL bCross_PlanUpper				= bDefaultLoad ? TRUE : pOpt->m_Cross_PlanUpper;
	BOOL bCross_PlanLower				= bDefaultLoad ? TRUE : pOpt->m_Cross_PlanLower;
	BOOL bCross_Sangse					= bDefaultLoad ? TRUE : pOpt->m_Cross_Sangse;
	//
	BOOL bCross_Front					= bDefaultLoad ? TRUE : pOpt->m_Cross_Front;
	BOOL bCross_GassetUpperDetail		= bDefaultLoad ? TRUE : pOpt->m_Cross_GassetUpperDetail;
	BOOL bCross_GassetLowerDetail		= bDefaultLoad ? TRUE : pOpt->m_Cross_GassetLowerDetail;
	BOOL bCross_GassetSupportDetail		= bDefaultLoad ? TRUE : pOpt->m_Cross_GassetSupportDetail;
	BOOL bCross_VStiffDetail			= bDefaultLoad ? TRUE : pOpt->m_Cross_VStiffDetail;
	BOOL bCross_HStiffDetail			= bDefaultLoad ? TRUE : pOpt->m_Cross_HStiffDetail;
	BOOL bCross_AnchorBarDetail			= bDefaultLoad ? TRUE : pOpt->m_Cross_AnchorBarDetail;
	BOOL bCross_GassetScallopDetail		= bDefaultLoad ? TRUE : pOpt->m_Cross_GassetScallopDetail;
	BOOL bCross_BKSupport				= bDefaultLoad ? TRUE : pOpt->m_Cross_BKSupport;
	BOOL bCross_BM						= bDefaultLoad ? TRUE : pOpt->m_Cross_BM;
//	BOOL bCross_SpliceDetail            = bDefaultLoad ? TRUE : pOpt->m_Cross_SpliceDetail;
//	BOOL bCross_DrawDiap				= bDefaultLoad ? TRUE : pOpt->m_Cross_DrawDiap;

	BOOL bCross_AllGirder				= bDefaultLoad ? FALSE : pOpt->m_Cross_AllGirder;
	BOOL bCross_TwoDrawing				= bDefaultLoad ? FALSE : pOpt->m_Cross_TwoDrawing;
	BOOL bCross_KeyPlan					= bDefaultLoad ? TRUE  : pOpt->m_Cross_KeyPlan;
	BOOL bCross_KeyPlanRoVer			= bDefaultLoad ? FALSE : pOpt->m_Cross_KeyPlanRoVer;
	BOOL bCross_KeyPlanSpliceMark		= bDefaultLoad ? TRUE  : pOpt->m_Cross_KeyPlanSpliceMark;
	BOOL bCross_KeyPlanJijumMark		= bDefaultLoad ? TRUE  : pOpt->m_Cross_KeyPlanJijumMark;
	BOOL bCross_KeyPlanCrossBeam		= bDefaultLoad ? TRUE  : pOpt->m_Cross_KeyPlanCrossBeam;
	BOOL bCross_KeyPlanDimension		= bDefaultLoad ? TRUE  : pOpt->m_Cross_KeyPlanDimension;
	BOOL bCross_KeyPlanDirection		= bDefaultLoad ? TRUE  : pOpt->m_Cross_KeyPlanDirection;
	BOOL bCross_KeyPlanOneLine			= bDefaultLoad ? TRUE  : pOpt->m_Cross_KeyPlanOneLine;

	BOOL bCross_bSpliceMark				= bDefaultLoad ? TRUE  : pOpt->m_Cross_bSpliceMark;
	BOOL bCross_bJiJumMark				= bDefaultLoad ? TRUE  : pOpt->m_Cross_bJiJumMark;
	BOOL bCross_bCrossBeam				= bDefaultLoad ? TRUE  : pOpt->m_Cross_bCrossBeam;
	BOOL bCross_bDimension				= bDefaultLoad ? TRUE : pOpt->m_Cross_bDimension;
	BOOL bCross_bDirection				= bDefaultLoad ? TRUE : pOpt->m_Cross_bDirection;
	BOOL bCross_bOneline				= bDefaultLoad ? TRUE : pOpt->m_Cross_bOneline;
	BOOL bCross_bLineInfo				= bDefaultLoad ? TRUE : pOpt->m_Cross_bLineInfo;	
	long Cross_nType                    = bDefaultLoad ? 1 : pOpt->m_Cross_Type;	
	//
m_Options.SetOptionItem("��µ��� ����", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption("Cross_nType", (bDefaultLoad ? 1 : pOpt->m_Cross_Type));
	/*m_Options.SetOptionItem("Cross_nType", "\t\t���κ��� Ÿ��", 0, IDB_RADIOON, IDB_RADIOOFF);	
		m_Options.SetOption("Cross_Front", bCross_Front ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_Front", "\t\t���鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_Plan", bCross_Plan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_Plan", "\t\t��鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_GassetUpperDetail", bCross_GassetUpperDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_GassetUpperDetail", "\t\t����� ��� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_GassetLowerDetail", bCross_GassetLowerDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_GassetLowerDetail", "\t\t����� �Ϻ� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_GassetSupportDetail", bCross_GassetSupportDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_GassetSupportDetail", "\t\t����� ������ ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_VStiffDetail", bCross_VStiffDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_VStiffDetail", "\t\t���������� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_HStiffDetail", bCross_HStiffDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_HStiffDetail", "\t\t���򺸰��� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_AnchorBarDetail", bCross_AnchorBarDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_AnchorBarDetail", "\t\t������ ��Ŀ ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_GassetScallopDetail", bCross_GassetScallopDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_GassetScallopDetail", "\t\t����� ��Ķ�� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_BKSupport", bCross_BKSupport ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_BKSupport", "\t\t����� ������ ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_BM", bCross_BM ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_BM", "\t\t���ǥ", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	*/

	m_Options.SetOption("Cross_nType", (bDefaultLoad ? 1 : pOpt->m_Cross_Type));
	m_Options.SetOptionItem("Cross_nType", "\t�ܸ麰 Ÿ��", 1, IDB_RADIOON, IDB_RADIOOFF);		
		m_Options.SetOption("Cross_Front", bCross_Front ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_Front", "\t\t���鵵", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_PlanUpper", bCross_PlanUpper ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_PlanUpper", "\t\t��鵵 ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_PlanLower", bCross_PlanLower ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_PlanLower", "\t\t��鵵 �Ϻ�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_Sangse", bCross_Sangse ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_Sangse", "\t\t����� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
//		m_Options.SetOption("Cross_SpliceDetail", bCross_SpliceDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
//		m_Options.SetOptionItem("Cross_SpliceDetail", "\t\t���������� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_GassetUpperDetail", bCross_GassetUpperDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_GassetUpperDetail", "\t\t����� ��� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_GassetLowerDetail", bCross_GassetLowerDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_GassetLowerDetail", "\t\t����� �Ϻ� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_BKSupport", bCross_BKSupport ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_BKSupport", "\t\t����� ������ ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_VStiffDetail", bCross_VStiffDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_VStiffDetail", "\t\t���������� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_HStiffDetail", bCross_HStiffDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_HStiffDetail", "\t\t���򺸰��� ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_AnchorBarDetail", bCross_AnchorBarDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_AnchorBarDetail", "\t\t������ ��Ŀ ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_BM", bCross_BM ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_BM", "\t\t���ǥ", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	
	
	m_Options.SetOptionItem("��� �ɼ�", IDB_ITEMS, IDB_ITEMS);	
		m_Options.SetOption("Cross_AllGirder", bCross_AllGirder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_AllGirder", "\t��ü �Ŵ� ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_TwoDrawing", bCross_TwoDrawing ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_TwoDrawing", "\t�󼼵� 2������ ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("Cross_KeyPlan", bCross_KeyPlan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("Cross_KeyPlan", "\t��൵ ���", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_SPLICE", bCross_KeyPlanSpliceMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_SPLICE", "\t\t�������� ��ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_JIJUMMARK", bCross_KeyPlanJijumMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_JIJUMMARK", "\t\t������ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_CROSS", bCross_KeyPlanCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_CROSS", "\t\t���κ� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_KEYDIMENSION", bCross_KeyPlanDimension ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_KEYDIMENSION", "\t\tġ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_DIRECTION", bCross_KeyPlanDirection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_DIRECTION", "\t\t������� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_KEYPLAN_ONELINE", bCross_KeyPlanOneLine ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_KEYPLAN_ONELINE", "\t\t�����Ѽ����� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_KEYPLAN_ROVER", bCross_KeyPlanRoVer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_KEYPLAN_ROVER", "\t\t��൵ ���� ȸ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);			

	m_Options.SetOptionItem("���κ� ġ��ǥ ���", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("Cross_Summary", bCross_Summary ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_Summary", "\t���κ� ��� ġ����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("Cross_Pyung", bCross_Pyung ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem("Cross_Pyung", "\t\t��ġ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
				m_Options.SetOption("Cross_Table", bCross_Table ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem("Cross_Table", "\t\tġ�����̺�", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	m_Options.SetOptionItem("ġ��ǥ ��¿ɼ�", IDB_ITEMS, IDB_ITEMS);	
			m_Options.SetOption("Cross_bSplice", bCross_bSpliceMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_bSplice", "\t�������� ��ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_bJiJumMark", bCross_bJiJumMark ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_bJiJumMark", "\t������ũ ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_bCross", bCross_bCrossBeam ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_bCross", "\t���κ� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_bDimension", bCross_bDimension ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_bDimension", "\tġ���� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_bDirection", bCross_bDirection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_bDirection", "\t������� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_bOneline", bCross_bOneline ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_bOneline", "\t���� �Ѽ����� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
			m_Options.SetOption("Cross_bLineInfo", bCross_bLineInfo ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem("Cross_bLineInfo", "\t�������� ǥ��", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();

	m_ListOption.Initialize(NULL);
}

void COptionCrossBeam::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{
		// Add the Properties
		long i=0;
		pOpt->m_CrossBeam_Scale.RemoveAll();
		pOpt->m_CrossBeam_Scale.Add(30);// ���鵵 
		
		pOpt->m_CrossBeam_Scale.Add(30);// ��鵵���
		pOpt->m_CrossBeam_Scale.Add(30);// ��鵵�Ϻ�
		pOpt->m_CrossBeam_Scale.Add(10);// �ܸ麰 Ÿ�� ��
		pOpt->m_CrossBeam_Scale.Add(10);// ���������� �� - ������
		pOpt->m_CrossBeam_Scale.Add(5);// �ܸ麰 Ÿ�� ����� ��
	
		pOpt->m_CrossBeam_Scale.Add(10);// ����� ���
		pOpt->m_CrossBeam_Scale.Add(10);// ����� �Ϻ�
		pOpt->m_CrossBeam_Scale.Add(10);// ����� ������
		pOpt->m_CrossBeam_Scale.Add(10);// ����������
		pOpt->m_CrossBeam_Scale.Add(10);// ���򺸰���
		pOpt->m_CrossBeam_Scale.Add(10);// ��Ŀ�� 
		pOpt->m_CrossBeam_Scale.Add(10);// ����� ������ �� 
		pOpt->m_CrossBeam_Scale.Add(5);// ����� ���̷�
		pOpt->m_CrossBeam_Scale.Add(500);// ��൵ ����
		pOpt->m_CrossBeam_Scale.Add(500);// ��൵ ����

		pOpt->m_Cross_nRowSu = 20;// ���̺� �� ��
		pOpt->m_Cross_dPosHScale = 200; // ���κ�ġ��ǥ ����
		pOpt->m_Cross_dPosVScale = 200; // ���κ�ġ��ǥ ����
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(20);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);	
	m_Grid.SetRowHeight(5, 0);
	m_Grid.SetColumnWidth(0,Rect.Width()*2/3);
	m_Grid.SetColumnWidth(1,Rect.Width()*1/3);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));	
	m_Grid.SetTextBkColor(RGB(255,255,255));

	m_Grid.SetTextMatrix(0,0,"�����׸�");
	m_Grid.SetTextMatrix(1,0,"���鵵",(UINT)DT_LEFT);
	
	m_Grid.SetTextMatrix(2,0,"��鵵 ���",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(3,0,"��鵵 �Ϻ�",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"�ܸ麰 Ÿ�� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"���������� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(6,0,"�ܸ麰 Ÿ�� ����� �� ",(UINT)DT_LEFT);

	m_Grid.SetTextMatrix(7,0,"����� ��� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(8,0,"����� �Ϻ� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(9,0,"����� ������",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(10,0,"���������� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(11,0,"���򺸰��� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(12,0,"������ ��Ŀ ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(13,0,"����� ������ ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(14,0,"����� ���̷� ��",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(15,0,"KEY PLAN : ����",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(16,0,"KEY PLAN : ����",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(17,0,"���κ�ġ��ǥ ��鵵 ����",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(18,0,"���κ�ġ��ǥ ��鵵 ����",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(19,0,"���κ�ġ��ǥ ���̺�� ����",(UINT)DT_LEFT);
		
	m_Grid.SetTextMatrix(0,1,"������");
	for(long i=0; i<pOpt->m_CrossBeam_Scale.GetSize(); i++)
		m_Grid.SetTextMatrix(i+1,1,"%.1f",pOpt->m_CrossBeam_Scale.GetAt(i),(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(17,1,"%.0f",pOpt->m_Cross_dPosHScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(18,1,"%.0f",pOpt->m_Cross_dPosVScale,(UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(19,1,"%.0f",pOpt->m_Cross_nRowSu,(UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE,TRUE);	
}

BOOL COptionCrossBeam::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTreeData(FALSE);
	InitGridData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionCrossBeam::OnDestroy() 
{
	CPropertyPage::OnDestroy();			
}

void COptionCrossBeam::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;
	

	pOpt->m_Cross_Summary				= m_Options.GetOption("Cross_Summary");
	pOpt->m_Cross_Pyung					= m_Options.GetOption("Cross_Pyung");
	pOpt->m_Cross_Table					= m_Options.GetOption("Cross_Table");
	//pOpt->m_Cross_Front					= m_Options.GetOption("Cross_Front");
	//pOpt->m_Cross_Plan					= m_Options.GetOption("Cross_Plan");

	pOpt->m_Cross_PlanUpper				= m_Options.GetOption("Cross_PlanUpper");
	pOpt->m_Cross_PlanLower				= m_Options.GetOption("Cross_PlanLower");
	pOpt->m_Cross_Sangse				= m_Options.GetOption("Cross_Sangse");

	//pOpt->m_Cross_GassetUpperDetail		= m_Options.GetOption("Cross_GassetUpperDetail");
	//pOpt->m_Cross_GassetLowerDetail		= m_Options.GetOption("Cross_GassetLowerDetail");
	//pOpt->m_Cross_GassetSupportDetail	= m_Options.GetOption("Cross_GassetSupportDetail");	
	//pOpt->m_Cross_VStiffDetail			= m_Options.GetOption("Cross_VStiffDetail");
	//pOpt->m_Cross_HStiffDetail			= m_Options.GetOption("Cross_HStiffDetail");
	//pOpt->m_Cross_AnchorBarDetail		= m_Options.GetOption("Cross_AnchorBarDetail");
	//pOpt->m_Cross_GassetScallopDetail	= m_Options.GetOption("Cross_GassetScallopDetail");
	//pOpt->m_Cross_BM					= m_Options.GetOption("Cross_BM");	
	//pOpt->m_Cross_BKSupport				= m_Options.GetOption("Cross_BKSupport");
	pOpt->m_Cross_AllGirder				= m_Options.GetOption("Cross_AllGirder");
	pOpt->m_Cross_TwoDrawing			= m_Options.GetOption("Cross_TwoDrawing");

	pOpt->m_Cross_KeyPlanRoVer			= m_Options.GetOption("Cross_KEYPLAN_ROVER");
	pOpt->m_Cross_KeyPlanSpliceMark		= m_Options.GetOption("Cross_SPLICE");
	pOpt->m_Cross_KeyPlanJijumMark		= m_Options.GetOption("Cross_JIJUMMARK");
	pOpt->m_Cross_KeyPlanCrossBeam		= m_Options.GetOption("Cross_CROSS");
	pOpt->m_Cross_KeyPlanDimension		= m_Options.GetOption("Cross_KEYDIMENSION");
	pOpt->m_Cross_KeyPlanDirection		= m_Options.GetOption("Cross_DIRECTION");
	pOpt->m_Cross_KeyPlanOneLine		= m_Options.GetOption("Cross_KEYPLAN_ONELINE");

	pOpt->m_Cross_Type       			= m_Options.GetOption("Cross_nType");
	pOpt->m_Cross_SpliceDetail          = FALSE;//m_Options.GetOption("Cross_SpliceDetail");
	
	pOpt->m_Cross_bSpliceMark	= m_Options.GetOption("Cross_bSplice");
	pOpt->m_Cross_bJiJumMark	= m_Options.GetOption("Cross_bJiJumMark");
	pOpt->m_Cross_bCrossBeam	= m_Options.GetOption("Cross_bCross");
	pOpt->m_Cross_bDimension	= m_Options.GetOption("Cross_bDimension");
	pOpt->m_Cross_bDirection	= m_Options.GetOption("Cross_bDirection");
	pOpt->m_Cross_bOneline		= m_Options.GetOption("Cross_bOneline");
	pOpt->m_Cross_bLineInfo		= m_Options.GetOption("Cross_bLineInfo");


	double Scale = 0;
	long nSize = pOpt->m_CrossBeam_Scale.GetSize();
	pOpt->m_CrossBeam_Scale.RemoveAll();
	for(long i=1; i<nSize+1; i++)
	{
		Scale = m_Grid.GetTextMatrixDouble(i,1);
		pOpt->m_CrossBeam_Scale.Add(Scale);
	}	
	
	pOpt->m_Cross_dPosHScale = m_Grid.GetTextMatrixLong(17,1);
	pOpt->m_Cross_dPosVScale = m_Grid.GetTextMatrixLong(18,1);
	pOpt->m_Cross_nRowSu = m_Grid.GetTextMatrixLong(19,1);
	pStd->GetDrawPageMng()->SetModified(PAGE_HGDETAIL);	
	CPropertyPage::OnOK();
}

void COptionCrossBeam::OnDefault()
{
	m_ListOption.DeleteAllItems();
	InitTreeData(TRUE);
	InitGridData(TRUE);
}

BOOL COptionCrossBeam::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd *pStd = m_pPlateDrawStd;

	pStd->m_pDomOptionStd->m_nOptionCur = OPTION_PAGE_AROAD_CROSS;

	return CPropertyPage::OnSetActive();
}