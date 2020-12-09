// DesignCSteelPage.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DesignCSteelPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignCSteelPage dialog
CDesignCSteelPage::CDesignCSteelPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignCSteelPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignCSteelPage)
	m_nIndex = 0;
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_Domyun.m_bEnablePopupMenu = TRUE;
	m_Domyun.GetDomyun()->SetCWnd(&m_Domyun);
}


void CDesignCSteelPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignCSteelPage)
	DDX_GridControl(pDX, IDC_GRID_CSTEEL, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_CSTEEL_PROPERTY, m_Grid_Property);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_CSTEEL_DOMYUN, m_Domyun);
}


BEGIN_MESSAGE_MAP(CDesignCSteelPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignCSteelPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW		, IDC_GRID_CSTEEL, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignCSteelPage message handlers
void CDesignCSteelPage::OnPreInitDialog()
{
	SetResize(IDC_GRID_CSTEEL,				SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_CSTEEL_PROPERTY,	SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_CSTEEL_DOMYUN,			SZ_TOP_RIGHT, SZ_TOP_RIGHT);
}

void CDesignCSteelPage::SetDataDefault()
{	
}

void CDesignCSteelPage::SetDataInit()
{
	SetGridTitle();
	SetCSteelProperty();
	DrawCSteel();

	CSteelMaterial *pSteelMat = m_pDataManage->GetSteelMaterial();

	for(long n=0; n<pSteelMat->m_MatDataArray[CSteelMaterialData::CSTEEL].GetSize(); n++)
	{
		CSteelMaterialData *pMatData = pSteelMat->m_MatDataArray[CSteelMaterialData::CSTEEL].GetAt(n);		
		long nCol=0;
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dH);		
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dB);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dT1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dT2);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dR1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dR2);
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

