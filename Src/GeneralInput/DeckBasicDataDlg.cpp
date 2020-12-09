// DeckBasicDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckBasicDataDlg dialog
CDeckBasicDataDlg::CDeckBasicDataDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckBasicDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckBasicDataDlg)
	//}}AFX_DATA_INIT
}


CDeckBasicDataDlg::~CDeckBasicDataDlg()
{
}



void CDeckBasicDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckBasicDataDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BDTOPEN_BUTTON, m_btnOptOpen);
	DDX_Control(pDX, IDC_BDTSAVE_BUTTON, m_btnOptSave);
}

BEGIN_MESSAGE_MAP(CDeckBasicDataDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckBasicDataDlg)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
 	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(IDC_BDTOPEN_BUTTON, OnOptionOpen)
	ON_BN_CLICKED(IDC_BDTSAVE_BUTTON, OnOptionsave)
	ON_BN_CLICKED(IDC_ONLYAPPLY, OnAllRecommend)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckBasicDataDlg message handlers
void CDeckBasicDataDlg::OnPreInitDialog()
{
	m_Tab.DeleteAllItems();

	m_Tab.InsertItem(0, "설계 환경");
	m_Tab.InsertItem(1, "철근 정착장");
	m_Tab.InsertItem(2, "철근 이음장");
	m_Tab.InsertItem(3, "철근 단위 중량 및 단면적");
	m_Tab.InsertItem(4, "철근 정착, 이음 보정 계수");

	m_Tab.SetCurSel(0);

	m_Grid.InitGrid(toKgPCm2(m_pStd->m_pDeckData->m_SigmaY));
	SetGridData();

	m_dCover	  = 100;
	m_dRebarSpace = 125;

	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_ONLYAPPLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BDTOPEN_BUTTON, SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);
	SetResize(IDC_BDTSAVE_BUTTON, SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);

//	// 확폭 체크 - APlate는 확폭지원 안함
//	CStringArray szText;
//	szText.Add("확폭 교량의 바닥판 배근은 지원되지 않습니다.\n\n");
//	szText.Add("단, 연속교에서 일부만 확폭이 되는 경우는\n");
//	szText.Add("\"일반입력 >> 횡단구성 >> 확폭교량을 일반교량으로 변환\" 버튼을      \n");
//	szText.Add("사용하실 경우 교량을 등폭으로 변경시켜 이용하실 수 있습니다.");
//
//	CPlateBridgeApp *pDB	 = m_pStd->GetBridge();
//	CPlateGirderApp *pGirPre = pDB->GetGirder(0);
//	for(long nG=1; nG<pDB->GetQtyGirder(); nG++)
//	{
//		CPlateGirderApp *pGir = pDB->GetGirder(nG);
//		if(pGirPre->GetLine() != pGir->GetLine())
//		{
//			AfxMessageBox(szText.GetAt(0)+szText.GetAt(1)+szText.GetAt(2)+szText.GetAt(3));
//			return;
//		}
//		pGirPre = pGir;
//	}
}


void CDeckBasicDataDlg::SetDataDefault()
{
	BOOL bFirst	= !IsUsedFlag();

	if(bFirst || GetCurTabName()=="설계 환경")					SetDataDefaultFactor();
	if(bFirst || GetCurTabName()=="철근 정착장")				SetDataDefaultSettle();
	if(bFirst || GetCurTabName()=="철근 이음장")				SetDataDefaultJoint();
	if(bFirst || GetCurTabName()=="철근 단위 중량 및 단면적")	SetDataDefaultWeight();
	if(bFirst || GetCurTabName()=="철근 정착, 이음 보정 계수")	SetDataDefaultRebarFactor();

	SetGridData();
	SetDataInit();
	m_pStd->m_pDoc->SetModifiedFlag();
}


void CDeckBasicDataDlg::OnCellChanged(NMHDR*nmgv,LRESULT*)
{
}



void CDeckBasicDataDlg::OnCellChangedData(NMHDR*nmgv,LRESULT*)
{

	CCellID cell = m_Grid.GetFocusCell();
	CString str = m_Grid.GetTextMatrix(cell.row, 0);

	SetDataSave();

	
	if(GetCurTabName()=="철근 정착장" || GetCurTabName()=="철근 이음장")
	{
		SetGridData();
		SetDataInit();
	}
}



