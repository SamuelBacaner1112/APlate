// JRibPosition.cpp : implementation file
// 수평 보강재 응력구간 위치 입력

#include "stdafx.h"
#include "GeneralInput.h"
#include "JRibPosition.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJRibPosition dialog

const LPCTSTR sPressRange		= _T("압축");
const LPCTSTR sStretchRange		= _T("인장");

#define RIB_CONTEXTSU		2
#define GetGirByRow(r)		( (r -1)/RIB_CONTEXTSU - 1 )
#define GetRowByGir(nG)		( (nG+1)*RIB_CONTEXTSU + 1 )

CJRibPosition::CJRibPosition(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CJRibPosition::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJRibPosition)
	m_bSameApplySameCol = TRUE;
	m_dDisCenterOnJRib  = 0.0;
	m_nLengthType       = 1;
	m_bSimpleItem       = FALSE;
	m_dCheckLength		= 0.0;
	//}}AFX_DATA_INIT
}

void CJRibPosition::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJRibPosition)	
// 	DDX_Control(pDX, IDC_JRIB_GIRDERLEN, m_Radio1);
// 	DDX_Control(pDX, IDC_JRIB_BRIDGELEN, m_Radio2);
	DDX_Control(pDX, IDC_JRIB_DISTANCE_CENTER, m_Edit);
	DDX_GridControl(pDX, IDC_GRID, m_uGrid);	
	DDX_GridControl(pDX, IDC_GRID_DN, m_lGrid);	
	DDX_Check(pDX, IDC_CHECK_SAMEAPPLY, m_bSameApplySameCol);
	DDX_Text(pDX, IDC_JRIB_DISTANCE_CENTER, m_dDisCenterOnJRib);
	DDX_Radio(pDX, IDC_JRIB_GIRDERLEN, m_nLengthType);
	DDX_Check(pDX, IDC_JRIB_ITEMTYPE, m_bSimpleItem);
	DDX_Text(pDX, IDC_HSTIFF_CHECK_LENGTH, m_dCheckLength);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_MOVELEFT, m_btnMoveLeft);
	DDX_Control(pDX, IDC_BUTTON_MOVERIGHT, m_btnMoveRight);
	
	DDX_Control(pDX, IDC_JRIBDETAIL_RECOMMEND_UPPER1, m_btnUp1);
	DDX_Control(pDX, IDC_JRIBDETAIL_RECOMMEND_UPPER2, m_btnUp2);
	DDX_Control(pDX, IDC_JRIBDETAIL_RECOMMEND_UPPER3, m_btnUp3);
	DDX_Control(pDX, IDC_JRIBDETAIL_RECOMMEND_LOWER1, m_btnDown1);
	DDX_Control(pDX, IDC_JRIBDETAIL_RECOMMEND_LOWER2, m_btnDown2);
	DDX_Control(pDX, IDC_JRIBDETAIL_RECOMMEND_LOWER3, m_btnDown3);
	
	DDX_Control(pDX, IDC_WHSTIFF_AUTOSETTING, m_btnAutoSetting);
	DDX_Control(pDX, IDC_JRIB_ARRANGE_THISGIRDER, m_btnSameAll);
	DDX_Control(pDX, IDC_JRIB_AUTO_SYMMETRY, m_btnSymmetry);	
}

BEGIN_MESSAGE_MAP(CJRibPosition, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CJRibPosition)	
	ON_BN_CLICKED(IDC_BUTTON_MOVELEFT, OnButtonMoveleft)
	ON_BN_CLICKED(IDC_BUTTON_MOVERIGHT, OnButtonMoveright)
	ON_BN_CLICKED(IDC_JRIB_BRIDGELEN, OnJribBridgelen)
	ON_BN_CLICKED(IDC_JRIB_GIRDERLEN, OnJribGirderlen)
	ON_BN_CLICKED(IDC_CHECK_SAMEAPPLY, OnCheckSameapply)
	ON_BN_CLICKED(IDC_JRIB_ARRANGE_THISGIRDER, OnJribArrangeThisgirder)
	ON_BN_CLICKED(IDC_JRIB_AUTO_SYMMETRY, OnJribAutoSymmetry)
	ON_BN_CLICKED(IDC_JRIB_ITEMTYPE, OnJribItemtype)
	ON_BN_CLICKED(IDC_JRIBDETAIL_RECOMMEND_LOWER1, OnJribdetailRecommendLower1)
	ON_BN_CLICKED(IDC_JRIBDETAIL_RECOMMEND_LOWER2, OnJribdetailRecommendLower2)
	ON_BN_CLICKED(IDC_JRIBDETAIL_RECOMMEND_LOWER3, OnJribdetailRecommendLower3)
	ON_BN_CLICKED(IDC_JRIBDETAIL_RECOMMEND_UPPER1, OnJribdetailRecommendUpper1)
	ON_BN_CLICKED(IDC_JRIBDETAIL_RECOMMEND_UPPER2, OnJribdetailRecommendUpper2)
	ON_BN_CLICKED(IDC_JRIBDETAIL_RECOMMEND_UPPER3, OnJribdetailRecommendUpper3)
	ON_BN_CLICKED(IDC_WHSTIFF_AUTOSETTING, OnWhstiffAutosetting)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedDataUpper)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChangedUpper)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)	
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)	
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)	
	//
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_DN, OnCellChangedDataLower)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_DN, OnCellChangedLower)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID_DN, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID_DN, OnCellDeleteCol)	
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID_DN, OnCellDeletedCol)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJRibPosition message handlers

