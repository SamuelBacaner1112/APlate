// PlateBasicIndex.cpp: implementation of the CPlateBasicIndex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h" 
#include "PlateBasicIndex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
  
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CPlateBasicIndex::m_sBxError = _T("");
CString CPlateBasicIndex::GetErrorBxString()
{
	return CPlateBasicIndex::m_sBxError;
}


CPlateBasicIndex::CPlateBasicIndex(CPlateBasicIndex *pBx)
{
	if(pBx)
	{
		*this = *pBx;
		return;
	}
	
	Clear();
	m_pGir = NULL;
}

CPlateBasicIndex::~CPlateBasicIndex()
{
	for(long n = 0; n < LINKOBJECTSU; n++)
		SetObjLinkSeq(n,-1);
}

void CPlateBasicIndex::Clear()
{
	m_nMatchCol = -1;
	m_dStation  = 0;

	m_BxFlag = 0;
	m_psLeftCross = NULL;
	m_psRightCross = NULL;
	m_psLeft = NULL;
	m_psRight = NULL;
	//m_psCenter = NULL;
	m_psSelf = NULL;

	m_bShift = FALSE;
	m_vAngle = CDPoint(0,1);
	for(long n = 0; n < LINKOBJECTSU; n++)
		m_nObjLinkSeq[n] = -1;


	m_dwFlag = 0;
}


CBracing *CPlateBasicIndex::GetBracing() const
{ 
	return (CBracing*)GetObjectLink(CPlateBxObject::LK_BRACING); 
}

CSection *CPlateBasicIndex::GetSection() const
{	
	return IsState(BX_CROSSBEAM_VBRACING) ? (CSection*)GetObjectLink(CPlateBxObject::LK_CROSSBEAM)
                                 : (CSection*)GetObjectLink(CPlateBxObject::LK_VSTIFF); 
}

CSection *CPlateBasicIndex::GetSectionLeft() const
{	
	CPlateGirderApp* pGir = (CPlateGirderApp*)GetGirder();
	CPlateGirderApp* pGirL = (CPlateGirderApp*)pGir->m_pLeft;
	if(pGirL == NULL) return NULL;
	
	CPlateBasicIndex *pBxLeft = GetBxMatchBySection(pGirL);
	return IsState(BX_CROSSBEAM_VBRACING) ? (CSection*)GetObjectLink(CPlateBxObject::LK_CROSSBEAM) 
		                         : (CSection*)GetObjectLink(CPlateBxObject::LK_VSTIFF); 
}

CVStiff *CPlateBasicIndex::GetJijumStiff() const
{
	return (CVStiff*)GetObjectLink(CPlateBxObject::LK_JIJUM_VSTIFF); 
}

CHBracingGusset *CPlateBasicIndex::GetHBracingGusset() const
{
	return (CHBracingGusset*)GetObjectLink(CPlateBxObject::LK_HBRACING);
}

void CPlateBasicIndex::SetPOSLeftCross(POS ps)
{
	SETDATA(m_psLeftCross,ps)
}

void CPlateBasicIndex::SetPOSRightCross(POS ps)
{
	SETDATA(m_psRightCross,ps)
}


BOOL CPlateBasicIndex::IsPier() const
{
	CSection* pSec = GetSection();
	return (pSec && pSec->GetenType() == SECTION_P) ? TRUE : FALSE;
}

BOOL CPlateBasicIndex::IsAbut() const
{
	CSection* pSec = GetSection();
	return (pSec && pSec->GetenType() == SECTION_A) ? TRUE : FALSE;
}
BOOL CPlateBasicIndex::IsAbutStt() const
{
	CSection* pSec = GetSection();
	if(pSec && pSec->GetenType() == SECTION_A && pSec->m_nSelfCount == 0)
		return TRUE;
	return FALSE;
}
BOOL CPlateBasicIndex::IsAbutEnd() const
{
	CSection* pSec = GetSection();
	if(pSec && pSec->GetenType() == SECTION_A && pSec->m_nSelfCount != 0)
		return TRUE;
	return FALSE;
}


BOOL CPlateBasicIndex::IsDanbuJijumStt() const
{
	CSection* pSec = GetSection();

	if(pSec==NULL)	return FALSE;

	if(pSec->m_nSelfCount == 0)
		return TRUE;
	else
		return FALSE;
}


BOOL CPlateBasicIndex::IsDanbuJijumEnd() const
{
	CSection* pSec = GetSection();

	if(pSec==NULL)	return FALSE;

	long nQtyBracing = GetGirder()->GetQtyBracingAll();
	if(pSec->m_nSelfCount == nQtyBracing-1)
		return TRUE;
	else
		return FALSE;
}

BOOL CPlateBasicIndex::IsDanbuJijum() const
{
	if(IsDanbuJijumStt() || IsDanbuJijumEnd())
		return TRUE;
	else
		return FALSE;
}



//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBasicIndex::GetBxInLeftGirderSection() const
{
	if(!GetCreate() || !GetPOSInLeftGirderSection()) return NULL;
	return m_pGir->GetBx(GetPOSInLeftGirderSection());
}
//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBasicIndex::GetBxInRighGirderSection() const
{
	if(!GetCreate() || !GetPOSInRightGirderSection()) return NULL;
	return m_pGir->GetBx(GetPOSInRightGirderSection());
}


CPlateBasicIndex* CPlateBasicIndex::GetBxMatchBySection(CPlateGirderApp *pGir) const
{
	if(pGir == NULL)	return NULL;
	CPlateBxFinder Finder((CPlateGirderApp *)pGir);
	//CPlateBasicIndex* pBxSelf = (CPlateBasicIndex*)GetBxCenter();
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF);
	while( pBx )
	{
		if( GetMatchCol() == pBx->GetMatchCol() )
			return (CPlateBasicIndex *)Finder.GetBx();

		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}

	return NULL;
}

// nReturnType=0 : 매치되는 가로보가 없을 경우에 가장 마지막 가로보를 리턴함.
// nReturnType=1 : 매치되는 가로보가 없을 경우에 NULL 리턴함.
CPlateBasicIndex* CPlateBasicIndex::GetBxMatchByCrossBeam(CPlateGirderApp *pGir, long nReturnType) const
{
	if(pGir == NULL)	return NULL;
	CPlateBxFinder Finder((CPlateGirderApp *)pGir);
	//CPlateBasicIndex* pBxSelf = (CPlateBasicIndex*)GetBxCenter();
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while( pBx )
	{
		if( GetMatchCol() == pBx->GetMatchCol() )
			return (CPlateBasicIndex *)Finder.GetBx();

		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}

	// 매치가 없을경우 가장 끝을 던져줌
	if(nReturnType==0)
		return Finder.GetBxFixTail();
	else
		return NULL;

}

