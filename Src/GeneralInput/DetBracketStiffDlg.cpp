 // DetBracketStiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetBracketStiffDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CDetBracketStiffDlg dialog


CDetBracketStiffDlg::CDetBracketStiffDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetBracketStiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetBracketStiffDlg)
	//}}AFX_DATA_INIT
	m_pStd = pStd;
}

void CDetBracketStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetBracketStiffDlg)
	DDX_Control(pDX, IDC_TAB_CROSSBEAM, m_tabCRType);
	DDX_Control(pDX, IDC_COMBO_BRACKET_TYPE, m_ctrlBracketType);
	DDX_GridControl(pDX, IDC_GRID_WELD, m_WeldGrid);
	DDX_GridControl(pDX, IDC_GRID_BOLT, m_BoltGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetBracketStiffDlg, TStackedPage)
	//{{AFX_MSG_MAP(CDetBracketStiffDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_BRACKET_TYPE, OnSelchangeComboBracketType)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CROSSBEAM, OnSelchangeTabCrossbeam)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,	IDC_GRID_WELD,	OnCellChangedDataWeld)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,	IDC_GRID_BOLT,	OnCellChangedDataBolt)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetBracketStiffDlg message handlers
BOOL CDetBracketStiffDlg::OnInitDialog() 
{	
	TStackedPage::OnInitDialog();

	return TRUE;
}

void CDetBracketStiffDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd		*pOptStd	= m_pStd->m_pDataManage->GetOptionStd();
	CPlateBridgeApp		*pBridge	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);
	CPlateBasicIndex	*pBx		= NULL;
	CPlateCrossBeam		*pC			= NULL;
	CDomyun				*pDom		= m_pStd->m_pDataManage->GetDomyun();

	pDom->ClearEtt(TRUE);

	CDomyun Dom(pDom);
	CAPlateDrawDanmyun	Draw(m_pStd->GetDataManage());
	CAPlateDrawCross	DrawCross(m_pStd->GetDataManage());
	CDRect  BoundRect;

	long	nCRType			= m_tabCRType.GetCurSel();

	if(nCRType==0)		pBx = pGir->GetBxBySectionType(SECTION_A, 0);	///< 가로보 A Type
	else if(nCRType==1)	pBx = pGir->GetBxBySectionType(SECTION_P, 0);	///< 가로보 P Type
	else if(nCRType==2)	pBx = pGir->GetBxBySectionType(SECTION_C, 0);	///< 가로보 C Type

	if(pBx)
		pC = pBx->GetSection()->GetCrossBeam();

	if(pC)
	{
		Draw.DrawInputBracketStiff(&Dom, pBx);			///< 용접이음형태
		CDRect	rectWeld;
		Dom.SetCalcExtRect();
		rectWeld = Dom.GetExtRect();
		Dom.Move(0, rectWeld.Height()/2);
		*pDom << Dom;

		Draw.DrawInputBStiffWithBolt(&Dom, pBx);		///< 볼트이음형태
		CDRect	rectBolt;
		Dom.SetCalcExtRect();
		rectBolt = Dom.GetExtRect();
		Dom.Move(0, -rectBolt.Height()/2);
	}
	else
	{
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.TextOut(0.0, 0.0, "N/A");
		pOptStd->SetEnvType(&Dom, HCAD_HIDD);
		Dom.SetCalcExtRect();
		BoundRect = Dom.GetExtRect();
		BoundRect.top -= BoundRect.Height()*0.9;
		BoundRect.bottom +=BoundRect.Height() * 0.9;
		Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);
	}
	*pDom << Dom;

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();
}

