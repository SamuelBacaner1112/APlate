// DetFieldJoinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetFieldJoinDlg.h"

#include "../APlateDBDraw/APlateDBDraw.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetFieldJoinDlg dialog


CDetFieldJoinDlg::CDetFieldJoinDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetFieldJoinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetFieldJoinDlg)
	m_bTotalView = FALSE;
	m_dPermissrate = 90.0;
	m_bAutoCalcAllG = FALSE;
	//}}AFX_DATA_INIT
	m_ZzsBoltSize = _T("");
	m_bSpliceSimpleType = TRUE;
	m_nG  = 0;
	m_nSp = 0;
}


void CDetFieldJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetFieldJoinDlg)
	DDX_Control(pDX, IDC_SPLICEDLG_BOLTSIZE, m_ComboBox2);
	DDX_Control(pDX, IDC_SPLICEDLG_CURRENTSPLICE, m_CurrSplice);
	DDX_CBString(pDX, IDC_SPLICEDLG_BOLTSIZE, m_ZzsBoltSize);
	DDX_Check(pDX, IDC_SPLICEDLG_SIMPLYTYPE, m_bSpliceSimpleType);
	DDX_Check(pDX, IDC_CHECK_TOTALVIEW, m_bTotalView);
	DDX_Text(pDX, IDC_PERMISSRATE, m_dPermissrate);
	DDV_MinMaxDouble(pDX, m_dPermissrate, 1., 100.);
	DDX_Check(pDX, IDC_AUTOCALC_ALLGIRDER, m_bAutoCalcAllG);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRIDN, m_Grid);
	DDX_Control(pDX, IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC, m_btnBoltAutoLength);
	DDX_Control(pDX, IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC_ALL, m_btnBoltAutoLengthAll);
	DDX_Control(pDX, IDC_SPLICEDLG_ALLTYPEAPPLY_STRENGTH, m_btnAllTension);
	DDX_Control(pDX, IDC_SPLICEDLG_ALLTYPEAPPLY_COMPRESS, m_btnAllCompress);
	DDX_Control(pDX, IDC_SPLICEDLG_ALLTYPEAPPLY_WEB, m_btnAllMiddle);
	DDX_Control(pDX, IDC_SPLICEDLG_ALLTYPEAPPLY_FLANGE, m_btnToptoBottom);
	DDX_Control(pDX, IDC_SPLICEDLG_ALLTYPEAPPLY, m_btnSameType);
	DDX_Control(pDX, IDC_SPLICEDLG_APPLY_ALL_GIRDERAPPLY, m_btnAllGirder);
	DDX_Control(pDX, IDC_SPLICEDLG_ALLSPLICESAME, m_btnAllSP);
	DDX_Control(pDX, IDC_SPLICEDLG_USERDESIGN, m_btnDesignCheck);
	DDX_Control(pDX, IDC_SPLICEDLG_AUTOCALC, m_btnAutoConnect);
	DDX_Control(pDX, IDC_SPLICEDLG_INPUTFINISH, m_btnAllInputSP);
}


BEGIN_MESSAGE_MAP(CDetFieldJoinDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetFieldJoinDlg)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnDialogDataSaveAndRedraw)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnDialogNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnDialogPrev)
	ON_CBN_SELCHANGE(IDC_SPLICEDLG_BOLTSIZE, OnSelchangeSpliceDlgBoltsize)
	ON_BN_CLICKED(IDC_SPLICEDLG_APPLY_ALL_GIRDERAPPLY, OnSpliceDlgAllgirderapply)
	ON_BN_CLICKED(IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC, OnSpliceDlgBoltlengthAutocalc)
	ON_BN_CLICKED(IDC_SPLICEDLG_INPUTFINISH, OnSpliceDlgInputfinish)
	ON_BN_CLICKED(IDC_SPLICEDLG_ALLSPLICESAME, OnSpliceDlgAllsplicesame)
	ON_BN_CLICKED(IDC_SPLICEDLG_SIMPLYTYPE, OnSpliceDlgSimplytype)
	ON_CBN_SELCHANGE(IDC_SPLICEDLG_CURRENTSPLICE, OnSelchangeSpliceDlgCurrentsplice)
	ON_BN_CLICKED(IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC_ALL, OnSpliceDlgBoltlengthAutocalcAll)
	ON_BN_CLICKED(IDC_SPLICEDLG_ALLTYPEAPPLY_WEB, OnSpliceDlgAlltypeapplyWeb)
	ON_BN_CLICKED(IDC_SPLICEDLG_ALLTYPEAPPLY_STRENGTH, OnSpliceDlgAlltypeapplyStrength)
	ON_BN_CLICKED(IDC_SPLICEDLG_ALLTYPEAPPLY_FLANGE, OnSpliceDlgAlltypeapplyFlange)
	ON_BN_CLICKED(IDC_SPLICEDLG_USERDESIGN, OnSpliceDlgUserdesign)
	ON_BN_CLICKED(IDC_SPLICEDLG_ALLTYPEAPPLY_COMPRESS, OnSpliceDlgAlltypeapplyCompress)
	ON_BN_CLICKED(IDC_CHECK_TOTALVIEW, OnCheckTotalview)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_SPLICEDLG_ALLTYPEAPPLY, OnSameApplySameType)
	ON_BN_CLICKED(IDC_SPLICEDLG_AUTOCALC, OnSplicedlgAutocalc)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRIDN, STMakeCursor)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRIDN, OnCellChangedData)
	ON_MESSAGE(WM_GET_NEARBX, OnClickTotalView)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDetFieldJoinDlg, CGeneralBaseDlg)
    //{{AFX_EVENTSINK_MAP(CDetFieldJoinDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetFieldJoinDlg message handlers
BOOL CDetFieldJoinDlg::OnInitDialog()
{
	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_AUTOCALC_ALLGIRDER,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC1,								SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC2,								SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_PERMISSRATE,							SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC,		SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC_ALL,	SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_SPLICEDLG_ALLTYPEAPPLY_STRENGTH,		SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_SPLICEDLG_ALLTYPEAPPLY_COMPRESS,		SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_SPLICEDLG_ALLTYPEAPPLY_WEB,			SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_SPLICEDLG_ALLTYPEAPPLY_FLANGE,		SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_SPLICEDLG_ALLTYPEAPPLY,				SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_SPLICEDLG_APPLY_ALL_GIRDERAPPLY,		SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_SPLICEDLG_ALLSPLICESAME,				SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_SPLICEDLG_USERDESIGN,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICEDLG_AUTOCALC,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICEDLG_INPUTFINISH,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_GROUP, SZ_MIDDLE_RIGHT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRIDN, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SPLICEDLG_SIMPLYTYPE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_TOTALVIEW,		SZ_TOP_RIGHT, SZ_TOP_RIGHT);


	return TRUE;
}

void CDetFieldJoinDlg::OnPreInitDialog()
{
	SetGridTitle(m_nSp);
	SetInitSpliceCombo();  
	SetInitBoltCombo();	

	CDataManage* pData = m_pStd->GetDataManage();

	if(pData->GetGlobalOption()->GetBoltLengthAuto())
	{
		GetDlgItem(IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPLICEDLG_BOLTLENGTH_AUTOCALC_ALL)->EnableWindow(FALSE);
	}	
}

void CDetFieldJoinDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(m_nG);	
	CPlateBasicIndex	*pBx		= pGir->GetBxBySpliceNoSPType(m_nSp);
	CARoadOptionStd		*pOptStd	= m_pStd->GetDataManage()->GetOptionStd();
	
	if(pBx==NULL)	return;

	if(pBx->GetSplice()->m_bSpliceType==FALSE)
	{
		CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
		return;
	}
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	pDom->SetLTScale(100);
	pDom->SetScaleDim(2);

	CDomyun Dom(pDom);
	CAPlateDrawJong DrawJong(m_pStd->GetDataManage());
	CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
	if(m_nTab==0)
	{
		DrawJong.DrawInputDetSpliceWeb(&Dom, m_nG, m_nSp);
		Dom.SetCalcExtRect();
		CDRect rectDom = Dom.GetExtRect();
		Dom.Move(-rectDom.CenterPoint().x, 0);
		Dom.Move(-rectDom.Width()/2-Dom.Always(10), 0);
		*pDom << Dom;
		
		DrawPyung.DrawInputDetSplicePyung(&Dom, m_nG, m_nSp);
		Dom.SetCalcExtRect();
		rectDom = Dom.GetExtRect();
		Dom.Move(-rectDom.CenterPoint().x, 0);
		Dom.Move(rectDom.Width()/2+Dom.Always(10), 0);
		*pDom << Dom;
	}
	else
	{
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
			CPlateBasicIndex	*pBx	= pGir->GetBxBySpliceNoSPType(m_nSp);
			CPlateSplice		*pSp	= pBx->GetSplice();
			CString	strTitle	= _T("");
			CDRect  BoundRect, BoundWeb;
			Dom.SetScaleDim(80);

			DrawJong.DrawInputDetSpliceWeb(&Dom, nG, m_nSp,FALSE);
			Dom.SetCalcExtRect();
			BoundWeb = Dom.GetExtRect();
			Dom.Move(0, -BoundWeb.CenterPoint().y);
			Dom.Move(3000*nG, 0);
			*pDom << Dom;

			DrawPyung.DrawUpperFlangeSplice(&Dom, pBx, 0, 0, FALSE);
			Dom.SetCalcExtRect();
			BoundRect = Dom.GetExtRect();
			Dom.Move(0, -BoundRect.top);
			Dom.Move(3000*nG, BoundWeb.Height()/2);

			pOptStd->SetEnvType(&Dom,HCAD_SMALL_TITLE);
			Dom.SetCalcExtRect();
			BoundRect = Dom.GetExtRect();			
			Dom.SetTextHeight(2.0);
			strTitle.Format("< G%d SP%d >", nG+1, m_nSp+1);
			Dom.TextOut(BoundRect.CenterPoint().x, BoundRect.bottom+Dom.GetTextHeight(), strTitle);
			*pDom << Dom;			

			DrawPyung.DrawLowerFlangeSplice(&Dom, pBx, 0, 0, FALSE);
			BoundRect = Dom.GetExtRect();
			Dom.Move(0, -BoundRect.bottom);
			Dom.Move(3000*nG, -BoundWeb.Height()/2);
			
			*pDom << Dom;
		}
	}

	if(m_bTotalView)
	{
		m_pStd->GetTotalView()->m_nFlag = BX_SPLICE;

		CDomyun Dom;
		CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
		CDomyun *pDomDlg = m_pStd->GetTotalView()->m_DomView.GetDomyun();
		m_pStd->GetTotalView()->Clear();
		DrawPyung.DrawPyungOnAirialView(&Dom);
		DrawPyung.DrawPyungFieldSpliceMark(&Dom, pBx);
		Dom.Rotate(CDPoint (0,0), pDB->GetAngleBridgeCenter().GetMirrorVert());
		if(m_pStd->GetTotalView()->GetSafeHwnd()) m_pStd->GetTotalView()->m_DomView.RedrawAll();
		*pDomDlg << Dom;

		m_pStd->ShowTotalView(m_pView);
	}		
	else
		m_pStd->HideTotalView();

	STMakeCursor(0,0);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll); 
}

