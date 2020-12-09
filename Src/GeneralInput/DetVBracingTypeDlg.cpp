// DetVBracingTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetVBracingTypeDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif  

/////////////////////////////////////////////////////////////////////////////
// CDetVBracingTypeDlg dialog
  

CDetVBracingTypeDlg::CDetVBracingTypeDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetVBracingTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetVBracingTypeDlg)
	m_nCurAnchorSu   = 0;		// 앵커수
	m_nCurLevelType  = 0;       // 단면배치방법(0: 수평, 1: 경사)
	m_bEdgeCutting   = FALSE;	// 모서리절단 무
	m_bCutLevel = FALSE;		// 브레이싱 이음판 구배 수평유무
	m_bSameLeftRight = TRUE;    // 좌우측 제원 동일
	m_bTotalView     = FALSE;	// 전체화면 보기
	m_ZzsBoltSize    = "M22";	// 볼트크기
	m_dBoltInterver  = 80;		// 기본볼트간격
	//}}AFX_DATA_INIT
	m_nG = 0;
	m_nPos = 0;
	m_nCurPos = 0;
	m_pVBracingCurrent = NULL;
}


void CDetVBracingTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetVBracingTypeDlg)
	
	DDX_Control(pDX, IDC_CHECK_APPLY_CALC, m_bApplyCalc);
	DDX_Control(pDX, IDC_CROSSBEAM_ANCHORSU, m_ctlAnchorCombo);
	DDX_Control(pDX, IDC_CROSSBEAM_BOLTSIZE_COMBO, m_ctlBoltSizeCombo);
	DDX_Control(pDX, IDC_CUR_CRSBM_COMBO, m_ctlPosCombo);
	DDX_Control(pDX, IDC_COMBO_STEELMATUP, m_comboSteelMatUp);
	DDX_Control(pDX, IDC_COMBO_STEELTYPEUP, m_comboSteelTypeUp);
	DDX_Control(pDX, IDC_COMBO_STEELMATLO, m_comboSteelMatLo);
	DDX_Control(pDX, IDC_COMBO_STEELTYPELO, m_comboSteelTypeLo);
	DDX_Control(pDX, IDC_COMBO_STEELMATDIAGONAL, m_comboSteelMatDiagonal);
	DDX_Control(pDX, IDC_COMBO_STEELTYPEDIAGONAL, m_comboSteelTypeDiagonal);
	DDX_Control(pDX, IDC_CHECK_CUT_LEVEL, m_ctrlCutLevel);
	DDX_Control(pDX, IDC_CHECK_BOLT_BASIC_INTERVER, m_checkBoltInterver);
	DDX_Control(pDX, IDC_CHECK_LEFT_RIGHT_SAME, m_ctrlSameLeftRight);
	DDX_Control(pDX, IDC_EDIT_BOLT_BASIC_INTERVER, m_ctrlEdit1);
	DDX_CBIndex(pDX, IDC_CROSSBEAM_ANCHORSU, m_nCurAnchorSu);
	DDX_Radio(pDX, IDC_RADIO_LRHEIGHT_HORZ, m_nCurLevelType);
	DDX_Check(pDX, IDC_CHECK_CUT_LEVEL, m_bCutLevel);
	DDX_Check(pDX, IDC_CHECK_LEFT_RIGHT_SAME, m_bSameLeftRight);
	DDX_Check(pDX, IDC_CHECK_TOTALVIEW, m_bTotalView);
	DDX_CBString(pDX, IDC_CROSSBEAM_BOLTSIZE_COMBO, m_ZzsBoltSize);
	DDX_Text(pDX, IDC_EDIT_BOLT_BASIC_INTERVER, m_dBoltInterver);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AUTO_BOLTLEN, m_btnBoltLenght);
	DDX_Control(pDX, IDC_BTYPEDLG_USERDESIGN, m_btnUserDesign);
	DDX_Control(pDX, IDC_BUTTON_AUTO_SPLICE, m_btnAutoSplice);	
	DDX_Control(pDX, IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON, m_btnAllGirder);
	DDX_Control(pDX, IDC_BUTTON_ALLDEFAULT, m_btnAllBracingBasic);
	DDX_Control(pDX, IDC_ALLSAMEAPPLY_BUTTON, m_btnSamePosition);
	DDX_Control(pDX, IDC_CROSSBEAM_INPUTCOMPLETE, m_btnAllInput);
}

BEGIN_MESSAGE_MAP(CDetVBracingTypeDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetVBracingTypeDlg)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnDialogNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnDialogPrev)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_SPLICE, OnButtonAutoSpliceCalc)
	ON_BN_CLICKED(IDC_CROSSBEAM_INPUTCOMPLETE, OnBTypeInputcomplete)
	ON_BN_CLICKED(IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON, OnApplyAllGirderSameApplyButton)
	ON_BN_CLICKED(IDC_ALLSAMEAPPLY_BUTTON, OnAllSameApply)
	ON_BN_CLICKED(IDC_RADIO_LRHEIGHT_EQUAL, OnRadioLRHeightEqual)
	ON_BN_CLICKED(IDC_RADIO_LRHEIGHT_HORZ, OnRadioLRHeightHorz)
	ON_BN_CLICKED(IDC_AUTO_BOLTLEN, OnAutoBoltLen)
	ON_CBN_SELCHANGE(IDC_CROSSBEAM_ANCHORSU, OnSelchangeAnchorsu)
	ON_CBN_SELCHANGE(IDC_CROSSBEAM_BOLTSIZE_COMBO, OnSelchangeBoltsize)
	ON_CBN_SELCHANGE(IDC_CUR_CRSBM_COMBO, OnSelchangeCurPos)
	ON_BN_CLICKED(IDC_CHECK_CUT_LEVEL, OnCheckCutLevel)
	ON_BN_CLICKED(IDC_CHECK_TOTALVIEW, OnCheckTotalview)
	ON_BN_CLICKED(IDC_CHECK_LEFT_RIGHT_SAME, OnCheckLeftRightSame)
	ON_BN_CLICKED(IDC_BTYPEDLG_USERDESIGN, OnDlgUserDesign)
	ON_BN_CLICKED(IDC_CHECK_BOLT_BASIC_INTERVER, OnCheckBoltBasicInterver)
	ON_EN_CHANGE(IDC_EDIT_BOLT_BASIC_INTERVER, OnChangeEditBoltBasicInterver)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELTYPEUP, OnSelchangeComboSteeltypeup)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELTYPELO, OnSelchangeComboSteeltypelo)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELTYPEDIAGONAL, OnSelchangeComboSteeltypediagonal)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELMATUP, OnSelchangeComboSteelmatup)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELMATLO, OnSelchangeComboSteelmatlo)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELMATDIAGONAL, OnSelchangeComboSteelmatdiagonal)
	ON_BN_CLICKED(IDC_CHECK_APPLY_CALC, OnCheckApplyCalc)
	ON_BN_CLICKED(IDC_BUTTON_ALLDEFAULT, OnButtonAlldefault)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_MESSAGE(WM_GET_NEARBX, OnClickTotalView)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, STMakeCursor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetVBracingTypeDlg message handlers
void CDetVBracingTypeDlg::OnPreInitDialog()
{
 	SetResize(IDC_GRID,								SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_STATIC_TEXT1,						SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_STATIC_TEXT3,						SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_CUR_CRSBM_COMBO,					SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_CROSSBEAM_ANCHORSU,				SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_CROSSBEAM_BOLTSIZE_COMBO,			SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_CHECK_LEFT_RIGHT_SAME,			SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_RADIO_LRHEIGHT_HORZ,				SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_RADIO_LRHEIGHT_EQUAL,				SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_AUTO_BOLTLEN,						SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_BUTTON_AUTO_SPLICE,				SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_STATIC_GROUP4,					SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_STATIC_GROUP2,					SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_CHECK_BOLT_BASIC_INTERVER,		SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_STATIC_BOLT_BASIC_INTERVER,		SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_EDIT_BOLT_BASIC_INTERVER,			SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON,SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
 	SetResize(IDC_ALLSAMEAPPLY_BUTTON,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
 	SetResize(IDC_CROSSBEAM_INPUTCOMPLETE,			SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_BTYPEDLG_USERDESIGN,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
 	SetResize(IDC_CHECK_TOTALVIEW,					SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_CHECK_APPLY_CALC,					SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_BUTTON_ALLDEFAULT,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetInitPositionCombo();	
	SetInitGita();
	CDataManage *pData = m_pStd->GetDataManage();
	if(pData->GetGlobalOption()->GetBoltLengthAuto())
		GetDlgItem(IDC_AUTO_BOLTLEN)->EnableWindow(FALSE);	
}

BOOL CDetVBracingTypeDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog(); 

	CPlateBridgeApp  *pDB	= m_pStd->GetBridge();
	CPlateGirderApp  *pGir	= pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx	= pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return TRUE;

	if(m_nCurAnchorSu>0)
		SetAnchorsuByBx(pBx);

/*	SetResize(IDC_BTYPEDLG_USERDESIGN,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ALLDEFAULT,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_ALLSAMEAPPLY_BUTTON,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CROSSBEAM_INPUTCOMPLETE,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);		

	SetResize(IDC_STATIC_GROUP2,						SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_TOTALVIEW,						SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	SetResize(IDC_STATIC_GROUP4,						SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_LRHEIGHT_HORZ,					SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_LRHEIGHT_EQUAL,					SZ_TOP_RIGHT, SZ_TOP_RIGHT);
		
	SetResize(IDC_GRID,									SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
*/
	UpdateData(FALSE);

	return TRUE;
}

void CDetVBracingTypeDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;

	UpdateData(TRUE);
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	CString str = m_Grid.GetItemText(0,nCol);

	if( str == "wE" || str == "wF" ) str = "wE@wF";
	if( str == "tE" || str == "tF" ) str = "tE@tF";
	if( str == "tlE" || str == "tlF" ) str = "tlE@tlF";
	if( str == "trE" || str == "trF" ) str = "trE@trF";

	if( str == "sB" || str == "sC" ) str = "sB@sC";
	if( str == "sE" || str == "sF" ) str = "sE@sF";

	
	if( str == "uB" || str == "uC" ) str = "uB@uC";
	if( str == "dB" || str == "dC" ) str = "dB@dC";

	if( str == "vW" || str == "vT" || str == "vL") str = "PL-vW x vT x vL";
	if( str == "vTL") str = "vT";
	if( str == "vHL") str = "vH";

	m_pView->GetDomyun()->STMakeCursor(str);
}

void CDetVBracingTypeDlg::SetBoltCombo(long nG, long nPos)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(nPos >= pDB->GetGirder(m_nG)->GetQtyVBracingOnly())	return;

	UpdateData(TRUE);
}


void CDetVBracingTypeDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);	
  	CARoadOptionStd *pOptStd = m_pStd->m_pDataManage->GetOptionStd();

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	//

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;

	long nG = pBx->GetNumberGirder();
	if(pBx->IsJijum()==FALSE && pDB->IsSafeDiapByCrossBeam(nG*2+3,pBx->GetMatchCol(), TRUE)==FALSE)
	{
		CDomyun Dom(pDom);
		Dom.SetScaleDim(30);
		pOptStd->SetEnvType(&Dom, HCAD_DIML);

		Dom.TextOut(0,0,_T("수직 브레이싱을 설치할 수 없습니다."));
		*pDom << Dom;

		CGeneralBaseDlg::DrawInputDomyunView(TRUE);
		return;
	}

	CDomyun Dom(pDom);
	Dom.SetScaleDim(30);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CAPlateDrawCross DrawCross(m_pStd->GetDataManage());	
	CAPlateDrawDanmyun DrawDanmyun(m_pStd->m_pDataManage);

	CDPoint xyTotalBndLT, xyTotalBndRD;
	if(m_nTab==0)
	{
		CDomyun Dom1(&Dom);
		CDomyun Dom2(&Dom);
		
		DrawCross.DrawInputVBracingFront(&Dom1, pBx);
		Dom1.SetCalcExtRect();
		CDRect BoundRect1 = Dom1.GetExtRect();
		Dom << Dom1;

		DrawCross.DrawInputVBracingPlan(&Dom2, pBx);
		Dom2.SetCalcExtRect();
		CDRect BoundRect2 = Dom2.GetExtRect();
		double dMoveY = BoundRect1.bottom-BoundRect2.top;
		Dom2.Move(0, dMoveY);

		// 전체 도면영역잡기
		Dom2.SetCalcExtRect();
		BoundRect2 = Dom2.GetExtRect();
		
		xyTotalBndLT = BoundRect2.BottomLeft();
		xyTotalBndRD = BoundRect1.TopRight();
		Dom << Dom2;
		*pDom << Dom;
	}
	else
	{
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, TRUE);				
		DrawDanmyun.DrawGuardWall(&Dom, pBx);	// 방호벽
		long nG = 0;
		for(nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(nG));
			DrawCross.DrawHStiffFront(&Dom, pBxMatch, TRUE);	  // 수평보강재
			DrawCross.DrawHStiffFront(&Dom, pBxMatch, FALSE);	  // 수평보강재
		}

		pOptStd->SetEnvType(&Dom, HCAD_DIML);

		DrawCross.DimVBracingFrontSplice(&Dom, pBx, 0);
		DrawCross.DrawVBracingBeamOnCRBx(&Dom, pBx, FALSE);
		DrawCross.DimVBracingFront(&Dom, pBx);
		// 전체 도면영역잡기
		Dom.SetCalcExtRect();
		CDRect BoundRect = Dom.GetExtRect();
		xyTotalBndLT = BoundRect.BottomLeft();
		xyTotalBndRD = BoundRect.TopRight();

		CString strTitle  = _T("");

		for(nG = 0 ; nG < pDB->GetGirdersu()-1; nG++) 
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			double dGirCenL = pGir->m_dGirderCenter;
			pGir = pDB->GetGirder(nG+1);
			double dGirCenR = pGir->m_dGirderCenter;				
			
			strTitle.Format("< G%d-G%d >", nG+1, nG+2);
			Dom.TextOut((dGirCenL+dGirCenR)/2, BoundRect.bottom+Dom.GetTextHeight(), strTitle);
		}


		*pDom << Dom;
	}

	if(m_bTotalView)
	{
		CDomyun Dom;
		CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
		CDomyun *pDomDlg = m_pStd->GetTotalView()->m_DomView.GetDomyun();
		m_pStd->GetTotalView()->Clear();
		DrawPyung.DrawPyungOnAirialView(&Dom);
		DrawPyung.DrawPyungCrossBeamMark(&Dom, pBx);
		Dom.Rotate(CDPoint (0,0), pDB->GetAngleBridgeCenter().GetMirrorVert());
		if(m_pStd->GetTotalView()->GetSafeHwnd()) m_pStd->GetTotalView()->m_DomView.RedrawAll();
		*pDomDlg << Dom;

		m_pStd->ShowTotalView(m_pView);
	}		
	else
		m_pStd->HideTotalView();

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);

}