BOOL CJRibPosition::OnInitDialog() 
{		
	CGeneralBaseDlg::OnInitDialog();
	
	SetResize(IDC_BUTTON_MOVELEFT,	   SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_MOVERIGHT,	   SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	SetResize(IDC_JRIBDETAIL_RECOMMEND_UPPER1,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIBDETAIL_RECOMMEND_UPPER2,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIBDETAIL_RECOMMEND_UPPER3,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIBDETAIL_RECOMMEND_LOWER1,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIBDETAIL_RECOMMEND_LOWER2,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIBDETAIL_RECOMMEND_LOWER3,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIB_ARRANGE_THISGIRDER,			SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_JRIB_AUTO_SYMMETRY,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);		
	SetResize(IDC_WHSTIFF_AUTOSETTING,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_CHECK_LEN,					SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_HSTIFF_CHECK_LENGTH,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_GROUPBOX,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIB_GIRDERLEN,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);	
	SetResize(IDC_JRIB_BRIDGELEN,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_JRIB_DISTANCE_CENTER,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	

	SetResize(IDC_STATIC_UPPER, SZ_TOP_LEFT, SZ_BOTTOM_CENTER);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_CENTER);

	SetResize(IDC_STATIC_LOWER, SZ_TOP_CENTER, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_DN, SZ_TOP_CENTER, SZ_BOTTOM_RIGHT);
	
	return TRUE;
}

void CJRibPosition::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);	
	
	CDomyun Dom(pDom);

	if(m_nTab==0)
	{
		CAPlateDrawPyung Draw(m_pStd->GetDataManage());
		Draw.DrawPyung(&Dom, 5);		
	}
	else
	{
		CAPlateDrawJong Draw(m_pStd->GetDataManage());
		Draw.DrawJongGirderTotal(&Dom);
	}
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CJRibPosition::SetDataDefaultUpper(long nG, long nDefaultType)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(nG);
	CGridCtrl       *pGridU  = &m_uGrid;	

	long row   = GetRowByGir( nG );

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;
	CString str;
	CPtrArray BxArr;

	double dFactor = 0.8;						//권고안 1
	if(nDefaultType == 1)		dFactor = 0.75;	//권고안 2
	else if(nDefaultType == 2)	dFactor = 0.85;	//권고안 3
	GetBxByJRibUpperPos(nG, BxArr, dFactor); 

	long nMin = BxArr.GetSize();	
	SetGridTitleSub(&m_uGrid, nMin+1);	

	for(long i=0; i<nMin; i++)
	{
		pBx = (CPlateBasicIndex*)BxArr[i]; 
		SetMatrixByStation(pGridU, row, i+1, pBx->GetStation());
		str.Format("%s",(i%2 == 0) ? sPressRange : sStretchRange);
		pGridU->SetTextMatrix(row+1,i+1,str);
		pGridU->SetItemState(row+1, i+1, pGridU->GetItemState(row+1,i+1) | GVIS_READONLY);
		pGridU->SetItemBkColour(row+1, i+1, RGBREADONLY);						
	}
	pGridU->SetRedraw(TRUE,TRUE);
}

void CJRibPosition::SetDataDefaultLower(long nG, long nDefaultType)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(nG);
	CGridCtrl       *pGridL  = &m_lGrid;	

	long nCols = GetRecommendLowerCols(nG);	
	long row   = GetRowByGir(nG);
	SetGridTitleSub(&m_lGrid, nCols);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;
	long nContinue;
	double DIST_STD = 5000.0;						//권고안 1
	if(nDefaultType == 1)		DIST_STD = 7500.0;	//권고안 2
	else if(nDefaultType == 2)	DIST_STD = 10000.0;	//권고안 3
	CString str;
	__int64 bxFlag = BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_STT_GIRDER|BX_END_GIRDER;

	for(long i=0; i<nCols-1; i++)
	{
		if(i < nCols-2) 
		{
			nContinue = i;
			while(GetRecommendLowerContinue( nG, ++nContinue));
			double staPrev = GetStationMatrix(&m_uGrid, row,nContinue);
			double sta     = staPrev + ((i%2==0) ? -DIST_STD : DIST_STD);
			pBx = pGir->GetBxByStation(sta,bxFlag);	
		}
		else
			pBx = Finder.GetBxLast(BX_END_GIRDER);

		SetMatrixByStation(pGridL, row, i+1, pBx->GetStation());
		str.Format("%s",(i%2) ? sPressRange : sStretchRange);
		pGridL->SetTextMatrix(row+1,i+1,str);
		pGridL->SetItemState(row+1, i+1, pGridL->GetItemState(row+1,i+1) | GVIS_READONLY);
	}
	pGridL->SetRedraw(TRUE,TRUE);
}

void CJRibPosition::SetDataInitGir(long nG, BOOL bUpper)
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(nG);

	CGridCtrl* pGrid = bUpper ? &m_uGrid : &m_lGrid;
	CTypedPtrArray <CObArray, CRangePressData *> *pArray = bUpper ? &pGir->m_pRangePress->m_ArrayUpper : &pGir->m_pRangePress->m_ArrayLower;	
	for(long nC=0; nC<pArray->GetSize(); nC++)
	{
		pGrid->SetTextMatrix(GetRowByGir(nG), nC+1, "%.0f", pArray->GetAt(nC)->m_dDistRange);
		pGrid->SetTextMatrix(GetRowByGir(nG)+1, nC+1, pArray->GetAt(nC)->m_bCompress ? sPressRange : sStretchRange);
	}
}

void CJRibPosition::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();		

	m_bSameApplySameCol = (BOOL)pBridge->m_cSameApplySameColJRib;
	m_dDisCenterOnJRib  = pBridge->m_dDisCenterOnJRib;
	m_nLengthType       = pBridge->m_nLengthTypeOnJRib;
	m_dCheckLength		= pBridge->m_dHStiffCheckLength;

	SetGridTitleSub(&m_uGrid, pBridge->GetGirder(0)->m_pRangePress->m_ArrayUpper.GetSize()+1);	// SetRows()
	SetGridTitleSub(&m_lGrid, pBridge->GetGirder(0)->m_pRangePress->m_ArrayLower.GetSize()+1);
	for(long nG=-1; nG<pBridge->GetGirdersu(); nG++)
	{
		SetDataInitGir(nG, TRUE);
		SetDataInitGir(nG, FALSE);
	}
	UpdateData(FALSE);
}

void CJRibPosition::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	m_bSameApplySameCol = (BOOL)pBridge->m_cSameApplySameColJRib;
	m_dDisCenterOnJRib  = pBridge->m_dDisCenterOnJRib;
	m_nLengthType       = pBridge->m_nLengthTypeOnJRib;
	m_dCheckLength		= pBridge->m_dHStiffCheckLength;

	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
	{						
		SetDataDefaultUpper(nG, 0);
		SetDataDefaultLower(nG, 0);		
	}
	UpdateData(FALSE);
	SetDataSave();
}

