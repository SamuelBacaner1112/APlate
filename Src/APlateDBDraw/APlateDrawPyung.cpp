// APlateDrawPyung.cpp: implementation of the CAPlateDrawPyung class.
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
BOOL CAPlateDrawPyung::m_InputDomRotate = TRUE;
BOOL CAPlateDrawPyung::IsInputDomRotate() { return m_InputDomRotate; }
CAPlateDrawPyung::CAPlateDrawPyung(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{
	m_nGirder	   = -1;
	m_nSwitch	   =  0;
	m_CurDirection =  0;
	m_CurDimPoint  = CDPoint(0,0);
}

CAPlateDrawPyung::~CAPlateDrawPyung()
{

}

// nDrawType=6�̸� ����������, Ⱦ�극�̽��� �׸��� ����.(���κ���ġ�Է¿��� ����������bx�� �������� �ʱ� ������)
void CAPlateDrawPyung::DrawPyung(CDomyun *pDom, long nDrawType, BOOL bUpper)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);

	Dom.SetScaleDim(100);
	DrawCenterLineEachGiderPyung(&Dom,TRUE);	//�Ŵ����߽ɼ�

	DrawSlabTotal(&Dom);						//������
	DrawGirder(&Dom, bUpper);					//�Ŵ�

	DrawCenterJijum(&Dom,TRUE);                 //�����߽ɼ�(���ۺ�)
	DrawCenterJijum(&Dom,FALSE);				//�����߽ɼ�(����)

	DrawAbutPierCenterLine(&Dom);               //���뱳���߽ɼ�

	for(long g=0; g<pBridge->GetGirdersu(); g++)
		DrawAbutPierSymbol(&Dom, g);			//����, �����ɺ���ũ
	
	DrawCRBeamCLTotalGirder(&Dom);				//�߽ɼ����׸�
	if(nDrawType!=6)
	{
		DrawHBracingCLTotalGirder(&Dom);		//�극�̽�
		DrawVStiff(&Dom);						//����������
	}
	DrawSplicePyung(&Dom, NULL, NULL, 0, bUpper, TRUE, TRUE, TRUE);//��������

	DrawFactJoinPyung(&Dom,TRUE, TRUE); // ��������(���)
	DrawFactJoinPyung(&Dom,FALSE, TRUE);// ��������(�Ϻ�)
	DrawFactJoinWebPyung(&Dom, TRUE);   // ��������(����)
	DrawShoe(&Dom);						// ���ɺ�
	DrawTaperLinePyung(&Dom);			// �����ۼ�
	DrawSectionSymbol(&Dom);			// ���κ���ġ �ܸ鸶ũ	
	
	if(nDrawType==0)//==============>�������� ���̾�α�
	{
		if(pBridge->IsTUGir())
			DimTextJiganUgan(&Dom);			//����ġ����
		DimTextJiganSlabToGirder(&Dom);	//������-�Ŵ� ġ����
		DimTextJiganGirderToShoe(&Dom);	//�Ŵ�-�� ġ����
		DimTextJiganLength(&Dom);		//������ ġ���� 
		DrawJiganInformation(&Dom, 100);//�Ŵ���������ġ x.y��ǥ
	}
	else if(nDrawType==1)//==============>�������� ���̾�α�
	{
		DrawJiganInformation(&Dom, 100);//�Ŵ���������ġ x.y��ǥ
	}
	else if(nDrawType==2)//==============>�����ġ ���̾�α�
	{		
		DrawJackUp(&Dom);    //��� ������
		DimJackUp(&Dom);
		DrawAbutPier(&Dom);	 //����, ������鵵
		if(!pBridge->m_bUseJackupSolePlate)
			DrawJSolplatePyung(&Dom);

		Dom.SetLineColor(2);
		DrawJiJumStiff(&Dom, NULL, NULL, bUpper); // ���� ������						
	}
	else if(nDrawType==3)//==============>������÷���Ʈ ���̾�α�
	{				
		DrawJackUp(&Dom);    //��� ������
		DrawJSolplatePyung(&Dom);		//��� ���÷���Ʈ ��ġ
//		DimJSoleplatePyung(&Dom);
		DrawAbutPier(&Dom);
	}
	else if(nDrawType==4)//==============>���������� ���̾�α�
	{
		DrawJiJumStiff(&Dom, NULL, NULL, bUpper); // ���� ������
		DimJijumStiff(&Dom);
		DrawAbutPier(&Dom);
	}
	else if(nDrawType==5 || nDrawType==7)
	{
		DrawJackUp(&Dom);    //��� ������
		DrawJiJumStiff(&Dom, NULL, NULL, bUpper); // ���� ������
	}
	else if(nDrawType==8)
	{
		// ���κ� ����Ʈ �÷�������(����������ġ �Է½� �������� Ȯ�� ���ؼ�...)
		for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(nG);
			CPlateBasicIndex *pBxStt = pGir->GetBxOnJijum(0);
			CPlateBasicIndex *pBxEnd = pGir->GetBxOnJijum(pBridge->m_nQtyJigan);
			DrawCRBeamBothGussetWithenBx(&Dom, pBxStt, pBxEnd, TRUE, FALSE, FALSE, FALSE, 0);
		}
	}

#ifdef _DEBUG	
	//���並 ���� �������� ǥ��....
//	CPlateGirderApp *pGir = pBridge->GetGirder(0);
//	CPlateBxFinder Finder(pGir);
//	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
//	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

//	CAPlateDrawJong Draw2(m_pDataMng);
//	Draw2.DrawAlternateSection(&Dom, 0, pBxStt, pBxEnd, TRUE);	
#endif
	
	CEnvRegist env;		
	BOOL bRotate = env.LoadEnvInt(CEnvRegist::INPUTDOM_ROTATE);
	if(bRotate)
		Dom.Rotate(CDPoint (0,0), pBridge->GetAngleBridgeCenter().GetMirrorVert());	
	*pDom << Dom;	
}

void CAPlateDrawPyung::DrawPyungOnAirialView(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);
	Dom.SetScaleDim(100);
//	DrawCenterLineEachGiderPyung(&Dom,TRUE);   //�Ŵ����߽ɼ�

	DrawSlabTotal(&Dom);							//������
	DrawGirder(&Dom, TRUE);                 //�Ŵ�

	DrawCenterJijum(&Dom,TRUE);                 //�����߽ɼ�(���ۺ�)
	DrawCenterJijum(&Dom,FALSE);				//�����߽ɼ�(����)
	DrawAbutPierCenterLine(&Dom);               //���뱳���߽ɼ�
	for(long g=0; g<pBridge->GetGirdersu(); g++)
	{
		DrawAbutPierSymbol(&Dom, g);     //����, �����ɺ���ũ
		DrawGirderNumber(&Dom, g);
	}
	DrawCRBeamCLTotalGirder(&Dom);     // ���κ�(�߽ɼ����׸�)
	DrawHBracingCLTotalGirder(&Dom);   // ����극�̽�(�߽ɼ����׸�)
	DrawVStiff(&Dom);				   // ����������

	*pDom << Dom;	
}


void CAPlateDrawPyung::DrawPyungVStiffPosMark(CDomyun *pDom, long nG, long nPos)
{
	CPlateBridgeApp *pBridge	= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(nG);
	if(pGir==NULL) return;

	CDomyun Dom(pDom);
	Dom.SetLineColor(2);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);
	pBx = Finder.GetBxNext(nPos);

	CDPoint ptPoint = pGir->GetXyGirderDis(pBx->GetStation(), 0, pBx->GetAngleSection());
	Dom.Circle(ptPoint,600);

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawPyungCrossBeamMark(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(!pBx) return;

	CPlateGirderApp *pGir = pBx->GetGirder();	
	if(pGir==NULL) return;

	CPlateGirderApp *pGirMatch = pGir->m_pRight;
	if(pGirMatch==NULL) return;

	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGirMatch);
	
	CDomyun Dom(pDom);
	Dom.SetLineColor(2);

	CDPoint ptLeftUp = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint ptLeftDn = pGirMatch->GetXyGirderDis(pBxMatch->GetStation());		
	CDPoint vAng = (ptLeftDn - ptLeftUp).Unit();

	CDPoint pt1 = ptLeftUp + vAng.RotateInv90() * 100;
	CDPoint pt2 = ptLeftUp - vAng.RotateInv90() * 100;
	CDPoint pt3 = ptLeftDn - vAng.RotateInv90() * 100;
	CDPoint pt4 = ptLeftDn + vAng.RotateInv90() * 100;

	Dom.Solid(pt1.x,pt1.y,pt2.x,pt2.y,pt3.x,pt3.y,pt4.x,pt4.y);
	
	*pDom << Dom;	
}


// ��� : ��� �Ŵ��� �Ŵ� ���� �߽ɼ��� �׸�.(Ȯ���� ��쿡�� �������� ����)
void CAPlateDrawPyung::DrawCenterLineEachGiderPyung(CDomyun *pDom, BOOL bGirderCenterDraw, BOOL bLineNameShow)
{
	CLineInfoApp	*pLine		= m_pDataMng->GetLineInfoApp();
	CPlateBridgeApp *pBridge	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();

	double dStaStt = pBridge->GetStationBridgeStt();
	double dStaEnd = pBridge->GetStationBridgeEnd();

	double dDist = 0;

	// ���� �߽ɼ� �׸� 	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE); 

	CDPoint xy = pLine->GetXyLineDis(dStaStt - 10000, -Dom.Always(1));

	CString szLineName = "";//pLine->GetNameLineEach();

	if(bLineNameShow) Dom.TextOut(xy, szLineName);
	Dom.Rotate(xy, pLine->GetAngleAzimuth(pBridge->GetStationBridgeStt() - 10000));

	pOptStd->SetEnvType(&Dom, HCAD_CENT, LT_DASHDOTDOT_LINE, 3);

	pLine->DrawLS(&Dom, dStaStt - 10000, dStaEnd + 10000, dDist);

	if(bGirderCenterDraw)   
	{
		pOptStd->SetEnvType(&Dom, HCAD_CENT);

		for(long nG = 0 ; nG < pBridge->GetGirdersu();nG++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(nG);

			dDist = pGir->m_dGirderCenter;

			pGir->GetLine()->DrawLS(&Dom, dStaStt - 5000, dStaEnd + 5000, dDist);
		}
	}

	*pDom << Dom;
}


// ���� �����Ͱ� ���� ������ ���� ���� ���...(������ �Բ� ����)
void CAPlateDrawPyung::DrawSlabVirtual(CDomyun *pDomP)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB     = m_pDataMng->GetBridge();

	CDomyun Dom(pDomP);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double staBridgeStt = pDB->m_dStationBridgeStt;
	double staBridgeEnd	= pDB->m_dStationBridgeStt + pDB->m_dLengthUganStt + pDB->m_dLengthTotalSlab + pDB->m_dLengthUganEnd;

	// ����������
	CPlateGirderApp	*pGir = pDB->GetGirder(0);	
	if(pGir==NULL) return;

	double Dist = 5000;
	double sta1 = pGir->GetStationBySc(staBridgeStt,Dist,CDPoint(0,1));
	double sta2 = pGir->GetStationBySc(staBridgeEnd,Dist,CDPoint(0,1));
	
	double dDrawChain = 2000;
	double dStaNext;
	CDPoint pt1, pt2, pt3, pt4;
	for(double dSta=sta1; dSta<=sta2; dSta+=dDrawChain)
	{
		if(dSta > sta2) dSta = sta2;
		dStaNext = dSta + dDrawChain;
		if(dStaNext + dDrawChain > sta2) dStaNext = dSta + dDrawChain;

		pt1 = pGir->GetXyGirderDis(dSta,		+Dist);	
		pt2 = pGir->GetXyGirderDis(dSta,		-Dist);	
		pt3 = pGir->GetXyGirderDis(dStaNext,	-Dist);	
		pt4 = pGir->GetXyGirderDis(dStaNext,	+Dist);	
		Dom.Solid(pt1.x,pt1.y,pt2.x,pt2.y,pt3.x,pt3.y,pt4.x,pt4.y);
	}
	*pDomP << Dom;
}


// ������ BX�� BX_STT_GIRDER, BX_END_GIRDE �� �Ǿ�� ��.
void CAPlateDrawPyung::DrawSlabByBx(CDomyun *pDomP, CPlateBasicIndex *pBxLeftStt, CPlateBasicIndex *pBxLeftEnd,
											CPlateBasicIndex *pBxRightStt, CPlateBasicIndex *pBxRightEnd)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB     = m_pDataMng->GetBridge();

	CDomyun Dom(pDomP);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	// ����������
	CPlateGirderApp	*pGir = pBxLeftStt->GetGirder();
	if(pGir==NULL) return;

	CDPoint xy1,xy2,xy3,xy4;
	BOOL bSttSlab = pBxLeftStt->IsState(BX_STT_GIRDER);
	BOOL bEndSlab = pBxLeftEnd->IsState(BX_END_GIRDER);
	double Dist = -pDB->m_dWidthSlabLeft;
	double sta1 = pGir->GetStationBySc(pBxLeftStt->GetStation(),Dist,pBxLeftStt->GetAngle());
	double sta2 = pGir->GetStationBySc(pBxLeftEnd->GetStation(),Dist,pBxLeftEnd->GetAngle());
	if(bSttSlab)
	{
		sta1 = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
		xy1  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
	}
	if(bEndSlab)
	{
		sta2 = pGir->GetStationBySc(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
		xy2  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
	}
	pGir->GetLine()->DrawLS(&Dom,sta1,sta2,pGir->m_dGirderCenter+Dist);

	// ����������
	pGir	= pBxRightStt->GetGirder();
	Dist = pDB->m_dWidthSlabRight;
	sta1 = pGir->GetStationBySc(pBxRightStt->GetStation(),Dist,pBxRightStt->GetAngle());
	sta2 = pGir->GetStationBySc(pBxRightEnd->GetStation(),Dist,pBxRightEnd->GetAngle());
	if(bSttSlab)
	{
		sta1 = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
		xy3  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
	}
	if(bEndSlab)
	{
		sta2 = pGir->GetStationBySc(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
		xy4  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
	}
	pGir->GetLine()->DrawLS(&Dom,sta1,sta2,pGir->m_dGirderCenter+Dist);
	
	if(bSttSlab)
	{
		CDPoint A[4];
		pDB->GetXySss(OFF_STT_SLAB, A);
		Dom.MoveTo(A[0]);
		for(long n = 1; n < 4; n++)
			Dom.LineTo(A[n]);
	}
	
	if(bEndSlab)
	{
		CDPoint A[4];
		pDB->GetXySss(OFF_END_SLAB, A);
		Dom.MoveTo(A[0]);
		for(long n = 1; n < 4; n++)
			Dom.LineTo(A[n]);
	}

	*pDomP << Dom;
}

// pBxLeftStt,  pBxLeftEnd  = ù��° �Ŵ�
// pBxRightStt, pBxRightEnd = ������ �Ŵ�
void CAPlateDrawPyung::DrawSlabCantileverHunchLine(CDomyun *pDomP, CPlateBasicIndex *pBxLeftStt, CPlateBasicIndex *pBxLeftEnd,
												   CPlateBasicIndex *pBxRightStt, CPlateBasicIndex *pBxRightEnd)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB     = m_pDataMng->GetBridge();

	CDomyun Dom(pDomP);
	pOptStd->SetEnvType(&Dom, HCAD_STLH);
	
	// ���� ��� ���ۺ� =====================================================================
	// ���� ������ ĵƿ���� ���� ��� ���ۺ�
	CPlateGirderApp	*pGir = pBxLeftStt->GetGirder();
	if(pGir==NULL) return;

	CDPoint xy1,xy2,xy3,xy4;
	BOOL bSttSlab = pBxLeftStt->IsState(BX_STT_GIRDER);
	BOOL bEndSlab = pBxLeftEnd->IsState(BX_END_GIRDER);

	double Dist = -pDB->m_dWidthHunch - pGir->GetMaxWidthFlange(TRUE)/2;
	double sta1 = pGir->GetStationBySc(pBxLeftStt->GetStation(),Dist,pBxLeftStt->GetAngle());
	double sta2 = pGir->GetStationBySc(pBxLeftEnd->GetStation(),Dist,pBxLeftEnd->GetAngle());
	if(bSttSlab)
	{
		sta1 = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
		xy1  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
	}
	if(bEndSlab)
	{
		sta2 = pGir->GetStationBySc(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
		xy2  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
	}
	pGir->GetLine()->DrawLS(&Dom,sta1,sta2,pGir->m_dGirderCenter+Dist);

	// ���� ������ ĵƿ���� ���� ��� ���ۺ�
	pGir = pBxRightStt->GetGirder();
	Dist = pDB->m_dWidthHunch + pGir->GetMaxWidthFlange(TRUE)/2;
	sta1 = pGir->GetStationBySc(pBxRightStt->GetStation(),Dist,pBxRightStt->GetAngle());
	sta2 = pGir->GetStationBySc(pBxRightEnd->GetStation(),Dist,pBxRightEnd->GetAngle());
	if(bSttSlab)
	{
		sta1 = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
		xy3  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
	}
	if(bEndSlab)
	{
		sta2 = pGir->GetStationBySc(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
		xy4  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
	}
	pGir->GetLine()->DrawLS(&Dom,sta1,sta2,pGir->m_dGirderCenter+Dist);
	*pDomP << Dom;
	
	// ������ ��ġ �����Է½� �߰���ġ���� =====================================================
	if(pDB->m_dHunchArray.GetSize()==0)		return;

	// ���� ������ ĵƿ���� ��ġ
	pGir = pBxLeftStt->GetGirder();
	bSttSlab = pBxLeftStt->IsState(BX_STT_GIRDER);
	bEndSlab = pBxLeftEnd->IsState(BX_END_GIRDER);

	Dist = -pDB->m_dWidthHunch - pGir->GetWidthFlangeUpper(pBxLeftStt)/2 - pDB->m_dHunchArray.GetAt(0);
	sta1 = pGir->GetStationBySc(pBxLeftStt->GetStation(),Dist,pBxLeftStt->GetAngle());
	sta2 = pGir->GetStationBySc(pBxLeftEnd->GetStation(),Dist,pBxLeftEnd->GetAngle());
	if(bSttSlab)
	{
		sta1 = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
		xy1  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
	}
	if(bEndSlab)
	{
		sta2 = pGir->GetStationBySc(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
		xy2  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
	}
	pGir->GetLine()->DrawLS(&Dom,sta1,sta2,pGir->m_dGirderCenter+Dist);

	// ���� ������ ĵƿ���� ��ġ
	pGir = pBxRightStt->GetGirder();
	Dist = pDB->m_dWidthHunch + pGir->GetWidthFlangeUpper(pBxRightStt)/2 + pDB->m_dHunchArray.GetAt(pDB->m_dHunchArray.GetSize()-1);
	sta1 = pGir->GetStationBySc(pBxRightStt->GetStation(),Dist,pBxRightStt->GetAngle());
	sta2 = pGir->GetStationBySc(pBxRightEnd->GetStation(),Dist,pBxRightEnd->GetAngle());
	if(bSttSlab)
	{
		sta1 = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
		xy3  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB),Dist,pGir->GetAAA(OFF_STT_SLAB));
	}
	if(bEndSlab)
	{
		sta2 = pGir->GetStationBySc(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
		xy4  = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB),Dist,pGir->GetAAA(OFF_END_SLAB));
	}
	pGir->GetLine()->DrawLS(&Dom,sta1,sta2,pGir->m_dGirderCenter+Dist);
	
	*pDomP << Dom;
}

// Taper �� �׸���
void CAPlateDrawPyung::DrawTaperLinePyung(CDomyun *pDom, BOOL bStt, BOOL bEnd)
{
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_CONH);

	CDPoint A[4];
	long n = 0;
	if(bStt)
	{
		m_pDataMng->GetBridge()->GetXySss(OFF_STT_HUNCHSTT, A);
		Dom.MoveTo(A[0]);
		for(n = 1; n < 4; n++)
			Dom.LineTo(A[n]);

		m_pDataMng->GetBridge()->GetXySss(OFF_STT_HUNCHEND, A);
		Dom.MoveTo(A[0]);
		for(n = 1; n < 4; n++)
			Dom.LineTo(A[n]);
	}

	if(bEnd)
	{
		m_pDataMng->GetBridge()->GetXySss(OFF_END_HUNCHEND, A);
		Dom.MoveTo(A[0]);
		for(n = 1; n < 4; n++)
			Dom.LineTo(A[n]);

		m_pDataMng->GetBridge()->GetXySss(OFF_END_HUNCHSTT, A);
		Dom.MoveTo(A[0]);
		for(n = 1; n < 4; n++)
			Dom.LineTo(A[n]);
	}
	*pDom << Dom;
}

// ������
void CAPlateDrawPyung::DrawSlabTotal(CDomyun *pDom)
{	
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();

	CSlabApp *pSlabLeft		= pBridge->GetSlabLeft();
	CSlabApp *pSlabRight	= pBridge->GetSlabRight();
	CPlateGirderApp *pGirCen     = pBridge->GetGirder(-1);
	if(pGirCen==NULL) return;
	
	double dStaStt = pGirCen->GetSSS(OFF_STT_SLAB);
	double dStaEnd = pGirCen->GetSSS(OFF_END_SLAB);
	double dDistLeft	= pSlabLeft->m_dGirderCenter;
	double dDistRight	= pSlabRight->m_dGirderCenter;
	CDPoint vAngStt	= pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint vAngEnd	= pGirCen->GetAAA(OFF_END_SLAB);

	CDPoint ptLeftUp	= pGirCen->GetXyGirderDis(dStaStt, dDistLeft, vAngStt);
	CDPoint ptLeftDn	= pGirCen->GetXyGirderDis(dStaStt, dDistRight, vAngStt);
	CDPoint ptRightUp	= pGirCen->GetXyGirderDis(dStaEnd, dDistLeft, vAngEnd);
	CDPoint ptRightDn	= pGirCen->GetXyGirderDis(dStaEnd, dDistRight, vAngEnd);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	Dom.LineTo(ptLeftUp, ptLeftDn);
	Dom.LineTo(ptRightUp, ptRightDn);

	double dStaSttSc = pGirCen->GetStationBySc(dStaStt,dDistLeft,vAngStt);
	double dStaEndSc = pGirCen->GetStationBySc(dStaEnd,dDistLeft,vAngEnd);
	pSlabLeft->GetLine()->DrawLS(&Dom, dStaSttSc, dStaEndSc, pSlabLeft->m_dGirderCenter);	
	dStaSttSc = pGirCen->GetStationBySc(dStaStt,dDistRight,vAngStt);
	dStaEndSc = pGirCen->GetStationBySc(dStaEnd,dDistRight,vAngEnd);
	pSlabRight->GetLine()->DrawLS(&Dom, dStaSttSc, dStaEndSc, pSlabRight->m_dGirderCenter);

	*pDom << Dom;
}

// �Ŵ�
void CAPlateDrawPyung::DrawGirder(CDomyun *pDom, BOOL bUpper, long nSide)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	CDomyun Dom(pDom);		 		
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	if(nSide==-1)
		pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);


	long nSttGir = (nSide==0 || nSide==-1) ? 0 : pBridge->GetGirdersu() / 2 + 1;
	long nEttGir = (nSide==0 || nSide== 1) ? pBridge->GetGirdersu() : pBridge->GetGirdersu() / 2;

	long nG;
	if(pBridge->GetGirdersu()%2)
	{
		__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE;
		if(bUpper) bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;

		CPlateBxFinder	Finder(pBridge->GetGirder(pBridge->GetGirdersu()/2));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag);

		while(Finder.GetBxFixNext())
		{
			DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), bUpper, nSide, 0);	
			DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), bUpper, nSide);	
			pBx = Finder.GetBxNext();
		}
	}

	for(nG = nSttGir; nG < nEttGir; nG++)
	{
		__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE;
		if(bUpper)	bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;

		CPlateBxFinder	Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag);

		while(Finder.GetBxFixNext())
		{			
			DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), bUpper, 0, 0);	
			DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), bUpper, 0);	
			pBx = Finder.GetBxNext();
		}
	}

	*pDom << Dom;
}

// ����
void CAPlateDrawPyung::DrawJijumLine(CDomyun *pDom, long nJijum)
{
	CPlateBridgeApp *pBridge	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();	

	CSlabApp *pSlabLeft  = pBridge->GetSlabLeft();
	CSlabApp *pSlabRight = pBridge->GetSlabRight();

	double dLen = 0;

	for(long nJ = 0; nJ <= nJijum; nJ++) dLen += pBridge->m_dLengthJigan[nJ];

	double dSta		= pBridge->GetStationBridgeStt() + dLen;
	double dDist	= 0;

	CDPoint vAng = ANGLE90;

	CDPoint ptLeftUp = pSlabLeft->GetXyGirderDis(dSta, dDist, vAng);
	CDPoint ptLeftDn = pSlabRight->GetXyGirderDis(dSta, dDist, vAng);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	Dom.LineTo(ptLeftUp, ptLeftDn);

	*pDom << Dom;
}

// ���� �߽�
void CAPlateDrawPyung::DrawCenterJijum(CDomyun *pDom, BOOL bStt)
{
	CPlateBridgeApp *pBridge	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();	

	CSlabApp		*pSlabLeft  = pBridge->GetSlabLeft();
	CSlabApp		*pSlabRight = pBridge->GetSlabRight();
	CPlateGirderApp *pGirCen	= pBridge->GetGirder(-1);
	if(pGirCen==NULL) return;
	
	double dSta = (bStt) ? pBridge->GetStationBridgeStt() : pBridge->GetStationBridgeEnd();	

	double dDistLeft	= pSlabLeft->m_dGirderCenter;
	double dDistRight	= pSlabRight->m_dGirderCenter;

	CDPoint vAng	= (bStt) ? pGirCen->GetAAA(OFF_STT_SLAB) : pGirCen->GetAAA(OFF_END_SLAB);	

	CDPoint ptLeftUp = pGirCen->GetXyGirderDis(dSta, dDistLeft,	 vAng);
	CDPoint ptLeftDn = pGirCen->GetXyGirderDis(dSta, dDistRight, vAng);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_CENT);

	Dom.LineTo(ptLeftUp, ptLeftDn);

	*pDom << Dom;
}

// ���� �߽ɼ�
void CAPlateDrawPyung::DrawAbutPierCenterLine(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	for(long nJ = 0; nJ <= pBridge->m_nQtyJigan; nJ++)
	{
		DrawPierCenterLineOne(pDom, nJ);
	}
}

// ���� �� �׸���
void CAPlateDrawPyung::DrawPierCenterLineOne(CDomyun *pDom, long nPier)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CARoadOptionStd	*pOptStd	= m_pDataMng->GetOptionStd();

	pOptStd->SetEnvType(pDom, HCAD_STLC);

	CSlabApp	*pSlabLeft	= pBridge->GetSlabLeft();
	CSlabApp	*pSlabRight	= pBridge->GetSlabRight();
	double		dStaLeft	= pSlabLeft->GetStationOnJijum(nPier);
	double		dStaRight	= pSlabRight->GetStationOnJijum(nPier);
	double		dDistLeft	= 0;
	double		dDistRight	= 0;
	CDPoint		vAngLeft	= pSlabLeft->GetAngleOnJijum(nPier);
	CDPoint		vAngRight	= pSlabRight->GetAngleOnJijum(nPier);
	CDPoint		sttXY		= pSlabLeft->GetXyGirderDis(dStaLeft,	dDistLeft,	vAngLeft);
	CDPoint		endXY		= pSlabRight->GetXyGirderDis(dStaRight, dDistRight, vAngRight);
	CDomyun		Dom(pDom);

	pDom->LineTo(sttXY, endXY);

	*pDom << Dom;
}

// ��������.
void CAPlateDrawPyung::DrawSplicePyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide, BOOL bUpperSp, BOOL bInput, BOOL bSolid, BOOL bText, long nShape)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	long nCheckGirder = pBxStt ? pBxStt->GetNumberGirder() : -1;

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		if(nCheckGirder > -1 && nCheckGirder != nG) continue;

		CPlateBxFinder Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE,pBxStt,pBxEnd);
//		long nShape = 0;	// -1: �߽ɼ����� ����������,  0 : ���� ����������,  1 : �߽ɼ� ���� ����������
		while(pBx)
		{
			if(nShape==-2)
			{
				if(pBx==pBxStt)			nShape = 1;
				else if(pBx==pBxEnd)	nShape = -1;
				else					nShape = 0;
			}

			if(bUpperSp)	DrawSpliceUpperOnBx(&Dom, pBx, nSide, FALSE, bInput, bSolid, bText, nShape);
			else			DrawSpliceLowerOnBx(&Dom, pBx, nSide, TRUE, bInput, bSolid, bText, nShape);
			DrawSpliceWebOnBx(&Dom, pBx, nSide, TRUE, bInput, bSolid, bText, nShape);
			pBx = Finder.GetBxNext();
		}
	}

	*pDom << Dom;
}

// �������� ������
void CAPlateDrawPyung::DrawSpliceWebPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide, BOOL bInput, BOOL bSolid)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	if(pGir==NULL) return;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE,pBxStt,pBxEnd);
	CDPoint vX(1,0), vY(0,1);
//		long nShape = 0;	// -1: �߽ɼ����� ����������,  0 : ���� ����������,  1 : �߽ɼ� ���� ����������
	
	while(pBx)
	{
		CPlateSplice *pSp = pBx->GetSplice();
		double  dSta	= pBx->GetStation();
		CDPoint vAng	= pBx->GetAngleSection();
		CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(dSta);
		double dWebT	= pGir->GetThickFactByStation(dSta, G_W);

		CDPoint ptUp = pGir->GetXyGirderDis(dSta, -dWebT/2, vAng);
		CDPoint ptDn = pGir->GetXyGirderDis(dSta,  dWebT/2, vAng);
		CDPoint vVec = vGir.Rotate90();

		double dWidth	= (pSp->m_sA+(pSp->m_sB+1)*pSp->m_sC)*2+pSp->m_sC1;
		double dThick	= pSp->m_sT;
		// ���Ʈ��, ���Ʈ ��ü���϶� ���Ʈ���� ���� �� ������� ���Ʈ���� ������ �����
		if(pSp->m_nWebType!=SP_WEB_FL)
		{
			double dWidthM = (pSp->m_mA+(pSp->m_mB+1)*pSp->m_mC)*2+pSp->m_mC1;
			if(dWidthM > dWidth)
			{
				dWidth = dWidthM;
				dThick = pSp->m_mT;
			}
		}

		CDPoint ptRect[4];
		CDRect	rectSplice;

		// ����������
		if(nSide>-1)
		{
			ptRect[0] = ptUp - vX*dWidth/2 + vY*dThick;
			ptRect[1] = ptUp - vX*dWidth/2;
			ptRect[2] = ptUp + vX*dWidth/2;
			ptRect[3] = ptUp + vX*dWidth/2 + vY*dThick;

			rectSplice = CDRect(ptRect[0], ptRect[2]);

			if(bSolid)	Dom.Solid(	ptRect[0].x, ptRect[0].y, ptRect[1].x, ptRect[1].y, 
									ptRect[2].x, ptRect[2].y, ptRect[3].x, ptRect[3].y);
			else		Dom.Rectangle(rectSplice);
			Dom.Rotate((ptUp+ptDn)/2, vGir);
			*pDom << Dom;
		}
		// ����������
		if(nSide<1)
		{
			ptRect[0] = ptDn - vX*dWidth/2;
			ptRect[1] = ptDn - vX*dWidth/2 - vY*dThick;
			ptRect[2] = ptDn + vX*dWidth/2 - vY*dThick;
			ptRect[3] = ptDn + vX*dWidth/2;

			rectSplice = CDRect(ptRect[0], ptRect[2]);

			if(bSolid)	Dom.Solid(	ptRect[0].x, ptRect[0].y, ptRect[1].x, ptRect[1].y, 
									ptRect[2].x, ptRect[2].y, ptRect[3].x, ptRect[3].y);
			else		Dom.Rectangle(rectSplice);
			Dom.Rotate((ptUp+ptDn)/2, vGir);
			*pDom << Dom;
		}

		if(nSide==0)
			Dom.LineTo(ptUp, ptDn);
		pBx = Finder.GetBxNext();
		*pDom << Dom;
	}

	*pDom << Dom;
}

// ��������.
void CAPlateDrawPyung::DrawFactJoinPyung(CDomyun *pDom, BOOL bUpper, BOOL bSolid)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);
	Dom.SetLineColor(bUpper ? 3 : 5);
	//pOptStd->SetEnvType(&Dom, HCAD_STLC);

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE);

		while(pBx)
		{			
			DrawFactJointOnBx(&Dom, pBx, bUpper, bSolid);

			pBx = Finder.GetBxNext();
		}
	}

	*pDom << Dom;
}

// ��������.
void CAPlateDrawPyung::DrawFactJoinWebPyung(CDomyun *pDom, BOOL bSolid)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	
	CDomyun Dom(pDom);
	Dom.SetLineColor(2);
	//pOptStd->SetEnvType(&Dom, HCAD_STLC);

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_WEB);

		while(pBx)
		{			
			DrawFactJointOnBx(&Dom, pBx, TRUE, bSolid);

			pBx = Finder.GetBxNext();
		}
	}

	*pDom << Dom;
}

// nSide=0 : ����
// nSide=-1: ����
// nSide= 1: ����
void CAPlateDrawPyung::DrawSpliceWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bInSide, BOOL bInput, BOOL bSolid, BOOL bText, long nShape)
{		
	CPlateGirderApp *pGir    = pBx->GetGirder();
	if(pGir==NULL) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateSplice    *pSp     = pBx->GetSplice();

	if(!pSp->m_bSpliceType || bInput || !bInSide)	return;

	CAPlateDrawEtc DrawEtc(m_pDataMng);

	double dSpliceT	= pSp->m_sT;
	double dLength	= (pSp->m_sA + pSp->m_sB * pSp->m_sC + pSp->m_sC1/2)*2;
	if(pSp->m_nWebType != SP_WEB_FL)	// ���Ʈor���Ʈ��ü�� �� ���
	{
		dSpliceT	= pSp->m_mT;
		dLength		= (pSp->m_mA + pSp->m_mB * pSp->m_mC + pSp->m_mC1/2)*2;
	}	

	CDPoint ptCen = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint vGir  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	
	CDomyun Dom(pDom);

	//�Ŵ����� ������
	CPlatePieceObject pObj1(pGir);
	pObj1.m_nMarkBuje = SP_W_PLAN;
	pSp->CreatePlatePieceObj(&pObj1, TRUE, FALSE, -1, TRUE, nShape);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);				
	pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);

	//�Ŵ����� ������
	CPlatePieceObject pObj2(pGir);
	pObj2.m_nMarkBuje = SP_W_PLAN;
	pSp->CreatePlatePieceObj(&pObj2, TRUE, FALSE, 1, TRUE, nShape);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);				
	pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);

	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_WEB, bNextFiller))
	{
		//�Ŵ����� �ʷ�
		CPlatePieceObject pObjFiller1(pGir);
		pObjFiller1.m_nMarkBuje = SP_W_PLAN_F;
		pSp->CreatePlatePieceObj(&pObjFiller1, FALSE, FALSE, -1, TRUE, nShape);
		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObjFiller1, TRUE);

		//�Ŵ����� �ʷ�
		CPlatePieceObject pObjFiller2(pGir);
		pObjFiller2.m_nMarkBuje = SP_W_PLAN_F;
		pSp->CreatePlatePieceObj(&pObjFiller2, FALSE, FALSE,  1, TRUE, nShape);
		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObjFiller2, TRUE);		
	}

	Dom.Rotate(CDPoint(0,0), vGir);
	Dom.Move(ptCen);

	*pDom << Dom;
}

// nSide=0 : ����
// nSide=-1: ����
// nSide= 1: ����
void CAPlateDrawPyung::DrawSpliceUpperOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bInSide, BOOL bInput, BOOL bSolid, BOOL bText, long nShape)
{		
	CPlateGirderApp *pGir    = pBx->GetGirder();
	if(pGir==NULL) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateSplice    *pSp     = pBx->GetSplice();

	CAPlateDrawEtc DrawEtc(m_pDataMng);

	double dWPre	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double dWNext	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double dWidth	= min(dWPre, dWNext);
	double dLength	= 600;
	long nColor	= 4;	
	if(pSp->m_bSpliceType==FALSE)
	{
		dLength = 50;
		nColor = 2;
	}

	double  dSta  = pBx->GetStation();
	double  dDist = dWidth/2;
	CDPoint vAng  = pBx->GetAngleSection();
	CDPoint ptRect[4];

	CDPoint ptUp = pGir->GetXyGirderDis(dSta, -dDist, vAng);
			CDPoint ptDn = pGir->GetXyGirderDis(dSta,  dDist, vAng);

	CDPoint vVec = (ptDn - ptUp).Unit();	
	
	ptRect[0] = ptUp + vVec.Rotate90() * dLength/2;
	ptRect[1] = ptDn + vVec.Rotate90() * dLength/2;
	ptRect[2] = ptDn - vVec.Rotate90() * dLength/2;
	ptRect[3] = ptUp - vVec.Rotate90() * dLength/2;

	CDomyun Dom(pDom);

	CDPoint vGir  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	if(bInput)
	{
		if(bSolid)
		{
			Dom.Solid(	ptRect[0].x, ptRect[0].y, 
						ptRect[1].x, ptRect[1].y, 
						ptRect[2].x, ptRect[2].y, 
						ptRect[3].x, ptRect[3].y, nColor);
		}
		else
			Dom.LineTo(ptUp, ptDn);

		*pDom << Dom;
		
		if(bText)
		{
			Dom.SetTextColor(nColor);			
			Dom.TextOut(ptUp - vVec*Dom.Always(3), pSp->GetMarkstring()/*m_bSpliceType ? "SP" : "FW"*/);
			Dom.Rotate(ptUp - vVec*Dom.Always(3), vGir);
		}

		*pDom << Dom;
	}
	else
	{		
		CPlateSplice *pSp = (CPlateSplice*)pBx->GetSplice();
		CDPoint vAng  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		//������1
		if(pSp->m_bSpliceType)
		{
			CPlatePieceObject pObj1(pGir);
			pObj1.m_nMarkBuje = SP_U_L;
			pSp->CreatePlatePieceObj(&pObj1, TRUE, FALSE, -1, TRUE, nShape);
			CPlatePieceObject pObj2(pGir);
			pObj2.m_nMarkBuje = SP_U_L;
			pSp->CreatePlatePieceObj(&pObj2, TRUE, FALSE, 1, TRUE, nShape);
			CPlatePieceObject pObjOut(pGir);
			pObjOut.m_nMarkBuje = SP_U_U;
			pSp->CreatePlatePieceObj(&pObjOut, TRUE, FALSE, 0, TRUE, nShape);
			if(bInSide)
			{
				//������1
				pOptStd->SetEnvType(&Dom, HCAD_STLC);				
				pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0), vAng);
				Dom.Move(ptUp);			
				*pDom << Dom;
				//������2
				pOptStd->SetEnvType(&Dom, HCAD_STLC);				
				pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0), vAng);
				Dom.Move(ptUp);			
				*pDom << Dom;
			}
			else
			{
				pOptStd->SetEnvType(&Dom, HCAD_STLC);			
				pObjOut.DrawPieceOutline(&Dom, pOptStd, TRUE);
				pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
				if(pObj1.GetObjSize()>4)
					Dom.LineTo(pObj1.GetPoint(1), pObj1.GetPoint(2));
				if(pObj2.GetObjSize()>4)
					Dom.LineTo(pObj2.GetPoint(3), pObj2.GetPoint(0));
				Dom.Rotate(CDPoint(0,0), vAng);
				Dom.Move(ptUp);			
				*pDom << Dom;
			}

			DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_UPPERFLANGE, &pObj1);
			DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_UPPERFLANGE, &pObj2);
			Dom.Rotate(CDPoint(0,0), vAng);
			Dom.Move(ptUp);
			*pDom << Dom;
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(ptUp, ptDn);
			*pDom << Dom;
		}
	}		
}

void CAPlateDrawPyung::DrawSpliceLowerOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bInSide, BOOL bInput, BOOL bSolid, BOOL bText, long nShape)
{		
	CPlateGirderApp *pGir    = pBx->GetGirder();
	if(pGir==NULL) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateSplice    *pSp     = pBx->GetSplice();

	CAPlateDrawEtc DrawEtc(m_pDataMng);

	double dWPre	= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double dWNext	= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double dWidth	= min(dWPre, dWNext);
	double dLength	= 600;
	long nColor	= 4;	
	if(pSp->m_bSpliceType==FALSE)
	{
		dLength = 50;
		nColor = 2;
	}

	double dSta	= pBx->GetStation();
	double dDist	= dWidth/2;

	CDPoint vAng = pBx->GetAngleSection();

	CDPoint ptRect[4];

	CDPoint ptUp = pGir->GetXyGirderDis(dSta, -dDist, vAng);
	CDPoint ptDn = pGir->GetXyGirderDis(dSta,  dDist, vAng);

	CDPoint vVec = (ptDn - ptUp).Unit();	
	
	ptRect[0] = ptUp + vVec.Rotate90() * dLength/2;
	ptRect[1] = ptDn + vVec.Rotate90() * dLength/2;
	ptRect[2] = ptDn - vVec.Rotate90() * dLength/2;
	ptRect[3] = ptUp - vVec.Rotate90() * dLength/2;

	CDomyun Dom(pDom);

	if(bInput)
	{
		if(bSolid)
		{
			Dom.Solid(	ptRect[0].x, ptRect[0].y, 
						ptRect[1].x, ptRect[1].y, 
						ptRect[2].x, ptRect[2].y, 
						ptRect[3].x, ptRect[3].y, nColor);
		}
		else Dom.LineTo(ptUp, ptDn);
		
		if(bText)
		{
			Dom.SetTextColor(nColor);
			Dom.TextOut(ptDn - vVec*Dom.Always(4), pSp->GetMarkstring()/*pSp->m_bSpliceType ? "SPLICE" : "WELD"*/);
		}

		*pDom << Dom;
	}
	else
	{		
		CDPoint vAng  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		//������1
		if(pSp->m_bSpliceType)
		{
			CPlatePieceObject pObj1(pGir);
			pObj1.m_nMarkBuje = SP_L_U;
			pSp->CreatePlatePieceObj(&pObj1, TRUE, FALSE, -1, TRUE, nShape);
			CPlatePieceObject pObj2(pGir);
			pObj2.m_nMarkBuje = SP_L_U;
			pSp->CreatePlatePieceObj(&pObj2, TRUE, FALSE, 1, TRUE, nShape);
			
			if(bInSide)
			{
				//������1
				pOptStd->SetEnvType(&Dom, HCAD_STLC);				
				pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0), vAng);
				Dom.Move(ptUp);			
				*pDom << Dom;
				if(nShape!=99)
				{
					//������2
					pOptStd->SetEnvType(&Dom, HCAD_STLC);					
					pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);
					Dom.Rotate(CDPoint(0,0), vAng);
					Dom.Move(ptUp);			
					*pDom << Dom;
				}
			}
			else
			{
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				CPlatePieceObject pObj1(pGir);
				pObj1.m_nMarkBuje = SP_L_L;
				pSp->CreatePlatePieceObj(&pObj1, TRUE, FALSE, 0, TRUE, nShape);
				pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0), vAng);
				Dom.Move(ptUp);			
				*pDom << Dom;
			}
			DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_LOWERFLANGE, &pObj1);
			if(nShape!=99)
				DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_LOWERFLANGE, &pObj2);
			Dom.Rotate(CDPoint(0,0), vAng);
			Dom.Move(ptUp);
			*pDom << Dom;
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(ptUp, ptDn);
			*pDom << Dom;
		}
	}
}

void CAPlateDrawPyung::DrawGirderWebWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nSide, long nLine)
{
	CPlateGirderApp *pGir	 = pBxStt->GetGirder();
	if(pGir==NULL) return;
	
	CDomyun Dom(pDom);
	
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_WEB, pBxStt, pBxEnd);
	
	if(pBx && pBx!=pBxStt)
	{
		CDPoint vAngEnd	= pBx->GetAngleSection();
		double dStaStt	= pBxStt->GetStation();
		double dStaEnd	= pBx->GetStation();		
		double dTWeb	= pBxStt->GetFactChain(G_W)->m_dFactChainThick;
		if(nLine==1) dTWeb = 0;

		if(nSide==0 || nSide==-1)
			pGir->GetLine()->DrawLS(&Dom, dStaStt, dStaEnd, pGir->m_dGirderCenter - dTWeb/2);
		if(nSide==0 || nSide== 1)
			pGir->GetLine()->DrawLS(&Dom, dStaStt, dStaEnd, pGir->m_dGirderCenter + dTWeb/2);

		CDPoint ptUp	= pGir->GetXyGirderDis(dStaEnd, -dTWeb/2, vAngEnd);
		CDPoint ptDn	= pGir->GetXyGirderDis(dStaEnd,  dTWeb/2, vAngEnd);
		Dom.LineTo(ptUp, ptDn);		
	}

	if(!pBx)
		pBx = pBxEnd;

	while(pBx)
	{
		CDPoint vAngEnd	= pBx->GetAngleSection();
		double dStaStt	= pBxStt->GetStation();
		double dStaEnd	= pBx->GetStation();		
		double dTWeb	= 0.0;
		
		if(pBxStt->GetFactChain(G_W))
			dTWeb = pBxStt->GetFactChain(G_W)->m_dFactChainThick;
		
		if(nLine==1) dTWeb = 0;

		if(nSide==0 || nSide==-1)
			pGir->GetLine()->DrawLS(&Dom, dStaStt, dStaEnd, pGir->m_dGirderCenter - dTWeb/2);
		if(nSide==0 || nSide== 1)
			pGir->GetLine()->DrawLS(&Dom, dStaStt, dStaEnd, pGir->m_dGirderCenter + dTWeb/2);

		CDPoint ptUp	= pGir->GetXyGirderDis(dStaEnd, -dTWeb/2, vAngEnd);
		CDPoint ptDn	= pGir->GetXyGirderDis(dStaEnd,  dTWeb/2, vAngEnd);
		Dom.LineTo(ptUp, ptDn);	

		pBxStt = pBx;
		pBx = Finder.GetBxNext();
	}

	if(pBxStt!=pBxEnd)
	{
		CDPoint vAngEnd	= pBxEnd->GetAngleSection();
		double dStaStt	= pBxStt->GetStation();
		double dStaEnd	= pBxEnd->GetStation();		
		double dTWeb	= pBxStt->GetFactChain(G_W)->m_dFactChainThick;
		if(nLine==1) dTWeb = 0;

		if(nSide==0 || nSide==-1)
			pGir->GetLine()->DrawLS(&Dom, dStaStt, dStaEnd, pGir->m_dGirderCenter - dTWeb/2);
		if(nSide==0 || nSide== 1)
			pGir->GetLine()->DrawLS(&Dom, dStaStt, dStaEnd, pGir->m_dGirderCenter + dTWeb/2);

		CDPoint ptUp	= pGir->GetXyGirderDis(dStaEnd, -dTWeb/2, vAngEnd);
		CDPoint ptDn	= pGir->GetXyGirderDis(dStaEnd,  dTWeb/2, vAngEnd);
		Dom.LineTo(ptUp, ptDn);		
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawGirderWithenStation(CDomyun *pDom, CPlateBasicIndex* pBx, double dStaStt, double dStaEnd, BOOL bUpper, BOOL bEndCuttingLine, BOOL bDrawGirder)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	 = pBx->GetGirder();
	if(pGir==NULL) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption   *pGlopt	 = m_pDataMng->GetGlobalOption();
	
	double dWidthSttPre  = pGir->GetWidthOnStation(dStaStt, bUpper, FALSE)/2;
	double dWidthEndPre  = pGir->GetWidthOnStation(dStaEnd, bUpper, FALSE)/2;
	double dTWeb = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	
	CDPoint vAngStt = pBx->GetAngle();
	CDPoint vAngEnd = pBx->GetAngle();	
	if(pGlopt->GetDesignGirderEndType()==0)//����
	{
		vAngStt = CDPoint(0,1);
		vAngEnd = CDPoint(0,1);
	}
	
	CDPoint ptLeftUp	= pGir->GetXyGirderDis(dStaStt, -dWidthSttPre, vAngStt);
	CDPoint ptLeftDn	= pGir->GetXyGirderDis(dStaStt,  dWidthSttPre, vAngStt);
	CDPoint ptRightUp	= pGir->GetXyGirderDis(dStaEnd, -dWidthEndPre, vAngEnd);
	CDPoint ptRightDn	= pGir->GetXyGirderDis(dStaEnd,  dWidthEndPre, vAngEnd);

	CDomyun Dom(pDom);			
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	double dStaSttLeft  = pGir->GetStationBySc(dStaStt, -dWidthSttPre, vAngStt);
	double dStaSttRight = pGir->GetStationBySc(dStaStt,  dWidthSttPre, vAngStt);
	double dStaEndLeft  = pGir->GetStationBySc(dStaEnd, -dWidthEndPre, vAngEnd);
	double dStaEndRight = pGir->GetStationBySc(dStaEnd,  dWidthEndPre, vAngEnd);
	//�Ŵ�����
	if(bDrawGirder)
	{
		if(pBridge->IsTUGir())
		{
//			double	dSttTU	= pBridge->m_dLengthTUBindConc_Stt;
//			double	dEndTU	= pBridge->m_dLengthTUBindConc_End;
			double	dWidTU	= pBridge->m_BindConc.m_dWidth/2;
			
			ptLeftUp	= pGir->GetXyGirderDis(dStaStt, -dWidTU, vAngStt);
			ptLeftDn	= pGir->GetXyGirderDis(dStaStt,  dWidTU, vAngStt);
			ptRightUp	= pGir->GetXyGirderDis(dStaEnd, -dWidTU, vAngEnd);
			ptRightDn	= pGir->GetXyGirderDis(dStaEnd,  dWidTU, vAngEnd);

			double dStaSttLeft_TU  = pGir->GetStationBySc(dStaStt, -dWidTU, vAngStt);
			double dStaSttRight_TU = pGir->GetStationBySc(dStaStt,  dWidTU, vAngStt);
			double dStaEndLeft_TU  = pGir->GetStationBySc(dStaEnd, -dWidTU, vAngEnd);
			double dStaEndRight_TU = pGir->GetStationBySc(dStaEnd,  dWidTU, vAngEnd);
			
			pGir->GetLine()->DrawLS(&Dom, dStaSttLeft_TU, dStaEndLeft_TU, pGir->m_dGirderCenter - dWidTU);
			pGir->GetLine()->DrawLS(&Dom, dStaSttRight_TU, dStaEndRight_TU, pGir->m_dGirderCenter + dWidTU);
			if(!bEndCuttingLine)
			{
				Dom.LineTo(ptLeftUp, ptLeftDn);
				Dom.LineTo(ptRightUp, ptRightDn);
			}
			pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_DOT_LINE);
		}
		pGir->GetLine()->DrawLS(&Dom, dStaSttLeft, dStaEndLeft, pGir->m_dGirderCenter - dWidthEndPre);
		pGir->GetLine()->DrawLS(&Dom, dStaSttRight, dStaEndRight, pGir->m_dGirderCenter + dWidthEndPre);
		if(!bEndCuttingLine)
		{
			Dom.LineTo(ptLeftUp, ptLeftDn);
			Dom.LineTo(ptRightUp, ptRightDn);
		}		
	}

	//������
	dStaSttLeft  = pGir->GetStationBySc(dStaStt, -dTWeb/2, vAngStt);
	dStaSttRight = pGir->GetStationBySc(dStaStt,  dTWeb/2, vAngStt);
	dStaEndLeft  = pGir->GetStationBySc(dStaEnd, -dTWeb/2, vAngEnd);
	dStaEndRight = pGir->GetStationBySc(dStaEnd,  dTWeb/2, vAngEnd);
	pGir->GetLine()->DrawLS(&Dom, dStaSttLeft, dStaEndLeft, pGir->m_dGirderCenter - dTWeb/2);	
	pGir->GetLine()->DrawLS(&Dom, dStaSttRight, dStaEndRight, pGir->m_dGirderCenter + dTWeb/2);	

	if(bEndCuttingLine)
	{
		pOptStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightningAnyDir(ptLeftUp, ptLeftDn, Dom.Always(2));
		Dom.CutLightningAnyDir(ptRightUp, ptRightDn, Dom.Always(2));
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
	}
	*pDom << Dom;

}
//Ⱦ�극�̽� �Է¿��� �ӽ÷� �����.
void CAPlateDrawPyung::DrawGirderWithenStationByLine(CDomyun *pDom, CPlateBasicIndex* pBx, double dStaStt, double dStaEnd, BOOL bUpper, BOOL bEndCuttingLine)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	if(pGir==NULL) return;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	CDomyun Dom(pDom);			
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CVector vGir        = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dWidth       = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dLengthFront = fabs(dStaStt - pBx->GetStation());
	double dLengthBack  = fabs(pBx->GetStation() - dStaEnd);
	double dTWeb        = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	CDPoint xyCen       = pGir->GetXyGirderDis(pBx->GetStation());

	CDPoint xyTemp = xyCen+vGir*dLengthFront+vGir.Rotate90()*dWidth/2;
	Dom.LineTo(xyTemp, xyTemp-vGir*(dLengthFront+dLengthBack));

	xyTemp = xyCen+vGir*dLengthFront-vGir.Rotate90()*dWidth/2;
	Dom.LineTo(xyTemp, xyTemp-vGir*(dLengthFront+dLengthBack));
	
	xyTemp = xyCen+vGir*dLengthFront+vGir.Rotate90()*dTWeb/2;
	Dom.LineTo(xyTemp, xyTemp-vGir*(dLengthFront+dLengthBack));
	
	xyTemp = xyCen+vGir*dLengthFront-vGir.Rotate90()*dTWeb/2;
	Dom.LineTo(xyTemp, xyTemp-vGir*(dLengthFront+dLengthBack));
	
	*pDom << Dom;

}

// nSide= 0: �¿���
// nSide=-1: ����
// nSide= 1: ����
void CAPlateDrawPyung::DrawGirderWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nSide, long nOption, BOOL bDrawSlab)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	 = pBxStt->GetGirder();
	if(pGir==NULL) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pGlopt	 = m_pDataMng->GetGlobalOption();

	double dStaStt = pBxStt->GetStation();
	double dStaEnd = pBxEnd->GetStation();
	
	double dWidthSttPre  = pGir->GetWidthOnStation(dStaStt, bUpper, FALSE)/2;
	double dWidthSttNext = pGir->GetWidthOnStation(dStaStt, bUpper, TRUE)/2;
	double dWidthEndPre  = pGir->GetWidthOnStation(dStaEnd, bUpper, FALSE)/2;
	double dWidthEndNext = pGir->GetWidthOnStation(dStaEnd, bUpper, TRUE)/2;
	
	CDPoint vAngStt = pBxStt->GetAngle();
	CDPoint vAngEnd = pBxEnd->GetAngle();
	
	if(pGlopt->GetDesignGirderEndType()==0)//����
	{
		vAngStt = CDPoint(0,1);
		vAngEnd = CDPoint(0,1);
	}

	double	dStndCuttingAngle	= pBridge->m_dGirEndStndGuttingAngle;
	BOOL	bSttCutAngle90		= (vAngStt.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90		= (vAngEnd.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bSttCutAngle90_Inv	= (vAngStt.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90_Inv	= (vAngEnd.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
	
	CDPoint ptLeftUp	= pGir->GetXyGirderDis(dStaStt, -dWidthSttPre, bSttCutAngle90_Inv? CDPoint(0,1) : vAngStt);
	CDPoint ptLeftCen	= pGir->GetXyGirderDis(dStaStt, 0, vAngStt);
	CDPoint ptLeftDn	= pGir->GetXyGirderDis(dStaStt,  dWidthSttPre, bSttCutAngle90? CDPoint(0,1) : vAngStt);
	CDPoint ptRightUp	= pGir->GetXyGirderDis(dStaEnd, -dWidthEndPre, bEndCutAngle90? CDPoint(0,1) : vAngEnd);
	CDPoint ptRightCen	= pGir->GetXyGirderDis(dStaEnd, 0, vAngEnd);
	CDPoint ptRightDn	= pGir->GetXyGirderDis(dStaEnd,  dWidthEndPre, bEndCutAngle90_Inv? CDPoint(0,1) : vAngEnd);

	double	dBindConcWidth		= pBridge->m_BindConc.m_dWidth;
	double	dBindConcLen_Stt	= pBxStt->IsState(BX_STT_GIRDER) ? pBridge->m_dLengthTUBindConc_Stt : 0;
	double	dBindConcLen_End	= pBxEnd->IsState(BX_END_GIRDER) ? pBridge->m_dLengthTUBindConc_End : 0;

	CDPoint ptLeftUp_BindConc	= pGir->GetXyGirderDis(dStaStt-dBindConcLen_Stt, -dBindConcWidth/2, bSttCutAngle90_Inv ? CDPoint(0,1) : vAngStt);
	CDPoint ptLeftCen_BindConc	= pGir->GetXyGirderDis(dStaStt-dBindConcLen_Stt, 0, vAngStt);
	CDPoint ptLeftDn_BindConc	= pGir->GetXyGirderDis(dStaStt-dBindConcLen_Stt,  dBindConcWidth/2, bSttCutAngle90 ? CDPoint(0,1) : vAngStt);
	CDPoint ptRightUp_BindConc	= pGir->GetXyGirderDis(dStaEnd+dBindConcLen_End, -dBindConcWidth/2, bEndCutAngle90 ? CDPoint(0,1) : vAngEnd);
	CDPoint ptRightCen_BindConc	= pGir->GetXyGirderDis(dStaEnd+dBindConcLen_End, 0, vAngEnd);
	CDPoint ptRightDn_BindConc	= pGir->GetXyGirderDis(dStaEnd+dBindConcLen_End,  dBindConcWidth/2, bEndCutAngle90_Inv ? CDPoint(0,1) : vAngEnd);
	CString szTxt				= _T("");

	szTxt.Format("%.0f:%.0f", pBridge->m_HTaper_H, pBridge->m_HTaper_W);

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	long nBindLine	= bDrawSlab ? LT_DOT_LINE : LT_SOLID;
	long nFlangLine	= pBridge->IsTUGir()&&bUpper ? LT_DOT_LINE : LT_SOLID;

	pOptStd->SetEnvLType(&Dom, nFlangLine);

	if(nSide==0)
	{
		if(pBxStt->IsState(BX_STT_GIRDER))
		{			
			Dom.LineTo(ptLeftUp, ptLeftCen);
			Dom.LineTo(ptLeftCen, ptLeftDn);

			if(pBridge->IsTUGir() && bUpper)
			{
				pOptStd->SetEnvLType(&Dom, nBindLine);

				Dom.LineTo(ptLeftUp_BindConc, ptLeftCen_BindConc);
				Dom.LineTo(ptLeftCen_BindConc, ptLeftDn_BindConc);

				pOptStd->SetEnvLType(&Dom, nFlangLine);
			}
		
		}
		
		if(pBxEnd->IsState(BX_END_GIRDER))
		{
			Dom.LineTo(ptRightUp, ptRightCen);
			Dom.LineTo(ptRightCen, ptRightDn);

			if(pBridge->IsTUGir() && bUpper)
			{
				pOptStd->SetEnvLType(&Dom, nBindLine);

				Dom.LineTo(ptRightUp_BindConc, ptRightCen_BindConc);
				Dom.LineTo(ptRightCen_BindConc, ptRightDn_BindConc);

				pOptStd->SetEnvLType(&Dom, nFlangLine);
			}
		
		}
	}	
	else if(nSide==-1)
	{
		if(pBxStt->IsState(BX_STT_GIRDER))
		{
//			Dom.LineTo(ptLeftUp, (ptLeftUp + ptLeftDn) / 2);
			Dom.LineTo(ptLeftUp, ptLeftCen);
			if(pBridge->IsTUGir())
			{
				pOptStd->SetEnvLType(&Dom, nBindLine);
				Dom.LineTo(ptLeftUp_BindConc, ptLeftCen_BindConc);
				pOptStd->SetEnvLType(&Dom, nFlangLine);
			}
		}
		if(pBxEnd->IsState(BX_END_GIRDER))
		{
//			Dom.LineTo(ptRightUp, (ptRightUp + ptRightDn) / 2);
			Dom.LineTo(ptRightUp, ptRightCen);
			if(pBridge->IsTUGir())
			{
				pOptStd->SetEnvLType(&Dom, nBindLine);
				Dom.LineTo(ptRightUp_BindConc, ptRightCen_BindConc);
				pOptStd->SetEnvLType(&Dom, nFlangLine);
			}
		}
	}
	else
	{
		if(pBxStt->IsState(BX_STT_GIRDER))
		{
//			Dom.LineTo((ptLeftUp + ptLeftDn)/2, ptLeftDn);
			Dom.LineTo(ptLeftCen, ptLeftDn);
			if(pBridge->IsTUGir())
			{
				pOptStd->SetEnvLType(&Dom, nBindLine);
				Dom.LineTo(ptLeftCen_BindConc, ptLeftDn_BindConc);
				pOptStd->SetEnvLType(&Dom, nFlangLine);
			}
		}
		if(pBxEnd->IsState(BX_END_GIRDER))
		{
//			Dom.LineTo((ptRightUp + ptRightDn)/2, ptRightDn);
			Dom.LineTo(ptRightCen, ptRightDn);
			if(pBridge->IsTUGir())
			{
				pOptStd->SetEnvLType(&Dom, nBindLine);
				Dom.LineTo(ptRightCen_BindConc, ptRightDn_BindConc);
				pOptStd->SetEnvLType(&Dom, nFlangLine);
			}
		}
	}
		
	if(!pBxStt->IsState(BX_STT_GIRDER) && pBxStt->IsState(bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE) && dWidthSttPre != dWidthSttNext)
	{		
		double dDist = fabs(dWidthSttNext - dWidthSttPre) * pBridge->m_HTaper_W / pBridge->m_HTaper_H;		
		CDPoint pt[4], ptTxt;		

		if(dWidthSttNext > dWidthSttPre)
		{
			pt[0]	= pGir->GetXyGirderDis(dStaStt,-dWidthSttPre);
			pt[1]	= pGir->GetXyGirderDis(dStaStt,dWidthSttPre);	
			
			dStaStt += dDist;

			pt[2]	= pGir->GetXyGirderDis(dStaStt,-dWidthSttNext);
			pt[3]	= pGir->GetXyGirderDis(dStaStt,dWidthSttNext);

			if(nSide==0 || nSide==-1)
			{
				Dom.LineTo(pt[0],pt[2]);

				if(DRAW_SLOPTEXT & nOption)
				{					
					CDPoint vAng = (pt[2]-pt[0]).Unit();
					CDomyun DomTxt(&Dom);

					pOptStd->SetEnvType(&DomTxt, HCAD_DIML);
					
					Dom.SetTextAlignHorz(DT_LEFT);
					Dom.SetTextAngle(vAng.GetAngleDegree());
					
					ptTxt = pt[0] + vAng * ~(pt[2]-pt[0])/2 + vAng.Rotate90() * DomTxt.Always(1.0);					
					
					Dom.TextOut(ptTxt, szTxt);
					
					Dom << DomTxt;
				}
			}

			if(nSide==0 || nSide== 1)
			{
				Dom.LineTo(pt[1],pt[3]);

				if(DRAW_SLOPTEXT & nOption)
				{
					CDPoint vAng = (pt[3]-pt[1]).Unit();
					CDomyun DomTxt(&Dom);

					pOptStd->SetEnvType(&DomTxt, HCAD_DIML);

					Dom.SetTextAlignHorz(DT_LEFT);
					Dom.SetTextAngle(vAng.GetAngleDegree());

					ptTxt = pt[1] + vAng * ~(pt[3]-pt[1])/2 - vAng.Rotate90() * (DomTxt.Always(1.0) + DomTxt.GetTextHeight());

					Dom.TextOut(ptTxt, szTxt);

					Dom << DomTxt;
				}
			}
		}
	}
	if(!pBxEnd->IsState(BX_END_GIRDER) && pBxEnd->IsState(bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE) && dWidthEndPre != dWidthEndNext)
	{
		double dDist = fabs(dWidthEndNext - dWidthEndPre) * pBridge->m_HTaper_W / pBridge->m_HTaper_H;
		CDPoint pt[4], ptTxt;	

		if(dWidthEndNext < dWidthEndPre)
		{
			pt[0]	=	pGir->GetXyGirderDis(dStaEnd,-dWidthEndNext);
			pt[1]	=	pGir->GetXyGirderDis(dStaEnd,dWidthEndNext);					
			dStaEnd -=	dDist;
			pt[2]	=	pGir->GetXyGirderDis(dStaEnd,-dWidthEndPre);
			pt[3]	=	pGir->GetXyGirderDis(dStaEnd,dWidthEndPre);

			if(nSide==0 || nSide==-1)
			{
				Dom.LineTo(pt[0],pt[2]);

				if(DRAW_SLOPTEXT & nOption)
				{
					CDPoint vAng = (pt[0]-pt[2]).Unit();
					CDomyun DomTxt(&Dom);
					
					pOptStd->SetEnvType(&DomTxt, HCAD_DIML);

					Dom.SetTextAlignHorz(DT_RIGHT);
					Dom.SetTextAngle(vAng.GetAngleDegree());

					ptTxt = pt[2] + vAng * ~(pt[0]-pt[2])/2 + vAng.Rotate90() * DomTxt.Always(1.0);

					Dom.TextOut(ptTxt, szTxt);

					Dom << DomTxt;
				}
			}

			if(nSide==0 || nSide== 1)
			{
				Dom.LineTo(pt[1],pt[3]);

				if(DRAW_SLOPTEXT & nOption)
				{
					CDPoint vAng = (pt[1]-pt[3]).Unit();
					CDomyun DomTxt(&Dom);

					pOptStd->SetEnvType(&DomTxt, HCAD_DIML);

					Dom.SetTextAlignHorz(DT_RIGHT);
					Dom.SetTextAngle(vAng.GetAngleDegree());

					ptTxt = pt[3] + vAng * ~(pt[1]-pt[3])/2 - vAng.Rotate90() * (DomTxt.Always(1.0) + DomTxt.GetTextHeight());

					Dom.TextOut(ptTxt, szTxt);

					Dom << DomTxt;
				}
			}
		}		
	}
	
	double dStaSttLeft  = pGir->GetStationBySc(dStaStt, -dWidthSttPre, bSttCutAngle90_Inv? CDPoint(0,1) : vAngStt);
	double dStaSttRight = pGir->GetStationBySc(dStaStt,  dWidthSttPre, bSttCutAngle90? CDPoint(0,1) : vAngStt);
	double dStaEndLeft  = pGir->GetStationBySc(dStaEnd, -dWidthEndPre, bEndCutAngle90? CDPoint(0,1) : vAngEnd);
	double dStaEndRight = pGir->GetStationBySc(dStaEnd,  dWidthEndPre, bEndCutAngle90_Inv? CDPoint(0,1) : vAngEnd);

	if(nSide==0 || nSide==-1)
		pGir->GetLine()->DrawLS(&Dom, dStaSttLeft, dStaEndLeft, pGir->m_dGirderCenter - dWidthEndPre);

	if(nSide==0 || nSide== 1)
		pGir->GetLine()->DrawLS(&Dom, dStaSttRight, dStaEndRight, pGir->m_dGirderCenter + dWidthEndPre);
	
	//������ũ��Ʈ ���̵� ó��
	if(pBridge->IsTUGir() && bUpper)
	{
		pOptStd->SetEnvLType(&Dom, nBindLine);

		dStaStt = pBxStt->GetStation();
		dStaEnd = pBxEnd->GetStation();

		dStaSttLeft		= pGir->GetStationBySc(dStaStt-dBindConcLen_Stt, -dBindConcWidth/2, bSttCutAngle90_Inv? CDPoint(0,1) : vAngStt);
		dStaSttRight	= pGir->GetStationBySc(dStaStt-dBindConcLen_Stt,  dBindConcWidth/2, bSttCutAngle90? CDPoint(0,1) : vAngStt);
		dStaEndLeft		= pGir->GetStationBySc(dStaEnd+dBindConcLen_End, -dBindConcWidth/2, bEndCutAngle90? CDPoint(0,1) : vAngEnd);
		dStaEndRight	= pGir->GetStationBySc(dStaEnd+dBindConcLen_End,  dBindConcWidth/2, bEndCutAngle90_Inv? CDPoint(0,1) : vAngEnd);

		if(nSide==0 || nSide==-1)
			pGir->GetLine()->DrawLS(&Dom, dStaSttLeft, dStaEndLeft, pGir->m_dGirderCenter - dBindConcWidth/2);

		if(nSide==0 || nSide== 1)
			pGir->GetLine()->DrawLS(&Dom, dStaSttRight, dStaEndRight, pGir->m_dGirderCenter + dBindConcWidth/2);
	}
	
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawFactJointOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bSolid)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	if(pGir==NULL) return;

	CDomyun Dom(pDom);

	double dWidth	= pGir->GetWidthOnStation(pBx->GetStation(), bUpper);
	double dLength	= 200;

	double dSta		= pBx->GetStation();
	double dDist	= dWidth/2;

	CDPoint vAng = pBx->GetAngleSection();

	CDPoint ptRect[4];

	CDPoint ptUp = pGir->GetXyGirderDis(dSta, -dDist, vAng);
	CDPoint ptDn = pGir->GetXyGirderDis(dSta,  dDist, vAng);

	if(bSolid)
	{
		CDPoint vVec = (ptDn - ptUp).Unit();	
		
		ptRect[0] = ptUp + vVec.Rotate90() * dLength/2;
		ptRect[1] = ptDn + vVec.Rotate90() * dLength/2;
		ptRect[2] = ptDn - vVec.Rotate90() * dLength/2;
		ptRect[3] = ptUp - vVec.Rotate90() * dLength/2;		

		Dom.Solid(	ptRect[0].x, ptRect[0].y, 
					ptRect[1].x, ptRect[1].y, 
					ptRect[2].x, ptRect[2].y, 
					ptRect[3].x, ptRect[3].y );
	}
	else	
		Dom.LineTo(ptUp, ptDn);	

	*pDom << Dom;
}

// pBx  = ���� �������� �׸� ���κ���ġ
// pBxN = ���� �극�̽̿����� ����Bx 
void CAPlateDrawPyung::DrawHBracingCLOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, BOOL bThickLine)
{
	if(pBxN==NULL)		return;

	CPlateGirderApp  *pGir = pBx->GetGirder();
	if(pGir==NULL) return;

	CPlateGirderApp  *pGirN= pGir->m_pRight; 
	if(pGirN==NULL) return;

	CBracing         *pBra = pBx->GetBracing();
	CPlateBasicIndex *pBxMatch  = pBx->GetBxMatchByCrossBeam(pGirN);
	CPlateBasicIndex *pBxMatchN = pBxN->GetBxMatchByCrossBeam(pGirN);
	double dStaMid=0;
	CDomyun Dom(pDom);

	double dThickLine = 200;
	if(pBra)
	{
		switch(pBra->GetType())
		{
		case CBracing::BRACTYPE_LEFTWAVE:   // �߾����� ������ ������ �߾ӿ��� ���� ����� Ⱦ�극�̽� ������ Bx�� ã�� �����Ѵ�.
			{
				if(pBxMatch && pBxMatchN)
				{
					dStaMid = (pBx->GetStation()+pBxN->GetStation())/2;						
					dStaMid = pGir->GetBxByStation(dStaMid, BX_HBRACINGGUSSET)->GetStation();
					
					CDPoint pt1 = pGirN->GetXyGirderDis(pBxMatch->GetStation());
					CDPoint pt2 = pGir->GetXyGirderDis(dStaMid);
					CDPoint pt3 = pGirN->GetXyGirderDis(pBxMatchN->GetStation());
					Dom.LineTo(pt1, pt2);
					Dom.LineTo(pt2, pt3);
					if(bThickLine)
					{
						Dom.SetPLineThick(dThickLine);
						Dom.PMoveTo(pt1.x, pt1.y);
						Dom.PLineTo(pt2.x, pt2.y);
						Dom.PLineTo(pt3.x, pt3.y);
						Dom.PLineEnd();
						/*
						CDPointArray vArr;
						vArr.Add(pt1+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						vArr.Add(pt1+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						Dom.HatchingByCDPointArray(vArr, 135, 200, TRUE);
						CDPointArray vArr2;
						vArr2.Add(pt2+(pt3-pt2).Unit().Rotate90()*dThickLine);
						vArr2.Add(pt3+(pt3-pt2).Unit().Rotate90()*dThickLine);
						vArr2.Add(pt3+(pt3-pt2).Unit().RotateInv90()*dThickLine);
						vArr2.Add(pt2+(pt3-pt2).Unit().RotateInv90()*dThickLine);
						Dom.HatchingByCDPointArray(vArr2, 45, 200, TRUE);
						*/
					}
				}
			}
			break;				
		case CBracing::BRACTYPE_RIGHTWAVE:	// �߾����� ������ ������ �߾ӿ��� ���� ����� Ⱦ�극�̽� ������ Bx�� ã�� �����Ѵ�.
			{						
				if(pBxMatch && pBxMatchN)
				{
					dStaMid = (pBxMatch->GetStation() + pBxMatchN->GetStation()) / 2;
					dStaMid = pGirN->GetBxByStation(dStaMid, BX_HBRACINGGUSSET)->GetStation();
					
					CDPoint pt1 = pGir->GetXyGirderDis(pBx->GetStation());
					CDPoint pt2 = pGirN->GetXyGirderDis(dStaMid);
					CDPoint pt3 = pGir->GetXyGirderDis(pBxN->GetStation());
					Dom.LineTo(pt1, pt2);
					Dom.LineTo(pt2, pt3);
					if(bThickLine)
					{
						Dom.SetPLineThick(dThickLine);
						Dom.PMoveTo(pt1.x, pt1.y);
						Dom.PLineTo(pt2.x, pt2.y);
						Dom.PLineTo(pt3.x, pt3.y);
						Dom.PLineEnd();

						/*
						CDPointArray vArr;
						vArr.Add(pt1+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						vArr.Add(pt1+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						Dom.HatchingByCDPointArray(vArr, 45, 200, TRUE);
						CDPointArray vArr2;
						vArr2.Add(pt2+(pt3-pt2).Unit().Rotate90()*dThickLine);
						vArr2.Add(pt3+(pt3-pt2).Unit().Rotate90()*dThickLine);
						vArr2.Add(pt3+(pt3-pt2).Unit().RotateInv90()*dThickLine);
						vArr2.Add(pt2+(pt3-pt2).Unit().RotateInv90()*dThickLine);
						Dom.HatchingByCDPointArray(vArr2, 135, 200, TRUE);		*/
					}
				}
			}
			break;
		case CBracing::BRACTYPE_CROSS:
			{
				if(pBxMatch && pBxMatchN)
				{
					CDPoint pt1 = pGir->GetXyGirderDis(pBx->GetStation());
					CDPoint pt2 = pGir->GetXyGirderDis(pBxN->GetStation());
					CDPoint pt3 = pGirN->GetXyGirderDis(pBxMatch->GetStation());
					CDPoint pt4 = pGirN->GetXyGirderDis(pBxMatchN->GetStation());
					Dom.LineTo(pt1, pt4);
					Dom.LineTo(pt2, pt3);
				}
			}
			break;
		case CBracing::BRACTYPE_SLOPLEFT:
			{
				if(pBxMatchN)
				{
					CDPoint pt1 = pGir->GetXyGirderDis(pBx->GetStation());
					CDPoint pt2 = pGirN->GetXyGirderDis(pBxMatchN->GetStation());
					Dom.LineTo(pt1, pt2);						
					if(bThickLine)
					{
						Dom.SetPLineThick(dThickLine);
						Dom.PMoveTo(pt1.x, pt1.y);
						Dom.PLineTo(pt2.x, pt2.y);
						Dom.PLineEnd();
						/*
						CDPointArray vArr;
						vArr.Add(pt1+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						vArr.Add(pt1+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						Dom.HatchingByCDPointArray(vArr, 45, 200, TRUE);
						*/
					}
				}
			}
			break;
		case CBracing::BRACTYPE_SLOPRIGHT:
			{
				if(pBxMatch)
				{
					CDPoint pt1 = pGirN->GetXyGirderDis(pBxMatch->GetStation());
					CDPoint pt2 = pGir->GetXyGirderDis(pBxN->GetStation());
					Dom.LineTo(pt1, pt2);		
					if(bThickLine)
					{
						Dom.SetPLineThick(dThickLine);
						Dom.PMoveTo(pt1.x, pt1.y);
						Dom.PLineTo(pt2.x, pt2.y);
						Dom.PLineEnd();
						/*
						CDPointArray vArr;
						vArr.Add(pt1+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().Rotate90()*dThickLine);
						vArr.Add(pt2+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						vArr.Add(pt1+(pt2-pt1).Unit().RotateInv90()*dThickLine);
						Dom.HatchingByCDPointArray(vArr, 135, 200, TRUE);*/
					}
				}
			}
			break;
		}
	}
	*pDom << Dom;
}

void CAPlateDrawPyung::GetPointHBracingCLOnBx(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, CDPoint &resultStt, CDPoint &resultEnd)
{
	if(pBxN==NULL)		return;

	CPlateGirderApp  *pGir = pBx->GetGirder();
	if(pGir==NULL) return;

	CPlateGirderApp  *pGirN= pGir->m_pRight; 
	if(pGirN==NULL) return;

	CBracing         *pBra = pBx->GetBracing();
	CPlateBasicIndex *pBxMatch  = pBx->GetBxMatchByCrossBeam(pGirN);
	CPlateBasicIndex *pBxMatchN = pBxN->GetBxMatchByCrossBeam(pGirN);
	double dStaMid=0;
		
	if(pBra)
	{
		switch(pBra->GetType())
		{
		case CBracing::BRACTYPE_LEFTWAVE:   // �߾����� ������ ������ �߾ӿ��� ���� ����� Ⱦ�극�̽� ������ Bx�� ã�� �����Ѵ�.
			{
				if(pBxMatch && pBxMatchN)
				{
					dStaMid = (pBx->GetStation()+pBxN->GetStation())/2;						
					dStaMid = pGir->GetBxByStation(dStaMid, BX_HBRACINGGUSSET)->GetStation();
					
					resultStt = pGirN->GetXyGirderDis(pBxMatch->GetStation());
					//resultMid = pGir->GetXyGirderDis(dStaMid);
					resultEnd = pGirN->GetXyGirderDis(pBxMatchN->GetStation());					
				}
			}
			break;				
		case CBracing::BRACTYPE_RIGHTWAVE:	// �߾����� ������ ������ �߾ӿ��� ���� ����� Ⱦ�극�̽� ������ Bx�� ã�� �����Ѵ�.
			{						
				if(pBxMatch && pBxMatchN)
				{
					dStaMid = (pBxMatch->GetStation() + pBxMatchN->GetStation()) / 2;
					dStaMid = pGirN->GetBxByStation(dStaMid, BX_HBRACINGGUSSET)->GetStation();
					
					resultStt = pGir->GetXyGirderDis(pBx->GetStation());
					//resultMid = pGirN->GetXyGirderDis(dStaMid);
					resultEnd = pGir->GetXyGirderDis(pBxN->GetStation());					
				}
			}
			break;
		case CBracing::BRACTYPE_CROSS:
			{
				if(pBxMatch && pBxMatchN)
				{
					resultStt = pGir->GetXyGirderDis(pBx->GetStation());
					resultEnd = pGir->GetXyGirderDis(pBxN->GetStation());
					CDPoint pt3 = pGirN->GetXyGirderDis(pBxMatch->GetStation());
					CDPoint pt4 = pGirN->GetXyGirderDis(pBxMatchN->GetStation());					
				}
			}
			break;
		case CBracing::BRACTYPE_SLOPLEFT:
			{
				if(pBxMatchN)
				{
					resultStt = pGir->GetXyGirderDis(pBx->GetStation());
					resultEnd = pGirN->GetXyGirderDis(pBxMatchN->GetStation());					
				}
			}
			break;
		case CBracing::BRACTYPE_SLOPRIGHT:
			{
				if(pBxMatch)
				{
					resultStt = pGirN->GetXyGirderDis(pBxMatch->GetStation());
					resultEnd = pGir->GetXyGirderDis(pBxN->GetStation());					
				}
			}
			break;
		}
	}
}

void CAPlateDrawPyung::DrawHBracingCLTotalGirder(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvLType(&Dom, LT_DASHDOT_LINE);
	
	for(long nG=0;nG<pBridge->GetGirdersu()-1;nG++)
	{
		CPlateGirderApp *pGir     = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxNext=NULL;
		CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
		// pBx�� Finder�� �˻��Ǵ°� �ƴ϶� pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2) �Լ��� ����.
		while(pBx)
		{
			pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
			if(pBxNext==NULL)	break;

			DrawHBracingCLOnBx(&Dom, pBx, pBxNext);

			if(pBxNext->GetStation() >= pBxEnd->GetStation())	break;	// ������ �극�̽� ���� �ϰ��
			
			pBx = pBxNext;
		}
	}

#ifdef _DEBUG
/*	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		if(pBx)
		{
			CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
			CString cs;
			long n=0;
			while(pBx)
			{
				CDPoint xyCen = pGir->GetXyGirderDis(pBx->GetStation());
				cs.Format("%.0lf", pBx->GetStation());
				Dom.TextOut(xyCen+vGir.Rotate90()*Dom.Always(3), cs);

				pBx = Finder.GetBxNext();
				n++;
			}
		}
	}*/
#endif
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawHBracingCLGirder(CDomyun *pDom, long nGirStt, long nGirEnd, long nColStt, long nColEnd)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	if (nColStt%2!=0) nColStt--;
	if (nColEnd%2!=0) nColEnd++;
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvLType(&Dom, LT_DASHDOT_LINE);
	
	for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{
		if (nG<nGirStt) continue;
		if (nG>=nGirEnd) continue;

		CPlateGirderApp *pGir     = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxNext=NULL;
		CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);

		// pBx�� Finder�� �˻��Ǵ°� �ƴ϶� pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2) �Լ��� ����.
		long nCol = 0;
		while(pBx)
		{
			pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
			if(pBxNext==NULL)	break;

			if (nCol<nColStt)
			{
				nCol+=2;
				pBx = pBxNext;
				continue;
			}
			if (nCol>=nColEnd)
			{
				nCol+=2;
				pBx = pBxNext;
				continue;
			}
			nCol+=2;
			
			DrawHBracingCLOnBx(&Dom, pBx, pBxNext);
			
			if(pBxNext->GetStation() >= pBxEnd->GetStation())	break;	// ������ �극�̽� ���� �ϰ��
			
			pBx = pBxNext;
		}
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawCRBeamCLTotalGirder(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge	=	m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	=	m_pDataMng->GetOptionStd();

	CDPoint vGir;
	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_CENT);

	for(long nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir		=	pBridge->GetGirder(nG);
		CPlateGirderApp *pGirMatch	=	pGir->m_pRight;

		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxMatch;
			
		while(pBx)
		{			
			pBxMatch = pBx->GetBxMatchByCrossBeam(pGirMatch);

			if(pBxMatch)
			{
				BOOL bSafe = pBridge->IsSafeDiapByCrossBeam(nG*2+3,pBx->GetMatchCol(), TRUE);
				if(bSafe)
				{
					CDPoint pt1 = pGir->GetXyGirderDis(pBx->GetStation());//,0,pBx->GetAngleSection());
					CDPoint pt2 = pGirMatch->GetXyGirderDis(pBxMatch->GetStation());//,0,pBxMatch->GetAngleSection());

					if(pBx->GetSection()->IsenType(SECTION_C))
					{
						double dWFlCr = 50;

						pOptStd->SetEnvType(&Dom, HCAD_DIML);

						vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()); 

						Dom.LineTo(pt1-vGir*dWFlCr, pt1+vGir*dWFlCr);
						Dom.LineTo(pt1+vGir*dWFlCr, pt2+vGir*dWFlCr);
						Dom.LineTo(pt2+vGir*dWFlCr, pt2-vGir*dWFlCr);
						Dom.LineTo(pt2-vGir*dWFlCr, pt1-vGir*dWFlCr);

						pOptStd->SetEnvType(&Dom, HCAD_CENT);
					}
					else
						Dom.LineTo(pt1, pt2);		
#ifdef _DEBUG
					Dom.SetTextAngle(vGir.Rotate90().GetAngleDegree());
					Dom.SetTextHeight(5);
					Dom.TextOut(pt1+vGir.Rotate90()*Dom.Always(20), Comma(pBx->GetStation()));
#endif					
				}
			}

			pBx = Finder.GetBxNext();
		}
	}

	*pDom << Dom;
}



void CAPlateDrawPyung::DrawHBracingCLWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bThickLine)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);

	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd), *pBxNext=NULL;
	while(pBx)
	{
		pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
		if(pBxNext==NULL)	break;
		DrawHBracingCLOnBx(&Dom, pBx, pBxNext, bThickLine);

		if(pBxNext->GetStation() >= pBxEnd->GetStation())	break;	

		pBx = pBxNext;
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawCRBeamCLWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBxStt->GetGirder();
	CPlateGirderApp *pGirMatch = (CPlateGirderApp*)pGir->m_pRight;

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd), *pBxMatch;
		
	while(pBx)
	{			
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirMatch, 1);
		if(pBxMatch)
		{
			CDPoint pt1 = pGir->GetXyGirderDis(pBx->GetStation());
			CDPoint pt2 = pGirMatch->GetXyGirderDis(pBxMatch->GetStation());
			Dom.LineTo(pt1, pt2);
		}
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}



void CAPlateDrawPyung::DrawVStiffSplice(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft, BOOL bInputDim)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	CDomyun Dom(pDom);

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);	
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
		DrawVStiffSpliceWithenBx(&Dom, pBxStt, pBxEnd, bLeft, bInputDim);
	}

	*pDom << Dom;
}
void CAPlateDrawPyung::DrawVStiffSpliceWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft, BOOL bInputDim)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pBxStt ? pBxStt->GetGirder() : pBridge->GetGirder(-1);
	if(pGir==NULL) return;
	
	CDomyun Dom(pDom);

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	
	while(pBx)
	{
		DrawVStiffSpliceOnBx(&Dom, pBx, bLeft, bInputDim);
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}
void CAPlateDrawPyung::DrawVBracingCenterSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim)
{
	CARoadOptionStd  *pOptStd= m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir   = (CPlateGirderApp*)pBx->GetGirder();
	if(pGir==NULL) return;

	CPlateGirderApp  *pGirN  = (CPlateGirderApp*)pGir->m_pRight;
	if(pGirN==NULL) return;

	CPlateBasicIndex *pBxN   = pBx->GetBxMatchByCrossBeam(pGirN);
	CVBracing* pVr           = pBx->GetSection()->GetVBracing();
	if(pVr==NULL) return;
	CVStiff	*pVStiff         = pBx->GetSection() ? pBx->GetSection()->GetVStiff() : NULL;
	if(pVStiff==NULL) return;

	CDPoint xyCenL = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint xyCenR = pGirN->GetXyGirderDis(pBxN->GetStation());
	CDPoint vCrossPlan = (xyCenR-xyCenL).Unit();
	CDPoint xyCen  = (xyCenL+xyCenR)/2+vCrossPlan.Rotate90()*pVStiff->m_dThick[0]/2;
	CDPoint vAng = pBx->GetAngleSectionCrossVBracing();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dT = pVr->m_dT;
	//
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.LineTo(xyCen-vCrossPlan*pVr->m_dc3, xyCen+vCrossPlan*pVr->m_dc4); 
	Dom.LineTo(xyCen-vCrossPlan*pVr->m_dc3+vCrossPlan.Rotate90()*dT, xyCen+vCrossPlan*pVr->m_dc4+vCrossPlan.Rotate90()*dT);
	Dom.LineTo(xyCen-vCrossPlan*pVr->m_dc3, xyCen-vCrossPlan*pVr->m_dc3+vCrossPlan.Rotate90()*dT);
	Dom.LineTo(xyCen+vCrossPlan*pVr->m_dc4, xyCen+vCrossPlan*pVr->m_dc4+vCrossPlan.Rotate90()*dT);
	*pDom << Dom;

	if(bDim)
	{
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetDirection("TOP");
		CDPoint xyStt = xyCen-vCrossPlan*pVr->m_dc3+vCrossPlan.Rotate90()*dT;
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
		Dom.DimLineToExtend(pVr->m_dc3, 0, "C3", COMMA(pVr->m_dc3));
		if(!pVr->m_bSameLeftRight)
			Dom.DimLineToExtend(pVr->m_dc4, 0, "C4", COMMA(pVr->m_dc4));

		Dom.Rotate(xyStt, vAng);
	}	
	
	*pDom << Dom;
}
//�����극�̽���ġ������ ������.
void CAPlateDrawPyung::DrawVStiffSpliceOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bInputDim)
{
	CPlateGirderApp  *pGir   = (CPlateGirderApp*)pBx->GetGirder();
	if(pGir==NULL) return;

	CPlateGirderApp  *pGirN  = (CPlateGirderApp*)pGir->m_pRight;
	if(pGirN==NULL) return;

	CPlateBasicIndex *pBxN   = pBx->GetBxMatchByCrossBeam(pGirN);
	CVBracing* pVr           = pBx->GetSection()->GetVBracing();
	if(pVr==NULL || pGirN==NULL) return;
	CVStiff	*pVStiff         = pBx->GetSection() ? pBx->GetSection()->GetVStiff() : NULL;
	CVStiff	*pVStiffN        = pBxN->GetSection() ? pBxN->GetSection()->GetVStiff() : NULL;
	
	CARoadOptionStd  *pOptStd= m_pDataMng->GetOptionStd();

	CDPoint vAng, vX, vY, xy;
	double dS=0, u3=0, u4=0;
	double dThickWeb  = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dThickWebN = pGirN->GetThickFactByStation(pBxN->GetStation(), G_W);
	CDPoint xyCenStt  = pGir->GetXyGirderDis(pBx->GetStation());
	if(!bLeft)
		xyCenStt  = pGirN->GetXyGirderDis(pBxN->GetStation());

	CDPoint vGir      = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vGirN     = pGirN->GetLine()->GetAngleAzimuth(pBxN->GetStation());
	
	if(pVStiff==NULL) return;
	//
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	//
	if(bLeft)
	{
		vAng = pBx->GetAngleSectionCrossVBracing();
		vX = vAng;
		vY = vX.Rotate90();
		u3 = pVr->m_duL3;
		u4 = pVr->m_duL4;
		dS = pVStiff->m_dWidth[1] - u3 + dThickWeb/2;
	}
	else
	{
		vAng = pBxN->GetAngleSectionCrossVBracing();
		vX = vAng;
		vY = vX.Rotate90();
		u3 = pVr->m_duR3;
		u4 = pVr->m_duR4;
		dS = pVStiffN->m_dWidth[0] - u3 + dThickWebN/2;
	}

	if(bLeft)
	{
		double dd = Compare(vAng.y, 0., _T("="), 0.0001)? 0 : dThickWeb/2/fabs(vAng.y);
		xy =  xyCenStt + vX*(dd+pVStiff->m_dWidth[1]-pVr->m_duL3) + vY*(pVStiff->m_dThick[1]/2+pVr->m_dT);
		Dom.MoveTo(xy);
		xy += vX* (u3+u4);
		Dom.LineTo(xy);
		xy -= vY*pVr->m_dT;
		Dom.LineTo(xy);
		xy -= vX* (u3+u4);
		Dom.LineTo(xy);
		xy += vY*pVr->m_dT;
		Dom.LineTo(xy);
		*pDom << Dom;

		if(bInputDim)
		{
			double dd = Compare(vAng.y, 0., _T("="), 0.0001) ? 0 : (dS+u3)/fabs(vAng.y);
			xy =  xyCenStt + vX*dd + vY*(pVStiff->m_dThick[1]/2);
			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xy.x, xy.y-pVr->m_dT, 0, "T", FALSE);
			Dom.DimLineTo(pVr->m_dT, 0, "");
			Dom.Rotate(xy,  -vAng);
			*pDom << Dom;
		}
	}
	else
	{
		double dd = Compare(vAng.y, 0., _T("="), 0.0001) ? 0 : dThickWebN/2/fabs(vAng.y);
		xy =  xyCenStt - vX*(dd+pVStiffN->m_dWidth[0]-pVr->m_duR3) + vY*(pVStiffN->m_dThick[0]/2+pVr->m_dT);
		Dom.MoveTo(xy);
		xy -= vX* (u3+u4);
		Dom.LineTo(xy);
		xy -= vY*pVr->m_dT;
		Dom.LineTo(xy);
		xy += vX* (u3+u4);
		Dom.LineTo(xy);
		xy += vY*pVr->m_dT;
		Dom.LineTo(xy);
		*pDom << Dom;
	}

}

void CAPlateDrawPyung::DrawVBracingFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon, BOOL bUpper)
{
	CGlobarOption	*pOpt = m_pDataMng->GetGlobalOption();
	CARoadOptionStd  *pOptStd= m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir   = (CPlateGirderApp*)pBx->GetGirder();
	if(pGir==NULL) return;

	CPlateGirderApp  *pGirN  = (CPlateGirderApp*)pGir->m_pRight;
	if(!pGirN)	return;

	CPlateBasicIndex *pBxN   = pBx->GetBxMatchByCrossBeam(pGirN, 1);
	if(!pBxN)	return;
	CVStiff			*pVStiff = pBx->GetSection() ? pBx->GetSection()->GetVStiff() : NULL;
	CVStiff			*pVStiffN= pBxN->GetSection() ? pBxN->GetSection()->GetVStiff() : NULL;
	CVBracing       *pVr     = pBx->GetSection()->GetVBracing();

	if(pVr == NULL) return;

	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CAPlateDrawSteel DrawSteel(m_pDataMng);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	double dWebT    = pGir->GetThickFactByStation(pBx->GetStation(),	G_W);
	double dTVStiff = pVStiff->m_dThick[1];
	long nSteelTypeUp = pVr->GetSteelMaterialData(bUpper ?  STEELMAT_UPPER : STEELMAT_LOWER)->m_nType;
	
	CDPoint vGir  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vGirN = pGirN->GetLine()->GetAngleAzimuth(pBxN->GetStation());
	CDPoint vDir  = pBx->GetAngleSectionCrossVBracing();

	double dH   = pBx->GetSection()->GetBeamBaseHeight(bUpper ? CSection::SEC_UP : CSection::SEC_DOWN, TRUE, FALSE);
	double dT2  = pBx->GetSection()->GetBeamJewon(bUpper ? CSection::SEC_UP : CSection::SEC_DOWN, CSection::T2);
	double dHCen= pBx->GetSection()->GetBeamJewon(bUpper ? CSection::SEC_UP : CSection::SEC_DOWN, CSection::CENTERDY);
	if(!pOpt->GetInstallBeamAtCenter())
		dHCen = dH/2;

	CDPoint vFrontUp = pVr->GetVectorFrame(0);

	CDPoint xyCenStt, xyCenEnd, xyStt, xyEnd, vAng;
	xyCenStt = pGir->GetXyGirderDis(pBx->GetStation());
	xyCenEnd = pGirN->GetXyGirderDis(pBxN->GetStation());
	vAng = pBx->GetAngleSectionCrossVBracing();

	double dd = Compare(vAng.y, 0., _T("="), 0.0001) ? 0 : dWebT/2/fabs(vAng.y);
	if(nSteelTypeUp==CSteelMaterialData::CTSTEEL)
	{
		xyStt = xyCenStt + vDir*(dd+pVStiff->m_dWidth[1]+pVr->m_duL4-pVr->m_dULG) + vDir.Rotate90()*(dTVStiff/2-dT2/2);
		xyEnd = xyCenEnd - vDir*(dd+pVStiffN->m_dWidth[0]+pVr->m_duR4-pVr->m_dURG) + vDir.Rotate90()*(dTVStiff/2-dT2/2);
	}
	else
	{
		xyStt = xyCenStt + vDir*(dd+pVStiff->m_dWidth[1]+pVr->m_duL4-pVr->m_dULG) + vDir.RotateInv90()*(dHCen - dTVStiff/2);
		xyEnd = xyCenEnd - vDir*(dd+pVStiffN->m_dWidth[0]+pVr->m_duR4-pVr->m_dURG) + vDir.RotateInv90()*(dHCen - dTVStiff/2);
	}
	
	long nInstallDir[4] = {0, 0, 0, 0};
	DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeUp, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, CAPlateDrawSteel::ALL, bJewon, FALSE, bUpper ? CAPlateDrawSteel::UPPER_VFLANGE : CAPlateDrawSteel::LOWER_VFLANGE );
	//DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeUp, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, CAPlateDrawSteel::ALL, bJewon, FALSE, CAPlateDrawSteel::UPPER_VFLANGE );
	*pDom << Dom;
}



void CAPlateDrawPyung::DrawCRBeamVBracingFlangeWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, BOOL bHalfTrim, BOOL bJewon)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);

	long nCheckGirder = pBxStt ? pBxStt->GetNumberGirder() : -1;

	for(long nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		if(nCheckGirder > -1 && nCheckGirder != nG) continue;

		CPlateGirderApp *pGir = pBridge->GetGirder(nG);

		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);				
		while(pBx)
		{			
			CSection *pSec = pBx->GetSection();
			ASSERT(pSec);
			CDomyun DomSub(&Dom);
			if(pSec->IsenDetType(SECDET_CROSSBEAM))
			{
				CPlateGirderApp	*pGirCheck	= (CPlateGirderApp*)pBx->GetGirder();
				CPlateGirderApp	*pGirR		= (CPlateGirderApp*)pGirCheck->m_pRight;
				CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR, 1);
				if(!pBxMatch)
				{
					pBx = Finder.GetBxNext();
					continue;
				}

				DrawCRBeamWebOnBx(&DomSub,pBx,bUpper,TRUE,TRUE);
				if(pSec->GetCrossBeam()->m_nLGirSpType)
				{
					BOOL bLeft = TRUE;
					DrawCRBeamGassetFlangeOnBx(&DomSub, pBx, bUpper, bLeft, FALSE, FALSE, FALSE, TRUE);
					DrawCRBeamGassetWebOnBx(&DomSub, pBx, bUpper, bLeft);
					DrawCRBeamSplicePlateOnBx(&DomSub,pBx,bUpper,bLeft,TRUE);
				}
				if(pSec->GetCrossBeam()->m_nRGirSpType)
				{
					BOOL bLeft = FALSE;
					DrawCRBeamGassetFlangeOnBx(&DomSub, pBx, bUpper, bLeft, FALSE, FALSE, FALSE, TRUE);
					DrawCRBeamGassetWebOnBx(&DomSub, pBx, bUpper, bLeft);
					DrawCRBeamSplicePlateOnBx(&DomSub,pBx,bUpper,bLeft,FALSE);
				}
				
			}
			else if(pSec->IsenDetType(SECDET_VBRACING))
			{
				if(pBridge->IsSafeDiapByCrossBeam(nG*2+3,pBx->GetMatchCol(), TRUE))
				{
					DrawVBracingFlangeOnBx(&DomSub, pBx, bJewon);
				}
			}

			if(bHalfTrim)
			{					
				CLineInfoApp *pLine = m_pDataMng->GetLineInfoApp();
				CDPoint cp1 = pLine->GetXyLineDis(pBx->GetStation(), pBridge->GetSlabLeft()->m_dGirderCenter);
				CDPoint cp2 = pLine->GetXyLineDis(pBx->GetStation(), pBridge->GetSlabRight()->m_dGirderCenter);
				CDPoint cpCen = (cp1+cp2)/2;
				CDPoint vGir = pLine->GetAngleAzimuth(pBx->GetStation());			
				DomSub.TrimByLineOnSide(cpCen-vGir*10000,cpCen+vGir*10000,bUpper ? 1 : -1,TRUE);
			}
			Dom<<DomSub;

			pBx = Finder.GetBxNext();
		}
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawCRBeamVBracingFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bHGBr)
{
	if(pBx==NULL) return;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);
	CSection *pSec = pBx->GetSection();
	if(pSec==NULL) return;

	if(pSec->IsenDetType(SECDET_CROSSBEAM))
	{
		DrawCRBeamWebOnBx(&Dom, pBx, bUpper, TRUE, TRUE);
		if(pSec->GetCrossBeam()->m_nLGirSpType)
		{
			BOOL bLeft = TRUE;
			DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bHGBr?FALSE:bUpper, TRUE, FALSE, FALSE, FALSE, TRUE);
			DrawCRBeamGassetWebOnBx(&Dom, pBx, bUpper, bLeft);
		}
		if(pSec->GetCrossBeam()->m_nRGirSpType)
		{
			BOOL bLeft = FALSE;
			DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bHGBr?FALSE:bUpper, FALSE, FALSE, FALSE, FALSE, TRUE);
			DrawCRBeamGassetWebOnBx(&Dom, pBx, bUpper, bLeft);
		}
	}
	else if(pSec->IsenDetType(SECDET_VBRACING))
	{
		DrawVBracingFlangeOnBx(&Dom, pBx);
	}

	*pDom << Dom;
}


void CAPlateDrawPyung::DrawVStiff(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(pBxStt==NULL && pBxEnd==NULL) return;

	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	CDomyun Dom(pDom);

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);	
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF | BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
		DrawVStiffWithenBx(&Dom, pBx, pBxEnd);
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUseAtHBracingGusset, BOOL bUpper)
{
	if(pBx==NULL) return;

	CPlateGirderApp  *pGir = (CPlateGirderApp*)pBx->GetGirder();
	if(pGir==NULL) return;
	
	double dGirLenL    = 50;
	double dGirLenR    = 50;
	if(bUseAtHBracingGusset)
	{
		CHBracingGusset* pBrGu   = pBx->GetHBracingGusset();
		double dWidth1 = pGir->GetWidthHBracingGussetJewon(pBx, 0);
		double dWidth2 = pGir->GetWidthHBracingGussetJewon(pBx, 1);
		if(!pBrGu)	dGirLenL  = dGirLenR = 500;			
		else		dGirLenL  = dGirLenR = max(dWidth1, dWidth2)/2;
	}
	double dThickWeb  = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	CDPoint vGir      = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vX(1,0), vY(0,1), vAng(0,0), xy(0,0);
	
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BxArrVStiff;
	//CPlateBasicIndexArray BxArrVStiff; 
	pGir->GetBxWithenStation(BxArrVStiff, pBx->GetStation()-dGirLenL, pBx->GetStation()+dGirLenR,
		                     BX_VSTIFF | BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex* pBxVStiff = NULL;

	CDomyun Dom(pDom);
	for(long n=0; n<BxArrVStiff.GetSize(); n++)
	{
		pBxVStiff = BxArrVStiff.GetAt(n);

		if(pBxVStiff->IsJijum())	continue;
		CVStiff *pVStiff = pBxVStiff->GetSection()->GetVStiff();

		if(pBxVStiff->GetSection()->IsenDetType(SECDET_CROSSBEAM) || pBxVStiff->GetSection()->IsenDetType(SECDET_VBRACING))
			vAng = pBxVStiff->GetAngleSectionCrossVBracing().Rotate(CDPoint(-1,0));
		else
			vAng = vGir.Rotate90();

		vX = vAng;
		vY = vX.RotateInv90();
		if(vAng.y==0) return;

		if(pVStiff)
		{
			//���������� ����
			if(pVStiff->m_dThick[1]>0)
			{
				CDPoint xyCenStt = pGir->GetXyGirderDis(pBxVStiff->GetStation(), dThickWeb/2, vAng.RotateInv(vGir));
				double dVStiffW = pVStiff->m_dWidth[1];
				
				Dom.MoveTo(xyCenStt - vY*pVStiff->m_dThick[1]/2 - vX*dVStiffW);
				Dom.LineTo(xyCenStt + vY*pVStiff->m_dThick[1]/2 - vX*dVStiffW);
				GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt + vY*pVStiff->m_dThick[1]/2, vX, xy);
				Dom.LineTo(xy);
				GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt - vY*pVStiff->m_dThick[1]/2, vX, xy);
				Dom.LineTo(xy);
				Dom.LineTo(xyCenStt - vY*pVStiff->m_dThick[1]/2 - vX*dVStiffW);
			}
			//���������� ����
			if(pVStiff->m_dThick[0]>0)
			{
				if((pBxVStiff->GetSection()->IsenDetType(SECDET_CROSSBEAM) || pBxVStiff->GetSection()->IsenDetType(SECDET_VBRACING))
					&& pBxVStiff->GetGirder()->m_pLeft)
				{
					CPlateGirderApp		*pGirL	= pBxVStiff->GetGirder()->m_pLeft;					
					CPlateBasicIndex	*pBxL	= pBxVStiff->GetBxMatchByCrossBeam(pGirL);
					vGir	= pGirL->GetLine()->GetAngleAzimuth(pBxL->GetStation());
					vAng	= pBxL->GetAngleSectionCrossVBracing().Rotate(CDPoint(-1,0));
					vX		= vAng;
					vY		= vX.RotateInv90();
				}
			
				CDPoint xyCenStt = pGir->GetXyGirderDis(pBxVStiff->GetStation(), -dThickWeb/2, vAng.RotateInv(vGir));
				double dVStiffW = pVStiff->m_dWidth[0];
				
				Dom.MoveTo(xyCenStt - vY*pVStiff->m_dThick[0]/2 + vX*dVStiffW);
				Dom.LineTo(xyCenStt + vY*pVStiff->m_dThick[0]/2 + vX*dVStiffW);
				GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt + vY*pVStiff->m_dThick[0]/2, vX, xy);
				Dom.LineTo(xy);
				GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt - vY*pVStiff->m_dThick[0]/2, vX, xy);
				Dom.LineTo(xy);
				Dom.LineTo(xyCenStt - vY*pVStiff->m_dThick[0]/2 + vX*dVStiffW);
			}
		}
	}
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawVStiffWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pBxStt ? pBxStt->GetGirder() : pBridge->GetGirder(-1);
	if(pGir==NULL) return;
	
	CDomyun Dom(pDom);

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF|BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();

		if(!pBx->IsJijum())
			DrawVStiffOnBx(&Dom, pBx);
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}


#define rad		400
//<summary>
// �� �׸���
//</summary>
//<param name="*pDom"></param>
void CAPlateDrawPyung::DrawShoe(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = NULL;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	CDomyun Dom(pDom);
	double  klen = 0, Dis = 0;
	CDPoint vAng;
	CString str;

	Dom.SetScaleDim(100);
	for(long g = 0; g < pBridge->GetGirdersu(); g++)
	{
		pGir = pBridge->GetGirder(g);
		for(long k = 0; k < pBridge->m_nQtyJigan+1; k++)	
		{
			klen = pGir->GetStationOnJijum(k);
			Dis  = -pGir->GetWidthOnStation(klen, TRUE)*1.3;
			// ����ǥ��
 			DrawShoeSymbol(&Dom, g, k);
			// ����ǥ��
			str.Format("%d", g + 1);
			pOptStd->SetEnvType(&Dom, HCAD_MIS2);
			vAng = pGir->GetAngleOnJijum(k);
			TextCircle(&Dom, g, klen+600, vAng, Dis, str);
		}
	}
	*pDom << Dom;
}

// ���� ���⼺ �ﰢ�� ǥ��(����� ������ ���� �������θ� ǥ�õ�)
// nG=-1, nJijum=-1 �̸� (0,0)�� ���������Ͽ� Drawing
void CAPlateDrawPyung::DrawShoeSymbol(CDomyun *pDom, long nG, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(nG);
	if(pGir==NULL) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(100);

	pOptStd->SetEnvType(&Dom, HCAD_SYMB);
	CDPoint xy    = pBridge->GetXyPlateShoe(nG, nJijum);
	double  dSta  = pGir->GetStationOnJijum(nJijum);
	CDPoint vAng  = pGir->GetLine()->GetAngleAzimuth(dSta);
	double  Angle = pBridge->GetAngleShoeOnGirCenter(nG, nJijum).GetAngleDegree();
	long    Dir   = pGir->GetShoe(nJijum)->m_cDirectionShoe;
	DrawShoeSymbolPlan(&Dom,Dir,xy.x,xy.y,vAng);
	Dom.Rotate(ToRadian(Angle),xy.x,xy.y);
	
	*pDom << Dom;
}


void CAPlateDrawPyung::DrawShoeSymbolPlan(CDomyun *pDom, long nType, double BX, double BY, CDPoint vAng, double ScaleFactor)
{
	if(nType<0 || nType>3)	return;

	CDomyun Dom(pDom);

	double radius = 200*ScaleFactor;	// ���� ������
	double Length = 500*ScaleFactor;	// ���� �߽� ~ �ﰢ���� ������
	double triLen = 150*ScaleFactor;	// �ﰢ���� ����
	
	//if(nType==0) radius *= 1.50;
	Dom.Circle(0,0,radius,!nType);

	if(nType==1 || nType==3) // ���Ϸ� �׸���
	{
		Dom.LineTo(0,-Length, 0,Length);
		Dom.Solid( 0, Length, -triLen/3, Length-triLen, triLen/3, Length-triLen, 0, Length);
		Dom.Solid( 0,-Length, -triLen/3,-Length+triLen, triLen/3,-Length+triLen, 0,-Length);
	}

	if(nType==1 || nType==2) // �¿�� �׸���
	{
		Dom.LineTo(-Length,0,  Length,0);
		Dom.Solid( -Length,0, -Length+triLen, -triLen/3, -Length+triLen, triLen/3, -Length,0);
		Dom.Solid(  Length,0,  Length-triLen, -triLen/3,  Length-triLen, triLen/3,  Length,0);
	}

	Dom.Rotate(CDPoint(0,0),vAng);
	Dom.Move(BX,BY);

	*pDom << Dom;
}

//<summary>
//���� �ؽ�Ʈ
//</summary>
//<param name="*pDom"></param>
//<param name="nGirder"></param>
//<param name="dSta"></param>
//<param name="&vAng"></param>
//<param name="dDist"></param>
//<param name="&str"></param>
void CAPlateDrawPyung::SymbolTextCircle(CDomyun *pDom, long nGirder, double dSta, CDPoint &vAng, double dDist, CString &str)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(nGirder);
	if(pGirder==NULL) return;


	CDPoint pXY = pGirder->GetXyGirderDis(dSta, dDist, vAng);

	double dAng = pGirder->GetLine()->GetAngleAzimuth(dSta).GetAngleRadian();

	pDom->TextCircle(pXY.x, pXY.y, str, -1, FALSE, FALSE, dAng);
}

void CAPlateDrawPyung::DrawGirderNumber(CDomyun *pDom, long g, BOOL bInput)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(g);
	if(pGir==NULL) return;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);

	if(bInput) pOptStd->SetEnvType(&Dom,HCAD_SMALL_TITLE);
	else	   pOptStd->SetEnvType(&Dom,HCAD_DIML);

	double dSta  = 0;
	CDPoint vAng(0,0);
	CDPoint pXY(0,0);	

	dSta = pGir->GetSSS(OFF_STT_GIRDER);
	vAng = pGir->GetAAA(OFF_STT_GIRDER);

	CString str;
	str.Format("G%d",g+1);

	if(pBridge->GetGirdersu()<=2)
	{
		SymbolTextCircle(&Dom, g, dSta - Dom.Always(5), vAng, 0, str);
	}
	else
	{
		if(g%2)
			SymbolTextCircle(&Dom, g, dSta - Dom.Always(5), vAng, 0, str);
		else
			SymbolTextCircle(&Dom, g, dSta - Dom.Always(10), vAng, 0, str);
	}
	*pDom << Dom;
}

//<summary>
// ���� �� ���� ��ȣ  Text ����ϱ� : AbutPier
//</summary>
//<param name="*pDom"></param>
//<param name="g"></param>
void CAPlateDrawPyung::DrawAbutPierSymbol(CDomyun *pDom, long g, BOOL bInput)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	long nGirderL = 0;
	long nGirderR = pBridge->GetGirdersu() - 1;

	CPlateGirderApp	*pGirderL	=	pBridge->GetGirder(nGirderL);
	if(pGirderL==NULL) return;
	CPlateGirderApp	*pGirderR	=	pBridge->GetGirder(nGirderR);
	if(pGirderR==NULL) return;

	CARoadOptionStd *pOptStd	=	m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);

	if(bInput) pOptStd->SetEnvType(&Dom,HCAD_SMALL_TITLE);
	else	   pOptStd->SetEnvType(&Dom,HCAD_DIML);

	long	nJigan	=	pBridge->m_nQtyJigan;
	double	dTerm	=	bInput ? Dom.Always(10) : Dom.Always(5);
	double	dWidthR =   pBridge->GetSlabRight()->m_dGirderCenter - pBridge->GetGirder(pBridge->GetGirdersu()-1)->m_dGirderCenter + dTerm;
	double	dSta	=	0.0;

	CString str(_T(""));

	CDPoint vAng(0,0);
	CDPoint pXY(0,0);

	str		=	pBridge->m_strJijumName[0];
	dSta	=	pGirderL->GetSSS(OFF_STT_SHOE);
	vAng	=	pGirderL->GetAAA(OFF_STT_SHOE);

//	SymbolTextCircle(&Dom, nGirderL, dSta, vAng, dWidthL, str);

	dSta	=	pGirderR->GetSSS(OFF_STT_SHOE);
	vAng    =	pGirderR->GetAAA(OFF_STT_SHOE);

	SymbolTextCircle(&Dom, nGirderR, dSta, vAng, dWidthR, str);

	for(long n = 0; n < nJigan - 1; n++)
	{
		str		=	pBridge->m_strJijumName[n+1];
		dSta	=	pGirderL->GetStationOnJijum(n+1);
		vAng    =	pGirderL->GetAngleOnJijum(n+1);

//		SymbolTextCircle(&Dom, nGirderL, dSta, vAng, dWidthL, str);

		dSta	=	pGirderR->GetStationOnJijum(n+1);
		vAng    =	pGirderR->GetAngleOnJijum(n+1);

		SymbolTextCircle(&Dom, nGirderR, dSta, vAng, dWidthR, str);
	}

	str		=	pBridge->m_strJijumName[nJigan];
	dSta	=	pGirderL->GetSSS(OFF_END_SHOE);
	vAng	=	pGirderL->GetAAA(OFF_END_SHOE);

//	SymbolTextCircle(&Dom, nGirderL, dSta, vAng, dWidthL, str);

	dSta	=	pGirderR->GetSSS(OFF_END_SHOE);
	vAng	=	pGirderR->GetAAA(OFF_END_SHOE);

	SymbolTextCircle(&Dom, nGirderR, dSta, vAng, dWidthR, str);

	*pDom << Dom;
}

// ���� ���� �Է¿��� ǥ��
void CAPlateDrawPyung::DrawJiganInformation(CDomyun *pDom, double dScale)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(-1);
	if(pGirder==NULL) return;

	CSlabApp *pSlabLeft = pBridge->GetSlabLeft();
	CSlabApp *pSlabRight = pBridge->GetSlabRight();

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetDirection("TOP");

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	long nGirder = -1;
	long nJigan  = pBridge->m_nQtyJigan;
	long nDir = 3;

	double dDist = 0;

	double dTermV = 0;
	double dTermH = 0;

	double dTop	   = 0;
	double dBottom = 0;

	double dSta = 0;

	CDPoint pXY(0,0);
	CDPoint vAng(0,1);

	CString strX(_T("")), strY(_T(""));

	Dom.SetTextAlignHorz(TA_LEFT);

	////////////////////////////////////////////////////////////
	// ������ ������ ��ǥ (��,����)
	for(long nSlabPos = 0; nSlabPos < 2; nSlabPos++)
	{
		dTop	= GetSlabTopStt(dSta);
		dBottom	= GetSlabBotStt(dSta);

		for(long n = 0; n < 3; n++)
		{
			nDir   = nSlabPos ? 1 : 3;
			dTermV = Dom.Always(10);// 700;

			if(n == 0) 
			{
				nGirder	= 0;
				CPlateGirderApp *pGir = pBridge->GetGirder(nGirder);
				dDist	= pSlabLeft->m_dGirderCenter-pGir->m_dGirderCenter;

				if(dTop > 0) nDir = nSlabPos ? 7 : 5;
			}
			else if(n == 1) 
			{
				nGirder	= -1;
				dDist	=  0;

				if(0 > dTop && dTop >= -1000 || dBottom <= 0)	nDir = nSlabPos ? 7 : 5;
			}
			else 
			{
				nGirder	=  pBridge->GetGirdersu() - 1;
				CPlateGirderApp *pGir = pBridge->GetGirder(nGirder);
				dDist	= pSlabRight->m_dGirderCenter-pGir->m_dGirderCenter;

				if(dBottom > 0 && dBottom <= 1000)	nDir = nSlabPos ? 7 : 5;
			}

			pGirder	= pBridge->GetGirder(nGirder);

			dSta = nSlabPos ? pGirder->GetSSS(OFF_END_SLAB) : pGirder->GetSSS(OFF_STT_SLAB);
			vAng = nSlabPos ? pGirder->GetAAA(OFF_END_SLAB) : pGirder->GetAAA(OFF_STT_SLAB);
			pXY	 = pGirder->GetXyGirderDis(dSta,dDist,vAng);

			// ���ϵ� ��ǥ�� mm�� M�� ��ȯ�Ͽ� ���
			strX.Format("X = %.6lf", toM(pXY.y));
			strY.Format("Y = %.6lf", toM(pXY.x));

			dTermH = nSlabPos	?  strX.GetLength() * Dom.GetTextHeight()
								: -strX.GetLength() * Dom.GetTextHeight();

			Dom.GiSiArrowExtend(pXY.x, pXY.y, dTermV, dTermH, nDir, strX, strY);

			vAng = pGirder->GetLine()->GetAngleAzimuth(dSta);

			Dom.Rotate(vAng.GetAngleRadian(),pXY.x,pXY.y);

			*pDom << Dom;
		}
	}

	////////////////////////////////////////////////////////////
	// �������� ��ǥ
	Dom.SetTextAlignHorz(TA_RIGHT);

	for(long nJiganPos = 0; nJiganPos < nJigan + 1; nJiganPos++)
	{
		dTop	= GetSlabTopStt(dSta);
		dBottom	= GetSlabBotStt(dSta);

		for(long n = 0; n < 3; n++)
		{
			nDir = 1;

			if(nJiganPos == nJigan) nDir = 3;

			dTermV = Dom.Always(10);

			if(n == 0)	// ������ ���� ����
			{
				nGirder	= 0;
				CPlateGirderApp *pGir = pBridge->GetGirder(nGirder);
				dDist	= pSlabLeft->m_dGirderCenter-pGir->m_dGirderCenter;

				if(dTop > 0)	nDir = 7;
			}
			else if(n == 1)	// �����߽�
			{
				nGirder	= -1;
				dDist	=  0;

				if(0 > dTop && dTop >= -1000 || dBottom <= 0)	nDir = 7;
			}
			else	// ���� ������ ����
			{
				nGirder	= pBridge->GetGirdersu()-1;
				CPlateGirderApp *pGir = pBridge->GetGirder(nGirder);
				dDist	= pSlabRight->m_dGirderCenter-pGir->m_dGirderCenter;

				if(dBottom > 0 && dBottom <= 1000)	nDir = 7;
			}

			pGirder	= pBridge->GetGirder(nGirder);

			dSta = pGirder->GetStationOnJijum(nJiganPos);
			vAng = pGirder->GetAngleOnJijum(nJiganPos);
			pXY	 = pGirder->GetXyGirderDis(dSta, dDist, vAng);

			// ���ϵ� ��ǥ�� mm�� M�� ��ȯ�Ͽ� ���
			strX.Format("X = %.6lf", toM(pXY.y));
			strY.Format("Y = %.6lf", toM(pXY.x));

			dTermH = strX.GetLength() * Dom.GetTextHeight();

			if(nJiganPos == nJigan && nDir == 7)	nDir = 5;
			if(nDir == 5 || nDir == 3)				dTermH = -strX.GetLength() * Dom.GetTextHeight();

			Dom.GiSiArrowExtend(pXY.x, pXY.y, dTermV, dTermH, nDir, strX, strY);

			vAng = pGirder->GetLine()->GetAngleAzimuth(dSta);

			Dom.Rotate(vAng.GetAngleRadian(),pXY.x,pXY.y);

			*pDom << Dom;
		}
	}

	*pDom << Dom;
}
// ���� �߽ɺ��� ���� ������ ���ܱ����� �̰ݰŸ�
double CAPlateDrawPyung::GetSlabTopStt(double dSta)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	double SlabTopStt = pBridge->GetDisSlabLeftAct(dSta);

	return SlabTopStt;
}

// ���� �߽ɺ��� ���� ������ ���ܱ����� �̰ݰŸ�
double CAPlateDrawPyung::GetSlabBotStt(double dSta)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	double SlabBotStt = GetSlabTopStt(dSta) + pBridge->GetWidthAct(dSta);

	return SlabBotStt;
}

//<summary>
//�����Ÿ�
//</summary>
//<param name="*pDom"></param>
void CAPlateDrawPyung::DimTextJiganUgan(CDomyun *pDom, BOOL bInput, BOOL bStt, BOOL bEnd)
{
	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(-1);

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pGlopt	 = m_pDataMng->GetGlobalOption();
	CLineInfoApp	*pLine		= m_pDataMng->GetLineInfoApp();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double	dBindConcLen_Stt	= pDB->m_dLengthTUBindConc_Stt;
	double	dBindConcLen_End	= pDB->m_dLengthTUBindConc_End;

	Dom.SetDirection("TOP");
	Dom.SetDimLevelDis(0);
	Dom.SetDimLineSpace(0);
	Dom.SetDimExo(0);
	Dom.SetDimExe(0);

	for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);

		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

		double dStaStt = pBxStt->GetStation();
		double dStaEnd = pBxEnd->GetStation();

		CDPoint vAngStt = pBxStt->GetAngle();
		CDPoint vAngEnd = pBxEnd->GetAngle();
		if(pGlopt->GetDesignGirderEndType()==0)//����
		{
			vAngStt = CDPoint(0,1);
			vAngEnd = CDPoint(0,1);
		}

		CDPoint ptStt	= pGir->GetXyGirderDis(dStaStt-dBindConcLen_Stt, 0, vAngStt);
		CDPoint ptStt1	= pGir->GetXyGirderDis(dStaStt, 0, vAngStt);

		CDPoint ptEnd	= pGir->GetXyGirderDis(dStaEnd, 0, vAngEnd);		
		CDPoint ptEnd1	= pGir->GetXyGirderDis(dStaEnd+dBindConcLen_End, 0, vAngEnd);
		
		double dLenStt = ~(ptStt1-ptStt);	// � ���� ������ ���󰡹Ƿ� �����Ÿ��� �޶�����
		double dLenEnd = ~(ptEnd1-ptEnd);

		if(bStt)
		{
			Dom.SetDimObq(-30);
			Dom.DimMoveTo(ptStt.x, ptStt.y, 0);
			Dom.DimLineTo(dLenStt, 0, bInput?"C1":COMMA(dBindConcLen_Stt));
			Dom.Rotate(ptStt, pLine->GetAngleAzimuth(dStaStt-dBindConcLen_Stt));
			*pDom << Dom;
		}

		if(bEnd)
		{
			Dom.SetDimObq(30);
			Dom.DimMoveTo(ptEnd.x, ptEnd.y, 0);
			Dom.DimLineTo(dLenEnd, 0, bInput?"C2":COMMA(dBindConcLen_End));
			Dom.Rotate(ptEnd, pLine->GetAngleAzimuth(dStaEnd));
			*pDom << Dom;
		}	
	}
}
//<summary>
//������-�Ŵ� �Ÿ�
//</summary>
//<param name="*pDom"></param>
void CAPlateDrawPyung::DimTextJiganSlabToGirder(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(-1);

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double dWidthStt = GetSlabBotStt(pGirder->GetSSS(OFF_STT_SLAB));
	double dWidthEnd = GetSlabBotStt(pGirder->GetSSS(OFF_END_SLAB));

	dWidthStt = max(dWidthStt,dWidthEnd);	if(dWidthStt<0) dWidthStt = 0;

	CString str(_T(""));

	CDomyun Dom(pDom);
	Dom.SetScaleDim(100);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	SetDirection(-1,"BOTTOM");

	double dUganStt = pBridge->m_dLengthUganStt;
	double dUganEnd = pBridge->m_dLengthUganEnd;

	DimMoveTo(&Dom, pGirder->GetSSS(OFF_STT_SLAB)-dUganStt, dWidthStt, 1, "J1");
	DimLineTo(&Dom, dUganStt, 1, "", "", TRUE, FALSE);
	DimLineTo(&Dom, pBridge->m_dLengthSlabGirderStt, 1, "S1", "", TRUE, FALSE);
	

	DimMoveTo(&Dom, pGirder->GetSSS(OFF_END_GIRDER), dWidthStt, 1);
	DimLineTo(&Dom, pBridge->m_dLengthSlabGirderEnd, 1, "S2", "", TRUE, FALSE);
	DimLineToOver(&Dom, dUganEnd, 1, "J2", "");

	*pDom << Dom;
}

//<summary>
//�Ŵ�-��
//</summary>
//<param name="*pDom"></param>
void CAPlateDrawPyung::DimTextJiganGirderToShoe(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(-1);

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double dWidthStt = GetSlabBotStt(pGirder->GetSSS(OFF_STT_SLAB));
	double dWidthEnd = GetSlabBotStt(pGirder->GetSSS(OFF_END_SLAB));

	dWidthStt = max(dWidthStt,dWidthEnd);	if(dWidthStt<0) dWidthStt = 0;

	CString str(_T(""));

	CDomyun Dom(pDom);
	Dom.SetScaleDim(100);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	SetDirection(-1,"BOTTOM");

	DimMoveTo(&Dom,pGirder->GetSSS(OFF_STT_GIRDER),dWidthStt,1);
	DimLineTo(&Dom,pBridge->m_dLengthGirderShoeStt,1,"G1");

	DimMoveTo(&Dom,pGirder->GetSSS(OFF_END_SHOE),dWidthStt,1);
	DimLineTo(&Dom,pBridge->m_dLengthGirderShoeEnd,1,"G2");
			
	*pDom << Dom;
}

//<summary>
//�����Ÿ�
//</summary>
//<param name="*pDom"></param>
void CAPlateDrawPyung::DimTextJiganLength(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(-1);

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double dWidthStt = GetSlabBotStt(pGirder->GetSSS(OFF_STT_SLAB));
	double dWidthEnd = GetSlabBotStt(pGirder->GetSSS(OFF_END_SLAB));

	dWidthStt = max(dWidthStt,dWidthEnd);	if(dWidthStt<0) dWidthStt = 0;

	CString str(_T(""));
	CString str2(_T(""));

	CDomyun Dom(pDom);
	Dom.SetScaleDim(100);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	SetDirection(-1,"BOTTOM");

	DimMoveTo(&Dom,pGirder->GetSSS(OFF_STT_SHOE),dWidthStt,1);
	for(long i = 0; i < pBridge->m_nQtyJigan; i++)
	{
		if(pBridge->m_dLengthJigan[i] > 0)
		{	//�������� 0 �̸� break
			str2.Format("L%d", i+1);
			DimLineTo(&Dom,pBridge->m_dLengthJigan[i],1,str2);
		}
	}

	*pDom << Dom;
}

//<summary>
// ġ���� ����
//</summary>
//<param name="nGirder"></param>
//<param name="str"></param>
void CAPlateDrawPyung::SetDirection(long nGirder, CString str)
{
	if	   (str == "LEFT"  ) m_CurDirection = 0;
	else if(str == "TOP"   ) m_CurDirection = 1;
	else if(str == "RIGHT" ) m_CurDirection = 2;
	else if(str == "BOTTOM") m_CurDirection = 3;

	m_nGirder = nGirder;
}

//<summary>
//ġ���� ������
//</summary>
//<param name="*pDom"></param>
//<param name="dSta"></param>
//<param name="dDist"></param>
//<param name="nCol"></param>
//<param name="str"></param>
void CAPlateDrawPyung::DimMoveTo(CDomyun *pDom, double dSta, double dDist, long nCol, CString str)
{
//	nCol += 1;

	double dTerm  = 1000 * nCol;

	if		(m_CurDirection == 0)	HPointLine(pDom, dSta, dTerm,  dDist,  TRUE);	// LEFT
	else if	(m_CurDirection == 1)	VPointLine(pDom, dSta, dDist - dTerm, FALSE);	// TOP
	else if	(m_CurDirection == 2)	HPointLine(pDom, dSta, dTerm,  dDist, FALSE);	// RIGHT
	else if	(m_CurDirection == 3)	VPointLine(pDom, dSta, dDist + dTerm,  TRUE);	// BOTTOM
	else							return;

	m_CurDimPoint = CDPoint(dSta, dDist);

	double dLength = str.GetLength();

	dLength *= 250 ;		// ġ���� �۾�ũ�� ���� 270

//	nCol-=1;

	if(str != "") DimOneLineTo(pDom, dSta, dDist, dLength, nCol, str);
}

//<summary>
//
//</summary>
//<param name="*pDom"></param>
//<param name="dSta"></param>
//<param name="dDistCntr"></param>
//<param name="dDist"></param>
//<param name="nCol"></param>
//<param name="str"></param>
//<param name="bLeft"></param>
void CAPlateDrawPyung::DimOneLineTo(CDomyun *pDom,double dSta, double dDistCntr, double dDist, long nCol, CString str, BOOL bLeft)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(m_nGirder);

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	if(dDist == 0 )	return;

	nCol += 1;

	long nSwitch = -1;

	double dGap = 50;

	CDPoint xyStt(0,0);
	CDPoint xyEnd(0,0);
	CDPoint vAng(0,0);

	pOptStd->SetEnvType(pDom, HCAD_DIML);

	double dAngle = 0;

	if(bLeft)
	{
		pDom->SetTextAlignHorz(TA_LEFT);
	}
	else
	{
		pDom->SetTextAlignHorz(TA_RIGHT);

		nSwitch = 1;
	}

	double dTerm  = 1000 * nCol;

	if(m_CurDirection == 0)		// LEFT
	{
		vAng = pGirder->GetLine()->GetAngleAzimuth(dSta);

		xyStt = pGirder->GetXyGirderDis(dSta, dDistCntr) - vAng * dTerm;
		xyEnd = xyStt + vAng.Rotate90() * (dDist * nSwitch);

		pDom->LineTo(xyStt, xyEnd);

		dAngle = vAng.Rotate90().GetAngleDegree();

		pDom->SetTextAngle(dAngle);

		xyEnd = xyEnd - vAng * dGap;

		pDom->TextOut(xyEnd, str);
	}
	else if(m_CurDirection == 1)	// TOP
	{
		vAng = pGirder->GetLine()->GetAngleAzimuth(dSta);

		xyStt = pGirder->GetXyGirderDis(dSta, dDistCntr - dTerm);
		xyEnd = xyStt + vAng * (dDist * nSwitch);

		pDom->LineTo(xyStt, xyEnd);

		dAngle = vAng.GetAngleDegree();

		pDom->SetTextAngle(dAngle);

		xyEnd = xyEnd + vAng.Rotate90() * dGap;

		pDom->TextOut(xyEnd, str);
	}
	else if(m_CurDirection == 2)	// RIGHT
	{
		vAng  = pGirder->GetLine()->GetAngleAzimuth(dSta);

		xyStt = pGirder->GetXyGirderDis(dSta, dDistCntr) + vAng * dTerm;
		xyEnd = xyStt + vAng.Rotate90() * (dDist * nSwitch);

		pDom->LineTo(xyStt, xyEnd);

		dAngle = vAng.Rotate90().GetAngleDegree();

		pDom->SetTextAngle(dAngle);

		xyEnd = xyEnd - vAng * dGap;

		pDom->TextOut(xyEnd, str);
	}
	else if(m_CurDirection==3)	// BOTTOM
	{
		vAng  = pGirder->GetLine()->GetAngleAzimuth(dSta);

		xyStt = pGirder->GetXyGirderDis(dSta, dDistCntr + dTerm);
		xyEnd = xyStt + vAng * (dDist * nSwitch);

		pDom->LineTo(xyStt, xyEnd);

		dAngle = vAng.GetAngleDegree();

		pDom->SetTextAngle(dAngle);

		xyEnd = xyEnd + vAng.Rotate90() * dGap;

		pDom->TextOut(xyEnd, str);
	}
}

//<summary>
// ���� ġ�� ���� || ���ۼ�
//</summary>
//<param name="*pDom"></param>
//<param name="dStaBase"></param>
//<param name="dTerm"></param>
//<param name="dDist"></param>
//<param name="bUpper"></param>
void CAPlateDrawPyung::HPointLine(CDomyun *pDom, double dStaBase, double dTerm, double dDist, BOOL bUpper)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(m_nGirder);

	CDPoint xyStt(0,0);
	CDPoint xyEnd(0,0);
	CDPoint xyCircle(0,0);

	CDPoint xyBase = pGirder->GetXyGirderDis(dStaBase, dDist);
	CDPoint vAng   = pGirder->GetLine()->GetAngleAzimuth(dStaBase);

	if(bUpper)	vAng = -vAng;

	xyStt	 = xyBase + vAng * (dTerm + 120);
	xyEnd	 = xyBase + vAng * (dTerm + 1120);
	xyCircle = xyBase + vAng * (dTerm + 1000);

	pDom->LineTo(xyStt, xyEnd);
	pDom->Circle(xyCircle, 60, TRUE);
}

//<summary>
// ���� ġ�� ���� || ���ۼ�
//</summary>
//<param name="*pDom"></param>
//<param name="dStaBase"></param>
//<param name="dDist"></param>
//<param name="bLeft"></param>
void CAPlateDrawPyung::VPointLine(CDomyun *pDom, double dStaBase, double dDist, BOOL bLeft)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(m_nGirder);

	CDPoint xyStt(0,0);
	CDPoint xyEnd(0,0);
	CDPoint xyCircle(0,0);

	CDPoint xyBase = pGirder->GetXyGirderDis(dStaBase, dDist);
	CDPoint vAng   = pGirder->GetLine()->GetAngleAzimuth(dStaBase).Rotate90();

	if(bLeft)	vAng *= -1;

	xyStt	 = xyBase + vAng *  120;
	xyEnd	 = xyBase + vAng * 1120;
	xyCircle = xyBase + vAng * 1000;

	pDom->LineTo(xyStt, xyEnd);
	pDom->Circle(xyCircle, 60, TRUE);
}

//<summary>
// ġ���� ����(����)
//</summary>
//<param name="*pDom"></param>
//<param name="dDist"></param>
//<param name="nCol"></param>
//<param name="str1"></param>
//<param name="str2"></param>
//<param name="bEndDraw"></param>
// bTextUp : m_CurDirection=3�϶� �ؽ�Ʈ�� ���� �����ΰ� �Ʒ��� �����ΰ�
void CAPlateDrawPyung::DimLineTo(CDomyun *pDom, double dDist, long nCol, CString str1, CString str2, BOOL bEndDraw, BOOL bTextUp)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(m_nGirder);

	if(dDist == 0) return;

	pDom->SetTextAlignHorz(TA_CENTER);

	double dTerm = 1000 * (nCol + 1);
	double dGap	 = 50;

	CDPoint xyPrev = m_CurDimPoint;

	CDPoint xyStt(0,0);
	CDPoint xyEnd(0,0);

	CDPoint vAng(0,0);

	double dAngle = 0;

	if(m_CurDirection == 0)	// Left
	{
		if(bEndDraw)	DimMoveTo(pDom, xyPrev.x, xyPrev.y + dDist, nCol);
		else			m_CurDimPoint = CDPoint(xyPrev.x, xyPrev.y + dDist);

		vAng  = pGirder->GetLine()->GetAngleAzimuth(xyPrev.x);

		xyStt = pGirder->GetXyGirderDis(xyPrev.x, xyPrev.y) - vAng * dTerm;
		xyEnd = pGirder->GetXyGirderDis(xyPrev.x, xyPrev.y + dDist) - vAng * dTerm;

		pDom->LineTo(xyStt, xyEnd);

		dAngle = vAng.Rotate90().GetAngleDegree();

		pDom->SetTextAngle(dAngle);

		xyEnd = (xyStt + xyEnd) / 2 - vAng * dGap;

		pDom->TextOut(xyEnd, str1);
	}
	else if(m_CurDirection == 1)  // Top
	{
		if(bEndDraw)	DimMoveTo(pDom, xyPrev.x+dDist, xyPrev.y, nCol);
		else			m_CurDimPoint = CDPoint(xyPrev.x+dDist, xyPrev.y);

		pGirder->GetLine()->DrawLS(pDom, xyPrev.x, xyPrev.x + dDist, xyPrev.y - dTerm + pGirder->m_dGirderCenter);

		dAngle = pGirder->GetLine()->GetAngleAzimuth(xyPrev.x + dDist/2).GetAngleDegree();

		pDom->SetTextAngle(dAngle);

		xyEnd = pGirder->GetXyGirderDis(xyPrev.x + dDist/2, xyPrev.y - dTerm-dGap);

		pDom->TextOut(xyEnd, str1);
	}
	else if(m_CurDirection == 2)  // Right
	{
		if(bEndDraw)	DimMoveTo(pDom, xyPrev.x, xyPrev.y + dDist, nCol);
		else			m_CurDimPoint = CDPoint(xyPrev.x, xyPrev.y + dDist);

		vAng  = pGirder->GetLine()->GetAngleAzimuth(xyPrev.x);

		xyStt = pGirder->GetXyGirderDis(xyPrev.x, xyPrev.y) + vAng * dTerm;
		xyEnd = pGirder->GetXyGirderDis(xyPrev.x, xyPrev.y + dDist) + vAng * dTerm;

		pDom->LineTo(xyStt, xyEnd);

		dAngle = vAng.Rotate90().GetAngleDegree();

		pDom->SetTextAngle(dAngle);

		xyEnd = (xyStt + xyEnd) / 2 - vAng * dGap;

		pDom->TextOut(xyEnd, str1);
	}
	else if(m_CurDirection == 3) // Bottom
	{
		if(bEndDraw)	DimMoveTo(pDom, xyPrev.x + dDist, xyPrev.y, nCol);
		else			m_CurDimPoint = CDPoint(xyPrev.x + dDist, xyPrev.y);

		pGirder->GetLine()->DrawLS(pDom, xyPrev.x, xyPrev.x + dDist, xyPrev.y + dTerm + pGirder->m_dGirderCenter);

		dAngle = pGirder->GetLine()->GetAngleAzimuth(xyPrev.x + dDist/2).GetAngleDegree();

		pDom->SetTextAngle(dAngle);
		
		double dPlus = 0;
		if(bTextUp==FALSE)
			dPlus = pDom->GetTextHeight()+pDom->Always(1);

		xyEnd = pGirder->GetXyGirderDis(xyPrev.x + dDist/2, xyPrev.y + dTerm - dGap + dPlus);

		pDom->TextOut(xyEnd, str1);
	}

	double dLength = 250 * str2.GetLength();		// ġ���� �۾�ũ�� ���� 270

	if(str1 != "")	DimOneLineTo(pDom, m_CurDimPoint.x, m_CurDimPoint.y, dLength, nCol, str2, FALSE);
}

void CAPlateDrawPyung::DimLineToOver(CDomyun *pDom, double dDist, long nCol, CString str1, CString str2)
{
	if(str1 == _T(""))
	{
		DimLineTo(pDom, dDist, nCol, "");
	}
	else
	{
		DimLineTo(pDom, dDist, nCol, "", str2);
		DimLineTo(pDom, str1.GetLength() * 270, nCol, str1, str2, FALSE);
	}
}

void CAPlateDrawPyung::TextCircle(CDomyun *pDom, long nGirder, double dSta, CDPoint &vAng, double dDist, CString &str)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirder = pBridge->GetGirder(nGirder);

	CDPoint xy = pGirder->GetXyGirderDis(dSta, dDist, vAng);

	double dAng = pGirder->GetLine()->GetAngleAzimuth(dSta).GetAngleRadian();

	pDom->TextCircle(xy.x, xy.y, str, -1, FALSE, FALSE, dAng);	
}


void CAPlateDrawPyung::DrawInputDetSplicePyung(CDomyun *pDom, long nG, long nSp)
{	
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp  *pGir	  = pBridge->GetGirder(nG);
	CPlateBasicIndex *pBx     = pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice	 *pSp     = pBx->GetSplice();
	
	CDomyun Dom(pDom);
	Dom.SetScaleDim(20);
	//
	DrawUpperFlangeSplice(&Dom, pBx, 0, 0);	
	Dom.SetCalcExtRect();
	DrawLowerFlangeSplice(&Dom, pBx, 0, Dom.GetExtRect().Height());

	*pDom << Dom;
} 

void CAPlateDrawPyung::DrawGirderForSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bDivUpDn)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir	 = (CPlateGirderApp*) pBx->GetGirder();
	CPlateSpliceBase	*pSp = pBx->GetSplice();
	if(!pSp)	return;
	
	double dLenPlusU	= (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1;
	double dLenPlusD	= (pSp->m_dA+pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1;
	double dLenPlus		= bUpper ? dLenPlusU : dLenPlusD;
	double dGirWPre		= pGir->GetWidthOnStation(pBx->GetStation(), bUpper, FALSE);
	double dGirWNex		= pGir->GetWidthOnStation(pBx->GetStation(), bUpper, TRUE);
	double dThickPre	= pBx->GetFactChain(G_W)?pBx->GetFactChain(G_W, FALSE)->m_dFactChainThick:0;
	double dThickNex	= pBx->GetFactChain(G_W)?pBx->GetFactChain(G_W, TRUE)->m_dFactChainThick:0;
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	dLenPlus += Dom.Always(10);
	// 1:2.5
	CDPoint xyUL	= CDPoint(-dLenPlus/2,  dGirWPre/2);
	CDPoint xyDL	= CDPoint(-dLenPlus/2, -dGirWPre/2);
	CDPoint xyUR	= CDPoint( dLenPlus/2,  dGirWNex/2);
	CDPoint xyDR	= CDPoint( dLenPlus/2, -dGirWNex/2);
	CDPoint xyUC	= CDPoint(0,  min(dGirWPre, dGirWNex)/2);
	CDPoint xyDC	= CDPoint(0, -min(dGirWPre, dGirWNex)/2);
	CDPoint vX(1,0), vY(0,1), xyUT(0,0), xyDT(0,0), vAng(0,0);

	// �÷��� ��
	Dom.LineTo(xyUC, xyDC);

	if(dGirWPre!=dGirWNex)
	{
		if(dGirWPre<dGirWNex)
		{			
			vAng = CDPoint(2.5,  1).Unit();
			GetXyMatchLineAndLine(xyUC, vAng, xyUR, vX, xyUT);
			vAng = CDPoint(2.5, -1).Unit();
			GetXyMatchLineAndLine(xyDC, vAng, xyDR, vX, xyDT);
			Dom.LineTo(xyUL, xyUC);
			Dom.LineTo(xyDL, xyDC);
			Dom.LineTo(xyUC, xyUT);
			Dom.LineTo(xyDC, xyDT);
			Dom.LineTo(xyUT, xyUR);
			Dom.LineTo(xyDT, xyDR);
		}
		else
		{
			vAng = CDPoint(-2.5,  1).Unit();
			GetXyMatchLineAndLine(xyUC, vAng, xyUL, vX, xyUT);
			vAng = CDPoint(-2.5, -1).Unit();
			GetXyMatchLineAndLine(xyDC, vAng, xyDL, vX, xyDT);
			Dom.LineTo(xyUL, xyUT);
			Dom.LineTo(xyDL, xyDT);
			Dom.LineTo(xyUT, xyUC);
			Dom.LineTo(xyDT, xyDC);
			Dom.LineTo(xyUC, xyUR);
			Dom.LineTo(xyDC, xyDR);
		}		
	}
	else
	{
		Dom.LineTo(xyUL, xyUR);
		Dom.LineTo(xyDL, xyDR);
	}

	Dom.TrimByLineOnSide(CDPoint(xyDL.x, min(xyDL.y, xyDR.y)-Dom.Always(3)), CDPoint(xyUL.x, max(xyUL.y, xyUR.y)+Dom.Always(3)), -1);
	Dom.TrimByLineOnSide(CDPoint(xyDR.x, min(xyDL.y, xyDR.y)-Dom.Always(3)), CDPoint(xyUR.x, max(xyUL.y, xyUR.y)+Dom.Always(3)),  1);

	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(xyDL.x, min(xyDL.y, xyDR.y)-Dom.Always(3), xyUL.x, max(xyUL.y, xyUR.y)+Dom.Always(3), TRUE, 1);
	Dom.CutLightning(xyDR.x, min(xyDL.y, xyDR.y)-Dom.Always(3), xyUR.x, max(xyUL.y, xyUR.y)+Dom.Always(3), TRUE, 1);

	// ������
	if(!bDivUpDn && bUpper)	pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
	else					pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_SOLID);
	xyUL	= CDPoint(-dLenPlus/2,  dThickPre/2);
	xyDL	= CDPoint(-dLenPlus/2, -dThickPre/2);
	xyUR	= CDPoint( dLenPlus/2,  dThickNex/2);
	xyDR	= CDPoint( dLenPlus/2, -dThickNex/2);
	xyUC	= CDPoint(0,  max(dThickPre, dThickNex)/2);
	xyDC	= CDPoint(0, -max(dThickPre, dThickNex)/2);
	Dom.LineTo(xyUL, CDPoint(0,  dThickPre/2));
	Dom.LineTo(xyDL, CDPoint(0, -dThickPre/2));
	if(bDivUpDn)
		pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
	Dom.LineTo(xyUR, CDPoint(0,  dThickNex/2));
	Dom.LineTo(xyDR, CDPoint(0, -dThickNex/2));

	Dom.Move(0, -min(dGirWPre, dGirWNex)/2);
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawUpperFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bTitle, BOOL bExcel, BOOL bPaintMap, BOOL bDim)
{
	CPlateGirderApp* pGir	 = (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice    *pSp     = (CPlateSplice*)pBx->GetSplice();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pOpt	 = m_pDataMng->GetGlobalOption();

	if(pSp==NULL)	return;
	//��λ���������
	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CPlatePieceObject pObj(pGir);
	pObj.m_nMarkBuje = SP_U_U;
	pSp->CreatePlatePieceObj(&pObj, TRUE, TRUE, 0, TRUE);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//�������������1
	pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
	CPlatePieceObject pObj1(pGir);
	pObj1.m_nMarkBuje = SP_U_L;
	pSp->CreatePlatePieceObj(&pObj1, TRUE, TRUE, -1, TRUE);
	pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//�������������2
	pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
	CPlatePieceObject pObj2(pGir);
	pObj2.m_nMarkBuje = SP_U_L;
	pSp->CreatePlatePieceObj(&pObj2, TRUE, TRUE, 1, TRUE);
	pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);

	DrawGirderForSplice(&Dom, pBx, TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	if(bTitle)	Dom.TextOut(0, Dom.Always(8), _T("��� �÷���"));
	if(bDim)	DimUpperSplice(&Dom, pBx, bExcel, bPaintMap);

	CAPlateDrawEtc DrawEtc(m_pDataMng);
	if(pOpt->GetDesignHeigtBaseUpper())
	{
		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_UPPERFLANGE, &pObj);
	}
	else
	{
		for(long n=0; n<2; n++)
		{
			if(n==0)	DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_UPPERFLANGE, &pObj1);
			else		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_UPPERFLANGE, &pObj2);
		}
	}
	
	*pDom << Dom;

	pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
	// ������1	
	CPlatePieceObject pObjWeb1(pGir);
	pObjWeb1.m_nMarkBuje = SP_W_PLAN;
	pSp->CreatePlatePieceObj(&pObjWeb1, FALSE, FALSE, -1, TRUE);
	pObjWeb1.DrawPieceOutline(&Dom, pOptStd, TRUE);
	// ������2
	CPlatePieceObject pObjWeb2(pGir);
	pObjWeb2.m_nMarkBuje = SP_W_PLAN;
	pSp->CreatePlatePieceObj(&pObjWeb2, FALSE, FALSE, 1, TRUE);
	pObjWeb2.DrawPieceOutline(&Dom, pOptStd, TRUE);

	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_WEB, bNextFiller))
	{
		CPlatePieceObject pObjWebFiller1(pGir);
		pObjWebFiller1.m_nMarkBuje = SP_W_PLAN_F;
		pSp->CreatePlatePieceObj(&pObjWebFiller1, FALSE, FALSE, -1, TRUE);
		CPlatePieceObject pObjWebFiller2(pGir);
		pObjWebFiller2.m_nMarkBuje = SP_W_PLAN_F;
		pSp->CreatePlatePieceObj(&pObjWebFiller2, FALSE, FALSE,  1, TRUE);

		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObjWebFiller1, TRUE);
		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObjWebFiller2, TRUE);		
	}

	double gWPre		= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double gWNext		= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double gW			= min(gWPre, gWNext);
	
	Dom.Move(0, -gW/2);		
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawLowerFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bTitle, BOOL bExcel, BOOL bPaintMap, BOOL bDim)
{
	CPlateGirderApp* pGir	 = (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice*    pSp     = (CPlateSplice*)pBx->GetSplice();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pOpt	 = m_pDataMng->GetGlobalOption();

	if(pSp==NULL)	return;
	CDomyun Dom(pDom);

	//�Ϻ�����
	pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
	CPlatePieceObject pObj(pGir);
	pObj.m_nMarkBuje = SP_L_L;
	pSp->CreatePlatePieceObj(&pObj, TRUE, TRUE, 0, TRUE);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//�Ϻλ���1
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CPlatePieceObject pObj1(pGir);
	pObj1.m_nMarkBuje = SP_L_U;
	pSp->CreatePlatePieceObj(&pObj1, TRUE, TRUE, -1, TRUE);
	pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//�Ϻλ���2
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CPlatePieceObject pObj2(pGir);
	pObj2.m_nMarkBuje = SP_L_U;
	pSp->CreatePlatePieceObj(&pObj2, TRUE, TRUE, 1, TRUE);
	pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);

	DrawGirderForSplice(&Dom, pBx, FALSE);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	if(bTitle)	Dom.TextOut(0, Dom.Always(8), _T("�Ϻ� �÷���"));
	if(bDim)	DimLowerSplice(&Dom, pBx, bExcel, bPaintMap);

	CAPlateDrawEtc DrawEtc(m_pDataMng);
	if(!pOpt->GetDesignHeigtBaseLower())
	{
		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_LOWERFLANGE, &pObj);
	}
	else
	{
		for(long n=0; n<2; n++)
		{
			if(n==0)	DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_LOWERFLANGE, &pObj1);
			else		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_LOWERFLANGE, &pObj2);
		}
	}

	if(!bPaintMap)
	{
		if(BY != 0)		Dom.Move(0, -BY-Dom.Always(5));
		else			Dom.Move(0, -1300);
	}

	*pDom << Dom;

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	// ������1	
	CPlatePieceObject pObjWeb1(pGir);
	pObjWeb1.m_nMarkBuje = SP_W_PLAN;
	pSp->CreatePlatePieceObj(&pObjWeb1, FALSE, FALSE, -1, TRUE);
	pObjWeb1.DrawPieceOutline(&Dom, pOptStd, TRUE);
	// ������2
	CPlatePieceObject pObjWeb2(pGir);
	pObjWeb2.m_nMarkBuje = SP_W_PLAN;
	pSp->CreatePlatePieceObj(&pObjWeb2, FALSE, FALSE, 1, TRUE);
	pObjWeb2.DrawPieceOutline(&Dom, pOptStd, TRUE);

	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_WEB, bNextFiller))
	{
		CPlatePieceObject pObjWebFiller1(pGir);
		pObjWebFiller1.m_nMarkBuje = SP_W_PLAN_F;
		pSp->CreatePlatePieceObj(&pObjWebFiller1, FALSE, FALSE, -1, TRUE);
		CPlatePieceObject pObjWebFiller2(pGir);
		pObjWebFiller2.m_nMarkBuje = SP_W_PLAN_F;
		pSp->CreatePlatePieceObj(&pObjWebFiller2, FALSE, FALSE,  1, TRUE);

		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObjWebFiller1, TRUE);
		DrawEtc.DrawFillerSpliceRect(&Dom, pBx, BX_WEB, &pObjWebFiller2, TRUE);		
	}

	double gWPre		= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double gWNext		= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double gW			= min(gWPre, gWNext);
	
	Dom.Move(0, -gW/2);	
	if(!bPaintMap)
	{
		if(BY != 0)		Dom.Move(0, -BY-Dom.Always(5));
		else			Dom.Move(0, -1300);
	}
	*pDom << Dom;
}

void CAPlateDrawPyung::DimUpperSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bExcel, BOOL bPaintMap)
{
	CPlateGirderApp* pGir	 = (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice         *pSp     = pBx->GetSplice();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	if(pBx == NULL) return;

	double dWGirPre = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double dWGirNex = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double dWidthGir = min(dWGirPre, dWGirNex);
	double sWidth    = (pSp->m_uA+pSp->m_uB*pSp->m_uC)*2+pSp->m_uC1;//	���ö��̽� ��
	//���鼳��
	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetScaleDim(20);
	double DimOff = Dom.Always(5);
	if(bExcel)
		Dom.SetTextHeight(Dom.Always(0.2));
	
	if(bPaintMap)//����󼼵����� �׸��°Ÿ�..
	{
		long nDimDan =0;
		Dom.SetDirection("RIGHT");		
		Dom.DimMoveTo(sWidth/2, -dWidthGir, nDimDan);
		Dom.DimLineTo(dWidthGir, nDimDan, "L");
		//�Ϻ�ġ����
		Dom.SetDirection("TOP");		
		Dom.DimMoveTo(-sWidth/2, 0, nDimDan);
		Dom.DimLineTo(sWidth, nDimDan, "W", TRUE);
		*pDom << Dom;
		return;
	}
	//���� ġ����
	if(!bExcel)
	{
		Dom.SetDirection("RIGHT");
		long nDimDan =0;
		double dS = (pSp->m_uW - pSp->m_uE * pSp->m_uF)/2;
	//	double dS = pSp->m_uA;		///< �ּҿ��ܰŸ� 40mm�� �Ѵ�.
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, COMMA(dS));
		Dom.DimLineTo(dS, nDimDan, "", 1);
		Dom.DimLineToExtend((pSp->m_uE*pSp->m_uF), nDimDan, "", "uE@uF");
		Dom.DimLineTo(dWidthGir-2*(dS+pSp->m_uE*pSp->m_uF), nDimDan, COMMA(dWidthGir-2*(dS+pSp->m_uE*pSp->m_uF)));
		Dom.DimLineToExtend((pSp->m_uE*pSp->m_uF), nDimDan, "", "uE@uF");
		Dom.DimLineTo(dS, nDimDan, COMMA(dS));
		nDimDan = 1;
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, "", FALSE);
		Dom.DimLineTo(dWidthGir, nDimDan, COMMA(dWidthGir));
		//�Ϻ�ġ����
		Dom.SetDirection("BOTTOM");
		nDimDan =0;

		Dom.DimMoveTo(-sWidth/2, -dWidthGir, nDimDan, "uA");
		Dom.DimLineTo(pSp->m_uA, nDimDan, "", TRUE);
		Dom.DimLineToExtend(pSp->m_uB*pSp->m_uC, nDimDan, "", "uB@uC");
		Dom.DimLineTo(pSp->m_uC1, nDimDan, "uC1", TRUE);
		Dom.DimLineToOver(0, nDimDan, "", 1);
		//����ġ����
		Dom.SetDirection("LEFT");
		nDimDan =0;
		Dom.DimMoveTo(-sWidth/2-DimOff, 0, nDimDan);
		Dom.DimLineTo(-pSp->m_uW, nDimDan, "uW", TRUE);
		Dom.DimLineTo(-(dWidthGir-pSp->m_uW*2), nDimDan, COMMA(dWidthGir-pSp->m_uW*2), TRUE);
		Dom.DimLineTo(-pSp->m_uW, nDimDan, "uW", TRUE);
		
		CDPoint Base;
		Base.x	= -sWidth/2;
		Base.y	= -dWidthGir/2;
		Dom.TextOut(Base.x-200, Base.y, "uT");
	}
	else
	{
		Dom.SetDirection("RIGHT");
		long nDimDan =0;
		double dS = (pSp->m_uW - pSp->m_uE * pSp->m_uF)/2;
	//	double dS = pSp->m_uA;		///< �ּҿ��ܰŸ� 40mm�� �Ѵ�.
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, COMMA(dS));
		Dom.DimLineTo(dS, nDimDan, "", 1);
		Dom.DimLineToExtend((pSp->m_uE*pSp->m_uF), nDimDan, "", COMMA(pSp->m_uE)+"@"+COMMA(pSp->m_uF));
		Dom.DimLineTo(dWidthGir-2*(dS+pSp->m_uE*pSp->m_uF), nDimDan, COMMA(dWidthGir-2*(dS+pSp->m_uE*pSp->m_uF)));
		Dom.DimLineToExtend((pSp->m_uE*pSp->m_uF), nDimDan, "", COMMA(pSp->m_uE)+"@"+COMMA(pSp->m_uF));
		Dom.DimLineTo(dS, nDimDan, COMMA(dS));
		nDimDan = 1;
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, "", FALSE);
		Dom.DimLineTo(dWidthGir, nDimDan, COMMA(dWidthGir));
		//�Ϻ�ġ����
		Dom.SetDirection("BOTTOM");
		nDimDan =0;

		Dom.DimMoveTo(-sWidth/2, -dWidthGir, nDimDan, COMMA(pSp->m_uA));
		Dom.DimLineTo(pSp->m_uA, nDimDan, "", TRUE);
		Dom.DimLineToExtend(pSp->m_uB*pSp->m_uC, nDimDan, "", COMMA(pSp->m_uB)+"@"+COMMA(pSp->m_uC));
		Dom.DimLineTo(pSp->m_uC1, nDimDan, COMMA(pSp->m_uC1), TRUE);
		Dom.DimLineToOver(0, nDimDan, "", 1);
		//����ġ����
		Dom.SetDirection("LEFT");
		nDimDan =0;
		Dom.DimMoveTo(-sWidth/2-DimOff, 0, nDimDan);
		Dom.DimLineTo(-pSp->m_uW, nDimDan, COMMA(pSp->m_uW), TRUE);
		Dom.DimLineTo(-(dWidthGir-pSp->m_uW*2), nDimDan, COMMA(dWidthGir-pSp->m_uW*2), TRUE);
		Dom.DimLineTo(-pSp->m_uW, nDimDan, COMMA(pSp->m_uW), TRUE);
	}
	*pDom << Dom;
}


void CAPlateDrawPyung::DimLowerSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bExcel, BOOL bPaintMap)
{
	CPlateGirderApp* pGir	 = (CPlateGirderApp*) pBx->GetGirder();
	CPlateSplice         *pSp     = pBx->GetSplice();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	if(pBx == NULL) return;
	double dWGirPre = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double dWGirNex = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double dWidthGir = min(dWGirPre, dWGirNex);
	double sWidth    = (pSp->m_dA+pSp->m_dB*pSp->m_dC)*2+pSp->m_dC1;//	���ö��̽� ��
	//���鼳��
	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetScaleDim(20);
	double DimOff = Dom.Always(5);
	if(bExcel)
		Dom.SetTextHeight(Dom.Always(0.2));

	if(bPaintMap)//����󼼵����� �׸��°Ÿ�..
	{
		long nDimDan =0;
		Dom.SetDirection("RIGHT");		
		Dom.DimMoveTo(sWidth/2, -dWidthGir, nDimDan);
		Dom.DimLineTo(dWidthGir, nDimDan, "L");
		//�Ϻ�ġ����
		Dom.SetDirection("TOP");		
		Dom.DimMoveTo(-sWidth/2, 0, nDimDan);
		Dom.DimLineTo(sWidth, nDimDan, "W", TRUE);
		*pDom << Dom;
		return;
	}

	if(!bExcel)
	{
	//����ġ����
		Dom.SetDirection("RIGHT");
		long nDimDan =0;
		double dS = (pSp->m_dW-pSp->m_dE*pSp->m_dF)/2;
	//	double dS = pSp->m_dA;	///< �ּҿ��ܰŸ�
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, COMMA(dS));
		Dom.DimLineTo(dS, nDimDan, "", 1);
		Dom.DimLineToExtend((pSp->m_dE*pSp->m_dF), nDimDan, "", "dE@dF");
		Dom.DimLineTo(dWidthGir-2*(dS+pSp->m_dE*pSp->m_dF), nDimDan, COMMA(dWidthGir-2*(dS+pSp->m_dE*pSp->m_dF)));
		Dom.DimLineToExtend((pSp->m_dE*pSp->m_dF), nDimDan, "", "dE@dF");
		Dom.DimLineToOver(dS, nDimDan, COMMA(dS), TRUE);

		nDimDan = 1;
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, "", FALSE);
		Dom.DimLineTo(dWidthGir, nDimDan, COMMA(dWidthGir));
		//�Ϻ�ġ����
		Dom.SetDirection("BOTTOM");
		nDimDan =0;
		Dom.DimMoveTo(-sWidth/2, -dWidthGir, nDimDan, "dA");
		Dom.DimLineTo(pSp->m_dA, nDimDan, "", TRUE);
		Dom.DimLineToExtend(pSp->m_dB*pSp->m_dC, nDimDan, "", "dB@dC", TRUE);
		Dom.DimLineTo(pSp->m_dC1, nDimDan, "dC1", TRUE);
		Dom.DimLineToOver(0, nDimDan, "", 1);
		//����ġ����
		Dom.SetDirection("LEFT");
		nDimDan =0;
		Dom.DimMoveTo(-sWidth/2-DimOff, 0, nDimDan);
		Dom.DimLineTo(-pSp->m_dW, nDimDan, "dW", TRUE);
		Dom.DimLineTo(-(dWidthGir-pSp->m_dW*2), nDimDan, COMMA(dWidthGir-pSp->m_dW*2), TRUE);
		Dom.DimLineTo(-pSp->m_dW, nDimDan, "dW", TRUE);
		
		CDPoint Base;
		Base.x	= -sWidth/2;
		Base.y	= -dWidthGir/2;
		Dom.TextOut(Base.x-200, Base.y, "dT");
	}
	else
	{
		Dom.SetDirection("RIGHT");
		long nDimDan =0;
		double dS = (pSp->m_dW-pSp->m_dE*pSp->m_dF)/2;
	//	double dS = pSp->m_dA;	///< �ּҿ��ܰŸ�
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, COMMA(dS));
		Dom.DimLineTo(dS, nDimDan, "", 1);
		Dom.DimLineToExtend((pSp->m_dE*pSp->m_dF), nDimDan, "", COMMA(pSp->m_dE)+"@"+COMMA(pSp->m_dF));
		Dom.DimLineTo(dWidthGir-2*(dS+pSp->m_dE*pSp->m_dF), nDimDan, COMMA(dWidthGir-2*(dS+pSp->m_dE*pSp->m_dF)));
		Dom.DimLineToExtend((pSp->m_dE*pSp->m_dF), nDimDan, "", COMMA(pSp->m_dE)+"@"+COMMA(pSp->m_dF));
		Dom.DimLineToOver(dS, nDimDan, COMMA(dS), TRUE);

		nDimDan = 1;
		Dom.DimMoveTo(sWidth/2+DimOff, -dWidthGir, nDimDan, "", FALSE);
		Dom.DimLineTo(dWidthGir, nDimDan, COMMA(dWidthGir));
		//�Ϻ�ġ����
		Dom.SetDirection("BOTTOM");
		nDimDan =0;
		Dom.DimMoveTo(-sWidth/2, -dWidthGir, nDimDan, COMMA(pSp->m_dA));
		Dom.DimLineTo(pSp->m_dA, nDimDan, "", TRUE);
		Dom.DimLineToExtend(pSp->m_dB*pSp->m_dC, nDimDan, "", COMMA(pSp->m_dB)+"@"+COMMA(pSp->m_dC), TRUE);
		Dom.DimLineTo(pSp->m_dC1, nDimDan, COMMA(pSp->m_dC1), TRUE);
		Dom.DimLineToOver(0, nDimDan, "", 1);
		//����ġ����
		Dom.SetDirection("LEFT");
		nDimDan =0;
		Dom.DimMoveTo(-sWidth/2-DimOff, 0, nDimDan);
		Dom.DimLineTo(-pSp->m_dW, nDimDan, COMMA(pSp->m_dW), TRUE);
		Dom.DimLineTo(-(dWidthGir-pSp->m_dW*2), nDimDan, COMMA(dWidthGir-pSp->m_dW*2), TRUE);
		Dom.DimLineTo(-pSp->m_dW, nDimDan, COMMA(pSp->m_dW), TRUE);
	}
	*pDom << Dom;
}

// �����߽ɼ�, �Ŵ��߽ɼ�
// bTextRoadCenter = �����߽ɼ�;
// bTextCenterEndPos �̸� "�����߽ɼ��� ���� �������� ��. �̶� CL of Road�� �տ� ����..
void CAPlateDrawPyung::DrawCenterLine(CDomyun* pDom, long nG, double dStaStt, double dStaEnd, BOOL bTextRoadCenter, BOOL bTextRadius, BOOL bKor, BOOL bTextRoadCenterEndPos)
{
  	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	 = (CPlateGirderApp*)pBridge->GetGirder(nG);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CLineInfo	    *pLine	 = pGir->GetLine();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_CENT);

	double Height = Dom.GetTextHeight();

	double dLineStt = dStaStt - Height*3;
	double dLineEnd = dStaEnd + Height*3;
	
	pLine->DrawLS(&Dom,dLineStt,dLineEnd,pGir->m_dGirderCenter);

	CDPoint cp;
	CString str = _T("");

	if(nG==-1 && bTextRoadCenter)		// ���� �߽ɼ�(FALSE)
	{
		str = "�����߽ɼ�";
//		if(bTextSide)
			cp = pLine->GetXyLineDis(dStaStt+Height*3,pGir->m_dGirderCenter-Dom.Always(1));
//		else
//			cp = pLine->GetXyLineDis(dStaStt,pGir->m_dGirderCenter-Dom.Always(1));
		Dom.SetTextAngle(pLine->GetAngleAzimuth(dStaStt).GetAngleDegree());
		Dom.SetTextAlignHorz(TA_LEFT);
		if(bTextRoadCenterEndPos)
		{
			cp = pLine->GetXyLineDis(dStaEnd-Height*3, pGir->m_dGirderCenter-Dom.Always(1));
			Dom.SetTextAngle(pLine->GetAngleAzimuth(dStaEnd).GetAngleDegree());
			Dom.SetTextAlignHorz(TA_RIGHT);
		}
		Dom.TextOut(cp,str);
		*pDom << Dom;	
		
		if(bTextRoadCenterEndPos)	// "�����߽ɼ�"�� �ڿ� �� ��� �տ� "CL OF ROAD"�� ��.
		{
			double Height = Dom.GetTextHeight();
			CDPoint vAng = pLine->GetAngleAzimuth(dStaStt);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_LEFT);

			cp = pLine->GetXyLineDis(dStaStt-Height*4, pGir->m_dGirderCenter-Dom.Always(2));
			Dom.TextOut(cp, "C");
			Dom.TextOut(cp+vAng*Dom.GetTextHeight()/2-vAng.Rotate90()*Dom.GetTextHeight()/2, "L OF ROAD");
			Dom.Rotate(cp, CDPoint(0,1));
			*pDom << Dom;	
		}
	}

	if(bTextRadius) // "R=300000"
	{
		CDomyun DomSub(pDom);
		pOptStd->SetEnvType(&DomSub,HCAD_DIML);
		CString str = pGir->GetRadiusString(dStaEnd, 0);

		if(str != "")
		{
			double LenStr = DomSub.GetTextWidth(str)*2;
			double Dist   = pGir->m_dGirderCenter;
			pLine->DrawLS(&DomSub,dStaEnd,dStaEnd+LenStr,Dist);
			Dom << DomSub;
			//
			CDPoint xy = pGir->GetXyGirderDis(dStaEnd+LenStr,Dist-DomSub.Always(2));
			DomSub.TextOut(xy,str);
			DomSub.Rotate(xy,pLine->GetAngleAzimuth(dStaEnd+LenStr/2));
			Dom << DomSub;
		}
	}

	*pDom << Dom;	
}

void CAPlateDrawPyung::DrawTopBottomViewMark(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bKor)
{
  	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CLineInfo	    *pLine	 = pBxStt->GetGirder()->GetLine();

	CString strUp  = bKor ?  " �� ��" : "  TOP VIEW";
	CString strLow = bKor ?  "�� �� " : "BOTTOM VIEW";
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_CENT);	

	double Len    = Dom.GetTextWidth(strLow,1);
	double Height = Dom.GetTextHeight();
	pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	CDPoint vGir = pLine->GetAngleAzimuth(pBxStt->GetStation());
	

	long nGirCount = pBridge->GetGirdersu();
	long nCenterGirderNumber = (long)RoundUp(nGirCount/2,1);
	CPlateGirderApp *pGir = pBridge->GetGirder(nCenterGirderNumber);
	CPlateGirderApp *pGir2 = pBridge->GetGirder(max(nCenterGirderNumber-1, 0));

	CDPoint cp1 = pLine->GetXyLineDis(pBxStt->GetStation()-Height*2, pGir->m_dGirderCenter);
	CDPoint cp2 = pLine->GetXyLineDis(pBxStt->GetStation()-Height*2, pGir2->m_dGirderCenter);
	CDPoint cpCen = (cp1+cp2)/2;
	Dom.LineTo(cpCen, cpCen+vGir*Height*2);
	double dCenter = (pGir->m_dGirderCenter +  pGir2->m_dGirderCenter) / 2; // pGir->m_dGirderCenter;//(pBridge->GetSlabLeft()->m_dGirderCenter + pBridge->GetSlabRight()->m_dGirderCenter)/2;
	if(abs(pBridge->GetGirder(-1)->m_dGirderCenter - dCenter) > 200)
		pLine->DrawLS(&Dom,pBxStt->GetStation(),pBxEnd->GetStation(),dCenter);

	Dom.SetTextAngle(vGir.GetAngleDegree()+90);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(cpCen,strUp);
	Dom.SetTextAlignHorz(TA_RIGHT);
	Dom.TextOut(cpCen,strLow);
	//
	CDomyun DomSub(&Dom);
	DomSub.Circle(cpCen.x,cpCen.y,DomSub.Always(0.5),TRUE);
	DomSub.GiSiArrow(cpCen.x,cpCen.y,Dom.Always(1),Len,0,"");
	DomSub.Rotate(pLine->GetAngleAzimuth(pBxStt->GetStation()).Rotate90().GetAngleRadian(),cpCen.x,cpCen.y);
	Dom << DomSub;
	//
	DomSub.GiSiArrow(cpCen.x,cpCen.y,Dom.Always(1),Len,0,"");
	DomSub.Rotate(pLine->GetAngleAzimuth(pBxStt->GetStation()).RotateInv90().GetAngleRadian(),cpCen.x,cpCen.y);
	Dom << DomSub;

	*pDom << Dom;	
}

void CAPlateDrawPyung::DrawHBracingSpliceWithenBx(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bJewon, long nType)
{
	CPlateGirderApp *pGir	 = (CPlateGirderApp*)pBxStt->GetGirder();
	CAPlateDrawHBracing DrawHBracing(m_pDataMng);
	CDomyun Dom(pDom);	
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET, pBxStt, pBxEnd);		

	while(pBx)
	{
		long nOption = CAPlateDrawStandard::SIDE_ALL;
		if(!pBx->IsState(BX_STT_GIRDER) && pBx == pBxStt)
			nOption = CAPlateDrawStandard::SIDE_RIGHT;
		if(!pBx->IsState(BX_END_GIRDER) && pBx == pBxEnd)
			nOption = CAPlateDrawStandard::SIDE_LEFT;

		DrawHBracing.DrawHGussetOnHGussetBx(&Dom, pBx, TRUE, nOption, FALSE, FALSE, FALSE, FALSE, bJewon,nType);
		pBx = Finder.GetBxNext();
	}	
	*pDom << Dom;
}
void CAPlateDrawPyung::DrawHBracingBeamWithenBx(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bJewon, BOOL bHalfTrim)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	 = (CPlateGirderApp*)pBxStt->GetGirder();
	CAPlateDrawHBracing DrawHBracing(m_pDataMng);
	CDomyun Dom(pDom);	
	
	CPlateBxFinder FinderCR(pGir);
	CPlateBasicIndex *pBxCR = FinderCR.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	CPlateBasicIndex *pBxCREnd = FinderCR.GetBxLast(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd), *pBxCRNext=NULL;
	
	while(pBxCR)
	{
		pBxCRNext = pBxCR->GetBxNext(BX_CROSSBEAM_VBRACING, 2);				
		CDomyun DomSub(&Dom);
		DrawHBracing.DrawBeamOnBracingPos(&DomSub, pBxCR, bJewon);

		if(bHalfTrim)
		{					
			CLineInfoApp *pLine = m_pDataMng->GetLineInfoApp();
			CDPoint cp1 = pLine->GetXyLineDis(pBxCR->GetStation(), pBridge->GetSlabLeft()->m_dGirderCenter);
			CDPoint cp2 = pLine->GetXyLineDis(pBxCR->GetStation(), pBridge->GetSlabRight()->m_dGirderCenter);
			CDPoint cpCen = (cp1+cp2)/2;
			CDPoint vGir = pLine->GetAngleAzimuth(pBxCR->GetStation());			
			DomSub.TrimByLineOnSide(cpCen-vGir*10000,cpCen+vGir*10000,-1,TRUE);
		}
		Dom<<DomSub;

		pBxCR = pBxCRNext;

		if(pBxCRNext==NULL || pBxCRNext->GetStation()>=pBxEnd->GetStation())	break;
	}	
	*pDom << Dom;
}

// bUpper=TRUE ����÷��� ��������
// bUpper=FALSE �Ϻ��÷��� ��������
// nSide =  0  : ��(��-��)
// nSide = -1 : ��(��-��), ������ �ؽ�Ʈ
// nSide =  1 : ��(��-��), ������ �ؽ�Ʈ
// nSide = -2 : ��(��-��), ������ �ؽ�Ʈ
// nSide =  2 : ��(��-��), ������ �ؽ�Ʈ
void CAPlateDrawPyung::DrawFlangeCut(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nBujeMark, long nSide, BOOL bText)
{
	CPlateGirderApp *pGir	 = (CPlateGirderApp*)pBxStt->GetGirder();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CLineInfo	    *pLine	 = pGir->GetLine();
	CPlateBxFinder Finder(pGir);

	CDPoint poLeft, poCenter, poRight;
	CDPoint poTextL, poTextR;
	double DistLeft, DistRight, dWidth, dMaxWidth;
	__int64 bxFlag = 0;
	if     (nBujeMark==G_F_U)	bxFlag = BX_UPPERFLANGE;
	else if(nBujeMark==G_F_L)	bxFlag = BX_LOWERFLANGE;
	else if(nBujeMark==G_W)		bxFlag = BX_WEB;
	BOOL bUpper = TRUE;
	if(nBujeMark==G_F_L)	bUpper = FALSE;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag, pBxStt, pBxEnd);

	CSymbolDom SymDom(pDomP,pOptStd);	

	while(pBx)
	{			
		if(pBx->IsState(BX_STT_GIRDER) || pBx->IsState(BX_SPLICE))
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		long nSelfCount = pBx->GetFactChain(nBujeMark)->GetSelfCount();
		dWidth    = min(pGir->GetWidthOnStation(pBx->GetStation(), bUpper, FALSE),
			            pGir->GetWidthOnStation(pBx->GetStation(), bUpper, TRUE));
		dMaxWidth = max(pGir->GetWidthOnStation(pBx->GetStation(), bUpper, FALSE),
			            pGir->GetWidthOnStation(pBx->GetStation(), bUpper, TRUE));
		DistLeft  = -dWidth / 2;
		DistRight = +dWidth / 2;
		poLeft		= pGir->GetXyGirderDis(pBx->GetStation(),DistLeft, pBx->GetAngleSection());	
		poCenter	= pGir->GetXyGirderDis(pBx->GetStation(),0, pBx->GetAngleSection());	
		poRight		= pGir->GetXyGirderDis(pBx->GetStation(),DistRight, pBx->GetAngleSection());
		
		poTextL		= pGir->GetXyGirderDis(pBx->GetStation(),-dMaxWidth/2 - SymDom.Always(1), pBx->GetAngleSection());
		poTextR		= pGir->GetXyGirderDis(pBx->GetStation(),+dMaxWidth/2 + SymDom.GetTextHeight()*1.5 + SymDom.Always(1), pBx->GetAngleSection());

		double dAng = pLine->GetAngleAzimuth(pBx->GetStation()).GetAngleRadian();

		CString szMark = "";
		
		if(nBujeMark==G_F_U)	
		{
			szMark.Format("TF%d", nSelfCount);
			pOptStd->SetEnvType(&SymDom, HCAD_STLC);
		}
		else if(nBujeMark==G_F_L)	
		{
			szMark.Format("BF%d", nSelfCount);
			pOptStd->SetEnvType(&SymDom, HCAD_STLC);
		}
		else if(nBujeMark==G_W)
		{
			szMark.Format("WF%d", nSelfCount);
			pOptStd->SetEnvType(&SymDom, HCAD_STLC);
		}

		switch(nSide)
		{


		case  0:	SymDom.LineTo(poLeft,poRight);	break;
		case -1:	SymDom.LineTo(poLeft,poCenter); 	
					pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
					SymDom.TextCircle(poTextL.x,poTextL.y,szMark,0,TRUE,TRUE,dAng);
					break;
		case  1:	SymDom.LineTo(poCenter,poRight);					
					pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
					SymDom.TextCircle(poTextR.x,poTextR.y,szMark,0,TRUE,TRUE,dAng);
					break;
		case -2:	SymDom.LineTo(poLeft,poRight); 					
					pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
					SymDom.TextCircle(poTextL.x,poTextL.y,szMark,0,TRUE,TRUE,dAng);
					break;
		case  2:	SymDom.LineTo(poLeft,poRight);					
					pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
					SymDom.TextCircle(poTextR.x,poTextR.y,szMark,0,TRUE,TRUE,dAng);
					break;
		}		
		pBx = Finder.GetBxNext();
	}
	*pDomP << SymDom;
}

void CAPlateDrawPyung::DrawSpliceCut(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide, BOOL bUpper, BOOL bText)
{
	CPlateGirderApp *pGir	 = (CPlateGirderApp*)pBxStt->GetGirder();	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CLineInfo	    *pLine	 = pGir->GetLine();
	CPlateBxFinder Finder(pGir);

	CDPoint poLeft, poCenter, poRight;
	CDPoint poTextL, poTextR;
	double DistLeft, DistRight, dWidth, dMaxWidth;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);

	CSymbolDom SymDom(pDomP,pOptStd);	
	while(pBx)
	{			
		if(pBx->IsState(BX_STT_GIRDER)) 
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		dWidth    = min(pGir->GetWidthOnStation(pBx->GetStation(), bUpper, FALSE),
			            pGir->GetWidthOnStation(pBx->GetStation(), bUpper, TRUE));		
		dMaxWidth = max(pGir->GetWidthOnStation(pBx->GetStation(), nSide==-1 ? TRUE : FALSE, FALSE),
			            pGir->GetWidthOnStation(pBx->GetStation(), nSide==-1 ? TRUE : FALSE, TRUE));
		DistLeft  = -dWidth / 2;
		DistRight = dWidth / 2;
		poLeft		= pGir->GetXyGirderDis(pBx->GetStation(),DistLeft, pBx->GetAngleSection());	
		poCenter	= pGir->GetXyGirderDis(pBx->GetStation(),0, pBx->GetAngleSection());	
		poRight		= pGir->GetXyGirderDis(pBx->GetStation(),DistRight, pBx->GetAngleSection());
		
		poTextL		= pGir->GetXyGirderDis(pBx->GetStation(),-dMaxWidth/2 - SymDom.Always(1), pBx->GetAngleSection());
		poTextR		= pGir->GetXyGirderDis(pBx->GetStation(),+dMaxWidth/2 + SymDom.GetTextHeight()*1.5 + SymDom.Always(1), pBx->GetAngleSection());

		pOptStd->SetEnvType(&SymDom, HCAD_STLC);
		if(nSide==0)
			SymDom.LineTo(poLeft, poRight);			
		else if(nSide==-1)
			SymDom.LineTo(poLeft, poCenter);			
		else if(nSide== 1)
			SymDom.LineTo(poCenter, poRight);	

		if(bText)
		{
			double dAng = pLine->GetAngleAzimuth(pBx->GetStation()).GetAngleRadian();		
			CString szTxt;
			szTxt = pBx->GetSplice()->GetMarkstring();					

			pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
			SymDom.TextCircle(poTextR.x,poTextR.y,szTxt,0,TRUE,TRUE,dAng);		
		}
		pBx = Finder.GetBxNext();
	}
	*pDomP << SymDom;
}


void CAPlateDrawPyung::DrawCRBeamGassetFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftGirder, 
											 BOOL bBoltHole,BOOL bBoltLine, BOOL bMakeChip, BOOL bFillCircle)
{
	if(!pBx || !pBx->GetSection()) return;
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir		= (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam	*pC			= pBx->GetSection()->GetCrossBeam();
	
	if(pDB->GetQtyGirder()==1 || !pC)	return;

	CPlateGirderApp  *pGirR   = pGir->m_pRight;
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGirR);
	
	CDomyun	Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	double dSta		= 0;
	CDPoint BP(0,0), vGir(0,0), vRotate(0,-1);
	CDPoint xyCrL(0,0), xyCrR(0,0), vCr(0,0);
	pC->GetXyBaseLR(xyCrL, xyCrR);
	vCr = pC->GetVectorPlan();

	if(bLeftGirder)	// �����Ŵ�����Ʈ��
	{
		dSta	= pBx->GetStation();
		vGir	= pGir->GetLine()->GetAngleAzimuth(dSta);
		BP		= pGir->GetXyGirderDis(dSta);
	}
	else			// �����Ŵ�����Ʈ��
	{
		dSta	= pBxNext->GetStation();
		vGir	= pGirR->GetLine()->GetAngleAzimuth(dSta);
		BP		= pGirR->GetXyGirderDis(dSta);
	}
	
	CPlatePieceObject pObj(pGir);
	if(bLeftGirder)	pObj.m_nCol = 0;
	else			pObj.m_nCol = 1;
	
	if(bUpper)	pObj.m_nMarkBuje = CP_BR_F_U;
	else		pObj.m_nMarkBuje = CP_BR_F_L;
	
	BOOL bHBGu = FALSE;
	if(!bUpper)
	{		
		if(pC->IsLeftHBGu() && bLeftGirder)
		{
			CPlatePieceObject pObjTemp(pGir);
			CHBracingGusset *pBrGu = pBx->GetHBracingGusset();
			bHBGu = TRUE;
			pBrGu->CreatePieceObj(&pObjTemp, &pObj, FALSE,FALSE);
			pObj.DrawPieceOutline(&Dom, pOptStd, bFillCircle);
			*pDom << Dom;
		}
		if(pC->IsRightHBGu() && !bLeftGirder)
		{
			CPlatePieceObject pObjTemp(pGir->m_pRight);
			CHBracingGusset *pBrGu = pBx->GetBxMatchByCrossBeam(pGir->m_pRight)->GetHBracingGusset();
			bHBGu = TRUE;
			pBrGu->CreatePieceObj(&pObj, &pObjTemp, FALSE,FALSE);
			pObj.DrawPieceOutline(&Dom, pOptStd, bFillCircle);
			*pDom << Dom;
		}

		if(bHBGu)
			return;
	}

	if((pC->m_nLGirSpType && bLeftGirder) || (pC->m_nRGirSpType && !bLeftGirder))
		pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);	
	pObj.DrawPieceOutline(&Dom, pOptStd, bFillCircle);

	Dom.Rotate(CDPoint(0,0),vRotate);
	Dom.Move(BP);
	
	*pDom << Dom;
}

//��鵵�� ������ȣ �����ϱ�
void CAPlateDrawPyung::AddWeldPyung(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,double rotAng, BOOL bUpper, long nSide, BOOL bWeldMarkOne)
{	
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBxStt->GetGirder();	
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();	
	CPlateBxFinder Finder(pGir);	
	Finder.SetIncludeBothPos(TRUE);		
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE), pBxStt, pBxEnd), *pBxNext=NULL, *pBxSp=NULL;
	ASSERT(pBx);
			

	CAPlateSymbolDom Dom(pDom, m_pDataMng);		
	//Dom.m_pDBStd = pStd;
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	CDPoint vX(1,0), vY(0,1);

	BOOL bSttSplice = FALSE;
	while(Finder.GetBxFixNext())
	{
		BOOL bLeft = FALSE;
		pBxNext = Finder.GetBxFixNext();
		//GET STA, PO
		double Sta = (pBx->GetStation() + pBxNext->GetStation()) / 2 - (pBxNext->GetStation() - pBx->GetStation()) / 5;
		CPlateBasicIndex *pmidBx = pGir->GetBxByStation(Sta);
		if(pmidBx->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF))
			Sta = pmidBx->GetStation() + 600;
		else
			Sta = pmidBx->GetStation();

		if(Sta > pBxNext->GetStation()) Sta = (pBx->GetStation() + pBxNext->GetStation()) /2;

		pBxSp = pGir->GetBxByStation(Sta, BX_SPLICE);
		double dStaComp = pBxSp ? pBxSp->GetStation() : Sta+901;
		if((Sta - dStaComp) > -900 && (Sta - dStaComp) < 0)
			Sta	-= (900 - fabs(Sta - dStaComp)); 
		if((Sta - dStaComp) < 900 && (Sta - dStaComp) >= 0)
			Sta	+= (900 - fabs(Sta - dStaComp));

		pBx = pGir->GetBxByStation(Sta, (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE));
//		CPlateBasicIndex *pBx = pGir->GetBxByStation(Sta, (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE));
		if(!pBx) pBx = pGir->GetBxByStation(Sta, BX_END_GIRDER);
		double BaseSta = pBx->GetStation();

		if(BaseSta < pmidBx->GetStation() && BaseSta > Sta)
		{
			Sta = Sta + (BaseSta-Sta) + (pmidBx->GetStation()-BaseSta)/2;
		}
		else if(BaseSta > pmidBx->GetStation() && BaseSta < Sta)
		{
			Sta = Sta + (BaseSta-Sta) + (pmidBx->GetStation()-BaseSta)/2;
		}
		if(dStaComp < Sta)	bLeft = FALSE;
		if(dStaComp > Sta)	bLeft = TRUE;

		double dWebT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
		CDPoint Po1 = pGir->GetXyGirderDis(Sta) - vY*(dWebT/2);
				
		CPlateBxFinder Finder2(pGir);
		if(Sta > BaseSta)
		{
			__int64 Flag = BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_SPLICE | BX_END_GIRDER;
			pmidBx = Finder2.GetBxFirst(Flag, pGir->GetBxByStation(Sta, (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE)), pBxEnd);
		}
		else if(Sta < BaseSta)
		{
			__int64 Flag = BX_STT_GIRDER | BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_SPLICE;
			pmidBx = Finder2.GetBxLast(Flag, pBxStt,pGir->GetBxByStation(Sta, (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE)));
		}
		//DRAWWELD
		bSttSplice = (pBxStt==pmidBx && pmidBx->IsState(BX_SPLICE)) ? TRUE : FALSE;
		Dom.DrawWeldMark(Po1, G_W, bUpper ? G_F_U : G_F_L,pmidBx,-1, 6,0,bLeft,FALSE,rotAng,TRUE,_T(""),TRUE,bSttSplice);		

		if(bWeldMarkOne) break;
		pBx = Finder.GetBxNext();		
	}
	
	*pDom << Dom;		
}

void CAPlateDrawPyung::AddWeldPyungFlange(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,double rotAng, BOOL bUpper, long nSide, BOOL bGrind)
{
	//SET ENV
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBxStt->GetGirder();	
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();	

	CPlateBxFinder Finder(pGir);	
			
	CPlateBasicIndex* pBx = Finder.GetBxFirst(bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE, pBxStt, pBxEnd), *pBxSp=NULL;
	if(!pBx) return;
	
	CAPlateSymbolDom Dom(pDom,m_pDataMng);			
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	long nHBase = m_pDataMng->GetGlobalOption()->GetDesignHeigtBase();
	BOOL bUpperSide	= bUpper ? TRUE : FALSE;
	
	BOOL bLeft = bUpper;
	long nMarkBuje = bUpper ? G_F_U : G_F_L;

	while(pBx)
	{
		if(pBx->IsState(BX_SPLICE))
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		double dSta = pBx->GetStation();
		pBxSp = pGir->GetBxByStation(dSta, BX_SPLICE);
		double dStaComp = pBxSp ? pBxSp->GetStation() : dSta+1001;
		if((dSta - dStaComp) > -1000 && (dSta - dStaComp) < 0 && bUpper)
			bLeft = !bUpper;
		if((dSta - dStaComp) < 1000 && (dSta - dStaComp) >= 0 && !bUpper)
			bLeft = !bUpper;
		
		double dWidth	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
		CDPoint Po = pGir->GetXyGirderDis(dSta,-dWidth/4 * nSide);

		if(!pBx->GetFactChain(nMarkBuje,TRUE) || !pBx->GetFactChain(nMarkBuje,FALSE)) break;
		double dT1   = pBx->GetFactChain(nMarkBuje,TRUE)->m_dFactChainThick;
		double dT2   = pBx->GetFactChain(nMarkBuje,FALSE)->m_dFactChainThick;

		if(bUpper)
		{
			if(nHBase == 1 || nHBase ==3)
				bUpperSide = FALSE;

			if(dT1 == dT2)	bUpperSide = TRUE;			
		}
		else
		{
			if(nHBase == 1 || nHBase ==2)
				bUpperSide = TRUE;

			if(dT1 == dT2)	bUpperSide = FALSE;
		}
		//��� ����...
		//Dom.DrawWeldMark(CDPoint(dLen,dY), G_F_U, G_F_U, pBx, -1, 10, 0, bLeft, TRUE, rotAng, FALSE, _T(""), bGrindUpper, FALSE, FALSE, bUpperSide);		
		//�Ϻ� �ݴ��...
		//Dom.DrawWeldMark(CDPoint(dLen,dY), G_F_L, G_F_L, pBx, -1, 10, 0, bLeft, FALSE, rotAng, FALSE, _T(""), bGrindLower, FALSE, FALSE, bUpperSide);
		Dom.DrawWeldMark(Po, nMarkBuje, nMarkBuje, pBx, -1, 12.0, 0, bLeft ? FALSE : TRUE, bUpper, rotAng, FALSE, _T(""), bGrind, FALSE, FALSE, bUpperSide);
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;	
}


void CAPlateDrawPyung::AddWeldVStiffPos(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng, BOOL bUpper, long nSide)
{
	CPlateGirderApp	*pGir		= pBxStt->GetGirder();
	CARoadOptionStd	*pOpt		= m_pDataMng->GetOptionStd();
	CLineInfo		*pLine		= pGir->GetLine();	

	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUMSTIFF|BX_JACKUP|BX_CROSSBEAM_VBRACING ,pBxStt, pBxEnd), *pBxSp=NULL;	

	CAPlateSymbolDom Dom(pDom,m_pDataMng);

	long nMarkBuje = bUpper ? G_F_U : G_F_L;

	BOOL bDrawCB = TRUE;
	BOOL bDrawJIJUMV = TRUE;
	BOOL bDrawJackUp = TRUE;

	pOpt->SetEnvType(&Dom, HCAD_DIML);	

	while(pBx)
	{		
		if(pBx->IsState(BX_CROSSBEAM_VBRACING) && bDrawCB)
		{
			double dSta = pBx->GetStation();
			pBxSp = pGir->GetBxByStation(dSta, BX_SPLICE);
			double dStaComp = pBxSp ? pBxSp->GetStation() : dSta+1001;
			double dWidth = pGir->GetWidthOnStation(dSta, bUpper);
			if(fabs(dSta - dStaComp) < 1000)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))	// �����극�̽�
			{
				CVStiff *pVstiff = pBx->GetSection()->GetVBracing()->m_pVStiff;
				double dT   = max(pVstiff->m_dThick[0], pVstiff->m_dThick[1]);
				//double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(), dSta)-dT/2;
				CDPoint Po = pGir->GetXyGirderDis(dSta+dT/2, -dWidth/4, pBx->GetAngle());
				Dom.DrawWeldMark(Po, nMarkBuje, V_ST, pBx, -1, 6, 0, FALSE, TRUE, 0);
			}
			bDrawCB = FALSE;
		}
		else if(pBx->IsState(BX_JIJUMSTIFF) && bDrawJIJUMV)
		{
			double dSta = pBx->GetStation();
			pBxSp = pGir->GetBxByStation(dSta, BX_SPLICE);
			double dStaComp = pBxSp ? pBxSp->GetStation() : dSta+1001;
			double dWidth = pGir->GetWidthOnStation(dSta, bUpper);
			if(fabs(dSta - dStaComp) < 1000)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			CVStiff *pVstiff = pBx->GetJijumStiff();
			double dT   = max(pVstiff->m_dThick[0], pVstiff->m_dThick[1]);
			//double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(), dSta)-dT/2;
			CDPoint Po = pGir->GetXyGirderDis(dSta-dT/2, dWidth/4, pBx->GetAngle());
			Dom.DrawWeldMark(Po, V_J_ST, nMarkBuje, pBx, -1, 6, 0, TRUE, FALSE, 0);
			bDrawJIJUMV = FALSE;
		}
		else if(pBx->IsState(BX_JACKUP) && bDrawJackUp)
		{
			double dSta = pBx->GetStation();
			pBxSp = pGir->GetBxByStation(dSta, BX_SPLICE);
			double dStaComp = pBxSp ? pBxSp->GetStation() : dSta+1001;
			//double dWidth = pGir->GetWidthOnStation(dSta, bUpper);
			if(fabs(dSta - dStaComp) < 1000)
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			CJackupStiff *pJackUp = pBx->GetJackupStiff();

			if(nMarkBuje == G_F_U)//���ǿ� ��������� ���� ��쿡��..d
			{
				//�Ŵ����� ���ؿ� ���� ���̰�...
				double dCurHeightGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
				//��������� ���̰� 0�̸� �ش� �Ŵ� ���̷�...
				double dCurHeightJackUp = pJackUp->m_dVL == 0 ? dCurHeightGir : pJackUp->m_dVL;
				if (dCurHeightGir > dCurHeightJackUp)
				{	
					pBx = Finder.GetBxNext();
					continue;
				}
			}

			double dT   = max(pJackUp->m_vJewon[0].y, pJackUp->m_vJewon[1].y);
			//������ȣ ���󿡼� ���������� ���̱� ���ؼ�...06.12.20....KB
			double dW   = max(pJackUp->m_vJewon[0].x, pJackUp->m_vJewon[1].x);
			//double dLen = pBxStt->GetStation() + pGir->GetLengthDimType(pBxStt->GetStation(), dSta)-dT/2;
			CDPoint Po = pGir->GetXyGirderDis(dSta-dT/2, dW,  pBx->GetAngle());;//dWidth/4,  pBx->GetAngle());
			Dom.DrawWeldMark(Po, GI_JA_L, nMarkBuje,pBx, -1, 6, 0, TRUE, FALSE, 0);
			bDrawJackUp = FALSE;
		}
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}


// �������
void CAPlateDrawPyung::AddWeldSplicePos(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng, BOOL bUpper, long nSide, BOOL bGrind)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();		
	CPlateGirderApp	*pGir		= pBxStt->GetGirder();
	CLineInfo		*pLine		= pGir->GetLine();	
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	CAPlateSymbolDom Dom(pDom,m_pDataMng);

	BOOL bLeft = TRUE;
	long nMarkBuje = bUpper ? G_F_U : G_F_L;
	long nWeldMarkBuje = bUpper ? WELD_FIELDU : WELD_FIELDL;
	BOOL bUpperSide = TRUE;
	
	double dCenSta = pBxStt->GetStation() + (pBxEnd->GetStation()-pBxStt->GetStation())/2;

	while(pBx)
	{		
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)
		{
			pBx = Finder.GetBxNext();	continue;
		}

		if(!pBx->GetFactChain(nMarkBuje, TRUE) || !pBx->GetFactChain(nMarkBuje, FALSE)) break;

		double dSta		= pBx->GetStation();
		double dWidth	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE);

		//if((dSta - dStaComp) > -1000 && (dSta - dStaComp) < 0 && bUpper)
		//	bLeft = !bUpper;
		//if((dSta - dStaComp) < 1000 && (dSta - dStaComp) >= 0 && !bUpper)
		//	bLeft = !bUpper;

		if(dSta > dCenSta) bLeft = FALSE;
		
		CDPoint Po = pGir->GetXyGirderDis(dSta, -dWidth/4 * nSide);

		//��� ==> �״��..
		//Dom.DrawWeldMark(CDPoint(dLen,dYUp), nMarkBuje, nMarkBuje, pBx, -1, 12, 0, bLeft, TRUE, rotAng, FALSE, "", bGrindUpper, FALSE, TRUE);		
		//�Ϻ�  ==> �ݴ��..
		//Dom.DrawWeldMark(CDPoint(dLen,dYLo), nMarkBuje, nMarkBuje, pBx, -1, 12, 0, bLeft, FALSE, rotAng, FALSE, "", bGrindLower, FALSE, TRUE, FALSE);

		long nIdx = pBridge->GetIndexBaseWeld(nWeldMarkBuje,nWeldMarkBuje,pBx);
		Dom.DrawWeldMark(Po, nMarkBuje, nMarkBuje, pBx, -1, 12, 0, bLeft, bUpper, rotAng, FALSE, "", bGrind, FALSE, TRUE, bUpperSide,nIdx);
		
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawHatchInRect(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bHatch)
{
	if(pBxStt==NULL || pBxEnd==NULL) return;
  	
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBxStt->GetGirder();	
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();	
		
	CDPoint po[4];
	//
	double  dist  = 0;

	CDomyun *pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom,HCAD_HATH);

	double Length = pBxEnd->GetStation() - pBxStt->GetStation();
	long nHatch  = (long)(Length/5000)+1; // 5.0M�������� HATCH

	// ����
	double dWidth	= pGir->GetWidthOnStation(pBxStt->GetStation(), TRUE);
	dist  = -dWidth/2;
	double sttSta   = pBxStt->GetStationByScInBx(dist);
	double endSta   = pBxEnd->GetStationByScInBx(dist);
	CDPoint sttVAng = pBxStt->GetAngleSection();
	CDPoint endVAng = pBxEnd->GetAngleSection();
	po[0]  = pGir->GetXyGirderDis(sttSta,dist);
	po[1]  = pGir->GetXyGirderDis(endSta,dist);

	pGir->GetLine()->DrawLS(pDom, sttSta, endSta, dist+pGir->m_dGirderCenter);
	// ����
	dist  = dWidth/2;
	sttSta  = pBxStt->GetStationByScInBx(dist);
	endSta  = pBxEnd->GetStationByScInBx(dist);
	sttVAng = pBxStt->GetAngleSection();
	endVAng = pBxEnd->GetAngleSection();
	po[2]   = pGir->GetXyGirderDis(sttSta,dist);
	po[3]   = pGir->GetXyGirderDis(endSta,dist);
	
	pGir->GetLine()->DrawLS(pDom, sttSta, endSta, dist+pGir->m_dGirderCenter);

	pDom->LineTo(po[0], po[2]);
	pDom->LineTo(po[1], po[3]);

//	pDom->HatchRect(po[0].x,po[0].y,po[1].x,po[1].y,
//					po[3].x,po[3].y,po[2].x,po[2].y,"ANSI31");

	sttSta  = pBxStt->GetStation();
	endSta  = pBxEnd->GetStation();
	for(long n=0; n<nHatch; n++) 
	{
		if(n>0) sttSta += Length/nHatch;
		endSta	 = sttSta + Length/nHatch;
		// ����
		dist  = -dWidth/2;
		po[0] = pGir->GetXyGirderDis(sttSta,dist);
		po[1] = pGir->GetXyGirderDis(endSta,dist);
		if(n==0)
			po[0]  = pGir->GetXyGirderDis(sttSta,dist,sttVAng);
		if(n==nHatch-1)
			po[1]  = pGir->GetXyGirderDis(endSta,dist,endVAng);

		// ����
		dist  = dWidth/2;
		po[2] = pGir->GetXyGirderDis(sttSta,dist);
		po[3] = pGir->GetXyGirderDis(endSta,dist);
		if(n==0)
			po[2]  = pGir->GetXyGirderDis(sttSta,dist,sttVAng);
		if(n==nHatch-1)
			po[3]  = pGir->GetXyGirderDis(endSta,dist,endVAng);

		//////////////////////////////////////
		if(bHatch)
		{
			pDom->HatchRect(po[0].x,po[0].y,po[1].x,po[1].y,
						po[3].x,po[3].y,po[2].x,po[2].y,"ANSI31");
		}
		else
		{
			pDom->Solid(po[0].x,po[0].y,po[1].x,po[1].y
					   ,po[3].x,po[3].y,po[2].x,po[2].y);
		}
//		pDom->HatchInQuadrAngle(po[0],po[1],po[2],po[3],dHatchSpace);

	}

	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawPyung::DrawHatchInRectOnSlab(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(pBxStt==NULL || pBxEnd==NULL) return;
  	
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
//	CPlateGirderApp *pGir = (CPlateGirderApp*)pBxStt->GetGirder();	
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();
	
  	CPlateGirderApp		*pGir = pBridge->GetGirder(-1);
	CPlateGirderApp		*pGirL = pBridge->GetGirder(0);
	CPlateGirderApp		*pGirR = pBridge->GetGirder(pBridge->GetGirdersu()-1);

//	double LSharp = pDB->GetWidSlabUpFlangeWeb(NULL, TRUE) + pDB->m_dLengthSlabGirderStt;
//	double RSharp = pDB->GetWidSlabUpFlangeWeb(NULL, FALSE) + pDB->m_dLengthSlabGirderEnd;
	CDPoint po[4];
	//
//	double LDist  = -pGirL->GetWidUpHalf(NULL, TRUE)-LSharp;
//	double RDist  =  pGirR->GetWidUpHalf(NULL, TRUE)+RSharp;
	double LDist  =  -pGirL->GetWidthSlabLeftDom();
	double RDist  =   pGirR->GetWidthSlabRightDom();

	CDomyun *pDom = new CDomyun(pDomP);
	pOptStd->SetEnvType(pDom,HCAD_HATH);
	
	double Length = pBxEnd->GetStation() - pBxStt->GetStation();
	long nHatch  = (long)(Length/5000)+1; // 5.0M�������� HATCH

	// ����	
	double sttSta   = pBxStt->GetStation();
	double endSta   = pBxEnd->GetStation();
	CDPoint sttVAng = pBxStt->GetAngle();
	CDPoint endVAng = pBxEnd->GetAngle();
	
	for(long n=0; n<nHatch; n++) 
	{
		if(n>0) sttSta += Length/nHatch;
		endSta	 = sttSta + Length/nHatch;
		
		po[0] = pGir->GetXyGirderDis(sttSta,LDist+pGirL->GetDisCenterLineToGirderDom(pBxStt));
		po[1] = pGir->GetXyGirderDis(endSta,LDist+pGirL->GetDisCenterLineToGirderDom(pBxStt));
		if(n==0)		po[0]  = pGir->GetXyGirderDis(sttSta,LDist+pGirL->GetDisCenterLineToGirderDom(pBxStt),sttVAng);
		if(n==nHatch-1)	po[1]  = pGir->GetXyGirderDis(endSta,LDist+pGirL->GetDisCenterLineToGirderDom(pBxEnd),endVAng);

		po[2] = pGir->GetXyGirderDis(sttSta,RDist+pGirR->GetDisCenterLineToGirderDom(pBxStt));
		po[3] = pGir->GetXyGirderDis(endSta,RDist+pGirR->GetDisCenterLineToGirderDom(pBxStt));
		if(n==0)		po[2]  = pGir->GetXyGirderDis(sttSta,RDist+pGirR->GetDisCenterLineToGirderDom(pBxStt),sttVAng);
		if(n==nHatch-1)	po[3]  = pGir->GetXyGirderDis(endSta,RDist+pGirR->GetDisCenterLineToGirderDom(pBxEnd),endVAng);

		pDom->HatchRect(po[0].x,po[0].y,po[1].x,po[1].y,
						po[3].x,po[3].y,po[2].x,po[2].y,"ANSI31");
	}

	*pDomP << *pDom;
	delete pDom;
}

// ��൵
// bOneLineBeam : ���κ�,���κ��� �Ѽ����� ó��
// nJijumStt	: ���� ������ȣ
// nJijumEnd	: ��   ������ȣ
void CAPlateDrawPyung::AddKeyPlan(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,
								 double Scale, BOOL bSPMarkZigZag, BOOL bSpliceMark,
								 BOOL bJijumMark,     BOOL bDirection, BOOL bDimension, 
								 BOOL bDrawCrossBeam, BOOL bOneLineBeam, 
								 long nJijumStt, long nJijumEnd,
								 BOOL bMarkAssemBig, BOOL bMarkBoxNumber,
								 CString strTextStt,CString strTextEnd, BOOL bDawDiap, double dDistDirection)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDomP);	
	Dom.SetScaleDim(Scale);

	DrawCenterLineEachGiderPyung(&Dom,FALSE,FALSE);   //�Ŵ����߽ɼ�

	DrawSlabTotal(&Dom);							//������
	long nG = 0;
	for(nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateBxFinder	Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE/* | BX_CROSSBEAM_VBRACING*/);

		while(Finder.GetBxFixNext())
		{
			DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0, 0);				//�Ŵ�
			pBx = Finder.GetBxNext();
		}
	}	
	// �������� : ���θ�ǥ��			
	double RotRad = 0;
	BOOL   bSpliceOver;
	CDPoint po[2],vAng;
	CString str = _T("");
	CAPlateSymbolDom SubSymDom(&Dom, m_pDataMng);
	for(nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateBxFinder	Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_SPLICE);
		CPlateGirderApp  *pGir= pBridge->GetGirder(nG);
		while(pBx)
		{
			if(pBx->GetSplice()) 
			{
				pOptStd->SetEnvType(&Dom, HCAD_STLC);

				if(nG==0) 
				{
					RotRad = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()).GetAngleRadian();
					str = pBx->GetMarkString();
					bSpliceOver = (BOOL)(pGir->GetNumberSpliceByBx(pBx) % 2) & bSPMarkZigZag;
					if(bSpliceMark)
					{
						po[0] = pGir->GetXyGirderDis(pBx->GetStation(),0/*DistL-distEndBeamDistL*/);
						po[1] = pGir->GetXyGirderDis(pBx->GetStation(),0/*DistR+distEndBeamDistR*/);
						pOptStd->SetEnvType(&SubSymDom, HCAD_SYMB);
						SubSymDom.DrawSymbolSPMark(po[0].x,po[0].y,Dom.GetTextHeight()*(bSpliceOver ? 2 : 4),str,RotRad,FALSE);
						po[0] = pGir->GetXyGirderDis(pBx->GetStation(),0/*DistL*/);
						po[1] = pGir->GetXyGirderDis(pBx->GetStation(),0/*DistR*/);
					}
				}
			}
			// ������ũ
			if(bJijumMark && pBx->IsJijum() && nG==pBridge->GetGirdersu()-1)
			{	// SHOE
				pOptStd->SetEnvType(&SubSymDom, HCAD_SYMB);
				CPlateGirderApp  *pGirSec = pBridge->GetGirder(0);
				CPlateBasicIndex *pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);

				po[0]  = pGirSec->GetXyGirderDis(pBxSec->GetStation(), 0/*
					                           ,-pGirSec->m_dWidth/2
											    -pGirSec->m_dULeftSharp-pDom->Always(1)- distEndBeamDistL
											   , pBxSec->GetAngle()*/);
				vAng   = pBx->GetAngle();
				po[1]  = pGir->GetXyGirderDis(pBx->GetStation(), pBridge->m_dWidthSlabRight);//*DistR+distEndBeamDistR+pDom->Always(1)*/,vAng);
				RotRad = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()).GetAngleRadian();
				// MARK�� �߽ɰŴ��� ǥ��
				pGirSec = pBridge->GetGirder(-1);
				pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);
				long nJijum = pBx->GetNumberJijum();
				if(nJijum<0 || nJijum>pBridge->m_nQtyJigan) break;

				str = pBridge->m_strJijumName[nJijum];

				pOptStd->SetEnvType(&Dom,HCAD_STLC);
				SubSymDom.DrawSymbolJijumMark(po[1].x,po[1].y,str,RotRad,FALSE);

				//��������
//				pStd->GetDBShoeCoord()->DrawPlanJijumLine(pDom, FALSE,pBx, pBx);
			}
			pBx = Finder.GetBxNext();
		}
	}	

	DrawCenterJijum(&Dom,TRUE);                 // �����߽ɼ�(������)
	DrawCenterJijum(&Dom,FALSE);				// �����߽ɼ�(������)

	DrawAbutPierCenterLine(&Dom);               // ���뱳���߽ɼ�
	for(long g=0; g<pBridge->GetGirdersu(); g++)// �Ŵ���ȣ
		DrawGirderNumber(&Dom, g, FALSE);
	
	if(bSpliceMark)
		DrawSplicePyung(&Dom, NULL, NULL, 0, TRUE, TRUE, TRUE);//��������

	pOptStd->SetEnvLType(&Dom, LT_DOT_LINE);
	if(bDrawCrossBeam && bOneLineBeam)
	{
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
 		DrawCRBeamCLTotalGirder(&Dom); //�߽ɼ����׸�
 		DrawHBracingCLTotalGirder(&Dom);   //�극�̽�	
	}

	// �������� ǥ��
	if(bDirection)
	{
		CSymbolDom SymDom(&Dom, pOptStd);
		pOptStd->SetEnvType(&SymDom, HCAD_SYMB);

		CPlateGirderApp *pGir = pBridge->GetGirder(0);

		double  Dis = 0;
		if (dDistDirection == -1)
			Dis = pGir->GetWidthOnStation(pGir->GetBxOnJijum(0)->GetStation(), TRUE)/2 + SymDom.Always(4);
		else
			Dis = dDistDirection;
		if(!strTextStt.IsEmpty())
		{
			CPlateBasicIndex *pBxStt = (nJijumStt==-1) ? pGir->GetBxOnJijum(0) 
													   : pGir->GetBxOnJijum(nJijumStt);
			CDPoint xy  = pGir->GetXyGirderDis(pBxStt->GetStation(),-Dis, pBxStt->GetAngleSection());
			SymDom.DrawSymbolDirectionArrow(xy, strTextStt, TRUE);
			CDPoint rAng = pGir->GetLine()->GetAngleAzimuth(pBxStt->GetStation());
			SymDom.Rotate(xy,rAng);
			*pDomP << SymDom;
		}

		if(!strTextEnd.IsEmpty())
		{
			CPlateBasicIndex *pBxEnd = (nJijumEnd==-1) ? pGir->GetBxOnJijum(pBridge->m_nQtyJigan) 
												  : pGir->GetBxOnJijum(nJijumEnd);
			CDPoint xy  = pGir->GetXyGirderDis(pBxEnd->GetStation(),-Dis, pBxEnd->GetAngle());
			SymDom.DrawSymbolDirectionArrow(xy, strTextEnd, FALSE);
			CDPoint rAng = pGir->GetLine()->GetAngleAzimuth(pBxEnd->GetStation());
			SymDom.Rotate(xy,rAng);
			*pDomP << SymDom;
		}
	}	
	// �󼼵� HATCH�� ���⼭ ~~~ 	
	if(pBxStt && pBxEnd) DrawHatchInRect(&Dom,pBxStt,pBxEnd,FALSE);

	*pDomP << SubSymDom;	
	*pDomP << Dom;

}

void CAPlateDrawPyung::DrawJiJumStiff(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, BOOL bSolePlate)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	CDomyun Dom(pDom);

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);	
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUMSTIFF, pBxStt, pBxEnd, bSolePlate);
		DrawJiJumStiffWithenBx(&Dom, pBx, pBxEnd, bUpper, bSolePlate);
	}

	*pDom << Dom;
}
//����BX ������ 0.5m���� ����(1m)�ȿ� ������ ���������縦 �׸���.
//������ ������� ���ʿ� ���� �����Ѵ�.
void CAPlateDrawPyung::DrawJiJumStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bSolePlate)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBx ? pBx->GetGirder() : pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double dGirLen = 500;
	CAPlateDrawDanmyun DrawDan(m_pDataMng);
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayJijumstiff;
	//CPlateBasicIndexArray BasicIndexArrayJijumstiff; 
	pGir->GetBxWithenStation(BasicIndexArrayJijumstiff, pBx->GetStation()-dGirLen, pBx->GetStation()+dGirLen,
		                     BX_JIJUMSTIFF);
	CPlateBasicIndex* pBxJijum = NULL;
	
	CDomyun Dom(pDom);
	if(bUpper)	pOptStd->SetEnvType(&Dom, HCAD_STLH);
	else		pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint vX(1,0), vY(0,1);
	
	for(long n=0; n<BasicIndexArrayJijumstiff.GetSize(); n++)
	{
		pBxJijum    = BasicIndexArrayJijumstiff.GetAt(n);
		long nJijum = pGir->GetNumberJijumByBx(pBx);
		CVStiff		*pVStiff	= pGir->GetJijumVStiffByJijum(nJijum, 0);	
		CSection	*pSec		= pBxJijum->GetSection();
		CDPoint		vGir		= pGir->GetLine()->GetAngleAzimuth(pBxJijum->GetStation());

		double dSta	= pBxJijum->GetStation();
		for(long n=0; n<2; n++)
		{
			CDPoint pt[4];
			double dWidth	= pVStiff->m_dWidth[n];
			double dThick	= pVStiff->m_dThick[n];	
			CPlateFactChain *pChain = pBx->GetFactChain(G_W);
			double dGirWebT	 = pChain ? pGir->GetThickJewon(G_W, pBx)	: 0;
			double dWebThick = pChain ? pChain->m_dFactChainThick		: 0;
			CDPoint vAng = ANGLE90;
			long nSide = (n==0) ? -1 : 1;
			if(pSec && (pSec->IsenDetType(SECDET_CROSSBEAM) || pSec->IsenDetType(SECDET_VBRACING)))
				vAng = pBxJijum->GetAngleSectionCrossVBracing().Rotate(CDPoint(-1,0));
			else
				vAng = vGir.Rotate90();

			vX = vAng;
			vY = vX.RotateInv90();

			CDPoint xyCenStt = pGir->GetXyGirderDis(dSta, -dGirWebT/2 * nSide, vAng.RotateInv(vGir));			
			pt[2] = xyCenStt - vY*dThick/2 + vX*dWidth*nSide;
			pt[1] = xyCenStt + vY*dThick/2 + vX*dWidth*nSide;
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt + vY*dThick/2, vX, pt[0]);
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt - vY*dThick/2, vX, pt[3]);

			Dom.LineTo(pt[0], pt[1]);
			Dom.LineTo(pt[1], pt[2]);
			Dom.LineTo(pt[2], pt[3]);
		}				
		if(!bUpper && bSolePlate) 
		{
			CDomyun DomSolePlate(&Dom);
			pOptStd->SetEnvLType(&DomSolePlate,LT_DOT_LINE);
			DrawDan.DrawPlanSolePlateBx(&DomSolePlate,pGir->GetBxOnJijum(nJijum),FALSE,TRUE,TRUE);
			Dom << DomSolePlate;
		}
	}	

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawJiJumStiffWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, BOOL bSolePlate)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBxStt ? pBxStt->GetGirder() : pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CAPlateDrawDanmyun DrawDan(m_pDataMng);

	CDomyun Dom(pDom);

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUMSTIFF,pBxStt,pBxEnd);

	while(pBx)
	{
		long nJijum = pGir->GetNumberJijumByBx(pBx);
		CVStiff *pVStiff = pGir->GetJijumVStiffByJijum(nJijum, 0);
		
		CSection *pSec = pBx->GetSection();
		CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());

		double dSta	= pBx->GetStation();
		for(long n=0; n<2; n++)
		{
			CDPoint pt[4];
			double dWidth	= pVStiff->m_dWidth[n];
			double dThick	= pVStiff->m_dThick[n];	
			CPlateFactChain *pChain = pBx->GetFactChain(G_W);
			double dGirWebT	 = pChain ? pGir->GetThickJewon(G_W, pBx)	: 0;
			CDPoint vAng = ANGLE90;
			long nSide = (n==0) ? -1 : 1;
			if(pSec && (pSec->IsenDetType(SECDET_CROSSBEAM) || pSec->IsenDetType(SECDET_VBRACING)))
				vAng = pBx->GetAngleSectionCrossVBracing().Rotate(CDPoint(-1,0));
			else
				vAng = vGir.Rotate90();

			CDPoint vX = vAng;
			CDPoint vY = vX.RotateInv90();

			if(dWidth == 0) continue;

			CDPoint xyCenStt = pGir->GetXyGirderDis(dSta, -dGirWebT/2 * nSide, vAng.RotateInv(vGir));			
			pt[2] = xyCenStt - vY*dThick/2 + vX*dWidth*nSide;
			pt[1] = xyCenStt + vY*dThick/2 + vX*dWidth*nSide;
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt + vY*dThick/2, vX, pt[0]);
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt - vY*dThick/2, vX, pt[3]);

			Dom.LineTo(pt[0], pt[1]);
			Dom.LineTo(pt[1], pt[2]);
			Dom.LineTo(pt[2], pt[3]);
			Dom.LineTo(pt[3], pt[0]);
		}				
		if(!bUpper && bSolePlate) 
		{
			CDomyun DomSolePlate(&Dom);
			pOptStd->SetEnvLType(&DomSolePlate,LT_DOT_LINE);
			DrawDan.DrawPlanSolePlateBx(&DomSolePlate,pGir->GetBxOnJijum(nJijum),FALSE,TRUE,TRUE);
			Dom << DomSolePlate;
		}

		pBx = Finder.GetBxNext();
	}	

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawJackUp(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();

	CDomyun Dom(pDom);

	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);	
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JACKUP,pBxStt,pBxEnd);
		DrawJackUpWithenBx(&Dom, pBx, pBxEnd);
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawJackUpWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBxStt ? pBxStt->GetGirder() : pBridge->GetGirder(-1);
	
	CDomyun Dom(pDom);

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JACKUP,pBxStt,pBxEnd);

	while(pBx)
	{
		long nJijum		= pGir->GetNumberJijumByBx(pBx);		
		double	dSta	= pBx->GetStation();
		CDPoint	vGir	= pGir->GetLine()->GetAngleAzimuth(dSta);
		CJackupStiff *pJack = pGir->GetJackupByJijum(nJijum, 0);

		double dWidthL	= pJack->m_vJewon[0].x;
		double dThickL	= pJack->m_vJewon[0].y;				
		double dWidthR	= pJack->m_vJewon[1].x;
		double dThickR	= pJack->m_vJewon[1].y;				

		CDPoint vAng = ANGLE90;
		//////////////////////////////////////////////////////////////////////////

		CPlateFactChain *pChain = pBx->GetFactChain(G_W);
		double dGirWebT	 = pChain ? pGir->GetThickJewon(G_W, pBx)	: 0;
		CDPoint vX = vGir.Rotate90();
		CDPoint vY = vGir;
		
		CDPoint pt[4];
		if(dWidthL>0 && dThickL>0)
		{
			CDPoint xyCenStt = pGir->GetXyGirderDis(dSta, -dGirWebT/2);			
			pt[2] = xyCenStt - vY*dThickL/2 + vX*dWidthL;
			pt[1] = xyCenStt + vY*dThickL/2 + vX*dWidthL;
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt + vY*dThickL/2, vX, pt[0]);
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt - vY*dThickL/2, vX, pt[3]);

			Dom.LineTo(pt[0], pt[1]);
			Dom.LineTo(pt[1], pt[2]);
			Dom.LineTo(pt[2], pt[3]);
		}
		if(dWidthR>0 && dThickR>0)
		{
			CDPoint xyCenStt = pGir->GetXyGirderDis(dSta, dGirWebT/2);
			pt[2] = xyCenStt - vY*dThickR/2 - vX*dWidthR;
			pt[1] = xyCenStt + vY*dThickR/2 - vX*dWidthR;
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt + vY*dThickR/2, vX, pt[0]);
			GetXyMatchLineAndLine(xyCenStt, vGir, xyCenStt - vY*dThickR/2, vX, pt[3]);

			Dom.LineTo(pt[0], pt[1]);
			Dom.LineTo(pt[1], pt[2]);
			Dom.LineTo(pt[2], pt[3]);
		}
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;	
}

void CAPlateDrawPyung::DrawJSolplatePyung(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	double dSta=0;
	CDPoint vGir;
	CDPoint xyCen, xy1, xy2, xy3, xy4;
	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		for(int j=0; j<pBridge->m_nQtyJigan+1; j++)
		{
			for(int i=0; i<2; i++)
			{
				if(pBridge->m_structJackupJewon[j][i].m_dL==0) continue;

				if(i==0)	dSta = pGir->GetStationOnJijum(j) - pBridge->m_structJackupJewon[j][i].m_dL;
				else		dSta = pGir->GetStationOnJijum(j) + pBridge->m_structJackupJewon[j][i].m_dL;

				vGir = pGir->GetLine()->GetAngleAzimuth(dSta); 
				xyCen = pGir->GetXyGirderDis(dSta);
				xy1 = xyCen+vGir.Rotate90()*pBridge->m_structJackupJewon[j][i].m_dH/2 + vGir*pBridge->m_structJackupJewon[j][i].m_dW/2;
				xy2 = xyCen+vGir.RotateInv90()*pBridge->m_structJackupJewon[j][i].m_dH/2 + vGir*pBridge->m_structJackupJewon[j][i].m_dW/2;
				xy3 = xyCen+vGir.Rotate90()*pBridge->m_structJackupJewon[j][i].m_dH/2 - vGir*pBridge->m_structJackupJewon[j][i].m_dW/2;
				xy4 = xyCen+vGir.RotateInv90()*pBridge->m_structJackupJewon[j][i].m_dH/2 - vGir*pBridge->m_structJackupJewon[j][i].m_dW/2;
				Dom.LineTo(xy1, xy2);
				Dom.LineTo(xy3, xy4);
				Dom.LineTo(xy1, xy3);
				Dom.LineTo(xy2, xy4);
				*pDom << Dom;

/*				pOptStd->SetEnvType(&Dom, HCAD_DIML);
				Dom.SetDirection("BOTTOM");
				DimMoveTo(&Dom, xy1.x, xy1.y, 0);
				DimLineTo(&Dom, pBridge->m_structJackupJewon[j][i].m_dW, 1, "W");
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.Rotate(xyCen, -vGir);
				*pDom << Dom;*/
			}
		}
	}
}


void CAPlateDrawPyung::DimJackUp(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(0);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());

	SetDirection(0,"BOTTOM");

	double w = 0;

	CString str;
	for(long nK = 0; nK <= pBridge->m_nQtyJigan; nK++)
	{
		double dSta = pGir->GetStationOnJijum(nK);
		CDPoint vAng = pGir->GetAngleOnJijum(nK);
		dSta  = pGir->GetStationBySc(dSta, 0, vAng);

		long nDan, nDanF = 0, nDanR = 0;
		for (long nJack = 0; nJack < JACKUPSUMAX; nJack++)
		{
			double len = pBridge->m_dJackUpTerm[nK][nJack];
			if (len == 0)
				continue;
			if (len > 0)
				nDan = nDanR++;
			else
				nDan = nDanF++;
			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			str.Format("%cJ%d", 'a' + nK, nJack + 1);
			DimMoveTo(&Dom, dSta, w, nDan);
			DimLineTo(&Dom, len, nDan, str);

			// Ŀ�� �׸��� ���� �׸�.
			pOptStd->SetEnvType(&Dom, HCAD_HIDD);
			str.Format("%s%d", pBridge->m_strJijumName[nK], nJack + 1);
			DimMoveTo(&Dom, dSta, w, nDan);
			DimLineTo(&Dom, len, nDan, str);
		}
	}
	*pDom << Dom;
}

void CAPlateDrawPyung::DimJijumStiff(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(0);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());

	SetDirection(0,"BOTTOM");

	double w = 0;

	CString str;
	for(long nK = 0; nK <= pBridge->m_nQtyJigan; nK++)
	{
		double dSta = pGir->GetStationOnJijum(nK);
		CDPoint vAng = pGir->GetAngleOnJijum(nK);
		dSta  = pGir->GetStationBySc(dSta, 0, vAng);

		long nDan, nDanF = 0, nDanR = 0;
		for (long nJack = 0; nJack < JACKUPSUMAX; nJack++)
		{
			double len = pBridge->m_dJiJumTerm[nK][nJack];
			if (len == 0)
				continue;
			if (len > 0)
				nDan = nDanR++;
			else
				nDan = nDanF++;
			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			str.Format("%cJ%d", 'a' + nK, nJack + 1);
			DimMoveTo(&Dom, dSta, w, nDan);
			DimLineTo(&Dom, len, nDan, str);

			// Ŀ�� �׸��� ���� �׸�.
			pOptStd->SetEnvType(&Dom, HCAD_HIDD);
			str.Format("%s%d", pBridge->m_strJijumName[nK], nJack + 1);
			DimMoveTo(&Dom, dSta, w, nDan);
			DimLineTo(&Dom, len, nDan, str);
		}
	}
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawCRBeamGussetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide)
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	CDPoint BP = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint vRotate = pGir->GetLine()->GetAngleAzimuth(
					  pGir->GetStationBySc(pBx->GetStation(), 0/*dWidth/2*/, pBx->GetAngleSection()));
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CPlatePieceObject pObj(pGir);
	pObj.m_nMarkBuje = bUpper ? CP_BR_F_U : CP_BR_F_L;		

	if(bUpper)
	{
		if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))			
		{
			pObj.m_nCol = 0;
			pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
			pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
			Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
			Dom.Move(BP);					
			*pDom << Dom;
		}
		if(pC->m_nRGirSpType && (nSide==1 || nSide==0))
		{
			pObj.m_nCol = 1;
			pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
			pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
			Dom.Rotate(CDPoint(0,0),vRotate.Rotate90());
			Dom.Move(BP);					
			*pDom << Dom;
		}				
	}
	else
	{
		if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))
		{
			pObj.m_nCol = 0;
			pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
			pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
			Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
			Dom.Move(BP);					
			*pDom << Dom;
		}
		if(pC->m_nRGirSpType && (nSide==1 || nSide==0))
		{
			pObj.m_nCol = 1;
			pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
			pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
			Dom.Rotate(CDPoint(0,0),vRotate.Rotate90());
			Dom.Move(BP);					
			*pDom << Dom;
		}
	}
}




void CAPlateDrawPyung::DrawCRBeamGussetWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBxStt ? pBxStt->GetGirder() : pBridge->GetGirder(-1);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		CDPoint BP = pGir->GetXyGirderDis(pBx->GetStation());
		CDPoint vRotate = pGir->GetLine()->GetAngleAzimuth(
						  pGir->GetStationBySc(pBx->GetStation(), 0/*dWidth/2*/, pBx->GetAngleSection()));
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{
			CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();

			CPlatePieceObject pObj(pGir);
			pObj.m_nMarkBuje = bUpper ? CP_BR_F_U : CP_BR_F_L;		

			if(bUpper)
			{
				if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))			
				{
					pObj.m_nCol = 0;
					pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
					pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
					Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
					Dom.Move(BP);					
					*pDom << Dom;
				}
				if(pC->m_nRGirSpType && (nSide==1 || nSide==0))
				{
					pObj.m_nCol = 1;
					pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
					pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
					Dom.Rotate(CDPoint(0,0),vRotate.Rotate90());
					Dom.Move(BP);					
					*pDom << Dom;
				}				
			}
			else
			{
				if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))
				{
					pObj.m_nCol = 0;
					pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
					pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
					Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
					Dom.Move(BP);					
					*pDom << Dom;
				}
				if(pC->m_nRGirSpType && (nSide==1 || nSide==0))
				{
					pObj.m_nCol = 1;
					pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
					pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
					Dom.Rotate(CDPoint(0,0),vRotate.Rotate90());
					Dom.Move(BP);					
					*pDom << Dom;
				}
			}
		}
		pBx = Finder.GetBxNext();
	}	
}




void CAPlateDrawPyung::DrawCRBeamBothGussetWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide, BOOL bDivType, BOOL bFlangeView, BOOL bDomyun)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBxStt ? pBxStt->GetGirder() : pBridge->GetGirder(-1);	
	CPlateGirderApp *pGirL   = pBridge->GetGirder(pBxStt->GetNumberGirder()-1);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	long nGirStatus = 0;
	if(pBxStt->GetNumberGirder() == 0)
		nGirStatus = 0;
	else if(pBxStt->GetNumberGirder() < pBridge->GetGirdersu()-1)
		nGirStatus = 2;
	else if(pBxStt->GetNumberGirder() == pBridge->GetGirdersu()-1)
		nGirStatus = 1;

	long nMode = 0;	
	
	CDomyun Dom(pDom);

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx		= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		CDPoint BP = pGir->GetXyGirderDis(pBx->GetStation());
		CDPoint vRotate = pGir->GetLine()->GetAngleAzimuth(
						  pGir->GetStationBySc(pBx->GetStation(), 0/*dWidth/2*/, pBx->GetAngleSection()));
		
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{
			CPlateCrossBeam	*pC      = pBx->GetSection()->GetCrossBeam();
			CPlateCrossBeam	*pCPrev  = pBx->GetBxMatchByCrossBeam(pGirL)->GetSection()->GetCrossBeam();
			CPlatePieceObject pObj(pGir);
			pObj.m_nMarkBuje = bUpper ? CP_BR_F_U : CP_BR_F_L;		
			
			if (bDivType)// ���κ� ������ ���� �� ��� ��ŸƮ�� �ص忡�� ��,�� �׸� ����...
			{
				if (pBxStt == pBx)
					nMode = 1;
				else if(pBxEnd == pBx)
					nMode = 2;
				else
					nMode = 0;
			}

			if(nGirStatus==0)
			{
				if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))			
				{
					if(!bFlangeView || (bUpper&&pC->m_dLH1==0) || (!bUpper&&pC->m_dLH2==0))
					{
						pObj.m_nCol = 0;
						pC->CreatePieceObjPlateGassetFlange(&pObj, bDomyun ? FALSE : TRUE,  bBoltHole, bBoltLine, bMakeChip, nMode);
						pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
						
						if(!bDomyun)
						{
							Dom.Rotate(CDPoint(0,0), CDPoint(0, -1));
						}

						Dom.Move(BP);					
						*pDom << Dom;
					}					
				}

			}
			else if(nGirStatus==2)
			{
				if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))			
				{
					if(!bFlangeView || (bUpper&&pC->m_dLH1==0) || (!bUpper&&pC->m_dLH2==0))
					{
						pObj.m_nCol = 0;
						pC->CreatePieceObjPlateGassetFlange(&pObj, bDomyun ? FALSE : TRUE,  bBoltHole, bBoltLine, bMakeChip, nMode);
						pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
						
						if(!bDomyun)
						{
							Dom.Rotate(CDPoint(0,0),CDPoint(0,-1));
						}

						Dom.Move(BP);					
						*pDom << Dom;
					}					
				}
				if(pCPrev && pCPrev->m_nRGirSpType && (nSide==1 || nSide==0))
				{
					if(!bFlangeView || (bUpper&&pC->m_dRH1==0) || (!bUpper&&pC->m_dRH2==0))
					{
						pObj.m_nCol = 1;
						pCPrev->CreatePieceObjPlateGassetFlange(&pObj, bDomyun ? FALSE : TRUE,  bBoltHole, bBoltLine, bMakeChip, nMode);
						pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
						
						if(!bDomyun)
						{
							Dom.Rotate(CDPoint(0,0),CDPoint(0,1));
						}

						Dom.Move(BP);
						*pDom << Dom;
					}					
				}	
			}
			else
			{
				if(pCPrev && pCPrev->m_nRGirSpType && (nSide==1 || nSide==0))
				{
					if(!bFlangeView || (bUpper&&pC->m_dRH1==0) || (!bUpper&&pC->m_dRH2==0))
					{
						pObj.m_nCol = 1;
						pCPrev->CreatePieceObjPlateGassetFlange(&pObj, bDomyun ? FALSE : TRUE,  bBoltHole, bBoltLine, bMakeChip, nMode);
						pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
						
						if(!bDomyun)
						{
							Dom.Rotate(CDPoint(0,0),CDPoint(0,-1));
						}

						Dom.Move(BP);
						*pDom << Dom;
					}					
				}	
			}
		}
		pBx = Finder.GetBxNext();
	}	
}

void CAPlateDrawPyung::DrawCRBeamBothGussetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = (pBx) ? pBx->GetGirder() : pBridge->GetGirder(-1);	
	CPlateGirderApp *pGirL= pBridge->GetGirder(pBx->GetNumberGirder()-1);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	long nGirStatus = 0;
	if(pBx->GetNumberGirder() == 0)
		nGirStatus = 0;
	else if(pBx->GetNumberGirder() < pBridge->GetGirdersu()-1)
		nGirStatus = 2;
	else if(pBx->GetNumberGirder() == pBridge->GetGirdersu()-1)
		nGirStatus = 1;

	CDomyun Dom(pDom);

	CSection *pSec = pBx->GetSection();
	if(pSec==NULL) return;

	CDPoint BP = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint vRotate = pGir->GetLine()->GetAngleAzimuth(
					  pGir->GetStationBySc(pBx->GetStation(), 0/*dWidth/2*/, pBx->GetAngleSection()));
	if(pSec->IsenDetType(SECDET_CROSSBEAM))
	{
		CPlateCrossBeam	*pC      = pBx->GetSection()->GetCrossBeam();
		CPlateCrossBeam	*pCPrev  = pBx->GetBxMatchByCrossBeam(pGirL)->GetSection()->GetCrossBeam();
		CPlatePieceObject pObj(pGir);
		pObj.m_nMarkBuje = bUpper ? CP_BR_F_U : CP_BR_F_L;		
		
		if(nGirStatus==0)
		{
			if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))			
			{
				pObj.m_nCol = 0;
				pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
				pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
				Dom.Move(BP);					
				*pDom << Dom;
			}

		}
		else if(nGirStatus==2)
		{
			if(pC->m_nLGirSpType && (nSide==-1 || nSide==0))			
			{
				pObj.m_nCol = 0;
				pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
				pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
				Dom.Move(BP);					
				*pDom << Dom;
			}
			if(pCPrev->m_nRGirSpType && (nSide==1 || nSide==0))
			{
				pObj.m_nCol = 1;
				pCPrev->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
				pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0),vRotate.Rotate90());
				Dom.Move(BP);					
				*pDom << Dom;
			}	
		}
		else
		{
			if(pCPrev->m_nRGirSpType && (nSide==1 || nSide==0))
			{
				pObj.m_nCol = 1;
				pCPrev->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
				pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
				Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
				Dom.Move(BP);					
				*pDom << Dom;
			}	
		}
	}
}
void CAPlateDrawPyung::DrawCRBeamWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, 
										   BOOL bLeftHalf, BOOL bRightHalf, long nLineWeb, long nLineFlange)
{
	if(!pBx || !pBx->GetSection()) return;
	CPlateGirderApp	*pGir    = (CPlateGirderApp*)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGirR   = (CPlateGirderApp*)pGir->m_pRight;
	CPlateCrossBeam	*pC      = pBx->GetSection()->GetCrossBeam();
	if(!pC)		return;

	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGirR, 1);
	if(pBxSec == NULL) return;

	CDPoint xyCrL(0,0), xyCrR(0,0);
	
	CDPoint vGirL	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()); 
	CDPoint vCross	= pC->GetVectorPlan();
	CDPoint vAng	= -vCross.RotateInv(vGirL);

	pC->GetXyBaseLR(xyCrL, xyCrR);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint BaseL = xyCrL;
	CDPoint BaseR = xyCrR;

	double Wid		= bUpper ? pC->m_uM_CR : pC->m_dM_CR;		// ���ǳʺ�/2 : ���ǳʺ�/2
	double DisL, DisR;
	double dWVstiffL = pBx->GetSection()->GetVStiff()->m_dWidth[1];
	double dTVstiffL = pBx->GetSection()->GetVStiff()->m_dThick[1];
	double dWVstiffR = pBxSec->GetSection()->GetVStiff()->m_dWidth[0];
	double dTVstiffR = pBxSec->GetSection()->GetVStiff()->m_dThick[0];

	////////////////////
	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		DisL = pC->m_P_CR;
		DisR = pC->m_P_CR;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		DisL = dWVstiffL+(bUpper?pC->m_dbL4:pC->m_dbL3);
		DisR = pC->m_P_CR;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		DisL = pC->m_P_CR;
		DisR = dWVstiffR+(bUpper?pC->m_dbR4:pC->m_dbR3);
	}
	else 
	{
		DisL = dWVstiffL+(bUpper?pC->m_dbL4:pC->m_dbL3);
		DisR = dWVstiffR+(bUpper?pC->m_dbR4:pC->m_dbR3);
	}

	DisL /= fabs(vAng.y);
	DisR /= fabs(vAng.y);

	BaseL = xyCrL + vCross * DisL;
	BaseR = xyCrR - vCross * DisR;
	if(!pC->m_nLGirSpType)	BaseL -= vCross.Rotate90()*(dTVstiffL-pC->m_T3_CR/2);
	if(!pC->m_nRGirSpType)	BaseR -= vCross.Rotate90()*(dTVstiffR-pC->m_T3_CR/2);

	CDPoint Lp1, Lp2, Rp1, Rp2, Cen1, Cen2;
	
	if(nLineFlange != 0)
	{
		Lp1 = BaseL + vCross.RotateInv90() * Wid;
		Lp2 = BaseL - vCross.RotateInv90() * Wid;
		Rp1 = BaseR + vCross.RotateInv90() * Wid;
		Rp2 = BaseR - vCross.RotateInv90() * Wid;
		Cen1 = (BaseL+BaseR)/2 + vCross.RotateInv90() * Wid;
		Cen2 = (BaseL+BaseR)/2 - vCross.RotateInv90() * Wid;

		if((!pC->m_nLGirSpType && pC->m_dLH1==0) || (!pC->m_nRGirSpType && pC->m_dRH1==0))
		{
			Lp1 = BaseL + vGirL * Wid;
			Lp2 = BaseL - vGirL * Wid;
			Rp1 = BaseR + vGirL * Wid;
			Rp2 = BaseR - vGirL * Wid;
			Cen1 = (BaseL+BaseR)/2 + vGirL * Wid;
			Cen2 = (BaseL+BaseR)/2 - vGirL * Wid;
		}
		if(bLeftHalf) 
		{
			if(nLineFlange>=1) Dom.LineTo(Lp1,Cen1);
			if(nLineFlange!=1) Dom.LineTo(Lp2,Cen2);
			if(nLineFlange> 1) Dom.LineTo(Lp1,Lp2);
		}
		if(bRightHalf) 
		{
			if(nLineFlange>=1) Dom.LineTo(Rp1,Cen1);
			if(nLineFlange!=1) Dom.LineTo(Rp2,Cen2);
			if(nLineFlange> 1) Dom.LineTo(Rp1,Rp2);
		}
	}
	// �β��� : ���κ� ������ ///////////////////////////////////////////////////////////////////////////
	Wid   = pC->m_T3_CR/2; // ���κ� ���÷���Ʈ T
	if(nLineWeb==0) Wid = 0;

	//Filler ����...
	CVStiff* pVStiff = pC->m_pVStiff;
	double dThickFiller = 0;
	double dMgap = 0;

	if(pC->m_nFillerUseType==1)//�����̸�...
	{
		dThickFiller = pVStiff->m_dThick[1] - pC->m_T3_CR;
		dMgap = pVStiff->m_dThick[1]/2 - pC->m_T3_CR/2;
	}
	
	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		DisL = pC->m_P_CR ;
		DisR = pC->m_P_CR;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		DisL = pC->GetLengthGirWebToCrWeb(TRUE);
		DisR = pC->m_P_CR;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		DisL = pC->m_P_CR;
		DisR = pC->GetLengthGirWebToCrWeb(FALSE);
	}
	else 
	{
		DisL = pC->GetLengthGirWebToCrWeb(TRUE);
		DisR = pC->GetLengthGirWebToCrWeb(FALSE);
	}

	DisL /= fabs(vAng.y);
	DisR /= fabs(vAng.y);

	BaseL = xyCrL + vCross * DisL;
	BaseR = xyCrR - vCross * DisR;
	if(!pC->m_nLGirSpType)	BaseL -= vCross.Rotate90()*((dTVstiffL-pC->m_T3_CR)/2+pC->m_T3_CR);
	if(!pC->m_nRGirSpType)	BaseR -= vCross.Rotate90()*((dTVstiffR-pC->m_T3_CR)/2+pC->m_T3_CR);

	Lp1 = BaseL - vCross.RotateInv90() * Wid;
	Lp2 = BaseL + vCross.RotateInv90() * Wid;
	Rp1 = BaseR - vCross.RotateInv90() * Wid;
	Rp2 = BaseR + vCross.RotateInv90() * Wid;

	if ((pC->m_nLGirSpType || pC->m_nRGirSpType) && pC->m_nFillerUseType==1 && dThickFiller > 2)	//ä������ 2mm ���� ũ��...
	{
		Lp1 = Lp1+ dMgap*vCross.Rotate90();
		Lp2 = Lp2+ dMgap*vCross.Rotate90();
		Rp1 = Rp1+ dMgap*vCross.Rotate90();
		Rp2 = Rp2+ dMgap*vCross.Rotate90();
	}
	
	if(bUpper||bLeftHalf)	Dom.LineTo(Lp1, Lp2);
	if(!bUpper||bRightHalf)	Dom.LineTo(Rp1, Rp2);

	// ����� ��� �β����� HIDDENǥ��
	if(bUpper) 
		pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);
	if(bLeftHalf) 
	{
		if(nLineWeb!=1) Dom.LineTo(Lp1,(Lp1+Rp1)/2);
		if(nLineWeb>=1) Dom.LineTo(Lp2,(Lp2+Rp2)/2);
	}
	if(bRightHalf) 
	{
		if(nLineWeb!=1) Dom.LineTo(Rp1,(Lp1+Rp1)/2);
		if(nLineWeb>=1) Dom.LineTo(Rp2,(Lp2+Rp2)/2);
	}

	*pDom << Dom;
}
//CDPoint GetXyRotateRadian(CDPoint xy, CDPoint xyTarget, double angRadian);
//CDPoint GetXyRotateVector(CDPoint xy, CDPoint xyTarget, CDPoint vAng); 



/*
void CAPlateDrawPyung::DrawPlanGassetPlateBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftGirder, 
											 BOOL bBoltHole,BOOL bBoltLine, BOOL bMakeChip, BOOL bFillCircle)
{
	if(pBx==NULL) return;
	CPlateBridgeApp		*pDB  = m_pDataMng->GetBridge();
	CPlateGirderApp	    *pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	    *pGirR= (CPlateGirderApp*)pGir->m_pRight;
	CPlateCrossBeam		*pC   = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	if(pDB->GetGirdersu()==1)	return;
	if(!pGirR) return;
	CPlateBasicIndex *pBxNext = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	double dWGir = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWGirR= pGirR->GetWidthOnStation(pBxNext->GetStation(), TRUE);
	if(pC == NULL) return;

	CDomyun	Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double CenSta   = pBx->GetStation();
	CDPoint vRotate = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint BP = pGir->GetXyGirderDis(CenSta);

	if(!bLeftGirder) 
	{
		CenSta  = pBxNext->GetStation();
		vRotate = pGirR->GetLine()->GetAngleAzimuth(pBxNext->GetStation());
		BP	    = pGirR->GetXyGirderDis(CenSta);
	}

	CPlatePieceObject pObj(pGir);
	if (bUpper)	pObj.m_nMarkBuje = CP_BR_F_U;
	else		pObj.m_nMarkBuje = CP_BR_F_L;
	if (bLeftGirder) pObj.m_nCol = 0;
	else			 pObj.m_nCol = 1;
	
	pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  bBoltHole, bBoltLine, bMakeChip);
	pObj.DrawPieceOutline(&Dom, pOptStd, bFillCircle);

	Dom.Rotate(CDPoint(0,0),vRotate.RotateInv90());
	Dom.Move(BP);

	*pDom << Dom;
	
}
*/
void CAPlateDrawPyung::DrawCRBeamBothGassetWebWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide, BOOL bFlangeView)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir    = pBxStt ? pBxStt->GetGirder() : pBridge->GetGirder(-1);	
	CPlateGirderApp *pGirL   = pBridge->GetGirder(pBxStt->GetNumberGirder()-1);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	long nGirStatus = 0;
	if(pBxStt->GetNumberGirder() == 0)
		nGirStatus = 0;
	else if(pBxStt->GetNumberGirder() < pBridge->GetGirdersu()-1)
		nGirStatus = 2;
	else if(pBxStt->GetNumberGirder() == pBridge->GetGirdersu()-1 )
		nGirStatus = 1;


	CPlateCrossBeam	*pC    = NULL;
	CPlateCrossBeam	*pCPrev= NULL;
	CPlateBasicIndex *pBx    = NULL;
	CPlateBasicIndex *pBxPrev= NULL;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);
	CPlateBxFinder	Finder(pGir);
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);				
	while(pBx)
	{			
		CSection *pSec = pBx->GetSection();
		ASSERT(pSec);
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{
			pC = pBx->GetSection()->GetCrossBeam();
			if(nGirStatus==0)
			{
				if((nSide==-1 || nSide==0) && pC->m_nLGirSpType)
				{
					if(!bFlangeView || pC->m_dLH1==0)
						DrawCRBeamGassetWebOnBx(&Dom, pBx, TRUE, TRUE);
				}
			}
			else if(nGirStatus==2)
			{
				pBxPrev  = pBx->GetBxMatchByCrossBeam(pGirL);
				pC     = pBx->GetSection()->GetCrossBeam();
				pCPrev = pBx->GetBxMatchByCrossBeam(pGirL)->GetSection()->GetCrossBeam();
				if((nSide==-1 || nSide==0) && pC->m_nLGirSpType)
				{
					if(!bFlangeView || pC->m_dLH1==0)
						DrawCRBeamGassetWebOnBx(&Dom, pBx, TRUE, TRUE);
				}
				if(pCPrev && pCPrev->m_nRGirSpType && (nSide==1 || nSide==0))
				{
					if(!bFlangeView || pC->m_dRH1==0)
						DrawCRBeamGassetWebOnBx(&Dom, pBxPrev, TRUE, FALSE);
				}
			}
			else
			{
				pBxPrev  = pBx->GetBxMatchByCrossBeam(pGirL);
				pCPrev = pBx->GetBxMatchByCrossBeam(pGirL)->GetSection()->GetCrossBeam();
				if(pCPrev && pCPrev->m_nRGirSpType && (nSide==1 || nSide==0))
				{
					if(!bFlangeView || pC->m_dRH1==0 )
						DrawCRBeamGassetWebOnBx(&Dom, pBxPrev, TRUE, FALSE);
				}	
			}
		}
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;

}

void CAPlateDrawPyung::DrawCRBeamBothGassetWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirL= pBridge->GetGirder(pBx->GetNumberGirder()-1);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	long nGirStatus = 0;
	if(pBx->GetNumberGirder() == 0)
		nGirStatus = 0;
	else if(pBx->GetNumberGirder() < pBridge->GetGirdersu()-1)
		nGirStatus = 2;
	else if(pBx->GetNumberGirder() == pBridge->GetGirdersu()-1 )
		nGirStatus = 1;

	CPlateCrossBeam	*pC    = NULL;
	CPlateCrossBeam	*pCPrev= NULL;
	CPlateBasicIndex *pBxPrev= NULL;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);
	CSection *pSec = pBx->GetSection();
	if(pSec==NULL) return;
	ASSERT(pSec);
	if(pSec->IsenDetType(SECDET_CROSSBEAM))
	{
		pC = pBx->GetSection()->GetCrossBeam();
		if(nGirStatus==0)
		{
			if((nSide==-1 || nSide==0) && pC->m_nLGirSpType)
				DrawCRBeamGassetWebOnBx(&Dom, pBx, TRUE, TRUE);
		}
		else if(nGirStatus==2)
		{
			pBxPrev  = pBx->GetBxMatchByCrossBeam(pGirL);
			pC     = pBx->GetSection()->GetCrossBeam();
			pCPrev = pBx->GetBxMatchByCrossBeam(pGirL)->GetSection()->GetCrossBeam();
			if((nSide==-1 || nSide==0) && pC->m_nLGirSpType)
				DrawCRBeamGassetWebOnBx(&Dom, pBx, TRUE, TRUE);					
			if((nSide== 1 || nSide==0) && pCPrev->m_nRGirSpType)
				DrawCRBeamGassetWebOnBx(&Dom, pBxPrev, TRUE, FALSE);
		}
		else
		{
			pBxPrev  = pBx->GetBxMatchByCrossBeam(pGirL);
			pCPrev = pBx->GetBxMatchByCrossBeam(pGirL)->GetSection()->GetCrossBeam();
			if(pCPrev->m_nRGirSpType && (nSide==1 || nSide==0))
			{
				DrawCRBeamGassetWebOnBx(&Dom, pBxPrev, TRUE, FALSE);
			}	
		}
	}
	*pDom << Dom;

}

void CAPlateDrawPyung::DrawCRBeamGassetWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftGirder, long nLine)
{
	if(!pBx || !pBx->GetSection()) return;
	CARoadOptionStd		*pOptStd	= m_pDataMng->GetOptionStd();
	CPlateGirderApp		*pGir		= (CPlateGirderApp*)pBx->GetGirder();		
	CPlateGirderApp		*pGirR		= pGir->m_pRight;
	CPlateBasicIndex	*pBxNext = pBx->GetBxMatchByCrossBeam(pGirR);
	
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	
	if(!pC) return;

	double dWidthGir = pGir->GetWidthOnStation(pBx->GetStation(), bUpper);
	double dWidthGirR = pGirR->GetWidthOnStation(pBxNext->GetStation(), bUpper);		
	
	CDPoint vCrL = pBx->GetAngleSection();
	double  dStaCrL  = pGir->GetStationBySc(pBx->GetStation(),dWidthGir/2,vCrL);
	
	CDPoint vCrR = pBxNext->GetAngleSection();
	double  dStaCrR  = pGirR->GetStationBySc(pBxNext->GetStation(),-dWidthGirR/2,vCrR);
	
	CDPoint vGirL = pGir->GetLine()->GetAngleAzimuth(dStaCrL); // �Ŵ��� ������ ����
	CDPoint vGirR = pGirR->GetLine()->GetAngleAzimuth(dStaCrR);
	CDPoint vCross = pC->GetVectorPlan();

	CDPoint xyCrL(0,0), xyCrR(0,0), Sp1(0,0), Sp2(0,0), Ep1(0,0), Ep2(0,0);
	pC->GetXyBaseLR(xyCrL, xyCrR);

	///////////////////////////////////////
	CDomyun Dom(pDom);
	if(bUpper)	pOptStd->SetEnvType(&Dom, HCAD_STLH);
	else		pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint	xyBase	= bLeftGirder ? xyCrL : xyCrR;
	double	dT		= nLine==0 ? 0 : pC->m_T3_CR/2; // ���κ� ���÷���Ʈ T

	// �β��� : ���κ� ������ �� ���κ� ��, ���ǿܰ���
	if(bLeftGirder) 
	{
		GetXyMatchLineAndLine(xyBase, vGirL, xyBase+vCross.RotateInv90()*dT/2, vCross, Sp1);
		GetXyMatchLineAndLine(xyBase, vGirL, xyBase-vCross.RotateInv90()*dT/2, vCross, Sp2);
		Ep1 = xyBase+vCross*pC->m_P_CR+vCross.RotateInv90()*dT/2;
		Ep2 = xyBase+vCross*pC->m_P_CR-vCross.RotateInv90()*dT/2;
	}
	else 
	{
		GetXyMatchLineAndLine(xyBase, vGirR, xyBase+vCross.RotateInv90()*dT/2, vCross, Sp1);
		GetXyMatchLineAndLine(xyBase, vGirR, xyBase-vCross.RotateInv90()*dT/2, vCross, Sp2);
		Ep1 = xyBase-vCross*pC->m_P_CR+vCross.RotateInv90()*dT/2;
		Ep2 = xyBase-vCross*pC->m_P_CR-vCross.RotateInv90()*dT/2;
	}

	if(nLine != 1) Dom.LineTo(Sp1,Ep1);
	if(nLine >= 1) Dom.LineTo(Sp2,Ep2);
	*pDom << Dom;
}


void CAPlateDrawPyung::DrawCRBeamSplicePlateOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, BOOL bMakeChild)
{
	if(!pBx || !pBx->GetSection()) return;
	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBx->GetGirder();		
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	if(!pC) return;

	CPlateGirderApp	*pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CDPoint xyCrL(0,0), xyCrR(0,0), xyPoint(0,0);
	pC->GetXyBaseLR(xyCrL, xyCrR);
	
	CDPoint vCross = pC->GetVectorPlan();
	CDPoint vAng = -vCross.RotateInv(pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()));

	CDomyun Dom(pDom);

	if(bLeft)	xyPoint = xyCrL + vCross * pC->m_P_CR/fabs(vAng.y);
	else		xyPoint = xyCrR - vCross * pC->m_P_CR/fabs(vAng.y);

	if (bUpper) 
	{
		if((bLeft && pC->m_nLGirSpType) || (!bLeft && pC->m_nRGirSpType))
		{

			// ������
			pOptStd->SetEnvType(&Dom,HCAD_STLC);
			CPlatePieceObject pObj(pGir);
			pObj.m_nMarkBuje = pBx->IsJijum() ? CP_SP_U_U : CG_SP_U_U;
			pObj.m_nCol = bLeft ? 0 : 1;
			pC->CreatePieceObjPlate(&pObj);
			pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

			// �������
			pOptStd->SetEnvType(&Dom,HCAD_STLH);
			CPlatePieceObject pObj2(pGir);
			pObj2.m_bFront =TRUE;
			pObj2.m_nCol = bLeft ? 0 : 1;
			pObj2.m_nMarkBuje = pBx->IsJijum() ? CP_SP_U_L : CG_SP_U_L;
			pC->CreatePieceObjPlate(&pObj2);
			pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);

			pOptStd->SetEnvType(&Dom,HCAD_STLH);
			CPlatePieceObject pObj3(pGir);
			pObj3.m_bFront =FALSE;
			pObj3.m_nCol = bLeft ? 0 : 1;
			pObj3.m_nMarkBuje = pBx->IsJijum() ? CP_SP_U_L : CG_SP_U_L;
			pC->CreatePieceObjPlate(&pObj3);
			pObj3.DrawPieceOutline(&Dom, pOptStd, TRUE);
		}
	}
	else 
	{
		if((bLeft && pC->m_nLGirSpType) || (!bLeft && pC->m_nRGirSpType))
		{
			// �ϸ����
			pOptStd->SetEnvType(&Dom,HCAD_STLC);
			CPlatePieceObject pObj2(pGir);
			pObj2.m_nMarkBuje = pBx->IsJijum() ? CP_SP_L_U : CG_SP_L_U;
			pObj2.m_bFront =TRUE;
			pObj2.m_nCol = bLeft ? 0 : 1;
			pC->CreatePieceObjPlate(&pObj2);
			pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);

			pOptStd->SetEnvType(&Dom,HCAD_STLC);
			CPlatePieceObject pObj3(pGir);
			pObj3.m_nMarkBuje = pBx->IsJijum() ? CP_SP_L_U : CG_SP_L_U;
			pObj3.m_bFront =FALSE;
			pObj3.m_nCol = bLeft ? 0 : 1;
			pC->CreatePieceObjPlate(&pObj3);
			pObj3.DrawPieceOutline(&Dom, pOptStd, TRUE);

			// �ϸ�����
			if(pC->m_cGasset_CR)	// ����Ϻ������� ���� ��� �������� 2���� �и���.
			{
				pOptStd->SetEnvType(&Dom,HCAD_STLC);
				CPlatePieceObject pObj(pGir);
				pObj.m_nMarkBuje = pBx->IsJijum() ? CP_SP_L_U : CG_SP_L_U;
				pObj.m_nCol = bLeft ? 0 : 1;
				pObj.m_bFront =TRUE;
				pC->CreatePieceObjPlate(&pObj);
				pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

				CPlatePieceObject pObj2(pGir);
				pObj2.m_nMarkBuje = pBx->IsJijum() ? CP_SP_L_U : CG_SP_L_U;
				pObj2.m_nCol = bLeft ? 0 : 1;
				pObj2.m_bFront =FALSE;
				pC->CreatePieceObjPlate(&pObj2);
				pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);
			}
			else
			{
				pOptStd->SetEnvType(&Dom,HCAD_STLH);
				CPlatePieceObject pObj(pGir);
				pObj.m_nMarkBuje = pBx->IsJijum() ? CP_SP_L_L : CG_SP_L_L;
				pObj.m_nCol = bLeft ? 0 : 1;
				pC->CreatePieceObjPlate(&pObj);
				pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
			}

			if(pC->m_nRadioConnectionPlateInOut)
			{
				pOptStd->SetEnvType(&Dom,HCAD_STLH);
				CPlatePieceObject pObj(pGir);
				pObj.m_nMarkBuje = pBx->IsJijum() ? CP_SP_L_L : CG_SP_L_L;
				pObj.m_nCol = bLeft ? 0 : 1;
				pObj.m_bFront	= FALSE;
				pC->CreatePieceObjPlate(&pObj);
				pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
			}
		}
	}
	Dom.Rotate(CDPoint(0,0),vCross);
	Dom.Move(xyPoint);
	
	*pDom << Dom;
}
void CAPlateDrawPyung::DrawCRBeamSpliceBoltOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, 
										   BOOL bLeft, BOOL bHalfBoltOnly)
{
	if(pBx==NULL) return;
	CPlateGirderApp*  pGir    = (CPlateGirderApp*)pBx->GetGirder();		
	CPlateGirderApp*  pGirR   = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateCrossBeam*  pC	  = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd*  pOptStd = m_pDataMng->GetOptionStd();
	if(pC == NULL) return;
	//
	CDPoint xyCrL   = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint xyCrR   = pGirR->GetXyGirderDis(pBxNext->GetStation());
	CDPoint vCross = pC->GetVectorPlan();
	// ���κ��� ����ϰ� ������ �߽��� : cPoint
	CDPoint cPoint = xyCrL + vCross * pC->m_P_CR/pBx->GetAngleSection().y;
	if(!bLeft)
		cPoint = xyCrR - vCross * pC->m_P_CR/pBxNext->GetAngleSection().y;

	double A  = bUpper ? pC->m_uA	 : pC->m_dA;
	double C  = bUpper ? pC->m_uC	 : pC->m_dC;
	double C1 = bUpper ? pC->m_uC1	 : pC->m_dC1;
	double F  = bUpper ? pC->m_uF : pC->m_dF;
	double D  = bUpper ? pC->m_uD	 : pC->m_dD;
	long   B  = bUpper ? pC->m_uB	 : pC->m_dB;
	long   E  = bUpper ? pC->m_uE : pC->m_dE;
	double F1 = bUpper ? (pC->m_uM_CR-(D+E*F))*2 : (pC->m_dM_CR-(D+E*F))*2;
	double BoltSize = (pC->m_dBoltSize==0) ? 22 : pC->m_dBoltSize;
	double Slop = 0;

	// ���簢������ (0,0)�� �߽����� �׸��� �̵� & ȸ��
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_STLS);
	if((bLeft && pC->m_nLGirSpType) || (!bLeft && pC->m_nRGirSpType))
	{
		if(bHalfBoltOnly) {
			DstDrawBoltHole(&Dom,A,B,C,C1,D,E,F,BoltSize,Slop,Slop,F1,0,TRUE,FALSE);
			DstDrawBoltLine(&Dom,A,B,C,C1,D,E,F,BoltSize,Slop,Slop,F1,0,TRUE,FALSE);
			Dom.Move(0,bUpper?pC->m_uM_CR:pC->m_dM_CR);
		}
		else {
			DstDrawBoltHole(&Dom,A,B,C,C1,D,E,F,BoltSize,Slop,Slop,F1);
			DstDrawBoltLine(&Dom,A,B,C,C1,D,E,F,BoltSize,Slop,Slop,F1);
			Dom.Move(0,bUpper?pC->m_uM_CR:pC->m_dM_CR);
		}
	}
	// ��������� �ϸ� �ӽ�ó��
	if((bLeft && pC->m_nLGirSpType) || (!bLeft && pC->m_nRGirSpType))
	{
		if(bUpper) { 
			double dH  = bUpper ? pC->m_uM_CR : pC->m_dM_CR;	// ���κ��� ��
			double wid = A+B*C+C1/2;
			pOptStd->SetEnvType(&Dom,HCAD_STLH);
			Dom.LineTo(-wid, dH-pC->m_uW,wid, dH-pC->m_uW);
			Dom.LineTo(-wid,-dH+pC->m_uW,wid,-dH+pC->m_uW);
		}
	}
	Dom.Move(cPoint.x, cPoint.y);
	Dom.Rotate(cPoint, vCross);

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawCRBeamVStiffnerOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftHalf, BOOL bRightHalf)
{
	if(!pBx || !pBx->GetSection()) return;
	CPlateGirderApp		*pGir		= (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam		*pC			= pBx->GetSection()->GetCrossBeam();
	if(!pC) return;

	CARoadOptionStd		*pOptStd	= m_pDataMng->GetOptionStd();

	CDPoint vGir		= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vCross		= pC->GetVectorPlan();	// ���κ��� ����
	CDPoint kPoint((LPARAM).0), cPoint((LPARAM)0.0), xyCrL((LPARAM)0,0), xyCrR((LPARAM)0,0);	// kPoint:������������ �߽� (���κ��������� �߽�) 
	pC->GetXyBaseLR(xyCrL, xyCrR);
	double dRate	= pC->GetVectorPlan().RotateInv(vGir).y;

	long nK = pC->m_nVStiffsu_CR, n=0;
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	
	if(!pC->m_nLGirSpType)	cPoint = xyCrR - vCross * (pC->m_P_CR+dWSp/2)/fabs(dRate);
	else					cPoint = xyCrL + vCross * (pC->m_P_CR+dWSp/2)/fabs(dRate);
	if(pC->m_nFillerUseType==1)
	{
		double dThickFiller = pC->GetFillerThick();
		cPoint += vCross.Rotate90()*dThickFiller/2;
	}

	CDomyun Dom(pDom);
	
	if(bUpper)	pOptStd->SetEnvType(&Dom, HCAD_STLH);
	else		pOptStd->SetEnvType(&Dom, HCAD_STLC);

	if(!bLeftHalf)	n  = long( (nK+1)/2 );
	if(!bRightHalf)	nK = long( (nK+1)/2 );

	double kDist = 0;
	for(long k=0; k<n; k++)
		kDist += pC->m_VStiffDis_CR[k];

	double RotRad = vCross.GetAngleRadian();
	for(; n<nK; n++)
	{
		kDist  += pC->m_VStiffDis_CR[n];
		kPoint  = cPoint + vCross * kDist/fabs(dRate);
		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			kPoint = kPoint + vCross.RotateInv90() * (pC->m_vT_G+pC->m_T3_CR)/2;
		DstDrawVStiffPlan(&Dom,kPoint.x,kPoint.y,pC->m_vT_CR,pC->m_vW_CR,pC->m_T3_CR, RotRad, (int)pC->m_cVSide_CR);
	}
	*pDom << Dom;
}
// ��鵵 �Լ�
void CAPlateDrawPyung::DrawCRBeamAnchorBarOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, 
										   BOOL bLeftHalf, BOOL bRightHalf)
{
	if(!pBx || !pBx->GetSection()) return;
	CPlateGirderApp	*pGir	= (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam *pC		= pBx->GetSection()->GetCrossBeam();
	if(!pC) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDPoint xyCrCen	= pGir->GetXyGirderDis(pBx->GetStation());	
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vCross	= pC->GetVectorPlan();
	double	dRate	= vCross.RotateInv(vGir).y;
	CDPoint aPoint(0,0), cPoint(0,0);	// aPoint : ������������ �߽� (���κ��������� �߽�) 

	long nA = pC->m_nAnchorsu_CR, n=0;
	
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	double webT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	
	if(!pC->m_nLGirSpType)	cPoint = xyCrCen + vCross * (pC->GetLengthGirWebToCrWeb(TRUE)+webT/2)/fabs(dRate);
	else					cPoint = xyCrCen + vCross * (pC->m_P_CR+webT/2+dWSp/2)/fabs(dRate);
	if(pC->m_nFillerUseType==1)
	{
		double dThickFiller = pC->GetFillerThick();
		cPoint += vCross.Rotate90()*dThickFiller/2;
	}

	CDomyun Dom(pDom);
	double aDist = 0;
	pOptStd->SetEnvType(&Dom,HCAD_STLC);

	if(!bLeftHalf)	n  = long( (nA+1)/2 );
	if(!bRightHalf)	nA = long( (nA+1)/2 );

	for(long k=0; k<n; k++)
		aDist += pC->m_AnchorDis_CR[n];

	double Ang = pGir->GetAngleSlabAnchor(pGir->m_pSangse->m_SlabAnchor_T, pC->m_uM_CR);
	double RotRad = vCross.GetAngleRadian()-ToRadian(Ang);
	double A = pGir->m_pSangse->m_SlabAnchor_A;
	double B = pGir->m_pSangse->m_SlabAnchor_B;
	double C = pGir->m_pSangse->m_SlabAnchor_C;
	long nType = pGir->m_pSangse->m_SlabAnchorType;

	for(; n<nA; n++) 
	{
		aDist  += pC->m_AnchorDis_CR[n];
		aPoint  = cPoint + vCross * aDist/fabs(dRate);
		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			aPoint = aPoint+vCross.RotateInv90()*webT;
		Dom.AnchorBarPlan(aPoint.x, aPoint.y, pGir->m_pSangse->m_SlabAnchor_T, RotRad, A, B, C, nType);
		*pDom << Dom;
	}

	*pDom << Dom;	
}


void CAPlateDrawPyung::DrawVBracingAnchorBarOnBx(CDomyun *pDom, CPlateBasicIndex *pBx) 
{
	if(pBx==NULL) return;
	CPlateGirderApp	*pGir	= (CPlateGirderApp*)pBx->GetGirder();
	CSection		*pSec	= pBx->GetSection();
	CVBracing		*pVr	= pSec->GetVBracing();
	if(pVr == NULL) return;

	double dHeight		= pVr->GetSteelMaterialData(CVBracing::UPPER)->m_dB;//pSec->GetBeamBaseHeight(CSection::SEC_UP, TRUE, FALSE);
	double dBeamT2		= pSec->GetBeamJewon(CSection::SEC_UP, CSection::T2);
	double dTVStiff		= pSec->GetVStiff()->m_dThick[1];

	CDPoint xyCenL		= pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint vFrontUp	= pVr->GetVectorFrame(0);
	CDPoint vCrossPlan	= pVr->GetVectorPlan();	// ��� ����
	CDPoint vGir		= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vAng		= vCrossPlan.RotateInv(vGir);
	CDPoint xyBeamStt	= xyCenL + vCrossPlan*(pVr->m_dS+pVr->m_dG[0]*fabs(vFrontUp.x));

	double Ang		= pGir->GetAngleSlabAnchor(pGir->m_pSangse->m_SlabAnchor_T, dHeight/2);
	double RotRad	= vCrossPlan.GetAngleRadian()-ToRadian(Ang);

	double A = pGir->m_pSangse->m_SlabAnchor_A;
	double B = pGir->m_pSangse->m_SlabAnchor_B;
	double C = pGir->m_pSangse->m_SlabAnchor_C;

	long nType = pGir->m_pSangse->m_SlabAnchorType;
	long nSteelTypeUp = pVr->GetSteelMaterialData(STEELMAT_UPPER)->m_nType;
	
	CDPoint xyStt = xyBeamStt + vCrossPlan.RotateInv90()*(dHeight-dTVStiff)/2;
	
	if(nSteelTypeUp==CSteelMaterialData::CTSTEEL)
		xyStt = xyBeamStt + vCrossPlan.Rotate90()*(pSec->GetVStiff()->m_dThick[1]-dBeamT2)/2;

	for(long n=0; n<pVr->m_nAnchorSu; n++) 
	{
		xyStt += vCrossPlan*pVr->m_AnchorDis[n];
		pDom->AnchorBarPlan(xyStt.x, xyStt.y, pGir->m_pSangse->m_SlabAnchor_T, RotRad, A, B, C, nType);
	}
}

void CAPlateDrawPyung::DstDrawVStiffPlan(CDomyun *pDom, double BX, double BY, double Thick, 
										double Width, double MoThick, double rotAng, BOOL bSide)
{
	CDomyun Dom(pDom);
	// ������ �������� ���� PLATE
	if(bSide==0 || bSide==1)
		Dom.Rectangle(-Thick/2,-MoThick/2,  Thick/2,-MoThick/2-Width);
	if(bSide==0 || bSide==2)
		Dom.Rectangle(-Thick/2,MoThick/2, Thick/2,MoThick/2+Width);
	
	Dom.Rotate(rotAng,0,0);
	Dom.Move(BX,BY);
	*pDom << Dom;
	
}


void CAPlateDrawPyung::DstDrawBoltHole(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
									double dD, long nE, double dF, double BoltSize, 
									double SlopStt, double SlopEnd, double dF1,	double RadRotate, 
									BOOL bLeft, BOOL bRight, long nBoltArrange, CDPoint xy)
{
	if(nB == 0 && nE == 0) return;
	if(SlopEnd!=SlopStt && dF1>0)	return;

	CDomyun *pDom = new CDomyun(pDomP);

	double x,y;
	double SlopInc = (nE>0 && nBoltArrange==0) ? (SlopEnd-SlopStt)/nE : 0; // ���� ������
	double R = (BoltSize+3)/2;
	//////////////////////////////////
	// ��������
	long i,j;
	long nV = dF1>0 ? (nE+1)*2 : nE+1;
	double yPos;
	if(bLeft) {
		for(i=0; i<nB+1; i++) {
			x = -(nB*dC+dC1/2) + dC*i;
			yPos = -dD;
			for(j=0; j<nV; j++) {
				if(dF1>0 && j==(long)(nV/2)) 
					yPos -= dF1;
				else if(j>0)
					yPos -= dF;
				y = yPos + x*(SlopStt+SlopInc*j); 
				if(j==nE)  // ��Ʈ �Ϲݹ�ġ
					y = yPos + x*SlopEnd;
				pDom->Circle(x,y,R,TRUE);
			}
		}
	}
	//////////////////////////////////
	// ��������
	if(bRight) {
		for(i=0; i<nB+1; i++) {
			x = dC1/2 + dC*i;
			yPos = -dD;
			for(j=0; j<nV; j++) {
				if(dF1>0 && j==(long)(nV/2)) 
					yPos -= dF1;
				else if(j>0)
					yPos -= dF;
				y = yPos + x*(SlopStt+SlopInc*j);
				if(j==nE)  // ��Ʈ �Ϲݹ�ġ
					y = yPos + x*SlopEnd;
				pDom->Circle(x,y,R,TRUE);
			}
		}
	}

	pDom->Rotate(RadRotate,0,0);
	pDom->Move(xy);
	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawPyung::DstDrawBoltLine(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
									double dD, long nE, double dF, double BoltSize, 
									double SlopStt, double SlopEnd, double dF1,	double RadRotate, 
									BOOL bLeft, BOOL bRight, long nBoltArrange, CDPoint xy)
{
	if(nB == 0 && nE == 0) return;
	if(SlopEnd!=SlopStt && dF1>0)	return;

	CDomyun *pDom = new CDomyun(pDomP);

	double xStt,xEnd,yStt,yEnd;
	double SlopInc = (nE>0 && nBoltArrange==0) ? (SlopEnd-SlopStt)/nE : 0; // ���� ������
	//////////////////////////////////
	// �������(��) 
	long i;
	long nV = dF1>0 ? (nE+1)*2 : nE+1;
	double yPos; // ��Ʈ�߽��� ��ġ
	if(bLeft) {
		xStt = -(nB*dC+dC1/2);
		xEnd = bRight ? 0 : -dC1/2;
		yPos =  -dD;
		for(i=0; i<nV; i++) {
			if(dF1>0 && i==(long)(nV/2)) 
				yPos -= dF1;
			else if(i>0)
				yPos -= dF;
			yStt = yPos + xStt*(SlopStt+SlopInc*i);
			yEnd = yPos + xEnd*(SlopStt+SlopInc*i);
			if(i==nE) { // ��Ʈ �Ϲݹ�ġ
				yStt = yPos + xStt*SlopEnd;
				yEnd = yPos + xEnd*SlopEnd;
			}
			pDom->LineTo(xStt, yStt, xEnd, yEnd);
		}
	}
	// �������(��) 
	if(bRight) {
		xStt = bLeft ? 0 : dC1/2;
		xEnd = (nB*dC+dC1/2);
		yPos =  -dD;
		for(i=0; i<nV; i++) {
			if(dF1>0 && i==(long)(nV/2)) 
				yPos -= dF1;
			else if(i>0)
				yPos -= dF;
			yStt = yPos + xStt*(SlopStt+SlopInc*i);
			yEnd = yPos + xEnd*(SlopStt+SlopInc*i);
			if(i==nE) { // ��Ʈ �Ϲݹ�ġ
				yStt = yPos + xStt*SlopEnd;
				yEnd = yPos + xEnd*SlopEnd;
			}
			pDom->LineTo(xStt, yStt, xEnd, yEnd);
		}
	}
	//////////////////////////////////
	// ��������(��) 
	if(bLeft) {
		xStt = -(nB*dC+dC1/2) - dC;
		for(i=0; i<nB+1; i++) {
			xStt += dC;
			yStt  = -dD + xStt*SlopStt;
			if(dF1>0)
				yEnd  = -dD -(nE*dF)*2 - dF1 + xStt*SlopEnd;
			else
				yEnd  = -dD -nE*dF + xStt*SlopEnd;
			pDom->LineTo(xStt, yStt, xStt, yEnd);
		}
	}
	// ��������(��) 
	if(bRight) {
		xStt = dC1/2 - dC;
		for(i=0; i<nB+1; i++) {
			xStt += dC;
			yStt  = -dD + xStt*SlopStt;
			if(dF1>0)
				yEnd  = -dD -(nE*dF)*2 - dF1 + xStt*SlopEnd;
			else
				yEnd  = -dD -nE*dF + xStt*SlopEnd;
			pDom->LineTo(xStt, yStt, xStt, yEnd);
		}
	}

	pDom->Rotate(RadRotate,0,0);
	pDom->Move(xy);
	*pDomP << *pDom;
	delete pDom;
}


void CAPlateDrawPyung::DrawGirderNumber(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBx->GetGirder();			
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_SYMB);
	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		CString szText;
		szText.Format("G%d",nG+1);
		double dSta = pBx->GetStation();
		double dStaCen = pBridge->GetGirder(-1)->GetStationBySc(dSta, -pGir->m_dGirderCenter, pBx->GetAngleSection());						
		double dStaReal = pBridge->GetGirder(-1)->GetStationBySc(dStaCen, pBridge->GetGirder(nG)->m_dGirderCenter, pBx->GetAngleSection());						
		TextCircle(&Dom, nG, dStaReal+600, pBx->GetAngleSection(), 0, szText);
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawVBracingBeamDimAllDanmyun(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	 *pBridge = m_pDataMng->GetBridge();

	long nGirsu = pBridge->GetQtyGirder();
	double dX[20];
	long   nG = 0;
	for(nG=0; nG<nGirsu; nG++)
		dX[nG] = pBridge->GetGirder(nG)->m_dGirderCenter;
	
	CDomyun Dom(pDom);

	CPlateGirderApp	*pGir = pBridge->GetGirder(0);
	CVStiff	        *pVS  = pBx->GetBxMatchByCrossBeam(pGir)->GetSection()->GetVStiff();
	double dWidth = pBridge->GetWidthAct(pBx->GetStation());

	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	//CDPoint vPlan	= pBx->GetAngleSectionCrossVBracing();
	CDPoint vPlan	= pBx->GetAngleSection();
	double dDistY   = Dom.Always(500)/Dom.GetScaleDim();
	double dUWidth  = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);
	double dLWidth  = pGir->GetWidthOnStation(pBx->GetStation(),FALSE);	
	
	Dom.SetDirection("BOTTOM");
	double dL=0, dY=0;
	for(nG=0; nG<nGirsu; nG++)
	{
		if(nG==0)	dL=0;
		else		dL = fabs(dX[nG-1]-dX[nG]);
		dY += -dL*vPlan.x/vPlan.y;
		if(bUpper)
		{
			Dom.DimMoveTo(dX[nG]-dUWidth/2, -dDistY+dY, 0);
			Dom.DimLineTo(dUWidth, 0, COMMA(dUWidth));
		}
		else
		{
			Dom.DimMoveTo(dX[nG]-dLWidth/2, -dDistY+dY, 0);
			Dom.DimLineTo(dLWidth, 0, COMMA(dLWidth));
		}
	}

	*pDom << Dom;

	long nDan = 0;
	if(vPlan.x/vPlan.y<0)
		dDistY = Dom.Always(500)/Dom.GetScaleDim() + dWidth*fabs(vPlan.x/vPlan.y);

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dX[0], dDistY, nDan);

	for(nG=0; nG<nGirsu-1; nG++)
	{
		pGir	= pBridge->GetGirder(nG);
		CPlateBasicIndex	*pBxL	= pBx->GetBxMatchByCrossBeam(pGir);
		CPlateBasicIndex	*pBxR	= pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
		CVBracing			*pVr	= pBx->GetBxMatchByCrossBeam(pGir)->GetSection()->GetVBracing();
		if(!pVr)
		{
			*pDom << Dom;
			return;
		}
		CVStiff *pVStiffL = pBxL->GetSection()->GetVStiff();
		CVStiff *pVStiffR = pBxR->GetSection()->GetVStiff();

		//////////////////////////////////////////////////////////////////////////
		CDPoint A[10];
		pGir->GetHunchXyDom(pBx, A);

		CDPoint xyCenL		= pGir->GetXyGirderDis(pBxL->GetStation());
		CDPoint xyCenR		= pGir->m_pRight->GetXyGirderDis(pBxR->GetStation());
		CDPoint vCrossPlan	= pVr->GetVectorPlan();
		CDPoint vGir		= pGir->GetLine()->GetAngleAzimuth(pBxL->GetStation());
		double	dAng		= fabs(vCrossPlan.RotateInv(vGir).y);

		double dSteelH	= pVr->GetSteelMaterialData(CVBracing::UPPER)->m_dH;
		double dTStiffL = pVStiffL ? pVStiffL->m_dThick[1] : 0;
		double dTStiffR = pVStiffR ? pVStiffR->m_dThick[0] : 0;
		double GTerm = ~(xyCenL-xyCenR);
		
		CString str = _T(""), str1 = _T(""), str2 = _T("");

		CDPoint vFrontUp	= pVr->GetVectorFrame(0);
		CDPoint xyBeamStt = xyCenL + vCrossPlan*(pVr->m_dS+pVr->m_dG[0]*fabs(vFrontUp.x)) + vCrossPlan.RotateInv90()*(dSteelH-dTStiffL)/2;
		CDPoint xyBeamEnd = xyCenR - vCrossPlan*(pVr->m_dS+pVr->m_dG[1]*fabs(vFrontUp.x)) + vCrossPlan.RotateInv90()*(dSteelH-dTStiffR)/2;
		xyCenL		= pGir->GetXyGirderDis(pBxL->GetStation()-(dSteelH-dTStiffL)/2/dAng);
		xyCenR		= pGir->m_pRight->GetXyGirderDis(pBxR->GetStation()-(dSteelH-dTStiffR)/2/dAng);
		double dStt = ~(xyCenL-xyBeamStt);
		double dEnd = ~(xyCenR-xyBeamEnd);
		dStt = dStt*fabs(pBxL->GetAngleSection().y);
		dEnd = dEnd*fabs(pBxL->GetAngleSection().y);

		double dLen = 0;
		BOOL bSkew = pVr->IsSkew();
		
		dLen = dStt;
		str1.Format("%s", COMMA(dLen*dAng));
		if(bSkew)	str2.Format("(%s)", COMMA(dLen));
		else		str2.Format("");
		Dom.DimLineToExtend(dLen*dAng, nDan, str1, str2);

		double disSum = 0;
		for(long i = 0 ; i < pVr->m_nAnchorSu; i++)
		{
			disSum += pVr->m_AnchorDis[i];

			dLen = pVr->m_AnchorDis[i];
			str1.Format("%s", COMMA(dLen*dAng));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen*dAng, nDan, str1, str2);
		}

		dLen = GTerm-disSum-(dStt+dEnd);
		str1.Format("%s", COMMA(dLen*dAng));
		if(bSkew)	str2.Format("(%s)", COMMA(dLen));
		else		str2.Format("");
		Dom.DimLineToExtend(dLen*dAng, nDan, str1, str2);

		dLen = dEnd;
		str1.Format("%s", COMMA(dLen*dAng));
		if(bSkew)	str2.Format("(%s)", COMMA(dLen));
		else		str2.Format("");
		Dom.DimLineToExtend(dLen*dAng, nDan, str1, str2);		
	}
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawCRBeamDimAllDanmyun(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	 *pBridge = m_pDataMng->GetBridge();
	CPlateBridgeApp *pDB			= m_pDataMng->GetBridge();

	long nGirsu = pBridge->GetGirdersu();
	double dSumGir = 0;
	double dX[20];
	long   nG = 0;
	for(nG=0; nG<nGirsu; nG++)
	{
		CPlateGirderApp	*pGir1 = pBridge->GetGirder(nG);
		dX[nG] = pGir1->m_dGirderCenter;
	}
	
	CDomyun Dom(pDom);

	CPlateGirderApp	*pGir = pBridge->GetGirder(0);
	CPlateCrossBeam	*pC1  = pBx->GetBxMatchByCrossBeam(pGir)->GetSection()->GetCrossBeam();
	CVStiff	        *pVS  = pBx->GetBxMatchByCrossBeam(pGir)->GetSection()->GetVStiff();
	double dWidth = pBridge->GetWidthAct(pBx->GetStation());

	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	// #APLATE-661, ��ȣ�� ������ ���� ������ �����Ѵ�. (�Ŵ��� ���κ��� 90���� �ƴϸ� ��ȣ�� ���)
	CDPoint vPlan = pBx->GetAngleSectionCrossVBracing();					// ���κ��� ������ �����´�. 
	CDPoint vGirder = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());	// �Ŵ��� ������ �����´�.
	CDPoint vDir = vPlan.BetweenAngle(vGirder);
	BOOL bSagag = EQUAL(fabs(vDir.y),1, 0.001) ? FALSE : TRUE;

	double dCRBeamW = pC1->m_uM_CR;
	double dCRBeamT = pC1->m_T3_CR;
	double dVStiffT = pVS->m_dThick[1];
	double dDistY   = Dom.Always(500)/Dom.GetScaleDim();
	double dUWidth  = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);
	double dLWidth  = pGir->GetWidthOnStation(pBx->GetStation(),FALSE);	

	CDPoint vAng = -vPlan.RotateInv(pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()));
	CDPoint vAng90(0,1);
	BOOL bSkewSta = ComparePt(vAng90, vAng, _T("="), 0.1)? FALSE : TRUE;

	double dTotL = 0;
	CLongArray nArrBeam;
	pDB->GetIndexGirderToCrossBeam(nArrBeam, pBx, TRUE);
	for (nG =0; nG < nArrBeam.GetAt(0); nG++)
	{
		dTotL += fabs(dX[nG]);
	}


	Dom.SetDirection("BOTTOM");	
	double dL=0, dY=0;
	double dYTmp = 0;
	for(nG=0; nG<nGirsu; nG++)
	{
		BOOL bMat = FALSE;
		for(long ix =0; ix < nArrBeam.GetSize(); ix++)
		{
			if(nG == nArrBeam.GetAt(ix))
			{
				bMat = TRUE;
				break;
			}
		}
		if(bMat == FALSE)
			continue;
	
		if(nG==0)
			dL=0;
		else
			dL = fabs(dX[nG-1]-dX[nG]);

		
		dY += -dL*vAng.x/vAng.y;
		dYTmp += -dL * vAng90.x / vAng90.y;	

		if(bUpper)
		{
			Dom.DimMoveTo(dX[nG]-dUWidth/2 - dTotL, -dDistY+dY, 0);
			Dom.DimLineTo(dUWidth, 0, COMMA(dUWidth));
		}
		else
		{
			Dom.DimMoveTo(dX[nG]-dLWidth/2 - dTotL, -dDistY+dY, 0);
			Dom.DimLineTo(dLWidth, 0, COMMA(dLWidth));
		}		
	}

	*pDom << Dom;

	pDB->GetIndexGirderToCrossBeam(nArrBeam, pBx, FALSE);

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(dX[0], -dCRBeamW-dVStiffT/2-dCRBeamT/2, 0, "");
	Dom.DimLineTo(dCRBeamW*2, 0, COMMA(dCRBeamW*2));
	Dom.Rotate(-vAng.x/vAng.y, dX[0], -dCRBeamW-dVStiffT/2-dCRBeamT/2);
	*pDom << Dom;

	if(vPlan.x/vPlan.y<0)
	{
		double dDiff =dY - dYTmp;
		dDistY = Dom.Always(500)/Dom.GetScaleDim() + dWidth*fabs(vAng.x/vAng.y) +dDiff;
	}

	long nDan = 0;
	for(nG=0; nG<nGirsu-1; nG++)
	{
		BOOL bMat = FALSE;
		for(long ix =0; ix < nArrBeam.GetSize(); ix++)
		{
			if(nG == nArrBeam.GetAt(ix))
			{
				bMat = TRUE;
				break;
			}
		}
		if(bMat == FALSE)
			continue;

		nDan = 0;
		CPlateGirderApp	 *pGir	 = pBridge->GetGirder(nG);
		CPlateCrossBeam	 *pC	 = pBx->GetBxMatchByCrossBeam(pGir)->GetSection()->GetCrossBeam();
		if(pC == NULL)	return;
		Dom.SetDirection("TOP");

		CDPoint A[10];
		pC->GetCrossXyDom(A);

		long nA = pC->m_nAnchorsu_CR;
		long nV = pC->m_nVStiffsu_CR;
		double GTerm = pGir->GetDisFrontPlateWebToWeb(pBx);
		CDPoint vAng = pBx->GetAngleSection().RotateInv90();
		double dWebT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
		double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
		double disSum = 0;

		// �Ѵ� ��Ʈ������ ���
		double temp=0;
		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			if(bUpper)
			{
				//1��
				if(nA>0)	// ��Ŀ�� ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2, dDistY, nDan,"");
					for(long n=0; n<nA; n++)
					{
						if(n==0)
						{
							temp = pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2+pC->m_AnchorDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						else
						{
							temp = pC->m_AnchorDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						disSum += pC->m_AnchorDis_CR[n];
					}
					if(disSum==0)
					{
						temp = GTerm+dWebT;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs((temp/vPlan.y))) + ")" : COMMA(temp));
					}
					else
					{
						temp = GTerm-disSum-pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs((temp/vPlan.y))) + ")" : COMMA(temp));
					}
					nDan++;
				}
			}
			else
			{
				//1��
				if(nV>0)	//���������簡 ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2,  dDistY, nDan,"");
					for(long n=0; n<nV; n++)
					{
						if(n==0)
						{
							temp = pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2+pC->m_VStiffDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						else
						{
							temp = pC->m_VStiffDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						disSum += pC->m_VStiffDis_CR[n];
					}
					if(disSum==0)
					{
						temp = GTerm+dWebT;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					}
					else
					{
						temp = GTerm-disSum-pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					}
					nDan++;
				}
			}
			//2��
			Dom.DimMoveTo(A[8].x+dWebT/2, dDistY, nDan, "");
			temp = GTerm+dWebT;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
			nDan++;
		}
		// ������ ������ �� ���
		else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			//1��
			Dom.DimMoveTo(A[8].x+dWebT/2,  dDistY, nDan,"");
			temp = pC->m_P_CR+dWebT/2;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
			temp = GTerm-pC->m_P_CR+dWebT/2;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));

			nDan++;
			if(bUpper)
			{
				//2��
				if(nA>0)	// ��Ŀ�� ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2,  dDistY, nDan,"");
					for(long n=0; n<nA; n++)
					{
						if(n==0)
						{
							temp = dWebT/2+pC->m_P_CR+dWSp/2+pC->m_AnchorDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						else
						{
							temp = pC->m_AnchorDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						disSum += pC->m_AnchorDis_CR[n];
					}
					if(disSum==0)
					{
						temp = GTerm+dWebT;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					}
					else
					{
						temp = GTerm-disSum-pC->m_P_CR-dWSp/2+dWebT/2;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					}
					nDan++;
				}
			}
			else
			{
				//2��
				if(nV>0)	// ���������簡 ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2,  dDistY, nDan,"");
					for(long n=0; n<nV; n++)
					{
						if(n==0)
						{
							temp = dWebT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						else
						{
							temp = pC->m_VStiffDis_CR[n];
							Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						}
						disSum += pC->m_VStiffDis_CR[n];
					}
					if(disSum==0)
					{
						temp = GTerm+dWebT;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					}
					else
					{
						temp = GTerm-disSum-pC->m_P_CR-dWSp/2+dWebT/2;
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					}
					nDan++;
				}
			}
		}
		// ������ �������� ���
		else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
		{
			//1��
			Dom.DimMoveTo(A[8].x+dWebT/2,  dDistY, nDan,"");
			temp = GTerm-pC->m_P_CR+dWebT/2;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
			temp = pC->m_P_CR+dWebT/2;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));

			nDan++;
			if(bUpper)
			{
				//2��
				if(nA>0)	// ��Ŀ�� ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2,  dDistY, nDan,"");
					for(long n=0; n<nA; n++)
					{
						if(n==0)
							temp = pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2+pC->m_AnchorDis_CR[n];
						else
							temp = pC->m_AnchorDis_CR[n];
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						disSum += pC->m_AnchorDis_CR[n];
					}
					if(disSum==0)
						temp = GTerm+dWebT;
					else
						temp = GTerm-disSum-pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2;
					Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					nDan++;
				}
			}
			else
			{
				//2��
				if(nV>0)	// ���������簡 ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2,  dDistY, nDan,"");
					for(long n=0; n<nV; n++)
					{
						if(n==0)
							temp = pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2+pC->m_VStiffDis_CR[n];
						else
							temp = pC->m_VStiffDis_CR[n];
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						disSum += pC->m_VStiffDis_CR[n];
					}
					if(disSum==0)
						temp = GTerm+dWebT;
					else
						temp = GTerm-disSum-pC->GetLengthGirWebToCrWeb(TRUE)+dWebT/2;
					Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					nDan++;
				}
			}
		}
		// �Ѵ� �������� ���
		else
		{
			//1��
			Dom.DimMoveTo(A[8].x+dWebT/2 -dTotL, dDistY, nDan,"");
			temp = pC->m_P_CR+dWebT/2;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
			temp = GTerm-pC->m_P_CR*2;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
			temp = pC->m_P_CR+dWebT/2;
			Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
			nDan++;
			if(bUpper)
			{
				//2��
				if(nA>0)	// ��Ŀ�� ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2 - dTotL, dDistY, nDan,"");
					for(long n=0; n<nA; n++)
					{
						if(n==0)
							temp = dWebT/2+pC->m_P_CR+dWSp/2+pC->m_AnchorDis_CR[n];
						else
							temp = pC->m_AnchorDis_CR[n];
						Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						disSum += pC->m_AnchorDis_CR[n];
					}
					if(disSum==0)
						temp = GTerm+dWebT;
					else
						temp = GTerm-disSum-pC->m_P_CR-dWSp/2+dWebT/2;
					Dom.DimLineTo(temp, nDan, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					nDan++;
				}
			}
			else
			{
				//2��
				if(nV>0)	// ���������簡 ���� ���
				{
					Dom.DimMoveTo(A[8].x+dWebT/2, dDistY, nDan,"");
					for(long n=0; n<nV; n++)
					{
						if(n==0)
							temp = dWebT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n];
						else
							temp = pC->m_VStiffDis_CR[n];
						Dom.DimLineTo(temp, 1, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
						disSum += pC->m_VStiffDis_CR[n];
					}
					if(disSum==0)
						temp = GTerm+dWebT;
					else
						temp = GTerm-disSum-pC->m_P_CR-dWSp/2+dWebT/2;
					Dom.DimLineTo(temp, 1, bSagag ? COMMA(temp) + "(" + COMMA(fabs(temp/vPlan.y)) + ")" : COMMA(temp));
					nDan++;
				}
			}
		}
		
		dSumGir += dX[nG+1]-dX[nG];
	}


	Dom.DimMoveTo(dX[0], dDistY, nDan,"");
	Dom.DimLineTo(dSumGir, nDan, bSagag ? COMMA(dSumGir) + "(" + COMMA(fabs(dSumGir/vPlan.y)) + ")" : COMMA(dSumGir));

	*pDom << Dom;
}
// dLineThick = �� �β�
// nPos       = 0 : �Ŵ� �߽�
//              -1: �Ŵ����� 
//               1: �Ŵ������� 
// nLineColor = -1: SetEnvType(&Dom, HCAD_DIML); ������ ����..
// nLineColor �� -1�ƴϸ� �� ������ ������.
void CAPlateDrawPyung::SetCursorPosition(long nG, double staStt, double staEnd, CDomyun *pDom, BOOL bDelCursor, 
										 double dLineThick, long nPos, long nLineColor)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();

	if(pDom == NULL)
		return;
	
	if(bDelCursor) pDom->DeleteCursor();

	CPlateGirderApp	*pGir = pBridge->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER|BX_END_GIRDER);
	if(!pBx) return;

	double dUWidth = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);
	double dLWidth = pGir->GetWidthOnStation(pBx->GetStation(),FALSE);
	double width   = dUWidth>dLWidth ? dUWidth/2 : dLWidth/2;
	BOOL bChangeLastColor = TRUE;

	CDPoint xy = pGir->GetXyGirderDis(staEnd);
	xy = GetXyRotateVector(CDPoint (0,0), xy, pBridge->GetAngleBridgeCenter().GetMirrorVert());

	CDPoint vDir = pGir->GetLine()->GetAngleAzimuth(staEnd);
	double  ang  = vDir.Rotate(pBridge->GetAngleBridgeCenter().GetMirrorVert()).Rotate90().GetAngleRadian();

	pDom->MakeCursor(xy.x, xy.y, ang);

	if(staStt >= 0.0)  
	{
		CDomyun Dom(pDom);
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		if(nLineColor!=-1)
			Dom.SetLineColor(nLineColor);

		Dom.SetPLineThick(dLineThick);
		double dL = 250+width;


		if(nPos==2)
		{
				CDPoint p1, p2, p3, p4;
				p1 = pGir->GetXyGirderDis(staStt, -dL);
				p2 = pGir->GetXyGirderDis(staStt,  dL);
				p3 = pGir->GetXyGirderDis(staEnd, -dL);
				p4 = pGir->GetXyGirderDis(staEnd,  dL);

				//Dom.SetPLineThick(0);
				Dom.PMoveTo(p1.x,p1.y,0,TRUE);
				Dom.PLineTo(p2.x,p2.y);
				Dom.PLineTo(p4.x,p4.y);
				Dom.PLineTo(p3.x,p3.y);
				Dom.PLineEnd();
		}
		else
		{
			CDPoint xy1, xy2, xy3, xy4, xy5;
			if(nPos==0)
			{
				xy1 = pGir->GetXyGirderDis(staStt, -dL);
				xy2 = pGir->GetXyGirderDis(staStt,  dL);
			}
			else if(nPos==-1)
			{
				xy1 = pGir->GetXyGirderDis(staStt, -dL*2);
				xy2 = pGir->GetXyGirderDis(staStt, -dL);
				xy3 = xy2-(xy1-xy2).Unit()*dLineThick;
				xy4 = xy3+(xy1-xy2).Unit().Rotate(ToDPointFrDegree(30))*dLineThick*2;
				xy5 = xy3+(xy1-xy2).Unit().Rotate(ToDPointFrDegree(-30))*dLineThick*2;
				Dom.Solid(xy3.x, xy3.y, xy4.x, xy4.y, xy5.x, xy5.y, xy3.x, xy3.y); 
			}
			else
			{
				xy1 = pGir->GetXyGirderDis(staStt,  dL);
				xy2 = pGir->GetXyGirderDis(staStt,  dL*2);
				xy3 = xy1-(xy2-xy1).Unit()*dLineThick;
				xy4 = xy3+(xy2-xy1).Unit().Rotate(ToDPointFrDegree(30))*dLineThick*2;
				xy5 = xy3+(xy2-xy1).Unit().Rotate(ToDPointFrDegree(-30))*dLineThick*2;
				Dom.Solid(xy3.x, xy3.y, xy4.x, xy4.y, xy5.x, xy5.y, xy3.x, xy3.y); 
			}
			Dom.PMoveTo(xy1.x, xy1.y);
			Dom.PLineTo(xy2.x, xy2.y);
			Dom.PLineEnd();
			if(nPos==0)
			{
				xy1 = pGir->GetXyGirderDis(staEnd, -dL);
				xy2 = pGir->GetXyGirderDis(staEnd,  dL);
			}
			else if(nPos==-1)
			{
				xy1 = pGir->GetXyGirderDis(staEnd, -dL*2);
				xy2 = pGir->GetXyGirderDis(staEnd, -dL);
				xy3 = xy2-(xy1-xy2).Unit()*dLineThick;
				xy4 = xy3+(xy1-xy2).Unit().Rotate(ToDPointFrDegree(30))*dLineThick*2;
				xy5 = xy3+(xy1-xy2).Unit().Rotate(ToDPointFrDegree(-30))*dLineThick*2;
				Dom.Solid(xy3.x, xy3.y, xy4.x, xy4.y, xy5.x, xy5.y, xy3.x, xy3.y); 
			}
			else
			{
				xy1 = pGir->GetXyGirderDis(staEnd,  dL);
				xy2 = pGir->GetXyGirderDis(staEnd,  dL*2);
				xy3 = xy1-(xy2-xy1).Unit()*dLineThick;
				xy4 = xy3+(xy2-xy1).Unit().Rotate(ToDPointFrDegree(30))*dLineThick*2;
				xy5 = xy3+(xy2-xy1).Unit().Rotate(ToDPointFrDegree(-30))*dLineThick*2;
				Dom.Solid(xy3.x, xy3.y, xy4.x, xy4.y, xy5.x, xy5.y, xy3.x, xy3.y); 
			}
			if(bChangeLastColor && nLineColor==-1)
				Dom.SetLineColor(6);
			Dom.PMoveTo(xy1.x, xy1.y);
			Dom.PLineTo(xy2.x, xy2.y);
			Dom.PLineEnd();
		}



		Dom.Rotate(CDPoint (0,0), pBridge->GetAngleBridgeCenter().GetMirrorVert());	

		*pDom << Dom;
		pDom->Invalidate();
	}
}

void CAPlateDrawPyung::DrawAbutPier(CDomyun *pDom)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();

	CDomyun Dom(pDom);	
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	pOptStd->SetEnvType(&Dom, HCAD_DIML, LT_DASHDOT_LINE);	
	
	for(long i=0;i<pBridge->m_nQtyJigan+1;i++)
	{
		CPierInfo *pPI = pBridge->m_PierInfo + i;
		CPlateGirderApp *pGirCR = pBridge->GetGirder(-1);
		CPlateGirderApp *pGirLeft = pBridge->GetGirder(0);
		CPlateGirderApp *pGirRight = pBridge->GetGirder(pBridge->GetGirdersu()-1);
		if(pPI->IsAbut())
		{
			double dWidth = pPI->m_daT4 - pPI->m_daT3;
			double dStaCR = (i==0) ? pGirCR->GetStationStt()  : pGirCR->GetStationEnd();			
			double dStaLeft  = (i==0) ? pGirLeft->GetStationStt()  : pGirLeft->GetStationEnd();			
			double dStaRight  = (i==0) ? pGirRight->GetStationStt()  : pGirRight->GetStationEnd();			
			CDPoint dAngCR = (i==0) ? pGirCR->GetAngleStt() : pGirCR->GetAngleEnd();
			CDPoint dAngLeft = (i==0) ? pGirLeft->GetAngleStt() : pGirLeft->GetAngleEnd();
			CDPoint dAngRight = (i==0) ? pGirRight->GetAngleStt() : pGirRight->GetAngleEnd();
			CDPoint LeftXY  = pGirLeft->GetXyGirderDis(dStaLeft,-pBridge->m_dWidthSlabLeft,dAngLeft);	
			CDPoint RightXY = pGirRight->GetXyGirderDis(dStaRight, pBridge->m_dWidthSlabLeft,dAngRight);
			CDPoint vDir = (RightXY - LeftXY).Unit();	

			CDPoint LeftXY1  =  LeftXY - ((i==0) ? vDir.Rotate90() : vDir.RotateInv90()) * pPI->m_daT3;
			CDPoint RightXY1 = RightXY - ((i==0) ? vDir.Rotate90() : vDir.RotateInv90()) * pPI->m_daT3;
			CDPoint LeftXY2  = LeftXY + ((i==0) ? vDir.Rotate90() : vDir.RotateInv90()) * dWidth;
			CDPoint RightXY2 = RightXY + ((i==0) ? vDir.Rotate90() : vDir.RotateInv90()) * dWidth;
			
			CDPoint CrossXY1, CrossXY2;
			CDPoint vVec = pGirCR->GetLine()->GetAngleAzimuth(dStaCR);			
			GetXyMatchSegAndLine(LeftXY2,RightXY2,LeftXY1,vVec,CrossXY1);
			GetXyMatchSegAndLine(LeftXY2,RightXY2,RightXY1,vVec,CrossXY2);
			
			Dom.LineTo(LeftXY1,CrossXY1);	
			Dom.LineTo(RightXY1,CrossXY2);
						
			Dom.LineTo(LeftXY1,RightXY1);
			Dom.LineTo(CrossXY1,CrossXY2);
		}
		else
		{			
			CPierInfo *pPI = pBridge->m_PierInfo + i;	

			double dStaCR = pGirCR->GetStationOnJijum(i);
			double dStaLeft  = pGirLeft->GetStationOnJijum(i);			
			double dStaRight  = pGirRight->GetStationOnJijum(i);			
			CDPoint dAngCR = pGirCR->GetAngleOnJijum(i);			
			if(i == 0) 
			{
				dStaCR = pGirCR->GetStationStt();	 	
				dAngCR = pGirCR->GetAngleStt();
			}
			if(i == pBridge->m_nQtyJigan) 
			{
				dStaCR = pGirCR->GetStationEnd();	 	
				dAngCR = pGirCR->GetAngleEnd();		
			}						
			
			CDPoint vPierVec;			
			CPlateGirderApp* pGirL = pBridge->GetGirder(0);
			CPlateGirderApp* pGirR = pBridge->GetGirder(pBridge->GetGirdersu()-1);
			CDPoint vAngLeft = pGirL->GetAngleOnJijum(i);
			CDPoint vAngRight = pGirR->GetAngleOnJijum(i);
			CDPoint vLeftPoint = pGirL->GetXyGirderDis(pGirL->GetStationOnJijum(i),0,vAngLeft);	
			CDPoint vRightPoint = pGirR->GetXyGirderDis(pGirR->GetStationOnJijum(i),0,vAngRight);	
			double dWidth = ~(vLeftPoint - vRightPoint);
			double dEdge = fabs((pPI->GetHorWidth() - dWidth - pPI->m_dpR*2)/2);
			
			CDPoint LeftXY    = pGirLeft->GetXyGirderDis(dStaLeft,-dEdge,vAngLeft);	
			CDPoint RightXY   = pGirRight->GetXyGirderDis(dStaRight,dEdge,vAngRight);
			CDPoint vDir = (RightXY - LeftXY).Unit();
			if(vDir.IsNull()) vDir = (pGirRight->GetXyGirderDis(dStaRight,100,vAngRight) - 
									  pGirLeft->GetXyGirderDis(dStaLeft,-100,vAngLeft)).Unit();	

			CDPoint LeftXY1   = LeftXY + vDir.RotateInv90() * pPI->m_dpP;
			CDPoint LeftXY2   = LeftXY + vDir.Rotate90() * pPI->m_dpN;				
			CDPoint RightXY1  = RightXY + vDir.RotateInv90() * pPI->m_dpP;
			CDPoint RightXY2  = RightXY + vDir.Rotate90() * pPI->m_dpN;
			CDPoint CrossXY1, CrossXY2;

			BOOL bCross = GetXyMatchSegAndLine(LeftXY2,RightXY2,LeftXY1,vDir.Rotate90(),CrossXY1);
			if(pPI->m_dpR == 0)					
				Dom.LineTo(LeftXY1,CrossXY1);					
			else
			{				
				if(!bCross) CrossXY1 = RightXY2;
				CDPoint pArcVec = (CrossXY1 - LeftXY1).Unit();				
				CDPoint CenPo = (LeftXY1 + CrossXY1) / 2 + pArcVec.Rotate90() * pPI->m_dpR;												
				Dom.Arc(CrossXY1.x,CrossXY1.y,CenPo.x,CenPo.y,LeftXY1.x,LeftXY1.y);
			}										
						
			bCross = GetXyMatchSegAndLine(LeftXY2,RightXY2,RightXY1,vDir.Rotate90(),CrossXY2);				
			if(pPI->m_dpR == 0)
				Dom.LineTo(RightXY1,CrossXY2);			
			else
			{
				if(!bCross) CrossXY2 = RightXY2;
				CDPoint pArcVec = (RightXY1 - CrossXY2).Unit();				
				CDPoint CenPo = (RightXY1 + CrossXY2) / 2 + pArcVec.Rotate90() * pPI->m_dpR;
				Dom.Arc(RightXY1.x,RightXY1.y,CenPo.x,CenPo.y,CrossXY2.x,CrossXY2.y);
			}						
			
			Dom.LineTo(LeftXY1,RightXY1);
			Dom.LineTo(CrossXY1,CrossXY2);						
		}		
	}

	*pDom << Dom;	
}

void CAPlateDrawPyung::DimJSoleplatePyung(CDomyun *pDom)
{
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.SetDimExo(1);
	
	for(long nG=0;nG<pBridge->GetGirdersu();nG++)
	{
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		//double dTWeb = 12;//pGir->GetThickFactByStation(pBx->GetStation(), 2);
		double w = pGir->m_dGirderCenter+300;//pGir->GetLengthSharpOnBx(pBx, TRUE)+dTWeb/2;

		CString str;
		for(long nK = 0; nK <= pBridge->m_nQtyJigan; nK++)
		{
			for (int i=-1; i<2; i+=2)
			{
				long nCol = (i==-1) ? 0 : 1;
				double dL = pBridge->m_structJackupJewon[nK][nCol].m_dL;
				double dH = pBridge->m_structJackupJewon[nK][nCol].m_dH;			
				double dD = pBridge->m_structJackupJewon[nK][nCol].m_dD;
				double dW = pBridge->m_structJackupJewon[nK][nCol].m_dW;
				if(dL==0 && dH==0 && dD==0 && dW==0) continue;

				if(nK==0 && i==-1 || nK==pBridge->m_nQtyJigan && i==1) continue;			

				double  dSta = pGir->GetStationOnJijum(nK);
				CDPoint vAng = pGir->GetAngleOnJijum(nK);
				pOptStd->SetEnvType(&Dom, HCAD_DIML);
						
				Dom.SetDirection("TOP");
				if(nG==pBridge->GetGirdersu()-1)
				{
					str.Format(i==-1 ? "lL%d" : "rL%d",nK+1);
					Dom.DimBaseCurvePoint(dSta, w + Dom.Always(16));
					Dom.DimCurveMoveTo(dSta, w, nG);
					Dom.DimCurveLineTo(dL * i, w, str, "", nG);

					str.Format(i==-1 ? "lW%d" : "rW%d",nK+1);
					Dom.DimBaseCurvePoint(dSta + dL * i - dW/2 * i, w + Dom.Always(8));
					Dom.DimCurveMoveTo(dSta + dL * i - dW/2 * i, w, nG);
					Dom.DimCurveLineTo(dW * i, w, str, "", nG);

					str.Format(i==-1 ? "lH%d" : "rH%d",nK+1);
					Dom.SetDirection(i==-1 ? "LEFT" : "RIGHT");
					Dom.DimBaseCurvePoint(Dom.Always(8) * i, w + dD - dH/2);
					Dom.DimCurveMoveTo(dSta + dL * i + dL/2 * i, w + dD - dH/2, nG);
					Dom.DimCurveLineTo(dSta + dL * i + dL/2 * i, -dH, str, "", nG);					

					str.Format(i==-1 ? "lD%d" : "rD%d",nK+1);
					Dom.DimBaseCurvePoint(Dom.Always(8) * 2 * i, w + dD - dH/2);
					Dom.DimCurveMoveTo(dSta + dL * i + dL/2 * i, w + dD - dH/2, nG);
					Dom.DimCurveLineTo(dSta + dL * i + dL/2 * i, -dD, str, "", nG);					
				}
				else
				{									
					Dom.DimBaseCurvePoint(dSta, w + Dom.Always(16));
					Dom.DimCurveMoveTo(dSta, w, nG);
					Dom.DimCurveLineTo(dL * i, w, COMMA(dL), "", nG);
					
					Dom.DimBaseCurvePoint(dSta + dL * i - dW/2 * i, w + Dom.Always(8));
					Dom.DimCurveMoveTo(dSta + dL * i - dW/2 * i, w, nG);
					Dom.DimCurveLineTo(dW * i, w, COMMA(dW), "", nG);

					Dom.SetDirection(i==-1 ? "LEFT" : "RIGHT");
					Dom.DimBaseCurvePoint(Dom.Always(8) * i, w + dD - dH/2);
					Dom.DimCurveMoveTo(dSta + dL * i + dL/2 * i, w + dD - dH/2, nG);
					Dom.DimCurveLineTo(dSta + dL * i + dL/2 * i, -dH, COMMA(dH), "", nG);					

					Dom.DimBaseCurvePoint(Dom.Always(8) * 2 * i, w + dD - dH/2);
					Dom.DimCurveMoveTo(dSta + dL * i + dL/2 * i, w + dD - dH/2, nG);
					Dom.DimCurveLineTo(dSta + dL * i + dL/2 * i, -dD, COMMA(dD), "", nG);	
				}
			}
		}
	}
	*pDom << Dom;
}

void CAPlateDrawPyung::DrawPyungFieldSpliceMark(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pBridge	= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(pBx->GetNumberGirder());

	CDomyun Dom(pDom);

	DrawSplicePyung(&Dom, NULL, NULL, 0, TRUE, TRUE);//��������

	Dom.SetLineColor(2);
	CDPoint ptPoint = pGir->GetXyGirderDis(pBx->GetStation());
	Dom.Circle(ptPoint,1000);

	*pDom << Dom;

}

/////////////////////////////////////////////////////////////////////////
/// Ⱦ�극�̽� ����� ���⼱�ܸ��� �׸�
//  ���� ���������꿡�� �ۿ��ϴ� ǳ���߸��� ������.(���������꿡�� �ۿ��ϴ� ǳ������ ������ ���� �߰��ٶ�)
//  nBracing�� 1�˰Ŵ��� 2���Ŵ����̿� �ִ� ����극�̽��� ��ġ
/////////////////////////////////////////////////////////////////////////
double CAPlateDrawPyung::DrawHBracingCalcForXL(CDomyun *pDom, long nBracing, BOOL bLeftSlab)
{
	CPlateBridgeApp *pBridge  = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd  = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir     = pBridge->GetGirder(bLeftSlab?0:pBridge->GetGirdersu()-2);
	CPlateGirderApp *pGirNext = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex *pBxStt = pGir->GetBxOnJijum(0);
	CPlateBasicIndex *pBxEnd = pGir->GetBxOnJijum(pBridge->m_nQtyJigan);
	CAPlateDrawDimPyung DrawDimPyung(m_pDataMng);	

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	// Ⱦ�극�̽� ���� ///////////////////////////////////////////////////////////
	while(Finder.GetBxFixNext())
	{
		DrawHBracingCLOnBx(&Dom, pBx, Finder.GetBxFixNext());
		pBx = Finder.GetBxNext();
	}
	// �Ŵ� ����	   ///////////////////////////////////////////////////////////
	CPlateBxFinder FinderGir(pGir);
	CPlateBasicIndex *pBxCr = FinderGir.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	while(FinderGir.GetBxFixNext())
	{
		CDPoint xyStt = pGir->GetXyGirderDis(pBxCr->GetStation());
		CDPoint xyEnd = pGir->GetXyGirderDis(FinderGir.GetBxFixNext()->GetStation());

		Dom.LineTo(xyStt, xyEnd);

		pBxCr = FinderGir.GetBxNext();
	}

	CPlateBasicIndex *pBxSttMatch = pBxStt->GetBxMatchByCrossBeam(pGirNext);
	CPlateBasicIndex *pBxEndMatch = pBxEnd->GetBxMatchByCrossBeam(pGirNext);
	CPlateBxFinder FinderGirMatch(pGirNext);
	pBxCr = FinderGirMatch.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxSttMatch, pBxEndMatch);
	while(FinderGirMatch.GetBxFixNext())
	{
		CDPoint xyStt = pGirNext->GetXyGirderDis(pBxCr->GetStation());
		CDPoint xyEnd = pGirNext->GetXyGirderDis(FinderGirMatch.GetBxFixNext()->GetStation());

		Dom.LineTo(xyStt, xyEnd);

		pBxCr = FinderGirMatch.GetBxNext();
	}
	// ���κ� ����	///////////////////////////////////////////////////////////
	DrawCRBeamCLWithenBx(&Dom, pBxStt, pBxEnd);
	// ġ���� ������ ///////////////////////////////////////////////////////////
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 100*(dDiagonalL/20000);
	// ���� ���ϼ�, ġ���� ///////////////////////////////////////////////////////////
	double dForce = 2000;
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(dScale);
	pBxCr = FinderGir.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	while(FinderGir.GetBxFixNext())
	{
		CDPoint xyStt = pGir->GetXyGirderDis(pBxCr->GetStation());
		CDPoint xyEnd = pGir->GetXyGirderDis(FinderGir.GetBxFixNext()->GetStation());
		CDPoint xyDir = (xyEnd-xyStt).Unit();
		CDPoint xyForceDir = (xyEnd-xyStt).Unit().RotateInv90();
		double dSeg = ~(xyEnd-xyStt)/4;
		CDPoint xyFStt, xyFEnd, xyTemp;
		for(long n=0; n<4; n++)
		{
			xyFStt = xyStt+xyDir*dSeg*n-xyForceDir*dForce;
			xyFEnd = xyStt+xyDir*dSeg*n-xyForceDir*dForce*0.2;
			Dom.LineTo(xyFStt, xyFEnd);
			double dRadian = (xyEnd-xyStt).Unit().RotateInv90().GetAngleRadian();
			Dom.Arrow(xyFEnd, dRadian);
			xyTemp = xyFStt;

			xyFStt = xyStt+xyDir*dSeg*(n+1)-xyForceDir*dForce;
			xyFEnd = xyStt+xyDir*dSeg*(n+1)-xyForceDir*dForce*0.2;
			Dom.LineTo(xyFStt, xyFEnd);
			Dom.Arrow(xyFEnd, dRadian);
			Dom.LineTo(xyTemp, xyFStt);
		}
		
		pBxCr = FinderGir.GetBxNext();
	}
	
	CDoubleArray DataArray;
	DrawDimPyung.GetDimArrayCrossBeam(pBxSttMatch, pBxEndMatch, DataArray, DIM_BASE_DIAP);
	DrawDimPyung.DimBasePlan(&Dom, pBxSttMatch, pBxEndMatch, DataArray, "", FALSE, 0, FALSE, bLeftSlab ? 0 : 1);
	// ȸ��  ///////////////////////////////////////////////////////////
	CDPoint xyOrigin = pGirNext->GetXyGirderDis(pBxSttMatch->GetStation());
	Dom.Rotate(xyOrigin, pBridge->GetAngleBridgeCenter().GetMirrorVert());
	Dom.Move(-xyOrigin);
	*pDom << Dom;

	// ���⼱ �׸���  ///////////////////////////////////////////////////////////
	CDPoint xyBase(0, -8000);
	// �ϴ� ������ x���� �迭�� ����
	CPlateBxFinder FinderInfK(pBridge->GetGirder(1));
	pBxCr = FinderInfK.GetBxFirst(BX_CROSSBEAM_VBRACING);
	double dSttCr = pBxSttMatch->GetStation();
	CDDWordArray dArrL;
	long nIndex = 0;
	while(pBxCr)
	{
		if(nIndex==0)
			dArrL.Add(0);
		else
			dArrL.Add(pBxCr->GetStation()-dSttCr);
		
		nIndex++;
		pBxCr = FinderInfK.GetBxNext();
	}
	// �⺻��
	Dom.LineTo(xyBase, CDPoint(dArrL[dArrL.GetSize()-1], xyBase.y));
	// ���ؿ��⼱
	double dHinfK   = 3000;
	double dHatchW  = 500; 
	CDPoint vX(1,0), vY(0,1);
	CDPoint xySttUp = xyBase;
	CDPoint xyEndUp = CDPoint(dArrL[dArrL.GetSize()-1], xyBase.y+dHinfK);
	CDPoint xySttDn = CDPoint(dArrL[0], xyBase.y-dHinfK);
	CDPoint xyEndDn = CDPoint(dArrL[dArrL.GetSize()-1], xyBase.y);

	Dom.LineTo(xySttUp, xyEndUp);
	Dom.LineTo(xySttDn, xyEndDn);
	Dom.LineTo(xySttUp, xySttDn);
	Dom.LineTo(xyEndUp, xyEndDn);

	CDPoint xyMatchUp, xyMatchDn, xyMatchCen;
	CDPoint xyBr1 = CDPoint(dArrL[nBracing], xyBase.y);
	CDPoint xyBr2 = CDPoint(dArrL[nBracing+1], xyBase.y);
	GetXyMatchLineAndLine(xySttUp, (xyEndUp-xySttUp).Unit(), xyBr1, CDPoint(0, 1), xyMatchUp);
	GetXyMatchLineAndLine(xySttDn, (xyEndDn-xySttDn).Unit(), xyBr2, CDPoint(0, -1), xyMatchDn);
	GetXyMatchLineAndLine(xyMatchUp, (xyMatchDn-xyMatchUp).Unit(), xySttUp, CDPoint(1, 0), xyMatchCen);

	Dom.LineTo(xySttUp, xyMatchUp);
	Dom.LineTo(xyMatchUp, xyMatchDn);
	Dom.LineTo(xyMatchDn, xyEndDn);

	CDPointArray vArrUp, vArrDn;

	vArrUp.Add(xySttUp);
	vArrUp.Add(xyMatchUp);
	vArrUp.Add(xyMatchCen);
	vArrUp.Add(xySttUp);
	
	vArrDn.Add(xyMatchCen);
	vArrDn.Add(xyMatchDn);
	vArrDn.Add(xyEndDn);
	vArrDn.Add(xyMatchCen);

	Dom.HatchingByCDPointArray(vArrUp, 45, dHatchW, FALSE);
	Dom.HatchingByCDPointArray(vArrDn, 45, dHatchW, FALSE);

	Dom.TextOut(xySttUp-vY*dHinfK/2-vX*dHinfK/3, "+1");
	Dom.TextOut(xyEndDn+vY*dHinfK/2+vX*dHinfK/3, "-1");
	double dValueUp = fabs(xyMatchUp.y-xySttUp.y)/dHinfK;
	double dValueDn = fabs(xyMatchDn.y-xyEndDn.y)/dHinfK;
	CString cs;
	cs.Format("%.3lf", dValueUp);
	Dom.TextOut(xyMatchUp+vY*Dom.Always(2), cs);
	cs.Format("%.3lf", dValueDn);
	Dom.TextOut(xyMatchDn-vY*Dom.Always(4), cs);

	Dom.SetTextHeight(3);
	double dArea = vArrUp.GetArea()+vArrDn.GetArea();
	cs.Format("�ܸ���(Area) = %.3lf m��", toM2(dArea)/3);
	Dom.TextOut(xySttDn-vY*Dom.Always(10)+vX*Dom.GetTextWidth(cs)/2, cs);
	
	*pDom << Dom;

	return toM2(dArea)/3;		// ���⼱�� ����
}

// ���÷���Ʈ,����������
// nUpDown
void CAPlateDrawPyung::DrawSolePlate(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									int nUpDown, long nLine, BOOL bFirstSkip, BOOL bLastSkip)
{
	if(pBxStt==NULL || pBxEnd==NULL) return;

	CPlateGirderApp	*pGir = pBxStt->GetGirder();

	__int64 BxFlag = BX_CROSSBEAM_VBRACING;

	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	if(pBx==NULL) return;

	if(bLastSkip) 
	{
		CPlateBasicIndex *pBxTmp = pGir->GetBxByStationDir(pBxEnd->GetStation(),-1,BxFlag);
		pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxTmp);
	}
	if(bFirstSkip) 
		pBx = Finder.GetBxNext();

	CDomyun* pDom = new CDomyun(pDomP);
	CString szOldLayer = pDom->GetLayerEx();
	while(pBx) 
	{
		if(pBx->IsJijum())
		{
			if(pBx==pBxStt)
				DrawShoeBuje(pDom,pBx,nUpDown,FALSE,TRUE);
			else if(pBx==pBxEnd)
				DrawShoeBuje(pDom,pBx,nUpDown,TRUE,FALSE);
			else
				DrawShoeBuje(pDom,pBx,nUpDown,TRUE,TRUE);
		}
		pBx = Finder.GetBxNext();
	}

	*pDomP << *pDom;
	delete pDom;
}


// ���÷���Ʈ�� ��� �������� ǥ��
void CAPlateDrawPyung::DrawShoeBuje(CDomyun *pDomP, CPlateBasicIndex *pBx, long nUpDown, BOOL bSttHalf, BOOL bEndHalf)
{
	if(pBx==NULL)	return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CAPlateDrawDanmyun DrawDan(m_pDataMng);

	CDomyun* pDom = new CDomyun(pDomP); // ���÷���Ʈ��........
	pOptStd->SetEnvLType(pDom, LT_DOT_LINE);

	DrawDan.DrawPlanSolePlateBx(pDom, pBx, FALSE, TRUE, TRUE);

	*pDomP << *pDom;
	delete pDom;
}


void CAPlateDrawPyung::DrawSlabTaperLineStt(CDomyun *pDomP, long nPier)
{		
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	long nTotalPier = pDB->m_nQtyJigan;
	
	if(nPier != 0 && nPier != nTotalPier) return;

	CDomyun Dom(pDomP);
	CDPoint A[5];

	if(nPier == 0)
		pDB->GetXySss(OFF_STT_HUNCHSTT, A);
	else
		pDB->GetXySss(OFF_END_HUNCHSTT, A);

	A[4] = A[3];//A[4]�� �ʱ�ȭ ���� ���� ���� ������. hgh 20030520

	// ������ ���� �߽ɼ��� ������ ��.
	int GirderSu = pDB->GetGirdersu();
	int nSttGirderNum, nEndGirderNum, nCenGirderNum;
	nSttGirderNum = nCenGirderNum = GirderSu/2;
	nEndGirderNum = nSttGirderNum - 1;

	CPlateGirderApp *pGirL = pDB->GetGirder(nSttGirderNum);
	CPlateGirderApp *pGirR = pDB->GetGirder(nEndGirderNum);
	CPlateGirderApp *pGirC = pDB->GetGirder(nCenGirderNum);

	CDPoint poInterSection = A[4];
	CDPoint poStt, poEnd;
	SLABGIRDERSHOE nSttType, nEndType;
	BOOL bInterSection;

	nSttType = (nPier == 0) ? OFF_STT_HUNCHSTT : OFF_END_HUNCHSTT;
	nEndType = (nPier == 0) ? OFF_STT_HUNCHEND : OFF_END_HUNCHEND;
	if(GirderSu%2 == 0)
	{
		CDPoint poSttLeft = pGirL->GetXyGirderDis(pGirL->GetSSS(nSttType));
		CDPoint poEndLeft = pGirL->GetXyGirderDis(pGirL->GetSSS(nEndType));
		CDPoint poSttRigh = pGirR->GetXyGirderDis(pGirR->GetSSS(nSttType));
		CDPoint poEndRigh = pGirR->GetXyGirderDis(pGirR->GetSSS(nEndType));

		poStt = (poSttLeft+poSttRigh)/2;
		poEnd = (poEndLeft+poEndRigh)/2;
	}
	else
	{
		poStt = pGirC->GetXyGirderDis(pGirC->GetSSS(nSttType));
		poEnd = pGirC->GetXyGirderDis(pGirC->GetSSS(nEndType));
	}
	int inter = 0;
	for(inter=0; inter<3; inter++)
	{
		// ���� ���÷�...������ �ణ ����...�Ǽ�...
		poStt = poStt+0.1*(poStt-poEnd).Unit();
		poEnd = poEnd+0.1*(poEnd-poStt).Unit();
		bInterSection = GetXyMatchSegAndSeg(A[inter], A[inter+1], poStt, poEnd, poInterSection);
		if(bInterSection)
			break;
	}
	int  i =0;
	for(i=0; i < 4; i++)
	{
		if(i == inter)
		{
			for(int j=4; j > i+1; j--)
			{
				A[j] = A[j-1];
			}
			A[i+1] = poInterSection; 
		}
	}

	//////////////////////////////////////////////////////////////
	// ������ ������
	pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);
	
	for(int nPo=0; nPo<4; nPo++)
	{
		if( nPo == inter+1 )
			pOptStd->SetEnvLType(&Dom, LT_SOLID);
		Dom.LineTo(A[nPo], A[nPo+1]);
	}
	
	*pDomP << Dom;	
}

void CAPlateDrawPyung::DrawSlabTaperLineEnd(CDomyun *pDomP, long nG, long nPier, int nUpDown, CPlateBasicIndex* pBxLeftBrk, CPlateBasicIndex* pBxRighBrk)
{		
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	
	CDPoint vAng;
	CDPoint ep[2];

	long nTotalPier = pDB->m_nQtyJigan;
	double Dist = 0, dSta = 0, dStaReal = 0;
	double LSharp      = pDB->m_dWidthSlabLeft;
	double RSharp      = pDB->m_dWidthSlabRight;		
	
	if(nPier != 0 && nPier != nTotalPier) return;

	CDomyun Dom(pDomP);	
	
	dSta = (nPier == 0) ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);	
	vAng = (nPier == 0) ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);		

	if(nG==0 && !pBxLeftBrk)
		Dist = -LSharp;
	else if(pGir->m_pLeft)		
		Dist = -fabs(pGir->m_dGirderCenter-pGir->m_pLeft->m_dGirderCenter)/2;
	
	dStaReal = pGir->GetStationBySc(dSta,Dist,vAng);		
	ep[0] = pGir->GetXyGirderDis(dStaReal,Dist);

	if(nG==pDB->GetGirdersu()-1 && !pBxRighBrk)
		Dist = RSharp;
	else if(pGir->m_pRight)
		Dist = fabs(pGir->m_dGirderCenter-pGir->m_pRight->m_dGirderCenter)/2;

	dStaReal = pGir->GetStationBySc(dSta,Dist,vAng);		
	ep[1] = pGir->GetXyGirderDis(dStaReal,Dist);	
			
	if(nUpDown==1)	
		pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);

	if(nUpDown==-1)
	{
		pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);
		Dom.LineTo(ep[0],(ep[0]+ep[1])/2);
		pOptStd->SetEnvLType(&Dom,LT_SOLID);
		Dom.LineTo((ep[0]+ep[1])/2,ep[1]);	
	}
	else
		Dom.LineTo(ep[0],ep[1]);

/*	if(pBxLeftBrk)
	{
		CBracket *pBrk = pBxLeftBrk->GetBracketLeft();
		Dist = -pGir->m_dUWidth/2 - LSharp;
		dStaReal = pGir->GetStationBySc(dSta, Dist, vAng);
		ep[0] = pGir->GetXyGirderDis(dStaReal, Dist);

		Dist = -pGir->m_dUWidth/2 - pBrk->m_L1_EB - pBrk->m_L2_EB + dHunchWidth;
		dStaReal = pGir->GetStationBySc(dSta, Dist, vAng);
		ep[1] = pGir->GetXyGirderDis(dStaReal, Dist);

		pStd->m_pOptionStd->SetEnvLType(&Dom,LT_DOT_LINE);
		Dom.LineTo(ep[0],ep[1]);
	}

	if(pBxRighBrk)
	{
		CBracket *pBrk = pBxRighBrk->GetBracketRight();
		if(pBrk)
		{
			Dist = pGir->m_dUWidth/2 + RSharp;
			dStaReal = pGir->GetStationBySc(dSta, Dist, vAng);
			ep[0] = pGir->GetXyGirderDis(dStaReal, Dist);

			Dist = pGir->m_dUWidth/2 + pBrk->m_L1_EB + pBrk->m_L2_EB - dHunchWidth;
			dStaReal = pGir->GetStationBySc(dSta, Dist, vAng);
			ep[1] = pGir->GetXyGirderDis(dStaReal, Dist);

			pStd->m_pOptionStd->SetEnvLType(&Dom,LT_SOLID);
			Dom.LineTo(ep[0],ep[1]);
		}
	}*/

	*pDomP << Dom;	
}


void CAPlateDrawPyung::DrawJointLine(CDomyun *pDomP, BOOL bSttJoint)
{
	CPlateBridgeApp *pDB     = m_pDataMng->GetBridge();
	
	CDomyun Dom(pDomP);
	
	if(bSttJoint)
	{
		CDPoint A[4];
		pDB->GetXySss(OFF_STT_STT, A);
		Dom.MoveTo(A[0]);
		for(long n = 1; n < 4; n++)
			Dom.LineTo(A[n]);
	}
	else
	{
		CDPoint A[4];
		pDB->GetXySss(OFF_END_END, A);
		Dom.MoveTo(A[0]);
		for(long n = 1; n < 4; n++)
			Dom.LineTo(A[n]);
	}

	*pDomP << Dom;
}

// tendon�� Bx������ ����ġ���� �׸�.
void CAPlateDrawPyung::DrawTendonArr(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	
	double dStaStt = pBxStt->GetStation();
	double dStaEnd = pBxEnd->GetStation();

	CDomyun Dom(pDom);
	
	double dSta = 0;
	long   nFix = 0;

	for(long n=0; n<pBridge->m_arrTendon.GetSize(); n++) 
	{
		dSta = pBridge->m_arrTendon.GetAt(n).x;
		nFix = pBridge->m_arrTendonDom.GetAt(n);

		if(dStaStt < dSta && dSta < dStaEnd)
			DrawTendonBySta(&Dom, dSta, nFix);	// nFix=0 : ���������� nFix=1 : ����������
	}

	*pDom << Dom;
}


void CAPlateDrawPyung::DrawTendonBySta(CDomyun *pDom, double dSta, long nFix)
{
	CPlateBridgeApp *pBridge   = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd   = m_pDataMng->GetOptionStd();
	CSlabApp        *pSlabLeft = pBridge->GetSlabLeft();
	CSlabApp        *pSlabRight= pBridge->GetSlabRight();
	
	double dSpace  = 300;	// ������ ���� ������ ���� ���������� �Ÿ�
	double dRadCir = 100;	// ������ ��ũ�� ������ 
	double dArrowHead = 400;	// ȭ��ǥ �Ӹ��� ����
	CDPoint xyLeftStt = pSlabLeft->GetXyGirderDis(dSta, dSpace);
	CDPoint xyRightStt= pSlabRight->GetXyGirderDis(dSta, -dSpace);
	CDPoint vGirL     = pSlabLeft->GetLine()->GetAngleAzimuth(dSta);
	CDPoint vGirR     = pSlabRight->GetLine()->GetAngleAzimuth(dSta);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CDPoint xy1, xy2, xy3, xy4;

	if(nFix==0)			// ���� ������
	{
		xy1 = xyRightStt;
		xy2 = xyRightStt+vGirR.Rotate90()*dArrowHead+vGirR*dArrowHead/4;
		xy3 = xyRightStt+vGirR.Rotate90()*dArrowHead;
		xy4 = xyRightStt+vGirR.Rotate90()*dArrowHead-vGirR*dArrowHead/4;

		Dom.Solid(xy1.x, xy1.y, xy2.x, xy2.y, xy3.x, xy3.y, xy4.x, xy4.y);	// ����� 
		Dom.Circle(xyLeftStt, dRadCir, TRUE);		// ������
		Dom.LineTo(xyLeftStt, xyRightStt);
	}
	else if(nFix==1)	// ���� ������
	{
		xy1 = xyLeftStt;
		xy2 = xyLeftStt-vGirL.Rotate90()*dArrowHead+vGirL*dArrowHead/4;
		xy3 = xyLeftStt-vGirL.Rotate90()*dArrowHead;
		xy4 = xyLeftStt-vGirL.Rotate90()*dArrowHead-vGirL*dArrowHead/4;

		Dom.Solid(xy1.x, xy1.y, xy2.x, xy2.y, xy3.x, xy3.y, xy4.x, xy4.y);	// ����� 
		Dom.Circle(xyRightStt, dRadCir, TRUE);		// ������
		Dom.LineTo(xyLeftStt, xyRightStt);
	}

	*pDom << Dom;
}

void CAPlateDrawPyung::DrawSectionSymbol(CDomyun *pDom)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CPlateGirderApp *pGirEnd = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	CPlateBxFinder	Finder(pGirEnd);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CString szTemp;
	while(pBx)
	{
		CDPoint  xy  = pGirEnd->GetXyGirderDis(pBx->GetStation(), pBridge->m_dWidthSlabRight/2+Dom.GetTextHeight(), pBx->GetAngle());
		CDPoint vGir = pGirEnd->GetLine()->GetAngleAzimuth(pBx->GetStation());

		if(pBx->GetMarkString().GetLength() > 0)
		{
			Dom.TextCircle(xy.x, xy.y, pBx->GetMarkString());
			Dom.Rotate(xy, vGir);
		}

		pBx = Finder.GetBxNext();
	
		*pDom << Dom;
	}
}

// ���Լ��� ���ʸ� ����
void CAPlateDrawPyung::DrawCRBeamFillerBothOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp	 *pGir  = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();

	if(!bLeft) 
	{
		CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);
		pC = pBxLeft->GetSection()->GetCrossBeam();
	}

	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint	vAng	= pC->GetVectorPlan();
	CDPoint	vX		= vAng;
	CDPoint	vY		= -vX.RotateInv90();
	if(vAng.y==0) return;
	double Zorib = 0; // ��������ġ
	double C1 = pC->m_sC1 + Zorib;
	double Wid   = pC->m_T3_CR/2; // ���κ� ���÷���Ʈ T
	double dsABCC = (pC->m_sA+pC->m_sB*pC->m_sC+C1/2); // ���κ� ���� ���������� �ʺ�

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLH);

	pC->m_dThickFiller = (long)pC->GetFillerThick();

	if(pC->m_dThickFiller < 2)		return;// �ʷ��� �β��� 2mm�̸��ΰ�� 
	if(pC->m_dThickFiller >= 9)		//9mm ���� ũ�� ������ �߰��ؼ� ��Ʈ ü��...
		dsABCC += (pC->m_sA*2);

	long nDir = bLeft ? 1 : -1;

	double dThickWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	CDPoint xyCenStt	= pGir->GetXyGirderDis(pBx->GetStation(), nDir*dThickWeb/2, vAng.RotateInv(vGir));

	double dWidth = pC->m_P_CR/fabs(vAng.RotateInv(vGir).y);
	// ȭ����� ����
	Dom.MoveTo(xyCenStt + nDir*vX*dWidth				+ vY*Wid );					//1
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth	+dsABCC)	+ vY*Wid);
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth	+dsABCC)	+ vY*pC->m_vT_G/2);
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth)				+ vY*pC->m_vT_G/2);			//4
	Dom.LineTo(xyCenStt + nDir*vX*dWidth				+ vY*Wid);					//5
	// ȭ����� �Ʒ���
	Dom.MoveTo(xyCenStt + nDir*vX*dWidth				- vY*Wid );					//1
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth	+dsABCC)	- vY*Wid);
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth	+dsABCC)	- vY*pC->m_vT_G/2);
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth)				- vY*pC->m_vT_G/2);			//4
	Dom.LineTo(xyCenStt + nDir*vX*dWidth				- vY*Wid);					//5

	*pDom << Dom;
}


// ���Լ��� ���ʸ� ����
void CAPlateDrawPyung::DrawCRBeamFillerOneOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp	 *pGir  = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();

	if(!bLeft) 
	{
		CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);
		pC = pBxLeft->GetSection()->GetCrossBeam();
	}

	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint	vAng	= pC->GetVectorPlan();
	CDPoint	vX		= vAng;
	CDPoint	vY		= -vX.RotateInv90();
	if(vAng.y==0) return;
	double Zorib = 0; // ��������ġ
	double C1		= pC->m_sC1 + Zorib;
	double Wid		= pC->m_T3_CR/2; // ���κ� ���÷���Ʈ T
	double dsABCC	= (pC->m_sA+pC->m_sB*pC->m_sC+C1/2); // ���κ� ���� ���������� �ʺ�

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLH);

	pC->m_dThickFiller = (long)pC->GetFillerThick();

	if(pC->m_dThickFiller < 2)		return;// �ʷ��� �β��� 2mm�̸��ΰ�� 
	if(pC->m_dThickFiller >= 9)//9mm ���� ũ�� ������ �߰��ؼ� ��Ʈ ü��...
		dsABCC += (pC->m_sA*2);

	long nDir = bLeft ? 1 : -1;
	double dThickWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	CDPoint xyCenStt	= pGir->GetXyGirderDis(pBx->GetStation(), nDir*dThickWeb/2, vAng.RotateInv(vGir));
	
	double dWidth = pC->m_P_CR/fabs(vAng.RotateInv(vGir).y);
	Dom.MoveTo(xyCenStt + nDir*vX*dWidth				- vY*pC->m_vT_G/2);							//1
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth	+dsABCC)	- vY*pC->m_vT_G/2);
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth	+dsABCC)	- vY*(pC->m_vT_G/2- pC->m_dThickFiller));
	Dom.LineTo(xyCenStt + nDir*vX*(dWidth)				- vY*(pC->m_vT_G/2- pC->m_dThickFiller));	//4
	Dom.LineTo(xyCenStt + nDir*vX*dWidth				- vY*Wid);									//5
	
	*pDom << Dom;
}



// ����������
void CAPlateDrawPyung::DrawExpJointLine(CDomyun *pDomP, BOOL bSttJoint, long nSide)
{
	CPlateBridgeApp *pDB	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDomP);
	pOptStd->SetEnvType(&Dom, HCAD_CONC);

	CDPoint A[4];
	pDB->GetXySss(bSttJoint ? OFF_STT_EXP : OFF_END_EXP, A);
	CDPoint xyCen = (A[1]+A[2])/2;

	if(nSide==0)		// ����	
	{
		Dom.MoveTo(A[0]);
		for(long n = 1; n < 4; n++)
			Dom.LineTo(A[n]);
	}
	else if(nSide==-1)	// ����
	{
		Dom.MoveTo(A[0]);
		Dom.LineTo(A[1]);
		Dom.LineTo(xyCen);
	}
	else if(nSide==1)	// ����
	{
		Dom.MoveTo(A[3]);
		Dom.LineTo(A[2]);
		Dom.LineTo(xyCen);
	}

	*pDomP << Dom;
}



void CAPlateDrawPyung::DrawUpDownLine(CDomyun *pDomP, double dStaStt, double dStaEnd)
{
	CPlateBridgeApp *pDB	= m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CLineInfo	    *pLine = pDB->GetGirder(-1)->GetLine();
	CADeckData		*pDeckData = m_pDataMng->GetDeckData();

	if(pDB->IsExtLine(-1))  return;

	CDomyun Dom(pDomP);
	pOptStd->SetEnvType(&Dom,HCAD_CENT);

	long   nGirderSu  = pDB->GetGirdersu();
	double distToLine = pDeckData->GetDistByJongBase(JONGBASEPOS_SLABCENTER);
	double wing		  = 1500;
	double sttStation = pLine->GetStationByScInLine(dStaStt, distToLine, pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB)) - wing;
	double endStation = pLine->GetStationByScInLine(dStaEnd, distToLine, pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB)) + wing*3;

	pLine->DrawLS(&Dom,sttStation,endStation+wing/3,distToLine);
	*pDomP << Dom;
	//
	pOptStd->SetEnvType(&Dom,HCADEX_GISI);
	CString strUp  = " �� ��";
	CString strLow = "�� �� ";
	double  Len	   = Dom.GetTextWidth(strLow,1);
	double  Height = Dom.GetTextHeight();

	CDPoint cp = pLine->GetXyLineDis(sttStation+Height*2,distToLine);
	Dom.SetTextAngle(pLine->GetAngleAzimuth(sttStation).GetAngleDegree()+90);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(cp,strUp);
	Dom.SetTextAlignHorz(TA_RIGHT);
	Dom.TextOut(cp,strLow);
	*pDomP << Dom;
	//
	cp = pLine->GetXyLineDis(sttStation+Height*2.1,distToLine);
	Dom.Circle(cp.x,cp.y,Dom.Always(0.5),TRUE);
	Dom.GiSiArrow(cp.x,cp.y,Dom.Always(0),Len,0,"");
	Dom.Rotate(pLine->GetAngleAzimuth(sttStation).Rotate90().GetAngleRadian(),cp.x,cp.y);
	*pDomP << Dom;
	//
	Dom.GiSiArrow(cp.x,cp.y,Dom.Always(0),Len,0,"");
	Dom.Rotate(pLine->GetAngleAzimuth(sttStation).RotateInv90().GetAngleRadian(),cp.x,cp.y);
	*pDomP << Dom;
}




double CAPlateDrawPyung::GetRotateAnglePyung(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp* pGir = pBxStt->GetGirder();

	CDPoint xy1 = pGir->GetXyGirderDis(pBxStt->GetStation());
	CDPoint xy2 = pGir->GetXyGirderDis(pBxEnd->GetStation());

	CDPoint vAng = (xy2-xy1).Unit();

	return -vAng.GetAngleRadian();
}



// ���κ� �׸���
// m_DArrBx = ��ġ�ΰ� �ִ� ���κ��� Bx
//  ...BX_CROSSBEAM_VBRACING ��ġ �߿��� �ܸ��� SECDET_CROSSBEAM Ÿ���̰�, ���κ����� ������ �ϸ� ������ 5cm������ ���
//  ...BX_CROSSBEAM_VBRACING ��ġ �߿��� �ܸ��� SECDET_VBRACING Ÿ���̰�, �������� ���..
void CAPlateDrawPyung::DrawPlanCrossBeam(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(pBxStt==NULL || pBxEnd==NULL)	return;

	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = pBxStt->GetGirder();
	
	long nG       = pBxStt->GetNumberGirder();
	long GirderSu = pDB->GetGirdersu();

	CDomyun Dom(pDomP);

	// ���ϸ� SETTING
	long nUpDown = 1;
	if(GirderSu%2==0) { // ¦������ �Ŵ�
		if(nG> (long)GirderSu/2-1) nUpDown =  0;
		if(nG==(long)GirderSu/2-1) nUpDown = -1;
		if(nG< (long)GirderSu/2-1) nUpDown =  1;
	}
	else {
		if(nG>=(long)GirderSu/2) nUpDown =  0;
		if(nG< (long)GirderSu/2) nUpDown =  1;
	}

	m_DArrBx.RemoveAll();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	while(pBx)
	{
//		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
//		{
//			CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
//			CDPoint A[10];
//			pGir->GetHunchXyDom(pBx, A);
//			CDPoint B[10];
//			pC->GetCrossXyDom(B);
//
//			if(B[0].y+pC->m_T1_CR+50/*������*/ > A[2].y || pBx->IsAbut())
//				m_DArrBx.Add(pBx);
//		}
//		// ������ �ϰ��� �����극�̽��� ��쵵 ����.
//		else if(pBx->GetSection()->IsenDetType(SECDET_VBRACING) && pBx->IsJijum())	
//			m_DArrBx.Add(pBx);

		long nJijumNum = pBx->GetNumberJijum();

		if(pGir->IsCrossBeamBxHasHunch(pBx))
			m_DArrBx.Add(pBx);
		else if(pBx->IsDanbuJijum())
		{
			// #0030791 - �������δ� ��ġ�� ����!�������ο� ����� ���κ���ġ�� ã�ƾ� �� - CSlabJewonDlg(Ⱦ�ܽ����� ����) ����
			CPlateBasicIndex *pBxCross = NULL;
			if(nJijumNum == 0)
			{
				pBxCross = Finder.GetBxFixNext();
				if(pBxCross)
					m_DArrBx.Add(pBxCross);
			}
			else
			{
				pBxCross = Finder.GetBxFixPrev();
				if(pBxCross)
					m_DArrBx.Add(pBxCross);
			}
	
		}
		else if(pBx->GetSection()->IsenDetType(SECDET_VBRACING) && pBx->IsJijum())	
			m_DArrBx.Add(pBx);
		
		pBx = Finder.GetBxNext();
	}

	for(long n=0; n<m_DArrBx.GetSize()-1; n++)
		DrawPlanMainPlateBxOnSlab(&Dom, m_DArrBx.GetAt(n), m_DArrBx.GetAt(n+1), nUpDown);

	*pDomP << Dom;
}


// ���κ� ������ �׸���
// ���� ��Ʈ���� �ϳ��ΰ�쿡 ���ؼ��� �����Ѵ�.
// �׷��� GetHunchXyDom()�� �������� ������ ��Ʈ���� �ΰ� �̻��� �������� ���Ѵ�...
// ���Ŀ� GetHunchXyDom()�� ������ �ذ�Ǹ� ���� ��ȯ ���� GetHunchXyDom()�κи� ó���� �ָ� �ȴ�...
// 2002. 04. 03 CMS
void CAPlateDrawPyung::DrawPlanMainPlateBxOnSlab(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, long nUpDown)
{
	if(pBx==NULL || pBxNext == NULL) return;	

	CPlateBridgeApp *pDB = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateGirderApp	*pGirR = pGir->m_pRight;
	CLineInfo		*pLine = pGir->GetLine();
	CLineInfo		*pLineR = NULL;

	if(pGirR)
		pLineR = pGirR->GetLine();

	long nLineType = 0;
	switch(nUpDown)
	{
	case -1:
	case 1:
		nLineType = LT_DOT_LINE;
		break;

	case 0:
		nLineType = LT_SOLID;
		break;
	}

	// ������ ��ġ�κ��� ��� �Ǽ����� ǥ����(�輺��)
//	nLineType = LT_SOLID;

	CDomyun* pDom = new CDomyun(pDomP);

	CDPointArray ptArr[2];
	CDPointArray ptArrNext[2];
	int nFirstDraw = -1, nNextDraw = -1;

	/////////////////////////////////////////////////////////////////////////
	nFirstDraw = GetPlanMainPlateHunch(pBx, pBxNext, ptArr, TRUE);
	if(pBxNext) 
		nNextDraw = GetPlanMainPlateHunch(pBxNext, pBx, ptArrNext, FALSE);
	//////////////////////////////////////////////////////////////////////////
		
	if(nFirstDraw == -1 && nNextDraw == -1) return;

	ModifyHunchCross(ptArr, ptArrNext);		

	m_pDataMng->GetOptionStd()->SetEnvLType(pDom,nLineType);

	double dStt = 0, dEnd = 0, dHunchWidth = pDB->m_dWidthHunch;
	double dDLeftUpper, dDLeftLower, dDRightUpper, dDRightLower;
	// ��ġ�κ� ����-------
	CDPoint A[41], A_Next[41];
	double dX[4], dX_Next[4];
	pGir->GetHunchXyDom(pBx, A);
	pGir->GetHunchXyDom(pBxNext, A_Next);

	dX[0] = fabs(A[4].x - A[3].x);
	dX[1] = 0;//fabs(A[12].x - A[11].x);
	dX[2] = 0;//fabs(A[15].x - A[14].x);
	dX[3] = fabs(A[6].x - A[5].x);

	dX_Next[0] = fabs(A_Next[4].x - A_Next[3].x);
	dX_Next[1] = 0;//fabs(A[12].x - A[11].x);
	dX_Next[2] = 0;//fabs(A[15].x - A[14].x);
	dX_Next[3] = fabs(A_Next[6].x - A_Next[5].x);

	////////////////////////////////////////////////////
	// �׸��� ����� ������ ����.......

	CDPoint Lp1, Lh1, Rh1, Rp1, Lp2, Lh2, Rh2, Rp2;
	long nSize = ptArr[1].GetSize() / 4;
	for(long i = 0; i < nSize; i++)
	{
		Lp1 = ptArr[1][i*4+0];
		Lh1 = ptArr[1][i*4+1];
		Rh1 = ptArr[1][i*4+2];
		Rp1 = ptArr[1][i*4+3];
		CDPointArray arrTemp1;
		arrTemp1.RemoveAll();
		arrTemp1.Add(Lp1);
		arrTemp1.Add(Rp1);
		arrTemp1.Add(Rh1);
		arrTemp1.Add(Lh1);
		double dArea1	= arrTemp1.GetArea();
		
		Lp2 = ptArrNext[0][i*4+0];
		Lh2 = ptArrNext[0][i*4+1];
		Rh2 = ptArrNext[0][i*4+2];
		Rp2 = ptArrNext[0][i*4+3];
		CDPointArray arrTemp2;
		arrTemp2.RemoveAll();
		arrTemp2.Add(Lh2);
		arrTemp2.Add(Rh2);
		arrTemp2.Add(Rp2);
		arrTemp2.Add(Lp2);
		double dArea2	= arrTemp2.GetArea();

		// ���κ� �κ����� �Ǻ�.....������ ������ ��찡 �ֿ� ���� dD�Ÿ��� �޶�����
		// (1) ���κ�, ���κ�
		// (2) ���κ�, ���κ� 
        // (3) ���κ�, ���κ�

		// ������ ���� Ȯ��.....
		BOOL bEndStringerPart = (i==1) ? TRUE : FALSE;		// ��Ʈ���� �κ� ��...
		// ��Ʈ���� �κ� �Ǻ�
		BOOL bIsStringerPart = nSize >=2 ? TRUE : FALSE;
/*		if(nSize >= 2)	// ���κ��� ���ԵǾ� �ִ�....
		{
			double dWidStringer = pBx->GetStringer()->m_uM;

			double dDisStringer = pGir->GetDisStringer(pBx, 0);	// �ϴ��� ������Ų��...�ʳ� ���κ��� �ΰ��̻��̸�..
			if(i == 0)				// ������~���κ�
			{
				dDLeftUpper = pGir->m_dGirderCenter + pGir->m_dUWidth / 2 + pGir->m_dURightSharp + dHunchWidth;
				dDLeftLower = dDLeftUpper + dX[0];
				dDRightLower = pGir->m_dGirderCenter + dDisStringer - dWidStringer - dHunchWidth;
				dDRightUpper = dDRightLower - dX[1];
			}
			else if(i == nSize-1)	// ���κ�~������
			{
				dDLeftUpper = pGir->m_dGirderCenter + dDisStringer + dWidStringer + dHunchWidth;
				dDLeftLower = dDLeftUpper + dX[2];
				dDRightLower = pGirR->m_dGirderCenter - pGirR->m_dUWidth / 2 - pGirR->m_dULeftSharp - dHunchWidth;
				dDRightUpper = dDRightLower - dX[3];
			}
			else					// ���κ�~���κ�......���� ����....
			{
				dDLeftUpper = 0;
				dDLeftLower = 0;
				dDRightUpper = 0;
				dDRightLower = 0;
			}
		}
		else*/	// ������~������
		{
			dDLeftUpper  = pGir->m_dGirderCenter + pGir->GetMaxWidthFlange(TRUE)/2 + dHunchWidth;
			if(dArea1>=dArea2)
				dDLeftLower  = dDLeftUpper + dX[0];
			else
				dDLeftLower  = dDLeftUpper + dX_Next[0];
			dDRightLower = pGirR->m_dGirderCenter - pGirR->GetMaxWidthFlange(TRUE)/2 - dHunchWidth;
			if(dArea1>=dArea2)
				dDRightUpper = dDRightLower - dX[3];
			else
				dDRightUpper = dDRightLower - dX_Next[3];
		}

		if(nUpDown == -1 && bIsStringerPart && bEndStringerPart)
			m_pDataMng->GetOptionStd()->SetEnvLType(pDom,LT_SOLID);

		CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		CDPoint poCenCrossLeft, poCenCrossRigh, poCenLeft, poCenRigh;
		if(nFirstDraw)
		{
//			if(nFirstDraw != -1)
			poCenCrossLeft = (Lp1+Rp1)/2;
			pDom->LineTo(Lp1,poCenCrossLeft);
			pDom->LineTo(Lp1,Lh1);
			GetXyMatchSegAndLine(Lh1, Rh1, poCenCrossLeft, vGir, poCenLeft);
			pDom->LineTo(Lh1,poCenLeft);

		}
		if(nNextDraw) 
		{
//			if(nNextDraw != -1)
			poCenCrossRigh = (Lp2+Rp2)/2;
			pDom->LineTo(Lp2,poCenCrossRigh);
			pDom->LineTo(Lp2,Lh2);
			GetXyMatchSegAndLine(Lh2, Rh2, poCenCrossRigh, vGir, poCenRigh);
			pDom->LineTo(Lh2,poCenRigh);
		}

		// �Ŵ��� ������ ���ι��� ���� �߱�---------���
		if(pBxNext && nNextDraw) 
		{
			pLine->GetStationMatchVerticalByXy(Lp1, dStt);
			pLine->GetStationMatchVerticalByXy(Lp2, dEnd);
			if(bIsStringerPart && i == nSize-1)
				pDom->LineTo(Lp1,Lp2);
			else
				pLine->DrawLS(pDom, dStt, dEnd, dDLeftUpper);

//			if(~(Lp1 - Lh1) == ~(Lp2 - Lh2))
			{
				pLine->GetStationMatchVerticalByXy(Lh1, dStt);
				pLine->GetStationMatchVerticalByXy(Lh2, dEnd);
				pLine->DrawLS(pDom, dStt, dEnd, dDLeftLower);
			}
//			else
//				pDom->LineTo(Lh1,Lh2);
		}

		if(nUpDown == -1 && !bIsStringerPart && !bEndStringerPart)
			m_pDataMng->GetOptionStd()->SetEnvLType(pDom,LT_SOLID);

		if(nFirstDraw) 
		{
//			if(nFirstDraw != -1)
			pDom->LineTo(Rp1,poCenCrossLeft);
			pDom->LineTo(Rp1,Rh1);
			pDom->LineTo(Rh1,poCenLeft);
//			pDom->LineTo(Lh1,Rh1);
		}
		if(nNextDraw) 
		{
//			if(nNextDraw != -1)
			pDom->LineTo(Rp2,poCenCrossRigh);
			pDom->LineTo(Rp2,Rh2);
			pDom->LineTo(Rh2,poCenRigh);
//			pDom->LineTo(Lh2,Rh2);
		}

		// �Ŵ��� ������ ���ι��� ���� �߱�........�ϸ�
		if(pBxNext && nNextDraw) 
		{
			pLineR->GetStationMatchVerticalByXy(Rp1, dStt);
			pLineR->GetStationMatchVerticalByXy(Rp2, dEnd);
			if(bIsStringerPart && i==0)
				pDom->LineTo(Rp1, Rp2);
			else
				pLineR->DrawLS(pDom, dStt, dEnd, dDRightLower);

//			if(~(Rp1 - Rh1) == ~(Rp2 - Rh2))
			{
				pLineR->GetStationMatchVerticalByXy(Rh1, dStt);
				pLineR->GetStationMatchVerticalByXy(Rh2, dEnd);
				pLineR->DrawLS(pDom, dStt, dEnd, dDRightUpper);
			}
//			else
//				pDom->LineTo(Rh1,Rh2);
		}
		
	}

	*pDomP << *pDom;
	delete pDom;
}



// ���κ� ������ ��ǥ ���
// ������ �κ��ϰ�� -1�� �����ϰ� ���κ� ���� �� �׷����� ��� 1�� �����Ѵ�.
// bStt : ���� BX���� ����. ���� ���κ� �ϰ�� TREU, �� ���κ��ϰ�� FALSE
int CAPlateDrawPyung::GetPlanMainPlateHunch(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, CDPointArray ptArr[], BOOL bStt)
{
	ASSERT(pBx);	

	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateGirderApp	*pGirR = pGir->m_pRight;
	CLineInfo		*pLine = pGir->GetLine();
	CLineInfo		*pLineR = NULL;
	if(pGirR)
		pLineR = pGirR->GetLine();
//	CPlateCrossBeam	*pC     = pBx->GetCrossBeam();
//	CPlateCrossBeam	*pCNext = pBxNext->GetCrossBeam();
//	if(pC == NULL || pCNext == NULL)
//		return -1;

	double dHunchWidth = pDB->m_dWidthHunch;	// ������ �������
	double dSta  = 0;
	// ������ �Ŵ� �߽ɺ��� ������ ����(��ġ�� ���۵Ǵ� ��)������ �Ÿ�
	double DisCL =  pGir->GetMaxWidthFlange(TRUE)/2  + dHunchWidth;
	double DisCR = -pGirR->GetMaxWidthFlange(TRUE)/2 - dHunchWidth;
	// �Ŵ� �߽ɺ��� ���κ���ġ���� ����(����), �Ŵ��߾ӿ��� ���κ����� ����, ���κ� ��/2
	//double DisStringerHunchCL = 0, DisStringerHunchCR = 0, DisStringer = 0, WidStringer = 0;

	BOOL bSttHunch = FALSE;
	BOOL bEndHunch = FALSE;
	
	// ������ ��ħ �κ� ���� ����
	CDPoint vAng;
	// �� ������ ����Ʈ
	CDPoint Lp, CLp, CRp, Rp, Lh, CLh, CRh, Rh;
	CDPoint Lp1, Lp2, CLp1, CLp2, CRp1, CRp2, Rp1, Rp2, Lh1, Lh2, CLh1, CLh2, CRh1, CRh2, Rh1, Rh2;
	///////////////////////////////////////////////////////////////////////
	// ��ġ�κ����� ����........�Ŵ����� �ƴ϶� ���κ� ��(�������� ����)�̾�� �Ѵ�
	// ������ ���� �����̼�, �� �����̼�, Bx�����̼�, ���� Bx�����̼�
	double dStaTapperSttLeft, dStaTapperSttRight, dStaTapperEndLeft, dStaTapperEndRight,dStaBxLeft, dStaBxRight;
	// �Ŵ� �߽ɺ��� ���� ���κ� ����� ����������� �Ÿ�
	double dFromGirCenToCrossLeft  = pGir->GetMaxWidthFlange(TRUE)/2;
	double dFromGirCenToCrossRight = pGir->GetDisPlanPlateWebToWeb(pBx);
	
	dStaTapperSttLeft  = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_HUNCHEND), dFromGirCenToCrossLeft, pGir->GetAAA(OFF_STT_HUNCHEND));
	dStaTapperSttRight = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_HUNCHEND), dFromGirCenToCrossRight, pGir->GetAAA(OFF_STT_HUNCHEND));

	dStaTapperEndLeft  = pGir->GetStationBySc(pGir->GetSSS(OFF_END_HUNCHEND), dFromGirCenToCrossLeft, pGir->GetAAA(OFF_END_HUNCHEND));
	dStaTapperEndRight = pGir->GetStationBySc(pGir->GetSSS(OFF_END_HUNCHEND), dFromGirCenToCrossRight, pGir->GetAAA(OFF_END_HUNCHEND));

	dStaBxLeft  = pGir->GetStationBySc(pBx->GetStation(), dFromGirCenToCrossLeft, pBx->GetAngle());
	dStaBxRight = pGir->GetStationBySc(pBx->GetStation(), dFromGirCenToCrossRight, pBx->GetAngle());

	// BX�� ������ �κа��� ���踦 ����ؼ� ������ �κ����� ����
	if(bStt)
	{
		if(dStaBxLeft < dStaTapperSttLeft && dStaBxRight < dStaTapperSttRight)
			bSttHunch = TRUE;
		if(dStaBxLeft > dStaTapperEndLeft || dStaBxRight > dStaTapperEndRight)
			bEndHunch = TRUE;
	}

	if(!bStt)
	{
		if(dStaBxLeft > dStaTapperEndLeft || dStaBxRight > dStaTapperEndRight)
			bEndHunch = TRUE;
		if(dStaBxLeft < dStaTapperSttLeft && dStaBxRight < dStaTapperSttRight)
			bSttHunch = TRUE;
	}

	// ������ �Ŵ��� BX
	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGirR);

	CDPoint A[41];
	double dX[4];
	pGir->GetHunchXyDom(pBx,A);

	// ���κ� ����
	//long nStringerCount = 0;//min(pBx->GetCrossBeam()->m_nQtyStringer_CR, 1);
	// ���κ� ���� ����
	//BOOL bIsStringer   = FALSE;
	//BOOL bLastStringer = FALSE;
/*	if(pBx->IsStringer() && bStt)
		bIsStringer = TRUE;
	else if(pBx->IsStringer() && pBxNext->IsStringer())
		bIsStringer = TRUE;
	else if(!bStt)		// ������ ���κ��� ��Ʈ���Ÿ� ���� �ʴ´�...
	{
		if(pBxNext->IsStringer())
		{
			bLastStringer = TRUE;
			pGir->GetHunchXyDom(pBxNext,A);
			nStringerCount = min(pBxNext->GetCrossBeam()->m_nQtyStringer_CR, 1);
		}
	}
*/
	dX[0] = fabs((A[4].x - A[3].x));
	dX[1] = 0;	// ���κ� ����
	dX[2] = 0;	// ���κ� ������
	dX[3] = fabs((A[6].x - A[5].x));
	
	// ������ ����....����....�κ�....
	if(bSttHunch || bEndHunch)
	{
		CDPoint Fir, Sec, StringerLeft, StringerRight, vAngVer(0, -1);
		// ��Ʈ���� �߰�....���κ� ��� ��� �߰�
		// ��ġ ����
		GetXyMatchSegAndLine(A[3], A[6], A[4], vAngVer.Unit(), Fir);
		GetXyMatchSegAndLine(A[3], A[6], A[5], vAngVer.Unit(), Sec);
//		GetXyMatchSegAndLine(A[3], A[6], A[11], vAngVer.Unit(), StringerLeft);
//		GetXyMatchSegAndLine(A[3], A[6], A[15], vAngVer.Unit(), StringerRight);
		double dHunchHeightL = (A[4].y - Fir.y);
		double dHunchHeightR = (A[5].y - Sec.y);
		double dHunchHeightStringerLeft  = 0;//(A[11].y - StringerLeft.y);
		double dHunchHeightStringerRight = 0;//(A[15].y - StringerRight.y);

		double dSlop = bSttHunch? 1 / (pDB->m_dHeightTaperStt / pDB->m_dLengthTaperToTaperStt) :
								  1 / (pDB->m_dHeightTaperEnd / pDB->m_dLengthTaperToTaperEnd) ;

		// ��ġ ����
		double dHunchLengthL = dHunchHeightL * dSlop;
		double dHunchLengthR = dHunchHeightR * dSlop;
		double dHunchLengthStringerLeft = dHunchHeightStringerLeft * dSlop;
		double dHunchLengthStringerRight = dHunchHeightStringerRight * dSlop;

		long nPos = bSttHunch ? 1 : 0;

		// �Ŵ����� ��ġ ����....
		CDPoint vHunchL(bSttHunch ? dHunchLengthL : -dHunchLengthL, -dX[0]);
		CDPoint vHunchR(bSttHunch ? dHunchLengthR : -dHunchLengthR,  dX[3]);
		CDPoint vHunchStringerLeft(bSttHunch ? dHunchLengthStringerLeft : -dHunchLengthStringerLeft, dX[1]);
		CDPoint vHunchStringerRight(bSttHunch ? dHunchLengthStringerRight : -dHunchLengthStringerRight, -dX[2]);

		// ���κ� ��� �κ�
		dSta  = bSttHunch ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);
		CDPoint vAng = bSttHunch ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);
		CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(dSta);

		Lp = pGir->GetXyGirderDis(dSta, DisCL, vAng);
		Lh = Lp + vGir.Rotate(vHunchL.Unit()) * ~vHunchL;
		pLine->GetStationMatchVerticalByXy(Lh, dSta);
		Lh = pGir->GetXyGirderDis(dSta, DisCL + dX[0]);

		ptArr[nPos].Add(Lp);
		ptArr[nPos].Add(Lh);

/*		// ��Ʈ���� �κ�
		if(bIsStringer || bLastStringer)
		{
			for(int nCount = 0; nCount < nStringerCount; nCount++)
			{
				if(bLastStringer)
					DisStringer = pGir->GetDisStringer(pBxNext, nCount);
				else
					DisStringer = pGir->GetDisStringer(pBx, nCount);
				if(bLastStringer)
					WidStringer = pBxNext->GetStringer()->m_uM;
				else
					WidStringer = pBx->GetStringer()->m_uM;
				DisStringerHunchCL = DisStringer - WidStringer - dHunchWidth;
				DisStringerHunchCR = DisStringer + WidStringer + dHunchWidth;

				dSta  = bSttHunch ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);
				vAng = bSttHunch ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);
				vGir = pGir->GetLine()->GetAngleAzimuth(dSta);
				CLp = pGir->GetXyGirderDis(dSta, DisStringerHunchCL, vAng);
				CLh = CLp + vGir.Rotate(vHunchStringerLeft.Unit()) * ~vHunchStringerLeft;
				pLine->GetStationMatchVerticalByXy(CLh, dSta);
				CLh = pGir->GetXyGirderDis(dSta, DisStringerHunchCL - dX[1]);

				dSta  = bSttHunch ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);
				vAng = bSttHunch ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);
				vGir = pGir->GetLine()->GetAngleAzimuth(dSta);
				CRp = pGir->GetXyGirderDis(dSta, DisStringerHunchCR, vAng);
				CRh = CRp + vGir.Rotate(vHunchStringerRight.Unit()) * ~vHunchStringerRight;
				pLine->GetStationMatchVerticalByXy(CRh, dSta);
				CRh = pGir->GetXyGirderDis(dSta, DisStringerHunchCR + dX[2]);

				ptArr[nPos].Add(CLh);	// ��ܺ��� Add�߱⶧���� ������ �ٲ��....
				ptArr[nPos].Add(CLp);
				ptArr[nPos].Add(CRp);
				ptArr[nPos].Add(CRh);
			}
		}
*/
		// ���κ� �ϴ� �κ�
		dSta  = bSttHunch ? pGirR->GetSSS(OFF_STT_HUNCHEND) : pGirR->GetSSS(OFF_END_HUNCHEND);
		vAng = bSttHunch ? pGirR->GetAAA(OFF_STT_HUNCHEND) : pGirR->GetAAA(OFF_END_HUNCHEND);
		vGir = pGirR->GetLine()->GetAngleAzimuth(dSta);

		Rp = pGirR->GetXyGirderDis(dSta, DisCR, vAng);
		Rh = Rp + vGir.Rotate(vHunchR.Unit()) * ~vHunchR;
		pLineR->GetStationMatchVerticalByXy(Rh, dSta);
		Rh = pGirR->GetXyGirderDis(dSta, DisCR - dX[3]);

		ptArr[nPos].Add(Rh);
		ptArr[nPos].Add(Rp);

		if(bSttHunch != bEndHunch)
			return -1;
		else
			return 1;
	}

	//===================================================
	// �� �Ŵ� ���̾������� �߽��� (�����ǳ��� ����)

	// ��� �Ŵ��� ������(���κ��� ����) �߽���
	CDPoint DiaLvAng = pBx->GetAngle();
	double  DiaLsta  = pGir->GetStationBySc(pBx->GetStation(), pGir->GetMaxWidthFlange(TRUE)/2, DiaLvAng);
	CDPoint DiaLxy   = pGir->GetXyGirderDis(pBx->GetStation(), pGir->GetMaxWidthFlange(TRUE)/2, DiaLvAng);

	// �ϴ� �Ŵ��� ����(���κ��� ������) �߽���
	CDPoint DiaRvAng = pBxSec->GetAngle();
	double  DiaRsta  = pGirR->GetStationBySc(pBxSec->GetStation(),-pGirR->GetMaxWidthFlange(TRUE)/2,DiaRvAng);
	CDPoint DiaRxy   = pGirR->GetXyGirderDis(pBxSec->GetStation(),-pGirR->GetMaxWidthFlange(TRUE)/2,DiaRvAng);

	// ���κ� ����
	CDPoint vGirL = pGir->GetLine()->GetAngleAzimuth(DiaLsta);  // ��� �Ŵ��� ������ ����
	CDPoint vGirR = pGirR->GetLine()->GetAngleAzimuth(DiaRsta); // �ϴ� �Ŵ��� ������ ����
	CDPoint vCros = (DiaLxy-DiaRxy).Unit();	                    // ���κ��� ����(�ϴ�->���)

	// ������
	CDPoint BaseL = DiaLxy;
	CDPoint BaseR = DiaRxy;
	CDPoint BaseStringerL, BaseStringerR;

	// ���������κ���(������) ���κ� ����(������ ��ġ ����)  
	CPlateCrossBeam	*pC = NULL;
	CVBracing       *pVr= NULL; 
	double dWFl = 0;
	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		pC   = pBx->GetSection()->GetCrossBeam();
		dWFl = pC->m_uM_CR;
	}
	else
	{
		pVr  = pBx->GetSection()->GetVBracing();
		dWFl = pBx->GetSection()->GetBeamJewon(0, CSection::B)/2;
	}

	double Wid  = dWFl + dHunchWidth;			// ���κ� ��/2
	double DisL = /*pGir->GetMaxWidthFlange(TRUE)/2  + */dHunchWidth;   // ��� ���������κ��� ������ �Ÿ�
	double DisR = /*pGirR->GetMaxWidthFlange(TRUE)/2 + */dHunchWidth;   // �ϴ� ���������κ��� ������ �Ÿ�

	// ���������κ���(������) ���κ� ���(������ ��ġ ����)
	//double DisStringerL = 0, DisStringerR = 0;

	// ���κ� ������ ������
	BaseL = DiaLxy + vCros * DisL/(vCros*vGirL);
	////////////////////////////////////////////////////////////////////////////////////////
	// ��Ȯ�� ����Ʈ ����� ���� ���Ϳ� ���� ����Ʈ�� �����̼����� ȯ������ �ٽ� ��ǥ�� ȯ��

	// ���κ� ���
	Lp1 = BaseL + vCros.Rotate90() * Wid;
	pLine->GetStationMatchVerticalByXy(Lp1, dSta);
	Lp1 = pGir->GetXyGirderDis(dSta, DisCL);

	vAng = (-vCros - vGirL).Unit();
	Lh1 = Lp1 + vAng * dX[0] / (vAng * vGirL); 
	pLine->GetStationMatchVerticalByXy(Lh1, dSta);
	Lh1 = pGir->GetXyGirderDis(dSta, DisCL + dX[0]);

	Lp2 = BaseL + vCros.RotateInv90() * Wid;
	pLine->GetStationMatchVerticalByXy(Lp2, dSta);
	Lp2 = pGir->GetXyGirderDis(dSta, DisCL);

	vAng = (-vCros + vGirL).Unit();
	Lh2 = Lp2 + vAng * dX[0] / (vAng * vGirL);
	pLine->GetStationMatchVerticalByXy(Lh2, dSta);
	Lh2 = pGir->GetXyGirderDis(dSta, DisCL + dX[0]);

	ptArr[0].Add(Lp1);
	ptArr[1].Add(Lp2);
	ptArr[0].Add(Lh1);
	ptArr[1].Add(Lh2);

	// ��Ʈ���� �κ�
/*	if(bIsStringer || bLastStringer)
	{
		CStringer *pStr = bLastStringer ? pBxNext->GetStringer() : pBx->GetStringer();
		pStr->SetStringerPointAll();
		for(int nCount = 0; nCount < nStringerCount; nCount++)
		{
			if(bLastStringer)
				DisStringer = pGir->GetDisStringer(pBxNext, nCount);
			else
				DisStringer = pGir->GetDisStringer(pBx, nCount);

			WidStringer = pStr->m_uM + dHunchWidth;

			CDPoint poSeroboStt = pStr->GetXySeroboCenter(TRUE);
//			DisStringerL = ~(DiaLxy - poSeroboStt) - WidStringer;
//			DisStringerR = ~(DiaLxy - poSeroboStt) + WidStringer;

			DisStringerL	   = DisStringer - pGir->m_dUWidth/2 - WidStringer;
			DisStringerR	   = DisStringer - pGir->m_dUWidth/2 + WidStringer;
			DisStringerHunchCL = pGir->m_dUWidth/2 + DisStringerL;
			DisStringerHunchCR = pGir->m_dUWidth/2 + DisStringerR;

			BaseL = DiaLxy + vCros * DisStringerL/(vCros*vGirL);
			BaseR = DiaLxy + vCros * DisStringerR/(vCros*vGirL);

			// ���κ� ���
			CLp1 = BaseL + vCros.Rotate90() * Wid;
			pLine->GetStationMatchVerticalByXy(CLp1, dSta);
			CLp1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL);

			vAng = (vCros - vGirL).Unit();
			CLh1 = CLp1 + vAng * dX[1] / (vGirL * vAng);
			pLine->GetStationMatchVerticalByXy(CLh1, dSta);
			CLh1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL - dX[1]);

			CLp2 = BaseL + vCros.RotateInv90() * Wid;
			pLine->GetStationMatchVerticalByXy(CLp2, dSta);
			CLp2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL);
 
			vAng = (vCros + vGirL).Unit();
			CLh2 = CLp2 + vAng * dX[1] / (vGirL * vAng);
			pLine->GetStationMatchVerticalByXy(CLh2, dSta);
			CLh2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL - dX[1]);

			// ���κ� �ϴ�
			CRp1 = BaseR + vCros.Rotate90() * Wid;
			pLine->GetStationMatchVerticalByXy(CRp1, dSta);
			CRp1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR);

			vAng = (-vCros - vGirL).Unit();
			CRh1 = CRp1 + vAng * dX[2] / (vAng * vGirL); 
			pLine->GetStationMatchVerticalByXy(CRh1, dSta);
			CRh1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR + dX[2]);

			CRp2 = BaseR + vCros.RotateInv90() * Wid;
			pLine->GetStationMatchVerticalByXy(CRp2, dSta);
			CRp2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR);

			vAng = (-vCros + vGirL).Unit();
			CRh2 = CRp2 + vAng * dX[2] / (vAng * vGirL);
			pLine->GetStationMatchVerticalByXy(CRh2, dSta);
			CRh2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR + dX[2]);

			ptArr[0].Add(CLh1);
			ptArr[1].Add(CLh2);
			ptArr[0].Add(CLp1);
			ptArr[1].Add(CLp2);

			ptArr[0].Add(CRp1);
			ptArr[1].Add(CRp2);
			ptArr[0].Add(CRh1);
			ptArr[1].Add(CRh2);
		}
	}*/

	BaseR = DiaRxy + vCros * DisR/(vGirR*vCros);
	// ���κ� �ϴ�
	Rp1 = BaseR + vCros.Rotate90() * Wid;
	pLineR->GetStationMatchVerticalByXy(Rp1, dSta);
	Rp1 = pGirR->GetXyGirderDis(dSta, DisCR);

	vAng = (vCros - vGirR).Unit();
	Rh1 = Rp1 + vAng * dX[3] / (vGirR * vAng);
	pLineR->GetStationMatchVerticalByXy(Rh1, dSta);
	Rh1 = pGirR->GetXyGirderDis(dSta, DisCR - dX[3]);

	Rp2 = BaseR + vCros.RotateInv90() * Wid;
	pLineR->GetStationMatchVerticalByXy(Rp2, dSta);
	Rp2 = pGirR->GetXyGirderDis(dSta, DisCR);
 
	vAng = (vCros + vGirR).Unit();
	Rh2 = Rp2 + vAng * dX[3] / (vGirR * vAng);
	pLineR->GetStationMatchVerticalByXy(Rh2, dSta);
	Rh2 = pGirR->GetXyGirderDis(dSta, DisCR - dX[3]);

	ptArr[0].Add(Rh1);
	ptArr[1].Add(Rh2);
	ptArr[0].Add(Rp1);
	ptArr[1].Add(Rp2);

	return 1;
}


//<summary>
//��ġ�� ������ �������� üũ �� ������ ��ǥ ����
//</summary>
//<param name="ptArr[2][6]">GetHunchXyDom ���� ���� �� ��ġ�θ� ���� ��ǥ�迭</param>
//<param name="ptArrNext[2][6]">���� CrossBeam�� ��ġ�� ��ǥ�迭</param>
void CAPlateDrawPyung::ModifyHunchCross(CDPointArray ptArr[], CDPointArray ptArrNext[])
{
	CDPoint ptResult;
	long nSize = ptArr[1].GetSize()/4;
	// Check #1
	if(GetXyMatchSegAndSeg(ptArr[1][0],ptArr[1][1],ptArrNext[0][0],ptArrNext[0][1],ptResult))
	{		
		ptArrNext[0][1] = ptArr[1][1] = ptResult;
	}
	// Check #2
	if(GetXyMatchSegAndSeg(ptArr[1][2],ptArr[1][3],ptArrNext[0][2],ptArrNext[0][3],ptResult))
	{
		ptArrNext[0][2] = ptArr[1][2] = ptResult;
	}

	if(nSize >=2)
	{
		// Check #1
		if(GetXyMatchSegAndSeg(ptArr[1][4],ptArr[1][5],ptArrNext[0][4],ptArrNext[0][5],ptResult))
		{		
			ptArrNext[0][5] = ptArr[1][5] = ptResult;
		}
		// Check #2
		if(GetXyMatchSegAndSeg(ptArr[1][6],ptArr[1][7],ptArrNext[0][6],ptArrNext[0][7],ptResult))
		{
			ptArrNext[0][6] = ptArr[1][6] = ptResult;
		}
	}
}




// ���� ���κ� �׸���
void CAPlateDrawPyung::DrawOutSideCrossBeam(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft)
{
/*	if(pBxStt==NULL || pBxEnd==NULL)	return;

	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pBxStt->GetGirder();

	long nG       = pBxStt->GetNumberGirder();
	long GirderSu = pDB->GetGirdersu();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = bLeft ? BX_OUTSIDE_CROSS_LEFT : BX_OUTSIDE_CROSS_RIGHT;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);

	CDomyun* pDom = new CDomyun(pDomP);

	// ���ϸ� SETTING
	long nUpDown = 1;
	if(GirderSu%2==0) { // ¦������ �Ŵ�
		if(nG> (long)GirderSu/2-1) nUpDown =  0;
		if(nG==(long)GirderSu/2-1) nUpDown = -1;
		if(nG< (long)GirderSu/2-1) nUpDown =  1;
	}
	else {
		if(nG>=(long)GirderSu/2) nUpDown =  0;
		if(nG< (long)GirderSu/2) nUpDown =  1;
	}

	while(pBx)
	{
		CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
		if(pBx != pBxEnd)
			DrawPlanOutSidePlateBxOnSlab(pDom, pBx, pBxNext, nUpDown, bLeft);
		pBx = Finder.GetBxNext();
	}

	*pDomP << *pDom;
	delete pDom;*/
}


////////////////////////////////////////////////
// ���� ���κ� ������ �׸���
// 2002. 06. 28 CMS 
void CAPlateDrawPyung::DrawPlanOutSidePlateBxOnSlab(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, long nUpDown, BOOL bLeft)
{
/*	if(pBx==NULL || pBxNext == NULL) return;	
	CARoadDrawStd *pStd = (CARoadDrawStd*)m_pStd;
	CBridgeApp		*pDB   = pStd->m_pDB;
	CPlateGirderApp		*pGir  = pBx->GetGirder();
	CPlateGirderApp		*pGirR = pGir->m_pRight;
	CLineInfo		*pLine = pGir->GetLine();
	CBracket		*pBrk = bLeft ? pBx->GetBracketLeft() : pBx->GetBracketRight();
	if(pBrk == NULL)	return;

	long nLineType = 0;
	switch(nUpDown)
	{
	case -1:
	case  1:
		nLineType = LT_DOT_LINE;
		break;

	case 0:
		nLineType = LT_SOLID;
		break;
	}

	CDomyun* pDom = new CDomyun(pDomP);

	CDPointArray ptArr[2];
	CDPointArray ptArrNext[2];
	int nFirstDraw = -1, nNextDraw = -1;

	/////////////////////////////////////////////////////////////////////////
	nFirstDraw = GetPlanOutSidePlateHunch(pBx, pBxNext, ptArr, TRUE, bLeft);
	if(pBxNext) 
		nNextDraw = GetPlanOutSidePlateHunch(pBxNext, pBx, ptArrNext, FALSE, bLeft);
	//////////////////////////////////////////////////////////////////////////
		
	if(nFirstDraw == -1 && nNextDraw == -1) return;

	ModifyHunchCross(ptArr, ptArrNext);		

	pStd->m_pOptStd->SetEnvLType(pDom,nLineType);

	double dStt = 0, dEnd = 0, dHunchWidth = pDB->m_dWidthHunch;
	double dDLeftUpper, dDLeftLower, dDRightUpper, dDRightLower;
	double dThickWeb = bLeft ? pGir->GetThickJewon(G_W_L,pBx) : pGir->GetThickJewon(G_W_R,pBx);
	// ��ġ�κ� ����-------
	CDPoint A[10], B[10];
	double dX[4];
	bLeft ? pGir->GetHunchXyCantileverLeftDom(pBx, A) : pGir->GetHunchXyCantileverRightDom(pBx, A);

	dX[0] = fabs(A[3].x - A[2].x);
	dX[1] = 0;
	dX[2] = 0;
	dX[3] = fabs(A[5].x - A[4].x);

	if(bLeft)
		GetSwapByMin(dX[0], dX[3]);

	////////////////////////////////////////////////////
	// �׸��� ����� ������ ����.......

	CDPoint Lp1, Lh1, Rh1, Rp1, Lp2, Lh2, Rh2, Rp2;
	long nSize = ptArr[1].GetSize() / 4;
	for(long i = 0; i < nSize; i++)
	{
		Lp1 = ptArr[1][i*4+0];
		Lh1 = ptArr[1][i*4+1];
		Rh1 = ptArr[1][i*4+2];
		Rp1 = ptArr[1][i*4+3];
		
		Lp2 = ptArrNext[0][i*4+0];
		Lh2 = ptArrNext[0][i*4+1];
		Rh2 = ptArrNext[0][i*4+2];
		Rp2 = ptArrNext[0][i*4+3];

		// ���κ� �κ����� �Ǻ�.....������ ������ ��찡 �ֿ� ���� dD�Ÿ��� �޶�����
		// (1) ���κ�, ���κ�
		// (2) ���κ�, ���κ� 
        // (3) ���κ�, ���κ�

		// ������ ���� Ȯ��.....
		long nType = 1;
		if(nSize >= 2)	// ���κ��� ���ԵǾ� �ִ�....
		{
			double dWidStringer = pBx->GetStringer()->m_uM;

			double dDisStringer = pGir->GetDisStringer(pBx, 0);	// �ϴ��� ������Ų��...�ʳ� ���κ��� �ΰ��̻��̸�..
			if(i == 0)				// ������~���κ�
			{
				dDLeftUpper  = pGir->m_dGirderCenter + pGir->m_dUWidth / 2 + pGir->m_dURightSharp + dHunchWidth;
				dDLeftLower  = dDLeftUpper + dX[0];
				dDRightLower = pGir->m_dGirderCenter + dDisStringer - dWidStringer - dHunchWidth;
				dDRightUpper = dDRightLower - dX[1];
			}
			else if(i == nSize-1)	// ���κ�~������
			{
				dDLeftUpper = pGir->m_dGirderCenter + dDisStringer + dWidStringer + dHunchWidth;
				dDLeftLower = dDLeftUpper + dX[2];
				dDRightLower = pGirR->m_dGirderCenter - pGirR->m_dUWidth / 2 - pGirR->m_dULeftSharp - dHunchWidth;
				dDRightUpper = dDRightLower - dX[3];
			}
			else					// ���κ�~���κ�......���� ����....
			{
				dDLeftUpper  = 0;
				dDLeftLower  = 0;
				dDRightUpper = 0;
				dDRightLower = 0;
			}
		}
		else						// ������~������
		{
			if(bLeft)
			{
				dDLeftUpper  = pGir->m_dGirderCenter - pGir->m_dUWidth/2 - dThickWeb - pBrk->m_L1_EB - pBrk->m_L2_EB + dHunchWidth;
				dDRightLower = pGir->m_dGirderCenter - pGir->m_dUWidth/2 - pGir->m_dULeftSharp - dHunchWidth;
			}
			else
			{
				dDLeftUpper  = pGir->m_dGirderCenter + pGir->m_dUWidth/2 + pGir->m_dURightSharp + dHunchWidth;
				dDRightLower = pGir->m_dGirderCenter + pGir->m_dUWidth/2 + dThickWeb + pBrk->m_L1_EB + pBrk->m_L2_EB - dHunchWidth;
			}

			dDLeftLower = dDLeftUpper + dX[0];
			dDRightUpper = dDRightLower - dX[3];
		}

		if(nFirstDraw)
		{
			if(nFirstDraw != -1)
				pDom->LineTo(Lp1,(Lp1+Rp1)/2);
			pDom->LineTo(Lp1,Lh1);
		}
		if(nNextDraw) 
		{
			if(nNextDraw != -1)
				pDom->LineTo(Lp2,(Lp2+Rp2)/2);
			pDom->LineTo(Lp2,Lh2);
		}

		// �Ŵ��� ������ ���ι��� ���� �߱�---------���
		if(pBxNext && nNextDraw) 
		{
			pLine->GetStationMatchVerticalByXy(Lp1, dStt);
			pLine->GetStationMatchVerticalByXy(Lp2, dEnd);
			pLine->DrawLS(pDom, dStt, dEnd, dDLeftUpper);

			if(~(Lp1 - Lh1) == ~(Lp2 - Lh2))
			{
				pLine->GetStationMatchVerticalByXy(Lh1, dStt);
				pLine->GetStationMatchVerticalByXy(Lh2, dEnd);
				pLine->DrawLS(pDom, dStt, dEnd, dDLeftLower);
			}
			else
				pDom->LineTo(Lh1,Lh2);
		}

//		if(nUpDown == -1)
//			pStd->m_pOptStd->SetEnvLType(pDom,LT_SOLID);
//		else
//			pStd->m_pOptStd->SetEnvLType(pDom,LT_DOT_LINE);


		if(nFirstDraw) 
		{
			if(nFirstDraw != -1)
				pDom->LineTo(Rp1,(Lp1+Rp1)/2);
			pDom->LineTo(Rp1,Rh1);
			pDom->LineTo(Lh1,Rh1);
		}
		if(nNextDraw) 
		{
			if(nNextDraw != -1)
				pDom->LineTo(Rp2,(Lp2+Rp2)/2);
			pDom->LineTo(Rp2,Rh2);
			pDom->LineTo(Lh2,Rh2);
		}

		// �Ŵ��� ������ ���ι��� ���� �߱�........�ϸ�
		if(pBxNext && nNextDraw) 
		{
			pLine->GetStationMatchVerticalByXy(Rp1, dStt);
			pLine->GetStationMatchVerticalByXy(Rp2, dEnd);
			pLine->DrawLS(pDom, dStt, dEnd, dDRightLower);

			if(~(Rp1 - Rh1) == ~(Rp2 - Rh2))
			{
				pLine->GetStationMatchVerticalByXy(Rh1, dStt);
				pLine->GetStationMatchVerticalByXy(Rh2, dEnd);
				pLine->DrawLS(pDom, dStt, dEnd, dDRightUpper);
			}
			else
				pDom->LineTo(Rh1,Rh2);
		}
		
	}

	*pDomP << *pDom;
	delete pDom;*/
}

// ���� ���κ� ������ ��ǥ ���
int CAPlateDrawPyung::GetPlanOutSidePlateHunch(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, CDPointArray ptArr[], BOOL bStt, BOOL bLeft)
{
/*	ASSERT(pBx);
	CARoadDrawStd	*pStd  = (CARoadDrawStd*)m_pStd;
	CBridgeApp		*pDB   = pStd->m_pDB;
	CPlateGirderApp		*pGir  = pBx->GetGirder();
	CLineInfo		*pLine = pGir->GetLine();
	CBracket		*pBrk  = bLeft ? pBx->GetBracketLeft() : pBx->GetBracketRight();
	if(pBrk == NULL)	return -1;

	double dHunchWidth = pDB->m_dWidthHunch;
	double dSta = 0;
	double dThickWeb = 0;
	// ������ �Ŵ� �߽ɺ��� �������κ� ������ ���ܱ����� �Ÿ�
	double DisCL, DisCR;
	if(bLeft)
	{
		dThickWeb = pGir->GetThickJewon(G_W_L,pBx);
		DisCL = -pGir->m_dUWidth/2 - dThickWeb - pBrk->m_L1_EB - pBrk->m_L2_EB + dHunchWidth;
		DisCR = -pGir->m_dUWidth/2 - pGir->m_dULeftSharp - dHunchWidth;
	}
	else
	{
		dThickWeb = pGir->GetThickJewon(G_W_R,pBx);
		DisCL = pGir->m_dUWidth/2 + pGir->m_dURightSharp + dHunchWidth;
		DisCR = pGir->m_dUWidth/2 + dThickWeb + pBrk->m_L1_EB + pBrk->m_L2_EB - dHunchWidth;
	}

	// �ʿ����
	// �Ŵ� �߽ɺ��� ���κ���ġ���� ����(����), �Ŵ��߾ӿ��� ���κ����� ����, ���κ� ��/2
	//double DisStringerHunchCL = 0, DisStringerHunchCR = 0, DisStringer = 0, WidStringer = 0;

	BOOL bSttHunch = FALSE, bEndHunch = FALSE;
	
	// ������ ��ħ �κ� ���� ����
	CDPoint vAng;
	// �� ������ ����Ʈ
	CDPoint Lp, CLp, CRp, Rp, Lh, CLh, CRh, Rh;
	CDPoint Lp1, Lp2, CLp1, CLp2, CRp1, CRp2, Rp1, Rp2, Lh1, Lh2, CLh1, CLh2, CRh1, CRh2, Rh1, Rh2;

	///////////////////////////////////////////////////////////////////////
	// ��ġ�κ����� ����........�Ŵ����� �ƴ϶� ���� ���κ� ��(�������� ����)�̾�� �Ѵ�

	// ������ ���� �����̼�, �� �����̼�, Bx�����̼�, ���� Bx�����̼�
	double dStaTapperSttLeft, dStaTapperSttRight, dStaTapperEndLeft, dStaTapperEndRight,dStaBxLeft, dStaBxRight;

	// �Ŵ� �߽ɺ��� �������κ� ����� ����������� �Ÿ�
	double dFromGirCenToCrossLeft, dFromGirCenToCrossRight;
	if(bLeft)
	{
		dThickWeb = pGir->GetThickJewon(G_W_L,pBx);
		dFromGirCenToCrossLeft  = -pGir->m_dUWidth/2 - dThickWeb - pBrk->m_L1_EB - pBrk->m_L2_EB;
		dFromGirCenToCrossRight = -pGir->m_dUWidth/2 - pGir->m_dLeftSharp;
	}
	else
	{
		dThickWeb = pGir->GetThickJewon(G_W_R,pBx);
		dFromGirCenToCrossLeft  = pGir->m_dUWidth/2 + pGir->m_dRightSharp;
		dFromGirCenToCrossRight = pGir->m_dUWidth/2 + dThickWeb + pBrk->m_L1_EB + pBrk->m_L2_EB;
	}

	
	dStaTapperSttLeft = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_HUNCHEND), dFromGirCenToCrossLeft, pGir->GetAAA(OFF_STT_HUNCHEND));
	dStaTapperSttRight = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_HUNCHEND), dFromGirCenToCrossRight, pGir->GetAAA(OFF_STT_HUNCHEND));

	dStaTapperEndLeft = pGir->GetStationBySc(pGir->GetSSS(OFF_END_HUNCHEND), dFromGirCenToCrossLeft, pGir->GetAAA(OFF_END_HUNCHEND));
	dStaTapperEndRight = pGir->GetStationBySc(pGir->GetSSS(OFF_END_HUNCHEND), dFromGirCenToCrossRight, pGir->GetAAA(OFF_END_HUNCHEND));

	dStaBxLeft = pGir->GetStationBySc(pBx->GetStation(), dFromGirCenToCrossLeft, pBx->GetAngle());
	dStaBxRight = pGir->GetStationBySc(pBx->GetStation(), dFromGirCenToCrossRight, pBx->GetAngle());

	// BX�� ������ �κа��� ���踦 ����ؼ� ������ �κ����� ����
	if(bStt)
	{
		if(dStaBxLeft < dStaTapperSttLeft && dStaBxRight < dStaTapperSttRight)
			bSttHunch = TRUE;
		if(dStaBxLeft > dStaTapperEndLeft || dStaBxRight > dStaTapperEndRight)
			bEndHunch = TRUE;
	}

	if(!bStt)
	{
		if(dStaBxLeft > dStaTapperEndLeft || dStaBxRight > dStaTapperEndRight)
			bEndHunch = TRUE;
		if(dStaBxLeft < dStaTapperSttLeft && dStaBxRight < dStaTapperSttRight)
			bSttHunch = TRUE;
	}

	CDPoint A[10];
	double dX[4];

	if(bLeft)
		pGir->GetHunchXyCantileverLeftDom(pBx, A);
	else
		pGir->GetHunchXyCantileverRightDom(pBx, A);

	// ���κ� ����
	long nStringerCount = 0;

	// ���κ� ���� ����
	BOOL bIsStringer = FALSE;
	BOOL bLastStringer = FALSE;
	/*
	if(pBx->IsStringer())
		bIsStringer = TRUE;
	else if(!bStt)		// ������ ���κ��� ��Ʈ���Ÿ� ���� �ʴ´�...
	{
		if(pBxNext->IsStringer())
		{
			bLastStringer = TRUE;
			pGir->GetHunchXyDom(pBxNext,A);
			nStringerCount = pBxNext->GetCrossBeam()->m_nQtyStringer_CR;
		}
	}
	* /

	dX[0] = fabs(A[3].x - A[2].x);
//	dX[1] = (A[12].x - A[11].x);	// ���κ� ����
//	dX[2] = (A[15].x - A[14].x);	// ���κ� ������
	dX[3] = fabs(A[5].x - A[4].x);
	if(bLeft)
		GetSwapByMin(dX[0],dX[3]);
	
	// ������ ����....����....�κ�....
	if(bSttHunch || bEndHunch)
	{

		CDPoint Fir, Sec, StringerLeft, StringerRight, vAngVer(0, -1);

		// ��Ʈ���� �߰�....���κ� ��� ��� �߰�
		// ��ġ ����
		GetXyMatchSegAndLine(A[2], A[5], A[4], vAngVer.Unit(), Fir);
		GetXyMatchSegAndLine(A[2], A[5], A[3], vAngVer.Unit(), Sec);
//		GetXyMatchSegAndLine(A[2], A[5], A[11], vAngVer.Unit(), StringerLeft);
//		GetXyMatchSegAndLine(A[2], A[5], A[15], vAngVer.Unit(), StringerRight);
		double dHunchHeightL = (A[4].y - Fir.y);
		double dHunchHeightR = (A[3].y - Sec.y);
//		double dHunchHeightStringerLeft = (A[11].y - StringerLeft.y);
//		double dHunchHeightStringerRight = (A[15].y - StringerRight.y);

		double dSlop = bSttHunch? 1 / (pDB->m_dHeightTaperStt / pDB->m_dLengthTaperToTaperStt) :
								  1 / (pDB->m_dHeightTaperEnd / pDB->m_dLengthTaperToTaperEnd) ;

		// ��ġ ����
		double dHunchLengthL = dHunchHeightL * dSlop;
		double dHunchLengthR = dHunchHeightR * dSlop;
//		double dHunchLengthStringerLeft = dHunchHeightStringerLeft * dslope;
//		double dHunchLengthStringerRight = dHunchHeightStringerRight * dSlop;

		long nPos = bSttHunch ? 1 : 0;

		// �Ŵ����� ��ġ ����....
		CDPoint vHunchL(bSttHunch ? dHunchLengthL : -dHunchLengthL, -dX[0]);
		CDPoint vHunchR(bSttHunch ? dHunchLengthR : -dHunchLengthR,  dX[3]);
//		CDPoint vHunchStringerLeft(bSttHunch ? dHunchLengthStringerLeft : -dHunchLengthStringerLeft, dX[1]);
//		CDPoint vHunchStringerRight(bSttHunch ? dHunchLengthStringerRight : -dHunchLengthStringerRight, -dX[2]);

		// ���κ� ��� �κ�
		dSta  = bSttHunch ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);
		CDPoint vAng = bSttHunch ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);
		CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(dSta);

		Lp = pGir->GetXyGirderDis(dSta, DisCL, vAng);
		Lh = Lp + vGir.Rotate(vHunchL.Unit()) * ~vHunchL;
		pLine->GetStationMatchVerticalByXy(Lh, dSta);
		Lh = pGir->GetXyGirderDis(dSta, DisCL + dX[0]);

		ptArr[nPos].Add(Lp);
		ptArr[nPos].Add(Lh);

		/*
		// ��Ʈ���� �κ�
		if(bIsStringer || bLastStringer)
		{
			for(int nCount = 0; nCount < nStringerCount; nCount++)
			{
				if(bLastStringer)
					DisStringer = pGir->GetDisStringer(pBxNext, nCount);
				else
					DisStringer = pGir->GetDisStringer(pBx, nCount);
				if(bLastStringer)
					WidStringer = pBxNext->GetStringer()->m_uM;
				else
					WidStringer = pBx->GetStringer()->m_uM;
				DisStringerHunchCL = DisStringer - WidStringer - dHunchWidth;
				DisStringerHunchCR = DisStringer + WidStringer + dHunchWidth;

				dSta  = bSttHunch ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);
				vAng = bSttHunch ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);
				vGir = pGir->GetLine()->GetAngleAzimuth(dSta);
				CLp = pGir->GetXyGirderDis(dSta, DisStringerHunchCL, vAng);
				CLh = CLp + vGir.Rotate(vHunchStringerLeft.Unit()) * ~vHunchStringerLeft;
				pLine->GetStationMatchVerticalByXy(CLh, dSta);
				CLh = pGir->GetXyGirderDis(dSta, DisStringerHunchCL - dX[1]);

				dSta  = bSttHunch ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);
				vAng = bSttHunch ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);
				vGir = pGir->GetLine()->GetAngleAzimuth(dSta);
				CRp = pGir->GetXyGirderDis(dSta, DisStringerHunchCR, vAng);
				CRh = CRp + vGir.Rotate(vHunchStringerRight.Unit()) * ~vHunchStringerRight;
				pLine->GetStationMatchVerticalByXy(CRh, dSta);
				CRh = pGir->GetXyGirderDis(dSta, DisStringerHunchCR + dX[2]);

				ptArr[nPos].Add(CLh);	// ��ܺ��� Add�߱⶧���� ������ �ٲ��....
				ptArr[nPos].Add(CLp);
				ptArr[nPos].Add(CRp);
				ptArr[nPos].Add(CRh);
			}
		}
		* /

		// ���κ� �ϴ� �κ�
		dSta  = bSttHunch ? pGir->GetSSS(OFF_STT_HUNCHEND) : pGir->GetSSS(OFF_END_HUNCHEND);
		vAng = bSttHunch ? pGir->GetAAA(OFF_STT_HUNCHEND) : pGir->GetAAA(OFF_END_HUNCHEND);
		vGir = pGir->GetLine()->GetAngleAzimuth(dSta);

		Rp = pGir->GetXyGirderDis(dSta, DisCR, vAng);
		Rh = Rp + vGir.Rotate(vHunchR.Unit()) * ~vHunchR;
		pLine->GetStationMatchVerticalByXy(Rh, dSta);
		Rh = pGir->GetXyGirderDis(dSta, DisCR - dX[3]);

		ptArr[nPos].Add(Rh);
		ptArr[nPos].Add(Rp);

		if(bSttHunch != bEndHunch)
			return -1;
		else
			return 1;
	}

	//===================================================
	// �� �Ŵ� ���̾������� �߽��� (�����ǳ��� ����)

	// ��� �Ŵ��� ������(���κ��� ����) �߽���
	dThickWeb = pGir->GetThickJewon(bLeft?G_W_L:G_W_R,pBx);
	double DisOutLeft = bLeft ? -pGir->m_dUWidth/2 - dThickWeb - pBrk->m_L1_EB - pBrk->m_L2_EB 
		                      :  pGir->m_dUWidth/2;
	CDPoint DiaLvAng  = pBx->GetAngle();
	double  DiaLsta   = pGir->GetStationBySc(pBx->GetStation(),DisOutLeft,DiaLvAng);
	CDPoint DiaLxy    = pGir->GetXyGirderDis(pBx->GetStation(),DisOutLeft,DiaLvAng);

	// �ϴ� �Ŵ��� ����(���κ��� ������) �߽���
	double DisOutRight = bLeft ? -pGir->m_dUWidth/2 
		                       :  pGir->m_dUWidth/2 + dThickWeb + pBrk->m_L1_EB + pBrk->m_L2_EB;
	CDPoint DiaRvAng = pBx->GetAngle();
	double  DiaRsta  = pGir->GetStationBySc(pBx->GetStation(),DisOutRight,DiaRvAng);
	CDPoint DiaRxy   = pGir->GetXyGirderDis(pBx->GetStation(),DisOutRight,DiaRvAng);

	// ���κ� ����
	CDPoint vGirL = pGir->GetLine()->GetAngleAzimuth(DiaLsta);  // ��� �Ŵ��� ������ ����
	CDPoint vGirR = pGir->GetLine()->GetAngleAzimuth(DiaRsta); // �ϴ� �Ŵ��� ������ ����
	CDPoint vCros = (DiaLxy-DiaRxy).Unit();	                    // ���κ��� ����(�ϴ�->���)

	// ������
	CDPoint BaseL = DiaLxy;
	CDPoint BaseR = DiaRxy;
	CDPoint BaseStringerL, BaseStringerR;

	// ���������κ���(������) ���κ� ����(������ ��ġ ����)  
	double Wid  = pBrk->m_uM_EB + dHunchWidth;			// ���κ� ��/2
	double DisL = bLeft ? dThickWeb - pBrk->m_L1_EB - pBrk->m_L1_EB - dHunchWidth 
		                : pGir->m_dURightSharp + dHunchWidth;   // ��� ���������κ��� ������ �Ÿ�
	double DisR = bLeft ? pGir->m_dULeftSharp + dHunchWidth 
		                : dThickWeb + pBrk->m_L1_EB + pBrk->m_L2_EB - dHunchWidth;   // �ϴ� ���������κ��� ������ �Ÿ�

	// ���������κ���(������) ���κ� ���(������ ��ġ ����)
	double DisStringerL = 0, DisStringerR = 0;

	// ���κ� ������ ������
	BaseL = DiaLxy + vCros * DisL/(vCros*vGirL);
	////////////////////////////////////////////////////////////////////////////////////////
	// ��Ȯ�� ����Ʈ ����� ���� ���Ϳ� ���� ����Ʈ�� �����̼����� ȯ������ �ٽ� ��ǥ�� ȯ��

	// ���κ� ���
	Lp1 = BaseL + vCros.Rotate90() * Wid;
	pLine->GetStationMatchVerticalByXy(Lp1, dSta);
	Lp1 = pGir->GetXyGirderDis(dSta, DisCL);

	vAng = (-vCros - vGirL).Unit();
	Lh1 = Lp1 + vAng * dX[0] / (vAng * vGirL); 
	pLine->GetStationMatchVerticalByXy(Lh1, dSta);
	Lh1 = pGir->GetXyGirderDis(dSta, DisCL + dX[0]);

	Lp2 = BaseL + vCros.RotateInv90() * Wid;
	pLine->GetStationMatchVerticalByXy(Lp2, dSta);
	Lp2 = pGir->GetXyGirderDis(dSta, DisCL);

	vAng = (-vCros + vGirL).Unit();
	Lh2 = Lp2 + vAng * dX[0] / (vAng * vGirL);
	pLine->GetStationMatchVerticalByXy(Lh2, dSta);
	Lh2 = pGir->GetXyGirderDis(dSta, DisCL + dX[0]);

	ptArr[0].Add(Lp1);
	ptArr[1].Add(Lp2);
	ptArr[0].Add(Lh1);
	ptArr[1].Add(Lh2);

	/*
	// ��Ʈ���� �κ�
	if(bIsStringer || bLastStringer)
	{
		for(int nCount = 0; nCount < nStringerCount; nCount++)
		{
			if(bLastStringer)
				DisStringer = pGir->GetDisStringer(pBxNext, nCount);
			else
				DisStringer = pGir->GetDisStringer(pBx, nCount);

			if(bLastStringer)
				WidStringer = pBxNext->GetStringer()->m_uM + dHunchWidth;
			else
				WidStringer = pBx->GetStringer()->m_uM + dHunchWidth;
			DisStringerL = DisStringer - pGir->m_dUWidth/2 - WidStringer;
			DisStringerR = DisStringer - pGir->m_dUWidth/2 + WidStringer;

			DisStringerHunchCL = pGir->m_dUWidth/2 + DisStringerL;
			DisStringerHunchCR = pGir->m_dUWidth/2 + DisStringerR;

			BaseL = DiaLxy + vCros * DisStringerL/(vCros*vGirL);
			BaseR = DiaLxy + vCros * DisStringerR/(vCros*vGirL);
			
			// ���κ� ���
			CLp1 = BaseL + vCros.Rotate90() * Wid;
			pLine->GetStationMatchVerticalByXy(CLp1, dSta);
			CLp1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL);

			vAng = (vCros - vGirL).Unit();
			CLh1 = CLp1 + vAng * dX[1] / (vGirL * vAng);
			pLine->GetStationMatchVerticalByXy(CLh1, dSta);
			CLh1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL - dX[1]);

			CLp2 = BaseL + vCros.RotateInv90() * Wid;
			pLine->GetStationMatchVerticalByXy(CLp2, dSta);
			CLp2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL);
 
			vAng = (vCros + vGirL).Unit();
			CLh2 = CLp2 + vAng * dX[1] / (vGirL * vAng);
			pLine->GetStationMatchVerticalByXy(CLh2, dSta);
			CLh2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCL - dX[1]);

			// ���κ� �ϴ�
			CRp1 = BaseR + vCros.Rotate90() * Wid;
			pLine->GetStationMatchVerticalByXy(CRp1, dSta);
			CRp1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR);

			vAng = (-vCros - vGirL).Unit();
			CRh1 = CRp1 + vAng * dX[2] / (vAng * vGirL); 
			pLine->GetStationMatchVerticalByXy(CRh1, dSta);
			CRh1 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR + dX[2]);

			CRp2 = BaseR + vCros.RotateInv90() * Wid;
			pLine->GetStationMatchVerticalByXy(CRp2, dSta);
			CRp2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR);

			vAng = (-vCros + vGirL).Unit();
			CRh2 = CRp2 + vAng * dX[2] / (vAng * vGirL);
			pLine->GetStationMatchVerticalByXy(CRh2, dSta);
			CRh2 = pGir->GetXyGirderDis(dSta, DisStringerHunchCR + dX[2]);

			ptArr[0].Add(CLh1);
			ptArr[1].Add(CLh2);
			ptArr[0].Add(CLp1);
			ptArr[1].Add(CLp2);

			ptArr[0].Add(CRp1);
			ptArr[1].Add(CRp2);
			ptArr[0].Add(CRh1);
			ptArr[1].Add(CRh2);
		}
	}
	* /

	BaseR = DiaRxy + vCros * DisR/(vGirR*vCros);
	// ���κ� �ϴ�
	Rp1 = BaseR + vCros.Rotate90() * Wid;
	pLine->GetStationMatchVerticalByXy(Rp1, dSta);
	Rp1 = pGir->GetXyGirderDis(dSta, DisCR);

	vAng = (vCros - vGirR).Unit();
	Rh1 = Rp1 + vAng * dX[3] / (vGirR * vAng);
	pLine->GetStationMatchVerticalByXy(Rh1, dSta);
	Rh1 = pGir->GetXyGirderDis(dSta, DisCR - dX[3]);

	Rp2 = BaseR + vCros.RotateInv90() * Wid;
	pLine->GetStationMatchVerticalByXy(Rp2, dSta);
	Rp2 = pGir->GetXyGirderDis(dSta, DisCR);
 
	vAng = (vCros + vGirR).Unit();
	Rh2 = Rp2 + vAng * dX[3] / (vGirR * vAng);
	pLine->GetStationMatchVerticalByXy(Rh2, dSta);
	Rh2 = pGir->GetXyGirderDis(dSta, DisCR - dX[3]);

	ptArr[0].Add(Rh1);
	ptArr[1].Add(Rh2);
	ptArr[0].Add(Rp1);
	ptArr[1].Add(Rp2);
*/
	return 1;
}