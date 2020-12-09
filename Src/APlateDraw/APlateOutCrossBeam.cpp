// APlateOutCrossBeam.cpp: implementation of the CAPlateOutCrossBeam class.
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
#define MPE_CROSS_FRONT			10
#define MPE_CROSS_UPPLAN		20
#define MPE_CROSS_BRUPDETAIL	30
#define MPE_CROSS_BRDNDETAIL	40 
#define MPE_CROSS_HSTIFFDETAIL	50
#define MPE_CROSS_VSTIFFDETAIL	60
#define MPE_CROSS_SLABANCHOR	70
#define MPE_CROSS_KEYPLAN		80
#define MPE_CROSS_BKSUPPORT		90
#define MPE_CROSS_SCALLOPEXT	100
#define MPE_CROSS_TABLE			110
#define MPE_CROSS_TABLETITLE	120
#define MPE_CROSS_MARK			130
#define MPE_CROSS_WEBSPLICE     180
#define MPE_CROSS_BKCOUPLE      190

#define MPE_CROSS_TOTAL_PLAN_UP	140
#define MPE_CROSS_TOTAL_PLAN_LO 150
#define MPE_CROSS_TOTAL_FRONT   160
#define MPE_CROSS_TOTAL_SANGSE  170
#define MPE_CROSS_TOTAL_SANGSE_GA 200
#define MPE_CROSS_KEY_PAN		 210

#define MPE_CROSS_POSITION		 220
#define MPE_CROSS_DIMTABLE		 220


CAPlateOutCrossBeam::CAPlateOutCrossBeam()
{

}

CAPlateOutCrossBeam::~CAPlateOutCrossBeam()
{

}

void CAPlateOutCrossBeam::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB		= pStd->GetDataManage()->GetBridge();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng    *pPageMng	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();

	long nPage    = pStd->m_nCurPage;//현재 전체 페이지수
	long nprePage = m_nSttPage;		// 일반도이전의 도면번호
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();
	
	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	
	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
	//pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sDomNumHead,"",nSttPage);
	pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pDB->m_sNameBridge, GetTitle(), _T(""),FALSE);
	pStd->AddMaterial(&Dom,Scale);
	pStd->AddJubujebubuje(&Dom,Scale);
	Dom.SetCalcExtRect();
	CDRect dgkRect = Dom.GetExtRect(), DomBound;
	*pDom << Dom;			

	BOOL bSummary = pDomOpt->m_Cross_Summary && (pDomOpt->m_Cross_Pyung || pDomOpt->m_Cross_Table);
	

	if(bSummary && nSubPage==1)
	{
		AddPageSummaryTable(&Dom);
		*pDom << Dom;
		return;
	}

	BOOL bTwoDrawing = pDomOpt->m_Cross_TwoDrawing;
	long nType = 0;//0-1장으로 출력...1-두장으로 출력인데 첫장..2-두장으로 츨력인데 2번째짱...
	long nCrBx = 0;
	if(bSummary)
	{
		nCrBx = nSubPage-2;	
		if(bTwoDrawing)//두장씩 입력이면..
		{
			if(nSubPage%2==0)
			{
				nType = 1;
				nCrBx = (nSubPage/2)-1;
			}
			else
			{
				nType = 2;
				nCrBx = ((nSubPage-1)/2)-1;
			}
		}						
	}
	else
	{
		nCrBx = nSubPage-1;
		if(bTwoDrawing)
		{
			if(nSubPage%2==1)
			{
				nType = 1;
				nCrBx = ((nSubPage+1)/2)-1;
			}
			else
			{
				nType = 2;
				nCrBx = ((nSubPage)/2)-1;
			}
		}	
	}

	if(pDomOpt->m_Cross_Type==0)
	{
//		EXIT_RTN;
		CPlateCrossBeam	*pC = m_CrossList.GetAt(nCrBx);
		CPlateBasicIndex *pBx = pC->GetBx();
		AddCrossBeamOneType(&Dom, pBx, nType);
		*pDom << Dom;
	}
	else 
	{
//		EXIT_RTN;
		CPlateCrossBeam	*pC = m_CrossListAtFirstGirder.GetAt(nCrBx);
		CPlateBasicIndex *pBx = pC->GetBx();
		AddCrossBeamTotalType(&Dom, pBx, nType);
		*pDom << Dom;
	}
}
void CAPlateOutCrossBeam::AddCrossBeamTotalType(CDomyun *pDom, CPlateBasicIndex *pBx, long nType)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	CDrawPageMng *pPageMng	  = pStd->GetDrawPageMng();	
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateCrossBeam	*pC		  = pBx->GetSection()->GetCrossBeam();

	double FrontScale			= pDomOpt->m_CrossBeam_Scale.GetAt(0);		//정면도
	double PlanUpperScale		= pDomOpt->m_CrossBeam_Scale.GetAt(1);
	double PlanLowerScale		= pDomOpt->m_CrossBeam_Scale.GetAt(2);
	double SangseScale          = pDomOpt->m_CrossBeam_Scale.GetAt(3);
	double SangseScaleGusset    = pDomOpt->m_CrossBeam_Scale.GetAt(5);
	double GassetUpperScale		= pDomOpt->m_CrossBeam_Scale.GetAt(6);
	double GassetLowerScale		= pDomOpt->m_CrossBeam_Scale.GetAt(7);
	double VStiffScale			= pDomOpt->m_CrossBeam_Scale.GetAt(9);
	double AnchorBarScale		= pDomOpt->m_CrossBeam_Scale.GetAt(11);
	double BKSupportScale		= pDomOpt->m_CrossBeam_Scale.GetAt(12);	

	CDRect DomBound;
	CDomyun	Dom(pDom);
	pDom->SetCalcExtRect();
	CDRect	DgkBound = pDom->GetExtRect();

	long n = 1;
	CDPoint xy;
	CDPoint xyMove;
	*pDom << Dom;

	
	BOOL bFirstDraw		= FALSE;
	BOOL bSecondDraw	= FALSE;

	if(nType == 0)//한장으로 그리는 거면..
	{
		bFirstDraw	= TRUE;
		bSecondDraw = TRUE;
	}
	else if(nType == 1)//첫번째면
		bFirstDraw = TRUE;
	else//두번째면...
		bSecondDraw = TRUE;
	
	CDPoint xySangSttLeft(0,0), xySangSttRight(0,0), xyBrkStt(0,0), xyBrkSupStt(0,0), vX(1,0), vY(0,1); 
	xyMove = CDPoint(Dom.Always(30), DgkBound.Height()-Dom.Always(80));
	*pDom << Dom;
	
	if(pDomOpt->m_Cross_PlanUpper && bFirstDraw)
	{
//		EXIT_RTN;
		CDPoint vGir = pBridge->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
		CDPoint xyOrigin = pBridge->GetGirder(0)->GetXyGirderDis(pBx->GetStation());

		pPageMng->SetProgressData("평면도(상부) 출력중 ...",30);
		Dom.SetScaleDim(PlanUpperScale);
		
		Dom.SetScaleDim(pDom->GetScaleDim());
		AddCrossBeamPyungAtTopTotalDanmyun(&Dom, pBx);	

		Dom.Rotate(-vGir.GetAngleRadian() + CDPoint(0,1).GetAngleRadian(), xyOrigin.x, xyOrigin.y);
		Dom.RedrawByScale(GetScale()/PlanUpperScale);//확대

		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xySangSttLeft = xyMove + vX*(rect.Width()+Dom.Always(20));
		xyMove.y -= rect.Height()+Dom.Always(40);
		
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_PLAN_UP));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_PLAN_UP,"평면도(상부)");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, PlanUpperScale, Dom.GetExtRect(), "평면도(상부)", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_PLAN_UP+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_PLAN_UP+1,"평면도(상부) 제목");
		*pDom << Dom;
		//MPE추가종료
	}
	if(pDomOpt->m_Cross_Front && bFirstDraw)
	{
//		EXIT_RTN;
		
		pPageMng->SetProgressData("정면도 출력중 ...",30);
		Dom.SetScaleDim(FrontScale);
		AddCrossBeamFrontAtTotalDanmyun(&Dom, pBx);	
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/FrontScale);//확대

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyMove.y -= rect.Height()+Dom.Always(40); 
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_FRONT));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_FRONT,"정면도");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, FrontScale, Dom.GetExtRect(), "정면도", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_FRONT+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_FRONT+1,"정면도 제목");
		*pDom << Dom;
		//MPE추가종료
	}

	if(pDomOpt->m_Cross_PlanLower && bFirstDraw)
	{
//		EXIT_RTN;
		CDPoint vGir = pBridge->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
		CDPoint xyOrigin = pBridge->GetGirder(0)->GetXyGirderDis(pBx->GetStation());
	
		pPageMng->SetProgressData("평면도(하부) 출력중 ...",30);
		Dom.SetScaleDim(PlanLowerScale);
		AddCrossBeamPyungAtBottomTotalDanmyun(&Dom, pBx);		
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.Rotate(-vGir.GetAngleRadian() + CDPoint(0,1).GetAngleRadian(), xyOrigin.x, xyOrigin.y);
		Dom.RedrawByScale(GetScale()/PlanLowerScale);//확대

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_PLAN_LO));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_PLAN_LO,"평면도(하부)");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, PlanLowerScale, Dom.GetExtRect(), "평면도(하부)", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_PLAN_LO+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_PLAN_LO+1,"평면도(하부) 제목");
		*pDom << Dom;
		//MPE추가종료
	}

	if(nType == 2) xySangSttLeft = xyMove;//두장씩 그리기면...			

	if(pDomOpt->m_Cross_Sangse && bSecondDraw)
	{
//		EXIT_RTN;
		CString str = "";
		pPageMng->SetProgressData("상세도 출력중 ...",30);
		//0번거더의 좌우측의 이음 형태가 동일할 때
		long nSangse = 1;
		//거더이음상세 좌측//////////////////////////////////////
		Dom.SetScaleDim(SangseScale);
		AddCrossBeamSangseOne(&Dom, pBx);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/SangseScale);//확대

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		
		Dom.Move(-rect.left, -rect.top - rect.Height());				
		Dom.Move(xySangSttLeft);
		if(!pC->m_cGasset_CR)
			xyBrkSupStt = xySangSttLeft - vY*(rect.Height()+Dom.Always(60));
		xySangSttRight = xySangSttLeft + vX*(rect.Width()+Dom.Always(20));
		xySangSttLeft.y -= rect.Height()+Dom.Always(40); 

		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE+5));
		Dom.SetCalcExtRect();
		str.Format("상세도 %d", nSangse);
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE+5, str);
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, SangseScale, Dom.GetExtRect(), str, TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE+6));
		Dom.SetCalcExtRect();
		str.Format("상세도 %d 제목", nSangse);
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE+6, str);
		//MPE추가종료
		*pDom << Dom;
		nSangse++;
		//가셋트이음상세 좌측//////////////////////////////////////
		if(pC->m_cGasset_CR)
		{
			Dom.SetScaleDim(SangseScaleGusset);
			AddCrossBeamSangseOneGusset(&Dom, pBx);	
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.RedrawByScale(GetScale()/SangseScaleGusset);//확대

			Dom.SetCalcExtRect();	
			CDRect rect = Dom.GetExtRect();
			Dom.Move(-rect.left, -rect.top - rect.Height()); 	
			Dom.Move(xySangSttLeft);
			xyBrkSupStt = xySangSttLeft - vY*(rect.Height()+Dom.Always(40));
			//MPE추가시작
			Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE_GA+5));
			Dom.SetCalcExtRect();	
			str.Format("상세도 %d", nSangse);
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE_GA+5, str);
			*pDom << Dom;
			// TITLE
			DrawStd.AddTitleOnDom(&Dom, SangseScaleGusset, Dom.GetExtRect(), str, TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE_GA+6));
			Dom.SetCalcExtRect();
			str.Format("상세도 %d 제목", nSangse);
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE_GA+6, str);
			//MPE추가종료
			*pDom << Dom;
			nSangse++;
		}
		//거더이음상세 우측//////////////////////////////////////
		Dom.SetScaleDim(SangseScale);
		AddCrossBeamSangseTwo(&Dom, pBx);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/SangseScale);//확대

		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.top - rect.Height()); 	
		Dom.Move(xySangSttRight);
		xyBrkStt = xySangSttRight + vX*(rect.Width()+Dom.Always(20));
		xySangSttRight.y -= rect.Height()+Dom.Always(40); 

		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE+7));
		Dom.SetCalcExtRect();
		str.Format("상세도 %d", nSangse);
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE+7, str);
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, SangseScale, Dom.GetExtRect(), str, TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE+8));
		Dom.SetCalcExtRect();
		str.Format("상세도 %d 제목", nSangse);
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE+8, str);
		//MPE추가종료
		*pDom << Dom;
		nSangse++;
		//가셋트이음상세 우측//////////////////////////////////////
		if(pC->m_cGasset_CR)
		{
			Dom.SetScaleDim(SangseScaleGusset);
			AddCrossBeamSangseTwoGusset(&Dom, pBx);	
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.RedrawByScale(GetScale()/SangseScaleGusset);//확대

			Dom.SetCalcExtRect();	
			CDRect rect = Dom.GetExtRect();
			Dom.Move(-rect.left, -rect.top - rect.Height()); 	
			Dom.Move(xySangSttRight);
			//MPE추가시작
			Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE_GA+7));
			Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
			str.Format("상세도 %d", nSangse);
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE_GA+7, str);
			*pDom << Dom;
			// TITLE
			DrawStd.AddTitleOnDom(&Dom, SangseScaleGusset, Dom.GetExtRect(), str, TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_CROSS_TOTAL_SANGSE_GA+8));
			Dom.SetCalcExtRect();
			str.Format("상세도 %d 제목", nSangse);
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TOTAL_SANGSE_GA+8, str);
			//MPE추가종료
			*pDom << Dom;
			nSangse++;
		}
	}

	if((pC->m_nLGirSpType || pC->m_nRGirSpType)  && bSecondDraw)
	{
		if(pDomOpt->m_Cross_GassetUpperDetail)
		{
			pPageMng->SetProgressData("브라켓 상부 상세도 출력중...",80/7*n++);
			Dom.SetScaleDim(GassetUpperScale);
			DrawDetailCrossGasset(&Dom, pBx, TRUE);
			Dom.SetScaleDim(pDom->GetScaleDim());
			Dom.RedrawByScale(GetScale()/GassetUpperScale);

			Dom.SetCalcExtRect();
			CDRect rect = Dom.GetExtRect();
			Dom.Move(-rect.left, -rect.top-rect.Height()); 	
			Dom.Move(xyBrkStt);
			xyBrkStt.y -= Dom.Always(40)+rect.Height();
		
			DomBound = Dom.GetExtRect();
			Dom.Move(pMove->GetPoint(MPE_CROSS_BRUPDETAIL));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRUPDETAIL,"브라켓 상부 상세");
			*pDom << Dom;
			// TITLE
			Dom.SetScaleDim(GetScale());
			DrawStd.AddTitleOnDom(&Dom,GassetUpperScale,DomBound,"브라켓 상부 상세",TRUE,TRUE,0);
			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_CROSS_BRUPDETAIL+1));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRUPDETAIL+1,"브라켓 상부 상세 제목");
			*pDom << Dom;
		}

		if (pDomOpt->m_Cross_GassetLowerDetail)
		{
			pPageMng->SetProgressData("브라켓 하부 상세 출력중...",80/7*n++);
			Dom.SetScaleDim(GassetLowerScale);
			DrawDetailCrossGasset(&Dom, pBx, FALSE);
			Dom.SetScaleDim(GetScale());
			Dom.RedrawByScale(GetScale()/GassetLowerScale);

			Dom.SetCalcExtRect();
			CDRect rect = Dom.GetExtRect();
			Dom.Move(-rect.left, -rect.top - rect.Height()); 	
			Dom.Move(xyBrkStt);
			//MPE추가시작
			DomBound = Dom.GetExtRect();
			Dom.Move(pMove->GetPoint(MPE_CROSS_BRDNDETAIL));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRDNDETAIL,"브라켓 하부 상세");
			*pDom << Dom;
			// TITLE
			Dom.SetScaleDim(GetScale());
			DrawStd.AddTitleOnDom(&Dom,GassetLowerScale,DomBound,"브라켓 하부 상세",TRUE,TRUE,0);
			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_CROSS_BRDNDETAIL+1));
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRDNDETAIL+1,"브라켓 하부 상세 제목");
			*pDom << Dom;
		}
	}

	if (pDomOpt->m_Cross_VStiffDetail && pC->m_nVStiffsu_CR != 0  && bSecondDraw)
	{
		pPageMng->SetProgressData("수직 보강재 상세 출력중...",80/7*n++);
		Dom.SetScaleDim(VStiffScale);
		DrawDetailCrossVStiff(&Dom, pBx);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/VStiffScale);//확대
		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		Dom.Move(xyBrkSupStt);
		xyBrkSupStt += vX*(rect.Width()+Dom.Always(20));
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_VSTIFFDETAIL));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_VSTIFFDETAIL, "수직 보강재 상세");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, VStiffScale, Dom.GetExtRect(), "수직 보강재 상세", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_VSTIFFDETAIL+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_VSTIFFDETAIL+1, "수직 보강재 상세 제목");
		//MPE추가종료
		*pDom << Dom;
	}

	if(pDomOpt->m_Cross_AnchorBarDetail && pBx->GetSection()->GetCrossBeam()->m_nAnchorsu_CR > 0  && bSecondDraw)
	{
		pPageMng->SetProgressData("슬래브앵커 상세 출력중...",80/7*n++);
		Dom.SetScaleDim(AnchorBarScale);
		DrawDetailAnchorBar(&Dom, pBx);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/AnchorBarScale);//확대

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		Dom.Move(xyBrkSupStt);
		xyBrkSupStt += vX*(rect.Width()+Dom.Always(20));
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_SLABANCHOR));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_SLABANCHOR, "슬래브앵커 상세");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, AnchorBarScale, Dom.GetExtRect(), "슬래브앵커 상세", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_SLABANCHOR+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_SLABANCHOR+1, "슬래브앵커 상세 제목");
		//MPE추가종료
		*pDom << Dom;

	}

	if(pDomOpt->m_Cross_BKSupport && pC->m_cGasset_CR  && bSecondDraw)
	{
		pPageMng->SetProgressData("브라켓 보강판 상세 출력중...",80/7*n++);
		//좌측....................................................
		Dom.SetScaleDim(BKSupportScale);
		DrawDetailBKSupport(&Dom, pBx, TRUE);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/BKSupportScale);//확대

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		Dom.Move(xyBrkSupStt);
		xyBrkSupStt += vX*(rect.Width()+Dom.Always(20));
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT, "보강판 상세(좌측)");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, BKSupportScale, Dom.GetExtRect(), "보강판 상세(좌측)", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT+1, "보강판 상세 제목");
		//MPE추가종료
		*pDom << Dom;
		//우측.................................................
		xyMove = CDPoint(900, 450);
		Dom.SetScaleDim(BKSupportScale);
		DrawDetailBKSupport(&Dom, pBx, FALSE);
		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.RedrawByScale(GetScale()/BKSupportScale);//확대

		Dom.SetCalcExtRect();	
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		Dom.Move(xyBrkSupStt);
		xyBrkSupStt += vX*(rect.Width()+Dom.Always(20));
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT+2));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT+2, "보강판 상세(우측)");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, BKSupportScale, Dom.GetExtRect(), "보강판 상세(우측)", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT+3));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT+3, "보강판 상세 제목");
		//MPE추가종료
		*pDom << Dom;
	}

	if(pDomOpt->m_Cross_KeyPlan  && bSecondDraw)
	{
//		EXIT_RTN;
		long nSize = pDomOpt->m_CrossBeam_Scale.GetSize();
		double dHScale = pDomOpt->m_CrossBeam_Scale.GetAt(nSize-2);

		pPageMng->SetProgressData("요약도 출력중...",99);						
		AddKeyPan(&Dom,0,0, pBx);
		Dom.SetScaleDim(pDom->GetScaleDim());

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		Dom.Move(xyBrkSupStt);
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_KEY_PAN));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_KEY_PAN, "요약도");
		*pDom << Dom;
		// TITLE
		rect = Dom.GetExtRect();
		//rect.bottom -= Dom.Always(30);
		DrawStd.AddTitleOnDom(&Dom, dHScale, rect, "요약도", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_KEY_PAN+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_KEY_PAN+1, "요약도 제목");
		//MPE추가종료
		*pDom << Dom;
	}

	if (pDomOpt->m_Cross_BM  && bSecondDraw)
	{
		pPageMng->SetProgressData("재료표 출력중...",90);
		AddDetailBMTotType(&Dom, pBx);
		Dom.SetCalcExtRect();
		CDRect	rect = Dom.GetExtRect();
		CDPoint xy;
		xy.x = DgkBound.Width() - rect.Width() - Dom.Always(20);// + Dom.Always(20);
		xy.y = DgkBound.Height() - Dom.Always(400);
		Dom.Move(xy.x,xy.y);
		Dom.Move(pMove->GetPoint(MPE_CROSS_TABLE));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TABLE,"재료표");
		*pDom << Dom;
	}

	pPageMng->SetProgressData("출력완료",100);
}

long CAPlateOutCrossBeam::GetPageSu()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng    *pPageMng	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();

	if(pDomOpt->m_Cross_Type==0)
		MakeCrossBeamList();
	else
		MakeCrossBeamListAtFirstGirder();

	BOOL bSummary  = pDomOpt->m_Cross_Summary && (pDomOpt->m_Cross_Pyung || pDomOpt->m_Cross_Table);
	long nCrossSu = pDomOpt->m_Cross_Type==0 ? m_CrossList.GetSize() : m_CrossListAtFirstGirder.GetSize();
	if (nCrossSu == 0)
	{
		m_nPage = 0;
		return m_nPage;
	}
	else
	{
		m_nPage = nCrossSu;

		//두장으로 출력이면 ...
		if(pDomOpt->m_Cross_TwoDrawing) m_nPage *=2;

		if(bSummary)	
			m_nPage++;
		return m_nPage;
	}
}

double CAPlateOutCrossBeam::GetScale()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt = pStd->m_pDomOptionStd;

	double Scale  = pOpt->m_CrossBeam_Scale.GetAt(0);
	return Scale;
}

CString CAPlateOutCrossBeam::GetTitle()
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CString szTitle;

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;
	long nSubPage = nPage - nprePage;
	long nPageSu = m_nPage;

	BOOL bSummary = pDomOpt->m_Cross_Summary && (pDomOpt->m_Cross_Pyung || pDomOpt->m_Cross_Table);

	if(nSubPage==1 && bSummary)
		szTitle.Format("가로보 치수표 (%d/%d)",nSubPage, nPageSu);
	else
	{
		//long nIdx = bSummary ? nSubPage-1 : nSubPage ;
		szTitle.Format("가로보 상세도 (%d/%d)",nSubPage, nPageSu);
	}

	return szTitle;
}

void CAPlateOutCrossBeam::AddPageSummaryTable(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawStandard	DrawStd(pStd->GetDataManage());	
	CPlateBridgeApp		*pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();	
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd			*pDomOpt	= pStd->GetDomOptionStd();

	CDRect DomBound;
	CDomyun	Dom(pDom);
	pDom->SetCalcExtRect();
	CDRect	DgkBound = pDom->GetExtRect();
	
	CDPoint xy;
	CDPoint xyMove;
	*pDom << Dom;
	
	CDPoint xySangSttLeft(0,0), xySangSttRight(0,0), xyBrkStt(0,0), xyBrkSupStt(0,0), vX(1,0), vY(0,1); 
	xyMove = CDPoint(Dom.Always(80), DgkBound.Height()-Dom.Always(80));
	*pDom << Dom;


	if(pDomOpt->m_Cross_Pyung)
	{
//		EXIT_RTN;			
		pPageMng->SetProgressData("위치도 출력중...",40);						
		AddKeyPan(&Dom,0,0, NULL, TRUE);

		Dom.SetScaleDim(pDom->GetScaleDim());
		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		Dom.Move(pDom->Always(100),pDom->Always(450));
		//MPE추가시작
		Dom.Move(pMove->GetPoint(MPE_CROSS_POSITION));
		Dom.SetCalcExtRect();	//도면의 이동으로 인한 영역 재계산
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_POSITION, "위치도");
		*pDom << Dom;
		// TITLE
		rect = Dom.GetExtRect();
		//rect.bottom -= Dom.Always(20);
		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_Cross_dPosHScale, rect, "위치도", TRUE,TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_POSITION+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_POSITION+1, "위치도 제목");
		//MPE추가종료
		*pDom << Dom;			
	}

	if(pDomOpt->m_Cross_Table)
	{
		pPageMng->SetProgressData("치수테이블 출력중...",70);		
		DrawSummaryTable(pDom);
	}
}

void CAPlateOutCrossBeam::DrawSummaryTable(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawStandard	DrawStd(pStd->GetDataManage());	
	CPlateBridgeApp		*pDB	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();	
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd			*pDomOpt	= pStd->GetDomOptionStd();	

	// 가로보 DATA

	CStringArray sArr[6];
	CString str = _T("");	
	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++) 
	{		
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);		
		int nCount = 1;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			ASSERT(pSec);
			if(pSec)
			{
				if(pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
				{
					str.Format("CR%d",nG+1);
					sArr[0].Add(str);

					if(nCount < 10)
						str.Format("%dCR0%d", nG+1,nCount);
					else
						str.Format("%dCR%d", nG+1,nCount);

					sArr[1].Add(str);
					
					double staGir		= pBx->GetStation();
					double staCR		= pGir->GetStationCenLineForEl(staGir, pGir->m_dGirderCenter);
					double disToCenLine = pGir->GetDisToCenterLineForExcel(staGir, pGir->m_dGirderCenter);
					double dPyunCenLine = pDB->GetGirder(-1)->GetLine()->GetPyungubae(staCR, disToCenLine);
					str.Format("%.2f %%",dPyunCenLine*100);
					sArr[2].Add(str);

					str.Format("%.2f %%",pGir->GetGiulCrossBody(pBx,TRUE)*100);
					sArr[3].Add(str);
					str.Format("%.2f",pGir->GetLengthCrossTaper(pBx, DIM_BASIC));
					sArr[4].Add(str);
					str.Format("%.2f",pGir->GetHeightCrossTaper(pBx));
					sArr[5].Add(str);
					//가로보 경사(%) = H/W * 100
				}
			}
			nCount++;
			pBx = Finder.GetBxNext();
		}	
	}

	long nSize       = sArr[0].GetSize();
	long nTableRowSu = pDomOpt->m_Cross_nRowSu;
	long nTableSu    = (nSize%nTableRowSu) ? nSize/nTableRowSu+1 : nSize/nTableRowSu;
	double Scale = GetScale();

	long nDataCount = 0;
	long cnt        = 0;
	long cntComp	= 0;
	CDPoint Base(0,Scale*250);
	long nT = 0;
	for(nT=0; nT<nTableSu; nT++)
	{
		CGridDomyunEx Dom(pDom);
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		Dom.SetScaleDim(Scale);

		long   nColSu  = 6;
		long   nRowSu  = min(nSize-nTableRowSu*nT,nTableRowSu)+1;
		double dHeightRow = Dom.Always(8);
		double UpperTitleWidth[] =  { Dom.Always(20), Dom.Always(20), Dom.Always(30), Dom.Always(30),
									  Dom.Always(30), Dom.Always(30)};
		Base.x = Dom.Always(150) + nT*Dom.Always(190);
		Dom.SetBasePoint(Base);
		Dom.SetColumnCount(nColSu);
		Dom.SetRowCount(nRowSu);
		Dom.SetRowHeightAll(dHeightRow);
		long i = 0;
		for(i=0; i<nColSu; i++)
			Dom.SetColumnWidth(i,UpperTitleWidth[i]);
		//테이블 합치기
		Dom.SetMergeCol(0,0,1);
		Dom.SetMargin(3);
		//테이블 제목 쓰기
		Dom.SetTextMatrix(0,0,"구 분");
		Dom.SetTextMatrix(0,2,"편경사(%)");
		Dom.SetTextMatrix(0,3,"가로보경사(%)");
		Dom.SetTextMatrix(0,4,"W");
		Dom.SetTextMatrix(0,5,"H");

		// DATA
		long rowMin = 1;
		long rowMax = 1;
		for(i=1; i<nRowSu; i++)
		{	
			if(cnt>=nSize) break;
			if(sArr[0].GetAt(cnt)==sArr[0].GetAt(cntComp))
			{
				if(i==nRowSu-1)
				{
					Dom.SetMergeRow(rowMin, nRowSu-1, 0);
					Dom.SetTextMatrix(rowMin, 0, sArr[0].GetAt(cntComp));
				}
				cntComp = cnt;
				cnt++;
				continue;
			}
			else
			{
				rowMax = i-1;
				Dom.SetMergeRow(rowMin, rowMax, 0);
				Dom.SetTextMatrix(rowMin, 0, sArr[0].GetAt(cntComp));
				rowMin = i;
				cnt++;
				cntComp = cnt;
				if(i==nRowSu-1)
				{
					Dom.SetMergeRow(rowMin, nRowSu-1, 0);
					Dom.SetTextMatrix(rowMin, 0, sArr[0].GetAt(cntComp));
				}
			}
		}

		for(long nRow=1; nRow<nRowSu; nRow++)
		{
			for(long j=1; j<6; j++)
				Dom.SetTextMatrix(nRow,j,sArr[j].GetAt(nDataCount));
			nDataCount++;
		}
		
		Dom.Draw(FALSE);
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
		Dom.DrawOutLine();

		Dom.SetCalcExtRect ();
		Dom.Move(pMove->GetPoint(MPE_CROSS_TABLE+nT));

		CString str;
		str.Format("치수테이블 %d", nT+1);
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_CROSS_TABLE+nT, str);
		*pDom << Dom;
	}	
	
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);
	long   n = min(nSize,nTableRowSu)+1;
	CDRect rect(0,0,0,0);
	rect.left = Scale*150;
	rect.bottom = Scale*250;
	rect.top  = rect.bottom - n*Scale*8;
	rect.right = rect.left+(nT-1)*Scale*190 + Scale*150;
	DrawStd.AddTitleOnDom(&Dom, 0, rect, "치 수 테 이 블", TRUE,FALSE, 0);
	Dom.Move(pMove->GetPoint(MPE_CROSS_TABLETITLE));
	Dom.SetCalcExtRect();
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TABLETITLE,"치수테이블 제목");
	*pDom << Dom;
}

