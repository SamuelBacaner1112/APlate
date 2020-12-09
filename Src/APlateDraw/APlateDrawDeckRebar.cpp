// APlateDrawDeckRebar.cpp: implementation of the CAPlateDrawDeckRebar class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "APlateDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MPE_REBAR_PLAN		 10
#define MPE_REBAR_JONG		 20
#define MPE_REBAR_HDANDETAIL 30
#define MPE_REBAR_HDAN_LEFT	 40
#define MPE_REBAR_HDAN_RIGH	 50
#define MPE_REBAR_KEYPLAN	 60

#define MPE_REBAR_ETC			150
#define MPE_REBAR_MAIN_DAN		200
#define MPE_REBAR_MAIN_GEN		240
#define MPE_REBAR_DISTRI		280
#define MPE_REBAR_MAIN_SKEW		320

#define MPE_TITLE_BM_TABLE			2600		
#define MPE_BM_DETAIL				2700
#define MPE_BM_TABLE				2800
#define MPE_NOTE					2900
#define MPE_BM_SPACER				3000
#define MPE_BM_SPACER_LO			3100
#define MPE_BM_SPACER_LO_TITLE		3200
#define MPE_BM_TABLE1				3300
#define MPE_PARASTIFF_R_DETAIL		3400
#define MPE_MOMENT_DIAGRAM			3500
#define MPE_TITLE					  99

//��ȣ�� 
#define MPE_GUARDBAR_REBAR			3600
#define	MPE_GUARDBAR_DETAIL			3610
#define MPE_GUARDBAR_TABLE			3620
#define MPE_GUARDBAR_TABLE1			3630


CAPlateDrawDeckRebar::CAPlateDrawDeckRebar()
{
	m_pBxStt = NULL;
	m_pBxEnd = NULL;

	m_nG = 0;
	
	for(long i=0; i<10; i++)
		m_nArrayPageNum[i]	= 0;
}

CAPlateDrawDeckRebar::~CAPlateDrawDeckRebar()
{

}

void CAPlateDrawDeckRebar::DrawOnePage()
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	CADeckData		*pDeckData	= pStd->m_pDataManage->GetDeckData();

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;
	long nSubPage = nPage - nprePage;		
	
	//�������� ���� Bx ����
	if(pBridge->GetGirdersu() >=2)
		m_nG = pBridge->GetGirdersu() - 2;
	else
		m_nG = 0;

	m_pBxStt = GetBxSttPage(nSubPage, m_nG);
	m_pBxEnd = GetBxEndPage(nSubPage, m_nG);

	CDomyun *pDomP = pStd->GetDomyun(nPage);
	pDomP->SetScaleDim(GetScale());
	CDomyun Dom(pDomP);

	// ����
	pPageMng->SetProgressData("�����...",0);
	pStd->GetDogak()->Add(&Dom,nPage,m_Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom,pBridge->m_sNameBridge,GetTitle(), _T(""),FALSE);	
	*pDomP << Dom;

	pDomP->SetCalcExtRect();
	CDRect dgkRect = pDomP->GetExtRect();
	m_rectDgk = dgkRect;

	CDRect rtPlan(0,0,0,0);
	CString str;

	// ���� ��� �ɼ�
	BOOL bPlanView				=	pDomOpt->m_bDeckRebar_PlanView;				// ��鵵
	BOOL bJongView				=	pDomOpt->m_bDeckRebar_JongView;				// ���鵵
	BOOL bFrontView				=	pDomOpt->m_bDeckRebar_FrontView;				// Ⱦ�ܸ鵵
	BOOL bSangse				=	pDomOpt->m_bDeckRebar_Sangse;					// �󼼵�
	
	BOOL bMainRebarView			=	pDomOpt->m_bDeckRebar_MainRebarView;			// ��ö�� ������
	BOOL bBelyckView			=	pDomOpt->m_bDeckRebar_BelyckView;				// ���ö�� ������
	
	BOOL bSangseRebar			=	pDomOpt->m_bDeckRebar_SangseRebar;				// ö�� ��
	BOOL bSangseGuard			=	pDomOpt->m_bDeckRebar_SangseGuard;				// ��ȣ�� ��
	
	//////////////////////////////////////////////////////////////////////////
	// ��鵵
	//////////////////////////////////////////////////////////////////////////
	if(nSubPage < m_nArrayPageNum[1])
	{
		double dRightFrontStt = 0;	//����Ⱦ�ܸ鵵 X�����
		if(bPlanView)
		{
			pPageMng->SetProgressData("��鵵 �����...",10);
			Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarPlan);
			AddPlan(&Dom, nSubPage);
			Dom.SetScaleDim(pDomP->GetScaleDim());
			Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarPlan);//Ȯ��		
			
			Dom.SetCalcExtRect();
			rtPlan = Dom.GetExtRect();
			double MoveX = Dom.Always(190);
			double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(55);
			Dom.Move(MoveX, MoveY);
			CDRect rect = Dom.GetExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_PLAN));
			dRightFrontStt = Dom.GetExtRect().right;

			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_PLAN,"��鵵");
			*pDomP << Dom;

			DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "�� �� ��", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_REBAR_PLAN+1));
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_PLAN+1,"��鵵 ����");
			*pDomP << Dom;
		}
		//////////////////////////////////////////////////////////////////////////
		// Ⱦ�ܸ鵵
		//////////////////////////////////////////////////////////////////////////
		if(bFrontView)
		{
			CString szName;
			CDRect rect;
			if(pDomOpt->m_dDeck_TypeRebarFrontLeft!=2)
			{
				pPageMng->SetProgressData("���� Ⱦ�ܸ鵵 �����...",15);

				Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarFront);
				CString szName = AddFront(&Dom, nSubPage, TRUE);	
				
				Dom.SetScaleDim(pDomP->GetScaleDim());
				Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarFront);//Ȯ��		

				Dom.SetCalcExtRect();
				double MoveX = Dom.Always(80);
				double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(55);
				Dom.Move(MoveX, MoveY);
				Dom.Move(pMove->GetPoint(MPE_REBAR_HDAN_LEFT));

				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_HDAN_LEFT,"���� Ⱦ�ܸ鵵");
				*pDomP << Dom;
				
				Dom.SetCalcExtRect();
				DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarFront, Dom.GetExtRect(), "Ⱦ �� �� �� ("+szName+")", TRUE,TRUE,0);
				Dom.Move(pMove->GetPoint(MPE_REBAR_HDAN_LEFT+MPE_TITLE));
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_HDAN_LEFT+MPE_TITLE, "���� Ⱦ�ܸ鵵 ����");
				*pDomP << Dom;
			}

			if(pDomOpt->m_dDeck_TypeRebarFrontRigh!=2)
			{
				pPageMng->SetProgressData("���� Ⱦ�ܸ鵵 �����...",15);
				
				Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarFront);
				CString szName = AddFront(&Dom, nSubPage, FALSE);	

				Dom.SetScaleDim(pDomP->GetScaleDim());
				Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarFront);//Ȯ��		

				Dom.SetCalcExtRect();
				double MoveX = dRightFrontStt+Dom.Always(100);
				double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(55);
				Dom.Move(MoveX, MoveY);
				Dom.Move(pMove->GetPoint(MPE_REBAR_HDAN_RIGH));

				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_HDAN_RIGH,"���� Ⱦ�ܸ鵵");
				*pDomP << Dom;

				Dom.SetCalcExtRect();
				DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarFront, Dom.GetExtRect(), "Ⱦ �� �� �� ("+szName+")", TRUE,TRUE,0);
				Dom.Move(pMove->GetPoint(MPE_REBAR_HDAN_RIGH+1));
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_HDAN_RIGH+1,"���� Ⱦ�ܸ鵵 ����");
				*pDomP << Dom;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ���ܸ鵵
		//////////////////////////////////////////////////////////////////////////
		if(bJongView)		
		{
			pPageMng->SetProgressData("���ܸ鵵 �����...",15);
			Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarJong);
			AddJong(&Dom, nSubPage);
			Dom.SetScaleDim(pDomP->GetScaleDim());
			Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarJong);//Ȯ��		

			Dom.SetCalcExtRect();
			double MoveX = Dom.Always(190);
			double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(300);
			Dom.Move(MoveX, MoveY);
			CDRect rect = Dom.GetExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_JONG));
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_JONG,"���� ���ܸ鵵");
			*pDomP << Dom;
			//Title
			Dom.SetScaleDim(GetScale());
			DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarJong, rect, "���ܸ鵵",TRUE,TRUE,0);
			Dom.Move(pMove->GetPoint(MPE_REBAR_JONG+1));
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_JONG+1,"���� ���ܸ鵵 ����");
			*pDomP << Dom;
		}

		//////////////////////////////////////////////////////////////////////////
		// ��൵
		//////////////////////////////////////////////////////////////////////////
		pPageMng->SetProgressData("��൵ �����...",75);
		AddKeyPlan(&Dom,nSubPage);				
		*pDomP << Dom;
	}
	//////////////////////////////////////////////////////////////////////////
	// ��ö�� ������
	//////////////////////////////////////////////////////////////////////////
	else if(nSubPage >= m_nArrayPageNum[1] && nSubPage < m_nArrayPageNum[2])
	{
		if(bMainRebarView)
		{
			long nAddPage = nSubPage - m_nArrayPageNum[1];		
			long nMainCycleDan = pDeckData->m_stMainCycleDan.size();
			long nMainCycleGen = pDeckData->m_stMainCycleCen.size();
			long nMainCycleAcro = pDeckData->m_stMainCycleSabogangStt.size();

			pPageMng->SetProgressData("�ܺ� ��ö�� ������ �����...", 10);
			long nSection = SECTION_DAN;			// �ܺ�
			double	dDanW = 0;
			long	nCycle = 0;
			for(nCycle=nAddPage*3; nCycle<nMainCycleDan; nCycle++)
			{			
				if(nCycle >= (nAddPage+1)*3)	break;
				
				Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarMain);
				AddMainRebar(&Dom, nCycle, nSection, nSubPage);
				Dom.SetScaleDim(pDomP->GetScaleDim());
				Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarMain);//Ȯ��		

				Dom.SetCalcExtRect();
				rtPlan	= Dom.GetExtRect();
				Dom.Move(-rtPlan.TopLeft());	//������ �����ϴ��� �������� ������.
				dDanW = rtPlan.Width();

				double MoveX = 300;
				double MoveY = dgkRect.Height()*0.8 - Dom.Always(150*(nCycle%3)+120);
				Dom.Move(MoveX, MoveY);
				CDRect rect = Dom.GetExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_DAN+nCycle*2));
				str.Format("CYCLE - %d", nCycle+1);
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_DAN+nCycle*2, str);
				*pDomP << Dom;

				Dom.SetScaleDim(GetScale());
				DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarMain, rect, str, TRUE, FALSE, 0, FALSE, "", HCAD_SMALL_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_DAN+nCycle*2+1));
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_DAN+nCycle*2+1,str+" ����");
				*pDomP << Dom;
				if(nCycle==nAddPage*3)
				{
					rect = Dom.GetExtRect();
					Dom.SetScaleDim(GetScale());
					DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarMain, rect, "�ܺ� ��ö�� ������", TRUE, TRUE, 0);
					Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_DAN+1000));
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_DAN+1000,"�ܺ� ��ö�� ������ ����");
					*pDomP << Dom;
				}
			}

			pPageMng->SetProgressData("�߾Ӻ� ��ö�� ������ �����...", 10);
			nSection = SECTION_CEN;			// �Ϲݺ�	
			for(nCycle=nAddPage*3; nCycle<nMainCycleGen; nCycle++)
			{			
				if(nCycle >= (nAddPage+1)*3)	break;

				Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarMain);
				AddMainRebar(&Dom, nCycle, nSection, nSubPage);
				Dom.SetScaleDim(pDomP->GetScaleDim());
				Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarMain);//Ȯ��		

				Dom.SetCalcExtRect();
				rtPlan	= Dom.GetExtRect();
				Dom.Move(-rtPlan.TopLeft());	//������ �����ϴ��� �������� ������.

				double MoveX = 300 + dDanW + 100;
				double MoveY = dgkRect.Height()*0.8 - Dom.Always(150*(nCycle%3)+120);
				Dom.Move(MoveX, MoveY);
				CDRect rect = Dom.GetExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_GEN+nCycle*2));
				str.Format("CYCLE - %d", nCycle+1);
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_GEN+nCycle*2, str);
				*pDomP << Dom;

				Dom.SetScaleDim(GetScale());
				DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarMain, rect, str, TRUE, FALSE, 0, FALSE, "", HCAD_SMALL_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_GEN+nCycle*2+1));
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_GEN+nCycle*2+1,str+" ����");
				*pDomP << Dom;
				if(nCycle==nAddPage*3)
				{
					rect = Dom.GetExtRect();
					Dom.SetScaleDim(GetScale());
					DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarMain, rect, "�߾Ӻ� ��ö�� ������", TRUE, TRUE, 0);
					Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_GEN+1000));
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_GEN+1000,"�߾Ӻ� ��ö�� ������ ����");
					*pDomP << Dom;
				}
			}
			pPageMng->SetProgressData("�纸��ö�� ������ �����...", 10);
			nSection = SECTION_SKEW_STT;			// �纸�� ö�ٺ�	
			if (pDeckData->IsSkewStt() || pDeckData->IsSkewEnd())
			{
				for(long nCycle=nAddPage*3; nCycle<nMainCycleAcro; nCycle++)
				{			
					if(nCycle >= (nAddPage+1)*3)	break;

					Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarMain);
					AddMainRebar(&Dom, nCycle, nSection, nSubPage);
					Dom.SetScaleDim(pDomP->GetScaleDim());
					Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarMain);//Ȯ��		

					Dom.SetCalcExtRect();
					rtPlan	= Dom.GetExtRect();
					Dom.Move(-rtPlan.TopLeft());	//������ �����ϴ��� �������� ������.

					double MoveX = 300 + dDanW*2 + 100;
					double MoveY = dgkRect.Height()*0.8 - Dom.Always(150*(nCycle%3)+120);
					Dom.Move(MoveX, MoveY);
					CDRect rect = Dom.GetExtRect();
					Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_SKEW+nCycle*2));
					str.Format("CYCLE - %d", nCycle+1);
					pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
					pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_SKEW+nCycle*2, str);
					*pDomP << Dom;

					Dom.SetScaleDim(GetScale());
					DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarMain, rect, str, TRUE, FALSE, 0, FALSE, "", HCAD_SMALL_TITLE);
					Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_SKEW+nCycle*2+1));
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_SKEW+nCycle*2+1,str+" ����");
					*pDomP << Dom;
					if(nCycle==nAddPage*3)
					{
						rect = Dom.GetExtRect();
						Dom.SetScaleDim(GetScale());
						DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarMain, rect, "�纸��ö�� ������", TRUE, TRUE, 0);
						Dom.Move(pMove->GetPoint(MPE_REBAR_MAIN_SKEW+1000));
						Dom.SetCalcExtRect();
						pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_MAIN_SKEW+1000,"�纸��ö�� ������ ����");
						*pDomP << Dom;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ���ö�� ������
	//////////////////////////////////////////////////////////////////////////
	else if(nSubPage >= m_nArrayPageNum[2] && nSubPage < m_nArrayPageNum[3])	
	{
		if(bBelyckView)
		{
			long nAddPage		= nSubPage - m_nArrayPageNum[2];		
			long nBeryukCycle	= pDeckData->m_stBeryukCycleLeft.size();
			
			pPageMng->SetProgressData("���ö�� ������ �����...",10);

			long nSubPage=0;
			for(long nCycle=nAddPage*4; nCycle<nBeryukCycle+1; nCycle++)
			{
				if(nSubPage>3)	break;

				CDoubleArray dArrX;

				if(nCycle == nBeryukCycle)	//�������� ���Ʈ��
				{
					if(pBridge->m_nQtyJigan==1)
						continue;
					AddDeadLoadMomentDiagram(&Dom, dArrX);
				}
				else
					AddDistriRebar(&Dom, nCycle, nSubPage);

				Dom.SetCalcExtRect();
				rtPlan = Dom.GetExtRect();
				double MoveX = (dgkRect.Width() - rtPlan.Width())/2.0;
				double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(160*(nCycle-nAddPage*4)+120);
				Dom.Move(MoveX, MoveY);
				CDRect rect = Dom.GetExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_DISTRI+nCycle*2));
				if(nCycle == nBeryukCycle)	// �������� ���Ʈ��
					str = "�������� ���Ʈ��";
				else
					str.Format("CYCLE - %d", nCycle+1);
				pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_DISTRI+nCycle*2, str);
				*pDomP << Dom;

				Dom.SetScaleDim(GetScale());
				DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarBelyck, rect, str, TRUE, FALSE, 0, FALSE, "", HCAD_SMALL_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_DISTRI+nCycle*2+1));
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_DISTRI+nCycle*2+1, str+" ����");
				*pDomP << Dom;

				if(nCycle==nAddPage*3)
				{
					rect = Dom.GetExtRect();
					Dom.SetScaleDim(GetScale());
					DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarBelyck, rect, "���ö�� ������", TRUE, TRUE, 0);
					Dom.Move(pMove->GetPoint(MPE_REBAR_DISTRI+1000));
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_DISTRI+1000,"���ö�� ������ ����");
					*pDomP << Dom;
				}
				nSubPage++;
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// �󼼵�
	//////////////////////////////////////////////////////////////////////////
	else if(nSubPage >= m_nArrayPageNum[3] && nSubPage < m_nArrayPageNum[4])
	{
		if(bSangse)
		{
			pPageMng->SetProgressData("�󼼵� �����...",5);
			AddSangseETC(&Dom, nSubPage);				
			*pDomP << Dom;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ö�� ��
	//////////////////////////////////////////////////////////////////////////
	else if(nSubPage >= m_nArrayPageNum[4] && nSubPage < m_nArrayPageNum[5])
	{
		if(bSangseRebar)
		{
			pPageMng->SetProgressData("ö�� �� �����...",10);
			AddPageBM(&Dom);
			*pDomP << Dom;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ��ȣ�� ��
	//////////////////////////////////////////////////////////////////////////
	else if(nSubPage >= m_nArrayPageNum[5])		// �󼼵�
	{
		if(bSangseGuard)
		{
			pPageMng->SetProgressData("��ȣ�� �󼼵� �����...",10);
			AddPageGuardRebar(&Dom);
			*pDomP << Dom;
		}
	}
	pPageMng->SetProgressData("��¿Ϸ�",100);	
}

long CAPlateDrawDeckRebar::GetPageSu()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	COptionStd		*pOpt		= pStd->m_pDomOptionStd;
	CADeckData		*pDeckData	= pStd->GetDataManage()->GetDeckData();

	long nMainCycleDan	= pDeckData->m_stMainCycleDan.size();
	long nMainCycleGen	= pDeckData->m_stMainCycleCen.size();
	long nMainCycleAcro = pDeckData->m_stMainCycleSabogangStt.size();
	long nMainCycle		= max(max(nMainCycleDan, nMainCycleGen), nMainCycleAcro);
	long nBeryukCycle	= pDeckData->m_stBeryukCycleLeft.size();

	m_nArrayPageNum[0] = 1;
	switch(pOpt->m_nDeck_Slab_DivType)
	{
		case 0:
			m_nPage = pBridge->m_nQtyJigan;
			break;
		case 1:
			m_nPage = 1;
			break;
	}

	if(pOpt->m_bDeckRebar_MainRebarView)
	{
		// ��ö�� ������ ����������
		m_nArrayPageNum[1] = m_nPage+1;
		// ��ö�� ������ ������ �߰�
		m_nPage += (long)RoundUp(nMainCycle/3.0, 0);
	}
	else
	{
		m_nArrayPageNum[1] = m_nPage+1;
	}

	if(pOpt->m_bDeckRebar_BelyckView)
	{
		// ���ö�� ������ ����������
		m_nArrayPageNum[2] = m_nPage+1;
		// ���ö�� ������ ������ �߰�
		m_nPage += (long)RoundUp(nBeryukCycle/4.0, 0);
	}
	else
	{
		m_nArrayPageNum[2] = m_nPage+1;
	}

	if(pOpt->m_bDeckRebar_Sangse)
	{
		// �󼼵� ���� ������
		m_nArrayPageNum[3] = m_nPage+1;
		// �󼼵� ������ �߰�
		m_nPage += 1;
	}
	else
	{
		m_nArrayPageNum[3] = m_nPage+1;
	}

	if(pOpt->m_bDeckRebar_SangseRebar)
	{
		// ö�� �� ����������
		m_nArrayPageNum[4] = m_nPage+1;
		// ö�� �� ������ �߰�
		m_nPage += 1;
	}
	else
	{
		m_nArrayPageNum[4] = m_nPage+1;
	}

	if(pOpt->m_bDeckRebar_SangseGuard)
	{
		// ��ȣ��ö�� ����������
		m_nArrayPageNum[5] = m_nPage+1;
		// ��ȣ��ö��
		m_nPage += 1;
	}
	else
	{
		m_nArrayPageNum[5] = m_nPage+1;
	}

	return m_nPage;
}

CString CAPlateDrawDeckRebar::GetTitle()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	CString csTitle;

	long nPageSu = m_nPage;
	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("�� �� �� �� �� �� (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("�� �� �� �� �� ��");

	return szTitle;
}

double CAPlateDrawDeckRebar::GetScale()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();
	
	m_Scale  = pOpt->m_dDeck_ScaleRebarPlan;

	return m_Scale;
}

CPlateBasicIndex* CAPlateDrawDeckRebar::GetBxSttPage(long nSubPage, long nG, BOOL bSttBridge)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	long nPageSuOrg = GetPageSu();
	long nPageOrg	= nSubPage%nPageSuOrg;
	if((nSubPage%nPageSuOrg)==0)
		nPageOrg = nPageSuOrg;

	CPlateBasicIndex *pBx = NULL;
	long nDivDom = pOpt->m_nDeck_Slab_DivType;
	__int64 BxFlag = bSttBridge ? BX_STT_JOINT: BX_STT_GIRDER;
	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum(nPageOrg-1);
			if(nPageOrg==1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BxFlag);
			}
			break;
		case 1 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxFirst(BxFlag);
			break;
	}
	
	return pBx;
}


