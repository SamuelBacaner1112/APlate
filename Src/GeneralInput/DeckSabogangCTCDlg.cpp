// DeckSabogangCTCDlg.cpp : implementation file
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
// CDeckSabogangCTCDlg dialog


CDeckSabogangCTCDlg::CDeckSabogangCTCDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckSabogangCTCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckSabogangCTCDlg)
	m_bShowTotal = FALSE;
	m_nPlanViewType = 0;
	m_bCheckBox = FALSE;
	//}}AFX_DATA_INIT
}


void CDeckSabogangCTCDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckSabogangCTCDlg)
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_SHOW_MAINREBAR_TOTAL, m_bShowTotal);
	DDX_Radio(pDX, IDC_RADIO_ONEBODY, m_nPlanViewType);
	DDX_Check(pDX, IDC_CHECK_BEAM, m_bCheckBox);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SAME_ALL, m_btnSameAll);
	DDX_Control(pDX, IDC_BUTTON_ADD_GUGAN, m_btnAdd);
}


BEGIN_MESSAGE_MAP(CDeckSabogangCTCDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckSabogangCTCDlg)
	ON_BN_CLICKED(IDC_BUTTON_SAME_ALL, OnButtonSameAll)
	ON_BN_CLICKED(IDC_CHECK_SHOW_MAINREBAR_UPDOWN, OnCheckShowMainRebarUpDn)
	ON_BN_CLICKED(IDC_CHECK_SHOW_MAINREBAR_TOTAL, OnCheckShowMainRebarTotal)
	ON_BN_CLICKED(IDC_RADIO_ONEBODY, OnRadioOneUpDnView)
	ON_BN_CLICKED(IDC_RADIO_TWOBODY, OnRadioTwoView)
	ON_BN_CLICKED(IDC_CHECK_BEAM, OnCheckBox)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_ADD_GUGAN, OnButtonAddGugan)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)	
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)	
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckSabogangCTCDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CDeckSabogangCTCDlg message handlers
void CDeckSabogangCTCDlg::OnPreInitDialog()
{
	InitTabCtrl();

	SetResize(IDC_STATIC_VIEW_OPTION,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_VIEW_UPDN,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SHOW_MAINREBAR_TOTAL,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_BEAM,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_ONEBODY,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_TWOBODY,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,							SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,							SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAME_ALL,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ADD_GUGAN,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}

void CDeckSabogangCTCDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	double dScaleDim = 0;

	if(m_nTab==0)	// 평면
	{
		dScaleDim = 100;
		Dom.SetScaleDim(dScaleDim);
		DrawPlanRebar(&Dom);
	}
	else			// 종단		
	{
		dScaleDim = 40;
		Dom.SetScaleDim(dScaleDim);
		DrawJongRebar(&Dom);
	}

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}



//nType=-1 상면 
//nType= 0 상하면 
//nType= 1 하면 
void CDeckSabogangCTCDlg::DrawPlanSlab(CDomyun *pDom, long nType)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);

	Dom.SetDirection("TOP");

	CPlateBxFinder Finder(pDB->GetGirder(-1));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_SLAB);

	DBPlan.DrawSlabTotal(&Dom);
	DBPlan.DrawCenterLine(&Dom, -1, pBxStt->GetStation(), pBxEnd->GetStation());
	DBPlan.DrawAbutPierCenterLine(&Dom);               //교대교각중심선
	for(long g=0; g<pDB->GetGirdersu(); g++)
		DBPlan.DrawAbutPierSymbol(&Dom, g);     //교대, 교각심볼마크
	DBPlan.DrawTaperLinePyung(&Dom);			// Taper 선 그리기

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_CENT);
	DBPlan.DrawUpDownLine(&Dom, pBxStt->GetStation(), pBxEnd->GetStation());		

	// 신축이음부
	CString sText = GetCurTabName();

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_CONC);
	if (nType==-1)
	{
		DBPlan.DrawExpJointLine(&Dom, TRUE,  0);
		DBPlan.DrawExpJointLine(&Dom, FALSE, 0);
	}
	else if(nType==0)
	{
		DBPlan.DrawExpJointLine(&Dom, TRUE,  -1);
		DBPlan.DrawExpJointLine(&Dom, FALSE, -1);
	}

	*pDom << Dom;
}



