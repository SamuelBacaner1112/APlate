// APlateOutVBracing.cpp: implementation of the CAPlateOutVBracing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MPE_VBRACING_FRONT		10
#define MPE_VBRACING_PLAN		20
#define MPE_VBRACING_SANGSE1	31
#define MPE_VBRACING_SANGSE2	32
#define MPE_VBRACING_SANGSE3	33 
#define MPE_VBRACING_SANGSE4	34
#define MPE_VBRACING_SANGSE5	35
#define MPE_VBRACING_DANMYUNAA	41
#define MPE_VBRACING_DANMYUNBB	42
#define MPE_VBRACING_ANCHORBAR	50

#define MPE_VBRACING_KEYPLAN		80
#define MPE_VBRACING_TABLE			110
#define MPE_VBRACING_TABLETITLE		120
#define MPE_VBRACING_MARK			130
#define MPE_VBRACING_KEY_PAN		140

CAPlateOutVBracing::CAPlateOutVBracing()
{

}  

CAPlateOutVBracing::~CAPlateOutVBracing()
{

}

void CAPlateOutVBracing::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng *pPageMng	 	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove	    = pStd->GetMovePoint(pStd->m_nCurPage);
	
	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// 일반도이전의 도면번호
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();

	double dFrontScale  = pStd->m_pDomOptionStd->m_VBracing_Scale.GetAt(2);
	double dPlanScale   = pStd->m_pDomOptionStd->m_VBracing_Scale.GetAt(3);
	double dSangseScale = pStd->m_pDomOptionStd->m_VBracing_Scale.GetAt(4);
	double dAnchorScale = pStd->m_pDomOptionStd->m_VBracing_Scale.GetAt(5);
	double dKeypanScale = pStd->m_pDomOptionStd->m_VBracing_Scale.GetAt(6);

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	
	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
	//long nSttPage = pDomOpt->m_nDomPageArray.GetAt(PAGE_VSDETAIL) == TRUE ? 0 : pDomOpt->m_nDomPageArray.GetAt(PAGE_VSDETAIL) + nSubPage - 1;	
	//pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sDomNumHead,"",nSttPage);
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
	CDPoint xyMove	 = CDPoint(Dom.Always(80), DgkBound.Height()-Dom.Always(80));

	CDPoint xyMoveLower(0,0), xyMoveAnchor(0,0);
	
	// 평면도(상면)
	if(pStd->GetDomOptionStd()->m_VBracing_Plan)
	{
		if(nSubPage-1 < m_VBracingList.GetSize())
		{
			CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
			CPlateBasicIndex *pBx = pVr->GetBx();

			CDPoint vGir = pBridge->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
			CDPoint xyOrigin = pBridge->GetGirder(0)->GetXyGirderDis(pBx->GetStation());

			pPageMng->SetProgressData("평면도 출력중 ...",40);
			Dom.SetScaleDim(dPlanScale);
			AddVBracingPyungTotalDanmyun(&Dom, pBx,TRUE);
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.Rotate(-vGir.GetAngleRadian() + CDPoint(0,1).GetAngleRadian(), xyOrigin.x, xyOrigin.y);
			Dom.RedrawByScale(GetScale()/dPlanScale);//확대

			Dom.SetCalcExtRect();
			CDRect rect = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rect.left, -rect.top-rect.Height()); 	
			xyMove.y -= rect.Height()+Dom.Always(40);
			
			//MPE추가시작
			Dom.Move(pMove->GetPoint(MPE_VBRACING_PLAN));
			Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_PLAN,"평면도(상면)");
			*pDom << Dom;
			// TITLE
			DrawStd.AddTitleOnDom(&Dom, dPlanScale, Dom.GetExtRect(), "평면도(상면)", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_VBRACING_PLAN+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_PLAN+1,"평면도 제목(상면)");
			*pDom << Dom;
			//MPE추가종료
		}		
	}


	// 횡단면도
	if(pStd->GetDomOptionStd()->m_VBracing_Front)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("횡단면도 출력중 ...",30);
		if(nSubPage-1 < m_VBracingList.GetSize())
		{
			CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
			CPlateBasicIndex *pBx = pVr->GetBx();
			Dom.SetScaleDim(dFrontScale);
			AddHDanVBracing(&Dom, pBx);		
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.RedrawByScale(GetScale()/dFrontScale);//확대
		
			Dom.SetCalcExtRect();
			CDRect rect = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rect.left, -rect.top-rect.Height()); 	
			xyMove.y -= rect.Height()+Dom.Always(40);

			//MPE추가시작
			Dom.Move(pMove->GetPoint(MPE_VBRACING_FRONT));
			Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_FRONT,"횡단면도");
			*pDom << Dom;
			// TITLE
			DrawStd.AddTitleOnDom(&Dom, dFrontScale, Dom.GetExtRect(), "횡단면도", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_VBRACING_FRONT+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_FRONT+1,"횡단면도 제목");
			*pDom << Dom;
			//MPE추가종료
		}
	}
	// 평면도(하면)
	if(pStd->GetDomOptionStd()->m_VBracing_Plan)
	{
		if(nSubPage-1 < m_VBracingList.GetSize())
		{
			CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
			CPlateBasicIndex *pBx = pVr->GetBx();

			CDPoint vGir = pBridge->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
			CDPoint xyOrigin = pBridge->GetGirder(0)->GetXyGirderDis(pBx->GetStation());

			pPageMng->SetProgressData("평면도 출력중 ...",40);
			Dom.SetScaleDim(dPlanScale);
			AddVBracingPyungTotalDanmyun(&Dom, pBx,FALSE);
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.Rotate(-vGir.GetAngleRadian() + CDPoint(0,1).GetAngleRadian(), xyOrigin.x, xyOrigin.y);
			Dom.RedrawByScale(GetScale()/dPlanScale);//확대

			Dom.SetCalcExtRect();
			CDRect rect = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rect.left, -rect.top-rect.Height()); 	
			xyMove.y -= rect.Height()+Dom.Always(40);
			
			//MPE추가시작
			Dom.Move(pMove->GetPoint(MPE_VBRACING_PLAN+2));
			Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_PLAN+2,"평면도(하면)");
			*pDom << Dom;
			// TITLE
			DrawStd.AddTitleOnDom(&Dom, dPlanScale, Dom.GetExtRect(), "평면도(하면)", TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_VBRACING_PLAN+3));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_PLAN+3,"평면도 제목(하면)");
			
			*pDom << Dom;
			//MPE추가종료
		}		
	}

	
	//상세1
	if(pStd->GetDomOptionStd()->m_VBracing_Detail)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("상세도 1 출력중 ...",50);
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		Dom.SetScaleDim(dSangseScale);
		AddHDanVBracingSangse(&Dom, pBx, 1);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dSangseScale);//확대
	
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMoveLower = xyMove - CDPoint(0,1)*(rect.Height()+Dom.Always(60));
		xyMove.x += rect.Width()+Dom.Always(40);
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE1));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE1,"상세도 1");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(pDom->GetScaleDim());
		DrawStd.AddTitleOnDom(&Dom, dSangseScale, Dom.GetExtRect(), "상세도 1", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE1+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE1+1,"상세도 1 제목");
		*pDom << Dom;
		//MPE추가종료
	}
	//상세2
	if(pStd->GetDomOptionStd()->m_VBracing_Detail)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("상세도 2 출력중 ...",55);
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		Dom.SetScaleDim(dSangseScale);
		AddHDanVBracingSangse(&Dom, pBx, 2);	
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dSangseScale);//확대

		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMove.x += rect.Width()+Dom.Always(40);
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE2));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE2,"상세도 2");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(pDom->GetScaleDim());
		DrawStd.AddTitleOnDom(&Dom, dSangseScale, Dom.GetExtRect(), "상세도 2", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE2+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE2+1,"상세도 2 제목");
		*pDom << Dom;
		//MPE추가종료
	}
	//상세3
	if(pStd->GetDomOptionStd()->m_VBracing_Detail)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("상세도 3 출력중 ...",60);
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		Dom.SetScaleDim(dSangseScale);
		AddHDanVBracingSangse(&Dom, pBx, 3);	
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dSangseScale);//확대

		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMoveAnchor = xyMove;
		xyMoveAnchor.x += rect.Width() + Dom.Always(40);
		//MPE추가시작
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE3));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE3,"상세도 3");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(pDom->GetScaleDim());
		DrawStd.AddTitleOnDom(&Dom, dSangseScale, Dom.GetExtRect(), "상세도 3", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE3+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE3+1,"상세도 3 제목");
		*pDom << Dom;
		//MPE추가종료
	}
	//상세4
	if(pStd->GetDomOptionStd()->m_VBracing_Detail)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("상세도 4 출력중 ...",65);
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		Dom.SetScaleDim(dSangseScale);
		AddHDanVBracingSangse(&Dom, pBx, 4);	
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dSangseScale);//확대

		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMoveLower);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMoveLower.x += rect.Width()+Dom.Always(30);
		//MPE추가시작
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE4,"상세도 4");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(pDom->GetScaleDim());
		DrawStd.AddTitleOnDom(&Dom, dSangseScale, Dom.GetExtRect(), "상세도 4", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE4+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE4+1,"상세도 4 제목");
		*pDom << Dom;
		//MPE추가종료

	}
	//상세5
	if(pStd->GetDomOptionStd()->m_VBracing_Detail)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("상세도 5 출력중 ...",70);
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		Dom.SetScaleDim(dSangseScale);
		AddHDanVBracingSangse(&Dom, pBx, 5);	
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dSangseScale);//확대

		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMoveLower);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMoveLower.x += rect.Width()+Dom.Always(30);
		//MPE추가시작
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE5));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE5,"상세도 5");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(pDom->GetScaleDim());
		DrawStd.AddTitleOnDom(&Dom, dSangseScale, Dom.GetExtRect(), "상세도 5", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_SANGSE5+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_SANGSE5+1,"상세도 5 제목");
		*pDom << Dom;
		//MPE추가종료
	}
	//단면A-A
	if(pStd->GetDomOptionStd()->m_VBracing_Detail)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("단면A-A 출력중 ...",75);
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		Dom.SetScaleDim(dSangseScale);
		AddHDanVBracingDanmyunAA(&Dom, pBx);	
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dSangseScale);//확대

		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMoveLower);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMoveLower.x += rect.Width()+Dom.Always(30);
		//MPE추가시작
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		Dom.Move(pMove->GetPoint(MPE_VBRACING_DANMYUNAA));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_DANMYUNAA,"단면A-A");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(pDom->GetScaleDim());
		DrawStd.AddTitleOnDom(&Dom, dSangseScale, Dom.GetExtRect(), "단면A-A", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_DANMYUNAA+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_DANMYUNAA+1,"단면A-A 제목");
		*pDom << Dom;
		//MPE추가종료
	}
	//단면B-B
	if(pStd->GetDomOptionStd()->m_VBracing_Detail)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("단면B-B 출력중 ...",80);
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		Dom.SetScaleDim(dSangseScale);
		AddHDanVBracingDanmyunBB(&Dom, pBx);	
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dSangseScale);//확대
	
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMoveLower);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMoveLower.x += rect.Width()+Dom.Always(30);
		//MPE추가시작
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		Dom.Move(pMove->GetPoint(MPE_VBRACING_DANMYUNBB));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_DANMYUNBB,"단면B-B");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(pDom->GetScaleDim());
		DrawStd.AddTitleOnDom(&Dom, dSangseScale, Dom.GetExtRect(), "단면B-B", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_DANMYUNBB+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_DANMYUNBB+1,"단면B-B 제목");
		*pDom << Dom;
		//MPE추가종료
	}
	//요약도
	if(pStd->GetDomOptionStd()->m_VBracing_KeyPlan)
	{
//		EXIT_RTN;			
		CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
		CPlateBasicIndex *pBx = pVr->GetBx();
		pPageMng->SetProgressData("요약도 출력중...",85);						
		AddKeyPan(&Dom,0,0, pBx);
		Dom.SetScaleDim(pDom->GetScaleDim());
	
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMoveLower);
		Dom.Move(-rect.left, -rect.top-rect.Height()+Dom.Always(30)); 	
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_VBRACING_KEY_PAN));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_KEY_PAN, "요약도 상세");
		*pDom << Dom;
		// TITLE
		rect = Dom.GetExtRect();
		rect.bottom -= Dom.Always(30);
		DrawStd.AddTitleOnDom(&Dom, dKeypanScale, rect, "요약도 상세", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_KEY_PAN+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_KEY_PAN+1, "요약도 상세 제목");
		//MPE추가종료
		*pDom << Dom;
	}
	//슬래브앵커
	CVBracing* pVr = m_VBracingList.GetAt(nSubPage-1);
	CPlateBasicIndex *pBx = pVr->GetBx();
	if(pStd->GetDomOptionStd()->m_VBracing_AnchorBarDetail && pVr->m_nAnchorSu > 0)
	{
		pPageMng->SetProgressData("슬래브앵커 상세 출력중...", 90);

		Dom.SetScaleDim(dAnchorScale);
		DrawDetailAnchorBar(&Dom, pBx);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/dAnchorScale);//확대

		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMoveAnchor);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_VBRACING_ANCHORBAR));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_ANCHORBAR, "슬래브앵커 상세");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, dAnchorScale, Dom.GetExtRect(), "슬래브앵커 상세", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_ANCHORBAR+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_ANCHORBAR+1, "슬래브앵커 상세 제목");
		//MPE추가종료
		*pDom << Dom;
	}	

	if (pStd->GetDomOptionStd()->m_VBracing_BM)
	{
		pPageMng->SetProgressData("재료표 출력중...",95);
		AddDetailBM(&Dom, pBx);
		Dom.SetCalcExtRect();
		CDRect	rect = Dom.GetExtRect();
		CDPoint xy;
		xy.x = DgkBound.Width() - rect.Width() - Dom.Always(20);
		xy.y = DgkBound.Height() - Dom.Always(60);
		Dom.Move(xy.x,xy.y);
		Dom.Move(pMove->GetPoint(MPE_VBRACING_TABLE));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_TABLE,"재료표");
		*pDom << Dom;
	}

	if(pStd->GetDomOptionStd()->m_VBracing_Note)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("노트사항 출력중...",100);
		InitNote();
		AddNote(&Dom,nSubPage);
		//MPE추가시작
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		Dom.Move(pMove->GetPoint(MPE_VBRACING_KEYPLAN));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_VBRACING_KEYPLAN,"노트사항");
		*pDom << Dom;
		//MPE추가종료
	}

	pPageMng->SetProgressData("출력완료",100);
}