void CDetFieldJoinDlg::SetGridTitle(long nSp)			// 그리드 설정
{
	long	nCol = 0;
	CString	str;

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(SP_ROWS);
	m_Grid.SetColumnCount(SP_COLS);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));

	for(nCol=0; nCol<SP_COLS; nCol++)
	{
		m_Grid.SetCellFixType(FU_ROW, nCol, nCol==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
		m_Grid.SetCellFixType(FL_ROW, nCol, nCol==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
		m_Grid.SetCellFixType(WEB_ROW, nCol, nCol==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
		m_Grid.SetCellFixType(MO_ROW, nCol, nCol==0 ? GVN_ONECELL_COLFIX : GVN_ONECELL_ROWFIX);
	}
	//////////////////////////////////////////////////////////////////
	//	상부현장이음
	nCol = 0;
	m_Grid.SetTextMatrix(FU_ROW, nCol, _T("상부 플랜지"));	m_Grid.SetMergeRow(FU_ROW, FU_ROW+1, nCol++);
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uA"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uB"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uC"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uC1"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uE"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uF"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uT"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uW"));
	m_Grid.SetTextMatrix(FU_ROW, nCol++, _T("uL"));

	//	하부 현장이음 변수
	nCol = 0;
	m_Grid.SetTextMatrix(FL_ROW, nCol, _T("하부 플랜지"));	m_Grid.SetMergeRow(FL_ROW, FL_ROW+1, nCol++);
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dA"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dB"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dC"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dC1"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dE"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dF"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dT"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dW"));
	m_Grid.SetTextMatrix(FL_ROW, nCol++, _T("dL"));

	//	복부판 현장이음 변수
	nCol = 0;
	m_Grid.SetTextMatrix(WEB_ROW, nCol, _T("복부판"));	m_Grid.SetMergeRow(WEB_ROW, WEB_ROW+1, nCol++);
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sA"));// 9EA
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sB"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sC"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sC1"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sE"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sF"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sE1"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sF1"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sT"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sW"));
	m_Grid.SetTextMatrix(WEB_ROW, nCol++, _T("sL"));

	//	모멘트 이음판
	nCol = 0;
	m_Grid.SetTextMatrix(MO_ROW, nCol, _T("모멘트이음판"));	m_Grid.SetMergeRow(MO_ROW, MO_ROW+1, nCol++);
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mA"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mB"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mC"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mC1"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mD")); //11EA
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mG"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mE"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mF"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mW"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mT"));
	m_Grid.SetTextMatrix(MO_ROW, nCol++, _T("mL"));

	m_Grid.SetRedraw(TRUE, TRUE);
}


void CDetFieldJoinDlg::SetDataSave()
{

	SetDataSaveSub(m_nG, m_nSp);

}

void CDetFieldJoinDlg::SetDataSaveSub(long nG, long nSp)
{

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp	   *pGir = pDB->GetGirder(nG);
	if(nSp >= pGir->GetQtySplice(CPlateGirder::SPLICE)) return;
	CPlateBasicIndex		*pBx   = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice			*pSp   = pBx->GetSplice();

	CString	str;
	long	nCol = 1;
	// 상부현장이음
	nCol = 1;
 	pSp->m_uA	=	m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
 	pSp->m_uB	=	m_Grid.GetTextMatrixLong(FU_ROW+1, nCol++);
 	pSp->m_uC	=	m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
 	pSp->m_uC1	=	m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
 	pSp->m_uE	=	(long)m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
 	pSp->m_uF	=	m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
 	pSp->m_uT	=	m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
	pSp->m_uW	=	m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
	pSp->m_uL   =   m_Grid.GetTextMatrixDouble(FU_ROW+1, nCol++);
	// 하부현장이음
	nCol = 1;
 	pSp->m_dA	=	m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);
 	pSp->m_dB	=	m_Grid.GetTextMatrixLong(FL_ROW+1, nCol++);
 	pSp->m_dC	=	m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);
 	pSp->m_dC1	=	m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);
	pSp->m_dE	=	(long)m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);
	pSp->m_dF	=	m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);
	pSp->m_dT	=	m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);
	pSp->m_dW	=	m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);
	pSp->m_dL   =   m_Grid.GetTextMatrixDouble(FL_ROW+1, nCol++);

	/////////////////////////////
	// 복부판현장이음
	nCol = 1;
	pSp->m_sA	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);
	pSp->m_sB	=	m_Grid.GetTextMatrixLong(WEB_ROW+1, nCol++);
	pSp->m_sC	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);
	pSp->m_sC1	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);
	pSp->m_sE	=	m_Grid.GetTextMatrixLong(WEB_ROW+1, nCol++);
	pSp->m_sF	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);
	pSp->m_sE1	=	m_Grid.GetTextMatrixLong(WEB_ROW+1, nCol++);
	pSp->m_sF1	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);
	pSp->m_sT	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);
	pSp->m_sW	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);
	pSp->m_sL	=	m_Grid.GetTextMatrixDouble(WEB_ROW+1, nCol++);

	if(pSp->m_sF == pSp->m_sF1)
	{
		pSp->m_sE += pSp->m_sE1*2;
		pSp->m_sE1 = 0;
		pSp->m_sF1 = 0;
	}
	// 모멘트 이음판
	nCol = 1;
	pSp->m_mA	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mB	=	m_Grid.GetTextMatrixLong(MO_ROW+1, nCol++);
	pSp->m_mC	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mC1	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mD	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mG	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mE	=	m_Grid.GetTextMatrixLong(MO_ROW+1, nCol++);
	pSp->m_mF	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mW	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mT	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	pSp->m_mL	=	m_Grid.GetTextMatrixDouble(MO_ROW+1, nCol++);
	
	pSp->m_ZzsBoltSize = m_ZzsBoltSize;
	pDB->m_cSpliceSimpleType = (BYTE)m_bSpliceSimpleType;

	//
	double dHeightBox = pSp->GetBx()->GetHeightGirderByBx();
	double HGap, Gap;
	BOOL bMoment = TRUE;	// 모멘트이음판이 없으면 FALSE
	if(pSp->m_mW*pSp->m_mE*pSp->m_mF*pSp->m_mB*pSp->m_mC == 0)	bMoment = FALSE;
		
	if(bMoment)
	{
		Gap		= (dHeightBox-(pSp->m_mG+pSp->m_mW+pSp->m_sW+pSp->m_mW+pSp->m_mG))/2;
		HGap	= (pSp->m_sW-pSp->m_sE*pSp->m_sF-pSp->m_sE1*pSp->m_sF1*2)/2 + Gap + pSp->m_mW-(pSp->m_mD-pSp->m_mG+pSp->m_mE*pSp->m_mF);
	}
	else 
		HGap = 0;

	pSp->m_mWidth = HGap;
	UpdateData(TRUE);
}

void CDetFieldJoinDlg::OnCursorChange(long nRow, long nCol)
{
	if(m_nTab==1)	return;
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CDetFieldJoinDlg::SetInitSpliceCombo()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = NULL;
	long nG, cSp, nSp;
	CString strSplice="";

	for(nG = 0 ; nG < pDB->GetGirdersu(); nG++)
	{
		pGir = pDB->GetGirder(nG);
		cSp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		for(nSp = 0 ; nSp < cSp ; nSp++)
		{
			strSplice.Format("G%d SP%d", nG+1, nSp+1);
			m_CurrSplice.AddString(strSplice);
		}
	}
	m_CurrSplice.SetCurSel(0);
	m_nG  = 0;
	m_nSp = 0;
}

