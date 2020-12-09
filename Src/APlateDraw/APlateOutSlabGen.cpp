// APlateOutSlabGen.cpp: implementation of the CAPlateOutSlabGen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
#include "../PlateGangje\Gangje.h"
#include "../APlateDBDraw/DimDomyunEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const int VIEW_VERLEN = 600;
const int VIEW_ADDLEN = 500;

const BOOL START = TRUE;		// 단면 A-A나 단면 B-B 거더 시작부분
const BOOL END = FALSE;			// 단면 A-A나 단면 B-B 거더 끝부분

const BOOL SECTION = TRUE;		// 단면 A-A나 단면 B-B
const BOOL JONGDAN = FALSE;		// 종단면도

const BOOL SECTIONAA = TRUE;	// 단면 A-A
const BOOL SECTIONBB = FALSE;	// 단면 B-B

// MPE
const int MPE_SLAB_GEN			 = 10;
const int MPE_SLAB_JONGDAN		 = 20;
const int MPE_GEN_HDANDETAIL	 = 50;
const int MPE_GEN_KEYPLAN		 = 60;
const int MPE_GEN_HDAN_LEFT		 = 70;
const int MPE_GEN_HDAN_RIGHT	 = 75;
const int MPE_SECTION_AA		 = 100;
const int MPE_SECTION_BB		 = 110;
const int MPE_SECTION_CC		 = 120;
const int MPE_SECTION_DD		 = 130;
const int MPE_SECTION_EE		 = 140;
const int MPE_GUARD_WALL_LEFT	 = 150;
const int MPE_GUARD_WALL_RIGH	 = 160;
const int MPE_NOTCH				 = 170;
const int MPE_DIMENSION_TABLE	 = 180;

// SLAB
const int SLAB_CEN = 0;
const int SLAB_MIN = 1;
const int SLAB_MAX = 2;

// nType----------For Length
// START는 시점부를 말하며 END는 종점부를 말한다
const int SLAB_GIRDER_LEN_START =		0;
const int SLAB_GIRDER_LEN_END   =		1;
const int GIRDER_SHOE_LEN_START =		2;
const int GIRDER_SHOE_LEN_END		=	3;
const int SHOE_LOWERTAPER_LEN_START =	4;
const int SHOE_LOWERTAPER_LEN_END =		5;
const int TAPER_LEN_START =				6;
const int TAPER_LEN_END =				7;

// nType----------For Height
const int SLAB_HEI_START	= 0;	// 테이퍼 이후 슬래브(순수 슬래브)
const int SLAB_HEI_END		= 1;
const int BYUNDAN_HEI_START = 2;	// 테이퍼 부분 슬래브
const int BYUNDAN_HEI_END	= 3;

// nType----------For SectionMark;
const int SECTION_AA_STT = 0;
const int SECTION_BB_STT = 1;
const int SECTION_AA_END = 2;
const int SECTION_BB_END = 3;
const int SECTION_CC = 4;
const int SECTION_DD = 5;
const int SECTION_EE = 6;


CAPlateOutSlabGen::CAPlateOutSlabGen()
{
	m_Scale  = 100;
	m_nG     = 0;
	m_pBxStt = NULL;
	m_pBxEnd = NULL;
	m_dPreStation = 0;

	m_poArrStart.RemoveAll();
	m_poArrEnd.RemoveAll();

	m_dLastY = 0;

	m_nDimensionTableVCount = 0;
	m_nDimensionTableHCount = 0;
}

CAPlateOutSlabGen::~CAPlateOutSlabGen()
{
	m_poArrStart.FreeExtra();
	m_poArrEnd.FreeExtra();
}

void CAPlateOutSlabGen::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB		= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	COptionStd		*pDomOpt	= pStd->GetDomOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// 일반도이전의 도면번호
	long nSubPage = nPage - nprePage;

	//페이지에 사용될 Bx 세팅
	if(pDB->GetGirdersu() >=2)
		m_nG = pDB->GetGirdersu() - 2; 
	else
		m_nG = 0;

	m_pBxStt = GetBxSttPage(nSubPage, m_nG);
	m_pBxEnd = GetBxEndPage(nSubPage, m_nG);

	CDomyun *pDomP = pStd->GetDomyun(nPage);
	pDomP->SetScaleDim(GetScale());
	CDomyun Dom(pDomP);
	CDomyun PlanDom(pDomP);

	// 도각
	double dScale = GetScale();
	double MoveX = 0, MoveY = 0;
	CDRect rtPlan(0,0,0,0);

	pPageMng->SetProgressData("도각 출력중...",0);
	pStd->GetDogak()->Add(&Dom,nPage,m_Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,
		pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom,pBridge->m_sNameBridge,GetTitle(), _T(""),FALSE);	
	
	pStd->AddMaterial(&Dom, dScale);
	pStd->AddJubujebubuje(&Dom, dScale);

	Dom.SetCalcExtRect();
	CDRect dgkRect = Dom.GetExtRect(), DomBound;
	CDPoint xyMove = CDPoint(Dom.Always(100), dgkRect.Height()-Dom.Always(60));

	*pDomP << Dom;

	if(GetPageSu() == nSubPage)
	{
		if(pDomOpt->m_bSlab_FrontDetView)
		{
			pPageMng->SetProgressData("횡단면도 상세도 출력중...",15);
			AddFrontDetail(&Dom, nSubPage);
			*pDomP << Dom;
		}
		// 편구배가 동일한 경우..
//		BOOL bPyunGubaeSame = pDB->IsPyunGubaeSame(pDB->GetGirder(0)->GetBxOnJijum(0)
//			                                     , pDB->GetGirder(0)->GetBxOnJijum(pDB->m_nQtyJigan));  
		if(pDomOpt->m_bSlab_DimTable && pDomOpt->m_bSlab_FrontDetView/* && bPyunGubaeSame==FALSE*/)//치수변화 테이블을 사용하고, 횡단면도 상세선택하였을 경우
		{
			pPageMng->SetProgressData("변화 치수표 출력중...", 80);
			AddDimensionTable(&Dom, nSubPage);
			Dom.SetCalcExtRect();
			// 치수변화스케일로 도면 크기 조정
			Dom.RedrawByScale(GetScale()/pDomOpt->m_dSlab_DimTableScale);
			Dom.SetCalcExtRect();
			CDRect rect = Dom.GetExtRect();
			MoveX = dgkRect.right - rect.Width() - Dom.Always(20);//(dgkRect.left+dgkRect.right)/2+rect.Width()/3;
			MoveY = dgkRect.top + rect.Height() + Dom.Always(20);
			Dom.Move(MoveX, MoveY);			
			Dom.Move(pMove->GetPoint(MPE_DIMENSION_TABLE));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();
			pMove->AddMPE(&Dom, rect, MPE_DIMENSION_TABLE, "변화 치수표");
			*pDomP << Dom;

			DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dSlab_DimTableScale, rect, "변화 치수표", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_DIMENSION_TABLE+1));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DIMENSION_TABLE+1, "변화치수표 제목");

			*pDomP << Dom;
		}

		if(pDomOpt->m_bSlab_GuideWall)
		{
			pPageMng->SetProgressData("방호벽 상세 출력중...", 60);
			long nGuardWallSu = pDB->GetGuardWallSu();

			if(nGuardWallSu >= 1)///중간에 있는 중분대는 생략...
			{
				if(nGuardWallSu >=2)
				{
					AddGuardWall(&Dom, nSubPage, (nGuardWallSu-1), TRUE);
					*pDomP << Dom;
				}

				AddGuardWall(&Dom, nSubPage, 0, FALSE);
				*pDomP << Dom;
			}

		}

		if(pDomOpt->m_bSlab_Notch)
		{
			pPageMng->SetProgressData("NOTCH 상세 출력중...", 70);
			AddNotch(&Dom, nSubPage);
			*pDomP << Dom;
		}	
	}
	else
	{
		if(pDomOpt->m_bSlab_PlanView)
		{
			pPageMng->SetProgressData("평면도 출력중...",10);
			AddPlan(&PlanDom, nSubPage);
		}

		if(nSubPage == 1)
		{
			if(pDomOpt->m_bSlab_SectionAA)
			{
				pPageMng->SetProgressData("단면 A-A 출력중...",20);
				AddSectionAABB(&Dom, nSubPage, SECTIONAA, START, SECTION);
				AddSectionMark(&PlanDom, nSubPage, SECTION_AA_STT);
				*pDomP << Dom;
			}

			if(pDomOpt->m_bSlab_SectionBB)
			{
				pPageMng->SetProgressData("단면 B-B 출력중...",20);
				AddSectionAABB(&Dom, nSubPage, SECTIONBB, START, SECTION);
				AddSectionMark(&PlanDom, nSubPage, SECTION_BB_STT);
				*pDomP << Dom;
			}
		}

		if(nSubPage == GetPageSu()-1)
		{
			if(pDomOpt->m_bSlab_SectionAA)
			{
				pPageMng->SetProgressData("단면 A-A 출력중...",20);
				AddSectionAABB(&Dom, nSubPage, SECTIONAA, END, SECTION);
				AddSectionMark(&PlanDom, nSubPage, SECTION_AA_END);
				*pDomP << Dom;
			}
			if(pDB->GetGirdersu() >= 2)
			{
				if(pDomOpt->m_bSlab_SectionBB)
				{
					pPageMng->SetProgressData("단면 B-B 출력중...",20);
					AddSectionAABB(&Dom, nSubPage, SECTIONBB, END, SECTION);
					AddSectionMark(&PlanDom, nSubPage, SECTION_BB_END);
					*pDomP << Dom;
				}
			}
		}

		if(pDomOpt->m_bSlab_SectionCC)
		{
			pPageMng->SetProgressData("단면 C-C 출력중...", 40);
			AddSectionCC(&Dom, nSubPage, SECTION);
			AddSectionMark(&PlanDom, nSubPage, SECTION_CC);
			*pDomP << Dom;
		}

		// 세로보가 있을 경우만 생성됨.
	/*	if(pDomOpt->m_bSlab_SectionCC)
		{
			pPageMng->SetProgressData("단면 D-D 출력중...", 30);
			AddSectionDD(&Dom, nSubPage);
			AddSectionMark(&PlanDom, nSubPage, SECTION_DD);
			*pDomP << Dom;
		}

		if(pDomOpt->m_bSlab_SectionEE)
		{
			pPageMng->SetProgressData("단면 E-E 출력중...", 50);
			AddSectionEE(&Dom, nSubPage);
			AddSectionMark(&PlanDom, nSubPage, SECTION_EE);
			*pDomP << Dom;
		}*/
		
		if(pDomOpt->m_bSlab_KeyPlan)
		{
			pPageMng->SetProgressData("위치도 출력중...",75);
			AddKeyPlan(&Dom,nSubPage, Dom.Always(650), Dom.Always(50));

			*pDomP << Dom;
		}
		// 평면도 도면 등록....여러곳에서 그리기 때문에 ....
		PlanDom.SetCalcExtRect();
		rtPlan = PlanDom.GetExtRect();
		double MoveX = PlanDom.Always(220);
		double MoveY = (dgkRect.bottom - rtPlan.bottom) - PlanDom.Always(90);
		PlanDom.Move(MoveX, MoveY);
		PlanDom.Move(pMove->GetPoint(MPE_SLAB_GEN));
		PlanDom.SetCalcExtRect();	
		CDRect rect = PlanDom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&PlanDom, rect, MPE_SLAB_GEN,"평면도");		
		*pDomP << PlanDom;	

		DrawStd.AddTitleOnDom(&PlanDom, GetScale(), rect, "평 면 도",TRUE, TRUE, 0);
		PlanDom.Move(pMove->GetPoint(MPE_SLAB_GEN+1));
		PlanDom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&PlanDom, PlanDom.GetExtRect(),MPE_SLAB_GEN+1,"평면도 제목");
		*pDomP << PlanDom;	

		m_dLastY = rect.top;	// 평면도 도면 영역의 하단 높이

		if(pDomOpt->m_bSlab_JongView)
		{
			pPageMng->SetProgressData("종단면도 출력중...", 90);
			AddJongDanmyunDo(&Dom, nSubPage);
			*pDomP << Dom;
		}

		if(pDomOpt->m_bSlab_FrontView)
		{
			if(pDomOpt->m_bSlab_FrontViewLeft)
			{
				pPageMng->SetProgressData("좌측 횡단면도 출력중...",93);
				AddFront(&Dom, nSubPage, TRUE);
				Dom.SetCalcExtRect();
				MoveX = Dom.Always(120);
				MoveY = (dgkRect.bottom - rtPlan.bottom) - PlanDom.Always(90);
				Dom.Move(MoveX, MoveY);
				Dom.Move(pMove->GetPoint(MPE_GEN_HDAN_LEFT));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_GEN_HDAN_LEFT,"좌측 횡단면도");
				CDRect rect = Dom.GetExtRect();
				*pDomP << Dom;
				//Title
				DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "횡단면도",TRUE,TRUE,0);
				Dom.Move(pMove->GetPoint(MPE_GEN_HDAN_LEFT+1));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_GEN_HDAN_LEFT+1,"좌측 횡단면도 제목");
				*pDomP << Dom;
			}

			if(pDomOpt->m_bSlab_FrontViewRight)
			{
				pPageMng->SetProgressData("우측 횡단면도 출력중...",97);
				AddFront(&Dom, nSubPage, FALSE);
				Dom.SetCalcExtRect();
				MoveX = Dom.Always(770);
				MoveY = (dgkRect.bottom - rtPlan.bottom) - PlanDom.Always(90);
				Dom.Move(MoveX, MoveY);				
				Dom.Move(pMove->GetPoint(MPE_GEN_HDAN_RIGHT));
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_GEN_HDAN_RIGHT,"우측 횡단면도");
				CDRect rect = Dom.GetExtRect();
				*pDomP << Dom;
				//Title
				DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "횡단면도",TRUE,TRUE,0);
				Dom.Move(pMove->GetPoint(MPE_GEN_HDAN_RIGHT+1));
				Dom.SetCalcExtRect();
				pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
				pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_GEN_HDAN_RIGHT+1,"우측 횡단면도 제목");
				*pDomP << Dom;
			}
		}
	}
	pPageMng->SetProgressData("출력완료",100);
	*pDomP << Dom;
}

long CAPlateOutSlabGen::GetPageSu()
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(0);
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	long nTotal  = 0;

	switch(pOpt->m_nSlab_DivType)
	{
		case 0:
			nTotal  = pDB->m_nQtyJigan;
			m_nPage = nTotal/pOpt->m_nSlab_DivNum;
			if(nTotal%pOpt->m_nSlab_DivNum)
				m_nPage++;
			break;
		case 1:
			nTotal  = pGir->GetQtySplice();
			m_nPage = nTotal/pOpt->m_nSlab_DivNum;
			if(nTotal%pOpt->m_nSlab_DivNum)
				m_nPage++;
			break;
		case 2:
			nTotal = pGir->GetQtyCrossBeamVBracing(); 
			m_nPage = nTotal/pOpt->m_nSlab_DivNum;
			if(nTotal%pOpt->m_nSlab_DivNum)
				m_nPage++;
			break;
		case 3:
			m_nPage = 1;
			break;
	}

	return ++m_nPage;
}

CString CAPlateOutSlabGen::GetTitle()
{
	if(m_nPage == 0) return _T("");

	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	long nPageSu = m_nPage;
	CString szTitle;

	if(nPageSu>1)
		szTitle.Format("슬래브 일반도 (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("슬래브 일반도");

	return szTitle;
}

double CAPlateOutSlabGen::GetScale()
{	
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	m_Scale = pOpt->m_dSlab_ScaleSlabGen;

	return m_Scale;
}

void CAPlateOutSlabGen::AddPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COutMovePoint	*pMove	 = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();

	CAPlateDrawPyung    DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	// 슬래브 외곽선 및 캔틸레버부 헌치라인 
	DrawSlabByPage(&Dom, nSubPage);

	CPlateGirderApp *pGir = m_pBxStt->GetGirder();

	// 슬래브 테이프부 시점
	BOOL bStt = FALSE;
	BOOL bEnd = FALSE;
	if(m_pBxStt->GetStation() == pGir->GetSSS(OFF_STT_STT))
		bStt = TRUE;
	if(m_pBxEnd->GetStation() == pGir->GetSSS(OFF_END_END))
		bEnd = TRUE;

	DrawPyung.DrawTaperLinePyung(&Dom, bStt, bEnd);

	long nUpDown   = 0;
	long nGirderSu = pDB->GetGirdersu();
	for(long nG=0; nG<nGirderSu; nG++)
	{
		CPlateGirderApp	 *pGir   = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nG, FALSE);
		CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nG, FALSE);

		pOptStd->SetEnvType(&Dom,HCAD_STLC);
		if(nGirderSu%2 ==0)
		{
			if(nG<=(nGirderSu/2-1)) nUpDown=1;
			else nUpDown=0; 
		}
		if(nGirderSu%2==1)
		{
			if(nG < (nGirderSu/2))	nUpDown=1;
			else if (nG == (nGirderSu/2))
			{
				nUpDown=-1;
				double staStt = pBxStt->GetStation();
				double staEnd = pBxEnd->GetStation();
				DrawPyung.DrawCenterLine(&Dom, nG, staStt, staEnd);
			}

			else nUpDown=0;
		}
		// 1. 거더라인
		AddPlanGirderLine(&Dom, nSubPage);
		// 도로 중심선
//		if(nGirderSu%2 ==0)
		{
			DrawCenterLineAndMark(&Dom, nSubPage);
			DrawPyung.DrawTopBottomViewMark(&Dom, pBxStt, pBxEnd);
		}


		// 2. 크로스 빔
		if(nG < nGirderSu-1)
			DrawPyung.DrawPlanCrossBeam(&Dom, pBxStt, pBxEnd);

		// 3. 외측 가로보.....외측 빔을 포함한다
/*		CPlateBxFinder Finder(pGir);
		__int64 BxFlagLeft = BX_OUTSIDE_CROSS_LEFT;
		__int64 BxFlagRight = BX_OUTSIDE_CROSS_RIGHT;
		CPlateBasicIndex *pBxLeft = Finder.GetBxFirst(BxFlagLeft, pBxStt, pBxEnd);
		CPlateBasicIndex *pBxRight = Finder.GetBxFirst(BxFlagRight, pBxStt, pBxEnd);
		if(pBxLeft)
			DrawPyung.DrawOutSideCrossBeam(&Dom, pBxStt, pBxEnd, TRUE);
		if(pBxRight)
			DrawPyung.DrawOutSideCrossBeam(&Dom, pBxStt, pBxEnd, FALSE);*/

		// 5. 슬래브 테이프부 끝점
		if(pBxStt->GetStation() == pGir->GetSSS(OFF_STT_GIRDER) || pBxEnd->GetStation() == pGir->GetSSS(OFF_END_GIRDER))
		{
			DrawPyung.DrawSlabTaperLineEnd(&Dom, nG, nSubPage-1, nUpDown, NULL, NULL);
			DrawPyung.DrawSlabTaperLineEnd(&Dom, nG, nSubPage,   nUpDown, NULL, NULL);
		}
	}
	// 평면도 치수선
	AddDimPlan(&Dom, nSubPage);

	//  도면회전
	long nGir = -1;
	pGir = pDB->GetGirder(nGir);
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nGir);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nGir);

	double rotAng  = DrawPyung.GetRotateAnglePyung(pBxStt, pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	Dom.Move(-MoveXY);
	Dom.Rotate(rotAng,0,0);

	double dY = pDB->GetGirder(0)->m_dGirderCenter;
	Dom.Move(0, dY);

	*pDom << Dom;
}

