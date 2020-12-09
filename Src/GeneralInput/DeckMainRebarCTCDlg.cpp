// DeckMainRebarCTCDlg.cpp : implementation file
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
// CDeckMainRebarCTCDlg dialog
CDeckMainRebarCTCDlg::CDeckMainRebarCTCDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckMainRebarCTCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckMainRebarCTCDlg)
	m_bCheckBox     = FALSE;
	m_bShowTotal    = FALSE;
	m_nPlanViewType = 0;// 일체형
	m_bCheckAutoCTC = TRUE;

	//}}AFX_DATA_INIT
}


void CDeckMainRebarCTCDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckMainRebarCTCDlg)
	DDX_Control(pDX, IDC_COMBO_MAIN_CTC_EXTRA, m_comboMainCTCExtra);
	DDX_Control(pDX, IDC_COMBO_JONG_MAIN_CTC, m_comboMainCTCBasePos);
	DDX_Control(pDX, IDC_EDIT_ERROR, m_dRangeError);
	DDX_Control(pDX, IDC_EDIT_SLAB_STT, m_dRangeDanbuStt);
	DDX_Control(pDX, IDC_EDIT_SLAB_END, m_dRangeDanbuEnd);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRID2, m_GridDim);
	DDX_Check(pDX, IDC_CHECK_BEAM, m_bCheckBox);
	DDX_Check(pDX, IDC_CHECK_SHOW_MAINREBAR_TOTAL, m_bShowTotal);
	DDX_Radio(pDX, IDC_RADIO_ONEBODY, m_nPlanViewType);
	DDX_Check(pDX, IDC_CHECK_CTC, m_bCheckAutoCTC);

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_CALC_REALTODIM, m_btnError);
	DDX_Control(pDX, IDC_BUTTON_SAME_ALL, m_btnSameAll);
}


BEGIN_MESSAGE_MAP(CDeckMainRebarCTCDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckMainRebarCTCDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_JONG_MAIN_CTC, OnSelchangeComboMainCTCBasePos)
	ON_CBN_SELCHANGE(IDC_COMBO_MAIN_CTC_EXTRA, OnSelchangeComboMainCTCExtraOption)
	ON_BN_CLICKED(IDC_BUTTON_CALC_REALTODIM, OnButtonCalcRealToDim)
	ON_BN_CLICKED(IDC_CHECK_BEAM, OnCheckBox)
	ON_BN_CLICKED(IDC_CHECK_SHOW_MAINREBAR_UPDOWN, OnCheckShowMainRebarUpDn)
	ON_BN_CLICKED(IDC_CHECK_SHOW_MAINREBAR_TOTAL, OnCheckShowMainRebarTotal)
	ON_BN_CLICKED(IDC_RADIO_ONEBODY, OnRadioOneUpDnView)
	ON_BN_CLICKED(IDC_RADIO_TWOBODY, OnRadioTwoView)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SAME_ALL, OnButtonSameAll)
	ON_EN_KILLFOCUS(IDC_EDIT_SLAB_STT, OnKillfocusEditSlabStt)
	ON_EN_KILLFOCUS(IDC_EDIT_SLAB_END, OnKillfocusEditSlabEnd)
	ON_EN_KILLFOCUS(IDC_EDIT_ERROR, OnKillfocusEditError)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)	
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID2, OnCellChangedDataDim)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID2, OnCellInsertedColDim)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID2, OnCellDeletedColDim)	
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckMainRebarCTCDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// CDeckMainRebarCTCDlg message handlers
void CDeckMainRebarCTCDlg::OnPreInitDialog()
{
	InitTabCtrl();
//	m_comboMainCTCBasePos.AddString("도로중심");
	m_comboMainCTCBasePos.AddString("슬래브중심");
	m_comboMainCTCBasePos.AddString("곡률반경이 큰쪽");
	
	SetResize(IDC_STATIC_CTC_OPTION,	SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_MAINCTC,		SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_EXTRA,			SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_JONG_MAIN_CTC,	SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_MAIN_CTC_EXTRA,	SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_DANRANGE,		SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SLAB_STT,		SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_SLAB_STT,		SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_SLAB_END,		SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_SLAB_END,		SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_GROUP2,		SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_1,				SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_ERROR,			SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_CALC_REALTODIM,SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_VIEW_OPTION,	SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SHOW_MAINREBAR_TOTAL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_BEAM,			SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_ONEBODY,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_TWOBODY,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_SAME_ALL,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_CTC,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_TAB,					SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GRID2,				SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDeckMainRebarCTCDlg::DrawInputDomyunView(BOOL bZoomAll)
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
void CDeckMainRebarCTCDlg::DrawPlanSlab(CDomyun *pDom, long nType)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);

	CPlateBxFinder Finder(pDB->GetGirder(-1));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_SLAB);

	CDomyun Dom(pDom);

	Dom.SetDirection("TOP");

	DBPlan.DrawSlabTotal(&Dom);
	DBPlan.DrawAbutPierCenterLine(&Dom);               //교대교각중심선
	for(long g=0; g<pDB->GetGirdersu(); g++)
		DBPlan.DrawAbutPierSymbol(&Dom, g);     //교대, 교각심볼마크
	DBPlan.DrawCenterLine(&Dom, -1, pBxStt->GetStation(), pBxEnd->GetStation());
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



