// DetHBracing.cpp : implementation file
//
  
#include "stdafx.h"
#include "GeneralInput.h"
#include "DetHBracing.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetHBracingDlg dialog

CDetHBracingDlg::CDetHBracingDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetHBracingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetHBracingDlg)
	m_ZzsBoltSize = "M22";
	m_nCurPos = 0;
	m_bTotalView = FALSE;
	//}}AFX_DATA_INIT
	m_nG = 0;
	m_nPos = 0;
}

void CDetHBracingDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetHBracingDlg)
	DDX_Control(pDX, IDC_COMBO_POS, m_ctlPosCombo);
	DDX_Control(pDX, IDC_HBRACING_BOLTSIZE_COMBO, m_ctlBoltSizeCombo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_CBString(pDX, IDC_HBRACING_BOLTSIZE_COMBO, m_ZzsBoltSize);
	DDX_CBIndex(pDX, IDC_COMBO_POS, m_nCurPos);
	DDX_Check(pDX, IDC_CHECK_TOTALVIEW, m_bTotalView);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_AUTOCALC_SPLICE, m_btnAutoPlate);
	DDX_Control(pDX, IDC_AUTO_BOLTLEN, m_btnBoltLength);
	DDX_Control(pDX, IDC_BUTTON_AUTOCALC_SPLICE_ALL, m_btnAllAutoPlate);
	DDX_Control(pDX, IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON, m_btnAllGirder);
	DDX_Control(pDX, IDC_ALLSAMEAPPLY_BUTTON, m_btnAllHBracing);
	DDX_Control(pDX, IDC_BUTTON_INPUT_BASICTYPE, m_btnAllHBracingBasic);
	DDX_Control(pDX, IDC_HBRACING_INPUTCOMPLETE, m_btnAllHBracingInput);
}

BEGIN_MESSAGE_MAP(CDetHBracingDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetHBracingDlg)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnDialogNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnDialogPrev)
	ON_BN_CLICKED(IDC_HBRACING_INPUTCOMPLETE, OnHBracingInputcomplete)
	ON_BN_CLICKED(IDC_AUTO_BOLTLEN, OnAutoBoltlen)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_CBN_SELCHANGE(IDC_HBRACING_BOLTSIZE_COMBO, OnSelChangeBoltsizeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_POS, OnSelchangeCurPos)
	ON_BN_CLICKED(IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON, OnApplyAllGirderSameApply)
	ON_BN_CLICKED(IDC_ALLSAMEAPPLY_BUTTON, OnAllSameApply)
	ON_BN_CLICKED(IDC_CHECK_TOTALVIEW, OnCheckTotalview)
	ON_BN_CLICKED(IDC_BUTTON_AUTOCALC_SPLICE, OnButtonAutoSpliceCalc)
	ON_BN_CLICKED(IDC_BUTTON_AUTOCALC_SPLICE_ALL, OnButtonAutocalcSpliceAll)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_BASICTYPE, OnButtonInputBasictype)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_MESSAGE(WM_GET_NEARBX, OnClickTotalView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetHBracingDlg message handlers
BOOL CDetHBracingDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	CAPlateDrawHBracing DrawHBracing(m_pStd->GetDataManage());
	
	CDomyun *pDom = m_pView->GetDomyun();
	DrawHBracing.SetHBracingSpJewonTotal(pDom);

	pDom->ClearEtt(TRUE);
	DrawInputDomyunView();

	SetResize(IDC_APPLY_ALL_GIRDER_SAMEAPPLY_BUTTON	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_ALLSAMEAPPLY_BUTTON				, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_INPUT_BASICTYPE			, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_TOTALVIEW					, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_CUT_EDGE					, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_GRID								, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_HBRACING_INPUTCOMPLETE			, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;
}

void CDetHBracingDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
	
	CDomyun Dom;
	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir	= pBridge->GetGirder(nG);
		CHBracingGusset *pBrGu	= pGir->GetHBracingGussetInTotal(m_nPos);
		DrawPyung.DrawHBracingBeamWithenBx(&Dom, pBrGu->GetBx(), pBrGu->GetBx(), TRUE);
		SetDataSaveSub(nG, m_nPos);
	}

	SetDataInit();
	CGeneralBaseDlg::OnCellChangedData(nmgv,0);
}

void CDetHBracingDlg::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int  nRow  = GridView->iRow;
	long nIdx  = m_nPos;

	m_nG      = nRow-1;

	for(long nG = 0; nG < m_nG; nG++)
		nIdx += pBridge->GetGirder(nG)->GetQtyHGusset();

	m_ctlPosCombo.SetCurSel(nIdx);
}
void CDetHBracingDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	CString str = m_Grid.GetItemText(0,nCol);

	if( str == "sB" || str == "sC" ) str = "sB@sC";

	m_pView->GetDomyun()->STMakeCursor(str);
}

void CDetHBracingDlg::SetBoltCombo(long nG, long nPos)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	if(nPos >= pBridge->GetGirder(m_nG)->GetQtyHGusset())	return;

	CPlateGirderApp	 *pGir = pBridge->GetGirder(nG);
	CPlateBasicIndex *pBx  = pGir->GetHBracingGussetInTotal(m_nPos)->GetBx();
	CHBracingGusset  *pHGu = pBx->GetHBracingGusset();

	CString szCombo;
	long n = 0;
	for (n=0; n<m_ctlBoltSizeCombo.GetCount(); n++)
	{
		m_ctlBoltSizeCombo.GetLBText(n, szCombo);
		if(szCombo==pHGu->m_ZzsBoltSize)
			break;
	}
	m_ctlBoltSizeCombo.SetCurSel(n);

	UpdateData(TRUE);
}

void CDetHBracingDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(m_nG);	
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();

	CAPlateDrawHBracing DrawHBracing(m_pStd->GetDataManage());	
	CAPlateDrawCross	DrawCross(m_pStd->GetDataManage());	
	CAPlateDrawDanmyun	DrawDan(m_pStd->GetDataManage());

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	//
	if(m_nPos>=pGir->GetQtyHGusset())
	{
		CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
		return;
	}

	CPlateBasicIndex *pBx = pGir->GetHBracingGussetInTotal(m_nPos)->GetBx();
	CHBracingGusset  *pHGu= pBx->GetHBracingGusset();
 
	if(pHGu->IsReal()==FALSE)
	{
		CDomyun Dom(pDom);
		Dom.SetTextHeight(50);
		Dom.TextOut(0, 0, "�������� �����ϴ�.");

		CDRect  BoundRect;
		pOptStd->SetEnvType(&Dom, HCAD_HIDD);
		Dom.SetCalcExtRect();
		BoundRect = Dom.GetExtRect();
		BoundRect.left -= BoundRect.Width();
		BoundRect.right +=BoundRect.Width()/2;
		Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);	
		Dom.Invalidate();
		*pDom << Dom;

		CGeneralBaseDlg::DrawInputDomyunView(TRUE);
		return;
	}

	if(m_nTab==0)	//�Է�
	{
		CDomyun Dom1(pDom);
		CDomyun Dom2(pDom);

		Dom1.SetScaleDim(30);
		DrawHBracing.DrawInputHBracingBySangseType(&Dom1, pGir, m_nPos, FALSE, FALSE, TRUE, FALSE);	

		Dom2.SetScaleDim(30);
 		DrawHBracing.DrawInputHBracingBySangseType(&Dom2, pGir, m_nPos, FALSE, FALSE, TRUE, TRUE);
		Dom2.RedrawByScale(2);
		Dom2.SetCalcExtRect();

		CDRect	rect1 = Dom1.GetExtRect();
		Dom1.Move(-rect1.right, -rect1.CenterPoint().y);

		CDRect	rect2 = Dom2.GetExtRect();
		Dom2.Move(-rect2.left, -rect2.CenterPoint().y);

		*pDom << Dom1;
 		*pDom << Dom2;
	}

	else			// Ⱦ�ܸ鵵 
	{
		CDomyun Dom(pDom);
		Dom.SetScaleDim(30);	
		DrawDan.DrawCRBeamFrontAllOnBx(&Dom, pBx, FALSE, FALSE, FALSE); 
		DrawDan.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, FALSE, FALSE);

		*pDom << Dom;
	}


	if(m_bTotalView)
	{
		CDomyun Dom(pDom);

		m_pStd->GetTotalView()->m_nFlag = BX_HBRACINGGUSSET;

		CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
		CDomyun *pDomDlg = m_pStd->GetTotalView()->m_DomView.GetDomyun();
		m_pStd->GetTotalView()->Clear();
		DrawPyung.DrawPyungOnAirialView(&Dom);
		DrawHBracing.DrawOutLineOfCurHGusset(&Dom, pGir, m_nPos, 1000);
		Dom.Rotate(CDPoint (0,0), pBridge->GetAngleBridgeCenter().GetMirrorVert());
		
		if(m_pStd->GetTotalView()->GetSafeHwnd()) 
			m_pStd->GetTotalView()->m_DomView.RedrawAll();

		*pDomDlg << Dom;

		m_pStd->ShowTotalView(m_pView);
	}
	else
		m_pStd->HideTotalView();

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetHBracingDlg::SetGridTitle()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(m_nG);

	long nRows = pBridge->GetGirdersu()+1;
	long nCols = 19;
	//
	m_Grid.ResetAttrib(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(40);		
	m_Grid.SetColumnWidth(0, 50);

	CString str;
	m_Grid.SetTextMatrix(0,0,"�� ��");

	long nG=0;
	for(long nRow=1; nRow < pBridge->GetGirdersu()+1; nRow++)
	{
		str.Format("G%d",nRow);
		m_Grid.SetTextMatrix(nG+1, 0, str);		
		nG++;
	}	
	
	long nCount = 1;
	long nCol = 0;
	for(nCol=1; nCol<13; )
	{	
		str.Format("dis%d", nCount);
		m_Grid.SetTextMatrix(0, nCol++, str);
		str.Format("W%d", nCount);
		m_Grid.SetTextMatrix(0, nCol++, str);
		str.Format("gW%d", nCount);
		m_Grid.SetTextMatrix(0, nCol++, str);
		nCount++;
	}

	m_Grid.SetTextMatrix(0, nCol++, "sA");
	m_Grid.SetTextMatrix(0, nCol++, "sB");
	m_Grid.SetTextMatrix(0, nCol++, "sC");
	m_Grid.SetTextMatrix(0, nCol++, "sD");
	m_Grid.SetTextMatrix(0, nCol++, "sE");
	m_Grid.SetTextMatrix(0, nCol++, "sL");

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetHBracingDlg::SetDataInit()
{
	SetGridTitle();	
	SetDataInitSub(m_nPos);
	SetBoltCombo(m_nG, m_nPos);
	UpdateData();
}

void CDetHBracingDlg::SetDataInitSub(long nPos)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateBasicIndex* pBx = NULL;
	long nCol;

	CDataManage* pEtc = m_pStd->GetDataManage();	
	BOOL bBoltLengthAuto = pEtc->GetGlobalOption()->GetBoltLengthAuto();	//��� ��Ʈ���� �ڵ���� ...
	CString szText = _T("");
	for(long nG = 0 ; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir  = pBridge->GetGirder(nG);				
		CHBracingGusset *pBrGu = pGir->GetHBracingGussetInTotal(nPos);

		if(pBrGu==NULL)	continue;

		pBx = (CPlateBasicIndex*)pBrGu->GetBx();	
		
		nCol=1;
		if(pBrGu->IsReal())
		{
			long nCount = 0;
			for(nCol = 1; nCol<13; )
			{	
				m_Grid.SetTextMatrix(nG+1, nCol, "%.0f", pBrGu->m_dDis[nCount]);
				if(pBrGu->m_nMatchCol[nCount]==0)
					m_Grid.SetItemState(nG+1, nCol, GVIS_READONLY);
				nCol++;
				m_Grid.SetTextMatrix(nG+1, nCol, "%.0f", pBrGu->m_dWidth[nCount]);
				if(pBrGu->m_nMatchCol[nCount]==0 && !pBrGu->IsReal(long(nCount/2)))
					m_Grid.SetItemState(nG+1, nCol, GVIS_READONLY);
				nCol++;
				m_Grid.SetTextMatrix(nG+1, nCol, "%.0f", pBrGu->m_gW[nCount]);
				if(pBrGu->m_nMatchCol[nCount]==0 && !pBrGu->IsReal(long(nCount/2)))
					m_Grid.SetItemState(nG+1, nCol, GVIS_READONLY);
				nCol++;
				nCount++;
			}
			
			m_Grid.SetTextMatrix(nG+1, nCol++, "%.0f", pBrGu->m_sA);
			szText.Format("%d", pBrGu->m_sB);
			m_Grid.SetTextMatrix(nG+1, nCol++, szText);
			m_Grid.SetTextMatrix(nG+1, nCol++, "%.0f", pBrGu->m_sC);
			m_Grid.SetTextMatrix(nG+1, nCol++, "%.0f", pBrGu->m_sD);
			szText.Format("%d", pBrGu->m_sE);
			m_Grid.SetTextMatrix(nG+1, nCol++, szText);
   
			if(bBoltLengthAuto)	//��Ʈ ���̸� �ڵ���� �� ���(�Է� ��� �� �� ���)
			{
				m_Grid.SetItemState(nG+1, nCol, m_Grid.GetItemState(nG+1, nCol) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nG+1, nCol, RGBREADONLY);
			}
			m_Grid.SetTextMatrix(nG+1, nCol++, "%.0f", pBrGu->GetSL());
		}
		else 
		{
			for(long n=1; n<m_Grid.GetColumnCount(); n++)
			{				
				m_Grid.SetTextMatrix(nG+1, n, "-");
				m_Grid.SetItemState(nG+1, n, m_Grid.GetItemState(nG+1, n) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nG+1, n, RGBREADONLY);
			}
		}
	}
}

void CDetHBracingDlg::SetDataDefault()
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	 

	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)		
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);	
		long nCount=0;
		while(pBx)
		{			
			SetDataDefaultSub(nG, nCount);
			SetDataCalcHBracing(pBx);
//			SetDataSpliceCalc(nG, nCount);

			pBx = Finder.GetBxNext();
			nCount++;
		}
	}
}