// 현재가로보에서 해당거더에 매치되는 가로보를 찾는다.
// 만약 매치되는 가로보가 없을 경우는 nDir방향으로 나올때 까지 찾는다.
// 매치되는 가로보가 있을 경우는 GetBxMatchByCrossBeam()함수와 동일함.
// nDir=-1 역방향검색
// nDir= 1 순방향검색
// 지금은 사용하는 곳이 없음.(일단 만들어 둠)==> 사용시 김성곤에게 문의요함.
CPlateBasicIndex* CPlateBasicIndex::GetBxMatchByCrossBeamByDir(CPlateGirderApp *pGir, long nDir) const
{
	//CPlateBasicIndex *pBxOrigin = (CPlateBasicIndex*)GetBxCenter();
	CPlateBasicIndex *pBx       = NULL;
	
	if(GetBxMatchByCrossBeam(pGir, 1)==NULL)
	{
		CPlateBasicIndex *pBxTemp = (CPlateBasicIndex *)this;
		for(long n=0; n<10; n++)	//가로보가 10개이상 빠지는 경우는 없다고 봄.
		{
			if(nDir==-1)
				pBx = pBxTemp->GetBxPrev(BX_CROSSBEAM_VBRACING, 3);
			else
				pBx = pBxTemp->GetBxNext(BX_CROSSBEAM_VBRACING, 3);
						
			if(pBx->GetBxMatchByCrossBeam(pGir))	// 매치되는 가로보가 있을 경우
				return pBx->GetBxMatchByCrossBeam(pGir);
			else									// 매치되는 가로보가 없을 경우
			{
				if(nDir==-1)
					pBxTemp = pBxTemp->GetBxPrev(BX_CROSSBEAM_VBRACING, 3);	// 현재 pBxTemp에서 한칸 뒤로 이동
				else
					pBxTemp = pBxTemp->GetBxNext(BX_CROSSBEAM_VBRACING, 3);	// 현재 pBxTemp에서 한칸 앞으로 이동
			}
		}
	}
	else
		return GetBxMatchByCrossBeam(pGir, 1);

	return NULL;

}


CPlateBasicIndex* CPlateBasicIndex::GetBxMatchByVStiff(CPlateGirderApp *pGir) const
{
	if(pGir == NULL)	return NULL;
	CPlateBxFinder Finder((CPlateGirderApp *)pGir);
//	CPlateBasicIndex* pBxSelf = (CPlateBasicIndex*)GetBxCenter();
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
	while( pBx )
	{
		if( GetMatchCol() == pBx->GetMatchCol() )
			return (CPlateBasicIndex *)Finder.GetBx();

		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}

	return NULL;
}


CPlateCrossBeam* CPlateBasicIndex::GetCrossBeam() const
{
	CSection *pSec = GetSection();
	if(pSec)
	{
		return pSec->GetCrossBeam();
	}
	ASSERT(TRUE);
	return NULL;
}

CPlateCrossBeam* CPlateBasicIndex::GetCrossBeamLeft() const
{
	CSection* pSecPre = GetSectionLeft();
	if(pSecPre)
	{
		return pSecPre->GetCrossBeam();
	}
	ASSERT(TRUE);
	return NULL;
}

CString CPlateBasicIndex::GetNameSectionType(BOOL bFullStr /*=TRUE*/) const
{
	CPlateGirderApp *pGir			= GetGirder();
	CPlateBridgeApp *pBridge	= (CPlateBridgeApp *)pGir->GetBridgeApp();	
	CString csType;
	long num = 0;

	if(IsState(BX_CROSSBEAM_VBRACING))
	{
		CSection *pSec = GetSection();		
		if(pSec->GetenType() == SECTION_A)
		{			
			long nJ = pGir->GetNumberJijumByBx((CPlateBasicIndex *)this);
			num = 1;
			if(pBridge->m_nTypeAbutOrPier[STT_POS] == 0 && nJ==0) ++num;
			if(pBridge->m_nTypeAbutOrPier[END_POS] == 0 && nJ!=0) ++num;
			csType = "A";
		}
		else if(pSec->GetenType() == SECTION_P)
		{
			long nJ = pGir->GetNumberJijumByBx((CPlateBasicIndex *)this);			
			num = nJ + 1;
			if(pBridge->m_nTypeAbutOrPier[STT_POS] != 0) num = nJ+1;			
			csType = "P";
		}
		else 
		{
			csType = "CR";
		}
	}
	else if(IsState(BX_VSTIFF))
	{
		CSection *pSec = GetSection();
		csType = "VS";
		num = pSec->GetenTypeNum();
	}
	else if(IsState(BX_SPLICE))
	{
		csType = "SP";
		CPlateSplice* pSp = GetSplice();
		num = pSp->m_enTypeNum;
		num++;
	}
	else if(IsState(BX_JACKUP))
	{
		csType = "J";
		num = 0;
	}
	else if(IsState(BX_JIJUMSTIFF))
	{
		csType = "JS";
		num = 0;
	}

//	if( bFullStr == FALSE || num == 0) return csType;

	CString sResult;
	sResult.Format("%s%ld",csType,num);
/*	if(IsJijum())
	{
		long nJijan = pGir->GetNumberJijumByStation(m_dStation);
		sResult = pBridge->m_strJijumName[nJijan];
	}
*/
	return sResult;
}

CString CPlateBasicIndex::GetNumberSectionTypeForOut(BOOL bCross) const
{
	CString str = "아직 미완성";
	/*
	long num = 0;

	if(IsState(BX_CROSSBEAM_VBRACING))
	{
		CDiaphragm* pDia = GetDiaphragm();
		num = pDia->m_enTypeNum;
 		switch(pDia->m_enType)
		{
		case CDiaphragm::TYPE_A :  num++; break;
		case CDiaphragm::TYPE_P :  num++; break;
		}
		str.Format("%d", num);
	}
	if(IsState(BX_SPLICE))
	{
		CPlateSplice* pSp = GetSplice();
		num = pSp->m_enTypeNum + 1;
		str.Format("%02d", num + ((CPlateBridgeApp*)GetGirder()->GetBridgeApp())->GetNumberStartSplice());
	}
	if(bCross)
	{
		num = GetCrossBeam()->GetSelfCount() + 1;
		str.Format("%02d", num);
	}

	if( num == 0 ) return "";

	*/
	return str;
}


CPlateBasicIndex* CPlateBasicIndex::GetBxLeft() const
{
	return m_pGir->GetBx(m_psLeft);
}
CPlateBasicIndex* CPlateBasicIndex::GetBxRight() const
{
	return m_pGir->GetBx(m_psRight);
}

CPlateBasicIndex* CPlateBasicIndex::GetBxMax() const
{
	if(GetStation() > GetBxLeft()->GetStation())
		return (GetStation() > GetBxRight()->GetStation()) ? (CPlateBasicIndex*)this : GetBxRight();

	return (GetBxLeft()->GetStation() > GetBxRight()->GetStation()) ? 
				GetBxLeft() : GetBxRight();
}

