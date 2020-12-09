// APlateOutDetail.cpp: implementation of the APlateOutDetail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DET_SCALE_MAIN			0
#define DET_SCALE_SUMARY_HOR	1
#define DET_SCALE_SUMARY_VER	2
#define DET_SCALE_WELD			3
#define DET_SCALE_SOLEPLATE		4
#define DET_SCALE_VSITFF		5
#define DET_SCALE_HSTIFF1		6
#define DET_SCALE_HSTIFF2		7
#define DET_SCALE_TU			8

#define UPPER_PLAN	10
#define LOWER_PLAN	20
#define WEB			30
#define SOLEPLATE	40
#define HSTIFFDET1	50
#define HSTIFFDET2	60
#define VSTIFFDET	70
#define BUM			80
#define NOTE		90
#define KEY_PLAN	100
#define DAN			200


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutDetail::CAPlateOutDetail()
{
	m_WeldDetailMoveNum = 500;
	m_cWeldTypeName = 'A';
	m_MaxSebuDetailSu = 0;
}

CAPlateOutDetail::~CAPlateOutDetail()
{

}

void CAPlateOutDetail::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove	    = pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
		
	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// �Ϲݵ������� �����ȣ
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();

	m_WeldDetailMoveNum = 500;
	m_cWeldTypeName = 'A';
	m_MaxSebuDetailSu = 0;

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	

	// ����
	pPageMng->SetProgressData("���� �����...",0);
	pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);	
	pStd->AddMaterial(&Dom,Scale);
	pStd->AddJubujebubuje(&Dom,Scale);
	Dom.SetCalcExtRect();
	CDRect dgkRect = Dom.GetExtRect(), DomBound;
	*pDom << Dom;			
	////////////////////////////
	pDom->SetCalcExtRect();
	CDRect	DgkBound = pDom->GetExtRect();
	CDPoint xyMove = CDPoint(Dom.Always(50), DgkBound.Height()-Dom.Always(80));
	if(pDomOpt->m_Det_bPlanView)
	{
//	EXIT_RTN;
		pPageMng->SetProgressData("��鵵 ��� ����� ...",5);
		AddPlan(&Dom, nSubPage, TRUE);
		Dom.SetScaleDim(pDom->GetScaleDim());
	
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMove.y -= rect.Height()+Dom.Always(40);

		// ���� ������ �����̵�
		Dom.Move(pMove->GetPoint(UPPER_PLAN));	
		Dom.SetCalcExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), UPPER_PLAN, "��鵵(���)");	
		*pDom << Dom;

		DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), "��鵵(���)", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(UPPER_PLAN+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(), UPPER_PLAN+1, "��鵵 ����");
		*pDom << Dom;
	}
	if(pDomOpt->m_Det_bJongView)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("���鵵 ����� ...",10);
		AddJong(&Dom, nSubPage);
		Dom.SetScaleDim(pDom->GetScaleDim());
	
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMove.y -= rect.Height()+Dom.Always(30);

		// ���� ������ �����̵�
		Dom.Move(pMove->GetPoint(WEB));	
		Dom.SetCalcExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), WEB, "���鵵");	
		*pDom << Dom;

		DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), "���鵵", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(WEB+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(), WEB+1, "���鵵");
		*pDom << Dom;
	}

	if(pBridge->IsTUGir() && pDomOpt->m_Det_bGirderdan)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("�ܸ鵵 ����� ...",15);
		AddDan(&Dom, nSubPage);		
		Dom.SetScaleDim(pDom->GetScaleDim());
		*pDom << Dom;
	}
	
	if(pDomOpt->m_Det_bPlanView)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("��鵵 �Ϻ� ����� ...",20);
		AddPlan(&Dom, nSubPage, FALSE);
		Dom.SetScaleDim(pDom->GetScaleDim());
	
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMove.y -= rect.Height()+Dom.Always(40);

		// ���� ������ �����̵�
		Dom.Move(pMove->GetPoint(LOWER_PLAN));	
		Dom.SetCalcExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), LOWER_PLAN, "��鵵(�Ϻ�)");	
		*pDom << Dom;

		DrawStd.AddTitleOnDom(&Dom, GetScale(), Dom.GetExtRect(), "��鵵(�Ϻ�)", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(LOWER_PLAN+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(), LOWER_PLAN+1, "��鵵 ����");
		*pDom << Dom;
	}

	if(pDomOpt->m_Det_bBasicNote)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("��Ʈ���� �����...",38);
		InitNote();
		AddNote(&Dom,nSubPage);
		*pDom << Dom;			
	}
	
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage);

	CDPoint xy(0,0);
	CDPoint NextPoint = CDPoint(Dom.Always(750), dgkRect.top+Dom.Always(520));
	if(pBridge->IsTUGir())
		NextPoint = CDPoint(Dom.Always(900), dgkRect.bottom-Dom.Always(50));
	
	if(pDomOpt->m_Det_bSolePlateDetail)
	{		
		pPageMng->SetProgressData("���÷���Ʈ ���ε� �����...",40);						
		xy = DrawSolPlate(&Dom, NextPoint.x, NextPoint.y, pBxStt, pBxEnd);
		*pDom << Dom;
	}
	pPageMng->SetProgressData("����, ���������� �����󼼵� �����...",45);
	NextPoint = xy;
	if(pDomOpt->m_Det_bVStiffDetail)
	{
		xy = DrawVStiffDetail(&Dom,NextPoint.x, NextPoint.y, pBxStt, pBxEnd);
		*pDom << Dom;
	}
//	NextPoint = CDPoint(Dom.Always(750), Dom.Always(340));
	NextPoint = xy;
	if(pDomOpt->m_Det_bHStiffDetail)
	{
		xy = DrawHStiffDetail(&Dom,NextPoint.x, NextPoint.y, pBxStt, pBxEnd);
		*pDom << Dom;
	}
//	NextPoint = CDPoint(Dom.Always(750), Dom.Always(250));
	NextPoint = xy;

	CDPoint FieldPoint = NextPoint;

	m_WeldDetailMoveNum = 500;

	if(pDomOpt->m_Det_bWeldDetail)
	{
		for (long i = 0; i<3; i++)
		{
			pPageMng->SetProgressData("�����󼼵� �����...",45+(10*i));						
			xy = DrawWeldDetail(&Dom, NextPoint.x, NextPoint.y, pBxStt, pBxEnd, i);
			*pDom << Dom;
			NextPoint = xy;
		}
	}

	if(pBridge->IsTUGir())
		FieldPoint = CDPoint(Dom.Always(850), dgkRect.bottom-Dom.Always(50));
	if(pDomOpt->m_Det_bFieldWeldDetail)
	{
		pPageMng->SetProgressData("�����󼼵� �����...",70);
		DrawFieldWeldDetail(&Dom, FieldPoint.x+5000, FieldPoint.y, pBxStt, pBxEnd);
		*pDom << Dom;
	}

	if(pDomOpt->m_Det_bKeyPlan)
	{
//		EXIT_RTN;
		double dKeyScaleHor = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_SUMARY_HOR);
		pPageMng->SetProgressData("��൵ �����...",80);						
		AddKeyPan(&Dom, pBxStt, pBxEnd);
		Dom.SetScaleDim(pDom->GetScaleDim());
		
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		xyMove = CDPoint(DgkBound.Width()/2 + Dom.Always(50), -Dom.Always(50));
		Dom.Move(-rect.right, -rect.bottom); 
		Dom.Move(xyMove);		

		// ���� ������ �����̵�
		Dom.Move(pMove->GetPoint(KEY_PLAN));	
		Dom.SetCalcExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), KEY_PLAN, "�� �� ��");	
		*pDom << Dom;

		rect = Dom.GetExtRect();
		DrawStd.AddTitleOnDom(&Dom, dKeyScaleHor, rect, "�� �� ��", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(KEY_PLAN+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(), KEY_PLAN+1, "��൵ ����");
		*pDom << Dom;
	}
	if(pDomOpt->m_Det_bLegend)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("���� ����� ...",90);
		AddBeomye(&Dom);
		Dom.SetCalcExtRect();		
		Dom.SetScaleDim(pDom->GetScaleDim());

		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		
		xyMove = CDPoint(Dom.Always(10), DgkBound.Height() - Dom.Always(40));
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.bottom);

		// ���� ������ ���� �̵�
		Dom.Move(pMove->GetPoint(BUM));		
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),BUM,"����");	
		*pDom << Dom;
	}

	pPageMng->SetProgressData("��¿Ϸ�",100);
	*pDom << Dom;
}

long CAPlateOutDetail::GetPageSu()
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();

	m_nPage = 0;
	long nType = pStd->m_pDomOptionStd->m_Det_nDivPage;
	if(nType==0)
	{
		m_nPage = pBridge->m_nQtyJigan * pBridge->GetGirdersu();
	}
	else if(nType==1)
	{
		for(long nG=0;nG<pBridge->GetGirdersu();nG++)
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			Finder.GetBxFirst(BX_SPLICE | BX_END_GIRDER);
			m_nPage += Finder.GetCountBx();
		}
	}
	else if(nType==2)//���κ� ������ �����ϰ��...
	{
		long nCrossNum = pStd->m_pDomOptionStd->m_Det_nUnitEaDraw;

		for(long nGir=0; nGir<pBridge->GetGirdersu(); nGir++)
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nGir));
			Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			long nCross = (long)Finder.GetCountBx();
			long nCount = (long)RoundUp((nCross-1.0)/nCrossNum,0);//1.0 �� .0 �Ⱥٿ����� ������ 1�̳� ����..��...070129...KB...

			//long nCount = (long)((nCross-1)/(nCrossNum-1));
			//if((nCross-1)%(nCrossNum-1)) nCount++;
			m_nPage += nCount;
		}
	}
	else
		m_nPage = pBridge->GetGirdersu();
	return m_nPage;
}

double CAPlateOutDetail::GetScale()
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	m_Scale = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_MAIN);
	return m_Scale;
}

CString CAPlateOutDetail::GetTitle()
{
	if(m_nPage==0)	return _T("");

 	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	long nPageSu = m_nPage;
	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("�� �� �� �� �� (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("�� �� �� �� ��");

	return szTitle;
}

void CAPlateOutDetail::AddPlan(CDomyun *pDom, long nSubPage, BOOL bUpper)
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	
	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	BOOL bDivType = FALSE;
	/* ..070202�� ���� �ٸ� �׸��� ���� �����ؾ� �ϹǷ� �ӽ÷� ���Ƴ���...070202.....KB...
	if(pDomOpt->m_Det_nDivPage == 2) bDivType = TRUE;//���κ� ������ �����̸�...
	*/

	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage);
	CPlateGirderApp	 *pGir   = pBxStt->GetGirder();	
 
	CAPlateDrawStandard  DrawStd     (pStd->GetDataManage());	
	CAPlateDrawPyung     DrawPyung   (pStd->GetDataManage());	
	CAPlateDrawHBracing  DrawHBracing(pStd->GetDataManage());	
	CAPlateDrawDimPyung  DrawDimPyung(pStd->GetDataManage());	
	CAPlateDrawDetailDim DetailDim   (pStd,&Dom,pBxStt, pBxEnd);
	// �Ŵ� �׸���
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE),pBxStt,pBxEnd);
	
	if(bUpper)	pOpt->SetEnvLType(&Dom,LT_DOT_LINE);
	else		pOpt->SetEnvLType(&Dom,LT_SOLID);
	if(pBx)
	{
		if(pBx != pBxStt)
		{
			DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBx, bUpper, 0, CAPlateDrawPyung::DRAW_SLOPTEXT, FALSE);			
			DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, bUpper, 0);			
		}
		while(Finder.GetBxFixNext())
		{					
			DrawPyung.DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), bUpper, 0, CAPlateDrawPyung::DRAW_SLOPTEXT, FALSE);	
			DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), bUpper, 0);			
			pBx = Finder.GetBxNext();
		}			
		if(pBx != pBxEnd)
		{
			DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, bUpper, 0, CAPlateDrawPyung::DRAW_SLOPTEXT, FALSE);	
			DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, bUpper, 0);					
		}	
	}
	else
	{
		DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBxEnd, bUpper, 0, CAPlateDrawPyung::DRAW_SLOPTEXT, FALSE);
		DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBxEnd, bUpper, 0);			
		pBx = Finder.GetBxNext();
	}

	// ����������
	if(bUpper)	pOpt->SetEnvType(&Dom, HCAD_STLH);
	else		pOpt->SetEnvType(&Dom, HCAD_STLC);

	DrawPyung.DrawVStiff(&Dom, pBxStt, pBxEnd);
	// ���������
	DrawPyung.DrawJackUpWithenBx(&Dom, pBxStt, pBxEnd);	
	// ����������
	DrawPyung.DrawJiJumStiffWithenBx(&Dom, pBxStt, pBxEnd, bUpper, TRUE);
	
	if(!bUpper) pOpt->SetEnvType(&Dom, HCAD_STLC);
	else		pOpt->SetEnvType(&Dom, HCAD_STLH);
	// ����극�̽� ������ - �����ǿ� ��ġ�ǹǷ� ǥ������ �ʴ´� 081205 APMS APlate2006_���� no.2667