void CDetFieldJoinDlg::SetInitBoltCombo()
{
	//	Bolt Size
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_SPLICEDLG_BOLTSIZE);
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(0);
	CPlateBasicIndex	*pBx	= pGir->GetBxBySpliceNoSPType(0);
	CSteelBolt		*pSteelBolt	= pDB->m_pDataManage->GetSteelBolt();

	CString szBoltGrade = pDB->GetMaterial(SP_U_BOLT, pBx);

	for(long nIdx=0; nIdx<pSteelBolt->GetSize(); nIdx++)
	{
		if(pSteelBolt->GetStringData(nIdx, BDIB_GRADE) == szBoltGrade)
			pCombo->AddString(pSteelBolt->GetStringData(nIdx, BDIB_SIZE));
	}

	pCombo->SetCurSel(1);//default:M22
	UpdateData(TRUE);
}

void CDetFieldJoinDlg::SetDlgTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CString str;
	str.Format("%s - 현장 이음 [G%d SP%d] 상세 입력",pDB->m_sNameBridge,m_nG+1, m_nSp+1);
	SetWindowText(str);
	DrawTitleButton();
}

void CDetFieldJoinDlg::SetSpliceCombo()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CString str;
	if(m_nSp >= pDB->GetGirder(m_nG)->GetQtySplice(CPlateGirder::SPLICE))	return;
	str.Format("G%d SP%d", m_nG+1, m_nSp+1);
	long nSp = m_CurrSplice.FindStringExact(0, str);
	m_CurrSplice.SetCurSel(nSp);
	SetDlgTitle();
}

void CDetFieldJoinDlg::SetBoltCombo(long nG, long nSp)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(m_nSp >= pDB->GetGirder(m_nG)->GetQtySplice(CPlateGirder::SPLICE))	return;
	CPlateGirderApp		*pGir  = pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx   = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice		*pSp   = pBx->GetSplice();
	CComboBox		*pCombo = (CComboBox *)GetDlgItem(IDC_SPLICEDLG_BOLTSIZE);
	m_ZzsBoltSize		= pSp->m_ZzsBoltSize;

	long nCombo		= pCombo->FindStringExact(0, m_ZzsBoltSize);
	if(nCombo<0)
	{
		m_ZzsBoltSize	= pSp->m_ZzsBoltSize = "M22";
		nCombo			= pCombo->FindStringExact(0, m_ZzsBoltSize);
	}
	pCombo->SetCurSel(nCombo);
	UpdateData(TRUE);
}
//	bInc :	TRUE	:	증가
//			FALSE	:	감소
void CDetFieldJoinDlg::ChangeSpliceIndex(BOOL bInc)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = NULL;

	if(bInc) {	//증가
		m_nSp++;
		if(m_nSp >= pDB->GetGirder(m_nG)->GetQtySplice(CPlateGirder::SPLICE)) {
			m_nSp = 0;
			if((m_nG+1) < pDB->GetGirdersu()) {
				m_nG++;
				pGir = pDB->GetGirder(m_nG);
			}
		}
	}
	else {	//감소
		m_nSp--;
		if(m_nSp < 0) {
			m_nSp = 0;
			if(m_nG > 0) {
				m_nG--;
				pGir = pDB->GetGirder(m_nG);
			}
		}
	}
	SetDlgTitle();
}



void CDetFieldJoinDlg::OnSelchangeSpliceDlgBoltsize() 
{
	UpdateData(TRUE);
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp	   *pGir = pDB->GetGirder(m_nG);
	if(m_nSp >= pGir->GetQtySplice(CPlateGirder::SPLICE)) return;
	CPlateBasicIndex		*pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
	CPlateSplice			*pSp   = pBx->GetSplice();
	pSp->m_ZzsBoltSize = m_ZzsBoltSize;
}

//[볼트길이 자동계산]
//현재 이음판의 볼트길이를 자동계산합니다..
void CDetFieldJoinDlg::OnSpliceDlgBoltlengthAutocalc() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp	*pGir  = pDB->GetGirder(m_nG);
	CPlateBasicIndex		*pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
	CPlateSplice			*pSp   = pBx->GetSplice();

	SetDataDefaultLengthBolt(m_nG, pSp);
	SetDataInitSubGirder(m_nG, m_nSp);
}

void CDetFieldJoinDlg::OnSpliceDlgInputfinish() 
{
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();

	m_pStd->MoveNextDialog();

}

//	모든 거더 동일적용
//	:	현재 ComboBox에 선택된 Splice(Drawing된) Data로
//		현재 Grid에 Display된 각 거더의 Splice Index Data에 적용
void CDetFieldJoinDlg::OnSpliceDlgAllgirderapply() 
{
	long nG, cSp;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp	*pGir  = pDB->GetGirder(m_nG);
	CPlateBasicIndex		*pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
	CPlateSplice			*pSp   = pBx->GetSplice();
	CPlateSplice *pSpSec;
	SetDataSaveSub(m_nG, m_nSp);
	for(nG = 0 ; nG < pDB->GetGirdersu() ; nG++) {
		pGir  = pDB->GetGirder(nG);
		cSp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= cSp)	continue;
		if(m_nG == nG)	continue;
		pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		pSpSec   = pBx->GetSplice();

		*pSpSec <<= *pSp;
		SetDataInitSubGirder(nG, m_nSp);
		SetDataSaveSub(nG,m_nSp);
	}
	DrawInputDomyunView();
}

// 최초실행시
void CDetFieldJoinDlg::SetDataDefault()
{
	long	nG=0, cSp=0, nSp=0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = NULL;

	for(nG = 0 ; nG < pDB->GetGirdersu(); nG++)
	{
		pGir= pDB->GetGirder(nG);
		cSp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		for(nSp = 0 ; nSp < cSp ; nSp++)
			SetDataDefaultSub(nG, nSp);
	}
}

void CDetFieldJoinDlg::SetDataDefaultSub(long nG, long nSp)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp	*pGir  = pDB->GetGirder(nG);
	CPlateBasicIndex		*pBx   = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice			*pSp   = pBx->GetSplice();

	if(pSp==NULL) return;

	pSp->m_ZzsBoltSize = _T("M22");
	
	SetDataDefaultSplieceUpper(nG,nSp);
	SetDataDefaultSplieceLower(nG,nSp);
	SetDataDefaultSplieceWeb(nG,nSp);
	SetDataDefaultLengthBolt(nG,pSp);
}

void CDetFieldJoinDlg::SetDataDefaultSplieceUpper(long nG, long nSp)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp	 *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx	  = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice	 *pSp	  = pBx->GetSplice();
	CSteelBolt       *pBolt   = m_pStd->GetDataManage()->GetSteelBolt();
	if(pSp==NULL) return;

	CString strSymbol;
	strSymbol.Format("%s %s", pDB->GetMaterial(SP_U_BOLT, pBx), pSp->m_ZzsBoltSize);
	long nBolt  = pBolt->GetIndex(strSymbol);
	pSp->m_uA	= pBolt->GetCalcEdgeDistance(nBolt);//전단면 최소 연단거리...
	pSp->m_uB	= 3;
	pSp->m_uC	= 85;
	pSp->m_uC1	= 100.0;
	pSp->m_uT	= 10.0;

	BOOL	bPre		= pBx->GetFactChain(G_F_U, FALSE)->m_dWidth > pBx->GetFactChain(G_F_U, TRUE)->m_dWidth;
	double	dWebT		= pBx->GetFactChain(G_W, bPre)->m_dFactChainThick;
	double	dFlangUW	= pBx->GetFactChain(G_F_U, bPre)->m_dWidth;
	pSp->m_uW = Round((dFlangUW-dWebT)/2 - 25, 0);

	if(pDB->IsTUGir())
	{
		pSp->m_uE   = 0;
		pSp->m_uF   = 0;
	}
	else
	{
		pSp->m_uE   = (long)Round((pSp->m_uW - pSp->m_uA*2)/75.0, 0);
		pSp->m_uF   = Round((pSp->m_uW - pSp->m_uA*2)/pSp->m_uE, 0);
	}
}

void CDetFieldJoinDlg::SetDataDefaultSplieceLower(long nG, long nSp)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp	 *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx     = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice	 *pSp     = pBx->GetSplice();
	CSteelBolt       *pBolt   = m_pStd->GetDataManage()->GetSteelBolt();
	if(pSp==NULL) return;
	
	CString strSymbol;
	strSymbol.Format("%s %s", pDB->GetMaterial(SP_L_BOLT, pBx), pSp->m_ZzsBoltSize);
	long nBolt  = pBolt->GetIndex(strSymbol);
	pSp->m_dA	= pBolt->GetCalcEdgeDistance(nBolt);
	pSp->m_dB	= 3;
	pSp->m_dC	= 85;
	pSp->m_dC1	= 100.0;
	pSp->m_dT	= 10;

	BOOL	bPre		= pBx->GetFactChain(G_F_L, FALSE)->m_dWidth > pBx->GetFactChain(G_F_L, TRUE)->m_dWidth;
	double	dWebT		= pBx->GetFactChain(G_W, bPre)->m_dFactChainThick;
	double	dFlangLW	= pBx->GetFactChain(G_F_L, bPre)->m_dWidth;

	pSp->m_dW   = Round((dFlangLW-dWebT)/2 - 25, 0);
	pSp->m_dE   = (long)Round((pSp->m_dW - pSp->m_dA*2.0)/75.0, 0);
	pSp->m_dF   = Round((pSp->m_dW-pSp->m_dA*2.0)/pSp->m_dE, 0);
}

