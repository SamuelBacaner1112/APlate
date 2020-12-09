// APlateOutGeneral.cpp: implementation of the CAPlateOutGeneral class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutGeneral.h"
#include "APlateDrawStd.h"
#include "APlateDrawDetailDim.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PLAN		10
#define JONG		20
#define NOTE		80
#define HDANMYUN	90
#define KEY_PLAN	100  
#define MPE_GEN_LEGEND 110

#define DANMYUN_JIJUM 15
#define DANMYUN_CROSS 25
#define DANMYUN_VBRA  35
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutGeneral::CAPlateOutGeneral()
{	
	m_JweonInfoStaArr.RemoveAll();
}

CAPlateOutGeneral::~CAPlateOutGeneral()
{

}

double CAPlateOutGeneral::GetScale()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	
	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// �Ϲݵ������� �����ȣ
	long nSubPage = nPage - nprePage;

	m_Scale  = pOpt->m_Gen_Scale.GetAt(m_nPage == nSubPage ? 1 : 0);

	return m_Scale;
}

CString CAPlateOutGeneral::GetTitle()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	long nPageSu = m_nPage;
	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("�� �� �� �� �� (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("�� �� �� �� ��");

	return szTitle;
}

long CAPlateOutGeneral::GetPageSu()
{ 
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(0);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	m_nPage = 0;
	long nTotal  = 0;

	BOOL bSplite = pOpt->m_Gen_bDivPlanJong;

	switch(pOpt->m_Gen_nDivPage)
	{
		case 0:
			nTotal = pBridge->m_nQtyJigan;
			m_nPage = nTotal/pOpt->m_Gen_nUnitEaDraw;
			if(nTotal%pOpt->m_Gen_nUnitEaDraw)
				m_nPage++;
			break;
		case 1:
			nTotal  = pGir->GetQtySplice()+1;
			m_nPage = nTotal/pOpt->m_Gen_nUnitEaDraw;
			if(nTotal%pOpt->m_Gen_nUnitEaDraw)
				m_nPage++;
			break;
		case 2:
			nTotal = pGir->GetQtyCrossBeamVBracing(); 
			m_nPage = nTotal/pOpt->m_Gen_nUnitEaDraw;
			if(nTotal%pOpt->m_Gen_nUnitEaDraw)
				m_nPage++;
			break;	
		case 3:
			m_nPage = 1;
			break;
	}

	if(bSplite) m_nPage *= 2;
	
	++m_nPage; // Ⱦ�ܸ鵵 ��
	return m_nPage;
}

void CAPlateOutGeneral::DrawOnePage()
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// �Ϲݵ������� �����ȣ
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();
	BOOL bSplite  = pDomOpt->m_Gen_bDivPlanJong;

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(30);
//	pDom->SetScaleDim(Scale);
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	// ����
	pPageMng->SetProgressData("���� �����...",0);
	pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);	
	pStd->AddMaterial(&Dom,Scale);
	pStd->AddJubujebubuje(&Dom,Scale);
	Dom.SetCalcExtRect();
	CDRect dgkRect = Dom.GetExtRect(), DomBound;
	*pDom << Dom;			

	pDom->SetCalcExtRect();
	CDRect	DgkBound = pDom->GetExtRect();
	CDPoint xyMove = CDPoint(Dom.Always(100), DgkBound.Height()-Dom.Always(60));
	CDRect rtPlan;
	////////////////////////////
	// ��鵵
	if(m_nPage == nSubPage)
	{		
////////////////////////////////////////////////////////////////////
//		EXIT_RTN;
		pPageMng->SetProgressData("������ ���κ� �ܸ鵵 �����...",20);
		AddHDanJiJum(&Dom);
		Dom.SetScaleDim(pDom->GetScaleDim());

		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height());
		xyMove.x += rect.Width()+Dom.Always(40);

		Dom.Move(pMove->GetPoint(DANMYUN_JIJUM));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DANMYUN_JIJUM,"������ ���κ�");
		*pDom << Dom;

		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "������ ���κ�",TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(DANMYUN_JIJUM+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DANMYUN_JIJUM+1,"������ ���κ� ����");
		*pDom << Dom;
////////////////////////////////////////////////////////////////////
//		EXIT_RTN;
		pPageMng->SetProgressData("�й� ���κ� �ܸ鵵 �����...",40);
		AddHDanHGirder(&Dom);
		Dom.SetScaleDim(pDom->GetScaleDim());
		
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height());
		xyMove.x -= rect.Width()+Dom.Always(40);
		xyMove.y -= rect.Height()+Dom.Always(40);

		Dom.Move(pMove->GetPoint(DANMYUN_CROSS));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DANMYUN_CROSS,"�й� ���κ�");
		*pDom << Dom;	

		CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "�й� ���κ�",TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(DANMYUN_CROSS+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DANMYUN_CROSS+1,"�й� ���κ� ����");
		*pDom << Dom;
////////////////////////////////////////////////////////////////////
//		EXIT_RTN;
		pPageMng->SetProgressData("�����극�̽� �ܸ鵵 �����...",70);
		// �����극�̽��� �ִ��� ����...
		CPlateBxFinder Finder(pBridge->GetGirder(0));	
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec->IsenDetType(SECDET_VBRACING)) break;			
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
		if(pBx)
		{
			AddHDanVBracing(&Dom);
			Dom.SetScaleDim(pDom->GetScaleDim());

			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rect.left, -rect.top-rect.Height());
			xyMove.y -= rect.Height()+Dom.Always(40);
			
			Dom.Move(pMove->GetPoint(DANMYUN_VBRA));
			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),DANMYUN_VBRA,"�����극�̽�");
			*pDom << Dom;	

			DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "�����극�̽�",TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(DANMYUN_VBRA+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),DANMYUN_VBRA+1,"�����극�̽� ����");
			*pDom << Dom;
		}
////////////////////////////////////////////////////////////////////
		if(pDomOpt->m_Gen_bBasicNote)
		{
//			EXIT_RTN;
			pPageMng->SetProgressData("��Ʈ���� �����...",90);
			InitNote();
			AddNote(&Dom,nSubPage);
			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rect.left, -rect.top-rect.Height());
			Dom.Move(pMove->GetPoint(NOTE));
			Dom.SetCalcExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),NOTE,"��Ʈ����");
			*pDom << Dom;	
		}
	}
	else
	{
		if(!bSplite || nSubPage<=GetPageSu()/2)
		{
			if(pDomOpt->m_Gen_bPlanView)
			{
//				EXIT_RTN;
				pPageMng->SetProgressData("��鵵 ����� ...",10);
				AddPlan(&Dom,nSubPage);
				Dom.SetCalcExtRect();
				rtPlan = Dom.GetExtRect();
				double MoveX = Dom.Always(190);
				double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(60);
				Dom.Move(MoveX, MoveY);
				Dom.Move(pMove->GetPoint(PLAN));
				Dom.SetCalcExtRect();	
				CDRect rect = Dom.GetExtRect();	
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,rect,PLAN,"��鵵");		
				*pDom << Dom;	

				DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "�� �� ��",TRUE, TRUE, 0);
				Dom.Move(pMove->GetPoint(PLAN+1));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),PLAN+1,"��鵵 ����");
				*pDom << Dom;
			}
			if(pDomOpt->m_Gen_bFrontView)
			{
//				EXIT_RTN;
				pPageMng->SetProgressData("Ⱦ�ܸ鵵 �����...",20);
				AddFront(&Dom,nSubPage);		
				Dom.SetCalcExtRect();
				double MoveX = Dom.Always(80);
				double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(60);
				Dom.Move(MoveX, MoveY);					
				Dom.Move(pMove->GetPoint(HDANMYUN));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),HDANMYUN,"Ⱦ�ܸ鵵");		
				CDRect rect = Dom.GetExtRect();
				*pDom << Dom;	

				DrawStd.AddTitleOnDom(&Dom, m_Scale, rect, "Ⱦ �� �� ��",TRUE, TRUE, 0, pDomOpt->m_Gen_bVerTitleFront);
				if(pDomOpt->m_Gen_bVerTitleFront==FALSE)
				{
					Dom.Rotate(ToRadian(-90), (rect.left+rect.right)/2, (rect.bottom+rect.top)/2);
					Dom.Move(0, rect.Height()/2);
				}
				Dom.Move(pMove->GetPoint(HDANMYUN+1));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),HDANMYUN+1,"Ⱦ�ܸ鵵 ����");	
				*pDom << Dom;
			}
		}
		
		if(!bSplite || nSubPage>GetPageSu()/2)
		{	
			if(pStd->GetDomOptionStd()->m_Gen_bJongView && (pStd->GetDomOptionStd()->m_Gen_bJongCenterGir || pStd->GetDomOptionStd()->m_Gen_bJongGenGir))
			{
//				EXIT_RTN;
				pPageMng->SetProgressData("���鵵 �����...",80);
				AddJong(&Dom,nSubPage);
				Dom.Move(pMove->GetPoint(JONG));
				Dom.SetCalcExtRect();	
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),JONG,"���鵵");
				CDRect rect = Dom.GetExtRect();
				*pDom << Dom;
					
				DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "�� �� ��",TRUE, TRUE, 0);	
				Dom.SetCalcExtRect();		
				Dom.Move(pMove->GetPoint(JONG+1));
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),JONG+1,"���鵵 ����");
				*pDom << Dom;
			}
		}
		if(pDomOpt->m_Gen_bKeyPlan)
		{
///\			EXIT_RTN;			
			pPageMng->SetProgressData("��൵ �����...",80);						
			AddKeyPan(&Dom,nSubPage,Dom.Always(600),Dom.Always(50));
			*pDom << Dom;
		}
		if(pDomOpt->m_Gen_bBasicNote)
		{
//			EXIT_RTN;
			pPageMng->SetProgressData("��Ʈ���� �����...",90);
			InitNote();
			AddNote(&Dom,nSubPage);
			Dom.SetCalcExtRect();
			double MoveX = Dom.Always(620);
			double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(680);
			Dom.Move(-MoveX, MoveY);
			Dom.Move(pMove->GetPoint(NOTE));
			
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),NOTE,"��Ʈ����");
			*pDom << Dom;			
		}		
		if(pDomOpt->m_Gen_bLegend)
		{
//		EXIT_RTN;
			pPageMng->SetProgressData("���� �����...",98);
			AddLegend(&Dom);
			Dom.SetCalcExtRect();
			double MoveX = Dom.Always(100);
			double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(450);
			Dom.Move(MoveX, MoveY);
			Dom.Move(pMove->GetPoint(MPE_GEN_LEGEND));
			Dom.SetCalcExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_GEN_LEGEND,"����");
			*pDom << Dom;
		}
	}
	//}	

	pPageMng->SetProgressData("��¿Ϸ�",100);
}