void CDeckSabogangCTCDlg::DrawPlanRebar(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);
	
	CDomyun Dom(pDom);

	// 회전
	CPlateBxFinder Finder(pDB->GetGirder(0));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_JOINT);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_JOINT);

	double rotAng  = DBPlan.GetRotateAnglePyung(pBxStt,pBxEnd);
	BOOL bRotate = CAPlateDrawPyung::IsInputDomRotate();
	//		
	long nSttDan  = 0;
	CString	sText = GetCurTabName();
	// 회전
	if(m_bShowTotal)	// 전체철근보기
	{
		if(m_nPlanViewType==0)	// 일체형
		{
			DrawPlanSlab(&Dom, 0);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			Dom.SetLineColor(6);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, TRUE, DRAW_LEFTSIDE, TRUE);	
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE, TRUE);	
			Dom.SetLineColor(2);
			DBDeckPlan.DrawSabogangRebar(&Dom, TRUE, DRAW_LEFTSIDE, TRUE, TRUE, FALSE);
			DBDeckPlan.DrawSabogangRebar(&Dom, FALSE, DRAW_RIGHTSIDE, TRUE, TRUE, FALSE);
			Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, TRUE, DRAW_LEFTSIDE, TRUE, TRUE, FALSE);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, FALSE, DRAW_RIGHTSIDE, TRUE, TRUE, FALSE);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER | DIM_DECK_TOTAL_LEFT, TRUE, FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER | DIM_DECK_TOTAL_RIGHT, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "전체철근(상/하면)", TRUE, TRUE, 0);

			*pDom << Dom;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// 상면
			DrawPlanSlab(&Dom, -1);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			Dom.SetLineColor(6);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, TRUE, DRAW_SIDEALL, TRUE);	
			Dom.SetLineColor(2);
			DBDeckPlan.DrawSabogangRebar(&Dom, TRUE, DRAW_SIDEALL, TRUE, TRUE, FALSE);
			Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, TRUE, DRAW_SIDEALL, TRUE, TRUE, FALSE);
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "전체철근(상면)", TRUE, TRUE, 0);
			
			Dom.Move(0, Dom.GetExtHeight()*0.6);

			*pDom << Dom;
			//////////////////////////////////////////////////////////////////////////
			// 하면					
			DrawPlanSlab(&Dom, 1);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			Dom.SetLineColor(6);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, FALSE, DRAW_SIDEALL, TRUE);	
			Dom.SetLineColor(2);
			DBDeckPlan.DrawSabogangRebar(&Dom, FALSE, DRAW_SIDEALL, TRUE, TRUE, FALSE);
			Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, FALSE, DRAW_SIDEALL, TRUE, TRUE, FALSE);
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "전체철근(하면)", TRUE, TRUE, 0);
			
			Dom.Move(0, -Dom.GetExtHeight()*0.6);

			*pDom << Dom;
		}
	}
	else
	{
		if(m_nPlanViewType==0)	// 일체형
		{
			//////////////////////////////////////////////////////////////////////////
			// 상하면동시
			DrawPlanSlab(&Dom, 0);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			if(sText.Find("상면")!=-1)	Dom.SetLineColor(2);
			else						Dom.SetLineColor(3);
			DBDeckPlan.DrawSabogangRebar(&Dom, TRUE, DRAW_LEFTSIDE, TRUE, TRUE, FALSE);
			if(sText.Find("상면")!=-1)	Dom.SetLineColor(3);
			else						Dom.SetLineColor(2);
			DBDeckPlan.DrawSabogangRebar(&Dom, FALSE, DRAW_RIGHTSIDE, TRUE, TRUE, FALSE);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER | DIM_DECK_TOTAL_LEFT, TRUE, FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER | DIM_DECK_TOTAL_RIGHT, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "사보강부주철근(상/하면)", TRUE, TRUE, 0);

			*pDom << Dom;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// 상면
			DrawPlanSlab(&Dom, -1);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);

			if(sText=="상면(시점)")	Dom.SetLineColor(2);
			else					Dom.SetLineColor(3);
			DBDeckPlan.DrawSabogangRebar(&Dom, TRUE, DRAW_SIDEALL, TRUE, FALSE, FALSE);
			if(sText=="상면(종점)")	Dom.SetLineColor(2);
			else					Dom.SetLineColor(3);
			DBDeckPlan.DrawSabogangRebar(&Dom, TRUE, DRAW_SIDEALL, FALSE, TRUE, FALSE);
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "사보강부주철근(상면)", TRUE, TRUE, 0);

			Dom.Move(0, Dom.GetExtHeight()*0.6);
			*pDom << Dom;

			//////////////////////////////////////////////////////////////////////////
			// 하면					
			DrawPlanSlab(&Dom, 1);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			if(sText=="하면(시점)")	Dom.SetLineColor(2);
			else					Dom.SetLineColor(3);
			DBDeckPlan.DrawSabogangRebar(&Dom, FALSE, DRAW_SIDEALL, TRUE, FALSE, FALSE);
			if(sText=="하면(종점)")	Dom.SetLineColor(2);
			else					Dom.SetLineColor(3);
			DBDeckPlan.DrawSabogangRebar(&Dom, FALSE, DRAW_SIDEALL, FALSE, TRUE, FALSE);
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "사보강부주철근(하면)", TRUE, TRUE, 0);

			Dom.Move(0, -Dom.GetExtHeight()*0.6);
			*pDom << Dom;
		}
	}

	*pDom << Dom;
}