void CDetFieldJoinDlg::SetDataDefaultSplieceWeb(long nG, long nSp)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp	 *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx     = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice	 *pSp     = pBx->GetSplice();
	CSteelBolt       *pBolt   = m_pStd->GetDataManage()->GetSteelBolt();

	if(pSp==NULL) return;
	
	double dLengthWeb  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	CString strSymbol;
	strSymbol.Format("%s %s", pDB->GetMaterial(SP_W_BOLT, pBx), pSp->m_ZzsBoltSize);
	long nBolt  = pBolt->GetIndex(strSymbol);
	
	pSp->m_nWebType = SP_WEB_FL;
	pSp->m_mT	= 0;
	pSp->m_mA   = 0;
	pSp->m_mB	= 0;
	pSp->m_mC	= 0;
	pSp->m_mC1	= 0;
	pSp->m_mD	= 0;
	pSp->m_mG	= 0;
	pSp->m_mE	= 0;
	pSp->m_mF	= 0;
	pSp->m_mW	= 0;

	pSp->m_sT	= 10;
	pSp->m_sA   = pBolt->GetCalcEdgeDistance(nBolt);
	pSp->m_sB	=  1;
	pSp->m_sC	= 85;
	pSp->m_sC1	= 100.0;
	pSp->m_sF	= 100;
	pSp->m_sE	= (long)((dLengthWeb - 80)/pSp->m_sF-1);
	pSp->m_sF1	= 0;
	pSp->m_sE1	= 0;
	if(pSp->m_sE<0)	pSp->m_sE = 1;

	pSp->m_sW	= pSp->m_sE*pSp->m_sF + pSp->m_sE1*pSp->m_sF1*2 +pSp->m_sA*2;
}

void CDetFieldJoinDlg::SetDataDefaultLengthBolt(long nG, CPlateSplice *pSp) const
{
	CPlateBasicIndex* pBx = pSp->GetBx();	

	double Tu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	double Tl	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double WebT	= pBx->GetFactChain(G_W)->m_dFactChainThick;

	CSteelBolt *pBolt = m_pStd->GetDataManage()->GetSteelBolt();

	pSp->m_uL  = pBolt->GetBoltLength(pBx, SP_U_BOLT, pSp->m_ZzsBoltSize, Tu + pSp->m_uT*2);
	pSp->m_dL  = pBolt->GetBoltLength(pBx, SP_L_BOLT, pSp->m_ZzsBoltSize, Tl + pSp->m_dT*2);
	pSp->m_mL  = pBolt->GetBoltLength(pBx, SP_W_BOLT, pSp->m_ZzsBoltSize, WebT + pSp->m_mT*2);
	pSp->m_sL  = pBolt->GetBoltLength(pBx, SP_W_BOLT, pSp->m_ZzsBoltSize, WebT + pSp->m_sT*2);
}

void CDetFieldJoinDlg::SetDataInit()
{
	SetSpliceCombo();
	SetDataInitSub(m_nSp);
	SetBoltCombo(m_nG, m_nSp);
	UpdateData(FALSE);	
}

void CDetFieldJoinDlg::SetDataInitSub(long nSp)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	m_bSpliceSimpleType = (BOOL)pDB->m_cSpliceSimpleType;
	SetDataInitSubGirder(m_nG, m_nSp);
}


void CDetFieldJoinDlg::SetDataInitSubGirder(long nG, long nSp)
{
	CPlateBridgeApp		*pDB			= m_pStd->GetBridge();
	CPlateGirderApp		*pGir			= pDB->GetGirder(nG);
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	CPlateBasicIndex	*pBx   = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice		*pSp   = pBx->GetSplice();
	
	if(nSp >= pGir->GetQtySplice(CPlateGirder::SPLICE)) 
	{
		for(long i=1; i<m_Grid.GetColumnCount(); i++)
			m_Grid.SetTextMatrix(nG+1,i,_T("0"));
		m_Grid.SetRedraw(TRUE,TRUE);
		return;
	}

	if(pSp->m_bSpliceType==FALSE)	return;

	BOOL bBoltLengthAuto = pDataManage->GetGlobalOption()->GetBoltLengthAuto();
	long	nCol = 1;
	CString	 str = "";
	// 상부현장이음
	nCol = 1;
	str.Format("%g", pSp->m_uA);		m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	str.Format("%d", (long)pSp->m_uB);	m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_uC);		m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_uC1);		m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	str.Format("%d", pSp->m_uE);		m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_uF);		m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_uT);		m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_uW);		m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);
	if(bBoltLengthAuto)	//볼트 길이를 자동사용 할 경우(입력 사용 안 할 경우)
	{
		m_Grid.SetItemState(FU_ROW+1, nCol, m_Grid.GetItemState(MO_ROW+1, nCol) | GVIS_READONLY);
		m_Grid.SetItemBkColour(FU_ROW+1, nCol, RGBREADONLY);
	}	
	str.Format("%g", pSp->GetuL(pBx->GetFactChain(G_F_U)->m_dFactChainThick));
	m_Grid.SetTextMatrix(FU_ROW+1, nCol++, str);	

	// 하부현장이음
	nCol = 1;
	str.Format("%g", pSp->m_dA);		m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	str.Format("%d", (long)pSp->m_dB);	m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_dC);		m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_dC1);		m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	str.Format("%d", pSp->m_dE);		m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_dF);		m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_dT);		m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_dW);		m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	if(bBoltLengthAuto)	//볼트 길이를 자동사용 할 경우(입력 사용 안 할 경우)
	{
		m_Grid.SetItemState(FL_ROW+1, nCol, m_Grid.GetItemState(MO_ROW+1, nCol) | GVIS_READONLY);
		m_Grid.SetItemBkColour(FL_ROW+1, nCol, RGBREADONLY);
	}
	str.Format("%g", pSp->GetdL(pBx->GetFactChain(G_F_L)->m_dFactChainThick));
	m_Grid.SetTextMatrix(FL_ROW+1, nCol++, str);
	// 복부판현장이음
	nCol = 1;
	str.Format("%g", pSp->m_sA);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%d", pSp->m_sB);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_sC);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_sC1);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%d", pSp->m_sE);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_sF);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%d", pSp->m_sE1);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_sF1);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_sT);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_sW);		m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);

	if(pSp->m_nWebType == SP_WEB_FLMO)	//연결형일 경우 sW의 Cell를 읽기전용으로 변경
	{
		m_Grid.SetItemState(WEB_ROW+1, nCol-1, GVIS_READONLY);
		m_Grid.SetItemBkColour(WEB_ROW+1, nCol-1, RGBREADONLY);
	}
	else
	{
		m_Grid.SetItemState(WEB_ROW+1, nCol-1, GVIS_MODIFIED);
	}

	str.Format("%g", pSp->GetsL(pBx->GetFactChain(G_W)->m_dFactChainThick));
	m_Grid.SetTextMatrix(WEB_ROW+1, nCol++, str);

	// 모멘트 이음판
	nCol = 1;
	str.Format("%g", pSp->m_mA);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%d", pSp->m_mB);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_mC);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_mC1);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_mD);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_mG);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%d", pSp->m_mE);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_mF);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_mW);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	str.Format("%g", pSp->m_mT);		m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	if(bBoltLengthAuto)	//볼트 길이를 자동사용 할 경우(입력 사용 안 할 경우)
	{
		m_Grid.SetItemState(MO_ROW+1, nCol, m_Grid.GetItemState(MO_ROW+1, nCol) | GVIS_READONLY);
		m_Grid.SetItemBkColour(MO_ROW+1, nCol, RGBREADONLY);
	}
	str.Format("%g", pSp->GetmL(pBx->GetFactChain(G_W)->m_dFactChainThick));
	m_Grid.SetTextMatrix(MO_ROW+1, nCol++, str);
	//str.Format("%g", pSp->m_mL);		m_Grid.SetTextMatrix(nRow, nCol++, str);

	m_ZzsBoltSize = pSp->m_ZzsBoltSize;
	ResetGrid();
	m_Grid.SetRedraw(TRUE,TRUE);
}
///////////////////////////////////////////////////////////////////////////////
BOOL CDetFieldJoinDlg::ContinueMsg()
{
	int ret;
	ret = MessageBox(" 기존의 데이타가 수정됩니다.\n\n 계속 하시겠습니까? ",
					 " 현장이음 상세", MB_YESNO);
	if(ret == IDYES) return TRUE;
	return FALSE;
}

void CDetFieldJoinDlg::OnSpliceDlgAllsplicesame()
// 모든 SP 동일적용
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if( ContinueMsg() == FALSE ) return;
	CPlateGirderApp *pGir = NULL;
	CPlateBasicIndex *pBx = NULL;
	CPlateBasicIndex *pBxSec = NULL;
	CPlateSplice *pSp = NULL,*pSpSec = NULL;
	CString str(_T("")),str1(_T("")),str2(_T(""));
	long cSp = 0;
	for(long nG = 0; nG < pDB->GetGirdersu();nG++)
	{
		pGir  = pDB->GetGirder(nG);
		cSp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= cSp)	continue;
		pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		pSp   = pBx->GetSplice();
		if( pSp == NULL ) continue;

		for(long nSp = 0; nSp < cSp; nSp++)
		{
			pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			pSpSec   = pBxSec->GetSplice();
			if( pSpSec == NULL ) continue;

			*pSpSec <<= *pSp;
		}
	}
	DrawInputDomyunView();
}

void CDetFieldJoinDlg::OnSpliceDlgSimplytype() 
// 심플 타입 Check Button
{
	m_bSpliceSimpleType = !m_bSpliceSimpleType;
	UpdateData(FALSE);
}

