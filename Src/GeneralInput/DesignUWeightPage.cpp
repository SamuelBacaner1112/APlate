// DesignUWeightPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignUWeightPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignUWeightPage dialog
CDesignUWeightPage::CDesignUWeightPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignUWeightPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignUWeightPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignUWeightPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignUWeightPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignUWeightPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignUWeightPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignUWeightPage message handlers
void CDesignUWeightPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignUWeightPage::SetDataDefault()
{
	m_pDataManage->GetSteelUWeight()->DataInit();
    SetDataInit();
}

void CDesignUWeightPage::SetDataInit()
{
	SetGridTitle();

	CSteelUWeight *pUWeight = m_pDataManage->GetSteelUWeight();	
	
	m_Grid.SetTextMatrix(1, 1, "%.1f", tokNPM3(pUWeight->m_dUWeightGangjae));
	m_Grid.SetTextMatrix(2, 1, "%.1f", tokNPM3(pUWeight->m_dUWeightAluminum));
	m_Grid.SetTextMatrix(3, 1, "%.1f", tokNPM3(pUWeight->m_dUWeightChulgunCon));
	m_Grid.SetTextMatrix(4, 1, "%.1f", tokNPM3(pUWeight->m_dUWeightMugunCon));
	m_Grid.SetTextMatrix(5, 1, "%.1f", tokNPM3(pUWeight->m_dUWeightCement));
	m_Grid.SetTextMatrix(6, 1, "%.1f", tokNPM3(pUWeight->m_dUWeightMokjae));
	m_Grid.SetTextMatrix(7, 1, "%.1f", tokNPM3(pUWeight->m_dUWeightYukchung));
	m_Grid.SetTextMatrix(8, 1, pUWeight->GetTypePave());
	m_Grid.SetTextMatrix(8, 2, "%.1f", tokNPM3(pUWeight->m_dUWeightAsphalt));
	m_Grid.SetTextMatrix(9, 1, "%.1f", tokNPM3(pUWeight->m_dUWeighGravel));
}
/*
double& CDesignUWeightPage::GetData(long row) const
{
	CSteelUWeight *pUWeight = m_pDataManage->GetSteelUWeight();	
	switch(row)
	{
	case 1 : return pUWeight->m_dUWeightGangjae;
	case 2 : return pUWeight->m_dUWeightAluminum;
	case 3 : return pUWeight->m_dUWeightChulgunCon;
	case 4 : return pUWeight->m_dUWeightMugunCon;
	case 5 : return pUWeight->m_dUWeightCement;
	case 6 : return pUWeight->m_dUWeightMokjae;
	case 7 : return pUWeight->m_dUWeightYukchung;
	case 8 : return pUWeight->m_dUWeightAsphalt;
	case 9 : return pUWeight->m_dUWeighGravel;
	}

	return pUWeight->m_dUWeightAsphalt;
}
*/
void CDesignUWeightPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	// 그리드 기본 셋팅 
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(ROW_COUNT_UWEIGHT);
	m_Grid.SetColumnCount(3);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);

	m_Grid.SetColumnWidth(0,170);
	m_Grid.SetColumnWidth(1,80);
	m_Grid.SetColumnWidth(2,80);

	m_Grid.SetRowHeightAll(20);				
	m_Grid.SetRowHeight(0, 20);	

	m_Grid.SetTextMatrix(0, 0, "재료");
	m_Grid.SetTextMatrix(0,	1,"단위중량( kN/m³)");		m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetTextMatrix(1, 0, "강재");					m_Grid.SetMergeCol(1, 1, 2);
	m_Grid.SetTextMatrix(2, 0, "알미늄");				m_Grid.SetMergeCol(2, 1, 2);
	m_Grid.SetTextMatrix(3, 0, "철근 콘크리트");		m_Grid.SetMergeCol(3, 1, 2);
	m_Grid.SetTextMatrix(4, 0, "무근 콘크리트");		m_Grid.SetMergeCol(4, 1, 2);
	m_Grid.SetTextMatrix(5, 0, "시멘트 모르터");		m_Grid.SetMergeCol(5, 1, 2);
	m_Grid.SetTextMatrix(6, 0, "목재");					m_Grid.SetMergeCol(6, 1, 2);
	m_Grid.SetTextMatrix(7, 0, "역청재(방수용)");		m_Grid.SetMergeCol(7, 1, 2);
	m_Grid.SetTextMatrix(8, 0, "포장");					m_Grid.SetCellType(8, 1, CT_COMBO);
	m_Grid.SetTextMatrix(9, 0, "자갈, 쇄석");			m_Grid.SetMergeCol(9, 1, 2);

	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL CDesignUWeightPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignUWeightPage::SetDataSave()
{
	UpdateData(FALSE);

	CSteelUWeight *pUWeight = m_pDataManage->GetSteelUWeight();	
	CCalcData	  *pData	= m_pDataManage->GetCalcData();

	CString strTypePave;
	pUWeight->m_dUWeightGangjae			= frkNPM3(m_Grid.GetTextMatrixDouble(1, 1));
	pUWeight->m_dUWeightAluminum		= frkNPM3(m_Grid.GetTextMatrixDouble(2, 1));
	pUWeight->m_dUWeightChulgunCon		= frkNPM3(m_Grid.GetTextMatrixDouble(3, 1));
	pUWeight->m_dUWeightMugunCon		= frkNPM3(m_Grid.GetTextMatrixDouble(4, 1));
	pUWeight->m_dUWeightCement			= frkNPM3(m_Grid.GetTextMatrixDouble(5, 1));
	pUWeight->m_dUWeightMokjae			= frkNPM3(m_Grid.GetTextMatrixDouble(6, 1));
	pUWeight->m_dUWeightYukchung		= frkNPM3(m_Grid.GetTextMatrixDouble(7, 1));

	strTypePave	= m_Grid.GetTextMatrix(8, 1);
	pUWeight->m_nTypePave				= pUWeight->GetTypePave(strTypePave);
	pUWeight->m_dUWeightAsphalt			= frkNPM3(m_Grid.GetTextMatrixDouble(8, 2));
	pUWeight->m_dUWeighGravel			= frkNPM3(m_Grid.GetTextMatrixDouble(9, 1));

	pData->DESIGN_UNIT_WEIGHT.m_dGangjae	= pUWeight->m_dUWeightGangjae;	
	pData->DESIGN_UNIT_WEIGHT.m_dAluminum	= pUWeight->m_dUWeightAluminum;
	pData->DESIGN_UNIT_WEIGHT.m_dAsphalt	= pUWeight->m_dUWeightAsphalt;
	pData->DESIGN_UNIT_WEIGHT.m_dCement		= pUWeight->m_dUWeightCement;	
	pData->DESIGN_UNIT_WEIGHT.m_dChulgun	= pUWeight->m_dUWeightChulgunCon;
	pData->DESIGN_UNIT_WEIGHT.m_dMokjae		= pUWeight->m_dUWeightMokjae;
	pData->DESIGN_UNIT_WEIGHT.m_dMugun		= pUWeight->m_dUWeightMugunCon;
	pData->DESIGN_UNIT_WEIGHT.m_dYukchung	= pUWeight->m_dUWeightYukchung;
	pData->DESIGN_UNIT_WEIGHT.m_dGravel		= pUWeight->m_dUWeighGravel;
}


BOOL CDesignUWeightPage::PreTranslateMessage(MSG* pMsg)
{
	CStringArray strCombo;

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.row == 8 && next.col==1)
		{
			strCombo.Add("아스팔트");
			strCombo.Add("모르터");
			strCombo.Add("LMC");
			m_Grid.SetComboString(strCombo);
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	return TStackedPage::PreTranslateMessage(pMsg);
}

void CDesignUWeightPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CSteelUWeight *pUWeight = m_pDataManage->GetSteelUWeight();	

	CCellID next		= m_Grid.GetFocusCell();
	CString	strTypePave	= _T("");
	if(next.row == 8 && next.col==1)
	{
		strTypePave	= m_Grid.GetTextMatrix(8, 1);
		pUWeight->m_nTypePave		= pUWeight->GetTypePave(strTypePave);
		pUWeight->m_dUWeightAsphalt	= pUWeight->GetPaveUnitWeight(strTypePave);
		SetDataInit();
	}
}
