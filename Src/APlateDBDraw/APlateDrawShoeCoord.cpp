// APlateDrawShoeCoord.cpp: implementation of the CAPlateDrawShoeCoord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDBDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateDrawShoeCoord::CAPlateDrawShoeCoord(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawShoeCoord::~CAPlateDrawShoeCoord()
{

}

void CAPlateDrawShoeCoord::DrawPierElevationTitle(CDomyun *pDom, CDPoint Base, double dRowHeight)
{
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CGlobarOption	*pGlopt		= m_pDataMng->GetGlobalOption();

	CGridDomyunEx Dom;
	Dom.SetScaleDim(pDom->GetScaleDim());
	pOptStd->SetEnvType(&Dom,HCAD_TBL1);

	long	nColSize	= 21;
	if(pGlopt->GetDesignHeigtBaseUpper()==0 && !pDB->IsTUGir())	nColSize++;
	if(pDB->IsTUGir())	nColSize++;

	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(nColSize);
	Dom.SetRowCount(2);
	Dom.SetRowHeightAll(dRowHeight);

	Dom.SetColumnWidthAll(Dom.Always(30));
	Dom.SetColumnWidth(0, Dom.Always(20));
	Dom.SetColumnWidth(1, Dom.Always(20));
	Dom.SetColumnWidth(2, Dom.Always(45));
	Dom.SetColumnWidth(nColSize-2, Dom.Always(35));
	Dom.SetColumnWidth(nColSize-1, Dom.Always(35));
	long nCol = 0;
	//테이블 합치기
	for(nCol=0; nCol<Dom.GetColumnCount(); nCol++)
	{
		if(nCol==6||nCol==7|| (nCol>=nColSize-7 && nCol<=nColSize-3))
			continue;
		Dom.SetMergeRow(0, 1, nCol);
	}
	Dom.SetMergeCol(0,6,7);
	Dom.SetMergeCol(0, nColSize-7, nColSize-3);
	Dom.SetMargin(3);

	nCol = 0;
	//테이블 제목 쓰기
	Dom.SetTextMatrix( 0, nCol, "지점\n위치");				nCol++;
	Dom.SetTextMatrix( 0, nCol, "거더");					nCol++;
	Dom.SetTextMatrix( 0, nCol, "STATION\n(m)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "도로중심\n계획고\n(m)");	nCol++;
	Dom.SetTextMatrix( 0, nCol, "도로중심\n이격거리\n(mm)");nCol++;
	Dom.SetTextMatrix( 0, nCol, "레벨거리\n(m)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "편 경 사");
	Dom.SetTextMatrix( 1, nCol, "편경사(%)");				nCol++;
	Dom.SetTextMatrix( 1, nCol, "높이\n(mm)");				nCol++;
	Dom.SetTextMatrix( 0, nCol, "포장면 EL\n(m)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "포장두께\n(mm)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "슬래브 두께\n(mm)");		nCol++;
	Dom.SetTextMatrix( 0, nCol, "거더 EL\n(m)");			nCol++;
	if(pDB->IsTUGir())
	{
		Dom.SetTextMatrix( 0, nCol, "구속콘크리트\n두께\n(mm)");
		nCol++;
	}
	else if(pGlopt->GetDesignHeigtBaseUpper()==0)
	{
		Dom.SetTextMatrix( 0, nCol, "상판두께\n(mm)");
		nCol++;
	}
	Dom.SetTextMatrix( 0, nCol, "복부판높이\n(mm)");				nCol++;
	Dom.SetTextMatrix( 0, nCol, "하판두께\n(mm)");					nCol++;
	Dom.SetTextMatrix( 0, nCol, "교좌장치점검을 위한 형하공간");
	Dom.SetTextMatrix( 1, nCol, "솔플레이트\n두께");				nCol++;
	Dom.SetTextMatrix( 1, nCol, "SHOE높이");						nCol++;
	Dom.SetTextMatrix( 1, nCol, "MORTAR\n두께");					nCol++;
	Dom.SetTextMatrix( 1, nCol, "BLOCK\n높이");						nCol++;
	Dom.SetTextMatrix( 1, nCol, "계");								nCol++;
	Dom.SetTextMatrix( 0, nCol, "교좌면 EL");						nCol++;
	Dom.SetTextMatrix( 0, nCol, "SHOE EL\n(몰탈상단)");				nCol++;

	Dom.Draw();

	*pDom << Dom;
}

void CAPlateDrawShoeCoord::DrawPierElevationTable(CDomyun *pDom, long nJijum, CDPoint Base, double dRowHeight)
{
	CPlateBridgeApp		*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd		*pOptStd	= m_pDataMng->GetOptionStd();
	CPlateGirderApp		*pGir		= NULL;
	CPlateBasicIndex	*pBx		= NULL;
	CGlobarOption		*pGlopt		= m_pDataMng->GetGlobalOption();

	CGridDomyunEx Dom;
	pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetScaleDim(pDom->GetScaleDim());

	long	nShoe		= pDB->GetQtyShoeOnJijum(nJijum);
	long	nColSize	= 21;
	if(pGlopt->GetDesignHeigtBaseUpper()==0 && !pDB->IsTUGir())	nColSize++;
	if(pDB->IsTUGir())	nColSize++;

	//테이블 환경설정
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(nColSize);
	Dom.SetRowCount(nShoe);
	Dom.SetRowHeightAll(dRowHeight);

	Dom.SetColumnWidthAll(Dom.Always(30));
	Dom.SetColumnWidth(0, Dom.Always(20));
	Dom.SetColumnWidth(1, Dom.Always(20));
	Dom.SetColumnWidth(2, Dom.Always(45));
	Dom.SetColumnWidth(nColSize-2, Dom.Always(35));
	Dom.SetColumnWidth(nColSize-1, Dom.Always(35));

	Dom.SetMergeRow(0,nShoe,0);	//PIER NO 합치기
	Dom.SetMargin(3);

	//피어 마크 얻기
	CString str = pDB->m_strJijumName[nJijum];
	Dom.SetTextMatrixCircle(0,0,str);

	long nG		= 0;
	long nGrid	= GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
	long nPosMM = 1;
	BOOL bUnitMeter  = m_pDataMng->GetGlobalOption()->GetDesignUnitTonfMeter();
	long nRondPos    = -1;
	long nRoundSlope = m_pDataMng->GetGlobalOption()->GetDesignPointSlope();
	for(int nRow=0; nRow<nShoe; nRow++)
	{
		nG   = pDB->GetNumberGirderByShoe(nRow,nJijum);
		pGir = pDB->GetGirder(nG);
		pBx  = pGir->GetBxOnJijum(nJijum);

		long nCol = 1;
		// GIRDER NUMBER
		Dom.SetTextMatrixDouble(nRow,nCol++,"G%.f",nG+1+pDB->GetNumberStartGirder());
		// STATION
		str = pDB->GetStationForStringOutDB(pDB->GetStationShoeOnGirCenter(nRow,nJijum));
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 도로중심 EL
		double	dPlanEL			= (pDB->GetElevationPlanElOnCenLineByShoe(nRow,nJijum));
		double	dLengthLevel	= fabs(pDB->GetDisLevelActOnShoe(nRow,nJijum));
		double	dLengthSlop		= fabs(pDB->GetDisSlopActOnShoe(nRow,nJijum));
		double	dPyungubae		= pDB->GetPyungubeCenLineByShoe(nRow,nJijum);
		double	dPaveThick		= pDB->m_dThickPave;
		double	dTu				= (pGlopt->GetDesignHeigtBaseUpper()==0 ? pGir->GetThickJewon(G_F_U, pBx) : 0);
		double	dElevation		= pGir->GetElevationGirderForReal(pBx, nRow, nJijum);

		str = GetStringOutElevation(dPlanEL, nRondPos, bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 도로중심 이격거리
		str = GetStringOutValue(dLengthLevel+dLengthSlop,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 레벨거리
		str = GetStringOutValue(dLengthLevel,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 편경사
		str = GetStringOutSlope(dPyungubae,nRoundSlope);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 높이 = 편경사 * 거리
		str = GetStringOutValue(dPyungubae*dLengthSlop,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 포장면 EL = 도로중심 EL - 높이
		str = GetStringOutElevation(dPlanEL+dPyungubae*dLengthSlop,nRondPos,bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 포장
		str = GetStringOutValue(dPaveThick,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 슬래브
		str = GetStringOutValue(pGir->GetThickSlab(pBx, 0), nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 거더 ELEVATION
		str = GetStringOutElevation(dElevation, nRondPos, bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 구속콘크리트
		if(pDB->IsTUGir())
		{
			str = GetStringOutValue(pDB->m_BindConc.m_dHeight,nPosMM);
			Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		}
		else if(pGlopt->GetDesignHeigtBaseUpper()==0)
		{
			str = GetStringOutValue(pDB->GetThickFlangeByShoe(nRow,nJijum,G_F_U),nPosMM);
			Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		}
		// 복부판 높이
		str = GetStringOutValue(pGir->GetHeightGirder(pBx, TRUE)-(pDB->IsTUGir() ? pDB->m_BindConc.m_dDis-dTu : 0), nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 하판 두께
		str = GetStringOutValue(pDB->GetThickFlangeByShoe(nRow,nJijum,G_F_L),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 솔플레이트 두께
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickSolePlate(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// SHOE 높이
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickShoe(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// MORTAR 두께
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickMortar(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 블럭 높이
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickBlock(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 계
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetHeightTotalShoe(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 교좌면 EL
		str = GetStringOutElevation(pDB->GetElevationOnBridgeSeatTop(nRow,nJijum),nRondPos,bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// 모르타르 EL
		str = GetStringOutElevation(pDB->GetElevationOnMortarTop(nRow,nJijum),nRondPos,bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol,str,nGrid);
	}

	Dom.Draw();
	*pDom << Dom;
}
