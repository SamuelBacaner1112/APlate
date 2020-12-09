// APlateOutBMBrInfo.cpp: implementation of the CAPlateOutBMBrInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutBM.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutBMBrInfo::CAPlateOutBMBrInfo(CAPlateOutBMStd *pAPlateOutBMStd)
{
	m_szBridgeName			= _T("");	// �������� �̸�
	m_szBridgePath			= _T("");	// �������� ���
	m_szBridgeDir			= _T("");	// ���� �������
	m_bOutTotSlab			= FALSE;	// ����������Ѱ� ��¿���
	m_bOutTotRebar			= TRUE;		// ö�ټ����Ѱ� ��¿���
	m_bOutRebar				= TRUE;		// ö�� ����ǥ ��¿���
	m_bOutSlab				= TRUE;		// ������ ���� ��¿���
	m_bOutCalcFoundation	= TRUE;		// ����ٰ�
	m_nSpan					= 0;		// 
	m_pAPlateOutBMStd		= pAPlateOutBMStd;
}

CAPlateOutBMBrInfo::~CAPlateOutBMBrInfo()
{

}

void CAPlateOutBMBrInfo::GetSlabHunch(CPlateBasicIndex *pBx, CVectorArray &ptArr)
{
	CPlateBridgeApp	*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pCRGir		= pDB->GetGirder(-1);
	CPlateGirderApp	*pGirL		= pDB->GetGirder(0);	//1�� �Ŵ����� ������
	CPlateGirderApp	*pGirR		= pDB->GetGirder(pDB->GetGirdersu()-1);	//1�� �Ŵ����� ������

	BOOL bIsJijum = pBx->IsJijum();//�ܺ��̸�...

	CDPoint APave[HDANLINESU], ALeft[4], ARigh[4];
	CDPoint xyLineCenter;
	CDPointArray xyArrUpper, xyArrLower;
	pDB->GetXyPaveAct(pBx->GetStation(), APave);	// ���������� ��ǥ
	long n = 0; 
	for (n=0; n<HDANLINESU; n++)
		xyArrUpper.Add(APave[n]);
	
	xyLineCenter.x = APave[0].x + fabs(pDB->GetDisSlabLeftDom(pBx));//�����߽ɿ��� ��������������� �Ÿ�..

	CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pGirL);
	pDB->GetXyFrontCantilever(pBxLeft, ALeft, TRUE);
	CPlateBasicIndex *pBxRigh = pBx->GetBxMatchByCrossBeam(pGirR);
	pDB->GetXyFrontCantilever(pBxRigh, ARigh, FALSE);

	for (n=0; n<2; n++)
		xyArrLower.Add(ALeft[n]);
	for (n=0; n<3; n++)
		xyArrLower.Add(ARigh[n]);	

	CDPoint A[10];
	for(long nG = 0; nG < pDB->GetGirdersu()-1 ; nG++)	
	{
		CPlateGirderApp *pGirCur = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch  = pGirCur->GetBxMatchByDiap(pBx, pGirCur);
		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE);//�Ŵ����̿ɼ� ����...
				
		xyArrLower.Add(A[0]);

		if(bIsJijum)
		{
			xyArrLower.Add(A[3]);
			xyArrLower.Add(A[6]);
		}
		else
		{
			BOOL bIsNull = FALSE;
			for (n=3; n<7; n++)
			{
				bIsNull = A[n].IsNull();
				if (!bIsNull)
					xyArrLower.Add(A[n]);		
			}
		}			
	}

	//�����߽ɼ� ã�Ƽ� �߰�...	
	xyArrLower.RemoveSameValue();	
	xyArrLower.Sort(TRUE, TRUE);

	double  d1 = 0.0;
	double  d2 = 0.0;
	CDPoint xyLower = (LPARAM)0.0;
	for(n=0; n<xyArrLower.GetSize()-1; n++)
	{
		xyLower = xyArrLower[n];
		d1		= xyLower.x;
		xyLower = xyArrLower[n+1];
		d2		= xyLower.x;
		if(xyLineCenter.x == d1 || (xyLineCenter.x > d1 && xyLineCenter.x < d2))
		{
			xyLower = xyArrLower[n];
			xyLineCenter.y = xyLower.y;
			xyArrLower.Add(xyLineCenter);
			break;
		}
		
	}

	xyArrUpper.RemoveSameValue();
	xyArrLower.RemoveSameValue();
	xyArrUpper.Sort(TRUE, TRUE);
	xyArrLower.Sort(TRUE, TRUE);

	xyArrUpper = pDB->m_pDataManage->GetDeckData()->GetCDPointArrByPolyLine(xyArrLower, xyArrUpper, TRUE);

	double dHeight = 0;
	double dThickPave = pDB->GetThickPave();
	for (n=0; n<xyArrUpper.GetSize(); n++)
	{
		xyLower = xyArrLower[n];
		//���� ������ �β��� ���ԵǾ� �����Ƿ� �β�����
		dHeight = ~(xyArrUpper[n]-xyArrLower[n]) - dThickPave;					
		ptArr.Add(CVector(xyLower.x, xyLower.y, dHeight));
	}
}