void CDetHBracingDlg::SetDataDefaultSub(long nG, long nPos, BOOL bInputStandard)
{
	CPlateBridgeApp		*pBridge	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(nG);
	CHBracingGusset		*pBrGu		= pGir->GetHBracingGussetInTotal(nPos);
	CPlateBasicIndex	*pBx		= pBrGu->GetBx();	
	CSection			*pSec		= pBx->GetSection();

	long nTotal = pBrGu->m_nMatchCol[0]+pBrGu->m_nMatchCol[1]+pBrGu->m_nMatchCol[2]+pBrGu->m_nMatchCol[3];
	if(nTotal== 0)
		return;

	// ������ �Ŵ����� �������� ���� �����Ƿ� �����Ŵ����� üũ
	BOOL bMid = pBx->IsState(BX_CROSSBEAM_VBRACING) ? FALSE : TRUE;
	CPlateBasicIndex	*pBxCross	= pBrGu->GetBxBracing();
	if(!pBrGu->IsReal(1))
		pBxCross	= pBxCross->GetBxMatchByCrossBeam(pGir->m_pLeft);
	// ��V���� 2������ ���� ���κ��� ���� ������
	if(!bMid && pBrGu->m_nMatchCol[0]==1)
		pBxCross	= pBxCross->GetBxPrev(BX_CROSSBEAM_VBRACING, 2);

	long	nSteelType = pBxCross->GetBracing()->GetSteelMaterialData()->m_nType;
	// L����, �εL����, �������϶� ��Ʈ�� ���ݼ� 0, CT�����϶� 1
	long	nsE			= (nSteelType==CSteelMaterialData::LSTEEL || nSteelType==CSteelMaterialData::LVALSTEEL) ? 0 : 1;	// CT���� �϶�

	pBrGu->m_ZzsBoltSize = pBridge->m_szHBrSpBoltDia;
	pBrGu->m_sA		= bInputStandard ? pBridge->m_dHBuSA : 40;
	pBrGu->m_sB		= 2;//bInputStandard ? pBridge->m_dHBuSE : nsB;
	pBrGu->m_sL		= bInputStandard ? pBridge->m_dHBuSL : 80;
	pBrGu->m_sC		= 80;
	pBrGu->m_sD		= 40;
	pBrGu->m_sE		= bInputStandard ? (long)pBridge->m_dHBuSE : (long)nsE;
	pBrGu->m_dT		= 10;

	double dDis = pBx->IsJijum() ? 300 : 150;
	for(long n=0; n<4; n++)
	{
		if(pBrGu->m_nMatchCol[n]==0)
		{
			pBrGu->m_dDis[n]	= 0;
			pBrGu->m_dWidth[n]	= pBrGu->IsReal(long(n/2)) ? -1 : 0;
			pBrGu->m_gW[n]		= 100;
		}
		else
		{
			pBrGu->m_dDis[n]	= dDis;//dDis;//-1;
			pBrGu->m_dWidth[n]	= -1;
			pBrGu->m_gW[n]	= bInputStandard ? pBridge->m_dHBuGw : 20;
		}
	}
}

void CDetHBracingDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	//******************************************************************************//
	// ����극�̽� ���̴� DrawPyung.DrawBeamOnBracingPos()���� �ش� BX �׸��� �׸��� �� 
	// ������ �Ǳ� ������ �Ʒ� ��ƾ���� ��ü�� �ѹ� �׷��ְ� ȭ�� ������ ���۵�.
	// ���� �� ��ƾ�� ������ �������� ���� ���̰� 0�� ����.(�ӵ����� ũ�� ������ ����.)==>������ ������..^^
	CDomyun *pDom = m_pView->GetDomyun();
	
	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
		SetDataSaveSub(nG, m_nPos);	

	for(long n = 0; n < pBridge->GetGirdersu(); n++)
	{
		CDomyun DomTemp(pDom);

		CPlateBasicIndex *pBxStt = pBridge->GetGirder(n)->GetBxOnJijum(0);
		CPlateBasicIndex *pBxEnd = pBridge->GetGirder(n)->GetBxOnJijum(pBridge->m_nQtyJigan);
		
		CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
		DrawPyung.DrawHBracingBeamWithenBx(&DomTemp, pBxStt, pBxEnd, TRUE);	
	}
}

void CDetHBracingDlg::OnPreInitDialog()
{	
	SetInitPositionCombo();	
	SetInitBoltCombo();
	SetInitGita();	

	CDataManage* pData = m_pStd->GetDataManage();
	if(pData->GetGlobalOption()->GetBoltLengthAuto())
	{
		GetDlgItem(IDC_AUTO_BOLTLEN)->EnableWindow(FALSE);		
	}
}

void CDetHBracingDlg::SetInitGita()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp	*pGir    = pBridge->GetGirder(m_nG);
	if(m_nPos>=pGir->GetQtyHGusset())	return;

	CHBracingGusset *pBrGu   = pGir->GetHBracingGussetInTotal(m_nPos);
	
	m_ZzsBoltSize   = pBrGu->m_ZzsBoltSize;
	
	UpdateData(FALSE);
}	

//G1-B-S, G1-B-M, G1-C-S, G1-C-M....�̷� ������� ��Ī �����....
//070123 ����...KB....
//�׻� ������ ���κ��� �극�̽�Ÿ���̴�...
void CDetHBracingDlg::SetInitPositionCombo()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CString strText="";
	CString sMid = "";
	CString sEnd = "";

	for(long nG = 0 ; nG < pBridge->GetGirdersu(); nG++) 
	{	
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);
		
		long nCount = 0;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			//ASSERT(pSec);
			if(pSec)
			{	
				nCount++;
				sEnd = "S";
				if(pSec->GetenDetType()==SECDET_CROSSBEAM)// ���κ� Ÿ��				
					sMid = "C";					
				else if(pSec->GetenDetType()==SECDET_VBRACING)// �극�̽� Ÿ��			
					sMid = "B";
				else//�� Ÿ���� �ƴѰ͵� �ֳ�..
				{
					sEnd = "M";
					nCount--;
				}
			}
			else
			{
				sEnd = "M";
			}

			strText.Format("G%d-%s%d-%s", nG+1, sMid, nCount, sEnd);
			m_ctlPosCombo.AddString(strText);

			pBx = (CPlateBasicIndex *)Finder.GetBxNext();		
		}
	}
	m_ctlPosCombo.SetCurSel(0);
}

void CDetHBracingDlg::SetInitBoltCombo()
{
	//	Bolt Size
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_HBRACING_BOLTSIZE_COMBO);
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(m_nG);
	CHBracingGusset *pBrGu	= pGir->GetHBracingGussetInTotal(m_nPos);

	if(!pBrGu)	return;

	CPlateBasicIndex *pBx	= (CPlateBasicIndex*)pBrGu->GetBx();
	CSteelBolt	*pSteelBolt	= pDB->m_pDataManage->GetSteelBolt();

	CString szBoltGrade = pDB->GetMaterial(HB_BOLT, pBx);

	for(long nIdx=0; nIdx<pSteelBolt->GetSize(); nIdx++)
	{
		if(pSteelBolt->GetStringData(nIdx, BDIB_GRADE) == szBoltGrade)
			pCombo->AddString(pSteelBolt->GetStringData(nIdx, BDIB_SIZE));
	}

	pCombo->SetCurSel(1);//default:M22
	UpdateData(TRUE);
	return;

}

void CDetHBracingDlg::OnDialogNext() 
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	SetDataSave();

	long Index  = m_ctlPosCombo.GetCurSel()+1;

	if(m_nPos >= pBridge->GetGirder(m_nG)->GetQtyHGusset()-1) 
	{
		m_pStd->MoveNextDialog();	
		return;
	}	
	ChangePosIndex(TRUE);	// TRUE : ����
	m_ctlPosCombo.SetCurSel(Index);
	SetDataInit();
	SetInitGita();
	//
	long nIdx = m_nPos;
	for(long nG = 0; nG < m_nG; nG++)
		nIdx += pBridge->GetGirder(nG)->GetQtyHGusset();
	m_ctlPosCombo.SetCurSel(nIdx);
	m_Grid.SetRedraw(TRUE,TRUE);
	//
	DrawInputDomyunView(FALSE);	
}

