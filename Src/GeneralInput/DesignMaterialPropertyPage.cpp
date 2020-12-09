// DesignMaterialPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignMaterialPropertyPage.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignMaterialPropertyPage dialog

CDesignMaterialPropertyPage::CDesignMaterialPropertyPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignMaterialPropertyPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignMaterialPropertyPage)
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_pDesignPageDlg = (CDesignPageDlg*)pParent;
}

void CDesignMaterialPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignMaterialPropertyPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignMaterialPropertyPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignMaterialPropertyPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)	
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignMaterialPropertyPage message handlers
void CDesignMaterialPropertyPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignMaterialPropertyPage::SetGridTitle()
{
	long nBridgeType = 0;
	
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - 구속 콘크리트 탄성계수비 nc 는 TU거더 옵션에서면 보여야 함

	long	nRows	=	bTUType ? 30 : 29;
	long	nCols	=	5;
	CString str		=	_T("");

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(3);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 220);
	m_Grid.SetColumnWidth(2, 130);
	m_Grid.SetColumnWidth(3, 100);

	
	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetTextMatrix(0,3,"설 정");
	m_Grid.SetTextMatrix(0,4,"단 위");
	m_Grid.SetMergeCol(0,0,2);

	//대분류
	m_Grid.SetTextMatrix(1,0,"콘크리트");
	m_Grid.SetMergeRow(1,3,0);

	m_Grid.SetTextMatrix(4,0,"철근");
	m_Grid.SetMergeRow(4,6,0);

	m_Grid.SetTextMatrix(7,0,"강재");
	m_Grid.SetMergeRow(7,8,0);

	if(bTUType)
	{
		m_Grid.SetTextMatrix(9, 0,"탄성계수");
		m_Grid.SetMergeRow(9,17,0);

		m_Grid.SetTextMatrix(18,0,"재료의 물리적 특성");
		m_Grid.SetMergeRow(18,23,0);

		m_Grid.SetTextMatrix(24,0,"허용응력");
		m_Grid.SetMergeRow(24,29,0);	
	}
	else
	{
		m_Grid.SetTextMatrix(9, 0,"탄성계수");
		m_Grid.SetMergeRow(9,16,0);

		m_Grid.SetTextMatrix(17,0,"재료의 물리적 특성");
		m_Grid.SetMergeRow(17,22,0);

		m_Grid.SetTextMatrix(23,0,"허용응력");
		m_Grid.SetMergeRow(23,28,0);	
	}


	//열 병합

	long	nRow	=	1;
	long	nUntil	=	24;	//허용응력 전까지의 행 개수

	for(long nR=1;nR < nUntil; nR++)
	{
		m_Grid.SetMergeCol(nR,1,2);
	}
	
	//콘크리트 중분류
	m_Grid.SetTextMatrix(nRow++,	1,	"fck");
	m_Grid.SetTextMatrix(nRow++,	1,	"fci");
	
	//철근 중분류
	m_Grid.SetTextMatrix(nRow++,	1,	"구속콘크리트 fck");
	m_Grid.SetTextMatrix(nRow++,	1,	"fy");
	m_Grid.SetTextMatrix(nRow++,	1,	"정착구 검토시 fsa");
	m_Grid.SetTextMatrix(nRow++,	1,	"바닥판 검토시 fsa");

	//강재 중분류
	m_Grid.SetTextMatrix(nRow++,	1,	"주부재");
	m_Grid.SetTextMatrix(nRow++,	1,	"부부재");

	//탄성계수 중분류
	m_Grid.SetTextMatrix(nRow++,	1,	"콘크리트 Ec");
	m_Grid.SetTextMatrix(nRow++,	1,	"콘크리트 Eci");
	m_Grid.SetTextMatrix(nRow++,	1,	"구속콘크리트 Ec");
	m_Grid.SetTextMatrix(nRow++,	1,	"철근 Es");
	m_Grid.SetTextMatrix(nRow++,	1,	"전단탄성계수 G");
	m_Grid.SetTextMatrix(nRow++,	1,	"강재 Est");
	m_Grid.SetTextMatrix(nRow++,	1,	"더미부재 Est");
	m_Grid.SetTextMatrix(nRow++,	1,	"바닥판 탄성계수비 n");
	if(bTUType == TRUE)
	{
		m_Grid.SetTextMatrix(nRow++,	1,	"구속콘크리트 탄성계수비 nc");
	}

	//재료의 물리적 특성 중분류
	m_Grid.SetTextMatrix(nRow++,	1,	"건조수축계수 εs");
	m_Grid.SetTextMatrix(nRow++,	1,	"선 팽창계수 α");
	m_Grid.SetTextMatrix(nRow++,	1,	"바닥판 콘크리트의 크리프 Φ1");
	m_Grid.SetTextMatrix(nRow++,	1,	"바닥판 콘크리트의 건조수축 Φ2");
	m_Grid.SetTextMatrix(nRow++,	1,	"콘크리트 포아송비");
	m_Grid.SetTextMatrix(nRow++,	1,	"바닥판 콘크리트와 강형과의 온도차");

	//허용응력 중분류
	long nRowTemp = nRow;
	m_Grid.SetTextMatrix(nRow, 1, "프리스트레스 도입직후 \n (크리프, 건조수축 손실전)");
	m_Grid.SetItemFormat(nRow, 1, DT_CENTER);
	m_Grid.SetMergeRow(nRow, nRow+1, 1);
	nRow++;

	//프리스트레스 소분류
	m_Grid.SetTextMatrix(nRowTemp,		2,	"압축응력");
	m_Grid.SetCellType(nRowTemp+1,		3, CT_COMBO);
	m_Grid.SetTextMatrix(nRowTemp+1,	2,	"인장응력");
	
	nRow++;
	nRowTemp	=	nRow;
	m_Grid.SetMergeRow(nRow,nRow+1,1);
	m_Grid.SetTextMatrix(nRow++,1,"설계 하중 작용시 (손실후)");
	
	//설계 하중 작용시 소분류
	m_Grid.SetTextMatrix(nRowTemp,		2,	"압축응력");

	m_Grid.SetCellType(nRowTemp+1,		3, CT_COMBO);
	m_Grid.SetTextMatrix(nRowTemp+1,	2,	"인장응력");

	nRow++;
	//콘크리트 허용 휨 인장응력
	m_Grid.SetMergeCol(nRow,1,2);
	m_Grid.SetTextMatrix(nRow++,1,"콘크리트 허용 휨 인장응력");
	
	//균열응력
	m_Grid.SetMergeCol(nRow,1,2);
	m_Grid.SetCellType(nRow,	3, CT_COMBO);
	m_Grid.SetTextMatrix(nRow,1,"균열응력");
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignMaterialPropertyPage::SetDataInit()
{
	SetGridTitle();

	CCalcData			*pData			= m_pDataManage->GetCalcData();
	long				nPrestressIdx	= pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	= m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);
	long				nRow			= 1;	

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - 구속 콘크리트 탄성계수비 nc 는 TU거더 옵션에서면 보여야 함
	
	CString szComboData = _T("");

	//콘크리트(대분류)
	
	////fck
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaCK);//1
	
	////fci
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaCi);//2

	////구속콘크리트 fck(TU)
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dBindSigmaCK);	// TU

	//철근(대분류)
	
	////fy
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaY);

	////정착구 검토 시 fsa
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaSA);

	////바닥판 검토 시 fsa
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaDeckSA);

	//강재(대분류)
	
	////주부재
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_szSwsJu);
	
	////부부재
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);	
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_szSwsBu);	
	
	//탄성계수(대분류)

	////콘크리트 Ec
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEc);

	////콘크리트 Eci
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEci);

	////구속콘크리트 Ec(TU)
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dBindEc);	

	////철근 Es
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEs);

	////전단 탄성계수 G
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dShearEs);

	////강재 Est
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEst);

	////더미부재 Est
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dDummyEst);

	////바닥판 탄성계수비 n
	m_Grid.SetTextMatrix(nRow++,3,"%g",(double)pData->DESIGN_MATERIAL.m_nN);

	////구속콘크리트 탄성계수비 nc(TU)
	if(bTUType == TRUE)
	{
		m_Grid.SetTextMatrix(nRow++,3,"%g",(double)pData->DESIGN_MATERIAL.m_nBindNc);	// TU
	}
	
	//재료의 물리적 특성(대분류)

	////건조수축계수 
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEpsilonS);
	
	////선 팽창 계수	
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dAlpha);
	
	////바닥판 콘크리트의 크리프 
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dCreep);
	
	////바닥판 콘크리트의 건조수축
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dCreep2);

	////콘크리트 포아송비
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_RatePoasong);

	////바닥판 콘크리트와 강형과의 온도차
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dTemparature);

	//허용응력(대분류)
	
	////프리스트레스 도입직후(중분류)

	//////압축응력
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);					
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow);

	//////인장응력
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow);

	////설계 하중 작용시(중분류)

	//////압축응력
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);					
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow );

	//////인장응력
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow);

	////콘크리트 허용 휨 인장응력
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_DeckConcStretchPow );

	////균열응력
	m_Grid.SetTextMatrix(nRow,3,pData->DESIGN_MATERIAL.m_strCrackPow);

	//단위 설정
	SetUnit();

	m_Grid.SetRedraw(TRUE,TRUE);
}
void CDesignMaterialPropertyPage::SetUnit()
{
	long nRow	=	0;
	long nMaxRow = 29;

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - 구속 콘크리트 탄성계수비 nc 는 TU거더 옵션에서면 보여야 함

	if(bTUType == TRUE)
	{
		nMaxRow = 30;
	}
	for(nRow = 1; nRow < nMaxRow; nRow++)
	{
		m_Grid.SetItemBkColour(nRow,4, RGBREADONLY);					
		m_Grid.SetItemState(nRow,4, m_Grid.GetItemState(nRow,4) | GVIS_READONLY);
	}
	
	//fck ~ 바닥판 검토시 fsa
	for(nRow = 1; nRow < 7; nRow++)
	{
		m_Grid.SetTextMatrix(nRow,	4,	"MPa");
	}
	
	//콘크리트 Ec ~ 더미부재 Est
	for(nRow = 9; nRow < 16; nRow++)
	{
		m_Grid.SetTextMatrix(nRow,		4,	"MPa");
	}

	long nRowChange = bTUType ? 23 : 22;
	m_Grid.SetTextMatrix( nRowChange,	4,"℃");	//바닥판 콘크리트와 강형과의 온도차

	//허용응력
	nRowChange = bTUType ? 24 : 23;
	for(nRow = nRowChange; nRow < nMaxRow; nRow++)
	{
		m_Grid.SetTextMatrix(nRow,	4,	"MPa");
	}
	
}
BOOL CDesignMaterialPropertyPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignMaterialPropertyPage::OnCellChanged(NMHDR* nmgv, LRESULT*)
{

}

