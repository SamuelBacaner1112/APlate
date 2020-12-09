// DesignBoltPage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DesignBoltPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignBoltPage dialog

CDesignBoltPage::CDesignBoltPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignBoltPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignBoltPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignBoltPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignBoltPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignBoltPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignBoltPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_BUTTONCLICK,	IDC_GRID, OnCellButtonClick)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignBoltPage message handlers
void CDesignBoltPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignBoltPage::SetDataInit()
{
	SetGridTitle();
	CSteelBolt *pSteelBolt = m_pDataManage->GetSteelBolt();

	long size = pSteelBolt->m_BoltArr.GetSize();
	if(size < 1) return;


	for(long nRow = 2; nRow < size+2 ; nRow++) 
	{	
		long nCol = 3;
		
		m_Grid.SetTextMatrix(nRow, nCol++, "%g", tokN(pSteelBolt->m_BoltArr[nRow-2]->m_ForceAllow));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_CENTERSPACEMIN));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_CENTERSPACEMAX));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_SHEARFACEDISTANCEMIN));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_STRESSFACEDISTANCEMIN));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_HOLEMARGIN));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_BASEWEIGHT));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_LENGTHPERWEIGHT));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_NUTWEIGHT));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_WASHERWEIGHT));
		m_Grid.SetTextMatrix(nRow, nCol++, pSteelBolt->GetStringData(nRow-2, BDIB_LENGTH));
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignBoltPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	CSteelBolt *pSteelBolt = m_pDataManage->GetSteelBolt();

	long nSize = pSteelBolt->m_BoltArr.GetSize();
	if(nSize < 1) return;

	// 그리드 기본 셋팅 
	m_Grid.ResetAttrib(TRUE);
	m_Grid.SetEditable();
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetRowCount(nSize+5);
	m_Grid.SetColumnCount(14);	
