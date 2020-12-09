// DetCrossBeamTypeDlg.cpp : implementation file
//
	
#include "stdafx.h"
#include "GeneralInput.h"
#include "DetCrossBeamTypeDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"
#include "CrossBeamSubInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDetCrossBeamTypeDlg dialog

CDetCrossBeamTypeDlg::CDetCrossBeamTypeDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetCrossBeamTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetCrossBeamTypeDlg)
	m_nCurLevelType  = 0;       // 단면배치방법(0: 수평, 1: 경사)
	m_bTotalView     = FALSE;			// 전체화면 보기
	m_nCurType = CR_TYPE_MANUFACTURE;
	//}}AFX_DATA_INIT
	
	m_nG   = 0; 
	m_nPos = 0;
	m_nCurPos = 0;
	m_pCrossBeamCurrent = NULL;
	COLOR_ADDFIX = ::GetSysColor(COLOR_3DFACE);
}


void CDetCrossBeamTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetCrossBeamTypeDlg)
	DDX_Control(pDX, IDC_CHECK_APPLY_CALC, m_bApplyCalc);
	DDX_Control(pDX, IDC_COMBO_STEELMATERIAL, m_comboSteelMat);
	DDX_Control(pDX, IDC_COMBO_STEELTYPE, m_comboSteelType);
	DDX_Control(pDX, IDC_CUR_CRSBM_COMBO, m_ctlPosCombo);
	DDX_Control(pDX, IDC_CUR_TYPE_COMBO, m_cmbCurType);
	DDX_GridControl(pDX, IDC_GRIDN, m_GridN);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_CBIndex(pDX, IDC_CUR_TYPE_COMBO, m_nCurType);
	DDX_Radio(pDX, IDC_RADIO_LRHEIGHT_HORZ, m_nCurLevelType);
	DDX_Check(pDX, IDC_CHECK_TOTALVIEW, m_bTotalView);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ATYPEDLG_USERDESIGN, m_btnDesignCheck);
	DDX_Control(pDX, IDC_APPLY_ALLCROSSBEAM_SAMEAPPLY_BUTTON2, m_btnAllCrossBeam);
	DDX_Control(pDX, IDC_BUTTON_SAMETYPE, m_btnSameType);
	DDX_Control(pDX, IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON, m_btnAllGirder);
	DDX_Control(pDX, IDC_BUTTON_ALLDEFAULT, m_btnAllCrossBeamBasic);
	DDX_Control(pDX, IDC_BUTTON_JIJUM, m_btnJijum);
	DDX_Control(pDX, IDC_RIST_RECOMMEND, m_btnSetRatio);
	DDX_Control(pDX, IDC_CROSSBEAM_INPUTCOMPLETE, m_btnAllInput);
}

BEGIN_MESSAGE_MAP(CDetCrossBeamTypeDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetCrossBeamTypeDlg)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnDialogNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnDialogPrev)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_CROSSBEAM_INPUTCOMPLETE, OnATypeInputcomplete)
	ON_BN_CLICKED(IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON, OnApplyAllGirderSameApplyButton)
	ON_BN_CLICKED(IDC_RADIO_LRHEIGHT_EQUAL, OnRadioLRHeightEqual)
	ON_BN_CLICKED(IDC_RADIO_LRHEIGHT_HORZ, OnRadioLRHeightHorz)
	ON_CBN_SELCHANGE(IDC_CUR_CRSBM_COMBO, OnSelchangeCurPos)
	ON_BN_CLICKED(IDC_CHECK_TOTALVIEW, OnCheckTotalview)
	ON_BN_CLICKED(IDC_ATYPEDLG_USERDESIGN, OnDlgUserDesign)
	ON_BN_CLICKED(IDC_APPLY_ALLCROSSBEAM_SAMEAPPLY_BUTTON2, OnApplyAllcrossbeamSameapplyButton2)
	ON_BN_CLICKED(IDC_BUTTON_SAMETYPE, OnButtonSametype)
	ON_BN_CLICKED(IDC_BUTTON_JIJUM, OnButtonJijum)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RIST_RECOMMEND, OnRistRecommend)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELTYPE, OnSelchangeComboSteeltype)
	ON_CBN_SELCHANGE(IDC_COMBO_STEELMATERIAL, OnSelchangeComboSteelmaterial)
	ON_BN_CLICKED(IDC_CHECK_APPLY_CALC, OnCheckApplyCalc)
	ON_BN_CLICKED(IDC_BUTTON_ALLDEFAULT, OnButtonAlldefault)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CUR_TYPE_COMBO, OnSelchangeCurTypeCombo)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRIDN, OnCellChangedRowGridN)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRIDN, OnCellChangedDataGridN)
	ON_NOTIFY(GVN_CELL_BUTTONCLICK,	IDC_GRID, OnCellButtonClickRecommend)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedRowGrid)

	ON_MESSAGE(WM_GET_NEARBX, OnClickTotalView)
	ON_MESSAGE(WM_DATA_CHANGED, OnCrossBeamSubInput)
	ON_MESSAGE(WM_SUBINPUT_CLOSE, OnCrossBeamSubInputClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetCrossBeamTypeDlg message handlers
BOOL CDetCrossBeamTypeDlg::PreTranslateMessage(MSG* pMsg)
{
	CStringArray strCombo;
	if(pMsg->hwnd==m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		long nRow = next.row;
		long nCol = next.col;
		// 수직 보강재
		if(nRow==1 && nCol==1)
		{
			strCombo.Add("없음");
			strCombo.Add("1");		strCombo.Add("2");
			strCombo.Add("3");		strCombo.Add("4");
			strCombo.Add("5");		strCombo.Add("6");
			m_Grid.SetComboString(strCombo);
		}
		if(nRow==1 && nCol==2)
		{
			strCombo.Add("양면");
			strCombo.Add("전면");
			strCombo.Add("후면");
			m_Grid.SetComboString(strCombo);
		}
		// 슬래브 앵커
		if(nRow==2)
		{
			strCombo.Add("없음");
			strCombo.Add("1");		strCombo.Add("2");
			strCombo.Add("3");		strCombo.Add("4");
			strCombo.Add("5");		strCombo.Add("6");
			m_Grid.SetComboString(strCombo);
		}
		// 수평보강재
		if((nRow==3 && nCol==2) || (nRow==4 && nCol==2))
		{
			strCombo.Add("없음");
			strCombo.Add("1단");
			strCombo.Add("2단");
			m_Grid.SetComboString(strCombo);
		}
		// 이음판
		if(nRow==5)
		{
			strCombo.Add("없음");
			strCombo.Add("좌측적용");
			strCombo.Add("우측적용");
			strCombo.Add("양측적용");
			m_Grid.SetComboString(strCombo);
		}
		// 브라켓 보강판
		if(nRow==6)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");
			m_Grid.SetComboString(strCombo);
		}
		// 채움판
		if(nRow==7)
		{
			strCombo.Add("없음");
			strCombo.Add("한쪽면");
			strCombo.Add("양쪽면");
			m_Grid.SetComboString(strCombo);
		}
		// 모멘트 이음판
		if(nRow==8)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");
			m_Grid.SetComboString(strCombo);
		}
		// 커넥션 플레이트
		if(nRow==9)
		{
			strCombo.Add("미적용");
			strCombo.Add("내외측적용");
			strCombo.Add("외측적용");
			m_Grid.SetComboString(strCombo);
		}
		// 볼트설정
		if(nRow==10 && nCol==1)
		{
			//	Bolt Size
			CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
			CPlateCrossBeam		*pC		= m_pCrossBeamCurrent;
			CPlateBasicIndex	*pBx	= pC->GetBx();
			CSteelBolt		*pSteelBolt	= pDB->m_pDataManage->GetSteelBolt();

			CString szBoltGrade = pDB->GetMaterial(CG_BOLT, pBx);

			for(long nIdx=0; nIdx<pSteelBolt->GetSize(); nIdx++)
			{
				if(pSteelBolt->GetStringData(nIdx, BDIB_GRADE) == szBoltGrade)
					strCombo.Add(pSteelBolt->GetStringData(nIdx, BDIB_SIZE));
			}
			m_Grid.SetComboString(strCombo);
		}
		// 볼트기본간격
		if(nRow==11 && nCol==1)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");
			m_Grid.SetComboString(strCombo);
		}

		// 가로보헌치상면접합
		if(nRow==12)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");			
			m_Grid.SetComboString(strCombo);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDetCrossBeamTypeDlg::IsValid()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nCrossBeamCount = 0;

	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++) 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec != NULL && pSec->GetenDetType() == SECDET_CROSSBEAM)
				nCrossBeamCount++;

			pBx = Finder.GetBxNext();
		}
	}

	if(nCrossBeamCount > 0)
		return TRUE;

	m_bUsedFlag = FALSE;
	return FALSE;
}

BOOL CDetCrossBeamTypeDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();

	CPlateBridgeApp  *pDB	  = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(m_nPos);
	if(pBx==NULL)	return TRUE;

	CPlateCrossBeam	*pC = pBx->GetSection()->GetCrossBeam();

	m_nCurType	= pC->GetType();
	UpdateData(FALSE);

	// Set Grid
	SetGrid2Titel();
	SetGrid2DataInit();

	CButton* pButton = (CButton*)GetDlgItem(IDC_RIST_RECOMMEND);
	if(!m_BitMap.GetSafeHandle())
	{
		m_BitMap.LoadBitmap(IDB_BITMAP_GIRDERRATE);
	}
	pButton->ModifyStyle(0,BS_BITMAP);
	pButton->SetBitmap(m_BitMap);

	SetResize(IDC_ATYPEDLG_USERDESIGN,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_APPLY_ALLCROSSBEAM_SAMEAPPLY_BUTTON2,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAMETYPE,						SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ALLDEFAULT,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_JIJUM,							SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RIST_RECOMMEND,						SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CROSSBEAM_INPUTCOMPLETE,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRIDN,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_TOTALVIEW,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_APPLY_CALC,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_GROUP4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_LRHEIGHT_HORZ, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_LRHEIGHT_EQUAL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_GRID,	SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
	
	return TRUE;
}


void CDetCrossBeamTypeDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	nRow -= (nRow % 2);
	CString str = m_GridN.GetItemText(nRow,nCol);

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
	if( str == "uB1" || str == "uC") str = "uB1@uC";
	if( str == "dB1" || str == "dC") str = "dB1@dC";
	if( str == "uE1" || str == "uF1") str = "uE1@uF1";
	if( str == "dE1" || str == "dF1") str = "dE1@dF1";
	m_pView->GetDomyun()->STMakeCursor(str);
}


void CDetCrossBeamTypeDlg::SetBoltCombo(long nG, long nPos)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(nPos >= pDB->GetGirder(m_nG)->GetQtyCrossBeamOnly())	return;

	UpdateData(TRUE);
}


void CDetCrossBeamTypeDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);	
  	CARoadOptionStd *pOptStd = m_pStd->m_pDataManage->GetOptionStd();

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	//
	CDomyun Dom(pDom);
	Dom.SetScaleDim(30);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CAPlateDrawCross DrawCross(m_pStd->GetDataManage());	
	CAPlateDrawDanmyun DrawDanmyun(m_pStd->m_pDataManage);

	if(!m_pCrossBeamCurrent)	return;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = m_pCrossBeamCurrent->GetBx();

	if(pBx==NULL)	return;

	CDPoint xyTotalBndLT, xyTotalBndRD;
	if(m_nTab==0)
	{
		//가로보타입
		CPlateCrossBeam* pC = m_pCrossBeamCurrent;
		if(!pC) return;

		DrawCross.DrawInputCRBeamFront(&Dom, pBx);
		CDRect BoundRect1	= Dom.GetExtRect();
		*pDom << Dom;

		DrawCross.DrawInputCRBeamPlan(&Dom, pBx);
		Dom.SetCalcExtRect();
		CDRect BoundRect2 = Dom.GetExtRect();
		double dMoveY = BoundRect1.bottom-BoundRect2.top;
		Dom.Move(0, dMoveY+Dom.Always(5));
		// 전체 도면영역잡기
		Dom.SetCalcExtRect();
		BoundRect2 = Dom.GetExtRect();
		xyTotalBndLT = BoundRect2.BottomLeft();
		xyTotalBndRD = BoundRect1.TopRight();
		*pDom << Dom;
		//
		if(!pC->m_nLGirSpType || !pC->m_nRGirSpType) 
		{
			DrawCross.DrawInputCRBeamSangse(&Dom, pBx);
			*pDom << Dom;
		}

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
		//가로보타입
		double BX=0,BY=0;
		DrawCross.DimCRBeamFrontTop(&Dom,pBx,BX,BY);
		DrawCross.DimCRBeamFrontBottom(&Dom,pBx,BX,BY);
		DrawCross.DimCRBeamFrontRight(&Dom,pBx,BX,BY);
		DrawCross.DimCRBeamFrontLeft(&Dom,pBx,BX,BY);
		// 전체 도면영역잡기
	
		Dom.SetCalcExtRect();
		CDRect BoundRect = Dom.GetExtRect();

		pOptStd->SetEnvType(&Dom,HCAD_SMALL_TITLE);
		//Dom.SetTextHeight(2.0);

		CString strTitle = _T("");
		for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			double dGirCenL = pGir->m_dGirderCenter;
			pGir = pDB->GetGirder(nG+1);
			double dGirCenR = pGir->m_dGirderCenter;				
			
			strTitle.Format("< G%d-G%d >", nG+1, nG+2);
			Dom.TextOut((dGirCenL+dGirCenR)/2, BoundRect.bottom+Dom.GetTextHeight(), strTitle);
		}


		Dom.SetCalcExtRect();
		BoundRect = Dom.GetExtRect();

		xyTotalBndLT = BoundRect.BottomLeft();
		xyTotalBndRD = BoundRect.TopRight();

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

void CDetCrossBeamTypeDlg::SetGridTitle()
{

}

void CDetCrossBeamTypeDlg::SetGrid2Titel()
{
	long nRows = 13;//070322...가로보헌치 상면 접합 추가..
	long nCols = 3;
	
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 95);
	m_Grid.SetColumnWidth(1, 60);
	m_Grid.SetColumnWidth(2, 68);
	m_Grid.ExpandColumnsToFit();
	
	for(long n=0;n<nRows;n++)			m_Grid.SetCellFixType(n, 0, GVN_ONECELL_COLFIX);
	m_Grid.SetCellFixType(0, 1, GVN_ONECELL_COLFIX);	m_Grid.SetCellFixType(0, 2, GVN_ONECELL_COLFIX);
	m_Grid.SetCellFixType(3, 1, GVN_ONECELL_COLFIX);	m_Grid.SetCellFixType(4, 1, GVN_ONECELL_COLFIX);
	
	m_Grid.SetTextMatrix(0, 0, "항 목");
	m_Grid.SetTextMatrix(0, 1, "설 정");			m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetTextMatrix(1, 0, "수직 보강재");		m_Grid.SetCellType(1, 1, CT_COMBO);	m_Grid.SetCellType(1, 2, CT_COMBO);
	m_Grid.SetTextMatrix(2, 0, "슬래브 앵커");		m_Grid.SetCellType(2, 1, CT_COMBO);	m_Grid.SetMergeCol(2, 1, 2);
	m_Grid.SetTextMatrix(3, 0, "수평 보강재");		m_Grid.SetMergeRow(3, 4, 0);
	m_Grid.SetTextMatrix(3, 1, "상부");				m_Grid.SetCellType(3, 2, CT_COMBO);
	m_Grid.SetTextMatrix(4, 1, "하부");				m_Grid.SetCellType(4, 2, CT_COMBO);
	m_Grid.SetTextMatrix(5, 0, "이음판");			m_Grid.SetCellType(5, 1, CT_COMBO);	m_Grid.SetMergeCol(5, 1, 2);
	m_Grid.SetTextMatrix(6, 0, "브라켓 보강판");
	m_Grid.SetCellType(6, 1, CT_BUTTON);			m_Grid.SetTextMatrix(6, 1, "권고안");
	m_Grid.SetCellType(6, 2, CT_COMBO);	
	m_Grid.SetTextMatrix(7, 0, "채움판");			m_Grid.SetCellType(7, 1, CT_COMBO);	m_Grid.SetMergeCol(7, 1, 2);
	m_Grid.SetTextMatrix(8, 0, "모멘트 이음판");
	m_Grid.SetCellType(8, 1, CT_BUTTON);			m_Grid.SetTextMatrix(8, 1, "권고안");
	m_Grid.SetCellType(8, 2, CT_COMBO);
	m_Grid.SetTextMatrix(9, 0, "커넥션 플레이트");
	m_Grid.SetCellType(9, 1, CT_BUTTON);			m_Grid.SetTextMatrix(9, 1, "권고안");
	m_Grid.SetCellType(9, 2, CT_COMBO);	
	m_Grid.SetTextMatrix(10, 0, "볼트 설정");		m_Grid.SetCellType(10,1, CT_COMBO);	m_Grid.SetMergeCol(10,1, 2);
	m_Grid.SetTextMatrix(10, 2, "볼트길이계산");
	m_Grid.SetTextMatrix(11, 0, "볼트 기본간격");	m_Grid.SetCellType(11,1, CT_COMBO);
	
	m_Grid.SetTextMatrix(12, 0, "헌치 상면 접합");
	m_Grid.SetCellType(12, 1, CT_BUTTON);			m_Grid.SetTextMatrix(12, 1, "권고안");
	m_Grid.SetCellType(12, 2, CT_COMBO);	
	
	
	
	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.ExpandToFit();
}