CPlateBasicIndex* CPlateBasicIndex::GetBxMin() const
{
	if(GetStation() < GetBxLeft()->GetStation())
		return (GetStation() < GetBxRight()->GetStation()) ? (CPlateBasicIndex*)this : GetBxRight();

	return (GetBxLeft()->GetStation() < GetBxRight()->GetStation()) ? 
				GetBxLeft() : GetBxRight();
}

//입력 : 
//		 
//기능 : 
CJackupStiff* CPlateBasicIndex::GetJackupStiff() const
{			
	return (CJackupStiff*)GetObjectLink(CPlateBxObject::LK_JACKUP);
}

//입력 : nMarkBuje  1= 상판, 2=하판, 3=좌복부판, 4=우복부판
//		 
//기능 : 
CPlateFactChain* CPlateBasicIndex::GetFactChain(long nMarkBujeFact, BOOL bNext) const
{
	if(!GetCreate()) return NULL;

	CTypedPtrArray <CObArray, CPlateFactChain*>	* pFact = NULL;
	long nLK = 0;
	__int64 bxFlag = BX_SPLICE;
	switch(nMarkBujeFact)
	{
	case G_F_U :	nLK = CPlateBxObject::LK_FACTCHAIN_UPPER;
				pFact = m_pGir->m_aFactChain;
				bxFlag |= BX_STT_GIRDER|BX_UPPERFLANGE;
				break;
	case G_F_L :	nLK = CPlateBxObject::LK_FACTCHAIN_LOWER;
				pFact = m_pGir->m_aFactChain+1;
				bxFlag |= BX_STT_GIRDER|BX_LOWERFLANGE;
				break;
	case G_W :	nLK = CPlateBxObject::LK_FACTCHAIN_WEB;
				pFact = m_pGir->m_aFactChain+2;
				bxFlag |= BX_STT_GIRDER|BX_WEB;
				break;	
	}


	if(bNext && m_nObjLinkSeq[nLK] >= 0)	// 공장이음 시작위치와 겹치는 경우
		return (CPlateFactChain*)GetObjectLink(nLK);

 
	// 이전 Bx 위치에서 찾기
	ASSERT( pFact );

	long nBound = 0;
	CPlateBxFinder Finder(m_pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag);
	while( pBx )
	{
		if(pBx->m_dStation >= m_dStation)
			break;
		if(pBx->m_nObjLinkSeq[nLK] >= 0)
			nBound = pBx->m_nObjLinkSeq[nLK];
		pBx = Finder.GetBxNext();
	}

	return pFact->GetSize() > nBound ? pFact->GetAt(nBound) : NULL;
}

//입력 : 
//		 
//기능 : 
CWebHStiff* CPlateBasicIndex::GetWebHStiff() const
{	
	return (CWebHStiff*)GetObjectLink(CPlateBxObject::LK_WEBHSTIFF);
}

//입력 : 
//		 
//기능 : 
BOOL CPlateBasicIndex::CheckSelfData(BOOL bAssertUse)
{
	RETURN_ASSERT(m_pGir, bAssertUse,"m_pGir")

	// Check LinkToBx
	for(long n = 0; n < LINKOBJECTSU; n++)
	{
		if(m_nObjLinkSeq[n] >= 0)
		{
			CString sCurrent;
			switch( n )
			{
			case CPlateBxObject::LK_SPLICE :  sCurrent = "LK_SPLICE In pBx"; break;
			case CPlateBxObject::LK_CROSSBEAM : sCurrent = "LK_CROSSBEAM In pBx"; break;
			case CPlateBxObject::LK_FACTCHAIN_UPPER: sCurrent = "LK_FACTCHAIN_UPPER In pBx"; break;
			case CPlateBxObject::LK_FACTCHAIN_LOWER: sCurrent = "LK_FACTCHAIN_LOWER In pBx"; break;
			case CPlateBxObject::LK_FACTCHAIN_WEB: sCurrent = "LK_FACTCHAIN_LEFT In pBx"; break;			
			case CPlateBxObject::LK_WEBHSTIFF: sCurrent = "LK_WEBHSTIFF In pBx"; break;
			case CPlateBxObject::LK_JACKUP: sCurrent = "LK_JACKUP In pBx"; break;
			case CPlateBxObject::LK_JIJUM_VSTIFF: sCurrent = "LK_JIJUM_STIFF In pBx"; break;
			}

			CPlateBxObject* pLK = (CPlateBxObject*)GetObjectLink(n);
			RETURN_ASSERT( pLK , bAssertUse,sCurrent)
			CPlateBasicIndex *pBx = pLK->GetBx();
			DWORD dwLinkType = pLK->GetLinkType();
			RETURN_ASSERT( this == pBx, bAssertUse,sCurrent)
		}
	}

	// Check m_psSelf
	RETURN_ASSERT(m_psSelf, bAssertUse,"pBxSelf")
	RETURN_ASSERT(this == m_pGir->GetBx( m_psSelf ), bAssertUse,"this == m_pGir->GetBx( m_psSelf )")

	// Check m_psCenter
//	RETURN_ASSERT(m_psCenter, bAssertUse,"pBxCenter")

	return TRUE;
}
 
