// DesignISteelPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignISteelPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignISteelPage dialog
CDesignISteelPage::CDesignISteelPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignISteelPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignISteelPage)
	m_nIndex	= 0;
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_Domyun.m_bEnablePopupMenu = TRUE;
}


void CDesignISteelPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignISteelPage)
	DDX_GridControl(pDX, IDC_GRID_ISTEEL, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_ISTEEL_PROPERTY, m_Grid_Property);
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_ISTEEL_DOMYUN, m_Domyun);
}


BEGIN_MESSAGE_MAP(CDesignISteelPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignISteelPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW		, IDC_GRID_ISTEEL, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignISteelPage message handlers
void CDesignISteelPage::OnPreInitDialog()
{
	SetResize(IDC_GRID_ISTEEL,				SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_ISTEEL_PROPERTY,	SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_ISTEEL_DOMYUN,			SZ_TOP_RIGHT, SZ_TOP_RIGHT);
}

void CDesignISteelPage::SetDataDefault()
{	
}

void CDesignISteelPage::SetDataInit()
{
	SetGridTitle();
	SetISteelProperty();
	DrawISteel();

	CSteelMaterial *pSteelMat = m_pDataManage->GetSteelMaterial();

	for(long n=0; n<pSteelMat->m_MatDataArray[CSteelMaterialData::ISTEEL].GetSize(); n++)
	{
		CSteelMaterialData *pMatData = pSteelMat->m_MatDataArray[CSteelMaterialData::ISTEEL].GetAt(n);		
		long nCol=0;
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dH);		
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dB);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dT1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dT2);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dR1);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dR2);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dSectionArea);
		m_Grid.SetTextMatrix(n+2,nCol++,"%g",pMatData->m_dUnitWeight);
	}
}

void CDesignISteelPage::SetGridTitle()
{
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(2);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(m_pDataManage->GetSteelMaterial()->m_MatDataArray[CSteelMaterialData::ISTEEL].GetSize()+2);
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

	m_Grid_Property.SetRowCount(6);
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

	m_Grid_Property.SetTextMatrix(0, 0, " · 2차 모멘트 ( cm⁴)", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(0, 0, 1);
	m_Grid_Property.SetItemBkColour(0, 0, Bakckcolor);

	m_Grid_Property.SetTextMatrix(2, 0, " · 회전반경 ( cm )", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(2, 0, 1);
	m_Grid_Property.SetItemBkColour(2, 0, Bakckcolor);

	m_Grid_Property.SetTextMatrix(4, 0, " · 단면계수 ( cm³)", 0, DT_LEFT);
	m_Grid_Property.SetMergeCol(4, 0, 1);
	m_Grid_Property.SetItemBkColour(4, 0, Bakckcolor);
	m_Grid_Property.SetRedraw(TRUE,TRUE);
}

BOOL CDesignISteelPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();

	m_Domyun.GetDomyun()->SetCWnd(&m_Domyun);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDesignISteelPage::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

	int nRow = GridView->iRow;

	m_nIndex = nRow-2;

	SetISteelProperty();
	DrawISteel();
}

void CDesignISteelPage::SetISteelProperty()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::ISTEEL].GetAt(m_nIndex);
	CString				sText;

	// 단면2차모멘트
	sText.Format("   Jx       = %.3f", pMatData->m_d2MomentOfInertia.x);
	m_Grid_Property.SetTextMatrix(1, 0, sText, 0, DT_LEFT);
	sText.Format("   Jy       = %.3f", pMatData->m_d2MomentOfInertia.y);
	m_Grid_Property.SetTextMatrix(1, 1, sText, 0, DT_LEFT);
	// 회전반경
	sText.Format("   lx       = %.3f", pMatData->m_dRadiusOfGyration.x);
	m_Grid_Property.SetTextMatrix(3, 0, sText, 0, DT_LEFT);
	sText.Format("   ly       = %.3f", pMatData->m_dRadiusOfGyration.y);
	m_Grid_Property.SetTextMatrix(3, 1, sText, 0, DT_LEFT);
	// 단면계수
	sText.Format("   Zx      = %.3f", pMatData->m_dModulusOfSection.x);
	m_Grid_Property.SetTextMatrix(5, 0, sText, 0, DT_LEFT);
	sText.Format("   Zy      = %.3f", pMatData->m_dModulusOfSection.y);
	m_Grid_Property.SetTextMatrix(5, 1, sText, 0, DT_LEFT);
	m_Grid_Property.SetRedraw(TRUE,TRUE);
}

