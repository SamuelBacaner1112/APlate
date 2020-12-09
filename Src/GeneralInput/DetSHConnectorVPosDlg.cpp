// DetSHConnectorVPosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetSHConnectorVPosDlg.h"

#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetSHConnectorVPosDlg dialog


CDetSHConnectorVPosDlg::CDetSHConnectorVPosDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetSHConnectorVPosDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetSHConnectorVPosDlg)
	m_nLengthType = 0;
	m_bApplyCSplice = FALSE;
	m_StudMinDis = 125;
	m_StudMidDis = 250;
	m_StudMaxDis = 500;
	m_bApplyFact = FALSE;
	m_bSameApplySameCol = TRUE;
	m_bApplyHBeamVStiff = FALSE;
	m_nStudType = -1;
	//}}AFX_DATA_INIT
	
	m_pGrid = &m_vGrid;
}


void CDetSHConnectorVPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetSHConnectorVPosDlg)
	DDX_GridControl(pDX, IDC_GRID_DN, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_UP, m_vGrid);
	DDX_Radio(pDX, IDC_STUD_GIRDERLEN, m_nLengthType);
	DDX_Text(pDX, IDC_STUD_MINDIS, m_StudMinDis);
	DDX_Text(pDX, IDC_STUD_MIDDIS, m_StudMidDis);
	DDX_Text(pDX, IDC_STUD_BASEDIS, m_StudMaxDis);
	DDX_Check(pDX, IDC_STUD_FACTAPPLY, m_bApplyFact);
	DDX_Check(pDX, IDC_SCONNECT_ALLSAMEAPPLY, m_bSameApplySameCol);
	DDX_Control(pDX, IDC_STUD_MINDIS, m_ctrlEdit1);
	DDX_Control(pDX, IDC_STUD_MIDDIS, m_ctrlEdit2);
	DDX_Control(pDX, IDC_STUD_BASEDIS, m_ctrlEdit3);
	DDX_Check(pDX, IDC_STUD_HBEAM_VSTIFF, m_bApplyHBeamVStiff);
	DDX_Check(pDX, IDC_CHECK_SPLICECEN, m_bApplyCSplice);	
	DDX_Radio(pDX, IDC_RADIO_ANCHOR, m_nStudType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, ID_BUTTON_RECOMMEND_UP, m_btnAdvice);
	DDX_Control(pDX, IDC_SCONNECT_PREVMOVE, m_btnMoveLeft);
	DDX_Control(pDX, IDC_SCONNECT_NEXTMOVE, m_btnMoveRight);
	DDX_Control(pDX, IDC_SCONNECT_AUTO_SYMMETRY, m_btnAutoSymmetry);
	DDX_Control(pDX, IDC_SHARE_ALLAPPLY, m_btnSameAll);
}

BEGIN_MESSAGE_MAP(CDetSHConnectorVPosDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetSHConnectorVPosDlg)
	ON_BN_CLICKED(IDC_SCONNECT_PREVMOVE, OnSconnectPrevmove)
	ON_BN_CLICKED(IDC_SCONNECT_NEXTMOVE, OnSconnectNextmove)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND_UP, OnButtonRecommendUp)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_SCONNECT_ALLSAMEAPPLY, OnSconnectAllsameapply)
	ON_BN_CLICKED(IDC_SCONNECT_AUTO_SYMMETRY, OnSconnectAutoSymmetry)
	ON_BN_CLICKED(IDC_STUD_FACTAPPLY, OnStudFactapply)
	ON_BN_CLICKED(IDC_SHARE_ALLAPPLY, OnSameApplyAllGirder)
	ON_BN_CLICKED(IDC_STUD_BRIDGELEN, OnStudBridgelen)
	ON_BN_CLICKED(IDC_STUD_GIRDERLEN, OnStudGirderlen)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_STUD_HBEAM_VSTIFF, OnStudHbeamVstiff)
	ON_BN_CLICKED(IDC_RADIO_STUD, OnRadioStud)
	ON_BN_CLICKED(IDC_RADIO_ANCHOR, OnRadioAnchor)
	ON_BN_CLICKED(IDC_RADIO_SQUARE, OnRadioSquare)
	ON_BN_CLICKED(IDC_CHECK_SPLICECEN, OnCheckSplicecen)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_UP, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_DN, OnCellChangedDataLower)

	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_UP, OnCellChangedUpper)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID_UP, OnColInsertedUpper)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID_UP, OnColDeletedUpper)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID_UP, OnColDeleteUpper)

	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_DN, OnCellChangedLower)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID_DN, OnColInsertedLower)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID_DN, OnColDeletedLower)	
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID_DN, OnColDeleteLower)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetSHConnectorVPosDlg message handlers

void CDetSHConnectorVPosDlg::OnPreInitDialog()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();

	if(pDB->IsTUGir())	GetDlgItem(IDC_RADIO_SQUARE)->ShowWindow(SW_SHOW);
	else				GetDlgItem(IDC_RADIO_SQUARE)->ShowWindow(SW_HIDE);
}

void CDetSHConnectorVPosDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pDB	 = m_pStd->GetDataManage()->GetBridge();
	CAPlateDrawStandard DrawStd(m_pStd->GetDataManage());	
	CAPlateDrawJong		DrawJong(m_pStd->GetDataManage());

	pDom->ClearEtt(TRUE);
	CDomyun	Dom(pDom);
	Dom.SetScaleDim(1);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		long nRow = GetRow(nG);

		CPlateGirderApp	*pGir	= pDB->GetGirder(nG);	
		CPlateBxFinder Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

		pGir->SetEleLevel(TRUE);

		DrawJong.m_dArrStud.RemoveAll();
		for(long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			double dDis = m_Grid.GetTextMatrixDouble(nRow,nCol);
			DrawJong.m_dArrStud.Add(dDis);
		}

		long nDir   = 0;
		long nLine  = 2;

		DrawJong.DrawGirderLineBx (&Dom, pBxStt, pBxEnd, nLine);
		DrawJong.DrawInOutGateBx  (&Dom, pBxStt, pBxEnd, TRUE);
		DrawJong.DrawVStiffBx     (&Dom, pBxStt, pBxEnd, 2, -1, TRUE);
		DrawJong.DrawJackUpBx     (&Dom, pBxStt, pBxEnd, 2, -1, TRUE);
		DrawJong.DrawJiJumVStiffBx(&Dom, pBxStt, pBxEnd, 2, -1, TRUE);
		DrawJong.DrawFieldSpliceHatchJong(&Dom,pBxStt,pBxEnd,nDir,FALSE,FALSE);	
		DrawJong.DrawStudBx(&Dom, pBxStt, pBxEnd, 0);
		DrawJong.DimJongStud(&Dom, pBxStt, pBxEnd);

		pGir->SetEleLevel(FALSE);

		// 도면 회전각도
		CDPoint MoveXY;
		MoveXY.x = -pBxStt->GetStation();
		MoveXY.y = -pGir->GetElevationFlangeCamber(pBxStt->GetStation());
		Dom.Move(MoveXY);

		Dom.SetCalcExtRect();
		CDRect rectDom = Dom.GetExtRect();
		double dTextHeight = (pBxEnd->GetStation()-pBxStt->GetStation()) * 0.01;
		Dom.SetTextColor(3);
		Dom.SetTextHeight(dTextHeight);
		CString szText = _T("");
		szText.Format("< Girder %d >", nG+1);
		Dom.TextOut(rectDom.left + Dom.GetTextWidth(szText)/2, rectDom.bottom + Dom.GetTextHeight()/2, szText);

		Dom.SetCalcExtRect();
		rectDom = Dom.GetExtRect();

		Dom.Move(0, -(rectDom.Height()+Dom.GetTextHeight())*nG);
		*pDom << Dom;
	}

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetSHConnectorVPosDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB		= m_pStd->GetBridge();
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	UpdateData();
	SetDataDefaultBase();
	SetDataDefaultDisSet();	
	SetDataInit();
	SetGridTitle();

	for(long nG = -1; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		if(pDB->IsTUGir())	pGir->m_pSangse->m_StudHori_Type = 2;
		else				pGir->m_pSangse->m_StudHori_Type = 1;
	}
	pDeckData->m_bInstallShearRebar = FALSE;

	DrawInputDomyunView();
}

