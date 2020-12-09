// DeckMainSkewRebarCTCDlg.cpp : implementation file
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
// CDeckMainSkewRebarCTCDlg dialog
CDeckMainSkewRebarCTCDlg::CDeckMainSkewRebarCTCDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckMainSkewRebarCTCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckMainSkewRebarCTCDlg)
	m_bShowTotal = FALSE;
	m_bCheckBox = FALSE;
	m_nPlanViewType = 0;
	//}}AFX_DATA_INIT
}


void CDeckMainSkewRebarCTCDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckMainSkewRebarCTCDlg)
	DDX_Control(pDX, IDC_EDIT_ERROR, m_dRangeError);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_SHOW_MAINREBAR_TOTAL, m_bShowTotal);
	DDX_Check(pDX, IDC_CHECK_BEAM, m_bCheckBox);
	DDX_Radio(pDX, IDC_RADIO_ONEBODY, m_nPlanViewType);
	DDX_Control(pDX, IDC_EDIT_RANGE_SKEW, m_sRangeSkewRebarStt);
	DDX_Control(pDX, IDC_EDIT_RANGE_SKEW2, m_sRangeSkewRebarEnd);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_CALC_REALTODIM, m_btnError);
	DDX_Control(pDX, IDC_BUTTON_SAME_ALL, m_btnSameAll);
	DDX_Control(pDX, IDC_BUTTON_ADD_GUGAN, m_btnAdd);
}


BEGIN_MESSAGE_MAP(CDeckMainSkewRebarCTCDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckMainSkewRebarCTCDlg)
	ON_BN_CLICKED(IDC_BUTTON_CALC_REALTODIM, OnButtonCalcRealToDim)
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
	ON_EN_KILLFOCUS(IDC_EDIT_RANGE_SKEW, OnKillfocusEditRangeSkew)
	ON_EN_KILLFOCUS(IDC_EDIT_RANGE_SKEW2, OnKillfocusEditRangeSkewEnd)
	ON_EN_KILLFOCUS(IDC_EDIT_ERROR, OnKillfocusEditError)	
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckMainSkewRebarCTCDlg message handlers
void CDeckMainSkewRebarCTCDlg::OnPreInitDialog()
{
	InitTabCtrl();

	SetResize(IDC_GRID,							SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB,							SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_VIEW_OPTION,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SHOW_MAINREBAR_TOTAL,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_BEAM,					SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC,						SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_ONEBODY,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_TWOBODY,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAME_ALL,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

}

void CDeckMainSkewRebarCTCDlg::DrawInputDomyunView(BOOL bZoomAll)
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
void CDeckMainSkewRebarCTCDlg::DrawPlanSlab(CDomyun *pDom, long nType)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);

	Dom.SetDirection("TOP");

	CPlateBxFinder Finder(pDB->GetGirder(-1));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_JOINT);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_JOINT);

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



void CDeckMainSkewRebarCTCDlg::DrawPlanRebar(CDomyun *pDom)
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
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, TRUE, DRAW_LEFTSIDE, TRUE, TRUE, FALSE);
			if(sText.Find("상면")!=-1)	Dom.SetLineColor(3);
			else						Dom.SetLineColor(2);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, FALSE, DRAW_RIGHTSIDE, TRUE, TRUE, FALSE);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER | DIM_DECK_TOTAL_LEFT, TRUE, FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER | DIM_DECK_TOTAL_RIGHT, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "사각부주철근(상/하면)", TRUE, TRUE, 0);

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
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, TRUE, DRAW_SIDEALL, TRUE, FALSE, FALSE);
			if(sText=="상면(종점)")	Dom.SetLineColor(2);
			else					Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, TRUE, DRAW_SIDEALL, FALSE, TRUE, FALSE);
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_UPPER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "사각부주철근(상면)", TRUE, TRUE, 0);

			Dom.Move(0, Dom.GetExtHeight()*0.6);
			*pDom << Dom;

			//////////////////////////////////////////////////////////////////////////
			// 하면					
			DrawPlanSlab(&Dom, 1);

			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);
			if(sText=="하면(시점)")	Dom.SetLineColor(2);
			else					Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, FALSE, DRAW_SIDEALL, TRUE, FALSE, FALSE);
			if(sText=="하면(종점)")	Dom.SetLineColor(2);
			else					Dom.SetLineColor(3);
			DBDeckPlan.DrawMainRebarAtSkew(&Dom, FALSE, DRAW_SIDEALL, FALSE, TRUE, FALSE);
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, TRUE,FALSE,"",nSttDan);
			DBDimDeckPlan.DimPyungTotal(&Dom,pBxStt, pBxEnd,DIM_DECK_MAINREBAR_LOWER, FALSE,FALSE,"",nSttDan);

			if(bRotate)		Dom.Rotate(rotAng, 0, 0);

			Dom.SetCalcExtRect();
			DrawStd.AddTitleOnDom(&Dom, Dom.GetScaleDim(), Dom.GetExtRect(), "사각부주철근(하면)", TRUE, TRUE, 0);

			Dom.Move(0, -Dom.GetExtHeight()*0.6);
			*pDom << Dom;
		}
	}
	*pDom << Dom;
}