void CAPlateOutBMBrInfo::DrawSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd	*pOptStd	= m_pAPlateOutBMStd->GetDataManage()->GetOptionStd();
	CVectorArray ptArr;

	GetSlabHunch(pBx, ptArr);
	
	CDomyun Dom(pDom);
	Dom.SetTextHeight(2);

	Dom.SetTextAlignHorz(TA_CENTER);
	CString	sText	= _T("");
	long	n		= 0;

	for(n=0; n<ptArr.GetSize(); n++)
	{						
		Dom.LineTo(ptArr[n].x, ptArr[n].y,ptArr[n].x, ptArr[n].y+ptArr[n].z);		
		sText.Format("H%d", n+1);
		if(n == 0)
			Dom.TextOut(ptArr[n].x-Dom.GetTextWidth(sText), ptArr[n].y+ptArr[n].z/2-Dom.GetTextHeight()/2, sText);
		else
			Dom.TextOut(ptArr[n].x+Dom.GetTextWidth(sText), ptArr[n].y+ptArr[n].z/2-Dom.GetTextHeight()/2, sText);
	}
	*pDom << Dom;

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextHeight(2);
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(ptArr[0].x, ptArr[0].y, 0, "");

	CDPoint xyCurr, xyNext;
	for(n=0; n<ptArr.GetSize()-1; n++)
	{
		xyCurr	= ptArr[n];
		xyNext	= ptArr[n+1];
		double	dDis	= fabs(xyNext.x-xyCurr.x); 
		sText.Format("L%d", n+1);
		Dom.DimLineTo(dDis, 0, sText);
	}	

	*pDom << Dom;
}

void CAPlateOutBMBrInfo::DrawMainDomyun(CDomyun *pDom, long nSectionPos)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateBasicIndex	*pBx		= GetBxSection(nSectionPos);
	CPlateGirderApp		*pGir		= pDB->GetGirder(pBx->GetNumberGirder());

	CAPlateDrawDanmyun DrawDanmyun(m_pAPlateOutBMStd->GetDataManage());
	CDomyun Dom(pDom);

	
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, FALSE, CAPlateDrawCross::UPPER);	// ������ �ܸ�
	
	DrawSlabHunch(&Dom, pBx);

	Dom.SetCalcExtRect();
	CDRect	rect;
	rect = Dom.GetExtRect();

	double dTrimHeight	= pGir->GetHeightGirderBySta(pBx->GetStation())*1/10;//pGir->GetHeightBox(pBx->GetStation())*4/5;

 	Dom.CutLightning(rect.left-Dom.Always(2), rect.top+dTrimHeight, rect.right+Dom.Always(2), rect.top+dTrimHeight, FALSE);

	*pDom << Dom;
}

void CAPlateOutBMBrInfo::DrawCrossRebarBindConc(CDomyun *pDom)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CADeckData			*pDeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CDataManage			*pDataMng	= m_pAPlateOutBMStd->GetDataManage();
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);

	CAPlateDrawDanmyun	DrawDanmyun(pDataMng);
	CAPlateDrawStandard	DrawStd(pDataMng);
	CDrawDBBaseRebarTU	DBRebarTU(pDataMng);
	
	BOOL	bShowMark	= FALSE;//pDeckData->m_bShowMark && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag() ? TRUE : FALSE;
	CString	sText		= _T("");

	CDomyun Dom(pDom);
	pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);

	// Draw Bind Concrete & Girder
	double dWebT		= pGir->GetValueBasicDefault("WEB_T");
	double dFlangeTUp	= pGir->GetValueBasicDefault("FLANGE_T");
	double dFlangeTDn	= pGir->GetValueBasicDefault("FLANGE_T");
	double dBindW		= pDB->m_BindConc.m_dWidth;
	double dBindH		= pDB->m_BindConc.m_dHeight;
	double dBindS1		= pDB->m_BindConc.m_dDeep;
	double dBindS2		= pDB->m_BindConc.m_dDis;
	double dSlabLowH	= -pDB->m_dThickSlabBase;	// ������ �ϸ��� ����
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
	pDataMng->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);

	DBRebarTU.DrawBincConcRebarAtCross(&Dom, pGir->GetBxOnJijum(0), TRUE);
	DBRebarTU.DimBincConcRebarAtCross(&Dom, 0, TRUE);

	*pDom << Dom;
}

CPlateBasicIndex *CAPlateOutBMBrInfo::GetBxSection(long nSectionPos)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateBasicIndex *pBx	= NULL;
	
	if(nSectionPos==SECTION_DANBU_STT)		// �������� ������ġ
	{
		return pDB->GetGirder(-1)->GetBxOnJijum(0);		
	}
	else if(nSectionPos==SECTION_DANBU_END)		// �������� ������ġ
	{
		return pDB->GetGirder(-1)->GetBxOnJijum(pDB->m_nQtyJigan);		
	}
	else if(nSectionPos==SECTION_GEN)//�Ϲݺ� �����ۺκ� �ٷ� ���� bxã��..
	{
		CPlateGirderApp *pGir1= pDB->GetGirder(0);
		CPlateBxFinder Finder(pGir1);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		if(pDB->m_dHeightTaperStt>0)
		{
			while (pBx) 
			{				
				//double dHeight = pGir1->GetHeightTaper(pBx->GetStation());
				if(pGir1->IsExistTaperArea(pBx, 0))//�����ۺο� ���ϸ�..				
					pBx = Finder.GetBxNext();
				else	break;				
			}
		}
		else
		{
			pBx	= Finder.GetBxNext(2);
		}

		return pBx;
	}
	if(nSectionPos==SECTION_JIJUM && pDB->m_nQtyJigan>1) 
		return pDB->GetGirder(-1)->GetBxOnJijum(1);

	ASSERT(FALSE);
	return NULL;
}

