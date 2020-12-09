// APlateDrawHBracing.cpp: implementation of the CAPlateDrawHBracing class.
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

CAPlateDrawHBracing::CAPlateDrawHBracing(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawHBracing::~CAPlateDrawHBracing()
{

}

BOOL CAPlateDrawHBracing::GetXyMatchSegAndSegNoChangeRes(CDPoint xyStt1, CDPoint xyEnd1, CDPoint xyStt2, CDPoint xyEnd2, CDPoint& xyResult)
{
	CDPoint xyTemp(xyResult);
	BOOL bResult = GetXyMatchSegAndSeg(xyStt1, xyEnd1, xyStt2, xyEnd2, xyResult);
	if(!bResult)
	{
		xyResult = xyTemp;
		return FALSE;
	}

	return TRUE;
}

//nPos=수평브레이싱이음판번호
void CAPlateDrawHBracing::DrawInputHBracingBySangseType(CDomyun *pDom,  CPlateGirderApp *pGir, long nPos, BOOL bSangseDom, BOOL bJewon, BOOL bInput, BOOL bCut)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CHBracingGusset *pBrGu   = pGir->GetHBracingGussetInTotal(nPos);
	CAPlateDrawPyung DrawPyung(m_pDataMng);

	if(nPos>=pGir->GetQtyHGusset())	return;

	//******************************************************************************//
	CPlateBasicIndex *pBxGu = pBrGu->GetBx(); // 현재 횡브레이싱이음판의 BX
	long nG      = pBxGu->GetNumberGirder();
	long nGTotal = pBridge->GetGirdersu();
	CDRect	rect;

	// 회전각
	CDPoint vAngle;

	// 현재 pBxGu를 포함하는 브레이싱의 가로보위치Bx(기본적으로 현재 pBx의 우측을 생성함)
	// 가로보와 수직브레이싱 생성시 사용함
	CPlateBasicIndex *pBx		= pBxGu->GetBxCrossBeamByHGuBx(FALSE, TRUE);

	// 새로운 거더 및 데이터 정리
	vector<CPlateGirderApp*> GirderArray;
	vector<CHBracingGusset*> GussetArray;
	vector<CPlateBasicIndex*> IndexGuArray;
	vector<CPlateBasicIndex*> IndexBxArray;

	long nPos0 = (nPos%2==0)? nPos-2 : nPos-1;
	long nPos1 = (nPos%2==0)? nPos-1 : nPos-0;
	long nPos2 = (nPos%2==0)? nPos-0 : nPos+1;
	
	while (nPos0<0)
	{
		nPos0++;
		nPos1++;
		nPos2++;
	}
	long n = 0;
	for (n=0; n<nGTotal; n++)
	{
		CPlateGirderApp* pGirder = new CPlateGirderApp();
		memcpy(pGirder, pBridge->GetGirder(n), sizeof(CPlateGirderApp));
		GirderArray.push_back(pGirder);

		CHBracingGusset* pGusset0 = new CHBracingGusset();
		CHBracingGusset* pGusset1 = new CHBracingGusset();
		CHBracingGusset* pGusset2 = new CHBracingGusset();
		CHBracingGusset* pHGusset0 = pGirder->GetHBracingGussetInTotal(nPos0);
		CHBracingGusset* pHGusset1 = pGirder->GetHBracingGussetInTotal(nPos1);
		CHBracingGusset* pHGusset2 = pGirder->GetHBracingGussetInTotal(nPos2);
		if(pHGusset0)
			memcpy(pGusset0, pHGusset0, sizeof(CHBracingGusset));
		if(pHGusset1)
			memcpy(pGusset1, pHGusset1, sizeof(CHBracingGusset));
		if(pHGusset2)
			memcpy(pGusset2, pHGusset2, sizeof(CHBracingGusset));
		GussetArray.push_back(pGusset0);
		GussetArray.push_back(pGusset1);
		GussetArray.push_back(pGusset2);

		CPlateBasicIndex* pBxGu0 = new CPlateBasicIndex();
		CPlateBasicIndex* pBxGu1 = new CPlateBasicIndex();
		CPlateBasicIndex* pBxGu2 = new CPlateBasicIndex();
		if(pGusset0 && pGusset0->GetBx())
			memcpy(pBxGu0, pGusset0->GetBx(), sizeof(CPlateBasicIndex));
		if(pGusset1 && pGusset1->GetBx())
			memcpy(pBxGu1, pGusset1->GetBx(), sizeof(CPlateBasicIndex));
		if(pGusset2 && pGusset2->GetBx())
			memcpy(pBxGu2, pGusset2->GetBx(), sizeof(CPlateBasicIndex));
		IndexGuArray.push_back(pBxGu0);
		IndexGuArray.push_back(pBxGu1);
		IndexGuArray.push_back(pBxGu2);

		CPlateBasicIndex* pBx0 = new CPlateBasicIndex();
		CPlateBasicIndex* pBx1 = new CPlateBasicIndex();
		CPlateBasicIndex* pBx2 = new CPlateBasicIndex();
		CPlateBasicIndex* pBxBxCr0 = pBxGu0->GetBxCrossBeamByHGuBx(FALSE, TRUE);
		CPlateBasicIndex* pBxBxCr1 = pBxGu1->GetBxCrossBeamByHGuBx(FALSE, TRUE);
		CPlateBasicIndex* pBxBxCr2 = pBxGu2->GetBxCrossBeamByHGuBx(FALSE, TRUE);
		if(pBxBxCr0) 
			memcpy(pBx0, pBxBxCr0, sizeof(CPlateBasicIndex));
		if(pBxBxCr1) 
			memcpy(pBx1, pBxBxCr1, sizeof(CPlateBasicIndex));
		if(pBxBxCr2) 
			memcpy(pBx2, pBxBxCr2, sizeof(CPlateBasicIndex));
		IndexBxArray.push_back(pBx0);
		IndexBxArray.push_back(pBx1);
		IndexBxArray.push_back(pBx2);
	}

	// 상세원의 범위	
	double dRad =max(700, pBrGu->m_dH[0]);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DrawPyung.DrawHBracingCLGirder(&Dom, 0, nGTotal, nPos0, nPos2);

	//수평브레이싱이음판*****************************************************************************
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	for (n=0; n<(long)IndexGuArray.size(); n++)
	{
		if (bCut)
		{
			long nCurPos = (n%3==0)? nPos0 : (n%3==1)? nPos1 : nPos2;
			if (nG!=n/3 || nCurPos!=nPos) continue;
			DrawHGussetOnHGussetBx(&Dom, IndexGuArray[n], FALSE, 0, TRUE, TRUE, bSangseDom, TRUE, bJewon,0,bInput);
		}
		else
		{
			DrawHGussetOnHGussetBx(&Dom, IndexGuArray[n], FALSE, 0, TRUE, TRUE, bSangseDom, TRUE, bJewon,0,bInput);
		}
	}

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxVStiff = Finder.GetBxFirst(BX_VSTIFF|BX_JIJUMSTIFF|BX_CROSSBEAM_VBRACING);

	for (n=0; n<(long)IndexBxArray.size(); n+=3)
	{
		CPlateBasicIndex* pBx0 = IndexBxArray[n+0];
		CPlateBasicIndex* pBx1 = IndexBxArray[n+1];
		CPlateBasicIndex* pBx2 = IndexBxArray[n+2];

		if(pBx0->IsJijum()) DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx0, FALSE, FALSE);	// 지점보강재
		else				DrawPyung.DrawVStiffOnBx(&Dom, pBx0, TRUE);				// 수직보강재
		if(pBx1->IsJijum()) DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx1, FALSE, FALSE);
		else				DrawPyung.DrawVStiffOnBx(&Dom, pBx1, TRUE);
		if(pBx2->IsJijum()) DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx2, FALSE, FALSE);
		else				DrawPyung.DrawVStiffOnBx(&Dom, pBx2, TRUE);

		double dLeft  = pBx1->GetStation() - pBx0->GetStation();
		double dRight = pBx2->GetStation() - pBx1->GetStation();
 		DrawPyung.DrawGirderWithenStationByLine(&Dom, pBx1, pBx1->GetStation()-dRight, pBx1->GetStation()+dLeft, TRUE);

		// 주부재 방향벡터
		if (n==0)
		{
			vAngle = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
			vAngle = vAngle.Unit();
		}
	}

	//가로보 상판, 가셋트 상판 및 복부판*************************************************************
	for (n=0; n<nGTotal; n++)
	{
		CPlateGirderApp*  pGirder = GirderArray[n];
		CPlateBasicIndex* pBx0 = IndexBxArray[n*3+0];
		CPlateBasicIndex* pBx1 = IndexBxArray[n*3+1];
		CPlateBasicIndex* pBx2 = IndexBxArray[n*3+2];

		if(n==0)
		{
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx0, TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx1, TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx2, TRUE, TRUE);
		}
		else if(n==nGTotal-1)
		{
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx0->GetBxMatchByCrossBeam(pGirder->m_pLeft), TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx1->GetBxMatchByCrossBeam(pGirder->m_pLeft), TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx2->GetBxMatchByCrossBeam(pGirder->m_pLeft), TRUE, TRUE);
		}
		else 
		{
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx0->GetBxMatchByCrossBeam(pGirder->m_pLeft), TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx0, TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx1->GetBxMatchByCrossBeam(pGirder->m_pLeft), TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx1, TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx2->GetBxMatchByCrossBeam(pGirder->m_pLeft), TRUE, TRUE);
			DrawPyung.DrawCRBeamVBracingFlangeOnBx(&Dom, pBx2, TRUE, TRUE);
		}
	}

	//수직브레이싱 이음판****************************************************************************
	for (n=0; n<nGTotal; n++)
	{
		CPlateGirderApp*  pGirder = GirderArray[n];

		for (long j=0; j<3; j++)
		{
			CHBracingGusset*  pGusset = GussetArray[n*3+j];
			CPlateBasicIndex* pBxGu   = IndexGuArray[n*3+j];
			CPlateBasicIndex* pBxIndx = IndexBxArray[n*3+j];
			
			if (pGusset->IsReal(0)) 
				DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBxIndx->GetBxMatchByCrossBeam(pGirder->m_pLeft), FALSE, FALSE);	//pBx1거더의 이전거더의 우측이음판
			if (pGusset->IsReal(1)) 
				DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBxIndx, TRUE, FALSE);
		}
	}

	//수평브레이싱부재생성(V형과 역V형 지원)*********************************************************
	for (n=0; n<nGTotal; n++)
	{
		if(n==0)
		{
			CPlateBasicIndex *pGuIdx = IndexGuArray[n*3];
			CPlateBasicIndex *pBxPrev = pGuIdx->GetBxCrossBeamByHGuBx(FALSE, TRUE)->GetBxPrev(BX_CROSSBEAM_VBRACING, 2); // 우측매치
 			DrawBeamOnBracingPos(&Dom, pGuIdx->GetBxCrossBeamByHGuBx(FALSE, TRUE));
		}
		else if(n==nGTotal - 1)	
		{
			CPlateGirderApp*  pGirder		= GirderArray[n];
			CPlateBasicIndex *pGuIdx		= IndexGuArray[n*3];
			CPlateBasicIndex *pBxCross	  = pGuIdx->GetBxCrossBeamByHGuBx(TRUE, TRUE);
			CPlateBasicIndex *pBxMatch	  = pBxCross ? pBxCross->GetBxMatchByCrossBeam(pGirder->m_pLeft) : NULL; // 좌측매치
			//CPlateBasicIndex *pBxPrevLeft = pBxMatch ? pBxMatch->GetBxPrev(BX_CROSSBEAM_VBRACING, 2) : NULL;// 좌측매치에서 우측매치가로보검사로 이전 찾음.

			if(pBxMatch)		DrawBeamOnBracingPos(&Dom, pBxMatch);
		}
		else
		{
			// 좌측
			CPlateGirderApp*  pGirder	  = GirderArray[n];
			CPlateBasicIndex *pGuIdx	  = IndexGuArray[n*3];

			CPlateBasicIndex *pBxCross	  = pGuIdx->GetBxCrossBeamByHGuBx(TRUE, TRUE);
			CPlateBasicIndex *pBxMatch	  = pBxCross ? pBxCross->GetBxMatchByCrossBeam(pGirder->m_pLeft) : NULL; // 좌측매치
			CPlateBasicIndex *pBxPrevLeft = pBxMatch ? pBxMatch->GetBxPrev(BX_CROSSBEAM_VBRACING, 2) : NULL;
			CPlateBasicIndex *pBxLeft = pBxMatch;
			if(pBxLeft)			DrawBeamOnBracingPos(&Dom, pBxLeft);

			// 우측
			DrawBeamOnBracingPos(&Dom, pGuIdx->GetBxCrossBeamByHGuBx(FALSE, TRUE));
		}
	}

	//이음판****************************************************************************************
	if (bCut)
	{
		CHBracingGusset  *pBrCur = pGir->GetHBracingGussetInTotal(nPos);
		CPlateBasicIndex *pBxCur = pBrCur->GetBx();
		double  dSta  = pBxCur->GetStation();
		CDPoint xyCen = pGir->GetXyGirderDis(dSta);

		Dom.TrimByCircle(xyCen, dRad*1.5, TRUE);
		Dom.Circle(xyCen, dRad*1.5);
		DrawOutLineOfCurHGusset(&Dom, pGir, nPos, dRad*1.5);
	}
	else
	{
		DrawOutLineOfCurHGusset(&Dom, pGir, nPos, dRad*1.5);
	}

	// Dom Rotation ****************************************************************************************
 	double dAngle = vAngle.GetAngleRadian();
 	Dom.Rotate(-dAngle);

	*pDom << Dom;

	// Array Release ****************************************************************************************
	for (n=0; n<(long)GirderArray.size(); n++)
	{
		CPlateGirderApp* p = GirderArray[n];
		p = NULL;
		delete p;
	}
	GirderArray.empty();

	for (n=0; n<(long)GussetArray.size(); n++)
	{
		CHBracingGusset* p = GussetArray[n];
		p = NULL;
		delete p;
	}
	GussetArray.empty();

	for (n=0; n<(long)IndexGuArray.size(); n++)
	{
		CPlateBasicIndex* p = IndexGuArray[n];
		p = NULL;
		delete p;
	}
	IndexGuArray.empty();

	for (n=0; n<(long)IndexBxArray.size(); n++)
	{
		CPlateBasicIndex* p = IndexGuArray[n];
		p = NULL;
		delete p;
	}
	IndexBxArray.empty();
}