void CDeckMainRebarCTCDlg::DrawPlanRebar(CDomyun *pDom)
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
	BOOL bDrawJoint = FALSE;
	
	if(m_bShowTotal)	// 전체철근	보기
	{
		if(m_nPlanViewType==0)	// 일체형
		{
			DrawPlanSlab(&Dom, 0);
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);

			Dom.SetLineColor(6);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, TRUE, DRAW_LEFTSIDE, TRUE, bDrawJoint);	
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE, TRUE, bDrawJoint);		
			Dom.SetLineColor(2);
			DBDeckPlan.DrawSabogangRebar(&Dom, TRUE, DRAW_LEFTSIDE, TRUE, TRUE, FALSE);
			DBDeckPlan.DrawSabogangRebar(&Dom, FALSE, DRAW_RIGHTSIDE, TRUE, TRUE, FALSE);
			Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, TRUE, DRAW_LEFTSIDE, TRUE, TRUE, FALSE);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, FALSE, DRAW_RIGHTSIDE, TRUE, TRUE, FALSE);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_DECK_MAINREBAR_UPPER | DIM_DECK_TOTAL_LEFT, TRUE, FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_DECK_MAINREBAR_LOWER | DIM_DECK_TOTAL_RIGHT, FALSE,FALSE,"",nSttDan);

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
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, TRUE, DRAW_SIDEALL, TRUE, bDrawJoint);		
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
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, FALSE, DRAW_SIDEALL, TRUE, bDrawJoint);		
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
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_CONC);
			DrawPlanSlab(&Dom, 0);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			if(sText=="상면")	Dom.SetLineColor(2);
			else				Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, TRUE, DRAW_LEFTSIDE, TRUE, bDrawJoint);		
			if(sText=="상면")	Dom.SetLineColor(3);
			else				Dom.SetLineColor(2);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE, TRUE, bDrawJoint);		

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER | DIM_DECK_TOTAL_LEFT, TRUE, FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER | DIM_DECK_TOTAL_RIGHT, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "주철근(상/하면)", TRUE, TRUE, 0);

			*pDom << Dom;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// 상면
			DrawPlanSlab(&Dom, -1);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			if(sText=="상면")	Dom.SetLineColor(2);
			else				Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, TRUE, DRAW_SIDEALL, TRUE, bDrawJoint);		
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd, DIM_DECK_MAINREBAR_UPPER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "주철근(상면)", TRUE, TRUE, 0);

			Dom.Move(0, Dom.GetExtHeight()*0.6);
			*pDom << Dom;

			//////////////////////////////////////////////////////////////////////////
			// 하면					
			DrawPlanSlab(&Dom, 1);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			if(sText=="하면")	Dom.SetLineColor(2);
			else				Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebar(&Dom, pBxStt, pBxEnd, FALSE, DRAW_SIDEALL, TRUE, bDrawJoint);		
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "주철근(하면)", TRUE, TRUE, 0);

			Dom.Move(0, -Dom.GetExtHeight()*0.6);
			*pDom << Dom;
		}
	}

	*pDom << Dom;
}



void CDeckMainRebarCTCDlg::DrawJongRebar(CDomyun *pDom)
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
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브좌측단", TRUE, FALSE, 0);

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
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브중심", TRUE, FALSE, 0);
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
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브우측단", TRUE, FALSE, 0);
	rect = Dom.GetExtRect();
	Dom.Move(0, -rect.Height()*2.5);

	*pDom << Dom;
}


CDPointArray* CDeckMainRebarCTCDlg::GetCTCRebar(long nPosSlab, BOOL bRealCTC, CString szCTC) 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	CString sText = szCTC=="" ? GetCurTabName() : szCTC;
	
	long nCTCType = 0;
	if(sText== "상면")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_MAIN_UPPER_LEFT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_MAIN_UPPER_RIGHT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_MAIN_UPPER_CENTER;

		return bRealCTC ? &pDeckData->m_arrCTCMainRebar[nCTCType] : &pDeckData->m_arrCTCMainRebarForDim[nCTCType];
	}
	else if(sText=="하면")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_MAIN_LOWER_LEFT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_MAIN_LOWER_RIGHT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_MAIN_LOWER_CENTER;

		return bRealCTC ? &pDeckData->m_arrCTCMainRebar[nCTCType] : &pDeckData->m_arrCTCMainRebarForDim[nCTCType];
	}


	return NULL;
	
}