void CJRibPosition::SetDataSave()
{	
	UpdateData(TRUE);

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	pBridge->m_cSameApplySameColJRib = (BYTE)m_bSameApplySameCol;
	pBridge->m_dDisCenterOnJRib      = m_dDisCenterOnJRib;
	pBridge->m_nLengthTypeOnJRib     = m_nLengthType;
	pBridge->m_dHStiffCheckLength	 = m_dCheckLength;
	long nR = 0;		
	for(nR=1; nR<m_uGrid.GetRowCount(); nR+=2)
	{
		long nG = GetGirByRow(nR);
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		pGir->m_pRangePress->MakeArray(m_uGrid.GetColumnCount()-1, TRUE);		
		for(long nC=1; nC<m_uGrid.GetColumnCount(); nC++)
		{						
			pGir->m_pRangePress->m_ArrayUpper.GetAt(nC-1)->m_dDistRange = m_uGrid.GetTextMatrixDouble(nR,nC);
			pGir->m_pRangePress->m_ArrayUpper.GetAt(nC-1)->m_bCompress  = m_uGrid.GetTextMatrix(nR+1,nC) == sPressRange ? TRUE : FALSE;
		}
	}

	for(nR=1; nR<m_lGrid.GetRowCount(); nR+=2)
	{
		long nG = GetGirByRow(nR);
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		pGir->m_pRangePress->MakeArray(m_lGrid.GetColumnCount()-1, FALSE);		
		for(long nC=1; nC<m_lGrid.GetColumnCount(); nC++)
		{						
			pGir->m_pRangePress->m_ArrayLower.GetAt(nC-1)->m_dDistRange = m_lGrid.GetTextMatrixDouble(nR,nC);
			pGir->m_pRangePress->m_ArrayLower.GetAt(nC-1)->m_bCompress  = m_lGrid.GetTextMatrix(nR+1,nC) == sPressRange ? TRUE : FALSE;
		}
	}
}

void CJRibPosition::SetGridTitle()
{

}

// 상.하부 Gridsetting
void CJRibPosition::SetGridTitleSub(CGridCtrl* pGrid,long cols)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	pGrid->EnableReturnPass(TRUE);
	pGrid->SetEditable(TRUE);
	pGrid->SetListMode(FALSE);
	pGrid->EnableDragAndDrop(TRUE);
	pGrid->EnableInsertRow(FALSE);
	pGrid->EnableInsertCol(TRUE);
	pGrid->EnableTitleTips(FALSE);
//	pGrid->SetTextBkColor(RGBEDITABLE);	
	pGrid->SetFixedRowCount(1);
	pGrid->SetFixedColumnCount(1);

	pGrid->SetRowCount((pBridge->GetGirdersu()+1)*RIB_CONTEXTSU + 1);
	pGrid->SetColumnCount(cols);

	pGrid->SetRowHeightAll(20);
	pGrid->SetRowHeight(0, 20);	
	pGrid->SetColumnWidthAll(60);
	pGrid->SetTextBkColor(RGB(225, 250, 250));

	pGrid->SetColumnWidth(0,65);
	pGrid->SetTextMatrix(0,0,"구 분");
	long	row = 1;
	CString str,str1;
	for(long nG = -1; nG < pBridge->GetGirdersu();nG++)
	{
		if(nG == -1)
			str1 ="(CR)";
		else str1.Format("(G%d)",nG+1+pBridge->GetNumberStartGirder());
		str = "길이" + str1;
		pGrid->SetTextMatrix(row++,0,str);
		str = "응력" + str1;		
		if (m_bSimpleItem)  pGrid->SetRowHeight(row, 0);
		pGrid->SetTextMatrix(row++,0,str);
	}
	SetGridTitleColum(pGrid);

	pGrid->SetRedraw(TRUE,TRUE);
}

// Grid의 colum제목 출력
void CJRibPosition::SetGridTitleColum(CGridCtrl * pGrid)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CString str;
	for(long c = 1;c < pGrid->GetCols();c++)
	{		
		str.Format("구간%d",c);
		pGrid->SetTextMatrix(0,c,str);

		for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
		{
			long row = GetRowByGir( nG );
			pGrid->SetItemState(row+1, c, pGrid->GetItemState(row+1,c) | GVIS_READONLY);
			pGrid->SetItemBkColour(row+1, c, RGBREADONLY);		
		}
	}
}

void CJRibPosition::OnPreInitDialog()
{	
	m_pGrid = &m_uGrid;	
}

double CJRibPosition::GetStationMatrix(CGridCtrl *pGrid, long nRow, long nCol)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	double lenSum = 0;
	for(long c = 1; c <= nCol; c++)
		lenSum += pGrid->GetTextMatrixDouble(nRow, c);
	double sta = pBridge->GetStationByLengthInGrid(FALSE, nRow/RIB_CONTEXTSU-1, lenSum, m_nLengthType, m_dDisCenterOnJRib);

	return sta;
}

// 단지간 구분 없음(포함) - 권고안 1 사용
void CJRibPosition::GetBxByJRibUpperPos(long nG, CPtrArray& BxArr, double dFactor) const
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);

	__int64 bxFlag = BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_STT_GIRDER|BX_END_GIRDER;
	CPlateBasicIndex* pBx;
	for(long k = 0; k < pBridge->m_nQtyJigan-1; k++)
	{
		double dSta1 = pGir->GetStationOnJijum(k);
		double dSta2 = pGir->GetStationOnJijum(k+1);
		double dSta3 = pGir->GetStationOnJijum(k+2);
		double dDis1 = dSta2 - dSta1;
		double dDis2 = dSta3 - dSta2;

		pBx = pGir->GetBxByStation(dSta1 + dDis1 * dFactor,bxFlag);
		BxArr.Add(pBx);
		pBx = pGir->GetBxByStation(dSta2 + dDis2 * (1-dFactor),bxFlag);
		BxArr.Add(pBx);
	}

	CPlateBxFinder Finder(pGir);
	BxArr.Add( Finder.GetBxLast(BX_END_GIRDER) );
}

void CJRibPosition::SetMatrixByStation(CGridCtrl *pGrid, long nRow, long nCol, double sta)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	double lenSum = 0;
	for(long c = 1; c < nCol; c++)
		lenSum += pGrid->GetTextMatrixDouble(nRow, c);
	double len = pBridge->GetLengthByStationInGrid(FALSE, nRow/RIB_CONTEXTSU-1, sta, m_nLengthType, m_dDisCenterOnJRib) - lenSum;
	pGrid->SetTextMatrix(nRow, nCol, "%.0f", len);
}

long CJRibPosition::GetRecommendLowerCols(long nG)
{
	CPlateBridgeApp *pBridge =  m_pStd->GetBridge();
	long row = GetRowByGir( nG );
	long nCols = m_uGrid.GetCols();
	long nResult = 2;

	CString sPress( m_uGrid.GetTextMatrix(row+1,1) );
	for(long n = 2; n < nCols; n++)
	{
		CString s = m_uGrid.GetTextMatrix(row+1,n);
		if(s != sPress) nResult++;
		sPress = s;
	}

	return nResult;
}