//	DrawHBracing.DrawHGuessetBx(&Dom, pBxStt, pBxEnd);
	
	// ���κ� ����� �÷���
	DrawPyung.DrawCRBeamBothGussetWithenBx(&Dom, pBxStt, pBxEnd, bUpper, FALSE, FALSE, FALSE, 0, bDivType, TRUE, TRUE);
	DrawPyung.DrawCRBeamBothGussetWithenBx(&Dom, pBxStt, pBxEnd, bUpper, FALSE, FALSE, FALSE, 1, bDivType, TRUE, TRUE);
			  
	if(!bUpper) pOpt->SetEnvType(&Dom, HCAD_STLH);
	else		pOpt->SetEnvType(&Dom, HCAD_STLC);	
	// ���κ� ����� ������
	if(bUpper) DrawPyung.DrawCRBeamBothGassetWebWithenBx(&Dom, pBxStt, pBxEnd, 0, TRUE);
	// ����������
	DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd, 0, bUpper, FALSE);
	// ����������ũ
	if(bUpper)	DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_U, 2, TRUE);
	else		DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_L, 2, TRUE);
	// ����������ũ
	DrawPyung.DrawSpliceCut(&Dom, pBxStt, pBxEnd, 0, bUpper, TRUE);					
	// ġ����///////////////////////////////////////////////////////////////////////////////
	pOpt->SetEnvType(&Dom,HCAD_DIML);					
	DetailDim.SetPyungDimDan(3,TRUE);
	// ��ü ġ����
	BOOL bSttPage = pBxStt->IsState(BX_STT_GIRDER) ? TRUE : FALSE;
	BOOL bEndPage = pBxEnd->IsState(BX_END_GIRDER) ? TRUE : FALSE;

	DetailDim.DimTotal(&Dom,bUpper ? pDomOpt->m_DetailDim_Plan_Upper : pDomOpt->m_DetailDim_Plan_Lower,TRUE,TRUE,TRUE,FALSE, bSttPage, bEndPage);
	// ��ġ����
	DetailDim.DimDetailMLMarkPyung(&Dom, pStd->GetCurPage()-m_nSttPage-1, bUpper);
	// �������� ġ����
	DrawDimPyung.DrawDimSttEnd(&Dom, pBxStt, pBxEnd, bUpper);	
	// ���������� ����
	if(!bUpper)
	{
		DrawDimPyung.DrawJijumStiffDim(&Dom, pBxStt, pBxEnd, 1);	
		DrawDimPyung.DrawJackupStiffDim(&Dom, pBxStt, pBxEnd, 1);	
	}
	// ����, ������
	if(pDomOpt->m_Det_bPierAbutPyung && !pBridge->IsExtLine())
		DrawAddAbutPierPyung(&Dom,pBxStt,pBxEnd);
	if(pDomOpt->m_Det_bSttEndStationPyung)
	{
		DrawStationMark(&Dom, pBxStt, DetailDim.GetPyungCurUpperDimDan());
		DrawStationMark(&Dom, pBxEnd, DetailDim.GetPyungCurUpperDimDan());
	}
	// ���� ȸ������/////////////////////////////////////////////////////////////////////////
	double rotAng=0;
	rotAng = DrawPyung.GetRotateAnglePyung(pBxStt,pBxEnd);

	BOOL bGrindUp = pDomOpt->m_Det_UpperWeldbGrind;
	BOOL bGrindDn = pDomOpt->m_Det_LowerWeldbGrind;
	// �÷��� ����
	DrawFlangeJewon(&Dom, pBxStt, pBxEnd, rotAng, bUpper);
	// ���������� ����
	DrawJijumStiffJewon(&Dom, pBxStt, pBxEnd, -rotAng, bUpper);
	// ������ũ
	DrawPyung.AddWeldPyung		(&Dom, pBxStt, pBxEnd, -rotAng, bUpper, bUpper ? -1 : 1);
	DrawPyung.AddWeldPyungFlange(&Dom, pBxStt, pBxEnd, -rotAng, bUpper, bUpper ? 1 : -1, bUpper ? bGrindUp : bGrindDn);
	DrawPyung.AddWeldSplicePos	(&Dom, pBxStt, pBxEnd, -rotAng, bUpper, bUpper ? 1 : -1, bUpper ? bGrindUp : bGrindDn);
	DrawPyung.AddWeldVStiffPos	(&Dom, pBxStt, pBxEnd, -rotAng, bUpper, bUpper ? 1 : -1);

	CDPoint MoveXY = pGir->GetXyGirderDis(pBxStt->GetStation());		
	Dom.Move(-MoveXY);	
	Dom.Rotate(rotAng,0,0);	

	*pDom << Dom;
}

void CAPlateOutDetail::AddJong(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COptionStd		*pDomOpt	= pStd->m_pDomOptionStd;	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);		
	CGlobarOption   *pGlopt     = pStd->GetDataManage()->GetGlobalOption();

	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage);
	CPlateGirderApp	*pGir	= pBxStt->GetGirder();	

	pGir->SetEleLevel(pDomOpt->m_Det_bLevelJong);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CAPlateDrawJong DrawJong(pStd->GetDataManage());	
	CAPlateDrawDetailDim  DetailDim(pStd,&Dom,pBxStt, pBxEnd);
	CAPlateDrawDimJong  DimJong(pStd->GetDataManage());

	long nDir   = 0;
	long nLine  = pDomOpt->m_Gen_bBujeThick ? 2 : 0;
	CHAR cWeldType = 'A';

	DrawJong.DrawGirderLineBx (&Dom, pBxStt, pBxEnd, nLine);
	DrawJong.DrawFactJongBx   (&Dom, pBxStt, pBxEnd);
	DrawJong.DrawHStiffBx     (&Dom, pBxStt, pBxEnd, nLine, -1);	
	DrawJong.DrawHGuessetBx   (&Dom, pBxStt, pBxEnd);
	DrawJong.DrawCrossBeamBrkFlangeBx(&Dom, pBxStt, pBxEnd);
	DrawJong.DrawJackUpBx     (&Dom, pBxStt, pBxEnd, nLine, -1);
	DrawJong.DrawJiJumVStiffBx(&Dom, pBxStt, pBxEnd, nLine, -1);
	DrawJong.DrawVStiffBx     (&Dom, pBxStt, pBxEnd, nLine, -1);
	DrawJong.DrawInOutGateBx  (&Dom, pBxStt, pBxEnd, TRUE);
	DrawJong.DrawStudBx       (&Dom, pBxStt, pBxEnd, 0);
	// ���򺸰��� ����
	pOpt->SetEnvType(&Dom, HCAD_DIML);
	DrawJong.DrawHStiffBxJewon(&Dom, pBxStt, pBxEnd);

	// ���������� ����
	if(pDomOpt->m_Det_bVStiffJewon)
	{
		BOOL	bLeftRight = pDomOpt->m_Det_bVStiffLeftRight;
		// ���������� ��ġ��ġ�� ���� ���� ������ ���򺸰��� �����̴�.
		
		if(pBxStt->GetGirder()->m_nInstallVStiff == 0 || pBxStt->GetGirder()->m_nInstallVStiff == 2)	//����
			DrawJong.DrawVStiffBxJewon(&Dom, pBxStt, pBxEnd, 1, bLeftRight);
		else
			DrawJong.DrawVStiffBxJewon(&Dom, pBxStt, pBxEnd, 0);
	}

	// �ܸ鸶ũ
	pOpt->SetEnvType(&Dom, HCAD_MIS2);
	if(pDomOpt->m_Det_bSectionMarkJong)
	{
		pOpt->SetEnvType(&Dom, HCAD_SYMB);
		DetailDim.DimJongSectionMark(&Dom, pBxStt, pBxEnd, FALSE, 120, pDomOpt->m_bMarkAlwaysInCircle);
	}

	// �ܸ�
	if(pBridge->IsTUGir() && pDomOpt->m_Det_bGirderdan)
	{
		pOpt->SetEnvType(&Dom, HCAD_SYMB);
		DimJong.DimDanmyunCheckMark(&Dom, pBxStt, pBxEnd);
	}

	// ��������
	pOpt->SetEnvType(&Dom,HCAD_STLC);
	DrawJong.DrawFieldSpliceHatchJong(&Dom,pBxStt,pBxEnd,nDir,FALSE,TRUE,1,1,TRUE,pDomOpt->m_Det_nDivPage);	
	// ����ǥ��
	BOOL bGrindUp = pDomOpt->m_Det_UpperWeldbGrind;
	BOOL bGrindDn = pDomOpt->m_Det_LowerWeldbGrind;	
	DrawJong.AddWeldJong(&Dom,pBxStt,pBxEnd,0,nDir,	FALSE);	
	DrawJong.AddWeldJongFlange(&Dom,pBxStt,pBxEnd,0,nDir, cWeldType, bGrindUp, bGrindDn, pDomOpt->m_Det_bWeldDetail);		
	DrawJong.AddWeldJongCrossBeam(&Dom, pBxStt, pBxEnd, 0);		
	DrawJong.AddWeldSplicePos    (&Dom, pBxStt, pBxEnd, 0, cWeldType, bGrindUp, bGrindDn, pDomOpt->m_Det_bFieldWeldDetail);

	// ����,����,���÷���Ʈ
	if(pDomOpt->m_Det_bPierAbutJong || pDomOpt->m_Det_bSolePlateJong)
		DrawAbutPier(&Dom,pBxStt,pBxEnd);
	//
	DrawWebJewon(&Dom,pBxStt,pBxEnd,-1);
			
	BOOL bSttPage = pBxStt->IsState(BX_STT_GIRDER) ? TRUE : FALSE;
	BOOL bEndPage = pBxEnd->IsState(BX_END_GIRDER) ? TRUE : FALSE;

	DetailDim.SetDirDimension(nDir);
	
	DetailDim.SetJongDimDan(2,TRUE);
	DetailDim.DimTotal(&Dom,pDomOpt->m_DetailDim_Jong_Upper,TRUE,FALSE,FALSE,FALSE, bSttPage, bEndPage);

	DetailDim.SetJongDimDan(2,FALSE);
	DetailDim.DimTotal(&Dom,pDomOpt->m_DetailDim_Jong_Lower,FALSE,FALSE,FALSE,FALSE, bSttPage, bEndPage);

	DetailDim.DimJongSttEndSideBx(&Dom, pBxStt, pBxEnd);
	DetailDim.DimJongTaperBx     (&Dom, pBxStt, pBxEnd);
	// ��������
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER);
	if(pBx!=pBxStt)		DetailDim.DimDetailSttJong(&Dom,pBxStt,pBxEnd,FALSE,FALSE,nDir,FALSE);
	pBx = Finder.GetBxFirst(BX_END_GIRDER);
	if(pBx!=pBxEnd)		DetailDim.DimDetailEndJong(&Dom, pBxStt, pBxEnd,FALSE,FALSE,nDir);	
	//
	DetailDim.DimDetailMLMarkJong(&Dom,pStd->m_nCurPage - m_nSttPage - 1, nDir);	
	DetailDim.DimJongByundanFactor(&Dom, pBxStt, pBxEnd);

	pGir->SetEleLevel(FALSE);
	// ���� ȸ������
	CDPoint MoveXY;
	MoveXY.x = -pBxStt->GetStation();
	MoveXY.y = -pGir->GetElevationFlangeCamber(pBxStt->GetStation());
	Dom.Move(MoveXY);	
	*pDom << Dom;

}

void CAPlateOutDetail::AddDan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	CDataManage		*pDataManage= pStd->GetDataManage();
	CPlateBridgeApp *pDB		= pDataManage->GetBridge();
	CARoadOptionStd *pOpt		= pDataManage->GetOptionStd();
	COptionStd		*pDomOpt	= pStd->m_pDomOptionStd;	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);

	if(pDomOpt->m_Det_Scale.GetSize() == DET_SCALE_TU)
		pDomOpt->m_Det_Scale.Add(30);
	CDomyun	Dom(pDom);
	double dScale = pDomOpt->m_Det_Scale.GetAt(DET_SCALE_TU);

	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage);
	CPlateGirderApp	 *pGir	 = pBxStt->GetGirder();	

	CAPlateDrawStandard DrawStd(pDataManage);
	CAPlateDrawDimJong	DimJong(pDataManage);
	CAPlateDrawDanmyun	DrawDan(pDataManage);