void CDetSHConnectorVPosDlg::SetDataInit()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(-1);

	m_bSameApplySameCol	= (BOOL)pDB->m_cSameApplySameColStud;
	m_StudMinDis		= pGir->m_pSangse->m_StudJong_MinDis;		// 최소 간격
	m_StudMidDis		= pGir->m_pSangse->m_StudJong_MidDis;		// 최소 간격 길이
	m_StudMaxDis		= pGir->m_pSangse->m_StudJong_MaxDis;		// 기본 간격

	m_bApplyFact		= pGir->m_pSangse->m_StudJong_FactJoin;		// 공장이음 적용 여부
	m_bApplyCSplice		= pGir->m_pSangse->m_StudJong_SpliceCenter;	// 현장 이음 중앙 적용 여부
	m_bApplyHBeamVStiff	= pGir->m_pSangse->m_StudJong_HBeamVStiff;//가로보와 연결된 수지보강재위 전달연결재 배치 여부..
	m_nLengthType		= pDB->m_nLengthTypeOnStud;

	m_nStudType = pGir->m_pSangse->m_StudHori_Type;
	
	SetDataInitUp();
	SetDataInitLow();
	SetGridTitle();
	SetText();

	UpdateData(FALSE);	
}

void CDetSHConnectorVPosDlg::SetDataSave()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	UpdateData(TRUE);
	
	for(long nG=-1; nG<pDB->GetGirdersu(); nG++) 
	{	
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		pGir->m_pSangse->m_StudJong_MinDis = m_StudMinDis;
		pGir->m_pSangse->m_StudJong_MidDis = m_StudMidDis;
		pGir->m_pSangse->m_StudJong_MaxDis = m_StudMaxDis;

		pGir->m_pSangse->m_StudJong_FactJoin	= m_bApplyFact;		// 공장이음 적용 여부
		pGir->m_pSangse->m_StudJong_SpliceCenter= m_bApplyCSplice;	// 현장 이음 중앙 적용 여부
		pGir->m_pSangse->m_StudJong_HBeamVStiff = m_bApplyHBeamVStiff; //가로보와 연결된 수지보강재위 전달연결재 배치 여부..
		pGir->m_pSangse->m_StudHori_Type		= m_nStudType;

		
	}
	pDB->m_nLengthTypeOnStud = m_nLengthType;
	DataSaveGirder();
}

//////////////////////////////////////////////////////////////////////////
void CDetSHConnectorVPosDlg::SetDataDefaultBase()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	double dBound1 = 5000, dBound2 = 10000;

	UpdateData(TRUE);

	for(long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir	= pDB->GetGirder(nG);
		CPlateSangSeDo	*pSang	= pGir->m_pSangse;

		CLineInfo  *pLine = pGir->GetLine();
		double dPreSta = pGir->GetSSS(OFF_STT_GIRDER);
		double dCurSta, dTerm, dBaseTerm;

		for(long n=0; n<STUD_JONG_SU; n++)
		{
			pSang->m_StudJong_BaseTerm[n] = 0;
			pSang->m_StudJong_BaseDis[n]  = 0;
		}

		long nIdx = 0;

		dTerm = pGir->GetSSS(OFF_STT_SHOE) - pGir->GetSSS(OFF_STT_GIRDER);
		pSang->m_StudJong_BaseTerm[nIdx]	= dTerm;
		pSang->m_StudJong_BaseDis[nIdx++]	= pLine->GetStationByDisLength(dPreSta, dTerm, 0) - dPreSta;
		
		dPreSta = pGir->GetSSS(OFF_STT_SHOE);		
		long nK = 0;
		for (nK = 0; nK < pDB->m_nQtyJigan; nK++)
		{
			double dSttJijum = pGir->GetStationOnJijum(nK);
			double dEndJijum = pGir->GetStationOnJijum(nK+1);

			for (long nTerm = 0; nTerm < 4; nTerm++)
			{
				switch(nTerm)   {
				case 0 :
					dCurSta = pLine->GetStationByDisLength(dSttJijum, dBound1, 0);
					dBaseTerm = pLine->GetStationByDisLength(dPreSta, nK ? m_StudMaxDis : m_StudMinDis, 0) - dPreSta;
					break;
				case 1 :
					dCurSta = pLine->GetStationByDisLength(dSttJijum, dBound2, 0);
					dBaseTerm = pLine->GetStationByDisLength(dPreSta,  nK == 0  ? m_StudMidDis : m_StudMaxDis, 0) - dPreSta;
					break;
				case 2 :
					dCurSta = pLine->GetStationByDisLength(dEndJijum, -dBound2, 0);
					dBaseTerm = pLine->GetStationByDisLength(dPreSta, m_StudMaxDis, 0) - dPreSta;
					break;
				case 3 :
					dCurSta = pLine->GetStationByDisLength(dEndJijum, -dBound1, 0);
					dBaseTerm = pLine->GetStationByDisLength(dPreSta, (nK == pDB->m_nQtyJigan-1) ? m_StudMidDis : m_StudMaxDis, 0) - dPreSta;
					break;
				}
				dCurSta	= pGir->GetBxByStationDir(dCurSta, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF)->GetStation();
				dTerm	= dCurSta - dPreSta;
				dPreSta	= dCurSta;
				
				if(dTerm > 0 && dBaseTerm > 0)
				{
					pGir->m_pSangse->m_StudJong_BaseTerm[nIdx]	= dTerm;
					pGir->m_pSangse->m_StudJong_BaseDis[nIdx++]	= dBaseTerm;
				}
			}
		}

		dCurSta = pGir->GetBxByStationDir(pGir->GetStationOnJijum(nK), 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF)->GetStation();
		dBaseTerm = pLine->GetStationByDisLength(dPreSta, m_StudMinDis, 0) - dPreSta;		
		dTerm = dCurSta - dPreSta;
		dPreSta	= dCurSta;
		if(dTerm > 0 && dBaseTerm > 0)
		{
			pGir->m_pSangse->m_StudJong_BaseTerm[nIdx]	= dTerm;
			pGir->m_pSangse->m_StudJong_BaseDis[nIdx++]	= dBaseTerm;
		}

		dTerm = pGir->GetSSS(OFF_END_GIRDER) - dPreSta;
		pGir->m_pSangse->m_StudJong_BaseTerm[nIdx]	= dTerm;
		pGir->m_pSangse->m_StudJong_BaseDis[nIdx++]	= pLine->GetStationByDisLength(dPreSta, dTerm, 0) - dPreSta;

		for(nIdx; nIdx<STUD_JONG_SU; nIdx++)
		{
			pGir->m_pSangse->m_StudJong_BaseTerm[nIdx] = 0;
			pGir->m_pSangse->m_StudJong_BaseDis[nIdx]  = 0;
		}
	}
}