void CDeckSabogangCTCDlg::DrawJongRebar(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CDrawDBBaseRebarJong DBDeckJong(m_pStd->m_pDataManage);
	CDimDBDeckBaseJong DBDeckJongDim(m_pStd->m_pDataManage);
	CADeckData		 	 *pDeckData  = m_pStd->m_pDeckData;
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	BOOL bShowMark = pDeckData->m_bShowMark && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag() ? TRUE : FALSE;

	DBDeckJong.DrawBeryukRebarAtJong(&Dom, JONGBASEPOS_SLABLEFT, 0, TRUE);
	DBDeckJong.DrawMainRebarAtJong(&Dom, JONGBASEPOS_SLABLEFT);
	DBDeckJong.DrawShearRebarAtJong(&Dom, JONGBASEPOS_SLABLEFT);

	DBDeckJong.DimMainRebarAtJong(&Dom, JONGBASEPOS_SLABLEFT, TRUE);
	DBDeckJong.DimMainRebarAtJong(&Dom, JONGBASEPOS_SLABLEFT, FALSE);
	DBDeckJong.DimMainRebarAtJongSd(&Dom, JONGBASEPOS_SLABLEFT, TRUE);
	DBDeckJong.DimMainRebarAtJongSd(&Dom, JONGBASEPOS_SLABLEFT, FALSE);

	if(bShowMark)
	{
		for (long nSpan=0; nSpan<pDB->m_nQtyJigan; nSpan++)
		{
			DBDeckJong.DrawMarkMainRebarAtJong(&Dom, nSpan, JONGBASEPOS_SLABLEFT);
			DBDeckJong.DrawMarkShearRebar(&Dom, nSpan, JONGBASEPOS_SLABLEFT);
		}
	}


	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_TXTG);
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브좌측단", 9, TRUE, FALSE);

	*pDom << Dom;

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	DBDeckJong.DrawBeryukRebarAtJong(&Dom, JONGBASEPOS_SLABCENTER, 0, TRUE);
	DBDeckJong.DrawMainRebarAtJong(&Dom, JONGBASEPOS_SLABCENTER);
	DBDeckJong.DrawShearRebarAtJong(&Dom, JONGBASEPOS_SLABCENTER);

	DBDeckJong.DimMainRebarAtJong(&Dom, JONGBASEPOS_SLABCENTER, TRUE);
	DBDeckJong.DimMainRebarAtJong(&Dom, JONGBASEPOS_SLABCENTER, FALSE);
	DBDeckJong.DimMainRebarAtJongSd(&Dom, JONGBASEPOS_SLABCENTER, TRUE);
	DBDeckJong.DimMainRebarAtJongSd(&Dom, JONGBASEPOS_SLABCENTER, FALSE);

	if(bShowMark)
	{
		for (long nSpan=0; nSpan<pDB->m_nQtyJigan; nSpan++)
		{
			DBDeckJong.DrawMarkMainRebarAtJong(&Dom, nSpan, JONGBASEPOS_SLABCENTER);
			DBDeckJong.DrawMarkShearRebar(&Dom, nSpan, JONGBASEPOS_SLABCENTER);
		}
	}

	Dom.SetCalcExtRect();
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_TXTG);
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브중심", 9, TRUE, FALSE);
	rect = Dom.GetExtRect();
	Dom.Move(0, -rect.Height()*1.5);

	*pDom << Dom;

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	DBDeckJong.DrawBeryukRebarAtJong(&Dom, JONGBASEPOS_SLABRIGHT, 0, TRUE);
	DBDeckJong.DrawMainRebarAtJong(&Dom, JONGBASEPOS_SLABRIGHT);
	DBDeckJong.DrawShearRebarAtJong(&Dom, JONGBASEPOS_SLABRIGHT);

	DBDeckJong.DimMainRebarAtJong(&Dom, JONGBASEPOS_SLABRIGHT, TRUE);
	DBDeckJong.DimMainRebarAtJong(&Dom, JONGBASEPOS_SLABRIGHT, FALSE);
	DBDeckJong.DimMainRebarAtJongSd(&Dom, JONGBASEPOS_SLABRIGHT, TRUE);
	DBDeckJong.DimMainRebarAtJongSd(&Dom, JONGBASEPOS_SLABRIGHT, FALSE);

	if(bShowMark)
	{
		for (long nSpan=0; nSpan<pDB->m_nQtyJigan; nSpan++)
		{
			DBDeckJong.DrawMarkMainRebarAtJong(&Dom, nSpan, JONGBASEPOS_SLABRIGHT);
			DBDeckJong.DrawMarkShearRebar(&Dom, nSpan, JONGBASEPOS_SLABRIGHT);
		}
	}
	
	Dom.SetCalcExtRect();
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_TXTG);
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브우측단", 9, TRUE, FALSE);
	rect = Dom.GetExtRect();
	Dom.Move(0, -rect.Height()*2.5);

	*pDom << Dom;
}