// �Ϲݵ� ��� ���� �׸���
void CAPlateOutGeneral::AddPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CPlateBasicIndex *pBxStt, *pBxEnd;

	CString str = _T("");
	long nLine	= 1;
	if(pDomOpt->m_Gen_bBujeThick)
		nLine = 2;
	
	BOOL bSplice	= (pDomOpt->m_Gen_PyungBuje & DRAW_PLAN_SPLICE)    ? TRUE : FALSE;
	BOOL bFact		= (pDomOpt->m_Gen_PyungBuje & DRAW_PLAN_FACT)	    ? TRUE : FALSE;
	BOOL bCrossBeam	= (pDomOpt->m_Gen_PyungBuje & DRAW_PLAN_CROSSBEAM) ? TRUE : FALSE;
	BOOL bSolePlate	= (pDomOpt->m_Gen_PyungBuje & DRAW_PLAN_SOLEPLATE) ? TRUE : FALSE;
	BOOL bJackUp	= (pDomOpt->m_Gen_PyungBuje & DRAW_PLAN_JACKUP)    ? TRUE : FALSE;
	BOOL bWebLine   = (pDomOpt->m_Gen_PyungBuje & DRAW_PLAN_WEB)		? TRUE : FALSE;
	BOOL bSlabLine  = (pDomOpt->m_Gen_PyungBuje & DRAW_PLAN_SLAB)		? TRUE : FALSE;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	
	pBxStt = GetPageBxStt(CENTERGIRDER,nSubPage);
	pBxEnd = GetPageBxEnd(CENTERGIRDER,nSubPage);	

	if(bSlabLine) //������
	{
		pPageMng->SetProgressData("��鵵 ����� (������)... ",10);
		long nG = 0;
		CPlateBasicIndex *pBxLeftStt = GetPageBxStt(nG, nSubPage, FALSE);
		CPlateBasicIndex *pBxLeftEnd = GetPageBxEnd(nG, nSubPage,FALSE);
		nG = pBridge->GetGirdersu()-1;
		CPlateBasicIndex *pBxRightStt = GetPageBxStt(nG, nSubPage,FALSE);
		CPlateBasicIndex *pBxRightEnd = GetPageBxEnd(nG, nSubPage,FALSE);
		DrawPyung.DrawSlabByBx(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);
	}
	// ���� �߽ɼ�, CL of Road
	pPageMng->SetProgressData("��鵵 ����� (���� �߽ɼ�)... ",10);
	DrawPyung.DrawCenterLine(&Dom, -1, pBxStt->GetStation(), pBxEnd->GetStation());

	// ���Ϻθ�ũǥ��(������ �߽�)
	pPageMng->SetProgressData("��鵵 ����� (���Ϻθ�ũ)... ",10);
	DrawPyung.DrawTopBottomViewMark(&Dom, pBxStt, pBxEnd);

	// ����극�̽� �� ���κ� �߽ɼ�
	pPageMng->SetProgressData("��鵵 ����� (����극�̽� �� ���κ� �߽ɼ�)... ",10);
	long nG = 0;
	for(nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		DrawPyung.DrawHBracingCLWithenBx(&Dom, pBxStt, pBxEnd);
		DrawPyung.DrawCRBeamCLWithenBx(&Dom, pBxStt, pBxEnd);
	}

	// �������� ��� �׷����� ������ ������
    // ����극�̽� ������
	// ����극�̽� ��
	pPageMng->SetProgressData("��鵵 ����� (����극�̽�)... ",10);
	long nGsu = pBridge->GetGirdersu();
	for(nG = 0; nG < nGsu; nG++)
	{
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		if(nGsu%2==1) // Ȧ���� �Ŵ�
		{		
			if(nG>=pBridge->GetGirdersu()/2)
			{		
				DrawPyung.DrawHBracingSpliceWithenBx(&Dom, pBxStt, pBxEnd,FALSE,nG==pBridge->GetGirdersu()/2 ? 1 : 0);	
				DrawPyung.DrawHBracingBeamWithenBx  (&Dom, pBxStt, pBxEnd);	
			}
		}
		else // ¦���� �Ŵ�
		{
			if(nG>=pBridge->GetGirdersu()/2)			
				DrawPyung.DrawHBracingSpliceWithenBx(&Dom, pBxStt, pBxEnd,FALSE,0);				
			if(nG>=pBridge->GetGirdersu()/2-1)			
				DrawPyung.DrawHBracingBeamWithenBx  (&Dom, pBxStt, pBxEnd,FALSE,nG==pBridge->GetGirdersu()/2-1 ? TRUE : FALSE);				
		}
	}
	pPageMng->SetProgressData("��鵵 ����� (���κ� �� �����극�̽�)... ",10);
	// ���κ� �� �����극�̽� ���
	if(bCrossBeam)
	{
		if(nGsu%2==1) // Ȧ���� �Ŵ�
		{	
			for(long nG = 0; nG < nGsu-1; nG++)
			{
				pBxStt = GetPageBxStt(nG,nSubPage);
				pBxEnd = GetPageBxEnd(nG,nSubPage);	
				if(pStd->GetDataManage()->GetGlobalOption()->GetDesignBridgeType()==1 || nG>=pBridge->GetGirdersu()/2)
					DrawPyung.DrawCRBeamVBracingFlangeWithenBx(&Dom, pBxStt, pBxEnd);	
			}
		}
		else
		{
			for(long nG = 0; nG < nGsu-1; nG++)
			{
				pBxStt = GetPageBxStt(nG,nSubPage);
				pBxEnd = GetPageBxEnd(nG,nSubPage);	
				if(pStd->GetDataManage()->GetGlobalOption()->GetDesignBridgeType()==1 || nG>=pBridge->GetGirdersu()/2-1)
				{
					DrawPyung.DrawCRBeamVBracingFlangeWithenBx(&Dom, pBxStt, pBxEnd,TRUE,nG==pBridge->GetGirdersu()/2-1 ? TRUE : FALSE);	
					if(nG==pBridge->GetGirdersu()/2-1)
						DrawPyung.DrawCRBeamVBracingFlangeWithenBx(&Dom, pBxStt, pBxEnd,FALSE,TRUE);	
				}
			}
		}
	}	
	pPageMng->SetProgressData("��鵵 ����� (�Ŵ�)... ",10);
	// nSide:-1 �߽ɰŴ����� �����Ŵ�
	// nSide:-1 �߽ɰŴ����� �����Ŵ�
	for(long nSide=-1;nSide<=1; nSide+=2)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);			
		if(nSide==-1) pStd->m_pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);

		long nSttGir = (nSide==0 || nSide==-1) ? 0 : nGsu / 2 + 1;
		long nEttGir = (nSide==0 || nSide== 1) ? nGsu : nGsu / 2;
		if(nGsu%2==0)
		{
			nSttGir = (nSide==0 || nSide==-1) ? 0 : nGsu / 2;
			nEttGir = (nSide==0 || nSide== 1) ? nGsu : nGsu / 2;
		}
		// �Ŵ����� Ȧ���϶��� �߽ɰŴ��� �������� ����, ����, �߽ɰŴ��� ���� �׸�.
		// �߽ɰŴ�
		if(nGsu%2)	
		{
			long nG = nGsu/2;
			// �� �������� BX����
			pBxStt = GetPageBxStt(nG,nSubPage);
			pBxEnd = GetPageBxEnd(nG,nSubPage);	
			// �÷��� �׸��� ------------------------------------------------------------
			CPlateBxFinder	Finder(pBridge->GetGirder(nG));
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | (nSide==-1 ? BX_UPPERFLANGE : BX_LOWERFLANGE),pBxStt,pBxEnd);
			if(pBx != pBxStt)
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), nSide, 0, TRUE);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), nSide, nLine);	
			}
			while(Finder.GetBxFixNext())
			{
				DrawPyung.DrawGirderWithenBx   (&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), nSide, 0, TRUE);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), nSide, nLine);	
				pBx = Finder.GetBxNext();
			}
			if(pBx != pBxEnd)
			{
				DrawPyung.DrawGirderWithenBx   (&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), nSide, 0, TRUE);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), nSide, nLine);	
			}
			// ����������ġ--------------------------------------------------------------
			if(bFact)
			{
				BOOL bUpper = nSide==-1 ? TRUE : FALSE;	// �߽ɰŴ��� ��ι���, �Ϻι����� �׸�.
				if(bUpper)
				{
					DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_U, nSide, TRUE);	// ��� ����������ġ 
			//		DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_W, 2, TRUE);		// ���� ����������ġ(�Ϲݵ� ��鿡���� �ȱ׸���� ��)
				}
				else
					DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_L, nSide, TRUE);	// �Ϻ� ����������ġ
	
			}
			// ����������ġ--------------------------------------------------------------
			if(bSplice)
			{
				DrawPyung.DrawSpliceCut  (&Dom, pBxStt, pBxEnd,	-1, TRUE, FALSE);		// ����������ũ	���
				DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd, -1, TRUE, FALSE, TRUE, FALSE, 99);		// ���������� ���

				DrawPyung.DrawSpliceCut  (&Dom, pBxStt, pBxEnd,	 1, FALSE, TRUE);		// ����������ũ �Ϻ�
				DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd,  1, FALSE, FALSE, TRUE, FALSE, 99);		// ���������� �Ϻ�
			}
			// ��� ������ --------------------------------------------------------------
			if(bJackUp)
				DrawPyung.DrawJackUpWithenBx(&Dom, pBxStt, pBxEnd);
			// �����߽ɼ�, ����������, ���������� ---------------------------------------------------------
		//	DrawPyung.DrawCenterLine (&Dom, nG, pBxStt->GetStation()-wing*1.5, pBxEnd->GetStation()+wing/3,FALSE,TRUE,FALSE,TRUE,TRUE);								
			DrawPyung.DrawVStiffWithenBx(&Dom, pBxStt, pBxEnd);
			DrawPyung.DrawJiJumStiffWithenBx(&Dom, pBxStt, pBxEnd);
		}

		for(long nG=nSttGir; nG<nEttGir; nG++)
		{
			CPlateBxFinder	Finder(pBridge->GetGirder(nG));
			// �� �������� BX����
			pBxStt = GetPageBxStt(nG,nSubPage);
			pBxEnd = GetPageBxEnd(nG,nSubPage);	
			// �÷��� �׸��� ------------------------------------------------------------
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | (nSide==-1 ? BX_UPPERFLANGE : BX_LOWERFLANGE), pBxStt, pBxEnd);				
			if(pBx != pBxStt)
			{
				DrawPyung.DrawGirderWithenBx   (&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), 0, 0, TRUE);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), 0, nLine);					
			}
			while(Finder.GetBxFixNext())
			{					
				DrawPyung.DrawGirderWithenBx   (&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), 0, 0, TRUE);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), 0, nLine);	
				pBx = Finder.GetBxNext();
			}			
			if(pBx != pBxEnd)
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), 0, 0, TRUE);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), 0, nLine);
			}
			// ����������ġ--------------------------------------------------------------
			if(bFact)
			{
				BOOL bUpper = nSide==-1 ? TRUE : FALSE;
				long nSideFact = nSide==-1 ? -2 : 2;	// ������ �׸��� ���� �� �׸�.
				if(bUpper)
				{
					DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_U, nSideFact, TRUE);	// ��� ����������ġ 
			//		DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_W, 2, TRUE);			// ���� ����������ġ
				}
				else
					DrawPyung.DrawFlangeCut(&Dom, pBxStt, pBxEnd, G_F_L, nSideFact, TRUE);	// �Ϻ� ����������ġ

			}
			// ����������ġ--------------------------------------------------------------
			if(bSplice)
			{
				DrawPyung.DrawSpliceCut  (&Dom, pBxStt, pBxEnd, 0, (nSide==-1 ? TRUE : FALSE), TRUE);	// ����������ũ
				DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd, 0, (nSide==-1 ? TRUE : FALSE), FALSE);	// ����������
			}
			// ��� ������ --------------------------------------------------------------
			if(bJackUp)
				DrawPyung.DrawJackUpWithenBx(&Dom, pBxStt, pBxEnd);
			// ���÷���Ʈ  ---------------------------------------------------------
			if(bSolePlate && nSide==1)
				DrawPyung.DrawSolePlate(&Dom, pBxStt, pBxEnd, 0, nLine, FALSE, FALSE);
			// �����߽ɼ�, ����������, ���������� ---------------------------------------------------------
		//	DrawPyung.DrawCenterLine (&Dom, nG, pBxStt->GetStation(), pBxEnd->GetStation(),FALSE,FALSE,FALSE,FALSE,TRUE);		
			DrawPyung.DrawVStiffWithenBx(&Dom, pBxStt, pBxEnd);
			DrawPyung.DrawJiJumStiffWithenBx(&Dom, pBxStt, pBxEnd);
		}		
	}			
	if(nGsu%2==0)	// �Ŵ��� ¦���� �� ���
		DrawPyung.DrawTopBottomViewMark(&Dom, pBxStt, pBxEnd);
	// ġ����ġ����
	pPageMng->SetProgressData("��鵵 ����� (ġ����)... ",10);
	AddDimPlan(&Dom, nSubPage);

	pBxStt = GetPageBxStt(CENTERGIRDER,nSubPage);	
	pBxEnd = GetPageBxEnd(CENTERGIRDER,nSubPage);	
	
	double rotAng  = DrawPyung.GetRotateAnglePyung(pBxStt,pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	Dom.Move(-MoveXY);
	Dom.Rotate(rotAng,0,0);			
	
	*pDom << Dom;	
}