long CAPlateOutVBracing::GetPageSu()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng *pPageMng		= pStd->GetDrawPageMng();	

	MakeVBracingList();
	BOOL bSummary = FALSE;//pDomOpt->m_VBracing_Summary && (pDomOpt->m_VBracing_Pyung || pDomOpt->m_VBracing_Table);

	if (m_VBracingList.GetSize() == 0)
	{
		m_nPage = 0;
		return m_nPage;
	}
	else
	{
		m_nPage = m_VBracingList.GetSize();
		if(bSummary)	
			m_nPage++;
		return m_nPage;
	}
	m_nPage = 1;
}

void CAPlateOutVBracing::MakeVBracingList()
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	CTypedPtrArray	<CObArray, CVBracing*> crListTemp;
	crListTemp.RemoveAll();
	
	m_VBracingList.RemoveAll();

	CPlateGirderApp	*pGir = pBridge->GetGirder(0);
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	while(pBx)
	{
		if(pBx->GetSection()->GetenDetType() == SECDET_VBRACING)
		{
			CVBracing* pVr = pBx->GetSection()->GetVBracing();
			crListTemp.Add(pVr);
		}

		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}

	long n = 0;
	//sort 지점부 - 일반부
	for (n = 0; n < crListTemp.GetSize(); n++)
	{
		if (crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_VBracingList.Add(crListTemp.GetAt(n));
	}
	for (n = 0; n < crListTemp.GetSize(); n++)
	{
		if (!crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_VBracingList.Add(crListTemp.GetAt(n));
	}
}
double CAPlateOutVBracing::GetScale()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt = pStd->m_pDomOptionStd;

	return pOpt->m_VBracing_Scale.GetAt(2);
}