void CDetHBracingDlg::OnDialogPrev() 
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	
	SetDataSave();
	long Index  = m_ctlPosCombo.GetCurSel()-1;

	if(m_nPos <= 0) {
		m_pStd->GetDetVBracingTypeDlg()->m_nPos = 0;
		m_pStd->MovePrevDialog();
		return;
	}
	ChangePosIndex(FALSE);	// FALSE : ����
	m_ctlPosCombo.SetCurSel(Index);
	SetDataInit();
	SetInitGita();
	//
	long nIdx = m_nPos;
	for(long nG = 0; nG < m_nG; nG++)
		nIdx += pBridge->GetGirder(nG)->GetQtyHGusset();
	m_ctlPosCombo.SetCurSel(nIdx);
	m_Grid.SetRedraw(TRUE,TRUE);
	//

	DrawInputDomyunView(FALSE);	
}


void CDetHBracingDlg::SetDataSaveSub(long nG, long nPos)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(nG);
	if(nPos>=pGir->GetQtyHGusset())	return;

	CHBracingGusset *pBrGu = pGir->GetHBracingGussetInTotal(nPos);	

	long nCount = 0;
	long nCol = 0;
	for(nCol = 1; nCol<13; nCol)
	{
		pBrGu->m_dDis[nCount]	= m_Grid.GetTextMatrixDouble(nG+1, nCol++);
		pBrGu->m_dWidth[nCount]	= m_Grid.GetTextMatrixDouble(nG+1, nCol++);
		pBrGu->m_gW[nCount]		= m_Grid.GetTextMatrixDouble(nG+1, nCol++);
		nCount++;
	}
	
	pBrGu->m_sA		= m_Grid.GetTextMatrixDouble(nG+1, nCol++);
	pBrGu->m_sB		= m_Grid.GetTextMatrixLong(nG+1, nCol++);
	pBrGu->m_sC		= m_Grid.GetTextMatrixDouble(nG+1, nCol++);
	pBrGu->m_sD		= m_Grid.GetTextMatrixDouble(nG+1, nCol++);
	pBrGu->m_sE		= m_Grid.GetTextMatrixLong(nG+1, nCol++);
	pBrGu->m_sL		= m_Grid.GetTextMatrixDouble(nG+1,nCol++);
}

void CDetHBracingDlg::ChangePosIndex(BOOL bInc)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	if(bInc)
	{	//����
		m_nPos++;
		if(m_nPos > pBridge->GetGirder(m_nG)->GetQtyHGusset()-1)
		{
			m_nPos	= 0;
			m_nG	= min(m_nG+1, pBridge->GetQtyGirder()-1);
		}
	}
	else
	{	//����
		if(m_nPos==0)	m_nG = max(0, m_nG-1);
		else			m_nPos--;
	}
}

void CDetHBracingDlg::OnHBracingInputcomplete() 
{
	SetDataInit();
	SetDataSave();

	m_pStd->MoveNextDialog();	
}


void CDetHBracingDlg::OnAutoBoltlen() 
{
	CPlateBridgeApp  *pBridge   = m_pStd->GetBridge();	
	CPlateGirderApp  *pGir      = pBridge->GetGirder(m_nG);
	CHBracingGusset  *pBrGu     = pGir->GetHBracingGussetInTotal(m_nPos);
	if(pBrGu == NULL)	return;
	CPlateBasicIndex *pBxBracing= pBrGu->GetBxBracing();
	CSteelBolt		 *pBolt		= m_pStd->GetBridge()->m_pDataManage->GetSteelBolt();
	CBracing		*pBracing	= pBrGu->GetBxBracing()->GetBracing();
	
	// �, �ε L������ T1 �̰� �׿ܴ� T2��.
	CSection *pSecBr    = pBxBracing->GetSection();
	long     nSteelType = pBracing->GetSteelMaterialData()->m_nType;
	double   dBeamT     = pSecBr->GetBeamJewon(CSection::SEC_LHBEAM, CSection::T2);

	if(nSteelType==CSteelMaterialData::LSTEEL || nSteelType==CSteelMaterialData::LVALSTEEL)
		dBeamT			= pSecBr->GetBeamJewon(CSection::SEC_LHBEAM, CSection::T1);

	double dT = pBrGu->m_dT + dBeamT;

	pBrGu->m_sL = pBolt->GetBoltLength(pBxBracing, HB_BOLT, pBrGu->m_ZzsBoltSize, dT);
	
	SetDataInit();				
	SetDataSave();
}

void CDetHBracingDlg::OnButtonRecommend() 
{
	CPlateBridgeApp  *pBridge = m_pStd->GetBridge();
	CHBracingGusset  *pBrGu   = pBridge->GetGirder(m_nG)->GetHBracingGussetInTotal(m_nPos);
	CPlateBasicIndex *pBxGu   = pBrGu->GetBx();
	
	SetDataDefaultSub(m_nG, m_nPos);
	SetDataCalcHBracing(pBxGu);
//	SetDataSpliceCalc(m_nG, m_nPos);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);
	
}

void CDetHBracingDlg::OnSelChangeBoltsizeCombo() 
{
	UpdateData(TRUE);
	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp	*pGir    = pBridge->GetGirder(m_nG);
	if(m_nPos>=pGir->GetQtyHGusset())	return;

	CHBracingGusset* pBrGu = pGir->GetHBracingGussetInTotal(m_nPos);

	if(pBrGu == NULL)	return;

	pBrGu->m_ZzsBoltSize = m_ZzsBoltSize;

	DrawInputDomyunView(FALSE);	
}

void CDetHBracingDlg::OnSelchangeCurPos() 
{
	long su = 0, oldsu = 0, sw = 0; 
	long pos = m_ctlPosCombo.GetCurSel();
	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	long g = 0;
	for(g=0; g < pBridge->GetGirdersu()-1; g++)
	{
		su += pBridge->GetGirder(g)->GetQtyHGusset() - sw;
		if( oldsu <= pos && pos < su ) break;
		oldsu = su+sw;
	}
	m_nG = g;
	m_nPos = pos - oldsu;

	SetDataInit();
	SetInitGita();
	//if(m_nG!=nGOld)
		DrawInputDomyunView(TRUE);
}

