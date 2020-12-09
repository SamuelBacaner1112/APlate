// DeckBindConcRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckBindConcRebarDlg dialog


CDeckBindConcRebarDlg::CDeckBindConcRebarDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckBindConcRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckBindConcRebarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDeckBindConcRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckBindConcRebarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_GridOpt);
	DDX_GridControl(pDX, IDC_GRID_CTC, m_GridCTC);
}


BEGIN_MESSAGE_MAP(CDeckBindConcRebarDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckBindConcRebarDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedDataOpt)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_CTC, OnCellChangedDataCTC)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckBindConcRebarDlg message handlers
void CDeckBindConcRebarDlg::OnPreInitDialog()
{
	UpdateData(FALSE);

	SetResize(IDC_STATIC1,			SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC2,			SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);	
	SetResize(IDC_GRID,				SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GRID_CTC,			SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}


void CDeckBindConcRebarDlg::SetGridData()
{
	SetGridDataOpt();
	SetGridDataCTC();
}

void CDeckBindConcRebarDlg::SetGridDataOpt()
{
	CADeckDataTU *pDeckTU = m_pStd->m_pDataManage->GetDeckDataTU();

	long nColsu = 12;
	long nRowsu = 6;
	long nLine = 0;	

	m_GridOpt.SetGridDefault(nRowsu, nColsu, 1, 0);

	m_GridOpt.AddCell(0, 0, "앵커철근제원");
	m_GridOpt.AddCell(0, 1, "앵커철근제원");	
	m_GridOpt.AddCell(0, 2, "피복(mm)");		
	m_GridOpt.AddCell(0, 3, "피복(mm)");		
	m_GridOpt.AddCell(0, 4, "횡방향철근제원");
	m_GridOpt.AddCell(0, 5, "횡방향철근제원");
	m_GridOpt.AddCell(0, 6, "종방향철근제원");
	m_GridOpt.AddCell(0, 7, "종방향철근제원");
	m_GridOpt.AddCell(0, 8, "기타철근제원");
	m_GridOpt.AddCell(0, 9, "기타철근제원");
	m_GridOpt.AddCell(0,10, "헌치보강철근제원");
	m_GridOpt.AddCell(0,11, "헌치보강철근제원");

	m_GridOpt.AddCell(1, 0, "앵커개수");
	m_GridOpt.AddCell(2, 0, "기본간격");
	m_GridOpt.AddCell(3, 0, "앵커폭(Aw)");
	m_GridOpt.AddCell(4, 0, "돌출높이(Ah)");
	m_GridOpt.AddCell(5, 0, "앵커간간격(Ad)");
	
	m_GridOpt.AddCell(1, 2, "상면(Uc)");
	m_GridOpt.AddCell(2, 2, "하면(Dc)");
	m_GridOpt.AddCell(3, 2, "측면(Sc)");
	m_GridOpt.AddCell(4, 2, "단부");
	m_GridOpt.AddCell(5, 2, "헌치면(Hc)");

	m_GridOpt.AddCell(1, 4, "스트럽");
	m_GridOpt.AddCell(2, 4, "단부");
	m_GridOpt.AddCell(3, 4, "앵커");
	m_GridOpt.AddCell(4, 4, "사보강");

	m_GridOpt.AddCell(1, 6, "상면");
	m_GridOpt.AddCell(2, 6, "하면");
	m_GridOpt.AddCell(3, 6, "측면");
	m_GridOpt.AddCell(4, 6, "모서리");

	m_GridOpt.AddCell(1, 8, "단부보강구간");
	m_GridOpt.AddCell(2, 8, "종방향분할수(E)");
	m_GridOpt.AddCell(3, 8, "사보강개수");

	m_GridOpt.AddCell(1,10, "헌치보강");
	m_GridOpt.AddCell(2,10, "헌치철근개수");
	m_GridOpt.AddCell(3,10, "돌출길이(Hd)");
	m_GridOpt.AddCell(4,10, "배치간격");

	// 앵커철근제원
	CStringArray szComboArr;
	szComboArr.Add("1");
	szComboArr.Add("2");
	szComboArr.Add("3");
	m_GridOpt.AddCell(1, 1, &pDeckTU->m_nQtyAnchor, DT_CENTER, FALSE, nLine, "", CELL_TYPE_COMBO, szComboArr);
	m_GridOpt.AddCell(2, 1, &pDeckTU->m_dCTCAnchor);
	m_GridOpt.AddCell(3, 1, &pDeckTU->m_dWidthAnchor);
	m_GridOpt.AddCell(4, 1, &pDeckTU->m_dLengthDolchul);
	m_GridOpt.AddCell(5, 1, &pDeckTU->m_dDisAnchor);
	// 피복
	m_GridOpt.AddCell(1, 3, &pDeckTU->m_dCoverUp);
	m_GridOpt.AddCell(2, 3, &pDeckTU->m_dCoverDn);
	m_GridOpt.AddCell(3, 3, &pDeckTU->m_dCoverSd);
	m_GridOpt.AddCell(4, 3, &pDeckTU->m_dCoverDanbu);
	m_GridOpt.AddCell(5, 3, &pDeckTU->m_dCoverHunch);
	// 횡방향철근제원
	m_GridOpt.AddCell(1, 5, &pDeckTU->m_dDiaStrup, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	m_GridOpt.AddCell(2, 5, &pDeckTU->m_dDiaDanbu, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	m_GridOpt.AddCell(3, 5, &pDeckTU->m_dDiaAnchor, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	m_GridOpt.AddCell(4, 5, &pDeckTU->m_dDiaSabong, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	// 종방향철근제원
	m_GridOpt.AddCell(1, 7, &pDeckTU->m_dDiaUpper, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	m_GridOpt.AddCell(2, 7, &pDeckTU->m_dDiaLower, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	m_GridOpt.AddCell(3, 7, &pDeckTU->m_dDiaSide, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	m_GridOpt.AddCell(4, 7, &pDeckTU->m_dDiaEdge, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	// 기타제원
	m_GridOpt.AddCell(1, 9, &pDeckTU->m_dRangeDanbuBogang);
	m_GridOpt.AddCell(2, 9, &pDeckTU->m_nQtyDivideJong);
	m_GridOpt.AddCell(3, 9, &pDeckTU->m_nQtySabogang);
	// 헌치보강철근제원
	m_GridOpt.AddCell(1,11, &pDeckTU->m_dDiaHunch, DT_CENTER, nLine, "", CELL_TYPE_DIA);
	m_GridOpt.AddCell(2,11, &pDeckTU->m_nQtyHunch);
	m_GridOpt.AddCell(3,11, &pDeckTU->m_dOutLength);
	m_GridOpt.AddCell(4,11, &pDeckTU->m_dDisHunch);
}

void CDeckBindConcRebarDlg::SetGridDataCTC()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();
	CADeckDataTU *pDeckTU = m_pStd->m_pDataManage->GetDeckDataTU();

	long nMaxGugan = 0;
	long nG = 0;
	for (nG=0; nG<pDB->GetGirdersu(); nG++)
		nMaxGugan = max(nMaxGugan, pDeckTU->m_arrCTC[nG].GetSize());

	long nColsu = nMaxGugan+5;
	long nRowsu = pDB->GetGirdersu()*2+1;

	CString sText;
	m_GridCTC.SetGridDefault(nRowsu, nColsu, 1, 2);
	m_GridCTC.AddCell(0, 0, "위치");
	m_GridCTC.AddCell(0, 1, "위치");

	m_GridCTC.AddCell(0, 2, "피복");
	for (long n=0; n<nMaxGugan; n++)
	{
		sText.Format("구간%d", n+1);
		m_GridCTC.AddCell(0, n+3, sText);
	}
	m_GridCTC.AddCell(0, nMaxGugan+3, "피복");
	m_GridCTC.AddCell(0, nMaxGugan+4, "합계");

	for (nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		sText.Format("G%d\n", nG+1);
		m_GridCTC.AddCell(nG*2+1, 0, sText);
		m_GridCTC.AddCell(nG*2+2, 0, sText);

		m_GridCTC.AddCell(nG*2+1, 1, "개수");
		m_GridCTC.AddCell(nG*2+2, 1, "간격");
	}

	for (nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CDPointArray arrXyCTC = pDeckTU->m_arrCTC[nG];
		long nGugansu = arrXyCTC.GetSize();
		CDPoint	xyDimTemp	= CDPoint(0,0);
		
		m_GridCTC.AddCell(nG*2+1, 2, "1");
		m_GridCTC.AddCell(nG*2+2, 2, COMMA(pDeckTU->m_dCoverDanbu));//	m_GridCTC.SetItemState(nG*2+2, 2, GVIS_READONLY);

		for (long n=0; n<nGugansu; n++)
		{
			m_GridCTC.AddCell(nG*2+1, n+3, &pDeckTU->m_arrCTC[nG], n, TRUE);
			m_GridCTC.AddCell(nG*2+2, n+3, &pDeckTU->m_arrCTC[nG], n, FALSE);

			xyDimTemp.x	+= arrXyCTC.GetAt(n).x;
			xyDimTemp.y	+= arrXyCTC.GetAt(n).y *arrXyCTC.GetAt(n).x;
		}
		m_GridCTC.AddCell(nG*2+1,arrXyCTC.GetSize()+3, "1");
		m_GridCTC.AddCell(nG*2+2, arrXyCTC.GetSize()+3, COMMA(pDeckTU->m_dCoverDanbu));	m_GridCTC.SetItemState(nG*2+2, arrXyCTC.GetSize()+3, GVIS_READONLY);

		sText.Format("%.0f", xyDimTemp.x+2);
		m_GridCTC.AddCell(nG*2+1, arrXyCTC.GetSize()+4, sText);
		sText.Format("%.0f", xyDimTemp.y+pDeckTU->m_dCoverDanbu*2);
		m_GridCTC.AddCell(nG*2+2, arrXyCTC.GetSize()+4, sText);
	}
}

void CDeckBindConcRebarDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckDataTU *pDeckDataTU = m_pStd->m_pDeckDataTU;
	CLineInfoApp *pLineApp = m_pStd->m_pDataManage->GetLineInfoApp();

//	BOOL bFirst = !IsUsedFlag();
//	if(bFirst)
	{
		pDeckDataTU->m_dCTCAnchor = 200;		// 앵커철근간격
		pDeckDataTU->m_dRangeDanbuBogang = 800;	// 단부보강구간
		pDeckDataTU->m_nQtySabogang = 10;		// 사보강개수
		pDeckDataTU->m_nQtyDivideJong = 8;		// 종방향분할수
		pDeckDataTU->m_nQtyHunch = 9;			// 헌치철근 개수
		pDeckDataTU->m_nQtyAnchor = 2;			// 앵커철근개수

		// 피복
		pDeckDataTU->m_dCoverUp = 70;		// 피복(상면)
		pDeckDataTU->m_dCoverDn = 70;		// 피복(하면)
		pDeckDataTU->m_dCoverSd = 70;		// 피복(측면)
		pDeckDataTU->m_dCoverDanbu = 50;	// 피복(단부)
		pDeckDataTU->m_dCoverHunch = 40;	// 피복(헌치)

		// 횡방향철근제원(직경)
		pDeckDataTU->m_dDiaStrup = 13;		// 횡방향철근
		pDeckDataTU->m_dDiaDanbu = 13;		// 단부철근
		pDeckDataTU->m_dDiaAnchor = 13;		// 앵커철근
		pDeckDataTU->m_dDiaSabong = 13;		// 사보강철근

		// 종방향철근제원(직경)
		pDeckDataTU->m_dDiaUpper = 16;		// 상면
		pDeckDataTU->m_dDiaLower = 16;		// 하면
		pDeckDataTU->m_dDiaSide = 16;		// 측면
		pDeckDataTU->m_dDiaEdge = 16;		// 모서리

		// 앵커철근제원
		pDeckDataTU->m_dWidthAnchor = 180;	// 앵커폭
		pDeckDataTU->m_dLengthDolchul = 32;	// 돌출높이
		pDeckDataTU->m_dDisAnchor = 600;	// 거더중심으로부터 이격거리

		// 헌치철근제원
		pDeckDataTU->m_dDiaHunch = 16;		// 헌치보강철근 직경
		pDeckDataTU->m_dOutLength = 460;	// 외측돌출길이
		pDeckDataTU->m_dDisHunch = 100;		// 배치간격	

		// 거더별 선형생성
		CString sName;
		long nLine =0;
		// 선형이 입력된 순서는 거더별 선형 -> 구속콘 배근용 선형
		for(nLine=0; pLineApp->GetLinesu(); nLine++)
		{
			BOOL bCheck = FALSE;
			for(long nG=-1; nG<pDB->GetQtyGirder(); nG++)
			{
				CLineInfo *pLine = pDB->GetGirder(nG)->GetLine();
				if(pLineApp->GetLine(nLine)==pLine)
				{
					bCheck = TRUE;
					break;
				}
			}
			
			if(bCheck)
				break;
		}
		for(long nDel=pLineApp->GetLinesu()-1; nDel>nLine; nDel--)
			pLineApp->DeleteLineInfo(1);
		
		for (long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			long nLineNumber = pGir->m_nLineNum;

			CLineInfo* pLine = pLineApp->GetLineByOffset(nLineNumber, pGir->m_dGirderCenter);
			sName.Format("G%d", nG);
			pLine->GetNameLineEach() = sName;
			pLineApp->AddLineInfo(pLine);
		}
	}
	
	pDeckDataTU->MakeDimArr();
	pDeckDataTU->MakePlanMainRebarTU();
	pDeckDataTU->MakePlanSabogangRebarTU();
	pDeckDataTU->MakePlanHunchRebarTU();
	pDeckDataTU->MakeShapeBeryukRebarTU();
	pDeckDataTU->MakeShapeStrupRebarTU();
}

void CDeckBindConcRebarDlg::SetDataInit()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();
	CADeckDataTU *pDeckTU = m_pStd->m_pDeckDataTU;
	SetGridData();

	m_GridOpt.UpdateData(FALSE);
	m_GridOpt.SetColumnWidthAll(50);
	m_GridOpt.SetColumnWidth(0, 100);
	m_GridOpt.SetColumnWidth(2, 80);
	m_GridOpt.SetColumnWidth(4, 80);
	m_GridOpt.SetColumnWidth(6, 80);
	m_GridOpt.SetColumnWidth(8, 100);
	m_GridOpt.SetColumnWidth(10,100);
	m_GridOpt.SetRedraw(TRUE, TRUE);
	m_GridOpt.EnableInsertCol(FALSE);
	m_GridOpt.EnableInsertRow(FALSE);
	m_GridOpt.MergeGrid(0, 0, 0, m_GridOpt.GetColumnCount());
	long nRow = 0;
	for(nRow=1; nRow<6; nRow++)
	{
		for(long nCol=0; nCol<11; nCol+=2)
		{
			m_GridOpt.SetItemState(nRow, nCol, GVIS_READONLY);
			m_GridOpt.SetItemBkColour(nRow, nCol, RGBREADONLY);
		}
	}
	for(long nCol=5; nCol<12; nCol+=2)
	{
		m_GridOpt.SetItemState(5, nCol, GVIS_READONLY);
		m_GridOpt.SetItemBkColour(5, nCol, RGBREADONLY);
	}
	m_GridOpt.SetItemState(4, 9, GVIS_READONLY);
	m_GridOpt.SetItemBkColour(4, 9, RGBREADONLY);
	m_GridOpt.SetRedraw(TRUE, TRUE);

	m_GridCTC.UpdateData(FALSE);
	m_GridCTC.SetColumnWidthAll(80);
	m_GridCTC.SetRedraw(TRUE, TRUE);
	m_GridCTC.EnableInsertCol(FALSE);
	m_GridCTC.EnableInsertRow(FALSE);
	m_GridCTC.MergeGrid(0, m_GridCTC.GetRowCount(), 0, 1);

	for(nRow=1; nRow<pDB->GetQtyGirder()*2+1; nRow++)
	{
		long nG = (nRow-1)/2;
		for(long nCol=2; nCol<pDeckTU->m_arrCTC[nG].GetSize()+5; nCol++)
		{
			m_GridCTC.SetItemState(nRow, nCol, GVIS_READONLY);
			m_GridCTC.SetItemBkColour(nRow, nCol, RGBREADONLY);
		}
	}
	m_GridCTC.SetRedraw(TRUE, TRUE);

	UpdateData(FALSE);
}

void CDeckBindConcRebarDlg::SetDataSave()
{
	CADeckData		*pDeckData	= m_pStd->m_pDeckData;
	CADeckDataTU	*pDeckDataTU= m_pStd->m_pDeckDataTU;

	m_GridOpt.UpdateData(TRUE);	
	m_GridCTC.UpdateData(TRUE);	

	pDeckDataTU->MakeDimArr();
	pDeckDataTU->MakePlanMainRebarTU();
	pDeckDataTU->MakePlanSabogangRebarTU();
	pDeckDataTU->MakePlanHunchRebarTU();
	pDeckDataTU->MakeShapeBeryukRebarTU();
	pDeckDataTU->MakeShapeStrupRebarTU();
	
	if(pDeckData->m_bRecalcJewon && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag())
		pDeckDataTU->MakeRebarJewonTU();
}

void CDeckBindConcRebarDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	double dScaleDim = 0;

	switch(m_nTab)
	{
	case 0:
		dScaleDim = 50;
		Dom.SetScaleDim(dScaleDim);
		DrawPlanRebarBindConc(&Dom);
		break;
	case 1:
		dScaleDim = 50;
		Dom.SetScaleDim(dScaleDim);
		DrawJongRebarBindConc(&Dom);
		break;
	case 2:
		dScaleDim = 20;
		Dom.SetScaleDim(dScaleDim);
		DrawCrossRebarBindConc(&Dom);
		break;
	case 3:
		dScaleDim = 20;
		Dom.SetScaleDim(dScaleDim);
		DrawPlanHunchRebarBindConc(&Dom);
		break;
	}

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);

}

void CDeckBindConcRebarDlg::OnCellChangedDataOpt(NMHDR*nmgv,LRESULT*)
{
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
	
}

void CDeckBindConcRebarDlg::OnCellChangedDataCTC(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
	
}

BOOL CDeckBindConcRebarDlg::IsValid()
{
	BOOL bTU = m_pStd->GetBridge()->IsTUGir();
	return bTU&&m_pStd->m_bInclude_Module_Deck ? TRUE : FALSE;
}

long CDeckBindConcRebarDlg::GetTabCount()
{
	return 4;
}

CString CDeckBindConcRebarDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0)			return "평면";
	else if(nIdx==1)	return "종단면";
	else if(nIdx==2)	return "횡단면";
	else if(nIdx==3)	return "헌치";

	return "";
}

void CDeckBindConcRebarDlg::DrawPlanRebarBindConc(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CAPlateDrawPyung DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarTU DBRebarTU(m_pStd->m_pDataManage);

	// 회전
	CPlateBxFinder Finder(pDB->GetGirder(0));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_JOINT);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_JOINT);

	double rotAng  = DBPlan.GetRotateAnglePyung(pBxStt,pBxEnd);
	BOOL bRotate = CAPlateDrawPyung::IsInputDomRotate();
	
	CDomyun Dom(pDom);
	DBPlan.DrawSlabTotal(&Dom);

	// 거더
	for (long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;

		CPlateBxFinder	Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag);

		while(Finder.GetBxFixNext())
		{
			DBPlan.DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0, 0);	
			pBx = Finder.GetBxNext();
		}

		DBRebarTU.DrawMainRebarTUAtPlan(&Dom, nG);
		DBRebarTU.DrawSabogangRebarTUAtPlan(&Dom, nG);
		DBRebarTU.DrawHunchRebarTUAtPlan(&Dom, nG);
		DBRebarTU.DimMainRebarTUAtPlan(&Dom, nG, TRUE);
		DBRebarTU.DimSabogangRebarTUAtPlan(&Dom, nG, TRUE);

		DBRebarTU.DrawBeryukRebarAtPlan(&Dom, nG);
	}

	if(bRotate)		Dom.Rotate(rotAng, 0, 0);

	*pDom << Dom;
}

