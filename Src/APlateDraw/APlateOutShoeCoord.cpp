// APlateOutShoeCoord.cpp: implementation of the CAPlateOutShoeCoord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MPE_SHOE_COORD  100
#define MPE_SHOE_TABLE  110
#define MPE_SHOE_LIST   130
#define MPE_SHOE_LIST_TITLE	  131
#define MPE_SHOE_LEGEND 140
#define MPE_SHOE_LEGEND_TITLE 141
#define MPE_SHOE_TABLE_TITLE  120

CAPlateOutShoeCoord::CAPlateOutShoeCoord()
{
	DgkBound = CDRect(0,0,0,0);
}

CAPlateOutShoeCoord::~CAPlateOutShoeCoord()
{

}

void CAPlateOutShoeCoord::DrawOnePage()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;
	long nSubPage = nPage - nprePage;

	switch(nSubPage)
	{
		case 1:
			AddPageShoeCoordinate();
			break;
		case 2:
			AddPageElevationCalc();
			break;
	}

	pPageMng->SetProgressData("��¿Ϸ�",100);
}

long CAPlateOutShoeCoord::GetPageSu()
{
	return (m_nPage = 2);
}

CString CAPlateOutShoeCoord::GetTitle()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	long nPage = pStd->m_nCurPage - m_nSttPage;

	CString csTitle = _T("");

	switch(nPage)
	{
		case 1:
			csTitle = "�� �� �� ħ �� ġ ��";
			break;
		case 2:
			csTitle = "ELEVATION ����ٰŵ�";
			break;
	}

	return csTitle;
}

double CAPlateOutShoeCoord::GetScale()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nPage = pStd->m_nCurPage - m_nSttPage;

	double Scale = 0;

	switch(nPage)
	{
		case 1:
			Scale = pStd->m_pDomOptionStd->m_Shoe_dScaleHori;
			break;
		case 2:
			Scale = pOpt->m_Elev_dScaleTable;
			break;
	}

	return Scale;
} 