void CAPlateOutBMBrInfo::DrawSlabHunchGen(CDomyun *pDom)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CARoadOptionStd		*pOptStd	= m_pAPlateOutBMStd->GetDataManage()->GetOptionStd();	
	CAPlateDrawPyung	DBPlan(m_pAPlateOutBMStd->GetDataManage());
	CPlateBasicIndex	*pBxStt		= NULL;
	CPlateBasicIndex	*pBxEnd		= NULL;
	CPlateBasicIndex	*pBxLeftStt	= NULL;
	CPlateBasicIndex	*pBxLeftEnd	= NULL;
	CPlateBasicIndex	*pBxRighStt	= NULL;
	CPlateBasicIndex	*pBxRighEnd	= NULL;
	CPlateGirderApp		*pGir		= NULL;

	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	long nG = 0;
	CPlateBxFinder	FinderLeft(pDB->GetGirder(nG));
	pBxLeftStt = FinderLeft.GetBxFirst(BX_STT_GIRDER);
	pBxLeftEnd = FinderLeft.GetBxFirst(BX_END_GIRDER);

	nG = pDB->GetGirdersu()-1;
	CPlateBxFinder	FinderRigh(pDB->GetGirder(nG));
	pBxRighStt = FinderRigh.GetBxFirst(BX_STT_GIRDER);
	pBxRighEnd = FinderRigh.GetBxFirst(BX_END_GIRDER);

	DBPlan.DrawSlabTotal(&Dom);
	DBPlan.DrawTaperLinePyung(&Dom);		// Taper �� �׸���
	DBPlan.DrawCRBeamCLTotalGirder(&Dom);	//�߽ɼ����׸�

	pGir	= pDB->GetGirder(0);
	CPlateBxFinder Finder(pGir);
	pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	double rotAng  = DBPlan.GetRotateAnglePyung(pBxStt,pBxEnd);		
	Dom.Rotate(rotAng,0,0);

	*pDom << Dom;
}

// ������ Ⱦ�ܸ鵵 �׸���
void CAPlateOutBMBrInfo::DrawSlabFrontGen(CDomyun *pDom)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CARoadOptionStd		*pOptStd	= m_pAPlateOutBMStd->GetDataManage()->GetOptionStd();

	CAPlateDrawDanmyun DrawDanmyun(m_pAPlateOutBMStd->GetDataManage());
	CPlateBasicIndex	*pBx		= GetBxSection(SECTION_GEN);
	CDomyun	Dom(pDom);
	
	// �ܸ�׸�
	DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, FALSE);
	DrawDanmyun.DrawGuardWall     (&Dom, pBx);
	DrawDanmyun.DrawPierPlace     (&Dom, pBx, FALSE);
	
	// Ⱦ�ܸ鵵 ġ����
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	DrawDanmyun.DimSlabUpper(&Dom, pBx, FALSE);

	*pDom << Dom;
}

// �Ŵ� ���� �ٸ���� �ϴ� 2���� ���� �����Ѵ�.....
void CAPlateOutBMBrInfo::GetGirderWidth(long &nGirdersu1, double &dGirderWidth1, long &nGirdersu2, double &dGirderWidth2)
{

}

// ĵƿ������ ��
double CAPlateOutBMBrInfo::GetCantileverWidth(BOOL bLeft)
{
// 	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
// 	long nG = 0;
// 	if(bLeft)	nG = 0;
// 	else		nG = pDB->GetGirdersu()-1;
// 	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
// 	CPlateBxFinder	Finder(pGir);
// 	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
// 
// 	double	dWidth	= pDB->GetWidSlabUpFlangeWeb(pBx, bLeft);
// 
// 	return dWidth;

	return 0;
}

//���κ� ��ġ ��� ���� �ִ�/�ּ� ����...070315...KB...
//1. ���κ���ġ������� ������ ���κ�
//2. ��/���� ��ġ�� Station���̿� ��ġ�� ���κ�
//3. �����극�̽� ����
//4. ��������Է¿��� W,W1 ������ ����...
//5. ��ġ�� ������ ���� ������...
void CAPlateOutBMBrInfo::GetCrossBeamHunch(double &dMaxLength, double &dMinLength, long &nQtyHunch, 
										double &dMinHeight, double &dMaxHeight, double &dLowWidth)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CDPoint A[10]; // �Ŵ��� ����Ʈ
	double dHunchLength = 0;
	double dHunchMaxHeight = 0;
	double dHunchMinHeight = 0;
	double dHunchLowWidth = 0;

	dMaxLength = 0;
	dMinLength = 1000000.0;
	dMaxHeight = 0;
	dMinHeight = 1000000.0;
	nQtyHunch  = 0;
	dLowWidth  = 0;

	//��/���� ��ġ�� Station���̿� ��ġ�� ���κ�

	double dHunchStt = 0;
 	double dHunchEnd = 0;
		
	for(long nG = 0; nG < pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		//CPlateBxFinder		FinderStt(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		//CPlateBasicIndex	*pBxStt	= FinderStt.GetBxFirst(BX_STT_SLAB);
		pBx	= Finder.GetBxNext();

		dHunchStt = pGir->GetSSS(OFF_STT_HUNCHEND);// 
		dHunchEnd = pGir->GetSSS(OFF_END_HUNCHEND);// 
	
		while(pBx)
		{			
			//���κ� Ÿ�Ը�...�극�̽�Ÿ�� ���ܵǰ� ����..
			CPlateCrossBeam		*pC	= pBx->GetCrossBeam();
			if(!pC)//Null�̸� 
			{
				pBx = Finder.GetBxNext();
				continue;
			}			

			//���κ���ġ������� ������ ���κ�
			if(!pC->m_bHunchTopConnect) 
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			//��/���� ��ġ�� Station���̿� ��ġ�� ���κ�
			

			double dSta = pBx->GetStation();

			if(dSta <= dHunchStt || dSta >= dHunchEnd)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			pDB->GetGirder(nG)->GetHunchXyDom(pBx,A);

			dHunchLength = A[6].x - A[3].x;
			dHunchMaxHeight = max(A[4].y - A[3].y, A[5].y - A[6].y);
			dHunchMinHeight = min(A[4].y - A[3].y, A[5].y - A[6].y);

			dHunchLowWidth += A[3].x - A[0].x;

			if(dHunchLength > dMaxLength)
				dMaxLength = dHunchLength;

			if(dHunchLength < dMinLength)
				dMinLength = dHunchLength;

			if(dHunchMaxHeight > dMaxHeight)
				dMaxHeight = dHunchMaxHeight;

			if(dHunchMinHeight < dMinHeight)
				dMinHeight = dHunchMinHeight;

			nQtyHunch++;

			pBx = Finder.GetBxNext();			
		}
	}

	if (nQtyHunch ==0)
	{
		dMinLength = 0;
		dMaxLength = 0;
		dMaxHeight = 0;
		dMinHeight = 0;
		dLowWidth  = 0;
	}
	else
		dLowWidth = dHunchLowWidth/nQtyHunch;//��հ��� ���ϱ� ����...	
}