CString CAPlateOutVBracing::GetTitle()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CString szTitle;

	long nPageSu = m_nPage;
	if(nPageSu>1)	szTitle.Format("수직브레이싱 상세도 (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else			szTitle.Format("수직브레이싱 상세도");

	return szTitle;	
}

void CAPlateOutVBracing::InitNote()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();			
	CSteelSection   *pSteelSec  = pStd->GetDataManage()->GetSteelSection();

	long   nBujeMark = 0;
	CString	strBuje1 = "주부재";
	long	n	= 0;
	for (n=0; n < TOTAL_PIECE_VALUE; n++)
	{
		if (strBuje1 == pSteelSec->GetClass(n))
		{
			nBujeMark = n;
			break;
		}
	}	
	strBuje1 = strBuje1+"("+pSteelSec->GetMaterialByMarkBuje(nBujeMark)+")";

	CString	strBuje2 = "부부재";
	for (n=0; n < TOTAL_PIECE_VALUE; n++)
	{
		if (strBuje2 == pSteelSec->GetClass(n))
		{
			nBujeMark = n;
			break;
		}
	}
	strBuje2 = strBuje2+"("+pSteelSec->GetMaterialByMarkBuje(nBujeMark)+")";

	m_sNoteArr.RemoveAll();
	m_sNoteArr.Add(".PLATE GIRDER 제작시 평면선형도와 종단경사를 참조하여");
	m_sNoteArr.Add("제작하여야 한다");
	m_sNoteArr.Add(".SOLE PLATE 제작시 GIRDER의 종단경사를 고려하여");
	m_sNoteArr.Add("제작하여야 한다");
	m_sNoteArr.Add(".신축이음장치 설치부의 단면은 강상형 제작전에 공급");
	m_sNoteArr.Add("업체로부터 관련제원을 확인하여 감독원(감리원)의 승");
	m_sNoteArr.Add("인하에 변경할 수 있다");
	m_sNoteArr.Add(".각 용접부의 용접 검사는 KSB-0845, KSB-0816를 만족");
	m_sNoteArr.Add("하는 용접이 될 수 있도록 한다");
	m_sNoteArr.Add("." + strBuje1 + "와 "+strBuje2+"를 용접하는 곳은");
	m_sNoteArr.Add("고강도용접을 하여야 한다");
	m_sNoteArr.Add(".부재의 조립후에 도장면이 되거나 외부에 자유단이 되는");
	m_sNoteArr.Add("절단면의 각은 반경 1mm 이상의 둥근형의 모서리 갈기를");
	m_sNoteArr.Add(" 하고, 기타 자유단은 반경 0.5mm이상의 둥근면으로 갈거");
	m_sNoteArr.Add("나 또는 0.5mm 정도의 모따기를 한다");
	m_sNoteArr.Add(".주부재의 홈용접과 필렛용접의 양단에는 반드시 엔드텝을");
	m_sNoteArr.Add("설치하여야 한다");
	m_sNoteArr.Add(".강교 운송중 손상의 우려가 있는 곳은 목재 혹은 앵글등으로");
	m_sNoteArr.Add("견고하게 포장하여 부재가 파손되지 않도록 유의 하여야 한다");
	m_sNoteArr.Add(".강교 운반시 운반 대차의 구조및 싣기방법에 유의하고, 운반");
	m_sNoteArr.Add("도중손상이 가지않도록 하여야 한다");
	m_sNoteArr.Add(".단면도의 치수중 높이는 상판 하면과 하판 상면 기준임");
	m_sNoteArr.Add(".모든 시험, 측정설비 및 장비는 공인기관의 교정검사를");
	m_sNoteArr.Add("필하여야하며 교정검사 필증은 설비, 장비에 부착하고 성적서");
	m_sNoteArr.Add("를 감독원에게 제출하여여 한다");
	m_sNoteArr.Add(".모든 치수는 mm단위이며, 15%%dC 기준이므로 강재 제작시 온");
	m_sNoteArr.Add("도보정을 행하여야한다");
	m_sNoteArr.Add(".볼트 접합되는 현장 이음부 접촉면의 경우 녹이나 흙피는");
	m_sNoteArr.Add("블라스팅등으로 제거하여야하며 전처리 도료나 녹막이 도료를");
	m_sNoteArr.Add("도장하지 않아야 한다");
	m_sNoteArr.Add(".SHOP DRAWING 승인시 설계도상의 치수와 SHOP DRAWING");
	m_sNoteArr.Add("상의 치수를 반드시 확인 대조하고 용접이음의 형식에");
	m_sNoteArr.Add("대해서도 검토하여야 한다");
	m_sNoteArr.Add(".가설 방법에 따라 마지막 작업에서 부재 폐합 오차가 발생할");
	m_sNoteArr.Add("수 있으므로 가설 계획시 충분히 검토하여야 한다");
	m_sNoteArr.Add(".지점 및 단부 DIAPHRAGM은 종경사의 영향을 고려하여 중력");
	m_sNoteArr.Add("방향으로 설치하여야 한다");
	m_sNoteArr.Add(".보강재의 용접");
	m_sNoteArr.Add("1) 수직 보강재와 압축 플랜지, 수직 보강재와 바닥판이 접하는");
	m_sNoteArr.Add("   인장 플랜지는 용접하여야 한다");
	m_sNoteArr.Add("2) 거세트 연결부와 인장 플랜지는 용접하지 않고 밀착 시킨다");
	m_sNoteArr.Add("3) 지점부의 수직 보강재는 플랜지와 용접하여야 한다");
	m_sNoteArr.Add("4) 곡선교의 경우 모든 수직 보강재는 플랜지와 용접하여야 한다");
	m_sNoteArr.Add(".강재 운반 및 설치시 필요한 LIFTING LUG는 강교 제작자가 위치");
	m_sNoteArr.Add("및 크기를 결정하여 제작시 반영해야 한다");
	m_sNoteArr.Add(".강교 가설시 하중의 불균형, 전도, 선회 등에 특히 주의");
	m_sNoteArr.Add("하여야 한다");
	m_sNoteArr.Add(".강교 공작도 작성은 공사기본도, 공사시방서 및 도로교표준");
	m_sNoteArr.Add("시방서에 언급된 내용에 준하여 수행하되 불일치 사항이나");
	m_sNoteArr.Add("명확하게 정의를 내려야할 사항들은 공작도 작성중에 발주처와");
	m_sNoteArr.Add("협의 및 승인과정을 거쳐 확정 및 제작시 반영되어야 한다");
}

