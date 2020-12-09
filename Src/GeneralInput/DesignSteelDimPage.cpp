//DesignSteelDimPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignSteelDimPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignSteelDimPage dialog
// SHGRIDTITLE CDesignSteelDimPage::m_gt[COLUMN_COUNT_STEEL] = 
// 	{	
// 		{"\n�԰�",										"%s",		 80,	DT_CENTER,	TRUE},
// 		{"\n���� ��ȣ",									"%s",		130,	DT_LEFT,	TRUE},
// 		{"������� ����\n40����\n(MPa)",			"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"������� ����\n40-75����\n(MPa)",		"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"������� ����\n75-100����\n(MPa)",		"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�������\n\n���°��",						"%-.2lf",	 70,	DT_RIGHT,	FALSE},
// 		{"��Ʈ����\n\n��      ��",						"%-.2lf",	 70,	DT_RIGHT,	FALSE},
// 		{"������� ����\n40����\n(MPa)",			"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"������� ����\n40-75����\n(MPa)",		"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"������� ����\n75-100����\n(MPa)",		"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�������\n\n���°��",						"%-.2lf",	 70,	DT_RIGHT,	FALSE},
// 		{"�ռ�����\n\n��      ��",						"%-.2lf",	 70,	DT_RIGHT,	FALSE},
// 		{"��������\n\n(kg/m��)",						"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�����\n�β�\n(mm)",							"%-.2lf",	 70,	DT_RIGHT,	FALSE},
// 		{"\n������ũ",									"%s",		 60,	DT_CENTER,	FALSE},// 10
// 		{"\n����â���",								"%-.6lf",	 90,	DT_RIGHT,	TRUE},
// 		{"ź�����\n\n(MPa)",						"%-.0lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�µ�\n\n(��)",								"%-.2lf",	 50,	DT_RIGHT,	TRUE},
// 		{"�������\n\n�ּҰ��",						"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�������\n\n�ִ���",						"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�������\n���°��\n(MPa)",				"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"��������\n�������� 40����\n(MPa)",		"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"��������\n�������� 40-75����\n(MPa)",	"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"��������\n�������� 75-100����\n(MPa)",	"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�������\n�����׺��� 40����\n(MPa)",		"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�������\n�����׺��� 40-75����\n(MPa)",	"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�������\n�����׺��� 75-100����\n(MPa)",	"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�������\n\n������",							"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"�������\n�� �� 40����\n(MPa)",			"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�������\n�� �� 40-75����\n(MPa)",		"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�������\n�� �� 40-100����\n(MPa)",		"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�������\n\n���°��",						"%-.2lf",	 70,	DT_RIGHT,	FALSE},
// 		{"�׺���\n16����\n(N/mm��)",					"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�׺���\n16-4����0\n(N/mm��)",				"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�׺���\n40-75����\n(N/mm��)",				"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"�׺���\n75�ʰ�\n(N/mm��)",					"%-.2lf",	 90,	DT_RIGHT,	TRUE},
// 		{"���尭��\n\n(N/mm��)",						"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"\n11,12ȣ����\n",								"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"\n5ȣ����\n",									"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"\n��������",									"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"\n���ʹ�����",								"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"\n�������",									"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 		{"���̴��߷�\n\n(N/m)",						"%-.2lf",	 70,	DT_RIGHT,	TRUE},
// 	};	

