// DesignPrestressPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "../APlateCalc/APlateCalc.h"
#include "DesignPrestressPropertyPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPropertyPage dialog
SHGRIDTITLE CDesignPrestressPropertyPage::m_gt[COL] = 
{
	{"종    류",							"%s",		170,	DT_RIGHT,	TRUE},
	{"종    류",							"%s",		170,	DT_RIGHT,	TRUE},
	{"설 정 값",							"%s",		200,	DT_RIGHT,	TRUE},
	{"단    위",							"%s",		100,	DT_RIGHT,	TRUE},
};

CDesignPrestressPropertyPage::CDesignPrestressPropertyPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignPrestressPropertyPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignPrestressPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignPrestressPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignPrestressPropertyPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignPrestressPropertyPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignPrestressPropertyPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPropertyPage message handlers
void CDesignPrestressPropertyPage::OnPreInitDialog()
{
	CXTResizeDialog::SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignPrestressPropertyPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	// 그리드 기본 셋팅 
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(ROW);
	m_Grid.SetColumnCount(COL);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.InputShGridTitle(m_gt, COL, TRUE);
	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 120);

	char *szUnit[] = { "", "", "mm²", "mm", "", "mm", "1/m", "1/rad", "mm", "MPa", "MPa", "MPa", "%", "", "mm", "mm", "MPa", "Mpa"};

	for(long n=0; n<ROW; n++)
	{
		m_Grid.SetTextMatrix(n, 3, szUnit[n]);
		m_Grid.SetItemState(n, 3, GVIS_READONLY);
		m_Grid.SetItemBkColour(n, 3, RGBREADONLY);
	}

	long	nRow		=	0;
	long	nRowMiddle	=	0;
	
	//대분류
	m_Grid.SetMergeCol(nRow,	0,	1);
	nRow++;
	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS강선 호칭");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS강선 공칭단면적");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS강선 직경");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"덕트의 형태");

	m_Grid.SetMergeCol(nRow	,	0	,1);
	m_Grid.SetTextMatrix(nRow++,	0,	"덕트의 직경");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"파상 마찰 계수");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"곡률 마찰 계수");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"정착구 활동량");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"탄성계수 Es");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"인장강도(fpu)");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"항복강도(fpy)");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"평균상대습도");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS 강재의 종류에 따른 계수(γp)");

	m_Grid.SetMergeRow(nRow,nRow+1,0);
	
	nRowMiddle	=	nRow;

	m_Grid.SetTextMatrix(nRow++,	0,	"쉬스관");
	
	++nRow;
	m_Grid.SetTextMatrix(nRow,		1,	"선택입력");
	m_Grid.SetTextMatrix(nRow++,	0,	"Jack에 의한 최대 긴장응력");
	m_Grid.SetTextMatrix(nRow,		1,	"수동입력");
	m_Grid.SetTextMatrix(nRow,		0,	"Jack에 의한 최대 긴장응력");
	
	m_Grid.SetMergeRow(nRow-1, nRow, 0, TRUE);
	//중분류
	m_Grid.SetTextMatrix(nRowMiddle++,	1,	"폭 (B)");
	m_Grid.SetTextMatrix(nRowMiddle,	1,	"높이 (H)");

	m_Grid.SetCellType(1,	2,	CT_COMBO);
	m_Grid.SetCellType(4,	2,	CT_COMBO);
	m_Grid.SetCellType(13,	2,	CT_COMBO);
	m_Grid.SetCellType(16,	2,	CT_COMBO);

	//HIDDEN
	//m_Grid.SetRowHeight(5, 0);	

}

void CDesignPrestressPropertyPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CCalcData	*pData	= m_pDataManage->GetCalcData();
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	if(nRow == 1 && nCol == 2)
	{
		CString strType = m_Grid.GetTextMatrix(1, 2);
		CSteelPrestressPlate *pPreStress	= m_pDataManage->GetSteelPrestress();
		CSteelPrestressData *pPSData= pPreStress->GetSteelPrestressData(strType);
		if(!pPSData) return;
		pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName = strType;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dArea				= pPSData->m_dArea;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter			= pPSData->m_dDiameter;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength	= GetValueUnitChange(pPSData->GetTensionStrength(), _T("kgf/cm²"), _T("MPa"));	///< 인장강도
		pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength		= GetValueUnitChange(pPSData->GetYieldStrength(), _T("kgf/cm²"), _T("MPa"));	///< 항복강도
		SetDataInit();
	}
	else if(nRow == 4 && nCol == 2)
	{
		CString strType = m_Grid.GetTextMatrix(4, 2);
		long nIndex = GetLondDuctType(strType);
		pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType = nIndex;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction = GetWobbleFriction(nIndex);
		pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction = GetCurvatureFriction(nIndex);
		SetDataInit();
	}
	else if(nRow == 16)
	{
		SetDataSave();
		SetDataInit();
	}
	else if (nRow==9 && nCol==2)
	{
		double dEs = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dEc = pData->DESIGN_MATERIAL.m_dEc;
		double dn  = dEs / dEc;	

		pData->DESIGN_STEELPRESTRESS_DATA.m_dEs = dEs;
		pData->DESIGN_MATERIAL.m_nN = (long)dn;
		SetDataInit();
	}
	else if(nRow == 17)
	{
		CString strValue = m_Grid.GetTextMatrix(nRow, 2);
		
		for(long nIdx = 0; nIdx < strValue.GetLength(); nIdx++)
		{
			if(!isdigit(strValue.GetAt(nIdx)) && strValue.GetAt(nIdx) != '.')
			{
				AfxMessageBox("숫자열을 입력해주세요!", MB_OK, MB_ICONWARNING);
				m_Grid.SetTextMatrix(nRow, 2, "0");
				return;
			}
		}
	}
	

	SetDataSave();

	
}

void CDesignPrestressPropertyPage::SetDataDefault()
{
	CCalcData			*pData = m_pDataManage->GetCalcData();
	CSteelPrestressPlate		*pPrestress	= m_pDataManage->GetSteelPrestress();
	CSteelPrestressData	*pPSData	= pPrestress->GetSteelPrestressData(12);

	//사용 Pressstress 강선의 번호
	pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx	= 12;												

	//사용 Pressstress 강선의 호칭
	pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName	= pPSData->m_szSymbol + " " + pPSData->m_szName;	

	//공칭 단면적
	pData->DESIGN_STEELPRESTRESS_DATA.m_dArea	= pPSData->m_dArea;
	
	//직경			
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter	= pPSData->m_dDiameter;			
	
	//덕트형태 - 아연도금 금속쉬스
	pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType	= 1;							
	
	//덕트 직경
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter	= 22.4;							
	
	//파상마찰계수
	pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction	= 0.0050;						
	
	//곡률마찰계수
	pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction	= 0.25;							
	
	//정착치 활동량
	pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip	= 6.0;							
	
	//탄성계수
	pData->DESIGN_STEELPRESTRESS_DATA.m_dEs	= 200000.0;			

	//인장하중
	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionLoad	=  GetValueUnitChange(pPSData->m_dTensionLoad, _T("kgf"), _T("N"));

	//연신하중
	pData->DESIGN_STEELPRESTRESS_DATA.m_dLoadOfElongation	= GetValueUnitChange(pPSData->m_dLoadOfElongation, _T("kgf"), _T("N"));

	//인장강도
	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength	= GetValueUnitChange(pPSData->GetTensionStrength(), _T("kgf/cm²"), _T("MPa"));

	//항복강도
	pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength	= GetValueUnitChange(pPSData->GetYieldStrength(), _T("kgf/cm²"), _T("MPa"));
	
	//평균상대습도
	pData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity	= 70.0;

	//PS 강재의 종류에 따른 계수
	pData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP	=	0.28;

	//쉬스관 폭
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW	=	190;

	//쉬스관 높이
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH	=	70;

	//Jack에 의한 최대 긴장 응력
	pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax	=	"0.90·fpy";

	SetDataInit();
}