void CDesignMaterialPropertyPage::SetConcreteEcSigmaCKByFck(double dFck)
{
	CCalcData *pCalcData				= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight				= m_pDataManage->GetSteelUWeight();	
	long				nPrestressIdx	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	= m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);

	double dWc = tokNPM3(pUWeight->m_dUWeightMugunCon)/10;
	double dEc = GetValueEcConc(dFck*10, dWc, TRUE);
	double dEs = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	
	pCalcData->DESIGN_MATERIAL.m_dEc		= dEc/10;
	pCalcData->DESIGN_MATERIAL.m_dSigmaCK	= dFck;

	double dn  = dEs/(dEc/10);
	pCalcData->DESIGN_MATERIAL.m_nN = (long)dn;
}

void CDesignMaterialPropertyPage::SetConcreteEcSigmaCKByFci(double dFci)
{
	CCalcData *pCalcData	= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight = m_pDataManage->GetSteelUWeight();	
	
	double dWc = tokNPM3(pUWeight->m_dUWeightMugunCon)/10;
	double dEc = GetValueEcConc(dFci*10, dWc, TRUE);
	
	pCalcData->DESIGN_MATERIAL.m_dEci		= dEc/10;
	pCalcData->DESIGN_MATERIAL.m_dSigmaCi	= dFci;
}