CDPointArray* CDeckSabogangCTCDlg::GetCTCRebar(CString szCTC) 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	CString sText = szCTC=="" ? GetCurTabName() : szCTC;
	
	if(sText=="상면(시점)")
	{
		return &pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT];
	}
	else if(sText== "하면(시점)")
	{
		return &pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT];
	}
	else if(sText== "상면(종점)")
	{
		return &pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END];
	}
	else if(sText=="하면(종점)")
	{
		return &pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END];
	}

	return NULL;
	
}


// 사보강철근과 신축이음부 하부 철근은 좌,우측 구분이 없음.
void CDeckSabogangCTCDlg::SetCTCRebar(CDPointArray &xyArrCTC, CString szCTC)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sText = szCTC=="" ? GetCurTabName() : szCTC;

	if(sText=="상면(시점)")
	{
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT].RemoveAll();
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT] = xyArrCTC;
	}
	else if(sText=="하면(시점)")
	{
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT].RemoveAll();
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT] = xyArrCTC;
	}
	else if(sText=="상면(종점)")
	{
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END].RemoveAll();
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END] = xyArrCTC;
	}
	else if(sText=="하면(종점)")
	{
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END].RemoveAll();
		pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END] = xyArrCTC;
	}

}


void CDeckSabogangCTCDlg::SetGridData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	if(pDeckData->IsSkew()==FALSE)
		return ;
	

	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	
	long nRowsu = 3;
	CDPointArray *pArrCTC = GetCTCRebar();
	if(pArrCTC==NULL)	return;

	long nCTCsu = pArrCTC->GetSize();
	long nColsu = nCTCsu+2;

	m_Grid.SetGridDefault(nRowsu, nColsu, 1, 1);
	
	m_Grid.AddCell(0, 0, "구분");
	m_Grid.AddCell(1, 0, "개수");	
	m_Grid.AddCell(2, 0, "간격");	
	long nCol = 0;
	for(nCol=1; nCol<nColsu; nCol++)
	{
		sz.Format("%d구간", nCol);
		m_Grid.AddCell(0, nCol, sz);
	}

	// 슬래브좌측(실제길이)
	double dTotalN = 0;
	double dTotalL = 0;
	for(nCol=0; nCol<pArrCTC->GetSize(); nCol++)
	{
		CDPoint *xyCTC = &pArrCTC->GetAt(nCol);
		m_Grid.AddCell(1, nCol+1, &xyCTC->x);	m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(2, nCol+1, &xyCTC->y);	m_Grid.SetCellDec(CELL_DEC_3);
		dTotalN += xyCTC->x;
		dTotalL += xyCTC->x * xyCTC->y;
	}	

	m_Grid.AddCell(0, nColsu-1,"TOTAL");
	sz.Format("%.0f", dTotalN);
	m_Grid.AddCell(1, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_Grid.AddCell(2, nColsu-1, sz, nFormat, CELL_READONLY);

}



