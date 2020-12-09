// DetWebHStiffPosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetWebHStiffPosDlg.h"

#include "../APlateDBDraw\APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW	
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetWebHStiffPosDlg dialog


CDetWebHStiffPosDlg::CDetWebHStiffPosDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetWebHStiffPosDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetWebHStiffPosDlg)
	m_bHstiffDefault = 0;
	m_cHStiffDimRound = FALSE;
	m_bSameApplySameCol = FALSE;
	m_cHStiffDimRoundValue = 0;
	m_cHStiffHeightOffset = FALSE;
	m_bJackupVStiff = FALSE;
	m_bHstiffDefault = 1;
	m_nLengthType = 1;
	//}}AFX_DATA_INIT
	m_Row = 1;
	m_Col = 1;
	m_nTab = 1;
}


void CDetWebHStiffPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetWebHStiffPosDlg)
	DDX_Radio(pDX, IDC_WEBHSTIFF_RADIO1, m_bHstiffDefault);
	DDX_Check(pDX, IDC_HSTIFF_DIM10, m_cHStiffDimRound);
	DDX_Control(pDX, IDC_HSTIFF_TRIBAPPLY, m_dHStiffTRibApply);
	DDX_Check(pDX, IDC_WEBHSTIFF_ALL, m_bSameApplySameCol);
	DDX_Control(pDX, IDC_WHSTIFF_MINLEN, m_dHStiffLengthMin);
	DDX_Radio(pDX, IDC_WHSTIFF_5MM, m_cHStiffDimRoundValue);
	DDX_Check(pDX, IDC_HSTIFF_HEIGHTOFFSET, m_cHStiffHeightOffset);
	DDX_Control(pDX, IDC_HSTIFF_HEIGHT1, m_dHStiffHeight1);
	DDX_Control(pDX, IDC_HSTIFF_HEIGHT2, m_dHStiffHeight2);
	DDX_Control(pDX, IDC_HSTIFF_HEIGHT3, m_dHStiffHeight3);
	DDX_Check(pDX, IDC_HSTIFF_JACKUPVSTIFF, m_bJackupVStiff);
	DDX_Control(pDX, IDC_HSTIFF_MINJACKUP, m_dHStiffMinLenJackup);
	DDX_Radio(pDX, IDC_HSTIFF_GIRDERLEN, m_nLengthType);
	DDX_Control(pDX, IDC_HSTIFF_DISTANCE_CENTER, m_dDisCenterOnHStiff);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_WHSTIFF_AUTOSYMMETRY, m_btnAutoSymmetry);
	DDX_Control(pDX, IDC_WHSTIFF_MLEFT, m_btnMoveLeft);
	DDX_Control(pDX, IDC_WHSTIFF_MRIGHT, m_btnMoveRight);
	DDX_Control(pDX, IDC_WHSTIFF_MINLENCALC, m_btnAutoCalc);
}

BEGIN_MESSAGE_MAP(CDetWebHStiffPosDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetWebHStiffPosDlg)
	ON_BN_CLICKED(IDC_WEBHSTIFF_RADIO1, OnWebhstiffRadio1)
	ON_BN_CLICKED(IDC_HSTIFF_DIM10, OnHstiffDim10)
	ON_BN_CLICKED(IDC_WHSTIFF_MLEFT, OnMoveLeft)
	ON_BN_CLICKED(IDC_WHSTIFF_MRIGHT, OnMoveRight)
	ON_BN_CLICKED(IDC_WEBHSTIFF_ALL, OnWebhstiffAll)
	ON_BN_CLICKED(IDC_WHSTIFF_5MM, OnWhstiff5mm)
	ON_BN_CLICKED(IDC_WHSTIFF_10MM, OnWhstiff10mm)
	ON_BN_CLICKED(IDC_WHSTIFF_MINLENCALC, OnWhstiffMinlencalc)
	ON_BN_CLICKED(IDC_WHSTIFF_AUTOSYMMETRY, OnSetDataSymmetry)
	ON_BN_CLICKED(IDC_HSTIFF_HEIGHTOFFSET, OnHstiffHeightoffset)
	ON_BN_CLICKED(IDC_HSTIFF_GIRDERLEN, OnHstiffGirderlen)
	ON_BN_CLICKED(IDC_HSTIFF_BRIDGELEN, OnHstiffBridgelen)
	ON_BN_CLICKED(IDC_WEBHSTIFF_RADIO2, OnWebHstiffRadio2)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)	
	ON_EN_KILLFOCUS(IDC_HSTIFF_TRIBAPPLY, OnKillfocusHstiffTribapply)
	ON_EN_KILLFOCUS(IDC_HSTIFF_DISTANCE_CENTER, OnKillfocusHstiffDistanceCenter)
	//}}AFX_MSG_MAP
	
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetWebHStiffPosDlg message handlers

void CDetWebHStiffPosDlg::OnWebhstiffRadio1() 
{
	UpdateData(TRUE);
	
	DrawInputDomyunView(FALSE);
}

void CDetWebHStiffPosDlg::OnWebHstiffRadio2() 
{
	UpdateData(TRUE);
	
	DrawInputDomyunView(FALSE);
}

// 치수보정
void CDetWebHStiffPosDlg::OnHstiffDim10() 

{
	m_cHStiffDimRound = !m_cHStiffDimRound;
	m_pStd->GetBridge()->m_cHStiffDimRound = (BYTE)m_cHStiffDimRound;
	m_Grid.SetFocus();
	if( m_cHStiffDimRound )
	{
		GetDlgItem(IDC_WHSTIFF_5MM)->EnableWindow(TRUE);
		GetDlgItem(IDC_WHSTIFF_10MM)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_WHSTIFF_5MM)->EnableWindow(FALSE);
		GetDlgItem(IDC_WHSTIFF_10MM)->EnableWindow(FALSE);
	}
}

void CDetWebHStiffPosDlg::OnHstiffHeightoffset() 
{
	m_cHStiffHeightOffset = !m_cHStiffHeightOffset;
	m_pStd->GetBridge()->m_cHStiffHeightOffset = (BYTE)m_cHStiffHeightOffset;
	m_Grid.SetFocus();

	GetDlgItem(IDC_HSTIFF_HEIGHT1)->EnableWindow(m_cHStiffHeightOffset);
	GetDlgItem(IDC_HSTIFF_HEIGHT2)->EnableWindow(m_cHStiffHeightOffset);
	GetDlgItem(IDC_HSTIFF_HEIGHT3)->EnableWindow(m_cHStiffHeightOffset);

}

// 5mm 절삭
void CDetWebHStiffPosDlg::OnWhstiff5mm() 
{
	m_cHStiffDimRoundValue = 0;
	m_pStd->GetBridge()->m_cHStiffDimRoundValue = (BYTE)m_cHStiffDimRoundValue;
}

// 10mm 절삭
void CDetWebHStiffPosDlg::OnWhstiff10mm() 
{
	m_cHStiffDimRoundValue = 1;
	m_pStd->GetBridge()->m_cHStiffDimRoundValue = (BYTE)m_cHStiffDimRoundValue;
}


// 모든거더적용 점검버튼
void CDetWebHStiffPosDlg::OnWebhstiffAll() 
{
	UpdateData(TRUE);
	m_pStd->GetBridge()->m_cSameApplySameColHStiff = (BYTE)m_bSameApplySameCol;
}

// 좌로이동
void CDetWebHStiffPosDlg::OnMoveLeft() 
{
	long	row		= m_Grid.GetFocusCell().row;
	long	col		= m_Grid.GetFocusCell().col;
	long	nCase	= row%6;
	long	nG		= (row-1)/6;
	
	switch(nCase)
	{
		case 1: MoveTerm	(nG,col,TRUE);	break;	// 간격이동			: 수직보강재 1개 간격
		case 3: MoveUpperDan(nG,col,TRUE);	break;	// 상부단수 이동	: -1
		case 4: MoveLowerDan(nG,col,TRUE);	break;	// 하부단수 이동	: -1
		case 5: MoveWidth	(nG,col,TRUE);	break;	// 폭 이동			: -10
		case 0: MoveThick	(nG,col,TRUE);	break;	// 두께 이동		: -2
	}

	m_Grid.Invalidate();
	m_Grid.SetFocus();
}
// 우로이동
void CDetWebHStiffPosDlg::OnMoveRight() 
{
	long	row		= m_Grid.GetFocusCell().row;
	long	col		= m_Grid.GetFocusCell().col;
	long	nCase	= row%6;
	long	nG		= (row-1)/6;

	switch(nCase)
	{
		case 1: MoveTerm	(nG,col,FALSE);	break;	// 간격이동			: 수직보강재 1개 간격
		case 3: MoveUpperDan(nG,col,FALSE);	break;	// 상부단수 이동	: +1
		case 4: MoveLowerDan(nG,col,FALSE);	break;	// 하부단수 이동	: +1
		case 5: MoveWidth	(nG,col,FALSE);	break;	// 폭 이동			: +10
		case 0: MoveThick	(nG,col,FALSE);	break;	// 두께 이동		: +2
	}

	m_Grid.Invalidate();
	m_Grid.SetFocus();
}
// 간격이동
void CDetWebHStiffPosDlg::MoveTerm(long nG, long nCol, BOOL bLeft)
{
	long nGsu = nG+1;
	if( m_bSameApplySameCol )
	{
		nG = 0;
		nGsu = m_pStd->GetBridge()->GetGirdersu();
	}
	for ( ; nG < nGsu; nG++)
	{
		CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
		long nRow = nG * 6 + 1;
		if (nCol >= m_Grid.GetValidCol(nRow, FALSE))
			continue;

		double sta = GetStationMatrix(nRow, nCol);
		double staNext = GetStationMatrix(nRow, nCol + 1);
		if (sta == staNext)
			continue;
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex  *pBxNext, *pBx = Finder.GetBxFirst(BX_PLATEHSTIFF | BX_END_GIRDER);
		while(pBx && Finder.GetBxFixNext())
		{
			double staMid = (pBx->GetStation() + Finder.GetBxFixNext()->GetStation()) / 2;
			if (staMid > sta)
				break;
			pBx = Finder.GetBxNext();
		}
		if (bLeft)
			pBxNext = Finder.GetBxFixPrev();
		else
			pBxNext = Finder.GetBxFixNext();

		SetMatrixByStation(nRow, nCol, pBxNext->GetStation());
		SetMatrixByStation(nRow, nCol + 1, staNext);
	}
}