void CDesignPrestressPropertyPage::SetDataInit()
{
	SetGridTitle();

	CCalcData			*pData			=	m_pDataManage->GetCalcData();
	long				nPrestressIdx	=	pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	=	m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);

	if(!pSteelPSData) return;

	m_Grid.SetTextMatrix(1,		2,			pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName);
	m_Grid.SetTextMatrix(2,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dArea);
	m_Grid.SetTextMatrix(3,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter);
	m_Grid.SetTextMatrix(4,		2,			GetStrDuctType(pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType));
	m_Grid.SetTextMatrix(5,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter);
	m_Grid.SetTextMatrix(6,		2,	"%.4f",	pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction);
	m_Grid.SetTextMatrix(7,		2,	"%.2f",	pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction);
	m_Grid.SetTextMatrix(8,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	m_Grid.SetTextMatrix(9,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dEs);
	m_Grid.SetTextMatrix(10,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength);
	m_Grid.SetTextMatrix(11,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength);
	m_Grid.SetTextMatrix(12,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity);
	
	m_Grid.SetTextMatrix(13,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP);
	m_Grid.SetTextMatrix(14,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW);
	m_Grid.SetTextMatrix(15,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH);
	
	CString strJackMax = pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax;
	
	if(strJackMax.Find("fpu") < 0 && strJackMax.Find("fpy") < 0) //사용자 정의 
	{
		m_Grid.SetTextMatrix(16, 2, "사용자 정의");
		m_Grid.SetTextMatrix(17, 2, pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax);
		m_Grid.SetItemState(17, 2, m_Grid.GetItemState(17, 2) & ~GVIS_READONLY);
	}
	else
	{
		m_Grid.SetTextMatrix(16, 2, pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax);
		m_Grid.SetTextMatrix(17, 2, "");
		m_Grid.SetItemState(17, 2, m_Grid.GetItemState(17, 2) |  GVIS_READONLY);
	}

	m_Grid.SetRedraw(TRUE,TRUE);

}

void CDesignPrestressPropertyPage::SetDataSave()
{
	CCalcData			*pData			= m_pDataManage->GetCalcData();
	CSteelPrestressPlate		*pPrestress		= m_pDataManage->GetSteelPrestress();
	CString				szText			= m_Grid.GetTextMatrix(1, 2);
	long				nPrestressIdx	= pPrestress->GetIndexByPrestressName(szText);
	CSteelPrestressData *pSteelPSData	= pPrestress->GetSteelPrestressData(nPrestressIdx);

	if(!pSteelPSData) return;

	
	pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx	= nPrestressIdx;
	
	//PS강선 호칭
	pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName	= m_Grid.GetTextMatrix(1, 2);

	//PS강선 공칭단면적
	pData->DESIGN_STEELPRESTRESS_DATA.m_dArea	= m_Grid.GetTextMatrixDouble(2, 2);
	
	//PS강선 직경
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter	= m_Grid.GetTextMatrixDouble(3, 2);

	//덕트의 형태
	CString strType = m_Grid.GetTextMatrix(4, 2);
	pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType	= GetLondDuctType(strType);

	//덕트의 직경
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter	= m_Grid.GetTextMatrixDouble(5, 2);

	//파상 마찰 계수
	pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction	= m_Grid.GetTextMatrixDouble(6, 2);

	//곡률 마찰 계수
	pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction	= m_Grid.GetTextMatrixDouble(7, 2);

	//정착구 활동량
	pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip	= m_Grid.GetTextMatrixDouble(8, 2);

	//탄성계수 Es
	pData->DESIGN_STEELPRESTRESS_DATA.m_dEs	= m_Grid.GetTextMatrixDouble(9, 2);

	//극한 강도
	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength	= m_Grid.GetTextMatrixDouble(10, 2);

	//항복강도
	pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength	= m_Grid.GetTextMatrixDouble(11, 2);

	//평균상대습도
	pData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity	= m_Grid.GetTextMatrixDouble(12, 2);

	//PS 강재의 종류에 따른 계수
	pData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP	= m_Grid.GetTextMatrixDouble(13, 2);

	//쉬스관 폭
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW	= m_Grid.GetTextMatrixDouble(14, 2);

	//쉬스관 높이
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH	= m_Grid.GetTextMatrixDouble(15, 2);

	//Jack에 의한 최대 긴장응력
	CString strJackMax = m_Grid.GetTextMatrix(16, 2);

	pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax	= m_Grid.GetTextMatrix(strJackMax == "사용자 정의" ? 17 : 16, 2);
}