void CAPlateOutCrossBeam::AddCrossBeamOneType(CDomyun *pDom, CPlateBasicIndex *pBx, long nType)
{ 
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	CDrawPageMng *pPageMng	  = pStd->GetDrawPageMng();	
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateCrossBeam	*pC		  = pBx->GetSection()->GetCrossBeam();

	double FrontScale			= pDomOpt->m_CrossBeam_Scale.GetAt(0);		//정면도
	double PlanScale			= pDomOpt->m_CrossBeam_Scale.GetAt(1);
	double GassetUpperScale		= pDomOpt->m_CrossBeam_Scale.GetAt(5);
	double GassetLowerScale		= pDomOpt->m_CrossBeam_Scale.GetAt(6);
	double VStiffScale			= pDomOpt->m_CrossBeam_Scale.GetAt(8);
	double HStiffScale			= pDomOpt->m_CrossBeam_Scale.GetAt(9);
	double AnchorBarScale		= pDomOpt->m_CrossBeam_Scale.GetAt(10);
	double BKSupportScale		= pDomOpt->m_CrossBeam_Scale.GetAt(11);

	BOOL bFirstDraw = FALSE;
	BOOL bSecondDraw = FALSE;
	
	if(nType == 0)//한장으로 그리는 거면..
	{
		bFirstDraw = TRUE;
		bSecondDraw = TRUE;
	}
	else if(nType == 1)//첫번째면
		bFirstDraw = TRUE;
	else//두번째면...
		bSecondDraw = TRUE;

	CDomyun	Dom(pDom);
	CDRect rect;
	double Scale  = GetScale();
	pDom->SetCalcExtRect();
	CDRect	DgkBound = pDom->GetExtRect();
	long n = 1;
	CDPoint xy;

	CDPoint xyMove = CDPoint(Dom.Always(100), DgkBound.Height()-Dom.Always(100));
	CDPoint xyBrk, xyBrk1, xyBrk2, xyBrk3;


	if(pDomOpt->m_Cross_Plan && bFirstDraw)
	{
		pPageMng->SetProgressData("평면도 출력중...",80/7*n++);
		Dom.SetScaleDim(PlanScale);
		DrawPlanCrossMain(&Dom, pBx);
		Dom.RedrawByScale(Scale/PlanScale);

		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyBrk = xyMove;
		xyBrk.x += rect.Width()+Dom.Always(50);
		xyBrk1 = xyBrk;
		xyMove.y -= rect.Height()+Dom.Always(50);

		Dom.Move(pMove->GetPoint(MPE_CROSS_UPPLAN));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_UPPLAN,"평면도");
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom,PlanScale,rect,"평 면 도",TRUE,TRUE,0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_UPPLAN+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_UPPLAN+1,"평면도 제목");
		*pDom << Dom;
	}
	if(pDomOpt->m_Cross_Front && bFirstDraw)
	{
		pPageMng->SetProgressData("정면도 출력중...",80/7*n++);		
		Dom.SetScaleDim(FrontScale);
		DrawFrontCrossMain(&Dom, pBx);
		Dom.RedrawByScale(Scale/FrontScale);

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xyMove);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	

		Dom.Move(pMove->GetPoint(MPE_CROSS_FRONT));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,rect,MPE_CROSS_FRONT,"정면도");		
		*pDom << Dom;	
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, FrontScale, rect, "정 면 도",TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_FRONT+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_FRONT+1,"정면도 제목");
		*pDom << Dom;
	}

	if(nType == 2) xyBrk = xyMove;//두장씩 그리기면...	
	//가로보와 거더의 연결타입이 볼트이음이면 그리지 않는다.
	if(((pC->m_nLGirSpType && pC->m_nRGirSpType)||
	   (!pC->m_nLGirSpType && pC->m_nRGirSpType)||
	   (pC->m_nLGirSpType && !pC->m_nRGirSpType)) && bSecondDraw)
	{
		if (pDomOpt->m_Cross_GassetUpperDetail)
		{
			pPageMng->SetProgressData("브라켓 상부 상세도 출력중...",80/7*n++);
			Dom.SetScaleDim(GassetUpperScale);
			DrawDetailCrossGasset(&Dom, pBx, TRUE);
			Dom.RedrawByScale(Scale/GassetUpperScale);

			Dom.SetCalcExtRect();	
			CDRect rect = Dom.GetExtRect();
			Dom.Move(xyBrk);
			Dom.Move(-rect.left, -rect.top-rect.Height()); 	
			xyBrk2 = xyBrk;
			xyBrk2.x += rect.Width()+Dom.Always(50);
			xyBrk.y -= rect.Height()+Dom.Always(70);
			
			Dom.Move(pMove->GetPoint(MPE_CROSS_BRUPDETAIL));
			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRUPDETAIL,"브라켓 상부 상세");
			*pDom << Dom;
			// TITLE
			Dom.SetScaleDim(Scale);
			DrawStd.AddTitleOnDom(&Dom,PlanScale,rect,"브라켓 상부 상세",TRUE,TRUE,0);
			Dom.Move(pMove->GetPoint(MPE_CROSS_BRUPDETAIL+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRUPDETAIL+1,"브라켓 상부 상세 제목");
			*pDom << Dom;
		}

		if (pDomOpt->m_Cross_GassetLowerDetail)
		{
			pPageMng->SetProgressData("브라켓 하부 상세도 출력중...",80/7*n++);
			Dom.SetScaleDim(GassetLowerScale);
			DrawDetailCrossGasset(&Dom, pBx, FALSE);
			Dom.RedrawByScale(Scale/GassetLowerScale);

			Dom.SetCalcExtRect();	
			CDRect rect = Dom.GetExtRect();
			Dom.Move(xyBrk);
			Dom.Move(-rect.left, -rect.top-rect.Height()); 	
			xyBrk.y -= rect.Height()+Dom.Always(70);

			Dom.Move(pMove->GetPoint(MPE_CROSS_BRDNDETAIL));
			Dom.SetCalcExtRect();
			rect = Dom.GetExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRDNDETAIL,"브라켓 하부 상세");
			*pDom << Dom;
			// TITLE
			Dom.SetScaleDim(Scale);
			DrawStd.AddTitleOnDom(&Dom,PlanScale,rect,"브라켓 하부 상세",TRUE,TRUE,0);
			Dom.Move(pMove->GetPoint(MPE_CROSS_BRDNDETAIL+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BRDNDETAIL+1,"브라켓 하부 상세 제목");
			*pDom << Dom;
		}
	}

	if(pDomOpt->m_Cross_AnchorBarDetail && pBx->GetSection()->GetCrossBeam()->m_cGasset_CR && bSecondDraw)
	{
		//좌측//////////////////////////////////////////////////////////////////////////
		pPageMng->SetProgressData("보강판 이음부 상세(좌측) 출력중...",80/7*n++);
		Dom.SetScaleDim(BKSupportScale);
		AddCrossBeamSangseOneGusset(&Dom, pBx);
		Dom.RedrawByScale(Scale/BKSupportScale);

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		CDPoint xyStt;
		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			xyStt = xyBrk1;
		else
			xyStt = xyBrk2;
		Dom.Move(xyStt);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xyStt.y -= rect.Height()+Dom.Always(50);
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKCOUPLE));
		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKCOUPLE,"보강판 이음부 상세(좌측)");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(Scale);
		DrawStd.AddTitleOnDom(&Dom,BKSupportScale,rect,"보강판 이음부 상세(좌측)",TRUE,TRUE,0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKCOUPLE+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKCOUPLE+1,"보강판 이음부 상세(좌측) 제목");
		*pDom << Dom;
		//우측//////////////////////////////////////////////////////////////////////////
		pPageMng->SetProgressData("보강판 이음부 상세(우측) 출력중...",80/7*n++);
		Dom.SetScaleDim(BKSupportScale);
		AddCrossBeamSangseTwoGusset(&Dom, pBx);
		Dom.RedrawByScale(Scale/BKSupportScale);

		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();
		Dom.Move(xyStt);
		xyBrk3 = xyStt;
		xyBrk3.y -= rect.Height()+Dom.Always(50); 
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	

		Dom.Move(pMove->GetPoint(MPE_CROSS_BKCOUPLE+2));
		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKCOUPLE+2,"보강판 이음부 상세(우측)");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(Scale);
		DrawStd.AddTitleOnDom(&Dom,BKSupportScale,rect,"보강판 이음부 상세(우측)",TRUE,TRUE,0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKCOUPLE+3));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKCOUPLE+3,"보강판 이음부 상세(우측) 제목");
		*pDom << Dom;
	}

	CDPoint xySttV;
	if (pDomOpt->m_Cross_VStiffDetail && pC->m_nVStiffsu_CR != 0 && bSecondDraw)
	{
		pPageMng->SetProgressData("수직 보강재 상세 출력중...",80/7*n++);
		Dom.SetScaleDim(VStiffScale);
		DrawDetailCrossVStiff(&Dom, pBx);
		Dom.RedrawByScale(Scale/VStiffScale);

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		if((!pC->m_nLGirSpType && !pC->m_nRGirSpType) && !pC->m_cGasset_CR)
			xySttV = xyBrk1;
		else if((!pC->m_nLGirSpType && !pC->m_nRGirSpType) && pC->m_cGasset_CR)
			xySttV = xyBrk3;
		else
			xySttV = xyBrk;
		Dom.Move(xySttV);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xySttV.x += rect.Width()+Dom.Always(70);

		Dom.Move(pMove->GetPoint(MPE_CROSS_VSTIFFDETAIL));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_VSTIFFDETAIL,"수직 보강재 상세");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(Scale);
		DrawStd.AddTitleOnDom(&Dom,HStiffScale,rect,"수직 보강재 상세",TRUE,TRUE,0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_VSTIFFDETAIL+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_VSTIFFDETAIL+1,"수직 보강재 상세 제목");
		*pDom << Dom;
	}

	if(pDomOpt->m_Cross_BKSupport && pC->m_cGasset_CR && bSecondDraw)
	{
		pPageMng->SetProgressData("브라켓 보강판 상세 출력중...",80/7*n++);
		//좌측
		Dom.SetScaleDim(BKSupportScale);
		DrawDetailBKSupport(&Dom, pBx, TRUE);
		Dom.RedrawByScale(Scale/BKSupportScale);

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xySttV);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xySttV.x += rect.Width()+Dom.Always(70);

		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT));
		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT,"브라켓 보강판 상세(좌측)");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(Scale);
		DrawStd.AddTitleOnDom(&Dom,BKSupportScale,rect,"브라켓 보강판 상세(좌측)",TRUE,TRUE,0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT+1,"브라켓 보강판 상세 제목");
		*pDom << Dom;
		//우측
		Dom.SetScaleDim(BKSupportScale);
		DrawDetailBKSupport(&Dom, pBx, FALSE);
		Dom.RedrawByScale(Scale/BKSupportScale);

		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();
		Dom.Move(xySttV);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		xySttV.x += rect.Width()+Dom.Always(70);

		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT+2));
		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT+2,"브라켓 보강판 상세(우측)");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(Scale);
		DrawStd.AddTitleOnDom(&Dom,BKSupportScale,rect,"브라켓 보강판 상세(우측)",TRUE,TRUE,0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_BKSUPPORT+3));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_BKSUPPORT+3,"브라켓 보강판 상세 제목");
		*pDom << Dom;
	}

	if(pDomOpt->m_Cross_AnchorBarDetail && pBx->GetSection()->GetCrossBeam()->m_nAnchorsu_CR > 0 && bSecondDraw)
	{
		pPageMng->SetProgressData("슬래브앵커 상세 출력중...",80/7*n++);
		Dom.SetScaleDim(AnchorBarScale);
		DrawDetailAnchorBar(&Dom, pBx);
		Dom.RedrawByScale(Scale/AnchorBarScale);

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(xySttV);
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	

		Dom.Move(pMove->GetPoint(MPE_CROSS_SLABANCHOR));
		Dom.SetCalcExtRect();	
		rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_SLABANCHOR,"슬래브앵커 상세");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(Scale);
		DrawStd.AddTitleOnDom(&Dom,AnchorBarScale,rect,"슬래브앵커 상세",TRUE,TRUE,0);
		Dom.Move(pMove->GetPoint(MPE_CROSS_SLABANCHOR+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_SLABANCHOR+1,"슬래브앵커 상세 제목");
		*pDom << Dom;
	}


	if(pDomOpt->m_Cross_GassetScallopDetail)
	{
/*		pPageMng->SetProgressData("브라켓 스켈럽 상세 출력중...",80/7*n++);
		Dom.SetScaleDim(BKScallopScale);
		DrawDetailCrossBKScallop(&Dom, pBx);
		Dom.RedrawByScale(Scale/BKScallopScale);
		Dom.SetCalcExtRect();
		xy.x = DgkBound.right - Dom.Always(100);
		xy.y = DgkBound.top + Dom.Always(150);
		Dom.Move(xy.x,xy.y);
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_CROSS_SCALLOPEXT));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_SCALLOPEXT,"브라켓 스켈럽 상세");
		*pDom << Dom;
		// TITLE
		Dom.SetScaleDim(Scale);
		DrawStd.AddTitleOnDom(&Dom,BKScallopScale,rect,"브라켓 스켈럽 상세",TRUE,TRUE,0);
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(MPE_CROSS_SCALLOPEXT+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_SCALLOPEXT+1,"브라켓 스켈럽 상세 제목");
		*pDom << Dom;*/
	}

	if (pDomOpt->m_Cross_BM && bSecondDraw)
	{
		pPageMng->SetProgressData("재료표 출력중...",90);
		AddDetailBMOneType(&Dom, pBx);
		Dom.SetCalcExtRect();
		CDRect	rect = Dom.GetExtRect();
		CDPoint xy;
		xy.x = DgkBound.Width() - rect.Width() - Dom.Always(20);
		xy.y = DgkBound.Height() - Dom.Always(60);
		Dom.Move(xy.x,xy.y);
		Dom.Move(pMove->GetPoint(MPE_CROSS_TABLE));
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_CROSS_TABLE,"재료표");
		*pDom << Dom;
	}
}

void CAPlateOutCrossBeam::AddCrossBeamSangseOne(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	if(!pBx) return;

	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateCrossBeam * pC         = pBx->GetSection()->GetCrossBeam();

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);

	CLongArray nArrGir;
	pBridge->GetIndexGirderToCrossBeam(nArrGir, pBx, TRUE);
	long nSize = nArrGir.GetSize();
	if(nSize == 0) return;

	long nSttGir = nArrGir.GetAt(0) == 0? 0 : nArrGir.GetAt(0);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);

	CPlateGirderApp* pGir = pBridge->GetGirder(nSttGir);
	DrawCross.DrawGirderFront(&Dom, pBx, CAPlateDrawCross::ALL);			
	
	DrawCross.DrawHStiffFront(&Dom, pBx, TRUE);
	DrawCross.DrawHStiffFront(&Dom, pBx, FALSE);
	DrawCross.DrawFrontHGussetSplice(&Dom, pBx, /*bDim*/FALSE);

	if(pC->m_nLGirSpType)// && pC->GetType()!=CR_TYPE_VSTIFF)
	{
		DrawCross.DrawCRBeamFrontGassetPlate_FlangeBx(&Dom, pBx, TRUE);
		DrawCross.DrawCRBeamFrontGassetPlate_WebLeftBx(&Dom, pBx, TRUE, TRUE, TRUE);
		DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);
		DrawCross.DrawCRBeamSpliceOnBx(&Dom, pBx, TRUE, FALSE);
		if(pC->m_cGasset_CR)
			DrawCross.DrawCRBeamFrontGassetSupportPlateLeftBx(&Dom, pBx);
	}
	else
	{
		DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);
		if(pC->m_cGasset_CR)
			DrawCross.DrawCRBeamFrontGassetSupportPlateByBoltLeftBx(&Dom, pBx);
	}

	CDPoint A[10], B[10];
	pGir->GetHunchXyDom(pBx, A);
	pC->GetCrossXyDom(B);

	CDPoint vX(1,0), vY(0,1), Stt, End;
	double dTWeb = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dExt = 100;
	CDPoint vDirBrk = (B[3]-B[2]).Unit();
	
	double dSpace = pGir->GetWidthFlangeUpper(pBx);	// 이음판과 브라켓보강판이 없을때
	if(pC->m_nLGirSpType)							// 이음판 있을때
//		dSpace = dTWeb/2 + pC->m_P_CR + pC->m_sA+pC->m_sB*pC->m_sC+pC->m_sC1 + 100;
		dSpace = dTWeb/2 + pC->m_P_CR + pC->m_uA+pC->m_uB*pC->m_uC+pC->m_uC1/2+100;
	else if(pC->m_cGasset_CR)						// 이음판이 없고 브라켓보강판이 있을경우
		dSpace = dTWeb/2 + pC->m_P_CR + 100;
	
	CDPoint xySupUp = pC->m_nLGirSpType ? pC->GetPointGassetSupPlateEndBx(pBx, CDPoint(0,1), TRUE, TRUE) 
										: pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), TRUE, TRUE);
	CDPoint xySupLo = pC->m_nLGirSpType ? pC->GetPointGassetSupPlateEndBx(pBx, CDPoint(0,1), FALSE, TRUE)
										: pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), FALSE, TRUE);

	Stt = (A[1]+A[2])/2 + vDirBrk*dSpace + vY*(dExt - pC->m_dLH1);
	GetXyMatchLineAndLine(xySupUp, vDirBrk, Stt, vY, End);

	End -= vY*dExt;
	if(pC->m_cGasset_CR == FALSE)
		End = CDPoint(Stt.x, B[2].y - pC->m_H_CR - pC->m_T1_CR - dExt);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);

	double dEraseX = pGir->m_dGirderCenter + pGir->GetDisGirderToGirder(pBx)/3 + pBridge->m_BindConc.m_dWidth/2 + Dom.Always(5);
	CDPoint xyTrim = CDPoint(dEraseX, (Stt.y+End.y)/2);



	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	CDRect rectErase(xyTrim.x, xyTrim.y+rectDom.Height()/2, xyTrim.x+rectDom.Width(), xyTrim.y-rectDom.Height()/2);	
	Dom.TrimByLineOnSide(CDPoint(xyTrim.x, xyTrim.y - rectDom.Height()/2), CDPoint(xyTrim.x, xyTrim.y + rectDom.Height()/2), 1);
	Dom.EraseRect(rectErase);
	Dom.CutLightning(xyTrim.x, End.y, xyTrim.x, Stt.y, TRUE, 1); 
	
	//scallop상세1...
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);	
	CDPoint xyCen;
	CDPoint vDir;
	double dRad      = pC->m_dwR*3;
	double dWVStiffR = pBx->GetSection()->GetCrossBeam()->m_pVStiff->m_dWidth[1];
	double dR        = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_R);
	double dB		 = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
	double dH		 = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	int    nID		 = pC->m_nLGirSpType ? 0:1;
	double dS1		 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S1);
//	double dS2 = pC->GetGussetPlateJewon(CPlateCrossBeam::S2);
//	double dHGusset = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
	double dSharp = pGir->GetLengthSharpOnBx(pBx,  TRUE);

	if(!pC->m_nLGirSpType)
	{
		CDomyun Dom1(&Dom);
		Dom1 += Dom;
		vDir = pC->GetVectorCross();
		xyCen = (A[1]+A[2])/2 - vY*(pC->m_dLH1+pC->m_T2_CR) + vDir*(dWVStiffR+dTWeb/2+pC->m_dbL4)/vDir.x;
		if(pC->m_dLH1==0)
			xyCen = A[2] + vDir*(dWVStiffR+pC->m_dbL4-dSharp)/vDir.x;

		Dom1.TrimByCircle(xyCen, dRad, TRUE);
		pStd->m_pOptStd->SetEnvType(&Dom1,HCAD_DIML);
		Dom1.Circle(xyCen, dRad);
		//치수선
		if(pStd->GetDomOptionStd()->m_Cross_bDimension)
		{
			Dom1.SetScaleDim(Dom.GetScaleDim()/2);
			Dom1.SetDirection("TOP");
			Dom1.DimMoveTo(xyCen.x, xyCen.y, 0);
			Dom1.DimLineTo(pC->m_dwB, 0, COMMA(pC->m_dwB));
			Dom1.SetDirection("LEFT");
			Dom1.DimMoveTo(xyCen.x, xyCen.y-pC->m_dwH, 0);
			Dom1.DimLineTo(pC->m_dwH, 0, COMMA(pC->m_dwH));
			//
			CString str="";
			Dom1.SetDirection("TOP");
			str.Format("R=%d", (long)pC->m_dwR);
			CDPoint vDir = pC->GetVectorCross();
			CDPoint xy = xyCen+vDir*(pC->m_dwB-pC->m_dwR)+CDPoint(1,-1).Unit()*pC->m_dwR;
			Dom1.GiSiArrowExt(xy.x, xy.y, Dom.Always(8), Dom.Always(8), 6, str);
		}
		Dom1.RedrawByScale(2);	
		//
		Dom1.SetCalcExtRect();
		CDRect rect = Dom1.GetExtRect();
		Dom1.Move(-rect.CenterPoint().x, -rect.CenterPoint().y); 	
		CDPoint xyCircleCen = (A[1]+A[2])/2 + vX*(dSpace+rect.Width()+Dom.Always(2));//+dRad*5) + vY*(pC->m_dLH1+pC->m_T2_CR);
		Dom1.Move(xyCircleCen.x, xyCircleCen.y);
		//
		*pDom << Dom1;
		//scallop상세2...
		if(pC->m_cGasset_CR)
		{
			CDomyun Dom2(&Dom);
			Dom2 += Dom;
			xyCen = (A[1]+A[2])/2 - vY*(pC->m_dLH1+pC->m_T2_CR+pC->m_H_CR+pC->m_T1_CR) + vDir*(dWVStiffR+dTWeb/2+pC->m_dbL3)/vDir.x;
			if(pC->m_dLH1==0)
				xyCen = A[2] + vDir*(dWVStiffR+pC->m_dbL3-dSharp)/vDir.x - vY*(pC->m_H_CR+pC->m_T1_CR);
			dRad = dR*3;
			Dom2.TrimByCircle(xyCen, dRad, TRUE);
			pStd->m_pOptStd->SetEnvType(&Dom2, HCAD_DIML);	
			Dom2.Circle(xyCen, dRad);
			//치수선
			if(pStd->GetDomOptionStd()->m_Cross_bDimension)
			{
				Dom2.SetScaleDim(Dom.GetScaleDim()/2);
				Dom2.SetDirection("TOP");
				Dom2.DimMoveTo(xyCen.x, xyCen.y, 0);
				Dom2.DimLineTo(dB, 0, COMMA(dB));
				Dom2.SetDirection("LEFT");
				Dom2.DimMoveTo(xyCen.x, xyCen.y-dB, 0);
				Dom2.DimLineTo(dH, 0, COMMA(dH));
				//
				CString str="";
				Dom2.SetDirection("TOP");
				str.Format("R=%d", (long)dR);
				CDPoint vDir = pC->GetVectorCross();
				CDPoint xy = xyCen+vDir*(dB-dR)+CDPoint(1,-1).Unit()*dR;
				Dom2.GiSiArrowExt(xy.x, xy.y, Dom.Always(8), Dom.Always(8), 6, str);
			}
			Dom2.RedrawByScale(2);	
			//
			Dom2.SetCalcExtRect();
			rect = Dom2.GetExtRect();
			Dom2.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()/2); 	
			xyCircleCen = (A[1]+A[2])/2 + vX*(dSpace+dRad*5) - vY*(pC->m_H_CR+pC->m_dLH1+pC->m_T1_CR);
			Dom2.Move(xyCircleCen.x, xyCircleCen.y);
			//
			*pDom << Dom2;
		}
		//scallop상세3...
		if(pC->m_cGasset_CR)
		{
			CDomyun Dom3(&Dom);
			Dom3 += Dom;
			xyCen = pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), FALSE, TRUE);
			dRad = dS1*2;
			Dom3.TrimByCircle(xyCen, dRad, TRUE);
			pStd->m_pOptStd->SetEnvType(&Dom3, HCAD_DIML);	
			Dom3.Circle(xyCen, dRad);
			//치수선
			if(pStd->GetDomOptionStd()->m_Cross_bDimension)
			{
				Dom3.SetScaleDim(Dom.GetScaleDim()/2);
				Dom3.SetDirection("BOTTOM");
				Dom3.DimMoveTo(xyCen.x-dS1, xyCen.y, 0);
				Dom3.DimLineTo(dS1, 0, COMMA(dS1));
//				Dom3.DimLineTo(dGirWLower/2-(dTWeb/2+dWVStiffR+dS1), 0, COMMA(dGirWLower/2-(dTWeb/2+dWVStiffR+dS1)));
			}
			Dom3.RedrawByScale(2);	
			//
			Dom3.SetCalcExtRect();
			rect = Dom3.GetExtRect();
			Dom3.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()/2); 	
			xyCircleCen = (A[1]+A[2])/2 + vX*(dSpace+dRad*5)
					  - vY*(pC->m_H_CR+pC->m_dLH1+pC->m_T1_CR+pC->m_T2_CR+pC->m_dltH+dRad);
			Dom3.Move(xyCircleCen.x, xyCircleCen.y);
			//
			*pDom << Dom3;
		}
	}
	//치수선은 Trim에서 제외함.

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	if(!pC->m_nLGirSpType)
	{
		xyCen = (A[1]+A[2])/2 - vY*(pC->m_dLH1+pC->m_T2_CR) + vDir*(dWVStiffR+dTWeb/2+pC->m_dbL4)/vDir.x;
		if(pC->m_dLH1==0)
			xyCen = A[2] + vDir*(dWVStiffR+pC->m_dbL4-dSharp)/vDir.x;
		Dom.Circle(xyCen, dRad);
		if(pC->m_cGasset_CR)
		{		
			xyCen = (A[1]+A[2])/2 - vY*(pC->m_dLH1+pC->m_T2_CR+pC->m_H_CR+pC->m_T1_CR) + vDir*(dWVStiffR+dTWeb/2+pC->m_dbL3)/vDir.x;
			if(pC->m_dLH1==0)
				xyCen = A[2] + vDir*(dWVStiffR+pC->m_dbL3-dSharp)/vDir.x - vY*(pC->m_H_CR+pC->m_T1_CR);
			Dom.Circle(xyCen, dRad);

			xyCen = pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), FALSE, TRUE);
			Dom.Circle(xyCen, dRad);
		}
	}

	if(pStd->GetDomOptionStd()->m_Cross_bDimension)
		DrawCross.DimCRBeamSangseOne(&Dom, pBx);
	
	*pDom << Dom;
}