void CDetVBracingTypeDlg::SetGridCellType(long nRow, long nCol)
{
	if(m_pVBracingCurrent->m_bSameLeftRight)
	{
		m_Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
		m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
	}
	else
	{
		m_Grid.SetItemBkColour(nRow, nCol, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, nCol, GVIS_MODIFIED);
	}
}

void CDetVBracingTypeDlg::SetDataInit()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();	
	CPlateGirderApp		*pGir	= pDB->GetGirder(m_nG);	
	CPlateBasicIndex	*pBx	= pGir->GetBxVBracingOnly(m_nPos);
	
	if(!pBx) return;

	CVBracing			*pBr	= pBx->GetSection()->GetVBracing();

	m_bApplyCalc.SetCheck(pBr->IsSectionCheck());

	SetBoltCombo(m_nG, m_nPos);
	SetInitGita();
	m_Grid.ResetAttrib();
	m_Grid.SetRowCount(GetRowSize());
	m_Grid.SetColumnCount(GetColSize());
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(45);
	m_Grid.SetColumnWidth(0,115);

	SetDataInitCommon();		// 공통
	SetDataInitChordUp();		// 상현재 좌우측
	SetDataInitChordLo();		// 하현재 좌우측
	SetDataInitCent();			// 상현재 또는 하현재 중앙부
	SetDataInitDiagonal();		// 사재 용접부
	SetDataInitAnchor();		// 슬래브 앵커
	m_Grid.SetRedraw(TRUE,TRUE);

	if(pBx)
	{
		CSection	*pSec    = pBx->GetSection();
		CVBracing	*pBr = pSec->GetVBracing();
		
		m_checkBoltInterver.SetCheck(pBr->m_bBoltInterver ? 1 : 0);
		m_dBoltInterver = pBr->m_dBoltInterver;
		GetDlgItem(IDC_EDIT_BOLT_BASIC_INTERVER)->EnableWindow(pBr->m_bBoltInterver);
	}

	SetSteelTypeCombo(STEELMAT_UPPER);
	SetSteelTypeCombo(STEELMAT_LOWER);
	SetSteelTypeCombo(STEELMAT_DIAGONAL);
	SetSteelMaterialCombo(STEELMAT_UPPER);
	SetSteelMaterialCombo(STEELMAT_LOWER);
	SetSteelMaterialCombo(STEELMAT_DIAGONAL);
	UpdateData(FALSE);
}

void CDetVBracingTypeDlg::SetDataDefault()
{	
	long nG, cPos, nPos;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	

	for(nG = 0 ; nG < pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir= pDB->GetGirder(nG);
		cPos = pGir->GetQtyVBracingOnly();
		for(nPos = 0 ; nPos < cPos ; nPos++) 
			SetDataDefaultSub(nG, nPos);
	}
	
	//형강종류 및 제원들은 기본값의 영향을 받지 않는다.(Issue:6674)
	if(!IsUsedFlag())
	{
		SetSteelTypeCombo(STEELMAT_UPPER);
		SetSteelTypeCombo(STEELMAT_LOWER);
		SetSteelTypeCombo(STEELMAT_DIAGONAL);
		SetSteelMaterialCombo(STEELMAT_UPPER);
		SetSteelMaterialCombo(STEELMAT_LOWER);
		SetSteelMaterialCombo(STEELMAT_DIAGONAL);
	}

	UpdateData(FALSE);
}

void CDetVBracingTypeDlg::SetDataDefaultSub(long nG, long nPos)
{
	CDataManage			*pDataMng	= m_pStd->GetDataManage();
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();	
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CPlateGirderApp		*pGirN		= pGir->m_pRight;
	CPlateBasicIndex	*pBx		= pGir->GetBxVBracingOnly(nPos);
	CSteelMaterial		*pMat		= pDataMng->GetSteelMaterial();	
	CSteelBolt			*pBolt		= pDataMng->GetSteelBolt();
	CGlobarOption		*pOpt		= pDataMng->GetGlobalOption();

	if(!pBx)	return;		
	
	BOOL bBoltLengthAuto = pDataMng->GetGlobalOption()->GetBoltLengthAuto();

	CPlateBasicIndex *pBxR	= pBx->GetBxMatchByCrossBeam(pGirN);
	CSection         *pSec	= pBx->GetSection();
	CVBracing        *pBr	= pSec->GetVBracing();

	if(!pBr)	return;

	double dTWeb       = pGir->GetThickFactByStation(pBx->GetStation(), G_W);

	//형강종류 및 제원들은 기본값의 영향을 받지 않는다.(Issue:6674)
	if(!IsUsedFlag())
	{
		CSteelMaterialData	*pMatDataUp			= pBr->GetSteelMaterialData(STEELMAT_UPPER);
		CSteelMaterialData	*pMatDataLo			= pBr->GetSteelMaterialData(STEELMAT_LOWER);
		CSteelMaterialData	*pMatDataDiagonal	= pBr->GetSteelMaterialData(STEELMAT_DIAGONAL);
		
		pMat->SetSteelMaterialDefault(pMatDataUp, CSteelMaterialData::LSTEEL);
		pMat->SetSteelMaterialDefault(pMatDataLo, CSteelMaterialData::LSTEEL);
		pMat->SetSteelMaterialDefault(pMatDataDiagonal, CSteelMaterialData::LSTEEL);

	}
	

	double dHBeamU =  pSec->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
	double dHBeamD =  pSec->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	double dHBeamC =  pSec->GetBeamBaseHeight(CSection::SEC_LEFT, TRUE, TRUE);
	double dWVStiff = pSec->GetVStiff()->m_dWidth[1];
	double dCenBeamU = pSec->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
	double dCenBeamD = pSec->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);

	if(!pOpt->GetInstallBeamAtCenter())
	{
		dCenBeamU = dHBeamU/2;
		dCenBeamD = dHBeamD/2;
	}



	/**************************************************************************/
	/*수직브레이싱 이음판 형태 기본값******************************************/
	/**************************************************************************/
 	pBr->m_dgW		= 20;	// 모따기 모서리여분

	pBr->m_dULG		= 195;	// 상부좌측이음판과 빔의 용접길이
	pBr->m_dURG		= 195;  // 상부우측이음판과 빔의 용접길이
	pBr->m_dDLG		= 195;	// 하부좌측이음판과 빔의 용접길이
	pBr->m_dDRG		= 195;	// 하부우측이음판과 빔의 용접길이
	pBr->m_dC1LG	= -1;//150;	// 빔(사현재)의 용접길이
	pBr->m_dC1RG	= -1;//150;
	pBr->m_dC2LG	= -1;//150;
	pBr->m_dC2RG	= -1;//150;

	pBr->m_nLevelType	= 1;

	// 기타제원 설정
	pBr->m_duE  = 40;
	pBr->m_ddE  = 40;
	pBr->m_dSL  = 80;
	pBr->m_dT   = 15;	
	pBr->m_dS   = dTWeb/2+dWVStiff-(pBr->m_duL3-pBr->m_duE);	
	pBr->m_ZzsBoltSize  = "M22";

	// 수평 브레이싱 연결판
	// 하현
	CHBracingGusset		*pBrRu	= pBx->GetHBracingGusset();	
	CSteelMaterialData	*pSM	= pBx->GetBracing()->GetSteelMaterialData();
	pBrRu->m_dT		= 12;
	pBrRu->m_ddH	= pBx->GetOffsetFromLowerFlange();
	
	CHBracingGusset		*pBrRuR = pBxR->GetHBracingGusset();
	if(pBrRuR)
	{
		pBrRuR->m_dT	= 12;
		pBrRuR->m_ddH	= pBxR->GetOffsetFromLowerFlange();
	}

	// 이음판 상하부 높이 설정 - dCenBeamD:중심선과 부재하단끝까지의 이격거리, 12:수평브레이싱 거세트판의 두께
	if(pBx->GetSection()->IsenType(SECTION_A) || pBx->GetSection()->IsenType(SECTION_P))
	{
		pBr->m_dLH1 = dCenBeamU;
		pBr->m_dRH1 = dCenBeamU;
	}
	else
	{
		pBr->m_dLH1 = 150;
		pBr->m_dRH1 = 150;
	}
	
	//상현재가 ㄷ형강일 경우 기본값을 누르면 아래 루틴이 적용됨(Issue:6674)
	CSteelMaterialData	*pMatDataUp	= pBr->GetSteelMaterialData(STEELMAT_UPPER);
	BOOL				bCSTEEL		= FALSE;
	
	if(pMatDataUp->m_nType == CSteelMaterialData::CSTEEL)
		bCSTEEL = TRUE;

	if(bCSTEEL)
	{
		CSteelMaterialData *pMatData = pBr->GetSteelMaterialData(STEELMAT_UPPER);
		double dHeight  = pMatData->m_dH;
		
		pBr->m_dLH1 = dHeight / 2;
		pBr->m_dRH1 = dHeight / 2;
	}

	CDPoint vBr = pBr->GetVectorFrame(CVBracing::LOWER);
	pBr->m_dLH2 = (pBx->GetOffsetFromLowerFlange()+(pBr->m_dS-dTWeb/2)*fabs(vBr.y)+dCenBeamD/fabs(vBr.x));
	pBr->m_dRH2 = (pBxR->GetOffsetFromLowerFlange()+(pBr->m_dS-dTWeb/2)*fabs(vBr.y)+dCenBeamD/fabs(vBr.x));

	// 앵커위치설정
	if(m_nCurAnchorSu>0)
		SetAnchorsuByBx(pBx);

	// 거더부착 수직보강재 제원 설정
	pBr->m_vW_G  = 150;
	pBr->m_vW1_G = 0;
	pBr->m_vT_G  = 12;

	if(bBoltLengthAuto)	pBr->m_dSL = pBolt->GetBoltLength(pBx, VB_BOLT, pBr->m_ZzsBoltSize, pBr->m_dT + pBr->m_vT_G);
	else				pBr->m_dSL = 80;

	pBr->m_pVStiff->m_dH[LEFT][UP] = 0;
	pBr->m_pVStiff->m_dH[LEFT][DOWN] = 0;
	pBr->m_pVStiff->m_dH[RIGHT][UP] = 0;
	pBr->m_pVStiff->m_dH[RIGHT][DOWN] = 0;

	// 이음판의 크기 및 이음판 볼트제원 설정 
	SetDataSpliceCalc(nG, nPos);
	SetDataVStiffJewon(pBx);

	// 이음판 설정 후 재설정해줌
	pBr->m_dS   = dTWeb/2+dWVStiff-(pBr->m_duL3-pBr->m_duE);
}

void CDetVBracingTypeDlg::SetDataSave()
{
	UpdateData(TRUE);
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(m_nG);	
	CPlateBasicIndex	*pBx	= pGir->GetBxVBracingOnly(m_nPos);
	CVBracing			*pBr	= pBx->GetSection()->GetVBracing();

	pBr->SetSectionCheck(m_bApplyCalc.GetCheck());

	SetDataSaveCommon();
	SetDataSaveChordUp();
	SetDataSaveChordLo();
	SetDataSaveCent();
	SetDataSaveDiagonal();
	SetDataSaveAnchor();
	if(m_checkBoltInterver.GetCheck())
		SetBoltInterver();
	SetDataSaveSteelMaterial();
	UpdateData(FALSE);
}

void CDetVBracingTypeDlg::OnDialogNext() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	UpdateData(TRUE);
	SetDataSave();
	long Index  = m_ctlPosCombo.GetCurSel()+1;

	if((m_nPos+1) >= pDB->GetGirder(m_nG)->GetQtyVBracingOnly()) 
	{
		m_pStd->MoveNextDialog();	
		return;
	}	
	ChangePosIndex(TRUE);	// TRUE : 증가
	m_ctlPosCombo.SetCurSel(Index);
	//
	long nIdx = m_nPos;
	for(long nG = 0; nG < m_nG; nG++)
		nIdx += pDB->GetGirder(nG)->GetQtyVBracingOnly();
	m_ctlPosCombo.SetCurSel(nIdx);
	SetVBracingCurrent();
	//
	SetInitGita();
	SetDataInit();
	SetBoltCombo(m_nG, m_nPos);	
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnDialogPrev() 
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	
	UpdateData(TRUE);
	SetDataSave();
	long Index  = m_ctlPosCombo.GetCurSel()-1;

	if(m_nPos <= 0) 
	{
		m_pStd->GetDetCrossBeamTypeDlg()->m_nPos = 0;
		m_pStd->MovePrevDialog();
		return;
	}
	ChangePosIndex(FALSE);	// FALSE : 감소
	m_ctlPosCombo.SetCurSel(Index);
	//
	long nIdx = m_nPos;
	for(long nG = 0; nG < m_nG; nG++)
		nIdx += pDB->GetGirder(nG)->GetQtyVBracingOnly();
	m_ctlPosCombo.SetCurSel(nIdx);
	SetVBracingCurrent();
	//
	SetInitGita();
	SetDataInit();
	SetBoltCombo(m_nG, m_nPos);	

	DrawInputDomyunView(FALSE);	
}

void CDetVBracingTypeDlg::ChangePosIndex(BOOL bInc)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(bInc) 
	{	//증가
		m_nPos++;
		if(m_nPos >= pDB->GetGirder(m_nG)->GetQtyVBracingOnly()) 
		{
			m_nPos = 0;
			if((m_nG+1) < pDB->GetGirdersu()-1) 
			{
				m_nG++;
			}
		}
	}
	else 
	{	//감소
		m_nPos--;
		if(m_nPos < 0) 
		{
			m_nPos = 0;
			if(m_nG > 0)
			{
				m_nG--;
			}
		}
	}
	SetDlgTitle();
}

void CDetVBracingTypeDlg::SetDlgTitle()
{
	CString szBridgeName = m_pStd->GetBridge()->m_sNameBridge;
	if(!szBridgeName.IsEmpty())
	{
		CString szTitle;		
		szTitle.Format("[Step %d] %s - 수직브레이싱 상세입력 [G%d-G%d-%d]", m_nMenuID - ID_GNINPUT_0, szBridgeName, m_nG+1, m_nG+2, m_nCurPos);//m_nPos+1);	
		SetWindowText(szTitle);
// 		DrawTitleButton();
	}	
}

void CDetVBracingTypeDlg::OnBTypeInputcomplete() 
{
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView(FALSE);

	m_pStd->MoveNextDialog();	
}

void CDetVBracingTypeDlg::OnRadioLRHeightEqual() 
{
	UpdateData();

	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);

	if(pBx==NULL)	return;

	CSection *pSec = pBx->GetSection();
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CVBracing *pBr = NULL;
	//항상 좌측거더를 기준으로 함.
	pBr = pSec->GetVBracing();
	if(A[1].y > A[7].y)	// 좌측거더가 높을 경우
	{
		pBr->m_dLH1 =  pBr->m_dRH1;
		pBr->m_dLH2 =  pBr->m_dRH2;
	}
	else
	{
		pBr->m_dRH1 =  pBr->m_dLH1;
		pBr->m_dRH2 =  pBr->m_dLH2;
	}
	pBr->m_nLevelType = m_nCurLevelType;

	SetDataInit();
	SetDataSave();
	
	DrawInputDomyunView(FALSE);
}