void CPlateBasicIndex::ContextOutBx(CString& sContext) const
{
	// station
	CString sFmt;
	sFmt.Format("%10.lf  ",m_dStation);			sContext += sFmt;

	// flag
	if(IsState(BX_STT_JOINT))  sContext += "BX_STT_JOINT, ";
	if(IsState(BX_STT_SLAB))   sContext += "BX_STT_SLAB, ";
	if(IsState(BX_STT_GIRDER)) sContext += "BX_STT_GIRDER, ";
	if(IsState(BX_END_JOINT))  sContext += "BX_END_JOINT, ";
	if(IsState(BX_END_SLAB))   sContext += "BX_END_SLAB, ";
	if(IsState(BX_END_GIRDER)) sContext += "BX_END_GIRDER, ";
	if(IsState(BX_STT_BENDING_LOWER)) sContext += "BX_STT_BENDING_LOWER, ";
	if(IsState(BX_STT_BENDING_UPPER)) sContext += "BX_STT_BENDING_UPPER, ";
	if(IsState(BX_END_BENDING_LOWER)) sContext += "BX_END_BENDING_LOWER, ";
	if(IsState(BX_END_BENDING_UPPER)) sContext += "BX_END_BENDING_UPPER, ";
	if(IsState(BX_STT_BYUN_UPPER)) sContext += "BX_STT_BYUN_UPPER, ";
	if(IsState(BX_STT_BYUN_LOWER)) sContext += "BX_STT_BYUN_LOWER, ";
	if(IsState(BX_END_BYUN_LOWER)) sContext += "BX_END_BYUN_LOWER, ";
	if(IsState(BX_END_BYUN_UPPER)) sContext += "BX_END_BYUN_UPPER, ";
	if(IsState(BX_JACKUP)) sContext += "BX_JACKUP, ";
	if(IsState(BX_CROSSBEAM_VBRACING)) sContext += "BX_CROSSBEAM_VBRACING, ";
	if(IsState(BX_UPPERFLANGE)) sContext += "BX_UPPERFLANGE, ";
	if(IsState(BX_LOWERFLANGE)) sContext += "BX_LOWERFLANGE, ";
	if(IsState(BX_WEB)) sContext += "BX_WEB, ";
	if(IsState(BX_SPLICE)) sContext += "BX_SPLICE, ";
	if(IsState(BX_JIJUM)) sContext += "BX_JIJUM, ";
	if(IsState(BX_LINE_P)) sContext += "BX_LINE_P, ";
	if(IsState(BX_LINE_V)) sContext += "BX_LINE_V, ";
	if(IsState(BX_LINE_VPYUN)) sContext += "BX_LINE_VPYUN, ";

	sContext += "\r\n                      ";

	for(long n = 0; n < LINKOBJECTSU; n++)
	{
		long k = m_nObjLinkSeq[n];
		if(k >= 0)
		{
			switch( n )
			{
			case CPlateBxObject::LK_SPLICE			: sFmt.Format("LK_SPLICE[%ld], ",k); break;
			case CPlateBxObject::LK_CROSSBEAM		: sFmt.Format("LK_CROSSBEAM[%ld], ",k); break;
			case CPlateBxObject::LK_FACTCHAIN_UPPER : sFmt.Format("LK_FACTCHAIN_UPPER[%ld], ",k); break;
			case CPlateBxObject::LK_FACTCHAIN_LOWER : sFmt.Format("LK_FACTCHAIN_LOWER[%ld], ",k); break;
			case CPlateBxObject::LK_FACTCHAIN_WEB	: sFmt.Format("LK_FACTCHAIN_LEFT[%ld], ",k); break;			
			case CPlateBxObject::LK_WEBHSTIFF		: sFmt.Format("LK_WEBHSTIFF_CENTER[%ld], ",k); break;
			case CPlateBxObject::LK_JACKUP			: sFmt.Format("LK_JACKUP[%ld], ",k); break;			
			case CPlateBxObject::LK_JIJUM_VSTIFF	: sFmt.Format("LK_JIJUM_STIFF[%ld], ",k); break;			
			}
			sContext += sFmt;
		}
	}
	
	sContext += "\r\n";

}

long CPlateBasicIndex::GetNumberGirder() const
{
	CPlateGirderApp* pGir = GetGirder();
	CPlateBridge* pDB = pGir->GetBridgeApp();
	long nG = 0;
	for(nG = -1; nG < pDB->GetGirdersu(); nG++)
		if(pGir == pDB->GetGirder(nG)) break;

	return nG;
}


//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBasicIndex::GetBxMatchByVert(CPlateGirderApp *pGir, __int64 bxFlag/*=NULL*/) const
{
	if(pGir == NULL)	return NULL;
	CLineInfo *pLineSec = pGir->GetLine();
	double sta = GetStation();

	if( GetGirder()->m_pLine != pLineSec )
	{
		CDPoint xy = GetGirder()->GetXyGirderDis(sta);
		pLineSec->GetStationMatchVerticalByXy(xy, sta);
	}

	return pGir->GetBxByStation(sta,bxFlag);
}

//입력 : 
//		 
//기능 : 
double CPlateBasicIndex::GetStationByScInBx(double dDisGir) const
{
	return GetGirder()->GetStationBySc(GetStation(),dDisGir,GetAngle());
}

BOOL CPlateBasicIndex::IsCrossBeamRight() const
{
	if(GetSection()->GetCrossBeam() == NULL && GetSection()->GetVBracing() == NULL)	return FALSE;

	return TRUE;
}

BOOL CPlateBasicIndex::IsCrossBeamLeft() const
{
	if(GetCrossBeamLeft() == NULL)	return FALSE;
	return TRUE;
}

long CPlateBasicIndex::GetNumberCrossBeam() const
{
	if(IsCrossBeamRight())		return GetSection()->GetSelfCount();
	return -1;
}

//입력 : nIdx
//기능 : 현재 인덱스의 실제 박스 높이
double CPlateBasicIndex::GetHeightGirderByBx(BOOL bReal) const
{
	CPlateGirderApp *pGir = GetGirder();
	return pGir->GetHeightGirderByStaAng(GetStation(),0,CDPoint(0,1),bReal);
}

BOOL CPlateBasicIndex::IsJijum() const
{
	return IsPier() || IsAbut();
}

//입력 : bUpper
//기능 : bUpper가 압축부인지를 점검
BOOL CPlateBasicIndex::IsCompressByJRib(BOOL bUpper) const
{
//	CJRib* pJ = GetJRib(bUpper);
//	return  pJ ? pJ->IsPress() : 0;
	return TRUE;
}

//입력 : bUpper
//		 bDefaultPress  TRUE =현재 pBx의 전,후 중에서 1곳이라도 압축부이면 압축부
//기능 : bUpper가 압축부인지를 점검
BOOL CPlateBasicIndex::IsCompressByJRibBoth(BOOL bUpper) const
{
	return GetGirder()->m_pRangePress->IsCompressOnBox(bUpper, (CPlateBasicIndex*)this);		
}

//입력 : bUpper 
//		 bDefaultPress  TRUE =현재 pBx의 전,후 중에서 1곳이라도 압축부이면 압축부
//기능 : 리턴 값 1=상부압축, 2=하부압축, 3=교번부 리턴
long CPlateBasicIndex::GetCompressTypeByComplex() const
	{
	BOOL bUPress = GetGirder()->m_pRangePress->IsCompressOnBox(TRUE, (CPlateBasicIndex*)this);		

	BOOL bLPress = GetGirder()->m_pRangePress->IsCompressOnBox(FALSE, (CPlateBasicIndex*)this);

	if(bUPress && !bLPress)			return 1;
	else if(!bUPress && bLPress)	return 2;
	else							return 3;
}

long CPlateBasicIndex::GetNumberGirderEx() const
{
	return GetNumberGirder() + 
		((CPlateBridgeApp*)GetGirder()->GetBridgeApp())->GetNumberStartGirder();
}

// 용접타입 제외
long CPlateBasicIndex::GetNumberSpliceEx() const
{
	long nCount = 0;
	CPlateBxFinder Finder(GetGirder());
	CPlateBasicIndex* p = Finder.GetBxFirst(BX_SPLICE);
	while(p)
	{
		if(GetStation() <= p->GetStation())
			break;
		if(p->GetSplice()->m_bSpliceType)
			nCount++;
		p = Finder.GetBxNext();
	}

	return nCount + ((CPlateBridgeApp*)GetGirder()->GetBridgeApp())->GetNumberStartSplice();
//	return GetNumberSplice() +  
//		((CPlateBridgeApp*)GetGirder()->GetBridgeApp())->GetNumberStartSplice();

}