CPlateBasicIndex* CAPlateDrawDeckRebar::GetBxEndPage(long nSubPage, long nG, BOOL bEndBridge)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	long nPageSuOrg = GetPageSu();
	long nPageOrg	= nSubPage%nPageSuOrg;
	if((nSubPage%nPageSuOrg)==0)
		nPageOrg = nPageSuOrg;

	CPlateBasicIndex *pBx = NULL;
	long nDivDom = pOpt->m_nDeck_Slab_DivType;
	__int64 BxFlag = bEndBridge ? BX_END_JOINT: BX_END_GIRDER;
	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum(nPageOrg);
			if(nPageOrg==pBridge->m_nQtyJigan)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BxFlag);
			}			
			break;
		case 1 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxLast(BxFlag);
			break;
	}
	
	return pBx;
}

BOOL CAPlateDrawDeckRebar::IsStringerInPage(long nPage)
{
//	CAPlateDrawStandard *pStd = (CAPlateDrawStandard*)m_pStd;
//	CPlateBridgeApp	  *pBridge  = pStd->GetDataManage()->GetBridge();
//	CPlateGirderApp	  *pGir = pBridge->GetGirder(-1);
//
//	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
//	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
//	{
//		pGir = pBridge->GetGirder(nG);
//
//		CPlateBasicIndex *pBxStt = GetBxSttPage(nPage,nG);
//		CPlateBasicIndex *pBxEnd = GetBxEndPage(nPage,nG);
//		CPlateBxFinder Finder(pGir);
//		CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
//		
//		while(pBx)
//		{
//			if(pBx->IsStringer())
//				return TRUE;
//			pBx = Finder.GetBxNext();
//		}
//	}

	return FALSE;
}

BOOL CAPlateDrawDeckRebar::IsStringerInPage(long nPage, long nG)
{
//	CAPlateDrawStandard *pStd = (CAPlateDrawStandard*)m_pStd;
//	CPlateBridgeApp	  *pBridge  = pStd->GetDataManage()->GetBridge();
//	CPlateGirderApp	  *pGir = pBridge->GetGirder(nG);
//
//	CPlateBasicIndex *pBxStr = GetBxDimensionTable(nPage);
//	CPlateBasicIndex *pBxGir = pBxStr->GetBxMatchByDiap(pGir);
//
//	return pBxGir->IsStringer();
	return FALSE;

}

CPlateBasicIndex* CAPlateDrawDeckRebar::GetBxDimensionTable(long nPage)
{
//	CAPlateDrawStandard *pStd = (CAPlateDrawStandard*)m_pStd;
//	CPlateBridgeApp	  *pBridge  = pStd->GetDataManage()->GetBridge();
//	CPlateGirderApp	  *pGir = pBridge->GetGirder(-1);
//
//	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
//	CPlateBasicIndex *pBxStt = GetBxSttPage(nPage,-1);
//	CPlateBasicIndex *pBxEnd = GetBxEndPage(nPage,-1);
//
//	CPlateBxFinder Finder(pGir);
//	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
//
//	if(IsStringerInPage(nPage)==FALSE)	return pBx;
//
//	// ���κ��� �ִ� Bx�� ����...
//	BOOL bCheckStringer = FALSE;
//	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
//	{
//		pGir = pBridge->GetGirder(nG);
//
//		CPlateBasicIndex *pBxStt = GetBxSttPage(nPage,nG);
//		CPlateBasicIndex *pBxEnd = GetBxEndPage(nPage,nG);
//		CPlateBxFinder Finder(pGir);
//		pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
//		
//		while(pBx)
//		{
//			if(pBx->IsStringer())
//			{
//				bCheckStringer = TRUE;
//				break;
//			}
//			pBx = Finder.GetBxNext();
//		}
//		if(bCheckStringer) break;
//	}
//

	return NULL;
}