void CDetVBracingTypeDlg::OnRadioLRHeightHorz() 
{
	UpdateData();

	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateGirderApp  *pGirN   = pGir->m_pRight;
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;

	CSection         *pSec   = pBx->GetSection();
	CPlateBasicIndex *pBxN   = pBx->GetBxMatchByCrossBeam(pGirN);
	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CVBracing *pBr = NULL;
	double dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dHeightGirN = pGirN->GetHeightGirderByStaAng(pBxN->GetStation());

	pBr = pSec->GetVBracing();
	
	if(pBr->GetGiulFrameLine(0)!=0)	// 상부제원
	{
		if(A[1].y > A[7].y)	// 좌측거더가 높을 경우
			pBr->m_dLH1 =  pBr->m_dRH1 - pBr->GetGiulFrameLine(0) * pBr->GetLengthFrameLine(CVBracing::UPPER);
		else				// 우측거더가 높을 경우
			pBr->m_dRH1 =  pBr->m_dLH1 + pBr->GetGiulFrameLine(0) * pBr->GetLengthFrameLine(CVBracing::UPPER);
	}

	if(pBr->GetGiulFrameLine(1)!=0)	// 하부제원
	{
		if(A[1].y > A[7].y)	// 좌측거더가 높을 경우
			pBr->m_dLH2 =  dHeightGirN - pBr->m_dLH1 - (dHeightGir-pBr->m_dRH1-pBr->m_dRH2);
		else
			pBr->m_dRH2 =  dHeightGirN - pBr->m_dRH1 - (dHeightGir-pBr->m_dLH1-pBr->m_dLH2);
	}
	
	pBr->m_nLevelType = m_nCurLevelType;

	SetDataInit();
	UpdateData(TRUE);
	SetDataSave();
	
	DrawInputDomyunView(FALSE); 
}


void CDetVBracingTypeDlg::OnAutoBoltLen() 
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp	 *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;

	CSteelBolt  *pBolt = m_pStd->GetBridge()->m_pDataManage->GetSteelBolt();

	CSection *pSec = pBx->GetSection();
	CVBracing *pVr = pSec->GetVBracing();
	
	if(pVr == NULL)	return;
	
	double  T1 = pVr->m_dT + pVr->m_vT_G; //pVr->m_pVStiff->m_dThick; 

	pVr->m_dSL = pBolt->GetBoltLength(pBx, VB_BOLT, pVr->m_ZzsBoltSize,T1);	

	SetDataInit();		
	SetDataSave();

	DrawInputDomyunView(FALSE);	
}

void CDetVBracingTypeDlg::OnSelchangeAnchorsu() 
{
	UpdateData(TRUE);

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx    = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;

	SetAnchorsuByBx(pBx);
	SetDataInit();

	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnSelchangeBoltsize() 
{
	UpdateData(TRUE);
	
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp	 *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;

	CSection *pSec = pBx->GetSection();
	CVBracing       *pVr= NULL;  

	pVr = pSec->GetVBracing();
	pVr->m_ZzsBoltSize = m_ZzsBoltSize;

	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnSelchangeCurPos() 
{
	SetVBracingCurrent();

	SetDataInit();
	SetDlgTitle();
	SetInitGita();

	UpdateData();
	
	DrawInputDomyunView(TRUE);	
}

void CDetVBracingTypeDlg::OnButtonRecommend() 
{
	UpdateData();

	SetDataDefaultSub(m_nG, m_nPos);
	SetInitGita();
	SetDataInit();
	SetSteelTypeCombo(STEELMAT_UPPER);
	SetSteelTypeCombo(STEELMAT_LOWER);
	SetSteelTypeCombo(STEELMAT_DIAGONAL);
	SetSteelMaterialCombo(STEELMAT_UPPER);
	SetSteelMaterialCombo(STEELMAT_LOWER);
	SetSteelMaterialCombo(STEELMAT_DIAGONAL);
	UpdateData(FALSE);
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::SetInitGita()
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp	 *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);

	if(pBx==NULL)	return;

	CSection *pSec = pBx->GetSection();
	CVBracing *pBr = pSec->GetVBracing();
	
	m_nCurLevelType = pBr->m_nLevelType;
	m_ZzsBoltSize   = pBr->m_ZzsBoltSize;
	m_nCurAnchorSu  = pBr->m_nAnchorSu;
	m_bEdgeCutting  = pBr->m_bEdgeCutting;
	m_bCutLevel     = pBr->m_bCutLevel;
	m_bSameLeftRight= pBr->m_bSameLeftRight;

	UpdateData(FALSE);
}

void CDetVBracingTypeDlg::SetInitPositionCombo()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CString strText="";

	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++) 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		int nCount = 1;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			ASSERT(pSec);
			if(pSec)
			{	
				
				if(pSec->GetenDetType()==SECDET_VBRACING)
				{
					strText.Format("G%d-G%d-B%d", nG+1, nG+2, nCount);
					m_ctlPosCombo.AddString(strText);
				}
			}
			pBx = Finder.GetBxNext();
			nCount++;
		}	
	}
	m_ctlPosCombo.SetCurSel(0);

	SetVBracingCurrent();
}

void CDetVBracingTypeDlg::OnApplyAllGirderSameApplyButton() 
{
	CString sz;
	sz.Format("현재 수직브레이싱을 기준으로 동일한 횡단상에 있는 수직브레이싱을 동일적용 합니다.\n계속 하시겠습니까?", m_nG+1);
	if(MessageBox(LPCTSTR(sz), "수직브레이싱 상세 입력 기본값", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);		
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);

	if(pBx==NULL)	return;
	
	SetDataAllGirderSame();
	SetDataInit();
	SetDataSave();

	UpdateData(FALSE);

	DrawInputDomyunView(FALSE);		
}

// 동일구간 동일적용
void CDetVBracingTypeDlg::OnAllSameApply() 
{
	CPlateBridgeApp	*pDB= m_pStd->GetBridge();

	long	nCurType	= m_pVBracingCurrent->GetType();
	long	nVBrCount	= 0;
	CString	szMessage	= _T("");
	CString	szText		= _T("");
	if(nCurType==TYPE_INVVBRACING)		szMessage = "시종점부 : ";
	else if(nCurType==TYPE_VBRACING)	szMessage = "< 일반부 >\n";
	long nG = 0;
	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		nVBrCount = 0;
		long nCount = 1;
		while(pBx)
		{
			CSection	*pSec	= pBx->GetSection();
			CVBracing	*pVBr	= pSec->GetVBracing();
			if(pVBr && pVBr!=m_pVBracingCurrent && pVBr->GetType()==nCurType)
			{
				szText.Format("G%d - G%d : B%d", pBx->GetNumberGirder()+1, pBx->GetNumberGirder()+2, nCount);// nVBrCount+1);
				szMessage += szText;
				nVBrCount++;
				if(nVBrCount%10==0)
					szMessage += "\n";
			}
			pBx = Finder.GetBxNext();
			nCount++;
		}
	}

	if(szMessage==_T(""))
	{
		szMessage = "동일 구간의 같은 타입이 없읍니다.";
		MessageBox(szMessage,"수직브레이싱 상세",MB_ICONINFORMATION );
		return;
	}
	else szMessage += "\n위 수직브레이싱 데이터가 수정됩니다.\n계속 하시겠습니까";
	if( MessageBox(szMessage,"수직브레이싱 상세",MB_ICONQUESTION | MB_YESNO ) == IDNO) return;

	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CSection	*pSec	= pBx->GetSection();
			CVBracing	*pVBr	= pSec->GetVBracing();
			if(pVBr && pVBr!=m_pVBracingCurrent && pVBr->GetType()==nCurType)
			{
				*pVBr <<= *m_pVBracingCurrent;
				pVBr->m_bBoltInterver	= m_pVBracingCurrent->m_bBoltInterver;
				pVBr->m_bEdgeCutting	= m_pVBracingCurrent->m_bEdgeCutting;
				pVBr->m_bSameLeftRight	= m_pVBracingCurrent->m_bSameLeftRight;
				pVBr->m_nLevelType		= m_pVBracingCurrent->m_nLevelType;
				pVBr->SetSteelMaterialData(m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_UPPER), STEELMAT_UPPER);
				pVBr->SetSteelMaterialData(m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_LOWER), STEELMAT_LOWER);
				pVBr->SetSteelMaterialData(m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_DIAGONAL), STEELMAT_DIAGONAL);

				for(long n=0; n<m_pVBracingCurrent->m_nAnchorSu; n++)
					pVBr->m_AnchorDis[n] = m_pVBracingCurrent->m_AnchorDis[n];
			}
			pBx = Finder.GetBxNext();
		}	
	}
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::SetDataAllGirderSame()
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;
	CPlateBasicIndex *pBxMatch= NULL; 

	CVBracing *pVr = pBx->GetSection()->GetVBracing();
	CVBracing *pVrSec = NULL;

	if(pVr == NULL) return;

	for(long nG = 0; nG < pDB->GetGirdersu()-1; nG++) 
	{
		pGir     = pDB->GetGirder(nG);
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);
		pVrSec   = pBxMatch->GetSection()->GetVBracing();

		if(pVrSec == NULL) continue;

		*pVrSec <<= *pVr;

		pVrSec->SetSteelMaterialData(pVr->GetSteelMaterialData(STEELMAT_UPPER), STEELMAT_UPPER);
		pVrSec->SetSteelMaterialData(pVr->GetSteelMaterialData(STEELMAT_LOWER), STEELMAT_LOWER);
		pVrSec->SetSteelMaterialData(pVr->GetSteelMaterialData(STEELMAT_DIAGONAL), STEELMAT_DIAGONAL);

		pVrSec->m_bBoltInterver = pVr->m_bBoltInterver;
		pVrSec->m_bEdgeCutting  = pVr->m_bEdgeCutting;
		pVrSec->m_bSameLeftRight= pVr->m_bSameLeftRight;
		pVrSec->m_nLevelType	= pVr->m_nLevelType;
	
		for(long n=0; n<m_nCurAnchorSu; n++)
			pVrSec->m_AnchorDis[n] = pVr->m_AnchorDis[n];
	}
}

void CDetVBracingTypeDlg::SetDataAllSame()
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;
	CPlateBasicIndex *pBxBType= NULL; 

	CVBracing *pVr    = pBx->GetSection()->GetVBracing();
	CVBracing *pVrSec = NULL;
	
	if(pVr == NULL)		return;

	for(long nG = 0; nG < pDB->GetGirdersu()-1;nG++)
	{
		pGir = pDB->GetGirder(nG);
		pVr  = pGir->GetBxMatchByCrossBeam(pBx, pGir)->GetSection()->GetVBracing();

		if(pVr==NULL)	return;

		for(long nVr=0; nVr<pDB->GetGirder(nG)->GetQtyVBracingOnly(); nVr++)
		{
			pBxBType = pGir->GetBxVBracingByNo(nVr);
			pVrSec   = pBxBType->GetSection()->GetVBracing();
			
			if(pVrSec == NULL) continue;

			*pVrSec <<= *pVr;
			pVrSec->SetSteelMaterialData(pVr->GetSteelMaterialData(STEELMAT_UPPER), STEELMAT_UPPER);
			pVrSec->SetSteelMaterialData(pVr->GetSteelMaterialData(STEELMAT_LOWER), STEELMAT_LOWER);
			pVrSec->SetSteelMaterialData(pVr->GetSteelMaterialData(STEELMAT_DIAGONAL), STEELMAT_DIAGONAL);

			pVrSec->m_bBoltInterver = pVr->m_bBoltInterver;
			pVrSec->m_bEdgeCutting  = pVr->m_bEdgeCutting;
			pVrSec->m_bSameLeftRight= pVr->m_bSameLeftRight;
			pVrSec->m_nLevelType	= pVr->m_nLevelType;
	
			for(long n=0; n<m_nCurAnchorSu; n++)
				pVrSec->m_AnchorDis[n] = pVr->m_AnchorDis[n];

		}
	} 
}	

long CDetVBracingTypeDlg::GetMaxVStiffNum(long nPos)
{
	long Max=0, nNum=0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateCrossBeam *pC      = NULL;

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBx  = pGir->GetBxVBracingOnly(m_nPos);

		if(nPos >= pDB->GetGirder(nG)->GetQtyVBracingOnly()) continue;
		if(pBx==NULL)	continue;

		pC = pBx->GetSection()->GetCrossBeam();

		if(pC == NULL)	continue;
		
		nNum = pC->m_nVStiffsu_CR;
		
		if(nNum > Max) Max = nNum;
	}
	return Max;
}

long CDetVBracingTypeDlg::GetMaxHStiffNum(long nPos)
{
	long Max=0, nNum=0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateCrossBeam *pC      = NULL;

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBx  = pGir->GetBxVBracingOnly(m_nPos);

		if(nPos >= pDB->GetGirder(nG)->GetQtyVBracingOnly()) continue;
		if(pBx==NULL)	continue;

		pC = pBx->GetSection()->GetCrossBeam();

		if(pC == NULL)	continue;

		nNum = pC->m_nHStiffUppersu_CR;
		
		if( nNum > Max ) Max = nNum;
	}

	return Max;
}


long CDetVBracingTypeDlg::GetMaxAnchorNum(long nPos)
{
	long Max=0, nNum=0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CVBracing       *pVr     = NULL;

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBx  = pGir->GetBxVBracingOnly(m_nPos);

		if(nPos >= pDB->GetGirder(nG)->GetQtyVBracingOnly()) continue;
		if(pBx==NULL)	continue;

		pVr = pBx->GetSection()->GetVBracing();

		if(pVr)//수직브레이싱타입
			nNum = pVr->m_nAnchorSu;
		
		if(nNum > Max ) Max = nNum;
	}

	return Max;
}



void CDetVBracingTypeDlg::OnCheckTotalview() 
{
	UpdateData();
	
	DrawInputDomyunView(FALSE);
}


void CDetVBracingTypeDlg::SetAnchorsuByBx(CPlateBasicIndex* pBx)
{
	CSection        *pSec = pBx->GetSection();
	CVector vAng = pBx->GetAngleSection();

	CVBracing* pVr = pSec->GetVBracing();
	if(pVr == NULL)	return;
	pVr->m_nAnchorSu = m_nCurAnchorSu;

	for(long n=0; n<m_nCurAnchorSu; n++)
		pVr->m_AnchorDis[n] = pVr->GetLengthBeam(0)/(m_nCurAnchorSu+1);

}