// 사보강철근과 신축이음부 하부 철근은 좌,우측 구분이 없음.
void CDeckMainRebarCTCDlg::SetCTCRebar(CDPointArray &xyArrCTC, long nPosSlab, BOOL bRealCTC, CString szCTC)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sText = szCTC=="" ? GetCurTabName() : szCTC;

	long nCTCType = 0;
	if(sText== "상면")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_MAIN_UPPER_LEFT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_MAIN_UPPER_RIGHT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_MAIN_UPPER_CENTER;

		if(bRealCTC)
		{
			pDeckData->m_arrCTCMainRebar[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebar[nCTCType] = xyArrCTC;
		}
		else
		{
			pDeckData->m_arrCTCMainRebarForDim[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarForDim[nCTCType] = xyArrCTC;
		}
	}
	else if(sText=="하면")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_MAIN_LOWER_LEFT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_MAIN_LOWER_RIGHT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_MAIN_LOWER_CENTER;

		if(bRealCTC)
		{
			pDeckData->m_arrCTCMainRebar[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebar[nCTCType] = xyArrCTC;
		}
		else
		{
			pDeckData->m_arrCTCMainRebarForDim[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarForDim[nCTCType] = xyArrCTC;
		}
	}
}


void CDeckMainRebarCTCDlg::SetGridData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	long nCTCLeftsu = GetCTCRebar(POS_SLAB_LEFT, TRUE)->GetSize();
	long nCTCRighsu = GetCTCRebar(POS_SLAB_RIGH, TRUE)->GetSize();
	long nColsu     = max(max(nCTCLeftsu+4, nCTCRighsu+4)+2, 4);

	m_Grid.SetGridDefault(1, 3, 1, 3);
	
	m_Grid.AddCell(0, 0, "구분");
	for (long nRow=1; nRow<=6; nRow++)
		m_Grid.AddCell(nRow, 0, "실제길이\n");
	
	m_Grid.AddCell(0, 1, "위치");				m_Grid.AddCell(0, 2, "구분");
	m_Grid.AddCell(1, 1, "슬래브좌측\n");		m_Grid.AddCell(1, 2, "개수");
	m_Grid.AddCell(2, 1, "슬래브좌측\n");		m_Grid.AddCell(2, 2, "간격");
	m_Grid.AddCell(3, 1, "슬래브중앙\n");		m_Grid.AddCell(3, 2, "개수");
	m_Grid.AddCell(4, 1, "슬래브중앙\n");		m_Grid.AddCell(4, 2, "간격");
	m_Grid.AddCell(5, 1, "슬래브우측\n");		m_Grid.AddCell(5, 2, "개수");
	m_Grid.AddCell(6, 1, "슬래브우측\n");		m_Grid.AddCell(6, 2, "간격");
	long nCol = 0;
	for(nCol=3; nCol<nColsu; nCol++)
	{
		sz.Format("%d구간", nCol-2);
		m_Grid.AddCell(0, nCol, sz);
	}

	CDPointArray *pArrLeftReal = GetCTCRebar(POS_SLAB_LEFT, TRUE);
	CDPointArray *pArrRighReal = GetCTCRebar(POS_SLAB_RIGH, TRUE);
	CDPointArray *pArrCentReal = GetCTCRebar(POS_SLAB_CENTER, TRUE);
	
	// 슬래브좌측(실제길이)
	double dTotalN = 0;
	double dTotalL = 0;
	for(nCol=0; nCol<pArrLeftReal->GetSize(); nCol++)
	{
		CDPoint *xyLeftReal = &pArrLeftReal->GetAt(nCol);
		m_Grid.AddCell(1, nCol+3, &xyLeftReal->x);	m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(2, nCol+3, &xyLeftReal->y);	m_Grid.SetCellDec(CELL_DEC_3);
		dTotalN += xyLeftReal->x;
		dTotalL += xyLeftReal->x * xyLeftReal->y;
	}	
	double dLengthRange = 0;
	sz.Format("%.0f", dTotalN);
	m_Grid.AddCell(1, nColsu-3, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_Grid.AddCell(2, nColsu-3, sz, nFormat, CELL_READONLY);
	m_Grid.AddCell(1, nColsu-2, "-", nFormat, CELL_READONLY);
	dLengthRange = pDeckData->GetLengthRebarRangeAtJongPos(JONGBASEPOS_SLABLEFT, GetCurTabName()=="상면" ? TRUE : FALSE);
	sz.Format("%.3f", dLengthRange);
	m_Grid.AddCell(2, nColsu-2, sz, nFormat, CELL_READONLY);
	m_Grid.AddCell(1, nColsu-1, "-", nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL-dLengthRange);
	m_Grid.AddCell(2, nColsu-1, sz, nFormat, CELL_READONLY);
	
	// 슬래브 중앙(실제길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrCentReal->GetSize(); nCol++)
	{
		CDPoint *xyCentReal = &pArrCentReal->GetAt(nCol);
		m_Grid.AddCell(3, nCol+3, &xyCentReal->x);		m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(4, nCol+3, &xyCentReal->y);		m_Grid.SetCellDec(CELL_DEC_3);
		dTotalN += xyCentReal->x;
		dTotalL += xyCentReal->x * xyCentReal->y;
	}	

	sz.Format("%.0f", dTotalN);
	m_Grid.AddCell(3, nColsu-3, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_Grid.AddCell(4, nColsu-3, sz, nFormat, CELL_READONLY);
	m_Grid.AddCell(3, nColsu-2, "-", nFormat, CELL_READONLY);
	dLengthRange = pDeckData->GetLengthRebarRangeAtJongPos(JONGBASEPOS_SLABCENTER, GetCurTabName()=="상면" ? TRUE : FALSE);
	sz.Format("%.3f", dLengthRange);
	m_Grid.AddCell(4, nColsu-2, sz, nFormat, CELL_READONLY);
	m_Grid.AddCell(3, nColsu-1, "-", nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL-dLengthRange);
	m_Grid.AddCell(4, nColsu-1, sz, nFormat, CELL_READONLY);

	// 슬래브우측(실제길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrRighReal->GetSize(); nCol++)
	{
		CDPoint *xyRighReal = &pArrRighReal->GetAt(nCol);
		m_Grid.AddCell(5, nCol+3, &xyRighReal->x);	m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(6, nCol+3, &xyRighReal->y);	m_Grid.SetCellDec(CELL_DEC_3);
		dTotalN += xyRighReal->x;
		dTotalL += xyRighReal->x * xyRighReal->y;
	}	

	sz.Format("%.0f", dTotalN);
	m_Grid.AddCell(5, nColsu-3, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_Grid.AddCell(6, nColsu-3, sz, nFormat, CELL_READONLY);
	m_Grid.AddCell(5, nColsu-2, "-", nFormat, CELL_READONLY);
	dLengthRange = pDeckData->GetLengthRebarRangeAtJongPos(JONGBASEPOS_SLABRIGHT, GetCurTabName()=="상면" ? TRUE : FALSE);
	sz.Format("%.3f", dLengthRange);
	m_Grid.AddCell(6, nColsu-2, sz, nFormat, CELL_READONLY);
	m_Grid.AddCell(5, nColsu-1, "-", nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL-dLengthRange);
	m_Grid.AddCell(6, nColsu-1, sz, nFormat, CELL_READONLY);

	m_Grid.AddCell(0, nColsu-3,"TOTAL");
	m_Grid.AddCell(0, nColsu-2,"배근범위");
	m_Grid.AddCell(0, nColsu-1,"오차");
}




void CDeckMainRebarCTCDlg::SetGridDataDim() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	
	long nRowsu = 7;
	long nCTCLeftsu = GetCTCRebar(POS_SLAB_LEFT, FALSE)->GetSize();
	long nCTCRighsu = GetCTCRebar(POS_SLAB_RIGH, FALSE)->GetSize();
	long nColsu = max(max(nCTCLeftsu+4, nCTCRighsu+4)+2, 4);

	m_GridDim.SetGridDefault(nRowsu, nColsu, 1, 3);
	
	m_GridDim.AddCell(0, 0, "구분");
	for (long nRow=1; nRow<=6; nRow++)
		m_GridDim.AddCell(nRow, 0, "도면치수\n");
	
	m_GridDim.AddCell(0, 1, "위치");				m_GridDim.AddCell(0, 2, "구분");
	m_GridDim.AddCell(1, 1, "슬래브좌측\n");		m_GridDim.AddCell(1, 2, "개수");
	m_GridDim.AddCell(2, 1, "슬래브좌측\n");		m_GridDim.AddCell(2, 2, "간격");
	m_GridDim.AddCell(3, 1, "슬래브중앙\n");		m_GridDim.AddCell(3, 2, "개수");
	m_GridDim.AddCell(4, 1, "슬래브중앙\n");		m_GridDim.AddCell(4, 2, "간격");
	m_GridDim.AddCell(5, 1, "슬래브우측\n");		m_GridDim.AddCell(5, 2, "개수");
	m_GridDim.AddCell(6, 1, "슬래브우측\n");		m_GridDim.AddCell(6, 2, "간격");
	long nCol = 0;
	for(nCol=3; nCol<nColsu; nCol++)
	{
		sz.Format("%d구간", nCol-2);
		m_GridDim.AddCell(0, nCol, sz);
	}

	CDPointArray *pArrLeftForDim = GetCTCRebar(POS_SLAB_LEFT, FALSE);
	CDPointArray *pArrRighForDim = GetCTCRebar(POS_SLAB_RIGH, FALSE);
	CDPointArray *pArrCentForDim = GetCTCRebar(POS_SLAB_CENTER, FALSE);
	
	// 슬래브좌측(치수길이)
	double dTotalN = 0;
	double dTotalL = 0;
	for(nCol=0; nCol<pArrLeftForDim->GetSize(); nCol++)
	{
		CDPoint *xyLeftForDim = &pArrLeftForDim->GetAt(nCol);
		m_GridDim.AddCell(1, nCol+3, &xyLeftForDim->x);	m_GridDim.SetCellDec(CELL_DEC_0);
		m_GridDim.AddCell(2, nCol+3, &xyLeftForDim->y);	m_GridDim.SetCellDec(CELL_DEC_3);
		dTotalN += xyLeftForDim->x;
		dTotalL += xyLeftForDim->x * xyLeftForDim->y;
	}	

	double dLengthRange = 0;
	sz.Format("%.0f", dTotalN);
	m_GridDim.AddCell(1, nColsu-3, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_GridDim.AddCell(2, nColsu-3, sz, nFormat, CELL_READONLY);
	m_GridDim.AddCell(1, nColsu-2, "-", nFormat, CELL_READONLY);
	dLengthRange = pDeckData->GetLengthRebarRangeAtJongPos(JONGBASEPOS_SLABLEFT, GetCurTabName()=="상면" ? TRUE : FALSE);
	sz.Format("%.3f", dLengthRange);
	m_GridDim.AddCell(2, nColsu-2, sz, nFormat, CELL_READONLY);
	m_GridDim.AddCell(1, nColsu-1, "-", nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL-dLengthRange);
	m_GridDim.AddCell(2, nColsu-1, sz, nFormat, CELL_READONLY);

	// 슬래브중앙(치수길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrCentForDim->GetSize(); nCol++)
	{
		CDPoint *xyCentForDim = &pArrCentForDim->GetAt(nCol);
		m_GridDim.AddCell(3, nCol+3, &xyCentForDim->x);		m_GridDim.SetCellDec(CELL_DEC_0);
		m_GridDim.AddCell(4, nCol+3, &xyCentForDim->y);		m_GridDim.SetCellDec(CELL_DEC_3);
		dTotalN += xyCentForDim->x;
		dTotalL += xyCentForDim->x * xyCentForDim->y;
	}	

	sz.Format("%.0f", dTotalN);
	m_GridDim.AddCell(3, nColsu-3, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_GridDim.AddCell(4, nColsu-3, sz, nFormat, CELL_READONLY);
	m_GridDim.AddCell(3, nColsu-2, "-", nFormat, CELL_READONLY);
	dLengthRange = pDeckData->GetLengthRebarRangeAtJongPos(JONGBASEPOS_SLABCENTER, GetCurTabName()=="상면" ? TRUE : FALSE);
	sz.Format("%.3f", dLengthRange);
	m_GridDim.AddCell(4, nColsu-2, sz, nFormat, CELL_READONLY);
	m_GridDim.AddCell(3, nColsu-1, "-", nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL-dLengthRange);
	m_GridDim.AddCell(4, nColsu-1, sz, nFormat, CELL_READONLY);
	
	// 슬래브우측(치수길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrRighForDim->GetSize(); nCol++)
	{
		CDPoint *xyRightForDim = &pArrRighForDim->GetAt(nCol);
		m_GridDim.AddCell(5, nCol+3, &xyRightForDim->x);	m_GridDim.SetCellDec(CELL_DEC_0);
 		m_GridDim.AddCell(6, nCol+3, &xyRightForDim->y);	m_GridDim.SetCellDec(CELL_DEC_3);
		dTotalN += xyRightForDim->x;
		dTotalL += xyRightForDim->x * xyRightForDim->y;
	}	

	sz.Format("%.0f", dTotalN, nFormat, CELL_READONLY);
	m_GridDim.AddCell(5, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL, nFormat, CELL_READONLY);
	m_GridDim.AddCell(6, nColsu-1, sz, nFormat, CELL_READONLY);

	sz.Format("%.0f", dTotalN);
	m_GridDim.AddCell(5, nColsu-3, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_GridDim.AddCell(6, nColsu-3, sz, nFormat, CELL_READONLY);
	m_GridDim.AddCell(5, nColsu-2, "-", nFormat, CELL_READONLY);
	dLengthRange = pDeckData->GetLengthRebarRangeAtJongPos(JONGBASEPOS_SLABRIGHT, GetCurTabName()=="상면" ? TRUE : FALSE);
	sz.Format("%.3f", dLengthRange);
	m_GridDim.AddCell(6, nColsu-2, sz, nFormat, CELL_READONLY);
	m_GridDim.AddCell(5, nColsu-1, "-", nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL-dLengthRange);
	m_GridDim.AddCell(6, nColsu-1, sz, nFormat, CELL_READONLY);

	m_GridDim.AddCell(0, nColsu-3,"TOTAL");
	m_GridDim.AddCell(0, nColsu-2,"배근범위");
	m_GridDim.AddCell(0, nColsu-1,"오차");
}



void CDeckMainRebarCTCDlg::SetDataDefault() 
{
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB		= m_pStd->m_pDataManage->GetBridge();	

	BOOL bFirst = IsUsedFlag() ? FALSE : TRUE;

	if(bFirst)
	{
		pDeckData->m_dErrorRangeCTC = 0.01;

		pDeckData->m_dRangeDanbuStt = pDB->m_dLengthSlabGirderStt+pDB->m_dLengthGirderShoeStt+pDB->m_dLengthShoeToTaperStt+pDB->m_dLengthTaperToTaperStt;	// 단부보강철근 배치범위(슬래브 시점)	
		pDeckData->m_dRangeDanbuEnd = pDB->m_dLengthSlabGirderEnd+pDB->m_dLengthGirderShoeEnd+pDB->m_dLengthShoeToTaperEnd+pDB->m_dLengthTaperToTaperEnd;	// 단부보강철근 배치범위(슬래브 종점)	
		pDeckData->m_nJongCtcCalcPos	   = 2;
		pDeckData->m_nOptMainRebarArrange  = 0;
	}

	pDeckData->CalcCTCMainRebar();
	if(!pDeckData->IsSkew())
	{
		pDeckData->CalcCTCMainRebarAtSkew();
		pDeckData->CalcCTCSabogangRebar();
	}
	
	pDeckData->MakePlanMainRebar();

	m_pStd->m_pDoc->SetModifiedFlag();
}



void CDeckMainRebarCTCDlg::SetDataInit() 
{
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_Grid.SetDefaultCharacterCount(0);
	SetGridData();
	m_Grid.SetColumnWidthAll(55);
	m_Grid.UpdateData(FALSE);
	m_Grid.SetColumnWidthAll(55);
	m_Grid.SetColumnWidth(0, 70);
	m_Grid.SetColumnWidth(1, 90);
	long nColsu = m_Grid.GetColumnCount();
	m_Grid.SetColumnWidth(nColsu-3, 80);
	m_Grid.SetColumnWidth(nColsu-2, 80);
	m_Grid.SetRowHeightAll(17);

	m_Grid.MergeGrid(0, 6, 0, 2);
		
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);
	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.SetFocusCell(1, 3);

	// 
	SetGridDataDim();
	m_GridDim.UpdateData(FALSE);
	m_GridDim.SetColumnWidthAll(55);	
	m_GridDim.SetColumnWidth(0, 70);
	m_GridDim.SetColumnWidth(1, 90);
	nColsu = m_GridDim.GetColumnCount();
	m_GridDim.SetColumnWidth(nColsu-3, 80);
	m_GridDim.SetColumnWidth(nColsu-2, 80);

	m_GridDim.MergeGrid(0, 6, 0, 2);
		
	m_GridDim.EnableInsertRow(FALSE);
	m_GridDim.EnableInsertCol(TRUE);
	m_GridDim.SetRedraw(TRUE, TRUE);
	
	m_comboMainCTCBasePos.SetCurSel(pDeckData->m_nJongCtcCalcPos-1);		// 주철근 CTC기준위치
	m_comboMainCTCExtra.SetCurSel(pDeckData->m_nOptMainRebarArrange);	// 여분치수남기기 
	
	sText.Format("%g",	pDeckData->m_dErrorRangeCTC);

	m_dRangeError.SetWindowText(sText);	// 구간오차범위

	sText.Format("%g",	pDeckData->m_dRangeDanbuStt);

	m_dRangeDanbuStt.SetWindowText(sText);	// 단부보강철근 배치범위(슬래브 시점)	

	sText.Format("%g",	pDeckData->m_dRangeDanbuEnd);

	m_dRangeDanbuEnd.SetWindowText(sText);	// 단부보강철근 배치범위(슬래브 종점)	

	GetDlgItem(IDC_STATIC_MAINCTC)->SetWindowText("주철근 CTC 기준위치");
	GetDlgItem(IDC_EDIT_ERROR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CALC_REALTODIM)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_JONG_MAIN_CTC)->EnableWindow(TRUE);
	
	UpdateData(FALSE);
}


void CDeckMainRebarCTCDlg::SetDataSave() 
{
	m_Grid.UpdateData(TRUE);
	m_GridDim.UpdateData(TRUE);

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	m_dRangeError.GetWindowText(sText);

	pDeckData->m_dErrorRangeCTC  = atof(sText);		// 구간오차범위

	m_dRangeDanbuStt.GetWindowText(sText);

	pDeckData->m_dRangeDanbuStt  = atof(sText);	// 단부보강철근 배치범위(슬래브 시점)	

	m_dRangeDanbuEnd.GetWindowText(sText);

	pDeckData->m_dRangeDanbuEnd  = atof(sText);	// 단부보강철근 배치범위(슬래브 종점)	
	pDeckData->m_nJongCtcCalcPos = m_comboMainCTCBasePos.GetCurSel()+1;		// 주철근 CTC기준위치
	pDeckData->m_nOptMainRebarArrange = m_comboMainCTCExtra.GetCurSel();	// 여분치수남기기 
}



void CDeckMainRebarCTCDlg::InitTabCtrl()
{
	long nTab = 0;
	
	m_ctlTab.DeleteAllItems();
	m_ctlTab.InsertItem(nTab++, "상면");
	m_ctlTab.InsertItem(nTab++, "하면");
	m_ctlTab.SetCurSel(0);
}

BOOL CDeckMainRebarCTCDlg::PreTranslateMessage(MSG* pMsg) 
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

	if(pMsg->hwnd == m_GridDim.GetSafeHwnd() && pMsg->message == WM_RBUTTONDOWN)
	{		
		CCellID cellID = m_GridDim.GetFocusCell();				
		CString sText = m_GridDim.GetTextMatrix(0, cellID.col);
		if(sText=="TOTAL" || sText=="배근범위" || sText=="오차")		
			m_GridDim.EnableInsertCol(FALSE);					
		else		
			m_GridDim.EnableInsertCol(TRUE);					
	}	
	
	m_GridDim.ViewGridCellForCombo(pMsg);

	
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}




void CDeckMainRebarCTCDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = pGridView->iRow;

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	CString sText = m_Grid.GetTextMatrix(nRow, 1);

	SetDataSave();

	if(!m_bCheckAutoCTC)
	{
		SetDataInit();
		DrawInputDomyunView(FALSE);		
		return;
	}
	

	 if(sText.Find("좌측")!=-1)		// 슬래브 좌측기준
		pDeckData->CalcCTCMainRebarBySlabLeftRighCTCBySlabUpper(TRUE);
	else if(sText.Find("우측")!=-1)		// 슬래브 우측기준
		pDeckData->CalcCTCMainRebarBySlabLeftRighCTCBySlabUpper(FALSE);
	else if(sText.Find("중앙")!=-1)		// 슬래브 중앙기준
		pDeckData->CalcCTCMainRebarBySlabCentCTCBySlabUpper();
	SetDataInit();

	DrawInputDomyunView(FALSE);		
}



void CDeckMainRebarCTCDlg::OnCellChangedDataDim(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();

	DrawInputDomyunView(FALSE);		
}




void CDeckMainRebarCTCDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetFocus();	
	SetDataInit();

	UpdateData();

	m_Grid.SetFocusCell(1, 3);

	DrawInputDomyunView();
}

void CDeckMainRebarCTCDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();
}


CString CDeckMainRebarCTCDlg::GetCurTabName()
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
void CDeckMainRebarCTCDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	

	int nCol = pGridView->iColumn;

	InsertCTCByGridCol(nCol, FALSE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}



//삭제
void CDeckMainRebarCTCDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	

	int nCol = pGridView->iColumn;	

	DeleteCTCByGridCol(nCol, FALSE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}




//추가
void CDeckMainRebarCTCDlg::OnCellInsertedColDim(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	

	int nCol = pGridView->iColumn;

	InsertCTCByGridCol(nCol, TRUE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}



//삭제
void CDeckMainRebarCTCDlg::OnCellDeletedColDim(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;

	int nCol = pGridView->iColumn;	

	DeleteCTCByGridCol(nCol, TRUE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}


void CDeckMainRebarCTCDlg::InsertCTCByGridCol(long nGridCol, BOOL bDim)
{
	BOOL bUpper = GetCurTabName()=="상면" ? TRUE : FALSE;
	long nGugan = nGridCol-3;
	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	for (long nPos=0; nPos<3; nPos++)	// 슬래브 실길이 0좌측,1우측,2중앙 치수길이3좌,4우,5중앙
	{
		CDPointArray *pArr = NULL;

		if(bDim)
		{
			if(nPos==0)		pArr = &pDeckData->m_arrCTCMainRebarForDim[bUpper ? CTC_MAIN_UPPER_LEFT : CTC_MAIN_LOWER_LEFT];
			if(nPos==1)		pArr = &pDeckData->m_arrCTCMainRebarForDim[bUpper ? CTC_MAIN_UPPER_RIGHT : CTC_MAIN_LOWER_RIGHT];
			if(nPos==2)		pArr = &pDeckData->m_arrCTCMainRebarForDim[bUpper ? CTC_MAIN_UPPER_CENTER : CTC_MAIN_LOWER_CENTER];
		}
		else
		{
			if(nPos==0)		pArr = &pDeckData->m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_LEFT : CTC_MAIN_LOWER_LEFT];
			if(nPos==1)		pArr = &pDeckData->m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_RIGHT : CTC_MAIN_LOWER_RIGHT];
			if(nPos==2)		pArr = &pDeckData->m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_CENTER : CTC_MAIN_LOWER_CENTER];
		}

		long   nRebarsu = long(pArr->GetAt(nGugan).x);
		double dCTC     = pArr->GetAt(nGugan).y;

		pArr->GetAt(nGugan).x = long(nRebarsu/2);
		pArr->GetAt(nGugan).y = dCTC;
		pArr->InsertAt(nGugan, CDPoint(nRebarsu-long(nRebarsu/2), dCTC));
	}
}