void CAPlateOutSlabGen::AddDimPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COutMovePoint	*pMove	 = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	long nPlanDan = 1;

	BOOL bSttPage = (nSubPage==1) ? TRUE : FALSE;
	BOOL bEndPage = (nSubPage==GetPageSu()-1) ? TRUE : FALSE;
	// 상면치수 -------------------------------------------
	long nGir = 0;
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nGir);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nGir);

	CAPlateDrawDetailDim DetailDim(pStd, &Dom, pBxStt, pBxEnd);

	__int64 DimTypeUpper = 0x0000;
	if(pOpt->m_bSlab_UpperJiganLeftDim)
		DimTypeUpper |= DIM_BASE_SPAN;
	if(pOpt->m_bSlab_UpperSlabLeftDim)
		DimTypeUpper |= DIM_BASE_DIAP;
	
	DetailDim.SetPyungDimDan(nPlanDan,TRUE,TRUE);
	DetailDim.DimTotal(&Dom, DimTypeUpper, TRUE, TRUE, TRUE, TRUE, bSttPage, bEndPage, FALSE);

	// 하면치수 -------------------------------------------
	nGir    = pDB->GetGirdersu()-1;
	pBxStt  = GetBxSttPage(nSubPage,nGir);
	pBxEnd  = GetBxEndPage(nSubPage,nGir);

	CAPlateDrawDetailDim DetailDimLower(pStd, &Dom, pBxStt, pBxEnd);

	__int64 DimTypeLower = 0x0000;
	if(pOpt->m_bSlab_LowerJiganRightDim)
		DimTypeLower |= DIM_BASE_SPAN;
	if(pOpt->m_bSlab_LowerSlabRightDim)
		DimTypeLower |= DIM_BASE_DIAP;
	
	DetailDimLower.SetPyungDimDan(nPlanDan,FALSE,TRUE);
	DetailDimLower.DimTotal(&Dom, DimTypeLower, FALSE, FALSE, TRUE, TRUE,bSttPage, bEndPage, FALSE);
	
	CAPlateOutGeneral *pOutGen = new CAPlateOutGeneral;
	pOutGen->m_pStd = pStd;
	if(pOpt->m_bSlab_bPyungLineInfo)
		pOutGen->DimTextLineInfo(&Dom, pBxStt, pBxEnd);

	// 횡단 치수 -------------------------------------------
	pBxStt = GetBxSttPage(nSubPage, -1);
	pBxEnd = GetBxEndPage(nSubPage, -1);
	
	BOOL bDimGirderType = pOpt->m_nSlab_PlanView_DimOpt==0 ? TRUE : FALSE;
	if(pBxStt)
		DrawDimPyung.DrawDimHDan(&Dom, pBxStt, TRUE, TRUE, 1, bDimGirderType);
	if(pBxEnd)
		DrawDimPyung.DrawDimHDan(&Dom, pBxEnd, FALSE, TRUE, 1, bDimGirderType);

	// 유간 치수 -------------------------------------------
	if(bSttPage && pDB->IsTUGir())
		DrawPyung.DimTextJiganUgan(&Dom, FALSE, TRUE, FALSE);
	if(bEndPage && pDB->IsTUGir())
		DrawPyung.DimTextJiganUgan(&Dom, FALSE, FALSE, TRUE);

	// Match Line -------------------------------------------
	if(pOpt->m_bSlab_MatchLine)
	{
		long nG = 0;
		pBxStt = GetBxSttPage(nSubPage, nG);
		pBxEnd = GetBxEndPage(nSubPage, nG);
		long nMLDan	   = 5;
		long nRealPage = nSubPage;
		if(!pBxStt->IsState(BX_STT_GIRDER|BX_STT_JOINT))	// 슬래브나 거더의 시점부는 매치라인을 생성하지 않음
			DimTextPyungMatchLine(&Dom, pBxStt, nRealPage-1, nMLDan);
		if(!pBxEnd->IsState(BX_END_GIRDER|BX_END_JOINT))	// 슬래브나 거더의 종점부는 매치라인을 생성하지 않음
			DimTextPyungMatchLine(&Dom, pBxEnd, nRealPage  , nMLDan);
	}
	//  -----------------------------------------------------
	*pDom << Dom;
}

void CAPlateOutSlabGen::DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDan)
{
 	CAPlateDrawStd	 *pStd    = (CAPlateDrawStd*)m_pStd;
	COptionStd	     *pOpt    = pStd->GetDomOptionStd();
	CPlateBridgeApp	 *pDB     = pStd->m_pDB;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirSec = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);	// pBx가 가로보 위치일 경우만 정상 작동됨.
	
	// pBx 가 현장이음 위치 일 경우...
	if(pBx->IsState(BX_SPLICE))
	{
		long nSp = pBx->GetSplice()->GetSelfCount();
		pBxSec = pGirSec->GetBxBySpliceNo(nSp);
	}

	if(pBxSec==NULL) return;

	CSymbolDom SymDom(pDom, pStd->m_pOptStd);
	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_SYMB);

	CString str;
	double sta = pBx->GetStation();
	CDPoint XY(0,0);
	
	double Dis = pGir->GetMaxWidthFlange(TRUE);

/*	if(pDB->GetBrBeamsu(TRUE))
	{
		CBrBeam *pBeam = pStd->m_pDB->GetBrBeam(0,TRUE);
		Dis = fabs(pBeam->GetDistFromGirderCenter(TRUE)+pBeam->m_uM_WB);
	}
*/
	double Scale   = SymDom.GetScaleDim();
	double dExo	   = SymDom.GetDimExo();
	double vLen	   = SymDom.GetDimVLen();
	double dLevel  = SymDom.GetDimLevelDis();
	double distExo = Dis + (dExo+vLen+dLevel*(nDan-1))*Scale;

	XY = pGir->GetXyGirderDis(sta, -distExo, pBx->GetAngle());
	str.Format("%s%d","M.L", nSubPage);
	CDPoint rAng = pGir->GetLine()->GetAngleAzimuth(sta);
	SymDom.DrawSymbolHexagonStr(XY,rAng,str);

	distExo -= SymDom.Always(5);
	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_CUTL);
	XY = pGir->GetXyGirderDis(sta, -distExo, pBx->GetAngle());
	SymDom.MoveTo(XY.x, XY.y);
//	Dis = pGirSec->GetMaxWidthFlange(TRUE) + SymDom.Always(5);
//	XY  = pGirSec->GetXyGirderDis(pBxSec->GetStation(), Dis, pBxSec->GetAngle());
	Dis = pDB->GetSlabRight()->m_dGirderCenter /*+ distExo*/+ SymDom.Always(5) - pGir->m_dGirderCenter;
	XY = pGir->GetXyGirderDis(sta, Dis, pBx->GetAngle());
	SymDom.LineTo(XY.x, XY.y);

	*pDom << SymDom;
}

void CAPlateOutSlabGen::DimTextJongMatchLine(CDomyun *pDom, long nSubPage, long nG, long nDan)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);

	CSymbolDom SymDom(pDom, pStd->m_pOptStd);
	CDPoint xy(0,0), rAng(0,0);
	CString str;

	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nG);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nG);

	long nRealPage = nSubPage;
	long MLNum	   = nRealPage;

	double SttSta = pBxStt->GetStation();
	double EndSta = pBxEnd->GetStation();
	if(!pBxStt->IsState(BX_STT_GIRDER|BX_STT_JOINT) && nRealPage>1)
	{
		pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
		xy.x = SttSta;
		xy.y = pGir->GetElevationFlange(SttSta) + nDan*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		xy.y = SymDom.Always(35);
		str.Format("%s%d","M.L", MLNum-1);
		SymDom.DrawSymbolHexagonStr(xy,rAng,str);

		pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_CUTL, LT_DOT_LINE);
		SymDom.MoveTo(xy.x, xy.y-SymDom.Always(5));
		xy.y = pGir->GetElevationFlange(SttSta,0,CDPoint(0,1),FALSE) - SymDom.Always(5);
		xy.y = SymDom.Always(0);
		if(pDB->IsTUGir())
			xy.y -= pDB->m_BindConc.m_dHeight;
		SymDom.LineTo(xy.x,xy.y);
		*pDom << SymDom;
	}

	if(!pBxEnd->IsState(BX_END_GIRDER|BX_END_JOINT) && nRealPage<GetPageSu())
	{
		pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_SYMB);
		xy.x = SttSta + pGir->GetLengthDimType(SttSta,EndSta);
		xy.y = pGir->GetElevationFlange(EndSta) + nDan*pDom->GetDimLevelDis()*pDom->GetScaleDim();
		xy.y = SymDom.Always(35);
		str.Format("%s%d","M.L", MLNum);
		SymDom.DrawSymbolHexagonStr(xy,rAng,str);

		pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_CUTL, LT_DOT_LINE);
		SymDom.MoveTo(xy.x, xy.y-SymDom.Always(5));
		xy.y = pGir->GetElevationFlange(EndSta,0,CDPoint(0,1),FALSE) - SymDom.Always(5);
		xy.y = SymDom.Always(0);
		if(pDB->IsTUGir())
			xy.y -= pDB->m_BindConc.m_dHeight;
		SymDom.LineTo(xy.x,xy.y);
		*pDom << SymDom;
	}
}


void CAPlateOutSlabGen::DrawSlabByPage(CDomyun *pDomP, long nSubPage, BOOL bDrawCantileverHunch)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	CDomyun Dom(pDomP);

	long nG = 0;
	CPlateBasicIndex *pBxLeftStt  = GetBxSttPage(nSubPage,nG,FALSE);
	CPlateBasicIndex *pBxLeftEnd  = GetBxEndPage(nSubPage,nG,FALSE);
	
	nG = pDB->GetGirdersu()-1;
	CPlateBasicIndex *pBxRightStt = GetBxSttPage(nSubPage,nG,FALSE);
	CPlateBasicIndex *pBxRightEnd = GetBxEndPage(nSubPage,nG,FALSE);
	
	// 슬래브 외곽라인
	DrawPyung.DrawSlabByBx(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);
	
	// 슬래브 캔틸레버부 헌치라인 
	if(!pDB->IsTUGir())
		DrawPyung.DrawSlabCantileverHunchLine(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);

	// 시종점라인
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_CENT);
	if(nSubPage==1)
		DrawPyung.DrawJointLine(&Dom,TRUE);
	if(nSubPage==GetPageSu())
		DrawPyung.DrawJointLine(&Dom,FALSE);

	*pDomP << Dom;
}

void CAPlateOutSlabGen::AddSectionMark(CDomyun *pDom, long nSubPage, long nType)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB   = pStd->m_pDB;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp *pGir  = m_pBxStt->GetGirder();
	CPlateGirderApp	*pGirR = pGir->m_pRight;

	CSymbolDom SymDom(pDom, pStd->m_pOptStd);
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	CDPoint xy1, xy2, Ang1, Ang2, vCros, vStringer, vLine;
	double Sta1, Sta2, Dis1, Dis2, CenSta, CrosSta, StringerSta;
	CenSta = (m_pBxStt->GetStation()+m_pBxEnd->GetStation())/2;
	CrosSta = (m_pBxStt->GetStation()+CenSta)/2;
	StringerSta = (CenSta+m_pBxEnd->GetStation())/2;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, m_pBxStt);
	if(!pBx)
		return;
	CPlateCrossBeam *pC = NULL;
//	CPlateStringer *pS = NULL;
	
	double dHunchWidth = pDB->m_dWidthHunch;
	switch(nType)
	{
	case SECTION_AA_STT:
		xy1 = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB), 0, pGir->GetAAA(OFF_STT_SLAB));
		xy2 = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_HUNCHEND), 0, pGir->GetAAA(OFF_STT_HUNCHEND));

		SymDom.DrawSymbolSectionGisiArrow((xy1+xy2)/2, ~(xy1-xy2), "A", "A", (xy2-xy1).GetAngleDegree());
		break;

	case SECTION_BB_STT:
		Sta1 = pGir->GetSSS(OFF_STT_SLAB);
		Dis1 = pGir->GetDisPlanPlateWebToWeb(pBx)/2;
		Ang1 = pGir->GetAAA(OFF_STT_SLAB);
		xy1 = pGir->GetXyGirderDis(Sta1, Dis1, Ang1);

		Sta2 = pGir->GetSSS(OFF_STT_HUNCHEND);
		Dis2 = pGir->GetDisPlanPlateWebToWeb(pBx)/2;
		Ang2 = pGir->GetAAA(OFF_STT_HUNCHEND);
		xy2 = pGir->GetXyGirderDis(Sta2, Dis2, Ang2);

		SymDom.DrawSymbolSectionGisiArrow((xy1+xy2)/2, ~(xy1-xy2), "B", "B", (xy2-xy1).GetAngleDegree());
		break;

	case SECTION_AA_END:
		xy1 = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB), 0, pGir->GetAAA(OFF_END_SLAB));
		xy2 = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_HUNCHEND), 0, pGir->GetAAA(OFF_END_HUNCHEND));

		SymDom.DrawSymbolSectionGisiArrow((xy1+xy2)/2, ~(xy1-xy2), "A", "A", (xy1-xy2).GetAngleDegree());
		break;

	case SECTION_BB_END:
		pBx = Finder.GetBxLast(BX_CROSSBEAM_VBRACING, m_pBxEnd);
		Sta1 = pGir->GetSSS(OFF_END_SLAB);
		Dis1 = pGir->GetDisPlanPlateWebToWeb(pBx)/2;
		Ang1 = pGir->GetAAA(OFF_END_SLAB);
		xy1 = pGir->GetXyGirderDis(Sta1, Dis1, Ang1);

		Sta2 = pGir->GetSSS(OFF_END_HUNCHEND);
		Dis2 = pGir->GetDisPlanPlateWebToWeb(pBx)/2;
		Ang2 = pGir->GetAAA(OFF_END_HUNCHEND);
		xy2 = pGir->GetXyGirderDis(Sta2, Dis2, Ang2);

		SymDom.DrawSymbolSectionGisiArrow((xy1+xy2)/2, ~(xy1-xy2), "B", "B", (xy1-xy2).GetAngleDegree());
		break;

	case SECTION_CC:
		{
	//	pBx = pGir->GetBxByStationDir(CrosSta, 0, BX_CROSSBEAM_VBRACING);
		if(DrawPyung.m_DArrBx.GetSize() < 2)		return;

		pBx = DrawPyung.m_DArrBx.GetAt(1);
//		pC = pBx->GetCrossBeam();
//		if(!pC)
//			return;
		double dWidthCr = 0;
		CVBracing *pVr = NULL;
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
		{
			pC = pBx->GetSection()->GetCrossBeam();
			dWidthCr = pC->m_uM_CR;
		}
		else if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
		{
			pVr = pBx->GetSection()->GetVBracing();
			dWidthCr = pBx->GetSection()->GetBeamJewon(0, CSection::B)/2;
		}

		Sta1 = pBx->GetStation();
		Dis1 = pGir->GetLengthSharpOnBx(pBx, TRUE);
		Ang1 = pBx->GetAngle();
		xy1 = pGir->GetXyGirderDis(Sta1, Dis1, Ang1);

		pBx = pBx->GetBxMatchByCrossBeam(pGirR);
		Sta2 = pBx->GetStation();
		Dis2 = -pGirR->GetLengthSharpOnBx(pBx, TRUE);
		Ang2 = pBx->GetAngle();
		xy2 = pGirR->GetXyGirderDis(Sta2, Dis2, Ang2);

		vCros = (xy1-xy2).Unit();
		xy1 = xy2 = (xy1+xy2)/2;
		
		xy1 += (vCros.Rotate90())*(dWidthCr+dHunchWidth);
		xy2 += (vCros.RotateInv90())*(dWidthCr+dHunchWidth);

		SymDom.DrawSymbolSectionGisiArrow((xy1+xy2)/2, ~(xy1-xy2), "C", "C", (xy2-xy1).GetAngleDegree());
		break;
		}

	case SECTION_DD:
		pBx = pGir->GetBxByStationDir(CenSta, 0, BX_CROSSBEAM_VBRACING);
		Dis1 = -pGir->GetLengthSharpOnBx(pBx, TRUE);
		xy1 = pGir->GetXyGirderDis(CenSta, Dis1, pBx->GetAngle());

		Dis2 = pGir->GetLengthSharpOnBx(pBx, TRUE);;
		xy2 = pGir->GetXyGirderDis(CenSta, Dis2, pBx->GetAngle());

		SymDom.DrawSymbolSectionGisiArrow((xy1+xy2)/2, ~(xy1-xy2), "D", "D", (xy2-xy1).GetAngleDegree());
		break;

	case SECTION_EE:
		pBx = pGir->GetBxByStationDir(StringerSta, 0, BX_CROSSBEAM_VBRACING);
		if(!pBx)
			return;
//		if(!pBx->IsStringer())
//			return;
//		pS = pBx->GetStringer();
		Sta1 = pBx->GetStation();
		Dis1 = pGir->GetLengthSharpOnBx(pBx, TRUE);
		Ang1 = pBx->GetAngle();
		xy1 = pGir->GetXyGirderDis(Sta1, Dis1, Ang1);

		pBx = pBx->GetBxMatchByCrossBeam(pGirR);
		Sta2 = pBx->GetStation();
		Dis2 = -pGirR->GetLengthSharpOnBx(pBx, TRUE);
		Ang2 = pBx->GetAngle();
		xy2 = pGirR->GetXyGirderDis(Sta2, Dis2, Ang2);

		vStringer = (xy1-xy2).Unit();
		xy1 = xy2 = (xy1+xy2)/2;

//		xy1 += (vStringer)*(pS->m_uM+dHunchWidth);
//		xy2 += (-vStringer)*(pS->m_uM+dHunchWidth);

		SymDom.DrawSymbolSectionGisiArrow((xy1+xy2)/2, ~(xy1-xy2), "E", "E", (xy2-xy1).GetAngleDegree());
		break;
	}

	// 도면회전
	long nGir = -1;
	pGir = pDB->GetGirder(nGir);
	CPlateBasicIndex *pBxStt;
	CPlateBasicIndex *pBxEnd;
	pBxStt = GetBxSttPage(nSubPage,nGir);
	pBxEnd = GetBxEndPage(nSubPage,nGir);

	double rotAng  = DrawPyung.GetRotateAnglePyung(pBxStt,pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	SymDom.Move(-MoveXY);
	SymDom.Rotate(rotAng,0,0);

	double dY = pDB->GetGirder(0)->m_dGirderCenter;
	SymDom.Move(0, dY);
	*pDom << SymDom;
}

// 슬래브의 시작 BX는 BX_STT_SLAB
CPlateBasicIndex* CAPlateOutSlabGen::GetBxSttPage(long nSubPage, long nG, BOOL bSttSlab)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	long nPageSuOrg = GetPageSu();

	CPlateBasicIndex *pBx = NULL;
	long nDivDom   = pOpt->m_nSlab_DivType;
	__int64 BxFlag = bSttSlab ? BX_STT_JOINT : BX_STT_GIRDER;
	
	if(nSubPage==1 || nSubPage==nPageSuOrg)	// 마지막장
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxFirst(BxFlag);

		return pBx;
	}
	
	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum((nSubPage-1)*pOpt->m_nSlab_DivNum);
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter((nSubPage-1)*pOpt->m_nSlab_DivNum);
			break;
		case 2 :
			if(nG==-1)
			{
				nG   = pDB->GetNumberGirderNearestToCenterLine();
				pGir = pDB->GetGirder(nG);
			}
			if(nG>pDB->GetGirdersu()-2)
			{
				pBx = pGir->m_pLeft->GetBxByCrossNo((nSubPage-1)*pOpt->m_nSlab_DivNum);
				pBx = pBx ? pBx->GetBxMatchByCrossBeam(pGir) : NULL;
			}
			else
				pBx = pGir->GetBxByCrossNo((nSubPage-1)*pOpt->m_nSlab_DivNum);
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxFirst(BxFlag);
			break;
	}

	if(pBx==NULL)
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxFirst(BxFlag);
	}
	
	return pBx;
}

// 슬래브의 시작 BX는 BX_END_SLAB
CPlateBasicIndex* CAPlateOutSlabGen::GetBxEndPage(long nSubPage, long nG, BOOL bEndSlab)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	long nPageSuOrg = GetPageSu();

	CPlateBasicIndex *pBx = NULL;
	long nDivDom   = pOpt->m_nSlab_DivType;
	__int64 BxFlag = bEndSlab ? BX_END_JOINT : BX_END_GIRDER;

	if(nSubPage>=nPageSuOrg-1)	// 마지막장
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxLast(BxFlag);

		return pBx;
	}

	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum(nSubPage * pOpt->m_nSlab_DivNum);	
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter(nSubPage * pOpt->m_nSlab_DivNum);
			break;
		case 2 :			
			if(nG==-1)
			{
				nG   = pDB->GetNumberGirderNearestToCenterLine();
				pGir = pDB->GetGirder(nG);
			}
			if(nG>pDB->GetGirdersu()-2)
			{
				pBx = pGir->m_pLeft->GetBxByCrossNo(nSubPage*pOpt->m_nSlab_DivNum);
				pBx = pBx ? pBx->GetBxMatchByCrossBeam(pGir) : NULL;
			}
			else
				pBx = pGir->GetBxByCrossNo(nSubPage*pOpt->m_nSlab_DivNum);
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxLast(BxFlag);
			break;
	}
	
	if(pBx==NULL)
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxLast(BxFlag);
	}
	return pBx;
}

void CAPlateOutSlabGen::AddFront(CDomyun *pDomP, long nSubPage, BOOL bLeft)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();
	COutMovePoint   *pMove= pStd->GetMovePoint(pStd->m_nCurPage);

	long nG = pDB->GetGirdersu()-1;
	CPlateGirderApp  *pGir   = pDB->GetGirder(nG);
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage,nG);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage,nG);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = bLeft ? Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt) 
		                          : Finder.GetBxLast (BX_CROSSBEAM_VBRACING, pBxEnd);

	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());	

	CDomyun Dom(pDomP);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);

	DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, FALSE);
	DrawDanmyun.DrawGuardWall     (&Dom, pBx);	
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
	DrawDanmyun.DimSlabUpper(&Dom, pBx, FALSE);
	DrawDanmyun.DimTypicalSectionBottom(&Dom, pBx, 0, bLeft);
	// 거더높이 치수선
	pGir = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBxMatchRightEnd  = pBx->GetBxMatchByCrossBeam(pGir);
	DrawDanmyun.DimHeightGirderAll(&Dom, pBxMatchRightEnd, FALSE, bLeft);

	// 원점으로 이동,회전
	if(bLeft)
		Dom.Mirror(0, TRUE);
	Dom.Rotate(CDPoint(0,0), bLeft? CDPoint(0,1) : CDPoint(0,-1));
	// 0번거더위치 중심위치를 원점으로 함(평면도와 일치시키기 위해서)
	Dom.Move(0, pDB->GetGirder(0)->m_dGirderCenter);

	*pDomP << Dom;
}