void CDetVBracingTypeDlg::SetAnchorsuByGirder(long nGir)
{
	UpdateData(TRUE);

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nGir);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{	
		CSection *pSec = pBx->GetSection();
		if(pSec->GetenDetType()==SECDET_VBRACING)
			SetAnchorsuByBx(pBx);

		pBx = Finder.GetBxNext();
	}
}


void CDetVBracingTypeDlg::SetAnchorsuBySection(CPlateBasicIndex* pBx)
{
	UpdateData(TRUE);

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);

	long nGSu = pDB->GetGirdersu();
	pBx = pBx->GetBxMatchByCrossBeam(pGir);//현재 Bx가 첫번째 거더와 match하는 bx
	for(long nG=0; nG<nGSu-1; nG++)
	{
		pGir = pDB->GetGirder(nG);
		SetAnchorsuByBx(pBx);
		pBx = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	}
}

void CDetVBracingTypeDlg::SetAnchorsu()
{
	CPlateBridgeApp* pDB = m_pStd->GetBridge();

	long nGSu = pDB->GetGirdersu();
	for(long nG=0; nG<nGSu-1; nG++)
		SetAnchorsuByGirder(nG);
}

void CDetVBracingTypeDlg::OnCheckLeftRightSame() 
{
	UpdateData(TRUE);

	CPlateBridgeApp  *pDB = m_pStd->GetBridge();	
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);	
	if(pBx==NULL)	return;

	CVBracing *pVr = pBx->GetSection()->GetVBracing();		

	pVr->m_bSameLeftRight = m_bSameLeftRight;

	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnDlgUserDesign() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}
	
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();	
	CPlateBasicIndex *pBx     = pDB->GetGirder(m_nG)->GetBxVBracingOnly(m_nPos);

	if(pBx==NULL)	return;

	CMDIFrameWnd	*pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	CPlateStressOut	pStressOut(m_pStd);
 
	CStringArray Arr;
	CMap <CString, LPCTSTR, double, double> Map;
	pStressOut.SectionCheckOutput(Arr, pBx, Map);
	m_pStd->OutputViewOnAView(Arr);
}


long CDetVBracingTypeDlg::OnClickTotalView(WPARAM wp, LPARAM lp) 
{
	CPlateBasicIndex	*pBx	= (CPlateBasicIndex*)wp;
	if(!pBx)	return 0;
	CSection  *pSec	= pBx->GetSection();
	if(!pSec)	return 0;
	CVBracing *pVr	= pSec->GetVBracing();
	if(!pVr)	return 0;

	if(pSec->GetenDetType()==SECDET_VBRACING)
	{
		if(pVr->GetSelfCount() >= 0 && pBx->GetNumberGirder()< m_pStd->GetBridge()->GetGirdersu()-1)
		{
			m_nG = pBx->GetNumberGirder();
			CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(m_nG);
			long nQtySec = pGir->GetQtyCrossSectionTotal();

			long nSelfSec = 0;
			long nCount = 1;
			for (long n=0; n<nQtySec; n++)
			{
				CSection *pSecCur = pGir->m_CrossSectionArr[n];
				if(pSec==pSecCur)
					break;
				if(pSecCur->IsenDetType(SECDET_VBRACING))
					nSelfSec++;

				nCount++;
			}
			m_nPos = nSelfSec;
			m_nCurPos = nCount;			

			CString strText;
			strText.Format("G%d-G%d-B%d", m_nG+1, m_nG+2, m_nCurPos);
			long nCombo = 0;
			for (nCombo=0; nCombo<m_ctlPosCombo.GetCount(); nCombo++)
			{
				CString rString;
				m_ctlPosCombo.GetLBText(nCombo, rString);
				if(rString==strText)
					break;
			}
			m_ctlPosCombo.SetCurSel(nCombo);

			OnSelchangeCurPos();
		}
	}
	return 0;
}

void CDetVBracingTypeDlg::OnCheckBoltBasicInterver() 
{
	UpdateData(TRUE);

	if(m_checkBoltInterver.GetCheck())
		GetDlgItem(IDC_EDIT_BOLT_BASIC_INTERVER)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT_BOLT_BASIC_INTERVER)->EnableWindow(FALSE);

	SetBoltInterver();
}

void CDetVBracingTypeDlg::SetBoltInterver()
{
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();
	UpdateData(TRUE);

	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++) 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		long nCount = pGir->GetQtyVBracingOnly();

		for(long nPos = 0; nPos < nCount; nPos++)
		{
			CPlateBasicIndex	*pBx  = pGir->GetBxVBracingOnly( nPos);
			if(pBx==NULL) continue;
			CSection *pSec = pBx->GetSection();
			CVBracing *pBr = pSec->GetVBracing();
			if(m_checkBoltInterver.GetCheck())
			{
				pBr->m_dC	= (long)m_dBoltInterver;
				pBr->m_uC	= (long)m_dBoltInterver;
				if(pSec->GetenDetType()==SECDET_VBRACING)
				{
					pBr->m_uB  =3;
					pBr->m_dB  =2;
				}
				else
				{
					pBr->m_uB  =2;
					pBr->m_dB  =3;
				}
			}
			pBr->m_bBoltInterver = m_checkBoltInterver.GetCheck();
			pBr->m_dBoltInterver = m_dBoltInterver;
		}
	}

	SetDataInit();
	UpdateData(FALSE);
}

void CDetVBracingTypeDlg::OnChangeEditBoltBasicInterver() 
{
//	UpdateData(TRUE);
//	if(m_checkBoltInterver.GetCheck())
//		SetBoltInterver();
}

void CDetVBracingTypeDlg::SetGridInit(long nRows, long nCols)
{
	for(long r = 1; r < nRows; r++)
	{
		for(long c = 1; c < nCols; c++)
		{
			m_Grid.SetItemState(r, c, m_Grid.GetItemState(r, c) & ~GVIS_READONLY);
			m_Grid.SetItemBkColour(r, c, RGB(225, 250, 250));
		}
	}
}


void CDetVBracingTypeDlg::SetUnusedGrid(long nG)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);	
	CPlateBasicIndex *pBx     = pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;

	CDataManage *pData = m_pStd->GetDataManage();
	BOOL bBoltLengthAuto = pData->GetGlobalOption()->GetBoltLengthAuto();
	
	long dPlus = GetMaxVStiffNum(m_nPos)+GetMaxAnchorNum(m_nPos);
	if(GetMaxVStiffNum(m_nPos)>0)
		dPlus += 3;
	if(GetMaxHStiffNum(m_nPos) > 0)
		dPlus += 2;	//	hW, hT
	

	//좌우측 제원을 같게 할 경우는 우측제원을 좌측제원에 맞춤.(그리드는 우측제원의 넓이를 0으로 함)
	CVBracing *pVr = pBx->GetSection()->GetVBracing();
	if(pVr->m_bSameLeftRight)
	{
		for(long a=9; a<=16; a++)
		{
			m_Grid.SetItemBkColour(nG+1, a, RGBREADONLY);
			m_Grid.SetItemState(nG+1, a, GVIS_READONLY);
		}
		m_Grid.SetItemState(nG+1, 20, GVIS_READONLY); //P
		m_Grid.SetItemState(nG+1, 27, GVIS_READONLY); //uN
		m_Grid.SetItemState(nG+1, 29, GVIS_READONLY); //dN
		m_Grid.SetItemState(nG+1, 32, GVIS_READONLY); //dN
		m_Grid.SetItemState(nG+1, 33, GVIS_READONLY); //dN	
		m_Grid.SetItemBkColour(nG+1, 20, RGBREADONLY);
		m_Grid.SetItemBkColour(nG+1, 27, RGBREADONLY);
		m_Grid.SetItemBkColour(nG+1, 29, RGBREADONLY);
		m_Grid.SetItemBkColour(nG+1, 32, RGBREADONLY);
		m_Grid.SetItemBkColour(nG+1, 33, RGBREADONLY);
	}
	if(pDB->m_nInstallVStiffOnCrossBeam==0)
	{
		m_Grid.SetItemBkColour(nG+1, 44, RGBREADONLY);
		m_Grid.SetItemState(nG+1, 44, GVIS_READONLY);
	}
	if(bBoltLengthAuto)
	{
		m_Grid.SetItemBkColour(nG+1, 42, RGBREADONLY);
		m_Grid.SetItemState(nG+1, 42, GVIS_READONLY);
	}
}

void CDetVBracingTypeDlg::STMakeCursor(NMHDR* nmgv, LRESULT*)
{
	CDomyun *pDom = m_pView->GetDomyun();

	long	nCol	= m_Grid.GetFocusCell().col;
	long	nRow	= m_Grid.GetFocusCell().row;
	CString	szTitle	= m_Grid.GetItemText(nRow-1,nCol);

	pDom->STMakeCursor(szTitle);
	return;
}

BOOL CDetVBracingTypeDlg::IsFocusItem(CString szTitle)
{
	if(szTitle.Find("<")!=-1)	return TRUE;
	if(szTitle=="uL1")	return TRUE;	if(szTitle=="uL2")	return TRUE;
	if(szTitle=="uL3")	return TRUE;	if(szTitle=="uL4")	return TRUE;
	if(szTitle=="uLG")	return TRUE;	if(szTitle=="uB@uC")return TRUE;
	if(szTitle=="uE")	return TRUE;	if(szTitle=="uR1")	return TRUE;
	if(szTitle=="uR2")	return TRUE;	if(szTitle=="uR3")	return TRUE;
	if(szTitle=="uR4")	return TRUE;	if(szTitle=="uRG")	return TRUE;
	if(szTitle=="dL1")	return TRUE;	if(szTitle=="dL2")	return TRUE;
	if(szTitle=="dL3")	return TRUE;	if(szTitle=="dL4")	return TRUE;
	if(szTitle=="dLG")	return TRUE;	if(szTitle=="dB@dC")return TRUE;
	if(szTitle=="dE")	return TRUE;	if(szTitle=="dR1")	return TRUE;
	if(szTitle=="dR2")	return TRUE;	if(szTitle=="dR3")	return TRUE;
	if(szTitle=="dR4")	return TRUE;	if(szTitle=="dRG")	return TRUE;
	if(szTitle=="gW")	return TRUE;	if(szTitle=="C1")	return TRUE;
	if(szTitle=="C2")	return TRUE;	if(szTitle=="C3")	return TRUE;
	if(szTitle=="C4")	return TRUE;	if(szTitle=="c1LG")	return TRUE;
	if(szTitle=="c1RG")	return TRUE;	if(szTitle=="c2LG")	return TRUE;
	if(szTitle=="c2RG")	return TRUE;	if(szTitle=="A1")	return TRUE;
	if(szTitle=="A2")	return TRUE;	if(szTitle=="A3")	return TRUE;
	if(szTitle=="A4")	return TRUE;	if(szTitle=="A5")	return TRUE;
	if(szTitle=="A6")	return TRUE;

	return CGeneralBaseDlg::IsFocusItem(szTitle);
}

void CDetVBracingTypeDlg::SetFocusItem(CString szTitle)
{

	//////////////////////////////////////////////////////////////////////////
	if(szTitle.Find("<")!=-1)
	{
		CString	szGir1	= _T("");
		CString	szGir2	= _T("");
		CString szCr	= _T("");
		CString szText	= _T("");
		long	nIdx1	= szTitle.Find("G");
		long	nIdx2	= szTitle.Find("-");
		long	nIdx3	= szTitle.Find(">");
		szGir1 = szTitle.Mid(nIdx1+1, nIdx2-nIdx1-1);
		if(szGir1.Find(" ")!=-1)	szGir1.TrimRight();
		m_nG = atoi(szGir1)-1;
		szGir1.Format("G%d", m_nG+1);
		szGir2.Format("G%d", m_nG+2);
		szCr.Format("B%d", m_nPos+1);
		
		szText.Format("%s-%s-%s", szGir1, szGir2, szCr);
		
		for(long nCombo=0; nCombo<m_ctlPosCombo.GetCount(); nCombo++)
		{
			CString szCombo = _T("");
			m_ctlPosCombo.GetLBText(nCombo, szCombo);
			
			if(szCombo.Mid(0,5)==szText.Mid(0,5))
			{
				m_ctlPosCombo.SetCurSel(nCombo);
				UpdateData(FALSE);
				SetVBracingCurrent();
				SetDataInit();
				DrawInputDomyunView(FALSE);
				return;
			}
		}
		
		return;
	}
	
	if(szTitle=="LH1")	{SetGridFocusCell(ROW_COMMON+1, 1);	return;	}	if(szTitle=="LH2")	{SetGridFocusCell(ROW_COMMON+1, 2);	return;	}
	if(szTitle=="RH1")	{SetGridFocusCell(ROW_COMMON+1, 3);	return;	}	if(szTitle=="RH2")	{SetGridFocusCell(ROW_COMMON+1, 4);	return;	}
	if(szTitle=="vWG")	{SetGridFocusCell(ROW_COMMON+1, 5);	return;	}	if(szTitle=="vTG")	{SetGridFocusCell(ROW_COMMON+1, 6);	return;	}
	if(szTitle=="vT")	{SetGridFocusCell(ROW_COMMON+1, 7);	return;	}	if(szTitle=="vHL")	{SetGridFocusCell(ROW_COMMON+1, 8);	return;	}
	if(szTitle=="vHR")	{SetGridFocusCell(ROW_COMMON+1, 9);return;	}	if(szTitle=="S")	{SetGridFocusCell(ROW_COMMON+1, 10);return;	}
	if(szTitle=="T")	{SetGridFocusCell(ROW_COMMON+1, 11);return;	}	if(szTitle=="gW")	{SetGridFocusCell(ROW_COMMON+1, 12);return;	}
	if(szTitle=="sL")	{SetGridFocusCell(ROW_COMMON+1, 13);return;	}

	if(szTitle=="uL1")	{SetGridFocusCell(ROW_CHORDUP+1, 1);return;	}	if(szTitle=="uL2")	{SetGridFocusCell(ROW_CHORDUP+1, 2);	return;	}
	if(szTitle=="uL3")	{SetGridFocusCell(ROW_CHORDUP+1, 3);return;	}	if(szTitle=="uL4")	{SetGridFocusCell(ROW_CHORDUP+1, 4);	return;	}
	if(szTitle=="uLG")	{SetGridFocusCell(ROW_CHORDUP+1, 5);return;	}	if(szTitle=="uB@uC"){SetGridFocusCell(ROW_CHORDUP+1, 6);	return;	}
	if(szTitle=="uE")	{SetGridFocusCell(ROW_CHORDUP+1, 8);return;	}	if(szTitle=="uR1")	{SetGridFocusCell(ROW_CHORDUP+1, 9);	return;	}
	if(szTitle=="uR2")	{SetGridFocusCell(ROW_CHORDUP+1, 10);return;}	if(szTitle=="uR3")	{SetGridFocusCell(ROW_CHORDUP+1, 11);	return;	}
	if(szTitle=="uR4")	{SetGridFocusCell(ROW_CHORDUP+1, 12);return;}	if(szTitle=="uRG")	{SetGridFocusCell(ROW_CHORDUP+1, 13);	return;	}

	if(szTitle=="dL1")	{SetGridFocusCell(ROW_CHORDLO+1, 1);return;	}	if(szTitle=="dL2")	{SetGridFocusCell(ROW_CHORDLO+1, 2);	return;	}
	if(szTitle=="dL3")	{SetGridFocusCell(ROW_CHORDLO+1, 3);return;	}	if(szTitle=="dL4")	{SetGridFocusCell(ROW_CHORDLO+1, 4);	return;	}
	if(szTitle=="dLG")	{SetGridFocusCell(ROW_CHORDLO+1, 5);return;	}	if(szTitle=="dB@dC"){SetGridFocusCell(ROW_CHORDLO+1, 6);	return;	}
	if(szTitle=="dE")	{SetGridFocusCell(ROW_CHORDLO+1, 8);return;	}	if(szTitle=="dR1")	{SetGridFocusCell(ROW_CHORDLO+1, 9);	return;	}
	if(szTitle=="dR2")	{SetGridFocusCell(ROW_CHORDLO+1, 10);return;}	if(szTitle=="dR3")	{SetGridFocusCell(ROW_CHORDLO+1, 11);	return;	}
	if(szTitle=="dR4")	{SetGridFocusCell(ROW_CHORDLO+1, 12);return;}	if(szTitle=="dRG")	{SetGridFocusCell(ROW_CHORDLO+1, 13);	return;	}
	if(szTitle=="gW")	{SetGridFocusCell(ROW_CHORDLO+1, 14);return;}
	if(szTitle=="C1")	{SetGridFocusCell(ROW_CENT+1, 1);	return;}	if(szTitle=="C2")	{SetGridFocusCell(ROW_CENT+1, 2);	return;	}
	if(szTitle=="C3")	{SetGridFocusCell(ROW_CENT+1, 3);	return;}	if(szTitle=="C4")	{SetGridFocusCell(ROW_CENT+1, 4);	return;	}

	if(szTitle=="c1LG")	{SetGridFocusCell(ROW_DIAGONAL+1, 1);	return;	}	if(szTitle=="c1RG")	{SetGridFocusCell(ROW_DIAGONAL+1, 2);	return;	}
	if(szTitle=="c2LG")	{SetGridFocusCell(ROW_DIAGONAL+1, 3);	return;	}	if(szTitle=="c2RG")	{SetGridFocusCell(ROW_DIAGONAL+1, 4);	return;	}

	if(szTitle=="A1")	{SetGridFocusCell(ROW_ANCHOR+1, 1);	return;	}	if(szTitle=="A2")	{SetGridFocusCell(ROW_ANCHOR+1, 2);	return;	}
	if(szTitle=="A3")	{SetGridFocusCell(ROW_ANCHOR+1, 3);	return;	}	if(szTitle=="A4")	{SetGridFocusCell(ROW_ANCHOR+1, 4);	return;	}
	if(szTitle=="A5")	{SetGridFocusCell(ROW_ANCHOR+1, 5);	return;	}	if(szTitle=="A6")	{SetGridFocusCell(ROW_ANCHOR+1, 6);	return;	}
	CGeneralBaseDlg::SetFocusItem(szTitle);
}

