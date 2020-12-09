// DeckPlanRebarDlg.cpp : implementation file
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
// CDeckPlanRebarDlg dialog


CDeckPlanRebarDlg::CDeckPlanRebarDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckPlanRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckPlanRebarDlg)
	m_bShowSlabHunch = FALSE;
	m_nCurSangsePos = 0;
	//}}AFX_DATA_INIT
}


void CDeckPlanRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckPlanRebarDlg)
	DDX_Control(pDX, IDC_EDIT_TRIMPER, m_dTrimRatio);
	DDX_Check(pDX, IDC_CHECK_SHOW_SLABHUNCH, m_bShowSlabHunch);
	DDX_Control(pDX, IDC_EDIT_CB, m_szDiaCB);
	DDX_Control(pDX, IDC_EDIT_CANT, m_szDiaCant);
	DDX_Control(pDX, IDC_EDIT_DAN, m_szDiaDan);
	DDX_Control(pDX, IDC_EDIT_OUTBK, m_szDiaOutBk);
	DDX_Control(pDX, IDC_EDIT_ST, m_szDiaStringer);
 	DDX_Radio(pDX, IDC_RADIO_CB, m_nCurSangsePos);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_RADIO_ST, m_btnVerGirder);
	DDX_Control(pDX, IDC_RADIO_OUTBK, m_btnSideGirder);
// 	DDX_Control(pDX, IDC_RADIO_CB, m_btnHorGirder);
	DDX_Control(pDX, IDC_RADIO_DAN, m_btnDanbu);
	DDX_Control(pDX, IDC_RADIO_CANT, m_btnCantil);
}

BEGIN_MESSAGE_MAP(CDeckPlanRebarDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckPlanRebarDlg)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SLABHUNCH, OnCheckShowSlabhunch)
	ON_BN_CLICKED(IDC_RADIO_CANT, OnRadioCant)
	ON_BN_CLICKED(IDC_RADIO_CB, OnRadioCB)
	ON_BN_CLICKED(IDC_RADIO_DAN, OnRadioDan)
	ON_BN_CLICKED(IDC_RADIO_OUTBK, OnRadioOutBk)
	ON_BN_CLICKED(IDC_RADIO_ST, OnRadioSt)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	ON_EN_KILLFOCUS(IDC_EDIT_CB, OnKillfocusEditCB)
	ON_EN_KILLFOCUS(IDC_EDIT_CANT, OnKillfocusEditCant)
	ON_EN_KILLFOCUS(IDC_EDIT_DAN, OnKillfocusEditDan)
	ON_EN_KILLFOCUS(IDC_EDIT_OUTBK, OnKillfocusEditOutbk)
	ON_EN_KILLFOCUS(IDC_EDIT_ST, OnKillfocusEditSt)
	ON_EN_KILLFOCUS(IDC_EDIT_TRIMPER, OnKillfocusEditTrim)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckPlanRebarDlg message handlers
BOOL CDeckPlanRebarDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}


void CDeckPlanRebarDlg::OnPreInitDialog()
{
	m_nTab=0;

	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();
	if(pDB->IsTUGir())
		GetDlgItem(IDC_RADIO_DAN)->EnableWindow(FALSE);

	SetResize(IDC_STATIC_GROUP, SZ_TOP_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_RADIO_CB, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_DAN, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_CANT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_CB, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_DAN, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_CANT, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_RADIO_ST, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_ST, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_OUTBK, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_OUTBK, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_TRIM, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_TRIMPER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_PERCENT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_CHECK_SHOW_SLABHUNCH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDeckPlanRebarDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();
	CADeckData			 *pDeckData		= m_pStd->m_pDeckData;
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	BOOL bShowMark = pDeckData->m_bShowMark && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag() ? TRUE : FALSE;
	
	CPlateBxFinder FinderB(pDB->GetGirder(-1));	
	CPlateBasicIndex *pBxStt = FinderB.GetBxFirst(BX_STT_SLAB);
	CPlateBasicIndex *pBxEnd = FinderB.GetBxFirst(BX_END_SLAB);	

	CDomyun Dom(pDom);
	Dom.SetScaleDim(100);
	Dom.SetDirection("TOP");

	// 일반도
	DBPlan.DrawSlabTotal(&Dom);
	DBPlan.DrawCenterJijum(&Dom,TRUE);                 //지점중심선(시작부)
	DBPlan.DrawCenterJijum(&Dom,FALSE);				//지점중심선(종부)
	DBPlan.DrawAbutPierCenterLine(&Dom);               //교대교각중심선
	for(long g=0; g<pDB->GetGirdersu(); g++)
		DBPlan.DrawAbutPierSymbol(&Dom, g);     //교대, 교각심볼마크
	DBPlan.DrawCenterLine(&Dom, -1, pBxStt->GetStation(), pBxEnd->GetStation());
	DBPlan.DrawTaperLinePyung(&Dom);			// Taper 선 그리기
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_CENT);
	DBPlan.DrawUpDownLine(&Dom, pBxStt->GetStation(), pBxEnd->GetStation());		
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_CONC);
	DBPlan.DrawExpJointLine(&Dom, TRUE,  -1);
	DBPlan.DrawExpJointLine(&Dom, FALSE, -1);

	// 거더라인 및 슬래브 헌치부(가로보 및 세로보 헌치부)
	long nGirderSu  = pDB->GetGirdersu();
	for(long nG=0; nG<nGirderSu; nG++)
	{
		CPlateGirderApp	*pGir   = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);	
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
		DBPlan.DrawGirderWithenStation(&Dom,pBxStt, pBxStt->GetStation(),pBxEnd->GetStation(), TRUE);		

		if(nG < nGirderSu-1 && m_bShowSlabHunch)
			DBPlan.DrawPlanCrossBeam(&Dom, pBxStt, pBxEnd);

	}