void CAPlateDrawDeckRebar::AddPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CDrawDBBaseRebarPlan DBDeckPlan(pStd->GetDataManage());
	CAPlateDrawPyung DBPlan(pStd->GetDataManage());

	BOOL bDrawMainJoint = pDomOpt->m_bDeck_ShowJointMainRebar;

	CDomyun Dom(pDom);

	CDRect rect;
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	
	BOOL bStt = m_pBxStt->IsState(BX_STT_JOINT) ? TRUE : FALSE;
	BOOL bEnd = m_pBxEnd->IsState(BX_END_JOINT) ? TRUE : FALSE;
	CPlateBasicIndex *pBxStt = NULL;
	CPlateBasicIndex *pBxEnd = NULL;

	// ������ ����
	DrawSlabByPage(&Dom, nSubPage);
	
	// �߽ɼ� �׸���
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_CENT);
	double dStaStt = GetBxSttPage(nSubPage, -1, TRUE)->GetStation();
	double dStaEnd = GetBxEndPage(nSubPage, -1, TRUE)->GetStation();
	DBPlan.DrawUpDownLine(&Dom, dStaStt, dStaEnd);		

	if(bStt)
	{
		DBPlan.DrawExpJointLine(&Dom, TRUE, -1);
		DBPlan.DrawSlabTaperLineStt(&Dom, nSubPage-1);
	}
	if(bEnd)
	{
		DBPlan.DrawExpJointLine(&Dom, FALSE, -1);	
		DBPlan.DrawSlabTaperLineStt(&Dom, nSubPage);
	}

	// �Ŵ����� �� ������ ��ġ��(���κ� �� ���κ� ��ġ��)
	long nGsu = pBridge->GetGirdersu();
	for(long nG=0; nG<nGsu; nG++)
	{				
		pBxStt = GetBxSttPage(nSubPage,nG, FALSE);
		pBxEnd = GetBxEndPage(nSubPage,nG, FALSE);

		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
		DBPlan.DrawGirderWithenBx(&Dom,pBxStt,pBxEnd, TRUE, 0);		

		pStd->m_pOptStd->SetEnvLType(&Dom,LT_DASH_LINE);
		if(pBxStt==NULL || pBxEnd==NULL)	return;
		pBxStt = GetBxSttPage(nSubPage,nG, TRUE);
		pBxEnd = GetBxEndPage(nSubPage,nG, TRUE);
		if(nG < nGsu-1)
			DBPlan.DrawPlanCrossBeam(&Dom, pBxStt, pBxEnd);
	}

	// Ʈ������ ����
	CPtrArray ObArcArray, ObArcForDeleteArray, ObCircleArrayCB, ObCircleArrayST, ObCircleArrayOBK, ObCircleArrayDan, ObCircleArrayCant;
	CDPointArray xyArrCB, xyArrST, xyArrOBK;
	CVectorArray xyArrDan, xyArrCant;

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);			
	DBDeckPlan.DrawTrimLine(&Dom, ObArcArray, ObArcForDeleteArray, ObCircleArrayCB, ObCircleArrayST, ObCircleArrayOBK, ObCircleArrayDan, ObCircleArrayCant, xyArrCB, xyArrST, xyArrOBK, xyArrDan, xyArrCant, m_pBxStt, m_pBxEnd);
	
	// ġ���� 
	AddDimPlan(&Dom, nSubPage);

	// ��ٵ���
	CDomyun ReBarDom(&Dom), TotalDom(&Dom);
	
	pBxStt = GetBxSttPage(nSubPage,-1, TRUE);	// �߽ɰŴ� ��������
	pBxEnd = GetBxEndPage(nSubPage,-1, TRUE);	// �߽ɰŴ� ��������

	CPlateGirderApp *pGirL = pBridge->GetGirder(0);
	CPlateGirderApp *pGirR = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	long nJStt	= pBxStt->IsState(BX_STT_JOINT|BX_STT_GIRDER) ? 0 : pBxStt->GetNumberJijum();
	long nJEnd	= pBxEnd->IsState(BX_END_JOINT|BX_END_GIRDER) ? pBridge->m_nQtyJigan : pBxEnd->GetNumberJijum();
	// ������ �簢�� ������ �Ϲ�,������ ���ö�� ������������� ���� #27157
	CPlateBasicIndex *pBxBeryukStt	= nJStt<=0						? pBxStt : GetBxSttPage(nSubPage-1,-1, TRUE);
	CPlateBasicIndex *pBxBeryukEnd	= nJEnd>=pBridge->m_nQtyJigan	? pBxEnd : GetBxEndPage(nSubPage+1,-1, TRUE);

	pStd->m_pOptStd->SetEnvType(&TotalDom,HCAD_BARC);
	DBDeckPlan.DrawMainRebar(&TotalDom, pBxStt, pBxEnd, TRUE, DRAW_LEFTSIDE, FALSE, bDrawMainJoint);
	DBDeckPlan.DrawMainRebar(&TotalDom, pBxStt, pBxEnd, FALSE, DRAW_RIGHTSIDE, FALSE, bDrawMainJoint);
	DBDeckPlan.DrawMainRebarAtSkew(&TotalDom, TRUE, DRAW_LEFTSIDE, bStt, bEnd);
	DBDeckPlan.DrawMainRebarAtSkew(&TotalDom, FALSE, DRAW_RIGHTSIDE, bStt, bEnd);
	DBDeckPlan.DrawSabogangRebar(&TotalDom, TRUE, DRAW_LEFTSIDE, bStt, bEnd);
	DBDeckPlan.DrawSabogangRebar(&TotalDom, FALSE, DRAW_RIGHTSIDE, bStt, bEnd);
	DBDeckPlan.DrawBeryukRebarOnCenter(&TotalDom, pBxBeryukStt, pBxBeryukEnd, TRUE,DRAW_LEFTSIDE);
	DBDeckPlan.DrawBeryukRebarOnCenter(&TotalDom, pBxBeryukStt, pBxBeryukEnd, FALSE,DRAW_RIGHTSIDE);
	DBDeckPlan.DrawBeryukRebarOnJijum(&TotalDom, pBxBeryukStt, pBxBeryukEnd, TRUE, DRAW_LEFTSIDE);
	DBDeckPlan.DrawBeryukRebarOnJijum(&TotalDom, pBxBeryukStt, pBxBeryukEnd, FALSE, DRAW_RIGHTSIDE);
	
	long nSize = min(ObArcArray.GetSize(), ObArcForDeleteArray.GetSize());
	for(long nx=0; nx<nSize; ++nx)				
	{	
		CDomyun TempDom(&Dom);
		TempDom += TotalDom;	// TemoDom = TotalDom; �ϴ°�� �����߻� #41405 �߰��������� ����
		if(nx%2==1)									// ���� ����
		{
			CObArc *pArc	= (CObArc *)ObArcArray.GetAt(nx);
			CObArc *pArcDel	= (CObArc *)ObArcForDeleteArray.GetAt(nx);
			TempDom.TrimByArc2(pArc, pArcDel->m_SttPoint, FALSE);
			TempDom.TrimByCircle2(pArcDel->m_SttPoint,pArcDel->m_Radius, TRUE);
			TempDom.DeleteByArcBetweenArc(pArc, pArcDel);
			++nx;
		}
		
		if(nx%2==0 && nx<nSize)		// ���� ����
		{
			CObArc *pArc	= (CObArc *)ObArcArray.GetAt(nx);
			CObArc *pArcDel	= (CObArc *)ObArcForDeleteArray.GetAt(nx);
			TempDom.TrimByArc2(pArc, pArcDel->m_SttPoint, FALSE);
			TempDom.TrimByCircle2(pArcDel->m_SttPoint,pArcDel->m_Radius, TRUE);
			TempDom.DeleteByArcBetweenArc(pArc, pArcDel);
		}

		ReBarDom << TempDom;
	}

	if(nJStt>0)
	{
		double dStaL	= pGirL->GetStationOnJijum(nJStt);
		double dStaR	= pGirR->GetStationOnJijum(nJStt);
		CDPoint xyStt	= pGirL->GetXyGirderDis(dStaL, -pBridge->m_dWidthSlabLeft-2000.0, pBxStt->GetAngle());
		CDPoint xyEnd	= pGirR->GetXyGirderDis(dStaR, pBridge->m_dWidthSlabRight+2000.0, pBxStt->GetAngle());
		ReBarDom.TrimByLineOnSide(xyStt, xyEnd, 1, TRUE);
	}
	if(nJEnd<pBridge->m_nQtyJigan)
	{
		double dStaL	= pGirL->GetStationOnJijum(nJEnd);
		double dStaR	= pGirR->GetStationOnJijum(nJEnd);
		CDPoint xyStt	= pGirL->GetXyGirderDis(dStaL, -pBridge->m_dWidthSlabLeft-2000.0, pBxEnd->GetAngle());
		CDPoint xyEnd	= pGirR->GetXyGirderDis(dStaR, pBridge->m_dWidthSlabRight+2000.0, pBxEnd->GetAngle());
		ReBarDom.TrimByLineOnSide(xyStt, xyEnd,-1, TRUE);
	}
	Dom << ReBarDom;

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	DBDeckPlan.DrawMark(&Dom, nSubPage-1);

	// �󼼿��ȿ� �ִ� ö��, ��ũ Ʈ��.
	for(long nx=0; nx<ObCircleArrayCB.GetSize(); ++nx)
	{
		CObArc *pCircle = (CObArc *)ObCircleArrayCB.GetAt(nx);
		Dom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius,FALSE);
	}
	for(long nx=0; nx<ObCircleArrayDan.GetSize(); ++nx)
	{
		CObArc *pCircle = (CObArc *)ObCircleArrayDan.GetAt(nx);
		Dom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, FALSE);
	}						
	for(long nx=0; nx<ObCircleArrayCant.GetSize(); ++nx)
	{
		CObArc *pCircle = (CObArc *)ObCircleArrayCant.GetAt(nx);
		Dom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, FALSE);
	}						

	//////////////////////////////////////////////////////////////////////////
	// �󼼵��� �׸��� (���κ�)
	for(long nx=0; nx<ObCircleArrayCB.GetSize(); ++nx)
	{
		long nG  = (long)xyArrCB[nx].x;
		long nCB = (long)xyArrCB[nx].y;

		if(nG<0 || nCB<0)	continue;

		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CSection *pSec   = pGir->GetSectionBySectionNo(nCB);
		DBDeckPlan.DrawBogangRebarAtCrossBeamBx(&ReBarDom,pSec->GetBx(),0, TRUE);

		CObArc *pCircle = (CObArc *)ObCircleArrayCB.GetAt(nx);
		ReBarDom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, TRUE);
		Dom << ReBarDom;
	}

	// �󼼵��� �׸��� (�ܺ�)
	if(pBridge->m_dHeightTaperStt > 0)
	{
		for(long nx=0; nx<ObCircleArrayDan.GetSize(); ++nx)
		{
			BOOL bSttSlab = xyArrDan[nx].z==-1 ? TRUE : FALSE;
			if(pDeckData->IsPointInJigan(CDPoint(xyArrDan[nx].x, xyArrDan[nx].y), nSubPage-1))
			{
				DBDeckPlan.DrawSangseUnderSinchuk(&ReBarDom, bSttSlab, DRAW_SIDEALL);		

				CObArc *pCircle = (CObArc *)ObCircleArrayDan.GetAt(nx);
				ReBarDom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, TRUE);
				Dom << ReBarDom;
			}
		}
	}
	// �󼼵��� �׸��� (ĵƿ������)
	for(long nx=0; nx<ObCircleArrayCant.GetSize(); ++nx)
	{
		BOOL bUpper = xyArrCant[nx].z==-1 ? FALSE : TRUE;

		if(pDeckData->IsPointInJigan(CDPoint(xyArrCant[nx].x, xyArrCant[nx].y), nSubPage-1))
		{
			DBDeckPlan.DrawMainRebar(&ReBarDom, pBxStt, pBxEnd, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE);
			DBDeckPlan.DrawMainRebarAtSkew(&ReBarDom, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE, TRUE, TRUE);
			DBDeckPlan.DrawSabogangRebar(&ReBarDom, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE, TRUE, TRUE);					
			DBDeckPlan.DrawBeryukRebarOnCenter(&ReBarDom, pBxStt, pBxEnd, bUpper,bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE);
			DBDeckPlan.DrawBeryukRebarOnJijum(&ReBarDom, pBxStt, pBxEnd, bUpper, bUpper?DRAW_RIGHTSIDE:DRAW_LEFTSIDE);

			CObArc *pCircle = (CObArc *)ObCircleArrayCant.GetAt(nx);
			ReBarDom.TrimByCircle2(pCircle->m_SttPoint, pCircle->m_Radius, TRUE);						
			Dom << ReBarDom;
		}
	}

	// �󼼵��� ��ũ
	DBDeckPlan.DrawMarkCB_ST_OBK(&Dom, nSubPage-1);
	DBDeckPlan.DrawMarkSangseDanbu(&Dom, nSubPage-1);
	DBDeckPlan.DrawMarkSangseCant(&Dom, nSubPage-1);

	//  ����ȸ��
	pBxStt = GetBxSttPage(nSubPage,-1);
	pBxEnd = GetBxEndPage(nSubPage,-1);

	double rotAng  = DBPlan.GetRotateAnglePyung(pBxStt,pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	Dom.Move(-MoveXY);
	Dom.Rotate(rotAng,0,0);

	double dY = pBridge->GetGirder(0)->m_dGirderCenter;
	Dom.Move(0, dY);

	*pDom << Dom;
}

void CAPlateDrawDeckRebar::AddDimPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CDrawDBBaseRebarPlan DBDeckPlan(pStd->GetDataManage());	
	CAPlateDrawPyung DBPlan(pStd->GetDataManage());
	CDimDBDeckBasePlan DBDeckDimPlan(pStd->GetDataManage());

	long nGir = 0;
	CPlateGirderApp *pGirDim = pBridge->GetGirder(nGir);
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage,nGir);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage,nGir);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);

	DBDeckDimPlan.DimPyungHDan(&Dom, pBxStt, DIMDECK_BASEHDAN_LOWER | DIMDECK_BASEHDAN_UPPER | DIMDECK_BASEHDAN_TOTAL, TRUE, 2);
	DBDeckDimPlan.DimPyungHDan(&Dom, pBxEnd, DIMDECK_BASEHDAN_LOWER | DIMDECK_BASEHDAN_UPPER | DIMDECK_BASEHDAN_TOTAL, FALSE, 2);		

	long  nSttDan = 1;
	DBDeckDimPlan.DimPyungTotal(&Dom,pBxStt,pBxEnd,DIM_DECK_MAINREBAR_UPPER | DIM_DECK_TOTAL_LEFT, TRUE, FALSE,"",nSttDan);	
	DBDeckDimPlan.DimPyungTotal(&Dom,pBxStt,pBxEnd,DIM_DECK_MAINREBAR_LOWER | DIM_DECK_TOTAL_RIGHT,FALSE,FALSE,"",nSttDan);

	//////////////////////////////////////////////////////////////////////////
	// �纸�� ö�� ġ����
	CDomyun DomAngleDim(pDom);
	pStd->m_pOptStd->SetEnvType(&DomAngleDim, HCAD_DIML);	

	CDPoint xyStt;
	CString szData;
	double dStaStt = 0;
	double LDist = pDeckData->GetSideBarDistFromCenterGir(TRUE, TRUE);
	double RDist = pDeckData->GetSideBarDistFromCenterGir(TRUE, FALSE);
	// ������ ���
	CPlateGirderApp *pGirCen = pBridge->GetGirder(-1);
	double dSttBlock = pStd->GetDataManage()->GetGlobalOption()->GetSttExpansionJointWidth();
	double dEndBlock = pStd->GetDataManage()->GetGlobalOption()->GetEndExpansionJointWidth();
	if(pDeckData->m_bInstallMainRebarAtBlockOut)
	{
		dSttBlock  = 0;
		dEndBlock  = 0;
	}

	if(pBxStt->IsState(BX_STT_JOINT))
	{		
		CDPointArray xyDim = pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT];
		for(long i=0; i<xyDim.GetSize(); i++)
		{		
			CDPoint ptData = xyDim.GetAt(i);
			if(ptData.x==1)		szData = COMMA(ptData.y);
			else				szData.Format("%.0f@%.0f=%s",ptData.x,ptData.y,COMMA(ptData.x*ptData.y));			

			double dSta = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB)+dSttBlock+pDeckData->m_dCoverSd, LDist, pGirCen->GetAAA(OFF_STT_SLAB));
			CDPoint ptBase = pGirCen->GetXyGirderDis(dSta, LDist);

			DomAngleDim.SetDirection("TOP");
			DomAngleDim.SetDimObq(90-pBxStt->GetAngle().GetAngleDegree());
			if(i==0) 
			{
				xyStt   = ptBase;
				dStaStt = dSta;
				DomAngleDim.DimMoveTo(ptBase.x, ptBase.y,0);
			}
			DomAngleDim.DimLineTo(ptData.x*ptData.y,0,szData);					
		}
		DomAngleDim.Rotate(xyStt, pGirCen->GetLine()->GetAngleAzimuth(dStaStt));
	}
	Dom << DomAngleDim;
	// ������ �Ϻ�
	if(pBxStt->IsState(BX_STT_JOINT))
	{		
		CDPointArray xyDim = pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT];
		for(long i=0; i<xyDim.GetSize(); i++)
		{		
			CDPoint ptData = xyDim.GetAt(i);
			if(ptData.x==1)		szData = COMMA(ptData.y);
			else				szData.Format("%.0f@%.0f=%s",ptData.x,ptData.y,COMMA(ptData.x*ptData.y));			

			double dSta = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB)+pDeckData->m_dCoverSd, RDist, pGirCen->GetAAA(OFF_STT_SLAB));
			CDPoint ptBase = pGirCen->GetXyGirderDis(dSta, RDist);
			
			DomAngleDim.SetDirection("BOTTOM");	
			DomAngleDim.SetDimObq(-(90-pBxStt->GetAngle().GetAngleDegree()));
			if(i==0) 
			{
				xyStt = ptBase;
				dStaStt = dSta;
				DomAngleDim.DimMoveTo(ptBase.x, ptBase.y,0);
			}
			DomAngleDim.DimLineTo(ptData.x*ptData.y,0,szData);					
		}
		DomAngleDim.Rotate(xyStt, pGirCen->GetLine()->GetAngleAzimuth(dStaStt));
	}
	Dom << DomAngleDim;

	// ������ ���
	LDist = pDeckData->GetSideBarDistFromCenterGir(FALSE, TRUE);
	RDist = pDeckData->GetSideBarDistFromCenterGir(FALSE, FALSE);
	if(pBxEnd->IsState(BX_END_JOINT))
	{		
		CDPointArray xyDim = pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END];
		double	dSum = 0;
		long	i	 = 0;
		for(i=0; i<xyDim.GetSize(); i++)
			dSum += xyDim.GetAt(i).x*xyDim.GetAt(i).y;

		double dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB)-dEndBlock-pDeckData->m_dCoverSd-dSum, LDist, pGirCen->GetAAA(OFF_END_SLAB));
		CDPoint ptBase = pGirCen->GetXyGirderDis(dStaStt, LDist);
		DomAngleDim.SetDirection("TOP");
		DomAngleDim.SetDimObq(90-pBxEnd->GetAngle().GetAngleDegree());
		DomAngleDim.DimMoveTo(ptBase.x, ptBase.y, 0);

		for(i=0; i<xyDim.GetSize(); i++)
		{						
			CDPoint ptData = xyDim.GetAt(xyDim.GetUpperBound()-i);
			if(ptData.x==1)		szData = COMMA(ptData.y);
			else				szData.Format("%.0f@%.0f=%s",ptData.x,ptData.y,COMMA(ptData.x*ptData.y));			
			DomAngleDim.DimLineTo(ptData.x*ptData.y, 0, szData);					
		}
		DomAngleDim.Rotate(ptBase, pGirCen->GetLine()->GetAngleAzimuth(dStaStt));
	}
	Dom << DomAngleDim;
	// ������ �Ϻ�
	if(pBxEnd->IsState(BX_END_JOINT))
	{		
		CDPointArray xyDim = pDeckData->m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END];
		double	dSum = 0;
		long	i	 = 0;
		for(i=0; i<xyDim.GetSize(); i++)
			dSum += xyDim.GetAt(i).x*xyDim.GetAt(i).y;
		
		double dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB)-pDeckData->m_dCoverSd-dSum, RDist, pGirCen->GetAAA(OFF_END_SLAB));
		CDPoint ptBase = pGirCen->GetXyGirderDis(dStaStt, RDist);
		DomAngleDim.SetDirection("BOTTOM");
		DomAngleDim.SetDimObq(-(90-pBxEnd->GetAngle().GetAngleDegree()));
		DomAngleDim.DimMoveTo(ptBase.x, ptBase.y, 0);

		for(i=0; i<xyDim.GetSize(); i++)
		{						
			CDPoint ptData = xyDim.GetAt(xyDim.GetUpperBound()-i);
			if(ptData.x==1)		szData = COMMA(ptData.y);
			else				szData.Format("%.0f@%.0f=%s",ptData.x,ptData.y,COMMA(ptData.x*ptData.y));			

			DomAngleDim.DimLineTo(ptData.x*ptData.y, 0, szData);					
		}
		DomAngleDim.Rotate(ptBase, pGirCen->GetLine()->GetAngleAzimuth(dStaStt));
	}
	Dom << DomAngleDim;	
	
	//////////////////////////////////////////////////////////////////////////
	// Match Line
	if(pDomOpt->m_bSlab_MatchLine)
	{
		long nG = 0;
		pBxStt = GetBxSttPage(nSubPage,nG, FALSE);
		pBxEnd = GetBxEndPage(nSubPage,nG, FALSE);
		long nMLDan	   = 5;
		long nRealPage = nSubPage;
		if(!pBxStt->IsState(BX_STT_GIRDER))
			DimTextPyungMatchLine(&Dom, pBxStt, nRealPage-1, nMLDan);
		if(!pBxEnd->IsState(BX_END_GIRDER))
			DimTextPyungMatchLine(&Dom, pBxEnd, nRealPage  , nMLDan);
	}

	*pDom << Dom;
}