void CDetVBracingTypeDlg::SetGridFocusCell(long nRow, long nCol)
{
	m_Grid.ResetSelection();
	m_Grid.SetFocusCell(nRow,nCol);
	m_Grid.SendMessage(WM_KEYDOWN,VK_F2);
}

void CDetVBracingTypeDlg::OnButtonAutoSpliceCalc() 
{
	SetDataSpliceCalc(m_nG, m_nPos);
	SetDataInit();

	DrawInputDomyunView(FALSE);
}

// 이음판의 제원을 결정함
void CDetVBracingTypeDlg::SetDataSpliceCalc(long nG, long nPos)
{
	CGlobarOption		*pOpt	= m_pStd->m_pDataManage->GetGlobalOption();
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateGirderApp		*pGirN	= pGir->m_pRight;
	CPlateBasicIndex	*pBx	= pGir->GetBxVBracingOnly(nPos);
	if(!pBx)		return;

	CPlateBasicIndex *pBxR     = pBx->GetBxMatchByCrossBeam(pGirN);
	CSection         *pSec     = pBx->GetSection();
	CVBracing        *pBr		= pSec->GetVBracing();
	if(!pBxR)		return;

	double dTWeb       = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dTWebR      = pGirN->GetThickFactByStation(pBxR->GetStation(), G_W);
	long nSteelTypeU = pBr->GetSteelMaterialData(STEELMAT_UPPER)->m_nType;
	long nSteelTypeD = pBr->GetSteelMaterialData(STEELMAT_LOWER)->m_nType;

	double dHBeamU =  pSec->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
	double dHBeamD =  pSec->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	double dHBeamC =  pSec->GetBeamBaseHeight(CSection::SEC_LEFT, TRUE, TRUE);
	double dTBeamU =  pSec->GetBeamJewon(CSection::SEC_DOWN, CSection::T1);
	double dTBeamD  = pSec->GetBeamJewon(CSection::SEC_DOWN, CSection::T1);
	double dCenBeamU = pSec->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
	double dCenBeamD =  pSec->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);
	double dCenBeamC =  pSec->GetBeamJewon(CSection::SEC_LEFT, CSection::CENTERDX);


	// 용접길이는 상수로 설정하고 이 값을 기준으로 이음판의 크기를 설정함
	
	//수직보강재 기준으로 거더 상판의 돌출폭 만큼 ULG, URG에서 뺌(Issue:6678)
	double dWidVStiffL = pSec->m_pVStiff->m_dWidth[1];
	double dWidVStiffR = pSec->m_pVStiff->m_dWidth[1];
	if(pBx->IsJijum() && pBx->GetJijumStiff())
	{
		dWidVStiffL = pBx->GetJijumStiff()->m_dWidth[1];
	}
	if(pBxR->IsJijum() && pBxR->GetJijumStiff())
	{
		dWidVStiffR = pBxR->GetJijumStiff()->m_dWidth[0];
	}
	
	double dWidthUpL  = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dThickWebL = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWidthUpR  = pGirN->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dThickWebR = pGirN->GetThickFactByStation(pBx->GetStation(), 2);
	
	double dSubL = (dWidthUpL / 2 - dThickWebL / 2) - dWidVStiffL;
	double dSubR = (dWidthUpR / 2 - dThickWebR / 2) - dWidVStiffR;

	
	if(!pOpt->GetInstallBeamAtCenter())
	{
		dCenBeamU = dHBeamU/2;
		dCenBeamD = dHBeamD/2;
		dCenBeamC = dHBeamC/2;
	}

	double dWVStiff = pSec->GetVStiff()->m_dWidth[1];
	double dWVStiffR= pBxR->GetSection()->GetVStiff()->m_dWidth[0];
	double dMinWeldL           = pBr->m_dC1RG < 0 ? 150 : pBr->m_dC1RG;		// 이음판과 브레이싱의 용접길이
	double dVBrBetweenMin      = 25;				// 브레이싱간 최소연단거리
	double dGapGussetAndBeamUp = 0;					// 거세트단부 최소이격거리 20(L형강), 50(CT형강)
	double dGapGussetAndBeamDn = 0;
	if(nSteelTypeU==CSteelMaterialData::LSTEEL || nSteelTypeU==CSteelMaterialData::LVALSTEEL)  dGapGussetAndBeamUp = 20;
	if(nSteelTypeD==CSteelMaterialData::LSTEEL || nSteelTypeD==CSteelMaterialData::LVALSTEEL)  dGapGussetAndBeamDn = 20;
	if(nSteelTypeU==CSteelMaterialData::CTSTEEL) dGapGussetAndBeamUp = 20+dTBeamU;
	if(nSteelTypeD==CSteelMaterialData::CTSTEEL) dGapGussetAndBeamDn = 20+dTBeamD;
	
	double dGapVStiffUpBeam = 20;	// 거더수직보강재와 상현재의 최소이격거리
	double dGapVStiffDnBeam = 15;	// 거더수직보강재와 하현재의 최소이격거리

	CDPoint xyMatch, xyMatch1, xyMatch2, xyMatch3, xyMatch4, xyCheck;
	CDPoint vX(1,0), vY(0,1);

	//********************************************************************************
	//수직브레이싱 중심부 이음판 (상,하)*********************************************
	//********************************************************************************
	if(pBr->GetType()==TYPE_INVVBRACING)
	{
		pBr->m_dc1 = dHBeamU-dGapGussetAndBeamUp;
		// 이음판 좌측부분을 기준으로 정함.
		CDPoint xyCen   = (pBr->GetPointFrame(0)+pBr->GetPointFrame(2))/2;
		CDPoint vDir    = (pBr->GetPointFrame(1)-xyCen).Unit();
		CDPoint vDirUp  = pBr->GetVectorFrame(0);
		// 좌측사재 시작부의 좌측모서리
		GetXyMatchLineAndLine(xyCen+vDir.RotateInv90()*dCenBeamC, vDir, xyCen+vDirUp.RotateInv90()*(dHBeamC-dCenBeamC+dVBrBetweenMin), vDirUp, xyMatch);
		// 이음판의 chamber부의 좌측
		xyMatch1 = xyMatch+vDir*dMinWeldL+vDir.RotateInv90()*pBr->m_dgW;
		// 이음판의 chamber부의 우측
		xyMatch2 = xyMatch1+vDir.Rotate90()*(dHBeamC+pBr->m_dgW*2);
		//프레임 중심 수직축과 이음판의 하단선이 교차되는 지점  
		GetXyMatchLineAndLine(xyCen, vY, xyMatch2, vDirUp, xyMatch3);

		pBr->m_dc3 = fabs(xyCen.x-xyMatch1.x);
		pBr->m_dc4 = pBr->m_dc3;
		pBr->m_dc2 = fabs((xyCen-vY*(dHBeamC-dCenBeamC)*vDirUp.x).y-xyMatch3.y);
		// 프레임 중심에서 수직축에서 좌측으로 브레이싱간 최소연단거리/2만큼 이격된 위치
		xyCheck = xyMatch1 + vDir.Rotate90()*(pBr->m_dgW+dHBeamC) - vDir*dMinWeldL;
		// 좌측사재 시작부의 우측 모서리가 중심축에서 최소연단거리/2이상 우측으로 떨어지지 않은 경우 
		if((xyMatch3.x - xyCheck.x) < dVBrBetweenMin/2)
		{
			// 프레임 중심 수직축에서 좌측으로 최소연단거리/2 만큼 이격된 위치와 사재의 우측라인이 교차하는 지점
			GetXyMatchLineAndLine(xyCen-vX*dVBrBetweenMin/2, vY, xyCen+vDir.Rotate90()*dCenBeamC, vDir, xyMatch4);
			// 새로 결정된 이음판 Chamber부의 우측
			CDPoint xyMatch5 = xyMatch4 + vDir*dMinWeldL + vDir.RotateInv90()*(dHBeamC+pBr->m_dgW);
			pBr->m_dc3 = fabs(xyCen.x-xyMatch5.x);
			pBr->m_dc4 = pBr->m_dc3;
			// 새로 결정된 이음판 Chamber부의 좌측
			CDPoint xyMatch6 = xyMatch4 + vDir*dMinWeldL + vDir.Rotate90()*(pBr->m_dgW);

			pBr->m_dc2 = fabs(xyCen.y-xyMatch6.y-(dHBeamC-dCenBeamC));
		}		
	}
	else 
	{
		pBr->m_dc2 = dHBeamD-dGapGussetAndBeamDn;
		// 이음판 좌측부분을 기준으로 정함.
		CDPoint xyCen   = (pBr->GetPointFrame(1)+pBr->GetPointFrame(3))/2;
		CDPoint vDir    = (pBr->GetPointFrame(0)-xyCen).Unit();
		CDPoint vDirDn  = pBr->GetVectorFrame(1);
		GetXyMatchLineAndLine(xyCen+vDir.Rotate90()*(dHBeamC-dCenBeamC), vDir, xyCen+vDirDn.Rotate90()*(dHBeamC-dCenBeamC+dVBrBetweenMin), vDirDn, xyMatch);
		xyMatch1 = xyMatch+vDir*dMinWeldL+vDir.Rotate90()*pBr->m_dgW;
		xyMatch2 = xyMatch1+vDir.RotateInv90()*(dHBeamC+pBr->m_dgW*2);
		GetXyMatchLineAndLine(xyCen, vY, xyMatch2, vDirDn, xyMatch3);
		pBr->m_dc3 = fabs(xyCen.x-xyMatch1.x);
		pBr->m_dc4 = pBr->m_dc3;
		pBr->m_dc1 = fabs((xyCen+vY*(dHBeamC-dCenBeamC)*vDirDn.x).y-xyMatch3.y);

		xyCheck = xyMatch1 + vDir.RotateInv90()*(pBr->m_dgW+dCenBeamC) - vDir*dMinWeldL;
		if((xyMatch3.x - xyCheck.x) < dVBrBetweenMin/2)
		{
			GetXyMatchLineAndLine(xyCen-vX*dVBrBetweenMin/2, vY, xyCen+vDir.RotateInv90()*dCenBeamC, vDir, xyMatch4);
			CDPoint xyMatch5 = xyMatch4 + vDir*dMinWeldL + vDir.Rotate90()*(dHBeamC+pBr->m_dgW);
			pBr->m_dc3 = fabs(xyCen.x-xyMatch5.x);
			pBr->m_dc4 = pBr->m_dc3;
			CDPoint xyMatch6 = xyMatch4 + vDir*dMinWeldL + vDir.RotateInv90()*(pBr->m_dgW);

			pBr->m_dc1 = fabs(xyMatch6.y-xyCen.y-(dHBeamC-dCenBeamC));
		}
	}	

	//********************************************************************************
	//수직브레이싱 좌우측 이음판 (상,하)*********************************************
	//********************************************************************************
	dMinWeldL = pBr->m_dC1LG < 0 ? 150 : pBr->m_dC1LG ;			// 이음판과 브레이싱의 용접길이
	if(pBr->GetType()==TYPE_VBRACING)	// V형
	{
		// 좌측 이음판
		pBr->m_duL1 = dHBeamU-dGapGussetAndBeamUp; 
		pBr->m_duL2 = pBr->m_duL1 * 2.5;
		pBr->m_duL3 = 80;
		// pBr->m_duL4 를 구하기 위해 최소연단거리를 두번체크해야함
		CDPoint xyCen   = pBr->GetPointFrame(CVBracing::LT);
		CDPoint vDir    = pBr->GetVectorFrame(CVBracing::LEFT);
		CDPoint vDirUp  = pBr->GetVectorFrame(CVBracing::UPPER);
		GetXyMatchLineAndLine(xyCen+vDir.RotateInv90()*(dHBeamC-dCenBeamC), vDir, xyCen+vX*(dWVStiff-(pBr->m_dS-dTWeb/2)+dVBrBetweenMin), vY, xyMatch);
		xyMatch1 = xyMatch+vDir*dMinWeldL+vDir.RotateInv90()*pBr->m_dgW;
		xyMatch2 = xyMatch1+vDir.Rotate90()*(dHBeamC+pBr->m_dgW*2);
		CDPoint xySpBase= pBr->GetCenterSplice(CVBracing::LT);
		pBr->m_duL4 = fabs(xyMatch2.x-xySpBase.x);
		pBr->m_duL2 = fabs(xyMatch1.y-xySpBase.y);
		
		GetXyMatchLineAndLine(xyCen+vDir.Rotate90()*dCenBeamC, vDir, xyCen+vDirUp.RotateInv90()*(dHBeamC-dCenBeamC+dVBrBetweenMin), vDirUp, xyMatch3);
		GetXyMatchLineAndLine(xyMatch3, vY, xySpBase, vDirUp, xyCheck);
		// 사재와 상현재간의 최소연단거리 검토
		if((xyCheck.y-xyMatch3.y) < dVBrBetweenMin)
		{
			xyMatch4 = xyMatch3 + vDir*dMinWeldL + vDir.Rotate90()*pBr->m_dgW;
			pBr->m_duL4 = fabs(xyMatch4.x-xySpBase.x);
		}		
		pBr->m_dULG = pBr->m_duL4-dGapVStiffUpBeam;
		pBr->m_ddL1 = (dHBeamD-dGapGussetAndBeamDn) * 1.5;
		pBr->m_ddL2 = dHBeamD-dGapGussetAndBeamDn;	
		pBr->m_ddL3 = 80;
		pBr->m_ddL4 = pBr->m_dDLG + 20 ;//+ dSubL;
		
		// 우측 이음판
		pBr->m_duR1 = dHBeamU-dGapGussetAndBeamUp; 	
		pBr->m_duR2 = pBr->m_duR1 * 2.5;
		pBr->m_duR3 = 80;
		// pBr->m_duL4 를 구하기 위해 최소연단거리를 두번체크해야함
		xyCen   = pBr->GetPointFrame(CVBracing::RT);
		vDir    = pBr->GetVectorFrame(CVBracing::RIGHT);
		vDirUp  = pBr->GetVectorFrame(CVBracing::UPPER);
		GetXyMatchLineAndLine(xyCen+vDir.Rotate90()*(dHBeamC-dCenBeamC), vDir, xyCen-vX*(dWVStiffR-(pBr->m_dS-dTWebR/2)+dVBrBetweenMin), vY, xyMatch);
		xyMatch1 = xyMatch+vDir*dMinWeldL+vDir.Rotate90()*pBr->m_dgW;
		xyMatch2 = xyMatch1+vDir.RotateInv90()*(dHBeamC+pBr->m_dgW*2);
		xySpBase = pBr->GetCenterSplice(CVBracing::RT);
		pBr->m_duR4 = fabs(xyMatch2.x-xySpBase.x);
		pBr->m_duR2 = fabs(xyMatch1.y-xySpBase.y);

		GetXyMatchLineAndLine(xyCen+vDir.RotateInv90()*dCenBeamC, vDir, xyCen+vDirUp.RotateInv90()*(dHBeamC-dCenBeamC+dVBrBetweenMin), vDirUp, xyMatch3);
		GetXyMatchLineAndLine(xyMatch3, vY, xySpBase, vDirUp, xyCheck);
		// 사재와 하현재간의 최소연단거리 검토
		if((xyCheck.y-xyMatch3.y) < dVBrBetweenMin)
		{
			xyMatch4 = xyMatch3 + vDir*dMinWeldL + vDir.RotateInv90()*pBr->m_dgW;
			pBr->m_duR4 = fabs(xyMatch4.x-xySpBase.x);
		}
		pBr->m_dURG = pBr->m_duR4-dGapVStiffUpBeam;
		pBr->m_ddR1 = (dHBeamD-dGapGussetAndBeamDn) * 1.5;
		pBr->m_ddR2 = dHBeamD-dGapGussetAndBeamDn;		
		pBr->m_ddR3 = 80;
		pBr->m_ddR4 = pBr->m_dDRG + 20 ;//+ dSubR;
	}
	else				// 역V형
	{
		// 좌측 이음판
		pBr->m_duL1 = dHBeamU-dGapGussetAndBeamUp; 
		pBr->m_duL2 = pBr->m_duL1 * 1.5;
		pBr->m_duL3 = 80;
		pBr->m_duL4 = pBr->m_dULG+20 + dSubL;

		pBr->m_ddL1 = (dHBeamD-dGapGussetAndBeamDn) * 2.5;
		pBr->m_ddL2 = dHBeamD-dGapGussetAndBeamDn;	
		pBr->m_ddL3 = 80;
		// pBr->m_ddL4를 구하기 위해 최소연단거리를 두번체크해야함
		CDPoint xyCen   = pBr->GetPointFrame(CVBracing::LD);
		CDPoint vDir    = pBr->GetVectorFrame(CVBracing::LEFT);
		CDPoint vDirLo  = pBr->GetVectorFrame(CVBracing::LOWER);
		CDPoint xyMatchTemp;
		GetXyMatchLineAndLine(xyCen+vDir.Rotate90()*dCenBeamC, vDir, xyCen+vX*(dWVStiff-(pBr->m_dS-dTWeb/2)+dVBrBetweenMin), vY, xyMatch);
		GetXyMatchLineAndLine(xyCen+vDir.RotateInv90()*(dHBeamC-dCenBeamC), vDir, xyCen+vX*(dWVStiff-(pBr->m_dS-dTWeb/2)+dVBrBetweenMin), vY, xyMatchTemp);
		xyMatch1 = xyMatch+vDir*dMinWeldL+vDir.Rotate90()*pBr->m_dgW;
		xyMatch2 = xyMatch1+vDir.RotateInv90()*(dHBeamC+pBr->m_dgW*2);
		CDPoint xySpBase= pBr->GetCenterSplice(CVBracing::LD);
		pBr->m_ddL4 = fabs(xyMatch2.x-xySpBase.x);
		pBr->m_ddL1 = fabs(xyMatch1.y-xySpBase.y);
		
		GetXyMatchLineAndLine(xyCen+vDir.RotateInv90()*(dHBeamC-dCenBeamC), vDir, xyCen+vDirLo.Rotate90()*((dHBeamC-dCenBeamC)+dVBrBetweenMin), vDirLo, xyMatch3);
		GetXyMatchLineAndLine(xyMatch3, vY, xySpBase, vDirLo, xyCheck);

		if((xyMatchTemp.y-xyCheck.y) < dVBrBetweenMin)	// 사재끝단과 하현재의 끝단 최소이격거리 만족.
		{
			xyMatch4 = xyMatch3 + vDir*dMinWeldL + vDir.RotateInv90()*pBr->m_dgW;
			pBr->m_ddL4 = fabs(xyMatch4.x-xySpBase.x);

			CDPoint xyMatch5;
			GetXyMatchLineAndLine(xyMatch4, vDir.RotateInv90(), xyMatch, vDir, xyMatch5);
			xyMatch5 += vDir.Rotate90()*pBr->m_dgW;
			pBr->m_ddL1 = fabs(xyMatch5.y-xySpBase.y);
		}			

		pBr->m_dDLG = pBr->m_ddL4-dGapVStiffDnBeam;
		// 우측 이음판
		pBr->m_duR1 = dHBeamU-dGapGussetAndBeamUp; 	
		pBr->m_duR2 = pBr->m_duR1 * 1.5;
		pBr->m_duR3 = 80;
		pBr->m_duR4 = pBr->m_dURG+20 + dSubR;
		
		pBr->m_ddR1 = (dHBeamD-dGapGussetAndBeamDn) * 2.5;
		pBr->m_ddR2 = dHBeamD-dGapGussetAndBeamDn;		
		pBr->m_ddR3 = 80;
		// pBr->m_ddR4를 구하기 위해 최소연단거리를 두번체크해야함
		xyCen   = pBr->GetPointFrame(CVBracing::RD);
		vDir    = pBr->GetVectorFrame(CVBracing::RIGHT);
		vDirLo  = pBr->GetVectorFrame(CVBracing::LOWER);
		GetXyMatchLineAndLine(xyCen+vDir.RotateInv90()*dCenBeamC, vDir, xyCen-vX*(dWVStiffR-(pBr->m_dS-dTWebR/2)+dVBrBetweenMin), vY, xyMatch);
		GetXyMatchLineAndLine(xyCen+vDir.Rotate90()*(dHBeamC-dCenBeamC), vDir, xyCen+vX*(dWVStiff-(pBr->m_dS-dTWeb/2)+dVBrBetweenMin), vY, xyMatchTemp);
		xyMatch1 = xyMatch+vDir*dMinWeldL+vDir.RotateInv90()*pBr->m_dgW;
		xyMatch2 = xyMatch1+vDir.Rotate90()*(dHBeamC+pBr->m_dgW*2);
		xySpBase = pBr->GetCenterSplice(CVBracing::RD);
		pBr->m_ddR4 = fabs(xyMatch2.x-xySpBase.x);
		pBr->m_ddR1 = fabs(xyMatch1.y-xySpBase.y);
		
		GetXyMatchLineAndLine(xyCen+vDir.Rotate90()*(dHBeamC-dCenBeamC), vDir, xyCen+vDirLo.Rotate90()*((dHBeamC-dCenBeamC)+dVBrBetweenMin), vDirLo, xyMatch3);
		GetXyMatchLineAndLine(xyMatch3, vY, xySpBase, vDirLo, xyCheck);

		if((xyMatchTemp.y-xyCheck.y) < dVBrBetweenMin)
		{
			xyMatch4 = xyMatch3 + vDir*dMinWeldL + vDir.Rotate90()*pBr->m_dgW;
			pBr->m_ddR4 = fabs(xyMatch4.x-xySpBase.x);
	
			CDPoint xyMatch5;
			GetXyMatchLineAndLine(xyMatch4, vDir.RotateInv90(), xyMatch, vDir, xyMatch5);
			xyMatch5 += vDir.RotateInv90()*pBr->m_dgW;
			pBr->m_ddR1 = fabs(xyMatch5.y-xySpBase.y);
		}			
		pBr->m_dDRG = pBr->m_ddR4-dGapVStiffDnBeam;
	}

	pBr->m_bBoltInterver = FALSE;
	pBr->m_dBoltInterver = 80.0;

	//상현재가 ㄷ형강일 경우 기본값을 누르면 아래 루틴이 적용됨(Issue:6674)
	CSteelMaterialData	*pMatDataUp	= pBr->GetSteelMaterialData(STEELMAT_UPPER);
	BOOL				bCSTEEL		= FALSE;
	
	if(pMatDataUp->m_nType == CSteelMaterialData::CSTEEL)
		bCSTEEL = TRUE;
	
	if(bCSTEEL)
	{
		CSteelMaterialData *pMatData = pBr->GetSteelMaterialData(STEELMAT_UPPER);
		double dHeight  = pMatData->m_dH;
		
		pBr->m_duL1 = dHeight - 20;
		pBr->m_duR1 = dHeight - 20;
		
		pBr->m_duL2 = 120;
		pBr->m_duR2 = 120;
	}

	//*******************************************************************************
	//수직브레이싱 이음판 볼트제원***************************************************
	//*******************************************************************************
	if(pBr->GetType()==TYPE_VBRACING)
	{
		pBr->m_uB  =3;	
		pBr->m_uC  =80;	
		pBr->m_dB  =2;	
		pBr->m_dC  =80;	
	}
	else
	{
		double du1 = min(pBr->m_duL1, pBr->m_duR1);
		double du2 = min(pBr->m_duL2, pBr->m_duR2);

		pBr->m_uC = 80;	
		pBr->m_uB = (long)((du1 + du2) / (pBr->m_uC) - 1);
		pBr->m_dB = 3;	
		pBr->m_dC = 80;	
	}

	//*******************************************************************************
	//수직브레이싱 이음판 볼트제원***************************************************
	//*******************************************************************************

	CPlatePieceObject pObj1(pGir), pObj2(pGir), pObj3(pGir), pObj4(pGir), pObj5(pGir);
	pBr->CreatePieceObj(&pObj1, FALSE, FALSE, 0);
	pBr->CreatePieceObj(&pObj2, FALSE, FALSE, 1);
	pBr->CreatePieceObj(&pObj3, FALSE, FALSE, 2);
	pBr->CreatePieceObj(&pObj4, FALSE, FALSE, 3);
	pBr->CreatePieceObj(&pObj5, FALSE, FALSE, 4);
}