void CAPlateOutCrossBeam::AddCrossBeamSangseOneGusset(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	if(!pBx) return;

	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateCrossBeam* pC         = pBx->GetSection()->GetCrossBeam();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CPlateGirderApp* pGir = pBridge->GetGirder(0);

	if(pC->m_nLGirSpType)
	{
		DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);
		DrawCross.DrawCRBeamFrontGassetPlate_FlangeBx(&Dom, pBx, TRUE);
		DrawCross.DrawCRBeamFrontGassetPlate_WebLeftBx(&Dom, pBx, TRUE, TRUE, FALSE);
		if(pC->m_cGasset_CR)
			DrawCross.DrawCRBeamFrontGassetSupportPlateLeftBx(&Dom, pBx);
	}
	else
	{
		DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);
		if(pC->m_cGasset_CR)
			DrawCross.DrawCRBeamFrontGassetSupportPlateByBoltLeftBx(&Dom, pBx);
	}
	DrawCross.DrawCRBeamVStiffnerOnBx(&Dom, pBx);
	CDPoint A[10], B[10];
	pGir->GetHunchXyDom(pBx, A);
	pC->GetCrossXyDom(B);
	CDPoint vX(1,0), vY(0,1);
	double dRad   = 150;//pC->m_P_CR*0.7;
	double dGirW     = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	long nID = pC->m_nLGirSpType ? 0 : 1;//용접0 볼트1
	int nGen = (int)pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_GEN);
	double dS2 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S2);
	double dS3 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S3);
	CDPoint	vDir = pC->GetVectorCross();
	CDPoint xyCen = B[2] - vY*(pC->m_H_CR+pC->m_T2_CR);
	Dom.TrimByCircle(xyCen, dRad, TRUE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);	
	Dom.Circle(xyCen, dRad);
	//치수선
	if(nGen==1)
		DrawCross.DimCRBeamGassetKneeBracing(&Dom, pBx);		//Goh
	if(pStd->GetDomOptionStd()->m_Cross_bDimension)
	{
		if(nGen==0)
		{
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xyCen.x-dS2, xyCen.y-dS3, 0);
			Dom.DimLineTo(dS3, 0, COMMA(dS3));
		}
		Dom.SetDirection("TOP");
		CDPoint xy = xyCen+vY*pC->m_T1_CR-vDir*150;
		Dom.WeldByValue(xy.x, xy.y, 6, 0, FALSE, TRUE);
		xy = xy + vDir*300 - vY*pC->m_T1_CR/2;
		Dom.CircleArrow(xy.x, xy.y, Dom.Always(15), Dom.Always(15), 7, "하부플랜지", "");
	}
	//
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); 	
	CDPoint xyCircleCen = (A[1]+A[2])/2 + CDPoint(1,1).Unit()*dGirW + vX*(pC->m_P_CR - 100);
	Dom.Move(xyCircleCen.x, xyCircleCen.y);
	//
	*pDom << Dom;
}
void CAPlateOutCrossBeam::AddCrossBeamSangseTwoGusset(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	if(!pBx) return;

	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateCrossBeam* pC         = pBx->GetSection()->GetCrossBeam();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CPlateGirderApp* pGir = pBridge->GetGirder(0);
	CPlateGirderApp* pGirR = pBridge->GetGirder(1);
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);
	CPlateCrossBeam* pCR = pBxR->GetSection()->GetCrossBeam();
	if(pC->m_nRGirSpType)
	{
		DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);
		DrawCross.DrawCRBeamFrontGassetPlate_FlangeBx(&Dom, pBx, FALSE);
		DrawCross.DrawCRBeamFrontGassetPlate_WebRightBx(&Dom, pBx, TRUE, TRUE, FALSE);
		if(pC->m_cGasset_CR)
			DrawCross.DrawCRBeamFrontGassetSupportPlateRightBx(&Dom, pBx);
	}
	else
	{
		DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);
		if(pC->m_cGasset_CR)
			DrawCross.DrawCRBeamFrontGassetSupportPlateByBoltRightBx(&Dom, pBx);
	}
	DrawCross.DrawCRBeamVStiffnerOnBx(&Dom, pBx);
	CDPoint A[10], B[10];
	pGir->GetHunchXyDom(pBx, A);
	pC->GetCrossXyDom(B);
	CDPoint vX(1,0), vY(0,1);
	double dRad   = 150;//pC->m_P_CR*0.7;
	double dGirW     = pGirR->GetWidthOnStation(pBxR->GetStation(), TRUE);
//	double dR = pC->GetGussetPlateJewon(CPlateCrossBeam::R);
//	double dB = pC->GetGussetPlateJewon(CPlateCrossBeam::B);
//	double dH = pC->GetGussetPlateJewon(CPlateCrossBeam::H);
//	double dS1 = pC->GetGussetPlateJewon(CPlateCrossBeam::S1);
	long nID = pC->m_nRGirSpType ? 0 : 1;//용접0 볼트1
	double dS2 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S2);
	double dS3 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S3);
	CDPoint	vDir = pC->GetVectorCross();
	CDPoint xyCen = B[5] - vY*(pC->m_H_CR+pC->m_T2_CR);

	Dom.TrimByCircle(xyCen, dRad, TRUE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);	
	Dom.Circle(xyCen, dRad);
	//치수선
	if(pStd->GetDomOptionStd()->m_Cross_bDimension)
	{
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(xyCen.x+dS2, xyCen.y-dS3, 0);
		Dom.DimLineTo(dS3, 0, COMMA(dS3));
		//
		Dom.SetDirection("TOP");
		CDPoint xy = xyCen+vY*pC->m_T1_CR;
		Dom.WeldByValue(xy.x, xy.y, 6, 0, FALSE, TRUE);
		xy = xy + vDir*50 - vY*pC->m_T1_CR/2;
		Dom.CircleArrow(xy.x, xy.y, Dom.Always(15), Dom.Always(15), 7, "하부플랜지", "");
	}
	//
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); 	
	CDPoint xyCircleCen = A[7] + CDPoint(1,1).Unit()*dGirW + vX*dGirW;
	Dom.Move(xyCircleCen.x, xyCircleCen.y);
	//
	*pDom << Dom;
}

void CAPlateOutCrossBeam::AddCrossBeamSangseTwo(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	if(!pBx) return;

	CAPlateDrawStd	 *pStd	 = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd  *pOpt	 = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp  *pBridge= pStd->GetDataManage()->GetBridge();		
	CPlateCrossBeam  *pC    = pBx->GetSection()->GetCrossBeam();

	CLongArray nArrGir;
	pBridge->GetIndexGirderToCrossBeam(nArrGir, pBx, TRUE);
	long nSize = nArrGir.GetSize();
	if(nSize == 0) return;

	BOOL bThreeGir = nSize > 2? TRUE  : FALSE;
	long nStartG = nArrGir.GetAt(0);


	CPlateBasicIndex *pBxR  = pBx->GetBxMatchByCrossBeam(pBridge->GetGirder(nStartG+1));
	CPlateCrossBeam  *pCR   = pBxR->GetSection()->GetCrossBeam();

	

	CPlateGirderApp  *pGir  = pBridge->GetGirder(nStartG);
	CPlateGirderApp  *pGirR = pBridge->GetGirder(nStartG+1);


	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross DrawCross(pBridge->m_pDataManage);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);

	

	DrawCross.DrawGirderFront(&Dom, pBxR, CAPlateDrawCross::ALL);			
	DrawCross.DrawHStiffFront(&Dom, pBxR, TRUE);
	DrawCross.DrawHStiffFront(&Dom, pBxR, FALSE);
	DrawCross.DrawFrontHGussetSplice(&Dom, pBxR, FALSE);
	DrawCross.DrawCRBeamTotalFrontOnBx(&Dom, pBx, FALSE, FALSE, FALSE);
	if(pGirR->m_pRight && pCR && bThreeGir)	//거더가 3개이상 일 경우
		DrawCross.DrawCRBeamTotalFrontOnBx(&Dom, pBxR, FALSE, FALSE, FALSE);

	CDPoint A[10], B[10];
	pGir->GetHunchXyDom(pBx, A);
	pC->GetCrossXyDom(B);

	CDPoint vX(1,0), vY(0,1), Stt, End;
	double dExt  = 100;
	double dWGir = pGirR->GetWidthOnStation(pBxR->GetStation(), TRUE);
	double dTWeb = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);

	CDPoint vDirBrk = (B[5]-B[6]).Unit();
	////////////////////////////////////
	// 좌측절단..
	double dSpace = pGirR->GetWidthFlangeUpper(pBx);	// 이음판과 브라켓보강판이 없을때
	if(pC->m_nRGirSpType)							// 이음판 있을때
		dSpace = dTWeb/2 + pC->m_P_CR + pC->m_uA+pC->m_uB*pC->m_uC+pC->m_uC1/2+100;
	else if(pC->m_cGasset_CR)						// 이음판이 없고 브라켓보강판이 있을경우
		dSpace = dTWeb/2 + pC->m_P_CR + 100;

	CDPoint xySupUp = pC->m_nRGirSpType ? pC->GetPointGassetSupPlateEndBx(pBx, CDPoint(0,1), TRUE, FALSE) 
										: pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), TRUE, FALSE);
	
	Stt = A[7] + vX*dWGir/2 + vDirBrk*dSpace + vY*(dExt - pC->m_dRH1);
	GetXyMatchLineAndLine(xySupUp, vDirBrk, Stt, vY, End);

	End -= vY*dExt;
	if(pC->m_cGasset_CR == FALSE)
		End = CDPoint(Stt.x, B[5].y - pC->m_H_CR - pC->m_T1_CR - dExt);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	//////////////////////////////////////////////////////////////////////////
	double dEraseX = pGirR->m_dGirderCenter - pGir->GetDisGirderToGirder(pBx)/2 - pBridge->m_BindConc.m_dWidth/2 - Dom.Always(5);
	CDPoint xyTrim = CDPoint(dEraseX, (Stt.y+End.y)/2);
	
	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	CDRect rectErase(xyTrim.x-rectDom.Width(), xyTrim.y+rectDom.Height(), xyTrim.x, xyTrim.y-rectDom.Height());	
	Dom.TrimByLineOnSide(CDPoint(xyTrim.x, xyTrim.y - rectDom.Height()), CDPoint(xyTrim.x, xyTrim.y + rectDom.Height()), -1);
	Dom.EraseRect(rectErase);	
	Dom.CutLightning(xyTrim.x, End.y, xyTrim.x, Stt.y, TRUE, 1);
	//////////////////////////////////////////////////////////////////////////
	
	if(pGirR->m_pRight && pCR )
	{
		// 우측절단..
		pGirR->GetHunchXyDom(pBx, A);
		pCR->GetCrossXyDom(B);
		vDirBrk = (B[5]-B[6]).Unit();
		dSpace  = pGirR->GetWidthFlangeUpper(pBx);	// 이음판과 브라켓보강판이 없을때
		if(pCR->m_nLGirSpType)							// 이음판 있을때
			dSpace = dTWeb/2 + pCR->m_P_CR + pCR->m_sA+pCR->m_sB*pCR->m_sC+pCR->m_sC1 + 100;				
		else if(pCR->m_cGasset_CR)						// 이음판이 없고 브라켓보강판이 있을경우
			dSpace = dTWeb/2 + pCR->m_P_CR + 100;

		xySupUp = pCR->m_nLGirSpType ? pCR->GetPointGassetSupPlateEndBx(pBxR, CDPoint(0,1), TRUE, TRUE) 
									 : pCR->GetPointGassetSupPlateByBoltEndBx(pBxR, CDPoint(0,1), TRUE, TRUE);

		Stt = (A[1]+A[2])/2 - vDirBrk*dSpace + vY*(dExt - pCR->m_dLH1);
		GetXyMatchLineAndLine(xySupUp, vDirBrk, Stt, vY, End);

		End -= vY*dExt;
		if(pCR->m_cGasset_CR == FALSE)
			End = CDPoint(Stt.x, B[5].y - pCR->m_H_CR - pCR->m_T1_CR - dExt);

		//////////////////////////////////////////////////////////////////////////
		double dEraseX = pGirR->m_dGirderCenter + pGir->GetDisGirderToGirder(pBx)/2 + pBridge->m_BindConc.m_dWidth/2 + Dom.Always(5);
		CDPoint xyTrim = CDPoint(dEraseX, (Stt.y+End.y)/2);

		Dom.SetCalcExtRect();
		CDRect rectDom = Dom.GetExtRect();
		CDRect rectErase(xyTrim.x, xyTrim.y+rectDom.Height(), xyTrim.x+rectDom.Width(), xyTrim.y-rectDom.Height());	
		Dom.TrimByLineOnSide(CDPoint(xyTrim.x, xyTrim.y - rectDom.Height()), CDPoint(xyTrim.x, xyTrim.y + rectDom.Height()), 1);
		Dom.EraseRect(rectErase);
		Dom.CutLightning(xyTrim.x, End.y, xyTrim.x, Stt.y, TRUE, 1);
		//////////////////////////////////////////////////////////////////////////
	}
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	//scallop상세1...
	pGir->GetHunchXyDom(pBx, A);
	pC->GetCrossXyDom(B);

	CDPoint xyCen;
	dTWeb  = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dRad      = pC->m_dwR*3;
	double dWVStiffL = pBxR->GetSection()->GetCrossBeam() ? pBxR->GetSection()->GetCrossBeam()->m_pVStiff->m_dWidth[0] : 0;
	double dGirW     = pGirR->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dR      = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_R);
	double dB      = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
	double dH	   = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	long   nID	   = pC->m_nRGirSpType ? 0 : 1;//용접0 볼트1
	double dS1     = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S1);
	CDPoint vDir   = pBx->GetSection()->GetCrossBeam()->GetVectorCross();
	double  dSharp = pGirR->GetLengthSharpOnBx(pBx,  TRUE);

	if(!pC->m_nRGirSpType)
	{
		CDomyun Dom1(&Dom);
		Dom1 += Dom;
		xyCen = A[7] + vX*dGirW/2 - vY*(pC->m_dRH1+pC->m_T2_CR) - vDir*(dWVStiffL+dTWeb/2+pC->m_dbR4)/vDir.x;
		if(pC->m_dRH1==0)
			xyCen = A[7] - vDir*(dWVStiffL+pC->m_dbR4-dSharp)/vDir.x;

		Dom1.TrimByCircle(xyCen, dRad, TRUE);
		pStd->m_pOptStd->SetEnvType(&Dom1,HCAD_DIML);	
		Dom1.Circle(xyCen, dRad);
		//치수선
		if(pStd->GetDomOptionStd()->m_Cross_bDimension)
		{
			Dom1.SetScaleDim(Dom.GetScaleDim()/2);
			Dom1.SetDirection("TOP");
			Dom1.DimMoveTo(xyCen.x-pC->m_dwB, xyCen.y, 0);
			Dom1.DimLineTo(pC->m_dwB, 0, COMMA(pC->m_dwB));
			Dom1.SetDirection("RIGHT");
			Dom1.DimMoveTo(xyCen.x, xyCen.y-pC->m_dwH, 0);
			Dom1.DimLineTo(pC->m_dwH, 0, COMMA(pC->m_dwH));
			//
			CString str="";
			Dom1.SetDirection("TOP");
			str.Format("R=%d", (long)pC->m_dwR);
			CDPoint xy = xyCen-vDir*(pC->m_dwB-pC->m_dwR)+CDPoint(-1,-1).Unit()*pC->m_dwR;
			Dom1.GiSiArrowExt(xy.x, xy.y, Dom.Always(8), Dom.Always(8), 6, str);
		}
		Dom1.RedrawByScale(2);	
		//
		Dom1.SetCalcExtRect();
		CDRect rect = Dom1.GetExtRect();
		Dom1.Move(-rect.CenterPoint().x, -rect.CenterPoint().y); 	
		CDPoint xyCircleCen = (A[7]+A[8])/2 + vX*(dSpace+rect.Width()+Dom.Always(2));//vX*(dGirW/2+dSpace+dRad*5) - vY*(pC->m_dRH1+pC->m_T2_CR);
		Dom1.Move(xyCircleCen.x, xyCircleCen.y);
		//
		*pDom << Dom1;
		//scallop상세2...
		if(pC->m_cGasset_CR)
		{
			CDomyun Dom2(&Dom);
			Dom2 += Dom;
			xyCen = A[7] + vX*dGirW/2 - vY*(pC->m_dRH1+pC->m_T2_CR+pC->m_H_CR+pC->m_T1_CR) - vDir*(dWVStiffL+dTWeb/2+pC->m_dbR3)/vDir.x;
			if(pC->m_dRH1==0)
				xyCen = A[7] - vDir*(dWVStiffL+pC->m_dbR3-dSharp)/vDir.x - vY*(pC->m_H_CR+pC->m_T1_CR);
			dRad = dR*3;
			Dom2.TrimByCircle(xyCen, dRad, TRUE);
			pStd->m_pOptStd->SetEnvType(&Dom2, HCAD_DIML);	
			Dom2.Circle(xyCen, dRad);
			//치수선
			if(pStd->GetDomOptionStd()->m_Cross_bDimension)
			{
				Dom2.SetScaleDim(Dom.GetScaleDim()/2);
				Dom2.SetDirection("TOP");
				Dom2.DimMoveTo(xyCen.x-dB, xyCen.y, 0);
				Dom2.DimLineTo(dB, 0, COMMA(dB));
				Dom2.SetDirection("RIGHT");
				Dom2.DimMoveTo(xyCen.x, xyCen.y-dB, 0);
				Dom2.DimLineTo(dH, 0, COMMA(dH));
				//
				CString str="";
				Dom2.SetDirection("TOP");
				str.Format("R=%d", (long)dR);
				CDPoint xy = xyCen - vDir*(dB-dR)+CDPoint(-1,-1).Unit()*dR;
				Dom2.GiSiArrowExt(xy.x, xy.y, Dom.Always(8), Dom.Always(8), 6, str);
			}
			Dom2.RedrawByScale(2);	
			//
			Dom2.SetCalcExtRect();
			rect = Dom2.GetExtRect();
			Dom2.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()/2); 	
			xyCircleCen = A[7] + vX*(dGirW/2+dSpace+dRad*5) - vY*(pC->m_H_CR+pC->m_dLH1+pC->m_T1_CR);
			Dom2.Move(xyCircleCen.x, xyCircleCen.y);
			//
			*pDom << Dom2;
		}
		if(pC->m_cGasset_CR)
		{
			//scallop상세3...
			CDomyun Dom3(&Dom);
			Dom3 += Dom;
			xyCen = pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), FALSE, FALSE);
			dRad = dS1*2;
			Dom3.TrimByCircle(xyCen, dRad, TRUE);
			pStd->m_pOptStd->SetEnvType(&Dom3, HCAD_DIML);	
			Dom3.Circle(xyCen, dRad);
			//치수선
			if(pStd->GetDomOptionStd()->m_Cross_bDimension)
			{
				Dom3.SetScaleDim(Dom.GetScaleDim()/2);
				Dom3.SetDirection("BOTTOM");
//				Dom3.DimMoveTo(xyCen.x-(dGirWLower/2-dTWeb/2-dWVStiffL-dS1), xyCen.y, 0);
//				Dom3.DimLineTo(dGirWLower/2-dTWeb/2-dWVStiffL-dS1, 0, COMMA(dGirWLower/2-dTWeb/2-dWVStiffL-dS1));
				Dom3.DimMoveTo(xyCen.x, xyCen.y, 0);
				Dom3.DimLineTo(dS1, 0, COMMA(dS1));
			}
			Dom3.RedrawByScale(2);	
			//
			Dom3.SetCalcExtRect();
			rect = Dom3.GetExtRect();
			Dom3.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()/2); 	
			xyCircleCen = A[7] + vX*(dGirW/2+dSpace+dRad*5) - vY*(pC->m_H_CR+pC->m_dLH1+pC->m_T1_CR+pC->m_T2_CR+pC->m_drtH+dRad);
			Dom3.Move(xyCircleCen.x, xyCircleCen.y);
			//
			*pDom << Dom3;
		}
	}
	//치수선은 Trim에서 제외함.
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	
	if(!pC->m_nRGirSpType)
	{
		xyCen = A[7] + vX*dGirW/2 - vY*(pC->m_dRH1+pC->m_T2_CR) - vDir*(dWVStiffL+dTWeb/2+pC->m_dbR4)/vDir.x;
		if(pC->m_dRH1==0)
			xyCen = A[7] - vDir*(dWVStiffL+pC->m_dbR4-dSharp)/vDir.x;
		Dom.Circle(xyCen, dRad);
		if(pC->m_cGasset_CR)
		{
			xyCen = A[7] + vX*dGirW/2 - vY*(pC->m_dRH1+pC->m_T2_CR+pC->m_H_CR+pC->m_T1_CR) - vDir*(dWVStiffL+dTWeb/2+pC->m_dbR3)/vDir.x;
			if(pC->m_dRH1==0)
				xyCen = A[7] - vDir*(dWVStiffL+pC->m_dbR3-dSharp)/vDir.x - vY*(pC->m_H_CR+pC->m_T1_CR);
			Dom.Circle(xyCen, dRad);

			xyCen = pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), FALSE, FALSE);
			Dom.Circle(xyCen, dRad);
		}
	}		
	
	if(pStd->GetDomOptionStd()->m_Cross_bDimension)
		DrawCross.DimCRBeamSangseTwo(&Dom, pBx);
	*pDom << Dom;
}

void CAPlateOutCrossBeam::AddCrossBeamFrontAtTotalDanmyun(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd		*pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp		*pBridge	= pStd->GetDataManage()->GetBridge();		
	COptionStd			*pDomOpt	= pStd->m_pDomOptionStd;

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawCross	DrawCross(pBridge->m_pDataManage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);

	BOOL bLeft, bFirst=TRUE;
	if(!pBx) return;

	//정면도 치수선
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	if(pStd->GetDomOptionStd()->m_Cross_bDimension)
	{
		DrawCross.DimCRBeamFrontAllGirder(&Dom, pBx);
	}


	CLongArray nArrGir;
	pBridge->GetIndexGirderToCrossBeam(nArrGir, pBx, TRUE);

	//정면도 거더
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);

	for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{
		if(nG<pBridge->GetGirdersu()/2)
			bLeft = FALSE;
		else
			bLeft = TRUE;
		
		if(pBx == NULL) continue;

		BOOL bMat = FALSE;
		for(long ix =0; ix < nArrGir.GetSize(); ix++)
		{
			if(nG == nArrGir.GetAt(ix))
			{
				bMat = TRUE;
				break;
			}
		}
		if(bMat == FALSE)
			continue;

		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		CPlateBasicIndex *pBxR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBx, pGirR);

		CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();

		DrawCross.DrawGirderFront(&Dom, pBx, CAPlateDrawCross::ALL);
		if(nG==pBridge->GetGirdersu()-2)
		{
			DrawCross.DrawGirderFront(&Dom, pBxR, CAPlateDrawCross::ALL);
			DrawCross.DrawHStiffFront(&Dom, pBxR, bLeft);
		}

		if(pBxR == NULL || pBx == NULL)
		{
			pBx = pBxR;
			continue;
		}


		DrawCross.DrawCRBeamTotalFrontOnBx(&Dom, pBx, FALSE, FALSE, TRUE);
		DrawCross.DrawHStiffFront(&Dom, pBx, bLeft);
		DrawCross.DrawFrontHGussetSplice(&Dom, pBx, /*bDim*/FALSE);
		
		if(!pC)
		{			
			pBx = pBxR;
			continue;
		}
		
		//가로보 제원
		DimTotalFrontJewonText(&Dom, pBx);
		//상세원그리기
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
		CDPoint A[10];
		pGir->GetHunchXyDom(pBx, A);
		CDPoint vX(1,0), vY(0,1);
		CDPoint vDir = pC->GetVectorCross();
		double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
		double dWGir = pGirR->GetWidthOnStation(pBx->GetStation(), TRUE);
		if(bFirst && pDomOpt->m_Cross_Sangse)
		{
			long n=1;
			CDPoint xyCen = (A[1]+A[2])/2 - vY*dHGir/2;
			CDPoint xyMajor = xyCen+vY*dHGir*1.3;
			CDPoint xyMinor = xyCen+vX*dHGir/4;
			Dom.Ellipse(xyCen.x, xyCen.y, CDPoint(0,dHGir*0.7),CDPoint(-dHGir/4,0), 0, 360);
			CString str = "";
			str.Format("상세 %d", n);
			Dom.CircleArrow(xyCen.x, xyCen.y-dHGir*0.7, Dom.Always(10), Dom.Always(10), 7, str, "");
			n++;
			if(pC->m_cGasset_CR==TRUE)
			{
				xyCen = (A[1]+A[2])/2 - vY*(pC->m_dLH1+pC->m_T1_CR+pC->m_T2_CR+pC->m_H_CR) + vDir*pC->m_P_CR/vDir.x;
				Dom.Circle(xyCen, 150/*pC->m_P_CR*0.7*/);
				str.Format("상세 %d", n);
				Dom.CircleArrow(xyCen.x, xyCen.y-150/*pC->m_P_CR*0.7*/, Dom.Always(10), Dom.Always(10), 7, str, "");
				n++;
			}
			xyCen = A[7] + vX*dWGir/2 - vY*dHGir/2;
			Dom.Ellipse(xyCen.x, xyCen.y, CDPoint(0,dHGir*0.7),CDPoint(-dHGir/4,0), 0, 360);
			str.Format("상세 %d", n);
			Dom.CircleArrow(xyCen.x, xyCen.y-dHGir*0.7, Dom.Always(10), Dom.Always(10), 7,str, "");
			n++;
			if(pC->m_cGasset_CR==TRUE)
			{
				xyCen = A[7] + vX*dWGir/2 - vY*(pC->m_dRH1+pC->m_T1_CR+pC->m_T2_CR+pC->m_H_CR) - vDir*pC->m_P_CR/vDir.x;
				Dom.Circle(xyCen, 150/*pC->m_P_CR*0.7*/);
				str.Format("상세 %d", n);
				Dom.CircleArrow(xyCen.x, xyCen.y-150/*pC->m_P_CR*0.7*/, Dom.Always(10), Dom.Always(10), 7, str, "");
			}
			bFirst = FALSE;
		}
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
		pBx = pBxR;
	}	
	*pDom << Dom;
}