//	CAPlateDrawCross	DrawCross(pDataManage);
	
	CDoubleArray dStaArr;
	DimJong.GetDanmyunCheckStation(pBxStt, pBxEnd, dStaArr);

	CString szTitle;
	char	sz		= 'A';
	double	dHGir	= 0;
	
	for(long n=0; n<dStaArr.GetSize(); n++)
	{
		pOpt->SetEnvType(&Dom, HCAD_STLC);
		Dom.SetScaleDim(dScale);
		
		CPlateBasicIndex *pBx = pGir->GetBxByStation(dStaArr.GetAt(n));
		DrawDan.DrawGirderOnBx(&Dom, pBx);
		DrawDan.DrawGirderBindConc(&Dom, pBx, FALSE);

		if(pBx->GetStation() == dStaArr.GetAt(n))	// �Ϲݺΰ� �ƴҶ�
		{
//			CPlateCrossBeam *pC = pBx->GetCrossBeam();
//			CPlateGirderApp	*pGirN	= pGir->m_pRight;
//			if(pBx->IsState(BX_CROSSBEAM_VBRACING) && pC && pGirN)			DrawCross.DrawTrimCRBeamOnBx(&Dom, pBx);
			if(pBx->IsState(BX_JIJUMSTIFF|BX_VSTIFF|BX_CROSSBEAM_VBRACING))	DrawDan.DrawVStiffOnBx(&Dom, pBx);
			if(pBx->IsState(BX_JACKUP))										DrawDan.DrawJackUpStiffOnBx(&Dom, pBx);
			if(pBx->IsState(BX_SPLICE))										DrawDan.DrawSpliceOnBx(&Dom, pBx);
			DrawDan.DimVStiffDet(&Dom, pBx, TRUE);
		}
		else
			DrawDan.DimVStiffDet(&Dom, pBx, FALSE);

		Dom.SetScaleDim(m_Scale);
		Dom.RedrawByScale(m_Scale/dScale);

		if(n==0)	dHGir = (pGir->GetHeightGirder(pBx, TRUE) + Dom.Always(50))*max((m_Scale/dScale),1);
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.bottom);
		Dom.Move((rect.Width()+Dom.Always(50))*(n%2) + Dom.Always(890), -Dom.Always(50) - dHGir*((long)n/2));		
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();

		szTitle.Format("�ܸ鵵 %c - %c", sz+n, sz+n);		

		// ���� ������ �����̵�
		Dom.Move(pMove->GetPoint(DAN + n*10));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), DAN + n*10, szTitle);	
		*pDom << Dom;

		Dom.SetScaleDim(m_Scale);
		DrawStd.AddTitleOnDom(&Dom, dScale, rect, szTitle, TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(DAN + n*10 + 1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(), DAN + n*10 + 1, szTitle);
		*pDom << Dom;
	}
}

CPlateBasicIndex* CAPlateOutDetail::GetBxSttPage(long nSubPage)
{
 	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	COptionStd		*pOpt  = pStd->m_pDomOptionStd;	
	CPlateBasicIndex		*pBxRet = NULL;

	long nType = pOpt->m_Det_nDivPage;
	long nCrossNum = pOpt->m_Det_nUnitEaDraw;
		
	if(nType==0)
	{
		long nG = nSubPage / pBridge->m_nQtyJigan;
		if(nSubPage%pBridge->m_nQtyJigan==0) nG-=1;
		long nPier = nSubPage - (pBridge->m_nQtyJigan*nG) - 1;
		if(nPier==0) 
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			return Finder.GetBxFirst(BX_STT_GIRDER);
		}
		return pBridge->GetGirder(nG)->GetBxOnJijum(nPier);		
	
	}
	else if(nType==1)
	{
		long nPage = 1;
		for(long nG=0;nG<pBridge->GetGirdersu();nG++)
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE | BX_STT_GIRDER);
			while(pBx)
			{
				if(nSubPage == nPage) return pBx;
				pBx = Finder.GetBxNext();
				nPage++;
			}
		}		
	}
	else if(nType==2)//���κ� ������ ���� pBxStt
	{
		long nAddNum = 0, nPreNum = 0;
		for(long nGir=0;nGir<pBridge->GetGirdersu();nGir++)
		{
			CPlateGirderApp	*pGir = pBridge->GetGirder(nGir);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING);
			
			long nCross = (long)Finder.GetCountBx()-2;
			long nCount = (long)RoundUp((nCross-1.0)/nCrossNum,0);

			nAddNum += nCount;
			if(nAddNum >= nSubPage)					
			{			
				if(nSubPage-1 == nPreNum)
					pBxRet = pBx;
				else
					pBxRet = Finder.GetBxNext(1 + nCrossNum*(nSubPage-1-nPreNum));
				
				if(pBxRet!=NULL) break;
			}
			nPreNum = nAddNum;			
		}
		return pBxRet;
	}
	else
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nSubPage-1));
		return Finder.GetBxFirst(BX_STT_GIRDER);
	}	

	return NULL;
}

CPlateBasicIndex* CAPlateOutDetail::GetBxEndPage(long nSubPage)
{
 	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	COptionStd		*pOpt  = pStd->m_pDomOptionStd;	
	CPlateBasicIndex		*pBxRet = NULL;

	long nType = pOpt->m_Det_nDivPage;
	long nCrossNum = pOpt->m_Det_nUnitEaDraw;
		
	if(nType == 0)
	{
		long nG = nSubPage / pBridge->m_nQtyJigan;
		if(nSubPage%pBridge->m_nQtyJigan==0) nG-=1;
		long nPier = nSubPage - (pBridge->m_nQtyJigan*nG);
		if(nPier==pBridge->m_nQtyJigan) 
		{
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			return Finder.GetBxFirst(BX_END_GIRDER);
		}
		return pBridge->GetGirder(nG)->GetBxOnJijum(nPier);		
	}
	else if(nType == 1)
	{
		long nPage = 1;
		for(long nG=0;nG<pBridge->GetGirdersu();nG++)
		{		
			CPlateBxFinder Finder(pBridge->GetGirder(nG));
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE | BX_END_GIRDER);
			while(pBx)
			{
				if(nSubPage == nPage) return pBx;
				pBx = Finder.GetBxNext();
				nPage++;
			}
		}
	}
	else if(nType == 2) //���κ� ������ ���� pBxEnd(LSH_ZZ_REVIEW)
	{
		long nTotalCount	  = 0;
		long nGir			  = 0;
		long nDivCount		  = 0;
		long nPreNum		  = 0;
		CPlateGirderApp	*pGir = NULL;

		//Girder Information Setting
		for(nGir = 0; nGir < pBridge->GetGirdersu(); nGir++)
		{
			 pGir = pBridge->GetGirder(nGir);
			 CPlateBxFinder		Finder(pGir);
			 CPlateBasicIndex	*pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			 			
			 long nCrossQty  = (long)Finder.GetCountBx();
			 
			 nDivCount		 = (long)RoundUp((nCrossQty - 1.0) / nCrossNum, 0);
			 nTotalCount	 += nDivCount;

			 if(nTotalCount >= nSubPage) break;
			 
			 nPreNum += nDivCount;
		}

		//End BX Setting
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING);
		
		long nBXCount = (nSubPage-nPreNum) % nDivCount;
		
		if(nBXCount == 0 )	nBXCount = nDivCount;

		pBxRet = Finder.GetBxNext(nBXCount * nCrossNum + 1);

		//Exception
		if(!pBxRet)
		{
			CPlateBxFinder Finder(pGir);	
			pBxRet = Finder.GetBxLast(BX_END_GIRDER);
		}
		// #APLATE-662, �� �ڿ� Bx�� �����ִµ��� ī��Ʈ�� �ȵȴٸ� Bx�� �� ã�´�.
		else if(Finder.GetBxFixNext() && Finder.GetBxFixNext()->IsState(BX_END_GIRDER))
			pBxRet = Finder.GetBxNext();
		//
		
		//End Jijum Setting
		if(pBridge->GetQtyGirder() - 1 == nGir && nSubPage ==  nTotalCount)
		{
			CPlateBxFinder Finder(pGir);	
			pBxRet = Finder.GetBxFirst(BX_END_GIRDER);
		}
		
		return pBxRet;
	}	
	else
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nSubPage-1));
		return Finder.GetBxFirst(BX_END_GIRDER);
	}	
	return NULL;
}


void CAPlateOutDetail::DrawEtcSymbolPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double Angle, BOOL bUpper)
{	
	CAPlateDrawStd  *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir = pBxStt->GetGirder();	
	CARoadOptionStd *pOpt = pStd->GetDataManage()->GetOptionStd();

	CAPlateSymbolDom SymDom(pDom,  pStd->GetDataManage());	
	
	double CenSta = (pBxEnd->GetStation() + pBxStt->GetStation() ) / 2;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx;
	double xlen;
	double Len;
	CDPoint xy, xy1;
		
	if(bUpper)
	{
		pBx = Finder.GetBxFirst(BX_UPPERFLANGE,pBxStt,pBxEnd);
		while(pBx)
		{		
//				EXIT_RTN;
			Len = pBx->GetStation(); //pBxStt->GetStation() + GetDisWebBySta(pBxStt->GetNumberGirder(),pBxStt->GetStation(),pBx->GetStation(),nSide);				
			xy = pGir->GetXyGirderDis(Len);
			if(Len > CenSta) xlen = 140;
			else			 xlen = -140;
			xy1 = pGir->GetXyGirderDis(Len+xlen,300-140);				
			pOpt->SetEnvType(&SymDom,HCAD_SYMB,LT_DOT_LINE);
			SymDom.Circle(xy.x,xy.y,200);
			pOpt->SetEnvType(&SymDom,HCAD_DIML);
			CString strFactCode = "TET";//pBridge->GetMarkWeld(WELD_FU,pBx);			
				
			CAPlateSymbolDom SubSymDom(&SymDom, pStd->GetDataManage());
			if(Len > CenSta)
				SubSymDom.GiSiArrowExt(xy1.x,xy1.y,SubSymDom.Always(15),SubSymDom.GetTextWidth(strFactCode),1,strFactCode);				
				//SymDom.DrawSymbolCircleMark(xy1.x,xy1.y,SymDom.Always(15),strFactCode,"-","",TRUE,FALSE,-Angle+ConstPi/2);				
			else
				SubSymDom.GiSiArrowExt(xy1.x,xy1.y,SubSymDom.Always(15),-SubSymDom.GetTextWidth(strFactCode),3,strFactCode);				
				//SymDom.DrawSymbolCircleMark(xy1.x,xy1.y,SymDom.Always(15),strFactCode,"-","",TRUE,TRUE,-Angle+ConstPi/2);				
			SubSymDom.Rotate(-Angle,xy1.x,xy1.y);
			SymDom << SubSymDom;
			pBx = Finder.GetBxNext();
		}
	}
	else
	{
		pBx = Finder.GetBxFirst(BX_LOWERFLANGE,pBxStt,pBxEnd);
			
		while(pBx)		
		{
//				EXIT_RTN;
			Len = pBx->GetStation();
			xy = pGir->GetXyGirderDis(Len,300);
			if(Len > CenSta) xlen = 140;
			else			 xlen = -140;
			xy1 = pGir->GetXyGirderDis(Len+xlen,300-140);
			pOpt->SetEnvType(&SymDom,HCAD_SYMB,LT_DOT_LINE);
			SymDom.Circle(xy.x,xy.y,200);
			pOpt->SetEnvType(&SymDom,HCAD_DIML);
			CString strFactCode = "TET";//pBridge->GetMarkWeld(WELD_FL,pBx);			
							
			CAPlateSymbolDom SubSymDom(&SymDom, pStd->GetDataManage());
			if(Len > CenSta)
				SubSymDom.GiSiArrowExt(xy1.x,xy1.y,SubSymDom.Always(15),SubSymDom.GetTextWidth(strFactCode),1,strFactCode);				
				//SymDom.DrawSymbolCircleMark(xy1.x,xy1.y,SymDom.Always(15),strFactCode,"-","",TRUE,FALSE,-Angle+ConstPi/2);				
			else		
				SubSymDom.GiSiArrowExt(xy1.x,xy1.y,SubSymDom.Always(15),-SubSymDom.GetTextWidth(strFactCode),3,strFactCode);				
				//SymDom.DrawSymbolCircleMark(xy1.x,xy1.y,SymDom.Always(15),strFactCode,"-","",TRUE,TRUE,-Angle+ConstPi/2);				
			SubSymDom.Rotate(-Angle,xy1.x,xy1.y);
			SymDom << SubSymDom;
			pBx = Finder.GetBxNext();
		}		
	}
	*pDom << SymDom;
}