void CDetSHConnectorVPosDlg::SetDataDefaultDisSet()
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(-1);
	
	CDPointArray LenArr, GridArr;
	
	// 현장 이음 중앙 적용 여부
//	m_bApplyCSplice	= pGir->m_pSangse->m_StudJong_SpliceCenter = FALSE;

	for(long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		LenArr.RemoveAll();

		CPlateGirderApp *pGir	= pDB->GetGirder(nG);
		CPlateSangSeDo	*pSang	= pGir->m_pSangse;

		CPlateBxFinder Finder(pGir);
		__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE;
		if (m_bApplyFact)								BxFlag |= BX_UPPERFLANGE;
		if(m_bApplyHBeamVStiff && !pDB->IsPlateGir())	BxFlag |= BX_CROSSBEAM_VBRACING;

		CPlateBasicIndex *pBxPrev = Finder.GetBxFirst(BxFlag);
		CPlateBasicIndex *pBxNext = Finder.GetBxNext();
		
		while(pBxNext)
		{
			CSection *pSec = pBxNext->GetSection();
			if(pSec && !pDB->IsPlateGir())
			{
				if(pSec->IsenDetType(SECDET_CROSSBEAM) ||pSec->IsenDetType(SECDET_VSITFF) )
				{
					if (ProcessStudTerm(pBxPrev, pBxNext, LenArr))
						pBxPrev = pBxNext;
				}
			}
			else if(pBxNext->IsState(BX_SPLICE) && !pBxNext->GetSplice()->m_bSpliceType)
			{
				if (ProcessStudTerm(pBxPrev, pBxNext, LenArr))
					pBxPrev = pBxNext;
			}
			else
			{
				if (ProcessStudTerm(pBxPrev, pBxNext, LenArr))
					pBxPrev = pBxNext;
			}
			pBxNext = Finder.GetBxNext();
		}
		
		long nIdx = 0;
		for(nIdx=0; nIdx<LenArr.GetSize(); nIdx++)
		{
			pGir->m_pSangse->m_StudJong_StudDis[nIdx] = Round(LenArr.GetAt(nIdx).x,0);
			pGir->m_pSangse->m_StudJong_Studsu[nIdx]  = (long)LenArr.GetAt(nIdx).y;
		}
		for(nIdx; nIdx<STUD_JONG_SU; nIdx++)
		{
			pGir->m_pSangse->m_StudJong_StudDis[nIdx] = 0;
			pGir->m_pSangse->m_StudJong_Studsu[nIdx]  = 0;
		}
	}
}

//전단 연결 데이터 읽기
void CDetSHConnectorVPosDlg::SetDataInitUp()
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir	= NULL;
	long nRow = 0;
	double sum = 0, term = 0, dis = 0;

	m_vGrid.SetRowCount(pDB->GetGirdersu()*2 + 1);
	m_vGrid.SetColumnCount(GetMaxDataV(TRUE)+1);
	for(long nG = 0; nG < pDB->GetGirdersu() ; nG++)
	{
		pGir = pDB->GetGirder(nG);
		nRow = GetRow(nG);
		sum = 0;
		for(long nCol = 0 ; nCol < GetMaxDataV(TRUE); nCol++)
		{
			term = pGir->m_pSangse->m_StudJong_BaseTerm[nCol];
			dis  = pGir->m_pSangse->m_StudJong_BaseDis[nCol];

			if(term==0 && dis==0)
				term = pGir->GetSSS(OFF_END_GIRDER) - pGir->GetSSS(OFF_STT_GIRDER) - sum;

			m_vGrid.SetTextMatrix(nRow, nCol+1, Comma(term));
			m_vGrid.SetTextMatrix(nRow+1, nCol+1, Comma(dis));
			sum += term;
		}
	}
	
	SetGridDataByChangeLengthType(&m_vGrid, 1);
	m_vGrid.SetRedraw(TRUE,TRUE);
	UpdateData(FALSE);
}

void CDetSHConnectorVPosDlg::SetDataInitLow()
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir	= NULL;
	long nRow = 0;
	double sum = 0, term = 0, dis = 0, su = 0;

	m_Grid.SetRowCount(pDB->GetGirdersu()*2 + 1);
	m_Grid.SetColumnCount(GetMaxDataV(FALSE)+1);
	for(long nG = 0; nG < pDB->GetGirdersu() ; nG++)
	{
		pGir = pDB->GetGirder(nG);
		nRow = GetRow(nG);
		sum = 0;
		for(long nCol = 0 ; nCol < GetMaxDataV(FALSE); nCol++)
		{
			dis = pGir->m_pSangse->m_StudJong_StudDis[nCol];
			su  = pGir->m_pSangse->m_StudJong_Studsu[nCol];

			if(dis==0 && su==0)
				term = pGir->GetSSS(OFF_END_GIRDER) - pGir->GetSSS(OFF_STT_GIRDER) - sum;

			m_Grid.SetTextMatrix(nRow, nCol+1, Comma(dis));
			m_Grid.SetTextMatrix(nRow+1, nCol+1, Comma(su));
			sum += term;
		}
	}
	
	SetGridDataByChangeLengthType(&m_Grid, 1);
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetSHConnectorVPosDlg::SetGridTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CString	str;

	//초기화
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	
	m_Grid.SetColumnWidthAll(40);
	m_Grid.SetColumnWidth(0,80);
	m_Grid.SetTextMatrix(0, 0, _T("(mm)"));

	m_vGrid.SetEditable(TRUE);
	m_vGrid.SetListMode(FALSE);
	m_vGrid.EnableDragAndDrop(TRUE);
	m_vGrid.EnableInsertRow(FALSE);
	m_vGrid.EnableInsertCol(TRUE);
	m_vGrid.EnableTitleTips(FALSE);
	m_vGrid.SetTextBkColor(RGB(225, 250, 250));	
	m_vGrid.SetFixedRowCount(1);
	m_vGrid.SetFixedColumnCount(1);
	
	m_vGrid.SetColumnWidthAll(50);
	m_vGrid.SetColumnWidth(0,80);
	m_vGrid.SetTextMatrix(0, 0, _T("(mm)"));

	for(long nG = 0 ; nG < pDB->GetGirdersu() ; nG++)
	{
		long nRow = GetRow(nG);

		str.Format("길이(G%d)", nG+1+pDB->GetNumberStartGirder());
		m_vGrid.SetTextMatrix(nRow,	0, str);
		str.Format("간격(G%d)", nG+1+pDB->GetNumberStartGirder());
		m_vGrid.SetTextMatrix(nRow+1,0, str);

		str.Format("간격   (G%d)", nG+1+pDB->GetNumberStartGirder());
		m_Grid.SetTextMatrix(nRow,	0, str);
		str.Format("간격수(G%d)", nG+1+pDB->GetNumberStartGirder());
		m_Grid.SetTextMatrix(nRow+1,0, str);
	}
	long nCol = 0;
	for(nCol=1; nCol<m_vGrid.GetColumnCount(); nCol++)
		m_vGrid.SetTextMatrix(0, nCol, COMMA(nCol));
	for(nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
		m_Grid.SetTextMatrix(0, nCol, COMMA(nCol));

	m_Grid.SetRowHeightAll(20);					//열의 모든 높이는 20
	m_Grid.SetRowHeight(0, 22);					//0~20

	m_vGrid.SetRowHeightAll(20);					//열의 모든 높이는 20
	m_vGrid.SetRowHeight(0, 22);					//0~20

	m_Grid.SetRedraw(TRUE,TRUE);
	m_vGrid.SetRedraw(TRUE,TRUE);
}