void CDeckBindConcRebarDlg::DrawPlanHunchRebarBindConc(CDomyun *pDom)
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CADeckDataTU	 *pDeckDataTU  = m_pStd->m_pDeckDataTU;
	CAPlateDrawPyung DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarTU DBRebarTU(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	CAPlateDrawStandard		DrawStd(m_pStd->m_pDataManage);

	// 회전
	long nG = 0;
	CPlateGirderApp  *pGir	 = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	CPlateBasicIndex *pBx = pGir->GetBxByStation(pBxStt->GetStation()+2000);

	CDomyun Dom(pDom);

	// 시점부
	DBPlan.DrawGirderWithenBx(&Dom, pBxStt, pBx, TRUE, 0, 0);	
	DBRebarTU.DrawMainRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawSabogangRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawHunchRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawBeryukRebarAtPlan(&Dom, nG);

	double dBindW	= pDB->m_BindConc.m_dWidth;
	double dOut		= pDeckDataTU->m_dOutLength;
	CDPoint xyTrim(0,0), xyRot(0,0), vAng(0,0), vX(1,0);	
	xyTrim	= pGir->GetXyGirderDis(pBxStt->GetStation()+2000);
	xyRot	= pGir->GetXyGirderDis(pBxStt->GetStation());
	vAng	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()).Rotate90();//pBx->GetAngle();

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	pDeckDataTU->MakeRebarMarkPosPlanTU(0, Dom.GetScaleDim());
	CRebar *pRB  = pDeckDataTU->GetRebarByName("헌치철근시점_G1");
	CDPointArray xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, "헌치철근시점_G1");
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 6, FALSE, 200, -1, FALSE);

	Dom.TrimByLineOnSide(xyTrim-vAng*dBindW, xyTrim+vAng*dBindW, 1, TRUE);
	Dom.CutLightningAnyDir(xyTrim-vAng*(dBindW/2+dOut), xyTrim+vAng*(dBindW/2+dOut), 0);
	Dom.Rotate(xyRot, vAng);
	
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyRot.x-(dBindW/2+dOut), xyRot.y-Dom.Always(10), 0);
	Dom.DimLineTo(dOut, 0, COMMA(dOut));
	Dom.DimLineTo(dBindW, 0, COMMA(dBindW));
	Dom.DimLineTo(dOut, 0, COMMA(dOut));
	Dom.DimMoveTo(xyRot.x-(dBindW/2+dOut), xyRot.y-Dom.Always(10), 1);
	Dom.DimLineTo(dBindW+dOut*2, 1, COMMA(dBindW+dOut*2));

	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	rectDom.left = rectDom.right-(rectDom.right-xyRot.x)*2;
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_TXTG);
	DrawStd.AddTitleOnDom(&Dom, 100, rectDom, "시점부", TRUE, FALSE, 0);
	Dom.Move(-Dom.GetExtRect().right-Dom.Always(5), 0);
	*pDom << Dom;

	// 종점부
	pBx = pGir->GetBxByStation(pBxEnd->GetStation()-2000);
	DBPlan.DrawGirderWithenBx(&Dom, pBx, pBxEnd, TRUE, 0, 0);	
	DBRebarTU.DrawMainRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawSabogangRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawHunchRebarTUAtPlan(&Dom, nG);
	DBRebarTU.DrawBeryukRebarAtPlan(&Dom, nG);

	xyTrim	= pGir->GetXyGirderDis(pBxEnd->GetStation()-2000);
	xyRot	= pGir->GetXyGirderDis(pBxEnd->GetStation());
	vAng	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()).Rotate90();//pBx->GetAngle();

	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
	pDeckDataTU->MakeRebarMarkPosPlanTU(0, Dom.GetScaleDim());
	pRB  = pDeckDataTU->GetRebarByName("헌치철근종점_G1");
	xyArr= pDeckDataTU->GetArrMarkPosByNameTU(nG, "헌치철근종점_G1");
	DBDeckPlan.DrawRebarMarkLine(&Dom, xyArr);
	if(pRB)	DBDeckPlan.DrawRebarMark(&Dom, xyArr, pRB->m_sMark, pRB->GetStringDia(), 7, FALSE, 200, -1, TRUE);

	Dom.TrimByLineOnSide(xyTrim-vAng*dBindW, xyTrim+vAng*dBindW, -1, TRUE);
	Dom.CutLightningAnyDir(xyTrim-vAng*(dBindW/2+dOut), xyTrim+vAng*(dBindW/2+dOut), 0);
	Dom.Rotate(xyRot, -vAng);
	
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyRot.x-(dBindW/2+dOut), xyRot.y-Dom.Always(10), 0);
	Dom.DimLineTo(dOut, 0, COMMA(dOut));
	Dom.DimLineTo(dBindW, 0, COMMA(dBindW));
	Dom.DimLineTo(dOut, 0, COMMA(dOut));
	Dom.DimMoveTo(xyRot.x-(dBindW/2+dOut), xyRot.y-Dom.Always(10), 1);
	Dom.DimLineTo(dBindW+dOut*2, 1, COMMA(dBindW+dOut*2));

	Dom.SetCalcExtRect();
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_TXTG);
	rectDom = Dom.GetExtRect();
	rectDom.right = rectDom.left+(xyRot.x-rectDom.left)*2;
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_TXTG);
	DrawStd.AddTitleOnDom(&Dom, 100, rectDom, "종점부", TRUE, FALSE, 0);
	Dom.Move(-Dom.GetExtRect().left+Dom.Always(5), 0);
	*pDom << Dom;
}