void CAPlateDrawDeckRebar::AddJong(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CDrawDBBaseRebarJong DBDeckJong(pStd->GetDataManage());	
	CDimDBDeckBaseJong DBDeckDimJong(pStd->GetDataManage());	

	CDomyun Dom(pDom);

	int  nJigan   = nSubPage - 1;
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarJong);
	
	// ������
	DBDeckJong.DrawBeryukRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos, 0, FALSE);
	DBDeckJong.DrawMainRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos);
	DBDeckJong.DrawShearRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos);
	// ġ����
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DBDeckDimJong.DimJongTotal(&Dom, nJigan, DIM_DECK_MAINREBAR_UPPER, pDomOpt->m_nDeck_JongDrawPos, pDomOpt->m_nDeck_Slab_DivType==1);
	DBDeckDimJong.DimJongTotal(&Dom, nJigan, DIM_DECK_MAINREBAR_LOWER, pDomOpt->m_nDeck_JongDrawPos, pDomOpt->m_nDeck_Slab_DivType==1);
	DBDeckJong.DrawMarkBeryukRebarAtJong(&Dom, 0, nJigan, pDomOpt->m_nDeck_JongDrawPos);
	DBDeckJong.DrawMarkMainRebarAtJong(&Dom, nJigan, pDomOpt->m_nDeck_JongDrawPos);
	DBDeckJong.DrawMarkShearRebar(&Dom, nJigan, pDomOpt->m_nDeck_JongDrawPos);

	// Trim
	double dPosStt = 0, dPosEnd = 0;
	BOOL bCutLeft = FALSE, bCutRight = FALSE;
	pDeckData->GetPosJiganAtJong(nJigan, dPosStt, dPosEnd, pDomOpt->m_nDeck_JongDrawPos);

//	if(nJigan == 0)  bCutLeft = FALSE;
//	else			 bCutLeft = TRUE;
//
//	if(nJigan == (pBridge->m_nQtyJigan - 1))	bCutRight = FALSE;
//	else									bCutRight = TRUE;

	if(pDomOpt->m_nDeck_Slab_DivType==0)
	{
		if(nJigan == 0)  bCutLeft = FALSE;
		else			 bCutLeft = TRUE;

		if(nJigan == (pBridge->m_nQtyJigan - 1))	bCutRight = FALSE;
		else										bCutRight = TRUE;
	}

	double  dSpace = 200;	// ���ܼ� ¥����
	CDPoint ptSttUpper = CDPoint(dPosStt, dSpace);
	CDPoint ptSttLower = CDPoint(dPosStt, -pBridge->m_dThickSlabBase - dSpace);
	CDPoint ptEndUpper = CDPoint(dPosEnd, dSpace);
	CDPoint ptEndLower = CDPoint(dPosEnd, -pBridge->m_dThickSlabBase - dSpace);
	double dCutLength = 10000000;

	if(bCutLeft)
	{
		CObLayer Lay;
		CObLine Line(ptSttUpper, ptSttLower, &Lay);

		Dom.TrimByLine(&Line, ptSttUpper + CDPoint(1,0)*dCutLength); 
		CDRect rectErase(0 - dCutLength, ptSttUpper.y + dCutLength,	ptSttLower.x, ptSttLower.y - dCutLength);
		Dom.EraseRect(rectErase);

		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CUTL);
		Dom.CutLightning(ptSttUpper.x, ptSttUpper.y, ptSttLower.x, ptSttLower.y, TRUE, 1); 
	}

	if(bCutRight)
	{
		CObLayer Lay;
		CObLine Line(ptEndUpper, ptEndLower, &Lay);
		Dom.TrimByLine(&Line, ptEndUpper + CDPoint(-1,0)*dCutLength); 
		CDRect rectErase(ptEndUpper.x, ptEndUpper.y + dCutLength, 
			ptEndLower.x + dCutLength, ptEndLower.y - dCutLength);
		Dom.EraseRect(rectErase);

		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CUTL);
		Dom.CutLightning(ptEndUpper.x, ptEndUpper.y, ptEndLower.x, ptEndLower.y, TRUE, 1); 
	}


	CDRect rect;
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.Move(-rect.left, -rect.bottom);

	*pDom << Dom;
}



CString CAPlateDrawDeckRebar::AddFront(CDomyun *pDomP, long nSubPage, BOOL bLeft)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);

	CAPlateDrawDanmyun DrawDan(pStd->GetDataManage());
	CDrawDBFrontRebar DBFrontRebar(pStd->GetDataManage());

	long nTypeFront = bLeft ? long(pDomOpt->m_dDeck_TypeRebarFrontLeft) : long(pDomOpt->m_dDeck_TypeRebarFrontRigh);
	long nSection = 0;
	CString szHDan;
	if(nTypeFront==0)	// ������
	{
		if(bLeft && m_pBxStt->IsState(BX_STT_JOINT))
			nSection = SECTION_DAN;
		else if(!bLeft && m_pBxEnd->IsState(BX_END_JOINT))
			nSection = SECTION_DAN;
		else
			nSection = SECTION_PIER;
	}
	else if(nTypeFront==1)
	{
		nSection = SECTION_CEN;
	}

	long nJijumStt = nSubPage-1;
	long nJijumEnd = nSubPage;
	if(nTypeFront == 0)
		szHDan.Format("������ %s", pBridge->m_strJijumName[bLeft?nJijumStt:nJijumEnd]);
	else if(nTypeFront == 1)
		szHDan.Format("�߾Ӻ� %s~%s", pBridge->m_strJijumName[nJijumStt], pBridge->m_strJijumName[nJijumEnd]);

	CPlateBasicIndex *pBx = pDeckData->GetBxBySection(nSection);
	long nCycle = 0;
	double dTextRatio = 1;
	// Ⱦ�ܸ鵵
	DrawDan.DrawDanmyunOnBx(&Dom,pBx,FALSE, FALSE);
	DBFrontRebar.DrawMainRebar(&Dom, nSection, nCycle, FALSE, FALSE);
	DBFrontRebar.DrawMainRebarShear(&Dom, nSection);
	DBFrontRebar.DrawBeryukRebar(&Dom, nSection);
	DBFrontRebar.DimMainRebarUp(&Dom, nSection, dTextRatio, FALSE, FALSE);
	DBFrontRebar.DimMainRebarLo(&Dom, nSection, FALSE);
	DBFrontRebar.DimMainRebarRigh(&Dom, nSection, FALSE);

	// �������� �̵�,ȸ��
	CPlateGirderApp *pGir = pBridge->GetGirder(-1);
	Dom.Mirror(0,TRUE);
	Dom.Rotate(CDPoint(0,0), CDPoint(0,1));
	Dom.Move(0,-pGir->m_dGirderCenter);
	double dY = pBridge->GetGirder(0)->m_dGirderCenter;
	Dom.Move(0, dY);
	*pDomP << Dom;

	// ��ũ�� Mirror�� ���� �ʰ� X�ุ ������ �ٲ�
	DBFrontRebar.DrawMarkMainRebar(&Dom, nSection, 0, TRUE);
	DBFrontRebar.DrawMarkBeryukRebar(&Dom, nSection, TRUE);
	DBFrontRebar.DrawMarkShearRebar(&Dom, nSection, TRUE);

	Dom.Rotate(CDPoint(0,0), CDPoint(0,1));
	Dom.Move(0,-pGir->m_dGirderCenter);
	Dom.Move(0, dY);

	*pDomP << Dom;

	return szHDan;
}


void CAPlateDrawDeckRebar::AddFrontDetail(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CAPlateDrawDanmyun DrawDan(pStd->GetDataManage());
	CDrawDBFrontRebar DBFrontRebar(pStd->GetDataManage());

	double dFrontDetScale = pDomOpt->m_dSlab_ScaleFrontDet;
	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetScaleDim(dFrontDetScale);

	CPlateBasicIndex *pBx  = GetBxDimensionTable(nSubPage);
	long nG = pBridge->GetGirdersu()-1;
	CPlateGirderApp	*pGir = pBridge->GetGirder(nG);

	for(long i=0; i<pBridge->GetGirdersu(); i++)
	{
		CPlateGirderApp	*pGirMatch = pBridge->GetGirder(i);
		if(pBx->GetBxMatchByCrossBeam(pGirMatch)==NULL)
		{			
			return;
		}
	}

	// Ⱦ�ܸ鵵
	DrawDan.DrawDanmyunOnBx(&Dom,pBx, FALSE, FALSE);
	DrawDan.DimTypicalSectionBottom(&Dom,pBx);	


	// Ⱦ�ܸ鵵 ġ����
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	long  nSttDan = 0;
	Dom.SetDirection("TOP");
	DrawDan.DimFrontHDanExtTop(&Dom,pBx,nSttDan);
	Dom.SetDirection("BOTTOM");
	DrawDan.DimFrontHDanExtBottom(&Dom,pBx,nSttDan);
	
	//AddFrontDetailSlabDim(&Dom, nSubPage);
	
	// ���� �� �ڽ�ġ��
	for(long n=0; n<pBridge->GetGirdersu()-1; n++)	
	{
		if(!pBridge->IsExtLine(n))
			break;
	}
	DrawDan.DimTextSlopeArrow(&Dom,pBx,m_pBxStt,m_pBxEnd, TRUE);

	double dDisSlabRight  = pBridge->GetDisSlabRightDom(pBx);
	CPlateBasicIndex *pBxMatchRightEnd  = pBx->GetBxMatchByCrossBeam(pGir);
//	CDimDomyunDBRoad DimDom(pDomP);
//	pStd->m_pOptStd->SetEnvType(&DimDom,HCAD_DIML);
//	DimDom.SetScaleDim(dFrontDetScale);
//	DimDom.SetDirection("RIGHT");
//	DimDom.DimMoveTo(dDisSlabRight,pGir->GetElevationDom(pBxMatchRightEnd)-pBxMatchRightEnd->GetHeightBoxByBx(),1);
//	DimDom.DimLineTo(dDisSlabRight,pBxMatchRightEnd->GetHeightBoxByBx(),COMMA(pBxMatchRightEnd->GetHeightBoxByBx()));
//	Dom << DimDom;

	Dom.RedrawByScale(GetScale()/dFrontDetScale);

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	double MoveX = (m_rectDgk.left+m_rectDgk.right)/2+rect.Width()/4;
	double MoveY = pDomP->Always(110);
	Dom.Move(MoveX, MoveY);
	Dom.Move(pMove->GetPoint(MPE_REBAR_HDANDETAIL));
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,rect,MPE_REBAR_HDANDETAIL,"Ⱦ�ܸ鵵 �󼼵�");
	*pDomP << Dom;

	Dom.SetScaleDim(GetScale());
	DrawStd.AddTitleOnDom(&Dom, dFrontDetScale, rect, "Ⱦ�ܸ鵵 �󼼵�", TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(MPE_REBAR_HDANDETAIL+1));
	Dom.SetCalcExtRect();
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_HDANDETAIL+1, "Ⱦ�ܸ鵵 �󼼵� ����");
	*pDomP << Dom;
}

void CAPlateDrawDeckRebar::AddKeyPlan(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CAPlateDrawPyung DBPlan(pStd->GetDataManage());	
	CAPlateDrawDimPyung DimPlan(pStd->GetDataManage());	

	double HScale = pDomOpt->m_dSlab_ScaleKeyPlanHor;
	double VScale = pDomOpt->m_dSlab_ScaleKeyPlanVer;

	CDomyun Dom(pDomP);
//	Dom.SetScaleDim(HScale);

	BOOL bSpliceMark = pDomOpt->m_bSlab_KeyPlanSpliceMark;
	BOOL bJijumMark	 = pDomOpt->m_bSlab_KeyPlanJijumMark;
	BOOL bCrossBeam  = pDomOpt->m_bSlab_KeyPlanCrossBeam;
	BOOL bDimension	 = pDomOpt->m_bSlab_KeyPlanDimension;
	BOOL bDirection	 = pDomOpt->m_bSlab_KeyPlanDirection;
	BOOL bOneLine	 = pDomOpt->m_bSlab_KeyPlanOneLine;

	// �������
	CString sBridgeDir = pDomOpt->m_sTextBridgeDirection;
	CString sCounterDir = pDomOpt->m_sTextCounterDirection;

	// KEY PLAN �׸���
	DBPlan.AddKeyPlan(&Dom ,NULL, NULL, HScale, FALSE, bSpliceMark, bJijumMark, bDirection, bDimension, bCrossBeam, bOneLine, -1, -1, FALSE, FALSE, sBridgeDir, sCounterDir);

	// HATCH �ֱ�
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage,-1);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage,-1);
	if(pBxStt->IsState(BX_STT_GIRDER)) pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
	if(pBxEnd->IsState(BX_END_GIRDER)) pBxEnd = Finder.GetBxFirst(BX_END_SLAB);
	DBPlan.DrawHatchInRectOnSlab(&Dom,pBxStt,pBxEnd);

	// ġ����	
	pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	pBxEnd = Finder.GetBxLast(BX_END_GIRDER);
	Dom.SetScaleDim(HScale);
	//if(bDimension)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
		//		CPlateBxFinder Finder(pBridge->GetGirder(-1));
		//
		__int64 nTypeUpper  = DIM_BASE_SPAN;
		long  nSttDan = 1;
		DimPlan.DimPyungTotal(&Dom,pBxStt,pBxEnd,nTypeUpper,FALSE,FALSE,"",nSttDan);
	}

	// �������
	if(bDirection)
	{
		if(sBridgeDir.GetLength())
			DimTextMarkDirection(&Dom,sBridgeDir,TRUE);
		if(sCounterDir.GetLength())
			DimTextMarkDirection(&Dom,sCounterDir,FALSE);
	}
	

	// ���� �̵� �� ȸ��, ����������

	pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	pBxEnd = Finder.GetBxLast(BX_END_GIRDER);
	
	double rotAng  = DBPlan.GetRotateAnglePyung(pBxStt,pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	Dom.Move(-MoveXY);	// �����߽� (0,0)
	Dom.Rotate(rotAng,0,0);

	BOOL bRotate = pDomOpt->m_bSlab_bKeyPlanRoVer;
	if (bRotate)
		Dom.Rotate(ConstPi/2, 0,0);

	Dom.SetCalcExtRect();	
	CDRect rectOutLine = Dom.GetExtRect();
/*//	if (pDomOpt->m_KeyPlanbOutLine)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
		rectOutLine.NormalizeRect();
		rectOutLine.left -= Dom.Always(10);
		if (pDomOpt->m_bSlab_bKeyPlanRoVer)
			rectOutLine.left -= Dom.Always(40);
		rectOutLine.top -= Dom.Always(10);
		rectOutLine.right += Dom.Always(10);
		if (pDomOpt->m_bSlab_bKeyPlanRoVer)
			rectOutLine.bottom += Dom.Always(10);
		else
			rectOutLine.bottom += Dom.Always(50);
		Dom.Rectangle(rectOutLine);
	}
*/		
	Dom.RedrawByHVScale(HScale/VScale,FALSE,FALSE, FALSE);
	Dom.RedrawByScale(GetScale()/HScale);

	CDRect rect;
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.Move(-rect.left+pDomP->Always(700),-rect.bottom+pDomP->Always(50));
	rect = Dom.GetExtRect();
	Dom.Move(pMove->GetPoint(MPE_REBAR_KEYPLAN));
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_KEYPLAN,"��൵");
	*pDomP << Dom;

	Dom.SetScaleDim(GetScale());