////////////////////////////////////////////////////
// 정착장 : 구조설계기준 163p
// 1) 30cm
// 2) 0.152*db*Fy/sqrt(Fck) * 보정계수
// 3) 0.285*db*Fy/sqrt(Fck) * αβγλ/((c+Ktr)/db)
// 1),2) 또는 3) 중 큰 값을 사용한다
// 기본정착,일반철근,상부철근/기본정착,나선철근
double CDeckBasicDataDlg::GetValueSettle(long nIdxDia, long nIdxType)
{
	double dDia	  = 0;
	if(nIdxDia>11 || nIdxType>4) return 0;

	if(nIdxDia==0)  dDia = 10;	if(nIdxDia==1)  dDia = 13;
	if(nIdxDia==2)  dDia = 16;	if(nIdxDia==3)  dDia = 19;
	if(nIdxDia==4)  dDia = 22;	if(nIdxDia==5)  dDia = 25;
	if(nIdxDia==6)  dDia = 29;	if(nIdxDia==7)  dDia = 32;
	if(nIdxDia==8)  dDia = 35;	if(nIdxDia==9)  dDia = 38;
	if(nIdxDia==10) dDia = 41;	if(nIdxDia==11) dDia = 51;

	// 인장이형철근
	double Fy	  = toKgPCm2(m_pStd->m_pDeckData->m_SigmaY);
	double Fck	  = toKgPCm2(m_pStd->m_pDeckData->m_SigmaCK);
	double alpha  = (nIdxType==2) ? 1.30 : 1.00;	// 철근배근 위치계수
	double beta   = 1.00;							// 에폭시 도막계수
	double ramda  = 1.00;							// 경량콘크리트 계수
	double factor = (dDia>=22) ? m_pStd->m_pDeckData->m_dGamma_D22Up : m_pStd->m_pDeckData->m_dGamma_D22Dn;
	double LdMiin = 300;
	double Ldb	  = 0.152*dDia*Fy/sqrt(Fck);
	double Ld	  = Round(factor*alpha*beta*ramda*Ldb,0);

	if(nIdxType==0)	return Round(Ldb,0);
	if(nIdxType<=2)	return max(LdMiin,Ld);

	// 압축이형철근
	LdMiin = 200;
	Ldb	   = max(0.08*dDia*Fy/sqrt(Fck), 0.004*dDia*Fy);

	if(nIdxType==3)	return Round(Ldb,0);
	if(nIdxType==4)	return Round(0.75*Ldb,0);

	return 0;
}

////////////////////////////////////////////////////
// 이음장 : 구조설계기준 174p
// 일반A,일반B,상부A,상부B,압축일반,띠철근,나선철근
double CDeckBasicDataDlg::GetValueJoint(long nIdxDia, long nIdxType)
{
	double dDia	  = 0;
	if(nIdxDia>11 || nIdxType>6) return 0;

	if(nIdxDia==0)  dDia = 10;	if(nIdxDia==1)  dDia = 13;
	if(nIdxDia==2)  dDia = 16;	if(nIdxDia==3)  dDia = 19;
	if(nIdxDia==4)  dDia = 22;	if(nIdxDia==5)  dDia = 25;
	if(nIdxDia==6)  dDia = 29;	if(nIdxDia==7)  dDia = 32;
	if(nIdxDia==8)  dDia = 35;	if(nIdxDia==9)  dDia = 38;
	if(nIdxDia==10) dDia = 41;	if(nIdxDia==11) dDia = 51;

	// 인장이형철근	
	double Fy	  = toKgPCm2(m_pStd->m_pDeckData->m_SigmaY);
	double Fck	  = toKgPCm2(m_pStd->m_pDeckData->m_SigmaCK);
	double factor = (nIdxType==1||nIdxType==3) ? 1.30 : 1.00;
	double Ldb    = GetValueSettle(nIdxDia,1);	// 기본정착
	if(nIdxType==2||nIdxType==3) 
		Ldb  = GetValueSettle(nIdxDia,2);	// 상부철근

	if(nIdxType<=3)  return Round(factor*Ldb,0);

	// 압축이형철근
	double Len  = (Fy<=4000) ? 0.0072*Fy*dDia : (0.013*Fy-24)*dDia;
	if(Len<300) Len = 300;
	if(Fck<210) Len = Len*(1+1/3);

	if(nIdxType==4)  return Round(1.00*Len,0);
	if(nIdxType==5)  return Round(0.83*Len,0);
	if(nIdxType==6)  return Round(0.75*Len,0);

	return 0;
}

