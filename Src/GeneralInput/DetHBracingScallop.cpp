// DetHBracingScallop.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetHBracingScallop.h"
#include "../APlateDBDraw/APlateDBDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetHBracingScallop dialog


CDetHBracingScallop::CDetHBracingScallop(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetHBracingScallop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetHBracingScallop)
	m_bApplyDefault = FALSE;
	m_pStd			= pStd;
	//}}AFX_DATA_INIT
}

void CDetHBracingScallop::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetHBracingScallop)
	DDX_GridControl(pDX, IDC_GRID_HVRACING, m_Grid_HVracing);
	DDX_GridControl(pDX, IDC_GRID_SCALLOP, m_Grid_Scallop);
	DDX_GridControl(pDX, IDC_GRID_FIELDSCALLOP, m_Grid_FieldScallop);
	DDX_Check(pDX, IDC_CHECK_DEFAULT_APPLY, m_bApplyDefault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetHBracingScallop, TStackedPage)
	//{{AFX_MSG_MAP(CDetHBracingScallop)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT_APPLY, OnCheckApplyDefaultSlot)
	//}}AFX_MSG_MAP
//	ON_NOTIFY(GVN_CELL_CHANGED		, IDC_GRID_HVRACING, OnCellChanged_HVracing)

	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID_HVRACING, OnCellChangedData_HVracing)
	ON_NOTIFY(GVN_CELL_CHANGED_COL  , IDC_GRID_HVRACING, OnCellChangedCol_HVracing)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA  , IDC_GRID_SCALLOP, OnCellChangedData_SCallop)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA  , IDC_GRID_FIELDSCALLOP, OnCellChangedData_FieldSCallop)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetHBracingScallop message handlers