BOOL CJRibPosition::GetRecommendLowerContinue(long nG,long nCurCol)
{
	CPlateBridgeApp *pBridge =  m_pStd->GetBridge();
	long row = GetRowByGir( nG );
	long nCols = m_uGrid.GetCols();
	
	CString sCur = m_uGrid.GetTextMatrix(row+1,nCurCol);
	if(nCols <= nCurCol + 1)	return FALSE;
	
	return sCur == m_uGrid.GetTextMatrix(row+1,nCurCol+1);
}

long CJRibPosition::GetRecommendCol(long nG,long col)
{
	CPlateBridgeApp *pBridge =  m_pStd->GetBridge();
	long row = GetRowByGir( nG );
	long nCols = m_uGrid.GetCols();
	long nResult = 1;
	long nSearchStep = col;

	CString sPress( m_uGrid.GetTextMatrix(row+1,1) );
	for(long n = 1; n < nCols; n++)
	{
		CString s = m_uGrid.GetTextMatrix(row+1,n);
		if(s != sPress) nResult++;
		sPress = s;

		if(nResult == nSearchStep) return n;
	}

	return nCols-1;
}



void CJRibPosition::OnCellChangedUpper(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	m_pGrid = &m_uGrid;

	OnRowColChangeUpperRibdetail();
}

void CJRibPosition::OnCellChangedLower(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	m_pGrid = &m_lGrid;

	OnRowColChangeLowerRibdetail();
}



void CJRibPosition::OnCellChangedDataUpper(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;	
	m_pGrid = &m_uGrid;

	if( (nRow-1) % RIB_CONTEXTSU == 0 )	// 0:길이
	{
		if(nCol == m_pGrid->GetColumnCount()-1)
			m_pGrid->SetTextMatrix(nRow,nCol,m_pGrid->GetLastChangeData());
		else
		{
			double staNew = GetStationMatrix(m_pGrid, nRow, nCol);
			m_pGrid->SetTextMatrix(nRow, nCol, m_pGrid->GetLastChangeData());
			double staDif = staNew - GetStationMatrix(m_pGrid, nRow, nCol);

			for (long r = 1; r < m_pGrid->GetRows(); r+=RIB_CONTEXTSU)   {
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				double sta = GetStationMatrix(m_pGrid, r, nCol) + staDif;
				double staNext = GetStationMatrix(m_pGrid, r, nCol+1);
				SetMatrixByStation(m_pGrid, r, nCol, sta);
				SetMatrixByStation(m_pGrid, r, nCol+1, staNext);
			}
		}
	}
	m_pGrid->Invalidate();
}

void CJRibPosition::OnCellChangedDataLower(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;	
	m_pGrid = &m_lGrid;


	if( (nRow-1) % RIB_CONTEXTSU == 0 )	// 0:길이
	{
		if(nCol == m_pGrid->GetColumnCount()-1)
			m_pGrid->SetTextMatrix(nRow,nCol,m_pGrid->GetLastChangeData());
		else
		{
			double staNew = GetStationMatrix(m_pGrid, nRow, nCol);
			m_pGrid->SetTextMatrix(nRow, nCol, m_pGrid->GetLastChangeData());
			double staDif = staNew - GetStationMatrix(m_pGrid, nRow, nCol);

			for (long r = 1; r < m_pGrid->GetRows(); r+=RIB_CONTEXTSU)   {
				if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
					continue;
				double sta = GetStationMatrix(m_pGrid, r, nCol) + staDif;
				double staNext = GetStationMatrix(m_pGrid, r, nCol+1);
				SetMatrixByStation(m_pGrid, r, nCol, sta);
				SetMatrixByStation(m_pGrid, r, nCol+1, staNext);
			}
		}
	}
	m_pGrid->Invalidate();
}

//추가
void CJRibPosition::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;	

	m_pGrid->DeleteColumn(nCol);
	m_pGrid->SetFocusCell(nRow,nCol);
	SetGridCellInsertedCol(TRUE);	
}

void CJRibPosition::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
    int nCol = GridView->iColumn;	

	m_pGrid->InsertColumn("",0,nCol);
}

//삭제
void CJRibPosition::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;	

	m_pGrid->SetFocusCell(nRow,nCol);
	SetGridCellInsertedCol(FALSE);
	m_pGrid->DeleteUnusedColumn();
}

void CJRibPosition::OnRowColChangeUpperRibdetail()
{
	if(m_nTab==1) return;

	CPlateBridgeApp *pBridge =  m_pStd->GetBridge();

	long	nRow = m_uGrid.GetFocusCell().row,
			nCol = m_uGrid.GetFocusCell().col;
	if( nRow <= 0 || nCol <= 0 ) return;

	//DrawInputDomyunView(FALSE);
	m_pView->GetDomyun()->ClearEtt(FALSE);

	long nG2 = -2;
	double presta2, sta2;
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->FixEtt(TRUE);
	for (long r = 1; r < m_uGrid.GetRowCount(); r++)   {
		if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		long nRow1 = r - (r - 1) % RIB_CONTEXTSU;  // Current위치를 길이위치로 바꿈.
		long nG = (nRow1 - (RIB_CONTEXTSU+1)) / RIB_CONTEXTSU;
		double presta, sta;
		presta = GetStationMatrix(&m_uGrid, nRow1, nCol-1);
		sta = GetStationMatrix(&m_uGrid, nRow1, nCol);
		if (r == nRow)   {
			nG2 = nG;
			presta2 = presta;
			sta2 = sta;
		}
		else
			Draw.SetCursorPosition(nG, presta, sta, pDom);
	}
	// 자기 거더는 맨 마지막에...
	if (nG2 >= -1)
		Draw.SetCursorPosition(nG2, presta2, sta2, pDom, TRUE, 400, 0, 100);
	pDom->FixEtt(FALSE);
/*
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();

	nRow -= (nRow - 1) % RIB_CONTEXTSU;  // Current위치를 길이위치로 바꿈.
	long nG = (nRow - (RIB_CONTEXTSU+1)) / RIB_CONTEXTSU;
	double presta, sta;
	
	presta = GetStationMatrix(&m_uGrid, nRow, nCol-1);
	sta = GetStationMatrix(&m_uGrid, nRow, nCol);
	pDom->ClearEtt(FALSE);  // 여기서 방금 그린 지시선 2개 지움.
	if(m_bSameApplySameCol)	
	{
		pDom->DeleteCursor();
		for(long nG=0;nG<pBridge->GetGirdersu();nG++)
		{
			presta = GetStationMatrix(&m_uGrid, nG * RIB_CONTEXTSU+(RIB_CONTEXTSU+1), nCol-1);
			sta = GetStationMatrix(&m_uGrid, nG * RIB_CONTEXTSU+(RIB_CONTEXTSU+1), nCol);
			Draw.SetCursorPosition(nG, presta, sta, pBridge->GetGirder(nG)->m_dWidth/2, pDom);
		}
	}
	else
		Draw.SetCursorPosition(nG, presta, sta, pBridge->GetGirder(nG)->m_dWidth/2, pDom);
	pDom->Invalidate();
*/

	EnableButton();
}