void CDeckSabogangCTCDlg::SetDataDefault() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	BOOL bFirst = IsUsedFlag() ? FALSE : TRUE;
	if(bFirst)
		pDeckData->m_dErrorRangeCTC = 0.01;

	SetDataDefaultRange();
	if(pDeckData->IsSkewStt() || pDeckData->IsSkewEnd())
		pDeckData->CalcCTCSabogangRebar();

	pDeckData->MakePlanSabogang();

	m_pStd->m_pDoc->SetModifiedFlag();
}




void CDeckSabogangCTCDlg::SetDataDefaultRange()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CPlateGirderApp *pGir = pDB->GetGirder(-1);

	double dStaSlabSttCen = pGir->GetSSS(OFF_STT_SLAB);
	double dStaSlabEndCen = pGir->GetSSS(OFF_END_SLAB);
	double dDisLStt		  = pDB->GetDisSlabLeftAct(dStaSlabSttCen) + pDeckData->m_dCoverSd;
	double dDisRStt		  = pDB->GetDisSlabRighAct(dStaSlabSttCen) - pDeckData->m_dCoverSd;
	double dDisLEnd		  = dDisLStt;
	double dDisREnd		  = dDisRStt;
	CDPoint	vAngStt		  = pGir->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd		  = pGir->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft= pDB->m_pLine->GetStationByScInLine(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh= pDB->m_pLine->GetStationByScInLine(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndLeft= pDB->m_pLine->GetStationByScInLine(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh= pDB->m_pLine->GetStationByScInLine(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dLengthSkewCenStt = pDB->m_pLine->GetLengthByDis(dStaSlabSttLeft, dStaSlabSttRigh, (dDisLStt+dDisRStt)/2);
	double dLengthSkewCenEnd = pDB->m_pLine->GetLengthByDis(dStaSlabEndLeft, dStaSlabEndRigh, (dDisLEnd+dDisREnd)/2);
	
	BOOL bSkewStt = pDeckData->IsSkewStt();
	BOOL bSkewEnd = pDeckData->IsSkewEnd();
	
	pDeckData->m_dRangeSabogangRebarStt = 0;
	pDeckData->m_dRangeSabogangRebarEnd = 0;

	if(bSkewStt)
		pDeckData->m_dRangeSabogangRebarStt = dLengthSkewCenStt/2;
	else
		pDeckData->m_dRangeSabogangRebarStt = 0;

	if(bSkewEnd)
		pDeckData->m_dRangeSabogangRebarEnd = dLengthSkewCenEnd/2;
	else
		pDeckData->m_dRangeSabogangRebarEnd = 0;

}



void CDeckSabogangCTCDlg::SetDataInit() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	SetGridData();

	if(pDeckData->IsSkew()==FALSE)
	{
		m_Grid.DeleteAllItems();
		return ;
	}

	m_Grid.UpdateData(FALSE);
	m_Grid.SetColumnWidthAll(55);	
	
	m_Grid.SetColumnWidth(m_Grid.GetColumnCount()-1, 80);
		
	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);
	
	CDPointArray *pArr = GetCTCRebar(GetCurTabName());
	if(pArr->GetSize()==0)
		GetDlgItem(IDC_BUTTON_ADD_GUGAN)->ShowWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_ADD_GUGAN)->ShowWindow(FALSE);
	
	UpdateData(FALSE);
}




void CDeckSabogangCTCDlg::SetDataSave() 
{
	m_Grid.UpdateData(TRUE);

}



void CDeckSabogangCTCDlg::InitTabCtrl()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nTab = 0;
	
	m_ctlTab.DeleteAllItems();
	if(pDeckData->IsSkewStt())
	{
		m_ctlTab.InsertItem(nTab++, "상면(시점)");
		m_ctlTab.InsertItem(nTab++, "하면(시점)");
	}
	if(pDeckData->IsSkewEnd())
	{
		m_ctlTab.InsertItem(nTab++, "상면(종점)");
		m_ctlTab.InsertItem(nTab++, "하면(종점)");
	}
	m_ctlTab.SetCurSel(0);
}