//	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetFixedRowCount(2);	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(1, 60);
	m_Grid.SetColumnWidthAll(40);
	m_Grid.SetColumnWidth(0, 70);
	m_Grid.SetColumnWidth(3, 60);
	m_Grid.SetColumnWidth(6, 60);
	m_Grid.SetColumnWidth(7, 60);
	m_Grid.SetColumnWidth(8, 50);
	m_Grid.SetColumnWidth(9, 60);
	m_Grid.SetColumnWidth(10, 60);
	m_Grid.SetColumnWidth(11, 60);
	m_Grid.SetColumnWidth(12, 60);

	// TITLE //////////////////////////////////////////////////////////////////////////
	m_Grid.SetMergeCol(0, 0, 3);
	m_Grid.SetMergeCol(0, 4, 8);
	m_Grid.SetMergeCol(0, 9, 13);
	
	m_Grid.SetTextMatrix(0, 0, "볼트제원");
	m_Grid.SetTextMatrix(0, 4, "볼트 및 배치 관련 간격 (mm)");
	m_Grid.SetTextMatrix(0, 9, "볼트 중량 (kg)");

	long nCol = 0;
	m_Grid.SetTextMatrix(1, nCol, "규격");						m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "볼트\n등급");				m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "나사\n호칭");				m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "허용력\n1볼트\n마찰면\n(kN)");	m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "최소\n간격");				m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "최대\n간격");				m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "전단면\n최소\n연단거리");	m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "압연면\n최소\n연단거리");	m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "볼트홀\n여유치");			m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "기본");						m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "길이당\n중량\n(kg/mm)");		m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "너트");						m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "와셔");						m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(1, nCol, "추가\n길이\n(mm)");			m_Grid.SetItemFormat(1, nCol++, DT_VANDHCENTER_AUTO);

	for(long nIdx=0; nIdx<nSize; nIdx++)
	{
		m_Grid.SetTextMatrix(nIdx+2, 0, pSteelBolt->GetStringData(nIdx, BDIB_STANDARD));
		m_Grid.SetTextMatrix(nIdx+2, 1, pSteelBolt->GetStringData(nIdx, BDIB_GRADE));
		m_Grid.SetTextMatrix(nIdx+2, 2, pSteelBolt->GetStringData(nIdx, BDIB_SIZE));

		if(m_Grid.GetTextMatrix(nIdx+2, 1).Find("13") == -1)
			m_Grid.SetCellFixType(nIdx+2, 1, GVN_ONECELL_COLFIX);
		m_Grid.SetCellFixType(nIdx+2, 0, GVN_ONECELL_COLFIX);
		m_Grid.SetCellFixType(nIdx+2, 2, GVN_ONECELL_COLFIX);
	}

	CString szText = m_Grid.GetTextMatrix(2, 0);
	long nMerge = 2;
	long nRow	= 0;
	for(nRow=3; nRow<nSize+2; nRow++)
	{
		if(szText == m_Grid.GetTextMatrix(nRow, 0))	continue;
		m_Grid.SetMergeRow(nMerge, nRow, 0);

		nMerge = nRow;	
		szText = m_Grid.GetTextMatrix(nRow, 0);
	}
	m_Grid.SetMergeRow(nMerge, nRow-1, 0);

	szText = m_Grid.GetTextMatrix(2, 1);
	nMerge = 2;
	for(nRow=3; nRow<nSize+2; nRow++)
	{		
		if(szText == m_Grid.GetTextMatrix(nRow, 1))	continue;
		
		if(m_Grid.GetTextMatrix(nRow, 1).Find("13")!=-1)
			m_Grid.SetCellType(nMerge, 1, CT_BUTTON);
		m_Grid.SetMergeRow(nMerge, nRow-1, 1);

		nMerge = nRow;
		szText = m_Grid.GetTextMatrix(nRow, 1);
	}
	m_Grid.SetMergeRow(nMerge, nRow-1, 1);
	CString strText = m_Grid.GetTextMatrix(nRow-1, 1);
	if(strText.Find("13")!=-1)
	{
		for(long n=nMerge; n<nRow; n++)
			m_Grid.SetCellType(n, 1, CT_BUTTON);
	}

	m_Grid.SetMergeCol(nSize+2, 0, 2);	m_Grid.SetMergeCol(nSize+2, 3, 13);
	m_Grid.SetMergeCol(nSize+3, 0, 2);	m_Grid.SetMergeCol(nSize+3, 3, 13);
	m_Grid.SetMergeCol(nSize+4, 0, 2);	m_Grid.SetMergeCol(nSize+4, 3, 13);

	for(nCol=0; nCol<3; nCol++)
	{
		m_Grid.SetMergeRow(nSize+2, nSize+4, nCol);
		m_Grid.SetCellFixType(nSize+2, nCol, GVN_ONECELL_COLFIX);
		m_Grid.SetCellFixType(nSize+3, nCol, GVN_ONECELL_COLFIX);
		m_Grid.SetCellFixType(nSize+4, nCol, GVN_ONECELL_COLFIX);
	}

	m_Grid.SetTextMatrix(nSize+2, 0, "단위중량 산식 (kgf/ea)");
	m_Grid.SetTextMatrix(nSize+2, 3, "              HTB : 기본 + 길이당중량 × 볼트길이 + 너트 + 2ea × 와셔", (UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nSize+3, 3, "              TSB : 기본 + 길이당중량 × 볼트길이 + 너트 + 1ea × 와셔", (UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nSize+4, 3, "              볼트길이 산정식 : 모재두께 + 이음판두께 × 2면 + 추가길이", (UINT)DT_LEFT);

	for(nCol=3; nCol<14; nCol++)
	{
		m_Grid.SetItemState(nSize+2, nCol, GVIS_READONLY);
		m_Grid.SetItemState(nSize+3, nCol, GVIS_READONLY);
		m_Grid.SetItemState(nSize+4, nCol, GVIS_READONLY);
	}
}

BOOL CDesignBoltPage::OnInitDialog()
{
	TStackedPage::OnInitDialog();
	
	return TRUE;
}

void CDesignBoltPage::SetDataDefault()
{
	CSteelBolt *pSteelBolt = m_pDataManage->GetSteelBolt();
	pSteelBolt->SetDataInit();
	SetDataInit();
}

void CDesignBoltPage::SetDataSave()
{
	CSteelBolt *pSteelBolt = m_pDataManage->GetSteelBolt();
	long nSize = pSteelBolt->m_BoltArr.GetSize();
		
	for(long nRow=2; nRow<nSize+2; nRow++)
	{
		long nCol = 3;
		pSteelBolt->SetForceAllow(nRow-2,				frkN(m_Grid.GetTextMatrixDouble(nRow, nCol++)));
		pSteelBolt->SetCenterSpaceMin(nRow-2,			m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetCenterSpaceMax(nRow-2,			m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetShearFaceDistanceMin(nRow-2,		m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetStressFaceDistanceMin(nRow-2,	m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetHoleMargin(nRow-2,				m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetBaseWeight(nRow-2,				m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetLengthPerWeight(nRow-2,			m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetNutWeight(nRow-2,				m_Grid.GetTextMatrixDouble(nRow, nCol++));
		pSteelBolt->SetWasherWeight(nRow-2,				m_Grid.GetTextMatrixDouble(nRow, nCol++));		
		pSteelBolt->SetLength(nRow-2,					m_Grid.GetTextMatrixDouble(nRow, nCol++));
	}
}

void CDesignBoltPage::OnCellButtonClick(NMHDR* nmgv, LRESULT*)
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, 318);
}

void CDesignBoltPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();	
	SetDataInit();
	return;
}