void CDetFieldJoinDlg::OnDialogNext() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	SetDataSaveSub(m_nG, m_nSp);
	long Index  = m_CurrSplice.GetCurSel()+1;

	if(m_nSp == 0)
	{
		CPlateGirderApp	*pGirCR  = pDB->GetGirder(-1);
		CPlateGirderApp	*pGir  = pDB->GetGirder(0);
		CPlateBasicIndex	*pBxPre = NULL;

		for(long nSp = 0; nSp < pGirCR->GetQtySplice(CPlateGirder::SPLICE); nSp++)
		{
			CPlateBasicIndex	* pBxCR	= pGirCR->GetBxBySpliceNoSPType(nSp);
			CPlateBasicIndex	* pBx   = pGir->GetBxBySpliceNoSPType(nSp);
			if(pBx)	pBxPre = pBx;

			CPlateSplice *pSpCR = pBxCR->GetSplice();
			CPlateSplice *pSp   = pBxPre->GetSplice();

			if(pBxPre->IsState(BX_SPLICE))
			*pSpCR <<= *pSp;

		}
	}
	if((m_nSp+1) >= pDB->GetGirder(m_nG)->GetQtySplice(CPlateGirder::SPLICE)) 
	{
		m_pStd->MoveNextDialog();	

		return;
	}
	ChangeSpliceIndex(TRUE);	// TRUE : 증가
	m_CurrSplice.SetCurSel(Index);
	SetGridTitle(m_nSp);
	SetDataInitSub(m_nSp);
	SetBoltCombo(m_nG, m_nSp);
	DrawInputDomyunView(TRUE);
}

void CDetFieldJoinDlg::OnDialogPrev() 
{
	SetDataSaveSub(m_nG, m_nSp);
	long Index  = m_CurrSplice.GetCurSel()-1;

	if(m_nSp <= 0) {
		m_pStd->MovePrevDialog();
		return;
	}
	ChangeSpliceIndex(FALSE);	// FALSE : 감소
	m_CurrSplice.SetCurSel(Index);
	SetGridTitle(m_nSp);
	SetDataInitSub(m_nSp);
	SetBoltCombo(m_nG, m_nSp);
	DrawInputDomyunView(TRUE);
}
void CDetFieldJoinDlg::OnDialogDataSaveAndRedraw() 
{
	SetDataSaveSub(m_nG, m_nSp);
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CDetFieldJoinDlg::STMakeCursor(NMHDR* nmgv, LRESULT*)
{
	if(m_nTab==1)	return;
	CDomyun *pDom = m_pView->GetDomyun();
	CString str;	
	long nCol = m_Grid.GetFocusCell().col;
	long nRow = m_Grid.GetFocusCell().row;

	if (nRow%2 == 1) nRow--;//선택된 셀의 제목을 찾아햐 하므로... 070125...KB...
	str = m_Grid.GetItemText(nRow,nCol);

	if( str == "uB" || str == "uC" ) str = "uB@uC";
	if( str == "uE" || str == "uF" ) str = "uE@uF";
	if( str == "dB" || str == "dC" ) str = "dB@dC";
	if( str == "dE" || str == "dF" ) str = "dE@dF";

	if( str == "sB" || str == "sC" ) str = "sB@sC";
	if( str == "mB" || str == "mC" ) str = "mB@mC";
	if( str == "sE" || str == "sF" ) str = "sE@sF";
	if( str == "sE1"|| str == "sF1") str = "sE1@sF1";
	if( str == "mE" || str == "mF" ) str = "mE@mF";
	pDom->STMakeCursor(str);

	return;
}

void CDetFieldJoinDlg::OnSelchangeSpliceDlgCurrentsplice() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long su  = 0,oldsu = 0;
	long pos = m_CurrSplice.GetCurSel();
	long nG = 0;
	for(nG = 0;nG < pDB->GetGirdersu()-1;nG++)
	{
		su += pDB->GetGirder(nG)->GetQtySplice(CPlateGirder::SPLICE);
		if( oldsu <= pos && pos < su ) break;
		oldsu = su;
	}
	m_nG  = nG;
	m_nSp = pos - oldsu;

	SetDlgTitle();
	m_CurrSplice.SetCurSel(pos);
	SetGridTitle(m_nSp);
	SetDataInitSub(m_nSp);
	SetDataSaveSub(m_nG, m_nSp);
	SetBoltCombo(m_nG, m_nSp);
	m_Grid.SetFocusCell(m_Grid.GetFocusCell().row, m_Grid.GetFocusCell().col);
	SetDlgTitle();

	DrawInputDomyunView(TRUE);
}

void CDetFieldJoinDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp		*pDB      = m_pStd->GetBridge();
	CPlateGirderApp		*pGir     = pDB->GetGirder(m_nG);
	CPlateBasicIndex	*pBx      = pGir->GetBxBySpliceNoSPType(m_nSp);
	CPlateSplice		*pSp      = pBx->GetSplice();
	double				dHeight	  = pBx->GetHeightGirderByBx();
	double				giulWeb   = pGir->GetGiulWebSplice(pBx,TRUE);
	
	dHeight = dHeight/cos(atan(giulWeb));
	
	SetDataSaveSub(m_nG, m_nSp);
	if( pSp->m_nWebType == SP_WEB_FLMO)
	{
		pSp->m_mA   = pSp->m_sA;
		pSp->m_mT   = pSp->m_sT;
		pSp->m_mC1  = pSp->m_sC1;
		
		pSp->m_mW	= (pSp->m_mA*2 + pSp->m_mE*pSp->m_mF);
		pSp->m_sW	= pSp->m_sE*pSp->m_sF+pSp->m_sE1*pSp->m_sF1*2+(pSp->m_sF-pSp->m_sA)*2;
		pSp->m_mG	= (dHeight - (pSp->m_sW+pSp->m_mW*2))/2;
		pSp->m_mD	= pSp->m_mG+pSp->m_mA;
		
	}
	SetDataInitSub(m_nSp);
	DrawInputDomyunView(TRUE);
}

