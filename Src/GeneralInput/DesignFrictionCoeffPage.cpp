// DesignFrictionCoeffPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "../APlateCalc/APlateCalc.h"
#include "DesignFrictionCoeffPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignFrictionCoeffPage dialog

SHGRIDTITLE CDesignFrictionCoeffPage::m_gt[COLUMN_COUNT] = 
{	
	{"\n\n�������� ����",							"%s",		120,	DT_RIGHT,	TRUE},
	{"\n\n��Ʈ�� ����",								"%s",		160,	DT_RIGHT,	TRUE},
	{"\n�Ļ������\n(1/m)",						"%-.2lf",	140,	DT_RIGHT,	TRUE},
	{"\n����������\n",							"%-.2lf",	140,	DT_RIGHT,	FALSE},	
};

CDesignFrictionCoeffPage::CDesignFrictionCoeffPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignFrictionCoeffPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignFrictionCoeffPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDesignFrictionCoeffPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignFrictionCoeffPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignFrictionCoeffPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignFrictionCoeffPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignFrictionCoeffPage message handlers
void CDesignFrictionCoeffPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignFrictionCoeffPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	// �׸��� �⺻ ���� 
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(ROW_COUNT);
	m_Grid.SetColumnCount(COLUMN_COUNT);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT, TRUE);

	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 52);
	m_Grid.SetMergeRow(6, 7, 0);

	m_Grid.SetItemText(1, 0, "\n\n\nPS ���� �Ǵ�\n���ݵ��� ����\nPS ��Ʈ����");
	m_Grid.SetItemFormat(1,0,DT_CENTER);
	m_Grid.SetMergeRow(1, 5, 0);
	m_Grid.SetTextMatrix(6, 0, "���� ����");

	m_Grid.SetTextMatrix(1, 1, "�ݼӽ���");
	m_Grid.SetTextMatrix(2, 1, "�ƿ����� �ݼӽ���");
	m_Grid.SetTextMatrix(3, 1, "�ƽ���Ʈ �Ǵ� �׸�����\n���õǰ� �Ǻ��Ȱ�");
	m_Grid.SetItemFormat(3, 1,DT_CENTER);
	m_Grid.SetMergeRow(3, 4, 1);
	
	m_Grid.SetTextMatrix(5, 1, "�ƿ����ݵ� ���� ��Ʈ");
	m_Grid.SetTextMatrix(6, 1, "�ݼӽ���");
	m_Grid.SetTextMatrix(7, 1, "�ƿ����ݵ� �ݼӽ���");
	m_Grid.SetMergeRow(3, 4, 2);
	m_Grid.SetMergeRow(3, 4, 3);
}

void CDesignFrictionCoeffPage::SetDataInit()
{
	SetGridTitle();
	double dUndulateFriction[] = { 0.0066, 0.0050, 0.0066, 0, 0.0007, 0.0010, 0.0007, };
	double dCurvatureFriction[] = { 0.30, 0.25, 0.30, 0, 0.25, 0.20, 0.15, };

	for(long n=1; n < 8; n++)
	{
		m_Grid.SetTextMatrix(n, 2, "%.4f", dUndulateFriction[n-1]);
		m_Grid.SetTextMatrix(n, 3, "%.2f", dCurvatureFriction[n-1]);

		if(n==3) n++;
	}
}

void CDesignFrictionCoeffPage::SetDataSave()
{

}

void CDesignFrictionCoeffPage::SetDataDefault()
{
	SetGridTitle();
	SetDataInit();
}

void CDesignFrictionCoeffPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}