BOOL CDetHBracingScallop::OnInitDialog() 
{	
	TStackedPage::OnInitDialog();

//	DrawInputDomyunView(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDetHBracingScallop::DrawInputDomyunView(BOOL bZoomAll)
{
	CAPlateDrawHBracing DrawHBracing(m_pStd->m_pDataManage);

	CDomyun *pDom = m_pStd->m_pDataManage->GetDomyun();
	pDom->ClearEtt(TRUE);
	//
	CDomyun Dom(pDom);
	Dom.SetScaleDim(7.5);
	double dThickSplice = 0;
	if(m_nCol==0)			dThickSplice = 10;
	else if(m_nCol==1)		dThickSplice = 14;
	else if(m_nCol==2)		dThickSplice = 24;

	DrawHBracing.DrawInputHBracingScallop(&Dom, dThickSplice);
	Dom.Move(800,200);
	*pDom << Dom;

	DrawFieldWeld(&Dom);
	DimFieldWeld(&Dom);
	Dom.Move(400,500);
	*pDom << Dom;

	DrawChamfer(&Dom);
	DimChamfer(&Dom);
	Dom.Move(-300,0);
	*pDom << Dom;

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();

}

void CDetHBracingScallop::DrawChamfer(CDomyun *pDomP)
{
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	
	double T1 = 12;
	double T2 = 12;
	double height = 700;
	double Length = 200;
	double C = 15*2;

	CDomyun *pDom = new CDomyun(pDomP);
	pDom->SetScaleDim(10);
	pOptStd->SetEnvType(pDom, HCAD_STLC);

	pDom->MoveTo(0,0);
	pDom->LineToEx(-T1/2, 0);
	pDom->LineToEx(0, height);
	pDom->MoveTo(0,0);
	pDom->LineToEx(T1/2, 0);
	pDom->LineToEx(0, height);

	pDom->Rectangle(-Length, 0, Length, - T2);
//
	pDom->MoveTo(-Length + C, 0);
	pDom->LineToEx(Length - C*2 - T1/2, 0);
	pDom->LineToEx(C, C);
	pDom->LineToEx(0, height-C);
	
	pDom->MoveTo(-Length + C, 0);
	pDom->LineToEx(-C, C);
	pDom->LineToEx(0, height-C);
//
	pDom->MoveTo(Length - C, 0);
	pDom->LineToEx(-Length + C*2 + T1/2, 0);
	pDom->LineToEx(-C, C);
	pDom->LineToEx(0, height-C);
	
	pDom->MoveTo(Length - C, 0);
	pDom->LineToEx(C, C);
	pDom->LineToEx(0, height-C);

	pOptStd->SetEnvType(pDom, HCAD_CUTL);
	
	pDom->CutLightning( -Length, height,  Length, height,FALSE,1);

	*pDomP << *pDom;
	delete pDom;

}

void CDetHBracingScallop::DimChamfer(CDomyun *pDomP)
{
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	
	double T1 = 12;
	double height = 1000;
	double Length = 200;
	double C = 15*2;

	CDimDomyunEx *pDom = new CDimDomyunEx(pDomP);
	pDom->SetScaleDim(10);
	pOptStd->SetEnvType(pDom, HCAD_DIML);

	CString str;
	str = "보강재 (t = 두께)";
	double len = pDom->GetTextWidth(str);
	pDom->SetDirection("TOP");
	pDom->CircleArrow(Length/2, height/1.8, len, len, 0,str,"");
	*pDomP << *pDom;

	CDPoint xy, vDir;
	vDir = CDPoint(1,1);
	str = "  C  ";
	len = pDom->GetTextWidth(str);
	xy = CDPoint(T1/2, 0) + C / sqrt(2.0) * vDir.Unit();
	pDom->GiSiArrowExtend(xy.x, xy.y, len, len, 0,str, "");
	pDom->Rotate(xy, vDir);
	*pDomP << *pDom;
	delete pDom;

}
//현장 용접 스켈럼 상세...그리기...

void CDetHBracingScallop::DrawFieldWeld(CDomyun *pDomP)
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	CPlateGirderApp  *pGir = pBridge->GetGirder(0);
	CPlateBasicIndex *pBx  = (CPlateBasicIndex*)pGir->GetBxOnJijum(0);
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	CAPlateSymbolDom Dom(pDomP,m_pStd->GetDataManage());
		
	double T1 = 24;	
	double height = 500;
	double Length = 200;
	double R1 = 50;
	CDPoint BasePt(0,0);

	
	Dom.SetScaleDim(10);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	Dom.MoveTo(BasePt.x - Length, BasePt.y);
	Dom.LineToEx(Length*2, BasePt.y);
	Dom.MoveTo(BasePt.x - Length, BasePt.y-T1);
	Dom.LineToEx(Length*2, BasePt.y);
	Dom.MoveTo(BasePt);
	Dom.LineTo(BasePt.x, BasePt.y-T1);

	Dom.Arc(BasePt.x, BasePt.y-T1, R1, 180.0, 180.0);
	
	Dom.MoveTo(BasePt.x, BasePt.y-T1-R1);
	Dom.LineTo(BasePt.x, BasePt.y-height);	
	
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.DrawWeldMark(BasePt, G_F_U, G_F_U, pBx, -1, 12, 0, 0, TRUE, 0.0, FALSE, "", -1, FALSE, TRUE,TRUE, 2);		

	*pDomP << Dom;
}

void CDetHBracingScallop::DimFieldWeld(CDomyun *pDomP)
{
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	
	double T1 = 24;	
	double R1 = 50;
	CDPoint BasePt(0,0);

	CDimDomyunEx *pDom = new CDimDomyunEx(pDomP);
	pDom->SetScaleDim(10);
	pOptStd->SetEnvType(pDom, HCAD_DIML);
	pDom->SetDirection("LEFT");

	CDPoint xy, vDir;
	vDir = CDPoint(1,-1);
	CString str = "R40~R50";
	long len = (long)pDom->GetTextWidth(str);
	
	xy = CDPoint(BasePt.x-T1,BasePt.y-T1-R1);// + vDir.Unit();
	pDom->GiSiArrowExtend(xy.x, xy.y, len+pDom->Always(10), len+pDom->Always(3), 6, str, "");
	pDom->Rotate(xy, vDir);
	*pDomP << *pDom;
	delete pDom;

}