void CDeckMainSkewRebarCTCDlg::DrawJongRebar(CDomyun *pDom)
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
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브좌측단", TRUE, FALSE);

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
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브중심", TRUE, FALSE);
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
	DrawStd.AddTitleOnDom(&Dom, 40, rect, "슬래브우측단", TRUE, FALSE);
	rect = Dom.GetExtRect();
	Dom.Move(0, -rect.Height()*2.5);

	*pDom << Dom;
}

CDPointArray* CDeckMainSkewRebarCTCDlg::GetCTCRebar(long nPosSlab, BOOL bRealCTC, CString szCTC) 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	CString sText = szCTC=="" ? GetCurTabName() : szCTC;
	
	long nCTCType = 0;
	if(sText=="상면(시점)")	// 시상
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_UPPER_LEFT_STT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_UPPER_RIGH_STT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_UPPER_CENTER_STT;

		return bRealCTC ? &pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] : &pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType];
	}
	else if(sText=="하면(시점)")	// 시하
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_LOWER_LEFT_STT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_LOWER_RIGH_STT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_LOWER_CENTER_STT;

		return bRealCTC ? &pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] : &pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType];
	}
	else if(sText=="상면(종점)")	// 종상
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_UPPER_LEFT_END;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_UPPER_RIGH_END;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_UPPER_CENTER_END;

		return bRealCTC ? &pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] : &pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType];
	}
	else if(sText== "하면(종점)")	// 종하
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_LOWER_LEFT_END;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_LOWER_RIGH_END;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_LOWER_CENTER_END;

		return bRealCTC ? &pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] : &pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType];
	}

	return NULL;
	
}


// 사보강철근과 신축이음부 하부 철근은 좌,우측 구분이 없음.
void CDeckMainSkewRebarCTCDlg::SetCTCRebar(CDPointArray &xyArrCTC, long nPosSlab, BOOL bRealCTC, CString szCTC)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sText = szCTC=="" ? GetCurTabName() : szCTC;

	long nCTCType = 0;

	if(sText=="상면(시점)")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_UPPER_LEFT_STT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_UPPER_RIGH_STT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_UPPER_CENTER_STT;

		if(bRealCTC)
		{
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] = xyArrCTC;
		}
		else
		{
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType] = xyArrCTC;
		}
	}
	else if(sText=="하면(시점)")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_LOWER_LEFT_STT;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_LOWER_RIGH_STT;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_LOWER_CENTER_STT;

		if(bRealCTC)
		{
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] = xyArrCTC;
		}
		else
		{
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType] = xyArrCTC;
		}
	}
	else if(sText== "상면(종점)")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_UPPER_LEFT_END;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_UPPER_RIGH_END;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_UPPER_CENTER_END;

		if(bRealCTC)
		{
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] = xyArrCTC;
		}
		else
		{
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType] = xyArrCTC;
		}
	}
	else if(sText=="하면(종점)")
	{
		if(nPosSlab==POS_SLAB_LEFT)				nCTCType = CTC_SKEWMAIN_LOWER_LEFT_END;
		else if(nPosSlab==POS_SLAB_RIGH)		nCTCType = CTC_SKEWMAIN_LOWER_RIGH_END;
		else if(nPosSlab==POS_SLAB_CENTER)		nCTCType = CTC_SKEWMAIN_LOWER_CENTER_END;

		if(bRealCTC)
		{
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkew[nCTCType] = xyArrCTC;
		}
		else
		{
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType].RemoveAll();
			pDeckData->m_arrCTCMainRebarAtSkewForDim[nCTCType] = xyArrCTC;
		}
	}

}