CDesignSteelDimPage::CDesignSteelDimPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignSteelDimPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignSteelDimPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignSteelDimPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignSteelDimPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignSteelDimPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignSteelDimPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignSteelDimPage message handlers
void CDesignSteelDimPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignSteelDimPage::SetGridTitle()
{
	CSteel			*pSteel	= m_pDataManage->GetSteel();
	if(!GetSafeHwnd()) return;
	if(!pSteel) return;
	long nSize = pSteel->GetSize();

	// �׸��� �⺻ ���� 
	m_Grid.ResetAttrib(TRUE);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nSize + 4);
	m_Grid.SetColumnCount(103);
	m_Grid.SetFixedRowCount(4);	
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetTextBkColor(RGBEDITABLE);
	m_Grid.EnableTitleTips(FALSE);

	m_Grid.SetColumnWidthAll(120);

	CString szText = _T("");
	
	szText.Format("���� ��ȣ");		m_Grid.SetTextMatrix(0,	0,	szText);
		szText.Format("�԰�");		m_Grid.SetTextMatrix(2, 0,	szText);
		szText.Format("����");		m_Grid.SetTextMatrix(2, 1,	szText);
	
	m_Grid.SetMergeCol(0, 0, 1, TRUE);m_Grid.SetMergeCol(1, 0, 1, TRUE);
	m_Grid.SetMergeRow(0, 1, 0, TRUE);m_Grid.SetMergeRow(0, 1, 1, TRUE);
	m_Grid.SetMergeRow(2, 3, 0, TRUE);m_Grid.SetMergeRow(2, 3, 1, TRUE);
	m_Grid.SetColumnWidth(0, 90);
	m_Grid.SetColumnWidth(1, 100);

	szText.Format("����ǵβ�(mm)");			m_Grid.SetTextMatrix(0,  2, szText);
	szText.Format("��.��.��ǥ 3.2.3(3.9.5)");	m_Grid.SetTextMatrix(1,  2, szText);
	m_Grid.SetMergeRow(2, 3, 2, TRUE);
	m_Grid.SetColumnWidth(2, 140);

	szText.Format("�ǵβ��� ���� ���� �׺���(MPa)");	m_Grid.SetTextMatrix(0,  3, szText);
	szText.Format("��.��.��ǥ 3.2.3(3.9.5)");			m_Grid.SetTextMatrix(1,	 3, szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	3,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	4,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 5,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 6,	szText);
	m_Grid.SetMergeCol(0, 3, 6, TRUE);m_Grid.SetMergeCol(1,	3, 6, TRUE);
	m_Grid.SetMergeRow(2, 3, 3, TRUE);m_Grid.SetMergeRow(2, 3, 4, TRUE);
	m_Grid.SetMergeRow(2, 3, 5, TRUE);m_Grid.SetMergeRow(2, 3, 6, TRUE);
	long nCol = 0;
	for(nCol = 3; nCol <= 6; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);
	
	szText.Format("����������� �� ��������(MPa)");	m_Grid.SetTextMatrix(0,  7, szText);
	szText.Format("��.��.��ǥ 3.3.1(3.3.4(a))");	m_Grid.SetTextMatrix(1,	 7,	szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	7,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	8,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 9,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 10,	szText);
	m_Grid.SetMergeCol(0, 7, 10, TRUE);m_Grid.SetMergeCol(1, 7, 10, TRUE);
	m_Grid.SetMergeRow(2, 3, 7, TRUE);m_Grid.SetMergeRow(2, 3, 8, TRUE);
	m_Grid.SetMergeRow(2, 3, 9, TRUE);m_Grid.SetMergeRow(2, 3, 10, TRUE);
	for(nCol = 7; nCol <= 10; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);
	
	szText.Format("�����±��� ������� ���� ���������������(MPa)");	m_Grid.SetTextMatrix(0,	11,	szText);
	szText.Format("��.��.��ǥ 3.3.2");									m_Grid.SetTextMatrix(1,	11,	szText);
		szText.Format("T��40");	m_Grid.SetTextMatrix(2,	11,	szText);
		m_Grid.SetMergeCol(2, 11, 14, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 11,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 12,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 13,	szText);
			szText.Format("a4");	m_Grid.SetTextMatrix(3, 14,	szText);
		szText.Format("40<T��75");	m_Grid.SetTextMatrix(2,	15,	szText);
		m_Grid.SetMergeCol(2, 15, 18, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 15,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 16,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 17,	szText);
			szText.Format("a4");	m_Grid.SetTextMatrix(3, 18,	szText);
		szText.Format("75<T��100");	m_Grid.SetTextMatrix(2, 19,	szText);
		m_Grid.SetMergeCol(2, 19, 22, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 19,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 20,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 21,	szText);
			szText.Format("a4");	m_Grid.SetTextMatrix(3, 22,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 23,	szText);
		m_Grid.SetMergeCol(2, 23, 26, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 23,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 24,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 25,	szText);
			szText.Format("a4");	m_Grid.SetTextMatrix(3, 26,	szText);
	m_Grid.SetMergeCol(0, 11, 26, TRUE);m_Grid.SetMergeCol(1, 11, 26, TRUE);
	for(nCol = 11; nCol <= 26; nCol++)
		m_Grid.SetColumnWidth(nCol, 50);

	szText.Format("����ھ�������(MPa)");	m_Grid.SetTextMatrix(0,	27, szText);
	szText.Format("��.��.��ǥ 3.3.4(b)");	m_Grid.SetTextMatrix(1,	27,	szText);
		szText.Format("T��40(Aw/Ac��2)");	m_Grid.SetTextMatrix(2, 27, szText);
		m_Grid.SetMergeCol(2, 27, 29, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 27,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 28,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 29,	szText);
		szText.Format("40<T��75(Aw/Ac��2)");	m_Grid.SetTextMatrix(2, 30, szText);
		m_Grid.SetMergeCol(2, 30, 32, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 30,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 31,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 32,	szText);
		szText.Format("75<T��100(Aw/Ac��2)");	m_Grid.SetTextMatrix(2, 33, szText);
		m_Grid.SetMergeCol(2, 33, 35, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 33,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 34,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 35,	szText);
		szText.Format("T�����β�(Aw/Ac��2)");	m_Grid.SetTextMatrix(2, 36, szText);
		m_Grid.SetMergeCol(2, 36, 38, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 36,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 37,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 38,	szText);
		szText.Format("T��40(Aw/Ac>2)");	m_Grid.SetTextMatrix(2, 39, szText);
		m_Grid.SetMergeCol(2, 39, 41, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 39,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 40,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 41,	szText);
		szText.Format("40<T��75(Aw/Ac>2)");	m_Grid.SetTextMatrix(2, 42, szText);
		m_Grid.SetMergeCol(2, 42, 44, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 42,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 43,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 44,	szText);
		szText.Format("75<T��100(Aw/Ac>2)");	m_Grid.SetTextMatrix(2, 45, szText);
		m_Grid.SetMergeCol(2, 45, 47, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 45,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 46,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 47,	szText);
		szText.Format("T�����β�(Aw/Ac>2)");	m_Grid.SetTextMatrix(2, 48, szText);
		m_Grid.SetMergeCol(2, 48, 50, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 48,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 49,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 50,	szText);
	m_Grid.SetMergeCol(0, 27, 50, TRUE);m_Grid.SetMergeCol(1, 27, 50, TRUE);
	for(nCol = 27; nCol <= 50; nCol++)
		m_Grid.SetColumnWidth(nCol, 50);

	szText.Format("�����������(MPa)");	m_Grid.SetTextMatrix(0,	51, szText);
	szText.Format("��.��.��ǥ 3.3.5");	m_Grid.SetTextMatrix(1,	51,	szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	51,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	52,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 53,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 54,	szText);
	m_Grid.SetMergeCol(0, 51, 54, TRUE);m_Grid.SetMergeCol(1, 51, 54, TRUE);
	m_Grid.SetMergeRow(2, 3, 51, TRUE);m_Grid.SetMergeRow(2, 3, 52, TRUE);
	m_Grid.SetMergeRow(2, 3, 53, TRUE);m_Grid.SetMergeRow(2, 3, 54, TRUE);
	for(nCol = 51; nCol <= 54; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);

	szText.Format("�����������(MPa)");	m_Grid.SetTextMatrix(0, 55, szText);
	szText.Format("��.��.��ǥ 3.3.5");	m_Grid.SetTextMatrix(1,	55, szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	55,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	56,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 57,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 58,	szText);
	m_Grid.SetMergeCol(0, 55, 58, TRUE);m_Grid.SetMergeCol(1, 55, 58, TRUE);
	m_Grid.SetMergeRow(2, 3, 55, TRUE);m_Grid.SetMergeRow(2, 3, 56, TRUE);
	m_Grid.SetMergeRow(2, 3, 57, TRUE);m_Grid.SetMergeRow(2, 3, 58, TRUE);
	for(nCol = 55; nCol <= 58; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);

	szText.Format("������ �����������(MPa)");	m_Grid.SetTextMatrix(0, 59, szText);
	szText.Format("��.��.��ǥ 3.3.7");			m_Grid.SetTextMatrix(1,	59,	szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	59,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	60,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 61,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 62,	szText);
	m_Grid.SetMergeCol(0, 59, 62, TRUE);m_Grid.SetMergeCol(1, 59, 62, TRUE);
	m_Grid.SetMergeRow(2, 3, 59, TRUE);m_Grid.SetMergeRow(2, 3, 60, TRUE);
	m_Grid.SetMergeRow(2, 3, 61, TRUE);m_Grid.SetMergeRow(2, 3, 62, TRUE);
	for(nCol = 59; nCol <= 62; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);
	
	szText.Format("���������� ���� �翬�������� �ּ��ǵβ�(mm)");	m_Grid.SetTextMatrix(0, 63, szText);
	szText.Format("��.��.��ǥ 3.4.2");	m_Grid.SetTextMatrix(1,	63,	szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	63,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	64,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 65,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 66,	szText);
	m_Grid.SetMergeCol(0, 63, 66, TRUE);m_Grid.SetMergeCol(1, 63, 66, TRUE);
	m_Grid.SetMergeRow(2, 3, 63, TRUE);m_Grid.SetMergeRow(2, 3, 64, TRUE);
	m_Grid.SetMergeRow(2, 3, 65, TRUE);m_Grid.SetMergeRow(2, 3, 66, TRUE);
	for(nCol = 63; nCol <= 66; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);
	
	szText.Format("�翬�������� �����±��� ���� �������(MPa)");	m_Grid.SetTextMatrix(0, 67, szText);
	szText.Format("��.��.��ǥ 3.4.3");	m_Grid.SetTextMatrix(1,	67,	szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	67,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	68,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 69,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 70,	szText);
	m_Grid.SetMergeCol(0, 67, 70, TRUE);m_Grid.SetMergeCol(1, 67, 70, TRUE);
	m_Grid.SetMergeRow(2, 3, 67, TRUE);m_Grid.SetMergeRow(2, 3, 68, TRUE);
	m_Grid.SetMergeRow(2, 3, 69, TRUE);m_Grid.SetMergeRow(2, 3, 70, TRUE);
	for(nCol = 67; nCol <= 70; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);

	szText.Format("������������ �����±��� ���� �������(MPa)");	m_Grid.SetTextMatrix(0, 71, szText);
	szText.Format("��.��.��ǥ 3.4.4");	m_Grid.SetTextMatrix(1,	71,	szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	71,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	72,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 73,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 74,	szText);
	m_Grid.SetMergeCol(0, 71, 74, TRUE);m_Grid.SetMergeCol(1, 71, 74, TRUE);
	m_Grid.SetMergeRow(2, 3, 71, TRUE);m_Grid.SetMergeRow(2, 3, 72, TRUE);
	m_Grid.SetMergeRow(2, 3, 73, TRUE);m_Grid.SetMergeRow(2, 3, 74, TRUE);
	for(nCol = 71; nCol <= 74; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);

	szText.Format("���������� �޴� ������ ���� �ּ��ǵβ�(mm)");	m_Grid.SetTextMatrix(0, 75, szText);
	szText.Format("��.��.��ǥ 3.4.5");	m_Grid.SetTextMatrix(1,	75, szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	75,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	76,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 77,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 78,	szText);
	m_Grid.SetMergeCol(0, 75, 78, TRUE);m_Grid.SetMergeCol(1, 75, 78, TRUE);
	m_Grid.SetMergeRow(2, 3, 75, TRUE);m_Grid.SetMergeRow(2, 3, 76, TRUE);
	m_Grid.SetMergeRow(2, 3, 77, TRUE);m_Grid.SetMergeRow(2, 3, 78, TRUE);
	for(nCol = 75; nCol <= 78; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);

	szText.Format("������ ���� �����±��� ���� �������(MPa)");	m_Grid.SetTextMatrix(0, 79, szText);
	szText.Format("��.��.��ǥ 3.4.6");			m_Grid.SetTextMatrix(1,	79,	szText);
		szText.Format("T��40");					m_Grid.SetTextMatrix(2,	79,	szText);
		m_Grid.SetMergeCol(2, 79, 81, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 79,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 80,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 81,	szText);
		szText.Format("40<T��75");	m_Grid.SetTextMatrix(2,	82,	szText);
		m_Grid.SetMergeCol(2, 82, 84, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 82,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 83,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 84,	szText);
		szText.Format("75<T��100");	m_Grid.SetTextMatrix(2, 85,	szText);
		m_Grid.SetMergeCol(2, 85, 87, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 85,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 86,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 87,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 88,	szText);
		m_Grid.SetMergeCol(2, 88, 90, TRUE);
			szText.Format("a1");	m_Grid.SetTextMatrix(3, 88,	szText);
			szText.Format("a2");	m_Grid.SetTextMatrix(3, 89,	szText);
			szText.Format("a3");	m_Grid.SetTextMatrix(3, 90,	szText);
	m_Grid.SetMergeCol(0, 79, 90, TRUE);m_Grid.SetMergeCol(1, 79, 90, TRUE);
	for(nCol = 79; nCol <= 90; nCol++)
		m_Grid.SetColumnWidth(nCol, 50);

	szText.Format("���������� �ּ��ǵβ�");		m_Grid.SetTextMatrix(0, 91, szText);
	szText.Format("��.��.��ǥ 3.4.7");			m_Grid.SetTextMatrix(1,	91,	szText);
	m_Grid.SetMergeRow(2, 3, 91, TRUE);
	m_Grid.SetColumnWidth(91, 150);

	szText.Format("���� �ٴ��� ���� ������ �������");	m_Grid.SetTextMatrix(0, 92, szText);
	szText.Format("��.��.��ǥ 3.6.4");					m_Grid.SetTextMatrix(1, 92, szText);
		szText.Format("T��40");			m_Grid.SetTextMatrix(2,	92,	szText);
		szText.Format("40<T��75");		m_Grid.SetTextMatrix(2,	93,	szText);
		szText.Format("75<T��100");		m_Grid.SetTextMatrix(2, 94,	szText);
		szText.Format("T�����β�");	m_Grid.SetTextMatrix(2, 95,	szText);
	m_Grid.SetMergeCol(0, 92, 95, TRUE);m_Grid.SetMergeCol(1, 92, 95, TRUE);
	m_Grid.SetMergeRow(2, 3, 92, TRUE);m_Grid.SetMergeRow(2, 3, 93, TRUE);
	m_Grid.SetMergeRow(2, 3, 94, TRUE);m_Grid.SetMergeRow(2, 3, 95, TRUE);
	for(nCol = 92; nCol <= 95; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);

	szText.Format("�������� ������ �ּҵβ�(mm)");	m_Grid.SetTextMatrix(0, 96, szText);
	szText.Format("��.��.��ǥ 3.8.2");				m_Grid.SetTextMatrix(1,	96,	szText);
		szText.Format("���򺸰��� �ܼ�");	m_Grid.SetTextMatrix(2,	96,	szText);
			szText.Format("0��");	m_Grid.SetTextMatrix(3,	96, szText);
			szText.Format("1��");	m_Grid.SetTextMatrix(3,	97,	szText);
			szText.Format("2��");	m_Grid.SetTextMatrix(3,	98,	szText);
	m_Grid.SetMergeCol(0, 96, 98, TRUE);m_Grid.SetMergeCol(1, 96, 98, TRUE);
	m_Grid.SetMergeCol(2, 96, 98, TRUE);
	for(nCol = 96; nCol <= 98; nCol++)
		m_Grid.SetColumnWidth(nCol, 80);

	szText.Format("���̺긮�� �������� ������ �ּҵβ� (��)");	m_Grid.SetTextMatrix(0, 99, szText);
	szText.Format("��.��.��ǥ 3.8.4");							m_Grid.SetTextMatrix(1, 99, szText);
		szText.Format("���򺸰��� �ܼ�");	m_Grid.SetTextMatrix(2,	99, szText);
			szText.Format("0��");	m_Grid.SetTextMatrix(3,	99, szText);
			szText.Format("1��");	m_Grid.SetTextMatrix(3,	100, szText);
			szText.Format("2��");	m_Grid.SetTextMatrix(3,	101, szText);
	m_Grid.SetMergeCol(0, 99, 101, TRUE);m_Grid.SetMergeCol(1, 99, 101, TRUE);
	m_Grid.SetMergeCol(2, 99, 101, TRUE);
	for(nCol = 99; nCol <= 101; nCol++)
		m_Grid.SetColumnWidth(nCol, 90);

	szText.Format("���������� ���������ִ�ġ(��)");	m_Grid.SetTextMatrix(0, 102,	szText);
	szText.Format("��.��.��ǥ 3.8.3");				m_Grid.SetTextMatrix(1,	102,	szText);
		szText.Format("�÷��� ������");				m_Grid.SetTextMatrix(2,	102,	szText);
			szText.Format("a(t)");					m_Grid.SetTextMatrix(3,	102,	szText);
	m_Grid.SetColumnWidth(102, 190);
}