void CDetCrossBeamTypeDlg::SetGrid2DataInit()
{
	
	SetGrid2Titel();

	CString	sText;
	
	if(m_pCrossBeamCurrent->m_nVStiffsu_CR==0)
	{
		m_Grid.SetTextMatrix(1, 1, "없음");
		SetCellReadOnly(m_Grid, 1, 2, TRUE);
	}
	else
	{
		sText.Format("%d", m_pCrossBeamCurrent->m_nVStiffsu_CR);
		m_Grid.SetTextMatrix(1, 1, sText);
		SetCellReadOnly(m_Grid, 1, 2, FALSE);
	}
	if(m_pCrossBeamCurrent->m_cVSide_CR==0)		m_Grid.SetTextMatrix(1, 2, "양면");
	else if(m_pCrossBeamCurrent->m_cVSide_CR==1)	m_Grid.SetTextMatrix(1, 2, "전면");
	else if(m_pCrossBeamCurrent->m_cVSide_CR==2)	m_Grid.SetTextMatrix(1, 2, "후면");
	
	if(m_pCrossBeamCurrent->m_nAnchorsu_CR==0)	m_Grid.SetTextMatrix(2, 1, "없음");
	else								m_Grid.SetTextMatrixFormat(2, 1, "%d", m_pCrossBeamCurrent->m_nAnchorsu_CR);
	
	sText.Format("%d단", m_pCrossBeamCurrent->m_nHStiffUppersu_CR);
	if(m_pCrossBeamCurrent->m_nHStiffUppersu_CR==0)	m_Grid.SetTextMatrix(3, 2, "없음");
	else										m_Grid.SetTextMatrix(3, 2, sText);
	sText.Format("%d단", m_pCrossBeamCurrent->m_nHStiffLowersu_CR);
	if(m_pCrossBeamCurrent->m_nHStiffLowersu_CR==0)	m_Grid.SetTextMatrix(4, 2, "없음");
	else										m_Grid.SetTextMatrix(4, 2, sText);
	
	if(m_pCrossBeamCurrent->m_nLGirSpType==0&&m_pCrossBeamCurrent->m_nRGirSpType==0)			m_Grid.SetTextMatrix(5, 1, "없음");
	else if(m_pCrossBeamCurrent->m_nLGirSpType==1&&m_pCrossBeamCurrent->m_nRGirSpType==0)		m_Grid.SetTextMatrix(5, 1, "좌측적용");
	else if(m_pCrossBeamCurrent->m_nLGirSpType==0&&m_pCrossBeamCurrent->m_nRGirSpType==1)		m_Grid.SetTextMatrix(5, 1, "우측적용");
	else if(m_pCrossBeamCurrent->m_nLGirSpType==1&&m_pCrossBeamCurrent->m_nRGirSpType==1)		m_Grid.SetTextMatrix(5, 1, "양측적용");
	
	if(m_pCrossBeamCurrent->m_cGasset_CR==0)		m_Grid.SetTextMatrix(6, 2, "미적용");
	else if(m_pCrossBeamCurrent->m_cGasset_CR==1)	m_Grid.SetTextMatrix(6, 2, "적용");
	if(m_pCrossBeamCurrent->GetType()==CR_TYPE_VSTIFF)	SetCellReadOnly(m_Grid, 6, 2, TRUE);
	else										SetCellReadOnly(m_Grid, 6, 2, FALSE);
	
	if(m_pCrossBeamCurrent->m_nFillerUseType==0)	m_Grid.SetTextMatrix(7, 1, "없음");
	else if(m_pCrossBeamCurrent->m_nFillerUseType==1)	m_Grid.SetTextMatrix(7, 1, "한쪽면");
	else if(m_pCrossBeamCurrent->m_nFillerUseType==2)	m_Grid.SetTextMatrix(7, 1, "양쪽면");
	
	if(m_pCrossBeamCurrent->m_nLGirSpType || m_pCrossBeamCurrent->m_nRGirSpType)
		SetCellReadOnly(m_Grid, 8, 2, FALSE);
	else
		SetCellReadOnly(m_Grid, 8, 2, TRUE);
	if(m_pCrossBeamCurrent->m_bMomentPan)	m_Grid.SetTextMatrix(8, 2, "적용");
	else							m_Grid.SetTextMatrix(8, 2, "미적용");
	
	if(!m_pCrossBeamCurrent->m_bConnectionPlate)	m_Grid.SetTextMatrix(9, 2, "미적용");
	else
	{
		if(m_pCrossBeamCurrent->m_nRadioConnectionPlateInOut==0)	m_Grid.SetTextMatrix(9, 2, "내외측적용");
		else												m_Grid.SetTextMatrix(9, 2, "외측적용");
	}	
	
	m_Grid.SetTextMatrix(10, 1, m_pCrossBeamCurrent->m_ZzsBoltSize);
	
	if(m_pCrossBeamCurrent->m_bBoltInterver)
	{
		m_Grid.SetTextMatrix(11, 1, "적용");
		SetCellReadOnly(m_Grid, 11, 2, FALSE);
	}
	else
	{
		m_Grid.SetTextMatrix(11, 1, "미적용");
		SetCellReadOnly(m_Grid, 11, 2, TRUE);
	}
	
	m_Grid.SetTextMatrix(11, 2, "%.0f", m_pCrossBeamCurrent->m_dBoltInterver);
	
	
	//가로보 헌치 상면 접합 여부...
	if(m_pCrossBeamCurrent->m_bHunchTopConnect)	m_Grid.SetTextMatrix(12, 2, "적용");
	else									m_Grid.SetTextMatrix(12, 2, "미적용");
	
	
	if(m_pCrossBeamCurrent->GetType()==CR_TYPE_VSTIFF)//수직보강재 연결 타입이면 
		SetCellReadOnly(m_Grid, 12, 2, TRUE);
	else if(m_pCrossBeamCurrent->m_nLevelType == 0)//단면배치 수평이면..
		SetCellReadOnly(m_Grid, 12, 2, TRUE);
	else
		SetCellReadOnly(m_Grid, 12, 2, FALSE);
	
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDetCrossBeamTypeDlg::SetGrid2DataSave()
{
	CString	sText;
	
	sText	= m_Grid.GetTextMatrix(1, 1);
	if(sText=="없음")
		m_pCrossBeamCurrent->m_nVStiffsu_CR = 0;
	else
	{
		sText = m_Grid.GetTextMatrix(1, 1);
		m_pCrossBeamCurrent->m_nVStiffsu_CR = atoi(sText);
		sText = m_Grid.GetTextMatrix(1, 2);
		if(sText=="양면")		m_pCrossBeamCurrent->m_cVSide_CR = 0;
		else if(sText=="전면")	m_pCrossBeamCurrent->m_cVSide_CR = 1;
		else if(sText=="후면")	m_pCrossBeamCurrent->m_cVSide_CR = 2;
	}
	
	m_pCrossBeamCurrent->m_nAnchorsu_CR			= GetGrid2longValue(m_Grid.GetTextMatrix(2, 1));
	m_pCrossBeamCurrent->m_nHStiffUppersu_CR	= GetGrid2longValue(m_Grid.GetTextMatrix(3, 2));
	m_pCrossBeamCurrent->m_nHStiffLowersu_CR	= GetGrid2longValue(m_Grid.GetTextMatrix(4, 2));
	
	sText = m_Grid.GetTextMatrix(5, 1);
	if(sText=="없음")
	{
		m_pCrossBeamCurrent->m_nLGirSpType = 0;
		m_pCrossBeamCurrent->m_nRGirSpType = 0;
	}
	else if(sText=="좌측적용")
	{
		m_pCrossBeamCurrent->m_nLGirSpType = 1;
		m_pCrossBeamCurrent->m_nRGirSpType = 0;
	}
	else if(sText=="우측적용")
	{
		m_pCrossBeamCurrent->m_nLGirSpType = 0;
		m_pCrossBeamCurrent->m_nRGirSpType = 1;
	}
	else if(sText=="양측적용")
	{
		m_pCrossBeamCurrent->m_nLGirSpType = 1;
		m_pCrossBeamCurrent->m_nRGirSpType = 1;
	}
	
	m_pCrossBeamCurrent->m_cGasset_CR = GetGrid2BoolValue(m_Grid.GetTextMatrix(6, 2));
	
	sText = m_Grid.GetTextMatrix(7, 1);
	if(sText=="없음")			m_pCrossBeamCurrent->m_nFillerUseType = 0;
	else if(sText=="한쪽면")	m_pCrossBeamCurrent->m_nFillerUseType = 1;
	else if(sText=="양쪽면")	m_pCrossBeamCurrent->m_nFillerUseType = 2;
	
	m_pCrossBeamCurrent->m_bMomentPan = GetGrid2BoolValue(m_Grid.GetTextMatrix(8, 2));
	
	sText = m_Grid.GetTextMatrix(9, 2);
	if(sText=="미적용")	m_pCrossBeamCurrent->m_bConnectionPlate = FALSE;
	else
	{
		m_pCrossBeamCurrent->m_bConnectionPlate = TRUE;
		if(sText=="내외측적용")		m_pCrossBeamCurrent->m_nRadioConnectionPlateInOut = 0;
		else if(sText=="외측적용")	m_pCrossBeamCurrent->m_nRadioConnectionPlateInOut = 1;
	}
	
	m_pCrossBeamCurrent->m_ZzsBoltSize = m_Grid.GetTextMatrix(10, 1);
	m_pCrossBeamCurrent->m_bBoltInterver = GetGrid2BoolValue(m_Grid.GetTextMatrix(11, 1));
	m_pCrossBeamCurrent->m_dBoltInterver = m_Grid.GetTextMatrixDouble(11, 2);
	
	m_pCrossBeamCurrent->m_bHunchTopConnect = GetGrid2BoolValue(m_Grid.GetTextMatrix(12, 2));
}

long CDetCrossBeamTypeDlg::GetGrid2longValue(CString szValue)
{
	long nValue = 0;
	if(szValue=="없음")	nValue = 0;
	else nValue = atoi(szValue);
	
	return nValue;
}

BOOL CDetCrossBeamTypeDlg::GetGrid2BoolValue(CString szValue)
{
	if(szValue=="적용")	return TRUE;
	else				return FALSE;
}

void CDetCrossBeamTypeDlg::Grid2RecommendBracketHeight()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateCrossBeam		*pC		= m_pCrossBeamCurrent;
	CPlateBasicIndex	*pBx	= pC->GetBx();
	CPlateGirderApp		*pGir	= pDB->GetGirder(pBx->GetNumberGirder());
	CPlateGirderApp		*pGirR	= pDB->GetGirder(pBx->GetNumberGirder()+1);
	CPlateBasicIndex	*pBxR	= pBx->GetBxMatchByCrossBeam(pGirR);
	
	double	dStation	= pBx->GetStation();
	double	dStationR	= pBxR->GetStation();
	double	dHeightGir  = pGir->GetHeightGirderByStaAng(dStation);
	double	dHeightGirR = pGirR->GetHeightGirderByStaAng(dStationR);
	
	//좌측보강재권고안
	CDPoint	vDir		= pC->GetVectorCross();
	double	dGussetH	= pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	pC->m_dltH    = dHeightGir-pC->m_T1_CR-pC->m_T2_CR-pC->m_dLH1-pC->m_H_CR+pC->GetLengthGirWebToCrWeb(TRUE)*vDir.y-dGussetH;
	if(pC->m_dLH1==0)
		pC->m_dltH = dHeightGir-pC->m_T1_CR-pC->m_H_CR-(pC->m_dbL2+pC->m_dbL3+pC->m_dbL1)*vDir.y-dGussetH;
	pC->m_ltF = 100;	
	pC->m_ltE = (long)((pC->m_dltH-2*100)/pC->m_ltF);
	//우측보강재권고안
	vDir  = pC->GetVectorCross();
	pC->m_drtH    = dHeightGir-pC->m_T1_CR-pC->m_T2_CR-pC->m_dRH1-pC->m_H_CR-pC->GetLengthGirWebToCrWeb(FALSE)*vDir.y-dGussetH;
	if(pC->m_dRH1==0)
		pC->m_drtH = dHeightGirR-pC->m_T1_CR-pC->m_H_CR+(pC->m_dbR2+pC->m_dbR3+pC->m_dbR1)*vDir.y-dGussetH;
	pC->m_rtF = 100;	
	pC->m_rtE = (long)((pC->m_drtH-2*100)/pC->m_rtF);
}

void CDetCrossBeamTypeDlg::Grid2RecommendMoment()
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CSteelBolt			*pBolt		= m_pStd->GetDataManage()->GetSteelBolt();
	CPlateCrossBeam		*pC			= m_pCrossBeamCurrent;
	
	// 복부 이음판만 다시 설정
	double Height	  = pC->m_H_CR;
	double PlateSpace = 15; // 모멘트판과 전단판의 간격
	
    CString str;
	str.Format("%s %s", pDB->GetMaterial(CG_BOLT, pC->GetBx()), pC->m_ZzsBoltSize);
	long idx = pBolt->GetIndex(str);
	
	pC->m_mA	= pBolt->GetCalcEdgeDistance(idx);
	pC->m_mB	=  2;
	pC->m_mC	= 85;
	pC->m_mC1	= 85;
	pC->m_mG	= 50;
	pC->m_mD	= pC->m_mG + pC->m_mA;
	pC->m_mE	=  2;
	pC->m_mF	= 100;
	pC->m_mW	= (pC->m_mA*2 + pC->m_mE*pC->m_mF);
	
	pC->m_sF	= 100;
	pC->m_sE	= (long)((Height - (pC->m_mG+pC->m_mW+PlateSpace)*2)/pC->m_sF-1);
	pC->m_sW	= pC->m_sE*pC->m_sF+pC->m_sA*2;
	
	pC->m_mT	= pC->m_sT;
	double  T4  = pC->m_mT*2+pC->m_T3_CR;	// 복부
	pC->m_mL = pBolt->GetBoltLength(pC->GetBx(), CG_BOLT, pC->m_ZzsBoltSize,T4);
}

void CDetCrossBeamTypeDlg::Grid2RecommendConnectionPlate()
{
	CPlateCrossBeam	*pC	= m_pCrossBeamCurrent;
	
	if(pC->GetType() == CR_TYPE_MANUFACTURE)
	{
		pC->m_P_CR	= 600;		pC->m_vW_CR	= 250;
		pC->m_vW1_G	= 0;		pC->m_uW	= 240;
		pC->m_uM1	= 280;		pC->m_uN_CR	= 100;
		pC->m_uE1	= 2;		pC->m_uF1	= 80;
		pC->m_uD1	= 40;		pC->m_uA	= 40;
		pC->m_uB1	= 2;		pC->m_uC	= 80;
		pC->m_uC1	= 100;		pC->m_uB	= 2;
		pC->m_uM_CR	= 200;		pC->m_uE	= 1;
		pC->m_uF	= 80;		pC->m_uD	= 40;
		
		pC->m_dW	= 240;		pC->m_dM1	= 280;
		pC->m_dN_CR	= 100;		pC->m_dE1	= 2;
		pC->m_dF1	= 80;		pC->m_dD1	= 40;
		pC->m_dA	= 40;		pC->m_dB1	= 2;
		pC->m_dC	= 80;		pC->m_dC1	= 100;
		pC->m_dB	= 2;		pC->m_dM_CR	= 200;
		pC->m_dE	= 1;		pC->m_dF	= 80;
		pC->m_dD	= 40;
	}
	else if(pC->GetType() == CR_TYPE_VSTIFF)
	{
		pC->m_P_CR	= 250;		pC->m_vW_CR	= 250;
		pC->m_vW1_G	= 0;		pC->m_uW	= 240;
		pC->m_uM1	= 280;		pC->m_uN_CR	= 100;
		pC->m_uE1	= 2;		pC->m_uF1	= 80;
		pC->m_uD1	= 40;		pC->m_uA	= 40;
		pC->m_uB1	= 1;		pC->m_uC	= 80;
		pC->m_uC1	= 100;		pC->m_uB	= 2;
		pC->m_uM_CR	= 200;		pC->m_uE	= 1;
		pC->m_uF	= 80;		pC->m_uD	= 40;
		
		pC->m_dW	= 240;		pC->m_dM1	= 280;
		pC->m_dN_CR	= 100;		pC->m_dE1	= 2;
		pC->m_dF1	= 80;		pC->m_dD1	= 40;
		pC->m_dA	= 40;		pC->m_dB1	= 1;
		pC->m_dC	= 80;		pC->m_dC1	= 100;
		pC->m_dB	= 2;		pC->m_dM_CR	= 200;
		pC->m_dE	= 1;		pC->m_dF	= 80;
		pC->m_dD	= 40;
	}
	else if(pC->GetType() == CR_TYPE_HSTEEL)
		pC->m_bConnectionPlate = FALSE;
}

void CDetCrossBeamTypeDlg::Grid2RecommendHunchTopConnect()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateCrossBeam		*pC		= m_pCrossBeamCurrent;
	CPlateBasicIndex	*pBx	= pC->GetBx();
	
	BOOL bCurrent	= m_pCrossBeamCurrent->m_bHunchTopConnect;//현재 적용/비적용 여부...
	
	if(pC->GetType()==CR_TYPE_VSTIFF)//수직보강재 연결 타입이면 
		pC->m_bHunchTopConnect = FALSE;
	else if(pC->m_nLevelType == 0)//단면배치 수평이면..
		pC->m_bHunchTopConnect = FALSE;
	else if (pBx->IsJijum() && !pDB->IsTUGir())//단부,지점부이면..
		pC->m_bHunchTopConnect = TRUE;
	else
		pC->m_bHunchTopConnect = FALSE;
	
	if (bCurrent != m_pCrossBeamCurrent->m_bHunchTopConnect)
		SetGrid2DataInit();
}

void CDetCrossBeamTypeDlg::OnCellButtonClickRecommend(NMHDR* nmgv, LRESULT*)
{
	CGeneralBaseDlg		*pDlg	= m_pStd->GetCurruntDlg();
	NM_GRIDVIEW			*pCell	= (NM_GRIDVIEW *)nmgv;
	
	long	nRow	= pCell->iRow;
	long	nCol	= pCell->iColumn;
	
	if(!m_pCrossBeamCurrent)	return;
	if(nRow==6 && nCol==1)
	{
		if(m_pCrossBeamCurrent->m_cGasset_CR)	Grid2RecommendBracketHeight();
		else
		{
			if(AfxMessageBox("브라켓 보강판이 적용되지 않았습니다. 브라켓 보강판을 설치하시겠습니까?", MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				m_pCrossBeamCurrent->m_cGasset_CR	= 1;
				Grid2RecommendBracketHeight();
			}
		}
	}
	if(nRow==8 && nCol==1)
	{
		if(m_pCrossBeamCurrent->m_bMomentPan)	Grid2RecommendMoment();
		else
		{
			if(AfxMessageBox("모멘트 이음판이 적용되지 않았습니다. 모멘트 이음판을 설치하시겠습니까?", MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				m_pCrossBeamCurrent->m_bMomentPan	=  TRUE;
				Grid2RecommendMoment();
			}
		}
	}
	if(nRow==9 && nCol==1)
	{
		if(m_pCrossBeamCurrent->m_bConnectionPlate)	Grid2RecommendConnectionPlate();
		else
		{
			if(AfxMessageBox("커넥션플레이트가 적용되지 않았습니다. 커넥션플레이트를 설치하시겠습니까?", MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				m_pCrossBeamCurrent->m_bConnectionPlate	= TRUE;
				Grid2RecommendConnectionPlate();
			}
		}
	}
	
	if(nRow==12 && nCol==1)
	{
		//권고안이 눌렸을땐 해당 가로보에서 적용인지 비적용인지 처리...		
		//수직보강재 연결 타입이아니고 단면배치 수평이 아니면..
		if(m_pCrossBeamCurrent->GetType()!=CR_TYPE_VSTIFF && m_pCrossBeamCurrent->m_nLevelType != 0)
			Grid2RecommendHunchTopConnect();	
	}
	SetGrid2DataInit();
	
	if(pDlg) pDlg->SendMessage(WM_DATA_CHANGED, (WPARAM)m_pCrossBeamCurrent, 1);
}

void CDetCrossBeamTypeDlg::OnCellChangedRowGrid(NMHDR* nmgv, LRESULT*)
{
	CGeneralBaseDlg *pDlg = m_pStd->GetCurruntDlg();
	NM_GRIDVIEW *pCell = (NM_GRIDVIEW *)nmgv;
	
	long	nRow	= pCell->iRow;
	
	SetGrid2DataSave();
	SetGrid2DataInit();
	if(pDlg)
		pDlg->SendMessage(WM_DATA_CHANGED, (WPARAM)m_pCrossBeamCurrent, nRow);

}

long CDetCrossBeamTypeDlg::GetRowSize()
{
	long nRow=2; /*공통*/
	if(m_pCrossBeamCurrent->m_nAnchorsu_CR != 0) nRow+=2;// 슬래브 앵커
	if(m_pCrossBeamCurrent->m_nVStiffsu_CR != 0) nRow+=2;// 수직보강재
	if(m_pCrossBeamCurrent->m_nHStiffUppersu_CR != 0 || m_pCrossBeamCurrent->m_nHStiffLowersu_CR != 0) nRow+=2;// 수평보강재
	if(!m_pCrossBeamCurrent->m_nLGirSpType || !m_pCrossBeamCurrent->m_nRGirSpType)	nRow+=2;// 볼트 연결시
	if(m_pCrossBeamCurrent->m_cGasset_CR) nRow+=2;//브라켓 보강판
	if(m_pCrossBeamCurrent->m_nLGirSpType || m_pCrossBeamCurrent->m_nRGirSpType)
	{
		nRow+=6; // 브라켓 상부, 하부플랜지, 전단이음판
		if(m_pCrossBeamCurrent->m_bMomentPan)
			nRow+=2; // 모멘트이음판
	}	
	return nRow;
}


long CDetCrossBeamTypeDlg::GetColsSize()
{
	long nCol=16; /*공통*/

	if(m_pCrossBeamCurrent->m_nAnchorsu_CR != 0) nCol = max(nCol,m_pCrossBeamCurrent->m_nAnchorsu_CR+1);// 슬래브 앵커
	if(m_pCrossBeamCurrent->m_nVStiffsu_CR != 0) nCol = max(nCol,m_pCrossBeamCurrent->m_nVStiffsu_CR+4);// 수직보강재
	if(m_pCrossBeamCurrent->m_nHStiffUppersu_CR != 0 || m_pCrossBeamCurrent->m_nHStiffLowersu_CR != 0) nCol = max(nCol,3);// 수평보강재
	if(!m_pCrossBeamCurrent->m_nLGirSpType || !m_pCrossBeamCurrent->m_nRGirSpType)	nCol = max(nCol,14);// 볼트 연결시
	if(m_pCrossBeamCurrent->m_cGasset_CR) nCol = max(nCol,7);//브라켓 보강판
	if(m_pCrossBeamCurrent->m_nLGirSpType || m_pCrossBeamCurrent->m_nRGirSpType)
	{
		nCol = max(nCol,18); // 브라켓 상부, 하부플랜지, 전단이음판
		if(m_pCrossBeamCurrent->m_bMomentPan)
			nCol = max(nCol,12);// 모멘트이음판
	}	
	return nCol;
}


void CDetCrossBeamTypeDlg::SetDataInitGridN()
{
	// 공통
	m_GridN.ResetAttrib();
	m_GridN.SetRowCount(GetRowSize());
	m_GridN.SetColumnCount(GetColsSize());
		
	m_GridN.SetEditable(TRUE);
	m_GridN.SetListMode(FALSE);
	m_GridN.EnableDragAndDrop(TRUE);
	m_GridN.EnableInsertRow(FALSE);
	m_GridN.EnableInsertCol(FALSE);	
	m_GridN.SetTextBkColor(RGB(225, 250, 250));
	m_GridN.EnableReturnPass(TRUE);
	m_GridN.SetRowHeightAll(20);
	m_GridN.SetColumnWidthAll(45);
	m_GridN.SetColumnWidth(0,115);

	long nRow=0;

	// 공통
	SetDataInitGridNCommon(nRow);	
	// 슬래브앵커
	SetDataInitGridNSlabAngker(nRow);
	// 수직보강재
	SetDataInitGridNVStiff(nRow);
	// 수평보강재
	SetDataInitGridNHStiff(nRow);
	//볼트 연결시
	SetDataInitGridNBoltConnect(nRow);
	// 브라켓 보강판
	SetDataInitGridNBraketBogangpan(nRow);
	// 브라켓 상부플랜지
	SetDataInitGridNBraketFlangeUpper(nRow);
	// 브라켓 하부플랜지
	SetDataInitGridNBraketFlangeLower(nRow);
	// 전단이음판
	SetDataInitGridNShareConnectPan(nRow);
	// 모멘트이음판
	SetDataInitGridNMomentConnectPan(nRow);
	UpdateData(FALSE);
}


void CDetCrossBeamTypeDlg::SetDataInitGridNCommon(long &nRow)
{
	m_GridN.SetMergeRow(nRow, nRow+1, 0);
	long nCol = 0;
	m_GridN.SetTextMatrix(nRow, nCol++, "공통",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "LH",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "RH",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "P",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "H",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "T1",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "T2",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "T3",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "vWG",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "vW1G", (UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "vTG",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "X",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "vTL",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "vHL",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "vTR",	(UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, nCol++, "vHR",	(UINT)DT_CENTER );
	for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
	
	/////////////////////////////////////////////////////////////////////
	// SetDataInit_GridN
	++nRow;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	CPlateGirderApp* pGir = pDB->GetGirder(m_nG);
	CPlateBasicIndex* pBx = m_pCrossBeamCurrent->GetBx();
	
	if(!pBx) return;
	CPlateCrossBeam *pC = m_pCrossBeamCurrent;
	
	if(!pC) return;
	CString str;
	// 공통
	nCol = 1;
	BOOL	bHSteel	= FALSE;
	if(pC->GetType()==CR_TYPE_HSTEEL)	bHSteel = TRUE;
	str.Format("%.0f",pC->m_dLH1);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++, FALSE);//pC->m_bHunchTopConnect);
	str.Format("%.0f",pC->m_dRH1);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++, FALSE);//pC->m_bHunchTopConnect);
	str.Format("%.0f",pC->m_P_CR);	m_GridN.SetTextMatrix(nRow, nCol, str);	nCol++;
	str.Format("%.0f",pC->m_H_CR);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,bHSteel);
	str.Format("%.0f",pC->m_T1_CR);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,bHSteel);
	str.Format("%.0f",pC->m_T2_CR);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,bHSteel);
	str.Format("%.0f",pC->m_T3_CR);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,bHSteel);

	if(m_pCrossBeamCurrent->GetBx()->IsJijum())
	{
		CVStiff *pVStiff = pBx->GetJijumStiff();
		pC->m_vW_G = pVStiff ? pVStiff->m_dWidth[0] : 0;
		pC->m_vT_G = pVStiff ? pVStiff->m_dThick[0] : 0;
	}
	
	BOOL bReadOnly = m_pCrossBeamCurrent->GetBx()->IsJijum();		// 0032646
	str.Format("%g", pC->m_vW_G);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++, bReadOnly);
	str.Format("%g", pC->m_vW1_G);	m_GridN.SetTextMatrix(nRow, nCol, str); nCol++;
	str.Format("%g", pC->m_vT_G);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++, bReadOnly);
	str.Format("%g", pC->m_X_CR);	m_GridN.SetTextMatrix(nRow, nCol, str);	nCol++;

	// 수평브레이싱 연결판 
	CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
	if(pBrRu)	
	{
		str.Format("%g", pBrRu->m_dT);	m_GridN.SetTextMatrix(nRow, nCol++, str);//"dTL"
		str.Format("%g", pBrRu->m_ddH);	m_GridN.SetTextMatrix(nRow, nCol++, str);//"dTR"
	}
	else nCol+=2;
	CPlateBasicIndex* pBxR = pGir->GetBxMatchByCrossBeam(pBx,pGir->m_pRight);
	if(!pBxR)	return;
	CHBracingGusset* pBrRuR = pBxR->GetHBracingGusset();
	if(!pBrRuR)	return;
	if(pBrRuR)	
	{
		str.Format("%g", pBrRuR->m_dT);	m_GridN.SetTextMatrix(nRow, nCol++, str);//"dHL"
		str.Format("%g", pBrRuR->m_ddH);m_GridN.SetTextMatrix(nRow, nCol++, str);//"dHR"
	}
	++nRow;
}