double CAPlateOutBMBrInfo::GetGirderHeight(BOOL bMax)
{		
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();	

	// ������ �Է��Ѱ��� �̿���
	double	dHeight	= bMax ? 0 : 9999999.9;
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{	
		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		for(long nJ=0; nJ<pDB->m_nQtyJigan+1; nJ++)
		{		
			if(bMax)	dHeight = max(dHeight, pGir->GetHeightGirderByJijum(nJ,FALSE));
			else		dHeight = min(dHeight, pGir->GetHeightGirderByJijum(nJ,FALSE));			
		}
	}

	return  dHeight;
}

// x : ���� y : ����
void CAPlateOutBMBrInfo::GetSlabVerticalDimensions(CVectorArray &ptArr)
{
	CADeckData		*pDeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CPlateBridgeApp *pDB		= m_pAPlateOutBMStd->GetBridgeApp();

	CDPoint xyDim;

	xyDim.x = pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABLEFT);
	xyDim.y = pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABRIGHT);
	ptArr.Add(xyDim);

	xyDim.x = pDB->m_dHeightTaperStt*5;
	xyDim.y = pDB->m_dHeightTaperStt*5;
	ptArr.Add(xyDim);

	double dLengthCentL = pDeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT) - pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABLEFT) - pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABLEFT) - pDB->m_dHeightTaperStt*5 - pDB->m_dHeightTaperEnd*5;
	double dLengthCentR = pDeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT) - pDeckData->GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABRIGHT) - pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABRIGHT) - pDB->m_dHeightTaperStt*5 - pDB->m_dHeightTaperEnd*5;

	xyDim.x = dLengthCentL;
	xyDim.y = dLengthCentR;
	ptArr.Add(xyDim);

	xyDim.x = pDB->m_dHeightTaperEnd*5;
	xyDim.y = pDB->m_dHeightTaperEnd*5;
	ptArr.Add(xyDim);

	xyDim.x = pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABLEFT);
	xyDim.y = pDeckData->GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABRIGHT);
	ptArr.Add(xyDim);

}


void CAPlateOutBMBrInfo::DrawSlabDongbari(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CDataManage			*pDataMng	= m_pAPlateOutBMStd->GetDataManage();
	CARoadOptionStd		*pOptStd	= pDataMng->GetOptionStd();
	CADeckData			*pDeckData	= pDataMng->GetDeckData();
	CBMOption			*pBMOpt		= &pDeckData->m_BMOption;
	
	CAPlateDrawDanmyun DrawDanmyun(pDataMng);	

	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);
	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetTextHeight(2);

	DrawDanmyun.DrawGuardWall(&Dom, pBx);	// ��ȣ��
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, TRUE, TRUE, CAPlateDrawCross::NOTDRAWVSTIFF);	// ������ �ܸ�
	DrawDanmyun.DimSlabCenterLine(&Dom, pBx, FALSE);
	//DrawDanmyun.DrawSlabDosang(&Dom,pBx);	
	
	long dFlangeTUpBase = pDataMng->GetGlobalOption()->GetDesignHeigtBaseUpper();		
	long nGirderCnt		= pDB->GetGirdersu();
	long nG				= 0;
	CDPointArray xyArrHatchCent, xyArrHatchLeft, xyArrHatchRigh;
	CDPointArray xyArrUpper, xyArrLower;
	double dHatchW    = 100;
	double dWFinisher = pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth;
	double dUpperT		= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	double dLowerT		= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double dBindH		= pDB->m_BindConc.m_dHeight;
	double dBindW		= pDB->m_BindConc.m_dWidth;
	
	//���Ŵ� ���� ����Ʈ ã��..	
	CDPoint A[10];	
	CDPoint aLeft, aRigh;
	
 	for (nG=0; nG<nGirderCnt; nG++)
	{
		CPlateGirderApp *pGirCur = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch  = pGirCur->GetBxMatchByDiap(pBx, pGirCur);
		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE, FALSE, pDB->IsTUGir());//�Ŵ����̿ɼ� ����...
		
		double dWFlangeUp	= pGirCur->GetWidthFlangeUpper(pBx);//����÷�������..
		double dWFlangeLo	= pGirCur->GetWidthFlangeLower(pBx);//�Ϻ��÷�������..
		double dHeight		= pGirCur->GetHeightGirderByStaAng(pBx->GetStation());//�������� ����...			
		double dWebT		= pGirCur->GetThickFactByStation(pBx->GetStation(), G_W);//������ �β�...
		double dGapUp		= (dWFlangeUp-dWebT)/2;
		double dGapLo		= (dWFlangeLo-dWebT)/2;

		if(dFlangeTUpBase ==1) dUpperT = 0;