BOOL CDeckSabogangCTCDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_RBUTTONDOWN)
	{		
		CCellID cellID = m_Grid.GetFocusCell();				
		CString sText = m_Grid.GetTextMatrix(0, cellID.col);
		if(sText=="TOTAL" || sText=="배근범위" || sText=="오차")		
			m_Grid.EnableInsertCol(FALSE);					
		else		
			m_Grid.EnableInsertCol(TRUE);					
	}	
	
	m_Grid.ViewGridCellForCombo(pMsg);


	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CDeckSabogangCTCDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;
	
	SetDataSave();
	
	DrawInputDomyunView(FALSE);
}


void CDeckSabogangCTCDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetFocus();	
	SetDataInit();

	UpdateData();

	m_Grid.SetFocusCell(1, 3);

	DrawInputDomyunView();

}



void CDeckSabogangCTCDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();

}


CString CDeckSabogangCTCDlg::GetCurTabName()
{
	CString sText;
	TCITEM item ;
	char cTableName[256];
	item.mask = TCIF_TEXT;
	item.pszText = cTableName;
	item.cchTextMax = 255;

	m_ctlTab.GetItem(m_ctlTab.GetCurSel(), &item);
	sText = item.pszText;

	return sText;
}





//추가
void CDeckSabogangCTCDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	

	int nCol = pGridView->iColumn;

	InsertCTCByGridCol(nCol);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}



//삭제
void CDeckSabogangCTCDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	

	int nCol = pGridView->iColumn;	

	DeleteCTCByGridCol(nCol);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}




long CDeckSabogangCTCDlg::GetTabCount()
{
	return 2;
}



CString CDeckSabogangCTCDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0)			return "평면";
	else				return "종단면";
}



void CDeckSabogangCTCDlg::InsertCTCByGridCol(long nGridCol)
{
	long nGugan = nGridCol-3;
	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sTab = GetCurTabName();
	BOOL bUpper = FALSE;
	BOOL bStt = FALSE;

	if(sTab=="상면(시점)")	{ bUpper = TRUE;	bStt = TRUE;}
	if(sTab=="하면(시점)")	{ bUpper = FALSE;	bStt = TRUE;}
	if(sTab=="상면(종점)")	{ bUpper = TRUE;	bStt = FALSE;}
	if(sTab=="하면(종점)")	{ bUpper = FALSE;	bStt = FALSE;}

	CDPointArray *pArr = &pDeckData->m_arrCTCSabogangRebar[bUpper ? (bStt?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_UPPER_END) : (bStt?CTC_SABOGANG_LOWER_STT:CTC_SABOGANG_LOWER_END)];
	long   nRebarsu = long(pArr->GetAt(nGugan).x);
	double dCTC     = pArr->GetAt(nGugan).y;

	pArr->GetAt(nGugan).x = long(nRebarsu/2);
	pArr->GetAt(nGugan).y = dCTC;
	pArr->InsertAt(nGugan, CDPoint(nRebarsu-long(nRebarsu/2), dCTC));
}



void CDeckSabogangCTCDlg::DeleteCTCByGridCol(long nGridCol)
{
	long nGugan = nGridCol-1;

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sTab = GetCurTabName();
	BOOL bUpper = FALSE;
	BOOL bStt = FALSE;

	if(sTab=="상면(시점)")	{ bUpper = TRUE;	bStt = TRUE;}
	if(sTab=="하면(시점)")	{ bUpper = FALSE;	bStt = TRUE;}
	if(sTab=="상면(종점)")	{ bUpper = TRUE;	bStt = FALSE;}
	if(sTab=="하면(종점)")	{ bUpper = FALSE;	bStt = FALSE;}

	CDPointArray *pArr = &pDeckData->m_arrCTCSabogangRebar[bUpper ? (bStt?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_UPPER_END) : (bStt?CTC_SABOGANG_LOWER_STT:CTC_SABOGANG_LOWER_END)];

	if(pArr->GetSize()==1)	// 이음이 하나일때
	{
		pArr->RemoveAt(nGugan);
	}
	else if(pArr->GetSize()>1 && nGugan==pArr->GetSize()-1)	// 마지막 이음위치
	{
		long   nRebarsu1 = long(pArr->GetAt(nGugan).x);
		double dCTC1     = pArr->GetAt(nGugan).y;
		long   nRebarsu2 = long(pArr->GetAt(nGugan-1).x);
		double dCTC2     = pArr->GetAt(nGugan-1).y;

		pArr->RemoveAt(nGugan);
		pArr->SetAt(nGugan-1, CDPoint((nRebarsu1*dCTC1)/dCTC2 + nRebarsu2, dCTC2));
	}
	else			// 이음이 둘이상일 경우
	{
		long   nRebarsu1 = long(pArr->GetAt(nGugan).x);
		double dCTC1     = pArr->GetAt(nGugan).y;
		long   nRebarsu2 = long(pArr->GetAt(nGugan+1).x);
		double dCTC2     = pArr->GetAt(nGugan+1).y;
		
		pArr->RemoveAt(nGugan);
		pArr->SetAt(nGugan, CDPoint((nRebarsu1*dCTC1)/dCTC2 + nRebarsu2, dCTC2));
	}
}