//[모든 SP 볼트길이 자동계산]
//모든 이음판의 볼트길이를 자동계산합니다.
void CDetFieldJoinDlg::OnSpliceDlgBoltlengthAutocalcAll() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CString	str = _T("모든 이음판(상판, 하판, 복부판)의 볼트길이를 자동계산합니다.\n계속 하시겠습니까?");
	if(::MessageBox(this->m_hWnd, str, "모든 현장이음 볼트길이 자동계산", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	
	for(long nG = 0 ; nG < pDB->GetGirdersu() ; nG++)  //상부가 압축부인지를 점검
	{
		CPlateGirderApp	*pGir  = pDB->GetGirder(nG);
		long nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		for(long nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			CPlateBasicIndex *pBx = pGir->GetBxBySpliceNoSPType(nSp);
			CPlateSplice*     pSp = pBx->GetSplice();
			SetDataDefaultLengthBolt(nG, pSp);
		}
	}
	SetDataInitSub(m_nSp);	
}
//[모든 복부판 동일적용]
//현재 이음판 번호의 복부판 제원(전단, 모멘트 이음판)으로 다른 모든 복부판 제원을 적용합니다
void CDetFieldJoinDlg::OnSpliceDlgAlltypeapplyWeb() 
{
	CString	str = _T("현재 이음판 번호의 복부판 제원(전단, 모멘트 이음판)으로 다른 모든 복부판 제원을 적용합니다.\n계속 하시겠습니까?");
	if(::MessageBox(this->m_hWnd, str, "모든 복부판 동일적용", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	for(long nG = 0 ; nG < pDB->GetGirdersu() ; nG++)  //상부가 압축부인지를 점검
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		long nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= nQtySp)	continue;
		CPlateBasicIndex *pBx = pGir->GetBxBySpliceNoSPType(m_nSp);
		for(long nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			if(m_nSp == nSp || m_nSp >= nQtySp)	continue;
			CPlateBasicIndex *pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			DataSaveWebToWeb(pBx, pBxSec);	//상판 데이터로 상판 데이터를 저장
		}
	}
	DrawInputDomyunView();
}
//<summary>
//현장이음 복부판(전단, 모멘트) 제원으로 복부판을 저장
//</summary>
//<param name="*pBx"></param>
//<param name="*pBxSec"></param>
void CDetFieldJoinDlg::DataSaveWebToWeb(CPlateBasicIndex *pBx, CPlateBasicIndex* pBxSec) 
{
	CPlateSplice *pSp    = pBx->GetSplice();
	CPlateSplice *pSpSec = pBxSec->GetSplice();

	// 복부판현장이음 좌측판
	pSpSec->m_sA	=	pSp->m_sA	;
	pSpSec->m_sB	=	pSp->m_sB	;
	pSpSec->m_sC	=	pSp->m_sC	;
	pSpSec->m_sC1	=	pSp->m_sC1	;
	pSpSec->m_sE	=	pSp->m_sE	;
	pSpSec->m_sF	=	pSp->m_sF	;
	pSpSec->m_sE1	=	pSp->m_sE1	;
	pSpSec->m_sF1	=	pSp->m_sF1	;
	pSpSec->m_sW	=	pSp->m_sW	;
	pSpSec->m_sT	=	pSp->m_sT	;
	pSpSec->m_sL	=	pSp->m_sL	;
	////
	pSpSec->m_mA	=	pSp->m_mA	;
	pSpSec->m_mB	=	pSp->m_mB	;
	pSpSec->m_mC	=	pSp->m_mC	;
	pSpSec->m_mC1	=	pSp->m_mC1	;
	pSpSec->m_mD	=	pSp->m_mD	;
	pSpSec->m_mG	=	pSp->m_mG	;
	pSpSec->m_mE	=	pSp->m_mE	;
	pSpSec->m_mF	=	pSp->m_mF	;
	pSpSec->m_mW	=	pSp->m_mW	;
	pSpSec->m_mT	=	pSp->m_mT	;
	pSpSec->m_mL	=	pSp->m_mL	;
}

//[모든 인장부 동일적용]
//현재 SP의 인장부(상판 또는 하판)으로 모든 인장부를 동일하게 적용함. - 상.하 구분없이 인장, 압축으로만 구분
//교번 구간일 경우 아무런 작동 없음.
void CDetFieldJoinDlg::OnSpliceDlgAlltypeapplyStrength() 
{
	CString	str = _T("현재 이음판 번호의 인장부(상판 또는 하판) 제원으로 다른 모든 인장부 제원을 적용합니다.\n계속 하시겠습니까?");
	if(::MessageBox(this->m_hWnd, str, "모든 인장부 동일적용", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nG = 0;
	for(nG = 0 ; nG < pDB->GetGirdersu() ; nG++) //상부가 인장부인지를 점검
	{
		CPlateGirderApp  *pGir  = pDB->GetGirder(nG);
		long nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= nQtySp)	continue;
		CPlateBasicIndex *pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		if(pBx->IsCompressByJRib(TRUE))	break;		//TRUE = 압축, FALSE = 인장
		for(long nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			if(m_nSp == nSp || m_nSp >= nQtySp)	continue;
			CPlateBasicIndex *pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			if(!pBxSec->IsCompressByJRib(TRUE)) //상부 종리브가 인장부
			{
				DataSaveUpperToUpper(pBx, pBxSec);	//상판 데이터로 상판 데이터를 저장
			}
			if(!pBxSec->IsCompressByJRib(FALSE)) //하부 종리브가 인장부
			{
				DataSaveUpperToLower(pBx, pBxSec);	//상판 데이터로 하판 데이터를 저장
			}
		}
	}
	for(nG = 0 ; nG < pDB->GetGirdersu() ; nG++)  //하부가 인장부인지를 점검
	{
		CPlateGirderApp *pGir  = pDB->GetGirder(nG);
		long nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= nQtySp)	continue;
		CPlateBasicIndex *pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		if(pBx->IsCompressByJRib(FALSE))	break;		//TRUE = 압축, FALSE = 인장
		for(long nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			if(m_nSp == nSp || m_nSp >= nQtySp)	continue;
			CPlateBasicIndex *pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			if(!pBxSec->IsCompressByJRib(TRUE)) //상부 종리브가 인장부
			{
				DataSaveLowerToUpper(pBx, pBxSec);	//상판 데이터로 상판 데이터를 저장
			}
			if(!pBxSec->IsCompressByJRib(FALSE)) //하부 종리브가 인장부
			{
				DataSaveLowerToLower(pBx, pBxSec);	//상판 데이터로 하판 데이터를 저장
			}
		}
	}
	DrawInputDomyunView();
}

//[모든 압축부 동일적용]
//현재 SP의 압축부(상판 또는 하판)으로 모든 압축부 동일하게 적용함. - 상.하 구분없이 인장, 압축으로만 구분
//교번 구간일 경우 상판제원으로 상하판 모두 적용
void CDetFieldJoinDlg::OnSpliceDlgAlltypeapplyCompress() 
{
	CString	str = _T("현재 이음판 번호의 압축부(상판 또는 하판) 제원으로 다른 모든 압축부 제원을 적용합니다.\n계속 하시겠습니까?");
	if(::MessageBox(this->m_hWnd, str, "모든 압축부 동일적용", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nG = 0;
	for(nG = 0 ; nG < pDB->GetGirdersu() ; nG++)  //상부가 압축부인지를 점검
	{
		CPlateGirderApp *pGir  = pDB->GetGirder(nG);
		long nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= nQtySp)	continue;
		CPlateBasicIndex *pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		if(!pBx->IsCompressByJRib(TRUE))	break;		//TRUE = 압축, FALSE = 인장
		for(long nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			if(m_nSp == nSp || m_nSp >= nQtySp)	continue;
			CPlateBasicIndex *pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			if(pBxSec->IsCompressByJRib(TRUE)) //상부 종리브가 압축부
			{
				DataSaveUpperToUpper(pBx, pBxSec);	//상판 데이터로 상판 데이터를 저장
			}
			if(pBxSec->IsCompressByJRib(FALSE)) //하부 종리브가 압축부
			{
				DataSaveUpperToLower(pBx, pBxSec);	//상판 데이터로 하판 데이터를 저장
			}
		}
	}

	for(nG = 0 ; nG < pDB->GetGirdersu() ; nG++)  //하부가 압축부인지를 점검
	{
		CPlateGirderApp *pGir  = pDB->GetGirder(nG);
		long nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= nQtySp)	continue;
		CPlateBasicIndex *pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		if(!pBx->IsCompressByJRib(FALSE))	break;		//TRUE = 압축, FALSE = 인장
		for(long nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			if(m_nSp == nSp || m_nSp >= nQtySp)	continue;
			CPlateBasicIndex *pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			if(pBxSec->IsCompressByJRib(TRUE)) //상부 종리브가 압축부
			{
				DataSaveLowerToUpper(pBx, pBxSec);	//상판 데이터로 상판 데이터를 저장
			}
			if(pBxSec->IsCompressByJRib(FALSE)) //하부 종리브가 압축부
			{
				DataSaveLowerToLower(pBx, pBxSec);	//상판 데이터로 하판 데이터를 저장
			}
		}
	}
	DrawInputDomyunView();
}

//[상판으로 하판 동일적용]
//현재 이음판의 상판 제원으로 하판을 동일하게 적용합니다.
void CDetFieldJoinDlg::OnSpliceDlgAlltypeapplyFlange() 
{
	CString	str = _T("현재 이음판의 상판 제원으로 하판을 동일하게 적용합니다.\n계속 하시겠습니까?");
	if(::MessageBox(this->m_hWnd, str, "상판으로 하판 동일적용", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir  = pDB->GetGirder(m_nG);
	CPlateBasicIndex *pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
	DataSaveUpperToLower(pBx, pBx);
	SetDataInitSub(m_nSp);

	DrawInputDomyunView(FALSE);
}

void CDetFieldJoinDlg::OnSpliceDlgUserdesign() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();	
	
	if(!pData->m_bReadFrameModelingOutput)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}
	CSpliceDesignResult Dlg(m_pStd, m_nG, m_nSp);
	Dlg.SetSplicePoint(m_nG, m_nSp);
	Dlg.DoModal();
}

//<summary>
//현장이음 상부이음판 제원으로 상부이음판을 저장
//</summary>
//<param name="*pBx"></param>
//<param name="*pBxSec"></param>
void CDetFieldJoinDlg::DataSaveUpperToUpper(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec) 
{
	CPlateSplice *pSp    = pBx->GetSplice();
	CPlateSplice *pSpSec = pBxSec->GetSplice();

 	pSpSec->m_uA	=	pSp->m_uA	;
 	pSpSec->m_uB	=	pSp->m_uB	;
 	pSpSec->m_uC	=	pSp->m_uC	;
 	pSpSec->m_uC1	=	pSp->m_uC1	;
 	pSpSec->m_uE	=	pSp->m_uE	;
 	pSpSec->m_uF	=	pSp->m_uF	;
 	pSpSec->m_uT	=	pSp->m_uT	;
 	pSpSec->m_uL	=	pSp->m_uL	;
 	pSpSec->m_uW	=	pSp->m_uW	;

}

//<summary>
//현장이음 상부이음판 제원으로 하부이음판을 저장
//</summary>
//<param name="*pBx"></param>
//<param name="*pBxSec"></param>
void CDetFieldJoinDlg::DataSaveUpperToLower(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec) 
{
	CPlateSplice	*pSp   = pBx->GetSplice();
	CPlateSplice	*pSpSec= pBxSec->GetSplice();

 	pSpSec->m_dA	=	pSp->m_uA	;
 	pSpSec->m_dB	=	pSp->m_uB	;
 	pSpSec->m_dC	=	pSp->m_uC	;
 	pSpSec->m_dC1	=	pSp->m_uC1	;
 	pSpSec->m_dE	=	pSp->m_uE	;
 	pSpSec->m_dF	=	pSp->m_uF	;
 	pSpSec->m_dT	=	pSp->m_uT	;
 	pSpSec->m_dL	=	pSp->m_uL	;
 	pSpSec->m_dW	=	pSp->m_uW	;

}

//<summary>
//현장이음 하부이음판 제원으로 상부이음판을 저장
//</summary>
//<param name="*pBx"></param>
//<param name="*pBxSec"></param>
void CDetFieldJoinDlg::DataSaveLowerToUpper(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec) 
{
	CPlateSplice	*pSp   = pBx->GetSplice();
	CPlateSplice	*pSpSec= pBxSec->GetSplice();

 	pSpSec->m_uA	=	pSp->m_dA	;
 	pSpSec->m_uB	=	pSp->m_dB	;
 	pSpSec->m_uC	=	pSp->m_dC	;
 	pSpSec->m_uC1	=	pSp->m_dC1	;
	pSpSec->m_uE	=	pSp->m_dE	;
	pSpSec->m_uF	=	pSp->m_dF	;
 	pSpSec->m_uT	=	pSp->m_dT	;
 	pSpSec->m_uL	=	pSp->m_dL	;
 	pSpSec->m_uW	=	pSp->m_dW	;
}
//<summary>
//현장이음 하부이음판 제원으로 하부이음판을 저장
//</summary>
//<param name="*pBx"></param>
//<param name="*pBxSec"></param>
void CDetFieldJoinDlg::DataSaveLowerToLower(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec) 
{
	CPlateSplice	*pSp   = pBx->GetSplice();
	CPlateSplice	*pSpSec= pBxSec->GetSplice();

 	pSpSec->m_dA	=	pSp->m_dA	;
 	pSpSec->m_dB	=	pSp->m_dB	;
 	pSpSec->m_dC	=	pSp->m_dC	;
 	pSpSec->m_dC1	=	pSp->m_dC1	;
 	pSpSec->m_dE	=	pSp->m_dE	;
 	pSpSec->m_dF	=	pSp->m_dF	;
 	pSpSec->m_dT	=	pSp->m_dT	;
 	pSpSec->m_dL	=	pSp->m_dL	;
 	pSpSec->m_dW	=	pSp->m_dW	;
}

void CDetFieldJoinDlg::OnCheckTotalview() 
{
	UpdateData();
	CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_TOTALVIEW);
	CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
	BOOL bTotalView = (pButton) ? pButton->GetCheck() : FALSE;
	if(bTotalView)
	{
		m_pStd->GetTotalView()->m_nFlag = BX_SPLICE;
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

void CDetFieldJoinDlg::OnButtonRecommend() 
{
	CSpliceBoltPositionDlg Dlg(m_pStd, m_nG, m_nSp, this);
	Dlg.DoModal();

	SetSpliceCombo();
	SetBoltCombo(m_nG, m_nSp);
	ResetGrid();
	DrawInputDomyunView(TRUE);	
}

void CDetFieldJoinDlg::ResetGrid()
{
	CPlateBridgeApp		*pDB    = m_pStd->GetBridge();
	CPlateGirderApp		*pGir   = pDB->GetGirder(m_nG);
	CPlateBasicIndex	*pBx    = pGir->GetBxBySpliceNoSPType(m_nSp);
	CPlateSplice		*pSp    = pBx->GetSplice();

	switch(pSp->m_nWebType)
	{
	case SP_WEB_FL: //통판형
		{
			for(long n=1;n < m_Grid.GetColumnCount();n++)
				m_Grid.SetItemState(7,n,GVIS_READONLY);
			m_Grid.SetItemState(5,7,GVIS_MODIFIED);
			m_Grid.SetItemState(5,8,GVIS_MODIFIED);
		}
		break;
	case SP_WEB_MO: //분리형
		{
			for(long n=1;n < m_Grid.GetColumnCount();n++)
				m_Grid.SetItemState(7,n,GVIS_MODIFIED);
			m_Grid.SetItemState(7,11,GVIS_READONLY); //m_mL			
		}
		break;
	case SP_WEB_FLMO: //연결형
		{
			for(long n=1;n < m_Grid.GetColumnCount();n++)
				m_Grid.SetItemState(7,n,GVIS_MODIFIED);

			m_Grid.SetItemState(5,7, GVIS_READONLY);
			m_Grid.SetItemState(5,8, GVIS_READONLY);
			m_Grid.SetItemState(5,10,GVIS_READONLY); //m_sW
			m_Grid.SetItemState(7,1,GVIS_READONLY);  //m_mA
			m_Grid.SetItemState(7,4,GVIS_READONLY);  //m_mC
			m_Grid.SetItemState(7,5,GVIS_READONLY);  //m_mD
			m_Grid.SetItemState(7,6,GVIS_READONLY);  //m_mG
			m_Grid.SetItemState(7,9,GVIS_READONLY);  //m_mW
			m_Grid.SetItemState(7,10,GVIS_READONLY); //m_mT
			m_Grid.SetItemState(7,11,GVIS_READONLY); //m_mL
		}
	}
	m_Grid.SetRedraw();
}

//[동일타입 동일적용]
//응력구간	심플타입 일 경우 이것 한가지만 비교
//박스 높이	10mm 까지는 동일타입으로 설정
//상판두께	
//하판두께	
//복부판두께	
void CDetFieldJoinDlg::OnSameApplySameType() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nG = 0, nSp = 0, nQtySp = 0;
	long nCount = 0;
	CString	str = _T(""), strGirder = _T(""), strSplice = _T("");
	CPlateGirderApp *pGir;
	CPlateSplice *pSp, *pSpSec;
	CPlateBasicIndex *pBx, *pBxSec;

	CString strComp = "동일타입 동일적용";	//m_bSpliceSimpleType ? "심플타입 적용" : "동일타입 동일적용";

	for(nG = 0 ; nG < pDB->GetGirdersu() ; nG++) 
	{
		pGir  = pDB->GetGirder(nG);
		nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= nQtySp)	continue;
		pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		pSp   = pBx->GetSplice();
		strGirder.Format("G%d 동일타입: ", nG+1+pDB->GetNumberStartGirder());
		nCount = 0;
		for(nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			if(m_nSp == nSp || m_nSp >= nQtySp)	continue;
			pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			pSpSec   = pBxSec->GetSplice();
			if(pGir->IsSameTypeBySplice(pBx, pBxSec, m_bSpliceSimpleType))
			{
				strSplice.Format("SP%d ", nSp+1+pDB->GetNumberStartSplice());
				strGirder += strSplice;
				nCount++;
			}
		}
		if(nCount) 
		{
			str += strGirder;
			str += _T("\n");
		}			
	}
	if(str == _T("")) 
	{
		::MessageBox(this->m_hWnd, "같은 타입이 하나도 없습니다!", strComp, MB_OK);
		return;
	}

	str += _T("에 입력되어 있는 데이터가 지워집니다.\n계속 하시겠습니까?");
	if(::MessageBox(this->m_hWnd, str, strComp, MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	///////////////////////////////////////////////////////////////////////////
	nCount = 0;
	str = strGirder = strSplice = _T("");
	for(nG = 0 ; nG < pDB->GetGirdersu() ; nG++) 
	{
		pGir  = pDB->GetGirder(nG);
		nQtySp = pGir->GetQtySplice(CPlateGirder::SPLICE);
		if(m_nSp >= nQtySp)	continue;
		pBx   = pGir->GetBxBySpliceNoSPType(m_nSp);
		pSp   = pBx->GetSplice();

		strGirder.Format("G%d 동일타입: ", nG+1+pDB->GetNumberStartGirder());
		nCount = 0;
		for(nSp = 0 ; nSp < nQtySp ; nSp++) 
		{
			if(m_nSp == nSp)	continue;
			pBxSec   = pGir->GetBxBySpliceNoSPType(nSp);
			pSpSec   = pBxSec->GetSplice();
			if (pGir->IsSameTypeBySplice(pBx,pBxSec,m_bSpliceSimpleType) )
			{
				*pSpSec <<= *pSp;   // Full Copy (06.13. CHO)
				strSplice.Format("SP%d ", nSp+1+pDB->GetNumberStartSplice());
				strGirder += strSplice;
				nCount++;
			}
		}
		if(nCount) 
		{
			str += strGirder;
			str += _T("\n");
		}
	}
	if(str != _T("")) 
	{
		str += _T("에 적용이 완료 되었습니다.");
		::MessageBox(this->m_hWnd, str, strComp, MB_OK);
	}
	DrawInputDomyunView();	
}

long CDetFieldJoinDlg::OnClickTotalView(WPARAM wp, LPARAM lp)
{
	CPlateBasicIndex *pBx  = (CPlateBasicIndex *)wp;
	CPlateGirderApp  *pGir = pBx->GetGirder();
	SetDataSave();

	CPlateSplice	*pCurSp	= pBx->GetSplice();
	if(!pCurSp->m_bSpliceType)
		return 0;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxSp = Finder.GetBxFirst(BX_SPLICE);
	long nSp = 0;
	m_nG	= pBx->GetNumberGirder();
	while(pBxSp)
	{
		CPlateSplice *pSp	= pBxSp->GetSplice();
		if(pSp->m_bSpliceType)
		{
			if(pBx==pBxSp)
			{
				m_nSp = nSp;
				break;
			}
			nSp++;
		}

		pBxSp = Finder.GetBxNext();
	}
	SetDataInit();
//	m_Grid.ResetSelection();
//	m_Grid.SetFocusCell(pBx->GetNumberGirder()+1, 1);
	DrawInputDomyunView(TRUE);
	
	return 0;
}

BOOL CDetFieldJoinDlg::IsValid()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	while (pBx)
	{
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)
			return TRUE;

		pBx = Finder.GetBxNext();
	}
	
	return FALSE;

}