// 상부단수 이동
void CDetWebHStiffPosDlg::MoveUpperDan(long nG, long nCol, BOOL bLeft)
{
	long nGsu = nG + 1;
	long nInc = bLeft ? -1 : 1;
	CString str;
	if( m_bSameApplySameCol )
	{
		nG = 0;
		nGsu = m_pStd->GetBridge()->GetGirdersu();
	}
	for ( ; nG < nGsu; nG++)
	{
		long nRow = nG*6 + 3;
		if (m_Grid.GetTextMatrix(nRow, nCol) == "")
			continue;
		long nDan = (long) m_Grid.GetTextMatrixDouble(nRow, nCol) + nInc;
		nDan = max(nDan, 0);  // 최하 0단
		nDan = min(nDan, 3);  // 최고 3단
		m_Grid.SetTextMatrix(nRow, nCol, "%.f", (double)nDan);
	}
}

void CDetWebHStiffPosDlg::MoveLowerDan(long nG, long nCol, BOOL bLeft)
// 하부단수 이동
{
	long nGsu = nG + 1;
	long nInc = bLeft ? -1 : 1;
	CString str;
	if( m_bSameApplySameCol )
	{
		nG = 0;
		nGsu = m_pStd->GetBridge()->GetGirdersu();
	}
	for ( ; nG < nGsu; nG++)
	{
		long nRow = nG*6 + 4;
		if (m_Grid.GetTextMatrix(nRow, nCol) == "")
			continue;
		long nDan = (long) m_Grid.GetTextMatrixDouble(nRow, nCol) + nInc;
		nDan = max(nDan, 0);  // 최하 0단
		nDan = min(nDan, 3);  // 최고 3단
		m_Grid.SetTextMatrix(nRow, nCol, "%.f", (double)nDan);
	}
}

void CDetWebHStiffPosDlg::MoveWidth(long nG, long nCol, BOOL bLeft)
// 폭 이동
{
	long nGsu = nG + 1;
	long nInc = bLeft ? -10 : 10;
	CString str;
	if( m_bSameApplySameCol )
	{
		nG = 0;
		nGsu = m_pStd->GetBridge()->GetGirdersu();
	}
	for ( ; nG < nGsu; nG++)
	{
		long nRow = nG*6 + 5;
		if (m_Grid.GetTextMatrix(nRow, nCol) == "")
			continue;
		long nDan = (long) m_Grid.GetTextMatrixDouble(nRow, nCol) + nInc;
		nDan = max(nDan, 0);  // 최하 0mm
		m_Grid.SetTextMatrix(nRow, nCol, "%.f", (double)nDan);
	}

}

void CDetWebHStiffPosDlg::MoveThick(long nG, long nCol, BOOL bLeft)
// 두께 이동
{
	long nGsu = nG + 1;
	long nInc = bLeft ? -2 : 2;
	CString str;
	if( m_bSameApplySameCol )
	{
		nG = 0;
		nGsu = m_pStd->GetBridge()->GetGirdersu();
	}
	for ( ; nG < nGsu; nG++)
	{
		long nRow = nG*6 + 6;
		if (m_Grid.GetTextMatrix(nRow, nCol) == "")
			continue;
		long nDan = (long) m_Grid.GetTextMatrixDouble(nRow, nCol) + nInc;
		nDan = max(nDan, 0);  // 최하 0mm
		m_Grid.SetTextMatrix(nRow, nCol, "%.f", (double)nDan);
	}

}

void CDetWebHStiffPosDlg::OnWhstiffMinlencalc() 
// 제원 최소 길이 계산
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	double			Len		= atof(m_Grid.GetTextMatrix(9,1));
	double			D		= pGir->m_pSangse->m_HStiff_VStiff_D;
	CString			sText	= _T("");
	if( D == 0 ) D = 10;

	double MinLen = (Len-D)*2 + 10;

	sText.Format("%g",	MinLen);
	m_dHStiffLengthMin.SetWindowText(sText);

	UpdateData(FALSE);
}



void CDetWebHStiffPosDlg::OnSetDataSymmetry() 
{
	UpdateData(TRUE);
	if(m_Grid.GetCols() <= 2) return;

	if(MessageBox("기존의 데이터가 삭제 됩니다. 계속하시겠습니까","복부판 수평보강재 위치",
		MB_ICONEXCLAMATION | MB_OKCANCEL)!=IDOK) return;

	long nG = (m_Grid.GetFocusCell().row-1)/6;
	long nGsu = nG + 1;
	if(m_bSameApplySameCol)
	{
		nG = 0;
		nGsu = m_pStd->GetBridge()->GetGirdersu();
	}

	for( nG; nG < nGsu; nG++)
	{
		WhstiffAutosymmetry(nG);
	}
	m_Grid.SetFocus();
}

void CDetWebHStiffPosDlg::WhstiffAutosymmetry(long nG)
{
	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
	long nRow = nG*6 + 1;

	double SttSta = pGir->GetSSS(OFF_STT_GIRDER);
	double EndSta = pGir->GetSSS(OFF_END_GIRDER);
	double HalfSta = (SttSta + EndSta) / 2.0;  // 교량 교대부 사이 중간 == 대칭점(기준점)
	double Sta = SttSta, SymSta;
	CDDWordArray StaArr;

	long nAddIdx = 1;
	StaArr.Add(SttSta);
	long nCol = 0;
	for (nCol = 1; nCol < m_Grid.GetCols(); nCol++)
	{
		Sta = GetStationMatrix(nRow, nCol);
		Sta = pGir->GetBxByStationDir(Sta, 0, BX_PLATEHSTIFF)->GetStation();  // 스테이션 보정
		SymSta = pGir->GetBxByStationDir(HalfSta + (HalfSta - Sta), 0, BX_PLATEHSTIFF)->GetStation();
		if (Sta == SymSta)   {
			StaArr.InsertAt(nAddIdx, Sta);
			break;
		}
		if (Sta > SymSta)
			break;
		StaArr.InsertAt(nAddIdx, SymSta);
		StaArr.InsertAt(nAddIdx++, Sta);
	}
	StaArr.Add(EndSta);

	long nSymCol = StaArr.GetSize() - 1;
	if (m_Grid.GetCols() < nSymCol + 1)
		m_Grid.SetCols(nSymCol + 1);
	
	for (nCol = 1; nCol < StaArr.GetSize(); nCol++)
	{
		SetMatrixByStation(nRow, nCol, StaArr.GetAt(nCol));
		if (nSymCol > nCol)
			for (long r = 1; r < 6; r++)
				m_Grid.SetTextMatrix(nRow + r, nSymCol, m_Grid.GetTextMatrix(nRow + r, nCol));
		nSymCol--;
	}
	// 사용후의 뒷부분 남은 Cell 삭제
	for ( ; nCol < m_Grid.GetCols(); nCol++)
		for (long r = 0; r < 6; r++)
			m_Grid.SetTextMatrix(nRow + r, nCol, "");

}

void CDetWebHStiffPosDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		

	CDomyun Dom(pDom);

	if(m_nTab==0)
	{
		CAPlateDrawPyung Draw(m_pStd->GetDataManage());
		Draw.DrawPyung(&Dom, 5);
	}
	else
	{
		CAPlateDrawJong Draw(m_pStd->GetDataManage());
		Draw.DrawJongGirderTotalHStiff(&Dom);
	}
	*pDom << Dom;	


	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

//기능 : 각 거더압축 구간상에서 나누어진 간격수가 가장 큰 값을 구한다
long CDetWebHStiffPosDlg::GetQtyNodeMax()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nMaxCol = 0;

	for (long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		//if(pGir->m_nInstallVStiff==0 || pGir->m_nInstallVStiff==2)	//양측, 우측
		//	bLeft = FALSE;
		//else
		//	bLeft = TRUE;

		long nCol = 0;		
		long   nUpperL, nLowerL, nPreUpperL = -1, nPreLowerL = -1;
		long   nUpperR, nLowerR, nPreUpperR = -1, nPreLowerR = -1;
		double dThickL, dWidthL, dPreThickL = -1, dPreWidthL = -1;
		double dThickR, dWidthR, dPreThickR = -1, dPreWidthR = -1;

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_PLATEHSTIFF);
		while(pBx)
		{
			CWebHStiff *pHs = pBx->GetWebHStiff();
			if(pHs)	
			{			
				nUpperL = pHs->GetTotalDansu(TRUE, TRUE);
				nLowerL = pHs->GetTotalDansu(FALSE, TRUE);
				nUpperR = pHs->GetTotalDansu(TRUE, FALSE);
				nLowerR = pHs->GetTotalDansu(FALSE, FALSE);
				dWidthL = pHs->m_dWidthHStiff[0];
				dThickL = pHs->m_dThickHStiff[0];
				dWidthR = pHs->m_dWidthHStiff[1];
				dThickR = pHs->m_dThickHStiff[1];
				if (nUpperL != nPreUpperL || nLowerL != nPreLowerL || dThickL != dPreThickL || dWidthL != dPreWidthL ||
					nUpperR != nPreUpperR || nLowerR != nPreLowerR || dThickR != dPreThickR || dWidthR != dPreWidthR)
				{
					nCol++;
					nPreUpperL = nUpperL;
					nPreLowerL = nLowerL;
					dPreWidthL = dWidthL;
					dPreThickL = dThickL;
					nPreUpperR = nUpperR;
					nPreLowerR = nLowerR;
					dPreWidthR = dWidthR;
					dPreThickR = dThickR;
				}				
			}	
			pBx = Finder.GetBxNext();
		}
		nMaxCol = max(nMaxCol, nCol);
	}
	return nMaxCol;
}

void CDetWebHStiffPosDlg::SetGridTitle(long nQtyCols)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nRows	= pDB->GetGirdersu()*6 + 1;
	long nCols	= GetQtyNodeMax()+1;
	if(nQtyCols >= 2)	nCols = nQtyCols;
	if(nCols < 2)		nCols = 2;

	CString str;