void CDetCrossBeamTypeDlg::SetCellReadOnly(CGridCtrl& Grid, long nRow, long nCol, BOOL bReadOnly)
{
	if(bReadOnly)
	{
		Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
		Grid.SetItemState(nRow, nCol, GVIS_READONLY);
	}
	else
	{
		Grid.SetItemBkColour(nRow, nCol, RGBEDITABLE);
		Grid.SetItemState(nRow, nCol, GVIS_MODIFIED);
	}
}

void CDetCrossBeamTypeDlg::SetDataInitGridNSlabAngker(long &nRow)
{
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nAnchorsu_CR == 0) return;
	
	m_GridN.SetTextMatrix(nRow, 0, "슬래브앵커" );
	int n = 0;
	for(n = 0; n < m_pCrossBeamCurrent->m_nAnchorsu_CR; n++)
	{
		CString sTxt; sTxt.Format("A%ld", n+1);
		m_GridN.SetTextMatrix(nRow, n+1, sTxt ); 
	}
	for(n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
	m_GridN.SetMergeRow(nRow, nRow+1, 0);

	/////////////////////////////////////////////////////////////////////
	// SetDataInit_GridN
	++nRow;

	CString str;
	int nCol = 1;
	if(m_pCrossBeamCurrent->m_nAnchorsu_CR > 0)
	{
		for(long i = 0 ; i < m_pCrossBeamCurrent->m_nAnchorsu_CR; i++)
		{
			str.Format("%.0f",m_pCrossBeamCurrent->m_AnchorDis_CR[i]);
			m_GridN.SetTextMatrix(nRow, nCol, str);
			SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
		}
	}

	++nRow;
}

void CDetCrossBeamTypeDlg::SetDataInitGridNVStiff(long &nRow)
{	
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nVStiffsu_CR == 0) return;	
	m_GridN.SetMergeRow(nRow, nRow+1, 0);
	m_GridN.SetTextMatrix(nRow, 0, "수직보강재" );
	m_GridN.SetTextMatrix(nRow, 1, "vW", (UINT)DT_CENTER);
	m_GridN.SetTextMatrix(nRow, 2, "vT", (UINT)DT_CENTER );
	m_GridN.SetTextMatrix(nRow, 3, "vL", (UINT)DT_CENTER );
	int n = 0;
	for(n = 3; n < m_pCrossBeamCurrent->m_nVStiffsu_CR+3; n++)
	{
		CString sTxt; sTxt.Format("V%ld", n-2);
		m_GridN.SetTextMatrix(nRow, n+1, sTxt );
	}
	for(n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);	
	
	/////////////////////////////////////////////////////////////////////
	// SetDataInit_GridN
	++nRow;
	CString str;
	int nCol = 1;
	if(m_pCrossBeamCurrent->m_nVStiffsu_CR > 0)
	{
		str.Format("%.0f",m_pCrossBeamCurrent->m_vW_CR);		m_GridN.SetTextMatrix(nRow, nCol,str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
		str.Format("%.0f",m_pCrossBeamCurrent->m_vT_CR);		m_GridN.SetTextMatrix(nRow, nCol,str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
		str.Format("%.0f",m_pCrossBeamCurrent->m_vL_CR);		m_GridN.SetTextMatrix(nRow, nCol++,str);
		for(long i = 0 ; i < m_pCrossBeamCurrent->m_nVStiffsu_CR; i++)
		{
			str.Format("%.0f",m_pCrossBeamCurrent->m_VStiffDis_CR[i]);
			m_GridN.SetTextMatrix(nRow, nCol++,str);		
		}
	}
	++nRow;
}

void CDetCrossBeamTypeDlg::SetDataInitGridNHStiff(long &nRow)
{	
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nHStiffUppersu_CR == 0 && m_pCrossBeamCurrent->m_nHStiffLowersu_CR == 0)
		return;
	m_GridN.SetMergeRow(nRow, nRow+1, 0);
	m_GridN.SetTextMatrix(nRow, 0, "수평보강재" );
	m_GridN.SetTextMatrix(nRow, 1, "hW", (UINT)DT_CENTER);
	m_GridN.SetTextMatrix(nRow, 2, "hT", (UINT)DT_CENTER );
	for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
	m_GridN.SetRowHeight(nRow, 20);
	m_GridN.SetRowHeight(nRow+1, 20);

	/////////////////////////////////////////////////////////////////////
	// SetDataInit_GridN
	++nRow;
	CString str;
	int nCol = 1;
	str.Format("%.0f",m_pCrossBeamCurrent->m_hW_CR);
	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
	str.Format("%.0f",m_pCrossBeamCurrent->m_hT_CR);
	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
	++nRow;
}

void CDetCrossBeamTypeDlg::SetDataInitGridNBoltConnect(long &nRow)
{
	if(!m_pCrossBeamCurrent) return;
//	if(m_pCrossBeamCurrent->GetType() != CR_TYPE_HSTEEL) // 볼트연결타입 (H형강타입)
//		return;
	if(m_pCrossBeamCurrent->m_nLGirSpType && m_pCrossBeamCurrent->m_nRGirSpType)
		return;
	m_GridN.SetMergeRow(nRow, nRow+1, 0);
	m_GridN.SetTextMatrix(nRow, 0, "볼트 연결시" );
	m_GridN.SetTextMatrix(nRow, 1, "bL1" );
	m_GridN.SetTextMatrix(nRow, 2, "bL2" );
	m_GridN.SetTextMatrix(nRow, 3, "bL3" );
	m_GridN.SetTextMatrix(nRow, 4, "bL4" );
	m_GridN.SetTextMatrix(nRow, 5, "bR1" );
	m_GridN.SetTextMatrix(nRow, 6, "bR2" );
	m_GridN.SetTextMatrix(nRow, 7, "bR3" );
	m_GridN.SetTextMatrix(nRow, 8, "bR4" );
	m_GridN.SetTextMatrix(nRow, 9, "wB" );
	m_GridN.SetTextMatrix(nRow, 10, "wR" );
	m_GridN.SetTextMatrix(nRow, 11, "wH" );
	m_GridN.SetTextMatrix(nRow, 12, "wE" );
	m_GridN.SetTextMatrix(nRow, 13, "wF" );
	for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
	
	/////////////////////////////////////////////////////////////////////
	// SetDataInit_GridN
	++nRow;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	CPlateGirderApp* pGir = pDB->GetGirder(m_nG);
	CPlateBasicIndex* pBx = pGir->GetBxCrossBeamOnly(m_nPos);
	if(!pBx) return;
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(!pC) return;
	CString str;
	int nCol = 1;
	str.Format("%.0f",pC->m_dbL1);	m_GridN.SetTextMatrix(nRow, nCol,str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);//14
	str.Format("%.0f",pC->m_dbL2);	m_GridN.SetTextMatrix(nRow, nCol,str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
	str.Format("%.0f",pC->m_dbL3);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dbL4);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dbR1);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dbR2);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dbR3);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dbR4);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dwB);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dwR);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",pC->m_dwH);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%d",  pC->m_wE);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%d",  pC->m_wF);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	++nRow;
}

void CDetCrossBeamTypeDlg::SetDataInitGridNBraketBogangpan(long &nRow)
{
	if(!m_pCrossBeamCurrent) return;
//	if(m_pCrossBeamCurrent->GetType() == CR_TYPE_VSTIFF) // 볼트연결타입 (H형강타입)
//		return;
	if(!m_pCrossBeamCurrent->m_cGasset_CR) // 브라켓 보강판
		return;
	m_GridN.SetMergeRow(nRow, nRow+1, 0);
	m_GridN.SetTextMatrix(nRow, 0, "브라켓 보강판" );
	m_GridN.SetTextMatrix(nRow, 1, "tlH" );
	m_GridN.SetTextMatrix(nRow, 2, "tlE" );
	m_GridN.SetTextMatrix(nRow, 3, "tlF" );
	m_GridN.SetTextMatrix(nRow, 4, "trH" );
	m_GridN.SetTextMatrix(nRow, 5, "trE" );
	m_GridN.SetTextMatrix(nRow, 6, "trF" );
	for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
		
	/////////////////////////////////////////////////////////////////////
	// SetDataInit_GridN
	++nRow;
	CString str;
	int nCol = 1;
	str.Format("%.0f",m_pCrossBeamCurrent->m_dltH);	m_GridN.SetTextMatrix(nRow, nCol,str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
	str.Format("%d",  m_pCrossBeamCurrent->m_ltE);	m_GridN.SetTextMatrix(nRow, nCol,str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
	str.Format("%d",  m_pCrossBeamCurrent->m_ltF);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%.0f",m_pCrossBeamCurrent->m_drtH);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%d",  m_pCrossBeamCurrent->m_rtE);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	str.Format("%d",  m_pCrossBeamCurrent->m_rtF);	m_GridN.SetTextMatrix(nRow, nCol++,str);
	++nRow;
}

void CDetCrossBeamTypeDlg::SetDataInitGridNBraketFlangeUpper(long &nRow)
{
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nLGirSpType || m_pCrossBeamCurrent->m_nRGirSpType)
	{
		m_GridN.SetMergeRow(nRow, nRow+1, 0);
		m_GridN.SetTextMatrix(nRow, 0, "브라켓 상부플랜지" );
		m_GridN.SetTextMatrix(nRow, 1, "uM" );
		m_GridN.SetTextMatrix(nRow, 2, "uN" );
		m_GridN.SetTextMatrix(nRow, 3, "uA" );
		m_GridN.SetTextMatrix(nRow, 4, "uB" );
		m_GridN.SetTextMatrix(nRow, 5, "uC" );
		m_GridN.SetTextMatrix(nRow, 6, "uC1" );
		m_GridN.SetTextMatrix(nRow, 7, "uD" );
		m_GridN.SetTextMatrix(nRow, 8, "uE" );
		m_GridN.SetTextMatrix(nRow, 9, "uF" );
		m_GridN.SetTextMatrix(nRow, 10, "uW" );
		m_GridN.SetTextMatrix(nRow, 11, "uT" );
		m_GridN.SetTextMatrix(nRow, 12, "uL" );
		m_GridN.SetTextMatrix(nRow, 13, "uM1" );
		m_GridN.SetTextMatrix(nRow, 14, "uD1" );
		m_GridN.SetTextMatrix(nRow, 15, "uE1" );
		m_GridN.SetTextMatrix(nRow, 16, "uF1" );
		m_GridN.SetTextMatrix(nRow, 17, "uB1" );
		for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
		
		/////////////////////////////////////////////////////////////////////
		// SetDataInit_GridN
		++nRow;
		CDataManage *pData = m_pStd->GetDataManage();
		BOOL bBoltLengthAuto = pData->GetGlobalOption()->GetBoltLengthAuto();
		CSteelBolt *pBolt = m_pStd->GetDataManage()->GetSteelBolt();
		BOOL	bHSteel	= FALSE;
		if(m_pCrossBeamCurrent->GetType()==CR_TYPE_HSTEEL)
			bHSteel = TRUE;
		CString str;
		int nCol = 1;
		str.Format("%.0f",m_pCrossBeamCurrent->m_uM_CR);	m_GridN.SetTextMatrix(nRow, nCol,str);		SetCellReadOnly(m_GridN, nRow, nCol++,bHSteel);
		str.Format("%.0f",m_pCrossBeamCurrent->m_uN_CR);	m_GridN.SetTextMatrix(nRow, nCol,str);		SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
 		str.Format("%g", m_pCrossBeamCurrent->m_uA);		m_GridN.SetTextMatrix(nRow, nCol++, str);//10
		str.Format("%d", m_pCrossBeamCurrent->m_uB);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uC);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uC1);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uD);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%d", m_pCrossBeamCurrent->m_uE);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uF);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uW);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uT);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		if(bBoltLengthAuto && m_pCrossBeamCurrent->GetType() != CR_TYPE_HSTEEL)//H형강 타입이 아니면
		{
			double T2 = m_pCrossBeamCurrent->m_uT*2 + m_pCrossBeamCurrent->m_T2_CR;
			str.Format("%g", pBolt->GetBoltLength(m_pCrossBeamCurrent->GetBx(), CG_BOLT, m_pCrossBeamCurrent->m_ZzsBoltSize,T2));
			m_GridN.SetTextMatrix(nRow, nCol++, str);
		}
		else
		{
			str.Format("%g", m_pCrossBeamCurrent->m_uL);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		}		
			//커넥션 플레이트
		str.Format("%g", m_pCrossBeamCurrent->m_uM1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uD1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uE1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uF1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_uB1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		++nRow;
	}
}

void CDetCrossBeamTypeDlg::SetDataInitGridNBraketFlangeLower(long &nRow)
{
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nLGirSpType || m_pCrossBeamCurrent->m_nRGirSpType)
	{
		m_GridN.SetMergeRow(nRow, nRow+1, 0);
		m_GridN.SetTextMatrix(nRow, 0, "브라켓 하부플랜지" );
		m_GridN.SetTextMatrix(nRow, 1, "dM" );
		m_GridN.SetTextMatrix(nRow, 2, "dN" );
		m_GridN.SetTextMatrix(nRow, 3, "dA" );
		m_GridN.SetTextMatrix(nRow, 4, "dB" );
		m_GridN.SetTextMatrix(nRow, 5, "dC" );
		m_GridN.SetTextMatrix(nRow, 6, "dC1" );
		m_GridN.SetTextMatrix(nRow, 7, "dD" );
		m_GridN.SetTextMatrix(nRow, 8, "dE" );
		m_GridN.SetTextMatrix(nRow, 9, "dF" );
		m_GridN.SetTextMatrix(nRow, 10, "dW" );
		m_GridN.SetTextMatrix(nRow, 11, "dT" );
		m_GridN.SetTextMatrix(nRow, 12, "dL" );
		m_GridN.SetTextMatrix(nRow, 13, "dM1" );
		m_GridN.SetTextMatrix(nRow, 14, "dD1" );
		m_GridN.SetTextMatrix(nRow, 15, "dE1" );
		m_GridN.SetTextMatrix(nRow, 16, "dF1" );
		m_GridN.SetTextMatrix(nRow, 17, "dB1" );
		for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
		
		/////////////////////////////////////////////////////////////////////
		// SetDataInit_GridN
		++nRow;
		CDataManage *pData		= m_pStd->GetDataManage();		
		CSteelBolt	*pBolt		= pData->GetSteelBolt();
		BOOL bBoltLengthAuto	= pData->GetGlobalOption()->GetBoltLengthAuto();
		BOOL	bHSteel	= FALSE;
		if(m_pCrossBeamCurrent->GetType()==CR_TYPE_HSTEEL)
			bHSteel = TRUE;
		CString str;
		int nCol = 1;
		str.Format("%.0f",m_pCrossBeamCurrent->m_dM_CR);	m_GridN.SetTextMatrix(nRow, nCol,str);		SetCellReadOnly(m_GridN, nRow, nCol++,bHSteel);
		str.Format("%.0f",m_pCrossBeamCurrent->m_dN_CR);	m_GridN.SetTextMatrix(nRow, nCol,str);		SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
 		str.Format("%g", m_pCrossBeamCurrent->m_dA);		m_GridN.SetTextMatrix(nRow, nCol++, str);//10
		str.Format("%d", m_pCrossBeamCurrent->m_dB);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dC);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dC1);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dD);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%d", m_pCrossBeamCurrent->m_dE);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dF);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dW);		m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dT);		m_GridN.SetTextMatrix(nRow, nCol++, str);

		if(bBoltLengthAuto && m_pCrossBeamCurrent->GetType() != CR_TYPE_HSTEEL)//H형강 타입이 아니면
		{
			double T1 = m_pCrossBeamCurrent->m_dT*2 + m_pCrossBeamCurrent->m_T1_CR;
			str.Format("%g", pBolt->GetBoltLength(m_pCrossBeamCurrent->GetBx(), CG_BOLT, m_pCrossBeamCurrent->m_ZzsBoltSize,T1));
			m_GridN.SetTextMatrix(nRow, nCol++, str);
		}
		else
		{
			str.Format("%g", m_pCrossBeamCurrent->m_dL);			m_GridN.SetTextMatrix(nRow, nCol++, str);
		}
			//커넥션 플레이트
		str.Format("%g", m_pCrossBeamCurrent->m_dM1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dD1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dE1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dF1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_dB1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		++nRow;
	}
}