void CAPlateOutSlabGen::AddFrontDetail(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();
	COutMovePoint   *pMove= pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	double dFrontDetScale = pOpt->m_dSlab_ScaleFrontDet;
	double dCurPageScale  = GetScale();

	// 횡단면도
	long nDrawDanmyun = 3;
	if(pDB->m_nQtyJigan>1)
		nDrawDanmyun = 4;

	for(long n=0; n<nDrawDanmyun; n++)
	{
		CDomyun Dom(pDomP);
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);

		Dom.SetScaleDim(dFrontDetScale);
		
		CPlateBasicIndex *pBx  = NULL;
		if(n==0)		// 시점부
			pBx = pDB->GetGirder(0)->GetBxOnJijum(0);
		else if(n==1)	// 종점부 
			pBx = pDB->GetGirder(0)->GetBxOnJijum(pDB->m_nQtyJigan);
		else if(n==2)	// 일반부
		{
			CPlateBxFinder Finder(pDB->GetGirder(0));
			pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			pBx = Finder.GetBxNext();
		}
		else if(n==3)	// 교각부
			pBx = pDB->GetGirder(0)->GetBxOnJijum(1);

		CPlateGirderApp	 *pGir = pDB->GetGirder(pDB->GetGirdersu()-1);
		CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());	
		// 단면그림
		DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, FALSE);			
		DrawDanmyun.DrawGuardWall     (&Dom, pBx);	
		DrawDanmyun.DrawPierPlace     (&Dom, pBx, FALSE);
		
		// 횡단면도 치수선
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	

		DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
		DrawDanmyun.DimSlabUpper(&Dom, pBx, FALSE);
		DrawDanmyun.DimTypicalSectionBottom(&Dom, pBx);	
		
		// 가변치수선		
		AddFrontDetailSlabDim(&Dom, pBx);
		
		// 거더높이 치수선
		pGir = pDB->GetGirder(pDB->GetGirdersu()-1);
		CPlateBasicIndex *pBxMatchRightEnd  = pBx->GetBxMatchByCrossBeam(pGir);
		DrawDanmyun.DimHeightGirderAll(&Dom, pBxMatchRightEnd, FALSE, FALSE);

		Dom.SetCalcExtRect();
		// 상세도 스케일로 도면 크기 조정
		Dom.RedrawByScale(dCurPageScale/dFrontDetScale);

		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();	
		double MoveX = pDomP->Always(200);
		double MoveY = pDomP->Always(500);
		MoveY -= pDomP->Always(160)*n;

		Dom.Move(MoveX, MoveY);
		Dom.Move(pMove->GetPoint(MPE_GEN_HDANDETAIL+n));
		
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		rect = Dom.GetExtRect();
		if(n==0)		// 시점부
			pMove->AddMPE(&Dom,rect,MPE_GEN_HDANDETAIL+n,"시점부 상세도");
		else if(n==1)	// 종점부 
			pMove->AddMPE(&Dom,rect,MPE_GEN_HDANDETAIL+n,"종점부 상세도");
		else if(n==2)	// 일반부
			pMove->AddMPE(&Dom,rect,MPE_GEN_HDANDETAIL+n,"일반부 상세도");
		else if(n==3)	// 교각부
			pMove->AddMPE(&Dom,rect,MPE_GEN_HDANDETAIL+n,"교각부 상세도");
		*pDomP << Dom;

		Dom.SetScaleDim(dCurPageScale);
		if(n==0)		// 시점부
			DrawStd.AddTitleOnDom(&Dom, dFrontDetScale, rect, "시점부 상세도", TRUE, TRUE, 0);
		else if(n==1)	// 종점부 
			DrawStd.AddTitleOnDom(&Dom, dFrontDetScale, rect, "종점부 상세도", TRUE, TRUE, 0);
		else if(n==2)	// 일반부
			DrawStd.AddTitleOnDom(&Dom, dFrontDetScale, rect, "일반부 상세도", TRUE, TRUE, 0);
		else if(n==3)	// 교각부
			DrawStd.AddTitleOnDom(&Dom, dFrontDetScale, rect, "교각부 상세도", TRUE, TRUE, 0);
		
		Dom.Move(pMove->GetPoint(MPE_GEN_HDANDETAIL+5+n));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업

		if(n==0)		// 시점부
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GEN_HDANDETAIL+5+n, "시점부 상세도 제목");
		else if(n==1)	// 종점부 
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GEN_HDANDETAIL+5+n, "종점부 상세도 제목");
		else if(n==2)	// 일반부
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GEN_HDANDETAIL+5+n, "일반부 상세도 제목");
		else if(n==3)	// 교각부
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_GEN_HDANDETAIL+5+n, "교각부 상세도 제목");
		*pDomP << Dom;
	}
}

// 횡단면도 상세 슬래브 두께 치수선
void CAPlateOutSlabGen::AddFrontDetailSlabDim(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	CARoadOptionStd *pOptStd = pStd->m_pOptStd;
	COutMovePoint   *pMove   = pStd->GetMovePoint(pStd->m_nCurPage);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	double dPave = pDB->m_dThickPave;
	long   nGsu  = pDB->GetGirdersu() - 1;

	// Set DimCount;
	int nVCount = 0;
	int nHCount = 0;

	CDPoint poA[HDANLINESU], poB[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveUpperDom(pBx, poA);
	pDB->GetXyPaveDom(pBx, poB);

	for(long cnt=0; cnt<HDANLINESU; cnt++)
	{
		poA[cnt].y -= dPave;
		poB[cnt].y -= dPave;
	}

	double dHTaper= pDB->GetGirder(0)->GetHeightTaper(pBx->GetStation());
	double dSlabT = pDB->GetSlabThick(CPlateBridgeApp::SLAB_LEFT) + dHTaper;

	pDom->SetDirection("LEFT");
	pDom->DimMoveTo(poA[0].x, poA[0].y - dSlabT, 0);
	pDom->DimLineTo(dSlabT, 0, COMMA(dSlabT));

	dSlabT = pDB->GetSlabThick(CPlateBridgeApp::SLAB_RIGHT)+ dHTaper;
	
	pDom->SetDirection("RIGHT");
	pDom->DimMoveTo(poB[pDB->GetQtyHDanDom(pBx)].x, poB[pDB->GetQtyHDanDom(pBx)].y - dSlabT, 0);
	pDom->DimLineTo(dSlabT, 0, COMMA(dSlabT));
	

	for(int nG=0; nG < pDB->GetGirdersu(); nG++)
	{
		// Make Dom
		CDimDomyunEx Dim(pDom);
		pOptStd->SetEnvType(&Dim, HCAD_DIML);
		
		// Set Dom Env
		CString sDirection;
		if(nG < pDB->GetGirdersu() / 2 )
			sDirection = "LEFT";
		else 
			sDirection = "RIGHT";

		Dim.SetDirection(sDirection);

		// Set Bx
		CPlateGirderApp  *pGir = pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);

		// GetHunch Dom
		CDPoint po[41];
		pGir->GetHunchXyDom(pBxMatch, po, FALSE);

		if(pBxMatch->IsDanbuJijum())
		{
			po[4] = po[3];
			po[5] = po[6];
		}

		// Set Hunch Vertical Vector
		CDPoint v(0,1);

		CDPoint poResult;

		DrawSlabDimHeight(&Dim, pBxMatch, po[0],				++nVCount);
		if(pDB->IsTUGir())
			DrawSlabDimHeight(&Dim, pBxMatch, (po[1]+po[2])/2,	++nVCount);
		DrawSlabDimHeight(&Dim, pBxMatch, po[3],				++nVCount);
		if(nG != nGsu)
		{
			// 헌치가 있을때만 치수선 그림
			if(po[3].x != po[4].x)
			{
				if(!pBxMatch->IsDanbuJijum())
					DrawSlabDimWidth(&Dim, po[3], po[4], ++nHCount);
				DrawSlabDimHeight(&Dim, pBxMatch, po[4], 0);
			}
			
			if(po[5].x != po[6].x)
			{
				if(!pBxMatch->IsDanbuJijum())
					DrawSlabDimWidth(&Dim, po[5], po[6], ++nHCount);
				DrawSlabDimHeight(&Dim, pBxMatch, po[5], 0);
			}
		}

		*pDom << Dim;
	}

	// 치수변화 테이블에 사용될 컬럼수 설정
	m_nDimensionTableVCount = nVCount;
	m_nDimensionTableHCount = nHCount;
}

void CAPlateOutSlabGen::DrawSlabDimHeight(CDimDomyunEx* pDim, CPlateBasicIndex* pBx, CDPoint po, int nVNumber)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	
	BOOL bTaper = pBx->GetGirder()->GetHeightTaper(pBx->GetStation()) != 0 ? TRUE : FALSE;
	double dPave = pDB->m_dThickPave;

	CDimDomyunEx Dim(pDim);
	int nGiulDegree = -30;
	Dim.SetDimExo(0);
	Dim.SetDimObq(nGiulDegree);

	CString sBurf;
	sBurf.Format("V%d", nVNumber);
	long nDan = 1;
	double dSlabHeight = pDB->GetThickFrPaveByXy(pBx, po) - dPave;
	
	if(nVNumber == 0)
	{
		if(!bTaper)
		{
			Dim.DimMoveTo(po, nDan);
			Dim.DimLineTo(0, dSlabHeight, COMMA(dSlabHeight));
		}
	}
	else
	{
		Dim.DimMoveTo(po, nDan);
		Dim.DimLineTo(0, dSlabHeight, sBurf);
	}
	*pDim << Dim;
}

void CAPlateOutSlabGen::DrawSlabDimWidth(CDimDomyunEx* pDim, CDPoint poStt, CDPoint poEnd, int nHNumber)
{
	CDimDomyunEx Dim(pDim);

	Dim.SetDirection("BOTTOM");
	CString sBurf;
	sBurf.Format("H%d", nHNumber);
	long nDan = 1;
	double dSlabWidth = fabs(poEnd.x - poStt.x);

	Dim.DimMoveTo(poStt, nDan);
	if(nHNumber == 0)
		Dim.DimLineTo(dSlabWidth, 0, COMMA(dSlabWidth));
	else
		Dim.DimLineTo(dSlabWidth, 0, sBurf);

	*pDim << Dim;
}

void CAPlateOutSlabGen::AddKeyPlan(CDomyun *pDomP, long nSubPage, double dXDis, double dYDis)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint*	pMove		= pStd->GetMovePoint(pStd->GetCurPage());	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CDomyun Dom(pDomP);	
	
	double dKeyScaleHor = pStd->m_pDomOptionStd->m_dSlab_ScaleKeyPlanHor;
	double dKeyScaleVer = pStd->m_pDomOptionStd->m_dSlab_ScaleKeyPlanVer;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	// 진행방향
	CString sBridgeDir	 = pDomOpt->m_sTextBridgeDirection;
	CString sCounterDir  = pDomOpt->m_sTextCounterDirection;
	BOOL bSpliceMark	 = pDomOpt->m_bSlab_KeyPlanSpliceMark;
	BOOL bJijumMark		 = pDomOpt->m_bSlab_KeyPlanJijumMark;
	BOOL bCrossBeam		 = pDomOpt->m_bSlab_KeyPlanCrossBeam;
	BOOL bDimension		 = pDomOpt->m_bSlab_KeyPlanDimension;
	BOOL bDirection		 = pDomOpt->m_bSlab_KeyPlanDirection;
	BOOL bOneLine		 = pDomOpt->m_bSlab_KeyPlanOneLine;

	Dom.SetScaleDim(dKeyScaleHor);

	// KEY PLAN 그리기	
	DrawPyung.AddKeyPlan(&Dom,NULL,NULL,dKeyScaleHor,FALSE,bSpliceMark, bJijumMark, bDirection, FALSE, bCrossBeam,bOneLine, -1, -1, FALSE, FALSE, sBridgeDir, sCounterDir);
	// HATCH 넣기
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage,-1);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage,-1);
	if(pBxStt->IsState(BX_STT_JOINT)) pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
	if(pBxEnd->IsState(BX_END_JOINT)) pBxEnd = Finder.GetBxFirst(BX_END_SLAB);
	DrawPyung.DrawHatchInRectOnSlab(&Dom, pBxStt, pBxEnd);

	// 치수선	
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

	// 도면 이동 및 회전, 스케일조정
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER);
	CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();

	ASSERT( pBx || pBxNext);
	double rotAng = DrawPyung.GetRotateAnglePyung(pBx,pBxNext);
	CDPoint MoveXY = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	Dom.Move(-MoveXY);	// 도로중심 (0,0)
	Dom.Rotate(rotAng,0,0);	

	BOOL bRotate = pDomOpt->m_bSlab_bKeyPlanRoVer;
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
	
	// 최초 기준점 이동
	Dom.Move(dXDis, dYDis);

	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();
	// 도면 기준점 영역 이동
	Dom.Move(pMove->GetPoint(MPE_GEN_KEYPLAN));
	
	// 도면 영역 등록		
	Dom.SetCalcExtRect();		
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(),MPE_GEN_KEYPLAN,"Key Plan");	
	*pDomP << Dom;

//	pOpt->SetEnvType(&Dom,HCAD_SUB_TITLE);
//	pStd->AddTitleOnDom(&Dom, "요 약 도");

	// TITLE
	Dom.SetScaleDim(GetScale());
/*	//if (pOption->m_KeyPlanbOutLine)
	{
//		if (pOption->m_Det_bKeyPlanRoVer)
//			rect.left += Dom.Always(40);
//		else
			rect.bottom -=Dom.Always(30);
	}
*/	DrawStd.AddTitleOnDom(&Dom, dKeyScaleHor, rect, "요 약 도",TRUE, TRUE, 0, bRotate);
	Dom.Move(pMove->GetPoint(MPE_GEN_KEYPLAN+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_GEN_KEYPLAN+1,"요약도 제목");

	*pDomP << Dom;
}

// 변화 치수 테이블
void CAPlateOutSlabGen::AddDimensionTable(CDomyun *pDomP, long nSubPage)
{
	if((m_nDimensionTableVCount+m_nDimensionTableHCount) == 0) return;

	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint   *pMove   = pStd->GetMovePoint(pStd->m_nCurPage);

	CGridDomyunEx GridDom(pDomP);
	pOptStd->SetEnvType(&GridDom, HCAD_TBL1);
	double dTableScale = pOpt->m_dSlab_DimTableScale;
	GridDom.SetScaleDim(dTableScale);

	SetDimensionTableFormat   (&GridDom, nSubPage);
	DrawDimensionTableContents(&GridDom, nSubPage);

	GridDom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&GridDom, HCAD_TBL2);
	GridDom.DrawOutLine();

	*pDomP << GridDom;
}

// 변화 치수 테이블 형식 결정
void CAPlateOutSlabGen::SetDimensionTableFormat(CGridDomyunEx *pGDom, int nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	double dTableWidthFactor  = pOpt->m_dSlab_DimTableWidhtFactor;	//테이블 폭 계수
	double dTableHegihtFactor = pOpt->m_dSlab_DimTableHeightFactor;	// 테이블 높이 계수
	int nDimTableType = pOpt->m_nSlab_DimTableType;	//0 : 교대,교각 1:모든 다이아프램

	int nCenGir = pDB->GetNumberGirderNearestToCenterLine();
	CPlateGirderApp	*pGirCenter = pDB->GetGirder(nCenGir);

	int nColumnCounts = 0;
	int nRowCounts    = 1;			// 제목

	CPlateGirderApp *pGir = pDB->GetGirder(0);
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, 0);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, 0);
	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	CPlateBasicIndex *pBxMatch = NULL;

	if(nDimTableType == 0)
	{
		while(pBx)
		{
			if(pBx->IsJijum())
				nRowCounts++;
			pBx = Finder.GetBxNext();
		}

	}
	else
	{
		while(pBx)
		{
			// 단박스이면서 외측가로보가 없는 경우에 에러발생 ................
//			if(pBx->GetCrossBeam() || pBx->IsBracketRight())
//			{
				pBxMatch = pBx->GetBxMatchByCrossBeam(pGirCenter);
				if(pBxMatch)
//				if(pBxMatch && ( pBxMatch->IsCrossBeamLeft() || 
//			                     pBxMatch->IsCrossBeamRight() ||
//								 pBxMatch->IsBracketLeft() ||
//								 pBxMatch->IsBracketRight()) )// ?????
					nRowCounts++;
//			}
			pBx = Finder.GetBxNext();
		}
	}

	int nAddCol = pOpt->m_bSlab_Pyungubae ? 3 : 1;		// STATION, 좌측편경사, 우측편경사
	nColumnCounts = m_nDimensionTableVCount + m_nDimensionTableHCount + nAddCol;	
	CStringArray sTitleArr;
	AddTitleArray(sTitleArr);	

	double dColumnWidth   = pGDom->Always(20)*dTableWidthFactor;
	double dRowHeight     = pGDom->Always(8)*dTableHegihtFactor;
	double dColTitleWidth = pGDom->Always(25)*dTableWidthFactor;
//	double dPyungubaeWidth = pGDom->Always(16)*dTableWidthFactor;

	pGDom->SetColumnCount(nColumnCounts);
	pGDom->SetRowCount(nRowCounts);

	pGDom->SetColumnWidthAll(dColumnWidth);
	pGDom->SetColumnWidth(0, dColTitleWidth);
//	pGDom->SetColumnWidth(1, dPyungubaeWidth);
	pGDom->SetRowHeightAll(dRowHeight);
	// 제목 쓰기
	for(int i=0; i<nColumnCounts; i++)
		pGDom->SetTextMatrix(0, i, sTitleArr[i]);
}

// 테이블 컬럼 제목 설정
void CAPlateOutSlabGen::AddTitleArray(CStringArray& sTitleArr)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	CString sBurf;
	sTitleArr.Add("STATION");
	if(pOpt->m_bSlab_Pyungubae)
	{
		sTitleArr.Add("좌측편경사");
		sTitleArr.Add("우측편경사");
	}
	long i = 0;
	for(i=0; i<m_nDimensionTableVCount; i++)
	{
		sBurf.Format("V%d", i+1);
		sTitleArr.Add(sBurf);
	}

	for(i=0; i<m_nDimensionTableHCount; i++)
	{
		sBurf.Format("H%d", i+1);
		sTitleArr.Add(sBurf);
	}
}

// 변화 치수 테이블 내용 쓰기
void CAPlateOutSlabGen::DrawDimensionTableContents(CGridDomyunEx *pGDom, int nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();
	CGlobarOption   *pGlopt = pStd->GetDataManage()->GetGlobalOption();

	int nDimTableType = pOpt->m_nSlab_DimTableType;
	BOOL bUnitMeter   = pGlopt->GetDesignUnitTonfMeter();
	long nRoundSlope  = pGlopt->GetDesignPointSlope();

	int nCenGir = -1;
	CPlateGirderApp	 *pGirCenter = pDB->GetGirder(nCenGir);
	CPlateBasicIndex *pBxStation = NULL;

	int nRow = 1;
	int nGSu = pDB->GetGirdersu();

	CPlateGirderApp	*pGirFirst = pDB->GetGirder(0);
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBxFinder Finder(pGirFirst);
	CPlateBasicIndex *pBxStt   = GetBxSttPage(nSubPage, 0);
	CPlateBasicIndex *pBxEnd   = GetBxEndPage(nSubPage, 0);
	CPlateBasicIndex *pBxFirst = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	CPlateBasicIndex *pBxCen   = GetBxDimensionTable(nSubPage);

	CString str = _T("");
	while(pBxFirst)
	{
		// 교대, 교각만 표시할 경우
		if(nDimTableType == 0)
		{
			while(!pBxFirst->IsJijum())
				pBxFirst = Finder.GetBxNext();
		}

		CDoubleArray dHeightArr, dWidthColArr;
		pBxStation = pBxFirst->GetBxMatchByCrossBeam(pGirCenter);
		if(!pBxStation)
		{
			pBxFirst = Finder.GetBxNext();
			continue;
		}

		double dStation = pBxStation->GetStation();
		dHeightArr.Add(dStation);
		if(pOpt->m_bSlab_Pyungubae)
		{
			dHeightArr.Add(pGirCenter->GetLine()->GetPyungubaeRight(dStation, FALSE));
			dHeightArr.Add(pGirCenter->GetLine()->GetPyungubaeRight(dStation, TRUE));
		}
		for(int nG=0; nG < nGSu; nG++)
		{
			CPlateGirderApp  *pGir = pDB->GetGirder(nG);
			CPlateBasicIndex *pBx  = pBxFirst->GetBxMatchByCrossBeam(pDB->GetGirder(nG));

			if(pBx==NULL) continue;

			// GetHunch Dom
			CDPoint po[41];
			pGir->GetHunchXyDom(pBx, po, FALSE);

			// Set Hunch Vertical Vector
			CDPoint v(0,1);

			dHeightArr.Add(GetSlabDimHeight(pBx, po[0]));
			if(pDB->IsTUGir())
				dHeightArr.Add(GetSlabDimHeight(pBx, (po[1]+po[2])/2));
			dHeightArr.Add(GetSlabDimHeight(pBx, po[3]));
			if(nG != nGSu-1)
			{
				if(po[3].x != po[4].x)	dWidthColArr.Add(GetSlabDimWidth(po[3], po[4]));
				if(po[5].x != po[6].x)	dWidthColArr.Add(GetSlabDimWidth(po[5], po[6]));
			}

		}

		int nVColSize = dHeightArr.GetSize();
		int nHColSize = dWidthColArr.GetSize();
		int nCol = nVColSize + nHColSize;
		for(int i=0; i < nCol; i++)
		{
			if(i>=pGDom->GetColumnCount()) break;
			double dColValue = 0;
			if(i < nVColSize)
				dColValue = dHeightArr.GetAt(i);
			else
				dColValue = dWidthColArr.GetAt(i-nVColSize);
			if(i==0)
			{
				pGDom->SetTextMatrixDouble(nRow, i,"%.1f", bUnitMeter ? toM(dColValue) : dColValue);
				continue;
			}
			if(pOpt->m_bSlab_Pyungubae)
			{
				if(i==1 || i==2) // 편경사
				{
					str = GetStringOutSlope(dColValue,nRoundSlope);
					pGDom->SetTextMatrix(nRow, i, str);
				}
				else
				{
					if(dColValue==-1)
						pGDom->SetTextMatrix(nRow, i, "-");
					else
						pGDom->SetTextMatrixDouble(nRow, i, "%.1f", dColValue);
				}
			}
			else
			{
				if(dColValue==-1)
					pGDom->SetTextMatrix(nRow, i, "-");
				else
					pGDom->SetTextMatrixDouble(nRow, i, "%.1f", dColValue);
			}
		}
		nRow++;
		pBxFirst = Finder.GetBxNext();
	}
}