void CAPlateOutShoeCoord::AddPageShoeCoordinate()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;
	long nSubPage = nPage - nprePage;
	long nUnit = pOpt->m_Shoe_dColumSu;

	CDomyun *pDomP = pStd->GetDomyun(nPage);
	pDomP->SetScaleDim(GetScale());
	CDomyun *pDom  = new CDomyun(pDomP);

	// ����,Ÿ��Ʋ
	pPageMng->SetProgressData("���� �����...",0);
	pStd->GetDogak()->Add(pDom,nPage,GetScale(),pOpt->m_szDGKName,pOpt->m_sHead_DomyunNumber,pOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(pDom, pBridge->m_sNameBridge, GetTitle(), _T(""), FALSE);
	*pDomP << *pDom;
	pDomP->SetCalcExtRect();
	DgkBound = pDomP->GetExtRect();

	// ����ġ��
	if(pOpt->m_Shoe_bShoeCoord)
	{
		pPageMng->SetProgressData("������ħ��ġ�� �����...", 30);
		AddShoeCoordinate(pDom);
		*pDomP << *pDom;
	}	

	//  ����ǥ
	if(pOpt->m_Shoe_bTableCoord && nUnit>0)
	{
		pPageMng->SetProgressData("������ħ ��ǥ �����...", 50);
		CDPoint Base(pDom->Always(30),pDom->Always(250));
		long nTable = (pBridge->m_nQtyJigan+1)/nUnit;
		if((pBridge->m_nQtyJigan+1)%nUnit)
			nTable++;
		if((pBridge->m_nQtyJigan+1)<=nUnit)
			nTable=1;
	
		for(long n=0; n< nTable; n++)
		{
			CDomyun Dom(pDom);
			AddTableShoeCoordinate(&Dom, Base, n);
			Dom.SetCalcExtRect(&Dom);
			CDRect rt = Dom.GetExtRect();
			Base.y = Base.y - rt.Height() - pDom->Always(5);
			*pDom << Dom;
		}

		*pDomP << *pDom;
	}

	// ��List
	if(pOpt->m_Shoe_bTableShoeBM)
	{
		CDPoint Base(pDom->Always(630),pDom->Always(300));
		AddTableShoeList(pDom, Base);
		*pDomP << *pDom;
	}

	// ����
	if(pOpt->m_Shoe_bTableLegend)
	{
		pPageMng->SetProgressData("���� �����...", 80);
		CDPoint Base(pDom->Always(680),pDom->Always(150));
		AddTableShoeLegend(pDom, Base);
		*pDomP << *pDom;
	}

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateOutShoeCoord::AddPageElevationCalc()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();
	
	CDomyun *pDomP = pStd->GetDomyun(nPage);
	pDomP->SetScaleDim(Scale);
	CDomyun *pDom  = new CDomyun(pDomP);

	// ����,Ÿ��Ʋ
	pPageMng->SetProgressData("���� �����...",0);
	pStd->GetDogak()->Add(pDom,nPage,Scale,pOpt->m_szDGKName,pOpt->m_sHead_DomyunNumber,pOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(pDom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);
	*pDomP << *pDom;

	if(pOpt->m_Elev_bTable)
	{
		pPageMng->SetProgressData("ELEVATION ����ٰŵ�",10);
		AddElevationCalcTable(pDom);
		*pDomP << *pDom;
	}

	if(pOpt->m_Elev_bSection)
	{
		pPageMng->SetProgressData("Ⱦ�ܸ鵵 �����....",70);
		AddElevationFrontSection(pDom);
		*pDomP << *pDom;
	}

	if(pOpt->m_Elev_bShoeDetail)
	{
		pPageMng->SetProgressData("������ħ�� �����....",90);
		AddElevationShoeDetail(pDom);
		*pDomP << *pDom;
	}		

	delete pDom;
}

void CAPlateOutShoeCoord::AddElevationCalcTable(CDomyun *pDomP)
{	
 	CAPlateDrawStd	*pStd	 = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir    = pBridge->GetGirder(-1);
	COutMovePoint	*pMove	 = pStd->GetMovePoint(pStd->m_nCurPage);
	
	CAPlateDrawShoeCoord DrawShoeCoord(pStd->GetDataManage());

	CDomyun Dom(pDomP);

	CDPoint Base = CDPoint(0,0);
	// ��� Ÿ��Ʋ ����
	double dRowHeight = Dom.Always(10);	//TEMP
	Base.y -= Dom.Always(20);
	DrawShoeCoord.DrawPierElevationTitle(&Dom, Base, dRowHeight);
	Base.y -= dRowHeight*2;

	// PIER�� TABLE
	for(long nPier=0; nPier<pBridge->m_nQtyJigan+1; nPier++)
	{
		DrawShoeCoord.DrawPierElevationTable(&Dom, nPier, Base, dRowHeight);
		Base.y -= pBridge->GetQtyShoeOnJijum(nPier)*dRowHeight;
	}
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL2);
	Dom.LineTo(rect.left , rect.bottom, rect.right, rect.bottom);
	Dom.LineTo(rect.right, rect.bottom, rect.right, rect.top);
	Dom.LineTo(rect.right, rect.top   , rect.left , rect.top);
	Dom.LineTo(rect.left , rect.top   , rect.left , rect.bottom);

	CDRect DomRect = pStd->GetDogak()->GetRectDraw(GetScale());
	Dom.Move((DomRect.left+DomRect.right)/2 - (rect.left+rect.right)/2,DomRect.bottom-Dom.Always(50));
	rect = Dom.GetExtRect();
	Dom.Move(pMove->GetPoint(10));
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),10,"������ġ ELEVATION ����ٰŵ�");
	*pDomP << Dom;


	CDomyun TDom(pDomP);
	pStd->m_pOptStd->SetEnvType(&TDom,HCAD_SUB_TITLE);
	TDom.TextOut(rect.left+TDom.GetTextWidth("������ġ ELEVATION ����ٰŵ�")/2, rect.bottom+Dom.Always(7),"������ġ ELEVATION ����ٰŵ�");
	TDom.Move(pMove->GetPoint(11));
	TDom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&TDom,TDom.GetExtRect(),11,"������ġ ELEVATION ����ٰŵ� ����");
	*pDomP << TDom;	
}