void CDetCrossBeamTypeDlg::SetDataInitGridNShareConnectPan(long &nRow)
{
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nLGirSpType || m_pCrossBeamCurrent->m_nRGirSpType)
	{
		m_GridN.SetMergeRow(nRow, nRow+1, 0);
		m_GridN.SetTextMatrix(nRow, 0, "전단이음판" );
		m_GridN.SetTextMatrix(nRow, 1, "sA" );
		m_GridN.SetTextMatrix(nRow, 2, "sB" );
		m_GridN.SetTextMatrix(nRow, 3, "sC" );
		m_GridN.SetTextMatrix(nRow, 4, "sC1" );
		m_GridN.SetTextMatrix(nRow, 5, "sE" );
		m_GridN.SetTextMatrix(nRow, 6, "sF" );
		m_GridN.SetTextMatrix(nRow, 7, "sW" );
		m_GridN.SetTextMatrix(nRow, 8, "sT" );
		m_GridN.SetTextMatrix(nRow, 9, "sL" );
		m_GridN.SetTextMatrix(nRow, 10, "sL1" );
		for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
		
		/////////////////////////////////////////////////////////////////////
		// SetDataInit_GridN
		++nRow;
		CDataManage		*pData	= m_pStd->GetDataManage();
		CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
		CSteelBolt		*pBolt	= m_pStd->GetDataManage()->GetSteelBolt();

		BOOL bBoltLengthAuto = pData->GetGlobalOption()->GetBoltLengthAuto();

		CString str;
		int nCol = 1;
		str.Format("%g", m_pCrossBeamCurrent->m_sA);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);//9
		str.Format("%d", m_pCrossBeamCurrent->m_sB);	m_GridN.SetTextMatrix(nRow, nCol, str);	SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
		str.Format("%g", m_pCrossBeamCurrent->m_sC);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_sC1);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%d", m_pCrossBeamCurrent->m_sE);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_sF);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_sW);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		str.Format("%g", m_pCrossBeamCurrent->m_sT);	m_GridN.SetTextMatrix(nRow, nCol++, str);
		if(bBoltLengthAuto)
		{
			if(m_pCrossBeamCurrent->GetType() == CR_TYPE_HSTEEL)//H형강 타입이면...
			{
				double T3 = m_pCrossBeamCurrent->m_T3_CR + m_pCrossBeamCurrent->m_vT_G;
				str.Format("%g", pBolt->GetBoltLength(m_pCrossBeamCurrent->GetBx(), CG_BOLT, m_pCrossBeamCurrent->m_ZzsBoltSize,T3));
				m_GridN.SetTextMatrix(nRow, nCol++, str);				
			}
			else
			{
				if(m_pCrossBeamCurrent->GetType() == CR_TYPE_MANUFACTURE && m_pCrossBeamCurrent->m_nLGirSpType == 0)//제작타입이고 좌/우측 이음판이 없을경우
				{					
					double T3 = m_pCrossBeamCurrent->m_T3_CR + m_pCrossBeamCurrent->m_vT_G;
					str.Format("%g", pBolt->GetBoltLength(m_pCrossBeamCurrent->GetBx(), CG_BOLT, m_pCrossBeamCurrent->m_ZzsBoltSize,T3));
					m_GridN.SetTextMatrix(nRow, nCol++, str);					
				}
				else
				{
					double T3 = m_pCrossBeamCurrent->m_sT*2 + m_pCrossBeamCurrent->m_T3_CR;
					str.Format("%g", pBolt->GetBoltLength(m_pCrossBeamCurrent->GetBx(), CG_BOLT, m_pCrossBeamCurrent->m_ZzsBoltSize,T3));
					m_GridN.SetTextMatrix(nRow, nCol++, str);					
				}	
			}
			
			if(m_pCrossBeamCurrent->GetType() == CR_TYPE_VSTIFF && !pDB->IsPlateGir())//소수주형orTU이고 수직보강재 연결일경우만 채움판...
			{
				CPlateBasicIndex* pBx = m_pCrossBeamCurrent->GetBx();
				str.Format("%g", m_pCrossBeamCurrent->GetFillerBoltLength(pBx, TRUE));			
				m_GridN.SetTextMatrix(nRow, nCol++, str);//채움판...
			}
			else
			{
				str.Format("%g", m_pCrossBeamCurrent->m_sL1);			
				m_GridN.SetTextMatrix(nRow, nCol++, str);//채움판...
			}
		}
		else
		{
			str.Format("%g", m_pCrossBeamCurrent->m_sL);	m_GridN.SetTextMatrix(nRow, nCol++, str);
			str.Format("%g", m_pCrossBeamCurrent->m_sL1);	m_GridN.SetTextMatrix(nRow, nCol++, str);//채움판...
		}	
		++nRow;
	}
}

void CDetCrossBeamTypeDlg::SetDataInitGridNMomentConnectPan(long &nRow)
{
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->GetType() != CR_TYPE_MANUFACTURE
		&& m_pCrossBeamCurrent->GetType() != CR_TYPE_VSTIFF) // 이음판 타입여부
		return;
	if(!m_pCrossBeamCurrent->m_bMomentPan) // 모멘트이음판
		return;
	m_GridN.SetMergeRow(nRow, nRow+1, 0);
	m_GridN.SetTextMatrix(nRow, 0, "모멘트이음판" );
	m_GridN.SetTextMatrix(nRow, 1, "mA" );
	m_GridN.SetTextMatrix(nRow, 2, "mB" );
	m_GridN.SetTextMatrix(nRow, 3, "mC" );
	m_GridN.SetTextMatrix(nRow, 4, "mC1" );
	m_GridN.SetTextMatrix(nRow, 5, "mE" );
	m_GridN.SetTextMatrix(nRow, 6, "mF" );
	m_GridN.SetTextMatrix(nRow, 7, "mW" );
	m_GridN.SetTextMatrix(nRow, 8, "mT" );
	m_GridN.SetTextMatrix(nRow, 9, "mL" );
	m_GridN.SetTextMatrix(nRow, 10, "mD" );
	m_GridN.SetTextMatrix(nRow, 11, "mG" );
	for(int n = 0; n < GetColsSize(); n++)	m_GridN.SetCellFixType(nRow, n, n==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
	
	/////////////////////////////////////////////////////////////////////
	// SetDataInit_GridN
	++nRow;
	CDataManage	*pData = m_pStd->GetDataManage();	
	CSteelBolt	*pBolt = pData->GetSteelBolt();
	BOOL bBoltLengthAuto = pData->GetGlobalOption()->GetBoltLengthAuto();
	CString str;
	int nCol = 1;
	str.Format("%g", m_pCrossBeamCurrent->m_mA);			m_GridN.SetTextMatrix(nRow, nCol, str);		SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
	str.Format("%d", m_pCrossBeamCurrent->m_mB);			m_GridN.SetTextMatrix(nRow, nCol, str);		SetCellReadOnly(m_GridN, nRow, nCol++,FALSE);
	str.Format("%g", m_pCrossBeamCurrent->m_mC);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	str.Format("%g", m_pCrossBeamCurrent->m_mC1);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	str.Format("%d", m_pCrossBeamCurrent->m_mE);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	str.Format("%g", m_pCrossBeamCurrent->m_mF);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	str.Format("%g", m_pCrossBeamCurrent->m_mW);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	str.Format("%g", m_pCrossBeamCurrent->m_mT);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	if(bBoltLengthAuto && m_pCrossBeamCurrent->GetType() != CR_TYPE_HSTEEL)//H형강 타입이 아니면
	{
		if(m_pCrossBeamCurrent->m_mT <= 0)
		{
			m_pCrossBeamCurrent->m_mL = 0;
			str.Format("%g", m_pCrossBeamCurrent->m_mL);
			m_GridN.SetTextMatrix(nRow, nCol++, str);	
		}
		else
		{
			double T4 = m_pCrossBeamCurrent->m_mT*2 + m_pCrossBeamCurrent->m_T3_CR;
			str.Format("%g", pBolt->GetBoltLength(m_pCrossBeamCurrent->GetBx(), CG_BOLT, m_pCrossBeamCurrent->m_ZzsBoltSize,T4));
			m_GridN.SetTextMatrix(nRow, nCol++, str);
		}			
	}
	else
	{
		str.Format("%g", m_pCrossBeamCurrent->m_mL);		m_GridN.SetTextMatrix(nRow, nCol++, str);
	}

	str.Format("%g", m_pCrossBeamCurrent->m_mD);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	str.Format("%g", m_pCrossBeamCurrent->m_mG);			m_GridN.SetTextMatrix(nRow, nCol++, str);
	++nRow;
}

//SetDataSave
void CDetCrossBeamTypeDlg::SetDataSaveGridN()
{
	SetDataSaveGridNCommon();

	// 슬래브앵커
	SetDataSaveGridNSlabAngker();
	// 수직보강재
	SetDataSaveGridNVStiff();
	// 수평보강재
	SetDataSaveGridNHStiff();
	//볼트 연결시
	SetDataSaveGridNBoltConnect();
	// 브라켓 보강판
	SetDataSaveGridNBraketBogangpan();
	// 브라켓 상부플랜지
	SetDataSaveGridNBraketFlangeUpper();
	// 브라켓 하부플랜지
	SetDataSaveGridNBraketFlangeLower();
	// 전단이음판
	SetDataSaveGridNShareConnectPan();
	// 모멘트이음판
	SetDataSaveGridNMomentConnectPan();

	if(!m_pCrossBeamCurrent) return;

	CPlateBasicIndex *pBx	= m_pCrossBeamCurrent->GetBx();
	CPlateBasicIndex *pBxR	= (CPlateBasicIndex*)pBx->GetBxMatchByCrossBeam(pBx->GetGirder()->m_pRight);
	if(m_nCurType==CR_TYPE_VSTIFF)	// 수직보강재 부착타입 가로보
	{
		CPlateGirderApp *pGir = pBx->GetGirder();
		CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(m_pCrossBeamCurrent->GetBx()->GetStation());
		double dCross	= fabs(m_pCrossBeamCurrent->GetVectorPlan().RotateInv(vGir).y);
		if(m_pCrossBeamCurrent->IsSkew())	m_pCrossBeamCurrent->m_P_CR = m_pCrossBeamCurrent->m_vW_G*dCross;
		else								m_pCrossBeamCurrent->m_P_CR = m_pCrossBeamCurrent->m_vW_G;

		if(m_pCrossBeamCurrent->m_dLH1==0)
			m_pCrossBeamCurrent->m_dLH1 = 1;
		if(m_pCrossBeamCurrent->m_dRH1==0)
			m_pCrossBeamCurrent->m_dRH1 = 1;
	}
	m_pCrossBeamCurrent->SetType(m_nCurType);

	SetDataSaveSteelMaterial();
}

void CDetCrossBeamTypeDlg::SetDataSaveGridNCommon()
{
	int nRow = GetFixRowAtGridN("공통");
	if(nRow < 0)//존재하지 않는 경우
		return;
	
	++nRow;
	CString str;
	// 공통
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_dLH1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);//11
	m_pCrossBeamCurrent->m_dRH1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_P_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_H_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_T1_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_T2_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_T3_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_vW_G = m_GridN.GetTextMatrixDouble(nRow, nCol++);// 거더 연결 수직보강재 
	m_pCrossBeamCurrent->m_vW1_G = m_GridN.GetTextMatrixDouble(nRow, nCol++);// 거더 연결 수직보강재 
	m_pCrossBeamCurrent->m_vT_G = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_X_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	
	// 수평브레이싱 연결판 
	CPlateBasicIndex* pBx = m_pCrossBeamCurrent->GetBx();
	CPlateGirderApp* pGir = pBx->GetGirder();
	CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
	if(pBrRu)	
	{
		pBrRu->m_dT		= m_GridN.GetTextMatrixDouble(nRow, nCol++);
		pBrRu->m_ddH	= m_GridN.GetTextMatrixDouble(nRow, nCol++);
		//	pBrRu->m_dh2L = m_pCrossBeamCurrent->m_P_CR;
		//	pBrRu->m_dh2R = m_pCrossBeamCurrent->m_P_CR;
	}
	else nCol+=2;
	CPlateBasicIndex* pBxR = pGir->GetBxMatchByCrossBeam(pBx,pGir->m_pRight);
	if(!pBxR)	return;
	CHBracingGusset* pBrRuR = pBxR->GetHBracingGusset();
	if(!pBrRuR)	return;
	if(pBrRuR)	
	{
		pBrRuR->m_dT	= m_GridN.GetTextMatrixDouble(nRow, nCol++);
		pBrRuR->m_ddH	= m_GridN.GetTextMatrixDouble(nRow, nCol++);
		//	pBrRuR->m_dh1L = m_pCrossBeamCurrent->m_P_CR;
		//	pBrRuR->m_dh1R = m_pCrossBeamCurrent->m_P_CR;
	}


}

void CDetCrossBeamTypeDlg::SetDataSaveGridNSlabAngker()
{
	CString sTitle = "슬래브앵커";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;

	++nRow;
	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nAnchorsu_CR > 0)
	{
		for(long i = 0 ; i < m_pCrossBeamCurrent->m_nAnchorsu_CR; i++)
		{
			m_pCrossBeamCurrent->m_AnchorDis_CR[i] = m_GridN.GetTextMatrixDouble(nRow, nCol++);
		}
	}
}

void CDetCrossBeamTypeDlg::SetDataSaveGridNVStiff()
{
	CString sTitle = "수직보강재";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;

	++nRow;
	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	if(m_pCrossBeamCurrent->m_nVStiffsu_CR > 0)
	{
		m_pCrossBeamCurrent->m_vW_CR	= m_GridN.GetTextMatrixDouble(nRow, nCol++);
		m_pCrossBeamCurrent->m_vT_CR	= m_GridN.GetTextMatrixDouble(nRow, nCol++);
		m_pCrossBeamCurrent->m_vL_CR	= m_GridN.GetTextMatrixDouble(nRow, nCol++);
		for(long i = 0 ; i < m_pCrossBeamCurrent->m_nVStiffsu_CR; i++)
		{
			m_pCrossBeamCurrent->m_VStiffDis_CR[i] = m_GridN.GetTextMatrixDouble(nRow, nCol++);
		}
	}
}

void CDetCrossBeamTypeDlg::SetDataSaveGridNHStiff()
{
	CString sTitle = "수평보강재";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;

	++nRow;
	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_hW_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_hT_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
}

void CDetCrossBeamTypeDlg::SetDataSaveGridNBoltConnect()
{
	CString sTitle = "볼트 연결시";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;
		
	++nRow;
	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_dbL1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dbL2 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dbL3 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dbL4 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dbR1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dbR2 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dbR3 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dbR4 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dwB = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dwR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dwH = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_wE = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_wF = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
}

void CDetCrossBeamTypeDlg::SetDataSaveGridNBraketBogangpan()
{
	CString sTitle = "브라켓 보강판";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;
	++nRow;
	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_dltH = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_ltE = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_ltF = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_drtH = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_rtE = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_rtF = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
}

void CDetCrossBeamTypeDlg::SetDataSaveGridNBraketFlangeUpper()
{
	CString sTitle = "브라켓 상부플랜지";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;

	++nRow;
	
	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_uM_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uN_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uA = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uB = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uC = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uC1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uD = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uE = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uF = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uW = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uT = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uL = m_GridN.GetTextMatrixDouble(nRow, nCol++);

	//커넥션 플레이트
	m_pCrossBeamCurrent->m_uM1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uD1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uE1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uF1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_uB1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
}

void CDetCrossBeamTypeDlg::SetDataSaveGridNBraketFlangeLower()
{
	CString sTitle = "브라켓 하부플랜지";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;

	++nRow;

	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_dM_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dN_CR = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dA = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dB = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dC = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dC1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dD = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dE = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dF = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dW = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dT = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dL = m_GridN.GetTextMatrixDouble(nRow, nCol++);

	//커넥션 플레이트
	m_pCrossBeamCurrent->m_dM1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dD1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dE1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dF1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_dB1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
}


void CDetCrossBeamTypeDlg::SetDataSaveGridNShareConnectPan()
{
	CString sTitle = "전단이음판";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;

	++nRow;

	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_sA = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_sB = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_sC = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_sC1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_sE = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_sF = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_sW = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_sT = m_GridN.GetTextMatrixDouble(nRow, nCol++);
}


void CDetCrossBeamTypeDlg::SetDataSaveGridNMomentConnectPan()
{
	CString sTitle = "모멘트이음판";
	int nRow = GetFixRowAtGridN(sTitle);
	if(nRow < 0)//존재하지 않는 경우
		return;

	++nRow;
	
	CString str;
	int nCol = 1;
	if(!m_pCrossBeamCurrent) return;
	m_pCrossBeamCurrent->m_mA = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mB = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mC = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mC1 = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mE = (long)m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mF = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mW = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mT = m_GridN.GetTextMatrixDouble(nRow, nCol++);

	m_pCrossBeamCurrent->m_mD = m_GridN.GetTextMatrixDouble(nRow, nCol++);
	m_pCrossBeamCurrent->m_mG = m_GridN.GetTextMatrixDouble(nRow, nCol++);
}



void CDetCrossBeamTypeDlg::SetDataInit()
{
	CPlateBridgeApp  *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp  *pGir    = pBridge->GetGirder(m_nG);	
	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(m_nPos);
	if(!pBx) return;
	CSection         *pSec    = pBx->GetSection();

//	GetDlgItem(IDC_RIST_RECOMMEND)->ShowWindow(pBridge->IsPlateGir() ? SW_HIDE : SW_SHOW);	
	GetDlgItem(IDC_RIST_RECOMMEND)->ShowWindow(SW_HIDE);	// #0031823 Rist 와 계약종료로 해당 기능 감춤

	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(!pC) return;
	m_nCurType = pC->GetType();
	m_bApplyCalc.SetCheck(pC->IsSectionCheck());
	SetBoltCombo(m_nG, m_nPos);	
	SetDataInitGridN();

	SetGrid2DataInit();
	// 형강제원 콤보박스
	SetSteelTypeCombo();
	SetSteelMaterialCombo();
	UpdateData(FALSE);
	// 가로보의 형상 변경에 따라 거더부착 수직보강재의 제원이 변경적용되어야 함.
	SetDataVStiffJewon(pBx);
	UpdateData(FALSE);

	SetPIButtonText();
}

void CDetCrossBeamTypeDlg::SetPIButtonText()
{	
	CPlateCrossBeam* pC = m_pCrossBeamCurrent;
	if(pC == NULL)	return;
	
	if( pC->GetBx()->IsJijum())
		GetDlgItem(IDC_BUTTON_JIJUM)->SetWindowText("지점부 동일적용");
	else GetDlgItem(IDC_BUTTON_JIJUM)->SetWindowText("일반부 동일적용");
}

void CDetCrossBeamTypeDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long	nG		= 0;
	long	cPos	= 0;
	long	nPos	= 0;
	long	nType	= pDB->GetBridgeType();
	for(nG = 0 ; nG < pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir= pDB->GetGirder(nG);
		cPos = pGir->GetQtyCrossBeamOnly();
		for(nPos = 0 ; nPos < cPos ; nPos++) 
		{
			CPlateBasicIndex	*pBx	= pGir->GetBxCrossBeamOnly(nPos);
			CSection			*pSec	= pBx->GetSection();
			CPlateCrossBeam		*pC		= pSec->GetCrossBeam();
			if(!IsUsedFlag())
			{
				if(nType==0)	pC->SetType(CR_TYPE_MANUFACTURE);
				else			pC->SetType(CR_TYPE_VSTIFF);
			}
			SetDataDefaultSub(nG, nPos);
		}
	}
	SetCrossBeamCurrent();
	UpdateData(FALSE);
}
void CDetCrossBeamTypeDlg::SetDataDefaultSub(long nG, long nPos)
{
	CPlateBridgeApp  *pDB	= m_pStd->GetBridge();
	CPlateGirderApp  *pGir  = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx   = pGir->GetBxCrossBeamOnly( nPos);
	CSection		 *pSec	= pBx->GetSection();
 	CPlateCrossBeam	 *pC	= pSec->GetCrossBeam();	
	CSteelBolt		 *pBolt = m_pStd->GetDataManage()->GetSteelBolt();
	
	//소수주형이고 지점부 가로보일경우 초기값및 권고안값 
	//1. 제작타입  2. 높이 70% 3. LH,RH -> 0  4. 컨넥션 플레이트는 미적용..
	//일반부 가로보 인경우..
	//1. 가로보 위치는 중앙으로 LH, RH 높이 조정(10라운드 처리)
	//2. 컨넥션 플레이트는 권고안 한번 누른 상태로...
	long	nBridgeType	= pDB->GetBridgeType();
	BOOL	bTuGir		= pDB->IsTUGir();

	if (nBridgeType != 0)
	{
		if(pBx->IsJijum() && !bTuGir)	pC->SetType(CR_TYPE_MANUFACTURE);//1. 제작타입으로..
		else							pC->SetType(CR_TYPE_VSTIFF);
	}

	SetDataDefaultSubCrossBeam(nG, nPos);
	SetDataVStiffJewon(pBx);

	CString str;
	str.Format("%s %s", pDB->GetMaterial(CG_BOLT, pC->GetBx()), pC->m_ZzsBoltSize);
	long nIdx		= pBolt->GetIndex(str);
	double dEdge	= pBolt->GetCalcEdgeDistance(nIdx);

	if(nBridgeType == 1)
	{
		OnRistRecommendOnRatio(FALSE, 0.7, 0.35, 0.7, pC);
		if(pBx->IsJijum())
		{
			pC->m_uM_CR	= 200;		pC->m_dM_CR	= 200;
			pC->m_uD	= 40;		pC->m_dD	= 40;
			pC->m_uE	= 1;		pC->m_dE	= 1;
			pC->m_uF	= 80;		pC->m_dF	= 80;
			pC->m_uW	= 160;		pC->m_dW	= 160;
			pC->m_uM1	= 200;		pC->m_dM1	= 200;
		}
		else
		{
			pC->m_P_CR	= 250;
			pC->m_vW1_G	= 0;		pC->m_uW	= 240;
			pC->m_uM1	= 280;		pC->m_uN_CR	= 100;
			pC->m_uE1	= 2;		pC->m_uF1	= 80;
			pC->m_uD1	= 40;		pC->m_uA	= dEdge;
			pC->m_uB1	= 1;		pC->m_uC	= 80;
			pC->m_uC1	= 100;		pC->m_uB	= 2;
			pC->m_uM_CR	= 200;		pC->m_uE	= 1;
			pC->m_uF	= 80;		pC->m_uD	= 40;

			pC->m_dW	= 240;		pC->m_dM1	= 280;
			pC->m_dN_CR	= 100;		pC->m_dE1	= 2;
			pC->m_dF1	= 80;		pC->m_dD1	= 40;
			pC->m_dA	= dEdge;		pC->m_dB1	= 1;
			pC->m_dC	= 80;		pC->m_dC1	= 100;
			pC->m_dB	= 2;		pC->m_dM_CR	= 200;
			pC->m_dE	= 1;		pC->m_dF	= 80;
			pC->m_dD	= 40;
		}
	}
	pC->m_vL_CR = pC->m_H_CR - 50;
}