//	// 외측가로보 헌치부(좌측)
//	if(pDB->GetLBracketsu()>0 && m_bShowSlabHunch)
//	{			
//		CPlateBxFinder Finder(pDB->GetGirder(0));
//		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_LEFT, NULL, NULL);						
//		long nUpLow = 1;
//		while(pBx)
//		{
//			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
//			if(pBxNext)
//				pDBPlan->DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, TRUE);
//			pBx = Finder.GetBxNext();
//		}
//	}		
//	// 외측가로보 헌치부(우측)	
//	if(pDB->GetRBracketsu()>0 && m_bShowSlabHunch)
//	{			
//		CPlateBxFinder Finder(pDB->GetGirder(pDB->GetGirdersu()-1));	
//		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_RIGHT, NULL, NULL);						
//		long nUpLow = 1;
//		while(pBx)
//		{
//			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
//			if(pBxNext)
//				pDBPlan->DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, FALSE);
//			pBx = Finder.GetBxNext();
//		}
//	}

	// 트림라인 설정
	CPtrArray ObArcArray, ObArcForDeleteArray, ObCircleArrayCB, ObCircleArrayST, ObCircleArrayOBK, ObCircleArrayDan, ObCircleArrayCant;
	CDPointArray xyArrCB, xyArrST, xyArrOBK;
	CVectorArray xyArrDan, xyArrCant;

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);			
	DBDeckPlan.DrawTrimLine(&Dom, ObArcArray, ObArcForDeleteArray, ObCircleArrayCB, ObCircleArrayST, ObCircleArrayOBK, ObCircleArrayDan, ObCircleArrayCant, xyArrCB, xyArrST, xyArrOBK, xyArrDan, xyArrCant);
	
	if(m_nTab==0)
	{
		if(CAPlateDrawPyung::IsInputDomRotate())
		{
			CDPoint xyStt = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB));
			CDPoint xyEnd = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB));
			CDPoint vDir  = (xyEnd-xyStt).Unit();

			vDir.MirrorVert();
			Dom.Rotate(xyStt,vDir);
		}

		*pDom << Dom;
		CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
		return;
	}

	// 치수선
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	DBDimDeckPlan.DimPyungHDan(&Dom, pBxStt, DIMDECK_BASEHDAN_LOWER | DIMDECK_BASEHDAN_UPPER | DIMDECK_BASEHDAN_TOTAL, TRUE, 2);
	DBDimDeckPlan.DimPyungHDan(&Dom, pBxEnd, DIMDECK_BASEHDAN_LOWER | DIMDECK_BASEHDAN_UPPER | DIMDECK_BASEHDAN_TOTAL, FALSE, 2);		
	DBDimDeckPlan.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_DECK_MAINREBAR_UPPER | DIM_DECK_TOTAL_LEFT,  TRUE,  FALSE, "", 3);
	DBDimDeckPlan.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_DECK_MAINREBAR_LOWER | DIM_DECK_TOTAL_RIGHT, FALSE, FALSE, "", 3);

	// 배근도면
	CDomyun ReBarDom(&Dom), TotalDom(&Dom), TempDom(&Dom);
	
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&TotalDom,HCAD_BARC);
	DBDeckPlan.DrawMainRebar(&TotalDom, pBxStt, pBxEnd, TRUE, DRAW_LEFTSIDE);
	DBDeckPlan.DrawMainRebar(&TotalDom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE);
	DBDeckPlan.DrawMainRebarAtSkew(&TotalDom, TRUE, DRAW_LEFTSIDE);
	DBDeckPlan.DrawMainRebarAtSkew(&TotalDom, FALSE, DRAW_RIGHTSIDE);
	DBDeckPlan.DrawSabogangRebar(&TotalDom, TRUE, DRAW_LEFTSIDE);
	DBDeckPlan.DrawSabogangRebar(&TotalDom, FALSE, DRAW_RIGHTSIDE);
	DBDeckPlan.DrawBeryukRebarOnCenter(&TotalDom, pBxStt, pBxEnd, TRUE,DRAW_LEFTSIDE);
	DBDeckPlan.DrawBeryukRebarOnCenter(&TotalDom, pBxStt, pBxEnd, FALSE,DRAW_RIGHTSIDE);
	DBDeckPlan.DrawBeryukRebarOnJijum(&TotalDom, pBxStt, pBxEnd, TRUE, DRAW_LEFTSIDE);
	DBDeckPlan.DrawBeryukRebarOnJijum(&TotalDom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE);
		
	for(long nx=0; nx<ObArcArray.GetSize(); ++nx)				
	{	
		TempDom.ClearEtt(TRUE, TRUE);
		TempDom = TotalDom;
		if(nx%2==1)									// 좌측 삭제
		{
			CObArc *pArc	= (CObArc *)ObArcArray.GetAt(nx);
			CObArc *pArcDel	= (CObArc *)ObArcForDeleteArray.GetAt(nx);
			TempDom.TrimByArc2(pArc, pArcDel->m_SttPoint, FALSE);
			TempDom.TrimByCircle2(pArcDel->m_SttPoint,pArcDel->m_Radius, TRUE);
			TempDom.DeleteByArcBetweenArc(pArc, pArcDel);
			++nx;
		}

		if(nx%2==0 && nx<ObArcArray.GetSize())		// 우측 삭제
		{
			CObArc *pArc	= (CObArc *)ObArcArray.GetAt(nx);
			CObArc *pArcDel	= (CObArc *)ObArcForDeleteArray.GetAt(nx);
			TempDom.TrimByArc2(pArc, pArcDel->m_SttPoint, FALSE);
			TempDom.TrimByCircle2(pArcDel->m_SttPoint,pArcDel->m_Radius, TRUE);
			TempDom.DeleteByArcBetweenArc(pArc, pArcDel);
		}

		ReBarDom << TempDom;
	}
	Dom << ReBarDom;

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&ReBarDom,HCAD_DIML);	
	// 마크
	if(bShowMark)
	{
		for(long nx=0; nx<pDB->m_nQtyJigan; nx++)
			DBDeckPlan.DrawMark(&Dom, nx);	
	}

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&ReBarDom,HCAD_BARC);	
	
	// 상세원안에 있는 철근, 마크 트림.
	for(long nx=0; nx<ObCircleArrayCB.GetSize(); nx++)
	{
		CObArc *pCircle = (CObArc *)ObCircleArrayCB.GetAt(nx);
		Dom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius,FALSE);
	}	
	for(long nx=0; nx<ObCircleArrayDan.GetSize(); nx++)
	{
		CObArc *pCircle = (CObArc *)ObCircleArrayDan.GetAt(nx);
		Dom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, FALSE);
	}						
	for(long nx=0; nx<ObCircleArrayCant.GetSize(); nx++)
	{
		CObArc *pCircle = (CObArc *)ObCircleArrayCant.GetAt(nx);
		Dom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, FALSE);
	}						

	//////////////////////////////////////////////////////////////////////////
	// 상세도면 그리기 (가로보)
	for(long nx=0; nx<ObCircleArrayCB.GetSize(); nx++)
	{
		CDPoint xyIndexCB = pDeckData->m_arrSangseIndexCB.GetAt(nx);
		long nG  = (long)xyIndexCB.x;
		long nCB = (long)xyIndexCB.y;

		if(nG<0 || nCB<0)	continue;

		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateCrossBeam *pC  = pGir->m_CrossSectionArr[nCB]->GetCrossBeam();
		if(pC==NULL)	continue;

		DBDeckPlan.DrawBogangRebarAtCrossBeamBx(&ReBarDom,pC->GetBx(),0, TRUE);

		CObArc *pCircle = (CObArc *)ObCircleArrayCB.GetAt(nx);
		ReBarDom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, TRUE);
		Dom << ReBarDom;
	}

	// 상세도면 그리기 (단부)
	if(pDB->m_dHeightTaperStt > 0)
	{
		for(long nx=0; nx<ObCircleArrayDan.GetSize(); nx++)
		{
			CVector vecPosDan = pDeckData->m_arrSangsePosDan[nx];
			BOOL bSttSlab = vecPosDan.z==-1 ? TRUE : FALSE;
			DBDeckPlan.DrawSangseUnderSinchuk(&ReBarDom, bSttSlab, DRAW_SIDEALL);		

			CObArc *pCircle = (CObArc *)ObCircleArrayDan.GetAt(nx);
			ReBarDom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, TRUE);
			Dom << ReBarDom;
		}
	}
	// 상세도면 그리기 (캔틸레버부)
	for(long nx=0; nx<ObCircleArrayCant.GetSize(); nx++)
	{
		CVector vecPosCant = pDeckData->m_arrSangsePosCant[nx];
		BOOL bUpper = vecPosCant.z==-1 ? FALSE : TRUE;

		DBDeckPlan.DrawMainRebar(&ReBarDom, pBxStt, pBxEnd, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE);
		DBDeckPlan.DrawMainRebarAtSkew(&ReBarDom, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE, TRUE, TRUE);
		DBDeckPlan.DrawSabogangRebar(&ReBarDom, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE, TRUE, TRUE);					
		DBDeckPlan.DrawBeryukRebarOnCenter(&ReBarDom, pBxStt, pBxEnd, bUpper,bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE);
		DBDeckPlan.DrawBeryukRebarOnJijum(&ReBarDom, pBxStt, pBxEnd, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE);

		CObArc *pCircle = (CObArc *)ObCircleArrayCant.GetAt(nx);
		ReBarDom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, TRUE);					
		Dom << ReBarDom;
	}						
	// 상세도면 마크
	if(bShowMark)
	{
		for (long nJigan=0; nJigan<pDB->m_nQtyJigan; nJigan++)
		{
			DBDeckPlan.DrawMarkCB_ST_OBK(&ReBarDom, nJigan);
			DBDeckPlan.DrawMarkSangseDanbu(&ReBarDom, nJigan);
			DBDeckPlan.DrawMarkSangseCant(&ReBarDom, nJigan);
			Dom << ReBarDom;	
		}
	}

	if(CAPlateDrawPyung::IsInputDomRotate())
	{
		CDPoint xyStt = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB));
		CDPoint xyEnd = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB));
		CDPoint vDir  = (xyEnd-xyStt).Unit();

		vDir.MirrorVert();
		Dom.Rotate(xyStt,vDir);
	}

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}


