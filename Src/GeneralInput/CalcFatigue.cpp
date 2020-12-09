// CalcFatigue.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcFatigue dialog


CCalcFatigue::CCalcFatigue(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcFatigue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcFatigue)
	m_nRoadType = 0;
	m_nStruType = 0;
	m_nCheckType = 0;
	m_sCount1 = _T("");
	m_sCount2 = _T("");
	m_sCount3 = _T("");
	//}}AFX_DATA_INIT
}


void CCalcFatigue::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcFatigue)
	DDX_Control(pDX, IDC_S_P_STRESS_COMBO2, m_cbST);
	DDX_Control(pDX, IDC_S_P_STRESS_COMBO1, m_cbRT);
	DDX_Control(pDX, IDC_S_P_STRESS_COMBO3, m_ComboBox1);
	DDX_GridControl(pDX, IDC_S_P_STRESS_GRID1, m_Grid1);
	DDX_GridControl(pDX, IDC_S_P_STRESS_GRID2, m_Grid2);
	DDX_GridControl(pDX, IDC_S_P_STRESS_GRID3, m_Grid3);
	DDX_CBIndex(pDX, IDC_S_P_STRESS_COMBO1, m_nRoadType);
	DDX_CBIndex(pDX, IDC_S_P_STRESS_COMBO2, m_nStruType);
	DDX_CBIndex(pDX, IDC_S_P_STRESS_COMBO3, m_nCheckType);
	DDX_Text(pDX, IDC_S_P_STRESS_EDIT201, m_sCount1);
	DDX_Text(pDX, IDC_S_P_STRESS_EDIT202, m_sCount2);
	DDX_Text(pDX, IDC_S_P_STRESS_EDIT203, m_sCount3);
	DDX_Control(pDX, IDC_S_P_STRESS_EDIT201, m_ctrlEdit1);
	DDX_Control(pDX, IDC_S_P_STRESS_EDIT202, m_ctrlEdit2);
	DDX_Control(pDX, IDC_S_P_STRESS_EDIT203, m_ctrlEdit3);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_S_P_STRESS_TOTEXT, m_btnOutText);
	DDX_Control(pDX, IDC_S_P_STRESS_TOEXCEL, m_btnOutExcel);
	DDX_Control(pDX, IDC_S_P_STRESS_TOPRINT, m_btnPrint);
}