void CDetCrossBeamTypeDlg::SetDataDefaultSubCrossBeam(long nG, long nPos)
{
	CSteelMaterial   *pMat    = m_pStd->GetDataManage()->GetSteelMaterial();
	CPlateBridgeApp  *pDB	= m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);
	CPlateGirderApp  *pGirN   = pGir->m_pRight;
	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(nPos);
	if(pBx==NULL)	return;
	CSteelBolt		 *pBolt = m_pStd->GetDataManage()->GetSteelBolt();
	CPlateBasicIndex *pBxR  = (CPlateBasicIndex*)pBx->GetBxMatchByCrossBeam(pGirN);
	CSection         *pSec  = pBx->GetSection();
	CPlateCrossBeam  *pC    = pSec->GetCrossBeam();

	if(!m_pCrossBeamCurrent) return;

	double dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dHeightGirR = pGirN->GetHeightGirderByStaAng(pBxR->GetStation());
	double dWidthGir   = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dThickWeb   = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dSharpLeft  = pGir->GetLengthSharpOnBx(pBx, TRUE);
	double dSharpRight = pGirN->GetLengthSharpOnBx(pBxR, TRUE);
	double tmp_mG      = 40;
	BOOL	bHSteel		= FALSE;
	long	nBridgeType	= pDB->GetBridgeType();	
	double dBrT = 12;	// 수평브레이싱 연결판 두께 권고안

	if(pC->GetType()==CR_TYPE_HSTEEL)// 형강 타입
	{
		bHSteel	= TRUE;
		pMat->SetSteelMaterialDefault(pC->GetSteelMaterialData(), CSteelMaterialData::HSTEEL);
		SetSteelTypeCombo();
		SetSteelMaterialCombo();

		pC->SetHSteelData();

		pC->m_nLGirSpType	= FALSE;
		pC->m_nRGirSpType	= FALSE;
		pC->m_cGasset_CR	= TRUE;
		pC->m_nLevelType	= m_nCurLevelType = 0;
	}	
	else
	{
		pC->m_nLGirSpType	= TRUE;
		pC->m_nRGirSpType	= TRUE;
		pC->m_cGasset_CR	= FALSE;
		pC->m_nLevelType	= m_nCurLevelType = 1;
	}

	//최초 실행되는 권고안이므로 세팅해줌...SetHunchTopConnect()함수에서 사용..
	if(pC->GetType()==CR_TYPE_VSTIFF)	pC->m_bHunchTopConnect = FALSE;
	else if(pC->m_nLevelType == 0)		pC->m_bHunchTopConnect = FALSE;
	else if(pBx->IsJijum())				pC->m_bHunchTopConnect = TRUE;
	else								pC->m_bHunchTopConnect = FALSE;

	pC->m_P_CR		=  500;
	if((pC->m_dLH1==0 || pC->m_dRH1==0) || ( pC->m_nLGirSpType==FALSE || pC->m_nRGirSpType==FALSE))
		pC->m_P_CR	=  800;
	// m_P_CR을 재정의(수직보강재 연결타입)
	if(pC->GetType()==CR_TYPE_VSTIFF)
	{
		CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		double dCross	= fabs(pC->GetVectorPlan().RotateInv(vGir).y);
		if(pC->IsSkew())	pC->m_P_CR = pC->m_vW_G*dCross;
		else				pC->m_P_CR = pC->m_vW_G;
	}

	// 복부 높이
	if(!bHSteel)
	{
		if(pBx->IsJijum())
		{
			if(!pC->m_bHunchTopConnect)	pC->m_H_CR = dHeightGir*0.4;
			else
			{
				CDPoint vAngCross = pC->GetVectorCross();
				double dTemp = (pC->m_P_CR-(dWidthGir-dThickWeb)/2)*vAngCross.y;
				pC->m_H_CR = dHeightGir - pBx->GetOffsetFromLowerFlange() - dTemp;
			}
		}
		else				pC->m_H_CR = dHeightGir*0.5;
	}
	pC->m_H_CR = Round(pC->m_H_CR, 0);

	if(!bHSteel)
	{
		pC->m_T1_CR		= 12;	// 하부플랜지 두께
		pC->m_T2_CR		= 12;	// 상부플랜지 두께
		pC->m_T3_CR		= 12;	// 복부 두께
		pC->m_uM_CR		= 100;	// 상부플랜지 폭
		pC->m_dM_CR		= 100;	// 하부플랜지 폭
	}

	SetHunchTopConnect(nG, nPos);	//m_dLH1, m_dLH2 Setting

	// 수직보강재의 돌출폭은 복부판 높이의 1/30에 50mm더한 값보다 크게 잡고, 
	// 보강재의 두께는 돌출폭의 1/13이싱이어야 한다.  -강도로교 상세부 설계지침 p28

	pC->m_vW_G  = RoundUp(pGir->GetHeightGirderByStaAng(pBx->GetStation())/30 + 50, -1);
	pC->m_vT_G  = pDB->IsPlateGir() ? 22 : RoundUp(pC->m_vW_G/13, 0);

	if(pDB->IsSosuGir())					pC->m_vW_G = 250;
	else if(pDB->IsTUGir())					pC->m_vW_G = 300;
	else if(pC->GetType()==CR_TYPE_VSTIFF)	pC->m_vW_G = max(pC->m_vW_G, 200);

	if(pBx->IsJijum())
	{
		CVStiff *pVStiff = pBx->GetJijumStiff();
		pC->m_vW_G = pVStiff ? pVStiff->m_dWidth[0] : 0;
		pC->m_vT_G = pVStiff ? pVStiff->m_dThick[0] : 0;
	}
	pC->m_vW1_G = 0;

	pC->m_dbL1 = 50;
	pC->m_dbL2 = 50;	
	pC->m_dbL3 = 20;
	pC->m_dbL4 = (pC->m_dLH1 > pGir->GetThickFactByStation(pBx->GetStation(), G_F_U)) ? 20 : ((dSharpLeft+20)-pC->m_vW_G);

	pC->m_dbR1 = 50;
	pC->m_dbR2 = 50;	
	pC->m_dbR3 = 20;
	pC->m_dbR4 = (pC->m_dRH1 > pGirN->GetThickFactByStation(pBxR->GetStation(), G_F_U)) ? 20 : ((dSharpRight+20)-pC->m_vW_G);

	pC->m_dwB = 0;
	pC->m_dwR = 20;
	pC->m_dwH = 20;
	pC->m_wF  = 80;
	pC->m_wE  = (long)((pC->m_H_CR-tmp_mG*2)/pC->m_wF);
	if(pC->m_wF*pC->m_wE+80 > pC->m_H_CR-pC->m_dwH)
		pC->m_wE--;

	pC->m_uN_CR		= 100;
	pC->m_dN_CR		= 100;

	CString str;
	str.Format("%s %s", pDB->GetMaterial(CG_BOLT, pC->GetBx()), pC->m_ZzsBoltSize);
	long nIdx		= pBolt->GetIndex(str);
	double dEdge	= pBolt->GetCalcEdgeDistance(nIdx);

	pC->m_uA	= bHSteel ? 0 : dEdge;	
	pC->m_uC	= bHSteel ? 0 : 80;
	pC->m_uC1	= bHSteel ? 0 : 85;
	if((pC->m_uA+2*pC->m_uC+pC->m_uC1/2+20) <= pC->m_P_CR)
				pC->m_uB = 2;
	else		pC->m_uB = 1;
	if(bHSteel)	pC->m_uB = 0;

	pC->m_X_CR  = 20;
	pC->m_uD	= (pC->m_uM_CR-pC->m_T3_CR/2-20)/2;
	pC->m_uE	= 0;
	pC->m_uF	= 0;
	pC->m_uW	= pC->m_uD*2;
	pC->m_uT	= 10;
	//
	pC->m_dA	= pC->m_uA;
	pC->m_dB	= pC->m_uB;
	pC->m_dC	= pC->m_uC;
	pC->m_dC1	= pC->m_uC1;
	pC->m_dD	= pC->m_uD;
	pC->m_dE	= pC->m_uE;
	pC->m_dF	= pC->m_uF;
	pC->m_dW	= pC->m_uW;
	pC->m_dT	= pC->m_uT;
	//
	pC->m_sA	= pC->m_uA;
	pC->m_sB	= pC->m_uB-1;
	pC->m_sC 	= pC->m_uC;
	pC->m_sC1	= pC->m_uC1;
	// 
	pC->m_sF	= 80;
	pC->m_sE	= (long)((pC->m_H_CR-tmp_mG*2)/pC->m_sF);
	pC->m_sW	= pC->m_sE*pC->m_sF + tmp_mG*2;
	if(pC->m_sW+80 > pC->m_H_CR)
		pC->m_sE--;
	pC->m_sW	= pC->m_sE*pC->m_sF + tmp_mG*2;
	pC->m_sT	= pC->m_T3_CR;
	// 모멘트 이음판
	pC->m_mA	= 0;
	pC->m_mB	= 0;
	pC->m_mC	= 0;
	pC->m_mC1	= 0;
	pC->m_mG	= 0;
	pC->m_mD	= 0;
	pC->m_mE	= 0;
	pC->m_mF	= 0;
	pC->m_mW	= 0;
	pC->m_mT	= 0;
	pC->m_mL	= 0;

	if(bHSteel)	// H 형강 타입
	{
		pC->m_sL = pBolt->GetBoltLength(pBx, CG_BOLT, pC->m_ZzsBoltSize, pC->m_T3_CR + pC->m_vT_G);
	}
	else
	{
		if(pC->GetType() == CR_TYPE_MANUFACTURE && pC->m_nLGirSpType == 0)//제작타입이고 좌/우측 이음판이 없을경우
		{
			pC->m_sL = pBolt->GetBoltLength(pBx, CG_BOLT, pC->m_ZzsBoltSize, pC->m_T3_CR + pC->m_vT_G);
		}
		else
		{
			double  T3 = pC->m_sT*2+pC->m_T3_CR;	// 복부
			pC->m_sL  = pBolt->GetBoltLength(pBx, CG_BOLT, pC->m_ZzsBoltSize, T3);
		}

		double  T2 = pC->m_uT*2+pC->m_T2_CR;	// 상부
		double  T1 = pC->m_dT*2+pC->m_T1_CR;	// 하부
		pC->m_dL = pBolt->GetBoltLength(pBx, CG_BOLT, pC->m_ZzsBoltSize, T1);
		pC->m_uL = pBolt->GetBoltLength(pBx, CG_BOLT, pC->m_ZzsBoltSize, T2);					
	}
		
	//
	// 수평보강재
	pC->m_nHStiffUppersu_CR = 0;
	pC->m_nHStiffLowersu_CR = 0;
	pC->m_nQtyStringer_CR	= 0;
	pC->m_hW_CR = 0;
	pC->m_hT_CR = 0;
	
	if(nBridgeType ==1 && pBx->IsJijum())//소수주형이고 지점부이면..
	{
		pC->m_nVStiffsu_CR = 3;
		pC->m_nAnchorsu_CR = 3;
	}
	else if(nBridgeType==2)
	{
		pC->m_nVStiffsu_CR = 3;
		pC->m_nAnchorsu_CR = 0;
	}
	else if(bHSteel)
		pC->m_nVStiffsu_CR = 0;	

	pC->m_bBoltInterver = FALSE;
	pC->m_dBoltInterver = 80;
	// Combo
	pC->m_cVSide_CR  = 0;

	if(pC->GetType() == CR_TYPE_VSTIFF && !pDB->IsPlateGir())//소수주형이고 수직보강재 연결일경우만 채움판...
	{
		pC->m_sL1 = pC->GetFillerBoltLength(pBx, TRUE);
	}
	else
	{
		pC->m_sL1 =0;
	}

	pC->m_dM1 =	pC->m_uM1 = 100;
	pC->m_dD1 =	pC->m_uD1 = 40;
	pC->m_dE1 =	pC->m_uE1 = 1;
	pC->m_dF1 =	pC->m_uF1 = 80;
	pC->m_dB1 =	pC->m_uB1 = 1;
	if(pC->GetType()==CR_TYPE_VSTIFF)
		pC->m_bConnectionPlate = TRUE;

	if(!pDB->IsPlateGir())
	{
		if (pBx->IsJijum() || pDB->IsTUGir())//소수주형이고 지점부이거나 TU이면 권고안에서 컨넥션플레이트 False
			pC->m_bConnectionPlate = FALSE;
		else
			pC->m_bConnectionPlate = TRUE;
	}

	if(pC->m_bConnectionPlate)
	{
		if((pC->m_dLH1==0 || pC->m_dRH1==0) || ( pC->m_nLGirSpType==FALSE || pC->m_nRGirSpType==FALSE))
			pC->m_P_CR	= 800;
		
		pC->m_dM_CR	= pC->m_uM_CR	= 120;
		pC->m_dN_CR	= pC->m_uN_CR	= 150;
		pC->m_dA	= pC->m_uA		= dEdge;
		pC->m_dB	= pC->m_uB		= 3;
		pC->m_dC	= pC->m_uC		= 75;
		pC->m_dC1	= pC->m_uC1		= 105;
		pC->m_dD	= pC->m_uD		= 40;
		pC->m_dE	= pC->m_uE		= 0;
		pC->m_dF	= pC->m_uF		= 0;
		pC->m_dW	= pC->m_uW		= 155;
		pC->m_dT	= pC->m_uT		= 10;
		pC->m_dL	= pC->m_uL		= 75;
		pC->m_dM1	= pC->m_uM1		= 175;
		pC->m_dD1	= pC->m_uD1		= 40;
		pC->m_dE1	= pC->m_uE1		= 1;
		pC->m_dF1	= pC->m_uF1		= 80;
		pC->m_dB1	= pC->m_uB1		= 1;
	}

	//좌측보강재권고안
	pC->m_dltH    = dHeightGir-pC->m_T1_CR-pC->m_T2_CR-pC->m_dLH1-pC->m_H_CR;
	CDPoint vDir  = pC->GetVectorCross();
	if(pC->m_dLH1==0)//ksg수정
		pC->m_dltH = dHeightGir-pC->m_T1_CR-pC->m_H_CR-(pC->m_dbL2+pC->m_dbL3+pC->m_dbL1)*vDir.y - pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	pC->m_ltF = 80;	
	pC->m_ltE = (long)((pC->m_dltH-tmp_mG*2)/pC->m_ltF);
	if(pC->m_ltE*pC->m_ltF+80 > pC->m_dltH)
		pC->m_ltE--;
	//우측보강재권고안
	pC->m_drtH    = dHeightGirR-pC->m_T1_CR-pC->m_T2_CR-pC->m_dRH1-pC->m_H_CR;
	vDir  = pC->GetVectorCross();
	if(pC->m_dRH1==0)//ksg수정
		pC->m_drtH = dHeightGirR-pC->m_T1_CR-pC->m_H_CR-(pC->m_dbR2+pC->m_dbR3+pC->m_dbR1)*vDir.y - pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	pC->m_rtF = 80;	
	pC->m_rtE = (long)((pC->m_drtH-tmp_mG*2)/pC->m_rtF);
	if(pC->m_rtE*pC->m_rtF+80 > pC->m_drtH)
		pC->m_rtE--;

	// 수평 브레이싱 연결판
	CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
	pBrRu->m_dT		= dBrT;
	pBrRu->m_ddH	= pBx->GetOffsetFromLowerFlange();
	pBrRu = pBxR->GetHBracingGusset();
	if(pBrRu)
	{
		pBrRu->m_dT		= dBrT;
		pBrRu->m_ddH	= pBxR->GetOffsetFromLowerFlange();
	}

	// 앵커, 수직보강재 권고안
	if(pC->m_nVStiffsu_CR>0)
		SetVStiffsuByBx(pBx);
	if(pC->m_nAnchorsu_CR>0)
		SetAnchorsuByBx(pBx);
	if(pC->m_nVStiffsu_CR>0)
	{
		pC->m_vW_CR = 100;
		pC->m_vT_CR = 12;		
	}
}

void CDetCrossBeamTypeDlg::SetDataSave()
{
	UpdateData(TRUE);
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(m_nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxCrossBeamOnly(m_nPos);
	if(pBx==NULL)	return;

	CPlateCrossBeam	*pC = pBx->GetSection()->GetCrossBeam();
	pC->SetSectionCheck(m_bApplyCalc.GetCheck());

	SetDataSaveGridN();
}

void CDetCrossBeamTypeDlg::OnPreInitDialog()
{
	SetInitPositionCombo();	
	SetInitGita();	
}

void CDetCrossBeamTypeDlg::OnDialogNext() 
{	
	SetDataSaveGridN();
	if(m_ctlPosCombo.GetCurSel() == m_ctlPosCombo.GetCount()-1)
	{
		m_pStd->MoveNextDialog();
		HideCrossBeamSubInput();
		return;
	}

	m_ctlPosCombo.SetCurSel(m_ctlPosCombo.GetCurSel()+1);
	OnSelchangeCurPos();
}

void CDetCrossBeamTypeDlg::OnDialogPrev() 
{	
	SetDataSaveGridN();
	if(m_ctlPosCombo.GetCurSel() == 0)
	{
		m_pStd->GetDetFieldJoinDlg()->m_nSp = 0;
		m_pStd->MovePrevDialog();
		HideCrossBeamSubInput();
		return;
	}

	m_ctlPosCombo.SetCurSel(m_ctlPosCombo.GetCurSel()-1);
	OnSelchangeCurPos();
}


void CDetCrossBeamTypeDlg::ChangePosIndex(BOOL bInc)
{/*
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = NULL;

	if(bInc)
	{	//증가
		m_nPos++;
		if(m_nPos >= pDB->GetGirder(m_nG)->GetQtyCrossBeamOnly())
		{
			m_nPos = 0;
			if((m_nG+1) < pDB->GetGirdersu()-1)
				m_nG++;
		}
	}
	else 
	{	//감소
		m_nPos--;
		if(m_nPos < 0) 
		{
			if(m_nG > 0)
				m_nG--;

			m_nPos = pDB->GetGirder(m_nG)->GetQtyCrossBeamOnly()-1;
		}
	}
	SetDlgTitle();
*/}

void CDetCrossBeamTypeDlg::SetDlgTitle()
{
	CString szBridgeName = m_pStd->GetBridge()->m_sNameBridge;
	if(!szBridgeName.IsEmpty())
	{
		CString szTitle;		
		szTitle.Format("[Step %d] %s - 가로보 상세입력 [G%d-G%d-%d]", m_nMenuID - ID_GNINPUT_0, szBridgeName, m_nG+1, m_nG+2, m_nCurPos);//m_nPos+1);	
		SetWindowText(szTitle);
		DrawTitleButton();
	}	
}

void CDetCrossBeamTypeDlg::OnATypeInputcomplete() 
{
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView(FALSE);
	HideCrossBeamSubInput();

	m_pStd->MoveNextDialog();
}

void CDetCrossBeamTypeDlg::OnRadioLRHeightEqual() 
{
	UpdateData();

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx = pGir->GetBxCrossBeamOnly(m_nPos);
	if(pBx==NULL)	return;

	CSection *pSec = pBx->GetSection();
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CPlateCrossBeam *pC = NULL;

	//항상 좌측거더를 기준으로 함.

	pC = pSec->GetCrossBeam();
	if(!pC) return;
	pC->m_nLevelType = m_nCurLevelType;
	if(A[1].y > A[7].y)	pC->m_dLH1 = pC->m_dRH1;
	else				pC->m_dRH1 = pC->m_dLH1;

	SetDataInitGridN();
	SetDataSaveGridN();	

	DrawInputDomyunView(FALSE);
}

void CDetCrossBeamTypeDlg::OnRadioLRHeightHorz() 
{
	UpdateData();           

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);
	CPlateGirderApp *pGirN   = pGir->m_pRight;
	CPlateBasicIndex *pBx = pGir->GetBxCrossBeamOnly(m_nPos);
	if(pBx==NULL)	return;

	CSection         *pSec   = pBx->GetSection();
	CPlateBasicIndex *pBxN   = pBx->GetBxMatchByCrossBeam(pGirN);
	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CPlateCrossBeam *pC = pSec->GetCrossBeam();

	 // 가로보 타입
	if(!pC) return;
	if(pC->m_dLH1==0 || pC->m_dRH1==0)
	{
		CString sz;
		sz.Format("LH와 RH가 0보다 커야합니다.");
		AfxMessageBox(sz);

		m_nCurLevelType = 1;
		UpdateData(FALSE);

		return;
	}
	pC->m_nLevelType = m_nCurLevelType;
	double dGapH = (A[7].y-A[2].y);
	if(A[1].y > A[7].y)	// 좌측거더가 높을 경우
		pC->m_dLH1 = pC->m_dRH1 - dGapH;
	else
		pC->m_dRH1 = pC->m_dLH1 + dGapH;

	SetDataInitGridN();
	SetDataSaveGridN();
	
	DrawInputDomyunView(FALSE); 
}