long CPlateBasicIndex::GetLinePTType() const
{
	double dStaBTC = GetGirder()->GetLine()->GetStationByPointLineInfo(GetStation(),(enum LINE_POINT)CLineSplit::PT_BTC_SP_KAA);
	double dStaBC  = GetGirder()->GetLine()->GetStationByPointLineInfo(GetStation(),(enum LINE_POINT)CLineSplit::PT_BC_PC_KAA);
	double dStaEC  = GetGirder()->GetLine()->GetStationByPointLineInfo(GetStation(),(enum LINE_POINT)CLineSplit::PT_EC_CP_KAE);
	double dStaETC = GetGirder()->GetLine()->GetStationByPointLineInfo(GetStation(),(enum LINE_POINT)CLineSplit::PT_ETC_PS_KAE);

	if(GetStation() == dStaBTC) return CLineSplit::BTC;
	if(GetStation() == dStaBC)  return CLineSplit::BC;
	if(GetStation() == dStaEC)  return CLineSplit::EC;
	if(GetStation() == dStaETC) return CLineSplit::ETC;

	ASSERT(TRUE);
	return -1;
}


//입력
//기능	Splice 번호를 리턴 (0, 1, 2...)
//       =>스플라이스 위치와 동일하면 이전 스플라이스 번호를 리턴
//       =>마지막 현장이음를 지난 구간은 총 현장이음 수를 리턴 
long CPlateBasicIndex::GetNumberSplice() const
{
	long nCount = 0;
	CPlateBxFinder Finder(GetGirder());
	CPlateBasicIndex* p = Finder.GetBxFirst(BX_SPLICE);
	while(p)
	{
		if(GetStation() <= p->GetStation())
			return nCount;
		nCount++;
		p = Finder.GetBxNext();
	}

	return nCount;
}

CPlateSplice* CPlateBasicIndex::GetSplice() const
{
	CPlateSplice *pSp = (CPlateSplice*)GetObjectLink(CPlateBxObject::LK_SPLICE);
	if(pSp)
		pSp->m_pBx = (CPlateBasicIndex *)this;
	return pSp;
}

//입력 : 
//		 
//기능 : 
//CPlateBasicIndex* CPlateBasicIndex::GetBxCenter() const
//{
//	if(!GetCreate()) return NULL;
//	return m_pGir->GetBx(GetPOSCenter());
//}

//입력 : 
//		 
//기능 : 
void CPlateBasicIndex::Serialize(CArchive& ar)
{
//	long nFlag = 0;
	long nFlag = 1;	// TU연속교 교번구간체크 m_bShift 추가

	if(ar.IsStoring())
	{
		ar << nFlag;					
		ar << m_dStation;			
		ar << m_vAngle;
		ar << m_nMatchCol;
		ar << m_dwFlag;
		Serialint64(ar, m_BxFlag);
		ar << LINKOBJECTSU;
		for(long n = 0; n < LINKOBJECTSU; n++)
			ar << m_nObjLinkSeq[n];
		ar << m_bShift;
	}
	else
	{
		ar >> nFlag;
		ar >> m_dStation;
		ar >> m_vAngle;
		ar >> m_nMatchCol;
		ar >> m_dwFlag;
		Serialint64(ar, m_BxFlag);
		long nLINKOBJECTSU;
		ar >> nLINKOBJECTSU;
		for(long n = 0; n < nLINKOBJECTSU; n++)
			ar >> m_nObjLinkSeq[n];
		if(nFlag>0)
			ar >> m_bShift;
	}
}

//입력 : 
//		 
//기능 : 
CPlateBxObject* CPlateBasicIndex::GetObjectLink(long enumLK) const
{
	long nBound = m_nObjLinkSeq[enumLK];

	if(!GetCreate() || nBound < 0) return NULL;

	CPlateLinkToBxArray * pArr = m_pGir->GetLKArr(enumLK);
	if(!pArr || pArr->GetSize() <= nBound) return NULL;
	return (CPlateBxObject*)pArr->GetAt(nBound);

}

void CPlateBasicIndex::SetJustModified(BOOL bModify/* = TRUE*/)
{
	SetBitFlag32(m_dwFlag,BIT_LK_JUSTMODIFIED,bModify);
}

BOOL CPlateBasicIndex::IsJustModified() const 
{
	return GetBitFlag32(m_dwFlag,BIT_LK_JUSTMODIFIED);
}

void CPlateBasicIndex::SetModify(BOOL bModify/* = TRUE*/)
{
	SetBitFlag32(m_dwFlag,BIT_LK_MODIFY,bModify);
}

BOOL CPlateBasicIndex::IsModify() const 
{
	return GetBitFlag32(m_dwFlag,BIT_LK_MODIFY);
}

// Access method
void CPlateBasicIndex::SetStation(double sta)
{
	SETDATA(m_dStation,sta)
}
void CPlateBasicIndex::SetAngle(const CDPoint& vAng)
{
	if(vAng.y < 0)
	{
		SETDATA(m_vAngle,-vAng)
	}
	else
	{
		SETDATA(m_vAngle,vAng)
	}
}
void CPlateBasicIndex::SetMatchCol(long nMatchCol)
{
	SETDATA(m_nMatchCol,nMatchCol)
}

void CPlateBasicIndex::SetPOSLeft(POS ps)
{
	SETDATA(m_psLeft,ps)
}
void CPlateBasicIndex::SetPOSRight(POS ps)
{
	SETDATA(m_psRight,ps)
}

/*
void CPlateBasicIndex::SetPOSCenter(POS ps)
{
	SETDATA(m_psCenter,ps)
}
*/

void CPlateBasicIndex::SetDataFilled(BOOL bFill/* = TRUE*/)
{
	SetBitFlag32(m_dwFlag,BIT_LK_FILL,bFill);
}

BOOL CPlateBasicIndex::IsDataFilled() const 
{
	return GetBitFlag32(m_dwFlag,BIT_LK_FILL);
}

//입력 : 
//		 
//기능 : 
CPlateGirderApp* CPlateBasicIndex::GetCreate() const
{
	return m_pGir;
}



//입력 : 
//		 
//기능 : 
void CPlateBasicIndex::SetPOSSelf(POS psSelf)
{
	m_psSelf = psSelf;
	m_psLeft = psSelf;
	m_psRight = psSelf;	
}

//입력 : 
//		 
//기능 : 
void CPlateBasicIndex::Create(CPlateGirderApp* pGir)
{
	m_pGir = pGir;

	for(long n = 0; n < LINKOBJECTSU; n++)
		SetObjLinkSeq(n, m_nObjLinkSeq[n]);


}