CDPoint CAPlateOutDetail::DrawSolPlate(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CPlateGirderApp      *pGir		= pBxStt->GetGirder();	
	COutMovePoint   *pMove		= pStd->GetMovePoint(pStd->m_nCurPage);		
	CPlateBxFinder		Finder(pGir);
	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());


	double dScale = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_SOLEPLATE);
	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	BOOL bDraw = FALSE;
	while(pBx)
	{
		//if (m_MaxSebuDetailSu >= nMaxSeub && nMaxSeub!=0)
		//	return CDPoint(xDis, yDis);

		if(pBx->IsJijum())
		{
			DrawSebu.AddSolePlateDetail(&Dom, pBx, TRUE);						
//			pStd->AddTitleOnDom(&Dom,"���÷���Ʈ ��");
			Dom.RedrawByScale(m_Scale/dScale);									
			bDraw = TRUE;
			m_MaxSebuDetailSu++;
			break;
		}
		pBx = Finder.GetBxNext();
	}
	if(!bDraw) return CDPoint(xDis, yDis);
		
	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();	
	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); // ��Ȯ�� ���� ������ ���Ͽ� �߰� - ����ȭ	
	Dom.Move(xDis, yDis);
	Dom.SetCalcExtRect();	
	rect = Dom.GetExtRect();	
	// ���� ������ ���� �̵�
	Dom.Move(pMove->GetPoint(SOLEPLATE));		
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),SOLEPLATE,"���¹�ħ ��");	
	*pDom << Dom;

	// TITLE
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawStd.AddTitleOnDom(&Dom, dScale, rect, "���¹�ħ ��",TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(SOLEPLATE+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),SOLEPLATE+1,"���¹�ħ �� ����");
	*pDom << Dom;
		
	return CDPoint(xDis, yDis - rect.Height() - pDom->Always(40));
}

void CAPlateOutDetail::AddBeomye(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();	

	CGridDomyunEx Dom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetRowCount(8);
	Dom.SetColumnCount(2);

	Dom.SetRowHeightAll(Dom.GetTextHeight()*2);
	Dom.SetRowHeight(0,400);
	Dom.SetRowHeight(7,50);
	Dom.SetColumnWidth(0,Dom.Always(10));
	Dom.SetColumnWidth(1,Dom.Always(33));

	Dom.SetTextMatrix(1,0,"M.L");
	Dom.SetTextMatrix(1,1,": Match Line",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	CString str = "TF";
	Dom.SetTextMatrix(2,0,str);
	Dom.SetTextMatrix(2,1,": ��������(����)",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	str = "WF";
	Dom.SetTextMatrix(3,0,str);
	Dom.SetTextMatrix(3,1,": ��������(����)",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);

	str = "BF";
	Dom.SetTextMatrix(4,0,str);
	Dom.SetTextMatrix(4,1,": ��������(����)",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);

	str = "SP";
	Dom.SetTextMatrix(5,0,str);
	Dom.SetTextMatrix(5,1,": ��������",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	str = "FW";
	Dom.SetTextMatrix(6,0,str);
	Dom.SetTextMatrix(6,1,": �������",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	CDRect rect = Dom.GetRect();	
	CString szText = "�� ��";
	double dTextLen = Dom.GetTextWidth(szText);	
	double dTextHeight = Dom.GetTextHeight();

	pOpt->SetEnvType(&Dom,HCAD_TBL2);
	Dom.TextCircle(rect.CenterPoint().x, rect.TopLeft().y,szText,0);

	Dom.MoveTo(rect.TopLeft());
	Dom.LineTo(rect.CenterPoint().x-dTextLen/2-dTextHeight*1.3,rect.TopLeft().y);
	Dom.MoveTo(rect.TopRight());
	Dom.LineTo(rect.CenterPoint().x+dTextLen/2+dTextHeight*1.3,rect.TopRight().y);

	Dom.LineTo(rect.TopLeft(),rect.BottomLeft());
	Dom.LineTo(rect.BottomLeft(),rect.BottomRight());
	Dom.LineTo(rect.TopRight(),rect.BottomRight());

	Dom.SetGridLines(GVL_NONE);
	Dom.Draw();

	*pDom << Dom;
}

CDPoint CAPlateOutDetail::DrawWeldDetail(CDomyun *pDomP, double dX, double dY, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpDown)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();			
	CPlateGirderApp	*pGir		= pBxStt->GetGirder();		
	CPlateBxFinder	Finder(pGir);

	long nHBase =  pStd->m_pDataManage->GetGlobalOption()->GetDesignHeigtBase();
	
	long nMarkBuje = 0;
	double dXDis = 0;
	double dYDis = 0;
	long nUpDown2 = 0;

	__int64 BxFlag;
	if(nUpDown==0)			
	{
		BxFlag = BX_UPPERFLANGE;
		nMarkBuje = G_F_U;
		nUpDown2 = 1;
		if (nHBase == 1 || nHBase == 3) nUpDown2 = -1;
	}
	else if(nUpDown==1)	
	{
		BxFlag = BX_LOWERFLANGE;	
		nMarkBuje = G_F_L;
		nUpDown2 = -1;
		if (nHBase == 1 || nHBase == 2) nUpDown2 = 1;
	}
	else if(nUpDown==2)		
	{
		BxFlag = BX_WEB;	
		nMarkBuje = G_W;
		nUpDown2 = 0;
	}
	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	if(pBx==NULL) return CDPoint(dX + dXDis,dY + dYDis);

	long nIdxWeld = pBridge->GetIndexBaseWeld(nMarkBuje,nMarkBuje,pBx);
	CSteelWeldData *pWeld = pStd->GetDataManage()->GetSteelWeld()->GetBaseWeldByIndex(nIdxWeld);
		
	double T1=0, T2=0;
	CDPoint rePoint;

	long nUpDn = nUpDown2;

	while(pBx)
	{
		if(!pBx->GetFactChain(nMarkBuje, TRUE) || !pBx->GetFactChain(nMarkBuje, FALSE)) break;

		T1 = pBx->GetFactChain(nMarkBuje,FALSE)->m_dFactChainThick;
		T2 = pBx->GetFactChain(nMarkBuje,TRUE)->m_dFactChainThick;
		
		if(/*(W1 != W2 || T1 != T2) && */!pBx->IsState(BX_SPLICE))
		{
			if(T1 == T2)
			{
				if(nMarkBuje == G_F_U)
					nUpDn = 1;
				else if (nMarkBuje == G_F_L)
					nUpDn = -1;
				else
					nUpDn = 0;
			}
			else
				nUpDn = nUpDown2;

			rePoint = DrawingWeldDetail(pDomP, dX, dY, pWeld, nUpDn, T1, T2, nMarkBuje==G_W?TRUE:FALSE);
			dX = rePoint.x;
			dY = rePoint.y;		
		}
		pBx = Finder.GetBxNext();
	}						
	return CDPoint(rePoint.x,rePoint.y);
}

// ������ �󼼵� ���������� ����÷���A, �Ϻ��÷���B, ������C�� �����Ͽ� m_cWeldTypeName �� 'C' �̸� ���������� �Ǵ��Ͽ�����
// �� ��� ���������� ��/�� ���������� 2���̻� �߻��� �����󼼿� ���鵵�� ����ġ������ �߻��ϹǷ� bWeb �ɼ��� �߰��� 2010-01-04 lina
CDPoint CAPlateOutDetail::DrawingWeldDetail(CDomyun *pDomP, double dX, double dY, CSteelWeldData *pWeld, long nUpDown2, double T1, double T2, BOOL bWeb)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd	*pOpt		= pStd->GetDataManage()->GetOptionStd();		
	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);					
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CDomyun p(pDomP);

	pOpt->SetEnvType(&p,HCAD_SMALL_TITLE);

	double dScale = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_WELD);	
	long nAddNum = 0;
	double dXDis = 0;
	double dYDis = 0;
	CDRect rect;
	
//	m_cWeldTypeName++;

	p.SetScaleDim(dScale);
	if(pWeld->m_nImproveType==3)
		if(!bWeb/*m_cWeldTypeName != 'C'*/)
			DrawSebu.DrawFactDetail_VType(&p,T1,T2, dScale, nUpDown2, 2,2,60,60,FALSE,FALSE);
		else	
			DrawSebu.DrawFactDetail_VType_CType(&p, T1, T2, dScale, nUpDown2, 2,2, 60, FALSE, FALSE, FALSE);
	else if(pWeld->m_nImproveType==4)
		if(!bWeb/*m_cWeldTypeName != 'C'*/)
			DrawSebu.DrawFactDetail_XType(&p,T1,T2, dScale, nUpDown2, 2,2,6,60,60,FALSE,FALSE);
		else
			DrawSebu.DrawFactDetail_XType_CType(&p,T1,T2, dScale, nUpDown2, 2,2,6,60,60,FALSE,FALSE);
	else
		DrawSebu.AddFactDetail(&p,T1,T2, dScale, nUpDown2, 2,2,6,60,60,FALSE,FALSE, bWeb);

	p.RedrawByScale(m_Scale/dScale);
	
	// ���� ������ �̵�
	p.SetCalcExtRect();
	rect = p.GetExtRect();
	p.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); // ��Ȯ�� ���� ������ ���Ͽ� �߰� - ����ȭ
	p.Move(dX+dXDis,dY+dYDis);								
	// ���� ������ ���� �̵�
	p.SetCalcExtRect();
	rect = p.GetExtRect();

	p.Move(pMove->GetPoint(m_WeldDetailMoveNum));		
	
	// ���� ���� ���							
	CString Name;
	Name.Format("�� �� �� �� \"%c\"", m_cWeldTypeName);			
	pStd->ChangeSeperateLayer(&p); // HCAD ���� ���� �۾�
	pMove->AddMPE(&p,p.GetExtRect(),m_WeldDetailMoveNum,Name);						
	nAddNum++;
	dYDis -= rect.Height() + pDomP->Always(30);
	*pDomP << p;

		// TITLE
	p.SetScaleDim(pDomP->GetScaleDim());
	DrawStd.AddTitleOnDom(&p, dScale, rect, Name,TRUE, TRUE, 0);
	p.Move(pMove->GetPoint(m_WeldDetailMoveNum+300));
	p.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&p); // HCAD ���� ���� �۾�
	pMove->AddMPE(&p,p.GetExtRect(),m_WeldDetailMoveNum+300,Name + " ����");
	
	*pDomP << p;

	m_cWeldTypeName++;
	m_WeldDetailMoveNum++;
	m_MaxSebuDetailSu++;

	return CDPoint(dX + dXDis,dY + dYDis);
}

//�������� �����ϰ�� ���� �󼼵� �׸���...
void CAPlateOutDetail::DrawFieldWeldDetail(CDomyun *pDom, double dX, double dY, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir		= pBxStt->GetGirder();	
	CPlateBxFinder	Finder(pGir);
	

	double dT1 = 0, dT2 = 0;
	long nMarkBuje = 0;
	long nWeldMarkBuje = 0;	
	CDPoint rePoint;

	//����...
	nMarkBuje = G_W;
	nWeldMarkBuje = WELD_FIELDWEB;	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	long nIdxWeld = pBridge->GetIndexBaseWeld(nWeldMarkBuje, nWeldMarkBuje, pBx);
	CSteelWeldData *pWeld = pStd->GetDataManage()->GetSteelWeld()->GetBaseWeldByIndex(nIdxWeld);

	
	while(pBx)
	{		
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)	
		{
			pBx = Finder.GetBxNext();
			continue;
		}

		if(!pBx->GetFactChain(nMarkBuje, TRUE) || !pBx->GetFactChain(nMarkBuje, FALSE)) break;		

		dT1	= pBx->GetFactChain(nMarkBuje, TRUE)->m_dFactChainThick;
		dT2	= pBx->GetFactChain(nMarkBuje, FALSE)->m_dFactChainThick;

		rePoint = DrawingWeldDetail(pDom, dX, dY, pWeld, 0, dT1, dT2, TRUE);
		dX = rePoint.x;
		dY = rePoint.y;		

		pBx = Finder.GetBxNext();
	}

	// ���
	nMarkBuje = G_F_U;
	nWeldMarkBuje = WELD_FIELDU;

	pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);	
	nIdxWeld = pBridge->GetIndexBaseWeld(nWeldMarkBuje, nWeldMarkBuje, pBx);
	pWeld = pStd->GetDataManage()->GetSteelWeld()->GetBaseWeldByIndex(nIdxWeld);

	while(pBx)
	{		
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)
		{
			pBx = Finder.GetBxNext();	continue;
		}

		if(!pBx->GetFactChain(nMarkBuje, TRUE) || !pBx->GetFactChain(nMarkBuje, FALSE)) break;

		dT1   = pBx->GetFactChain(nMarkBuje, TRUE)->m_dFactChainThick;
		dT2   = pBx->GetFactChain(nMarkBuje, FALSE)->m_dFactChainThick;
		
		rePoint = DrawingWeldDetail(pDom, dX, dY, pWeld, 1, dT1, dT2);
		dX = rePoint.x;
		dY = rePoint.y;			
		
		pBx = Finder.GetBxNext();
	}

	//�Ϻ�
	nMarkBuje = G_F_L;
	nWeldMarkBuje = WELD_FIELDL;

	pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);	
	nIdxWeld = pBridge->GetIndexBaseWeld(nWeldMarkBuje, nWeldMarkBuje, pBx);
	pWeld = pStd->GetDataManage()->GetSteelWeld()->GetBaseWeldByIndex(nIdxWeld);

	while(pBx)
	{		
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)
		{
			pBx = Finder.GetBxNext();	continue;
		}

		if(!pBx->GetFactChain(nMarkBuje, TRUE) || !pBx->GetFactChain(nMarkBuje, FALSE)) break;
		dT1	= pBx->GetFactChain(nMarkBuje, TRUE)->m_dFactChainThick;
		dT2	= pBx->GetFactChain(nMarkBuje, FALSE)->m_dFactChainThick;

		rePoint = DrawingWeldDetail(pDom, dX, dY, pWeld, 1, dT1, dT2);
		dX = rePoint.x;
		dY = rePoint.y;		
		
		pBx = Finder.GetBxNext();
	}			
}