void CDetCrossBeamTypeDlg::OnSelchangeCurPos() 
{
	SetCrossBeamCurrent();
	SetDataInit();
	SetDlgTitle();
	SetInitGita();

	//UpdateData();
	
	DrawInputDomyunView(TRUE);	
}



void CDetCrossBeamTypeDlg::OnButtonRecommend() 
{
//	UpdateData();
// 	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
// 	CPlateGirderApp	 *pGir    = pDB->GetGirder(m_nG);
// 	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(m_nPos);
// 	CSection *pSec = pBx->GetSection();
// 	CPlateCrossBeam *pC = pSec->GetCrossBeam();	

/*
	if(!IsUsedFlag())
	{
		if(nBridgeType==0)	pC->SetType(CR_TYPE_MANUFACTURE);
		else				pC->SetType(CR_TYPE_VSTIFF);
	} */

	SetDataDefaultSub(m_nG, m_nPos);
	SetInitGita();
	SetDataInitGridN();
		
	DrawInputDomyunView(TRUE);
}


void CDetCrossBeamTypeDlg::SetInitGita()
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp	 *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(m_nPos);

	if(pBx==NULL)	return;

	CSection *pSec = pBx->GetSection();
	CPlateCrossBeam *pC = pSec->GetCrossBeam();
	if(!pC) return;

	m_nCurType      = pC->GetType();
	m_nCurLevelType = pC->m_nLevelType;
	pC->m_nAnchorsu_CR  = pC->m_nAnchorsu_CR;
	pC->m_nVStiffsu_CR  = pC->m_nVStiffsu_CR;
	UpdateData(FALSE);
}


void CDetCrossBeamTypeDlg::SetInitPositionCombo()
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
				if(pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
				{
					strText.Format("G%d-G%d-C%d", nG+1, nG+2, nCount);
					m_ctlPosCombo.AddString(strText);
				}
			}
			pBx = Finder.GetBxNext();
			nCount++;
		}
	}
	m_ctlPosCombo.SetCurSel(0);

	SetCrossBeamCurrent();
}



void CDetCrossBeamTypeDlg::OnApplyAllGirderSameApplyButton() 
{
	CString sz;
	sz.Format("현재 가로보를 기준으로 동일한 횡단상에 있는 가로보를 동일적용 합니다.\n계속 하시겠습니까?", m_nG+1);
	if(MessageBox(LPCTSTR(sz), "가로보 상세 입력 기본값", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	SetDataAllGirderSame();
	SetDataInitGridN();

	UpdateData(FALSE);
	DrawInputDomyunView(FALSE);
}

void CDetCrossBeamTypeDlg::SetDataAllGirderSame()
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(m_nPos);
	if(!pBx) return;
	CPlateBasicIndex *pBxMatch= NULL; 

	CPlateCrossBeam *pC    = m_pCrossBeamCurrent;//GetSection()->GetCrossBeam();
	CPlateCrossBeam *pCSec = NULL;
	if(pC == NULL) return;

	for(long nG = 0; nG < pDB->GetGirdersu()-1; nG++) 
	{
		pGir     = pDB->GetGirder(nG);
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);
		pCSec    = pBxMatch->GetSection()->GetCrossBeam();

		if(pCSec == NULL)	continue;
		if(pCSec == pC)		continue;

		*pCSec <<= *pC;
		SetSameLevel(pC, pCSec);

		pCSec->m_nLGirSpType		= pC->m_nLGirSpType;
		pCSec->m_nRGirSpType		= pC->m_nRGirSpType;
		pCSec->m_bBoltInterver		= pC->m_bBoltInterver;
		pCSec->m_nFillerUseType		= pC->m_nFillerUseType;
		pCSec->m_bHunchTopConnect	= pC->m_bHunchTopConnect;
		pCSec->m_bConnectionPlate			= pC->m_bConnectionPlate;
		pCSec->m_nRadioConnectionPlateInOut	= pC->m_nRadioConnectionPlateInOut;
		long n = 0;
		for(n=0; n<pC->m_nAnchorsu_CR; n++)
			pCSec->m_AnchorDis_CR[n] = pC->m_AnchorDis_CR[n];

		for(n=0; n<pC->m_nVStiffsu_CR; n++)
			pCSec->m_VStiffDis_CR[n] = pC->m_VStiffDis_CR[n];

		pCSec->GetSteelMaterialData()->m_nType = pC->GetSteelMaterialData()->m_nType;
		pCSec->GetSteelMaterialData()->m_nJewonIdx = pC->GetSteelMaterialData()->m_nJewonIdx;
	}
	
}

long CDetCrossBeamTypeDlg::GetMaxVStiffNum(long nPos)
{
	long Max=0, nNum=0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateCrossBeam *pC      = NULL;

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBx  = pGir->GetBxCrossBeamOnly(m_nPos);
		if(!pBx) continue;

		if(nPos >= pDB->GetGirder(nG)->GetQtyCrossBeamOnly()) continue;

		pC = pBx->GetSection()->GetCrossBeam();

		if(pC == NULL)	continue;
		
		nNum = pC->m_nVStiffsu_CR;
		
		if(nNum > Max) Max = nNum;
	}
	return Max;
}

long CDetCrossBeamTypeDlg::GetMaxHStiffNum(long nPos)
{
	long Max=0, nNum=0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateCrossBeam *pC      = NULL;

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBx  = pGir->GetBxCrossBeamOnly(m_nPos);
		if(!pBx) continue;

		if(nPos >= pDB->GetGirder(nG)->GetQtyCrossBeamOnly()) continue;

		pC = pBx->GetSection()->GetCrossBeam();

		if(pC == NULL)	continue;

		nNum = pC->m_nHStiffUppersu_CR;
		
		if( nNum > Max ) Max = nNum;
	}

	return Max;
}


long CDetCrossBeamTypeDlg::GetMaxAnchorNum(long nPos)
{
	long Max=0, nNum=0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateCrossBeam *pC      = NULL;

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBx  = pGir->GetBxCrossBeamOnly(m_nPos);

		if(nPos >= pDB->GetGirder(nG)->GetQtyCrossBeamOnly()) continue;

		pC  = pBx->GetSection()->GetCrossBeam();

		if(pC != NULL)//가로보타입
			nNum = pC->m_nAnchorsu_CR;
		
		if(nNum > Max ) Max = nNum;
	}

	return Max;
}

void CDetCrossBeamTypeDlg::OnCheckTotalview() 
{
	UpdateData();
	CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_TOTALVIEW);
	CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
	BOOL bTotalView = (pButton) ? pButton->GetCheck() : FALSE;
	if(bTotalView)
	{
		m_pStd->GetTotalView()->m_nFlag = BX_CROSSBEAM_VBRACING;
		CDomyun *pDomDlg = m_pStd->GetTotalView()->m_DomView.GetDomyun();
		pDomDlg->SetCWnd(m_pView->GetDomyun()->GetCWnd());
		m_pStd->GetTotalView()->Clear();
		DrawPyung.DrawPyungOnAirialView(pDomDlg);
		m_pStd->ShowTotalView(m_pView);
	}
	else
		m_pStd->HideTotalView();
	DrawInputDomyunView(FALSE);
}

void CDetCrossBeamTypeDlg::SetVStiffsuByBx(CPlateBasicIndex* pBx)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);
	CSection        *pSec    = pBx->GetSection();
	CPlateCrossBeam *pC      = pSec->GetCrossBeam();
	

	if(pC == NULL)	return;
	
	if(pC->m_nVStiffsu_CR>0)
	{
		if(pC->m_vW_CR==0)
			pC->m_vW_CR = Round(pC->m_dM_CR-(pC->m_T3_CR/2+10),-1);
		if(pC->m_vT_CR==0)
			pC->m_vT_CR = pC->m_T3_CR;
	//	if(pC->m_vL_CR==0)
		pC->m_vL_CR = pC->m_H_CR - 50;
	}

	// 수직보강재 부착타입은 수직보강재 폭을 따라감
	double P    = /*pC->GetType() == CR_TYPE_VSTIFF ? pC->m_vW_G : */pC->m_P_CR;
	double l    = 0;
	double l1   = 0;
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;

	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		l  = pGir->GetDisFrontPlateWebToWeb(pBx) - pC->GetLengthGirWebToCrWeb(TRUE) - pC->GetLengthGirWebToCrWeb(FALSE);
		l1 = l;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		l  = pGir->GetDisFrontPlateWebToWeb(pBx) - (pC->GetLengthGirWebToCrWeb(TRUE)+P);
		l1 = l - dWSp/2;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		l  = pGir->GetDisFrontPlateWebToWeb(pBx) - (pC->GetLengthGirWebToCrWeb(FALSE)+P);
		l1 = l - dWSp/2;
	}
	else
	{
		l = pGir->GetDisFrontPlateWebToWeb(pBx) - P*2;
		l1= l - dWSp;
	}
	//세로보는 고려안함.
	for(long n = 0;n < pC->m_nVStiffsu_CR;n++)
	{
		pC->m_VStiffDis_CR[n] = l1/(pC->m_nVStiffsu_CR+1);
		//if( n == 0 ) pC->m_VStiffDis_CR[n] += (l-l1)/2;
	}
}

void CDetCrossBeamTypeDlg::SetVStiffsuByGirder(long nGir)
{
	UpdateData(TRUE);

	if(m_pCrossBeamCurrent->m_nVStiffsu_CR==0)	return;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nGir);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
	while(pBx)
	{		
		CSection *pSec = pBx->GetSection();

		if(pSec->IsenType(SECTION_A))
			SetVStiffsuByBx(pBx);
		
		pBx = Finder.GetBxNext();
	}
}
void CDetCrossBeamTypeDlg::SetVStiffsuBySection(CPlateBasicIndex* pBx)
{
	UpdateData(TRUE);
	CPlateBridgeApp*  pDB = m_pStd->GetBridge();
	CPlateGirderApp*  pGir    = pDB->GetGirder(m_nG);
	pBx = pBx->GetBxMatchByCrossBeam(pGir);//현재 Bx가 첫번째 거더와 match하는 bx

	long nGSu = pDB->GetGirdersu();

	for(long nG=0; nG<nGSu-1; nG++)
	{
		pGir = pDB->GetGirder(nG);
		SetVStiffsuByBx(pBx);
		pBx = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	}
}
void CDetCrossBeamTypeDlg::SetVStiffsu()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
		SetVStiffsuByGirder(nG);
}

void CDetCrossBeamTypeDlg::SetAnchorsuByBx(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CSection        *pSec = pBx->GetSection();
	CVector vAng = pBx->GetAngleSection();

	CPlateCrossBeam*  pC  = pSec->GetCrossBeam();
	

	if(pC == NULL)	return;
	
//	pC->m_nAnchorsu_CR = pC->m_nAnchorsu_CR;
	double P  = pC->m_P_CR;
	double l = 0;
	double l1 = 0;
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		l  = pGir->GetDisFrontPlateWebToWeb(pBx) - pC->GetLengthGirWebToCrWeb(TRUE) - pC->GetLengthGirWebToCrWeb(FALSE);
		l1 = l;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		l  = pGir->GetDisFrontPlateWebToWeb(pBx) - (pC->GetLengthGirWebToCrWeb(TRUE) +P);
		l1 = l - dWSp/2;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		l  = pGir->GetDisFrontPlateWebToWeb(pBx) - (pC->GetLengthGirWebToCrWeb(FALSE)+P);
		l1 = l - dWSp/2;
	}
	else
	{
		l = pGir->GetDisFrontPlateWebToWeb(pBx) - P*2;
		l1= l - dWSp;
	}
	for(long n=0; n<pC->m_nAnchorsu_CR; n++)
		pC->m_AnchorDis_CR[n] = l1/(pC->m_nAnchorsu_CR+1);
}

void CDetCrossBeamTypeDlg::SetAnchorsuByGirder(long nGir)
{
	UpdateData(TRUE);
	CPlateBridgeApp*  pDB = m_pStd->GetBridge();
	CPlateGirderApp*  pGir    = pDB->GetGirder(nGir);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	while(pBx)
	{	
		CSection* pSec = pBx->GetSection();
		if(pSec->IsenType(SECTION_A))
			SetAnchorsuByBx(pBx);

		pBx = Finder.GetBxNext();
	}
}


void CDetCrossBeamTypeDlg::SetAnchorsuBySection(CPlateBasicIndex* pBx)
{
	UpdateData(TRUE);
	CPlateBridgeApp*  pDB = m_pStd->GetBridge();
	CPlateGirderApp*  pGir    = pDB->GetGirder(m_nG);
	long nGSu = pDB->GetGirdersu();
	pBx = pBx->GetBxMatchByCrossBeam(pGir);//현재 Bx가 첫번째 거더와 match하는 bx
	for(long nG=0; nG<nGSu-1; nG++)
	{
		pGir = pDB->GetGirder(nG);
		SetAnchorsuByBx(pBx);
		pBx = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	}
}

void CDetCrossBeamTypeDlg::SetAnchorsu()
{
	CPlateBridgeApp* pDB = m_pStd->GetBridge();
	long nGSu = pDB->GetGirdersu();
	for(long nG=0; nG<nGSu-1; nG++)
		SetAnchorsuByGirder(nG);
}

void CDetCrossBeamTypeDlg::OnDlgUserDesign() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	CPlateBasicIndex *pBx = pDB->GetGirder(m_nG)->GetBxCrossBeamOnly(m_nPos);
	if(pBx==NULL)	return;

	CPlateStressOut	pStressOut(m_pStd);
 
	CStringArray Arr;
	CMap <CString, LPCTSTR, double, double> Map;
	pStressOut.SectionCheckOutput(Arr, pBx, Map);
	m_pStd->OutputViewOnAView(Arr);
}

/////////////////////////////////////////////////////////////////
// 가로보에 설치되는 거더수직보강재의 제원을 결정함
// m_vW1_G 는 외측거더에서만 적용됨
// 수직보강재의 설치여부는 폭으로 확인함. m_dH[][Up] 의 값이 dHGir 면 없는것
/////////////////////////////////////////////////////////////////
void CDetCrossBeamTypeDlg::SetDataVStiffJewon(CPlateBasicIndex *pBx)
{
	if(!pBx) return;

	CPlateBridgeApp  *pDB = m_pStd->GetBridge();		
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBasicIndex *pBxMatch= pBx->GetBxMatchByCrossBeam(pGir->m_pRight);

	//double    dHGir   = pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	CSection *pSec    = pBx->GetSection();
	CVStiff  *pVStiffL= pSec->GetVStiff();
	CVStiff  *pVStiffR= pBxMatch->GetSection()->GetVStiff();
	CPlateCrossBeam* pC = pSec->GetCrossBeam();
	CPlateCrossBeam* pCRight = pBxMatch->GetSection()->GetCrossBeam();
	if(!pC) return;

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
	// 1번 거더일 때 (첫번째 가로보 영역)
	if(pBx->GetNumberGirder()==0)
	{
		// 가로보 기준 좌측
		pVStiffL->m_dWidth[0]		= pC->m_vW_G;//0;
		pVStiffL->m_dThick[0]		= pC->m_vT_G;//0;
		pVStiffL->m_dH[LEFT][UP]	= 0;//dHGir;
		pVStiffL->m_dH[LEFT][DOWN]	= 0;

		pVStiffL->m_dWidth[1]		= pC->m_vW_G;
		pVStiffL->m_dThick[1]		= pC->m_vT_G;

		pVStiffR->m_dWidth[0]		= pC->m_vW_G;
		pVStiffR->m_dThick[0]		= pC->m_vT_G;

		// 거더가 두개일 경우 2번째 거더의 수직보강재를 처리함
		if(pDB->GetGirdersu()==2)
		{
			pVStiffR->m_dWidth[1]		= pC->m_vW_G;//0;
			pVStiffR->m_dThick[1]		= pC->m_vT_G;//0;
			pVStiffR->m_dH[RIGHT][UP]	= 0;//dHGir;
			pVStiffR->m_dH[RIGHT][DOWN]	= 0;
		}
	}
	////////////////////////////////////////////
	// 마지막 바로 이전 거더일 때 (마지막 가로보 영역)
	else if(pBx->GetNumberGirder()==pDB->GetGirdersu()-2)
	{
		// 가로보 기준 좌측
		pVStiffL->m_dWidth[1]		= pC->m_vW_G;
		pVStiffL->m_dThick[1]		= pC->m_vT_G;

		// 가로보 기준 우측
		pVStiffR->m_dWidth[0]		= pC->m_vW_G;
		pVStiffR->m_dThick[0]		= pC->m_vT_G;

		pVStiffR->m_dWidth[1]		= pC->m_vW_G;//0;
		pVStiffR->m_dThick[1]		= pC->m_vT_G;//0;
		pVStiffR->m_dH[RIGHT][UP]	= 0;//dHGir;
		pVStiffR->m_dH[RIGHT][DOWN]	= 0;
	}
	////////////////////////////////////////////
	// 중간위치 거더일때 (중앙부가로보영역)
	else
	{
		// 좌측
		pVStiffL->m_dWidth[1]		= pC->m_vW_G;
		pVStiffL->m_dThick[1]		= pC->m_vT_G;

		// 우측
		pVStiffR->m_dWidth[0]		= pC->m_vW_G;
		pVStiffR->m_dThick[0]		= pC->m_vT_G;
	}

	if(pDB->IsTUGir() && !pBx->IsJijum())
	{
		pVStiffL->m_dH[RIGHT][UP]	= m_pStd->m_pDataManage->GetGlobalOption()->GetInsulationVStiffUF();//max(50, pVStiffL->m_dH[RIGHT][UP]);
		pVStiffR->m_dH[LEFT][UP]	= m_pStd->m_pDataManage->GetGlobalOption()->GetInsulationVStiffUF();//max(50, pVStiffR->m_dH[LEFT][UP]);
	}

	if(pBx->GetNumberGirder()==0)
		pVStiffL->m_dH[LEFT][UP] = pVStiffL->m_dH[RIGHT][UP];
	if(pBx->GetNumberGirder()==pDB->GetQtyGirder()-1)
		pVStiffR->m_dH[RIGHT][UP] = pVStiffR->m_dH[LEFT][UP];
}

