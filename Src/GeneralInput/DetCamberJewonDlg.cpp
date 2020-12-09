// DetCamberJewonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetCamberJewonDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetCamberJewonDlg dialog


CDetCamberJewonDlg::CDetCamberJewonDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetCamberJewonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetCamberJewonDlg)
	m_nUnitCM = 0;
	m_nDistanceType = 1;
	//}}AFX_DATA_INIT
	m_nG = 0;
//	m_nQtyCamberType = 13;
	m_nQtyCamberType = 11;
}


void CDetCamberJewonDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetCamberJewonDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_EDIT_DISTANCE, m_dDistance);
	DDX_Radio(pDX, IDC_RADIO_UNIT_CENTIMETER, m_nUnitCM);
	DDX_Radio(pDX, IDC_RADIO_REAL_DISTANCE, m_nDistanceType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_CALC, m_btnCallCalc);
	DDX_Control(pDX, IDC_BUTTON_AUTOMATIC_SYMMETRY, m_btnAutoSymmetry);
	DDX_Control(pDX, IDC_BUTTON_APPLY_INITIAL_ALLITEM, m_btnInitialItem);
	DDX_Control(pDX, IDC_BUTTON_INIT_STATION, m_btnInitialStation);
	DDX_Control(pDX, IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER, m_btnAllSameGirder);
	DDX_Control(pDX, IDC_BUTTON_EXCHANGE_SYMMETRYGIRDER, m_btnExchangeGirder);
	DDX_Control(pDX, IDC_BUTTON_INPUT_FINISH, m_btnInput);
}

BEGIN_MESSAGE_MAP(CDetCamberJewonDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetCamberJewonDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_INITIAL_ALLITEM		, OnButtonApplyInitialAllItem	)
	ON_BN_CLICKED(IDC_BUTTON_EXCHANGE_SYMMETRYGIRDER	, OnButtonExchangeSymmetryGirder)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER	, OnButtonApplyIdentityAllGirder)
	ON_BN_CLICKED(IDC_BUTTON_AUTOMATIC_SYMMETRY			, OnButtonAutomaticSymmetry		)
	ON_BN_CLICKED(IDC_RADIO_UNIT_CENTIMETER, OnRadioUnitCentimeter)
	ON_BN_CLICKED(IDC_RADIO_UNIT_MILIMETER, OnRadioUnitMilimeter)
	ON_BN_CLICKED(IDC_RADIO_BASIS_DISTANCE, OnRadioBasisDistance)
	ON_BN_CLICKED(IDC_RADIO_REAL_DISTANCE, OnRadioRealDistance)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnDialogNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnDialogPrev)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_FINISH, OnButtonInputFinish)
	ON_BN_CLICKED(IDC_BUTTON_CALC, OnButtonAutoCalc)
	ON_BN_CLICKED(IDC_BUTTON_INIT_STATION, OnButtonInitStation)
	ON_EN_KILLFOCUS(IDC_EDIT_DISTANCE, OnKillfocusCamberDistanceCenter)
	//}}AFX_MSG_MAP
	
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_COPIED_DATA, IDC_GRID, OnCellCopiedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetCamberJewonDlg message handlers

void CDetCamberJewonDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	//
	CDomyun Dom(pDom);
	CAPlateDrawCamber Draw(m_pStd->GetDataManage());	

	Draw.AllDrawCamber(&Dom, m_nG);

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetCamberJewonDlg::SetGridTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CString str;
//	str.Format("%s - 솟음량 입력 [G%d]", pDB->m_sNameBridge, m_nG+1/*+pDB->GetNumberStartGirder()*/);
//	SetWindowText(str);

	CPlateGirderApp* pGir = pDB->GetGirder(m_nG);
	long cCamber = pGir->GetQtyNodeCamber();
	if(cCamber < 1)
		return;
	m_Grid.SetColumnCount(cCamber+1);
	m_Grid.SetRowCount(m_nQtyCamberType+1);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);	
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));

	m_Grid.SetColumnWidth(0, 210);
	long nCol = 0;
	for(nCol = 1; nCol < cCamber+1 ; nCol++) 
	{		
		m_Grid.SetColumnWidth(nCol, 50);
		m_Grid.SetTextMatrix(0, nCol, "%g", (double)nCol);
	}

	CString szText = _T("");
	long nRow = 0;
	m_Grid.SetTextMatrix(nRow++, 0, "구     분", (UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++, 0, "길이 (mm)", (UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++, 0, "Station (m)", (UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++, 0, "δ1 : 종단곡선", (UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++, 0, "δ2 : 편경사", (UINT)DT_LEFT);
	m_Grid.SetTextMatrix(nRow++, 0, "δ3 : 강재자중", (UINT)DT_LEFT);
	if(pDB->IsTUGir())
	{
		m_Grid.SetTextMatrix(nRow++, 0, "δ4 : 구속콘크리트자중", (UINT)DT_LEFT);
		m_Grid.SetTextMatrix(nRow++, 0, "δ5 : 180˚ Turnning", (UINT)DT_LEFT);
	}
	szText.Format("δ%d : 합성전 1차 사하중", nRow-2);
	m_Grid.SetTextMatrix(nRow++, 0, szText, (UINT)DT_LEFT);
	szText.Format("δ%d : 합성전 2차 사하중", nRow-2);
	m_Grid.SetTextMatrix(nRow++, 0, szText, (UINT)DT_LEFT);
	if(pDB->IsTUGir())
	{
		m_Grid.SetTextMatrix(nRow++, 0, "δ8 : 공장제작시 (δ3 ~ δ7의 합계)", (UINT)DT_LEFT);
		m_Grid.SetTextMatrix(nRow++, 0, "δ9 : 현장거치시 (δ8-δ3-δ4-δ5)", (UINT)DT_LEFT);
	}
	else
	{
		m_Grid.SetTextMatrix(nRow++, 0, "δ6 : 공장제작시 (δ3 ~ δ5의 합계)", (UINT)DT_LEFT);
		m_Grid.SetTextMatrix(nRow++, 0, "δ7 : 현장거치시 (δ6 - δ3)", (UINT)DT_LEFT);
	}
	szText.Format("δ%d : 1차 사하중 재하시 (δ%d - δ%d)", nRow-2, nRow-3, nRow-6);
	m_Grid.SetTextMatrix(nRow++, 0, szText, (UINT)DT_LEFT);
	szText.Format("δ%d : 2차 사하중 재하시 (δ%d - δ%d)", nRow-2, nRow-3, nRow-6);
	m_Grid.SetTextMatrix(nRow++, 0, szText, (UINT)DT_LEFT);

	for(nCol=1; nCol < cCamber+1 ; nCol++)
	{
		for(long nRow=5; nRow<m_nQtyCamberType-3; nRow++)
		{
			m_Grid.SetItemState(nRow, nCol, GVIS_MODIFIED);
			m_Grid.SetItemBkColour(nRow, nCol, RGB(225, 250, 250));
		}
	}
	for( nCol=1; nCol < cCamber+1 ; nCol++) 
	{
		m_Grid.SetItemState(1, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(1, nCol, RGBREADONLY);
		m_Grid.SetItemState(2, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(2, nCol, RGBREADONLY);
		m_Grid.SetItemState(3, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(3, nCol, RGBREADONLY);
		m_Grid.SetItemState(4, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(4, nCol, RGBREADONLY);

		m_Grid.SetItemState(m_nQtyCamberType-3, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(m_nQtyCamberType-3, nCol, RGBREADONLY);
		m_Grid.SetItemState(m_nQtyCamberType-2, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(m_nQtyCamberType-2, nCol, RGBREADONLY);
		m_Grid.SetItemState(m_nQtyCamberType-1, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(m_nQtyCamberType-1, nCol, RGBREADONLY);		
		m_Grid.SetItemState(m_nQtyCamberType, nCol, GVIS_READONLY);
		m_Grid.SetItemBkColour(m_nQtyCamberType, nCol, RGBREADONLY);
	}
	m_Grid.SetRedraw(TRUE,TRUE);

}
	
void CDetCamberJewonDlg::SetDataInit()
{
	CString			sText	= _T("");
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();

	m_nDistanceType	= pDB->m_nLengthTypeOnCamber;
	
	sText.Format("%g",	pDB->m_dDisCenterOnCamber);

	m_dDistance.SetWindowText(sText);

	SetGridTitle();

	m_nUnitCM = pDB->m_cCamberUnitToMM;

	UpdateData(FALSE);

	double	sta;
	CString str,strDan;

	CPlateGirderApp* pGir = pDB->GetGirder(m_nG);
	long cNode = pGir->GetQtyNodeCamber();
	double dStaPre = pGir->GetStationOnJijum(0);
	strDan = m_nUnitCM ? "%.2lf" : "%.3lf" ;

	for(long nNode = 0 ; nNode < cNode; nNode++) 
	{
		CCamber *pCam = pGir->GetCamberByNode(nNode);
		if(nNode == cNode-1)
			sta  = pGir->GetStationOnJijum(pDB->m_nQtyJigan);
		else
			sta  = pCam->GetCamberStation();
		str.Format("%.1lf", sta - dStaPre);
		m_Grid.SetTextMatrix(1, nNode+1, str, (UINT)DT_RIGHT);
		m_Grid.SetTextMatrix(2, nNode+1, COMMA(sta), (UINT)DT_RIGHT);
		for (long nRow = 3; nRow < m_nQtyCamberType+1; nRow++)
		{
			if(nRow < 5)
				str.Format(strDan, DBType(pGir->GetCamberByStation(sta, nRow - 3)));//종단,편경사
			else
				str.Format(strDan, DBType(pCam->GetCamberByType(pDB->GetBridgeType(), nRow - 3)));
			m_Grid.SetTextMatrix(nRow, nNode+1, str, (UINT)DT_RIGHT);
//			if (nRow < 4 || nRow > pDB->IsTUGir()?8:6)
//			{
//				m_Grid.SetItemState(nRow, nNode+1, m_Grid.GetItemState(nRow, nNode+1) | GVIS_READONLY);
//				m_Grid.SetItemBkColour(nRow, nNode+1, RGBREADONLY);
//			}
//			else
//			{
//				m_Grid.SetItemState(nRow, nNode+1, m_Grid.GetItemState(nRow, nNode+1) & ~GVIS_READONLY);
//				m_Grid.SetItemBkColour(nRow, nNode+1, RGBEDITABLE);
//			}
		}
		dStaPre = sta;
	}

	m_Grid.Invalidate();
	UpdateData(FALSE);

//	GetDataArrIndex();
//	SetDataCurIndex();
}

void CDetCamberJewonDlg::SetDataDefault(long nG)
{
	m_nUnitCM = 1;
	UpdateData();

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	pGir->MakeCamber(Finder.GetCountBx());

	long nCamber = 0;
	while (pBx)
	{
		double dSta = pBx->GetStation();		
		pGir->GetCamberByNode(nCamber++)->SetCamberStation(dSta);
		pBx = Finder.GetBxNext();
	}
}

void CDetCamberJewonDlg::SetDataSave()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pDB->GetGirder(m_nG);
	
	CString	sText	= _T("");
	double	dCamber = 0;
	double	dSta	= pGir->GetStationOnJijum(0);
	long	cCol	= m_Grid.GetColumnCount();

	UpdateData(TRUE);
	pDB->m_nLengthTypeOnCamber = m_nDistanceType;

	m_dDistance.GetWindowText(sText);
	pDB->m_dDisCenterOnCamber = atof(sText);

	pGir->MakeCamber(cCol-1);
	for(long nCol = 1 ; nCol < cCol ; nCol++) 
	{
		dSta += m_Grid.GetTextMatrixDouble(1, nCol);
		pGir->GetCamberByNode(nCol-1)->SetCamberStation(dSta);
		for(long nRow = 3; nRow < m_nQtyCamberType+1; nRow++)
		{
			dCamber = DBType(m_Grid.GetTextMatrixDouble(nRow, nCol),TRUE);
			pGir->GetCamberByNode(nCol-1)->SetCamberByType(pDB->GetBridgeType(), nRow-3, dCamber);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void CDetCamberJewonDlg::OnButtonApplyInitialAllItem()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	if( MessageBox("기존의 데이터를 모두 초기화시킵니다.\n계속 하시겠습니까 ?","솟음도 입력 초기화",
		MB_ICONQUESTION|MB_YESNO) == IDNO ) return;

	pDB->m_nLengthTypeOnCamber = 1;
	pDB->m_cCamberUnitToMM = 0;
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);		
	
	SetGridTitle();		
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)		
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		for(long nCol = 1 ; nCol < pGir->GetQtyNodeCamber()+1 ; nCol++) 
		{		
			pGir->GetCamberByNode(nCol-1)->SetCamberStation(0);
			for(long nRow = 3; nRow < m_nQtyCamberType+1; nRow++)
			{			
				pGir->GetCamberByNode(nCol-1)->SetCamberByType(pDB->GetBridgeType(), nRow-3, 0);
			}
		}		
		SetDataDefault(nG);	
	}
			
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetCamberJewonDlg::OnButtonExchangeSymmetryGirder()
{
}

void CDetCamberJewonDlg::OnButtonApplyIdentityAllGirder()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp* pGir;

	CString	sText	= _T("");
	double	dCamber	= 0.0;
	double	dSta	= 0.0;
	long	cCol	= m_Grid.GetColumnCount();

	UpdateData(TRUE);
	pDB->m_nLengthTypeOnCamber = m_nDistanceType;

	m_dDistance.GetWindowText(sText);
	pDB->m_dDisCenterOnCamber = atof(sText);

	for(long g = 0;g < pDB->GetGirdersu();g++)
	{
		pGir = pDB->GetGirder(g);
		dCamber = 0;
		dSta = pGir->GetStationOnJijum(0);				
		pGir->MakeCamber(cCol-1);
		for(long nCol = 1 ; nCol < cCol ; nCol++) 
		{
			for(long nRow = 5; nRow < m_nQtyCamberType+1; nRow++)
			{
				dCamber = DBType(m_Grid.GetTextMatrixDouble(nRow, nCol),TRUE);
				pGir->GetCamberByNode(nCol-1)->SetCamberByType(pDB->GetBridgeType(), nRow-3, dCamber);
			}
		}	
	}
}

void CDetCamberJewonDlg::OnButtonAutomaticSymmetry()
{
	AfxMessageBox("현재 지원하지 않음");
}

///////////////////////////////////////////////////////////////////////////////

void CDetCamberJewonDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,nCol));
}


void CDetCamberJewonDlg::SetDlgTitle()
{	
//	CPlateBridgeApp	*pDB  = m_pStd->GetBridge();
		
//	m_szTitle.Format("%s - 솟음량 입력 [G%d]", pDB->m_sNameBridge, m_nG+1+pDB->GetNumberStartGirder());
	SetWindowText("솟음량 상세");
// 	DrawTitleButton();	
}

BOOL CDetCamberJewonDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	m_nG = 0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nG = 0;
	for (nG = -1; nG < pDB->GetGirdersu(); nG++)
		SetDataDefault(nG);
	
	for (nG = -1; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		if(!pGir->GetQtyNodeCamber())
			SetDataDefault(nG);	
	}

	m_nQtyCamberType = pDB->IsTUGir()?13:11;

	SetGridTitle();
	SetDataInit();
	SetDataSave();		// TEMP 	
	DrawInputDomyunView(TRUE);

#ifdef _AROAD
	GetDlgItem(IDC_CAMBER_ANALYSIS_INPUT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_AUTOFACTORY)->EnableWindow(FALSE);
#endif

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_APPLY_INITIAL_ALLITEM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_INIT_STATION, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EXCHANGE_SYMMETRYGIRDER, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_INPUT_FINISH, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_REAL_DISTANCE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_BASIS_DISTANCE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_DISTANCE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC3, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC4, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_RADIO_UNIT_CENTIMETER, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_RADIO_UNIT_MILIMETER, SZ_TOP_CENTER, SZ_TOP_CENTER);

	SetResize(IDC_BUTTON_CALC, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_AUTOMATIC_SYMMETRY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	return TRUE;
}

double CDetCamberJewonDlg::DBType(double Data,BOOL bSave /* = FALSE */)
{
	double newData = 0;

	if( bSave )
		newData = m_nUnitCM ? Data : Data*10;
	else
		newData = m_nUnitCM ? Data : Data/10;
	
	return newData;
}

void CDetCamberJewonDlg::OnRadioUnitCentimeter() 
{
	// TODO: Add your control notification handler code here
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	if( m_nUnitCM == 0 )
		return;
	m_nUnitCM = 0;
	pDB->m_cCamberUnitToMM = (BYTE)m_nUnitCM;
	SetDataInit();
}

void CDetCamberJewonDlg::OnRadioUnitMilimeter() 
{
	// TODO: Add your control notification handler code here
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	if( m_nUnitCM == 1 )
		return;
	m_nUnitCM = 1;
	pDB->m_cCamberUnitToMM = (BYTE)m_nUnitCM;
	SetDataInit();	
}

void CDetCamberJewonDlg::OnRadioBasisDistance() 
{
	CString			sText	= _T("");

	UpdateData();

	long nOldType = m_nDistanceType;

	m_dDistance.GetWindowText(sText);

	double dOldDisCenter = atof(sText);

	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CDetCamberJewonDlg::OnRadioRealDistance() 
{
	CString			sText	= _T("");

	UpdateData();
	
	long nOldType			= m_nDistanceType;
	
	m_dDistance.GetWindowText(sText);

	double dOldDisCenter	= atof(sText);

	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CDetCamberJewonDlg::SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter)
{
	long nCols = m_Grid.GetCols();

	long nRow = 1;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(m_nG);
	double staStt = pGir->GetSSS(OFF_STT_SHOE);
	double sta, lenSum = 0;
	for (long nCol = 1; nCol < nCols; nCol++)   {
		lenSum += m_Grid.GetTextMatrixDouble(nRow, nCol);
		if(nCol == nCols-1) 
		{
			sta = pGir->GetSSS(OFF_END_SHOE);
		}
		else
		{
			if (nOldType == 0)
				sta = pGir->GetStationByActLength(staStt, lenSum);
			else
				sta = pGir->GetLine()->GetStationByDisLength(staStt, lenSum, dOldDisCenter);
		}
		SetMatrixByStation(nRow, nCol, sta);
	}	

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetCamberJewonDlg::SetMatrixByStation(long nRow, long nCol, double sta)
{
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	double			lenSum	= 0;
	CString			sText	= _T("");

	for(long c = 1; c < nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);

	m_dDistance.GetWindowText(sText);
	double len = pDB->GetLengthByStationInGrid(TRUE, m_nG, sta, m_nDistanceType, atof(sText)) - lenSum;

	m_Grid.SetTextMatrix(nRow, nCol, "%.1f", len);
}

void CDetCamberJewonDlg::OnDialogNext() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	if(m_nG != pDB->GetGirdersu()-1)
	{
		m_nG++;
		SetGridTitle();
		SetDlgTitle();
		SetDataInit();
		SetDataSave();
		SetDataInit();
		DrawInputDomyunView(TRUE);
		return;
	}
	m_pStd->MoveNextDialog();	
}

void CDetCamberJewonDlg::OnDialogPrev() 
{
	SetDataSave();
	if(m_nG != 0)
	{
		m_nG--;
		SetGridTitle();
		SetDlgTitle();
		SetDataInit();
		SetDataSave();
		SetDataInit();
		DrawInputDomyunView(TRUE);
		return;
	}
	m_pStd->GetDetVStiffJewonDlg()->m_nCmpPos = 0;
	m_pStd->MovePrevDialog();	
}

void CDetCamberJewonDlg::OnKillfocusCamberDistanceCenter()
{
	long	nOldType	= m_nDistanceType;
	CString	sText		= _T("");

	m_dDistance.GetWindowText(sText);

	double dOldDisCenter = atof(sText);

	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CDetCamberJewonDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;
	if(nCol == m_Grid.GetColumnCount()-1)
	{
		SetDataInit();
	}

	CString str;
	int nRow = GridView->iRow;
	if(nRow == 1)
	{
		double dChangedData = atof(m_Grid.GetLastChangeData()) - m_Grid.GetTextMatrixDouble(1, nCol);
		str.Format("%.3lf", m_Grid.GetTextMatrixDouble(1, nCol+1) + dChangedData);
		m_Grid.SetTextMatrix(1, nCol+1, str, (UINT)DT_RIGHT);
	}

	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
//	DrawTotal();
}

void CDetCamberJewonDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;

	CString str;
	for (long nRow = 1; nRow < m_Grid.GetRowCount(); nRow++)
	{
		if(nRow == 1)
			str.Format("%.3lf", m_Grid.GetTextMatrixDouble(nRow, nCol+1)/2);
		else
			str = m_Grid.GetTextMatrix(nRow, nCol+1);
		m_Grid.SetTextMatrix(nRow, nCol, str, (UINT)DT_RIGHT);
		m_Grid.SetTextMatrix(nRow, nCol+1, str, (UINT)DT_RIGHT);
	}
	m_Grid.Invalidate();

	SetDataSave();
	SetGridTitle();
	SetDataInit();
	DrawInputDomyunView(TRUE);
//	DrawTotal();
	m_Grid.SetFocus();
}

void CDetCamberJewonDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	SetDataSave();
	SetGridTitle();
	SetDataInit();
	DrawInputDomyunView(TRUE);
//	DrawTotal();
	m_Grid.SetFocus();
}

void CDetCamberJewonDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;

	if(nCol == m_Grid.GetColumnCount()-1)
		return;

	CString str;
	str.Format("%.3lf", m_Grid.GetTextMatrixDouble(1, nCol)+m_Grid.GetTextMatrixDouble(1, nCol+1));
	m_Grid.SetTextMatrix(1, nCol+1, str, (UINT)DT_RIGHT);
}

void CDetCamberJewonDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{			
//	long nCol = m_Grid.GetFocusCell().col;
//	if(nCol >= 0)
//		m_pDDBGen->GetCamberDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CDetCamberJewonDlg::OnCellCopiedData(NMHDR* nmgv, LRESULT*)
{			
	SetDataSave();
	SetGridTitle();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CDetCamberJewonDlg::OnButtonInputFinish() 
{
	// TODO: Add your control notification handler code here
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();

	m_pStd->MoveNextDialog();
}

void CDetCamberJewonDlg::OnButtonAutoCalc() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();
	
	if( MessageBox("기존의 데이터가 변환됩니다.\n계속 하시겠습니까","솟음도 입력",
		MB_ICONQUESTION|MB_YESNO) == IDNO ) return;	

	for(long nG=-1; nG<pDB->GetGirdersu(); nG++)	
		SetDataDefault(nG);
	SetGridTitle();

	if(pData->m_bReadFrameModelingOutput == FALSE)
	{
		MessageBox("읽혀진 SAP File이 없습니다","솟음도 입력",MB_ICONINFORMATION);
		return;
	}	
	SapRead();

	SetDataInit();
	SetDataSave();		// TEMP 	
	SetDataInit();
	
	DrawInputDomyunView(TRUE);
}


void CDetCamberJewonDlg::SapRead()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = NULL;
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	long	sIdx = 0;
	long	eIdx = 0;	
	long	nCam = 0;

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		pGir = pDB->GetGirder(nG);
		nCam = 0;

		ModelCalc.GetJointRangeByGirder(nG, sIdx, eIdx);		

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);								
		if(Finder.GetCountBx() != pGir->GetQtyNodeCamber()) return;
		while(pBx)				
		{
			if(pBx->IsJijum())
			{
				pGir->GetCamberByNode(nCam)->SetCamberByType(pDB->GetBridgeType(), 2, 0.0);
				pGir->GetCamberByNode(nCam)->SetCamberByType(pDB->GetBridgeType(), 3, 0.0);
				pGir->GetCamberByNode(nCam)->SetCamberByType(pDB->GetBridgeType(), 4, 0.0);
			}
			else
			{
				CCamber		*pCamcber	= pGir->GetCamberByNode(nCam);
				CJoint		*pJ			= NULL;
				CFEMManage	*pManage	= NULL;
				double		dCamber		= 0;
				if(pDB->IsTUGir())
				{
					long nIdxForce = 0;
					pManage	= m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
					pJ      = pManage->GetJoint((unsigned short)sIdx);
					dCamber = pJ->GetResultForce(nIdxForce++)->m_Disp.z;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 2, -frM(dCamber));// 강재자중

					nIdxForce = 0;
					pManage = m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);
					pJ      = pManage->GetJoint((unsigned short)sIdx);

					dCamber = pJ->GetResultForce(nIdxForce++)->m_Disp.z;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 3, -frM(dCamber));// 구속콘크리트 자중

					if(!pDB->IsTUContinuousSpan())
						dCamber = pJ->GetResultForce(nIdxForce++)->m_Disp.z;
					else
						dCamber = 0;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 4, -frM(dCamber));// 180도 터닝

					dCamber = pJ->GetResultForce(nIdxForce++)->m_Disp.z;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 5, -frM(dCamber));// 합성후 1차

					nIdxForce = 0;
					pManage = m_pStd->m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
					pJ      = pManage->GetJoint((unsigned short)sIdx);
					dCamber = pJ->GetResultForce(nIdxForce++)->m_Disp.z;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 6, -frM(dCamber));// 합성후 2차
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 7, 0);
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 8, 0);
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 9, 0);
				}
				else
				{
					pManage = m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
					pJ      = pManage->GetJoint((unsigned short)sIdx);
					dCamber = pJ->GetResultForce(0)->m_Disp.z;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 2, -frM(dCamber));// 합성전

					dCamber = pJ->GetResultForce(1)->m_Disp.z;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 3, -frM(dCamber));// 합성후 1차

					pManage = m_pStd->m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
					pJ      = pManage->GetJoint((unsigned short)sIdx);
					dCamber = pJ->GetResultForce(0)->m_Disp.z;
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 4, -frM(dCamber));// 합성후 2차

					pCamcber->SetCamberByType(pDB->GetBridgeType(), 5, 0);
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 6, 0);
					pCamcber->SetCamberByType(pDB->GetBridgeType(), 7, 0);
				}
			}
			++sIdx;
			++nCam;
			pBx = Finder.GetBxNext();
		}
	}	
}