CDPoint CAPlateOutDetail::DrawVStiffDetail(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp		*pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd		*pOpt		= pStd->GetDataManage()->GetOptionStd();		
	CPlateGirderApp		*pGir		= pBxStt->GetGirder();	
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd			*pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawSebu		DrawSebu(pStd->GetDataManage());
	CAPlateDrawStandard	DrawStd(pStd->GetDataManage());
	CAPlateDrawDanmyun	DrawDan(pStd->GetDataManage());
	CPlateBxFinder		Finder(pGir);

	long nMaxSeub = pStd->m_pDomOptionStd->m_Det_MaxSebuDetailSu;
	double dScale = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_VSITFF);
	BOOL bDraw = FALSE;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_VSTIFF,pBxStt,pBxEnd);
	
	if (m_MaxSebuDetailSu >= nMaxSeub && nMaxSeub!=0)
		return CDPoint(xDis, yDis);
	
	if(pBx)
	{
		if(!pDomOpt->m_Det_bVStiffAll)
		{
			DrawSebu.AddVStiffDetail(&Dom,  pBx, 0, 0, dScale);
		}
		else
		{
			DrawDan.DrawGirderOnBx(&Dom, pBx);
			DrawDan.DrawVStiffOnBx(&Dom, pBx);
			DrawDan.DimVStiffOnBx(&Dom, pBx, FALSE);
		}
		Dom.RedrawByScale(m_Scale/dScale);
		bDraw = TRUE;
		m_MaxSebuDetailSu++;		
	}

	if(!bDraw) return CDPoint(xDis, yDis);
			
	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();
	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); // ��Ȯ�� ���� ������ ���Ͽ� �߰� - ����ȭ	
	Dom.Move(xDis,yDis);
	Dom.SetCalcExtRect();	
	rect = Dom.GetExtRect();

	// ���� ������ ���� �̵�
	Dom.Move(pMove->GetPoint(140));
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),140,"���� ������ ������");	
	*pDom << Dom;

	// TITLE
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawStd.AddTitleOnDom(&Dom, dScale, rect, "���� ������ ������",TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(150+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),150+1,"���� ������ ������ ����");
	*pDom << Dom;

	return CDPoint(xDis, yDis - rect.Height() - Dom.Always(40));
}

CDPoint CAPlateOutDetail::DrawHStiffDetailByRib(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();		
	CPlateGirderApp		*pGir		= pBxStt->GetGirder();	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CPlateBxFinder		Finder(pGir);

	long nMaxSeub = pStd->m_pDomOptionStd->m_Det_MaxSebuDetailSu;
	double dScale = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_HSTIFF2);
	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF,pBxStt,pBxEnd);
	while(pBx) 
	{
		if (m_MaxSebuDetailSu >= nMaxSeub && nMaxSeub !=0 )
			return CDPoint(xDis, yDis);

		/*
		if(pBx->IsMatchHStiffAndTRib(0))
		{
			CWebHStiff *pHs = pBx->GetWebHStiff(-1);		
			BOOL bFind = FALSE;
			for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
			{
				if(pHs->IsEmpty(nDan))	continue;			
				bFind = TRUE;
			}		
			if(!bFind) 
			{
				pBx = Finder.GetBxNext();
				continue;
			}
		
			DrawSebu.AddHStiffDetail_WithHRib(&Dom, pBx, -1, dScale);					
//			pStd->AddTitleOnDom(&Dom,"���� ������ ������");
			m_MaxSebuDetailSu++;
			break;
		}
		*/
		pBx = Finder.GetBxNext();
	}

	if(!pBx) return CDPoint(xDis, yDis);
	Dom.RedrawByScale(m_Scale/dScale);
		
	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();		
	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); // ��Ȯ�� ���� ������ ���Ͽ� �߰� - ����ȭ	
	Dom.Move(xDis,yDis);

	Dom.SetCalcExtRect();	
	rect = Dom.GetExtRect();		
	// ���� ������ ���� �̵�
	Dom.Move(pMove->GetPoint(HSTIFFDET1));
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),HSTIFFDET1,"���� ������ ������");
	*pDom << Dom;

	// TITLE
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawStd.AddTitleOnDom(&Dom, dScale, rect, "���� ������ ������",TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(HSTIFFDET1+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),HSTIFFDET1+1,"���� ������ ������ ����");
	*pDom << Dom;

	return CDPoint(xDis, yDis - rect.Height() - Dom.Always(30));
}

CDPoint CAPlateOutDetail::DrawHStiffDetail(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();		
	CPlateGirderApp		*pGir		= pBxStt->GetGirder();	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CPlateBxFinder		Finder(pGir);

	long nMaxSeub = pStd->m_pDomOptionStd->m_Det_MaxSebuDetailSu;
	double dScale = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_HSTIFF1);
	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF,pBxStt,pBxEnd);
	while(pBx) 
	{
		if (m_MaxSebuDetailSu >= nMaxSeub && nMaxSeub!=0)
			return CDPoint(xDis, yDis);

		DrawSebu.AddHStiffDetail_WithVStiff(&Dom, pBx, dScale,FALSE);					
//		pStd->AddTitleOnDom(&Dom,"���� ������ ������");
		m_MaxSebuDetailSu++;
		break;
	}
	if(!pBx) CDPoint(xDis, yDis);

	Dom.RedrawByScale(m_Scale/dScale);
	
	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();
	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); // ��Ȯ�� ���� ������ ���Ͽ� �߰� - ����ȭ	
	Dom.Move(xDis,yDis);

	Dom.SetCalcExtRect();	
	rect = Dom.GetExtRect();
	// ���� ������ ���� �̵�
	Dom.Move(pMove->GetPoint(HSTIFFDET2));
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),HSTIFFDET2,"���� ������ ������");
	*pDom << Dom;

	// TITLE
	Dom.SetScaleDim(pDom->GetScaleDim());
	DrawStd.AddTitleOnDom(&Dom, dScale, rect, "���� ������ ������",TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(HSTIFFDET2+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),HSTIFFDET2+1,"���� ������ ������ ����");
	*pDom << Dom;

	return CDPoint(xDis, yDis - rect.Height() - Dom.Always(30));
}

void CAPlateOutDetail::AddKeyPan(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint*	pMove		= pStd->GetMovePoint(pStd->GetCurPage());	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CDomyun Dom(pDom);	
	
	double dKeyScaleHor = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_SUMARY_HOR);
	double dKeyScaleVer = pStd->m_pDomOptionStd->m_Det_Scale.GetAt(DET_SCALE_SUMARY_VER);

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	// �������
	CString sBridgeDir	= pDomOpt->m_sTextBridgeDirection;
	CString sCounterDir = pDomOpt->m_sTextCounterDirection;
	BOOL bDirection		= pDomOpt->m_Det_bKeyPlanDirection;	
	BOOL bOneLine       = pDomOpt->m_Det_bKeyPlanOneLine;	
	BOOL bSpliceMArk    = pDomOpt->m_Det_bKeyPlanSpliceMark;	
	BOOL bJijumMark     = pDomOpt->m_Det_bKeyPlanJijumMark;	
	BOOL bDimesion      = pDomOpt->m_Det_bKeyPlanDimension;	
	BOOL bCross         = pDomOpt->m_Det_bKeyPlanCrossBeam;	

	Dom.SetScaleDim(dKeyScaleHor);

	// KEY PLAN �׸���
	DrawPyung.AddKeyPlan(&Dom,pBxStt,pBxEnd,dKeyScaleHor,FALSE, bSpliceMArk,bJijumMark, bDirection,FALSE,bCross,bOneLine,-1,-1,FALSE,FALSE,sBridgeDir,sCounterDir);	

	// ġ����	
	if(bDimesion)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
		CPlateBxFinder Finder(pBridge->GetGirder(-1));
		//
		__int64 nTypeUpper  = DIM_BASE_SPAN|DIM_BASE_SPLICE;
		long  nSttDan = 1;
		DrawDimPyung.DimPyungTotal(&Dom,Finder.GetBxFirst(BX_STT_GIRDER),
			 Finder.GetBxLast(BX_END_GIRDER),nTypeUpper,FALSE,FALSE,"",nSttDan);
	}

	// ���� �̵� �� ȸ��, ����������
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER);
	CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();

	ASSERT( pBx || pBxNext);
	double rotAng = DrawPyung.GetRotateAnglePyung(pBx,pBxNext);
	CDPoint MoveXY = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	Dom.Move(-MoveXY);	// �����߽� (0,0)
	Dom.Rotate(rotAng,0,0);	

	BOOL bRotate = 	FALSE;//pOption->m_Det_bKeyPlanRoVer;
	if (bRotate)
		Dom.Rotate(ConstPi/2, 0,0);

	Dom.SetCalcExtRect();	
	CDRect rectOutLine = Dom.GetExtRect();
/*	//if (pOption->m_KeyPlanbOutLine)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
		rectOutLine.NormalizeRect();
		rectOutLine.left -= Dom.Always(10);
		//if (pOption->m_Det_bKeyPlanRoVer)
		//	rectOutLine.left -= Dom.Always(40);
		rectOutLine.top -= Dom.Always(10);
		rectOutLine.right += Dom.Always(10);
		//if (pOption->m_Det_bKeyPlanRoVer)
		//	rectOutLine.bottom += Dom.Always(10);
		//else
			rectOutLine.bottom += Dom.Always(30);
		Dom.Rectangle(rectOutLine);
	}
*/
	Dom.RedrawByHVScale(dKeyScaleHor/dKeyScaleVer,FALSE,TRUE);
	Dom.RedrawByScale(m_Scale/dKeyScaleHor);	
	
	*pDom << Dom;
}

void CAPlateOutDetail::InitNote()
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CGlobarOption	*pOpt	= pStd->GetDataManage()->GetGlobalOption();

	CString szText = _T("");

	m_sNoteArr.RemoveAll();
	m_sNoteArr.Add(".GIRDER ���۽� ��鼱������ ���ܰ�縦");
	m_sNoteArr.Add("�����Ͽ� ����.");
	m_sNoteArr.Add(".������ �����Ŀ� ������� �ǰų� �ܺο� ����");
	m_sNoteArr.Add("���� �Ǵ� ���ܸ��� ���� �ݰ� 2mm �̻���");
	m_sNoteArr.Add("�ձ������� �𼭸� ���⸦ �ϰ� ��Ÿ ������");
	m_sNoteArr.Add("�ݰ� 0.5mm �̻��� �ձٸ����� ���ų� �Ǵ�");
	m_sNoteArr.Add("0.5 ������ ����⸦ �Ѵ�.");
	m_sNoteArr.Add(".STUD�� ������ ����(PRESS WELDING)��");
	m_sNoteArr.Add("����� ��.");
	m_sNoteArr.Add(".STUD BOL�� �÷����� ���������δ� �����ð�");
	m_sNoteArr.Add("�� ���� �ܷ����� ���Ҹ� ���Ͽ� �����ηκ���");
	m_sNoteArr.Add("50mm �̻�, TAPER �ܺη� ���� 20mm �̻�");
	m_sNoteArr.Add("�̰ݽ�Ų��.");
	m_sNoteArr.Add(".���������ο� ��ġ�Ǵ� STUD BOLT�� ��������");
	m_sNoteArr.Add("�κ��� 50mm �̰ݽ��� ��ġ��.");
	m_sNoteArr.Add(".������ �����ϴ� �κп��� �ֺ����� �������Ӽ�");
	m_sNoteArr.Add("�� Ȯ���ϰ� �������� ���������� ���ϱ� ���Ͽ�");
	m_sNoteArr.Add("SCALLOP ������ �Ѵ�.");
	m_sNoteArr.Add(".��� �����ο� ���ؼ��� �ݵ�� �˻����� ����");
	m_sNoteArr.Add("�� �հݱ��ؿ� �ǰ� ��缱 ��������(KS B 08");
	m_sNoteArr.Add("45)�� ������ �˻����� ������ �� �� �����");
	m_sNoteArr.Add("���������� �����Ͽ��� �Ѵ�.");
	m_sNoteArr.Add(".�ܸ��� ���� �ٸ� �ֿ������ �´�� ������");
	m_sNoteArr.Add("�־�� �β� �� ���� ������ ��ȭ���� ����");
	m_sNoteArr.Add("������ ��簡 1:5 ���ϰ� �ǵ��� �Ѵ�.");
	szText.Format(".�´�� �������� ���� ���� ���� �β��� %.0fmm", pOpt->GetDesignThickXWeldApply());
	m_sNoteArr.Add(szText);
	szText.Format("�� �ʰ��� ������ X�� �������� �ϰ�, %.0fmm", pOpt->GetDesignThickXWeldApply());
	m_sNoteArr.Add(szText);
	m_sNoteArr.Add("������ ������ V�� �������� �Ѵ�.");
	m_sNoteArr.Add(".FILLET ���� �β��� ������ ǥ������ �ʴ� ��");
	m_sNoteArr.Add("6mm �� �Ѵ�.");
	m_sNoteArr.Add(".���򺸰��� �������� �ڵ������� ����Ͽ� 35mm,");
	m_sNoteArr.Add("����������� ���� �� ���� �÷����� ����.");
	m_sNoteArr.Add(".�����ǰ� ���򺸰����� �������� 20mm�� ǥ��");
	m_sNoteArr.Add("���� �Ѵ�.");
	m_sNoteArr.Add(".��Ʈ�� ü������� �߾ӿ��� �ܺη� ���ϰ� 2ȸ");
	m_sNoteArr.Add("���̱⸦ ��Ģ���� �Ѵ�.");
	m_sNoteArr.Add(".������ ������, JACK UP�� ������� ū ����°�");
	m_sNoteArr.Add("�������ܷ��� �߻��ϹǷ� ����� (chipping, 15C)");
	m_sNoteArr.Add("�Ͽ� �������� ä���.");	
	m_sNoteArr.Add(".()���� ġ���� �ݴ��� ���� ġ���̴�.");
}

