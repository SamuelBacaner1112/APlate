#include "stdafx.h"
#include <afxdlgs.h>
#include <afxtempl.h>
#include "APlateData.h"
#include "PlateGirder.h"

template <class T>
void AhTPASerialForOld(CArchive& ar,void* pDest, T* tv)
{
	CTypedPtrArray < CObArray,T*> * pDT = (CTypedPtrArray < CObArray,T*> *)pDest;
	long nSize = pDT->GetSize();
	if(ar.IsStoring())
	{
		ar << nSize;
	}
	else
	{
		ar >> nSize;
		AhTPAMake(nSize,pDT,tv);
	}

	for(long n = 0; n < nSize; n++)
	{
		T *pArr = (T*)pDT->GetAt(n);
		pArr->SerializeForOld(ar);
	}
}


//////////////////////////////////////////////////////////////////////
// class CPlateGirder

CPlateGirder::CPlateGirder()
{
}

CPlateGirder::CPlateGirder(CPlateBridge* pDB, CLineInfo* pLine, CPlateGirderApp *pLeft/* = NULL */)
{
	m_pSangse		= new CPlateSangSeDo((CPlateGirderApp *)this);
	m_pRangePress	= new CRangePress;
	m_pBridgeApp	= pDB;
	m_pLine			= pLine;
	m_pLeft			= pLeft;
	m_pRight		= NULL;
	m_bEleLevel     = FALSE;
	m_dGirderCenter = 0;
	m_nLineNum		= 0;
	for(long n = 0; n < KYUNGGANSUMAX+1; n++)
		m_dHeightOnJijum[n]	= 0;
	m_nInstallVStiff= 0;
}

CPlateGirder::~CPlateGirder()
{	
	AhPtrListDelete(m_BxList,(CPlateBasicIndex*)0);

	delete m_pRangePress;
	delete m_pSangse;	
}

CPlateGirder& CPlateGirder::operator=(const CPlateGirder& obj)
{
//	AhTPACopy (&m_BracingArr,		&obj.m_BracingArr,		 (CBracing*) 0);
//	AhTPACopy (&m_VStiffSectionArr, &obj.m_VStiffSectionArr, (CSection*) 0);
//	AhTPACopy (&m_CrossSectionArr,  &obj.m_CrossSectionArr,  (CSection*) 0);
//	AhTPACopy (&m_HGussetArr,       &obj.m_HGussetArr,		 (CHBracingGusset*) 0);
//	AhTPACopy (&m_JiJumVStiff,      &obj.m_JiJumVStiff,      (CVStiff*) 0);
//	AhTPACopy (&m_aWebHStiffArr,    &obj.m_aWebHStiffArr,    (CWebHStiff*) 0);
//	AhTPACopy (&m_aJackupArr,       &obj.m_aJackupArr,       (CJackupStiff*) 0);
//	AhTPACopy (&m_pCamber,			&obj.m_pCamber,			 (CCamber*) 0);
//	AhTPACopy (&m_pSplice,			&obj.m_pSplice,			 (CPlateSplice*) 0);
//	AhTPACopy (&m_aJackupArr,		&obj.m_aJackupArr,		 (CJackupStiff*) 0);
//	AhTPACopy (&m_aWebHStiffArr,	&obj.m_aWebHStiffArr,    (CWebHStiff*) 0);
//
//	for(long n = 0; n < 4; n++)	AhTPACopy (m_aFactChain+n, obj.m_aFactChain+n,  (CPlateFactChain*) 0);
//
//	AhTPACopy	 (&m_pCamber,  &obj.m_pCamber,  (CCamber*) 0);
//	AhPtrListCopy(m_BxList,obj.m_BxList,(CPlateBasicIndex*)0);
//
//	*m_pSangse  = *obj.m_pSangse;
//	m_pBridgeApp = obj.m_pBridgeApp;
//	m_pLine      = obj.m_pLine;
//	m_pLeft      = obj.m_pLeft;
//	m_pRight     = obj.m_pRight;
//
//	m_dGirderCenter	= obj.m_dGirderCenter	;	//[횡단구성 입력]
//	m_nLineNum		= obj.m_nLineNum		;
//
//	for(n = 0;n < KYUNGGANSUMAX+1;n++)
//	{
//		m_dHeightOnJijum[n] = obj.m_dHeightOnJijum[n];
//	}

	return *this;
}