// 슬래브 변화 테이블 높이 구하기
double CAPlateOutSlabGen::GetSlabDimHeight(CPlateBasicIndex* pBx, CDPoint po)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;

	double dPave = pDB->m_dThickPave;

	double dSlabHeight = pDB->GetThickFrPaveByXy(pBx, po) - dPave;

	return dSlabHeight;
}

// 슬래브 변화 테이블 폭 구하기
double CAPlateOutSlabGen::GetSlabDimWidth(CDPoint poStt, CDPoint poEnd)
{
	double dSlabWidth = fabs(poEnd.x - poStt.x);
	
	return dSlabWidth;
}

// 종단면도 그리기
void CAPlateOutSlabGen::AddJongDanmyunDo(CDomyun *pDom, long nPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	// 슬래브 종단 라인 그리기		
	m_pBxStt->GetGirder()->SetEleLevel(TRUE);

	DrawSlabLineUpperGirder(&Dom, m_pBxStt, m_pBxEnd);
	AddDimJong(&Dom, nPage);

	m_pBxStt->GetGirder()->SetEleLevel(FALSE);

	// 이동
	Dom.SetCalcExtRect();
	double MoveX = m_pBxStt->GetStation();
	double MoveY = Dom.GetExtRect().bottom;
	Dom.Move(-MoveX, -MoveY);

	// 추가이동(메인거더와 現거더의 차이)
	MoveX	= GetBxSttPage(nPage, -1)->GetStation() - m_pBxStt->GetStation(); // 추가이동(메인거더와 現거더의 차이)
	MoveY	= m_dLastY - Dom.Always(10);	// 평면도 영역 바로 아래위치			
	Dom.Move(MoveX, MoveY);
	
	// 평면도가 X축으로 이동한 량
	MoveX = Dom.Always(220);
	Dom.Move(MoveX, 0);

	// 영역 등록
	Dom.Move(pMove->GetPoint(MPE_SLAB_JONGDAN));
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,rect, MPE_SLAB_JONGDAN, "종단면도");

	*pDom << Dom;

	// Title
	DrawStd.AddTitleOnDom(&Dom, GetScale(),rect,"종단면도",TRUE,TRUE,0);
	Dom.Move(pMove->GetPoint(MPE_SLAB_JONGDAN+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_SLAB_JONGDAN+1,"종단면도 제목");

	*pDom << Dom;
}

//기능 : 측면도에서 거더위의 슬라브 그리기...(브라켓단면에서의 슬래브 높이 변화)
void CAPlateOutSlabGen::DrawSlabLineUpperGirder(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(pBxStt->GetStation() >= pBxEnd->GetStation()) return;

  	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd	= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB		= pStd->m_pDB;
	CPlateGirderApp *pGir		= pBxStt->GetGirder();
	CAPlateDrawJong DrawJong(pStd->m_pDataManage);

	double dPave = pDB->m_dThickPave;

	// 좌표 초기화...
	m_poArrStart.RemoveAll();
	m_poArrEnd.RemoveAll();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_CROSSBEAM_VBRACING | BX_BENDING | BX_STT_SLAB | BX_END_SLAB;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, TRUE, TRUE);

	BOOL bTaperSection = FALSE;
	if(pBxStt->IsJijum())
	{
		m_dPreStation = pBxStt->GetStation();
		bTaperSection = TRUE;
	}

	while(pBx)
	{
		// 시점부 슬래브 단면
		if(pBx->IsState(BX_STT_SLAB))	
			AddSectionAABB(pDomP, 0, SECTIONBB, START, JONGDAN, pBx, TRUE);

		// 테이퍼 구간에서는 가로보 단면상세를 그리지 않음
		if(pBx->IsState(BX_STT_BENDING_UPPER))	
			bTaperSection = TRUE;
		if(pBx->IsState(BX_END_BENDING_UPPER))
			bTaperSection = FALSE;

		// 테이퍼구간이 없을 경우
		// ...BX_CROSSBEAM위치이고, 교대부위치의 가로보가 아닐 경우 생성
		if(pDB->m_dHeightTaperStt==0 && pBx->IsState(BX_CROSSBEAM_VBRACING) &&  pBx->IsDanbuJijum()==FALSE)		
			AddSectionCC(pDomP, 0, JONGDAN, pBx);	// 가로보 단면
		// 테이퍼구간이 있을 경우
		else if(pDB->m_dHeightTaperStt>0 && pDB->m_dHeightTaperEnd>0)	
		{
			if(bTaperSection)	// 시점테이퍼부가 끝나는 지점부터 종점테이퍼부가 시작되는 지점까지
				AddSectionCC(pDomP, 0, JONGDAN, pBx);	// 가로보 단면
		}
		// 종점부 슬래브 단면
		if(pBx->IsState(BX_END_SLAB))
			AddSectionAABB(pDomP, 0, SECTIONBB, END, JONGDAN, pBx, TRUE);
		
		pBx = Finder.GetBxNext();
	}

	// 가로보와 가로보 연결하기...^^
	long nSizeArr = min(m_poArrStart.GetSize(),m_poArrEnd.GetSize());
	for(int i=0; i < nSizeArr; i++)
	{
		CDPoint xyStt = m_poArrStart.GetAt(i), xyEnd = m_poArrEnd.GetAt(i);
		pDomP->LineTo(xyStt, xyEnd);

		// 구속콘크리트
		if(pDB->IsTUGir() && i%2 != 0)
		{
			double dBindTop = pDB->m_BindConc.m_dDeep;
			double dBindBottom = pDB->m_BindConc.m_dHeight-dBindTop;

			pOptStd->SetEnvLType(pDomP, LT_DOT_LINE);
			pDomP->LineTo(xyStt.x, xyStt.y+dBindTop, xyEnd.x, xyEnd.y+dBindTop);
			pOptStd->SetEnvLType(pDomP, LT_SOLID);
			pDomP->LineTo(xyStt.x, xyStt.y-dBindBottom, xyEnd.x, xyEnd.y-dBindBottom);
		}

		// 짝수인 경우만 포장 상면 있음....
		if(i%2 == 0)
		{
			pDomP->Solid(xyStt.x, xyStt.y+dPave, xyEnd.x, xyEnd.y+dPave,
						 xyEnd.x, xyEnd.y,		 xyStt.x, xyStt.y);
		}
	}
}

// 종단면도 치수선 그리기
void CAPlateOutSlabGen::AddDimJong(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir = pDB->GetGirder(m_nG);

	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	
	pGir->SetEleLevel(TRUE);

	long nDimDan = 2;
	if(pDB->m_dHeightTaperStt==0)
		nDimDan = 3;
	BOOL bSttPage = (nSubPage==1) ? TRUE : FALSE;
	BOOL bEndPage = (nSubPage==GetPageSu()) ? TRUE : FALSE;
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, m_nG);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, m_nG);

	CAPlateDrawDetailDim DimJong(pStd, &Dom, pBxStt, pBxEnd);
	DimJong.SetJongDimDan(nDimDan,TRUE,TRUE);
	DimJong.DimTotal(&Dom,DIM_BASE_DIAP,TRUE,FALSE,FALSE,TRUE,bSttPage,bEndPage,FALSE);
	DimJong.DimTotal(&Dom,DIM_BASE_TOTAL,TRUE,FALSE,FALSE,TRUE,bSttPage,bEndPage,FALSE);

	pGir->SetEleLevel(FALSE);

	if(pOpt->m_bSlab_MatchLine)
	{
		long nG = pBxStt->GetNumberGirder();
		long nMLDan = 5;
		DimTextJongMatchLine(&Dom, nSubPage, nG, nMLDan);
	}

	*pDomP << Dom;
}

// 2002버젼에서는 평면도상의 아래쪽 거더로서 구한다...
// GetSlabLengthOnGirder에 의해 거더 중앙값과 좌측 우측(최대...최소) 값을 배열로 리턴받고...
// 여기서 배열은 define된 센터, 왼쪽, 오른쪽이다...세값이 값다면 TRUE를 리턴한다..

// 세숫자중 오름차순으로 정렬하고 세수가 같은 값인지 판단
void CAPlateOutSlabGen::SetMinMaxAndSame(double aSlabH[], BOOL &bSame)
{
	// Set Max, min Length
	double dBurf = 0;
	if(aSlabH[SLAB_MIN] > aSlabH[SLAB_MAX])
	{
		dBurf = aSlabH[SLAB_MAX];
		aSlabH[SLAB_MAX] = aSlabH[SLAB_MIN];
		aSlabH[SLAB_MIN] = dBurf;
	}

	// Set Same Len aSlabH
	if(Round(aSlabH[SLAB_MIN],0) == Round(aSlabH[SLAB_MAX],0))
		bSame = TRUE;
	else
		bSame = FALSE;
}


// nType에 의해서 거더중심의 첫번째와 두번째 스테이션과 각을 리턴한다
void CAPlateOutSlabGen::GetCenSSSAAAOnGirder(int nType, double &dFirCenSSS, double &dSecCenSSS, CDPoint &dFirAAA, CDPoint &dSecAAA)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(m_nG);
	
	SLABGIRDERSHOE nTargetFirst, nTargetSecond;
	switch(nType)
	{
	case SLAB_GIRDER_LEN_START :		// 슬래브 ~ 거더
		nTargetFirst = OFF_STT_GIRDER;
		nTargetSecond = OFF_STT_SLAB;		break;

	case SLAB_GIRDER_LEN_END :			// 거더 ~ 슬래브
		nTargetFirst = OFF_END_SLAB;
		nTargetSecond = OFF_END_GIRDER;		break;
		
	case GIRDER_SHOE_LEN_START :		// 거더 ~ 슈
		nTargetFirst = OFF_STT_SHOE;
		nTargetSecond = OFF_STT_GIRDER;		break;

	case GIRDER_SHOE_LEN_END :			// 슈 ~ 거더
		nTargetFirst = OFF_END_GIRDER;
		nTargetSecond = OFF_END_SHOE;		break;

	case SHOE_LOWERTAPER_LEN_START :		// 슈 ~ 헌치
		nTargetFirst = OFF_STT_HUNCHSTT;
		nTargetSecond = OFF_STT_SHOE;		break;

	case SHOE_LOWERTAPER_LEN_END :			// 헌치 ~ 슈
//		nTargetFirst = OFF_END_SLAB;
		nTargetFirst = OFF_END_SHOE;
		nTargetSecond = OFF_END_HUNCHSTT;		break;

	case TAPER_LEN_START :					// 헌치시작 ~ 헌치끝
		nTargetFirst = OFF_STT_HUNCHEND;
		nTargetSecond = OFF_STT_HUNCHSTT;		break;

	case TAPER_LEN_END :						// 헌치끝 ~ 헌치시작
		nTargetFirst = OFF_END_HUNCHSTT;
		nTargetSecond = OFF_END_HUNCHEND;		break;
	}

	// return value
	dFirCenSSS = pGir->GetSSS(nTargetFirst);
	dSecCenSSS = pGir->GetSSS(nTargetSecond);
	dFirAAA	   = pGir->GetAAA(nTargetFirst);
	dSecAAA    = pGir->GetAAA(nTargetSecond);
}

// nType에 의해서 각스테이션에서 거더 양단까지 최대, 최소의 떨어진 거리를 구한다
void CAPlateOutSlabGen::GetSideDis(int nType, BOOL bSectionAABB, double &dDisLeft, double &dDisRight)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(m_nG);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, m_pBxStt, m_pBxEnd);

	if(bSectionAABB)
	{
		switch(nType)
		{
		case SLAB_GIRDER_LEN_START:
		case SLAB_GIRDER_LEN_END:
		case SHOE_LOWERTAPER_LEN_START :
		case SHOE_LOWERTAPER_LEN_END :
		case TAPER_LEN_START :
		case TAPER_LEN_END :
			dDisLeft = -pGir->GetLengthSharpOnBx(pBx, TRUE) - pDB->m_dWidthHunch;
			dDisRight = pGir->GetLengthSharpOnBx(pBx, TRUE) + pDB->m_dWidthHunch;
			break;

		case GIRDER_SHOE_LEN_START :
		case GIRDER_SHOE_LEN_END :
				dDisLeft = 0;
				dDisRight = 0;
			break;
		}
	}
	else	// 현재 크로스빔까지 거리가 약간 맞지 않음....2002, 3, 13 CMS
	{
		CPlateGirderApp *pGirSec = pGir->m_pRight;

		switch(nType)
		{
		case SLAB_GIRDER_LEN_START:
		case SLAB_GIRDER_LEN_END:
		case SHOE_LOWERTAPER_LEN_START :
		case SHOE_LOWERTAPER_LEN_END :
		case TAPER_LEN_START :
		case TAPER_LEN_END :
			dDisLeft  = pGir->GetLengthSharpOnBx(pBx, TRUE) + pDB->m_dWidthHunch;
			if(pGirSec)
				dDisRight = pGir->GetDisPlanPlateWebToWeb(pBx) - pDB->m_dWidthHunch;
			else
				dDisRight = 0;
			break;

		case GIRDER_SHOE_LEN_START :
		case GIRDER_SHOE_LEN_END :
				dDisLeft  = 0;
				dDisRight = 0;
			break;
		}
	}

}

// 시종점부 슬래브 길이 구하기(거더상의)
// GetCenSSSAAAOnGirder() --> 두점의 스테이션과 각도를 구한다
// GetSideDis() --> 구하고저 하는 슬래브 점에서 스래브 폭까지(왼편, 오른편) 떨어진 거리를 구한다 
void CAPlateOutSlabGen::GetSlabLength(int nType, BOOL bSectionAABB, double aChgSlabH[], BOOL &bSameLen)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(m_nG);
	
	double dFirCenSSS, dSecCenSSS;		// 중심 스테이션
	CDPoint FirAAA, SecAAA;			// 중심 각도
	double  dFirLeftSSS, dFirRightSSS, dSecLeftSSS, dSecRightSSS; // 좌우 스테이션
	double dDisLeft, dDisRight;		// 거더중심에서 떨어진 거리

	// Get Station, Angle
	GetCenSSSAAAOnGirder(nType, dFirCenSSS, dSecCenSSS, FirAAA, SecAAA);

	// Get Dis;	
	GetSideDis(nType, bSectionAABB, dDisLeft, dDisRight);

	// Set Left-Right Station
	dFirLeftSSS  = pGir->GetStationBySc(dFirCenSSS, dDisLeft, FirAAA);
	dFirRightSSS = pGir->GetStationBySc(dFirCenSSS, dDisRight, FirAAA);
	dSecLeftSSS  = pGir->GetStationBySc(dSecCenSSS, dDisLeft, SecAAA);
	dSecRightSSS = pGir->GetStationBySc(dSecCenSSS, dDisRight, SecAAA);

	// Set Len aChgSlabH
	double dDisAverage  = bSectionAABB ? 0 : (dDisLeft + dDisRight) / 2;
	aChgSlabH[SLAB_CEN] = pGir->GetLengthDimType(dFirCenSSS, dSecCenSSS, dDisAverage);
	aChgSlabH[SLAB_MIN] = pGir->GetLengthDimType(dFirLeftSSS, dSecLeftSSS, dDisLeft);
	aChgSlabH[SLAB_MAX] = pGir->GetLengthDimType(dFirRightSSS, dSecRightSSS, dDisRight);

	// Set Max, min Length
	SetMinMaxAndSame(aChgSlabH, bSameLen);
}

// 슬래브 높이 구하기(테이퍼 부분 제외한)
void CAPlateOutSlabGen::GetSlabHeight(int nType, BOOL bSectionAABB, double aSlabH[], BOOL &bSameLen)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(m_nG);
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;
	
	switch(nType)
	{
		case SLAB_HEI_START :
			pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, m_pBxStt, m_pBxEnd);		
			break;	

		case SLAB_HEI_END :
			pBx = Finder.GetBxLast(BX_CROSSBEAM_VBRACING, m_pBxEnd, m_pBxStt);			
			break;
	}

	if(pBx==NULL) return;

	double dTaperHeight = pDB->IsTUGir() ? 0 : pDB->m_dHeightTaperStt;

	if(bSectionAABB)
	{
		aSlabH[SLAB_MIN] = pGir->GetHunchXyHeightDom(pBx, TRUE) - dTaperHeight - pDB->m_dThickPave;	//거더에서 슬래브면 까지의 높이
		aSlabH[SLAB_MAX] = pGir->GetHunchXyHeightDom(pBx, FALSE) - dTaperHeight - pDB->m_dThickPave;
		aSlabH[SLAB_CEN] = (aSlabH[SLAB_MIN] + aSlabH[SLAB_MAX]) / 2;
	}
	else			// 횡단슬래브 입력에 따라 고정된다
	{
		for(int i=0; i<3; i++)
			aSlabH[i] = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
	}

	SetMinMaxAndSame(aSlabH, bSameLen);
}

//<summary>
//거더상의 슬래브 높이 구하기(테이퍼 부분만)
//</summary>
//<param name="nType">시점부 헌치, 종점부 헌치</param>
//				const int SLAB_HEI_START	= 0;	// 테이퍼 이후 슬래브(순수 슬래브)
//				const int SLAB_HEI_END		= 1;
//				const int BYUNDAN_HEI_START = 2;	// 테이퍼 부분 슬래브
//				const int BYUNDAN_HEI_END	= 3;
//<param name="bSectionAABB"></param>
//				const BOOL SECTIONAA = TRUE;	// 단면 A-A
//				const BOOL SECTIONBB = FALSE;	// 단면 B-B
//<param name="aChgSlabH[]"></param>
//<param name="aChgSlopH[]"></param>
//<param name="&bSameLen"></param>
void CAPlateOutSlabGen::GetChgSlabHeight(int nType, BOOL bSectionAABB, double aChgSlabH[], double aChgSlopH[], BOOL &bSameLen)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(m_nG);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;
	__int64 bxFlag = BX_CROSSBEAM_VBRACING;

	double dTaperHeight;
	switch(nType)
	{
	case BYUNDAN_HEI_START :
		dTaperHeight = pDB->m_dHeightTaperStt;
		pBx = Finder.GetBxFirst(bxFlag, m_pBxStt, m_pBxEnd);
		break;	
	case BYUNDAN_HEI_END :
		dTaperHeight = pDB->m_dHeightTaperEnd;
		pBx = Finder.GetBxLast(bxFlag, m_pBxEnd, m_pBxStt);
		break;
	}
	if(pBx==NULL) return;

	if(bSectionAABB == SECTIONAA)
	{
		for(int i=0; i<3; i++)
			aChgSlabH[i] = dTaperHeight;
	}
	else
	{
		CDPoint A[41];
		CDPoint Fir, Sec, vAng(0, -1);
		pGir->GetHunchXyDom(pBx, A);

		GetXyMatchSegAndLine(A[3], A[6], A[4], vAng.Unit(), Fir);
		GetXyMatchSegAndLine(A[3], A[6], A[5], vAng.Unit(), Sec);
		aChgSlopH[SLAB_MAX] = (A[4].y - Fir.y);
		aChgSlopH[SLAB_MIN] = (A[5].y - Sec.y);
		aChgSlopH[SLAB_CEN] = (aChgSlopH[SLAB_MAX] + aChgSlopH[SLAB_MIN])/2;
		aChgSlabH[SLAB_MAX] = dTaperHeight + aChgSlopH[SLAB_MAX];// * dSlop;
		aChgSlabH[SLAB_MIN] = dTaperHeight + aChgSlopH[SLAB_MIN]; //* dSlop;
		aChgSlabH[SLAB_CEN] = (aChgSlabH[SLAB_MIN] + aChgSlabH[SLAB_MAX]) / 2;
	}

	SetMinMaxAndSame(aChgSlabH, bSameLen);
}