void CDeckMainSkewRebarCTCDlg::SetGridData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	if(pDeckData->IsSkew()==FALSE)
		return ;
	
	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	
	long nRowsu = 13;
	CDPointArray *pArrLeftReal   = GetCTCRebar(POS_SLAB_LEFT, TRUE);
	CDPointArray *pArrRighReal   = GetCTCRebar(POS_SLAB_RIGH, TRUE);
	CDPointArray *pArrCentReal   = GetCTCRebar(POS_SLAB_CENTER, TRUE);
	CDPointArray *pArrLeftForDim = GetCTCRebar(POS_SLAB_LEFT, FALSE);
	CDPointArray *pArrRighForDim = GetCTCRebar(POS_SLAB_RIGH, FALSE);
	CDPointArray *pArrCentForDim = GetCTCRebar(POS_SLAB_CENTER, FALSE);
	
	if(pArrLeftReal==NULL)		return;
	
	long nCTCLeftsu = pArrLeftReal ? pArrLeftReal->GetSize() : 0;
	long nCTCRighsu = pArrRighReal ? pArrRighReal->GetSize() : 0;
	long nColsu = max(max(nCTCLeftsu+4, nCTCRighsu+4), 4);

	m_Grid.SetGridDefault(nRowsu, nColsu, 1, 3);
	
	m_Grid.AddCell(0, 0, "구분");
	long nRow = 0;
	for (nRow=1; nRow<=6; nRow++)
		m_Grid.AddCell(nRow, 0, "실제길이\n");
	for (nRow=7; nRow<=12; nRow++)
		m_Grid.AddCell(nRow, 0, "도면치수\n");
	
	m_Grid.AddCell(0, 1, "위치");			m_Grid.AddCell(0, 2, "구분");
	m_Grid.AddCell(1, 1, "슬래브좌측\n");		m_Grid.AddCell(1, 2, "개수");
	m_Grid.AddCell(2, 1, "슬래브좌측\n");		m_Grid.AddCell(2, 2, "간격");
	m_Grid.AddCell(3, 1, "슬래브우측\n");		m_Grid.AddCell(3, 2, "개수");
	m_Grid.AddCell(4, 1, "슬래브우측\n");		m_Grid.AddCell(4, 2, "간격");
	m_Grid.AddCell(5, 1, "슬래브중앙\n");		m_Grid.AddCell(5, 2, "개수");
	m_Grid.AddCell(6, 1, "슬래브중앙\n");		m_Grid.AddCell(6, 2, "간격");
	m_Grid.AddCell(7, 1, "슬래브좌측\n");		m_Grid.AddCell(7, 2, "개수");
	m_Grid.AddCell(8, 1, "슬래브좌측\n");		m_Grid.AddCell(8, 2, "간격");
	m_Grid.AddCell(9, 1, "슬래브우측\n");		m_Grid.AddCell(9, 2, "개수");
	m_Grid.AddCell(10,1, "슬래브우측\n");		m_Grid.AddCell(10,2, "간격");
	m_Grid.AddCell(11,1, "슬래브중앙\n");		m_Grid.AddCell(11,2, "개수");
	m_Grid.AddCell(12,1, "슬래브중앙\n");		m_Grid.AddCell(12,2, "간격");
	long nCol = 0;
	for(nCol=3; nCol<nColsu; nCol++)
	{
		sz.Format("%d구간", nCol-2);
		m_Grid.AddCell(0, nCol, sz);
	}


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

	sz.Format("%.0f", dTotalN);
	m_Grid.AddCell(1, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_Grid.AddCell(2, nColsu-1, sz, nFormat, CELL_READONLY);
	
	// 슬래브우측(실제길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrRighReal->GetSize(); nCol++)
	{
		CDPoint *xyRighReal = &pArrRighReal->GetAt(nCol);
		m_Grid.AddCell(3, nCol+3, &xyRighReal->x);	m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(4, nCol+3, &xyRighReal->y);	m_Grid.SetCellDec(CELL_DEC_3);
		dTotalN += xyRighReal->x;
		dTotalL += xyRighReal->x * xyRighReal->y;
	}	

	sz.Format("%.0f", dTotalN);
	m_Grid.AddCell(3, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_Grid.AddCell(4, nColsu-1, sz, nFormat, CELL_READONLY);
	// 슬래브 중앙(실제길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrCentReal->GetSize(); nCol++)
	{
		CDPoint *xyCentReal = &pArrCentReal->GetAt(nCol);
		m_Grid.AddCell(5, nCol+3, &xyCentReal->x);		m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(6, nCol+3, &xyCentReal->y);		m_Grid.SetCellDec(CELL_DEC_3);
		dTotalN += xyCentReal->x;
		dTotalL += xyCentReal->x * xyCentReal->y;
	}	

	sz.Format("%.0f", dTotalN);
	m_Grid.AddCell(5, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.3f", dTotalL);
	m_Grid.AddCell(6, nColsu-1, sz, nFormat, CELL_READONLY);

	// 슬래브좌측(치수길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrLeftForDim->GetSize(); nCol++)
	{
		CDPoint *xyLeftForDim = &pArrLeftForDim->GetAt(nCol);
		m_Grid.AddCell(7, nCol+3, &xyLeftForDim->x);	m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(8, nCol+3, &xyLeftForDim->y);	m_Grid.SetCellDec(CELL_DEC_0);
		dTotalN += xyLeftForDim->x;
		dTotalL += xyLeftForDim->x * xyLeftForDim->y;
	}	

	sz.Format("%.0f", dTotalN, nFormat, CELL_READONLY);
	m_Grid.AddCell(7, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.0f", dTotalL, nFormat, CELL_READONLY);
	m_Grid.AddCell(8, nColsu-1, sz, nFormat, CELL_READONLY);

	// 슬래브우측(치수길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrRighForDim->GetSize(); nCol++)
	{
		CDPoint *xyRightForDim = &pArrRighForDim->GetAt(nCol);
		m_Grid.AddCell(9, nCol+3, &xyRightForDim->x);		m_Grid.SetCellDec(CELL_DEC_0);
 		m_Grid.AddCell(10, nCol+3, &xyRightForDim->y);		m_Grid.SetCellDec(CELL_DEC_0);
		dTotalN += xyRightForDim->x;
		dTotalL += xyRightForDim->x * xyRightForDim->y;
	}	

	sz.Format("%.0f", dTotalN, nFormat, CELL_READONLY);
	m_Grid.AddCell(9, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.0f", dTotalL, nFormat, CELL_READONLY);
	m_Grid.AddCell(10, nColsu-1, sz, nFormat, CELL_READONLY);

	// 슬래브중앙(치수길이)
	dTotalN = 0;
	dTotalL = 0;
	for(nCol=0; nCol<pArrCentForDim->GetSize(); nCol++)
	{
		CDPoint *xyCentForDim = &pArrCentForDim->GetAt(nCol);
		m_Grid.AddCell(11, nCol+3, &xyCentForDim->x);		m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(12, nCol+3, &xyCentForDim->y);		m_Grid.SetCellDec(CELL_DEC_0);
		dTotalN += xyCentForDim->x;
		dTotalL += xyCentForDim->x * xyCentForDim->y;
	}	

	sz.Format("%.0f", dTotalN, nFormat, CELL_READONLY);
	m_Grid.AddCell(11, nColsu-1, sz, nFormat, CELL_READONLY);
	sz.Format("%.0f", dTotalL, nFormat, CELL_READONLY);
	m_Grid.AddCell(12, nColsu-1, sz, nFormat, CELL_READONLY);

	m_Grid.AddCell(0, nColsu-1,"TOTAL");

}



void CDeckMainSkewRebarCTCDlg::SetDataDefault() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	BOOL bFirst = IsUsedFlag() ? FALSE : TRUE;
	if(bFirst)
	{
		pDeckData->m_dErrorRangeCTC = 0.01;
		SetDataDefaultRange();
	}
	
	if(pDeckData->IsSkewStt() || pDeckData->IsSkewEnd())
		pDeckData->CalcCTCMainRebarAtSkew();

	pDeckData->MakePlanMainRebarSkew();

	m_pStd->m_pDoc->SetModifiedFlag();
}




void CDeckMainSkewRebarCTCDlg::SetDataDefaultRange()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CPlateGirderApp *pGir	  = pDB->GetGirder(-1);

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
	
	pDeckData->m_dRangeMainRebarAtSkewStt = 0;
	pDeckData->m_dRangeMainRebarAtSkewEnd = 0;

	if(bSkewStt)
		pDeckData->m_dRangeMainRebarAtSkewStt = dLengthSkewCenStt/2;
	else
		pDeckData->m_dRangeMainRebarAtSkewStt = 0;

	if(bSkewEnd)
		pDeckData->m_dRangeMainRebarAtSkewEnd = dLengthSkewCenEnd/2;
	else
		pDeckData->m_dRangeMainRebarAtSkewEnd = 0;

}



void CDeckMainSkewRebarCTCDlg::SetDataInit() 
{
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	SetGridData();

	if(pDeckData->IsSkew()==FALSE)
	{
		m_Grid.DeleteAllItems();
		return ;
	}

	m_Grid.UpdateData(FALSE);
	m_Grid.MergeGrid(0, 12, 0, 2);
	m_Grid.SetColumnWidthAll(55);	
	m_Grid.SetColumnWidth(0, 70);
	m_Grid.SetColumnWidth(1, 90);
	long nColsu = m_Grid.GetColumnCount();
	
	m_Grid.SetColumnWidth(nColsu-1, 80);
	
	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);


	sText.Format("%g",	pDeckData->m_dErrorRangeCTC);

	m_dRangeError.SetWindowText(sText);	// 구간오차범위

	if(pDeckData->IsBridgeHasCurveLine())
	{
		GetDlgItem(IDC_EDIT_ERROR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CALC_REALTODIM)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_ERROR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CALC_REALTODIM)->EnableWindow(FALSE);
	}

	CDPointArray *pArrLeftReal= GetCTCRebar(POS_SLAB_LEFT, TRUE);
	if(pArrLeftReal->GetSize()==0)
		GetDlgItem(IDC_BUTTON_ADD_GUGAN)->ShowWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_ADD_GUGAN)->ShowWindow(FALSE);

	sText.Format("%.0lf", pDeckData->m_dRangeMainRebarAtSkewStt);

	m_sRangeSkewRebarStt.SetWindowText(sText);

	sText.Format("%.0lf", pDeckData->m_dRangeMainRebarAtSkewEnd);

	m_sRangeSkewRebarEnd.SetWindowText(sText);
	
	UpdateData(FALSE);
}