// 횡브레이싱을 그리는 함수는 마지막 이전거더 까지임.
void CAPlateDrawHBracing::DrawBeamOnBracingPos(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon)
{
	if(pBx==NULL) return;
	if(pBx->GetGirder()->m_pRight==NULL)	return;

	CBracing *pBr = pBx->GetBracing();
	if(!pBr)	return;
	
	CDomyun Dom(pDom);

	if(pBr->GetType() == CBracing::BRACTYPE_LEFTWAVE)  
		DrawBeamTypeVInv(&Dom, pBx, bJewon);
	if(pBr->GetType() == CBracing::BRACTYPE_RIGHTWAVE) 
		DrawBeamTypeV(&Dom, pBx, bJewon);
	if(pBr->GetType() == CBracing::BRACTYPE_CROSS)	
		DrawBeamTypeX(&Dom, pBx, bJewon);
	if(pBr->GetType() == CBracing::BRACTYPE_SLOPLEFT)  
		DrawBeamTypeSLOPLEFT(&Dom, pBx, bJewon);
	if(pBr->GetType() == CBracing::BRACTYPE_SLOPRIGHT) 
		DrawBeamTypeSLOPRIGHT(&Dom, pBx, bJewon);

	*pDom << Dom;
}
 

//nPos = Bracing No.
void CAPlateDrawHBracing::DrawBeamTypeV(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	if(pGir==NULL) return;
	
	CAPlateDrawSteel DrawSteel(m_pDataMng);

	CDPoint xyStt, xyEnd, xySttMid, xyEndMid; 
	BOOL bEndArrow = FALSE;
	CPlateBasicIndex *pBxNext  = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
	if(pBxNext==NULL)		return;

	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pRight); 
	CPlateBasicIndex *pBxMatchNext = pBxNext->GetBxMatchByCrossBeam(pGir->m_pRight); 
	CPlateBasicIndex *pBxCenter	= pGir->m_pRight->GetBxByStation((pBxMatch->GetStation()+pBxMatchNext->GetStation())/2, BX_HBRACINGGUSSET);
	CHBracingGusset  *pBrGu1 = pBx->GetHBracingGusset();
	CHBracingGusset  *pBrGu2 = pBxNext->GetHBracingGusset();
	CHBracingGusset  *pBrGu3 = pBxCenter->GetHBracingGusset();
	long nSteelTypeDown = pBx->GetBracing()->GetSteelMaterialData()->m_nType;
	
	CDomyun Dom(pDom);
	
	//부재생성
	long nDir = m_pDataMng->GetGlobalOption()->GetDesignHBracingDirChange();
	long nInstallDir[4] = {nDir, nDir, 0, nDir};

	///////////////////////////////////////////////////////////////////////////////////
	//첫번째 부재
	xyStt   = pGir->GetXyGirderDis(pBx->GetStation());
	xyEnd	= pGir->m_pRight->GetXyGirderDis(pBxCenter->GetStation());

	CDPoint vDir = (xyEnd-xyStt).Unit();
	double	dBrGu1Sp	= pBrGu1->m_sA+pBrGu1->m_sB*pBrGu1->m_sC+pBrGu1->m_sD;
	double	dBrGu2Sp	= pBrGu2->m_sA+pBrGu2->m_sB*pBrGu2->m_sC+pBrGu2->m_sD;
	double	dBrGu3Sp	= pBrGu3->m_sA+pBrGu3->m_sB*pBrGu3->m_sC+pBrGu3->m_sD;
	xyStt += vDir*pBrGu1->m_dDis[3];
	xyEnd -= vDir*pBrGu3->m_dDis[0];
	xySttMid	= xyStt + vDir*dBrGu1Sp;
	xyEndMid	= xyEnd - vDir*dBrGu3Sp;
		
	pBx->GetBracing()->m_dLengthBeam[0] = ~(xyEnd-xyStt);//부재길이설정

	if(nDir==1)	// 상향
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST);
	else
	{
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xySttMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xySttMid, xyEndMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyEndMid, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
	}

	///////////////////////////////////////////////////////////////////////////////////
	//두번째 부재
	xyStt   = pGir->m_pRight->GetXyGirderDis(pBxCenter->GetStation());
	xyEnd   = pGir->GetXyGirderDis(pBxNext->GetStation());
	
	vDir    = (xyEnd-xyStt).Unit();
	xyStt += vDir*pBrGu3->m_dDis[1];
	xyEnd -= vDir*pBrGu2->m_dDis[2];
	xySttMid	= xyStt + vDir*dBrGu3Sp;
	xyEndMid	= xyEnd - vDir*dBrGu2Sp;

	pBx->GetBracing()->m_dLengthBeam[1] = ~(xyEnd-xyStt);//부재길이설정	

	if(nDir==1)	// 상향
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND);
	else
	{
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xySttMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND, TRUE);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xySttMid, xyEndMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyEndMid, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND, TRUE);
	}

	*pDom << Dom;
}