void CDetSHConnectorVPosDlg::DataSaveGirder()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	BOOL bTranse = FALSE;
	if(m_nLengthType==0)
	{
		bTranse = TRUE;
		m_nLengthType=1;
		SetGridDataByChangeLengthType(&m_vGrid, 0);
		SetGridDataByChangeLengthType(&m_Grid,  0);		
	}

	for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp *pGir	= pDB->GetGirder(nG);
		CPlateSangSeDo	*pSang	= pGir->m_pSangse;

		long nRow = GetRow(nG);
		long nCol = 0;
		for(nCol=1; nCol<m_vGrid.GetColumnCount(); nCol++)
		{
			pSang->m_StudJong_BaseTerm[nCol-1]	= m_vGrid.GetTextMatrixDouble(nRow, nCol);
			pSang->m_StudJong_BaseDis[nCol-1]	= m_vGrid.GetTextMatrixDouble(nRow+1, nCol);
		}
		for(nCol; nCol<STUD_JONG_SU; nCol++)
		{
			pSang->m_StudJong_BaseTerm[nCol-1]	= 0;
			pSang->m_StudJong_BaseDis[nCol-1]	= 0;
		}
		
		for(nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			pSang->m_StudJong_StudDis[nCol-1]	= m_Grid.GetTextMatrixDouble(nRow, nCol);
			pSang->m_StudJong_Studsu[nCol-1]	= m_Grid.GetTextMatrixLong(nRow+1, nCol);
		}
		for(nCol; nCol<STUD_JONG_SU; nCol++)
		{
			pSang->m_StudJong_StudDis[nCol-1]	= 0;
			pSang->m_StudJong_Studsu[nCol-1]	= 0;
		}
	}

	if(bTranse)
	{
		m_nLengthType=0;
		SetGridDataByChangeLengthType(&m_vGrid, 1);
		SetGridDataByChangeLengthType(&m_Grid,  1);
	}
}

void CDetSHConnectorVPosDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,nCol));
}

long CDetSHConnectorVPosDlg::GetMaxDataV(BOOL bBase)
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();	

	long nMaxIdx = 0;
	for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp *pGir	= pDB->GetGirder(nG);
		CPlateSangSeDo	*pSang	= pGir->m_pSangse;
		long nIdx = 0;
		for(nIdx=0; nIdx<STUD_JONG_SU; nIdx++)
		{
			if(bBase && pSang->m_StudJong_BaseDis[nIdx] < 1)	break;
			if(!bBase && pSang->m_StudJong_StudDis[nIdx] < 1)	break;			
		}
		nMaxIdx = max(nMaxIdx, nIdx);
	}
	
	return nMaxIdx;
}

// nOldType : 0 - 거더중앙 실제길이, 1: 선형중심 거리
void CDetSHConnectorVPosDlg::SetGridDataByChangeLengthType(CGridCtrl *pGrid, int nOldType)
{
	if(nOldType == m_nLengthType)	return;
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	BOOL bUpperGrid = (pGrid == &m_vGrid);

	long nRows = pGrid->GetRows();
	long nCols = pGrid->GetCols();

	CDPointArray vGridData;

	for (long nRow = 1; nRow < nRows; nRow += 2) 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nRow/2);
		double dStaStt	= pGir->GetSSS(OFF_STT_GIRDER);
		double dStaPre	= dStaStt, dSta;
		double dLen		= 0;

		vGridData.RemoveAll();
		long nCol = 0;
		for(nCol=1; nCol<nCols; nCol++)
			vGridData.Add(CDPoint(pGrid->GetTextMatrixDouble(nRow, nCol), pGrid->GetTextMatrixDouble(nRow+1, nCol)));
		
		for (nCol = 1; nCol < nCols; nCol++)
		{
			if (pGrid->GetTextMatrix(nRow, nCol) == "")	break;

			// dLen = 거더 시점부터의 거리
			dLen += vGridData.GetAt(nCol-1).x*(bUpperGrid ? 1 : vGridData.GetAt(nCol-1).y);
			
			if (nOldType == 0)	dSta = pGir->GetStationByActLength(dStaStt, dLen);
			else				dSta = dStaStt+pGir->GetLine()->GetLengthByDis(dStaStt, dStaStt+dLen, pGir->m_dGirderCenter);

			if (bUpperGrid)
			{
				double dStaTemp;
				double length = vGridData.GetAt(nCol-1).y;

				if (nOldType == 0)	dStaTemp = pGir->GetStationByActLength(dStaPre, length);
				else				dStaTemp = dStaPre+pGir->GetLine()->GetLengthByDis(dStaPre, dStaPre+length, pGir->m_dGirderCenter);
				pGrid->SetTextMatrix(nRow, nCol, Comma(dSta-dStaPre));				
				pGrid->SetTextMatrix(nRow + 1, nCol, Comma(dStaTemp-dStaPre));
			}
			else
			{
				pGrid->SetTextMatrix(nRow, nCol, Comma((dSta-dStaPre)/vGridData.GetAt(nCol-1).y));
			}
			dStaPre = dSta;
		}
	}

	pGrid->Invalidate();
}

// pBx는 현장이음 아니면 공장이음임.
// nDir : pBx기준 앞뒤 지정.
// dStaBase : 스터드를 배치하는 기준 스테이션.
double CDetSHConnectorVPosDlg::GetStationStudTerm(CPlateBasicIndex *pBx, long nDir, double &dStaBase)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBx->GetGirder();
	double dCurSta = pBx->GetStation();
	double dWidth = 0, dStaReal;

	if (pBx->IsState(BX_SPLICE))
	{
		CPlateSplice *pSp = pBx->GetSplice();
		if(!pSp->m_bSpliceType)	dWidth = 50;
		else					dWidth = pSp->m_uA + pSp->m_uB*pSp->m_uC + pSp->m_uC1/2 + 50;
	}
	else if (pBx->IsState(BX_UPPERFLANGE))
	{
		long nHBase = m_pStd->m_pDataManage->GetGlobalOption()->GetDesignHeigtBase();

		double dPreThick	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
		double dThick		= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
		double dTaper_W	= pDB->GetGirder(-1)->m_pSangse->m_Taper_W;

	// 두께가 다를경우 테이퍼 끝단에서 20 간격이고 20 포함 간격이 50 간격보다 작으면 50 적용..
	// 거더 높이 기준이 0, 2  인 경우만 해당됨 아닌 경우 무조건 50 간격..		
		
		if (nHBase == 0 || nHBase == 2)
		{
			dWidth = max(fabs(dPreThick - dThick) * dTaper_W + 20, 50);
			dWidth = ((nDir > 0) ^ (dPreThick < dThick)) ? 50 : dWidth;
		}
		else
		{
			dWidth = 50;
		}				
	}
	else if (m_bApplyHBeamVStiff && pBx->IsState(BX_CROSSBEAM_VBRACING))
	{
		dWidth = 50;
	}
	dWidth *= nDir;
	dStaReal = pGir->GetStationByActLength(dCurSta, dWidth);
	dStaBase = (1) ? (dCurSta + dWidth) : dStaReal;
	return dStaReal;
}

