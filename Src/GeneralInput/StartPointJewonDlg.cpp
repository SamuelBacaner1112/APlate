// StartPointJewonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "StartPointJewonDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartPointJewonDlg dialog


CStartPointJewonDlg::CStartPointJewonDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CStartPointJewonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStartPointJewonDlg)
	m_nTaperType = 0;
	m_nStiffType = 1;
	m_bManhole = FALSE;
	m_bStiffPlate = FALSE;
	m_bDuplicate = TRUE;
	//}}AFX_DATA_INIT
	m_nCurPos = 0;
}


void CStartPointJewonDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartPointJewonDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Radio(pDX,	 IDC_RADIO_TAPER,		m_nTaperType);
	DDX_Radio(pDX,	 IDC_RADIO_STIFF,		m_nStiffType);
	DDX_Check(pDX,	 IDC_CHECK_MANHOLE,		m_bManhole);
	DDX_Check(pDX,	 IDC_CHECK_STIFFPLATE,	m_bStiffPlate);
	DDX_Check(pDX, IDC_DUPLICATE, m_bDuplicate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartPointJewonDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CStartPointJewonDlg)
	ON_BN_CLICKED(IDC_CHECK_MANHOLE, OnCheckManhole)
	ON_BN_CLICKED(IDC_CHECK_STIFFPLATE, OnCheckStiffplate)
	ON_BN_CLICKED(IDC_RADIO_TAPER, OnRadioTaper1)
	ON_BN_CLICKED(IDC_RADIO_TAPER2, OnRadioTaper2)	
	ON_BN_CLICKED(IDC_RADIO_STIFF, OnRadioStiff)
	ON_BN_CLICKED(IDC_RADIO_STIFF2, OnRadioStiff2)
	ON_BN_CLICKED(IDC_DUPLICATE, OnDuplicate)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartPointJewonDlg message handlers

void CStartPointJewonDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	CDomyun Dom(pDom);
	CAPlateDrawJong Draw(m_pStd->GetDataManage());
	BOOL bStt = m_nCurPos==0? TRUE : FALSE;
	Draw.DrawJongSttEnd(&Dom, bStt);
	Draw.DimJongSttEnd(&Dom, bStt);

	*pDom << Dom;
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CStartPointJewonDlg::SetGridTitle()
{
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass();
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);

	m_Grid.SetRowCount(3);
	m_Grid.SetColumnCount(8);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(50);	
	m_Grid.SetColumnWidth(0,90);

	m_Grid.SetTextMatrix(0,0,"구 분");	
	m_Grid.SetTextMatrix(1,0,"시점부(mm)");	
	m_Grid.SetTextMatrix(2,0,"종점부(mm)");	
	m_Grid.SetTextMatrix(0,1,"A");
	m_Grid.SetTextMatrix(0,2,"B");
	m_Grid.SetTextMatrix(0,3,"C");
	m_Grid.SetTextMatrix(0,4,"D");
	m_Grid.SetTextMatrix(0,5,"E");
	m_Grid.SetTextMatrix(0,6,"G");
	m_Grid.SetTextMatrix(0,7,"R");
}

void CStartPointJewonDlg::SetDataInit()
{
	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	

	SetGridTitle();
	ResetGrid();

	if(m_nCurPos==0) m_nTaperType = pDB->m_cTaperAngleTypeStt;
	else			 m_nTaperType = pDB->m_cTaperAngleTypeEnd;

	m_bManhole			= pDB->m_cFaceManhole;
	m_bStiffPlate		= pDB->m_cFacePlateUse;	
	m_nStiffType		= pDB->m_cFacePlateRoundType;

	GetDlgItem(IDC_STATIC_PIC_STTEND1)->ShowWindow(m_nTaperType==0?SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_STATIC_PIC_STTEND2)->ShowWindow(m_nTaperType==0?SW_HIDE:SW_SHOW);

	for(long i=0;i<2;i++)
	{
		m_Grid.SetTextMatrix(i+1,1,"%g",i==0 ? pDB->m_dLengthShoeToTaperStt : pDB->m_dLengthShoeToTaperEnd);
		m_Grid.SetTextMatrix(i+1,2,"%g",i==0 ? pDB->m_dLengthTaperToTaperStt : pDB->m_dLengthTaperToTaperEnd);
		m_Grid.SetTextMatrix(i+1,3,"%g",i==0 ? pDB->m_dHeightTaperStt : pDB->m_dHeightTaperEnd);
		m_Grid.SetTextMatrix(i+1,4,"%g",i==0 ? pDB->m_dHeightInOutLowerStt : pDB->m_dHeightInOutLowerEnd);
		m_Grid.SetTextMatrix(i+1,5,"%g",i==0 ? pDB->m_dHeightInOutStt : pDB->m_dHeightInOutEnd);
		m_Grid.SetTextMatrix(i+1,6,"%g",i==0 ? pDB->m_dWidthInOutStt : pDB->m_dWidthInOutEnd);
		m_Grid.SetTextMatrix(i+1,7,"%g",i==0 ? pDB->m_dRadiusInOutStt : pDB->m_dRadiusInOutEnd);
	}

	UpdateData(FALSE);
	SetDlgItemEnable();
}