void CAPlateDrawHBracing::DrawBeamTypeVInv(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	if(pGir==NULL) return;
	
	CAPlateDrawSteel DrawSteel(m_pDataMng);

	CDPoint xyStt, xyEnd, xySttMid, xyEndMid;
	BOOL bEndArrow = FALSE;
	CPlateBasicIndex *pBxNext  = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
	if(pBxNext==NULL)		return;

	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pRight); 
	CPlateBasicIndex *pBxMatchNext = pBxNext->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateBasicIndex *pBxCenter = pGir->GetBxByStation((pBx->GetStation()+pBxNext->GetStation())/2, BX_HBRACINGGUSSET);
	
	CHBracingGusset  *pBrGu1 = pBxMatch->GetHBracingGusset();
	CHBracingGusset  *pBrGu2 = pBxMatchNext->GetHBracingGusset();
	CHBracingGusset  *pBrGu3 = pBxCenter->GetHBracingGusset(); 
	long nSteelTypeDown = pBx->GetBracing()->GetSteelMaterialData()->m_nType;

	CDomyun Dom(pDom);
	//부재생성
	long nDir = m_pDataMng->GetGlobalOption()->GetDesignHBracingDirChange();
	long nInstallDir[4] = {nDir, nDir, 0, nDir};

	///////////////////////////////////////////////////////////////////////////////////
	//첫번째 부재
	xyStt   = pGir->m_pRight->GetXyGirderDis(pBxMatch->GetStation());
	xyEnd   = pGir->GetXyGirderDis(pBxCenter->GetStation());
	
	CDPoint vDir = (xyEnd-xyStt).Unit();
	double	dBrGu1Sp	= pBrGu1->m_sA+pBrGu1->m_sB*pBrGu1->m_sC+pBrGu1->m_sD;
	double	dBrGu2Sp	= pBrGu2->m_sA+pBrGu2->m_sB*pBrGu2->m_sC+pBrGu2->m_sD;
	double	dBrGu3Sp	= pBrGu3->m_sA+pBrGu3->m_sB*pBrGu3->m_sC+pBrGu3->m_sD;
	xyStt += vDir*pBrGu1->m_dDis[1];
	xyEnd -= vDir*pBrGu3->m_dDis[2];
	xySttMid	= xyStt + vDir*dBrGu1Sp;
	xyEndMid	= xyEnd - vDir*dBrGu3Sp;
	
	pBx->GetBracing()->m_dLengthBeam[0] = ~(xyEnd-xyStt);//부재길이설정

	if(nDir==1)	// 상향
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST);
	else
	{
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xySttMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xySttMid, xyEndMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyEndMid, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
	}

	///////////////////////////////////////////////////////////////////////////////////
	//두번째 부재
	xyStt = pGir->GetXyGirderDis(pBxCenter->GetStation());
	xyEnd = pGir->m_pRight->GetXyGirderDis(pBxMatchNext->GetStation());

	vDir = (xyEnd-xyStt).Unit();
	xyStt += vDir*pBrGu3->m_dDis[3];
	xyEnd -= vDir*pBrGu2->m_dDis[0];
	xySttMid	= xyStt + vDir*dBrGu3Sp;
	xyEndMid	= xyEnd - vDir*dBrGu2Sp;

	pBx->GetBracing()->m_dLengthBeam[1] = ~(xyEnd-xyStt);//부재길이설정

	if(nDir==1)	// 상향
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND);
	else
	{
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xySttMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND, TRUE);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xySttMid, xyEndMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyEndMid, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND, TRUE);
	}

	*pDom << Dom;

}