void CDeckBindConcRebarDlg::DrawJongRebarBindConc(CDomyun *pDom)
{
	CPlateBridgeApp	*pDB		= m_pStd->m_pDataManage->GetBridge();	
	CADeckData		*pDeckData  = m_pStd->m_pDeckData;
	CDrawDBBaseRebarJong	DBDeckJong(m_pStd->m_pDataManage);
	CDimDBDeckBaseJong		DBDeckJongDim(m_pStd->m_pDataManage);
	CAPlateDrawStandard		DrawStd(m_pStd->m_pDataManage);
	CAPlateDrawJong			DrawJong(m_pStd->m_pDataManage);
	CDrawDBBaseRebarTU		DBRebarTU(m_pStd->m_pDataManage);
	
	CDomyun Dom(pDom);

	CString	sText		= _T("");
	BOOL	bEleLevelCur= FALSE;
	
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateBxFinder		Finder(pDB->GetGirder(nG));
		CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex	*pBxEnd	= Finder.GetBxFirst(BX_END_GIRDER);
		
		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
		
		// Ele Level 설정
		bEleLevelCur	= pDB->GetGirder(nG)->GetEleLevel();
		pDB->GetGirder(nG)->SetEleLevel(TRUE);

		DrawJong.DrawPaveLine(&Dom, pBxStt, pBxEnd, 0);
		DrawJong.DrawGirderLineBx(&Dom, pBxStt, pBxEnd);
		DrawJong.DrawInOutGateBx(&Dom, pBxStt, pBxEnd);
		DrawJong.DrawJiJumVStiffBx(&Dom, pBxStt, pBxEnd);
		DBRebarTU.DrawBincConcRebarAtJong(&Dom, nG, TRUE);
		DBRebarTU.DimBincConcRebarAtJong(&Dom, nG, TRUE);
		
		sText.Format("G%d 종단면도", nG+1);
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom, HCAD_TXTG);
		DrawStd.AddTitleOnDom(&Dom, 100, rect, sText, TRUE, FALSE, 0);
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();
		Dom.Move(0, -(rect.Height()+Dom.Always(10))*nG);

		*pDom << Dom;

		// Ele Level 원상태로 되돌림
		pDB->GetGirder(nG)->SetEleLevel(bEleLevelCur);
	}

}