void CAPlateOutCrossBeam::AddCrossBeamPyungAtTopTotalDanmyun(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawPyung DrawPyung(pBridge->m_pDataManage);
	CAPlateDrawPyung DrawPyung2(pBridge->m_pDataManage);
	
	CDomyun Dom(pDom);
	CDomyun DomDim(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	double dGirL = 500;
	BOOL bUpper = TRUE;
	if(!pBx) return;

	CDPoint xyStt = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	double dX = pBridge->GetGirder(0)->m_dGirderCenter;
	CDPoint vGir1 = pBx->GetGirder()->GetLine()->GetAngleAzimuth(pBx->GetStation());
	
	//평면도 치수선
	pStd->m_pOptStd->SetEnvType(&DomDim,HCAD_DIML);
	if(pStd->GetDomOptionStd()->m_Cross_bDimension)
	{
		DrawPyung2.DrawCRBeamDimAllDanmyun(&DomDim, pBx, TRUE);
	}
	DomDim.Move(xyStt.x-dX, xyStt.y);
	double dRotate = vGir1.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
	DomDim.Rotate(-dRotate, xyStt.x, xyStt.y); 
	
	//
	CLongArray nArrBeamGir;
	pBridge->GetIndexGirderToCrossBeam(nArrBeamGir, pBx, TRUE);

	//평면도 거더
	CPlateBasicIndex* pBxGir = pBx;
	CDPoint vGir = pBridge->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
	long nG = 0;
	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{		
		if(pBxGir == NULL) continue;

		BOOL bSameGir = FALSE;
		for (long ix =0; ix < nArrBeamGir.GetSize(); ix++)
		{
			long nGir = nArrBeamGir.GetAt(ix);
			if(nG == nGir)
			{
				bSameGir = TRUE;
				break;;
			}
		}
		if(bSameGir == FALSE) continue;

		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		CPlateBasicIndex *pBxGirR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBxGir, pGirR);
		DrawPyung.DrawGirderWithenStation(&Dom, pBxGir, pBxGir->GetStation()-dGirL, pBxGir->GetStation()+dGirL, TRUE, TRUE); 
		pBxGir = pBxGirR;
	}	

	//평면도 가로보 
	for(nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{		

		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		CPlateBasicIndex *pBxR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
		CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();

		if(pBxR == NULL || pC == NULL) 
			continue;

		// CROSS BEAM : 상면
		DrawPyung.DrawCRBeamWebOnBx(&Dom, pBx, bUpper, TRUE, TRUE);
		if(pC && pC->m_nLGirSpType)
		{
			BOOL bLeft = TRUE;
			DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bUpper, bLeft, TRUE, TRUE, FALSE, FALSE);
			DrawPyung.DrawCRBeamGassetWebOnBx(&Dom, pBx, bUpper, bLeft);
			DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom, pBx, bUpper, bLeft, TRUE);
		}
		if(pC && pC->m_nRGirSpType)
		{
			BOOL bLeft = FALSE;
			DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bUpper, bLeft, TRUE, TRUE, FALSE,FALSE);
			DrawPyung.DrawCRBeamGassetWebOnBx(&Dom, pBx, bUpper, bLeft);
			DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom, pBx, bUpper, bLeft, TRUE);
		}
		//가로보 복부 수직보강재
		DrawPyung.DrawCRBeamVStiffnerOnBx(&Dom,pBx,bUpper,TRUE,TRUE);
		DrawPyung.DrawCRBeamAnchorBarOnBx(&Dom,pBx,TRUE,TRUE);

		//지점부 수직보강재 or 거더 복부 수직보강재
		if(pBx->IsJijum())	DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx, bUpper, FALSE);
		else				DrawPyung.DrawVStiffOnBx(&Dom, pBx, FALSE, bUpper);

		if(pBxR->IsJijum())	DrawPyung.DrawJiJumStiffOnBx(&Dom, pBxR, bUpper, FALSE);
		else				DrawPyung.DrawVStiffOnBx(&Dom, pBxR, FALSE, bUpper);
		
		//가로보 제원
		DimTotalPlanUpperJewonText(&Dom, pBx);

		pBx = pBxR;
	}


	//
	*pDom << Dom;
	*pDom << DomDim;
}

void CAPlateOutCrossBeam::AddCrossBeamPyungAtBottomTotalDanmyun(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CAPlateDrawPyung DrawPyung(pBridge->m_pDataManage);
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	double dGirL = 500;
	BOOL bUpper = FALSE;
	if(!pBx) return;

	CDPoint xyStt = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	double dX = pBridge->GetGirder(0)->m_dGirderCenter;
	CDPoint vGir1 = pBx->GetGirder()->GetLine()->GetAngleAzimuth(pBx->GetStation());

	//평면도 치수선
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	if(pStd->GetDomOptionStd()->m_Cross_bDimension)
	{
		DrawPyung.DrawCRBeamDimAllDanmyun(&Dom, pBx, FALSE);
	}
	Dom.Move(xyStt.x-dX, xyStt.y);
	double dRotate = vGir1.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
	Dom.Rotate(-dRotate, xyStt.x, xyStt.y); 
	

	//평면도 거더
	CLongArray nArrBeamGir;
	pBridge->GetIndexGirderToCrossBeam(nArrBeamGir, pBx, TRUE);

	CPlateBasicIndex* pBxGir = pBx;
	CDPoint vGir = pBridge->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
	long nG = 0;
	for(nG=0; nG<pBridge->GetGirdersu(); nG++)
	{		
		if(pBxGir == NULL) continue;

		BOOL bSameGir = FALSE;
		for (long ix =0; ix < nArrBeamGir.GetSize(); ix++)
		{
			long nGir = nArrBeamGir.GetAt(ix);
			if(nG == nGir)
			{
				bSameGir = TRUE;
				break;;
			}
		}

		if(bSameGir == FALSE) continue;
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		CPlateBasicIndex *pBxGirR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBxGir, pGirR);
		DrawPyung.DrawGirderWithenStation(&Dom, pBxGir, pBxGir->GetStation()-dGirL, pBxGir->GetStation()+dGirL, FALSE, TRUE); 
		pBxGir = pBxGirR;
	}	

	for(nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{	
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CPlateGirderApp* pGirR = pBridge->GetGirder(nG+1);
		CPlateBasicIndex *pBxR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
		CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();

		if(pBxR == NULL) continue;

		if(!pC) continue;
		// CROSS BEAM : 하면
		bUpper = FALSE;
		DrawPyung.DrawCRBeamWebOnBx(&Dom, pBx, bUpper, TRUE, TRUE);
		if(pC->m_nLGirSpType)
		{
			BOOL bLeft = TRUE;
			DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bUpper, bLeft, TRUE, TRUE, FALSE, FALSE);
			DrawPyung.DrawCRBeamGassetWebOnBx(&Dom, pBx, bUpper, bLeft);
			DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom, pBx, bUpper, bLeft, TRUE);
		}
		if(pC->m_nRGirSpType)
		{
			BOOL bLeft = FALSE;
			DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bUpper, bLeft, TRUE, TRUE, FALSE, FALSE);
			DrawPyung.DrawCRBeamGassetWebOnBx(&Dom, pBx, bUpper, bLeft);
			DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom, pBx, bUpper, bLeft, TRUE);
		}
		//가로보 복부 수직보강재
		DrawPyung.DrawCRBeamVStiffnerOnBx(&Dom,pBx,bUpper,TRUE,TRUE);
		//거더   복부 수직보강재
		DrawPyung.DrawVStiffOnBx(&Dom, pBx, FALSE, bUpper);
		DrawPyung.DrawVStiffOnBx(&Dom, pBxR, FALSE, bUpper);
		//지점부 수직보강재
		DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx, bUpper, FALSE);
		DrawPyung.DrawJiJumStiffOnBx(&Dom, pBxR, bUpper, FALSE);
		//가로보 제원
		DimTotalPlanLowerJewonText(&Dom, pBx);

		pBx = pBxR;
	}

	//
	*pDom << Dom;
}

void CAPlateOutCrossBeam::MakeCrossBeamList()
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	BOOL bAllGirder = pDomOpt->m_Cross_AllGirder;

	CTypedPtrArray	<CObArray, CPlateCrossBeam*> crListTemp;
	crListTemp.RemoveAll();

	m_CrossList.RemoveAll();

	long GirderSu = pBridge->GetGirdersu();
	long n = 0;
	for (n = 0; n < GirderSu-1; n++)
	{
		CPlateGirderApp	*pGir = pBridge->GetGirder(n);
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			if(pBx->GetSection()->GetenDetType() == SECDET_CROSSBEAM)
			{
				CPlateCrossBeam	*pC = pBx->GetSection()->GetCrossBeam();
				long nGNum1 = pBx->GetNumberGirder();
				long nSize = crListTemp.GetSize();
				long i = 0;
				for (i = 0; i < nSize; i++)
				{
					CPlateCrossBeam *pCComp = crListTemp.GetAt(i);
					long nGNum2 = pCComp->GetBx()->GetNumberGirder();
					if (bAllGirder)
					{
						if(pC->IsSamePlateJewon(pCComp) && nGNum1 == nGNum2)
							break;
					}
					else
					{
						if(pC->IsSamePlateJewon(pCComp))
							break;
					}
				}
				if (i == nSize) {
					crListTemp.Add(pC);
				}
			}

			pBx = (CPlateBasicIndex*)Finder.GetBxNext();
		}
	}

	//sort 지점부 - 일반부
	
	for(n = 0; n < crListTemp.GetSize(); n++)
	{
		if (crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_CrossList.Add(crListTemp.GetAt(n));
	}
	for (n = 0; n < crListTemp.GetSize(); n++)
	{
		if (!crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_CrossList.Add(crListTemp.GetAt(n));
	}
}

//첫번째 거도의 가로보의 BX만 저장하고 있음.
void CAPlateOutCrossBeam::MakeCrossBeamListAtFirstGirder()
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	CTypedPtrArray	<CObArray, CPlateCrossBeam*> crListTemp;
	crListTemp.RemoveAll();
	
	m_CrossListAtFirstGirder.RemoveAll();

	//거더 중앙 배치인지 아닌지를 구별
	if(pBridge->m_nCrossArrangeType !=0 )
	{
		CPlateGirderApp	*pGir = pBridge->GetGirder(0);
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			if(pBx->GetSection()->GetenDetType() == SECDET_CROSSBEAM)
			{
				CPlateCrossBeam	*pC = pBx->GetSection()->GetCrossBeam();
				crListTemp.Add(pC);
			}

			pBx = (CPlateBasicIndex*)Finder.GetBxNext();
		}
	}
	else
	{
		//{APLATE-688 거더 첫번째와 거더 첫번째-2인 구간에서 가로보의 위치를 구함
		long nCntGir = pBridge->GetQtyGirder();
		CDoubleArray dArrStation;
		for (long nStartGir =0; nStartGir < 2; nStartGir++)
		{
			long nG = nStartGir == 0? 0 : nCntGir-2;
			if(nG == -1) 
				break;

			CPlateGirderApp *pGir = pBridge->GetGirder(nG);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			int nCount = 1;
			while(pBx)
			{
				CSection *pSec = pBx->GetSection();
				if(pSec == NULL) continue;

				if(pSec)
				{	
					if(pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
					{
						double dStation = pBx->GetStation();
						CDPoint vAng = pBx->GetAngle();

						if(pBx->GetSection()->IsenType(SECTION_C) == FALSE && vAng != CDPoint(0,1) && nG > 0)
						{
							pBx = Finder.GetBxNext();
							continue;									
						}

						BOOL bSame = FALSE;
						for (long ix =0; ix < dArrStation.GetSize(); ix++)
						{
							double dPrevSta = dArrStation.GetAt(ix);
							if(Compare(dPrevSta, dStation, _T("="), 0.1))
							{
								bSame  = TRUE;
								break;;
							}
						}

						if(bSame == TRUE)
							break;

						dArrStation.Add(dStation);
						CPlateCrossBeam	*pC = pBx->GetSection()->GetCrossBeam();
						crListTemp.Add(pC);
					}
				}
				pBx = Finder.GetBxNext();
				nCount++;
			}
		}

		//순서를 정렬해줌
		for (long ix =0; ix < dArrStation.GetSize()-1; ix++)
		{
			double dStation				= dArrStation.GetAt(ix);
			CPlateCrossBeam	*pC	= crListTemp.GetAt(ix);

			for (long jx =ix+1; jx < dArrStation.GetSize(); jx++)
			{
				double dStationNext		= dArrStation.GetAt(jx);
				CPlateCrossBeam *pCN		= crListTemp.GetAt(jx);

				if(Compare(dStation, dStationNext, _T(">"), 0.1) )
				{
					crListTemp.SetAt(ix, pCN);
					crListTemp.SetAt(jx, pC);

					dArrStation.SetAt(ix, dStationNext);
					dArrStation.SetAt(jx, dStation);
				}
			}
		}
	}
	

	long n = 0;
	//sort 지점부 - 일반부
	for (n = 0; n < crListTemp.GetSize(); n++)
	{
		if (crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_CrossListAtFirstGirder.Add(crListTemp.GetAt(n));
	}
	for (n = 0; n < crListTemp.GetSize(); n++)
	{
		if (!crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_CrossListAtFirstGirder.Add(crListTemp.GetAt(n));
	}
}

void CAPlateOutCrossBeam::DrawFrontCrossMain(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir     = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	*pGirR    = (CPlateGirderApp*)pGir->m_pRight;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);

	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	CDPoint A[20], B[10];
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	CAPlateDrawCross DrawCross(pStd->GetDataManage());	
	//
	DrawCross.DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);
	DrawCross.DrawCRBeamVStiffnerOnBx(&Dom, pBx);
	DrawCross.DrawCRBeamAnchorBar(&Dom, pBx);
	//
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCross.DrawCRBeamSpliceOnBx(&Dom, pBx);
 	//
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCross.DrawCRBeamGassetOnBx(&Dom, pBx);
	//
	DrawCross.DrawGirderFront(&Dom, pBx, CAPlateDrawCross::ALL);
	DrawCross.DrawGirderFront(&Dom, pBxR, CAPlateDrawCross::ALL);
	//수평보강재
	DrawCross.DrawHStiffFront(&Dom, pBx, TRUE);
	DrawCross.DrawHStiffFront(&Dom, pBx, FALSE);
	DrawCross.DrawHStiffFront(&Dom, pBxR, TRUE);
	DrawCross.DrawHStiffFront(&Dom, pBxR, FALSE);
	
	//치수선
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	//
 	DimFrontCrossMainTop(&Dom, pBx);
	DimFrontCrossMainLeft(&Dom, pBx);
	DimFrontCrossMainRight(&Dom, pBx);
	DimTextFrontSlope(&Dom, pBx);
	DrawCross.DimCRBeamSpliceTotal(&Dom, pBx, A[2].x, A[2].y, FALSE, 1);
	DimFrontCrossJewonText(&Dom, pBx);
	DimWeldFrontCrossMain(&Dom, pBx);
	DimFrontBracketBolt(&Dom, pBx);

//	if(!pOpt->m_Cross_BKSupport)
	{
		if(pC->m_cGasset_CR)
		{
			DimFrontCrossGassetSupportLeft(&Dom, pBx);
			DimFrontCrossGassetSupportRight(&Dom, pBx);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 이동 : 좌측복부판의 상단을 (0,0)으로
	CDPoint xyOrigin = (B[1]+B[2])/2;
	Dom.Move(-xyOrigin.x, -xyOrigin.y);

	*pDom << Dom;

}
void CAPlateOutCrossBeam::DrawPlanCrossMain(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir     = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	*pGirR    = (CPlateGirderApp*)pGir->m_pRight;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();
	double dWebT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dGirLen = 500;
	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	BOOL bUpper	   = TRUE;
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	// CROSS BEAM
	DrawPyung.DrawCRBeamWebOnBx(&Dom,pBx,bUpper,TRUE,FALSE);
	if(pC->m_nLGirSpType)
	{
		DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bUpper, TRUE, TRUE, TRUE, FALSE, FALSE);
		DrawPyung.DrawCRBeamGassetWebOnBx(&Dom,pBx,bUpper,TRUE);
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,TRUE,TRUE);
	}
	DrawPyung.DrawCRBeamVStiffnerOnBx(&Dom,pBx,bUpper,TRUE,FALSE);
	DrawPyung.DrawCRBeamAnchorBarOnBx(&Dom,pBx,TRUE,FALSE);
	// CROSS BEAM : 하면
	bUpper = FALSE;
	DrawPyung.DrawCRBeamWebOnBx(&Dom,pBx,bUpper,FALSE,TRUE);
	if(pC->m_nRGirSpType)
	{
		DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom, pBx, bUpper, FALSE, TRUE, TRUE, FALSE, FALSE);
		DrawPyung.DrawCRBeamGassetWebOnBx(&Dom,pBx,bUpper,FALSE);
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,FALSE,TRUE);
	}
	//가로보 복부 수직보강재
	DrawPyung.DrawCRBeamVStiffnerOnBx(&Dom,pBx,bUpper,FALSE,TRUE);
	//거더   복부 수직보강재
	DrawPyung.DrawVStiffOnBx(&Dom, pBx);
	DrawPyung.DrawVStiffOnBx(&Dom, pBxR);
	//지점부 수직보강재
	DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx, TRUE, FALSE);
	DrawPyung.DrawJiJumStiffOnBx(&Dom, pBxR, TRUE, FALSE);
	//거더라인
	DrawPyung.DrawGirderWithenStation(&Dom, pBx, pBx->GetStation()-dGirLen, pBx->GetStation()+dGirLen, TRUE);
	DrawPyung.DrawGirderWithenStation(&Dom, pBxR, pBxR->GetStation()-dGirLen, pBxR->GetStation()+dGirLen, FALSE);
	//
	CDPoint xyOrigin = pGir->GetXyGirderDis( pBx->GetStation());
	CDPoint vCros    = pC->GetVectorPlan();
	Dom.Move(-xyOrigin.x, -xyOrigin.y);
	Dom.Rotate(-vCros.GetAngleRadian(),0,0);
	Dom.Move(0,0);
	//
	double Len = pGir->GetDisFrontPlateWebToWeb(pBx)/2 + dWebT/2;
	double dY  = max(pC->m_uM_CR+pC->m_uN_CR,pC->m_dM_CR+pC->m_dN_CR);
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	Dom.GiSiArrow(Len,-dY-Dom.Always(5),-Dom.Always(10), Dom.Always(7),4,"상 면 ");
	Dom.GiSiArrow(Len,-dY-Dom.Always(5), Dom.Always(10), Dom.Always(7),0," 하 면");
	pOpt->SetEnvType(&Dom,HCAD_CENT);
	Dom.LineTo(Len,dY+Dom.Always(1),Len,-dY-Dom.Always(7));
	//치수선
	DimPlanCrossMainTop(&Dom, pBx);
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
		DimPlanCrossMainSplice(&Dom, pBx);
	DimPlanCrossSpliceSide(&Dom, pBx);
	DimPlanCrossJewonText(&Dom, pBx);
	//
	*pDom << Dom;
}


void CAPlateOutCrossBeam::DrawDetailCrossGasset(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir     = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	*pGirR    = (CPlateGirderApp*)pGir->m_pRight;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();
	double dGirLen = 500;
	CDomyun	Dom(pDom);
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	// CROSS BEAM
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) || 
	   (!pC->m_nLGirSpType && pC->m_nRGirSpType))
	{
		pOpt->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,FALSE);
		pOpt->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom,pBx,bUpper,FALSE,TRUE,TRUE, FALSE, TRUE);		// 우
		DrawPyung.DrawCRBeamGassetWebOnBx(&Dom,pBx,bUpper,FALSE);					// 우
		////
//		pOpt->SetEnvType(&Dom, HCAD_STLC);
//		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,FALSE,TRUE);
		// 우측 거더
		pOpt->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawGirderWithenStation(&Dom, pBxR, pBxR->GetStation()-dGirLen, pBxR->GetStation()+dGirLen, bUpper);
		DrawPyung.DrawVStiffOnBx(&Dom, pBxR, FALSE, bUpper);
		DrawPyung.DrawJiJumStiffOnBx(&Dom, pBxR, bUpper, FALSE);
		DrawCrossBeamFlange(&Dom, pBx, bUpper, FALSE);
	}
	else
	{
		pOpt->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,TRUE);
		pOpt->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom,pBx,bUpper,TRUE,TRUE,TRUE, FALSE, TRUE);		// 우
		DrawPyung.DrawCRBeamGassetWebOnBx(&Dom,pBx,bUpper,TRUE);					// 우
		////
//		pOpt->SetEnvType(&Dom, HCAD_STLC);
//		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,TRUE,TRUE);
		//
		pOpt->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawGirderWithenStation(&Dom, pBx, pBx->GetStation()-dGirLen, pBx->GetStation()+dGirLen, bUpper);
		DrawPyung.DrawVStiffOnBx(&Dom, pBx, FALSE, bUpper);
		DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx, bUpper, FALSE);
		DrawCrossBeamFlange(&Dom, pBx, bUpper, TRUE);
	}
	// 이동 및 회전 : 좌측복부판의 중심을 (0,0)으로
	CDPoint GirL =  pGir->GetXyGirderDis( pBx->GetStation());
	CDPoint GirR = pGirR->GetXyGirderDis(pBxR->GetStation());
	CDPoint vCros = pC->GetVectorPlan();
	Dom.Move(-GirR.x,-GirR.y);
	Dom.Rotate(-vCros.GetAngleRadian(),0,0);
	Dom.Move(0,0);

	DimDetailCrossGasset(&Dom, pBx, bUpper);

	*pDom << Dom;
}
void CAPlateOutCrossBeam::DrawCrossBeamFlange(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft)
{
	CAPlateDrawStd	 *pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp  *pBridge = pStd->GetDataManage()->GetBridge();
	COptionStd	     *pDomOpt = pStd->GetDomOptionStd();
	CPlateGirderApp	 *pGir    = pBx->GetGirder();
	CPlateGirderApp	 *pGirR   = pGir->m_pRight;
	CPlateBasicIndex *pBxR    = pBx->GetBxMatchByCrossBeam(pGirR);
	CARoadOptionStd  *pOpt	  = pStd->GetDataManage()->GetOptionStd();
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	double dWebT  = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWebTR = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);

	CDomyun	Dom(pDom);
	CDPoint xyStt, xyEnd;
	CDPoint vCross = pC->GetVectorPlan();
	double dHalf=0;
	if(bUpper)
		dHalf = pC->m_uM_CR;
	else
		dHalf = pC->m_dM_CR;

	if(bLeft)
	{
		xyStt = pGir->GetXyGirderDis( pBx->GetStation());
		xyStt += vCross*(pC->m_P_CR+dWebT/2);
		xyEnd = xyStt+vCross*pC->m_P_CR;
		Dom.MoveTo(xyStt+vCross.Rotate90()*dHalf);
		Dom.LineTo(xyEnd+vCross.Rotate90()*dHalf);
		Dom.MoveTo(xyStt-vCross.Rotate90()*dHalf);
		Dom.LineTo(xyEnd-vCross.Rotate90()*dHalf);
		
		if(bUpper)		pOpt->SetEnvType(&Dom, HCAD_STLH);
		else			pOpt->SetEnvType(&Dom, HCAD_STLC);

		Dom.MoveTo(xyStt+vCross.Rotate90()*pC->m_T3_CR/2);
		Dom.LineTo(xyEnd+vCross.Rotate90()*pC->m_T3_CR/2);
		Dom.MoveTo(xyStt-vCross.Rotate90()*pC->m_T3_CR/2);
		Dom.LineTo(xyEnd-vCross.Rotate90()*pC->m_T3_CR/2);

		pOpt->SetEnvLType(&Dom, LT_SOLID);
	}
	else
	{
		xyStt = pGirR->GetXyGirderDis(pBxR->GetStation());
		xyStt += -vCross*(pC->m_P_CR+dWebTR/2);
		xyEnd = xyStt-vCross*pC->m_P_CR;

		Dom.MoveTo(xyStt+vCross.Rotate90()*dHalf);
		Dom.LineTo(xyEnd+vCross.Rotate90()*dHalf);
		Dom.MoveTo(xyStt-vCross.Rotate90()*dHalf);
		Dom.LineTo(xyEnd-vCross.Rotate90()*dHalf);

		if(bUpper)		pOpt->SetEnvType(&Dom, HCAD_STLH);
		else			pOpt->SetEnvType(&Dom, HCAD_STLC);

		Dom.MoveTo(xyStt+vCross.Rotate90()*pC->m_T3_CR/2);
		Dom.LineTo(xyEnd+vCross.Rotate90()*pC->m_T3_CR/2);
		Dom.MoveTo(xyStt-vCross.Rotate90()*pC->m_T3_CR/2);
		Dom.LineTo(xyEnd-vCross.Rotate90()*pC->m_T3_CR/2);

		pOpt->SetEnvLType(&Dom, LT_SOLID);
	}

	*pDom << Dom;
}
void CAPlateOutCrossBeam::DrawDetailCrossVStiff(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir     = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	*pGirR    = (CPlateGirderApp*)pGir->m_pRight;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();

	
	double height	= pC->m_H_CR;			//가로보 높이
	double uW		= pC->m_uM_CR;			//상부플렌지 너비/2
	double dW		= pC->m_dM_CR;			//하부플렌지 너비/2
	double T2		= pC->m_T2_CR;			//상부플렌지 두께
	double T1		= pC->m_T1_CR;			//하부플렌지 두께
	double T3		= pC->m_T3_CR;			//복부판 두께

	double CenSta	= pBx->GetStation();//GetStationCrossRight();		//오른쪽에있는 가로보의 스테이션을 구함
	CDPoint cAng	= pBx->GetAngleSection();		//각도
//	double  cDist	= pGir->GetDisWebToWeb90(pBx)/2;
//	double  Sta		= pGir->GetStationBySc(CenSta,cDist,cAng);
	double  JSlop	= pGir->GetGiulFlangeUpperLowerCamber(CenSta);//ksg=>CenSta

	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	//가로보 상부플렌지 
	//우측 상부 아래면 
	CDPoint xy1, xy2;
	xy1.x = -uW;
	xy1.y = xy1.x * JSlop;
	xy2.x = uW;
	xy2.y = xy2.x * JSlop + T2;
	Dom.Rectangle(xy1, xy2);
	
	xy1.x = -T3/2;
	xy1.y = xy1.x * JSlop;
	xy2.x = T3/2;
	xy2.y = xy2.x * JSlop;
	Dom.LineTo(xy1.x,xy1.y,xy1.x,xy1.y-(height+xy1.y));
	Dom.LineTo(xy2.x,xy2.y,xy2.x,xy2.y-(height+xy2.y));

	xy1.x = -dW;
	xy1.y -= height+xy1.y;
	xy2.x = dW;
	xy2.y -= height+xy2.y+T1;
	Dom.Rectangle(xy1, xy2);

	CPlatePieceObject pObj(pGir, CP_V);
	pObj.m_nRow = 0;
	pObj.m_nMarkBuje = pBx->IsJijum() ? CP_V : CG_V;
	pC->CreatePieceObjPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pStd->m_pOptStd, TRUE);

	CPlatePieceObject pObj2(pGir, CP_V);
	pObj2.m_nRow = 1;
	pObj2.m_nMarkBuje = pBx->IsJijum() ? CP_V : CG_V;
	pC->CreatePieceObjPlate(&pObj2);
	pObj2.DrawPieceOutline(&Dom, pStd->m_pOptStd, TRUE);

	Dom.Move(0,-T2);	//치수선과 기준점 차이로 인하여 

	DimDetailCrossVStiff(&Dom, pBx);
	
	*pDom << Dom;
}
void CAPlateOutCrossBeam::DimDetailCrossGasset(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir     = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	*pGirR    = (CPlateGirderApp*)pGir->m_pRight;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();

	CDimDomyunEx Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_DIML);

	CDPoint	vAng = pBx->GetAngle();
	double	Angle = ToRadian(90) - vAng.GetAngleRadian();
	double	AA = bUpper ? pC->m_uA : pC->m_dA;
	long	B = bUpper ? pC->m_uB : pC->m_dB;
	long	B1 = bUpper ? (long)pC->m_uB1 : (long)pC->m_dB1;
	double	C = bUpper ? pC->m_uC : pC->m_dC;
	double	C1 = bUpper ? pC->m_uC1 : pC->m_dC1;
	double	D = bUpper ? pC->m_uD : pC->m_dD;
	long	E = bUpper ? pC->m_uE : pC->m_dE;
	double	F = bUpper ? pC->m_uF : pC->m_dF;
	double	M = bUpper ? pC->m_uM_CR : pC->m_dM_CR;
	double	M1 = bUpper ? pC->m_uM1 : pC->m_dM1;
	double	dWebT  = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double	dWebTR = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
	double	dWebToWeb = pGir->GetDisPlanPlateWebToWeb(pBx);
	BOOL	bConnect	= pC->m_bConnectionPlate;
	BOOL	bSpType		= FALSE;
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) || (!pC->m_nLGirSpType && pC->m_nRGirSpType))
		bSpType = TRUE;
	else	bSpType = FALSE;

	CDPoint BP;
	if(bSpType)
	{
		BP.x = - (pC->m_P_CR+dWebTR/2)/cos(Angle) - AA - B*C - C1/2;
		BP.y = -M;
	}
	else
	{
		BP.x = - (dWebTR/2-pC->m_P_CR)/cos(Angle) - dWebToWeb - AA - B*C - C1/2;
		BP.y = -M;
	}
	Dom.SetDirection("BOTTOM");
	long DimDan = 1;

	Dom.DimMoveTo(BP,DimDan,FALSE,COMMA(AA));
	Dom.DimLineTo(AA,BP.y,"");
