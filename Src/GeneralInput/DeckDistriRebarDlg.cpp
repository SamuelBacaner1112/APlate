// DeckDistriRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateDBDraw/APlateDBDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckDistriRebarDlg dialog


CDeckDistriRebarDlg::CDeckDistriRebarDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckDistriRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDistriRebarDlg)
	m_szScale = _T("");
	m_szA1 = _T("");
	m_szA2 = _T("");
	m_szB1 = _T("");
	m_szB2 = _T("");
	m_szC1 = _T("");
	m_szC2 = _T("");
	m_bShowDia = FALSE;
	//}}AFX_DATA_INIT
}


void CDeckDistriRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDistriRebarDlg)
	DDX_Control(pDX, IDC_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_COMBO_VIEW_CYCLE, m_ctrlComboCycle);
	DDX_Control(pDX, IDC_COMBO_DAN_BERYUK_STYLE, m_ctrlComboDanBogangArrangeMethod);
	DDX_Control(pDX, IDC_COMBO_DAN_BERYUK_MAIN_INSTALL, m_ctrlComboUseJujangRebar);
	DDX_Control(pDX, IDC_COMBO_JIJUM_MAIN_STYLE, m_ctrlComboJijumBogangArrange);
	DDX_Text(pDX, IDC_STATIC_SCALE, m_szScale);
	DDX_Text(pDX, IDC_EDIT_A1, m_szA1);
	DDX_Text(pDX, IDC_EDIT_A2, m_szA2);
	DDX_Text(pDX, IDC_EDIT_B1, m_szB1);
	DDX_Text(pDX, IDC_EDIT_B2, m_szB2);
	DDX_Check(pDX, IDC_CHECK_DIA, m_bShowDia);
	DDX_Text(pDX, IDC_EDIT_C1, m_szC1);
	DDX_Text(pDX, IDC_EDIT_C2, m_szC2);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_BUTTON_SAMEBY_CURTAP, m_btnSameTap);
	DDX_Control(pDX, IDC_BUTTON_RECALC_SHAPE, m_btnReCalc);
}

BEGIN_MESSAGE_MAP(CDeckDistriRebarDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckDistriRebarDlg)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_DAN_BERYUK_STYLE, OnSelchangeComboDanBogangArrangeMethod)
	ON_CBN_SELCHANGE(IDC_COMBO_DAN_BERYUK_MAIN_INSTALL, OnSelchangeComboUseJujangRebar)
	ON_CBN_SELCHANGE(IDC_COMBO_JIJUM_MAIN_STYLE, OnSelchangeComboJijumMainStyle)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW_CYCLE, OnSelchangeComboViewCycle)
	ON_BN_CLICKED(IDC_BUTTON_SAMEBY_CURTAP, OnButtonSameByCurTap)
	ON_BN_CLICKED(IDC_BUTTON_RECALC_SHAPE, OnButtonRecalcShape)
	ON_BN_CLICKED(IDC_CHECK_DIA, OnCheckShowDia)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
 	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDistriRebarDlg message handlers

void CDeckDistriRebarDlg::OnPreInitDialog()
{
	InitTabCtrl();
	InitComboCycle();
	m_Grid.InitGrid(toKgPCm2(m_pStd->m_pDeckData->m_SigmaY));

	SetResize(IDC_GROUP, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_A1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_A2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_B1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_B2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC13, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_C1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC14, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_C2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC9, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_DAN_BERYUK_STYLE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC10, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_DAN_BERYUK_MAIN_INSTALL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC11, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_JIJUM_MAIN_STYLE, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_SAMEBY_CURTAP, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_RECALC_SHAPE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_DIA, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_XAXILE_SLIDER, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_SCALE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC12, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_VIEW_CYCLE, SZ_TOP_LEFT, SZ_TOP_LEFT);
}

void CDeckDistriRebarDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CADeckData		*pDeckData  = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CDrawDBBaseRebarJong DBDeckJong(m_pStd->m_pDataManage);
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CDimDBDeckBasePlan	 DBDimDeckPlan(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);
	
	BOOL bShowMark =  pDeckData->m_bShowMark && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag() ? TRUE : FALSE;
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	pDom->SetScaleDim(50);

	long nJongBasePos = GetJongBasePosByCurTab();
	vector<REBAR_SHAPE>* pvShape = pDeckData->GetRebarShapeArrByJongBasePos(nJongBasePos, 0);
	if(pvShape==NULL)	return;
	
	long nShapesu = pvShape->size();
	if(nShapesu==0)		return;

	long nRow = m_Grid.GetFocusCell().row;
	CString sText = m_Grid.GetTextMatrix(nRow, 0);
	if(sText.GetLength()>0)
		sText = sText.GetAt(0);
	long nCycle = atol(sText)-1;	
	if(nCycle<0)	nCycle = 0;

	CDomyun Dom(pDom);

	Dom.SetDirection("TOP");

	if (m_nTab == 0)	// 종단
	{
		Dom.SetScaleDim(20);
		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,MAIN_MEMBER);

		CDRect rect;
		double dMoveY = 0;
		long nJongBasePos = GetJongBasePosByCurTab();

		long nCycleTotal = pDeckData->GetRebarCycleSuByJongBasePos(nJongBasePos);
		long nCycleStt = m_ctrlComboCycle.GetCurSel();
		long nCycleEnd = m_ctrlComboCycle.GetCurSel()+1;
		if (m_ctrlComboCycle.GetCurSel()==m_ctrlComboCycle.GetCount()-1)
		{
			nCycleStt = 0;
			nCycleEnd = nCycleTotal;
		}

		for(long nCycle=nCycleStt; nCycle<nCycleEnd; nCycle++)
		{
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);
			DBDeckJong.DrawBeryukRebarAtJong(&Dom, nJongBasePos, nCycle, TRUE);
			
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
			DBDeckJong.DimBeryukRebarAtJong(&Dom, nJongBasePos, nCycle, TRUE, m_dScaleDim/100);

			if(bShowMark)
				DBDeckJong.DrawMarkBeryukRebarAtJongTotal(&Dom, nCycle, nJongBasePos);

			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			CString szText, szScale;
			szText.Format("%d - CYCLE : %s", nCycle+1, GetCurTabName());

			DrawStd.AddTitleOnDom(&Dom, 80, rect, szText, TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
			if(nCycle>0)
				dMoveY -= rect.Height()*2;
			Dom.Move(0, dMoveY);
			Dom.RedrawByHVScale(m_nScaleGrade/20.0, TRUE, FALSE, FALSE);

			*pDom << Dom;
		}

		if(pDB->m_nQtyJigan > 1)
		{
			//  사하중 모멘트도                                                     
			long nG = GetCurGirderNo();
			CDoubleArray dArrMomentX, dArrBeryukX;

			DrawMomentDiagmDeadLoad(&Dom, nG, dArrMomentX);
			BOOL bOpenOutput = dArrMomentX.GetSize() > pDB->m_nQtyJigan+1 ? TRUE : FALSE;
			// 제목
			CString sName;
			sName.Format("고정하중 모멘트도 - 거더 %d(중심)", nG+1);
			Dom.SetCalcExtRect();
			CDRect rectMo = Dom.GetExtRect();
			DrawStd.AddTitleOnDom(&Dom, 80, rectMo, sName, TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
			Dom.RedrawByHVScale(m_nScaleGrade/20.0, TRUE, FALSE, FALSE);
			if(bOpenOutput)		Dom.Move(0, dMoveY-rect.Height()-rectMo.Height());  //1.5
			else				Dom.Move(0, dMoveY-rect.Height()-rectMo.Height());
			*pDom << Dom;
			
			if (bOpenOutput)
			{
				// 위치수정
				dArrMomentX.Sort();
				dArrMomentX.RemoveAt(0);	// 부모멘트 시종점 위치 제거
				dArrMomentX.RemoveAt(dArrMomentX.GetUpperBound());
				CPlateBxFinder Finder(pDB->GetGirder(nG));
				CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
				long n = 0;
				for (n=0; n<dArrMomentX.GetSize(); n++)
					dArrMomentX[n] = dArrMomentX[n] - pBxStt->GetStation() + pDB->m_dLengthSlabGirderStt;

				// 모멘트도와 배력철근의 가이드라인
				double dYUpper = nCycleStt==0 ? 0 : dMoveY;
				double dYCenter= dMoveY-rect.Height()*1.3;
				double dYLower = dMoveY-rect.Height()-rectMo.Height();//dMoveY-rect.Height()*2-rectMo.Height();

				CPlateGirderApp *pGirCen = pDB->GetGirder(-1);
				double dStaSttSlabCen = pGirCen->GetSSS(OFF_STT_SLAB);
				double dDis = pDeckData->GetDistByJongBase(nJongBasePos);	// 거더 중심에서 이격거리
				double dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB), dDis, pGirCen->GetAAA(OFF_STT_SLAB));

				for (n=0; n<pDB->m_nQtyJigan+1; n++)
				{
					CPlateBasicIndex *pBx = pGirCen->GetBxOnJijum(n);
					double dStaJijim = pGirCen->GetStationBySc(pBx->GetStation(), dDis, pBx->GetAngle());
					double dLJijim   = pGirCen->GetLengthDimType(dStaStt, dStaJijim, dDis);
					dArrBeryukX.Add(dLJijim);
				}
				
				CDoubleArray dArrTemp;
				for (n=0; n<pDB->m_nQtyJigan-1; n++)
				{
					dArrTemp.Add(dArrBeryukX[n+1] - fabs(dArrMomentX[n*3+2]-dArrMomentX[n*3+1]));
					dArrTemp.Add(dArrBeryukX[n+1] + fabs(dArrMomentX[n*3+3]-dArrMomentX[n*3+2]));
				}

				for (n=0; n<dArrTemp.GetSize(); n++)
					dArrBeryukX.Add(dArrTemp[n]);
				
				dArrBeryukX.Sort();
				m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_DASH);
				m_pStd->m_pDataManage->GetOptionStd()->SetEnvLType(&Dom, LT_DASH_LINE);
				
				for (n=0; n<dArrBeryukX.GetSize(); n++)
				{
					Dom.MoveTo(CDPoint(dArrBeryukX[n], dYUpper));
					Dom.LineTo(CDPoint(dArrBeryukX[n], dYCenter));
					Dom.LineTo(CDPoint(dArrMomentX[n], dYCenter));
					Dom.LineTo(CDPoint(dArrMomentX[n], dYLower));
				}
				Dom.RedrawByHVScale(m_nScaleGrade/20.0, TRUE, FALSE, FALSE);
			}
			*pDom << Dom;
		}		
	}
	else
	{
		Dom.SetScaleDim(100);
		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,MAIN_MEMBER);

		CPlateGirderApp	*pGir   = pDB->GetGirder(-1);
		CPlateBxFinder Finder(pGir);	
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_SLAB);

		DBPlan.DrawSlabTotal(&Dom);
		DBPlan.DrawCenterLine(&Dom, -1, pBxStt->GetStation(), pBxEnd->GetStation());
		DBPlan.DrawTaperLinePyung(&Dom);			// Taper 선 그리기

		DBPlan.DrawAbutPierCenterLine(&Dom);               //교대교각중심선
		DBPlan.DrawAbutPierSymbol(&Dom, CENTERGIRDER);
		
		CDomyun ReBarDom(&Dom);
		if(m_nTab==1)	// 상부
		{
			// 신축이음부
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_CONC);
			DBPlan.DrawExpJointLine(&Dom, TRUE,  -1);
			DBPlan.DrawExpJointLine(&Dom, TRUE,   1);	
			DBPlan.DrawExpJointLine(&Dom, FALSE, -1);
			DBPlan.DrawExpJointLine(&Dom, FALSE,  1);	
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);			
			Dom.SetLineColor(6);
			DBDeckPlan.DrawBeryukRebarOnJijum(&Dom, pBxStt, pBxEnd, TRUE, DRAW_SIDEALL);
