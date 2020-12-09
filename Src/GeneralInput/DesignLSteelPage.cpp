// DesignLSteelPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignLSteelPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignLSteelPage dialog
CDesignLSteelPage::CDesignLSteelPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignLSteelPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignLSteelPage)
	m_nIndex	= 0;
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_Domyun.m_bEnablePopupMenu = TRUE;
	m_Domyun.GetDomyun()->SetCWnd(&m_Domyun);
}


void CDesignLSteelPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignLSteelPage)
	DDX_GridControl(pDX, IDC_GRID_LSTEEL, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_LSTEEL_PROPERTY, m_Grid_Property);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_LSTEEL_DOMYUN, m_Domyun);
}


BEGIN_MESSAGE_MAP(CDesignLSteelPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignLSteelPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW		, IDC_GRID_LSTEEL, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignLSteelPage message handlers
void CDesignLSteelPage::OnPreInitDialog()
{
	SetResize(IDC_GRID_LSTEEL,				SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_LSTEEL_DOMYUN,			SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_GRID_LSTEEL_PROPERTY,	SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
}

void CDesignLSteelPage::SetDataDefault()
{	
}

void CDesignLSteelPage::SetDataInit()
{
	SetGridTitle();
	SetLSteelProperty();
	DrawLSteel();
	
	CSteelMaterial *pSteelMat = m_pDataManage->GetSteelMaterial();

	for(long n=0; n<pSteelMat->m_MatDataArray[CSteelMaterialData::LSTEEL].GetSize(); n++)
	{
		CSteelMaterialData *pMatData = pSteelMat->m_MatDataArray[CSteelMaterialData::LSTEEL].GetAt(n);		
		long nCol=0;
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dH);		
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dB);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dT1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dR1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dR2);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dSectionArea);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dUnitWeight);
/*
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dCenterOfGravity.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dCenterOfGravity.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_d2MomentOfInertia.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_d2MomentOfInertia.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_d2MomentOfInertiaMaxMin.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_d2MomentOfInertiaMaxMin.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dRadiusOfGyration.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dRadiusOfGyration.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dRadiusOfGyrationMaxMin.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dRadiusOfGyrationMaxMin.y);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dModulusOfSection.x);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dModulusOfSection.y);
*/
	}
}

void CDesignLSteelPage::SetGridTitle()
{
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(2);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(m_pDataManage->GetSteelMaterial()->m_MatDataArray[CSteelMaterialData::LSTEEL].GetSize()+2);
//	m_Grid.SetColumnCount(19);
	m_Grid.SetColumnCount(7);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(50);			

	m_Grid.SetTextMatrix(0, 0, "치 수 (mm)");
	m_Grid.SetMergeCol(0,0,4);
	m_Grid.SetTextMatrix(1, 0, "H");
	m_Grid.SetTextMatrix(1, 1, "B");
	m_Grid.SetTextMatrix(1, 2, "T");
	m_Grid.SetTextMatrix(1, 3, "R1");
	m_Grid.SetTextMatrix(1, 4, "R2");

	m_Grid.SetTextMatrix(0, 5, "단면적(cm²)");
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
	m_Grid.SetMergeCol(0,9,12);
	m_Grid.SetTextMatrix(1, 9, "Jx");
	m_Grid.SetTextMatrix(1, 10, "Jy");
	m_Grid.SetTextMatrix(1, 11, "최대Jx");
	m_Grid.SetTextMatrix(1, 12, "최소Jy");

	m_Grid.SetTextMatrix(0, 13, "회전반경(cm)");
	m_Grid.SetMergeCol(0,13,16);
	m_Grid.SetTextMatrix(1, 13, "Ix");
	m_Grid.SetTextMatrix(1, 14, "Iy");
	m_Grid.SetTextMatrix(1, 15, "최대Ix");
	m_Grid.SetTextMatrix(1, 16, "최소Iy");

	m_Grid.SetTextMatrix(0, 17, "단면계수(cm3)");
	m_Grid.SetMergeCol(0,17,18);
	m_Grid.SetTextMatrix(1, 17, "Zx");
	m_Grid.SetTextMatrix(1, 18, "Zy");
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

	m_Grid_Property.SetRowCount(10);
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

	m_Grid_Property.SetTextMatrix(5, 0, " · 회전반경 ( cm )", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(5, 0, 1);
	m_Grid_Property.SetItemBkColour(5, 0, Bakckcolor);

	m_Grid_Property.SetTextMatrix(8, 0, " · 단면계수 ( cm³)", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(8, 0, 1);
	m_Grid_Property.SetItemBkColour(8, 0, Bakckcolor);

	m_Grid_Property.SetRedraw(TRUE,TRUE);
}

BOOL CDesignLSteelPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignLSteelPage::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

	int nRow = GridView->iRow;

	m_nIndex = nRow-2;

	SetLSteelProperty();
	DrawLSteel();
}

void CDesignLSteelPage::SetLSteelProperty()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::LSTEEL].GetAt(m_nIndex);
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
	sText.Format("   최대 Jx = %.3f",pMatData->m_d2MomentOfInertiaMaxMin.x);
	m_Grid_Property.SetTextMatrix(4, 0, sText, 0, DT_LEFT);
	sText.Format("   최대 Jy = %.3f",pMatData->m_d2MomentOfInertiaMaxMin.y);
	m_Grid_Property.SetTextMatrix(4, 1, sText, 0, DT_LEFT);
	// 회전반경
	sText.Format("   lx       = %.3f", pMatData->m_dRadiusOfGyration.x);
	m_Grid_Property.SetTextMatrix(6, 0, sText, 0, DT_LEFT);
	sText.Format("   ly       = %.3f", pMatData->m_dRadiusOfGyration.y);
	m_Grid_Property.SetTextMatrix(6, 1, sText, 0, DT_LEFT);
	sText.Format("   최대 lx = %.3f",pMatData->m_dRadiusOfGyrationMaxMin.x);
	m_Grid_Property.SetTextMatrix(7, 0, sText, 0, DT_LEFT);
	sText.Format("   최대 ly = %.3f",pMatData->m_dRadiusOfGyrationMaxMin.y);
	m_Grid_Property.SetTextMatrix(7, 1, sText, 0, DT_LEFT);
	// 단면계수
	sText.Format("   Zx      = %.3f", pMatData->m_dModulusOfSection.x);
	m_Grid_Property.SetTextMatrix(9, 0, sText, 0, DT_LEFT);
	sText.Format("   Zy      = %.3f", pMatData->m_dModulusOfSection.y);
	m_Grid_Property.SetTextMatrix(9, 1, sText, 0, DT_LEFT);
	m_Grid_Property.SetRedraw(TRUE,TRUE);

}

