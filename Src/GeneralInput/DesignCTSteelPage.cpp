// DesignCTSteelPage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DesignCTSteelPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignCTSteelPage dialog

CDesignCTSteelPage::CDesignCTSteelPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignCTSteelPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignCTSteelPage)
	m_nIndex	= 0;
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_Domyun.m_bEnablePopupMenu = TRUE;
	m_Domyun.GetDomyun()->SetCWnd(&m_Domyun);
}


void CDesignCTSteelPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignCTSteelPage)
	DDX_GridControl(pDX, IDC_GRID_CTSTEEL, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_CTSTEEL_PROPERTY, m_Grid_Property);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_CTSTEEL_DOMYUN, m_Domyun);
}


BEGIN_MESSAGE_MAP(CDesignCTSteelPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignCTSteelPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW		, IDC_GRID_CTSTEEL, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignCTSteelPage message handlers
void CDesignCTSteelPage::OnPreInitDialog()
{
	SetResize(IDC_GRID_CTSTEEL,				SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_CTSTEEL_PROPERTY,	SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_CTSTEEL_DOMYUN,			SZ_TOP_RIGHT, SZ_TOP_RIGHT);
}

void CDesignCTSteelPage::SetDataDefault()
{	
}

void CDesignCTSteelPage::SetDataInit()
{
	SetGridTitle();
	SetCTSteelProperty();
	DrawCTSteel();

	CSteelMaterial *pSteelMat = m_pDataManage->GetSteelMaterial();

	for(long n=0; n<pSteelMat->m_MatDataArray[CSteelMaterialData::CTSTEEL].GetSize(); n++)
	{
		CSteelMaterialData *pMatData = pSteelMat->m_MatDataArray[CSteelMaterialData::CTSTEEL].GetAt(n);		
		long nCol=0;
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dH);		
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dB);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dT1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dT2);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dR1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dSectionArea);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dUnitWeight);
/*
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dCenterOfGravity.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dCenterOfGravity.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_d2MomentOfInertia.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_d2MomentOfInertia.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dRadiusOfGyration.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dRadiusOfGyration.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dModulusOfSection.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dModulusOfSection.y);
*/
	}
}

