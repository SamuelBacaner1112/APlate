// SelectLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAXIPCOUNT	100	// 최대 IP수

/////////////////////////////////////////////////////////////////////////////
// CSelectLineDlg dialog

CSelectLineDlg::CSelectLineDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CSelectLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectLineDlg)
	m_strFilePath = _T("");
	m_strLineName = _T("");
	m_strStationStt = _T("");
	m_nLineImport = -1;
	//}}AFX_DATA_INIT


	m_bUsedFlag		=	TRUE;	// 항상선택할수 있도록
	m_dVScaleDraw	=	5.00;
	m_bJongDan = FALSE;

}

CSelectLineDlg::~CSelectLineDlg()
{

}


void CSelectLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectLineDlg)
	DDX_Control(pDX, IDC_STATIC_LINENAME, m_stcLineName);
	DDX_Control(pDX, IDC_EDIT_LINENAME, m_edtLineName);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_edtFilePath);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_LINENAME, m_strLineName);
	DDX_Text(pDX, IDC_EDIT_STATION_STT, m_strStationStt);
	DDX_Radio(pDX, IDC_RADIO_LINEIMPORT, m_nLineImport);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Control(pDX, IDC_BUTTON_SYMLR, m_btnSymLR);
	DDX_Control(pDX, IDC_BUTTON_LINESAVE, m_btnLineSave);
	DDX_Control(pDX, IDC_BUTTON_LINEOPEN, m_btnLineOpen);
	DDX_Control(pDX, IDC_BUTTON_INSERT_ROW, m_btnInsert);
	DDX_Control(pDX, IDC_BUTTON_EQLR, m_btnEQLR);
	DDX_Control(pDX, IDC_BUTTON_DELETE_ROW, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_ADD_ROW, m_btnAdd);
	DDX_Control(pDX, IDC_COMBO_BRIDGE, m_ctlBridgeType);
	DDX_Control(pDX, IDC_COMBO_LINE, m_ctlLineSelect);
	DDX_Check(pDX, IDC_CHECK_JONGDAN, m_bJongDan);
}


BEGIN_MESSAGE_MAP(CSelectLineDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CSelectLineDlg)
	ON_BN_CLICKED(IDC_RADIO_LINEIMPORT, OnRadioLineimport)
	ON_BN_CLICKED(IDC_RADIO_LINEINPUT, OnRadioLineinput)
	ON_BN_CLICKED(IDC_BUTTON_LINEOPEN, OnButtonLineopen)
	ON_BN_CLICKED(IDC_BUTTON_LINESAVE, OnButtonLinesave)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE, OnComboBridgeType)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ROW, OnButtonAddRow)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ROW, OnButtonDelRow)
	ON_BN_CLICKED(IDC_BUTTON_EQLR, OnButtonEqual)
	ON_BN_CLICKED(IDC_BUTTON_SYMLR, OnButtonSymtri)
	ON_BN_CLICKED(IDC_CHECK_JONGDAN, OnCheckJonDan)

END_MESSAGE_MAP()

void CSelectLineDlg::OnPreInitDialog()
{
	//Tab 초기화
	InitTabCtrl();

	// Combo 초기화 - 1
	CDataManage		*pManage	= m_pStd->GetDataManage();
	CGlobarOption	*pOpt		= pManage->GetGlobalOption();

	m_ctlBridgeType.AddString("판형교");
	m_ctlBridgeType.AddString("소수주거더교");
	if(CGeneralBaseStd::IsModuleTUGirder())
		m_ctlBridgeType.AddString("Turnover거더");
	m_ctlBridgeType.SetCurSel(pOpt->GetDesignBridgeType());

	// Combo 초기화 - 2
	SetLineName(0);

	SetViewItem(0);
}

void CSelectLineDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun				*pDom	=	m_pView->GetDomyun();

	UpdateData();
	
	if( m_pStd->GetLineInfoApp()->IsValidLineInuptData()==FALSE) 
	{
		MessageBox("선형 정보가 없습니다.\n교량파일/선형 파일 가져오기 메뉴를 먼저 실행하십시오.");
		return;
	}
	
	CString csErrorMsg(_T(""));

	if(m_pStd->GetLineInfoApp()->SetDataIp(csErrorMsg) )  // m_prjIP 데이터 대입
	{
		m_pView->GetDomyun()->ClearEtt(TRUE);
		
		int nTab = m_ctlTab.GetCurSel();
		
		switch(nTab)
		{
		case 0:	DrawLinePlane(pDom);	break;
		case 1:	DrawVertPlanEL(pDom);	break;
		case 2:	
			DrawVertGroundEL(pDom);
			if(m_bJongDan)
				DrawVertPlanEL(pDom);
			break;
		case 3:	DrawSlope(pDom,	FALSE);DrawSlope(pDom,	TRUE);break;
		case 4:
			DrawSlope(pDom,	TRUE);
			DrawSlope(pDom,	FALSE);
			break;
		}
		
	}
	if(csErrorMsg!="") MessageBox(csErrorMsg);	
	
	CGeneralBaseDlg::DrawInputDomyunView(TRUE);
}

void CSelectLineDlg::DrawLinePlane(CDomyun* pDom)
{
	CPlateBridgeApp		*pDB	=	m_pStd->GetBridge();
	CAPlateDrawPyung	DrawPyung(m_pStd->GetDataManage());
	CAPlateDrawDimPyung DrawDimPyung(m_pStd->GetDataManage());

	CDomyun Dom(m_pView->GetDomyun());
 	Dom.SetScaleDim(1);

	//m_pStd->GetLineInfoApp()->DrawLineOne(m_pView->GetDomyun(), 0, 7, dLenClothoidLine);
	m_pStd->GetLineInfoApp()->DrawLineAll(&Dom, 0);
	
	// Slab 그리기
	CPlateBxFinder		FinderSlabL(pDB->GetGirder(0));
	CPlateBxFinder		FinderSlabR(pDB->GetGirder(pDB->GetGirdersu()-1));
	
	CPlateBasicIndex	*pBxLeftStt		=	FinderSlabL.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxLeftEnd		=	FinderSlabL.GetBxFirst(BX_END_GIRDER);
	CPlateBasicIndex	*pBxRightStt	=	FinderSlabR.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxRightEnd	=	FinderSlabR.GetBxFirst(BX_END_GIRDER);
	
	if(pDB->m_dStationBridgeStt <= 0 && pDB->m_dLengthTotalSlab <= 0) return;
	
	if(pBxLeftStt && pBxLeftEnd && pBxRightStt && pBxRightEnd)
		DrawPyung.DrawSlabByBx(m_pView->GetDomyun(), pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);
	else
		DrawPyung.DrawSlabVirtual(m_pView->GetDomyun());

	m_pView->GetDomyun()->SetScaleDim(800);	

	DrawDimPyung.DimXyCenter(m_pView->GetDomyun());
	
	*pDom << Dom;
}

void CSelectLineDlg::DrawVertPlanEL(CDomyun *pDom)
{

	if(m_pStd->GetBridge()->GetLengthBridge() <= 0)
	{
		m_pView->GetDomyun()->SetLineColor(1);
		m_pView->GetDomyun()->SetTextColor(4);
		m_pView->GetDomyun()->SetTextHeight(m_pStd->GetLineInfoApp()->GetTextHeightIp());
		
		m_pStd->GetLineInfoApp()->DrawJongDan(m_pView->GetDomyun(),m_dVScaleDraw); // 종단 곡선모양
	}
	else
		DrawJongGuBaeDo();
}