//	if (C == C1)
//		Dom.DimLineTo((B*2+1)*C,BP.y,COMMA((double)B*2+1)+"@"+COMMA(C),"="+COMMA(((double)B*2+1)*C));
//	else
	{
		Dom.DimLineTo(B*C,BP.y,COMMA(B)+"@"+COMMA(C),"="+COMMA(B*C));
		Dom.DimLineTo(C1,BP.y,COMMA(C1));
		if(bConnect)
		{
			Dom.DimLineTo(B1*C,BP.y,COMMA(B1)+"@"+COMMA(C),"="+COMMA(B1*C));
		}
		else
			Dom.DimLineTo(B*C,BP.y,COMMA(B)+"@"+COMMA(C),"="+COMMA(B*C));
	} 
	Dom.DimLineTo(AA,BP.y,COMMA(AA),"",FALSE,TRUE);
	//전체치수 
	DimDan++;
	Dom.DimMoveTo(BP,DimDan);
	double Len = 0;
	if(bConnect)	Len = AA + B*C + C1 + B1*C + AA;
	else			Len = (AA+B*C+C1/2)*2;
	Dom.DimLineTo(Len,BP.y,COMMA(Len));

	DimDan=1;
	if(bSpType)
	{
		Dom.SetDirection("LEFT");
		BP.x = - (pC->m_P_CR+dWebTR/2)*2/cos(Angle);
		BP.y = -M;
	}
	else
	{
		Dom.SetDirection("RIGHT");
		BP.x = - (dWebToWeb-pC->m_P_CR*2)/cos(Angle);
		BP.y = -M;
	}

	Dom.DimMoveTo(BP,DimDan,FALSE,COMMA(D));
	Dom.DimLineTo(BP.x,D,"");
	if (E == 0)
	{
		Len = M*2 - D*2;
		Dom.DimLineTo(BP.x, Len,COMMA(Len));
	}
	else
	{
		if (E == 1)
		{
			Len = E * F;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
			Len = M*2 - D*2 - E*F*2;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
			Len =E*F;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
		}
		else
		{
			Len = E * F;
			Dom.DimLineTo(BP.x, Len,COMMA((double)E)+"@"+COMMA(F),"="+COMMA(Len));
			Len = M*2 - D*2 - E*F*2;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
			Len = E * F;
			Dom.DimLineTo(BP.x, Len,COMMA((double)E)+"@"+COMMA(F),"="+COMMA(Len));
		}
	}
	Dom.DimLineTo(BP.x,D,COMMA(D),"",FALSE,TRUE);
	//전체치수
	DimDan++;
	Dom.DimMoveTo(BP,DimDan);
	Dom.DimLineTo(BP.x, M*2,COMMA(M*2));

	DimDan=1;
	if(bSpType)
	{
		Dom.SetDirection("RIGHT");
		BP.x = 0;
		BP.y = -M1;
	}
	else
	{
		Dom.SetDirection("LEFT");
		BP.x = -dWebToWeb-(dWebT+dWebTR)/2;
		BP.y = -M1;
	}
	double dW = bUpper ? pC->m_uW : pC->m_dW;
	Dom.DimMoveTo(BP,DimDan);
	Dom.DimLineTo(BP.x, dW,COMMA(dW));
	Len = M1*2 - dW*2;
	Dom.DimLineTo(BP.x, Len, "",COMMA(Len));
	Dom.DimLineTo(BP.x, dW,COMMA(dW));
	*pDom << Dom;

	CDPoint vCros = pBx->GetAngle();
	CPlatePieceObject pObj(pGir);
	
	if(bUpper)	pObj.m_nMarkBuje = CP_BR_F_U;
	else		pObj.m_nMarkBuje = CP_BR_F_L;
	if(bSpType)	pObj.m_nCol = 1;
	else		pObj.m_nCol = 0;

	pC->CreatePieceObjPlateGassetFlange(&pObj, TRUE,  TRUE, TRUE, TRUE);
	CDPointArray mArr;
	pObj.GetPointArray(mArr);

	DimDan=1;
	Dom.SetDirection("TOP");
	C = bUpper ? pBridge->m_dRoundChipCrossUpper : pBridge->m_dRoundChipCrossLower;

	if(bSpType)	BP = mArr.GetAt(1);
	else		BP = mArr.GetAt(0);

	Dom.DimMoveTo(BP.x,BP.y,DimDan);
	Dom.DimLineTo(C,BP.y,COMMA(C), "", FALSE, TRUE);
	Dom.Rotate(ToRadian((180-vCros.GetAngleDegree())-90),0,0);
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) ||
	   (!pC->m_nLGirSpType && pC->m_nRGirSpType))
	    Dom.Move(0,0);
	else
		Dom.Move(-dWebToWeb-(dWebT+dWebTR)/2, 0);
	*pDom << Dom;
	///////// R
	CDPoint RBase, xy, xy1, xy2;
	CString str;
	double R	= bUpper ? pBridge->m_dRoundCrossUpper	   : pBridge->m_dRoundCrossLower;

	xy1 = mArr.GetAt(1);
	xy2 = mArr.GetAt(2);

	double Bulge = GetArcBulge(xy1, xy2,R);	
	RBase = GetCenterArc(xy1,xy2,Bulge);
	vAng = ((xy2+xy1)/2 - RBase).Unit();
	xy = RBase + R*vAng.Unit();

	str.Format("R = %s",COMMA(R));
	double len = Dom.GetTextWidth(str);
	if(bSpType)	Dom.GiSiArrowExt(xy.x,xy.y,len/2, -len,3,str);
	else		Dom.GiSiArrowExt(xy.x,xy.y,len/2, len,1,str);

	Dom.Rotate(-ToRadian((180-vCros.GetAngleDegree())-90), xy.x, xy.y);
	Dom.Rotate(ToRadian((180-vCros.GetAngleDegree())-90),0,0);
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) ||
	   (!pC->m_nLGirSpType && pC->m_nRGirSpType))
	    Dom.Move(0,0);
	else
		Dom.Move(-dWebToWeb-(dWebT+dWebTR)/2, 0);
	*pDom << Dom;

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	if(bSpType)
	{
		xy1.x = -(pC->m_P_CR+dWebTR/2) * 2/cos(Angle);
		xy1.y = -M-Dom.Always(5);
		xy2.x = -(pC->m_P_CR+dWebTR/2) * 2/cos(Angle);
		xy2.y = M+Dom.Always(5);
	}
	else
	{
		xy1.x = -(dWebToWeb-pC->m_P_CR*2)/cos(Angle);
		xy1.y = -M-Dom.Always(5);
		xy2.x = -(dWebToWeb-pC->m_P_CR*2)/cos(Angle);
		xy2.y = M+Dom.Always(5);
	}
	Dom.CutLightning(xy1.x,xy1.y,xy2.x,xy2.y,TRUE,0.5);
	
	*pDom << Dom;

}

void CAPlateOutCrossBeam::DimDetailCrossVStiff(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	COptionStd	    *pDomOpt  = pStd->GetDomOptionStd();
	CPlateGirderApp	*pGir     = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp	*pGirR    = (CPlateGirderApp*)pGir->m_pRight;
	CARoadOptionStd *pOpt	  = pStd->GetDataManage()->GetOptionStd();
	CPlateCrossBeam *pC    = pBx->GetSection()->GetCrossBeam();
	CGlobarOption   *pGlopt= pBridge->m_pDataManage->GetGlobalOption();

	if(pC == NULL)	return;
	
	//가로보
	double height = pC->m_H_CR;		//가로보 높이
	double upwide = pC->m_uM_CR;	//상부플렌지 너비/2
	double downwide = pC->m_dM_CR;	//하부플렌지 너비/2
	double up_t	= pC->m_T2_CR;	//상부플렌지 두께
	double down_t = pC->m_T1_CR;		//하부플렌지 두께
	double web_t = pC->m_T3_CR;		//복부판 두께
	double VerH = upwide - (pC->m_vW_CR + web_t/2);	//상부플렌지 너비/2 와 수직보강재 너비의 차이

	double VerW = pC->m_vW_CR;	//수직보강재 너비
	double VerL = pC->m_vL_CR;	//수직보강재의 길이
	double VerT = pC->m_vT_CR;

	double VerSlopL = VerW-10;					//수직보강재 경사부분 높이 및 폭 

	double  Dist	= pGir->GetDisFrontPlateWebToWeb(pBx)/2;
	CDPoint cAng	= pBx->GetAngle();		//각도
	double  Sta		= pGir->GetStationBySc(pBx->GetStation(),Dist,cAng);
	double  JSlop	= pGir->GetGiulFlangeUpperLowerCamber(Sta);

	CDimDomyunEx DimDom(pDom);
	pOpt->SetEnvType(&DimDom, HCAD_DIML);

	CString str = _T(""), str2 = _T("");
	long nDan = 1;
	if (JSlop !=0)
	{
		DimDom.SetDirection("LEFT");
		str.Format("%0.1f",fabs(JSlop * VerW));
		DimDom.DimMoveTo(-(VerW+VerT/2),-up_t - JSlop*(VerW+VerT/2),nDan++,FALSE,str);
		DimDom.DimLineTo(-(VerW+VerT/2),JSlop*(VerW+VerT/2),"","");
		DimDom.SetDirection("RIGHT");
		DimDom.DimMoveTo((VerW+VerT/2),-up_t,1,FALSE,str);
		DimDom.DimLineTo((VerW+VerT/2),JSlop*(VerW+VerT/2),"","");
	}

	// 오른쪽
	// 수평보강재 치수
	DimDom.SetDirection("RIGHT");
/*	double Hdis[4], hDisRate[2];
	long nH = 0, Lower = 0;
	nH = pC->GetHStiffUpperDistRate(hDisRate);
	for(long i = 0; i < nH; i++)	Hdis[i] = hDisRate[i];
	Lower = pC->GetHStiffLowerDistRate(hDisRate);
	for(i = 0; i < Lower; i++)		Hdis[nH++] = hDisRate[i];

	double L = 0;
	if(nH+Lower > 0)
	{
		DimDom.DimMoveTo(downwide,-up_t,nDan++);
		for(i = 0; i< nH; i++)
		{
			Hdis[i]=Hdis[i]-L;
			DimDom.DimLineTo(downwide,-Hdis[i],COMMA(Hdis[i]));
			L = L + Hdis[i];
		}
		DimDom.DimLineTo(downwide,-(VerL-L),COMMA(VerL-L));
	}*/
	//전체 치수
	DimDom.DimMoveTo(downwide,-up_t-height,nDan);
	DimDom.DimLineTo(downwide,height-VerL,COMMA(height-VerL),"");
	DimDom.DimLineTo(downwide,VerL,COMMA(VerL),"");

	if (VerL < height)
	{
		DimDom.SetDirection("LEFT");
		DimDom.DimMoveTo(-upwide+VerH,-up_t-VerL,1);
		DimDom.DimLineTo(-upwide+VerH,VerSlopL,COMMA(VerSlopL),"");
		DimDom.SetDirection("BOTTOM");
		DimDom.DimMoveTo(-upwide+VerH+VerSlopL,-up_t-VerL,1);
		DimDom.DimLineTo(-VerSlopL,-up_t-VerL+VerSlopL,COMMA(VerSlopL),"");
	}

	DimDom.SetDirection("TOP");
	str = COMMA(VerH);
	double len = DimDom.GetTextWidth(str);

	DimDom.DimMoveTo(-upwide+VerH,0,1);
	DimDom.DimLineTo(upwide-VerH-web_t/2,0,COMMA(VerW));
	DimDom.DimLineTo(web_t,0,"",COMMA(web_t));
	DimDom.DimLineTo(upwide-VerH-web_t/2,0,COMMA(upwide-VerH-web_t/2));

	*pDom << DimDom;

	// 부재 표현
	long nMarkBuje = pBridge->GetMarkBujeByMarkBujeAndBx(CP_V, pBx);
	DimDom.SetTextAngle(90);
	CVector vJ	= pGir->GetJewonTotal(nMarkBuje,pBx,0,0,DIM_BASIC,TRUE);
	str2 = pBridge->GetMaterialSteelSymbol(nMarkBuje);
	str = pStd->m_pOptStd->GetJewonString(0,vJ.x,vJ.y,vJ.z,str2);
	DimDom.TextOut(-web_t -DimDom.Always(1), -VerL/2, str);
	*pDom << DimDom;

	if(!pC->m_cVSide_CR)
	{
		vJ	= pGir->GetJewonTotal(nMarkBuje,pBx,0,0,DIM_BASIC,FALSE);
		str2 = pBridge->GetMaterialSteelSymbol(nMarkBuje);
		str = pStd->m_pOptStd->GetJewonString(0,vJ.x,vJ.y,vJ.z,str2);
		DimDom.TextOut(web_t + DimDom.Always(1) + DimDom.GetTextHeight(), -VerL/2, str);
		*pDom << DimDom;
	}
	
	DimDom.SetTextAngle(0);

	if (VerL < height)
	{
		DimDom.LineTo(-upwide+VerH-20,-up_t - VerL + VerSlopL+20,
					 -upwide+VerH-100,-up_t - VerL + VerSlopL + 100);
		DimDom.LineTo(-upwide+VerH-100,-up_t - VerL + VerSlopL + 100,
					 -upwide+VerH-100,-up_t - VerL + VerSlopL + 20);
		DimDom.LineTo(-upwide+VerH-100,-up_t - VerL + VerSlopL + 20,
					 -upwide+VerH-20,-up_t - VerL + VerSlopL + 20);
		
		DimDom.TextOut(-upwide+VerH-60,-up_t - VerL + VerSlopL-20 ,"1");
		DimDom.TextOut(-upwide+VerH-120,-up_t - VerL + VerSlopL+50 ,"1");
		*pDom << DimDom;
	}

	if (JSlop >0 && JSlop!=0)
	{
			
		str.Format("%g",VerW);
		double texth = DimDom.GetTextHeight(TRUE);
		DimDom.TextOut(50,-texth - DimDom.Always(1),str);
		DimDom.LineTo(0,0,100,0);
		
		DimDom.SetTextAlignHorz(TA_LEFT);
		str.Format("%0.1f",VerW*JSlop);
		DimDom.TextOut(100+DimDom.Always(1),0,str);
		DimDom.LineTo(100,0,100,texth);
		DimDom.Move(upwide + 30,-up_t+DimDom.Always(10));


	}
	else if (JSlop!=0)
	{
		str.Format("%g",VerW);
		double texth = DimDom.GetTextHeight(TRUE);
		DimDom.TextOut(50,-texth -DimDom.Always(1),str);
		DimDom.LineTo(0,0,100,0);
		DimDom.SetTextAlignHorz(TA_RIGHT);
		str.Format("%0.1f",fabs(VerW*JSlop));
		DimDom.TextOut(-DimDom.Always(1),0,str);
		DimDom.LineTo(0,0,0,texth);
		DimDom.Move(upwide + 120,-up_t+DimDom.Always(10));

	}

	//내측 
	double C = VerT <=16 ? pGir->m_pSangse->m_Chamfer_C1 : pGir->m_pSangse->m_Chamfer_C2;
	double scallopR = VerT <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;

	double sw = (pC->m_cVSide_CR) ? -1 : 1;
	long nDir = (pC->m_cVSide_CR) ? 5 : 7;
	CDPoint txy = CDPoint (sw*web_t/2,-up_t+sw*web_t/2*JSlop);
	CDPoint vAng = pC->m_cVSide_CR ? CDPoint(-1,-1) : CDPoint(1,-1);
	long nMarkBuje1 = pBridge->GetMarkBujeByMarkBujeAndBx(CP_V, pBx);
	long nMarkBuje2 = pBridge->GetMarkBujeByMarkBujeAndBx(CP_W, pBx);
	long nMarkBuje3 = pBridge->GetMarkBujeByMarkBujeAndBx(CP_F_U, pBx);
	long nScallop   = pBridge->GetIndexBaseScallop(nMarkBuje1,nMarkBuje2,nMarkBuje3,pBx); // 스,채,모,없
	nMarkBuje2		= pBridge->GetMarkBujeByMarkBujeAndBx(CP_W, pBx);
	nMarkBuje3		= pBridge->GetMarkBujeByMarkBujeAndBx(CP_F_L, pBx);
	long nScallopLower = pBridge->GetIndexBaseScallop(nMarkBuje1,nMarkBuje2,nMarkBuje3,pBx); // 스,채,모,없
	if (nScallop == 0)
	{
		double MinWeldThick = pGlopt->GetDesignMinLenOfFilletWeld();	//용접 최소 길이
		double len1 = (VerW - scallopR -C)/cos(atan(JSlop));
		if (MinWeldThick > len1)
			nScallop = 2;
	}
	if (nScallopLower == 0)
	{
		double MinWeldThick = pGlopt->GetDesignMinLenOfFilletWeld();	//용접 최소 길이
		double len1 = VerW - scallopR -C;
		if (MinWeldThick > len1)
			nScallopLower = 2;
	}
	len = 0;

	if (nScallop == 0)
	{
		txy = txy + scallopR * vAng.Unit();
		str.Format("R=%.0f",scallopR);
		len = DimDom.GetTextWidth(str);
		DimDom.GiSiArrowExtend(txy.x,txy.y,len,sw*len,nDir,str,"(TYP.)");
	}
	else if (nScallop == 1 || nScallop == 2)
	{
		txy = txy + C*sqrt(2.0)/2 * vAng.Unit();
		str.Format("C=%.0f",C);
		len = DimDom.GetTextWidth(str);
		DimDom.GiSiArrowExtend(txy.x,txy.y,len,sw*len,nDir,str,"(TYP.)");
	}

	// 하부 스캘럽 R
	txy = CDPoint (sw*web_t/2,-height-up_t);
	vAng = pC->m_cVSide_CR ? CDPoint(-1,1) : CDPoint(1,1);
	nDir = (pC->m_cVSide_CR) ? 3 : 1;
	if(VerL == height)
	{
		if (nScallopLower == 0)
		{
			txy = txy + scallopR * vAng.Unit();
			str.Format("R=%.0f",scallopR);
			len = DimDom.GetTextWidth(str);
			DimDom.GiSiArrowExtend(txy.x,txy.y,len,sw*len,nDir,str,"(TYP.)");
		}
		else
		{
			txy = txy + C*sqrt(2.0)/2 * vAng.Unit();
			str.Format("C=%.0f",C);
			len = DimDom.GetTextWidth(str);
			DimDom.GiSiArrowExtend(txy.x,txy.y,len,sw*len,nDir,str,"(TYP.)");
		}
	}

	// 용접표시 : 가로보 복부판+수직보강재
	txy.x = -web_t/2;
	txy.y = -VerL*2/3+web_t/2*JSlop;
	DimDom.Weld(txy.x,txy.y,web_t,VerT,FILLET, TRUE,FALSE);

	// 용접표시 : 가로보 플랜지+수직보강재
	txy.x = -VerW/2;
	txy.y = -up_t-VerW/2*JSlop;
	DimDom.Weld(txy.x,txy.y,down_t,VerT,FILLET, TRUE,FALSE);
	*pDom << DimDom;
}

void CAPlateOutCrossBeam::DrawDetailAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd* pStd   = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt  = pStd->GetDataManage()->GetOptionStd();
	CAPlateDrawEtc DrawEtc(pStd->GetDataManage());	
	if(pBx->GetSection()->GetCrossBeam()->m_nAnchorsu_CR <= 0) return;
	double AnchorDia = pBx->GetGirder()->m_pSangse->m_SlabAnchor_T;

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	DrawEtc.AddAnchorBarDetailCrBeam(&Dom, pBx, Dom.GetScaleDim(), AnchorDia);

	*pDom << Dom;
}


void CAPlateOutCrossBeam::DrawDetailBKSupport(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	CAPlateDrawStd* pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd* pOpt = pStd->GetDataManage()->GetOptionStd();
	CAPlateDrawCross DrawCross(pStd->GetDataManage());	

	long nMarkBuje = pBx->IsJijum() ? CP_BR_GA_FA : CG_BR_GA_FA;
	long nMarkBuje2 = pBx->IsJijum() ? CP_BR_GA : CG_BR_GA;
	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	long nCol = bLeft ? 0 : 1;
	double Len   = pGir->GetLengthJewon(nMarkBuje,pBx,0,nCol);
	double Thick = pGir->GetThickJewon(nMarkBuje2,pBx,0,nCol);
	DrawCross.DstDrawGassetSupPlate(&Dom, pBx, Len, Thick, bLeft);
	DimGassetSupPlateDetail(&Dom, pBx, Len, Thick, bLeft);
	
	*pDom << Dom;
}
void CAPlateOutCrossBeam::DimGassetSupPlateDetail(CDomyun *pDom, CPlateBasicIndex *pBx, double Length, double Thick, BOOL bLeft)
{
	CAPlateDrawStd* pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGirC = pDB->GetGirder(-1);
	CARoadOptionStd* pOpt = pStd->GetDataManage()->GetOptionStd();

	int nCol = bLeft?0:1;
	long nID=0;
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음

	int nGen = (int)pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_GEN);
	double uA = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UA);
	double uB = nGen==0 ? pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UB) : 0;
	double uC = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UC);
	double dA = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DA);
	double dB = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DB);
	double dC = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DC);
	double  W = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_W);
	double dFW = pC->m_dM_CR;

	CDimDomyunEx DimDom(pDom);
	DimDom.SetScaleDim(DimDom.GetScaleDim());
	pOpt->SetEnvType(&DimDom,HCAD_DIML);

	long DimDan   = 1;
	double xP = -W/2;
	double yP =  dB;
	BOOL bDimMark = FALSE;
	DimDom.SetDirection("LEFT");

	yP =    0;

	DimDom.DimMoveTo(xP,yP,DimDan);
	DimDom.DimLineTo(xP,dB,COMMA(dB));
	if(nGen==0)
	{
		if (dA != 0)
			DimDom.DimLineTo(xP,dA,COMMA(dA));
		DimDom.DimLineTo(xP,Length-dB-uB-dA-uA,COMMA(Length-dB-uB-dA-uA));
		if (uA != 0)
			DimDom.DimLineTo(xP,uA,COMMA(uA));
		DimDom.DimLineTo(xP,uB,COMMA(uB));
	}
	else
	{
		DimDom.DimLineTo(xP,Length-dB,COMMA(Length-dB));
	}

	yP =    0;
	DimDan++;
	DimDom.DimMoveTo(xP,yP,DimDan);
	DimDom.DimLineTo(xP,Length,COMMA(Length));

	DimDom.SetDirection("TOP");
	yP = Length+uB;
	DimDan = 1;
	if(nGen==0)
	{
		xP = -W/2;
		DimDom.DimMoveTo(xP    ,yP,DimDan,bDimMark,COMMA(W/2-uC));
		DimDom.DimLineTo(W/2-uC,yP,"","",bDimMark);
		DimDom.DimLineTo(uC*2  ,yP,COMMA(uC*2),"",bDimMark);
		DimDom.DimLineTo(W/2-uC,yP,COMMA(W/2-uC),"",bDimMark,TRUE);
	}
	else
	{
		xP = -dFW;
		DimDom.DimMoveTo(xP    ,yP,DimDan);
		DimDom.DimLineTo(dFW*2,yP,COMMA(dFW*2));
	}
	
	DimDom.SetDirection("BOTTOM");
	yP = -dB;
	DimDan = 1;
	if(nGen==0)
	{
		xP = -W/2;
		DimDom.DimMoveTo(xP         ,yP,DimDan,bDimMark);
		DimDom.DimLineTo(W/2-Thick/2,yP,COMMA(W/2-Thick/2),"",bDimMark);
		DimDom.DimLineTo(Thick,yP,"",COMMA(Thick),bDimMark);
		DimDom.DimLineTo(W/2-Thick/2,yP,COMMA(W/2-Thick/2),"",bDimMark);
		//
		DimDan++;
		DimDom.DimMoveTo(xP,yP,DimDan,bDimMark);
		DimDom.DimLineTo(W ,yP,COMMA(W),"",bDimMark);
	}
	else
	{
		DimDom.DimMoveTo(-dC, yP, DimDan, bDimMark, COMMA(dC-Thick/2));
		DimDom.DimLineTo(dC-Thick/2, yP, "", "", bDimMark);
		DimDom.DimLineTo(Thick,yP,"",COMMA(Thick),bDimMark);
		DimDom.DimLineTo(dC-Thick/2, yP,COMMA(dC-Thick/2),"", bDimMark, TRUE);
	}

	DimDom.Move(0,-Length);

	*pDom << DimDom;
}