BEGIN_MESSAGE_MAP(CCalcFatigue, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcFatigue)
	ON_CBN_SELCHANGE(IDC_S_P_STRESS_COMBO1, OnSelchangeStressCombo1)
	ON_CBN_SELCHANGE(IDC_S_P_STRESS_COMBO2, OnSelchangeStressCombo2)
	ON_CBN_SELCHANGE(IDC_S_P_STRESS_COMBO3, OnSelchangeStressCombo3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcFatigue message handlers
void CCalcFatigue::OnPreInitDialog()
{
	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();

	m_cbRT.AddString("고속도로,국도및 주간선도로");
	m_cbRT.AddString("기타도로");
	m_cbST.AddString("다재하경로구조");
	m_cbST.AddString("단재하경로구조");	
	
	m_nRoadType = pData->m_nFatigueRoadType;
	m_nStruType = pData->m_nFatigueStructureType;

	m_cbRT.SetCurSel(m_nRoadType);
	m_cbST.SetCurSel(m_nStruType);

	SetComboData();

	SetResize(IDC_S_P_STRESS_STATIC, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_F_S_STATIC7, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_S_STATIC8, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_P_STRESS_COMBO1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_P_STRESS_COMBO2, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_F_S_STATIC1, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_F_S_STATIC2, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_F_S_STATIC3, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_F_S_STATIC4, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_F_S_STATIC5, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_F_S_STATIC6, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_S_P_STRESS_EDIT201, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_S_P_STRESS_EDIT202, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_S_P_STRESS_EDIT203, SZ_TOP_CENTER, SZ_TOP_CENTER);

	SetResize(IDC_S_P_STRESS_STATIC2, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_MOMENT_TEXT, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC7, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC8, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC9, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_S_P_STRESS_COMBO3, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_S_P_STRESS_GRID1, SZ_TOP_CENTER, SZ_MIDDLE_RIGHT);

	SetResize(IDC_STATIC_SHEAR_TEXT, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC10, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC11, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC12, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_13, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_14, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_15, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_16, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_17, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_18, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_19, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC_20, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_STATIC_21, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_S_P_STRESS_GRID2, SZ_MIDDLE_CENTER, SZ_MIDDLE_RIGHT);
	SetResize(IDC_STATIC_22, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_S_P_STRESS_GRID3, SZ_MIDDLE_CENTER, SZ_BOTTOM_RIGHT);

	SetResize(IDC_S_P_STRESS_TOTEXT, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_S_P_STRESS_TOEXCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_S_P_STRESS_TOPRINT, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	UpdateData(TRUE);
}

void CCalcFatigue::SetDataDefault()
{
	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp *pDB   = m_pStd->GetBridge();

	if(pDB->GetGirdersu()>=2)	
		pData->m_nFatigueStructureType = 0;
	else
		pData->m_nFatigueStructureType = 1;

	m_nRoadType = pData->m_nFatigueRoadType;
	m_nStruType = pData->m_nFatigueStructureType;

	m_cbRT.SetCurSel(m_nRoadType);
	m_cbST.SetCurSel(m_nStruType);
}

void CCalcFatigue::SetDataSave()
{
	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();
	pData->m_nFatigueRoadType		= m_nRoadType;
	pData->m_nFatigueStructureType	= m_nStruType;
}
void CCalcFatigue::DrawInputDomyunView(BOOL bZoomAll)
{
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		

	CDomyun Dom(pDom);
	Dom.SetScaleDim(500);

	StressCalc.OutputOfFatigueCheckPos(&Dom, TRUE, 3);
	
	double dRatio=0;
	long   nEleIdx=0, nNodeIdx=0;
	nNodeIdx = ModelCalc.GetMaxFatigueIndex(m_nCheckType+1, nEleIdx, dRatio, 1);  

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);

}

void CCalcFatigue::OnSelchangeStressCombo1() 
{
	UpdateData(TRUE);
	SetDataInit();
}

void CCalcFatigue::OnSelchangeStressCombo2() 
{
	UpdateData(TRUE);
	SetDataInit();
}

void CCalcFatigue::OnSelchangeStressCombo3()
{
	UpdateData(TRUE);
	SetDataInit();

	DrawInputDomyunView(TRUE);
}

void CCalcFatigue::SetGridTitle()
{
	SetGridTitleMoment();
	SetGridTitleShear();
}

void CCalcFatigue::SetGridTitleMoment()
{
	long nRows = 8;
	long nCols = 11;

	m_Grid1.DeleteAllItems();
	m_Grid1.SetRowCount(nRows);
	m_Grid1.SetColumnCount(nCols);

	m_Grid1.SetTextBkColor(RGB(225, 250, 250));	
	m_Grid1.SetFixedBkColor(m_Grid1.GetFixedBkColor());
    m_Grid1.SetRowResize(TRUE);
    m_Grid1.SetColumnResize(TRUE);
	m_Grid1.SetGridLines(GVL_BOTH);
	m_Grid1.SetFixedRowCount(2);
	m_Grid1.SetFixedColumnCount(1);
	m_Grid1.SetRowHeightAll(20);
	m_Grid1.SetEditable(FALSE);
	m_Grid1.SetListMode(FALSE);

	m_Grid1.SetColumnWidth(0, 30);
	m_Grid1.SetColumnWidth(1, 65);
	m_Grid1.SetColumnWidth(2, 65);
	m_Grid1.SetColumnWidth(3, 85);
	m_Grid1.SetColumnWidth(4, 55);
	m_Grid1.SetColumnWidth(5, 65);
	m_Grid1.SetColumnWidth(6, 65);
	m_Grid1.SetColumnWidth(7, 40);
	m_Grid1.SetColumnWidth(8, 65);
	m_Grid1.SetColumnWidth(9, 65);
	m_Grid1.SetColumnWidth(10, 40);

	m_Grid1.SetMergeRow(0, 1, 0);
	m_Grid1.SetMergeRow(0, 1, 3);
	m_Grid1.SetMergeRow(0, 1, 4);
	m_Grid1.SetMergeRow(0, 1, 10);
	m_Grid1.SetMergeCol(0, 1, 2);
	m_Grid1.SetMergeCol(0, 5, 6);
	m_Grid1.SetMergeCol(0, 8, 9);

	m_Grid1.SetTextMatrix(0, 0, "구분");
	m_Grid1.SetTextMatrix(0, 1, "ΔM (kN m)");
	m_Grid1.SetTextMatrix(1, 1, "DB 하중");
	m_Grid1.SetTextMatrix(1, 2, "DL 하중");
	m_Grid1.SetTextMatrix(0, 3, "Iv (mm4)");
	m_Grid1.SetTextMatrix(0, 4, "y (mm)");
	m_Grid1.SetTextMatrix(0, 5, "Δf (MPa)");
	m_Grid1.SetTextMatrix(1, 5, "DB 하중");
	m_Grid1.SetTextMatrix(1, 6, "DL 하중");
	m_Grid1.SetTextMatrix(0, 7, "응력");
	m_Grid1.SetTextMatrix(1, 7, "범주");
	m_Grid1.SetTextMatrix(0, 8, "fsr (MPa)");
	m_Grid1.SetTextMatrix(1, 8, "DB 하중");
	m_Grid1.SetTextMatrix(1, 9, "DL 하중");
	m_Grid1.SetTextMatrix(0, 10, "비고");

}

void CCalcFatigue::SetGridTitleShear()
{
	for (long nUpper = 0; nUpper < 2; nUpper++)  
	{
		CGridCtrl *pGrid = nUpper ? &m_Grid2 : &m_Grid3;

		long nRows = 3;
		long nCols = 13;

		pGrid->DeleteAllItems();
		pGrid->SetRowCount(nRows);
		pGrid->SetColumnCount(nCols);

		pGrid->SetTextBkColor(RGB(225, 250, 250));
		pGrid->SetFixedBkColor(pGrid->GetFixedBkColor());
		pGrid->SetRowResize(TRUE);
		pGrid->SetColumnResize(TRUE);
		pGrid->SetGridLines(GVL_BOTH);
		pGrid->SetEditable(FALSE);
		pGrid->SetListMode(FALSE);
		pGrid->SetTextBkColor(RGB(225, 250, 250));	

		pGrid->SetFixedRowCount(1);
		pGrid->SetFixedColumnCount(1);
		pGrid->SetRowHeight(0, 27);
		pGrid->SetRowHeight(1, 17);

		pGrid->SetColumnWidth(0, 30);
		pGrid->SetColumnWidth(1, 60);
		pGrid->SetColumnWidth(2, 45);
		pGrid->SetColumnWidth(3, 45);
		pGrid->SetColumnWidth(4, 50);
		pGrid->SetColumnWidth(5, 45);
		pGrid->SetColumnWidth(6, 55);
		pGrid->SetColumnWidth(7, 80);
		pGrid->SetColumnWidth(8, 45);
		pGrid->SetColumnWidth(9, 45);
		pGrid->SetColumnWidth(10, 65);
		pGrid->SetColumnWidth(11, 65);
		pGrid->SetColumnWidth(12, 40);

		for (long nCol = 1; nCol < nCols - 1; nCol++)
			pGrid->SetItemFormat(0, nCol, DT_CENTER|DT_VCENTER);

		pGrid->SetTextMatrix(0, 0, "구분");
		pGrid->SetTextMatrix(1, 0, "DB");
		pGrid->SetTextMatrix(2, 0, "DL");
		pGrid->SetItemText(0, 1, "ΔS\n(kN)");
		pGrid->SetItemText(0, 2, nUpper ? "tu\n(㎜)" : "tl\n(㎜)");
		pGrid->SetItemText(0, 3, "B\n(mm)");
		pGrid->SetItemText(0, 4, "A\n(mm²)");
		pGrid->SetItemText(0, 5, nUpper ? "Yvsu\n(mm)" : "Yvsl\n(mm)");
		pGrid->SetItemText(0, 6, "Q\n(mm³)");
		pGrid->SetItemText(0, 7, "Iv\n(mm⁴)");
		pGrid->SetItemText(0, 8, "s\n(㎜)");
		pGrid->SetItemText(0, 9, "Σa\n(mm)");
		pGrid->SetItemText(0, 10, "Δυ\n(MPa)");
		pGrid->SetItemText(0, 11, "υsr\n(MPa)");
		pGrid->SetTextMatrix(0, 12, "비고");
	}
}

void CCalcFatigue::SetComboData()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nDanSu = min(pDB->m_nQtyJigan, 3);
	CString str="";

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_S_P_STRESS_COMBO3);
	pCombo->ResetContent();

	for (long i=0; i<nDanSu; i++)  
	{
		str.Format("단면 %d", i + 1);
		pCombo->AddString(str);
	}
	pCombo->SetCurSel(0);
}