void CAPlateOutGeneral::AddDimPlan(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	CDomyun Dom(pDomP);
	pOpt->SetEnvType(&Dom, HCAD_DIML);	

	BOOL bStartPage     = (nSubPage ==1) ? TRUE : FALSE;
	BOOL bLastPage		= (nSubPage==m_nPage-1) ? TRUE : FALSE;
	BOOL bGirderNumber  = FALSE;

	if(pDomOpt->m_Gen_bDimCenterGirder && pDomOpt->m_Gen_bDimNormalGirder) bGirderNumber = TRUE;
	if(pDomOpt->m_Gen_nPyungDim == 0) bGirderNumber = TRUE;
	// ���ġ��
	long nSttDanOrg   = 1;
	long nSttDanUpper = nSttDanOrg;
	long nDanUpper = GetDimDan(pDomOpt->m_GenDim_Plan_Upper);
	if(pDomOpt->m_Gen_bDimCenterGirder)
	{
		CPlateGirderApp	*pGir    = pBridge->GetGirder(-1);
		CPlateBasicIndex *pBxStt = GetPageBxStt(-1, nSubPage);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(-1, nSubPage);

		CAPlateDrawDetailDim DimGenUpper(pStd, &Dom, pBxStt, pBxEnd);
		DimGenUpper.SetPyungDimDan(nSttDanUpper,TRUE);
		DimGenUpper.DimTotal(&Dom,pDomOpt->m_GenDim_Plan_Upper,TRUE,TRUE,TRUE,TRUE,bStartPage,bLastPage,bGirderNumber);
		nSttDanUpper += nDanUpper;
	}
	long nG = 0;
	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		if(!pDomOpt->m_Gen_bDimNormalGirder) break;
		if(pDomOpt->m_Gen_nPyungDim==1 && nG>0) break;
		CPlateGirderApp	*pGir   = pBridge->GetGirder(nG);
		CPlateBasicIndex *pBxStt = GetPageBxStt(nG, nSubPage);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG, nSubPage);
		CAPlateDrawDetailDim DimGenUpper(pStd, &Dom, pBxStt, pBxEnd);
		DimGenUpper.SetPyungDimDan(nSttDanUpper,TRUE);
		DimGenUpper.DimTotal(&Dom,pDomOpt->m_GenDim_Plan_Upper,TRUE,TRUE,TRUE,TRUE,bStartPage,bLastPage,bGirderNumber);
		nSttDanUpper += nDanUpper;
	}

	// �ϸ�ġ��
	long nSttDanLower = 1;
	long nDanLower = GetDimDan(pDomOpt->m_GenDim_Plan_Lower);
	if(pDomOpt->m_Gen_bDimCenterGirder)
	{
		CPlateGirderApp	*pGir   = pBridge->GetGirder(-1);
		CPlateBasicIndex *pBxStt = GetPageBxStt(-1, nSubPage);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(-1, nSubPage);
		CAPlateDrawDetailDim DimGenLower(pStd, &Dom, pBxStt, pBxEnd);
		DimGenLower.SetPyungDimDan(nSttDanLower,FALSE);
		DimGenLower.DimTotal(&Dom,pDomOpt->m_GenDim_Plan_Lower,FALSE,FALSE,TRUE,TRUE,bStartPage,bLastPage,bGirderNumber);
		nSttDanLower += nDanLower;
	}
	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		if(!pDomOpt->m_Gen_bDimNormalGirder) break;
		if(pDomOpt->m_Gen_nPyungDim==1 && nG<pBridge->GetGirdersu()-1) continue;
		CPlateGirderApp	*pGir   = pBridge->GetGirder(nG);
		CPlateBasicIndex *pBxStt = GetPageBxStt(nG, nSubPage);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG, nSubPage);
		CAPlateDrawDetailDim DimGenLower(pStd, &Dom, pBxStt, pBxEnd);
		DimGenLower.SetPyungDimDan(nSttDanLower,FALSE);
		DimGenLower.DimTotal(&Dom,pDomOpt->m_GenDim_Plan_Lower,FALSE,FALSE,TRUE,TRUE,bStartPage,bLastPage,bGirderNumber);
		nSttDanLower += nDanLower;
	}

	if(pDomOpt->m_Gen_bPyungJijumStation)
	{
		long nSttDanOrg = 0;
		CPlateGirderApp *pGirL = pBridge->GetGirder(0);
		CPlateBasicIndex* pBx = pGirL->GetBxOnJijum(0);
		double distDim = pGirL->GetWidthOnStation(pBx->GetStation(), TRUE)/2;
		CPlateBasicIndex *pBx1 = GetPageBxStt(0, nSubPage);
		CPlateBasicIndex *pBx2 = GetPageBxEnd(0, nSubPage);
		long nDimDan   = nSttDanUpper - nSttDanOrg;
		double Scale   = Dom.GetScaleDim();
		double dExo	   = Dom.GetDimExo();
		double vLen	   = Dom.GetDimVLen();
		double dLevel  = Dom.GetDimLevelDis();
		double dHeight = (vLen+dLevel*(nDimDan-nSttDanOrg+2))*Scale;
//		double dHeight = (vLen+dLevel*(nDanUpper))*Scale;
		double distExo = distDim + (dExo+vLen+dLevel*(nSttDanOrg-2))*Scale;
		DimPyungJijumText(&Dom, pBx1, pBx2, dHeight, distExo, TRUE);
	}

//	CPlateGirderApp	*pGir   = pBridge->GetGirder(-1);
	CPlateBasicIndex *pBxStt = GetPageBxStt(-1, nSubPage);
	CPlateBasicIndex *pBxEnd = GetPageBxEnd(-1, nSubPage);
	if(pDomOpt->m_Gen_bPyungLineInfo)
		DimTextLineInfo(&Dom, pBxStt, pBxEnd);

	// Ⱦ�� ġ��
	DrawDimPyung.DrawDimHDan(&Dom, pBxStt, TRUE, TRUE, 0);
	DrawDimPyung.DrawDimHDan(&Dom, pBxEnd, FALSE, TRUE, 0);

/*	// Ⱦ�� ġ��(Ȯ������ġ����)
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx1 = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt,pBxEnd);
	CPlateBasicIndex *pBx2 = Finder.GetBxLast(BX_CROSSBEAM_VBRACING,pBxEnd,pBxStt);
	if(pBx1)
		pDBBase->DimPyungHDanExt(&Dom,pBx1,TRUE,nSttDanOrg);
	if(pBx2)
	{
		long nDan = nSttDanOrg;
		for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGirder = pBridge->GetGirder(nG);
			CPlateBxFinder BxFinder(pGirder);
			CPlateBasicIndex *pBx3 = BxFinder.GetBxLast(BX_CROSSBEAM_VBRACING,pBxEnd,pBxStt);
			CString str = pGirder->GetRadiusString(pBx3->GetStation(), 0);
			if(str.GetLength()!=0)
			{
				nDan = nSttDanOrg+2;
				break;
			}
		}
		pDBBase->DimPyungHDanExt(&Dom,pBx2,FALSE,nDan);
	}*/

	// Match Line
	if(pDomOpt->m_Gen_bMatchLine)
	{
		long nG = -1;
		CPlateBasicIndex *pBxStt = GetPageBxStt(nG, nSubPage);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG, nSubPage);
		long nMLDanUp	   = nSttDanUpper;
		long nMLDanLow	   = nSttDanLower;
		long nRealPage = pDomOpt->m_Gen_bDivPlanJong ? nSubPage-(m_nPage/2) : nSubPage;
//		if(pDomOpt->m_Gen_bPyungJijumStation)	nMLDanUp+=3;

		if(!pBxStt->IsState(BX_STT_GIRDER))
			DimTextPyungMatchLine(&Dom, pBxStt, nRealPage-1, nMLDanUp, nMLDanLow);
		if(!pBxEnd->IsState(BX_END_GIRDER))
			DimTextPyungMatchLine(&Dom, pBxEnd, nRealPage  , nMLDanUp, nMLDanLow);
	}

	// �ɺ�,��ũ
	pOpt->SetEnvType(&Dom, HCAD_SYMB);

	DimTextMarkPlan(&Dom, nSubPage);
	DrawPyung.DrawGirderNumber(&Dom, pBxEnd);// ���� �ɺ�

	*pDomP << Dom;
}

void CAPlateOutGeneral::AddJong(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();

	CAPlateDrawJong DrawJong(pStd->GetDataManage());	
	CAPlateDrawDimJong DrawDimJong(pStd->GetDataManage());	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	
	CPlateBasicIndex *pBxStt, *pBxEnd;
	CString str = _T("");

	double dMoveGirderY = 0;
	BOOL bLevel = pStd->GetDomOptionStd()->m_Gen_bJongLevel;
	long nDir   = 0;
	long nLine  = pDomOpt->m_Gen_bBujeThick ? 2 : 0;
	long nGStt  = pDomOpt->m_Gen_bJongCenterGir ? -1 : 0;
	long nGEnd  = pDomOpt->m_Gen_bJongGenGir ? pBridge->GetGirdersu()-1 : -1;
	if (!pDomOpt->m_Gen_bJongCenterGir && !pDomOpt->m_Gen_bJongGenGir)
		return;
	
	if(pDomOpt->m_Gen_bJongGenGir && pDomOpt->m_Gen_nOutGirder==0)
	{
		if(pDomOpt->m_Gen_bJongCenterGir)
			nGEnd = pBridge->GetNumberGirderNearestToCenterLine();
		else
			nGStt = nGEnd = pBridge->GetNumberGirderNearestToCenterLine();
	}
	BOOL bVStiff	= pDomOpt->m_Gen_JongBuje & DRAW_JONG_VSTIFF    ? TRUE : FALSE;
	BOOL bSplice	= pDomOpt->m_Gen_JongBuje & DRAW_JONG_SPLICE    ? TRUE : FALSE;
	BOOL bJackUp	= pDomOpt->m_Gen_JongBuje & DRAW_JONG_JACKUP    ? TRUE : FALSE;
	BOOL bHStiff	= pDomOpt->m_Gen_JongBuje & DRAW_JONG_HSTIFF    ? TRUE : FALSE;
	BOOL bSlabLine	= pDomOpt->m_Gen_JongBuje & DRAW_JONG_SLAB      ? TRUE : FALSE;

	for(long nG=nGStt; nG<=nGEnd; nG++)
	{
		if(pDomOpt->m_Gen_bJongCenterGir && pDomOpt->m_Gen_nOutGirder==0)
		{
			if(nG != -1 && nG != pBridge->GetNumberGirderNearestToCenterLine())
				continue;
		}

		CPlateGirderApp	*pGir   = pBridge->GetGirder(nG);
		pBxStt = GetPageBxStt(nG, nSubPage);
		pBxEnd = GetPageBxEnd(nG, nSubPage);
	
		CDomyun Dom(pDom);
		pOpt->SetEnvType(&Dom,HCAD_STLC);
		pGir->SetEleLevel(bLevel);		// ���ܱ��踦 ������ ����

		DrawJong.DrawGirderLineBx (&Dom, pBxStt, pBxEnd, nLine, 0, 1, 1, TRUE);
		DrawJong.DrawJiJumVStiffBx(&Dom, pBxStt, pBxEnd,nLine);
		DrawJong.DrawInOutGateBx  (&Dom, pBxStt, pBxEnd, TRUE);
		DrawJong.DrawHGuessetBx   (&Dom, pBxStt, pBxEnd);
		DrawJong.DrawCrossBeamBrkFlangeBx(&Dom, pBxStt, pBxEnd);
		if(bHStiff)
			DrawJong.DrawHStiffBx(&Dom,pBxStt,pBxEnd,nLine);
		if(bVStiff)
			DrawJong.DrawVStiffBx(&Dom,pBxStt,pBxEnd,nLine);
		if(bJackUp)
			DrawJong.DrawJackUpBx(&Dom,pBxStt,pBxEnd,nLine);
		if(pDomOpt->m_Gen_bJijumSymbol)
			DrawPierJong(&Dom,pBxStt,pBxEnd);
		if(bSlabLine)
			DrawJong.DrawPaveLine(&Dom,pBxStt,pBxEnd,nDir);
		if(bSplice)
		{
			pOpt->SetEnvType(&Dom, HCAD_STLC);
			DrawJong.DrawFieldSpliceHatchJong(&Dom,pBxStt,pBxEnd,nDir,FALSE,TRUE);
		}		
		DrawJong.DrawFactJongBx(&Dom, pBxStt, pBxEnd, TRUE, TRUE);

		AddDimJong(&Dom, nSubPage, nG);

		pGir->SetEleLevel(FALSE);	// ���ܱ��踦 ���� ����

		Dom.SetCalcExtRect();
		CDPoint MoveXY(0,0);
		MoveXY.x = pBxStt->GetStation();
		MoveXY.y = Dom.GetExtRect().bottom;
		Dom.Move(-MoveXY.x, -MoveXY.y);

		double MoveX	= Dom.Always(190);
		double MoveY	= Dom.Always(250)-dMoveGirderY;
		Dom.Move(MoveX, MoveY);
		CDRect rect;
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect ();
		dMoveGirderY += rect.Height();

		*pDom << Dom;
	}
	
}