void CDetHBracingScallop::SetGridTitle_HVracing()
{
	long nColCount = 4;
	long nRowCount = 3;//압축측 상세 없애기.... 06.12.21....KB

	m_Grid_HVracing.SetEditable(TRUE);
	m_Grid_HVracing.SetListMode(FALSE);
	m_Grid_HVracing.EnableDragAndDrop(TRUE);
	m_Grid_HVracing.EnableInsertRow(FALSE);
	m_Grid_HVracing.EnableInsertCol(FALSE);
//	m_Grid_HVracing.SetTextBkColor(RGBEDITABLE);
	m_Grid_HVracing.SetFixedRowCount(1);
	m_Grid_HVracing.SetFixedColumnCount(1);
	m_Grid_HVracing.EnableReturnPass(TRUE);

	m_Grid_HVracing.SetRowCount(nRowCount);
	m_Grid_HVracing.SetColumnCount(nColCount);

	m_Grid_HVracing.SetRowHeightAll(20);
	m_Grid_HVracing.SetRowHeight(0, 20);	

	m_Grid_HVracing.SetColumnWidthAll(120);
	m_Grid_HVracing.SetColumnWidth(0, 50);

	char *szColArr[] = { "구분", "t ≤ 12", "12 ＜ t ≤ 22", "22 ＜ t ≤ 32" };
	//char *szRowArr[] = { "구분", "A", "B", "R", "R2"};
	char *szRowArr[] = { "구분", "A", "R2"};

	for(long nCol = 0; nCol < nColCount; nCol++) 
		m_Grid_HVracing.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid_HVracing.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid_HVracing.SetFocusCell(1,1);
	m_Grid_HVracing.SetRedraw(TRUE,TRUE);
}

void CDetHBracingScallop::SetGridTitle_Scallop()
{
	long nColCount = 3;
	long nRowCount = 3;

	m_Grid_Scallop.SetEditable(TRUE);
	m_Grid_Scallop.SetListMode(FALSE);
	m_Grid_Scallop.EnableDragAndDrop(TRUE);
	m_Grid_Scallop.EnableInsertRow(FALSE);
	m_Grid_Scallop.EnableInsertCol(FALSE);
	m_Grid_Scallop.SetFixedRowCount(1);
	m_Grid_Scallop.SetFixedColumnCount(1);
	m_Grid_Scallop.EnableReturnPass(TRUE);

	m_Grid_Scallop.SetRowCount(nRowCount);
	m_Grid_Scallop.SetColumnCount(nColCount);

	m_Grid_Scallop.SetRowHeightAll(20);
	m_Grid_Scallop.SetRowHeight(0, 20);	

	m_Grid_Scallop.SetColumnWidthAll(120);
	m_Grid_Scallop.SetColumnWidth(0, 50);

	char *szColArr[] = { "구분", "t ≤ 16", "t ＞ 16" };
	char *szRowArr[] = { "구분", "R", "C" };

	for(long nCol = 0; nCol < nColCount; nCol++) 
		m_Grid_Scallop.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid_Scallop.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid_Scallop.SetRedraw(TRUE,TRUE);	

}