void CDeckPlanRebarDlg::SetGridData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nColsu = pDeckData->m_arrTrimRange.GetSize()+1;
	m_Grid.SetGridDefault(3, nColsu, 1, 1);

	CString str;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	
	m_Grid.AddCell(0,0,"구 분");
	m_Grid.AddCell(1,0,"길이");
	m_Grid.AddCell(2,0,"상태");
	
	CStringArray sArr;
	for(long nCol=1; nCol<nColsu; nCol++)
	{
		str.Format("구간%d",nCol);
		m_Grid.AddCell(0,nCol,str);
		m_Grid.AddCell(1,nCol,&pDeckData->m_arrTrimRange, nCol-1, nFormat, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, NULL, CELL_DEC_0);		
		m_Grid.AddCell(2,nCol,nCol%2==1 ? "Draw" : "Trim", nFormat, CELL_READONLY);
	}
	m_Grid.SetRedraw(TRUE,TRUE);
	
}


void CDeckPlanRebarDlg::SetDataDefault()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	pDeckData->m_arrSangsePosCB.RemoveAll();
	pDeckData->m_arrSangsePosST.RemoveAll();
	pDeckData->m_arrSangsePosOBK.RemoveAll();
	pDeckData->m_arrSangsePosDan.RemoveAll();
	pDeckData->m_arrSangsePosCant.RemoveAll();
	pDeckData->m_arrSangseIndexCB.RemoveAll();
	pDeckData->m_arrSangseIndexST.RemoveAll();
	pDeckData->m_arrSangseIndexOBK.RemoveAll();

	if(GetSafeHwnd()) UpdateData();
	
	if(IsUsedFlag()==FALSE)
		pDeckData->m_dTrimRatio = 35;

	// 트림구간설정
	CPlateGirderApp *pGir = pDB->GetGirder(-1);
	CPlateBxFinder Finder(pGir);	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_SLAB);
	CPlateBasicIndex *pBxCur, *pBxPre = pBxStt;
	
	double dLenJigan=0, dCurSta=pBxStt->GetStation(), dTotalLen=0;
	double LDist = pDeckData->GetSideBarDistFromCenterGir(SECTION_DAN, TRUE)  - pDeckData->m_dCoverSd;
	double RDist = pDeckData->GetSideBarDistFromCenterGir(SECTION_DAN, FALSE) + pDeckData->m_dCoverSd;
	pDeckData->m_arrTrimRange.SetSize(3+(pDB->m_nQtyJigan-1)*2);	
	
	long	nC		= 0;
	CString	sText	= _T("");

	for(long nJ=1; nJ<=pDB->m_nQtyJigan; nJ++)
	{		
		pBxCur = pGir->GetBxOnJijum(nJ);
		if(nJ==pDB->m_nQtyJigan) pBxCur = pBxEnd;
		dLenJigan = pBxCur->GetStation() - pBxPre->GetStation();		

		// 1. 첫번째 삽입
		double dLen = Round(dTotalLen + dLenJigan * pDeckData->m_dTrimRatio/100,-2);		
		double dLenCenter = pGir->GetLine()->GetLengthByDis(dCurSta, dCurSta+dLen, (LDist+RDist)/2);				
		pDeckData->m_arrTrimRange.SetAt(nC++,dLen);		
		dCurSta += dLen;		
				
		// 2. 다음번째 삽입
		dLen = Round(dLenJigan - Round(dLenJigan * pDeckData->m_dTrimRatio/100,-2)*2,-2);		
		dLenCenter = pGir->GetLine()->GetLengthByDis(dCurSta, dCurSta+dLen, (LDist+RDist)/2);				
		pDeckData->m_arrTrimRange.SetAt(nC++,dLen);
		dCurSta += dLen;

		dTotalLen = Round(dLenJigan * pDeckData->m_dTrimRatio/100,-2);		

		pBxPre = pBxCur;
	}	
	pDeckData->m_arrTrimRange.SetAt(nC++,Round(dLenJigan * pDeckData->m_dTrimRatio/100,-2));		