void CAPlateOutDetail::AddNote(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);	

	CDomyun Dom(pDomP);

	BOOL bVerticalNote = pOpt->m_Det_bBasicNoteVert;
	if(bVerticalNote)
		DimNoteTypeVertical(&Dom);
	else
		DimNoteTypeHorizontol(&Dom);
	
	CDRect rect;
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.Move(-rect.left,-rect.bottom);
	if(!pBridge->IsTUGir())
		Dom.Move(Dom.Always(550),Dom.Always(320));
	else
		Dom.Move(Dom.Always(500), -Dom.Always(10));
	
//	if(pBridge->IsTUGir())
//		Dom.Move(Dom.Always(10), -Dom.Always(180));
	
	// ���� ������ �����̵�
	Dom.Move(pMove->GetPoint(NOTE));	
	Dom.SetCalcExtRect();	
	rect = Dom.GetExtRect();	
	// ���� ������ �������			
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),NOTE,"��Ʈ����");	

	*pDomP << Dom;
}

void CAPlateOutDetail::DimNoteTypeVertical(CDomyun *pDomP)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;

	long nSize = m_sNoteArr.GetSize();
	CGridDomyunEx Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetGridLines(GVL_NONE);
	Dom.SetColumnCount(2);
	Dom.SetRowCount(nSize+2);
	Dom.SetColumnWidth(0,Dom.Always(12));
	Dom.SetColumnWidth(1,Dom.Always(100));
	Dom.SetRowHeightAll(Dom.GetTextHeight()*1.4);
	Dom.SetRowHeight(0,Dom.Always(5));
	Dom.SetRowHeight(nSize+1,Dom.Always(5));

	CString str = _T("");
	long nIdxNote = 0;
	for(long i=1; i<nSize+1; i++)
	{
		str = m_sNoteArr.GetAt(i-1);
		if(str.Find(".")==0)
		{
			nIdxNote++;
			Dom.SetTextMatrix(i,0,COMMA(nIdxNote)+". ", GRID_ALIGN_RIGHT);
			str = str.Right(str.GetLength()-1);
		}
		Dom.SetTextMatrix(i,1,str,GRID_ALIGN_LEFT);
	}
	
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL2);
	Dom.DrawOutLine();
	Dom.LineTo(0,0,0,Dom.Always(10));
	Dom.LineTo(0,Dom.Always(10),Dom.Always(30),Dom.Always(10));
	Dom.LineTo(Dom.Always(30),Dom.Always(10),Dom.Always(40),0);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.TextOut(Dom.Always(15), Dom.Always(3.5), "��Ʈ ����");
	*pDomP << Dom;
}

void CAPlateOutDetail::DimNoteTypeHorizontol(CDomyun *pDomP)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;

	CGridDomyunEx Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetGridLines(GVL_NONE);
	long nRows = 15;
	long nCols =  6;
	Dom.SetColumnCount(nCols);
	Dom.SetRowCount(nRows+2);
	long j = 0;
	for(j = 0; j < nCols; j++)
	{
		if(j%2==0)
			Dom.SetColumnWidth(j,Dom.Always(12));
		else
			Dom.SetColumnWidth(j,Dom.Always(100));
	}

	Dom.SetRowHeightAll(Dom.GetTextHeight()*1.4);
	Dom.SetRowHeight(0,Dom.Always(5));
	Dom.SetRowHeight(nRows+1,Dom.Always(5));
	long nIndex = 0;
	long Number = 0;
	for(j=0; j<nCols; j++)
	{
		for(long i=0; i<nRows ; i++)
		{
			if(j%2==1)
			{
				if(nIndex>=m_sNoteArr.GetSize()) break;
				CString str = m_sNoteArr.GetAt(nIndex++);
				if(str.Find(".")==0)
					str = str.Right(str.GetLength()-1);
				Dom.SetTextMatrix(i+1, j, str,GRID_ALIGN_LEFT|GRID_ALIGN_VCENTER);
			}
		}
		if(nIndex>=m_sNoteArr.GetSize()) break;
	}

	nIndex = 0;
	for(j=0; j<nCols; j++)
	{
		for(long i=0; i<nRows ; i++)
		{
			CString sNum = _T("");
			if(j%2==0)
			{
				if(nIndex>=m_sNoteArr.GetSize()) break;
				if(m_sNoteArr.GetAt(nIndex++).Left(1)==".")
				{
					sNum.Format("%d.", ++Number);
					Dom.SetTextMatrix(i+1, j, sNum,GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER);
				}
			}
		}
		if(nIndex>=m_sNoteArr.GetSize()) break;
	}

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL2);
	Dom.DrawGirdOutLine(TRUE);
	Dom.LineTo(0,0,0,Dom.Always(10));
	Dom.LineTo(0,Dom.Always(10),Dom.Always(30),Dom.Always(10));
	Dom.LineTo(Dom.Always(30),Dom.Always(10),Dom.Always(40),0);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.TextOut(Dom.Always(15), Dom.Always(3.5), "��Ʈ ����");
	*pDomP << Dom;
}

void CAPlateOutDetail::DrawAbutPier(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp	*pGir		= (CPlateGirderApp*)pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
	CAPlateDrawJong DrawJong(pStd->GetDataManage());	

	__int64 BxFlg = BX_CROSSBEAM_VBRACING;	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlg,pBxStt,pBxEnd);	

	while(pBx)
	{
//		EXIT_RTN;
		if(pBx->IsJijum())
		{
			long nJijum = pGir->GetNumberJijumByBx(pBx);
			CPierInfo *pPI = pBridge->m_PierInfo + nJijum;

			if(pPI->IsAbut())	DrawAbutSubJong(pDomP, pBxStt, pBxEnd, pBx, pBxStt->IsState(BX_STT_GIRDER) ? TRUE : FALSE);
			else				DrawPierSubJong(pDomP, pBxStt, pBxEnd, pBx);
		}
		pBx = Finder.GetBxNext();
	}	
}

void CAPlateOutDetail::DrawAbutSubJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CPlateBasicIndex *pBx, BOOL bStt)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp	*pGir		= (CPlateGirderApp*)pBx->GetGirder();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	
	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());
	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_MIS1);
			
	CShoe *pShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx));

	double dStaStt       = pBxStt->GetStation();	
	double dSta          = pBx->GetStation();
	double dThickFlange  = pGir->GetThickJewon(G_F_L,pBx);
	if(pStd->GetDataManage()->GetGlobalOption()->GetDesignHeigtBaseLower()==1) dThickFlange = 0;
	double ShoeHeight    = pShoe->GetHeightTotalShoe();
	double dBaseH        = pGir->GetElevationFlangeCamber(dSta,0,CDPoint(0,1),FALSE) - dThickFlange - ShoeHeight;
	long   nJijum        = pGir->GetNumberJijumByBx(pBx);
	
	CPierInfo *pPI = pBridge->m_PierInfo + nJijum;
	double dWidth = pPI->m_daT4;
	double dStaGirSttToAbutBemyun =  dStaStt - pBridge->m_dLengthSlabGirderStt - pBridge->m_dLengthUganStt;
	double dStaGirEndToAbutBemyun =  pBxEnd->GetStation() + pBridge->m_dLengthSlabGirderEnd + pBridge->m_dLengthUganEnd;

	if (!pBridge->IsExtLine() && pStd->GetDomOptionStd()->m_Det_bPierAbutJong)
	{
		if(bStt)
		{
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBxSttBridge = Finder.GetBxFirst(BX_STT_JOINT);
			dWidth /= pBxSttBridge->GetAngle().y;
			
			Dom.MoveTo(dStaGirSttToAbutBemyun - (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3), dBaseH + ShoeHeight - Dom.Always(1));
			Dom.LineTo(dStaGirSttToAbutBemyun - (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3), dBaseH - Dom.Always(15));

			Dom.MoveTo(dStaGirSttToAbutBemyun - pPI->m_daT3,dBaseH + ShoeHeight - Dom.Always(1));
			Dom.LineTo(dStaGirSttToAbutBemyun - pPI->m_daT3,dBaseH);
			Dom.LineTo(dStaGirSttToAbutBemyun - pPI->m_daT3 + dWidth,dBaseH);
			Dom.LineTo(dStaGirSttToAbutBemyun - pPI->m_daT3 + dWidth,dBaseH - Dom.Always(15));

			pOpt->SetEnvType(&Dom,HCAD_CUTL);
			
			Dom.MoveTo(dStaGirSttToAbutBemyun - (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3) - Dom.Always(2),dBaseH + ShoeHeight - Dom.Always(1));
			Dom.LineTo(dStaGirSttToAbutBemyun - pPI->m_daT3 + Dom.Always(2),dBaseH + ShoeHeight - Dom.Always(1));

			Dom.MoveTo(dStaGirSttToAbutBemyun - (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3) - Dom.Always(2),dBaseH - Dom.Always(15));
			Dom.LineTo(dStaGirSttToAbutBemyun - pPI->m_daT3 + dWidth + Dom.Always(2),dBaseH - Dom.Always(15));
		}
		else
		{
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBxEndBridge = Finder.GetBxFirst(BX_END_JOINT);
			dWidth /= pBxEndBridge->GetAngle().y;

			Dom.MoveTo(dStaGirEndToAbutBemyun + (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3),dBaseH + ShoeHeight - Dom.Always(1));
			Dom.LineTo(dStaGirEndToAbutBemyun + (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3),dBaseH - Dom.Always(15));

			Dom.MoveTo(dStaGirEndToAbutBemyun + pPI->m_daT3,dBaseH + ShoeHeight - Dom.Always(1));
			Dom.LineTo(dStaGirEndToAbutBemyun + pPI->m_daT3,dBaseH);
			Dom.LineTo(dStaGirEndToAbutBemyun + pPI->m_daT3 - dWidth,dBaseH);
			Dom.LineTo(dStaGirEndToAbutBemyun + pPI->m_daT3 - dWidth,dBaseH - Dom.Always(15));

			pOpt->SetEnvType(&Dom,HCAD_CUTL);
			Dom.MoveTo(dStaGirEndToAbutBemyun + (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3) + Dom.Always(2),dBaseH + ShoeHeight - Dom.Always(1));
			Dom.LineTo(dStaGirEndToAbutBemyun + pPI->m_daT3 - Dom.Always(2),dBaseH + ShoeHeight - Dom.Always(1));

			Dom.MoveTo(dStaGirEndToAbutBemyun + (pPI->m_daT1 + pPI->m_daT2 - pPI->m_daT3) + Dom.Always(2),dBaseH - Dom.Always(15));
			Dom.LineTo(dStaGirEndToAbutBemyun + pPI->m_daT3 - dWidth - Dom.Always(2),dBaseH - Dom.Always(15));
		}
		*pDom << Dom;
	
		CDimDomyunEx DimDom(pDom);
		pOpt->SetEnvType(&DimDom,HCAD_DIML);
		DimDom.SetDirection("BOTTOM");
		if(bStt)
		{	
			DimDom.DimMoveTo(dStaGirSttToAbutBemyun - pPI->m_daT3, dBaseH, 1);
			DimDom.DimLineTo(pPI->m_daT3 , dBaseH, COMMA(pPI->m_daT3));	
			double dTempL = pBridge->m_dLengthSlabGirderStt + pBridge->m_dLengthUganStt;
			DimDom.DimLineTo(dTempL , dBaseH, COMMA(dTempL));	
			DimDom.DimLineTo(pBridge->m_dLengthGirderShoeStt, dBaseH, COMMA(pBridge->m_dLengthGirderShoeStt));
		}
		else
		{	
			DimDom.DimMoveTo(dStaGirEndToAbutBemyun-(pBridge->m_dLengthSlabGirderEnd+ pBridge->m_dLengthUganEnd+pBridge->m_dLengthGirderShoeEnd), dBaseH, 1);
			DimDom.DimLineTo(pBridge->m_dLengthGirderShoeEnd, dBaseH, COMMA(pBridge->m_dLengthGirderShoeEnd));
			double dTempL = pBridge->m_dLengthSlabGirderEnd+ pBridge->m_dLengthUganEnd;
			DimDom.DimLineTo(dTempL , dBaseH, COMMA(dTempL));	
			DimDom.DimLineTo(pPI->m_daT3 , dBaseH, COMMA(pPI->m_daT3));	
		}
		*pDom << DimDom;
	}

	// �� �׸���
	if(pStd->GetDomOptionStd()->m_Det_bSolePlateJong)
	{
		CDomyun ShoeDom(pDom);
		pOpt->SetEnvType(&ShoeDom,HCAD_STLC);
		double dLen = pGir->GetLengthDimType(dStaStt, dSta, 0);
		DrawSebu.DrawShoeBoxDetail(&ShoeDom,/*m_nJijum,*/pBx,0,0,1.0,TRUE/*,dBogus*/);
		ShoeDom.Move(dStaStt + dLen,dBaseH);
		
		*pDom << ShoeDom;		
	}
}