void CDetCrossBeamTypeDlg::SetBoltInterver()
{

	CPlateBridgeApp	*pDB = m_pStd->GetBridge();
	UpdateData(TRUE);
	double dTmp_mG = 40;

	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++) 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		long nCount = pGir->GetQtyCrossBeamOnly();

		for(long nPos = 0 ; nPos < nCount ; nPos++)
		{
			CPlateBasicIndex	*pBx	= pGir->GetBxCrossBeamOnly( nPos);
			CSection			*pSec	= pBx->GetSection();

			CPlateCrossBeam *pC	= pBx->GetSection()->GetCrossBeam();
			if(!pC) continue;
//			if(m_checkBoltInterver.GetCheck())
			{
				pC->m_dC	= pC->m_dBoltInterver;
				pC->m_uC	= pC->m_dBoltInterver;
				pC->m_sC	= pC->m_dBoltInterver;
				pC->m_wF	= (long)pC->m_dBoltInterver;
				pC->m_sF	= pC->m_dBoltInterver;
				if( (pC->m_uA+2*pC->m_uC+pC->m_uC1/2+20) <= pC->m_P_CR ) pC->m_uB = 2;
				else pC->m_uB = 1;
				pC->m_dB	= pC->m_uB;
				pC->m_sB	= pC->m_uB-1;
				pC->m_sE	= (long)((pC->m_sW-dTmp_mG*2)/pC->m_sF);
				pC->m_wE	= (long)((pC->m_H_CR-dTmp_mG*2)/pC->m_wF);
				if(pC->m_cGasset_CR)
				{
					pC->m_ltF = (long)pC->m_dBoltInterver;
					pC->m_rtF = (long)pC->m_dBoltInterver;
					pC->m_ltE = (long)(pC->m_dltH-dTmp_mG*2)/pC->m_ltF;
					if(pC->m_ltE*pC->m_ltF+80 > pC->m_dltH)
						pC->m_ltE--;
					pC->m_rtE = (long)((pC->m_drtH-dTmp_mG*2)/pC->m_rtF);
					if(pC->m_rtE*pC->m_rtF+80 > pC->m_drtH)
						pC->m_rtE--;
				}
			}
//			pC->m_bBoltInterver = m_checkBoltInterver.GetCheck();
//			pC->m_dBoltInterver = m_dBoltInterver;
			
		}
	}
	SetDataInit();
	UpdateData(FALSE);
}

BOOL CDetCrossBeamTypeDlg::IsFocusItem(CString szTitle)
{
	if(szTitle.Find("<")!=-1)	return TRUE;
	if(m_nTab==1)	return FALSE;
	
	if(szTitle=="wE@wF") return TRUE;
	if(szTitle=="tlE@tlF") return TRUE;
	if(szTitle=="trE@trF") return TRUE;
	if(szTitle=="PL-vWxvTxvL") return TRUE;
	if(szTitle=="PL-hW x hT") return TRUE;
	if(szTitle=="uB@uC") return TRUE;
	if(szTitle=="dB@dC") return TRUE;
	if(szTitle=="sB@sC") return TRUE;
	if(szTitle=="sE@sF") return TRUE;
	if(szTitle=="mE@mF") return TRUE;
	if(szTitle=="uB1@uC")	return TRUE;
	if(szTitle=="uE1@uF1")	return TRUE;
	if(szTitle=="dB1@dC")	return TRUE;
	if(szTitle=="dE1@dF1")	return TRUE;
	if(szTitle=="uM1")	return TRUE;
	if(szTitle=="dM1")	return TRUE;
	if(szTitle=="uE@uF")	return TRUE;
	if(szTitle=="dE@dF")	return TRUE;
	if(szTitle=="dD1")	return TRUE;
	if(szTitle=="uD1")	return TRUE;

	if(GetDlgItem(IDC_GRIDN)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRIDN);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				for(long nRow=0; nRow<pGrid->GetRowCount(); nRow+=2)
				{				
					if(pGrid->GetTextMatrix(nRow,nCol)==szTitle)
					{
						return TRUE;					
					}
				}
			}
		}
	}	
	return CGeneralBaseDlg::IsFocusItem(szTitle);
}

void CDetCrossBeamTypeDlg::SetFocusItem(CString szTitle)
{
	CString szText;
	long nRow = GetFixRowAtGridN("공통");
	long nCol = 1;

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
		szCr.Format("C%d", m_nPos+1);

		szText.Format("%s-%s-%s", szGir1, szGir2, szCr);

		for(long nCombo=0; nCombo<m_ctlPosCombo.GetCount(); nCombo++)
		{
			CString szCombo = _T("");
			m_ctlPosCombo.GetLBText(nCombo, szCombo);

			if(szCombo==szText)
			{
				m_ctlPosCombo.SetCurSel(nCombo);
				UpdateData(FALSE);
				SetCrossBeamCurrent();
				SetDataInit();
				DrawInputDomyunView(FALSE);
			}
		}

		return;
	}
	//////////////////////////////////////////////////////////////////////////
	
	if(nRow!=-1)
	{
		nRow++;
		if(szTitle=="LH")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="RH")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="P")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="H")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="T1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="T2")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="T3")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="vWG")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="vW1G")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="vTG")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="X")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="vTL")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="VHL")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="vTR")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="VHR")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
	}

	nRow = GetFixRowAtGridN("수직보강재");
	nCol = 1;
	if(nRow!=-1)
	{
		nRow++;
		if(szTitle=="PL-vWxvTxvL")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle.Find("V")!=-1)
		{
			szTitle.TrimLeft("V");
			nCol = atoi(szTitle)+3;
			SetGridFocusCell(nRow,nCol);return;
		}
	}

	nRow = GetFixRowAtGridN("수평보강재");
	nCol = 1;
	if(nRow!=-1)
	{
		nRow++;
		if(szTitle=="hW")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="hT")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
	}

	nRow = GetFixRowAtGridN("브라켓 상부플랜지");
	nCol = 1;
	if(nRow!=-1)
	{			
		nRow++;
		if(szTitle=="uM")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uN")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uA")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uB@uC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uC1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uD")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uE@uF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uW")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uT")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uL")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uM1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uD1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uE1@uF1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uF1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="uB1@uC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
	}
	
	nRow = GetFixRowAtGridN("브라켓 하부플랜지");
	nCol = 1;
	if(nRow!=-1)
	{
		nRow++;
		if(szTitle=="dM")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dN")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dA")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dB@dC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dC1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dD")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dE@dF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dW")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dT")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dL")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dM1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dD1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dE1@dF1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dF1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="dB1@dC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
	}
	nRow = GetFixRowAtGridN("전단이음판");
	nCol = 1;
	if(nRow!=-1)
	{
		nRow++;
		if(szTitle=="sA")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sB@sC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sC1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sE@sF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sW")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sT")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sL")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="sL1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
	}
	nRow = GetFixRowAtGridN("모멘트이음판");
	nCol = 1;
	if(nRow!=-1)
	{
		nRow++;
		if(szTitle=="mA")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mB@mC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mC")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mC1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mE@mF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mW")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mT")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mL")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mD")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="mG")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
	}

	nRow = GetFixRowAtGridN("슬래브앵커");
	if(nRow!=-1)
	{
		nRow++;
		szText	= szTitle;
		szText.TrimLeft("A");
		nCol = atoi(szText);
		SetGridFocusCell(nRow, nCol);
	}

	nRow = GetFixRowAtGridN("볼트 연결시");
	nCol = 1;
	if(nRow!=1)
	{
		nRow++;
		if(szTitle=="bL1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="bL2")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="bL3")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="bL4")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="bR1")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="bR2")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="bR3")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="bR4")	{SetGridFocusCell(nRow,nCol);return;}nCol++;

		if(szTitle=="wB")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="wR")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="wH")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="wE@wF"){SetGridFocusCell(nRow,nCol);return;}nCol++;
	}

	nRow = GetFixRowAtGridN("브라켓 보강판");
	nCol = 1;
	if(nRow!=1)
	{
		nRow++;
		if(szTitle=="tlH")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="tlE@tlF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="tlF")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="trH")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="trE@trF")	{SetGridFocusCell(nRow,nCol);return;}nCol++;
		if(szTitle=="trF")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	}
	
}

void CDetCrossBeamTypeDlg::SetGridFocusCell(long nRow, long nCol)
{
	m_GridN.ResetSelection();
	m_GridN.SetFocusCell(nRow,nCol);
	m_GridN.SendMessage(WM_KEYDOWN,VK_F2);
}

void CDetCrossBeamTypeDlg::OnSelchangeCurTypeCombo() 
{
	UpdateData(TRUE);	

	CPlateBridgeApp  *pDB	= m_pStd->GetBridge();
	CPlateGirderApp  *pGir	= pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx	= pGir->GetBxCrossBeamOnly(m_nPos);
	if(!pBx) return;
	CPlateCrossBeam *pC		= pBx->GetSection()->GetCrossBeam();
	CSteelMaterial	*pMat	= m_pStd->GetDataManage()->GetSteelMaterial();

	pC->SetType(m_nCurType);

	SetDataDefaultSubCrossBeam(m_nG, m_nPos);
	SetDataVStiffJewon(pBx);	// 거더의 수직보강재

	if(pC->GetType()==CR_TYPE_HSTEEL)
	{
		pMat->SetSteelMaterialDefault(pC->GetSteelMaterialData(), CSteelMaterialData::HSTEEL);
		SetSteelTypeCombo();
		SetSteelMaterialCombo();
	}
	
	SetInitGita();
	SetDataInit();

	DrawInputDomyunView(TRUE);
}

void CDetCrossBeamTypeDlg::SetCrossBeamCurrent()
{
	UpdateData();
	
	if(!m_ctlPosCombo.GetSafeHwnd()) return;

	int nCurSel = m_ctlPosCombo.GetCurSel();
	if(nCurSel < 0) return;

	CString rString;
	m_ctlPosCombo.GetLBText(nCurSel, rString);
	if(rString.IsEmpty() == TRUE) return;

	//"G%d-G%d-C%d"
	int nF = rString.Find("-");
	CString sGir = rString.Left(nF);//G%d
	sGir = sGir.Mid(1);//%d

	int nF2 = rString.Find("-", nF+1);
	CString sCr = rString.Mid(nF2+2);


	int nG = atol(sGir) - 1;
	int nCr = atol(sCr);// - 1;

	CPlateBridgeApp* pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	int nCrossCount = 0;
	int nCount = 1;
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec)
		{
			if(pSec->GetenDetType() == SECDET_CROSSBEAM)
			{
				if(nCr == nCount)//nCrossCount)
				{
					m_pCrossBeamCurrent = pSec->GetCrossBeam();
					m_nG = nG;
					m_nPos = nCrossCount;
					m_nCurPos = nCount;
					break;
				}
				nCrossCount++;				
			}
		}
		pBx = Finder.GetBxNext();
		nCount++;
	}
}

void CDetCrossBeamTypeDlg::OnCellChangedDataGridN(NMHDR*nmgv,LRESULT*)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(m_nPos);
	if(pBx==NULL)	return;

	SetDataSaveGridN();
	SetDataVStiffJewon(m_pCrossBeamCurrent->GetBx());
	SetDataInitGridN();

	DrawInputDomyunView(FALSE);
}

void CDetCrossBeamTypeDlg::OnCellChangedRowGridN(NMHDR*nmgv,LRESULT*)
{
	
}

void CDetCrossBeamTypeDlg::OnApplyAllcrossbeamSameapplyButton2() 
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까", "가로보 상세 입력 기본값", 
		MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	SetDataAllSame();
	SetDataSave();
	UpdateData(FALSE);
	DrawInputDomyunView(FALSE);	
}

void CDetCrossBeamTypeDlg::SetDataAllSame()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(m_nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxCrossBeamOnly(m_nPos);
	if(!pBx) return;

	CPlateCrossBeam *pCCur = pBx->GetSection()->GetCrossBeam();
	if(pCCur == NULL) return;
	
	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec && pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
			{
				CPlateCrossBeam *pC2 = pSec->GetCrossBeam();
				if(pC2 && pC2!=pCCur)
				{
					*pC2 <<= *pCCur;
					SetSameLevel(pCCur, pC2);

					pC2->m_nLGirSpType		= pCCur->m_nLGirSpType;
					pC2->m_nRGirSpType		= pCCur->m_nRGirSpType;
					pC2->m_bBoltInterver	= pCCur->m_bBoltInterver;
					*pC2->m_pVStiff			= *pCCur->m_pVStiff;
					pC2->m_nFillerUseType	= pCCur->m_nFillerUseType;
					pC2->m_bHunchTopConnect	= pCCur->m_bHunchTopConnect;
					pC2->m_bConnectionPlate				= pCCur->m_bConnectionPlate;
					pC2->m_nRadioConnectionPlateInOut	= pCCur->m_nRadioConnectionPlateInOut;
					long n = 0;
					for(n=0; n<pCCur->m_nAnchorsu_CR; n++)
						pC2->m_AnchorDis_CR[n] = pCCur->m_AnchorDis_CR[n];

					for(n=0; n<pCCur->m_nVStiffsu_CR; n++)
						pC2->m_VStiffDis_CR[n] = pCCur->m_VStiffDis_CR[n];

					pC2->GetSteelMaterialData()->m_nType		= pCCur->GetSteelMaterialData()->m_nType;
					pC2->GetSteelMaterialData()->m_nJewonIdx	= pCCur->GetSteelMaterialData()->m_nJewonIdx;
				}
			}

			pBx = Finder.GetBxNext();
		}	
	}
}	

int CDetCrossBeamTypeDlg::GetFixRowAtGridN(const CString& sTitle) 
{
	if(m_GridN.GetColumnCount() == 0)
		return -1;

	for(int n = 0; n < m_GridN.GetRowCount(); n++)
	{
		if(sTitle == m_GridN.GetTextMatrix(n, 0))
			return n;
	}

	return -1;
}

// 동일타입 동일적용
void CDetCrossBeamTypeDlg::OnButtonSametype() 
{
	CPlateBridgeApp	*pDB= m_pStd->GetBridge();
	CPlateCrossBeam	*pC	= m_pCrossBeamCurrent;
	if( pC == NULL ) return;

	long	nCurPos	= GetCrossBeamPosition(pC->GetBx());

	CString	szTitle		= _T("");
	CString	szMessage	= _T("");
	CString	szMessage2	= _T("");
	CString	szText		= _T("");
	CString szText2		= _T("");	
	BOOL	bExist		= FALSE;
	BOOL    bCount		= FALSE;
	if(nCurPos==POS_STT_END)		szTitle = "< 시종점부 >";
	else if(nCurPos==POS_CEN_JIJUM)	szTitle = "< 중앙지점부 >";
	else if(nCurPos==POS_GENERAL)	szTitle = "< 일반부 >";
	long nG = 0;
	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		
		long nCount = 1;				
		szMessage2 = _T("");
		szText2.Format("G%d - G%d :", nG+1, nG+2);
		bCount = FALSE;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec && pSec->GetenDetType()==SECDET_CROSSBEAM)
			{
				CPlateCrossBeam *pC2 = pSec->GetCrossBeam();
				long nPos	 = GetCrossBeamPosition(pBx);
				if(pC2 && nPos==nCurPos)
				{
					if (pC2==pC)
					{
						szText.Format(" C%d)\n\n", nCount);
						szTitle += "(" + szText2 + szText;
						pBx = Finder.GetBxNext();
						nCount++;
						continue;
					}
					if (bCount) szText.Format(" ,C%d", nCount);
					else		szText.Format(" C%d", nCount);
					
					szMessage2 += szText;
					bExist	= TRUE;
					bCount  = TRUE;
				}				
			}
			pBx = Finder.GetBxNext();
			nCount++;
		}
		
		if (bCount)
			szMessage += szText2 + szMessage2 + "\n\n";		
	}

	if(!bExist)
	{
		szMessage = "동일 구간의 같은 타입이 없읍니다.";
		MessageBox(szMessage,"가로보 상세",MB_ICONINFORMATION );
		return;
	}
	else szMessage = szTitle + szMessage + "\n위 가로보 데이터가 수정됩니다.\n계속 하시겠습니까";
	if( MessageBox(szMessage,"가로보 상세",MB_ICONQUESTION | MB_YESNO ) == IDNO) return;

	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec && pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
			{
				CPlateCrossBeam *pC2 = pSec->GetCrossBeam();
				long nPos = GetCrossBeamPosition(pBx);
				if(pC2 && pC2!=pC && nPos==nCurPos)
				{
					*pC2 <<= *pC;
					SetSameLevel(pC, pC2);

					pC2->m_nLGirSpType		= pC->m_nLGirSpType;
					pC2->m_nRGirSpType		= pC->m_nRGirSpType;
					pC2->m_bBoltInterver	=pC->m_bBoltInterver;
					*pC2->m_pVStiff			= *pC->m_pVStiff;
					pC2->m_nFillerUseType	=pC->m_nFillerUseType;
					pC2->m_bConnectionPlate	= pC->m_bConnectionPlate;
					pC2->m_bMomentPan		= pC->m_bMomentPan;
					pC2->m_bHunchTopConnect	= pC->m_bHunchTopConnect;
					pC2->m_bConnectionPlate				= pC->m_bConnectionPlate;
					pC2->m_nRadioConnectionPlateInOut	= pC->m_nRadioConnectionPlateInOut;

					long n = 0;
					for(n=0; n<pC->m_nAnchorsu_CR; n++)
						pC2->m_AnchorDis_CR[n] = pC->m_AnchorDis_CR[n];

					for(n=0; n<pC->m_nVStiffsu_CR; n++)
						pC2->m_VStiffDis_CR[n] = pC->m_VStiffDis_CR[n];

					pC2->GetSteelMaterialData()->m_nType = pC->GetSteelMaterialData()->m_nType;
					pC2->GetSteelMaterialData()->m_nJewonIdx = pC->GetSteelMaterialData()->m_nJewonIdx;
				}
			}

			pBx = Finder.GetBxNext();
		}	
	}
}

void CDetCrossBeamTypeDlg::OnButtonJijum() 
{
	CPlateBridgeApp	*pDB= m_pStd->GetBridge();
	CPlateCrossBeam	*pC	= m_pCrossBeamCurrent;
	if( pC == NULL ) return;

	long	nCurPos	= GetCrossBeamPosition(pC->GetBx());

	CString	szTitle		= _T("");
	
	if(nCurPos==POS_GENERAL)
	{
		OnButtonSametype();
		return;
	}
	else
		szTitle = "< 지점부 >";

	CString	szMessage	= _T("");
	CString	szMessage2	= _T("");
	CString	szText		= _T("");
	CString szText2		= _T("");	
	BOOL	bExist		= FALSE;
	BOOL    bCount		= FALSE;
	long	nG			= 0;
	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		
		long nCount = 1;				
		szMessage2 = _T("");
		szText2.Format("G%d - G%d :", nG+1, nG+2);
		bCount = FALSE;

		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec && pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
			{
				CPlateCrossBeam *pC2 = pSec->GetCrossBeam();
				if(pC2 && pBx->IsJijum())
				{
					//szText.Format("G%d - G%d : C%d\n", pBx->GetNumberGirder()+1, pBx->GetNumberGirder()+2, nCount);
					//szMessage += szText;

					if (pC2==pC)
					{
						szText.Format(" C%d)\n\n", nCount);
						szTitle += "(" + szText2 + szText;
						pBx = Finder.GetBxNext();
						nCount++;
						continue;
					}
					if (bCount) szText.Format(" ,C%d", nCount);
					else		szText.Format(" C%d", nCount);
					
					szMessage2 += szText;
					bExist	= TRUE;
					bCount  = TRUE;
				}
				
			}
			pBx = Finder.GetBxNext();
			nCount++;
		}

		if (bCount)
			szMessage += szText2 + szMessage2 + "\n\n";
	}

	if(szMessage==_T("지점부 :"))
	{
		szMessage = "지점부 가로보가 없습니다.";
		MessageBox(szMessage,"가로보 상세",MB_ICONINFORMATION );
		return;
	}
	else szMessage = szTitle + szMessage + "\n위 가로보 데이터가 수정됩니다.\n계속 하시겠습니까";
	
	if( MessageBox(szMessage,"가로보 상세",MB_ICONQUESTION | MB_YESNO ) == IDNO) return;

	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec && pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
			{
				CPlateCrossBeam *pC2 = pSec->GetCrossBeam();
				if(pC2 && pC2!=pC && pBx->IsJijum())
				{
					*pC2 <<= *pC;
					SetSameLevel(pC, pC2);

					pC2->m_nLGirSpType		= pC->m_nLGirSpType;
					pC2->m_nRGirSpType		= pC->m_nRGirSpType;
					pC2->m_bBoltInterver	=pC->m_bBoltInterver;
					*pC2->m_pVStiff			= *pC->m_pVStiff;
					pC2->m_nFillerUseType	=pC->m_nFillerUseType;
					pC2->m_bConnectionPlate	= pC->m_bConnectionPlate;
					pC2->m_bMomentPan		= pC->m_bMomentPan;
					pC2->m_bHunchTopConnect	= pC->m_bHunchTopConnect;
					pC2->m_bConnectionPlate				= pC->m_bConnectionPlate;
					pC2->m_nRadioConnectionPlateInOut	= pC->m_nRadioConnectionPlateInOut;
					long n = 0;
					for(n=0; n<pC->m_nAnchorsu_CR; n++)
						pC2->m_AnchorDis_CR[n] = pC->m_AnchorDis_CR[n];

					for(n=0; n<pC->m_nVStiffsu_CR; n++)
						pC2->m_VStiffDis_CR[n] = pC->m_VStiffDis_CR[n];

					pC2->GetSteelMaterialData()->m_nType = pC->GetSteelMaterialData()->m_nType;
					pC2->GetSteelMaterialData()->m_nJewonIdx = pC->GetSteelMaterialData()->m_nJewonIdx;
				}
			}

			pBx = Finder.GetBxNext();
		}	
	}
}