void CAPlateOutGeneral::AddDimJong(CDomyun *pDomP, long nSubPage, long nG)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp *pGir       = pBridge->GetGirder (nG);
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd	    *pDomOpt       = pStd->GetDomOptionStd();

	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	
	pGir->SetEleLevel(pDomOpt->m_Gen_bJongLevel);
	CPlateBasicIndex *pBxStt = GetPageBxStt(nG, nSubPage);
	CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG, nSubPage);

	long nSttDimDan = 2;
	BOOL bStartPage = (nSubPage == 1) ? TRUE : FALSE;
	BOOL bLastPage = (nSubPage==m_nPage-1) ? TRUE : FALSE;

	CAPlateDrawDetailDim DimJong(pStd, &Dom, pBxStt, pBxEnd);
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER);
	if(pBx!=pBxStt)
	DimJong.DimDetailSttJong(&Dom, pBxStt, pBxEnd, FALSE, FALSE, 0, FALSE);

	pBx = Finder.GetBxFirst(BX_END_GIRDER);
	if(pBx!=pBxEnd)
		DimJong.DimDetailEndJong(&Dom, pBxStt, pBxEnd, FALSE, FALSE);

	DimJong.SetJongDimDan(nSttDimDan, TRUE);
	DimJong.DimTotal(&Dom, pDomOpt->m_GenDim_Jong_Upper,TRUE,FALSE,FALSE,TRUE,bStartPage,bLastPage);
	DimJong.SetJongDimDan(nSttDimDan, FALSE);
	DimJong.DimTotal(&Dom, pDomOpt->m_GenDim_Jong_Lower,FALSE,FALSE,FALSE,TRUE,bStartPage,bLastPage);

	////////////////////////////////
//	DimJongInOutGate(&Dom,pBxStt, pBxEnd);
	DimJong.DimTextJongTotal(&Dom, pBxStt, pBxEnd);

	if(pDomOpt->m_Gen_bJongJijumStation)
	{
		long nDimDan   = GetDimDan(pDomOpt->m_GenDim_Jong_Upper);
		double Scale   = Dom.GetScaleDim();
		double dExo	   = Dom.GetDimExo();
		double vLen	   = Dom.GetDimVLen();
		double dLevel  = Dom.GetDimLevelDis();
		double dHeight = (vLen+dLevel*(nDimDan))*Scale;
		double distExo = (dExo+vLen+dLevel*(nSttDimDan-2))*Scale;
		double dMaxEL  = pGir->GetMaxPlanEL(pBxStt, pBxEnd);
		double dSttEL  = pGir->GetMinPlanEL(pBxStt, pBxEnd);
		
		DimJongJijumText(&Dom,pBxStt,pBxEnd,dHeight,distExo+(dMaxEL-dSttEL),TRUE,TRUE);
	}

	pOpt->SetEnvType(&Dom, HCAD_SYMB);
	DimJong.DimJongSectionMark(&Dom, pBxStt, pBxEnd, TRUE, 0, pDomOpt->m_bMarkAlwaysInCircle);

	if(pDomOpt->m_Gen_bMatchLine)
	{
		long nMLDan = GetDimDan(pDomOpt->m_GenDim_Jong_Upper)+nSttDimDan+1;
		DimTextJongMatchLine(&Dom, nSubPage, nG, nMLDan);
	}

	pGir->SetEleLevel(FALSE);

	*pDomP << Dom;
}

void CAPlateOutGeneral::AddFront(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	long nG = pBridge->GetGirdersu()-1;
	CPlateGirderApp	*pGir    = pBridge->GetGirder(nG);
	CPlateBasicIndex *pBxStt = GetPageBxStt(nG, nSubPage);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt);

	CAPlateDrawDanmyun Draw(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pStd->GetDataManage());

	CDomyun Dom(pDom);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);

	Draw.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, FALSE);	
	Draw.DrawGuardWall(&Dom,pBx);	
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	
	Draw.DimTextSlopeArrow(&Dom, pBx);
	Draw.DimSlabUpper(&Dom, pBx, FALSE);	
	Draw.DimTypicalSectionBottom(&Dom, pBx, 0, TRUE);
	Draw.DimHeightGirderAll(&Dom, pBx, FALSE, TRUE, TRUE);

	// �������� �̵�,ȸ�� , AddFront
	Dom.Mirror(0,TRUE);
 	Dom.Rotate(CDPoint(0,0),CDPoint(0,1));		

	*pDom << Dom;
}

void CAPlateOutGeneral::AddHDanJiJum(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CAPlateDrawDanmyun DrawDanmyun(pBridge->m_pDataManage);
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);

	CPlateBasicIndex *pBx = (CPlateBasicIndex *)pBridge->GetGirder(0)->GetBxOnJijum(0);
	DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, FALSE, FALSE);
	DrawDanmyun.DrawGuardWall(&Dom, pBx);	
	DrawDanmyun.DrawPierPlace(&Dom, pBx, TRUE);

	// �Ŵ����� ġ���� =====================================================================================
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CPlateGirderApp	*pGir = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	double dDisSlabRight  = pBridge->GetGirder(pBridge->GetGirdersu()-1)->m_dGirderCenter + pGir->GetWidthFlangeUpper(pBx)/2;
	CPlateBasicIndex *pBxMatchRightEnd  = pBx->GetBxMatchByCrossBeam(pGir);
	Dom.SetDirection("RIGHT");
	double dYSttPos = 0;
	double dHeightGir = pBxMatchRightEnd->GetGirder()->GetHeightGirderBySta(pBxMatchRightEnd->GetStation()) + pGir->GetThickFlangeByBase(pBxMatchRightEnd->GetStation()) + pGir->GetThickFactByStation(pBxMatchRightEnd->GetStation(), G_F_L);
	if(pBx->IsDanbuJijumStt())
		dHeightGir -= pBridge->m_dHeightTaperStt;
	else if(pBx->IsDanbuJijumEnd())
		dHeightGir -= pBridge->m_dHeightTaperEnd;
	if(pBx->IsDanbuJijumStt())
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir - pBridge->m_dHeightTaperStt;
	else if(pBx->IsDanbuJijumEnd())
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir - pBridge->m_dHeightTaperEnd;
	else
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir;

	Dom.DimMoveTo(dDisSlabRight, dYSttPos, 0);
	Dom.DimLineTo(dHeightGir, 0, COMMA(dHeightGir));
	// ======================================================================================================
	// ġ����
	Dom.SetDirection("TOP");
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	

	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
	DrawDanmyun.DimSlabUpper(&Dom, pBx, FALSE);
	DrawDanmyun.DimTypicalSectionBottom(&Dom, pBx);	

	Dom.SetCalcExtRect();
	CDPoint MoveXY(0,0);
	MoveXY.x = pBridge->GetGirder(0)->m_dGirderCenter;
	MoveXY.y = 0;
	Dom.Move(-MoveXY.x, -MoveXY.y);
	*pDom << Dom;	

}

void CAPlateOutGeneral::AddHDanVBracing(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove	 = pStd->GetMovePoint(pStd->m_nCurPage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CAPlateDrawDanmyun DrawDanmyun(pBridge->m_pDataManage);
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);

	CPlateBxFinder Finder(pBridge->GetGirder(0));	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec->IsenDetType(SECDET_VBRACING)) break;			
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	if(!pBx) return;
	
	DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, FALSE, FALSE);		
	DrawDanmyun.DrawGuardWall(&Dom, pBx);		
	DrawDanmyun.DrawPierPlace(&Dom, pBx, TRUE);
	// �Ŵ����� ġ���� =====================================================================================
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CPlateGirderApp	*pGir = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	double dDisSlabRight  = pBridge->GetGirder(pBridge->GetGirdersu()-1)->m_dGirderCenter + pGir->GetWidthFlangeUpper(pBx)/2;
	CPlateBasicIndex *pBxMatchRightEnd  = pBx->GetBxMatchByCrossBeam(pGir);
	Dom.SetDirection("RIGHT");
	double dYSttPos = 0;
	double dHeightGir = pBxMatchRightEnd->GetGirder()->GetHeightGirderBySta(pBxMatchRightEnd->GetStation()) + pGir->GetThickFlangeByBase(pBxMatchRightEnd->GetStation()) + pGir->GetThickFactByStation(pBxMatchRightEnd->GetStation(), G_F_L);
	if(pBx->IsDanbuJijumStt())
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir - pBridge->m_dHeightTaperStt;
	else if(pBx->IsDanbuJijumEnd())
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir - pBridge->m_dHeightTaperEnd;
	else
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir;

	Dom.DimMoveTo(dDisSlabRight, dYSttPos, 0);
	Dom.DimLineTo(dHeightGir, 0, COMMA(dHeightGir));
	// ======================================================================================================
	// ġ����
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	

	Dom.SetDirection("TOP");
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
	DrawDanmyun.DimSlabUpper(&Dom, pBx, FALSE);
	DrawDanmyun.DimTypicalSectionBottom(&Dom,pBx);	

	Dom.SetCalcExtRect();
	CDPoint MoveXY(0,0);
	MoveXY.x = pBridge->GetGirder(0)->m_dGirderCenter;
	MoveXY.y = 0;
	Dom.Move(-MoveXY.x, -MoveXY.y);
	*pDom << Dom;
}

void CAPlateOutGeneral::AddHDanHGirder(CDomyun *pDom)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGirL		= pBridge->GetGirder(0);
	CPlateGirderApp	*pGirR		= pBridge->GetGirder(pBridge->GetQtyGirder()-1);
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CAPlateDrawDanmyun DrawDanmyun(pBridge->m_pDataManage);
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);

	CPlateBxFinder Finder(pGirL);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pBx->IsJijum() || pSec->GetenDetType() != SECDET_CROSSBEAM)
		{
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
			continue;
		}
		
		CPlateBasicIndex *pBxR	= pGirL->GetBxMatchByCrossBeam(pBx, pGirR);
		double dHTaperL = pGirL->GetHeightTaper(pBx->GetStation());
		double dHTaperR = pBxR ? pGirR->GetHeightTaper(pBxR->GetStation()) : dHTaperL;

		if(dHTaperL==0 && dHTaperR==0) break;
		
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	if(!pBx) return;

	DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, FALSE);
	DrawDanmyun.DrawGuardWall(&Dom, pBx);
	// �Ŵ����� ġ���� =====================================================================================
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CPlateGirderApp	*pGir = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	double dDisSlabRight  = pBridge->GetGirder(pBridge->GetGirdersu()-1)->m_dGirderCenter + pGir->GetWidthFlangeUpper(pBx)/2;
	CPlateBasicIndex *pBxMatchRightEnd  = pBx->GetBxMatchByCrossBeam(pGir);
	Dom.SetDirection("RIGHT");
	double dYSttPos = 0;
	double dHeightGir = pBxMatchRightEnd->GetGirder()->GetHeightGirderBySta(pBxMatchRightEnd->GetStation()) + pGir->GetThickFlangeByBase(pBxMatchRightEnd->GetStation()) + pGir->GetThickFactByStation(pBxMatchRightEnd->GetStation(), G_F_L);
	if(pBx->IsDanbuJijumStt())
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir - pBridge->m_dHeightTaperStt;
	else if(pBx->IsDanbuJijumEnd())
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir - pBridge->m_dHeightTaperEnd;
	else
		dYSttPos = pGir->GetElevationDom(pBxMatchRightEnd) - dHeightGir;

	Dom.DimMoveTo(dDisSlabRight, dYSttPos, 0);
	Dom.DimLineTo(dHeightGir, 0, COMMA(dHeightGir));
	// ======================================================================================================

	// ġ����
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	
	Dom.SetDirection("TOP");
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
	DrawDanmyun.DimSlabUpper(&Dom, pBx, FALSE);
	DrawDanmyun.DimTypicalSectionBottom(&Dom,pBx);	
	
	Dom.SetCalcExtRect();
	CDPoint MoveXY(0,0);
	MoveXY.x = pBridge->GetGirder(0)->m_dGirderCenter;
	MoveXY.y = 0;
	Dom.Move(-MoveXY.x, -MoveXY.y);
	*pDom << Dom;	
}