void CDeckMainSkewRebarCTCDlg::SetDataSave() 
{
	m_Grid.UpdateData(TRUE);

	CString		sText		= _T("");
	CADeckData *pDeckData	= m_pStd->m_pDeckData;
	
	m_dRangeError.GetWindowText(sText);

	pDeckData->m_dErrorRangeCTC  = atof(sText);		// 구간오차범위

	m_sRangeSkewRebarStt.GetWindowText(sText);

	pDeckData->m_dRangeMainRebarAtSkewStt = atof(sText);

	m_sRangeSkewRebarEnd.GetWindowText(sText);

	pDeckData->m_dRangeMainRebarAtSkewEnd = atof(sText);
}



void CDeckMainSkewRebarCTCDlg::InitTabCtrl()
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

BOOL CDeckMainSkewRebarCTCDlg::PreTranslateMessage(MSG* pMsg) 
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

void CDeckMainSkewRebarCTCDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();

	DrawInputDomyunView(FALSE);
}

void CDeckMainSkewRebarCTCDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetFocus();	
	SetDataInit();

	UpdateData();

	m_Grid.SetFocusCell(1, 3);	DrawInputDomyunView();
	
	DrawInputDomyunView();
}

void CDeckMainSkewRebarCTCDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();
}


CString CDeckMainSkewRebarCTCDlg::GetCurTabName()
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
void CDeckMainSkewRebarCTCDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
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
void CDeckMainSkewRebarCTCDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	

	int nCol = pGridView->iColumn;	

	DeleteCTCByGridCol(nCol);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

	return;
}