//입력 : 
//		 
//기능 : 
void CPlateBasicIndex::SetObjLinkSeq(long lkType, long nSeq)
{
	// 이전 객체를 가르키는 POS 초기화
	CPlateBxObject* pLk = (CPlateBxObject*)GetObjectLink( lkType ) ;
	if( pLk && pLk->m_psBx && GetCreate())	
		if(m_pGir->GetBx(pLk->m_psBx) == this)
			pLk->m_psBx = NULL;

	m_nObjLinkSeq[lkType] = nSeq;

	// 현재 객체를 가르키는 POS SET
	if( nSeq >= 0 )
	{
		CPlateBxObject* p = (CPlateBxObject*)GetObjectLink( lkType ) ;
		if( p )	p->SetPOS(this);
	}

}

CPlateBasicIndex& CPlateBasicIndex::operator=(const CPlateBasicIndex& obj)
{
	if(this == &obj) return *this;
	m_nMatchCol		= obj.m_nMatchCol;
	m_dStation		= obj.m_dStation;

	m_psLeftCross	= obj.m_psLeftCross;
	m_psRightCross	= obj.m_psRightCross;
	m_psLeft		= obj.m_psLeft;
	m_psRight		= obj.m_psRight;
//	m_psCenter		= obj.m_psCenter;
	m_psSelf		= obj.m_psSelf;
	m_dwFlag		= obj.m_dwFlag;
	m_bShift		= obj.m_bShift;	
	
	m_BxFlag		= obj.m_BxFlag;
	m_pGir			= obj.m_pGir;

	m_vAngle = obj.m_vAngle;
	for(long n = 0; n < LINKOBJECTSU; n++)
	{
		SetObjLinkSeq(n, obj.m_nObjLinkSeq[n]);
	}

	return *this;
}

void CPlateBasicIndex::SetState(__int64 bxFlag, BOOL bApply)
{
	if(bApply)	
		m_BxFlag |= bxFlag;	
	else
	{
		if(GetCreate())
		{
			// Link 객체 해제
			if( bxFlag & BX_CROSSBEAM_VBRACING )
			{
				SetObjLinkSeq(CPlateBxObject::LK_CROSSBEAM,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_CROSSBEAM] = -1;
				SetObjLinkSeq(CPlateBxObject::LK_HBRACING,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_HBRACING] = -1;				
			}
			if( bxFlag & BX_SPLICE )
			{
				SetObjLinkSeq(CPlateBxObject::LK_SPLICE,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_SPLICE] = -1;
			}
			if( bxFlag & (BX_STT_GIRDER | BX_UPPERFLANGE) )
			{
				SetObjLinkSeq(CPlateBxObject::LK_FACTCHAIN_UPPER,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_FACTCHAIN_UPPER] = -1;
			}
			if( bxFlag & (BX_STT_GIRDER | BX_LOWERFLANGE) )
			{
				SetObjLinkSeq(CPlateBxObject::LK_FACTCHAIN_LOWER,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_FACTCHAIN_LOWER] = -1;
			}
			if( bxFlag & (BX_STT_GIRDER | BX_WEB) )
			{
				SetObjLinkSeq(CPlateBxObject::LK_FACTCHAIN_WEB,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_FACTCHAIN_WEB] = -1;
			}
			if( bxFlag & BX_JACKUP )
			{
				SetObjLinkSeq(CPlateBxObject::LK_JACKUP,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_JACKUP] = -1;
			}
			if( bxFlag & BX_JIJUMSTIFF)
			{
				SetObjLinkSeq(CPlateBxObject::LK_JIJUM_VSTIFF,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_JIJUM_VSTIFF] = -1;				
			}
			if( bxFlag & BX_PLATEHSTIFF)
			{
				SetObjLinkSeq(CPlateBxObject::LK_WEBHSTIFF,-1);
				m_nObjLinkSeq[CPlateBxObject::LK_WEBHSTIFF] = -1;
			}			
		}

		m_BxFlag &= ~bxFlag;	
	}
}

void CPlateBasicIndex::CopyLinkVariable(CPlateBasicIndex *pBx)
{
	memcpy(m_nObjLinkSeq,pBx->m_nObjLinkSeq,4 * LINKOBJECTSU);
	m_BxFlag = pBx->m_BxFlag;

}

long CPlateBasicIndex::GetObjLinkSeq(long n) const
{
	ASSERT(n >= 0 && n < LINKOBJECTSU);
	return m_nObjLinkSeq[n]; 
}

// nCheckMatchCr=0 : 가로보 매치에 상관없이 후방1개뒤 BX
// nCheckMatchCr=1 : 가로보 매치가 좌측에 있을 때까지 후방검색
// nCheckMatchCr=2 : 가로보 매치가 우측에 있을 때까지 후방검색
// nCheckMatchCr=3 : 가로보 매치가 양측에 있을 때까지 후방검색
// 주의 : bxFlag=BX_CROSSBEAM일 경우에 nCheckMatchCr이 적용
CPlateBasicIndex* CPlateBasicIndex::GetBxNext(__int64 bxFlag, long nCheckMatchCr)
{
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)this;
	CPlateGirderApp  *pGir= pBx->GetGirder();
	CPlateBridgeApp  *pDB = (CPlateBridgeApp *)pGir->GetBridgeApp();	
	CPlateGirderApp  *pGirLeft = pGir->m_pLeft;
	CPlateGirderApp  *pGirRight= pGir->m_pRight;

	BOOL bMatchLeft  = FALSE;
	BOOL bMatchRight = FALSE;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxComp = Finder.GetBxFirst(bxFlag);
	BOOL bMeetSame = FALSE;
	while(pBxComp)
	{
		if(pBx==pBxComp || bMeetSame)
		{
			bMeetSame = TRUE;

			if(Finder.GetBxFixNext()==NULL)		return NULL;
			if(pGirLeft && nCheckMatchCr!=0)	bMatchLeft  = Finder.GetBxFixNext()->GetBxMatchByCrossBeam(pGirLeft, 1) ? TRUE : FALSE;
			if(pGirRight&& nCheckMatchCr!=0)	bMatchRight = Finder.GetBxFixNext()->GetBxMatchByCrossBeam(pGirRight, 1)? TRUE : FALSE;
			if(pBx->GetNumberGirder()==0)						bMatchLeft = TRUE;
			if(pBx->GetNumberGirder()==pDB->GetGirdersu()-1)	bMatchRight = TRUE;
		
			if( nCheckMatchCr==0
			 ||(nCheckMatchCr==1 && bMatchLeft)  
			 ||(nCheckMatchCr==2 && bMatchRight)  
			 ||(nCheckMatchCr==3 && bMatchLeft && bMatchRight) ) 
				return Finder.GetBxFixNext();
		}
		pBxComp = Finder.GetBxNext();
	}
	return NULL;
}



CDPoint CPlateBasicIndex::GetAngleSectionLeft() const
{
	if(GetSectionLeft() == NULL)	return CDPoint(0,1);
	return GetAngle();
}