void CJRibPosition::OnRowColChangeLowerRibdetail()
{
	if(m_nTab==1) return;

	CPlateBridgeApp *pBridge =  m_pStd->GetBridge();

	long	nRow = m_lGrid.GetFocusCell().row,
			nCol = m_lGrid.GetFocusCell().col;
	if( nRow <= 0 || nCol <= 0 ) return;

	//DrawInputDomyunView(FALSE);
	m_pView->GetDomyun()->ClearEtt(FALSE);

	long nG2 = -2;
	double presta2, sta2;
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->FixEtt(TRUE);
	for (long r = 1; r < m_lGrid.GetRowCount(); r++)   {
		if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		long nRow1 = r - (r - 1) % RIB_CONTEXTSU;  // Current위치를 길이위치로 바꿈.
		long nG = (nRow1 - (RIB_CONTEXTSU+1)) / RIB_CONTEXTSU;
		double presta, sta;
		presta = GetStationMatrix(&m_lGrid, nRow1, nCol-1);
		sta = GetStationMatrix(&m_lGrid, nRow1, nCol);
		if (r == nRow)   {
			nG2 = nG;
			presta2 = presta;
			sta2 = sta;
		}
		else
			Draw.SetCursorPosition(nG, presta, sta, pDom);
	}
	// 자기 거더는 맨 마지막에...
	if (nG2 >= -1)
		Draw.SetCursorPosition(nG2, presta2, sta2, pDom, TRUE, 400, 0, 100);
	pDom->FixEtt(FALSE);
/*
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();

	nRow -= (nRow - 1) % RIB_CONTEXTSU;  // Current위치를 길이위치로 바꿈.
	long nG = (nRow - (RIB_CONTEXTSU+1)) / RIB_CONTEXTSU;
	double presta, sta;
	presta = GetStationMatrix(&m_lGrid, nRow, nCol-1);
	sta = GetStationMatrix(&m_lGrid, nRow, nCol);
	pDom->ClearEtt(FALSE);  // 여기서 방금 그린 지시선 2개 지움.
	if(m_bSameApplySameCol)
	{		
		pDom->DeleteCursor();
		for(long nG=0;nG<pBridge->GetGirdersu();nG++)
		{
			presta = GetStationMatrix(&m_lGrid, nG * RIB_CONTEXTSU+(RIB_CONTEXTSU+1), nCol-1);
			sta = GetStationMatrix(&m_lGrid, nG * RIB_CONTEXTSU+(RIB_CONTEXTSU+1), nCol);
			Draw.SetCursorPosition(nG, presta, sta, pBridge->GetGirder(nG)->m_dWidth/2, pDom);
		}
	}
	else
		Draw.SetCursorPosition(nG, presta, sta, pBridge->GetGirder(nG)->m_dWidth/2, pDom);
	pDom->Invalidate();
*/

	EnableButton();
}

void CJRibPosition::MoveLeftRight(BOOL bLeft) 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	UpdateData(TRUE);
	
	long	row = m_pGrid->GetFocusCell().row,
			col = m_pGrid->GetFocusCell().col;

	long nG = GetGirByRow(row);
	long nValue = (row-1) % RIB_CONTEXTSU;	// 0:길이, 1:응력

	if(m_bSameApplySameCol)
		for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
			MoveLeftRightByValue(nG, nValue, col, bLeft);
	else
		MoveLeftRightByValue(nG, nValue, col, bLeft);

	m_pGrid->SetFocus();	

	m_pGrid->Invalidate();
}

// nValue - 0:길이, 1:응력
void CJRibPosition::MoveLeftRightByValue(long nG,long nValue,long col,BOOL bLeft) 
{
	long row = GetRowByGir(nG) + nValue;
	__int64 bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING|BX_VSTIFF;
	switch( nValue )
	{
	case 0:
		{
			CPlateBridgeApp *pBridge = m_pStd->GetBridge();
			CPlateGirderApp *pGir    = pBridge->GetGirder(nG);
			CPlateBxFinder Finder(pGir);
			double sta = GetStationMatrix(m_pGrid, row, col);

			CPlateBasicIndex *pBx = pGir->GetBxByStation(sta,bxFlag);
			CPlateBasicIndex *pBxMove;

			if(bLeft)	pBxMove  = Finder.GetBxLast(bxFlag,pBx,NULL,FALSE);
			else		pBxMove  = Finder.GetBxFirst(bxFlag,pBx,NULL,FALSE);
			if(!pBxMove) pBxMove = pBx;

			double dNextSta = GetStationMatrix(m_pGrid, row, col + 1);
			SetMatrixByStation(m_pGrid, row, col, pBxMove->GetStation());
			SetMatrixByStation(m_pGrid, row, col+1, dNextSta);
			break;
		}
	case 1:
		m_pGrid->SetTextMatrix(row, col,
			m_pGrid->GetTextMatrix(row,col)==sPressRange ? sStretchRange : sPressRange);		
		break;
	}
}

void CJRibPosition::MoveSellData(long nStdRow,long col,BOOL bAdd)
{
	long nCols = m_pGrid->GetCols();
	__int64 bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING|BX_VSTIFF;

	if(bAdd)// Add
	{
		for(long n = nCols-1; n > col; n--)
			for(long i = 0; i < RIB_CONTEXTSU; i++)
				m_pGrid->SetTextMatrix(nStdRow+i, n, m_pGrid->GetTextMatrix(nStdRow+i,n-1));

		CPlateBridgeApp* pBridge = m_pStd->GetBridge();
		CPlateGirderApp*	 pGir = pBridge->GetGirder( GetGirByRow(nStdRow) );

		double staFir = GetStationMatrix(m_pGrid, nStdRow, col-1);
		double staSec = GetStationMatrix(m_pGrid, nStdRow, col);

		double sta = (staFir + staSec) / 2;
		CPlateBasicIndex* pBx = pGir->GetBxByStation(sta,bxFlag);
		sta = pBx->GetStation();

		m_pGrid->SetTextMatrix(nStdRow, col+1, "%.0f", staSec - sta);
		m_pGrid->SetTextMatrix(nStdRow, col, "%.0f",  sta - staFir);
	}
	else
	{
		if(nCols <= col) return;

		double sum = m_pGrid->GetTextMatrixDouble(nStdRow, col);
		sum += m_pGrid->GetTextMatrixDouble(nStdRow, col+1);
		m_pGrid->SetTextMatrix(nStdRow, col+1, "%.0f",sum);
		for(long n = col; n < nCols-1; n++)
			for(long i = 0; i < RIB_CONTEXTSU; i++)
				m_pGrid->SetTextMatrix(nStdRow+i, n, m_pGrid->GetTextMatrix(nStdRow+i,n+1));

		// last colum data clear
		for(long i = 0; i < RIB_CONTEXTSU; i++)
			m_pGrid->SetTextMatrix(nStdRow+i, nCols-1, _T(""));
	}
}