void CAPlateOutShoeCoord::AddShoeCoordinate(CDomyun *pDomP)
{	
 	CAPlateDrawStd	 *pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp  *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	 *pGir    = pBridge->GetGirder(-1);
	COutMovePoint	 *pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd       *pOpt    = pStd->m_pDomOptionStd;	

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());		

	double HScale = pStd->m_pDomOptionStd->m_Shoe_dScaleHori;
	double VScale = pStd->m_pDomOptionStd->m_Shoe_dScaleVert;

	CPlateBxFinder	FinderSttEnd(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = FinderSttEnd.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER);		

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(HScale);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_CENT);
	DrawPyung.DrawCenterLine(&Dom, -1, pBx->GetStation()-Dom.Always(5), FinderSttEnd.GetBxFixNext()->GetStation()+Dom.Always(5), FALSE, FALSE, TRUE, FALSE);									 
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);	
	DrawPyung.DrawSlabTotal(&Dom);
	DrawPyung.DrawCenterJijum(&Dom,TRUE);               //�����߽ɼ�(���ۺ�)
	DrawPyung.DrawCenterJijum(&Dom,FALSE);				//�����߽ɼ�(����)
	DrawPyung.DrawAbutPierCenterLine(&Dom);             //���뱳���߽ɼ�
	DrawPyung.DrawShoe(&Dom);
	DrawPyung.DrawJiganInformation(&Dom, HScale);		//�Ŵ���������ġ x.y��ǥ	
	for(long g=0; g<pBridge->GetGirdersu(); g++)
		DrawPyung.DrawAbutPierSymbol(&Dom, g, FALSE);			//����, �����ɺ���ũ
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	
	// DIMENSION
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	long nDimDan = 1;
	if(pOpt->m_Shoe_bSttEndXY||pOpt->m_Shoe_bCLSttEndXY)
		nDimDan = 4;

	for(long nJijum=0; nJijum<pBridge->m_nQtyJigan+1; nJijum++)
		DrawDimPyung.DimHDanShoeDist(&Dom,nJijum, nDimDan);

	__int64 nType = 0x0000000000000000;
	if(pOpt->m_Shoe_bSpanCenter)
		nType |= DIM_BASE_SPAN;
	if(pOpt->m_Shoe_bTotalCenter)
		nType |= DIM_BASE_TOTAL;
	pGir = pBridge->GetGirder(-1);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_JOINT);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_JOINT);

	DrawDimPyung.DimPyungTotal(&Dom,pBxStt,pBxEnd,nType,FALSE,FALSE,"",2);
	//DrawDimPyung.DrawDimHDan(&Dom,pBxStt,TRUE);//, FALSE);
	//DrawDimPyung.DrawDimHDan(&Dom,pBxEnd,FALSE);//, FALSE);

	// ���� ����
//	if(pStd->m_pDomOptionStd->m_Shoe_bLineInfo)
//		pStd->GetDimDBBasePlan()->DimTextLineInfo(&Dom, pBxStt, pBxEnd);

	//  ����ȸ��
	pGir   = pBridge->GetGirder(-1);
	pBxStt = pGir->GetBxOnJijum(0);
	pBxEnd = pGir->GetBxOnJijum(pBridge->m_nQtyJigan);

	double rotAng  = DrawPyung.GetRotateAnglePyung(pBxStt,pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	Dom.Move(-MoveXY);
	Dom.Rotate(rotAng,0,0);
	double dY = pBridge->GetGirder(0)->m_dGirderCenter;
	Dom.Move(0, dY);

	Dom.RedrawByHVScale(HScale/VScale,FALSE,FALSE);

	// �̵�
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	double MoveX = (DgkBound.left+DgkBound.right)/2 - (rect.left+rect.right)/2;
	double MoveY = DgkBound.bottom - rect.Height() - Dom.Always(10);
	Dom.Move(MoveX,MoveY);
	Dom.Move(pMove->GetPoint(MPE_SHOE_COORD));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_SHOE_COORD,"������ħ ��ġ��");

	*pDomP << Dom;	
}