void CDetHBracingScallop::SetGridTitle_FieldScallop()
{
	long nColCount = 3;
	long nRowCount = 3;

	m_Grid_FieldScallop.SetEditable(TRUE);
	m_Grid_FieldScallop.SetListMode(FALSE);
	m_Grid_FieldScallop.EnableDragAndDrop(TRUE);
	m_Grid_FieldScallop.EnableInsertRow(FALSE);
	m_Grid_FieldScallop.EnableInsertCol(FALSE);
	m_Grid_FieldScallop.SetFixedRowCount(1);
	m_Grid_FieldScallop.SetFixedColumnCount(1);
	m_Grid_FieldScallop.EnableReturnPass(TRUE);

	m_Grid_FieldScallop.SetRowCount(nRowCount);
	m_Grid_FieldScallop.SetColumnCount(nColCount);

	m_Grid_FieldScallop.SetRowHeightAll(20);
	m_Grid_FieldScallop.SetRowHeight(0, 20);	

	m_Grid_FieldScallop.SetColumnWidthAll(120);
	m_Grid_FieldScallop.SetColumnWidth(0, 50);
	

	char *szColArr[] = { "구분", "복부판t < 기준t", "복부판t ≥ 기준t" };
	char *szRowArr[] = { "구분", "R", "기준t" };

	for(long nCol = 0; nCol < nColCount; nCol++) 
		m_Grid_FieldScallop.SetTextMatrix(0, nCol, szColArr[nCol]);

	for(long nRow = 1; nRow < nRowCount; nRow++)
		m_Grid_FieldScallop.SetTextMatrix(nRow, 0, szRowArr[nRow]);

	m_Grid_FieldScallop.SetRedraw(TRUE,TRUE);	

}
void CDetHBracingScallop::SetGridTitle()
{
	SetGridTitle_HVracing();
	SetGridTitle_Scallop();
	SetGridTitle_FieldScallop();
}

	
void CDetHBracingScallop::SetDataInit()
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	CString str(_T(""));
	SetGridTitle();

	//횡브레이싱 스켈럽 상세..
	m_bApplyDefault  = pBridge->m_bApplyDefaultSlot;

	str.Format("%g", pBridge->m_dSlotA[0]);	m_Grid_HVracing.SetTextMatrix(1	, 1, str);
	str.Format("%g", pBridge->m_dSlotA[1]);	m_Grid_HVracing.SetTextMatrix(1	, 2, str);
	str.Format("%g", pBridge->m_dSlotA[2]);	m_Grid_HVracing.SetTextMatrix(1	, 3, str);

	str.Format("%g", pBridge->m_dSlotR2[0]);	m_Grid_HVracing.SetTextMatrix(2	, 1, str);
	str.Format("%g", pBridge->m_dSlotR2[1]);	m_Grid_HVracing.SetTextMatrix(2	, 2, str);
	str.Format("%g", pBridge->m_dSlotR2[2]);	m_Grid_HVracing.SetTextMatrix(2	, 3, str);
	//	
	long nRow = 1;	
	str.Format("%g", pBridge->GetGirder(-1)->m_pSangse->m_Scallop_R1);	m_Grid_Scallop.SetTextMatrix(nRow	, 1, str);
	str.Format("%g", pBridge->GetGirder(-1)->m_pSangse->m_Scallop_R2);	m_Grid_Scallop.SetTextMatrix(nRow	, 2, str);
	str.Format("%g", pBridge->GetGirder(-1)->m_pSangse->m_Chamfer_C1);	m_Grid_Scallop.SetTextMatrix(nRow+1	, 1, str);
	str.Format("%g", pBridge->GetGirder(-1)->m_pSangse->m_Chamfer_C2);	m_Grid_Scallop.SetTextMatrix(nRow+1	, 2, str);

	nRow = 1;	
	str.Format("%g", pBridge->m_FieldScallop_R1);	m_Grid_FieldScallop.SetTextMatrix(nRow	, 1, str);
	str.Format("%g", pBridge->m_FieldScallop_R2);	m_Grid_FieldScallop.SetTextMatrix(nRow	, 2, str);
	str.Format("%g", pBridge->m_FieldScallop_BaseT);	m_Grid_FieldScallop.SetTextMatrix(nRow+1	, 1, str);
	m_Grid_FieldScallop.SetMergeCol(nRow+1,1,2);

	m_Grid_HVracing.SetRedraw(TRUE,TRUE);
	m_Grid_Scallop.SetRedraw(TRUE,TRUE);
	m_Grid_FieldScallop.SetRedraw(TRUE,TRUE);
}

void CDetHBracingScallop::SetDataDefault()
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	//횡브레이싱 스켈럽 상세..
	m_pStd->GetBridge()->m_dSlotA[0] = 70;
	m_pStd->GetBridge()->m_dSlotB[0] = 50;
	m_pStd->GetBridge()->m_dSlotR[0] = 35;
	m_pStd->GetBridge()->m_dSlotR2[0] = 20;
	m_pStd->GetBridge()->m_dSlotA[1] = 80;
	m_pStd->GetBridge()->m_dSlotB[1] = 55;
	m_pStd->GetBridge()->m_dSlotR[1] = 35;
	m_pStd->GetBridge()->m_dSlotR2[1] = 20;
	m_pStd->GetBridge()->m_dSlotA[2] = 90;
	m_pStd->GetBridge()->m_dSlotB[2] = 65;
	m_pStd->GetBridge()->m_dSlotR[2] = 35;
	m_pStd->GetBridge()->m_dSlotR2[2] = 25;

	m_pStd->GetBridge()->m_bApplyDefaultSlot = FALSE;

	//스켈럽 상세...
	for(long n=-1; n<pBridge->GetGirdersu(); n++)
	{
		pBridge->GetGirder(n)->m_pSangse->m_Scallop_R1 = 35;	//16보다 작을때
		pBridge->GetGirder(n)->m_pSangse->m_Scallop_R2 = 40;	//16보다 클때
		pBridge->GetGirder(n)->m_pSangse->m_Chamfer_C1 = 10;	//16보다 작을때
		pBridge->GetGirder(n)->m_pSangse->m_Chamfer_C2 = 15;	//16보다 클때
	}

	pBridge->m_FieldScallop_BaseT = 25.0;
	pBridge->m_FieldScallop_R1 = 40.0;
	pBridge->m_FieldScallop_R2 = 50.0;

	m_Grid_HVracing.SetRedraw(TRUE,TRUE);
	m_Grid_Scallop.SetRedraw(TRUE,TRUE);
	m_Grid_FieldScallop.SetRedraw(TRUE,TRUE);
}