///< 용접이음 Grid
void CDetBracketStiffDlg::SetGridTitleWeld()
{
	long nColCount		= 17;
	long nRowCount		=  2;
	long nBracketType	= m_ctrlBracketType.GetCurSel();

	m_WeldGrid.SetEditable(TRUE);
	m_WeldGrid.SetListMode(FALSE);
	m_WeldGrid.EnableDragAndDrop(TRUE);
	m_WeldGrid.EnableInsertRow(FALSE);
	m_WeldGrid.EnableInsertCol(FALSE);
	m_WeldGrid.EnableReturnPass(TRUE);
	m_WeldGrid.SetColumnCount(nColCount);
	m_WeldGrid.SetRowCount(nRowCount);

	m_WeldGrid.SetFixedRowCount(1);
	m_WeldGrid.SetFixedColumnCount(1);
	m_WeldGrid.SetRowHeightAll(20);
	m_WeldGrid.SetColumnWidthAll(50);

	m_WeldGrid.SetTextMatrix(0, 0, "구분");
	m_WeldGrid.SetTextMatrix(1, 0, "제원");
	m_WeldGrid.SetTextMatrix(0, 1, "T");
	m_WeldGrid.SetTextMatrix(0, 2, "S1");
	m_WeldGrid.SetTextMatrix(0, 3, "S2");
	m_WeldGrid.SetTextMatrix(0, 4, "S3");

	m_WeldGrid.SetTextMatrix(0, 5, "W");
	m_WeldGrid.SetTextMatrix(0, 6, "dA");
	m_WeldGrid.SetTextMatrix(0, 7, "dB");
	m_WeldGrid.SetTextMatrix(0, 8, "dC");
	m_WeldGrid.SetTextMatrix(0, 9, "uA");
	m_WeldGrid.SetTextMatrix(0, 10, "uB");
	m_WeldGrid.SetTextMatrix(0, 11, "uC");

	m_WeldGrid.SetTextMatrix(0, 12, "sT");
	m_WeldGrid.SetTextMatrix(0, 13, "dD");
	m_WeldGrid.SetTextMatrix(0, 14, "dE");
	m_WeldGrid.SetTextMatrix(0, 15, "uD");
	m_WeldGrid.SetTextMatrix(0, 16, "uE");
	
	if(nBracketType==1)		///< 역사다리꼴 형태
	{
		m_WeldGrid.SetColumnWidth(1, 50);
		m_WeldGrid.SetColumnWidth(2, 50);
		m_WeldGrid.SetColumnWidth(3, 0);
		m_WeldGrid.SetColumnWidth(4, 0);

		m_WeldGrid.SetColumnWidth(5,  0);
		m_WeldGrid.SetColumnWidth(6,  0);
		m_WeldGrid.SetColumnWidth(7, 50);
		m_WeldGrid.SetColumnWidth(8, 50);
		m_WeldGrid.SetColumnWidth(9,  0);
		m_WeldGrid.SetColumnWidth(10, 0);
		m_WeldGrid.SetColumnWidth(11, 0);

		m_WeldGrid.SetColumnWidth(12, 50);
		m_WeldGrid.SetColumnWidth(13, 50);
		m_WeldGrid.SetColumnWidth(14, 50);
		m_WeldGrid.SetColumnWidth(15, 50);
		m_WeldGrid.SetColumnWidth(16, 50);
	}
	else				///< 일반 형태
	{
		m_WeldGrid.SetColumnWidth(1,  50);
		m_WeldGrid.SetColumnWidth(2,  50);
		m_WeldGrid.SetColumnWidth(3,  50);
		m_WeldGrid.SetColumnWidth(4,  50);
		m_WeldGrid.SetColumnWidth(5,  50);
		m_WeldGrid.SetColumnWidth(6,  50);
		m_WeldGrid.SetColumnWidth(7,  50);
		m_WeldGrid.SetColumnWidth(8,  50);
		m_WeldGrid.SetColumnWidth(9,  50);
		m_WeldGrid.SetColumnWidth(10, 50);
		m_WeldGrid.SetColumnWidth(11, 50);
		m_WeldGrid.SetColumnWidth(12,  0);
		m_WeldGrid.SetColumnWidth(13,  0);
		m_WeldGrid.SetColumnWidth(14,  0);
		m_WeldGrid.SetColumnWidth(15,  0);
		m_WeldGrid.SetColumnWidth(16,  0);
	}

	m_WeldGrid.SetRedraw(TRUE,TRUE);
}