//	if(pDeckData->IsBridgeHasCrossBeam() && pDB->m_dThickSlabHunch > 0 )
//		pDeckData->m_arrSangsePosCB.Add(FindXyPosByIndexCB(0, 1));
//	if(pDeckData->IsBridgeHasStringer() && pDB->m_dThickSlabHunch > 0 )
//		pDeckData->m_arrSangsePosST.Add(FindXyPosByIndexST(0, 1));
//	if(pDeckData->IsBridgeHasOutBracket() && pDB->m_dThickSlabHunch > 0 )
//		pDeckData->m_arrSangsePosOBK.Add(FindXyPosByIndexOBK(TRUE, 1));

//	pDeckData->m_arrSangseIndexCB.Add(CDPoint(0,1));
//	pDeckData->m_arrSangseIndexST.Add(CDPoint(0,1));
//	pDeckData->m_arrSangseIndexOBK.Add(CDPoint(0,1));
	pDeckData->m_dDiaSangsePosCB   = 2500;
	pDeckData->m_dDiaSangsePosST   = 2500;
	pDeckData->m_dDiaSangsePosOBK  = 2500;
	pDeckData->m_dDiaSangsePosDan  = 2500;
	pDeckData->m_dDiaSangsePosCant = 2500;
}


CDPoint CDeckPlanRebarDlg::FindXyPosByIndexCB(long nG, long nCB)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateCrossBeam *pC   = pGir->m_CrossSectionArr[nCB]->GetCrossBeam();
	
	if(pC==NULL)	return CDPoint(0,0);

	CPlateBasicIndex *pBx   = pC->GetBx();
	CPlateBasicIndex *pBxR  = pBx->GetBxInRighGirderSection();
	if(pBxR==NULL)	return CDPoint(0,0);
	
	CPlateGirderApp  *pGirR = pBx->GetGirder()->m_pRight;

	double  sta1  = pGir->GetStationBySc (pBx->GetStation(),  pGir->GetWidthFlangeUpper(pBx)/2, pBx->GetAngle());
	double  sta2  = pGirR->GetStationBySc (pBxR->GetStation(), -pGirR->GetWidthFlangeUpper(pBxR)/2, pBxR->GetAngle());
	CDPoint diap1 = pGir->GetXyGirderDis (sta1,  pGir->GetWidthFlangeUpper(pBx)/2);
	CDPoint diap2 = pGirR->GetXyGirderDis (sta2, -pGirR->GetWidthFlangeUpper(pBxR)/2);
	CDPoint vGir1 = pGir->GetLine()->GetAngleAzimuth(sta1);
	CDPoint vGir2 = pGirR->GetLine()->GetAngleAzimuth(sta2);
	CDPoint vCross= (diap1 - diap2).Unit();
	CDPoint base1 = diap1;
	CDPoint base2 = diap2;
	CDPoint xyCen = (diap1 + diap2)/2;	

	return xyCen;
}