/*//	if (pDomOpt->m_KeyPlanbOutLine)
	{
		if (pDomOpt->m_bSlab_bKeyPlanRoVer)
			rect.left += Dom.Always(40);
		else
			rect.bottom -=Dom.Always(30);
	}
*/
	Dom.SetScaleDim(GetScale());
	DrawStd.AddTitleOnDom(&Dom, HScale, rect, "��൵", TRUE, TRUE, 0, bRotate);
	Dom.Move(pMove->GetPoint(MPE_REBAR_KEYPLAN+1));
	Dom.SetCalcExtRect();
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_KEYPLAN+1, "��൵ ����");
	*pDomP << Dom;
}



void CAPlateDrawDeckRebar::DrawSlabByPage(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CDomyun Dom(pDomP);
	long nG = 0;
	CPlateBasicIndex *pBxLeftStt = GetBxSttPage(nSubPage,nG,FALSE);
	CPlateBasicIndex *pBxLeftEnd = GetBxEndPage(nSubPage,nG,FALSE);
	nG = pBridge->GetGirdersu()-1;
	CPlateBasicIndex *pBxRightStt = GetBxSttPage(nSubPage,nG,FALSE);
	CPlateBasicIndex *pBxRightEnd = GetBxEndPage(nSubPage,nG,FALSE);
	
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());
	DrawPyung.DrawSlabByBx(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);
	DrawPyung.DrawSlabCantileverHunchLine(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);

	// ����������
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_CENT);
	if(nSubPage==1)
		DrawPyung.DrawJointLine(&Dom, TRUE);
	if(nSubPage==pStd->GetDataManage()->GetBridge()->m_nQtyJigan)
		DrawPyung.DrawJointLine(&Dom, FALSE);

	*pDomP << Dom;
}

// nType=0 : �ܺ�
// nType=1 : �Ϲݺ�
void CAPlateDrawDeckRebar::AddMainRebar(CDomyun *pDom, long nCycle, long nSection, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CPlateBasicIndex*pBx		= pDeckData->GetBxBySection(nSection);	

	CAPlateDrawDanmyun DrawDan(pStd->GetDataManage());
	CDrawDBFrontRebar DBFrontRebar(pStd->GetDataManage());
	CDomyun	Dom(pDom);

	Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarMain);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CONC);
	DrawDan.DrawSlabUpper(&Dom, pBx, FALSE, FALSE);
	DrawDan.DrawSlabHunch(&Dom, pBx);

//	DrawDan.DrawSlabCantileverLeft(&Dom, pBx, FALSE);
//	DrawDan.DrawSlabCantileverRight(&Dom, pBx, FALSE);

	// �Ʒ��Լ� Layer�� �Լ������� ����...
	DBFrontRebar.DrawMainRebar(&Dom, nSection, nCycle, FALSE, TRUE);
	DBFrontRebar.DimMainRebarJolib(&Dom, nSection, nCycle, FALSE);
	DBFrontRebar.DrawMarkMainRebar(&Dom, nSection, nCycle);

	Dom.RedrawByScale(m_Scale/pDomOpt->m_dDeck_ScaleRebarMain);

	*pDom << Dom;
}


void CAPlateDrawDeckRebar::AddDistriRebar(CDomyun *pDom, long nCycle, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CDrawDBBaseRebarJong DBDeckJong(pStd->GetDataManage());

	CDomyun	Dom(pDom);

	Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarBelyck);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CONC);
	DBDeckJong.DrawBeryukRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos , nCycle, FALSE);
		
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DBDeckJong.DimBeryukRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos, nCycle, FALSE);
	
	DBDeckJong.DrawMarkBeryukRebarAtJongTotal(&Dom, nCycle, pDomOpt->m_nDeck_JongDrawPos);
	
	Dom.RedrawByScale(m_Scale/pDomOpt->m_dDeck_ScaleRebarBelyck);
	Dom.RedrawByHVScale(0.15, TRUE, FALSE, FALSE);
	
	*pDom << Dom;

}

void CAPlateDrawDeckRebar::AddPageBM(CDomyun *pDomP)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();

	CHgBaseConcStd HGConc;
	CHgBaseDrawStd HGDraw;

	CDomyun Dom(pDomP);
	CDPoint xyBase(0, Dom.Always(525));
	pPageMng->SetProgressData("ö����� ������...",5);

	if(pDomOpt->m_nTypeRebarTable == 0) //�Ϲ�Ÿ�� �϶� ö�ٻ�
	{
		if(pDomOpt->m_bDeckRebar_SangseMatrial)
		{
			pPageMng->SetProgressData("ö�ٻ� �����...",30);
			pDeckData->ShellSortRebarList(&pDeckData->m_arrRebarJewon, &CRebar::CompareForList);
			AddRebarDetail(&Dom);
			*pDomP << Dom;
		}
	}

	HGConc.SetRebarIndexBySortRebarList(&pDeckData->m_arrRebarJewon, TRUE);  //���̺� ��ȣ ����

	//ö�ٺи� ���ϴ°��
	if(pDomOpt->m_nDeck_Slab_RebarTableOpt==0)
	{
		if(pDomOpt->m_bDeckRebar_SangseMatrialTable)
		{
			pPageMng->SetProgressData("ö�����ǥ �����...",80);
			pDeckData->ShellSortRebarList(&pDeckData->m_arrRebarJewon, &CRebar::CompareForTable);
			xyBase = AddRebarTable(&Dom, &pDeckData->m_arrRebarJewon);
			*pDomP << Dom;
		}
		if(pDomOpt->m_bDeckRebar_SangseMatrial)
		{
			if(pDomOpt->m_nTypeRebarTable == 1) //����ȭŸ�� ö�ٻ�
			{
				pPageMng->SetProgressData("ö�ٻ� �����...", 80);
				HGConc.ShellSortRebarList(&pDeckData->m_arrRebarJewon, &CRebar::CompareForList);
				HGDraw.DrawRebarListDetailSimple(&Dom, &pDeckData->m_arrRebarJewon, xyBase, pDomOpt->m_dWidthRebarTableRect, pDomOpt->m_dHeightRebarTableRect);

				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(MPE_BM_DETAIL));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DETAIL, _T("ö�ٻ� ����"));

				*pDomP << Dom;
			}
		}

	}
	//ö�ٺи�
	else
	{
		CSafetyTypedPtrArray<CObArray, CRebar*> arrRebar;
		const long nCountTable = 2;
		CString sTitle[nCountTable] = {"��ö��", "��Ÿö��"};
		long nIdxTable = 0;

		for(long table = 0; table < nCountTable; table++)
		{
			AhTPADelete(&arrRebar,	(CRebar*)0);
			MakeRebarList(table==0 ? 1 : 2, &arrRebar);

			if(pDomOpt->m_bDeckRebar_SangseMatrialTable)
			{
				pPageMng->SetProgressData("ö�����ǥ �����...",80);
				HGConc.ShellSortRebarList(&arrRebar,&CRebar::CompareForTable);
				xyBase	= AddRebarTable(&Dom, &arrRebar, sTitle[table], nIdxTable++, xyBase);
				*pDomP << Dom;
			}

			if(pDomOpt->m_bDeckRebar_SangseMatrial)
			{
				if(pDomOpt->m_nTypeRebarTable == 1) //����ȭŸ�� ö�ٻ�
				{
					pPageMng->SetProgressData("ö�ٻ� �����...", 80);
					HGConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
					HGDraw.DrawRebarListDetailSimple(&Dom, &arrRebar, xyBase, pDomOpt->m_dWidthRebarTableRect, pDomOpt->m_dHeightRebarTableRect);

					Dom.SetCalcExtRect();
					Dom.Move(pMove->GetPoint(MPE_BM_DETAIL + nIdxTable));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DETAIL + nIdxTable, _T("ö�ٻ� ����"));

					CDRect rect = Dom.GetExtRect();
					xyBase.x += fabs(rect.Width()+ Dom.Always(40));

					*pDomP << Dom;
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// ��ȣ���� ������ ö��
// long nType : ���� ��� 
// long nType - 0 : ����
// long nType - 1 : ��ö�ٸ�
// long nType - 2 : ��Ÿö�ٸ�
void CAPlateDrawDeckRebar::MakeRebarList(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CRebar *pRB;
	BOOL bOK = FALSE;

	long nSize = pDeckData->m_arrRebarJewon.GetSize();
	for(long nList=0; nList<nSize; nList++)
	{
		pRB = pDeckData->m_arrRebarJewon.GetAt(nList);
		if(pRB->m_sMark == "" || pRB->m_dDia==0 || pRB->m_nEa==0 || pRB->GetLength()==0) continue;

		if(nType == 0) bOK = TRUE;
		else if(nType == 1) bOK = pRB->m_bMainRebar ? TRUE : FALSE;
		else if(nType == 2) bOK = pRB->m_bMainRebar ? FALSE : TRUE;
		else bOK = FALSE;
		
		if(bOK)
			pArrRebar->Add(new CRebar(pRB));
	}
}

//////////////////////////////////////////////////////////////////////////
// ��ȣ�� ö��
// long nType : ���� ��� 
// long nType - 0 : ����
// long nType - 1 : ��ö�ٸ�
// long nType - 2 : ��Ÿö�ٸ�
void CAPlateDrawDeckRebar::MakeRebarListGuard(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CRebar *pRB;
	BOOL bOK = FALSE;

	long nSize = pDeckData->m_arrRebarJewonGuard.GetSize();
	for(long nList=0; nList<nSize; nList++)
	{
		pRB = pDeckData->m_arrRebarJewonGuard.GetAt(nList);
		if(pRB->m_sMark == "" || pRB->m_dDia==0 || /*pRB->m_nEa==0 ||*/ pRB->GetLength()==0) continue;

		if(nType == 0) bOK = TRUE;
		else if(nType == 1) bOK = pRB->m_bMainRebar ? TRUE : FALSE;
		else if(nType == 2) bOK = pRB->m_bMainRebar ? FALSE : TRUE;
		else bOK = FALSE;
		
		if(bOK)
			pArrRebar->Add(new CRebar(pRB));
	}
}

void CAPlateDrawDeckRebar::AddRebarDetail(CDomyun *pDomP)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	CRebar			*pRB		= NULL;

	CConcSymbol symDB(pOpt);
	CDomyun Dom(pDomP);
	CHgBaseDrawStd HGDraw;
	double dSpace = pDomOpt->m_dDeck_SpaceBetweenJewon;
	double dMaxY  = Dom.Always(525);
	double dMinY  = Dom.Always(60);
	double dYPlus = 0;
	CDPoint xy(Dom.Always(70),dMaxY);
	

	pOpt->SetEnvType(&Dom, HCAD_SUB_TITLE);
	Dom.TextOut(xy.x+Dom.Always(50),xy.y+Dom.GetTextHeight()/2,"ö �� �� ��");
	Dom.Move(pMove->GetPoint( MPE_BM_DETAIL-1));
	pStd->ChangeSeperateLayer(&Dom);
	Dom.SetCalcExtRect();	
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DETAIL-1, "ö�ٻ� ����");
	*pDomP << Dom;

	pOpt->SetEnvType(&Dom,HCAD_DIML);

	long nSize  = pDeckData->m_arrRebarJewon.GetSize();
	long nRow   = 0;
	long nType  = 0;
	long nDetailCol = 0;
	BOOL bJoint = FALSE;
	for(long i=0; i<nSize; i++)
	{
		pRB		= pDeckData->m_arrRebarJewon.GetAt(i);
		if(pRB->m_nTypeRebar!=nType || bJoint!=(pRB->m_nJoint>0))
		{
			nRow += 2;
			nType	= pRB->m_nTypeRebar;
			bJoint	= (pRB->m_nJoint>0) ? TRUE : FALSE;
			if((dMaxY-dSpace*nRow) < dMinY)
			{
				Dom.Move(pMove->GetPoint(MPE_BM_DETAIL+nDetailCol*10));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DETAIL+nDetailCol*10, "ö�ٻ� "+COMMA(1+nDetailCol));
				*pDomP << Dom;
				nDetailCol++;
				xy.x += Dom.Always(120);
				xy.y  = dMaxY;
				nRow  = 3;
			}

			CDPoint xyDrawPoint = CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus);
			nRow = HGDraw.GetPlusCntRowDrawSymbolHeight(&Dom, pOpt, nRow, nType, dSpace,  i, bJoint, xyDrawPoint);

			xy.y   = dMaxY - dSpace*nRow;
			dYPlus = (nType==1) ? Dom.Always(8) : 0;
			symDB.RebarDetailBasic(&Dom,nType,CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus),bJoint);
		}
		else
			xy.y = dMaxY - dSpace*nRow;
		if(xy.y < dMinY)
		{
			Dom.Move(pMove->GetPoint(MPE_BM_DETAIL+nDetailCol*10));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DETAIL+nDetailCol*10, "ö�ٻ� "+COMMA(1+nDetailCol));
			*pDomP << Dom;
			nDetailCol++;
			xy.x += Dom.Always(120);
			xy.y  = dMaxY;
			nRow  = 0;
		}
		symDB.DrawJewonBar(&Dom,pRB,xy);
		nRow++;
	}

	Dom.Move(pMove->GetPoint(MPE_BM_DETAIL+nDetailCol*10));
	pStd->ChangeSeperateLayer(&Dom);
	Dom.SetCalcExtRect();
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DETAIL+nDetailCol*10, "ö�ٻ� "+COMMA(1+nDetailCol));
	*pDomP << Dom;
}