void CPlateGirder::Serialize(CArchive& ar)
{
	long nFlag = 2, n;

	if(ar.IsStoring())
	{
		ar << nFlag;
		m_pSangse->SerializeSangse(ar);
		m_pRangePress->Serialize(ar);

		AhTPASerial(ar,&m_pSplice,(CPlateSplice*)0);
		AhTPASerial(ar,&m_aJackupArr,(CJackupStiff*)0);
		AhTPASerial(ar,&m_aWebHStiffArr,(CWebHStiff*)0);		
		AhTPASerial(ar,&m_pCamber,(CCamber*)0);
		for(n = 0; n < 4; n++)	AhTPASerial(ar,m_aFactChain+n,(CPlateFactChain*)0);
		AhTPASerial(ar, &m_BracingArr, (CBracing *)0);
		AhTPASerial(ar, &m_VStiffSectionArr, (CSection *)0);	
		AhTPASerial(ar, &m_CrossSectionArr, (CSection *)0);	
		AhTPASerial(ar, &m_HGussetArr, (CHBracingGusset *)0);
		AhTPASerial(ar, &m_JiJumVStiff, (CVStiff *)0);		
		AhPtrListSerial(ar,m_BxList,(CPlateBasicIndex*)0);

		ar << m_dGirderCenter					;	//[횡단구성 입력]
		ar << m_nLineNum						;
		ar << m_pBridgeApp->m_nQtyJigan;
		for(n = 0; n < m_pBridgeApp->m_nQtyJigan+1; n++)
		{
			ar << m_dHeightOnJijum[n]	;	//[박스제원 입력]
			m_Shoe[n].SerializeShoe(ar);
		}
		ar << m_nInstallVStiff;
	}
	else
	{
		ar >> nFlag;		
		m_pSangse->SerializeSangse(ar);
		m_pRangePress->Serialize(ar);

		AhTPASerial(ar,&m_pSplice,(CPlateSplice*)0);
		AhTPASerial(ar,&m_aJackupArr,(CJackupStiff*)0);
		AhTPASerial(ar,&m_aWebHStiffArr,(CWebHStiff*)0);		
		AhTPASerial(ar,&m_pCamber,(CCamber*)0);
		for(n = 0; n < 4; n++)	AhTPASerial(ar,m_aFactChain+n,(CPlateFactChain*)0);
		
		if(nFlag<1)		AhTPASerialForOld(ar, &m_BracingArr, (CBracing *)0);
		else			AhTPASerial(ar, &m_BracingArr, (CBracing *)0);
		AhTPASerial(ar, &m_VStiffSectionArr, (CSection *)0);
		AhTPASerial(ar, &m_CrossSectionArr, (CSection *)0);
		AhTPASerial(ar, &m_HGussetArr, (CHBracingGusset *)0);
		AhTPASerial(ar, &m_JiJumVStiff, (CVStiff *)0);
		AhPtrListSerial(ar,m_BxList,(CPlateBasicIndex*)0);
		
		ar >> m_dGirderCenter					;	//[횡단구성 입력]
		ar >> m_nLineNum						;
		if(nFlag<2)
		{
			for(n = 0; n < 21; n++)
			{
				ar >> m_dHeightOnJijum[n];	//[박스제원 입력]				
				m_Shoe[n].SerializeShoe(ar);
			}
		}
		else
		{
			long nQtyJigan;
			ar >> nQtyJigan;
			for(n = 0; n < nQtyJigan+1; n++)
			{
				ar >> m_dHeightOnJijum[n];	//[박스제원 입력]				
				m_Shoe[n].SerializeShoe(ar);
			}
		}
		ar >> m_nInstallVStiff;

		SetPOSAndEachPointer();
	}	
}

void CPlateGirder::ReStandVStiff()
{
	CPlateGirderApp *pGir = (CPlateGirderApp*)this;
	CPlateBxFinder Finder(pGir);

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);

	if(!pBx) return;
	CVStiff *pVStiff = pBx->GetSection()->GetVStiff();

	while (pBx)
	{
		pVStiff = pBx->GetSection()->GetVStiff();

		if(pVStiff->m_nFlag > 0)
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		else
			pVStiff->m_nFlag = 1;
			
		double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());

		pVStiff->m_dH[LEFT][DOWN]	= max(dHGir - pVStiff->m_dLeft, 0);
		pVStiff->m_dH[RIGHT][DOWN]	= max(dHGir - pVStiff->m_dRight, 0);

		pBx = Finder.GetBxNext();
	}
}
	