void CDesignCSteelPage::SetGridTitle()
{
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(2);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(m_pDataManage->GetSteelMaterial()->m_MatDataArray[CSteelMaterialData::CSTEEL].GetSize()+2);
//	m_Grid.SetColumnCount(16);
	m_Grid.SetColumnCount(8);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(50);			

	m_Grid.SetTextMatrix(0, 0, "치 수 (mm)");
	m_Grid.SetMergeCol(0,0,5);
	m_Grid.SetTextMatrix(1, 0, "H");
	m_Grid.SetTextMatrix(1, 1, "B");
	m_Grid.SetTextMatrix(1, 2, "T1");
	m_Grid.SetTextMatrix(1, 3, "T2");
	m_Grid.SetTextMatrix(1, 4, "R1");
	m_Grid.SetTextMatrix(1, 5, "R2");

	m_Grid.SetTextMatrix(0, 6, "단면적(cm2)");
	m_Grid.SetMergeRow(0,1,6);
	m_Grid.SetColumnWidth(6,100);		

	m_Grid.SetTextMatrix(0, 7, "단위중량(kg/m)");
	m_Grid.SetMergeRow(0,1,7);
	m_Grid.SetColumnWidth(7,100);		
/*
	m_Grid.SetTextMatrix(0, 8, "도심위치(cm)");	
	m_Grid.SetMergeCol(0,8,9);
	m_Grid.SetTextMatrix(1, 8, "Cx");
	m_Grid.SetTextMatrix(1, 9, "Cy");

	m_Grid.SetTextMatrix(0, 10, "2차 모멘트 (cm4)");
	m_Grid.SetMergeCol(0,10,11);
	m_Grid.SetTextMatrix(1, 10, "Jx");
	m_Grid.SetTextMatrix(1, 11, "Jy");
	
	m_Grid.SetTextMatrix(0, 12, "회전반경(cm)");
	m_Grid.SetMergeCol(0,12,13);
	m_Grid.SetTextMatrix(1, 12, "Ix");
	m_Grid.SetTextMatrix(1, 13, "Iy");
	
	m_Grid.SetTextMatrix(0, 14, "단면계수(cm3)");
	m_Grid.SetMergeCol(0,14,15);
	m_Grid.SetTextMatrix(1, 14, "Zx");
	m_Grid.SetTextMatrix(1, 15, "Zy");
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
	m_Grid_Property.SetRedraw(TRUE,TRUE);
}

BOOL CDesignCSteelPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignCSteelPage::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

	int nRow = GridView->iRow;

	m_nIndex = nRow-2;

	SetCSteelProperty();
	DrawCSteel();
}

void CDesignCSteelPage::SetCSteelProperty()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::CSTEEL].GetAt(m_nIndex);
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

void CDesignCSteelPage::DrawCSteel()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::CSTEEL].GetAt(m_nIndex);
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
	double dR2 = pMatData->m_dR2;

	CDPoint vX(1,0), vY(0,1);
	CDPoint xyUpper(1,0), xyLower(1,0);
	CDPoint xy5Degree = ToDPointFrRadian(ToRadian(5));
	xyUpper = xyUpper.Rotate(xy5Degree);
	xyLower = xyLower.RotateInv(xy5Degree);
	
	CDPointArray xyPieceArr, xyCent;
	CDPoint xyTemp(0,0);
	long n = 0;
	for(n=0; n<13; n++)
		xyPieceArr.Add(xyTemp);
	for(n=0; n<5; n++)
		xyCent.Add(xyTemp);

	xyPieceArr.SetAt(0, CDPoint(0, 0));
	xyPieceArr.SetAt(1, CDPoint(0, dH/2));
	xyPieceArr.SetAt(2, CDPoint(dB, dH/2));
	xyPieceArr.SetAt(11, CDPoint(dB, -dH/2));
	xyPieceArr.SetAt(12, CDPoint(0, -dH/2));

	CDPoint xyA = xyPieceArr.GetAt(1)  + vX*((dB-dT1)/2+dT1) - vY*dT2;
	CDPoint xyB = xyPieceArr.GetAt(12) + vX*((dB-dT1)/2+dT1) + vY*dT2;

	CDPoint xyCen(0,0);
	double dAng1 = 0, dAng2 = 0;

	GetXyMatchLineAndLine(xyPieceArr.GetAt(2)-vX*dR2, vY, xyA+xyUpper.Rotate90()*dR2, xyUpper, xyCent[0]);
	GetXyMatchLineAndLine(xyPieceArr[2], -vY, xyCent[0], vX, xyPieceArr[3]);
	GetXyMatchLineAndLine(xyA, xyUpper, xyCent[0], xyUpper.RotateInv90(), xyPieceArr[4]);
	dAng1 = (xyPieceArr.GetAt(3)-xyCent[0]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(4)-xyCent[0]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[0], dR2, dAng2, InAngle(dAng1-dAng2, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(0)+vX*(dT1+dR1), vY, xyA-xyUpper.Rotate90()*dR1, xyUpper, xyCent[1]);
	GetXyMatchLineAndLine(xyA, xyUpper, xyCent[1], xyUpper.Rotate90(), xyPieceArr[5]);
	GetXyMatchLineAndLine(xyPieceArr[0]+vX*dT1, vY, xyCent[1], -vX, xyPieceArr[6]);
	dAng1 = (xyPieceArr.GetAt(5)-xyCent[1]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(6)-xyCent[1]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[1], dR1, dAng1, InAngle(dAng2-dAng1, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(0)+vX*(dT1+dR1), -vY, xyB+xyLower.Rotate90()*dR1, xyLower, xyCent[2]);	
	GetXyMatchLineAndLine(xyPieceArr[0]+vX*dT1, vY, xyCent[2], -vX, xyPieceArr[7]);
	GetXyMatchLineAndLine(xyB, xyLower, xyCent[2], xyLower.Rotate90(), xyPieceArr[8]);
	dAng1 = (xyPieceArr.GetAt(7)-xyCent[2]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(8)-xyCent[2]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[2], dR1, dAng1, InAngle(dAng2-dAng1, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(11)-vX*dR2, vY, xyB+xyLower.RotateInv90()*dR2, xyLower, xyCent[3]);
	GetXyMatchLineAndLine(xyB, xyLower, xyCent[3], xyLower.RotateInv90(), xyPieceArr[9]);
	GetXyMatchLineAndLine(xyPieceArr[11], vY, xyCent[3], vX, xyPieceArr[10]);
	dAng1 = (xyPieceArr.GetAt(9)-xyCent[3]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(10)-xyCent[3]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[3], dR2, dAng2, InAngle(dAng1-dAng2, FALSE));

	Dom.LineTo(xyPieceArr.GetAt(1), xyPieceArr.GetAt(12));
	Dom.LineTo(xyPieceArr.GetAt(1), xyPieceArr.GetAt(2));
	Dom.LineTo(xyPieceArr.GetAt(2), xyPieceArr.GetAt(3));
	Dom.LineTo(xyPieceArr.GetAt(4), xyPieceArr.GetAt(5));
	Dom.LineTo(xyPieceArr.GetAt(6), xyPieceArr.GetAt(7));
	Dom.LineTo(xyPieceArr.GetAt(8), xyPieceArr.GetAt(9));
	Dom.LineTo(xyPieceArr.GetAt(10), xyPieceArr.GetAt(11));
	Dom.LineTo(xyPieceArr.GetAt(11), xyPieceArr.GetAt(12));

	// 치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CDRect  BoundRect;

	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale = 2.5*(dDiagonalL/125);
	Dom.SetScaleDim(dScale);

	Dom.SetDimVLen(5);
	Dom.SetDimExo(0);

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(xyPieceArr.GetAt(1).x, xyPieceArr.GetAt(1).y, 0, "");
	Dom.DimLineToExtend(-dH, 0, Comma(dH), "H");

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyPieceArr.GetAt(2).x, xyPieceArr.GetAt(2).y, 0, "");
	Dom.DimLineToExtend(-dT2, 0, Comma(dT2), "T2");

	Dom.DimMoveTo(xyPieceArr.GetAt(11).x, xyPieceArr.GetAt(11).y, 0, "");
	Dom.DimLineTo(dT2, 0, Comma(dT2));

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyPieceArr.GetAt(12).x, xyPieceArr.GetAt(12).y, 0, "");
	Dom.DimLineToExtend(dB, 0, Comma(dB), "B");

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xyPieceArr.GetAt(1).x, xyPieceArr.GetAt(1).y, 0, "");
	Dom.DimLineToExtend(dT1, 0, Comma(dT1), "T1");

	Dom.SetDirection("TOP");
	GetXyMatchLineAndLine(xyPieceArr.GetAt(9), vY, xyPieceArr.GetAt(10), xyLower, xyCen);
	Dom.GiSiArrowExtend(xyCen.x+dR2/2, xyCen.y+dR2/2, Dom.Always(7), Dom.Always(7), 1, Comma(dR2), "R2");

	Dom.SetDirection("TOP");
	GetXyMatchLineAndLine(xyPieceArr.GetAt(5), vY, xyPieceArr.GetAt(6), xyUpper, xyCen);
	Dom.GiSiArrowExtend(xyCen.x-dR1/2, xyCen.y+dR1/2, Dom.Always(7), Dom.Always(7), 7, Comma(dR1), "R1");

	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	BoundRect.top		-= 5;
	BoundRect.bottom	+= 5;
	BoundRect.left		-= 5;
	BoundRect.right		+= 5;
	Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);

	*m_Domyun.GetDomyun() << Dom;
	m_Domyun.ZoomAll();
}