void CDeckSabogangCTCDlg::OnCheckShowMainRebarUpDn() 
{
	UpdateData(TRUE);

	DrawInputDomyunView(FALSE);	
	
}

void CDeckSabogangCTCDlg::OnCheckShowMainRebarTotal() 
{
	UpdateData(TRUE);

	DrawInputDomyunView(FALSE);	
}




void CDeckSabogangCTCDlg::OnRadioOneUpDnView() 
{
	UpdateData( );

	DrawInputDomyunView();
	
}

void CDeckSabogangCTCDlg::OnRadioTwoView() 
{
	UpdateData();

	DrawInputDomyunView();
	
}


void CDeckSabogangCTCDlg::OnButtonSameAll() 
{
	CString	sText = GetCurTabName();

	CDPointArray *pArr = GetCTCRebar(sText);

	for (long n=0; n<4; n++)
	{
		CString szCur = "";
		if(n==0)		szCur = "상면(시점)";
		else if(n==1)	szCur = "하면(시점)";
		else if(n==2)	szCur = "상면(종점)";
		else if(n==3)	szCur = "하면(종점)";

		if(szCur==sText)	continue;

		SetCTCRebar(*pArr, szCur);
	}

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE); 	
}


BOOL CDeckSabogangCTCDlg::IsValid() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	if(pDeckData->IsSkew())
		return TRUE;
	else
		return FALSE;

}

void CDeckSabogangCTCDlg::OnCheckBox() 
{
	UpdateData(TRUE);

	DrawInputDomyunView();			
}


void CDeckSabogangCTCDlg::OnButtonApply() 
{
	UpdateData();
	SetDataSave();
	SetDataInit();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakePlanSabogang();

	DrawInputDomyunView();
}

void CDeckSabogangCTCDlg::OnButtonPrev() 
{
	UpdateData();
	SetDataSave();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakePlanSabogang();
	
	m_pView->GetDomyun()->ClearEtt(TRUE);	
	m_pStd->MovePrevDialog();	
}

void CDeckSabogangCTCDlg::OnButtonNext() 
{
	UpdateData();
	SetDataSave();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakePlanSabogang();
	
	m_pView->GetDomyun()->ClearEtt(TRUE);	
	m_pStd->MoveNextDialog();	
}


void CDeckSabogangCTCDlg::OnButtonAddGugan() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sTab = GetCurTabName();
	BOOL bUpper = FALSE;
	BOOL bStt = FALSE;

	if(sTab=="상면(시점)")	{ bUpper = TRUE;	bStt = TRUE;}
	if(sTab=="하면(시점)")	{ bUpper = FALSE;	bStt = TRUE;}
	if(sTab=="상면(종점)")	{ bUpper = TRUE;	bStt = FALSE;}
	if(sTab=="하면(종점)")	{ bUpper = FALSE;	bStt = FALSE;}

	CDPointArray *pArr = &pDeckData->m_arrCTCSabogangRebar[bUpper ? (bStt?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_UPPER_END) : (bStt?CTC_SABOGANG_LOWER_STT:CTC_SABOGANG_LOWER_END)];

	pArr->RemoveAll();
	pArr->Add(CDPoint(1,125));

	SetDataInit();
}