void CDetBracketStiffDlg::SetGridTitleBolt()
{
	long nColCount	= 20;
	long nRowCount	=  2;
	long nBracketType = m_ctrlBracketType.GetCurSel();

	m_BoltGrid.SetEditable(TRUE);
	m_BoltGrid.SetListMode(FALSE);
	m_BoltGrid.EnableDragAndDrop(TRUE);
	m_BoltGrid.EnableInsertRow(FALSE);
	m_BoltGrid.EnableInsertCol(FALSE);
	m_BoltGrid.EnableReturnPass(TRUE);
	m_BoltGrid.SetColumnCount(nColCount);
	m_BoltGrid.SetRowCount(nRowCount);

	m_BoltGrid.SetFixedRowCount(1);
	m_BoltGrid.SetFixedColumnCount(1);
	m_BoltGrid.SetRowHeightAll(20);
	m_BoltGrid.SetColumnWidthAll(50);

	m_BoltGrid.SetTextMatrix(0, 0, "구분");
	m_BoltGrid.SetTextMatrix(1, 0, "제원");
	m_BoltGrid.SetTextMatrix(0, 1, "T");
	m_BoltGrid.SetTextMatrix(0, 2, "S1");
	m_BoltGrid.SetTextMatrix(0, 3, "S2");
	m_BoltGrid.SetTextMatrix(0, 4, "S3");

	m_BoltGrid.SetTextMatrix(0, 5, "B");
	m_BoltGrid.SetTextMatrix(0, 6, "H");
	m_BoltGrid.SetTextMatrix(0, 7, "R");

	m_BoltGrid.SetTextMatrix(0, 8, "W");
	m_BoltGrid.SetTextMatrix(0, 9, "dA");
	m_BoltGrid.SetTextMatrix(0, 10, "dB");
	m_BoltGrid.SetTextMatrix(0, 11, "dC");
	m_BoltGrid.SetTextMatrix(0, 12, "uA");
	m_BoltGrid.SetTextMatrix(0, 13, "uB");
	m_BoltGrid.SetTextMatrix(0, 14, "uC");

	m_BoltGrid.SetTextMatrix(0, 15, "sT");
	m_BoltGrid.SetTextMatrix(0, 16, "dD");
	m_BoltGrid.SetTextMatrix(0, 17, "dE");
	m_BoltGrid.SetTextMatrix(0, 18, "uD");
	m_BoltGrid.SetTextMatrix(0, 19, "uE");

	if(nBracketType==1)///< 역사다리꼴 형태
	{
		m_BoltGrid.SetColumnWidth(1, 55);
		m_BoltGrid.SetColumnWidth(2, 55);
		m_BoltGrid.SetColumnWidth(3,  0);
		m_BoltGrid.SetColumnWidth(4,  0);
		m_BoltGrid.SetColumnWidth(5, 55);
		m_BoltGrid.SetColumnWidth(6, 55);
		m_BoltGrid.SetColumnWidth(7, 55);
		m_BoltGrid.SetColumnWidth(8,  0);
		m_BoltGrid.SetColumnWidth(9,  0);
		m_BoltGrid.SetColumnWidth(10, 55);
		m_BoltGrid.SetColumnWidth(11, 55);
		m_BoltGrid.SetColumnWidth(12,  0);
		m_BoltGrid.SetColumnWidth(13,  0);
		m_BoltGrid.SetColumnWidth(14,  0);
		m_BoltGrid.SetColumnWidth(15, 55);
		m_BoltGrid.SetColumnWidth(16, 55);
		m_BoltGrid.SetColumnWidth(17, 55);
		m_BoltGrid.SetColumnWidth(18, 55);
		m_BoltGrid.SetColumnWidth(19, 55);
	}
	else	///< 일반 형태
	{
		m_BoltGrid.SetColumnWidth(1, 55);
		m_BoltGrid.SetColumnWidth(2, 55);
		m_BoltGrid.SetColumnWidth(3, 55);
		m_BoltGrid.SetColumnWidth(4, 55);
		m_BoltGrid.SetColumnWidth(5, 55);
		m_BoltGrid.SetColumnWidth(6, 55);
		m_BoltGrid.SetColumnWidth(7, 55);
		m_BoltGrid.SetColumnWidth(8, 55);
		m_BoltGrid.SetColumnWidth(9, 55);
		m_BoltGrid.SetColumnWidth(10, 55);
		m_BoltGrid.SetColumnWidth(11, 55);
		m_BoltGrid.SetColumnWidth(12, 55);
		m_BoltGrid.SetColumnWidth(13, 55);
		m_BoltGrid.SetColumnWidth(14, 55);
		m_BoltGrid.SetColumnWidth(15, 0);
		m_BoltGrid.SetColumnWidth(16, 0);
		m_BoltGrid.SetColumnWidth(17, 0);
		m_BoltGrid.SetColumnWidth(18, 0);
		m_BoltGrid.SetColumnWidth(19, 0);
	}

	m_BoltGrid.SetRedraw(TRUE,TRUE);
}