//			DBDeckPlan.DrawBeryukRebarOnJijum(&Dom, pBxStt, pBxEnd, TRUE, DRAW_RIGHTSIDE);	
			Dom.SetLineColor(4);
			DBDeckPlan.DrawBeryukRebarOnCenter(&Dom, pBxStt, pBxEnd, TRUE, DRAW_SIDEALL);
//			DBDeckPlan.DrawBeryukRebarOnCenter(&Dom, pBxStt, pBxEnd, TRUE, DRAW_RIGHTSIDE);					
		}
		else
		{
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);			
			Dom.SetLineColor(6);
			DBDeckPlan.DrawBeryukRebarOnJijum(&Dom, pBxStt, pBxEnd, FALSE, DRAW_SIDEALL);
//			DBDeckPlan.DrawBeryukRebarOnJijum(&Dom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE);	
			Dom.SetLineColor(4);
			DBDeckPlan.DrawBeryukRebarOnCenter(&Dom, pBxStt, pBxEnd, FALSE, DRAW_SIDEALL);
//			DBDeckPlan.DrawBeryukRebarOnCenter(&Dom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE);					
		}
		CAPlateDrawPyung DrawPyung(m_pStd->m_pDataManage);
		double rotAng  = DrawPyung.GetRotateAnglePyung(pBxStt,pBxEnd);

		if(CAPlateDrawPyung::IsInputDomRotate())
			Dom.Rotate(rotAng,0,0);
		// 제목
		CString sName;
		sName = m_nTab==1 ? "평면도(상부배력근)" : "평면도(하부배력근)";
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.SetTextHeight(Dom.Always(0.2));
		Dom.TextOut( (rect.left+rect.right)/2, rect.bottom+Dom.GetTextHeight(), sName);

		*pDom << Dom;
	}
	
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDeckDistriRebarDlg::SetGridData() 
{
	CADeckData			*pDeckData  = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	CDPoint	vAngStt = pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd = pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	BOOL bAllSame = ((vAngStt==vAngEnd) && !pDeckData->IsBridgeHasCurveLine()) ? TRUE : FALSE;
	
	//////////////////////////////////////////////////////////////////////////
	CString sz;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	long nLine = 0;	
	long nJongBasePos = GetJongBasePosByCurTab();
	vector<REBAR_CYCLE>* pvRebarCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos);
	long nCyclesu = pvRebarCycle->size();
	if(nCyclesu==0)		return;
	
	long nShapesuUp = pvRebarCycle->at(0).m_dArrJointUp.GetSize();
	long nShapesuDn = pvRebarCycle->at(0).m_dArrJointDn.GetSize();
	long nRowsu = nCyclesu*2+1;
	long nColsu = nShapesuUp+nShapesuDn+2;
	m_Grid.SetGridDefault(nRowsu, nColsu, 1, 2);

	CString sTabName = GetCurTabName();
	CString szDia;
	double dFy = toKgPCm2(m_pStd->m_pDeckData->m_SigmaY);
	// Title
	m_Grid.AddCell(0, 0, "Cycle");
	m_Grid.AddCell(0, 1, "구분");
	long nCycle = 1;
	for (long nRow=1; nRow<nRowsu; nRow+=2) 
	{
		if(m_bShowDia)		sz.Format("%d\n", nCycle);
		else				sz.Format("%d", nCycle);
		m_Grid.AddCell(nRow,   0, sz);		m_Grid.AddCell(nRow,   1, "길이");	
		m_Grid.AddCell(nRow+1, 0, sz);		m_Grid.AddCell(nRow+1, 1, "직경");
		nCycle++;
	}
	long nJoint = 1;
	long nCol	= 0;
	for (nCol=2; nCol<nShapesuUp+2; nCol++) 
	{
		sz.Format("U%d", nJoint);
		m_Grid.AddCell(0, nCol, sz);	
		nJoint++;
	}
	nJoint = 1;
	for (nCol=nShapesuUp+2; nCol<nShapesuUp+nShapesuDn+2; nCol++) 
	{
		sz.Format("D%d", nJoint);
		m_Grid.AddCell(0, nCol, sz);	
		nJoint++;
	}

	for ( nCycle=0; nCycle<nCyclesu; nCycle++) 
	{
		long nShape = 0;
		for (nShape=0; nShape<nShapesuUp; nShape++)
		{
			if( (nShape-1)%3==0 || (bAllSame&&sTabName=="슬래브우측") || sTabName=="슬래브중앙" || sTabName=="가로보중앙")	//일반부
				nLine = CELL_READONLY;
			else
				nLine = CELL_LINE_NO;

			m_Grid.AddCell(nCycle*2 + 1, nShape+2, &pvRebarCycle->at(nCycle).m_dArrJointUp, nShape, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);// 길이
			szDia.Format("%c%.0lf", GetCharRebarMark(dFy),  pvRebarCycle->at(nCycle).m_dArrDiaUp[nShape]);
			m_Grid.AddCell(nCycle*2 + 2, nShape+2, szDia, nFormat, CELL_READONLY);
		}
	
		for (nShape=nShapesuUp; nShape<nShapesuUp+nShapesuDn; nShape++)
		{
			if( (nShape-1)%3==0 || (bAllSame&&sTabName=="슬래브우측") || sTabName=="슬래브중앙" || sTabName=="가로보중앙")	//일반부
				nLine = CELL_READONLY;
			else
				nLine = CELL_LINE_NO;
			
			m_Grid.AddCell(nCycle*2 + 1, nShape+2, &pvRebarCycle->at(nCycle).m_dArrJointDn, nShape-nShapesuUp, nFormat, nLine);	m_Grid.SetCellDec(CELL_DEC_0);// 길이
			szDia.Format("%c%.0lf", GetCharRebarMark(dFy), pvRebarCycle->at(nCycle).m_dArrDiaDn[nShape-nShapesuUp]);
			m_Grid.AddCell(nCycle*2 + 2, nShape+2, szDia, nFormat, CELL_READONLY);
		}
	}
}