if(pDB->IsTUGir())
		{
			dGapUp = (dBindW-dWebT)/2;
			aLeft = CDPoint(A[1].x+dGapUp, A[2].y-dBindH);
			aRigh = CDPoint(A[2].x-dGapUp, A[2].y-dBindH);
		}
		else
		{
			aLeft = CDPoint(A[1].x+dGapUp, A[1].y-dUpperT);
			aRigh = CDPoint(A[2].x-dGapUp, A[2].y-dUpperT);
		}

		xyArrUpper.Add(A[0]);
		if(pDB->IsTUGir())
		{			
			xyArrUpper.Add(CDPoint(A[1].x, A[2].y-dBindH));
			xyArrUpper.Add(aLeft);
			xyArrUpper.Add(aLeft);
		}
		else
		{
			xyArrUpper.Add(A[1]);
			xyArrUpper.Add(CDPoint(A[1].x, aLeft.y));
			xyArrUpper.Add(aLeft);
		}

		xyArrUpper.Add(aRigh);
		if(pDB->IsTUGir())
		{
			xyArrUpper.Add(CDPoint(A[2].x, aRigh.y));
			xyArrUpper.Add(CDPoint(A[2].x, aRigh.y));
			xyArrUpper.Add(A[3]);
		}
		else
		{
			xyArrUpper.Add(CDPoint(A[2].x, aRigh.y));
			xyArrUpper.Add(A[2]);
			xyArrUpper.Add(A[3]);
		}		

		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE);
		aLeft.y = A[1].y-dUpperT;
		aRigh.y = A[2].y-dUpperT;
		//�Ϻ� �÷��� �¿� ����Ʈ...
		xyArrLower.Add(CDPoint(aLeft.x-dGapLo, aLeft.y-dHeight));
		xyArrLower.Add(CDPoint(aLeft.x, aLeft.y-dHeight));
		xyArrLower.Add(CDPoint(aRigh.x, aLeft.y-dHeight));
		xyArrLower.Add(CDPoint(aRigh.x+dGapLo, aLeft.y-dHeight));
	}

	long nCntUp = 0;
	long nCntLo = 0;
	int i = 0;
	for (nG=0; nG<nGirderCnt-1; nG++)
	{
		xyArrHatchCent.RemoveAll();

		nCntUp = nG*8+4;
		nCntLo = nG*4+2+3;
								
		for (i=0; i<8; i++)
			xyArrHatchCent.Add(xyArrUpper[nCntUp++]);

		for (i=0; i<4; i++)
			xyArrHatchCent.Add(xyArrLower[nCntLo--]);

		//xyArrHatchCent.RemoveSameValue();//���⼭�� ���� ���ʿ� ����....
		
		nCntUp = nG*8+4;
		nCntLo = nG*4+2+3;

		xyArrHatchCent.Add(xyArrUpper[nCntUp]);//�ٰ��� ù������ ���ƾ���...
		
		Dom.SetDirection("TOP");
		Dom.HatchingByCDPointArray(xyArrHatchCent, 45, dHatchW, TRUE);
		
		//Dom.LineTo(xyArrLower[nCntLo+1], xyArrLower[nCntLo+2]);

		CDPoint xyGisi = (xyArrLower[nCntLo]+xyArrLower[nCntLo-2])/2;
 		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y+200, Dom.Always(10), Dom.Always(20), 7, "���ٸ�(����)");
	}
	
	//��/���� ������ ���ٸ�...

	// ��ũ�Ǵϼ���ġ		0 : ������  1 : ��������   2 : ��������   3 : ��������
	long nDFPos = pBMOpt->m_structTimber.m_nPosDeckFinisher;	
	CDPoint APave[HDANLINESU], ALeft[4], ARigh[4];
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyLeftIn, xyLeftOut;//  = A[9];
	CDPoint xyRighIn, xyRighOut;// = A[9] - vX*dWFinisher;

	CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(0));
	pDB->GetXyFrontCantilever(pBxLeft, ALeft, TRUE);
	CPlateBasicIndex *pBxRigh = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(nGirderCnt-1));
	pDB->GetXyFrontCantilever(pBxRigh, ARigh, FALSE);

	xyArrHatchLeft.RemoveAll();
	xyArrHatchRigh.RemoveAll();
	
	//��������ġ ����/������� ���� ��ǥ �ϳ� �� ��...
	//����
	for (i=0; i<3; i++)
		xyArrHatchLeft.Add(ALeft[i]);
	for(i=1; i<4; i++)
		xyArrHatchLeft.Add(xyArrUpper[i]);
	for(i=1; i>-1; i--)
		xyArrHatchLeft.Add(xyArrLower[i]);

	xyArrHatchLeft.Add(xyArrLower[0] - vY*dLowerT);//CDPoint(xyArrLower[0].x, xyArrLower[0].y - dLowerT));

	xyLeftIn = ALeft[0];
	xyLeftOut = ALeft[0] - vX*dWFinisher;

	//�ؿ��� ����ؾ� �ϹǷ�...���� ���...
	xyRighIn = ARigh[0];
	xyRighOut = ARigh[0] + vX*dWFinisher;
	
	//����..��ũ�Ǵϼ�����..
	if(nDFPos ==1 || nDFPos ==3)//���������̳� ���������̸�...
	{
		xyArrHatchLeft.Add(xyLeftOut);//�ٰ��� ù������ ���ƾ���...

		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyLeftOut.x, xyLeftOut.y, 0);
		Dom.DimLineTo(dWFinisher, 0, COMMA(dWFinisher));

		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyRighIn.x+dWFinisher, xyArrLower[xyArrLower.GetUpperBound()].y-dLowerT, 0);
		CString sText;
		sText.Format("%s ~ %s", COMMA(xyRighIn.y-xyArrLower[xyArrLower.GetUpperBound()].y-pDB->m_dHeightTaperStt), 
			COMMA(xyRighIn.y-xyArrLower[xyArrLower.GetUpperBound()].y));
		Dom.DimLineTo(xyRighIn.y-xyArrLower[xyArrLower.GetUpperBound()].y, 0, sText);

		Dom.SetDirection("TOP");
 		CDPoint xyGisi = xyArrLower[0];
 		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y+200, Dom.Always(10), -Dom.Always(20), 5, "��ũ�Ǵϼ�");
	}
	
	xyArrHatchLeft.RemoveSameValue();//
	xyArrHatchLeft.Add(xyLeftIn);//�ٰ��� ù������ ���ƾ���...	
		
	Dom.SetDirection("TOP");
	Dom.HatchingByCDPointArray(xyArrHatchLeft, 45, dHatchW, TRUE);
	
	//��������ġ ����/������� ���� ��ǥ �ϳ� �� ��...
	//������...������ �ϺΰŴ����� ����...�ߴ��� ������ �ȸ³�..��...������ �ǳ����� ����..�� �ȵǳ�..��...�Ŵ� ������ ����������...	

	nCntLo = xyArrLower.GetUpperBound();//(nGirderCnt-1)*4+7;//�Ϻ� �Ŵ� �� ������ �迭...
	nCntUp = (nGirderCnt-1)*8+4;;//���..������...
	
	for (i=nCntUp; i<nCntUp+3; i++)
		xyArrHatchRigh.Add(xyArrUpper[i]);
	for(i=2; i>-1; i--)
		xyArrHatchRigh.Add(ARigh[i]);	

	//������..��ũ�Ǵϼ�����..
	if(nDFPos ==2 || nDFPos ==3)//������ �����̳� ���������̸�...
	{
		xyArrHatchRigh.Add(xyRighOut);//�ٰ��� ù������ ���ƾ���...

		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyRighIn.x, xyRighIn.y, 0);
		Dom.DimLineTo(dWFinisher, 0, COMMA(dWFinisher));

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(xyLeftIn.x-dWFinisher, xyArrLower[xyArrLower.GetUpperBound()].y-dLowerT, 0);
		CString sText;
		sText.Format("%s ~ %s", COMMA(xyLeftIn.y-xyArrLower[xyArrLower.GetUpperBound()].y-pDB->m_dHeightTaperStt), 
			COMMA(xyLeftIn.y-xyArrLower[xyArrLower.GetUpperBound()].y));
		Dom.DimLineTo(xyLeftIn.y-xyArrLower[xyArrLower.GetUpperBound()].y, 0, sText);

		Dom.SetDirection("TOP");
 		CDPoint xyGisi = xyArrLower[xyArrLower.GetUpperBound()];
 		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y+200, Dom.Always(10), Dom.Always(20), 7, "��ũ�Ǵϼ�");
	}
		
	CDPoint xyLastPt = xyArrLower[nCntLo] - vY*dLowerT;//CDPoint(xyArrLower[nCntLo].x, xyArrLower[nCntLo].y - dLowerT);
	xyArrHatchRigh.Add(xyLastPt);
	xyArrHatchRigh.Add(xyArrLower[nCntLo]);
	xyArrHatchRigh.Add(xyArrLower[nCntLo-1]);
	
	xyArrHatchRigh.RemoveSameValue();//
	xyArrHatchRigh.Add(xyArrUpper[nCntUp]);//�ٰ��� ù������ ���ƾ���...
			
	Dom.SetDirection("TOP");
	Dom.HatchingByCDPointArray(xyArrHatchRigh, 45, dHatchW, TRUE);

	*pDom << Dom;

	if(!pDB->IsTUGir())	return;	
	for(nG=0; nG<nGirderCnt; nG++)
	{
		CPlateGirderApp *pGirCur = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch  = pGirCur->GetBxMatchByDiap(pBx, pGirCur);
		pGirCur->GetHunchXyDom(pBxMatch, A, FALSE, FALSE, TRUE);

		double dDeep = pDB->m_BindConc.m_dDeep;
		CDRect rectTrim(A[2].x-dBindH, A[2].y-dBindH+0.0001, A[2].x-0.0001, A[2].y-dDeep);
		pDom->TrimByRect(rectTrim);
	}
 		