void CDetHBracingDlg::OnApplyAllGirderSameApply() 
{
	if(MessageBox("������ �Է��� �����Ͱ� ��� �������ϴ�.\n ��� �Ͻðڽ��ϱ�", "����극�̽� �� �Է� �⺻��", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	
	SetDataAllGirderSame();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetHBracingDlg::OnAllSameApply() 
{
	if(MessageBox("������ �Է��� �����Ͱ� ��� �������ϴ�.\n ��� �Ͻðڽ��ϱ�", "����극�̽� �� �Է� �⺻��", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	SetDataAllSame();

	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CDetHBracingDlg::SetDataAllGirderSame()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir  = pBridge->GetGirder(m_nG);
	CPlateGirderApp *pGirN = pBridge->GetGirder(m_nG+1);
	if(m_nPos>=pGir->GetQtyHGusset())	return;

	CHBracingGusset *pBrGu = pGir->GetHBracingGussetInTotal(m_nPos);

	CHBracingGusset* pBrGuN = NULL;
	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++) 
	{
		pGir = pBridge->GetGirder(nG);
		pBrGuN= pGir->GetHBracingGussetInTotal(m_nPos);
		if( pBrGuN == NULL ) continue;

		*pBrGuN <<= *pBrGu;  
	}
}

void CDetHBracingDlg::SetDataAllSame()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(m_nG);
	CPlateGirderApp *pGirN = pBridge->GetGirder(m_nG+1);
	if(m_nPos>=pGir->GetQtyHGusset())	return;

	CHBracingGusset *pBrGu = pGir->GetHBracingGussetInTotal(m_nPos);

	CHBracingGusset* pBrGuN  = NULL;

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		pGir = pBridge->GetGirder(nG);
		for(long nHBr = 0; nHBr < pBridge->GetGirder(nG)->GetQtyHGusset(); nHBr++)
		{
			pBrGuN = pGir->GetHBracingGussetInTotal(nHBr);
			*pBrGuN <<= *pBrGu;   
		}
	}
}

void CDetHBracingDlg::OnCheckTotalview() 
{
	UpdateData();
	DrawInputDomyunView(FALSE);
}

long CDetHBracingDlg::OnClickTotalView(WPARAM wp, LPARAM lp) 
{
	CPlateBasicIndex *pBx  = (CPlateBasicIndex *)wp;
	CPlateGirderApp  *pGir = pBx->GetGirder();
	SetDataSave();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxComp = Finder.GetBxFirst(BX_HBRACINGGUSSET);
	long nHBr = 0;
	while(pBxComp)
	{
		if(pBx==pBxComp)
		{
			m_nPos = nHBr;
			break;
		}
		
		nHBr++;
		pBxComp = Finder.GetBxNext();
	}
	SetDataInit();
	m_Grid.ResetSelection();
	m_Grid.SetFocusCell(pBx->GetNumberGirder()+1, 1);
	DrawInputDomyunView(FALSE);

	return 0;
}
/*
// ��Ʈ��ġ��(sB*sC+sD*2)�� �������� �������� ũ�⸦ ������
// 1. �����ǿ��� ���糡�ܱ��� ���ܰŸ��� �ּ�50mm�� �Ǹ� OK. ������ 50mm�� �Ǵ� ���� ���� �������� ��´�.
// 2. ����� �����극�̽� ��������� �ּҰŸ� 20mm
// 3. ����� ���κ� ���ο��� �ּҰŸ� 20mm
// 4. ����극�̽� ����� ����극�̽� ���� �ּҰŸ� 20mm
// 5. ����극�̽� ����� �������� ���̷� �ּҰŸ� 20mm
// ��ġ : (5.1) ���κ� ������ ���������� ��ġ.
//        (5.2) ���κ���ġ �� �������� �и����� �ʰ� ���������� ��ġ���� ���̷��� ����� ���. 
void CDetHBracingDlg::SetDataSpliceCalc(long nG, long nPos)
{
	CLineInfoApp     *pLine      = m_pStd->GetLineInfoApp();
	CPlateBridgeApp  *pBridge    = m_pStd->GetBridge();
	CPlateGirderApp  *pGir       = pBridge->GetGirder(nG);
	CHBracingGusset  *pBrGu      = pGir->GetHBracingGussetInTotal(nPos);
	CPlateBasicIndex *pBx        = pBrGu->GetBx();
	CPlateBasicIndex *pBxBracing = pBrGu->GetBxBracing();
	CGlobarOption *pOpt = m_pStd->m_pDataManage->GetGlobalOption();

	double dSta        = pBx->GetStation();
	double dLengthWeld = pBrGu->m_SB*pBrGu->m_dSC+pBrGu->m_dSD*2;
	double dThickWeb   = pGir->GetThickFactByStation(dSta, G_W);
	double dLengthMin  = pBridge->m_dHBuStep1;	// �����ǿ��� ��������� ���ܰŸ�
	double dGap        = pBrGu->m_dgW; 
	double dHBeam      = pBxBracing->GetSection()->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);	// ������ ��(�����ο� �Ϲݺ� �����ؾ� ��)
	double dHBeamCen   = pBxBracing->GetSection()->GetBeamJewon(CSection::SEC_LHBEAM, CSection::CENTERDX);		
	if(!pOpt->GetInstallBeamAtCenter())
		dHBeamCen = dHBeam/2;
	CDPoint xyGirCen   = pGir->GetXyGirderDis(dSta);
	CDPoint vGir       = pLine->GetAngleAzimuth(dSta);
	CDPoint xyBeamStt[4], xyMatch[4], xyTemp;
	CDPoint xyBeamLT[2], xyBeamRT[2], xyBeamLD[2], xyBeamRD[2];

	CString fs;
	// 1. �����ǿ��� ���糡�ܱ��� ���ܰŸ��� �ּ�50mm�� �Ǹ� OK. ������ 50mm�� �Ǵ� ���� ���� �������� ��´�.
	for(long nDir=0; nDir<4; nDir++)
	{
		if(pBrGu->m_nMatchCol[nDir]==0)		continue;

		switch(nDir)
		{
		case 0:
			GetXyMatchLineAndLine(xyGirCen + vGir.Rotate90()*(dThickWeb/2+dLengthMin), vGir, 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*(dHBeam-dHBeamCen)  , pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLT [0]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLT [1]    = xyBeamLT[0]   + pBrGu->xyDir[nDir].RotateInv90()*(dHBeam+dGap*2);;

			
			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLT[0], vGir, xyTemp);
			
			pBrGu->m_dw1U = ~(xyBeamLT[0]-xyTemp);
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLT[1], vGir.Rotate90(), xyTemp);
			pBrGu->m_dh1L  = ~(xyBeamLT[1]-xyTemp)-dThickWeb/2;
			break;
		case 1:
			GetXyMatchLineAndLine(xyGirCen + vGir.Rotate90()*(dThickWeb/2+dLengthMin), vGir, 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*(dHBeam-dHBeamCen)  , pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRT [0]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRT [1]    = xyBeamRT[0]   + pBrGu->xyDir[nDir].Rotate90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRT[0], vGir, xyTemp);
			pBrGu->m_dw2U = ~(xyBeamRT[0]-xyTemp);
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRT[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh1L > 0)	// case 0 ���� ���� pBrGu->m_dh1L�� ���Ͽ� ū ���� ������.
				pBrGu->m_dh1L = max(pBrGu->m_dh1L, ~(xyBeamRT[1]-xyTemp)-dThickWeb/2);
			break;
		case 2:
			GetXyMatchLineAndLine(xyGirCen - vGir.Rotate90()*(dThickWeb/2+dLengthMin), vGir, 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*dHBeamCen  , pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLD [0]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLD [1]    = xyBeamLD[0]   + pBrGu->xyDir[nDir].Rotate90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLD[0], vGir, xyTemp);
			pBrGu->m_dw1D = ~(xyBeamLD[0]-xyTemp);
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLD[1], vGir.Rotate90(), xyTemp);
			pBrGu->m_dh2L  = ~(xyBeamLD[1]-xyTemp)-dThickWeb/2;
			break;
		case 3:
			GetXyMatchLineAndLine(xyGirCen - vGir.Rotate90()*(dThickWeb/2+dLengthMin), vGir, 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*dHBeamCen  , pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRD [0]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRD [1]    = xyBeamRD[0]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRD[0], vGir, xyTemp);
			pBrGu->m_dw2D  = ~(xyBeamRD[0]-xyTemp);
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRD[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh2L > 0)	// case 2 ���� ���� pBrGu->m_dh2L�� ���Ͽ� ū ���� ������.
				pBrGu->m_dh2L = max(pBrGu->m_dh2L,  ~(xyBeamRD[1]-xyTemp)-dThickWeb/2);

			break;
		}
	}
	// 2. ����� �����극�̽� ��������� �ּҰŸ� 20mm
	// 1���� ��찡 2���� ���ǿ� ������ ���並 ���� �ʰ� ���� 2�����ǿ� �����ϴ� ũ�⸦ ���� ��
	// 1�������� ũ��� ���Ͽ� ū ���� �����.
	dLengthMin = pBridge->m_dHBuStep2;
	for(nDir=0; nDir<4; nDir++)
	{
		if(pBrGu->m_nMatchCol[nDir]==0)			continue;	// ������簡 ���� ���
		if(pBx->IsState(BX_CROSSBEAM_VBRACING)==FALSE)	continue;	// ���κ���ġ�� �ƴ� ���(���κ�����)
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))	continue;// �����극�̽��� �ƴ� ���
		
		double dWBeamV = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::B);
		CDPoint vAng   = pBx->GetAngle();
		CVBracing *pVr = pBx->GetSection()->GetVBracing();
		double dTVStiff= max(pVr->m_pVStiff->m_dThick[0], pVr->m_pVStiff->m_dThick[1]);

		switch(nDir)
		{
		case 0:
			GetXyMatchLineAndLine(xyGirCen + vGir.Rotate(vAng).Rotate90()*(dLengthMin+dWBeamV-dTVStiff/2), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLT [0]    = xyBeamLT[1]   + pBrGu->xyDir[nDir].Rotate90()*(dHBeam+dGap*2);;

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLT[0], vGir, xyTemp);
			if(pBrGu->m_dw1U > 0)
				pBrGu->m_dw1U = max(pBrGu->m_dw1U, ~(xyBeamLT[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLT[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh1L > 0)
				pBrGu->m_dh1L  = max(pBrGu->m_dh1L, ~(xyBeamLT[1]-xyTemp)-dThickWeb/2);
			break;
		case 1:
			GetXyMatchLineAndLine(xyGirCen + vGir.Rotate(vAng).RotateInv90()*(dLengthMin+dTVStiff+pVr->m_dT/2), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRT [0]    = xyBeamRT[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRT[0], vGir, xyTemp);
			if(pBrGu->m_dw2U > 0)
				pBrGu->m_dw2U = max(pBrGu->m_dw2U, ~(xyBeamRT[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRT[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh1L > 0)
				pBrGu->m_dh1L = max(pBrGu->m_dh1L, ~(xyBeamRT[1]-xyTemp)-dThickWeb/2);
			break;
		case 2:
			GetXyMatchLineAndLine(xyGirCen - vGir.Rotate(vAng).RotateInv90()*(dLengthMin+dWBeamV-dTVStiff/2), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLD [0]    = xyBeamLD[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLD[0], vGir, xyTemp);
			if(pBrGu->m_dw1D > 0)
				pBrGu->m_dw1D = max(pBrGu->m_dw1D, ~(xyBeamLD[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLD[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh2L > 0)
				pBrGu->m_dh2L = max(pBrGu->m_dh2L, ~(xyBeamLD[1]-xyTemp)-dThickWeb/2);
			break;
		case 3:
			GetXyMatchLineAndLine(xyGirCen - vGir.Rotate(vAng).Rotate90()*(dLengthMin+dTVStiff+pVr->m_dT/2), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRD [0]    = xyBeamRD[1]   + pBrGu->xyDir[nDir].Rotate90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRD[0], vGir, xyTemp);
			if(pBrGu->m_dw2D > 0)
				pBrGu->m_dw2D  = max(pBrGu->m_dw2D, ~(xyBeamRD[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRD[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh2L > 0)
				pBrGu->m_dh2L  = max(pBrGu->m_dh2L, ~(xyBeamRD[1]-xyTemp)-dThickWeb/2);
		
			break;
		}
	}
	// 3. ����� ���κ� ���ο��� �ּҰŸ� 20mm
	dLengthMin = pBridge->m_dHBuStep3;
	for(nDir=0; nDir<4; nDir++)
	{
		if(pBrGu->m_nMatchCol[nDir]==0)			continue;	// ������簡 ���� ���
		if(pBx->IsState(BX_CROSSBEAM_VBRACING)==FALSE)	continue;	// ���κ���ġ�� �ƴ� ���(���κ�����)
		if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))	continue;// ���κ��� �ƴ� ���
		
		CDPoint vAng   = pBx->GetAngle();
		double dThickCrweb = 0;
		if(nDir < 2)
			dThickCrweb = pBx->GetCrossBeam()->m_T3_CR;
		else
		{
			CPlateBasicIndex *pBxTemp	= pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);
			CPlateCrossBeam	 *pCTemp	= NULL;
			if(pBxTemp && pBxTemp->GetSection())
				pCTemp	= pBxTemp->GetCrossBeam();

			dThickCrweb =  pCTemp? pCTemp->m_T3_CR : 12;
		}
	
		switch(nDir)
		{
		case 0:
			GetXyMatchLineAndLine(xyGirCen + vGir.Rotate(vAng).Rotate90()*(dThickCrweb/2+dLengthMin), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLT [0]    = xyBeamLT[1]   + pBrGu->xyDir[nDir].Rotate90()*(dHBeam+dGap*2);;

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLT[0], vGir, xyTemp);
			if(pBrGu->m_dw1U > 0)
				pBrGu->m_dw1U = max(pBrGu->m_dw1U, ~(xyBeamLT[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLT[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh1L > 0)
				pBrGu->m_dh1L  = max(pBrGu->m_dh1L, ~(xyBeamLT[1]-xyTemp)-dThickWeb/2);
			break;
		case 1:
			GetXyMatchLineAndLine(xyGirCen + vGir.Rotate(vAng).RotateInv90()*(dThickCrweb/2+dLengthMin), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRT [0]    = xyBeamRT[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRT[0], vGir, xyTemp);
			if(pBrGu->m_dw2U > 0)
				pBrGu->m_dw2U = max(pBrGu->m_dw2U, ~(xyBeamRT[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRT[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh1L > 0)
				pBrGu->m_dh1L = max(pBrGu->m_dh1L, ~(xyBeamRT[1]-xyTemp)-dThickWeb/2);
			break;
		case 2:
			GetXyMatchLineAndLine(xyGirCen - vGir.Rotate(vAng).RotateInv90()*(dThickCrweb/2+dLengthMin), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLD [0]    = xyBeamLD[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLD[0], vGir, xyTemp);
			if(pBrGu->m_dw1D > 0)
				pBrGu->m_dw1D = max(pBrGu->m_dw1D, ~(xyBeamLD[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLD[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh2L > 0)
				pBrGu->m_dh2L = max(pBrGu->m_dh2L, ~(xyBeamLD[1]-xyTemp)-dThickWeb/2);
			break;
		case 3:
			GetXyMatchLineAndLine(xyGirCen - vGir.Rotate(vAng).Rotate90()*(dThickCrweb/2+dLengthMin), vGir.Rotate(vAng), 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRD [0]    = xyBeamRD[1]   + pBrGu->xyDir[nDir].Rotate90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRD[0], vGir, xyTemp);
			if(pBrGu->m_dw2D > 0)
				pBrGu->m_dw2D  = max(pBrGu->m_dw2D, ~(xyBeamRD[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRD[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh2L > 0)
				pBrGu->m_dh2L  = max(pBrGu->m_dh2L, ~(xyBeamRD[1]-xyTemp)-dThickWeb/2);

			break;
		}
	}
	// 4. ����극�̽� ����� ����극�̽� ���� �ּҰŸ� 20mm
	//     �� ����극�̽� ������̰��������� �������� 10mm�̰� ���� ����.
	// 3�������� ũ��� ���Ͽ� ū ���� �����.
	// ��ġ : ���κ� ������ġ�� �ƴ϶� ���κ����� ��ġ��.
	dLengthMin = pBridge->m_dHBuStep4;
	for(nDir=0; nDir<4; nDir++)
	{
		if(pBrGu->m_nMatchCol[nDir]==0)			continue;	// ������簡 ���� ���
		if(pBx->IsState(BX_CROSSBEAM_VBRACING))			continue;	// ���κ���ġ�� ���
	
		switch(nDir)
		{
		case 0:
			GetXyMatchLineAndLine(xyGirCen - vGir*dLengthMin/2, (pBrGu->xyDir[0]+pBrGu->xyDir[1])/2, 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLT [0]    = xyBeamLT[1]   + pBrGu->xyDir[nDir].Rotate90()*(dHBeam+dGap*2);;

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLT[0], vGir, xyTemp);
			if(pBrGu->m_dw1U > 0)
				pBrGu->m_dw1U = max(pBrGu->m_dw1U, ~(xyBeamLT[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLT[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh1L > 0)
				pBrGu->m_dh1L  = max(pBrGu->m_dh1L, ~(xyBeamLT[1]-xyTemp)-dThickWeb/2);
			break;
		case 1:
			GetXyMatchLineAndLine(xyGirCen + vGir*dLengthMin/2, (pBrGu->xyDir[0]+pBrGu->xyDir[1])/2, 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRT [0]    = xyBeamRT[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRT[0], vGir, xyTemp);
			if(pBrGu->m_dw2U > 0)
				pBrGu->m_dw2U = max(pBrGu->m_dw2U, ~(xyBeamRT[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRT[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh1L > 0)
				pBrGu->m_dh1L = max(pBrGu->m_dh1L, ~(xyBeamRT[1]-xyTemp)-dThickWeb/2);
			break;
		case 2:
			GetXyMatchLineAndLine(xyGirCen - vGir*dLengthMin/2, (pBrGu->xyDir[2]+pBrGu->xyDir[3])/2, 
				                  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamLD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamLD [0]    = xyBeamLD[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLD[0], vGir, xyTemp);
			if(pBrGu->m_dw1D > 0)
				pBrGu->m_dw1D = max(pBrGu->m_dw1D, ~(xyBeamLD[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLD[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh2L > 0)
				pBrGu->m_dh2L = max(pBrGu->m_dh2L, ~(xyBeamLD[1]-xyTemp)-dThickWeb/2);
			break;
		case 3:
			GetXyMatchLineAndLine(xyGirCen + vGir*dLengthMin/2, (pBrGu->xyDir[2]+pBrGu->xyDir[3])/2, 
				                  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
			xyBeamRD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
			xyBeamRD [0]    = xyBeamRD[1]   + pBrGu->xyDir[nDir].Rotate90()*(dGap*2+dHBeam); 

			GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRD[0], vGir, xyTemp);
			if(pBrGu->m_dw2D > 0)
				pBrGu->m_dw2D  = max(pBrGu->m_dw2D, ~(xyBeamRD[0]-xyTemp));
			GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRD[1], vGir.Rotate90(), xyTemp);
			if(pBrGu->m_dh2L > 0)
				pBrGu->m_dh2L  = max(pBrGu->m_dh2L, ~(xyBeamRD[1]-xyTemp)-dThickWeb/2);


			break;
		}
	}
	// 5. ����극�̽� ����� �������� ���̷� �ּҰŸ� 20mm
	// 4�������� ũ��� ���Ͽ� ū ���� �����.
	// ��ġ : (1) ���κ� ������ ���������� ��ġ.
	//        (2) ���κ���ġ �� �������� �и����� �ʰ� ���������� ��ġ���� ���̷��� ����� ���. 
	
	dLengthMin = pBridge->m_dHBuStep5;	// ���̷��� ��80+�ּҿ��ܰŸ�20*2

	// �Ŵ� �߽ɿ��� ���� 25mm�ȿ� ����������� ���̷��� �ִ� ���
	CPlateBasicIndexArray BasicIndexArrayVstiff; 
	pGir->GetBxWithenStation(&BasicIndexArrayVstiff, pBx->GetStation()-25, pBx->GetStation()+25,
							 BX_VSTIFF | BX_CROSSBEAM_VBRACING);
	if(BasicIndexArrayVstiff.GetSize() > 0)
	{
		CPlateBasicIndex *pBxVStiff = BasicIndexArrayVstiff.GetAt(0);	// �⺻������ �Ѱ��� �ִٰ� ������.
		CVStiff          *pVStiff   = pBxVStiff->GetSection()->GetVStiff();
		CDPoint xyScallopCen = pGir->GetXyGirderDis(pBxVStiff->GetStation());
		CDPoint	vAngScallop  = ANGLE90;
		if(pBx->IsState(BX_CROSSBEAM_VBRACING))		vAngScallop = pBxVStiff->GetAngleSection(); 

		for(nDir=0; nDir<4; nDir++)
		{
			BOOL bDivideSp = nDir<2 ? pBrGu->IsSpMatchingWithWeb(TRUE) : pBrGu->IsSpMatchingWithWeb(FALSE);

			// ������簡 ���ų� �������� �������� �и��Ǵ� ���
			if(pBrGu->m_nMatchCol[nDir]==0 || bDivideSp==TRUE)			continue;	
			if(pBx->IsState(BX_CROSSBEAM_VBRACING) && pBx->GetSection()->IsenDetType(SECDET_VBRACING))		continue;

			switch(nDir)
			{
			case 0:
				if(pVStiff->m_dThick[0]==0)			break;

				GetXyMatchLineAndLine(xyScallopCen + vGir.Rotate(vAngScallop).Rotate90()*dLengthMin/2, vGir.Rotate(vAngScallop), 
									  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
				xyBeamLT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
				xyBeamLT [0]    = xyBeamLT[1]   + pBrGu->xyDir[nDir].Rotate90()*(dHBeam+dGap*2);

				GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLT[0], vGir, xyTemp);
				if(pBrGu->m_dw1U > 0)
					pBrGu->m_dw1U = max(pBrGu->m_dw1U, ~(xyBeamLT[0]-xyTemp));
				GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLT[1], vGir.Rotate90(), xyTemp);
				if(pBrGu->m_dh1L > 0)
					pBrGu->m_dh1L  = max(pBrGu->m_dh1L, ~(xyBeamLT[1]-xyTemp)-dThickWeb/2);

				break;
			case 1:
				if(pVStiff->m_dThick[0]==0)			break;

				GetXyMatchLineAndLine(xyScallopCen - vGir.Rotate(vAngScallop).Rotate90()*dLengthMin/2, vGir.Rotate(vAngScallop), 
									  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*dHBeamCen, pBrGu->xyDir[nDir], xyMatch[nDir]);
				xyBeamRT [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
				xyBeamRT [0]    = xyBeamRT[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

				GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRT[0], vGir, xyTemp);
				if(pBrGu->m_dw2U > 0)
					pBrGu->m_dw2U = max(pBrGu->m_dw2U, ~(xyBeamRT[0]-xyTemp));
				GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRT[1], vGir.Rotate90(), xyTemp);
				if(pBrGu->m_dh1L > 0)
					pBrGu->m_dh1L = max(pBrGu->m_dh1L, ~(xyBeamRT[1]-xyTemp)-dThickWeb/2);

				break;
			case 2:
				if(pVStiff->m_dThick[1]==0)			break;

				GetXyMatchLineAndLine(xyScallopCen + vGir.Rotate(vAngScallop).Rotate90()*dLengthMin/2, vGir.Rotate(vAngScallop), 
									  xyGirCen + pBrGu->xyDir[nDir].Rotate90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
				xyBeamLD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].Rotate90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
				xyBeamLD [0]    = xyBeamLD[1]   + pBrGu->xyDir[nDir].RotateInv90()*(dGap*2+dHBeam); 

				GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamLD[0], vGir, xyTemp);
				if(pBrGu->m_dw1D > 0)
					pBrGu->m_dw1D = max(pBrGu->m_dw1D, ~(xyBeamLD[0]-xyTemp));
				GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamLD[1], vGir.Rotate90(), xyTemp);
				if(pBrGu->m_dh2L > 0)
					pBrGu->m_dh2L = max(pBrGu->m_dh2L, ~(xyBeamLD[1]-xyTemp)-dThickWeb/2);

				break;
			case 3:
				if(pVStiff->m_dThick[1]==0)			break;

				GetXyMatchLineAndLine(xyScallopCen - vGir.Rotate(vAngScallop).Rotate90()*dLengthMin/2, vGir.Rotate(vAngScallop), 
									  xyGirCen + pBrGu->xyDir[nDir].RotateInv90()*(dHBeam-dHBeamCen), pBrGu->xyDir[nDir], xyMatch[nDir]);
				xyBeamRD [1]    = xyMatch[nDir] + pBrGu->xyDir[nDir].RotateInv90()*dGap + pBrGu->xyDir[nDir]*dLengthWeld;
				xyBeamRD [0]    = xyBeamRD[1]   + pBrGu->xyDir[nDir].Rotate90()*(dGap*2+dHBeam); 

				GetXyMatchLineAndLine(xyGirCen, vGir.Rotate90(), xyBeamRD[0], vGir, xyTemp);
				if(pBrGu->m_dw2D > 0)
					pBrGu->m_dw2D  = max(pBrGu->m_dw2D, ~(xyBeamRD[0]-xyTemp));
				GetXyMatchLineAndLine(xyGirCen, vGir, xyBeamRD[1], vGir.Rotate90(), xyTemp);
				if(pBrGu->m_dh2L > 0)
					pBrGu->m_dh2L  = max(pBrGu->m_dh2L, ~(xyBeamRD[1]-xyTemp)-dThickWeb/2);
				break;
			}
		}
	}
}
*/
void CDetHBracingDlg::OnButtonAutoSpliceCalc() 
{
	CPlateBridgeApp  *pBridge = m_pStd->GetBridge();
	CHBracingGusset  *pBrGu   = pBridge->GetGirder(m_nG)->GetHBracingGussetInTotal(m_nPos);
	CPlateBasicIndex *pBxGu   = pBrGu->GetBx();

	SetDataDefaultSub(m_nG, m_nPos);
	SetDataCalcHBracing(pBxGu);
//	SetDataSpliceCalc(m_nG, m_nPos);
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);
}

BOOL CDetHBracingDlg::IsValid() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)		
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);	
		while(pBx)
		{
			if(pBx->GetHBracingGusset()->IsReal())
				return TRUE;

			pBx = Finder.GetBxNext();
		}
	}
	return FALSE;
}