// 두 스테이션을 경계로 하는 구간에 전단연결재 간격 나누고 간격과 개수 형태로 리턴.
void CDetSHConnectorVPosDlg::GetStudTermArr(long nG, double dSttSta, double dEndSta, CDPointArray &LenArr)
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(nG);
	CPlateSangSeDo	*pSang	= pGir->m_pSangse;

	CDPointArray BaseArr;	
	CDPoint Len;
	double dPreSta = pGir->GetSSS(OFF_STT_GIRDER);
	double dCurSta = dPreSta;

	for(long n=0; n<STUD_JONG_SU; n++)
	{
		double dTerm	= pSang->m_StudJong_BaseTerm[n];
		double dDis		= pSang->m_StudJong_BaseDis[n];

		dCurSta += dTerm;
		if (dSttSta <= dCurSta)
		{
			dCurSta = min(dCurSta, dEndSta);
			Len.x = dCurSta - dSttSta;
			Len.y = dDis;
			BaseArr.Add(Len);
			if (dEndSta == dCurSta)
				break;
		}
	}

	CDDWordArray TermArr;
	double dHalfSta = (pGir->GetSSS(OFF_END_GIRDER) + pGir->GetSSS(OFF_STT_GIRDER)) / 2;
	if (dEndSta <= dHalfSta)		GetTermDivideFoward(BaseArr, TermArr);
	else if (dSttSta >= dHalfSta)	GetTermDivideBackward(BaseArr, TermArr);
	else							GetTermDivideBoth(BaseArr, TermArr);

	double dPreTerm = 0;
	if(TermArr.GetSize()>0)	
		dPreTerm = TermArr.GetAt(0);
	long nCount = 1;
	long nSize = TermArr.GetSize();
	for (long nIdx = 1; nIdx <= nSize; nIdx++)
	{
		double dTerm = 0;
		if (nIdx < nSize)
			dTerm = TermArr.GetAt(nIdx);
		if (dTerm == dPreTerm)
		{
			nCount++;
			continue;
		}
		Len.x = dPreTerm;
		Len.y = nCount;
		LenArr.Add(Len);
		dPreTerm = dTerm;
		nCount = 1;
	}
}

BOOL CDetSHConnectorVPosDlg::ProcessStudTerm(CPlateBasicIndex *pBxPrev, CPlateBasicIndex *pBxNext, CDPointArray &LenArr)
{
	double dSttReal = 0, dEndReal = 0, dNextReal = 0, dSttBase = 0, dEndBase = 0, dNextBase = 0;
	dSttReal = GetStationStudTerm(pBxPrev,  1, dSttBase);
	dEndReal = GetStationStudTerm(pBxNext, -1, dEndBase);
	dNextReal = GetStationStudTerm(pBxNext, 1, dNextBase);
	if (dEndReal == 0)
		return FALSE;

	CDPoint Len;
	if (dEndReal > dSttReal)
	{
		long nDiffIdx = LenArr.GetSize();
		GetStudTermArr(pBxPrev->GetNumberGirder(), dSttBase, dEndBase, LenArr);
		if(fabs(dSttReal - dSttBase) > 0.001)
		{  // 앞쪽 구간 보정
			Len = LenArr.GetAt(nDiffIdx);
			LenArr.RemoveAt(nDiffIdx);
			if(Len.y > 1)
			{
				Len.y -= 1;
				LenArr.InsertAt(nDiffIdx, Len);
			}
			Len.x -= dSttReal - dSttBase;
			Len.y = 1;
			LenArr.InsertAt(nDiffIdx, Len);
		}

		nDiffIdx = LenArr.GetUpperBound();
		if(fabs(dEndReal - dEndBase) > 0.001)
		{  // 뒤쪽 구간 보정
			Len = LenArr.GetAt(nDiffIdx);
			LenArr.RemoveAt(nDiffIdx);
			if (Len.y > 1)
			{
				Len.y -= 1;
				LenArr.InsertAt(nDiffIdx, Len);
			}
			Len.x += dEndReal - dEndBase;
			Len.y = 1;
			LenArr.Add(Len);
		}

		if(pBxNext->IsState(BX_SPLICE) && m_bApplyCSplice) 
		{
			Len.x = (dNextReal - dEndReal) / 2;
			Len.y = 2;
			if(!pBxNext->GetSplice()->m_bSpliceType)
			{
				Len.x = dNextReal - dEndReal;
				Len.y = 1;
			}
		}
		else
		{
			if(m_bApplyHBeamVStiff)
			{
				if(pBxNext->IsState(BX_CROSSBEAM_VBRACING))		
				{
					Len.x = (dNextReal - dEndReal) / 2;
					Len.y = 2;
				}
				else
				{
					Len.x = dNextReal - dEndReal;
					Len.y = 1;
				}
			}
			else
			{
				Len.x = dNextReal - dEndReal;
				Len.y = 1;
			}
		}

		if(Len.x > 0)
			LenArr.Add(Len);
	}
	else 
	{
		// 현장이음과 공장이음이 겹치는 경우임.
	}

	return TRUE;
}

double CDetSHConnectorVPosDlg::GetStationMatrix(CGridCtrl *pGrid, long nRow, long nCol)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	double lenSum = 0;
	for(long c = 1; c <= nCol; c++)
		lenSum += pGrid->GetTextMatrixDouble(nRow, c);
	double sta = pDB->GetStationByLengthInGrid(FALSE, nRow/2, lenSum, m_nLengthType, 0);
	return sta;
}


// 상부 그리드에서 간격항목에만 해당됨. (현재 간격을 스테이션 기준 간격으로 구해줌.)
double CDetSHConnectorVPosDlg::GetLengthMatrix(long nRow, long nCol, double sta)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CPlateGirderApp *pGir = pDB->GetGirder((nRow - 1) / 2);
	if (sta < 0)
		sta = GetStationMatrix(&m_vGrid, nRow - 1, nCol - 1);
	double length = m_vGrid.GetTextMatrixDouble(nRow, nCol);
	double stalen;
	if (m_nLengthType == 0)
		stalen = pGir->GetStationByActLength(sta, length);
	else
		stalen = pGir->GetLine()->GetStationByDisLength(sta, length, 0);

	return stalen - sta;
}

// 임의의 길이를 원하는 간격으로 나눔. (간격이 바뀌는 곳에 반드시 배치하지는 않음. 융통성! 크크...)
//  < 함수 종류 >
// GetTermDivideFoward   : 앞에서부터 배치
// GetTermDivideBackward : 뒤부터 배치
// GetTermDivideBoth     : 앞뒤에서 배치
//  < 입력 >
// LenArr.x : 구간 길이 (선두를 0으로한 누적길이!)
// LenArr.y : 그 구간의 배치 간격
//  < 리턴 >
// TermArr : 실제 간격
//
//  < 짜투리 계산 원칙 >   이건 거의 인공지능이군... 나도 모르겠다.
//  * Foward or Backward인 경우
// 1. 짜투리가 기본간격의 절반보다 크면 그냥 사용.
// 2. 짜투리가 기본간격의 절반보다 작으면 앞 간격과 합쳐 2등분함.
// 3. 짜투리가 지정된 최소길이보다 작은 경우는 다음과 같이 처리
//    -> 1번 경우의 짜투리가 작은 경우 2번으로 처리
//    -> 2번 경우의 짜투리가 작은 경우는 앞 간격과 합친다.
//  * 양방향인 경우
// 1. 짜투리를 가운데로 배치하여 양 짜투리를 합침.
// 2. 합친 길이가 양 기본간격 평균 보다 크면 2등분함
// 3. 짜투리가 지정된 최소길이보다 작은 경우는 양쪽 간격 합치고 2등분함