void CAPlateOutCrossBeam::DrawDetailCrossBKScallop(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateGirderApp* pGirC = pDB->GetGirder(-1);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd* pOpt = pStd->GetDataManage()->GetOptionStd();
	CGlobarOption  *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	CAPlateDrawSebu DrawSebu(pStd->GetDataManage());
	CDPoint A[10], vAng;
	pC->GetCrossXyDom(A);
	vAng = CDPoint (0,1);

	CDomyun Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	double scallopR = pC->m_T3_CR <=16 ? pGirC->m_pSangse->m_Scallop_R1 : pGirC->m_pSangse->m_Scallop_R2;
	double scallopC = pC->m_T3_CR <=16 ? pGirC->m_pSangse->m_Chamfer_C1 : pGirC->m_pSangse->m_Chamfer_C2;
	double slop1  = 0;
	double slop2  = pGir->GetGiulPlateCrossGusset(pBx);
	double Thick1 = pGir->GetThickFactByStation(pBx->GetStation(), 0);
	double Thick2 = pC->m_T2_CR;
	double WebT   = pGir->GetThickFactByStation(pBx->GetStation(), 2);
//	double Length = pGirR->m_dULeftSharp/vAng.y - WebT;
	double Length = pGirR->GetLengthSharpOnBx(pBx, TRUE) - WebT/2;

	// 확장스캘럽으로 처리할지를 판단
	double dLenFilletMin = pGlopt->GetDesignMinLenOfFilletWeld();
	double dLenFilletLen = 0;
	BOOL   bExtScallop	 = TRUE;
	long nMarkBuje = pBx->IsJijum() ? CP_BR_W : CG_BR_W;
	long nScallop  = pDB->GetIndexBaseScallop(nMarkBuje,G_W,G_F_U,pBx); // 스,채,모,없
	double sR = scallopR;
	if (nScallop != 0)
		sR = scallopC;

	dLenFilletLen   = pGir->GetLengthSharpOnBx(pBx, TRUE) - WebT/2 - (scallopR + sR);
	bExtScallop	    = (dLenFilletLen<dLenFilletMin) ? TRUE : FALSE;	

	DrawSebu.DrawScallopExtend(&Dom, pBx, 0, 0, scallopR,Length,slop1,slop2,Thick1,Thick2,WebT,FALSE,bExtScallop,nScallop,FALSE, FALSE);
	DimDetailCrossBKScallop(&Dom, pBx,0, 0, scallopR, scallopC, Length,slop1,slop2,Thick1,Thick2,WebT,FALSE,bExtScallop,nScallop);
	*pDom << Dom;
}

// 확장 스캘럽표현하기 : 치수선..
void CAPlateOutCrossBeam::DimDetailCrossBKScallop(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, double ScallopR, double ScallopC,
									      double Length, double Slop1,  double Slop2,  double Thick1, double Thick2, 
										  double WebT, BOOL bLeft, BOOL bExtend, long nScallop)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd* pOpt = pStd->GetDataManage()->GetOptionStd();

	double sw = bLeft ? 1.0 : -1.0;
	double C = ScallopC;

	CDimDomyunEx *pDim = new CDimDomyunEx(pDom);
	pOpt->SetEnvType(pDim,HCAD_DIML);
	pDim->SetScaleDim(pDom->GetScaleDim());

	////////////////////////////////////////////////////////
	// 스캘럽 치수
	// 상부 치수
	long DimDan = 1;
	pDim->SetDirection("TOP");
	pDim->DimMoveTo(0,0,DimDan,FALSE);
	pDim->DimLineTo(Length*sw,0,COMMA(Length),"",FALSE);

	bLeft ? pDim->SetDirection("RIGHT") : pDim->SetDirection("LEFT");
	double HDiff = Length*Slop1*sw;
	if(HDiff>0) {
		pDim->DimMoveTo(Length*sw,-ScallopR,DimDan,FALSE);
		pDim->DimLineTo(Length*sw,ScallopR,COMMA(ScallopR),"",FALSE);
		if(HDiff)
		pDim->DimLineTo(Length*sw,HDiff,COMMA(HDiff),"",FALSE,TRUE);
	}
	else {
		pDim->DimMoveTo(Length*sw,-ScallopR,DimDan,FALSE);
		pDim->DimLineTo(Length*sw,ScallopR+HDiff,COMMA(ScallopR+HDiff),"",FALSE);
		if(HDiff)
		pDim->DimLineTo(Length*sw,-HDiff,COMMA(-HDiff),"",FALSE,TRUE);
	}
	*pDom << *pDim;

	////////////////////////////////////////////////////////
	// R 지시선
	pDim->SetDirection("TOP");
	CDPoint rp = CDPoint(0,0);
	rp.x = bLeft ? -ScallopR*cos(ConstPi/4) : ScallopR*cos(ConstPi/4);
	rp.y = -ScallopR*cos(ConstPi/4);
	CString str = _T("");
	str.Format("R=%.0f",ScallopR);
	long Dir = bLeft ? 4 : 0;
	double Dis1 = bLeft ? pDim->Always(15)  : pDim->Always(0);
	pDim->GiSiArrowExt(rp.x, rp.y, Dis1, pDim->GetTextWidth(str), Dir, str);
	pDim->Rotate(ConstPi/4*sw,rp.x,rp.y);
	pDim->LineTo(rp.x,rp.y,0,0);
	*pDom << *pDim;
	if (!bExtend)
	{
		if (nScallop == 0)
		{
			CDPoint vAng = bLeft ?CDPoint(-1,-1) : CDPoint(1,-1);
			rp = CDPoint(Length*sw,0);
			CDPoint rp2;
			rp2 = rp + ScallopR * vAng.Unit();
			CString str = _T("");
			str.Format("R=%.0f",ScallopR);
			long Dir = bLeft ? 4 : 0;
			double Dis1 = bLeft ? pDim->Always(15)  : pDim->Always(0);
			pDim->GiSiArrowExt(rp2.x, rp2.y, Dis1, pDim->GetTextWidth(str), Dir, str);
			pDim->Rotate(ConstPi/4*sw,rp2.x,rp2.y);
			pDim->LineTo(rp.x,rp.y,rp2.x,rp2.y);
			*pDom << *pDim;
		}
		else if (nScallop == 1 || nScallop == 2)
		{
			CDPoint vAng = bLeft ?CDPoint(-1,-1) : CDPoint(1,-1);
			rp = CDPoint(Length*sw,0);
			CDPoint rp2;
			rp2 = rp + C*sqrt(0.5) * vAng.Unit();
			CString str = _T("");
			str.Format("C=%.0f",C);
			long Dir = bLeft ? 4 : 0;
			double Dis1 = bLeft ? pDim->Always(20)  : pDim->Always(0);
			pDim->GiSiArrowExt(rp2.x, rp2.y, Dis1, pDim->GetTextWidth(str), Dir, str);
			pDim->Rotate(ConstPi/4*sw,rp2.x,rp2.y);
			*pDom << *pDim;
		}
	}

	CAPlateSymbolDom SymDom(pDom,  pStd->GetDataManage());	
	pOpt->SetEnvType(&SymDom,HCAD_DIML);
	// 용접 MARK
	double Height  = 12;
	CDPoint xy;
	xy = CDPoint(Thick2/2*-sw,Thick2/2);

	long nMarkBuje1 = pBx->IsJijum() ? CP_BR_F_U : CG_BR_F_U;
	long nMarkBuje2 = G_F_U;
	SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,bLeft, TRUE, 0, TRUE,"");

	*pDom << SymDom;
	delete pDim;
}

void CAPlateOutCrossBeam::AddDetailBMOneType(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CDataManage		*pDataManage	= pStd->GetDataManage();
	COptionStd		*pDomOpt		= pStd->GetDomOptionStd();
	CBMCreator		*pBMStd			= pDataManage->GetBMStandard();
	CGlobarOption	*pGlopt			= pDataManage->GetGlobalOption();

	double dScale = GetScale();
	CGridDomyunEx Dom(pDom);
	Dom.SetScaleDim(dScale);

	CPtrList pList;
	pBMStd->GetBMListCrossBeam(&pList, pBx);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);

	double dTitleWidth[] = {Dom.Always(35), Dom.Always(18), Dom.Always(18), Dom.Always(18),  
						    Dom.Always(18), Dom.Always(22), Dom.Always(24), Dom.Always(22)};
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER; 
	long nCols  = 8;
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetColumnCount(nCols);	
	Dom.SetRowCount(pList.GetCount()+2);
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

	POSITION pos = pList.GetHeadPosition();
	double dTotalSum = 0;
	for(i=0; i<pList.GetCount(); i++)
	{
		CBMRecord *pRec = (CBMRecord *)pList.GetNext(pos);
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
	Dom.SetTextMatrix(pList.GetCount()+1,0, "소 계", GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(pList.GetCount()+1,6, str,GRID_ALIGN_RIGHT| GRID_ALIGN_VCENTER);
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	AhPtrListDelete(pList,(CBMRecord*)0);

	*pDom << Dom;
}

void CAPlateOutCrossBeam::DimFrontCrossMainTop(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGirR->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;

	CDPoint A[10], B[10], vX(1,0), vAng = CDPoint(0,1);
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	double Angle = ToRadian(90) - vAng.GetAngleRadian();
	double WebT   = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double WebTN   = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
	
	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	CString str = _T("");
	double  Len = 0;

	long DimDan   = 1;
	Dom.SetDirection("TOP");
///////////////////////////////////////////////////////
// 1단 : ANCHOR BAR ///////////////////////////////////
	CDPoint BP = CDPoint(A[8].x, max(B[2].y,B[7].y));
	str.Format("ANCHOR BAR %0.lf", WebT);
	Dom.DimBasePoint(BP,DimDan);
	Dom.DimMoveTo(BP, DimDan, FALSE, str);
	Dom.DimLineTo(WebT, BP.y, "");
	double GTerm  = pGir->GetDisFrontPlateWebToWeb(pBx);
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;

	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y,COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE)), BP.y, 
			    COMMA(GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE))));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle)));
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y,COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE), BP.y, COMMA(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE)));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle)));
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y,COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE), BP.y, COMMA(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE)));
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
	}
	else
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-pC->m_P_CR*2-dWSp, BP.y, COMMA(GTerm-Len-pC->m_P_CR*2-dWSp));
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
	}
	Dom.DimLineTo(WebTN, BP.y, COMMA(WebTN));
///////////////////////////////////////////////////////
// 2단 : V-STIFFNER ///////////////////////////////////
	DimDan++;
	Len = 0;
	str.Format("V-STIFFNER %0.lf", WebT);
	Dom.DimBasePoint(BP,DimDan);
	Dom.DimMoveTo(BP, DimDan,FALSE,str);
	Dom.DimLineTo(WebT, BP.y, "");
	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE)))/cos(Angle), BP.y, 
			    COMMA((GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE)))/cos(Angle)));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)));
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle), BP.y, COMMA((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle)));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)));
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle), BP.y, COMMA((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle)));
		Dom.DimLineTo(pC->m_P_CR+dWSp/2/cos(Angle), BP.y, COMMA(pC->m_P_CR+dWSp/2));
	}
	else
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-pC->m_P_CR*2-dWSp)/cos(Angle), BP.y, COMMA((GTerm-Len-pC->m_P_CR*2-dWSp)/cos(Angle)));
		Dom.DimLineTo(pC->m_P_CR+dWSp/2/cos(Angle), BP.y, COMMA(pC->m_P_CR+dWSp/2));
	}
	Dom.DimLineTo(WebTN, BP.y, COMMA(WebTN));
///////////////////////////////////////////////////////
// 3단 : 가로보     ///////////////////////////////////
	DimDan++;
	Dom.DimBasePoint(BP,DimDan);
	Dom.DimMoveTo(BP, DimDan,FALSE, COMMA(WebT));
	Dom.DimLineTo(WebT, BP.y, "");
	Len = pGir->GetLengthSharpOnBx(pBx, TRUE)/cos(Angle);
	double LenR = pGirR->GetLengthSharpOnBx(pBxR, TRUE)/cos(Angle);
	double dWebToWeb = pGir->GetDisPlanPlateWebToWeb(pBx);
	double dSharpToSharp = dWebToWeb - Len/cos(Angle) - LenR/cos(Angle);
	Dom.DimLineTo(Len,BP.y,COMMA(Len));
	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo(dSharpToSharp,BP.y,COMMA(dSharpToSharp));
		Dom.DimLineTo(LenR/cos(Angle),BP.y,COMMA(LenR/cos(Angle)));
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo((pC->m_P_CR-Len)/cos(Angle), BP.y, COMMA((pC->m_P_CR-Len)/cos(Angle)));
		Dom.DimLineTo(dWebToWeb-(pC->m_P_CR+LenR)/cos(Angle),BP.y
			         ,COMMA(dWebToWeb-(pC->m_P_CR+LenR)/cos(Angle)));
		Dom.DimLineTo(LenR,BP.y,COMMA(LenR));
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		Dom.DimLineTo(dWebToWeb-(pC->m_P_CR+Len)/cos(Angle), BP.y, 
			         COMMA(dWebToWeb-(pC->m_P_CR+Len)/cos(Angle)));
		Dom.DimLineTo((pC->m_P_CR-LenR)/cos(Angle),BP.y,COMMA((pC->m_P_CR-LenR)/cos(Angle)));
		Dom.DimLineTo(LenR/cos(Angle), BP.y, COMMA(LenR/cos(Angle)));
	}
	else
	{
		Dom.DimLineTo((pC->m_P_CR-Len)/cos(Angle), BP.y, COMMA((pC->m_P_CR-Len)/cos(Angle)));
		Dom.DimLineTo(dWebToWeb-pC->m_P_CR*2/cos(Angle), BP.y, 
			          COMMA(dWebToWeb-pC->m_P_CR*2/cos(Angle)));
		Dom.DimLineTo((pC->m_P_CR-LenR)/cos(Angle), BP.y, COMMA((pC->m_P_CR-LenR)/cos(Angle)));
		Dom.DimLineTo(LenR/cos(Angle),BP.y,COMMA(LenR/cos(Angle)));
	}
	Dom.DimLineTo(WebTN, BP.y, COMMA(WebTN));

	*pDom << Dom;
}

void CAPlateOutCrossBeam::DimFrontCrossMainLeft(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("LEFT");

	double dFLTU = pGir->GetThickFactByStation(pBx->GetStation(), 0);
	double dFLTD = pGir->GetThickFactByStation(pBx->GetStation(), 1);

	CDPoint A[10], B[10];;
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	long   DimDan    = 1;
	double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	// 1ST
	Dom.DimMoveTo(B[1].x, B[1].y-dHGir-dFLTD, DimDan, TRUE);
	Dom.DimLineTo(B[1].x, dFLTD, COMMA(dFLTD));
	double dT1 = pC->m_T1_CR;
	if(pC->m_dLH1<=0)
		dT1 = 0;
	double dis = dHGir - (pC->m_dLH1+dT1+pC->m_T2_CR+pC->m_H_CR);
	Dom.DimLineTo(B[1].x, dis,COMMA(dis));
	Dom.DimLineTo(B[1].x, pC->m_T1_CR, COMMA(pC->m_T1_CR));
	Dom.DimLineTo(B[1].x, pC->m_H_CR,  COMMA(pC->m_H_CR));
	if(pC->m_dLH1>0)
	{
		Dom.DimLineTo(B[1].x, pC->m_T2_CR, COMMA(pC->m_T2_CR));
		Dom.DimLineTo(B[1].x, pC->m_dLH1, COMMA(pC->m_dLH1));
	}
	Dom.DimLineTo(B[1].x, dFLTU, COMMA(dFLTU));
	// 2ND
	DimDan=2;
	Dom.DimMoveTo(B[1].x, B[1].y-dHGir-dFLTD, DimDan, TRUE, "");
	Dom.DimLineTo(B[1].x, dFLTD, COMMA(dFLTD));
	Dom.DimLineTo(B[1].x, dHGir, COMMA(dHGir));
	Dom.DimLineTo(B[1].x, dFLTU, COMMA(dFLTU));

	*pDom << Dom;
}
void CAPlateOutCrossBeam::DimFrontCrossMainRight(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("RIGHT");

	double dFLTU = pGirR->GetThickFactByStation(pBxR->GetStation(), 0);
	double dFLTD = pGirR->GetThickFactByStation(pBxR->GetStation(), 1);
	double dWGirR = pGirR->GetWidthOnStation(pBxR->GetStation(), TRUE);
	CDPoint A[10], B[10], vX(1,0);
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	long   DimDan    = 1;
	double dHGir = pGirR->GetHeightGirderByStaAng(pBxR->GetStation());
	// 1ST
	CDPoint xyStt = B[7]+vX*dWGirR;
	Dom.DimMoveTo(xyStt.x, xyStt.y-dHGir-dFLTD,DimDan);
	double dT1 = pC->m_T1_CR;
	if(pC->m_dRH1<=0)
		dT1 = 0;
	double dis = dHGir - (pC->m_dRH1+dT1+pC->m_T2_CR+pC->m_H_CR);
	Dom.DimLineTo(xyStt.x, dFLTD,COMMA(dFLTD));
	Dom.DimLineTo(xyStt.x, dis,COMMA(dis));
	Dom.DimLineTo(xyStt.x, pC->m_T1_CR, COMMA(pC->m_T1_CR));
	Dom.DimLineTo(xyStt.x, pC->m_H_CR,  COMMA(pC->m_H_CR));
	if(pC->m_dRH1>0)
	{
		Dom.DimLineTo(xyStt.x, pC->m_T2_CR, COMMA(pC->m_T2_CR));
		Dom.DimLineTo(xyStt.x, pC->m_dRH1, COMMA(pC->m_dLH1));
	}
	Dom.DimLineTo(xyStt.x, dFLTU, COMMA(dFLTU));
	// 2ND
	DimDan=2;
	Dom.DimMoveTo(xyStt.x, xyStt.y-dHGir-dFLTD, DimDan, TRUE, "");
	Dom.DimLineTo(xyStt.x, dFLTD, COMMA(dFLTD));
	Dom.DimLineTo(xyStt.x, dHGir, COMMA(dHGir));
	Dom.DimLineTo(xyStt.x, dFLTU, COMMA(dFLTU));

	*pDom << Dom;
}
void CAPlateOutCrossBeam::DimTextFrontSlope(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateBridgeApp* pBridge = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	CGlobarOption *pGlopt  = pBridge->m_pDataManage->GetGlobalOption();
	long nLevel = pGlopt->GetStrDesignCrossBeamBracketType()=="일반타입" ? 0 : 1;
	if(pC == NULL) return;

	CDPoint A[10], B[10], vX(1,0);
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	double deltaX = nLevel==1 ?  A[4].x-A[3].x : A[5].x-A[2].x;
	double deltaY = nLevel==1 ?  A[4].y-A[3].y : A[5].y-A[2].y;
	double sX = (A[4].x+A[3].x)/2;
	double sY = (A[4].y+A[3].y)/2-(pC->m_H_CR+pC->m_T1_CR);
	double CrossSlop = pGir->GetGiulCrossBeam(pBx);

	CDimDomyunEx DimDom(pDom);
	pStd->m_pOptStd->SetEnvType(&DimDom,HCAD_DIML);

	double Len = DimDom.Always(20);
	double Hgt = DimDom.Always(6);

	CString str;
	if (CrossSlop >=0)
	{
		DimDom.MoveTo(sX-Len/2, sY-Hgt*2);
		DimDom.LineToEx(Len, 0);
		DimDom.LineToEx(0, Hgt);
		DimDom.LineToEx(-Len, -Hgt);
		//
		DimDom.SetTextAlignHorz(TA_LEFT);
		str.Format("%.f ",fabs(deltaY));
		DimDom.TextOut(sX+Len/2+DimDom.Always(1),sY-Hgt*2+Hgt/2-DimDom.GetTextHeight()/2,str);
	}
	else
	{
		DimDom.MoveTo(sX-Len/2, sY-Hgt*2);
		DimDom.LineToEx(Len, 0);
		DimDom.LineToEx(-Len, Hgt);
		DimDom.LineToEx(0, -Hgt);
		//
		DimDom.SetTextAlignHorz(TA_RIGHT);
		str.Format("%.f ",fabs(deltaY));
		DimDom.TextOut(sX-Len/2-DimDom.Always(1),sY-Hgt*2+Hgt/2-DimDom.GetTextHeight()/2,str);
	}

	DimDom.SetTextAlignHorz(TA_CENTER);
	str.Format("%.f",deltaX);
	DimDom.TextOut(sX,sY-Hgt*2-DimDom.GetTextHeight()-DimDom.Always(1),str);

	*pDom << DimDom;
}



void CAPlateOutCrossBeam::DimFrontCrossJewonText(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd  *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB     = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CPlateGirderApp *pGirStt = pDB->GetGirder(0);
	CPlateGirderApp *pGirEnd = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateCrossBeam *pC      = pBx->GetSection()->GetCrossBeam();
	CPlateSangSeDo  *pSang   = pDB->GetGirder(-1)->m_pSangse;
	CGlobarOption   *pGlopt  = pStd->GetDataManage()->GetGlobalOption();

	if(pC == NULL) return;
	
	CDPoint A[10], B[10], C[10], D[10], vX(1,0), vAng(0,1), xy1, xy2;
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	pGirStt->GetHunchXyDom(pBx, C);
	pGirEnd->GetHunchXyDom(pBx, D);

	CDimDomyunEx	Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.SetTextAlignHorz(TA_CENTER);
	//가로보 상하부 플랜지 
	double dHGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation()); 	
	CDPoint ptPos(0,0);
	ptPos.x = (A[8].x+A[9].x)/2;//A[1].x + pC->m_P_CR/2;;
	ptPos.y = min(A[8].y,A[9].y);//C[2].y>D[7].y ? D[7].y : C[2].y;
	ptPos.y -= dHGir;//+Dom.Always(15); 
	//주 복부판
	CVector vJ    = pGir->GetJewonTotal(CP_W,pBx);
	CString sMark = pDB->GetMaterialSteelSymbol(CP_W);
	CString szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	CString str = "";
	if(pC->GetType()==CR_TYPE_HSTEEL)	str.Format("1-H %s", pGir->GetStringCrossHSteelJewon(pBx));
	else								str.Format("1-WEB PL.%s", szJewon);

	Dom.TextOut(ptPos.x, ptPos.y, str);
	ptPos.y -= Dom.GetTextHeight()*1.5;
	//수직보강재
	if(pC->m_nVStiffsu_CR>0)
	{
		long nQty = pC->m_nVStiffsu_CR;
		vJ    = pGir->GetJewonTotal(CP_V, pBx);
		sMark = pDB->GetMaterialSteelSymbol(CP_V);
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-STIFF PL.%s", nQty, szJewon);
		Dom.TextOut(ptPos.x, ptPos.y, str);
		ptPos.y -= Dom.GetTextHeight()*1.5;
	}
	//수평 보강재 
	if(	(pC->m_nHStiffUppersu_CR >0) || (pC->m_nHStiffLowersu_CR >0))
	{
		// 강재량 체크부분이랑 동일한 형태가 되어야함.
		CDPoint Po[18];
		BOOL bIncludeGasset = pGlopt->GetDesignCrossBeamHStiffAtBracket()==0 ? TRUE : FALSE;
		long nQty = pC->m_cVSide_CR==0 ? 2 : 1;	//양면수직보강재 이면 수평보강재는 2 그렇지 않으면 1
		long nV = 0;
		BOOL bFront = pC->m_cVSide_CR==2 ? FALSE : TRUE;  
		if (bFront)		// 양면, 전면
			nV = pGir->GetPointVStiffOnCrossBeamPrv(pBx, Po, FALSE, bIncludeGasset);
		else			// 후면
			nV = pGir->GetPointVStiffOnCrossBeam(pBx, Po, FALSE, bIncludeGasset);

		long nH = pC->m_nHStiffUppersu_CR + pC->m_nHStiffLowersu_CR;	// 수평보강재 수
		long nColStt = bIncludeGasset&&pC->m_nLGirSpType ? 0 : 1;
		long nColEnd = bIncludeGasset&&pC->m_nRGirSpType ? nV-1 : nV-2;

		for(long nRow=0; nRow<nH; nRow++)	// 수평보강재 단수
		{	
			for(long nCol=nColStt; nCol<nColEnd; nCol++)	// 수평보강재 컬럼별수.
			{
				vJ    = pGir->GetJewonTotal(CP_H, pBx, nRow, nCol, DIM_BASIC, bFront);
				sMark = pDB->GetMaterialSteelSymbol(CP_H);
				szJewon = pStd->m_pOptStd->GetJewonString(1, vJ.x, vJ.y, vJ.z);
				str.Format("%d-STIFF PL.%s", nQty, szJewon);
				Dom.TextOut(ptPos.x, ptPos.y, str);
				ptPos.y -= Dom.GetTextHeight()*1.5;
			}
		}
	}
	//브라켓복부판
	if((pC->m_nLGirSpType || pC->m_nRGirSpType) && pC->GetType()==CR_TYPE_MANUFACTURE)
	{
		long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 2 : 1;
		vJ    = pGir->GetJewonTotal(CP_BR_W, pBx);
		sMark = pDB->GetMaterialSteelSymbol(CP_BR_W);
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-WEB PL.%s", nQty, szJewon);
		Dom.TextOut(ptPos.x, ptPos.y, str);
		ptPos.y -= Dom.GetTextHeight()*1.5;
	}
	//브라켓 보강재, 보강판
	if(pC->m_cGasset_CR)
	{	
		long n = 0;
		//보강재
		for(n=0; n<2; n++)
		{
			vJ      = pGir->GetJewonTotal(CP_BR_GA, pBx, 0, n);
			sMark   = pDB->GetMaterialSteelSymbol(CP_BR_GA);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-WEB PL.%s", szJewon);
			Dom.TextOut(ptPos.x, ptPos.y, str);
			ptPos.y -= Dom.GetTextHeight()*1.5;
		}
		//보강판
		for(n=0; n<2; n++)
		{
			vJ      = pGir->GetJewonTotal(CP_BR_GA_FA, pBx, 0, n);
			sMark   = pDB->GetMaterialSteelSymbol(CP_BR_GA_FA);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-FLG PL.%s", szJewon);
			Dom.TextOut(ptPos.x, ptPos.y, str);
			ptPos.y -= Dom.GetTextHeight()*1.5;
		}
	}
	//복부이음판(전단이음판)
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 4 : 2;
		vJ    = pGir->GetJewonTotal(CP_SP_W_SH, pBx);
		sMark = pDB->GetMaterialSteelSymbol(CP_SP_W_SH);
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-SPL PL.%s", nQty, szJewon);
		Dom.TextOut(ptPos.x, ptPos.y, str);
		ptPos.y -= Dom.GetTextHeight()*1.5;
	}
	//복부이음판(모멘트이음판)
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 8 : 4;
		vJ    = pGir->GetJewonTotal(CP_SP_W_MO, pBx);
		if(vJ.x>0 && vJ.y>0 && vJ.z>0)
		{		
			sMark = pDB->GetMaterialSteelSymbol(CP_SP_W_MO);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("%d-SPL PL.%s", nQty, szJewon);
			Dom.TextOut(ptPos.x, ptPos.y, str);
			ptPos.y -= Dom.GetTextHeight()*1.5;
		}
	}
	//볼트
	long nQtySp = (pC->m_sB+1)*2 * (pC->m_sE+1);
	long nQty = 0;
	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
		nQty = nQtySp*2;
	else if((!pC->m_nLGirSpType && pC->m_nRGirSpType) || (pC->m_nLGirSpType && !pC->m_nRGirSpType))
	{
		nQty = nQtySp + pC->m_wE+1; 
		if(pC->m_cGasset_CR)
			nQty += pC->m_ltE+1; 
	}
	else
	{
		nQty = (pC->m_wE+1)*2;
		if(pC->m_cGasset_CR)
			nQty = (pC->m_wE+1 + pC->m_ltE+1)*2; 
	}
	
	vJ      = pGir->GetJewonTotal(CP_BOLT, pBx, 2);
	sMark   = "H.T.B";
	szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	str.Format("%d-%s M%.0fx%.0f(%s)",nQty,sMark,vJ.y,vJ.z,pDB->GetMaterial(CP_BOLT));
	Dom.TextOut(ptPos.x, ptPos.y, str);

	*pDom << Dom;
}
void CAPlateOutCrossBeam::DimFrontBracketBolt(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pBridge  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC    = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;
	
	CDPoint A[10], B[10];
	CDPoint vX(1,0), vY(0,1), xyStt;
 	pC->GetCrossXyDom(A);
 	pGir->GetHunchXyDom(pBx, B);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	CString str;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//좌측가로보열 치수선	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//가로보제원
	long   nCol = 0;
	CDPoint vDir  = pC->GetVectorCross();

	if((!pC->m_nLGirSpType && !pC->m_nRGirSpType) || (!pC->m_nLGirSpType && pC->m_nRGirSpType))
	{
		Dom.SetDirection("RIGHT");
		//복부판 볼트이음
		nCol = 0;
		double dYoundanDisWeb = (pC->m_H_CR-(pC->m_wE*pC->m_wF)-pC->m_dwH)/2;
		xyStt = pC->xyBoltPos[0];
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));
		CString str;
		str.Format("%d@%d=%d", pC->m_wE, pC->m_wF, pC->m_wE*pC->m_wF);
		Dom.DimLineTo(pC->m_wE*pC->m_wF, nCol, str);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));
		Dom.DimLineTo(pC->m_dwH, nCol, COMMA(pC->m_dwH));
		nCol = 1;
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		//브라켓보강재 볼트이음
		if(pC->m_cGasset_CR)
		{
			double dLowerSpace = (pC->m_dltH - pC->m_ltE*pC->m_ltF)/2;
			double dB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
			Dom.DimMoveTo(pC->xyBoltPos[1].x, pC->xyBoltPos[1].y-dLowerSpace, 0, "", TRUE);
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineTo(pC->m_ltE*pC->m_ltF, 0, COMMA(pC->m_ltE*pC->m_ltF));
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineToExtend(dB, 0, "", COMMA(dB));
			Dom.DimMoveTo(pC->xyBoltPos[1].x, pC->xyBoltPos[1].y-dLowerSpace, 1, "", TRUE);
			Dom.DimLineTo(pC->m_dltH+dB, 1, COMMA(pC->m_dltH+dB));
		}
	}	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//우측가로보열 치수선	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	nCol  = 0;
	if((pC->m_nLGirSpType && !pC->m_nRGirSpType))
	{
		Dom.SetDirection("LEFT");
		//복부판 볼트이음
		nCol = 0;
		double dYoundanDisWeb = (pC->m_H_CR-(pC->m_wE*pC->m_wF)-pC->m_dwH)/2;
		xyStt = pC->xyBoltPos[2];
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));
		CString str;
		str.Format("%d@%d=%d", pC->m_wE, pC->m_wF, pC->m_wE*pC->m_wF);
		Dom.DimLineTo(pC->m_wE*pC->m_wF, nCol, str);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));
		Dom.DimLineTo(pC->m_dwH, nCol, COMMA(pC->m_dwH));
		nCol = 1;
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		//브라켓보강재 볼트이음
		if(pC->m_cGasset_CR)
		{
			double dLowerSpace = (pC->m_drtH - pC->m_rtE*pC->m_rtF)/2;
			double dB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
			Dom.DimMoveTo(pC->xyBoltPos[3].x, pC->xyBoltPos[3].y-dLowerSpace, 0, "", TRUE);
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineTo(pC->m_rtE*pC->m_rtF, 0, COMMA(pC->m_rtE*pC->m_rtF));
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineToExtend(dB, 0, "", COMMA(dB));
			Dom.DimMoveTo(pC->xyBoltPos[3].x, pC->xyBoltPos[3].y-dLowerSpace, 1, "", TRUE);
			Dom.DimLineTo(pC->m_drtH+dB, 1, COMMA(pC->m_drtH+dB));
		}
	}
	*pDom << Dom;

}