void CDetHBracingScallop::SetDataSave()
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	
	//횡브레이싱 스켈럽 상세..
	pBridge->m_dSlotA[0] = atof(m_Grid_HVracing.GetItemText(1, 1));
	pBridge->m_dSlotA[1] = atof(m_Grid_HVracing.GetItemText(1, 2));
	pBridge->m_dSlotA[2] = atof(m_Grid_HVracing.GetItemText(1, 3));

	pBridge->m_dSlotR2[0] = atof(m_Grid_HVracing.GetItemText(2, 1));
	pBridge->m_dSlotR2[1] = atof(m_Grid_HVracing.GetItemText(2, 2));
	pBridge->m_dSlotR2[2] = atof(m_Grid_HVracing.GetItemText(2, 3));

	//스켈럽 상세...
	for(long n=-1; n<pBridge->GetGirdersu(); n++)
	{
		pBridge->GetGirder(n)->m_pSangse->m_Scallop_R1 = atof(m_Grid_Scallop.GetItemText(1,1));	//16보다 작을때
		pBridge->GetGirder(n)->m_pSangse->m_Scallop_R2 = atof(m_Grid_Scallop.GetItemText(1,2));	//16보다 클때
		pBridge->GetGirder(n)->m_pSangse->m_Chamfer_C1 = atof(m_Grid_Scallop.GetItemText(2,1));	//16보다 작을때
		pBridge->GetGirder(n)->m_pSangse->m_Chamfer_C2 = atof(m_Grid_Scallop.GetItemText(2,2));	//16보다 클때
	}

	pBridge->m_FieldScallop_R1 = atof(m_Grid_FieldScallop.GetItemText(1,1));
	pBridge->m_FieldScallop_R2 = atof(m_Grid_FieldScallop.GetItemText(1,2));
	pBridge->m_FieldScallop_BaseT = atof(m_Grid_FieldScallop.GetItemText(2,1));
}

///////////////////////////////////////////////////////////////////////////////
void CDetHBracingScallop::OnCellChangedData_HVracing(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetHBracingScallop::OnCellChangedData_SCallop(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetHBracingScallop::OnCellChangedData_FieldSCallop(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetHBracingScallop::OnCursorChange(long nRow, long nCol)
{
	m_pView->GetDomyun()->STMakeCursor(m_Grid_HVracing.GetItemText(nRow, 0));
}

void CDetHBracingScallop::OnCheckApplyDefaultSlot() 
{
	m_bApplyDefault = !m_bApplyDefault;
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	pBridge->m_bApplyDefaultSlot = m_bApplyDefault;

	UpdateData(FALSE);

	DrawInputDomyunView();		
}

void CDetHBracingScallop::OnCellChangedCol_HVracing(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;

	if(nCol>0)
	{
		m_nCol = nCol-1;
		DrawInputDomyunView(TRUE);
	}
}

void CDetHBracingScallop::OnPreInitDialog()
{
 	SetResize(IDC_GRID_SCALLOP,			SZ_TOP_LEFT, SZ_TOP_RIGHT); 	
 	SetResize(IDC_GRID_FIELDSCALLOP,	SZ_TOP_LEFT, SZ_TOP_RIGHT);	
	SetResize(IDC_GRID_HVRACING,		SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_STATIC_01,			SZ_TOP_LEFT, SZ_TOP_RIGHT);	
 	SetResize(IDC_STATIC_02,			SZ_TOP_LEFT, SZ_TOP_RIGHT);
 	SetResize(IDC_STATIC_03,			SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetGridTitle();
}

// BOOL CDetHBracingScallop::IsValid() 
// {
// 	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
// 
// 	if(pBridge->GetBridgeType()==SOSU_GIR)	// 소수주형
// 		return FALSE;
// 	else
// 		return TRUE;
// }