void CDesignMaterialPropertyPage::SetConcreteEcSigmaCKByFck2(double dFck)
{
	CCalcData *pCalcData				= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight				= m_pDataManage->GetSteelUWeight();	
	long				nPrestressIdx	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	= m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);
	
	double dWc = tokNPM3(pUWeight->m_dUWeightMugunCon)/10;
	double dEc = GetValueEcConc(dFck*10, dWc, TRUE);
	double dEs = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	
	pCalcData->DESIGN_MATERIAL.m_dBindEc		= dEc/10;
	pCalcData->DESIGN_MATERIAL.m_dBindSigmaCK	= dFck;
	
	double dbn = dEs/(dEc/10);
	pCalcData->DESIGN_MATERIAL.m_nBindNc = (long)dbn;
}

void CDesignMaterialPropertyPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	CCalcData *pCalcData = m_pDataManage->GetCalcData();
	long				nPrestressIdx	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData   = m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);

	if (nRow == 1 && nCol == 3)
	{
		double dFck = m_Grid.GetTextMatrixDouble(nRow, nCol);
		SetConcreteEcSigmaCKByFck(dFck);
		SetDataInit();
	}
	if (nRow == 2 && nCol == 3)
	{
		double dFci = m_Grid.GetTextMatrixDouble(nRow, nCol);
		SetConcreteEcSigmaCKByFci(dFci);
		SetDataInit();
	}
	if (nRow == 3 && nCol == 3)
	{
		double dFck = m_Grid.GetTextMatrixDouble(nRow, nCol);
		SetConcreteEcSigmaCKByFck2(dFck);
		SetDataInit();
	}
	if (nRow==9 && nCol==3)
	{
		double dEc = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dEs = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
		double dn  = dEs/(dEc);
		
		pCalcData->DESIGN_MATERIAL.m_dEc = dEc;
		pCalcData->DESIGN_MATERIAL.m_nN = (long) dn;
		SetDataInit();
	}
	if (nRow==11 && nCol==3)
	{
		double dEc = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dEst = pCalcData->DESIGN_MATERIAL.m_dEst;
		double dn  = dEst/(dEc);
		
		pCalcData->DESIGN_MATERIAL.m_dBindEc = dEc;
		pCalcData->DESIGN_MATERIAL.m_nBindNc = (long) dn;
		SetDataInit();
	}
	if (nRow==13 && nCol==3)
	{
		double dShearEs = m_Grid.GetTextMatrixDouble(nRow, nCol);
		pCalcData->DESIGN_MATERIAL.m_dShearEs = dShearEs;
		SetDataInit();
	}
	if (nRow==14 && nCol==3)
	{
		double dEc  = pCalcData->DESIGN_MATERIAL.m_dBindEc;
		double dEst = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dn	= dEst / dEc;
		pCalcData->DESIGN_MATERIAL.m_dEst	 = dEst;
		pCalcData->DESIGN_MATERIAL.m_nBindNc = (long)dn;
		SetDataInit();
	}
	if (nRow==15 && nCol==3)
	{
		double m_dDummyEst = m_Grid.GetTextMatrixDouble(nRow, nCol);
		pCalcData->DESIGN_MATERIAL.m_dDummyEst = m_dDummyEst;
		SetDataInit();
	}
}