void CDeckBasicDataDlg::SetDataSave()
{
	m_Grid.UpdateData(TRUE);

}

void CDeckBasicDataDlg::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	long nRows = 0;
	long nCols = 0;
	
	// 머지할게 있는지 체크하고 머지함.
	m_Grid.GetCellCount(nRows, nCols);

	if(GetCurTabName()=="설계 환경")
	{
		m_Grid.MergeGrid(0, nRows-1, 0, 1);
	}
	if(GetCurTabName()=="철근 정착장")
	{
		m_Grid.MergeGrid(0, 1, 0, nCols-1);
		if(m_pStd->m_pDeckData->m_nTypeRebarConnect != 0)
		{
			m_Grid.MergeGrid(nRows-2, nRows-2, 1, nCols-1);
			m_Grid.MergeGrid(nRows-1, nRows-1, 1, nCols-1);
		}
	}
	else if(GetCurTabName()=="철근 이음장")
	{
		m_Grid.MergeGrid(0, 2, 0, nCols-1);
		if(m_pStd->m_pDeckData->m_nTypeRebarConnect != 0)
		{
			m_Grid.MergeGrid(nRows-2, nRows-2, 1, nCols-1);
			m_Grid.MergeGrid(nRows-1, nRows-1, 1, nCols-1);
		}
	}
	else if(GetCurTabName()=="철근 정착, 이음 보정 계수")
	{
		m_Grid.MergeGrid(1, nRows, 0, 1);
	}

	AfxGetMainWnd()->SendMessage(WM_USER+12);
}



void CDeckBasicDataDlg::SetDataDefaultFactor()
{
	CADeckData *pData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	pData->m_dLengthRebarMax		= 8000;
	pData->m_nTypeRebarConnect		=	1;  // 철근겹이음 방식 - 보정계수에 의한 방법
	pData->m_nTypeBMApply			=	1;
	pData->m_nRoundRebarConnect		= 1;
	pData->m_bIncludeSumZero		= TRUE;
	pData->m_bInstallMainRebarAtBlockOut = FALSE;
	pData->m_bInstallBeryukRebarAtBlockOut = FALSE;
	pDB->m_bApplyHunchAtCB = TRUE;

}

void CDeckBasicDataDlg::SetDataDefaultSettle() 
{
	CADeckData *pData = m_pStd->m_pDeckData;

	double dLength = 0;
	for(long nRow=0; nRow<REBAR_SETTLE_ROW; nRow++)
	{
		for(long nCol=0; nCol<REBAR_SETTLE_COL; nCol++)
		{
			dLength = Round(GetValueSettle(nRow,nCol)/10,0)*10; //pStd->GetValueSettle(pStd->m_Fck,pStd->m_Fy,nRow,nCol)/10,0)*10;
			pData->m_RebarSettle[nRow][nCol] = dLength;
		}
	}
	m_dCover	  = 100;
	m_dRebarSpace = 125;
}

void CDeckBasicDataDlg::SetDataDefaultJoint() 
{
	CADeckData *pData = m_pStd->m_pDeckData;

	double dLength = 0;
	for(long nRow=0; nRow<REBAR_JOINT_ROW; nRow++)
	{
		for(long nCol=0; nCol<REBAR_JOINT_COL; nCol++)
		{
			dLength = Round(GetValueJoint(nRow,nCol)/10,0)*10;
			pData->m_RebarJoint[nRow][nCol] = dLength;
		}
	}

	m_dCover	  = 100;
	m_dRebarSpace = 125;
}

void CDeckBasicDataDlg::SetDataDefaultWeight()
{
	m_pStd->m_pDeckData->SetDataDefaultRebarInfo();
}


void CDeckBasicDataDlg::SetDataDefaultRebarFactor()
{
	m_pStd->m_pDeckData->SetDataDefaultRebarFactor();
}