void CDetVBracingTypeDlg::OnCheckCutLevel() 
{
	UpdateData();
	CVBracing *pBr = m_pVBracingCurrent;
	pBr->m_bCutLevel = m_bCutLevel;

	DrawInputDomyunView(FALSE);
}

BOOL CDetVBracingTypeDlg::IsValid()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	for (long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while (pBx)
		{
			CSection* pSec = pBx->GetSection();
			if(pSec->GetenDetType()==SECDET_VBRACING)
				return TRUE;
			pBx = Finder.GetBxNext();
		}
	}

	return FALSE;
}

void CDetVBracingTypeDlg::SetVBracingCurrent()
{
	if(!m_ctlPosCombo.GetSafeHwnd()) return;

	if(m_ctlPosCombo.GetCount() <= 0) return;

	int nCurSel = m_ctlPosCombo.GetCurSel();

	CString rString;
	m_ctlPosCombo.GetLBText(nCurSel, rString);

	//"G%d-G%d-C%d"
	int		nF	 = 0;
	int		nF2	 = 0;
	int		nG	 = 0;
	int		nCr	 = 0;
	CString sGir = _T("");
	CString sCr  = _T("");
	
	if(rString.GetLength() > 0 )
	{
		nF = rString.Find("-");

		sGir= rString.Left(nF);//G%d
		sGir = sGir.Mid(1);//%d

		nF2 = rString.Find("-", nF+1);
		sCr = rString.Mid(nF2+2);

		nG	 = atol(sGir) - 1;
		nCr = atol(sCr);// - 1;
	}

	CPlateBridgeApp* pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	int nVBCount = 0;
	int nCount = 1;
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec)
		{	
			if(pSec->GetenDetType()==SECDET_VBRACING)
			{
				if(nCr == nCount)
				{
					m_pVBracingCurrent = pSec->GetVBracing();
					m_nG = nG;
					m_nPos = nVBCount;
					m_nCurPos = nCount;
					break;
				}
				nVBCount++;
			}
		}
		pBx = Finder.GetBxNext();
		nCount++;
	}
}

long CDetVBracingTypeDlg::GetRowSize()
{
	long	nRow = 10;
	if(m_pVBracingCurrent->m_nAnchorSu>0)	nRow+= 2;

	return nRow;
}

