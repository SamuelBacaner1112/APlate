// BindConcJewonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBindConcJewonDlg dialog


CBindConcJewonDlg::CBindConcJewonDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: CDialog(CBindConcJewonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBindConcJewonDlg)
	//}}AFX_DATA_INIT

	m_pStd = pStd;
	m_pParent = (CHDanDlg *)pParent;
	m_Domyun.m_bEnablePopupMenu = TRUE;
	m_Domyun.GetDomyun()->SetCWnd(&m_Domyun);
}


void CBindConcJewonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBindConcJewonDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_BINDCONC_DOMYUN, m_Domyun);
}


BEGIN_MESSAGE_MAP(CBindConcJewonDlg, CDialog)
	//{{AFX_MSG_MAP(CBindConcJewonDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_APPLY1, OnApplyJewon)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBindConcJewonDlg message handlers
BOOL CBindConcJewonDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_MOUSEWHEEL)
		m_Domyun.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	
	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CBindConcJewonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CPlateBridgeApp *pDB		= m_pStd->GetBridge();
	m_dGirH		= pDB->GetGirder(0)->GetHeightGirderByJijum(0,FALSE)+pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDis;
	SetDataInit();
	
	return TRUE;
}

void CBindConcJewonDlg::OnOK() 
{	
	SetDataSave();
	m_pParent->SetDataInit();
	m_pParent->DrawInputDomyunView();	
	CDialog::OnOK();
}

void CBindConcJewonDlg::OnApplyJewon() 
{
	SetDataSave();
	DrawBindConc();
	m_pParent->SetDataInit();
	m_pParent->DrawInputDomyunView();
}

void CBindConcJewonDlg::OnDefault() 
{
	SetDataDefault();
	SetDataInit();
	DrawBindConc();
}

void CBindConcJewonDlg::SetGridTitle()
{
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetRowCount(8);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));

	m_Grid.SetTextMatrix(0,0,"항 목");
	m_Grid.SetTextMatrix(0,1,"치수 (mm)");

	m_Grid.SetTextMatrix(1,0,"T   : 슬래브 두께",DT_LEFT,FALSE);
	m_Grid.SetTextMatrix(2,0,"b   : 구속콘크리트 폭",DT_LEFT,FALSE);
	m_Grid.SetTextMatrix(3,0,"h   : 구속콘크리트 두께",DT_LEFT,FALSE);
	m_Grid.SetTextMatrix(4,0,"S1 : 구속콘크리트 매입깊이",DT_LEFT,FALSE);
	m_Grid.SetTextMatrix(5,0,"S2 : 주형 매입깊이 (플랜지 상면~구속콘 하면)",DT_LEFT,FALSE);
	m_Grid.SetTextMatrix(6,0,"H   : 교량높이 (슬래브 상면 ~ 거더 하면)",DT_LEFT,FALSE);
	m_Grid.SetTextMatrix(7,0,"H'  : 거더높이 (H-T+S1)",DT_LEFT,FALSE);
	m_Grid.SetItemState(7, 1, GVIS_READONLY);
}

void CBindConcJewonDlg::SetDataInit()
{
	SetGridTitle();

	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	m_Grid.SetTextMatrix(1, 1, "%g", pDB->m_dThickSlabBase);
	m_Grid.SetTextMatrix(2, 1, "%g", pDB->m_BindConc.m_dWidth);
	m_Grid.SetTextMatrix(3, 1, "%g", pDB->m_BindConc.m_dHeight);
	m_Grid.SetTextMatrix(4, 1, "%g", pDB->m_BindConc.m_dDeep);
	m_Grid.SetTextMatrix(5, 1, "%g", pDB->m_BindConc.m_dDis);
	m_Grid.SetTextMatrix(6, 1, "%g", m_dGirH+pDB->m_dThickSlabBase-pDB->m_BindConc.m_dDeep);
	m_Grid.SetTextMatrix(7, 1, "%g", m_dGirH);

	m_Grid.SetRedraw(TRUE,TRUE);
	m_Grid.ExpandToFit();

	DrawBindConc();
}

void CBindConcJewonDlg::SetDataSave()
{
	UpdateData(TRUE);
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	pDB->m_dThickSlabBase		= m_Grid.GetTextMatrixDouble(1, 1);
	pDB->m_BindConc.m_dWidth	= m_Grid.GetTextMatrixDouble(2, 1);
	pDB->m_BindConc.m_dHeight	= m_Grid.GetTextMatrixDouble(3, 1);
	pDB->m_BindConc.m_dDeep		= m_Grid.GetTextMatrixDouble(4, 1);
	pDB->m_BindConc.m_dDis		= m_Grid.GetTextMatrixDouble(5, 1);
	m_dGirH						= m_Grid.GetTextMatrixDouble(6, 1) -pDB->m_dThickSlabBase+pDB->m_BindConc.m_dDeep;
	SetGirderHeight();
}

void CBindConcJewonDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(pDB->IsTUGir())
	{
		pDB->m_dThickSlabBase		= 300;	// 300;
		pDB->m_BindConc.m_dWidth	= 1600;	// 2500;
		pDB->m_BindConc.m_dHeight	= 450;	// 200;
		pDB->m_BindConc.m_dDeep		= 36;	// 36;
		pDB->m_BindConc.m_dDis		= 225;	// 100;
		m_dGirH						= 2436;	// 2000;
		SetGirderHeight();
	}
	else
	{
		pDB->m_BindConc.m_dWidth	= 0;
		pDB->m_BindConc.m_dHeight	= 0;
		pDB->m_BindConc.m_dDeep		= 0;
		pDB->m_BindConc.m_dDis		= 0;
		m_dGirH						= -1;
	}
}

void CBindConcJewonDlg::DrawBindConc()
{
	CPlateBridgeApp *pDB		= m_pStd->GetBridge();
	CARoadOptionStd	*pOptStd	= m_pStd->GetDataManage()->GetOptionStd();
	CAPlateDrawDanmyun DrawDanmyun(m_pStd->GetDataManage());

	m_Domyun.GetDomyun()->ClearEtt(TRUE);
	m_Domyun.GetDomyun()->SetCalcExtRect();
	CDomyun Dom(m_Domyun.GetDomyun());
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	long nG = pDB->GetQtyGirder() > 2 ? 1 : 0;
	CPlateGirderApp	*pGir= pDB->GetGirder(nG);
	double dGirderCenter = pGir->m_dGirderCenter;

	DrawDanmyun.DrawHDanGirderForInput(&Dom, 0);
	Dom.SetCalcExtRect();
	CDRect  rectTrim = Dom.GetExtRect();
	rectTrim.left	= dGirderCenter-pDB->m_BindConc.m_dWidth/2;
	rectTrim.right	= dGirderCenter+pDB->m_BindConc.m_dWidth/2;
	Dom.TrimByRect(rectTrim, FALSE);
	
	rectTrim.left -= Dom.Always(400);
	rectTrim.right+= Dom.Always(400);
	rectTrim.bottom += Dom.Always(pDB->m_dThickSlabBase+10);

	DrawDanmyun.DrawHDanSlabForInput(&Dom);
	Dom.TrimByRect(rectTrim, FALSE);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.CutLightningAnyDir(CDPoint(rectTrim.left, -300), CDPoint(rectTrim.left, -300+pDB->m_dThickSlabBase), 50);
	Dom.CutLightningAnyDir(CDPoint(rectTrim.right, -300), CDPoint(rectTrim.right, -300+pDB->m_dThickSlabBase), 50);
	
	DrawDanmyun.DimBindConcJewonForInput(&Dom, 0);

	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	Dom.SetCalcExtRect();
	rectTrim = Dom.GetExtRect();
	if(rectTrim.Width() > rectTrim.Height())
		rectTrim.left -= Dom.Always(100);
	Dom.Rectangle(rectTrim);

	*m_Domyun.GetDomyun() << Dom;
	m_Domyun.ZoomAll();
}

void CBindConcJewonDlg::OnCellChangedData(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    long nRow = GridView->iRow;
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(nRow == 6)
		m_dGirH = m_Grid.GetTextMatrixDouble(6, 1) -pDB->m_dThickSlabBase+pDB->m_BindConc.m_dDeep;		

	SetDataSave();
	SetDataInit();
	DrawBindConc();
}

void CBindConcJewonDlg::OnClose() 
{
	CDialog::OnClose();
}

void CBindConcJewonDlg::SetGirderHeight()
{
	CPlateBridgeApp *pDB		= m_pStd->GetBridge();

	if(m_dGirH < 0) return;

	double dGirH = m_dGirH-pDB->m_BindConc.m_dHeight+pDB->m_BindConc.m_dDis;

	for(long nJijum = 0; nJijum < pDB->m_nQtyJigan + 1; nJijum++)
	{
		for(long nGir = -1; nGir < pDB->GetGirdersu(); nGir++)
		{			
			CPlateGirderApp *pGir = pDB->GetGirder(nGir);
			
			pGir->SetHeightGirderAtJijum(nJijum,dGirH);
		}
	}
}