BOOL CJRibPosition::IsData(long row,long col) const
{
 	long nG = GetGirByRow(row);
	row = GetRowByGir(nG);
	
	if(m_pGrid->GetCols() <= col)
		return FALSE;

	BOOL bResult = atol(m_pGrid->GetTextMatrix(row,col)) ? TRUE : FALSE;// length
	bResult |=	!m_pGrid->GetTextMatrix(row+1,col).IsEmpty();				// 응력

	return bResult;
}


void CJRibPosition::MoveSellGirderGroup(long nG, long col,BOOL bAdd) 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	long nCols = m_pGrid->GetCols();
	long row = GetRowByGir(nG);

	if(bAdd)
	{
		BOOL bGridAdd = IsData(row,nCols-1);
		if(bGridAdd)
		{
			m_pGrid->SetCols(nCols+1);
			SetGridTitleColum(m_pGrid);
		}

		MoveSellData(row,col,TRUE);
	}
	else
	{
		MoveSellData(row,col,FALSE);
		BOOL bGridDel = TRUE;
		for(long n = -1; n < pBridge->GetGirdersu(); n++)
			bGridDel &= (IsData( GetRowByGir(n), nCols-1) ? 0 : 1);
		SetGridTitleColum(m_pGrid);
		if(bGridDel) m_pGrid->SetCols(nCols-1);
	}
}

void CJRibPosition::SetGridCellInsertedCol(BOOL bAdd) 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	UpdateData(TRUE);

	long	row = m_pGrid->GetFocusCell().row,
			col = m_pGrid->GetFocusCell().col;
 	long nG = GetGirByRow(row);


	if(m_bSameApplySameCol)
		for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
			MoveSellGirderGroup(nG,col,bAdd);
	else
		MoveSellGirderGroup(nG, col, bAdd);
		
	EnableButton();
	m_pGrid->SetFocus();
}

void CJRibPosition::OnButtonMoveleft() 
{
	MoveLeftRight(TRUE);	
//	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CJRibPosition::OnButtonMoveright() 
{
	MoveLeftRight(FALSE);	
//	SetDataInit();
	DrawInputDomyunView(FALSE);
}

// Button 처리 (Enable)
void CJRibPosition::EnableButton()
{
	long	row = m_pGrid->GetFocusCell().row,
			col = m_pGrid->GetFocusCell().col;

	if(m_pGrid->GetCols() == 2) 
	{
		if(row%RIB_CONTEXTSU == 1)
		{
			GetDlgItem(IDC_BUTTON_MOVELEFT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOVERIGHT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_MOVELEFT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_MOVERIGHT)->EnableWindow(TRUE);
		}
	}
	else
	{
		if(m_pGrid->GetTextMatrix(row,col) == "")
		{
			GetDlgItem(IDC_BUTTON_MOVELEFT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MOVERIGHT)->EnableWindow(FALSE);
		}
		else
		{
			if(col == m_pGrid->GetCols()-1 && row%RIB_CONTEXTSU == 1){
				GetDlgItem(IDC_BUTTON_MOVELEFT)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_MOVERIGHT)->EnableWindow(FALSE);
			}
			else {
				GetDlgItem(IDC_BUTTON_MOVELEFT)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_MOVERIGHT)->EnableWindow(TRUE);
			}
		}
	}
}

void CJRibPosition::SetGridDataByChangeLengthType(CGridCtrl *pGrid, int nOldType, double dOldDisCenter)
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	long nRows = pGrid->GetRows();
	long nCols = pGrid->GetCols();

	for (long nRow = 1; nRow < nRows; nRow += RIB_CONTEXTSU)   {
		CPlateGirderApp *pGir = pBridge->GetGirder(nRow/RIB_CONTEXTSU - 1);
		double staStt = pGir->GetSSS(OFF_STT_GIRDER);
		double sta, lenSum = 0;
		for (long nCol = 1; nCol < nCols; nCol++)   {
			lenSum += pGrid->GetTextMatrixDouble(nRow, nCol);
			if(nCol == nCols-1) 
			{
				sta = pGir->GetSSS(OFF_END_GIRDER);
			}
			else
			{
				if (nOldType == 0)
					sta = pGir->GetStationByActLength(staStt, lenSum);
				else
					sta = pGir->GetLine()->GetStationByDisLength(staStt, lenSum, dOldDisCenter);
			}
			SetMatrixByStation(pGrid, nRow, nCol, sta);
		}
	}

	pGrid->Invalidate();
}


void CJRibPosition::OnJribBridgelen() 
{
	// TODO: Add your control notification handler code here
	long nOldType = m_nLengthType;
	double dOldDisCenter = m_dDisCenterOnJRib;
	UpdateData();
	SetGridDataByChangeLengthType(&m_uGrid, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_lGrid, nOldType, dOldDisCenter);
}

void CJRibPosition::OnJribGirderlen() 
{
	// TODO: Add your control notification handler code here
	long nOldType = m_nLengthType;
	double dOldDisCenter = m_dDisCenterOnJRib;
	UpdateData();
	SetGridDataByChangeLengthType(&m_uGrid, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_lGrid, nOldType, dOldDisCenter);
}

void CJRibPosition::OnKillfocusJribDistanceCenter(NMHDR* nmgv, LRESULT*)
{
	long nOldType = m_nLengthType;
	double dOldDisCenter = m_dDisCenterOnJRib;
	UpdateData();
	SetGridDataByChangeLengthType(&m_uGrid, nOldType, dOldDisCenter);
	SetGridDataByChangeLengthType(&m_lGrid, nOldType, dOldDisCenter);
	
}

void CJRibPosition::OnCheckSameapply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pGrid->SetFocus();
}