long CDetCrossBeamTypeDlg::OnClickTotalView(WPARAM wp, LPARAM lp) 
{
	CPlateBasicIndex	*pBx	= (CPlateBasicIndex*)wp;
	if(!pBx)	return 0;
	CSection			*pSec	= pBx->GetSection();
	if(!pSec)	return 0;
	CPlateCrossBeam		*pC		= pSec->GetCrossBeam();
	if(!pC)		return 0;

	if(pSec->GetenDetType()==SECDET_CROSSBEAM)
	{
		if(pC->GetSelfCount() >= 0 && pBx->GetNumberGirder()< m_pStd->GetBridge()->GetGirdersu()-1)
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
				if(pSecCur->IsenDetType(SECDET_CROSSBEAM))
					nSelfSec++;

				nCount++;
			}
			m_nPos = nSelfSec;
			m_nCurPos = nCount;

			CString strText;
			strText.Format("G%d-G%d-C%d", m_nG+1, m_nG+2, m_nCurPos);//m_nPos+1);
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

void CDetCrossBeamTypeDlg::OnDestroy() 
{
	CGeneralBaseDlg::OnDestroy();
}

LRESULT CDetCrossBeamTypeDlg::OnCrossBeamSubInputClose(WPARAM wp,LPARAM lp)
{
	UpdateData(FALSE);
	return 0;
}

LRESULT CDetCrossBeamTypeDlg::OnCrossBeamSubInput(WPARAM wp,LPARAM lp)
{
	CPlateCrossBeam		*pC		= (CPlateCrossBeam*)wp;
	CPlateBasicIndex	*pBx	= m_pCrossBeamCurrent->GetBx();

	if(lp==1)	SetVStiffsuByBx(pBx);
	if(lp==2)	SetAnchorsuByBx(pBx);
	if(pC->m_bBoltInterver && lp==11)
		SetBoltInterver();

	if(m_pCrossBeamCurrent->m_nHStiffUppersu_CR !=0 || m_pCrossBeamCurrent->m_nHStiffLowersu_CR)
	{
		if( pC->m_hW_CR == 0 || pC->m_hT_CR == 0)
		{
			m_pCrossBeamCurrent->m_hW_CR = m_pCrossBeamCurrent->m_dM_CR-m_pCrossBeamCurrent->m_vT_CR/2-10;
			m_pCrossBeamCurrent->m_hT_CR = m_pCrossBeamCurrent->m_vT_CR;
		}
	}
	
	long	nG		= pBx->GetNumberGirder();
	long	nPos	= pBx->GetNumberCrossBeam();
	//가로보 헌지 상면 접합이면..	
	SetHunchTopConnect(nG, nPos);//SetDataInitGridN() 함수 전에 호툴 되어야 한다..

	SetDataInitGridN();
	long nRow = GetFixRowAtGridN("브라켓 보강판");
	if(m_pCrossBeamCurrent->m_cGasset_CR==0)
	{
		m_pCrossBeamCurrent->m_dltH	= 0;
		m_pCrossBeamCurrent->m_dltH	= 0;
		m_pCrossBeamCurrent->m_ltF	= 0;
		m_pCrossBeamCurrent->m_ltE	= 0;
		m_pCrossBeamCurrent->m_drtH	= 0;
		m_pCrossBeamCurrent->m_drtH	= 0;
		m_pCrossBeamCurrent->m_rtF	= 0;
		m_pCrossBeamCurrent->m_rtE	= 0;
	}

	nRow = GetFixRowAtGridN("모멘트이음판");
	if(!m_pCrossBeamCurrent->m_bMomentPan)
	{
		m_pCrossBeamCurrent->m_mA	= 0;
		m_pCrossBeamCurrent->m_mB	= 0;
		m_pCrossBeamCurrent->m_mC	= 0;
		m_pCrossBeamCurrent->m_mC1	= 0;
		m_pCrossBeamCurrent->m_mG	= 0;
		m_pCrossBeamCurrent->m_mD	= 0;
		m_pCrossBeamCurrent->m_mE	= 0;
		m_pCrossBeamCurrent->m_mF	= 0;
		m_pCrossBeamCurrent->m_mW	= 0;
		m_pCrossBeamCurrent->m_mT	= 0;
		m_pCrossBeamCurrent->m_mL	= 0;
	}
	else
	{
		SetDataInitGridNMomentConnectPan(nRow);
	}

	m_pCrossBeamCurrent->m_bConnectionPlate		= pC->m_bConnectionPlate;
	m_pCrossBeamCurrent->m_nRadioConnectionPlateInOut	= pC->m_nRadioConnectionPlateInOut;
	m_pCrossBeamCurrent->m_ZzsBoltSize			= pC->m_ZzsBoltSize;
	m_pCrossBeamCurrent->m_bBoltInterver		= pC->m_bBoltInterver;
	m_pCrossBeamCurrent->m_dBoltInterver		= pC->m_dBoltInterver;
	m_pCrossBeamCurrent->m_nFillerUseType		= pC->m_nFillerUseType;

	DrawInputDomyunView(FALSE);	
	m_GridN.SetRedraw(TRUE, TRUE);

	return 0;
}

//가로보 헌치 상면접합..
//070322...KB...
//적용시   1. LH,RH = 0값...,		2.LH, RH 비활성화...3.슬래브 일반도 가로보 헌치 적용..
//비적용시 1. LH,RH = 권고안값...,	2.LH, RH 활성화...	3.슬래브 일반도 가로보 헌치 미적용..
//권고안값...단부,지점부 적용, 일반부 미적용...
void CDetCrossBeamTypeDlg::SetHunchTopConnect(long nG, long nPos)
{
	CPlateBridgeApp  *pDB	= m_pStd->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);
	CPlateGirderApp  *pGirN   = pGir->m_pRight;
	CPlateBasicIndex *pBx     = pGir->GetBxCrossBeamOnly(nPos);
	if(pBx==NULL)	return;
	CPlateBasicIndex *pBxR  = (CPlateBasicIndex*)pBx->GetBxMatchByCrossBeam(pGirN);
	CSection         *pSec  = pBx->GetSection();
	CPlateCrossBeam  *pC    = pSec->GetCrossBeam();

	double  dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double	dHeightGirR = pGirN->GetHeightGirderByStaAng(pBxR->GetStation());
	long	nBridgeType	= pDB->GetBridgeType();

	//가로보 헌치 상면 접합이면..
	if (pC->m_bHunchTopConnect)
	{
		pC->m_dLH1 = 0;
		pC->m_dRH1 = 0;
	}
	else
	{
		if(nBridgeType==1 && !pC->GetBx()->IsJijum())//지점부 아니고 소수주거더교이면...
		{
			//중간에 위치... (높이 - ((높이*35%)+두께)/2
			double dLR_Pt = (dHeightGir - ((dHeightGir*0.35)+24))/2;
			dLR_Pt = RoundDown(dLR_Pt,-1);
			pC->m_dLH1 = dLR_Pt;
			pC->m_dRH1 = dLR_Pt;
		}
		else if(pC->GetType()==CR_TYPE_VSTIFF)//수직보강재 연결 타입이면 
		{
			if(nBridgeType==2)
			{
				pC->m_dLH1 = (dHeightGir-pC->m_H_CR)/2;
				pC->m_dRH1 = (dHeightGir-pC->m_H_CR)/2;
			}
			else
			{
				pC->m_dLH1 = 150;
				pC->m_dRH1 = 150;
			}
		}
		else if(pC->GetBx()->IsJijum())//지점부이면...
		{
			if(pDB->IsTUGir())
			{
				pC->m_dLH1 = (dHeightGir-pC->m_H_CR)/2;
				pC->m_dRH1 = (dHeightGir-pC->m_H_CR)/2;
			}
			else
			{
				pC->m_dLH1 = 0;
				pC->m_dRH1 = 0;
			}
		}	
		else
		{
			pC->m_dLH1 = 150;
			pC->m_dRH1 = 150;
		}
		//항목 ReadOnly = False

		pC->m_dLH1 = dHeightGir-pBx->GetOffsetFromLowerFlange()-pC->m_H_CR-pC->m_T2_CR;
		pC->m_dRH1 = dHeightGirR-pBxR->GetOffsetFromLowerFlange()-pC->m_H_CR-pC->m_T2_CR;

		if(pC->GetType()==CR_TYPE_HSTEEL)
		{
			pC->m_dLH1 = dHeightGir-pBx->GetOffsetFromLowerFlange()-pC->m_H_CR-pC->m_T2_CR-pC->m_T1_CR;
			pC->m_dRH1 = dHeightGirR-pBxR->GetOffsetFromLowerFlange()-pC->m_H_CR-pC->m_T2_CR-pC->m_T1_CR;
		}
	}
	
	if(pC->m_nLevelType==0)
	{
		CDPoint A[10];
		pGir->GetHunchXyDom(pBx, A);
		double dGapH = (A[7].y-A[2].y);
		if(pC->GetType()==CR_TYPE_HSTEEL)
		{
			if(pC->IsRightHBGu())	pC->m_dLH1 = pC->m_dRH1 - dGapH;
			else					pC->m_dRH1 = pC->m_dLH1 + dGapH;
		}
		else if(A[1].y > A[7].y)	pC->m_dLH1 = pC->m_dRH1 - dGapH;	// 좌측거더가 높을 경우			
		else						pC->m_dRH1 = pC->m_dLH1 + dGapH;
	}
}

void CDetCrossBeamTypeDlg::OnRistRecommend() 
{		
	m_SubCrossDlg.m_pCRBeamDlg = this;
	if(!m_SubCrossDlg.GetSafeHwnd()) m_SubCrossDlg.Create(CSubCrossHeightRistDlg::IDD,this);		
	CRect rect, rectSub;
	GetDlgItem(IDC_RIST_RECOMMEND)->GetWindowRect(&rect);	
	m_SubCrossDlg.GetWindowRect(&rectSub);
	m_SubCrossDlg.SetWindowPos(&wndTop,rect.left, rect.top-rectSub.Height(), -1, -1, SWP_NOSIZE);
	m_SubCrossDlg.ShowWindow(SW_SHOW);	
}

void CDetCrossBeamTypeDlg::OnRistRecommendOnRatio(BOOL bAllCross, double dDan, double dGen, double dCenter, CPlateCrossBeam *pC)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	double tmp_mG      = 40;
	double dRatio;
	if(bAllCross)
	{		
		for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			while(pBx)
			{
				CSection *pSec = pBx->GetSection();
				if(pSec && pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
				{
					CPlateCrossBeam *pC = pSec->GetCrossBeam();
					dRatio = pBx->GetSection()->IsenType(SECTION_A) ? dDan : pBx->GetSection()->IsenType(SECTION_P) ? dCenter : dGen;
					pC->m_H_CR = Round(pBx->GetHeightGirderByBx() * dRatio,-1);					

					pC->m_sF	= 80;
					pC->m_sE	= (long)((pC->m_H_CR-tmp_mG*2)/pC->m_sF);
					pC->m_sW	= pC->m_sE*pC->m_sF + tmp_mG*2;
					if(pC->m_sW+80 > pC->m_H_CR)
						pC->m_sE--;
					pC->m_sW	= pC->m_sE*pC->m_sF + tmp_mG*2;
					pC->m_sT	= pC->m_T3_CR;
				}

				pBx = Finder.GetBxNext();
			}	
		}
	}
	else
	{		
		//최초에 초기화를 위해.. 전체를 초기화 할때 m_pCrossBeamCurrent 가 셋팅이 안되므로 강제로 할당...
		if (pC) m_pCrossBeamCurrent = pC;

		dRatio = m_pCrossBeamCurrent->GetBx()->GetSection()->IsenType(SECTION_A) ? dDan : m_pCrossBeamCurrent->GetBx()->GetSection()->IsenType(SECTION_P) ? dCenter : dGen;
		m_pCrossBeamCurrent->m_H_CR = Round(m_pCrossBeamCurrent->GetBx()->GetHeightGirderByBx() * dRatio,-1);

		m_pCrossBeamCurrent->m_sF	= 80;
		m_pCrossBeamCurrent->m_sE	= (long)((m_pCrossBeamCurrent->m_H_CR-tmp_mG*2)/m_pCrossBeamCurrent->m_sF);
		m_pCrossBeamCurrent->m_sW	= m_pCrossBeamCurrent->m_sE*m_pCrossBeamCurrent->m_sF + tmp_mG*2;
		if(m_pCrossBeamCurrent->m_sW+80 > m_pCrossBeamCurrent->m_H_CR)
			m_pCrossBeamCurrent->m_sE--;
		m_pCrossBeamCurrent->m_sW	= m_pCrossBeamCurrent->m_sE*m_pCrossBeamCurrent->m_sF + tmp_mG*2;
		m_pCrossBeamCurrent->m_sT	= m_pCrossBeamCurrent->m_T3_CR;
	}

	if(bAllCross)
	{
		SetDataInit();
		DrawInputDomyunView(TRUE);
	}	
}

void CDetCrossBeamTypeDlg::ShowCrossBeamSubInput(CWnd *pDlgParent, CWnd *pWnd)
{
	if(!m_pCrossBeamCurrent)	return;
	if(!m_CrossBeamSubInput.GetSafeHwnd())
	{
		m_CrossBeamSubInput.Create(IDD_DIALOG_CROSSBEAM_SUBINPUT,pWnd);
		CRect rect,rectParentWin;
		pDlgParent->GetWindowRect(&rectParentWin);
		m_CrossBeamSubInput.GetWindowRect(&rect);
		m_CrossBeamSubInput.SetWindowPos(&wndTop,rectParentWin.right-rect.Width(),rectParentWin.top-rect.Height(),-1,-1,SWP_NOSIZE);
	}
	m_CrossBeamSubInput.m_pCrossBeam	= m_pCrossBeamCurrent;
	m_CrossBeamSubInput.m_pStd			= m_pStd;
	m_CrossBeamSubInput.ShowWindow(SW_SHOW);
	m_CrossBeamSubInput.SetDataInit();
}

void CDetCrossBeamTypeDlg::HideCrossBeamSubInput()
{
	if(m_CrossBeamSubInput.GetSafeHwnd())
		m_CrossBeamSubInput.ShowWindow(SW_HIDE);
}

long CDetCrossBeamTypeDlg::GetCrossBeamPosition(CPlateBasicIndex *pBx)
{
	if(!m_pCrossBeamCurrent)	return -1;

	long	nPosition = -1;
	long	nG	= pBx->GetNumberGirder();
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex	*pBxFir	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxLast(BX_CROSSBEAM_VBRACING);

	if(pBx==pBxFir || pBx==pBxEnd)	nPosition	= POS_STT_END;
	else if(pBx->IsPier())			nPosition	= POS_CEN_JIJUM;
	else							nPosition	= POS_GENERAL;

	return	nPosition;
}

void CDetCrossBeamTypeDlg::SetSteelTypeCombo()
{
	if(m_pCrossBeamCurrent->GetType()!=CR_TYPE_HSTEEL)
	{
		m_comboSteelType.ShowWindow(SW_HIDE);
		m_comboSteelType.ResetContent();
		return;
	}
	else
	{
		m_comboSteelType.ShowWindow(SW_SHOW);
		m_comboSteelType.ResetContent();
		CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
		CSteelMaterialData	*pMatData	= m_pCrossBeamCurrent->GetSteelMaterialData();

		for(long nCount=0; nCount<CSteelMaterialData::STEELCOUNT; nCount++)
		{
			m_comboSteelType.AddString(pMat->GetTypeName(nCount));
		}
		m_comboSteelType.SetCurSel(pMatData->m_nType);
	}
}

void CDetCrossBeamTypeDlg::SetSteelMaterialCombo()
{
	if(m_pCrossBeamCurrent->GetType()!=CR_TYPE_HSTEEL)
	{
		m_comboSteelMat.ShowWindow(SW_HIDE);
		m_comboSteelMat.ResetContent();
		return;
	}
	else
	{
		m_comboSteelMat.ShowWindow(SW_SHOW);
		m_comboSteelMat.ResetContent();
		CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
		CSteelMaterialData	*pMatData	= m_pCrossBeamCurrent->GetSteelMaterialData();

		long nSteelType = pMatData->m_nType;
		long nJewonIdx	= pMatData->m_nJewonIdx;

		for(long nCount=0; nCount<pMat->GetSizeJewon(nSteelType); nCount++)
			m_comboSteelMat.AddString(pMat->GetJewonDataString(nSteelType, nCount));
		m_comboSteelMat.SetCurSel(nJewonIdx);
	}

//	DrawInputDomyunView(FALSE);
}

void CDetCrossBeamTypeDlg::SetDataSaveSteelMaterial()
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pCurMat	= m_pCrossBeamCurrent->GetSteelMaterialData();
	CSteelMaterialData	*pMatData	= NULL;

	if(m_pCrossBeamCurrent->GetType()!=CR_TYPE_HSTEEL)	return;
	
	long	nSteelType	= m_comboSteelType.GetCurSel();		
	long	nJewonIdx	= m_comboSteelMat.GetCurSel();

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

	m_pCrossBeamCurrent->SetHSteelData();
}

void CDetCrossBeamTypeDlg::OnSelchangeComboSteeltype() 
{
	CSteelMaterial		*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= m_pCrossBeamCurrent->GetSteelMaterialData();

	long nOldType = pMatData->m_nType;
	UpdateData(TRUE);
	//enum { LSTEEL, LVALSTEEL, CSTEEL, CTSTEEL, ISTEEL, HSTEEL, STEELCOUNT };
	//가로보 형강 타입은 뒤에 두개만 있으면 됨...070110....KB	
	long nNewType	= m_comboSteelType.GetCurSel();		
	if (nNewType < 4)
	{
		CString sz;
		sz.Format("가로보에서 사용할 수 없는 타입입니다.");	
		MessageBox(LPCTSTR(sz), "가로보 형강 타입", MB_ICONQUESTION|MB_OK);				
		m_comboSteelType.SetCurSel(nOldType);		
		return;
	}

	pMatData->m_nType = nNewType;
	pMat->SetSteelMaterialDefault(pMatData, pMatData->m_nType);
	SetSteelMaterialCombo();
	SetDataSaveSteelMaterial();
	SetDataInitGridN();
	DrawInputDomyunView(FALSE);
}

void CDetCrossBeamTypeDlg::OnSelchangeComboSteelmaterial() 
{
	SetDataSaveSteelMaterial();
	SetDataInitGridN();
	DrawInputDomyunView(FALSE);
}

void CDetCrossBeamTypeDlg::OnCheckApplyCalc() 
{
	UpdateData(TRUE);
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(m_nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxCrossBeamOnly(m_nPos);
	if(pBx==NULL)	return;

	CPlateCrossBeam	*pC = pBx->GetSection()->GetCrossBeam();
	pC->SetSectionCheck(m_bApplyCalc.GetCheck());
}

void CDetCrossBeamTypeDlg::OnButtonAlldefault() 
{
	SetDataDefault();
	SetDataInit();
	DrawInputDomyunView();	
}

void CDetCrossBeamTypeDlg::SetSameLevel(CPlateCrossBeam *pCCur, CPlateCrossBeam *pCSec)
{
	if(pCCur->m_nLevelType!=0)	return;

	CPlateBasicIndex *pBxCur = pCCur->GetBx();
	CPlateBasicIndex *pBxSec = pCSec->GetBx();
	CPlateGirderApp	 *pGirCur = pBxCur->GetGirder();
	CPlateGirderApp	 *pGirSec = pBxSec->GetGirder();
	
	double dAngCur = pGirCur->GetLine()->GetPyungubae(pBxCur->GetStation(), pGirCur->m_dGirderCenter);
	double dAngSec = pGirSec->GetLine()->GetPyungubae(pBxSec->GetStation(), pGirSec->m_dGirderCenter);

	if(pGirCur->m_dGirderCenter < 0)	dAngCur*=-1;
	if(pGirSec->m_dGirderCenter < 0)	dAngSec*=-1;
	if(dAngCur*dAngSec < 0)	// 구배가 역방향이면
	{
		pCSec->m_dLH1	= pCCur->m_dRH1;
		pCSec->m_dRH1	= pCCur->m_dLH1;
	}
	if(dAngCur!=0 && pGirSec->m_dGirderCenter<0 && pGirSec->m_pRight->m_dGirderCenter > 0)
	{
		pCSec->m_dLH1 = pCSec->m_dRH1 = pCCur->m_dRH1;
	}
}
