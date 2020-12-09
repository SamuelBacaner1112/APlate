// CalcCantileverDeadLoad.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverDeadLoad dialog


CCalcCantileverDeadLoad::CCalcCantileverDeadLoad(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcCantileverDeadLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverDeadLoad)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCalcCantileverDeadLoad::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverDeadLoad)
	DDX_Control(pDX, IDC_TAB_DESIGN, m_ctlTab);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);

}


BEGIN_MESSAGE_MAP(CCalcCantileverDeadLoad, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverDeadLoad)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DESIGN, OnSelchangeTabDesign)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverDeadLoad message handlers
void CCalcCantileverDeadLoad::SetDataInit()
{
	SetGridTitle();

	m_Grid.ExpandColumnsToFit();
	m_Grid.SetRedraw(TRUE,TRUE);
	SetDataDefault();
}

void CCalcCantileverDeadLoad::SetDataDefault()
{

}

void CCalcCantileverDeadLoad::SetDataSave()
{

}

void CCalcCantileverDeadLoad::SetGridTitle()
{	
	CCalcFloor      *pCalcFloor	= m_pStd->m_pCalcStd->m_pCalcFloor;
	long nSel = m_ctlTab.GetCurSel();

	CStringArray Arr;

	pCalcFloor->SetSlabMinThick(FLOOR_LEFT);
	pCalcFloor->SetSlabMinThick(FLOOR_RIGHT);
	pCalcFloor->SetSlabMinThick(FLOOR_MID);

	if(nSel==0)		 pCalcFloor->CalcSlabLeftDeadLoad(Arr);
	else if(nSel==1) pCalcFloor->CalcSlabRightDeadLoad(Arr);
	else if(nSel==2) pCalcFloor->CalcSlabCenterDeadLoad(Arr);

	long nArrsu = Arr.GetSize();
	long nRow   = (long)(nArrsu/4)+2;
	long nCol   = 5;

	m_Grid.DeleteAllItems();
	if(nSel != 2)
		m_Grid.SetRowCount(nRow);
	else
		m_Grid.SetRowCount(nRow-1);
	m_Grid.SetColumnCount(nCol);

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);	
	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidth(0,80);
	m_Grid.SetColumnWidth(1,400);
	m_Grid.SetColumnWidth(2,100);
	m_Grid.SetColumnWidth(3,100);
	m_Grid.SetColumnWidth(4,100);
	m_Grid.ExpandColumnsToFit();

	m_Grid.SetTextMatrix(0,0,"번호");
	m_Grid.SetTextMatrix(0,1,"제           원");
	m_Grid.SetTextMatrix(0,2,"중  량 (kN)");
	m_Grid.SetTextMatrix(0,3,"거  리 ( m )");
	m_Grid.SetTextMatrix(0,4,"M ( kN·m )");

	if(Arr.GetSize()==0)	return;

	CString str;
	long index = 0;

	for(long r = 1; r < nRow-1; r++)
	{
		str.Format("%d",r);
		m_Grid.SetTextMatrix(r, 0, str);
		m_Grid.SetTextMatrix(r, 1, Arr[index++], (UINT)DT_LEFT);
		str.Format("%.3f", atof(Arr[index++]));	m_Grid.SetTextMatrix(r, 2, str);
		str.Format("%.3f", atof(Arr[index++]));	m_Grid.SetTextMatrix(r, 3, str);
		str.Format("%.3f", atof(Arr[index++]));	m_Grid.SetTextMatrix(r, 4, str);
	}

	if(nSel != 2)
	{
		m_Grid.SetTextMatrix(nRow-1, 0, "합계");
		m_Grid.SetTextMatrix(nRow-1, 2, Arr[index++]);
		str.Format("%.3f", atof(Arr[index]));	m_Grid.SetTextMatrix(nRow-1, 4, str);
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CCalcCantileverDeadLoad::DrawInputDomyunView(BOOL bZoomAll)
{
	CAPlateDrawCalc DrawCalc(m_pStd->m_pDataManage);

	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateBasicIndex    *pBx = pDB->GetGirder(0)->GetCrossBxAtMinFlangeWidth();
	CCentSeparation    *pSep = NULL;
	CCalcFloor CalcFloor(m_pStd->GetDataManage());
	CCalcGeneral CalcGeneral(m_pStd->GetDataManage());
	double dLen = 0;

	long nSel = m_ctlTab.GetCurSel();
	long	nType			= 0;
	
	if(nSel==0)			
	{
		nType	= CalcFloor.GetGuardWallType(FLOOR_LEFT);
		pSep = pDB->GetCurGuardWall(CCentSeparation::LEFT);
		dLen = pDB->GetLengthHDanDom(pBx, 0);
	}
	else if(nSel==1)	
	{
		nType	= CalcFloor.GetGuardWallType(FLOOR_RIGHT);
		pSep = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
		dLen = pDB->GetLengthHDanDom(pBx, pDB->GetQtyHDanNode()-1);
	}
	if(!pSep)	dLen	= 0;

	CDomyun *pDom = m_pView->GetDomyun();

	pDom->ClearEtt(TRUE);
	pDom->SetScaleDim(20);
	pOptStd->SetEnvType(pDom, HCAD_DIML);

	double	dDis_Car		= CalcGeneral.GetDBLOAD_Dis_CAR();
	double	dDis_Tank		= CalcGeneral.GetDBLOAD_Dis_Tank();
	double	dDis_Trailer	= CalcGeneral.GetDBLOAD_Dis_Trailer();

	DrawCalc.SetDBLoad(dDis_Car, dDis_Tank, dDis_Trailer);

	if(nSel==0)
	{	
		DrawCalc.SetGuardType(nType);
		DrawCalc.DrawGuardWallInputLeft(pDom,pSep, dLen); 
		DrawCalc.DrawGuardDivideDimLeft(pDom,pSep, dLen);
	}
	else if(nSel==1)
	{		
		DrawCalc.SetGuardType(nType);
		DrawCalc.DrawGuardWallInputRight(pDom,pSep, dLen);
		DrawCalc.DrawGuardDivideDimRight(pDom,pSep, dLen);
	}
		
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcCantileverDeadLoad::OnCursorChange(long nRow, long nCol)
{
}

void CCalcCantileverDeadLoad::OnPreInitDialog()
{
	m_ctlTab.InsertItem(0,"좌측 캔틸레버부");
	m_ctlTab.InsertItem(1,"우측 캔틸레버부");
	m_ctlTab.InsertItem(2,"중앙부 바닥판");

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_GRID,			SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_DESIGN,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CCalcCantileverDeadLoad::OnSelchangeTabDesign(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData();

	SetDataInit();
	DrawInputDomyunView();
}