long CDeckMainSkewRebarCTCDlg::GetTabCount()
{
	return 2;
}



CString CDeckMainSkewRebarCTCDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0)			return "평면";
	else				return "종단면";
}



void CDeckMainSkewRebarCTCDlg::InsertCTCByGridCol(long nGridCol)
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

	for (long nPos=0; nPos<6; nPos++)	// 슬래브 실길이 0좌측,1우측,2중앙 치수길이3좌,4우,5중앙
	{
		CDPointArray *pArr = NULL;
		if(nPos==0)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_LEFT_END) : (bStt?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_END)];
		if(nPos==1)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_UPPER_RIGH_END) : (bStt?CTC_SKEWMAIN_LOWER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_END)];
		if(nPos==2)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_UPPER_CENTER_END) : (bStt?CTC_SKEWMAIN_LOWER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_END)];
		if(nPos==3)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_LEFT_END) : (bStt?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_END)];
		if(nPos==4)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_UPPER_RIGH_END) : (bStt?CTC_SKEWMAIN_LOWER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_END)];
		if(nPos==5)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_UPPER_CENTER_END) : (bStt?CTC_SKEWMAIN_LOWER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_END)];

		long   nRebarsu = long(pArr->GetAt(nGugan).x);
		double dCTC     = pArr->GetAt(nGugan).y;

		pArr->GetAt(nGugan).x = long(nRebarsu/2);
		pArr->GetAt(nGugan).y = dCTC;
		pArr->InsertAt(nGugan, CDPoint(nRebarsu-long(nRebarsu/2), dCTC));
	}
}