CDPoint CAPlateDrawDeckRebar::AddRebarTable(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, long nIdxTable, CDPoint xyBase)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	
	CConcSymbol symDB(pStd->m_pOptStd);

	CHgBaseConcStd HGConc;
	CHgBaseDrawStd HGDraw;
	
	long nDefTable			= MPE_TITLE+MPE_BM_TABLE;
	CString sTitle			= _T("ö �� �� �� ǥ");
	if(sSubTitle != "") sTitle += "(" + sSubTitle + ")";
	switch(nIdxTable)
	{
	case 0: nDefTable	= MPE_BM_TABLE; break;
	case 1: nDefTable	= MPE_BM_TABLE1; break;
	default: nDefTable	= MPE_BM_TABLE; break;
	}

	long nTableType = pDomOpt->m_nTypeRebarTable;
	long nType =  pDomOpt->m_nDeck_Slab_RebarTableOpt;

	CGridDomyunEx Dom(pDomP);
	if(nIdxTable == 0) 
		xyBase	= CDPoint(Dom.Always(650),Dom.Always(525));
	if(nIdxTable == 0 && nTableType == 1)
		xyBase.x = Dom.Always(30);


	//�⺻Ÿ���̰ų�, ����ȭ�ε� ���̺� �������� ��츸 ���
	if(nTableType == 0 || (nTableType == 1 && nType == 1))
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
		Dom.TextOut(xyBase.x+Dom.Always(155/2),xyBase.y+Dom.Always(10),sTitle);
		Dom.Move(pMove->GetPoint(MPE_TITLE+nDefTable));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE+nDefTable, sTitle);
		*pDomP << Dom;
	}

	double dLengthSum		= 0;
	double dLengthSumAdd	= 0;
	double dLengthTotal		= 0;	
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;
	long nSize = pArrRebar->GetSize();
	long nMaxRow = (long)pDomOpt->m_dDeck_DatasuOnRebarTable;

	double dFy = toKgPCm2(pDeckData->m_SigmaY);
	BOOL bShadeOutLine = TRUE;
	
	CDRect rtTable(0, 0, 0, 0);
	long nIndex = 0;
	long nTableSu = 0;
	while(1)
	{
		if(nIndex>=nSize) break;
		if(nTableType == 0) //�Ϲ�Ÿ��
		{
			nIndex = HGDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, 
				dWeightTotal, dWeightTotalAdd, *pArrRebar,dFy, bShadeOutLine);
		}
		else //����ȭŸ��
		{
			nIndex = HGDraw.DrawRebarTableSimple(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, 
				dWeightTotal, dWeightTotalAdd, *pArrRebar,dFy, bShadeOutLine);
		}
		Dom.SetCalcExtRect();
		rtTable = Dom.GetExtRect();

		Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "ö�����ǥ");
		*pDomP << Dom;
		xyBase.x += rtTable.Width() + Dom.Always(10);
		nTableSu++;
	}		

	return xyBase;
}



void CAPlateDrawDeckRebar::DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDan)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd		    *pDomOpt    = pStd->GetDomOptionStd();  
	CPlateBridgeApp     *pBridge  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp     *pGir = pBx->GetGirder();
	CPlateGirderApp     *pGirSec = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	CPlateBasicIndex	*pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);
	CARoadOptionStd		*pOpt		= pStd->GetDataManage()->GetOptionStd();

	if(pBxSec==NULL) return;

	CDPoint CLeft[4], CRigh[4];
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(0));
	pBridge->GetXyFrontCantilever(pBxMatch, CLeft, TRUE);
	pBxMatch = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(pBridge->GetGirdersu()-1));
	pBridge->GetXyFrontCantilever(pBxMatch, CRigh, FALSE);
	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_TXTG);
	CString str;
	double sta = pBx->GetStation();
	CDPoint XY(0,0);
	double LDist = -pBridge->GetGirder(0)->GetWidthFlangeUpper(pBx)/2 - (CLeft[3].x-CLeft[0].x);
	double RDist =  pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetWidthFlangeUpper(pBx)/2 + (CRigh[0].x-CRigh[3].x);
	
	pOpt->SetEnvType(&Dom, HCAD_CENT);
	XY = pGir->GetXyGirderDis(sta, LDist-Dom.Always(5), pBx->GetAngle());
	Dom.MoveTo(XY.x, XY.y);	
	XY  = pGirSec->GetXyGirderDis(pBxSec->GetStation(), RDist+Dom.Always(5), pBxSec->GetAngle());
	Dom.LineTo(XY.x, XY.y);

	*pDom << Dom;
}

void CAPlateDrawDeckRebar::AddSangseETC(CDomyun *pDom, long nIndex)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();

	CAPlateDrawStandard	DrawStd(pStd->GetDataManage());
	CDrawOpenRebar		DrawOpen(pStd->GetDataManage());
	CDrawSupportRFBar	DrawSptRFBar(pStd->GetDataManage());
	CDrawSpacer			DrawSpacer(pStd->GetDataManage());
	CDrawDBFrontRebar   DBFrontRebar(pStd->GetDataManage());

	CDomyun			Dom(pDom);

	CString	csSection	= "Afx Directory";
	CString	csWorkItem	= "Fixed Directory";
	CString	szPath		= AfxGetApp()->GetProfileString(csSection, csWorkItem);
	CString	szName		= _T("");
	double	dMoveX		= 0;
	double	dMoveY		= 0;
	CDRect rtPlan(0,0,0,0);
	CDRect dgkRect		= pDom->GetExtRect();
	
	if(pDeckData->m_bInstallWaterHole && pDomOpt->m_bDeckRebar_SangseWaterReinforce)
	{
		DrawOpen.SetType(pDeckData->m_nTypeWaterHole);
		pPageMng->SetProgressData("����� ���� �󼼵� �����...",10);
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce);
		DrawOpen.DrawRebar(&Dom);
		DrawOpen.DimRebar(&Dom);
		DrawOpen.MarkRebar(&Dom);
		DrawOpen.DrawOpen(&Dom);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(50);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(100);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC,"����� ���� �󼼵�");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce, rect, "����� ���� �󼼵�", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+1));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+1,"����� ���� �󼼵� ����");
		*pDom << Dom;
	}


	if(pDomOpt->m_bDeckRebar_SangseWaterReinforce && pBridge->m_nQtyJigan > 1)
	{
		pPageMng->SetProgressData("�߾������� ����ö�� �����󼼵�...",20);
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarSangseCentReinforce);
		szName.Format("%s\\BlockDomyun\\CenterReinforceDetail.dxf", szPath);
		Dom.DxfIn(szName);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarSangseCentReinforce);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(350);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(100);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+2,"�߾������� ����ö�� �����󼼵�");
		ReplaceData(&Dom, "");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarSangseCentReinforce, rect, "�߾������� ����ö�� �����󼼵�", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+3));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+3,"�߾������� ����ö�� �����󼼵� ����");
		*pDom << Dom;
	}

	if(pDomOpt->m_bDeckRebar_SangseDoubleChain)
	{
		pPageMng->SetProgressData("ö�� ������ ��...",30);
		Dom.SetScaleDim(pDomOpt->m_dDEck_ScaleRebarSangseDoubleChain);
		szName.Format("%s\\BlockDomyun\\LabJointDetail.dxf", szPath);
		Dom.DxfIn(szName);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDEck_ScaleRebarSangseDoubleChain);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(600);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(100);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+4,"ö�� ������ ��");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDEck_ScaleRebarSangseDoubleChain, rect, "ö�� ������ ��", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+5));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+5,"ö�� ������ �� ����");
		*pDom << Dom;
	}

	if(pDomOpt->m_bDeckRebar_SangseChairBlock)
	{
		pPageMng->SetProgressData("SPACE-BAR �� CHAIR MORTAR BLOCK ��...",40);
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleSangseChairBlock);
		szName.Format("%s\\BlockDomyun\\SpaceBarDetail.dxf", szPath);
		Dom.DxfIn(szName);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleSangseChairBlock);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(50);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(280);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+18));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+18,"SPACE-BAR �� CHAIR MORTAR BLOCK ��");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleSangseChairBlock, rect, "SPACE-BAR �� CHAIR MORTAR BLOCK ��", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+19));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+19,"SPACE-BAR �� CHAIR MORTAR BLOCK �� ����");
		*pDom << Dom;
	}

	if(pDomOpt->m_bDeckRebar_SangseShearReinforce && pDeckData->m_bInstallShearRebar)
	{
		pPageMng->SetProgressData("���� ����ö�� ��...",50);
		// ��������
		Dom.SetScaleDim(pDomOpt->m_dDeck_SacleSangseShearReinforce);
		DrawSptRFBar.DrawRebar(&Dom, 0);
		DrawSptRFBar.DimRebar(&Dom, 0);
		DrawSptRFBar.MarkRebar(&Dom, 0);
		DrawSptRFBar.DrawDetailCircle(&Dom, 0);
		DrawSptRFBar.DrawDetailCircleText(&Dom, 0, "��A");
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(pDom->GetScaleDim()/pDomOpt->m_dDeck_SacleSangseShearReinforce);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(350);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(280);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+6));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+6,"��������");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_SacleSangseShearReinforce, rect, "��������", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+7));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+7,"�������� ����");
		*pDom << Dom;

		// �� A
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleSangseA);
		DrawSptRFBar.DrawDetail(&Dom, 0, 1500);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(pDom->GetScaleDim()/pDomOpt->m_dDeck_ScaleSangseA);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(250);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(300);
		Dom.Move(dMoveX, dMoveY);
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+8));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+8,"��'A'");
		*pDom << Dom;
		
		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleSangseA, rect, "��'A'", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+9));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+9,"��'A' ����");
		*pDom << Dom;

		// �߰�������
		if(pBridge->m_nQtyJigan > 1)
		{
			Dom.SetScaleDim(pDomOpt->m_dDeck_SacleSangseShearReinforce);
			DrawSptRFBar.DrawRebar(&Dom, 1);
			DrawSptRFBar.DimRebar(&Dom, 1);
			DrawSptRFBar.MarkRebar(&Dom, 1);
			DrawSptRFBar.DrawDetailCircle(&Dom, 1);
			DrawSptRFBar.DrawDetailCircleText(&Dom, 1, "��B");
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_SacleSangseShearReinforce);//Ȯ��
			Dom.SetCalcExtRect();
			rtPlan	= Dom.GetExtRect();
			dMoveX	= Dom.Always(500);
			dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(280);
			Dom.Move(dMoveX, dMoveY);
			rect = Dom.GetExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+10));
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+10,"����������");
			*pDom << Dom;

			Dom.SetScaleDim(GetScale());
			DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_SacleSangseShearReinforce, rect, "����������", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+11));
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+11,"���������� ����");
			*pDom << Dom;

			// �� B
			Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleSangseA);
			DrawSptRFBar.DrawDetail(&Dom, 1, 1500);
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleSangseA);//Ȯ��
			Dom.SetCalcExtRect();
			rtPlan	= Dom.GetExtRect();
			dMoveX	= Dom.Always(350);
			dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(300);
			Dom.Move(dMoveX, dMoveY);
			rect = Dom.GetExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+12));
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+12,"��'B'");
			*pDom << Dom;

			Dom.SetScaleDim(GetScale());
			DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleSangseA, rect, "��'B'", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+13));
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+13,"��'B' ����");
			*pDom << Dom;
		}
	}
	
	if(pDomOpt->m_bDeckRebar_SangseShearArray)
	{
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleSangseShearArray);
		pPageMng->SetProgressData("����ö�� ��ġ��...",70);
		DrawSpacer.DrawGridRebar(&Dom);
		DrawSpacer.DrawPlaceRebar(&Dom);
		DrawSpacer.DimPlaceRebar(&Dom);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleSangseShearArray);//Ȯ��

		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(700);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(280);
		Dom.Move(dMoveX, dMoveY);

		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+14));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+14,"����ö�� ��ġ��");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleSangseShearArray, rect, "����ö�� ��ġ��", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+15));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+15,"����ö�� ��ġ�� ����");
		*pDom << Dom;
	}
	
	if(pDomOpt->m_bDeckRebar_SangseDanbu)
	{
		pPageMng->SetProgressData("������ܺ� �󼼵� �����...", 90);
		
		// ������
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarSangseDanbu);
		AddDetailDanbu(&Dom, TRUE);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarSangseDanbu);

		Dom.SetCalcExtRect();
		rtPlan = Dom.GetExtRect();
		dMoveX	= Dom.Always(20);
		dMoveY	= Dom.Always(100);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+20));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+20,"������ܺ�(������) �󼼵�");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarSangseDanbu, rect, "������ܺ�(������) �󼼵�", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+21));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+21,"������ܺ�(������) �󼼵� ����");
		*pDom << Dom;

		// ������
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarSangseDanbu);
		AddDetailDanbu(&Dom, FALSE);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarSangseDanbu);

		Dom.SetCalcExtRect();
		rtPlan = Dom.GetExtRect();
		dMoveX	= Dom.Always(20);
		dMoveY	= Dom.Always(50);
		Dom.Move(dMoveX, dMoveY);
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+22));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+22,"������ܺ�(������) �󼼵�");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarSangseDanbu, rect, "������ܺ�(������) �󼼵�", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+23));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+23,"������ܺ�(������) �󼼵� ����");
		*pDom << Dom;
	}
	
	if(pDomOpt->m_bDeckRebar_SangseCrossBeam &&  pStd->GetDataManage()->GetBridge()->m_dThickSlabHunch>0)
	{
		pPageMng->SetProgressData("���κ� �󼼵� �����...", 90);
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleSangseCrossBeam);
		DBFrontRebar.DrawSangseCrossBeam(&Dom);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleSangseCrossBeam);//Ȯ��

		Dom.SetCalcExtRect();
		rtPlan = Dom.GetExtRect();
		dMoveX	= dgkRect.Width()/2 + Dom.Always(20);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(450);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+25));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+25,"���κ� �󼼵�");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleSangseCrossBeam, rect, "���κ� �󼼵�", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+26));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+26,"���κ� �󼼵� ����");
		*pDom << Dom;
	}