//	m_Grid.DeleteAllItems();
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);		
	m_Grid.SetColumnWidthAll(60);	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetTextMatrix(0,0,"구 분");
	for(long col = 1;col < nCols;col++)
	{
		str.Format("%d",col);
		m_Grid.SetTextMatrix(0,col,str);
	}

	for(long row = 1; row < nRows;row+=6)
	{
		str.Format("(G%d)",row/6+pDB->GetNumberStartGirder()+1);
		m_Grid.SetTextMatrix(row  ,0,"간격    "+str);
		m_Grid.SetTextMatrix(row+1,0,"설치위치"+str); 
		for(long nC=1; nC<nCols; nC++)
		{
			m_Grid.SetCellType(row+1,nC,CT_COMBO);
	//		m_Grid.SetItemState(row+1,nC, m_Grid.GetItemState(row+1,nC) | GVIS_READONLY);
		}
		m_Grid.SetTextMatrix(row+2,0,"상부단수"+str);
		m_Grid.SetTextMatrix(row+3,0,"하부단수"+str);
		m_Grid.SetTextMatrix(row+4,0," W      "+str);
		m_Grid.SetTextMatrix(row+5,0," T      "+str);
		
		m_Grid.SetItemState(row, nCols-1, m_Grid.GetItemState(row, nCols-1) | GVIS_READONLY);
	}	
	CStringArray szCombo;
	szCombo.Add("좌측"); szCombo.Add("우측"); szCombo.Add("양측");
	m_Grid.SetComboString(szCombo);

	m_Grid.SetRedraw(TRUE,TRUE);
}
	
void CDetWebHStiffPosDlg::SetDataInit()
{
	CString			sText	= _T("");
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();

	m_bSameApplySameCol		= (BOOL)pDB->m_cSameApplySameColHStiff;
	m_bHstiffDefault		= (BOOL)pDB->m_nHStiffDefault;
	m_cHStiffDimRound		= (BOOL)pDB->m_cHStiffDimRound;

	sText.Format("%d",	(BOOL)pDB->m_dHStiffTRibApply);
	m_dHStiffTRibApply.SetWindowText(sText);

	m_cHStiffDimRoundValue	= (BOOL)pDB->m_cHStiffDimRoundValue;
	
	sText.Format("%g",	pDB->m_dHStiffLengthMin);
	m_dHStiffLengthMin.SetWindowText(sText);

	m_cHStiffHeightOffset	= (BOOL)pDB->m_cHStiffHeightOffset;

	sText.Format("%g",	pDB->m_dHStiffHeight1);
	m_dHStiffHeight1.SetWindowText(sText);

	sText.Format("%g",	pDB->m_dHStiffHeight2);
	m_dHStiffHeight2.SetWindowText(sText);

	sText.Format("%g",	pDB->m_dHStiffHeight3);
	m_dHStiffHeight3.SetWindowText(sText);

	sText.Format("%g",	pDB->m_dHStiffMinLenJackup);
	m_dHStiffMinLenJackup.SetWindowText(sText);

	m_bJackupVStiff			= pDB->m_cHStiffJackupVStiff;
	m_nLengthType			= pDB->m_nLengthTypeOnHStiff;

	sText.Format("%g",	pDB->m_dDisCenterOnHStiff);
	m_dDisCenterOnHStiff.SetWindowText(sText);

	UpdateData(FALSE);
	GetDlgItem(IDC_WHSTIFF_5MM)->EnableWindow(m_cHStiffDimRound);
	GetDlgItem(IDC_WHSTIFF_10MM)->EnableWindow(m_cHStiffDimRound);
	GetDlgItem(IDC_HSTIFF_HEIGHT1)->EnableWindow(m_cHStiffHeightOffset);
	GetDlgItem(IDC_HSTIFF_HEIGHT2)->EnableWindow(m_cHStiffHeightOffset);
	GetDlgItem(IDC_HSTIFF_HEIGHT3)->EnableWindow(m_cHStiffHeightOffset);
	
	SetGridTitle();
	for(long nG = 0;nG < pDB->GetGirdersu();nG++)
		DataInitGir(nG);	
}

void CDetWebHStiffPosDlg::DataInitGir(long nG)
{		
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);
	CWebHStiff		*pPreHs		= NULL;

	long nRow = nG * 6 + 1;
	long nCol = 1;
	long   nUpper = 0, nLower = 0, nPreUpper = -1, nPreLower = -1;
	double dThick = 0, dWidth = 0, dPreThick = -1, dPreWidth = -1;
	double dSta = 0, dPreSta = pGir->GetSSS(OFF_STT_GIRDER);
	CString str = _T("");
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_PLATEHSTIFF);
	while(pBx)
	{
		CWebHStiff *pHs = pBx->GetWebHStiff();
		if (!pHs) break;

		BOOL bLeft = FALSE;
		if(pHs->m_nPos==WEB_SIDE_BOTH || pHs->m_nPos==WEB_SIDE_RIGHT/*pGir->m_nInstallVStiff==0 || pGir->m_nInstallVStiff==2*/)	//양측, 우측
			bLeft = FALSE;
		else
			bLeft = TRUE;
			
		long nC = bLeft ? 0 : 1;

		if((pHs->GetTotalDansu(TRUE, bLeft) >= 0 
		&& pHs->GetTotalDansu(FALSE, bLeft) >= 0)
		/*|| pDB->GetBridgeType()==SOSU_GIR || pOpt->GetDesignHStiffInstall()==1 소수주형인 경우*/)	
		{
			nUpper = pHs->GetTotalDansu(TRUE, bLeft);
			nLower = pHs->GetTotalDansu(FALSE, bLeft);
			dWidth = pHs->m_dWidthHStiff[nC];
			dThick = pHs->m_dThickHStiff[nC];
		}
		if (nUpper != nPreUpper || nLower != nPreLower || dThick != dPreThick || dWidth != dPreWidth)
		{
			dSta = pBx->GetStation();
			if (!pBx->IsState(BX_STT_GIRDER))
			{
				SetMatrixByStation(nRow, nCol, dSta);
				m_Grid.SetTextMatrix(nRow+1, nCol, pHs->m_nPos==WEB_SIDE_BOTH ? "양측" : pHs->m_nPos==WEB_SIDE_LEFT ? "좌측" : pHs->m_nPos==WEB_SIDE_RIGHT ? "우측" : "편측");
				m_Grid.SetTextMatrix(nRow+2, nCol, "%.f", (double)nPreUpper);
				m_Grid.SetTextMatrix(nRow+3, nCol, "%.f", (double)nPreLower);
				m_Grid.SetTextMatrix(nRow+4, nCol, "%.f", dPreWidth);
				m_Grid.SetTextMatrix(nRow+5, nCol, "%.f", dPreThick);
				nCol++;
			}
			dPreSta   = dSta;
			nPreUpper = nUpper;
			nPreLower = nLower;
			dPreWidth = dWidth;
			dPreThick = dThick;
			pPreHs = pHs;
		}		
		pBx = Finder.GetBxNext();
	}
	CString szPos;
	if(pPreHs)	szPos = pPreHs->m_nPos==WEB_SIDE_BOTH ? "양측" : pPreHs->m_nPos==WEB_SIDE_LEFT ? "좌측" : pPreHs->m_nPos==WEB_SIDE_RIGHT ? "우측" : "편측";
	else		szPos = pGir->m_nInstallVStiff==0 ? "양측" : pGir->m_nInstallVStiff==1 ? "좌측" : pGir->m_nInstallVStiff==2 ? "우측" : "편측";
	dSta = pGir->GetSSS(OFF_END_GIRDER);
	SetMatrixByStation(nRow, nCol, dSta);
	nPreUpper = (nPreUpper >= 0) ? nPreUpper : 1;
	nPreLower = (nPreLower >= 0) ? nPreLower : 2;
	dPreWidth = (dPreWidth >= 0) ? dPreWidth : 150;
	dPreThick = (dPreThick >= 0) ? dPreThick : 12;
	m_Grid.SetTextMatrix(nRow+1, nCol, szPos);
	m_Grid.SetTextMatrix(nRow+2, nCol, "%.f", (double)nPreUpper);
	m_Grid.SetTextMatrix(nRow+3, nCol, "%.f", (double)nPreLower);
	m_Grid.SetTextMatrix(nRow+4, nCol, "%.f", dPreWidth);
	m_Grid.SetTextMatrix(nRow+5, nCol, "%.f", dPreThick);
}

void CDetWebHStiffPosDlg::SetDataDefault()
{
	SetGridTitle();	// SetDataDefault()에서 그리드에 값을 직접 쓰기 때문에 여기서 그리드 초기화함.

	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	double dHeight = 0;
	double nQtyRowHStiff = 0;
	for (long nG = 0; nG < m_pStd->GetBridge()->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
		dHeight = pGir->GetHeightMaxByJijum();
		if(!pDB->IsPlateGir()/* || pOpt->GetDesignHStiffInstall()==1*/)	//소수주형
			nQtyRowHStiff = 0;
		else	// 플레이트교량
		{
			if(dHeight > 2500)		nQtyRowHStiff = 2;
			else					nQtyRowHStiff = 1;
		}

		long nRow = nG * 6 + 1;
		if(!pDB->IsPlateGir()/* || pOpt->GetDesignHStiffInstall()==1*/)	//소수주형
		{
			SetMatrixByStation  (nRow,   1, pGir->GetSSS(OFF_END_GIRDER));
			m_Grid.SetTextMatrix(nRow+1, 1, pGir->m_nInstallVStiff==0 ? "양측" : pGir->m_nInstallVStiff==1 ? "좌측" : pGir->m_nInstallVStiff==2 ? "우측" : "편측");
			m_Grid.SetTextMatrix(nRow+2, 1, "%.f", 0.0);
			m_Grid.SetTextMatrix(nRow+3, 1, "%.f", 0.0);
			m_Grid.SetTextMatrix(nRow+4, 1, "%.f", 0.0);
			m_Grid.SetTextMatrix(nRow+5, 1, "%.f", 0.0);
			m_Grid.SetItemState(nRow, 1, m_Grid.GetItemState(nRow, 1) | GVIS_READONLY);
//			m_Grid.SetItemBkColour(nRow, 1, RGBREADONLY);
		}
		else
		{
			SetMatrixByStation  (nRow,   1, pGir->GetSSS(OFF_END_GIRDER));
			m_Grid.SetTextMatrix(nRow+1, 1, pGir->m_nInstallVStiff==0 ? "양측" : pGir->m_nInstallVStiff==1 ? "좌측" : pGir->m_nInstallVStiff==2 ? "우측" : "편측");
			m_Grid.SetTextMatrix(nRow+2, 1, "%.f", 1.0);
			m_Grid.SetTextMatrix(nRow+3, 1, "%.f", nQtyRowHStiff);
			m_Grid.SetTextMatrix(nRow+4, 1, "%.f", 150.0);
			m_Grid.SetTextMatrix(nRow+5, 1, "%.f", 12.0);
			m_Grid.SetItemState(nRow, 1, m_Grid.GetItemState(nRow, 1) & ~GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow, 1, CLR_DEFAULT);
		}

	}
	pDB->m_dHStiffTRibApply	    = 50;
	pDB->m_nHStiffDefault   	= 1;
	pDB->m_cHStiffDimRound   	= FALSE;
	pDB->m_cHStiffDimRoundValue	= 0;
	pDB->m_dHStiffLengthMin	    = 130;
	pDB->m_cHStiffHeightOffset  = FALSE;

	SetDataSave();
	SetDataBx();	// 설정된 데이터를 직접 생성해줌..
}