void CDetCamberJewonDlg::OnButtonInitStation() 
{
	if( MessageBox("현재 거더의 Station을 초기화시킵니다.\n계속 하시겠습니까 ? ","솟음도 입력 초기화",
		MB_ICONQUESTION|MB_YESNO) == IDNO ) return;

	SetDataDefault(m_nG);
	SetGridTitle();
	SetDataInit();

	DrawInputDomyunView(TRUE);	
}

BOOL CDetCamberJewonDlg::CheckSapDataCompare()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CPlateGirderApp	*pGir		= NULL;
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	BOOL	bEqual	= TRUE;
	long	nIdx	= 0;
	long	nEIdx	= 0;	
	long	nCam	= 0;

	for(long nG = 0;nG < pDB->GetGirdersu();nG++)
	{
		pGir	= pDB->GetGirder(nG);
		nCam	= 0;
		ModelCalc.GetJointRangeByGirder(nG, nIdx, nEIdx);

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		if(Finder.GetCountBx() != pGir->GetQtyNodeCamber() ) return FALSE;
		while(pBx)
		{
			if(pBx->IsJijum())
			{
				if(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 2) != 0.0) { bEqual = FALSE; break; }
				if(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 3) != 0.0) { bEqual = FALSE; break; }
				if(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 4) != 0.0) { bEqual = FALSE; break; }				
			}
			else
			{
				CFEMManage		*pManage	= m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
				CJoint			*pJ			= pManage->GetJoint((unsigned short)nIdx);
				CResultForce	*pResult	= pJ->GetResultForce(0);
				if(pResult)
				{

					double dCValue = DBType(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 2), FALSE);
					double dFValue = -frM(DBType(pResult->m_Disp.z, FALSE));
					if(!EQUAL(dCValue,dFValue,0.01)) {	bEqual = FALSE; break; }

					if(pDB->IsTUGir())
					{
						pManage	= m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);
						pJ      = pManage->GetJoint((unsigned short)nIdx);
						dCValue = DBType(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 3), FALSE);
						dFValue = -frM(DBType(pJ->GetResultForce(0)->m_Disp.z, FALSE));
						if(!EQUAL(dCValue,dFValue,0.01)) { bEqual = FALSE; break; }

						dCValue = DBType(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 4), FALSE);
						dFValue = -frM(DBType(pJ->GetResultForce(1)->m_Disp.z, FALSE));
						if(!EQUAL(dCValue,dFValue,0.01)) { bEqual = FALSE; break; }

						dCValue = DBType(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 5), FALSE);
						dFValue = -frM(DBType(pJ->GetResultForce(2)->m_Disp.z, FALSE));
						if(!EQUAL(dCValue,dFValue,0.01)) { bEqual = FALSE; break; }

						pManage	= m_pStd->m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
						pJ      = pManage->GetJoint((unsigned short)nIdx);
						dCValue = DBType(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 6), FALSE);
						dFValue = -frM(DBType(pJ->GetResultForce(0)->m_Disp.z, FALSE));
						if(!EQUAL(dCValue,dFValue,0.01))	{ bEqual = FALSE; break; }
					}
					else
					{
						pManage	= m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
						pJ      = pManage->GetJoint((unsigned short)nIdx);
						dCValue = DBType(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 3), FALSE);
						dFValue = -frM(DBType(pJ->GetResultForce(1)->m_Disp.z, FALSE));
						if(!EQUAL(dCValue,dFValue,0.01)) { bEqual = FALSE; break; }

						pManage	= m_pStd->m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
						pJ      = pManage->GetJoint((unsigned short)nIdx);
						dCValue = DBType(pGir->GetCamberByNode(nCam)->GetCamberByType(pDB->GetBridgeType(), 4), FALSE);
						dFValue = -frM(DBType(pJ->GetResultForce(0)->m_Disp.z, FALSE));
						if(!EQUAL(dCValue,dFValue,0.01))	{ bEqual = FALSE; break; }
					}
				}
			}
			++nIdx;
			++nCam;
			pBx = Finder.GetBxNext();
		}
	}

	return bEqual;
}

void CDetCamberJewonDlg::SetCamber()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(m_nG);

	double	dCamber = 0;
	double	dSta	= 0;;
	long	nNodeCamber	= pGir->GetQtyNodeCamber();

	for(long nCol = 1 ; nCol < nNodeCamber ; nCol++) 
	{
		dSta = pGir->GetCamberByNode(nCol-1)->GetCamberStation();		
		for(long nRow = 3; nRow < m_nQtyCamberType+1; nRow++)
		{
			dCamber = pGir->GetCamberByNode(nCol-1)->GetCamberByType(pDB->GetBridgeType(), nRow-3);
			pGir->GetCamberByNode(nCol-1)->SetCamberByType(pDB->GetBridgeType(), nRow-3, dCamber);
		}
	}

}