void CCalcFatigue::SetDataInit()
{
	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();

	SetGridTitle();
	
	if(pData->m_bReadFrameModelingOutput)
	{
		SetDataInitMoment();
		SetDataInitShear();
	}

	SetDataSave();
}

void CCalcFatigue::SetDataInitMoment()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CFEMManage  *pManange = m_pStd->m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	CString sCount[][3] = {
		{ "200만회", "50만회", "200만회 이상" },
		{ "10만회", "10만회", "50만회 이상" }
	};
	int nAllow[][8] = {
		{ 442, 260, 168, 168, 351, 203, 168, 168 },
		{ 344, 203, 126, 112, 274, 161, 112, 112 },
		{ 274, 161, 101,  84, 218, 126,  77,  77 },
		{ 250, 147,  91,  70, 196, 112,  70,  63 },
		{ 196, 112,  70,  49, 154,  91,  56,  35 },
		{ 154,  91,  56,  31, 119,  70,  42,  16 },
		{ 112,  64,  40,  18,  84,  49,  28,   9 },
		{ 105,  84,  63,  56,  84,  63,  49,  42 }
	};

	if (m_nRoadType >= 0 && m_nRoadType <= 1)
	{
		m_sCount1 = sCount[m_nRoadType][0];
		m_sCount2 = sCount[m_nRoadType][1];
		m_sCount3 = sCount[m_nRoadType][2];
	}

	double dRatio=0, Yvsu=0, Yvsl=0, Iv=0, DB=0, DL=0, dThickFlUP=0, dThickFlDN=0;
	long nEleIdxDB=0, nNodeIdxDB=0;
	long nG=0, nGSu = pDB->GetGirdersu();
	double y=0, fDB=0, fDL=0, aDB=0, aDL=0;
	long nHStiffDan=0;
	double dHeightWStiff=0, dSta=0;
	CPlateBasicIndex *pBx = NULL;

	// DB일때 : 현재는 절점 위치만 나옴.
	nNodeIdxDB = ModelCalc.GetMaxFatigueIndex(m_nCheckType + 1, nEleIdxDB, dRatio, 1);  

	if(nNodeIdxDB >= 0)
	{
		double	dDBMax	= StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MAX, nEleIdxDB, dRatio);
		double	dDBMin	= StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MIN, nEleIdxDB, dRatio);
		DB = frkNM(StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MAX, nEleIdxDB, dRatio) -
					StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MIN, nEleIdxDB, dRatio));

		DL = frkNM(StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DL_MAX, nEleIdxDB, dRatio) -
					 StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DL_MIN, nEleIdxDB, dRatio));

		nG    = ModelCalc.GetGirderNoByJointID(nNodeIdxDB);
		pBx   = m_pStd->m_pCalcStd->GetBxByJoint(pManange->GetJoint((unsigned short)nNodeIdxDB));
		dSta  = pBx->GetStation();

		CVStiff    *pVStiff = pBx->GetSection()->GetVStiff();
		CWebHStiff *pWStiff = pBx->GetWebHStiff(); 
		CHBracingGusset *pHGusset = pBx->GetHBracingGusset();
		StressCalc.GetSectionStress(nG, dSta);

		Yvsu = StressCalc.GetMapValueSecStress("Yvsu");
		Yvsl = StressCalc.GetMapValueSecStress("Yvsl");
		dThickFlUP = pDB->GetGirder(nG)->GetThickFactByStation(dSta, G_F_U);
		dThickFlDN = pDB->GetGirder(nG)->GetThickFactByStation(dSta, G_F_L);
		
		if(m_nCheckType==0 || m_nCheckType==2)	nHStiffDan = 0;
		else					nHStiffDan = MAX(pWStiff->GetUpperDan(TRUE)+pWStiff->GetLowerDan(TRUE)
			                                ,pWStiff->GetUpperDan(FALSE)+pWStiff->GetLowerDan(FALSE)) -1;
		dHeightWStiff		 =  pWStiff->GetHeightHStiffBySta(nHStiffDan, TRUE, dSta);
		if(dHeightWStiff==0)	pWStiff->GetHeightHStiffBySta(nHStiffDan, FALSE, dSta);

		if(StressCalc.IsPositive(ELE_MOMENT3, nG, dSta))
		{
			ModelCalc.GetSectionCoefficient(nG, dSta);  // 정모멘트
			Iv = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
		}
		else
		{
			StressCalc.GetSectionStress(nG, dSta);
			Iv = StressCalc.GetMapValueSecStress("Irv");  // 부모멘트
		}

		double dHeightGir = pBx->GetHeightGirderByBx();
		
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CSection        *pSec = pBx->GetSection();
		if(pSec == NULL) pSec = pBx->GetSectionLeft();

		if(pSec || nGSu == 1) 
		{
			CString sPosType;
			long nPosIdx, nDBCol, nDLCol;
			nDBCol  = (m_nRoadType == 0) ? 2 : 0;
			nDLCol  = (m_nRoadType == 0) ? 1 : 0;
			nDBCol += (m_nStruType == 0) ? 0 : 4;
			nDLCol += (m_nStruType == 0) ? 0 : 4;

			CWebHStiff *pHs = pBx->GetWebHStiff();
			long nPosSu = 4;
			long nPos = 0;
			for (nPos = 0; nPos < nPosSu; nPos++)  
			{
				if(m_nCheckType==0 || m_nCheckType==2)
				{
					switch (nPos) 
					{
					case 0 : y = Yvsl+dThickFlDN;	break;			///< 플랜지와 복부판의 연속 필렛 용접부
					case 1 : y = Yvsl+dThickFlDN+MIN(pVStiff->m_dGap[0], pVStiff->m_dGap[1]); break; ///< 수직보강재의 용접 끝부분
					case 2 : y = Yvsu-dThickFlUP-dHeightWStiff;	break;	///< 수평보강재 용접 끝부분
					case 3 : y = Yvsl+pHGusset->m_ddH-pHGusset->m_dT;	break;	///< 복부판에 부착된 가로보 연결용 거셋판 용접 끝부분
					}
					switch (nPos)  
					{
					case 0 : sPosType = "B";	nPosIdx  = 1;	break;
					case 1 : sPosType = "C";	nPosIdx  = 3;	break;
					case 2 : sPosType = "E";	nPosIdx  = 5;	break;
					case 3 : sPosType = pDB->m_dRoundCrossLower < 50 ? "E" : "D";
							 nPosIdx  = pDB->m_dRoundCrossLower < 50 ? 5 : 4;		break;
					}
				}
				else
				{
					switch (nPos) 
					{
					case 0 : y = Yvsu-dThickFlUP;	break;					///< 플랜지와 복부판의 연속 필렛 용접부
					case 1 : y = Yvsu-dThickFlUP;	break;					///< 지점부보강재 용접 끝부분
					case 2 : y = Yvsu-dThickFlUP-dHeightWStiff;	break;		///< 수평보강재 용접 끝부분
					case 3 : y = Yvsu;				break;					///< 스터드에 인접한 인장측 플랜지부	
					}
					switch (nPos)  
					{
					case 0 : sPosType = "B";	nPosIdx  = 1;	break;
					case 1 : sPosType = "C";	nPosIdx  = 3;	break;
					case 2 : sPosType = "E";	nPosIdx  = 5;	break;
					case 3 : sPosType = "C";	nPosIdx  = 3;	break;
					}
				}
				fDB = ABS(DB * y / Iv);
				fDL = ABS(DL * y / Iv);
				aDB = nAllow[nPosIdx][nDBCol];
				aDL = nAllow[nPosIdx][nDLCol];

				m_Grid1.SetTextMatrix(2, 1, "%.3f", tokNM(DB));
				m_Grid1.SetTextMatrix(2, 2, "%.3f", tokNM(DL));
				m_Grid1.SetTextMatrix(2, 3, "%.0f", Iv);
				m_Grid1.SetTextMatrix(2 + nPos, 4, "%.1f", ABS(y));
				m_Grid1.SetTextMatrix(2 + nPos, 5, "%.3f", fDB);
				m_Grid1.SetTextMatrix(2 + nPos, 6, "%.3f", fDL);
				m_Grid1.SetTextMatrix(2 + nPos, 7, sPosType);
				m_Grid1.SetTextMatrix(2 + nPos, 8, "%.0f", aDB);
				m_Grid1.SetTextMatrix(2 + nPos, 9, "%.0f", aDL);
				m_Grid1.SetTextMatrix(2 + nPos, 10, (fDB <= aDB && fDL <= aDL) ? "OK" : "NG");
			}
			if (!pSec) 
			{
				m_Grid1.DeleteRow(4);
				m_Grid1.DeleteRow(6);
				nPosSu -= (m_nCheckType == 1) ? 2 : 1;
			}
			long nDelRowSu = m_Grid1.GetRowCount() - (2 + nPosSu);
			for (nPos = 0; nPos < nDelRowSu; nPos++)
				m_Grid1.DeleteRow(2 + nPosSu);

			for (long nCol = 1; nCol <= 3; nCol++)
				m_Grid1.SetMergeRow(2, 1 + nPosSu, nCol);

			CString sNum[] = {"①", "②", "③", "④", "⑤", "⑥"};
			for (nPos = 0; nPos < nPosSu; nPos++)
				m_Grid1.SetTextMatrix(2 + nPos, 0, sNum[nPos]);

		}
	}
	m_Grid1.SetRedraw(TRUE,TRUE);
	UpdateData(FALSE);
}