void CDetWebHStiffPosDlg::SetDataSave()
{
	CString			sText	= _T("");
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();

	UpdateData();

	
	pDB->m_nHStiffDefault      = m_bHstiffDefault;

	m_dHStiffLengthMin.GetWindowText(sText);
	pDB->m_dHStiffLengthMin    = atof(sText);

	m_dHStiffMinLenJackup.GetWindowText(sText);
	pDB->m_dHStiffMinLenJackup = atof(sText);

	pDB->m_nHStiffDefault	   = (BYTE)m_bHstiffDefault;
	pDB->m_cHStiffDimRound     = (BYTE)m_cHStiffDimRound;
	
	m_dHStiffTRibApply.GetWindowText(sText);
	pDB->m_dHStiffTRibApply	   = (BYTE)atof(sText);

	pDB->m_cHStiffDimRoundValue= (BYTE)m_cHStiffDimRoundValue;
	pDB->m_cHStiffHeightOffset = m_cHStiffHeightOffset;

	m_dHStiffHeight1.GetWindowText(sText);
	pDB->m_dHStiffHeight1      = atof(sText);

	m_dHStiffHeight2.GetWindowText(sText);
	pDB->m_dHStiffHeight2      = atof(sText);

	m_dHStiffHeight3.GetWindowText(sText);

	pDB->m_dHStiffHeight3      = atof(sText);

	m_dHStiffMinLenJackup.GetWindowText(sText);
	pDB->m_dHStiffMinLenJackup = atof(sText);

	pDB->m_cHStiffJackupVStiff = m_bJackupVStiff;
	pDB->m_nLengthTypeOnHStiff = m_nLengthType;

	m_dDisCenterOnHStiff.GetWindowText(sText);
	pDB->m_dDisCenterOnHStiff  = atof(sText);
}

void CDetWebHStiffPosDlg::SetDataBx()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	for(long nG = -1; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		// Make CWebHStiff
		CPlateBxFinder Finder(pGir);
		long nMake = 0;
		POS ps = Finder.GetPOSFirst(BX_PLATEHSTIFF);
		while(ps)
		{
			nMake++;
			ps = Finder.GetPOSNext();
		}
		// BX객체를 산정하여 수평보강재를 생성
		pGir->MakeWebStiff(nMake);
		
		// no use to set BxList
		// set LinkToBx in WEBHSTIFF		
		pGir->SetLKClearPOS(CPlateBxObject::LK_WEBHSTIFF);
		
		long nCount = 0;		
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_PLATEHSTIFF);
		while(pBx)
		{
			pBx->SetObjLinkSeq(CPlateBxObject::LK_WEBHSTIFF, nCount++);			
			pBx = Finder.GetBxNext();			
		}
	}
	ClearHStiffObj();	// 수평보강재의 제원을 초기화함.
	SetHStiffObj();		// 수평보강재의 실제 제원을 생성함.
}

//기능 : Set - 수평보강재 실제 객체의 내용
//       수직보강재의 설치방향으로 복부기준으로 배치(양측, 좌측, 우측, 편측)
//<summary>
//
//</summary>
//<param name=""></param>
void CDetWebHStiffPosDlg::SetHStiffObj()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CPlateBasicIndex *pBx, *pBx2;
	for(long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		__int64 BxFlag = 0;
		BOOL bContinue = FALSE;
		BxFlag = /*BX_PLATEHSTIFF;//*/BX_COMMON_PLATEGIRDER | BX_SPLICE;
	
	//LSH_ZZ 2009.08.12 삭제이유 : 수직보강재의 의해서 수평보강재의 설치위치가 영향을 받지 않기 때문.
		// 기본 위치로 Setting.
	/*	pBx = Finder.GetBxFirst(BxFlag);
		while(pBx && Finder.GetBxFixNext())
		{
			pBx2 = Finder.GetBxFixNext();
			if(pBx->GetWebHStiff())
			{
				CWebHStiff *pHs  = pBx->GetWebHStiff();
				if(pHs)
				{			
					if(pGir->m_nInstallVStiff==WEB_SIDE_BOTH)	//양측
					{					
						pHs->m_nPos=WEB_SIDE_BOTH;
						bContinue |= SetHStiffOneObj(pBx, pBx2, TRUE);
						bContinue |= SetHStiffOneObj(pBx, pBx2, FALSE);
					}
					else if(pGir->m_nInstallVStiff==WEB_SIDE_LEFT)	//좌측
					{
						pHs->m_nPos=WEB_SIDE_LEFT;
						bContinue |= SetHStiffOneObj(pBx, pBx2, TRUE);
					}
					else if(pGir->m_nInstallVStiff==WEB_SIDE_RIGHT)	//우측
					{
						pHs->m_nPos=WEB_SIDE_RIGHT;
						bContinue |= SetHStiffOneObj(pBx, pBx2, FALSE);
					}
					else if(pDB->m_nInstallVStiff==WEB_SIDE_AUTO)	//편측
					{
						pHs->m_nPos=WEB_SIDE_AUTO;
						if(nG<pDB->GetGirdersu()/2)		// 교량중심 좌측방향거더들(우측배치)-거더수가 홀수개 일 경우는 중앙거더는 우측으로 포함됨.
							bContinue |= SetHStiffOneObj(pBx, pBx2, FALSE);
						else
							bContinue |= SetHStiffOneObj(pBx, pBx2, TRUE);
					}
				}
			}

			pBx = Finder.GetBxNext();
		}
	*/
		pBx = Finder.GetBxFirst(BxFlag);
		while(pBx && Finder.GetBxFixNext())
		{
			pBx2 = Finder.GetBxFixNext();
			if(pBx->GetWebHStiff())
			{
				CWebHStiff *pHs  = pBx->GetWebHStiff();

				if(pHs)
				{	
					long nG		= pBx->GetNumberGirder();
					long row	= nG*6 + 1;
					long col	= GetGridColumnByBx(pBx, pBx2, row);

					if(nG == -1)	row = 1;

					CString szPos = m_Grid.GetTextMatrix(row+1, col);

					pHs->m_nPos = szPos=="양측" ? 0 : szPos=="좌측" ? 1 : szPos=="우측" ? 2 : 3;
				
					if(pHs->m_nPos == WEB_SIDE_BOTH)
					{
						bContinue |= SetHStiffOneObj(pBx, pBx2, TRUE);
						bContinue |= SetHStiffOneObj(pBx, pBx2, FALSE);
					}
					else if(pHs->m_nPos == WEB_SIDE_LEFT)
					{
						pHs->m_nPos=WEB_SIDE_LEFT;
						bContinue |= SetHStiffOneObj(pBx, pBx2, TRUE);
					}
					else if(pHs->m_nPos == WEB_SIDE_RIGHT)
					{
						pHs->m_nPos=WEB_SIDE_RIGHT;
						bContinue |= SetHStiffOneObj(pBx, pBx2, FALSE);
					}
					/*else if(pDB->m_nInstallVStiff==WEB_SIDE_AUTO)	//편측
					{
						pHs->m_nPos=WEB_SIDE_AUTO;
						if(nG<pDB->GetGirdersu()/2)		// 교량중심 좌측방향거더들(우측배치)-거더수가 홀수개 일 경우는 중앙거더는 우측으로 포함됨.
							bContinue |= SetHStiffOneObj(pBx, pBx2, FALSE);
						else
							bContinue |= SetHStiffOneObj(pBx, pBx2, TRUE);
					}
					*/
				}
			}
			pBx = Finder.GetBxNext();
		}
		// 연결된 구간이 있으면 하나로 합친다.
	/*	if (bContinue)
		{
			double staEnd[WEBHSTIFFSUMAX];
			ZeroMemory(staEnd,sizeof(staEnd));
			Finder.GetBxLast(BxFlag);
			CWebHStiff *pHsN = Finder.GetBxNext()->GetWebHStiff();
			CWebHStiff *pHs  = Finder.GetBxNext()->GetWebHStiff();
			CWebHStiff *pHsP = Finder.GetBxNext()->GetWebHStiff();
			while(pHsN && pHsP)
			{
				SetHStiffTermAdjust(pHs, pHsP, pHsN, staEnd);
				pHsN = pHs;
				pHs  = pHsP;
				pBx  = Finder.GetBxNext();
				pHsP = pBx ? pBx->GetWebHStiff() : NULL;
			}
		}
*/
		// 최소길이 점검, 길이 보정(절사)			
		pBx = Finder.GetBxFirst(BxFlag);
		
		//LSH_ZZ 2009.08.12 삭제이유 : 수직보강재의 의해서 수평보강재의 설치위치가 영향을 받지 않기 때문.
	/*
		while(pBx && Finder.GetBxFixNext())
		{
			pBx2 = Finder.GetBxFixNext();
			CWebHStiff *pHs  = pBx->GetWebHStiff();
			if(pHs)
			{
				if(pGir->m_nInstallVStiff==0)	//양측	pHs->m_nPos==WEB_SIDE_BOTH
				{
					//pHs->m_nPos=WEB_SIDE_BOTH;
					SetHStiffLengthAdjust(pBx, pBx2, TRUE);
					SetHStiffLengthAdjust(pBx, pBx2, FALSE);
				}
				else if(pGir->m_nInstallVStiff==1)	//좌측	pHs->m_nPos==WEB_SIDE_LEFT
				{
					//pHs->m_nPos=WEB_SIDE_LEFT;
					SetHStiffLengthAdjust(pBx, pBx2, TRUE);
				}
				else if(pGir->m_nInstallVStiff==2)	//우측	pHs->m_nPos==WEB_SIDE_RIGHT
				{
					//pHs->m_nPos=WEB_SIDE_RIGHT;
					SetHStiffLengthAdjust(pBx, pBx2, FALSE);
				}
				else if(pDB->m_nInstallVStiff==3)	//편측	pHs->m_nPos==WEB_SIDE_AUTO
				{
					//pHs->m_nPos=WEB_SIDE_AUTO;
					if(nG<pDB->GetGirdersu()/2)		// 교량중심 좌측방향거더들(우측배치)-거더수가 홀수개 일 경우는 중앙거더는 우측으로 포함됨.
						SetHStiffLengthAdjust(pBx, pBx2, FALSE);
					else
						SetHStiffLengthAdjust(pBx, pBx2, TRUE);
				}
			}
			pBx = Finder.GetBxNext();
		}
	*/
		while(pBx && Finder.GetBxFixNext())
		{
			pBx2 = Finder.GetBxFixNext();
			CWebHStiff *pHs  = pBx->GetWebHStiff();
			if(pHs)
			{
				if(pHs->m_nPos==WEB_SIDE_BOTH)	//양측	
				{
					SetHStiffLengthAdjust(pBx, pBx2, TRUE);
					SetHStiffLengthAdjust(pBx, pBx2, FALSE);
				}
				else if(pHs->m_nPos==WEB_SIDE_LEFT)	//좌측	
				{
					SetHStiffLengthAdjust(pBx, pBx2, TRUE);
				}
				else if(pHs->m_nPos==WEB_SIDE_RIGHT)	//우측	
				{
					SetHStiffLengthAdjust(pBx, pBx2, FALSE);
				}
			/*	else if(pDB->m_nInstallVStiff==3)	//편측	pHs->m_nPos==WEB_SIDE_AUTO
				{
					//pHs->m_nPos=WEB_SIDE_AUTO;
					if(nG<pDB->GetGirdersu()/2)		// 교량중심 좌측방향거더들(우측배치)-거더수가 홀수개 일 경우는 중앙거더는 우측으로 포함됨.
						SetHStiffLengthAdjust(pBx, pBx2, FALSE);
					else
						SetHStiffLengthAdjust(pBx, pBx2, TRUE);
				}
			*/
			}
			pBx = Finder.GetBxNext();
		}
	}	
}