void CDesignSteelDimPage::SetDataInit()
{
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();

 	if(!GetSafeHwnd())				return;
 	if(!m_pDataManage->GetSteel())	return;
 	
 	SetGridTitle();
// 
	CSteel	*pSteel = m_pDataManage->GetSteel();

 	CString	str	= "", strStandard	= "";	
 	long nSize = pSteel->GetSize();
	long nSubIdx = 0, nThickIdx = 0;
// 	
 	for(long nRowIdx = 0; nRowIdx < nSize; nRowIdx++) 
 	{
		long nRow = nRowIdx + 4;
		long nCol = 0;
		CString sDecimalPT = "%g";
 		str = pSteel->GetStandard(nRowIdx);
 		
		if(str == strStandard)	
			str			= " ";
		else
			strStandard	= str;
		
		// �԰�	
 		m_Grid.SetTextMatrix(nRow, nCol++, str);							
		
		// �����ȣ
		m_Grid.SetTextMatrix(nRow, nCol++, pSteel->GetSymbol(nRowIdx));
		
		// ����� �β�
		m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_SHEARPLATE_THICK, nRowIdx, THICKNONE));
		
		// �ǵβ��� ���� �����׺���
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_STDYIELDPOINT_UP, nRowIdx, nThickIdx));
		
		//����������� �� ��������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_TENSIONSTRESS, nRowIdx, nThickIdx));
		
		//�����±��� ������� ���� ��������������� 
		//T��40
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx,	THICK40BELOW, nSubIdx));
	
		//40<T��75
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx,	THICK75BELOW, nSubIdx));
		
		//75<T��100
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx,	THICK100BELOW, nSubIdx));
		
		//T�����β�
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx,	THICKALLOWTHICKBELOW, nSubIdx));

		//����ھ�������
		//T��40 (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICK40BELOW_AWDIVAC2BELOW, nSubIdx));

		//40<T��75 (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICK75BELOW_AWDIVAC2BELOW, nSubIdx));
		
		//75<T��100 (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICK100BELOW_AWDIVAC2BELOW, nSubIdx));
		
		//T�����β� (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICKALLOWTHICKBELOW_AWDIVAC2BELOW, nSubIdx));
		
		//T��40 (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICK40BELOW_AWDIVAC2ABOVE, nSubIdx));
		
		//40<T��75 (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICK75BELOW_AWDIVAC2ABOVE, nSubIdx));
		
		//75<T��100 (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICK100BELOW_AWDIVAC2ABOVE, nSubIdx));
		
		//T�����β� (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	THICKALLOWTHICKBELOW_AWDIVAC2ABOVE, nSubIdx));
	
		//�����������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_SHEARSTRESS, nRowIdx, nThickIdx));
		//�����������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BOLTSTRESS, nRowIdx, nThickIdx));
		//������ �����������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_WELD_SHEARSTRESS, nRowIdx, nThickIdx));
		//���������� �޴� �翬�������� �ּ��ǵβ�
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_TWODOUBLE_COMP_MINTHICK, nRowIdx, nThickIdx));
		//�翬�������� �����±������� ������� (MPa)
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_TWOPLATE_STRESS, nRowIdx, nThickIdx));
		//������������ �����±��� ���� �������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_FREEJETTY, nRowIdx, nThickIdx));
		//���������� �޴� ������ ���� �ּ��ǵβ�
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_REINFORCEMINTHICK, nRowIdx, nThickIdx));

		//������ ���� �����±��� ���� �������
		//T��40
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx,	THICK40BELOW, nSubIdx));

		//40<T��75
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx,	THICK75BELOW, nSubIdx));
		
		//75<T��100
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx,	THICK100BELOW, nSubIdx));
		
		//T�����β�
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx,	THICKALLOWTHICKBELOW, nSubIdx));
		
		//������ ���� �ּ��ǵβ�
		m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_SUPPORTPLATE_MINTHICK, nRowIdx, THICK40BELOW));
		//���� �ٴ��� ���� ������ �������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			m_Grid.SetTextMatrix(nRow, nCol++,	sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_STEELDECK_SHEARSTRESS, nRowIdx, nThickIdx));
		//�������� ������ �ּҵβ� (��) 
		for(nThickIdx = 0; nThickIdx < 3; nThickIdx++)	
			m_Grid.SetTextMatrix(nRow, nCol++,	sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_PLATEBRIDGE_WEBMINTHICK, nRowIdx, nThickIdx));
		
		//���̺긮�� �������� ������ �ּҵβ� (��)
		
		double	dZero		= 0;
		for(nThickIdx = 0; nThickIdx < 3; nThickIdx++)
		{
			if(pOpt->GetSteelStandardYear() == APPLYYEAR2008)
				m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, dZero);
			else
				m_Grid.SetTextMatrix(nRow, nCol++,	sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_HYPLATEBDIRGE_WEBMINTHICK, nRowIdx, nThickIdx));
		}

		//���������� ���� ���� �ִ�ġ
		m_Grid.SetTextMatrix(nRow, nCol++, sDecimalPT, pSteel->GetSteelAllowValue(ALLOW_VSTIFFNOT, nRowIdx,	THICK40BELOW));
	}

 	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL CDesignSteelDimPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignSteelDimPage::SetDataDefault()
{
	CSteel *pSteel = m_pDataManage->GetSteel();
	pSteel->SetDataInit();
	SetDataInit();
}

