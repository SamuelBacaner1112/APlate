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
	//���̺� ��ġ��
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
	//���̺� ���� ����
	Dom.SetTextMatrix( 0, nCol, "����\n��ġ");				nCol++;
	Dom.SetTextMatrix( 0, nCol, "�Ŵ�");					nCol++;
	Dom.SetTextMatrix( 0, nCol, "STATION\n(m)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "�����߽�\n��ȹ��\n(m)");	nCol++;
	Dom.SetTextMatrix( 0, nCol, "�����߽�\n�̰ݰŸ�\n(mm)");nCol++;
	Dom.SetTextMatrix( 0, nCol, "�����Ÿ�\n(m)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "�� �� ��");
	Dom.SetTextMatrix( 1, nCol, "����(%)");				nCol++;
	Dom.SetTextMatrix( 1, nCol, "����\n(mm)");				nCol++;
	Dom.SetTextMatrix( 0, nCol, "����� EL\n(m)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "����β�\n(mm)");			nCol++;
	Dom.SetTextMatrix( 0, nCol, "������ �β�\n(mm)");		nCol++;
	Dom.SetTextMatrix( 0, nCol, "�Ŵ� EL\n(m)");			nCol++;
	if(pDB->IsTUGir())
	{
		Dom.SetTextMatrix( 0, nCol, "������ũ��Ʈ\n�β�\n(mm)");
		nCol++;
	}
	else if(pGlopt->GetDesignHeigtBaseUpper()==0)
	{
		Dom.SetTextMatrix( 0, nCol, "���ǵβ�\n(mm)");
		nCol++;
	}
	Dom.SetTextMatrix( 0, nCol, "�����ǳ���\n(mm)");				nCol++;
	Dom.SetTextMatrix( 0, nCol, "���ǵβ�\n(mm)");					nCol++;
	Dom.SetTextMatrix( 0, nCol, "������ġ������ ���� ���ϰ���");
	Dom.SetTextMatrix( 1, nCol, "���÷���Ʈ\n�β�");				nCol++;
	Dom.SetTextMatrix( 1, nCol, "SHOE����");						nCol++;
	Dom.SetTextMatrix( 1, nCol, "MORTAR\n�β�");					nCol++;
	Dom.SetTextMatrix( 1, nCol, "BLOCK\n����");						nCol++;
	Dom.SetTextMatrix( 1, nCol, "��");								nCol++;
	Dom.SetTextMatrix( 0, nCol, "���¸� EL");						nCol++;
	Dom.SetTextMatrix( 0, nCol, "SHOE EL\n(��Ż���)");				nCol++;

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

	//���̺� ȯ�漳��
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

	Dom.SetMergeRow(0,nShoe,0);	//PIER NO ��ġ��
	Dom.SetMargin(3);

	//�Ǿ� ��ũ ���
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
		// �����߽� EL
		double	dPlanEL			= (pDB->GetElevationPlanElOnCenLineByShoe(nRow,nJijum));
		double	dLengthLevel	= fabs(pDB->GetDisLevelActOnShoe(nRow,nJijum));
		double	dLengthSlop		= fabs(pDB->GetDisSlopActOnShoe(nRow,nJijum));
		double	dPyungubae		= pDB->GetPyungubeCenLineByShoe(nRow,nJijum);
		double	dPaveThick		= pDB->m_dThickPave;
		double	dTu				= (pGlopt->GetDesignHeigtBaseUpper()==0 ? pGir->GetThickJewon(G_F_U, pBx) : 0);
		double	dElevation		= pGir->GetElevationGirderForReal(pBx, nRow, nJijum);

		str = GetStringOutElevation(dPlanEL, nRondPos, bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// �����߽� �̰ݰŸ�
		str = GetStringOutValue(dLengthLevel+dLengthSlop,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// �����Ÿ�
		str = GetStringOutValue(dLengthLevel,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ����
		str = GetStringOutSlope(dPyungubae,nRoundSlope);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ���� = ���� * �Ÿ�
		str = GetStringOutValue(dPyungubae*dLengthSlop,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ����� EL = �����߽� EL - ����
		str = GetStringOutElevation(dPlanEL+dPyungubae*dLengthSlop,nRondPos,bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ����
		str = GetStringOutValue(dPaveThick,nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ������
		str = GetStringOutValue(pGir->GetThickSlab(pBx, 0), nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// �Ŵ� ELEVATION
		str = GetStringOutElevation(dElevation, nRondPos, bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ������ũ��Ʈ
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
		// ������ ����
		str = GetStringOutValue(pGir->GetHeightGirder(pBx, TRUE)-(pDB->IsTUGir() ? pDB->m_BindConc.m_dDis-dTu : 0), nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ���� �β�
		str = GetStringOutValue(pDB->GetThickFlangeByShoe(nRow,nJijum,G_F_L),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ���÷���Ʈ �β�
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickSolePlate(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// SHOE ����
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickShoe(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// MORTAR �β�
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickMortar(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// �� ����
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetThickBlock(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ��
		str = GetStringOutValue(pDB->GetShoeByRowCol(nRow,nJijum)->GetHeightTotalShoe(),nPosMM);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ���¸� EL
		str = GetStringOutElevation(pDB->GetElevationOnBridgeSeatTop(nRow,nJijum),nRondPos,bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol++,str,nGrid);
		// ��Ÿ�� EL
		str = GetStringOutElevation(pDB->GetElevationOnMortarTop(nRow,nJijum),nRondPos,bUnitMeter);
		Dom.SetTextMatrix(nRow,nCol,str,nGrid);
	}

	Dom.Draw();
	*pDom << Dom;
}