//<summary>
//(1) 수평보강재 길이가 잭업-잭업간 최소 길이보다 작은가를 점검
//(2) 수평보강재 길이가 일반구간 최소 길이보다 작은가를 점검
//(3) 수평보강재 길이를 치수보정 한 값을 적용
//</summary>
//<param name="*pBx">현재 세팅할 수평보강재</param>
//<param name="*pBxN">다음 수평보강재가 있는 Bx</param>
//<param name="bLeft"></param>
void CDetWebHStiffPosDlg::SetHStiffLengthAdjust(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, BOOL bLeft)
{
	BOOL bPrevJackup = pBx->IsState(BX_JACKUP);
	BOOL bNextJackup = pBxN->IsState(BX_JACKUP);

	CPlateGirderApp *pGir	= pBx->GetGirder();
	double			dRound	= m_cHStiffDimRoundValue ? 10 : 5;	
	long			nC		= bLeft ? 0 : 1;
	CString			sText	= _T("");	

	for (long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		CWebHStiff *pHs  = pBx->GetWebHStiff();
		if(pHs->IsEmpty(nDan, bLeft))			continue;
		
		double dPrevSta = pHs->m_dStationStt[nC][nDan];
		double dNextSta = pHs->m_dStationEnd[nC][nDan];
		double dHFactor = pHs->m_dHeightFactor[nC][nDan];
		double dPrevEle = pGir->GetElevationFlangeCamber(dPrevSta, 0) - pHs->GetHeightHStiffOnStt(nDan, bLeft, TRUE);
		double dNextEle = pGir->GetElevationFlangeCamber(dNextSta, 0) - pHs->GetHeightHStiffOnStt(nDan, bLeft, FALSE);

		double dXYLen = pGir->GetLengthDimType(dPrevSta, dNextSta, 0);
		double dLength = hypot(dXYLen, dNextEle - dPrevEle);	//수평보강재 실제 길이
		if(bPrevJackup && bNextJackup)	//잭업-잭업 최소 간격
		{
			m_dHStiffMinLenJackup.GetWindowText(sText);

			if(dLength < atof(sText))
				pHs->m_dHeightFactor[nC][nDan] = 0.0;

			continue;		//잭업-잭업 이면 다음으로 진행안하고 continue 함.
		}
		
		m_dHStiffLengthMin.GetWindowText(sText);
		if(dPrevSta >= dNextSta || dLength < atof(sText)) //일반 구간 최소길이 점검(잭업-잭업 제외)
		{
			pHs->m_dHeightFactor[nC][nDan] = 0.0;
			continue;		//일반 구간 최소길이 보다 작을 경우 continue 함.
		}

		if(m_cHStiffDimRound)  //치수보정 (절사)
		{
			double dMod = fmod(dLength, dRound);
			double dRoundOff = dMod * dXYLen / dLength;
			double dPrevRoundOff = dRoundOff / 2;
			double dNextRoundOff = dRoundOff / 2;
			if (bPrevJackup)   
			{
				dPrevRoundOff = 0;
				dNextRoundOff = dRoundOff;
			}
			if (bNextJackup)   
			{
				dPrevRoundOff = dRoundOff;
				dNextRoundOff = 0;
			}
			pHs->m_dStationStt[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] = pGir->GetStationByActLength(dPrevSta, dPrevRoundOff);
			pHs->m_dStationEnd[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] = pGir->GetStationByActLength(dNextSta, -dNextRoundOff);
		}
	}
}