void CAPlateDrawHBracing::DrawBeamTypeSLOPLEFT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon)
{
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	
	CAPlateDrawSteel DrawSteel(m_pDataMng);
	if(pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2)==NULL)		return;

	CPlateBasicIndex *pBxMatchNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2)->GetBxMatchByCrossBeam(pGir->m_pRight);
	CHBracingGusset  *pBrGu1 = pBx->GetHBracingGusset();
	CHBracingGusset  *pBrGu2 = pBxMatchNext->GetHBracingGusset();
	
	CDPoint xyStt, xyEnd, xySttMid, xyEndMid;
	long nSteelTypeDown = pBx->GetBracing()->GetSteelMaterialData()->m_nType;

	CDomyun Dom(pDom);
	//부재생성
	long nDir = m_pDataMng->GetGlobalOption()->GetDesignHBracingDirChange();
	long nInstallDir[4] = {nDir, nDir, 0, nDir};

	//////////////////////////////////////////////////////////////////////////////////
	//첫번째 부재
	xyStt = pGir->GetXyGirderDis(pBx->GetStation());
	xyEnd = pGir->m_pRight->GetXyGirderDis(pBxMatchNext->GetStation());

	CDPoint vDir = (xyEnd-xyStt).Unit();
	double	dBrGu1Sp	= pBrGu1->m_sA+pBrGu1->m_sB*pBrGu1->m_sC+pBrGu1->m_sD;
	double	dBrGu2Sp	= pBrGu2->m_sA+pBrGu2->m_sB*pBrGu2->m_sC+pBrGu2->m_sD;

	xyStt += vDir*pBrGu1->m_dDis[3];
	xyEnd -= vDir*pBrGu2->m_dDis[0];
	xySttMid	= xyStt + vDir*dBrGu1Sp;
	xyEndMid	= xyEnd - vDir*dBrGu2Sp;
	
	//부재길이설정
	pBx->GetBracing()->m_dLengthBeam[0] = ~(xyEnd-xyStt);

	if(nDir==1)	// 상향
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, FALSE, CAPlateDrawSteel::HFLANGE_FIRST);
	else
	{
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xySttMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, FALSE, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xySttMid, xyEndMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, FALSE, CAPlateDrawSteel::HFLANGE_FIRST);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyEndMid, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, FALSE, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
	}

	*pDom << Dom;
}

void CAPlateDrawHBracing::DrawBeamTypeSLOPRIGHT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon)
{
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	
	CAPlateDrawSteel DrawSteel(m_pDataMng);

	CPlateBasicIndex *pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
	if(pBxNext==NULL)		return;

	CPlateBasicIndex *pBxMatch= pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CHBracingGusset  *pBrGu1 = pBxMatch->GetHBracingGusset();
	CHBracingGusset  *pBrGu2 = pBxNext->GetHBracingGusset();
	
	CDPoint xyStt, xyEnd, xySttMid, xyEndMid;
	long nSteelTypeDown = pBx->GetBracing()->GetSteelMaterialData()->m_nType;
	
	CDomyun Dom(pDom);
	//부재생성
	long nDir = m_pDataMng->GetGlobalOption()->GetDesignHBracingDirChange();
	long nInstallDir[4] = {nDir, nDir, 0, nDir};

	///////////////////////////////////////////////////////////////////////////////////
	//첫번째 부재
	xyStt = pGir->m_pRight->GetXyGirderDis(pBxMatch->GetStation());
	xyEnd = pGir->GetXyGirderDis(pBxNext->GetStation());
	
	CDPoint vDir = (xyEnd-xyStt).Unit();
	double	dBrGu1Sp	= pBrGu1->m_sA+pBrGu1->m_sB*pBrGu1->m_sC+pBrGu1->m_sD;
	double	dBrGu2Sp	= pBrGu2->m_sA+pBrGu2->m_sB*pBrGu2->m_sC+pBrGu2->m_sD;

	xyStt += vDir*pBrGu1->m_dDis[1];
	xyEnd -= vDir*pBrGu2->m_dDis[2];
	xySttMid	= xyStt + vDir*dBrGu1Sp;
	xyEndMid	= xyEnd - vDir*dBrGu2Sp;
	
	//부재길이설정
	pBx->GetBracing()->m_dLengthBeam[0] = ~(xyEnd-xyStt);
	
	if(nDir==1)	// 상향
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, FALSE, CAPlateDrawSteel::HFLANGE_FIRST);
	else
	{
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xySttMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, FALSE, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xySttMid, xyEndMid, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, bJewon, FALSE, CAPlateDrawSteel::HFLANGE_FIRST);
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyEndMid, xyEnd, CAPlateDrawSteel::HBRACING,
			CAPlateDrawSteel::ALL, FALSE, FALSE, CAPlateDrawSteel::HFLANGE_FIRST, TRUE);
	}

	*pDom << Dom;
}

// 일단은 교차로 가정..지원 안하나? 왜 막혀있지
void CAPlateDrawHBracing::DrawBeamTypeX(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon)
{
/*
	CSteelMaterial   *pMat    = m_pDataMng->GetSteelMaterial();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	
	CAPlateDrawSteel DrawSteel(m_pDataMng);

	CDPoint xyStt, xyEnd;
	long nSteelTypeUp   = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_UHBEAM);
	long nSteelTypeDown = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_LHBEAM);
	BOOL bEndArrow = FALSE;
	CPlateBasicIndex *pBxNext  = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
	if(pBxNext==NULL)		return;

	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pRight); 
	CPlateBasicIndex *pBxMatchNext = pBxNext->GetBxMatchByCrossBeam(pGir->m_pRight); 
	CHBracingGusset  *pBrGu1 = pBx->GetHBracingGusset();
	CHBracingGusset  *pBrGu2 = pBxNext->GetHBracingGusset();
	CHBracingGusset  *pBrGu3 = pBxMatch->GetHBracingGusset();
	CHBracingGusset  *pBrGu4 = pBxMatchNext->GetHBracingGusset();
	///////////////////////////////////////////////////////////////////////////////////
	//첫번째 부재
	xyStt = pGir->GetXyGirderDis(pBx->GetStation());
	xyEnd = pGir->m_pRight->GetXyGirderDis(pBxMatchNext->GetStation());
	CDPoint vDir = (xyEnd-xyStt).Unit();
	xyStt += vDir*pBrGu1->m_dDis[3];
	xyEnd -= vDir*pBrGu4->m_dDis[0];
	//부재생성
	long nInstallDir[4] = {0, 0, 0, 0};
	CDomyun Dom(pDom);
	pBx->GetBracing()->m_dLengthBeam[0] = ~(xyEnd-xyStt);//부재길이설정
		if(m_pDataMng->GetGlobalOption()->GetDesignHBracingDirChange() == 0)//하향
		{
			CPlatePieceObject Obj1(pGir), Obj2(pGir), Obj3(pGir), Obj4(pGir);
			Obj1.m_nMarkBuje = Obj2.m_nMarkBuje = Obj3.m_nMarkBuje = Obj4.m_nMarkBuje = HB_GU;
			pBrGu1->CreatePieceObj(&Obj1, &Obj2, &Obj3, &Obj4, FALSE, FALSE);
			CPlatePiecePointArray& vArr = Obj1.GetPiecePointArray();
			CDPoint xyResult(xyStt);

			int nCountCross = 0;
			for (long n=0; n<vArr.GetSize()-1; n++)
				if(GetXyMatchSegAndSegNoChangeRes(vArr[n], vArr[n+1], xyStt, xyEnd, xyResult))
					nCountCross++;
			if(nCountCross==0)
				DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST);
			else if(nCountCross==1)
			{
				DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyResult, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST,TRUE);
				DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyResult, xyEnd, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST,FALSE);
			}
			else ASSERT(FALSE);
		}
		else
			DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_FIRST);
	///////////////////////////////////////////////////////////////////////////////////
	//두번째 부재
	xyStt = pGir->m_pRight->GetXyGirderDis(pBxMatch->GetStation());
	xyEnd = pGir->GetXyGirderDis(pBxNext->GetStation());
	vDir = (xyEnd-xyStt).Unit();
	xyStt += vDir*pBrGu3->m_dDis[1];
	xyEnd -= vDir*pBrGu2->m_dDis[2];
	//부재생성
	long nInstallDir1[4] = {0, 0, 0, 0};
	pBx->GetBracing()->m_dLengthBeam[1] = ~(xyEnd-xyStt);//부재길이설정
		if(m_pDataMng->GetGlobalOption()->GetDesignHBracingDirChange() == 0)//하향
		{
			CPlatePieceObject Obj1(pGir), Obj2(pGir), Obj3(pGir), Obj4(pGir);
			Obj1.m_nMarkBuje = Obj2.m_nMarkBuje = Obj3.m_nMarkBuje = Obj4.m_nMarkBuje = HB_GU;
			pBrGu4->CreatePieceObj(&Obj1, &Obj2, &Obj3, &Obj4, FALSE, FALSE);
			CPlatePiecePointArray& vArr = Obj1.GetPiecePointArray();
			CDPoint xyResult(xyStt);

			int nCountCross = 0;
			for (long n=0; n<vArr.GetSize()-1; n++)
				if(GetXyMatchSegAndSegNoChangeRes(vArr[n], vArr[n+1], xyStt, xyEnd, xyResult))
					nCountCross++;
			if(nCountCross==0)
				DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND);
			else if(nCountCross==1)
			{
				DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyStt, xyResult, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND,FALSE);
				DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir, 1, xyResult, xyEnd, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND,TRUE);
			}
			else ASSERT(FALSE);
		}
		else
			DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeDown, nInstallDir1, 1, xyStt, xyEnd, CAPlateDrawSteel::HBRACING, CAPlateDrawSteel::ALL, bJewon, bEndArrow, CAPlateDrawSteel::HFLANGE_SECOND);
	
	*pDom << Dom;
	*/
}