void CDeckDistriRebarDlg::SetDataDefault() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	BOOL bFirst = IsUsedFlag() ? FALSE : TRUE;

	if(bFirst || pDeckData->m_nUseJujangRebar<0)
	{
		pDeckData->m_nTypeDanbuTaperBeryukRebar	= 0;
		pDeckData->m_nUseJujangRebar	= 0;	// 총철근량1/3을 순지간1/16까지 배근
		pDeckData->m_nJijumBogangArrange= 2;	// 지점각과 평행배치
		pDeckData->m_dLengthBeryukSttA  = pDeckData->GetValueSettle(toKgPCm2(pDeckData->m_SigmaCK), toKgPCm2(pDeckData->m_SigmaY), pDeckData->m_dDiaMainRebar_Dan[POS_SLAB_CENTER], JOINT_TENS_NORMAL_B, toCm(pDeckData->m_dCoverUp), toCm(pDeckData->m_dEndSupportRebarCTC[POS_SLAB_CENTER]));
		pDeckData->m_dLengthBeryukSttB  = 30*pDeckData->m_dDiaMainRebar_Dan[POS_SLAB_CENTER];	// 중앙단부 상부 철근 직경의 30배
		pDeckData->m_dLengthBeryukSttC  = 30*pDeckData->m_dDiaMainRebar_Dan[POS_SLAB_CENTER];	// 중앙단부 상부 철근 직경의 30배
		pDeckData->m_dLengthBeryukEndA  = pDeckData->m_dLengthBeryukSttA;
		pDeckData->m_dLengthBeryukEndB  = pDeckData->m_dLengthBeryukSttB;
		pDeckData->m_dLengthBeryukEndC  = pDeckData->m_dLengthBeryukSttC;
		m_nScaleGrade = 3;
	}

	CDoubleArray dArrLeft = GetArrJongMomentPos(0);
	CDoubleArray dArrRigh = GetArrJongMomentPos(pDB->GetGirdersu()-1);
	pDeckData->MakeCycleBeryukRebar(dArrLeft, dArrRigh);
	pDeckData->MakeShapeBeryukRebar();
	
	InitComboCycle();

	m_pStd->m_pDoc->SetModifiedFlag();
}




void CDeckDistriRebarDlg::SetDataInit() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	long nJongBasePos = GetJongBasePosByCurTab();
	vector<REBAR_CYCLE>* pvRebarCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos);
	if(pvRebarCycle->size()==0)		return;
	
	long nCyclesu = pvRebarCycle->size();
	long nRowsu = nCyclesu*3+1;

	SetGridData();

	m_Grid.UpdateData(FALSE);
	if(m_bShowDia)
		m_Grid.MergeGrid(0, nRowsu-1, 0, 1);
	
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.EnableReturnPass();
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	for (long nRow=1; nRow<m_Grid.GetRowCount(); nRow++)
	{
		if(nRow%2==0)
			m_Grid.SetRowHeight(nRow, m_bShowDia ? 20 : 0);
	}
	if(!m_bShowDia)
		m_Grid.SetColumnWidth(1, 0);

	m_Grid.SetRedraw(TRUE, TRUE);
	
	SetViewControl();
	
	if(pDeckData->m_bInstallBeryukRebarAtBlockOut)
	{
		GetDlgItem(IDC_EDIT_A1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_A2)->EnableWindow(FALSE);
		pDeckData->m_dLengthBeryukSttA = 0;
		pDeckData->m_dLengthBeryukEndA = 0;
	}
	else
	{
		GetDlgItem(IDC_EDIT_A1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_A2)->EnableWindow(TRUE);
	}

	if(pDB->m_cSlabThickAddTypeStt==1)
	{
		GetDlgItem(IDC_EDIT_C1)->EnableWindow(TRUE);
	}
	else
	{
		pDeckData->m_dLengthBeryukSttC = 0;
		GetDlgItem(IDC_EDIT_C1)->EnableWindow(FALSE);
	}

	if(pDB->m_cSlabThickAddTypeEnd==1)
	{
		GetDlgItem(IDC_EDIT_C2)->EnableWindow(TRUE);
	}
	else
	{
		pDeckData->m_dLengthBeryukEndC = 0;
		GetDlgItem(IDC_EDIT_C2)->EnableWindow(FALSE);
	}

	m_ctrlComboDanBogangArrangeMethod.SetCurSel(pDeckData->m_nTypeDanbuTaperBeryukRebar);
	m_ctrlComboUseJujangRebar.SetCurSel(pDeckData->m_nUseJujangRebar);
	m_ctrlComboJijumBogangArrange.SetCurSel(pDeckData->m_nJijumBogangArrange);
	m_szA1.Format("%.0lf", pDeckData->m_dLengthBeryukSttA);
	m_szA2.Format("%.0lf", pDeckData->m_dLengthBeryukEndA);
	m_szB1.Format("%.0lf", pDeckData->m_dLengthBeryukSttB);
	m_szB2.Format("%.0lf", pDeckData->m_dLengthBeryukEndB);
	m_szC1.Format("%.0lf", pDeckData->m_dLengthBeryukSttC);
	m_szC2.Format("%.0lf", pDeckData->m_dLengthBeryukEndC);

	if(pDB->m_nQtyJigan>1)	// 연속교
		GetDlgItem(IDC_COMBO_DAN_BERYUK_MAIN_INSTALL)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_COMBO_DAN_BERYUK_MAIN_INSTALL)->EnableWindow(FALSE);
	
	if(pDeckData->IsSkew())
		GetDlgItem(IDC_COMBO_JIJUM_MAIN_STYLE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_COMBO_JIJUM_MAIN_STYLE)->EnableWindow(FALSE);

	UpdateData(FALSE);
}