void CDeckMainRebarCTCDlg::DeleteCTCByGridCol(long nGridCol, BOOL bDim)
{
	BOOL bUpper = GetCurTabName()=="상면" ? TRUE : FALSE;
	long nGugan = nGridCol-3;

	CADeckData *pDeckData = m_pStd->m_pDeckData;

	for (long nPos=0; nPos<3; nPos++)	// 슬래브 실길이 0좌측,1우측,2중앙 치수길이3좌,4우,5중앙
	{
		CDPointArray *pArr = NULL;
		if(bDim)
		{
			if(nPos==0)		pArr = &pDeckData->m_arrCTCMainRebarForDim[bUpper ? CTC_MAIN_UPPER_LEFT : CTC_MAIN_LOWER_LEFT];
			if(nPos==1)		pArr = &pDeckData->m_arrCTCMainRebarForDim[bUpper ? CTC_MAIN_UPPER_RIGHT : CTC_MAIN_LOWER_RIGHT];
			if(nPos==2)		pArr = &pDeckData->m_arrCTCMainRebarForDim[bUpper ? CTC_MAIN_UPPER_CENTER : CTC_MAIN_LOWER_CENTER];
		}
		else
		{
			if(nPos==0)		pArr = &pDeckData->m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_LEFT : CTC_MAIN_LOWER_LEFT];
			if(nPos==1)		pArr = &pDeckData->m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_RIGHT : CTC_MAIN_LOWER_RIGHT];
			if(nPos==2)		pArr = &pDeckData->m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_CENTER : CTC_MAIN_LOWER_CENTER];
		}

		if(pArr->GetSize()<=nGugan)		continue;
		
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
}