void CDesignCTSteelPage::SetGridTitle()
{
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(2);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(m_pDataManage->GetSteelMaterial()->m_MatDataArray[CSteelMaterialData::CTSTEEL].GetSize()+2);
//	m_Grid.SetColumnCount(15);
	m_Grid.SetColumnCount(7);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(50);			

	m_Grid.SetTextMatrix(0, 0, "치 수 (mm)");
	m_Grid.SetMergeCol(0,0,4);
	m_Grid.SetTextMatrix(1, 0, "H");
	m_Grid.SetTextMatrix(1, 1, "B");
	m_Grid.SetTextMatrix(1, 2, "T1");
	m_Grid.SetTextMatrix(1, 3, "T2");
	m_Grid.SetTextMatrix(1, 4, "R");

	m_Grid.SetTextMatrix(0, 5, "단면적(cm2)");
	m_Grid.SetMergeRow(0,1,5);
	m_Grid.SetColumnWidth(5,100);		

	m_Grid.SetTextMatrix(0, 6, "단위중량(kg/m)");
	m_Grid.SetMergeRow(0,1,6);
	m_Grid.SetColumnWidth(6,100);		
/*
	m_Grid.SetTextMatrix(0, 7, "도심위치(cm)");
	m_Grid.SetMergeCol(0,7,8);
	m_Grid.SetTextMatrix(1, 7, "Cx");	
	m_Grid.SetTextMatrix(1, 8, "Cy");	
	
	m_Grid.SetTextMatrix(0, 9, "2차 모멘트 (cm4)");
	m_Grid.SetMergeCol(0,9,10);
	m_Grid.SetTextMatrix(1, 9, "Jx");
	m_Grid.SetTextMatrix(1, 10, "Jy");
	
	m_Grid.SetTextMatrix(0, 11, "회전반경(cm)");
	m_Grid.SetMergeCol(0,11,12);
	m_Grid.SetTextMatrix(1, 11, "Ix");
	m_Grid.SetTextMatrix(1, 12, "Iy");
	
	m_Grid.SetTextMatrix(0, 13, "단면계수(cm3)");
	m_Grid.SetMergeCol(0,13,14);
	m_Grid.SetTextMatrix(1, 13, "Zx");
	m_Grid.SetTextMatrix(1, 14, "Zy");
*/
	m_Grid.SetRedraw(TRUE,TRUE);
//////////////////////////////////////////////////////////////////////
	COLORREF Bakckcolor = 16777215;
	m_Grid_Property.SetEditable(FALSE);
	m_Grid_Property.SetListMode(FALSE);
	m_Grid_Property.EnableDragAndDrop(TRUE);
	m_Grid_Property.EnableInsertRow(FALSE);
	m_Grid_Property.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid_Property.SetFixedRowCount(0);
	m_Grid_Property.EnableReturnPass(TRUE);

	m_Grid_Property.SetRowCount(8);
	m_Grid_Property.SetColumnCount(2);

	m_Grid_Property.SetRowHeightAll(20);
	m_Grid_Property.SetRowHeight(0, 20);
	m_Grid_Property.SetColumnWidthAll(145);

	for (long nRow = 0; nRow < m_Grid_Property.GetRowCount(); nRow++)
	{
		for (long nCol = 0; nCol < m_Grid_Property.GetColumnCount(); nCol++)
		{
			COLORREF	color = 0xfffaf0;
			m_Grid_Property.SetItemBkColour(nRow,nCol, color);
		}
	}

	m_Grid_Property.SetTextMatrix(0, 0, " · 도심위치 ( cm )", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(0, 0, 1);
	m_Grid_Property.SetItemBkColour(0,0, Bakckcolor);

	m_Grid_Property.SetTextMatrix(2, 0, " · 2차 모멘트 ( cm⁴)", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(2, 0, 1);
	m_Grid_Property.SetItemBkColour(2, 0, Bakckcolor);

	m_Grid_Property.SetTextMatrix(4, 0, " · 회전반경 ( cm )", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(4, 0, 1);
	m_Grid_Property.SetItemBkColour(4, 0, Bakckcolor);

	m_Grid_Property.SetTextMatrix(6, 0, " · 단면계수 ( cm³)", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(6, 0, 1);
	m_Grid_Property.SetItemBkColour(6, 0, Bakckcolor);
}

BOOL CDesignCTSteelPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDesignCTSteelPage::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

	int nRow = GridView->iRow;

	m_nIndex = nRow-2;

	SetCTSteelProperty();
	DrawCTSteel();
}

void CDesignCTSteelPage::SetCTSteelProperty()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::CTSTEEL].GetAt(m_nIndex);
	CString				sText;

	// 도심위치
	sText.Format("   Cx       = %.3f", pMatData->m_dCenterOfGravity.x);
	m_Grid_Property.SetTextMatrix(1, 0, sText, 0, DT_LEFT);
	sText.Format("   Cy       = %.3f", pMatData->m_dCenterOfGravity.y);
	m_Grid_Property.SetTextMatrix(1, 1, sText, 0, DT_LEFT);
	// 단면2차모멘트
	sText.Format("   Jx       = %.3f", pMatData->m_d2MomentOfInertia.x);
	m_Grid_Property.SetTextMatrix(3, 0, sText, 0, DT_LEFT);
	sText.Format("   Jy       = %.3f", pMatData->m_d2MomentOfInertia.y);
	m_Grid_Property.SetTextMatrix(3, 1, sText, 0, DT_LEFT);
	// 회전반경
	sText.Format("   lx       = %.3f", pMatData->m_dRadiusOfGyration.x);
	m_Grid_Property.SetTextMatrix(5, 0, sText, 0, DT_LEFT);
	sText.Format("   ly       = %.3f", pMatData->m_dRadiusOfGyration.y);
	m_Grid_Property.SetTextMatrix(5, 1, sText, 0, DT_LEFT);
	// 단면계수
	sText.Format("   Zx      = %.3f", pMatData->m_dModulusOfSection.x);
	m_Grid_Property.SetTextMatrix(7, 0, sText, 0, DT_LEFT);
	sText.Format("   Zy      = %.3f", pMatData->m_dModulusOfSection.y);
	m_Grid_Property.SetTextMatrix(7, 1, sText, 0, DT_LEFT);
	m_Grid_Property.SetRedraw(TRUE,TRUE);
}

void CDesignCTSteelPage::DrawCTSteel()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::CTSTEEL].GetAt(m_nIndex);
  	CARoadOptionStd		*pOptStd	= m_pDataManage->GetOptionStd();

	m_Domyun.GetDomyun()->ClearEtt(TRUE);
	m_Domyun.GetDomyun()->SetCalcExtRect();
	CDomyun Dom(m_Domyun.GetDomyun());
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double dH  = pMatData->m_dH;
	double dB  = pMatData->m_dB;
	double dT1 = pMatData->m_dT1;
	double dT2 = pMatData->m_dT2;
	double dR1 = pMatData->m_dR1;

	CDPoint vX(1,0), vY(0,1);
	CDPointArray xyPieceArr;
	CDPoint xyTemp(0,0);
	long n = 0;
	for(n = 0; n < 11; n++)
		xyPieceArr.Add(xyTemp);

	xyPieceArr.SetAt(0, CDPoint(0, 0));
	xyPieceArr.SetAt(1, CDPoint(dB/2, 0));
	xyPieceArr.SetAt(2, CDPoint(dB/2, -dT2));
	xyPieceArr.SetAt(3, CDPoint(dT1/2+dR1, -dT2));
	xyPieceArr.SetAt(4, CDPoint(dT1/2, -dT2-dR1));
	xyPieceArr.SetAt(5, CDPoint(dT1/2, -dH));
	xyPieceArr.SetAt(6, CDPoint(-dT1/2, -dH));
	xyPieceArr.SetAt(7, CDPoint(-dT1/2, -dT2-dR1));
	xyPieceArr.SetAt(8, CDPoint(-dT1/2-dR1, -dT2));
	xyPieceArr.SetAt(9, CDPoint(-dB/2, -dT2));
	xyPieceArr.SetAt(10, CDPoint(-dB/2, 0));

	for(n = 0; n < xyPieceArr.GetSize()-1; n++)
	{
		if(n == 3 || n == 7) continue;
		Dom.LineTo(xyPieceArr.GetAt(n), xyPieceArr.GetAt(n+1));
	}
		Dom.LineTo(xyPieceArr.GetAt(10), xyPieceArr.GetAt(0));

	CDPoint xyCen(0,0);
	double dAng1 = 0, dAng2 = 0;
	GetXyMatchLineAndLine(xyPieceArr.GetAt(3), vY, xyPieceArr.GetAt(4), vX, xyCen);
	dAng1 = (xyPieceArr.GetAt(3)-xyCen).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(4)-xyCen).Unit().GetAngleDegree();
	Dom.Arc(xyCen, dR1, dAng1, InAngle(dAng2-dAng1, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(7), vX, xyPieceArr.GetAt(8), vY, xyCen);
	dAng1 = (xyPieceArr.GetAt(7)-xyCen).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(8)-xyCen).Unit().GetAngleDegree();
	Dom.Arc(xyCen, dR1, dAng1, InAngle(dAng2-dAng1, FALSE));

	// 치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	
	Dom.SetCalcExtRect();
	CDRect BoundRect = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale = 2*(dDiagonalL/125);
	Dom.SetScaleDim(dScale);
	
	Dom.SetDimVLen(5);
	Dom.SetDimExo(0);

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xyPieceArr.GetAt(10).x, xyPieceArr.GetAt(10).y, 0, "");
	Dom.DimLineToExtend(dB, 0, Comma(dB), "B");

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(xyPieceArr.GetAt(10).x, xyPieceArr.GetAt(10).y, 0, "");
	Dom.DimLineToExtend(-dT2, 0, Comma(dT2), "T2");

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyPieceArr.GetAt(1).x, xyPieceArr.GetAt(1).y, 0, "");
	Dom.DimLineToExtend(-dH, 0, Comma(dH), "H");

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyPieceArr.GetAt(6).x, xyPieceArr.GetAt(6).y, 0, "");
	Dom.DimLineToExtend(dT1, 0, Comma(dT1), "T1");

	Dom.SetDirection("TOP");
	GetXyMatchLineAndLine(xyPieceArr.GetAt(3), vY, xyPieceArr.GetAt(4), vX, xyCen);
	Dom.GiSiArrowExtend(xyCen.x-dR1/2, xyCen.y+dR1/2, Dom.Always(7), Dom.Always(7), 7, Comma(dR1), "R");

	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	BoundRect.top		-= 5;
	BoundRect.bottom	+= 5;
	BoundRect.left		-= 20;
	BoundRect.right		+= 20;
	Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);

	*m_Domyun.GetDomyun() << Dom;
	m_Domyun.ZoomAll();

}