void CDesignMaterialPropertyPage::SetDataSave()
{
	CCalcData		*pData		= m_pDataManage->GetCalcData();
	CADeckData		*pDeck		= m_pDataManage->GetDeckData();
	long			nRow		= 1;

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - 구속 콘크리트 탄성계수비 nc 는 TU거더 옵션에서면 보여야 함

	pData->DESIGN_MATERIAL.m_dSigmaCK		= m_Grid.GetTextMatrixDouble(nRow++,3);	//fck
	pData->DESIGN_MATERIAL.m_dSigmaCi		= m_Grid.GetTextMatrixDouble(nRow++,3);	//fci
	pData->DESIGN_MATERIAL.m_dBindSigmaCK	= m_Grid.GetTextMatrixDouble(nRow++,3);	//구속콘크리트 fck
	pData->DESIGN_MATERIAL.m_dSigmaY		= m_Grid.GetTextMatrixDouble(nRow++,3); //fy
	pData->DESIGN_MATERIAL.m_sSigmaY		= GetCharRebarMark(pData->DESIGN_MATERIAL.m_dSigmaY, FALSE);
	pData->DESIGN_MATERIAL.m_dSigmaSA		= m_Grid.GetTextMatrixDouble(nRow++,3); //정착구 검토시 fsa
	pData->DESIGN_MATERIAL.m_dSigmaDeckSA	= m_Grid.GetTextMatrixDouble(nRow++,3); //바닥판 검토시 fsa
	pData->DESIGN_MATERIAL.m_szSwsJu		= m_Grid.GetTextMatrix(nRow++,3);		//주부재
	pData->DESIGN_MATERIAL.m_szSwsBu		= m_Grid.GetTextMatrix(nRow++,3);		//부부재
	pData->DESIGN_MATERIAL.m_dEc			= m_Grid.GetTextMatrixDouble(nRow++,3);	//콘크리트 Ec
	pData->DESIGN_MATERIAL.m_dEci			= m_Grid.GetTextMatrixDouble(nRow++,3);	//콘크리트 Eci
	pData->DESIGN_MATERIAL.m_dBindEc		= m_Grid.GetTextMatrixDouble(nRow++,3);	//구속콘크리트 Ec
	pData->DESIGN_MATERIAL.m_dEs			= m_Grid.GetTextMatrixDouble(nRow++,3);	//철근 Es
	pData->DESIGN_MATERIAL.m_dShearEs		= m_Grid.GetTextMatrixDouble(nRow++,3);	//전단탄성계수 G
	pData->DESIGN_MATERIAL.m_dEst			= m_Grid.GetTextMatrixDouble(nRow++,3);	//강재 Est
	pData->DESIGN_MATERIAL.m_dDummyEst		= m_Grid.GetTextMatrixDouble(nRow++,3);	//더미부재 Est
	pData->DESIGN_MATERIAL.m_nN				= m_Grid.GetTextMatrixLong(nRow++,3);	//바닥판 탄성계수비 n
	if(bTUType == TRUE)
	{
		pData->DESIGN_MATERIAL.m_nBindNc		= m_Grid.GetTextMatrixLong(nRow++,3);	//구속콘크리트 탄성계수비 nc
	}
	
	pData->DESIGN_MATERIAL.m_dEpsilonS		= m_Grid.GetTextMatrixDouble(nRow++,3);	//건조수축계수
	pData->DESIGN_MATERIAL.m_dAlpha			= m_Grid.GetTextMatrixDouble(nRow++,3);	//선 팽창 계수
	pData->DESIGN_MATERIAL.m_dCreep			= m_Grid.GetTextMatrixDouble(nRow++,3);	//바닥판 콘크리트의 크리프
	pData->DESIGN_MATERIAL.m_dCreep2		= m_Grid.GetTextMatrixDouble(nRow++,3);	//바닥판 콘크리트의 건조수축
	pData->DESIGN_MATERIAL.m_RatePoasong	= m_Grid.GetTextMatrixDouble(nRow++,3);	//콘크리트 포아송비
	pData->DESIGN_MATERIAL.m_dTemparature	= m_Grid.GetTextMatrixDouble(nRow++,3);	//바닥판 콘크리트와 강형과의 온도차

	pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow		=	m_Grid.GetTextMatrix(nRow++,3);	//압축응력(프리스트레스 도입 시)
	pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow	=	m_Grid.GetTextMatrix(nRow++,3);	//인장응력(프리스트레스 도입 시)	
	pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow		=	m_Grid.GetTextMatrix(nRow++,3);	//압축응력(설계 하중 작용시)
	pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow	=	m_Grid.GetTextMatrix(nRow++,3);	//인장응력(설계 하차중 작용시)
	pData->DESIGN_MATERIAL.m_strAllow_DeckConcStretchPow	=	m_Grid.GetTextMatrix(nRow++,3);	//바닥판 콘크리트와 강형과의 온도차
	pData->DESIGN_MATERIAL.m_strCrackPow					=	m_Grid.GetTextMatrix(nRow++,3);	//균열응력

	// 코딩의 편의상 어쩔수 없이 이런짓을 하였음.(KSG)
	pDeck->m_SigmaCK = pData->DESIGN_MATERIAL.m_dSigmaCK/10;
	pDeck->m_SigmaY  = pData->DESIGN_MATERIAL.m_dSigmaY/10;
	pDeck->m_SigmaSA = pData->DESIGN_MATERIAL.m_dSigmaSA/10;
}