void CDeckDistriRebarDlg::SetDataSave() 
{
	m_Grid.UpdateData(TRUE);

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	pDeckData->m_nTypeDanbuTaperBeryukRebar = m_ctrlComboDanBogangArrangeMethod.GetCurSel();
	pDeckData->m_nUseJujangRebar = m_ctrlComboUseJujangRebar.GetCurSel();
	pDeckData->m_nJijumBogangArrange = m_ctrlComboJijumBogangArrange.GetCurSel();
	pDeckData->m_dLengthBeryukSttA = atof(m_szA1);
	pDeckData->m_dLengthBeryukEndA = atof(m_szA2);
	pDeckData->m_dLengthBeryukSttB = atof(m_szB1);
	pDeckData->m_dLengthBeryukEndB = atof(m_szB2);
	pDeckData->m_dLengthBeryukSttC = atof(m_szC1);
	pDeckData->m_dLengthBeryukEndC = atof(m_szC2);
}



void CDeckDistriRebarDlg::InitTabCtrl()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	m_ctrlTab.DeleteAllItems();

	m_ctrlTab.InsertItem(0, "슬래브좌측");
	m_ctrlTab.InsertItem(1, "슬래브우측");
	m_ctrlTab.InsertItem(2, "슬래브중앙");
	if(pDB->GetGirdersu()>1)
		m_ctrlTab.InsertItem(3, "가로보중앙");
	
	m_ctrlTab.SetCurSel(0);
}



void CDeckDistriRebarDlg::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;
	
	CString sz  = m_Grid.GetTextMatrix(pGridView->iRow, 0);
	long nCycle = atol(sz)-1;

	if(!m_bResetCombo)
		m_ctrlComboCycle.SetCurSel(nCycle);

	m_bResetCombo = FALSE;

	DrawInputDomyunView(FALSE);
}


void CDeckDistriRebarDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;
	long nRow = pGridView->iRow;
	long nCol = pGridView->iColumn;

	CString szTitle = m_Grid.GetTextMatrix(0, nCol);
	CString szTitleTemp =  m_Grid.GetTextMatrix(0, nCol);

	BOOL    bUpper  = szTitle.Find("U")!=-1 ? TRUE : FALSE;
	szTitle.Delete(0);
	long	nShape  = atol(szTitle)-1;
	CString szCycle = m_Grid.GetTextMatrix(nRow, 0);
	long	nCycle  = atol(szCycle)-1;
	long	nJongBasePos = GetJongBasePosByCurTab();

	if(m_Grid.GetTextMatrix(nRow, 1)=="직경")	// 진입불가능(직경은 READ_ONLY임)
	{
		SetDataSave();
		SetDataInit();
	}
	else
	{
		ReMakeCycleByChangedData(nJongBasePos, nCycle, nShape, bUpper, m_Grid.GetTextMatrixDouble(nRow, nCol));
		SetDataInit();
		SetDataSave();
	}

	pDeckData->MakeShapeBeryukRebar();

	DrawInputDomyunView(FALSE);
}

void CDeckDistriRebarDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetFocus();	
	SetDataInit();

	UpdateData();

	m_Grid.SetFocusCell(1, 2);

	DrawInputDomyunView();
}

void CDeckDistriRebarDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->MakeShapeBeryukRebar();
}


long CDeckDistriRebarDlg::GetTabCount()
{
	return 3;
}

CString CDeckDistriRebarDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0)			return "종단면도";
	else if(nIdx==1)	return "평면도(상부)";
	else				return "평면도(하부)";
}




void CDeckDistriRebarDlg::SetViewControl()
{
	CSliderCtrl *pSlider1 = (CSliderCtrl *) GetDlgItem (IDC_XAXILE_SLIDER);

	if(m_nScaleGrade <= 0 || m_nScaleGrade >= 100)
		m_nScaleGrade = 3;

	pSlider1->SetRange(1, 20, TRUE);
	pSlider1->SetPos(m_nScaleGrade);
	m_szScale.Format("SCALE: 1 / %.2f", double(m_nScaleGrade)/20);

	UpdateData(FALSE);
}



void CDeckDistriRebarDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *pSlider1 = (CSliderCtrl *) GetDlgItem (IDC_XAXILE_SLIDER);
	
	m_nScaleGrade = pSlider1->GetPos();
	m_szScale.Format("SCALE: 1 / %.2f", double(m_nScaleGrade)/20);

	UpdateData(FALSE);

	DrawInputDomyunView(TRUE);

	CGeneralBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CDeckDistriRebarDlg::DrawMomentDiagmDeadLoad(CDomyun *pDom, long nG, CDoubleArray &dArrX)
{
	CPlateBridgeApp		*pDB		= m_pStd->m_pDataManage->GetBridge();	
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CFemStressCalc		StressCalc(m_pStd->m_pDataManage, m_pStd->m_pCalcStd);
	CCalcData           *pCalcData	= m_pStd->m_pDataManage->GetCalcData();
	
	CDomyun		Dom(pDom);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

	double	dStaStt		= pBxStt->GetStation();
	double	dStaEnd		= pBxEnd->GetStation();
	double	dSta		= 0;
	double	dYScale		= 0.01;//MFactor;
	double	dPreValue	= 0;
	double	dPreSta		= 0;
	double	dValue		= 0;
	
	CString cs, cs2;
	CDoubleArray dArrDim;	// 치수선 설정을 위한 스테이션
	CDoubleArray dArrDimEL;	// 치수선 설정(Y값)
	//////////////////////////////////////////////////////////////////////////
	// 기준선 그림.
	Dom.SetDirection("TOP");
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	Dom.MoveTo(dStaStt, 0);
	Dom.LineTo(dStaEnd, 0);
	//////////////////////////////////////////////////////////////////////////
	// 절점번호 그림.
	Dom.SetTextHeight(3.5);
	Dom.SetTextAlignHorz(TA_CENTER);
	long nNode = 1;
	long nJijum = 0;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	while (pBx)   
	{
		dSta = pBx->GetStation();
		cs.Format("%d", nNode++);
		Dom.LineTo(CDPoint(dSta, Dom.Always(5)), CDPoint(dSta, -Dom.Always(5)));
		Dom.TextOut(dSta, Dom.Always(10), cs);
		
		if (pBx->IsJijum())
		{
		//	cs = pDB->m_->m_bscsXxTypeNameForOutByJijumNo[nJijum];
	
			CSymbolDom *pSym = new CSymbolDom(&Dom, m_pStd->m_pDataManage->GetOptionStd());
			pSym = new CSymbolDom(&Dom, m_pStd->m_pDataManage->GetOptionStd());
			pSym->DrawSymbolJijumMark(0, 0, cs, 0, TRUE);
			pSym->SetBlockName(cs);
		
			CObInsert *pIns = new CObInsert;		
			pIns = new CObInsert;		
			pIns->m_sBlockName = cs;
			pIns->m_vPoint = CDPoint(dSta, 0);	
			pIns->m_vSFactor = CVector(1,1,1);
			Dom.AddBlockAndInsert(pSym, pIns);

			dArrDim.Add(dSta);	
			dArrX.Add(dSta);	// 지점위치추가
			nJijum++;
		}
		pBx = Finder.GetBxNext();
	}
	//////////////////////////////////////////////////////////////////////////
	// 사하중(합성전+합성후)모멘트도
	if(pCalcData->m_bMakeModeling==FALSE)
	{
		Dom.Move (-dStaStt, 0);
		*pDom << Dom;
		return;
	}

	long	nIdx	= 0;
	long	nIdxTmp	= 0;
	//////////////////////////////////////////////////////////////////////////
	// Max Moment 그림
	BOOL bMaxSearch = FALSE;
	Dom.SetLineColor(3);

	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	double staINode = pBx->GetStation(), staJNode;
	dPreValue = 0;
	dPreSta = staINode;
	Dom.MoveTo(dPreSta, -dPreValue * dYScale);
	while (pBx = Finder.GetBxNext())  
	{
		staJNode = pBx->GetStation();
		for (long i = 1; i < 5/*MAXMIN_POINT*/; i++)  
		{
			dSta = staINode + (staJNode - staINode) * i / (5/*MAXMIN_POINT*/ - 1);
			dValue = StressCalc.GetElementForce(ELE_MOMENT3,LC_PREV_S, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, dSta, 0);
			if(nIdx >= 80) nIdx =0;
			nIdx ++;
			nIdxTmp++;
			if (dValue > 0.0) 
			{
				if (dPreValue <= 0.0)
				{
					dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
					dArrDim.Add(dPreSta);	// *************************
					Dom.MoveTo(dPreSta, 0.0);
					bMaxSearch = TRUE;
				}
				if (bMaxSearch && dValue < dPreValue)  
				{  // 첫 감소구간
					if (dSta >= dStaStt && dSta <= dStaEnd) 
					{
						Dom.MoveTo(dPreSta, 0.0);
						Dom.LineTo(dPreSta, -dPreValue * dYScale);
						CDPoint P1(dPreSta, -dPreValue * dYScale * 0.5);
						cs.Format ("Max(+) = %.1f", dPreValue);
						Dom.TextOut (P1, cs);
						dArrDimEL.Add(-dPreValue * dYScale);
					}
					bMaxSearch = FALSE;
				}
				if (dSta <= dStaStt)
					Dom.MoveTo(dSta, -dValue * dYScale);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dSta, -dValue * dYScale);
			}
			else if (dPreValue > 0.0)  
			{
				dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dPreSta, 0.0);
			}
			dPreValue = dValue;
			dPreSta  = dSta;
		}
		staINode = staJNode;
	}

	//////////////////////////////////////////////////////////////////////////
	// Min Moment 그림
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	staINode = pBx->GetStation(), staJNode;
	dPreValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, staINode, 0)
				+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, staINode, 0)
				+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, staINode, 0);
	nIdx		= 0;
	nIdxTmp		= 0;
	dPreSta = staINode;
	while (pBx = Finder.GetBxNext())  
	{
		staJNode = pBx->GetStation();
		for (long i = 1; i < 5; i++) 
		{
			dSta = staINode + (staJNode - staINode) * i / 4;
			dValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, dSta, 0);
			if(nIdx >= 80) nIdx = 0;
			nIdx++;
			nIdxTmp++;

			long nPier = pGir->GetNumberJiganByStation(dPreSta);
			if ((nPier == 0 && dValue > dPreValue) || (nPier == pDB->m_nQtyJigan - 1 && dValue < dPreValue))  
			{
				dPreValue = dValue;
				dPreSta   = dSta;
				continue;   // 거더의 첫 부분과 끝부분에서 발생하는 - Moment는 처리안함.
			}
			if (dValue < 0.0)  
			{
				if (dPreValue >= 0.0) 
				{
					dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
					dArrDim.Add(dPreSta);
					Dom.MoveTo(dPreSta, 0.0);
				}
				if (dSta <= dStaStt)
					Dom.MoveTo(dSta, -dValue * dYScale);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dSta, -dValue * dYScale);
				if (i == 4 && pBx->IsJijum()) 
				{
					if (dSta >= dStaStt && dSta <= dStaEnd) 
					{
						Dom.SetLineColor(6);
						Dom.MoveTo(dSta, 0.0);
						Dom.LineTo(dSta, -dValue * dYScale);
						CDPoint P1(dSta, -dValue * dYScale * 0.4);
						cs.Format("Max(-) = %.1f", dValue);
						Dom.TextOut(P1, cs);
						Dom.SetLineColor(3);
						dArrDimEL.Add(-dValue * dYScale);
					}
				}
			}
			else if (dPreValue < 0.0)  
			{
				dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dPreSta, 0.0);
			}
			dPreValue = dValue;
			dPreSta   = dSta;
		}
		staINode = staJNode;
	}

	//////////////////////////////////////////////////////////////////////////
	// 지점에서 교차점까지의 치수선
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);

	dArrDim.Sort();
	dArrDimEL.Sort(FALSE);

	if(dArrDimEL.GetSize()<=0)
	{
		Dom.Move (-dStaStt, 0);
		*pDom << Dom;
		return;
	}
	
	Dom.DimMoveTo(dArrDim.GetAt(0), 0, 0);
	for(long n=1; n<dArrDim.GetSize(); n++)
	{
		if(n==1)
		{
			double dL = dArrDim.GetAt(1)-dArrDim.GetAt(0);
			if(dL>500)	// 시점부 부모멘트 제거위해서...
				Dom.DimLineTo(dL, 0, COMMA(dL));
		}
		else
		{
			double dL = dArrDim.GetAt(n)-dArrDim.GetAt(n-1);
			if(dL>500)	// 시점부 부모멘트 제거위해서...
				Dom.DimLineTo(dL, 0, COMMA(dL));
		}
	}

	// 모멘트변화구간추가
	for(long i = 0; i < dArrDim.GetSize(); i+=3)
	{
		if(dArrDim.GetSize() > i + 1)	dArrX.Add(dArrDim.GetAt(i+1));
		if(dArrDim.GetSize() > i + 2)	dArrX.Add(dArrDim.GetAt(i+2));
		
	}

	//////////////////////////////////////////////////////////////////////////
	// 도면이동
	Dom.Move (-dStaStt+pDB->m_dLengthSlabGirderStt, 0);

	*pDom << Dom;
}



CDoubleArray CDeckDistriRebarDlg::GetArrJongMomentPos(long nG)
{
	CPlateBridgeApp		*pDB		= m_pStd->m_pDataManage->GetBridge();	
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CFemStressCalc		StressCalc(m_pStd->m_pDataManage, m_pStd->m_pCalcStd);
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

	double	dSta		= 0;
	double	dValue		= 0;

	CString cs, cs2;
	CDoubleArray dArrDim;	// 치수선 설정을 위한 스테이션
	//////////////////////////////////////////////////////////////////////////
	// Max Moment 그림
	BOOL bMaxSearch = FALSE;

	CPlateBasicIndex *pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	double staINode		= pBx->GetStation();
	double staJNode		= 0;
	double dPreValue	= 0;
	double dPreSta		= staINode;
	
	while (pBx = Finder.GetBxNext())  
	{
		staJNode = pBx->GetStation();
		for (long i = 1; i < 5/*MAXMIN_POINT*/; i++)  
		{
			dSta = staINode + (staJNode - staINode) * i / 4;
			dValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, dSta, 0);
			if (dValue > 0.0) 
			{
				if (dPreValue <= 0.0)
				{
					dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
					dArrDim.Add(dPreSta);	
					bMaxSearch = TRUE;
				}
				if (bMaxSearch && dValue < dPreValue)  
					bMaxSearch = FALSE;
			}
			else if (dPreValue > 0.0)  
				dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);

			dPreValue = dValue;
			dPreSta	  = dSta;
		}
		staINode = staJNode;
	}
	//////////////////////////////////////////////////////////////////////////
	// Min Moment 그림
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	staINode = pBx->GetStation();
	dPreValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, staINode, 0)
				+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, staINode, 0)
				+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, staINode, 0);
	dPreSta = staINode;
	while (pBx = Finder.GetBxNext())  
	{
		staJNode = pBx->GetStation();
		for (long i = 1; i < 5/*MAXMIN_POINT*/; i++) 
		{
			dSta = staINode + (staJNode - staINode) * i / (5/*MAXMIN_POINT*/ - 1);
			dValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, dSta, 0);

			long nPier = pGir->GetNumberJiganByStation(dPreSta);
			if ((nPier == 0 && dValue > dPreValue) || (nPier == pDB->m_nQtyJigan - 1 && dValue < dPreValue))  
			{
				dPreValue = dValue;
				dPreSta   = dSta;
				continue;   // 거더의 첫 부분과 끝부분에서 발생하는 - Moment는 처리안함.
			}
			if (dValue < 0.0)  
			{
				if (dPreValue >= 0.0) 
				{
					dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
					dArrDim.Add(dPreSta);	// *************************
				}
			}
			else if (dPreValue < 0.0)  
				dPreSta = StressCalc.Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);

			dPreValue = dValue;
			dPreSta   = dSta;
		}
		staINode = staJNode;
	}

	pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while (pBx)  
	{
		if(pBx->IsJijum() && pBx->IsDanbuJijumStt()==FALSE)
		{
			dArrDim.Add(pBx->GetStation());
		}
		pBx = Finder.GetBxNext();
	}

	dArrDim.Sort();

	return dArrDim;
}


