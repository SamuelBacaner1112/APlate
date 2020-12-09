// PlateGirderApp.cpp: implementation of the CPlateGirderApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"	

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlateGirderApp::CPlateGirderApp()
{
}

CPlateGirderApp::CPlateGirderApp(CPlateBridge* pDB,CLineInfo* pLine,CPlateGirderApp* pLeft) : CPlateGirder(pDB, pLine, pLeft)
{

}  

CPlateGirderApp::~CPlateGirderApp()
{
	AhTPADelete(&m_pSplice, (CPlateSplice*)0);
	AhTPADelete(&m_aJackupArr, (CJackupStiff*)0);
	AhTPADelete(&m_aWebHStiffArr, (CWebHStiff*)0);
	AhTPADelete(m_aFactChain, (CPlateFactChain*)0);
	AhTPADelete(m_aFactChain+1, (CPlateFactChain*)0);
	AhTPADelete(m_aFactChain+2, (CPlateFactChain*)0);
	AhTPADelete(&m_pCamber, (CCamber*)0);
	AhTPADelete(&m_BracingArr,    	(CBracing *)0);
	AhTPADelete(&m_VStiffSectionArr,(CSection *)0);
	AhTPADelete(&m_CrossSectionArr, (CSection *)0);	
	AhTPADelete(&m_HGussetArr,      (CHBracingGusset*)0);
	AhTPADelete(&m_JiJumVStiff,     (CVStiff*)0);
}

CPlateGirderApp& CPlateGirderApp::operator=(const CPlateGirderApp& obj)
{
	CPlateGirder::operator =(obj);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//Setter, Getter
//////////////////////////////////////////////////////////////////////////
CBracing* CPlateGirderApp::GetBracingInTotal(long nPos) const 
{ 
	if(nPos  < 0 || nPos >= m_BracingArr.GetSize())
		return NULL;

	return m_BracingArr.GetAt(nPos); 
}

double CPlateGirderApp::GetWidthOnStation(double dSta, BOOL bUpper, BOOL bNext) const
{
	__int64 flag = bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE;
	
	CPlateGirderApp		*pGir = (CPlateGirderApp *)this;
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBx = Finder.GetBxFirst(flag | BX_STT_SLAB | BX_STT_GIRDER | BX_END_GIRDER | BX_END_SLAB);
	
	if(!pBx)
		return pGir->GetValueBasicDefault("FLANGE_T"); 

	while(pBx)
	{
		double dStaIf1 = pBx->GetStation();

		if(dStaIf1 >= dSta)
		{
			CPlateFactChain* pFactChain		=	(CPlateFactChain*) pBx->GetFactChain(bUpper ? 0 : 1, bNext);
			CPlateFactChain* pFactChainPre	=	(CPlateFactChain*) pBx->GetFactChain(bUpper ? 0 : 1, FALSE);

			if(pFactChain==NULL || pFactChainPre==NULL)
				return pGir->GetValueBasicDefault("FLANGE_T"); 

			if(pFactChainPre->GetBx()==NULL)	break;

			if(dSta < pBx->GetStation() && dSta > pFactChainPre->GetBx()->GetStation())
				pFactChain = (CPlateFactChain*)pBx->GetFactChain(bUpper ? 0 : 1, FALSE);			

			return pFactChain->GetWidth();
		}
		pBx = Finder.GetBxNext();
	}

	pBx = Finder.GetBxLast(flag | BX_STT_SLAB | BX_STT_GIRDER | BX_END_GIRDER | BX_END_SLAB);
	CPlateFactChain* pFactChain = (CPlateFactChain*)pBx->GetFactChain(bUpper ? 0 : 1, bNext);

	if(pFactChain==NULL) return pGir->GetValueBasicDefault("FLANGE_T");

	return pFactChain->GetWidth();
}

double CPlateGirderApp::GetThickOnStation(double dSta, BOOL bUpper, BOOL bNext) const
{
	__int64 flag = bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE;
	
	CPlateGirderApp *pGir = (CPlateGirderApp *)this;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(flag | BX_STT_GIRDER | BX_END_GIRDER);
	
	if(!pBx)
		return pGir->GetValueBasicDefault("FLANGE_W"); 

	while(Finder.GetBxFixNext())
	{
		if(pBx->GetStation() >= dSta)
		{
			CPlateFactChain* pFactChain =(CPlateFactChain*)pBx->GetFactChain(bUpper ? 0 : 1, bNext);
			CPlateFactChain* pFactChainPre =(CPlateFactChain*)pBx->GetFactChain(bUpper ? 0 : 1, FALSE);

			if(pFactChain==NULL || pFactChainPre==NULL)
				return pGir->GetValueBasicDefault("FLANGE_W"); 

			if(pFactChainPre->GetBx()==NULL)	break;

			if(dSta < pBx->GetStation() && dSta > pFactChainPre->GetBx()->GetStation())
				pFactChain = (CPlateFactChain*)pBx->GetFactChain(bUpper ? 0 : 1, FALSE);			

			return pFactChain->m_dFactChainThick;
		}
		pBx = Finder.GetBxNext();
	}

	CPlateFactChain* pFactChain = (CPlateFactChain*)pBx->GetFactChain(bUpper ? 0 : 1, bNext);

	if(pFactChain==NULL) return pGir->GetValueBasicDefault("FLANGE_W");

	return pFactChain->m_dFactChainThick;
}

void CPlateGirderApp::MakeHGusset(long nCount)
{
	AhTPAMake(nCount, &m_HGussetArr, (CHBracingGusset *)0);
	SetLKGirderPointAndSelfCount(&m_HGussetArr);
}

void CPlateGirderApp::MakeVStiffSection(long nCount)
{
	AhTPAMake(nCount, &m_VStiffSectionArr, (CSection *)0);
	SetLKGirderPointAndSelfCount(&m_VStiffSectionArr);
}

void CPlateGirderApp::MakeCrossSection(long nCount)
{
	AhTPAMake(nCount, &m_CrossSectionArr, (CSection *)0);
	SetLKGirderPointAndSelfCount(&m_CrossSectionArr);
}

void CPlateGirderApp::MakeJiJumStiff(long nCount)
{
	AhTPAMake(nCount, &m_JiJumVStiff, (CVStiff *)0);
	SetLKGirderPointAndSelfCount(&m_JiJumVStiff);
}

void CPlateGirderApp::MakeBracing(long nCount)
{
	AhTPAMake(nCount, &m_BracingArr, (CBracing *)0);
	SetLKGirderPointAndSelfCount(&m_BracingArr);
}

CPlateBasicIndex* CPlateGirderApp::newBx(CPlateBasicIndex *pBx /*= NULL*/)
{
	CPlateBasicIndex* pResult = new CPlateBasicIndex(pBx);
	pResult->Create((CPlateGirderApp*)this);
	return pResult;
}

CPlateBasicIndex* CPlateGirderApp::InsertBx(const CPlateBasicIndex *pBx)
{
	for(POS pos = GetPOSHead(); pos; pos = GetPOSNext(pos))
	{
		CPlateBasicIndex* p = GetBx(pos);

		if(pBx->GetStation() == p->GetStation())
		{
			if( pBx->IsState(BX_CROSSBEAM_VBRACING) )
			{
				p->SetMatchCol(pBx->GetMatchCol());
			}
			if( pBx->GetBxFlagState() & ~(BX_BYUN) )	// 변단면 이외의 플래그가 있는 경우
			{
				p->SetAngle(pBx->GetAngle());
			}

			p->SetState( pBx->GetBxFlagState(),TRUE);
			return p;
		}
		else
		{
			if(pBx->GetStation() < p->GetStation())
			{
				CPlateBasicIndex* pNew = newBx((CPlateBasicIndex*)pBx);

				pNew->SetPOSSelf( m_BxList.InsertBefore( pos , pNew) );
				return pNew;
			}
		}
	}

	CPlateBasicIndex* pNew = newBx((CPlateBasicIndex*)pBx);

	pNew->SetPOSSelf( m_BxList.AddTail(pNew) );
	return pNew;
}

void CPlateGirderApp::SetLKPOS(long nLK)
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx;
	// 공장이음 상부
	if(nLK == CPlateBxObject::LK_FACTCHAIN_UPPER)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_STT_GIRDER | BX_UPPERFLANGE);		
		while(pBx)
		{
			ASSERT(nCount < m_aFactChain[0].GetSize());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_FACTCHAIN_UPPER, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}

		SetLKEndPOS(m_aFactChain);

	}
	// 공장이음 하부 
	if(nLK == CPlateBxObject::LK_FACTCHAIN_LOWER)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_STT_GIRDER | BX_LOWERFLANGE);
		while(pBx)
		{
			ASSERT(nCount < m_aFactChain[1].GetSize());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_FACTCHAIN_LOWER, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
		SetLKEndPOS(m_aFactChain+1);	// 공장이음위치 입력
	}
	// 공장이음 복부
	if(nLK == CPlateBxObject::LK_FACTCHAIN_WEB)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_STT_GIRDER| BX_WEB);
		while(pBx)
		{
			ASSERT(nCount < m_aFactChain[2].GetSize());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_FACTCHAIN_WEB, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
		SetLKEndPOS(m_aFactChain+2);	// 공장이음위치 입력
	}
	// 수평브레이싱
	if(nLK == CPlateBxObject::LK_BRACING)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{	
			ASSERT(nCount < m_BracingArr.GetSize());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_BRACING, nCount++);	
			pBx = (CPlateBasicIndex *)Finder.GetBxNext(); 
		}
	}
	// 현장이음
	if(nLK == CPlateBxObject::LK_SPLICE)	// set LinkToBx in LK_SPLICE
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_SPLICE);
		while(pBx)
		{
			ASSERT(nCount < m_pSplice.GetSize());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_SPLICE, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}
	// 수직보강재
	if(nLK == CPlateBxObject::LK_VSTIFF)
	{				
		long nCountVStiff = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);

		while(pBx)
		{				
			ASSERT(nCountVStiff < GetQtyVStiffSectionTotal());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_VSTIFF, nCountVStiff++);						
			pBx = (CPlateBasicIndex *)Finder.GetBxNext(); 
		}		
	}
	// 가로보(수직브레이싱 포함)
	if(nLK == CPlateBxObject::LK_CROSSBEAM)
	{				
		long nCountSection = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{				
			ASSERT(nCountSection < GetQtyCrossSectionTotal());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_CROSSBEAM, nCountSection++);						
			pBx = (CPlateBasicIndex *)Finder.GetBxNext(); 
		}		
	}
	// 거더 수평보강재
	if(nLK == CPlateBxObject::LK_WEBHSTIFF)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_PLATEHSTIFF);
		while(pBx)
		{
//			ASSERT(nCount < m_aWebHStiffArr.GetSize());
			if(nCount >= m_aWebHStiffArr.GetSize()) for(long i = 0; i < 3; i++)MakeWebStiff(nCount+1);
			pBx->SetObjLinkSeq(CPlateBxObject::LK_WEBHSTIFF, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}
	// 횡브레이싱 이음판
	if(nLK == CPlateBxObject::LK_HBRACING)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_HBRACINGGUSSET);
		while(pBx)
		{
 			ASSERT(nCount < m_HGussetArr.GetSize());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_HBRACING, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}		
	}
	// 잭업보강재
	if(nLK == CPlateBxObject::LK_JACKUP)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_JACKUP);
		while(pBx)
		{
			ASSERT(nCount < m_aJackupArr.GetSize());
			pBx->SetObjLinkSeq(CPlateBxObject::LK_JACKUP, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}	
	// 지점 보강재
	if(nLK == CPlateBxObject::LK_JIJUM_VSTIFF)
	{
		long nCount = 0;
		pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_JIJUMSTIFF);
		while(pBx)
		{
			//ASSERT(nCount < m_JiJumVStiff.GetSize());
			if(nCount >= m_JiJumVStiff.GetSize()) //제한적으로 사용해봄 2006.6.28
				MakeJiJumStiff(nCount);
			pBx->SetObjLinkSeq(CPlateBxObject::LK_JIJUM_VSTIFF, nCount++);
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}		
	}
}

double CPlateGirderApp::GetElevation(CPlateBasicIndex *pBx, double dis) const
{
	CPlateBridgeApp *pDB  = (CPlateBridgeApp *)m_pBridgeApp;
	CPlateGirderApp *pGir = (CPlateGirderApp *)this;

	double dWMaxGir  = pGir->GetMaxWidthFlange(TRUE);

	double lDis = m_dGirderCenter - dWMaxGir/2 - pDB->m_dHunchDisForBaseThick;
	double rDis = m_dGirderCenter + dWMaxGir/2 + pDB->m_dHunchDisForBaseThick;
	double lEle = pDB->GetHeightSlabFromRoadCenterDom(pBx, lDis);
	double rEle = pDB->GetHeightSlabFromRoadCenterDom(pBx, rDis);
	
	double minEle = min(lEle, rEle);

	return minEle - (pDB->m_dThickSlabBase+pDB->m_dThickPave);
}

// 거더의 실제 엘리베이션
double CPlateGirderApp::GetElevationGirderForReal(CPlateBasicIndex *pBx, long nRow, long nCol)
{
	CPlateBridgeApp *pDB  = (CPlateBridgeApp *)m_pBridgeApp;
	CPlateGirderApp *pGir = (CPlateGirderApp *)this;

	// 도로중심 계획고
	double	dPlanEL		= pGir->GetElevationPlanElOnCenLineAct(pBx->GetStation());
	double	dPaveThick	= pDB->m_dThickPave;
	double	dSlabThick	= pGir->GetThickSlab(pBx, 0);
	double	dPyungubae	= pDB->GetPyungubeCenLineByShoe(nRow,nCol);
	double	dLengthSlop	= fabs(pDB->GetDisSlopActOnShoe(nRow,nCol));
	double	dElev		= dPlanEL+dPyungubae*dLengthSlop-(dPaveThick+dSlabThick);
	return dElev;

	// 도로중심 계획고를 기준으로 한 엘리베이션	
//	double	dEle		= pGir->GetElevationPaveAct(pBx->GetStation()) - (dPaveThick+dSlabThick);
//	return	dPlanEL + dEle;
}

long CPlateGirderApp::GetNumberGirder() const
{	
	CPlateBridge* pDB = GetBridgeApp();
	long nG = 0;
	for(nG = -1; nG < pDB->GetGirdersu(); nG++)
		if(this == pDB->GetGirder(nG)) break;

	return nG;
}

void CPlateGirderApp::GetHunchXyDom(CPlateBasicIndex *pBx, CDPoint* A, BOOL bApplyFlangeOpt, BOOL bJijumHunchApply, BOOL bExceptTUBlock) const
{
	CPlateBridgeApp	*pDB	= (CPlateBridgeApp *)m_pBridgeApp;	
	CGlobarOption	*pGlopt	= pDB->m_pDataManage->GetGlobalOption();
	CPlateGirderApp *pGir	= (CPlateGirderApp*)this;
	CSection		*pSec	= ((CPlateBasicIndex *)pBx)->GetSection();
	CPlateBasicIndex *pBxRight = m_pRight ? m_pRight->GetBxByStation(pBx->GetStation()) : NULL;
	CPlateBxFinder Finder(m_pRight);

	CDPoint Pave[HDANLINESU], xyMatch(0,0);
	pDB->GetXyPaveDom(pBx, Pave);
	double	dTPave = pDB->GetThickPave();
	long	cnt	   = 0;
	for(cnt=0; cnt<HDANLINESU; cnt++)
		Pave[cnt].y -= dTPave;

	if(pSec)
	{
		if(pSec->IsenDetType(SECDET_CROSSBEAM) || pSec->IsenDetType(SECDET_VBRACING))
			pBxRight = ((CPlateBasicIndex *)pBx)->GetBxMatchByCrossBeam(m_pRight); 
		else if(pSec->IsenDetType(SECDET_VSITFF))
			pBxRight = ((CPlateBasicIndex *)pBx)->GetBxMatchByVStiff(m_pRight); 
	}
	else if(pBx->IsState(BX_STT_SLAB) && m_pRight)
		pBxRight = Finder.GetBxFirst(BX_STT_SLAB);
	else if(pBx->IsState(BX_END_SLAB) && m_pRight)
		pBxRight = Finder.GetBxFirst(BX_END_SLAB);
	
	for(long i=0;i<10;i++)
	{
		A[i].x = 0;
		A[i].y = 0;
	}

	double dSta			= pBx->GetStation();		
	double dWGir		= pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWMaxGir		= pGir->GetMaxWidthFlange(TRUE);
	double dWidthGap	= (dWMaxGir -dWGir)/2; // 최대플랜지 폭과 현재 플랜지폭 차의 반 
	double dWHunch		= pDB->m_dWidthHunch;
	double mL			= 1/pDB->m_dSlopHunch;		//거더 좌측 헌치 기울기
	BOOL   bTaperL		= IsExistTaperArea(pBx, 0) && !bJijumHunchApply;
	BOOL   bTaperR		= m_pRight&&pBxRight ? m_pRight->IsExistTaperArea(pBxRight, 0) && !bJijumHunchApply : bTaperL;

	if(pDB->IsTUGir())
	{
		A[0].x = m_dGirderCenter - pDB->m_BindConc.m_dWidth/2;
		A[0].y = GetElevation(pBx, -pDB->m_BindConc.m_dWidth/2) - GetHeightFlangeUpperByTaper(dSta, 0, CDPoint(0,1), bExceptTUBlock);
		A[1].x = m_dGirderCenter - dWGir/2;
		A[1].y = A[0].y+pDB->m_BindConc.m_dDeep-pDB->m_BindConc.m_dHeight+pDB->m_BindConc.m_dDis;

		A[3].x = m_dGirderCenter + pDB->m_BindConc.m_dWidth/2;
		A[3].y = GetElevation(pBx, pDB->m_BindConc.m_dWidth/2) - GetHeightFlangeUpperByTaper(dSta, 0, CDPoint(0,1), bExceptTUBlock);
		A[2].x = m_dGirderCenter + dWGir/2;
		A[2].y = A[3].y+pDB->m_BindConc.m_dDeep-pDB->m_BindConc.m_dHeight+pDB->m_BindConc.m_dDis;
	}
	else
	{
		A[0].x = m_dGirderCenter - dWMaxGir/2 - dWHunch;
		A[0].y = GetElevation(pBx,0) - GetHeightFlangeUpperByTaper(dSta, 0, CDPoint(0,1), TRUE);
		A[1].x = A[0].x + dWHunch + dWidthGap;
		A[1].y = A[0].y;
		A[2].x = A[1].x + dWGir;
		A[2].y = A[1].y;
		A[3].x = A[2].x + dWHunch + dWidthGap;
		A[3].y = A[2].y;
	}
	
	if(bExceptTUBlock)
	{
		double dTaper = bTaperL ? GetHeightFlangeUpperByTaper(dSta, 0, CDPoint(0,1), bExceptTUBlock) : 0;
		A[1] = A[0] + CDPoint(0,1)*(pDB->m_BindConc.m_dDeep+dTaper);
		A[2] = A[3] + CDPoint(0,1)*(pDB->m_BindConc.m_dDeep+dTaper);
	}

	A[4] = pDB->GetMatchPavePointDom(pBx, A[3], mL);
	if(pDB->m_dThickSlabHunch==0)
		A[4] = A[3];	

	if(pBxRight)
	{
		CPlateGirderApp *pGirR = pBxRight->GetGirder();

		double dStaRight = pBxRight->GetStation();
		double dWGirR     = pGirR->GetWidthOnStation(dStaRight, TRUE);
		double dWMaxGirR  = pGirR->GetMaxWidthFlange(TRUE);
		double dWidthGapR = (dWMaxGirR - dWGirR)/2; // 최대플랜지 폭과 현재 플랜지폭 차의 반 

		if(pDB->IsTUGir())
		{
			A[6].x = pGirR->m_dGirderCenter - pDB->m_BindConc.m_dWidth/2;
			A[6].y = pGirR->GetElevation(pBxRight,-pDB->m_BindConc.m_dWidth/2) - pGirR->GetHeightFlangeUpperByTaper(dStaRight, 0, CDPoint(0,1), bExceptTUBlock);
			A[7].x = pGirR->m_dGirderCenter - dWGirR/2;
			A[7].y = A[6].y+pDB->m_BindConc.m_dDeep-pDB->m_BindConc.m_dHeight+pDB->m_BindConc.m_dDis;

			A[9].x = pGirR->m_dGirderCenter + pDB->m_BindConc.m_dWidth/2;
			A[9].y = pGirR->GetElevation(pBxRight,pDB->m_BindConc.m_dWidth/2) - pGirR->GetHeightFlangeUpperByTaper(dStaRight, 0, CDPoint(0,1), bExceptTUBlock);
			A[8].x = pGirR->m_dGirderCenter + dWGirR/2;
			A[8].y = A[9].y+pDB->m_BindConc.m_dDeep-pDB->m_BindConc.m_dHeight+pDB->m_BindConc.m_dDis;
		}
		else
		{
			A[9].x = pGirR->m_dGirderCenter + dWGirR/2 + dWHunch + dWidthGapR;
			A[9].y = pGirR->GetElevation(pBxRight, 0) - pGirR->GetHeightFlangeUpperByTaper(dStaRight, 0, CDPoint(0,1), TRUE);
			A[8].x = pGirR->m_dGirderCenter + dWGirR/2;
			A[8].y = A[9].y;

			A[7].x = pGirR->m_dGirderCenter - dWGirR/2;
			A[7].y = A[8].y;
			A[6].x = pGirR->m_dGirderCenter - dWGirR/2 - dWHunch - dWidthGapR;
			A[6].y = A[7].y;
		}

		if(bExceptTUBlock)
		{
			double dTaper = bTaperR ? pGirR->GetHeightFlangeUpperByTaper(dStaRight, 0, CDPoint(0,1), bExceptTUBlock) : 0;
			A[7] = A[6] + CDPoint(0,1)*(pDB->m_BindConc.m_dDeep+dTaper);
			A[8] = A[9] + CDPoint(0,1)*(pDB->m_BindConc.m_dDeep+dTaper);
		}

		A[5] = pDB->GetMatchPavePointDom(pBxRight, A[6], -mL);		
		if(pDB->m_dThickSlabHunch==0)
			A[5] = A[6];
	}

	if(bApplyFlangeOpt && pGlopt->GetDesignHeigtBaseUpper()==0)
	{
		for(long i=0;i<10;i++)
		{
			A[i].y -= GetThickFlangeByBase(pBx->GetStation());
		}
	}
	
	if(bTaperL)	GetXyMatchSegAndLine(A[3], A[6], A[4], CDPoint(0,1), A[4]);
	if(bTaperR)	GetXyMatchSegAndLine(A[3], A[6], A[5], CDPoint(0,1), A[5]);

	// Mantis 6003 - 삼표요청
	if(pDB->IsTUGir())
	{
		// 0, 3, 6, 9
		for(long n=0; n<10; n+=3)
		{
			for(cnt=0; cnt<29; cnt++)
			{
				if(!GetXyMatchSegAndLine(Pave[cnt], Pave[cnt+1], A[n], CDPoint(0,1), xyMatch)) continue;
				if(xyMatch.y-A[n].y > pDB->m_dThickSlabBase) break;
				A[n].y = xyMatch.y-pDB->m_dThickSlabBase;
				break;
			}
		}
		if(pDB->m_dThickSlabHunch==0)
		{
			A[4] = A[3];
			A[5] = A[6];
		}
	}
}

CShoe* CPlateGirderApp::GetShoeByJijumNo(long nJijumNo)
{
	return &m_Shoe[nJijumNo];
}

CPlateBasicIndex* CPlateGirderApp::GetBxOnJijum(long nPier) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);

	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_JIJUM);
	long nCount = 0;
	while( pBx )
	{
		if(nCount == nPier)
			return pBx;
		else
			nCount++;

		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}

	return NULL;

}

double	CPlateGirderApp::GetGiulJongDanShoe(CPlateBasicIndex *pBx) const
{	
	ASSERT(pBx);
	ASSERT(pBx->IsJijum());
	return GetGiulJongDanOnlyLine(pBx->GetStation(), 0, pBx->GetAngle());
}

void CPlateGirderApp::MakeFactChainUpp(long nMake)
{
	AhTPAMake(nMake,m_aFactChain,(CPlateFactChain*)0);
	SetLKGirderPointAndSelfCount(m_aFactChain);
	SetLKSideFactChain();
}

void CPlateGirderApp::MakeFactChainLow(long nMake)
{
	AhTPAMake(nMake,m_aFactChain+1,(CPlateFactChain*)0);
	SetLKGirderPointAndSelfCount(m_aFactChain+1);
	SetLKSideFactChain();
}

void CPlateGirderApp::MakeFactChainWeb(long nMake)
{
	AhTPAMake(nMake,m_aFactChain+2,(CPlateFactChain*)0);
	SetLKGirderPointAndSelfCount(m_aFactChain+2);
	SetLKSideFactChain();
}


long CPlateGirderApp::GetQtyCrossBeamOnly() const
{
	return GetQtyCrossBeamOrVBracing(TRUE);
}
long CPlateGirderApp::GetQtyVBracingOnly() const
{
	return GetQtyCrossBeamOrVBracing(FALSE);
}

long CPlateGirderApp::GetQtyCrossBeamOrVBracing(BOOL bCrossBeamOrVBracing) const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	int nCount = 0;
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec)
		{
			if( (bCrossBeamOrVBracing && pSec->GetenDetType() == SECDET_CROSSBEAM) //가로보 
				|| (!bCrossBeamOrVBracing && pSec->GetenDetType() == SECDET_VBRACING) ) // 수직브레이싱
			{
				nCount++;
			}
		}


		pBx = Finder.GetBxNext();
	}	
	return nCount;
}




long CPlateGirderApp::GetQtyCrossAType() const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nCount = 0;
	while(pBx)
	{
		if(pBx->GetSection()->GetenType() == SECTION_A) ++nCount;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	return nCount;
}

long CPlateGirderApp::GetQtyCrossCType() const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nCount = 0;
	while(pBx)
	{
		if(pBx->GetSection()->GetenType() == SECTION_C) ++nCount;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	return nCount;
}

long CPlateGirderApp::GetQtyCrossBType() const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nCount = 0;
	while(pBx)
	{
		if(pBx->GetSection()->GetenType() == SECTION_B) ++nCount;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	return nCount;
}

long CPlateGirderApp::GetQtyCrossPType() const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nCount = 0;
	while(pBx)
	{
		if(pBx->GetSection()->GetenType() == SECTION_P) ++nCount;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	return nCount;
}
long CPlateGirderApp::GetQtyHGusset(BOOL bReal) const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_HBRACINGGUSSET);
	long nCount = 0;
//	long nConnect =0 ;
	while(pBx)
	{
// 		nConnect = pBx->GetHBracingGusset()->m_nMatchCol[0] + pBx->GetHBracingGusset()->m_nMatchCol[1]
// 			      +pBx->GetHBracingGusset()->m_nMatchCol[2] + pBx->GetHBracingGusset()->m_nMatchCol[3];
//		if(nConnect > 0) ++nCount;
		 ++nCount;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	return nCount;
}

long CPlateGirderApp::GetQtyBracingReal() const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_HBRACINGGUSSET);
	long nCount = 0;
	while(pBx)
	{
		if(pBx->GetHBracingGusset()->IsReal())
			nCount++;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	return nCount;
}

long CPlateGirderApp::GetQtyVStiff() const
{
/*	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
	long nCount = 0;
	while(pBx)
	{		
		if(pBx->GetSection()->GetenType() == SECDET_VSITFF) ++nCount;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
	return nCount;
*/
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	Finder.GetBxFirst(BX_VSTIFF);
	return Finder.GetCountBx();
}

long CPlateGirderApp::GetQtyCrossBeamVBracing() const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	return Finder.GetCountBx();
}


long CPlateGirderApp::GetQtyCrossBeamVBracingHunch() const
{
	CPlateBridgeApp *pBridge = (CPlateBridgeApp *)m_pBridgeApp;

	long nQtyHunch = 0;
	if(pBridge->m_bApplyHunchAtCB==FALSE)	return 0;

	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{	
			CPlateCrossBeam *pC = pSec->GetCrossBeam();
			if((pC->m_dLH1==0 || pC->m_dRH1==0) && GetHeightTaper(pBx->GetStation())==0)
				nQtyHunch++;
		}

		pBx = Finder.GetBxNext();
	}

	return nQtyHunch;
}



CPlateBasicIndex *CPlateGirderApp::GetBxCrossBeamOnly(long nCount) const
{
	return GetBxCrossBeamOrVBracing(TRUE, nCount);
}
	


CPlateBasicIndex* CPlateGirderApp::GetBxVBracingOnly(long nCount) const
{
	return GetBxCrossBeamOrVBracing(FALSE, nCount);
}



CPlateBasicIndex *CPlateGirderApp::GetBxCrossBeamHasHunch() const
{
	if(!m_pBridgeApp->m_bApplyHunchAtCB)	return NULL;
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec->IsenDetType(SECDET_CROSSBEAM))
		{	
			CPlateCrossBeam *pC = pSec->GetCrossBeam();
			if((pC->m_dLH1==0 || pC->m_dRH1==0)
				&& pGir->GetHeightTaper(pBx->GetStation()) == 0)

				return pBx;
		}

		pBx = Finder.GetBxNext();
	}

	return NULL;
}
	

CPlateBasicIndex* CPlateGirderApp::GetBxCrossBeamOrVBracing(BOOL bCrossBeamOrVBracing, long nCount) const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	int nInCount = 0;
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec)
		{
			if( (bCrossBeamOrVBracing && pSec->GetenDetType() == SECDET_CROSSBEAM)
				|| (!bCrossBeamOrVBracing && pSec->GetenDetType() == SECDET_VBRACING))
			{
				if(nInCount == nCount)
					return pBx;
				nInCount++;
			}
		}
		pBx = Finder.GetBxNext();
	}	

	return NULL;
}


CPlateBasicIndex *CPlateGirderApp::GetBxBySectionType(long nSectType, long nPos) const
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF);
	long nCount = -1;
	while(pBx)
	{
		if(pBx->GetSection()->GetenType() == nSectType) ++nCount;
		if(nPos == nCount) break;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();	
	}

	return pBx;

}

CPlateBasicIndex *CPlateGirderApp::GetBxByVBracingPos(long nPos)
{
	CPlateBxFinder Finder((CPlateGirderApp *)this);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF);
	long nCount = -1;
	while(pBx)
	{
		if(pBx->GetSection()->GetenDetType() == SECDET_VBRACING) ++nCount;
		if(nPos == nCount) break;
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();	
	}
	if(pBx)
		return pBx;
	else
		return NULL;
}

long CPlateGirderApp::GetQtyVBracing()
{
	CPlateGirderApp *pGir =(CPlateGirderApp*)this;
	
	long nTotalVBr = GetQtyCrossBType();
	if(pGir->GetBxOnJijum(0)->GetSection()->IsenDetType(SECDET_VBRACING))
		nTotalVBr++;
	if(pGir->GetBxOnJijum(m_pBridgeApp->m_nQtyJigan)->GetSection()->IsenDetType(SECDET_VBRACING))
		nTotalVBr++;

	return nTotalVBr;
}

long CPlateGirderApp::GetVBracingPosByBx(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir =(CPlateGirderApp*)this;

	long nCurVBracing = 0;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxComp = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBxComp)
	{
		if(pBxComp==pBx)	break;

		if(pBxComp->GetSection()->IsenDetType(SECDET_VBRACING))
			nCurVBracing++;
		
		pBxComp = Finder.GetBxNext();
	}

	return nCurVBracing;
}

CPlateGirderApp *CPlateGirderApp::GetNextGirder() const
{
	CPlateBridgeApp *pBridge = (CPlateBridgeApp *)m_pBridgeApp;

	for(long nG=0;nG<pBridge->GetGirdersu()-1;nG++)
		if(pBridge->GetGirder(nG) == this) return pBridge->GetGirder(nG+1);

	return NULL;
}

CBracing* CPlateGirderApp::GetBracingByHBrGussetPos(long nPos)
{
	CHBracingGusset* pBrGu = GetHBracingGussetInTotal(nPos);
	CPlateBasicIndex* pBx = pBrGu->GetBx();
	double dSta = pBx->GetStation();
	CPlateBasicIndex* pBxBr = (CPlateBasicIndex* )GetBxByStationDir(dSta, -1, BX_CROSSBEAM_VBRACING);
	if(!pBxBr) pBxBr = (CPlateBasicIndex* )GetBxByStationDir(dSta, 0, BX_CROSSBEAM_VBRACING);
	CBracing* pBr = pBxBr->GetBracing();
	
	return pBr;

}

long CPlateGirderApp::GetBracingPosByHBrGussetPos(long nPos)
{
	CHBracingGusset  *pBrGu = GetHBracingGussetInTotal(nPos);
	CPlateBasicIndex *pBx   = pBrGu->GetBx();
	double dSta = pBx->GetStation();
	//HBrGusset위치에 BX_CROSSBEAM_CENTER이 있을경우는 그 BX를 설정하고 
	//없을 경우는 스테이션에서 이전방향의 첫번째 BX를 설정한다.
	CPlateBasicIndex* pBxBr = NULL;
	if(pBx->GetSection()==NULL)
	{
		pBxBr = (CPlateBasicIndex* )GetBxByStationDir(dSta, -1, BX_CROSSBEAM_VBRACING); 
	}
	else
	{
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) || pBx->GetSection()->IsenDetType(SECDET_VBRACING))
			pBxBr = pBx;
		else
			pBxBr = (CPlateBasicIndex* )GetBxByStationDir(dSta, -1, BX_CROSSBEAM_VBRACING); 
	}

	if(pBxBr)		return pBxBr->GetBracing()->GetSelfCount();
	else			return -1;

}


//거더복부판중심과 다음거더복부판중심까지의 거리
double CPlateGirderApp::GetDisGirderToGirder(CPlateBasicIndex* pBx)
{
	if(m_pRight == NULL)	return 0;
	if(!IsExtLineGirder() && !m_pRight->IsExtLineGirder())
		return m_pRight->m_dGirderCenter - m_dGirderCenter;

	CPlateBasicIndex *pBxRight = pBx->GetBxMatchByCrossBeam(m_pRight);
	if(pBxRight == NULL)
		pBxRight = pBx->GetBxMatchByVert(m_pRight);

	double  staRight      = m_pRight->GetStationBySc(pBxRight->GetStation(), 0, CDPoint(0,1));
	CDPoint xyRight       = m_pRight->GetXyGirderDis(staRight);
	double  disGirderLine = m_pLine->GetDisMatchVerticalByXy(xyRight);

	return fabs(disGirderLine - m_pRight->m_dGirderCenter);

}
//유의사항
//거더상부플랜지 중앙하면에서 dLh1+dT1만큼 내려온곳에서 다음거더동일위치의 기울기
double CPlateGirderApp::GetGiulCrossBeam(CPlateBasicIndex* pBx)
{
	if(m_pRight == NULL)	return 0;
	CPlateBasicIndex* pBxN = GetBxMatchByCrossBeam(pBx, m_pRight);
	CPlateGirderApp* pGirN = (CPlateGirderApp*)m_pRight;
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();	
	
	double dWidthGir  = GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWidthGirN = pGirN->GetWidthOnStation(pBxN->GetStation(), TRUE);
	double dThickWeb  = GetThickFactByStation(pBx->GetStation(), 2);
	double dThickWebN = pGirN->GetThickFactByStation(pBxN->GetStation(), 2);	
	CDPoint A[10];
	GetHunchXyDom(pBx, A);
	CDPoint xyStt = A[2] + CDPoint(-1,0)*(dWidthGir/2-dThickWeb/2) 
		            + CDPoint(0,-1)*(pC->m_dLH1);
	CDPoint xyEnd = A[7] + CDPoint( 1,0)*(dWidthGirN/2-dThickWebN/2)
		            + CDPoint(0,-1)*(pC->m_dRH1);
	CDPoint vDir = (xyEnd-xyStt).Unit();
	double dGiul = vDir.y/vDir.x;

	return dGiul;
}
//유의사항
//가로보의 중심부분의 기울기
double CPlateGirderApp::GetGiulCrossBeamCenter(CPlateBasicIndex* pBx)
{
	if(m_pRight == NULL)	return 0;
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();	
	
	CDPoint A[10];
	pC->GetCrossXyDom(A);

	CDPoint vDir = (A[4]-A[3]).Unit();
	double dGiul = vDir.y/vDir.x;

	return dGiul;
}

CPlateBasicIndex	*CPlateGirderApp::GetBxMatchByCrossBeam(CPlateBasicIndex *pBx, CPlateGirderApp *pGir) const
{
	if(pBx==NULL) return NULL;

	return pGir->GetBx( GetPOSMatchByCross( pBx->GetPOS(),pGir));
}

CPlateBasicIndex	*CPlateGirderApp::GetBxMatchByVStiff(CPlateBasicIndex *pBx, CPlateGirderApp *pGir) const
{
	return pGir->GetBx( GetPOSMatchByVStiff( pBx->GetPOS(),pGir));
}

//입력 : nIdx	현재 거더의 인덱스(다이아프램 인덱스만 해당됨) 
//		 pGir	매치 인덱스를 찾으려고 하는 거더 포인터
//기능 : 찾으려고 하는 거더의 인덱스
//		 없으면 -1;
POS CPlateGirderApp::GetPOSMatchByCross(POS psBx, CPlateGirderApp *pGir) const
{
	ASSERT( pGir && psBx );

	CPlateBasicIndex *pBx = GetBx(psBx);
	CPlateBxFinder Finder((CPlateGirderApp *)pGir);
	CPlateBasicIndex *pBxN = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while( pBxN )
	{
		if( pBxN->GetMatchCol() == pBx->GetMatchCol() )
			return Finder.GetPOS();

		pBxN = Finder.GetBxNext();
	}

	return NULL;
}

POS CPlateGirderApp::GetPOSMatchByVStiff(POS psBx, CPlateGirderApp *pGir) const
{
	ASSERT( pGir && psBx );

	CPlateBasicIndex *pBx = GetBx(psBx);
	CPlateBxFinder Finder((CPlateGirderApp *)pGir);
	CPlateBasicIndex *pBxN = Finder.GetBxFirst(BX_VSTIFF);
	while( pBxN )
	{
		if( pBxN->GetMatchCol() == pBx->GetMatchCol() )
			return Finder.GetPOS();

		pBxN = Finder.GetBxNext();
	}

	return NULL;
}

void CPlateGirderApp::SetBxPosMatchCross()
{//  거더 전체 pBx->GetMatchCol() 값이 setting된 후에 이함수를 하나씩 호출해야함
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	while(pBx)
	{
		if(m_pLeft)
		{
			CPlateBxFinder FL(m_pLeft);
			CPlateBasicIndex *pBxL = (CPlateBasicIndex *)FL.GetBxFirst(BX_CROSSBEAM_VBRACING);
			while( pBxL )
			{
				if(pBxL->GetMatchCol() == pBx->GetMatchCol())
				{	pBx->SetPOSLeftCross(FL.GetPOS());break;	}
				pBxL = (CPlateBasicIndex *)FL.GetBxNext();
			}
		}
		else	pBx->SetPOSLeftCross(NULL);

		// Right Girder
		if(m_pRight)
		{
			CPlateBxFinder FL(m_pRight);
			CPlateBasicIndex *pBxL = (CPlateBasicIndex *)FL.GetBxFirst(BX_CROSSBEAM_VBRACING);
			while( pBxL )
			{
				if(pBxL->GetMatchCol() == pBx->GetMatchCol())
				{	pBx->SetPOSRightCross(FL.GetPOS());break;	}
				pBxL = (CPlateBasicIndex *)FL.GetBxNext();
			}
		}
		else	pBx->SetPOSRightCross(NULL);

		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}
}

double	CPlateGirderApp::GetThickVStiff(CPlateBasicIndex *pBx, BOOL bLeft) const
{
	CSection *pSec = ((CPlateBasicIndex *)pBx)->GetSection();
	ASSERT(pSec);
	if(!pSec) return 0;
	CVStiff *pVStiff = pSec->GetVStiff();
	ASSERT(pVStiff);
	if(!pVStiff) return 0;
	return pVStiff->m_dThick[bLeft ? 0 : 1];
}

//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 수직보강재에 대한 길이를 리턴		//좌우 구분할 것.****
//bLeft  = TRUE   ==> 거더좌측의 수직보강재
//수직보강재의 폭이 0 일경우는 이 함수를 들어오지 않음.
//Upper는 상부플랜지와 수직보강재 사이의 이격거리(TU=50)이지만, 이 경우 수직보강재의 길이를 의미함
double CPlateGirderApp::GetLengthVStiff(CPlateBasicIndex *pBx, BOOL bLeft, DIMTYPE nDimType) const
{
	CSection *pSec = ((CPlateBasicIndex *)pBx)->GetSection();
	ASSERT(pSec);
	if(!pSec) return 0;
	CVStiff *pVStiff = pSec->GetVStiff();
	ASSERT(pVStiff);
	if(!pVStiff) return 0;

	double dHGir = pBx->GetGirder()->GetHeightGirderByStaAng(pBx->GetStation());

	return pVStiff->GetVStiffHeight(bLeft, dHGir);
}

double CPlateGirderApp::GetWidthVStiff(CPlateBasicIndex *pBx, BOOL bLeft) const
{
	CSection *pSec = ((CPlateBasicIndex *)pBx)->GetSection();
	ASSERT(pSec);
	if(!pSec) return 0;
	CVStiff *pVStiff = pSec->GetVStiff();
	ASSERT(pVStiff);
	if(!pVStiff) return 0;
	return pVStiff->m_dWidth[bLeft ? 0 : 1];
}

double CPlateGirderApp::GetWidthPlateSolePlateJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{	
	return m_Shoe[GetNumberJijumByBx(pBx)].GetHeigthTotalSolePlate();		
}

//입력 : nRow		0=좌측 기준 1번째, 1= 2번째 솔플레이트
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 솔플레이트의 길이(교축방향)
double CPlateGirderApp::GetLengthPlateSolePlateJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{		
	return m_Shoe[GetNumberJijumByBx(pBx)].GetWidthTotalSolePlate();			
}

//입력 : nRow		0=좌측 기준 1번째, 1= 2번째 솔플레이트
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 솔플레이트의 두께(가장 큰 두께)
double CPlateGirderApp::GetThickPlateSolePlateJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	long nNum = GetNumberJijumByBx(pBx);		

	double dThick    =  m_Shoe[nNum].m_dThick_SolePlate;
	double dLength   =  m_Shoe[nNum].GetWidthTotalSolePlate();
	double dJongGiul =	GetGiulJongDanShoe(pBx);

	return RoundUp(dThick + dLength * fabs(dJongGiul) / 2,0);
}

//입력 : nMarkBuje	부재 넘버 (전체 부재중에서)
//		 nRow		종리브, 수평보강재, 다이아프램 보강재 등 해당 번호 순서에 의한 것
//		 nCol		다이아프램 보강재, 가로보 수직보강재, 수평보강재(복부판, 가로보 등)
//					1번 인덱스, 2번째 줄-상부, 2번째 잭업의 수평보강재(nIdx = 0, nRow = 1, nCol= 2)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 부재에 대한 길이를 리턴
double	CPlateGirderApp::GetLengthJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam) const
{
	CPlateBridgeApp *pBridge = (CPlateBridgeApp *)GetBridgeApp();
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CGlobarOption   *pGlopt  = pBridge->m_pDataManage->GetGlobalOption();

	double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());

	switch(nMarkBuje)
	{
		case G_F_U : 	return GetLengthFlangeUpperJewon(pBx, TRUE, nDimType);
		case G_F_L :	return GetLengthFlangeUpperJewon(pBx, FALSE, nDimType);
		case G_W :		return GetLengthFlangeWebJewon(pBx, nDimType);
		case G_H_L:		return GetLengthHStiffJewon(pBx, nRow, nCol, TRUE, nDimType);
		case G_H_R:		return GetLengthHStiffJewon(pBx, nRow, nCol, FALSE, nDimType);
		case V_ST:		return GetLengthVStiff(pBx, nCol==0 ? TRUE : FALSE, DIM_BASIC);
		case V_J_ST:	return pBx->GetJijumStiff()->GetVStiffHeight(nRow==0?TRUE:FALSE, dHGir);
		//수직브레이싱
		case VB_UB: 
		case VB_LB: 
		case VB_CR:     return 0;
		case VB_GU:		return GetLengthVBracingGussetJewon(pBx, nCol, nDimType);
		case VB_BOLT:
			{				
				CVBracing*       pVr  = pBx->GetSection()->GetVBracing();
				if(pVr == NULL)	return 0;
				return pVr->m_dSL;
			}
		//수평브레이싱
		case HB_B:
		case HB_GU:		return GetLengthHBracingGussetJewon(pBx, nRow, nCol, nDimType);
		case HB_BOLT:
			{
				CHBracingGusset* pBrGu = pBx->GetHBracingGusset();
				if(pBrGu == NULL)	return 0;
				return pBrGu->GetSL();
			}
		case SP_U_U:		//현장이음 상판		
		case SP_U_L:
		case SP_U_FILL:
			{
				CPlateSplice*    pSp  = pBx->GetSplice();
				double dLen = (pSp->m_uA + pSp->m_uB*pSp->m_uC)*2 + pSp->m_uC1;
				return (nMarkBuje==SP_U_FILL) ? dLen/2 : dLen;
			}
		case SP_L_U:		//현장이음 하판
		case SP_L_L:
		case SP_L_FILL:
			{
				CPlateSplice*    pSp  = pBx->GetSplice();
				double dLen = (pSp->m_dA + pSp->m_dB*pSp->m_dC)*2 + pSp->m_dC1;
				return (nMarkBuje==SP_L_FILL) ? dLen/2 : dLen;
			}
		case SP_W_MO:
			{
				CPlateSplice*    pSp  = pBx->GetSplice();
				return (pSp->m_mA + pSp->m_mB*pSp->m_mC)*2 + pSp->m_mC1;
			}
		case SP_W_SH:
			{
				CPlateSplice*    pSp  = pBx->GetSplice();
				double dFL = (pSp->m_sA + pSp->m_sB*pSp->m_sC)*2 + pSp->m_sC1;
				double dMO = (pSp->m_mA + pSp->m_mB*pSp->m_mC)*2 + pSp->m_mC1;
				if(pSp->m_nWebType!=SP_WEB_FLMO)
					return dFL;
				else
					return max(dFL, dMO);
			}
		case SP_W_FILL:
			{
				if(nRow==0)	return GetLengthJewon(SP_W_MO, pBx, nRow, nCol, nDimType, bFront, bLeftBeam)/2;
				if(nRow==1)	return GetLengthJewon(SP_W_SH, pBx, nRow, nCol, nDimType, bFront, bLeftBeam)/2;
				if(nRow==2)	return GetLengthJewon(SP_W_MO, pBx, nRow, nCol, nDimType, bFront, bLeftBeam)/2;
			}
		case SP_U_BOLT:
			{
				CPlateSplice*    pSp  = pBx->GetSplice();
				return pSp->m_uL;
			}
		case SP_L_BOLT:
			{
				CPlateSplice*    pSp  = pBx->GetSplice();
				return pSp->m_dL;
			}
		case SP_W_BOLT:
			{
				CPlateSplice*    pSp  = pBx->GetSplice();
				return pSp->m_sL;
			}

		//가로보
		case CP_STEEL:
		case CG_STEEL:
		case CG_W:
		case CP_W:		return GetLengthCrossBeamWeb(pBx, nDimType);
		case CG_F_U:
		case CP_F_U:		return GetLengthCrossBeamFlange(pBx, nDimType);
		case CG_F_L:
		case CP_F_L:		return GetLengthCrossBeamFlange(pBx, nDimType);

		case CG_BR_W:
		case CP_BR_W:		return GetLengthCrossBeamBracketWeb(pBx, nCol, nDimType);//가로보 브라켓 복부판
		case CG_BR_F_U:
		case CP_BR_F_U:		return GetLengthCrossBeamBracketPlate(pBx, 0, nCol, nDimType);//가로보 브라켓 상판
		case CG_BR_F_L:
		case CP_BR_F_L:		return GetLengthCrossBeamBracketPlate(pBx, 1, nCol, nDimType);//가로보 브라켓 하판

		case CG_BR_GA:
		case CP_BR_GA:		return GetLengthCrossBeamBracketGasset(pBx, nCol, nDimType);//가로보 브라켓 보강재
		case CG_BR_GA_FA:
		case CP_BR_GA_FA:	return GetLengthCrossBeamBracketGassetPlate(pBx, nCol, nDimType);//가로보 브라켓 보강판
		case CG_V:
		case CP_V:			return GetLengthCrossBeamVStiff(pBx, nRow, nCol, nDimType);//가로보 수직보강재
		case CG_H:
		case CP_H:			return GetLengthCrossBeamHStiff(pBx, nRow, nCol, nDimType, bFront);//가로보 수평보강재
		case CG_SP_U_U:
		case CG_SP_U_L:
		case CP_SP_U_U:
		case CP_SP_U_L:		//가로보 현장이음 상판			
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				if(pC->m_bConnectionPlate && (pC->GetType()==CR_TYPE_MANUFACTURE || pC->GetType()==CR_TYPE_VSTIFF) )
				{
					return pC->m_uA+ (pC->m_uB1*pC->m_uC) + pC->m_uC1 + (pC->m_uB*pC->m_uC) + pC->m_uA;
				}
				return (pC->m_uA + pC->m_uB * pC->m_uC)*2 + pC->m_uC1;


			}
		case CG_SP_L_U:
		case CG_SP_L_L:
		case CP_SP_L_U:
		case CP_SP_L_L:		//가로보 현장이음 하판
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_dA*2 + (pC->m_dB * pC->m_dC)*2 + pC->m_dC1;
			}
		case CG_SP_W_MO:
		case CP_SP_W_MO:	//가로보 현장이음 복부판 모멘트
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return (pC->m_mA+pC->m_mB*pC->m_mC)*2 + pC->m_mC1;
			}
		case CG_SP_W_SH:
		case CP_SP_W_SH:	//가로보 현장이음 복부판 전단
		case CP_SP_W_FH:	//채움판..
		case CG_SP_W_FH:
			{
			CPlateCrossBeam* pC   = pBx->GetCrossBeam();
			if(pC == NULL)	return 0;

				if(nDimType==DIM_BASIC)	
					return pC->m_sW;
				else
				{
					long nBoltArrange = pGlopt->GetDesignBoltHoleArrangeCrossBeam();

					double dDis  = GetWidthCrossWebSpliceShare(pBx,nDimType);
					double Giul = (nBoltArrange==2) ? 0 : GetGiulCrossGasset(pBx,TRUE,TRUE);
					return pC->m_sW + fabs(dDis*Giul);
				}
			}
		case CG_BOLT:
		case CP_BOLT:	//가로보 현장이음 복부판 전단
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				if(nRow == 0)	return pC->m_uL;
				if(nRow == 1)	return pC->m_mL;
				if(nRow == 2)	return pC->m_sL;
				if(nRow == 3)	return pC->m_mL;
				if(nRow == 4)	return pC->m_dL;
				if(nRow == 5)	//채움판 볼트길이...
				{
					return pC->m_sL1;//pC->GetFillerBoltLength(pBx);
				}
				return 0;
			}

		// 기타부재
		case GI_SB:		return m_pSangse->m_StudHori_H + m_pSangse->m_StudHori_E;	// 전단연결재
		case GI_SQ:		return m_pSangse->m_StudHoriSQ_L;
		case GI_SA:		return (m_pSangse->m_SlabAnchor_A+m_pSangse->m_SlabAnchor_C)*2+m_pSangse->m_SlabAnchor_B; 
		case GI_SO:		return GetLengthPlateSolePlateJewon(pBx, nDimType);
		case GI_FA:		return GetLengthFacePlateJewon(pBx, nRow, nDimType);
		case GI_LP:		return 0;	// 추후 CHECK !!!	, 들고리
		case GI_LP_R:	return 0;	// 추후 CHECK !!!	, 들고리 링
		case GI_LP_PL:	return 0;	// 추후 CHECK !!!	, 들고리 보강재
		case GI_JA_L:	return pBx->GetJackupStiff()->m_dVL==0 ? pBx->GetJackupStiff()->GetHeight(TRUE) : pBx->GetJackupStiff()->m_dVL; 
		case GI_JA_R:	return pBx->GetJackupStiff()->m_dVL==0 ? pBx->GetJackupStiff()->GetHeight(FALSE) : pBx->GetJackupStiff()->m_dVL; 
		case GI_JA_SA:		
			{
				if(nCol>=2) return 0;
				long nJijum = GetNumberJijumByBx(pBx);
				return m_pBridgeApp->m_structJackupJewon[nJijum][nCol].m_dW;	// nCol = 전,후(0,1)
			}
		case GI_ZST:	return 0;
		case GI_HG:		
			{	if(pBridge->GetGirder(-1)->m_pSangse->m_Dongbari_Type==0) 
					return pBridge->GetGirder(-1)->m_pSangse->m_Dongbari_E*2;
				else
					return pBridge->GetGirder(-1)->m_pSangse->m_Dongbari_B;
			}		
	}
	return 0;
}


//입력 : nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 복부판의 너비를 리턴
double CPlateGirderApp::GetWidthCrossBeamBracketWebJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;
	CDPoint vAng = pBx->GetAngle();
	double dResult = 0;
	dResult = pC->m_P_CR/ vAng.y;

	return dResult;
}

//입력 : nRow		0=상판, 1=하판
//		 nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 상.하판의 너비
double CPlateGirderApp::GetWidthCrossBeamBracketPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CPlateGirderApp* pGir      = (CPlateGirderApp*)this;
	CPlateGirderApp* pGirR     = (CPlateGirderApp*)m_pRight;
	if(pGirR==NULL) return 0;
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR);
	if(pBxMatch == NULL) return 0;
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;
	double gH			 = GetHeightGirderByStaAng(pBx->GetStation(), 0, pBx->GetAngle());
	double dResult		 = 0;
	double dLengthGasset = 0;

	BOOL bUpper = (nRow == 0) ? TRUE : FALSE;
	CDPoint vAng	= pBx->GetAngle();
	double dSharp	= nCol ? pGir->GetLengthSharpOnBx(pBx, bUpper) : pGirR->GetLengthSharpOnBx(pBxMatch, bUpper);

	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		if(nRow==0)	//상판
		{
			if(pC->m_dLH1>0)
				dLengthGasset = pC->m_P_CR;
			else
				dLengthGasset = pC->m_P_CR - dSharp;
		}
		if(nRow==1)	//하판
		{
			if(pC->m_H_CR == gH)
				dLengthGasset = pC->m_P_CR - dSharp;
			else
				dLengthGasset = pC->m_P_CR;
		}
		dResult = dLengthGasset/vAng.y;
	}
	else
	{
		long nMarkBuje = (nRow==0) ? CG_BR_F_U : CG_BR_F_L;
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje,NULL,nRow,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize().y;
	}

	return dResult;
}
//입력 : nCol		0=이전 가로보 브라켓, 1=이후 가로보 브라켓(현재 인덱스 기준)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 보강재의 너비
double CPlateGirderApp::GetWidthCrossBeamBracketGasset(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam((CPlateGirderApp*)m_pRight);
	CPlateBridgeApp  *pBridge  = (CPlateBridgeApp *)GetBridgeApp();

	if(pBxMatch == NULL) return 0;
	
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	CPlateCrossBeam *pCR= pBxMatch->GetSection()->GetCrossBeam();
	
	if(pC == NULL)	return 0;
	if(pC->m_cGasset_CR == FALSE)	return 0;

	double dResult = 0;
	if(nCol == 0)
	{
		if(pC->m_nLGirSpType)
			dResult = pC->m_P_CR/pBx->GetAngle().y - pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S2);
		else
			dResult = pC->m_P_CR/pBx->GetAngle().y - pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S2)
			        - pC->GetLengthGirWebToCrWeb(TRUE)/pBx->GetAngle().y;
	}
	else if(nCol == 1)
	{
		if(pC->m_nRGirSpType)
			dResult = pC->m_P_CR/pBxMatch->GetAngle().y - pBridge->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S2);
		else
			dResult = pC->m_P_CR/pBxMatch->GetAngle().y - pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S2)
					- pC->GetLengthGirWebToCrWeb(FALSE)/pBx->GetAngle().y;
	}

	return dResult;
}
//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 모멘트 이음판 너비
double CPlateGirderApp::GetWidthCrossBeamWebSpliceMoment(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;
	if(pC->m_mW == 0 || pC->m_mE == 0 || pC->m_mF == 0)	return 0;
	double dResult = pC->m_mW;

	return dResult;
}
//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 전단 이음판 너비
double CPlateGirderApp::GetWidthCrossBeamWebSpliceShare(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;
	double dResult = (pC->m_sA+pC->m_sB*pC->m_sC+pC->m_sC1/2)*2;

	return dResult;
}

//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 채움판 너비
double CPlateGirderApp::GetWidthCrossBeamWebSpliceFiller(CPlateBasicIndex *pBx) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;

	double dResult;
	if(pC->GetFillerThick() >= 9)//두께가 ... 9mm 이상이면...
		dResult = ((pC->m_sA+pC->m_sB*pC->m_sC+pC->m_sC1/2)*2)+pC->m_sA*2;
	else
		dResult = (pC->m_sA+pC->m_sB*pC->m_sC+pC->m_sC1/2)*2;

	return dResult;
}

double	CPlateGirderApp::GetLengthCrossBeamWeb(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateBridgeApp *pBridge   = (CPlateBridgeApp *)GetBridgeApp();
	CPlateGirderApp* pGir      = (CPlateGirderApp*)this;
	CPlateGirderApp* pGirR     = (CPlateGirderApp*)m_pRight;
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR);
	if(pBxMatch == NULL) return 0;
	CPlateCrossBeam	*pC		   = pBx->GetSection()->GetCrossBeam();
	CPlateCrossBeam	*pCR	   = pBxMatch->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;

	double dResult = 0;
	// DIM_BASIC
	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		double	dDistWeb		= pGir->GetDisPlanPlateWebToWeb(pBx);
		double	dLenGassetRight	= 0;
		double	dLenGassetNext	= 0;
		double	dWebToCrLeft	= pC->GetLengthGirWebToCrWeb(TRUE);
		double	dWebToCrRight	= pC->GetLengthGirWebToCrWeb(FALSE);
		CDPoint	ptAngle			= pBx->GetAngle();
		CDPoint	ptAngleMatch	= pBxMatch->GetAngle();

		if(!pBridge->IsExtLine(-1))
		{
			dLenGassetRight = pC->m_P_CR/pBx->GetAngle().y;
			dLenGassetNext  = pC->m_P_CR/pBxMatch->GetAngle().y;
		}

		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			dResult = dDistWeb - (dLenGassetRight + dLenGassetNext);
		else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
			dResult = dDistWeb - dLenGassetRight - dWebToCrLeft/ptAngle.y;
		else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
			dResult = dDistWeb - dLenGassetNext - dWebToCrRight/ptAngleMatch.y;
		else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			dResult = dDistWeb - (dWebToCrLeft + dWebToCrRight)/ptAngle.y;
	}
	else
	{
		CPlatePieceObject Obj(pBx->GetGirder(),CP_W);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).x;
	}

	return dResult;
}

//입력 : nCol		0=이전 가로보 브라켓, 1=이후 가로보 브라켓(현재 인덱스 기준)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 보강판의 길이(상하방향)
double	CPlateGirderApp::GetLengthCrossBeamBracketGassetPlate(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	CPlateBridgeApp *pBridge   = (CPlateBridgeApp *)GetBridgeApp();
	if(pC == NULL)	return 0;
	if(pC->m_cGasset_CR == FALSE)	return 0;

	BOOL bLeft  = nCol==0 ? TRUE : FALSE;
	CDPoint Gp1, Gp2;
	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		Gp1 = pC->GetPointGassetSupPlateEndBx(pBx, pBx->GetAngle(), TRUE, bLeft);
		Gp2 = pC->GetPointGassetSupPlateEndBx(pBx, pBx->GetAngle(),FALSE, bLeft);
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if(bLeft)
		{
			Gp1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, pBx->GetAngle(), TRUE, bLeft);
			Gp2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, pBx->GetAngle(),FALSE, bLeft);
		}
		else
		{
			Gp1 = pC->GetPointGassetSupPlateEndBx(pBx, pBx->GetAngle(), TRUE, bLeft);
			Gp2 = pC->GetPointGassetSupPlateEndBx(pBx, pBx->GetAngle(),FALSE, bLeft);

		}
	}
	else if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if(bLeft)
		{
			Gp1 = pC->GetPointGassetSupPlateEndBx(pBx, pBx->GetAngle(), TRUE, bLeft);
			Gp2 = pC->GetPointGassetSupPlateEndBx(pBx, pBx->GetAngle(),FALSE, bLeft);
		}
		else
		{
			Gp1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, pBx->GetAngle(), TRUE, bLeft);
			Gp2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, pBx->GetAngle(),FALSE, bLeft);

		}
	}
	else 
	{
		Gp1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, pBx->GetAngle(), TRUE, bLeft);
		Gp2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, pBx->GetAngle(),FALSE, bLeft);
	}
		
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음

	double uB = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UB);
	double dB = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DB);
	double dResult = ~(Gp1-Gp2) - (uB + dB);

	return dResult;
}
double	CPlateGirderApp::GetLengthCrossBeamFlange(CPlateBasicIndex *pBx,  DIMTYPE nDimType) const
{
	CPlateBasicIndex *pBxN = GetBxMatchByCrossBeam(pBx, m_pRight);
	if(pBxN==NULL)	return 0;
	
	CPlateCrossBeam  *pC   = pBx->GetSection()->GetCrossBeam();	
	if(pC == NULL)	return 0;

	double dResult = 0;

	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			dResult = GetLengthCrossBeamWeb(pBx,nDimType);
		else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
		{
			dResult = GetLengthCrossBeamWeb(pBx,nDimType) - (pC->m_dbL1+pC->m_dbL2+pC->m_dbL3)/pBx->GetAngle().y;
		}
		else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			dResult = GetLengthCrossBeamWeb(pBx,nDimType) - (pC->m_dbR1+pC->m_dbR2+pC->m_dbR3)/pBx->GetAngle().y;
		}
		else
		{
			dResult = GetLengthCrossBeamWeb(pBx,nDimType) 
				    - (pC->m_dbL1+pC->m_dbL2+pC->m_dbL3+pC->m_dbR1+pC->m_dbR2+pC->m_dbR3)/pBx->GetAngle().y;
				     
		}
	}
	else
	{
		CPlatePieceObject Obj(pBx->GetGirder(), CP_F_U);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize().x;
	}

	// DIM_BASIC
	return dResult;

}

//입력 : nMarkBuje	부재 넘버 (전체 부재중에서)
//		 nRow		종리브, 수평보강재, 다이아프램 보강재 등 해당 번호 순서에 의한 것
//		 nCol		다이아프램 보강재, 가로보 수직보강재, 수평보강재(복부판, 가로보 등)
//					1번 인덱스, 2번째 줄-상부, 2번째 잭업의 수평보강재(nIdx = 0, nRow = 1, nCol= 2)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 부재에 대한 너비를 리턴
double	CPlateGirderApp::GetWidthJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam) const
{
	CPlateBridgeApp *pBridge	= (CPlateBridgeApp *)GetBridgeApp();
	CPlateGirderApp	*pGir		= pBx->GetGirder();
	CGlobarOption	*pOpt		= pBridge->m_pDataManage->GetGlobalOption();
	
	switch(nMarkBuje)
	{
		case G_F_U: return GetWidthOnStation(pBx->GetStation(), TRUE);
		case G_F_L: return GetWidthOnStation(pBx->GetStation(), FALSE);
		case G_W:   return GetWidthFlangeWebJewon(pBx, nDimType);
		
		case G_H_L: return GetWidthHStiffJewon(pBx, TRUE, nDimType);
		case G_H_R: return GetWidthHStiffJewon(pBx, FALSE, nDimType);
		
		case V_ST:  return GetWidthVStiff(pBx, nCol==0 ? TRUE : FALSE);
		case V_J_ST: return ((CPlateBasicIndex *)pBx)->GetJijumStiff()->m_dWidth[nRow];
		
		case VB_UB:
		case VB_LB:
		case VB_CR: return 0;
		case VB_GU: return GetWidthVBracingGussetJewon(pBx, nCol, nDimType);				//수직브레이싱		
		
		case HB_B:
		case HB_GU: return GetWidthHBracingGussetJewon(pBx, nRow, nCol, nDimType);			//수평브레이싱		

		case SP_U_U:
			{
				double dWidthPre	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
				double dWidthNext	= pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
				return min(dWidthPre, dWidthNext);
			}			
		case SP_U_L:	return pBx->GetSplice()->m_uW;
		case SP_U_FILL:	
			{
				nMarkBuje = pOpt->GetDesignHeigtBaseUpper() ? SP_U_U : SP_U_L;
				return GetWidthJewon(nMarkBuje, pBx, nRow, nCol, nDimType, bFront, bLeftBeam);
			}
		case SP_L_L:
			{
				double dWidthPre	= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
				double dWidthNext	= pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
				return min(dWidthPre, dWidthNext);
			}			
		case SP_L_U:	return pBx->GetSplice()->m_dW;
		case SP_L_FILL:	
			{
				nMarkBuje = !pOpt->GetDesignHeigtBaseLower() ? SP_L_L : SP_L_U;
				return GetWidthJewon(nMarkBuje, pBx, nRow, nCol, nDimType, bFront, bLeftBeam);
			}
		case SP_W_MO:	return GetWidthSpliceWebMomentJewon(pBx, nRow, nCol, nDimType);//현장이음 복부판 모멘트판
		case SP_W_SH:	return GetWidthSpliceWebShareJewon(pBx, nRow, nCol, nDimType); //현장이음 복부판 전단판
		case SP_W_FILL:
			{
				if(nRow==0) return GetWidthSpliceWebMomentJewon(pBx, nRow, nCol, nDimType); //현장이음 복부판 모멘트판
				if(nRow==1)	return GetWidthSpliceWebShareJewon(pBx, nRow, nCol, nDimType);	//현장이음 복부판 전단판
				if(nRow==2) return GetWidthSpliceWebMomentJewon(pBx, nRow, nCol, nDimType); //현장이음 복부판 모멘트판
			}
		case CG_W: case CP_W:
			{
				return  GetWidthCrossBeamWebPlate(pBx, nDimType);	//가로보
			}
		case CG_F_U:	case CP_F_U:	
			{
				return GetWidthCrossBeamFlangeUpper(pBx, nDimType);	//가로보 상판
			}
		case CG_F_L:	case CP_F_L:
			{
				return GetWidthCrossBeamFlangeLower(pBx, nDimType);
			}
		case CG_BR_W:	case CP_BR_W:		
			{
				return GetWidthCrossBeamBracketWebJewon(pBx, nCol, nDimType);	//가로보 브라켓 복부판
			}
		case CG_BR_F_U:	case CP_BR_F_U:
			{
				return GetWidthCrossBeamBracketPlate(pBx, 0, nCol, nDimType);	//가로보 브라켓 상판
			}
		case CG_BR_F_L:	case CP_BR_F_L:	
			{
				return GetWidthCrossBeamBracketPlate(pBx, 1, nCol, nDimType);	//가로보 브라켓 하판
			}
		case CG_BR_GA:	case CP_BR_GA:		
			{
				return GetWidthCrossBeamBracketGasset(pBx, nCol, nDimType);		//가로보 브라켓 보강재
			}
		case CG_BR_GA_FA:	case CP_BR_GA_FA:	
			{	
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				long nID=0;
				if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
					nID = 0;		//용접이음
				if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
					nID = 1;		//볼트이음
				return pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_W);//가로보 브라켓 보강판
			}
		case CG_V:	case CP_V:			
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_vW_CR;
			}//가로보 수직보강재
		case CG_H:	case CP_H:			
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_hW_CR;}//가로보 수평보강재
		case CP_SP_U_U:	case CG_SP_U_U:		
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				if(pC->m_bConnectionPlate && (pC->GetType()==CR_TYPE_MANUFACTURE || pC->GetType()==CR_TYPE_VSTIFF) )
					return pC->m_uM1*2;
				return pC->m_uM_CR*2;
			}	//가로보 현장이음 상판	

		case CP_SP_U_L:	case CG_SP_U_L:
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_uW;
			}
		case CP_SP_L_U:	case CG_SP_L_U:		
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_dW;
			}	//가로보 현장이음 하판 상

		case CG_SP_L_L:	case CP_SP_L_L:		
			{
				CPlateCrossBeam* pC   = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				if(pC->m_cGasset_CR == TRUE)
				{
					return pC->m_dW;
				}
				else
				{
					if(pC->m_bConnectionPlate && (pC->GetType()==CR_TYPE_MANUFACTURE || pC->GetType()==CR_TYPE_VSTIFF) )
						return pC->m_uM1*2;
					return pC->m_dM_CR*2;
				}

			}	//가로보 현장이음 하판 하
		case CG_SP_W_MO:	case CP_SP_W_MO:	
			{
				return GetWidthCrossBeamWebSpliceMoment(pBx, nDimType);	//가로보 현장이음 복부판 모멘트
			}
		case CG_SP_W_SH:	case CP_SP_W_SH:	
			{
				return GetWidthCrossBeamWebSpliceShare(pBx, nDimType);	//가로보 현장이음 복부판 전단
			}
		case CG_SP_W_FH:	case CP_SP_W_FH:
			{
				return GetWidthCrossBeamWebSpliceFiller(pBx);	//가로보 현장이음 복부판 전단
			}
		case GI_SB:			return 0;//전단연결재
		case GI_SQ:			return m_pSangse->m_StudHoriSQ_W;
		case GI_JA_SA:		
			{
				if(nCol>=2) return 0;
				long nJijum = GetNumberJijumByBx(pBx);
				return m_pBridgeApp->m_structJackupJewon[nJijum][nCol].m_dH;	// nCol = 전,후(0,1)
			}
		case GI_SO:			return GetWidthPlateSolePlateJewon(pBx, nDimType);		
		case GI_FA:			return 100;
		case GI_LP:			return 0;	// 추후 CHECK !!!	, 들고리
		case GI_LP_R:		return 0;	// 추후 CHECK !!!	, 들고리 링
		case GI_LP_PL:		return 0;	// 추후 CHECK !!!	, 들고리 보강재
		case GI_JA_L:		return pBx->GetJackupStiff()->GetWidth(TRUE);				 
		case GI_JA_R:		return pBx->GetJackupStiff()->GetWidth(FALSE);				 
		case GI_ZST:
		case GI_HG:			return pBridge->GetGirder(-1)->m_pSangse->m_Dongbari_W;
	}
	return 0;
}

double	CPlateGirderApp::GetWidthCrossBeamWebPlate(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
 	if(pC == NULL)	return 0;

	if(nDimType==DIM_RECT) 
	{
		CPlatePieceObject Obj(pBx->GetGirder(),CP_W);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		double dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).y;

		return dResult;
	}

	return pC->m_H_CR;
}
double	CPlateGirderApp::GetWidthCrossBeamFlangeUpper(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;

	double FoldLen = 0;
//	if(nDimType==DIM_RECT) 
//		FoldLen = GetDisCrossbeamByFolding(pBx);

	return pC->m_uM_CR*2 + fabs(FoldLen);
}
double	CPlateGirderApp::GetWidthCrossBeamFlangeLower(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;

	return pC->m_dM_CR*2;
}

double	CPlateGirderApp::GetThickJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam) const
{
	switch(nMarkBuje)
	{
		case G_F_U : 		
		case G_F_L :		
		case G_W :			return GetThickFactChainJewon(nMarkBuje, pBx);				
		case G_H_L :		return pBx->GetWebHStiff()->m_dThickHStiff[0];
		case G_H_R :		return pBx->GetWebHStiff()->m_dThickHStiff[1];
		
		case V_ST: return GetThickVStiff(pBx, nCol==0 ? TRUE : FALSE);
		case V_J_ST: return ((CPlateBasicIndex *)pBx)->GetJijumStiff()->m_dThick[nRow];
	
		case VB_UB:
		case VB_LB:
		case VB_CR: return GetThickVBracingSteelJewon(pBx, nMarkBuje);
		case VB_GU: return GetThickVBracingGussetJewon(pBx, nCol, nDimType);
		case VB_BOLT:
			{
				CVBracing *pVr = pBx->GetSection()->GetVBracing();
				if(pVr == NULL)	return 0;
				return atof(pVr->m_ZzsBoltSize.Mid(1,2));				
			}
		
		case HB_B: return 0;
		case HB_GU: return GetThickHBracingGussetJewon(pBx, nRow, nCol, nDimType);
		case HB_BOLT:
			{
				CHBracingGusset *pHGu = pBx->GetHBracingGusset();
				if(pHGu == NULL)	return 0;
				return atof(pHGu->m_ZzsBoltSize.Mid(1,2));				
			}

		case SP_U_U:		//현장이음 상판		
		case SP_U_L:		return pBx->GetSplice()->m_uT;
		case SP_U_FILL:
			{
				double dTPre = pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
				double dTNex = pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
				return fabs(dTPre-dTNex);
			}
		case SP_L_U:		//현장이음 하판
		case SP_L_L:		return pBx->GetSplice()->m_dT;
		case SP_L_FILL:
			{
				double dTPre = pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
				double dTNex = pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;
				return fabs(dTPre-dTNex);
			}
		case SP_W_MO:		return pBx->GetSplice()->m_mT;//모멘트 이음판
		case SP_W_SH:		return pBx->GetSplice()->m_sT;//전단 이음판
		case SP_W_FILL:
			{
				double dTPre = pBx->GetFactChain(G_W, FALSE)->m_dFactChainThick;
				double dTNex = pBx->GetFactChain(G_W, TRUE)->m_dFactChainThick;
				return fabs(dTPre-dTNex)/2;
			}
		case SP_U_BOLT:		//상판 볼트
		case SP_L_BOLT:		//하판 볼트
		case SP_W_BOLT:		return atof(pBx->GetSplice()->m_ZzsBoltSize.Mid(1,2));//상판 볼트
		
		case CG_W:
		case CP_W:
		case CG_BR_W:
		case CP_BR_W:
		case CG_BR_GA:
		case CP_BR_GA:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_T3_CR;
			}

		case CG_F_U:
		case CP_F_U:		//가로보 상판
		case CG_BR_F_U:
		case CP_BR_F_U:		//가로보 브라켓 상판
			{	
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_T2_CR;
			}

		case CP_STEEL:
		case CG_STEEL:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return MAX(pC->GetSteelMaterialData()->m_dT1, pC->GetSteelMaterialData()->m_dT2);
			}
		case CG_F_L:
		case CP_F_L:
		case CG_BR_F_L:
		case CP_BR_F_L:		//가로보 브라켓 하판
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_T1_CR;
			}

		case CG_BR_GA_FA:
		case CP_BR_GA_FA:
			{
				CPlateBridgeApp *pBridge   = (CPlateBridgeApp *)GetBridgeApp();
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				long nID=0;
				if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
					nID = 0;		//용접이음
				if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
					nID = 1;		//볼트이음
				return pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_T);	
			}
		case CG_V:
		case CP_V:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_vT_CR;
			}//가로보 수직보강재
		
		case CG_H:
		case CP_H:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_hT_CR;
			}//가로보 수평보강재
		
		case CP_SP_U_U:		//가로보 현장이음 상판	
		case CG_SP_U_U:
		case CP_SP_U_L:		
		case CG_SP_U_L:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_uT;
			}

		case CP_SP_L_U:		//가로보 현장이음 하판 상
		case CG_SP_L_U:
		case CG_SP_L_L:		//가로보 현장이음 하판 하
		case CP_SP_L_L:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_dT;
			}

		case CG_SP_W_MO:	//가로보 현장이음 복부판 모멘트
		case CP_SP_W_MO:
		case CG_SP_W_SH:	//가로보 현장이음 복부판 전단
		case CP_SP_W_SH:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->m_sT;
			}
		case CG_SP_W_FH:
		case CP_SP_W_FH: //채움판 두께...
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return pC->GetFillerThick();
			}
		case CG_BOLT:
		case CP_BOLT:
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if(pC == NULL)	return 0;
				return atof(pC->m_ZzsBoltSize.Mid(1,2));
			}
						
		case GI_SB:			return m_pSangse->m_StudHori_D;//전단연결재
		case GI_SQ:			return m_pSangse->m_StudHoriSQ_H;
		case GI_SA:			return m_pSangse->m_SlabAnchor_T;//전단연결재			
		case GI_SO:			return GetThickPlateSolePlateJewon(pBx, nDimType);
		case GI_FA:			return 10;
		case GI_LP:			return m_pSangse->m_LiftLug_T[0];//기타부재	들고리		임시 작업함.
		case GI_LP_R:		return m_pSangse->m_LiftLug_lT[0];//기타부재	들고리
		case GI_LP_PL:		return m_pSangse->m_LiftLug_sB[0];//기타부재	들고리
		case GI_JA_L:		return pBx->GetJackupStiff()->GetThick(TRUE);
		case GI_JA_R:		return pBx->GetJackupStiff()->GetThick(FALSE);
		case GI_JA_SA:		return GetThickSolePlateJackupJewon(pBx, nRow, nDimType);
		case GI_ZST:		return 0;
		case GI_HG:			return m_pSangse->m_Dongbari_T;
	}
	return 0;
}



//입력 : nJijum
//		 nVStiff 해당 지점의 지점보강재 순번 0~5(최대 6개)
CVStiff *CPlateGirderApp::GetJijumVStiffByJijum(long nJijum, long nVStiff) const
{
	if(nVStiff >= JACKUPSUMAX)	return NULL;

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_JIJUMSTIFF);
	long nJ = 0;
	while (pBx)
	{
		long nK = GetNumberJijumByBx(pBx);
		if(nK == nJijum)
		{
			if(nJ == nVStiff)	return pBx->GetJijumStiff();
			else				++nJ;
		}

		pBx = Finder.GetBxNext();
	}
	return NULL;
}

double	CPlateGirderApp::GetLengthSharpOnBx(CPlateBasicIndex* pBx, BOOL bUpper)
{
	if(pBx==NULL) return 0;
	double dWidthGir  = GetWidthOnStation(pBx->GetStation(), bUpper?TRUE:FALSE);
	double dThickWeb  = GetThickFactByStation(pBx->GetStation(), G_W);

	return dWidthGir/2-dThickWeb/2;
}

double	CPlateGirderApp::GetGiulPlateCrossGusset(CPlateBasicIndex* pBx)
{
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();	
	CDPoint A[10];
	pC->GetCrossXyDom(A);

	return (A[3]-A[2]).Unit().y;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///평면에서 pBx위치에서 매치되는 pBx까지 투영길이(2차원길이-웨브에서 웨브사이)
///////////////////////////////////////////////////////////////////////////////////////////////
double	CPlateGirderApp::GetDisPlanPlateWebToWeb(CPlateBasicIndex* pBx)
{
	CPlateBasicIndex *pBxRight = pBx->GetBxMatchByCrossBeam(m_pRight);
	if(m_pRight == NULL || pBxRight == NULL) return 0;
	CVector xyLeft, xyRight;
	double staLeft, staRight;

	double dTWeb  = GetThickFactByStation(pBx->GetStation(), 2);
	double dTWebN = GetThickFactByStation(pBxRight->GetStation(), 2);

	staLeft = GetStationBySc(pBx->GetStation(), dTWeb/2, pBx->GetAngle());
	staRight= m_pRight->GetStationBySc(pBxRight->GetStation(), -dTWebN/2, pBxRight->GetAngle());
	xyLeft  = GetXyGirderDis(staLeft, dTWeb/2);
	xyRight = m_pRight->GetXyGirderDis(staRight, -dTWebN/2);

	return ~(xyLeft - xyRight);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///정먄에서 보았을때 pBx위치에서 매치되는 pBx까지 투영길이(2차원길이-웨브에서 웨브사이)
///////////////////////////////////////////////////////////////////////////////////////////////
double	CPlateGirderApp::GetDisFrontPlateWebToWeb(CPlateBasicIndex* pBx)
{
	CPlateBasicIndex *pBxRight = pBx->GetBxMatchByCrossBeam(m_pRight);
	
	if(m_pRight == NULL || pBxRight == NULL) return 0;

	CVector xyLeft, xyRight;
	double dTWeb = GetThickFactByStation(pBx->GetStation(), G_W);
	double dTWebN = m_pRight->GetThickFactByStation(pBxRight->GetStation(), G_W);
	double dWGirN = m_pRight->GetWidthOnStation(pBxRight->GetStation(), TRUE);

	CDPoint A[10];
	GetHunchXyDom(pBx, A);
	
	xyLeft = (A[1]+A[2])/2;
	xyRight = A[7]+CDPoint(1,0)*(dWGirN/2);

	return ABS(xyLeft.x - xyRight.x)-(dTWeb+dTWebN)/2;
}

void CPlateGirderApp::GetBxWithenStation(CTypedPtrArray <CObArray, CPlateBasicIndex *> &ptrArray, double staStt, double staEnd, __int64 bxFindFlag/* = 0*/) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(bxFindFlag);
	while (pBx)   {
		if(staStt<=pBx->GetStation() && staEnd>=pBx->GetStation())
			ptrArray.Add(pBx);
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}	
}


//nCol=0 좌측상단 이음판
//nCol=1 우측상단 이음판
//nCol=2 좌측하단 이음판
//nCol=3 우측하단 이음판
//nCol=4 중앙     이음판
//횡단방향
double CPlateGirderApp::GetWidthVBracingGussetJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CVBracing* pVr = ((CPlateBasicIndex *)pBx)->GetSection()->GetVBracing();
	switch(nCol)
	{
	case 0:
		return pVr->m_duL3+pVr->m_duL4;
	case 1:
		return pVr->m_duR3+pVr->m_duR4;
	case 2:
		return pVr->m_ddL3+pVr->m_ddL4;
	case 3:
		return pVr->m_ddR3+pVr->m_ddR4;
	case 4:
		return pVr->m_dc3+pVr->m_dc4;
	}
	return 100;
}

double CPlateGirderApp::GetLengthVBracingGussetJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CVBracing* pVr = ((CPlateBasicIndex *)pBx)->GetSection()->GetVBracing();
	switch(nCol)
	{
	case 0:
		return pVr->m_duL1+pVr->m_duL2;
	case 1:
		return pVr->m_duR1+pVr->m_duR2;
	case 2:
		return pVr->m_ddL1+pVr->m_ddL2;
	case 3:
		return pVr->m_ddR1+pVr->m_ddR2;
	case 4:
		return pVr->m_dc1+pVr->m_dc2;
	}
	return 100;
}

double CPlateGirderApp::GetThickVBracingSteelJewon(CPlateBasicIndex*pBx, long nMarkBuje) const
{
	CVBracing* pVr = ((CPlateBasicIndex *)pBx)->GetSection()->GetVBracing();
	CSteelMaterialData	*pMat = NULL;
	switch(nMarkBuje)
	{
	case VB_UB:	pMat = pVr->GetSteelMaterialData(STEELMAT_UPPER);		break;
	case VB_LB: pMat = pVr->GetSteelMaterialData(STEELMAT_LOWER);		break;
	case VB_CR: pMat = pVr->GetSteelMaterialData(STEELMAT_DIAGONAL);	break;
	}
	return pMat ? pMat->m_dT1 : 0;
}

double CPlateGirderApp::GetThickVBracingGussetJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CVBracing* pVr = ((CPlateBasicIndex *)pBx)->GetSection()->GetVBracing();
	return pVr->m_dT;
}

double CPlateGirderApp::GetWidthHBracingGussetJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();
	if(!pBrGu)	return 0;
	if(nRow==0)	return pBrGu->m_dWidth[0]+pBrGu->m_dWidth[1];
	else		return pBrGu->m_dWidth[2]+pBrGu->m_dWidth[3];
}

double CPlateGirderApp::GetLengthHBracingGussetJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CHBracingGusset* pBrGu = ((CPlateBasicIndex *)pBx)->GetHBracingGusset();
	if(!pBrGu)	return 0;
	return pBrGu->m_dH[nRow];
}

double CPlateGirderApp::GetThickHBracingGussetJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CHBracingGusset* pBrGu = pBx->GetHBracingGusset();
	if(!pBrGu)	return 0;
	return pBrGu->m_dT;
}

// 0 : 거더축에서 첫번째 부재 - 1 : 거더축에서 첫번째 부재
CString CPlateGirderApp::GetStringHBracingBeamJewon(CPlateBasicIndex *pBx, long nCol, BOOL bUpper) const
{	
	CPlateBridgeApp		*pBridge	= (CPlateBridgeApp *)GetBridgeApp();
	CSteelMaterial		*pMat		= pBridge->m_pDataManage->GetSteelMaterial();
	CSection			*pSec		= pBx->GetSection();
	CBracing			*pBracing	= pBx->GetBracing();
	CSteelMaterialData	*pMatData	= pBracing->GetSteelMaterialData();

	ASSERT(pSec);

	long	nSteelType	= pMatData->m_nType;
	long	nJewonType	= pMatData->m_nJewonIdx;
	CString	szLenth		= COMMA(pBx->GetBracing()->GetBeamJewon(nCol, bUpper, CBracing::LENGTH));

	return pMat->GetJewonDataString(nSteelType, nJewonType) + "x" + szLenth;	
}

// 0 : Upper - 1 : Lower - 2,3 : 경사 부재
CString CPlateGirderApp::GetStringVBracingBeamJewon(CPlateBasicIndex *pBx, long nCol) const
{	
	CPlateBridgeApp	*pDB	= (CPlateBridgeApp *)GetBridgeApp();
	CSection		*pSec	= pBx->GetSection();
	CSteelMaterial	*pMat	= pDB->m_pDataManage->GetSteelMaterial();
	CVBracing		*pVr	= pSec->GetVBracing();

	ASSERT(pSec);

	if(pSec->IsenDetType(SECDET_VBRACING)==FALSE)		return "";

	long nSteelTypeUp = pVr->GetSteelMaterialData(STEELMAT_UPPER)->m_nType;
	long nSteelTypeLo = pVr->GetSteelMaterialData(STEELMAT_LOWER)->m_nType;
	long nSteelTypeC  = pVr->GetSteelMaterialData(STEELMAT_DIAGONAL)->m_nType;
	long nJewonTypeUp = pVr->GetSteelMaterialData(STEELMAT_UPPER)->m_nJewonIdx;
	long nJewonTypeLo = pVr->GetSteelMaterialData(STEELMAT_LOWER)->m_nJewonIdx;
	long nJewonTypeC  = pVr->GetSteelMaterialData(STEELMAT_DIAGONAL)->m_nJewonIdx;

	CString szLenth = COMMA(pBx->GetSection()->GetBeamJewon(nCol, CBracing::LENGTH));

	if(nCol==0)			return pMat->GetJewonDataString(nSteelTypeUp,nJewonTypeUp) + "x" + szLenth;	
	else if(nCol==1)	return pMat->GetJewonDataString(nSteelTypeLo,nJewonTypeLo) + "x" + szLenth;
	else if(nCol==2)	return pMat->GetJewonDataString(nSteelTypeC,nJewonTypeC) + "x" + szLenth;
	else if(nCol==3)	return pMat->GetJewonDataString(nSteelTypeC,nJewonTypeC) + "x" + szLenth;

	return "";
}

CString CPlateGirderApp::GetStringCrossHSteelJewon(CPlateBasicIndex *pBx) const
{
	CPlateBridgeApp	*pDB	= (CPlateBridgeApp *)GetBridgeApp();
	CSection		*pSec	= pBx->GetSection();
	CSteelMaterial	*pMat	= pDB->m_pDataManage->GetSteelMaterial();
	CPlateCrossBeam	*pCr	= pSec->GetCrossBeam();

	ASSERT(pSec);

	if(pSec->IsenDetType(SECDET_CROSSBEAM)==FALSE)		return "";

	long nSteelType = pCr->GetSteelMaterialData()->m_nType;
	long nJewonType = pCr->GetSteelMaterialData()->m_nJewonIdx;

	CString szLenth	= COMMA(GetLengthJewon(CP_STEEL, pBx));
	CString	szJewon	= pMat->GetJewonDataString(nSteelType,nJewonType) + "x" + szLenth;

	return szJewon;
}

//입력 : nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 복부판의 길이(높이)를 리턴
double CPlateGirderApp::GetLengthCrossBeamBracketWeb(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;

	double dResult = 0;
	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
		dResult = pC->m_H_CR;
	else
	{
		CPlatePieceObject Obj(pBx->GetGirder(),CP_BR_W,NULL,0,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).x;
	}

	return dResult;
}
//입력 : nRow		0=상판, 1=하판
//		 nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 상.하판의 길이(교축방향)
double CPlateGirderApp::GetLengthCrossBeamBracketPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;
	double dResult = 0;

	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		if(nRow == 0)		//상판
			dResult = (pC->m_uM1 + pC->m_uN_CR)*2;
		else if(nRow == 1)	//하판
			dResult = (pC->m_dM1 + pC->m_dN_CR)*2;
	}
	else
	{
		long nMarkBuje = (nRow==0) ? CG_BR_F_U : CG_BR_F_L;
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje,NULL,nRow,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize().x;
	}

	return dResult;
}
//입력 : nCol		0=이전 가로보 브라켓, 1=이후 가로보 브라켓(현재 인덱스 기준)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 보강재의 길이(상하방향)
double CPlateGirderApp::GetLengthCrossBeamBracketGasset(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam((CPlateGirderApp*)m_pRight);
	if(pBxMatch == NULL) return 0;
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();

	if(pC==NULL || pC->m_cGasset_CR==FALSE)	return 0;
	double dResult = 0;
	double dHGir = GetHeightGirderByStaAng(pBx->GetStation());
	double dHGirR = ((CPlateGirderApp*)m_pRight)->GetHeightGirderByStaAng(pBx->GetStation());
	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		if(!pC->m_nLGirSpType && nCol==0)
			dResult = pC->m_dltH;
		if(pC->m_nLGirSpType && nCol==0)
			dResult = dHGir - (pC->m_H_CR+pC->m_T1_CR+pC->m_T2_CR+pC->m_dLH1);
		if(!pC->m_nRGirSpType && nCol==1)
			dResult = pC->m_drtH;
		if(pC->m_nRGirSpType && nCol==1)
			dResult = dHGirR- (pC->m_H_CR+pC->m_T1_CR+pC->m_T2_CR+pC->m_dRH1);

	}
	else
	{
		long nMarkBuje = CP_BR_GA;
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje,NULL,0,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).x;
	}

	return dResult;
}


//입력 : nCol		0번째 수직보강재, 1번째 수직보강재...
//       nRow       0=정면의부재(시점기준) 1=뒷면의 부재
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 수직보강재의 길이
double CPlateGirderApp::GetLengthCrossBeamVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return 0;

	double Length = pC->m_vL_CR;

	CDPoint A[10];
	GetHunchXyDom(pBx,A);
// 	double disVS  = A[2].x;
// 	for(long i=0; i<=nCol; i++)
// 		disVS += pC->m_VStiffDis_CR[i];
// 	BOOL bFront = (nRow==0) ? TRUE : FALSE;
// 	long nCrossBeamSu = pBx->GetGirder()->GetQtyCrossBeamVBracing();
	
	// long nMeetStr = FALSE;//bFront ? CheckMeetCrossBeamVStiffStringerPrv(pBx,disVS) : CheckMeetCrossBeamVStiffStringer(pBx,disVS);
/*	if(nMeetStr != -1)
	{
		long nCross = bFront ? pBx->GetNumberCrossBeam()-1 : pBx->GetNumberCrossBeam();
		if(pBx->GetNumberCrossBeam()==0 && bFront) nCross = 0;
		CStringer *pStr = GetStringerByCrossNo(nCross, nMeetStr);
	if(pStr)
		{
			pStr->SetStringerPointAll();

			double GiulCross = GetGiulCrossBody(pBx, FALSE);
			double sGiul	 = pStr->GetGiulSeroboBody();
			double dDisStr	 = bFront ? GetDisStringerEnd(pStr->GetBx(),nMeetStr) : GetDisStringer(pBx,nMeetStr);
			if (pBx->GetNumberCrossBeam() == 0)
				dDisStr	= GetDisStringer(pBx,nMeetStr);
			else if (nCrossBeamSu-1 == pBx->GetNumberCrossBeam())
				dDisStr	= GetDisStringerEnd(pStr->GetBx(),nMeetStr);
			double dDisVs	 = (A[7].x+dDisStr-m_dUWidth/2) - disVS;
			double StaSc	 = GetStationBySc(pBx->GetStationCrossRight(),dDisStr,pBx->GetAngleSectionRight());
			double JGiul	 = GetGiulFlangeUpperLowerCamber(StaSc);
			if (pBx->GetNumberCrossBeam() == 0 && bFront)
				sGiul = JGiul;
			else if (!bFront && nCrossBeamSu-1 == pBx->GetNumberCrossBeam())
				sGiul = JGiul;

			double dHgtStr	 = pStr->m_H - dDisVs * GiulCross;
			CDPoint vBetAng  = pStr->GetBetweenAngleWithCrossBeam(TRUE);
			CDPoint vsGiul = CDPoint(1, sGiul).Unit();
			Length  = pC->m_vL_CR - (dHgtStr+pStr->m_T1/vsGiul.x);
			if(bFront)
				Length -= pC->m_uM_CR * (JGiul-sGiul);
			else
				Length += pC->m_uM_CR * (JGiul-sGiul);
		}
	}
*/
	if(nDimType==DIM_RECT) 
	{
		long nMarkBuje = CP_V;
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje,NULL,nRow,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		Length = Obj.GetRectSize(CDPoint(0,1),FALSE).x;

	}

	return Length;
}
//입력 : nRow		0=상부 1단, 1=상부 2단, 2=하부 2단, 3=하부 1단
//		 nCol		0=0번째 수직보강재...
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 수평보강재의 길이
// 길이는 기본적으로 실치수이기 때문에 bShopCut=TRUE임
double CPlateGirderApp::GetLengthCrossBeamHStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront) const
{
	CPlateBridgeApp *pDB   = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateCrossBeam *pC    = pBx->GetCrossBeam();
	CGlobarOption   *pGlopt= pDB->m_pDataManage->GetGlobalOption();

	CDPoint Po[18];	// 가로보상 수직부재의 중심포인트

	double sOffset    = m_pSangse->m_HStiff_Splice_B;
	double vOffset    = m_pSangse->m_HStiff_VStiff_B;
	BOOL bShopCut     = pDB->IsExtLine(-1) ? FALSE : TRUE;
	BOOL bIncludeSide = pGlopt->GetDesignCrossBeamHStiffAtBracket()==0 ? TRUE : FALSE;
	double giulCross  = GetGiulCrossBody(pBx, bShopCut);
	CDPoint vAng      = bShopCut ? pBx->GetAngleSectionRight() : CDPoint(0,1);

	long PoEa;
	if (bFront)	// 양면, 전면
		PoEa = GetPointVStiffOnCrossBeamPrv(pBx,Po,bShopCut,bIncludeSide);
	else		// 후면
		PoEa = GetPointVStiffOnCrossBeam(pBx,Po,bShopCut,bIncludeSide);

	double VT  = pC->m_vT_CR;									// 수직보강재 두께
	double WT  = GetThickFactChainJewon(G_W, pBx) / vAng.y;		// 수평보강재 두께
	double SW  = pC->m_sA + (pC->m_sB*pC->m_sC) + pC->m_sC1/2;  // 이음판의 폭 (/2) : 전단 이음판 폭
	double Len = 0;												// 수평보강재의 길이

	for(long n=0; n < PoEa-1; n++) 
	{
		if(Po[n].x==-1 && Po[n].y==-1) break;

		Len  = Po[n+1].x - Po[n].x - (vOffset+VT/2)*2;		// 수직보강재와 수직보강재 사이
		 // 이음면
		if(n==0 || n==1) 
		{
			if(n==0) // 좌측 복부판와 좌측 이음판 사이(이음판이 있을 경우만 진입)
				Len = Po[n+1].x - Po[n].x - (SW+sOffset+vOffset+WT);
			if(n==1) // 좌측 이음판과 첫번째 수직보강재
			{
				if(pC->m_nVStiffsu_CR==0) // 수직보강재가 없는 경우(복부판와 이음면포인트)
				{
					// 수평보강재의 길이는 이음판의 부착여부에 따라 달라짐...
					// 좌측에만 있을 경우
					if(pC->m_nLGirSpType && pC->m_nRGirSpType==FALSE)
						Len = Po[n+1].x - Po[n].x - (SW + sOffset);
					// 우측에만 있을 경우
					if(pC->m_nLGirSpType==FALSE && pC->m_nRGirSpType)
						Len = Po[n+1].x - Po[n].x - (SW + sOffset);
					// 양측 모두 있을 경우
					if(pC->m_nLGirSpType && pC->m_nRGirSpType)
						Len = Po[n+1].x - Po[n].x - (SW*2 + sOffset*2);
					// 양측 모두 이음판이 없을 경우..
					else if(pC->m_nLGirSpType==FALSE && pC->m_nRGirSpType==FALSE)
						Len  = Po[n+1].x - Po[n].x;
				}
				else	// 수직보강재가 있을 경우
				{
					Len = Po[n+1].x - Po[n].x - (SW+sOffset+vOffset+VT/2);
					if(pC->m_nLGirSpType==FALSE)
						Len  = Po[n+1].x - Po[n].x - (vOffset+VT/2);
				}
			}
		}
		else if(n==(PoEa-2) || n==(PoEa-3)) 
		{ // 이음면
			if(n==(PoEa-2)) 
				Len = Po[n+1].x - Po[n].x - (SW+sOffset+vOffset+WT);
			if(n==(PoEa-3)) 
			{
				Len  = Po[n+1].x - Po[n].x - (VT/2+vOffset+sOffset+SW);
				if(pC->m_nRGirSpType==FALSE)
					Len  = Po[n+1].x - Po[n].x - (VT/2+vOffset);
			}
		}
		else
		{
			Len  = Po[n+1].x - Po[n].x - (vOffset+VT/2)*2;
		}
		
		if(n==nCol)		break;

	}

	if(nDimType==DIM_RECT) 
		return Round(Len/cos(atan(giulCross)),0);

	return Round(Len,0);

}

CPlateBasicIndex* CPlateGirderApp::GetBxVBracingByNo(long nPos)
{
	CPlateBasicIndex* pBx = NULL;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayCRBeam;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayAType;
//	CPlateBasicIndexArray BasicIndexArrayCRBeam, BasicIndexArrayAType;
	GetBxWithenStation(BasicIndexArrayCRBeam, GetSSS(OFF_STT_STT), GetSSS(OFF_END_END), BX_CROSSBEAM_VBRACING);
	for(long n=0; n<BasicIndexArrayCRBeam.GetSize(); n++)
	{
		pBx = BasicIndexArrayCRBeam.GetAt(n);
		if(pBx->GetSection()->GetenDetType()==SECDET_VBRACING)
			BasicIndexArrayAType.Add(pBx);
	}
	return BasicIndexArrayAType.GetAt(nPos);
}

CPlateBasicIndex* CPlateGirderApp::GetBxATypeByNo(long nPos)
{
	CPlateBasicIndex* pBx = NULL;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayCRBeam;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayAType;
//	CPlateBasicIndexArray BasicIndexArrayCRBeam, BasicIndexArrayAType;
	GetBxWithenStation(BasicIndexArrayCRBeam, GetSSS(OFF_STT_STT), GetSSS(OFF_END_END), BX_CROSSBEAM_VBRACING);
	for(long n=0; n<BasicIndexArrayCRBeam.GetSize(); n++)
	{
		pBx = BasicIndexArrayCRBeam.GetAt(n);
		if(pBx->GetSection()->IsenType(SECTION_A))
			BasicIndexArrayAType.Add(pBx);
	}
	return BasicIndexArrayAType.GetAt(nPos);
}

CPlateBasicIndex* CPlateGirderApp::GetBxPTypeByNo(long nPos)
{
	CPlateBasicIndex* pBx = NULL;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayCRBeam;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayPType;
	//CPlateBasicIndexArray BasicIndexArrayCRBeam, BasicIndexArrayPType;
	GetBxWithenStation(BasicIndexArrayCRBeam, GetSSS(OFF_STT_STT), GetSSS(OFF_END_END), BX_CROSSBEAM_VBRACING);
	for(long n=0; n<BasicIndexArrayCRBeam.GetSize(); n++)
	{
		pBx = BasicIndexArrayCRBeam.GetAt(n);
		if(pBx->GetSection()->IsenType(SECTION_P))
			BasicIndexArrayPType.Add(pBx);
	}
	return BasicIndexArrayPType.GetAt(nPos);
}

CPlateBasicIndex* CPlateGirderApp::GetBxBTypeByNo(long nPos)
{
	CPlateBasicIndex* pBx = NULL;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayCRBeam;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayBType;
//	CPlateBasicIndexArray BasicIndexArrayCRBeam, BasicIndexArrayBType;
	GetBxWithenStation(BasicIndexArrayCRBeam, GetSSS(OFF_STT_STT), GetSSS(OFF_END_END), BX_CROSSBEAM_VBRACING);
	for(long n=0; n<BasicIndexArrayCRBeam.GetSize(); n++)
	{
		pBx = BasicIndexArrayCRBeam.GetAt(n);
		if(pBx->GetSection()->IsenType(SECTION_B))
			BasicIndexArrayBType.Add(pBx);
	}
	return BasicIndexArrayBType.GetAt(nPos);
}

CPlateBasicIndex* CPlateGirderApp::GetBxCTypeByNo(long nPos)
{
	CPlateBasicIndex* pBx = NULL;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayCRBeam;
	CTypedPtrArray <CObArray, CPlateBasicIndex *> BasicIndexArrayCType;
//	CPlateBasicIndexArray BasicIndexArrayCRBeam, BasicIndexArrayCType;
	GetBxWithenStation(BasicIndexArrayCRBeam, GetSSS(OFF_STT_STT), GetSSS(OFF_END_END), BX_CROSSBEAM_VBRACING);
	for(long n=0; n<BasicIndexArrayCRBeam.GetSize(); n++)
	{
		pBx = BasicIndexArrayCRBeam.GetAt(n);
		if(pBx->GetSection()->IsenType(SECTION_C))
			BasicIndexArrayCType.Add(pBx);
	}
	return BasicIndexArrayCType.GetAt(nPos);
}


CDPoint CPlateGirderApp::FindAngleSectionLeft(CPlateBasicIndex *pBx) const
{
	if(!pBx) return CDPoint(0,1);

	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(m_pLeft);
	if(!pBxMatch) return CDPoint(0,1);

	if(pBx->GetSection()->IsenDetType(SECDET_VSITFF))
		return CDPoint(0,1);

	double  CurSta = pBx->GetStation();
	CDPoint CurAng = pBx->GetAngle();
	double  MatSta = pBxMatch->GetStation();
	CDPoint MatAng = pBxMatch->GetAngle();
	
	if (CurSta == MatSta && CurAng.x == 0.0 && MatAng.x == 0.0)
		return CDPoint(0, 1);	

	CDPoint Curxy = GetXyGirderDis(CurSta);
	CDPoint Matxy = m_pLeft->GetXyGirderDis(MatSta);
	CDPoint vCross = (Matxy - Curxy).Unit();
	CDPoint vCrossAng = m_pLine->GetAngleAzimuth(CurSta).BetweenAngle(vCross);

	return vCrossAng;
}

CDPoint CPlateGirderApp::FindAngleSectionRight(CPlateBasicIndex *pBx) const
{
	if(!pBx || !pBx->IsState(BX_CROSSBEAM_VBRACING)) return CDPoint(0,1);

	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(m_pRight);
	BOOL bCenterGirder = pBx->GetNumberGirder()==-1 ? TRUE : FALSE;
	if(bCenterGirder)
	{
		pBx = pBx->GetBxMatchByCrossBeam(m_pBridgeApp->GetGirder(0));
		if(!pBx) return CDPoint(0,1);
		pBxMatch = pBx->GetBxMatchByCrossBeam(m_pBridgeApp->GetGirder(1));
	}
	if(!pBxMatch) return CDPoint(0,1);

	if(pBx->GetSection()->IsenDetType(SECDET_VSITFF))
		return CDPoint(0,1);

	double  CurSta = pBx->GetStation();
	CDPoint CurAng = pBx->GetAngle();
	double  MatSta = pBxMatch->GetStation();
	CDPoint MatAng = pBxMatch->GetAngle();
	
	if (CurSta == MatSta && CurAng.x == 0.0 && MatAng.x == 0.0)
		return CDPoint(0, 1);	

	CDPoint Curxy, Matxy, vCross, vCrossAng;
	double CenSta, CrossSta;
	CenSta = CurSta;
	Curxy = m_pLine->GetXyLineDis(CenSta, m_dGirderCenter);
	if(bCenterGirder)
	{
		Curxy = m_pBridgeApp->GetGirder(0)->GetXyGirderDis(CenSta);
		Matxy = m_pBridgeApp->GetGirder(1)->GetXyGirderDis(MatSta);
	}
	else
		Matxy = m_pRight->GetXyGirderDis(MatSta);

	vCross = (Curxy - Matxy).Unit();
	vCrossAng = m_pLine->GetAngleAzimuth(CenSta).BetweenAngle(vCross);
	CrossSta = m_pLine->GetStationByScInLine(CenSta, 0, vCrossAng, m_dGirderCenter);
	vCrossAng = m_pLine->GetAngleAzimuth(CrossSta).BetweenAngle(vCross);

	return vCrossAng;
}



//<summary>
//pBx 위치의 우측가로보가 현재 pBx 거더의 중앙과 만나는 스테이션을 구함
//</summary>
//<param name="*pBx"></param>
double CPlateGirderApp::FindStaCrossRigh(CPlateBasicIndex *pBx) const
{
	if(!pBx || !pBx->IsState(BX_CROSSBEAM_VBRACING)) return -1;
	CPlateBasicIndex *pBxMatch = pBx->GetBxInRighGirderSection();
	if(!pBxMatch) return -1;

	double  staBxLeft = pBx->GetStation();
	CDPoint vAngBxLeft = pBx->GetAngle();
	double  staBxRigh = pBxMatch->GetStation();
	CDPoint vAngBxRigh = pBxMatch->GetAngle();

	CDPoint xyBxLeft = pBx->GetGirder()->GetXyGirderDis(staBxLeft, pBx->GetGirder()->GetWidthFlangeUpper(pBx)/2, vAngBxLeft);
	CDPoint xyBxRigh = pBxMatch->GetGirder()->GetXyGirderDis(staBxRigh, -pBxMatch->GetGirder()->GetWidthFlangeUpper(pBx)/2, vAngBxRigh);
	CDPoint vAngCrossbeamAbs = (xyBxRigh - xyBxLeft).Unit();
	double staCrossbeam = 0;
	m_pLine->GetStationMatchByXyAndAngle(xyBxLeft, vAngCrossbeamAbs, staCrossbeam, m_dGirderCenter);
	return staCrossbeam;


}


//<summary>
//pBx 위치의 좌측가로보가 현재 pBx 거더의 중앙과 만나는 스테이션을 구함
//</summary>
//<param name="*pBx"></param>
double CPlateGirderApp::FindStaCrossLeft(CPlateBasicIndex *pBx) const
{
	if(!pBx || !pBx->IsState(BX_CROSSBEAM_VBRACING)) return -1;
	CPlateBasicIndex *pBxMatch = pBx->GetBxInLeftGirderSection();
	if(!pBxMatch) return -1;

	double  staBxLeft = pBx->GetStation();
	CDPoint vAngBxLeft = pBx->GetAngle();
	double  staBxRigh = pBxMatch->GetStation();
	CDPoint vAngBxRigh = pBxMatch->GetAngle();

	CDPoint xyBxLeft = pBx->GetGirder()->GetXyGirderDis(staBxLeft, -pBx->GetGirder()->GetWidthFlangeUpper(pBx)/2, vAngBxLeft);
	CDPoint xyBxRigh = pBxMatch->GetGirder()->GetXyGirderDis(staBxRigh, pBxMatch->GetGirder()->GetWidthFlangeUpper(pBx)/2, vAngBxRigh);
	CDPoint vAngCrossbeamAbs = (xyBxRigh - xyBxLeft).Unit();
	double staCrossbeam = 0;
	m_pLine->GetStationMatchByXyAndAngle(xyBxLeft, vAngCrossbeamAbs, staCrossbeam, m_dGirderCenter);
	return staCrossbeam;
}


double CPlateGirderApp::GetStationByPOS(POS psBx) const
{	
	return ((CPlateBasicIndex*)m_BxList.GetAt(psBx))->GetStation();
}

//입력 : nCross 가로보 번호
//기능 : nCross에서의 pBx를 구함
CPlateBasicIndex* CPlateGirderApp::GetBxByCrossNo(long nCross) const
{
	if(nCross >= GetQtyCrossBeamVBracing() || nCross < 0) return NULL;

	return m_CrossSectionArr[nCross]->GetBx();
}

//입력 : nFact  공장이음 nFact
//       nMarkBuje
//기능 : 공장이음 번호로 그 공장이음의 종점 Station을 구하기
double CPlateGirderApp::GetStationFactEndByFactNo(long nMarkBujeFact, long nFact)	const
{
	__int64 BxFlag;
	if(nMarkBujeFact == G_F_U) // 상부 
		BxFlag = BX_UPPERFLANGE | BX_END_GIRDER;
	else if(nMarkBujeFact == G_F_L) // 하부
		BxFlag = BX_LOWERFLANGE | BX_END_GIRDER;
	else if(nMarkBujeFact == G_W) // 복부판 좌
		BxFlag = BX_WEB | BX_END_GIRDER;
	
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
	for (long i = 0; i < nFact; i++)
		pBx = Finder.GetBxNext();
	if (pBx)		return pBx->GetStation();
	return GetSSS(OFF_END_GIRDER);

}

//기능 : 현재 pBx에서 가장 가까운 지점번호를 리턴
long CPlateGirderApp::GetNumberJijumByBx(CPlateBasicIndex *pBx) const
{
	return GetNumberJijumByStation(pBx->GetStation());
}
//기능 : 현재 station에서 가장 가까운 지점번호를 리턴
long CPlateGirderApp::GetNumberJijumByStation(double sta) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	double staJijum = 0;
	long nK = 0;
	for(nK=1; nK <= pDB->m_nQtyJigan; nK++)
	{
		staJijum = GetStationOnJijum(nK);
		if(sta <= staJijum)	break;
	}

	if((staJijum - sta) > (sta - GetStationOnJijum(nK-1)))	return nK-1;
	if(nK > pDB->m_nQtyJigan) return pDB->m_nQtyJigan;
	
	return nK;
}

double CPlateGirderApp::GetHeightMaxByJijum() const
{
	double h = 0;
	for(long n = 0; n < m_pBridgeApp->m_nQtyJigan+1;n++)
	{
		if( GetHeightGirderByJijum(n) > h ) h = GetHeightGirderByJijum(n);
	}
	
	return h;
}

//입력 : sta	현재 거더의 스테이션
//		 SD		현재 거더 센터에서의 이격거리 (좌측 -, 우측 +)
//		 vAng	현재 거더 센터와의 각도
//기능 : 각도차로 인한 실제 station
double CPlateGirderApp::GetStationBySc(double sta, double SD, CDPoint vAng) const
{
 	return m_pLine->GetStationByScInLine(sta, SD, vAng, m_dGirderCenter);
}
//입력 : sta	현재 거더의 스테이션
//		 SD		현재 거더 센터에서의 이격거리 (좌측 -, 우측 +)
//		 vAng	현재 거더 센터와의 각도
//기능 : 현재거더 중앙에서 이격거리의 좌표를 구함
CDPoint CPlateGirderApp::GetXyGirderDis(double sta,double D/*=0*/, CDPoint vAng/*(0,1)*/) const
{
	return	m_pLine->GetXyLineDis(GetStationBySc(sta, D, vAng), (m_dGirderCenter+D));
}


//입력 : sta : 기준 스테이션
//       len : dis만큼 이격거리에서의 실제 길이 (음수도 가능)
//       dis   거더 중심에서의 이격 거리 (좌측 -, 우측 +)
//기능 : (m_dGirderCenter+dis)만큼 이격된 거리에서 실제 len만큼 이동된 곳의 Station.
double CPlateGirderApp::GetStationByActLength(double sta, double len, double dis/*=0*/) const
{
	return m_pLine->GetStationByDisLength(sta, len, m_dGirderCenter+dis);
}


//기능 : 현재 거더의 교량 시점(교대 시점에서 자기 선형에 수직으로 내린) 스테이션
double CPlateGirderApp::GetStationStt() const
{
	return GetSSS(OFF_STT_STT);
}

//기능 : 각 거더 시점의 각도->확폭일 경우
//       모든거더의 기준은 도로 중심에 대한 각도(확폭이 아닌 모든 거더가 같음.
//기능 : 현재 거더의 교량 종점(교대 종점에서 자기 선형에 수직으로 내린) 스테이션
double CPlateGirderApp::GetStationEnd() const
{
	return GetSSS(OFF_END_END);
}


//기능 : 각 거더 시점의 각도(vector)
//       모든거더의 기준은 도로 중심에 대한 각도(확폭이 아닌 모든 거더가 같음.
CDPoint CPlateGirderApp::GetAngleStt() const
{
	return GetAAA(OFF_STT_STT);
}
CDPoint CPlateGirderApp::GetAngleEnd() const
{
	return GetAAA(OFF_END_END);
}
//입력 : nTarget	OFF_STT_SLAB		슬래브 시종점		항상 옵셋	
//					OFF_STT_GIRDER		거더 시종점			옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_SHOE		슈 시종점			옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_HUNCHSTT	시작헌치 시종점		옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_HUNCHEND	종점헌치 시종점		옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//모든거더의 기준은 도로 중심에 대한 실제 스테이션(확폭이 아닌 모든 거더가 같음.)
//GetStationStt()을 "0" 로 한 스테이션이 아님.******     
double CPlateGirderApp::GetSSS(SLABGIRDERSHOE nTarget, double dDist) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CGlobarOption   *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	BOOL bSttOffset = pDB->m_cOffSetStt;
	BOOL bEndOffset = pDB->m_cOffSetEnd;
	double dCover = 50;//	추후 수정
	long nHunchType = -1; //-1= 헌치가 아님, 0=90도, 1=시종점과 같은 각도, 2=선형과 같은 각도
	double dLenSttSlab = pDB->m_dLengthUganStt + dDist;
	double dLenSttCover= pDB->m_dLengthUganStt + dDist + dCover/fabs(pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).y);
	double dLenSttExp  = pDB->m_dLengthUganStt + pGlopt->GetSttExpansionJointWidth() + dDist;
	double dLenSttGirder = dLenSttSlab + pDB->m_dLengthSlabGirderStt + dDist;
	double dLenSttShoe = dLenSttGirder + pDB->m_dLengthGirderShoeStt + dDist;
	double dLenSttTaperS = dLenSttShoe + pDB->m_dLengthShoeToTaperStt + dDist;
	double dLenSttTaperE = dLenSttTaperS + pDB->m_dLengthTaperToTaperStt + dDist;
	double dLenEndSlab = -pDB->m_dLengthUganEnd + dDist;
	double dLenEndCover= -pDB->m_dLengthUganEnd + dDist - dCover/fabs(pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).y);
	double dLenEndExp  = -pDB->m_dLengthUganEnd - pGlopt->GetEndExpansionJointWidth() + dDist;
	double dLenEndGirder = dLenEndSlab - pDB->m_dLengthSlabGirderEnd + dDist;
	double dLenEndShoe = dLenEndGirder - pDB->m_dLengthGirderShoeEnd + dDist;
	double dLenEndTaperS = dLenEndShoe - pDB->m_dLengthShoeToTaperEnd + dDist;
	double dLenEndTaperE = dLenEndTaperS - pDB->m_dLengthTaperToTaperEnd + dDist;

	switch(nTarget)
	{
	case OFF_STT_STT:
		return GetSssDis(TRUE, FALSE, 0, nHunchType);
	case OFF_STT_SLAB:
		return GetSssDis(TRUE, TRUE, dLenSttSlab, nHunchType);
	case OFF_STT_COVER:
		return GetSssDis(TRUE, TRUE, dLenSttCover, nHunchType);
	case OFF_STT_EXP:
		return GetSssDis(TRUE, TRUE, dLenSttExp, nHunchType);
	case OFF_STT_GIRDER:
		return GetSssDis(TRUE, bSttOffset, dLenSttGirder, nHunchType);
	case OFF_STT_SHOE:
		return GetSssDis(TRUE, bSttOffset, dLenSttShoe, nHunchType);
	case OFF_STT_HUNCHSTT:
		{
			if(pDB->m_dLengthShoeToTaperStt == 0 || pDB->m_dLengthTaperToTaperStt == 0 || pDB->m_dHeightTaperStt == 0)
				return GetSssDis(TRUE, bSttOffset, dLenSttShoe, nHunchType);//헌치가 없는 경우, 슈와 동일

			if(pDB->m_cTaperAngleTypeStt == 2)		bSttOffset = TRUE;	//헌치 옵셋
			else									bSttOffset = FALSE;

			return GetSssDis(TRUE, bSttOffset, dLenSttTaperS, pDB->m_cTaperAngleTypeStt);
		}
	case OFF_STT_HUNCHEND:
		{
			if(pDB->m_dLengthShoeToTaperStt == 0 || pDB->m_dLengthTaperToTaperStt == 0 || pDB->m_dHeightTaperStt == 0)
				return GetSssDis(TRUE, bSttOffset, dLenSttShoe, nHunchType);//헌치가 없는 경우, 슈와 동일

			if(pDB->m_cTaperAngleTypeStt == 2)		bSttOffset = TRUE;	//헌치 옵셋
			else									bSttOffset = FALSE;
			
			return GetSssDis(TRUE, bSttOffset, dLenSttTaperE, pDB->m_cTaperAngleTypeStt);
		}
	case OFF_END_END:
		return GetSssDis(FALSE, FALSE, 0, nHunchType);
	case OFF_END_SLAB:
		return GetSssDis(FALSE, TRUE, dLenEndSlab, nHunchType);
	case OFF_END_COVER:
		return GetSssDis(FALSE, TRUE, dLenEndCover, nHunchType);
	case OFF_END_EXP:
		return GetSssDis(FALSE, TRUE, dLenEndExp, nHunchType);
	case OFF_END_GIRDER:
		return GetSssDis(FALSE, bEndOffset, dLenEndGirder, nHunchType);
	case OFF_END_SHOE:
		return GetSssDis(FALSE, bEndOffset, dLenEndShoe, nHunchType);
	case OFF_END_HUNCHSTT:
		{
			if(pDB->m_dLengthShoeToTaperEnd == 0 || pDB->m_dLengthTaperToTaperEnd == 0 || pDB->m_dHeightTaperEnd == 0)
				return GetSssDis(FALSE, bEndOffset, dLenEndShoe, nHunchType);//헌치가 없는 경우, 슈와 동일

			if(pDB->m_cTaperAngleTypeEnd == 2)		bEndOffset = TRUE;	//헌치 옵셋
			else									bEndOffset = FALSE;
			
			return GetSssDis(FALSE, bEndOffset, dLenEndTaperS, pDB->m_cTaperAngleTypeEnd);
		}
	case OFF_END_HUNCHEND:
		{
			if(pDB->m_dLengthShoeToTaperEnd == 0 || pDB->m_dLengthTaperToTaperEnd == 0 || pDB->m_dHeightTaperEnd == 0)
				return GetSssDis(FALSE, bEndOffset, dLenEndShoe,  nHunchType);//헌치가 없는 경우, 슈와 동일

			if(pDB->m_cTaperAngleTypeEnd == 2)		bEndOffset = TRUE;	//헌치 옵셋
			else									bEndOffset = FALSE;
			
			return GetSssDis(FALSE, bEndOffset, dLenEndTaperE, pDB->m_cTaperAngleTypeEnd);
		}
	}

	return 0;
}
 
//<summary>
//현재 거더 시.종점에서 선형 중심 길이인 dis(항상 +) 스테이션 만큼 이동된 길이의 스테이션을 구함.
//</summary>
//<param name="dLenMove">시.종점 스테이션에서 이격된 선형 거리(기준 선형 기준), [+]값은 시점, [-]값은 종점</param>
//<param name="bOffset">옵셋 처리 할 것인지를 결정</param>
//<param name="bStt">시점.종점 기준을 구분</param>
//<param name="nHunchType"> -1= 헌치가 아님, 0=90도, 1=시종점과 동일 각도, 2=시종점과 옵셋각도, 2=각 선형기준 동일(확폭)<br>
//						"0" 경우에만 각도가 90도로 고정이고 나머지는 bOffSet에 따라서 결정 </param>
//<return></return>
double CPlateGirderApp::GetSssDis(BOOL bStt, BOOL bOffset, double dLenMove, long nHunchType) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CLineInfo  *pLineCR = pDB->GetGirder(-1)->GetLine();

	double	staCR = pDB->m_dStationBridgeStt;
	CDPoint vAngCR = pDB->m_vAngJijum[0];
	double	dLenW = 0;		//확폭 꺽임 길이
	if(bStt == FALSE)
	{
		staCR = pDB->m_dStationBridgeStt+pDB->GetLengthBridge();
		vAngCR = pDB->m_vAngJijum[pDB->m_nQtyJigan];
	}
	if(nHunchType == 0)		vAngCR = CDPoint(0, 1);		//90도

	if(!IsExtLineGirder())	//확폭 거더가 아니면
		return GetSssDisOrdinaryLine(staCR, vAngCR, bOffset, dLenMove);

	double dDisLine = pLineCR->GetDisMatchAngByTargetLine(m_pLine, staCR+dLenMove);
	if(dDisLine < 0)		//좌측일 경우
	{
		if(bStt)	dLenW = pDB->m_dDisExtLineSttLeft;
		else		dLenW = pDB->m_dDisExtLineEndLeft;
	}
	else
	{
		if(bStt)	dLenW = pDB->m_dDisExtLineSttRight;
		else		dLenW = pDB->m_dDisExtLineEndRight;
	}

	if(dLenW == 0 || nHunchType == 0 || nHunchType == 1 || nHunchType == 2)	//확폭 꺽임 길이가 0일 경우
		return GetSssDisExtLineSymple(staCR, vAngCR, bOffset, dLenMove);

	return GetSssDisExtLine(staCR, vAngCR, bOffset, dLenMove, dLenW);
}

//일반선형 일 경우(확폭이 아닐 경우)
double CPlateGirderApp::GetSssDisOrdinaryLine(double staCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const
{
	CDPoint vAngCRMove = vAngCR;
	if(bOffset)		vAngCRMove = m_pLine->GetAngleByOffset(staCR, staCR+dLenMove, vAngCR);

	return m_pLine->GetStationByScInLine(staCR+dLenMove, m_dGirderCenter, vAngCRMove);
}

//확폭이고 확폭 꺽임 길이가 0일 경우
double CPlateGirderApp::GetSssDisExtLineSymple(double staCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CLineInfo  *pLineCR = pDB->GetGirder(-1)->GetLine();

	CDPoint vAngCRMove = vAngCR;
	double  staCRMove = staCR + dLenMove;
	if(bOffset)				vAngCRMove = pLineCR->GetAngleByOffset(staCR, staCRMove, vAngCR);

	CDPoint vAngBang = pLineCR->GetAngleAzimuth(staCRMove);//방위각
	CDPoint xyCRMove  = pLineCR->GetXyLine(staCRMove);

 	double  staGir = 0;
	if(!m_pLine->GetStationMatchByXyAndAngle(xyCRMove, vAngBang.Rotate(vAngCRMove), staGir))	return 0;
	CDPoint xyGir = m_pLine->GetXyLine(staGir);
	CDPoint vAngBangTar = m_pLine->GetAngleAzimuth(staGir);
	CDPoint vAngGir = vAngBangTar.BetweenAngle((xyGir - xyCRMove).Unit());
	
	double  staReturn = m_pLine->GetStationByScInLine(staGir, m_dGirderCenter, vAngGir);
	return staReturn;
}

//확폭이고 확폭 꺽임 길이가 있을 경우
double CPlateGirderApp::GetSssDisExtLine(double staCR, CDPoint vAngCR, BOOL bOffset, double dLenMove, double dLenW) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp *pGirCR	= pDB->GetGirder(-1);
	CPlateGirderApp *pGirEnd	= pDB->GetGirder(0);
	if(dLenW > 0)		pGirEnd	= pDB->GetGirder(pDB->GetGirdersu()-1);

	CLineInfo  *pLineCR = pGirCR->GetLine();
	CLineInfo  *pLineEnd = pGirEnd->GetLine();

	CDPoint xyW = pLineCR->GetXyLineByLengthAndAngle(staCR, dLenW, vAngCR);

	double staCRMove = staCR+dLenMove;	//도로중심 현재위치(dLenMove) 이동된 스테이션
	CDPoint vAngBangCRMove = pLineCR->GetAngleAzimuth(staCRMove);
	CDPoint xyCRMove = pLineCR->GetXyLine(staCRMove);

	double staEnd = 0;
	pLineEnd->GetStationMatchVerticalByXy(xyW, staEnd);	//0번 또는 마지막 거더의 스테이션
	if(pGirEnd == this && !bOffset)		return staEnd+dLenMove;
	CDPoint vAngEnd = CDPoint(0, 1);	//0번 또는 마지막 거더의 각도 (항상 90도로 만남)
	double staEndMove = staEnd + dLenMove;
	CDPoint vAngBangEndMove = pLineEnd->GetAngleAzimuth(staEndMove);
	CDPoint xyEndMove = pLineEnd->GetXyLine(staEndMove);
	
	CDPoint vAngCRMove = vAngCR;
	CDPoint vAngEndMove = vAngEnd;
	if(bOffset)
	{
		vAngCRMove = pLineCR->GetAngleByOffset(staCR, staCRMove, vAngCR);
		vAngEndMove = pLineEnd->GetAngleByOffset(staEnd, staEndMove, vAngEnd);
	}

	CDPoint xyWMove;
	BOOL bMatch = GetXyMatchLineAndLine(xyCRMove, vAngBangCRMove.Rotate(vAngCRMove), xyEndMove, vAngBangEndMove.Rotate(vAngEndMove), xyWMove);

	double staGir;
	double staReturn;
	CDPoint vAngBangGir;
	CDPoint vAngGir;
	bMatch = m_pLine->GetStationMatchSegment(xyCRMove, xyWMove, staGir);
	if(bMatch)		//기준선형 꺽임 사이에 있을때
	{
		vAngBangGir = m_pLine->GetAngleAzimuth(staGir);
		vAngGir = vAngBangGir.BetweenAngle((xyWMove - xyCRMove).Unit());
		staReturn = m_pLine->GetStationByScInLine(staGir, m_dGirderCenter, vAngGir);

		return staReturn;
	}

	if(!m_pLine->GetStationMatchByXyAndAngle(xyEndMove, (xyWMove-xyEndMove).Unit(), staGir))	return 0;
	CDPoint xyGir = m_pLine->GetXyLine(staGir);
	vAngBangGir = m_pLine->GetAngleAzimuth(staGir);
	vAngGir = vAngBangGir.BetweenAngle((xyWMove - xyEndMove).Unit());
	staReturn = m_pLine->GetStationByScInLine(staGir, m_dGirderCenter, vAngGir);

	return staReturn;
} 

//입력 : nTarget	OFF_STT_SLAB		슬래브 시종점		항상 옵셋	
//					OFF_STT_GIRDER		거더 시종점			옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_SHOE		슈 시종점			옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_HUNCHSTT	시작헌치 시종점		옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_HUNCHEND	종점헌치 시종점		옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//각도의 기준은 도로 중심(선형기준)에 대한 각도(확폭이 아닌 모든 거더가 같음.)

CDPoint CPlateGirderApp::GetAAA(SLABGIRDERSHOE nTarget, double dDist) const
{
	CPlateBridgeApp* pDB	= (CPlateBridgeApp*)m_pBridgeApp;
	CGlobarOption   *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	BOOL	bSttOffset		= pDB->m_cOffSetStt;
	BOOL	bEndOffset		= pDB->m_cOffSetEnd;
	double	dCover			= 50;
	long	nHunchType		= -1; // -1: 헌치가 아님,  0: 90도, 1: 시종점과 같은 각도,  2: 선형과 같은 각도
	double	dLenSttSlab		= pDB->m_dLengthUganStt + dDist;
	double	dLenSttCover	= pDB->m_dLengthUganStt + dDist + dCover;
	double	dLenSttExp		= pDB->m_dLengthUganStt + pGlopt->GetSttExpansionJointWidth() + dDist;
	double	dLenSttGirder	= dLenSttSlab + pDB->m_dLengthSlabGirderStt + dDist;
	double	dLenSttShoe		= dLenSttGirder + pDB->m_dLengthGirderShoeStt + dDist;
	double	dLenSttTaperS	= dLenSttShoe + pDB->m_dLengthShoeToTaperStt + dDist;
	double	dLenSttTaperE	= dLenSttTaperS + pDB->m_dLengthTaperToTaperStt + dDist;
	double	dLenEndSlab		= -pDB->m_dLengthUganEnd + dDist;
	double	dLenEndCover	= -pDB->m_dLengthUganEnd + dDist - dCover;
	double	dLenEndExp		= -pDB->m_dLengthUganEnd - pGlopt->GetEndExpansionJointWidth() + dDist;
	double	dLenEndGirder	= dLenEndSlab - pDB->m_dLengthSlabGirderEnd + dDist;
	double	dLenEndShoe		= dLenEndGirder - pDB->m_dLengthGirderShoeEnd + dDist;
	double	dLenEndTaperS	= dLenEndShoe - pDB->m_dLengthShoeToTaperEnd + dDist;
	double	dLenEndTaperE	= dLenEndTaperS - pDB->m_dLengthTaperToTaperEnd + dDist;
	
	switch(nTarget)
	{
	case OFF_STT_STT:
		return GetAaaDis(TRUE, FALSE, 0, nHunchType);
	case OFF_STT_SLAB:
		return GetAaaDis(TRUE, TRUE, dLenSttSlab, nHunchType);
	case OFF_STT_COVER:
		return GetAaaDis(TRUE, TRUE, dLenSttCover, nHunchType);
	case OFF_STT_EXP:
		return GetAaaDis(TRUE, TRUE, dLenSttExp, nHunchType);
	case OFF_STT_GIRDER:
		return GetAaaDis(TRUE, bSttOffset, dLenSttGirder, nHunchType);
	case OFF_STT_SHOE:
		return GetAaaDis(TRUE, bSttOffset, dLenSttShoe, nHunchType);
	case OFF_STT_HUNCHSTT:
		{
			if(pDB->m_dLengthShoeToTaperStt == 0 || pDB->m_dLengthTaperToTaperStt == 0 || pDB->m_dHeightTaperStt == 0)
				return GetAaaDis(TRUE, bSttOffset, dLenSttShoe, nHunchType);//헌치가 없는 경우, 슈와 동일

			if(pDB->m_cTaperAngleTypeStt == 2)		bSttOffset = TRUE;	//헌치 옵셋
			else									bSttOffset = FALSE;
			
			return GetAaaDis(TRUE, bSttOffset, dLenSttTaperS, pDB->m_cTaperAngleTypeStt);
		}
	case OFF_STT_HUNCHEND:
		{
			if(pDB->m_dLengthShoeToTaperStt == 0 || pDB->m_dLengthTaperToTaperStt == 0 || pDB->m_dHeightTaperStt == 0)
				return GetAaaDis(TRUE, bSttOffset, dLenSttShoe, nHunchType);

			if(pDB->m_cTaperAngleTypeStt == 2)		bSttOffset = TRUE;	//헌치 옵셋
			else									bSttOffset = FALSE;
			
			return GetAaaDis(TRUE, bSttOffset, dLenSttTaperE, pDB->m_cTaperAngleTypeStt);
		}
	case OFF_END_END:
		return GetAaaDis(FALSE, FALSE, 0, nHunchType);
	case OFF_END_SLAB:
		return GetAaaDis(FALSE, TRUE, dLenEndSlab, nHunchType);
	case OFF_END_COVER:
		return GetAaaDis(FALSE, TRUE, dLenEndCover, nHunchType);
	case OFF_END_EXP:
		return GetAaaDis(FALSE, TRUE, dLenEndExp, nHunchType);
	case OFF_END_GIRDER:
		return GetAaaDis(FALSE, bEndOffset, dLenEndGirder, nHunchType);
	case OFF_END_SHOE:
		return GetAaaDis(FALSE, bEndOffset, dLenEndShoe, nHunchType);
	case OFF_END_HUNCHSTT:
		{
			if(pDB->m_dLengthShoeToTaperEnd == 0 || pDB->m_dLengthTaperToTaperEnd == 0 || pDB->m_dHeightTaperEnd == 0)
				return GetAaaDis(FALSE, bEndOffset, dLenEndShoe, nHunchType);

			if(pDB->m_cTaperAngleTypeEnd == 2)		bEndOffset = TRUE;	//헌치 옵셋
			else									bEndOffset = FALSE;
			
			return GetAaaDis(FALSE, bEndOffset, dLenEndTaperS, pDB->m_cTaperAngleTypeEnd);
		}
	case OFF_END_HUNCHEND:
		{
			if(pDB->m_dLengthShoeToTaperEnd == 0 || pDB->m_dLengthTaperToTaperEnd == 0 || pDB->m_dHeightTaperEnd == 0)
				return GetAaaDis(FALSE, bEndOffset, dLenEndShoe, nHunchType);

			if(pDB->m_cTaperAngleTypeEnd == 2)		bEndOffset = TRUE;	//헌치 옵셋
			else									bEndOffset = FALSE;
			
			return GetAaaDis(FALSE, bEndOffset, dLenEndTaperE, pDB->m_cTaperAngleTypeEnd);
		}
	}

	return CDPoint(0,1);
}

//<summary>
//현재 거더 시.종점에서 선형 중심 길이인 dis(항상 +) 스테이션 만큼 이동된 길이의 각도 구함.
//</summary>
//<param name="dLenMove">	시.종점 스테이션에서 이격된 선형 거리(기준 선형 기준), [+]값은 시점, [-]값은 종점</param>
//<param name="bOffset">	옵셋 처리 할 것인지를 결정</param>
//<param name="bStt">		시점.종점 기준을 구분</param>
//<param name="nHunchType"> -1= 헌치가 아님, 0=90도, 1=시종점과 동일 각도, 2=시종점과 옵셋각도, 2=각 선형기준 동일(확폭)<br>
//							"0" 경우에만 각도가 90도로 고정이고 나머지는 bOffSet에 따라서 결정 </param>
//<return></return>
CDPoint CPlateGirderApp::GetAaaDis(BOOL bStt, BOOL bOffset, double dLenMove, long nHunchType) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CLineInfo  *pLineCR = pDB->GetGirder(-1)->GetLine();

	double	staCR = pDB->m_dStationBridgeStt;
	CDPoint vAngCR = pDB->m_vAngJijum[0];
	double	dLenW = 0;		//확폭 꺽임 길이
	if(bStt == FALSE)
	{
		staCR = pDB->m_dStationBridgeStt+pDB->GetLengthBridge();
		vAngCR = pDB->m_vAngJijum[pDB->m_nQtyJigan];
	}
	if(nHunchType == 0)		vAngCR = CDPoint(0, 1);		//90도

	if(!IsExtLineGirder())	//확폭 거더가 아니면
		return GetAaaDisOrdinaryLine(staCR, vAngCR, bOffset, dLenMove);

	double dDisLine = pLineCR->GetDisMatchAngByTargetLine(m_pLine, staCR+dLenMove);
	if(dDisLine < 0)		//좌측일 경우
	{
		if(bStt)	dLenW = pDB->m_dDisExtLineSttLeft;
		else		dLenW = pDB->m_dDisExtLineEndLeft;
	}
	else
	{
		if(bStt)	dLenW = pDB->m_dDisExtLineSttRight;
		else		dLenW = pDB->m_dDisExtLineEndRight;
	}

	if(dLenW == 0 || nHunchType == 0 || nHunchType == 1 || nHunchType == 2)	//확폭 꺽임 길이가 0일 경우
		return GetAaaDisExtLineSymple(staCR, vAngCR, bOffset, dLenMove);

	return GetAaaDisExtLine(staCR, vAngCR, bOffset, dLenMove, dLenW);
}

//일반선형 일 경우(확폭이 아닐 경우)
CDPoint CPlateGirderApp::GetAaaDisOrdinaryLine(double staCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const
{
	CDPoint vAngCRMove = vAngCR;
	if(bOffset)		vAngCRMove = m_pLine->GetAngleByOffset(staCR, staCR+dLenMove, vAngCR);
	double  staGir = m_pLine->GetStationByScInLine(staCR, m_dGirderCenter, vAngCRMove);
	CDPoint vAngGir = m_pLine->GetAngleByOffset(staCR, staGir, vAngCRMove);

	return vAngGir;
}

//<summary>
//확폭이고 확폭 꺽임 길이가 0일 경우 dLenMove 만큼 움직인 지점의 각도를 구함
//
//</summary>
//<param name="staCR">기준선형 스테이션</param>
//<param name="vAngCR">기준선형 각도</param>
//<param name="bOffset">옵셋 여부</param>
//<param name="dLenMove">기준선형 이동거리(스테이션)</param>
//<return></return>
CDPoint CPlateGirderApp::GetAaaDisExtLineSymple(double staCR, CDPoint vAngCR, BOOL bOffset, double dLenMove) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CLineInfo  *pLineCR = pDB->GetGirder(-1)->GetLine();

	CDPoint vAngCRMove = vAngCR;
	double  staCRMove = staCR + dLenMove;
	if(bOffset)				vAngCRMove = pLineCR->GetAngleByOffset(staCR, staCRMove, vAngCR);

	CDPoint vAngBang = pLineCR->GetAngleAzimuth(staCRMove);//방위각
	CDPoint xyCRMove  = pLineCR->GetXyLine(staCRMove);

	double  staGir = 0;
	if(!m_pLine->GetStationMatchByXyAndAngle(xyCRMove, vAngBang.Rotate(vAngCRMove), staGir))	return CDPoint(0, 0);
	CDPoint xyGir = m_pLine->GetXyLine(staGir);
	CDPoint vAngBangTar = m_pLine->GetAngleAzimuth(staGir);
	CDPoint vAngGir = vAngBangTar.BetweenAngle((xyGir - xyCRMove).Unit());
	
	double  staReturn = m_pLine->GetStationByScInLine(staGir, m_dGirderCenter, vAngGir);
	CDPoint vAngReturn = m_pLine->GetAngleByOffset(staGir, staReturn, vAngGir);
	return vAngReturn;
}

//확폭이고 확폭 꺽임 길이가 있을 경우
CDPoint CPlateGirderApp::GetAaaDisExtLine(double staCR, CDPoint vAngCR, BOOL bOffset, double dLenMove, double dLenW) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp *pGirCR	= pDB->GetGirder(-1);
	CPlateGirderApp *pGirEnd	= pDB->GetGirder(0);
	if(dLenW > 0)		pGirEnd	= pDB->GetGirder(pDB->GetGirdersu()-1);

	CLineInfo  *pLineCR = pGirCR->GetLine();
	CLineInfo  *pLineEnd = pGirEnd->GetLine();
	if(pGirEnd == this && !bOffset)		return CDPoint(0, 1);	//가장 좌.우측 거더는 무조건 90도

	CDPoint xyW = pLineCR->GetXyLineByLengthAndAngle(staCR, dLenW, vAngCR);

	double staCRMove = staCR+dLenMove;	//도로중심 현재위치(dLenMove) 이동된 스테이션
	CDPoint vAngBangCRMove = pLineCR->GetAngleAzimuth(staCRMove);
	CDPoint xyCRMove = pLineCR->GetXyLine(staCRMove);

	double staEnd = 0;
	pLineEnd->GetStationMatchVerticalByXy(xyW, staEnd);	//0번 또는 마지막 거더의 스테이션
	CDPoint vAngEnd = CDPoint(0, 1);	//0번 또는 마지막 거더의 각도 (항상 90도로 만남)
	double staEndMove = staEnd + dLenMove;
	CDPoint vAngBangEndMove = pLineEnd->GetAngleAzimuth(staEndMove);
	CDPoint xyEndMove = pLineEnd->GetXyLine(staEndMove);
	
	CDPoint vAngCRMove = vAngCR;
	CDPoint vAngEndMove = vAngEnd;
	if(bOffset)
	{
		vAngCRMove = pLineCR->GetAngleByOffset(staCR, staCRMove, vAngCR);
		vAngEndMove = pLineEnd->GetAngleByOffset(staEnd, staEndMove, vAngEnd);
	}

	CDPoint xyWMove;
	BOOL bMatch = GetXyMatchLineAndLine(xyCRMove, vAngBangCRMove.Rotate(vAngCRMove), xyEndMove, vAngBangEndMove.Rotate(vAngEndMove), xyWMove);

	double staGir;
	double staReturn;
	CDPoint vAngReturn;
	CDPoint vAngBangGir;
	CDPoint vAngGir;
	bMatch = m_pLine->GetStationMatchSegment(xyCRMove, xyWMove, staGir);
	if(bMatch)		//기준선형 꺽임 사이에 있을때
	{
		vAngBangGir = m_pLine->GetAngleAzimuth(staGir);
		vAngGir = vAngBangGir.BetweenAngle((xyWMove - xyCRMove).Unit());
		staReturn = m_pLine->GetStationByScInLine(staGir, m_dGirderCenter, vAngGir);
		vAngReturn = m_pLine->GetAngleByOffset(staGir, staReturn, vAngGir);

		return vAngReturn;
	}

	if(!m_pLine->GetStationMatchByXyAndAngle(xyEndMove, (xyWMove-xyEndMove).Unit(), staGir))	return CDPoint(0, 0);
	CDPoint xyGir = m_pLine->GetXyLine(staGir);
	vAngBangGir = m_pLine->GetAngleAzimuth(staGir);
	vAngGir = vAngBangGir.BetweenAngle((xyWMove - xyEndMove).Unit());
	staReturn = m_pLine->GetStationByScInLine(staGir, m_dGirderCenter, vAngGir);
	vAngReturn = m_pLine->GetAngleByOffset(staGir, staReturn, vAngGir);

	return vAngReturn;
}


//입력 : nJijum 교대,교각 번호 (시점 교대 = "0")
//기능 : nJijum 교대,교각의 거더 중심 스테이션을 리턴(슈 위치 스테이션)
double CPlateGirderApp::GetStationOnJijum(long nJijum) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);

	double retSta = 0;
	if(nJijum == 0)	retSta = GetSSS(OFF_STT_SHOE);
	else if(nJijum == pDB->m_nQtyJigan)	retSta = GetSSS(OFF_END_SHOE);
	else				retSta = GetStationOnPier(nJijum-1);

	return retSta;
}
//입력 : nPier   교각 번호 (첫번째 교각 = "0")
//기능 : nPier 교각의 거더 중심 스테이션을 리턴
double CPlateGirderApp::GetStationOnPier(long nPier) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);
	CLineInfo*	pLineCR = pGirCR->GetLine();
//	if(nPier < pDB->m_nQtyJigan-2) return 0;
	double staReturn = 0;
	double staBridge = 0;
	double staCRMove = pDB->m_dStationBridgeStt + pDB->m_dLengthUganStt+pDB->m_dLengthSlabGirderStt + pDB->m_dLengthGirderShoeStt;
	for(long k = 0; k< nPier+1; k++)		//마지막 지간(종점 교대부)은 계산할 필요 없음
		staCRMove += pDB->m_dLengthJigan[k];

	CDPoint vAng = pDB->m_vAngJijum[nPier+1];

	if(m_pLine == pLineCR)	//확폭이 아니면
	{
		staReturn = m_pLine->GetStationByScInLine(staCRMove, m_dGirderCenter, vAng);
	}
	else		//확폭이면
	{
		CDPoint vAngBang = pLineCR->GetAngleAzimuth(staCRMove);//방위각
		CDPoint xyCRMove  = pLineCR->GetXyLine(staCRMove);
		if(!m_pLine->GetStationMatchByXyAndAngle(xyCRMove, vAngBang.Rotate(vAng), staBridge))	return 0;
		CDPoint xyTar = m_pLine->GetXyLine(staBridge);
		CDPoint vAngBangTar = m_pLine->GetAngleAzimuth(staBridge);
		CDPoint vAngTar = vAngBangTar.BetweenAngle((xyTar - xyCRMove).Unit());
		
		staReturn = m_pLine->GetStationByScInLine(staBridge, m_dGirderCenter, vAngTar);
	}

	return staReturn;
}

//입력 : nJijum 교대,교각 번호 (시점 교대 = "0")
//기능 : nJijum 교대,교각의 거더 중심 가도를 리턴(슈 위치 각도)
CDPoint CPlateGirderApp::GetAngleOnJijum(long nJijum) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);

	CDPoint retAng(0,1);
	if(nJijum == 0)	retAng = GetAAA(OFF_STT_SHOE);
	else if(nJijum == pDB->m_nQtyJigan)	retAng = GetAAA(OFF_END_SHOE);
	else				retAng = GetAngleOnPier(nJijum-1);

	return retAng;
}
//입력 : nPier   교각 번호 (첫번째 교각 = "0")
//기능 : nPier 교각의 거더 중심 각도를 리턴
CDPoint CPlateGirderApp::GetAngleOnPier(long nPier) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);
	CLineInfo*	pLineCR = pGirCR->GetLine();
//	if(nPier < pDB->m_nQtyJigan-2) return CDPoint(0,1);
	CDPoint vAngRet(0,0);
	double staReturn = GetStationOnPier(nPier);
	double staBridge = 0;
	double staCRMove = pDB->m_dStationBridgeStt + pDB->m_dLengthUganStt+pDB->m_dLengthSlabGirderStt + pDB->m_dLengthGirderShoeStt;
	for(long k = 0; k< nPier+1; k++)		//마지막 지간(종점 교대부)은 계산할 필요 없음
		staCRMove += pDB->m_dLengthJigan[k];

	CDPoint vAng = pDB->m_vAngJijum[nPier+1];
	if(m_pLine == pLineCR)	//확폭이 아니면
	{
		vAngRet = m_pLine->GetAngleByOffset(staCRMove, staReturn, vAng);
	}
	else		//확폭이면
	{
		CDPoint vAngBang = pLineCR->GetAngleAzimuth(staCRMove);//방위각
		CDPoint xyCRMove  = pLineCR->GetXyLine(staCRMove);
		if(!m_pLine->GetStationMatchByXyAndAngle(xyCRMove, vAngBang.Rotate(vAng), staBridge)) return CDPoint(0, 0);
		CDPoint xyTar = m_pLine->GetXyLine(staBridge);
		CDPoint vAngBangTar = m_pLine->GetAngleAzimuth(staBridge);
		CDPoint vAngTar = vAngBangTar.BetweenAngle((xyTar - xyCRMove).Unit());
		
			vAngRet = m_pLine->GetAngleByOffset(staBridge, staReturn, vAngTar);
	}

	return vAngRet;
}

//입력 : sta  구하려는 스테이션
//기능 : sta, D, vAng 점의 박스 높이(헌치부는 제외 됨)
//       해당 sta 에 거더가 없으면 "0"을 리턴함.
double CPlateGirderApp::GetHeightGirderByStaAng(double sta, double D, CDPoint vAng, BOOL bReal, BOOL bSlab) const
{
	double dResult = GetHeightFlangeLower(GetStationBySc(sta, D, vAng),D, bReal) - GetHeightFlangeUpperByTaper(sta, D, vAng, bSlab);
	
	long nJiganNo = GetNumberJiganByStation(sta);//변단인경우 거더높이 옵션적용을 위해.... 070228....
	if(bReal && IsByunDanMyunInJigan(nJiganNo))
		dResult -= GetThickFlangeByBase(sta,TRUE) + GetThickFlangeByBase(sta,FALSE);				
 	return dResult;
}

//입력 : sta  구하려는 스테이션
//기능 : sta 의 거더 높이(헌치부는 제외 됨)  Abut 높이 - Hunch 높이가 리턴
//       해당 sta 에 거더가 없으면 시.종점의 높이차를 리턴함.(헌치부가 아닐 경우 "0"을 리턴)
//<summary>
//
//</summary>
//<param name="sta"></param>
//<param name="D"></param>
//<param name="vAng"></param>
//<return></return>
double CPlateGirderApp::GetHeightFlangeUpperByTaper(double sta, double D, CDPoint vAng, BOOL bSlab) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	if(pDB->m_dHeightTaperStt == 0 && pDB->m_dHeightTaperEnd == 0)	return 0;
	if(pDB->IsTUGir() && !bSlab)	return 0;
//	if(sta <= GetSSS(OFF_STT_GIRDER))	return pDB->m_dHeightTaperStt;
//	if(sta >= GetSSS(OFF_END_GIRDER))	return pDB->m_dHeightTaperEnd;

	double staDis = GetStationBySc(sta, D, vAng);
	double staSttEnd = GetStationBySc(GetSSS(OFF_STT_HUNCHEND), D, GetAAA(OFF_STT_HUNCHEND));//시점부 높은쪽
	double staEndEnd = GetStationBySc(GetSSS(OFF_END_HUNCHEND), D, GetAAA(OFF_END_HUNCHEND));//종점부 높은쪽
	if(staDis >= staSttEnd && staDis <= staEndEnd)	return 0;

	double staSttStt = GetStationBySc(GetSSS(OFF_STT_HUNCHSTT), D, GetAAA(OFF_STT_HUNCHSTT));//시점부 낮은쪽
	if(staDis <= staSttStt)							return pDB->m_dHeightTaperStt;
	if(staDis > staSttStt && staDis < staSttEnd)	return (staSttEnd-staDis) * pDB->m_dHeightTaperStt /(staSttEnd-staSttStt);

	double dStaEHS = GetSSS(OFF_END_HUNCHSTT);
	CDPoint vAngEHS = GetAAA(OFF_END_HUNCHSTT);
	double staEndStt = GetStationBySc(dStaEHS, D, vAngEHS);//종점부 낮은쪽
	if(staDis >= staEndStt)					return pDB->m_dHeightTaperEnd;
	if(staDis > staEndEnd && staDis < staEndStt)	return (staDis-staEndEnd) * pDB->m_dHeightTaperEnd /(staEndStt-staEndEnd);

	return 0;
}

//입력 : sta  높이를 구하려는 스테이션
//기능 : sta 의 거더 높이(헌치부는 제외 됨)  - 헌치부 높이 상관없이 하면 까지의 높이
//       해당 sta 에 거더가 없으면 "-1"을 리턴함.
//<summary>
//상판에서의 높이를 리턴(테이퍼 부는 적용 안한 높이)
//</summary>
//<param name="sta"></param>
//<param name="D"></param>
//<return></return>
double CPlateGirderApp::GetHeightFlangeLower(double sta, double D, BOOL bReal) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);
	long nJiganNo = GetNumberJiganByStation(sta);
	if(sta < GetSSS(OFF_STT_GIRDER))	return GetHeightGirderByJijum(0,bReal);
	if(sta > GetSSS(OFF_END_GIRDER))	return GetHeightGirderByJijum(nJiganNo+1,bReal);

	double heightLower = GetHeightGirderBySta(sta, bReal);

	if(IsByunDanMyunInJigan(nJiganNo))		//변단면이면
	{
		double staJijum = Round(GetStationOnJijum(nJiganNo), 0);//소수점 아래 값으로 높이가 달라지는 것을 방지함.(사교 단면타입 동일적용 등)
		double len = sta - staJijum;
		heightLower = GetHeightByunType(nJiganNo, len, pDB->m_nStyleByun[nJiganNo]);
	}
/*	else		//변단면이 아니고 지점 박스 높이가 다를 경우에만 사용하므로 이를 없다고 생각하고 삭제함.
	{
		double staJijumNext	   = GetStationOnJijum(nJiganNo+1);//해당 지간의 종점 지점 스테이션
		double heightJijum	   = GetHeightGirderByJijum(nJiganNo);
		double heightJijumNext = GetHeightGirderByJijum(nJiganNo+1);
		heightLower = (sta-staJijum) * (heightJijumNext-heightJijum) / (staJijumNext-staJijum);
		heightLower += heightJijum;
	}
*/
	return heightLower;
}
//입력 : sta  지간 넘버를 구하려는 스테이션
//기능 : 해당 sta의 지간 넘버를 리턴(0, 1, 2...)
//       해당 sta 에 거더가 없으면 "-1"을 리턴함.(시점슈 이전, 종점슈 이후 일 경우 해당 지간을 리턴)
long CPlateGirderApp::GetNumberJiganByStation(double sta) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	long k = 0;
	for(k = 0; k< pDB->m_nQtyJigan-1; k++)
	{
		if(sta < GetStationOnJijum(k+1))
			break;
	}
	return k;
}

//입력 : nJiganNo  점검하려는 지간 넘버
//기능 : 해당 지간이 변단면인지를 점검함(0=변단면타입이 아님, 5=레벨 타입)
BOOL CPlateGirderApp::IsByunDanMyunInJigan(long nJiganNo) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);
	if(nJiganNo > pDB->m_nQtyJigan-1)	return FALSE;

	if(pDB->m_cByunDan == 0  || pDB->m_nStyleByun[nJiganNo] == 0) return FALSE;
	if(GetHeightGirderByJijum(nJiganNo) == pDB->m_dHeightByun[nJiganNo] 
		&& GetHeightGirderByJijum(nJiganNo+1) == pDB->m_dHeightByun[nJiganNo]) return FALSE;  //높이 3개가 동일하면
	

	return TRUE;
}

//입력 : nJiganNo 현재 지간 넘버
//       len 해당 지간 시작 지점으로부터의 길이(도로중심 길이=>스테이션 개념)
//       nByunType => pDB->m_nStyleByun[nJiganNo]		변단면 종류 0~4
//기능 : 
double CPlateGirderApp::GetHeightByunType(long nJiganNo, double len, long nByunType) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);
	double jiganLen = GetStationOnJijum(nJiganNo+1) - GetStationOnJijum(nJiganNo); 
	double leftLen   = pDB->m_dLenLeftByun[nJiganNo];
	double leftCurv  = pDB->m_dLenLeftCurve[nJiganNo];
	double rightLen  = pDB->m_dLenRightByun[nJiganNo];
	double rightCurv = pDB->m_dLenRightCurve[nJiganNo];
	if(nByunType == 4)		//변단면타입 4번 일 경우
		leftCurv = rightCurv = (jiganLen - (leftLen+rightLen)) / 2;

	double cenLen = jiganLen - (leftLen+leftCurv+rightCurv + rightLen);
	double sttH = GetHeightGirderByJijum(nJiganNo, FALSE);
	double cenH = pDB->m_dHeightByun[nJiganNo];
	double endH = GetHeightGirderByJijum(nJiganNo+1, FALSE);
	double retH = 0;
	double diffH = 0;				//변단면 곡선 편차 항상 +를 취함.
	if(len <= leftLen)	//시점부에 있을 경우
		retH = sttH;
	else if(len >= (leftLen+leftCurv) && len <= (leftLen+leftCurv+cenLen))//중앙부에 있을 경우
		retH = cenH;
	else if(len >= (leftLen+leftCurv+cenLen+rightCurv))//종점부에 있을 경우
		retH = endH;
	else if(len > leftLen && len < (leftLen+leftCurv))//시점부 커브 구간일 경우
	{
		diffH = GetHeightByunByCurve(leftCurv, sttH-cenH, leftLen+leftCurv-len, pDB->m_nByunCurveType[nJiganNo]);
		retH = cenH + diffH;
	}
	else if(len > (leftLen+leftCurv+cenLen) && len < (leftLen+leftCurv+cenLen+rightCurv))//중앙부에 있을 경우
	{
		diffH = GetHeightByunByCurve(rightCurv, endH-cenH, len - (leftLen+leftCurv+cenLen), pDB->m_nByunCurveType[nJiganNo]);
		retH = cenH + diffH;
	}

	return retH;
}

double CPlateGirderApp::GetGiulJongDanOnlyLine(double sta, double D/*=0*/, CDPoint vAng/*= CDPoint(0,1)*/) const
{
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;

	double staCR = 0;
	pDB->m_pLine->GetStationMatchVerticalByXy(GetXyGirderDis(sta, D, vAng), staCR);
	return	m_bEleLevel ? 0 : pDB->GetGirder(-1)->GetLine()->GetJongDanGiul(staCR);
}

//<summary>
//현재 sta, D에 해당하는 캠버 & 종단 합의 기울기 - D 값에 의하여 다름
//상판일 경우 <Hunch 부>도 적용된 상태임 
//하판일 경우 <변단면> 도 적용된 상태임.
//</summary>
//<param name="sta"></param>
//<param name="D"></param>
//<param name="bUpper">TRUE = 상판 하면, FALSE = 하판 상면</param>
//<return></return>
double CPlateGirderApp::GetGiulFlangeUpperLower(double sta, double D, BOOL bUpper) const
{
//	return GetGiulJongDanOnlyLine(sta);
	double Gap = 0.1;
	double dElePrev;
	double dEleNext;
	double dLen  = GetLengthDimType(sta - Gap, sta + Gap, D);
	if(bUpper)
	{
		dElePrev = GetHeightFlangeUpperByTaper(sta - Gap, D);
		dEleNext = GetHeightFlangeUpperByTaper(sta + Gap, D);
	}
	else
	{
		dElePrev = GetHeightFlangeLower(sta - Gap,D);
		dEleNext = GetHeightFlangeLower(sta + Gap,D);
	}
	return ((dElePrev - dEleNext) / dLen) + GetGiulJongDanOnlyLine(sta);

}

//<summary>
//현재 sta, D에 해당하는 캠버 & 종단 합의 기울기 - D 값에 의하여 다름
//하판일 경우 <변단면> 도 적용된 상태임.
//</summary>
//<param name="sta"></param>
//<param name="D"></param>
//<param name="bUpper">TRUE = 상판 하면, FALSE = 하판 상면</param>
//<return></return>
double CPlateGirderApp::GetGiulFlangeUpperLowerCamber(double sta, double D, BOOL bUpper) const
{
	double Gap = 0.1;
	double ele1 = GetElevationFlangeCamber(sta - Gap, D, CDPoint(0,1), bUpper);
	double ele2 = GetElevationFlangeCamber(sta + Gap, D, CDPoint(0,1), bUpper);
	double len  = GetLengthDimType(sta - Gap, sta + Gap, D);
	return (ele2 - ele1) / len;
}

double CPlateGirderApp::GetGiulWebCamber(double sta, double D, BOOL bUpper) const
{
	double Gap = 0.1;
	double dSta1 = sta-Gap;
	double dSta2 = sta+Gap;
	CDPoint vAng = CDPoint(0,1);

	//////////////////////////////////////////////////////////////////////////
	double staSc1 = GetStationBySc(dSta1, D, vAng);
	double camber1 = GetCamberByStation(staSc1, 7);
	if(m_bEleLevel) camber1 = 0;

	double dEle1	= GetElevationWeb(sta, D, vAng, bUpper)+camber1;
	//////////////////////////////////////////////////////////////////////////
	double staSc2 = GetStationBySc(dSta2, D, vAng);
	double camber2 = GetCamberByStation(staSc2, 7);
	if(m_bEleLevel) camber2 = 0;

	double dEle2	= GetElevationWeb(sta, D, vAng, bUpper)+camber2;
	//////////////////////////////////////////////////////////////////////////	
	
	double len  = GetLengthDimType(dSta1, dSta2, D);
	return (dEle2 - dEle1) / len;	
}

//<summary>
//현재 인덱스의 하판(변단면아니면 0)의 기울기를 구한다.
//스테이션 진행방향 기준 올라가면[+]
//</summary>
//<param name="*pBx"></param>
//<return></return>
double CPlateGirderApp::GetGiulByunDan(CPlateBasicIndex *pBx) const
{
	double sta	= pBx->GetStation()-100;
	double sta2	= sta + 200;
	double dif	= GetHeightFlangeLower(sta);
	double dif2	= GetHeightFlangeLower(sta2);
	double dx = sta2 - sta;
	if(dx==0) return 0;
	return (dif-dif2) / dx;
}
//<summary>
//항상 기준선형을 기준으로 하여서 편경사값을 구함
//</summary>
//<param name="sta"></param>
//<param name="D"></param>
//<param name="vAng"></param>
//<return></return>
double CPlateGirderApp::GetPyungubaeCenLine(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp*	pDB		= (CPlateBridgeApp*)m_pBridgeApp;
	CDPoint xy = GetXyGirderDis(sta, D, vAng);
	CLineInfo* pLineCR	= pDB->GetGirder(-1)->GetLine();

	return pLineCR->GetPyungubaeByXy(xy);
}

//<summary>
//편경사 값을 구함. 
//현재 위치(D 에 의하여)가 도로중심 좌측에 있을 경우 [-]를 곱함. 좌측부터 기울기를 구할 경우 사용
//</summary>
//<param name="sta"></param>
//<param name="D"></param>
//<param name="vAng"></param>
//<return></return>
double CPlateGirderApp::GetPyungubaeFromLeft(double sta, double D, CDPoint vAng/*= CDPoint(0,1)*/) const
{
	double dPyungubae = GetPyungubaeCenLineOrGirLine(sta, D, vAng);
	double dLeft = 1;
	double disCenter = GetDisToCenterLineAct(sta,D, vAng);
	if(disCenter < 0)	dLeft = -1; //도로중심 보다 좌측에 있을 경우

	return dPyungubae * dLeft;
}



//<summary>
//해당 좌표에서 기준선형과 수직으로 만나는 선의 길이를 리턴(좌측 [-], 우측 [+])
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리(좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetDisToCenterLineAct(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp*	pDB		= (CPlateBridgeApp*)m_pBridgeApp;
	if(!IsExtLineGirder())		return m_dGirderCenter + D;

	return pDB->GetGirder(-1)->GetLine()->GetDisMatchVerticalByXy(GetXyGirderDis(sta, D, vAng));
}
 
//<summary>
//AShop - 엑셀 제원 점검에서만 사용함.
//해당 좌표에서 기준선형과 수직으로 만나는 선의 길이를 리턴(좌측 [-], 우측 [+])
// 확폭 좌우측 거더선형일 경우에는 거더 선형중심에서 기준선형까지의 이격거리
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리(좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetDisToCenterLineForExcel(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp*	pDB		= (CPlateBridgeApp*)m_pBridgeApp;
	if(!IsExtLineGirder())		return m_dGirderCenter + D;
	
	double dAng = vAng.GetAngleDegree(); //항상 0~360사이에 있음.
	CDPoint vAngGir = vAng;
	if(dAng > 180)	dAng -= 180;	//항상 0~180사이에 있도록 함.
	if(dAng > 88 && dAng < 92) //각도가 90도에 가까우면 항상 90도를 취함(엘리베이션에 한함. 평면 좌표 등은 항상 정확한 값)
	{
		vAngGir = CDPoint(0, 1);
	}

	CDPoint xy;//실제 거더중심 좌표
	//if(IsLineSameLeftOrRight() && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignExtLineElvationType()==1)	//램프 계획고, 램프 편경사 사용(좌우측 선형만)
	//	xy = m_pLine->GetXyLine(GetStationBySc(sta, D, vAngGir));
	//else
		xy = GetXyGirderDis(sta, D, vAngGir);//실제 거더중심 좌표

	return pDB->GetGirder(-1)->GetLine()->GetDisMatchVerticalByXy(xy);
}

//<summary>
//현재 거더 이격된 위치에서 도로중심에 만나는 도로중심(본선) 선상의 스테이션을 리턴
//단순하게 현재 위치에서 직각으로 만나는 위치의 도로중심 스테이션을 구함
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리 (좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetStationCenterLine(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp*	pDB		= (CPlateBridgeApp*)m_pBridgeApp;
	if(!IsExtLineGirder())		return GetStationBySc(sta, D, vAng);

	CDPoint xy = GetXyGirderDis(sta, D, vAng);
	double staCR = 0;
	pDB->GetGirder(-1)->GetLine()->GetStationMatchVerticalByXy(xy, staCR);
	return staCR;
}
//<summary>
//EL 을 구하기 위하여 기준선형의 스테이션을 구할 경우 사용
//확폭이고 가장 좌우측 선형(램프선형)일 경우 : 램프선형 중심 좌표에서 기준선형에 직각으로 내린 기준선형 스테이션
//확폭이고 가장 좌우측 선형(램프선형)아닐  경우 : 해당 좌표에서 기준선형에 직각으로 내린 기준선형 스테이션
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리 (좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetStationCenLineForEl(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp*	pDB		= (CPlateBridgeApp*)m_pBridgeApp;
	double dAng = vAng.GetAngleDegree(); //항상 0~360사이에 있음.
	CDPoint vAngGir = vAng;
	if(dAng > 180)	dAng -= 180;	//항상 0~180사이에 있도록 함.
	if(dAng > 88 && dAng < 92) //각도가 90도에 가까우면 항상 90도를 취함(엘리베이션에 한함. 평면 좌표 등은 항상 정확한 값)
	{
		vAngGir = CDPoint(0, 1);
	}

	if(!IsExtLineGirder())		return GetStationBySc(sta, D, vAngGir);//확폭이 아닐 경우

	CDPoint xy;
	//if(IsLineSameLeftOrRight() && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignExtLineElvationType()==1)	//램프 계획고, 램프 편경사 사용(좌우측 선형만)
	//	xy = m_pLine->GetXyLine(GetStationBySc(sta, D, vAngGir));
	//else
		xy = GetXyGirderDis(sta, D, vAngGir);

	double staCR = 0;
	pDB->GetGirder(-1)->GetLine()->GetStationMatchVerticalByXy(xy, staCR);
	return staCR;
}

//<summary>
//AShop - 엑셀 제원 점검에서만 사용함.
//확폭이 아닐 경우 거더 스테이션을 구함
//확폭이고 가장 좌우측 선형(램프선형)일 경우 :  자기 거더의 스테이션을 구함
//확폭이고 가장 좌우측 선형(램프선형)아닐  경우 : 해당 좌표에서 기준선형에 직각으로 내린 기준선형 스테이션
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리 (좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetStationCenLineOrGirLine(double sta, double D, CDPoint vAng) const
{
	//if(!IsExtLineGirder() || (IsLineSameLeftOrRight() && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignExtLineElvationType()==1))	//기준선형 또는 좌우측 선형이면
	//	return GetStationBySc(sta, D, vAng);//확폭이 아닐 경우

	return GetStationCenLineForEl(sta, D, vAng);
}

//<summary>
//기준선형의 계획고를 리턴
//</summary>
//<param name="sta">거더 스테이션</param>
//<param name="D/*=0*/">거더 중앙에서 이격거리</param>
//<param name="vAng">각도</param>
//<return></return>
double	CPlateGirderApp::GetElevationPlanElOnCenLineAct(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;

	double staCR = GetStationCenLineForEl(sta, D, vAng);
	return	pDB->GetGirder(-1)->GetLine()->GetElevationPlanEl(staCR);
}

//<summary>
//AShop - 엑셀 제원 점검에서만 사용함.
//확폭이 아닐 경우 거더 편경사를 구함
//확폭이고 가장 좌우측 선형(램프선형)일 경우 :  자기 거더의 편경사를 구함
//확폭이고 가장 좌우측 선형(램프선형)아닐  경우 : 해당 좌표에서 기준선형에 직각으로 내린 기준선형 편경사
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리 (좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetPyungubaeCenLineOrGirLine(double sta, double D, CDPoint vAng) const
{
	//if(!IsExtLineGirder() || (IsLineSameLeftOrRight() && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignExtLineElvationType()==1))	//기준선형 또는 좌우측 선형이면
	//	return GetLine()->GetPyungubaeByXy(GetXyGirderDis(sta, D, vAng));//확폭이 아닐 경우
	return GetPyungubaeCenLine(sta, D, vAng);
}
//<summary>
//AShop - 엑셀 제원 점검에서만 사용함.
//확폭이 아닐 경우 거더 편경사를 구함
//확폭이고 가장 좌우측 선형(램프선형)일 경우 :  자기 거더의 편경사를 구함
//확폭이고 가장 좌우측 선형(램프선형)아닐  경우 : 해당 좌표에서 기준선형에 직각으로 내린 기준선형 편경사
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리 (좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetDisLevelAct(double sta, double D/*=0*/, CDPoint vAng/*= CDPoint(0,1)*/) const
{
	double dis = GetDisToCenterLineAct(sta, D, vAng);
	if(dis < 0)		return -GetDisLevelLeftAct(sta, D, vAng);
	else			return GetDisLevelRightAct(sta, D, vAng);
}

//<summary>
//도로중심과의 사이에 있는 레벨 구간의 길이의 합을 리턴(좌측 구간)
//현재 좌표가 도로중심 좌측에 있을 경우
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리 (좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetDisLevelLeftAct(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp*	pDB		= (CPlateBridgeApp*)m_pBridgeApp;
	double dDisLevel	= 0;		//도로중심 좌측의 레벨 총 거리
	double dis = GetDisToCenterLineAct(sta, D, vAng);		//항상 [-], 좌측에 있음
	if(dis==0) return 0;
	double staCR = GetStationCenLineForEl(sta, D, vAng);
	double slabLeftDis = pDB->GetDisSlabLeftAct(staCR);	//도로중심에서 슬래브 좌측까지의 길이
	double slabWidth = pDB->GetWidthAct(staCR);
	double slabRightDis = pDB->GetDisSlabLeftAct(staCR) + slabWidth;	//도로중심에서 슬래브 우측까지의 길이
	if(slabLeftDis >= 0)							return fabs(dis);
	if(slabRightDis <= 0 && dis >= slabRightDis)
	{
		if(pDB->m_cLevelRoadCenterToSlabEdge != TRUE) //도로중심 간격 레벨적용 이면
			return 0;
		else
			return fabs(dis);
	}
	if(dis < slabLeftDis)	return 0;	//좌측 슬래브 끝단을 초과하였을 경우

	double disStt = slabLeftDis;
	double disEnd = slabLeftDis;
	double disNode = 0;
	for(long n=0; n<pDB->GetQtyHDanNode(); n++)
	{

		disNode = pDB->GetLengthHDanAct(staCR, n);
		disEnd += disNode;
		if(disEnd >= 0)
		{
			if(pDB->GetTypeLevelHDan(n) == 1)
				if(disStt > dis)		dDisLevel += disStt;	//항상 [-] 값이 나옴.
				else					dDisLevel += dis;	//D 값이 도로중심에서 레벨만을 가지고 있는 경우
			return dDisLevel;
		}

		if(pDB->GetTypeLevelHDan(n) == 1 && disEnd > dis)		//Level 이고 dis 보다 크면
		{
			if(disStt > dis)		dDisLevel += (-disNode);		//항상 [-] 값이 나옴.
			else					dDisLevel += (dis - disEnd);	//항상 [-] 값이 나옴.
		}
		disStt = disEnd;
	}
	if(slabRightDis < 0 && pDB->m_cLevelRoadCenterToSlabEdge == TRUE)	dDisLevel += slabRightDis;
	return dDisLevel;
}

//<summary>
//도로중심과의 사이에 있는 레벨 구간의 길이의 합을 리턴(우측 구간)
//현재 좌표가 도로중심 좌측에 있을 경우
//</summary>
//<param name="sta">거더 자신의 스테이션(거더 선형 중심)</param>
//<param name="D">거더 중심에서의 이격거리 (좌측 [-], 우측 [+])</param>
//<param name="vAng">각도</param>
//<return></return>
double CPlateGirderApp::GetDisLevelRightAct(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp*	pDB		= (CPlateBridgeApp*)m_pBridgeApp;
	double dDisLevel	= 0;		//도로중심 좌측의 레벨 총 거리
	double dis = GetDisToCenterLineAct(sta, D, vAng);		//항상 [+], 우측에 있음
	if(dis==0) return 0;
	double staCR = GetStationCenLineForEl(sta, D, vAng);
	double slabLeftDis =  pDB->GetDisSlabLeftAct(staCR);	//도로중심에서 슬래브 좌측까지의 길이
	double slabRightDis =  pDB->GetDisSlabLeftAct(staCR) + pDB->GetWidthAct(staCR);	//도로중심에서 슬래브 우측까지의 길이
	if(slabLeftDis >= 0 && dis <= slabLeftDis)
	{
		if(pDB->m_cLevelRoadCenterToSlabEdge != TRUE)
			return 0;
		else
			return fabs(dis);
	}
	if(slabRightDis <= 0)							return fabs(dis);
	if(dis > slabRightDis)	return 0;	//우측 슬래브 끝단을 초과하였을 경우

	double disStt = slabLeftDis;
	double disEnd = slabLeftDis;
	double disNode = 0;
	for(long n=0; n<pDB->GetQtyHDanNode(); n++)
	{
		disNode = pDB->GetLengthHDanAct(staCR, n);
		disEnd += disNode;

		if(pDB->GetTypeLevelHDan(n) == 1 && disEnd > 0 && disStt < dis)		//Level 이고 dis 보다 크면
		{
			if(disEnd < dis) {
				if(disStt>=0)		dDisLevel += disNode;		//항상 [+] 값이 나옴.
				else				dDisLevel  = (disNode+disStt);//항상 [+] 값이 나옴.
			}
			else
				dDisLevel += (dis - disStt);	//항상 [+] 값이 나옴.
		}
		disStt = disEnd;
		if(disStt > dis)	break;
	}
	if(slabLeftDis > 0 && pDB->m_cLevelRoadCenterToSlabEdge == TRUE)	dDisLevel += slabLeftDis;
	return dDisLevel;
}


//입력 : sta	스테이션
//		 nType	캠버의 종류
//		ⓞ종단 곡선에 의한 조정량
//		①편경사에 의한 조정량
//		②강재 자중에 의한 솟음량
//		③합성전 1차사하중에 의한 솟음량
//		④합성후 2차사하중에 의한 솟음량
//		⑤사용자 조정량1
//		⑥사용자 조정량2
//		⑦공장 제작시(2~6의 합계)
//		⑧현장 거치시(7-②-5-6)
//		⑨1차사하중 재하시(8-③)
//		10. 2차사하중 재하시(9-④)
//기능 : 
double CPlateGirderApp::GetCamberByStation(double sta, long nType) const
{
	CPlateBridgeApp*	pDB	= (CPlateBridgeApp*)m_pBridgeApp;
	
	long	nBridgeType	= pDB->GetBridgeType();
	double staNode1, staNode2, value1, value2, result;
	long nOutRange = 0;		//시.종점 슈를 벗어 났을 경우

	if(m_pCamber.GetSize() <= 0)	return 0;
	double staStt = GetCamberByNode(0)->GetCamberStation();
	double staEnd = GetCamberByNode(GetQtyNodeCamber()-1)->GetCamberStation();

	if (sta < staStt)
	{
		sta = staStt + (staStt - sta);
		nOutRange = -1;
	}
	if (sta > staEnd)
	{
		sta = staEnd - (sta - staEnd);
		nOutRange = 1;
	}

	if (nType == 0 || nType == 1)
	{  // 종단곡선 or 편경사에 의한 Camber
		double baseE, curE;
		long nJijum = GetNumberJiganByStation(sta);
		staNode1 = GetStationOnJijum(nJijum);
		staNode2 = GetStationOnJijum(nJijum+1);
		if(nType == 0)
		{  // 종단곡선
			if((IsLineSameLeftOrRight()) || (!IsLineSameLeftOrRight() && IsExtLineGirder()))
			{
				CLineInfo *pLineCR = pDB->GetGirder(-1)->GetLine();
				double staCR = GetStationCenLineForEl(sta);
				double staCR1 = GetStationCenLineForEl(staNode1);
				double staCR2 = GetStationCenLineForEl(staNode2);
			
				value1 = pLineCR->GetElevationPlanEl(staCR1);
				value2 = pLineCR->GetElevationPlanEl(staCR2);
				curE   = pLineCR->GetElevationPlanEl(staCR);
			}
			else
			{
				value1 = m_pLine->GetElevationPlanEl(staNode1);
				value2 = m_pLine->GetElevationPlanEl(staNode2);
				curE   = m_pLine->GetElevationPlanEl(sta);
			}
		}
		else
		{  // 편경사
			value1 = GetThickPyungubae(staNode1);
			value2 = GetThickPyungubae(staNode2);
			curE   = GetThickPyungubae(sta);
		}
		baseE  = (value2 - value1) / (staNode2 - staNode1) * (sta - staNode1) + value1;
		result = curE - baseE;
	}
	else
	{
		long nNode = GetCamberNodeByStation(sta);
		if(nNode >= GetQtyNodeCamber())		return 0;
		staNode1 = GetCamberByNode(nNode)->GetCamberStation();
		if(staNode1 == sta)		return GetCamberByNode(nNode)->GetCamberByType(nBridgeType, nType);
		else		
		{
			if(nNode+1 >= GetQtyNodeCamber())		return 0;
			staNode2 = GetCamberByNode(nNode+1)->GetCamberStation();
		}
		value1 = GetCamberByNode(nNode)->GetCamberByType(nBridgeType, nType);
		value2 = GetCamberByNode(nNode+1)->GetCamberByType(nBridgeType, nType);
		result = (value2 - value1) / (staNode2 - staNode1) * (sta - staNode1) + value1;
	}

	if (nOutRange == 0)		return result;

	if (nOutRange == -1)	value1 = GetCamberByNode(0)->GetCamberByType(nBridgeType, nType);	//시점 슈보다 스테이션이 작은 경우
	if (nOutRange == 1)		value1 = GetCamberByNode(GetQtyNodeCamber()-1)->GetCamberByType(nBridgeType, nType);//종점 슈보다 스테이션이 작은 경우
	
	return value1 - (result - value1);
}

//기능 : 현 sta보다 같거나 작은 캠버 노드를 리턴
long CPlateGirderApp::GetCamberNodeByStation(double dSta) const
{
	if(m_pCamber.GetSize() <= 0)	return 0;

	long nQtyNode = GetQtyNodeCamber();
	for (long nNode = 0; nNode < nQtyNode; nNode++) 
	{
		double dStaNode = GetCamberByNode(nNode)->GetCamberStation();
		if(nNode == 0 && dSta <= dStaNode)			return 0;
		if(nNode == nQtyNode-1 && dSta >= dStaNode)	return nNode;
		
		if(dSta == dStaNode)	return nNode;
		if(dSta < dStaNode)		return nNode-1;
	}
	return 0;
}
//기능 : 사용자 캠버 1, 2 중에서 전부 0이 아닌 캠버 수를 리턴
//		 사용자 캠버 1이 모두 0일 경우 사용자 캠버 2는 점검안하고 0을 리턴
long CPlateGirderApp::GetQtyCamberByUser() const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	if(m_pCamber.GetSize() <= 0)	return 0;

	long nBridgeType = pDB->GetBridgeType();
	long nQtyNode = GetQtyNodeCamber();
	long nQtyCamberByUser = 0;
	for (long nType = 5; nType < 7; nType++)
	{
		for (long nNode = 0; nNode < nQtyNode; nNode++) 
		{
			if (GetCamberByNode(nNode)->GetCamberByType(nBridgeType, nType) != 0.0)
			{
				nQtyCamberByUser++;
				break;
			}

		}
		if(nQtyCamberByUser == 0)	return 0;
	}
	return nQtyCamberByUser;
}

double CPlateGirderApp::GetMaxCamberInGirder() const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;

	if(m_pCamber.GetSize() <= 0)	return 0;

	long nBridgeType = pDB->GetBridgeType();
	long nQtyNode = GetQtyNodeCamber();
	double dMaxCamber = 0;

	for (long nType = 0; nType <= 10; nType++)
		for (long nNode = 0; nNode < nQtyNode; nNode++) 
			dMaxCamber = max(fabs(dMaxCamber), fabs(GetCamberByNode(nNode)->GetCamberByType(nBridgeType, nType)));

	return dMaxCamber;
}

//입력 : sta  거더 자신의 스테이션
//       nType  1= 상부..
//기능 : 현재 스테이션의 공장이음 넘버를 리턴함. 동일 위치인(경계선) 경우 이전 넘버를 리턴
long CPlateGirderApp::GetFactNumberByStationBefore(double sta, long nMarkBujeFact) const
{
	__int64 BxFlag;
	if(nMarkBujeFact == G_F_U) // 상부 
		BxFlag = BX_UPPERFLANGE | BX_END_GIRDER;
	else if(nMarkBujeFact == G_F_L) // 하부
		BxFlag = BX_LOWERFLANGE | BX_END_GIRDER;
	else if(nMarkBujeFact == G_W) // 복부판 좌
		BxFlag = BX_WEB | BX_END_GIRDER;
	
	long nFact = 0;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
	while(pBx)
	{
		if(pBx->GetStation() >= sta)
			return nFact;
		nFact++;
		pBx = Finder.GetBxNext();
	}
	return nFact - 1;
}
//입력 : x1		기준(변단면 곡선기준)에서의 x축 이격거리
//       x2		x2 > x1
//       D		 거더 중심에서의 이격거리
//		 dLengthByunOnly	
//		 dHeightByunMax
//기능 : 변단면 구간의 실제 길이(하판 좌우, 복부판 좌우 등)
//<summary>
//하판의 길이를 구함-변단면일 경우만 실행됨
//</summary>
//<param name="x1">시작 x좌표</param>
//<param name="x2">종점 x좌표</param>
//<param name="nJiganNo">지간 번호, 교대->P1까지 = 0, P1->P2 = 2....</param>
//<param name="dLengthByunOnly">변단면 구간 총길이</param>
//<param name="dHeightByunMax">변단면 높이차</param>
//<return></return>
double CPlateGirderApp::GetLengthFlangeLowerByByun(double x1, double x2, long nJiganNo, double dLengthByunOnly, double dHeightByunMax) const
{
	if(x2 == x1)	return 0;
	if(dHeightByunMax == 0)	
		return x2-x1;

	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pCG	= pDB->GetGirder(-1);
	double R = 0;
	if(pDB->m_nByunCurveType[nJiganNo] == 0)			//원
	{
		R = (dLengthByunOnly*dLengthByunOnly + dHeightByunMax*dHeightByunMax) / (2*dHeightByunMax);
	 	return GetLengthArcByTwoPoint(x1, x2, R);
	}
	else if(pDB->m_nByunCurveType[nJiganNo] == 1)	//2차 포물선
	{
		R = (dHeightByunMax) / (dLengthByunOnly*dLengthByunOnly);
		return GetLengthCurveMultiDegree(x1, x2, R, 2);
	}
	else if(pDB->m_nByunCurveType[nJiganNo] == 2)	//n차 포물선
	{
		R = log10(dHeightByunMax) / log10(dLengthByunOnly);
		return GetLengthCurveMultiDegree(x1, x2, 1, R);
	}
	else											//직선
		return sqrt(dLengthByunOnly*dLengthByunOnly + dHeightByunMax*dHeightByunMax)*(x2-x1) / dLengthByunOnly;
		
 	return 0;
}


//입력 : sta 현재 거더의 스테이션
//기능 : 현재 변단면 곡선의 R, 차수, A 값등을 리턴
double CPlateGirderApp::GetFactorByByunDan(double sta, double D) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	long nK = GetNumberJiganByStation(sta);
	long nStyleByun = pDB->m_nStyleByun[nK];
	double staStt, staEnd, heightByun = 0;

	if (!pDB->m_cByunDan || nStyleByun == 0)
		return 0.0;

	if (nStyleByun == 1)   {
		heightByun = GetHeightGirderByJijum(nK) - pDB->m_dHeightByun[nK];
		staStt = GetStationOnJijum(nK) + pDB->m_dLenLeftByun[nK];
		staEnd = staStt + pDB->m_dLenLeftCurve[nK];
	}
	else if (nStyleByun == 2)   {
		heightByun = GetHeightGirderByJijum(nK+1) - GetHeightGirderByJijum(nK);
		staEnd = GetStationOnJijum(nK+1) - pDB->m_dLenRightByun[nK];
		staStt = staEnd - pDB->m_dLenRightCurve[nK];
	}
	else if (nStyleByun == 3)   {
		heightByun = GetHeightGirderByJijum(nK) - GetHeightGirderByJijum(nK+1);
		staStt = GetStationOnJijum(nK) + pDB->m_dLenLeftByun[nK];
		staEnd = staStt + pDB->m_dLenLeftCurve[nK];
	}
	else if (nStyleByun == 4)   {
		heightByun = GetHeightGirderByJijum(nK) - pDB->m_dHeightByun[nK];
		staStt = GetStationOnJijum(nK) + pDB->m_dLenLeftByun[nK];
		staEnd = staStt + pDB->m_dLenLeftCurve[nK];
//		staEnd = staStt + pDB->m_dLenLeftCurve[nK] / 2;
	}
	double lenByun = GetLengthDimType(staStt, staEnd, D);


	return GetFactorByunByCurve(lenByun, heightByun, pDB->m_nByunCurveType[nK]);
}

void CPlateGirderApp::ContextOutBasicIndex(CString sPathName /*= _T("")*/) const
{
	if(sPathName.IsEmpty())
	{
	 	CFileDialog dlg(FALSE, "*.txt|*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		              "데이터 (*.txt)|*.txt|모든파일 (*.*)|*.*||");
	   dlg.m_ofn.lpstrTitle = "BasicIndex Context saved";

		if (dlg.DoModal() != IDOK) return;
		sPathName = dlg.GetPathName();
	}

	CFile F;
	if(F.Open(sPathName,CFile::modeCreate|CFile::modeWrite)==FALSE)
	{
		AfxMessageBox(CString(sPathName + " 파일을 열수 없습니다"));
		return;
	}


	long nCount = 0;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(NULL);
	CString sContext;
	while( pBx )
	{
		sContext.Empty();
		sContext.Format("Bx(%3d) : ",nCount++);
		pBx->ContextOutBx(sContext);
		F.Write(sContext,sContext.GetLength());
		
		pBx = Finder.GetBxNext();
	}

	F.Close();
}


// 구하고자 하는 위치들의 station(x), BxType(y)을 구함.
//입력 : dSttSta	시작 스테이션
//		 dEndSta	종점 스테이션
//		 BxFlag		Bx 종류
//		 b3DModel	Taper or 공장이음에 엇갈리게 걸리는 Bx 제거.
//기능 : dSttSta, dEndSta 포함, 상.하판 구분하여 <상.하판의 해당 기준 항목>의 스테이션을 리턴 
//       상.하판 면에 관련된 사항의 스테이션만 사용할 것.(좌.우 복부판 판은 사용하지 말것)
void CPlateGirderApp::GetStationArrayByFlangeSection(CDPointArray &StaArr, double dSttSta, double dEndSta, double D, __int64 BxFlag, __int64 BxCross) const
{
	double dis=0;

	if (dSttSta >= dEndSta)
		return;
	StaArr.RemoveAll();

	CDPoint data;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while (pBx)
	{
		//pBx = pBx->GetBxCenter();
		double sta = pBx->GetStation();
		if (pBx->IsState(BX_STT_GIRDER | BX_END_GIRDER | BX_BENDING | BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_JACKUP))
			sta = GetStationBySc(sta, D, pBx->GetAngle());

		if (dSttSta <= sta && sta <= dEndSta)
		{
			if (!BxCross || !GetCrossBxSu(pBx, dis, BxCross))   
			{
				data = CDPoint(sta, (double) pBx->GetBxFlagState());
				StaArr.Add(data);
			}
		}
		pBx = Finder.GetBxNext();
	}

	// Sort시작.. (Bx를 Center기준으로 잡으므로 사교에서는 순서 엇갈릴수 있음.)
	long i, j, nCount = StaArr.GetSize();
	for (i = 1; i < nCount; i++)   {
		CDPoint temp = StaArr[i];
		j = i;
		while (j > 0 && StaArr[j - 1].x > temp.x)   
		{
			StaArr.SetAt(j, StaArr[j - 1]);				
			j--;				
		}
		StaArr.SetAt(j, temp);
	}		

	if (StaArr.GetSize() == 0 || StaArr.GetAt(0).x > dSttSta)  // 구간 시작위치 저장
	{		
		data = CDPoint(dSttSta, 0);
		StaArr.InsertAt(0, data);
	}
	if (StaArr.GetAt(StaArr.GetUpperBound()).x < dEndSta)  // 구간 끝위치 저장
	{		
		data = CDPoint(dEndSta, 0);
		StaArr.Add(data);
	}
}


void CPlateGirderApp::GetStationArray(CDPointArray &StaArr, double dSttSta, double dEndSta, __int64 BxFlag) const
{
	if (dSttSta >= dEndSta)
		return;
	StaArr.RemoveAll();

	CDPoint data;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while (pBx)
	{
		//pBx = pBx->GetBxCenter();
		double sta = pBx->GetStation();
	
		if (dSttSta <= sta && sta <= dEndSta)
		{
			data = CDPoint(sta, (double) pBx->GetBxFlagState());
			StaArr.Add(data);
		}
		pBx = Finder.GetBxNext();
	}
}

//입력 : pBx (Center Bx임.)
//       D		거더 중심에서의 이격거리. (부호는 상관없음)
//기능 : pBx에서 ±D이격거리 사이에 지정된 Bx종류와의 교차수를 구함.
long CPlateGirderApp::GetCrossBxSu(CPlateBasicIndex *pBx, double D, __int64 BxFlag) const
{
	if (pBx->IsState(BxFlag))
		return 0;

	double sta = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();

	long nCount = 0;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxTaper = Finder.GetBxFirst(BxFlag);
	while (pBxTaper)
	{
		double staTaper = pBxTaper->GetStation();
		CDPoint vAngTaper = pBxTaper->GetAngle();
		if (vAng != vAngTaper && fabs(staTaper - sta) < 0)  // 우하하하... 속도 땜시...
		{
			CDPoint BxL = GetXyGirderDis(sta);
			CDPoint BxR = GetXyGirderDis(sta);
			CDPoint TaperL = GetXyGirderDis(staTaper);
			CDPoint TaperR = GetXyGirderDis(staTaper);
			if (GetXyMatchSegAndSeg(BxL, BxR, TaperL, TaperR))
				nCount++;
		}
		pBxTaper = Finder.GetBxNext();
	}

	return nCount;
}

//입력 : sttSta
//       endSta
//       D		 거더 중심에서의 이격거리
//기능 : 거더 하판의 실제 길이(거더중심, 하판 좌우, 복부판 좌우 등)
double CPlateGirderApp::GetLengthFlangeLowerBySta(double staStt, double staEnd, double D) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pCG	= pDB->GetGirder(-1);
	long nKNoS = GetNumberJiganByStation(staStt);
	long nKNoE = GetNumberJiganByStation(staEnd);
	double staS = staStt;
	double lenSum = 0;
	double dLengthByunOnly = 0;
	double dHeightByunMax = 0;
	double x1 = 0;
	double x2 = 0;
	
	for(long nK = nKNoS; nK <= nKNoE; nK++)
	{
		double kSta = pCG->GetStationOnJijum(nK);
		double kSta2 = pCG->GetStationOnJijum(nK+1);	//지간은 지점보다 1개 작음.
		if(IsByunDanMyunInJigan(nK) == FALSE && staEnd <= kSta2)
			return lenSum + GetLengthDimType(staS, staEnd, D);
		else if(IsByunDanMyunInJigan(nK) == FALSE && staEnd > kSta2)
		{
			lenSum += GetLengthDimType(staS, kSta2, D);
			staS = kSta2;
			continue;
		}

		double lenL = pDB->m_dLenLeftByun[nK];
		double lenR = pDB->m_dLenRightByun[nK];
		double curL = pDB->m_dLenLeftCurve[nK];
		double curR = pDB->m_dLenRightCurve[nK];
		if(staS < kSta + lenL)
		{
			if(staEnd <= kSta + lenL)
				return GetLengthDimType(staS, staEnd, D);
			lenSum += GetLengthDimType(staS, kSta + lenL, D);
			staS = kSta + lenL;
		}
		if(staS < kSta + lenL + curL)//변단면구간
		{
			dLengthByunOnly = GetLengthDimType(kSta + lenL, kSta + lenL + curL, D);
			dHeightByunMax = GetHeightGirderByJijum(nK) - pDB->m_dHeightByun[nK];
			x2 = GetLengthDimType(staS, kSta + lenL + curL, D);
			if(staEnd <= kSta + lenL + curL)
			{
				x1 = GetLengthDimType(staEnd, kSta + lenL + curL, D);
				return lenSum + GetLengthFlangeLowerByByun(x1, x2, nK, dLengthByunOnly, dHeightByunMax);
			}
			x1 = 0;
			lenSum += GetLengthFlangeLowerByByun(x1, x2, nK, dLengthByunOnly, dHeightByunMax);
			staS = kSta + lenL + curL;
		}

		if(staS < kSta2 - lenR - curR)
		{ 
			if(staEnd <= kSta2 - lenR - curR)
				return lenSum + GetLengthDimType(staS, staEnd, D);
			lenSum += GetLengthDimType(staS, kSta2 - lenR - curR, D);
			staS =  kSta2 - lenR - curR;
		}

		if(staS < kSta2 - lenR)//변단면구간
		{
			dLengthByunOnly = GetLengthDimType(kSta2 - lenR - curR, kSta2 - lenR, D);
			dHeightByunMax = GetHeightGirderByJijum(nK+1) - pDB->m_dHeightByun[nK];
			x1 = GetLengthDimType(kSta2 - lenR - curR, staS, D);
			if(staEnd <= kSta2 - lenR)
			{
				x2 = GetLengthDimType(kSta2 - lenR - curR, staEnd, D);
				return lenSum + GetLengthFlangeLowerByByun(x1, x2, nK, dLengthByunOnly, dHeightByunMax);
			}
			x2 = GetLengthDimType(kSta2 - lenR - curR, kSta2 - lenR, D);
			lenSum += GetLengthFlangeLowerByByun(x1, x2, nK, dLengthByunOnly, dHeightByunMax);
			staS =  kSta2 - lenR;
		}
		if(staEnd <= kSta2)		return lenSum + GetLengthDimType(staS, staEnd, D);
	}

	return GetLengthByDisGir(staStt, staEnd, D);
}

//입력 : sta1, sta2 : 구하려고 하는 길이의 스테이션
//       dis    거더 중심에서의 이격 거리 (좌측 -, 우측 +)
//기능 : (m_dGirderCenter+dis)만큼 이격된 거리에서 sta1에서 sta2까지의 실제길이
double CPlateGirderApp::GetLengthByDisGir(double sta1, double sta2, double dis) const
{
//	double dStaStt = m_pLine->m_dStationBp;
	double dStaEnd = m_pLine->GetStationEpLine();
//	if(sta1 < dStaStt || sta1 > dStaEnd || sta2 < dStaStt || sta2 > dStaEnd)	return 0;
	if(sta1 > dStaEnd || sta2 > dStaEnd)		return 0;
	if(m_dGirderCenter+dis==0) return sta2 > sta1 ? sta2-sta1 : sta1-sta2;

	return m_pLine->GetLengthByDis(sta1, sta2, m_dGirderCenter+dis);
}

//<summary>
//(m_dGirderCenter+dis)만큼 이격된 거리에서 sta1에서 sta2까지의 실제길이
//</summary>
//<param name="sta1">구하려고 하는 길이의 시점 스테이션</param>
//<param name="sta2">구하려고 하는 길이의 종점 스테이션</param>
//<param name="dis">거더 중심에서의 이격 거리 (좌측 -, 우측 +)</param>
//<param name="bUpper">상판, 하판을 구분함</param>
//<param name="nDimType">0=평면치수, 1=캠버 치수, 2=사용하지 말것, 3=2차원 치수(높이차가 고려되지 않은 길이)</param>
//<return>(m_dGirderCenter+dis)만큼 이격된 거리에서 sta1에서 sta2까지의 실제길이</return>
double CPlateGirderApp::GetLengthDimType(double sta1, double sta2, double dis, BOOL bUpper, DIMTYPE nDimType) const
{
//	double dStaStt = GetStationStt();
//	double dStaEnd = GetStationEnd();
//	if(sta1 < dStaStt || sta1 > dStaEnd || sta2 < dStaStt || sta2 > dStaEnd)	return 0;
	if (sta1 > sta2)   
	{
		double temp = sta1;
		sta1 = sta2;
		sta2 = temp;
	}
	if(nDimType == DIM_BASIC && bUpper)			return GetLengthByDisGir(sta1, sta2, dis);
	else if(nDimType == DIM_BASIC && !bUpper)	return GetLengthFlangeLowerBySta(sta1, sta2, dis);
	else if(nDimType == DIM_CAMBER)				return GetLengthFlangeByStaJong(sta1, sta2, dis, bUpper, TRUE);
	else if(nDimType == 3)				return GetLengthFlangeByStaJong(sta1, sta2, dis, bUpper, FALSE);

	return 0;
}

//입력 : sttSta
//       endSta
//       D		 거더 중심에서의 이격거리
//       bUpper	 TRUE = 상판 기준, FALSE = 하판 기준
//       bJong	 TRUE = 캠버 길이 , FALSE = 2차원 길이(높이차가 고려되지 않은 길이)
//기능 : 상.하판 면에서 D 만큼 이격된 거리의 종단 캠버 고려한 실제 길이를 구하는 함수.
//		 staStt, staEnd [구간]을 자동으로 나누어서 그 합의 길이를 구한
double CPlateGirderApp::GetLengthFlangeByStaJong(double staStt, double staEnd, double D, BOOL bUpper, BOOL bJong) const
{
	CDPointArray StaArr;
	__int64 BxType = bUpper ? BX_UPPER : BX_LOWER;
	GetStationArrayByFlangeSection(StaArr, staStt, staEnd, D, BxType);

	long nCount = StaArr.GetSize();
	if (nCount == 0)	return 0;
	double len = 0;
	double sta, presta = StaArr.GetAt(0).x;
	for (long i = 1; i < nCount; i++)
	{
		sta = StaArr.GetAt(i).x;
		len += GetLengthFlangeByStaJongBase(presta, sta, D, bUpper, bJong);
		presta = sta;
	}
	return len;
}

//기능 : staStt와 staEnd의 두 점을 직선으로 이은 길이를 구함.
double CPlateGirderApp::GetLengthFlangeByStaJongBase(double staStt, double staEnd, double D, BOOL bUpper, BOOL bJong) const
{
	CVector xyzStt = GetXyGirderDis(staStt, D);
	CVector xyzEnd = GetXyGirderDis(staEnd, D);
	CDPoint vAng = CDPoint(0,1);
	xyzStt.z = bJong ? GetElevationFlangeCamber(staStt, D, vAng, bUpper) : 0;
	xyzEnd.z = bJong ? GetElevationFlangeCamber(staEnd, D, vAng, bUpper) : 0;

	return ~(xyzEnd - xyzStt);
}

//vArr   : 플랜지상의 xy좌표
//staStt : 시작스테이션
//staEnd : 종료스테이션
//bUpper : 상,하 플랜지
//nPos   : -1=좌측 0=중앙 1=우측 
void CPlateGirderApp::GetVertexFlangeByStation(CVectorArray &vArr, double staStt, double staEnd, BOOL bUpper, long nPos, __int64 BxType) const
{
	CDPointArray StaArr;

	if (BxType == 0)
		BxType = bUpper ? BX_UPPER : BX_LOWER;
	vArr.RemoveAll();
	GetStationArray(StaArr, staStt, staEnd, BxType);
	long nCount = StaArr.GetSize();
	double dDis = 0;

	for (long i = 0; i < nCount; i++)   {
		double sta = StaArr.GetAt(i).x;

		if(nPos==-1) dDis = -GetWidthOnStation(sta, bUpper)/2;
		if(nPos== 0) dDis = 0;
		if(nPos== 1) dDis =  GetWidthOnStation(sta, bUpper)/2;
		
		CVector xyz = GetXyGirderDis(sta, dDis);
		xyz.z = GetElevationFlangeCamber(sta, 0, CDPoint(0,1), bUpper);
		vArr.Add(xyz);
	}
}

// 상부 또는 하판의 좌우측 꼭지점들을 구함. (지그재그 형태...)
// (입력 Station은 거더중심기준. 거더 시종점부만 사각 가능. 일반부는 무조건 90도.)
void CPlateGirderApp::GetVertexFlangeByModel(CVectorArray &vFlange, double staStt, double staEnd, BOOL bUpper, __int64 BxType, BOOL b3DModel) const
{
	vFlange.RemoveAll();
	CVectorArray vLeft, vRight;
	GetVertexFlangeByStation(vLeft,  staStt, staEnd, bUpper, -1, BxType);
	GetVertexFlangeByStation(vRight, staStt, staEnd, bUpper,  1, BxType);
	long nCount = min(vLeft.GetSize(), vRight.GetSize());

	for (long i = 0; i < nCount; i++)   {
		vFlange.Add(vLeft.GetAt(i));
		vFlange.Add(vRight.GetAt(i));
	}
}


// 좌측 또는 우측 복부판 상하부 꼭지점들을 구함. (지그재그 형태...)
// (입력 Station은 거더중심기준. 거더 시종점부만 사각 가능. 일반부는 무조건 90도.)
void CPlateGirderApp::GetVertexWebByModel(CVectorArray &vWeb, double staStt, double staEnd, __int64 BxType, BOOL b3dModel) const
{
	double staSttUpper=0, staSttLower=0, staEndUpper=0, staEndLower=0;
	
	staSttUpper = staStt;
	staSttLower = staStt;
	staEndUpper = staEnd;
	staEndLower = staEnd;
		
	vWeb.RemoveAll();
	CVectorArray vUpper, vLower;
	__int64 BxFlag = BxType == 0 ? BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE |
		BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_BENDING | BX_BYUN : BxType;

	GetVertexFlangeByStation(vUpper, staSttUpper, staEndUpper, TRUE,  0, BxFlag);
	GetVertexFlangeByStation(vLower, staSttLower, staEndLower, FALSE, 0, BxFlag);
	long nCount = min(vUpper.GetSize(), vLower.GetSize());

	for (long i = 0; i < nCount; i++)   {
		vWeb.Add(vUpper.GetAt(i));
		vWeb.Add(vLower.GetAt(i));
	}
}

// Bx 버전.
void CPlateGirderApp::GetVertexFlange(CVectorArray &vFlange, CPlateBasicIndex *pBx, BOOL bUpper) const
{
	__int64 bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_SPLICE;
	bxFlag |= bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE;
	
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxStt = Finder.GetBxLast(bxFlag,pBx,NULL);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(bxFlag,pBx,NULL,FALSE);

	GetVertexFlangeByModel(vFlange, pBxStt->GetStation(), pBxEnd->GetStation(), bUpper, 0, FALSE);
}

// Bx 버전.
void CPlateGirderApp::GetVertexWeb(CVectorArray &vWeb, CPlateBasicIndex *pBx) const
{
	__int64 bxFlag = BX_SPLICE | BX_STT_GIRDER  | BX_END_GIRDER | BX_WEB;	
	
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxStt = Finder.GetBxLast(bxFlag,pBx,NULL);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(bxFlag,pBx,NULL,FALSE);

	GetVertexWebByModel(vWeb, pBxStt->GetStation(), pBxEnd->GetStation());
}


//기능 : 현재 거더 중앙기준 D 이격거리의 슬래브+포장 두께를 산출,
//		 상판 내에 있는것만 유효함-가로보, 세로보 위치등의 두께는 안됨.
double CPlateGirderApp::GetThickSlabPave(double sta, double D, CDPoint vAng)
{
	BOOL bLevel = m_bEleLevel;

	SetEleLevel(FALSE);

	double dElePave = GetElevationPaveAct(sta, D, vAng);
	double dEleFlangeUpper = GetElevationFlange(sta, D, vAng);//캠버 없는 엘리베이션

	SetEleLevel(bLevel);

	return dElePave - dEleFlangeUpper;
}
//<summary>
//기준선형 중심에서 포장 상부까지의 높이차(편경사에 의한)
//	 포장 상부가 도로중심보다 높으면[+]
//   확폭이고 램프선형일 경우에도 (기준선형 Ele - 현재 포장 Ele) 를 구함
//</summary>
//<param name="sta"></param>
//<param name="D"></param>
//<param name="vAng"></param>
//<return></return>
double CPlateGirderApp::GetThickPyungubae(double sta, double D, CDPoint vAng) const
{
	double dElePave = GetElevationPaveAct(sta, D, vAng);
	return dElePave;//-dElePLCenLine;
}

//<summary>
//현재 거더 중앙기준 D 이격거리의 포장 상면 실제 elevation 을 산출
//  확폭이고 램프선형일 경우 pDB->GetXyPaveAct() 와 다른 값을 구함-자체 계획고, 편경사로 elevation을 구함.
//</summary>
//<param name="sta">거더선형 스테이션</param>
//<param name="D">이격거리</param>
//<param name="vAng">거더선형과 이루는 각도</param>
//<return></return>
double	CPlateGirderApp::GetElevationPaveAct(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;

	double	dAng	= vAng.GetAngleDegree();
	CDPoint vAngGir = vAng;
	if(dAng > 180)	dAng -= 180;	//항상 0~180사이에 있도록 함.
	if(dAng > 88 && dAng < 92) //각도가 90도에 가까우면 항상 90도를 취함(엘리베이션에 한함. 평면 좌표 등은 항상 정확한 값)
	{
		vAngGir = CDPoint(0, 1);
	}

	return pDB->GetElevationPaveActByDis(GetStationBySc(sta, D, vAngGir), m_dGirderCenter+D); 
}

//<summary>
//현재 거더 스테이션의 거더 상판 하면 좌우측 중에서 낮은 포장 상면 Elevation
//확폭일 경우
//   && 좌우측 선형일 경우
//     0 = 본선 계획고, 본선 편경사. 램프 편경사 사용
//     1 = 램프 계획고, 램프 편경사 사용(좌우측 선형만)
//     2 = 본선 계획고, 본선 편경사만  사용(ARoad2000)  => 이 설정이 되어 있으면, 좌우측 선형, 중앙선형 구분이 무의미함.
//</summary>
//<param name="sta">현재 거더의 실제 스테이션</param>
//<param name="D">거더 중앙에서의 이격거리</param>
//<param name="vAng">거더 중앙의 각도</param>
//<return></return>
double CPlateGirderApp::GetElevationPaveFlangeLeftOrRight(double sta, double D, CDPoint vAng) const
{
	if(m_bEleLevel) return 0;
	CPlateBridgeApp	*pDB = (CPlateBridgeApp *)m_pBridgeApp;
	double dAng = vAng.GetAngleDegree(); //항상 0~360사이에 있음.
	CDPoint vAngGir = vAng;
	if(dAng > 180)	dAng -= 180;	//항상 0~180사이에 있도록 함.
	if(dAng > 88 && dAng < 92) //각도가 90도에 가까우면 항상 90도를 취함(엘리베이션에 한함. 평면 좌표 등은 항상 정확한 값)
	{
		vAngGir = CDPoint(0, 1);
	}

	if (pDB->GetGirder(-1) == this) //현재 거더가 기준거더이면 기준선형의 계획고를 리턴
	{
		return GetLine()->GetElevationPlanEl(GetStationBySc(sta, D, vAngGir)); 
	}

	//if(!IsExtLineGirder())
//	{
//		if(pDB->m_pBaseChief && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignBoxType()==2) //해당 좌표의 포장 상면 Elevation, 노면구배형일 경우
//			return pDB->GetElevationPaveActByXY(GetXyGirderDis(sta, D, vAngGir));
//	}
	//else
	//{
	//	if(pDB->m_pBaseChief && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignBoxType()==2 && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignExtLineElvationType()==2) //해당 좌표의 포장 상면 Elevation, 노면구배형일 경우
	//		return pDB->GetElevationPaveActByXY(GetXyGirderDis(sta, D, vAngGir));
	//}

	if(!IsExtLineGirder())
	{
		double	staSc		= GetStationBySc(sta, D, vAngGir);
		double	dMaxWidth	= GetMaxWidthFlange(TRUE);
		double	lengthLeft	= m_dGirderCenter -  (dMaxWidth/2 + pDB->m_dHunchDisForBaseThick); 
		double	lEle		= pDB->GetElevationPaveActByDis(staSc, lengthLeft);	
		double	lengthRight	= m_dGirderCenter + (dMaxWidth/2 + pDB->m_dHunchDisForBaseThick); 
		double	rEle		= pDB->GetElevationPaveActByDis(staSc, lengthRight);
		
		return min(lEle, rEle);
	}
	/*
	else if(IsLineSameLeftOrRight() && pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignExtLineElvationType()!=2)
		//확폭거더이고 램프선형 거더일 경우
	{
		double staSc = GetStationBySc(sta, D, vAngGir);
		double lengthLeft = m_dGirderCenter - (m_dWidth/2 + m_dULeftSharp + pDB->m_dHunchDisForBaseThick); 
		double dPyunLeft = m_pLine->GetPyungubae(staSc, lengthLeft);
		double lHeight = fabs(lengthLeft) * dPyunLeft;	
		double lengthRight = m_dGirderCenter +(m_dWidth/2 + m_dURightSharp + pDB->m_dHunchDisForBaseThick); 
		double dPyunRight = m_pLine->GetPyungubae(staSc, lengthRight);
		double rHeight = fabs(lengthRight) * dPyunRight;	
		double mHeight = min(lHeight, rHeight);

		double dEle = 0;
		if(pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignExtLineElvationType()==1) //램프 계획고, 램프 편경사 사용(좌우측 선형만)
		{
			dEle = GetLine()->GetElevationPlanEl(staSc);
		}
		else	//본선 계획고, 본선 편경사. 램프 편경사 사용
		{
			CDPoint xy = m_pLine->GetXyLine(staSc);	//거더선형 상의 좌표
			dEle = pDB->GetElevationPaveActByXY(xy);	
		}
		return dEle + mHeight;
	}
	else   //중앙선형 일 경우
	{
		double staCR = 0;
		CLineInfo *pLineCR = pDB->GetGirder(-1)->GetLine();
		if(vAngGir == CDPoint(0, 1))
			D = 0;		//중앙선형이고 그 선형에 직각일 경우 D를 0로 함.
	 	CDPoint xyGir = GetXyGirderDis(sta, D, vAngGir);

		pLineCR->GetStationMatchVerticalByXy(xyGir, staCR);
		CDPoint xyCen = pLineCR->GetXyLine(staCR);
		CDPoint uV = (xyGir-xyCen).Unit();	//전체 좌표계에서 X축에 대한 각도
		double staGirMatch = 0;
		if(!m_pLine->GetStationMatchByXyAndAngle(xyCen, uV, staGirMatch))	return 0;
		CDPoint vAngGirMatch = m_pLine->GetAngleAzimuth(staGirMatch).BetweenAngle(uV);//거더선형과 이루는 각도
		double sinVal = fabs(vAngGirMatch.y);

		double disGirderCenter = pLineCR->GetDisMatchVerticalByXy(m_pLine->GetXyLine(staGirMatch));
//		double lLength = disGirderCenter + (m_dGirderCenter - (m_dUWidth/2 + m_dULeftSharp + pDB->m_dHunchDisForBaseThick))/sinVal; 
		double lLength = disGirderCenter + (m_dGirderCenter/sinVal - (m_dUWidth/2 + m_dULeftSharp + pDB->m_dHunchDisForBaseThick)); 
		double lEle = pDB->GetElevationPaveActByDis(staCR, lLength);	
//		double rLength = disGirderCenter + (m_dGirderCenter +(m_dUWidth/2 + m_dURightSharp + pDB->m_dHunchDisForBaseThick))/sinVal; 
		double rLength = disGirderCenter + (m_dGirderCenter/sinVal +(m_dUWidth/2 + m_dURightSharp + pDB->m_dHunchDisForBaseThick)); 
		double rEle = pDB->GetElevationPaveActByDis(staCR, rLength);
		return min(lEle, rEle);
	}
	*/


	return	0;
}

double CPlateGirderApp::GetElevationPaveWeb(double sta, double D, CDPoint vAng) const
{
	if(m_bEleLevel) return 0;
	CPlateBridgeApp	*pDB = (CPlateBridgeApp *)m_pBridgeApp;
	double dAng = vAng.GetAngleDegree(); //항상 0~360사이에 있음.
	CDPoint vAngGir = vAng;
	if(dAng > 180)	dAng -= 180;	//항상 0~180사이에 있도록 함.
	if(dAng > 88 && dAng < 92) //각도가 90도에 가까우면 항상 90도를 취함(엘리베이션에 한함. 평면 좌표 등은 항상 정확한 값)
	{
		vAngGir = CDPoint(0, 1);
	}

	if (pDB->GetGirder(-1) == this) //현재 거더가 기준거더이면 기준선형의 계획고를 리턴
	{
		return GetLine()->GetElevationPlanEl(GetStationBySc(sta, D, vAngGir)); 
	}

	if(!IsExtLineGirder())
	{
		double staSc = GetStationBySc(sta, D, vAngGir);
		double dEle = pDB->GetElevationPaveActByDis(staSc, m_dGirderCenter);
		
		return dEle;
	}

	return	0;
}

//<summary>
//현재 거더 스테이션의 거더 상판 하면 기준 실제 Elevation
//		 테이퍼부에는 각도가 90도이어도 D 값에 의하여 상판 elevation이 다를수 있음.
//</summary>
//<param name="sta">현재 거더의 실제 스테이션</param>
//<param name="D">거더 중앙에서의 이격거리</param>
//<param name="vAng">거더 중앙의 각도</param>
//<param name="bUpper/*=TRUE*/">TRUE = 상판 하면 엘리베이션 산출, FALSE = 하판 상면 엘리베이션 산출</param>
//<return></return>
double CPlateGirderApp::GetElevationFlange(double sta, double D, CDPoint vAng, BOOL bUpper/*=TRUE*/) const
{
	CPlateBridgeApp	*pDB = (CPlateBridgeApp *)m_pBridgeApp;
	double dEle = GetElevationPaveFlangeLeftOrRight(sta, D, vAng);
	
	if(bUpper)
	{
		if(pDB->IsTUGir())
			return dEle - (pDB->m_dThickSlabBase + pDB->m_dThickPave);
		return dEle - GetHeightFlangeUpperByTaper(sta, D, vAng) - (pDB->m_dThickSlabBase + pDB->m_dThickPave);
	}
	
	return	dEle - GetHeightFlangeLower(GetStationBySc(sta, D, vAng),D) - (pDB->m_dThickSlabBase + pDB->m_dThickPave);
}

double CPlateGirderApp::GetElevationWeb(double sta, double D, CDPoint vAng, BOOL bUpper/*=TRUE*/) const
{
	CPlateBridgeApp	*pDB = (CPlateBridgeApp *)m_pBridgeApp;
	double dEle = GetElevationPaveWeb(sta, D, vAng);
	
	if(bUpper)
	{
		if(pDB->IsTUGir())
			return dEle - (pDB->m_dThickSlabBase + pDB->m_dThickPave);
		return dEle - GetHeightFlangeUpperByTaper(sta, D, vAng) - (pDB->m_dThickSlabBase + pDB->m_dThickPave);
	}
	
	return	dEle - GetHeightFlangeLower(GetStationBySc(sta, D, vAng),D) - (pDB->m_dThickSlabBase + pDB->m_dThickPave);
}

//기능 : 공장 제작 캠버포함한 상.하판의 elevation을 리턴
//		 캠버는 같은 station에서는 같다고 결정함.
double CPlateGirderApp::GetElevationFlangeCamber(double sta, double D, CDPoint vAng, BOOL bUpper/*=TRUE*/) const
{
//	if(/*!IsSectionTapered(pBx) && */IsExtLineGirder()) D=0;
	double staSc = GetStationBySc(sta, D, vAng);
	double camber = GetCamberByStation(staSc, 7);	//②강재 자중에 의한 솟음량
											//+③합성전 1차사하중에 의한 솟음량
											//+④합성후 2차사하중에 의한 솟음량
											//+5+6 : User입력값 (10개까지 가능)
	if(m_bEleLevel) camber = 0;

	return	GetElevationFlange(sta, D, vAng, bUpper)+camber;
}

double CPlateGirderApp::GetGiulCamber(double sta, double D, CDPoint vAng) const
{
//	if(/*!IsSectionTapered(pBx) && */IsExtLineGirder()) D=0;
	if(m_bEleLevel) return 0;

	double staSc = GetStationBySc(sta, D, vAng);
	double Gap = 0.1;
	double camber1 = GetCamberByStation(staSc - Gap, 7);
	double camber2 = GetCamberByStation(staSc + Gap, 7);	//②강재 자중에 의한 솟음량
											//+③합성전 1차사하중에 의한 솟음량
											//+④합성후 2차사하중에 의한 솟음량
											//+5+6 : User입력값 (10개까지 가능)
	double len  = GetLengthDimType(staSc - Gap, staSc + Gap, D);
	return (camber2 - camber1) / len;
}

/*
//입력 : nIdx	베이직 인덱스
//기능 : 가로보(다이아프램) 번호를 리턴(0, 1, 2...)
//		 nIdx에 가로보가 없어도 유효한 인덱스이면 이전 가로보 번호 리턴
//		 "0"번 인덱스-> "0", 마지막 인덱스->마지막 가로보 번호
long CPlateGirderApp::GetNumberCrossBeamByBxBefore(CPlateBasicIndex *pBx) const
{
	double  staCen	     = pBx->GetBxCenter()->GetStation();
	double  staDiapRight = GetStationBySc(staCen,m_dWidth/2,pBx->GetAngle());
	double  staCross     = 0;
	CDPoint vAngCross    = CDPoint(0,1);
	for(long nCross = 0;nCross < GetQtyCrossBeamVBracing();nCross++)
	{
		vAngCross = m_pCrossBeam[nCross]->m_vAngLeft;
		staCross  = GetStationBySc(m_pCrossBeam[nCross]->m_dStationLeft, m_dUWidth/2, vAngCross);
		if( fabs(staCross - staDiapRight) < 10 )	return nCross;
		if( staCross > staDiapRight )	return nCross-1;
	}
	return GetQtyCrossBeamVBracing()-1;	//마지막 가로보일 경우임.
}
*/
POS CPlateGirderApp::GetPOSNext(POS pos,long nCount/*=1*/) const
{
	long n = 0;
	for(n = 0; n < nCount && pos; n++)	m_BxList.GetNext(pos);
	for(n = 0; n > nCount && pos; n--)		m_BxList.GetPrev(pos);
	return pos;
}


POS CPlateGirderApp::GetPOSHead()
{	
	return m_BxList.GetHeadPosition();
}
POS CPlateGirderApp::GetPOSTail()
{
	return m_BxList.GetTailPosition();
}

/*
void CPlateGirderApp::ModifyMapList(__int64 BxFlag)
{	
	CPtrList *pObjList = NULL;	
	pObjList = NULL;
	if(!m_BxMap.Lookup(BxFlag,pObjList))		
	{
		pObjList = new CPtrList;
		m_BxMap.SetAt(BxFlag, pObjList);
		CPlateBxFinder Finder(this);
		CPlateBasicIndex *pBx = Finder.GetBxFirstOrg(BxFlag);
		while(pBx)
		{			
			pObjList->AddTail(pBx);
			pBx = Finder.GetBxNextOrg();
		}		
	}			
	m_pCurBxList = pObjList;
}
*/
void CPlateGirderApp::InsertBx(const CPtrList& RList)
{
	POS pos = RList.GetHeadPosition();
	while(pos)
	{
		CPlateBasicIndex* p = (CPlateBasicIndex*)RList.GetNext(pos);
		InsertBx(p);
	}
}

void CPlateGirderApp::GetUsedFlagArrInPressBxList(CPtrArray& UsedFlagArr,__int64 bxFlag) const
{
	for(long n = 0; n < 64; n++)
	{
		__int64 fg = 1;
		fg <<= n;
		if(bxFlag & fg)
		{
			long i = 0;
			for(i = 0; i < UsedFlagArr.GetSize(); i++)
				if( fg == *(__int64*)UsedFlagArr.GetAt(i) )
					break;

			if(i == UsedFlagArr.GetSize())	// 없는 경우 추가
			{
				__int64* p = new __int64(fg);
				long j = 0;
				for(j = 0; j < UsedFlagArr.GetSize(); j++)
					if(fg <	*(__int64*)UsedFlagArr.GetAt(j))
					{
						UsedFlagArr.InsertAt(j,p);
						break;
					}
				if(j == UsedFlagArr.GetSize())
					UsedFlagArr.Add( p );
			}
		}
	}

}

void CPlateGirderApp::PressBxList(const CPtrList& InputList,__int64 bxFlag /*= 0*/,CDWordArray* pLKException/*NULL*/)
{
	ASSERT(InputList.GetCount() > 0 || bxFlag);

	// 사용된 bxFlag 찾기
	CPtrArray  UsedFlagArr;
	CPtrList NList;
	AhPtrListCopy(NList,InputList,(CPlateBasicIndex*)0);

	// set UsedFlagArr
	if(bxFlag)
	{
		GetUsedFlagArrInPressBxList(UsedFlagArr,bxFlag);
	}
	else
	{
		POS ps = NList.GetHeadPosition();
		while( ps )
		{
			CPlateBasicIndex* p = (CPlateBasicIndex*)NList.GetNext(ps);
			GetUsedFlagArrInPressBxList(UsedFlagArr,p->GetBxFlagState());
		}
	}


	// call PressBxListForOneFlag() sequent 
	for(long n = 0; n < UsedFlagArr.GetSize(); n++)
	{
		CPtrList JList;
		__int64 fg = *(__int64*)UsedFlagArr.GetAt(n);
		POS psPrev, ps = NList.GetHeadPosition();
		while( ps )
		{
			psPrev = ps;
			CPlateBasicIndex* p = (CPlateBasicIndex*)NList.GetNext(ps);
			if(p->IsState(fg))
			{
				JList.AddTail( p );
				NList.RemoveAt(psPrev);
			}

		}

		PressBxListForOneFlag(JList, fg, pLKException);
		AhPtrListDelete(JList,(CPlateBasicIndex*)0);
	}

	AhPtrArrayDelete(UsedFlagArr,(__int64*)0);	
}



void CPlateGirderApp::PressBxListForOneFlag(const CPtrList& InputList,__int64 bxFlagOnlyOne,CDWordArray* pLKException)
{
	POS pos,pos2,psPrev,psPrev2;
	CPtrList NList,LKList;
	AhPtrListCopy(NList,InputList,(CPlateBasicIndex*)0);

	BOOL bResettingLKArr[LINKOBJECTSU];
	memset(bResettingLKArr,0,4*LINKOBJECTSU);

	// ResettingLK LKList
	for(pos = GetPOSHead(); pos; )
	{
		CPlateBasicIndex* p = (CPlateBasicIndex*)m_BxList.GetNext(pos);
		if(p->IsState(bxFlagOnlyOne))
			LKList.AddTail(p);
	}
	pos = LKList.GetHeadPosition();
	while(pos)
	{
		CPlateBasicIndex* p = (CPlateBasicIndex*)LKList.GetNext(pos);
		for(long n = 0; n < LINKOBJECTSU; n++)
			if(p->GetObjLinkSeq(n) >= 0)
				bResettingLKArr[n] = TRUE;
	}
	if(pLKException)
		for(long n = 0; n < pLKException->GetSize(); n++)
			bResettingLKArr[pLKException->GetAt(n)] = FALSE;


	CPtrList AList;	// 다른 flag와 or 연산이 없는 경우에만 기존의 리스트의 내용을 삭제함(Move개념임)
	for(pos = GetPOSHead(); pos; )
	{
		psPrev = pos;
		CPlateBasicIndex* p = (CPlateBasicIndex*)m_BxList.GetNext(pos);
		if(p->IsState(bxFlagOnlyOne))
		{
			if(bxFlagOnlyOne == p->GetBxFlagState())
			{
				AList.AddTail(p);
				m_BxList.RemoveAt(psPrev);
			}
			else
			{
				CPlateBasicIndex *pBx = newBx(p);
				p->SetState(bxFlagOnlyOne,FALSE);
				pBx->SetState(pBx->GetBxFlagState(),FALSE);
				pBx->SetState(bxFlagOnlyOne,TRUE);
				AList.AddTail(pBx);
			}

		}
	}



	// AList, NList는 공통부분리스트의 나머지부분 남음
	CPtrList RList;// 공통부분리스트 구하기
	pos = AList.GetHeadPosition();
	pos2 = NList.GetHeadPosition();
	while(pos)
	{
		psPrev = pos;
		CPlateBasicIndex* p = (CPlateBasicIndex*)AList.GetNext(pos);
		while(pos2)
		{
			psPrev2 = pos2;
			CPlateBasicIndex* p2 = (CPlateBasicIndex*)NList.GetNext(pos2);
			if(p->GetStation() == p2->GetStation())
			{
				RList.AddTail(p2);

				AList.RemoveAt(psPrev);
				NList.RemoveAt(psPrev2);
				delete p;
				//delete p2;
				break;
			}

		}
	}

	// station을 제외한 모든 내용 m_nObjLinkSeq[LINKOBJECTSU] copy  (AList => NList)
	pos = AList.GetHeadPosition();
	pos2 = NList.GetHeadPosition();
	while(pos && pos2)
	{
		CPlateBasicIndex* pA = (CPlateBasicIndex*)AList.GetNext(pos);
		CPlateBasicIndex* pN = (CPlateBasicIndex*)NList.GetNext(pos2);

		pN->CopyLinkVariable(pA);
	}


	InsertBx(RList);
	InsertBx(NList);


	AhPtrListDelete(AList,(CPlateBasicIndex*)0);
	AhPtrListDelete(RList,(CPlateBasicIndex*)0);
	AhPtrListDelete(NList,(CPlateBasicIndex*)0);

	// ResettingLK 
	for(long nLK = 0; nLK < LINKOBJECTSU; nLK++)
		if(bResettingLKArr[nLK]) 
			SetLKPOS( nLK );	
}
/*
void CPlateGirderApp::InsertTableByBxFlag(CPlateBasicIndex *pBx)
{
	__int64 BxFlag = pBx->GetBxFlagState(), nFlag;
	CPtrList *pObjList = NULL;	
	// 각각을 해쉬 테이블에 등록
	for(long i=0;i<64;i++)
	{
		nFlag = 1 << i;
		if(nFlag & BxFlag)
		{
			if(!m_BxMap.Lookup(nFlag,pObjList))		
			{
				pObjList = new CPtrList;
				m_BxMap.SetAt(nFlag, pObjList);
			}		
			pObjList->AddTail(pBx);
		}
	}	
	// 조합된 플래그를 해쉬 테이블에 등록
	pObjList = NULL;
	if(!m_BxMap.Lookup(BxFlag,pObjList))		
	{
		pObjList = new CPtrList;
		m_BxMap.SetAt(BxFlag, pObjList);
	}		
	pObjList->AddTail(pBx);
}
*/
/*
//입력 : sta  찾으려고 하는 인덱스의 스테이션(선형 중심 스테이션)
//기능 : 현재 스테이션에 가장 가까운 스테이션의 POSITION을 리턴함.
POS CPlateGirderApp::GetPOSByStationDir(double sta,long nDir,__int64 bxFindFlag) const
{
	double PreSta,PostSta;
	double dDisPre,dDisPost;

	BOOL bApplyFindFlag = bxFindFlag ? TRUE : FALSE;
	POS psPre = GetPOSHead();
	for(POS ps = GetPOSHead(); ps ; ps = GetPOSNext(ps))
	{
		CPlateBasicIndex *pBx = GetBx(ps);

		if(bApplyFindFlag && !pBx->IsState(bxFindFlag)) continue;

		if(sta < pBx->GetStation() )
		{
			PostSta = pBx->GetStation();
			PreSta = GetStationByPOS(psPre);
			break;
		}
		psPre = ps;
	}
	if ( !ps )
		return GetPOSTail();

	if (nDir < 0)
		return psPre;
	if (nDir > 0)
		return ps;
	dDisPre = fabs(sta - PreSta);
	dDisPost = fabs(PostSta - sta);

	return (dDisPre < dDisPost) ? psPre : ps;
}
*/

//기능 : 시.종점부 상판 하단에서 출입구까지의 높이
double CPlateGirderApp::GetHeightInOutUpper(BOOL bStt, BOOL bReal) const
{
	CPlateBridgeApp *pDB    = (CPlateBridgeApp*)m_pBridgeApp;
	CGlobarOption   *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	if(bStt)
		return GetHeightGirderByJijum(0, bReal) - (pDB->m_dHeightInOutLowerStt + pDB->m_dHeightInOutStt + pDB->m_dHeightTaperStt);
	else
		return GetHeightGirderByJijum(pDB->m_nQtyJigan, bReal) - (pDB->m_dHeightInOutLowerEnd + pDB->m_dHeightInOutEnd + pDB->m_dHeightTaperEnd);
}

//기능 : 다시 제작할 것.
double CPlateGirderApp::GetStationOffset(double dSta, CDPoint vAng, double dLengthLine) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp* pGirCR	= pDB->GetGirder(-1);
	CLineInfo*	pLineCR = pGirCR->GetLine();
	double staReturn = 0;
	double staCR = 0;
	double staCRMove = 0;

	if(dLengthLine == 0) return dSta;

	staCR = GetStationBySc(dSta, -m_dGirderCenter, vAng);
	staCRMove = staCR + dLengthLine;
	CDPoint vStdAng = m_pLine->GetAngleByOffset(dSta, staCRMove, vAng);

	if(m_pLine == pLineCR)	//확폭이 아니면
	{
		staReturn = m_pLine->GetStationByScInLine(staCRMove, m_dGirderCenter, vStdAng);
	}
	else		//확폭이면
	{
/*		CDPoint vAngBang = pLineCR->GetAngleAzimuth(staCRMove);//방위각
		CDPoint xyCRMove  = pLineCR->GetXyLine(staCRMove);
		staBridge = m_pLine->GetStationMatchByXyAndAngle(xyCRMove, vAngBang.Rotate(vAng));
		CDPoint xyTar = m_pLine->GetXyLine(staBridge);
		CDPoint vAngBangTar = m_pLine->GetAngleAzimuth(staBridge);
		CDPoint vAngTar = vAngBangTar.BetweenAngle((xyTar - xyCRMove).Unit());

		staReturn = m_pLine->GetStationByScInLine(staBridge+dis, m_dGirderCenter, vAngBri);
*/
		staReturn = dSta + dLengthLine;  // 임시
	}

	return staReturn;

}

//기능 : 다시 제작할 것.
CDPoint CPlateGirderApp::GetAngleOffset(double dSta, CDPoint vAng, double dLengthLine) const
{
	double staCRMove = GetStationOffset(dSta, vAng, dLengthLine);
	CDPoint vStdAng = m_pLine->GetAngleByOffset(dSta, staCRMove, vAng);
	return vStdAng;
}
//입력 : nIdx	현재 거더의 인덱스(다이아프램 인덱스만 해당됨) 
//		 pGir	매치 인덱스를 찾으려고 하는 거더 포인터
//기능 : 찾으려고 하는 거더의 인덱스
//		 없으면 -1;
POS CPlateGirderApp::GetPOSMatchByDiap(POS psBx, CPlateGirderApp *pGir) const
{
	ASSERT( pGir && psBx );

	CPlateBasicIndex *pBx = GetBx(psBx);
	CPlateBxFinder Finder((CPlateGirderApp *)pGir);
	CPlateBasicIndex *pBxN = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while( pBxN )
	{
		if( pBxN->GetMatchCol() == pBx->GetMatchCol() )
			return Finder.GetPOS();

		pBxN = Finder.GetBxNext();
	}

	return NULL;
}


POS CPlateGirderApp::GetPOSMatchByVert(POS psBx, CPlateGirderApp *pGir, __int64 bxFlag/*=NULL*/) const
{
	ASSERT( pGir && psBx );

	CLineInfo *pLineSec = pGir->GetLine();
	CPlateBasicIndex *pBx = GetBx(psBx);

	double sta  = pBx->GetStation();

	if( m_pLine != pLineSec )
	{
		CDPoint xy = GetXyGirderDis(sta);
		pLineSec->GetStationMatchVerticalByXy(xy, sta);
	}

	return pGir->GetPOSByStation(sta,bxFlag);
}

//PlateDeck 수량땜시 추가...070305....KB.....
CPlateBasicIndex* CPlateGirderApp::GetBxMatchByDiap(CPlateBasicIndex *pBx, CPlateGirderApp *pGir) const
{
	return pGir->GetBx( GetPOSMatchByDiap( pBx->GetPOS(),pGir));
}

CPlateBasicIndex* CPlateGirderApp::GetBxMatchByVert(CPlateBasicIndex *pBx, CPlateGirderApp *pGir, __int64 bxFlag/*=NULL*/) const
{
	return pGir->GetBx( GetPOSMatchByVert( pBx->GetPOS(),pGir,bxFlag) );
}

//<summary>
//현재 스테이션에서 가장 가까운 Bx 를 리턴
//</summary>
//<param name="sta"></param>
//<param name="bxFindFlag"></param>
//<return></return>
CPlateBasicIndex* CPlateGirderApp::GetBxByStation(double sta, __int64 bxFindFlag /*= 0*/) const
{
	return GetBx( GetPOSByStation( sta,bxFindFlag) );
}



//입력 : sta  찾으려고 하는 인덱스의 스테이션(선형 중심 스테이션)
//기능 : 현재 스테이션에 가장 가까운 스테이션의 POSITION을 리턴함.
POS CPlateGirderApp::GetPOSByStation(double sta, __int64 bxFindFlag /*= 0*/) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);

	POS ps = Finder.GetPOSFirst(bxFindFlag);
	while(ps)
	{
		CPlateBasicIndex *pBx = GetBx(ps);
		if(sta < pBx->GetStation() )
		{
			double dStaPrev = Finder.GetBxFixPrev()->GetStation();
			return (fabs(sta - dStaPrev) < fabs(pBx->GetStation() - sta)) ? Finder.GetPOSFixPrev() : ps;
		}

		ps = Finder.GetPOSNext();
	}


	return Finder.GetPOSLast(bxFindFlag);

}

//<summary>
//임의의 스테이션에서 nDir 방향으로 가장 가까운 스테이션, 단면사이의 중간위치 스테이션 구함.
//</summary>
//<param name="dSta"></param>
//<param name="nDir">0=가장 가까운 Bx 스테이션, -1=바로 이전의 Bx, 1 = 바로 이후의 Bx</param>
//<return></return>
double CPlateGirderApp::GetStationByOneStepMid(double dSta, long nDir, __int64 bxFindFlag) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	__int64 BxFlag = bxFindFlag;
	CPlateBasicIndex *pBx;
	if (nDir > 0)		pBx = Finder.GetBxFirst(BxFlag, GetBxByStationDir(dSta, -1, BxFlag));
	else				pBx = Finder.GetBxLast(BxFlag, GetBxByStationDir(dSta, 1, BxFlag));

	if(nDir != 0)	pBx = Finder.GetBxNext(abs(nDir));

	if (!pBx || !Finder.GetBxFixNext())
		return dSta;
	return (pBx->GetStation() + Finder.GetBxFixNext()->GetStation()) / 2;
}
//<summary>
//임의의 스테이션에서 nDir 방향으로 가장 가까운 Bx
//</summary>
//<param name="sta">임의의 스테이션</param>
//<param name="nDir">0=가장 가까운 Bx, -1=바로 이전의 Bx, 1 = 바로 이후의 Bx</param>
//<param name="bxFindFlag*/"></param>
//<return></return>
CPlateBasicIndex* CPlateGirderApp::GetBxByStationDir(double sta, long nDir, __int64 bxFindFlag/* = 0*/) const
{
	if(nDir == 0)	return GetBxByStation(sta, bxFindFlag);

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFindFlag), *pBxPre = NULL;
	while (pBx)   {
		if (pBx->GetStation() >= sta)
			break;
		pBxPre = pBx;
		pBx = Finder.GetBxNext();
	}
	if (nDir < 0 || !pBx)			return pBxPre;
	return pBx;
}

//<summary>
//임의의 스테이션에서 nDir 방향으로 Dist 이격된 거리, vAngle 적용된 스테이션에서 가장 가까운 Bx
//</summary>
//<param name="sta">임의의 스테이션</param>
//<param name="nDir">0=가장 가까운 Bx, -1=바로 이전의 Bx, 1 = 바로 이후의 Bx</param>
//<param name="bxFindFlag"></param>
//<param name="dist"></param>
//<return></return>
CPlateBasicIndex* CPlateGirderApp::GetBxByStationDistDir(double sta,long nDir,__int64 bxFindFlag/* = 0*/, double dist) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFindFlag), *pBxPre = NULL;
	double bxSta = 0;
	while (pBx)   {
		bxSta = GetStationBySc(pBx->GetStation(), dist, pBx->GetAngle());
		if (bxSta >= sta)
			break;
		pBxPre = pBx;
		pBx = Finder.GetBxNext();
	}
	if (nDir < 0 || nDir == 0 && !pBx)
		return pBxPre;
	if (nDir > 0 || nDir == 0 && !pBxPre)
		return pBx;

	double dDisPre  = fabs(sta - GetStationBySc(pBxPre->GetStation(), dist, pBxPre->GetAngle()));
	double dDisPost = fabs(bxSta - sta);

	return (dDisPre < dDisPost) ? pBxPre : pBx;
}

//<summary>
//단면 사이 구간에 존재하는 종류(현장이음, 공장이음, 맨홀, 등등...)가 있는지 조사.
//</summary>
//<param name="sta">단면 사이 구간 지정 (정확히 가운데가 아니어도 됨.)</param>
//<param name="BxFlag">찾고자 하는 Bx종류 지정</param>
//<return></return>
//Xx IsObjectInMidByStation()
BOOL CPlateGirderApp::IsBxObjectInMidByStation(double sta, __int64 BxFlag)
{
	double staMid = 0;
	double dSta = GetStationByOneStepMid(sta, 0);  // 스테이션 보정

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
	while (pBx)
	{
		staMid = GetStationByOneStepMid(pBx->GetStation(), 0);
		if (staMid >= dSta)
			break;
		pBx = Finder.GetBxNext();
	}
	if (staMid == dSta)
		return TRUE;
	return FALSE;
}

//입력 :
//		
//기능 :
BOOL CPlateGirderApp::IsSameTypeBySplice(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec, BOOL bSimple) const
{
	if(pBx->GetSplice() == NULL || pBxSec->GetSplice() == NULL)	return FALSE;

	double dWFlUp    = GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWFlUpSec = GetWidthOnStation(pBxSec->GetStation(), TRUE);
	double dWFlLo    = GetWidthOnStation(pBx->GetStation(), FALSE);
	double dWFlLoSec = GetWidthOnStation(pBxSec->GetStation(), FALSE);
	double dHeightBoxByBx    = pBx->GetHeightGirderByBx();
	double dHeightBoxByBxSec = pBxSec->GetHeightGirderByBx();

	if(dWFlUp != dWFlUpSec || dWFlLo != dWFlLoSec || dHeightBoxByBx != dHeightBoxByBxSec)
		return FALSE;

	if(bSimple == FALSE)
	{
		if(pBx->GetFactChain(G_F_U)->m_dFactChainThick != pBxSec->GetFactChain(G_F_U)->m_dFactChainThick)	return FALSE;
		if(pBx->GetFactChain(G_F_L)->m_dFactChainThick != pBxSec->GetFactChain(G_F_L)->m_dFactChainThick)	return FALSE;
		if(pBx->GetFactChain(G_W)->m_dFactChainThick   != pBxSec->GetFactChain(G_W)->m_dFactChainThick)		return FALSE;		
	}

	return TRUE;
}
//입력 : nMarkBuje	부재 넘버 (전체 부재중에서)
//		 nRow		종리브, 수평보강재, 다이아프램 보강재 등 해당 번호 순서에 의한 것
//		 nCol		다이아프램 보강재, 가로보 수직보강재, 수평보강재(복부판, 가로보 등)
//					1번 인덱스, 2번째 줄-상부, 2번째 잭업의 수평보강재(nIdx = 0, nRow = 1, nCol= 2)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//		 bFront		TRUE - 시점쪽, FALSE - 종점쪽 (station 기준)
//기능 : vJewon.x = 너비, vJewon.y = 두께, vJewon.z = 길이
CVector CPlateGirderApp::GetJewonTotal(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam)
{
//	nMarkBuje = m_pBridgeApp->GetMarkRevision(nMarkBuje,pBx);

	CVector vJewon(0,0,0);
	try
	{	
		vJewon.x = GetWidthJewon(nMarkBuje, pBx, nRow, nCol, nDimType, bFront,bLeftBeam);
		vJewon.y = GetThickJewon(nMarkBuje, pBx, nRow, nCol, nDimType, bFront,bLeftBeam);
		vJewon.z = GetLengthJewon(nMarkBuje, pBx, nRow, nCol, nDimType, bFront,bLeftBeam);
	}
	catch(CException *e)
	{		
		REPORT_MFC_ERROR(e);
	}
#ifndef _DEBUG
	catch(...)
	{
		CString szData;
		szData.Format("에러 부재 코드 : %d\n제원 추출에서 비정상적인 값이 확인됩니다 !\n한길로 문의하시기 바랍니다. !",nMarkBuje); 
		AfxMessageBox(szData);
	}
#endif
	return vJewon;
}

//거더형강의 제원 및 성질
double CPlateGirderApp::GetJewonAndProperty(long nMarkBuje, long nID)
{
/*	CPlateBridgeApp*	pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CSteelMaterial* pSteelMat = pDB->m_pDataManage->GetSteelMaterial();

	long n=0;
	switch(nMarkBuje)
	{
	case G_H:	{n=0; break;}
	case C_J_H: {n=1; break;}
	case C_M_H: {n=2; break;}
	case C_D_H: {n=3; break;}
	}
	if(nID==CSteelMaterial::MAT_PERIMETER)
	{
		double H  = pSteelMat->GetJewon(n, CSteelMaterial::MAT_H);
		double B  = pSteelMat->GetJewon(n, CSteelMaterial::MAT_B);
		double T1 = pSteelMat->GetJewon(n, CSteelMaterial::MAT_T1);
		double T2 = pSteelMat->GetJewon(n, CSteelMaterial::MAT_T2);
		double R  = pSteelMat->GetJewon(n, CSteelMaterial::MAT_R);
		double dPerimeter = B*2+(B-T1)*2+(H*2);
		return dPerimeter; 
	}
	else
		return pSteelMat->GetJewon(n, nID);
		*/
	return 1.111;//수정
}

//입력 : nDimType  0= 평면치수, 1=캠버치수, 2=제원치수, 3=XY 치수
//기능 : 상판 치수
double CPlateGirderApp::GetLengthFlangeUpperJewon(CPlateBasicIndex *pBx, BOOL bUpper, DIMTYPE nDimType) const
{
	if(nDimType == 2)	return GetRectangleFlangeByBx(pBx, bUpper).x;
	
	__int64 bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_SPLICE;
	bxFlag |= bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE;
	
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxStt = Finder.GetBxLast(bxFlag,pBx,NULL);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(bxFlag,pBx,NULL,FALSE);

	double dStaStt  = pBxStt->GetStation();
	double dStaEnd  = pBxEnd->GetStation();

	double dLen = GetLengthDimType(dStaStt,  dStaEnd,  0,  bUpper, nDimType);
 	return dLen;
}

double CPlateGirderApp::GetLengthFlangeWebJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	if(nDimType == 2)	return GetRectangleFlangeWebByBx(pBx).x;
	
	__int64 bxFlag = BX_SPLICE;
	bxFlag |= BX_STT_GIRDER | BX_END_GIRDER | BX_WEB;					  
	
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxStt = Finder.GetBxLast(bxFlag,pBx,NULL);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(bxFlag,pBx,NULL,FALSE);

	if(!pBxStt || !pBxEnd)
		return 0;
	
	double dStaStt = pBxStt->GetStation();
	double dStaEnd = pBxEnd->GetStation();
	return GetLengthDimType(dStaStt, dStaEnd, 0, TRUE, nDimType);	//항상 상판기준 치수
}

double CPlateGirderApp::GetWidthFlangeWebJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{	
	double dResult  = GetHeightGirderByStaAng(pBx->GetStation());
	double dHeight  = 0;

	if(nDimType == DIM_BASIC || nDimType == DIM_CAMBER)
	{
		CPlateBxFinder Finder((CPlateGirderApp*)this);
		long nSp = GetSpliceNoByBx(pBx);
		if(pBx->IsState(BX_STT_GIRDER))		nSp = 0;
		if(pBx->IsState(BX_SPLICE))	nSp++;
		__int64 BxFlag = BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER|BX_WEB;
//		CPlateBasicIndex *pBxStt = pBx->GetFactChain(nMarkBuje)->GetBx();
//		CPlateBasicIndex *pBxEnd = pBx->GetFactChain(nMarkBuje)->GetBxEnd();
		CPlateBasicIndex *pBxStt = Finder.GetBxLast(BxFlag,pBx,NULL);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BxFlag,pBx,NULL,FALSE);
		CPlateBasicIndex *p = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
		while(p)
		{
			dHeight = GetHeightGirderByStaAng(p->GetStation());
			dResult = max(dResult, dHeight);
			p = Finder.GetBxNext();
		}
	}
	else
		dResult = GetRectangleFlangeWebByBx(pBx).y;

	return dResult;
}
//입력 : pBx  현재 pBx 포함한 부재, 겹쳤을 경우 다음 부재
//		 bUpper TRUE = 상판, FALSE = 하판
//기능 : 상부 또는 하판의 제원 치수 => 전개된 직사각형 Size 구함.
CDPoint CPlateGirderApp::GetRectangleFlangeByBx(CPlateBasicIndex *pBx, BOOL bUpper) const
{
	double staStt, staEnd;
	__int64 bxFlag = BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER;
	bxFlag |= bUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE;

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex* pBxEnd;
	CPlateBasicIndex* pBxStt = Finder.GetBxLast(bxFlag,pBx);
	if(!pBxStt)// 거더시점의 경우
	{
		pBxStt = Finder.GetBxFirst(bxFlag);
		pBxEnd = Finder.GetBxNext();
	}
	else
	{
		pBxEnd = Finder.GetBxFirst(bxFlag,pBx,NULL,FALSE);
		if(!pBxEnd) // 거더종점의 경우
		{
			pBxEnd = Finder.GetBxLast(bxFlag,pBx);
			pBxStt = Finder.GetBxNext();
		}
	}

	staStt = pBxStt->GetStation();
	staEnd = pBxEnd->GetStation();
	
	CVectorArray vFlange;
	CDPointArray vPlate;

	GetVertexFlangeByModel(vFlange, staStt, staEnd, bUpper);  // 플랜지 꼭지점 좌표구함.
	GetFoldingVertex(vFlange, vPlate);  // 플랜지 전개.
	CDPoint size;
	size = GetRectangleFromVertex(vPlate, FALSE, 4);  // 직사각형 Size 구함.

	return size;
}


//기능 : 복부판의 제원치수 => 전개된 직사각형 Size 구함.
CDPoint CPlateGirderApp::GetRectangleFlangeWebByBx(CPlateBasicIndex *pBx) const
{
	double staStt, staEnd;
	__int64 bxFlag = BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER|BX_WEB;							

	CPlateBxFinder Finder((CPlateGirderApp*)this);	

	CPlateBasicIndex* pBxEnd;
	CPlateBasicIndex* pBxStt = Finder.GetBxLast(bxFlag,pBx);
	if(!pBxStt)// 거더시점의 경우
	{
		pBxStt = Finder.GetBxFirst(bxFlag);
		pBxEnd = Finder.GetBxNext();
	}
	else
	{
		pBxEnd = Finder.GetBxFirst(bxFlag,pBx,NULL,FALSE);
		if(!pBxEnd) // 거더종점의 경우
		{
			pBxEnd = Finder.GetBxLast(bxFlag,pBx);
			pBxStt = Finder.GetBxNext();
		}
	}

	staStt = pBxStt->GetStation();
	staEnd = pBxEnd->GetStation();

	CVectorArray vWeb;
	CDPointArray vPlate;

	GetVertexWebByModel(vWeb, staStt, staEnd);  // 복부판 꼭지점 좌표구함.
	GetFoldingVertex(vWeb, vPlate);  // 복부판 전개.
	CDPoint size;
	size = GetRectangleFromVertex(vPlate, FALSE, 4, TRUE, CDPoint(1,0));  // 직사각형 Size 구함.

	return size;
}

//기능 : 상부 또는 하부 종리브의 제원 치수 => 전개된 직사각형 Size 구함.
// bLiner : 종리브 편차값 적용 여부 (Default : FALSE)
CDPoint CPlateGirderApp::GetRectangleJRib(CPlateBasicIndex *pBx, long nRow, BOOL bUpper, BOOL bLiner) const
{
	/*
	CJRib* pRib = pBx->GetJRib(bUpper);

	CDPointArray vRib;
	GetVertexJRib(vRib, pBx, nRow, bUpper);  // 전개된 종리브 꼭지점 좌표구함.
	CDPoint rect = GetRectangleFromVertex(vRib, TRUE, 4, TRUE);  // 직사각형 Size 구함.
	if (!bLiner)
		return rect;

	// 종리브 편차값이 허용치보다 작으면 그냥 직선으로 간주.
	// 상부 테이퍼 구간, 하부 변단면 꺾임 구간은 제외.
	CPlateBasicIndex *pBxStt = pRib->GetBxJRibStt(pBx, nRow);
	CPlateBasicIndex *pBxEnd = pRib->GetBxJRibEnd(pBx, nRow);
/*
////////////////////////////////////////////////////////////////////////////
//	임시 루틴 (대우 MBR1 작업)
	CPlateBxFinder FinderMBR((CPlateGirderApp*) this);
	if (bUpper && FinderMBR.GetBxFirst(BX_BENDING, pBxStt, pBxEnd, FALSE, FALSE))  // Taper 구간
	{
		CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
		rect.x = GetLengthJRibJewon(pBx, nRow, TRUE, DIM_BASIC);
		if (FinderMBR.GetBx()->IsState(BX_STT_BENDING_LOWER | BX_STT_BENDING_UPPER))
			rect.y = pRib->m_dWidth + pDB->m_dHeightTaperStt;
		else
			rect.y = pRib->m_dWidth + pDB->m_dHeightTaperEnd;
		return rect;
	}
////////////////////////////////////////////////////////////////////////////
* /
	__int64 BxFlag = bUpper ? (BX_BENDING) : (BX_STT_BYUN_LOWER | BX_END_BYUN_LOWER);
	CPlateBxFinder Finder((CPlateGirderApp*) this);
	BOOL bBend = (Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, FALSE, FALSE) != NULL);  // 꺾이는 종리브.
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	BOOL bGap  = (rect.y - pRib->m_dWidth > pDB->m_pBaseChief->m_BaseEtcCtl.GetJRibOcha());  // 편차값이 허용치보다 큼.
	if (bBend || bGap)
		return rect;

	rect.x = GetLengthJRibJewon(pBx, nRow, bUpper, DIM_CAMBER);
	double slop = GetGiulFlangeUpperLowerCamber(pBxStt->GetStation());
	if (pBxStt->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF) && ((slop < 0) ^ bUpper))
		rect.x += fabs(slop) * pRib->m_dWidth;
	slop = GetGiulFlangeUpperLowerCamber(pBxEnd->GetStation());
	if (pBxEnd->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF) && ((slop > 0) ^ bUpper))
		rect.x += fabs(slop) * pRib->m_dWidth;
	rect.y = pRib->m_dWidth;
	return rect;


/*	CJRib* pRib = pBx->GetJRib(bUpper);

	CDPointArray vRib;
	GetVertexJRib(vRib, pBx, nRow, bUpper);  // 전개된 종리브 꼭지점 좌표구함.
	CDPoint rect = GetRectangleFromVertex(vRib, TRUE, 4, TRUE);  // 직사각형 Size 구함.

	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	if (rect.y - pRib->m_dWidth > pDB->m_pBaseChief->m_BaseEtcCtl.GetJRibOcha())  // 종리브 편차값이 허용치보다 큼.
		return rect;

	CPlateBasicIndex *pBxStt = pRib->GetBxJRibStt(pBx, nRow);
	CPlateBasicIndex *pBxEnd = pRib->GetBxJRibEnd(pBx, nRow);
	__int64 BxFlag = bUpper ? (BX_BENDING) : (BX_STT_BYUN_LOWER | BX_END_BYUN_LOWER);
	CPlateBxFinder Finder((CPlateGirderApp*) this);
	if (Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd, FALSE, FALSE)) // 상부 테이퍼 또는 하부 변단면 꺾임 구간.
		return rect;

	rect.x = GetLengthJRibJewon(pBx, nRow, bUpper, DIM_CAMBER);
	rect.y = pRib->m_dWidth;
	return rect;
*/
	return CDPoint(0,0);
}

//입력 : nDir		0= 중심, -1=좌측, 1=우측
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 수평보강재 길이

// nCol nDimType 는 현재 사용하지 않음
double CPlateGirderApp::GetLengthHStiffJewon(CPlateBasicIndex *pBx, long nRow,long nCol,BOOL bLeft, DIMTYPE nDimType) const
{
	CWebHStiff* pH = pBx->GetWebHStiff();
	return pH->GetLength(nRow, bLeft, nDimType);
}
/*
double CPlateGirderApp::FindStaCrossLeft(CPlateBasicIndex *pBx) const
{
	if(!pBx || !pBx->IsState(BX_CROSSBEAM_VBRACING)) return -1;
	CPlateBasicIndex *pBxMatch = pBx->GetBxInLeftGirderDiap();
	if(!pBxMatch) return -1;

	double  CurSta = pBx->GetStation();
	CDPoint CurAng = pBx->GetAngle();
	double  MatSta = pBxMatch->GetStation();
	CDPoint MatAng = pBxMatch->GetAngle();
	
	if (CurSta == MatSta && CurAng.x == 0.0 && MatAng.x == 0.0)
		return CurSta;

	CDPoint Curxy, Matxy, vCross, vCrossAng;
	double DiapSta, CrossSta;
	DiapSta = GetStationBySc(CurSta, -m_dWidth/2, CurAng);
	Curxy = m_pLine->GetXyLineDis(DiapSta, m_dGirderCenter - m_dWidth/2);
	Matxy = m_pLeft->GetXyGirderDis(MatSta, m_pLeft->m_dWidth/2, MatAng);

	vCross = (Matxy - Curxy).Unit();
	vCrossAng = m_pLine->GetAngleAzimuth(DiapSta).BetweenAngle(vCross);
	CrossSta = m_pLine->GetStationByScInLine(DiapSta, m_dWidth/2, vCrossAng, m_dGirderCenter-m_dWidth/2);

	return CrossSta;
}
CDPoint CPlateGirderApp::FindAngleCrossLeft(CPlateBasicIndex *pBx) const
{
	if(!pBx || !pBx->IsState(BX_CROSSBEAM_VBRACING)) return CDPoint(0,1);

	CPlateBasicIndex *pBxMatch = pBx->GetBxInLeftGirderDiap();
	if(!pBxMatch) return CDPoint(0,1);

	double  CurSta = pBx->GetStation();
	CDPoint CurAng = pBx->GetAngle();
	double  MatSta = pBxMatch->GetStation();
	CDPoint MatAng = pBxMatch->GetAngle();
	
	if (CurSta == MatSta && CurAng.x == 0.0 && MatAng.x == 0.0)
		return CDPoint(0, 1);	

	CDPoint Curxy, Matxy, vCross, vCrossAng;
	double DiapSta, CrossSta;
	DiapSta = GetStationBySc(CurSta, -m_dWidth/2, CurAng);
	Curxy = m_pLine->GetXyLineDis(DiapSta, m_dGirderCenter - m_dWidth/2);
	Matxy = m_pLeft->GetXyGirderDis(MatSta, m_pLeft->m_dWidth/2, MatAng);

	vCross = (Matxy - Curxy).Unit();
	vCrossAng = m_pLine->GetAngleAzimuth(DiapSta).BetweenAngle(vCross);
	CrossSta = m_pLine->GetStationByScInLine(DiapSta, m_dWidth/2, vCrossAng, m_dGirderCenter-m_dWidth/2);
	vCrossAng = m_pLine->GetAngleAzimuth(CrossSta).BetweenAngle(vCross);

	return vCrossAng;
}
double CPlateGirderApp::FindStaCrossRight(CPlateBasicIndex *pBx) const
{
	if(!pBx || !pBx->IsState(BX_CROSSBEAM_VBRACING)) return -1;
	CPlateBasicIndex *pBxMatch = pBx->GetBxInRightGirderDiap();
	if(!pBxMatch) return -1;

	double  CurSta = pBx->GetStation();
	CDPoint CurAng = pBx->GetAngle();
	double  MatSta = pBxMatch->GetStation();
	CDPoint MatAng = pBxMatch->GetAngle();

	if (CurSta == MatSta && CurAng.x == 0.0 && MatAng.x == 0.0)
		return CurSta;	

	CDPoint Curxy, Matxy, vCross, vCrossAng;
	double DiapSta, CrossSta;
	DiapSta = GetStationBySc(CurSta, m_dWidth/2, CurAng);
	Curxy = m_pLine->GetXyLineDis(DiapSta, m_dGirderCenter + m_dWidth/2);
	Matxy = m_pRight->GetXyGirderDis(MatSta, -m_pRight->m_dWidth/2, MatAng);

	vCross = (Curxy - Matxy).Unit();
	vCrossAng = m_pLine->GetAngleAzimuth(DiapSta).BetweenAngle(vCross);
	CrossSta = m_pLine->GetStationByScInLine(DiapSta, -m_dWidth/2, vCrossAng, m_dGirderCenter+m_dWidth/2);

	return CrossSta;
}
CDPoint CPlateGirderApp::FindAngleCrossRight(CPlateBasicIndex *pBx) const
{
	if(!pBx || !pBx->IsState(BX_CROSSBEAM_VBRACING)) return CDPoint(0,1);
	CPlateBasicIndex *pBxMatch = pBx->GetBxInRightGirderDiap();
	if(!pBxMatch) return CDPoint(0,1);

	double  CurSta = pBx->GetStation();
	CDPoint CurAng = pBx->GetAngle();
	double  MatSta = pBxMatch->GetStation();
	CDPoint MatAng = pBxMatch->GetAngle();
	
	if (CurSta == MatSta && CurAng.x == 0.0 && MatAng.x == 0.0)
		return CDPoint(0, 1);	

	CDPoint Curxy, Matxy, vCross, vCrossAng;
	double DiapSta, CrossSta;
	DiapSta = GetStationBySc(CurSta, m_dWidth/2, CurAng);
	Curxy = m_pLine->GetXyLineDis(DiapSta, m_dGirderCenter + m_dWidth/2);
	Matxy = m_pRight->GetXyGirderDis(MatSta, -m_pRight->m_dWidth/2, MatAng);

	vCross = (Curxy - Matxy).Unit();
	vCrossAng = m_pLine->GetAngleAzimuth(DiapSta).BetweenAngle(vCross);
	CrossSta = m_pLine->GetStationByScInLine(DiapSta, -m_dWidth/2, vCrossAng, m_dGirderCenter+m_dWidth/2);
	vCrossAng = m_pLine->GetAngleAzimuth(CrossSta).BetweenAngle(vCross);

	return vCrossAng;
}
*/
//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 상판, 하판, 복부판 대한 길이를 리턴
double CPlateGirderApp::GetLengthCrossFlangeJewon(long nMarkBuje, CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;

	double dResult = 0;
	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
		dResult = GetLengthCrossWebJewon(pBx,nDimType);
	else
	{
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize().x;
	}

	// DIM_BASIC
	return dResult;
}

double CPlateGirderApp::GetLengthCrossWebJewon(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateBridgeApp	 *pDB	   = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(m_pRight);
	CPlateCrossBeam	 *pC	   = pBx->GetCrossBeam();
	CPlateGirderApp  *pGir     = pBx->GetGirder();

	if(pC == NULL)	return 0;

	double dResult = 0;

	// DIM_BASIC
	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		double dWebToWeb	   = pGir->GetDisPlanPlateWebToWeb(pBx);
		double dLenGassetRight = pC->m_P_CR;
		double dLenGassetNext  = pC->m_P_CR;
		if(!pDB->IsExtLine(-1))
		{
			dLenGassetRight = pC->m_P_CR/pBx->GetAngleSectionRight().y;
			dLenGassetNext  = pC->m_P_CR/pBxMatch->GetAngleSectionLeft().y;
		}
		dResult = dWebToWeb - (dLenGassetRight + dLenGassetNext);
	}
	else
	{
		CPlatePieceObject Obj(pBx->GetGirder(),CP_W);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).x;
	}

	return dResult;
}

//입력 : nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 복부판의 길이(높이)를 리턴
double CPlateGirderApp::GetLengthCrossBracketWebJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;

	double dResult = 0;
	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
		dResult = pC->m_H_CR;
	else
	{
		CPlatePieceObject Obj(pBx->GetGirder(),CP_BR_W,NULL,0,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).x;
	}

	return dResult;
}
//입력 : nRow		0=상판, 1=하판
//		 nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 상.하판의 길이(교축방향)
double CPlateGirderApp::GetLengthCrossBracketPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;
	double dResult = 0;

	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		if(nRow == 0)		//상판
			dResult = (pC->m_uM_CR + pC->m_uN_CR)*2;
		else if(nRow == 1)	//하판
			dResult = (pC->m_dM_CR + pC->m_dN_CR)*2;
	}
	else
	{
		long nMarkBuje = (nRow==0) ? CG_BR_F_U : CG_BR_F_L;
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje,NULL,nRow,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize().x;
	}

	return dResult;
}
//입력 : nCol		0=이전 가로보 브라켓, 1=이후 가로보 브라켓(현재 인덱스 기준)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 보강재의 길이(상하방향)
double CPlateGirderApp::GetLengthCrossBracketGasset(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();

	if(pC==NULL || pC->m_cGasset_CR==FALSE)	return 0;
	double dResult = 0;

	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		if(nCol==0)
			dResult = GetHeightGirderByStaAng(pBx->GetStation()) - (pC->m_H_CR+pC->m_T1_CR);
		else
		{
			CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(m_pRight);
			dResult = m_pRight->GetHeightGirderByStaAng(pBxMatch->GetStation()) - (pC->m_H_CR+pC->m_T1_CR);
		}
	}
	else
	{
		long nMarkBuje = CP_BR_GA;
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje,NULL,0,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).x;
	}

	return dResult;

/*
	double  gH	  = 0;
	double  cH	  = 0;
	double  dLen  = 0;
	double	dWebT = 0;
	double  dGiulGasset = GetGiulCrossGasset(pBx,TRUE,TRUE);
	double  dGiulDiap   = 0;
	CDPoint A[41],vAng(0,1);
	GetXyHunchShop(pBx,A);

	if(nCol==0)
	{
		gH		  = GetHeightGirderByStaAng(pBx->GetStation(),m_dWidth/2,pBx->GetAngle());
		dWebT	  = GetThickJewon(G_W_R,pBx);
		vAng	  = pBx->GetAngleSectionRight();
		dGiulDiap = (A[8].y-A[7].y)/(A[8].x-A[7].x);
		cH        = A[7].y + dWebT/vAng.y*dGiulDiap - (A[31].y-pC->m_H_CR-(pC->m_P_CR-dWebT)/vAng.y*dGiulGasset);
		dLen	  = gH - cH - pC->m_T1_CR;
	}
	else
	{
		CPlateBasicIndex *pBxMatch = pBx->GetBxInRightGirderDiap();
		gH		  = m_pRight->GetHeightGirderByStaAng(pBxMatch->GetStation(),-m_dWidth/2,pBxMatch->GetAngle());
		dWebT	  = m_pRight->GetThickJewon(G_W,pBxMatch);
		vAng	  = pBxMatch->GetAngleSectionLeft();
		dGiulDiap = (A[19].y-A[18].y)/(A[19].x-A[18].x);
		cH        = A[19].y - dWebT/vAng.y*dGiulDiap - (A[39].y-pC->m_H_CR+(pC->m_P_CR-dWebT)/vAng.y*dGiulGasset);
		dLen	  = gH - cH - pC->m_T1_CR;
	}

	return dLen;
*/
}

//입력 : nCol		0=이전 가로보 브라켓, 1=이후 가로보 브라켓(현재 인덱스 기준)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 보강판의 길이(상하방향)
double CPlateGirderApp::GetLengthCrossBracketGassetPlate(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;
	if(pC->m_cGasset_CR == FALSE)	return 0;

	BOOL bLeft  = nCol==0 ? TRUE : FALSE;
	CDPoint Gp1 = pC->GetPointGassetSupPlateEndBx(pBx,pBx->GetAngleSectionRight(), TRUE,bLeft);
	CDPoint Gp2 = pC->GetPointGassetSupPlateEndBx(pBx,pBx->GetAngleSectionRight(),FALSE,bLeft);

	double uB = m_pSangse->m_GassetPlate_uB;
	double dB = m_pSangse->m_GassetPlate_dB;
	double dResult = ~(Gp1-Gp2) - (uB + dB);
	return dResult;
}

//입력 : nRow		직각 타입 페이스 플레이트 일 경우에만 적용 0=상.하 플레이트, 1 = 수직 플레이트
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 페이스 플레이트의 길이
double CPlateGirderApp::GetLengthFacePlateJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const
{
	CPlateBridgeApp*	pDB = (CPlateBridgeApp*)m_pBridgeApp;
	double dResult = 0;
	double dWidthFace  = pDB->m_dWidthInOutStt;
	double dHeightFace = pDB->m_dHeightInOutStt;
	double dRadiusFace = pDB->m_dRadiusInOutStt;

	if(pBx->IsDanbuJijum() && pBx->IsState(BX_END_GIRDER))
	{
		dWidthFace  = pDB->m_dWidthInOutEnd;
		dHeightFace = pDB->m_dHeightInOutEnd;
		dRadiusFace = pDB->m_dRadiusInOutEnd;
	}

	if(pDB->m_cFacePlateRoundType)
		dResult = (dWidthFace*2 + dHeightFace - dRadiusFace*4) + dRadiusFace*ConstPi;
	else
	{
		if(nRow == 0)	dResult = dWidthFace;
		else			dResult = dHeightFace;
	}	
	return dResult;
}
//입력 : nDir		0= 중심, -1=좌측, 1=우측
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 종리브 너비
double CPlateGirderApp::GetWidthHStiffJewon(CPlateBasicIndex *pBx, BOOL bLeft, DIMTYPE nDimType) const
{
	CWebHStiff* pH = pBx->GetWebHStiff();

//	ASSERT(pH);
	if(!pH) return 0;

	return pH->m_dWidthHStiff[bLeft ? WEB_LEFT : WEB_RIGHT];
}

//입력 : nRow		0 = 상단 모멘트 이음판, 1= 하단 모멘트 이음판
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 복부판 모멘트 이음판 상하 방향 길이
double CPlateGirderApp::GetWidthSpliceWebMomentJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CPlateSplice *pSp = pBx->GetSplice();
	if(nDimType == DIM_BASIC || nDimType == DIM_CAMBER)		return pSp->m_mW;

	if(nDimType==DIM_RECT)
	{
		BOOL bByunDan = IsByunByStation(pBx->GetStation());
		
		double JongSlop		= GetGiulFlangeUpperLower(pBx->GetStation());
		double BottSlop		= GetGiulFlangeUpperLower(pBx->GetStation(), 0, FALSE);
		double MinSlopJong  = 0;
		double MinSlopBott  = bByunDan ? 0 : MinSlopJong;

		if(fabs(JongSlop) < MinSlopJong) JongSlop = 0.0;
		if(fabs(BottSlop) < MinSlopBott) BottSlop = 0.0;

		long dSlopRoundPos = 3; //반올림 자리수
		JongSlop = Round(JongSlop, dSlopRoundPos);
		BottSlop = Round(BottSlop, dSlopRoundPos);

		double Length = GetLengthJewon(SP_W_MO,pBx,nRow,nCol);
		double mW     = pSp->m_mW;
		double Width  = mW + Length*(fabs(JongSlop)+fabs(BottSlop));

		return Width;
	}
	
//	return (nCol==0) ? pSp->m_mW : pSp->m_mW_R;
	return pSp->m_mW;
}
//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 복부판 전단 이음판 상하 방향 길이
double CPlateGirderApp::GetWidthSpliceWebShareJewon(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CPlateSplice *pSp = pBx->GetSplice();

	double dSw	= pSp->m_sW;
	double dMwU	= pSp->m_nWebType==SP_WEB_FLMO ? GetWidthSpliceWebMomentJewon(pBx, 0, 0, nDimType) : 0;
	double dMwL = pSp->m_nWebType==SP_WEB_FLMO ? GetWidthSpliceWebMomentJewon(pBx, 1, 0, nDimType) : 0;
	if(nDimType == DIM_BASIC || nDimType == DIM_CAMBER)		return dSw+dMwU+dMwL;

	if(nDimType==DIM_RECT)
	{		
		double JongSlop = GetGiulJongDanOnlyLine(pBx->GetStation());
		double MinSlop  = 0;
		if(fabs(JongSlop) < MinSlop) JongSlop = 0.0;

		long dSlopRoundPos = 3; //반올림 자리수
		JongSlop = Round(JongSlop, dSlopRoundPos);

		double Length = GetLengthJewon(SP_W_SH,pBx,nRow,nCol);
		return dSw + Length*fabs(JongSlop)*2 + dMwU + dMwL;
	}

	return dSw+dMwU+dMwL;
}

//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 복부판의 너비(높이)
double CPlateGirderApp::GetWidthCrossFlangeUpper(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;

	double FoldLen = 0;
	if(nDimType==DIM_RECT) 
		FoldLen = GetDisCrossbeamByFolding(pBx);

	return pC->m_uM_CR*2 + fabs(FoldLen);
}

double CPlateGirderApp::GetWidthCrossFlangeLower(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;

	return pC->m_dM_CR*2;
}

//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 복부판의 너비(높이)
double CPlateGirderApp::GetWidthCrossWebPlate(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
 	if(pC == NULL)	return 0;

	if(nDimType==DIM_RECT) 
	{
		CPlatePieceObject Obj(pBx->GetGirder(),CP_W);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		double dResult = Obj.GetRectSize(CDPoint(0,1),FALSE).y;

		return dResult;
	}

	return pC->m_H_CR;
}

//입력 : nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 복부판의 너비를 리턴
double CPlateGirderApp::GetWidthCrossBracketWebJewon(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC   = pBx->GetCrossBeam();
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBridgeApp;
	if(pC == NULL)	return 0;

	double dResult = 0;
	CPlateFactChain *pFact = pBx->GetFactChain(G_W);
	CDPoint vAng = pDB->IsExtLine(-1) ? CDPoint(0,1) : pBx->GetAngleSectionRight();
	dResult = (pC->m_P_CR - pFact->m_dFactChainThick ) / vAng.y;
	
	return dResult;
}

//입력 : nRow		0=상판, 1=하판
//		 nCol		0=현재 거더 우측 브라켓, 1=다음 거더 좌측 브라켓
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 상.하판의 너비
double CPlateGirderApp::GetWidthCrossBracketPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType) const
{
	CPlateGirderApp* pGir      = (CPlateGirderApp*)this;
	CPlateGirderApp* pGirR     = (CPlateGirderApp*)m_pRight;
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(m_pRight);
	CPlateCrossBeam *pC   = pBx->GetCrossBeam();
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBridgeApp;
	if(pC == NULL)	return 0;
	double gH			 = GetHeightGirderByStaAng(pBx->GetStation(), 0, pBx->GetAngle());
	double dResult		 = 0;
	double dLengthGasset = 0;

	CDPoint vAng	= nCol ? pBxMatch->GetAngleSectionLeft() : pBx->GetAngleSectionRight();
	double  webT	= pBxMatch->GetFactChain(G_W)->m_dFactChainThick;
	double dUSharp	= nCol ? pGir->GetLengthSharpOnBx(pBxMatch, TRUE) : pGirR->GetLengthSharpOnBx(pBx, TRUE);
	double dSharp	= nCol ? pGir->GetLengthSharpOnBx(pBxMatch, FALSE) : pGirR->GetLengthSharpOnBx(pBx, FALSE);

	if(nDimType==DIM_BASIC || nDimType==DIM_CAMBER)
	{
		if(nRow==0)	//상판
			dLengthGasset = pC->m_P_CR - dUSharp/vAng.y; // 확폭인 경우 상부돌출폭만 각도를 고려하여 계산...
		if(nRow==1)	//하판
		{
			if(pC->m_H_CR == gH)
				dLengthGasset = pC->m_P_CR - dSharp;
			else 
				dLengthGasset = pC->m_P_CR - webT;
		}
		if(pDB->IsExtLine(-1)) vAng = CDPoint(0,1);
		dResult = dLengthGasset/vAng.y;
	}
	else
	{
		long nMarkBuje = (nRow==0) ? CG_BR_F_U : CG_BR_F_L;
		CPlatePieceObject Obj(pBx->GetGirder(),nMarkBuje,NULL,nRow,nCol);
		pC->CreatePieceObjPlate(&Obj, FALSE, FALSE);
		dResult = Obj.GetRectSize().y;
	}

	return dResult;
}
//입력 : nCol		0=이전 가로보 브라켓, 1=이후 가로보 브라켓(현재 인덱스 기준)
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 브라켓 보강재의 너비
double CPlateGirderApp::GetWidthCrossBracketGasset(CPlateBasicIndex *pBx, long nCol, DIMTYPE nDimType) const
{
	CPlateCrossBeam *pC   = pBx->GetCrossBeam();
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBridgeApp;
	if(pC == NULL)	return 0;
	if(pC->m_cGasset_CR == FALSE)	return 0;

	double dResult = 0;
	CPlateFactChain* pFact = pBx->GetFactChain(G_W);
	CDPoint vAng = pDB->IsExtLine() ? CDPoint(0,1) : pBx->GetAngleSectionRight();
	dResult = (pC->m_P_CR - pFact->m_dFactChainThick) / vAng.y 
			- m_pSangse->m_GassetPlate_S2;
	
	return dResult;
}
//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 모멘트 이음판 너비
double CPlateGirderApp::GetWidthCrossWebSpliceMoment(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	/*
	CDiaphragm *pDia = pBx->GetDiaphragm();
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;
	if(pC->m_mW == 0 || pC->m_mE == 0 || pC->m_mF == 0)	return 0;
	double dResult = pC->m_mW;

	return dResult;
	*/
	return 0;
}
//입력 : 
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 가로보 전단 이음판 너비
double CPlateGirderApp::GetWidthCrossWebSpliceShare(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	/*
	CDiaphragm *pDia = pBx->GetDiaphragm();
	CPlateCrossBeam *pC = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;
	double dResult = (pC->m_sA+pC->m_sB*pC->m_sC+pC->m_sC1/2)*2;

	return dResult;
	*/
	return 0;
}

//입력 : nRow		0=좌측 기준 1번째, 1= 2번째 솔플레이트
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 솔플레이트의 너비(교축직각방향)
double CPlateGirderApp::GetWidthSolePlateJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const
{
	//CDiaphragm *pDia = pBx->GetDiaphragm();
	double dResult = 0;

	/*
	if(nRow == 0)		dResult = pDia->m_ShoeFir.GetHeigthTotalSolePlate();
	else if(nRow == 1)	dResult = pDia->m_ShoeSec.GetHeigthTotalSolePlate();
	*/
	return dResult;
}

//입력 : nIdx
//       nMarkBuje 
//기능 : 현재 스테이션의 두께를 리턴함. 동일 인덱스인(경계선) 경우 이후 두께를 리턴
double CPlateGirderApp::GetThickFactChainJewon(long nMarkBujeFact, CPlateBasicIndex *pBx) const
{
	CPlateFactChain* pFact = pBx->GetFactChain(nMarkBujeFact);
	if(pFact == NULL) return 0;
	return pFact->m_dFactChainThick;
}
//입력 : nRow		0=좌측 기준 1번째, 1= 2번째 솔플레이트
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 솔플레이트의 두께(가장 큰 두께)
double CPlateGirderApp::GetThickSolePlateJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const
{
	/*
	CDiaphragm *pDia = pBx->GetDiaphragm();
	if(!pDia) return 0;
	double dThick    =  pDia->m_ShoeFir.m_dThick_SolePlate;
	double dLength   =  pDia->m_ShoeFir.GetWidthTotalSolePlate();
	double dJongGiul = GetGiulJongDanShoe(pBx, TRUE);

	if(nRow == 1)
	{
		dThick    = pDia->m_ShoeSec.m_dThick_SolePlate;
		dLength   = pDia->m_ShoeSec.GetWidthTotalSolePlate();
		dJongGiul = GetGiulJongDanShoe(pBx, FALSE);
	}

	return RoundUp(dThick + dLength * fabs(dJongGiul) / 2,0);
	*/
	return 0;
}
double CPlateGirderApp::GetGiulJongDanShoe(CPlateBasicIndex *pBx, BOOL bFirShoe) const
{
	/*
	CDiaphragm *pDia = pBx->GetDiaphragm();
	if(!pDia) return 0;
	double dDis = 0;
	if( bFirShoe )		dDis = -m_dWidth/2 + pDia->m_ShoeFir.GetDisShoeFrWeb();
	else				dDis =  m_dWidth/2 - pDia->m_ShoeSec.GetDisShoeFrWeb();

	double dGiulJong = GetGiulJongDanOnlyLine(pBx->GetStation(), dDis, pBx->GetAngle());
//	double dGiulCamber = GetGiulCamber(pBx->GetStation(), dDis, pBx->GetAngle()); 2002.10.04
	double dGiulCamber = 0;

	return dGiulJong + dGiulCamber;

//	return GetGiulFlangeUpperLowerCamber(GetStationBySc(pBx->GetStation(), dDis, pBx->GetAngle()), dDis, FALSE);
	*/
	return 0;
}

//입력 : nRow		0=좌측 기준 1번째, 1= 2번째 솔플레이트
//		 nDimType	0=평면 치수, 1=캠버 치수, 2=제원 치수(직사각형)
//기능 : 해당 인덱스 잭업용 솔플레이트의 두께(가장 큰 두께)
//		 종경사 정확하게 적용 결정할 것.(다이아프램 앞뒤, 좌우에 따라서)
double CPlateGirderApp::GetThickSolePlateJackupJewon(CPlateBasicIndex *pBx, long nRow, DIMTYPE nDimType) const
{
	long nCol = nRow;
	if(nCol>=2) return 0;
	long nJijum = GetNumberJijumByBx(pBx);

	double dJongGiul = GetGiulFlangeUpperLower(pBx->GetStation());
	double dLength   = m_pBridgeApp->m_structJackupJewon[nJijum][nCol].m_dW;
	double dThick    = m_pBridgeApp->m_structJackupJewon[nJijum][nCol].m_dT;	// nCol = 전,후(0,1)

	return RoundUp(dThick + dLength/2 * fabs(dJongGiul),0);
}
//<summary>
//pBx 가 속한 Splice 번호를 리턴 (1, 2...)  *** [1] 부터 시작함
//</summary>
//<param name="*pBx"></param>
//<param name="bNext/*=FALSE*/"></param>
//		 bNext	bNext = FALSE	현재 pBx가 현장이음이면 현재 현장이음 번호 리턴,
//					    TRUE => 현재 pBx가 현장이음이면 다음 현장이음 번호 리턴
//<return></return>
CString CPlateGirderApp::GetNumberSpliceForOut(CPlateBasicIndex *pBx, BOOL bNext/*=FALSE*/) const
{
	long nSp = GetNumberSpliceByBx(pBx)+1;

	if(bNext == TRUE && pBx->IsState(BX_SPLICE) == TRUE)	nSp++;
	CString str;

	str.Format("%02d", nSp + ((CPlateBridgeApp*)GetBridgeApp())->GetNumberStartSplice());
	return str;
}
//입력 : pBx
//기능 : pBx 속한 Splice 번호를 리턴 (0, 1, 2...)
//       =>스플라이스 위치와 pBx 동일하면 현재 스플라이스 객체 번호를 리턴
//       =>마지막 현장이음를 지난 구간은 총 현장이음 수를 리턴 
long CPlateGirderApp::GetNumberSpliceByBx(CPlateBasicIndex *pBx) const
{
	if( !pBx ) return -1;

	long nCount = 0;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex* p = Finder.GetBxFirst(BX_SPLICE);
	while(p)
	{
		if(pBx->GetStation() <= p->GetStation())
			return nCount;
		nCount++;
		p = Finder.GetBxNext();
	}

	return nCount;
}


//입력 : nSp
//기능 : nSp에 해당하는 pBx 를 리턴
//		 nSp < 0 일경우     거더 시점 pBx 리턴(BX_STT_GIRDER, CENTER, RIGHT 중에서 Sta가 가장 작은것)
//		 nSp > nSpsu일 경우 거더 종점 pBx 리턴(BX_END_GIRDER, CENTER, RIGHT 중에서 Sta가 가장 큰 것)
CPlateBasicIndex* CPlateGirderApp::GetBxBySpliceNo(long nSp) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	if(nSp < 0) 
		return Finder.GetBxFirst(BX_STT_GIRDER)->GetBxMin();

	Finder.GetBxFirst(BX_END_GIRDER|BX_SPLICE);
	CPlateBasicIndex *pBxResult = Finder.GetBxNext(nSp);
	if(pBxResult == Finder.GetBxFixTail())
		return Finder.GetBxLast(BX_END_GIRDER)->GetBxMax();
 
	return pBxResult;
}

//입력 : nSp
//기능 : nSp에 해당하는 pBx 를 리턴
//		 nSp < 0 일경우     거더 시점 pBx 리턴(BX_STT_GIRDER, CENTER, RIGHT 중에서 Sta가 가장 작은것)
//		 nSp > nSpsu일 경우 거더 종점 pBx 리턴(BX_END_GIRDER, CENTER, RIGHT 중에서 Sta가 가장 큰 것)
CPlateBasicIndex* CPlateGirderApp::GetBxBySpliceNoSPType(long nSp) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_END_GIRDER|BX_SPLICE);
	long nCount=0;
	while(pBx)
	{
		if(pBx->GetSplice()==NULL)
		{
			pBx = Finder.GetBxNext();
			continue;
		}

		if(nSp==nCount && pBx->GetSplice()->m_bSpliceType)
			return pBx;
		
		if(pBx->GetSplice()->m_bSpliceType)
			nCount++;

		pBx = Finder.GetBxNext();
	}
	return NULL;
}


//입력 : nSp
//기능 : nSp에 해당하는 pBx 를 리턴
//		 nSp < 0 일경우     거더 시점 pBx 리턴(BX_STT_GIRDER, CENTER, RIGHT 중에서 Sta가 가장 작은것)
//		 nSp > nSpsu일 경우 거더 종점 pBx 리턴(BX_END_GIRDER, CENTER, RIGHT 중에서 Sta가 가장 큰 것)
CPlateBasicIndex* CPlateGirderApp::GetBxBySpliceNoWeldType(long nSp) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_END_GIRDER|BX_SPLICE);
	long nCount=0;
	while(pBx)
	{
		if(nSp==nCount && pBx->GetSplice()->m_bSpliceType==FALSE)
			return pBx;
		
		if(pBx->GetSplice()->m_bSpliceType==FALSE)
			nCount++;

		pBx = Finder.GetBxNext();
	}
	return NULL;
}


//입력 : nSp
//기능 : nSp에 해당하는 pBx 를 리턴
//		 nSp < 0 일경우     거더 시점 pBx 리턴(BX_STT_GIRDER_CETNER 리턴)
//		 nSp > nSpsu일 경우 거더 종점 pBx 리턴(BX_END_GIRDER 리턴)
CPlateBasicIndex* CPlateGirderApp::GetBxBySpliceNoCenter(long nSp) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	if(nSp < 0) 
		return Finder.GetBxFirst(BX_STT_GIRDER);

	Finder.GetBxFirst(BX_END_GIRDER|BX_SPLICE);
	CPlateBasicIndex *pBxResult = Finder.GetBxNext(nSp);
	if(pBxResult == Finder.GetBxFixTail())
		return Finder.GetBxLast(BX_END_GIRDER);
 
	return pBxResult;
}

//입력 : pBx (Center 기준)
//기능 : pBx가 속하는 nSp(결국 Box 번호)를 리턴 (위 함수의 정반대.)
//		(거더 시점 Bx만 -1임. 그럼 경계선은 어떻게 되는지 알겠쥐? -> 첫번째 현장이음은 0임.)
long CPlateGirderApp::GetSpliceNoByBx(CPlateBasicIndex *pBx) const
{
	if (pBx->IsState(BX_STT_GIRDER))
		return -1;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	pBx = Finder.GetBxFirst(BX_SPLICE, pBx);
	if (!pBx)
		return GetQtySplice();
	return pBx->GetSplice()->GetSelfCount();

}

double CPlateGirderApp::GetWidthSlabLeftDom(CPlateBasicIndex *pBx) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	if(!m_pLeft==NULL)	return 0;
	if(!pDB->IsExtLine())	return pDB->m_dWidthSlabLeft;
	CDPoint vAng;
	if(pBx == NULL)		vAng = GetAAA(OFF_STT_SHOE);
	else				vAng = pBx->GetAngle();

	return	pDB->m_dWidthSlabLeft / vAng.y;
}
double CPlateGirderApp::GetWidthSlabRightDom(CPlateBasicIndex *pBx) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	if(!m_pRight==NULL)	return 0;
	if(!pDB->IsExtLine())	return pDB->m_dWidthSlabRight;
	CDPoint vAng;
	if(pBx == NULL)		vAng = GetAAA(OFF_STT_SHOE);
	else				vAng = pBx->GetAngle();

	return	pDB->m_dWidthSlabRight / vAng.y;
}

//입력 : bShop == TRUE  Shop에서 사용
//기능 : 현재 인덱스에서의 가로보 기울기([37], [33])를 구함
//		 가로보가 없으면 "0" 리턴
double CPlateGirderApp::GetGiulCrossBody(CPlateBasicIndex *pBx, BOOL bShop) const
{
	CDPoint A[10];
	pBx->GetCrossBeam()->GetCrossXyDom(A);
	if(A[5].y == A[2].y)	return 0;
	
	return (A[5].y-A[2].y)/(A[5].x-A[2].x);	//좌측기준 올라가면 + 
}
//입력 : bShop  TRUE일 경우 Shop에서 사용
//		 bLeft  TRUE일 경우 가로보 기준하여 좌측 브라켓의 기울기를 구함
//기능 : 현재 인덱스에서의 가로보 기울기([37], [33])를 구함
//		 가로보가 없으면 "0" 리턴
double CPlateGirderApp::GetGiulCrossGasset(CPlateBasicIndex *pBx, BOOL bShop, BOOL bLeft) const
{
	CDPoint A[10];
	pBx->GetCrossBeam()->GetCrossXyDom(A);

	if( bLeft && A[2].y==A[0].y) return 0;
	if(!bLeft && A[7].y==A[5].y) return 0;

	if(bLeft == TRUE)	return (A[2].y-A[0].y)/(A[2].x-A[0].x);	//좌측기준 올라가면 + 
	else				return (A[7].y-A[5].y)/(A[7].x-A[5].x);	//좌측기준 올라가면 + 
}
 
double CPlateGirderApp::GetLengthCrossTaper(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp *)m_pBridgeApp;
	CGlobarOption *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	CPlateCrossBeam	*pC	= pBx->GetCrossBeam();

	if(pBx->GetNumberGirder() == pDB->GetQtyGirder()-1)
		pBx = pBx->GetBxMatchByCrossBeam(pBx->GetGirder()->m_pLeft);

	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CPlateBasicIndex	*pBxSec		= pBx->GetBxMatchByCrossBeam(pGir->m_pRight);

	CDPoint vGir		= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double	dWVStiffL	= pBx->GetSection()->GetVStiff()->m_dWidth[1];
	double	dWVStiffR	= pBxSec->GetSection()->GetVStiff()->m_dWidth[0];
	double	dCross		= fabs(pBx->GetAngleSectionCrossVBracing().RotateInv(vGir).y);

	CDPoint xyCrL, xyCrR;
	pC->GetXyBaseLR(xyCrL, xyCrR);
	if(pC->GetType()==CR_TYPE_HSTEEL)
	{
		double dGab = (dWVStiffL-pC->m_dbL1-pC->m_dbL2) + (dWVStiffR-pC->m_dbR1-pC->m_dbR2);
		return ~(xyCrL-xyCrR)-dGab/dCross;		
	}
	else
		return ~(xyCrL-xyCrR);

}
double CPlateGirderApp::GetHeightCrossTaper(CPlateBasicIndex *pBx) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp *)m_pBridgeApp;
	CGlobarOption *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	CPlateCrossBeam	*pC	= pBx->GetCrossBeam();

	CDPoint A[10];
	pC->GetCrossXyDom(A);

	if(pC->GetType()==CR_TYPE_HSTEEL)
		return pC->GetSteelMaterialData()->m_dH;
	
	if(pGlopt->GetDesignCrossBeamBracketType() == DIM_BASIC)		//일반 타입이면
		return A[5].y - A[2].y;
	else															//레벨 타입이면
		return A[4].y - A[3].y;						
}

//입력 : pBx
//기능 : Piece 에서 Folding할때의 좌우 거리차
//		 [-]이면 이전 스테이션
double CPlateGirderApp::GetDisCrossbeamByFolding(CPlateBasicIndex *pBx, BOOL bCamber) const
{
/*	if(pBx->GetCrossBeam() == NULL)	return 0;
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
//	CBaseChief *pBC	= (CBaseChief*)pDB->m_pBaseChief;
//	if(pBC->m_BaseEtcCtl.GetCrossBeamType() == 0)	return 0;		//일반 타입이면
	double dH = 0;//GetHeightCrossbeamAct(pBx);
	double dSta = GetStationBySc(pBx->GetStationCrossRight(), m_dWidth/2+m_dURightSharp, pBx->GetAngleSectionRight());
	double dGiul = GetGiulFlangeUpperLowerCamber(dSta, m_dWidth/2+m_dURightSharp);
	if(dGiul == 0)	return 0;
	return sin(atan(dGiul)) * dH;

  */
	return 0;
}
// 리턴 : 포인트의 개수, Po[]
// bIncludeSide ; TRUE - 가로보 이음판외측에도 수평보강재를 배치
long CPlateGirderApp::GetPointVStiffOnCrossBeam(CPlateBasicIndex *pBx, CDPoint Po[], BOOL bShopCut, BOOL bIncludeSide) const
{
	CPlateCrossBeam	*pC	  = pBx->GetCrossBeam();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)GetBridgeApp();
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();
	if(pC == NULL)	return 0;
	if(pC->m_cVSide_CR == 1) return 0;

	CDPoint A[10];
	pC->GetCrossXyDom(A);
	CDPoint vAng = bShopCut ? pBx->GetAngleSectionRight() : CDPoint(0,1);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxPrv = Finder.GetBxLast(BX_CROSSBEAM_VBRACING, pBx,NULL,FALSE);
	CPlateCrossBeam	*pCPrv = NULL;
	while(pBxPrv)
	{
		pCPrv = pBxPrv->GetCrossBeam();
		if(pCPrv)
			break;
		pBxPrv = Finder.GetBxNext();
	}

	long   nvStiffSu   = pC->m_nVStiffsu_CR;
	double DisVStiff   = 0;
	double giulCross   = GetGiulCrossBody(pBx,bShopCut);
	double LevelDist   = A[3].x - A[2].x/*A[33].x - A[31].x*/;
	long	n = 0;
	for(n=0; n<18; n++) Po[n] = CDPoint(-1,-1);
	CDPointArray PoArr;

//	if (bIncludeSide)
	{
		PoArr.Add(A[8/*7*/]);
		PoArr.Add(A[9/*19*/]);
	}
	PoArr.Add(A[2/*31*/]);
	PoArr.Add(A[5/*39*/]);

	double dShearHalfW = (pC->m_sB*pC->m_sC)+pC->m_sA+pC->m_sC1/2;
	if (pC->m_cVSide_CR != 1)
	{
		for(n=0; n < nvStiffSu; n++)
		{
			DisVStiff += pC->m_VStiffDis_CR[n]/vAng.y;
			CDPoint Potemp;
			Potemp.x = A[2/*31*/].x + DisVStiff + dShearHalfW;
			Potemp.y = A[3/*33*/].y + (DisVStiff-LevelDist)*giulCross;
			PoArr.Add(Potemp);
		}
	}
	/*
	BOOL bDanBu = pGlopt->GetDesignStringerJijumApply();
	
	if (pBx->IsAbutEnd() && bDanBu)
	{
		for(n=0; n < pCPrv->m_nQtyStringer_CR; n++)
		{
			CStringer *pStr   = pBxPrv->GetStringer(n);
			if(pStr)
			{
				CDPoint    Potemp = CDPoint(A[7].x + (GetDisStringerEnd(pBxPrv, n)-m_dUWidth/2)/vAng.y,0);
				if(pDB->IsExtLine())
					Potemp.x = A[7].x + (GetDisStringerEnd(pBxPrv, n)-m_dUWidth/2)/pBx->GetAngleSectionRight().y;
				Potemp.y = A[33].y + ((Potemp.x - A[31].x)-LevelDist)*giulCross;
				PoArr.Add(Potemp);
			}
		}
	}
	else if (!pBx->IsAbutEnd())
	{
		for(n=0; n < nStringerSu; n++)
		{
			CStringer *pStr   = pBx->GetStringer(n);
			if(pStr)
			{
				CDPoint    Potemp = CDPoint(A[7].x + (GetDisStringer(pBx, n)-m_dUWidth/2)/vAng.y,0);
				Potemp.y = A[33].y + ((Potemp.x - A[31].x)-LevelDist)*giulCross;
				PoArr.Add(Potemp);
			}
		}
	}
	*/

	PoArr.Sort();
	for(n=0; n < PoArr.GetSize(); n++)
		Po[n] = PoArr.GetAt(n);

	return PoArr.GetSize();
/*	CPlateCrossBeam	*pC	  = pBx->GetCrossBeam();
	if(pC == NULL)	return 0;

	CDPoint A[10];
	bShopCut ? GetXyHunchShop(pBx, A)  : GetHunchXyDom(pBx, A);
	CDPoint vAng = bShopCut ? pBx->GetAngleSectionRight() : CDPoint(0,1);

	for(long n=0; n<12; n++) Po[n] = CDPoint(-1,-1);
	long  nPoint     = pC->m_nVStiffsu_CR;
	double DisVStiff = 0;
	double giulCross = GetGiulCrossBody(pBx,bShopCut);
	double LevelDist = A[33].x - A[31].x;
	BOOL bMeetStringer   = FALSE;
	BOOL bStringerInsert = FALSE;

	n = 0;
	if(bIncludeSide) {Po[0] = A[7]; n=1; nPoint++;}
	Po[n] = A[31];

	long nV = 0;
	for(; n<nPoint; n++,nV++) {
		DisVStiff += pC->m_VStiffDis_CR[nV]/vAng.y;
		Po[n+1].x  = A[31].x + DisVStiff;
		Po[n+1].y  = A[33].y + (DisVStiff-LevelDist)*giulCross;
		// 세로보가 있을 경우 포인트를 조정
		if(pBx->IsStringer() && !bStringerInsert) 
		{
			if(CheckMeetCrossBeamVStiffStringer(pBx,Po[n+1].x,bShopCut))
				Po[n+1] = A[35];
			else if(Po[n+1].x > A[35].x && Po[n].x < A[35].x) { // 세로보 추가
				bStringerInsert = TRUE;
				Po[n+1] = A[35];	
				n++;
				nPoint++;
				Po[n+1].x = A[31].x + DisVStiff;
				Po[n+1].y = A[33].y + (DisVStiff-LevelDist)*giulCross;
			}
		}
	}

	Po[++n] = A[39];
	if(bIncludeSide) Po[++n] = A[19];

	return n+1;*/

}

// 리턴 : 포인트의 개수, Po[]
// bIncludeSide ; TRUE - 가로보 이음판외측에도 수평보강재를 배치
long CPlateGirderApp::GetPointVStiffOnCrossBeamPrv(CPlateBasicIndex *pBx, CDPoint Po[], BOOL bShopCut, BOOL bIncludeSide) const
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC	  = pBx->GetCrossBeam();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)GetBridgeApp();
	CGlobarOption   *pGlopt= pDB->m_pDataManage->GetGlobalOption();

	if(pC == NULL)	return 0;
	if(pC->m_cVSide_CR == 2) return 0;

	CDPoint A[10];
	pC->GetCrossXyDom(A);
	CDPoint vAng = bShopCut ? pBx->GetAngleSectionRight() : CDPoint(0,1);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxPrv = Finder.GetBxLast(BX_CROSSBEAM_VBRACING, pBx,NULL,FALSE);
	CPlateCrossBeam	 *pCPrv  = NULL;
	while(pBxPrv)
	{
		pCPrv = pBxPrv->GetCrossBeam();
		if(pCPrv)
			break;
		pBxPrv = Finder.GetBxNext();
	}
	long   nvStiffSu   = pC->m_nVStiffsu_CR;
	double DisVStiff   = 0;
	double giulCross   = GetGiulCrossBody(pBx,bShopCut);
	double LevelDist   = A[3].x - A[2].x/*A[33].x - A[31].x*/;
	double dShearHalfW = (pC->m_sB*pC->m_sC)+pC->m_sA+pC->m_sC1/2;
	double dTGirWeb    = pGir->GetThickFactByStation(pBx->GetStation(), G_W);  
	long	n	= 0;
	for(n=0; n<18; n++) Po[n] = CDPoint(-1,-1);
	CDPointArray PoArr;

//	if (bIncludeSide)	// 항상 포함되어야 함...
	{
		PoArr.Add(A[8/*7*/]);
		PoArr.Add(A[9/*19*/]);
	}
	if(pC->m_nLGirSpType)		PoArr.Add(A[2/*31*/]);
	else						PoArr.Add(A[0/*31*/]);

	if(pC->m_nRGirSpType)		PoArr.Add(A[5/*39*/]);
	else						PoArr.Add(A[7/*39*/]);

	
	if (pC->m_cVSide_CR != 2)
	{
		for(n=0; n < nvStiffSu; n++)
		{
			DisVStiff += pC->m_VStiffDis_CR[n]/vAng.y;
			CDPoint Potemp;
			Potemp.x = A[2/*31*/].x + DisVStiff + dShearHalfW;
			if(pC->m_nLGirSpType==FALSE)	// 이음판이 없을 경우
				Potemp.x = A[8].x + dTGirWeb + pC->GetLengthGirWebToCrWeb(TRUE) + DisVStiff;

			Potemp.y = A[3/*33*/].y + (DisVStiff-LevelDist)*giulCross;
			PoArr.Add(Potemp);
		}
	}
	/*
	BOOL bDanBu = pGlopt->GetDesignStringerJijumApply();
	
	if (pBx->IsAbutStt() && bDanBu)
	{
		for(n=0; n < nStringerSu; n++)
		{
			CStringer *pStr   = pBx->GetStringer(n);
			if(pStr)
			{
				CDPoint    Potemp = CDPoint(A[7].x + (GetDisStringer(pBx, n)-m_dUWidth/2)/vAng.y,0);
				Potemp.y = A[33].y + ((Potemp.x - A[31].x)-LevelDist)*giulCross;
				PoArr.Add(Potemp);
			}
		}
	}
	else if (!pBx->IsAbutStt())
	{
		for(n=0; n < pCPrv->m_nQtyStringer_CR; n++)
		{
			CStringer *pStr   = pBxPrv->GetStringer(n);
			if(pStr)
			{
				CDPoint    Potemp = CDPoint(A[7].x + (GetDisStringerEnd(pBxPrv, n)-m_dUWidth/2)/vAng.y,0);
				Potemp.y = A[33].y + ((Potemp.x - A[31].x)-LevelDist)*giulCross;
				PoArr.Add(Potemp);
			}
		}
	}
	*/

	PoArr.Sort();
	for(n=0; n < PoArr.GetSize(); n++)
		Po[n] = PoArr.GetAt(n);

	return PoArr.GetSize();
}

// 임의의 플랜지의 좌우측 꼭지점들을 구함. (지그재그 형태...)
// (입력 Station은 거더중심기준. 거더 시종점부만 사각 가능. 일반부는 무조건 90도.)
void CPlateGirderApp::GetVertexPointByDis(CVectorArray &vFlange, double staStt, double staEnd, double widL, double widR, BOOL bUpper) const
{	
/*	double staSttL, staSttR, staEndL, staEndR;
	staSttL = staSttR = staStt;
	staEndL = staEndR = staEnd;
	if (staStt == GetSSS(OFF_STT_GIRDER))
	{
		staSttL = GetStationBySc(staStt, widL, GetAAA(OFF_STT_GIRDER));
		staSttR = GetStationBySc(staStt, widR, GetAAA(OFF_STT_GIRDER));
	}
	if (staEnd == GetSSS(OFF_END_GIRDER))
	{
		staEndL = GetStationBySc(staEnd, widL, GetAAA(OFF_END_GIRDER));
		staEndR = GetStationBySc(staEnd, widR, GetAAA(OFF_END_GIRDER));
	}

	vFlange.RemoveAll();
	CVectorArray vLeft, vRight;
	GetVertexFlangeByStation(vLeft,  staSttL, staEndL, bUpper);
	GetVertexFlangeByStation(vRight, staSttR, staEndR, widR, bUpper);
	long nCount = min(vLeft.GetSize(), vRight.GetSize());

	for (long i = 0; i < nCount; i++)   {
		vFlange.Add(vLeft.GetAt(i));
		vFlange.Add(vRight.GetAt(i));
	}*/
}

// 구간 내에 변단면 존재 유무 구함. (나중에 다시 작성.)
BOOL CPlateGirderApp::IsByunByTerm(double StaStt, double StaEnd) const
{
	double dSttH = GetHeightFlangeLower(StaStt);
	double dEndH = GetHeightFlangeLower(StaEnd);
	double dMidH = GetHeightFlangeLower((StaStt + StaEnd) / 2);
	return !((dSttH == dMidH) && (dEndH == dMidH));
}
 
// 변단면 위치인지 조사.
BOOL CPlateGirderApp::IsByunByStation(double sta) const
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_BYUN);
	CDDWordArray darrSttByunLower, darrSttByunUpper;
	CDDWordArray darrEndByunLower, darrEndByunUpper;
	while (pBx)
	{
		if(pBx->IsState(BX_STT_BYUN_LOWER))
			darrSttByunLower.Add(pBx->GetStation());
		if(pBx->IsState(BX_STT_BYUN_UPPER))
			darrSttByunUpper.Add(pBx->GetStation());
		if(pBx->IsState(BX_END_BYUN_UPPER))
			darrEndByunUpper.Add(pBx->GetStation());
		if(pBx->IsState(BX_END_BYUN_LOWER))
			darrEndByunLower.Add(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}
	long i = 0;
	for(i=0;i<darrSttByunLower.GetSize();i++)
	{
		double	dStaLower	= darrSttByunLower.GetAt(i);
		double	dStaUpper	= darrSttByunUpper.GetAt(i);
		if(sta > dStaLower && sta < dStaUpper)
			return TRUE;
	}
	for(i=0;i<darrEndByunUpper.GetSize();i++)
	{
		double	dStaLower	= darrEndByunUpper.GetAt(i);
		double	dStaUpper	= darrEndByunLower.GetAt(i);
		if(sta > dStaLower && sta < dStaUpper)
			return TRUE;
	}
// 	for(long i=0;i<darrSttByunLower.GetSize();i++)
// 	{
// 		if(sta > darrSttByunLower.GetAt(i) && sta < darrSttByunUpper.GetAt(i)) return TRUE;
// 	}
// 	for(i=0;i<darrEndByunUpper.GetSize();i++)
// 	{
// 		if(sta > darrEndByunUpper.GetAt(i) && sta < darrEndByunLower.GetAt(i)) return TRUE;
// 	}
	return FALSE;
	//if (pBx && pBx->GetStation() == sta)
	//	return TRUE;
	//return bByun;
}

// 현장이음 구간내의 전단연결재 수
long CPlateGirderApp::GetStudBoltSu(long nSp)
{
	double SttSta, EndSta;
	SttSta = GetBxBySpliceNo(nSp-1)->GetStation();
	EndSta = GetBxBySpliceNo(nSp)->GetStation();

	BOOL bFirst = TRUE;
	double Miss = 50;
	long nStud = m_pSangse->m_StudHori_Qty, nSum  = 0;
	double sta = GetSSS(OFF_STT_GIRDER);

	if (nSp !=0 && m_pSangse->m_StudJong_SpliceCenter)
		nSum = nStud;
	for (long i = 0; i < STUD_JONG_SU; i++)   {
		long   n    = m_pSangse->m_StudJong_Studsu[i];
		double term = m_pSangse->m_StudJong_StudDis[i];
		if (term * n < 1)
			break;
		sta += term * n;
		if (SttSta - Miss < sta)   {
			if (i == 0)
				bFirst = FALSE;
			if (bFirst)   {
				bFirst = FALSE;
				if (sta < SttSta + Miss)   {  // 시작 경계선상에 걸림
					sta = SttSta;  // 스테이션 보정
					continue;
				}
				else
					n = (long) RoundUp((sta - SttSta) / term, 0);
			}
			if (EndSta - Miss < sta)   {
				if (sta >= EndSta + Miss)   {  // 끝 경계선상에 걸리지 않음
					n = n - (long) RoundUp((sta - EndSta) / term, 0);
					sta = EndSta;
				}
			}
			nSum += nStud * n;
		}
		if (EndSta - Miss < sta)
			break;
	}

	return nSum;
}


//입력 : nJijum
//		 nJackup	해당 지점의 잭업 순번 0~5(최대 6개)
//		 nSide		-1=좌, 0=중앙, 1=우
//기능 : 
CJackupStiff *CPlateGirderApp::GetJackupByJijum(long nJijum, long nJackup) const
{
	if(nJackup >= JACKUPSUMAX)	return NULL;

	__int64 BxFlag = BX_JACKUP;
	
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
	long nJack = 0;
	while (pBx)
	{
		long nK = GetNumberJijumByBx(pBx);
		if(nK == nJijum)
		{
			if(nJack == nJackup)	return pBx->GetJackupStiff();
			else					nJack++;
		}

		pBx = Finder.GetBxNext();
	}
	return NULL;
}

//입력   pBx     : 지점부 Bx (결국... 다이아프램 Bx)
//       nJackup : Bx 기준 잭업보강재 순서 (+ : 종점 방향, - : 시점방향)
//기능   자기 지점에 속하는 잭업보강재 pBx 를 리턴
CPlateBasicIndex* CPlateGirderApp::GetJackupBxByJijumBx(CPlateBasicIndex *pBx, long nJackup, long nSide) const
{
	if (nJackup == 0)	return NULL;
	long nJackCount = abs(nJackup);
	long nJijum = GetNumberJijumByBx(pBx);
	__int64 BxFlag = BX_JACKUP;
	//pBx = pBx->GetBxCenter();
	if (nSide > 0)   {
		BxFlag = BX_JACKUP;
		pBx = pBx->GetBxRight();
	}
	if (nSide < 0)   {
		BxFlag = BX_JACKUP;
		pBx = pBx->GetBxLeft();
	}
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxJack;
	if (nJackup < 0)
		pBxJack = Finder.GetBxLast(BxFlag, pBx);
	else
		pBxJack = Finder.GetBxFirst(BxFlag, pBx);

	if (nJackCount > 1)
		pBxJack = Finder.GetBxNext(nJackCount - 1);
	if (pBxJack == NULL || GetNumberJijumByBx(pBxJack) != nJijum)
		return NULL;

	return pBxJack;
}

/*
//<summary>
//거더 중앙에서 세로보 까지의 수직 거리
//</summary>
//<param name="*pBx"></param>
//<param name="nStr">스트링거 번호, 1개의 가로보에 있는 스트링거 번호</param>
//<return></return>
double CPlateGirderApp::GetDisStringer(CPlateBasicIndex *pBx, long nStr) const
{
//  2003.03.10 수정사유 : 확폭일경우 pC->m_P_CR가 실제거리이므로 xyLeft,xyRight가 잘못나옴.
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateCrossBeam* pC = pBx->GetCrossBeam();
	if(!pC) return 0;
	CPlateGirderApp* pGirLeft  = pBx->GetGirder();
	CPlateGirderApp* pGirRight = pGirLeft->m_pRight;;

	double staLeft		 =  pC->GetStationCrossLeftInCrossBeam();
	CDPoint angLeft		 = pC->GetAngleSectionLeftInCrossBeam();
	CDPoint xyLeft		 =  pGirLeft->GetXyGirderDis(staLeft, pGirLeft->m_dUWidth/2, angLeft);
	double staRight		 = pC->GetStationCrossRightInCrossBeam();
	CDPoint angRight	 = pC->GetAngleSectionRightInCrossBeam();
	CDPoint xyRight		 =  pGirRight->GetXyGirderDis(staRight, -(pGirRight->m_dUWidth/2), angRight);

	double  dP_Left		 = pDB->IsExtLine() ? pC->m_P_CR : pC->m_P_CR/angLeft.y;
	double  dP_Right	 = pDB->IsExtLine() ? pC->m_P_CR : pC->m_P_CR/angRight.y;
	double  dValuePos    = (double)(nStr+1) / (double)(pC->m_nQtyStringer_CR+1);
	double  dLengthCro   = ~(xyRight-xyLeft) - (dP_Left+dP_Right);
	CDPoint xyCrossCen   = xyLeft + (xyRight-xyLeft).Unit() * (dP_Left+dLengthCro*dValuePos);
	double  dDisToCL     = pGirLeft->GetLine()->GetDisMatchVerticalByXy(xyCrossCen);
	double	dDisStringer = dDisToCL - pGirLeft->m_dGirderCenter;

	if(pDB->IsExtLine())
		dDisStringer = ~(xyCrossCen-xyLeft) + pGirLeft->m_dUWidth/2;

	return fabs(dDisStringer);	//확폭일 경우

/*  2003.03.10 수정하기전의 원본
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateCrossBeam* pC = pBx->GetCrossBeam();
	if(!pC) return 0;
	CPlateGirderApp* pGirLeft  = pBx->GetGirder();
	CPlateGirderApp* pGirRight = pGirLeft->m_pRight;;

	double staLeft		 =  pC->GetStationCrossLeftInCrossBeam();
	CDPoint angLeft		 = pC->GetAngleSectionLeftInCrossBeam();
	CDPoint xyLeft		 =  pGirLeft->GetXyGirderDis(staLeft, pGirLeft->m_dUWidth/2+pC->m_P_CR, angLeft);
	double staRight		 = pC->GetStationCrossRightInCrossBeam();
	CDPoint angRight	 = pC->GetAngleSectionRightInCrossBeam();
	CDPoint xyRight		 =  pGirRight->GetXyGirderDis(staRight, -(pGirRight->m_dUWidth/2+pC->m_P_CR), angRight);

	double  dValuePos    = (double)(nStr+1) / (double)(pC->m_nQtyStringer_CR+1);
	double  dLengthCro   = ~(xyRight-xyLeft);
	CDPoint xyCrossCen   = xyLeft + (xyRight-xyLeft).Unit() * dLengthCro * dValuePos;
	double  dDisToCL     = pGirLeft->GetLine()->GetDisMatchVerticalByXy(xyCrossCen);
	double	dDisStringer = dDisToCL - pGirLeft->m_dGirderCenter;

	return fabs(dDisStringer);	//확폭일 경우
*/
/*	2002.7.20 수정 하기 전의 원본임.
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateCrossBeam* pC = pBx->GetCrossBeam();
	if(!pC) return 0;
	long	nQtyStringer = pC->m_nQtyStringer_CR;
	CPlateGirderApp* pGirLeft	= pBx->GetGirder();
	CPlateGirderApp* pGirRight	= pGirLeft->m_pRight;;
	if(nQtyStringer <= 0)	return 0;
	double staLeft = pC->GetStationCrossLeftInCrossBeam();
	CDPoint angLeft = pC->GetAngleSectionLeftInCrossBeam();
	CDPoint xyLeft =  pGirLeft->GetXyGirderDis(staLeft, pGirLeft->m_dUWidth+pC->m_P_CR, angLeft);
	double staRight = pC->GetStationCrossRightInCrossBeam();
	CDPoint angRight = pC->GetAngleSectionRightInCrossBeam();
	CDPoint xyRight =  pGirRight->GetXyGirderDis(staRight, -(pGirRight->m_dUWidth+pC->m_P_CR), angRight);
	CDPoint xyCrossCen = (xyLeft+xyRight)/(nQtyStringer+1);
	double	dDisStringer = pGirLeft->GetLine()->GetDisMatchVerticalByXy(xyCrossCen)-pGirLeft->m_dGirderCenter;
* /
}
*/


/////////////////////////////////////////////////////////////////////////////////////////////
///현재 거더 pBx 복부판 중심에서 다음 거더 매치 pBx 복부판 중심까지의 실제길이(사거리)
///매치 pBx가 없으면 0를 리턴
///  nDimType == DIM_CAMBER 일 경우 종단 & Camber 적용한 치수(3차원 치수)
///  nDimType == DIM_CAMBER 가 아닐 경우는 2차원 치수
/////////////////////////////////////////////////////////////////////////////////////////////
double CPlateGirderApp::GetDisCRSttToCREndReal(CPlateBasicIndex *pBx, DIMTYPE nDimType) const
{
	CPlateBasicIndex *pBxRight = pBx->GetBxMatchByCrossBeam(m_pRight);
	if(m_pRight == NULL || pBxRight == NULL) return 0;

	double  staLeft  = pBx->GetStation();
	double  staRight = pBxRight->GetStation();
	CVector xyLeft   = GetXyGirderDis(staLeft);
	CVector xyRight  = m_pRight->GetXyGirderDis(staRight);

	if (nDimType == DIM_CAMBER)  
	{
		xyLeft.z  = GetElevationFlangeCamber(staLeft);
		xyRight.z = m_pRight->GetElevationFlangeCamber(staRight);
	}
	return ~(xyLeft - xyRight);
}


//입력 : 
//기능 : 현재 스테이션에서 D만큼 이격된 거리의 R값을 구함
//		 직선 -> ""
//		 원   -> "R=150000.0"
//		 크로 -> "A=100000.0"
CString CPlateGirderApp::GetRadiusString(double sta, double dis) const
{
	CLineSplit *pSplit = m_pLine->GetLineSplitByStation(sta);
	CString str;

	long nLineType = m_pLine->GetTypeLine(sta);
	double dRadius = 0;
	if(nLineType == 1)		//크로소이드 BTC
	{
		dRadius = pSplit->GetA1();
		str.Format("A1=%.0f",dRadius);
	}
	else if(nLineType == 2)	//원
	{
		dRadius = pSplit->GetRadius_Split();
		if(m_pLine->GetDirectionRotate(sta)==-1) // 시계방향 - = +
			dRadius -= (m_dGirderCenter+dis);
		else 
			dRadius += (m_dGirderCenter+dis);
		str.Format("R=%.0f",dRadius);
	}
	else if(nLineType == 3)	//크로소이드 ETC
	{
		dRadius = pSplit->GetA2();
		str.Format("A2=%.0f",dRadius);
	}
	else					//직선
	{
		str = "";
	}

	return str;
}

//입력 : pBx
//		 nMarkBuje		현재는 상판, 하판, 복부판, 종리브 4종류임
//		 nMarkBujeSec
//		 bSttSplice		현재 Bx가 현장이음이고 구간의 시점일 경우 TRUE, 기본값은 FASLE
//기능 : 현재 Bx에서 [기초설정]에 설정된 용접 치수 기준에 따라 현재 Bx의 필렛 용접 치수를 구함.
//		 [기초설정] - [필렛 용접 치수] : 
//			0=실제 치수 적용, 1=박스별 최대 치수적용, 2=상하 측판별 최대치수, 3=전체 8mm 적용,
//			4=홀수두께 올림(실제 치수), 5=홀수두께 올림(박스별 최대), 6=홀수두께 올림(중조립마크별)
double CPlateGirderApp::GetSizeWeldFillet(CPlateBasicIndex *pBx, long nMarkBuje, long nMarkBujeSec, BOOL bSttSplice) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;	
	CGlobarOption *pGlopt= pDB->m_pDataManage->GetGlobalOption();

	long nIdx = pDB->GetIndexBaseWeld(nMarkBuje,nMarkBujeSec,pBx);
	if(nIdx==-1) nIdx = pDB->GetIndexBaseWeld(nMarkBuje,nMarkBujeSec,pBx);

	BOOL bMainBuje = TRUE;
	if((nMarkBuje    != G_F_U && nMarkBuje    != G_F_L && nMarkBuje    != G_W) ||
	   (nMarkBujeSec != G_F_U && nMarkBujeSec != G_F_L && nMarkBujeSec != G_W))
		bMainBuje = FALSE;

	long nWeldType  = pGlopt->GetDesignDimensionOfFilletWeld();
	if(nWeldType == 3)	return 8;		//3=전체 8mm 적용,

	double dT	 = GetThickJewon(nMarkBuje, pBx);
	double dT2	 = GetThickJewon(nMarkBujeSec, pBx);
//	if(!bMainBuje) dT2 = 0;
	long nApply = pGlopt->GetSteelStandardYear();
	CString strApply = pGlopt->GetStrSteelStandardYear(nApply);
	double dTMax = GetSizeWeldFilletSimple(dT, dT2, strApply);

	if(nWeldType == 0)	return dTMax;					//0=실제 치수 적용
	if(!bMainBuje)		return dTMax;					//실제 치수 적용(기타부재)
	if(nWeldType == 4)	return RoundUp(dTMax/2, 0)*2;	//홀수 두께 올림

	long nSp = GetNumberSpliceByBx(pBx);
	if(bSttSplice == TRUE)		nSp++;

	CPlateBasicIndex *pBxStt = GetBxBySpliceNo(nSp-1);
	CPlateBasicIndex *pBxEnd = GetBxBySpliceNo(nSp);

	__int64 BxFlag = BX_SPLICE|BX_UPPERFLANGE|BX_LOWERFLANGE|BX_WEB;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxMax = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd ,FALSE, FALSE);
	while (pBxMax) 
	{
		if(nWeldType == 2 || nWeldType == 6)
		{
			if(nMarkBuje == G_F_U || nMarkBujeSec == G_F_U)
				dT	= max(GetThickJewon(G_F_U, pBxMax),GetThickJewon(G_W, pBxMax));
			else
				dT	= max(GetThickJewon(G_F_L, pBxMax),GetThickJewon(G_W, pBxMax));
		}
		else
		{
			dT	= max(GetThickJewon(G_F_U, pBxMax),GetThickJewon(G_W, pBxMax));						
		}
		dTMax = max(GetSizeWeldFilletSimple(dT, dT2, strApply), dTMax);
		pBxMax = Finder.GetBxNext();
	}

	if(nWeldType == 5 || nWeldType == 6)		return RoundUp(dTMax/2, 0)*2;	//홀수두께 올림
	return dTMax;	//거더별 최대 치수적용
}


BOOL CPlateGirderApp::IsSectionTapered(CPlateBasicIndex *pBx) const
{
	if(!pBx->IsState(BX_VSTIFF|BX_CROSSBEAM_VBRACING)) return FALSE;

	return GetCrossBxSu(pBx, 0, BX_BENDING) > 0;
}

long CPlateGirderApp::GetQtySectionTapered(CPlateBasicIndex *pBx) const
{
	return GetCrossBxSu(pBx, 0, BX_BENDING);

}


//기능 : 박스의 좌측웨브상단과의 높이차이를 리턴
double CPlateGirderApp::GetHeightDiffBySectionDist(CPlateBasicIndex *pBx, double DistFromLeftEnd, 
											  BOOL bUpper, BOOL bShop) const
{
	double  sta  = pBx->GetStation();
	double  dist = DistFromLeftEnd;

	CDPoint vAng = bShop ? pBx->GetAngle() : CDPoint(0,1);

	double LeftY = bShop ? GetElevationFlangeCamber(sta, 0, vAng, bUpper) :
							GetElevationFlange(sta, 0, vAng, bUpper);
	double distY = bShop ? GetElevationFlangeCamber(sta, dist, vAng, bUpper) :
							GetElevationFlange(sta, dist, vAng, bUpper); 

	return (distY - LeftY);
}


CVector CPlateGirderApp::GetVectorNormalOnFlange(double sta, double D, BOOL bUpper/*=TRUE*/) const
{
	CDPoint vAng(1, GetGiulFlangeUpperLowerCamber(sta, D));
	vAng = vAng.Unit();
	CVector vAxis = m_pLine->GetAngleAzimuth(sta).RotateInv90();
	CVector vNormal(0, 0, 1);
	return vNormal.GetRotateAsAxis(CVector(0, 0, 0), vAxis, vAng);
}

double CPlateGirderApp::GetThickFactByStation(double sta, long nMarkBujeFact, BOOL bNext) const
{
	CPlateFactChain *pFact = GetFactChain(nMarkBujeFact, GetFactNumberByStationBefore(bNext ? sta+1 : sta, nMarkBujeFact));
	if (!pFact)
		return 0.0;
	return pFact->m_dFactChainThick;
}

//<summary>
//슬래브 앵커의 각도를 리턴 
//</summary>
//<param name="dAnchorT">슬래브 앵커의 지름</param>
//<param name="duM">슬래브 앵커가 위치한 곳의 상부 플랜지 너비의 1/2</param>
//<return></return>
double CPlateGirderApp::GetAngleSlabAnchor(double dAnchorT, double duM) const
{
	double dAngDegree = 45;
//	double dLenAnchor = sqrt(pow(200,2)+pow(50,2))*2;
	double dLenAnchor = (200+50)*2;
	double dAngAnchor = atan(50/(200+dAnchorT));

	double dWid = 0; 

	while(1)
	{
		if(dAngDegree==0) break;
		dWid = dLenAnchor*sin(ToRadian(dAngDegree)+dAngAnchor);
		if(dWid<duM*2)
			break;
		else
			dAngDegree -= 5.0;
	}

	return dAngDegree;
}

BOOL CPlateGirderApp::IsExtLineGirder() const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	return (pDB->GetGirder(-1)->GetLine() != GetLine());
}

//<summary>
//기준 선형의 staCR, vAngCR 직선과 거더 중앙이 만나는 점의 거더 스테이션
//</summary>
//<param name="staCR"></param>
//<param name="vAngCR"></param>
//<return></return>
double CPlateGirderApp::GetStationMatchByCRLine(double staCR, CDPoint vAngCR) const
{
	if(!IsExtLineGirder()) return m_pLine->GetStationByScInLine(staCR, m_dGirderCenter, vAngCR);
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CLineInfo  *pLineCR = pDB->GetGirder(-1)->GetLine();

	CDPoint vAngBang = pLineCR->GetAngleAzimuth(staCR);//방위각
	CDPoint xyCR  = pLineCR->GetXyLine(staCR);
	double  staGir = 0;
	if(!GetLine()->GetStationMatchByXyAndAngle(xyCR, vAngBang.Rotate(vAngCR), staGir))	return 0;
	CDPoint xyGir = GetLine()->GetXyLine(staGir);
	CDPoint vAngBangGir = GetLine()->GetAngleAzimuth(staGir);
	CDPoint vAngGir = vAngBangGir.BetweenAngle((xyGir - xyCR).Unit());
	double  sta = GetLine()->GetStationByScInLine(staGir, m_dGirderCenter, vAngGir);

	return sta;
}

//<summary>
//기준 선형의 staCR, vAngCR 직선과 거더 중앙이 만나는 점의 거더 중앙 각도
//</summary>
//<param name="staCR"></param>
//<param name="vAngCR"></param>
//<return></return>
CDPoint CPlateGirderApp::GetAngleMatchByCRLine(double staCR, CDPoint vAngCR) const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CLineInfo  *pLineCR = pDB->GetGirder(-1)->GetLine();

	double sta = GetStationMatchByCRLine(staCR, vAngCR);
	CDPoint vAngBang = GetLine()->GetAngleAzimuth(sta);
	CDPoint vAngBangCR = pLineCR->GetAngleAzimuth(staCR);//방위각
	CDPoint vAng = vAngBang.BetweenAngle(vAngBangCR.Rotate(vAngCR));

	return vAng;  
}
//<summary>
//거더 중심에서 기준선형에 직각으로 선을 내렸을 경우의 거더 중심 각도
//</summary>
//<param name="sta">거더 중심 스테이션</param>
//<return></return>
CDPoint CPlateGirderApp::GetAngleGirderCenterToCenterLine(double sta) const
{
	double staCR = GetStationCenterLine(sta);
	CDPoint vAngCR = CDPoint(0, 1);
	CDPoint vAngGir = GetAngleMatchByCRLine(staCR, vAngCR);
//	if(vAngGir.y < 0)	vAngGir = -vAngGir;

	return vAngGir;
}

// 구간내에 최고 ELEVATION 찾기
double CPlateGirderApp::GetMaxPlanEL(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd) const 
{
	if(!pBxStt) return 0;

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	if(pBxEnd==NULL) 
		pBxEnd=Finder.GetBxLast(BX_END_GIRDER);
	
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	if(pBx==NULL)
		return GetElevationFlangeCamber(pBxStt->GetStation());

	double MaxEL = GetElevationFlangeCamber(pBx->GetStation());
	while(pBx) 
	{
		if(GetElevationFlangeCamber(pBx->GetStation()) > MaxEL)
			MaxEL = GetElevationFlangeCamber(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}

	return MaxEL;
}

double CPlateGirderApp::GetMinPlanEL(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd) const 
{
	if(!pBxStt) return 0;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	if(pBxEnd==NULL) 
		pBxEnd=Finder.GetBxLast(BX_END_GIRDER);
	
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	if(pBx==NULL)
		return GetElevationFlangeCamber(pBxStt->GetStation());

	double MinEL = GetElevationFlangeCamber(pBx->GetStation());
	while(pBx) 
	{
		if(GetElevationFlangeCamber(pBx->GetStation()) < MinEL)
			MinEL = GetElevationFlangeCamber(pBx->GetStation());
		pBx = Finder.GetBxNext();
	}

	return MinEL;
}

//기능 : 현재 Bx 에서 기본선형 까지의 거리, 확폭일 경우 다이아프램, 가로보 실제 길이의 합, 일반타입 수직 거리
double CPlateGirderApp::GetDisCenterLineToGirderDom(CPlateBasicIndex *pBx) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
//	if(!IsExtLineGirder())	return m_dGirderCenter;		//확폭아니면 리턴
	if(!pDB->IsExtLine())	return m_dGirderCenter;		//확폭아니면 리턴 2002.10.18 수정(河)

	CPlateGirderApp *pGir      = NULL;
	CPlateBasicIndex *pBxMatch = NULL;
	double dDisFrSlabLeft = 0;
	double disSlabLeft	  = pDB->GetDisSlabLeftDom(pBx);

	for(long n = 0; n < pDB->GetGirdersu(); n++)
	{
		pGir = pDB->GetGirder(n);
		pBxMatch = pBx->GetBxMatchByCrossBeam(m_pRight);
		if(n < pDB->GetGirdersu()-1 && pBxMatch == NULL)	return 0;//마지막 거더가 아니고 매치 인덱스가 없으면,
		if(n == 0)				 dDisFrSlabLeft += pDB->m_dWidthSlabLeft;

		if(pBx == pBxMatch)
		{
			dDisFrSlabLeft += 0;
			break;
		}
		dDisFrSlabLeft += pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
		dDisFrSlabLeft += pGir->GetDisCRSttToCREndReal(pBxMatch);
	}

	return disSlabLeft + dDisFrSlabLeft;
}



//입력 : pBx	현재 거더 pBx
//기능 : 상판 하면 기준 도로중심과의 높이차(종단 ele 는 포함하지 않음)
double CPlateGirderApp::GetElevationDom(CPlateBasicIndex *pBx) const
{	
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;
	CPlateGirderApp* pGir = (CPlateGirderApp*)this;
	double lDis = m_dGirderCenter - pGir->GetLengthSharpOnBx(pBx, TRUE) - pDB->m_dHunchDisForBaseThick;
	double rDis = m_dGirderCenter + pGir->GetLengthSharpOnBx(pBx, TRUE) + pDB->m_dHunchDisForBaseThick;
	double lEle = pDB->GetHeightSlabFromRoadCenterDom(pBx, lDis);
	double rEle = pDB->GetHeightSlabFromRoadCenterDom(pBx, rDis);
	double minEle = min(lEle, rEle);

	return minEle - (pDB->m_dThickSlabBase+pDB->m_dThickPave);
}

//입력 : pBx	현재 거더 pBx
//기능 : 거더높이기준설정에 의한 거더 높이 (내/외측 구분함, 단부 헌치 고려됨)
double CPlateGirderApp::GetElevationDomForExtLine(CPlateBasicIndex *pBx) const
{
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;

	double dEle = GetElevationDom(pBx);

	if(pBx->IsDanbuJijumStt())		dEle -= pDB->m_dHeightTaperStt;
	else if(pBx->IsDanbuJijumEnd())	dEle -= pDB->m_dHeightTaperEnd;

	return dEle - (pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDeep-pDB->m_BindConc.m_dDis);
}

// 목적 : 각 거더 좌.우측 돌출부 끝단에서 W(50) 만큼 이격 거리에서 슬래브면 까지의 높이
// 입력 : nIdx  
//        bLeft  - 구하려는 거더의 좌측(TRUE), 우측 (FALSE)
// 결과 : 높이
double CPlateGirderApp::GetHunchXyHeightDom(CPlateBasicIndex *pBx, BOOL bLeft) const
{
	if(bLeft)		return GetHunchXyHeightNodeDom(pBx, 0);
	else			return GetHunchXyHeightNodeDom(pBx, 9);
}

//목적 : 거더부 절점과 슬래브면과의 높이차 구하기 
//입력 :	nIdx
//			nNode		구하려는 높이의 절점(0 - 25)	
//기능 : 높이차 = A[0].y - A[4].y
double CPlateGirderApp::GetHunchXyHeightNodeDom(CPlateBasicIndex *pBx, long nNode) const
{
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;
	CDPoint vAng = CDPoint(0, 1);	//90도
	CDPoint xyResult;		
	CDPoint A[10];
	CDPoint B[HDANLINESU];
	pDB->GetXyPaveUpperDom(pBx, B);
	GetHunchXyDom(pBx, A);
	
	for(long n=0; n<pDB->GetQtyHDanNode(); n++)
		if(GetXyMatchSegAndLine(B[n], B[n+1], A[nNode], vAng, xyResult)) break;

	return xyResult.y - A[nNode].y;
}


double CPlateGirderApp::GetWidthFlangeUpper(CPlateBasicIndex *pBx) const
{
	return GetWidthOnStation(pBx->GetStation(), TRUE);
}

double CPlateGirderApp::GetWidthFlangeLower(CPlateBasicIndex *pBx) const
{
	return GetWidthOnStation(pBx->GetStation(), FALSE);
}

double CPlateGirderApp::GetGiulWebSplice(CPlateBasicIndex *pBx, BOOL bUpper)
{
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)m_pBridgeApp;
	CGlobarOption   *pGlo = pDB->m_pDataManage->GetGlobalOption();

	double JongSlop = GetGiulWebCamber(pBx->GetStation());
	double BottSlop = GetGiulWebCamber(pBx->GetStation(),0,FALSE);
	BOOL   bByunDan = IsByunByStation(pBx->GetStation());

	double MinSlopJong = pGlo->GetDesignBoltJongVSlopApplyUp()/100;
	double MinSlopBott = pGlo->GetDesignBoltJongVSlopApplyDn()/100;
//	long   BoltArrange = pBet->GetWebSpliceBoltArrange();

	if(fabs(JongSlop) < MinSlopJong)	
		JongSlop = 0.0;
	if(fabs(BottSlop) < MinSlopBott)
		BottSlop = 0.0;

	long dSlopRoundPos = pGlo->GetDesignByundanVSlopRoundPos(); //반올림 자리수

	JongSlop = Round(JongSlop, dSlopRoundPos);
	BottSlop = Round(BottSlop, dSlopRoundPos);

	double dGiul = bUpper ? JongSlop : BottSlop;
	if(bByunDan && !bUpper && dGiul==0)
		dGiul = GetGiulByunDan(pBx);

	return dGiul;
}

// 0 : 비연결재 - 하나로만 구성된 판
// 1 : 끝단재   - 두개이상으로 구성된 판의 끝에 위치한 판
// 2 : 중간재   - 세개이상으로 구성된 판의 중간에 위치한 판
long CPlateGirderApp::GetFactConnectType(CPlateBasicIndex *pBx, long nMarkBuje) const
{
	long nSpEnd = GetNumberSpliceByBx(pBx);
	if(pBx->IsState(BX_SPLICE)) nSpEnd++;
	long nSpStt = nSpEnd-1;

	CPlateBasicIndex *pBxStt = GetBxBySpliceNo(nSpStt);
	CPlateBasicIndex *pBxEnd = GetBxBySpliceNo(nSpEnd);

	__int64 BxFlag = BX_STT_GIRDER|BX_SPLICE|BX_END_GIRDER;
	if(nMarkBuje==G_F_U) BxFlag |= BX_UPPERFLANGE;
	if(nMarkBuje==G_F_L) BxFlag |= BX_LOWERFLANGE;
	if(nMarkBuje==G_W)   BxFlag |= BX_WEB;	

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBxCount = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd,TRUE,FALSE);

	long nTotal   = 0;
	long nCurrent = 0;

	while(pBxCount)
	{
		if(pBxCount==pBx)
			nCurrent = nTotal;
		nTotal++;
		pBxCount = Finder.GetBxNext();
	}

	if(nCurrent==0 && nTotal==1) return 0;
	if(nCurrent==0 || nCurrent==nTotal-1) return 1;

	return 2;
}

// 현재 BX의 지점보강재의 열수
long CPlateGirderApp::GetQtyRowJijumVStiffByBx(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;

	long nJijum = GetNumberJijumByBx(pBx);
	long nRow   = 0;
	for(long n=0; n<JACKUPSUMAX; n++)
	{
		if(pDB->m_dJiJumTerm[nJijum][n] != 0)
			nRow++;
	}

	return nRow;
}

// 현재 BX 지점의 지점보강재의 수
long CPlateGirderApp::GetQtyJijumVStiffByBx(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp * pDB = (CPlateBridgeApp *)m_pBridgeApp;

	long nJijum = GetNumberJijumByBx(pBx);
	return pDB->GetQtyJijumStiff(nJijum);
}


// 현재 BX의 잭업보강재의 열수
long CPlateGirderApp::GetQtyRowJackupByBx(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp *)m_pBridgeApp;

	long nJijum = GetNumberJijumByBx(pBx);
	long nRow   = 0;
	for(long n=0; n<JACKUPSUMAX; n++)
	{
		if(pDB->m_dJackUpTerm[nJijum][n] != 0)
			nRow++;
	}

	return nRow;
}

//  현재 BX의 속하는 잭업보강재 수를 구함
long CPlateGirderApp::GetQtyJackupByBx(CPlateBasicIndex *pBx, long nDir) const
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pBridgeApp;
	CPlateGirderApp *pGir= pDB->GetGirder(pBx->GetNumberGirder()); 

	long nJijum  = GetNumberJijumByBx(pBx);
	long nTotalSum = 0;
	for(long n=0; n<JACKUPSUMAX; n++)
	{
		if(pDB->m_dJackUpTerm[nJijum][n] == 0)
			break;

		CJackupStiff *pJackUp = pGir->GetJackupByJijum(nJijum, n);
		if(pJackUp->m_vJewon[0].x>0)	nTotalSum++;
		if(pJackUp->m_vJewon[1].x>0)	nTotalSum++;
	}
	return nTotalSum;
}

// 현재거더에서 횡단의 모든 가로보bx가 존재하는 BX(단지점부는 제외)
CPlateBasicIndex* CPlateGirderApp::GetBxAllSafeHDan()
{
	CPlateBridgeApp *pBridge = (CPlateBridgeApp*)m_pBridgeApp;	
	CPlateGirderApp	*pGir	 = (CPlateGirderApp*)this;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		BOOL bSafeHDan = TRUE;
		if(pBx->IsJijum()==FALSE)
		{
			for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
			{
				CPlateGirderApp *pGirMatch = pBridge->GetGirder(nG);	
				CPlateBasicIndex *pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGirMatch);

				if(pBxMatch==NULL)	
					bSafeHDan = FALSE;
			}
			if(bSafeHDan)
				return pBx;
		}
		pBx = Finder.GetBxNext();
	}
	return NULL;
}



CVStiff* CPlateGirderApp::GetVStiffByPos(long nPos)
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
	pBx = (CPlateBasicIndex *)Finder.GetBxNext(nPos);

	CSection *pSec = pBx->GetSection();
	ASSERT(pSec);					
	CVStiff *pVStiff = pSec->GetVStiff();

	if(pVStiff == NULL)	return NULL;

	return pVStiff;
}


CPlateBasicIndex *CPlateGirderApp::GetBxMaxFlangeWidth()
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	__int64 BxFlag = BX_UPPERFLANGE|BX_STT_GIRDER|BX_END_GIRDER;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BxFlag);
	double dMaxWidth = 0;
	while(pBx)
	{
		double dWFlange = pGir->GetWidthFlangeUpper(pBx);
		if(dMaxWidth < dWFlange)
		{
			dMaxWidth = dWFlange;
			return pBx;
		}
		pBx = Finder.GetBxNext();
	}

	return NULL;
}


CPlateBasicIndex *CPlateGirderApp::GetBxMinFlangeWidth()
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	__int64 BxFlag = BX_UPPERFLANGE|BX_STT_GIRDER|BX_END_GIRDER;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BxFlag);
	double dMinWidth = 10000;
	while(pBx)
	{
		double dWFlange = pGir->GetWidthFlangeUpper(pBx);
		if(dMinWidth > dWFlange)
		{
			dMinWidth = dWFlange;
			return pBx;
		}
		pBx = Finder.GetBxNext();
	}

	return NULL;
}


double CPlateGirderApp::GetMaxWidthFlange(BOOL bUpper) const
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	__int64 BxFlag = bUpper ? BX_UPPERFLANGE|BX_STT_GIRDER|BX_END_GIRDER : BX_LOWERFLANGE|BX_STT_GIRDER|BX_END_GIRDER;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BxFlag);
	double dMaxWidth = 0;
	while(pBx)
	{
		double dWFlange = bUpper ? pGir->GetWidthFlangeUpper(pBx) : pGir->GetWidthFlangeLower(pBx);
		if(dMaxWidth < dWFlange)
			dMaxWidth = dWFlange;

		pBx = Finder.GetBxNext();
	}

	return dMaxWidth;
}

double CPlateGirderApp::GetMinWidthFlange(BOOL bUpper) const
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	__int64 BxFlag = bUpper ? BX_UPPERFLANGE|BX_STT_GIRDER|BX_END_GIRDER : BX_LOWERFLANGE|BX_STT_GIRDER|BX_END_GIRDER;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BxFlag);
	double dMinWidth = 10000;
	while(pBx)
	{
		double dWFlange = bUpper ? pGir->GetWidthFlangeUpper(pBx) : pGir->GetWidthFlangeLower(pBx);
		if(dMinWidth > dWFlange)
			dMinWidth = dWFlange;

		pBx = Finder.GetBxNext();
	}

	return dMinWidth;
}
void CPlateGirderApp::MakeFactChain(long nMake,long nMarkBujeFact)
{
	switch( nMarkBujeFact )
	{
	case G_F_U : MakeFactChainUpp(nMake); break;
	case G_F_L : MakeFactChainLow(nMake); break;
	case G_W   : MakeFactChainWeb(nMake); break;	
	}
}
/*
void CPlateGirderApp::MakeFactChainUpp(long nMake)
{
	AhTPAMake(nMake,m_aFactChain,(CPlateFactChain*)0);
	SetLKGirderPointAndSelfCount(m_aFactChain);
	SetLKSideFactChain();
}

void CPlateGirderApp::MakeFactChainLow(long nMake)
{
	AhTPAMake(nMake,m_aFactChain+1,(CPlateFactChain*)0);
	SetLKGirderPointAndSelfCount(m_aFactChain+1);
	SetLKSideFactChain();
}

void CPlateGirderApp::MakeFactChainWeb(long nMake)
{
	AhTPAMake(nMake,m_aFactChain+2,(CPlateFactChain*)0);
	SetLKGirderPointAndSelfCount(m_aFactChain+2);
	SetLKSideFactChain();
}
*/
void CPlateGirderApp::MakeSplice(long nMake)
{
	AhTPAMake(nMake,&m_pSplice,(CPlateSplice*)0);
	SetLKGirderPointAndSelfCount(&m_pSplice);
	SetEnTypeNumSplice();
}

void CPlateGirderApp::MakeJackupStiff(long nMake)
{
	AhTPAMake(nMake,&m_aJackupArr,(CJackupStiff*)0);
	SetLKGirderPointAndSelfCount(&m_aJackupArr);	
}

void CPlateGirderApp::MakeWebStiff(long nMake)
{	
	AhTPAMake(nMake,&m_aWebHStiffArr,(CWebHStiff*)0);
	SetLKGirderPointAndSelfCount(&m_aWebHStiffArr);	
}

void CPlateGirderApp::MakeCamber(long nMake)
{
	AhTPAMake(nMake,&m_pCamber,(CCamber*)0);

}

long CPlateGirderApp::GetQtyFactChain(long nMarkBujeFact) const
{
	if(nMarkBujeFact < 0 || nMarkBujeFact > 3)	return 0;
	return m_aFactChain[nMarkBujeFact].GetSize();

}

long CPlateGirderApp::GetQtySplice(long nSpType) const
{
	long nSplice=0;
	long nWeld  =0;

	for(long n=0; n<m_pSplice.GetSize(); n++)
	{
		CPlateSplice *pSp = m_pSplice.GetAt(n);
		if(pSp->m_bSpliceType)
			nSplice++;
		else
			nWeld++;
	}

	if(nSpType==CPlateGirder::SPLICE)
		return nSplice;
	else if(nSpType==CPlateGirder::WELD)
		return nWeld;
	else
		return nSplice+nWeld;

}

CPlateFactChain* CPlateGirderApp::GetFactChain(long nMarkBujeFact, long nCount) const
{
	if(nCount < 0 || nCount >= GetQtyFactChain(nMarkBujeFact)) return NULL;
	if(nMarkBujeFact < 0 || nMarkBujeFact > 3)	return NULL;
	return m_aFactChain[nMarkBujeFact][nCount];
}

// 상부 플래지의 두께가 가장 큰 위치의 공장이음 시점에서 교량진행방향으로 가장 가까운 가로보위치(nDir=1)
CPlateBasicIndex* CPlateGirderApp::GetCrossBxAtMaxFlangeWidth()
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	double dMaxWFlange = pGir->GetMaxWidthFlange(TRUE);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_UPPERFLANGE);
	if(pBx==NULL)
		return pGir->GetBxOnJijum(0);

	while(pBx)
	{
		double dWFlange = pGir->GetWidthFlangeUpper(pBx);
		if(dWFlange==dMaxWFlange)
			break;

		pBx = Finder.GetBxNext();
	}
	if(pBx)
		pBx = pGir->GetBxByStationDir(pBx->GetStation(), 1, BX_CROSSBEAM_VBRACING);

	return pBx;
}

// 상부 플래지의 두께가 가장 큰 위치의 공장이음 시점에서 교량진행방향으로 가장 가까운 가로보위치(nDir=1)
CPlateBasicIndex* CPlateGirderApp::GetCrossBxAtMinFlangeWidth()
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	double dMinWFlange = pGir->GetMinWidthFlange(TRUE);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_UPPERFLANGE);
	if(pBx==NULL)
		return pGir->GetBxOnJijum(0);
	while(pBx)
	{
		double dWFlange = pGir->GetWidthFlangeUpper(pBx);
		if(dWFlange==dMinWFlange)
			break;

		pBx = Finder.GetBxNext();
	}
	if(pBx==NULL)
		return pGir->GetBxOnJijum(0);
	pBx = pGir->GetBxByStationDir(pBx->GetStation(), 1, BX_CROSSBEAM_VBRACING);

	return pBx;
}

long CPlateGirderApp::GetVBracingsu()
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	long nCount = 0;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
			nCount++;
			
		pBx = Finder.GetBxNext();
	}

	return nCount;
}

long CPlateGirderApp::GetCrossBeamsu()
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	long nCount = 0;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			nCount++;
			
		pBx = Finder.GetBxNext();
	}

	return nCount;

}

double CPlateGirderApp::GetValueBasicDefault(CString szName)
{
	CPlateBridgeApp *pDB    = (CPlateBridgeApp *)m_pBridgeApp;
	CGlobarOption   *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	double dLKyunggan = pDB->m_dLengthTotalSlab/pDB->m_nQtyJigan;
	double dWeb_H = 0;
	double dWeb_T = 0;
	double dFl_W = 0;
	double dFl_T = 0;
	
/*	if(pGlopt->GetDesignBridgeType()==0)	// H 형강 교량
	{
		dWeb_H = Round(dLKyunggan/22, -2);
		dWeb_T = Round(dWeb_H/310, 0);
		dFl_W  = Round(dWeb_H/5, -2);
		dFl_T  = Round((dFl_W-dWeb_T)/32, 0);
	}
	else */if(pGlopt->GetDesignBridgeType()==PLATE_GIR)	// 판형교
	{
		dWeb_H = Round(dLKyunggan/22, -2);
		dWeb_T = Round(dWeb_H/310, 0);
		dFl_W  = Round(dWeb_H/5, -2);
		dFl_T  = Round((dFl_W-dWeb_T)/32, 0);
	}
	else if(pGlopt->GetDesignBridgeType()==SOSU_GIR)	// 소수주형
	{
		dWeb_H = Round(dLKyunggan/18, -2);
		dWeb_T = Round(dWeb_H/123, 0);
		dFl_W  = Round(dWeb_H/4, -2);
		dFl_T  = Round((dFl_W-dWeb_T)/32, 0);
	}
	else if(pGlopt->GetDesignBridgeType()==TU_GIR)		// TU거더
	{
		double dBindH  = pDB->m_BindConc.m_dHeight;
		double dBindS1 = pDB->m_BindConc.m_dDeep;
		double dBindS2 = pDB->m_BindConc.m_dDis;
		dWeb_H = Round(dLKyunggan/18, -2) - (dBindH-dBindS1-dBindS2);
		dWeb_T = Round(dWeb_H/123, 0);
		dFl_W  = Round(dWeb_H/4, -2);
		dFl_T  = Round((dFl_W-dWeb_T)/32, 0);
	}


	if(szName=="WEB_H")			return dWeb_H;
	else if(szName=="WEB_T")	return dWeb_T;
	else if(szName=="FLANGE_W")	return dFl_W;
	else if(szName=="FLANGE_T")	return dFl_T;

	return 0;
}

double CPlateGirderApp::GetHeightTaper(double sta, double D, CDPoint vAng) const
{
	CPlateBridgeApp *pDB  = (CPlateBridgeApp *)m_pBridgeApp;
	if(pDB->m_dHeightTaperStt == 0 && pDB->m_dHeightTaperEnd == 0)	return 0;

	double staDis = GetStationBySc(sta, D, vAng);
	double staSttEnd = GetStationBySc(GetSSS(OFF_STT_HUNCHEND), D, GetAAA(OFF_STT_HUNCHEND));//시점부 높은쪽
	double staEndEnd = GetStationBySc(GetSSS(OFF_END_HUNCHEND), D, GetAAA(OFF_END_HUNCHEND));//종점부 높은쪽
	if(staDis >= staSttEnd && staDis <= staEndEnd)	return 0;

	double staSttStt = GetStationBySc(GetSSS(OFF_STT_HUNCHSTT), D, GetAAA(OFF_STT_HUNCHSTT));//시점부 낮은쪽
	if(staDis <= staSttStt)							return pDB->m_dHeightTaperStt;
	if(staDis > staSttStt && staDis < staSttEnd)	return (staSttEnd-staDis) * pDB->m_dHeightTaperStt /(staSttEnd-staSttStt);

	double staEndStt = GetStationBySc(GetSSS(OFF_END_HUNCHSTT), D, GetAAA(OFF_END_HUNCHSTT));//종점부 낮은쪽
	if(staDis >= staEndStt)					return pDB->m_dHeightTaperEnd;
	if(staDis > staEndEnd && staDis < staEndStt)	return (staDis-staEndEnd) * pDB->m_dHeightTaperEnd /(staEndStt-staEndEnd);

	return 0;
}

// bReal : TRUE  복부판의 길이
//         FALSE 거더높이기준 설정에 따른 길이 (횡단구성 입력값)
double CPlateGirderApp::GetHeightGirderByJijum(long nJ, BOOL bReal) const
{
	CPlateGirderApp	*pGir = (CPlateGirderApp *)this;
	CPlateBasicIndex *pBx = GetBxOnJijum(nJ);
	double dUpperThick = 0;
	double dLowerThick = 0;

	if(pBx && pBx->GetFactChain(G_F_U))
		dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? GetThickJewon(G_F_U,pBx) : 0;
	else
		dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? pGir->GetValueBasicDefault("FLANGE_T") : 0;
	
	if(pBx && pBx->GetFactChain(G_F_L))
		dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? GetThickJewon(G_F_L,pBx) : 0;
	else
		dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? pGir->GetValueBasicDefault("FLANGE_T") : 0;
	
	return (m_dHeightOnJijum[nJ] - (bReal ? dUpperThick + dLowerThick : 0));
}

double CPlateGirderApp::GetHeightGirder(CPlateBasicIndex *pBx, BOOL bReal) const
{
	return GetHeightGirderByStaAng(pBx->GetStation(), 0, CDPoint(0,1), bReal);
}

double CPlateGirderApp::GetThickFlangeByBase(long nJ, BOOL bUpper) const
{
	CPlateBasicIndex *pBx = GetBxOnJijum(nJ);
	double dUpperThick = 0;
	double dLowerThick = 0;

	if(pBx && pBx->GetFactChain(G_F_U))
		dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? GetThickJewon(G_F_U,pBx) : 0;
	
	if(pBx && pBx->GetFactChain(G_F_L))
		dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? GetThickJewon(G_F_L,pBx) : 0;
	
	return bUpper ? dUpperThick : dLowerThick;
}

double CPlateGirderApp::GetHeightGirderBySta(double dSta, BOOL bReal) const
{
	double dUpperThick = 0;
	double dLowerThick = 0;

	long nJ = GetNumberJijumByStation(dSta);
	
	CPlateBasicIndex *pBxUpper = GetBxByStationDir(dSta, -1, BX_UPPERFLANGE);
	CPlateBasicIndex *pBxLower = GetBxByStationDir(dSta, -1, BX_LOWERFLANGE);

	CPlateGirderApp *pGir = (CPlateGirderApp *)this;
	CPlateBxFinder Finder(pGir);
	if(!pBxUpper) pBxUpper = Finder.GetBxFirst(BX_STT_GIRDER);
	if(!pBxLower) pBxLower = Finder.GetBxFirst(BX_STT_GIRDER);

	if(pBxUpper && pBxUpper->GetFactChain(G_F_U))
		dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? GetThickJewon(G_F_U,pBxUpper) : 0;
	
	if(pBxLower && pBxLower->GetFactChain(G_F_L))
		dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? GetThickJewon(G_F_L,pBxLower) : 0;
	
	return (m_dHeightOnJijum[nJ] - (bReal ? dUpperThick + dLowerThick : 0));	
}

double CPlateGirderApp::GetThickFlangeByBase(double dSta, BOOL bUpper) const
{
	double dUpperThick = 0;
	double dLowerThick = 0;
	
	CPlateBasicIndex *pBxUpper = GetBxByStationDir(dSta, -1, BX_UPPERFLANGE);
	CPlateBasicIndex *pBxLower = GetBxByStationDir(dSta, -1, BX_LOWERFLANGE);

	CPlateGirderApp *pGir = (CPlateGirderApp *)this;
	CPlateBxFinder Finder(pGir);
	if(!pBxUpper) pBxUpper = Finder.GetBxFirst(BX_STT_GIRDER);
	if(!pBxLower) pBxLower = Finder.GetBxFirst(BX_STT_GIRDER);

	if(pBxUpper && pBxUpper->GetFactChain(G_F_U))
		dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? GetThickJewon(G_F_U,pBxUpper) : 0;
	
	if(pBxLower && pBxLower->GetFactChain(G_F_L))
		dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? GetThickJewon(G_F_L,pBxLower) : 0;
	
	return bUpper ? dUpperThick : dLowerThick;
}

void CPlateGirderApp::SetHeightGirderAtJijum(long nJ, double dH)
{	
	m_dHeightOnJijum[nJ] = dH;
}

double CPlateGirderApp::GetThickSlab(CPlateBasicIndex *pBx, double dDist)
{
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBridgeApp;
	double	dThickSlab		= 0;
	double	dThickSlabPave	= GetThickSlabPave(pBx->GetStation(), dDist);
	double	dThickPave		= pDB->GetThickPave();
	if(pDB->m_BindConc.m_dWidth/2 <= fabs(dDist))
		dThickSlab = dThickSlabPave - dThickPave;
	else
		dThickSlab = dThickSlabPave - dThickPave - pDB->m_BindConc.m_dDeep;

	return dThickSlab;
}

//<summary>
//복부판 내측과 만나는 가로보 중심선의 좌우 좌표를 구함
//</summary>
//<param name="*pBx"></param>
//<param name="bLeft">TRUE = 가로보 기준 좌측(좌측 박스의 우측 복부판), FALSE = 가로보 기준 좌측(좌측 박스의 우측 복부판)</param>
//<return></return>
CDPoint CPlateGirderApp::GetXyPlanSlabCrossbeamBase(CPlateBasicIndex *pBx, BOOL bLeft) const
{
	CSection		*pSec = pBx->GetSection();
	if(!pSec) return CDPoint(0, 0);

	CDPoint xy;
	if(bLeft)
	{
		CPlateGirderApp* pGir = pBx->GetGirder();
		double dSta	 = pBx->GetStation();
		CDPoint vAng = pBx->GetAngle();
		xy	 = pGir->GetXyGirderDis(dSta, pGir->GetWidthOnStation(pBx->GetStation(), FALSE), vAng);
	}
	else
	{
		CPlateGirderApp* pGir = pBx->GetGirder()->m_pRight;
		if(pGir==NULL)	return CDPoint(0,0);
		
		double dSta	 = pBx->GetStation();
		CDPoint vAng = pBx->GetAngle();
		xy	 = pGir->GetXyGirderDis(dSta, -(pGir->GetWidthOnStation(pBx->GetStation(), TRUE)), vAng);
	}

	return xy;
}


CSection* CPlateGirderApp::GetSectionCRandVBByNo(long nSec) const
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;

	if(nSec < 0 || nSec >= pGir->GetCrossBeamsu()) return NULL;
	
	return m_CrossSectionArr[nSec];
}



BOOL CPlateGirderApp::IsCrossBeamBxHasHunch(CPlateBasicIndex *pBx)
{
	if(pBx==NULL)	return FALSE;
	
	if(!m_pBridgeApp->m_bApplyHunchAtCB)	return FALSE;

	CSection *pSec = pBx->GetSection();
	if(pSec==NULL)	return FALSE;

	if(!pSec->IsenDetType(SECDET_CROSSBEAM))	return FALSE;

	if(GetHeightTaper(pBx->GetStation()) > 0)	return FALSE;
	
	CPlateCrossBeam *pC = pSec->GetCrossBeam();
	if(pC->m_dLH1==0 || pC->m_dRH1==0)
		return TRUE;	
	else
		return FALSE;	
}



CSection* CPlateGirderApp::GetSectionBySectionNo(long nSec) const
{
	if(nSec < 0 || nSec >= GetQtyCrossBeamVBracing()) return NULL;
	return m_CrossSectionArr[nSec];
}


//입력 : nTarget	OFF_STT_SLAB		슬래브 시종점		항상 옵셋	
//					OFF_STT_GIRDER		거더 시종점			옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_SHOE		슈 시종점			옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_HUNCHSTT	시작헌치 시종점		옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//					OFF_STT_HUNCHEND	종점헌치 시종점		옵셋 변수 받음 pDB->m_cOffSetStt,pDB->m_cOffSetEnd
//모든거더의 기준은 도로 중심에 대한 실제 스테이션(확폭이 아닌 모든 거더가 같음.)
//GetStationStt()을 "0" 로 한 스테이션이 아님.******     
BOOL CPlateGirderApp::IsExistTaperArea(CPlateBasicIndex *pBx, double dDist) const
{
	double staStt =  GetStationBySc(GetSSS(OFF_STT_HUNCHEND), dDist, GetAAA(OFF_STT_HUNCHEND));
	double staEnd =  GetStationBySc(GetSSS(OFF_END_HUNCHEND), dDist, GetAAA(OFF_END_HUNCHEND));
	double sta = GetStationBySc(pBx->GetStation(), dDist, pBx->GetAngle());
	if(sta < staStt || sta > staEnd)
		return TRUE;

	return FALSE;
}

long CPlateGirderApp::GetSizeFactChain(long nMarkBujeFact)
{
	long	nSize	= 0;
	if(m_aFactChain[nMarkBujeFact].GetSize())
		nSize = m_aFactChain[nMarkBujeFact].GetSize();

	return nSize;
}

long CPlateGirderApp::GetNumberJijumMinGirderEL() const 
{	
	double sta = 0;
	long nJMin = 0;
	double eleJijum = 0;
	double eleMin = 100000000;
	CPlateBridgeApp *pDB  = (CPlateBridgeApp *)m_pBridgeApp;
	for(long nJ=0; nJ <= pDB->m_nQtyJigan; nJ++)
	{
		sta = GetStationOnJijum(nJ);
		eleJijum = GetElevationFlange(sta);
		if(eleJijum < eleMin)
		{
			nJMin = nJ;
			eleMin = eleJijum;
		}
	}
	
	return nJMin;
}

// TRUE : 현장이음위치 에서 설정한 교번부-교번부 사이의 부모멘트부
BOOL CPlateGirderApp::IsShiftSection(CPlateBasicIndex *pBx, BOOL bNext)
{
	return IsShiftSection(pBx->GetStation(), bNext);
}

BOOL CPlateGirderApp::IsShiftSection(double dSta, BOOL bNext)
{
	CPlateBridgeApp *pDB  = (CPlateBridgeApp *)m_pBridgeApp;
	CPlateBxFinder Finder((CPlateGirderApp*)this);

	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_SPLICE);
	CPlateBasicIndex *pBxEnd = NULL;

	if(!pDB->IsTUContinuousSpan())
		return FALSE;

	while(pBxStt)
	{
		while(pBxStt && !pBxStt->IsShift())
			pBxStt = Finder.GetBxNext();
		
		pBxEnd = Finder.GetBxNext();
		while(pBxEnd && !pBxEnd->IsShift())
			pBxEnd = Finder.GetBxNext();

		// !!교번부 지정 갯수가 짝수가 아니어도 FALSE 리턴함!! - 입력오류
		if(!pBxStt)	return FALSE;
		if(!pBxEnd)	return FALSE;

		if(bNext	&& pBxStt->GetStation() <=	dSta && pBxEnd->GetStation() >	dSta)	return TRUE;
		if(!bNext	&& pBxStt->GetStation() <	dSta && pBxEnd->GetStation() >=	dSta)	return TRUE;

		pBxStt = Finder.GetBxNext();
	}

	return FALSE;
}

// return : pBx가 속한 정모멘트부의 구간길이, dLenCalc : 정모멘트 구간내에서 모멘트 계산길이
double CPlateGirderApp::GetLengthPositiveSection(double dSta, double &dLenCalc)
{
	dLenCalc = 0;
	if (IsShiftSection(dSta))	return 0;	// 부모멘트부일때 이곳에 들어오는것 부터가 문제임

	double dLenSection = 0;
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	
	CPlateBasicIndex *pBxCrStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxCrEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxSp = Finder.GetBxFirst(BX_SPLICE);

	CDoubleArray dArrSecSta;
	dArrSecSta.Add(pBxCrStt->GetStation());
	while(pBxSp)
	{
		if(pBxSp->IsShift())
			dArrSecSta.Add(pBxSp->GetStation());
		pBxSp = Finder.GetBxNext();
	}
	dArrSecSta.Add(pBxCrEnd->GetStation());

	dArrSecSta.Sort();

	for(long nSec=0; nSec<dArrSecSta.GetSize()-1; nSec++)
	{
		double dStaStt = dArrSecSta.GetAt(nSec);
		double dStaEnd = dArrSecSta.GetAt(nSec+1);

		if(dStaStt <= dSta && dStaEnd >= dSta)
		{
			dLenSection = dStaEnd-dStaStt;
			dLenCalc	= dSta-dStaStt;
			return dLenSection;
		}
	}

	return 0;
}

long CPlateGirderApp::GetQtyJoint()	const
{
	long nQtyCr = GetQtyCrossBeamVBracing();
	long nQtySp = 0;

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	while (pBx)
	{
		if(pBx->IsShift())	nQtySp++;
		pBx = Finder.GetBxNext();
	}

	return nQtyCr+nQtySp;
}