// 	CHgBaseDrawStd baseDraw;
// 	CStringArray sArrText;
// 	CStringArray sArrUnderText;
// 	CString strUp, strDn, sDir = _T("BOTTOM");
// 	CDoubleArray dArrDimSum, dArrDim;
// 	dArrDimSum.Add(xyArrLowerLeft[0].x);
// 	dArrDimSum.Add(xyArrLowerRigh[xyArrLowerRigh.GetUpperBound()].x);
// 	for ( nG=0; nG<pDB->GetGirdersu(); nG++)
// 	{
// 		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
// 
// 		dArrDimSum.Add(xyArrLowerLeft[nG].x + (pGir->m_dLeftSharp-dArrWebTL[nG]));
// 		dArrDimSum.Add(xyArrLowerRigh[nG].x - (pGir->m_dRighSharp-dArrWebTR[nG]));
// 	}
// 	dArrDimSum.Sort();
// 
// 	for (long n=0; n<dArrDimSum.GetSize()-1; n++)
// 		dArrDim.Add(dArrDimSum[n+1]-dArrDimSum[n]);
// 
// 	baseDraw.AutoDimCont(&Dom, xyArrLowerLeft[0], dArrDim, sArrText, sArrUnderText, 0, sDir, -1, FALSE, FALSE, FALSE);
// 
}