void CSelectLineDlg::DrawJongGuBaeDo()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	double dStaStt = 0;
	double dStaEnd = 0;
	
	double dStaBridgeStt = pDB->m_dStationBridgeStt;
	double dStaBridgeEnd = dStaBridgeStt + pDB->GetLengthBridge();
	
	BOOL bUser = FALSE;
	
	m_pView->GetDomyun()->SetScaleDim(1);
	m_pView->GetDomyun()->SetTextHeight(m_pStd->GetLineInfoApp()->GetTextHeightIp());
	
	m_pStd->GetLineInfoApp()->DrawJongDan(m_pView->GetDomyun(),m_dVScaleDraw);
	
	m_pView->GetDomyun()->SetLineColor(7);
	m_pStd->GetLineInfoApp()->DrawJongDanCurveOnlyBridge(m_pView->GetDomyun(), dStaStt, dStaEnd, dStaBridgeStt, dStaBridgeEnd, 5, bUser);
	
	m_pView->GetDomyun()->SetLineColor(2);
	m_pView->GetDomyun()->SetTextColor(4);
	m_pStd->GetLineInfoApp()->DrawJongDanBridgeTextOut(m_pView->GetDomyun(), dStaStt, dStaEnd, dStaBridgeStt, dStaBridgeEnd, 5, bUser);
}

void CSelectLineDlg::DrawVertGroundEL(CDomyun *pDom)
{
	if(m_Grid.GetRowCount()<=2)
		return;
	
	CDataManage *pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	CDomyun Dom(pDom);
	
	Dom.SetLineColor(1);
	Dom.SetTextColor(2);
	Dom.SetTextHeight(pLine->GetTextHeightIp());
	
   	pLine->DrawJongEarthLine(&Dom, m_dVScaleDraw); // 종단 곡선모양
	
	DrawCursor(&Dom, m_Grid.GetFocusCell().row);
	
	
	*pDom << Dom;
}
void CSelectLineDlg::DrawCursor(CDomyun* pDom, int nRow)
{
	if(nRow < 0)
		return;
	
	if(nRow > 2 && m_Grid.GetTextMatrixDouble(nRow,1) < m_Grid.GetTextMatrixDouble(nRow-1,1))
		return;
	
	CDataManage		*pStd		=	m_pStd->GetDataManage();
	CLineInfoApp	*pLineApp	=	pStd->GetLineInfoApp();
	
	double JCURVERATE		=	5;
	double dSta				=	frM(m_Grid.GetTextMatrixDouble(nRow,1));
	double dEle				=	frM(m_Grid.GetTextMatrixDouble(nRow,3));
	double dStaSttJongDan	=	pLineApp->m_VIPData[0][0];
	double dStaEndJongDan	=	pLineApp->m_VIPData[pLineApp->m_VIPsu - 1][0];
	double dElevBase		=	pLineApp->GetMinSectionEL(dStaSttJongDan, dStaEndJongDan);
	
	CDomyun Dom(pDom);
	
	Dom.SetLineColor(4);
	Dom.SetPLineThick(180);

	Dom.PMoveTo(dSta,(dEle-dElevBase)*JCURVERATE-15000);
	Dom.PLineTo(dSta,(dEle-dElevBase)*JCURVERATE+15000);
	
	Dom.PLineEnd();
	
	*pDom << Dom;
}

void CSelectLineDlg::DrawSlope(CDomyun *pDom, BOOL bLeft)
{
	CDataManage *pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	// 함수 그대로 가져옴.
	// DrawPyungubae(CDomyun* pDomP, double dStaStt, double dStaEnd, BOOL bLeft, double dRatio)
	CDomyun Dom(pDom);
	
	const CTypedPtrArray <CObArray, CSlop*>& rSlopArr = bLeft ? pLine->m_pSlopLeft : pLine->m_pSlopRight; // 편경사
	
	int nTotalIP = rSlopArr.GetSize();
	
	if(nTotalIP <= 0) return;
	
	double dStaStt = 0.0, dStaEnd = 0.0;
	if(nTotalIP == 1)
	{
		dStaStt = pLine->m_dStationBp;
		dStaEnd = pLine->GetStationEpLine();
	}
	
	if(dStaStt == 0 && dStaEnd == 0)
	{
		dStaStt = rSlopArr.GetAt(0)->m_dStationPyungubae;            // 입력된 첫     IP의 Station
		dStaEnd = rSlopArr.GetAt(nTotalIP - 1)->m_dStationPyungubae; // 입력된 마지막 IP의 Station
	}
	
	long nCase = -1;
	
	if(bLeft) nCase = 1;
	else      nCase = 2;
	
	BOOL	bTotal	=  TRUE;
	BOOL	bUser	= FALSE;
	double	dRatio	= 1000000.0;

	Dom.SetTextColor(4);
	Dom.SetTextHeight(pLine->GetTextHeightIp());
	
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextAngle(0);
	Dom.SetLineColor(1);
	pLine->DrawPyungubaeRatioBar(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal, bUser);
	pLine->DrawPyungubaeCenterLine(&Dom, dStaStt, dStaEnd, nCase, bUser);
	
	long nLineColor = bLeft ? 3 : 5;

	Dom.SetLineColor(nLineColor);
	pLine->DrawPyungubaePlan(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal);
	
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.SetTextAngle(90);
	pLine->DrawPyungubaeStationDim(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal, bUser);
	
	CString str = bLeft ? _T("좌측") : _T("우측");

	Dom.SetTextColor(nLineColor);
	
	double TextHeight	=	pLine->GetTextHeightIp()*1.5;
	double dSlop		=	rSlopArr.GetAt(0)->m_dValuePyungubae;
	double dMoveX		=	bLeft ? 8000.0 + TextHeight*1.5 : 8000.0;
	double dMoveY		=	pLine->GetPyungubae(dStaStt,0)*10000.0;

	Dom.SetTextHeight(TextHeight);
	Dom.SetTextAlignHorz(TA_CENTER);
	
	Dom.TextOut(dStaStt-dMoveX, dSlop*dRatio+dMoveY, str);
	
	Dom.Invalidate();
	
	*pDom << Dom;
}

BOOL CSelectLineDlg::OnInitDialog() 
{
	BOOL bFirst = !m_pStd->GetDlgByIndex(1)->IsUsedFlag();
	
	CGeneralBaseDlg::OnInitDialog();
	
	//CSelectLineDlg Dialog Box는 m_bUsedFlag(부모클래스 멤버변수)가 항상 참이기 때문에
	//강제적으로 SetDataDefault()메소드를 실행 시켜줘야 초기화가 됨.

	if(bFirst)
	{
		SetDataDefault();
		SetDataInit();
	}

	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_ADD_ROW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE_ROW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_CHECK_JONGDAN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EQLR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SYMLR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	return TRUE;  
}