void CAPlateDrawHBracing::DrawHGuessetBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(!m_pDataMng->GetBridge()->IsPlateGir())	// 플레이트교량이 아니면
		return;

	CPlateGirderApp *pGir = pBxStt->GetGirder();
	__int64 BxFlag = BX_HBRACINGGUSSET;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	CDomyun Dom(pDom);	
	CAPlateDrawPyung DrawPyung(m_pDataMng);
	while(pBx)
	{
		DrawHGussetOnHGussetBx(&Dom, pBx, FALSE, 0, FALSE, FALSE, FALSE, FALSE, FALSE);
		pBx = Finder.GetBxNext();
	}
	*pDom << Dom;
}

//pBx = BX_HBRACINGGUSSET
  
void CAPlateDrawHBracing::DrawHGussetOnHGussetBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bOptionApply, long nOption, 
										BOOL bBoltHole, BOOL bBoltLine, BOOL bSangSe, BOOL bDim, BOOL bJewon, long nType, BOOL bInput)
{
	CARoadOptionStd* pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir    = pBx->GetGirder();
	CPlateGirderApp* pGirL   = pGir->m_pLeft;
	CPlateGirderApp* pGirR   = pGir->m_pRight;
	CHBracingGusset* pBrGu   = pBx->GetHBracingGusset();
	
	if(!pBrGu->IsReal()) return;	// m_nMatchCol이 모두 0일경우

//		┌─┰─┐
//		│0 ┃ 1│
//		┝━╋━┥	m_nMatchCol의 대응위치
//		│2 ┃ 3│
//		└─┸─┘

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CPlatePieceObject pObj1(pGir), pObj2(pGir);
	pBrGu->CreatePieceObj(&pObj1, &pObj2, TRUE, TRUE);

	if(pBrGu->IsReal(0))	
		pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);
	if(pBrGu->IsReal(1))	
		pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);

	if(bDim)	DimHGussetBx(&Dom, pBx, bBoltHole||bBoltLine,bInput);
		
	
//	if(bJewon)
//		DrawHGussetPlateJewon(&Dom, pBrGu, xyCen, CEN_LD_RD);

	*pDom << Dom;
}