BOOL CDeckBasicDataDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		if(m_Grid.GetFocusCell().col==m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	m_Grid.ViewGridCellForCombo(pMsg);


	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CDeckBasicDataDlg::SetGridData()
{
	if(GetCurTabName()=="설계 환경")	SetGridDataFactor();
	if(GetCurTabName()=="철근 정착장")	SetGridDataSettle();
	if(GetCurTabName()=="철근 이음장")	SetGridDataJoint();
	if(GetCurTabName()=="철근 단위 중량 및 단면적")	SetGridDataWeight();
	if(GetCurTabName()=="철근 정착, 이음 보정 계수")	SetGridDataRebarFactor();
}



void CDeckBasicDataDlg::SetGridDataFactor()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 2, FALSE);

	long row = 0;
	long col = 0;
	CStringArray sArr;
	CString str = _T(""), strTitle = _T("");
	UINT nFormatLeft  = GVHT_LEFT;
	UINT nFormatRight = DT_RIGHT;
	UINT nLine = CELL_LINE_NO;
	
	m_Grid.AddCell(row, col++,"구  분");
	m_Grid.AddCell(row, col++,"구  분");
	m_Grid.AddCell(row, col++,"설정값");
	m_Grid.AddCell(row, col++,"단  위");

	col = 0; 
	strTitle = "  철근 겹이음 적용 길이";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_dLengthRebarMax, nFormatRight); m_Grid.AddCell(row, col++, "mm",nFormatLeft);

	sArr.RemoveAll(); sArr.Add("보정계수에 의한방법"); sArr.Add("산식에 의한방법");
	strTitle = "  겹이음 및 정착장 산출방법";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_nTypeRebarConnect, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	
	sArr.RemoveAll(); sArr.Add("일반수량산출기준"); sArr.Add("실적수량산출기준(공통수량분리)"); sArr.Add("실적수량산출기준(공통수량포함)");
	strTitle = "  수량산출 적용기준";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_nTypeBMApply, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	
	sArr.RemoveAll(); sArr.Add("올림"); sArr.Add("반올림"); sArr.Add("반올림 안함");
	strTitle = "  철근 겹이음, 정착장 산정시 반올림";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_nRoundRebarConnect, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	
	sArr.RemoveAll(); sArr.Add("포함"); sArr.Add("포함안함");
	strTitle = "  수량집계에 빈수량(ZERO) 포함";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_bIncludeSumZero, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll(); sArr.Add("설치"); sArr.Add("미설치");
	strTitle = "  무수축콘크리트 주철근설치";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_bInstallMainRebarAtBlockOut, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr, &m_DlgInstallMainAtBlockout); 

	sArr.RemoveAll(); sArr.Add("설치"); sArr.Add("미설치");
	strTitle = "  무수축콘크리트 배력철근설치";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_bInstallBeryukRebarAtBlockOut, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr, &m_DlgInstallBeryukAtBlockout); 
	
	sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("미적용");
	strTitle = "  입력화면에서 철근마크보기";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_bShowMark, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);
	
	sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("미적용");
	strTitle = "  [적용]시 철근제원재계산";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_bRecalcJewon, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("미적용");
	strTitle = "  가로보 헌치적용(거더상판 일치시)";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->GetBridge()->m_bApplyHunchAtCB, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("75, 7@150, 75 (H13)");
	sArr.Add("50, 11@100, 50 (H13)");
	strTitle = "프리캐스트판넬 철근";
	col = 0; m_Grid.AddCell(++row, col++,strTitle, nFormatLeft); m_Grid.AddCell(row, col++,strTitle, nFormatLeft);
	m_Grid.AddCell(row, col++, &m_pStd->m_pDeckData->m_nOptionLBDeckTensionRebarCTC, nFormatRight, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr); 

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,150);
	m_Grid.SetColumnWidth(1,150);
	m_Grid.SetColumnWidth(2,220);
	m_Grid.SetColumnWidth(3, 80);
//	m_Grid.SetRowHeight(10, 0);
}

