// APlateOutSummary.cpp: implementation of the CAPlateOutSummary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MPE_MEMBER		10
#define MPE_SECTION		20
#define MPE_DEADLOAD	30

#define BASE_Y			400

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutSummary::CAPlateOutSummary()
{
	m_Scale = 300;
	m_nG = 0;
	m_pBxStt = NULL;
	m_pBxEnd = NULL;
}

CAPlateOutSummary::~CAPlateOutSummary()
{

}

void CAPlateOutSummary::DrawOnePage()
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOptStd	= pStd->GetDataManage()->GetOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	
	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// 일반도이전의 도면번호
	long nSubPage = nPage - nprePage;
	double dScale  = GetScale();

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);	
	CDomyun Dom(pDom);	
	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
	//long nSttPage = pDomOpt->m_nDomPageArray.GetAt(PAGE_SUMMARY) == TRUE ? 0 : pDomOpt->m_nDomPageArray.GetAt(PAGE_SUMMARY) + nSubPage - 1;	
	//pStd->GetDogak()->Add(&Dom,nPage,dScale,pDomOpt->m_szDGKName,pDomOpt->m_sDomNumHead,"",nSttPage);
	pStd->GetDogak()->Add(&Dom,nPage,dScale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);
	pStd->AddMaterial(&Dom,dScale);
	pStd->AddJubujebubuje(&Dom,dScale);
	Dom.SetCalcExtRect();
	CDRect dgkRect = Dom.GetExtRect();
	*pDom << Dom;
	
	long nGSu  = pBridge->GetGirdersu();
	long nPageSuPerGirder = GetPageSu()/nGSu;
	long nG = (long) nSubPage / nPageSuPerGirder;
	if((nSubPage%nPageSuPerGirder)==0) nG--;

	long nCurPage = nSubPage - (nPageSuPerGirder * nG);

	CPlateBasicIndex *pBxStt = GetBxSttPage(nCurPage,nG);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nCurPage,nG);
	
	pOptStd->SetEnvType(&Dom,HCAD_STLC); 
	
	double dMomentY = pDomOpt->m_dSum_MomentY;
		
	DrawStressOfMember(&Dom, nG, dMomentY, dScale, pBxStt, pBxEnd);//부재력도..
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();

	double dMoveX = -rect.left+(dgkRect.Width()-rect.Width())/2;
	Dom.Move(dMoveX,pDom->Always(BASE_Y));
	rect = Dom.GetExtRect();	
	Dom.Move(pMove->GetPoint(MPE_MEMBER));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_MEMBER,"부재력도");
	*pDom << Dom;

	Dom.SetScaleDim(GetScale());
	DrawStd.AddTitleOnDom(&Dom, dScale, rect, "부재력도",TRUE,FALSE,0);		
	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(MPE_MEMBER+1));
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_MEMBER+1,"부재력도 제목");
	*pDom << Dom;

	DrawSectionSummary(&Dom, nG, dMomentY, dScale, pBxStt, pBxEnd);//단면요약도..
	Dom.SetCalcExtRect();	
	Dom.Move(dMoveX,pDom->Always(BASE_Y-200));
	rect = Dom.GetExtRect();
	Dom.Move(pMove->GetPoint(MPE_SECTION));
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_SECTION,"단면요약도");
	*pDom << Dom;

	Dom.SetScaleDim(GetScale());
	DrawStd.AddTitleOnDom(&Dom, dScale, rect, "단면요약도",TRUE,FALSE,0);		
	Dom.SetCalcExtRect();
	Dom.Move(pMove->GetPoint(MPE_SECTION+1));
	pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_SECTION+1,"단면요약도 제목");

	*pDom << Dom;
	pPageMng->SetProgressData("출력완료",100);
}

void CAPlateOutSummary::DrawStressOfMember(CDomyun *pDom, long nG, double dMomentY, double dScale, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	CDataManage     *pDataManage	= pStd->GetDataManage();
	CDrawPageMng	*pPageMng		= pStd->GetDrawPageMng();
	CAPlateCalcStd	*pCalcStd		= pStd->m_pCalcStd;
	CFemStressCalc  StressCalc(pDataManage, pCalcStd);	
	
	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);

	pPageMng->SetProgressData("부재력도 출력중...",30);
	StressCalc.OutputOfMomentDiagram(&Dom, nG, dMomentY, dScale, pBxStt, pBxEnd);
	*pDom << Dom;
}

void CAPlateOutSummary::DrawSectionSummary(CDomyun *pDom, long nG, double dMomentY, double dScale, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CAPlateDrawStd	*pStd			= (CAPlateDrawStd*)m_pStd;
	CDataManage     *pDataManage	= pStd->GetDataManage();
	CDrawPageMng	*pPageMng		= pStd->GetDrawPageMng();
	CAPlateCalcStd	*pCalcStd		= pStd->m_pCalcStd;
	CFemStressCalc  StressCalc(pDataManage, pCalcStd);	
	
	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);

	pPageMng->SetProgressData("단면요약도 출력중...",70);
	StressCalc.OutputOfSectionSummary(&Dom, nG, pStd->GetDomOptionStd()->m_dSum_HeightY, dMomentY, dScale, pBxStt, pBxEnd);
	*pDom << Dom;
}