void CAPlateOutCrossBeam::DimWeldFrontCrossMain(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDPoint A[10], B[10], vX(1,0), vAng = CDPoint(0,1);
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	double Giul = pGir->GetGiulCrossBeamCenter(pBx);
	long nVStiff = pC->m_nVStiffsu_CR;

	CAPlateSymbolDom SymDom(pDom, pStd->m_pDataManage);
	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_DIML);
	// FLANGE 용접
	vAng = pBx->GetAngle();
	double Height  = 10;
	long nMarkBuje1 = pBx->IsJijum() ? CP_BR_F_U : CG_BR_F_U;
	long nMarkBuje2 = G_F_U;
	CDPoint xy(0,0);
	if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		xy.x = A[0].x;
		xy.y = A[0].y+pC->m_T2_CR;
		SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,FALSE, TRUE, 0, TRUE,"");
	}
	else if((!pC->m_nLGirSpType && pC->m_nRGirSpType) || (pC->m_nLGirSpType && pC->m_nRGirSpType))
	{
		xy.x = A[7].x;
		xy.y = A[7].y+pC->m_T2_CR;
		SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,TRUE, TRUE, 0, TRUE,"");
	}
	
	Height  = 8;
	// 수직 보강재
	if(nVStiff > 0)
	{
		nMarkBuje1 = pBx->IsJijum() ? CP_V : CG_V;
		nMarkBuje2 = pBx->IsJijum() ? CP_W : CG_W;
		double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
		CDPoint vCross = pC->GetVectorCross();
		CDPoint xyOr; 
		double webT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
		if(pC->m_nLGirSpType)
		{
			xyOr = A[2]+vCross*dWSp/2/vCross.x;
			xy.x = xyOr.x + pC->m_VStiffDis_CR[0]/vAng.y + pC->m_vT_CR/2;
			xy.y = xyOr.y + (pC->m_VStiffDis_CR[0]/vAng.y + pC->m_vT_CR/2)*Giul - pC->m_H_CR + SymDom.Always(5);
		}
		else
		{
			xyOr = A[8]+CDPoint(1,0)*webT+vCross*pC->GetLengthGirWebToCrWeb(TRUE)/vCross.x; 
			xy.x = xyOr.x + pC->m_VStiffDis_CR[0]/vAng.y + pC->m_vT_CR/2;
			xy.y = xyOr.y + (pC->m_VStiffDis_CR[0]/vAng.y + pC->m_vT_CR/2)*Giul - pC->m_H_CR + SymDom.Always(5);
		}
		SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,FALSE,TRUE, 0, TRUE,"");
	}
	// 상부플랜지
	nMarkBuje1 = pBx->IsJijum() ? CP_F_U : CG_F_U;
	nMarkBuje2 = pBx->IsJijum() ? CP_W : CG_W;
	long Dis     = 0;
	for(long i=0; i< nVStiff ; i++)
	{
		Dis += (long)pC->m_VStiffDis_CR[i];
		if(i >= nVStiff/2)	break;
	}
	if(Dis==0)
		Dis = (long)(A[5].x - A[2].x);
	xy.x = A[2].x + Dis - SymDom.Always(15);
	xy.y = A[2].y + (Dis - SymDom.Always(15))*Giul;
	SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,FALSE,FALSE, 0, TRUE,"");

	// 하부플랜지
	nMarkBuje1 = pBx->IsJijum() ? CP_F_L : CG_F_L;
	nMarkBuje2 = pBx->IsJijum() ? CP_W : CG_W;
	xy.y -= pC->m_H_CR; 
	SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,FALSE,TRUE, 0, TRUE,"");

	// 브라켓 복부판 + 거더Web
	nMarkBuje1 = pBx->IsJijum() ? CP_BR_W : CG_BR_W;
	nMarkBuje2 = G_W;
	if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		xy.x = A[8].x + pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
		xy.y = A[8].y - pC->m_H_CR/2;
		SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,TRUE,TRUE, 0, TRUE,"");
	}
	else if((!pC->m_nLGirSpType && pC->m_nRGirSpType) || (pC->m_nLGirSpType && pC->m_nRGirSpType))
	{
		xy.x = A[9].x - pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
		xy.y = A[9].y - pC->m_H_CR/2;
		SymDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx,-1,Height,0,TRUE,TRUE, 0, TRUE,"");
	}
	*pDom << SymDom;
}

void CAPlateOutCrossBeam::DimFrontCrossGassetSupportLeft(CDomyun *pDom, CPlateBasicIndex *pBx)
{
//	if (!pBx->IsCrossBeamRight()) return;
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirC = (CPlateGirderApp*)pDB->GetGirder(-1);
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDimDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint A[10], B[10], vX(1,0), vAng = CDPoint(0,1), xy1, xy2;
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	if(pC->m_nLGirSpType){
		xy1 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, TRUE, TRUE);
		xy2 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, FALSE, TRUE);
	}
	else
	{
		xy1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, TRUE, TRUE);
		xy2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, FALSE, TRUE);
	}
	double dB = pGirC->m_pSangse->m_GassetPlate_dB;
	double uB = pGirC->m_pSangse->m_GassetPlate_uB;

	CDPoint Ang = (xy2 - xy1).Unit();
	double Dis = ~(xy2 - xy1) - uB - dB;

	Dom.SetDirection("BOTTOM");

	long DimDan = 1;
	Dom.DimMoveTo(xy2,DimDan,FALSE,COMMA(dB));
	Dom.DimLineTo(dB,xy2.y,"");
	Dom.DimLineTo(Dis, xy2.y,COMMA(Dis));
	Dom.DimLineTo(uB,xy2.y,COMMA(uB),"",FALSE,TRUE);

	Dom.Rotate(xy2, -Ang);
	*pDom << Dom;
}

void CAPlateOutCrossBeam::DimFrontCrossGassetSupportRight(CDomyun *pDom, CPlateBasicIndex *pBx)
{
//	if (!pBx->IsCrossBeamRight()) return;
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirC = (CPlateGirderApp*)pDB->GetGirder(-1);
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDimDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint A[10], B[10], vX(1,0), vAng = CDPoint(0,1), xy1, xy2;
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	if(pC->m_nLGirSpType){
		xy1 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, TRUE, FALSE);
		xy2 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, FALSE, FALSE);
	}
	else
	{
		xy1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, TRUE, FALSE);
		xy2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, FALSE, FALSE);
	}
	double dB = pGirC->m_pSangse->m_GassetPlate_dB;
	double uB = pGirC->m_pSangse->m_GassetPlate_uB;
 
	CDPoint Ang = (xy2 - xy1).Unit();
	double Dis = ~(xy2 - xy1) - uB - dB;

	Dom.SetDirection("BOTTOM");

	long DimDan = 1;
	Dom.DimMoveTo(xy2.x - (Dis+uB+dB), xy2.y,DimDan,FALSE,COMMA(uB));
	Dom.DimLineTo(uB,xy2.y,"");
	Dom.DimLineTo(Dis, xy2.y,COMMA(Dis));
	Dom.DimLineTo(dB,xy2.y,COMMA(dB),"",FALSE,TRUE);
	Dom.Rotate(xy2, Ang);

	*pDom << Dom;
} 

void CAPlateOutCrossBeam::DimPlanCrossMainTop(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirR = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxR = (CPlateBasicIndex*)pGirR->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;

	CDPoint A[10], B[10], vX(1,0), vAng = CDPoint(0,1);
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	double Angle = ToRadian(90) - vAng.GetAngleRadian();
	double WebT   = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double WebTN   = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
	
	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	CString str = _T("");

	double  Len = 0;

	long DimDan   = 1;
	Dom.SetDirection("TOP");
///////////////////////////////////////////////////////
// 1단 : ANCHOR BAR ///////////////////////////////////
	CDPoint BP = CDPoint(-WebT/2, pC->m_uM_CR);
//	CDPoint BP = CDPoint(A[8].x, max(B[2].y,B[7].y));
	str.Format("ANCHOR BAR %0.lf", WebT);
	Dom.DimBasePoint(BP,DimDan);
	Dom.DimMoveTo(BP, DimDan, FALSE, str);
	Dom.DimLineTo(WebT, BP.y, "");
	double GTerm  = pGir->GetDisFrontPlateWebToWeb(pBx);
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;

	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y,COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE)), BP.y,
				COMMA(GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE))));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle)));
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y,COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE), BP.y, COMMA(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE)));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle)));
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y,COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE), BP.y, COMMA(GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE)));
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
	}
	else
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nAnsu = pC->m_nAnchorsu_CR;
		for(long i = 0; i < nAnsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_AnchorDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_AnchorDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_AnchorDis_CR[i]/cos(Angle)));
			Len += pC->m_AnchorDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo(GTerm-Len-pC->m_P_CR*2-dWSp, BP.y, COMMA(GTerm-Len-pC->m_P_CR*2-dWSp));
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
	}
	Dom.DimLineTo(WebTN, BP.y, COMMA(WebTN));
///////////////////////////////////////////////////////
// 2단 : V-STIFFNER ///////////////////////////////////
	DimDan++;
	Len = 0;
	str.Format("V-STIFFNER %0.lf", WebT);
	Dom.DimBasePoint(BP,DimDan);
	Dom.DimMoveTo(BP, DimDan,FALSE,str);
	Dom.DimLineTo(WebT, BP.y, "");
	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE)))/cos(Angle), BP.y, 
			    COMMA((GTerm-Len-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE)))/cos(Angle)));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)));
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle), BP.y, COMMA((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle)));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(FALSE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(FALSE)));
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle), BP.y, COMMA(pC->GetLengthGirWebToCrWeb(TRUE)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle), BP.y, COMMA((GTerm-Len-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE))/cos(Angle)));
		Dom.DimLineTo(pC->m_P_CR+dWSp/2/cos(Angle), BP.y, COMMA(pC->m_P_CR+dWSp/2));
	}
	else
	{
		Dom.DimLineTo((pC->m_P_CR+dWSp/2)/cos(Angle), BP.y, COMMA((pC->m_P_CR+dWSp/2)/cos(Angle)));
		const long nVStiffsu = pC->m_nVStiffsu_CR;
		for(long i = 0; i < nVStiffsu; i++ )
		{
			if( i == 0 )
			{
				Len = pC->m_VStiffDis_CR[i]/cos(Angle);
				Dom.DimLineTo(Len, BP.y, COMMA(Len));
				continue;
			}
			Dom.DimLineTo(pC->m_VStiffDis_CR[i]/cos(Angle), BP.y, COMMA(pC->m_VStiffDis_CR[i]/cos(Angle)));
			Len += pC->m_VStiffDis_CR[i]/cos(Angle);
		}
		Dom.DimLineTo((GTerm-Len-pC->m_P_CR*2-dWSp)/cos(Angle), BP.y, COMMA((GTerm-Len-pC->m_P_CR*2-dWSp)/cos(Angle)));
		Dom.DimLineTo(pC->m_P_CR+dWSp/2/cos(Angle), BP.y, COMMA(pC->m_P_CR+dWSp/2));
	}
	Dom.DimLineTo(WebTN, BP.y, COMMA(WebTN));
///////////////////////////////////////////////////////
// 3단 : 가로보     ///////////////////////////////////
	DimDan++;
	Dom.DimBasePoint(BP,DimDan);
	Dom.DimMoveTo(BP, DimDan,FALSE, COMMA(WebT));
	Dom.DimLineTo(WebT, BP.y, "");
	Len = pGir->GetLengthSharpOnBx(pBx, TRUE)/cos(Angle);
	double LenR = pGirR->GetLengthSharpOnBx(pBxR, FALSE)/cos(Angle);
	double dWebToWeb = pGir->GetDisPlanPlateWebToWeb(pBx);
	double dSharpToSharp = dWebToWeb - Len/cos(Angle) - LenR/cos(Angle);
	Dom.DimLineTo(Len,BP.y,COMMA(Len));
	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo(dSharpToSharp,BP.y,COMMA(dSharpToSharp));
		Dom.DimLineTo(LenR/cos(Angle),BP.y,COMMA(LenR/cos(Angle)));
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		Dom.DimLineTo((pC->m_P_CR-Len)/cos(Angle), BP.y, COMMA((pC->m_P_CR-Len)/cos(Angle)));
		Dom.DimLineTo(dWebToWeb-(pC->m_P_CR+LenR)/cos(Angle),BP.y
			         ,COMMA(dWebToWeb-(pC->m_P_CR+LenR)/cos(Angle)));
		Dom.DimLineTo(LenR,BP.y,COMMA(LenR));
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		Dom.DimLineTo(dWebToWeb-(pC->m_P_CR+Len)/cos(Angle), BP.y, 
			         COMMA(dWebToWeb-(pC->m_P_CR+Len)/cos(Angle)));
		Dom.DimLineTo((pC->m_P_CR-LenR)/cos(Angle),BP.y,COMMA((pC->m_P_CR-LenR)/cos(Angle)));
		Dom.DimLineTo(LenR/cos(Angle), BP.y, COMMA(LenR/cos(Angle)));
	}
	else
	{
		Dom.DimLineTo((pC->m_P_CR-Len)/cos(Angle), BP.y, COMMA((pC->m_P_CR-Len)/cos(Angle)));
		Dom.DimLineTo(dWebToWeb-pC->m_P_CR*2/cos(Angle), BP.y, 
			          COMMA(dWebToWeb-pC->m_P_CR*2/cos(Angle)));
		Dom.DimLineTo((pC->m_P_CR-LenR)/cos(Angle), BP.y, COMMA((pC->m_P_CR-LenR)/cos(Angle)));
		Dom.DimLineTo(LenR/cos(Angle),BP.y,COMMA(LenR/cos(Angle)));
	}
	Dom.DimLineTo(WebTN, BP.y, COMMA(WebTN));

	*pDom << Dom;
}

void CAPlateOutCrossBeam::DimPlanCrossMainSplice(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDimDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CDPoint vAng = pBx->GetAngle();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double Angle = ToRadian(90) - vAng.GetAngleRadian();
	double uA = pC->m_uA;
	long   uB = pC->m_uB;
	double uC = pC->m_uC;
	double uC1 = pC->m_uC1;
	double uD = pC->m_uD;
	long   uE = pC->m_uE;
	double uF = pC->m_uF;
	double uM = pC->m_uM_CR;

	CDPoint BP(0,0);
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) || (pC->m_nLGirSpType && !pC->m_nRGirSpType))
	{
		BP.x = ABS(pC->m_P_CR/cos(Angle)) - uA - uB*uC - uC1/2;
		BP.y = -uM;
	}
	else
	{
		double dWebToWeb = pGir->GetDisPlanPlateWebToWeb(pBx);
		BP.x = dWebToWeb - ABS(pC->m_P_CR/cos(Angle)) - uA - uB*uC - uC1/2;
		BP.y = -uM;
	}
	
	Dom.SetDirection("BOTTOM");

	long DimDan = 1;

	Dom.DimMoveTo(BP,DimDan,FALSE,COMMA(uA));
	Dom.DimLineTo(uA,BP.y,"");
	if (uC == uC1)
		Dom.DimLineTo((uB*2+1)*uC,BP.y,COMMA((double)uB*2+1)+"@"+COMMA(uC),"="+COMMA(((double)uB*2+1)*uC));
	else {
		Dom.DimLineTo(uB*uC,BP.y,COMMA(uB)+"@"+COMMA(uC),"="+COMMA(uB*uC));
		Dom.DimLineTo(uC1,BP.y,COMMA(uC1));
		Dom.DimLineTo(uB*uC,BP.y,COMMA(uB)+"@"+COMMA(uC),"="+COMMA(uB*uC));
	}
	Dom.DimLineTo(uA,BP.y,COMMA(uA),"",FALSE,TRUE);
	//전체치수 
	DimDan++;
	Dom.DimMoveTo(BP,DimDan);
	double Len = (uA+uB*uC+uC1/2)*2;
	Dom.DimLineTo(Len,BP.y,COMMA(Len));

	DimDan=1;
	Dom.SetDirection("RIGHT");
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) || (pC->m_nLGirSpType && !pC->m_nRGirSpType))
	{
		BP.x = pC->m_P_CR/cos(Angle) + uA + uB*uC + uC1/2;
		BP.y = -uM;
	}
	else
	{
		double dWebToWeb = pGir->GetDisPlanPlateWebToWeb(pBx);
		BP.x = dWebToWeb - ABS(pC->m_P_CR/cos(Angle)) + uA + uB*uC + uC1/2;
		BP.y = -uM;
	}

	Dom.DimMoveTo(BP,DimDan,FALSE,COMMA(uD));
	Dom.DimLineTo(BP.x,uD,"");
	if (uE == 0) {
		Len = uM*2 - uD*2;
		Dom.DimLineTo(BP.x, Len,COMMA(Len));
	}
	else {
		if (uE == 1) {
			Len = uE * uF;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
			Len = uM*2 - uD*2 - uE*uF*2;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
			Len =uE*uF;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
		}
		else {
			Len = uE * uF;
			Dom.DimLineTo(BP.x, Len,COMMA((double)uE)+"@"+COMMA(uF),"="+COMMA(Len));
			Len = uM*2 - uD*2 - uE*uF*2;
			Dom.DimLineTo(BP.x, Len,COMMA(Len));
			Len = uE * uF;
			Dom.DimLineTo(BP.x, Len,COMMA((double)uE)+"@"+COMMA(uF),"="+COMMA(Len));
		}
	}
	Dom.DimLineTo(BP.x,uD,COMMA(uD),"",FALSE,TRUE);
	//전체치수
	DimDan++;
	Dom.DimMoveTo(BP,DimDan);
	Dom.DimLineTo(BP.x, uM*2,COMMA(uM*2));
	*pDom << Dom;
}

void CAPlateOutCrossBeam::DimPlanCrossSpliceSide(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;
	
	CDimDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint A[10],vX(1,0), vAng = CDPoint(0,1), xy1, xy2;
	pC->GetCrossXyDom(A);

	double Angle = ToRadian(90) - vAng.GetAngleRadian();
	double uM = pC->m_uM_CR;
	double T3 = pC->m_T3_CR;
	double uA = pC->m_uA;
	long   uB = pC->m_uB;
	double uC = pC->m_uC;
	double uC1 = pC->m_uC1;
	double dSpaceGtoG = pGir->GetDisPlanPlateWebToWeb(pBx);
	double dSpHalf = uA + uB*uC + uC1/2;
	CDPoint BP;
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) || (pC->m_nLGirSpType && !pC->m_nRGirSpType))
		BP.x = pC->m_P_CR/cos(Angle) - dSpHalf;
	else
		BP.x = dSpaceGtoG - pC->m_P_CR/cos(Angle) - dSpHalf;

	BP.y = -uM;
	Dom.SetDirection("LEFT");
	long DimDan = 1;
	Dom.DimMoveTo(BP,DimDan);
	Dom.DimLineTo(BP.x, uM-T3/2,COMMA(uM-T3/2));
	Dom.DimLineTo(BP.x, T3,"",COMMA(T3));
	Dom.DimLineTo(BP.x, uM-T3/2,COMMA(uM-T3/2));

	*pDom << Dom;
}

void CAPlateOutCrossBeam::DimPlanCrossJewonText(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	CPlateSangSeDo*       pSang= pDB->GetGirder(-1)->m_pSangse;
	CGlobarOption *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	if(pC == NULL) return;
	
	CDPoint A[10], vX(1,0), vAng(0,1), xy1, xy2;
	pC->GetCrossXyDom(A);
	double dWGir = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);

	CDimDomyunEx	Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(pDom->GetScaleDim());
	//////////////////////////////////////////////////////////////////////////////////////////
	//가로보 상부 플랜지 
	//////////////////////////////////////////////////////////////////////////////////////////
	CVector vJ    = pGir->GetJewonTotal(CP_F_U,pBx);
	CString sMark = pDB->GetMaterialSteelSymbol(CP_F_U);
	CDPoint vGir  = pDB->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dTextH = 0;
	CDPoint ptPos(dWGir/2,-600); 
	CString str = "상면";
	double dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	dTextH = Dom.GetTextHeight()*1.5;
	ptPos.y -= dTextH;

	CString szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	str.Format("1-FLG PL.%s", szJewon);
	dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	dTextH = Dom.GetTextHeight()*1.5;
	ptPos.y -= dTextH;
	//가로보 상부이음판
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		//상부상면
		long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 2 : 1; 
		vJ    = pGir->GetJewonTotal(CP_SP_U_U, pBx);
		sMark = pDB->GetMaterialSteelSymbol(CP_SP_U_U);
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-SPL PL.%s", nQty, szJewon);
		dTextWidth = Dom.GetTextWidth(str);
		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
		ptPos.y -= dTextH; 
		//상부하면
		nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 4 : 2; 
		vJ    = pGir->GetJewonTotal(CP_SP_U_L, pBx);
		sMark = pDB->GetMaterialSteelSymbol(CP_SP_U_L);
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-SPL PL.%s", nQty, szJewon);
		dTextWidth = Dom.GetTextWidth(str);
		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
		ptPos.y -= dTextH;

	}
	//가로보 슬래브앵커
	if(pC->m_nAnchorsu_CR>0)
	{
/*
		dTextWidth = Dom.GetTextWidth("SLAB ANCHOR");
		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, "SLAB ANCHOR");
		ptPos.y -= dTextH;
	
		CVector vJ = pGir->GetJewonTotal(GI_SA, pBx);
//		str.Format("D%s L=%.0lf %dEA", COMMA(pSang->m_SlabAnchor_T), pSang->m_SlabAnchor_A*2+pSang->m_SlabAnchor_B,  pC->m_nAnchorsu_CR);
		str.Format("D%s L=%.0lf %dEA", COMMA(pSang->m_SlabAnchor_T), vJ.z,  pC->m_nAnchorsu_CR);
		dTextWidth = Dom.GetTextWidth(str);
		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
		ptPos.y -= dTextH;
*/
	}
	//볼트
	long E = pC->m_uE;
	long B = pC->m_uB;
	BOOL bZigZag = FALSE;
	if(pGlopt->GetDesignBoltHoleJigJaegOfHorPlate() == 0)
		bZigZag = TRUE;
	if(E==0) bZigZag = FALSE;
	long nQtySp = (B+1)*2 * (E+1)*2;
	long nQty   = 0;
	if(bZigZag) {
		if (E%2 || B%2)
			nQty = (long)(nQty/2);
		else
			nQty = (long)(nQty/2) + 2;
	}

	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		long nQty = 0;
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			nQty = nQtySp*2;
		else 
			nQty = nQtySp;
		
		vJ    = pGir->GetJewonTotal(CP_BOLT,pBx);
		sMark = "H.T.B";
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-%s M%.0fx%.0f(%s)",nQty,sMark,vJ.y,vJ.z,pDB->GetMaterial(CP_BOLT));
		dTextWidth = Dom.GetTextWidth(str);

		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
		ptPos.y -= dTextH;
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//가로보 하부 플랜지 
	//////////////////////////////////////////////////////////////////////////////////////////
	vJ    = pGir->GetJewonTotal(CP_F_L,pBx);
	sMark = pDB->GetMaterialSteelSymbol(CP_F_L);
	vGir  = pDB->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dSpGtoG = pGir->GetDisFrontPlateWebToWeb(pBx);
	ptPos.x = dSpGtoG/2;
	ptPos.y = -600; 
	str = "하면";
	dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	ptPos.y -= dTextH;

	szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	str.Format("1-FLG PL.%s", szJewon);
	dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	dTextH = Dom.GetTextHeight()*1.5;
	ptPos.y -= dTextH;
	//가로보 하부이음판
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		//하부상면
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
		{
			if(pC->m_cGasset_CR)	nQty = 8;
			else					nQty = 4;	
		}
		else
		{
			if(pC->m_cGasset_CR)	nQty = 4;
			else					nQty = 2;	
		}	
		vJ    = pGir->GetJewonTotal(CP_SP_L_U, pBx);
		sMark = pDB->GetMaterialSteelSymbol(CP_SP_L_U);
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-SPL PL.%s", nQty, szJewon);
		dTextWidth = Dom.GetTextWidth(str);
		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
		ptPos.y -= dTextH;
		//하부하면
		if(!pC->m_cGasset_CR)
		{
			long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 2 : 1; 
			vJ    = pGir->GetJewonTotal(CP_SP_L_L, pBx);
			sMark = pDB->GetMaterialSteelSymbol(CP_SP_L_L);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("%d-SPL PL.%s", nQty, szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
			ptPos.y -= dTextH;
		}
	}
	//볼트
	E = pC->m_dE;
	B = pC->m_dB;
	bZigZag = FALSE;
	if(pGlopt->GetDesignBoltHoleJigJaegOfHorPlate() == 0)
		bZigZag = TRUE;
	if(E==0) bZigZag = FALSE;
	nQtySp = (B+1)*2 * (E+1)*2;
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		long nQty = 0;
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			nQty = nQtySp*2;
		else
			nQty = nQtySp;
		
		vJ    = pGir->GetJewonTotal(CP_BOLT,pBx);
		sMark = "H.T.B";
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-%s M%.0fx%.0f(%s)",nQty,sMark,vJ.y,vJ.z,pDB->GetMaterial(CP_BOLT));
		dTextWidth = Dom.GetTextWidth(str);

		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
	}	
	*pDom << Dom;
}