void CDeckBasicDataDlg::SetGridDataSettle()
{
	CADeckData *pDeck = m_pStd->m_pDeckData;
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 1, FALSE);

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	UINT nLine = CELL_LINE_NO;	

	CString str = _T("");
	if(m_pStd->m_pDeckData->m_nTypeRebarConnect==0)
	{	
		// 타이틀 그리기
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"압축철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"기본정착");
		m_Grid.AddCell(nRow, 2,"일반철근");
		m_Grid.AddCell(nRow, 3,"상부철근");	
		m_Grid.AddCell(nRow, 4,"기본정착");
		m_Grid.AddCell(nRow, 5,"나선철근");
		nRow = 2;
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");

		// 데이타
		m_Grid.GetCellCount(nRow, nCol);
		CString str = _T("");
		for(long i=2; i<nRow; i++)
		{
			for(long j=1; j<nCol; j++)
			{
				m_Grid.AddCell(i, j, &m_pStd->m_pDeckData->m_RebarSettle[i-2][j-1], nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			}
		}
	}
	else
	{			
		// 타이틀
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"압축철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"기본정착");
		m_Grid.AddCell(nRow, 2,"일반철근");
		m_Grid.AddCell(nRow, 3,"상부철근");	
		m_Grid.AddCell(nRow, 4,"기본정착");
		m_Grid.AddCell(nRow++, 5,"나선철근");
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");
		m_Grid.AddCell(nRow++, 0,"피복두께");	
		m_Grid.AddCell(nRow++, 0,"철근간격");	

		// 데이타
		CString str = _T("");
		double dVal[] = { 10, 13, 16, 19, 22, 25, 29, 32, 35, 38 , 41, 51 };
		m_Grid.GetCellCount(nRow, nCol);
		long i = 0;
		for(i=2; i<nRow-2; i++)
		{
			for(long j=1; j<nCol; j++)
			{
				m_pStd->m_pDeckData->m_RebarSettle[i-2][j-1]	= pDeck->GetValueSettle(toKgPCm2(pDeck->m_SigmaCK), toKgPCm2(pDeck->m_SigmaY), dVal[i-2], j-1, toCm(m_dCover), toCm(m_dRebarSpace));
				str.Format("%.0f",pDeck->m_RebarSettle[i-2][j-1]);
				m_Grid.AddCell(i, j, str, nFormat, CELL_READONLY);
			}
		}
		
		for(long c = 1; c < nCol; c++)
		{
			if(c == 1)
			{
				m_Grid.AddCell(i, c, &m_dCover);
				m_Grid.AddCell(i+1, c, &m_dRebarSpace);
			}
			else
			{
				str.Format("%.0f",m_dCover);
				m_Grid.AddCell(i, c, str);
				str.Format("%.0f",m_dRebarSpace);
				m_Grid.AddCell(i+1, c, str);
			}
		}
	}

	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
}