// Ⱦ�극�̽� �������� ������ ������ �����.
// pBrGu->xyDir[0~3]
// 0 : �»�, 1: ���, 2: ����, 3: ����
void CDetHBracingDlg::SetDataCalcHBracing(CPlateBasicIndex *pBx)
{
	CPlateGirderApp* pGir    = pBx->GetGirder();
	CPlateGirderApp* pGirL   = pGir->m_pLeft;
	CPlateGirderApp* pGirR   = pGir->m_pRight;
	CHBracingGusset* pBrGu   = pBx->GetHBracingGusset();
	//
	if(!pBrGu->IsReal()) return;	// m_nMatchCol�� ��� 0�ϰ��
	CPlatePieceObject pObj1(pGir), pObj2(pGir);

	pBrGu->xyDir[0] = CDPoint(0,0);
	pBrGu->xyDir[1] = CDPoint(0,0);
	pBrGu->xyDir[2] = CDPoint(0,0);
	pBrGu->xyDir[3] = CDPoint(0,0);

	// Ⱦ�극�̽� �������� ������Ʈ�� �����ϴ� �������� ������ ������ �����
	pBrGu->CreatePieceObj(&pObj1, &pObj2, FALSE, FALSE);
}	
	

void CDetHBracingDlg::OnButtonAutocalcSpliceAll() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)		
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);	
		long nCount=0;
		while(pBx)
		{			
			SetDataDefaultSub(nG, nCount);
			SetDataCalcHBracing(pBx);
//			SetDataSpliceCalc(nG, nCount);

			pBx = Finder.GetBxNext();
			nCount++;
		}
	}
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);	
}