// 단면 A-A(B-B) 시점부
/*
        (0,0)                     TLen
         --------------------------------------------------
         |
         |
ExtSlabH |                                     ------------
         |                                 |  SlabChgH
	     |           ExtSlabL         |
	     -------------------------    SlabChgL
       ShoeSpace|
	            |
                |
*/
void CAPlateOutSlabGen::AddSectionAABB(CDomyun *pDom, long nPage, BOOL bSectionAABB, BOOL bStt, BOOL bSection, CPlateBasicIndex *pBx, BOOL bBindConc)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);
	CGlobarOption   *pGlopt  = pStd->GetDataManage()->GetGlobalOption();
	CPlateGirderApp	*pGir    = pDB->GetGirder(m_nG);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	CSymbolDom *p = new CSymbolDom(pDom, pOptStd);
	pOptStd->SetEnvType(p, HCAD_STLC);

	long nViewAddLen = bSection ? 500 : 0;
	// Scale
	double dSectionAABBScale = bSectionAABB ? pOpt->m_dSlab_ScaleSectionAA : pOpt->m_dSlab_ScaleSectionBB;
	double dCurPageScale   = GetScale();

	if(bSection)
		p->SetScaleDim(dSectionAABBScale);
	
	// EXP Joint
	double dExpjointWidth  = pGlopt->GetSttExpansionJointWidth();
	double dExpjointHeight = pGlopt->GetSttExpansionJointHeight();
	// Flange Thickness
	double UpperFlangeThickness = pGlopt->GetDesignHeigtBaseUpper()==0 ? -pGlopt->GetDesignUDFlangeMinThick(TRUE) : pGlopt->GetDesignUDFlangeMinThick(TRUE);
	double Pavement             = pDB->m_dThickPave;

	// Set Slab Length Data
	double aSlabL[3], aShoeSpace[3], aExtSlabL[3], aSlabChgL[3];
	BOOL bSameSlabL, bSameShoeSpace, bSameExtSlabL, bSameSlabChgL;

	////////////////////////////////////////////////////////////////////////////////
	// 슬래브 길이 구하기
	//
	// 슬래브 끝단 ~ 거더 시작 : SLAB_GIRDER_LEN_START
	// 거더 시작 ~ 슈(다이아프램) : GIRDER_SHOE_LEN_START
	// 슈 ~ 테이퍼 시작 : SHOE_LOWERTAPER_LEN_START
	// 테이퍼 길이 : TAPER_LEN_START
	GetSlabLength(bStt ? SLAB_GIRDER_LEN_START : SLAB_GIRDER_LEN_END, bSectionAABB, aSlabL, bSameSlabL); 
	GetSlabLength(bStt ? GIRDER_SHOE_LEN_START : GIRDER_SHOE_LEN_END, bSectionAABB, aShoeSpace, bSameShoeSpace);
	GetSlabLength(bStt ? SHOE_LOWERTAPER_LEN_START : SHOE_LOWERTAPER_LEN_END, bSectionAABB, aExtSlabL, bSameExtSlabL);
	GetSlabLength(bStt ? TAPER_LEN_START : TAPER_LEN_END, bSectionAABB, aSlabChgL, bSameSlabChgL);

	double DisGir	 = aSlabL[SLAB_CEN];
	double ShoeSpace = aShoeSpace[SLAB_CEN];
	double ExtSlabL	 = aExtSlabL[SLAB_CEN];		// 가로보중심에서 테이퍼시점까지의 거리
	// 테이퍼가 없는 경우
	// 시점과 종점의 가로보를 제원을 이용하여 이격거리를 산출함(가로보 절반길이+m_dWidthHunch)
	if(ExtSlabL==0)	
	{
		CPlateBasicIndex *pBxJijum = NULL;
		if(bStt)	pBxJijum = pDB->GetGirder(0)->GetBxOnJijum(0);
		else		pBxJijum = pDB->GetGirder(0)->GetBxOnJijum(pDB->m_nQtyJigan);

		if(pBxJijum->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			ExtSlabL = pBxJijum->GetSection()->GetCrossBeam()->m_uM_CR + pDB->m_dWidthHunch;
		else if(pBxJijum->GetSection()->IsenDetType(SECDET_VBRACING))
			ExtSlabL = pBxJijum->GetSection()->GetBeamJewon(0, CSection::B)/2 + pDB->m_dWidthHunch;
	}
	double SlabChgL  = aSlabChgL[SLAB_CEN];
	double TLen		 = DisGir + ShoeSpace + ExtSlabL + SlabChgL + nViewAddLen;

	// Set Slab Height Data
	double aSlabH[3], aChgSlabH[3], aChgSlopH[3];
	BOOL bSameSlabH, bSameChgSlabH;

	// 슬래브 높이 구하기///////////////////////////////////////////////////////////////
	// 순수 슬래브 높이
	GetSlabHeight(bStt? SLAB_HEI_START : SLAB_HEI_END, bSectionAABB, aSlabH, bSameSlabH);
	// 테이퍼 부분 슬래브 높이
	GetChgSlabHeight(bStt? BYUNDAN_HEI_START : BYUNDAN_HEI_END, bSectionAABB, aChgSlabH, aChgSlopH, bSameChgSlabH);

	double dSlop = bStt? 1 / (pDB->m_dHeightTaperStt / pDB->m_dLengthTaperToTaperStt) :
						 1 / (pDB->m_dHeightTaperEnd / pDB->m_dLengthTaperToTaperEnd) ;
	 if(bStt && pDB->m_dHeightTaperStt==0)	dSlop = 0;
	 if(bStt==FALSE && pDB->m_dHeightTaperEnd==0)	dSlop = 0;

	if(bSectionAABB == SECTIONBB)
	{
		for(int i=0; i<3; i++)
			aSlabChgL[i] += aChgSlopH[i] * 5;

		SlabChgL = aSlabChgL[SLAB_CEN];
		TLen	 = DisGir + ShoeSpace + ExtSlabL + SlabChgL + nViewAddLen;
	}

	double dBindConcStt = bStt ? pDB->m_dLengthTUBindConc_Stt : -pDB->m_dLengthTUBindConc_End;
	double dBindDis		= pDB->m_BindConc.m_dDis;
	double dBindDeep	= pDB->m_BindConc.m_dDeep;
	double dBindH		= pDB->m_BindConc.m_dHeight;
	double SlabH	= aSlabH[SLAB_CEN];
	double SlabChgH = aChgSlabH[SLAB_CEN];
	double ExtSlabH = Pavement + SlabH + SlabChgH;
	double ExtGirH	= ExtSlabH - (bStt ? pDB->m_dHeightTaperStt : pDB->m_dHeightTaperEnd) - dBindDeep + dBindH - dBindDis;
	if(!pDB->IsTUGir())
	{
		dBindDeep	= 0;
		dBindH		= 0;
		ExtGirH		= ExtSlabH;
	}

	if(!bStt) // 거더 끝 단면인 경우 모든 가로방향 길이에 대해 음수가 되야함...
	{
		nViewAddLen		= -nViewAddLen;
		dExpjointWidth	= -pGlopt->GetEndExpansionJointWidth();
		dExpjointHeight =  pGlopt->GetEndExpansionJointHeight();
		DisGir			= -DisGir;
		ShoeSpace		= -ShoeSpace;
		ExtSlabL		= -ExtSlabL;
		SlabChgL		= -SlabChgL;
		TLen			= -TLen;
	}

	/////////////////////////////////
	// Draw Slab
	p->MoveTo(0,0);
	p->LineTo(TLen, 0);
	p->MoveTo(0,0);
	p->LineToEx(0, -ExtSlabH);
	p->LineToEx(DisGir+ShoeSpace+ExtSlabL, 0);
	p->LineToEx(SlabChgL, SlabChgH);
	p->LineToEx(nViewAddLen, 0);
	
	// Draw UpperFlange
	if(bSectionAABB)
	{
		pOptStd->SetEnvType(p, pGlopt->GetDesignHeigtBaseUpper()==0 ? HCAD_STLC : HCAD_STLH);
		if(pDB->IsTUGir())
			pOptStd->SetEnvLType(p, LT_DOT_LINE);

		if(pDB->IsTUGir())
		{
			p->MoveTo(DisGir, -ExtGirH);
			p->LineToEx(ShoeSpace+ExtSlabL+SlabChgL+nViewAddLen, 0);
		}
		p->MoveTo(DisGir, -ExtGirH);
		p->LineToEx(0, UpperFlangeThickness);
		p->LineToEx(ShoeSpace+ExtSlabL, 0);
		if(pDB->IsTUGir())		p->LineToEx(SlabChgL, 0);
		else					p->LineToEx(SlabChgL, SlabChgH);
		p->LineToEx(nViewAddLen, 0);

		pOptStd->SetEnvLType(p, LT_SOLID);
		pOptStd->SetEnvType(p, HCAD_STLC);
	}
	
	// Draw Bind Concrete
	if(pDB->IsTUGir() && (bSectionAABB || bBindConc))
	{		
		pOptStd->SetEnvLType(p, LT_DOT_LINE);
		p->MoveTo(TLen, -ExtGirH-dBindDis+dBindH);
		p->LineToEx(-TLen+dBindConcStt, 0);
		pOptStd->SetEnvLType(p, LT_SOLID);
		p->LineToEx(0, -dBindH);
		p->LineToEx(TLen-dBindConcStt, 0);
	}

	// EXP Joint	
	p->RectSolid(0,0,dExpjointWidth,-dExpjointHeight, 10, 2);
	// Pavement
	p->MoveTo(dExpjointWidth, -Pavement);
	p->LineTo(TLen, -Pavement);
	// Fill Pavement
	p->Solid(dExpjointWidth, 0, TLen,0, TLen, -Pavement,dExpjointWidth, -Pavement);
	// Draw Diaphragm
	if(bSectionAABB)
	{
		p->MoveTo(DisGir, -ExtGirH+UpperFlangeThickness);
		if(!pDB->IsTUGir())
			p->LineToEx(0, -VIEW_VERLEN);
		else
		{
			pOptStd->SetEnvLType(p, LT_DOT_LINE);
			p->LineToEx(0, -dBindDis+UpperFlangeThickness);
			pOptStd->SetEnvLType(p, LT_SOLID);
			p->LineToEx(0, -VIEW_VERLEN+(dBindH-dBindDeep+UpperFlangeThickness));
		}
	}
	else if(!pDB->IsTUGir())
	{
		double dTWebCr = 12;
		double dWFlCr  = 200;	// 임시
		double dTemp = pGlopt->GetDesignHeigtBaseUpper()==0 ? -dTWebCr : 0;
		p->MoveTo(DisGir+ShoeSpace, -ExtSlabH+dTemp);
		p->LineToEx(0, -VIEW_VERLEN);
		p->MoveTo(DisGir+ShoeSpace+dTWebCr, -ExtSlabH+dTemp);
		p->LineToEx(0, -VIEW_VERLEN);
		CDPoint xyStt, xyEnd;
		xyStt.x = DisGir+ShoeSpace-dTWebCr/2-dWFlCr/2;
		xyStt.y = -ExtSlabH+dTWebCr+dTemp;
		xyEnd.x = DisGir+ShoeSpace+dTWebCr/2+dWFlCr/2;
		xyEnd.y = -ExtSlabH+dTemp;

		p->Rectangle(xyStt, xyEnd);
	}

	if(bSection)
	{
		// 우측 커팅 라인
		pOptStd->SetEnvType(p, HCAD_CUTL);
		p->CutLightning(TLen, -ExtSlabH - VIEW_VERLEN - p->Always(1), TLen, p->Always(1), TRUE, 2);
		// 하부 절단 라인
		if(!pDB->IsTUGir())
		{
			p->MoveTo(-p->Always(1),-ExtSlabH - VIEW_VERLEN + UpperFlangeThickness);
			p->LineToEx(TLen - p->Always(1),0);
		}

		// 경사 표시..
		if((bStt==TRUE&&pDB->m_dHeightTaperStt>0) || (bStt==FALSE&&pDB->m_dHeightTaperEnd>0))
		{
			pOptStd->SetEnvType(p, HCAD_DIML);
			CString str;
			str.Format("1 : %.0f",dSlop);
			double dTextAngle = bStt? ToDegree(atan(1/dSlop)) : -ToDegree(atan(1/dSlop));
			double dAngL = 0, dAngH  = 0;
			if(bStt)
			{
				dAngL = 2;
				dAngH = 1.5;
			}
			else if(bSectionAABB)
			{
				dAngL = 100;
				dAngH = 1.7;
			}
			else
			{
				dAngL = 10;
				dAngH = 1.6;
			}

			p->SetTextAngle(dTextAngle);
			p->TextOut(DisGir+ShoeSpace+ExtSlabL+SlabChgL/dAngL , -SlabH-SlabChgH/dAngH, str);
		}

		// 지시글자
		if(ExtSlabL==0)	// 테이퍼가 없는 경우
		{
			if(bStt)
				ExtSlabL += p->Always(20);
			else
				ExtSlabL -= p->Always(20);
		}
		p->DrawSymbolPropName((DisGir+ShoeSpace+ExtSlabL + SlabChgL) / 2, -ExtSlabH / 2, _T("콘크리트"),15);
		if(bSectionAABB)
			p->DrawSymbolPropName((DisGir+ShoeSpace+ExtSlabL + SlabChgL + nViewAddLen) / 2,-ExtSlabH - VIEW_VERLEN / 2 + p->Always(3), _T("WEB"),15);
	}

	// 치수선
	AddSectionAABBDim(p, nPage, bSectionAABB, bStt, bSection);

	if(bSection)
	{
		double xDis, yDis;
		long LowValue;
		CString sName;
		if(bSectionAABB)
		{
			xDis = bStt? 80 : 200;
			yDis = 100;
			LowValue = MPE_SECTION_AA;
			sName = "단면 A-A";

			if(GetPageSu() == 2)
			{
				yDis = 150;
				sName = bStt ? "시점 단면 A-A" : "종점 단면 A-A";
				LowValue = bStt? MPE_SECTION_AA : MPE_SECTION_AA+5;
			}

		}
		else
		{
			xDis = bStt? 330 : 450;
			yDis = 100;
			LowValue = MPE_SECTION_BB;
			sName = "단면 B-B";

			if(GetPageSu() == 2)
			{
				yDis = 150;
				sName = bStt ? "시점 단면 B-B" : "종점 단면 B-B";
				LowValue = bStt ? MPE_SECTION_BB : MPE_SECTION_BB + 5;
			}
		}

		p->RedrawByScale(dCurPageScale/dSectionAABBScale);
		p->SetCalcExtRect();
		CDRect rectDom = p->GetExtRect();

		p->Move(-rectDom.left, -rectDom.bottom);
		p->Move(pDom->Always(xDis), pDom->Always(yDis));
		
		p->Move(pMove->GetPoint(LowValue));
		pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업		
		p->SetCalcExtRect();
		CDRect rect = p->GetExtRect();
		pMove->AddMPE(p,rect, LowValue, sName);
		*pDom << *p;

		p->SetScaleDim(dCurPageScale);
		DrawStd.AddTitleOnDom(p, dSectionAABBScale, rect, sName, TRUE, TRUE, 0);
		p->Move(pMove->GetPoint(LowValue+1));
		p->SetCalcExtRect();
		pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업
		pMove->AddMPE(p, p->GetExtRect(), LowValue+1, sName+" 제목");

		*pDom << *p;
		delete p;
	}
	else
	{
		double xMove = pBx->GetStation();
		double cxMove = 0;
		if(bStt)
			p->Move(xMove, Pavement+SlabH);
		else
		{
			cxMove = pGir->GetLengthDimType(m_dPreStation, xMove);
			p->Move(m_dPreStation+cxMove, Pavement+SlabH);
		}

		if(bStt)
		{
			CDPoint poU(TLen+xMove,SlabH), poL(TLen+xMove, 0);
			m_poArrStart.Add(poU);
			m_poArrStart.Add(poL);
		}
		else
		{
			CDPoint poU(TLen+m_dPreStation+cxMove, SlabH), poL(TLen+m_dPreStation+cxMove, 0);
			m_poArrEnd.Add(poU);
			m_poArrEnd.Add(poL);
		}
		m_dPreStation = xMove;

		*pDom << *p;
		delete p;
	}
}