CDPoint CDeckPlanRebarDlg::FindXyPosByIndexST(long nG, long nCB)
{
//	CADeckData  *pDeckData = m_pStd->m_pDeckData;
//	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
//	CPlateGirderApp  *pGir	   = pDB->GetGirder(nG);
//	CPlateBasicIndex *pBx	   = pGir->GetCrossBeamByCrossNo(nCB)->GetBx();
//	CPlateBasicIndex *pBxNext   = pGir->GetCrossBeamByCrossNo(nCB+1)->GetBx();
//
//	long nS = 0;
//	BOOL bMatchStt = FALSE;
//	BOOL bMatchEnd = FALSE;
//	CDPoint xy1 = pGir->GetXyPlanSlabStgBase(pBx, nS, TRUE);
//	CDPoint xy2 = pGir->GetXyPlanSlabStgBase(pBxNext, nS, FALSE);
//	CDPoint xyMatchStt, xyMatchEnd;
//
//	CStringer *pStg = pBx->GetStringer(nS);
//	if(pStg==NULL)	return CDPoint(0,0);
//	
//	pDeckData->IsStringerMatchAtTaper(pStg, bMatchStt, bMatchEnd, xyMatchStt, xyMatchEnd);
//
//	if(bMatchStt)		xy1 = xyMatchStt;
//	if(bMatchEnd)		xy2 = xyMatchEnd;
//
//	CDPoint vVec = (xy2 - xy1).Unit();
//	CDPoint xyCen= (xy1 + xy2) / 2;	

	CDPoint xyCen;
	return xyCen;
}



CDPoint CDeckPlanRebarDlg::FindXyPosByIndexOBK(BOOL bLeftSlab, long nObk)
{
//	CADeckData  *pDeckData = m_pStd->m_pDeckData;
//	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
//	CPlateGirderApp  *pGir      = pDB->GetGirder(bLeftSlab ? 0 : pDB->GetGirdersu()-1);
//	CBracket    *pBrk      = pDB->GetBracket(nObk, bLeftSlab);
//	
//	if(pBrk==NULL)	return CDPoint(0,0);
//
//	CDPoint xyStt = pGir->GetXyPlanSlabOutBrkBase(pBrk->GetBx(), bLeftSlab, TRUE);
//	CDPoint xyEnd = pGir->GetXyPlanSlabOutBrkBase(pBrk->GetBx(), bLeftSlab, FALSE);
//	CDPoint xyCen = (xyStt+xyEnd)/2;
	
	CDPoint xyCen;
	return xyCen;
}