BOOL CDetHBracingDlg::IsFocusItem(CString szTitle)
{
	if(szTitle=="sB@sC") return TRUE;

	return CGeneralBaseDlg::IsFocusItem(szTitle);
}

void CDetHBracingDlg::SetFocusItem(CString szTitle)
{	
	if(szTitle=="sB@sC")	{m_Grid.ResetSelection(); m_Grid.SetFocusCell(m_nG+1, 11); m_Grid.SendMessage(WM_KEYDOWN,VK_F2); return; }

	CGeneralBaseDlg::SetFocusItem(szTitle);
}

void CDetHBracingDlg::OnButtonInputBasictype() 
{
	SetDataDefault();
	SetDataInit();
	DrawInputDomyunView();
/*	CPlateBridgeApp		*pBridge = m_pStd->GetBridge();
	CHBrSpStdInputDlg	StdDlg;

	StdDlg.m_pStd	= m_pStd;

	if(StdDlg.DoModal() == IDOK)
	{
		if(MessageBox("������ �Է��� �����Ͱ� ��� �������ϴ�.\n ��� �Ͻðڽ��ϱ�",
					  "��ǥ�ܸ� �Է�", MB_ICONQUESTION | MB_YESNO) == IDNO)
			return;

		SetStandardInput();
	}

	UpdateData(FALSE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);
*/}


void CDetHBracingDlg::SetStandardInput()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)		
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);	
		long nCount=0;
		while(pBx)
		{			
			SetDataDefaultSub(nG, nCount, TRUE);
			SetDataCalcHBracing(pBx);
//			SetDataSpliceCalc(nG, nCount);

			pBx = Finder.GetBxNext();
			nCount++;
		}
	}
}
