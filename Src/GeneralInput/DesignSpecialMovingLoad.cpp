// DesignSpecialMovingLoad.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignSpecialMovingLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignSpecialMovingLoad property page

//IMPLEMENT_DYNCREATE(CDesignSpecialMovingLoad, TStackedPage)

CDesignSpecialMovingLoad::CDesignSpecialMovingLoad(CDataManage *pMng,  CWnd* pParent /*=NULL*/) : TStackedPage(CDesignSpecialMovingLoad::IDD)
{
	//{{AFX_DATA_INIT(CDesignSpecialMovingLoad)

	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_BitTank.LoadBitmap(IDB_TANK);
}

CDesignSpecialMovingLoad::~CDesignSpecialMovingLoad()
{

}

void CDesignSpecialMovingLoad::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignSpecialMovingLoad)
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDesignSpecialMovingLoad, TStackedPage)
	//{{AFX_MSG_MAP(CDesignSpecialMovingLoad)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignSpecialMovingLoad message handlers
void CDesignSpecialMovingLoad::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignSpecialMovingLoad::SetDataDefault()
{
	CCalcData *pData = m_pDataManage->GetCalcData();

	// UNIT kN m
	pData->m_TankDis    = 0.600;
	pData->m_TankWidth  = 2.600;
	pData->m_TrailDis   = 0.600;
	pData->m_TrailWidth = 2.225;

	pData->m_TankWeight = 544.2;
	pData->m_TankEdit1  = 4.500;
	pData->m_TankEdit2  = 0.700;	
	pData->m_TankEdit3  = 1.900;	
	pData->m_TankImpact	= 15;	
		
	pData->m_TrailEdit1	= 3.600;
	pData->m_TrailEdit2	= 1.500;
	pData->m_TrailEdit3	= 4.500;
	pData->m_TrailEdit4	= 1.200;
	pData->m_TrailEdit5	= 0.975;
	pData->m_TrailEdit6	= 1.250;	
	pData->m_TrailImpact= 15;	

	pData->m_TrailP1    = 40.815;
	pData->m_TrailP2    = 81.630;
	pData->m_TrailP3    = 58.955;
	SetDataInit();
}