void CDeckPlanRebarDlg::SetDataDefaultByTrimRatio() 
{
	CADeckData  *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	CPlateGirderApp *pGir = pDB->GetGirder(-1);
	CPlateBxFinder Finder(pGir);	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_SLAB);
	CPlateBasicIndex *pBxCur, *pBxPre = pBxStt;
	
	double dLenJigan=0, dCurSta=pBxStt->GetStation(), dTotalLen=0;
	double LDist = pDeckData->GetSideBarDistFromCenterGir(SECTION_DAN, TRUE)  - pDeckData->m_dCoverSd;
	double RDist = pDeckData->GetSideBarDistFromCenterGir(SECTION_DAN, FALSE) + pDeckData->m_dCoverSd;
	
	pDeckData->m_arrTrimRange.SetSize(3+(pDB->m_nQtyJigan-1)*2);	
	long nC = 0;
	for(long nJ=1; nJ<=pDB->m_nQtyJigan; nJ++)
	{		
		pBxCur = pGir->GetBxOnJijum(nJ);
		if(nJ==pDB->m_nQtyJigan) pBxCur = pBxEnd;
		dLenJigan = pBxCur->GetStation() - pBxPre->GetStation();		

		// 1. 첫번째 삽입
		double dLen = Round(dTotalLen + dLenJigan * pDeckData->m_dTrimRatio/100,-2);		
		double dLenCenter = pGir->GetLine()->GetLengthByDis(dCurSta, dCurSta+dLen, (LDist+RDist)/2);				
		pDeckData->m_arrTrimRange.SetAt(nC++,dLen);		
		dCurSta += dLen;		
				
		// 2. 다음번째 삽입
		dLen = Round(dLenJigan - Round(dLenJigan * pDeckData->m_dTrimRatio/100,-2)*2,-2);		
		dLenCenter = pGir->GetLine()->GetLengthByDis(dCurSta, dCurSta+dLen, (LDist+RDist)/2);				
		pDeckData->m_arrTrimRange.SetAt(nC++,dLen);
		dCurSta += dLen;

		dTotalLen = Round(dLenJigan * pDeckData->m_dTrimRatio/100,-2);		

		pBxPre = pBxCur;
	}	
	pDeckData->m_arrTrimRange.SetAt(nC++,Round(dLenJigan * pDeckData->m_dTrimRatio/100,-2));		
}


void CDeckPlanRebarDlg::SetDataInit()
{	
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	SetGridData();
	m_Grid.UpdateData(FALSE);
	m_Grid.SetColumnWidthAll(80);	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,65);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetRedraw(TRUE, TRUE);
	
	sText.Format("%g",	pDeckData->m_dTrimRatio);

	m_dTrimRatio.SetWindowText(sText);
	
	m_szDiaCB.SetWindowText(Comma(pDeckData->m_dDiaSangsePosCB));
	m_szDiaStringer.SetWindowText(Comma(pDeckData->m_dDiaSangsePosST));
	m_szDiaOutBk.SetWindowText(Comma(pDeckData->m_dDiaSangsePosOBK));
	m_szDiaDan.SetWindowText(Comma(pDeckData->m_dDiaSangsePosDan));
	
	m_szDiaCant.SetWindowText(Comma(pDeckData->m_dDiaSangsePosCant));

	GetDlgItem(IDC_RADIO_CB)->EnableWindow(pDeckData->m_pDB->GetsuCrossBeamBxHasHunch()>0 ? TRUE : FALSE);
	GetDlgItem(IDC_EDIT_CB)->EnableWindow(pDeckData->m_pDB->GetsuCrossBeamBxHasHunch()>0 ? TRUE : FALSE);
	GetDlgItem(IDC_RADIO_ST)->EnableWindow(pDeckData->IsBridgeHasStringer());
	GetDlgItem(IDC_EDIT_ST)->EnableWindow(pDeckData->IsBridgeHasStringer());
	GetDlgItem(IDC_RADIO_OUTBK)->EnableWindow(pDeckData->IsBridgeHasOutBracket());
	GetDlgItem(IDC_EDIT_OUTBK)->EnableWindow(pDeckData->IsBridgeHasOutBracket());

	UpdateData(FALSE);
}

void CDeckPlanRebarDlg::SetDataSave()
{
	CString			sText		= _T("");
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	m_Grid.UpdateData(TRUE);

	m_dTrimRatio.GetWindowText(sText);
	pDeckData->m_dTrimRatio			= atof(sText);
	
	m_szDiaCB.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosCB   = atof(sText);

	m_szDiaStringer.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosST   = atof(sText);

	m_szDiaOutBk.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosOBK  = atof(sText);
	
	m_szDiaCant.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosCant = atof(sText);
	
	m_szDiaDan.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosDan  = atof(sText);
}


long CDeckPlanRebarDlg::GetTabCount()
{
	return 2;
}


CString CDeckPlanRebarDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0) return "간략보기";
	return "상세보기";	
}


void CDeckPlanRebarDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	

	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CString szData = m_Grid.GetLastChangeData();
	double dChangedVal = m_Grid.GetTextMatrixDouble(nRow, nCol+1) + atof(szData) - m_Grid.GetTextMatrixDouble(nRow, nCol);
	m_Grid.SetTextMatrix(nRow, nCol+1,"%.1f",max(dChangedVal,0));

	SetDataSave();

	DrawInputDomyunView(FALSE);
}