void CAPlateOutShoeCoord::AddTableShoeCoordinate(CDomyun *pDomP, CDPoint Base, long nTableSu)
{
 	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pROpt  = pStd->m_pOptStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp *pDB= pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	= pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER;
	long nT = nTableSu;
	long nUnit = pOpt->m_Shoe_dColumSu;
	// Colum, Row �� ����
	long nSttPier = nT*nUnit;
	long nEndPier = (nT+1)*nUnit-1;
	if((pDB->m_nQtyJigan) < (nSttPier+nUnit-1))
		nEndPier = pDB->m_nQtyJigan;
	int RowSu = 0;
	for(long nP=nSttPier ; nP<=nEndPier; nP++)
	{
		int ShoeSu = pDB->GetGirdersu();
		RowSu = max(ShoeSu, RowSu);
	}
	long CoordSu = 3;
	if(pOpt->m_Shoe_bCoordZ)	
		CoordSu++;
	if(pOpt->m_Shoe_bAngle)			
		CoordSu++;

	// ���̺� ����
	long nPier = 0;
	if((pDB->m_nQtyJigan+1)<=nUnit)
		nPier = pDB->m_nQtyJigan+1;
	else
	{
		nPier = nUnit;
		if((nT+1)*nUnit > (pDB->m_nQtyJigan+1))
			nPier = (pDB->m_nQtyJigan+1)%nUnit;
	}
	if(nPier==0)	return;

	// Col, Row �� ����
	long ColSu = nPier*CoordSu+1;
	if(nUnit > pDB->m_nQtyJigan+1)
		ColSu = (pDB->m_nQtyJigan+1)*CoordSu+1;

	CGridDomyunEx Dom(pDomP);
	pROpt->SetEnvType(&Dom, HCAD_TBL1);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(ColSu);
	Dom.SetRowCount(RowSu+2);
	double Height = 8.0;
	double Width = 40.0;
	Dom.SetMargin(3);
	Dom.SetRowHeightAll(Dom.Always(Height));
	Dom.SetColumnWidthAll(Dom.Always(Width));
	Dom.SetColumnWidth(1, Dom.Always(35.0));
	Dom.SetColumnWidth(0, Dom.Always(13));
	Dom.SetMergeRow(0,1,0);
	Dom.SetTextMatrix(0,0,"����", nAlign);

	BOOL bUnitMeter = pStd->GetDataManage()->GetGlobalOption()->GetDesignUnitTonfMeter();
	long nRondPos   = -1;
	for(long n=0; n<=(nEndPier-nSttPier); n++)
	{
		long nCol = nUnit*nT+n;
		CString PierMark = pDB->m_strJijumName[n];
		long nFlag = 1;
		Dom.SetMergeCol(0, 1+n*CoordSu, CoordSu+n*CoordSu);
		Dom.SetTextMatrix(0, 1+n*CoordSu, PierMark, nAlign);
		Dom.SetTextMatrix(1, n*CoordSu+nFlag++, "SHOE", nAlign);
		Dom.SetTextMatrix(1, n*CoordSu+nFlag++, "X", nAlign);
		Dom.SetTextMatrix(1, n*CoordSu+nFlag++, "Y", nAlign);
		if(pOpt->m_Shoe_bCoordZ)
			Dom.SetTextMatrix(1, n*CoordSu+nFlag++, "Z", nAlign);
		if(pOpt->m_Shoe_bAngle)
			Dom.SetTextMatrix(1, n*CoordSu+nFlag++, "Angle", nAlign);

		for(long nRow=0; nRow<pDB->GetGirdersu(); nRow++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nRow);
			CShoe *pShoe = pGir->GetShoe(nCol);
			CString str = _T(""), StrDir  = _T("");
			switch(pShoe->m_cDirectionShoe)
			{
			case 0:
			case 4:
				StrDir = "������";
				str.Format("%d�� %s ", pShoe->m_dForceShoe, StrDir); break;
			case 1:
				StrDir = "�����";
				str.Format("%d�� %s ", pShoe->m_dForceShoe, StrDir); break;
			case 2:
			case 3:
				StrDir = "�Ϲ���";
				str.Format("%d�� %s ", pShoe->m_dForceShoe, StrDir); break;
			}
			long nFlag = 1;
			Dom.SetTextMatrix(nRow+2, n*CoordSu+nFlag++, str, nAlign);
			str.Format("%d", nRow+1); // ����ȣ
			Dom.SetTextMatrixCircle(nRow+2, 0, str, nAlign);

			CDPoint ShoeXY = pDB->GetXyShoe(nRow, nCol);
			// X��ǥ
			str = GetStringOutCoordinate(ShoeXY.y,nRondPos,bUnitMeter);
			Dom.SetTextMatrix(nRow+2, n*CoordSu+nFlag++, str, nAlign);
			// Y��ǥ
			str = GetStringOutCoordinate(ShoeXY.x,nRondPos,bUnitMeter);
			Dom.SetTextMatrix(nRow+2, n*CoordSu+nFlag++, str, nAlign);
			// Z��ǥ
			if(pOpt->m_Shoe_bCoordZ)
			{
				str = GetStringOutElevation(pDB->GetElevationOnBridgeSeatTop(nRow, nCol),nRondPos,bUnitMeter);
				Dom.SetTextMatrix(nRow+2, n*CoordSu+nFlag++, str, nAlign);
			}
			// ������ġ�� ������
			if(pOpt->m_Shoe_bAngle)
			{
				CPlateGirderApp *pGir = pDB->GetGirder(pDB->GetNumberGirderByShoe(nRow,nCol));
				double sta = pDB->GetStationShoeOnGirCenter(nRow,nCol);
				CDPoint vAngBang = pGir->GetLine()->GetAngleAzimuth(sta);
				CDPoint vAngShoe = pDB->GetAngleShoeOnGirCenter(nRow,nCol);
				vAngShoe = vAngBang.Rotate(vAngShoe.RotateInv90());
				str = vAngShoe.GetDegreeString(1, TRUE);
				Dom.SetTextMatrix(nRow+2, n*CoordSu+nFlag++,str, nAlign);
			}
		}
	}

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	Dom.Move(pMove->GetPoint(MPE_SHOE_TABLE+nTableSu));
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	CString str;
	str.Format("������ħ ��ǥ %d", nTableSu+1);
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom, rect, MPE_SHOE_TABLE+nTableSu, str);
	*pDomP << Dom;

	// TITLE
	if(nTableSu==0)
	{
		CDomyun TDom(pDomP);
		CDRect DomBound(0,0,0,0);
		DomBound.left = Base.x;
		DomBound.right = Base.x + rect.Width();
		DomBound.bottom = Base.y;
		DomBound.top = Base.y - rect.Height();
		
		DrawStd.AddTitleOnDom(&TDom, GetScale(), DomBound, "������ħ ��ǥ",TRUE,FALSE,0);			
		TDom.Move(pMove->GetPoint(MPE_SHOE_TABLE_TITLE));
		TDom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&TDom, TDom.GetExtRect(), MPE_SHOE_TABLE_TITLE, "������ħ ��ǥ ���̺� ����");
		*pDomP << TDom;
	}
}