void CSelectLineDlg::InitTabCtrl()
{
	m_ctlTab.InsertItem(0, _T("평면 선형"));
	m_ctlTab.InsertItem(1, _T("종단 계획고"));
	m_ctlTab.InsertItem(2, _T("종단 지반고"));
	m_ctlTab.InsertItem(3, _T("좌측 편경사"));
	m_ctlTab.InsertItem(4, _T("우측 편경사"));
}

void CSelectLineDlg::SetGridTitle()
{
	long nRows = m_Grid.GetRowCount();
	long nCols = m_Grid.GetColumnCount();
	
	for(long nRow = 1; nRow < nRows; nRow++)
	{
		for(long nCol = 1; nCol < nCols; nCol++)
			m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) & ~GVIS_READONLY);
	}
	
	m_Grid.SetSelectedRange(0, 0, 0, 0);

	int nTab = m_ctlTab.GetCurSel();
	switch(nTab)
	{
	case 0:
		SetGridTitleLinePlane();
		break;
	case 1:
		SetGridTitleVertPlanEL();
		break;
	case 2:
		SetGridTitleVertGroundEL();
		break;
	case 3:
	case 4:
		SetGridTitleSlope();
		break;
	}
}
void CSelectLineDlg::SetGridTitleLinePlane()
{
	CDataManage *pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	long nIPs = pLine->GetQtyIp();
	long nRows = nIPs + 3;
	long nCols = 6;
	
	m_Grid.EnableInsertRow(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	int nCol = 0;
	for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}
	
	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);
	
	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("IP"));
	m_Grid.SetTextMatrix(0, nCol++, _T("X 좌표 (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Y 좌표 (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("반경 R (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("매개변수 A1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("매개변수 A2"));
	
	m_Grid.SetTextMatrix(1, 0, _T("BP"));
	CString str = _T("");
	for(long nIP=0; nIP<nIPs; nIP++)
	{
		str.Format(_T("IP %d"), nIP+1L);
		m_Grid.SetTextMatrix(nIP+2, 0, str);
	}
	m_Grid.SetTextMatrix(nIPs+2, 0, _T("EP"));
	
	for(int i=3; i<nCols; i++)
	{
		m_Grid.SetTextMatrix(1, i, _T("-"));
		m_Grid.SetItemState(1, i, m_Grid.GetItemState(1, i) | GVIS_READONLY);
		m_Grid.SetItemBkColour(1, i, RGBREADONLY);
		m_Grid.SetItemFgColour(1, i, RGB(128, 128, 128));
		m_Grid.SetTextMatrix(nRows-1, i, _T("-"));
		m_Grid.SetItemState(nRows-1, i, m_Grid.GetItemState(nRows-1, i) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRows-1, i, RGBREADONLY);
		m_Grid.SetItemFgColour(nRows-1, i, RGB(128, 128, 128));
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);

}
void CSelectLineDlg::SetGridTitleVertPlanEL()
{
	CDataManage *pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	long nRows = pLine->m_VIPsu + 1;
	long nCols = 4;
	
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	int nCol = 0;
	for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}
	
	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);
	
	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("VIP"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Station (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Elevation (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("곡선 길이 (m)"));
	CString str = _T("");
	for(long nRow=1; nRow<nRows; nRow++)
	{
		str.Format(_T("%d"), nRow);
		m_Grid.SetTextMatrix(nRow, 0, str);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);

}
void CSelectLineDlg::SetGridTitleVertGroundEL()
{
	CDataManage *pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	long nStas = pLine->m_VEarthLine.GetSize();
	long nRows = nStas + 1;
	long nCols = 3;
	
	BOOL bFirst = nRows==1 ? TRUE : FALSE;
	if(bFirst)
		nRows += 1;
	
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	long nCol = 0;
	for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}
	
	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);
	
	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("순번 (No.)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("측점 (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("지반고 (m)"));
	CString str = _T("");
	for(long nRow=1; nRow<nRows; nRow++)
	{
		str.Format(_T("%d"), nRow);
		m_Grid.SetTextMatrix(nRow, 0, str);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);

}
void CSelectLineDlg::SetGridTitleSlope()
{
	CDataManage *pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	int nTab = m_ctlTab.GetCurSel();
	
	long nRows = 0;
	if(nTab==3)
		nRows = pLine->m_pSlopLeft.GetSize() + 1;
	if(nTab==4)
		nRows = pLine->m_pSlopRight.GetSize() + 1;
	long nCols = 3;
	
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	long nCol = 0;
	for(nCol=1; nCol<nCols; nCol++)
	{
		m_Grid.SetColumnWidth(nCol, 120);
	}
	
	COLORREF clrBack = RGB(253, 250, 255);
	m_Grid.SetTextBkColor(clrBack);
	
	nCol = 0;
	m_Grid.SetTextMatrix(0, nCol++, _T("순번 (No.)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("Station (m)"));
	m_Grid.SetTextMatrix(0, nCol++, _T("편경사 (%)"));
	CString str = _T("");
	for(long nRow=1; nRow<nRows; nRow++)
	{
		str.Format(_T("%d"), nRow);
		m_Grid.SetTextMatrix(nRow, 0, str);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);

}

void CSelectLineDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	long nTab = m_ctlTab.GetCurSel();
	SetViewItem(nTab);
	SetDataInit();
	UpdateData();
	DrawInputDomyunView(FALSE);
}

void CSelectLineDlg::SetDataInit() 
{
	CDataManage *pStd	=	m_pStd->GetDataManage();
	
	//m_nLineImport = pStd->m_nHowLineInput;
	UpdateData(FALSE);
	
	//SetEnableWindow();
	
	SetGridTitle();
	m_strLineName = pStd->GetLineInfoApp()->GetNameLineEach();
	m_strStationStt.Format("%.3f",toM(pStd->GetLineInfoApp()->m_dStationBp));
	
	int nTab = m_ctlTab.GetCurSel();
	switch(nTab)
	{
	case 0:	SetDataInitLinePlane();		break;
	case 1:	SetDataInitVertPlanEL();	break;
	case 2:	SetDataInitVertGroundEL();	break;
	case 3:
	case 4:	SetDataInitSlope();			break;
	}
	
	UpdateData(FALSE);
}
void CSelectLineDlg::SetDataInitLinePlane()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	CString		str		=	_T("");
	
	//m_strFilePath = pStd->m_strLineFilePath;

	if(m_strFilePath == "")	//Import 한 선형이 없으면
		m_nLineImport = 1;
	
	m_strLineName = pLine->GetNameLineEach();
	UpdateData(FALSE);
	
	str.Format("%.6f", toM(pLine->m_xyBp.x));
	m_Grid.SetTextMatrix(1, 2, str);
	str.Format("%.6f", toM(pLine->m_xyBp.y));
	m_Grid.SetTextMatrix(1, 1, str);
	
	int nIPs = pLine->GetQtyIp();
	for(int nRow=0; nRow<nIPs; nRow++)
	{
		CIPRecord* pIpRec = &pLine->m_pIpRec[nRow];
		
		str.Format("%.6f", toM(pIpRec->m_xy_IpRecord.x));
		m_Grid.SetTextMatrix(nRow+2, 2, str);
		str.Format("%.6f", toM(pIpRec->m_xy_IpRecord.y));
		m_Grid.SetTextMatrix(nRow+2, 1, str);
		str.Format("%.6f", toM(pIpRec->m_dR_IpRecord));
		m_Grid.SetTextMatrix(nRow+2, 3, str);
		str.Format("%.6f", toM(pIpRec->m_dA1_IpRecord));
		m_Grid.SetTextMatrix(nRow+2, 4, str);
		str.Format("%.6f", toM(pIpRec->m_dA2_IpRecord));
		m_Grid.SetTextMatrix(nRow+2, 5, str);
	}
	
	str.Format("%.6f", toM(pLine->m_xyEp.x));
	m_Grid.SetTextMatrix(nIPs+2, 2, str);
	str.Format("%.6f", toM(pLine->m_xyEp.y));
	m_Grid.SetTextMatrix(nIPs+2, 1, str);
	
	m_Grid.SetRedraw(TRUE,TRUE);

}
void CSelectLineDlg::SetDataInitVertPlanEL()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	if(pLine->m_VIPsu<=1)
		return;
	
	int nRows = pLine->m_VIPsu + 1;
	
	CString str;
	for(int nRow=1; nRow<nRows; nRow++)
	{
		for(int nCol=1; nCol<4; nCol++)
		{
			str.Format("%.6f",toM(pLine->m_VIPData[nRow-1][nCol-1]));
			m_Grid.SetTextMatrix(nRow, nCol, str);
		}
	}
	
	// 종단계획고 처음과 마지막 row 에는 반경을 못 넣도록 하자 ///////////////
	if(nRows > 1)
	{
		m_Grid.SetTextMatrix(1, 3, "-");
		m_Grid.SetItemState(1, 3, m_Grid.GetItemState(1, 3) | GVIS_READONLY);
		m_Grid.SetItemBkColour(1, 3, RGBREADONLY);
		m_Grid.SetItemFgColour(1, 3, RGB(128, 128, 128));
		
		m_Grid.SetTextMatrix(nRows-1, 3, "-");
		m_Grid.SetItemState(nRows-1, 3, m_Grid.GetItemState(nRows-1, 3) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRows-1, 3, RGBREADONLY);
		m_Grid.SetItemFgColour(nRows-1, 3, RGB(128, 128, 128));
	}
	
	
	m_Grid.SetRedraw(TRUE, TRUE);
}
void CSelectLineDlg::SetDataInitVertGroundEL()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	double	dSta	=	0;
	double	dElev	=	0;
	int		nRows	=	pLine->m_VEarthLine.GetSize();

	for(int nRow=0; nRow<nRows; nRow++)
	{
		dSta = toM(pLine->m_VEarthLine.GetAt(nRow)->dSta);
		dElev = toM(pLine->m_VEarthLine.GetAt(nRow)->dEle);
		
		m_Grid.SetTextMatrix(nRow+1, 1, "%.6f", dSta);
		m_Grid.SetTextMatrix(nRow+1, 2, "%.6f", dElev);
		
		m_Grid.SetItemFormat(nRow+1, 1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		m_Grid.SetItemFormat(nRow+1, 2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
	
}
void CSelectLineDlg::SetDataInitSlope()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	CSlop		*pSlop	=	NULL;
	
	int		nTab	=	m_ctlTab.GetCurSel();
	CString str		=	_T("");
	long	nRows	=	0;
	if(nTab==3)
	{
		nRows	=	pLine->m_pSlopLeft.GetSize() + 1;
		
		
		for(long nRow=1; nRow<nRows; nRow++)
		{
			pSlop = pLine->m_pSlopLeft[nRow-1];
			
			str.Format("%.6f", toM(pSlop->m_dStationPyungubae));
			m_Grid.SetTextMatrix(nRow,1,str);
			str.Format("%.6f", pSlop->m_dValuePyungubae*100);
			m_Grid.SetTextMatrix(nRow,2,str);
		}
	}
	
	if(nTab==4)
	{
		nRows = pLine->m_pSlopRight.GetSize() + 1;
		
		for(long nRow=1; nRow<nRows; nRow++)
		{
			pSlop = pLine->m_pSlopRight[nRow-1];
			
			str.Format("%.6f", toM(pSlop->m_dStationPyungubae));
			m_Grid.SetTextMatrix(nRow,1,str);
			str.Format("%.6f", pSlop->m_dValuePyungubae*100);
			m_Grid.SetTextMatrix(nRow,2,str);
		}
	}
	
	m_Grid.SetRedraw();
}

void CSelectLineDlg::SetDataDefault()
{
	CDataManage		*pStd		= m_pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pStd->GetLineInfoApp();
	CDataManage*	pManage		= m_pStd->GetDataManage();

//	if(pStd->m_nHowLineInput==0) return;	
//	pStd->m_nHowLineInput = 1;
	
	double Length			=	520000;
	double SttSta			=	0;
	double dPyungubeLeft	=	-2.0;
	double dPyungubeRight	=	-2.0;

		
	pLineApp->MakeLineInfo(0);
	pLineApp->NewDocument();
	pLineApp->SetValidLineInuptData(TRUE);
	pLineApp->SetNameLineEach("선형1");					// 선형명
	pLineApp->SetQtyIp(0);
	pLineApp->SetTextHeightIp(frM(2.5));
	
	pLineApp->m_sNameProject		= m_pStd->GetLineInfoApp()->m_sNameProject;	// 사업명
	pLineApp->m_dStationBp			= SttSta;
	pLineApp->m_dZfactor			= 11.8;
	pLineApp->m_NeedDelete_dCantMax	= 160;
	pLineApp->m_dRadiusMax			= 2000;
	pLineApp->m_dValueC				= 0;
	pLineApp->m_dUnitCoordinate		= frM(100.0);
	pLineApp->m_dTextHeightChain	= frM(1.5);
	m_strStationStt					= "0.000";

	
	long nSizeIP = 2;
	//////////////////////////////////////////////
	// 선형
	pLineApp->m_dDisChainUnit = frM(20);
	pLineApp->m_xyBp = CDPoint(0,0);
	pLineApp->m_xyEp = CDPoint(Length,0);
	
	
	//////////////////////////////////////////////
	// 종단
	
	pLineApp->m_VIPsu = nSizeIP;
	pLineApp->m_VIPData[0][0] = pLineApp->m_dStationBp;
	pLineApp->m_VIPData[0][1] = 10000;
	pLineApp->m_VIPData[1][0] = pLineApp->GetStationEpLine(); //Station
	pLineApp->m_VIPData[1][1] = 10000;		 //Elevation
	
	
	//////////////////////////////////////////////////////////////////////////
	// 지반고
	
	CLineInfo* pLineBase = pLineApp->GetLine(0);
	AhTPAMake(nSizeIP, &pLineBase->m_VEarthLine, (CVEarthLine*)0);
	pLineBase->m_VEarthLine.GetAt(0)->dSta = pLineApp->m_dStationBp;
	pLineBase->m_VEarthLine.GetAt(0)->dEle = 2000;
	pLineBase->m_VEarthLine.GetAt(1)->dSta = pLineApp->GetStationEpLine();
	pLineBase->m_VEarthLine.GetAt(1)->dEle = 2000;
	
	//////////////////////////////////////////////
	// 편경사
	
	AhTPAMake(nSizeIP, &pLineApp->m_pSlopLeft,  (CSlop*)0);
	pLineApp->m_pSlopLeft[0]->m_dStationPyungubae = pLineApp->m_dStationBp;          // 선형시점
	pLineApp->m_pSlopLeft[0]->m_dValuePyungubae   = dPyungubeLeft/100;
	pLineApp->m_pSlopLeft[1]->m_dStationPyungubae = pLineApp->GetStationEpLine();		// 선형종점
	pLineApp->m_pSlopLeft[1]->m_dValuePyungubae   = dPyungubeLeft/100;
	
	AhTPAMake(nSizeIP, &pLineApp->m_pSlopRight, (CSlop*)0);
	pLineApp->m_pSlopRight[0]->m_dStationPyungubae = pLineApp->m_dStationBp;         // 선형시점
	pLineApp->m_pSlopRight[0]->m_dValuePyungubae   = dPyungubeRight/100;
	pLineApp->m_pSlopRight[1]->m_dStationPyungubae = pLineApp->GetStationEpLine();	// 선형종점
	pLineApp->m_pSlopRight[1]->m_dValuePyungubae   = dPyungubeRight/100;
	
	m_pStd->m_pDoc->SetModifiedFlag();
	
	m_Grid.SetFocusCell(1, 1);	
}

void CSelectLineDlg::SetDataSave()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	
	UpdateData();

	//pStd->m_nHowLineInput = m_nLineImport;
	//if(pStd->m_nHowLineInput==0) return;
	
	pStd->GetLineInfoApp()->SetNameLineEach(m_strLineName);
	
	int nTab = m_ctlTab.GetCurSel();
	switch(nTab)
	{
	case 0:	SetDataSaveLinePlane();		break;
	case 1:	SetDataSaveVertPlanEL();	break;
	case 2:	SetDataSaveVertGroundEL();	break;
	case 3:
	case 4:	SetDataSaveSlope();			break;
	}
	
	//CheckInput();
	
	m_pStd->m_pDoc->SetModifiedFlag();

}

void CSelectLineDlg::SetDataSaveLinePlane()
{

	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	UpdateData();
	pLine->SetNameLineEach(m_strLineName);
	
	long nIPs = m_Grid.GetRowCount() - 3;
	pLine->SetQtyIp(nIPs);
	
	pLine->m_xyBp.x = frM(atof(m_Grid.GetTextMatrix(1, 2)));
	pLine->m_xyBp.y = frM(atof(m_Grid.GetTextMatrix(1, 1)));
	for(int nRow=0; nRow<nIPs; nRow++)
	{
		CIPRecord* pIpRec = &pLine->m_pIpRec[nRow];
		
		pIpRec->m_xy_IpRecord.x = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 2)));
		pIpRec->m_xy_IpRecord.y = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 1)));
		pIpRec->m_dR_IpRecord     = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 3)));
		pIpRec->m_dA1_IpRecord	  = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 4)));
		pIpRec->m_dA2_IpRecord	  = frM(atof(m_Grid.GetTextMatrix(nRow + 2, 5)));
	}
	pLine->m_xyEp.x = frM(atof(m_Grid.GetTextMatrix(nIPs+2, 2)));
	pLine->m_xyEp.y = frM(atof(m_Grid.GetTextMatrix(nIPs+2, 1)));
}