void CAPlateOutVBracing::AddNote(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();

	CDomyun Dom(pDomP);

	DimNoteTypeVertical(&Dom);

	CDRect rect;
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.Move(-rect.left,-rect.top);
	Dom.Move(Dom.Always(650),Dom.Always(200));

	*pDomP << Dom;
}

void CAPlateOutVBracing::DimNoteTypeVertical(CDomyun *pDomP)
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
	Dom.SetColumnWidth(1,Dom.Always(120));
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
	Dom.TextOut(Dom.Always(15), Dom.Always(3.5), "노트 사항");
	*pDomP << Dom;
}
//0번거더의 BX가 넘어옴.
void CAPlateOutVBracing::AddHDanVBracing(CDomyun *pDom, CPlateBasicIndex* pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawDanmyun DrawDanmyun(pBridge->m_pDataManage);
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);
	BOOL bLeft=TRUE;
	if(!pBx) return;
	CPlateBasicIndex* pBxTemp = pBx;
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	if(pStd->GetDomOptionStd()->m_VBracing_bDimension)
		DrawCross.DimVBracingFrontAllGirder(&Dom, pBx);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	long nG = 0;
	for(nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{		
		if(nG<pBridge->GetGirdersu()/2)
			bLeft = FALSE;
		else
			bLeft = TRUE;

		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		CPlateBasicIndex *pBxR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBxTemp, pGirR);
		if(pBxR==NULL) continue;
		
		DrawCross.DrawGirderFront(&Dom, pBxTemp, CAPlateDrawCross::ALL);
		if(nG==pBridge->GetGirdersu()-2)
		{
			DrawCross.DrawGirderFront(&Dom, pBxR, CAPlateDrawCross::ALL);	
			DrawCross.DrawHStiffFront(&Dom, pBxR, bLeft);
		}
		
		if(pBxTemp->GetSection()->GetVBracing() == NULL)
		{
			pBxTemp = pBxR;
			continue;
		}

		DrawCross.DrawVBracingSpliceOnCRBx(&Dom,pBxTemp, FALSE);
		DrawCross.DrawVBracingBeamOnCRBx(&Dom, pBxTemp, TRUE);
		DrawCross.DrawHStiffFront(&Dom, pBxTemp, bLeft);		
		DrawCross.DrawVBracingAnchorBar(&Dom, pBxTemp);

		DimHDanVBracingJewonText(&Dom, pBxTemp);
		
		pBxTemp = pBxR;
	}	
	//수평브레이싱 단면도
	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		CPlateBasicIndex *pBxR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
		DrawCross.DrawFrontHGussetSplice(&Dom, pBx, FALSE);
		pBx = pBxR;
	}
	*pDom << Dom;
}


