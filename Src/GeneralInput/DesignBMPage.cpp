// DesignBMPage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DesignBMPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SHGRIDTITLE CDesignBMPage::m_gt[COLUMN_COUNT_BILLENV] = 
	{	
		{"종 류",	"%s",	250, DT_CENTER,  TRUE},		
		{"집계표 적용기준 설계용(상.하판)",	"%s",	250, DT_LEFT,  FALSE},		
		{"집계표 적용기준 제작용",	"%s",	250, DT_LEFT,  FALSE},		
		{"집계표 총중량 자리수",	"%.0f",	250, DT_LEFT,  FALSE},
		{"집계표 단위중량 자리수",	"%.0f",	250, DT_LEFT,  FALSE},
		{"중량 포함 볼트",	"%s",	250, DT_LEFT,  FALSE},		
		{"중량 포함 전단연결재",	"%s",	250, DT_LEFT,  FALSE},		
		{"중량 포함 슬래브 앵커",	"%s",	250, DT_LEFT,  FALSE},		
		//{"중량 포함 들고리",	"%s",	250, DT_LEFT,  FALSE},		
		{"차량적재 산출시 현장이음 포함",	"%s",	250, DT_LEFT,  FALSE},		
		//{"차량적재 산출시 가로보브라켓 포함","%s",	250, DT_LEFT,  FALSE},		
	};

/////////////////////////////////////////////////////////////////////////////
// CDesignBMPage dialog


CDesignBMPage::CDesignBMPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignBMPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignBMPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignBMPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignBMPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignBMPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignBMPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignBMPage message handlers
void CDesignBMPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignBMPage::SetDataDefault()
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
	pOpt->InitDesignQty();	
	SetDataInit();
}

void CDesignBMPage::SetGridTitle()
{
	m_Grid.SetColumnCount(2);	
	m_Grid.SetFixedColumnCount(1);		
	m_Grid.SetFixedRowCount(1);		
	m_Grid.SetColumnWidth(1,100);
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT_BILLENV, FALSE, FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowHeightAll(20);					
	m_Grid.SetColumnWidth(1,200);
}

void CDesignBMPage::SetDataInit()
{	
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();

	SetGridTitle();

	m_Grid.SetCellType(1,1,CT_COMBO);
	m_Grid.SetCellType(2,1,CT_COMBO);
	m_Grid.SetCellType(5,1,CT_COMBO);
	m_Grid.SetCellType(6,1,CT_COMBO);
	m_Grid.SetCellType(7,1,CT_COMBO);
	m_Grid.SetCellType(8,1,CT_COMBO);
	//m_Grid.SetCellType(9,1,CT_COMBO);
	//m_Grid.SetCellType(10,1,CT_COMBO);

	m_Grid.SetTextMatrix(0, 1,"설정값");			
	m_Grid.SetGridData(m_gt, 1, 1,pOpt->GetStrSummaryApplyBaseOfDesign());	
	m_Grid.SetGridData(m_gt, 2, 1,pOpt->GetStrSummaryApplyBaseOfAssm());
	m_Grid.SetGridData(m_gt, 3, 1,pOpt->GetPointValueOfGangJae());
	m_Grid.SetGridData(m_gt, 4, 1,pOpt->GetPointValueOfUnitGangJae());
	m_Grid.SetGridData(m_gt, 5, 1,pOpt->GetSummaryAddBoltWeight() ? "개소수, 중량 포함" : "개소수만 포함");
	m_Grid.SetGridData(m_gt, 6, 1,pOpt->GetSummaryAddStudWeight() ? "개소수, 중량 포함" : "개소수만 포함");
	m_Grid.SetGridData(m_gt, 7, 1,pOpt->GetSummaryAddSlabAnchorWeight() ? "개소수, 중량 포함" : "개소수만 포함");
	//m_Grid.SetGridData(m_gt, 8, 1,pOpt->GetSummaryAddGoriWeight() ? "개소수, 중량 포함" : "포함 안함");	
	m_Grid.SetGridData(m_gt, 8, 1,pOpt->GetSummaryAddSpliceWeight() ? "포함" : "포함 안함");	
	//m_Grid.SetGridData(m_gt, 10, 1,pOpt->GetAssmUseCrossBeamBracketGirderOfBigAssm() ? "포함" : "포함 안함");	

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignBMPage::SetDataSave()
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();

	pOpt->SetStrSummaryApplyBaseOfDesign(m_Grid.GetTextMatrix(1,1));	
	pOpt->SetStrSummaryApplyBaseOfAssm(m_Grid.GetTextMatrix(2,1));
	pOpt->SetPointValueOfGangJae(m_Grid.GetTextMatrixLong(3,1));
	pOpt->SetPointValueOfUnitGangJae(m_Grid.GetTextMatrixLong(4,1));
	pOpt->SetSummaryAddBoltWeight(m_Grid.GetTextMatrix(5,1) == "개소수, 중량 포함" ? 1 : 0);
	pOpt->SetSummaryAddStudWeight(m_Grid.GetTextMatrix(6,1) == "개소수, 중량 포함" ? 1 : 0);
	pOpt->SetSummaryAddSlabAnchorWeight(m_Grid.GetTextMatrix(7,1) == "개소수, 중량 포함" ? 1 : 0);
	//pOpt->SetSummaryAddGoriWeight(m_Grid.GetTextMatrix(8,1) == "개소수, 중량 포함" ? 1 : 0);
	pOpt->SetSummaryAddSpliceWeight(m_Grid.GetTextMatrix(8,1) == "포함" ? 1 : 0);
	//pOpt->SetAssmUseCrossBeamBracketBoxOfBigAssm(m_Grid.GetTextMatrix(10,1) == "포함" ? 1 : 0);
	
	return;	
}


BOOL CDesignBMPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDesignBMPage::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		switch(next.row)
		{			
		case 1:
			strCombo.Add("평면치수 좌.우 기준");
			strCombo.Add("평면치수 거더 중앙 기준");
			m_Grid.SetComboString(strCombo);
			break;
		case 2:
			strCombo.Add("전개치수 좌우 적용");
			strCombo.Add("전개치수 직사각형 적용");
			strCombo.Add("전개치수 수축량, 좌우 적용");
			strCombo.Add("전개치수 수축량, 직사각형 적용");			
			m_Grid.SetComboString(strCombo);
			break;
		case 5: case 6: case 7:
			strCombo.Add("개소수, 중량 포함");
			strCombo.Add("개소수만 포함");
			m_Grid.SetComboString(strCombo);
			break;
		case 8:
			strCombo.Add("포함");
			strCombo.Add("포함 안함");
			m_Grid.SetComboString(strCombo);
			break;
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	return TStackedPage::PreTranslateMessage(pMsg);
}

void CDesignBMPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();

	return;
}