void CAPlateOutDetail::DrawPierSubJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp	*pGir		= (CPlateGirderApp*)pBx->GetGirder();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();	

	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());

	CDimDomyunEx Dom(pDom), ShoeDom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_MIS1);
			
	CShoe *pShoe = pGir->GetShoe(pGir->GetNumberJijumByBx(pBx));
	double dStaStt = pBxStt->GetStation();
	double dSta = pBx->GetStation();
	double dThickFlange = pGir->GetThickJewon(G_F_L,pBx);
	if(pStd->GetDataManage()->GetGlobalOption()->GetDesignHeigtBaseLower()==1) dThickFlange = 0;
	double ShoeHeight = pShoe->GetHeightTotalShoe();
	double dBaseH = pGir->GetElevationFlangeCamber(dSta,0,CDPoint(0,1),FALSE) - ShoeHeight - dThickFlange;
		
	long nJijum = pGir->GetNumberJijumByBx(pBx);
	CPierInfo *pPI = pBridge->m_PierInfo + nJijum;
	double dLenHead = (nJijum==0) ? pGir->GetSSS(OFF_STT_SHOE) - pGir->GetSSS(OFF_STT_SLAB) + pBridge->m_dLengthUganStt :
									pGir->GetSSS(OFF_END_SLAB) - pGir->GetSSS(OFF_END_SHOE) + pBridge->m_dLengthUganEnd ;		
				
	double dLen = pGir->GetLengthDimType(dStaStt, dSta, 0);	
	if(nJijum == 0) 			
		dLen = -pGir->GetLengthDimType(pGir->GetStationStt(), dStaStt, 0);	 	
	if(nJijum == pBridge->m_nQtyJigan) 
		dLen = pGir->GetLengthDimType(dStaStt, pGir->GetStationEnd(), 0);			

	if (!pBridge->IsExtLine() && pStd->GetDomOptionStd()->m_Det_bPierAbutJong)
	{
		Dom.MoveTo(dStaStt + dLen - pPI->m_dpP,dBaseH - Dom.Always(10));
		Dom.LineTo(dStaStt + dLen - pPI->m_dpP,dBaseH);
		Dom.LineTo(dStaStt + dLen + pPI->m_dpN,dBaseH);
		Dom.LineTo(dStaStt + dLen + pPI->m_dpN,dBaseH - Dom.Always(10));
		Dom.CutLightning(dStaStt + dLen - pPI->m_dpP - Dom.Always(2),dBaseH - Dom.Always(10),
						 dStaStt + dLen + pPI->m_dpN + Dom.Always(2),dBaseH - Dom.Always(10),FALSE);

		pOpt->SetEnvType(&Dom,HCAD_DIML);
		Dom.SetDirection("BOTTOM");	
		if(nJijum == 0)
		{
			Dom.DimMoveTo(dStaStt + dLen - pPI->m_dpP,dBaseH - Dom.Always(10),1);
			Dom.DimLineTo(pPI->m_dpP,dBaseH,COMMA(pPI->m_dpP));
			Dom.DimLineTo(dLenHead,dBaseH,COMMA(dLenHead));
			Dom.DimLineTo(pPI->m_dpN - dLenHead,dBaseH - Dom.Always(10),COMMA(pPI->m_dpN - dLenHead));
		}
		else if(nJijum == pBridge->m_nQtyJigan)
		{
			Dom.DimMoveTo(dStaStt + dLen - pPI->m_dpP,dBaseH - Dom.Always(10),1);
			Dom.DimLineTo(pPI->m_dpP - dLenHead,dBaseH,COMMA(pPI->m_dpP - dLenHead));
			Dom.DimLineTo(dLenHead,dBaseH,COMMA(dLenHead));
			Dom.DimLineTo(pPI->m_dpN,dBaseH - Dom.Always(10),COMMA(pPI->m_dpN));		
		}
		else
		{
			Dom.DimMoveTo(dStaStt + dLen - pPI->m_dpP,dBaseH - Dom.Always(10),1);
			Dom.DimLineTo(pPI->m_dpP,dBaseH,COMMA(pPI->m_dpP));
			Dom.DimLineTo(pPI->m_dpN,dBaseH - Dom.Always(10),COMMA(pPI->m_dpN));
		}
	}

	double	dFlT	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	long	nPier	= pGir->GetNumberJijumByBx(pBx);
	CPierInfo *pInfo = pBridge->m_PierInfo + nPier;
	CDPoint xy(dSta, -dFlT+dBaseH+ShoeHeight/2);
	xy.y -= Dom.Always(2);
	pDom->TextCircle(xy.x, xy.y, pBridge->m_strJijumName[nPier], 3);
	*pDom << Dom;

	if (pStd->GetDomOptionStd()->m_Det_bSolePlateJong)
	{
		// �� �׸���
//		double dBogus = pBridge->m_pDDB->m_pDBStd->GetDBBaseJong()->GetBogusFactor(pBx,G_F_L,bLeft?-1:1,pBxStt,pBxEnd)
//					  * pBridge->m_pDDB->m_pDBStd->GetBogusThick();
		dLen = pGir->GetLengthDimType(dStaStt, dSta, 0);
		pOpt->SetEnvType(&ShoeDom,HCAD_STLC);
		DrawSebu.DrawShoeBoxDetail(&ShoeDom,/*m_nJijum,*/pBx,0,0,1,TRUE/*,dBogus*/);
		ShoeDom.Move(dStaStt + dLen,dBaseH);
		
		*pDom << ShoeDom;
	}
	*pDom << Dom;
}

void CAPlateOutDetail::DrawFlangeJewon(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double Angle, BOOL bUpper)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp	*pGir		= (CPlateGirderApp*)pBxStt->GetGirder();	
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBxFinder		Finder(pGir);
	
	CDomyun*		p = new CDomyun(pDom);		
	pOpt->SetEnvType(p,HCAD_DIML);
	long nMarkBujeFact = (bUpper) ? G_F_U : G_F_L;
	CString szText;
	
	long nDir = bUpper ? 1 : -1;
	__int64 BxFlag = (bUpper) ? BX_UPPERFLANGE | BX_SPLICE: 
								BX_LOWERFLANGE | BX_SPLICE;	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, TRUE);	
	CPlateBasicIndex *pBxNext = NULL;
	BOOL	bFirst	= TRUE;
	while(pBx)		
	{
		pBxNext	= Finder.GetBxFixNext();
		if(!pBxNext)	pBxNext = pBxEnd;
		if(bFirst) 
		{
			pBxNext = pBx;
			pBx = pBxStt;			
		}

		CDomyun SubDom(p);
		double	dWidth	= pGir->GetWidthOnStation(pBx->GetStation(), bUpper);
		double	dL		= fabs(pBxNext->GetStation() - pBx->GetStation());
		double	dSta	= pBx->GetStation()+dL/5;
		CDPoint	xyPos	= pGir->GetXyGirderDis(dSta, (dWidth/5)*nDir);
		CVector	vJ		= pGir->GetJewonTotal(nMarkBujeFact, pBx, 0, 0, DIM_BASIC);
		CString szJewon	= pOpt->GetJewonString(1,vJ.x,vJ.y,vJ.z);

		szText.Format(bUpper ? "��� �÷���(%s)" : "�Ϻ� �÷���(%s)", pBx->GetFactChain(nMarkBujeFact)->m_strSymbol);
		double	dStrLen	= max(p->GetTextWidth(szText,2), p->GetTextWidth(szJewon,2));
		double	dSpace	= p->Always(20);
		double dLineAng = pGir->GetLine()->GetAngleAzimuth(dSta+dSpace).GetAngleRadian();

		SubDom.WaveArrow(xyPos.x, xyPos.y, dSpace, dStrLen, bUpper ? 6 : 2, szText, szJewon);
		SubDom.Rotate(dLineAng, xyPos.x, xyPos.y);
		*p << SubDom;
		pBx = Finder.GetBxNext();
		if(bFirst)
			pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, TRUE);
		bFirst = FALSE;
	}
	*pDom << *p;
	delete p;
}

void CAPlateOutDetail::DrawWebJewon(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp	*pGir		= (CPlateGirderApp*)pBxStt->GetGirder();
	CLineInfo		*pLine		= pGir->GetLine();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBxFinder		Finder(pGir);
	
	CDomyun			p(pDom);
	p.SetScaleDim(pDom->GetScaleDim());		

	__int64 BxFlag = BX_SPLICE | BX_WEB;
	CPlateBasicIndex	*pBx		= Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, TRUE);
	CPlateBasicIndex	*pBxNext	= NULL;

	BOOL	bFirst	= TRUE;
	while(pBx)
	{
		pBxNext	= Finder.GetBxFixNext();
		if(!pBxNext)	pBxNext = pBxEnd;
		if(bFirst) 
		{
			pBxNext = pBx;
			pBx = pBxStt;			
		}

		CVector vWeb		= pGir->GetJewonTotal(G_W, pBx, 0, 0, DIM_BASIC);
		CString	szJewon		= _T("");
		CString	szText		= _T("");

		szJewon = pOpt->GetJewonString(1, vWeb.x, vWeb.y, vWeb.z);
		szText.Format("������ �÷���Ʈ (%s)", pBx->GetFactChain(G_W)->m_strSymbol);

		double	szLen	= max(p.GetTextWidth(szText,4), p.GetTextWidth(szJewon,4));
		double	dSta	= pBx->GetStation()+(pBxNext->GetStation()-pBx->GetStation())/5;
		double	dElev	= pGir->GetElevationFlangeCamber(dSta, 0, CDPoint(0,1), FALSE);
		p.WaveArrow(dSta, dElev+100, p.Always(26), szLen, 6, szText, szJewon);		

		pBx = Finder.GetBxNext();
		if(bFirst)
			pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, TRUE);

		bFirst = FALSE;
	}

	*pDom << p;	
}

void CAPlateOutDetail::DrawJijumStiffJewon(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double dAng, BOOL bUpper)
{
	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd  *pOpt	= pStd->GetDataManage()->GetOptionStd();	
	CPlateGirderApp	 *pGir  = (CPlateGirderApp	*)pBxStt->GetGirder();		

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_JIJUMSTIFF, pBxStt, pBxEnd);

	if(pBx)
	{
		CDomyun Dom(pDom);
		pOpt->SetEnvType(&Dom,HCAD_DIML);	
		double dWidth = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);
		double dThick = pBx->GetJijumStiff()->m_dThick[0];
		CDPoint ptPos = pGir->GetXyGirderDis(pBx->GetStation() + dThick/2, bUpper ? -dWidth/4 : dWidth/4);				
		CVector vJ = pGir->GetJewonTotal(V_J_ST,pBx,0,0,DIM_BASIC);		
		CString szJewon = pOpt->GetJewonString(2, vJ.x, vJ.y, vJ.z);		
		Dom.GiSiArrowExtend(ptPos.x, ptPos.y, Dom.Always(20), Dom.GetTextWidth(szJewon), bUpper ? 1 : 7, "������ ������", szJewon);
		Dom.Rotate(dAng,ptPos.x, ptPos.y);

		*pDom << Dom;
	}
}