// 단면 A-A 시점부 치수선
void CAPlateOutSlabGen::AddSectionAABBDim(CDomyun *pDom, long nPage, BOOL bSectionAABB, BOOL bStt, BOOL bSection)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);
	CGlobarOption   *pGlopt  = pStd->GetDataManage()->GetGlobalOption();
	CPlateGirderApp	*pGir    = pDB->GetGirder(-1);

	long nViewAddLen = 500;
	CDimDomyunEx*	p = new CDimDomyunEx(pDom);
	pOptStd->SetEnvType(p, HCAD_DIML);

	// EXP Joint
	double dExpjointWidth  = pGlopt->GetSttExpansionJointWidth();
	double dExpjointHeight = pGlopt->GetSttExpansionJointHeight();

	// Flange Thickness
	double dPave                = pDB->m_dThickPave;

	// Slab
	double aSlabL[3], aShoeSpace[3], aExtSlabL[3], aSlabChgL[3];
	BOOL bSameSlabL, bSameShoeSpace, bSameExtSlabL, bSameSlabChgL;
	// 슬래브 길이 구하기
	GetSlabLength(bStt ? SLAB_GIRDER_LEN_START : SLAB_GIRDER_LEN_END, bSectionAABB, aSlabL, bSameSlabL); 
	GetSlabLength(bStt? GIRDER_SHOE_LEN_START : GIRDER_SHOE_LEN_END, bSectionAABB, aShoeSpace, bSameShoeSpace);
	GetSlabLength(bStt? SHOE_LOWERTAPER_LEN_START : SHOE_LOWERTAPER_LEN_END, bSectionAABB, aExtSlabL, bSameExtSlabL);
	GetSlabLength(bStt? TAPER_LEN_START : TAPER_LEN_END, bSectionAABB, aSlabChgL, bSameSlabChgL);

	double DisGir    = aSlabL[SLAB_CEN];
	double ShoeSpace = aShoeSpace[SLAB_CEN];
	double ExtSlabL  = aExtSlabL[SLAB_CEN];
	double SlabChgL  = aSlabChgL[SLAB_CEN];
	double TLen      = ShoeSpace + ExtSlabL + SlabChgL + nViewAddLen;

	// Hight
	double aSlabH[3], aChgSlabH[3], aChgSlopH[3];
	BOOL bSameSlabH, bSameChgSlabH;
	GetSlabHeight(bStt? SLAB_HEI_START : SLAB_HEI_END, bSectionAABB, aSlabH, bSameSlabH);
	GetChgSlabHeight(bStt? BYUNDAN_HEI_START : BYUNDAN_HEI_END, bSectionAABB, aChgSlabH, aChgSlopH, bSameChgSlabH);

	if(bSectionAABB == SECTIONBB)
	{
		for(int i=0; i<3; i++)
			aSlabChgL[i] += aChgSlopH[i] * 5;

		SlabChgL = aSlabChgL[SLAB_CEN];
		TLen = ShoeSpace + ExtSlabL + SlabChgL + nViewAddLen;
	}

	double SlabH	= aSlabH[SLAB_CEN];
	double SlabChgH = aChgSlabH[SLAB_CEN];
	double ExtSlabH = dPave + SlabH + SlabChgH;

	if(!bStt) // 거더 끝 단면인 경우 모든 가로방향 길이에 대해 음수가 되야함...Mirror는 지시선때문에 사용하지 X
	{
		nViewAddLen     = -nViewAddLen;
		dExpjointWidth  = pGlopt->GetEndExpansionJointWidth();
		dExpjointHeight = pGlopt->GetEndExpansionJointHeight();
		DisGir = -DisGir;
		ShoeSpace = -ShoeSpace;
		ExtSlabL = -ExtSlabL;
		SlabChgL = -SlabChgL;
		TLen = -TLen;
	}

	CString tmpDimText1, tmpDimText2;
	if(bSection && (bSectionAABB || ExtSlabL != 0))
	{
		p->SetDirection("BOTTOM");
		p->DimMoveTo(0, -ExtSlabH-VIEW_VERLEN, 1);

		if(bSectionAABB == SECTIONBB)
		{
			ShoeSpace += DisGir;
		}
		else
		{
			if(bSameSlabL)
			{
				if(pDB->IsTUGir())
				{
					double dBindStt = pDB->m_dLengthTUBindConc_Stt;
					p->DimLineTo(dBindStt, 0, _T(""),COMMA(fabs(dBindStt)));
					p->DimLineTo(DisGir-dBindStt, 0, COMMA(fabs(DisGir-dBindStt)), _T(""));
				}
				else
					p->DimLineTo(DisGir, 0, _T(""),COMMA(fabs(DisGir)));
			}
			else
			{
				tmpDimText1.Format("%s",COMMA(aSlabL[SLAB_MIN]));
				tmpDimText2.Format("~%s",COMMA(aSlabL[SLAB_MAX]));
				p->DimLineTo(DisGir, 0, tmpDimText1, tmpDimText2);
			}
		}
		if(bSectionAABB)
		{
			if(ExtSlabL)
			{
				if(!bSameShoeSpace)
				{
					tmpDimText1.Format("%s",COMMA(aShoeSpace[SLAB_MIN]));
					tmpDimText2.Format("~%s",COMMA(aShoeSpace[SLAB_MAX]));
				}
				if(bSameExtSlabL)
					p->DimLineTo(ExtSlabL+ShoeSpace, 0, COMMA(fabs(ExtSlabL+ShoeSpace)));
				else
				{
					tmpDimText1.Format("%s",COMMA(aExtSlabL[SLAB_MIN]+ShoeSpace));
					tmpDimText2.Format("~%s",COMMA(aExtSlabL[SLAB_MAX]+ShoeSpace));
					p->DimLineTo(ExtSlabL+ShoeSpace, 0, tmpDimText1, tmpDimText2);
				}
			}			
		}
		else
		{
			if(bSameShoeSpace)
			{
				p->DimLineTo(ShoeSpace, 0, COMMA(fabs(ShoeSpace)));
			}
			else
			{
				tmpDimText1.Format("%s",COMMA(aShoeSpace[SLAB_MIN]));
				tmpDimText2.Format("~%s",COMMA(aShoeSpace[SLAB_MAX]));
				p->DimLineTo(ShoeSpace, 0, tmpDimText1, tmpDimText2);
			}
			if(bSameExtSlabL)
				p->DimLineTo(ExtSlabL, 0, COMMA(fabs(ExtSlabL)));
			else
			{
				tmpDimText1.Format("%s",COMMA(aExtSlabL[SLAB_MIN]));
				tmpDimText2.Format("~%s",COMMA(aExtSlabL[SLAB_MAX]));
				p->DimLineTo(ExtSlabL, 0, tmpDimText1, tmpDimText2);
			}
		}
		
		if(bSameSlabChgL)
			p->DimLineTo(SlabChgL, 0, COMMA(fabs(SlabChgL)));		
		else
		{
			tmpDimText1.Format("%s",COMMA(aSlabChgL[SLAB_MIN]));
			tmpDimText2.Format("~%s",COMMA(aSlabChgL[SLAB_MAX]));
			p->DimLineTo(SlabChgL, 0, tmpDimText1, tmpDimText2);
		}

		//  EXP Joint
		p->SetDirection("TOP");

		p->DimMoveTo(0, 0, 1);
		p->DimLineTo(bStt?dExpjointWidth:-dExpjointWidth, 0, COMMA(dExpjointWidth));
	}

	//  슬래브 단부 좌측
	p->SetDirection(bStt? "LEFT" : "RIGHT");
	p->DimMoveTo(0, 0, 1);
	p->DimLineTo(0, -dExpjointHeight, COMMA(dExpjointHeight));

	if(bSameSlabH && bSameChgSlabH)
		p->DimLineTo(0, -ExtSlabH+dExpjointHeight, COMMA(ExtSlabH-dExpjointHeight));
	else
	{
		tmpDimText1.Format("%s",COMMA(aSlabH[SLAB_MIN] + aChgSlabH[SLAB_MIN]-dExpjointHeight+dPave));
		tmpDimText2.Format("~%s",COMMA(aSlabH[SLAB_MAX] + aChgSlabH[SLAB_MAX]-dExpjointHeight+dPave));
		p->DimLineTo(0, -ExtSlabH+dExpjointHeight, tmpDimText1, tmpDimText2);
	}

	p->DimMoveTo(0, 0, 2);
	if(bSameSlabH && bSameChgSlabH)
		p->DimLineTo(0, -ExtSlabH, COMMA(ExtSlabH));
	else
	{
		tmpDimText1.Format("%s",COMMA(aSlabH[SLAB_MIN]+aChgSlabH[SLAB_MIN]+dPave));
		tmpDimText2.Format("~%s",COMMA(aSlabH[SLAB_MAX]+aChgSlabH[SLAB_MAX]+dPave));
		p->DimLineTo(0, -ExtSlabH, tmpDimText1, tmpDimText2);
	}

	if(bSection)
	{
		//  슬래브 우측
		p->SetDirection(bStt? "RIGHT" : "LEFT");
		p->DimMoveTo(TLen,0,1);
		p->DimLineTo(0, -dPave,COMMA(dPave));			
		if(bSameSlabH)
			p->DimLineTo(0, -SlabH, COMMA(SlabH));
		else
		{
			tmpDimText1.Format("%s",COMMA(aSlabH[SLAB_MIN]));
			tmpDimText2.Format("~%s",COMMA(aSlabH[SLAB_MAX]));
			p->DimLineTo(0, -SlabH, tmpDimText1, tmpDimText2);
		}

		if(bSameChgSlabH)
			p->DimLineTo(0, -SlabChgH, COMMA(SlabChgH));
		else
		{
			tmpDimText1.Format("%s",COMMA(aChgSlabH[SLAB_MIN]));
			tmpDimText2.Format("~%s",COMMA(aChgSlabH[SLAB_MAX]));
			p->DimLineTo(0, -SlabChgH, tmpDimText1, tmpDimText2);
		}

		// 지시선	
		p->SetDirection("TOP");
		tmpDimText1.Format("%.0lf X %.0lf",dExpjointWidth,dExpjointHeight);
		double dLenText = max(p->GetTextWidth(tmpDimText1,2),p->GetTextWidth("무수축 콘크리트",2));
		p->GiSiArrowExtend((bStt?dExpjointWidth:-dExpjointWidth)/2,-dExpjointHeight/2,p->Always(10),bStt?-dLenText:dLenText, bStt?3:1, _T("무수축 콘크리트"),tmpDimText1);	
		dLenText = p->GetTextWidth("아스팔트 포장",2);
		if(ExtSlabL==0)	// 테이퍼가 없는 경우
		{
			if(bStt)
				ExtSlabL += pDom->Always(20);
			else
				ExtSlabL -= pDom->Always(20);
		}

		p->GiSiArrowExtend(ExtSlabL,0,p->Always(10), bStt?dLenText:-dLenText, 2,_T("아스팔트 포장"),_T("T = "+COMMA(dPave)+"mm"));
	}
	
	*pDom << *p;
	delete p;
}

/*

// 단면 D-D(거더단면) 그리기
/*
           (0,0)
            ----------------------- 
			   |               |
			   |               |
			   |               |
			   |               |

*/
void CAPlateOutSlabGen::AddSectionDD(CDomyun *pDom, long nPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);
	CGlobarOption   *pGlopt  = pStd->GetDataManage()->GetGlobalOption();
	CPlateGirderApp	*pGir    = pDB->GetGirder(m_nG);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	
	CSymbolDom *p = new CSymbolDom(pDom, pOptStd);	
	pOptStd->SetEnvType(p, HCAD_STLC);
 
	long nViewAddLen = 500;
	// Scale
	double dSectionCCScale = pOpt->m_dSlab_ScaleSectionCC;
	double dCurPageScale   = GetScale();
	p->SetScaleDim(dSectionCCScale);

	// 데이터 구하기
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, m_pBxStt, m_pBxEnd);
	double nCount = Finder.GetCountByFlag(BX_CROSSBEAM_VBRACING);
	pBx = Finder.GetBxNext(long(nCount / 2));///수정

	double UpperFlangeThickness = pGlopt->GetDesignUDFlangeMinThick(TRUE);
	double WebFlangeThickness   = pGlopt->GetDesignWebFalngeMinThick();		
	double dGirderW = pGir->GetWidthFlangeUpper(pBx);
	double dGirderH = dGirderW*2;
	double dSlabW = pDB->m_dWidthHunch;
	double dThickSlabBase = pDB->m_dThickSlabBase;
	double dSlabHunchThick = pDB->m_dThickSlabHunch;
	double dPave = pDB->m_dThickPave;
	double dSlabHeiL = pGir->GetHunchXyHeightDom(pBx, TRUE)- dPave;
	double dSlabHeiR = pGir->GetHunchXyHeightDom(pBx, FALSE) - dPave;
	double dThickSlab = dThickSlabBase - dSlabHunchThick;

	CDPoint poResultL, poResultR;
	CDPoint lS(-dSlabW, dSlabHeiL-dThickSlab), rS(dGirderW+dSlabW, dSlabHeiR-dThickSlab);
	CDPoint lGL(-dSlabW-3,1), rGL(-dSlabW,0);
	GetXyMatchSegAndSeg(lS, rS, lGL, rGL, poResultL);

	CDPoint lGR(dGirderW+dSlabW, 0), rGR(dGirderW+dSlabW+3, 1);
	GetXyMatchSegAndSeg(lS, rS, lGR, rGR, poResultR);
	
	double dChgSlabLH = poResultL.y;
	double dChgSlabLW = poResultL.x-lS.x;
	double dChgSlabRH = poResultR.y;
	double dChgSlabRW = poResultR.x-dGirderW-dSlabW;
	double dSlop = (dSlabHeiR - dSlabHeiL) / (dGirderW + dSlabW*2);
	double dAllLeftW  = -dSlabW+dChgSlabLW-nViewAddLen;
	double dAllRightW = dGirderW+dSlabW+dChgSlabRW+nViewAddLen;

	// 거더 그리기
	p->Rectangle(0,UpperFlangeThickness, dGirderW, 0);
	p->MoveTo(-WebFlangeThickness/2+dGirderW/2,0);		p->LineToEx(0, -dGirderH);
	p->MoveTo( WebFlangeThickness/2+dGirderW/2,0);		p->LineToEx(0, -dGirderH);
	pOptStd->SetEnvType(p, HCAD_CUTL);
	p->CutLightning(-p->Always(1), -dGirderH, dGirderW+p->Always(1), -dGirderH, FALSE, 1.5);
	pOptStd->SetEnvType(p, HCAD_STLC);

	// 슬래브 그리기
	// 좌측
	p->MoveTo(0,0);
	p->LineToEx(-dSlabW, 0);
	p->LineToEx(+dChgSlabLW, dChgSlabLH);
	p->LineToEx(-nViewAddLen, -nViewAddLen*dSlop);
	pOptStd->SetEnvType(p, HCAD_CUTL);
	p->CutLightning(dAllLeftW, dChgSlabLH-nViewAddLen*dSlop-p->Always(1),
		dAllLeftW, dAllLeftW*dSlop+dSlabHeiL+dPave+p->Always(1));
	pOptStd->SetEnvType(p, HCAD_STLC);

	// 우측
	p->MoveTo(dGirderW, 0);
	p->LineToEx(dSlabW, 0);
	p->LineToEx(dChgSlabRW, dChgSlabRH);
	p->LineToEx(nViewAddLen, nViewAddLen*dSlop);
	pOptStd->SetEnvType(p, HCAD_CUTL);
	p->CutLightning(dAllRightW, dChgSlabRH+nViewAddLen*dSlop-p->Always(1),
		dAllRightW, dSlabHeiR+(dAllRightW-dGirderW)*dSlop+dPave+p->Always(1));
	pOptStd->SetEnvType(p, HCAD_STLC);

	double dLeftTopSlab = dSlabHeiL + (dAllLeftW*dSlop);
	double dRightTopSlab = dSlabHeiR + ((dAllRightW-dGirderW)*dSlop);
	
	p->Solid(dAllLeftW, dLeftTopSlab+dPave, dAllRightW, dRightTopSlab+dPave,
		dAllRightW, dRightTopSlab, dAllLeftW, dLeftTopSlab);

	AddSectionDDDim(p, nPage);

	p->RedrawByScale(dCurPageScale/dSectionCCScale);

	p->Move(pDom->Always(430), pDom->Always(230));
	p->Move(pMove->GetPoint(MPE_SECTION_DD));
	p->SetCalcExtRect();
	CDRect rect = p->GetExtRect();
	pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업
	pMove->AddMPE(p,rect, MPE_SECTION_DD, "단면 D-D");
	*pDom << *p;

	p->SetScaleDim(dCurPageScale);
	DrawStd.AddTitleOnDom(p, dSectionCCScale, rect, "단면 D-D", TRUE, TRUE, 0);
	p->Move(pMove->GetPoint(MPE_SECTION_DD+1));
	p->SetCalcExtRect();
	pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업
	pMove->AddMPE(p, p->GetExtRect(), MPE_SECTION_DD+1, "단면 D-D 제목");
	*pDom << *p;
	delete p;
}

// 단면 DD 치수선
void CAPlateOutSlabGen::AddSectionDDDim(CDomyun *pDom, long nPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp	*pGir    = pDB->GetGirder(m_nG);
	CGlobarOption   *pGlopt  = pStd->GetDataManage()->GetGlobalOption();

	CDimDomyunEx *p = new CDimDomyunEx(pDom);	
	pOptStd->SetEnvType(p, HCAD_DIML);	

	// 데이터 구하기
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, m_pBxStt, m_pBxEnd);
	double nCount = Finder.GetCountByFlag(BX_CROSSBEAM_VBRACING);
	pBx = Finder.GetBxNext(long(nCount / 2));
	
	double dGirderW = pGir->GetWidthFlangeUpper(pBx);
	double dSlabW = pDB->m_dWidthHunch;
	double dThickSlabBase = pDB->m_dThickSlabBase;
	double dSlabHunchThick = pDB->m_dThickSlabHunch;
	double dPave = pDB->m_dThickPave;
	double dSlabHeiL = pGir->GetHunchXyHeightDom(pBx, TRUE) - dPave;
	double dSlabHeiR = pGir->GetHunchXyHeightDom(pBx, FALSE) - dPave;
	double dThickSlab = dThickSlabBase - dSlabHunchThick;

	CDPoint poResultL, poResultR;
	CDPoint lS(-dSlabW, dSlabHeiL-dThickSlab), rS(dGirderW+dSlabW, dSlabHeiR-dThickSlab);
	CDPoint lGL(-dSlabW-3,1), rGL(-dSlabW,0);
	GetXyMatchSegAndSeg(lS, rS, lGL, rGL, poResultL);

	CDPoint lGR(dGirderW+dSlabW, 0), rGR(dGirderW+dSlabW+3, 1);
	GetXyMatchSegAndSeg(lS, rS, lGR, rGR, poResultR);
	
	double dChgSlabLW = poResultL.x-lS.x;

	p->SetDirection("BOTTOM");
	// 좌하단
	p->DimMoveTo(-dSlabW+dChgSlabLW, 0, 1);
	p->DimLineTo(fabs(dChgSlabLW), 0, COMMA(fabs(dChgSlabLW)));
	p->DimLineTo(dSlabW, 0, "", COMMA(dSlabW));

	// 우하단
	p->DimMoveTo(dGirderW, 0, 1);
	p->DimLineTo(dSlabW, 0, COMMA(dSlabW));
	p->DimLineTo(fabs(dChgSlabLW), 0, "", COMMA(fabs(dChgSlabLW)));

	// 상단
	p->SetDirection("TOP");
	p->GiSiArrowExtend(dGirderW/2,(dSlabHeiL+dSlabHeiR)/2+dPave,p->Always(10),p->Always(20),2,_T("아스팔트 포장"),_T("T = "+COMMA(dPave)+"mm"));		

	// 슬래브 두께
	p->SetDimExo(0);
	p->SetDirection("RIGHT");
	p->SetDimObq(-30);
	p->DimMoveTo(-dSlabW,0,1);
	p->DimLineTo(0, dSlabHeiL, COMMA(dSlabHeiL));
	p->DimLineTo(0, dPave, COMMA(dPave));

	p->DimMoveTo(dGirderW+dSlabW, 0, 1);
	p->DimLineTo(0, dSlabHeiR, COMMA(dSlabHeiR));
	p->DimLineTo(0, dPave, COMMA(dPave));

	*pDom << *p;
	delete p;
}