BOOL CDeckDistriRebarDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}


// 단부 Taper 위치 배력철근 형상
// 0 : 헌치부에서 정착길이만큼 연장
// 1 : 단부측면 피복위치까지 연장
void CDeckDistriRebarDlg::OnSelchangeComboDanBogangArrangeMethod() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	pDeckData->m_nTypeDanbuTaperBeryukRebar	 = m_ctrlComboDanBogangArrangeMethod.GetCurSel();

	pDeckData->MakeShapeBeryukRebar();

	SetDataInit();
	InitComboCycle();

	DrawInputDomyunView(TRUE);
}


// 주장율철근 설치옵션
// 0 : 총철근량1/3을 순지간1/16까지 배근
// 1 : 총철근량1/2을 순지간1/16까지 배근
void CDeckDistriRebarDlg::OnSelchangeComboUseJujangRebar() 
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	pDeckData->m_nUseJujangRebar = m_ctrlComboUseJujangRebar.GetCurSel();
	
	CDoubleArray dArrLeft = GetArrJongMomentPos(0);
	CDoubleArray dArrRigh = GetArrJongMomentPos(pDB->GetGirdersu()-1);
	pDeckData->MakeCycleBeryukRebar(dArrLeft, dArrRigh);
	pDeckData->MakeShapeBeryukRebar();

	SetDataInit();
	InitComboCycle();

	DrawInputDomyunView(TRUE);
}


// [다경간]지점보강 주철근 배치"
// 0: 교축직각 방향
// 1: 지점각과 동일배치
// 2: 지점각과 평행배치
void CDeckDistriRebarDlg::OnSelchangeComboJijumMainStyle() 
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	pDeckData->m_nJijumBogangArrange = m_ctrlComboJijumBogangArrange.GetCurSel();
	
	CDoubleArray dArrLeft = GetArrJongMomentPos(0);
	CDoubleArray dArrRigh = GetArrJongMomentPos(pDB->GetGirdersu()-1);
	pDeckData->MakeCycleBeryukRebar(dArrLeft, dArrRigh);
	pDeckData->MakeShapeBeryukRebar();

	SetDataInit();
	InitComboCycle();

	DrawInputDomyunView(TRUE);
}

void CDeckDistriRebarDlg::OnButtonSameByCurTap() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nJongBasePosCur = GetJongBasePosByCurTab();
	long nCyclesu        = pDeckData->GetRebarCycleSuByJongBasePos(nJongBasePosCur);

	for (long nCycle=0; nCycle<nCyclesu; nCycle++)
	{
		REBAR_CYCLE *pCycleBase = pDeckData->GetRebarCycleByJongBasePos(nJongBasePosCur, nCycle);

		for (long nJongBasePos=JONGBASEPOS_SLABCENTER; nJongBasePos<=JONGBASEPOS_ONCB; nJongBasePos++)
		{
			if(nJongBasePos==nJongBasePosCur)	continue;

			REBAR_CYCLE *pCycleCur = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos, nCycle);
			// Span별 길이를 계산함.
			CDoubleArray dArrSpanUp, dArrSpanDn;
			long nShapeSu = pCycleCur->m_dArrJointUp.GetSize();
			long nShape = 0;
			for (nShape=0; nShape<nShapeSu; nShape+=3)
			{
				dArrSpanUp.Add(pCycleCur->m_dArrJointUp[nShape]+pCycleCur->m_dArrJointUp[nShape+1]+pCycleCur->m_dArrJointUp[nShape+2]);
			}
			nShapeSu = pCycleCur->m_dArrJointDn.GetSize();
			for (nShape=0; nShape<nShapeSu; nShape+=3)
			{
				dArrSpanDn.Add(pCycleCur->m_dArrJointDn[nShape]+pCycleCur->m_dArrJointDn[nShape+1]+pCycleCur->m_dArrJointDn[nShape+2]);
			}
			// Span의 양측이음길이를 넘겨줌
			nShapeSu = pCycleBase->m_dArrJointUp.GetSize();
			for (nShape=0; nShape<nShapeSu; nShape++)
			{
				if((nShape-1)%3 != 0)
				{
					pCycleCur->m_dArrJointUp.SetAt(nShape, pCycleBase->m_dArrJointUp[nShape]);
					pCycleCur->m_dArrJointDn.SetAt(nShape, pCycleBase->m_dArrJointDn[nShape]);
				}
			}
			// Span의 양측이음길이를 재계산함.
			for (nShape=0; nShape<nShapeSu; nShape+=3)
			{
				long nSpan = long(nShape/3);

				double dNewLengthUp = dArrSpanUp[nSpan]-pCycleCur->m_dArrJointUp[nShape]-pCycleCur->m_dArrJointUp[nShape+2];
				double dNewLengthDn = dArrSpanDn[nSpan]-pCycleCur->m_dArrJointDn[nShape]-pCycleCur->m_dArrJointDn[nShape+2];
				pCycleCur->m_dArrJointUp.SetAt(nShape+1, dNewLengthUp);
				pCycleCur->m_dArrJointDn.SetAt(nShape+1, dNewLengthDn);
			}
		}
	}	
	pDeckData->MakeShapeBeryukRebar();
	
	SetDataInit();

	DrawInputDomyunView();
}

void CDeckDistriRebarDlg::OnButtonRecalcShape() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	long nCycleSu = pDeckData->GetRebarCycleSuByJongBasePos(JONGBASEPOS_SLABLEFT);		// 각 종단위치 모두 동일함.

	for (long nCycle=0; nCycle<nCycleSu; nCycle++)
	{
		for (long nJongBasePos=JONGBASEPOS_SLABCENTER; nJongBasePos<=JONGBASEPOS_ONCB; nJongBasePos++)
		{
			REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos, nCycle);
			pRebarCycle->m_dArrDiaUp.RemoveAll();
			pRebarCycle->m_dArrDiaDn.RemoveAll();
			long nPos = 0;
			if(nJongBasePos==JONGBASEPOS_SLABCENTER)	nPos = POS_SLAB_CENTER;
			if(nJongBasePos==JONGBASEPOS_SLABLEFT)		nPos = POS_SLAB_LEFT;
			if(nJongBasePos==JONGBASEPOS_SLABRIGHT)		nPos = POS_SLAB_RIGH;
			if(nJongBasePos==JONGBASEPOS_ONCB)			nPos = POS_SLAB_LEFT;

			for (long n=0; n<pRebarCycle->m_dArrJointUp.GetSize(); n++)	
			{
				if(n==0 || n==pRebarCycle->m_dArrJointUp.GetUpperBound())	// 단부
				{
					pRebarCycle->m_dArrDiaUp.Add(pDeckData->m_dDiaBeryukRebar_Dan[nPos]);
					pRebarCycle->m_dArrDiaDn.Add(pDeckData->m_dDiaBeryukRebar_Dan[nPos]);
				}
				else if( (n-1)%3==0)	// 일반부
				{
					pRebarCycle->m_dArrDiaUp.Add(pDeckData->m_dDiaBeryukRebar_Cent[nPos]);
					pRebarCycle->m_dArrDiaDn.Add(pDeckData->m_dDiaBeryukRebar_Cent[nPos]);
				}
				else					// 지점부 
				{
					pRebarCycle->m_dArrDiaUp.Add(pDeckData->m_dDiaJijumRebarUp[nPos]);
					pRebarCycle->m_dArrDiaDn.Add(pDeckData->m_dDiaJijumRebarDn[nPos]);
				}
			}
		}
	}
	
	pDeckData->MakeShapeBeryukRebar();

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(TRUE);	
}