CPlateBasicIndex* CAPlateOutGeneral::GetPageBxStt(long nG, long nSubPage, BOOL bFrontMatch, BOOL bNextMatch)
{	 
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	BOOL bSplite	= pOpt->m_Gen_bDivPlanJong;
	long nPageSuOrg = bSplite ? GetPageSu()/2 : GetPageSu() - 1;
	long nPageOrg	= nSubPage%nPageSuOrg;
	if((nSubPage%nPageSuOrg)==0)
		nPageOrg = nPageSuOrg;

	CPlateBasicIndex *pBx = NULL;
	long nDivDom = pOpt->m_Gen_nDivPage;
	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum((nPageOrg-1)*pOpt->m_Gen_nUnitEaDraw);
			if(nPageOrg==1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BX_STT_GIRDER);
			}
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter((nPageOrg-1)*pOpt->m_Gen_nUnitEaDraw-1);
			if(nPageOrg==1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BX_STT_GIRDER);
			}
			break;
		case 2 :
			if(nPageOrg==1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BX_STT_GIRDER);
			}
			else
			{
				if(nG==-1){
					nG = 0;//pStd->m_pDrawDBStandard->GetNumberGirderNearestToCenterLine();
					pGir = pBridge->GetGirder(nG);
				}
				if(nG>pBridge->GetGirdersu()-2)
					pBx = pGir->m_pLeft->GetBxByCrossNo((nPageOrg-1)*pOpt->m_Gen_nUnitEaDraw)->GetBxMatchByCrossBeam(pGir);
				else
					pBx = pGir->GetBxByCrossNo((nPageOrg-1)*pOpt->m_Gen_nUnitEaDraw);
			}
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxFirst(BX_STT_GIRDER);
			break;
	}
	
	return pBx;
}