void CPlateGirder::SetPOSAndEachPointer()
{
	// SetLKGirderPointAndSelfCount
	SetLKGirderPointAndSelfCount(&m_BracingArr);
	SetLKGirderPointAndSelfCount(&m_VStiffSectionArr);
	SetLKGirderPointAndSelfCount(&m_CrossSectionArr);
	SetLKGirderPointAndSelfCount(&m_pSplice);
	SetLKGirderPointAndSelfCount(&m_HGussetArr);		
	SetLKGirderPointAndSelfCount(&m_aJackupArr);		
	SetLKGirderPointAndSelfCount(&m_JiJumVStiff);
	SetLKGirderPointAndSelfCount(&m_aWebHStiffArr);

	for(long n = 0; n < 3; n++)	SetLKGirderPointAndSelfCount(m_aFactChain+n);
	
	// Set Bx
	// SetLK SttPOS 의 내용 포함
	POS psPrev, ps = m_BxList.GetHeadPosition();
	while(ps) 
	{
		psPrev = ps;
		CPlateBasicIndex *pBx = (CPlateBasicIndex*)m_BxList.GetNext(ps);
		pBx->Create((CPlateGirderApp*)this);
		pBx->SetPOSSelf( psPrev );
	}

	// SetLK EndPOS
	SetLKEndPOS(m_aFactChain);		// 공장이음위치 입력
 	SetLKEndPOS(m_aFactChain+1);	// 공장이음위치 입력
	SetLKEndPOS(m_aFactChain+2);	// 공장이음위치 입력	
	
	// SetLK Side
	SetLKSideFactChain();	

	// set center position
	SetBxPosLeftRight(BX_STT_GIRDER);
	SetBxPosLeftRight(BX_END_GIRDER);
	// set m_enTypeNum Splice
//	SetEnTypeNumSplice();
}

// CRib, CPlateFactChain, CStringer, OutsideBeam 4개의 클래스와 해당됨
void CPlateGirder::SetLKEndPOS(void* pTPA)
{
	CTypedPtrArray<CObArray, CPlateBxObject*> * pArr = (CTypedPtrArray<CObArray, CPlateBxObject*> *)pTPA;

	// set m_psBxEnd :  < pArr->GetSize()-1
	for(long n = 0; n < pArr->GetSize()-1; n++) 
		pArr->GetAt(n)->SetPOSEnd( pArr->GetAt(n+1)->GetPOS() );	
	if(pArr->GetSize())
		pArr->GetAt(pArr->GetUpperBound())->SetPOSEnd(NULL);
}

void CPlateGirder::SetLKGirderPointAndSelfCount(void* pTPA)
{
	CTypedPtrArray<CObArray, CPlateBxObject*> * pArr = (CTypedPtrArray<CObArray, CPlateBxObject*> *)pTPA;

	// set CPlateGirderApp , SelfCount
	for(long n = 0; n < pArr->GetSize(); n++) 
	{
		CPlateBxObject* pLK = pArr->GetAt(n);
		pLK->Create((CPlateGirderApp*)this,NULL);
		pLK->SetSelfCount(n);
	}
}

void CPlateGirder::SetLKSideFactChain()
{
	for(long i = 0; i < 4; i++)
		for(long n = 0; n < m_aFactChain[i].GetSize(); n++) 
			m_aFactChain[i][n]->m_nMarkBujeFact = i;	

}
CPlateBasicIndex* CPlateGirder::newBx(CPlateBasicIndex *pBx /*= NULL*/)
{
	CPlateBasicIndex* pResult = new CPlateBasicIndex(pBx);
	pResult->Create((CPlateGirderApp*)this);
	return pResult;
}