CDPoint CPlateBasicIndex::GetAngleSectionRight() const
{
	if(GetSection() == NULL)	return CDPoint(0,1);
	return GetAngle();
}

CDPoint CPlateBasicIndex::GetAngleSection() const
{
	CPlateGirderApp* pGir = (CPlateGirderApp*)GetGirder();
	CPlateBridgeApp *pBridge = (CPlateBridgeApp *)pGir->GetBridgeApp();	
	if(GetNumberGirder()==pBridge->GetGirdersu()-1)
	{
		return GetAngleSectionLeft();
	}
	else
	{
		if(GetBxMatchByCrossBeam(pGir->m_pRight, 1))
			return GetAngleSectionRight();
		else
			return GetAngleSectionLeft();
	}
}

CDPoint CPlateBasicIndex::GetAngleSectionCrossVBracing() const
{
	if(!GetSection())
		return GetGirder()->GetLine()->GetAngleAzimuth(GetStation()).RotateInv90();//GetAngleSection().RotateInv90();
	if(GetSection()->IsenDetType(SECDET_CROSSBEAM))
		return GetSection()->GetCrossBeam()->GetVectorPlan();
	if(GetSection()->IsenDetType(SECDET_VBRACING))
		return GetSection()->GetVBracing()->GetVectorPlan();

	return GetGirder()->GetLine()->GetAngleAzimuth(GetStation()).RotateInv90();//GetAngleSection().RotateInv90();
}

// nCheckMatchCr=0 : 가로보 매치에 상관없이 후방1개뒤 BX
// nCheckMatchCr=1 : 가로보 매치가 좌측에 있을 때까지 후방검색
// nCheckMatchCr=2 : 가로보 매치가 우측에 있을 때까지 후방검색
// nCheckMatchCr=3 : 가로보 매치가 양측에 있을 때까지 후방검색
// 주의 : bxFlag=BX_CROSSBEAM일 경우에 nCheckMatchCr이 적용
CPlateBasicIndex* CPlateBasicIndex::GetBxPrev(__int64 bxFlag, long nCheckMatchCr)
{
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)this;
	CPlateGirderApp  *pGir= pBx->GetGirder();
	CPlateBridgeApp  *pDB = (CPlateBridgeApp *)pGir->GetBridgeApp();	
	CPlateGirderApp  *pGirLeft = pGir->m_pLeft;
	CPlateGirderApp  *pGirRight= pGir->m_pRight;

	BOOL bMatchLeft  = FALSE;
	BOOL bMatchRight = FALSE;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxComp = Finder.GetBxLast(bxFlag);
	BOOL bMeetSame = FALSE;
	while(pBxComp)
	{
		if(pBx==pBxComp || bMeetSame)
		{
			bMeetSame = TRUE;
			if(Finder.GetBxFixNext()==NULL)		return NULL;
			if(pGirLeft && nCheckMatchCr!=0)	bMatchLeft  = Finder.GetBxFixNext()->GetBxMatchByCrossBeam(pGirLeft, 1) ? TRUE : FALSE;
			if(pGirRight&& nCheckMatchCr!=0)	bMatchRight = Finder.GetBxFixNext()->GetBxMatchByCrossBeam(pGirRight, 1)? TRUE : FALSE;
			if(pBx->GetNumberGirder()==0)						bMatchLeft = TRUE;
			if(pBx->GetNumberGirder()==pDB->GetGirdersu()-1)	bMatchRight = TRUE;
		
			if( nCheckMatchCr==0
			 ||(nCheckMatchCr==1 && bMatchLeft)  
			 ||(nCheckMatchCr==2 && bMatchRight)  
			 ||(nCheckMatchCr==3 && bMatchLeft && bMatchRight) ) 
				return Finder.GetBxFixNext();
		}
		pBxComp = Finder.GetBxNext();
	}
	return NULL;
}
// bLeftMathch=TRUE : 좌측에 매치가로보가 있는 경우를 기준으로 검색
// bLeftMathch=FALSE: 우측에 매치가로보가 있는 경우를 기준으로 검색
// 양쪽에 모두 매치되는 가로보가 있는 경우는 bLeftMatch가 영향없음.
// bPrevBx=TRUE : 브레이싱 구역의 시작BX
// bPrevBx=FALSE: 브레이싱 구역의 끝BX
CPlateBasicIndex* CPlateBasicIndex::GetBxCrossBeamByHGuBx(BOOL bLeftMatch, BOOL bPrevBx)
{
	CPlateBasicIndex *pBx      = (CPlateBasicIndex *)this;
	CPlateGirderApp  *pGir     = pBx->GetGirder();
	if(pGir==NULL) return NULL;

	CPlateBridgeApp  *pDB      = (CPlateBridgeApp *)pGir->GetBridgeApp();	
	CPlateGirderApp  *pGirLeft = pGir->m_pLeft;
	CPlateGirderApp  *pGirRight= pGir->m_pRight;
	
	double dSta = pBx->GetStation();
	BOOL bCrossBeamBx = FALSE;
	BOOL bMatchLeft   = FALSE;
	BOOL bMatchRight  = FALSE;
	CPlateBasicIndex *pBxPrev=NULL, *pBxNext=NULL;
	// 이음판의 Bx가 가보로나 수직브레이싱위치에 있는 경우
	if(pBx->GetSection())	
	{
		if( pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) // 가로보거나
		  ||pBx->GetSection()->IsenDetType(SECDET_VBRACING)) // 수직브레이싱위치이면
		  bCrossBeamBx = TRUE;
	}
	if(bPrevBx)// 브레이싱 구역의 시작BX
	{
		// 현재 이음판 bx가 가로보위치의 bx가 아닐경우 현재 bx에서 바로 이전의 가로보bx를 검색해서 대체함
		if(bCrossBeamBx==FALSE)
		{
			pBxPrev = pGir->GetBxByStationDir(dSta, -1, BX_CROSSBEAM_VBRACING);
			pBx = pBxPrev;
		}
		// 우측거더가 있는 상태에서 매치되는 우측가로보가 있는 경우
		if(pGirLeft)	bMatchLeft  = pBx->GetBxMatchByCrossBeam(pGirLeft,  1) ? TRUE : FALSE;
		if(pGirRight)	bMatchRight = pBx->GetBxMatchByCrossBeam(pGirRight, 1) ? TRUE : FALSE;
		// 시작거더는 좌측가로보가 없기 때문에 TRUE설정
		// 마지막 거더는 우측가로가 없기 때문에 TRUE로 설정
		if(pBx->GetNumberGirder()==0)						bMatchLeft  = TRUE;
		if(pBx->GetNumberGirder()==pDB->GetGirdersu()-1)	bMatchRight = TRUE;

		if(bMatchRight && bLeftMatch==FALSE)	// 우쪽 매치되는 가로보가 있고 우측가로보검색 옵션시 리턴
			return pBx;
		else if(bMatchLeft && bLeftMatch)		// 좌쪽 매치되는 가로보가 있을 좌측가로보검색 옵션시 리턴
			return pBx;
		else			//좌측, 우측 매치되는 가로보가 없을 경우는 좌측,우측매치가로보가 있을 때까지 후방검색해서 리턴
			return pBx->GetBxPrev(BX_CROSSBEAM_VBRACING, bLeftMatch?1:2);
	}
	else	 // 브레이싱 구역의 끝BX
	{
		if(bCrossBeamBx==FALSE)
		{
			pBxNext = pGir->GetBxByStationDir(dSta, 1, BX_CROSSBEAM_VBRACING);
			pBx = pBxNext;
		}
		if(pGirLeft)	bMatchLeft  = pBx->GetBxMatchByCrossBeam(pGirLeft, 1) ? TRUE : FALSE;
		if(pGirRight)	bMatchRight = pBx->GetBxMatchByCrossBeam(pGirRight, 1)? TRUE : FALSE;
		// 시작거더는 좌측가로보가 없기 때문에 TRUE설정
		// 마지막 거더는 우측가로가 없기 때문에 TRUE로 설정
		if(pBx->GetNumberGirder()==0)						bMatchLeft  = TRUE;
		if(pBx->GetNumberGirder()==pDB->GetGirdersu()-1)	bMatchRight = TRUE;

		if(bMatchRight && bLeftMatch==FALSE)	// 우쪽 매치되는 가로보가 있고 우측가로보검색 옵션시 리턴
			return pBxNext;
		else if(bMatchLeft && bLeftMatch)		// 좌쪽 매치되는 가로보가 있을 좌측가로보검색 옵션시 리턴
			return pBxNext;
		else
			return pBx->GetBxNext(BX_CROSSBEAM_VBRACING, bLeftMatch?1:2);
	}

}