CPlateBasicIndex* CAPlateOutGeneral::GetPageBxEnd(long nG, long nSubPage, BOOL bFrontMatch, BOOL bNextMatch)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	BOOL bSplite	= pOpt->m_Gen_bDivPlanJong;
	long nPageSuOrg = bSplite ? GetPageSu()/2 : GetPageSu() - 1;
	long nPageOrg	= nSubPage%nPageSuOrg;
	if((nSubPage%nPageSuOrg)==0)
		nPageOrg = nPageSuOrg;

	CPlateBasicIndex *pBx = NULL;
	switch(pOpt->m_Gen_nDivPage)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum(nPageOrg * pOpt->m_Gen_nUnitEaDraw);
			if(nPageOrg==nPageSuOrg)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BX_END_GIRDER);
			}			
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter(nPageOrg * pOpt->m_Gen_nUnitEaDraw-1);
			if(nPageOrg==nPageSuOrg)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BX_END_GIRDER);
			}			
			break;
		case 2 :
			if(nPageOrg==nPageSuOrg)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BX_END_GIRDER);
			}			
			else
			{
				if(nG==-1){
					nG = 0;//pStd->m_pDrawDBStandard->GetNumberGirderNearestToCenterLine();
					pGir = pBridge->GetGirder(nG);
				}
				if(nG>pBridge->GetGirdersu()-2)
					pBx = pGir->m_pLeft->GetBxByCrossNo(nPageOrg*pOpt->m_Gen_nUnitEaDraw)->GetBxMatchByCrossBeam(pGir);
				else
					pBx = pGir->GetBxByCrossNo(nPageOrg*pOpt->m_Gen_nUnitEaDraw);
			}
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxLast(BX_END_GIRDER);
			break;
	}
	
	return pBx;
}
/*
void CAPlateOutGeneral::DrawAbutPier(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp	*pGir		= (CPlateGirderApp*)pBxStt->GetGirder();
	CAPlateDrawJong DrawJong(pStd->GetDataManage());	

	CDimDomyunEx Dom(pDomP);

	INTFLAG BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	double staStd = pBxStt->GetStation();
	double staCur = pBxStt->GetStation();
	double Length = 0;
	CDPoint xy;

	CString sMark = _T("");
	BOOL bAbut = FALSE;
	while(pBx) 
	{
		EXIT_RTN;
		bAbut  = FALSE;
		if(pBx->IsState(BX_STT_GIRDER))
		{
			sMark  = pBridge->GetMarkSection(SECTION_MAIN,pGir->GetBxOnJijum(0));
			bAbut  = (sMark.Find("A")!=-1) ? TRUE : FALSE;
			if(bAbut)
				staCur = pGir->GetStationStt();
			else
				staCur = pGir->GetBxOnJijum(0)->GetStation();
		}
		else if(pBx->IsState(BX_END_GIRDER))
		{
			sMark  = pBridge->GetMarkSection(SECTION_MAIN,pGir->GetBxOnJijum(pBridge->m_nQtyJigan));
			bAbut  = (sMark.Find("A")!=-1) ? TRUE : FALSE;
			if(bAbut)
				staCur = pGir->GetStationEnd();
			else
				staCur = pGir->GetBxOnJijum(pBridge->m_nQtyJigan)->GetStation();
		}
		else if(pBx->IsPier())
			staCur = pBx->GetStation();
		else 
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		Length = pGir->GetLengthDimType(staStd,staCur);
		if(staStd>staCur) Length *= -1.0;

		xy.x = staStd + Length;
		if(bAbut)
			xy.y = pGir->GetElevationFlangeCamber(staCur) + pGir->GetThickSlabPave(staCur);
		else
		{			
			xy.y = pGir->GetElevationFlangeCamber(staCur,0,CDPoint(0,0),FALSE) 
				 - pGir->GetThickJewon(G_F_L,pBx)
			     - pGir->GetShoe(pBridge->m_nQtyJigan)->GetHeightTotalShoe();
		}

		BOOL bMark = TRUE;
		BOOL bPierSymbol	= FALSE;//pStd->GetDomOptionStd()->m_bGenDimJong_PierSymbol;
		if (bPierSymbol)
			bMark = FALSE;
		if(pBx->IsState(BX_STT_GIRDER) || pBx->IsState(BX_END_GIRDER) || pBx->IsPier())
			DrawJong.DrawPier(&Dom,pGir->GetNumberJijumByBx(pBx),pBx->GetNumberGirder(),xy,TRUE,bMark, TRUE);

		pBx = Finder.GetBxNext();
	}

	*pDomP << Dom;		
}
*/
void CAPlateOutGeneral::InitNote()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();			
	CSteelSection   *pSteelSec  = pStd->GetDataManage()->GetSteelSection();
	
	long TotalBujemarkSu = TOTAL_PIECE_VALUE;
	long nBujeMark = 0;
	CString	strBuje1 = "�ֺ���";
	long n = 0;
	for (n=0; n < TotalBujemarkSu; n++)
	{
		if (strBuje1 == pSteelSec->GetClass(n))
		{
			nBujeMark = n;
			break;
		}
	}	
	strBuje1 = strBuje1+"("+pSteelSec->GetMaterialByMarkBuje(nBujeMark)+")";

	CString	strBuje2 = "�κ���";
	for (n=0; n < TotalBujemarkSu; n++)
	{
		if (strBuje2 == pSteelSec->GetClass(n))
		{
			nBujeMark = n;
			break;
		}
	}
	strBuje2 = strBuje2+"("+pSteelSec->GetMaterialByMarkBuje(nBujeMark)+")";

	m_sNoteArr.RemoveAll();
	m_sNoteArr.Add(".PLATE GIRDER ���۽� ��鼱������ ���ܰ�縦 �����Ͽ�");
	m_sNoteArr.Add("�����Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".SOLE PLATE ���۽� GIRDER�� ���ܰ�縦 ����Ͽ�");
	m_sNoteArr.Add("�����Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".����������ġ ��ġ���� �ܸ��� ������ �������� ����");
	m_sNoteArr.Add("��ü�κ��� ���������� Ȯ���Ͽ� ������(������)�� ��");
	m_sNoteArr.Add("���Ͽ� ������ �� �ִ�");
	m_sNoteArr.Add(".�� �������� ���� �˻�� KSB-0845, KSB-0816�� ����");
	m_sNoteArr.Add("�ϴ� ������ �� �� �ֵ��� �Ѵ�");
	m_sNoteArr.Add("." + strBuje1 + "�� "+strBuje2+"�� �����ϴ� ����");
	m_sNoteArr.Add("���������� �Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".������ �����Ŀ� ������� �ǰų� �ܺο� �������� �Ǵ�");
	m_sNoteArr.Add("���ܸ��� ���� �ݰ� 1mm �̻��� �ձ����� �𼭸� ���⸦");
	m_sNoteArr.Add(" �ϰ�, ��Ÿ �������� �ݰ� 0.5mm�̻��� �ձٸ����� ����");
	m_sNoteArr.Add("�� �Ǵ� 0.5mm ������ ����⸦ �Ѵ�");
	m_sNoteArr.Add(".�ֺ����� Ȩ������ �ʷ������� ��ܿ��� �ݵ�� ��������");
	m_sNoteArr.Add("��ġ�Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".���� ����� �ջ��� ����� �ִ� ���� ���� Ȥ�� �ޱ۵�����");
	m_sNoteArr.Add("�߰��ϰ� �����Ͽ� ���簡 �ļյ��� �ʵ��� ���� �Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".���� ��ݽ� ��� ������ ������ �Ʊ����� �����ϰ�, ���");
	m_sNoteArr.Add("���߼ջ��� �����ʵ��� �Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".�ܸ鵵�� ġ���� ���̴� ���� �ϸ�� ���� ��� ������");
	m_sNoteArr.Add(".��� ����, �������� �� ���� ���α���� �����˻縦");
	m_sNoteArr.Add("���Ͽ����ϸ� �����˻� ������ ����, ��� �����ϰ� ������");
	m_sNoteArr.Add("�� ���������� �����Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".��� ġ���� mm�����̸�, 15%%dC �����̹Ƿ� ���� ���۽� ��");
	m_sNoteArr.Add("�������� ���Ͽ����Ѵ�");
	m_sNoteArr.Add(".��Ʈ ���յǴ� ���� ������ ���˸��� ��� ���̳� ���Ǵ�");
	m_sNoteArr.Add("����õ����� �����Ͽ����ϸ� ��ó�� ���ᳪ �츷�� ���Ḧ");
	m_sNoteArr.Add("�������� �ʾƾ� �Ѵ�");
	m_sNoteArr.Add(".SHOP DRAWING ���ν� ���赵���� ġ���� SHOP DRAWING");
	m_sNoteArr.Add("���� ġ���� �ݵ�� Ȯ�� �����ϰ� ���������� ���Ŀ�");
	m_sNoteArr.Add("���ؼ��� �����Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".���� ����� ���� ������ �۾����� ���� ���� ������ �߻���");
	m_sNoteArr.Add("�� �����Ƿ� ���� ��ȹ�� ����� �����Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".�������� ����");
	m_sNoteArr.Add("1) ���� ������� ���� �÷���, ���� ������� �ٴ����� ���ϴ�");
	m_sNoteArr.Add("   ���� �÷����� �����Ͽ��� �Ѵ�");
	m_sNoteArr.Add("2) �ż�Ʈ ����ο� ���� �÷����� �������� �ʰ� ���� ��Ų��");
	m_sNoteArr.Add("3) �������� ���� ������� �÷����� �����Ͽ��� �Ѵ�");
	m_sNoteArr.Add("4) ����� ��� ��� ���� ������� �÷����� �����Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".���� ��� �� ��ġ�� �ʿ��� LIFTING LUG�� ���� �����ڰ� ��ġ");
	m_sNoteArr.Add("�� ũ�⸦ �����Ͽ� ���۽� �ݿ��ؾ� �Ѵ�");
	m_sNoteArr.Add(".���� ������ ������ �ұ���, ����, ��ȸ � Ư�� ����");
	m_sNoteArr.Add("�Ͽ��� �Ѵ�");
	m_sNoteArr.Add(".���� ���۵� �ۼ��� ����⺻��, ����ù漭 �� ���α�ǥ��");
	m_sNoteArr.Add("�ù漭�� ��޵� ���뿡 ���Ͽ� �����ϵ� ����ġ �����̳�");
	m_sNoteArr.Add("��Ȯ�ϰ� ���Ǹ� �������� ���׵��� ���۵� �ۼ��߿� ����ó��");
	m_sNoteArr.Add("���� �� ���ΰ����� ���� Ȯ�� �� ���۽� �ݿ��Ǿ�� �Ѵ�");
}

void CAPlateOutGeneral::AddNote(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	//COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);	

	CDomyun Dom(pDomP);
	BOOL bVerticalNote = pOpt->m_Gen_bVerticalNote;

	if(bVerticalNote)
		DimNoteTypeVertical(&Dom);
	else
		DimNoteTypeHorizontol(&Dom);
	
	CDRect rect;
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.Move(-rect.left,-rect.top);
	Dom.Move(Dom.Always(630),Dom.Always(250));

	*pDomP << Dom;
}

void CAPlateOutGeneral::DimNoteTypeVertical(CDomyun *pDomP)
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
	Dom.SetColumnWidth(1,Dom.Always(130));
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

void CAPlateOutGeneral::DimNoteTypeHorizontol(CDomyun *pDomP)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;

	CGridDomyunEx Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetGridLines(GVL_NONE);
	long nRows = 15;
	long nCols =  8;
	Dom.SetColumnCount(nCols);
	Dom.SetRowCount(nRows+2);
	long j = 0;
	for(j = 0; j < nCols; j++)
	{
		if(j%2==0)
			Dom.SetColumnWidth(j,Dom.Always(12));
		else
			Dom.SetColumnWidth(j,Dom.Always(120));
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

BOOL CAPlateOutGeneral::CheckStation(double sta1, double sta2)
{
	BOOL bCheck = FALSE;
	for (long n=0; n < m_JweonInfoStaArr.GetSize(); n++)
	{
		double dStaPre = m_JweonInfoStaArr[n].x;
		double dStaEnd = m_JweonInfoStaArr[n].y;

		if((dStaPre >= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd >= sta2) || 
		   (dStaPre <= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd >= sta2) ||
		   (dStaPre <= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd <= sta2) ||
		   (dStaPre >= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd <= sta2))
			bCheck = TRUE;
	}

	return bCheck;
}

void CAPlateOutGeneral::TextOutJewonAndMat(CDomyun *pDomP, CDPoint xy,CString strBuje, CString strMat, BOOL bDirForward)
{
	double dLenStr  = max(pDomP->GetTextWidth(strBuje),pDomP->GetTextWidth(strMat));
	if(!bDirForward) dLenStr *= -1.0;

	pDomP->LineTo(xy.x,xy.y,xy.x+dLenStr,xy.y);
	if(strBuje.GetLength())
		pDomP->TextOut(xy.x+dLenStr/2,xy.y+pDomP->GetTextHeight()*1/4,strBuje);
	if(strMat.GetLength())
	{
		double dY = xy.y-pDomP->GetTextHeight()*5/4;
		if(strBuje.GetLength()==0)
			dY = xy.y+pDomP->GetTextHeight()*1/4;
		pDomP->TextOut(xy.x+dLenStr/2,dY,strMat);
	}
}

void CAPlateOutGeneral::DimTextPyungVStiff(CDomyun *pDomP, CPlateBasicIndex *pBxStt, 
										   CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi)
{
	/*
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
 	CPlateGirderApp	*pGir	    = pBxStt->GetGirder();	
	CSteelSection	*pSteelSec  = pStd->GetDataManage()->GetSteelSection();	

	CPlateBxFinder Finder(pGir);
//	CPlateBasicIndex *pBxStd = Finder.GetBxFirst(BX_STT_GIRDER|BX_SPLICE,pBxStt,pBxEnd,FALSE);
//	if(pBxStd==NULL) return;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF,pBxStt,pBxEnd);
	long nCount = Finder.GetCountBx();
	pBx = Finder.GetBxNext(nCount/2);
	CPlateBasicIndex *pBxold = NULL;
	if(pBx==NULL) return;

	CDomyun Dom(pDomP);

	CString strBuje	= "����������";
	CString strMat	= pSteelSec->GetMaterialByMarkBuje(B_V);
	double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));

	double sta  = pBx->GetStation();
	double sta2 = pGir->GetStationByActLength(sta, textLen, 0);
	pBxold = pBx;
	while (1)
	{
		if (CheckStation(sta, sta2))
		{
			while(pBx)
			{
				pBx = Finder.GetBxNext();
				if (pBx == NULL)
					break;
				else
					continue;
			}
			if (pBx == NULL)
				break;
			sta = pBx->GetStation();
			sta2 = pGir->GetStationByActLength(sta, textLen, 0);
		}
		else
			break;
	}
	if (pBx==NULL)
		pBx = pBxold;	

	long   nG = pBxStt->GetNumberGirder();
	double dEndDist = pGir->m_dWidth/2+dLenGisi;
	double dist = pGir->m_dWidth/2;
	sta  = pBx->GetStation();
	CDPoint xy1 = pGir->GetXyGirderDis(sta,dist,pBx->GetAngle());
	CDPoint xy2 = pGir->GetXyGirderDis(sta,dEndDist);
	m_JweonInfoStaArr.Add(CDPoint(sta,sta));

	double R = Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
	Dom.Circle(xy1,R,TRUE);
	Dom.LineTo(xy1,xy2);
	*pDomP << Dom;
	//
	strBuje	= "����������";
	strMat	= pSteelSec->GetMaterialByMarkBuje(B_V);
	TextOutJewonAndMat(&Dom,xy2,strBuje,strMat);
	Dom.Rotate(xy2,pGir->GetLine()->GetAngleAzimuth(sta));

	*pDomP << Dom;
	*/
}

// ���� : ���� ���������� ���� �������������(1~2)
// ���� : ���� ���������� ���� �������������(1~2)
void CAPlateOutGeneral::DimTextPyungFlange(CDomyun *pDomP, CPlateBasicIndex *pBxStt, 
										   CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();	
	CPlateGirderApp	*pGir		= pBxStt->GetGirder();	

	BOOL	bUpper		= (nUpper==1) ? TRUE : FALSE;
	long	nMarkBuje	= bUpper ? G_F_U : G_F_L;
	CString	szMaterial	= _T("");
	CString	strBuje		= bUpper ? "�� ��" : "�� ��";
	double	dEndDist	= dLenGisi;
	__int64	BxFlag		= bUpper ? BX_CROSSBEAM_VBRACING|BX_UPPERFLANGE : BX_CROSSBEAM_VBRACING|BX_LOWERFLANGE;

	CDomyun	Dom(pDomP);
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	while(pBx)
	{
		CPlateBasicIndex	*pBxNext	= Finder.GetBxFixNext();
		if(!pBxNext)	pBxNext=pBx;
		CPlateFactChain		*pFact		= pBx->GetFactChain(nMarkBuje);
		CPlateFactChain		*pFactNext	= pBxNext->GetFactChain(nMarkBuje);

		if(szMaterial != pFact->m_strSymbol)
		{
			double	R		= Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
			double	dStaGisi	= (pBx->GetStation()+pBxNext->GetStation())/2;
			CDPoint	xy1			= pGir->GetXyGirderDis(dStaGisi, 0,pBx->GetAngle());
			CDPoint	xy2			= pGir->GetXyGirderDis(dStaGisi, dEndDist);
			Dom.Circle(xy1,R,TRUE);
			Dom.LineTo(xy1,xy2);
			*pDomP << Dom;

			m_JweonInfoStaArr.Add(CDPoint(dStaGisi, dStaGisi));	
			TextOutJewonAndMat(&Dom,xy2,strBuje,pFact->m_strSymbol);
			Dom.Rotate(xy2,pGir->GetLine()->GetAngleAzimuth(dStaGisi));

			*pDomP << Dom;
		}
		szMaterial	= pFact->m_strSymbol;
		pBx = Finder.GetBxNext();
	}
/*
	__int64 BxFlag = (bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx1 = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	CPlateBasicIndex *pBx2 = pBxEnd;
	if(pBx1==NULL || pBx2==NULL) return;

	CDomyun Dom(pDomP);
	CDPoint xy1,xy2;
	double dist = 0;//GetDisCirclePoint(pBx1,nUpper);
	double sta1 = pBx1->GetStation();
	double sta2 = pBx2->GetStation();
	double staGisi = (sta1+sta2)/2;
	double staGisi2 = 0;

	long   nG = pBxStt->GetNumberGirder();
	double dEndDist = dLenGisi;

	CString strBuje	= _T("");
	CString strMat	= _T("");

	if(bUpper) 
	{
		CPlateBasicIndex *pBxS1 = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_UPPERFLANGE,pBx1,pBx2);
		CPlateBasicIndex *pBxS2 = Finder.GetBxNext();
		staGisi = (pBxS1->GetStation()+pBxS2->GetStation())/2;
		strBuje	= "�� ��";		
		strMat	= pSteelSec->GetMaterialByMarkBuje(G_F_U);		

		double R = Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
		//ǥ�ñ����� ��ġ������ CHECK!!!
		double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));
		staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
		double staGisiold = staGisi;
		double staGisi2old = staGisi2;
		CPlateBasicIndex *pBxS1old = pBxS1;
		CPlateBasicIndex *pBxS2old = pBxS2;
		while (1)
		{
			if (CheckStation(staGisi, staGisi2))
			{
				__int64 BxFlag2 = BX_CROSSBEAM_VBRACING | BX_VSTIFF;
				pBxS1 = pBxS2;
				pBxS2 = Finder.GetBxFirst(BxFlag2, pBxS1, pBx2, FALSE, FALSE);
				if (pBxS1==NULL || pBxS2==NULL)
					break;
				sta1 = pBxS1->GetStation();
				sta2 = pBxS2->GetStation();
				staGisi = (sta1+sta2)/2;
				staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
			}
			else
				break;
		}

		if (pBxS1==NULL || pBxS2==NULL)
		{
			staGisi = staGisiold;
			staGisi2 = staGisi2old;
			pBxS1 = pBxS1old;
			pBxS2 = pBxS2old;
		}
		xy1		= pGir->GetXyGirderDis(staGisi,dist,pBxS1->GetAngle());
		xy2		= pGir->GetXyGirderDis(staGisi,dEndDist);
	}
	else
	{
		CPlateBasicIndex *pBxS1 = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_VSTIFF|BX_LOWERFLANGE,pBx1,pBx2);
		CPlateBasicIndex *pBxS2 = Finder.GetBxNext();
		staGisi = (pBxS1->GetStation()+pBxS2->GetStation())/2;
		strBuje	= "�� ��";		
		strMat	=  pSteelSec->GetMaterialByMarkBuje(G_F_L);

		//ǥ�ñ����� ��ġ������ CHECK!!!
		double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));
		staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
		double staGisiold = staGisi;
		double staGisi2old = staGisi2;
		CPlateBasicIndex *pBxS1old = pBxS1;
		CPlateBasicIndex *pBxS2old = pBxS2;
		while (1)
		{
			if (CheckStation(staGisi, staGisi2))
			{
				__int64 BxFlag2 = BX_CROSSBEAM_VBRACING | BX_VSTIFF;
				pBxS1 = pBxS2;
				pBxS2 = Finder.GetBxFirst(BxFlag2, pBxS1, pBx2, FALSE, FALSE);
				if (pBxS1==NULL || pBxS2==NULL)
					break;
				sta1 = pBxS1->GetStation();
				sta2 = pBxS2->GetStation();
				staGisi = (sta1+sta2)/2;
				staGisi2 = pGir->GetStationByActLength(staGisi, textLen, 0);
			}
			else
				break;
		}

		if (pBxS1==NULL || pBxS2==NULL)
		{
			staGisi = staGisiold;
			staGisi2 = staGisi2old;
			pBxS1 = pBxS1old;
			pBxS2 = pBxS2old;
		}

		xy1		= pGir->GetXyGirderDis(staGisi,dist,pBxS1->GetAngle());
		xy2		= pGir->GetXyGirderDis(staGisi,dEndDist);
	}

	double R = Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
	Dom.Circle(xy1,R,TRUE);
	Dom.LineTo(xy1,xy2);
	*pDomP << Dom;
	
	m_JweonInfoStaArr.Add(CDPoint(staGisi, staGisi2));	
	TextOutJewonAndMat(&Dom,xy2,strBuje,strMat);
	Dom.Rotate(xy2,pGir->GetLine()->GetAngleAzimuth(staGisi));

	*pDomP << Dom;
	*/
}

void CAPlateOutGeneral::DimTextPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();	
	CPlateGirderApp	*pGir		= pBxStt->GetGirder();	
	
	BOOL bVStiffBuje	= (pOpt->m_Gen_BujeInfo & BUJEINFO_VSTIFF_NAME)		? TRUE : FALSE;
	BOOL bVStiffMat		= (pOpt->m_Gen_BujeInfo & BUJEINFO_VSTIFF_MAT)		? TRUE : FALSE;

	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);

	double LenGisi = Dom.Always(12);
	if(pBxStt->GetNumberGirder()==pBridge->GetGirdersu()-1)
		LenGisi = Dom.Always(6)+pBridge->m_dWidthSlabRight;

	m_JweonInfoStaArr.RemoveAll();

	//�������� ���� ��� 
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxSplice = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	while(pBxSplice)
	{
		double sta = pBxSplice->GetStation();
		if(sta > pBxEnd->GetStation()) sta = pBxEnd->GetStation(); 
		CPlateSplice	*pSp = pBxSplice->GetSplice();
		double W = (pSp->m_uA + pSp->m_uB * pSp->m_uC + pSp->m_uC1/2) * 2;
		CDPoint staxy;
		staxy.x = pGir->GetStationByActLength(sta, -W/2, 0);
		staxy.y = pGir->GetStationByActLength(sta,  W/2, 0);
		m_JweonInfoStaArr.Add(staxy);
		pBxSplice = Finder.GetBxNext();
	}
	if(bVStiffBuje || bVStiffMat)
	{
		SortSta();
		DimTextPyungFlange(&Dom, pBxStt, pBxEnd, nUpper,LenGisi);	// ������ 
	}

	if(bVStiffBuje || bVStiffMat)
	{
		SortSta();
		DimTextPyungVStiff(&Dom, pBxStt, pBxEnd, nUpper,LenGisi);   // ����������
	}

	*pDomP << Dom;	
}

void CAPlateOutGeneral::DimTextMarkPlan(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	
	CDomyun Dom(pDomP);

	long nUpDown   = 1;
	long nGirdersu = pBridge->GetGirdersu();

	for (long nG=0; nG < nGirdersu ; nG++)
	{
		CPlateBasicIndex *pBxStt = GetPageBxStt(nG,nSubPage);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG,nSubPage);

		if(nGirdersu%2 ==0)	
		{
			if(nG<=(nGirdersu/2-1)) nUpDown=1;
			else nUpDown=0; 
		}
		if(nGirdersu%2==1)
		{
			if(nG < (nGirdersu/2))	nUpDown=1; 
			else if (nG == (nGirdersu/2)) 
				nUpDown=-1;
			else nUpDown=0;
		}		
//		if(pOpt->m_Gen_bMarkCorss)
//			DimTextMarkCrossBeamPlan(pDom,pBxStt,pBxEnd,nUpDown);
//		if(pOpt->m_Gen_bMarkStringer)
//			DimTextMarkSeroboPlan(pDom,pBxStt,pBxEnd,nUpDown);

		DimTextPyungTotal(&Dom, pBxStt, pBxEnd, nUpDown);
	}

	*pDomP << Dom;	
}

void CAPlateOutGeneral::AddKeyPan(CDomyun *pDom, long nSubPage, double xDis, double yDis)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint*	pMove		= pStd->GetMovePoint(pStd->GetCurPage());	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CDomyun Dom(pDom);	
	
	double dKeyScaleHor = pStd->m_pDomOptionStd->m_Gen_Scale.GetAt(2);
	double dKeyScaleVer = pStd->m_pDomOptionStd->m_Gen_Scale.GetAt(3);

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	// �������
	CString sBridgeDir	= pDomOpt->m_sTextBridgeDirection;
	CString sCounterDir = pDomOpt->m_sTextCounterDirection;
	BOOL bSpliceMark = pDomOpt->m_Gen_bKeyPlanSpliceMark;
	BOOL bJijumMark	 = pDomOpt->m_Gen_bKeyPlanJijumMark;
	BOOL bCrossBeam  = pDomOpt->m_Gen_bKeyPlanCrossBeam;
	BOOL bDimension	 = pDomOpt->m_Gen_bKeyPlanDimension;
	BOOL bDirection	 = pDomOpt->m_Gen_bKeyPlanDirection;
	BOOL bOneLine	 = pDomOpt->m_Gen_bKeyPlanOneLine;

	Dom.SetScaleDim(dKeyScaleHor);

	// KEY PLAN �׸���	
	DrawPyung.AddKeyPlan(&Dom,NULL,NULL,dKeyScaleHor,FALSE,bSpliceMark, bJijumMark, bDirection, FALSE, bCrossBeam,bOneLine, -1, -1, FALSE, FALSE, sBridgeDir, sCounterDir);
	// HATCH �ֱ�
	for(long nG=0; nG<pBridge->GetGirdersu(); nG++) 
	{
		CPlateBasicIndex *pBxStt = GetPageBxStt(nG,nSubPage);
		CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG,nSubPage);
		DrawPyung.DrawHatchInRect(&Dom,pBxStt,pBxEnd,FALSE);
	}

	// ġ����	
	if(bDimension)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
		CPlateBxFinder Finder(pBridge->GetGirder(-1));
		//
		__int64 nTypeUpper  = DIM_BASE_SPAN;
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

	BOOL bRotate = pDomOpt->m_Gen_bKeyPlanRoVer;
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
	
	// ���� ������ �̵�
	Dom.Move(xDis,yDis);

	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();
	// ���� ������ ���� �̵�
	Dom.Move(pMove->GetPoint(KEY_PLAN));
	
	// ���� ���� ���		
	Dom.SetCalcExtRect();		
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom, Dom.GetExtRect(),KEY_PLAN,"Key Plan");	
	*pDom << Dom;

//	pOpt->SetEnvType(&Dom,HCAD_SUB_TITLE);
//	pStd->AddTitleOnDom(&Dom, "�� �� ��");

	// TITLE
	Dom.SetScaleDim(GetScale());
/*	//if (pOption->m_KeyPlanbOutLine)
	{
//		if (pOption->m_Det_bKeyPlanRoVer)
//			rect.left += Dom.Always(40);
//		else
			rect.bottom -=Dom.Always(30);
	}
*/	DrawStd.AddTitleOnDom(&Dom, dKeyScaleHor, rect, "�� �� ��",TRUE, TRUE, 0, bRotate);
	Dom.Move(pMove->GetPoint(KEY_PLAN+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),KEY_PLAN+1,"��൵ ����");
	*pDom << Dom;
}

void CAPlateOutGeneral::DimTextJongMatchLine(CDomyun *pDom, long nSubPage, long nG, long nDan)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(nG);	

	CSymbolDom SymDom(pDom, pStd->m_pOptStd);
	CDPoint xy(0,0), rAng(0,0);
	CString str;

	CPlateBasicIndex *pBxStt = GetPageBxStt(nG, nSubPage);
	CPlateBasicIndex *pBxEnd = GetPageBxEnd(nG, nSubPage);

	long nRealPage = pOpt->m_Gen_bDivPlanJong ? nSubPage-(m_nPage/2) : nSubPage;
	long MLNum	   = nRealPage;
	double TextHeight = pDom->GetTextHeight()*2.2;

	double SttSta = pBxStt->GetStation();
	double EndSta = pBxEnd->GetStation();

	double Scale   = SymDom.GetScaleDim();
	double dExo	   = SymDom.GetDimExo();
	double vLen	   = SymDom.GetDimVLen();
	double dLevel  = SymDom.GetDimLevelDis();
	double distExo = (dExo+vLen+dLevel*(nDan-1))*Scale;
	double dMaxEL  = pGir->GetMaxPlanEL(pBxStt, pBxEnd);
	double dSttEL  = pGir->GetMinPlanEL(pBxStt, pBxEnd);

	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
	if(!pBxStt->IsState(BX_STT_GIRDER))
	{
		xy.x = SttSta;
		if (pOpt->m_Gen_bJongJijumStation)
			xy.y = pGir->GetElevationFlange(SttSta) + distExo+(dMaxEL-dSttEL);
		else
			xy.y = pGir->GetElevationFlange(SttSta) + (nDan-1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		
		str.Format("%s%d","M.L", MLNum-1);
		SymDom.DrawSymbolHexagonStr(xy,rAng,str);

		SymDom.MoveTo(xy.x, xy.y-TextHeight/2);
		xy.y = pGir->GetElevationFlange(SttSta,0,CDPoint(0,1),FALSE)-TextHeight/2;
		SymDom.LineTo(xy.x,xy.y);
		*pDom << SymDom;
	}

	if(!pBxEnd->IsState(BX_END_GIRDER))
	{
		xy.x = SttSta + pGir->GetLengthDimType(SttSta,EndSta);
		if (pOpt->m_Gen_bJongJijumStation)
			xy.y = pGir->GetElevationFlange(EndSta) + distExo+(dMaxEL-dSttEL);
		else
			xy.y = pGir->GetElevationFlange(EndSta) + (nDan-1)*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		
		str.Format("%s%d","M.L", MLNum);
		SymDom.DrawSymbolHexagonStr(xy,rAng,str);

		SymDom.MoveTo(xy.x, xy.y-TextHeight/2);
		xy.y = pGir->GetElevationFlange(EndSta,0,CDPoint(0,1),FALSE)-TextHeight/2;
		SymDom.LineTo(xy.x,xy.y);
		*pDom << SymDom;
	}
}

void CAPlateOutGeneral::DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDanUp, long nDanLow)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	CPlateGirderApp		*pGir		= pBx->GetGirder();	

	CSymbolDom SymDom(pDom, pStd->m_pOptStd);
	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
	CString str;
	double sta   = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();
	CDPoint xy1(0,0),xy2(0,0);
	
	double Dis	   = pBridge->GetDisSlabLeftDom(pBx);
	double Scale   = SymDom.GetScaleDim();
	double dExo	   = SymDom.GetDimExo();
	double vLen	   = SymDom.GetDimVLen();
	double dLevel  = SymDom.GetDimLevelDis();
	double distExo = (dExo+vLen+dLevel*(nDanUp))*Scale;
	double TextHeight = SymDom.GetTextHeight()*2.2;

	xy1 = pGir->GetXyGirderDis(sta, Dis-distExo, vAng);
	str.Format("%s%d","M.L", nSubPage);
	CDPoint rAng = pGir->GetLine()->GetAngleAzimuth(sta);
	SymDom.DrawSymbolHexagonStr(xy1,rAng,str);

	distExo -= TextHeight/2;
	xy1 = pGir->GetXyGirderDis(sta, Dis-distExo, vAng);
	SymDom.MoveTo(xy1);
	Dis		= pBridge->GetDisSlabRightDom(pBx);
	distExo = (dExo+vLen+dLevel*(nDanLow))*Scale;
	xy2		= pGir->GetXyGirderDis(sta, Dis+distExo, vAng);
	SymDom.LineTo(xy2);

	*pDom << SymDom;
}

void CAPlateOutGeneral::DrawPierJong(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir		= (CPlateGirderApp*)pBxStt->GetGirder();			

	CAPlateDrawJong DrawJong(pStd->GetDataManage());	

	__int64 BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_CROSSBEAM_VBRACING;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst (BxFlag,pBxStt,pBxEnd);
	CDomyun *pDom = new CDomyun(pDomP);
	
	double SttSta = pBxStt->GetStation();
	double dBindGap	= pBridge->m_BindConc.m_dHeight-pBridge->m_BindConc.m_dDeep-pBridge->m_BindConc.m_dDis;
	BOOL bJongLevel = TRUE;
	while(pBx)
	{
		if(pBx->IsPier() || pBx->IsState(BX_STT_GIRDER) || pBx->IsState(BX_END_GIRDER))
		{
			if( (pBx->IsState(BX_STT_GIRDER) && pBridge->m_nTypeAbutOrPier[STT_POS]==0 ) || 
				(pBx->IsState(BX_END_GIRDER) && pBridge->m_nTypeAbutOrPier[END_POS]==0 ) )
			{
				CDPoint cp = CDPoint (0,0);
				if(pBx->IsState(BX_STT_GIRDER))
					cp.x = SttSta-pGir->GetLengthDimType(pGir->GetStationStt(), SttSta);
				if(pBx->IsState(BX_END_GIRDER))
					cp.x = SttSta+pGir->GetLengthDimType(pGir->GetStationEnd(), SttSta);
				if(bJongLevel)
					cp.y = pGir->GetElevationFlange(pBx->GetStation())
					+ (pBridge->IsTUGir()?0:pBridge->m_dHeightTaperStt) + pBridge->m_dThickPave
						 + pBridge->m_dThickSlabBase + dBindGap;
				else
					cp.y = pGir->GetLine()->GetElevationPlanEl(cp.x) + dBindGap;
				DrawJong.DrawPier(pDom, pGir->GetNumberJijumByBx(pBx),pBx->GetNumberGirder(),cp,TRUE,TRUE,pOpt->m_Gen_bJijumSimple);
			}
			else
			{
				double dShoeHeight = 0;	
				long nJiJumNo = pGir->GetNumberJijumByBx(pBx);
				if(pBx->IsState(BX_STT_GIRDER))
					dShoeHeight = pGir->GetShoe(0)->GetHeightTotalShoe();
				if(pBx->IsState(BX_END_GIRDER))
					dShoeHeight = pGir->GetShoe(pBridge->m_nQtyJigan)->GetHeightTotalShoe();
				if(pBx->IsPier())
				{
					dShoeHeight = pGir->GetShoe(nJiJumNo)->GetHeightTotalShoe();
					if( (nJiJumNo == 0 && pBridge->m_nTypeAbutOrPier[STT_POS]==1) ||
						(nJiJumNo == pBridge->m_nQtyJigan && pBridge->m_nTypeAbutOrPier[END_POS]==1))
					{
						pBx=Finder.GetBxNext ();
						continue;
					}
				}
				CDPoint cp = CDPoint (0,0);
				cp.x = SttSta+pGir->GetLengthDimType(pBx->GetStation(), SttSta);
				cp.y = pGir->GetElevationFlangeCamber(pBx->GetStation(),0,CDPoint(0,1),FALSE)
					 - (pStd->GetDataManage()->GetGlobalOption()->GetDesignHeigtBaseLower()==1 ? 0 : pBx->GetFactChain(G_F_L)->m_dFactChainThick)
			         - dShoeHeight;
				DrawJong.DrawPier(pDom, pGir->GetNumberJijumByBx(pBx),pBx->GetNumberGirder(),cp,TRUE,TRUE,pOpt->m_Gen_bJijumSimple);
			}
		}

		pBx=Finder.GetBxNext ();
	}
	DrawJong.DrawShoeMarkBx(pDom, pBxStt, pBxEnd, pDom->GetScaleDim());
	
	*pDomP << *pDom;
	delete pDom;
}

void CAPlateOutGeneral::AddLegend(CDomyun *pDomP)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();

	CGridDomyunEx Dom(pDomP);
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

	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.Move(-rect.left,-rect.top);
	Dom.Move(Dom.Always(680),Dom.Always(50));

	*pDomP << Dom;
}


// ���ü��� �����߽ɿ� ������ ���� ���� ��������
// ���ü��� �����߽ɿ� ������ ���� ���� ��������
// ���ü��� �����߽ɿ� �߰��� ���� ���� ����� ������
//<summary>
//
//</summary>
//<param name="*pDomP"></param>
//<param name="*pBxStt">�׻� �����߽� �Ŵ��� ����.</param>
//<param name="*pBxEnd">�׻� �����߽� �Ŵ��� ����.</param>
void CAPlateOutGeneral::DimTextLineInfo(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(pBxStt==NULL || pBxEnd==NULL)	return;
	CAPlateDrawStd	*pStd	 = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir    = pBridge->GetGirder(-1);
	CLineInfo		*pLine   = pGir->GetLine();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_LINE_P, pBxStt, pBxEnd);

	CDomyun Dom(pDom);

	CDPoint xy1(0,0),xy2(0,0),vAng(0,1);
	double sta=0, DisLeft=0, DisRight=0, Dis;

	CString sLineInfo = _T("");
	CString sRadius   = _T("");

	while(pBx)
	{
		sta  = pBx->GetStation();
		xy1	 = pLine->GetXyLineDis(sta,0);
		vAng = pLine->GetAngleAzimuth(sta);

		sLineInfo  = pLine->GetStringPoint(sta);
		sLineInfo += "  ";
		sLineInfo += pBridge->GetStationForStringOutDB(sta);
		sRadius    = pBridge->GetGirder(-1)->GetRadiusString(sta,0);//�׻� �����߽� ���� ǥ��

		BOOL bClockDir = (pLine->GetDirectionRotate(sta)==-1) ? TRUE : FALSE;

		if(bClockDir)
		{
			CPlateBasicIndex* pBx = pGir->GetBxOnJijum(0);
			pGir = pBridge->GetGirder(pBridge->GetGirdersu()-1);
			DisRight = pGir->m_dGirderCenter+pGir->GetWidthOnStation(pBx->GetStation(), TRUE)/2+pBridge->m_dWidthSlabRight;
			Dis  = (DisRight<0) ? Dom.GetTextWidth(sLineInfo) + Dom.Always(5) 
				                : DisRight + Dom.GetTextWidth(sLineInfo) + Dom.Always(5);
			xy2  = pLine->GetXyLineDis(sta,Dis);
			Dom.LineTo(xy1,xy2);
			Dom.SetTextAngle(vAng.RotateInv90().GetAngleDegree());
			Dom.SetTextAlignHorz(TA_RIGHT);
			//
			xy2  = pLine->GetXyLineDis(sta+Dom.Always(1),Dis);
			Dom.TextOut(xy2,sLineInfo + "  ");
			xy2  = pLine->GetXyLineDis(sta-Dom.GetTextHeight()-Dom.Always(1),Dis);
			Dom.TextOut(xy2,"(" + sRadius + ")  ");
		}
		else
		{
			CPlateBasicIndex* pBx = pGir->GetBxOnJijum(0);
			pGir = pBridge->GetGirder(0);
			DisLeft  = pGir->m_dGirderCenter-pGir->GetWidthOnStation(pBx->GetStation(), TRUE)/2-pBridge->m_dWidthSlabLeft;
			Dis  = (DisLeft>0) ? -Dom.GetTextWidth(sLineInfo) - Dom.Always(5) 
				               : DisLeft - Dom.GetTextWidth(sLineInfo) - Dom.Always(5);
			xy2  = pLine->GetXyLineDis(sta,Dis);
			Dom.LineTo(xy1,xy2);
			Dom.SetTextAngle(vAng.Rotate90().GetAngleDegree());
			Dom.SetTextAlignHorz(TA_RIGHT);
			//
			xy2  = pLine->GetXyLineDis(sta-Dom.Always(1),Dis);
			Dom.TextOut(xy2,"  " + sLineInfo);
			xy2  = pLine->GetXyLineDis(sta+Dom.GetTextHeight()+Dom.Always(1),Dis);
			Dom.TextOut(xy2,"(" + sRadius + ")  ");
		}

		*pDom << Dom;
		pBx = Finder.GetBxNext();
	}
}