void CDeckBasicDataDlg::SetGridDataJoint()
{
	CADeckData *pDeck = m_pStd->m_pDeckData;
	// 그리드 초기화
	m_Grid.SetGridDefault(3, 3, 3, 1, FALSE);

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	UINT nLine = CELL_LINE_NO;	
	
	if(m_pStd->m_pDeckData->m_nTypeRebarConnect==0)
	{	
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+2, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"인장철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		m_Grid.AddCell(nRow, 6,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"일반");
		m_Grid.AddCell(nRow, 2,"일반");
		m_Grid.AddCell(nRow, 3,"상부");
		m_Grid.AddCell(nRow, 4,"상부");
		m_Grid.AddCell(nRow, 5,"일반");	
		m_Grid.AddCell(nRow, 6,"폐합");
		nRow = 2;
		m_Grid.AddCell(nRow, 1,"A급");
		m_Grid.AddCell(nRow, 2,"B급");
		m_Grid.AddCell(nRow, 3,"A급");
		m_Grid.AddCell(nRow, 4,"B급");
		m_Grid.AddCell(nRow, 5,"B급");
		m_Grid.AddCell(nRow, 6,"띠철근");
		m_Grid.AddCell(0, 7,"나선철근\n");
		m_Grid.AddCell(1, 7,"나선철근\n");
		m_Grid.AddCell(2, 7,"나선철근\n");
		//
		nRow = 3;
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");


		m_Grid.GetCellCount(nRow, nCol);
		CString str = _T("");
		for(long i=3; i<nRow; i++)
		{
			for(long j=1; j<nCol; j++)
			{
				m_Grid.AddCell(i, j, &pDeck->m_RebarJoint[i-3][j-1], nFormat, nLine);
			}
		}
	}
	else
	{	
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+2, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"인장철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		m_Grid.AddCell(nRow, 6,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"일반");
		m_Grid.AddCell(nRow, 2,"일반");
		m_Grid.AddCell(nRow, 3,"상부");
		m_Grid.AddCell(nRow, 4,"상부");
		m_Grid.AddCell(nRow, 5,"일반");	
		m_Grid.AddCell(nRow, 6,"폐합");
		nRow = 2;
		m_Grid.AddCell(nRow, 1,"A급");
		m_Grid.AddCell(nRow, 2,"B급");
		m_Grid.AddCell(nRow, 3,"A급");
		m_Grid.AddCell(nRow, 4,"B급");
		m_Grid.AddCell(nRow, 5,"B급");
		m_Grid.AddCell(nRow, 6,"띠철근");
		m_Grid.AddCell(0, 7,"나선철근\n");
		m_Grid.AddCell(1, 7,"나선철근\n");
		m_Grid.AddCell(2, 7,"나선철근\n");
		//
		nRow = 3;
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");

		nRow = 15;
		m_Grid.AddCell(nRow++, 0,"피복두께");	
		m_Grid.AddCell(nRow, 0,"철근간격");	

		m_Grid.GetCellCount(nRow, nCol);
		CString str = _T("");
		double dVal[] = { 10, 13, 16, 19, 22, 25, 29, 32, 35, 38 , 41, 51 };
		long i = 0;
		for(i=3; i<nRow-2; i++)
		{
			for(long j=1; j<nCol; j++)
			{
				str.Format("%g",pDeck->GetValueJoint(toKgPCm2(pDeck->m_SigmaCK), toKgPCm2(pDeck->m_SigmaY), dVal[i-3], j-1, toCm(m_dCover), toCm(m_dRebarSpace)));
				m_Grid.AddCell(i,j,str, nFormat, CELL_READONLY);
			}			
		}

		for(long c = 1; c < nCol; c++)
		{
			if(c == 1)
			{
				m_Grid.AddCell(i, c, &m_dCover);
				m_Grid.AddCell(i+1, c, &m_dRebarSpace);
			}
			else
			{
				str.Format("%.0f",m_dCover);
				m_Grid.AddCell(i, c, str);
				str.Format("%.0f",m_dRebarSpace);
				m_Grid.AddCell(i+1, c, str);
			}
		}
	}

		
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
}

void CDeckBasicDataDlg::SetGridDataWeight()
{
	// 그리드 초기화
	m_Grid.SetGridDefault();

	long nRow = 0;
	long nCol = 0;
	CString szTitle	= _T("");

	m_Grid.AddCell(nRow, nCol++, "\n구 분");
	m_Grid.AddCell(nRow, nCol++, "\n단위질량\n(kg/m)");

	szTitle.Format("\n단면적(mm²)\n");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n지  름(mm)\n");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n둘  레(mm)");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n모서리부 구부림\n반지름(mm)");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n모서리부 절곡시\n적용값(mm)");
	m_Grid.AddCell(nRow, nCol++, szTitle);
	//
	nRow = 1;
	m_Grid.AddCell(nRow++, 0,"D10");
	m_Grid.AddCell(nRow++, 0,"D13");
	m_Grid.AddCell(nRow++, 0,"D16");
	m_Grid.AddCell(nRow++, 0,"D19");
	m_Grid.AddCell(nRow++, 0,"D22");
	m_Grid.AddCell(nRow++, 0,"D25");
	m_Grid.AddCell(nRow++, 0,"D29");
	m_Grid.AddCell(nRow++, 0,"D32");
	m_Grid.AddCell(nRow++, 0,"D35");
	m_Grid.AddCell(nRow++, 0,"D38");
	m_Grid.AddCell(nRow++, 0,"D41");
	m_Grid.AddCell(nRow++, 0,"D51");


	CString str = _T("");
	CString szCGS = _T("");
	for(long i=1; i<nRow; i++)
	{
		for(long j=1; j<nCol; j++)
		{
			if(j==nCol-1 || j==nCol-2)
				str.Format("%.0f", m_pStd->m_pDeckData->m_RebarWeight[i-1][j-1]);
			else
				str.Format("%.3f", m_pStd->m_pDeckData->m_RebarWeight[i-1][j-1]);
			m_Grid.AddCell(i, j, &m_pStd->m_pDeckData->m_RebarWeight[i-1][j-1]);
			if(j==nCol-1 || j==nCol-2)
				m_Grid.SetCellDec(0);
			else
				m_Grid.SetCellDec(3);

			if(j == 2)	// 단면적 단위계 적용
			{
				m_Grid.SetCellUnit(szCGS, "mm²");
			}
			else if(j == 4)
			{
				m_Grid.SetCellUnit(szCGS, "mm");
			}
		}
	}

	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0,50);
	m_Grid.SetColumnWidthAll(105);
}