void CDesignISteelPage::DrawISteel()
{
	CSteelMaterial		*pSteelMat	= m_pDataManage->GetSteelMaterial();
	CSteelMaterialData	*pMatData	= pSteelMat->m_MatDataArray[CSteelMaterialData::ISTEEL].GetAt(m_nIndex);
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
	CDPoint xy8Degree = ToDPointFrRadian(ToRadian(8));
	xyUpper = xyUpper.Rotate(xy8Degree);
	xyLower = xyLower.RotateInv(xy8Degree);
	
	CDPointArray xyPieceArr, xyCent;
	CDPoint xyTemp(0,0);
	long n = 0;
	for(n=0; n<22; n++)
		xyPieceArr.Add(xyTemp);
	for(n=0; n<9; n++)
		xyCent.Add(xyTemp);

	xyPieceArr.SetAt(0, CDPoint(0, 0));
	xyPieceArr.SetAt(1, CDPoint(0, dH/2));
	xyPieceArr.SetAt(2, CDPoint(dB/2, dH/2));
	xyPieceArr.SetAt(11, CDPoint(dB/2, -dH/2));
	xyPieceArr.SetAt(12, CDPoint(0, -dH/2));

	CDPoint xyA = xyPieceArr.GetAt(1)  + vX*((dB-dT1)/4) - vY*dT2;
	CDPoint xyB = xyPieceArr.GetAt(12) + vX*((dB-dT1)/4) + vY*dT2;
	CDPoint xyC = xyPieceArr.GetAt(1)  - vX*((dB-dT1)/4) - vY*dT2;
	CDPoint xyD = xyPieceArr.GetAt(12) - vX*((dB-dT1)/4) + vY*dT2;

	CDPoint xyCen(0,0);
	double dAng1 = 0, dAng2 = 0;

	GetXyMatchLineAndLine(xyPieceArr.GetAt(2)-vX*dR2, vY, xyA+xyUpper.Rotate90()*dR2, xyUpper, xyCent[0]);
	GetXyMatchLineAndLine(xyPieceArr[2], -vY, xyCent[0], vX, xyPieceArr[3]);
	GetXyMatchLineAndLine(xyA, xyUpper, xyCent[0], xyUpper.RotateInv90(), xyPieceArr[4]);
	dAng1 = (xyPieceArr.GetAt(3)-xyCent[0]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(4)-xyCent[0]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[0], dR2, dAng2, InAngle(dAng1-dAng2, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(0)+vX*(dT1/2+dR1), vY, xyA-xyUpper.Rotate90()*dR1, xyUpper, xyCent[1]);
	GetXyMatchLineAndLine(xyA, xyUpper, xyCent[1], xyUpper.Rotate90(), xyPieceArr[5]);
	GetXyMatchLineAndLine(xyPieceArr[0]+vX*(dT1/2), vY, xyCent[1], -vX, xyPieceArr[6]);
	dAng1 = (xyPieceArr.GetAt(5)-xyCent[1]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(6)-xyCent[1]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[1], dR1, dAng1, InAngle(dAng2-dAng1, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(0)+vX*(dT1/2+dR1), -vY, xyB+xyLower.Rotate90()*dR1, xyLower, xyCent[2]);	
	GetXyMatchLineAndLine(xyPieceArr[0]+vX*(dT1/2), vY, xyCent[2], -vX, xyPieceArr[7]);
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

	long nIdx = 11;
	for(n = 12; n < 22; n++)
	{
		xyTemp = xyPieceArr.GetAt(nIdx);
		xyTemp.MirrorHorz();
		xyPieceArr.SetAt(n, xyTemp);
		nIdx--;
	}

	CDPoint xyLUpper(1,0), xyLLower(1,0);
	xyLUpper = xyLUpper.Rotate(ToDPointFrRadian(ToRadian(-8)));
	xyLLower = xyLLower.RotateInv(ToDPointFrRadian(ToRadian(-8)));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(21)+vX*dR2, vY, xyC+xyLUpper.Rotate90()*dR2, xyLUpper, xyCent[4]);
	dAng1 = (xyPieceArr.GetAt(19)-xyCent[4]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(20)-xyCent[4]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[4], dR2, dAng2, InAngle(dAng1-dAng2, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(0)-vX*(dT1/2+dR1), vY, xyC-xyLUpper.Rotate90()*dR1, xyLUpper, xyCent[5]);
	dAng1 = (xyPieceArr.GetAt(17)-xyCent[5]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(18)-xyCent[5]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[5], dR1, dAng1, InAngle(dAng2-dAng1, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(0)-vX*(dT1/2+dR1), -vY, xyD+xyLLower.Rotate90()*dR1, xyLLower, xyCent[6]);
	dAng1 = (xyPieceArr.GetAt(15)-xyCent[6]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(16)-xyCent[6]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[6], dR1, dAng1, InAngle(dAng2-dAng1, FALSE));

	GetXyMatchLineAndLine(xyPieceArr.GetAt(12)+vX*dR2, vY, xyD+xyLLower.RotateInv90()*dR2, xyLLower, xyCent[7]);
	dAng1 = (xyPieceArr.GetAt(13)-xyCent[7]).Unit().GetAngleDegree();
	dAng2 = (xyPieceArr.GetAt(14)-xyCent[7]).Unit().GetAngleDegree();
	Dom.Arc(xyCent[7], dR2, dAng2, InAngle(dAng1-dAng2, FALSE));

	for(n = 1; n < xyPieceArr.GetSize()-1; n++)
	{
		if(n ==3 || n == 5 || n == 7 || n == 9 || n == 13 || n == 15 || n == 17 || n == 19) continue;
		Dom.LineTo(xyPieceArr.GetAt(n), xyPieceArr.GetAt(n+1));
	}
	Dom.LineTo(xyPieceArr.GetAt(21), xyPieceArr.GetAt(1));

	// 치수선
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CDRect  BoundRect;

	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale = 2.5*(dDiagonalL/145);
	Dom.SetScaleDim(dScale);
	Dom.SetDimVLen(5);
	Dom.SetDimExo(0);
///	Dom.SetTextHeight(1.5);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(-dB/2, -dH/2, 0, "");
	Dom.DimLineToExtend(dB, 0, "B", Comma(dB));

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dB/2, dH/2, 0, "");
	Dom.DimLineToExtend(-dH, 0, "H", Comma(dH));

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-dT1/2, 0, 0);
	Dom.DimLineToOverExtend(dT1, 0, "T1", Comma(dT1));

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(-dT1/2 - (dB -dT1)/4, dH/2, 0);
	Dom.DimLineToOverExtend(-dT2, 0, "T2", Comma(dT2));

	Dom.SetDirection("TOP");
	Dom.GiSiArrowExtend(xyCent[0].x+dR2/2, xyCent[0].y-dR2/2, Dom.Always(7), Dom.Always(7), 7, Comma(dR2), "R2");

	Dom.SetDirection("TOP");
	Dom.GiSiArrowExtend(xyCent[2].x-dR1/2, xyCent[2].y-dR1/2, Dom.Always(7), Dom.Always(7), 1, Comma(dR1), "R1");


	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	BoundRect.top		-= 5;
	BoundRect.bottom	+= 5;
	BoundRect.left		-= 5;
	BoundRect.right		+= 5;
	Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);	
	*m_Domyun.GetDomyun() << Dom;
	m_Domyun.ZoomAll();
}