void CAPlateOutDetail::DrawStationMark(CDomyun* pDom, CPlateBasicIndex *pBx, int nDan)
{
	CAPlateDrawStd  *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CARoadOptionStd *pOpt 	 = pStd->GetDataManage()->GetOptionStd();	
	CLineInfo       *pLine   = pGir->GetLine();
	
	CDomyun Dom(pDom);	
	pOpt->SetEnvType(&Dom, HCAD_DIML);

	CString cs;
	double Sta      = pBx->GetStation();
	double Angle    = pLine->GetAngleAzimuth(Sta).GetAngleRadian();
	double dWFlange = pGir->GetWidthFlangeUpper(pBx);
	double width    = pGir->m_dGirderCenter - dWFlange/2;
	double Height   = Dom.Always(Dom.GetDimExo() + Dom.GetDimExe() + Dom.GetDimVLen() + Dom.GetDimLevelDis() * nDan);
	double dStaHPos = Dom.Always(Dom.GetDimLevelDis());
	double dSpace   = Dom.Always(10);	// ��ġ���ΰ� ������ ���ϱ� ���� �ణ ������.

	CDPoint po1 = pLine->GetXyLineDis(Sta, width-Dom.GetDimTvp());
	CDPoint po2 = pLine->GetXyLineDis(Sta, width-Height+dStaHPos+dSpace);
	Dom.LineTo(po1,po2);

	CDomyun DomSta(pDom);	
	pOpt->SetEnvType(&DomSta, HCAD_DIML);	
	DomSta.SetTextAlignHorz(TA_LEFT);
	DomSta.SetTextAngle(ToDegree(Angle));

	CString strSta = pBridge->GetStationForStringOutDB(Sta);	
	double dStaE   = pGir->GetStationByActLength(Sta,Dom.GetTextWidth(strSta)+Dom.Always(4),-dWFlange/2-Height+DomSta.Always(DomSta.GetDimLevelDis()));
	double dStaLen = pGir->GetStationByActLength(Sta,Dom.Always(4),-dWFlange/2-Height+DomSta.Always(DomSta.GetDimLevelDis()));
	po1 = pLine->GetXyLineDis(Sta,width-Height+DomSta.Always(DomSta.GetDimLevelDis())+dSpace);
	po2 = pLine->GetXyLineDis(dStaE,width-Height+DomSta.Always(DomSta.GetDimLevelDis())+dSpace);

	DomSta.LineTo(po1,po2);	
	
	CDPoint po3 = pLine->GetXyLineDis(dStaLen,width-Height+DomSta.Always(DomSta.GetDimLevelDis()-DomSta.GetDimTvp())+dSpace);	
	DomSta.TextOut(po3.x,po3.y,strSta);	
	DomSta.Rotate(ConstPi/12,po1.x,po1.y);
	Dom << DomSta;	

	*pDom << Dom;
}

void CAPlateOutDetail::DrawAddAbutPierPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{	
	CAPlateDrawStd  *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp *pGir    = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUM, pBxStt, pBxEnd);	

	while(pBx)
	{		
		long nJijum = pGir->GetNumberJijumByBx(pBx);
		CPierInfo *pPI = pBridge->m_PierInfo + nJijum;

		if(pPI->IsAbut())	DrawAbutSubPyung(pDom, pBx, pBxStt->IsState(BX_STT_GIRDER) ? TRUE : FALSE);
		else				DrawPierSubPyung(pDom, pBx);

		pBx = Finder.GetBxNext();
	}
}


void CAPlateOutDetail::DrawAbutSubPyung(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bStt)
{
	CAPlateDrawStd  *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CARoadOptionStd *pOpt 	 = pStd->GetDataManage()->GetOptionStd();	

	CDomyun Dom(pDom);	
		
	long nJijum = pGir->GetNumberJijumByBx(pBx);
	CPierInfo *pPI = pBridge->m_PierInfo + nJijum;
	double dWidth   = pPI->m_daT4 - pPI->m_daT3;
	double dWFlangeUp = pGir->GetWidthFlangeUpper(pBx);
	double dWFlangeDn = pGir->GetWidthFlangeLower(pBx);

	double dSta = bStt ? pGir->GetStationStt() : pGir->GetStationEnd();
	CDPoint dAng = bStt ? pGir->GetAngleStt() : pGir->GetAngleEnd();
	CDPoint LeftXY  = pGir->GetXyGirderDis(dSta,-dWFlangeUp/2-Dom.Always(12),dAng);	
	CDPoint RightXY = pGir->GetXyGirderDis(dSta, dWFlangeDn/2+Dom.Always(12),dAng);
	CDPoint vDir = (RightXY - LeftXY).Unit();	

	CDPoint LeftXY1  =  LeftXY - (bStt ? vDir.Rotate90() : vDir.RotateInv90()) * pPI->m_daT3;
	CDPoint RightXY1 = RightXY - (bStt ? vDir.Rotate90() : vDir.RotateInv90()) * pPI->m_daT3;
	CDPoint LeftXY2  = LeftXY + (bStt ? vDir.Rotate90() : vDir.RotateInv90()) * dWidth;
	CDPoint RightXY2 = RightXY + (bStt ? vDir.Rotate90() : vDir.RotateInv90()) * dWidth;
	
	CDPoint CrossXY1, CrossXY2;
	CDPoint vVec = pGir->GetLine()->GetAngleAzimuth(dSta);
	GetXyMatchSegAndLine(LeftXY2,RightXY2,LeftXY1,vVec,CrossXY1);
	GetXyMatchSegAndLine(LeftXY2,RightXY2,RightXY1,vVec,CrossXY2);

	pOpt->SetEnvType(&Dom,HCAD_CUTL);
	Dom.LineTo(LeftXY1,CrossXY1);	
	Dom.LineTo(RightXY1,CrossXY2);
	
	pOpt->SetEnvType(&Dom,HCAD_MIS1);
	Dom.LineTo(LeftXY1,RightXY1);
	Dom.LineTo(CrossXY1,CrossXY2);
	
	*pDom << Dom;
}


void CAPlateOutDetail::DrawPierSubPyung(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd  *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CARoadOptionStd *pOpt 	 = pStd->GetDataManage()->GetOptionStd();	

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_MIS1);
				
	double  dSta = pBx->GetStation();
	double dWFlangeUp = pGir->GetWidthFlangeUpper(pBx);
	double dWFlangeDn = pGir->GetWidthFlangeLower(pBx);
	CDPoint dAng = pBx->GetAngle();

	long nJijum = pGir->GetNumberJijumByBx(pBx);
	if(nJijum == 0) 
	{
		dSta = pGir->GetStationStt();	 	
		dAng = pGir->GetAngleStt();
	}	
	if(nJijum == pBridge->m_nQtyJigan) 
	{
		dSta = pGir->GetStationEnd();	 	
		dAng = pGir->GetAngleEnd();		
	}
	
	CDPoint LeftXY  = pGir->GetXyGirderDis(dSta,-dWFlangeUp/2-Dom.Always(10),dAng);	
	CDPoint RightXY = pGir->GetXyGirderDis(dSta, dWFlangeDn/2+Dom.Always(10),dAng);
	CDPoint vDir = (RightXY - LeftXY).Unit();

	CPierInfo *pPI = pBridge->m_PierInfo + nJijum;	

	CDPoint LeftXY1  = LeftXY + vDir.RotateInv90() * pPI->m_dpP;
	CDPoint LeftXY2  = LeftXY + vDir.Rotate90() * pPI->m_dpN;
	CDPoint RightXY1 = RightXY + vDir.RotateInv90() * pPI->m_dpP;
	CDPoint RightXY2 = RightXY + vDir.Rotate90() * pPI->m_dpN;

	CDPoint CrossXY1, CrossXY2;
	CDPoint vVec = pGir->GetLine()->GetAngleAzimuth(dSta);
	if(GetXyMatchSegAndLine(LeftXY2,RightXY2,LeftXY1,vVec,CrossXY1))
	{
		CrossXY1 = LeftXY2;
		GetXyMatchSegAndLine(LeftXY1,RightXY1,LeftXY2,vVec,LeftXY1);
	}
	if(GetXyMatchSegAndLine(LeftXY2,RightXY2,RightXY1,vVec,CrossXY2))
	{
		CrossXY2 = RightXY2;
		GetXyMatchSegAndLine(LeftXY1,RightXY1,RightXY2,vVec,RightXY1);
	}

	CDPoint vPierVec;
	long nPierNum = pGir->GetNumberJijumByBx(pBx);
	CPlateGirderApp  *pGirL = pBridge->GetGirder(0);
	CPlateGirderApp  *pGirR = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	CPlateBasicIndex *pBxL  = pGirL->GetBxOnJijum(nPierNum);
	CPlateBasicIndex *pBxR  = pGirR->GetBxOnJijum(nPierNum);	
	CDPoint vAng = pBxL->GetAngle();
	CDPoint vLeftPoint = pGirL->GetXyGirderDis(pBxL->GetStation(),0,vAng);	
	CDPoint vRightPoint = pGirR->GetXyGirderDis(pBxR->GetStation(),0,vAng);	
	double dWidth = ~(vLeftPoint - vRightPoint);
	double dEdge = (pPI->m_dpW - dWidth)/2;

	if(pBx->GetNumberGirder() == 0)
	{	
		LeftXY  = pGir->GetXyGirderDis(dSta,-dEdge,dAng);	
		LeftXY1  = LeftXY + vDir.RotateInv90() * pPI->m_dpP;
//		bCross = GetXyMatchSegAndLine(LeftXY2,RightXY2,LeftXY1,vVec,CrossXY1);
		if(GetXyMatchSegAndLine(LeftXY2,RightXY2,LeftXY1,vVec,CrossXY1))
		{
			CrossXY1 = LeftXY2;
			GetXyMatchSegAndLine(LeftXY1,RightXY1,LeftXY2,vVec,LeftXY1);
		}
		if(pPI->m_dpR == 0)					
			Dom.LineTo(LeftXY1,CrossXY1);					
		else
		{
			CDPoint pArcVec = (CrossXY1 - LeftXY1).Unit();
			CDPoint CenPo = (LeftXY1 + CrossXY1) / 2 + pArcVec.Rotate90() * pPI->m_dpR;
			Dom.Arc(CrossXY1.x,CrossXY1.y,CenPo.x,CenPo.y,LeftXY1.x,LeftXY1.y);
		}
	}
	else
	{
		pOpt->SetEnvType(&Dom,HCAD_CUTL);
		vPierVec = (CrossXY1 - LeftXY1).Unit();
		CDPoint Po1 = LeftXY1 - vPierVec * Dom.Always(2);
		CDPoint Po2 = CrossXY1 + vPierVec * Dom.Always(2);
		//Dom.CutLightning(Po1.x,Po1.y,Po2.x,Po2.y,FALSE);
		Dom.LineTo(Po1,Po2);			
		pOpt->SetEnvType(&Dom,HCAD_MIS1);
	}	
	
	if(pBx->GetNumberGirder() == pBridge->GetGirdersu()-1)
	{
		RightXY  = pGir->GetXyGirderDis(dSta,dEdge,dAng);	
		RightXY1  = RightXY + vDir.RotateInv90() * pPI->m_dpP;
//		bCross = GetXyMatchSegAndLine(LeftXY2,RightXY2,RightXY1,vVec,CrossXY2);
		if(GetXyMatchSegAndLine(LeftXY2,RightXY2,RightXY1,vVec,CrossXY2))
		{
			CrossXY2 = RightXY2;
			GetXyMatchSegAndLine(LeftXY1,RightXY1,RightXY2,vVec,RightXY1);
		}
			
		if(pPI->m_dpR == 0)
			Dom.LineTo(RightXY1,CrossXY2);			
		else
		{
			CDPoint pArcVec = (RightXY1 - CrossXY2).Unit();
			CDPoint CenPo = (RightXY1 + CrossXY2) / 2 + pArcVec.Rotate90() * pPI->m_dpR;
			Dom.Arc(RightXY1.x,RightXY1.y,CenPo.x,CenPo.y,CrossXY2.x,CrossXY2.y);
		}
	}
	else
	{
		pOpt->SetEnvType(&Dom,HCAD_CUTL);
		vPierVec = (CrossXY2 - RightXY1).Unit();
		CDPoint Po1 = RightXY1 - vPierVec * Dom.Always(2);
		CDPoint Po2 = CrossXY2 + vPierVec * Dom.Always(2);
		//Dom.CutLightning(Po1.x,Po1.y,Po2.x,Po2.y,FALSE);
		Dom.LineTo(Po1,Po2);
		pOpt->SetEnvType(&Dom,HCAD_MIS1);
	}

	pOpt->SetEnvType(&Dom,HCAD_MIS1);
	Dom.LineTo(LeftXY1,RightXY1);
	Dom.LineTo(CrossXY1,CrossXY2);

	//Dom.LineTo(LeftXY1,LeftXY2);
	//Dom.LineTo(RightXY1,RightXY2);
	//Dom.LineTo(LeftXY1,RightXY1);
	//Dom.LineTo(LeftXY2,RightXY2);

	*pDom << Dom;
}