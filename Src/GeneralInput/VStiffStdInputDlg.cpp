// VStiffStdInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVStiffStdInputDlg dialog


CVStiffStdInputDlg::CVStiffStdInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVStiffStdInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVStiffStdInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Domyun.m_bEnablePopupMenu = TRUE;

}


void CVStiffStdInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVStiffStdInputDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_DOMYUNVIEW, m_Domyun);
	DDX_Control(pDX, IDC_BUTTON_RECOMMEND, m_btnRecom);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CVStiffStdInputDlg, CDialog)
	//{{AFX_MSG_MAP(CVStiffStdInputDlg)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVStiffStdInputDlg message handlers

BOOL CVStiffStdInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CPlateBridgeApp	*pDB  = m_pStd->GetBridge();
	m_dA_p	= pDB->m_dA_p;
	m_dB_p	= pDB->m_dB_p;
	m_dC_p	= pDB->m_dC_p;
	m_dT_p	= pDB->m_dT_p;
	m_dC_n	= pDB->m_dC_n;
	m_dT_n	= pDB->m_dT_n;

	m_Domyun.GetDomyun()->SetCWnd(&m_Domyun);

	SetDataInit();
	DrawInputDomyunView(TRUE);

	return TRUE;
}

void CVStiffStdInputDlg::OnButtonRecommend() 
{
	SetDataDefault();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CVStiffStdInputDlg::SetDataDefault()
{
	CPlateBridgeApp	*pDB  = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(0);  
	// 대표단면을 위한 기본제원은 거더1번의 시작지점
	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetBxOnJijum(0);
	double dWebT = pGir->GetThickJewon(G_W, pBx);

	if(pDB->IsPlateGir())
	{
		m_dA_p = dWebT*5;
		m_dB_p = 20;
		m_dC_p = 150;
		m_dT_p = 12;
		m_dC_n = 150;
		m_dT_n = 12;
	}
	else
	{
		m_dA_p = dWebT*5;
		m_dB_p = 20;
		m_dC_p = 250;
		m_dT_p = 22;
		m_dC_n = 250;
		m_dT_n = 22;
	}
}

void CVStiffStdInputDlg::SetGridTitle()
{
	long	nRows	= 4;
	long	nCols	= 5;
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(82);
	for(long n = 0; n < nCols; n++)
		m_Grid.SetCellFixType(2, n, GVN_ONECELL_ROWFIX);

	m_Grid.SetMergeRow(0, 1, 0);
	m_Grid.SetMergeRow(2, 3, 0);

	m_Grid.SetTextMatrix(0, 0, "일반부");
	m_Grid.SetTextMatrix(0, 1, "A");
	m_Grid.SetTextMatrix(0, 2, "B");
	m_Grid.SetTextMatrix(0, 3, "C");
	m_Grid.SetTextMatrix(0, 4, "T");

	m_Grid.SetTextMatrix(2, 0, "부모멘트부");
	m_Grid.SetTextMatrix(2, 1, "");
	m_Grid.SetTextMatrix(2, 2, "");
	m_Grid.SetTextMatrix(2, 3, "C");
	m_Grid.SetTextMatrix(2, 4, "T");

	m_Grid.ExpandColumnsToFit();
	m_Grid.ExpandRowsToFit();
	m_Grid.SetRedraw(TRUE,TRUE);
}


void CVStiffStdInputDlg::SetDataInit()
{
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();

	SetGridTitle();

	m_Grid.SetTextMatrix(1, 1, "%.0lf", m_dA_p);
	m_Grid.SetTextMatrix(1, 2, "%.0lf", m_dB_p);
	m_Grid.SetTextMatrix(1, 3, "%.0lf", m_dC_p);
	m_Grid.SetTextMatrix(1, 4, "%.0lf", m_dT_p);
	if(pDB->m_nQtyJigan>1)
	{
		m_Grid.SetTextMatrix(3, 3, "%.0lf", m_dC_n);
		m_Grid.SetTextMatrix(3, 4, "%.0lf", m_dT_n);
	}
	m_Grid.ExpandToFit();
	m_Grid.SetRedraw();
}

void CVStiffStdInputDlg::SetDataSave()
{
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();

	m_dA_p = m_Grid.GetTextMatrixDouble(1, 1);
	m_dB_p = m_Grid.GetTextMatrixDouble(1, 2);
	m_dC_p = m_Grid.GetTextMatrixDouble(1, 3);
	m_dT_p = m_Grid.GetTextMatrixDouble(1, 4);
	if(pDB->m_nQtyJigan>1)
	{
		m_dC_n = m_Grid.GetTextMatrixDouble(3, 3);
		m_dT_n = m_Grid.GetTextMatrixDouble(3, 4);
	}
}

void CVStiffStdInputDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp		*pBridgeApp		= m_pStd->GetBridge();
	CPlateGirderApp		*pGir			= pBridgeApp->GetGirder(m_nG);
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();

	m_Domyun.GetDomyun()->ClearEtt(TRUE);
	m_Domyun.GetDomyun()->SetCalcExtRect();
	CDomyun Dom(m_Domyun.GetDomyun());
	
	Dom.SetScaleDim(10);

	DrawInputGenType(&Dom);
	if(pBridgeApp->m_nQtyJigan>1)
	{
		Dom.Move(Dom.Always(-100), 0);
		DrawInputNegaType(&Dom);
		Dom.Move(Dom.Always(100), 0);
	}

	// 화면의 중앙으로 정렬
	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	CDRect  BoundRect;
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	BoundRect.top -= BoundRect.Height()*0.05;
	BoundRect.bottom +=BoundRect.Height() * 0.05;
	Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);	
	Dom.Invalidate();

	*m_Domyun.GetDomyun() << Dom;
	m_Domyun.ZoomAll();
}