void CDeckBasicDataDlg::SetGridDataRebarFactor()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1, FALSE);

	long nRow = 0;
	long nRows = 0;
	long nCols = 0;

	m_Grid.AddCell(nRow, 0,"구분");		
	m_Grid.AddCell(nRow, 1,"종류");		
	m_Grid.AddCell(nRow, 2,"계수");		
	

	nRow = 1;
	m_Grid.AddCell(nRow, 0,"α\n"); 
	m_Grid.AddCell(nRow+1, 0,"α\n"); 
	m_Grid.AddCell(nRow, 1,"상부철근"); m_Grid.AddCell(nRow, 2, &m_pStd->m_pDeckData->m_dAlpha_UpRebar); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"일반철근"); m_Grid.AddCell(nRow+1, 2, &m_pStd->m_pDeckData->m_dAlpha_GenBar); m_Grid.SetCellDec(CELL_DEC_2); 
	nRow = 3;
	m_Grid.AddCell(nRow, 0,"β\n");
	m_Grid.AddCell(nRow+1, 0,"β\n");
	m_Grid.AddCell(nRow+2, 0,"β\n");
	m_Grid.AddCell(nRow, 1,"피복두께 3db미만, 순간격 6db미만 도막철근");	m_Grid.AddCell(nRow, 2, &m_pStd->m_pDeckData->m_dBeta_Tomak1);	m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"기타도막철근"); m_Grid.AddCell(nRow+1, 2, &m_pStd->m_pDeckData->m_dBeta_Tomak2); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+2, 1,"도막되지 않은철근"); m_Grid.AddCell(nRow+2, 2, &m_pStd->m_pDeckData->m_dBeta_GenBar); m_Grid.SetCellDec(CELL_DEC_2); 
	nRow = 6;
	m_Grid.AddCell(nRow, 0,"r\n");
	m_Grid.AddCell(nRow+1, 0,"r\n");
	m_Grid.AddCell(nRow, 1,"D22이상의 철근"); m_Grid.AddCell(nRow, 2, &m_pStd->m_pDeckData->m_dGamma_D22Up); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"D19이하의 철근과 이형철선"); m_Grid.AddCell(nRow+1, 2, &m_pStd->m_pDeckData->m_dGamma_D22Dn); m_Grid.SetCellDec(CELL_DEC_2); 
	nRow = 8;
	m_Grid.AddCell(nRow, 0,"λ\n");
	m_Grid.AddCell(nRow+1, 0,"λ\n");
	m_Grid.AddCell(nRow, 1,"경량콘크리트"); m_Grid.AddCell(nRow, 2, &m_pStd->m_pDeckData->m_dIpSmallConc); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"일반콘크리트"); m_Grid.AddCell(nRow+1, 2, &m_pStd->m_pDeckData->m_dIpGenConc); m_Grid.SetCellDec(CELL_DEC_2); 


	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(1,300);
}


void CDeckBasicDataDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	m_pView->GetDomyun()->ClearEtt(TRUE);

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}


BOOL CDeckBasicDataDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}

void CDeckBasicDataDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetFocus();
	m_Grid.SetGridDefault();
	
	SetGridData();
	SetDataInit();

	DrawInputDomyunView(TRUE);
}

void CDeckBasicDataDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();
}


CString CDeckBasicDataDlg::GetCurTabName()
{
	CString sText;
	TCITEM item ;
	char cTableName[256];
	item.mask = TCIF_TEXT;
	item.pszText = cTableName;
	item.cchTextMax = 255;

	m_Tab.GetItem(m_Tab.GetCurSel(), &item);
	sText = item.pszText;

	return sText;
	
}


void CDeckBasicDataDlg::OnOptionOpen() 
{
	// TODO: Add your control notification handler code here
	
}

void CDeckBasicDataDlg::OnOptionsave() 
{
	// TODO: Add your control notification handler code here
	
}

void CDeckBasicDataDlg::OnAllRecommend() 
{
	// TODO: Add your control notification handler code here
	
}