void CPlateGirder::SetBxPosLeftRight(__int64 bxLeftFlag)
{
	// 확인 - 사용할수 있는 Flag
	__int64 bxCheck = 0;	
	bxCheck |= BX_STT_GIRDER;
	bxCheck |= BX_END_GIRDER;
	bxCheck |= BX_STT_JOINT;
	bxCheck |= BX_END_JOINT;
	bxCheck |= BX_STT_SLAB;
	bxCheck |= BX_END_SLAB;
	bxCheck |= BX_STT_BENDING_LOWER;
	bxCheck |= BX_STT_BENDING_UPPER;
	bxCheck |= BX_END_BENDING_LOWER;
	bxCheck |= BX_END_BENDING_UPPER;
	bxCheck |= BX_JACKUP;
	bxCheck |= BX_CROSSBEAM_VBRACING;
	bxCheck |= BX_JIJUM;
	//bxCheck |= BX_OUTSIDE_CROSS_LEFT;
	//bxCheck |= BX_OUTSIDE_ENDBEAM_LEFT;
	ASSERT( bxLeftFlag & bxCheck );

	__int64 bxRightFlag = bxLeftFlag << 1;
	__int64 bxCenterFlag = bxRightFlag << 1;
	CPlateGirderApp* pGir = (CPlateGirderApp*)this;

	CPlateBxFinder FLeft((CPlateGirderApp*)this);
	CPlateBxFinder FRight((CPlateGirderApp*)this);
	CPlateBxFinder FCenter((CPlateGirderApp*)this);

	POS psLeft = FLeft.GetPOSFirst(bxLeftFlag);
	POS psRight = FRight.GetPOSFirst(bxRightFlag);
	POS psCenter = FCenter.GetPOSFirst(bxCenterFlag);

	while(psCenter)
	{
		if(psLeft && psRight)
		{
			CPlateBasicIndex *pBxLeft = pGir->GetBx(psLeft);
			CPlateBasicIndex *pBxRight = pGir->GetBx(psRight);
			//CPlateBasicIndex *pBxCenter = pGir->GetBx(psCenter);

			pBxLeft->SetPOSLeft(psLeft);
			pBxLeft->SetPOSRight(psRight);
			//pBxLeft->SetPOSCenter(psCenter);

			pBxRight->SetPOSLeft(psLeft);
			pBxRight->SetPOSRight(psRight);
			//pBxRight->SetPOSCenter(psCenter);

			//pBxCenter->SetPOSLeft(psLeft);
			//pBxCenter->SetPOSRight(psRight);
			//pBxCenter->SetPOSCenter(psCenter);

			psLeft = FLeft.GetPOSNext();
			psRight = FRight.GetPOSNext();
		}
		psCenter = FCenter.GetPOSNext();		
	}

}


CPlateLinkToBxArray * CPlateGirder::GetLKArr(long nLK) const
{
	CPlateLinkToBxArray * pArr = NULL;
	switch( nLK )
	{
		case CPlateBxObject::LK_SPLICE :		  pArr = (CPlateLinkToBxArray*)&m_pSplice; break;
		case CPlateBxObject::LK_FACTCHAIN_UPPER : pArr = (CPlateLinkToBxArray*)(m_aFactChain); break;
		case CPlateBxObject::LK_FACTCHAIN_LOWER : pArr = (CPlateLinkToBxArray*)(m_aFactChain+1); break;
		case CPlateBxObject::LK_FACTCHAIN_WEB :	  pArr = (CPlateLinkToBxArray*)(m_aFactChain+2); break;	
		case CPlateBxObject::LK_WEBHSTIFF :	      pArr = (CPlateLinkToBxArray*)&m_aWebHStiffArr; break;
		case CPlateBxObject::LK_JACKUP :		  pArr = (CPlateLinkToBxArray*)&m_aJackupArr; break;	
		case CPlateBxObject::LK_BRACING :	      pArr = (CPlateLinkToBxArray*)&m_BracingArr; break;
		case CPlateBxObject::LK_VSTIFF :	      pArr = (CPlateLinkToBxArray*)&m_VStiffSectionArr; break;
		case CPlateBxObject::LK_CROSSBEAM :	      pArr = (CPlateLinkToBxArray*)&m_CrossSectionArr; break;
		case CPlateBxObject::LK_HBRACING :	      pArr = (CPlateLinkToBxArray*)&m_HGussetArr; break;				
		case CPlateBxObject::LK_JIJUM_VSTIFF:     pArr = (CPlateLinkToBxArray*)&m_JiJumVStiff; break;			
	}

	return pArr;
}

void CPlateGirder::SetLKClearPOS(long nLK)
{
	CTypedPtrArray<CObArray, CPlateBxObject*> * pArr = GetLKArr(nLK);
	for(long n = 0; n < pArr->GetSize(); n++)
		pArr->GetAt(n)->ClearPOS();

	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(NULL);
	while( pBx )
	{
		pBx->SetObjLinkSeq(nLK, -1);
		pBx = Finder.GetBxNext();
	}
	
}