void CDeckPlanRebarDlg::SetPosition(CPoint ptPoint)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CPlateGirderApp		*pGirCen    = pDB->GetGirder(-1);
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	CDPoint xyClickPos = m_pView->ClientToExt(ptPoint.x, ptPoint.y);	
	if(CAPlateDrawPyung::IsInputDomRotate())
	{
		CDPoint xyStt = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB));
		CDPoint xyEnd = pDB->m_pLine->GetXyLine(pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB));
		CDPoint vDir  = (xyEnd-xyStt).Unit();

		xyClickPos = GetXyRotateVector(xyStt, xyClickPos, vDir);
	}
	
	// 더블클릭한 위치가 xyArrPoly 영역안에 존재해야 함. 
	CRebarPlacing RebarPlace;
	CDPointArray xyArrPoly;
	double dStaExt = 1000;
	long n = 0;
	for (n=0; n<=pDB->m_nQtyJigan; n++)
	{
		double  dStaJijum = pGirCen->GetStationOnJijum(n);
		if(n==0)
			dStaJijum   = pGirCen->GetSSS(OFF_STT_SLAB)-dStaExt;
		else if(n==pDB->m_nQtyJigan)
			dStaJijum   = pGirCen->GetSSS(OFF_END_SLAB)+dStaExt;

		CDPoint xyJijumRigh = pDB->m_pLine->GetXyLineDisByAng(dStaJijum, pDB->GetDisSlabLeftAct(dStaJijum)-2000, pGirCen->GetAngleOnJijum(n));
		xyArrPoly.Add(xyJijumRigh);
	}
	for (n=pDB->m_nQtyJigan; n>=0; n--)
	{
		double  dStaJijum  = pGirCen->GetStationOnJijum(n);
		if(n==0)
			dStaJijum   = pGirCen->GetSSS(OFF_STT_SLAB)-dStaExt;
		else if(n==pDB->m_nQtyJigan)
			dStaJijum   = pGirCen->GetSSS(OFF_END_SLAB)+dStaExt;

		CDPoint xyJijumLeft = pDB->m_pLine->GetXyLineDisByAng(dStaJijum, pDB->GetDisSlabRighAct(dStaJijum)+2000, pGirCen->GetAngleOnJijum(n));
		xyArrPoly.Add(xyJijumLeft);
	}
	xyArrPoly.Add(xyArrPoly[0]);
	BOOL bCheckIn = RebarPlace.CheckXyByPolygon(xyClickPos, xyArrPoly, TRUE);
	if(!bCheckIn)
	{
		AfxMessageBox("영역을 벗어났습니다. 다시 선택하세요");
		return;
	}
	// xyClickPos에서 가장 근접한 해당Bx를 찾음.
	CPlateBasicIndex *pBx = NULL;
	long nG=0, nCB=0, nObk=0;
	if(m_nCurSangsePos==0 || m_nCurSangsePos==1)	// 가로보, 세로보
	{
		pBx = pDB->GetBxOnNearByPoint(xyClickPos, BX_CROSSBEAM_VBRACING);
		if(pBx->GetCrossBeam()==NULL)
			pBx = pBx->GetBxMatchByCrossBeam(pBx->GetGirder()->m_pLeft);
		if(pBx)
		{
			nG  = pBx->GetNumberGirder();
			nCB = pBx->GetNumberCrossBeam();	
		}
	}