//	if(pDomOpt->m_bDeckRebar_SangseStringer && pDeckData->IsBridgeHasStringer() && pStd->GetDataManage()->GetBridge()->m_dThickSlabHunch>0)
//	{
//		pPageMng->SetProgressData("���κ� �����...", 90);
//		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleSangseStringer);
//		DBFrontRebar.DrawSangseStringer(&Dom);
//		Dom.SetScaleDim(pDom->GetScaleDim());
//		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleSangseStringer);//Ȯ��
//
//		Dom.SetCalcExtRect();
//		rtPlan = Dom.GetExtRect();
//		dMoveX	= dgkRect.Width()/2 + rtPlan.Width() + Dom.Always(100);
//		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(450);
//		Dom.Move(dMoveX, dMoveY);
//		CDRect rect = Dom.GetExtRect();
//		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+27));
//		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
//		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+27,"���κ� �󼼵�");
//		*pDom << Dom;
//
//		Dom.SetScaleDim(GetScale());
//		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleSangseStringer, rect, "���κ� �󼼵�", TRUE, TRUE, 0);
//		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+28));
//		Dom.SetCalcExtRect();
//		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+28,"���κ� �󼼵� ����");
//		*pDom << Dom;
//	}
//
//	if(pDeckData->IsBridgeHasOutBracket() &&  pStd->GetDataManage()->GetBridge()->m_dThickSlabHunch>0)
//	{
//		pPageMng->SetProgressData("�������κ� �󼼵� �����...", 90);
//		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleSangseCrossBeam);
//		DBFrontRebar.DrawSangseOutBracket(&Dom);
//		Dom.SetScaleDim(pDom->GetScaleDim());
//		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleSangseCrossBeam);//Ȯ��
//
//		Dom.SetCalcExtRect();
//		rtPlan = Dom.GetExtRect();
//		dMoveX	= dgkRect.Width()/2 + rtPlan.Width() + Dom.Always(150);
//		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(450);
//		Dom.Move(dMoveX, dMoveY);
//		CDRect rect = Dom.GetExtRect();
//		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+33));
//		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
//		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+33,"�������κ� �󼼵�");
//		*pDom << Dom;
//
//		Dom.SetScaleDim(GetScale());
//		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleSangseCrossBeam, rect, "�������κ� �󼼵�", TRUE, TRUE, 0);
//		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC+34));
//		Dom.SetCalcExtRect();
//		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_REBAR_ETC+34,"�������κ� �󼼵� ����");
//		*pDom << Dom;
//	}

	if(pDomOpt->m_bDeckRebar_SangseShearArray)		// ����(2005.09.01) �ܸ����ȯ�漳���� ������ �׸��� ����...�ϴ� ����ö�ٹ�ġ�� �� ����
	{
		pPageMng->SetProgressData("������ �󼼵� �����...",10);
		//1. ��鵵 
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce);
		DrawSpacer.DrawPlanRebar(&Dom);
		DrawSpacer.DimPlanRebar(&Dom);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(200);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) - Dom.Always(170);
		Dom.Move(dMoveX, dMoveY);
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC + 30));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(), MPE_REBAR_ETC + 30, "������ ��鵵 �󼼵�");
		*pDom << Dom;

		//2. ���鵵 
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce);
		DrawSpacer.DrawFrontRebar(&Dom);
		DrawSpacer.DimFrontRebar(&Dom, FALSE);
		DrawSpacer.MarkRebar(&Dom);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce);//Ȯ��
		Dom.SetCalcExtRect();
		rtPlan	= Dom.GetExtRect();
		dMoveX	= Dom.Always(200);
		dMoveY	= (dgkRect.bottom - rtPlan.bottom) -  Dom.Always(100);
		Dom.Move(dMoveX, dMoveY);
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC + 29));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(), MPE_REBAR_ETC + 29, "������ ���鵵 �󼼵�");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleRebarSangseWaterReinforce, rect, "������ �󼼵�", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_REBAR_ETC + 31));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(), MPE_REBAR_ETC + 31, "������ �󼼵� ����");
		*pDom << Dom;
	}

}

void CAPlateDrawDeckRebar::ReplaceData(CDomyun *pDom, CString szType)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CString	szText;
	szText.Format("C.T.C%d", pDeckData->m_dJijumRebarCTCUp[POS_SLAB_CENTER]);
	pDom->ReplaceString("C.T.C125", szText);

}

void CAPlateDrawDeckRebar::AddDetailDanbu(CDomyun *pDom, BOOL bStt)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();

	CDrawDBBaseRebarJong	DBDeckJong(pStd->GetDataManage());
	CDimDBDeckBaseJong      DBDeckJongDim(pStd->GetDataManage());
	COptionStd	*pDomOpt= pStd->m_pDomOptionStd;

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarSangseDanbu);
	
	double dExtL  = 2000;
	double dLCutStt = pDeckData->GetLengthSlabToTaper(TRUE, pDomOpt->m_nDeck_JongDrawPos) + pBridge->m_dLengthTaperToTaperStt + dExtL;
	double dLCutEnd = pDeckData->GetLengthSlabToTaper(FALSE, pDomOpt->m_nDeck_JongDrawPos) + pBridge->m_dLengthTaperToTaperEnd + dExtL;
	double dSpace = 100;	// ���ܼ� ¥����

	if(bStt)
	{
		//////////////////////////////////////////////////////////////////////////
		// ������
		DBDeckJong.DrawBeryukRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos, 0, FALSE);
		DBDeckJong.DrawMainRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawShearRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos);
		// ġ����
		pOpt->SetEnvType(&Dom, HCAD_DIML);
		DBDeckJongDim.DimJongDanbuDetail(&Dom, TRUE, DIM_DECK_MAINREBAR_UPPER|DIM_DECK_MAINREBAR_LOWER, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawMarkBeryukRebarAtJong(&Dom, 0, 0, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawMarkMainRebarAtJong(&Dom, 0, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawMarkShearRebar(&Dom, 0, pDomOpt->m_nDeck_JongDrawPos);

		CDPoint xyStt = CDPoint(dLCutStt, dSpace);
		CDPoint xyEnd = CDPoint(dLCutStt, -pBridge->m_dThickSlabBase - dSpace);		

		CObLayer Lay;
		CObLine Line(xyStt, xyEnd, &Lay);
		Dom.TrimByLine(&Line, xyStt-CDPoint(1,0)*10000000); 
		CDRect rectErase(xyStt.x, xyStt.y+10000, xyEnd.x+10000000, xyEnd.y-10000);
		Dom.EraseRect(rectErase);
		pOpt->SetEnvType(&Dom, HCAD_CUTL);
		Dom.CutLightning(xyEnd.x, xyEnd.y, xyStt.x, xyStt.y, TRUE, 1); 
		CDRect rect = Dom.GetExtRect();
		Dom.Move(0, rect.Height()/2+Dom.Always(3), 0);

		*pDom << Dom;
	}
	else
	{
		pOpt->SetEnvType(&Dom,HCAD_STLC);
		
		double dSlabL = pDeckData->GetLengthSlabTotal(pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawBeryukRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos, 0, FALSE);
		DBDeckJong.DrawMainRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawShearRebarAtJong(&Dom, pDomOpt->m_nDeck_JongDrawPos);
		// ġ����
		pOpt->SetEnvType(&Dom, HCAD_DIML);
		DBDeckJongDim.DimJongDanbuDetail(&Dom, FALSE, DIM_DECK_MAINREBAR_UPPER|DIM_DECK_MAINREBAR_LOWER, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawMarkBeryukRebarAtJong(&Dom, 0, pBridge->m_nQtyJigan-1, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawMarkMainRebarAtJong(&Dom, pBridge->m_nQtyJigan-1, pDomOpt->m_nDeck_JongDrawPos);
		DBDeckJong.DrawMarkShearRebar(&Dom, pBridge->m_nQtyJigan-1, pDomOpt->m_nDeck_JongDrawPos);

		CDPoint xyStt = CDPoint(dSlabL-dLCutEnd, dSpace);
		CDPoint xyEnd = CDPoint(dSlabL-dLCutEnd, -pBridge->m_dThickSlabBase-dSpace);

		CObLayer Lay2;
		CObLine Line2(xyStt, xyEnd, &Lay2);
		Dom.TrimByLine(&Line2, xyStt+CDPoint(1,0)*10000000); 
		CDRect rectErase2(xyStt.x-10000000, xyStt.y+10000, xyEnd.x, xyEnd.y-10000);
		Dom.EraseRect(rectErase2);
		pOpt->SetEnvType(&Dom, HCAD_CUTL);
		Dom.CutLightning(xyEnd.x, xyEnd.y, xyStt.x, xyStt.y, TRUE, 1); 
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-dSlabL+dLCutEnd, -rect.Height()/2-Dom.Always(3), 0);

		*pDom << Dom;
	}
}


void CAPlateDrawDeckRebar::AddDeadLoadMomentDiagram(CDomyun *pDom, CDoubleArray &dArrX)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp	*pGir		= pBridge->GetGirder(0);
	CAPlateCalcStd	*pCalcStd	= pStd->m_pCalcStd;
	
	CFemModelingCalc ModelCalc(pStd->GetDataManage(), pCalcStd);
	CFemStressCalc   StressCalc(pStd->GetDataManage(), pCalcStd);
	CCalcData *pData = pStd->GetDataManage()->GetCalcData();

	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  

	CDomyun	Dom(pDom);
	Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleRebarBelyck);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

	long	nG			= pBxStt->GetNumberGirder();
	double	dStaStt		= pBxStt->GetStation();
	double	dStaEnd		= pBxEnd->GetStation();
	double	dSta		= 0;
	double	dYScale		= 0.01;	// ���Ʈ ���̾�׷� ����������
	double	dPreValue	= 0;
	double	dPreSta		= 0;
	double	dValue		= 0;
	
	CString cs, cs2;
	CDoubleArray dArrDim;	// ġ���� ������ ���� �����̼�
	CDoubleArray dArrDimEL;	// ġ���� ����(Y��)

	//////////////////////////////////////////////////////////////////////////
	// ���ؼ� �׸�.
	Dom.SetDirection("TOP");
	pOpt->SetEnvType(&Dom,HCAD_STLC);
	Dom.MoveTo(dStaStt, 0);
	Dom.LineTo(dStaEnd, 0);
	//////////////////////////////////////////////////////////////////////////
	// ������ȣ �׸�.
	Dom.SetTextHeight(3.5);
	Dom.SetTextAlignHorz(TA_CENTER);
	long nNode = 1;
	long nJijum = 0;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	while (pBx)   
	{
		dSta = pBx->GetStation();
		cs.Format("%d", nNode++);
		Dom.LineTo(CDPoint(dSta, Dom.Always(5)), CDPoint(dSta, -Dom.Always(5)));
		Dom.TextOut(dSta, Dom.Always(10), cs);
		
		if (pBx->IsJijum())
		{
			cs = pBridge->m_strJijumName[nJijum];
	
			CSymbolDom *pSym = new CSymbolDom(&Dom, pOpt);
			pSym = new CSymbolDom(&Dom, pOpt);
			pSym->DrawSymbolJijumMark(0, 0, cs, 0, TRUE);
			pSym->SetBlockName(cs);
		
			CObInsert *pIns = new CObInsert;		
			pIns = new CObInsert;		
			pIns->m_sBlockName = cs;
			pIns->m_vPoint = CDPoint(dSta, 0);	
			pIns->m_vSFactor = CVector(1,1,1);
			Dom.AddBlockAndInsert(pSym, pIns);

			dArrDim.Add(dSta);	
			nJijum++;
		}
		pBx = Finder.GetBxNext();
	}

	//////////////////////////////////////////////////////////////////////////
	// ��������(�ռ���+�ռ���)���Ʈ��
	if(pData->m_bReadFrameModelingOutput==FALSE || pBridge->m_nQtyJigan < 2)
	{
		// �����̵�
		Dom.Move (-dStaStt, 0);

		Dom.RedrawByScale(m_Scale/pDomOpt->m_dDeck_ScaleRebarBelyck);
		Dom.RedrawByHVScale(0.15, TRUE, FALSE, FALSE);

		*pDom << Dom;

		return;
	}


	long	nIdx	= 0;
	long	nIdxTmp	= 0;
	//////////////////////////////////////////////////////////////////////////
	// Max Moment �׸�
	BOOL bMaxSearch = FALSE;
	Dom.SetLineColor(3);
	
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	double staINode = pBx->GetStation(), staJNode;
	dPreValue = 0;
	dPreSta = staINode;
	Dom.MoveTo(dPreSta, -dPreValue * dYScale);
	while (pBx = Finder.GetBxNext())  
	{
		staJNode = pBx->GetStation();
		for (long i = 1; i < 5; i++)  
		{
			dSta = staINode + (staJNode - staINode) * i / (5 - 1);
			dValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, dSta, 0);

			if(nIdx >= 80) nIdx =0;
			nIdx ++;
			nIdxTmp++;
			if (dValue > 0.0) 
			{
				if (dPreValue <= 0.0)
				{
					dPreSta = Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
					dArrDim.Add(dPreSta);	// *************************
					Dom.MoveTo(dPreSta, 0.0);
					bMaxSearch = TRUE;
				}
				if (bMaxSearch && dValue < dPreValue)  
				{  // ù ���ұ���
					if (dSta >= dStaStt && dSta <= dStaEnd) 
					{
						Dom.MoveTo(dPreSta, 0.0);
						Dom.LineTo(dPreSta, -dPreValue * dYScale);
						CDPoint P1(dPreSta, -dPreValue * dYScale * 0.5);
						cs.Format ("Max(+) = %.1f", dPreValue);
						Dom.TextOut (P1, cs);
						dArrDimEL.Add(-dPreValue * dYScale);
					}
					bMaxSearch = FALSE;
				}
				if (dSta <= dStaStt)
					Dom.MoveTo(dSta, -dValue * dYScale);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dSta, -dValue * dYScale);
			}
			else if (dPreValue > 0.0)  
			{
				dPreSta = Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dPreSta, 0.0);
			}
			dPreValue = dValue;
			dPreSta  = dSta;
		}
		staINode = staJNode;
	}

	//////////////////////////////////////////////////////////////////////////
	// Min Moment �׸�
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	staINode = pBx->GetStation(), staJNode;
	dPreValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, staINode, 0)
				+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, staINode, 0)
				+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, staINode, 0);
	nIdx		= 0;
	nIdxTmp		= 0;
	dPreSta = staINode;
	while (pBx = Finder.GetBxNext())  
	{
		staJNode = pBx->GetStation();
		for (long i = 1; i < 5; i++) 
		{
			dSta = staINode + (staJNode - staINode) * i / (5 - 1);
			dValue = StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta, 0)
					+StressCalc.GetElementForce(ELE_MOMENT3, LC_NEXT,   nG, dSta, 0);
			if(nIdx >= 80) nIdx = 0;
			nIdx++;
			nIdxTmp++;

			long nPier = pGir->GetNumberJiganByStation(dPreSta);
			if ((nPier == 0 && dValue > dPreValue) || (nPier == pBridge->m_nQtyJigan - 1 && dValue < dPreValue))  
			{
				dPreValue = dValue;
				dPreSta   = dSta;
				continue;   // �Ŵ��� ù �κа� ���κп��� �߻��ϴ� - Moment�� ó������.
			}
			if (dValue < 0.0)  
			{
				if (dPreValue >= 0.0) 
				{
					dPreSta = Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
					dArrDim.Add(dPreSta);	// *************************
					Dom.MoveTo(dPreSta, 0.0);
				}
				if (dSta <= dStaStt)
					Dom.MoveTo(dSta, -dValue * dYScale);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dSta, -dValue * dYScale);
				if (i == 5-1 && pBx->IsJijum()) 
				{
					if (dSta >= dStaStt && dSta <= dStaEnd) 
					{
						Dom.SetLineColor(6);
						Dom.MoveTo(dSta, 0.0);
						Dom.LineTo(dSta, -dValue * dYScale);
						CDPoint P1(dSta, -dValue * dYScale * 0.4);
						cs.Format("Max(-) = %.1f", dValue);
						Dom.TextOut(P1, cs);
						Dom.SetLineColor(3);
						dArrDimEL.Add(-dValue * dYScale);
					}
				}
			}
			else if (dPreValue < 0.0)  
			{
				dPreSta = Bogan(dPreValue, dValue, dPreSta, dSta, 0.0);
				if (dSta > dStaStt && dSta <= dStaEnd)
					Dom.LineTo(dPreSta, 0.0);
			}
			dPreValue = dValue;
			dPreSta   = dSta;
		}
		staINode = staJNode;
	}
	//////////////////////////////////////////////////////////////////////////
	// �������� ������������ ġ����
	pOpt->SetEnvType(&Dom,HCAD_DIML);

	dArrDim.Sort();
	dArrDimEL.Sort(FALSE);

	if(dArrDimEL.GetSize()<=0)
	{
		// �����̵�
		Dom.Move (-dStaStt, 0);

		Dom.RedrawByScale(m_Scale/pDomOpt->m_dDeck_ScaleRebarBelyck);
		Dom.RedrawByHVScale(0.15, TRUE, FALSE, FALSE);

		*pDom << Dom;

		return;
	}

	Dom.DimMoveTo(dArrDim.GetAt(0), 0, 0);
	for(long n=1; n<dArrDim.GetSize(); n++)
	{
		if(n==1)
		{
			double dL = dArrDim.GetAt(1)-dArrDim.GetAt(0);
			if(dL>500)	// ������ �θ��Ʈ �������ؼ�...
				Dom.DimLineTo(dL, 0, COMMA(dL));
		}
		else
		{
			double dL = dArrDim.GetAt(n)-dArrDim.GetAt(n-1);
			if(dL>500)	// ������ �θ��Ʈ �������ؼ�...
				Dom.DimLineTo(dL, 0, COMMA(dL));
		}
	}

	// �����̵�
	Dom.Move (-dStaStt, 0);

	Dom.RedrawByScale(m_Scale/pDomOpt->m_dDeck_ScaleRebarBelyck);
	Dom.RedrawByHVScale(0.15, TRUE, FALSE, FALSE);

	*pDom << Dom;
}