void CJRibPosition::OnJribArrangeThisgirder() 
{
	// TODO: Add your control notification handler code here
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	long nCurRow = m_pGrid->GetFocusCell().row;
	if(nCurRow<=0) nCurRow = 1;
	nCurRow -= (nCurRow - 1) % RIB_CONTEXTSU;

	CString str;
	CString str2 = m_bUpper ? "상부" : "하부";
	str.Format("  %d번 거더 기준으로 %s종리브를 설정합니다. \n    계속하시겠습니까?", GetGirByRow(nCurRow)+1, str2);
	if (IDCANCEL == MessageBox(str, "모든 거더 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	long nRows = m_pGrid->GetRows();
	long nCols = m_pGrid->GetCols();
	long nEndCol = m_pGrid->GetValidCol(nCurRow, FALSE);
	CPlateGirderApp *pCurGir = pBridge->GetGirder(GetGirByRow(nCurRow));

	for (long nRow = 1; nRow < nRows; nRow += RIB_CONTEXTSU)   {
		if (nRow == nCurRow)
			continue;
		CPlateGirderApp *pGir = pBridge->GetGirder(GetGirByRow(nRow));
		long nJijum = 0;
		double staCurStt = pCurGir->GetSSS(OFF_STT_GIRDER);
		double staCurEnd = staCurStt;
		double staStt = pGir->GetSSS(OFF_STT_GIRDER);
		double staEnd = staStt;
		long nCol = 0;
		for (nCol = 1; nCol < nEndCol; nCol++)
		{
			double staCur = GetStationMatrix(m_pGrid, nCurRow, nCol);
			double staCurMid = pCurGir->GetStationByOneStepMid(staCur, 0);
			if (staCur > staCurEnd)   {
				nJijum++;
				staCurStt = staCurEnd;
				staCurEnd = pCurGir->GetStationOnJijum(nJijum);
				staStt = staEnd;
				staEnd = pGir->GetStationOnJijum(nJijum);
			}
			double staMid = (staCurMid - staCurStt) / (staCurEnd - staCurStt) * (staEnd - staStt) + staStt;
			staMid = pGir->GetStationByOneStepMid(staMid, 0);
			double sta = staMid + (staCur - staCurMid);
			SetMatrixByStation(m_pGrid, nRow, nCol, sta);
			for (long item = 1; item < RIB_CONTEXTSU; item++)
				m_pGrid->SetTextMatrix(nRow + item, nCol, m_pGrid->GetTextMatrix(nCurRow + item, nCol));
		}
		SetMatrixByStation(m_pGrid, nRow, nCol, pGir->GetSSS(OFF_END_GIRDER));
		for (long item = 1; item < RIB_CONTEXTSU; item++)
			m_pGrid->SetTextMatrix(nRow + item, nCol, m_pGrid->GetTextMatrix(nCurRow + item, nCol));

		for (nCol++ ; nCol < nCols; nCol++)   {
			for (long item = 1; item < RIB_CONTEXTSU; item++)
				m_pGrid->SetTextMatrix(nRow + item, nCol, "");
		}
	}
	m_pGrid->DeleteUnusedColumn();
	m_pGrid->Invalidate();

	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CJRibPosition::OnJribAutoSymmetry() 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	if( m_bSameApplySameCol )
	{
		for (long nG = -1; nG < pBridge->GetGirdersu(); nG++)
			JRibAutoSymmetry(nG);
	}
	else
		JRibAutoSymmetry(GetGirByRow(m_pGrid->GetFocusCell().row));

	m_pGrid->DeleteUnusedColumn();
	SetGridTitleColum(m_pGrid);
	m_pGrid->Invalidate();

	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CJRibPosition::JRibAutoSymmetry(long nG)
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();

	long nCols = m_pGrid->GetCols();
	long nRow  = nG*RIB_CONTEXTSU + 3;
	long nPier =1;
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);
	double SttSta = pGir->GetSSS(OFF_STT_GIRDER);
	double EndSta = pGir->GetSSS(OFF_END_GIRDER);
	double HalfSta = (SttSta + EndSta) / 2.0;  // 교량 교대부 사이 중간 == 대칭점(기준점)
	double sta, staMid, staSym;
	CDDWordArray StaArr;
	long nCenCol = 0;
	for (nCenCol = 1; nCenCol < nCols; nCenCol++)
	{
		sta = GetStationMatrix(m_pGrid, nRow, nCenCol);
		if(sta > pGir->GetStationOnJijum(nPier))
		{						
			if(nPier >= pBridge->m_nQtyJigan) break;
			SttSta = pGir->GetStationOnJijum(nPier);
			EndSta = pGir->GetStationOnJijum(pBridge->m_nQtyJigan-nPier);
			HalfSta = (SttSta + EndSta) / 2.0;
			++nPier;
		}
		if (sta == HalfSta)   {  // 이런 경우는 진짜 드물거야...
			nCenCol++;
			break;
		}
		if (sta > HalfSta)
			break;
		staMid = pGir->GetStationByOneStepMid(sta, 0, BX_CROSSBEAM_VBRACING|BX_VSTIFF);
		staSym = pGir->GetStationByOneStepMid(HalfSta+(HalfSta-staMid), 0, BX_CROSSBEAM_VBRACING|BX_VSTIFF);  // 이동전의 상태임.
		staSym -= sta - staMid;  // 이동량 보정.
		StaArr.Add(staSym);
	}
	long nNewCols = nCenCol + StaArr.GetSize() + 1;
	if (m_pGrid->GetCols() < nNewCols)
		m_pGrid->SetColumnCount(nNewCols);

	long nSymCol = StaArr.GetSize();
	for ( ; nCenCol < nNewCols - 1; nCenCol++)
	{
		staSym = StaArr.GetAt(nSymCol - 1);
		SetMatrixByStation(m_pGrid, nRow, nCenCol, staSym);
		m_pGrid->SetTextMatrix(nRow+1, nCenCol + 1, m_pGrid->GetItemText(nRow+1, nSymCol));
		nSymCol--;
	}
	// 마지막 구간 저장
	SetMatrixByStation(m_pGrid, nRow, nCenCol, pGir->GetSSS(OFF_END_GIRDER));

	// 뒤에 남은 Data 삭제.
	for (nCenCol++; nCenCol < nCols; nCenCol++)   {
		for (long item = 1; item < RIB_CONTEXTSU; item++)
			m_pGrid->SetTextMatrix(nRow + item, nCenCol, "");
	}
}

void CJRibPosition::OnJribItemtype() 
{
	UpdateData();
	SetGridTitleSub(&m_uGrid, m_uGrid.GetColumnCount());
	SetGridTitleSub(&m_lGrid, m_lGrid.GetColumnCount());
}

void CJRibPosition::OnJribdetailRecommendLower1() 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
		SetDataDefaultLower(nG, 0);		

	SetDataSave();
	SetDataInit();
}