void CStartPointJewonDlg::SetDataDefault()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	

	m_nTaperType		= 0;
	m_bManhole			= FALSE;
	m_bStiffPlate		= FALSE;
	m_nStiffType		= 1;

	UpdateData(FALSE);

	pDB->m_cTaperAngleTypeStt = 0;
	pDB->m_cTaperAngleTypeEnd = 0;
	double dCTU = pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDeep;
	
	pDB->m_dLengthShoeToTaperStt	= pDB->m_dLengthShoeToTaperEnd	= pDB->IsTUGir()? 750 : 1500;
	pDB->m_dLengthTaperToTaperStt	= pDB->m_dLengthTaperToTaperEnd = pDB->IsTUGir()? dCTU*5 : 1000;
	pDB->m_dHeightTaperStt			= pDB->m_dHeightTaperEnd		= pDB->IsTUGir()? dCTU : 200; 
	pDB->m_dHeightInOutLowerStt		= pDB->m_dHeightInOutLowerEnd	= 0;
	pDB->m_dHeightInOutStt			= pDB->m_dHeightInOutEnd		= 0;
	pDB->m_dWidthInOutStt			= pDB->m_dWidthInOutEnd			= 0;
	pDB->m_dRadiusInOutStt			= pDB->m_dRadiusInOutEnd		= 0;	
	pDB->m_cFaceManhole				= m_bManhole;
	pDB->m_cFacePlateUse			= m_bStiffPlate;
	pDB->m_cFacePlateRoundType		= m_nStiffType;

}

void CStartPointJewonDlg::SetDataSave()
{
	CPlateBridge *pDB = m_pStd->GetBridge();	

	UpdateData();

	pDB->m_cFaceManhole			= m_bManhole;
	pDB->m_cFacePlateUse		= m_bStiffPlate;
	pDB->m_cFacePlateRoundType	= m_nStiffType;
	
	if(m_nCurPos==0) 
	{
		pDB->m_cTaperAngleTypeStt	= m_nTaperType;
		pDB->m_dLengthShoeToTaperStt = m_Grid.GetTextMatrixDouble(1,1);
		pDB->m_dLengthTaperToTaperStt = m_Grid.GetTextMatrixDouble(1,2);
		pDB->m_dHeightTaperStt = m_Grid.GetTextMatrixDouble(1,3);
		pDB->m_dHeightInOutLowerStt = m_Grid.GetTextMatrixDouble(1,4);
		pDB->m_dHeightInOutStt = m_Grid.GetTextMatrixDouble(1,5);
		pDB->m_dWidthInOutStt = m_Grid.GetTextMatrixDouble(1,6);
		pDB->m_dRadiusInOutStt = m_Grid.GetTextMatrixDouble(1,7);
	}
	else
	{
		pDB->m_cTaperAngleTypeEnd	= m_nTaperType;
		pDB->m_dLengthShoeToTaperEnd = m_Grid.GetTextMatrixDouble(2,1);
		pDB->m_dLengthTaperToTaperEnd = m_Grid.GetTextMatrixDouble(2,2);
		pDB->m_dHeightTaperEnd = m_Grid.GetTextMatrixDouble(2,3);
		pDB->m_dHeightInOutLowerEnd = m_Grid.GetTextMatrixDouble(2,4);
		pDB->m_dHeightInOutEnd = m_Grid.GetTextMatrixDouble(2,5);
		pDB->m_dWidthInOutEnd = m_Grid.GetTextMatrixDouble(2,6);
		pDB->m_dRadiusInOutEnd = m_Grid.GetTextMatrixDouble(2,7);
	}	
}

void CStartPointJewonDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);
	
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CStartPointJewonDlg::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	CPlateBridge *pDB = m_pStd->GetBridge();	
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nRow = GridView->iRow;
	
	SetDataSave();

	m_nCurPos = nRow - m_Grid.GetFixedRowCount();	
	
	if(m_nCurPos==0)	m_nTaperType = pDB->m_cTaperAngleTypeStt;
	else				m_nTaperType = pDB->m_cTaperAngleTypeEnd;
	UpdateData(FALSE);

	DrawInputDomyunView(TRUE);
}

void CStartPointJewonDlg::OnCheckManhole() 
{
	UpdateData(TRUE);

	CPlateBridge *pDB = m_pStd->GetBridge();	
	pDB->m_cFaceManhole = m_bManhole;

	if(m_bManhole)
	{
		pDB->m_dHeightInOutLowerStt = pDB->m_dHeightInOutLowerEnd	= 300;
		pDB->m_dHeightInOutStt		= pDB->m_dHeightInOutEnd		= 800;
		pDB->m_dWidthInOutStt		= pDB->m_dWidthInOutEnd			= 150;
		pDB->m_dRadiusInOutStt		= pDB->m_dRadiusInOutEnd		= 50;				
	}
	else
	{
		pDB->m_cFacePlateUse		= 0;
		pDB->m_dHeightInOutLowerStt = pDB->m_dHeightInOutLowerEnd	= 0;
		pDB->m_dHeightInOutStt		= pDB->m_dHeightInOutEnd		= 0;
		pDB->m_dWidthInOutStt		= pDB->m_dWidthInOutEnd			= 0;
		pDB->m_dRadiusInOutStt		= pDB->m_dRadiusInOutEnd		= 0;
	}

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(TRUE);
}
void CStartPointJewonDlg::OnCheckStiffplate() 
{
	SetDataSave();
	SetDlgItemEnable();
	DrawInputDomyunView(TRUE);
}

void CStartPointJewonDlg::OnRadioTaper1() 
{
	GetDlgItem(IDC_STATIC_PIC_STTEND1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PIC_STTEND2)->ShowWindow(SW_HIDE);	
}

void CStartPointJewonDlg::OnRadioTaper2() 
{
	GetDlgItem(IDC_STATIC_PIC_STTEND1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PIC_STTEND2)->ShowWindow(SW_SHOW);	
}

//void CStartPointJewonDlg::OnCheckStiffplate() 
//{
//	UpdateData(TRUE);
//
//	CPlateBridge *pDB = m_pStd->GetBridge();	
//	pDB->m_cFacePlateUse = m_bStiffPlate;
//
//	if(m_bStiffPlate)
//	{
//		pDB->m_dHeightInOutStt = pDB->m_dHeightInOutEnd = 1000;
//		
//		CPlateGirderApp *pGir = pDB->GetGirder(0);
//		double dHGir = pGir->GetHeightGirderByJijum(0, FALSE);
//		double dTemp = (dHGir-pDB->m_dHeightInOutStt)/2;
//		pDB->m_dHeightInOutLowerStt = Round(dTemp, -1);
//
//		dHGir = pGir->GetHeightGirderByJijum(pDB->m_nQtyJigan, FALSE);
//		dTemp = (dHGir-pDB->m_dHeightInOutEnd)/2;
//		pDB->m_dHeightInOutLowerEnd = Round(dTemp, -1);
//
//		pDB->m_dWidthInOutStt = pDB->m_dWidthInOutEnd = 200;
//		pDB->m_dRadiusInOutStt = pDB->m_dRadiusInOutEnd = 100;	
//	}
//	else
//	{
//		pDB->m_dHeightInOutLowerStt = pDB->m_dHeightInOutLowerEnd = 0;
//		pDB->m_dHeightInOutStt = pDB->m_dHeightInOutEnd = 0;
//		pDB->m_dWidthInOutStt = pDB->m_dWidthInOutEnd = 0;
//		pDB->m_dRadiusInOutStt = pDB->m_dRadiusInOutEnd = 0;	
//	}
//	SetDataInit();
//	SetDataSave();
//	DrawInputDomyunView(TRUE);
//}