// 단면C-C(가로보 슬래브)
// 단면의 개념상 문제가 있는 함수임 (2002.09.13) => 다시 작성 필요...
void CAPlateOutSlabGen::AddSectionCC(CDomyun *pDom, long nPage, BOOL bSection, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	
	if(pDB->GetGirdersu()<2 && bSection) return;
	
	CPlateGirderApp	*pGir    = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateGirderApp	*pGirSec = pDB->GetGirder(pDB->GetGirdersu()-2);
	CGlobarOption   *pGlopt  = pStd->GetDataManage()->GetGlobalOption();

	BOOL bBxHasHunch = pGir->IsCrossBeamBxHasHunch(pBx);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nViewAddLen = bSection ? 500 : 0;
	CDomyun *p = new CDomyun(pDom);	
	pOptStd->SetEnvType(p, HCAD_STLC);

	// Scale
	double dSectionCCScale = pOpt->m_dSlab_ScaleSectionCC;
	double dCurPageScale   = GetScale();
	p->SetScaleDim(dSectionCCScale);

	// 헌치있는 가로보가 BX영역에서 2개이상일 경우만 단면C타입 마크 그림.
	// 단면마크를 그릴 경우는 아래의 BX 대입
	if(bSection)
	{
		if(DrawPyung.m_DArrBx.GetSize() < 2)		return;
		pBx = DrawPyung.m_DArrBx.GetAt(1);
	}

	CPlateCrossBeam *pC  = NULL;
	CVBracing       *pVr = NULL;
	CSection        *pSec= pBx->GetSection();
	
	// BX가 CSection이 아닌 경우도 있음(BX_BENDING | BX_STT_SLAB | BX_END_SLAB)
	if(pSec==NULL)	return;

	CDPoint A[41], B[41];
	// 딘면이 가로보위치이지만 슬래브하면과 플랜지 상면이 5cm이하일 경우는 return함.
	if(pSec->IsenDetType(SECDET_CROSSBEAM))
	{
		pC  = pSec->GetCrossBeam();
		pGir= pBx->GetGirder();
		pGir->GetHunchXyDom(pBx, A);
		pC->GetCrossXyDom(B);

//		if(B[0].y+pC->m_T1_CR+50/*여유분*/ <= A[2].y)
//			return;
	}
	// 수직브레이싱인 경우는 지점부가 아닌 경우는 return함.
	else if(pSec->IsenDetType(SECDET_VBRACING))
	{
		pVr = pSec->GetVBracing();
		if(pBx->IsJijum()==FALSE)
			return;
	}
	else
		return;

	// 슬래브 변화
	CDPoint uXy2(0,1);
	CDPoint xyResultFir(0,0), xyResultSec(0,0);

	memset(A,0,sizeof(A));

	if(pC || pVr)
	{
		pBx = pBx->GetBxMatchByCrossBeam(pGirSec);
		pGirSec->GetHunchXyDom(pBx, A);
/*
		// 각 절점에서 수선으로 내린 점
		if(A[33].x > A[10].x)		// 두 선분이 만나는 점과 관련해서 가로보의 좌표에 따라
			GetXyMatchSegAndLine(A[9], A[33], A[10], uXy2, xyResultFir);
		else
			GetXyMatchSegAndLine(A[33], A[14], A[10], uXy2, xyResultFir);
*/
		if(bBxHasHunch)
			GetXyMatchSegAndLine(A[4], A[5], A[3], uXy2, xyResultFir);
		else
			GetXyMatchSegAndLine(A[3], A[6], A[3], uXy2, xyResultFir);

	}

	double dChgSlabFirH = (pC||pVr) ? xyResultFir.y-A[3].y : 0;
	double dChgSlabSecH = 0;
/*	if(pBx->IsStringer())
	{
		GetXyMatchSegAndLine(A[10], A[11], A[12], uXy2, xyResultSec);
		dChgSlabSecH = xyResultSec.y-A[12].y;
	}
	else*/
	{
		if(bBxHasHunch)
		{
			GetXyMatchSegAndLine(A[4], A[5], A[6], uXy2, xyResultSec);
			dChgSlabSecH = xyResultSec.y-A[6].y;
		}
		else
		{
			GetXyMatchSegAndLine(A[3], A[6], A[6], uXy2, xyResultSec);
			dChgSlabSecH = xyResultSec.y-A[6].y;
		}
	}

	// 슬배브 변화 구간 제원
	double dChgSlabFirW = dChgSlabFirH * 3;
	double dChgSlabSecW = dChgSlabSecH * 3;
	double dChgSlabAverW = (dChgSlabFirW + dChgSlabSecW) / 2;
	double dChgSlabAverH = (dChgSlabFirH + dChgSlabSecH) / 2;
	double dSlabW = pDB->m_dWidthHunch;

	// 가로보 단면 제원
	double dCrossH  = pC ? pC->m_H_CR / 3 : 0;
	double dCrossWT = pC ? pC->m_T3_CR : 0;
	double dCrossUT = pC ? pC->m_T1_CR : 0;
	double dCrossUW = pC ? pC->m_uM_CR : 0;
	if(pVr)
	{
		dCrossH  = 100;
		dCrossWT = 12;
		dCrossUT = 12;
		dCrossUW = 150;
	}
	
	// 슬래브및 포장 제원
	double dPave = pDB->m_dThickPave;
	double dSlabH = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
	double dSlabAllW = dCrossUW+dSlabW+dChgSlabAverW;	//전체 슬래브의 반
	double dBindBottom = pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDeep;
	
	// 가로보 단면 그리기
	if(bBxHasHunch)
	{
		if(pGlopt->GetDesignHeigtBaseUpper()==0)
		{
			p->Rectangle(-dCrossUW, 0, dCrossUW,-dCrossUT);
			p->Rectangle(-dCrossWT/2,-dCrossUT, 0, -dCrossH-dCrossUT);
			pOptStd->SetEnvType(p, HCAD_CUTL);
			p->LineTo(-dCrossUW, -dCrossH-dCrossUT, dCrossUW, -dCrossH-dCrossUT);
			if(pDB->IsTUGir())
				p->LineTo(-dCrossUW, -dCrossH-dCrossUT-dBindBottom, dCrossUW, -dCrossH-dCrossUT-dBindBottom);
		}
		else
		{
			p->Rectangle(-dCrossUW, dCrossUT, dCrossUW,0);
			p->Rectangle(-dCrossWT/2,0, dCrossWT, -dCrossH);
			pOptStd->SetEnvType(p, HCAD_CUTL);
			p->LineTo(-dCrossUW, -dCrossH, dCrossUW, -dCrossH);
			if(pDB->IsTUGir())
				p->LineTo(-dCrossUW, -dCrossH-dBindBottom, dCrossUW, -dCrossH-dBindBottom);
		}
	}
	else
	{
		pOptStd->SetEnvType(p, HCAD_STLC);
		p->LineTo(-dCrossUW, 0, dCrossUW, 0);
		if(pDB->IsTUGir())
			p->LineTo(-dCrossUW, -dBindBottom, dCrossUW, -dBindBottom);
	}

	pOptStd->SetEnvType(p, HCAD_STLC);
	// 슬래브 그리기
	p->MoveTo(-dCrossUW,0);
	p->LineToEx(-dSlabW,0);
	p->LineToEx(-dChgSlabAverW, dChgSlabAverH);
	p->LineToEx(-nViewAddLen, 0);

	p->MoveTo(dCrossUW, 0);
	p->LineToEx(dSlabW, 0);
	p->LineToEx(dChgSlabAverW, dChgSlabAverH);
	p->LineToEx(nViewAddLen, 0);

	pOptStd->SetEnvLType(p, LT_DOT_LINE);
	p->MoveTo(-dSlabAllW, dChgSlabAverH);
	p->LineTo(dSlabAllW, dChgSlabAverH);

	pOptStd->SetEnvLType(p, LT_SOLID);
	// 포장
	double dPaveTopH = dChgSlabAverH + dSlabH + dPave;
	double dPaveBotH = dChgSlabAverH + dSlabH;
	p->Solid(-dSlabAllW-nViewAddLen, dPaveTopH, dSlabAllW+nViewAddLen, dPaveTopH, 
		dSlabAllW+nViewAddLen, dPaveBotH, -dSlabAllW-nViewAddLen, dPaveBotH);

	if(bSection)
	{
		pOptStd->SetEnvType(p, HCAD_CUTL);
		// 절단면
		p->CutLightning(-dSlabAllW-nViewAddLen, dChgSlabAverH-p->Always(1),
			-dSlabAllW-nViewAddLen, dChgSlabAverH+dSlabH+dPave+p->Always(1));
		p->CutLightning(dSlabAllW+nViewAddLen, dChgSlabAverH-p->Always(1),
			dSlabAllW+nViewAddLen, dChgSlabAverH+dSlabH+dPave+p->Always(1));

		/////////////
		// 치수선
		CDimDomyunEx pDim(p);
		pOptStd->SetEnvType(&pDim, HCAD_DIML);

		// 큰수와 작은수 세팅
		GetSwapByMin(dChgSlabFirW, dChgSlabSecW);
		GetSwapByMin(dChgSlabFirH, dChgSlabSecH);

		pDim.SetDirection("BOTTOM");
		pDim.DimMoveTo(-dSlabAllW, 0, 1);
		pDim.DimLineTo(dChgSlabAverW, 0, COMMA(dChgSlabFirW), "~"+COMMA(dChgSlabSecW)+"     ");
		pDim.DimLineTo(dSlabW, 0, "", COMMA(dSlabW));
		pDim.DimLineTo(dCrossUW, 0, COMMA(dCrossUW));
		pDim.DimLineTo(dCrossUW, 0, COMMA(dCrossUW));
		pDim.DimLineTo(dSlabW, 0, "", COMMA(dSlabW));
		pDim.DimLineTo(dChgSlabAverW, 0, COMMA(dChgSlabFirW), "~"+COMMA(dChgSlabSecW));
		
		pDim.SetDirection("LEFT");
		pDim.DimMoveTo(-dSlabAllW-nViewAddLen, 0, 1);
		pDim.DimLineTo(0, dChgSlabAverH, COMMA(dChgSlabFirH)+"     ~"+COMMA(dChgSlabSecH));
		pDim.DimLineTo(0, dSlabH, "", COMMA(dSlabH));
		pDim.DimLineTo(0, dPave, COMMA(dPave));

		pDim.SetDirection("TOP");
		pDim.GiSiArrowExtend(0,dChgSlabAverH+dSlabH+dPave,pDim.Always(10),pDim.Always(20),2,_T("아스팔트 포장"),_T("T = "+COMMA(dPave)+"mm"));		
		*p << pDim;

		p->RedrawByScale(dCurPageScale/dSectionCCScale);

		p->Move(pDom->Always(650), pDom->Always(120));
		p->Move(pMove->GetPoint(MPE_SECTION_CC));
		p->SetCalcExtRect();
		CDRect rect = p->GetExtRect();
		pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업
		pMove->AddMPE(p,rect, MPE_SECTION_CC, "단면 C-C");
		*pDom << *p;

		p->SetScaleDim(dCurPageScale);
		DrawStd.AddTitleOnDom(p, dSectionCCScale, rect, "단면 C-C", TRUE, TRUE, 0);
		p->Move(pMove->GetPoint(MPE_SECTION_CC+1));
		p->SetCalcExtRect();
		pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업
		pMove->AddMPE(p, p->GetExtRect(), MPE_SECTION_CC+1, "단면 C-C 제목");
		*pDom << *p;
		delete p;
	}
	else
	{
		double xMove  = pBx->GetStation();
		double cxMove = pGirSec->GetLengthDimType(m_dPreStation, xMove);

		p->Move(m_dPreStation+cxMove, -dChgSlabAverH);

		CDPoint poEndU(-dSlabAllW+m_dPreStation+cxMove, dChgSlabAverH+dSlabH-dChgSlabAverH), poEndL(-dSlabAllW+m_dPreStation+cxMove, dChgSlabAverH-dChgSlabAverH);
		CDPoint poStartU(dSlabAllW+m_dPreStation+cxMove, dChgSlabAverH+dSlabH-dChgSlabAverH), poStartL(dSlabAllW+m_dPreStation+cxMove, dChgSlabAverH-dChgSlabAverH);

		if(pBx->IsJijum() && !m_pBxStt->IsState(BX_STT_SLAB))
		{
			if(pBx == m_pBxStt)
			{
				m_poArrStart.Add(poEndU);
				m_poArrStart.Add(poEndL);
			}
			else if(pBx == m_pBxEnd)
			{
				m_poArrEnd.Add(poStartU);
				m_poArrEnd.Add(poStartL);
			}
		}
		else
		{
			m_poArrEnd.Add(poEndU);
			m_poArrEnd.Add(poEndL);
			m_poArrStart.Add(poStartU);
			m_poArrStart.Add(poStartL);			
		}

		*pDom << *p;
		delete p;
	}
}

// 외측가로보 슬래브 상세
void CAPlateOutSlabGen::AddSectionWingBeam(CDomyun *pDom, long nPage, BOOL bSection, CPlateBasicIndex *pBx)
{

}

// 외측빔 슬래브 상세
void CAPlateOutSlabGen::AddSectionEndBeam(CDomyun *pDom, long nPage)
{

}

// 단면 E - E : 스트링거 슬래브 상세
void CAPlateOutSlabGen::AddSectionEE(CDomyun *pDom, long nPage)
{
/*	CARoadDrawStd	*pStd  = (CARoadDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd  = (CARoadOptionStd*)pStd->m_pOptStd;
	CBridgeApp		*pDB   = pStd->m_pDB;
	COptionStd		*pOpt  = pStd->m_pDomOptionStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp		*pGir  = pDB->GetGirder(m_nG);

	long nViewAddLen = 500;
	CDomyun *p = new CDomyun(pDom);	
	pOptStd->SetEnvType(p, HCAD_STLC);

	// Scale
	double dSectionEEScale = pOpt->m_dSlab_ScaleSectionEE;
	double dCurPageScale   = GetScale();
	p->SetScaleDim(dSectionEEScale);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;
	__int64 bxFlag = BX_CROSSBEAM_VBRACING;
	pBx = Finder.GetBxFirst(bxFlag, m_pBxStt, m_pBxEnd);
	double nCount = Finder.GetCountByFlag(bxFlag);
	pBx = Finder.GetBxNext(long(nCount / 2));

	CStringer *pS = NULL;
	while(pBx)
	{     
		if(pBx->IsStringer())
			break;
		else
			pBx = Finder.GetBxNext();
	}

	if(pBx==NULL)	return;
	pS = pBx->GetStringer();
	// 슬래브 변화
	CDPoint A[41];
	pGir->GetHunchXyDom(pBx, A);

	// 각 절점에서 수선으로 내린 점
	CDPoint uXy2(0,1);

	CDPoint xyResultFir, xyResultSec;

	GetXyMatchSegAndLine(A[10], A[11], A[12], uXy2, xyResultFir);

	double dChgSlabFirH = xyResultFir.y - A[12].y;
	double dChgSlabSecH = 0;
	GetXyMatchSegAndLine(A[15], A[16], A[14], uXy2, xyResultSec);
	dChgSlabSecH = xyResultSec.y - A[14].y;

	// 슬배브 변화 구간 제원
	double dChgSlabFirW = dChgSlabFirH * 3;
	double dChgSlabSecW = dChgSlabSecH * 3;
	double dChgSlabAverW = (dChgSlabFirW + dChgSlabSecW) / 2;
	double dChgSlabAverH = (dChgSlabFirH + dChgSlabSecH) / 2;
	double dSlabW = pDB->m_dWidthHunch;

	// 가로보 단면 제원
	double dStringerH = pS->m_H / 3;
	double dStringerWT = pS->m_T3;
	double dStringerUT = pS->m_T2;
	double dStringerLT = pS->m_T1;
	double dStringerUW = pS->m_uM;

	// 슬래브및 포장 제원
	double dPave = pDB->m_dThickPave;
	double dSlabH = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;

	double dSlabAllW = dStringerUW+dSlabW+dChgSlabAverW;	//전체 슬래브의 반
	// 가로보 단면 그리기
	p->Rectangle(-dStringerUW, dStringerUT, dStringerUW,0);
	p->Rectangle(0,0, dStringerWT, -dStringerH);
	pOptStd->SetEnvType(p, HCAD_CUTL);
	p->LineTo(-dStringerUW, -dStringerH, dStringerUW, -dStringerH);

	pOptStd->SetEnvType(p, HCAD_STLC);
	// 슬래브 그리기
	p->MoveTo(-dStringerUW,0);
	p->LineToEx(-dSlabW,0);
	p->LineToEx(-dChgSlabAverW, dChgSlabAverH);
	p->LineToEx(-nViewAddLen, 0);

	p->MoveTo(dStringerUW, 0);
	p->LineToEx(dSlabW, 0);
	p->LineToEx(dChgSlabAverW, dChgSlabAverH);
	p->LineToEx(nViewAddLen, 0);

	pOptStd->SetEnvLType(p, LT_DOT_LINE);
	p->MoveTo(-dSlabAllW, dChgSlabAverH);
	p->LineTo(dSlabAllW, dChgSlabAverH);

	pOptStd->SetEnvLType(p, LT_SOLID);
	// 포장
	double dPaveTopH = dChgSlabAverH + dSlabH + dPave;
	double dPaveBotH = dChgSlabAverH + dSlabH;
	p->Solid(-dSlabAllW-nViewAddLen, dPaveTopH, dSlabAllW+nViewAddLen, dPaveTopH, 
		dSlabAllW+nViewAddLen, dPaveBotH, -dSlabAllW-nViewAddLen, dPaveBotH);

	pOptStd->SetEnvType(p, HCAD_CUTL);
	// 절단면
	p->CutLightning(-dSlabAllW-nViewAddLen, dChgSlabAverH-p->Always(1),
		-dSlabAllW-nViewAddLen, dChgSlabAverH+dSlabH+dPave+p->Always(1));
	p->CutLightning(dSlabAllW+nViewAddLen, dChgSlabAverH-p->Always(1),
		dSlabAllW+nViewAddLen, dChgSlabAverH+dSlabH+dPave+p->Always(1));

	/////////////
	// 치수선
	CDimDomyunEx pDim(p);
	pOptStd->SetEnvType(&pDim, HCAD_DIML);

	// 큰수와 작은수 세팅
	GetSwapByMin(dChgSlabFirW, dChgSlabSecW);
	GetSwapByMin(dChgSlabFirH, dChgSlabSecH);

	pDim.SetDirection("BOTTOM");
	pDim.DimMoveTo(-dSlabAllW, 0, 1);
	pDim.DimLineTo(dChgSlabAverW, 0, COMMA(dChgSlabFirW)+"~"+COMMA(dChgSlabSecW));
	pDim.DimLineTo(dSlabW, 0, "", COMMA(dSlabW));
	pDim.DimLineTo(dStringerUW, 0, COMMA(dStringerUW));
	pDim.DimLineTo(dStringerUW, 0, COMMA(dStringerUW));
	pDim.DimLineTo(dSlabW, 0, "", COMMA(dSlabW));
	pDim.DimLineTo(dChgSlabAverW, 0, COMMA(dChgSlabFirW)+"~"+COMMA(dChgSlabSecW));

	pDim.SetDirection("LEFT");
	pDim.DimMoveTo(-dSlabAllW-nViewAddLen, 0, 1);
	pDim.DimLineTo(0, dChgSlabAverH, COMMA(dChgSlabFirH)+"~"+COMMA(dChgSlabSecH));
	pDim.DimLineTo(0, dSlabH, "", COMMA(dSlabH));
	pDim.DimLineTo(0, dPave, COMMA(dPave));

	pDim.SetDirection("TOP");
	pDim.GiSiArrowExtend(0,dChgSlabAverH+dSlabH+dPave,pDim.Always(10),pDim.Always(20),2,_T("아스팔트 포장"),_T("T = "+COMMA(dPave)+" MM"));		
	*p << pDim;

	p->RedrawByScale(dCurPageScale/dSectionEEScale);

	p->Move(pDom->Always(300), pDom->Always(150));
	p->Move(pMove->GetPoint(MPE_SECTION_EE));
	p->SetCalcExtRect();
	CDRect rect = p->GetExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(p,rect, MPE_SECTION_EE, "단면 E-E");
	*pDom << *p;

	p->SetScaleDim(dCurPageScale);
	DrawStd.AddTitleOnDom(p, dSectionEEScale, rect, "단면 E-E", TRUE, TRUE, 0);
	p->Move(pMove->GetPoint(MPE_SECTION_EE+1));
	p->SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(p, p->GetExtRect(), MPE_SECTION_EE+1, "단면 E-E 제목");
	*pDom << *p;
	delete p;*/
}

// 방호벽 상세
void CAPlateOutSlabGen::AddGuardWall(CDomyun *pDom, long nPage, long nGuard, BOOL bRight)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);
	
	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());
	CCentSeparation *pGWall = pDB->GetGuardWall(nGuard);
	long nHDan = pDB->GetNumHDanByGuardFence(pGWall);
	
	if(pGWall == NULL)
		return;

	long nGir = bRight ? pDB->GetGirdersu()-1 : 0;
	CPlateGirderApp *pGir  = pDB->GetGirder(nGir);

	CDomyun *p = new CDomyun(pDom);	
	pOptStd->SetEnvType(p, HCAD_STLC);

	// Scale
	double dGuardWallScale = pOpt->m_dSlab_ScaleGuideWall;
	double dCurPageScale   = GetScale();
	p->SetScaleDim(dGuardWallScale);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = GetBxSttPage(nPage, nGir);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nPage, nGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nHDanNode = pDB->GetQtyHDanNode()-1;
	//
	double BX = 0;
	double BY = 0;
	double LEN = 0;
	double Width = pGWall->GetWidth();
	double D1 = pGWall->m_D1;
	double Co = 0;

	double dSt = 0;

	long   pos = pGWall->GetPlace();
	
	if(pGWall->GetWidth() !=0)
	{
		if(pos==CCentSeparation::RIGHT) 
		{
			LEN = pDB->GetLengthHDanDom(pBx, nHDanNode);
			Co = Width-(LEN-D1);

			double lenHDan = pDB->GetLengthHDanDom(pBx,nHDan);
			dSt = Width - lenHDan;
		}

		DrawDanmyun.DrawGuardWall(p,pGWall,BX+Co,BY);
		pOptStd->SetEnvType(p,HCAD_DIML);
		DrawDanmyun.DimGuardWallTop(p,pGWall,BX+dSt,BY,FALSE,dGuardWallScale);

		if(pos==1) BX = -pGWall->m_D1;
		DrawDanmyun.DimGuardWallSide(p,pGWall,BX,BY,!bRight,FALSE,dGuardWallScale);
		
	}
	
	// SLAB 그리기
	double Len    = 0;
	double SlabT1 = pDB->m_dThickSlabLeft;
	double SlabT2 = pDB->m_dThickSlabBase;
	double SlabT3 = pDB->m_dThickSlabRight;
	double Pavement  =	pDB->m_dThickPave;
	double AddLen = 500;
	double EndSlab = Width+D1;

	// NOTCH
	double cyNotchH = 20;
	double cxNotchW = 30;

	// 배수구 관련
	double DrainPipeUW = 0;
	double DrainPipeW = 50;
	double DrainPipeH = 500;

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	pDB->GetXyPaveDom(m_pBxStt, A);

	double dDistH = A[0].y-A[1].y;
	double dDistW = A[0].x-A[1].x;
	double dSlopSlab = dDistW == 0 ? 0 :dDistH / dDistW;

	double dDistH1 = A[1].y-A[2].y;
	double dDistW1 = A[1].x-A[2].x;
	double dSlopSlab2 = dDistW1 == 0 ? 0 : dDistH1 / dDistW1;

	double dSlopSlabCheck  = (A[0].y-A[2].y)/(A[0].x-A[2].x);
	if(pos==CCentSeparation::LEFT)// 배수구및 기타 미완성
	{
		EndSlab = -D1;
		nHDanNode = pDB->GetQtyHDanNode()-1;
		Len = pDB->GetLengthHDanDom(pBx, nHDanNode)+AddLen+DrainPipeUW;
		double dSlopThick = (SlabT1-SlabT2)/Len;
		pOptStd->SetEnvType(p,HCAD_STLC);
		p->LineTo(EndSlab,  EndSlab*dSlopSlab,    Width, Width*dSlopSlab);
		p->LineTo(Width,  Width*dSlopSlab,    Len, Len*dSlopSlab2);
		p->LineTo(EndSlab,  EndSlab*dSlopSlab, EndSlab, EndSlab*dSlopSlab-SlabT1);
		EndSlab = Width;		
		//SlabLower
		p->LineToEx(EndSlab*0.3, EndSlab*0.3*dSlopThick);
		p->LineToEx(0, cyNotchH);
		p->LineToEx(cxNotchW, cxNotchW*dSlopThick);
		p->LineToEx(0, -cyNotchH);
		p->LineToEx(Len+EndSlab*0.7+cxNotchW, -(-Len+(+EndSlab*0.3+cxNotchW))*dSlopThick);

		Len = pDB->GetLengthHDanDom(pBx, nHDanNode)+AddLen+DrainPipeUW+Width+D1;
		// 배수구
		if(dSlopSlabCheck >= 0)
		{
			DrainPipeUW = 150;
			p->MoveTo(EndSlab, EndSlab*dSlopSlab);
			p->LineToEx(0, -DrainPipeW);
			p->LineToEx(DrainPipeW, -DrainPipeW);
			p->LineToEx(0, -DrainPipeH);
			p->LineToEx(DrainPipeW, 0);
			p->LineToEx(0, DrainPipeH);
			p->LineToEx(DrainPipeW, DrainPipeW);
			p->LineToEx(0, (DrainPipeW*3)*dSlopSlab2 + DrainPipeW);
		}
		pOptStd->SetEnvType(p,HCAD_CUTL);
		p->CutLightning(Len, -SlabT2-50, Len, Pavement+50+dSlopSlab2*Len);
		pOptStd->SetEnvType(p,HCAD_STLC);
		CDPoint XY1(EndSlab+DrainPipeUW, (EndSlab)*dSlopSlab);
		CDPoint XY2(Len                ,  Len*dSlopSlab2);
		CDPoint XY3(Len                , Len*dSlopSlab2+Pavement);
		CDPoint XY4(EndSlab+Pavement+DrainPipeUW, (EndSlab)*dSlopSlab+Pavement);
		p->Solid(XY1.x,XY1.y, XY2.x,XY2.y, XY3.x,XY3.y, XY4.x, XY4.y);

		pOptStd->SetEnvType(p, HCAD_DIML);
		p->SetDirection("TOP");
		XY1 = CDPoint(EndSlab+DrainPipeUW*2, (EndSlab+DrainPipeUW*2)*dSlopSlab+Pavement);
		p->GiSiArrowExtend(XY1.x,XY1.y,p->Always(10),p->Always(20),2,_T("아스팔트 포장"),_T("T = "+COMMA(Pavement)+"mm"));
		p->GiSiArrowExtend(EndSlab*0.3, EndSlab*0.3*dSlopSlab+(-SlabT2+SlabT1)/-Len*(-EndSlab)*0.3+-SlabT1,p->Always(10), -p->Always(15), 5, "NOTCH", "");
	}	
	else if(pos==CCentSeparation::CENTER)	 // CENTER
	{
		for(long n=0; n < pDB->GetQtyHDanNode(); n++)
		{
			if(pDB->GetWallGuardTypeHDan( n) == 1)  //방호벽이면
			{
				nHDanNode = n;
				if(n != 0) break;
			}
		}

		//Len = pDB->GetLengthHDanDom(pBx, nHDanNode);
		Len = pDB->GetLengthHDanDom(pBx, nHDan);//nHDanNode);
		double DiffDis    = (Len-pGWall->m_D1)*dSlopSlab;
		pOptStd->SetEnvType(p,HCAD_STLC);
		p->LineTo(-pGWall->m_D1, 0,   Len-pGWall->m_D1, DiffDis);
		p->LineTo(-pGWall->m_D1, -SlabT1,  Len-pGWall->m_D1, -SlabT1);

		pOptStd->SetEnvType(p,HCAD_CUTL);
		p->CutLightning(-pGWall->m_D1,-SlabT1-50, -pGWall->m_D1,Pavement+50);
		p->CutLightning( Len-pGWall->m_D1,-SlabT1-50, Len-pGWall->m_D1,Pavement+50+DiffDis);
		pOptStd->SetEnvType(p,HCAD_STLC);

		p->Solid(pGWall->GetWidth(),    0, Len-pGWall->m_D1, 0,
			        Len-pGWall->m_D1, Pavement, pGWall->GetWidth(), Pavement);

		p->Solid(-pGWall->m_D1,  0,  0, 0,
			        0, Pavement,  -pGWall->m_D1, Pavement);
	}
	else if(pos==CCentSeparation::RIGHT)	 // RIGHT
	{
		dDistH = A[0].y-A[1].y;
		dDistW = A[0].x-A[1].x;
		dSlopSlab = dDistW == 0 ? 0 :dDistH / dDistW;

		dDistH1 = A[1].y-A[2].y;
		dDistW1 = A[1].x-A[2].x;
		dSlopSlab2 = dDistW1 == 0 ? 0 : dDistH1 / dDistW1;

		if(A[nHDanNode+1] == A[nHDanNode+2])
		{
			dSlopSlab	= 0;
			dSlopSlab2	= 0;
		}

		//Width = pGWall->m_W1;		
		nHDanNode = pDB->GetQtyHDanNode()-1;
		Len = pDB->GetLengthHDanDom(pBx, nHDanNode)+AddLen+DrainPipeUW;
		double dSlopThick = (SlabT3-SlabT2)/Len;
		pOptStd->SetEnvType(p,HCAD_STLC);
		p->LineTo(EndSlab,  EndSlab*dSlopSlab,    dSt, dSt*dSlopSlab);
		p->LineTo(dSt, dSt*dSlopSlab,    -Len, -Len*dSlopSlab2);
		p->LineTo(EndSlab,  EndSlab*dSlopSlab, EndSlab, EndSlab*dSlopSlab-SlabT3);
		
		p->LineToEx(-EndSlab*0.3, EndSlab*0.3*dSlopThick);
		p->LineToEx(0, cyNotchH);
		p->LineToEx(-cxNotchW, cxNotchW*dSlopThick);
		p->LineToEx(0, -cyNotchH);
		p->LineToEx(-Len-EndSlab*0.7+cxNotchW, -(-Len+(EndSlab*0.3+cxNotchW))*dSlopThick);

		// 배수구
		if(dSlopSlabCheck >= 0)
		{
			DrainPipeUW = 150;
			p->MoveTo(dSt, 0);
			p->LineToEx(0, -DrainPipeW);
			p->LineToEx(-DrainPipeW, -DrainPipeW);
			p->LineToEx(0, -DrainPipeH);
			p->LineToEx(-DrainPipeW, 0);
			p->LineToEx(0, DrainPipeH);
			p->LineToEx(-DrainPipeW, DrainPipeW);
			p->LineToEx(0, -DrainPipeW*3*dSlopSlab2+DrainPipeW);
		}

		pOptStd->SetEnvType(p,HCAD_CUTL);
		p->CutLightning(-Len, -SlabT2-50, -Len, Pavement+50-Len*dSlopSlab2);
		pOptStd->SetEnvType(p,HCAD_STLC);

		CDPoint XY1(dSt-DrainPipeUW, dSt*dSlopSlab);
		CDPoint XY2(-Len                ,  -Len*dSlopSlab2);
		CDPoint XY3(-Len                , -Len*dSlopSlab2+Pavement);
		CDPoint XY4(dSt-DrainPipeUW-Pavement, dSt*dSlopSlab+Pavement);

		p->Solid(XY1.x ,XY1.y , XY2.x, XY2.y, XY3.x, XY3.y, XY4.x, XY4.y);

		pOptStd->SetEnvType(p, HCAD_DIML);
		p->SetDirection("TOP");
		p->GiSiArrowExtend(-DrainPipeUW*2, -DrainPipeUW*2*dSlopSlab+Pavement,p->Always(10),-p->Always(20),2,_T("아스팔트 포장"),_T("T = "+COMMA(Pavement)+"mm"));
		p->GiSiArrowExtend(EndSlab*0.7, EndSlab*dSlopSlab+(-SlabT2+SlabT3)/-Len*(-EndSlab)*0.3+-SlabT3,
			p->Always(10), p->Always(15), 7, "NOTCH", "30 X 20");
	}

	p->RedrawByScale(dCurPageScale/dGuardWallScale);

	long nMoveX = bRight ? 150 : 300;	//150* (nGuard+1);//
	long nMPE = bRight ? MPE_GUARD_WALL_RIGH : MPE_GUARD_WALL_LEFT;
	CString sTitle = pGWall->m_strName + " 상세";//CString sTitle = bRight ? "우측 방호벽 상세" : "좌측 방호벽 상세";
	//sTitle = bCenWall ? "중앙 분리대 상세" : sTitle;
	p->Move(pDom->Always(400), pDom->Always(nMoveX));
	p->Move(pMove->GetPoint(nMPE));
	p->SetCalcExtRect();
	CDRect rect = p->GetExtRect();
	pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업
	pMove->AddMPE(p,rect, nMPE, sTitle);
	*pDom << *p;

	p->SetScaleDim(dCurPageScale);
	DrawStd.AddTitleOnDom(p, dGuardWallScale, rect, sTitle, TRUE, TRUE, 0);
	p->Move(pMove->GetPoint(nMPE+1));
	p->SetCalcExtRect();
	pStd->ChangeSeperateLayer(p); // HCAD 블럭을 위한 작업
	pMove->AddMPE(p, p->GetExtRect(), nMPE+1, sTitle + " 제목");
	*pDom << *p;
	delete p;
}