void CAPlateDrawHBracing::DimHGussetBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bBoltHole, BOOL bInput)
{
	CARoadOptionStd		*pOptStd	= m_pDataMng->GetOptionStd();
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CHBracingGusset		*pBrGu		= pBx->GetHBracingGusset();
	CPlateBasicIndex	*pBxCr		= pGir->GetBxByStationDir(pBx->GetStation()+10, -1, BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex	*pBxCrL		= pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);

	CDPoint xyCen	= pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint	vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dBeamW	= 0;
	double dBoltSp	= pBrGu->m_sB*pBrGu->m_sC;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	
	BOOL bDimBolt = !bBoltHole;
	long nDan	  = 0;
	
	if(pBrGu->m_nMatchCol[0]==1)
	{
		if(pBrGu->m_nMatchCol[1]==1)	Dom.SetDirection("BOTTOM");
		else							Dom.SetDirection("TOP");

		Dom.DimMoveTo(xyCen.x, xyCen.y,		nDan);
		Dom.DimLineTo(-pBrGu->m_dDis[0],	nDan, bInput ? "dis1" : COMMA(pBrGu->m_dDis[0]));
		if(!bDimBolt)
		{
			Dom.DimLineToExtend(-pBrGu->m_sA,		nDan, "", bInput ? "sA" : COMMA(pBrGu->m_sA));
			Dom.DimLineTo(-pBrGu->m_sB*pBrGu->m_sC, nDan,bInput ?  "sB@sC" : COMMA(pBrGu->m_sB*pBrGu->m_sC));
			Dom.DimLineToExtend(-pBrGu->m_sD,		nDan, "",  bInput ? "sD" : COMMA(pBrGu->m_sD));
			bDimBolt = TRUE;
		}		
		
		dBeamW = pBrGu->pSec0 ? pBrGu->pSec0->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE) : 0;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(xyCen.x-pBrGu->m_dDis[0]-dBoltSp, xyCen.y-dBeamW/2, 0);
		Dom.DimLineTo(-pBrGu->m_gW[0], 0, bInput ? "gW1" : COMMA(pBrGu->m_gW[0]));
		Dom.DimMoveTo(xyCen.x-pBrGu->m_dDis[0]-dBoltSp, xyCen.y+dBeamW/2, 0);
		Dom.DimLineTo(pBrGu->m_gW[0], 0, bInput ?  "gW1" : COMMA(pBrGu->m_gW[0]));

		Dom.Rotate(xyCen, -pBrGu->xyDir[0]);
		*pDom << Dom;
	}
	else if(pBrGu->IsReal(0) && (!pBxCrL || !pBxCrL->GetSection()->IsenDetType(SECDET_CROSSBEAM)))
	{
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xyCen.x, xyCen.y+pBrGu->m_dH[0], 0);
		Dom.DimLineTo(-pBrGu->m_gW[0], 0,bInput ? "gW1" : COMMA(pBrGu->m_gW[0]));

		Dom.Rotate(xyCen, vGir);
		*pDom << Dom;
	}

	nDan = (bBoltHole && !bDimBolt) ? 0 : 1;
	if(pBrGu->m_nMatchCol[1]==1)
	{
		if(pBrGu->m_nMatchCol[3]==1)	Dom.SetDirection("TOP");
		else							Dom.SetDirection("BOTTOM");
		
		Dom.DimMoveTo(xyCen.x, xyCen.y,		nDan);
		Dom.DimLineToExtend(pBrGu->m_dDis[1],		nDan, bInput ? "dis2" : COMMA(pBrGu->m_dDis[1]),"");
		if(!bDimBolt)
		{
			Dom.DimLineToExtend(pBrGu->m_sA,		nDan, "", bInput ? "sA" : COMMA(pBrGu->m_sA));
			Dom.DimLineTo(pBrGu->m_sB*pBrGu->m_sC,	nDan, bInput ? "sB@sC" : COMMA(pBrGu->m_sB*pBrGu->m_sC));
			Dom.DimLineToExtend(pBrGu->m_sD,		nDan, "", bInput ? "sD" : COMMA(pBrGu->m_sD));
			bDimBolt = TRUE;
		}

		dBeamW = pBrGu->pSec1 ? pBrGu->pSec1->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE) : 0;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyCen.x+pBrGu->m_dDis[1]+dBoltSp, xyCen.y-dBeamW/2, 0);
		Dom.DimLineTo(-pBrGu->m_gW[1], 0, bInput ? "gW2" : COMMA(pBrGu->m_gW[1]));
		Dom.DimMoveTo(xyCen.x+pBrGu->m_dDis[1]+dBoltSp, xyCen.y+dBeamW/2, 0);
		Dom.DimLineTo(pBrGu->m_gW[1], 0, bInput ? "gW2" : COMMA(pBrGu->m_gW[1]));
		
		Dom.Rotate(xyCen, pBrGu->xyDir[1]);
		*pDom << Dom;
	}
	else if(pBrGu->IsReal(0) && (!pBxCrL || !pBxCrL->GetSection()->IsenDetType(SECDET_CROSSBEAM)))
	{
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xyCen.x, xyCen.y+pBrGu->m_dH[0], 0);
		Dom.DimLineTo(pBrGu->m_gW[1], 0,bInput ? "gW2" : COMMA(pBrGu->m_gW[1]));

		Dom.Rotate(xyCen, vGir);
		*pDom << Dom;
	}

	nDan = (bBoltHole && !bDimBolt) ? 0 : 1;
	if(pBrGu->m_nMatchCol[2]==1)
	{
		if(pBrGu->m_nMatchCol[0]==1)	Dom.SetDirection("BOTTOM");
		else							Dom.SetDirection("TOP");

		Dom.DimMoveTo(xyCen.x, xyCen.y,		nDan);
		Dom.DimLineTo(-pBrGu->m_dDis[2],	nDan, bInput ? "dis3" : COMMA(pBrGu->m_dDis[2]));
		if(!bDimBolt)
		{
			Dom.DimLineToExtend(-pBrGu->m_sA,		nDan, "", bInput ? "sA" : COMMA(pBrGu->m_sA));
			Dom.DimLineTo(-pBrGu->m_sB*pBrGu->m_sC,	nDan, bInput ? "sB@sC" : COMMA(pBrGu->m_sB*pBrGu->m_sC));
			Dom.DimLineToExtend(-pBrGu->m_sD,		nDan, "",bInput ?  "sD" : COMMA(pBrGu->m_sD));
			bDimBolt = TRUE; 
		}

		dBeamW = pBrGu->pSec2 ? pBrGu->pSec2->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE) : 0;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(xyCen.x-pBrGu->m_dDis[2]-dBoltSp, xyCen.y-dBeamW/2, 0);
		Dom.DimLineTo(-pBrGu->m_gW[2], 0, bInput ? "gW3" : COMMA(pBrGu->m_gW[2]));
		Dom.DimMoveTo(xyCen.x-pBrGu->m_dDis[2]-dBoltSp, xyCen.y+dBeamW/2, 0);
		Dom.DimLineTo(pBrGu->m_gW[2], 0, bInput ? "gW3" : COMMA(pBrGu->m_gW[2]));
		

		Dom.Rotate(xyCen, -pBrGu->xyDir[2]);
		*pDom << Dom;
	}
	else if(pBrGu->IsReal(1) && !pBx->GetCrossBeam())
	{
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyCen.x, xyCen.y-pBrGu->m_dH[1], 0);
		Dom.DimLineTo(-pBrGu->m_gW[2], 0, bInput ? "gW3" : COMMA(pBrGu->m_gW[2]));

		Dom.Rotate(xyCen, vGir);
		*pDom << Dom;
	}

	nDan = (bBoltHole && !bDimBolt) ? 0 : 1;
	if(pBrGu->m_nMatchCol[3]==1)
	{
		if(pBrGu->m_nMatchCol[2]==1)	Dom.SetDirection("TOP");
		else							Dom.SetDirection("BOTTOM");

		Dom.DimMoveTo(xyCen.x, xyCen.y,		nDan);
		Dom.DimLineTo(pBrGu->m_dDis[3],		nDan, bInput ? "dis4" :COMMA(pBrGu->m_dDis[3]) );
		if(!bDimBolt)
		{
			Dom.DimLineToExtend(pBrGu->m_sA,		nDan, "", bInput ? "sA" : COMMA(pBrGu->m_sA));
			Dom.DimLineTo(pBrGu->m_sB*pBrGu->m_sC,	nDan, bInput ? "sB@sC" : COMMA(pBrGu->m_sB*pBrGu->m_sC));
			Dom.DimLineToExtend(pBrGu->m_sD,		nDan, "", bInput ? "sD" :COMMA(pBrGu->m_sD));
			bDimBolt = TRUE;
		}

		dBeamW = pBrGu->pSec3 ? pBrGu->pSec3->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE) : 0;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyCen.x+pBrGu->m_dDis[3]+dBoltSp, xyCen.y-dBeamW/2, 0);
		Dom.DimLineTo(-pBrGu->m_gW[3], 0, bInput ? "gW4" : COMMA(pBrGu->m_gW[3]));
		Dom.DimMoveTo(xyCen.x+pBrGu->m_dDis[3]+dBoltSp, xyCen.y+dBeamW/2, 0);
		Dom.DimLineTo(pBrGu->m_gW[3], 0, bInput ? "gW4" : COMMA(pBrGu->m_gW[3]));
		
		
		Dom.Rotate(xyCen, pBrGu->xyDir[3]);
		*pDom << Dom;
	}
	else if(pBrGu->IsReal(1) && !pBx->GetCrossBeam())
	{
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyCen.x, xyCen.y-pBrGu->m_dH[1], 0);
		Dom.DimLineTo(pBrGu->m_gW[3], 0,bInput ? "gW4" : COMMA(pBrGu->m_gW[3]));

		Dom.Rotate(xyCen, vGir);
		*pDom << Dom;
	}

	if(pBrGu->IsReal(0))
	{
		if(pBrGu->IsReal(1))
		{
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyCen.x-pBrGu->m_dWidth[0], xyCen.y+pBrGu->m_dH[0], 0);
		}
		else
		{
			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(xyCen.x-pBrGu->m_dWidth[0], xyCen.y, 0);
		}
		
		Dom.DimLineTo(pBrGu->m_dWidth[0], 0, bInput ? "W1" : COMMA(pBrGu->m_dWidth[0]));
		Dom.DimLineTo(pBrGu->m_dWidth[1], 0, bInput ? "W2" : COMMA(pBrGu->m_dWidth[1]));
		Dom.Rotate(xyCen, vGir);
		*pDom << Dom;
	}

	if(pBrGu->IsReal(1))
	{
		if(pBrGu->IsReal(0))
		{
			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(xyCen.x-pBrGu->m_dWidth[2], xyCen.y-pBrGu->m_dH[0], 0);
		}
		else
		{
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyCen.x-pBrGu->m_dWidth[2], xyCen.y, 0);
		}
		
		Dom.DimLineTo(pBrGu->m_dWidth[2], 0, bInput ? "W3" : COMMA(pBrGu->m_dWidth[2]));
		Dom.DimLineTo(pBrGu->m_dWidth[3], 0, bInput ? "W4" : COMMA(pBrGu->m_dWidth[3]));
		Dom.Rotate(xyCen, vGir);
		*pDom << Dom;
	}
	*pDom << Dom;
}

//nPos = 횡거더이음판의 번호
void CAPlateDrawHBracing::DrawOutLineOfCurHGusset(CDomyun *pDom, CPlateGirderApp *pGir, long nPos, double dRad)
{
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();
	CHBracingGusset  *pBrGu    = pGir->GetHBracingGussetInTotal(nPos);
	CPlateBasicIndex *pBx      = pBrGu->GetBx();

	double  dSta  = pBx->GetStation();
	CDPoint xyCen = pGir->GetXyGirderDis(dSta);
	CDPoint vDir  = pGir->GetLine()->GetAngleAzimuth(dSta);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetLineColor(2);
	Dom.Circle(xyCen, dRad);

	*pDom << Dom;
}

