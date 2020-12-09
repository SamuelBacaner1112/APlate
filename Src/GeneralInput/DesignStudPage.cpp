// DesignStudPage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DesignStudPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignBoltPage dialog

CDesignStudPage::CDesignStudPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignStudPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignStudPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignStudPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignStudPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignStudPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignStudPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignStudPage message handlers
void CDesignStudPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignStudPage::SetDataInit()
{
	CSteelStud* pSteelStud =  (CSteelStud*)m_pDataManage->GetSteelStud();

	SetGridTitle();

	for(long nRow = 0;nRow<pSteelStud->m_StudArr.GetSize();nRow++)
	{
		CSteelStudData	*pData  = pSteelStud->m_StudArr.GetAt(nRow);
		m_Grid.SetTextMatrix(nRow+1,1,"%.3f",pData->m_dUWeight19);
		m_Grid.SetTextMatrix(nRow+1,2,"%.3f",pData->m_dUWeight22);
		m_Grid.SetTextMatrix(nRow+1,3,"%.3f",pData->m_dUWeight25);
	}

}

void CDesignStudPage::SetGridTitle()
{
	CSteelStud* pSteelStud	= (CSteelStud*) m_pDataManage->GetSteelStud();
	long		nSize		= pSteelStud->m_StudArr.GetSize();
	
	m_Grid.SetColumnCount(4);
	m_Grid.SetRowCount(nSize+1);
	m_Grid.SetFiexedRows(1);
	m_Grid.SetFixedCols(1);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);


	m_Grid.SetTextMatrix(0,0,"È£Äª");
	m_Grid.SetTextMatrix(0,1,"¥Õ19");
	m_Grid.SetTextMatrix(0,2,"¥Õ22");
	m_Grid.SetTextMatrix(0,3,"¥Õ25");
	for(long nCount = 0; nCount < pSteelStud->m_StudArr.GetSize();nCount++)
	{
		CSteelStudData	*pData  = pSteelStud->m_StudArr.GetAt(nCount);
		m_Grid.SetTextMatrix(nCount+1,0,pData->m_strName);
	}
	
	if(!GetSafeHwnd()) return;
}

BOOL CDesignStudPage::OnInitDialog()
{
	TStackedPage::OnInitDialog();
	
	return TRUE;
}

void CDesignStudPage::SetDataDefault()
{
	m_pDataManage->GetSteelStud()->SetDataInit();
	SetDataInit();
}

void CDesignStudPage::SetDataSave()
{
	CSteelStud* pSteelStud =  (CSteelStud*)m_pDataManage->GetSteelStud();

	for(long nRow = 0;nRow<pSteelStud->m_StudArr.GetSize();nRow++)
	{
		CSteelStudData	*pData  = pSteelStud->m_StudArr.GetAt(nRow);
		pData->m_dUWeight19 = m_Grid.GetTextMatrixDouble(nRow+1,1);
		pData->m_dUWeight22 = m_Grid.GetTextMatrixDouble(nRow+1,2);
		pData->m_dUWeight25 = m_Grid.GetTextMatrixDouble(nRow+1,3);
	}
		
}

void CDesignStudPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}