// �ڽ��� �߽���ġ�� ���
void CAPlateOutGeneral::DimPyungJijumText(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									    double dist, double distExo, BOOL bIncludeSttEnd)
{
	CAPlateDrawStd	*pStd	 = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir    = pBxStt->GetGirder();

	CSymbolDom Dom(pDom, pStd->m_pOptStd);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStart  = pBxStt;
	CPlateBasicIndex *pBxFinish = pBxEnd;
	if(pBxStt->IsState(BX_STT_GIRDER))
		pBxStart  = Finder.GetBxFirst(BX_STT_JOINT);
	if(pBxEnd->IsState(BX_END_GIRDER))
		pBxFinish = Finder.GetBxFirst(BX_END_JOINT);
	//
	__int64 BxFlag = bIncludeSttEnd ? BX_STT_JOINT|BX_END_JOINT| BX_CROSSBEAM_VBRACING : BX_CROSSBEAM_VBRACING;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStart,pBxFinish);
	if(!pBx) return;
	
	CDPoint xy;
	CString strMark,strStation;
	while(pBx) 
	{
		if(pBx->IsState(BX_END_JOINT) || pBx->IsState(BX_STT_JOINT) || pBx->IsJijum())
		{
			BOOL bLeftDir = FALSE;
			if(pBx->IsState(BX_STT_JOINT) || pBx->IsAbutEnd()) bLeftDir = TRUE;
			if(pBx->IsPier() && pBx->GetNumberJijum()==pBridge->m_nQtyJigan)
				bLeftDir = TRUE;
			double sta  = pBx->GetStation();
			xy = pBridge->GetSlabLeft()->GetXyGirderDis(sta, distExo)+CDPoint(0,1)*Dom.Always(12);
			if(pBx->IsState(BX_STT_JOINT))	
				strMark = "��������";
			if(pBx->IsState(BX_END_JOINT))	
				strMark = "��������";
			if(pBx->IsJijum())
				strMark = pBridge->m_strJijumName[pBx->GetNumberJijum()];

			strStation = pBridge->GetStationForStringOutDB(sta);

			CDPoint rAngle = pGir->GetLine()->GetAngleAzimuth(sta);
			Dom.DrawSymbolStationFlag(xy,strMark,strStation, dist,bLeftDir?-1:1,rAngle);
		}
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}