long CDetFieldJoinDlg::GetTabCount()
{
	return 2;
}

CString CDetFieldJoinDlg::GetTabTitle(long nIndex)
{
	CString	strText	= _T("");
	strText.Format("현장이음G%dSP%d", m_nG+1, m_nSp+1);
	if(nIndex==0)	return strText;
	else			return "전체보기";
	return "";
}

BOOL CDetFieldJoinDlg::IsFocusItem(CString szTitle)
{
	if(szTitle.Find("<")!=-1)	return TRUE;
	if(m_nTab==1)	return FALSE;

	if(szTitle=="mB@mC") return TRUE;
	if(szTitle=="mE@mF") return TRUE;
	if(szTitle=="sB@sC") return TRUE;
	if(szTitle=="sE@sF") return TRUE;
	if(szTitle=="sE1@sF1") return TRUE;
	if(szTitle=="uB@uC") return TRUE;
	if(szTitle=="uE@uF") return TRUE;
	if(szTitle=="dB@dC") return TRUE;
	if(szTitle=="dE@dF") return TRUE;

	if(szTitle=="uA") return TRUE;
	if(szTitle=="uC1") return TRUE;
	if(szTitle=="uT") return TRUE;
	if(szTitle=="uW") return TRUE;

	if(szTitle=="dW") return TRUE;
	if(szTitle=="dT") return TRUE;
	if(szTitle=="dA") return TRUE;
	if(szTitle=="dC1") return TRUE;

	if(szTitle=="sT") return TRUE;
	if(szTitle=="sW") return TRUE;
	if(szTitle=="sA") return TRUE;
	if(szTitle=="sC1") return TRUE;

	if(szTitle=="mA") return TRUE;
	if(szTitle=="mC1") return TRUE;
	if(szTitle=="mD") return TRUE;
	if(szTitle=="mG") return TRUE;
	if(szTitle=="mW") return TRUE;
	if(szTitle=="mT") return TRUE;

	return CGeneralBaseDlg::IsFocusItem(szTitle);
}