void CDesignLSteelPage::DrawLSteel()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::LSTEEL].GetAt(m_nIndex);
  	CARoadOptionStd		*pOptStd	= m_pDataManage->GetOptionStd();

	m_Domyun.GetDomyun()->ClearEtt(TRUE);
	m_Domyun.GetDomyun()->SetCalcExtRect();
	CDomyun Dom(m_Domyun.GetDomyun());
	
	double dH  = pMatData->m_dH;
	double dB  = pMatData->m_dB;
	double dT1 = pMatData->m_dT1;
	double dR1 = pMatData->m_dR1;
	double dR2 = pMatData->m_dR2;

	CDPoint vX(1,0), vY(0,1);
	CDPoint xyStt = CDPoint(-dB/2, 0);
	CDPoint xyEnd = CDPoint( dB/2, 0);

//	pOptStd->SetEnvType(&Dom, HCAD_CENT);
//	Dom.LineTo(xyStt - vX * dB * 1/3 + vY * dH * 2/3, xyEnd + vX * dB * 1/3 + vY *dH * 2/3);
//	Dom.LineTo(vX*dB*2/3/2-vY*dH*1/3, vX*dB*2/3/2+vY*(dH+dH*1/3));

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.LineTo(xyStt, xyEnd);
	Dom.LineTo(xyStt, xyStt + vY*dH);
	Dom.LineTo(xyStt + vY*dH, xyStt + vX*(dT1-dR2) + vY*dH);
	Dom.LineTo(xyEnd, xyEnd + vY*(dT1-dR2));

	Dom.LineTo(xyStt + vX*(dT1+dR1) + vY * dT1, xyEnd - vX * dR2 + vY * dT1);
	Dom.LineTo(xyStt + vX*dT1 + vY * (dT1 + dR1), xyStt + vX*(dT1) + vY * (dH - dR2));

	CDPoint	xyCen;

	xyCen = xyEnd - vX * dR2 + vY*(dT1-dR2);
	Dom.Arc(xyCen.x, xyCen.y, dR2, 0, 90);
	xyCen = xyStt + vX * (dT1+dR1) + vY*(dT1+dR1);
	Dom.Arc(xyCen.x, xyCen.y, dR1, 180, 90);
	xyCen = xyStt + vX * (dT1 - dR2) + vY*(dH - dR2);
	Dom.Arc(xyCen.x, xyCen.y, dR2, 0, 90);

	// 치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CDRect  BoundRect;

	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale = 4*(dDiagonalL/125);
	Dom.SetScaleDim(dScale);

	Dom.SetDimVLen(5);
	Dom.SetDimExo(0);
	Dom.SetTextHeight(1.5);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(-dB/2, 0, 0, "");
	Dom.DimLineToExtend(dB, 0, "B", Comma(dB));

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(-dB/2, 0, 0, "");
	Dom.DimLineToExtend(dH, 0, "H", Comma(dH));

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-dB/2, dH, 0, "");
	Dom.DimLineToExtend(dT1, 0, "T", Comma(dT1));

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dB/2, 0, 0, "");
	Dom.DimLineToExtend(dT1, 0, "T", Comma(dT1));

	Dom.SetDirection("TOP");
	xyCen = xyEnd-vX * dR2/2 + vY * (dT1 - dR2/2);
	Dom.GiSiArrowExtend(xyCen.x, xyCen.y, Dom.Always(7), Dom.Always(7), 1, Comma(dR2), "R2");

	xyCen = xyStt + vX * (dT1 + dR1/2) + vY * (dT1 + dR1/2);
	Dom.GiSiArrowExtend(xyCen.x, xyCen.y, Dom.Always(7), Dom.Always(7), 1, Comma(dR1), "R1");

	Dom.SetDirection("TOP");
	xyCen = xyStt + vX * (dT1 - dR2/2) + vY * (dH - dR2/2);
	Dom.GiSiArrowExtend(xyCen.x, xyCen.y, Dom.Always(7), Dom.Always(7), 1, Comma(dR2), "R2");

	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	BoundRect.top		-= 5;
	BoundRect.bottom	+= 5;
	BoundRect.left		-= 5;
	BoundRect.right		+= 5;
	Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);	

	*m_Domyun.GetDomyun() << Dom;
	m_Domyun.ZoomAll();

}