void CDeckBindConcRebarDlg::DrawCrossRebarBindConc(CDomyun *pDom)
{
	CPlateBridgeApp		*pDB			= m_pStd->m_pDataManage->GetBridge();	
	CADeckData			*pDeckData		= m_pStd->m_pDeckData;
	CADeckDataTU		*pDeckDataTU	= m_pStd->m_pDeckDataTU;
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	CPlateBasicIndex	*pBx			= pGir->GetBxOnJijum(0);
	
	CAPlateDrawDanmyun	DrawDanmyun(m_pStd->GetDataManage());
	CAPlateDrawStandard	DrawStd(m_pStd->m_pDataManage);
	CDrawDBBaseRebarTU	DBRebarTU(m_pStd->m_pDataManage);
	
	CString	sText		= _T("");

	CDomyun Dom(pDom);
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);

	// Draw Bind Concrete & Girder
	double dWebT		= pDeckDataTU->GetMaxWebT(0);
	double dFlangeTUp	= pGir->GetValueBasicDefault("FLANGE_T");
	double dFlangeTDn	= pGir->GetValueBasicDefault("FLANGE_T");
	double dBindW		= pDB->m_BindConc.m_dWidth;
	double dBindH		= pDB->m_BindConc.m_dHeight;
	double dBindS1		= pDB->m_BindConc.m_dDeep;
	double dBindS2		= pDB->m_BindConc.m_dDis;
	double dSlabLowH	= -pDB->m_dThickSlabBase;	// 슬래브 하면의 높이
	double dFlangH		= dSlabLowH - (dBindH-dBindS1-dBindS2);
	double dGirderCenter= pGir->m_dGirderCenter;
	double dHeight		= pGir->GetHeightGirderByJijum(0);
	double dUWidth		= pGir->GetValueBasicDefault("FLANGE_W");
	double dLWidth		= pGir->GetValueBasicDefault("FLANGE_W");

	Dom.Rectangle(dGirderCenter-dUWidth/2, dFlangH, dGirderCenter+dUWidth/2, dFlangH-dFlangeTUp);
	Dom.Rectangle(dGirderCenter-dWebT/2,   dFlangH-dFlangeTUp, dGirderCenter + dWebT/2, dFlangH-dFlangeTUp-dHeight);
	Dom.Rectangle(dGirderCenter-dLWidth/2, dFlangH-dFlangeTUp-dHeight, dGirderCenter+dLWidth/2, dFlangH-dFlangeTUp-dFlangeTDn-dHeight);

	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dSlabLowH+dBindS1);
	Dom.LineTo(dGirderCenter-dBindW/2, dSlabLowH+dBindS1, dGirderCenter-dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dSlabLowH+dBindS1, dGirderCenter+dBindW/2, dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter-dBindW/2, dFlangH-dBindS2,	dGirderCenter-dWebT/2,  dFlangH-dBindS2);
	Dom.LineTo(dGirderCenter+dBindW/2, dFlangH-dBindS2,	dGirderCenter+dWebT/2,  dFlangH-dBindS2);

	// Rebar
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);

	DBRebarTU.DrawBincConcRebarAtCross(&Dom, pBx, TRUE);
	DBRebarTU.DimBincConcRebarAtCross(&Dom, 0, TRUE);

	*pDom << Dom;
}