void CDesignSteelDimPage::SetDataSave()
{
	CSteel			*pSteel = m_pDataManage->GetSteel();
 	if(!GetSafeHwnd())	return;
 	if(!pSteel)		return;
 	
// 
 	long nSize = pSteel->GetSize();
	long nSubIdx = 0, nThickIdx = 0;
// 	
 	for(long nRowIdx = 0; nRowIdx < nSize; nRowIdx++) 
 	{
		long nRow = nRowIdx + 4;
		long nCol = 2;
		
		// ����� �β�
		pSteel->SetSteelAllowValue(ALLOW_SHEARPLATE_THICK, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), THICKNONE);
		
		// �ǵβ��� ���� �����׺���
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_STDYIELDPOINT_UP, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		
		//����������� �� ��������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_TENSIONSTRESS, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		
		//�����±��� ������� ���� ��������������� 
		//T��40
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK40BELOW, nSubIdx);
		//40<T��75
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx,  m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK75BELOW, nSubIdx);
		//75<T��100
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx,  m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK100BELOW, nSubIdx);
		//T�����β�
		for(nSubIdx = 0; nSubIdx < 4; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_UNCONSIDER, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++),THICKALLOWTHICKBELOW, nSubIdx);

		//����ھ�������
		//T��40 (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK40BELOW_AWDIVAC2BELOW, nSubIdx);
		//40<T��75 (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK75BELOW_AWDIVAC2BELOW, nSubIdx);
		//75<T��100 (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
		pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,  m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK100BELOW_AWDIVAC2BELOW, nSubIdx);
		//T�����β� (Aw/Ac��2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++),THICKALLOWTHICKBELOW_AWDIVAC2BELOW, nSubIdx);
		//T��40 (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK40BELOW_AWDIVAC2ABOVE, nSubIdx);
		//40<T��75 (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK75BELOW_AWDIVAC2ABOVE,  nSubIdx);
		//75<T��100 (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK100BELOW_AWDIVAC2ABOVE, nSubIdx);
		//T�����β� (Aw/Ac>2)
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BENDINGCOMP_CASE1, nRowIdx,	m_Grid.GetTextMatrixDouble(nRow, nCol++),THICKALLOWTHICKBELOW_AWDIVAC2ABOVE, nSubIdx);
		
		//�����������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_SHEARSTRESS, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		//�����������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BOLTSTRESS, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		//������ �����������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_WELD_SHEARSTRESS, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		//���������� �޴� �翬�������� �ּ��ǵβ�
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_TWODOUBLE_COMP_MINTHICK, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		//�翬�������� �����±������� ������� (MPa)
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_TWOPLATE_STRESS, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		//������������ �����±��� ���� �������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_FREEJETTY,	nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		//���������� �޴� ������ ���� �ּ��ǵβ�
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_REINFORCEMINTHICK,	nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);

		//������ ���� �����±��� ���� �������
		//T��40
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx,	m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK40BELOW, nSubIdx);
		//40<T��75
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK75BELOW, nSubIdx);
		//75<T��100
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++),THICK100BELOW, nSubIdx);
		//T�����β�
		for(nSubIdx = 0; nSubIdx < 3; nSubIdx++)
			pSteel->SetSteelAllowValue(ALLOW_BUCKLING_REINFORCE, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++),THICKALLOWTHICKBELOW, nSubIdx);

		//������ ���� �ּ��ǵβ�
		pSteel->SetSteelAllowValue(ALLOW_SUPPORTPLATE_MINTHICK, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), THICK40BELOW);

		//���� �ٴ��� ���� ������ �������
		for(nThickIdx = 0; nThickIdx < 4; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_STEELDECK_SHEARSTRESS, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		//�������� ������ �ּҵβ� (��) 
		for(nThickIdx = 0; nThickIdx < 3; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_PLATEBRIDGE_WEBMINTHICK, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		
		//���̺긮�� �������� ������ �ּҵβ� (��) 2010�� ��
		for(nThickIdx = 0; nThickIdx < 3; nThickIdx++)
			pSteel->SetSteelAllowValue(ALLOW_HYPLATEBDIRGE_WEBMINTHICK,	nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol++), nThickIdx);
		
		//���������� ���� ���� �ִ�ġ
		pSteel->SetSteelAllowValue(ALLOW_VSTIFFNOT, nRowIdx, m_Grid.GetTextMatrixDouble(nRow, nCol), THICK40BELOW);
	}
}

void CDesignSteelDimPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}