//<summary>
//
//</summary>
//<param name="*pBx">현재 Bx, 수평보강재 이전 Bx</param>
//<param name="*pBxN">다음 Bx, 수평보강재 이후 Bx</param>
//<param name="bLeft">-1 = 좌측, 1 = 우측, 0 = 중앙</param>
//<return></return>
BOOL CDetWebHStiffPosDlg::SetHStiffOneObj(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, BOOL bLeft)
{	
	CPlateGirderApp *pGir = pBx->GetGirder();
	CWebHStiff *pHs  = pBx->GetWebHStiff();
	long nC = bLeft ? 0 : 1;
	
	double dPrevLen = 0, dNextLen = 0, dHFactor = 0;
	BOOL bContinue = FALSE;

	long nG = pBx->GetNumberGirder();
	long row = nG*6 + 1;
	if(nG == -1)	row = 1;

	long col = GetGridColumnByBx(pBx, pBxN, row);
	CString szPos = m_Grid.GetTextMatrix(row+1, col);
	pHs->m_nPos = szPos=="양측" ? 0 : szPos=="좌측" ? 1 : szPos=="우측" ? 2 : 3;
	pHs->SetTotalDansu(TRUE,  bLeft, atol(m_Grid.GetTextMatrix(row+2, col)));
	pHs->SetTotalDansu(FALSE, bLeft, atol(m_Grid.GetTextMatrix(row+3, col)));
	pHs->m_dWidthHStiff[nC] = atof(m_Grid.GetTextMatrix(row+4, col));
	pHs->m_dThickHStiff[nC] = atof(m_Grid.GetTextMatrix(row+5, col));
	
	//070105수정...KB..070109..Revision 2			
	BOOL bUPrevPress = pGir->m_pRangePress->IsCompressOnBox(TRUE,pBx);
	//BOOL bUNextPress = pGir->m_pRangePress->IsCompressOnBox(TRUE,pBxN);
	long nUpper = (bUPrevPress) ? pHs->GetTotalDansu(TRUE, bLeft) : 0;

	BOOL bLPrevPress = pGir->m_pRangePress->IsCompressOnBox(FALSE,pBx);
	//BOOL bLNextPress = pGir->m_pRangePress->IsCompressOnBox(FALSE,pBxN);
	long nLower = (bLPrevPress) ? pHs->GetTotalDansu(FALSE, bLeft) : 0;
	
/*  이방식으로 도저히 안됨.... 헥헥...
	//1=상부압축, 2=하부압축, 3=교번부 리턴	
 	long nPrevComp = pBx->GetCompressTypeByComplex();
	long nNextComp = pBxN->GetCompressTypeByComplex();
	long nUpper = ((nNextComp==2) && (nPrevComp==2 || nPrevComp==3)) ? 0 : pHs->GetTotalDansu(TRUE, bLeft);//하부 압축이면 상부 0..	
	long nLower = ((nNextComp==1) && (nPrevComp==1 || nPrevComp==3)) ? 0 : pHs->GetTotalDansu(FALSE, bLeft);//상부 압축이면 하부 0...
*/

	CString	sText	= _T("");

	for (long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		dHFactor = GetHFactor(nUpper, nLower, nDan);
		pHs->m_dHeightFactor[nC][nDan] = dHFactor;
		if (dHFactor == 0.0)
			continue;

		if (m_cHStiffHeightOffset && nDan < WEBHSTIFFSUMAX/2) 
		{
			switch(nDan)   
			{
			case 0 : 
				m_dHStiffHeight1.GetWindowText(sText);
				dHFactor = atof(sText);	
				break;
			case 1 : 
				m_dHStiffHeight2.GetWindowText(sText);
				dHFactor = atof(sText);	
				break;
			case 2 : 
				m_dHStiffHeight3.GetWindowText(sText);
				dHFactor = atof(sText);	
				break;
			}
			pBx->GetHeightGirderByBx();
			pHs->m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] = dHFactor;
		}

		double dPrevH = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
		double dNextH = pHs->GetHeightHStiffBySta(nDan, bLeft, pBxN->GetStation());
		double dSlop = GetSlopHStiff(pBx, pBxN, dPrevH, dNextH, 0);

		if(pBx->IsState(BX_STT_GIRDER))
			dPrevLen = GetOffsetHStiffOnSttEnd(pBx, dPrevH);
		if(pBx->IsState(BX_SPLICE))
			dPrevLen = GetOffsetHStiffOnSplice(pBx, dPrevH, bLeft, dSlop, FALSE);
		if(pBx->IsState(BX_CROSSBEAM_VBRACING))
			dPrevLen = GetOffsetHStiffOnCrossBeam(pBx, dPrevH, bLeft, dSlop);
		if(pBx->IsState(BX_VSTIFF))
			dPrevLen = GetOffsetHStiffOnVStiff(pBx, dPrevH, bLeft, dSlop, TRUE);
		if(pBx->IsState(BX_JACKUP))		
		{
			CJackupStiff *pJack = pBx->GetJackupStiff();
			double dHBox=pGir->GetHeightGirderByStaAng(pBx->GetStation());
			if(pJack->m_dVL==0)			
				dPrevLen = GetOffsetHStiffOnJackup(pBx, bLeft, pBxN);
			else if(dHBox - pJack->m_dVL < dPrevH)
				dPrevLen = GetOffsetHStiffOnJackup(pBx, bLeft, pBxN);
			//dPrevLen = GetOffsetHStiffOnJackup(pBx, bLeft, pBxN);		
		}
		if(pBx->IsState(BX_JIJUMSTIFF))
			dPrevLen = GetOffsetHStiffOnJijumStiff(pBx, bLeft, pBxN);		

		if(pBxN->IsState(BX_END_GIRDER))
			dNextLen = GetOffsetHStiffOnSttEnd(pBxN, dNextH);
		if(pBxN->IsState(BX_SPLICE))
			dNextLen = GetOffsetHStiffOnSplice(pBxN, dNextH, bLeft, -dSlop, TRUE);
		if(pBxN->IsState(BX_CROSSBEAM_VBRACING))
			dNextLen = GetOffsetHStiffOnCrossBeam(pBxN, dNextH, bLeft, -dSlop);
		if(pBxN->IsState(BX_VSTIFF))
			dNextLen = GetOffsetHStiffOnVStiff(pBxN, dNextH, bLeft, -dSlop, FALSE);
		if(pBxN->IsState(BX_JACKUP))		
		{
			CJackupStiff *pJack = pBxN->GetJackupStiff();
			double dHBox=dHBox = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			if(pJack->m_dVL==0)			
				dNextLen = GetOffsetHStiffOnJackup(pBxN, bLeft, pBx);
			else if(dHBox - pJack->m_dVL < dNextH)
				dNextLen = GetOffsetHStiffOnJackup(pBxN, bLeft, pBx);
		}
			//dNextLen = GetOffsetHStiffOnJackup(pBxN, bLeft, pBx);		
		if(pBxN->IsState(BX_JIJUMSTIFF))
			dNextLen = GetOffsetHStiffOnJijumStiff(pBxN, bLeft, pBx);		

		if (dPrevLen == 0 || dNextLen == 0)
			bContinue = TRUE;

		pHs->m_dStationStt[nC][nDan] = pGir->GetStationByActLength(pBx->GetStation(), dPrevLen, 0);
		pHs->m_dStationEnd[nC][nDan] = pGir->GetStationByActLength(pBxN->GetStation(), -dNextLen, 0);

		if (pBx->IsState(BX_JACKUP) || pBxN->IsState(BX_JACKUP))	//시작과 끝이 잭업일 경우에만 잭업에 접착
		{
			if(pBx->IsState(BX_JACKUP))
			{
				CJackupStiff* pJack = pBx->GetJackupStiff();
				if(pJack == NULL)	pHs->m_dDistStt[nC][nDan] = pHs->m_dWidthHStiff[nC]-10;
				else				pHs->m_dDistStt[nC][nDan] = min(pHs->m_dWidthHStiff[nC], pJack->GetWidth(bLeft))-10;
			}
			else if(pBx->IsState(BX_JIJUMSTIFF))
			{
				CVStiff* pStiff = ((CPlateBasicIndex *)pBx)->GetJijumStiff();
				if(pStiff == NULL)	pHs->m_dDistStt[nC][nDan] = pHs->m_dWidthHStiff[nC]-10;
				else				pHs->m_dDistStt[nC][nDan] = min(pHs->m_dWidthHStiff[nC], pStiff->m_dWidth[0])-10;
			}
			else
			{
				pHs->m_dDistStt[nC][nDan] = pBx->GetGirder()->m_pSangse->m_HStiff_VStiff_D;
			}
			if(pBxN->IsState(BX_JACKUP))
			{
				CJackupStiff* pJack = pBxN->GetJackupStiff();
				if(pJack == NULL)	pHs->m_dDistEnd[nC][nDan] = pHs->m_dWidthHStiff[nC]-10;
				else				pHs->m_dDistEnd[nC][nDan] = min(pHs->m_dWidthHStiff[nC], pJack->GetWidth(bLeft))-10;
			}
			else if(pBxN->IsState(BX_JIJUMSTIFF))
			{
				CVStiff* pStiff = ((CPlateBasicIndex *)pBxN)->GetJijumStiff();
				if(pStiff == NULL)	pHs->m_dDistStt[nC][nDan] = pHs->m_dWidthHStiff[nC]-10;
				else				pHs->m_dDistStt[nC][nDan] = min(pHs->m_dWidthHStiff[nC], pStiff->m_dWidth[0])-10;
			}
			else
			{
				pHs->m_dDistEnd[nC][nDan] = pBx->GetGirder()->m_pSangse->m_HStiff_VStiff_D;
			}
		}
		else
		{
			pHs->m_dDistStt[nC][nDan] = pBx->GetGirder()->m_pSangse->m_HStiff_VStiff_D;
			pHs->m_dDistEnd[nC][nDan] = pBx->GetGirder()->m_pSangse->m_HStiff_VStiff_D;

			double dMin = (pHs->m_dWidthHStiff[nC] - pBx->GetGirder()->m_pSangse->m_HStiff_VStiff_D)*2;
			double dLengthHStiff = pHs->GetLength(nDan, bLeft, DIM_BASIC) ;
			if(dLengthHStiff > 0 && dLengthHStiff < dMin) //수평보강재 꺽임으로 인해 오류가 있을 경우
			{
				if(pBx->IsState(BX_SPLICE))	
					pHs->m_dDistStt[nC][nDan] = RoundUp(dMin - dLengthHStiff + 20, -1);
				if(pBxN->IsState(BX_SPLICE))	
				{
					pHs->m_dDistEnd[nC][nDan] = RoundUp(dMin - dLengthHStiff + 20, -1);
				}
			}
		}

	}
	
	return bContinue;
}

void CDetWebHStiffPosDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,nCol));
}

void CDetWebHStiffPosDlg::SetMatrixByStation(long nRow, long nCol, double sta)
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	double lenSum = 0;
	for(long c = 1; c < nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);

	CString	sText	= _T("");

	m_dDisCenterOnHStiff.GetWindowText(sText);

	double	len		= pDB->GetLengthByStationInGrid(FALSE, nRow/5, sta, m_nLengthType, atof(sText)) - lenSum;

	m_Grid.SetTextMatrix(nRow, nCol, "%.0f", len);
}

long CDetWebHStiffPosDlg::GetGridColumnByBx(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, long nRow)
{
	long nCols = m_Grid.GetCols();
	if (nCols <= 2)		return 1;  // 수행속도가 빨라지려나?
	double dStaMid = (pBx->GetStation() + pBxN->GetStation()) / 2;
	double sta = pBx->GetGirder()->GetSSS(OFF_STT_GIRDER);
	long nCol = 0;
	for (nCol = 1; nCol < nCols - 1; nCol++)
	{
		sta = GetStationMatrix(nRow, nCol);
		if (sta > dStaMid)			break;
	}
	return nCol;
}

double CDetWebHStiffPosDlg::GetStationMatrix(long nRow, long nCol)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	
	double lenSum = 0;
	for(long c = 1; c <= nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);

	CString	sText	= _T("");
	
	m_dDisCenterOnHStiff.GetWindowText(sText);

	double	sta = pDB->GetStationByLengthInGrid(FALSE, nRow/5, lenSum, m_nLengthType, atof(sText));
	return sta;
}

//입력 : nUpper : 상부 수평보강재 개수
//		 nLower : 하부 수평보강재 개수
//		 nDan	: 현재 수평보강재 단수
//기능 : 현재 Bx에서의 보강재 단수에 해당하는 높이비율 구함.
double CDetWebHStiffPosDlg::GetHFactor(long nUpper, long nLower, long nDan)
{
	double dRatio = 0;
	long nHStiff = nUpper;
	long nDanWeb = nDan;

	if (nDan >= 3)
	{  // 단수가 하부 위치면
		nHStiff = nLower;
		nDanWeb = nLower - nDan + 2;
	}
	if (nDanWeb < 0 || nDanWeb >= nHStiff)		return 0.0;

	if (nHStiff == 1)
	{
		if (nDanWeb == 0)		dRatio = 0.2;
	}
	if (nHStiff == 2)
	{
		if (nDanWeb == 0)		dRatio = 0.14;
		if (nDanWeb == 1)		dRatio = 0.36;
	}
	if (nHStiff == 3)
	{
		if (nDanWeb == 0)		dRatio = 0.14;
		if (nDanWeb == 1)		dRatio = 0.26;
		if (nDanWeb == 2)		dRatio = 0.40;
	}
	if (nDan >= 3)				dRatio = 1.0 - dRatio;

	return dRatio;
}

//기능 : 수평보강재의 실제 기울기를 구한다.
// (Data Setting 중이기 때문에 아직은 CWebHStiff::GetAngleRadianHStiff 사용못함.)
double CDetWebHStiffPosDlg::GetSlopHStiff(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, double dPrevH, double dNextH, double dWidth)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	double staPrev  = pBx->GetStation();
	double staNext  = pBxN->GetStation();
	double dElePrev = pGir->GetElevationFlangeCamber(staPrev, dWidth) - dPrevH;
	double dEleNext = pGir->GetElevationFlangeCamber(staNext, dWidth) - dNextH;
	double dDis = pGir->GetLengthDimType(staPrev, staNext, dWidth);
	double dSlop = (dEleNext - dElePrev) / dDis;
	return dSlop;
}