void CDesignSpecialMovingLoad::SetGridTitle()
{
	long	nRows	= 21;
	long	nCols	= 5;
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 120);
	m_Grid.SetColumnWidth(2, 80);
	m_Grid.SetColumnWidth(3, 60);
	m_Grid.SetColumnWidth(4, 400);
	for(long nRow=0;nRow<nRows;nRow++)
	{
		m_Grid.SetCellFixType(nRow, 0, GVN_ONECELL_COLFIX);
		m_Grid.SetCellFixType(nRow, 1, GVN_ONECELL_COLFIX);
		m_Grid.SetItemState(nRow, 4, GVIS_READONLY);
	}

	m_Grid.SetMergeRow(0, 20, 4);
	CBitmap *pBitmapTank	= new CBitmap;
	pBitmapTank->LoadBitmap(IDB_TANK);
	m_Grid.AddBitmap(0,4,pBitmapTank);
	m_Grid.SetTextMatrix(0, 0, "< 탱크 궤도 하중 >");	m_Grid.SetMergeCol(0, 0, 3);
	m_Grid.SetTextMatrix(1, 0, "연석으로부터의 거리");	m_Grid.SetMergeCol(1, 0, 1);
	m_Grid.SetTextMatrix(1, 3, "m");					m_Grid.SetItemState(1, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(2, 0, "활하중의 충격계수");	m_Grid.SetMergeCol(2, 0, 1);
	m_Grid.SetTextMatrix(2, 3, "%적용");				m_Grid.SetItemState(2, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(3, 0, "탱크 총 중량");			m_Grid.SetMergeCol(3, 0, 1);
	m_Grid.SetTextMatrix(3, 3, "kN");					m_Grid.SetItemState(3, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(4, 0, "작용하중위치");			m_Grid.SetMergeRow(4, 7, 0);
	m_Grid.SetTextMatrix(4, 1, "궤도접지길이(L1)");
	m_Grid.SetTextMatrix(4, 3, "m");					m_Grid.SetItemState(4, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(5, 1, "궤도폭(B1)");
	m_Grid.SetTextMatrix(5, 3, "m");					m_Grid.SetItemState(5, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(6, 1, "궤도내측간거리(B2)");
	m_Grid.SetTextMatrix(6, 3, "m");					m_Grid.SetItemState(6, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(7, 1, "하중간거리(Ptk)");
	m_Grid.SetTextMatrix(7, 3, "m");					m_Grid.SetItemState(7, 3, GVIS_READONLY);

	m_Grid.SetTextMatrix(8, 0, "< 탱크 트레일러 하중 >");	m_Grid.SetMergeCol(8, 0, 3);
	m_Grid.SetTextMatrix(9, 0, "연석으로부터의 거리");	m_Grid.SetMergeCol(9, 0, 1);
	m_Grid.SetTextMatrix(9, 3, "m");					m_Grid.SetItemState(9, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(10, 0, "활하중의 충격계수");	m_Grid.SetMergeCol(10, 0, 1);
	m_Grid.SetTextMatrix(10, 3, "%적용");				m_Grid.SetItemState(10, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(11, 0, "차축중량");			m_Grid.SetMergeRow(11, 13, 0);
	m_Grid.SetTextMatrix(11, 1, "P1");
	m_Grid.SetTextMatrix(11, 3, "kN");					m_Grid.SetItemState(11, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(12, 1, "P2");
	m_Grid.SetTextMatrix(12, 3, "kN");					m_Grid.SetItemState(12, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(13, 1, "P3");
	m_Grid.SetTextMatrix(13, 3, "kN");					m_Grid.SetItemState(13, 3, GVIS_READONLY);

	m_Grid.SetTextMatrix(14, 0, "작용하중위치");		m_Grid.SetMergeRow(14, 20, 0);
	m_Grid.SetTextMatrix(14, 1, "차축간거리(L1)");
	m_Grid.SetTextMatrix(14, 3, "m");					m_Grid.SetItemState(14, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(15, 1, "차축간거리(L2)");
	m_Grid.SetTextMatrix(15, 3, "m");					m_Grid.SetItemState(15, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(16, 1, "차축간거리(L3)");
	m_Grid.SetTextMatrix(16, 3, "m");					m_Grid.SetItemState(16, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(17, 1, "차축간거리(L4)");
	m_Grid.SetTextMatrix(17, 3, "m");					m_Grid.SetItemState(17, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(18, 1, "바퀴폭(B1)");
	m_Grid.SetTextMatrix(18, 3, "m");					m_Grid.SetItemState(18, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(19, 1, "바퀴내측간거리(B2)");
	m_Grid.SetTextMatrix(19, 3, "m");					m_Grid.SetItemState(19, 3, GVIS_READONLY);
	m_Grid.SetTextMatrix(20, 1, "하중간거리(Ptr)");
	m_Grid.SetTextMatrix(20, 3, "m");					m_Grid.SetItemState(20, 3, GVIS_READONLY);
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDesignSpecialMovingLoad::SetDataInit()
{
	SetGridTitle();
	CCalcData *pData = m_pDataManage->GetCalcData();

	m_Grid.SetTextMatrix(1, 2, "%.3f", pData->m_TankDis);
	m_Grid.SetTextMatrix(2, 2, "%.3f", pData->m_TankImpact);
	m_Grid.SetTextMatrix(3, 2, "%.3f", pData->m_TankWeight);
	m_Grid.SetTextMatrix(4, 2, "%.3f", pData->m_TankEdit1);
	m_Grid.SetTextMatrix(5, 2, "%.3f", pData->m_TankEdit2);
	m_Grid.SetTextMatrix(6, 2, "%.3f", pData->m_TankEdit3);
	m_Grid.SetTextMatrix(7, 2, "%.3f", pData->m_TankWidth);
	m_Grid.SetTextMatrix(9, 2, "%.3f", pData->m_TrailDis);
	m_Grid.SetTextMatrix(10, 2, "%.3f", pData->m_TrailImpact);
	m_Grid.SetTextMatrix(11, 2, "%.3f", pData->m_TrailP1);
	m_Grid.SetTextMatrix(12, 2, "%.3f", pData->m_TrailP2);
	m_Grid.SetTextMatrix(13, 2, "%.3f", pData->m_TrailP3);
	m_Grid.SetTextMatrix(14, 2, "%.3f", pData->m_TrailEdit1);
	m_Grid.SetTextMatrix(15, 2, "%.3f", pData->m_TrailEdit2);
	m_Grid.SetTextMatrix(16, 2, "%.3f", pData->m_TrailEdit3);
	m_Grid.SetTextMatrix(17, 2, "%.3f", pData->m_TrailEdit4);
	m_Grid.SetTextMatrix(18, 2, "%.3f", pData->m_TrailEdit5);
	m_Grid.SetTextMatrix(19, 2, "%.3f", pData->m_TrailEdit6);
	m_Grid.SetTextMatrix(20, 2, "%.3f", pData->m_TrailWidth);
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDesignSpecialMovingLoad::SetDataSave()
{
	CCalcData *pData = m_pDataManage->GetCalcData();

	pData->m_TankDis	= m_Grid.GetTextMatrixDouble(1, 2);
	pData->m_TankImpact	= m_Grid.GetTextMatrixDouble(2, 2);
	pData->m_TankWeight	= m_Grid.GetTextMatrixDouble(3, 2);
	pData->m_TankEdit1	= m_Grid.GetTextMatrixDouble(4, 2);
	pData->m_TankEdit2	= m_Grid.GetTextMatrixDouble(5, 2);
	pData->m_TankEdit3	= m_Grid.GetTextMatrixDouble(6, 2);
	pData->m_TankWidth	= m_Grid.GetTextMatrixDouble(7, 2);
	pData->m_TrailDis	= m_Grid.GetTextMatrixDouble(9, 2);
	pData->m_TrailImpact= m_Grid.GetTextMatrixDouble(10, 2);
	pData->m_TrailP1	= m_Grid.GetTextMatrixDouble(11, 2);
	pData->m_TrailP2	= m_Grid.GetTextMatrixDouble(12, 2);
	pData->m_TrailP3	= m_Grid.GetTextMatrixDouble(13, 2);
	pData->m_TrailEdit1	= m_Grid.GetTextMatrixDouble(14, 2);
	pData->m_TrailEdit2	= m_Grid.GetTextMatrixDouble(15, 2);
	pData->m_TrailEdit3	= m_Grid.GetTextMatrixDouble(16, 2);
	pData->m_TrailEdit4	= m_Grid.GetTextMatrixDouble(17, 2);
	pData->m_TrailEdit5	= m_Grid.GetTextMatrixDouble(18, 2);
	pData->m_TrailEdit6	= m_Grid.GetTextMatrixDouble(19, 2);
	pData->m_TrailWidth	= m_Grid.GetTextMatrixDouble(20, 2);
}