void CDetBracketStiffDlg::SetGridTitle()
{
	SetGridTitleWeld();
	SetGridTitleBolt();
}
	
void CDetBracketStiffDlg::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	SetGridTitle();

	CString str;
	long nCRType	= m_tabCRType.GetCurSel();
	long nWeldType	= pBridge->m_nGussetStiff_Gen[nCRType][0];
	long nBoltType	= pBridge->m_nGussetStiff_Gen[nCRType][1];
	UpdateData(FALSE);

	///< 용접이음 형태
	if(nWeldType==1)
	{
		m_WeldGrid.SetTextMatrix(1,  1, "%g", pBridge->m_dGS_T[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  2, "%g", pBridge->m_dGS_S1[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  5, "%g", pBridge->m_dGS_W[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  6, "%g", pBridge->m_dGS_dA[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  7, "%g", pBridge->m_dGS_Trap_dB[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  8, "%g", pBridge->m_dGS_Trap_dC[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1, 12, "%g", pBridge->m_dGS_Sub_sT[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1, 13, "%g", pBridge->m_dGS_Sub_dD[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1, 14, "%g", pBridge->m_dGS_Sub_dE[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1, 15, "%g", pBridge->m_dGS_Sub_uD[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1, 16, "%g", pBridge->m_dGS_Sub_uE[nCRType][0]);
	}
	else
	{
		m_WeldGrid.SetTextMatrix(1,  1, "%g", pBridge->m_dGS_T[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  2, "%g", pBridge->m_dGS_S1[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  3, "%g", pBridge->m_dGS_S2[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  4, "%g", pBridge->m_dGS_S3[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  5, "%g", pBridge->m_dGS_W[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  6, "%g", pBridge->m_dGS_dA[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  7, "%g", pBridge->m_dGS_Gen_dB[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  8, "%g", pBridge->m_dGS_Gen_dC[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1,  9, "%g", pBridge->m_dGS_uA[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1, 10, "%g", pBridge->m_dGS_uB[nCRType][0]);
		m_WeldGrid.SetTextMatrix(1, 11, "%g", pBridge->m_dGS_uC[nCRType][0]);
	}

	///< 볼트 이음 형태
	if(nBoltType==1)
	{
		m_BoltGrid.SetTextMatrix(1,  1, "%g", pBridge->m_dGS_T[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1,  2, "%g", pBridge->m_dGS_S1[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1,  5, "%g", pBridge->m_dGS_Bolt_B[nCRType]);
		m_BoltGrid.SetTextMatrix(1,  6, "%g", pBridge->m_dGS_Bolt_H[nCRType]);
		m_BoltGrid.SetTextMatrix(1,  7, "%g", pBridge->m_dGS_Bolt_R[nCRType]);
		m_BoltGrid.SetTextMatrix(1, 10, "%g", pBridge->m_dGS_Trap_dB[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 11, "%g", pBridge->m_dGS_Trap_dC[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 15, "%g", pBridge->m_dGS_Sub_sT[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 16, "%g", pBridge->m_dGS_Sub_dD[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 17, "%g", pBridge->m_dGS_Sub_dE[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 18, "%g", pBridge->m_dGS_Sub_uD[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 19, "%g", pBridge->m_dGS_Sub_uE[nCRType][1]);	
	}
	else
	{
		m_BoltGrid.SetTextMatrix(1,  1, "%g", pBridge->m_dGS_T[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1,  2, "%g", pBridge->m_dGS_S1[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1,  3, "%g", pBridge->m_dGS_S2[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1,  4, "%g", pBridge->m_dGS_S3[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1,  5, "%g", pBridge->m_dGS_Bolt_B[nCRType]);
		m_BoltGrid.SetTextMatrix(1,  6, "%g", pBridge->m_dGS_Bolt_H[nCRType]);
		m_BoltGrid.SetTextMatrix(1,  7, "%g", pBridge->m_dGS_Bolt_R[nCRType]);
		m_BoltGrid.SetTextMatrix(1,  8, "%g", pBridge->m_dGS_W[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1,  9, "%g", pBridge->m_dGS_dA[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 10, "%g", pBridge->m_dGS_Gen_dB[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 11, "%g", pBridge->m_dGS_Gen_dC[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 12, "%g", pBridge->m_dGS_uA[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 13, "%g", pBridge->m_dGS_uB[nCRType][1]);
		m_BoltGrid.SetTextMatrix(1, 14, "%g", pBridge->m_dGS_uC[nCRType][1]);
	}
	m_WeldGrid.SetRedraw(TRUE,TRUE);
	m_BoltGrid.SetRedraw(TRUE,TRUE);
}

void CDetBracketStiffDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long nCRType		= m_tabCRType.GetCurSel();
	long nBracketType	=0;// m_ctrlBracketType.GetCurSel();

	for(nCRType = 0; nCRType < 3; nCRType++)
	{
		pBridge->m_nGussetStiff_Gen[nCRType][0] = nBracketType;
		pBridge->m_nGussetStiff_Gen[nCRType][1] = nBracketType;

		///< 용접이음 형태
		pBridge->m_dGS_T[nCRType][0] = 10;			//[Type][Weld:Bolt]
		pBridge->m_dGS_S1[nCRType][0] = 30;
		pBridge->m_dGS_Trap_dB[nCRType][0]	= 30;
		pBridge->m_dGS_Trap_dC[nCRType][0]	= 25;
		pBridge->m_dGS_Sub_sT[nCRType][0]	= 10;
		pBridge->m_dGS_Sub_dD[nCRType][0]	= 30;
		pBridge->m_dGS_Sub_dE[nCRType][0]	= 200;
		pBridge->m_dGS_Sub_uD[nCRType][0]	= 30;
		pBridge->m_dGS_Sub_uE[nCRType][0]	= 200;

		pBridge->m_dGS_S2[nCRType][0]		= 0;
		pBridge->m_dGS_S3[nCRType][0]		= 30;
		pBridge->m_dGS_W[nCRType][0]		= 120;
		pBridge->m_dGS_dA[nCRType][0]		= 90;
		pBridge->m_dGS_Gen_dB[nCRType][0]	= 30;
		pBridge->m_dGS_Gen_dC[nCRType][0]	= 30;
		pBridge->m_dGS_uA[nCRType][0]		= 90;
		pBridge->m_dGS_uB[nCRType][0]		= 30;
		pBridge->m_dGS_uC[nCRType][0]		= 30;

		///< 볼트이음 형태
		pBridge->m_dGS_T[nCRType][1]		= 10;
		pBridge->m_dGS_S1[nCRType][1]		= 30;
		pBridge->m_dGS_S2[nCRType][1]		= 30;
		pBridge->m_dGS_S3[nCRType][1]		= 30;
		pBridge->m_dGS_Bolt_B[nCRType]		= 20;
		pBridge->m_dGS_Bolt_H[nCRType]		= 20;
		pBridge->m_dGS_Bolt_R[nCRType]		= 20;
		pBridge->m_dGS_W[nCRType][1]		= 120;
		pBridge->m_dGS_dA[nCRType][1]		= 90;
		pBridge->m_dGS_Gen_dB[nCRType][1]	= 30;
		pBridge->m_dGS_Gen_dC[nCRType][1]	= 30;
		pBridge->m_dGS_Trap_dB[nCRType][1]	= 30;
		pBridge->m_dGS_Trap_dC[nCRType][1]	= 25;
		pBridge->m_dGS_uA[nCRType][1]		= 90;
		pBridge->m_dGS_uB[nCRType][1]		= 30;
		pBridge->m_dGS_uC[nCRType][1]		= 30;
		pBridge->m_dGS_Sub_sT[nCRType][1]	= 10;
		pBridge->m_dGS_Sub_dD[nCRType][1]	= 30;
		pBridge->m_dGS_Sub_dE[nCRType][1]	= 200;
		pBridge->m_dGS_Sub_uD[nCRType][1]	= 30;
		pBridge->m_dGS_Sub_uE[nCRType][1]	= 200;
	}
}

///< 용접이음 형태 볼트이음 형태 두가지 모두 저장
void CDetBracketStiffDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long nCRType		= m_tabCRType.GetCurSel();	///< 가로보 타입
	long nBracketType	= m_ctrlBracketType.GetCurSel();///< 브라켓 보강판 형태
	pBridge->m_nGussetStiff_Gen[nCRType][0] = nBracketType;
	pBridge->m_nGussetStiff_Gen[nCRType][1] = nBracketType;

	///< 용접이음 공통
	pBridge->m_dGS_T[nCRType][0]			= m_WeldGrid.GetTextMatrixDouble(1,1);	///< T
	pBridge->m_dGS_S1[nCRType][0]			= m_WeldGrid.GetTextMatrixDouble(1,2);	///< S1
	if(nBracketType==1)
	{
		///< 용접이음 역사다리꼴 형태
		pBridge->m_dGS_Trap_dB[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,7);	///< dB
		pBridge->m_dGS_Trap_dC[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,8);	///< dC
		pBridge->m_dGS_Sub_sT[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,12);	///< sT
		pBridge->m_dGS_Sub_dD[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,13);	///< dD
		pBridge->m_dGS_Sub_dE[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,14);	///< dE
		pBridge->m_dGS_Sub_uD[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,15);	///< uD
		pBridge->m_dGS_Sub_uE[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,16);	///< uE
	}
	else
	{
		///< 용접이음 일반형태 저장
		pBridge->m_dGS_S2[nCRType][0]		= m_WeldGrid.GetTextMatrixDouble(1,3);	///< S2
		pBridge->m_dGS_S3[nCRType][0]		= m_WeldGrid.GetTextMatrixDouble(1,4);	///< S3
		pBridge->m_dGS_W[nCRType][0]		= m_WeldGrid.GetTextMatrixDouble(1,5);	///< W
		pBridge->m_dGS_dA[nCRType][0]		= m_WeldGrid.GetTextMatrixDouble(1,6);	///< dA
		pBridge->m_dGS_Gen_dB[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,7);	///< dB
		pBridge->m_dGS_Gen_dC[nCRType][0]	= m_WeldGrid.GetTextMatrixDouble(1,8);	///< dC
		pBridge->m_dGS_uA[nCRType][0]		= m_WeldGrid.GetTextMatrixDouble(1,9);	///< uA
		pBridge->m_dGS_uB[nCRType][0]		= m_WeldGrid.GetTextMatrixDouble(1,10);	///< uB
		pBridge->m_dGS_uC[nCRType][0]		= m_WeldGrid.GetTextMatrixDouble(1,11);	///< uC
	}

	///< 볼트이음 공통
	pBridge->m_dGS_T[nCRType][1]			= m_BoltGrid.GetTextMatrixDouble(1,1);	///< T
	pBridge->m_dGS_S1[nCRType][1]			= m_BoltGrid.GetTextMatrixDouble(1,2);	///< S1
	pBridge->m_dGS_S2[nCRType][1]			= m_BoltGrid.GetTextMatrixDouble(1,3);	///< S2
	pBridge->m_dGS_S3[nCRType][1]			= m_BoltGrid.GetTextMatrixDouble(1,4);	///< S3
	pBridge->m_dGS_Bolt_B[nCRType]			= m_BoltGrid.GetTextMatrixDouble(1,5);	///< B
	pBridge->m_dGS_Bolt_H[nCRType]			= m_BoltGrid.GetTextMatrixDouble(1,6);	///< H
	pBridge->m_dGS_Bolt_R[nCRType]			= m_BoltGrid.GetTextMatrixDouble(1,7);	///< R
	if(nBracketType==1)
	{
		///< 볼트이음 역사다리꼴 형태 저장
		pBridge->m_dGS_Trap_dB[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,10);	///< dB
		pBridge->m_dGS_Trap_dC[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,11);	///< dC
		pBridge->m_dGS_Sub_sT[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,15);	///< sT
		pBridge->m_dGS_Sub_dD[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,16);	///< dD
		pBridge->m_dGS_Sub_dE[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,17);	///< dE
		pBridge->m_dGS_Sub_uD[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,18);	///< uD
		pBridge->m_dGS_Sub_uE[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,19);	///< uE
	}
	else
	{
		///< 볼트이음 일반형태 저장
		pBridge->m_dGS_W[nCRType][1]		= m_BoltGrid.GetTextMatrixDouble(1,8);	///< W
		pBridge->m_dGS_dA[nCRType][1]		= m_BoltGrid.GetTextMatrixDouble(1,9);	///< dA
		pBridge->m_dGS_Gen_dB[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,10);	///< dB
		pBridge->m_dGS_Gen_dC[nCRType][1]	= m_BoltGrid.GetTextMatrixDouble(1,11);	///< dC
		pBridge->m_dGS_uA[nCRType][1]		= m_BoltGrid.GetTextMatrixDouble(1,12);	///< uA
		pBridge->m_dGS_uB[nCRType][1]		= m_BoltGrid.GetTextMatrixDouble(1,13);	///< uB
		pBridge->m_dGS_uC[nCRType][1]		= m_BoltGrid.GetTextMatrixDouble(1,14);	///< uC
	}
}

///////////////////////////////////////////////////////////////////////////////

void CDetBracketStiffDlg::OnButtonRecommend()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "브라켓 보강판 부착 상세 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long nCRType		= m_tabCRType.GetCurSel();
	long nBracketType	= m_ctrlBracketType.GetCurSel();

	pBridge->m_nGussetStiff_Gen[nCRType][0] = nBracketType;
	pBridge->m_nGussetStiff_Gen[nCRType][1] = nBracketType;

	///< 용접이음 형태
	pBridge->m_dGS_T[nCRType][0] = 10;			//[Type][Weld:Bolt]
	pBridge->m_dGS_S1[nCRType][0] = 30;
	if(nBracketType==1)
	{
		pBridge->m_dGS_Trap_dB[nCRType][0]	= 30;
		pBridge->m_dGS_Trap_dC[nCRType][0]	= 25;
		pBridge->m_dGS_Sub_sT[nCRType][0]	= 10;
		pBridge->m_dGS_Sub_dD[nCRType][0]	= 30;
		pBridge->m_dGS_Sub_dE[nCRType][0]	= 200;
		pBridge->m_dGS_Sub_uD[nCRType][0]	= 30;
		pBridge->m_dGS_Sub_uE[nCRType][0]	= 200;
	}
	else
	{
		pBridge->m_dGS_S2[nCRType][0]		= 0;
		pBridge->m_dGS_S3[nCRType][0]		= 30;
		pBridge->m_dGS_W[nCRType][0]		= 120;
		pBridge->m_dGS_dA[nCRType][0]		= 90;
		pBridge->m_dGS_Gen_dB[nCRType][0]	= 30;
		pBridge->m_dGS_Gen_dC[nCRType][0]	= 30;
		pBridge->m_dGS_uA[nCRType][0]		= 90;
		pBridge->m_dGS_uB[nCRType][0]		= 30;
		pBridge->m_dGS_uC[nCRType][0]		= 30;
	}

	///< 볼트이음 형태
	if(nBracketType==1)
	{
		pBridge->m_dGS_T[nCRType][1]		= 10;
		pBridge->m_dGS_S1[nCRType][1]		= 30;
		pBridge->m_dGS_Bolt_B[nCRType]		= 20;
		pBridge->m_dGS_Bolt_H[nCRType]		= 20;
		pBridge->m_dGS_Bolt_R[nCRType]		= 20;
		pBridge->m_dGS_Trap_dB[nCRType][1]	= 30;
		pBridge->m_dGS_Trap_dC[nCRType][1]	= 25;
		pBridge->m_dGS_uA[nCRType][1]		= 90;
		pBridge->m_dGS_uB[nCRType][1]		= 30;
		pBridge->m_dGS_uC[nCRType][1]		= 30;
		pBridge->m_dGS_Sub_sT[nCRType][1]	= 10;
		pBridge->m_dGS_Sub_dD[nCRType][1]	= 30;
		pBridge->m_dGS_Sub_dE[nCRType][1]	= 200;
		pBridge->m_dGS_Sub_uD[nCRType][1]	= 30;
		pBridge->m_dGS_Sub_uE[nCRType][1]	= 200;
	}
	else
	{
		pBridge->m_dGS_T[nCRType][1]		= 10;
		pBridge->m_dGS_S1[nCRType][1]		= 30;
		pBridge->m_dGS_S2[nCRType][1]		= 30;
		pBridge->m_dGS_S3[nCRType][1]		= 30;
		pBridge->m_dGS_Bolt_B[nCRType]		= 20;
		pBridge->m_dGS_Bolt_H[nCRType]		= 20;
		pBridge->m_dGS_Bolt_R[nCRType]		= 20;
		pBridge->m_dGS_W[nCRType][1]		= 120;
		pBridge->m_dGS_dA[nCRType][1]		= 90;
		pBridge->m_dGS_Gen_dB[nCRType][1]	= 30;
		pBridge->m_dGS_Gen_dC[nCRType][1]	= 30;
		pBridge->m_dGS_uA[nCRType][1]		= 90;
		pBridge->m_dGS_uB[nCRType][1]		= 30;
		pBridge->m_dGS_uC[nCRType][1]		= 30;
	}

	SetDataInit();
	DrawInputDomyunView();
}

///////////////////////////////////////////////////////////////////////////////
void CDetBracketStiffDlg::OnCellChangedDataWeld(NMHDR* nmgv, LRESULT*)
{
//	TStackedPage::OnCellChangedData(nmgv, 0);

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetBracketStiffDlg::OnCellChangedDataBolt(NMHDR* nmgv, LRESULT*)
{
//	TStackedPage::OnCellChangedData(nmgv, 0);

	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetBracketStiffDlg::OnCursorChange(long nRow, long nCol)
{
//	TStackedPage::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_WeldGrid.GetItemText(0,nCol));
}

///////////////////////////////////////////////////////////////////////////////

void CDetBracketStiffDlg::OnPreInitDialog()
{
 	SetResize(IDC_TAB_CROSSBEAM,		SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_GRID_WELD,			SZ_TOP_LEFT, SZ_TOP_RIGHT);
 	SetResize(IDC_GRID_BOLT,			SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_STATIC_BRACKET_TEXT,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_COMBO_BRACKET_TYPE,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_STATIC_WELD,			SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_STATIC_BOLT,			SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetInitTypeCombo();
}

void CDetBracketStiffDlg::SetInitTypeCombo()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	m_tabCRType.DeleteAllItems();
	m_tabCRType.InsertItem(0, _T("A Type"));
	m_tabCRType.InsertItem(1, _T("P Type"));
	m_tabCRType.InsertItem(2, _T("C Type"));

	m_ctrlBracketType.AddString(_T("일반형태"));
	m_ctrlBracketType.AddString(_T("역사다리꼴 형태"));

	m_tabCRType.SetCurSel(0);
	m_ctrlBracketType.SetCurSel(pBridge->m_nGussetStiff_Gen[0][0]);
}

///< 브라켓 보강판 형태 combo
void CDetBracketStiffDlg::OnSelchangeComboBracketType() 
{
	CPlateBridgeApp		*pBridge	= m_pStd->GetBridge();
	UpdateData(TRUE);

	long nCRCombo		= m_tabCRType.GetCurSel();
	long nBracketCombo	= m_ctrlBracketType.GetCurSel();
	pBridge->m_nGussetStiff_Gen[nCRCombo][0] = nBracketCombo;
	pBridge->m_nGussetStiff_Gen[nCRCombo][1] = nBracketCombo;
	SetGridTitle();
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

void CDetBracketStiffDlg::OnSelchangeTabCrossbeam(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPlateBridgeApp		*pBridge	= m_pStd->GetBridge();
	UpdateData(TRUE);
	long nCRTab = m_tabCRType.GetCurSel();
	m_ctrlBracketType.SetCurSel(pBridge->m_nGussetStiff_Gen[nCRTab][0]);

	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}