//	else if(m_nCurSangsePos==2)						// 외측가로보
//	{
//		pBx = pDB->GetBxOnNearByPoint(xyClickPos, BX_OUTSIDE_CROSS_LEFT|BX_OUTSIDE_CROSS_RIGHT);
//		if(pBx)
//		{
//			nG   = pBx->GetNumberGirder();
//			nObk = pBx->GetBracketLeft() ? pBx->GetBracketLeft()->GetSelfCount() : pBx->GetBracketRigh()->GetSelfCount();
//		}
//	}
//	
	//////////////////////////////////////////////////////////////////////////
	BOOL bDuplicated = FALSE;
	if(m_nCurSangsePos==0)			// 가로보
	{
		for(long n=0; n<pDeckData->m_arrSangseIndexCB.GetSize(); n++)
		{
			CDPoint ptData = pDeckData->m_arrSangseIndexCB.GetAt(n);
			if(ptData.x==nG && ptData.y==nCB)
			{
				pDeckData->m_arrSangseIndexCB.RemoveAt(n);				
				pDeckData->m_arrSangsePosCB.RemoveAt(n);				
				bDuplicated = TRUE;
				break;
			}
		}		
		if(!bDuplicated)
		{
			pDeckData->m_arrSangseIndexCB.Add(CDPoint(nG, nCB));
			pDeckData->m_arrSangsePosCB.Add(FindXyPosByIndexCB(nG, nCB));
		}
	}
	else if(m_nCurSangsePos==1)		// 세로보
	{
		for(long n=0; n<pDeckData->m_arrSangseIndexST.GetSize(); n++)
		{
			CDPoint ptData = pDeckData->m_arrSangseIndexST.GetAt(n);
			if(ptData.x==nG && ptData.y==nCB)
			{
				pDeckData->m_arrSangseIndexST.RemoveAt(n);				
				pDeckData->m_arrSangsePosST.RemoveAt(n);				
				bDuplicated = TRUE;
				break;
			}
		}		
		if(!bDuplicated)
		{
			pDeckData->m_arrSangseIndexST.Add(CDPoint(nG, nCB));
			pDeckData->m_arrSangsePosST.Add(FindXyPosByIndexST(nG, nCB));
		}
	}
	else if(m_nCurSangsePos==2)		// 외측가로보
	{
		for(long n=0; n<pDeckData->m_arrSangseIndexOBK.GetSize(); n++)
		{
			CDPoint ptData = pDeckData->m_arrSangseIndexOBK.GetAt(n);
			if(ptData.x==nG && ptData.y==nObk)
			{
				pDeckData->m_arrSangseIndexOBK.RemoveAt(n);				
				pDeckData->m_arrSangsePosOBK.RemoveAt(n);				
				bDuplicated = TRUE;
				break;
			}
		}		
		if(!bDuplicated)
		{
			pDeckData->m_arrSangseIndexOBK.Add(CDPoint(nG, nObk));
			pDeckData->m_arrSangsePosOBK.Add(FindXyPosByIndexOBK(nG==0?TRUE:FALSE, nObk));
		}
	}
	else if(m_nCurSangsePos==3)		// 단부
	{
		for(long n=0; n<pDeckData->m_arrSangsePosDan.GetSize(); n++)
		{
			CDPoint xyPosDan = pDeckData->m_arrSangsePosDan.GetAt(n);
			if(~(xyClickPos-xyPosDan) < pDeckData->m_dDiaSangsePosDan/2)
			{
				pDeckData->m_arrSangsePosDan.RemoveAt(n);				
				bDuplicated = TRUE;
				break;
			}
		}		
		BOOL   bSttSlab = FALSE;
		double dStaCur  = 0;
		pDB->m_pLine->GetStationMatchVerticalByXy(xyClickPos, dStaCur);
		double dStaMid = (pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB)+pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB))/2;
		if(dStaCur<dStaMid)
			bSttSlab = TRUE;

		if(!bDuplicated)
		{
			CVector vData = CVector(xyClickPos.x, xyClickPos.y, bSttSlab ? -1 : 1);
			pDeckData->m_arrSangsePosDan.Add(vData);
		}
	}
	else if(m_nCurSangsePos==4)		// 캔틸레버부
	{
		for(long n=0; n<pDeckData->m_arrSangsePosCant.GetSize(); n++)
		{
			CDPoint xyPosCant = pDeckData->m_arrSangsePosCant.GetAt(n);
			if(~(xyClickPos-xyPosCant) < pDeckData->m_dDiaSangsePosCant/2)
			{
				pDeckData->m_arrSangsePosCant.RemoveAt(n);				
				bDuplicated = TRUE;
				break;
			}
		}		
		if(!bDuplicated)
		{
			CDPoint AStt[4], AEnd[4];
			pDB->GetXySss(OFF_STT_SLAB, AStt);
			pDB->GetXySss(OFF_END_SLAB, AEnd);
			CDPoint xyPosSlabSttMid = (AStt[0]+AStt[3])/2;			
			CDPoint xyPosSlabEndMid = (AEnd[0]+AEnd[3])/2;			
			long nDir = GetDirectionRotateByPoint(xyPosSlabEndMid, xyPosSlabSttMid, xyClickPos);
			CVector vData = CVector(xyClickPos.x, xyClickPos.y, nDir);
			pDeckData->m_arrSangsePosCant.Add(vData);
		}
	}
		
	DrawInputDomyunView(FALSE);
}


BOOL CDeckPlanRebarDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}


void CDeckPlanRebarDlg::OnCheckShowSlabhunch() 
{
	UpdateData();

	DrawInputDomyunView();	
}

void CDeckPlanRebarDlg::OnKillfocusEditTrim() 
{
	CString			sText		= _T("");
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;

	UpdateData(TRUE);

	m_dTrimRatio.GetWindowText(sText);

	pDeckData->m_dTrimRatio = atof(sText);
	
	SetDataDefaultByTrimRatio();
	SetDataInit();

	DrawInputDomyunView(FALSE);
}


void CDeckPlanRebarDlg::OnRadioCant() 
{
	UpdateData();
}

void CDeckPlanRebarDlg::OnRadioCB() 
{
	UpdateData();	
}

void CDeckPlanRebarDlg::OnRadioDan() 
{
	UpdateData();
}

void CDeckPlanRebarDlg::OnRadioOutBk() 
{
	UpdateData();	
}

void CDeckPlanRebarDlg::OnRadioSt() 
{
	UpdateData();	
}

void CDeckPlanRebarDlg::OnKillfocusEditCB() 
{
	UpdateData();
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_szDiaCB.GetWindowText(sText);

	pDeckData->m_dDiaSangsePosCB   = atof(sText);

	DrawInputDomyunView(FALSE);	
}

void CDeckPlanRebarDlg::OnKillfocusEditCant() 
{
	UpdateData();
	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_szDiaCant.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosCant   = atof(sText);

	DrawInputDomyunView(FALSE);	
	
}

void CDeckPlanRebarDlg::OnKillfocusEditDan() 
{
	UpdateData();

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_szDiaDan.GetWindowText(sText);

	pDeckData->m_dDiaSangsePosDan   = atof(sText);

	DrawInputDomyunView(FALSE);	
		
}

void CDeckPlanRebarDlg::OnKillfocusEditOutbk() 
{
	UpdateData();

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_szDiaOutBk.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosOBK   = atof(sText);

	DrawInputDomyunView(FALSE);	
		
}

void CDeckPlanRebarDlg::OnKillfocusEditSt() 
{
	UpdateData();

	CString		sText		= _T("");
	CADeckData	*pDeckData	= m_pStd->m_pDeckData;

	m_szDiaStringer.GetWindowText(sText);
	pDeckData->m_dDiaSangsePosST = atof(sText);

	DrawInputDomyunView(FALSE);		
}

void CDeckPlanRebarDlg::OnButtonApply() 
{
	UpdateData();
	SetDataSave();
	SetDataInit();

	DrawInputDomyunView();	
}