void CSelectLineDlg::SetDataSaveVertPlanEL()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
/*	UpdateData();

	int nSize = m_Grid.GetRowCount() - 1;
	
	AhTPAMake(nSize, &pLine->m_VEarthLine, (CVEarthLine*)0);
	
	for(long nRow=0; nRow<nSize; nRow++)
	{
		pLine->m_VEarthLine.GetAt(nRow)->dSta = frM(m_Grid.GetTextMatrixDouble(nRow+1, 1));
		pLine->m_VEarthLine.GetAt(nRow)->dEle = frM(m_Grid.GetTextMatrixDouble(nRow+1, 2));
	}
*/

	int nRows = pLine->m_VIPsu;
	for(int nRow=0; nRow<nRows; nRow++)
	{
		for(int nCol=0; nCol<3; nCol++)
		{
			pLine->m_VIPData[nRow][nCol] = frM(m_Grid.GetTextMatrixDouble(nRow+1,nCol+1));
		}
	}	
}

void CSelectLineDlg::SetDataSaveVertGroundEL()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	int nSize = m_Grid.GetRowCount() - 1;
	
	AhTPAMake(nSize, &pLine->m_VEarthLine, (CVEarthLine*)0);
	
	for(long nRow=0; nRow<nSize; nRow++)
	{
		pLine->m_VEarthLine.GetAt(nRow)->dSta = frM(m_Grid.GetTextMatrixDouble(nRow+1, 1));
		pLine->m_VEarthLine.GetAt(nRow)->dEle = frM(m_Grid.GetTextMatrixDouble(nRow+1, 2));
	}
}