//0번거더의 pBx
void CAPlateOutVBracing::AddHDanVBracingSangse(CDomyun *pDom, CPlateBasicIndex* pBx, long nPos)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pStd->GetDataManage());

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
//	DrawCross.DrawVBracingDetail(&Dom, pBx, nPos);
	CPlateGirderApp  *pGirBefoEnd = pBridge->GetGirder(pBridge->GetGirdersu()-2);
	CPlateBasicIndex *pBxLast     = pBx->GetBxMatchByCrossBeam(pGirBefoEnd);
	CVBracing        *pVr         = pBx->GetSection()->GetVBracing();
	CVBracing        *pVrLast     = pBxLast->GetSection()->GetVBracing();
	
	if(nPos==1 || nPos==2 || nPos==5)
	{		
		DrawCross.DrawVBracingBeamOnCRBx(&Dom, pBx, FALSE);
		DrawCross.DrawCRBeamTotalFrontOnBx(&Dom, pBx, TRUE);
		if(nPos==1 || nPos==2) DrawCross.DrawHStiffFront(&Dom, pBx, FALSE);		
	}
	else
	{
		DrawCross.DrawVBracingBeamOnCRBx(&Dom, pBxLast, FALSE);
		DrawCross.DrawCRBeamTotalFrontOnBx(&Dom, pBxLast, TRUE);

		CPlateGirderApp  *pGirEnd = pBridge->GetGirder(pBridge->GetGirdersu()-1);
		CPlateBasicIndex *pBxEnd  = pBx->GetBxMatchByCrossBeam(pGirEnd);
		DrawCross.DrawHStiffFront(&Dom, pBxEnd, TRUE);		
	}

	if(pVr==NULL && pVrLast==NULL)
		return;

	double dRadius = 500;
	CDPoint xyCen, xyCen1, xyCen2, xyCen3, xyCen4, xyCen5;
	if(pVr)
	{
		xyCen1 = pVr->GetPointFrame(CVBracing::LT);
		xyCen2 = pVr->GetPointFrame(CVBracing::LD);
		if(pVr->GetType()==TYPE_VBRACING)
			xyCen5 = (pVr->GetPointFrame(CVBracing::LD)+pVr->GetPointFrame(CVBracing::RD))/2;
		else
			xyCen5 = (pVr->GetPointFrame(CVBracing::LT)+pVr->GetPointFrame(CVBracing::RT))/2;
	}
	if(pVrLast)
	{
		xyCen3 = pVrLast->GetPointFrame(CVBracing::RT);
		xyCen4 = pVrLast->GetPointFrame(CVBracing::RD);
	}

	if	   (nPos==1 && pVr)		xyCen = xyCen1;
	else if(nPos==2 && pVr)		xyCen = xyCen2;
	else if(nPos==3 && pVrLast)	xyCen = xyCen3;
	else if(nPos==4 && pVrLast)	xyCen = xyCen4;
	else if(nPos==5 && pVr)		xyCen = xyCen5;
	else return;

	Dom.TrimByCircle(xyCen, dRadius, TRUE);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);	
	Dom.Circle(xyCen, dRadius);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	if(pStd->GetDomOptionStd()->m_VBracing_bDimension)
		DrawCross.DimVBracingDetail(&Dom, pBx, nPos);	

	*pDom << Dom;	
}