void CAPlateDrawHBracing::SetHBracingSpJewonTotal(CDomyun *pDom/*임시로*/)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = NULL;
	for(long n=0; n<pBridge->GetGirdersu(); n++)
	{
		pGir = pBridge->GetGirder(n);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);		
		while(pBx)
		{
			DrawHGussetOnHGussetBx(pDom, pBx, FALSE, 0);
			pBx = Finder.GetBxNext();
		}	
	}
}

void CAPlateDrawHBracing::DrawCompScallop(CDomyun *pDomP, double BX, double BY, double ribT, double ribW, BOOL bDrawVStiff, double Length, BOOL bLower, double slope, double sinVal)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = m_pDataMng->GetBridge()->GetGirder(-1);
	if(ribT == 0 || ribW == 0)	return;
	
	CDomyun *pDom = new CDomyun(pDomP);
	
	double B = 0;
	double R = 0;
	if(ribT<=12)
	{
		B = pBridge->m_dSlotB[0];
		R = pBridge->m_dSlotR[0];
	}
	else if(ribT>12 && ribT<=22)
	{
		B = pBridge->m_dSlotB[1];
		R = pBridge->m_dSlotR[1];
	}
	else
	{
		B = pBridge->m_dSlotB[2];
		R = pBridge->m_dSlotR[2];
	}
	double scallopR = B-ribT;
	pDom->LineTo(-(ribT/2/sinVal+Length+scallopR), -(scallopR+ribT/2)/sinVal*slope, (-R+ribT/2)/sinVal-Length, ribW);
	//
	pDom->LineTo(ribT/sinVal/2+Length, ribW-R, ribT/sinVal/2+Length, ribW-R);
	pDom->Arc(ribT/sinVal/2+Length, ribW, R, 270, 180);
	pDom->LineTo(ribT/sinVal/2+Length, ribW+R, ribT/sinVal/2-Length, ribW+R);
	pDom->Arc(ribT/sinVal/2-Length, ribW, R, 90, 90);

	double scallopGenR = ribT > 16 ? pGir->m_pSangse->m_Scallop_R2 : pGir->m_pSangse->m_Scallop_R1;
	pDom->Arc(ribT/sinVal/2+Length, (ribT/sinVal/2)*slope, scallopGenR, ToDegree(atan(slope)), 90-ToDegree(atan(slope)));
	pDom->LineTo(ribT/sinVal/2+Length, scallopR, ribT/sinVal/2+Length, scallopR);
	//
	if(bDrawVStiff)
	{
		pDom->LineTo(-ribT/sinVal/2-Length, -ribT/sinVal/2*slope, -ribT/sinVal/2-Length, ribW);
		pDom->LineTo( ribT/sinVal/2+Length,  ribT/sinVal/2*slope,  ribT/sinVal/2+Length, ribW);
		pDom->LineTo( -ribT/sinVal/2-Length, ribW,  ribT/sinVal/2+Length, ribW);
	}
	pDom->Mirror((0,0),TRUE);
	if(!bLower)
		pDom->Mirror((0,0),FALSE);

	pDom->Move(BX, BY);
	pDom->Rotate(CDPoint(BX, BY), CDPoint(1, slope).Unit());
	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawHBracing::DrawScallopWithThreePoint(CDomyun *pDom, CDPoint xyStt, CDPoint xyMid, CDPoint xyEnd, double dT, double dW)
{
	CPlateGirderApp	*pGir = m_pDataMng->GetBridge()->GetGirder(-1);
	if(dT == 0 || dW == 0)	return;
	
	CDomyun Dom(pDom);

	double scallopW = 30;//pGir->m_pSangse->m_Scallop_JRib_D2;
	double scallopGenR = dT > 16 ? pGir->m_pSangse->m_Scallop_R2 : pGir->m_pSangse->m_Scallop_R1;
	double T = pGir->m_pSangse->m_ScallopBasePoint_Tension ? dT/2: 0;

	Dom.MoveTo(xyStt);
	Dom.LineTo(xyMid.x, xyMid.y-dT/2-scallopW);
	Dom.LineTo(xyMid.x, xyMid.y-dT/2-scallopW, xyMid.x+dW,  xyMid.y-scallopGenR-T);
	Dom.LineTo(xyMid.x+dW, xyMid.y+scallopGenR+T, xyMid.x, xyMid.y+dT/2+scallopW);
	Dom.LineTo(xyEnd);
	Dom.Arc(xyMid.x+dW, xyMid.y+T, scallopGenR, 0, 90);
	Dom.Arc(xyMid.x+dW, xyMid.y-T, scallopGenR, 270, 90);
	//
	*pDom << Dom;
}

// 입력 : slope   - 다이아프램의 경사경사 (다이아프램방향으로 단면을 짤랐을 경우에만 값이 있음)
//               - AROAD의 Drawing방식에서는 항상 Zero =>SHOP에서만 값이 필요
//        sinVal - SKEW에의한 박스 폭원의 변화 비율 pGir->GetAngle(nIdx).y;
//               - AROAD의 Drawing방식에서는 항상 1.00 =>SHOP에서만 값이 필요  
void CAPlateDrawHBracing::DrawTensionScallop(CDomyun *pDomP, double BX, double BY, double ribT
										, double ribW, double Length, BOOL bLower, double slope, double sinVal, BOOL bDrawVStiff)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = m_pDataMng->GetBridge()->GetGirder(-1);
	if(ribT == 0 || ribW == 0)	return;
	
	CDomyun *pDom = new CDomyun(pDomP);
	double A = 0;
	double R = 0;
	if(ribT<=12)
	{
		A = pBridge->m_dSlotA[0];
		R = pBridge->m_dSlotR2[0];
	}
	else if(ribT>12 && ribT<=22)
	{
		A = pBridge->m_dSlotA[1];
		R = pBridge->m_dSlotR2[1];
	}
	else
	{
		A = pBridge->m_dSlotA[2];
		R = pBridge->m_dSlotR2[2];
	}
	double scallopW = (A-ribT)/2;
	double T = pGir->m_pSangse->m_ScallopBasePoint_Tension ? ribT/2: 0;
	
	pDom->LineTo(-ribT/sinVal/2 -scallopW-Length, (ribT/sinVal/2+scallopW)*slope, -R-T-Length, ribW);
	pDom->LineTo( ribT/sinVal/2 + scallopW+Length,  -(ribT/sinVal/2+scallopW)*slope,  R+T+Length, ribW);
	//
	pDom->Arc(-T-Length, ribW, R, 90, 90);
	pDom->Arc( T+Length, ribW, R,  0, 90);
	pDom->LineTo(-T-Length, ribW+R, T+Length, ribW+R);
	//
	if(bDrawVStiff)
	{
		pDom->LineTo(-ribT/sinVal/2-Length, ribT/sinVal/2*slope, -ribT/sinVal/2-Length, ribW);
		pDom->LineTo( ribT/sinVal/2+Length,  -ribT/sinVal/2*slope,  ribT/sinVal/2+Length, ribW);
		pDom->LineTo(-ribT/sinVal/2-Length,  ribW,  ribT/sinVal/2+Length, ribW);
	}
	if(!bLower)	{
		pDom->Mirror((0,0),FALSE);
		pDom->Mirror((0,0),TRUE);
	}

	pDom->Move(BX, BY);
	pDom->Rotate(CDPoint(BX, BY), CDPoint(1, slope).Unit());

	*pDomP << *pDom;
	delete pDom;
}