// NOTCH 상세
void CAPlateOutSlabGen::AddNotch(CDomyun *pDom, long nPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();

	if(pOpt->m_nSlab_NotchType == 0) 
		AddRectangleNotch(pDom, nPage);
	else
		AddTrapezoidNotch(pDom, nPage);
}

// 사각형 NOTCH 상세
void CAPlateOutSlabGen::AddRectangleNotch(CDomyun *pDom, long nPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	double dThick = 5;
	double dWidth = 50;
	double dNotchH	= pOpt->m_dSlab_NotchHeight;
	double dNotchW	= pOpt->m_dSlab_NotchTopWidth;

	// Scale
	double dNotchScale = pOpt->m_dSlab_ScaleNothch;
	double dCurPageScale = GetScale();
	Dom.SetScaleDim(dNotchScale);

	// NOTCH 그리기
	CDPoint xyStt(0,0), xyTemp(0,0);
	CDPoint vX(1,0), vY(0,1);

	Dom.MoveTo(xyStt);
	xyTemp = xyStt + vX*dWidth;
	Dom.LineTo(xyTemp);
	xyTemp += vY*dNotchH;
	Dom.LineTo(xyTemp);
	xyTemp += vX*dNotchW;
	Dom.LineTo(xyTemp);
	xyTemp += vY*(-dNotchH);
	Dom.LineTo(xyTemp);
	xyTemp += vX*dWidth;
	Dom.LineTo(xyTemp);
	xyTemp += vY*dThick;
	Dom.LineTo(xyTemp);
	xyTemp += vX*(-dWidth+dThick);
	Dom.LineTo(xyTemp);
	xyTemp += vY*dNotchH;
	Dom.LineTo(xyTemp);
	xyTemp += vX*(-dNotchW-2*dThick);
	Dom.LineTo(xyTemp);
	xyTemp += vY*(-dNotchH);
	Dom.LineTo(xyTemp);
	xyTemp += vX*(-dWidth+dThick);
	Dom.LineTo(xyTemp);
	xyTemp += vY*(-dThick);
	Dom.LineTo(xyTemp);

	// ANCHOR
	double dAnchorHT	= 4;
	double dAnchorHH	= 1;
	double dAnchorBT	= 0.6;
	double dAnchorBH	= 20;
	double dAnchorHole	= 10;

	Dom.Rectangle((dWidth-dAnchorHT)/2,	0, (dWidth+dAnchorHT)/2, -dAnchorHH);
	Dom.Rectangle((dWidth-dAnchorBT)/2, dAnchorBH, (dWidth+dAnchorBT)/2, 0);
	Dom.Rectangle(dWidth+dNotchW+(dWidth-dAnchorHT)/2, 0, dWidth+dNotchW+(dWidth+dAnchorHT)/2, -dAnchorHH);
	Dom.Rectangle(dWidth+dNotchW+(dWidth-dAnchorBT)/2, dAnchorBH, dWidth+dNotchW+(dWidth+dAnchorBT)/2, 0);

	// ANCHOR 구멍
	Dom.LineTo((dWidth-dAnchorHole)/2, dThick, (dWidth-dAnchorHole)/2, 0);
	Dom.LineTo((dWidth+dAnchorHole)/2, dThick, (dWidth+dAnchorHole)/2, 0);
	Dom.LineTo(dWidth+dNotchW+(dWidth-dAnchorHole)/2, dThick, dWidth+dNotchW+(dWidth-dAnchorHole)/2, 0);
	Dom.LineTo(dWidth+dNotchW+(dWidth+dAnchorHole)/2, dThick, dWidth+dNotchW+(dWidth+dAnchorHole)/2, 0);

	// 치수기입
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(0, 0, 0);
	Dom.DimLineTo(dWidth,	0, _T(COMMA(dWidth)));
	Dom.DimLineTo(dNotchW,	0, _T(COMMA(dNotchW)));
	Dom.DimLineTo(dWidth,	0, _T(COMMA(dWidth)));
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(0, 0, 0);
	Dom.DimLineTo(dNotchH,	0, _T(COMMA(dNotchH)));

	Dom.SetDirection("TOP");
	pOptStd->SetEnvType(&Dom, HCADEX_GISI);
	Dom.GiSiArrowExtend(dWidth+dNotchW+(dWidth+dAnchorBT)/2, dAnchorBH,
						Dom.Always(10), Dom.Always(25), 1, _T("SET ANCHOR"),_T("C.T.C 1.000"));
	Dom.GiSiArrowExtend(dWidth+dNotchW+dWidth*3/4, dThick,
						Dom.Always(5), Dom.Always(25), 1, _T("알루미늄판"),_T("T=5mm"));

	Dom.RedrawByScale(dCurPageScale/dNotchScale);	
	
	Dom.Move(pDom->Always(400), pDom->Always(50));
	Dom.SetCalcExtRect();		
	Dom.Move(pMove->GetPoint(MPE_NOTCH));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();	
	pMove->AddMPE(&Dom, rect, MPE_NOTCH, "NOTCH 상세");
	*pDom << Dom;

	Dom.SetScaleDim(dCurPageScale);
	DrawStd.AddTitleOnDom(&Dom, dNotchScale, rect, "NOTCH 상세", TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(MPE_NOTCH+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_NOTCH+1, "NOTCH 상세 제목");

	*pDom << Dom;
}

// 제형 NOTCH 상세
void CAPlateOutSlabGen::AddTrapezoidNotch(CDomyun *pDom, long nPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove   = pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	CDomyun Dom(pDom);
		
	double dNotchH		= pOpt->m_dSlab_NotchHeight;
	double dNotchTopW	= pOpt->m_dSlab_NotchTopWidth;
	double dNotchBotW	= pOpt->m_dSlab_NotchBotWidth;
	double dRad			= 50+dNotchBotW/2;

	// Scale
	double dNotchScale = pOpt->m_dSlab_ScaleNothch;
	double dCurPageScale = GetScale();
	Dom.SetScaleDim(dNotchScale);

	// 절단선
	pOptStd->SetEnvType(&Dom, HCAD_MIS1);
	CDPointArray xyArr;
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyStt(0,0), xyTemp(0,0);

	Dom.Arc(dRad, 0, dRad, -15, 210);

	xyArr.Add(xyTemp);
	for(long i=1 ; i<18 ; i++)
	{
		xyTemp.x = dRad*(1-cos(ToRadian(10*i))); 
		xyTemp.y = dRad*sin(ToRadian(10*i));
		xyArr.Add(xyTemp);
	}

	// NOTCH 그리기
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	xyTemp.x = dRad*2;
	xyTemp.y = 0;
	xyArr.Add(xyTemp);
	Dom.MoveTo(xyTemp);
	xyTemp += vX*(-dRad+dNotchBotW/2);
	Dom.LineTo(xyTemp);
	xyArr.Add(xyTemp);
	xyTemp += vX*(-dNotchBotW+dNotchTopW)/2 + vY*dNotchH;
	Dom.LineTo(xyTemp);
	xyArr.Add(xyTemp);
	xyTemp += vX*(-dNotchTopW);
	Dom.LineTo(xyTemp);
	xyArr.Add(xyTemp);
	xyTemp += vX*(-dNotchBotW+dNotchTopW)/2 + vY*(-dNotchH);
	Dom.LineTo(xyTemp);
	xyArr.Add(xyTemp);
	xyTemp += vX*(-dRad+dNotchBotW/2);
	Dom.LineTo(xyTemp);
	xyArr.Add(xyTemp);

	Dom.HatchingByCDPointArray(xyArr, 85, 10, FALSE);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(dRad-dNotchBotW/2, 0, 0);
	Dom.DimLineTo(dNotchBotW, 0, _T(COMMA(dNotchBotW)));
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(dRad-dNotchBotW/2, 0, 0);
	Dom.DimLineTo(dNotchH,	0, _T(COMMA(dNotchH)));
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dRad-dNotchTopW/2, dNotchH, 0);
	Dom.DimLineTo(dNotchTopW,	0, _T(COMMA(dNotchTopW)));
	
	pOptStd->SetEnvType(&Dom, HCADEX_GISI);
	Dom.GiSiArrowExtend(dRad+dNotchTopW/2, dNotchH,
						Dom.Always(10), Dom.Always(25), 1, _T("NOTCH"),_T("사다리꼴 면목"));
	
	Dom.RedrawByScale(dCurPageScale/dNotchScale);	
	
	Dom.Move(pDom->Always(400), pDom->Always(50));
	Dom.SetCalcExtRect();
		
	Dom.Move(pMove->GetPoint(MPE_NOTCH));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();	
	pMove->AddMPE(&Dom, rect, MPE_NOTCH, "NOTCH 상세");
	*pDom << Dom;

	Dom.SetScaleDim(dCurPageScale);
	DrawStd.AddTitleOnDom(&Dom, dNotchScale, rect, "NOTCH 상세", TRUE, TRUE, 0);
	Dom.Move(pMove->GetPoint(MPE_NOTCH+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_NOTCH+1, "NOTCH 상세 제목");

	*pDom << Dom;
}

CPlateBasicIndex* CAPlateOutSlabGen::GetBxDimensionTable(long nPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(0);

	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex *pBxStt = GetBxSttPage(nPage,-1);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nPage,-1);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

//	if(IsStringerInPage(nPage)==FALSE)	return pBx;

	// 세로보가 있는 Bx를 구함...
/*	BOOL bCheckStringer = FALSE;
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		pGir = pDB->GetGirder(nG);

		CPlateBasicIndex *pBxStt = GetBxSttPage(nPage,nG);
		CPlateBasicIndex *pBxEnd = GetBxEndPage(nPage,nG);
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
		
		while(pBx)
		{
			if(pBx->IsStringer())
			{
				bCheckStringer = TRUE;
				break;
			}
			pBx = Finder.GetBxNext();
		}
		if(bCheckStringer) break;
	}
*/
	return pBx;
}

BOOL CAPlateOutSlabGen::IsStringerInPage(long nPage)
{
/*	CARoadDrawStd *pStd = (CARoadDrawStd*)m_pStd;
	CBridgeApp	  *pDB  = pStd->m_pDB;
	CPlateGirderApp	  *pGir = pDB->GetGirder(-1);

	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		pGir = pDB->GetGirder(nG);

		CPlateBasicIndex *pBxStt = GetBxSttPage(nPage,nG);
		CPlateBasicIndex *pBxEnd = GetBxEndPage(nPage,nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
		
		while(pBx)
		{
			if(pBx->IsStringer())
				return TRUE;
			pBx = Finder.GetBxNext();
		}
	}*/

	return FALSE;
}

BOOL CAPlateOutSlabGen::IsStringerInPage(long nPage, long nG)
{
/*	CARoadDrawStd *pStd = (CARoadDrawStd*)m_pStd;
	CBridgeApp	  *pDB  = pStd->m_pDB;
	CPlateGirderApp	  *pGir = pDB->GetGirder(nG);

	CPlateBasicIndex *pBxStr = GetBxDimensionTable(nPage);
	CPlateBasicIndex *pBxGir = pBxStr->GetBxMatchByCrossBeam(pGir);

	return pBxGir->IsStringer();*/
	return FALSE;
}
void CAPlateOutSlabGen::DrawCenterLineAndMark(CDomyun *pDomP, long subPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	CDomyun* pDom = new CDomyun(pDomP);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_CENT);
	
	CPlateBasicIndex *pBxStt = GetBxSttPage(subPage,-1);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(subPage,-1);


	// 도로중심선 (MAIN 선형)
	double  sttStation = pBxStt->GetStation();
	double  endStation = pBxEnd->GetStation();

	DrawPyung.DrawCenterLine(pDom,-1,sttStation,endStation);

	*pDomP << *pDom;
	delete pDom;
}

// 거더 상하부 플랜지 라인을 그림.
void CAPlateOutSlabGen::AddPlanGirderLine(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	BOOL bWebLine = TRUE;
	long nGsu = pDB->GetGirdersu();
	// nSide:-1 중심거더기준 좌측거더
	// nSide:-1 중심거더기준 우측거더

	CDomyun Dom(pDom);

/*	for(long nSide=-1;nSide<=1; nSide+=2)
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
		// 거더수가 홀수일때는 중심거더를 기준으로 좌측, 우측, 중심거더를 따라 그림.
		// 중심거더
		if(nGsu%2)	
		{
			long nG = nGsu/2;
			CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nG, FALSE);
			CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nG, FALSE);	
			CPlateBxFinder	Finder(pDB->GetGirder(nG));
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | (nSide==-1 ? BX_UPPERFLANGE : BX_LOWERFLANGE),pBxStt,pBxEnd);
			if(pBx != pBxStt)
			{
				DrawPyung.DrawGirderWithenBx   (&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), nSide, 0);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), nSide, nLine);	
			}
			while(Finder.GetBxFixNext())
			{
				DrawPyung.DrawGirderWithenBx   (&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), nSide, 0);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), nSide, nLine);	
				pBx = Finder.GetBxNext();
			}
			if(pBx != pBxEnd)
			{
				DrawPyung.DrawGirderWithenBx   (&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), nSide, 0);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), nSide, nLine);	
			}
		}

		for(long nG=nSttGir; nG<nEttGir; nG++)
		{
			CPlateBxFinder	Finder(pDB->GetGirder(nG));
			CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nG, FALSE);
			CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nG, FALSE);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | (nSide==-1 ? BX_UPPERFLANGE : BX_LOWERFLANGE), pBxStt, pBxEnd);				
			if(pBx != pBxStt)
			{
				DrawPyung.DrawGirderWithenBx   (&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), 0, 0);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, (nSide==-1 ? TRUE : FALSE), 0, nLine);					
			}
			while(Finder.GetBxFixNext())
			{					
				DrawPyung.DrawGirderWithenBx   (&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), 0, 0);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), (nSide==-1 ? TRUE : FALSE), 0, nLine);	
				pBx = Finder.GetBxNext();
			}			
			if(pBx != pBxEnd)
			{
				DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), 0, 0);	
				if(bWebLine)
					DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, (nSide==-1 ? TRUE : FALSE), 0, nLine);
			}
		}		
	}	*/
	for(long nG=0; nG<nGsu; nG++)
	{
		pStd->m_pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);

		CPlateBxFinder	Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nG, FALSE);
		CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nG, FALSE);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | BX_UPPERFLANGE, pBxStt, pBxEnd);				
		if(pBx != pBxStt)
		{
			DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBx, TRUE, 0, 0);	
			if(bWebLine)
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, TRUE, 0, 0);					
		}
		while(Finder.GetBxFixNext())
		{					
			DrawPyung.DrawGirderWithenBx   (&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0, 0);	
			if(bWebLine)
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0, 0);	
			pBx = Finder.GetBxNext();
		}			
		if(pBx != pBxEnd)
		{
			DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, TRUE, 0, 0);	
			if(bWebLine)
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, TRUE, 0, 0);
		}
	}		
	*pDom << Dom;
	
}