void CDeckMainRebarCTCDlg::OnButtonCalcRealToDim() 
{
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	UpdateData(TRUE);

	m_dRangeError.GetWindowText(sText);

	pDeckData->m_dErrorRangeCTC = atof(sText);
	pDeckData->CalcCTCMainRebarForDim();
	
	SetDataInit();
	SetDataSave();	

	DrawInputDomyunView(FALSE);		
}

void CDeckMainRebarCTCDlg::OnCheckShowMainRebarUpDn() 
{
	UpdateData(TRUE);

	DrawInputDomyunView(FALSE);	
	
}

void CDeckMainRebarCTCDlg::OnCheckShowMainRebarTotal() 
{
	UpdateData(TRUE);

	DrawInputDomyunView(FALSE);	
}


// 종단면주철근 CTC기준
// m_comboMainCTCBasePos.GetCurSel() = 0 : 도로 중심기준
// m_comboMainCTCBasePos.GetCurSel() = 1 : 슬래브 중심기준
// m_comboMainCTCBasePos.GetCurSel() = 2 : 곡률반경이 큰쪽
void CDeckMainRebarCTCDlg::OnSelchangeComboMainCTCBasePos() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	pDeckData->m_nJongCtcCalcPos = m_comboMainCTCBasePos.GetCurSel()+1;
	
	SetDataDefault();
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);
	
}