void CDetSHConnectorVPosDlg::GetTermDivideFoward(CDPointArray &LenArr, CDDWordArray &TermArr)
{
	double dMinTerm = 100;  // 최소길이

	long   nTermIdx = 0;
	double dCurLen = 0, dBoundLen, dBaseLen;

	while (LenArr.GetSize() > nTermIdx)
	{
		dBoundLen = LenArr.GetAt(nTermIdx).x;
		dBaseLen  = LenArr.GetAt(nTermIdx).y;
		while (dCurLen < dBoundLen - 1.0)   {
			TermArr.Add(dBaseLen);
			dCurLen += dBaseLen;
		}
		nTermIdx++;
	}
	if (dCurLen == dBoundLen)  // 짜투리가 있으면? 없으면?
		return;

	dBaseLen = TermArr.GetAt(TermArr.GetUpperBound());
	TermArr.RemoveAt(TermArr.GetUpperBound());  // 초과길이 제거
	double dRemain = dBoundLen - (dCurLen - dBaseLen);  // 짜투리 구함

	if (dBoundLen < dMinTerm)  // 구간 총길이가 최소길이보다 작음
		return;

	// 짜투리 1번 경우
	if (TermArr.GetSize() == 0 || ((dRemain > dBaseLen / 2) && (dRemain >= dMinTerm)))   {
		TermArr.Add(dRemain);
		return;
	}
	// 짜투리 2번 경우
	dBaseLen = TermArr.GetAt(TermArr.GetUpperBound());
	TermArr.RemoveAt(TermArr.GetUpperBound());
	dRemain = dRemain + dBaseLen;
	if (dRemain / 2 >= dMinTerm)
	{
		TermArr.Add(dRemain / 2);
		TermArr.Add(dRemain / 2);
		return;
	}
	TermArr.Add(dRemain);
}

// Backward는 Data순서만 바꿔 Foward로 계산.
void CDetSHConnectorVPosDlg::GetTermDivideBackward(CDPointArray &LenArr, CDDWordArray &TermArr)
{
	// Swap Array.
	long nSymIdx = LenArr.GetSize() - 1;
	long nIdx = 0;
	for (nIdx = 0; nIdx < nSymIdx; nIdx++)
	{
		CDPoint temp = LenArr.GetAt(nIdx);
		LenArr.SetAt(nIdx, LenArr.GetAt(nSymIdx));
		LenArr.SetAt(nSymIdx, temp);
		nSymIdx--;
	}
	// 거리 재계산
	double dTotal = LenArr.GetAt(0).x;
	for (nIdx = 0; nIdx < LenArr.GetSize(); nIdx++)
	{
		CDPoint temp = LenArr.GetAt(nIdx);
		if (nIdx < LenArr.GetSize() - 1)
			temp.x = dTotal - LenArr.GetAt(nIdx + 1).x;
		else
			temp.x = dTotal;
		LenArr.SetAt(nIdx, temp);
	}

	GetTermDivideFoward(LenArr, TermArr);

	// Swap Array.
	nSymIdx = TermArr.GetSize() - 1;
	for (nIdx = 0; nIdx < nSymIdx; nIdx++)   {
		double temp = TermArr.GetAt(nIdx);
		TermArr.SetAt(nIdx, TermArr.GetAt(nSymIdx));
		TermArr.SetAt(nSymIdx, temp);
		nSymIdx--;
	}
}

void CDetSHConnectorVPosDlg::GetTermDivideBoth(CDPointArray &LenArr, CDDWordArray &TermArr)
{
	double dMinTerm = 100;  // 최소길이

	CDDWordArray Forword, BackWord;
	GetTermDivideFoward(LenArr, Forword);
	GetTermDivideBackward(LenArr, BackWord);
	double dHalfLen = LenArr.GetAt(LenArr.GetUpperBound()).x / 2;
	double dSumLen = 0, dLen;
	long nSize = Forword.GetSize();
	long nIdx = 0;
	for (nIdx = 0; nIdx < nSize; nIdx++)
	{
		dLen = Forword.GetAt(nIdx);
		dSumLen += dLen;
		if (dSumLen > dHalfLen)
			break;
		TermArr.Add(dLen);
	}
	double dRemain1 = dHalfLen - (dSumLen - dLen);
	double dBaseLen1 = dLen;

	long nInsertIdx = nIdx;
	dSumLen = 0;
	nSize = BackWord.GetSize();
	for (nIdx = nSize - 1; nIdx >= 0; nIdx--)
	{
		dLen = BackWord.GetAt(nIdx);
		dSumLen += dLen;
		if (dSumLen > dHalfLen)
			break;
		TermArr.InsertAt(nInsertIdx, dLen);
	}
	double dRemain2 = dHalfLen - (dSumLen - dLen);
	double dBaseLen2 = dLen;
	double dRemain = dRemain1 + dRemain2;
	if (dRemain < dMinTerm)   {  // 3번 경우
		dLen = TermArr.GetAt(nInsertIdx - 1);
		TermArr.SetAt(nInsertIdx - 1, dLen + dRemain / 2);
		dLen = TermArr.GetAt(nInsertIdx);
		TermArr.SetAt(nInsertIdx, dLen + dRemain / 2);
		return;
	}
	if (dRemain > (dBaseLen1 + dBaseLen2) / 2 && dRemain / 2 >= dMinTerm)
	{  // 2번 경우
		TermArr.InsertAt(nInsertIdx, dRemain / 2);
		TermArr.InsertAt(nInsertIdx, dRemain / 2);
		return;
	}
	TermArr.InsertAt(nInsertIdx, dRemain);  // 1번 경우

}

double CDetSHConnectorVPosDlg::GetGirderLength(long nG)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	double sta = pDB->GetGirder(nG)->GetSSS(OFF_END_GIRDER);
	double len = pDB->GetLengthByStationInGrid(FALSE, nG, sta, m_nLengthType, 0);
	return len;
}

void CDetSHConnectorVPosDlg::OnCellChangedUpper(NMHDR* nmgv, LRESULT*)
{
	m_pGrid = &m_vGrid;
	SetText();
}

void CDetSHConnectorVPosDlg::OnCellChangedLower(NMHDR* nmgv, LRESULT*)
{
	m_pGrid = &m_Grid;
	SetText();
}

//데이터가 변경되었을 경우
void CDetSHConnectorVPosDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	m_pGrid = &m_vGrid;
	SetText();
}

void CDetSHConnectorVPosDlg::OnCellChangedDataLower(NMHDR* nmgv, LRESULT*)
{
	m_pGrid = &m_Grid;
	SetText();
	DrawInputDomyunView(FALSE);
}