void CDeckDistriRebarDlg::OnCheckShowDia() 
{
	UpdateData(TRUE);

	SetDataInit();	
}


void CDeckDistriRebarDlg::OnSelchangeComboViewCycle() 
{
	DrawInputDomyunView(TRUE);
	
}



long CDeckDistriRebarDlg::GetJongBasePosByCurTab()
{
	if		(GetCurTabName()=="슬래브좌측")	return JONGBASEPOS_SLABLEFT;
	else if (GetCurTabName()=="슬래브우측")	return JONGBASEPOS_SLABRIGHT;
	else if	(GetCurTabName()=="슬래브중앙")	return JONGBASEPOS_SLABCENTER;	
	else if	(GetCurTabName()=="가로보중앙")	return JONGBASEPOS_ONCB;	

	return -1;

}


void CDeckDistriRebarDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	CString str1 = m_Grid.GetItemText(0, nCol);
	CString str2 = m_Grid.GetItemText(nRow, 0);
	CString str  = str1+"-"+str2;

	m_pView->GetDomyun()->STMakeCursor(str);
}




CString CDeckDistriRebarDlg::GetCurTabName()
{
	CString sText;
	TCITEM item ;
	char cTableName[256];
	item.mask = TCIF_TEXT;
	item.pszText = cTableName;
	item.cchTextMax = 255;

	m_ctrlTab.GetItem(m_ctrlTab.GetCurSel(), &item);
	sText = item.pszText;

	return sText;
}





//추가
void CDeckDistriRebarDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nRow = pGridView->iRow;
    int nCol = pGridView->iColumn;

	InsertJointByGridCol(nCol);

	SetDataInit();

	DrawInputDomyunView(FALSE);
}



//삭제
void CDeckDistriRebarDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)nmgv;

	int nRow = pGridView->iRow;
    int nCol = pGridView->iColumn;	

	DeleteJointByGridCol(nCol);

	SetDataInit();

	DrawInputDomyunView(FALSE);
}



void CDeckDistriRebarDlg::InsertJointByGridCol(long nGridCol)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	long nJongBasePos = GetJongBasePosByCurTab();
	long nCycleSu = pDeckData->GetRebarCycleSuByJongBasePos(nJongBasePos);
	CDPoint xy(0, 0);

	for (long nCycle=0; nCycle<nCycleSu; nCycle++) 
	{
		REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos, nCycle);

		long nSizeUp = pRebarCycle->m_dArrValidUp.GetSize();
		if(nGridCol > nSizeUp)	// 하부이음추가
		{
			double dLength = pRebarCycle->m_dArrJointDn[nGridCol-2-nSizeUp];
			double dDia    = pRebarCycle->m_dArrDiaDn[nGridCol-2-nSizeUp];

			pRebarCycle->m_dArrJointDn[nGridCol-2-nSizeUp] = dLength/2;
			pRebarCycle->m_dArrJointDn.InsertAt(nGridCol-2-nSizeUp, dLength/2);
			pRebarCycle->m_dArrDiaDn.InsertAt(nGridCol-2-nSizeUp, dDia);
			pRebarCycle->m_dArrValidDn.InsertAt(nGridCol-2-nSizeUp, TRUE);
		}
		else				// 상부이음추가
		{
			double dLength = pRebarCycle->m_dArrJointUp[nGridCol-2];
			double dDia    = pRebarCycle->m_dArrDiaUp[nGridCol-2];

			pRebarCycle->m_dArrJointUp[nGridCol-2] = dLength/2;
			pRebarCycle->m_dArrJointUp.InsertAt(nGridCol-2, dLength/2);
			pRebarCycle->m_dArrDiaUp.InsertAt(nGridCol-2, dDia);
			pRebarCycle->m_dArrValidUp.InsertAt(nGridCol-2, TRUE);
		}
	}
}


void CDeckDistriRebarDlg::DeleteJointByGridCol(long nGridCol)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	long nJongBasePos = GetJongBasePosByCurTab();
	long nCycleSu = pDeckData->GetRebarCycleSuByJongBasePos(nJongBasePos);
	CDPoint xy(0, 0);

	for (long nCycle=0; nCycle<nCycleSu; nCycle++) 
	{
		REBAR_CYCLE* pRebarCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos, nCycle);

		long nSizeUp = pRebarCycle->m_dArrValidUp.GetSize();
		long nSizeDn = pRebarCycle->m_dArrValidDn.GetSize();
		
		double dDis1 = 0;
		double dDis2 = 0;
		if(nGridCol > nSizeUp+1)	// 하부이음삭제 
		{
			if(nSizeDn==2)
			{
				pRebarCycle->m_dArrJointDn.RemoveAt(nGridCol-2-nSizeUp);
				pRebarCycle->m_dArrDiaDn.RemoveAt(nGridCol-2-nSizeUp);
				pRebarCycle->m_dArrValidDn.RemoveAt(nGridCol-2-nSizeUp);
			}
			else if(nSizeDn>2 && nGridCol==nSizeUp+nSizeDn)
			{
				dDis1	= pRebarCycle->m_dArrJointDn.GetAt(nGridCol-2-nSizeUp);
				dDis2	= pRebarCycle->m_dArrJointDn.GetAt(nGridCol-3-nSizeUp);

				pRebarCycle->m_dArrJointDn.RemoveAt(nGridCol-2-nSizeUp);
				pRebarCycle->m_dArrDiaDn.RemoveAt(nGridCol-2-nSizeUp);
				pRebarCycle->m_dArrValidDn.RemoveAt(nGridCol-2-nSizeUp);

				pRebarCycle->m_dArrJointDn[nGridCol-3-nSizeUp] = dDis1+dDis2;
			}
			else
			{
				dDis1	= pRebarCycle->m_dArrJointDn.GetAt(nGridCol-2-nSizeUp);
				dDis2	= pRebarCycle->m_dArrJointDn.GetAt(nGridCol-1-nSizeUp);

				pRebarCycle->m_dArrJointDn.RemoveAt(nGridCol-2-nSizeUp);
				pRebarCycle->m_dArrDiaDn.RemoveAt(nGridCol-2-nSizeUp);
				pRebarCycle->m_dArrValidDn.RemoveAt(nGridCol-2-nSizeUp);

				pRebarCycle->m_dArrJointDn[nGridCol-2-nSizeUp] = dDis1+dDis2;
			}
		}
		else				// 상부이음삭제
		{
			if(nSizeUp==2)	// 이음이 하나일때
			{
				pRebarCycle->m_dArrJointUp.RemoveAt(nGridCol-2);
				pRebarCycle->m_dArrDiaUp.RemoveAt(nGridCol-2);
				pRebarCycle->m_dArrValidUp.RemoveAt(nGridCol-2);
			}
			else if(nSizeUp>2 && nGridCol==nSizeUp)	// 마지막 이음위치
			{
				dDis1	= pRebarCycle->m_dArrJointUp.GetAt(nGridCol-2);
				dDis2	= pRebarCycle->m_dArrJointUp.GetAt(nGridCol-3);

				pRebarCycle->m_dArrJointUp.RemoveAt(nGridCol-2);
				pRebarCycle->m_dArrDiaUp.RemoveAt(nGridCol-2);
				pRebarCycle->m_dArrValidUp.RemoveAt(nGridCol-2);

				pRebarCycle->m_dArrJointUp[nGridCol-3] = dDis1+dDis2;
			}
			else			// 이음이 둘이상일 경우
			{
				dDis1	= pRebarCycle->m_dArrJointUp.GetAt(nGridCol-2);
				dDis2	= pRebarCycle->m_dArrJointUp.GetAt(nGridCol-1);

				pRebarCycle->m_dArrJointUp.RemoveAt(nGridCol-2);
				pRebarCycle->m_dArrDiaUp.RemoveAt(nGridCol-2);
				pRebarCycle->m_dArrValidUp.RemoveAt(nGridCol-2);

				pRebarCycle->m_dArrJointUp[nGridCol-2] = dDis1+dDis2;
			}
		}
	}
}