void CCalcFatigue::SetDataInitShear()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CFEMManage      *pManange = m_pStd->m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	int nAllow[] = { 105,  84,  63,  56,  84,  63,  49,  42 };	// MPa

	SetGridTitleShear();

	double dRatio, Yvsu, Iv, DB, DL;
	long nEleIdxDB, nNodeIdxDB;
	long nG;
	CPlateBasicIndex *pBx = NULL;

	nNodeIdxDB = ModelCalc.GetMaxFatigueIndex(4, nEleIdxDB, dRatio, 1);
	if (nNodeIdxDB<0) return;
	CPlateBasicIndex *pBxNode = m_pStd->m_pCalcStd->GetBxByJoint(pManange->GetJoint((unsigned short)nNodeIdxDB));

	BOOL bFixPostiveMoment = FALSE;
	if(pBxNode->IsDanbuJijum())	// 거더 시점부가 부모멘트가 미세하게 생길 경우는 무시하고 정모멘트로 처리함.
		bFixPostiveMoment = TRUE;

	if (nNodeIdxDB >= 0)  
	{
		double Yvsl, tu, tl, tw, Bu, Bl, su, sl;
		DB = frkN(StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, nEleIdxDB, dRatio)
			 -StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, nEleIdxDB, dRatio));

		DL = frkN(StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, nEleIdxDB, dRatio)
			 -StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, nEleIdxDB, dRatio));

		nG    = ModelCalc.GetGirderNoByJointID(nNodeIdxDB);
		pBx   = m_pStd->m_pCalcStd->GetBxByJoint(pManange->GetJoint((unsigned short)nNodeIdxDB));

		ModelCalc.GetSectionCoefficient(nG, pBx->GetStation());

		Yvsu = ModelCalc.GetMapValueSecCo("Yvsu");
		Yvsl = -ModelCalc.GetMapValueSecCo("Yvsl");
		double dSta = pBx->GetStation();