double CDetSHConnectorVPosDlg::GetInputLength(long nG)
{
	long	nCol, cCol, nRow;
	long	su;
	double	dis, sum;
	CString	str;

	sum = 0;
	cCol = m_Grid.GetColumnCount();
	nRow = GetRow(nG);
	for(nCol = 1 ; nCol <  cCol ; nCol++) {
		str = m_Grid.GetItemText(nRow, nCol);
		if(str == _T(""))	break;
		dis = atof(str);
		su  = atoi(m_Grid.GetItemText(nRow+1, nCol));
		sum+= (dis*su);
	}
	return sum;
}

double CDetSHConnectorVPosDlg::GetInputMinus(long nG)
{
	return(GetInputLength(nG) - GetGirderLength(nG));
}

void CDetSHConnectorVPosDlg::OnSconnectPrevmove() 
{
	MoveLeftRight(-1);
	m_vGrid.SetRedraw(TRUE,TRUE);	
	m_vGrid.SetFocus();	
}

void CDetSHConnectorVPosDlg::OnSconnectNextmove() 
{
	MoveLeftRight(1);
	m_vGrid.SetRedraw(TRUE,TRUE);	
	m_vGrid.SetFocus();
}

void CDetSHConnectorVPosDlg::SetMatrixByStation(CGridCtrl *pGrid, long nRow, long nCol, double dSta)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	BOOL bUpperGrid = (pGrid == &m_vGrid);

	double dLenSum = 0;
	long c = 0;
	for(c = 1; c < nCol; c++)
	{
		if (bUpperGrid)	dLenSum += pGrid->GetTextMatrixDouble(nRow, c);
		else			dLenSum += pGrid->GetTextMatrixDouble(nRow, c) * pGrid->GetTextMatrixDouble(nRow+1, c);  // 개수 곱함.
	}
	double dLen = pDB->GetLengthByStationInGrid(FALSE, nRow/2, dSta, m_nLengthType, 0) - dLenSum;
	if (!bUpperGrid)
		dLen /= pGrid->GetTextMatrixDouble(nRow+1, c);  // 다시 개수 나눔.
	pGrid->SetTextMatrix(nRow, nCol, Comma(dLen));
}

// nDir : 항상 -1 또는 +1
void CDetSHConnectorVPosDlg::MoveLeftRight(long nDir)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	UpdateData(TRUE);
	CPlateGirderApp* pGir;
	long nG = 0;
	long nCurRow = m_vGrid.GetFocusCell().row;			
	long nCurCol = m_vGrid.GetFocusCell().col;
	if(nCurRow < 0 || nCurCol < 0) return;
	nCurRow -= (nCurRow - 1) % 2;
	double dSta, dNextSta, dMoveLen;

	if(nCurCol == m_vGrid.GetColumnCount()-1) return;

	for(long nRow = 1; nRow < m_vGrid.GetRowCount(); nRow+=2)
	{
		if (!(m_bSameApplySameCol || nRow == nCurRow))
			continue;
		nG = (nRow-1)/2;
		pGir = pDB->GetGirder(nG);
		dSta = GetStationMatrix(&m_vGrid, nRow, nCurCol);
		dNextSta = GetStationMatrix(&m_vGrid, nRow, nCurCol+1);
		double staNew = dSta;

		__int64 bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING|BX_VSTIFF;
		CPlateBxFinder Finder(pGir);		
		CPlateBasicIndex* pBx = Finder.GetBxFirst(bxFlag);
		while (pBx)
		{
			if(pBx == pGir->GetBxByStation(dSta,bxFlag))	break;
			else pBx = Finder.GetBxNext();
		}
		if(nDir>0)	pBx = Finder.GetBxFixNext();
		else		pBx = Finder.GetBxFixPrev();
		staNew = pBx ? pBx->GetStation() : dSta;
		
		dMoveLen = staNew - dSta;
		if(staNew==pGir->GetSSS(OFF_STT_GIRDER))
			dMoveLen = 0;
		SetMatrixByStation(&m_vGrid, nRow, nCurCol, dSta + dMoveLen);
		SetMatrixByStation(&m_vGrid, nRow, nCurCol+1, dNextSta);
	}
}

void CDetSHConnectorVPosDlg::OnButtonRecommendUp() 
{	
	UpdateData();
	SetDataDefaultBase();	
	SetDataInit();
	SetGridTitle();

	DrawInputDomyunView();
}

void CDetSHConnectorVPosDlg::OnButtonRecommend() 
{
	UpdateData();
	SetDataSave();
	SetDataDefaultDisSet();	
	SetDataInit();
	SetGridTitle();

	DrawInputDomyunView();
}

void CDetSHConnectorVPosDlg::OnColInsertedUpper(NMHDR* nmgv, LRESULT*)
{		
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	__int64 bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING|BX_VSTIFF;

	CString str;
	for (long row = 1; row < m_Grid.GetRowCount(); row+=2)
	{
		if (!(m_bSameApplySameCol || row == nRow))
			continue;
		if (!m_bSameApplySameCol)   {
			for (long n = 0; n < 2; n++)
				if (row + n != GridView->iRow)
					m_Grid.InsertColumn("", DT_CENTER|DT_VCENTER|DT_SINGLELINE, nCol, row + n);			
		}
		long nG = (row-1)/2;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		double sta = GetStationMatrix(&m_vGrid, row, nCol);		
		double staNext = GetStationMatrix(&m_vGrid, row, nCol+1);
		CPlateBasicIndex* pBx = pGir->GetBxByStation((sta+staNext)/2,bxFlag);
		double staNew = pBx->GetStation();
		SetMatrixByStation(&m_vGrid,row, nCol, staNew);
		SetMatrixByStation(&m_vGrid,row, nCol+1, staNext);
		m_vGrid.SetTextMatrix(row+1, nCol, m_vGrid.GetTextMatrix(row+1,nCol+1));		
	}

	for(long nC = 1 ; nC < m_vGrid.GetColumnCount() ; nC++) 
	{
		str.Format("%d", nC);		
		m_vGrid.SetTextMatrix(0, nC, str);
	}	
	m_Grid.SetFocus();
}

void CDetSHConnectorVPosDlg::OnColDeletedUpper(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	if(nCol == m_vGrid.GetColumnCount()-1)	return;
	nRow -= (nRow - 1) % 2;  // 길이Row로 바꿈.

	for (long row = 1; row < m_vGrid.GetRowCount(); row+=2)
	{
		if (!(m_bSameApplySameCol || row == nRow))
			continue;
		double dist1 = m_vGrid.GetTextMatrixDouble(row, nCol);
		double dist2 = m_vGrid.GetTextMatrixDouble(row, nCol + 1);
		m_vGrid.SetTextMatrix(row, nCol+1, "%.0f", dist1 + dist2);
		if (!m_bSameApplySameCol)
			for (long n = 0; n < 2; n++)
				if (row + n != GridView->iRow)
					m_vGrid.DeleteColumn(nCol, row + n);
	}

	m_vGrid.DeleteUnusedColumn();
}

void CDetSHConnectorVPosDlg::OnColDeleteUpper(NMHDR* nmgv, LRESULT*)
{
	for(long nCol = 1 ; nCol < m_vGrid.GetColumnCount() ; nCol++) 
		m_vGrid.SetTextMatrix(0, nCol, Comma(nCol));

	m_vGrid.SetRedraw();
	m_vGrid.SetFocus();
}