void CDetFieldJoinDlg::SetFocusItem(CString szTitle)
{
	long	nRow	= 0;
	long	nCol	= 0;

	if(szTitle.Find("<")!=-1)
	{
		CString	szGir	= _T("");
		CString	szSp	= _T("");
		long	nIdx1	= szTitle.Find("G");
		long	nIdx2	= szTitle.Find("S");
		long	nIdx3	= szTitle.Find(">");
		szGir = szTitle.Mid(nIdx1+1, nIdx2-nIdx1-1);
		if(szGir.Find(" ")!=-1)	szGir.TrimRight();
		m_nG = atoi(szGir)-1;

		szSp = szTitle.Mid(nIdx2+2, nIdx3-nIdx2-2);
		if(szSp.Find(" ")!=-1)	szSp.TrimRight();
		m_nSp = atoi(szSp)-1;

		SetSpliceCombo();
		SetDataInit();
		return;
	}
	
	nRow	= FU_ROW+1;
	nCol	= 1;
	if(szTitle=="uA")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="uB@uC")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="uC1")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="uE@uF")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="uT")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="uW")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="uL")		{SetGridFocusCell(nRow,nCol);return;}nCol++;

	nRow	= FL_ROW+1;
	nCol	= 1;
	if(szTitle=="dA")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="dB@dC")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="dC1")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="dE@dF")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="dT")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="dW")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="dL")		{SetGridFocusCell(nRow,nCol);return;}nCol++;

	nRow	= WEB_ROW+1;
	nCol	= 1;
	if(szTitle=="sA")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="sB@sC")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="sC1")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="sE@sF")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="sE1@sF1")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="sT")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="sW")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="sL")		{SetGridFocusCell(nRow,nCol);return;}nCol++;

	nRow	= MO_ROW+1;
	nCol	= 1;
	if(szTitle=="mA")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="mB@mC")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;	
	if(szTitle=="mC1")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="mD")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="mG")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="mE@mF")	{SetGridFocusCell(nRow,nCol);return;}nCol+=2;
	if(szTitle=="mW")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="mT")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
	if(szTitle=="mL")		{SetGridFocusCell(nRow,nCol);return;}nCol++;
}

void CDetFieldJoinDlg::SetGridFocusCell(long nRow, long nCol)
{
	m_Grid.ResetSelection();
	m_Grid.SetFocusCell(nRow,nCol);
	m_Grid.SendMessage(WM_KEYDOWN,VK_F2);
}

void CDetFieldJoinDlg::OnSplicedlgAutocalc() 
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();	
	CTStandard		*pTS	= m_pStd->GetDataManage()->GetBMStandard()->m_pTStandard;
	
	if(!pData->m_bReadFrameModelingOutput)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}

	UpdateData(TRUE);

	if(m_dPermissrate<50)
		if(MessageBox("설정된 허용률이 낮습니다.\n 계속 하시겠습니까", "현장이음 자동계산", MB_ICONQUESTION|MB_YESNO)==IDNO)	return;

	CRect rect = CRect(200, 3, 400, 18);
	if(!m_ctlProgress.GetSafeHwnd())
		m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_ctlProgress.SetRange(0, 100);
	m_ctlProgress.ShowWindow(SW_SHOW);
	pTS->SetPaneText(0,"현장이음 제원 자동 설정중...",TRUE);
	m_ctlProgress.SetPos(0);

	if(m_bAutoCalcAllG)
	{
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			double dProgG	= 100/pDB->GetQtyGirder();
			double dProgSp	= dProgG/pGir->GetQtySplice();

			for(long nSp=0; nSp<pGir->GetQtySplice(); nSp++)
			{
				double dProg = dProgG*nG + dProgSp*(nSp+1);
				m_ctlProgress.SetPos((int)dProg);
				
				SetSpliceAutoCalc(nG, nSp);
			}
		}
	}
	else
		SetSpliceAutoCalc(m_nG, m_nSp);	
	
	m_ctlProgress.SetPos(100);
	pTS->SetPaneText(0,"현장이음 제원 총 설정 완료.",TRUE);
	m_ctlProgress.ShowWindow(SW_HIDE);
	
	SetDataInit();
	DrawInputDomyunView();
}

void CDetFieldJoinDlg::SetSpliceAutoCalc(long nG, long nSp)
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice		*pSp	= pBx->GetSplice();

	CPlateStressOut	pStressOut(m_pStd);
	CMap<CString, LPCTSTR, double, double> MapSplice;
	CStringArray	szCalcArr;

	double dTu		= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	double dTl		= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double dWebT	= pBx->GetFactChain(G_W)->m_dFactChainThick;

	CString szBolt	= _T("");
	BOOL bMoment	= (pSp->m_mT+pSp->m_mA+pSp->m_mB+pSp->m_mC > 0) ? TRUE : FALSE;
	BOOL bContinue	= TRUE;
	
	pSp->m_uT = RoundUp(dTu/2,0);		// 상판
	pSp->m_dT = RoundUp(dTl/2,0);		// 하판
	pSp->m_sT = RoundUp(dWebT/2,0);		// 복부판
	if(long(pSp->m_uT)%2 != 0)	pSp->m_uT++;
	if(long(pSp->m_dT)%2 != 0)	pSp->m_dT++;
	if(long(pSp->m_sT)%2 != 0)	pSp->m_sT++;
	pSp->m_uT = max(10, pSp->m_uT);
	pSp->m_dT = max(10, pSp->m_dT);
	pSp->m_sT = max(10, pSp->m_sT);
	pSp->m_mT = bMoment ? pSp->m_sT : 0;

	pSp->m_uB = 1;
	pSp->m_dB = 1;
	pSp->m_mB = bMoment ? 1 : 0;
	pSp->m_sB = 1;
	while(bContinue)
	{
		bContinue = FALSE;
		
		pStressOut.SpliceCheckOutput(szCalcArr, nG, nSp, MapSplice);
		double dSa = MapSplice["Safu"];				// 허용 합성 전단력
		double dSu = MapSplice["SPLICE_STRESS_UP"];	// 이음판 실제 응력
		double dSl = MapSplice["SPLICE_STRESS_DN"];	// 이음판 실제 응력
		double dWu = MapSplice["UP_WEB_STRESS"];	// 이음판 실제 응력
		double dWl = MapSplice["DN_WEB_STRESS"];	// 이음판 실제 응력

		if(dSu/dSa*100 > m_dPermissrate)			{	pSp->m_uT += 2;		bContinue = TRUE;	}
		if(dSl/dSa*100 > m_dPermissrate)			{	pSp->m_dT += 2;		bContinue = TRUE;	}
		if(max(dWu, dWl)/dSa*100 > m_dPermissrate)	{	pSp->m_sT += 2;		bContinue = TRUE;	}

		double dRa = MapSplice["BOLT_ALLOW_STRESS"];		// 이음판 허용 응력 (볼트의 허용 전단력)
		double dRu = MapSplice["BOLT_ACTING_STRESS_UP"];	// 실제 합성 전단력
		double dRl = MapSplice["BOLT_ACTING_STRESS_DN"];	// 실제 합성 전단력
		double dRm = MapSplice["BOLT_ALLOW_1"];				// 1열의 실제 합성 전단력
		if(pSp->m_mE > 0)	szBolt.Format("BOLT_ALLOW_%d", pSp->m_mE+2);
		else				szBolt.Format("BOLT_ALLOW_1");		
		double dRs = MapSplice[szBolt];						// 복부판 첫열의 실제 합성 전단력				

		if(dRu/dRa*100 > m_dPermissrate)			{	pSp->m_uB++;		bContinue = TRUE;	}
		if(dRl/dRa*100 > m_dPermissrate)			{	pSp->m_dB++;		bContinue = TRUE;	}
		if(dRm/dRa*100 > m_dPermissrate && bMoment)	{	pSp->m_mB++;		bContinue = TRUE;	}		
		if(dRs/dRa*100 > m_dPermissrate)			{	pSp->m_sB++;		bContinue = TRUE;	}
		if(bMoment)	pSp->m_mT = pSp->m_sT;
	}
}