void CJRibPosition::OnJribdetailRecommendLower2() 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
		SetDataDefaultLower(nG,1);		
	SetDataSave();
	SetDataInit();
}

void CJRibPosition::OnJribdetailRecommendLower3() 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
		SetDataDefaultLower(nG, 2);		
}

void CJRibPosition::OnJribdetailRecommendUpper1() 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
		SetDataDefaultUpper(nG, 0);
	SetDataSave();
	SetDataInit();
}

void CJRibPosition::OnJribdetailRecommendUpper2() 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
		SetDataDefaultUpper(nG, 1);	
	SetDataSave();
	SetDataInit();
}

void CJRibPosition::OnJribdetailRecommendUpper3() 
{
	CPlateBridgeApp* pBridge = m_pStd->GetBridge();
	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
		SetDataDefaultUpper(nG, 2);	
	SetDataSave();
	SetDataInit();
}

BOOL CJRibPosition::IsValid()
{
	return	TRUE;
//	CPlateBridgeApp  *pBridge = m_pStd->GetBridge();
//	CGlobarOption    *pOpt    = m_pStd->GetDataManage()->GetGlobalOption();
//	
//	if(pBridge->GetBridgeType()==SOSU_GIR || pOpt->GetDesignHStiffInstall()==1)	// 소수주형
//		return FALSE;
//	else
//		return TRUE;
}


//해석이 끝난후 수평보강재 입력 위치를 교번구간 고려하여 자동 셋팅하기...070104..KB
void CJRibPosition::OnWhstiffAutosetting() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();	
	
	if(!pData->m_bReadFrameModelingOutput)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}

	long nArrSize = pData->m_ArrMaxZeroSta.GetSize();
	if(nArrSize == 0)
	{
		AfxMessageBox("수평보강재 관련 교번구간 데이타가 없습니다.\n단면요약도 입력화면을 실행시켜주십시요.");
		return;
	}

	UpdateData(TRUE);

	for(long nG = -1;nG < pBridge->GetGirdersu();nG++)
	{						
		SetDataAutoSettingUpper(nG);
		SetDataAutoSettingLower(nG);		
	}	
	SetDataSave();	
}

void CJRibPosition::SetDataAutoSettingUpper(long nG)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(nG);
	CGridCtrl       *pGridU  = &m_uGrid;	
	
	CFemStressCalc  StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CCalcData		*pData		=  m_pStd->GetDataManage()->GetCalcData();
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;
	CPtrArray BxArr;

	__int64 bxFlag = BX_PLATEHSTIFF;//BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_STT_GIRDER|BX_END_GIRDER;

	//long nCols = pGir->m_pRangePress->m_ArrayUpper.GetSize() + 1;
	long row   = GetRowByGir( nG );

	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

	double staX1 = 0;
	double dCurSta = 0;
	long	nCnt = 0;

	//070109...다시 도전...너무 난감한경우가 많다....KB...
	//상하부 별도로 다시 계산...
	StressCalc.InsertionSort(pData->m_ArrMaxZeroSta);
	long nArrSize = pData->m_ArrMaxZeroSta.GetSize();
	
	for (long nIdx = 0; nIdx < nArrSize; nIdx++)//nIdx += 2)   
	{			
		staX1 = pData->m_ArrMaxZeroSta[nIdx];

		if (nCnt%2 ==0)
			dCurSta = staX1 + m_dCheckLength;
		else
			dCurSta = staX1 - m_dCheckLength;
	
		pBx = pGir->GetBxByStation(dCurSta,bxFlag);			
		BxArr.Add(pBx);
		nCnt++;
	}

	BxArr.Add( Finder.GetBxLast(BX_END_GIRDER) );

	long nMin = BxArr.GetSize();	
	SetGridTitleSub(&m_uGrid, nMin+1);	

	CString str;
	for(long i=0; i<nMin; i++)
	{
		pBx = (CPlateBasicIndex*)BxArr[i]; 
		SetMatrixByStation(pGridU, row, i+1, pBx->GetStation());
		str.Format("%s",(i%2 == 0) ? sPressRange : sStretchRange);
		pGridU->SetTextMatrix(row+1,i+1,str);
		pGridU->SetItemState(row+1, i+1, pGridU->GetItemState(row+1,i+1) | GVIS_READONLY);
		pGridU->SetItemBkColour(row+1, i+1, RGBREADONLY);						
	}
	pGridU->SetRedraw(TRUE,TRUE);
}

void CJRibPosition::SetDataAutoSettingLower(long nG)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = pBridge->GetGirder(nG);
	CGridCtrl       *pGridL  = &m_lGrid;
	
	CFemStressCalc  StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CCalcData		*pData		=  m_pStd->GetDataManage()->GetCalcData();
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;
	CPtrArray BxArr;

	__int64 bxFlag = BX_PLATEHSTIFF;//BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_STT_GIRDER|BX_END_GIRDER;
	
	//long nCols = GetRecommendLowerCols(nG);	
	long row   = GetRowByGir(nG);

	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

	double staX1 = 0;
	double dCurSta = 0;
	long	nCnt = 0;

	StressCalc.InsertionSort(pData->m_ArrMinZeroSta);
	long nArrSize = pData->m_ArrMinZeroSta.GetSize();
	
	for (long nIdx = 0; nIdx < nArrSize; nIdx++)//nIdx += 2)   
	{			
		staX1 = pData->m_ArrMinZeroSta[nIdx];

		if (nCnt%2 ==0)
			dCurSta = staX1 - m_dCheckLength;
		else
			dCurSta = staX1 + m_dCheckLength;
	
		pBx = pGir->GetBxByStation(dCurSta,bxFlag);			
		BxArr.Add(pBx);
		nCnt++;
	}

	BxArr.Add( Finder.GetBxLast(BX_END_GIRDER) );

	long nMin = BxArr.GetSize();	
	SetGridTitleSub(&m_lGrid, nMin+1);	

	CString str;
	for(long i=0; i<nMin; i++)
	{
		pBx = (CPlateBasicIndex*)BxArr[i]; 
		SetMatrixByStation(pGridL, row, i+1, pBx->GetStation());
		str.Format("%s",(i%2) ? sPressRange : sStretchRange);
		pGridL->SetTextMatrix(row+1,i+1,str);
		pGridL->SetItemState(row+1, i+1, pGridL->GetItemState(row+1,i+1) | GVIS_READONLY);
		pGridL->SetItemBkColour(row+1, i+1, RGBREADONLY);						
	}
	pGridL->SetRedraw(TRUE,TRUE);
}