void CAPlateOutCrossBeam::DimTotalFrontJewonText(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	DimFrontCrossJewonText(pDom, pBx);
}

void CAPlateOutCrossBeam::DimTotalPlanUpperJewonText(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	CPlateSangSeDo*       pSang= pDB->GetGirder(-1)->m_pSangse;
	if(pC == NULL) return;

	CPlateGirderApp	 *pGirR = pGir->m_pRight;
	CPlateBasicIndex *pBxR	= pBx->GetBxMatchByCrossBeam(pGirR);
	
	CDPoint A[10], vX(1,0), vAng(0,1), xy1, xy2;
	pC->GetCrossXyDom(A);

	CDimDomyunEx	Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.SetTextAlignHorz(TA_CENTER);

	CVector vJ    = pGir->GetJewonTotal(CP_F_U,pBx);
	CString sMark = pDB->GetMaterialSteelSymbol(CP_F_U);
	CDPoint vGir  = pDB->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
	//가로보 상부 플랜지 
	double dTextH = 0;
	CDPoint ptPos = pGir->GetXyGirderDis(pBx->GetStation(), 0, CDPoint(0,1));
	CDPoint ptPosR = pGirR->GetXyGirderDis(pBxR->GetStation(), 0, CDPoint(0,1));
	ptPos.x = min(ptPos.x, ptPosR.x);
	
	ptPos.y = (ptPos.y + ptPosR.y)/2 + (Dom.GetDimLevelDis() * (Dom.GetDimDan()+1) + Dom.GetDimVLen()) * Dom.GetScaleDim();

	CString szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	CString str = "";
	if(pC->GetType()!=CR_TYPE_HSTEEL)
		str.Format("1-FLG PL.%s", szJewon);
	//double dTextWidth = Dom.GetTextWidth(str);
	Dom.TextOut(ptPos.x, ptPos.y, str);
	double dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
	Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 
	*pDom << Dom;
	//가로보 상부이음판
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		// 가로보가 주형상판과 이격되어 있는 경우
		BOOL bDividePlate = (pC->m_dLH1 > 0 || pC->m_dRH1 > 0) ? TRUE : FALSE;

		//상부상면
		if(pC->GetType()!=CR_TYPE_HSTEEL && bDividePlate == FALSE)
		{
			dTextH += Dom.GetTextHeight()*1.5;
			long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 2 : 1; 
			vJ    = pGir->GetJewonTotal(CP_SP_U_U, pBx);
			sMark = pDB->GetMaterialSteelSymbol(CP_SP_U_U);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x, vJ.y, vJ.z);
			str.Format("%d-SPL PL.%s", nQty, szJewon);
			Dom.TextOut(ptPos.x, ptPos.y-dTextH, str);
			dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
			Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 
			*pDom << Dom;
		}
		//상부하면
		if(pC->m_nRadioConnectionPlateInOut==0)
		{
			dTextH += Dom.GetTextHeight()*1.5;
			long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 4 : 2; 
			if(bDividePlate)
			{
				nQty *= 2;
			}
			vJ    = pGir->GetJewonTotal(CP_SP_U_L, pBx);
			sMark = pDB->GetMaterialSteelSymbol(CP_SP_U_L);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			if(pC->GetType()==CR_TYPE_HSTEEL)
				nQty += nQty;
			str.Format("%d-SPL PL.%s", nQty, szJewon);
			Dom.TextOut(ptPos.x, ptPos.y-dTextH, str);
			dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
			Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 
			*pDom << Dom;
		}
	}
	//가로보 슬래브앵커
	if(pC->m_nAnchorsu_CR>0)
	{
/*
		dTextH += Dom.GetTextHeight()*1.5;
		ptPos = pGir->GetXyGirderDis(pBx->GetStation()-600-dTextH, pC->m_P_CR/2, CDPoint(0,1));
		dTextWidth = Dom.GetTextWidth("SLAB ANCHOR");
		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, "SLAB ANCHOR");
		dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
		Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 
		*pDom << Dom;

		CVector vJ = pGir->GetJewonTotal(GI_SA, pBx);
		dTextH += Dom.GetTextHeight()*1.5;
		ptPos = pGir->GetXyGirderDis(pBx->GetStation()-600-dTextH, pC->m_P_CR/2, CDPoint(0,1));
//		str.Format("D%s L=%.0lf %dEA", COMMA(pSang->m_SlabAnchor_T), pSang->m_SlabAnchor_A*2+pSang->m_SlabAnchor_B,  pC->m_nAnchorsu_CR);
		str.Format("D%s L=%.0lf %dEA", COMMA(pSang->m_SlabAnchor_T), vJ.z, pC->m_nAnchorsu_CR);
		dTextWidth = Dom.GetTextWidth(str);
		Dom.TextOut(ptPos.x+dTextWidth/2, ptPos.y, str);
		dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
		Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 
		*pDom << Dom;
*/
	}
	//볼트
	long E = pC->m_uE;
	long B = pC->m_uB;

	long E1 = (long)pC->m_uE1;
	long B1 = (long)pC->m_uB1;

	BOOL bZigZag = FALSE;
	CGlobarOption *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	if(pGlopt->GetDesignBoltHoleJigJaegOfHorPlate() == 0)
		bZigZag = TRUE;
	if(E==0) bZigZag = FALSE;
	long nQtyBolt = (B+1)*(E+1)*2*2;
	if(pC->m_bConnectionPlate)
	{
		nQtyBolt += (B1+1)*(E1+1)*2*2;
	}
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		dTextH += Dom.GetTextHeight()*1.5;
		long nQty = 0;
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			nQty = nQtyBolt*2;
		else 
			nQty = nQtyBolt;
		
		vJ    = pGir->GetJewonTotal(CP_BOLT, pBx, 0);
		sMark = "H.T.B";
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-%s M%.0fx%.0f(%s)",nQty,sMark,vJ.y,vJ.z,pDB->GetMaterial(CP_BOLT));

		Dom.TextOut(ptPos.x, ptPos.y-dTextH, str);
		dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
		Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 

		*pDom << Dom;
	}
	
}
void CAPlateOutCrossBeam::DimTotalPlanLowerJewonText(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd*  pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp* pDB  = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	CPlateSangSeDo*       pSang= pDB->GetGirder(-1)->m_pSangse;
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();
	if(pC == NULL) return;

	CPlateGirderApp	 *pGirR = pGir->m_pRight;
	CPlateBasicIndex *pBxR	= pBx->GetBxMatchByCrossBeam(pGirR);
	
	CDPoint A[10], vX(1,0), vAng(0,1), xy1, xy2;
	pC->GetCrossXyDom(A);

	CDimDomyunEx	Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.SetTextAlignHorz(TA_CENTER);

	//가로보 하부 플랜지 
	CVector vJ    = pGir->GetJewonTotal(CP_F_L,pBx);
	CString sMark = pDB->GetMaterialSteelSymbol(CP_F_L);
	CDPoint vGir  = pDB->GetGirder(0)->GetLine()->GetAngleAzimuth(pBx->GetStation());
	
	CDPoint ptPos = pGir->GetXyGirderDis(pBx->GetStation(), 0, CDPoint(0,1));
	CDPoint ptPosR = pGirR->GetXyGirderDis(pBxR->GetStation(), 0, CDPoint(0,1));
	ptPos.x = min(ptPos.x, ptPosR.x);
	ptPos.y = (ptPos.y + ptPosR.y)/2;

	CString szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	CString str = "";
	if(pC->GetType()!=CR_TYPE_HSTEEL)
		str.Format("1-FLG PL.%s", szJewon);
	Dom.TextOut(ptPos.x, ptPos.y, str);
	double dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
	Dom.Rotate(-dRotate, ptPos.x, ptPos.y);
	*pDom << Dom;
	double dTextH = 0;
	//가로보 하부이음판
	long nQty = 0;
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		//하부상면
		if(pC->m_nRadioConnectionPlateInOut==0 && pC->GetType()!=CR_TYPE_HSTEEL)
		{
			dTextH += Dom.GetTextHeight()*1.5;
			nQty = (pC->m_nLGirSpType && pC->m_nRGirSpType) ? 8 : 4;
			vJ    = pGir->GetJewonTotal(CP_SP_L_U, pBx);
			sMark = pDB->GetMaterialSteelSymbol(CP_SP_L_U);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("%d-SPL PL.%s", nQty, szJewon);
			Dom.TextOut(ptPos.x, ptPos.y-dTextH, str);
			dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
			Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 
			*pDom << Dom;
		}
		//하부하면
		if(pC->GetType()==CR_TYPE_MANUFACTURE)
		{
			dTextH += Dom.GetTextHeight()*1.5;
			long nQty = pC->m_nLGirSpType && pC->m_nRGirSpType ? 2 : 1; 
			vJ    = pGir->GetJewonTotal(CP_SP_L_L, pBx);
			sMark = pDB->GetMaterialSteelSymbol(CP_SP_L_L);
			szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("%d-SPL PL.%s", nQty, szJewon);
			Dom.TextOut(ptPos.x, ptPos.y-dTextH, str);
			dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
			Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 
			*pDom << Dom;
		}
	}

	//볼트
	long E = pC->m_dE;
	long B = pC->m_dB;

	long E1 = (long)pC->m_dE1;
	long B1 = (long)pC->m_dB1;

	BOOL bZigZag = FALSE;
	if(pGlopt->GetDesignBoltHoleJigJaegOfHorPlate() == 0)
		bZigZag = TRUE;
	if(E==0) bZigZag = FALSE;
	long nQtyBolt = (B+1)*(E+1)*2*2;
	if(pC->m_bConnectionPlate)
	{
		nQtyBolt += (B1+1)*(E1+1)*2*2;
	}

	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		dTextH += Dom.GetTextHeight()*1.5;
		long nQty = 0;
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			nQty = nQtyBolt*2;
		else
			nQty = nQtyBolt;
		
		vJ    = pGir->GetJewonTotal(CP_BOLT, pBx, 4);
		sMark = "H.T.B";
		szJewon = pStd->m_pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-%s M%.0fx%.0f(%s)",nQty,sMark,vJ.y,vJ.z,pDB->GetMaterial(CP_BOLT));

		Dom.TextOut(ptPos.x, ptPos.y-dTextH, str);
		dRotate = vGir.BetweenAngle(CDPoint(0,1)).GetAngleRadian();
		Dom.Rotate(-dRotate, ptPos.x, ptPos.y); 

		*pDom << Dom;
	}	
}

void CAPlateOutCrossBeam::AddKeyPan(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxCr, BOOL bSummary)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint*	pMove		= pStd->GetMovePoint(pStd->GetCurPage());	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CLongArray nArrGir;
	pBridge->GetIndexGirderToCrossBeam(nArrGir, pBxCr, TRUE);

	double dHScale = 0;
	double dVScale = 0;
	BOOL bSpliceMark = FALSE;
	BOOL bJijumMark	 = FALSE;
	BOOL bCross		 = FALSE;
	BOOL bDimension	 = FALSE;
	BOOL bDirection	 = FALSE;
	BOOL bOneLine	 = FALSE;
	BOOL bLineInfo	 = FALSE;

	CDomyun Dom(pDom);	
		
	if(bSummary)//위치도에서 그리는 거면...
	{
		dHScale = pDomOpt->m_Cross_dPosHScale;
		dVScale = pDomOpt->m_Cross_dPosVScale;

		bSpliceMark  = pDomOpt->m_Cross_bSpliceMark;
		bJijumMark	 = pDomOpt->m_Cross_bJiJumMark;
		bCross		 = pDomOpt->m_Cross_bCrossBeam;
		bDimension	 = pDomOpt->m_Cross_bDimension;
		bDirection	 = pDomOpt->m_Cross_bDirection;
		bOneLine	 = pDomOpt->m_Cross_bOneline;
		bLineInfo	 = pDomOpt->m_Cross_bLineInfo;
	}
	else
	{
		long nSize = pDomOpt->m_CrossBeam_Scale.GetSize();
		dHScale = pDomOpt->m_CrossBeam_Scale.GetAt(nSize-2);
		dVScale = pDomOpt->m_CrossBeam_Scale.GetAt(nSize-1);

		bSpliceMark = pDomOpt->m_Cross_KeyPlanSpliceMark;
		bJijumMark	 = pDomOpt->m_Cross_KeyPlanJijumMark;
		bCross		 = pDomOpt->m_Cross_KeyPlanCrossBeam;
		bDimension	 = pDomOpt->m_Cross_KeyPlanDimension;
		bDirection	 = pDomOpt->m_Cross_KeyPlanDirection;
		bOneLine	 = pDomOpt->m_Cross_KeyPlanOneLine;
	}
	
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	
	
	// 진행방향
	CString sBridgeDir	= pDomOpt->m_sTextBridgeDirection;
	CString sCounterDir = pDomOpt->m_sTextCounterDirection;

	Dom.SetScaleDim(dHScale);
	
	// KEY PLAN 그리기
	DrawPyung.AddKeyPlan(&Dom,NULL,NULL,100,FALSE, bSpliceMark,bJijumMark, bDirection,FALSE,bCross,bOneLine,-1,-1,FALSE,FALSE,sBridgeDir,sCounterDir);	

		
	// 선형 정보
	if(bLineInfo)
	{
		CPlateGirderApp	*pGir = pBridge->GetGirder(-1);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex	*pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex	*pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
		CAPlateOutGeneral *pOutGen = new CAPlateOutGeneral;

		pOutGen->m_pStd = pStd;	
		pOutGen->DimTextLineInfo(&Dom, pBxStt, pBxEnd);

		delete pOutGen;
	}

	if(bSummary)
	{
		DrawMarkSummaryPlan(&Dom); // 가로보마크
	}
	else // HATCH 넣기
	{
		long nStartG = nArrGir.GetSize() >0? nArrGir.GetAt(0) : 0;
		//nStartG = 0;

		pOpt->SetEnvType(&Dom, HCAD_DIML);
		CPlateBasicIndex *pBxMatch;
		CDPoint po[4];
		double hWid = 300;

		for(long nG = nStartG; nG < pBridge->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(nG);
			CPlateGirderApp *pGirMatch = (CPlateGirderApp *)pGir->m_pRight;

			pBxMatch = pBxCr->GetBxMatchByCrossBeam(pGirMatch);
			if(pBxMatch)
			{
				double dSta1 = pBxCr->GetStation();
				double dSta2 = pBxMatch->GetStation();
				BOOL bDiffSta = TRUE;
				if(Compare(dSta1, dSta2, _T("="), 0.1) == FALSE && pBridge->m_nCrossArrangeType == 0)
				{
					bDiffSta = FALSE;
					dSta2 = dSta1;
				}

				CDPoint pt1 = pGir->GetXyGirderDis(dSta1); //pBxCr->GetStation()
				CDPoint pt2 = pGirMatch->GetXyGirderDis(dSta2); //pBxMatch->GetStation()
				if(nG== nStartG)
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
				else if(bDiffSta == FALSE)
				{
					CDPoint ptU = (pt2-pt1).Unit().Rotate90();
					po[2] = pt1-ptU*hWid;
					po[3] = pt1+ptU*hWid;

					break;
				}
			}
		}
		Dom.LineTo(po[0], po[1]);
		Dom.LineTo(po[2], po[3]);
		Dom.LineTo(po[0], po[2]);
		Dom.LineTo(po[1], po[3]);
		Dom.HatchRect(po[0].x,po[0].y,po[1].x,po[1].y,po[3].x,po[3].y,po[2].x,po[2].y,"ANSI31");

	}
	
	// 치수선	
	if(bDimension)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
		CPlateBxFinder Finder(pBridge->GetGirder(-1));
		//
//		__int64 nTypeUpper  = DIM_BASE_SPAN;
//		long  nSttDan = 1;
// 		DrawDimPyung.DimPyungTotal(&Dom,Finder.GetBxFirst(BX_STT_GIRDER),
// 			 Finder.GetBxLast(BX_END_GIRDER),nTypeUpper,FALSE,FALSE,"",nSttDan);
	}

	// 도면 이동 및 회전, 스케일조정
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER);
	CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();

	ASSERT( pBx || pBxNext);
	double rotAng = DrawPyung.GetRotateAnglePyung(pBx,pBxNext);
	CDPoint MoveXY = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
	Dom.Move(-MoveXY);	// 도로중심 (0,0)
	Dom.Rotate(rotAng,0,0);	

	BOOL bRotate = pDomOpt->m_Cross_KeyPlanRoVer;
	if (bRotate && !bSummary)
		Dom.Rotate(ConstPi/2, 0,0);

	Dom.SetCalcExtRect();	
	CDRect rectOutLine = Dom.GetExtRect();
/*	if (!bSummary)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
		rectOutLine.NormalizeRect();
		rectOutLine.left -= Dom.Always(10);	
		rectOutLine.top -= Dom.Always(10);
		rectOutLine.right += Dom.Always(10);
		rectOutLine.bottom += Dom.Always(30);
		Dom.Rectangle(rectOutLine);
	}
*/
	Dom.RedrawByHVScale(dHScale/dVScale,FALSE,TRUE);
	Dom.RedrawByScale(GetScale()/dHScale);	
	
	// 최초 기준점 이동
	Dom.Move(xDis,yDis);
	*pDom << Dom;
}


void CAPlateOutCrossBeam::DrawMarkSummaryPlan(CDomyun *pDomP)
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB	= pStd->GetDataManage()->GetBridge();
	COptionStd		*pDomOpt		= pStd->GetDomOptionStd();
	CString strText="";

	CDPoint PoStt(0,0),PoEnd(0,0),PoMid(0,0),vDir(0,1);
	CSymbolDom SDom(pDomP, pStd->m_pOptStd);

	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++) 
	{
		pStd->m_pOptStd->SetEnvType(&SDom,HCAD_TXTG);
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		int nCount = 1;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			ASSERT(pSec);
			if(pSec)
			{
				if(pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
				{
					double	dSta	= pBx->GetStation();
					double	Dis		= pGir->GetWidthFlangeUpper(pBx)/2 + pGir->GetDisGirderToGirder(pBx)/2;
					CDPoint CrossAng = pBx->GetAngle();
					if(nCount < 10)
						strText.Format("%dCR0%d", nG+1,nCount);
					else
						strText.Format("%dCR%d", nG+1,nCount);

					PoStt = pGir->GetXyGirderDis(dSta-SDom.Always(3), Dis, CrossAng);
					SDom.SymbolTextCircle(PoStt, strText, strText, CrossAng,pDomOpt->m_bMarkAlwaysInCircle);
				}
			}
			nCount++;
			pBx = Finder.GetBxNext();
		}	
	}

	*pDomP << SDom;
}

void CAPlateOutCrossBeam::AddDetailBMTotType(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
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

		pBMStd->GetBMListCrossBeam(&pList, pBxMatch);
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


void CAPlateOutCrossBeam::DimFrontJewonTextHStiff(CDomyun *pDom, CPlateBasicIndex *pBx)
{
/*	CARoadDrawStd	*pStd = (CARoadDrawStd*)m_pStd;
	CDrawDBStandard	*pDST = pStd->m_pDrawDBStandard;
	CBridgeApp		*pDB  = pStd->m_pDB;
	CGirderApp		*pGir = pBx->GetGirder();
	CCrossBeam		*pC = pBx->GetCrossBeam();

	CDPoint A[41], vAng = CDPoint(0,1);
	if (pStd->m_pDrawDBStandard->GetShopCutting()) {
		pGir->GetXyHunchShop(pBx, A);
		vAng = pBx->GetAngleCrossRight();
	}
	else
		pGir->GetHunchXyDom(pBx, A);

	CDimDomyunEx	Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(pDomP->GetScaleDim());

	double Hdis[4], hDisRate[2];
	long nHStiff = 0, Lower = 0;
	nHStiff = pC->GetHStiffUpperDistRate(hDisRate);
	for(long i = 0; i < nHStiff; i++)	
		Hdis[i] = hDisRate[i];
	Lower = pC->GetHStiffLowerDistRate(hDisRate);
	for(i = 0; i < Lower; i++)		
		Hdis[nHStiff++] = hDisRate[i];


	CDPoint Po[18], xy(0,0);	// 가로보상 수직부재의 중심포인트
	long nSize = pGir->GetPointVStiffOnCrossBeamPrv(pBx, Po, TRUE);
	double SpliceOffset = pGir->m_pSangse->m_HStiff_Splice_B;
	double VStiffOffset = pGir->m_pSangse->m_HStiff_VStiff_B;
	double SpliceWidth  = (pC->m_sA*2 + (pC->m_sB*pC->m_sC)*2 + pC->m_sC1);
	double WebTLeft     = pGir->GetThickJewon(G_W_L, pBx)/vAng.y;
	double WebTRight    = pGir->m_pRight->GetThickJewon(G_W_R, pBx)/vAng.y;
	double TextHeight   = Dom.GetTextHeight();
	double hT           = pC->m_hT_CR;

	CVector vJ(0,0,0);
	CString sMark=_T(""), szJewon=_T("");
	long nQty = 0;

	double Giul = (A[37].y - A[33].y)/(A[37].x - A[33].x);
	double sinval = 1;
	Dom.SetTextAngle(ToDegree(atan(Giul)));

	// 상부
	long nRow = 0;
	double giulCross  = pGir->GetGiulCrossBody(pBx, pDST->GetShopCutting());
	double guilGasset = pGir->GetGiulCrossGasset(pBx, pDST->GetShopCutting(), TRUE);

	CBaseChief *pBC	= (CBaseChief*)pDST->m_pDB->m_pBaseChief;
	BOOL bIncludeGasset = !pBC->m_BaseEtcCtl.GetHStiffGussetPosApply();

	long PoEa = pGir->GetPointVStiffOnCrossBeamPrv(pBx,Po,pDST->GetShopCutting());
	long nColStt = bIncludeGasset ? 0 : 1;
	long nColEnd = bIncludeGasset ? PoEa-1 : PoEa-2;

	CDPoint sp,ep;
	double HT  = pC->m_hT_CR/cos(atan(giulCross));
	double VT  = pC->m_vT_CR;
	double Len = 0;
	double SW  = 0;

	for(long nH=0; nH<nHStiff; nH++)
	{
		/// (nCol == 0)				// 좌측 복부판 ~ 좌측 이음판 까지 : 항상 있음
		if(pC->m_mW>0 && Hdis[nH]< (pC->m_mD+pC->m_mW) || Hdis[nH]>(pC->m_H_CR-pC->m_mD-pC->m_mW))
			SW = pC->m_mA*2 + (pC->m_mB*pC->m_mC)*2 + pC->m_mC1;
		else
			SW = pC->m_sA*2 + (pC->m_sB*pC->m_sC)*2 + pC->m_sC1;
		for(long n=nColStt; n<nColEnd; n++) 
		{
			if(Po[n].x==-1 && Po[n].y==-1) break;
			
			if(n == 0)				// 좌측 복부판와 좌측 이음판 사이
			{
				sp.x = Po[n].x + VStiffOffset;
				sp.y = Po[n].y + VStiffOffset * guilGasset;
				Len  = Po[n+1].x - Po[n].x - (SW/2+SpliceOffset+VStiffOffset);
				ep.x = sp.x + Len;
				ep.y = sp.y + Len * guilGasset;
				
			}
			else if(n == 1)			// 좌측 이음판과 첫번째 수직보강재
			{
				ep.x = Po[n+1].x -  VT/2 - VStiffOffset;
				ep.y = Po[n+1].y - (VT/2 + VStiffOffset) * giulCross;
				if(PoEa == 4)		// 수직보강재가 없는 경우
					Len = Po[n+1].x - Po[n].x - (SW + SpliceOffset*2);
				else
					Len = Po[n+1].x - Po[n].x - (SW/2+SpliceOffset+VStiffOffset+VT/2);

				sp.x = ep.x - Len;
				sp.y = ep.y - Len*giulCross;
			}		
			else if(n == (PoEa-3))	// 마지막 수직보강재와 우측 이음판
			{
				sp.x = Po[n].x + VT/2 + VStiffOffset;
				sp.y = Po[n].y + (VT/2 + VStiffOffset) * giulCross;
				Len  = Po[n+1].x - Po[n].x - (VT/2+VStiffOffset+SpliceOffset+SW/2);
				ep.x = sp.x + Len;
				ep.y = sp.y + Len*giulCross;
			}
			else if(n == (PoEa-2))	// 우측 이음판과 우측 복부판 사이
			{
				ep.x = Po[n+1].x - VStiffOffset;
				ep.y = Po[n+1].y - VStiffOffset * guilGasset;
				Len  = Po[n+1].x - Po[n].x - (SW/2+SpliceOffset+VStiffOffset);
				sp.x = ep.x - Len;
				sp.y = ep.y - Len * guilGasset;
			}
			else					// 수직보강재와 수직 수강재 사이
			{
				sp.x = Po[n].x + (VT/2+VStiffOffset);
				sp.y = Po[n].y + (VT/2+VStiffOffset) * giulCross;
				Len  = Po[n+1].x - Po[n].x - (VStiffOffset+VT/2)*2;
				ep.x = sp.x + Len;
				ep.y = sp.y + Len*giulCross;
			}
			if(Len<=0) continue;

			CDPoint ptUp[2], ptDn[2];
			CDPoint vVec;
			CDPoint ptTextUp, ptTextDn;
			ptUp[0] = CDPoint(sp.x,sp.y-Hdis[nH]+HT/2);
			ptUp[1] = CDPoint(ep.x,ep.y-Hdis[nH]+HT/2);
			ptDn[0] = CDPoint(sp.x,sp.y-Hdis[nH]-HT/2);
			ptDn[1] = CDPoint(ep.x,ep.y-Hdis[nH]-HT/2);

			vVec = (ptUp[1] - ptUp[0]).Unit();
			ptTextUp = ptUp[0] + vVec * ~(ptUp[1] - ptUp[0])/2 + vVec.Rotate90() * Dom.Always(1);//* Dom.Always(Dom.GetDimTvp());
			ptTextDn = ptDn[0] + vVec * ~(ptDn[1] - ptDn[0])/2 + vVec.RotateInv90() * (Dom.GetTextHeight() + Dom.Always(1));//* (Dom.GetTextHeight() + Dom.Always(Dom.GetDimTvp()));

			vJ = pGir->GetJewonTotal(CP_H, pBx, nH, n, DIM_BASIC);
			sMark = pDB->GetMaterialSteelSymbol(CP_H);
			szJewon = pStd->m_pOptStd->GetJewonString(nQty,vJ.x,vJ.y,vJ.z,sMark);
		
			Dom.TextOut(ptTextUp.x, ptTextUp.y, "H-STIFF");
			Dom.TextOut(ptTextDn.x, ptTextDn.y, szJewon);		
		}
	}	
	*pDomP << Dom;
	*/
}