void CVStiffStdInputDlg::OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}


void CVStiffStdInputDlg::DrawInputGenType(CDomyun *pDom)
{
	CDomyun Dom(pDom);
	
	CPlateBridgeApp	*pBridgeApp		= m_pStd->GetBridge();
	CPlateGirderApp	*pGir			= pBridgeApp->GetGirder(0);
	CDataManage		*pDataManage	= m_pStd->GetDataManage();
	CARoadOptionStd	*pOptStd		= pDataManage->GetOptionStd();
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(0);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vX(1,0), vY(0,1), xyLeft(1,0);
	CDPointArray xyArrFlange, xyArrWeb, xyArrLVStiff, xyArrRVStiff;
	CDPoint xy45Degree = ToDPointFrRadian(ToRadian(45));
	xyLeft = xy45Degree.Rotate(xy45Degree);
	long n = 0;
	for(n=0; n<HDANLINESU; n++)
	{
		xyArrFlange.Add(CDPoint(0,0));
		xyArrWeb.Add(CDPoint(0,0));
		xyArrLVStiff.Add(CDPoint(0,0));
		xyArrRVStiff.Add(CDPoint(0,0));
	}
	double	dSta	= pBx->GetStation();
	double	dWidth	= pGir->GetWidthFlangeUpper(pBx);
	double	dThickF	= pGir->GetThickJewon(G_F_L, pBx);
	double	dThickW	= pGir->GetThickJewon(G_W, pBx);
	double	dHeight	= pGir->GetHeightGirderByStaAng(dSta)*0.45;	///< 거더 높이 0.45까지만 그린다
	double	dA		= m_dA_p;
	double	dB		= m_dB_p;
	double	dC		= m_dC_p;
	double	d45VLen	= (dC-dB)*tan(45*3.141592/180.0);

	xyArrFlange.SetAt(0, CDPoint(0, 0));
	xyArrFlange.SetAt(1, CDPoint(-dWidth/2, 0));
	xyArrFlange.SetAt(2, CDPoint(-dWidth/2, dThickF));
	xyArrFlange.SetAt(3, CDPoint(0, dThickF));
	xyArrFlange.SetAt(4, CDPoint(dWidth/2, dThickF));
	xyArrFlange.SetAt(5, CDPoint(dWidth/2, 0));
	xyArrFlange.SetAt(6, CDPoint(0, 0));

	xyArrWeb.SetAt(0, CDPoint(-dThickW/2, dThickF));
	xyArrWeb.SetAt(1, CDPoint(-dThickW/2, dThickF+dHeight));
	xyArrWeb.SetAt(2, CDPoint(dThickW/2, dThickF+dHeight));
	xyArrWeb.SetAt(3, CDPoint(dThickW/2, dThickF));
	xyArrWeb.SetAt(4, CDPoint(-dThickW/2, dThickF));

	xyArrLVStiff.SetAt(0, CDPoint(-dThickW/2, dThickF+dA));
	xyArrLVStiff.SetAt(1, CDPoint(-dThickW/2-dB, dThickF+dA));
	xyArrLVStiff.SetAt(2, CDPoint(-dThickW/2-dC, dThickF+dA+d45VLen));
	xyArrLVStiff.SetAt(3, CDPoint(-dThickW/2-dC, dThickF+dHeight));

	xyArrRVStiff.SetAt(0, CDPoint(dThickW/2, dThickF+dA));
	xyArrRVStiff.SetAt(1, CDPoint(dThickW/2+dB, dThickF+dA));
	xyArrRVStiff.SetAt(2, CDPoint(dThickW/2+dC, dThickF+dA+d45VLen));
	xyArrRVStiff.SetAt(3, CDPoint(dThickW/2+dC, dThickF+dHeight));
	for(n=0; n<6; n++)
		Dom.LineTo(xyArrFlange.GetAt(n), xyArrFlange.GetAt(n+1));
	for(n=0; n<4; n++)
		Dom.LineTo(xyArrWeb.GetAt(n), xyArrWeb.GetAt(n+1));
	for(n=0; n<3; n++)
	{
		Dom.LineTo(xyArrLVStiff.GetAt(n), xyArrLVStiff.GetAt(n+1));
		Dom.LineTo(xyArrRVStiff.GetAt(n), xyArrRVStiff.GetAt(n+1));
	}

	*pDom << Dom;

	///< 치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextHeight(5);
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(0, dThickF, 0);
	Dom.DimLineToExtend(dA, 0, COMMA(dA), "A");

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-dThickW/2, dThickF+dA, 0);
	Dom.DimLineToOverExtend(-dB, 0, COMMA(dB), "B", 2);

	Dom.DimMoveTo(-dThickW/2, dThickF+dHeight*3/5, 0);
	Dom.DimLineToOverExtend(-dC, 0, COMMA(dC), "C", 2);

	Dom.SetDirection("RIGHT");
	Dom.DimArrowTo(dThickW/2+dC/2+Dom.Always(20), dThickF+dHeight/4, Dom.Always(20), 4);
	Dom.TextOut(dThickW/2+dC/2+Dom.Always(12), dThickF+dHeight/4+Dom.GetTextHeight()/2, "V-STIFF");
	Dom.CutLightning(-dWidth/2-dWidth*0.2, dThickF+dHeight, dWidth/2+dWidth*0.2, dThickF+dHeight, FALSE);

	Dom.TextOut(-dWidth/2-Dom.Always(5), dThickF+dHeight/2, "T");
	Dom.TextOut(0, dThickF+dHeight+Dom.Always(5), "일반부");

	*pDom << Dom;
}