long CDetVBracingTypeDlg::GetColSize()
{
	long	nCol = 15;
	return	nCol;
}

void CDetVBracingTypeDlg::SetDataInitCommon()
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();		
	CVBracing			*pBr		= m_pVBracingCurrent;
	CPlateGirderApp		*pGir		= pDB->GetGirder(m_nG);
	CPlateBasicIndex	*pBx		= pBr->GetBx();
	CHBracingGusset		*pBrRu		= pBx->GetHBracingGusset();
	CPlateBasicIndex	*pBxR		= pGir->GetBxMatchByCrossBeam(pBx,pGir->m_pRight);
	if(pBxR==NULL) return;

	CHBracingGusset *pBrRuR = pBxR->GetHBracingGusset();
	CSteelBolt *pBolt = m_pStd->GetDataManage()->GetSteelBolt();
	
	m_Grid.SetMergeRow(ROW_COMMON, ROW_COMMON+1, 0);
	long nCol = 0;

	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"공통",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"LH1",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"LH2",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"RH1",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"RH2",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"vWG",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"vW1G",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"vTG",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"vT",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"vHL",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"vHR",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"S",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"T",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"gW",	(UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_COMMON, nCol++,	"sL",	(UINT)DT_CENTER);

	for(int n = 0; n < GetColSize(); n++)
		m_Grid.SetCellFixType(ROW_COMMON, n, GVN_ONECELL_COLFIX);

	nCol		= 1;
	long nRow	= ROW_COMMON+1;	
	BOOL bBoltLengthAuto = m_pStd->GetDataManage()->GetGlobalOption()->GetBoltLengthAuto();
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dLH1);
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dLH2);
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dRH1);
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dRH2);
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_vW_G);
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_vW1_G);
	 if(pBx->IsJijum())
	 {
		 m_Grid.SetItemBkColour(nRow, nCol-1, RGBREADONLY);
		 m_Grid.SetItemState(nRow, nCol-1, GVIS_READONLY);
	 }
	 else
	 {
		 m_Grid.SetItemBkColour(nRow, nCol-1, RGB(225, 250, 250));
		 m_Grid.SetItemState(nRow, nCol-1, GVIS_MODIFIED);
	 }
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_vT_G);
	 if(pBrRu)
	 {
		 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBrRu->m_dT);
		 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBrRu->m_ddH);
	 }
	 else	nCol+=2;
	 if(pBrRuR)
	 {
		 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBrRuR->m_ddH);
	 }
	 else	nCol+=1;
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dS);
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dT);
	 m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dgW);
	if(bBoltLengthAuto)
	{
		double sL = pBolt->GetBoltLength(pBx, VB_BOLT, pBr->m_ZzsBoltSize, pBr->m_dT + pBr->m_vT_G);
		m_Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
		m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
		m_Grid.SetTextMatrix(nRow, nCol++,"%.0f",sL);
	}
	else
	{
		m_Grid.SetItemBkColour(nRow, nCol, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, nCol, GVIS_MODIFIED);
		m_Grid.SetTextMatrix(nRow, nCol++,"%.0f",pBr->m_dSL);
	}
}

void CDetVBracingTypeDlg::SetDataInitChordUp()
{
	CVBracing			*pBr		= m_pVBracingCurrent;

	m_Grid.SetMergeRow(ROW_CHORDUP, ROW_CHORDUP+1, 0);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 0, "상현재좌우측", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 1, "uL1", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 2, "uL2", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 3, "uL3", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 4, "uL4", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 5, "uLG", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 6, "uB", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 7, "uC", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 8, "uE", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 9, "uR1", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 10, "uR2",(UINT) DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 11, "uR3",(UINT) DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 12, "uR4", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDUP, 13, "uRG",(UINT) DT_CENTER);
	for(int n = 0; n < GetColSize(); n++)
		m_Grid.SetCellFixType(ROW_CHORDUP, n, GVN_ONECELL_COLFIX);

	long	nRow	= ROW_CHORDUP+1;
	long	nCol	= 1;
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duL1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duL2);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duL3);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duL4);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dULG);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", (double)pBr->m_uB);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", (double)pBr->m_uC);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duE);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duR1);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duR2);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duR3);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_duR4);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dURG);	SetGridCellType(nRow, nCol-1);
}

void CDetVBracingTypeDlg::SetDataInitChordLo()
{
	CVBracing			*pBr		= m_pVBracingCurrent;
	m_Grid.SetMergeRow(ROW_CHORDLO, ROW_CHORDLO+1, 0);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 0, "하현재좌우측", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 1, "dL1", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 2, "dL2", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 3, "dL3", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 4, "dL4", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 5, "dLG", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 6, "dB", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 7, "dC", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 8, "dE", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 9, "dR1", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 10, "dR2",(UINT) DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 11, "dR3",(UINT) DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 12, "dR4", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CHORDLO, 13, "dRG",(UINT) DT_CENTER);
	for(int n = 0; n < GetColSize(); n++)
		m_Grid.SetCellFixType(ROW_CHORDLO, n, GVN_ONECELL_COLFIX);

	long	nRow	= ROW_CHORDLO+1;
	long	nCol	= 1;
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddL1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddL2);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddL3);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddL4);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dDLG);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", (double)pBr->m_dB);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", (double)pBr->m_dC);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddE);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddR1);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddR2);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddR3);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_ddR4);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dDRG);	SetGridCellType(nRow, nCol-1);
}

void CDetVBracingTypeDlg::SetDataInitCent()
{
	CVBracing			*pBr		= m_pVBracingCurrent;
	m_Grid.SetMergeRow(ROW_CENT, ROW_CENT+1, 0);
	if(m_pVBracingCurrent->GetType() == TYPE_VBRACING)
		m_Grid.SetTextMatrix(ROW_CENT, 0, "하현재중앙", (UINT)DT_CENTER);
	else
		m_Grid.SetTextMatrix(ROW_CENT, 0, "상현재중앙", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CENT, 1, "C1", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CENT, 2, "C2", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CENT, 3, "C3", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_CENT, 4, "C4", (UINT)DT_CENTER);
	for(int n = 0; n < GetColSize(); n++)
		m_Grid.SetCellFixType(ROW_CENT, n, GVN_ONECELL_COLFIX);

	long	nRow	= ROW_CENT+1;
	long	nCol	= 1;
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dc1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dc2);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dc3);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dc4);	SetGridCellType(nRow, nCol-1);
}

void CDetVBracingTypeDlg::SetDataInitDiagonal()
{
	CVBracing			*pBr		= m_pVBracingCurrent;
	m_Grid.SetMergeRow(ROW_DIAGONAL, ROW_DIAGONAL+1, 0);
	m_Grid.SetTextMatrix(ROW_DIAGONAL, 0, "사재용접부", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_DIAGONAL, 1, "c1LG", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_DIAGONAL, 2, "c1RG", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_DIAGONAL, 3, "c2LG", (UINT)DT_CENTER);
	m_Grid.SetTextMatrix(ROW_DIAGONAL, 4, "c2RG", (UINT)DT_CENTER);
	for(int n = 0; n < GetColSize(); n++)
		m_Grid.SetCellFixType(ROW_DIAGONAL, n, GVN_ONECELL_COLFIX);
	long	nRow	= ROW_DIAGONAL+1;
	long	nCol	= 1;
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dC1LG);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dC1RG);	
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dC2LG);	SetGridCellType(nRow, nCol-1);
	m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_dC2RG);	SetGridCellType(nRow, nCol-1);
}

void CDetVBracingTypeDlg::SetDataInitAnchor()
{
	CVBracing	*pBr		= m_pVBracingCurrent;
	if(pBr->m_nAnchorSu<1)	return;

	CString	szText	= _T("");
	m_Grid.SetMergeRow(ROW_ANCHOR, ROW_ANCHOR+1, 0);
	m_Grid.SetTextMatrix(ROW_ANCHOR, 0, "슬래브앵커", (UINT)DT_CENTER);
	long n = 0;
	for(n=0; n<m_pVBracingCurrent->m_nAnchorSu; n++)
	{
		szText.Format("A%d", n+1);
		m_Grid.SetTextMatrix(ROW_ANCHOR, n+1, szText, (UINT)DT_CENTER);
	}
	for(n=0; n<GetColSize(); n++)
		m_Grid.SetCellFixType(ROW_ANCHOR, n, GVN_ONECELL_COLFIX);
	long	nRow	= ROW_ANCHOR+1;
	long	nCol	= 1;
	for(n=0; n< pBr->m_nAnchorSu;n++)
	{
		m_Grid.SetTextMatrix(nRow, nCol++, "%.0f", pBr->m_AnchorDis[n]);
	}
}

void CDetVBracingTypeDlg::SetDataSaveCommon()
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();		
	CPlateGirderApp		*pGir		= pDB->GetGirder(m_nG);
	CPlateBasicIndex	*pBx		= pGir->GetBxVBracingOnly(m_nPos);
	CSection			*pSec		= pBx->GetSection();
	CVBracing			*pBr		= pSec->GetVBracing();
	CHBracingGusset		*pBrRu		= pBx->GetHBracingGusset();
	CPlateBasicIndex	*pBxR		= pGir->GetBxMatchByCrossBeam(pBx,pGir->m_pRight);
	if(pBxR==NULL) return;

	CHBracingGusset		*pBrRuR		= pBxR->GetHBracingGusset();

	long	nRow	= ROW_COMMON+1;
	long	nCol	= 1;
	pBr->m_dLH1		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_dLH2		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_dRH1		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_dRH2		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_vW_G		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_vW1_G	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_vT_G		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	
	if(pBrRu)
	{
		pBrRu->m_dT		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		if(pBrRuR)	pBrRuR->m_dT = pBrRu->m_dT;
		pBrRu->m_ddH	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	}
	else	nCol+=2;
	if(pBrRuR)
	{
		pBrRuR->m_ddH	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	}
	else	nCol+=2;
	pBr->m_dS	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_dT	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_dgW	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBr->m_dSL	= m_Grid.GetTextMatrixDouble(nRow, nCol++);

	pBr->m_pVStiff->m_dH[LEFT][UP] = 0;
	pBr->m_pVStiff->m_dH[LEFT][DOWN] = 0;
	pBr->m_pVStiff->m_dH[RIGHT][UP] = 0;	
	pBr->m_pVStiff->m_dH[RIGHT][DOWN] = 0;

	SetDataSpliceCalc(m_nG, m_nPos);
	SetDataVStiffJewon(pBx);
}

void CDetVBracingTypeDlg::SetDataSaveChordUp()
{
	CVBracing	*pBr	= m_pVBracingCurrent;

	long	nRow	= ROW_CHORDUP+1;
	BOOL	bSymmetry	= m_pVBracingCurrent->m_bSameLeftRight;
	pBr->m_duL1	= m_Grid.GetTextMatrixDouble(nRow, 1);
	pBr->m_duL2	= m_Grid.GetTextMatrixDouble(nRow, 2);
	pBr->m_duL3	= m_Grid.GetTextMatrixDouble(nRow, 3);
	pBr->m_duL4	= m_Grid.GetTextMatrixDouble(nRow, 4);
	pBr->m_dULG	= m_Grid.GetTextMatrixDouble(nRow, 5);
	pBr->m_uB	= m_Grid.GetTextMatrixLong(nRow, 6);
	pBr->m_uC	= m_Grid.GetTextMatrixLong(nRow, 7);
	pBr->m_duE	= m_Grid.GetTextMatrixDouble(nRow, 8);
	pBr->m_duR1	= bSymmetry ? pBr->m_duL1 : m_Grid.GetTextMatrixDouble(nRow, 9);
	pBr->m_duR2	= bSymmetry ? pBr->m_duL2 : m_Grid.GetTextMatrixDouble(nRow, 10);
	pBr->m_duR3	= bSymmetry ? pBr->m_duL3 : m_Grid.GetTextMatrixDouble(nRow, 11);
	pBr->m_duR4	= bSymmetry ? pBr->m_duL4 : m_Grid.GetTextMatrixDouble(nRow, 12);
	pBr->m_dURG	= bSymmetry ? pBr->m_dULG : m_Grid.GetTextMatrixDouble(nRow, 13);
}

void CDetVBracingTypeDlg::SetDataSaveChordLo()
{
	CVBracing	*pBr	= m_pVBracingCurrent;

	long	nRow	= ROW_CHORDLO+1;
	BOOL	bSymmetry	= m_pVBracingCurrent->m_bSameLeftRight;
	pBr->m_ddL1	= m_Grid.GetTextMatrixDouble(nRow, 1);
	pBr->m_ddL2	= m_Grid.GetTextMatrixDouble(nRow, 2);
	pBr->m_ddL3	= m_Grid.GetTextMatrixDouble(nRow, 3);
	pBr->m_ddL4	= m_Grid.GetTextMatrixDouble(nRow, 4);
	pBr->m_dDLG	= m_Grid.GetTextMatrixDouble(nRow, 5);
	pBr->m_dB	= (long)m_Grid.GetTextMatrixDouble(nRow, 6);
	pBr->m_dC	= (long)m_Grid.GetTextMatrixDouble(nRow, 7);
	pBr->m_ddE	= m_Grid.GetTextMatrixDouble(nRow, 8);
	pBr->m_ddR1	= bSymmetry ? pBr->m_ddL1 : m_Grid.GetTextMatrixDouble(nRow, 9);
	pBr->m_ddR2	= bSymmetry ? pBr->m_ddL2 : m_Grid.GetTextMatrixDouble(nRow, 10);
	pBr->m_ddR3	= bSymmetry ? pBr->m_ddL3 : m_Grid.GetTextMatrixDouble(nRow, 11);
	pBr->m_ddR4	= bSymmetry ? pBr->m_ddL4 : m_Grid.GetTextMatrixDouble(nRow, 12);
	pBr->m_dDRG	= bSymmetry ? pBr->m_dDLG : m_Grid.GetTextMatrixDouble(nRow, 13);
}

void CDetVBracingTypeDlg::SetDataSaveCent()
{
	CVBracing	*pBr	= m_pVBracingCurrent;

	long	nRow	= ROW_CENT+1;
	BOOL	bSymmetry	= m_pVBracingCurrent->m_bSameLeftRight;
	pBr->m_dc1	= m_Grid.GetTextMatrixDouble(nRow, 1);
	pBr->m_dc2	= m_Grid.GetTextMatrixDouble(nRow, 2);
	pBr->m_dc3	= m_Grid.GetTextMatrixDouble(nRow, 3);
	pBr->m_dc4	= bSymmetry ? pBr->m_dc3 : m_Grid.GetTextMatrixDouble(nRow, 4);
}