void CAPlateOutVBracing::AddHDanVBracingDanmyunAA(CDomyun *pDom, CPlateBasicIndex* pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pStd->GetDataManage());
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCross.DrawVBracingDetailAA(&Dom, pBx, pStd->GetDomOptionStd()->m_VBracing_bDimension);

	*pDom << Dom;	
}
void CAPlateOutVBracing::AddHDanVBracingDanmyunBB(CDomyun *pDom, CPlateBasicIndex* pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pStd->GetDataManage());
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCross.DrawVBracingDetailBB(&Dom, pBx, pStd->GetDomOptionStd()->m_VBracing_bDimension);

	*pDom << Dom;	
}
void CAPlateOutVBracing::DimHDanVBracingJewonText(CDomyun *pDom, CPlateBasicIndex* pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CVBracing* pVr        = pBx->GetSection()->GetVBracing();
	if(pVr == NULL) return;
	
	CDPoint A[10], B[10], C[10], vX(1,0), vY(0,1);
	pGir->GetHunchXyDom(pBx, A);
	pDB->GetGirder(0)->GetHunchXyDom(pBx, B);
	pDB->GetGirder(pDB->GetGirdersu()-2)->GetHunchXyDom(pBx, C);

	CDomyun	Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(pDom->GetScaleDim());
	//제원스트링 시작위치
	double dHGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation()); 	
	CDPoint ptPos(0,0);
	ptPos.y = B[2].y>C[7].y ? C[7].y : B[2].y;
	ptPos.y -= dHGir+Dom.Always(15); 
	ptPos.x = A[2].x;
	//상현재
	CString szJewon="";
	szJewon.Format("1-L %s", pGir->GetStringVBracingBeamJewon(pBx, 0));
	double dTextWidth = Dom.GetTextWidth(szJewon);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, szJewon);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//상부이음판
	CVector vJ    = pGir->GetJewonTotal(VB_GU, pBx, 0, 0);
	szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	CString str = "";
	str.Format("2-Guss.PL %s", szJewon);
	dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//하부이음판
	vJ    = pGir->GetJewonTotal(VB_GU, pBx, 0, 2);
	szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	str.Format("2-Guss.PL %s", szJewon);
	dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//중앙부이음판
	vJ    = pGir->GetJewonTotal(VB_GU, pBx, 0, 4);
	szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	str.Format("1-Guss.PL %s", szJewon);
	dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//사재(좌측)
	szJewon.Format("1-L %s", pGir->GetStringVBracingBeamJewon(pBx, 2));
	dTextWidth = Dom.GetTextWidth(szJewon);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, szJewon);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//사재(우측)
	szJewon.Format("1-L %s", pGir->GetStringVBracingBeamJewon(pBx, 3));
	dTextWidth = Dom.GetTextWidth(szJewon);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, szJewon);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//하현재
	szJewon.Format("1-L %s", pGir->GetStringVBracingBeamJewon(pBx, 1));
	dTextWidth = Dom.GetTextWidth(szJewon);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, szJewon);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//볼트
	long nQty = (pVr->m_uB+1)*2 + (pVr->m_dB+1)*2;
	vJ    = pGir->GetJewonTotal(VB_BOLT,pBx);
	CString sMark = "H.T.B";
	szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	str.Format("%d-%s M%.0fx%.0f(%s)",nQty,sMark,vJ.y,vJ.z,pDB->GetMaterial(VB_BOLT, pBx));
	dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);

	*pDom << Dom;
}


void CAPlateOutVBracing::DrawDetailAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd* pStd   = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt  = pStd->GetDataManage()->GetOptionStd();
	CAPlateDrawEtc DrawEtc(pStd->GetDataManage());	
	if(pBx->GetSection()->GetVBracing()->m_nAnchorSu <= 0) return;
	double AnchorDia = pBx->GetGirder()->m_pSangse->m_SlabAnchor_T;

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	DrawEtc.AddAnchorBarDetailVBracing(&Dom, pBx, Dom.GetScaleDim(), AnchorDia);

	*pDom << Dom;
}