double CAPlateDrawDeckRebar::Bogan(double x1, double x2, double y1, double y2, double x)
{
	if(fabs(x1-x2) < 0.001)
		return (y1 + y2) / 2;
	if(fabs(x2-x) < 0.001)
		return y2;
	return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
}


//��ȣ��ö�� �� 
void CAPlateDrawDeckRebar::AddGuardDetail(CDomyun* pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	CHgBaseConcStd  baseConc;
	CHgBaseDrawStd  baseDraw;
	CRebar			*pRB		= NULL;
	
	CConcSymbol symDB(pOpt);
	CDomyun Dom(pDom);
	CHgBaseDrawStd HGDraw;

	double dSpace = pDomOpt->m_dDeck_SpaceBetweenJewon;
	double dMaxY  = Dom.Always(525);
	double dMinY  = Dom.Always(60);
	double dYPlus = 0;
	CDPoint xy(Dom.Always(70),dMaxY);
	if(pDomOpt->m_nTypeRebarTable == 1)  //����ȭŸ��
		xy.x = Dom.Always(650);

	//Sorting
	pDeckData->ShellSortRebarList(&pDeckData->m_arrRebarJewonGuard, &CRebar::CompareForList);

	pOpt->SetEnvType(&Dom, HCAD_DIML);
	long nSize  = pDeckData->m_arrRebarJewonGuard.GetSize();
	long nRow   = 0;
	long nType  = 0;
	long nDetailCol = 0;
	BOOL bJoint = FALSE;

	if(pDomOpt->m_nTypeRebarTable == 0)  //�⺻Ÿ�� ö�ٻ�
	{
		pOpt->SetEnvType(&Dom, HCAD_SUB_TITLE);
		Dom.TextOut(xy.x+Dom.Always(50),xy.y+Dom.GetTextHeight()/2,"ö �� �� ��");
		Dom.Move(pMove->GetPoint( MPE_GUARDBAR_DETAIL-1));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();	
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GUARDBAR_DETAIL-1, "ö�ٻ� ����");
		*pDom << Dom;

		for(long i=0; i<nSize; i++)
		{
			pRB		= pDeckData->m_arrRebarJewonGuard.GetAt(i);
			if(pRB->m_nTypeRebar!=nType || bJoint!=(pRB->m_nJoint>0))
			{
				nRow += 2;
				nType	= pRB->m_nTypeRebar;
				bJoint	= (pRB->m_nJoint>0) ? TRUE : FALSE;
				if((dMaxY-dSpace*nRow) < dMinY)
				{
					Dom.Move(pMove->GetPoint(MPE_GUARDBAR_DETAIL+nDetailCol*10));
					Dom.SetCalcExtRect();
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GUARDBAR_DETAIL+nDetailCol*10, "ö�ٻ� "+COMMA(1+nDetailCol));
					*pDom << Dom;
					nDetailCol++;
					xy.x += Dom.Always(120);
					xy.y  = dMaxY;
					nRow  = 3;
				}

				CDPoint xyDrawPoint = CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus);
				nRow = HGDraw.GetPlusCntRowDrawSymbolHeight(&Dom, pOpt, nRow, nType, dSpace,  i, bJoint, xyDrawPoint);

				xy.y   = dMaxY - dSpace*nRow;
				dYPlus = (nType==1) ? Dom.Always(8) : 0;
				symDB.RebarDetailBasic(&Dom,nType,CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus),bJoint);
			}
			else
				xy.y = dMaxY - dSpace*nRow;
			if(xy.y < dMinY)
			{
				Dom.Move(pMove->GetPoint(MPE_GUARDBAR_DETAIL+nDetailCol*10));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GUARDBAR_DETAIL+nDetailCol*10, "ö�ٻ� "+COMMA(1+nDetailCol));
				*pDom << Dom;

				nDetailCol++;
				xy.x += Dom.Always(120);
				xy.y  = dMaxY;
				nRow  = 0;
			}
			symDB.DrawJewonBar(&Dom,pRB,xy);
			nRow++;
		}
	}
	else //����ȭŸ��
	{
		baseConc.SetRebarIndexBySortRebarList(&pDeckData->m_arrRebarJewonGuard, TRUE);  //���̺� ��ȣ ����
		baseDraw.DrawRebarListDetailSimple(&Dom, &pDeckData->m_arrRebarJewonGuard, xy, pDomOpt->m_dWidthRebarTableRect, pDomOpt->m_dHeightRebarTableRect);
	}

	Dom.Move(pMove->GetPoint(MPE_GUARDBAR_DETAIL+nDetailCol*10));
	pStd->ChangeSeperateLayer(&Dom);
	Dom.SetCalcExtRect();
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GUARDBAR_DETAIL+nDetailCol*10, "ö�ٻ� "+COMMA(1+nDetailCol));
	*pDom << Dom;
}


//��ȣ�� ö�����ǥ 
void CAPlateDrawDeckRebar::AddGuardTable(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();

	CHgBaseConcStd HGConc;
	CHgBaseDrawStd HGDraw;

	CDomyun Dom(pDom);

	if(pDomOpt->m_bDeckRebar_SangseMatrialTable)
	{
		if(pDomOpt->m_nDeck_Slab_RebarTableOpt==0)
		{
			pPageMng->SetProgressData("ö�����ǥ �����...",80);
			pDeckData->ShellSortRebarList(&pDeckData->m_arrRebarJewonGuard, &CRebar::CompareForTable);
			AddRebarTableGuard(&Dom, &pDeckData->m_arrRebarJewonGuard);
			*pDom << Dom;
		}
		else 
		{
			long nIdxTable		= 0;
			CDPoint xyBase		= CDPoint(0, 0);

			CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarMain = new CSafetyTypedPtrArray<CObArray, CRebar*>;
			CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarEtc  = new CSafetyTypedPtrArray<CObArray, CRebar*>;
			MakeRebarListGuard(1, pArrRebarMain);
			
			if(pArrRebarMain->GetSize()>0)
			{
				pPageMng->SetProgressData("ö�����ǥ �����...",80);
				HGConc.ShellSortRebarList(pArrRebarMain,&CRebar::CompareForTable);
				xyBase	= AddRebarTableGuard(&Dom, pArrRebarMain, "��ö��", nIdxTable++, CDPoint(0,0));
				*pDom << Dom;
			}
			
			MakeRebarListGuard(2, pArrRebarEtc);
			if(pArrRebarEtc->GetSize()>0)
			{
				pPageMng->SetProgressData("ö�����ǥ �����...",80);
				HGConc.ShellSortRebarList(pArrRebarEtc,&CRebar::CompareForTable);
				xyBase	= AddRebarTableGuard(&Dom, pArrRebarEtc, "��Ÿö��", nIdxTable++, xyBase);
				*pDom << Dom;
			}
		}
	}
}

CDPoint CAPlateDrawDeckRebar::AddRebarTableGuard(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, long nIdxTable, CDPoint xyBase)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();

	CConcSymbol symDB(pOpt);

	CHgBaseConcStd HGConc;
	CHgBaseDrawStd HGDraw;
	
	long nDefTable			= MPE_TITLE+MPE_GUARDBAR_TABLE;
	CString sTitle			= _T("ö �� �� �� ǥ");
	if(sSubTitle != "") sTitle += "(" + sSubTitle + ")";
	switch(nIdxTable)
	{
	case 0: nDefTable	= MPE_GUARDBAR_TABLE; break;
	case 1: nDefTable	= MPE_GUARDBAR_TABLE1; break;
	default: nDefTable	= MPE_GUARDBAR_TABLE; break;
	}
	
	long nTableType = pDomOpt->m_nTypeRebarTable;
	long nType =  pDomOpt->m_nDeck_Slab_RebarTableOpt;

	CGridDomyunEx Dom(pDomP);
	if(nIdxTable == 0) 
		xyBase	= CDPoint(Dom.Always(650),Dom.Always(525));
	if(nIdxTable == 0 && pDomOpt->m_nTypeRebarTable == 1)  //����ȭŸ��
		xyBase.x = Dom.Always(200);

	//�⺻Ÿ���̰ų�, ����ȭ�ε� ���̺� �������� ��츸 ���
	if(nTableType == 0 || (nTableType == 1 && nType == 1))
	{
		pOpt->SetEnvType(&Dom,HCAD_SUB_TITLE);
		Dom.TextOut(xyBase.x+Dom.Always(155/2),xyBase.y+Dom.Always(10),sTitle);
		Dom.Move(pMove->GetPoint(MPE_TITLE+nDefTable));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE+nDefTable, sTitle);
		*pDomP << Dom;
	}

	double dLengthSum		= 0;
	double dLengthSumAdd	= 0;
	double dLengthTotal		= 0;	
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;
	long nSize = pArrRebar->GetSize();
	long nMaxRow = (long)pDomOpt->m_dDeck_DatasuOnRebarTable;

	double dFy = toKgPCm2(pDeckData->m_SigmaY);
	BOOL bShadeOutLine = TRUE;
	
	long nIndex = 0;
	long nTableSu = 0;
	while(1)
	{
		if(nIndex>=nSize) break;
//		nIndex = HGDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd,m_pArrRebar,dFy,bShadeOutLone,_T(""),pStd->m_pBasicConcInfo);
		if(nTableType == 0) //�Ϲ�Ÿ��
		{
			nIndex = HGDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, 
				dWeightTotal, dWeightTotalAdd, *pArrRebar,dFy, bShadeOutLine);
		}
		else //����ȭŸ��
		{
			nIndex = HGDraw.DrawRebarTableSimple(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, 
				dWeightTotal, dWeightTotalAdd, *pArrRebar,dFy, bShadeOutLine);
		}
		Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "ö�����ǥ");
		*pDomP << Dom;
		xyBase.x += Dom.Always(155 + 10);
		nTableSu++;
	}		
	return xyBase;
}

void CAPlateDrawDeckRebar::AddPageGuardRebar(CDomyun *pDom)
{
	CDomyun Dom(pDom);

	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();

	pPageMng->SetProgressData("��ȣ�� ö����� ������...",5);

	if(1)
	{
		pPageMng->SetProgressData("��ȣ�� �ܸ鵵 �����...", 30);
		AddFrontGuardRebar(&Dom);
		*pDom << Dom;
	}

	if(pDomOpt->m_bDeckRebar_SangseMatrial)
	{
		pPageMng->SetProgressData("��ȣ�� �� �����...", 60);
		AddGuardDetail(&Dom);
		*pDom << Dom;
	}

	if(pDomOpt->m_bDeckRebar_SangseMatrialTable)
	{
		pPageMng->SetProgressData("��ȣ�� ���ǥ �����...", 90);
		AddGuardTable(&Dom);
		*pDom << Dom;
	}

	*pDom << Dom;
}


//��ȣ�� ö�� �ܸ鵵�� �׸���.
void CAPlateDrawDeckRebar::AddFrontGuardRebar(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();

	CDPoint xyMove(pDom->Always(300), pDom->Always(100));
	if(pDomOpt->m_nTypeRebarTable == 1)  //����ȭŸ��
		xyMove.x = pDom->Always(70);

	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex		*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	CAPlateDrawStandard	DrawStd(pStd->GetDataManage());
	CAPlateDrawDanmyun DrawDan(pStd->GetDataManage());
	CDrawGuardFenceRebar DrawGuard(pStd->GetDataManage());

	for(long i = 0; i < pBridge->GetGuardFencesu(); i++)
	{
		CDomyun Dom(pDom);
		Dom.SetScaleDim(pDomOpt->m_dDeck_ScaleGuardRebar);
		
		CCentSeparation *pGuard = pBridge->GetGuardFence(i);
		long nHDan = pBridge->GetNumHDanByGuardFence(pGuard);
		DrawDan.DstDrawGuardFence(&Dom, pBx, nHDan);
		DrawGuard.DrawRebar(&Dom, i);	
		DrawGuard.DrawGroundLine(&Dom, i);
		DrawGuard.DimRebar(&Dom, i);	
		DrawGuard.MarkRebar(&Dom, i);	

		Dom.RedrawByScale(GetScale() / pDomOpt->m_dDeck_ScaleGuardRebar);
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();

		Dom.Move(-rect.TopLeft());	//������ �����ϴ��� �������� ������.
		Dom.Move(xyMove);
		xyMove.x += pDom->Always(200);
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_GUARDBAR_REBAR + i*4));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GUARDBAR_REBAR + i*4 + 1, "��ȣ��ö�� �ܸ鵵");
		*pDom << Dom;

		CString szName;
		szName.Format("��ȣ��ö�� �ܸ� %d ", i+1);
		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dDeck_ScaleGuardRebar, rect, szName, TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_GUARDBAR_REBAR + i*4 + 2));
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_GUARDBAR_REBAR + i*4 + 3,"��ȣ��ö�� �ܸ鵵 ����");
		*pDom << Dom;
	}

}



// ���� ���� ǥ��
void CAPlateDrawDeckRebar::DimTextMarkDirection(CDomyun *pDomP, CString str, BOOL bFront)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	COptionStd      *pDomOpt    = pStd->GetDomOptionStd();  
	CADeckData		*pDeckData  = pStd->GetDataManage()->GetDeckData();


	CSymbolDom SymDom(pDomP, pOpt);
	pOpt->SetEnvType(&SymDom, HCAD_TXTG);

	CPlateGirderApp  *pGir = pBridge->GetGirder(0);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_END_GIRDER);


	double Dis = pGir->GetWidthFlangeLower(pBxStt)/2 + SymDom.Always(4);
	double sta = bFront ? pBxStt->GetStation() : pBxEnd->GetStation();
	CDPoint vAng = bFront ? pBxStt->GetAngle() : pBxEnd->GetAngle();
	CDPoint XY = pGir->GetXyGirderDis(sta,-Dis, vAng);
	SymDom.DrawSymbolDirectionArrow(XY, str, bFront);
	SymDom.SetCalcExtRect();
	CDRect rect = SymDom.GetExtRect();
	CDPoint rAng = pGir->GetLine()->GetAngleAzimuth(sta);
	SymDom.Rotate(rAng.GetAngleRadian(), XY.x, XY.y);
	*pDomP << SymDom;
}