// 경사부 계산(1:5)
void CStartPointJewonDlg::TaperCalc(long col)
{
	double b = atof(m_Grid.GetItemText(1,2));
	CString str,str1;
	str.Format("%g",b/5);
	str1 = m_Grid.GetItemText(1,3);
	if(col == 2)
		m_Grid.SetTextMatrix(1,3,str);
	else if(col ==3&& str != str1)
	{
		MessageBox("거더 경사면과 높이의 비(5:1)가 맞지 않습니다.\n다시 확인해 주십시오",
				"시점부 제원",MB_ICONEXCLAMATION );
		double c= m_Grid.GetTextMatrixDouble(1,3);
		str.Format("%.lf",b = c*5);
		m_Grid.SetTextMatrix(1,2,str);
	}
	
		
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CStartPointJewonDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW  *GridView = (NM_GRIDVIEW *)nmgv;
	CPlateBridge *pDB	   =  m_pStd->GetBridge();
	
	int nCol = GridView->iColumn;	
	
	if(nCol == 2) TaperCalc(nCol);
	else if(nCol == 3) TaperCalc(nCol);
	SetDataSave();
	if(m_bDuplicate)
	{
		pDB->m_dLengthShoeToTaperEnd  = pDB->m_dLengthShoeToTaperStt;
		pDB->m_dLengthTaperToTaperEnd = pDB->m_dLengthTaperToTaperStt;
		pDB->m_dHeightTaperEnd		  = pDB->m_dHeightTaperStt;
		pDB->m_dHeightInOutLowerEnd   = pDB->m_dHeightInOutLowerStt;
		pDB->m_dHeightInOutEnd		  = pDB->m_dHeightInOutStt;
		pDB->m_dWidthInOutEnd		  = pDB->m_dWidthInOutStt;
		pDB->m_dRadiusInOutEnd		  = pDB->m_dRadiusInOutStt;
		SetDataInit();
	}

	DrawInputDomyunView();
}

void CStartPointJewonDlg::OnRadioStiff() 
{
	SetDataSave();
	DrawInputDomyunView();	
}

void CStartPointJewonDlg::OnRadioStiff2() 
{
	SetDataSave();
	DrawInputDomyunView();	
}

void CStartPointJewonDlg::SetDlgItemEnable()
{
	UpdateData();
	if(m_bManhole)
	{
		GetDlgItem(IDC_CHECK_STIFFPLATE)->EnableWindow(TRUE);
		if(m_bStiffPlate)
		{
			GetDlgItem(IDC_STATIC_BOX1)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_BOX2)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_STIFF)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_STIFF2)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_BOX1)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_BOX2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_STIFF)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_STIFF2)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_CHECK_STIFFPLATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_BOX1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_BOX2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STIFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STIFF2)->EnableWindow(FALSE);
	}
}

void CStartPointJewonDlg::OnDuplicate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ResetGrid();
	CPlateBridge* pDB =  m_pStd->GetBridge();
	if(m_bDuplicate)
	{
		
		pDB->m_dLengthShoeToTaperEnd  = pDB->m_dLengthShoeToTaperStt;
		pDB->m_dLengthTaperToTaperEnd = pDB->m_dLengthTaperToTaperStt;
		pDB->m_dHeightTaperEnd		  = pDB->m_dHeightTaperStt;
		pDB->m_dHeightInOutLowerEnd   = pDB->m_dHeightInOutLowerStt;
		pDB->m_dHeightInOutEnd		  = pDB->m_dHeightInOutStt;
		pDB->m_dWidthInOutEnd		  = pDB->m_dWidthInOutStt;
		pDB->m_dRadiusInOutEnd		  = pDB->m_dRadiusInOutStt;
		SetDataInit();
	}	
	
}
void CStartPointJewonDlg::ResetGrid()
{
	UpdateData();
	if(m_bDuplicate)
	{	
		for(long n=1;n<m_Grid.GetColumnCount();n++)
		{
			m_Grid.SetItemState(2,n,GVIS_READONLY);
		}
	}
	else
	{
		for(long n=1;n<m_Grid.GetColumnCount();n++)
		{
			m_Grid.SetItemState(2,n,GVIS_MODIFIED);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

BOOL CStartPointJewonDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	SetResize(IDC_CHECK_MANHOLE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_STIFFPLATE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_BOX1,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_STIFF,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_STIFF2,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_BOX2,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_DUPLICATE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetResize(IDC_STATIC_PIC_STTEND1, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_PIC_STTEND2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_GROUPBOX, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_TAPER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_TAPER2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;
}