void CAPlateOutVBracing::AddKeyPan(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxCr)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint*	pMove		= pStd->GetMovePoint(pStd->GetCurPage());	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CDomyun Dom(pDom);	
	
	long nSize = pDomOpt->m_VBracing_Scale.GetSize();
	double dKeyScaleHor = pDomOpt->m_VBracing_Scale.GetAt(nSize-2);
	double dKeyScaleVer = pDomOpt->m_VBracing_Scale.GetAt(nSize-1);

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	BOOL bSpliceMark = pDomOpt->m_VBracing_KeyPlanSpliceMark;
	BOOL bJijumMark	 = pDomOpt->m_VBracing_KeyPlanJijumMark;
	BOOL bCross		 = pDomOpt->m_VBracing_KeyPlanCrossBeam;
	BOOL bDimension	 = pDomOpt->m_VBracing_KeyPlanDimension;
	BOOL bDirection	 = pDomOpt->m_VBracing_KeyPlanDirection;
	BOOL bOneLine	 = pDomOpt->m_VBracing_KeyPlanSpliceMark;
	// 진행방향
	CString sBridgeDir	= pDomOpt->m_sTextBridgeDirection;
	CString sCounterDir = pDomOpt->m_sTextCounterDirection;

	Dom.SetScaleDim(dKeyScaleHor);

	CPlateBasicIndex *pBxStt = NULL;
	CPlateBasicIndex *pBxEnd = NULL;
	// KEY PLAN 그리기
	DrawPyung.AddKeyPlan(&Dom,pBxStt,pBxEnd,dKeyScaleHor,FALSE, bSpliceMark,bJijumMark, bDirection,FALSE,bCross,bOneLine,-1,-1,FALSE,FALSE,sBridgeDir,sCounterDir);	

	// HATCH 넣기
	pOpt->SetEnvType(&Dom, HCAD_DIML);
	CPlateBasicIndex *pBxMatch;
	CDPoint po[4];
	double hWid = 300;
	for(long nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateGirderApp *pGirMatch = (CPlateGirderApp *)pGir->m_pRight;

		pBxMatch = pBxCr->GetBxMatchByCrossBeam(pGirMatch);
		if(pBxMatch)
		{
			CDPoint pt1 = pGir->GetXyGirderDis(pBxCr->GetStation());
			CDPoint pt2 = pGirMatch->GetXyGirderDis(pBxMatch->GetStation());
			if(nG==0)
			{
				CDPoint ptU = (pt2-pt1).Unit().Rotate90();
				po[0] = pt1-ptU*hWid;
				po[1] = pt1+ptU*hWid;
			}
			if(nG==pBridge->GetGirdersu()-2)
			{
				CDPoint ptU = (pt2-pt1).Unit().Rotate90();
				po[2] = pt2-ptU*hWid;
				po[3] = pt2+ptU*hWid;
			}
		}
	}
	Dom.LineTo(po[0], po[1]);
	Dom.LineTo(po[2], po[3]);
	Dom.LineTo(po[0], po[2]);
	Dom.LineTo(po[1], po[3]);
	Dom.HatchRect(po[0].x,po[0].y,po[1].x,po[1].y,po[3].x,po[3].y,po[2].x,po[2].y,"ANSI31");

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
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER);
	CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();

	ASSERT( pBx || pBxNext);
	double rotAng = DrawPyung.GetRotateAnglePyung(pBx,pBxNext);
	CDPoint MoveXY = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	Dom.Move(-MoveXY);	// 도로중심	 (0,0)
	Dom.Rotate(rotAng,0,0);	

	BOOL bRotate = pDomOpt->m_VBracing_KeyPlanRoVer;
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
	Dom.RedrawByScale(GetScale()/dKeyScaleHor);	
	*pDom << Dom;
	
}