void CDesignMaterialPropertyPage::SetDataDefault()
{
	CCalcData *pData = m_pDataManage->GetCalcData();
	CSteel *pSteel = m_pDataManage->GetSteel();
	CSteelConc *pConc = m_pDataManage->GetSteelConc();
	CSteelSection *pSteelSection = m_pDataManage->GetSteelSection();

	long nIdx = pConc->GetIndex("S27");	//콘크리트
	if (m_pDataManage->GetGlobalOption()->GetDesignBridgeType()!=PLATE_GIR)
		nIdx = pConc->GetIndex("S40");

//	m_pDataManage->GetDeckData()->m_UWeightConcrete

	double	Wc = (m_pDataManage->GetDeckData()->m_UWeightConcrete>0) ? m_pDataManage->GetDeckData()->m_UWeightConcrete/1000 : 2.350;

	long nApplyYear = pSteel->GetApplyYear();

	pData->DESIGN_MATERIAL.m_dSigmaCK		= pConc->GetBasicStrength(nIdx);//m_pDataManage->GetGlobalOption()->GetDesignBridgeType()==0 ? 27.0 : 40.0;
	pData->DESIGN_MATERIAL.m_dSigmaCi		= 28;
	pData->DESIGN_MATERIAL.m_dSigmaY		= 400.0;
	pData->DESIGN_MATERIAL.m_sSigmaY		= "H";
	pData->DESIGN_MATERIAL.m_dSigmaSA		= 180;
	pData->DESIGN_MATERIAL.m_dSigmaDeckSA	= 160;
	pData->DESIGN_MATERIAL.m_szSwsJu		= pSteelSection->GetMaterialByMarkBuje(G_F_U);	//주부재
	pData->DESIGN_MATERIAL.m_szSwsBu		= pSteelSection->GetMaterialByMarkBuje(CP_W);	//부부재
	pData->DESIGN_MATERIAL.m_dEc			= GetValueEcConc(pData->DESIGN_MATERIAL.m_dSigmaCK/10,Wc,TRUE);
	pData->DESIGN_MATERIAL.m_dEci			= GetValueEcConc(pData->DESIGN_MATERIAL.m_dSigmaCi/10,Wc,TRUE);
	pData->DESIGN_MATERIAL.m_dBindEc		= 34000;
	pData->DESIGN_MATERIAL.m_dEs			= 200000.0;									//철근탄성계수
	pData->DESIGN_MATERIAL.m_dShearEs		= nApplyYear == APPLYYEAR2008 ? 81000.0 : 79000.0;			///< 전단 탄성계수
	pData->DESIGN_MATERIAL.m_dEpsilonS		= pConc->GetEpsilon(nIdx);						//건조수축계수
	pData->DESIGN_MATERIAL.m_dAlpha			= pConc->GetAlpha(nIdx);						//선팽창계수
	pData->DESIGN_MATERIAL.m_dCreep			= pConc->GetCreep1(nIdx);						//크리프1 
	pData->DESIGN_MATERIAL.m_dCreep2		= pConc->GetCreep2(nIdx);						//크리프2
	pData->DESIGN_MATERIAL.m_dTemparature	= pConc->GetTemperature(nIdx);				//온도차
	pData->DESIGN_MATERIAL.m_RatePoasong	= 0.167;
	nIdx = pSteel->GetIndex(pData->DESIGN_MATERIAL.m_szSwsJu);
	
	pData->DESIGN_MATERIAL.m_dEst         = nApplyYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//강재탄성계수
	pData->DESIGN_MATERIAL.m_dDummyEst    = nApplyYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//강재탄성계수
	pData->DESIGN_MATERIAL.m_nN           = 8;

	// TU거더교 추가
	pData->DESIGN_MATERIAL.m_dBindSigmaCK = 50;
	pData->DESIGN_MATERIAL.m_nBindNc      = 6;
	
	//허용응력
	pData->SetDefaultAllowDesignMaterials();
	SetDataInit();
}