void CSelectLineDlg::SetDataSaveSlope()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	int nTab = m_ctlTab.GetCurSel();

	int nSIPs = m_Grid.GetRowCount() - 1;
	if(nTab==3)
		AhTPAMake(nSIPs, &pLine->m_pSlopLeft, (CSlop*)0);
	if(nTab==4)
		AhTPAMake(nSIPs, &pLine->m_pSlopRight, (CSlop*)0);

	CSlop *pSlop;

	double sta = 0, val = 0;

	for(long nRow=1; nRow<=nSIPs; nRow++)
	{
		sta = atof(m_Grid.GetTextMatrix(nRow, 1));
		val = atof(m_Grid.GetTextMatrix(nRow, 2));

		if(nTab==3)
			pSlop = pLine->m_pSlopLeft[nRow-1];
		if(nTab==4)
			pSlop = pLine->m_pSlopRight[nRow-1];

		pSlop->m_dStationPyungubae = frM(sta);
		pSlop->m_dValuePyungubae   = val*0.01;
	}
}

void CSelectLineDlg::CheckInput()
{
/*	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	// 1. BP, IP, EP에 중복된 위치가 있으면 평면 구조도가 정상적으로 그려지지 않을 수 있음
	BOOL bDuplicationIP	= FALSE;
	long nCountIP = pLine->GetQtyIp();	
	CDPointArray xyArr;
	xyArr.Add(pLine->m_xyBp);
	for(long ip = 0; ip < nCountIP; ip++)
		xyArr.Add((pLine->m_pIpRec+ip)->m_xy_IpRecord);
	xyArr.Add(pLine->m_xyEp);
	
	CDPoint xy1(0, 0), xy2(0, 0);
	long nSize	= xyArr.GetSize();
	for(long i = 0; i < nSize - 1; i++)
	{
		xy1	= xyArr.GetAt(i);
		for(long j = i + 1; j < nSize; j++)
		{
			xy2	= xyArr.GetAt(j);
			if(ComparePt(xy1, xy2, "="))
			{
				bDuplicationIP	= TRUE;
				break;
			}
		}
	}
	
	if(bDuplicationIP)
	{
		
		m_pStd->SetComment("BP, IP, EP에 중복된 위치가 있으면 평면 구조도가 정상적으로 그려지지 않을 수 있습니다.", TRUE);
		return;
	}
	
	
	m_pStd->SetComment("", FALSE);
*/
}