void CDetVBracingTypeDlg::SetDataSaveDiagonal()
{
	CVBracing	*pBr	= m_pVBracingCurrent;

	long	nRow	= ROW_DIAGONAL+1;
	BOOL	bSymmetry	= m_pVBracingCurrent->m_bSameLeftRight;
	pBr->m_dC1LG	= m_Grid.GetTextMatrixDouble(nRow, 1);
	pBr->m_dC1RG	= m_Grid.GetTextMatrixDouble(nRow, 2);
	pBr->m_dC2LG	= bSymmetry ? pBr->m_dC1RG : m_Grid.GetTextMatrixDouble(nRow, 3);
	pBr->m_dC2RG	= bSymmetry ? pBr->m_dC1LG : m_Grid.GetTextMatrixDouble(nRow, 4);
}

void CDetVBracingTypeDlg::SetDataSaveAnchor()
{
	CVBracing	*pBr	= m_pVBracingCurrent;

	long	nRow	= ROW_ANCHOR+1;
	long	nCol	= 1;
	for(long n=0; n<pBr->m_nAnchorSu;n++)
	{
		pBr->m_AnchorDis[n]	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	}
}

void CDetVBracingTypeDlg::SetSteelTypeCombo(long nSteelType)
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= m_pVBracingCurrent->GetSteelMaterialData(nSteelType);
	long nComboID = 0;
	if(nSteelType==STEELMAT_UPPER)			nComboID	= IDC_COMBO_STEELTYPEUP;
	else if(nSteelType==STEELMAT_LOWER)		nComboID	= IDC_COMBO_STEELTYPELO;
	else if(nSteelType==STEELMAT_DIAGONAL)	nComboID	= IDC_COMBO_STEELTYPEDIAGONAL;
	CComboBox *pItem	= (CComboBox*)GetDlgItem(nComboID);

	pItem->ResetContent();
	// 등변ㄱ, 부등변ㄱ, ㄷ, CT 허용 / I, H 불허
	for(long nCount=0; nCount<4/*CSteelMaterial::BUJE_COUNT*/; nCount++)
		pItem->AddString(pMat->GetTypeName(nCount));
	pItem->SetCurSel(pMatData->m_nType);
}

void CDetVBracingTypeDlg::SetSteelMaterialCombo(long nSteelType)
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= m_pVBracingCurrent->GetSteelMaterialData(nSteelType);

	long nComboID = 0;
	if(nSteelType==STEELMAT_UPPER)			nComboID	= IDC_COMBO_STEELMATUP;
	else if(nSteelType==STEELMAT_LOWER)		nComboID	= IDC_COMBO_STEELMATLO;
	else if(nSteelType==STEELMAT_DIAGONAL)	nComboID	= IDC_COMBO_STEELMATDIAGONAL;
	CComboBox *pItem	= (CComboBox*)GetDlgItem(nComboID);

	pItem->ResetContent();
	for(long nCount=0; nCount<pMat->GetSizeJewon(pMatData->m_nType); nCount++)
		pItem->AddString(pMat->GetJewonDataString(pMatData->m_nType, nCount));
	pItem->SetCurSel(pMatData->m_nJewonIdx);
}

void CDetVBracingTypeDlg::SetDataSaveSteelMaterial()
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pCurMat	= NULL;
	CSteelMaterialData	*pMatData	= NULL;
	CComboBox			*pItemType	= NULL;
	CComboBox			*pItemJewon	= NULL;

	long	nSteelType	= 0;
	long	nJewonIdx	= 0;

	for(long n=0;n<3;n++)
	{

		if(n==0)
		{
			pItemType	= (CComboBox*)GetDlgItem(IDC_COMBO_STEELTYPEUP);
			pItemJewon	= (CComboBox*)GetDlgItem(IDC_COMBO_STEELMATUP);
			pCurMat		= m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_UPPER);
		}
		if(n==1)
		{
			pItemType	= (CComboBox*)GetDlgItem(IDC_COMBO_STEELTYPELO);
			pItemJewon	= (CComboBox*)GetDlgItem(IDC_COMBO_STEELMATLO);
			pCurMat		= m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_LOWER);
		}
		if(n==2)
		{
			pItemType	= (CComboBox*)GetDlgItem(IDC_COMBO_STEELTYPEDIAGONAL);
			pItemJewon	= (CComboBox*)GetDlgItem(IDC_COMBO_STEELMATDIAGONAL);
			pCurMat		= m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_DIAGONAL);
		}
		nSteelType						= pItemType->GetCurSel();
		nJewonIdx						= pItemJewon->GetCurSel();
		pMatData						= pMat->GetMatData(nSteelType, nJewonIdx);
		pCurMat->m_nType				= nSteelType;
		pCurMat->m_nJewonIdx			= nJewonIdx;
		pCurMat->m_dH					= pMatData->m_dH;				
		pCurMat->m_dB					= pMatData->m_dB;
		pCurMat->m_dT1					= pMatData->m_dT1;
		pCurMat->m_dT2					= pMatData->m_dT2;
		pCurMat->m_dR1					= pMatData->m_dR1;
		pCurMat->m_dSectionArea			= pMatData->m_dSectionArea;
		pCurMat->m_dUnitWeight			= pMatData->m_dUnitWeight;
		pCurMat->m_d2MomentOfInertia.x	= pMatData->m_d2MomentOfInertia.x;
		pCurMat->m_d2MomentOfInertia.y	= pMatData->m_d2MomentOfInertia.y;
		pCurMat->m_dRadiusOfGyration.x	= pMatData->m_dRadiusOfGyration.x;
		pCurMat->m_dRadiusOfGyration.y	= pMatData->m_dRadiusOfGyration.y;
		pCurMat->m_dModulusOfSection.x	= pMatData->m_dModulusOfSection.x;
		pCurMat->m_dModulusOfSection.y	= pMatData->m_dModulusOfSection.y;
	}
}

/////////////////////////////////////////////////////////////////
// 수직브레이싱에 설치되는 거더수직보강재의 제원을 결정함
// 수직보강재의 설치여부는 폭으로 확인함. m_dH[][Up] 의 값이 dHGir 면 없는것
/////////////////////////////////////////////////////////////////
void CDetVBracingTypeDlg::SetDataVStiffJewon(CPlateBasicIndex *pBx)
{
	if(!pBx) return;

	CPlateBridgeApp  *pDB = m_pStd->GetBridge();		
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBasicIndex *pBxMatch= pBx->GetBxMatchByCrossBeam(pGir->m_pRight);

	//double    dHGir   = pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	CSection *pSec    = pBx->GetSection();
	CVStiff  *pVStiffL= pSec->GetVStiff();
	CVStiff  *pVStiffR= pBxMatch->GetSection()->GetVStiff();
	CVBracing	*pVr = pSec->GetVBracing();
	if(!pVr) return;

	pVStiffL->m_pSection = pSec;
	pVStiffR->m_pSection = pBxMatch->GetSection();

	// 초기화
	pVStiffL->m_dWidth[1]		= 0;
	pVStiffL->m_dThick[1]		= 0;
	pVStiffL->m_dH[RIGHT][UP]	= 0;
	pVStiffL->m_dH[RIGHT][DOWN] = 0;

	pVStiffR->m_dWidth[0]		= 0;
	pVStiffR->m_dThick[0]		= 0;
	pVStiffR->m_dH[LEFT][UP]	= 0;
	pVStiffR->m_dH[LEFT][DOWN]	= 0;

	if(pBx->IsJijum() && pBx->GetJijumStiff())
	{
		CVStiff *pVStiffJ = pBx->GetJijumStiff();
		pVStiffL->m_dWidth[1] = pVStiffJ->m_dWidth[1];
		pVStiffL->m_dThick[1] = pVStiffJ->m_dThick[1];

		if(pBx->GetBxMatchByCrossBeam(pBx->GetGirder()->m_pRight))
		{
			pVStiffJ = pBx->GetBxMatchByCrossBeam(pBx->GetGirder()->m_pRight)->GetJijumStiff();
			pVStiffR->m_dWidth[0] = pVStiffJ->m_dWidth[0];
			pVStiffR->m_dThick[0] = pVStiffJ->m_dThick[0];
		}
		return;
	}

	////////////////////////////////////////////
	// 1번 거더일 때 (첫번째 수직브레이싱 영역)
	if(pBx->GetNumberGirder()==0)
	{
		// 가로보 기준 좌측
		pVStiffL->m_dWidth[0]		= pVr->m_vW1_G;
		//pVStiffL->m_dWidth[0]		= pVr->m_vW_G;//0;
		pVStiffL->m_dThick[0]		= pVr->m_vT_G;//0;
		pVStiffL->m_dH[LEFT][UP]	= 0;//dHGir;
		pVStiffL->m_dH[LEFT][DOWN]	= 0;

		pVStiffL->m_dWidth[1]		= pVr->m_vW_G;
		pVStiffL->m_dThick[1]		= pVr->m_vT_G;

		pVStiffR->m_dWidth[0]		= pVr->m_vW_G;
		pVStiffR->m_dThick[0]		= pVr->m_vT_G;

		// 거더가 두개일 경우 2번째 거더의 수직보강재를 처리함
		if(pDB->GetGirdersu()==2)
		{
			pVStiffR->m_dWidth[1]		= pVr->m_vW1_G;//0;
			//pVStiffR->m_dWidth[1]		= pVr->m_vW_G;//0;
			pVStiffR->m_dThick[1]		= pVr->m_vT_G;//0;
			pVStiffR->m_dH[RIGHT][UP]	= 0;//dHGir;
			pVStiffR->m_dH[RIGHT][DOWN]	= 0;
		}
	}
	////////////////////////////////////////////
	// 마지막 바로 이전 거더일 때 (마지막 수직브레이싱 영역)
	else if(pBx->GetNumberGirder()==pDB->GetGirdersu()-2)
	{
		// 가로보 기준 좌측
		pVStiffL->m_dWidth[1]		= pVr->m_vW_G;
		pVStiffL->m_dThick[1]		= pVr->m_vT_G;

		// 가로보 기준 우측
		pVStiffR->m_dWidth[0]		= pVr->m_vW_G;
		pVStiffR->m_dThick[0]		= pVr->m_vT_G;

		pVStiffR->m_dWidth[1]		= pVr->m_vW1_G;//0;
		//pVStiffR->m_dWidth[1]		= pVr->m_vW_G;//0;
		pVStiffR->m_dThick[1]		= pVr->m_vT_G;//0;
		pVStiffR->m_dH[RIGHT][UP]	= 0;//dHGir;
		pVStiffR->m_dH[RIGHT][DOWN]	= 0;
	}
	////////////////////////////////////////////
	// 중간위치 거더일때 (중앙부 수직브레이싱 영역)
	else
	{
		// 좌측
		pVStiffL->m_dWidth[1]		= pVr->m_vW_G;
		pVStiffL->m_dThick[1]		= pVr->m_vT_G;

		// 우측
		pVStiffR->m_dWidth[0]		= pVr->m_vW_G;
		pVStiffR->m_dThick[0]		= pVr->m_vT_G;
	}

	if(pDB->IsTUGir() && !pBx->IsJijum())
	{
		pVStiffL->m_dH[RIGHT][UP]	= max(50, pVStiffL->m_dH[RIGHT][UP]);
		pVStiffR->m_dH[LEFT][UP]	= max(50, pVStiffR->m_dH[LEFT][UP]);
	}
}

void CDetVBracingTypeDlg::OnSelchangeComboSteeltypeup() 
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_UPPER);

	//enum { LSTEEL, LVALSTEEL, CSTEEL, CTSTEEL, ISTEEL, HSTEEL, STEELCOUNT };
	//가로보 형강 타입은 뒤에 두개는 없어도 됨...070111....KB
	long nOldType = pMatData->m_nType;
	UpdateData(TRUE);
	long nNewType	= m_comboSteelTypeUp.GetCurSel();	
	
	if (nNewType > 3)
	{
		CString sz;
		sz.Format("수직브레이싱에서 사용할 수 없는 타입입니다.");	
		MessageBox(LPCTSTR(sz), "수직브레이싱 형강 타입", MB_ICONQUESTION|MB_OK);				
		m_comboSteelTypeUp.SetCurSel(nOldType);		
		return;
	}

	pMatData->m_nType	= nNewType;
	pMat->SetSteelMaterialDefault(pMatData, pMatData->m_nType);
	SetSteelMaterialCombo(STEELMAT_UPPER);
	SetDataSaveSteelMaterial();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnSelchangeComboSteeltypelo() 
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_LOWER);

	long nOldType = pMatData->m_nType;
	UpdateData(TRUE);
	long nNewType	= m_comboSteelTypeLo.GetCurSel();	
	
	if (nNewType > 3)
	{
		CString sz;
		sz.Format("수직브레이싱에서 사용할 수 없는 타입입니다.");	
		MessageBox(LPCTSTR(sz), "수직브레이싱 형강 타입", MB_ICONQUESTION|MB_OK);				
		m_comboSteelTypeLo.SetCurSel(nOldType);		
		return;
	}

	pMatData->m_nType	= nNewType;
	
	pMat->SetSteelMaterialDefault(pMatData, pMatData->m_nType);
	SetSteelMaterialCombo(STEELMAT_LOWER);
	SetDataSaveSteelMaterial();
	SetDataInit();
	DrawInputDomyunView(FALSE);
	
}

void CDetVBracingTypeDlg::OnSelchangeComboSteeltypediagonal() 
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= m_pVBracingCurrent->GetSteelMaterialData(STEELMAT_DIAGONAL);
	
	long nOldType = pMatData->m_nType;
	UpdateData(TRUE);
	long nNewType	= m_comboSteelTypeDiagonal.GetCurSel();	
	
	if (nNewType > 3)
	{
		CString sz;
		sz.Format("수직브레이싱에서 사용할 수 없는 타입입니다.");	
		MessageBox(LPCTSTR(sz), "수직브레이싱 형강 타입", MB_ICONQUESTION|MB_OK);				
		m_comboSteelTypeDiagonal.SetCurSel(nOldType);		
		return;
	}

	pMatData->m_nType	= nNewType;
	pMat->SetSteelMaterialDefault(pMatData, pMatData->m_nType);
	SetSteelMaterialCombo(STEELMAT_DIAGONAL);
	SetDataSaveSteelMaterial();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnSelchangeComboSteelmatup() 
{
	SetDataSaveSteelMaterial();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnSelchangeComboSteelmatlo() 
{
	SetDataSaveSteelMaterial();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnSelchangeComboSteelmatdiagonal() 
{
	SetDataSaveSteelMaterial();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetVBracingTypeDlg::OnCheckApplyCalc() 
{
	UpdateData(TRUE);
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();	
	CPlateGirderApp		*pGir	= pDB->GetGirder(m_nG);	
	CPlateBasicIndex	*pBx	= pGir->GetBxVBracingOnly(m_nPos);
	if(pBx==NULL)	return;
	CVBracing			*pBr	= pBx->GetSection()->GetVBracing();

	pBr->SetSectionCheck(m_bApplyCalc.GetCheck());
}

void CDetVBracingTypeDlg::OnButtonAlldefault() 
{
	SetDataDefault();
	SetDataInit();
	DrawInputDomyunView();	
}