//nRow=이음판의 형상정의(1: 1/4짜리, 2: 1/2짜리, 3: 1/2+1/4짜리, 4: 4/4짜리)
//nCol=이음판의 위치정의(0: CEN_LU, 1: CEN_RU, 2: CEN_LD, 3: CEN_RD)
//nCol=이음판의 위치정의(0: CEN_LU_RU, 1: CEN_LD_RD)
void CAPlateDrawHBracing::DrawHGussetPlateJewon(CDomyun *pDom, CHBracingGusset* pBrGu, CDPoint xyCen, long nID)
{
/*	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateBasicIndex *pBx     = pBrGu->GetBx();	
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CSteelMaterial   *pMat    = m_pDataMng->GetSteelMaterial();
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	long nRow=0, nCol=0, nQty=0; 
	double dTextWidth=0;
	CVector vJ;
	CString str="", szJewon="", sMark="";
	CVector vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dW = max(pBrGu->m_dW[0], pBrGu->m_dW[1]);
	double dH = max(pBrGu->m_dH[0], pBrGu->m_dH[1]);

	xyCen += vGir*dW*1.3 + vGir.Rotate90()*dH*1.3;

	else
	{
		//거더를 중심으로 양쪽 모두 분리되지 않는경우 
		if(!pBrGu->IsSpMatchingWithWeb(TRUE) && !pBrGu->IsSpMatchingWithWeb(FALSE))
		{
			nRow = 2;
			nCol = 0;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nCol = 1;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;
		}
		//거더를 중심으로 윗부분만 분리되고 아래는 분리안됨모두 분리되지 않는경우 
		else if(pBrGu->IsSpMatchingWithWeb(TRUE) && !pBrGu->IsSpMatchingWithWeb(FALSE))
		{
			nRow = 1;
			nCol = 0;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nCol = 1;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nRow = 2;
			nCol = 1;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;
		
		}
		//거더를 중심으로 윗부분만 분리안되고 아래는 분리됨
		else if(pBrGu->IsSpMatchingWithWeb(TRUE) && !pBrGu->IsSpMatchingWithWeb(FALSE))
		{
			nRow = 2;
			nCol = 0;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nRow = 1;
			nCol = 2;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nCol = 3;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;
	
		}
		else//모두 분리됨
		{
			nRow = 1;
			nCol = 0;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nCol = 1;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nCol = 2;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;

			nCol = 3;
			vJ      = pGir->GetJewonTotal(HB_GU,pBx, nRow, nCol);
			szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
			str.Format("1-Guss.PL %s", szJewon);
			dTextWidth = Dom.GetTextWidth(str);
			Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
			xyCen.y -= Dom.GetTextHeight()*1.5;
			Dom.Rotate(xyCen, vGir);
			*pDom << Dom;
	
		}
		nQty  = pBrGu->GetBoltsu();
		vJ    = pGir->GetJewonTotal(HB_BOLT, pBx);
		sMark = "H.T.B";
		szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
		str.Format("%d-%s M%.0fx%.0f(%s)",nQty,sMark,vJ.y,vJ.z,pBridge->GetMaterial(HB_BOLT, pBx));
		dTextWidth = Dom.GetTextWidth(str);
		Dom.TextOut(xyCen.x+dTextWidth/2, xyCen.y, str);
		Dom.Rotate(xyCen, vGir);
		*pDom << Dom;

	}
*/
}

void CAPlateDrawHBracing::DrawInputHBracingScallop(CDomyun *pDom, double dThickSplice)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double Bx=0,By=0;
	double ribW	= 150;
	double wing = 400;
	double LowT	=  12;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	//압축측
/*
	DrawCompScallop(&Dom,Bx,By,dThickSplice,ribW, TRUE, 0, TRUE, 0);
	if (pGir->m_pSangse->m_ScallopRight)
		Dom.Mirror(Bx,TRUE);
	
	Dom.LineTo(Bx-wing,By, Bx+wing,By);
	Dom.LineTo(Bx-wing,By-LowT, Bx+wing,By-LowT);

	Dom.LineTo(Bx-wing+100, By, Bx-wing+100, By+300);
	Dom.LineTo(Bx-wing+100, By+300, Bx-wing+200, By+400);
	Dom.LineTo(Bx-wing+200, By+400, Bx+wing-200, By+400);
	Dom.LineTo(Bx+wing-200, By+400, Bx+wing-100, By+300);
	Dom.LineTo(Bx+wing-100, By+300, Bx+wing-100, By);

	pOptStd->SetEnvType(&Dom, HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.TextOut(Bx,By+200+ribW*1.5,"압축측");
	if (pGir->m_pSangse->m_ScallopRight)
		Dom.Mirror((Bx,By),TRUE);
	DimInputCompressionRib(&Dom, dThickSplice);
	Dom.Move(-Dom.Always(100), 0);
	*pDom << Dom;
*/
	//인장측
	Bx = 300, By = 0;
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	DrawTensionScallop(&Dom,Bx,By,dThickSplice,ribW, 0, TRUE, 0);

	Dom.LineTo(Bx-wing,By, Bx+wing,By);
	Dom.LineTo(Bx-wing,By-LowT, Bx+wing,By-LowT);

	Dom.LineTo(Bx-wing+100, By, Bx-wing+100, By+300);
	Dom.LineTo(Bx-wing+100, By+300, Bx-wing+200, By+400);
	Dom.LineTo(Bx-wing+200, By+400, Bx+wing-200, By+400);
	Dom.LineTo(Bx+wing-200, By+400, Bx+wing-100, By+300);
	Dom.LineTo(Bx+wing-100, By+300, Bx+wing-100, By);

	pOptStd->SetEnvType(&Dom, HCAD_SUB_TITLE);
	//Dom.SetTextAlignHorz(TA_CENTER);
	//Dom.TextOut(Bx,By+200+ribW*1.5,"횡브레이싱부");
	DimInputTensionRib(&Dom, dThickSplice);

	*pDom << Dom;
}

void CAPlateDrawHBracing::DimInputCompressionRib(CDomyun *pDom, double dThickSplice)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double B=0, R=0;
	double ribW	= 150;
	
	if(dThickSplice<=12)
	{
		B = pBridge->m_dSlotB[0];
		R = pBridge->m_dSlotR[0];
	}
	else if(dThickSplice>12 && dThickSplice<=22)
	{
		B = pBridge->m_dSlotB[1];
		R = pBridge->m_dSlotR[1];
	}
	else
	{
		B = pBridge->m_dSlotB[2];
		R = pBridge->m_dSlotR[2];
	}
	CString str = _T("");
	CString str2 = _T("");

	CDomyun	Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	pDom->SetDirection("TOP");

	Dom.GiSiArrowExtend(-dThickSplice/2-R,ribW,50,-50,4,"R",COMMA(R));
	Dom.LineTo(-dThickSplice/2-R,ribW,-dThickSplice/2,ribW);
	Dom.Rotate(ToRadian(-45),-dThickSplice/2,ribW);
	*pDom << Dom;

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(-dThickSplice/2,0,0);
	Dom.DimLineToExtend(B,0,"B",COMMA(B));
	if(pGir->m_pSangse->m_ScallopRight)
	{
		Dom.Mirror(0,TRUE);
	}	
	*pDom << Dom;
}

void CAPlateDrawHBracing::DimInputTensionRib(CDomyun *pDom, double dThickSplice)
{
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = pBridge->GetGirder(-1);
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	
	double Bx=300,By=0;
	double ribW	= 150;
	double A=0, R=0;
	if(dThickSplice<=12)
	{
		A = pBridge->m_dSlotA[0];
		R = pBridge->m_dSlotR2[0];
	}
	else if(dThickSplice>12 && dThickSplice<=22)
	{
		A = pBridge->m_dSlotA[1];
		R = pBridge->m_dSlotR2[1];
	}
	else
	{
		A = pBridge->m_dSlotA[2];
		R = pBridge->m_dSlotR2[2];
	}
	CString str = _T("");
	//
	CDomyun	Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	pDom->SetDirection("TOP");

	Dom.GiSiArrowExtend(R,ribW,50,50,0,"R2",COMMA(R));
	Dom.LineTo(R,ribW,0,ribW);
	Dom.Rotate(ToRadian(45),0,ribW);
	Dom.Move(Bx,By);
	*pDom << Dom;
		
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(Bx-A/2,By,0);
	Dom.DimLineToExtend(A,0,"A",COMMA(A));
	*pDom << Dom;
}