double CDesignMaterialPropertyPage::GetConcreteEcByFck(double dFck)
{
	CSteelConc *pConc = m_pDataManage->GetSteelConc();

	for (long n=0; n< pConc->GetSize(); n++)
	{
		double fc = pConc->GetBasicStrength(n);

		if(dFck == pConc->GetBasicStrength(n))
		{
			return pConc->GetElasticModulus(n);
		}
	}

	return 0;
}

double CDesignMaterialPropertyPage::GetConcreteFckByEc(double dEc)
{
	if(dEc == 22000.0)		return 21.0;
	else if(dEc == 23500.0)	return 24.0;
	else if(dEc == 25000.0)	return 27.0;
	else if(dEc == 26000.0)	return 30.0;
	else if(dEc == 30000.0)	return 40.0;
	else if(dEc == 34000.0)	return 50.0;

	return 0;
}

BOOL CDesignMaterialPropertyPage::PreTranslateMessage(MSG* pMsg)
{
	CStringArray strCombo;
	CString strLength = _T("");

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - 구속 콘크리트 탄성계수비 nc 는 TU거더 옵션에서면 보여야 함

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		long nSelectedRow = next.row;
		if(bTUType == FALSE)
		{
			nSelectedRow +=1;
		}
		switch(nSelectedRow)
		{
		case 25:	//인장응력(프리스트레스 도입직후)
			{
				strCombo.Add("1.4");
				strCombo.Add("0.25 √(fci)");
				strCombo.Add("0.50 √(fci)");
				m_Grid.SetComboString(strCombo);
				break;
			}
		case 27:	//인장응력(설계 하중 작용 시)
			{
				CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
				long nStandardYear = pOpt->GetSteelStandardYear();

				if(nStandardYear == APPLYYEAR2010)	// #0028192 이슈 - 도로교 설계기준 2010 선택시 0.63 입력 가능토록 추가
					strCombo.Add("0.63 √(fck)");

				strCombo.Add("0.50 √(fck)");
				strCombo.Add("0.25 √(fck)");
				strCombo.Add("0");
				m_Grid.SetComboString(strCombo);
				break;
			}
		case 29:	//균열응력
			{
				strCombo.Add("0.63 √(fck)");
				strCombo.Add("0.54 √(fck)");
				strCombo.Add("0.47 √(fci)");
				m_Grid.SetComboString(strCombo);
				break;
			}
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	return TStackedPage::PreTranslateMessage(pMsg);
}