//		if(StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, dSta, 0) > 0.0 || bFixPostiveMoment)
		if(StressCalc.IsPositive(ELE_MOMENT3, nG, dSta) || bFixPostiveMoment)
		{
			ModelCalc.GetSectionCoefficient(nG, dSta);
			Iv = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);  // 정모멘트
		}
		else
		{
			StressCalc.GetSectionStress(nG, dSta);
			Iv = StressCalc.GetMapValueSecStress("Irv");  // 부모멘트
		}
		
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		tu = pGir->GetThickFactByStation(dSta, G_F_U);
		tl = pGir->GetThickFactByStation(dSta, G_F_L);
		tw = pGir->GetThickFactByStation(dSta, G_W);
		Bu = pGir->GetWidthFlangeUpper(pBx);
		Bl = pGir->GetWidthFlangeLower(pBx);
		su = max(RoundUp(sqrt(2 * max(tu, tw)), 0), 6.0);
		sl = max(RoundUp(sqrt(2 * max(tl, tw)), 0), 6.0);

		long nDBCol, nDLCol;
		nDBCol  = (m_nRoadType == 0) ? 2 : 0;
		nDLCol  = (m_nRoadType == 0) ? 1 : 0;
		nDBCol += (m_nStruType == 0) ? 0 : 4;
		nDLCol += (m_nStruType == 0) ? 0 : 4;
		double aDB, aDL;
		aDB = nAllow[nDBCol];
		aDL = nAllow[nDLCol];

		for (long nUpper = 0; nUpper < 2; nUpper++) 
		{
			CGridCtrl *pGrid = nUpper ? &m_Grid2 : &m_Grid3;

			double t, B, A, Yv, Q, s, Sa, fDB, fDL;
			t  = nUpper ? tu : tl;
			B  = nUpper ? Bu : Bl;
			Yv = nUpper ? Yvsu : Yvsl;
			s  = nUpper ? su : sl;
			A  = t * B;
			Q  = A * (Yv - t/2);
			Sa = s * 4 / sqrt(2.0);
			fDB = DB * Q / (Iv * Sa);
			fDL = DL * Q / (Iv * Sa);

			pGrid->SetTextMatrix(1, 1, "%.3f", tokN(DB));
			pGrid->SetTextMatrix(2, 1, "%.3f", tokN(DL));
			pGrid->SetTextMatrix(1, 2, "%.0f", t);
			pGrid->SetTextMatrix(1, 3, "%.1f", B);
			pGrid->SetTextMatrix(1, 4, "%.1f", A);
			pGrid->SetTextMatrix(1, 5, "%.1f", Yv);
			pGrid->SetTextMatrix(1, 6, "%.0f", Q);
			pGrid->SetTextMatrix(1, 7, "%.0f", Iv);
			pGrid->SetTextMatrix(1, 8, "%.0f", s);
			pGrid->SetTextMatrix(1, 9, "%.2f", Sa);
			pGrid->SetTextMatrix(1, 10, "%.1f", fDB);
			pGrid->SetTextMatrix(2, 10, "%.1f", fDL);
			pGrid->SetTextMatrix(1, 11, "%.0f", aDB);
			pGrid->SetTextMatrix(2, 11, "%.0f", aDL);
			pGrid->SetTextMatrix(1, 12, fDB <= aDB ? "OK" : "NG");
			pGrid->SetTextMatrix(2, 12, fDL <= aDL ? "OK" : "NG");
		
			pGrid->SetMergeRow(1, 2, 2);
			pGrid->SetMergeRow(1, 2, 3);
			pGrid->SetMergeRow(1, 2, 4);
			pGrid->SetMergeRow(1, 2, 5);
			pGrid->SetMergeRow(1, 2, 6);
			pGrid->SetMergeRow(1, 2, 7);
			pGrid->SetMergeRow(1, 2, 8);
			pGrid->SetMergeRow(1, 2, 9);
		}
	}
	m_Grid2.SetRedraw(TRUE,TRUE);
	m_Grid3.SetRedraw(TRUE,TRUE);
}