void CDetSHConnectorVPosDlg::OnColInsertedLower(NMHDR* nmgv, LRESULT*)
{	
	for(long nCol = 1 ; nCol < m_Grid.GetColumnCount() ; nCol++) 
		m_Grid.SetTextMatrix(0, nCol, Comma(nCol));

	m_Grid.SetRedraw();
}

void CDetSHConnectorVPosDlg::OnColDeletedLower(NMHDR* nmgv, LRESULT*)
{
	for(long nCol = 1 ; nCol < m_Grid.GetColumnCount() ; nCol++) 
		m_Grid.SetTextMatrix(0, nCol, Comma(nCol));

	m_Grid.SetRedraw();
}

void CDetSHConnectorVPosDlg::OnColDeleteLower(NMHDR* nmgv, LRESULT*)
{
	for(long nCol = 1 ; nCol < m_Grid.GetColumnCount() ; nCol++) 
		m_Grid.SetTextMatrix(0, nCol, Comma(nCol));

	m_Grid.SetRedraw();
}

void CDetSHConnectorVPosDlg::OnSconnectAllsameapply() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	UpdateData();
	pDB->m_cSameApplySameColStud = (BYTE)m_bSameApplySameCol;	
}

void CDetSHConnectorVPosDlg::OnSconnectAutoSymmetry() 
{
//	CPlateBridgeApp *pDB = m_pStd->GetBridge();

//	for(long nG = 0;nG < pDB->GetGirdersu();nG++)
//		SetSconnectAutoSymmetry(nG);		

	SetDataInitUp();
}

void CDetSHConnectorVPosDlg::OnStudFactapply() 
{
	UpdateData(TRUE);
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(-1);
	pGir->m_pSangse->m_StudJong_FactJoin = m_bApplyFact;
	//SetDataInit();
	//DrawInputDomyunView(FALSE);
}

void CDetSHConnectorVPosDlg::OnSameApplyAllGirder() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long	nGSrc, nRowSrc, nG, nRow, nCol;
	CString str;

	nGSrc = GetGirder(m_pGrid->GetFocusCell().row);
	nRowSrc = nGSrc * 2 + 1;
	for(nG = 0; nG < pDB->GetGirdersu() ; nG++) {
		if(nGSrc == nG) continue;
		nRow = nG * 2 + 1;
		for(nCol = 1 ; nCol < m_pGrid->GetColumnCount(); nCol++) {
			str = m_pGrid->GetItemText(nRowSrc, nCol);
			m_pGrid->SetTextMatrix(nRow, nCol, str);
			str = m_pGrid->GetItemText(nRowSrc + 1, nCol);
			m_pGrid->SetTextMatrix(nRow + 1, nCol, str);
		}
	}
	SetDataSave();
	m_pGrid->SetRedraw(TRUE,TRUE);	
}

void CDetSHConnectorVPosDlg::OnButtonApply() 
{
	SetDataSave();
	DrawInputDomyunView();
}

void CDetSHConnectorVPosDlg::OnStudHbeamVstiff() 
{
	// 소수주형교 가로보 연결 수직보강재 위에 전단연결재 배치하지 않는다...
	UpdateData(TRUE);
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(-1);
	pGir->m_pSangse->m_StudJong_HBeamVStiff = m_bApplyHBeamVStiff;	
}

BOOL CDetSHConnectorVPosDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	if(pDB->IsPlateGir())
		GetDlgItem(IDC_STUD_HBEAM_VSTIFF)->EnableWindow(FALSE);

	SetResize(IDC_STATIC_GROUP1, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID_UP, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_STATIC1, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC2, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_STATIC_GROUP6, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_DN, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC3, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STUD_GLENGTH, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC4, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STUD_INLENGTH, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC5, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_STUD_MINUS, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);

	SetResize(IDC_STATIC_GROUP2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STUD_GIRDERLEN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STUD_BRIDGELEN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_GROUP3, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_ANCHOR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_STUD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_SQUARE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_SHARE_ALLAPPLY, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STUD_HBEAM_VSTIFF, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SPLICECEN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STUD_FACTAPPLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_TEXT1,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_TEXT2,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_TEXT3,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STUD_MINDIS,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STUD_MIDDIS,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STUD_BASEDIS,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	return TRUE;
}

void CDetSHConnectorVPosDlg::SetText()
{
	long	nRow = 0;
	long	nG = 0;
	CString strg;
	nRow = m_pGrid->GetFocusCell().row;
	if(nRow < 1)	nG = 0;

	nG = GetGirder(nRow);
	//	Girder Length
	strg.Format("%.lf", GetGirderLength(nG));
	SendDlgItemMessage(IDC_STUD_GLENGTH, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strg);
	//	Input Length
	strg.Format("%.lf", GetInputLength(nG));
	SendDlgItemMessage(IDC_STUD_INLENGTH, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strg);
	//	Input Minus
	strg.Format("%+.lf", GetInputMinus(nG));
	SendDlgItemMessage(IDC_STUD_MINUS, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)strg);

	UpdateData(FALSE);
}

void CDetSHConnectorVPosDlg::OnRadioAnchor() 
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	for(long nG=-1; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		pGir->m_pSangse->m_StudHori_Type = 0;
		m_nStudType = 0;
	}
	if(!m_pStd->GetDlgOfIDD(IDD_DECK_08_ETCREBAR)->IsUsedFlag())
		pDeckData->m_bInstallShearRebar = FALSE;
	DrawInputDomyunView();
}

void CDetSHConnectorVPosDlg::OnRadioStud() 
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	for(long nG=-1; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		pGir->m_pSangse->m_StudHori_Type = 1;
		m_nStudType = 1;
	}
	if(!m_pStd->GetDlgOfIDD(IDD_DECK_08_ETCREBAR)->IsUsedFlag())
		pDeckData->m_bInstallShearRebar = TRUE;
	DrawInputDomyunView();
}

void CDetSHConnectorVPosDlg::OnRadioSquare() 
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	for(long nG=-1; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		pGir->m_pSangse->m_StudHori_Type = 2;
		m_nStudType = 2;
	}
	if(!m_pStd->GetDlgOfIDD(IDD_DECK_08_ETCREBAR)->IsUsedFlag())
		pDeckData->m_bInstallShearRebar = FALSE;
	DrawInputDomyunView();
}

void CDetSHConnectorVPosDlg::OnStudBridgelen() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nOldType = m_nLengthType;
	UpdateData();
	SetGridDataByChangeLengthType(&m_vGrid, nOldType);
	SetGridDataByChangeLengthType(&m_Grid,  nOldType);
	pDB->m_nLengthTypeOnStud = m_nLengthType;
	DrawInputDomyunView(FALSE);
}

void CDetSHConnectorVPosDlg::OnStudGirderlen() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	
	long nOldType = m_nLengthType;
	UpdateData();
	SetGridDataByChangeLengthType(&m_vGrid, nOldType);
	SetGridDataByChangeLengthType(&m_Grid, nOldType);
	pDB->m_nLengthTypeOnStud = m_nLengthType;
	DrawInputDomyunView(FALSE);
}

void CDetSHConnectorVPosDlg::OnCheckSplicecen() 
{
	UpdateData(TRUE);
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	for(long nG=-1; nG<pDB->GetQtyGirder(); nG++)
		pDB->GetGirder(nG)->m_pSangse->m_StudJong_SpliceCenter = m_bApplyCSplice;	
}