void CDeckMainSkewRebarCTCDlg::DeleteCTCByGridCol(long nGridCol)
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

	for (long nPos=0; nPos<6; nPos++)	// 슬래브 실길이 0좌측,1우측,2중앙 치수길이3좌,4우,5중앙
	{
		CDPointArray *pArr = NULL;
		if(nPos==0)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_LEFT_END) : (bStt?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_END)];
		if(nPos==1)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_UPPER_RIGH_END) : (bStt?CTC_SKEWMAIN_LOWER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_END)];
		if(nPos==2)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_UPPER_CENTER_END) : (bStt?CTC_SKEWMAIN_LOWER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_END)];
		if(nPos==3)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_LEFT_END) : (bStt?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_END)];
		if(nPos==4)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_UPPER_RIGH_END) : (bStt?CTC_SKEWMAIN_LOWER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_END)];
		if(nPos==5)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_UPPER_CENTER_END) : (bStt?CTC_SKEWMAIN_LOWER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_END)];

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



void CDeckMainSkewRebarCTCDlg::OnButtonCalcRealToDim() 
{
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;
	
	UpdateData(TRUE);

	m_dRangeError.GetWindowText(sText);

	pDeckData->m_dErrorRangeCTC = atof(sText);
	pDeckData->CalcCTCMainRebarAtSkewForDim();
	
	SetDataInit();
	SetDataSave();	

	DrawInputDomyunView(FALSE);		
}


void CDeckMainSkewRebarCTCDlg::OnCheckShowMainRebarUpDn() 
{
	UpdateData(TRUE);

	DrawInputDomyunView(FALSE);	
	
}

void CDeckMainSkewRebarCTCDlg::OnCheckShowMainRebarTotal() 
{
	UpdateData(TRUE);

	DrawInputDomyunView(FALSE);	
}




void CDeckMainSkewRebarCTCDlg::OnRadioOneUpDnView() 
{
	UpdateData( );

	DrawInputDomyunView();
	
}

void CDeckMainSkewRebarCTCDlg::OnRadioTwoView() 
{
	UpdateData();

	DrawInputDomyunView();
	
}


void CDeckMainSkewRebarCTCDlg::OnButtonSameAll() 
{
	CString	sText = GetCurTabName();

	CDPointArray *pArrLeftReal   = GetCTCRebar(POS_SLAB_LEFT, TRUE, sText);
	CDPointArray *pArrRighReal   = GetCTCRebar(POS_SLAB_RIGH, TRUE, sText);
	CDPointArray *pArrCentReal   = GetCTCRebar(POS_SLAB_CENTER, TRUE, sText);
	CDPointArray *pArrLeftForDim = GetCTCRebar(POS_SLAB_LEFT, FALSE, sText);
	CDPointArray *pArrRighForDim = GetCTCRebar(POS_SLAB_RIGH, FALSE, sText);
	CDPointArray *pArrCentForDim = GetCTCRebar(POS_SLAB_CENTER, FALSE, sText);

	for (long n=0; n<4; n++)
	{
		CString szCur = "";
		if(n==0)		szCur = "상면(시점)";
		else if(n==1)	szCur = "하면(시점)";
		else if(n==2)	szCur = "상면(종점)";
		else if(n==3)	szCur = "하면(종점)";

		if(szCur==sText)	continue;

		SetCTCRebar(*pArrLeftReal, POS_SLAB_LEFT, TRUE, szCur);
		SetCTCRebar(*pArrRighReal, POS_SLAB_RIGH, TRUE, szCur);
		SetCTCRebar(*pArrCentReal, POS_SLAB_CENTER, TRUE, szCur);
		SetCTCRebar(*pArrLeftForDim, POS_SLAB_LEFT, TRUE, szCur);
		SetCTCRebar(*pArrRighForDim, POS_SLAB_RIGH, TRUE, szCur);
		SetCTCRebar(*pArrCentForDim, POS_SLAB_CENTER, TRUE, szCur);
	}

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE); 	
}