BOOL CDeckBindConcRebarDlg::IsFocusItem(CString szTitle)
{
	if(szTitle=="Aw") return TRUE;
	if(szTitle=="Ah") return TRUE;
	if(szTitle=="Ad") return TRUE;
	if(szTitle=="Hc") return TRUE;
	if(szTitle=="Uc") return TRUE;
	if(szTitle=="Dc") return TRUE;
	if(szTitle=="Sc") return TRUE;
	if(szTitle=="Hd") return TRUE;
	if(szTitle.Find("E")!=-1) return TRUE;

	return CGeneralBaseDlg::IsFocusItem(szTitle);
}

void CDeckBindConcRebarDlg::SetFocusItem(CString szTitle)
{
	if(szTitle.Find("A")==-1 && szTitle.Find("H")==-1 && szTitle.Find("c")==-1 && szTitle.Find("E")==-1)	return;

	m_GridOpt.ResetSelection();

	if(szTitle=="Aw") m_GridOpt.SetFocusCell(3, 1);
	if(szTitle=="Ah") m_GridOpt.SetFocusCell(4, 1);
	if(szTitle=="Ad") m_GridOpt.SetFocusCell(5, 1);	
	if(szTitle=="Uc") m_GridOpt.SetFocusCell(1, 3);
	if(szTitle=="Dc") m_GridOpt.SetFocusCell(2, 3);
	if(szTitle=="Sc") m_GridOpt.SetFocusCell(3, 3);
	if(szTitle=="Hc") m_GridOpt.SetFocusCell(5, 3);
	if(szTitle=="Hd") m_GridOpt.SetFocusCell(3,11);
	if(szTitle.Find("E")!=-1)  m_GridOpt.SetFocusCell(2, 9);

	m_GridOpt.SendMessage(WM_KEYDOWN,VK_F2);
}

void CDeckBindConcRebarDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);
	if(m_nTab!=2) return;

	CString szText = _T("");

	if(nRow==3 && (nCol==0||nCol==1))	szText="Aw";
	if(nRow==4 && (nCol==0||nCol==1))	szText="Ah";
	if(nRow==5 && (nCol==0||nCol==1))	szText="Ad";
	if(nRow==1 && (nCol==2||nCol==3))	szText="Uc";
	if(nRow==2 && (nCol==2||nCol==3))	szText="Dc";
	if(nRow==3 && (nCol==2||nCol==3))	szText="Sc";
	if(nRow==5 && (nCol==2||nCol==3))	szText="Hc";
	if(nRow==3 && (nCol==10||nCol==11))	szText="Hd";
	if(nRow==2 && (nCol==8||nCol==9))
	{
		CPlateBridgeApp	*pDB		= m_pStd->m_pDataManage->GetBridge();
		CADeckDataTU	*pDeckTU	= m_pStd->m_pDataManage->GetDeckDataTU();
		double dDis = (pDB->m_BindConc.m_dWidth-pDeckTU->m_dCoverSd*2)/pDeckTU->m_nQtyDivideJong;
		szText.Format("E@%s", COMMA(dDis));
	}

	m_pView->GetDomyun()->STMakeCursor(szText);
}