BOOL CDesignPrestressPropertyPage::PreTranslateMessage(MSG* pMsg)
{
	CStringArray strCombo;
	CCalcData *pData = m_pDataManage->GetCalcData();
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		switch(next.row)
		{			
		case 1:
			{
				CSteelPrestressPlate *pSteelPS = m_pDataManage->GetSteelPrestress();
				if(!pSteelPS) break;
				for(long n=0; n<pSteelPS->GetSizePsGangsun(); n++)
				{
					CSteelPrestressData *pSteelPSData = pSteelPS->GetSteelPrestressData(n);
					strCombo.Add(pSteelPSData->m_szSymbol + " " + pSteelPSData->m_szName);
				}

				m_Grid.SetComboString(strCombo);
				break;
			}
		case 4:
			{
				strCombo.Add("금속쉬스");
				strCombo.Add("아연도금 금속쉬스");
				strCombo.Add("아스팔트 또는 그리스로코팅");
				strCombo.Add("아연도금된 강성덕트");

				m_Grid.SetComboString(strCombo);
				break;
			}
		case 13:
			{
				strCombo.Add("0.4");
				strCombo.Add("0.28");
				strCombo.Add("0.55");

				m_Grid.SetComboString(strCombo);
				break;

			}
		case 16:
			{
				pData->GetDefaultConstPSf(strCombo);
				m_Grid.SetComboString(strCombo);
				break;
			}
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return TStackedPage::PreTranslateMessage(pMsg);
}

CString CDesignPrestressPropertyPage::GetStrDuctType(long nType)
{
	if(nType==0) return _T("금속쉬스");
	if(nType==1) return _T("아연도금 금속쉬스");
	if(nType==2) return _T("아스팔트 또는 그리스로코팅");
	if(nType==3) return _T("아연도금된 강성덕트");
	return _T("");
}

long CDesignPrestressPropertyPage::GetLondDuctType(CString strDuctType)
{
	if(strDuctType == _T("금속쉬스"))						return 0;
	if(strDuctType == _T("아연도금 금속쉬스"))				return 1;
	if(strDuctType == _T("아스팔트 또는 그리스로코팅"))		return 2;
	if(strDuctType == _T("아연도금된 강성덕트"))			return 3;

	return -1;
}

///< 곡률마찰계수
double CDesignPrestressPropertyPage::GetCurvatureFriction(long nIndex)
{
	if(nIndex == 0) return 0.30;
	if(nIndex == 1) return 0.25;
	if(nIndex == 2) return 0.30;
	if(nIndex == 3) return 0.25;
	return 0;
}

///< 파상마찰계수
double CDesignPrestressPropertyPage::GetWobbleFriction(long nIndex)
{
	if(nIndex == 0) return 0.0066;
	if(nIndex == 1) return 0.0050;
	if(nIndex == 2) return 0.0066;
	if(nIndex == 3) return 0.0007;
	return 0;
}