void CAPlateOutGeneral::DimJongJijumText(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
				      double Height, double HeightExo, BOOL bIncludeSttEnd, BOOL bSameHeight)
{
	CAPlateDrawStd	*pStd	 = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir    = pBxStt->GetGirder();

	CSymbolDom Dom(pDom, pStd->m_pOptStd);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStart  = pBxStt;
	CPlateBasicIndex *pBxFinish = pBxEnd;
	if(pBxStt->IsState(BX_STT_GIRDER))
		pBxStart  = Finder.GetBxFirst(BX_STT_JOINT);
	if(pBxEnd->IsState(BX_END_GIRDER))
		pBxFinish = Finder.GetBxFirst(BX_END_JOINT);
	//
	__int64 BxFlag = bIncludeSttEnd ? BX_STT_JOINT|BX_END_JOINT|BX_CROSSBEAM_VBRACING : BX_CROSSBEAM_VBRACING;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStart,pBxFinish);
	if(!pBx) return;

	double StaStt = pBxStt->GetStation();
	CDPoint xy(0,0);
	
	CString strMark,strStation;
	while(pBx) 
	{
		if(pBx->IsState(BX_END_JOINT) || pBx->IsState(BX_STT_JOINT) || pBx->IsJijum())
		{
			BOOL bLeftDir = FALSE;
			if(pBx->IsState(BX_STT_JOINT) || pBx->IsAbutEnd() || pBxStt==pBx) bLeftDir = TRUE;
			if(pBx->IsPier() && pBx->GetNumberJijum()==pBridge->m_nQtyJigan)
				bLeftDir = TRUE;

			double sta  = pBx->GetStation();
			if(pBx->IsState(BX_STT_JOINT))
				xy.x = StaStt - pGir->GetLengthDimType(StaStt, sta);
			else
				xy.x = StaStt + pGir->GetLengthDimType(StaStt, sta);
			xy.y = pGir->GetElevationFlangeCamber(sta) + HeightExo;

			if(pBx->IsState(BX_STT_JOINT))	
				strMark = "��������";
			else if(pBx->IsState(BX_END_JOINT))
				strMark = "��������";
			else if(pBx->IsJijum())
			{
				long nJijum = pBx->GetNumberJijum();
				if(nJijum<0 || nJijum>pBridge->m_nQtyJigan) return;

				strMark = pBridge->m_strJijumName[nJijum];
			}

			strStation = pBridge->GetStationForStringOutDB(sta);
			Dom.DrawSymbolStationFlag(xy, strMark, strStation, Height, bLeftDir ? -1 : 1);
		}
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}


int CAPlateOutGeneral::GetDimDan(__int64 DimFlag)
{
	CAPlateDrawStd *pStd	 = (CAPlateDrawStd*)m_pStd;

	long cnt = 0;
	if(DimFlag & DIM_BASE_WELDSTUD)	cnt++;
	if(DimFlag & DIM_BASE_VSTIFF)	cnt++;
	if(DimFlag & DIM_BASE_DIAP)		cnt++;
	if(DimFlag & DIM_BASE_JOINUPPER) cnt++;
	if(DimFlag & DIM_BASE_JOINLOWER) cnt++;
	if(DimFlag & DIM_BASE_JOINWEB)	cnt++;
	if(DimFlag & DIM_BASE_SPLICE)	cnt++;
	if(DimFlag & DIM_BASE_FACTUPPER) cnt++;
	if(DimFlag & DIM_BASE_FACTLOWER) cnt++;
	if(DimFlag & DIM_BASE_FACTWEB)	cnt++;
	if(DimFlag & DIM_BASE_BYUN)		cnt++;
	if(DimFlag & DIM_BASE_TENDON)	cnt++;
	if(DimFlag & DIM_BASE_SPAN)		cnt++;
	if(DimFlag & DIM_BASE_TOTAL)	cnt++;

	return cnt;
}

void CAPlateOutGeneral::SortSta()
{
	while(1)
	{
		BOOL bSort = FALSE;
		for (long n =0; n < m_JweonInfoStaArr.GetSize(); n++)
		{
			double x = m_JweonInfoStaArr[n].x;
			double y = m_JweonInfoStaArr[n].y;
			if (x > y)
			{
				m_JweonInfoStaArr[n].x = y;
				m_JweonInfoStaArr[n].y = x;
				bSort = TRUE;
			}
		}
		if (!bSort)
			break;
	}

}