void CAPlateOutVBracing::AddDetailBM(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	CDataManage		*pDataManage	= pStd->GetDataManage();
	COptionStd		*pDomOpt		= pStd->GetDomOptionStd();
	CBMCreator		*pBMStd			= pDataManage->GetBMStandard();
	CGlobarOption	*pGlopt			= pDataManage->GetGlobalOption();
	CPlateBridgeApp *pDB            = pDataManage->GetBridge();

	double dScale = GetScale();
	CGridDomyunEx Dom(pDom);
	Dom.SetScaleDim(dScale);

	CPtrList pList;
	CPtrList pBxList;

	AhPtrListDelete(pBMStd->m_BMCrossBeamList,(CBMRecord*)0);

	for(long k = 0; k < pDB->GetGirdersu()-1; k++)
	{
		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(k));
		if(pBxMatch)
		{
			pBxList.AddTail(pBxMatch);
		}
		else	break;

		pBMStd->GetBMListVBracing(&pList, pBxMatch);
		POSITION pos = pList.GetHeadPosition();
		for(long i=0; i<pList.GetCount(); i++)
		{
			CBMRecord *pRec = (CBMRecord*)pList.GetNext(pos);
			if(pRec->m_nQty>0)
				pBMStd->m_BMCrossBeamList.AddTail(pRec);
		}
		pList.RemoveAll();
	}
	pBMStd->MakeListCrossBeamJewon();
	long nCount = pBMStd->m_BMCrossBeamList.GetCount();

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);

	double dTitleWidth[] = {Dom.Always(35), Dom.Always(18), Dom.Always(18), Dom.Always(18),  
						    Dom.Always(18), Dom.Always(22), Dom.Always(24), Dom.Always(22)};
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER; 
	long nCols  = 8;
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetColumnCount(nCols);	
	Dom.SetRowCount(pBMStd->m_BMCrossBeamList.GetCount()+2);
	double dHeight = 8.0;

	Dom.SetRowHeightAll(Dom.Always(dHeight));
	long i = 0;
	for(i=0; i<nCols; i++)
		Dom.SetColumnWidth(i,dTitleWidth[i]);
	Dom.SetMargin(3);
	Dom.SetRowHeight(0,Dom.Always(dHeight*1.5));
	Dom.SetTextMatrix(0, 0, "부재명", nAlign);
	Dom.SetTextMatrix(0, 1, "너비\n(mm)", nAlign);		
	Dom.SetTextMatrix(0, 2, "두께\n(mm)", nAlign);		
	Dom.SetTextMatrix(0, 3, "길이\n(mm)", nAlign);	
	Dom.SetTextMatrix(0, 4, "수량\n(개)", nAlign);		
	Dom.SetTextMatrix(0, 5, "단위중량\n(kgf/m3)", nAlign);
	Dom.SetTextMatrix(0, 6, "총중량\n(kgf)", nAlign);
	Dom.SetTextMatrix(0, 7, "재질", nAlign);
	long nRoundTot   = pGlopt->GetPointValueOfGangJae();			///< 총중량 강재 반올림 자리수
	long nRoundUnit  = pGlopt->GetPointValueOfUnitGangJae();		///< 단위중량 강재 반올림 자리수

	POSITION pos = pBMStd->m_BMCrossBeamList.GetHeadPosition();
	double dTotalSum = 0;

	for(i=0; i<pBMStd->m_BMCrossBeamList.GetCount(); i++)
	{
		CBMRecord *pRec = (CBMRecord *)pBMStd->m_BMCrossBeamList.GetNext(pos);
		Dom.SetTextMatrix(i+1, 0, pRec->GetStrSubEleName(), GRID_ALIGN_LEFT   | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 1, pRec->GetStrWidth(),		GRID_ALIGN_RIGHT  | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 2, pRec->GetStrThick(),		GRID_ALIGN_RIGHT  | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 3, pRec->GetStrLength(),		GRID_ALIGN_RIGHT  | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 4, pRec->GetStrQty(),		GRID_ALIGN_RIGHT  | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 5, pRec->GetStrUWeight(nRoundUnit),		GRID_ALIGN_RIGHT  | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 6, pRec->GetStrTWeight(FALSE, nRoundTot),GRID_ALIGN_RIGHT  | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 7, pRec->GetStrMaterial(),	GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER);
		dTotalSum += pRec->m_dTWeight;
	}
	CString str;
	str = GetStringOutValue(dTotalSum,nRoundTot);
	Dom.SetTextMatrix(nCount+1,0, "소 계", GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(nCount+1,6, str,GRID_ALIGN_RIGHT| GRID_ALIGN_VCENTER);
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;
}

void CAPlateOutVBracing::AddVBracingPyungTotalDanmyun(CDomyun *pDom, CPlateBasicIndex* pBx,	BOOL bUpper)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawPyung DrawPyung(pBridge->m_pDataManage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);

	double dGirL = 500;

	if(!pBx) return;

	CDPoint xyStt	= pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	CDPoint vGir	= pBx->GetGirder()->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double	dX		= pBridge->GetGirder(0)->m_dGirderCenter;	
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	DrawPyung.DrawVBracingBeamDimAllDanmyun(&Dom, pBx,bUpper);
	
	Dom.Move(xyStt.x-dX, xyStt.y);
	double dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
	Dom.Rotate(-dRotate, xyStt.x, xyStt.y);
	*pDom << Dom;

	CPlateBasicIndex* pBxGir = pBx;
	vGir = pBridge->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
	long nG = 0;
	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{		
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		if(pGir==NULL || pGirR==NULL) break;

		CPlateBasicIndex *pBxGirR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBxGir, pGirR);
		if(pBxGir==NULL || pBxGirR==NULL) break;

		DrawPyung.DrawGirderWithenStation(&Dom, pBxGir, pBxGir->GetStation()-dGirL, pBxGir->GetStation()+dGirL, bUpper, TRUE); 
		pBxGir = pBxGirR;
	}	

	for(nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{		
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		if(pGir==NULL || pGirR==NULL) break;

		CPlateBasicIndex *pBxR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
		if(pBx==NULL || pBxR==NULL) break;

		DrawPyung.DrawVBracingFlangeOnBx(&Dom, pBx, TRUE,bUpper);
		DrawPyung.DrawVBracingAnchorBarOnBx(&Dom, pBx);
		DrawPyung.DrawVBracingCenterSplice(&Dom, pBx, FALSE);
		DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBx, TRUE, FALSE);
		DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBx, FALSE, FALSE);

		//지점부 수직보강재 or 거더 복부 수직보강재
		if(pBx->IsJijum())	DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx, bUpper, FALSE);
		else				DrawPyung.DrawVStiffOnBx(&Dom, pBx, FALSE, bUpper);

		if(pBxR->IsJijum())	DrawPyung.DrawJiJumStiffOnBx(&Dom, pBxR, bUpper, FALSE);
		else				DrawPyung.DrawVStiffOnBx(&Dom, pBxR, FALSE, bUpper);
		
		pBx = pBxR;
	}

	*pDom << Dom;
}