void CDeckMainRebarCTCDlg::OnSelchangeComboMainCTCExtraOption() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	pDeckData->m_nOptMainRebarArrange = m_comboMainCTCExtra.GetCurSel();

	SetDataDefault();
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);
}



void CDeckMainRebarCTCDlg::OnRadioOneUpDnView() 
{
	UpdateData( );

	DrawInputDomyunView();
	
}

void CDeckMainRebarCTCDlg::OnRadioTwoView() 
{
	UpdateData();

	DrawInputDomyunView();
	
}

void CDeckMainRebarCTCDlg::OnButtonSameAll() 
{
	CString	sText = GetCurTabName();

	CDPointArray *pArrLeftReal   = GetCTCRebar(POS_SLAB_LEFT, TRUE, sText);
	CDPointArray *pArrRighReal   = GetCTCRebar(POS_SLAB_RIGH, TRUE, sText);
	CDPointArray *pArrCentReal   = GetCTCRebar(POS_SLAB_CENTER, TRUE, sText);
	CDPointArray *pArrLeftForDim = GetCTCRebar(POS_SLAB_LEFT, FALSE, sText);
	CDPointArray *pArrRighForDim = GetCTCRebar(POS_SLAB_RIGH, FALSE, sText);
	CDPointArray *pArrCentForDim = GetCTCRebar(POS_SLAB_CENTER, FALSE, sText);

	SetCTCRebar(*pArrLeftReal, POS_SLAB_LEFT, TRUE, sText=="상면"?"하면":"상면");
	SetCTCRebar(*pArrRighReal, POS_SLAB_RIGH, TRUE, sText=="상면"?"하면":"상면");
	SetCTCRebar(*pArrCentReal, POS_SLAB_CENTER, TRUE, sText=="상면"?"하면":"상면");
	SetCTCRebar(*pArrLeftForDim, POS_SLAB_LEFT, FALSE, sText=="상면"?"하면":"상면");
	SetCTCRebar(*pArrRighForDim, POS_SLAB_RIGH, FALSE, sText=="상면"?"하면":"상면");
	SetCTCRebar(*pArrCentForDim, POS_SLAB_CENTER, FALSE, sText=="상면"?"하면":"상면");
	
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE); 		
}