void CVStiffStdInputDlg::DrawInputNegaType(CDomyun *pDom)
{
	CDomyun Dom(pDom);
	
	CPlateBridgeApp	*pBridgeApp		= m_pStd->GetBridge();
	CPlateGirderApp	*pGir			= pBridgeApp->GetGirder(0);
	CDataManage		*pDataManage	= m_pStd->GetDataManage();
	CARoadOptionStd	*pOptStd		= pDataManage->GetOptionStd();
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(0);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vX(1,0), vY(0,1), xyLeft(1,0);
	CDPointArray xyArrFlange, xyArrWeb, xyArrLVStiff, xyArrRVStiff;
	CDPoint xy45Degree = ToDPointFrRadian(ToRadian(45));
	xyLeft = xy45Degree.Rotate(xy45Degree);
	long n = 0;
	for(n=0; n<HDANLINESU; n++)
	{
		xyArrFlange.Add(CDPoint(0,0));
		xyArrWeb.Add(CDPoint(0,0));
		xyArrLVStiff.Add(CDPoint(0,0));
		xyArrRVStiff.Add(CDPoint(0,0));
	}
	double	dSta	= pBx->GetStation();
	double	dWidth	= pGir->GetWidthFlangeUpper(pBx);
	double	dThickF	= pGir->GetThickJewon(G_F_L, pBx);
	double	dThickW	= pGir->GetThickJewon(G_W, pBx);
	double	dHeight	= pGir->GetHeightGirderByStaAng(dSta)*0.45;	///< 거더 높이 0.45까지만 그린다
	double	dC		= m_dC_n;

	xyArrFlange.SetAt(0, CDPoint(0, 0));
	xyArrFlange.SetAt(1, CDPoint(-dWidth/2, 0));
	xyArrFlange.SetAt(2, CDPoint(-dWidth/2, dThickF));
	xyArrFlange.SetAt(3, CDPoint(0, dThickF));
	xyArrFlange.SetAt(4, CDPoint(dWidth/2, dThickF));
	xyArrFlange.SetAt(5, CDPoint(dWidth/2, 0));
	xyArrFlange.SetAt(6, CDPoint(0, 0));

	xyArrWeb.SetAt(0, CDPoint(-dThickW/2, dThickF));
	xyArrWeb.SetAt(1, CDPoint(-dThickW/2, dThickF+dHeight));
	xyArrWeb.SetAt(2, CDPoint(dThickW/2, dThickF+dHeight));
	xyArrWeb.SetAt(3, CDPoint(dThickW/2, dThickF));
	xyArrWeb.SetAt(4, CDPoint(-dThickW/2, dThickF));

	xyArrLVStiff.SetAt(0, CDPoint(-dThickW/2-dC, dThickF));
	xyArrLVStiff.SetAt(1, CDPoint(-dThickW/2-dC, dThickF+dHeight));

	xyArrRVStiff.SetAt(0, CDPoint(dThickW/2+dC, dThickF));
	xyArrRVStiff.SetAt(1, CDPoint(dThickW/2+dC, dThickF+dHeight));

	for(n=0; n<6; n++)
		Dom.LineTo(xyArrFlange.GetAt(n), xyArrFlange.GetAt(n+1));
	for(n=0; n<4; n++)
		Dom.LineTo(xyArrWeb.GetAt(n), xyArrWeb.GetAt(n+1));
	for(n=0; n<1; n++)
	{
		Dom.LineTo(xyArrLVStiff.GetAt(n), xyArrLVStiff.GetAt(n+1));
		Dom.LineTo(xyArrRVStiff.GetAt(n), xyArrRVStiff.GetAt(n+1));
	}

	*pDom << Dom;

	///< 치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextHeight(5);

	Dom.DimMoveTo(-dThickW/2, dThickF+dHeight*3/5, 0);
	Dom.DimLineToOverExtend(-dC, 0, COMMA(dC), "C", 2);

	Dom.SetDirection("RIGHT");
	Dom.DimArrowTo(dThickW/2+dC/2+Dom.Always(20), dThickF+dHeight/4, Dom.Always(20), 4);
	Dom.TextOut(dThickW/2+dC/2+Dom.Always(12), dThickF+dHeight/4+Dom.GetTextHeight()/2, "V-STIFF");
	Dom.CutLightning(-dWidth/2-dWidth*0.2, dThickF+dHeight, dWidth/2+dWidth*0.2, dThickF+dHeight, FALSE);

	Dom.TextOut(-dC-Dom.Always(5), dThickF+dHeight/2, "T");
	Dom.TextOut(0, dThickF+dHeight+Dom.Always(5), "부모멘트부");

	*pDom << Dom;
}

void CVStiffStdInputDlg::OnOK() 
{
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();

	pDB->m_dA_p	= m_dA_p;
	pDB->m_dB_p	= m_dB_p;
	pDB->m_dC_p	= m_dC_p;
	pDB->m_dT_p	= m_dT_p;
	pDB->m_dC_n	= m_dC_n;
	pDB->m_dT_n	= m_dT_n;
	
	CDialog::OnOK();
}

void CVStiffStdInputDlg::OnCancel() 
{
	CDialog::OnCancel();
}