double CAPlateOutBMBrInfo::GetBridgeLength(long nGuardFence)
{	
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);	
	CADeckData	*pADeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CFemStressCalc  StressCalc(m_pAPlateOutBMStd->GetDataManage(), NULL);
	
	double	dResult		= 0;
	long	nQtyHDan	= pDB->GetQtyHDanDom(pBx);
	long	nSumGuard	= 0;
	long	nHDan		= 0;

	if(nGuardFence!=0)
	{
		for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			long nType = pDB->GetValueTypeHDan(nHDan, 1);
			if(nType == 1 || nType == 2)	 // ��ȣ��, �ߺд�
				nSumGuard++;
			if(nSumGuard == nGuardFence)
				break;
		}
	}

	double	dDistGuard		= pDB->GetLengthHDanDom(pBx, nHDan);
	double	dDistL			= pADeckData->GetSideBarDistFromCenterGir(TRUE, TRUE);
	double	dLengthL		= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT);
	double	dDistR			= pADeckData->GetSideBarDistFromCenterGir(TRUE, FALSE);
	double	dLengthR		= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT);
	double	dLengthReturn	= 0;
	dLengthReturn = StressCalc.Bogan(dDistL, dDistR, dLengthL, dLengthR, dDistGuard); 

	return dLengthReturn;
}

double CAPlateOutBMBrInfo::GetLengthGirder(long nG)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);

	double	dstaGirStt	= pGir->GetSSS(OFF_STT_GIRDER);
	double	dstaGirEnd	= pGir->GetSSS(OFF_END_GIRDER);

	return dstaGirEnd - dstaGirStt;
}

CDoubleArray CAPlateOutBMBrInfo::GetSizeConnectPart(long nG, BOOL bUpper)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxLast(BX_END_GIRDER);

	double staGirStt = pGir->GetSSS(OFF_STT_SLAB);// ���ؽ������� ���� �����̼�
	double staGirEnd = pGir->GetSSS(OFF_END_SLAB);// ���ؽ������� ���� �����̼�	
	double	dThick = 0, sta = 0;
	double	dSizeTen = 0, dSizeCom = 0, dSizeAlter = 0;
	CDoubleArray	arrPartSize;
	arrPartSize.RemoveAll();
	
	if(bUpper)
	{
		// ���� �������� ġ���� �׸�.
		CPlateBasicIndex *pBx		= Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE, pBxStt, pBxEnd);
		double	dPreSta		= staGirStt;
		double	dPreThick	= 0;
		while(pBx)   
		{
			dThick = pGir->GetThickJewon(G_F_U, pBx);
			if (!pBx->IsState(BX_UPPERFLANGE) || dThick != dPreThick)   
			{
				sta = pBx->GetStation();
				
				if (staGirStt < sta && sta < staGirEnd)   
				{
					long nValue	= pBx->GetCompressTypeByComplex();	// 0:��ξ���, 1:�Ϻξ���, 2:����

					if(nValue==1)		++dSizeCom;		
					else if(nValue==2)	++dSizeAlter;
					else if(nValue==3)	++dSizeTen;
				}
				
				if (sta >= staGirEnd)
					break;
				dPreSta = sta;
				dPreThick = dThick;
			}
			pBx = Finder.GetBxNext();
		}
	}
	else
	{
		// ���� �������� ġ���� �׸�.
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE, pBxStt, pBxEnd);
		double	dPreSta		= staGirStt;
		double	dPreThick	= 0;
		while(pBx)   
		{
			dThick = pGir->GetThickJewon(G_F_L, pBx);
			if (!pBx->IsState(BX_LOWERFLANGE) || dThick != dPreThick)   
			{
				sta = pBx->GetStation();
				
				if (staGirStt < sta && sta < staGirEnd)   
				{
					long nValue	= pBx->GetCompressTypeByComplex();	// 0:��ξ���, 1:�Ϻξ���, 2:����

					if(nValue==1)		++dSizeTen;
					else if(nValue==2)	++dSizeAlter;
					else if(nValue==3)	++dSizeCom;
				}

				if (sta >= staGirEnd)
					break;
				dPreSta = sta;
				dPreThick = dThick;
			}
			pBx = Finder.GetBxNext();
		}
	}

	arrPartSize.Add(dSizeTen);
	arrPartSize.Add(dSizeAlter);
	arrPartSize.Add(dSizeCom);

	return arrPartSize;	
}

CDoubleArray CAPlateOutBMBrInfo::GetLengthWebButtWelding()
{	
	CPlateBridgeApp	*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CWeldStandard	*pWeldStd	= m_pAPlateOutBMStd->GetDataManage()->GetWeldStandard();
	
	CPtrList		*pList		= &pWeldStd->m_ListSang;

	if(pList->GetCount() <= 0) return 0;

	CDoubleArray	arrLengthButtWelding;
	arrLengthButtWelding.RemoveAll();

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		POSITION pos = pList->GetHeadPosition();
		double	dLengthButtWelding	= 0;

		while( pos )
		{	
			CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

			if(pRec->m_nGirder==nG && pRec->GetStrSubEleName()=="������" && pRec->m_sWeldType=="�´�� ����")
				dLengthButtWelding += pRec->m_dLengthWeld;
		}
		arrLengthButtWelding.Add(dLengthButtWelding);
	}

	return arrLengthButtWelding; 
}