void CDeckDistriRebarDlg::ReMakeCycleByChangedData(long nJongBasePos, long nCycle, long nShape, BOOL bUpper, double dNewLength)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	CDPoint	vAngStt = pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd = pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);
	BOOL bAllSame = ((vAngStt==vAngEnd) && !pDeckData->IsBridgeHasCurveLine()) ? TRUE : FALSE;

	REBAR_CYCLE *pCycle = pDeckData->GetRebarCycleByJongBasePos(nJongBasePos, nCycle);
	long   nShapeSu = pCycle->m_dArrJointUp.GetSize();
	double dLength  = 0;

	if(nShape==0)	//시점
	{
		if(bUpper)
		{
			dLength = pCycle->m_dArrJointUp[nShape];
			pCycle->m_dArrJointUp[nShape] = dNewLength;
			pCycle->m_dArrJointUp[nShape+1] += dLength-dNewLength;
		}
		else
		{
			dLength = pCycle->m_dArrJointDn[nShape];
			pCycle->m_dArrJointDn[nShape] = dNewLength;
			pCycle->m_dArrJointDn[nShape+1] += dLength-dNewLength;
		}
	}
	else if(nShape==nShapeSu-1)		// 종점
	{
		if(bUpper)
		{
			dLength = pCycle->m_dArrJointUp[nShape];
			pCycle->m_dArrJointUp[nShape] = dNewLength;
			pCycle->m_dArrJointUp[nShape-1] += dLength-dNewLength;
		}
		else
		{
			dLength = pCycle->m_dArrJointDn[nShape];
			pCycle->m_dArrJointDn[nShape] = dNewLength;
			pCycle->m_dArrJointDn[nShape-1] += dLength-dNewLength;
		}
	}
	else if( (nShape+1)%3==0 )		// 지점 전
	{
		if(bUpper)
		{
			dLength = pCycle->m_dArrJointUp[nShape];
			pCycle->m_dArrJointUp[nShape] = dNewLength;
			pCycle->m_dArrJointUp[nShape-1] += dLength-dNewLength;
		}
		else
		{
			dLength = pCycle->m_dArrJointDn[nShape];
			pCycle->m_dArrJointDn[nShape] = dNewLength;
			pCycle->m_dArrJointDn[nShape-1] += dLength-dNewLength;
		}
		
	}
	else if( (nShape+1)%3==1 )		// 지점 후
	{
		if(bUpper)
		{
			dLength = pCycle->m_dArrJointUp[nShape];
			pCycle->m_dArrJointUp[nShape] = dNewLength;
			pCycle->m_dArrJointUp[nShape+1] += dLength-dNewLength;
		}
		else
		{
			dLength = pCycle->m_dArrJointDn[nShape];
			pCycle->m_dArrJointDn[nShape] = dNewLength;
			pCycle->m_dArrJointDn[nShape+1] += dLength-dNewLength;
		}		
	}

	// 슬래브 좌우측면을 기준으로 중앙부의 이음위치를 계산함.
	REBAR_CYCLE *pCycleL = pDeckData->GetRebarCycleByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle);
	REBAR_CYCLE *pCycleR = pDeckData->GetRebarCycleByJongBasePos(JONGBASEPOS_SLABRIGHT, nCycle);
	REBAR_CYCLE *pCycleC = pDeckData->GetRebarCycleByJongBasePos(JONGBASEPOS_SLABCENTER, nCycle);
	REBAR_CYCLE *pCycleCB= pDeckData->GetRebarCycleByJongBasePos(JONGBASEPOS_ONCB, nCycle);

	if(bAllSame)	// 좌측기준으로 우측과 중앙부를 동일적용함..
	{
		pCycleR->m_dArrJointUp = pCycleL->m_dArrJointUp;
		pCycleC->m_dArrJointUp = pCycleL->m_dArrJointUp;
		pCycleCB->m_dArrJointUp = pCycleL->m_dArrJointUp;
		pCycleR->m_dArrJointDn = pCycleL->m_dArrJointDn;
		pCycleC->m_dArrJointDn = pCycleL->m_dArrJointDn;
		pCycleCB->m_dArrJointDn = pCycleL->m_dArrJointDn;
	}
	else
	{
		long nJoint = 0;
		for (nJoint=0; nJoint<pCycleC->m_dArrJointUp.GetSize(); nJoint++) 
		{
			pCycleC->m_dArrJointUp[nJoint] = (pCycleL->m_dArrJointUp[nJoint]+pCycleR->m_dArrJointUp[nJoint])/2;
			pCycleC->m_dArrJointDn[nJoint] = (pCycleL->m_dArrJointDn[nJoint]+pCycleR->m_dArrJointDn[nJoint])/2;
			pCycleCB->m_dArrJointUp[nJoint] = (pCycleL->m_dArrJointUp[nJoint]+pCycleR->m_dArrJointUp[nJoint])/2;
			pCycleCB->m_dArrJointDn[nJoint] = (pCycleL->m_dArrJointDn[nJoint]+pCycleR->m_dArrJointDn[nJoint])/2;
		}
		double dDistL = pDeckData->GetDistByJongBase(JONGBASEPOS_SLABLEFT);
		double dDistR = pDeckData->GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
		double dDistCB = pDeckData->GetDistByJongBase(JONGBASEPOS_ONCB);

		for (nJoint=0; nJoint<pCycleC->m_dArrJointUp.GetSize(); nJoint++) 
		{
			double dJointL = pCycleL->m_dArrJointUp[nJoint];
			double dJointR = pCycleR->m_dArrJointUp[nJoint];
			double dJointC = pDeckData->Bogan(dDistL, dDistR, dJointL, dJointR, dDistCB);
			pCycleCB->m_dArrJointUp.Add(dJointC);

			dJointL = pCycleL->m_dArrJointDn[nJoint];
			dJointR = pCycleR->m_dArrJointDn[nJoint];
			dJointC = pDeckData->Bogan(dDistL, dDistR, dJointL, dJointR, dDistCB);
			pCycleCB->m_dArrJointDn.Add(dJointC);
		}
	}
	
}



void CDeckDistriRebarDlg::InitComboCycle()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	m_ctrlComboCycle.ResetContent();

	long nJongBasePos = GetJongBasePosByCurTab();
	long nCyclesu = pDeckData->GetRebarCycleSuByJongBasePos(nJongBasePos);

	for (long n=0; n<nCyclesu; n++)
	{
		m_ctrlComboCycle.AddString(COMMA(n+1));
	}
	m_ctrlComboCycle.AddString("ALL");
	m_ctrlComboCycle.SetCurSel(nCyclesu);

	m_bResetCombo = TRUE;
}

long CDeckDistriRebarDlg::GetCurGirderNo()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();

	long nJongBasePos = GetJongBasePosByCurTab();
	long nG = 0;
	switch(nJongBasePos) {
	case JONGBASEPOS_SLABCENTER:
		nG = long(pDB->GetGirdersu()/2); 
		break;
	case JONGBASEPOS_SLABLEFT:
		nG = 0;
		break;
	case JONGBASEPOS_SLABRIGHT:
		nG = pDB->GetGirdersu()-1;
		break;
	case JONGBASEPOS_SLABLC:
		nG = 0;
		break;
	case JONGBASEPOS_SLABRC:
		nG = pDB->GetGirdersu()-1;
		break;
	case JONGBASEPOS_ONCB:
		nG = 1;
		break;
	}

	return nG;

}

void CDeckDistriRebarDlg::OnButtonApply() 
{
	UpdateData();

	SetDataSave();
	SetDataInit();

	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	pDeckData->MakeShapeBeryukRebar();
	if(pDeckData->m_bRecalcJewon && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag())
		pDeckData->MakeRebarJewon();

	DrawInputDomyunView();		

}