void CAPlateOutShoeCoord::AddTableShoeList(CDomyun *pDomP, CDPoint Base)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt  = pStd->m_pOptStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CString str = _T("");
	CGridDomyunEx Dom(pDomP);
	pOpt->SetEnvType(&Dom, HCAD_TBL1);
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER;
	Dom.SetBasePoint(Base);	
	//Ton �� ���
	CDoubleArray dArrTon;
	for(long nJijum=0; nJijum<pBridge->m_nQtyJigan+1; nJijum++)
	{
		for(long nRow=0; nRow<pBridge->GetQtyShoeOnJijum(nJijum); nRow++)
		{
			double TonValue = pBridge->GetShoeByRowCol(nRow,nJijum)->GetForceShoe();
			long i = 0;
			for(i=0; i<dArrTon.GetSize(); i++)
				if(TonValue==dArrTon.GetAt(i)) break;
			if(i==dArrTon.GetSize())
				dArrTon.Add(TonValue);
		}
	}

	long i = 0;
	// Ton Sort
	for(i=0; i<dArrTon.GetSize(); i++) {
		for(long j=0; j<dArrTon.GetSize(); j++)
		{
			if(dArrTon.GetAt(i)<dArrTon.GetAt(j)) {
				double Temp      = dArrTon.GetAt(i);
				dArrTon.GetAt(i) = dArrTon.GetAt(j);
				dArrTon.GetAt(j) = Temp;
			}
		}
	}

	long nCols = pBridge->m_nQtyJigan +4;
	long nRows = 1+4*dArrTon.GetSize();
	Dom.SetColumnCount(nCols);
	Dom.SetRowCount(nRows);
	double Height = 8.0;
	double Width = 25.0;
	Dom.SetMargin(3);
	Dom.SetRowHeightAll(Dom.Always(Height));
	Dom.SetColumnWidthAll(Dom.Always(Width));

	// ��� title
	Dom.SetMergeCol(0,0,1);
	Dom.SetTextMatrix(0, 0, "��  ��", nAlign);
	Dom.SetTextMatrix(0, pBridge->m_nQtyJigan+3, "��", nAlign);
	for(i=0; i<pBridge->m_nQtyJigan+1; i++)
	{
		CPlateBasicIndex *pBx = pBridge->GetGirder(-1)->GetBxOnJijum(i);
		CString str = pBridge->m_strJijumName[i];
		Dom.SetTextMatrix(0, i+2, str, nAlign);
	}


	for(i=0; i<dArrTon.GetSize(); i++)
	{
		long TotalFix	 = 0;	// ������
		long TotalOneWay = 0;
		long TotalTwoWay = 0;
		Dom.SetMergeRow(1+4*i, 4+4*i, 0);
		str.Format("%d ��", (long)dArrTon.GetAt(i));
		Dom.SetColumnWidth(0, Dom.Always(20));
		Dom.SetTextMatrix(1+4*i,0, str, nAlign);
		Dom.SetTextMatrix(1+4*i, 1, "������", nAlign);
		Dom.SetTextMatrix(2+4*i, 1, "�Ϲ���", nAlign);
		Dom.SetTextMatrix(3+4*i, 1, "�����", nAlign);
		Dom.SetTextMatrix(4+4*i, 1, "��", nAlign);
		long nJijum = 0;
		for(nJijum=0; nJijum<pBridge->m_nQtyJigan+1; nJijum++)
		{
			long Fix=0;
			long OneWay=0;
			long TwoWay=0;
			for(long nRow=0; nRow<pBridge->GetQtyShoeOnJijum(nJijum); nRow++)
			{
				CShoe *pShoe = pBridge->GetShoeByRowCol(nRow,nJijum);
				if(pShoe->m_dForceShoe == dArrTon.GetAt(i))
				{
					switch(pShoe->m_cDirectionShoe)
					{
					case 0:
					case 4:
						Fix++; break;
					case 2:
					case 3:
						OneWay++; break;
					case 1:
						TwoWay++; break;
					}
				}
			}
			str.Format("%d", Fix);
			Dom.SetTextMatrix(1+4*i,nJijum+2, str, nAlign);
			str.Format("%d", OneWay);
			Dom.SetTextMatrix(2+4*i,nJijum+2, str, nAlign);
			str.Format("%d", TwoWay);
			Dom.SetTextMatrix(3+4*i,nJijum+2, str, nAlign);
			str.Format("%d", Fix+OneWay+TwoWay);
			Dom.SetTextMatrix(4+4*i,nJijum+2, str, nAlign);

			TotalFix=TotalFix+Fix;
			TotalOneWay=TotalOneWay+OneWay;
			TotalTwoWay=TotalTwoWay+TwoWay;
		}
		str.Format("%d", TotalFix);
		Dom.SetTextMatrix(1+4*i,nJijum+2,str, nAlign);
		str.Format("%d", TotalOneWay);
		Dom.SetTextMatrix(2+4*i,nJijum+2,str, nAlign);
		str.Format("%d", TotalTwoWay);
		Dom.SetTextMatrix(3+4*i,nJijum+2,str, nAlign);
		str.Format("%d", TotalFix+TotalOneWay+TotalTwoWay);
		Dom.SetTextMatrix(4+4*i,nJijum+2,str, nAlign);
	}
		
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	Dom.Move(pMove->GetPoint(MPE_SHOE_LIST));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_LIST, "������ħ ���� ���̺�");
	*pDomP << Dom;

	CDomyun SDom(pDomP);
	CDRect DomBound(0,0,0,0);
	DomBound.left = Base.x;
	DomBound.bottom = Base.y;
	DomBound.right = Base.x+nCols*Dom.Always(Width);
	DomBound.top = Base.y - nRows*Dom.Always(Height);
	
	DrawStd.AddTitleOnDom(&SDom, GetScale(), DomBound, "������ħ ���� ���̺�", TRUE, FALSE, 0);	
	SDom.Move(pMove->GetPoint(MPE_SHOE_LIST_TITLE));
	SDom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&SDom, SDom.GetExtRect(), MPE_SHOE_LIST_TITLE, "������ħ ���� ���̺� ����");
	*pDomP << SDom;
}