void CSelectLineDlg::SetEnableWindow()
{
	UpdateData();
	BOOL bEnable = (BOOL)m_nLineImport;
	
	m_edtFilePath.EnableWindow(!bEnable);
	m_btnLineOpen.EnableWindow(!bEnable);
	m_btnLineSave.EnableWindow(TRUE);
	
	m_stcLineName.EnableWindow(bEnable);
	m_edtLineName.EnableWindow(bEnable);
	
	GetDlgItem(IDC_STATIC_STATION_STT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_STATION_STT)->EnableWindow(bEnable);
	
	m_btnAdd.EnableWindow(bEnable);
	m_btnDelete.EnableWindow(bEnable);
	m_btnInsert.EnableWindow(bEnable);
	
	m_btnEQLR.EnableWindow(bEnable);
	m_btnSymLR.EnableWindow(bEnable);
	
	for(long i=0; i<m_Grid.GetRowCount(); i++)
	{
		for(long j=0; j<m_Grid.GetColumnCount(); j++)
		{
			if(bEnable)
				m_Grid.SetItemState(i,j,m_Grid.GetItemState(i,j)& ~ GVIS_READONLY);
			else
				m_Grid.SetItemState(i,j,m_Grid.GetItemState(i,j)| GVIS_READONLY);
		}
	}
	COLORREF clr = bEnable ? RGB(0, 0, 0) : RGB(128, 128, 128);
	m_Grid.SetTextColor(clr);
	m_Grid.SetFixedTextColor(clr);
	m_Grid.SetRedraw(TRUE, TRUE);
	
}

void CSelectLineDlg::OnRadioLineimport() 
{
	SetEnableWindow();	
	if(m_strFilePath.IsEmpty())
	{
		OnButtonLineopen();
	}
}

void CSelectLineDlg::OnRadioLineinput() 
{
	SetEnableWindow();	
	SetDataSave();
}

void CSelectLineDlg::OnButtonLineopen() 
{
	if(!m_strFilePath.IsEmpty())
	{
		CString sMsg = "선형 정보는 이미 로드 되어있습니다.\r\n";		
		sMsg += "선형 파일을 다시 로드 하면 기본 자료 데이터가 맞지 않을수 있습니다.\n계속하시겠습니까?";
		if(AfxMessageBox(sMsg,MB_YESNO|MB_DEFBUTTON2|MB_ICONEXCLAMATION|MB_ICONWARNING)==IDNO)
		{
			return;
		}
	}
	
	CFileDialog  dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ALine 파일 (*.lin)|*.lin||"));
	
	if(dlg.DoModal()==IDOK)
	{
		m_strFilePath = dlg.GetPathName();
		
		//pStd->m_strLineFilePath = m_strFilePath;
		SetLineInfoFile(m_strFilePath);
		m_nLineImport = 0;
		UpdateData(FALSE);
		DrawInputDomyunView();
		SetEnableWindow();
	}
	
	//AfxGetMainWnd()->SendMessage(WM_USER+300, (LPARAM)pStd->GetLineInfoApp());
	
	SetDataSave();
	
}

void CSelectLineDlg::SetLineInfoFile(CString strFilePath)
{
	CDataManage		*pStd		=	m_pStd->GetDataManage();
	CLineInfoApp	*pLineApp	=	pStd->GetLineInfoApp();
	
	CFile file;
	if(file.Open(strFilePath, CFile::modeRead)==FALSE) return;
	
	CArchive ar(&file,CArchive::load);

	pLineApp->Serialize(ar);

	ar.Close();
	file.Close();					
	
	CString csErrorMsg(_T(""));
	if(!pLineApp->SetDataIp(csErrorMsg))
	{
		AfxMessageBox(csErrorMsg,MB_OK | MB_ICONEXCLAMATION);
	}
	SetDataInit();
}

void CSelectLineDlg::OnButtonLinesave() 
{
	AfxGetMainWnd()->SendMessage(WM_USER+16);
}

void CSelectLineDlg::SetViewItem(long nTab)
{
	switch (nTab)
	{
	case 0:	// 평면선형
		GetDlgItem(IDC_BUTTON_EQLR)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SYMLR)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_JONGDAN)->ShowWindow(FALSE);
		break;
	case 1: // 종단계획고
		GetDlgItem(IDC_BUTTON_EQLR)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SYMLR)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_JONGDAN)->ShowWindow(FALSE);

		break;
	case 2: // 종단지반고
		GetDlgItem(IDC_BUTTON_EQLR)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SYMLR)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_JONGDAN)->ShowWindow(TRUE);
		break;
	case 3: // 좌측 편경사
		GetDlgItem(IDC_BUTTON_EQLR)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EQLR)->SetWindowText("우측과 동일");
		GetDlgItem(IDC_BUTTON_SYMLR)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SYMLR)->SetWindowText("우측과 대칭");
		GetDlgItem(IDC_CHECK_JONGDAN)->ShowWindow(FALSE);
		break;
	case 4: // 우측 편경사
		GetDlgItem(IDC_BUTTON_EQLR)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EQLR)->SetWindowText("좌측과 동일");
		GetDlgItem(IDC_BUTTON_SYMLR)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SYMLR)->SetWindowText("좌측과 대칭");
		GetDlgItem(IDC_CHECK_JONGDAN)->ShowWindow(FALSE);
		break;
	}
}

void CSelectLineDlg::SetLineName(long nCur)
{
	CDataManage		*pStd		= m_pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pStd->GetLineInfoApp();

	m_ctlLineSelect.ResetContent();
	for(long nLine=0; nLine<pLineApp->GetLinesu(); nLine++)
	{
		m_ctlLineSelect.AddString(pLineApp->GetLine(nLine)->GetNameLineEach());
	}
	m_ctlLineSelect.SetCurSel(nCur);
	m_strLineName = GetCurLineInfo()->GetNameLineEach();
}

CLineInfo* CSelectLineDlg::GetCurLineInfo()
{
	CDataManage		*pStd		= m_pStd->GetDataManage();
	CLineInfoApp	*pLineApp	= pStd->GetLineInfoApp();

	return pLineApp->GetLine(m_ctlLineSelect.GetCurSel());
}