void CDeckMainRebarCTCDlg::OnKillfocusEditSlabStt() 
{
	UpdateData(TRUE);
	
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dRangeDanbuStt.GetWindowText(sText);

	pDeckData->m_dRangeDanbuStt = atof(sText);
}

void CDeckMainRebarCTCDlg::OnKillfocusEditSlabEnd() 
{
	UpdateData(TRUE);

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dRangeDanbuEnd.GetWindowText(sText);

	pDeckData->m_dRangeDanbuEnd = atof(sText);
}

void CDeckMainRebarCTCDlg::OnKillfocusEditError() 
{
	UpdateData(TRUE);

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dRangeError.GetWindowText(sText);

	pDeckData->m_dErrorRangeCTC = atof(sText);
}

void CDeckMainRebarCTCDlg::OnCheckBox() 
{
	UpdateData(TRUE);

	DrawInputDomyunView();			
}


void CDeckMainRebarCTCDlg::OnSize(UINT nType, int cx, int cy) 
{
	CGeneralBaseDlg::OnSize(nType, cx, cy);
	
}

void CDeckMainRebarCTCDlg::OnButtonApply() 
{
	UpdateData();
	SetDataSave();
	SetDataInit();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakePlanMainRebar();

	DrawInputDomyunView();
}

long CDeckMainRebarCTCDlg::GetTabCount()
{
	return 2;
}



CString CDeckMainRebarCTCDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0)			return "평면";
	else				return "종단면";
}