long CAPlateOutSummary::GetPageSu()
{
	CAPlateDrawStd		*pStd  = (CAPlateDrawStd*)m_pStd;	
	CPlateBridgeApp		*pDB   = pStd->m_pDB;
	CPlateGirderApp		*pGir = pDB->GetGirder(0);
	COptionStd			*pOpt = pStd->m_pDomOptionStd;

	long nTotal  = 0;
	long nGSu = pDB->GetGirdersu();

	if(pOpt->m_nSum_DivNum == 0)
		pOpt->m_nSum_DivNum = 1;

	switch(pOpt->m_nSum_DivType)
	{
		case 0:
			nTotal = pDB->m_nQtyJigan;
			m_nPage = nTotal/pOpt->m_nSum_DivNum;
			if(nTotal%pOpt->m_nSum_DivNum)
				m_nPage++;
			break;
		case 1:
			nTotal  = pGir->GetQtySplice();
			m_nPage = nTotal/pOpt->m_nSum_DivNum;
			if(nTotal%pOpt->m_nSum_DivNum)
				m_nPage++;
			break;
		case 2:
			nTotal = pGir->GetQtyCrossBeamVBracing(); 
			m_nPage = nTotal/pOpt->m_nSum_DivNum;
			if(nTotal%pOpt->m_nSum_DivNum)
				m_nPage++;
			break;
		case 3:
			m_nPage = 1;
			break;
	}
	m_nPage = m_nPage * nGSu;

	return m_nPage;
}

CString CAPlateOutSummary::GetTitle()
{
	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	long nPageSu = m_nPage;
	CString szTitle = "";	
	szTitle.Format("단 면 요 약 도 (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	
	return szTitle;
}

double CAPlateOutSummary::GetScale()
{
	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	 = pStd->m_pDomOptionStd;

	m_Scale = pOpt->m_dSum_Scale;
	return m_Scale;
}

CPlateBasicIndex* CAPlateOutSummary::GetBxSttPage(long nPage, long nG)
{
 	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	    *pDB   = pStd->m_pDB;
	CPlateGirderApp		*pGir  = pDB->GetGirder(nG);
	COptionStd		*pOpt  = pStd->m_pDomOptionStd;

	CPlateBasicIndex *pBx = NULL;
	long nDivDom = pOpt->m_nSum_DivType;	
	switch(nDivDom)
	{
		case 0 :
			if(nPage == 1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BX_STT_GIRDER);
			}
			else
			{
				pBx = pGir->GetBxOnJijum((nPage-1)*pOpt->m_nSum_DivNum);
			}
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter((nPage-1)*pOpt->m_nSum_DivNum);
			break;
		case 2 :
			if(nG==-1){
				nG = pDB->GetNumberGirderNearestToCenterLine();
				pGir = pDB->GetGirder(nG);
			}

			if(nG>pDB->GetGirdersu()-2)
				pBx = pGir->m_pLeft->GetBxByCrossNo((nPage-1)*pOpt->m_nSum_DivNum)->GetBxMatchByCrossBeam(pGir);
			else
				pBx = pGir->GetBxByCrossNo((nPage-1)*pOpt->m_nSum_DivNum);
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxFirst(BX_STT_GIRDER);
			break;
	}	
	
	return pBx;
}


CPlateBasicIndex* CAPlateOutSummary::GetBxEndPage(long nPage, long nG)
{
 	CAPlateDrawStd   *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	    *pDB   = pStd->m_pDB;
	CPlateGirderApp		*pGir  = pDB->GetGirder(nG);
	COptionStd		*pOpt  = pStd->m_pDomOptionStd;

	CPlateBasicIndex *pBx = NULL;
	switch(pOpt->m_nSum_DivType)
	{
		case 0 :
			if(nPage == (long)(GetPageSu()/pDB->GetGirdersu()))
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BX_END_GIRDER);
			}
			else
			{
				pBx = pGir->GetBxOnJijum((nPage) * pOpt->m_nSum_DivNum);
			}
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter(nPage * pOpt->m_nSum_DivNum);
			break;
		case 2 :
			if(nG==-1){
				nG = 0;//pStd->m_pDrawDBStandard->GetNumberGirderNearestToCenterLine();
				pGir = pDB->GetGirder(nG);
			}
			if(nG>pDB->GetGirdersu()-2)
				pBx = pGir->m_pLeft->GetBxByCrossNo(nPage*pOpt->m_nSum_DivNum)->GetBxMatchByCrossBeam(pGir);
			else
				pBx = pGir->GetBxByCrossNo(nPage*pOpt->m_nSum_DivNum);
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxLast(BX_END_GIRDER);
			break;
	}
	
	return pBx;
}