CString CPlateBasicIndex::GetMarkString()
{
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)this;
	CString sz;
	if(pBx->GetSection())
	{
		CSection *pSec = pBx->GetSection();
		
		if(pBx->IsJijum())
		{
			if(pBx->IsAbut())
				return "A";
			else 
				return "P";
		}
		else
		{
			if(pSec->IsenDetType(SECDET_CROSSBEAM))
				return "C";
			else if(pSec->IsenDetType(SECDET_VSITFF))
				return "V";
			else if(pSec->IsenDetType(SECDET_VBRACING))
				return "B";
		}
	}
	else
	{
		if(pBx->IsState(BX_SPLICE))
		{
//			sz.Format("SP%d", pBx->GetSplice()->GetSelfCount()+1);
			sz = pBx->GetSplice()->GetMarkstring();
			return sz;
		}
		else
		{
			return "";
		}
	}
	return "";
}

long CPlateBasicIndex::GetNumberJijum()
{
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)this;
	
	CPlateBxFinder Finder(pBx->GetGirder());

	CPlateBasicIndex *pBxJijum = Finder.GetBxFirst(BX_JIJUM);
	long nJijum = 0;
	while(pBxJijum)
	{
		if(pBxJijum->IsJijum() && pBx==pBxJijum)
			return nJijum;

		nJijum++;

		pBxJijum = Finder.GetBxNext();
	}

/*
	CPlateBasicIndex *pBxJijum = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nJijum = 0;
	while(pBxJijum)
	{
		if(pBxJijum->IsJijum() && pBx==pBxJijum)
			return nJijum;
		
		if(pBxJijum->IsJijum())
			nJijum++;
		pBxJijum = Finder.GetBxNext();
	}
*/
	return -1;
}

double CPlateBasicIndex::GetOffsetFromLowerFlange()
{
	CPlateGirderApp *pGir = GetGirder();
	CPlateBridgeApp  *pDB = (CPlateBridgeApp*)pGir->GetBridgeApp();
	
	double dHeightGir = GetHeightGirderByBx(TRUE);
	if (pDB->m_nBaseHeightPosHBraceSplice==0)
	{
		return dHeightGir-pDB->m_dBaseHeightFixPosHBraceSplice;
	}
	else if(pDB->m_nBaseHeightPosHBraceSplice==1)
	{
		return pDB->m_dBaseHeightFixPosHBraceSplice;
	}
	else if(pDB->m_nBaseHeightPosHBraceSplice==2)
	{
		return dHeightGir - dHeightGir*pDB->m_dBaseHeightRatioPosHBraceSplice/100;

	}

	return 0;
}

// BX_FLANGE = BX_UPPERFLANGE, BX_LOWERFLANGE, BX_WEB 상판,하판,복부판
BOOL CPlateBasicIndex::IsFiller(__int64 nFlag, BOOL &bNext)	const
{
	if(!(nFlag & BX_FLANGE))	return FALSE;	// 체크하고자 하는 위치가 공장이음이어야 함
	if(!IsState(nFlag))			return FALSE;	// 현재 위치가 공장이음이어야 함
	if(!IsState(BX_SPLICE))		return FALSE;	// 현재 위치가 현장이음이어야 함

	if(nFlag & BX_UPPERFLANGE)
	{
		double dTPre	= GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
		double dTNext	= GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
		bNext = dTPre>dTNext;
		if(dTPre!=dTNext) return TRUE;
	}

	if(nFlag & BX_LOWERFLANGE)
	{
		double dTPre	= GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
		double dTNext	= GetFactChain(G_F_L, TRUE)->m_dFactChainThick;
		bNext = dTPre>dTNext;
		if(dTPre!=dTNext) return TRUE;
	}

	if(nFlag & BX_WEB)
	{
		double dTPre	= GetFactChain(G_W, FALSE)->m_dFactChainThick;
		double dTNext	= GetFactChain(G_W, TRUE)->m_dFactChainThick;
		bNext = dTPre>dTNext;
		if(dTPre!=dTNext) return TRUE;
	}
	return FALSE;
}

BOOL CPlateBasicIndex::IsHybrid() const
{
	return FALSE;	// #APLATE-674 하이브리드 기능 제거

// 	CString szMaterialU = GetFactChain(G_F_U)->m_strSymbol;
// 	CString szMaterialL = GetFactChain(G_F_L)->m_strSymbol;
// 	CString szMaterialW = GetFactChain(G_W)->m_strSymbol;
// 
// 	if(szMaterialU != szMaterialL)	return TRUE;
// 	if(szMaterialU != szMaterialW)	return TRUE;
// 	return FALSE;
}

// 현재 위치가 절점이 존재하는 위치인지?
// 가로보 or 절점을 추가한 현장이음
BOOL CPlateBasicIndex::IsJoint() const
{
	if(IsState(BX_CROSSBEAM_VBRACING))	return TRUE;
	return IsState(BX_SPLICE)&&IsShift() ? TRUE : FALSE;
}