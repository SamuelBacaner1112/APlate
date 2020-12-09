// DesignDeductPage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DesignDeductPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SHGRIDTITLE CDesignDeductPage::m_gt[ROW_COUNT_DEDUCT] = 
	{		
		{"�� ��",											"%s"	,350,	DT_CENTER,	TRUE},		
		{"���� (�米�� ��쿡�� ����)",						"%s"	,350,	DT_LEFT,	FALSE},
		{"���� (�米�� ��쿡�� ����)",						"%s"	,350,	DT_LEFT,	FALSE},
		{"������ (���ܸ�, ������ ���� ���)",				"%s"	,350,	DT_LEFT,	FALSE},
		{"���򺸰���",										"%s"	,350,	DT_LEFT,	FALSE},
		{"����������",										"%s"	,350,	DT_LEFT,	FALSE},
		{"���κ�",											"%s"	,350,	DT_LEFT,	FALSE},
		{"���κ� ����� ������",							"%s"	,350,	DT_LEFT,	FALSE},		
		{"���κ� ����� ��.����",							"%s"	,350,	DT_LEFT,	FALSE},
		{"���κ� ����� ������",							"%s"	,350,	DT_LEFT,	FALSE},
		{"���κ� ����� ������",							"%s"	,350,	DT_LEFT,	FALSE},				
		{"���������� ���ٸ�",								"%s"	,350,	DT_LEFT,	FALSE},				
		{"�Ϲ� ��Ķ�� ����",								"%s"	,350,	DT_LEFT,	FALSE},		
		{"����� ����",										"%s"	,350,	DT_LEFT,	FALSE},		
		{"��ƮȦ ����",										"%s"	,350,	DT_LEFT,	FALSE},		
		{"�������� ���� (���Ʈ �������� ��쿡�� ����)",	"%s"	,350,	DT_LEFT,	FALSE},	
		{"���÷���Ʈ ����",									"%s"	,350,	DT_LEFT,	FALSE}
	};

/////////////////////////////////////////////////////////////////////////////
// CDesignDeductPage dialog


CDesignDeductPage::CDesignDeductPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignDeductPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignDeductPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;

	m_strCombo.Add("���� ����");
	m_strCombo.Add("����");	
}


void CDesignDeductPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignDeductPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignDeductPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignDeductPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignDeductPage message handlers
void CDesignDeductPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

BOOL CDesignDeductPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignDeductPage::SetDataDefault()
{
	m_pDataManage->GetSteelDeduct()->InitData();	
	SetDataInit();
}

void CDesignDeductPage::SetGridTitle()
{
	m_Grid.SetComboString(m_strCombo);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(ROW_COUNT_DEDUCT);
	m_Grid.SetColumnCount(2);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);				
	m_Grid.SetColumnWidth(1,150);
	m_Grid.InputShGridTitle(m_gt, ROW_COUNT_DEDUCT, FALSE, FALSE);	
	m_Grid.SetRowHeightAll(20);				
	m_Grid.SetRowHeight(0, 20);	
	m_Grid.SetRowHeight(4, 0);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
}

void CDesignDeductPage::SetDataInit()
{
	SetGridTitle();

	CSteelDeduct* pDdt = m_pDataManage->GetSteelDeduct();	

	m_Grid.SetGridData(m_gt, 0,	1,"������");	
	m_Grid.SetGridData(m_gt, 1,	1,pDdt->m_bDeductUpperFlange				? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 2,	1,pDdt->m_bDeductLowerFlange				? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 3,	1,pDdt->m_bDeductWeb						? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 4,	1,pDdt->m_bDeductHStiff						? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 5,	1,pDdt->m_bDeductVerStiff					? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 6,	1,pDdt->m_bDeductCrossBeamWebPlate			? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 7, 1,pDdt->m_bDeductCrossBeamBracketWebPlate	? "����" : "���� ����");																		
	m_Grid.SetGridData(m_gt, 8, 1,pDdt->m_bDeductCrossBeamBracketUDFlange	? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 9, 1,pDdt->m_bDeductCrossBeamBracketStiffner	? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 10,1,pDdt->m_bDeductCrossBeamBracketStiffPlate ? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 11,1,pDdt->m_bDeductDongBari					? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 12,1,pDdt->m_bDeductGeneralScallop				? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 13,1,pDdt->m_bDeductEdgeCut					? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 14,1,pDdt->m_bDeductBoltHole					? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 15,1,pDdt->m_bDeductSplice						? "����" : "���� ����");
	m_Grid.SetGridData(m_gt, 16,1,pDdt->m_bDeductSolePlate					? "����" : "���� ����");
																									
	for(long i=1;i<ROW_COUNT_DEDUCT;i++) m_Grid.SetCellType(i,1,CT_COMBO);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignDeductPage::SetDataSave()
{
	CSteelDeduct* pDdt = m_pDataManage->GetSteelDeduct();	
	
	long nSeq = 1;

	pDdt->m_bDeductUpperFlange	= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 1. ����
	pDdt->m_bDeductLowerFlange	= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 2. ����
	pDdt->m_bDeductWeb			= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 3. ������ ����	
	pDdt->m_bDeductHStiff		= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 5. ���򺸰���
	pDdt->m_bDeductVerStiff		= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 6. ����������
	pDdt->m_bDeductCrossBeamWebPlate			= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 9. ����� ������
	pDdt->m_bDeductCrossBeamBracketWebPlate		= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 10. ���κ� ����� ������
	pDdt->m_bDeductCrossBeamBracketUDFlange		= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 11. ����� ������
	pDdt->m_bDeductCrossBeamBracketStiffner		= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 12. ����� ������
	pDdt->m_bDeductCrossBeamBracketStiffPlate	= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 13. ����� ������	
	pDdt->m_bDeductDongBari						= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 18. ���������� ���ٸ�
	pDdt->m_bDeductGeneralScallop				= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 20. �Ϲ� ��Ķ��
	pDdt->m_bDeductEdgeCut						= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 21. ����� ����
	pDdt->m_bDeductBoltHole						= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 22. ��ƮȦ ����
	pDdt->m_bDeductSplice						= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 23. �������� ����
	pDdt->m_bDeductSolePlate					= m_Grid.GetItemText(nSeq++, 1) == "����" ? 1 : 0;	// 24. ���÷���Ʈ ����
}

BOOL CDesignDeductPage::PreTranslateMessage(MSG* pMsg) 
{
	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return TStackedPage::PreTranslateMessage(pMsg);
}
	
void CDesignDeductPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();

	return;
}