//���/�Ϻ�  �÷��� ���� �� ���Ʈ ����.....
void CAPlateOutBMBrInfo::GetFlangeLengthCompress(BOOL bUpper, CDoubleArray &arrFlangeWidth, 
									CDoubleArray &arrCompress, CDoubleArray &arrSort, double dCompress[3][99])
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp *pGir	= pDB->GetGirder(0);
	CPlateBxFinder Finder(pGir);	

	__int64 BxFlag;
	if (bUpper)	BxFlag = BX_UPPERFLANGE | BX_SPLICE;//���ǰ�������, �������
	else		BxFlag = BX_LOWERFLANGE | BX_SPLICE;//���ǰ�������..�������
	
	double dWidth = 0;	
	long nType = 0;
	BOOL bSpliceType = FALSE;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);

	while(pBx)
	{				
		//����������� �ƴ��� üũ ���....
		if (pBx->IsState(BX_SPLICE))
		{
			bSpliceType = pBx->GetSplice()->m_bSpliceType;
			if(bSpliceType) 
			{
				pBx = Finder.GetBxNext();
				continue;//���������̹Ƿ� ���...
			}
		}
		
		if (bUpper)	dWidth = pGir->GetWidthFlangeUpper(pBx);
		else		dWidth = pGir->GetWidthFlangeLower(pBx);

		arrFlangeWidth.Add(dWidth);
		//���� �� 1=��ξ���, 2=�Ϻξ���, 3=������ ����
		nType = pBx->GetCompressTypeByComplex();
		
		arrCompress.Add(nType);
		pBx = Finder.GetBxNext();
	}
	
	arrSort = arrFlangeWidth;
	arrSort.RemoveSameValue();
	arrSort.Sort();

	long dSize = arrSort.GetSize();	
	long nR = 0;
	long nFix = 0;
	//��� = 0:����, 1:����, 2:����... [99] = dSize��ŭ��...
	//�Ϻ� = 0:����, 1:����, 2:����... [99] = dSize��ŭ��...
	//dCompress[0][] : ����η�
	//dCompress[1][] : �����η�
	//dCompress[2][] : ����η�
	long nTension = bUpper ? 2 : 1;	// �����
	long nComplex = 3;

	long i, j;
	
	//�迭 �ʱ�ȭ
	for (i=0;i<3;i++)
		for (j=0; j<99;j++)		
			dCompress[i][j] = 0;					
		
	for(i=0; i<arrSort.GetSize(); i++)
		for(j=0; j<arrFlangeWidth.GetSize();j++)
		{
			if(arrSort[i] == arrFlangeWidth[j])
			{
				nR = (long)arrCompress[j];

				if (nR == nTension)
					nFix = 0;//�����					
				else if (nR == nComplex)
					nFix = 1;//������
				else
					nFix = 2;//�����

				dCompress[nFix][i] = dCompress[nFix][i] + 1;
			}
	}

}

CDoubleArray CAPlateOutBMBrInfo::GetLengthHStiffener()
{
	CPlateBridgeApp	*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CWeldStandard	*pWeldStd	= m_pAPlateOutBMStd->GetDataManage()->GetWeldStandard();

	CPtrList		*pList		= &pWeldStd->m_ListSang;

	if(pList->GetCount() <= 0) return 0;

	CDoubleArray	arrLengthHStiffener;
	arrLengthHStiffener.RemoveAll();

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		POSITION pos = pList->GetHeadPosition();
		double	dLengthHStiffener	= 0;

		while( pos )
		{	
			CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

			if(pRec->m_nGirder==nG && pRec->GetStrEleName()=="�Ϲ�" && pRec->GetStrSubEleName()=="���򺸰���")
				dLengthHStiffener += pRec->m_dLengthWeld;
		}
		arrLengthHStiffener.Add(dLengthHStiffener);
	}

	return arrLengthHStiffener; 
}

//���������� 
void CAPlateOutBMBrInfo::GetLengthJijumStiffener(long nG, CDoubleArray &arrLength, CDoubleArray &arrSize)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
	
 	arrLength.RemoveAll();
	arrSize.RemoveAll();

	double dWidth = 0;
	double dLength = 0;
	long nCount = 0;
	
	for(long nJijum = 0; nJijum <= pDB->m_nQtyJigan; nJijum++)
	{
		nCount = 0;
		for (long nJack = 0; nJack < JACKUPSUMAX; nJack++)
		{
			dLength = pDB->m_dJiJumTerm[nJijum][nJack];
			if (dLength != 0) nCount++;			
		}

		CVStiff *pVStiff = pGir->GetJijumVStiffByJijum(nJijum, 0);
		if(pVStiff)
		{		
			arrLength.Add((pVStiff->m_dWidth[0]+pVStiff->m_dWidth[1])*nCount);
			arrSize.Add(nCount*2);	
		}
		else
		{		
			arrLength.Add(0);
			arrSize.Add(0);	
		}
	}
}	

double CAPlateOutBMBrInfo::GetUpperLengthAver(BOOL bPositive)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();

	double	dLengthTot	= 0;
	long	nIndex		= 0;

	for(long nG = 0; nG < pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_UPPERFLANGE | BX_END_GIRDER);

		nIndex	= 0;
		while(pBx)
		{
			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
			double	dStaPre		= pBx->GetStation();
			double	dStaNext	= pBxNext ? pBxNext->GetStation() : pBx->GetStation();
			double	dDist		= dStaNext - dStaPre;
			if(bPositive)
			{
				if(nIndex%2==0)
					dLengthTot += dDist;
			}
			else
			{
				if(nIndex%2!=0)
					dLengthTot += dDist;
			}
			nIndex++;
			pBx = Finder.GetBxNext();
		}
	}

	return dLengthTot/pDB->GetQtyGirder();
}

double CAPlateOutBMBrInfo::GetConfinementLengthAver()
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateBxFinder	Finder(pGir);

	double	dSta1		= Finder.GetBxFirst(BX_STT_SLAB)->GetStation() + (pDB->m_dLengthSlabGirderStt-pDB->m_dLengthTUBindConc_Stt);
	double	dSta2		= Finder.GetBxFirst(BX_END_SLAB)->GetStation() - (pDB->m_dLengthSlabGirderEnd-pDB->m_dLengthTUBindConc_End);
	double	dLength		= pGir->GetLengthDimType(dSta1, dSta2);

	return dLength;
}