void CAPlateOutShoeCoord::AddTableShoeLegend(CDomyun *pDomP, CDPoint Base)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt  = pStd->m_pOptStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CGridDomyunEx Dom(pDomP);
	pOpt->SetEnvType(&Dom, HCAD_TBL1);

	Dom.SetColumnCount(2);
	Dom.SetRowCount(4);
	Dom.SetColumnWidth(0, Dom.Always(20));
	Dom.SetColumnWidth(1, Dom.Always(30));
	Dom.SetRowHeight(0,Dom.Always(8));
	for(long i=1; i<4; i++)
		Dom.SetRowHeight(i, Dom.Always(15));
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER;
	Dom.SetTextMatrix(0, 0, "����", nAlign);
	Dom.SetTextMatrix(0, 1, "��ȣ", nAlign);
	Dom.SetTextMatrix(1, 1, "������ SHOE"); 
	Dom.SetTextMatrix(3, 1, "����� SHOE");
	Dom.SetTextMatrix(2, 1, "�Ϲ��� SHOE");

	CSymbolDom SymDom(&Dom, pOpt);
	double shoeX = Dom.Always(10);
	SymDom.DrawSymbolShoeMarkPlanGen(shoeX, -Dom.Always(15), 0, 0);
	SymDom.DrawSymbolShoeMarkPlanGen(shoeX, -Dom.Always(30), 2, 0);
	SymDom.DrawSymbolShoeMarkPlanGen(shoeX, -Dom.Always(45.5), 3, 0);
	Dom << SymDom;
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	Dom.Move(Base);
	CDRect DomBound = Dom.GetExtRect();
	Dom.Move(pMove->GetPoint(MPE_SHOE_LEGEND));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_LEGEND, "����");
	*pDomP << Dom;

	CDomyun TDom(pDomP);
	DrawStd.AddTitleOnDom(&TDom, GetScale(), DomBound, "�� ��", TRUE,FALSE,0);	
	TDom.Move(pMove->GetPoint(MPE_SHOE_LEGEND_TITLE));
	TDom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&TDom, TDom.GetExtRect(), MPE_SHOE_LEGEND_TITLE, "���� ����");
	*pDomP << TDom;

}