BOOL CPlateGirder::CheckSelfData(BOOL bAssertUse)
{
	CPlateBxFinder Finder((CPlateGirderApp*)this);
	CPlateBasicIndex * pBx;

	if(m_BxList.GetCount() > 0)
	{
		Finder.GetBxFirst(BX_STT_GIRDER);
		RETURN_ASSERT( Finder.GetCountBx() == 1,bAssertUse,"BX_STT_GIRDER !=1")
		Finder.GetBxFirst(BX_END_GIRDER);
		RETURN_ASSERT( Finder.GetCountBx() == 1,bAssertUse,"BX_END_GIRDER!=1")
		Finder.GetBxFirst(BX_STT_JOINT);
		RETURN_ASSERT( Finder.GetCountBx() == 1,bAssertUse,"BX_STT_JOINT!=1")
		Finder.GetBxFirst(BX_END_JOINT);
		RETURN_ASSERT( Finder.GetCountBx() == 1,bAssertUse,"BX_END_JOINT!=1")
		Finder.GetBxFirst(BX_STT_SLAB);
		RETURN_ASSERT( Finder.GetCountBx() == 1,bAssertUse,"BX_STT_SLAB!=1")
		Finder.GetBxFirst(BX_END_SLAB);
		RETURN_ASSERT( Finder.GetCountBx() == 1,bAssertUse,"BX_END_SLAB!=1")
	}

	// Check Center position
	__int64 bxFlag = 
		BX_STT_GIRDER| BX_END_GIRDER |
		BX_STT_JOINT | BX_END_JOINT  |
		BX_STT_SLAB  | BX_END_SLAB   | 
		BX_STT_BENDING_UPPER | BX_STT_BENDING_LOWER |
		BX_END_BENDING_UPPER | BX_END_BENDING_LOWER | 
		BX_JACKUP | BX_CROSSBEAM_VBRACING;

	pBx = Finder.GetBxFirst(bxFlag);
	while( pBx )
	{
		//RETURN_ASSERT( pBx->GetPOSCenter() , bAssertUse,"pBx->m_psCenter")
		pBx = Finder.GetBxNext();
	}

	// Check POS in Each CPlateBxObject	pLB;
	long nCountLK = 0;
	CPlateLinkToBxArray * pArr;
	CString sFmt;
	while( pArr = GetLKArr(nCountLK++) )
	{		
		if(nCountLK >= CPlateBxObject::LK_WEBHSTIFF) break;
		for(long n = 0; n < pArr->GetSize(); n++)
		{
			CPlateBxObject * pLB = pArr->GetAt(n);
			// 1. LinkToBx
			if(pLB->GetPOS() == NULL)
			{
				sFmt.Format("CPlateBxObject::xxxArr[%ld] Link to Bx",n);
				RETURN_ASSERT( FALSE, bAssertUse, sFmt)
			}
		}

	}

	return TRUE;
}

void CPlateGirder::SetEnTypeNumSplice()
{
	for(long n = 0;n < m_pSplice.GetSize(); n++)
	{
		m_pSplice[n]->m_enTypeNum = n;
	}
}


BOOL CPlateGirder::IsJustModified() const
{
	CPlateLinkToBxArray * pArr;
	long nLK = 0;
	while( pArr = GetLKArr(nLK++))
	{
		 for(long n = 0; n < pArr->GetSize(); n++)
			 if(pArr->GetAt(n)->IsJustModified()) return TRUE;
	}
	return FALSE;
}

BOOL CPlateGirder::IsModify() const
{
	CPlateLinkToBxArray * pArr;
	long nLK = 0;
	while( pArr = GetLKArr(nLK++))
	{
		 for(long n = 0; n < pArr->GetSize(); n++)
			 if(pArr->GetAt(n)->IsModify()) return TRUE;
	}
	return FALSE;
}

BOOL CPlateGirder::IsDataFilled() const
{
	CPlateLinkToBxArray * pArr;
	long nLK = 0;
	while( pArr = GetLKArr(nLK++))
	{
		 for(long n = 0; n < pArr->GetSize(); n++)
			 if(pArr->GetAt(n)->IsDataFilled()==FALSE) return FALSE;
	}
	return TRUE;
}


void CPlateGirder::SetJustModified(BOOL bModify)
{
	CPlateLinkToBxArray * pArr;
	long nLK = 0;
	while( pArr = GetLKArr(nLK++))
	{
		 for(long n = 0; n < pArr->GetSize(); n++)
			 pArr->GetAt(n)->SetJustModified(bModify);
	}
}

// 현재 거더의 선형이 가장 좌측 또는 가장 우측 거더의 선형과 동일한 지를 점검
BOOL CPlateGirder::IsLineSameLeftOrRight() const
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pBridgeApp;
	return (pDB->GetGirder(0)->GetLine() == m_pLine || pDB->GetGirder(pDB->GetGirdersu()-1)->GetLine() == m_pLine);	//가장 좌우측 거더선형이면 참
}