BOOL CDeckMainSkewRebarCTCDlg::IsValid() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	if(pDeckData->IsSkew())
		return TRUE;
	else
		return FALSE;

}

void CDeckMainSkewRebarCTCDlg::OnKillfocusEditError() 
{
	UpdateData(TRUE);

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_dRangeError.GetWindowText(sText);

	pDeckData->m_dErrorRangeCTC = atof(sText);	
}

void CDeckMainSkewRebarCTCDlg::OnCheckBox() 
{
	UpdateData(TRUE);

	DrawInputDomyunView();			
}


void CDeckMainSkewRebarCTCDlg::OnButtonApply() 
{
	UpdateData();
	SetDataSave();
	SetDataInit();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakePlanMainRebarSkew();

	DrawInputDomyunView();
}

void CDeckMainSkewRebarCTCDlg::OnButtonPrev() 
{
	UpdateData();
	SetDataSave();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakePlanMainRebarSkew();
	
	m_pView->GetDomyun()->ClearEtt(TRUE);	
	m_pStd->MovePrevDialog();	
}

void CDeckMainSkewRebarCTCDlg::OnButtonNext() 
{
	UpdateData();
	SetDataSave();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakePlanMainRebarSkew();
	
	m_pView->GetDomyun()->ClearEtt(TRUE);	
	m_pStd->MoveNextDialog();	
}


void CDeckMainSkewRebarCTCDlg::OnButtonAddGugan() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	CString sTab = GetCurTabName();
	BOOL bUpper = FALSE;
	BOOL bStt = FALSE;

	if(sTab=="상면(시점)")	{ bUpper = TRUE;	bStt = TRUE;}
	if(sTab=="하면(시점)")	{ bUpper = FALSE;	bStt = TRUE;}
	if(sTab=="상면(종점)")	{ bUpper = TRUE;	bStt = FALSE;}
	if(sTab=="하면(종점)")	{ bUpper = FALSE;	bStt = FALSE;}

	for (long nPos=0; nPos<6; nPos++)	// 슬래브 실길이 0좌측,1우측,2중앙 치수길이3좌,4우,5중앙
	{
		CDPointArray *pArr = NULL;
		if(nPos==0)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_LEFT_END) : (bStt?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_END)];
		if(nPos==1)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_UPPER_RIGH_END) : (bStt?CTC_SKEWMAIN_LOWER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_END)];
		if(nPos==2)		pArr = &pDeckData->m_arrCTCMainRebarAtSkew[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_UPPER_CENTER_END) : (bStt?CTC_SKEWMAIN_LOWER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_END)];
		if(nPos==3)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_UPPER_LEFT_END) : (bStt?CTC_SKEWMAIN_LOWER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_END)];
		if(nPos==4)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_UPPER_RIGH_END) : (bStt?CTC_SKEWMAIN_LOWER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_END)];
		if(nPos==5)		pArr = &pDeckData->m_arrCTCMainRebarAtSkewForDim[bUpper ? (bStt?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_UPPER_CENTER_END) : (bStt?CTC_SKEWMAIN_LOWER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_END)];

		pArr->RemoveAll();
		pArr->Add(CDPoint(1,125));
	}	

	SetDataInit();
}

void CDeckMainSkewRebarCTCDlg::OnKillfocusEditRangeSkew() 
{
	UpdateData(TRUE);

	SetDataSave();
	DrawInputDomyunView();
}

void CDeckMainSkewRebarCTCDlg::OnKillfocusEditRangeSkewEnd() 
{
	UpdateData(TRUE);

	SetDataSave();
	DrawInputDomyunView();
}