void CSelectLineDlg::OnComboBridgeType()
{
	UpdateData(TRUE);

	CDataManage*	pManage	= m_pStd->GetDataManage();
	CGlobarOption	*pOpt	= pManage->GetGlobalOption();
	CPlateBridgeApp *pDB	= pManage->GetBridge();
	
	long nType = m_ctlBridgeType.GetCurSel();
	pOpt->SetDesignBridgeType(nType);

	if (nType==0)
	{
		pDB->m_bTendonInputComplete = FALSE;
		pDB->m_dHeightLeftTension  = 0;
		pDB->m_dHeightRightTension = 0;
		pDB->m_dLengthGinjangdan   = 0;
		pDB->m_dLengthJungchakdan  = 0;
		pDB->m_nSlabTensionSize    = 0;
		pDB->m_nTendonFix          = 0;
		for(long n=0; n<pDB->m_nSlabTensionSize; n++)
		{
			pDB->m_SlabTension[n].m_dL		= 0;
			pDB->m_SlabTension[n].m_dH		= 0;
			pDB->m_SlabTension[n].m_dR		= 0;
		}
	}
	else
	{
		pDB->m_bTendonInputComplete = TRUE;
		pDB->m_dHeightLeftTension	= pDB->m_dThickSlabLeft/2;
		pDB->m_dHeightRightTension	= pDB->m_dThickSlabRight/2;
		pDB->m_dLengthGinjangdan	= 200;
		pDB->m_dLengthJungchakdan	= 200;
		pDB->m_nSlabTensionSize		= 1+(pDB->GetGirdersu()-1)*2;
		pDB->m_nTendonFix			= 0;	// 슬래브좌측
		pDB->m_nTendonDomFix		= 0;	// 슬래브좌측
		pDB->m_dLengthSecMove		= pDB->GetLengthHDanDom(NULL, 0);
		pDB->m_dLengthDuctOffset	= 600;
		pDB->m_dLengthSecMoveEnd	= 110;
		
		for(long n=0; n<pDB->m_nSlabTensionSize; n++)
		{
			if(n%2==0) 
			{
				pDB->m_SlabTension[n].m_dH = 120;
				pDB->m_SlabTension[n].m_dR = 10000;
			}
			else
			{
				pDB->m_SlabTension[n].m_dH = pDB->m_dThickSlabBase - 100 - pDB->m_dThickSlabHunch;
				pDB->m_SlabTension[n].m_dR = 15000;
			}
		}
		
		CPlateBasicIndex *pBx		= pDB->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
		CPlateBasicIndex *pBxMatch	= pBx->GetBxMatchByCrossBeam(pDB->GetGirder(0));
		long nGsu = pDB->GetGirdersu();
		double Len = 0;
		pDB->m_SlabTension[0].m_dL = (nGsu<1)? 0 : pDB->GetWidthSlabLeft();
		
		long nK = 1;
		
		for(long nG=0; nG < nGsu-1; nG++) 
		{
			pBxMatch = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(nG));		
			Len      = pDB->GetGirder(nG)->GetDisGirderToGirder(pBxMatch);
			pDB->m_SlabTension[nK++].m_dL = Len/2;
			pDB->m_SlabTension[nK++].m_dL = Len/2;
		}	
	}

	// 0 판형교, 1 소수주형, 2 TU거더
	long nBridgeType = pOpt->GetDesignBridgeType();
	
	CString szTemp = "";
	long nGHType = 1;	//소수주거더교 기본 타입 두번째꺼
	if (nBridgeType==PLATE_GIR) nGHType = 0;
	pOpt->SetDesignHeigtBase(nGHType);
	
	if(nBridgeType==2)	//TU
	{
		// 구속콘크리트
		pDB->m_BindConc.m_dWidth	= 2500;
		pDB->m_BindConc.m_dHeight	= 200;
		pDB->m_BindConc.m_dDeep		= 36;
		pDB->m_BindConc.m_dDis		= 100;
		
		// 지간구성
		pDB->m_dLengthSlabGirderStt = 150;
		pDB->m_dLengthSlabGirderEnd = 150;
		pDB->m_dLengthTUBindConc_Stt= 100;
		pDB->m_dLengthTUBindConc_End= 100;
		
		pOpt->SetDesignMaxLengthOfFieldSplice(20000);
	}
	else
	{
		// 구속콘크리트
		pDB->m_BindConc.m_dWidth	= 0;
		pDB->m_BindConc.m_dHeight	= 0;
		pDB->m_BindConc.m_dDeep		= 0;
		pDB->m_BindConc.m_dDis		= 0;
		
		// 지간구성
		pDB->m_dLengthSlabGirderStt = 50;
		pDB->m_dLengthSlabGirderEnd = 50;
		pDB->m_dLengthTUBindConc_Stt= 0;
		pDB->m_dLengthTUBindConc_End= 0;
	}

	UpdateData(FALSE);
}

void CSelectLineDlg::SetAddRowLinePlane()
{
	if(m_Grid.GetRows()>=MAXIPCOUNT)
	{
		CString csMessage;
		csMessage.Format("현재 IP수가 최대 IP수(%ld개))에 해당하므로 더 이상 추가할 수 없습니다.  ", MAXIPCOUNT);
		MessageBox(csMessage, "평면 선형 입력", MB_ICONINFORMATION);
		m_Grid.SetFocus();
		return;
	}
	
	CCellID cID = m_Grid.GetFocusCell();
	
	long nRow = cID.row+1;
	m_Grid.InsertRow(_T(""), nRow);
	
	CLineInfo* pLine = GetCurLineInfo();
	
	long nIPs = pLine->GetQtyIp();
	nIPs++;
	pLine->SetQtyIp(nIPs);
	
	SetGridTitle();
	SetDataInit();
		
	m_Grid.SetFocusCell(nRow, cID.col);
}

void CSelectLineDlg::SetAddRowVertPlanEL()
{
	CCellID cID = m_Grid.GetFocusCell();	
	if(m_Grid.GetRowCount()>100)// || (cID.row==m_Grid.GetRows()-1 && m_Grid.GetRowCount()>2))
	{
		m_Grid.SetFocus();
		return;
	}
	
	long nRow = cID.row;
	m_Grid.InsertRow(_T(""), nRow);
	
	CLineInfo* pLine = GetCurLineInfo();
	
	pLine->m_VIPsu++;
	
	SetGridTitle();
	
	double dPrevX = m_Grid.GetTextMatrixDouble(nRow+1, 1);
	double dNextX = 0;
	if(cID.row==m_Grid.GetRowCount()-2)	dNextX = m_Grid.GetTextMatrixDouble(nRow-1, 1);
	else								dNextX = m_Grid.GetTextMatrixDouble(nRow+1, 1);
	m_Grid.SetTextMatrix(nRow, 1, "%.6f", (dPrevX+dNextX)/2);
	m_Grid.SetTextMatrix(nRow, 2, "%.6f", 0.0);
	m_Grid.SetTextMatrix(nRow, 3, "%.6f", 0.0);
	
	m_Grid.SetFocusCell(nRow+1, cID.col);
	
	SetDataSave();
	DrawInputDomyunView();
}

void CSelectLineDlg::SetAddRowVertGroundEL()
{
	CCellID cID = m_Grid.GetFocusCell();
	
	if(cID.row<=0)
	{
		cID.row = 1;
	}
	long nIdxCur = cID.row-1;
	
	CLineInfo* pLine = GetCurLineInfo();
	
	//	long nStas = pLine->m_VEarthLine.GetSize();
	CVEarthLine	*pVEarthLineCur	= pLine->m_VEarthLine.GetAt(nIdxCur);
	CVEarthLine	*pVEarthLine	= NULL;
	if(nIdxCur==0)	pVEarthLine = pLine->m_VEarthLine.GetAt(nIdxCur+1);
	else			pVEarthLine = pLine->m_VEarthLine.GetAt(nIdxCur-1);
	
	CVEarthLine	*pObj = new CVEarthLine();
	pObj->dSta	= (pVEarthLineCur->dSta + pVEarthLine->dSta)/2;
	pObj->dEle	= (pVEarthLineCur->dEle + pVEarthLine->dEle)/2;
	pLine->m_VEarthLine.InsertAt(nIdxCur+1, pObj);
	
	SetGridTitle();
	SetDataInit();
	
	DrawInputDomyunView();
}