void CAPlateOutShoeCoord::AddElevationFrontSection(CDomyun *pDomP)
{	
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COutMovePoint *pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd    *pOpt  = pStd->m_pDomOptionStd;
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CAPlateDrawDanmyun Draw(pStd->GetDataManage());		
	double Scale  = pOpt->m_Elev_dScaleSection;
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(Scale);

	// Ⱦ�ܸ鵵
	CPlateBasicIndex *pBx = pBridge->GetGirder(0)->GetBxOnJijum(0);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	Draw.DrawDanmyunOnBx(&Dom, pBx, FALSE, TRUE, CAPlateDrawCross::NOTDRAWVSTIFF);	// ������ �ܸ�
	Draw.DrawGuardWall(&Dom,pBx);
	Draw.DrawPierPlace(&Dom, pBx);

	// ġ����
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	Draw.DimTypicalSectionUpper(&Dom,pBx);
	Draw.DimTextSlopeArrow(&Dom,pBx,NULL,NULL, FALSE);
	if(pBridge->IsTUGir())
		Draw.DimGirderAndSlab(&Dom, pBx);
	Dom.RedrawByScale(pDomP->GetScaleDim()/Scale);

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	CDRect DomRect = pStd->GetDogak()->GetRectDraw(GetScale());
	Dom.Move(-rect.left,-rect.bottom);
	Dom.Move((DomRect.left+DomRect.right)/2 - rect.Width() - pDomP->Always(50), pDomP->Always(200));
	rect = Dom.GetExtRect();
	Dom.Move(pMove->GetPoint(20));
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),20,"Ⱦ�ܸ鵵");

	*pDomP << Dom;
	//Title
	CDomyun TDom(pDomP);
	TDom.SetScaleDim(GetScale());	
	DrawStd.AddTitleOnDom(&TDom, Scale, rect, "Ⱦ �� �� ��", TRUE,TRUE,0);	
	TDom.Move(pMove->GetPoint(21));
	TDom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&TDom,TDom.GetExtRect(),21,"Ⱦ�ܸ鵵 ����");
	*pDomP << TDom;
}

void CAPlateOutShoeCoord::AddElevationShoeDetail(CDomyun *pDomP)
{	
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COutMovePoint *pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd    *pOpt  = pStd->m_pDomOptionStd;
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());

	double Scale  = pOpt->m_Elev_dScaleShoeDetail;
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(Scale);

	// Ⱦ�ܸ鵵
	CPlateBasicIndex *pBx = pBridge->GetGirder(0)->GetBxOnJijum(0);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawSebu.DrawShoeBoxDetail(&Dom,pBx);
	
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	long nJijum = pGir->GetNumberJijumByBx(pBx);
	CShoe *pShoe = pGir->GetShoe(nJijum);

	double T = pGir->GetThickJewon(G_F_L,pBx);
	double H = pShoe->m_dThick_SolePlate
	         + pShoe->m_dThick_Shoe
	         + pShoe->m_dThick_Mortar
	         + pShoe->m_dThick_Block;
	Dom.LineTo(-pShoe->m_dW_SolePlate/2-250, H  ,pShoe->m_dW_SolePlate/2+250,H);
	Dom.LineTo(-pShoe->m_dW_SolePlate/2-250, H+T,pShoe->m_dW_SolePlate/2+250,H+T);
	// CENTER LINE
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_CENT);
	Dom.LineTo(0,-100,0, T+H+Dom.Always(15));

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(-Dom.GetTextHeight(),T+H+Dom.Always(15)," L OF SHOE");
	Dom.TextOut(-Dom.GetTextHeight()+Dom.GetTextHeight()/2,T+H+Dom.Always(15)+Dom.GetTextHeight()/2,"C");

	DimShoeMortarDetail(&Dom, 0, 0);
	Dom.RedrawByScale(pDomP->GetScaleDim()/Scale);

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	Dom.Move(-rect.left,-rect.bottom);
	CDRect DomRect = pStd->GetDogak()->GetRectDraw(GetScale());
	Dom.Move((DomRect.left+DomRect.right)/2 + pDomP->Always(50), pDomP->Always(200));
	rect = Dom.GetExtRect();
	Dom.Move(pMove->GetPoint(30));
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),30,"������ħ��");
	*pDomP << Dom;

	//Title
	CDomyun TDom(pDomP);
	TDom.SetScaleDim(GetScale());
	DrawStd.AddTitleOnDom(&TDom, Scale, rect, "������ħ��", TRUE,TRUE,0);	
	TDom.Move(pMove->GetPoint(31));
	TDom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&TDom,TDom.GetExtRect(),31,"������ħ��");
	*pDomP << TDom;
}