//기능 : 수평보강재와 거더 시종점과의 실제 이격거리 구함
double CDetWebHStiffPosDlg::GetOffsetHStiffOnSttEnd(CPlateBasicIndex *pBx, double dStiffH)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CPlateGirderApp *pGir = pBx->GetGirder();
	BOOL bStt = pBx->IsState(BX_STT_GIRDER);
	double dInoutH = pGir->GetHeightInOutUpper(bStt);

	double dLen = pGir->m_pSangse->m_HStiff_VStiff_B;
	if (dStiffH > dInoutH - 30)		dLen += bStt ? pDB->m_dWidthInOutStt : pDB->m_dWidthInOutEnd;

	return dLen;
}

//기능 : 수평보강재와 현장이음 중심선과의 실제 이격거리 구함
double CDetWebHStiffPosDlg::GetOffsetHStiffOnSplice(CPlateBasicIndex *pBx, double dStiffH, BOOL bLeft, double dSlop, BOOL bFront)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateSplice *pSp = pBx->GetSplice();
	double sta = pBx->GetStation();
	double dBoxH = pGir->GetHeightGirderByStaAng(sta, 0);
	BOOL bUpper = (dStiffH < dBoxH * 0.5);
	if (!bUpper)
		dStiffH = dBoxH - dStiffH;

	double dWidthShear  = pSp->m_sA + pSp->m_sB*pSp->m_sC + pSp->m_sC1/2;
	double dWidthMoment = pSp->m_mA + pSp->m_mB*pSp->m_mC + pSp->m_mC1/2;
	double dHeightMoment = pSp->m_mG + pSp->m_mW + pDB->m_dHStiffTRibApply;

	if(!pSp->m_bSpliceType)
	{
		dWidthMoment	= 0;
		dWidthShear		= 0;
		dHeightMoment	= pDB->m_dHStiffTRibApply;
	}
	double dSpliceSlop = 0;
	double dEle1 = dHeightMoment + dWidthMoment * dSpliceSlop * (bFront ^ bUpper ? -1 : 1);
	double dEle2 = dHeightMoment + dWidthShear  * dSpliceSlop * (bFront ^ bUpper ? -1 : 1);
	double dRealH1 = dStiffH - dSlop * dWidthMoment * (bUpper ? 1 : -1);
	double dRealH2 = dStiffH - dSlop * dWidthShear * (bUpper ? 1 : -1);

	double dLen = pGir->m_pSangse->m_HStiff_Splice_B;
	if (dRealH1 < dEle1 || dRealH2 < dEle2)
		dLen += dWidthMoment;
	else
		dLen += dWidthShear;

	return dLen;
}

//기능 : 수평보강재와 단면 중심선과의 실제 이격거리 구함
double CDetWebHStiffPosDlg::GetOffsetHStiffOnVStiff(CPlateBasicIndex *pBx, double dStiffH, BOOL bLeft, double dSlop, BOOL bStt)
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CWebHStiff *pHs  = pBx->GetWebHStiff();
	if(pHs == NULL)	return 0;
	CSection *pSec = ((CPlateBasicIndex *)pBx)->GetSection();
	if(pSec->GetenDetType() != SECDET_VSITFF) 
	{
		ASSERT(FALSE);
		return 0;
	}

	CVStiff *pVStiff = pBx->GetSection()->GetVStiff();	
	CPlateGirderApp *pGir = pBx->GetGirder();

	double sta = pBx->GetStation();
	double dBoxH = pGir->GetHeightGirderByStaAng(sta, 0);
	double dMaxW = bLeft ? pVStiff->m_dThick[LEFT]/2 : pVStiff->m_dThick[RIGHT]/2;	

	// 수평 브레이싱 브라켓
	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();
	if(pBrGu)
	{	
		double dGap = pDB->m_dHStiffTRibApply + (pBrGu->m_dT + pHs->m_dThickHStiff[bLeft ? 0 : 1])/2;		
		double dEle = dBoxH - pBrGu->m_ddH;
		CDPoint xyDirLowerFL = CDPoint(1, pGir->GetGiulFlangeUpperLower(sta, 0, FALSE)).Unit();

		if(fabs(dStiffH-dEle)<pBrGu->m_dT)
		{
			if(bStt)	dMaxW = max(dMaxW, (xyDirLowerFL * max(pBrGu->m_dWidth[1], pBrGu->m_dWidth[3])).x);
			else		dMaxW = max(dMaxW, (xyDirLowerFL * max(pBrGu->m_dWidth[0], pBrGu->m_dWidth[2])).x);
		}
	}	
	return pGir->m_pSangse->m_HStiff_VStiff_B + dMaxW;	
}


//<summary>
//수평보강재와 다이아프램 중심선과의 실제 이격거리 구함
//</summary>
//<param name="*pBx">수평보강재 앞의 Bx</param>
//<param name="dStiffH">pBx 위치에서의 수평보강재 높이</param>
//<param name="bLeft">-1= 좌측, 0=중앙, 1=우측</param>
//<param name="dSlop">수평보강재 기울기</param>
//<return></return>
double CDetWebHStiffPosDlg::GetOffsetHStiffOnCrossBeam(CPlateBasicIndex *pBx, double dStiffH, BOOL bLeft, double dSlop)
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CWebHStiff		*pHs	= pBx->GetWebHStiff();
	if(pHs == NULL)	return 0;
	CSection *pSec = ((CPlateBasicIndex *)pBx)->GetSection();
	if(pSec->GetenDetType() != SECDET_CROSSBEAM && pSec->GetenDetType() != SECDET_VBRACING) 
	{
		ASSERT(FALSE);
		return 0;
	}

	CPlateGirderApp *pGir = pBx->GetGirder();
	long	nG		= pBx->GetNumberGirder();
	double	sta		= pBx->GetStation();
	double	dMaxW	= 0;
	if(pSec->GetenDetType() == SECDET_CROSSBEAM)
	{
		CPlateCrossBeam	*pCross = pSec->GetCrossBeam();
		if(nG==pDB->GetQtyGirder()-1)
			pCross = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft)->GetSection()->GetCrossBeam();
		
		if(!pCross) return 0.0;

		double dGap = pDB->m_dHStiffTRibApply + (pCross->m_T3_CR+pHs->m_dThickHStiff[bLeft ? 0 : 1])/2;
		double dEle = pCross->m_dLH1 + pCross->m_T2_CR/2;
		dMaxW = pCross->m_T3_CR/2;	
		// 상부 브라켓
		if (fabs(dStiffH - dEle) < dGap)
		{
			dMaxW = max(dMaxW, pCross->m_uM_CR+pCross->m_uN_CR);			
		}
		// 하부 브라켓
		dEle = pCross->m_dLH1 + pCross->m_T2_CR + pCross->m_H_CR + pCross->m_T1_CR/2;
		if (fabs(dStiffH - dEle) < dGap)
		{
			dMaxW = max(dMaxW, pCross->m_uM_CR+pCross->m_dN_CR);			
		}
	}
	else
	{
		CVStiff *pVStiff = ((CPlateBasicIndex *)pBx)->GetSection()->GetVStiff();	
		dMaxW = bLeft ? pVStiff->m_dThick[LEFT]/2 : pVStiff->m_dThick[RIGHT]/2;
	}

	double dBoxH = pGir->GetHeightGirderByStaAng(sta, 0);		
//	pBx = pBx->GetBxCenter();
//	ASSERT(pBx->GetAngle().y);
	double sinValue = pBx->GetAngle().y;
	if(sinValue == 0)	sinValue = 1;
	double Ratio = 1 / sinValue;
	return pGir->m_pSangse->m_HStiff_VStiff_B + dMaxW * Ratio;		
}

//기능 : 수평보강재와 잭업보강재 중심선과의 실제 이격거리 구함
double CDetWebHStiffPosDlg::GetOffsetHStiffOnJackup(CPlateBasicIndex *pBx, BOOL bLeft, CPlateBasicIndex *pBxN)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CJackupStiff* pJack = pBx->GetJackupStiff();
	if(!pJack) return 0;
	double dLen = pJack->GetThick(FALSE) / 2;
	if (m_bJackupVStiff && pBxN->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_JIJUMSTIFF))
		dLen += pGir->m_pSangse->m_HStiff_VStiff_B;	
	return dLen;
}

double CDetWebHStiffPosDlg::GetOffsetHStiffOnJijumStiff(CPlateBasicIndex *pBx, BOOL bLeft, CPlateBasicIndex *pBxN)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CVStiff* pVStiff = ((CPlateBasicIndex *)pBx)->GetJijumStiff();
	if(!pVStiff) return 0;
	long nCol = bLeft ? 0 : 1;
	double dLen = pVStiff->m_dThick[nCol] / 2;
	if (m_bJackupVStiff && pBxN->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_JACKUP))
		dLen += pGir->m_pSangse->m_HStiff_VStiff_B;	
	return dLen;
}


// 
//<summary>
//연결된 구간이면 수평보강재 하나로 합친다.
//코드가 문제가 있음. pStaEnd의 역할이 없음. 코드가 하는 일이 없음.
//</summary>
//<param name="*pHs">2번째 수평보강재</param>
//<param name="*pHsP">1번째 수평보강재</param>
//<param name="*pHsN">3번째 수평보강재</param>
//<param name="*pStaEnd"></param>
void CDetWebHStiffPosDlg::SetHStiffTermAdjust(CWebHStiff *pHs, CWebHStiff *pHsP, CWebHStiff *pHsN, double *pStaEnd)
{
/*	BOOL bLeft = TRUE;
	long nC = bLeft ? 0 : 1;

	for (long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		if(pHs->IsEmpty(nDan, bLeft))
			continue;
		BOOL bSttContinue = !pHsP->IsEmpty(nDan, bLeft) && pHs->m_dStationStt[nDan] == pHsP->m_dStationEnd[nDan];
		BOOL bEndContinue = (!pHsN->IsEmpty(nDan, bLeft) || pStaEnd[nDan] > 0) && pHs->m_dStationEnd[nDan] == pHsN->m_dStationStt[nDan];

		if(bSttContinue && !bEndContinue)  // 연결구간의 끝부분
			pStaEnd[nDan] = pHs->m_dStationEnd[bLeft ? WEB_LEFT : WEB_RIGHT][nDan];

		if(!bSttContinue && bEndContinue)   // 연결구간의 앞부분
		{
			pHs->m_dStationEnd[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] = pStaEnd[nDan];
			pStaEnd[nDan] = 0.0;
		}
		if (bSttContinue)   // 연결 구간의 중간, 끝부분
			pHs->m_dHeightFactor[bLeft ? WEB_LEFT : WEB_RIGHT][nDan] = 0.0;
	}*/
}


void CDetWebHStiffPosDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;	
	
	if((nRow-1)%6==2 || (nRow-1)%6==3)
	{
		long nData = m_Grid.GetTextMatrixLong(nRow,nCol);	
//		if(nData==0 && pDB->GetBridgeType()!=SOSU_GIR /*소수주형이 아닐 경우*/)	
		if(nData<0)	
		{
//			AfxMessageBox("단수는 1단 이상이어야 합니다.", IDOK | MB_ICONEXCLAMATION);
			m_Grid.SetTextMatrix(nRow,nCol,"0");
		}
	}
}

void CDetWebHStiffPosDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;	

	EnableWindow();
	if( m_Row == -2 ) return;
	if( nRow == 0 || nCol == 0 ) return;

	m_Row = nRow;
	m_Col = nCol;

	nRow -= (nRow - 1) % 6;
	double presta = GetStationMatrix(nRow, nCol-1);
	double sta    = GetStationMatrix(nRow, nCol);
	double dWGir  = 0; 
	
	if(m_nTab==1)	return;

	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(FALSE);
	pDom->FixEtt(TRUE);
	
	if(m_bSameApplySameCol)
	{
		pDom->DeleteCursor();
		for(long nG=0;nG<m_pStd->GetBridge()->GetGirdersu();nG++)
		{
			CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
			presta = GetStationMatrix(nG*6+1, nCol-1);
			sta    = GetStationMatrix(nG*6+1, nCol);
			dWGir  = pGir->GetWidthOnStation(presta, TRUE);
			Draw.SetCursorPosition(nG, presta, sta, pDom, FALSE);
		}
	}
	else
		Draw.SetCursorPosition(nRow/6, presta, sta, pDom, FALSE);

	pDom->FixEtt(FALSE);

	return;
}

void CDetWebHStiffPosDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
//	long	nRow = m_Grid.GetFocusCell().row;
//	long	nCol = m_Grid.GetFocusCell().col;
//	long	nG	= (nRow-1) / 6;
//	long	nGsu = nG+1;
//
//	if( m_bSameApplySameCol )
//	{
//		nG = 0;
//		nGsu = m_pStd->GetBridge()->GetGirdersu();
//	}
//	for(nG; nG < nGsu; nG++)
//	{
//		CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
//		long row = nG * 6 + 1;
//		for (long r = 0; r < 5; r++)
//		{
//			if (row + r != nRow)
//				m_Grid.InsertColumn("", DT_CENTER|DT_VCENTER|DT_SINGLELINE, nCol, row + r);
//		}
//		if (nCol > m_Grid.GetValidCol(row, FALSE))
//			continue;
//
//		double staPre = GetStationMatrix(row, nCol - 1);
//		double sta = GetStationMatrix(row, nCol + 1);
//		double staMid = (staPre + sta) / 2;
//		staMid = Round(pGir->GetBxByStationDir(staMid, 0, BX_PLATEHSTIFF)->GetStation(), 0);
//		if (staMid == staPre || staMid == sta)
//			break;
//		SetMatrixByStation(row, nCol, staMid);
//		SetMatrixByStation(row, nCol + 1, sta);
//		for(r = 1; r < 6; r++)
//			m_Grid.SetTextMatrix(row + r, nCol, m_Grid.GetTextMatrix(row + r, nCol + 1));
//	}
//	SetGridTitle(m_Grid.GetCols());
//	m_Grid.Invalidate();
//	m_Grid.SetFocus();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
//	long nRow = GridView->iRow;
    long nCol = GridView->iColumn;

	long	nGsu = m_pStd->GetBridge()->GetGirdersu()+1;
	CString str;
	for(long nG	= 0; nG < nGsu; nG++)
	{
		for(long row = nG*6+1; row < nG*6+7; row++)
		{
			if(row%6 == 1)	str.Format("%.lf", m_Grid.GetTextMatrixDouble(row, nCol+1)/2);
			else			str = m_Grid.GetTextMatrix(row, nCol+1);
			m_Grid.SetTextMatrix(row, nCol, str);
			m_Grid.SetTextMatrix(row, nCol+1, str);
		}
	}
	SetGridTitle(m_Grid.GetCols());
}

//기능 : 모든 거더의 현재 컬럼을 삭제함.
//		 []모든 거더 동일적용(m_bSameApplySameCol)	는 적용되지 않음.
void CDetWebHStiffPosDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{
//	long	nRow = m_Grid.GetFocusCell().row;
//	long	nCol = m_Grid.GetFocusCell().col;
//	long	nG	= (nRow - 1) / 5;
//	long	nGsu = nG+1;
//
//	if( m_bSameApplySameCol )
//	{
//		nG = 0;
//		nGsu = m_pStd->GetBridge()->GetGirdersu();
//	}
//	for(nG; nG < nGsu; nG++)
//	{
//		CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
//		long row = nG * 6 + 1;
//		if (nCol >= m_Grid.GetValidCol(row, FALSE))
//			continue;
//
//		double len = m_Grid.GetTextMatrixDouble(row, nCol) + m_Grid.GetTextMatrixDouble(row, nCol+1);
//		m_Grid.SetTextMatrix(row, nCol+1, "%.lf", len);
//		for (long r = 0; r < 5; r++)   {
//			if (row + r != nRow)
//				m_Grid.DeleteColumn(nCol, row + r);
//		}
//	}
//	SetGridTitle(m_Grid.GetCols());

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	long nCol = GridView->iColumn;
	if(nCol == m_Grid.GetCols()-1)	return;

	long	nGsu = m_pStd->GetBridge()->GetGirdersu();
	CString str;

	for(long nG = 0; nG < nGsu; nG++)
	{
		str.Format("%.lf", m_Grid.GetTextMatrixDouble(nG*6+1, nCol)+m_Grid.GetTextMatrixDouble(nG*6+1, nCol+1));
		m_Grid.SetTextMatrix(nG*6+1, nCol+1, str, (UINT)DT_CENTER);
	}
}

void CDetWebHStiffPosDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{
	SetGridTitle(m_Grid.GetCols());
}

void CDetWebHStiffPosDlg::OnKillfocusHstiffTribapply()
{
	UpdateData(TRUE);
}

void CDetWebHStiffPosDlg::OnKillfocusHstiffDistanceCenter()
{
	long	nOldType	= m_nLengthType;
	CString	sText		= _T("");
	
	m_dDisCenterOnHStiff.GetWindowText(sText);

	double dOldDisCenter = atof(sText);
	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
}

void CDetWebHStiffPosDlg::OnHstiffGirderlen() 
{
	long nOldType = m_nLengthType;
	CString	sText		= _T("");
	
	m_dDisCenterOnHStiff.GetWindowText(sText);

	double dOldDisCenter = atof(sText);

	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
}

void CDetWebHStiffPosDlg::OnHstiffBridgelen() 
{
	long	nOldType	= m_nLengthType;
	CString	sText		= _T("");
	
	m_dDisCenterOnHStiff.GetWindowText(sText);

	double dOldDisCenter = atof(sText);

	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
}

void CDetWebHStiffPosDlg::SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter)
{
	CPlateBridgeApp* pDB = m_pStd->GetBridge();

	long nRows = m_Grid.GetRows();
	long nCols = m_Grid.GetCols();

	for (long nRow = 1; nRow < nRows; nRow += 6)   {
		CPlateGirderApp *pGir = pDB->GetGirder(nRow / 6);
		double staStt = pGir->GetSSS(OFF_STT_GIRDER);
		double sta, lenSum = 0;
		for (long nCol = 1; nCol < nCols; nCol++)   {
			lenSum += m_Grid.GetTextMatrixDouble(nRow, nCol);
			if(nCol == nCols-1) 
			{
				sta = pGir->GetSSS(OFF_END_SHOE);
			}
			else
			{
				if (nOldType == 0)
					sta = pGir->GetStationByActLength(staStt, lenSum);
				else
					sta = pGir->GetLine()->GetStationByDisLength(staStt, lenSum, dOldDisCenter);
			}
			SetMatrixByStation(nRow, nCol, sta);
		}
	}
	m_Grid.Invalidate();
}

void CDetWebHStiffPosDlg::ClearHStiffObj()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	for(long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_PLATEHSTIFF);

		while(pBx)
		{
			if(pBx->GetWebHStiff())
			{
				CWebHStiff *pHs  = pBx->GetWebHStiff();
				pHs->Clear();
			}
			pBx = Finder.GetBxNext();
		}
	}	
}

void CDetWebHStiffPosDlg::OnButtonRecommend() 
{
	SetDataDefault();
	//SetDataSave();
	//SetDataBx();
	SetDataInit();

	m_Grid.SetRedraw(TRUE,TRUE);
	m_Grid.SetFocus();
}

BOOL CDetWebHStiffPosDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_WHSTIFF_AUTOSYMMETRY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_WEBHSTIFF_ALL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_WHSTIFF_MLEFT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_WHSTIFF_MRIGHT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HSTIFF_GIRDERLEN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HSTIFF_BRIDGELEN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_HSTIFF_DISTANCE_CENTER, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_HSTIFF_JACKUPVSTIFF, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_GROUPBOX2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_WEBHSTIFF_RADIO1, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_WEBHSTIFF_RADIO2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_HSTIFF_DIM10, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_GROUPBOX4, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_HSTIFF_HEIGHT1, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_3, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_HSTIFF_HEIGHT2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_4, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_HSTIFF_HEIGHT3, SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	SetResize(IDC_STATIC_5, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_HSTIFF_TRIBAPPLY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_6, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_HSTIFF_MINJACKUP, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_WHSTIFF_MINLENCALC, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_WHSTIFF_MINLEN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_GROUPBOX3, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_WHSTIFF_5MM, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_WHSTIFF_10MM, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_HSTIFF_HEIGHTOFFSET, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_GROUPBOX1, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;
}