void CSelectLineDlg::SetAddRowSlope()
{
	CCellID cID = m_Grid.GetFocusCell();
	if( m_Grid.GetRows()>100)// || (cID.row==m_Grid.GetRows()-1 && m_Grid.GetRowCount()>2))
	{
		m_Grid.SetFocus();
		return;
	}
	
	long nRow = cID.row;
	m_Grid.InsertRow("", nRow);
	
	CLineInfo* pLine = GetCurLineInfo();
	
	long nTab = m_ctlTab.GetCurSel();
	if(nTab==3)
	{
		long nSIPs = pLine->m_pSlopLeft.GetSize();
		nSIPs++;
		AhTPAMake(nSIPs, &pLine->m_pSlopLeft, (CSlop*)0);
	}
	
	if(nTab==4)
	{
		long nSIPs = pLine->m_pSlopRight.GetSize();
		nSIPs++;
		AhTPAMake(nSIPs, &pLine->m_pSlopRight, (CSlop*)0);
	}
	
	SetGridTitle();
	
	double dPrevX = m_Grid.GetTextMatrixDouble(nRow+1, 1);
	double dNextX = 0;
	if(cID.row==m_Grid.GetRowCount()-2)	dNextX = m_Grid.GetTextMatrixDouble(nRow-1, 1);
	else								dNextX = m_Grid.GetTextMatrixDouble(nRow+1, 1);
	m_Grid.SetTextMatrix(nRow, 1, "%.6f", (dPrevX+dNextX)/2);
	m_Grid.SetTextMatrix(nRow, 2, "%.6f", 0.0);
	m_Grid.SetTextMatrix(nRow, 3, "%.6f", 0.0);
	
	m_Grid.SetFocusCell(cID.row, cID.col);
	
	SetDataSave();
	DrawInputDomyunView();
}

void CSelectLineDlg::SetDeleteRowLinePlane()
{
	CCellID cID = m_Grid.GetFocusCell();
	long nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}
	
	CLineInfo* pLine = GetCurLineInfo();
	
	pLine->SetQtyIp(pLine->GetQtyIp() - 1);
	
	m_Grid.DeleteRow(nRow);
	
	SetGridTitleLinePlane();
	
	m_Grid.SetFocusCell(nRow, cID.col);
}

void CSelectLineDlg::SetDeleteRowVertPlanEL()
{
	CCellID cID = m_Grid.GetFocusCell();
	long nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}
	
	CLineInfo* pLine = GetCurLineInfo();
	
	pLine->m_VIPsu--;
	
	m_Grid.DeleteRow(nRow);
	
	SetGridTitleVertPlanEL();
	
	m_Grid.SetFocusCell(nRow, cID.col);
	
	SetDataSave();
	DrawInputDomyunView();
}

void CSelectLineDlg::SetDeleteRowVertGroundEL()
{
	CCellID cID = m_Grid.GetFocusCell();
	long nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}
	
	CLineInfo* pLine = GetCurLineInfo();
	
	long nStas = pLine->m_VEarthLine.GetSize();
	nStas--;
	AhTPAMake(nStas, &pLine->m_VEarthLine, (CVEarthLine*)0);
	
	m_Grid.DeleteRow(nRow);
	
	SetGridTitleVertGroundEL();
	
	m_Grid.SetFocusCell(nRow, cID.col);
	
	SetDataSave();
	DrawInputDomyunView();
}

void CSelectLineDlg::SetDeleteRowSlope()
{
	CCellID cID = m_Grid.GetFocusCell();
	long nRow = cID.row;
	if(nRow==1 || nRow==m_Grid.GetRows()-1)
	{
		AfxMessageBox(_T("첫째 행과 마지막 행은 삭제할 수 없습니다.  "));
		m_Grid.SetFocus();
		return;
	}
	
	CLineInfo* pLine = GetCurLineInfo();
	
	long nTab = m_ctlTab.GetCurSel();
	if(nTab==3)
	{
		long nSIPs = pLine->m_pSlopLeft.GetSize();
		nSIPs--;
		AhTPAMake(nSIPs, &pLine->m_pSlopLeft, (CSlop*)0);
	}
	
	if(nTab==4)
	{
		long nSIPs = pLine->m_pSlopRight.GetSize();
		nSIPs--;
		AhTPAMake(nSIPs, &pLine->m_pSlopRight, (CSlop*)0);
	}
	
	m_Grid.DeleteRow(nRow);
	
	SetGridTitleSlope();
	
	m_Grid.SetFocusCell(nRow, cID.col);
	
	SetDataSave();
	DrawInputDomyunView();
}

void CSelectLineDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetModifyInputData(TRUE);
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CSelectLineDlg::OnButtonAddRow()
{
	long nTab = m_ctlTab.GetCurSel();
	switch(nTab)
	{
	case 0:	SetAddRowLinePlane();		break;
	case 1:	SetAddRowVertPlanEL();		break;
	case 2:	SetAddRowVertGroundEL();	break;
	case 3:
	case 4:	SetAddRowSlope();			break;
	}
}

void CSelectLineDlg::OnButtonDelRow()
{
	long nTab = m_ctlTab.GetCurSel();
	switch(nTab)
	{
	case 0:	SetDeleteRowLinePlane();		break;
	case 1:	SetDeleteRowVertPlanEL();		break;
	case 2:	SetDeleteRowVertGroundEL();		break;
	case 3:
	case 4:	SetDeleteRowSlope();			break;
	}	
}

void CSelectLineDlg::OnButtonEqual()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	long nTab = m_ctlTab.GetCurSel();
	
	if(nTab==3)
	{
		AhTPACopy(&pLine->m_pSlopLeft, &pLine->m_pSlopRight, (CSlop*)0);
		SetDataInitSlope();
	}
	
	if(nTab==4)
	{
		AhTPACopy(&pLine->m_pSlopRight, &pLine->m_pSlopLeft, (CSlop*)0);
		SetDataInitSlope();
	}
	
	DrawInputDomyunView();
}

void CSelectLineDlg::OnButtonSymtri()
{
	CDataManage	*pStd	=	m_pStd->GetDataManage();
	CLineInfo	*pLine	=	pStd->GetLineInfoApp()->GetLine(0);
	
	long nTab = m_ctlTab.GetCurSel();
	
	if(nTab==3)
	{
		AhTPACopy(&pLine->m_pSlopLeft, &pLine->m_pSlopRight, (CSlop*)0);
		for(long i=0; i<pLine->GetRSlopsu(); i++)
		{
			pLine->m_pSlopLeft[i]->m_dValuePyungubae *= -1;
		}
		SetDataInitSlope();
	}
	
	if(nTab==4)
	{
		AhTPACopy(&pLine->m_pSlopRight, &pLine->m_pSlopLeft, (CSlop*)0);
		for(long i=0; i<pLine->GetLSlopsu(); i++)
		{
			pLine->m_pSlopRight[i]->m_dValuePyungubae *= -1;
		}
		SetDataInitSlope();
	}
	
	DrawInputDomyunView();
}

void CSelectLineDlg::OnCheckJonDan()
{
	UpdateData();
	DrawInputDomyunView(TRUE);
}