void CAPlateOutShoeCoord::DimShoeMortarDetail(CDomyun *pDomP, long nPier, long nG)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);

	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(nPier);
	long nJijum = pGir->GetNumberJijumByBx(pBx);
	CShoe *pShoe = pGir->GetShoe(nJijum);

	double LowerT = pGir->GetThickJewon(G_F_L,pBx);
	double H1  = pShoe->m_dThick_SolePlate;
	double H2  = pShoe->m_dThick_Shoe;
	double H3  = pShoe->m_dThick_Mortar;
	double H4  = pShoe->m_dThick_Block;

	CDimDomyunEx Dom(pDomP);

	long DimDan = 1;
	double sX = -pShoe->m_dW_SolePlate/2-250;
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(sX,0,DimDan);
	Dom.DimLineTo(sX,H4,"");
	Dom.DimLineTo(sX,H3,"");
	Dom.DimLineTo(sX,H2,"");
	Dom.DimLineTo(sX,H1,"");
	Dom.DimLineTo(sX,LowerT,"");

	// TEXT
	double Exo  = pDomP->GetDimExo();
	double VLen = pDomP->GetDimVLen();

	Dom.SetDirection("TOP");
	Dom.SetTextAlignHorz(TA_LEFT);
	sX -= (Exo+VLen)*Dom.GetScaleDim();
	if(H4>0)
		Dom.LineTo(sX,H4/2         , sX-Dom.Always(45),H4/2);
	if(H3 >0)
		Dom.LineTo(sX,H4+H3/2      , sX-Dom.Always(45),H4+H3/2);
	if(H2 >0)
		Dom.LineTo(sX,H4+H3+H2/2   , sX-Dom.Always(45),H4+H3+H2/2);
	if(H1 >0)
		Dom.LineTo(sX,H4+H3+H2+H1/2, sX-Dom.Always(45),H4+H3+H2+H1/2);
	//
	if(LowerT >0)
	{	Dom.LineTo(sX,H4+H3+H2+H1+LowerT/2, sX-Dom.Always(10),H4+H3+H2+H1+LowerT/2);

		Dom.LineTo(sX-Dom.Always(10),H4+H3+H2+H1+LowerT/2, 
					  sX-Dom.Always(10),H4+H3+H2+H1+LowerT+Dom.Always(5));
		Dom.LineTo(sX-Dom.Always(10),H4+H3+H2+H1+LowerT+Dom.Always(5),
					  sX-Dom.Always(45),H4+H3+H2+H1+LowerT+Dom.Always(5));
	}
	//
	if(H4>0)
		Dom.TextOut(sX-Dom.Always(45),Exo+H4/2+Dom.Always(1)         ,"CON'C");
	if(H3 >0)
		Dom.TextOut(sX-Dom.Always(45),Exo+H4+H3/2+Dom.Always(1)      ,"MORTAR");
	if(H2 >0)
		Dom.TextOut(sX-Dom.Always(45),Exo+H4+H3+H2/2+Dom.Always(1)   ,"SHOE HEIGHT");
	if(H1 >0)
		Dom.TextOut(sX-Dom.Always(45),Exo+H4+H3+H2+H1/2+Dom.Always(1),"SOLE PLATE THK");
	if(LowerT >0)
		Dom.TextOut(sX-Dom.Always(45),Exo+H4+H3+H2+H1+LowerT+Dom.Always(6),"BOTTOM FLG. THK");

	/////////////
	double TH   = Dom.GetTextHeight();
	double dLen = Dom.GetTextWidth("BASIC MORTAR ����");
	sX = pShoe->m_dW_SolePlate/2 + H3/2;
	Dom.LineTo(sX,H4+H3   ,sX+400,H4+H3);
	Dom.LineTo(sX+400,H4+H3       ,sX+400,H4+H3-TH*2);
	Dom.LineTo(sX+400,H4+H3-TH*2  ,sX+400+dLen,H4+H3-TH*2);
	//
	Dom.TextOut(sX+400,H4+H3-TH*1.7 ,"BASIC MORTAR ����");
	//////
	dLen = Dom.GetTextWidth("SOLE PL �ϴ� EL");
	Dom.LineTo(sX,H4+H3+H2  ,sX+400,H4+H3+H2);
	Dom.LineTo(sX+400,H4+H3+H2      ,sX+400,H4+H3+H2+TH*2);
	Dom.LineTo(sX+400,H4+H3+H2+TH*2 ,sX+400+dLen,H4+H3+H2+TH*2);
	//
	Dom.TextOut(sX+400 ,H4+H3+H2+TH*2.4 ,"SOLE PL �ϴ� EL");
	Dom.TextOut(sX+400+dLen,H4+H3+H2+TH*1.6 ,"(SHOE TOP EL)");
	Dom.TextOut(sX+400 ,H4+H3+H2+TH*0.8,"& SHOE ��� EL");

	//////
	Dom.CircleArrow(pShoe->m_dW_SolePlate/4,H4+H3+H2+H1/2,
		               Dom.Always(6),Dom.GetTextWidth("SOLE PL"),2,"SOLE PL","");
	Dom.CircleArrow(-pShoe->m_dW_SolePlate/4,H4+H3+H2/2,
		                H2/2+H1/2+Dom.Always(6),-Dom.GetTextWidth("SHOE"),2,"SHOE","");

	*pDomP << Dom;
}