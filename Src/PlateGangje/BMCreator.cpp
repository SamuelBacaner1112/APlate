// BMCreator.cpp: implementation of the CBMCreator class.
//
//////////////////////////////////////////////////////////////////////
 
#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "GangjeBase.h"
#include "BMRecord.h" 
#include "BMCreator.h"
#include "TStandard.h"
 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif  
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
   
CBMCreator::CBMCreator(CDataManage *pMng)
{
	m_pDataManage = pMng;
	m_pDB = pMng->GetBridge();
 
	m_nWorkValue = 0;			// 0:생성, 1:tbl파일로드, 2:액셀파일로드
	m_nDimType	 = DIM_RECT;		// DimType
	m_nMakeFlag  = MAKE_BM_SANG;	// MakeFlag

	m_dwMakeMarkFlag = 0;
	m_bUnitMode		 = FALSE;

	//SetMakeMark(CMarkSerial::MARK_BUJE,TRUE);
	//SetMakeMark(CMarkSerial::MARK_SECTION,TRUE);
	//SetMakeMark(CMarkSerial::MARK_ASSEM,TRUE);
	//SetMakeMark(CMarkSerial::MARK_ASSEM_MID,TRUE);
	//SetMakeMark(CMarkSerial::MARK_ASSEM_BIG,TRUE);

	m_TotalWeight = 0.0;



}

CBMCreator::~CBMCreator()
{
	ClearAll();
	AhPtrListDelete(m_BMList_StandardSang,(CBMRecord*)0);

}
void CBMCreator::SetMakeMark(long nMAKE_MARK,BOOL bApply)
{
	SetBitFlag32(m_dwMakeMarkFlag,nMAKE_MARK,bApply);
}

BOOL CBMCreator::IsMakeMark(long nMAKE_MARK) const
{
	return GetBitFlag32(m_dwMakeMarkFlag,nMAKE_MARK);
}
void CBMCreator::ClearBase()
{
	AhPtrListDelete(m_BMStardardList,(CBMRecord*)0);
}

void CBMCreator::ClearAll()
{ 
	// 강제집계표 제작용
	AhPtrListDelete(m_BMList_Sang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_StandardSang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_StandardShop,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_StandardShopType2,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_Thick,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_ThickSang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_BujeMarkSang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_BujeMarkTotal,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_BujeMarkDeduct,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_Car,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_CarTotal,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_GrandAssemblySang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_GrandAssemblyTotal,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_MidAssemblySang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_MidAssemblyTotal,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_AssemblySang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_AssemblyTotal,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_AssemblyDeduct,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_AssemblyFact,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_AssemblyField,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_SectionComp,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_BujeComp,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_BasicComp,(CBMRecord*)0);
											
	// 강제집계표 설계용	
	AhPtrListDelete(m_BMList_SectionType,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_Buje,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_BujeTotal,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_DeductSang,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_DeductBuje,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_Girder,(CBMRecord*)0);
	AhPtrListDelete(m_BMList_BujeKindTotal,(CBMRecord*)0);

	////////////////////////////////////////////////////////////////////////
	AhPtrListDelete(m_BMUnitList,(CBMRecord*)0);
	AhPtrListDelete(m_BMGangjeList,(CBMRecord*)0);
}

BOOL CBMCreator::SameAndIncludeRecord(CPtrList* pList, const CBMRecord* pRec,DWORD exceptionFlag/*=0*/)
{
	BOOL bSameAndInclude = FALSE;
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		CBMRecord * pSouRec = (CBMRecord *)pList->GetNext(pos);
		if( pRec->IsSameRecord( pSouRec,exceptionFlag ) )
		{
			pSouRec->m_nQty			 += pRec->m_nQty;
			pSouRec->m_dTWeight		 += pRec->m_dTWeight;
			pSouRec->m_dTWeightBasic += pRec->m_dTWeightBasic;
			bSameAndInclude = TRUE;
			break;
		}

	}

	return bSameAndInclude;
}


CBMRecord* CBMCreator::AddBMRecord(CBMRecord* pRec /*=NULL*/)
{
	pRec = new CBMRecord(pRec);
	pRec->m_nCountMake = m_BMStardardList.GetCount();

	if(m_bUnitMode)
		m_BMUnitList.AddTail(pRec);		// 단위재료표
	else
		m_BMStardardList.AddTail(pRec);	// 강재집계표

	return pRec;
}

CString CBMCreator::GetMarkBx(long nMarkType,long nMark,CPlateBasicIndex* pBx,long nRow,long nCol, BOOL bFront) const
{
	/*
	if(!IsMakeMark(nMarkType)) return _T("");

	switch(nMarkType)
	{
	case CMarkSerial::MARK_BUJE :	
		return m_pDB->GetMarkBuje(nMark,pBx,nRow,nCol,bFront);
	case CMarkSerial::MARK_SECTION :	
		return m_pDB->GetMarkSection(nMark,pBx);
	case CMarkSerial::MARK_ASSEM :		
		return m_pDB->GetMarkAssembly(nMark,pBx,nRow);
	case CMarkSerial::MARK_ASSEM_MID :		
		return m_pDB->GetMarkAssemblyMid(nMark,pBx,nRow);
	case CMarkSerial::MARK_ASSEM_BIG :		
		return m_pDB->GetMarkAssemblyBig(nMark,pBx,nRow);
	}
	*/
	return _T("");
}


void CBMCreator::AddBMFactChain(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,long nMarkFactType)
{
	// m_nDimType : dimtype 사용
	CPlateGirderApp* pGir = pBxStt->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	CPlateBxFinder Finder(pGir);
	__int64 bxFlag;
	CString sSubName;
	CString sKindName = _T("");

	switch( nMarkFactType )
	{
	case G_F_U : bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_UPPERFLANGE|BX_SPLICE;
					sSubName	 = "상판";
					sKindName	 = "상판";
		break;
	case G_F_L : bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_LOWERFLANGE|BX_SPLICE;
					sSubName	 = "하판";
					sKindName	 = "하판";
		break;
	case G_W : bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_WEB|BX_SPLICE;
		pBxStt = pBxStt->GetBxLeft();	pBxEnd = pBxEnd->GetBxLeft();
					sSubName	 = "복부판";
					sKindName	 = "복부판";
		break;	
	}
	
	CPlateBasicIndex* pBx = Finder.GetBxFirst(bxFlag,pBxStt,pBxEnd,TRUE,FALSE);

	while( pBx )
	{
		CBMRecord * pRec = AddBMRecord();
	
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		pRec->m_nFirstClass = FIRST_GIRDER;
		if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
		pRec->m_nQty = 1;
		pRec->SetStrEleName("일반");
		pRec->SetStrProcessBig("MAIN P/N");
		pRec->SetStrProcessMid("비연결재");
		if(pGir->GetFactConnectType(pBx,nMarkFactType)==1) pRec->SetStrProcessMid("끝단재");
		if(pGir->GetFactConnectType(pBx,nMarkFactType)==2) pRec->SetStrProcessMid("중간재");
		pRec->SetStrSubEleName(sSubName);
		pRec->SetStrSectionType(sKindName);
		//
		SetJewonBMRecord(pRec,pBx,nMarkFactType,0,0);

		pBx = Finder.GetBxNext();
	}
}

void CBMCreator::AddBMHStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,BOOL bLeft)
{
	// m_nDimType : dimtype 사용
	CPlateGirderApp*	pGir = pBxStt->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	CPlateBxFinder Finder(pGir);
	__int64 bxFlag;
	CString sKindName, sEleName, sSubEleName;
	long nMarkPiece;

	sEleName	 = "일반";
	sSubEleName  = "수평보강재";
	bxFlag		 = BX_PLATEHSTIFF;
	nMarkPiece	 = bLeft ? G_H_L : G_H_R;
	sKindName	 = "복부판";
	
	CPlateBasicIndex* pBx = Finder.GetBxFirst(bxFlag,pBxStt,pBxEnd,TRUE,FALSE);
	
	long nCol = 0;
	while( pBx )
	{
		long nSide = bLeft ? -1 : 1;
		CWebHStiff *pHs = pBx->GetWebHStiff();
		for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
		{
			if(pHs==NULL)	break;
			if(pHs->IsEmpty(nDan, bLeft))	continue;
			//
			CBMRecord * pRec = AddBMRecord();
		
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
			pRec->m_nFirstClass = FIRST_GIRDER;
			if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
			pRec->m_nQty = 1;
			pRec->SetStrEleName(sEleName);
			pRec->SetStrProcessBig("GIRDER");
			pRec->SetStrProcessMid("H-STIFF");
			pRec->SetStrSubEleName(sSubEleName);
			pRec->SetStrSectionType(sKindName);
			SetJewonBMRecord(pRec,pBx,nMarkPiece,nDan,nCol);
		}
		pBx = Finder.GetBxNext();
	}
}

void CBMCreator::AddBMSplice(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, BOOL bIncludeStt, BOOL bIncludeEnd)
{
	CPlateGirderApp		*pGir = pBxStt->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_SPLICE,pBxStt,pBxEnd);

	if(!bIncludeStt && pBx==pBxStt)
		pBx = Finder.GetBxNext();

	while( pBx )
	{
		if(!bIncludeEnd && pBx==pBxEnd)
			break;
		AddBMSpliceUpper(pBx);
		AddBMSpliceLower(pBx);
		AddBMSpliceWeb(pBx);
		if(pDed->m_bDeductBoltHole)
		{
			DeductBMSpliceUpperBoltHole(pBx);
			DeductBMSpliceLowerBoltHole(pBx);
			DeductBMSpliceWebBoltHole(pBx);
		}
		if(pDed->m_bDeductSplice)
			DeductBMSpliceWeb(pBx);
			
		pBx = Finder.GetBxNext();
	}
}

// nType = 0:상부, 1:하부, 2:복부판좌측, 3:복부판우측
void CBMCreator::AddBMSpliceUpper(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateSplice    *pSp  = pBx->GetSplice();
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();

	CBMRecord * pRec = AddBMRecord();
	SetBMSpliceRecord(pRec,pBx);
	pRec->SetStrEleName("상부 현장이음");
	pRec->m_nQty = 1;

	BOOL bNextFiller = TRUE;
	// 상면 이음판
	long nMarkBuje = SP_U_U;
	pRec->SetStrSubEleName("상면 이음판");
	SetJewonBMRecord(pRec,pBx,nMarkBuje);

	// 하면 이음판: 좌,우로 2개
	for(long n=0; n<2; n++) 
	{
		nMarkBuje = SP_U_L;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("하면 이음판");
		SetJewonBMRecord(pRec,pBx,nMarkBuje);
	}
	if(pBx->IsFiller(BX_UPPERFLANGE, bNextFiller))
	{
		long nSize = pOpt->GetDesignHeigtBaseUpper() ? 1 : 2;
		for(long n=0; n<nSize; n++) 
		{
			nMarkBuje = SP_U_FILL;
			pRec = AddBMRecord(pRec);
			pRec->SetStrSubEleName("채움판");
			SetJewonBMRecord(pRec,pBx,nMarkBuje);
		}		
	}

	// 볼트&너트
	nMarkBuje = SP_U_BOLT;
	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("볼트-너트");
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("H.T.BOLT");
	(long)pRec->m_nQty = (pSp->m_uB+1)*(pSp->m_uE+1)*4;
	SetJewonBMRecord(pRec,pBx,nMarkBuje);

}

void CBMCreator::AddBMSpliceLower(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateSplice	*pSp  = pBx->GetSplice();
	CGlobarOption	*pOpt = m_pDataManage->GetGlobalOption();

	CBMRecord * pRec = AddBMRecord();
	SetBMSpliceRecord(pRec,pBx);
	pRec->SetStrEleName("하부 현장이음");
	pRec->m_nQty = 1;

	BOOL bNextFiller = TRUE;
	// 하면 이음판
	long nMarkBuje = SP_L_L;
	pRec->SetStrSubEleName("하면 이음판");
	SetJewonBMRecord(pRec,pBx,nMarkBuje);
	
	// 상면 이음판: 좌,우로 2개
	for(long n=0; n<2; n++) 
	{
		nMarkBuje = SP_L_U; 
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("상면 이음판");
		SetJewonBMRecord(pRec,pBx,nMarkBuje);
	}
	if(pBx->IsFiller(BX_LOWERFLANGE, bNextFiller))
	{
		long nSize = !pOpt->GetDesignHeigtBaseLower() ? 1 : 2;
		for(long n=0; n<nSize; n++) 
		{
			nMarkBuje = SP_L_FILL;
			pRec = AddBMRecord(pRec);
			pRec->SetStrSubEleName("채움판");
			SetJewonBMRecord(pRec,pBx,nMarkBuje);
		}		
	}
	// 볼트&너트
	nMarkBuje = SP_L_BOLT;
	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("볼트-너트");
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("H.T.BOLT");
	(long)pRec->m_nQty = (pSp->m_dB+1)*(pSp->m_dE+1)*4;
	SetJewonBMRecord(pRec,pBx,nMarkBuje);

}

void CBMCreator::AddBMSpliceWeb(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateSplice	*pSp  = pBx->GetSplice();

	BOOL bNextFiller = TRUE;
	// 전단 이음판
	CBMRecord * pRec = AddBMRecord();
	SetBMSpliceRecord(pRec,pBx);
	pRec->SetStrEleName("복부판 현장이음");
	pRec->SetStrSubEleName("전단 이음판");	

	pRec->m_nQty = 1;
	SetJewonBMRecord(pRec,pBx,SP_W_SH,0,0);	// 좌측
	pRec = AddBMRecord(pRec);
	SetJewonBMRecord(pRec,pBx,SP_W_SH,0,1);	// 우측

	long n = 0;

	// 모멘트 이음판
	if(pSp->m_nWebType & SP_WEB_MO)
	{
		for(n=0; n<2; n++) //좌우
		{
			pRec = AddBMRecord(pRec);
			pRec->SetStrSubEleName("모멘트 이음판");			

			pRec->m_nQty = 1;
			SetJewonBMRecord(pRec,pBx,SP_W_MO,0, n); // 상판
			pRec = AddBMRecord(pRec);
			SetJewonBMRecord(pRec,pBx,SP_W_MO,1, n); // 하판
		}
	}
	
	if(pBx->IsFiller(BX_WEB, bNextFiller))
	{
		for(n=0; n<2; n++) 
		{
			pRec = AddBMRecord(pRec);
			pRec->SetStrSubEleName("채움판");
			SetJewonBMRecord(pRec,pBx,SP_W_FILL,1,n);
		}
		// 모멘트 이음판
		if(pSp->m_nWebType & SP_WEB_MO)
		{
			for( n=0; n<2; n++) 
			{
				pRec = AddBMRecord(pRec);
				pRec->SetStrSubEleName("채움판");
				SetJewonBMRecord(pRec,pBx,SP_W_FILL,0,n);
				
				pRec = AddBMRecord(pRec);
				pRec->SetStrSubEleName("채움판");
				SetJewonBMRecord(pRec,pBx,SP_W_FILL,2,n);
			}
		}
	}

	// 볼트&너트
	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("볼트-너트");
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("H.T.BOLT");
	pRec->m_nQty = (pSp->m_sE+pSp->m_sE1*2+1)*(pSp->m_sB+1)*2; 
	if(pSp->m_nWebType & SP_WEB_FLMO)
		pRec->m_nQty += (pSp->m_mE+1)*(pSp->m_mB+1)*2*2;
	SetJewonBMRecord(pRec,pBx,SP_W_BOLT); // 전단이음판

	if(pSp->m_nWebType & SP_WEB_MO)
	{
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->SetStrProcessBig("BOLT");
		pRec->SetStrProcessMid("H.T.BOLT");
		pRec->m_nQty = (pSp->m_mE+1)*(pSp->m_mB+1)*2*2;
		SetJewonBMRecord(pRec,pBx,SP_W_BOLT);
	}
//	else if(pSp->m_nWebType & SP_WEB_FLMO)
//	{
//		pRec->m_nQty += (pSp->m_mE+1)*(pSp->m_mB+1)*2*2;
//	}
}

void CBMCreator::DeductBMSpliceWeb(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;
	if(pBx->GetSplice()->m_nWebType != SP_WEB_FLMO)	return;

	CPlateSplice *pSp  = pBx->GetSplice();
	double dFL = (pSp->m_sA + pSp->m_sB*pSp->m_sC)*2 + pSp->m_sC1;
	double dMO = (pSp->m_mA + pSp->m_mB*pSp->m_mC)*2 + pSp->m_mC1;

	CBMRecord *pRec = AddBMRecord();
	SetBMSpliceRecord(pRec,pBx);
	pRec->SetStrEleName("복부판 현장이음");

	// 전단 이음판
	long nMarkBuje = SP_W_SH;
	pRec->SetStrSubEleName("전단 이음판");
	pRec->m_nQty	 = -2;
	pRec->m_vJewon.x = pSp->m_sW;
	pRec->m_vJewon.y = pSp->m_sT;
	pRec->m_vJewon.z = (dMO-dFL)/2;
	pRec->m_dAreaBuje = pRec->m_vJewon.x*pRec->m_vJewon.z;
	SetDeductRecord(pRec,pBx,nMarkBuje);

	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("전단 이음판");
	SetDeductRecord(pRec,pBx,nMarkBuje);

	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_WEB, bNextFiller))
	{
		pRec = AddBMRecord(pRec);
		SetJewonBMRecord(pRec, pBx, SP_W_FILL);
		pRec->m_vJewon.y  = Round(pBx->GetGirder()->GetThickJewon(SP_W_FILL, pBx), 0);
		SetDeductRecord(pRec,pBx,SP_W_FILL);
	}
}

CBMRecord* CBMCreator::AddSimpleBMRecord(CBMRecord* pRec,long nMarkPiece,
						 CPlateBasicIndex* pBx,const CString& sSubEleName)
{
	CPlateGirderApp* pGir = pBx->GetGirder();
	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName(sSubEleName);
	pRec->m_vJewon    = pGir->GetJewonTotal(nMarkPiece, pBx,0,0,m_nDimType);
	pRec->m_sMaterial = m_pDB->GetMaterial(nMarkPiece,pBx);
	pRec->m_dUWeight  = m_pDB->GetWeightUnit(nMarkPiece, pBx, pRec->m_vJewon);

	return pRec;
}

////////////////////////// VBracing /////////////////////////////////////
void CBMCreator::AddBMVBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);

	while( pBx )
	{
		if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
		{
			CSection *pSec = pBx->GetSection();
			if(pSec->IsenDetType(SECDET_VBRACING)) 		
			{
				AddBMVBracingAngle(pBx);
				AddBMVBracingGusset(pBx);
				AddBMVBracingBolt(pBx);
			}
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
		
	}
}

void CBMCreator::AddBMVBracingAngle(CPlateBasicIndex* pBx)
{	
	CGlobarOption   *pGOpt = m_pDataManage->GetGlobalOption();
	CSteelMaterial  *pSteelMat = m_pDataManage->GetSteelMaterial();
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CSection        *pSec  = pBx->GetSection();
	CVBracing       *pVBra = pSec->GetVBracing();
	CSteelMaterialData	*pMatData	= NULL;

	long nRoundUnit = pGOpt->GetPointValueOfUnitGangJae();
	long nRoundTot  = pGOpt->GetPointValueOfGangJae();
	long nSteelType = 0;	// 형강종류
	long nJewonIdx	= 0;
	long nBeamPos   = 0;	// 형강위치

	for(long n=0; n<4; n++)
	{
		CBMRecord * pRec = AddBMRecord();

		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		pRec->SetStrEleName("수직 브레이싱");
		// 0 - 상현재, 1 - 하현재, 2 - 사재 좌측, 3 - 사재 우측
		if(n==0)	
		{
			pMatData	= pVBra->GetSteelMaterialData(STEELMAT_UPPER);
			nSteelType	= pMatData->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_UVBEAM);
			nJewonIdx	= pMatData->m_nJewonIdx;
			nBeamPos	= CSteelMaterial::BUJE_UVBEAM;
			pRec->m_nMarkBuje = VB_UB;
		}
		else if(n==1)
		{
			pMatData	= pVBra->GetSteelMaterialData(STEELMAT_LOWER);
			nSteelType	= pMatData->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_LVBEAM);
			nJewonIdx	= pMatData->m_nJewonIdx;
			nBeamPos	= CSteelMaterial::BUJE_LVBEAM;
			pRec->m_nMarkBuje = VB_LB;
		}
		else 
		{
			pMatData	= pVBra->GetSteelMaterialData(STEELMAT_DIAGONAL);
			nSteelType	= pMatData->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_CVBEAM);
			nJewonIdx	= pMatData->m_nJewonIdx;
			nBeamPos	= CSteelMaterial::BUJE_CVBEAM;
			pRec->m_nMarkBuje = VB_CR;
		}

		if(nSteelType==CSteelMaterialData::LSTEEL)
			pRec->SetStrSubEleName("등변 ㄱ 형강");
		else if(nSteelType==CSteelMaterialData::LVALSTEEL)
			pRec->SetStrSubEleName("부등변 ㄱ 형강");
		else if(nSteelType==CSteelMaterialData::CSTEEL)
			pRec->SetStrSubEleName("ㄷ 형강");
		else if(nSteelType==CSteelMaterialData::CTSTEEL)
			pRec->SetStrSubEleName("CT 형강");
		else if(nSteelType==CSteelMaterialData::ISTEEL)
			pRec->SetStrSubEleName("I 형강");
		else if(nSteelType==CSteelMaterialData::HSTEEL)
			pRec->SetStrSubEleName("H 형강");
		
		pRec->m_nQty = 1;
		pRec->m_nKindName  = MATERIAL_KIND_BEAM;
		
//		pRec->m_dUWeight = Round(pSteelMat->GetUWeight(pBx, nBeamPos)*1000, nRoundUnit);		// kg/m		
		pRec->m_dUWeight = Round(toTon(pMatData->m_dUnitWeight)*1000, nRoundUnit);
		pRec->m_sMaterial  = m_pDB->GetMaterial(pRec->m_nMarkBuje,pBx);
		
		pRec->m_vJewon.x      = pSec->GetBeamJewon(n,0); //dH
		pRec->m_vJewonAngle.x = pSec->GetBeamJewon(n,1); //dB
		pRec->m_vJewon.y      = pSec->GetBeamJewon(n,2); //dT1
		pRec->m_vJewonAngle.y = pSec->GetBeamJewon(n,3); //dT2		
		
		pRec->m_vJewon.z = pSec->GetVBracing()->GetLengthBeam(n);
		pRec->m_dTWeight = Round(pRec->m_dUWeight * toM(pRec->m_vJewon.z), nRoundTot);	// kg
	}
}

void CBMCreator::AddBMVBracingGusset(CPlateBasicIndex* pBx)
{	
	CPlateGirderApp *pGir = pBx->GetGirder();
	for(long n=0; n<5; n++)
	{
		CBMRecord * pRec = AddBMRecord();
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		long nMarkBuje = VB_GU;		
		pRec->SetStrEleName("수직 브레이싱");
		pRec->SetStrSubEleName("브레이싱 이음판");	
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,0,n);
	}
}

void CBMCreator::AddBMVBracingBolt(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CSection *pSec   = pBx->GetSection();
	CVBracing *pB	 = pSec->GetVBracing();

	long nMarkBuje = VB_BOLT; // 상부	
	CBMRecord *pRec = AddBMRecord();
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);	
	pRec->SetStrEleName("수직 브레이싱");
	pRec->SetStrSubEleName("볼트-너트");
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("H.T.BOLT");
	pRec->m_nQty  = (long)( (pB->m_uB + 1) * 2 + (pB->m_dB + 1) * 2);
	SetJewonBMRecord(pRec,pBx,nMarkBuje);
}
////////////////////////////////////////////////////////////////////////

////////////////////////// 수직 보강재 /////////////////////////////////////
// 가로보 및 수직브레이싱 위치의 수직보강재는 AddBMCrossBeam()에서 처리함
void CBMCreator::AddBMVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
	
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_VSTIFF, pBxStt, pBxEnd);	
	
	if(pBx==NULL)	return;

	while(pBx)
	{
		for(long nCol=0; nCol<2; nCol++) //거더기준으로 좌우측.
		{
			CVStiff *pVStiff = pBx->GetSection()->GetVStiff();

			if(pVStiff->m_dWidth[nCol]==0 || pVStiff->m_dThick[nCol]==0) continue;

			if(pVStiff->GetVStiffHeight(nCol==0?TRUE:FALSE)>0)
			{
				long nMarkBuje = V_ST;		
				CBMRecord * pRec = AddBMRecord();	
				pRec->m_nGirder = pBx->GetNumberGirder();
				pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
				pRec->m_nQty = 1;				
				pRec->SetStrEleName("일반");
				pRec->SetStrSubEleName("수직보강재");
				pRec->SetStrProcessMid("V-STIFF");
				SetJewonBMRecord(pRec,pBx,nMarkBuje, 0, nCol);
			}
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}
////////////////////////////////////////////////////////////////////////

////////////////////////// VBracing /////////////////////////////////////
void CBMCreator::AddBMHBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBxFinder Finder2(pBxStt->GetGirder());
	
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	CPlateBasicIndex* pBx2 = (CPlateBasicIndex*)Finder2.GetBxFirst(BX_HBRACINGGUSSET,pBxStt,pBxEnd);

	CPlateBasicIndex *pBxNext = NULL;
	while( pBx )
	{
		// 0030849 CHBracingPositionDlg::SetDataInit() 참고
		pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
		if(pBxNext==NULL)	break;

		AddBMHBracingAngle(pBx);

		if(pBxNext->GetStation() >= pBxEnd->GetStation()) break;
		pBx = pBxNext;
	}

	while( pBx2 )
	{
		AddBMHBracingGusset(pBx2);
		AddBMHBracingBolt(pBx2);		
		pBx2 = (CPlateBasicIndex*)Finder2.GetBxNext();
	}

}

void CBMCreator::AddBMHBracingAngle(CPlateBasicIndex* pBx)
{
	CGlobarOption   *pGOpt     = m_pDataManage->GetGlobalOption();
	CSteelMaterial  *pSteelMat = m_pDataManage->GetSteelMaterial();
	CPlateGirderApp *pGir      = pBx->GetGirder();
	CSection        *pSec      = pBx->GetSection();
	CBracing        *pBra      = pBx->GetBracing();
	CSteelMaterialData	*pMatData	= pBra->GetSteelMaterialData();

	long nRoundUnit = pGOpt->GetPointValueOfUnitGangJae();
	long nRoundTot  = pGOpt->GetPointValueOfGangJae();

	if(pBra->GetType() == CBracing::BRACTYPE_LEFTWAVE || pBra->GetType() == CBracing::BRACTYPE_RIGHTWAVE || pBra->GetType() == CBracing::BRACTYPE_CROSS)
	{
		for(long n=0; n<2; n++)
		{		
			CBMRecord * pRec = AddBMRecord();

			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);

			// 0 , 1
			pRec->m_nMarkBuje = HB_B;
			pRec->SetStrEleName("수평 브레이싱");

			long nSteelType = pMatData->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_LHBEAM);
			long nJewonIdx	= pMatData->m_nJewonIdx;

			if(nSteelType==CSteelMaterialData::LSTEEL)
				pRec->SetStrSubEleName("등변 ㄱ 형강");
			else if(nSteelType==CSteelMaterialData::LVALSTEEL)
				pRec->SetStrSubEleName("부등변 ㄱ 형강");
			else if(nSteelType==CSteelMaterialData::CSTEEL)
				pRec->SetStrSubEleName("ㄷ 형강");
			else if(nSteelType==CSteelMaterialData::CTSTEEL)
				pRec->SetStrSubEleName("CT 형강");
			else if(nSteelType==CSteelMaterialData::ISTEEL)
				pRec->SetStrSubEleName("I 형강");
			else if(nSteelType==CSteelMaterialData::HSTEEL)
				pRec->SetStrSubEleName("H 형강");

			pRec->m_nQty = 1;
			pRec->m_nKindName  = MATERIAL_KIND_BEAM;			
//			pRec->m_dUWeight   = Round(pSteelMat->GetUWeight(pBx, CSteelMaterial::BUJE_LHBEAM)*1000, nRoundUnit);		// kg/m		
			pRec->m_dUWeight   = Round(toTon(pMatData->m_dUnitWeight)*1000, nRoundUnit);		// kg/m		
			pRec->m_sMaterial  = m_pDB->GetMaterial(pRec->m_nMarkBuje,pBx);
			/////////////////////////////////////////////////////////////////////////////////////////////////
			pRec->m_vJewon.x      = pBra->GetBeamJewon(n,FALSE,0);	//dH
			pRec->m_vJewonAngle.x = pBra->GetBeamJewon(n,FALSE,1); //dB
			pRec->m_vJewon.y      = pBra->GetBeamJewon(n,FALSE,2); //dT1
			pRec->m_vJewonAngle.y = pBra->GetBeamJewon(n,FALSE,3); //dT2		
			/////////////////////////////////////////////////////////////////////////////////////////////////

			pRec->m_vJewon.z = pBra->GetBeamJewon(n,FALSE,6);
			pRec->m_dTWeight = Round(pRec->m_dUWeight * toM(pRec->m_vJewon.z), nRoundTot);	// kg
		}
	}
	else if(pBra->GetType() == CBracing::BRACTYPE_SLOPLEFT || pBra->GetType() == CBracing::BRACTYPE_SLOPRIGHT)
	{
		CBMRecord * pRec = AddBMRecord();

		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);

		// 0 , 1
		pRec->m_nMarkBuje = HB_B;
		pRec->SetStrEleName("수평 브레이싱");

		// 0 - 상부  1 - 하부
		long nSteelType	= pBra->GetSteelMaterialData()->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_LHBEAM);
		long nJewonIdx	= pBra->GetSteelMaterialData()->m_nJewonIdx;

		if(nSteelType==CSteelMaterialData::LSTEEL)
			pRec->SetStrSubEleName("등변 ㄱ 형강");
		else if(nSteelType==CSteelMaterialData::LVALSTEEL)
			pRec->SetStrSubEleName("부등변 ㄱ 형강");
		else if(nSteelType==CSteelMaterialData::CSTEEL)
			pRec->SetStrSubEleName("ㄷ 형강");
		else if(nSteelType==CSteelMaterialData::CTSTEEL)
			pRec->SetStrSubEleName("CT 형강");
		else if(nSteelType==CSteelMaterialData::ISTEEL)
			pRec->SetStrSubEleName("I 형강");
		else if(nSteelType==CSteelMaterialData::HSTEEL)
			pRec->SetStrSubEleName("H 형강");

		pRec->m_nQty = 1;
		pRec->m_nKindName  = MATERIAL_KIND_BOLT;			
//		pRec->m_dUWeight   = Round(pSteelMat->GetUWeight(pBx, CSteelMaterial::BUJE_LHBEAM), nRoundUnit);
		pRec->m_dUWeight   = Round(toTon(pMatData->m_dUnitWeight)*1000, nRoundUnit);
		pRec->m_sMaterial  = m_pDB->GetMaterial(pRec->m_nMarkBuje,pBx);
		/////////////////////////////////////////////////////////////////////////////////////////////////
		pRec->m_vJewon.x      = pBra->GetBeamJewon(0,FALSE,0);	//dH
		pRec->m_vJewonAngle.x = pBra->GetBeamJewon(0,FALSE,1); //dB
		pRec->m_vJewon.y      = pBra->GetBeamJewon(0,FALSE,2); //dT1
		pRec->m_vJewonAngle.y = pBra->GetBeamJewon(0,FALSE,3); //dT2		
		/////////////////////////////////////////////////////////////////////////////////////////////////

		pRec->m_vJewon.z = pBra->GetBeamJewon(0,FALSE,6);
		pRec->m_dTWeight = Round(pRec->m_dUWeight * pRec->m_vJewon.z, nRoundTot);
	}
}

void CBMCreator::AddBMHBracingGusset(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();

	for(long nRow=0; nRow<2; nRow++)
	{
		if(pBrGu->IsReal(nRow))
		{
			CBMRecord * pRec = AddBMRecord();
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
			long nMarkBuje = HB_GU;		
			pRec->SetStrEleName("수평 브레이싱");
			pRec->SetStrSubEleName("브레이싱 이음판");	
			pRec->m_nQty = 1;
		
			SetJewonBMRecord(pRec,pBx,nMarkBuje, nRow, 0);
		}
	}
}

void CBMCreator::AddBMHBracingBolt(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CHBracingGusset* pBrGu = pBx->GetHBracingGusset();
	if(pBrGu->IsReal())
	{
		long nMarkBuje = HB_BOLT; // 상부	
		CBMRecord *pRec = AddBMRecord();
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);	
		pRec->SetStrEleName("수평 브레이싱");
		pRec->SetStrSubEleName("볼트-너트");
		pRec->SetStrProcessBig("BOLT");
		pRec->SetStrProcessMid("H.T.BOLT");
		pRec->m_nQty = pBrGu->GetBoltsu();
		
		SetJewonBMRecord(pRec,pBx,nMarkBuje);
	}
}
////////////////////////////////////////////////////////////////////////

void CBMCreator::AddBMCrossBeam(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING
		                    , ((CPlateBasicIndex*)pBxStt), ((CPlateBasicIndex*)pBxEnd));

	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
		{
			CPlateCrossBeam  *pC = pBx->GetSection()->GetCrossBeam();
			if(pC==NULL) break;
			
			AddBMCrossBeamMain(pBx);		
			if(pC->m_nLGirSpType)
				AddBMCrossBeamBracket(pBx,TRUE);
			if(pC->m_nRGirSpType)
				AddBMCrossBeamBracket(pBx,FALSE);
			if(pC->m_cGasset_CR)
			{
				AddBMCrossBeamBracketGusset(pBx,TRUE);
				AddBMCrossBeamBracketGusset(pBx,FALSE);
			}		
			AddBMCrossBeamSplice(pBx,TRUE);
			AddBMCrossBeamSplice(pBx,FALSE);

			DeductBMCrossBeamMain(pBx);
			if(pC->m_nLGirSpType)
				DeductBMCrossBeamBracket(pBx,TRUE);
			if(pC->m_nRGirSpType)
				DeductBMCrossBeamBracket(pBx,FALSE);
			if(pC->m_cGasset_CR)
			{
				DeductBMCrossBeamBracketGasset(pBx,TRUE);
				DeductBMCrossBeamBracketGasset(pBx,FALSE);
			}

			DeductBMCrossBeamSplice(pBx,TRUE);
			DeductBMCrossBeamSplice(pBx,FALSE);		
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}

void CBMCreator::AddBMCrossBeamMain(CPlateBasicIndex* pBx)
{
	if(pBx->GetSection()==NULL) return;

	CPlateGirderApp		*pGir	= pBx->GetGirder();
	CPlateBasicIndex	*pBxR	= pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CGlobarOption		*pGlopt	= m_pDB->m_pDataManage->GetGlobalOption();
	CSection			*pSec	= pBx->GetSection();
	CGangjeBase pBase(m_pDB);

	if(pBxR == NULL) return;
	
	CPlateCrossBeam  *pC   = pBx->GetSection()->GetCrossBeam();
	
	if(pC==NULL) return;

	long	nRoundUnit	= pGlopt->GetPointValueOfUnitGangJae();
	long	nRoundTot	= pGlopt->GetPointValueOfGangJae();
	double	dDistWeb	= pGir->GetDisFrontPlateWebToWeb(pBx);
	double	dWVstiffL	= pC->m_pVStiff->m_dWidth[1];
	double	dWVstiffR	= pBxR->GetSection()->GetVStiff()->m_dWidth[0];

	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR*2)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffL-pC->m_dbL3)<1.00)  return;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffR-pC->m_dbR3)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-dWVstiffL-dWVstiffR-pC->m_dbL3-pC->m_dbR3)<1.00)  return;
	}
	CBMRecord * pRec = AddBMRecord();
	SetBMCrossBeamRecord(pRec,pBx);

	////////////////////////////
	long nRow=0,nCol=0;
	long nMarkBuje=0;
	if(pC->GetType()==CR_TYPE_HSTEEL)
	{
		CSteelMaterialData	*pMatData	= pC->GetSteelMaterialData();

		long	nSteelType	= pMatData->m_nType;
		CString	szSubName	= _T("");

		if(pMatData->m_nType==CSteelMaterialData::LSTEEL)			szSubName	= _T("등변 ㄱ 형강");
		else if(pMatData->m_nType==CSteelMaterialData::LVALSTEEL)	szSubName	= _T("부등변 ㄱ 형강");
		else if(pMatData->m_nType==CSteelMaterialData::CSTEEL)		szSubName	= _T("ㄷ 형강");
		else if(pMatData->m_nType==CSteelMaterialData::CTSTEEL)		szSubName	= _T("CT 형강");
		else if(pMatData->m_nType==CSteelMaterialData::ISTEEL)		szSubName	= _T("I 형강");
		else if(pMatData->m_nType==CSteelMaterialData::HSTEEL)		szSubName	= _T("H 형강");

		pRec->SetStrSubEleName(szSubName);
		pRec->m_nMarkBuje		= CP_STEEL;
		pRec->m_nQty			= 1;
		pRec->m_nKindName		= pBase.GetMaterialKindNumber(pRec->m_nMarkBuje);
		pRec->m_dUWeight		= Round(toTon(pMatData->m_dUnitWeight)*1000, nRoundUnit);
		pRec->m_sMaterial		= m_pDB->GetMaterial(pRec->m_nMarkBuje,pBx);
		pRec->m_vJewon.x		= pMatData->m_dH;
		pRec->m_vJewonAngle.x	= pMatData->m_dB;
		pRec->m_vJewon.y		= pMatData->m_dT1;
		pRec->m_vJewonAngle.y	= pMatData->m_dT2;
		pRec->m_vJewon.z		= Round(pGir->GetLengthJewon(pRec->m_nMarkBuje, pBx),0);
		pRec->m_dTWeight		= Round(pRec->m_dUWeight * toM(pRec->m_vJewon.z), nRoundTot);	// kg
	}
	else
	{
		nMarkBuje = CP_W;
		pRec->SetStrSubEleName("복부판");
		pRec->SetStrProcessMid("WEB");
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);

		////////////////////////////
		nMarkBuje = CP_F_U;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("상판");
		pRec->SetStrProcessMid("FLG");
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);

		////////////////////////////
		nMarkBuje = CP_F_L;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("하판");
		pRec->SetStrProcessMid("FLG");
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}

	////////////////////////////
	nMarkBuje = CP_V;
	pRec->m_nQty = 1;
	
	long n = 0;

	for(n=0; n<pC->m_nVStiffsu_CR; n++) 
	{
		if(pC->m_cVSide_CR==2) break;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("수직보강재");
		pRec->SetStrProcessMid("V-STIFF");
		SetJewonBMRecord(pRec,pBx,nMarkBuje,0,n);
	}
	for(n=0; n<pC->m_nVStiffsu_CR; n++) 
	{
		if(pC->m_cVSide_CR==1) break;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("수직보강재");
		pRec->SetStrProcessMid("V-STIFF");
		SetJewonBMRecord(pRec,pBx,nMarkBuje,1,n);
	}

	// 수평보강재가 있으면..
	if(pC->m_nHStiffUppersu_CR + pC->m_nHStiffLowersu_CR > 0)	
	{
		// 가로보 도면 물량이랑 동일한 형태가 되어야함.
		CDPoint Po[18];
		BOOL bIncludeGasset = pGlopt->GetDesignCrossBeamHStiffAtBracket()==0 ? TRUE : FALSE;
		long nQty = pC->m_cVSide_CR==0 ? 2 : 1;	//양면수직보강재 이면 수평보강재는 2 그렇지 않으면 1
		long nV = 0;
		BOOL bFront = pC->m_cVSide_CR==2 ? FALSE : TRUE;  
		if (bFront)
			nV = pGir->GetPointVStiffOnCrossBeamPrv(pBx, Po, FALSE, bIncludeGasset);
		else
			nV = pGir->GetPointVStiffOnCrossBeam(pBx, Po, FALSE, bIncludeGasset);

		long nH = pC->m_nHStiffUppersu_CR + pC->m_nHStiffLowersu_CR;
		long nColStt = bIncludeGasset&&pC->m_nLGirSpType  ? 0 : 1;
		long nColEnd = bIncludeGasset&&pC->m_nRGirSpType  ? nV-1 : nV-2;

		for(nRow=0; nRow<nH; nRow++) 
		{
			for(nCol=nColStt; nCol<nColEnd; nCol++) 
			{
				pRec = AddBMRecord(pRec);
				pRec->SetStrSubEleName("수평보강재");
				pRec->SetStrProcessMid("H-STIFF");
				pRec->m_nQty = nQty;
				SetJewonBMRecord(pRec,pBx,CP_H,nRow,nCol,TRUE);
			}
		}
	}

	////////////////////////////
	if(pC->m_nAnchorsu_CR>0)
	{
		nMarkBuje = GI_SA;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("슬래브 앵커");
		pRec->SetStrProcessBig("BOLT");
		pRec->SetStrProcessMid("SLAB ANCHOR");
		pRec->m_nQty = pC->m_nAnchorsu_CR;
		SetJewonBMRecord(pRec,pBx,nMarkBuje);
	}
	
}

void CBMCreator::AddBMCrossBeamBracket(CPlateBasicIndex* pBx, BOOL bLeft)
{
	if(pBx->GetSection()==NULL) return;
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC==NULL) return;
	CPlateGirderApp* pGir = pBx->GetGirder();

	CBMRecord * pRec = NULL;

	////////////////////////////
	long nRow = 0;
	long nCol = bLeft ? 0 : 1;
	long nMarkBuje = pBx->IsJijum() ? CP_BR_W : CG_BR_W;
	if(pC->GetType()!=CR_TYPE_VSTIFF)
	{
		pRec = AddBMRecord();
		SetBMCrossBeamRecord(pRec,pBx);
		pRec->SetStrSubEleName("브라켓 복부판");
		pRec->SetStrProcessMid("WEB");
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}
	////////////////////////////
	nMarkBuje = pBx->IsJijum() ? CP_BR_F_U : CG_BR_F_U;
	pRec = AddBMRecord(pRec);
	SetBMCrossBeamRecord(pRec,pBx);
	pRec->SetStrSubEleName("브라켓 상판");
	pRec->SetStrProcessMid("T-FLG");
	pRec->m_nQty = 1;
	SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);

	////////////////////////////
	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();
	if(!bLeft)
	{
		CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
		pBrGu = pBxR ? pBxR->GetHBracingGusset() : NULL;
	}

	if(!pBrGu || (bLeft&&!pBrGu->IsReal(1)) || (!bLeft&&!pBrGu->IsReal(0)))
	{
		nMarkBuje = pBx->IsJijum() ? CP_BR_F_L : CG_BR_F_L;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("브라켓 하판");
		pRec->SetStrProcessMid("B-FLG");
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}	
}

void CBMCreator::AddBMCrossBeamBracketGusset(CPlateBasicIndex* pBx, BOOL bLeft)
{
	if(pBx->GetSection()==NULL) return;
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC==NULL) return;
	CPlateGirderApp* pGir = pBx->GetGirder();
	CPlateBridgeApp	*pBridge = m_pDataManage->GetBridge();

	CBMRecord * pRec;
	CBMRecord RecMain;
	SetBMCrossBeamRecord(&RecMain,pBx);
	////////////////////////////
	long nRow = 0;
	long nCol = bLeft ? 0 : 1;

	long nMarkBuje = CP_BR_GA;
	pRec = AddBMRecord(&RecMain);
	pRec->SetStrSubEleName("브라켓 보강재");
	pRec->SetStrProcessMid("V-보강");
	pRec->m_nQty = 1;
	SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);

	////////////////////////////
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음
	double dT = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_T);
	if(dT) 
	{
		nMarkBuje = CP_BR_GA_FA;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("브라켓 보강판");
		pRec->SetStrProcessMid("V-보강FLG");
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}
}

// 가로보 위치(BX_CROSSBEAM_VBRACING)에 있는 거더부착 수직보강재의 물량계산
void CBMCreator::AddBMCrossBeamGirderVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp *pGir	= pBxStt->GetGirder();
	CGangjeBase pBase(m_pDB);
	CGlobarOption *pGOpt = m_pDataManage->GetGlobalOption();
	CPlateGirderApp *pGirL	= pGir->GetNumberGirder() != 0 ? pGir->m_pLeft : pGir;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	long nRoundUnit = pGOpt->GetPointValueOfUnitGangJae();
	long nRoundTot  = pGOpt->GetPointValueOfGangJae();

	while(pBx)
	{
		if(!pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))	{ pBx = Finder.GetBxNext();	continue;}
		if(pBx->IsJijum())	{ pBx = Finder.GetBxNext();	continue;}
		CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
		CVStiff *pVStiff = pC ? pC->m_pVStiff : NULL;

		if(!pGirL || !pVStiff)
		{
			pBx = (CPlateBasicIndex*)Finder.GetBxNext();
			continue;
		}
		CPlateBasicIndex	*pBxMatch	= pGir->GetBxMatchByCrossBeam(pBx, pGirL);
		if(!pBxMatch)
		{
			pBx = (CPlateBasicIndex*)Finder.GetBxNext();
			continue;
		}
		CPlateCrossBeam	*pCL = pGirL ? pBxMatch->GetSection()->GetCrossBeam() : NULL;
		long nMarkBuje =V_ST;

		CBMRecord *pRec;
		CBMRecord RecMain;
		SetBMCrossBeamRecord(&RecMain,pBx);
		if(pVStiff->m_dWidth[0] > 0 && pVStiff->GetVStiffHeight(TRUE) > 0)
		{
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrEleName("일반");
			pRec->SetStrSubEleName("수직보강재");
			pRec->SetStrProcessMid("V-STIFF");
			pRec->m_nQty = 1;
			long	nRow	= 0;
			long	nCol	= 0;
			CVector vJewon,vJewonBasic;
			vJewon.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewon.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewon.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewonBasic.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			vJewonBasic.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			vJewonBasic.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			if(pCL && pCL->GetType() == CR_TYPE_MANUFACTURE)
			{
				vJewon.z -= (pCL->m_H_CR + pCL->m_T1_CR);
				vJewonBasic.z -= (pCL->m_H_CR + pCL->m_T1_CR);
				if(pCL->m_dRH1 > 0)
				{
					vJewon.z -= pCL->m_T2_CR;
					vJewonBasic.z -= pCL->m_T2_CR;
				}
			}
			SetJewonBMRecord(pRec, pBx, nMarkBuje, nRow, nCol, TRUE, TRUE, &vJewon, &vJewonBasic);
		}
		if(pVStiff->m_dWidth[1] > 0 && pVStiff->GetVStiffHeight(FALSE) > 0)
		{
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrEleName("일반");
			pRec->SetStrSubEleName("수직보강재");
			pRec->SetStrProcessMid("V-STIFF");
			pRec->m_nQty = 1;

			long	nRow	= 0;
			long	nCol	= 1;
			CVector vJewon,vJewonBasic;
			vJewon.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewon.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewon.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewonBasic.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			vJewonBasic.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			vJewonBasic.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			if(pC->GetType() == CR_TYPE_MANUFACTURE)
			{
				vJewon.z -= (pC->m_H_CR + pC->m_T1_CR);
				vJewonBasic.z -= (pC->m_H_CR + pC->m_T1_CR);
				if(pC->m_dLH1 > 0)
				{
					vJewon.z -= pC->m_T2_CR;
					vJewonBasic.z -= pC->m_T2_CR;
				}
			}
			SetJewonBMRecord(pRec, pBx, nMarkBuje, nRow, nCol, TRUE, TRUE, &vJewon, &vJewonBasic);
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}


void CBMCreator::AddBMCrossBeamSplice(CPlateBasicIndex* pBx, BOOL bLeft)
{
	if(pBx->GetSection()==NULL) return;
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	if(pBxR==NULL) return;
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC==NULL) return;
	double dDistWeb  = pGir->GetDisFrontPlateWebToWeb(pBx);
	double dWVstiffL = pC->m_pVStiff->m_dWidth[1];
	double dWVstiffR = pBxR->GetSection()->GetVStiff()->m_dWidth[0];

	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR*2)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffL-pC->m_dbL3)<1.00)  return;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffR-pC->m_dbR3)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-dWVstiffL-dWVstiffR-pC->m_dbL3-pC->m_dbR3)<1.00)  return;
	}

	CBMRecord * pRec;
	CBMRecord RecMain;
	SetBMCrossBeamRecord(&RecMain,pBx);

	////////////////////////////
	long nRow = 0;
	long nCol = bLeft ? 0 : 1;
	long nMarkBuje = CP_SP_U_U;
	if((pC->m_nLGirSpType && bLeft==TRUE) || (pC->m_nRGirSpType && bLeft==FALSE))
	{
		//수직보강재타입이고 커넥션플레이트가 미적용일 때
		if(pC->GetType()==CR_TYPE_VSTIFF && pC->m_bConnectionPlate == 0)
		{
			nMarkBuje = CP_SP_U_L;
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrSubEleName("상면 이음판");
			pRec->m_nQty = 2;
			SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		}
		else
		{		
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrSubEleName("상면 이음판");
			pRec->m_nQty = 1;
			SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);		
		}
		////////////////////////////
		if(pC->m_nRadioConnectionPlateInOut==0)
		{
			nMarkBuje = CP_SP_U_L;
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrSubEleName("상면 이음판");
			pRec->m_nQty = 2;
			SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		
			////////////////////////////
			nMarkBuje = CP_SP_L_U;
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrSubEleName("하면 이음판");
			pRec->m_nQty = (pC->GetType()==CR_TYPE_MANUFACTURE) ? 2 : 4;
			SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		}
		////////////////////////////
		if(pC->GetType()==CR_TYPE_MANUFACTURE)
		{
			nMarkBuje = CP_SP_L_L;
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrSubEleName("하면 이음판");
			pRec->m_nQty = 2;
			if(pC->m_cGasset_CR==0 || pC->m_H_CR==pBx->GetHeightGirderByBx())
				pRec->m_nQty = 1;
		
			SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		}
	}

	////////////////////////////
	if(pC->m_mB && pC->m_mC)
	{
		nMarkBuje = CP_SP_W_MO;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("모멘트 이음판");
		pRec->m_nQty = 4;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}

	BOOL bShearSplice = pC->m_sT > 0 ? TRUE : FALSE;
	if(pC->GetType()==CR_TYPE_HSTEEL)
		bShearSplice = FALSE;
	////////////////////////////
	if(bShearSplice)
	{
		nMarkBuje = CP_SP_W_SH;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("전단 이음판");
		pRec->m_nQty = 2;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}

	///채움판 (Filler)
	double dThickFiller;
	long iQty = pC->m_nFillerUseType;	// 0, 1:한쪽, 2:양쪽

	dThickFiller = pC->GetFillerThick();
	if(pC->m_nFillerUseType>0 && dThickFiller >= 2 && !pDB->IsPlateGir())
	{
		nMarkBuje = CP_SP_W_FH;//132
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("채움판");//("전단 이음판");
		pRec->m_nQty = iQty;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}
	//채움판 두께가 9mm이상인경우
	if(pC->m_nFillerUseType>0 && dThickFiller >= 9 && !pDB->IsPlateGir())
	{
		nMarkBuje = CP_BOLT; 
		nRow = 5;//볼트길이용...
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->m_nQty = pC->m_sE+1;//볼트갯수.
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}	
	////////////////////////////
	CGlobarOption *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	BOOL bZigZag = (pGlopt->GetDesignBoltHoleJigJaegOfHorPlate()==0 && pC->m_uE>0) ? TRUE : FALSE;
	if((pC->m_nLGirSpType && bLeft) || (pC->m_nRGirSpType && bLeft==FALSE))
	{
		nMarkBuje = CP_BOLT; // 상부
		nRow = 0;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->SetStrProcessBig("BOLT");
		pRec->SetStrProcessMid("H.T.BOLT");

		pRec->m_nQty  = (long)( (pC->m_uB+1)*2 * (pC->m_uE+1));
		if(bZigZag)
			if((pC->m_uB%2)==0  && (pC->m_uE%2)==0)
				pRec->m_nQty = (long)(pRec->m_nQty/2)+1;
			else
				pRec->m_nQty = (long)(pRec->m_nQty/2);

		if(pC->m_bConnectionPlate && (pC->GetType()==CR_TYPE_MANUFACTURE || pC->GetType()==CR_TYPE_VSTIFF) )
		{
			int nQty = (int)((pC->m_uB1+1)*2 * (pC->m_uE1+1));
			if(bZigZag)
			{
				nQty >>= 1; //(long)(nQty/2);
				if(((int)pC->m_uB1%2)==0  && ((int)pC->m_uE1%2)==0)
					nQty++;
			}
			//nQty <<= 1;	//nQty *= 2;
			pRec->m_nQty += nQty;
		}
		else
		{
			pRec->m_nQty *= 2;
		}
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);

		nMarkBuje = CP_BOLT; // 하부
		nRow = 4;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->m_nQty = (long)( (pC->m_dB+1)*2 * (pC->m_dE+1));
		if(bZigZag)
			if((pC->m_dB%2)==0  && (pC->m_dE%2)==0)
				pRec->m_nQty = (long)(pRec->m_nQty/2)+1;
			else
				pRec->m_nQty = (long)(pRec->m_nQty/2);

		if(pC->m_bConnectionPlate && (pC->GetType()==CR_TYPE_MANUFACTURE || pC->GetType()==CR_TYPE_VSTIFF) )
		{
			int nQty = (int)((pC->m_uB1+1)*2 * (pC->m_uE1+1));
			if(bZigZag)
			{
				nQty >>= 1; //(long)(nQty/2);
				if(((int)pC->m_uB1%2)==0  && ((int)pC->m_uE1%2)==0)
					nQty++;
			}
			//nQty <<= 1;	//nQty *= 2;
			pRec->m_nQty += nQty;
		}
		else
		{
			pRec->m_nQty *= 2;
		}
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}
	if(pC->m_mB && pC->m_mC && pC->m_mE && pC->m_mF) 
	{
		nMarkBuje = CP_BOLT; // MOMENT
		nRow = 1;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->m_nQty = (long)( (pC->m_mB+1)*2 * (pC->m_mE+1)*2 );
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}
	
	if(bShearSplice) 
	{
		nMarkBuje = CP_BOLT; // SHEAR
		nRow = 2;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->m_nQty = (long)( (pC->m_sB+1)*2 * (pC->m_sE+1) );
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	}
	//가로보와 거더의 수직보강재가 연결타입일 경우. 
	if(!pC->m_nLGirSpType && bLeft==TRUE)
	{
		nMarkBuje = CP_BOLT; 
		nRow = 2;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->m_nQty = pC->m_wE+1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		if(pC->m_cGasset_CR)
		{
			nMarkBuje = CP_BOLT; 
			nRow = 2;
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrSubEleName("볼트-너트");
			pRec->m_nQty = (long)(pC->m_ltE+1);
			SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		}					
	}
	if(!pC->m_nRGirSpType && bLeft==FALSE)
	{
		nMarkBuje = CP_BOLT; 
		nRow = 2;
		pRec = AddBMRecord(&RecMain);
		pRec->SetStrSubEleName("볼트-너트");
		pRec->m_nQty = pC->m_wE+1;
		SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		if(pC->m_cGasset_CR)
		{
			nMarkBuje = CP_BOLT;
			nRow = 2;
			pRec = AddBMRecord(&RecMain);
			pRec->SetStrSubEleName("볼트-너트");
			pRec->m_nQty = (long)(pC->m_ltE+1);
			SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
		}
	}
}

// 수직브레이싱 위치(BX_CROSSBEAM_VBRACING)에 있는 거더부착 수직보강재의 물량계산
void CBMCreator::AddBMVBracingGirderVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp *pGir	= pBxStt->GetGirder();
	CPlateGirderApp *pGirL	= pGir->GetNumberGirder() != 0 ? pGir->m_pLeft : pGir;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	while(pBx)
	{
		double dW=0, dT=0, dH=0, dReault=0;
		if(pBx->GetSection()->IsenDetType(SECDET_VBRACING) && !pBx->IsJijum())	// 지점일때는 지점부보강재에서
		{
			CVBracing *pVr = pBx->GetSection()->GetVBracing();
			CVStiff *pVStiff = pVr ? pVr->m_pVStiff : NULL;
			if(pVStiff==NULL) continue;

			CPlateBasicIndex *pBxL	= pGirL ? pGir->GetBxMatchByCrossBeam(pBx, pGirL) : NULL;
			CSection		 *pSecL	= pBxL  ? pBxL->GetSection() : NULL;
			CPlateCrossBeam	 *pCL	= (pSecL && pSecL->IsenDetType(SECDET_CROSSBEAM)) ? pSecL->GetCrossBeam() : NULL;

			long nMarkBuje =V_ST;

			CBMRecord *pRec;
			CBMRecord RecMain;
			SetBMCrossBeamRecord(&RecMain,pBx);
			if(pVStiff->m_dWidth[0] > 0)
			{
				if(pVStiff->GetVStiffHeight(TRUE) > 0)	// 거더 좌측
				{
					pRec = AddBMRecord(&RecMain);
					pRec->SetStrEleName("일반");
					pRec->SetStrSubEleName("수직보강재");
					pRec->SetStrProcessMid("V-STIFF");
					pRec->m_nQty = 1;
				
					SetJewonBMRecord(pRec, pBx, nMarkBuje, 0, 0);

					// 수직브레이싱 좌측에 제작타입의 가로보가 설치된 경우 (동일스테이션에 수직브레이싱, 가로보 혼합설치 고려)
					if(pCL && pCL->GetType() == CR_TYPE_MANUFACTURE)
					{
						pRec->m_vJewon.z -= (pCL->m_H_CR + pCL->m_T1_CR);
						if(pCL->m_dRH1 > 0)
							pRec->m_vJewon.z -= pCL->m_T2_CR;
					}
				}
			}
			if(pVStiff->m_dWidth[1] > 0)
			{
				if(pVStiff->GetVStiffHeight(FALSE) > 0)	// 거더 우측
				{
					pRec = AddBMRecord(&RecMain);
					pRec->SetStrEleName("일반");
					pRec->SetStrSubEleName("수직보강재");
					pRec->SetStrProcessMid("V-STIFF");
					pRec->m_nQty = 1;

					SetJewonBMRecord(pRec, pBx, nMarkBuje, 0, 1);
				}
			}		
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}

void CBMCreator::AddBMGitaBuje(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	__int64 BxFlag = BX_CROSSBEAM_VBRACING|BX_VSTIFF;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);

	while( pBx )
	{		
		AddBMGitaBuje_SolePlate(pBx);
		pBx = Finder.GetBxNext();
	}

	pBx = Finder.GetBxFirst(BX_VSTIFF, pBxStt, pBxEnd);
	while( pBx )
	{		
		AddBMGitaBuje_Dongbari(pBx);
		pBx = Finder.GetBxNext();
	}

}

void CBMCreator::AddBMGitaBuje_Dongbari(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CSection        *pSec = pBx->GetSection();

	if(pSec==NULL)	return;
	if(pSec->GetVStiff()==NULL)	 return;
	
	CVStiff *pVStiff = pSec->GetVStiff();

	BOOL bUpper = pVStiff->m_cCarryUpper;
	BOOL bLower = pVStiff->m_cCarryLower;

	if(!bUpper && !bLower) return;

	BOOL bLeft  = TRUE;
	BOOL bRight = TRUE;
	
	if(pVStiff->m_dWidth[0]>0 && pVStiff->m_dWidth[1]==0)  bLeft = FALSE;
	if(pVStiff->m_dWidth[0]==0 && pVStiff->m_dWidth[1]>0)  bRight = FALSE;

	if(!bLeft && !bRight) return;

	long nMarkBuje = GI_HG;

	if(bLeft)
	{
		CBMRecord *pRec = AddBMRecord();
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		pRec->m_nFirstClass		= FIRST_GIRDER;

		pRec->SetStrEleName("일반"/*"기타부재"*/);
		pRec->SetStrSubEleName("동바리용 고리");
		pRec->SetStrProcessBig("GIRDER");
		pRec->SetStrProcessMid("동바리");
		pRec->m_nQty = 0;
		
		if(bLower)	pRec->m_nQty++;
		if(bUpper)	pRec->m_nQty++;
		SetJewonBMRecord(pRec,pBx,nMarkBuje);
	}
	if(bRight)
	{
		CBMRecord *pRec = AddBMRecord();
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		pRec->m_nFirstClass		= FIRST_GIRDER;

		pRec->SetStrEleName("일반"/*"기타부재"*/);
		pRec->SetStrSubEleName("동바리용 고리");
		pRec->SetStrProcessBig("GIRDER");
		pRec->SetStrProcessMid("동바리");
		pRec->m_nQty = 0;

		if(bLower)	pRec->m_nQty++;
		if(bUpper)	pRec->m_nQty++;
		SetJewonBMRecord(pRec,pBx,nMarkBuje);
	}
}


void CBMCreator::AddBMJackUpStiffSub(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_JACKUP))	return;
	
	CPlateGirderApp *pGir = pBx->GetGirder();	

	for(long nCol=0; nCol<2; nCol++)
	{
		long nMarkBuje = nCol==0 ? GI_JA_L : GI_JA_R;

		CBMRecord *pRec = AddBMRecord();
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		pRec->m_nFirstClass = FIRST_GIRDER;
		pRec->SetStrSectionType("복부판");		
		pRec->SetStrProcessBig("GIRDER");
		pRec->SetStrProcessMid("JACK-UP보강");
		pRec->SetStrEleName("일반"/*"기타부재"*/);
		pRec->SetStrSubEleName("잭업보강재");
		pRec->m_nQty = 1;
	
		SetJewonBMRecord(pRec,pBx,nMarkBuje);	
	}
}

void CBMCreator::AddBMJiJumStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, __int64 BxFlag)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	CPlateGirderApp *pGirL = pGir->GetNumberGirder() > 0 ? pGir->m_pLeft : NULL;
	CPlateBxFinder Finder(pGir);
	
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	while(pBx)
	{
		for(long nCol=0; nCol<2; nCol++) //거더기준으로 좌우측.
		{
			CVStiff *pVStiff = pBx->GetJijumStiff();
			if(pVStiff->m_dWidth[nCol]==0 || pVStiff->m_dThick[nCol]==0) continue;

			CPlateBasicIndex* pBxL = pGirL ? pGirL->GetBxMatchByCrossBeam(pBx, pGirL) : NULL;
			
			long nMarkBuje = V_J_ST;		
			long	nRow	= 0;
			CVector vJewon,vJewonBasic;
			vJewon.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewon.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewon.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType),0);
			vJewonBasic.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			vJewonBasic.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);
			vJewonBasic.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC),0);

			CBMRecord * pRec = AddBMRecord();	
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
			pRec->m_nQty = 1;				
			pRec->SetStrEleName("일반");
			pRec->SetStrSubEleName("지점부보강재");
			// 지점부보강재에 제작타입 가로보가 설치되어 있을 경우 - 거더 좌측
			if(nCol == 0 && pBxL && pBxL->IsState(BX_CROSSBEAM_VBRACING))
			{
				CSection *pSec = pBxL->GetSection();
				CPlateCrossBeam *pC = pSec->IsenDetType(SECDET_CROSSBEAM) ? pSec->GetCrossBeam() : NULL;
				if(pC && pC->GetType() == CR_TYPE_MANUFACTURE)
				{
					vJewon.z -= (pC->m_H_CR + pC->m_T1_CR);
					vJewonBasic.z -= (pC->m_H_CR + pC->m_T1_CR);
					if(pC->m_dRH1 > 0)
					{
						vJewon.z -= pC->m_T2_CR;
						vJewonBasic.z -= pC->m_T2_CR;
					}
				}
			}
			// 지점부보강재에 제작타입 가로보가 설치되어 있을 경우 - 거더 우측
			if(nCol == 1 && pBx->IsState(BX_CROSSBEAM_VBRACING))
			{
				CSection		*pSec	= pBx->GetSection();
				CPlateCrossBeam	*pC		= pSec->IsenDetType(SECDET_CROSSBEAM) ? pSec->GetCrossBeam() : NULL;
				if(pC && pC->GetType() == CR_TYPE_MANUFACTURE)
				{
					vJewon.z -= (pC->m_H_CR + pC->m_T1_CR);
					vJewonBasic.z -= (pC->m_H_CR + pC->m_T1_CR);
					if(pC->m_dLH1 > 0)
					{
						vJewon.z -= pC->m_T2_CR;
						vJewonBasic.z -= pC->m_T2_CR;
					}
				}
			}	
			SetJewonBMRecord(pRec,pBx,nMarkBuje, 0, nCol, TRUE, TRUE, &vJewon, &vJewonBasic);
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}

void CBMCreator::AddBMJackUpStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, __int64 BxFlag)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	while( pBx )
	{
		if(pBx->IsState(BX_JACKUP))
		{
			if(pBx->GetJackupStiff())
				AddBMJackUpStiffSub(pBx);
		}
		pBx = Finder.GetBxNext();
	}
	// 잭업솔플레이트
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	while( pBx )
	{
		if(pBx->IsJijum())
			AddBMGitaBuje_JackUpPlate(pBx);
		pBx = Finder.GetBxNext();
	}
}
 
void CBMCreator::AddBMGitaBuje_SolePlate(CPlateBasicIndex* pBx)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();

	if(pDed->m_bDeductSolePlate) return;

	if(!pBx->IsJijum()) return;

	long nMarkBuje = GI_SO;
	CBMRecord *pRec = AddBMRecord();
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;
	pRec->SetStrEleName("일반"/*"기타부재"*/);
	pRec->SetStrProcessBig("GIRDER");
	pRec->SetStrProcessMid("SOLE");
	pRec->SetStrSubEleName("솔플레이트");
	pRec->SetStrSectionType("솔플레이트");
	long nRow = 0;
	long nCol = pGir->GetNumberJijumByBx(pBx);
	pRec->m_nQty = 1;
	SetJewonBMRecord(pRec,pBx,nMarkBuje,nRow,nCol);
}

void CBMCreator::AddBMGitaBuje_JackUpPlate(CPlateBasicIndex* pBx)
{
	if(!pBx->IsJijum()) return;
	CPlateBridgeApp	*pBridge = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir = pBx->GetGirder();
	long nJijum = pGir->GetNumberJijumByBx(pBx);
	for(long n=0; n<2; n++)
	{
		double dH = pBridge->m_structJackupJewon[nJijum][n].m_dH;
		double dW = pBridge->m_structJackupJewon[nJijum][n].m_dW;	
		if(dH>0 && dW>0)
		{
			long nMarkBuje  = GI_JA_SA;
			CBMRecord *pRec = AddBMRecord();
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
			pRec->m_nFirstClass = FIRST_GIRDER;
			pRec->SetStrEleName("기타부재");
			pRec->SetStrProcessBig("GIRDER");
			pRec->SetStrProcessMid("JACK-UP보강");
			pRec->SetStrSubEleName("잭업 솔플레이트");
			pRec->SetStrSectionType("솔플레이트");
			pRec->m_nQty = 1;
			SetJewonBMRecord(pRec,pBx,nMarkBuje,0,n);
		}
	}
}

void CBMCreator::SetJewonBMRecord(CBMRecord *pRec, CPlateBasicIndex* pBx, long nMarkBuje, long nRow, long nCol, BOOL bFront, BOOL bLeftBeam, CVector *pJewon, CVector *pJewonBasic)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CGangjeBase pBase(m_pDB);
	
	CGlobarOption *pGOpt = m_pDataManage->GetGlobalOption();

	long nRoundUnit = pGOpt->GetPointValueOfUnitGangJae();
	long nRoundTot  = pGOpt->GetPointValueOfGangJae();

	long nWeightBolt   = pGOpt->GetSummaryAddBoltWeight();
	long nWeightStud   = pGOpt->GetSummaryAddStudWeight();
	long nWeightAnchor = pGOpt->GetSummaryAddSlabAnchorWeight();
	long nWeightGori   = pGOpt->GetSummaryAddGoriWeight();

	CVector vJewon,vJewonBasic;
	if(!pJewon)
	{
		vJewon.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
		vJewon.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
		vJewon.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
	}
	else
		vJewon = *pJewon;

	// 단위중량의 단위를 조정.............
	pRec->m_nMarkBuje = nMarkBuje;
	pRec->m_vJewon	  = vJewon;
	pRec->m_sMaterial = m_pDB->GetMaterial(nMarkBuje,pBx);
	pRec->m_dUWeight  = Round(m_pDB->GetWeightUnitKgM(nMarkBuje,pBx, pRec->m_vJewon),nRoundUnit);
	pRec->m_dTWeight  = Round(pBase.GetWeightJewon(nMarkBuje,pBx,nRow,nCol,m_nDimType,bFront,bLeftBeam, &vJewon)
		                      * pRec->m_dFactorArea * pRec->m_nQty,nRoundTot);
	pRec->m_dAreaBuje = pBase.GetAreaBuje(nMarkBuje,pBx,nRow,nCol,m_nDimType,bFront) * pRec->m_nQty;
	pRec->m_dStationRec = pBx->GetStation();
	pRec->m_nKindName   = pBase.GetMaterialKindNumber(pRec->m_nMarkBuje);

	CSteelSection *pSec  = m_pDataManage->GetSteelSection();
	pRec->m_nSizeType = pSec->GetBujeSizeType(pRec->m_nMarkBuje)=="대형부재" ? 1 : pSec->GetBujeSizeType(pRec->m_nMarkBuje)=="소형부재" ? 2 : 3;

	if( (m_pDB->IsBolt(pRec->m_nMarkBuje) && nWeightBolt==0) || 
	   ((nMarkBuje==GI_SA)    && nWeightAnchor==0) ||
	   ((nMarkBuje==GI_SB)    && nWeightStud==0)   ||
	   ((nMarkBuje==GI_LP)    && nWeightGori==0)   ||
	   ((nMarkBuje==GI_LP_R)  && nWeightGori==0)   ||
	   ((nMarkBuje==GI_LP_PL) && nWeightGori==0)
	  )
	{
		pRec->m_dTWeight	  = 0;
		pRec->m_dTWeightBasic = 0;
		pRec->m_dUWeight	  = 0;
	}
	// BASIC DATA
	if(!pJewonBasic)
	{
		vJewonBasic.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC, bFront),0);
		vJewonBasic.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC, bFront),0);
		vJewonBasic.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, DIM_BASIC, bFront),0);
	}
	else
		vJewonBasic = *pJewonBasic;
	pRec->m_vJewonBasic    = vJewonBasic;
	pRec->m_dTWeightBasic  = pBase.GetWeightJewon(nMarkBuje,pBx,nRow,nCol,DIM_BASIC,bFront,bLeftBeam,&vJewonBasic)
							* pRec->m_dFactorAreaBasic * pRec->m_nQty;
}

void CBMCreator::AddBMWeldStud(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_SPLICE,pBxStt,pBxEnd,TRUE,FALSE);

	while( pBx )
	{
		CPlateGirderApp *pGir = pBx->GetGirder();
		CBMRecord		*pRec = AddBMRecord();

		long	nType	 = pGir->m_pSangse->m_StudHori_Type;	// 0 앵커, 1 스터드, 2 각형
		long	nMarkBuje;
		CString szBuje;
		if(nType == 0)
		{
			nMarkBuje = GI_SA;
			szBuje.Format("슬래브 앵커");
		}
		else if(nType == 1)
		{
			nMarkBuje = GI_SB;
			szBuje.Format("전단연결재");
		}
		else
		{
			nMarkBuje = GI_SQ;
			szBuje.Format("전단연결재 각형");
		}

		pRec->m_nGirder = pBx->GetNumberGirder();
		
		pRec->m_nFirstClass		= FIRST_GIRDER;
		pRec->SetStrEleName("일반"/*"기타부재"*/);
		pRec->SetStrProcessBig("BOLT");
		pRec->SetStrProcessMid("STUD BOLT");
		pRec->SetStrSectionType(szBuje);	// Weld Stud
		pRec->SetStrSubEleName(szBuje);	// Weld Stud
		long nSp = pGir->GetNumberSpliceByBx(pBx);
		
		//Splice번호의 동기화
		if(pBx->GetSplice()) nSp += 1;
		
		pRec->m_nSplice = nSp;

		pRec->m_nQty = pBx->GetGirder()->GetStudBoltSu(nSp);
		if(nType != 1)	// 전단연결재의 숫자 계산으로, 스터드 수만큼 곱해져 있다..
			pRec->m_nQty /= pGir->m_pSangse->m_StudHori_Qty;

		SetJewonBMRecord(pRec,pBx,nMarkBuje);
		//
		pBx = Finder.GetBxNext();
	}
}

// FACE PLATE
void CBMCreator::AddBMFacePlate(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp	*pGir = pBxStt->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	if(!pDB->m_cFacePlateUse) return;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_STT_GIRDER|BX_END_GIRDER,pBxStt,pBxEnd);

	while( pBx )
	{
		CPlateGirderApp *pGir = pBx->GetGirder();
		CBMRecord *pRec = AddBMRecord();
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		pRec->m_nFirstClass		= FIRST_GIRDER;
		pRec->SetStrEleName("기타부재");
		pRec->SetStrSectionType("복부판");
		pRec->SetStrSubEleName("FACE PLATE");
		pRec->SetStrProcessBig("GIRDER");
		pRec->SetStrProcessMid("MAIN WEB RING");
		pRec->m_nQty = 1;
		SetJewonBMRecord(pRec,pBx,GI_FA);
		//
		pBx = Finder.GetBxNext();
	}
}

// 주판
void CBMCreator::DeductBMFactChain(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,long nMarkFactType)
{
	CPlateGirderApp		*pGir = pBxStt->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();

	CPlateBxFinder Finder(pGir);
	__int64 bxFlag;

	switch( nMarkFactType )
	{
		case G_F_U : 
			bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_UPPERFLANGE|BX_SPLICE;
			break;
		case G_F_L : 
			bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_LOWERFLANGE|BX_SPLICE;
			break;
		case G_W : 
			bxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_WEB|BX_SPLICE;
			pBxStt = pBxStt->GetBxLeft();	
			pBxEnd = pBxEnd->GetBxLeft();
			break;		
	}

	CPlateBasicIndex* pBx = Finder.GetBxFirst(bxFlag,pBxStt,pBxEnd);
	while(Finder.GetBxFixNext())
	{
		switch( nMarkFactType )
		{
			case G_F_U : 
				if(pDed->m_bDeductUpperFlange)
				{
					DeductBMFlangeSkew(pBx,TRUE);
					if(Finder.GetBxFixNext())
						DeductBMFlangeSkew(Finder.GetBxFixNext(),TRUE);
				}
				if(pDed->m_bDeductBoltHole)
				{
					DeductBMUpperFlangeBoltHole(pBx,FALSE);
					if(Finder.GetBxFixNext())
						DeductBMUpperFlangeBoltHole(Finder.GetBxFixNext(),TRUE);
				}
				break;
			case G_F_L :
				if(pDed->m_bDeductLowerFlange)
				{
					DeductBMFlangeSkew(pBx,FALSE);
					if(Finder.GetBxFixNext())
						DeductBMFlangeSkew(Finder.GetBxFixNext(),FALSE);
				}
				if(pDed->m_bDeductBoltHole)
				{
					DeductBMLowerFlangeBoltHole(pBx,FALSE);
					if(Finder.GetBxFixNext())
						DeductBMLowerFlangeBoltHole(Finder.GetBxFixNext(),TRUE);
				}
				break;
			case G_W :							
				if(pDed->m_bDeductWeb)
				{
					DeductBMWebTaper(pBx);					
					if(Finder.GetBxFixNext())											
						DeductBMWebTaper(Finder.GetBxFixNext());										
				}
				if(pDed->m_bDeductBoltHole)
				{
					DeductBMWebBoltHole(pBx,FALSE);
					if(Finder.GetBxFixNext())
						DeductBMWebBoltHole(Finder.GetBxFixNext(),TRUE);
				}
				break;
		}
		pBx = Finder.GetBxNext();
	}	
}


void CBMCreator::DeductBMFlangeSkew(CPlateBasicIndex* pBx, BOOL bUpper)
{
	if(!pBx->IsState(BX_STT_GIRDER|BX_END_GIRDER)) return;

	CDPoint vAng = pBx->GetAngleSection();
	if(vAng.x==0 || vAng.y==1) return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	long nMarkBuje = bUpper ? G_F_U : G_F_L;
	double dWidth  = pGir->GetWidthOnStation(pBx->GetStation(), bUpper);
	if(fabs(dWidth*vAng.x/vAng.y)<10.0) return;	// 10mm보다 작으면 공제하지 않음.

	CBMRecord * pRec = AddBMRecord();
	pRec->m_nGirder  = pBx->GetNumberGirder();
	pRec->m_nSplice  = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;
	pRec->m_nQty = -1;
	pRec->SetStrEleName("일반");
	pRec->SetStrProcessBig("MAIN P/N");
	pRec->SetStrProcessMid("비연결재");
	if(pGir->GetFactConnectType(pBx,nMarkBuje)==1) pRec->SetStrProcessMid("끝단재");
	if(pGir->GetFactConnectType(pBx,nMarkBuje)==2) pRec->SetStrProcessMid("중간재");
	pRec->SetStrSubEleName(bUpper ? "상판" : "하판");
	pRec->SetStrSectionType(bUpper ? "상판" : "하판");
	//
	pRec->m_vJewon.x  = 0;
	pRec->m_vJewon.y  = pGir->GetThickFactByStation(pBx->GetStation(), nMarkBuje);
	pRec->m_vJewon.z  = fabs(pRec->m_vJewon.x*vAng.x/vAng.y);
	pRec->m_dAreaBuje = (pRec->m_vJewon.x*pRec->m_vJewon.z)/2;
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

// 상부플랜지 볼트홀
void CBMCreator::DeductBMUpperFlangeBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateSplice	    *pSp  = pBx->GetSplice();
	CGangjeBase pBase(pDB);

	CBMRecord * pRec = AddBMRecord();
	pRec->m_nGirder  = pBx->GetNumberGirder();
	pRec->m_nSplice  = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;
	if(pBx->IsState(BX_SPLICE) && !bEndBx) pRec->m_nSplice++;
	pRec->m_nQty = -1;
	pRec->SetStrEleName("일반");
	pRec->SetStrSubEleName("상판");
	pRec->SetStrSectionType("상판");
	pRec->SetStrProcessBig("MAIN P/N");
	pRec->SetStrProcessMid("비연결재");
	if(pGir->GetFactConnectType(pBx,G_F_U)==1) pRec->SetStrProcessMid("끝단재");
	if(pGir->GetFactConnectType(pBx,G_F_U)==2) pRec->SetStrProcessMid("중간재");
	
	long nMarkBuje = G_F_U;
	if(pBx->GetSplice()->m_uG>0)
		pRec->m_nQty -= (long)((pSp->m_uB+1)*(pSp->m_uE+1)*4);
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

void CBMCreator::DeductBMLowerFlangeBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateSplice	    *pSp  = pBx->GetSplice();
	CGangjeBase pBase(pDB);

	CBMRecord * pRec = AddBMRecord();
	pRec->m_nGirder  = pBx->GetNumberGirder();
	pRec->m_nSplice  = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;
	if(pBx->IsState(BX_SPLICE) && !bEndBx) pRec->m_nSplice++;
	pRec->m_nQty = -1;
	pRec->SetStrEleName("일반");
	pRec->SetStrSubEleName("하판");
	pRec->SetStrSectionType("하판");
	pRec->SetStrProcessBig("MAIN P/N");
	pRec->SetStrProcessMid("비연결재");
	if(pGir->GetFactConnectType(pBx,G_F_L)==1) pRec->SetStrProcessMid("끝단재");
	if(pGir->GetFactConnectType(pBx,G_F_L)==2) pRec->SetStrProcessMid("중간재");

	long nMarkBuje = G_F_L;
	if(pBx->GetSplice()->m_dG>0)
		pRec->m_nQty -= (long)((pSp->m_dB+1)*(pSp->m_dE+1)*4 );
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

void CBMCreator::DeductBMWebBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateSplice	    *pSp  = pBx->GetSplice();
	CGangjeBase pBase(pDB);

	CBMRecord * pRec = AddBMRecord();
	pRec->m_nGirder  = pBx->GetNumberGirder();
	pRec->m_nSplice  = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;
	if(pBx->IsState(BX_SPLICE) && !bEndBx) pRec->m_nSplice++;
	long nMarkBuje = G_W;
	//
	pRec->m_nQty = -1;
	pRec->SetStrEleName("일반");
	pRec->SetStrProcessBig("MAIN P/N");
	pRec->SetStrProcessMid("비연결재");
	if(pGir->GetFactConnectType(pBx,nMarkBuje)==1) pRec->SetStrProcessMid("끝단재");
	if(pGir->GetFactConnectType(pBx,nMarkBuje)==2) pRec->SetStrProcessMid("중간재");
	pRec->SetStrSubEleName("복부판");
	pRec->SetStrSectionType("복부판");
	//
	pRec->m_nQty   = -(long)( (pSp->m_sB+1)*(pSp->m_sE+pSp->m_sE1*2+1) + (pSp->m_mB+1)*(pSp->m_mE+1)*2 );
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

void CBMCreator::DeductBMWebTaper(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_STT_GIRDER|BX_END_GIRDER)) return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGangjeBase pBase(pDB);

	CBMRecord * pRec = AddBMRecord();
	pRec->m_nGirder  = pBx->GetNumberGirder();
	pRec->m_nSplice  = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;
	pRec->m_nQty = -1;
	long nMarkBuje = G_W;

	pRec->SetStrEleName("일반");
	pRec->SetStrProcessBig("MAIN P/N");
	pRec->SetStrProcessMid("비연결재");
	if(pGir->GetFactConnectType(pBx,nMarkBuje)==1) pRec->SetStrProcessMid("끝단재");
	if(pGir->GetFactConnectType(pBx,nMarkBuje)==2) pRec->SetStrProcessMid("중간재");
	pRec->SetStrSubEleName("복부판");
	pRec->SetStrSectionType("복부판");
	//
	pRec->m_vJewon    = pBase.GetJewonDeductWebTaper(pBx,TRUE);
	pRec->m_dAreaBuje = pBase.GetAreaDeductWebTaper(pBx,TRUE);
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

void CBMCreator::DeductBMVStiffner(CPlateBasicIndex* pBx)
{
	/*
	CDanmyun   *pDan = pBx->GetDanmyun();
	CPlateGirderApp *pGir = pBx->GetGirder();
	
	if(pDan->m_vT1_DaDa==0 || pDan->m_vW1_DaDa==0) return;
	if(pDan->m_vH_DaDa==0)	return;

	CBMRecord *pRec = AddBMRecord();
	SetBMVStiffRecord(pRec,pBx);

	long nRow=0;
	long nCol=0;
	long nMarkBuje = B_V;
	pRec->m_nQty = -2;
	pRec->m_vJewon.x  = pDan->m_vW1_DaDa-pDan->m_vV_DaDa;
	pRec->m_vJewon.y  = pDan->m_vT1_DaDa;
	pRec->m_vJewon.z  = pDan->m_vW1_DaDa-pDan->m_vV_DaDa;
	pRec->m_dAreaBuje = (pRec->m_vJewon.x*pRec->m_vJewon.z)/2;
	SetDeductRecord(pRec,pBx,nMarkBuje,nRow,nCol);
	*/
}

void CBMCreator::DeductBMHStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,BOOL bLeft)
{
	// m_nDimType : dimtype 사용
	CPlateGirderApp*	pGir = pBxStt->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();;

	CPlateBxFinder Finder(pGir);
	__int64 bxFlag;
	CString sKindName, sEleName, sSubEleName;
	long nMarkPiece;

	sEleName	 = "일반";
	sSubEleName  = "수평보강재";
	bxFlag		 = BX_PLATEHSTIFF;
	nMarkPiece	 = bLeft ? G_H_L : G_H_R;
	sKindName	 = "복부판";
	
	CPlateBasicIndex* pBx = Finder.GetBxFirst(bxFlag,pBxStt,pBxEnd,TRUE,FALSE);
	
	long nCol = 0;
	while( pBx )
	{
		long nSide = bLeft ? -1 : 1;
		CWebHStiff *pHs = pBx->GetWebHStiff();
		for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
		{
			if(pHs==NULL)	break;
			if(pHs->IsEmpty(nDan, bLeft))	continue;
			double dW   = pHs->m_dWidthHStiff[bLeft ? 0 : 1];
			double dL   = pHs->m_dLengthHStiff[bLeft ? 0 : 1][nDan];
			double dCW  = dW - pHs->m_dDistStt[bLeft ? 0 : 1][nDan];
			double dCL  = 155;//<<== 대우 pH->m_dDistStt[nDan]; // APiece의 설정치와 동일.....
			double dCL2 = 155;//<<== 대우 pH->m_dDistEnd[nDan];
			double dCW2 = dW - pHs->m_dDistEnd[bLeft ? 0 : 1][nDan];
			if(dCL + dCL2 >= dL) continue;
			//
			CBMRecord * pRec = AddBMRecord();
		
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
			pRec->m_nFirstClass = FIRST_GIRDER;
			if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
			pRec->m_nQty = -2;
			pRec->SetStrEleName(sEleName);
			pRec->SetStrSubEleName(sSubEleName);
			pRec->SetStrProcessBig("GIRDER");
			pRec->SetStrProcessMid("H-STIFF");
			pRec->SetStrSectionType(sKindName);

			// 공제물량 산정 (양쪽의 공제량은 같다고 가정) => nQty = -2
			pRec->m_vJewon.x  = dCW;
			pRec->m_vJewon.y  = pHs->m_dThickHStiff[bLeft ? 0 : 1];
			pRec->m_vJewon.z  = (dL-dCL-dCL2)/2;
			pRec->m_dAreaBuje = (pRec->m_vJewon.x*pRec->m_vJewon.z)/2;
			SetDeductRecord(pRec,pBx,nMarkPiece,nDan,nCol);
		}
		pBx = Finder.GetBxNext();
	}
}


// 필히 수량,면적,제원을 설정한후에 호출할 것
// 면적계수 실제면적/(W*L);
void CBMCreator::SetDeductRecord(CBMRecord *pRec, CPlateBasicIndex* pBx, long nMarkBuje, long nRow, long nCol, BOOL bFront, BOOL bLeftBeam)
{
	if(pRec->m_nQty>=0)			return;
	if(pRec->m_dAreaBuje==0)	return;

	CPlateGirderApp *pGir    = pBx->GetGirder();
	CGlobarOption   *pGloOpt = m_pDataManage->GetGlobalOption(); 
	CGangjeBase pBase(m_pDB);

	long nRoundUnit = pGloOpt->GetPointValueOfUnitGangJae();
	long nRoundTot  = pGloOpt->GetPointValueOfGangJae();
	long nRoundArea = 2;

	pRec->m_nMarkBuje	= nMarkBuje;
	pRec->m_sMaterial	= m_pDB->GetMaterial(nMarkBuje, pBx);
	pRec->m_dUWeight    = Round(m_pDB->GetWeightUnitKgM(nMarkBuje,pBx, pRec->m_vJewon), nRoundUnit);
	// 면적계수
	pRec->m_dFactorArea = pRec->m_dAreaBuje / (pRec->m_vJewon.x*pRec->m_vJewon.z); 
	// 공제중량
	double W = toM(Round(pRec->m_vJewon.x,0));
	double T = toM(Round(pRec->m_vJewon.y,0));
	double L = toM(Round(pRec->m_vJewon.z,0));
	pRec->m_dTWeight	= Round( W*T*L*pRec->m_dFactorArea*pRec->m_dUWeight*pRec->m_nQty,nRoundTot);
	pRec->m_dStationRec = pBx->GetStation();
	pRec->m_nKindName   = pBase.GetMaterialKindNumber(pRec->m_nMarkBuje);
	// BASIC DATA
	pRec->m_vJewonBasic		 = pRec->m_vJewon;
	pRec->m_dFactorAreaBasic = pRec->m_dFactorArea;
	pRec->m_dTWeightBasic	 = pRec->m_dTWeight;
}

void CBMCreator::DeductBMSpliceUpperBoltHole(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();;
	CPlateSplice	   *pSp  = pBx->GetSplice();
	CGangjeBase pBase(pDB);

	CBMRecord * pRec = AddBMRecord();
	SetBMSpliceRecord(pRec,pBx);
	pRec->SetStrEleName("상부 현장이음");

	// 상면 이음판
	long nSpsu  = 1;
	long nMarkBuje = SP_U_U;
	pRec->SetStrSubEleName("상면 이음판");
	pRec->m_nQty -= (long)((pSp->m_uB+1)*(pSp->m_uE+1)*4);
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);

	// 내측 이음판 (돌출부) : 좌,우로 2개
	for(long n=0; n<2; n++) 
	{
		nMarkBuje = SP_U_L;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("하면 이음판");
		pRec->m_nQty	  = -(long)((pSp->m_uB+1)*(pSp->m_uE+1)*2);
		pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
		pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
		SetDeductRecord(pRec,pBx,nMarkBuje);
	}

	// 필러 플레이트
	BOOL bNextFiller = TRUE;
	if(!pBx->IsFiller(BX_UPPERFLANGE, bNextFiller))	return;

	nMarkBuje = SP_U_FILL;
	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("채움판");
	pRec->m_nQty	  = -(long)((pSp->m_uB+1)*(pSp->m_uE+1)*2);
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

void CBMCreator::DeductBMSpliceLowerBoltHole(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();;
	CPlateSplice	   *pSp  = pBx->GetSplice();
	CGangjeBase pBase(pDB);

	CBMRecord * pRec = AddBMRecord();
	SetBMSpliceRecord(pRec,pBx);
	pRec->SetStrEleName("하부 현장이음");

	// 외측 이음판
	long nSpsu  = 1;
	long nMarkBuje = SP_L_L;
	pRec->SetStrSubEleName("하면 이음판");
	pRec->m_nQty -= (long)((pSp->m_dB+1)*(pSp->m_dE+1)*4);
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);

	// 내측 이음판 (돌출부) : 좌,우로 2개
	for(long n=0; n<2; n++) 
	{
		nMarkBuje = SP_L_U;
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("상면 이음판");
		pRec->m_nQty	  = -(long)((pSp->m_dB+1)*(pSp->m_dE+1)*2);
		pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
		pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
		SetDeductRecord(pRec,pBx,nMarkBuje);
	}

	// 필러 플레이트
	BOOL bNextFiller = TRUE;
	if(!pBx->IsFiller(BX_LOWERFLANGE, bNextFiller))	return;

	nMarkBuje = SP_L_FILL;
	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("채움판");
	pRec->m_nQty	  = -(long)((pSp->m_dB+1)*(pSp->m_dE+1)*2);
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

void CBMCreator::DeductBMSpliceWebBoltHole(CPlateBasicIndex* pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();;
	CPlateSplice	   *pSp  = pBx->GetSplice();
	CGangjeBase pBase(pDB);

	CBMRecord * pRec = AddBMRecord();
	SetBMSpliceRecord(pRec,pBx);
	pRec->SetStrEleName("복부판 현장이음");

	// 전단 이음판
	long nMarkBuje = SP_W_SH;
	pRec->SetStrSubEleName("전단 이음판");
	pRec->m_nQty	  = -(long)( (pSp->m_sB+1)*(pSp->m_sE+pSp->m_sE1*2+1)*2 );
	if(pSp->m_nWebType==SP_WEB_FLMO)	// 일체형
		pRec->m_nQty	  -= (long)( (pSp->m_mB+1)*(pSp->m_mE+1)*2 )*2;
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);	
	SetDeductRecord(pRec,pBx,nMarkBuje);

	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("전단 이음판");
	SetDeductRecord(pRec,pBx,nMarkBuje);

	if(pSp->m_nWebType==SP_WEB_MO)		// 분리형
	{		
		for(long a=0; a<2; a++)//좌우 
		{
			for(long n=0; n<2; n++) //상하
			{
				pRec = AddBMRecord(pRec);
				pRec->SetStrSubEleName("모멘트 이음판");
				pRec->m_nQty	  = -(long)( (pSp->m_mB+1)*(pSp->m_mE+1)*2 );
				pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(SP_W_MO,pBx,pSp->m_ZzsBoltSize);
				pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(SP_W_MO,pBx,pSp->m_ZzsBoltSize);
				SetDeductRecord(pRec,pBx,SP_W_MO);
			}
		}
	}

	BOOL bNextFiller = TRUE;
	if(!pBx->IsFiller(BX_WEB, bNextFiller))	return;

	// 전단 이음판
	nMarkBuje = SP_W_FILL;
	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("채움판");
	pRec->m_nQty	  = -(long)( (pSp->m_sB+1)*(pSp->m_sE+pSp->m_sE1*2+1) );
	if(pSp->m_nWebType!=SP_WEB_FL)	// 모멘트판 부분
		pRec->m_nQty	  -= (long)( (pSp->m_mB+1)*(pSp->m_mE+1) )*2;
	pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(nMarkBuje,pBx,pSp->m_ZzsBoltSize);
	SetDeductRecord(pRec,pBx,nMarkBuje);

	pRec = AddBMRecord(pRec);
	pRec->SetStrSubEleName("채움판");
	SetDeductRecord(pRec,pBx,nMarkBuje);
}

void CBMCreator::SetBMVStiffRecord(CBMRecord *pRec, CPlateBasicIndex* pBx)
{
	/*
	CDanmyun   *pDan = pBx->GetDanmyun();
	CPlateGirderApp *pGir = pBx->GetGirder();
	
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;

	switch(pDan->m_enType) 
	{
		case CDanmyun::TYPE_B:
			pRec->SetStrEleName("B",pDan->m_enTypeNum);
			break;
		case CDanmyun::TYPE_D:
			pRec->SetStrEleName("D",pDan->m_enTypeNum);
			break;
		case CDanmyun::TYPE_E:
			pRec->SetStrEleName("E",pDan->m_enTypeNum);
			break;
		case CDanmyun::TYPE_F:
			pRec->SetStrEleName("F",pDan->m_enTypeNum);
			break;
		case CDanmyun::TYPE_G:
			pRec->SetStrEleName("G",pDan->m_enTypeNum);
			break;
	}
	pRec->SetStrSubEleName("수직보강재");
	pRec->SetStrSectionType("수직보강재");
	pRec->SetStrProcessBig("SUB DIAPH");
	pRec->SetStrProcessMid("V-STIFF");
	pRec->m_nMarkAssem		= ASSEMBLY_V;
	pRec->m_nMarkGrandAssem = ASSEMBLYBIG_BOX;
	pRec->m_nMarkSection	= SECTION_MAIN;
	pRec->m_sMarkAssem		= GetMarkBx(CMarkSerial::MARK_ASSEM,ASSEMBLY_V,pBx);
	pRec->m_sMarkGrandAssem	= GetMarkBx(CMarkSerial::MARK_ASSEM_BIG,ASSEMBLYBIG_BOX,pBx);
	pRec->m_sMarkSection	= GetMarkBx(CMarkSerial::MARK_SECTION,SECTION_MAIN,pBx);
	*/
}

// 가로보
void CBMCreator::DeductBMCrossBeamMain(CPlateBasicIndex* pBx)
{
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();;
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();
	CPlateBasicIndex* pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	////////////////////////////
	CDPoint A[10];
	pC->GetCrossXyDom(A);
	double webT    = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double webTR	= pGir->m_pRight->GetThickFactByStation(pBxNext->GetStation(), 2);		

	if(pDed->m_bDeductCrossBeamWebPlate && fabs(A[5].y-A[2].y)>1.0)	// 높이차가 거의 없는경우는 공제가 무의미
	{
		// LEVEL타입
		// 사각형공제(LEVEL타입일 경우)+삼각형공제, 상.하
		if(m_pDataManage->GetGlobalOption()->GetDesignCrossBeamBracketType()==1)
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("복부판");
			pRec->m_nQty = -2;
			pRec->m_vJewon.x  = fabs(A[4].y-A[3].y);
			pRec->m_vJewon.y  = pC->m_T3_CR;
			pRec->m_vJewon.z  = fabs(A[4].x-A[3].x);
			pRec->m_dAreaBuje = pRec->m_vJewon.x*pRec->m_vJewon.z;
			SetDeductRecord(pRec,pBx,CP_W);
			//
			pRec = AddBMRecord(pRec);
			pRec->m_nQty = -2;
			pRec->m_vJewon.x  = fabs(A[4].y-A[3].y);
			pRec->m_vJewon.y  = pC->m_T3_CR;
			pRec->m_vJewon.z  = fabs(A[4].x-A[3].x);
			pRec->m_dAreaBuje = pRec->m_vJewon.x*pRec->m_vJewon.z/2;
			SetDeductRecord(pRec,pBx,CP_W);
		}
		// 일반타입
		else
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("복부판");
			pRec->m_nQty = -2;
			if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			{
				pRec->m_vJewon.x  = fabs(A[5].y-A[2].y);
				pRec->m_vJewon.y  = pC->m_T3_CR;
				pRec->m_vJewon.z  = fabs(A[5].x-A[2].x);
			}
			else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
			{
				pRec->m_vJewon.x  = fabs(A[5].y-A[8].y)-webT-pC->GetLengthGirWebToCrWeb(TRUE);
				pRec->m_vJewon.y  = pC->m_T3_CR;
				pRec->m_vJewon.z  = fabs(A[5].x-A[8].x);
			}
			else if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			{
				pRec->m_vJewon.x  = fabs(A[9].y-A[2].y)-webTR-pC->GetLengthGirWebToCrWeb(FALSE);
				pRec->m_vJewon.y  = pC->m_T3_CR;
				pRec->m_vJewon.z  = fabs(A[9].x-A[2].x);
			}
			else
			{
				pRec->m_vJewon.x  = fabs(A[9].y-A[8].y)-webT-webTR-pC->GetLengthGirWebToCrWeb(TRUE)-pC->GetLengthGirWebToCrWeb(FALSE);
				pRec->m_vJewon.y  = pC->m_T3_CR;
				pRec->m_vJewon.z  = fabs(A[9].x-A[8].x);
			}
			pRec->m_dAreaBuje = pRec->m_vJewon.x*pRec->m_vJewon.z/2;
			SetDeductRecord(pRec,pBx,CP_W);
		}
	}

	//볼트홀 공제
	if(pDed->m_bDeductBoltHole)
	{
		CGangjeBase pBase(pDB);
		CBMRecord * pRec = AddBMRecord();
		SetBMCrossBeamRecord(pRec,pBx);
		pRec->SetStrSubEleName("복부판");

		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			pRec->m_nQty = -(pC->m_sB+1)*(pC->m_sE+1)*2;
		else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			pRec->m_nQty = -(pC->m_wE+1)*2;
		else if((!pC->m_nLGirSpType && pC->m_nRGirSpType)
			   ||(pC->m_nLGirSpType && !pC->m_nRGirSpType))
			pRec->m_nQty = -(pC->m_wE+1);

		pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_W,pBx,pC->m_ZzsBoltSize);
		pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_W,pBx,pC->m_ZzsBoltSize);
		SetDeductRecord(pRec,pBx,CP_W);
		//
		long nZigzag = m_pDataManage->GetGlobalOption()->GetDesignBoltHoleJigJaegOfHorPlate();
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("상판");
		long nBoltsu = (long)(pC->m_uB+1)*(pC->m_uE+1)*2*2;
		if(nZigzag==0 && pC->m_uE>0) // 지그재그 적용
		{
			if(pC->m_uE%2==1 || pC->m_uB%2==1)	
				nBoltsu = (long)(nBoltsu/2);
			else
				nBoltsu = (long)(nBoltsu/2)+1;
		}
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			pRec->m_nQty = -nBoltsu;
		else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			pRec->m_nQty = 0;
		else if((!pC->m_nLGirSpType && pC->m_nRGirSpType)
			   ||(pC->m_nLGirSpType && !pC->m_nRGirSpType))
			pRec->m_nQty = -nBoltsu/2;

		pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_F_U,pBx,pC->m_ZzsBoltSize);
		pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_F_U,pBx,pC->m_ZzsBoltSize);
		SetDeductRecord(pRec,pBx,CP_F_U);
		//
		pRec = AddBMRecord(pRec);
		pRec->SetStrSubEleName("하판");
		nBoltsu = (long)(pC->m_dB+1)*(pC->m_dE+1)*2*2;
		if(nZigzag==0 && pC->m_dE>0) // 지그재그 적용
		{
			if(pC->m_dE%2==1 || pC->m_dB%2==1)	
				nBoltsu = (long)(nBoltsu/2);
			else
				nBoltsu = (long)(nBoltsu/2)+1;
		}
		if(pC->m_nLGirSpType && pC->m_nRGirSpType)
			pRec->m_nQty = -nBoltsu;
		else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			pRec->m_nQty = 0;
		else if((!pC->m_nLGirSpType && pC->m_nRGirSpType)
			   ||(pC->m_nLGirSpType && !pC->m_nRGirSpType))
			pRec->m_nQty = -nBoltsu/2;
		
		pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_F_L,pBx,pC->m_ZzsBoltSize);
		pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_F_L,pBx,pC->m_ZzsBoltSize);
		SetDeductRecord(pRec,pBx,CP_F_L);
	}

	////////////////////////////
	if(pDed->m_bDeductVerStiff)
	{
		double D = 10; // 가로보복부와의 LEVEL 거리(??) : APiece의 값과 동일
		for(long n=0; n<pC->m_nVStiffsu_CR; n++) 
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("수직보강재");
			pRec->m_nQty = pC->m_cVSide_CR ? -1 : -2;
			
			pRec->m_vJewon.x  = pC->m_vW_CR - D;
			pRec->m_vJewon.y  = pC->m_vT_CR;
			pRec->m_vJewon.z  = pC->m_vW_CR - D;
			pRec->m_dAreaBuje = pRec->m_vJewon.x*pRec->m_vJewon.z/2;
			SetDeductRecord(pRec,pBx,CP_V);
		}
	}

	////////////////////////////
/*	if(pDed->m_bDeductHStiff)
	{
		CDPoint Po[18];
		//CBaseChief *pBC	= (CBaseChief*)pDB->m_pBaseChief;
		BOOL bIncludeGasset = pGlopt->GetDesignCrossBeamHStiffAtBracket()==0 ? TRUE : FALSE;
		CGangjeBase pBase(pDB);

		long nH = pC->m_nHStiffUppersu_CR + pC->m_nHStiffLowersu_CR;
		if(nH) 
		{
			long nV = pGir->GetPointVStiffOnCrossBeam(pBx,Po);
			long nColStt = bIncludeGasset ? 0 : 1;
			long nColEnd = bIncludeGasset ? nV-1 : nV-2;

			for(long nRow=0; nRow<nH; nRow++) 
			{
				for(long nCol=nColStt; nCol<nColEnd; nCol++) 
				{
					CBMRecord * pRec = AddBMRecord();
					SetBMCrossBeamRecord(pRec,pBx,ASSEMBLY_CR);
					pRec->SetStrSubEleName("수평보강재");
					pRec->m_nQty = pC->m_cVSide_CR ? -1 : -2;
					//
					pRec->m_dAreaBuje = pBase.GetAreaDeductCrossBeamHStiff(CP_H,pBx,nRow,nCol);
					pRec->m_vJewon    = pBase.GetJewonDeductCrossBeamHStiff(CP_H,pBx,nRow,nCol);
					SetDeductRecord(pRec,pBx,CP_H);
				}
			}
		}
	}*/
}

void CBMCreator::DeductBMCrossBeamBracket(CPlateBasicIndex* pBx, BOOL bLeft)
{
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();
	CGangjeBase pBase(pDB);

	long nCol = bLeft ? 0 : 1;
	long nRow = 0;	
	// 브라켓 상하판 공제
	if(pDed->m_bDeductCrossBeamBracketUDFlange)
	{
		if((pC->m_nLGirSpType && bLeft==TRUE)
			||(pC->m_nRGirSpType && bLeft==FALSE))
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 상판");
		//	pRec->SetStrProcessMid("T-FLG");
			pRec->m_nQty = -2;
			pRec->m_vJewon    = pBase.GetJewonDeductCrossBeamBracketFlange(CP_BR_F_U,pBx,nRow,nCol);
			pRec->m_dAreaBuje = pBase.GetAreaDeductCrossBeamBracketFlange(CP_BR_F_U,pBx,nRow,nCol);
			SetDeductRecord(pRec,pBx,CP_BR_F_U);
			//
			pRec = AddBMRecord(pRec);
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 하판");
		//	pRec->SetStrProcessMid("B-FLG");
			pRec->m_nQty = -2;
			pRec->m_vJewon    = pBase.GetJewonDeductCrossBeamBracketFlange(CP_BR_F_L,pBx,nRow,nCol);
			pRec->m_dAreaBuje = pBase.GetAreaDeductCrossBeamBracketFlange(CP_BR_F_L,pBx,nRow,nCol);
			SetDeductRecord(pRec,pBx,CP_BR_F_L);
		}
	}

	// 브라켓 복부판 공제
	double dDiff = pGir->GetGiulPlateCrossGusset(pBx)*pC->m_P_CR;
	if(pDed->m_bDeductCrossBeamBracketWebPlate && dDiff>1.0)
	{
		if((pC->m_nLGirSpType && bLeft==TRUE)
			||(pC->m_nRGirSpType && bLeft==FALSE))
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 복부판");
		//	pRec->SetStrProcessMid("WEB");
			pRec->m_nQty = -2;
			pRec->m_vJewon    = pBase.GetJewonDeductCrossBeamBracketWeb(CP_BR_W,pBx,nRow,nCol);
			pRec->m_dAreaBuje = pBase.GetAreaDeductCrossBeamBracketWeb(CP_BR_W,pBx,nRow,nCol);
			SetDeductRecord(pRec,pBx,CP_BR_W,nRow,nCol);
		}
	}

	//볼트홀 공제
	if(pDed->m_bDeductBoltHole)
	{
		if((pC->m_nLGirSpType && bLeft==TRUE)
			||(pC->m_nRGirSpType && bLeft==FALSE))
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 복부판");
//			pRec->SetStrProcessMid("WEB");
			pRec->m_nQty = -(pC->m_sB+1)*(pC->m_sE+1);
			pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_BR_W,pBx,pC->m_ZzsBoltSize);
			pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_BR_W,pBx,pC->m_ZzsBoltSize);
			SetDeductRecord(pRec,pBx,CP_BR_W,nRow,nCol);
			//
			pRec = AddBMRecord(pRec);
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 상판");
	//		pRec->SetStrProcessMid("T-FLG");
			long nBoltsu = (long)(pC->m_uB+1)*(pC->m_uE+1)*2;
			long nZigzag = m_pDataManage->GetGlobalOption()->GetDesignBoltHoleJigJaegOfHorPlate();
			if(nZigzag==0 && pC->m_uE>0) // 지그재그 적용
			{
				if(pC->m_uE%2==1 || pC->m_uB%2==1)	
					nBoltsu = (long)(nBoltsu/2);
				else
					nBoltsu = (long)(nBoltsu/2)+1;
			}
			pRec->m_nQty	  = -nBoltsu;
			pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_BR_F_U,pBx,pC->m_ZzsBoltSize);
			pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_BR_F_U,pBx,pC->m_ZzsBoltSize);
			SetDeductRecord(pRec,pBx,CP_BR_F_U,nRow,nCol);
			//
			pRec = AddBMRecord(pRec);
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 하판");
//			pRec->SetStrProcessMid("B-FLG");
			nBoltsu = (long)(pC->m_dB+1)*(pC->m_dE+1)*2;
			if(nZigzag==0 && pC->m_dE>0) // 지그재그 적용
			{
				if(pC->m_dE%2==1 || pC->m_dB%2==1)
					nBoltsu = (long)(nBoltsu/2);
				else
					nBoltsu = (long)(nBoltsu/2)+1;
			}
			pRec->m_nQty	  = -nBoltsu;
			pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_BR_F_L,pBx,pC->m_ZzsBoltSize);
			pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_BR_F_L,pBx,pC->m_ZzsBoltSize);
			SetDeductRecord(pRec,pBx,CP_BR_F_L,nRow,nCol);
		}
	}
}

void CBMCreator::DeductBMCrossBeamBracketGasset(CPlateBasicIndex* pBx, BOOL bLeft)
{
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();
	CGangjeBase pBase(pDB);
	CPlateBridgeApp	*pBridge = m_pDataManage->GetBridge();

	// 보강재
	if(pDed->m_bDeductCrossBeamBracketStiffner)
	{
		CBMRecord * pRec = AddBMRecord();
		SetBMCrossBeamRecord(pRec,pBx);
		pRec->SetStrSubEleName("브라켓 보강재");
		//pRec->SetStrProcessMid("V보강");
		pRec->m_nQty = -1;
		pRec->m_dAreaBuje = pBase.GetAreaDeductCrossBeamBracketGasset(CP_BR_GA,pBx,0,bLeft?0:1);
		pRec->m_vJewon    = pBase.GetJewonDeductCrossBeamBracketGasset(CP_BR_GA,pBx,0,bLeft?0:1);
		SetDeductRecord(pRec,pBx,CP_BR_GA);
	}
	// 보강판
	long nCol = bLeft?0:1;
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음
	double dT = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_T);
	if(pDed->m_bDeductCrossBeamBracketStiffPlate && dT)
	{
		double dA = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_DA);
		double dC = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_DC);
		double uA = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_UA);
		double uC = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_UC);
		double W  = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_W);
		if(W/2>uC && uA>0)
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 보강판");
			//pRec->SetStrProcessMid("V-보강FLG");
			pRec->m_nQty = -2;
			pRec->m_dAreaBuje = uA*(W/2-uC)/2;
			pRec->m_vJewon.x  = W/2-uC;
			pRec->m_vJewon.z  = uA;
			pRec->m_vJewon.y  = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_T);
			SetDeductRecord(pRec,pBx,CP_BR_GA_FA);
		}
		if(W/2>dC && dA>0)
		{
			CBMRecord * pRec = AddBMRecord();
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("브라켓 보강판");
			//pRec->SetStrProcessMid("V-보강FLG");
			pRec->m_nQty = -2;
			pRec->m_dAreaBuje = dA*(W/2-dC)/2;
			pRec->m_vJewon.x  = W/2-dC;
			pRec->m_vJewon.z  = dA;
			pRec->m_vJewon.y  = pBridge->GetGussetPlateJewon((CPlateBasicIndex*)pBx, nID, CPlateBridgeApp::GUSSET_T);
			SetDeductRecord(pRec,pBx,CP_BR_GA_FA);
		}
	}
}

void CBMCreator::DeductBMCrossBeamSplice(CPlateBasicIndex* pBx, BOOL bLeft)
{
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();
	CGangjeBase pBase(pDB);

	//볼트홀 공제
	if(!pDed->m_bDeductBoltHole) return;

	if((pC->m_nLGirSpType && bLeft==TRUE)||(pC->m_nRGirSpType && bLeft==FALSE))
	{
		// 상부 상면 이음판
		long nZigzag = m_pDataManage->GetGlobalOption()->GetDesignBoltHoleJigJaegOfHorPlate();
		CBMRecord *pRec = AddBMRecord();
		SetBMCrossBeamRecord(pRec,pBx);
		pRec->SetStrSubEleName("상면 이음판");
		long nBoltsu = (long)(pC->m_uB+1)*(pC->m_uE+1)*2*2;
		if(nZigzag==0 && pC->m_uE>0) // 지그재그 적용
		{
			if(pC->m_uE%2==1 || pC->m_uB%2==1)	
				nBoltsu = (long)(nBoltsu/2);
			else
				nBoltsu = (long)(nBoltsu/2)+1;
		}
		pRec->m_nQty	  = -nBoltsu;
		pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_SP_U_U,pBx,pC->m_ZzsBoltSize);
		pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_SP_U_U,pBx,pC->m_ZzsBoltSize);
		SetDeductRecord(pRec,pBx,CP_SP_U_U);
		long i = 0;
		// 상부 하면 이음판
		for(i=0; i<2; i++)
		{
			pRec = AddBMRecord(pRec);
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("상면 이음판");
			nBoltsu = (long)(pC->m_uB+1)*(pC->m_uE+1)*2;
			if(nZigzag==0 && pC->m_dE>0) // 지그재그 적용
			{
				if(pC->m_uE%2==1 || pC->m_uB%2==1)	
					nBoltsu = (long)(nBoltsu/2);
				else
					nBoltsu = (long)(nBoltsu/2)+1;
			}
			pRec->m_nQty	  = -nBoltsu;
			pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_SP_U_L,pBx,pC->m_ZzsBoltSize);
			pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_SP_U_L,pBx,pC->m_ZzsBoltSize);
			SetDeductRecord(pRec,pBx,CP_SP_U_L);
		}

		// 하부 상면 이음판
		for(i=0; i<2; i++)
		{
			pRec = AddBMRecord(pRec);
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("하면 이음판");
			nBoltsu = (long)(pC->m_dB+1)*(pC->m_dE+1)*2;
			if(nZigzag==0 && pC->m_dE>0) // 지그재그 적용
			{
				if(pC->m_dE%2==1 || pC->m_dB%2==1)	
					nBoltsu = (long)(nBoltsu/2);
				else
					nBoltsu = (long)(nBoltsu/2)+1;
			}
			pRec->m_nQty	  = -nBoltsu;
			pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_SP_L_U,pBx,pC->m_ZzsBoltSize);
			pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_SP_L_U,pBx,pC->m_ZzsBoltSize);
			SetDeductRecord(pRec,pBx,CP_SP_L_U);
		}

		// 하부 하면 이음판
		for(i=0; i<2; i++)
		{
			pRec = AddBMRecord(pRec);
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("하면 이음판");
			nBoltsu = (long)(pC->m_dB+1)*(pC->m_dE+1)*2;
			if(nZigzag==0 && pC->m_dE>0) // 지그재그 적용
			{
				if(pC->m_dE%2==1 || pC->m_dB%2==1)	
					nBoltsu = (long)(nBoltsu/2);
				else
					nBoltsu = (long)(nBoltsu/2)+1;
			}
			pRec->m_nQty	  = -nBoltsu;
			// 이음판 1개
			if(pC->m_cGasset_CR==0 || pC->m_H_CR==pBx->GetHeightGirderByBx())
				pRec->m_nQty = -nBoltsu*2;

			pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_SP_L_L,pBx,pC->m_ZzsBoltSize);
			pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_SP_L_L,pBx,pC->m_ZzsBoltSize);
			SetDeductRecord(pRec,pBx,CP_SP_L_L);
			if(pC->m_cGasset_CR==0 || pC->m_H_CR==pBx->GetHeightGirderByBx())
				break;
		}
		if(pC->m_sB>0 && pC->m_sE>0)
		{
			// 전단 이음판
			pRec = AddBMRecord(pRec);
			SetBMCrossBeamRecord(pRec,pBx);
			pRec->SetStrSubEleName("전단 이음판");
			pRec->m_nQty = -(pC->m_sB+1)*(pC->m_sE+1)*2; // 이음판이 2개!
			pRec->m_vJewon    = pBase.GetJewonDeductBoltHole(CP_W,pBx,pC->m_ZzsBoltSize);
			pRec->m_dAreaBuje = pBase.GetAreaDeductBoltHole(CP_W,pBx,pC->m_ZzsBoltSize);
			SetDeductRecord(pRec,pBx,CP_SP_W_SH);
		}
	}
}
//#define		ASSEMBLY_CR					17
//#define		ASSEMBLY_CR_BRACKET_LEFT	18
//#define		ASSEMBLY_CR_BRACKET_RIGHT	19
//#define		ASSEMBLY_CR_SLICE			20
void CBMCreator::SetBMCrossBeamRecord(CBMRecord *pRec, CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	pRec->m_nGirder     = pBx->GetNumberGirder();
	pRec->m_nSplice     = pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_CROSS;

	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		if(pBx->IsJijum())
			pRec->SetStrEleName("지점부 가로보"); 
		else
			pRec->SetStrEleName("분배 가로보");
	}
	else if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
		pRec->SetStrEleName("수직 브레이싱");
	else
		pRec->SetStrSectionType("가로보");
}

void CBMCreator::SetBMSpliceRecord(CBMRecord *pRec, CPlateBasicIndex* pBx)
{
	CPlateGirderApp*	pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();;

	pRec->m_nGirder		= pBx->GetNumberGirder();
	pRec->m_nSplice		= pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nFirstClass = FIRST_GIRDER;
//	if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
	pRec->SetStrSectionType("현장이음");
	pRec->SetStrProcessBig("SPLICE");
	pRec->SetStrProcessMid("GIRDER");
}








///////////////////////////////////
// BMStandard

UINT CBMCreator::DoWork()
{
	UINT nErr = 0;

	// 초기화
	ClearAll();

	// m_BMStardardList 내용 생성
	switch( m_nWorkValue )
	{
		case 0  : nErr = AddAllCreator();		break;
		case 1  : nErr = MakeToLoadFileXL();	break;
		case 10 : nErr = AddAllCreator();		break;
	}

	if(nErr)	// 사용자 종료, 또는 Error시
	{
		OnProgress(4);
		ClearBase();
		return nErr;
	}

	// 플레이트 타입설정
	SetPlateTypeInStandardList();

	// 각 재료표별 정리, 생성
	OnProgress(0,90);

	// ASHOP_TYPE
	if(m_nMakeFlag & MAKE_BM_SANG)		// [01]기본도의 강재집계표
	{
		OnProgressText(1, "상세집계표");
		MakeListSang();
	}

	if(m_nMakeFlag & MAKE_BM_STDANDARD_SANG) // [02]표준집계표
	{
		OnProgressText(1, "표준집계표");
		MakeListStandardSang();
	}

	OnProgress(0,92);
	if(m_nMakeFlag & MAKE_BM_STDANDARD_SHOP) // [03]표준집계표(제작용)
	{
		OnProgressText(1, "표준집계표(제작용)");
		MakeListStandardShop();
	}
	if(m_nMakeFlag & MAKE_BM_STDANDARD_SHOP_TYPE2) // [03]표준집계표(제작용)
	{
		OnProgressText(1, "표준집계표(제작용) : TYPE2");
		MakeListStandardShopType2();
	}

	if(m_nMakeFlag & MAKE_BM_THICK)	// [04]강종별 두께별집계표
	{
		OnProgressText(1, "강종별 두께별 집계표");
		MakeListThick();
	}
	if(m_nMakeFlag & MAKE_BM_THICK_SANG)	// [04]강종별 두께별집계표
	{
		OnProgressText(1, "공정별 두께별 집계표");
		MakeListThickSang();
	}

	OnProgress(0,94);
	if(m_nMakeFlag & MAKE_BM_BUJEMARK_SANG)			// [05]부재마크별 상세집계표
	{
		OnProgressText(1, "부재마크별 상세집계표");
		MakeListBujeMarkSang();
	}
	if(m_nMakeFlag & MAKE_BM_BUJEMARK_TOTAL)		// [06]부재마크별 총괄집계표
	{
		OnProgressText(1, "부재마크별 총괄집계표");
		MakeListBujeMarkTotal();
	}
	OnProgress(0,95);
	if(m_nMakeFlag & MAKE_BM_BUJEMARK_DEDUCT)		// [07]부재마크별 공제집계표
	{
		OnProgressText(1, "부재마크별 공제집계표");
		MakeListBujeMarkDeduct();
	}

	if(m_nMakeFlag & MAKE_BM_CAR)					// [08]차량적재 상세집계표
	{
		OnProgressText(1, "차량적재 강재집계표");
		MakeListCar();
	}
	OnProgress(0,96);
	if(m_nMakeFlag & MAKE_BM_CARTOTAL)				// [09]차량적재 총괄집계표
	{
		OnProgressText(1, "차량적재 총괄집계표");
		MakeListCarTotal();
	}

	if(m_nMakeFlag & MAKE_BM_GRAND_ASSEMBLY_SANG)	// [10]대조립별 상세집계표
	{
		OnProgressText(1, "대조립마크별 상세집계표");
		MakeListGrandAssemblySang();
	}
	if(m_nMakeFlag & MAKE_BM_GRAND_ASSEMBLY_TOTAL)	// [11]대조립별 총괄집계표
	{
		OnProgressText(1, "대조립마크별 총괄집계표");
		MakeListGrandAssemblyTotal();
	}
	OnProgress(0,97);
	if(m_nMakeFlag & MAKE_BM_MID_ASSEMBLY_SANG)		// [12]중조립별 상세집계표
	{
		OnProgressText(1, "중조립마크별 상세집계표");
		MakeListMidAssemblySang();
	}
	if(m_nMakeFlag & MAKE_BM_MID_ASSEMBLY_TOTAL)	// [13]중조립별 총괄집계표
	{
		OnProgressText(1, "중조립마크별 총괄집계표");
		MakeListMidAssemblyTotal();
	}
	if(m_nMakeFlag & MAKE_BM_ASSEMBLY_SANG)			// [14]소조립별 상세집계표
	{
		OnProgressText(1, "소조립마크별 상세집계표");
		MakeListAssemblySang();
	}
	OnProgress(0,98);
	if(m_nMakeFlag & MAKE_BM_ASSEMBLY_TOTAL)		// [15]소조립별 총괄집계표
	{
		OnProgressText(1, "소조립마크별 총괄집계표");
		MakeListAssemblyTotal();
	}
	if(m_nMakeFlag & MAKE_BM_ASSEMBLY_DEDUCT)		// [16]소조립별 공제집계표
	{
		OnProgressText(1, "소조립마크별 공제집계표");
		MakeListAssemblyDeduct();
	}
	if(m_nMakeFlag & MAKE_BM_ASSEMBLY_FACT)			// [17]소조립별 공장제작용집계표
	{
		OnProgressText(1, "강제집계표 공장제작용");
		MakeListAssemblyFact();
	}
	if(m_nMakeFlag & MAKE_BM_ASSEMBLY_FIELD)		// [18]소조립별 현장설치용집계표
	{
		OnProgressText(1, "강제집계표 현장설치용");
		MakeListAssemblyField();
	}
	if(m_nMakeFlag & MAKE_BM_SECTION_COMP)			// [19]단면별 집계표(기본도정산용)
	{
		OnProgressText(1, "단면별 집계표(기본도정산용)");
		MakeListSectionComp();
	}
	if(m_nMakeFlag & MAKE_BM_BUJE_COMP)				// [20]부재별 집계표(기본도정산용)
	{
		OnProgressText(1, "부재별 집계표(기본도정산용)");
		MakeListBujeComp();
	}
	if(m_nMakeFlag & MAKE_BM_BASIC_COMP)			// [21]기본도 정산용 집계표
	{
		OnProgressText(1, "기본도정산용 집계표");
		MakeListBasicComp();
	}
	// AROAD_TYPE
	OnProgress(0,99);
	if(m_nMakeFlag & MAKE_BM_SECTIONTYPE)
	{
		OnProgressText(1, "단면타입별 집계표");
		MakeListSectionType();
	}
	if(m_nMakeFlag & MAKE_BM_BUJE)
	{
		OnProgressText(1, "부재별 집계표");
		MakeListBuje();
	}
	if(m_nMakeFlag & MAKE_BM_BUJE_TOTAL)
	{
		OnProgressText(1, "부재별 총괄 집계표");
		MakeListBujeTotal();
	}
	if(m_nMakeFlag & MAKE_BM_GIRDER_TOTAL)
	{
		OnProgressText(1, "거더별 총괄 집계표");
		MakeListGirder();
	}
	if(m_nMakeFlag & MAKE_BM_BUJEKIND_TOTAL)
	{
		OnProgressText(1, "부재종류별 총괄 집계표");
		MakeListBujeKindTotal();
	}
	
	if(m_nMakeFlag & MAKE_BM_DEDUCT_SANG)
	{
		OnProgressText(1, "상세별 공제 집계표");
		MakeListDeductSang();
	}
	if(m_nMakeFlag & MAKE_BM_DEDUCT_BUJE)
	{
		OnProgressText(1, "부재별 공제 집계표");
		MakeListDeductBuje();
	}

	OnProgress(0,100);
	OnProgress(4);
	ClearBase();
	return nErr;
}


#define ABORT(x) {if(m_bAbort == TRUE) return x;}

UINT CBMCreator::AddAllCreator()
{
	// support progress
	BOOL	bCalcMode = (m_nWorkValue==10) ? TRUE : FALSE;
	CString sFmt	  = _T("");
	long	TRange    = 0,nCount = 0;

	for(long n = 0; n < m_pDB->GetGirdersu(); n++)	
		TRange += m_pDB->GetGirder(n)->GetQtySplice() + 1;

	//////////////////////////////////////////////////////////////////////
	// OPTION PAGE	
	long			nGirdersu	= m_pDB->GetGirdersu();
	CFrameWnd		*pWnd		= (CFrameWnd*)AfxGetMainWnd();
	CProgressCtrl	progressCtrl;
	

	if(bCalcMode && !pWnd->GetTitle())	
		bCalcMode = FALSE;
	if(bCalcMode)
	{
		CRect rect = CRect(250, 3, 450, 18);
		progressCtrl.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
		progressCtrl.ShowWindow(SW_SHOW);
		TRange = 100;
		progressCtrl.SetRange32(0, TRange);
		progressCtrl.SetPos(0);
	}

	long nSpTotal = m_pDB->GetGirder(0)->GetQtySplice() * m_pDB->GetGirdersu();
 	for(long nG = 0; nG < nGirdersu; nG++)
	{
		CPlateGirderApp	 *pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder	 Finder(pGir);
		CPlateBasicIndex *pBx	  = Finder.GetBxFirst(BX_SPLICE | BX_STT_GIRDER | BX_END_GIRDER);
		CPlateBasicIndex *pBxNext = NULL;

		while(Finder.GetBxFixNext())
		{	
			pBxNext = Finder.GetBxFixNext();

			OnProgress(0,  (long)((double)++nCount / (double)TRange * 90.0));// support progress
			
			if(bCalcMode)	progressCtrl.SetPos((long)(((double)nCount / (double)nSpTotal) * 10.0));
			
			sFmt.Format("G%ld SP%ld 플랜지", nG + 1 + m_pDB->GetNumberStartGirder(), Finder.GetCurrentCount() + 1 + m_pDB->GetNumberStartSplice());
			
			if(bCalcMode)	m_pTStandard->SetPaneText(0,"강재생성중 : " + sFmt,TRUE);

			OnProgressText(1, sFmt);
			AddBMFactChain(pBx, pBxNext,G_F_U);			ABORT(1)	// 상부 플랜지
			AddBMFactChain(pBx, pBxNext,G_F_L);			ABORT(1)	// 하부 플랜지
			AddBMFactChain(pBx, pBxNext,G_W);			ABORT(1)	// 웨브 플랜지		
			DeductBMFactChain(pBx, pBxNext,G_F_U);		ABORT(1)	// 상부 플랜지 공제
			DeductBMFactChain(pBx, pBxNext,G_F_L);		ABORT(1)	// 하부 플랜지 공제
			DeductBMFactChain(pBx, pBxNext,G_W);		ABORT(1)	// 웨브 플랜지 공제						
			AddBMVStiff(pBx, pBxNext);					ABORT(1)	// 수직보강재
			AddBMCrossBeamGirderVStiff(pBx, pBxNext);	ABORT(1)	// 가로보 위치(BX_CROSSBEAM_VBRACING)에 있는 거더부착 수직보강재 (지점제외)
			AddBMVBracingGirderVStiff(pBx, pBxNext);	ABORT(1)	// 수직브레이싱 위치(BX_CROSSBEAM_VBRACING)에 있는 거더부착 수직보강재 (지점제외)
			AddBMJackUpStiff(pBx, pBxNext);				ABORT(1)	// 잭업보강재
			AddBMJiJumStiff(pBx, pBxNext);				ABORT(1)	// 지점보강재 (가로보, 수직브레이싱 위치 포함)
	
			sFmt.Format("G%ld SP%ld 수평보강재",nG + 1 + m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount() + 1 + m_pDB->GetNumberStartSplice());
			
			if(bCalcMode) m_pTStandard->SetPaneText(0,"강재생성중 : " + sFmt,TRUE);
			
			OnProgressText(1, sFmt);

			AddBMHStiff(pBx, pBxNext,TRUE);  ABORT(1)
			AddBMHStiff(pBx, pBxNext,FALSE); ABORT(1)
			
			sFmt.Format("G%ld SP%ld 현장이음",nG + 1 + m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount() + 1 + m_pDB->GetNumberStartSplice());

			if(bCalcMode)	m_pTStandard->SetPaneText(0,"강재생성중 : " + sFmt,TRUE);

			OnProgressText(1, sFmt);
			AddBMSplice(pBx, pBxNext,FALSE,TRUE); ABORT(1)

			if(nG != nGirdersu - 1)
			{
				sFmt.Format("G%ld SP%ld 가로보",nG + 1 + m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount() + 1 + m_pDB->GetNumberStartSplice());

				if(bCalcMode)	m_pTStandard->SetPaneText(0,"강재생성중 : " + sFmt,TRUE);

				AddBMVBracing(pBx, pBxNext);	ABORT(1)
				AddBMHBracing(pBx, pBxNext);	ABORT(1)				
				AddBMCrossBeam(pBx, pBxNext);	ABORT(1)
			}

			sFmt.Format("G%ld SP%ld 기타부재",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());

			if(bCalcMode)	m_pTStandard->SetPaneText(0,"강재생성중 : " + sFmt,TRUE);

			OnProgressText(1, sFmt);

			AddBMGitaBuje(pBx, pBxNext);	ABORT(1)
			AddBMFacePlate(pBx, pBxNext);	ABORT(1)
			AddBMWeldStud(pBx, pBxNext);	ABORT(1) // 앵커, 스터드, 각형

			pBx = Finder.GetBxNext();
		}
	}

	return FALSE;
}
#undef  ABORT
  

UINT CBMCreator::MakeToLoadFileXL()
{
	CXLControl	xl;
	if(xl.OpenXL(m_sPath) == FALSE)	return TRUE;

	CStringArray sArrReadXL;
	CString sLineData = _T("");
	CString sProcess  = _T("");
	long nRow	= 1;
	long nCol	= 0;
	long nColsu = 0;

	// 타이틀을 읽어서 COLUMN수 결정
	for(nCol = 0; nCol < MAX_BM_COLUMN; nCol++)
	{
		sLineData = xl.GetXL(0, nCol);
		if(!sLineData.IsEmpty()) nColsu++;
	}

	// DATA READING : 연속두줄이 공백이 아닐 때까지....
	while(1)
	{
		nCol = 0;
		CString strX1 = xl.GetXL(nRow,nCol);
		CString	strX2 = xl.GetXL(nRow+1,nCol);

		if(strX1.IsEmpty() && strX2.IsEmpty()) 
			break;

		sArrReadXL.RemoveAll();
		for(nCol = 0; nCol < nColsu; nCol++)
		{
			sLineData = xl.GetXL(nRow, nCol);
			sLineData.TrimLeft();
			sLineData.TrimRight();
			sArrReadXL.Add(sLineData);
		}
		//
		AddBMXLFileLine(sArrReadXL);
		sProcess.Format("Excel 파일 읽는중 [%ld열] : %s %s - %s , %s",nRow,sArrReadXL[0]
			             ,sArrReadXL[1],sArrReadXL[2],sArrReadXL[3]);
		OnProgressText(1, sProcess);
		if(m_bAbort == TRUE) 
		{
			xl.QuitXL();
			return TRUE;
		}
		nRow++;
	}

	xl.QuitXL();
	return FALSE;

}

void CBMCreator::AddBMXLFileLine(const CStringArray& sArr)
{
	if(sArr[0].IsEmpty()) return;

	CGlobarOption *pGOpt = m_pDataManage->GetGlobalOption();
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	// Ton일 경우 Ton으로 저장된것으로 판단
	double dFactorUnit = bUnitTon ? 1000.0: 1.0; 

	long nGirder = atoi(sArr[0].Right(sArr[0].GetLength()-1)) - m_pDB->GetNumberStartGirder();
	long nSplice = atoi(sArr[1].Right(sArr[1].GetLength()-1)) - m_pDB->GetNumberStartSplice();

	CBMRecord * pRec = AddBMRecord();
	CString	szEleName = sArr[2];
	CString szSubName = sArr[3];

	// 거더번호,현장이음번호,종류번호, 부재명
	pRec->m_nGirder	= nGirder - 1;
	pRec->m_nSplice	= nSplice - 1;
	pRec->SetStrEleName(sArr[2]);
	pRec->SetStrSubEleName(sArr[3]);
	// 기본단면,단면마크,소조립마크,부재마크
	long nCol = 4;
//	pRec->m_sMarkSection		= sArr[nCol++];
//	pRec->m_sMarkBuje			= sArr[nCol];  //너비
	// 두께,너비,길이,수량
	nCol = 6;
	pRec->m_vJewon.y	= atof(sArr[nCol++]);
	pRec->m_vJewon.x	= atof(sArr[nCol++]);
	pRec->m_vJewon.z	= atof(sArr[nCol++]);
	pRec->m_nQty		= atoi(sArr[nCol]);
	// 단위중량,총중량,재질, 대조립, 중조립, 소조립마크,기본단면, 부재종류,면적,참조도면,사용용도
	nCol = 10;
	pRec->m_dUWeight	= atof(sArr[nCol++]);
	pRec->m_dTWeight	= atof(sArr[nCol++])*dFactorUnit;
	pRec->m_sMaterial	= sArr[nCol++];
//	pRec->m_sMarkGrandAssem	= sArr[nCol++];
//	pRec->m_sMarkMidAssem	= sArr[nCol++];
//	pRec->m_sMarkAssem		= sArr[nCol++];
//	pRec->m_sMarkSectionBasic = sArr[nCol++];
	nCol = 17;
	pRec->m_sKindName   = sArr[nCol++];
	pRec->m_dAreaBuje	= frM2(atof(sArr[nCol++]));
	pRec->m_sRefDWG		= sArr[nCol++];
	pRec->m_sUsage		= sArr[nCol];
	// 면적계수,너비2,두께2
	nCol = 21;
	pRec->m_dFactorArea		= atof(sArr[nCol++]);
	pRec->m_dThickSecond	= atof(sArr[nCol++]);
	pRec->m_dWidthSecond	= atof(sArr[nCol++]);
	// 중량산식 (nCol=24)
	// 부재번호,소조립번호,중조립번호,대조립번호,기본단면번호,단면번호(long)
	nCol = 24;
	pRec->m_dTWeight			= atof(sArr[nCol++])*dFactorUnit;
	pRec->m_nMarkBuje			= atoi(sArr[nCol++]);
//	pRec->m_nMarkAssem			= atoi(sArr[nCol++]);
//	pRec->m_nMarkMidAssem		= atoi(sArr[nCol++]);
//	pRec->m_nMarkGrandAssem		= atoi(sArr[nCol++]);
//	pRec->m_nMarkSectionBasic	= atoi(sArr[nCol++]);
//	pRec->m_nMarkSection		= atoi(sArr[nCol]);
	// 1차분류,종류번호,STATION,m_dwFlag,m_dwDisableFlag,m_nKindName
	nCol = 31;
	pRec->m_nFirstClass			= atoi(sArr[nCol++]);
	pRec->m_nSectionNumber		= atoi(sArr[nCol++]);
	pRec->m_dStationRec			= atof(sArr[nCol++]);
	pRec->m_dwFlag				= (DWORD)atoi(sArr[nCol++]);
	pRec->m_dwDisableFlag		= (DWORD)atoi(sArr[nCol++]);
	pRec->m_nKindName			= atoi(sArr[nCol]);
}


void CBMCreator::SetWorkValue(long nWorkValue,const CString sPath,DIMTYPE nDimType,long nMakeFlag)
{
	m_nWorkValue = nWorkValue;	// 0:생성, 1:액셀파일로드
	m_sPath		 = sPath;
	m_nDimType   = nDimType;
	m_nMakeFlag  = nMakeFlag;
}
 
void CBMCreator::ShellSortBMRecord(CPtrList* pList, int (CBMRecord::*fcmp)(const CBMRecord*) const) const
{
	CPtrArray  Arr;
	POSITION pos = pList->GetHeadPosition();
	while( pos ) Arr.Add( pList->GetNext(pos) );

	long nSize = Arr.GetSize();

	long i, j, k, h;
	CBMRecord Rec;
	for(h = 1; h < nSize; h = 3*h+1);

	for(h /= 3; h > 0; h /=3)
		for(i = 0; i < h; i++)
			for(j = i+h; j < nSize; j += h)
			{
//				pRec = Arr[j];
				Rec = *(CBMRecord*)Arr[j];
				k = j;
				while(k > h - 1 && (((CBMRecord*)Arr[k-h])->*fcmp)(&Rec) > 0)
				{
//					Arr[k] = Arr[k-h];
					*(CBMRecord*)Arr[k] = *(CBMRecord*)Arr[k-h];
					k -= h;
				}
//				Arr[k] = pRec;
				*(CBMRecord*)Arr[k] = Rec;
			}

	Arr.RemoveAll();
}

// 강제집계표 제작용
// [01]기본도의 강재집계표 : 2002.06.10
void CBMCreator::MakeListSang()			
{
	if(m_nMakeFlag & (MAKE_BM_SANG == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionflag = 0x000404;
	// MARK_SECTION이어야 하나 ARoad에서도 SECTION MARK를 사용하므로 부득이하게 MARK_ASSEM으로 사용
/*	if(!IsMakeMark(CMarkSerial::MARK_BUJE))		 exceptionflag |= 0x024000;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM))	 exceptionflag |= 0x012000;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM))	 exceptionflag |= (0x001000|0x008000);
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_MID)) exceptionflag |= (0x100000|0x200000);
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_BIG)) exceptionflag |= (0x040000|0x080000);*/
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_Sang,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_Sang.AddTail( iList.GetNext(pos) );
}

// [02]표준집계표 : AROAD2000의 강재집계표
// 재료표 리스트의 순서
// 1. 다이아프램, 단면
// 2. 메인거더
// [03]유사하며 다이아프램,단면만 SET로 처리
void CBMCreator::MakeListStandardSang()
{
	if(m_nMakeFlag & (MAKE_BM_STDANDARD_SANG == 0)) 
		return;
	AhPtrListDelete(m_BMList_StandardSang,(CBMRecord*)0);

	// 강제별,SET별 중량을 산정....
	if(m_BMGangjeList.GetCount()==0)	
		MakeListGangje();

	// 강제별,SET별 중량을 저장....
	POSITION pos = m_BMGangjeList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord *pRec = (CBMRecord *)m_BMGangjeList.GetNext(pos);		
		m_BMList_StandardSang.AddTail( new CBMRecord(pRec) );
	}

	// MAIN BOX
 	CPtrList iList;
	for(long nG = 0; nG < m_pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER);

		while( Finder.GetBxFixNext())
		{
			GetBMListBoxGangje(&iList,pBx, Finder.GetBxFixNext(),TRUE,TRUE,FALSE);
			pBx = Finder.GetBxNext();
			pos = iList.GetHeadPosition();	
			while(pos) 
				m_BMList_StandardSang.AddTail( (CBMRecord *)iList.GetNext(pos) );
			if(iList.GetCount()) // 빈줄추가
				m_BMList_StandardSang.AddTail( new CBMRecord(NULL) );
			iList.RemoveAll();
		}
	}
	
}

// [03]표준집계표(제작용)
// 재료표 리스트의 순서
// 1. 다이아프램, 단면, 현장이음
// 2. 가로보브라켓, 세로보브라켓, 외측가로보브라켓
// 3. 가로보이음판, 세로보이음판, 외측가로보이음판 (?? => 순서가 여기인지는 추후결정)
// 4. 맨홀, 환기구, 인입구보강재, LIFTING LUG, 내부출입구, 결로집수구
// 5. 메인거더
// 6. 가로보, 세로보, 외측가로보, 외측빔
// m_BMStardardList를 사용하지않고 생성 (전체리스트에서 정렬을 하는 방식이 아님)
void CBMCreator::MakeListStandardShop()
{
	if(m_nMakeFlag & (MAKE_BM_STDANDARD_SHOP == 0)) 
		return;
	AhPtrListDelete(m_BMList_StandardShop,(CBMRecord*)0);

	// 강제별,SET별 중량을 산정....
	if(m_BMGangjeList.GetCount()==0)	
		MakeListGangje();

	// 강제별,SET별 중량을 저장....
	POSITION pos = m_BMGangjeList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord *pRec = (CBMRecord *)m_BMGangjeList.GetNext(pos);
		m_BMList_StandardShop.AddTail( new CBMRecord(pRec) );
	}

	// MAIN BOX
 	CPtrList iList;
	long	 nG = 0;

	for(nG = 0; nG < m_pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER);

		while( Finder.GetBxFixNext())
		{
			GetBMListAssemblyBox(&iList,pBx, Finder.GetBxFixNext());
			pBx = Finder.GetBxNext();
			pos = iList.GetHeadPosition();	
			while(pos) 
				m_BMList_StandardShop.AddTail( (CBMRecord *)iList.GetNext(pos) );
			if(iList.GetCount()) // 빈줄추가
				m_BMList_StandardShop.AddTail( new CBMRecord(NULL) );
			iList.RemoveAll();
		}
	}

	// 가로보
 	for(nG = 0; nG < m_pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBxStt = pGir->GetBxOnJijum(0);
		CPlateBasicIndex* pBxEnd = pGir->GetBxOnJijum(m_pDB->m_nQtyJigan);

		GetBMListAssemblyCrossBeam(&iList,pBxStt, pBxEnd,TRUE,TRUE);
		pos = iList.GetHeadPosition();	
		while(pos) 
			m_BMList_StandardShop.AddTail( (CBMRecord *)iList.GetNext(pos) );
		iList.RemoveAll();
	}

	// 세로보

	// 외측가로보

	// 외측빔
}

// 동운타입 상세집계표... (AROAD에서는 사용안함)
void CBMCreator::MakeListStandardShopType2()
{
	if(m_nMakeFlag & (MAKE_BM_STDANDARD_SHOP_TYPE2 == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionflag = 0x000404;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareStandardShopType2);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_StandardShopType2,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_StandardShopType2.AddTail( iList.GetNext(pos) );
}

// [04]강종별 두께별집계표
void CBMCreator::MakeListThick()
{
	if(m_nMakeFlag & (MAKE_BM_THICK == 0)) 
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		DWORD exceptionFlag;
		if(m_pDB->IsPlate(pRec->m_nMarkBuje))
			exceptionFlag = (0xFFFFFFFF & ~(0x80|0x800));	// thick, material만 비교
		else
			exceptionFlag = (0xFFFFFFFF & ~(0x80|0x800|0x000100));	// thick, material,길이
		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareThick);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_Thick,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_Thick.AddTail( iList.GetNext(pos) );
}

void CBMCreator::MakeListThickSang()
{
	if(m_nMakeFlag & (MAKE_BM_GRAND_ASSEMBLY_SANG == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x4000|0x20000));		// 부재마크
//										  0x1000|0x8000|		// 소조립마크
//										  0x100000|0x200000|	// 중조립마크
//		                                  0x40000|0x80000));	// 대조립마크
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareThickSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_ThickSang,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_ThickSang.AddTail( iList.GetNext(pos) );
}

// [05]부재마크별 상세집계표
void CBMCreator::MakeListBujeMarkSang()
{
	if(m_nMakeFlag & (MAKE_BM_BUJEMARK_SANG == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionflag = (0xFFFFFFFF & ~(0x4000|0x20000|0x40000|0x80000));	// 대조립,부재마크만 비교
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareBujeMarkSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_BujeMarkSang,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_BujeMarkSang.AddTail( iList.GetNext(pos) );
}

// [06]부재마크별 총괄집계표
void CBMCreator::MakeListBujeMarkTotal()
{
	return;
	if(m_nMakeFlag & (MAKE_BM_BUJEMARK_TOTAL == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x20000));	// 부재마크만 비교
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

//	ShellSortBMRecord(&iList, CBMRecord::CompareBujeMarkTotal);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_BujeMarkTotal,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_BujeMarkTotal.AddTail( iList.GetNext(pos) );
}

// [07]부재마크별 공제집계표
void CBMCreator::MakeListBujeMarkDeduct()
{
	if(m_nMakeFlag & (MAKE_BM_BUJEMARK_SANG == 0)) 
		return;
	CPtrList iList;

//	DWORD exceptionflag = 0x400;
	DWORD exceptionflag = (0xFFFFFFFF & ~(0x4000|0x20000|0x40000|0x80000));	// 대조립,부재마크만 비교
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if(pRec->m_nQty>0) continue;

		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareBujeMarkDeduct);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_BujeMarkDeduct,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_BujeMarkDeduct.AddTail( iList.GetNext(pos) );
}

// [08]차량적재 상세집계표
void CBMCreator::MakeListCar()
{
	if(m_nMakeFlag & (MAKE_BM_CAR == 0)) return;
	CPtrList iList1;
	CPtrList iList2; 

	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if(IsCarBuje(pRec))
		{
			DWORD exceptionflag = 0x400;
//			if(IsMakeMark(CMarkSerial::MARK_ASSEM))
//				exceptionflag |= (0x000008|0x000010|0x000020);
			if( SameAndIncludeRecord(&iList1,pRec,exceptionflag) == FALSE )
					iList1.AddTail( new CBMRecord(pRec) );
		}
		else
		{
			DWORD exceptionflag = (0xFFFFFFFF & ~(0x000008|0x000010|0x000020|
				                                  0x000040|0x000080|0x000100));
//			if(IsMakeMark(CMarkSerial::MARK_ASSEM))	
//				exceptionflag = 0x400|0x004000|0x040000|0x100000;
			if( SameAndIncludeRecord(&iList2,pRec,exceptionflag) == FALSE )
					iList2.AddTail( new CBMRecord(pRec) );
		}
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

/*	if(IsMakeMark(CMarkSerial::MARK_ASSEM))
	{
		ShellSortBMRecord(&iList1, CBMRecord::CompareCarAssembly);
		ShellSortBMRecord(&iList2, CBMRecord::CompareCarSection);
	}
	else*/
	{
		ShellSortBMRecord(&iList1, &CBMRecord::CompareCar);
		ShellSortBMRecord(&iList2, &CBMRecord::CompareSectionType);
	}
	if(m_bAbort == TRUE) return;	// 사용자 취소
	
	AhPtrListDelete(m_BMList_Car,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList1.GetHeadPosition();	
	while(pos) m_BMList_Car.AddTail( iList1.GetNext(pos) );
	pos = iList2.GetHeadPosition();	
	while(pos) m_BMList_Car.AddTail( iList2.GetNext(pos) );

}

// [09]차량적재 총괄집계표
void CBMCreator::MakeListCarTotal()
{
	if(m_nMakeFlag & (MAKE_BM_CARTOTAL == 0)) 
		return;

	CPtrList iList1,iList2;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if(IsCarBuje(pRec))
		{
			DWORD exceptionFlag = (0xFFFFFFFF & ~(0x001|0x002));	// Girder, Splice만 비교
			if( SameAndIncludeRecord(&iList1,pRec,exceptionFlag) == FALSE )
					iList1.AddTail( new CBMRecord(pRec) );
		}
		else
		{
			DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000008));	// ELENAME
			if( SameAndIncludeRecord(&iList2,pRec,exceptionFlag) == FALSE )
					iList2.AddTail( new CBMRecord(pRec) );
		}

		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList1, &CBMRecord::CompareCarTotal);
	ShellSortBMRecord(&iList2, &CBMRecord::CompareSectionType);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_CarTotal,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList1.GetHeadPosition();	
	while(pos) m_BMList_CarTotal.AddTail( iList1.GetNext(pos) );
	pos = iList2.GetHeadPosition();	
	while(pos) m_BMList_CarTotal.AddTail( iList2.GetNext(pos) );

}

// [10]대조립별 상세집계표
void CBMCreator::MakeListGrandAssemblySang()
{
	if(m_nMakeFlag & (MAKE_BM_GRAND_ASSEMBLY_SANG == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x4000|0x20000|		// 부재마크
										  0x1000|0x8000|		// 소조립마크
										  0x100000|0x200000|	// 중조립마크
		                                  0x40000|0x80000));	// 대조립마크
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareGrandAssemblySang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_GrandAssemblySang,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_GrandAssemblySang.AddTail( iList.GetNext(pos) );
}

// [11]대조립별 총괄집계표
void CBMCreator::MakeListGrandAssemblyTotal()
{
	return;
	if(m_nMakeFlag & (MAKE_BM_GRAND_ASSEMBLY_TOTAL == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x40000|0x80000));	// 대조립마크만 비교
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


//	ShellSortBMRecord(&iList, CBMRecord::CompareGrandAssemblyTotal);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_GrandAssemblyTotal,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_GrandAssemblyTotal.AddTail( iList.GetNext(pos) );
}

// [12]중조립별 상세집계표
void CBMCreator::MakeListMidAssemblySang()
{
	return;
	if(m_nMakeFlag & (MAKE_BM_MID_ASSEMBLY_SANG == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x4000|0x20000|		// 부재마크
										  0x1000|0x8000|		// 소조립마크
										  0x100000|0x200000|	// 중조립마크
		                                  0x40000|0x80000));	// 대조립마크
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


//	ShellSortBMRecord(&iList, CBMRecord::CompareMidAssemblySang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_MidAssemblySang,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_MidAssemblySang.AddTail( iList.GetNext(pos) );
}

// [13]중조립별 총괄집계표
void CBMCreator::MakeListMidAssemblyTotal()
{
	return;
	if(m_nMakeFlag & (MAKE_BM_MID_ASSEMBLY_TOTAL == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x100000|0x200000)); // 중조립마크
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


//	ShellSortBMRecord(&iList, CBMRecord::CompareMidAssemblyTotal);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_MidAssemblyTotal,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_MidAssemblyTotal.AddTail( iList.GetNext(pos) );
}

// [14]소조립별 상세집계표
// 공제물량은 제외
// 목적 : 박스별,주판별로 소조립마크를 세분화하여 BOX별로 점검
void CBMCreator::MakeListAssemblySang()
{
	return;
	if(m_nMakeFlag & (MAKE_BM_ASSEMBLY_SANG == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x4000|0x20000|		// 부재마크
										  0x1000|0x8000|		// 소조립마크
										  0x100000|0x200000|	// 중조립마크
		                                  0x40000|0x80000));	// 대조립마크
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


//	ShellSortBMRecord(&iList, CBMRecord::CompareAssemblySang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_AssemblySang,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord *pRec = (CBMRecord *)iList.GetNext(pos);
		m_BMList_AssemblySang.AddTail( pRec );
	}
}

// [15]소조립별 총괄집계표
void CBMCreator::MakeListAssemblyTotal()
{
	if(m_nMakeFlag & (MAKE_BM_ASSEMBLY_TOTAL == 0)) 
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x8000));	// 소조립마크만 비교
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareAssemblyTotal);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_AssemblyTotal,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord* pRec  = (CBMRecord*)iList.GetNext(pos);
		pRec->m_nQty = 0;
		m_BMList_AssemblyTotal.AddTail( pRec );
	}
}

// [16]소조립별 공제집계표
void CBMCreator::MakeListAssemblyDeduct()
{
	return;
	if(m_nMakeFlag & (MAKE_BM_ASSEMBLY_DEDUCT == 0)) 
		return;
	CPtrList iList;

	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x1000|0x8000|		// 소조립마크
										  0x100000|0x200000|	// 중조립마크
		                                  0x40000|0x80000));	// 대조립마크
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if(pRec->m_nQty>0) continue;

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


//	ShellSortBMRecord(&iList, CBMRecord::CompareAssemblyDeduct);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_AssemblyDeduct,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_AssemblyDeduct.AddTail( iList.GetNext(pos) );
}

// [17]소조립별 공장제작용집계표
void CBMCreator::MakeListAssemblyFact()
{
	return;
	if(m_nMakeFlag & (MAKE_BM_ASSEMBLY_FACT == 0)) 
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x1000|0x8000|		// 소조립마크
										  0x100000|0x200000|	// 중조립마크
		                                  0x40000|0x80000));	// 대조립마크
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

//		if(pRec->m_nMarkAssem == ASSEMBLY_SHOE) continue;

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareAssemblyFact);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_AssemblyFact,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_AssemblyFact.AddTail( iList.GetNext(pos) );
}

// [18]소조립별 현장설치용집계표
void CBMCreator::MakeListAssemblyField()
{
	if(m_nMakeFlag & (MAKE_BM_ASSEMBLY_FIELD == 0)) 
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x1000|0x8000|		// 소조립마크
										  0x100000|0x200000|	// 중조립마크
		                                  0x40000|0x80000));	// 대조립마크
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

//		if(pRec->m_nMarkAssem != ASSEMBLY_SHOE) continue;

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareAssemblyField);
	if(m_bAbort == TRUE) return;	// 사용자 취소


	AhPtrListDelete(m_BMList_AssemblyField,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_AssemblyField.AddTail( iList.GetNext(pos) );
}

// [19]단면별 집계표(기본도정산용)
void CBMCreator::MakeListSectionComp()
{
	if(m_nMakeFlag & (MAKE_BM_SECTION_COMP == 0)) return;
	AhPtrListDelete(m_BMList_SectionComp,(CBMRecord*)0);

	CPtrList iList;

	// 종류,부재명,제원,부재마크,대조립마크를 비교
	// 종류,부재명,제원를 비교 (2002.06.17)
//	DWORD exceptionflag = (0xFFFFFFFF & ~(0x08|0x10|0x20|0x40|0x80|0x100|0x4000|0x20000|0x40000|0x80000));
	DWORD exceptionflag = (0xFFFFFFFF & ~(0x08|0x10|0x20|0x40|0x80|0x100));
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareSectionComp);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_SectionComp,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_SectionComp.AddTail( iList.GetNext(pos) );
}

// [20]부재별 집계표(기본도정산용)
void CBMCreator::MakeListBujeComp()
{
	if(m_nMakeFlag & (MAKE_BM_BUJE_COMP == 0)) return;
	AhPtrListDelete(m_BMList_BujeComp,(CBMRecord*)0);

	CPtrList iList;

	// 부재명,제원,부재마크,대조립마크를 비교
	// 부재명,제원를 비교 (2002.06.17)
//	DWORD exceptionflag = (0xFFFFFFFF & ~(0x20|0x40|0x80|0x100|0x4000|0x20000|0x40000|0x80000));
	DWORD exceptionflag = (0xFFFFFFFF & ~(0x20|0x40|0x80|0x100));
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);

		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareBujeComp);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_BujeComp,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_BujeComp.AddTail( iList.GetNext(pos) );
}


// [21]기본도 정산용 집계표
void CBMCreator::MakeListBasicComp()
{
	if(m_nMakeFlag & (MAKE_BM_BASIC_COMP == 0)) return;
	AhPtrListDelete(m_BMList_BasicComp,(CBMRecord*)0);

	// MakeListStandardSang과 동일하며 출력만 다르게함........
}

// BxFlag는 하나씩만 돌린다.....
// 단면마크를 기준으로하고 가로보만 중조립마크로..........
void CBMCreator::GetBMListGangjeAllGirder(CPtrList* pList,  __int64 BxFlag, long nMarkMidAssem, long nMarkAssem, long nMarkSection)
{
	return;
/*
	DWORD exceptionFlag = 0x400;

	CString szMark = _T("");
	CStringArray sArrMark;
	for(long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CPlateBxFinder Finder(m_pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);
		while(pBx)
		{
//			if(nMarkSection==SECTION_CR && !pBx->IsCrossBeamRight())
			{
				pBx = Finder.GetBxNext();
				continue;
			}

//			if(nMarkSection==SECTION_CR)
//				szMark = m_pDB->GetMarkAssemblyMid(nMarkMidAssem,pBx);
//			else
//				szMark = m_pDB->GetMarkSection(nMarkSection,pBx);

			for(long i=0; i<sArrMark.GetSize(); i++)
				if(szMark==sArrMark.GetAt(i)) break;
			if(i==sArrMark.GetSize())
			{
				CPtrList iList;
				sArrMark.Add(szMark);
//				GetBMListByBxAndFlag(&iList,pBx,pBx,BxFlag,TRUE,TRUE,(nMarkSection==SECTION_CR));
				ShellSortBMRecord(&iList, CBMRecord::CompareAssemblyFact);

				CBMRecord *pRecTotal = new CBMRecord(NULL); // 합계를 첫머리에 추가....
				pRecTotal->m_dwSummaryFlag = BM_SUMMARY_SECTION;
				pList->AddTail(pRecTotal);

				POSITION pos = iList.GetHeadPosition();
				while(pos)
				{
					CBMRecord * pRec = (CBMRecord *)iList.GetNext(pos);
					if(pRec->m_nQty<1)	continue;
					if( SameAndIncludeRecord(pList,pRec,exceptionFlag) == FALSE )
					{
//						if(pRecTotal->m_sMarkGrandAssem.IsEmpty())
						{
							// [3]표준집계표(제작용)
							pRecTotal->m_sMarkGrandAssem = pRec->m_sMarkGrandAssem;
							pRecTotal->m_sMarkGrandAssem = pRec->m_sMarkGrandAssem;
							pRecTotal->m_sMarkMidAssem   = pRec->m_sMarkMidAssem;
							pRecTotal->m_sMarkAssem      = pRec->GetStrMarkAssem();
							pRecTotal->m_sMarkSection    = pRec->GetStrMarkSection();
							pRecTotal->m_dStationRec     = pRec->m_dStationRec;
							pRecTotal->m_nMarkGrandAssem = pRec->m_nMarkGrandAssem;
							pRecTotal->m_nMarkMidAssem   = pRec->m_nMarkMidAssem;
							pRecTotal->m_nMarkAssem      = pRec->m_nMarkAssem;
							pRecTotal->m_nMarkSection    = pRec->m_nMarkSection;
							// [2]표준집계표
							pRecTotal->m_nGirder = pRec->m_nGirder;
							pRecTotal->m_nSplice = pRec->m_nSplice;
							pRecTotal->SetStrEleName(pRec->GetStrEleName());
						}
						pRecTotal->m_dTWeight		+= pRec->m_dTWeight;
						pRecTotal->m_dTWeightBasic  += pRec->m_dTWeightBasic;
						pList->AddTail( new CBMRecord(pRec) );
					}
				}
				pList->AddTail( new CBMRecord(NULL) );	// 빈칸.............
				
				AhPtrListDelete(iList, (CBMRecord *)0);
			}
			pBx = Finder.GetBxNext();
		}
	}
*/
}

void CBMCreator::MakeListGangje()
{
	return;
	/*
	CPtrList iList;
	POSITION pos;	

	// 다이아프램
	GetBMListGangjeAllGirder(&iList,BX_CROSSBEAM_VBRACING,ASSEMBLYMID_DIAP,ASSEMBLY_DIAP,SECTION_MAIN);
	pos = iList.GetHeadPosition();	
	while(pos) m_BMGangjeList.AddTail( iList.GetNext(pos) );
	iList.RemoveAll();
	// 단면
	//GetBMListGangjeAllGirder(&iList,BX_SECTION,ASSEMBLYMID_SECTION,ASSEMBLY_TRIB,SECTION_MAIN);
	//pos = iList.GetHeadPosition();	
	//while(pos) m_BMGangjeList.AddTail( iList.GetNext(pos) );
	//iList.RemoveAll();
	// 현장이음
	GetBMListGangjeAllGirder(&iList,BX_SPLICE,ASSEMBLYMID_SP,ASSEMBLY_SP,SECTION_SP);
	pos = iList.GetHeadPosition();	
	while(pos) m_BMGangjeList.AddTail( iList.GetNext(pos) );
	iList.RemoveAll();

	// 맨홀
//	GetBMListGangjeAllGirder(&iList,BX_MANHOLE,ASSEMBLYMID_MH,ASSEMBLY_MH,SECTION_MH);
//	pos = iList.GetHeadPosition();	
//	while(pos) m_BMGangjeList.AddTail( iList.GetNext(pos) );
//	iList.RemoveAll();

	// 결로집수구
	// 환기구
	// 출입구
	// LIFTING LUG  (???) : 마크부재
	// 인입구보강재 (???) : 마크부재

	// 가로보 브라켓 (좌측)
	GetBMListGangjeAllGirder(&iList,BX_CROSSBEAM_VBRACING,ASSEMBLYMID_CR_BRACKET_LEFT,ASSEMBLY_CR_BRACKET_LEFT,SECTION_CR);
	pos = iList.GetHeadPosition();	
	while(pos) m_BMGangjeList.AddTail( iList.GetNext(pos) );
	iList.RemoveAll(); 
	// 가로보 브라켓 (우측)
	GetBMListGangjeAllGirder(&iList,BX_CROSSBEAM_VBRACING,ASSEMBLYMID_CR_BRACKET_RIGHT,ASSEMBLY_CR_BRACKET_RIGHT,SECTION_CR);
	pos = iList.GetHeadPosition();	
	while(pos) m_BMGangjeList.AddTail( iList.GetNext(pos) );
	iList.RemoveAll(); 
	// 가로보 이음판
	GetBMListGangjeAllGirder(&iList,BX_CROSSBEAM_VBRACING,ASSEMBLYMID_CR_SPLICE,ASSEMBLY_CR_SPLICE,SECTION_CR);
	pos = iList.GetHeadPosition();	
	while(pos) m_BMGangjeList.AddTail( iList.GetNext(pos) );
	iList.RemoveAll(); 

	// 세로보 브라켓 (좌측)
	// 세로보 브라켓 (우측)
	// 세로보 이음판
	*/
}


void CBMCreator::GetBMListAssemblySection(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
										   BOOL bIncludeSttBx, BOOL bIncludeEndBx, __int64 BxFlag)
{
	/*
	// 강제별,SET별 중량을 산정....
	if(m_BMGangjeList.GetCount()==0)	
		MakeListGangje();

	CPtrList iListTotal;
	POSITION pos = m_BMGangjeList.GetHeadPosition();
	while(pos)
	{
		CBMRecord *pRec = (CBMRecord *)m_BMGangjeList.GetNext(pos);
		if(pRec->m_dwSummaryFlag)
			iListTotal.AddTail(new CBMRecord(pRec)); 
	}

	CPlateGirderApp* pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd,bIncludeSttBx,bIncludeEndBx);

	CString csMark  = _T("");
	CStringArray sArrMark;
	long nSectEa   = 0;
	long nMarkSection = (BxFlag==BX_SPLICE) ? SECTION_SP : SECTION_MAIN;
	while(pBx)
	{
		csMark = m_pDB->GetMarkSection(nMarkSection,pBx);
		pos = iListTotal.GetHeadPosition();
		CBMRecord *pRec = NULL;
		while(pos)
		{
			pRec = (CBMRecord *)iListTotal.GetNext(pos);
			if(csMark==pRec->m_sMarkSection) break;
		}
		for(long j=0; j<sArrMark.GetSize(); j++)
			if(csMark==sArrMark.GetAt(j)) break;
		if(j==sArrMark.GetSize())
		{
			CPlateBxFinder FinderEa(pGir);
			CPlateBasicIndex *pBxEa = FinderEa.GetBxFirst(BxFlag,pBxStt,pBxEnd);
			while(pBxEa)
			{
				if(csMark==m_pDB->GetMarkSection(nMarkSection,pBxEa))
					nSectEa++;
				pBxEa = FinderEa.GetBxNext();
			}
			CBMRecord *pRecSect  = new CBMRecord(pRec);
			pRecSect->m_nQty	 = nSectEa;
			pRecSect->m_dTWeight	  = pRecSect->m_dTWeight*nSectEa;
			pRecSect->m_dTWeightBasic = pRecSect->m_dTWeightBasic*nSectEa;
			pList->AddTail(pRecSect);
			sArrMark.Add(csMark);
		}
		nSectEa = 0;
		pBx = Finder.GetBxNext();
	}
	AhPtrListDelete(iListTotal, (CBMRecord *)0);
	*/
}

// 가로보의 재료표
// bUnitWeightBracket : 브라켓을 단위중량으로 처리할 것인가 
// bUnitWeightSplice  : 이음판을 단위중량으로 처리할 것인가 (TRUE => 이음판 개수와 총중량만 출력)
void CBMCreator::GetBMListAssemblyCrossBeam(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
											 BOOL bUnitWeightBracket, BOOL bUnitWeightSplice)
{
	/*
	CPtrList iList;
	POSITION pos;

	CPlateGirderApp* pGir = pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);

	while(pBx)
	{
		if(pBx->IsCrossBeamRight())
		{
			CPtrList iListMain;
			GetBMListByBxAndFlag(&iListMain,pBx, pBx,BX_CROSSBEAM_VBRACING,TRUE,TRUE,TRUE);
			CBMRecord *pRecTotal = new CBMRecord(NULL); // 합계를 첫머리에 추가....
			pRecTotal->m_dwSummaryFlag = BM_SUMMARY_SECTION;
			iList.AddTail(pRecTotal);
			pos = iListMain.GetHeadPosition();
			while(pos)
			{
				CBMRecord *pRec = (CBMRecord *)iListMain.GetNext(pos);
				if(pRec->m_nMarkAssem==ASSEMBLY_CR)
					iList.AddTail( new  CBMRecord(pRec) );
				if(!bUnitWeightBracket && (pRec->m_nMarkAssem==ASSEMBLY_CR_BRACKET_LEFT || pRec->m_nMarkAssem==ASSEMBLY_CR_BRACKET_RIGHT))
					iList.AddTail( new  CBMRecord(pRec) );
				if(!bUnitWeightSplice && pRec->m_nMarkAssem==ASSEMBLY_CR_SPLICE)
					iList.AddTail( new  CBMRecord(pRec) );
			}
			AhPtrListDelete(iListMain, (CBMRecord *)0);

			if(bUnitWeightBracket)
			{
				GetBMListAssemblyCrossBeamGasset(&iList,pBx,pBx,TRUE);
				GetBMListAssemblyCrossBeamGasset(&iList,pBx,pBx,FALSE);
			}
			if(bUnitWeightSplice)
				GetBMListAssemblyCrossBeamSplice(&iList,pBx,pBx);

			pos = iList.GetHeadPosition();	
			double dTotalWeight = 0;
			while(pos) 
			{
				CBMRecord *pRec = (CBMRecord *)iList.GetNext(pos);
				dTotalWeight += pRec->m_dTWeight;
				pList->AddTail(pRec);
				if(pRecTotal->m_sMarkGrandAssem.IsEmpty())
				{
					pRecTotal->m_sMarkGrandAssem = pRec->m_sMarkGrandAssem;
					pRecTotal->m_sMarkMidAssem   = pRec->m_sMarkMidAssem;
					pRecTotal->m_sMarkAssem      = pRec->GetStrMarkAssem();
					pRecTotal->m_sMarkSection    = pRec->GetStrMarkSection();
					pRecTotal->m_dStationRec     = pRec->m_dStationRec;
					pRecTotal->m_nMarkAssem      = pRec->m_nMarkAssem;
					pRecTotal->m_nMarkSection    = pRec->m_nMarkSection;
					pRecTotal->m_nMarkGrandAssem = pRec->m_nMarkGrandAssem;
				}
			}
			pRecTotal->m_dTWeight = dTotalWeight;
			pList->AddTail( new CBMRecord(NULL) ); // 빈칸.....
			iList.RemoveAll();
		}
		pBx = Finder.GetBxNext();
	}
	*/
}

// bLeft : 가로보를 기준으로 좌측
void CBMCreator::GetBMListAssemblyCrossBeamGasset(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft)
{	
	/*
	// 강제별,SET별 중량을 산정....
	if(m_BMGangjeList.GetCount()==0)	
		MakeListGangje();

	CPtrList iListTotal;
	POSITION pos = m_BMGangjeList.GetHeadPosition();
	while(pos)
	{
		CBMRecord *pRec = (CBMRecord *)m_BMGangjeList.GetNext(pos);
		if(pRec->m_dwSummaryFlag)
			iListTotal.AddTail(new CBMRecord(pRec)); 
	}

	CPlateGirderApp* pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);

	long nMarkAssemMid = bLeft ? ASSEMBLYMID_CR_BRACKET_LEFT : ASSEMBLYMID_CR_BRACKET_RIGHT;
	CString csMark  = _T("");
	CStringArray sArrMark;
	long nSectEa   = 0;
	BOOL bLastGirder = (pBx->GetNumberGirder()==m_pDB->GetGirdersu()-1) ? TRUE : FALSE;
	while(pBx)
	{
		if(pBx->IsCrossBeamRight())
		{
			csMark = bLastGirder ? m_pDB->GetMarkAssemblyMid(nMarkAssemMid,pBx->GetBxMatchByCrossBeam(pGir->m_pLeft))
				                 : m_pDB->GetMarkAssemblyMid(nMarkAssemMid,pBx);
			pos = iListTotal.GetHeadPosition();
			CBMRecord *pRec = NULL;
			while(pos)
			{
				pRec = (CBMRecord *)iListTotal.GetNext(pos);
				if(csMark==pRec->m_sMarkMidAssem && nMarkAssemMid==pRec->m_nMarkMidAssem) break;
			}
			for(long j=0; j<sArrMark.GetSize(); j++)
				if(csMark==sArrMark.GetAt(j)) break;
			if(j==sArrMark.GetSize())
			{
				CPlateBxFinder FinderEa(pGir);
				CPlateBasicIndex *pBxEa = FinderEa.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
				while(pBxEa)
				{
					if(pBxEa->IsCrossBeamRight())
					{
						CString csComp = _T("");
						if(bLastGirder)
							csComp = m_pDB->GetMarkAssemblyMid(nMarkAssemMid,pBxEa->GetBxMatchByCrossBeam(pGir->m_pLeft));
						else
							csComp = m_pDB->GetMarkAssemblyMid(nMarkAssemMid,pBxEa);
						if(csMark==csComp)
							nSectEa++;
					}
					pBxEa = FinderEa.GetBxNext();
				}
				CBMRecord *pRecSect  = new CBMRecord(pRec);
				pRecSect->m_nQty		 = nSectEa;
				pRecSect->m_dTWeight	 = pRecSect->m_dTWeight*nSectEa;
				pRecSect->m_nMarkSection = SECTION_CR;
				pList->AddTail(pRecSect);
				sArrMark.Add(csMark);
			}
			nSectEa = 0;
		}
		pBx = Finder.GetBxNext();
	}
	AhPtrListDelete(iListTotal, (CBMRecord *)0);
	*/
}

void CBMCreator::GetBMListAssemblyCrossBeamSplice(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	/*
	// 강제별,SET별 중량을 산정....
	if(m_BMGangjeList.GetCount()==0)	
		MakeListGangje();

	CPtrList iListTotal;
	POSITION pos = m_BMGangjeList.GetHeadPosition();
	while(pos)
	{
		CBMRecord *pRec = (CBMRecord *)m_BMGangjeList.GetNext(pos);
		if(pRec->m_dwSummaryFlag)
			iListTotal.AddTail(new CBMRecord(pRec)); 
	}

	CPlateGirderApp* pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);

	long nMarkAssem = ASSEMBLY_CR_SPLICE;
	CString csMark  = _T("");
	CStringArray sArrMark;
	long nSectEa   = 0;
	while(pBx)
	{
		if(pBx->IsCrossBeamRight())
		{
			csMark = m_pDB->GetMarkAssembly(nMarkAssem,pBx);
			pos = iListTotal.GetHeadPosition();
			CBMRecord *pRec = NULL;
			while(pos)
			{
				pRec = (CBMRecord *)iListTotal.GetNext(pos);
				if(csMark==pRec->m_sMarkAssem) break;
			}
			for(long j=0; j<sArrMark.GetSize(); j++)
				if(csMark==sArrMark.GetAt(j)) break;
			if(j==sArrMark.GetSize())
			{
				CPlateBxFinder FinderEa(pGir);
				CPlateBasicIndex *pBxEa = FinderEa.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
				while(pBxEa)
				{
					if(pBxEa->IsCrossBeamRight())
					{
						CString csComp = m_pDB->GetMarkAssembly(nMarkAssem,pBxEa);
						if(csMark==csComp)
							nSectEa++;
					}
					pBxEa = FinderEa.GetBxNext();
				}
				CBMRecord *pRecSect  = new CBMRecord(pRec);
				pRecSect->m_nQty		 = nSectEa*2; // 좌,우
				pRecSect->m_dTWeight	 = pRecSect->m_dTWeight*pRecSect->m_nQty;
				pRecSect->m_nMarkSection = SECTION_CR;
				pList->AddTail(pRecSect);
				sArrMark.Add(csMark);
			}
			nSectEa = 0;
		}
		pBx = Finder.GetBxNext();
	}
	AhPtrListDelete(iListTotal, (CBMRecord *)0);
	*/
}

void CBMCreator::GetBMListAssemblySerobo(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{

}

void CBMCreator::GetBMListAssemblySeroboGasset(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bStt)
{

}

void CBMCreator::GetBMListAssemblySeroboSplice(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bStt)
{

}


// AShop 강상형 상세도의 재료료 1단위
void CBMCreator::GetBMListAssemblyBox(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									   BOOL bIncludeSttBx, BOOL bIncludeEndBx, BOOL bSpaceTerm)
{	
	/*
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	CPtrList iList;

	CBMRecord *pRecTotal = new CBMRecord(NULL);
	pRecTotal->m_sMarkGrandAssem = m_pDB->GetMarkAssemblyBig(ASSEMBLYBIG_BOX,pBxStt);
	pRecTotal->m_dwSummaryFlag   = BM_SUMMARY_ASSEM_BIG;
	iList.AddTail(pRecTotal);

	// 상.하판
	GetBMListByBxAndFlag(&iList,pBxStt, pBxEnd,BX_UPPERFLANGE|BX_JRIB_UPPER);
	if(bSpaceTerm) iList.AddTail(new CBMRecord(NULL));
	GetBMListByBxAndFlag(&iList,pBxStt, pBxEnd,BX_LOWERFLANGE|BX_JRIB_LOWER);
	if(bSpaceTerm) iList.AddTail(new CBMRecord(NULL));

	// 좌측판
	GetBMListByBxAndFlag(&iList,pBxStt->GetBxLeft(), pBxEnd->GetBxLeft(),(BX_WEB   | BX_HSTIFF_LEFT)  & ~BX_SPLICE);
	if(pGir->m_pLeft)
	{
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx1 = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt->GetBxLeft(),pBxEnd->GetBxLeft())->GetBxMatchByCrossBeam(pGir->m_pLeft);
		CPlateBasicIndex *pBx2 = Finder.GetBxLast(BX_CROSSBEAM_VBRACING,pBxEnd->GetBxLeft(),pBxStt->GetBxLeft())->GetBxMatchByCrossBeam(pGir->m_pLeft);
		GetBMListAssemblyCrossBeamGasset(&iList,pBx1, pBx2,FALSE);
	}
	if(bSpaceTerm) iList.AddTail(new CBMRecord(NULL));

	// 우측판
	//GetBMListByBxAndFlag(&iList,pBxStt->GetBxRight(), pBxEnd->GetBxRight(),(BX_WEB | BX_HSTIFF_RIGHT) & ~BX_SPLICE);
	if(pGir->m_pRight)
		GetBMListAssemblyCrossBeamGasset(&iList,pBxStt->GetBxRight(), pBxEnd->GetBxRight(),TRUE);
	if(bSpaceTerm) iList.AddTail(new CBMRecord(NULL));

	GetBMListAssemblySection(&iList,pBxStt, pBxEnd,TRUE,TRUE,BX_CROSSBEAM_VBRACING);
	GetBMListAssemblySection(&iList,pBxStt, pBxEnd,TRUE,TRUE,BX_SECTION);
	GetBMListAssemblySection(&iList,pBxStt, pBxEnd,FALSE,TRUE,BX_SPLICE);
	//
	double dTotalWeight = 0;
	POSITION pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord *pRec = (CBMRecord*)iList.GetNext(pos);
		dTotalWeight += pRec->m_dTWeight;
		pList->AddTail(pRec);
	}
	pRecTotal->m_dTWeight = dTotalWeight;
	*/
}

// [2]표준집계표의 박스단위
void CBMCreator::GetBMListBoxGangje(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeSttBx, BOOL bIncludeEndBx, BOOL bSpaceTerm)
{
	/*
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	CPtrList iList;

	CBMRecord *pRecTotal = new CBMRecord(NULL);
	pRecTotal->m_sMarkGrandAssem = m_pDB->GetMarkAssemblyBig(ASSEMBLYBIG_BOX,pBxStt);
	pRecTotal->m_dwSummaryFlag   = BM_SUMMARY_ASSEM_BIG;
	iList.AddTail(pRecTotal);

	// 상.하판, 복부판
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_UPPERFLANGE|BX_JRIB_UPPER);
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_LOWERFLANGE|BX_JRIB_LOWER);
	GetBMListByBxAndFlagSang(&iList,pBxStt->GetBxLeft(), pBxEnd->GetBxLeft(),(BX_WEB   | BX_HSTIFF_LEFT)  & ~BX_SPLICE);
	//GetBMListByBxAndFlagSang(&iList,pBxStt->GetBxRight(), pBxEnd->GetBxRight(),(BX_WEB | BX_HSTIFF_RIGHT) & ~BX_SPLICE);
	// 현장이음
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_SPLICE,FALSE,TRUE);
	// 가로보,세로보
	//GetBMListStringer(&iList,pBxStt, pBxEnd);
	//GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_CROSSBEAM_VBRACING,TRUE,TRUE,TRUE);
	// 외측가로보
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_OUTSIDE_ENDBEAM_LEFT);
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_OUTSIDE_ENDBEAM_RIGHT);
	// 외측빔
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_OUTSIDE_CROSS_LEFT);
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_OUTSIDE_CROSS_RIGHT);
	// 맨홀,출입구,환기구
	//GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_MANHOLE);
	//GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_VENT);
	//GetBMListAccessDoor(&iList,pBxStt, pBxEnd);
	// 결로수집수구,잭업
	//GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_WATEROUT);
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_JACKUP);
	GetBMListByBxAndFlagSang(&iList,pBxStt, pBxEnd,BX_JACKUP);
	// 다이아프램,단면
	//GetBMListAssemblySection(&iList,pBxStt, pBxEnd,TRUE,TRUE,BX_CROSSBEAM_VBRACING);
	//GetBMListAssemblySection(&iList,pBxStt, pBxEnd,TRUE,TRUE,BX_SECTION);
	//
	double dTotalWeight = 0;
	double dTotalWeightBasic = 0;
	POSITION pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord *pRec = (CBMRecord*)iList.GetNext(pos);
		if(pRecTotal->m_nGirder<0)
		{
			pRecTotal->m_nGirder = pRec->m_nGirder;
			pRecTotal->m_nSplice = pRec->m_nSplice;
		}
		dTotalWeight	  += pRec->m_dTWeight;
		dTotalWeightBasic += pRec->m_dTWeightBasic;
		pList->AddTail(pRec);
	}
	pRecTotal->m_dTWeight	   = dTotalWeight;
	pRecTotal->m_dTWeightBasic = dTotalWeightBasic;
	*/
}

// 강제집계표 설계용
// 단면타입별 집계표
void CBMCreator::MakeListSectionType()
{	
	if(m_nMakeFlag & (MAKE_BM_SECTIONTYPE == 0)) return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		DWORD exceptionFlag = 0x10000|0x8000|0x4000|0x403;	// Girder, Splice 예외, 총중향
		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareSectionType);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_SectionType,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_SectionType.AddTail( iList.GetNext(pos) );
}

// 부재별 집계표
void CBMCreator::MakeListBuje()	
{
	if(m_nMakeFlag & (MAKE_BM_BUJE == 0)) 
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000020|0x000040|0x000080|0x000100|0x000800));	
		// 부재명,제원,재질만 비교

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareBuje);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_Buje,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_Buje.AddTail( iList.GetNext(pos) );
}
	
// 부재별 총괄 집계표
void CBMCreator::MakeListBujeTotal()	
{
	if(m_nMakeFlag & (MAKE_BM_BUJE_TOTAL == 0)) 
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000020|0x000800));	// 부재, 강종만 비교 m_nSubEleName

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareBuje);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_BujeTotal,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_BujeTotal.AddTail( iList.GetNext(pos) );
}
	
// 거더별 총괄 집계표
void CBMCreator::MakeListGirder()
{
	if(m_nMakeFlag & (MAKE_BM_GIRDER_TOTAL == 0))
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000001));

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_Girder,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_Girder.AddTail( iList.GetNext(pos) );
}

// 부재종류별 총괄 집계표
void CBMCreator::MakeListBujeKindTotal()	
{
	if(m_nMakeFlag & (MAKE_BM_BUJEKIND_TOTAL == 0))
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000020|0x000800|0x008000));	// 부재,재질,종류 비교 

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareSizeType);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	
	AhPtrListDelete(m_BMList_BujeKindTotal,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_BujeKindTotal.AddTail( iList.GetNext(pos) );
}


// 상세별공제
void CBMCreator::MakeListDeductSang()
{
	if(m_nMakeFlag & (MAKE_BM_DEDUCT_SANG == 0))
		return;
	CPtrList iList;

	DWORD exceptionflag = 0x400;
/*	if(!IsMakeMark(CMarkSerial::MARK_BUJE))		 exceptionflag |= 0x024000;
	if(!IsMakeMark(CMarkSerial::MARK_SECTION))	 exceptionflag |= 0x002000;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM))	 exceptionflag |= 0x001000;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_MID)) exceptionflag |= 0x100000;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_BIG)) exceptionflag |= 0x040000;*/
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		if(pRec->m_nQty>0) continue;

		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}


	ShellSortBMRecord(&iList,&CBMRecord::CompareSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	AhPtrListDelete(m_BMList_DeductSang,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_DeductSang.AddTail( iList.GetNext(pos) );
}

// 부재별공제
void CBMCreator::MakeListDeductBuje()		
{
	if(m_nMakeFlag & (MAKE_BM_DEDUCT_BUJE == 0)) 
		return;

	CPtrList iList;
	POSITION pos = m_BMStardardList.GetHeadPosition();
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMStardardList.GetNext(pos);
		if(pRec->m_nQty>0)	continue;

		DWORD exceptionFlag = 0x3cc1f;//0x1F;	
		/* nSubElement, W, T, L, dUWeight 만 비교 */

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
		if(m_bAbort == TRUE) return;	// 사용자 취소
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareBuje);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	AhPtrListDelete(m_BMList_DeductBuje,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) m_BMList_DeductBuje.AddTail( iList.GetNext(pos) );
}

void CBMCreator::SetPlateTypeInStandardList()
{
	POSITION pos = m_BMStardardList.GetHeadPosition();

	CGangjeBase pGangBase(m_pDB);

	m_TotalWeight = 0;

	while( pos )
	{
		CBMRecord* pRec = (CBMRecord*)m_BMStardardList.GetNext(pos);
		pRec->m_nKindName = pGangBase.GetMaterialKindNumber(pRec->m_nMarkBuje);
/*		if(pRec->m_sMaterial.Find("SM")!=-1) 
			pRec->m_nKindName = MATERIAL_KIND_PLATE;
		if(pRec->m_sMaterial.Find("SD")!=-1) 
			pRec->m_nKindName = MATERIAL_KIND_REBAR;
*/		m_TotalWeight += pRec->m_dTWeight;
	}

}

void CBMCreator::GetBMListByBxAndFlag(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									   __int64 BxFlag, BOOL bIncludeSttBx, BOOL bIncludeEndBx, 
									   BOOL bCrossBeam)
{
	AhPtrListDelete(m_BMUnitList,(CBMRecord*)0);
	CGangjeBase pGangBase(m_pDB);
	m_bUnitMode = TRUE;

	MakeUnitList(pBxStt, pBxEnd, BxFlag,bIncludeSttBx,bIncludeEndBx,bCrossBeam);

	CPtrList iList;
	POSITION pos = m_BMUnitList.GetHeadPosition();
//	DWORD exceptionFlag = 0x400|0x001000|0x008000|0x040000|0x080000|0x100000|0x200000;
	DWORD exceptionFlag = 0x400;
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMUnitList.GetNext(pos);
		if(pRec->m_nQty<1)	continue;
		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
	}


	ShellSortBMRecord(&iList, &CBMRecord::CompareAssemblyFact);
	
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord* pRec  = (CBMRecord*)iList.GetNext(pos);
		pList->AddTail(pRec);
	}
	m_bUnitMode = FALSE;
}

void CBMCreator::GetBMListByBxAndFlagSang(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									   __int64 BxFlag, BOOL bIncludeSttBx, BOOL bIncludeEndBx, BOOL bCrossBeam)
{
	AhPtrListDelete(m_BMUnitList,(CBMRecord*)0);
	m_bUnitMode = TRUE;

	MakeUnitList(pBxStt, pBxEnd, BxFlag,bIncludeSttBx,bIncludeEndBx,bCrossBeam);

	CPtrList iList;
	POSITION pos = m_BMUnitList.GetHeadPosition();

	DWORD exceptionflag = 0x000400;
	// MARK_SECTION이어야 하나 ARoad에서도 SECTION MARK를 사용하므로 부득이하게 MARK_ASSEM으로 사용
/*	if(!IsMakeMark(CMarkSerial::MARK_BUJE))		 exceptionflag |= 0x024000;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM))	 exceptionflag |= 0x012000;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM))	 exceptionflag |= (0x001000|0x008000);
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_MID)) exceptionflag |= (0x100000|0x200000);
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_BIG)) exceptionflag |= (0x040000|0x080000);*/
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMUnitList.GetNext(pos);
		if(pRec->m_nQty<1)	continue;
		if( SameAndIncludeRecord(&iList,pRec,exceptionflag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareSang);
	
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord* pRec  = (CBMRecord*)iList.GetNext(pos);
		pList->AddTail( pRec );
	}
	m_bUnitMode = FALSE;
}
void CBMCreator::GetBMListVBracing(CPtrList* pList, CPlateBasicIndex *pBx)
{
	if(!pBx->IsCrossBeamRight()) return;
	AhPtrListDelete(m_BMUnitList,(CBMRecord*)0);
	m_bUnitMode = TRUE;

	SetDimType(DIM_BASIC);
	AddBMVBracing(pBx, pBx);

	CPtrList iList;
	POSITION pos = m_BMUnitList.GetHeadPosition();
	DWORD exceptionFlag = 0x400|0x1000|0x2000|0x4000|0x40000|0x100000; // 마크는 모두무시
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMUnitList.GetNext(pos);
		if(pRec->m_nQty<1)	continue;

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareSang);
	
	pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord* pRec  = (CBMRecord*)iList.GetNext(pos);
		pList->AddTail( pRec );
	}
	m_bUnitMode = FALSE;
}

// AROAD에서만 사용........DIM_BASIC
void CBMCreator::GetBMListCrossBeam(CPtrList* pList, CPlateBasicIndex *pBx)
{
	if(!pBx->IsCrossBeamRight()) return;
	AhPtrListDelete(m_BMUnitList,(CBMRecord*)0);
	m_bUnitMode = TRUE;

	SetDimType(DIM_BASIC);
	AddBMCrossBeam(pBx, pBx);

	CPtrList iList;
	POSITION pos = m_BMUnitList.GetHeadPosition();
	DWORD exceptionFlag = 0x400|0x1000|0x2000|0x4000|0x40000|0x100000; // 마크는 모두무시
	while(pos)
	{
		CBMRecord * pRec = (CBMRecord *)m_BMUnitList.GetNext(pos);
		if(pRec->m_nQty<1)	continue;

		if( SameAndIncludeRecord(&iList,pRec,exceptionFlag) == FALSE )
			iList.AddTail( new CBMRecord(pRec) );
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareSang);
	
	pos = iList.GetHeadPosition();	
	while(pos) 
	{
		CBMRecord* pRec  = (CBMRecord*)iList.GetNext(pos);
		pList->AddTail( pRec );
	}
	m_bUnitMode = FALSE;
}

void CBMCreator::MakeUnitList(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 BxFlag, 
							   BOOL bIncludeSttBx, BOOL bIncludeEndBx, BOOL bCrossBeam)
{	
	long nG = pBxStt->GetNumberGirder();
	CPlateGirderApp *pGir = pBxStt->GetGirder();

	if(BxFlag & BX_UPPERFLANGE)
		AddBMFactChain(pBxStt, pBxEnd,G_F_U);
	if(BxFlag & BX_LOWERFLANGE)
		AddBMFactChain(pBxStt, pBxEnd,G_F_L);
	if(BxFlag & BX_WEB)
		AddBMFactChain(pBxStt, pBxEnd,G_W);
	if(BxFlag & BX_JACKUP || BxFlag & BX_JACKUP)
		AddBMJackUpStiff(pBxStt, pBxStt, BxFlag);
	if(BxFlag!=BX_JACKUP)
	{
		if((BxFlag & ~BX_SPLICE) & BX_PLATEHSTIFF)
			AddBMHStiff(pBxStt, pBxEnd,TRUE); 
	}
	if(BxFlag!=BX_JACKUP)
	{
		if((BxFlag & ~BX_SPLICE) & BX_PLATEHSTIFF)
			AddBMHStiff(pBxStt, pBxEnd,FALSE);
	}
	if(BxFlag & BX_SPLICE)
		AddBMSplice(pBxStt, pBxEnd,bIncludeSttBx,bIncludeEndBx);

	if(BxFlag & BX_CROSSBEAM_VBRACING && bCrossBeam)
	{
		AddBMCrossBeam(pBxStt, pBxEnd);
		AddBMCrossBeamGirderVStiff(pBxStt, pBxEnd);	// 가로보위치에 있는 거더부착 수직보강재
	}
}


void CBMCreator::MakeListCrossBeamJewon()			
{
	CPtrList iList;

	DWORD exceptionflag = (0xFFFFFFFF & ~(0x000040|0x000080|0x000100|0x000020));
	POSITION pos = m_BMCrossBeamList.GetHeadPosition();
	while(pos)
	{
		CBMRecord *pRec = (CBMRecord *)m_BMCrossBeamList.GetNext(pos);

		if(SameAndIncludeRecord(&iList, pRec, exceptionflag) == FALSE)
			iList.AddTail(new CBMRecord(pRec));
	}

	ShellSortBMRecord(&iList, &CBMRecord::CompareBuje);

	AhPtrListDelete(m_BMCrossBeamList,(CBMRecord*)0);
	// new, delete 없이 pointer만 copy
	pos = iList.GetHeadPosition();
	while(pos)
	{
		CBMRecord *pBMRec = (CBMRecord*)iList.GetNext(pos);
		m_BMCrossBeamList.AddTail(pBMRec);
	}
}


BOOL CBMCreator::IsCarBuje(CBMRecord *pRec)
{
/*	if(pRec->m_nMarkGrandAssem==ASSEMBLYBIG_BOX)	return TRUE;
	if(pRec->m_nMarkGrandAssem==ASSEMBLYBIG_DR)		return TRUE;
	if(pRec->m_nMarkGrandAssem==ASSEMBLYBIG_MH)		return TRUE;
	if(pRec->m_nMarkGrandAssem==ASSEMBLYBIG_VP)		return TRUE;
	if(pRec->m_nMarkGrandAssem==ASSEMBLYBIG_SHOE)	return TRUE;

	// 대조립 마크 가로보 브라켓 박스에 사용
	CBaseChief *pBC	 = (CBaseChief*)m_pDB->m_pBaseChief;
	if(pBC->m_BaseEtcCtl.GetAssmUseCrossBeamBracketBoxOfBigAssm())
	{
		if(pRec->m_nMarkGrandAssem==ASSEMBLYBIG_CR_BRACKET_LEFT)	return TRUE;
		if(pRec->m_nMarkGrandAssem==ASSEMBLYBIG_CR_BRACKET_RIGHT)	return TRUE;
	}
	if(pBC->m_BaseEtcCtl.GetSummaryAddSpliceWeight() == TRUE && pRec->m_nMarkGrandAssem==ASSEMBLYBIG_SP)
		return TRUE;

	return FALSE;*/
	CGlobarOption *pGOpt = m_pDataManage->GetGlobalOption();

	if(pRec->m_nMarkBuje==G_F_U)	return TRUE;
	if(pRec->m_nMarkBuje==G_F_L)	return TRUE;
	if(pRec->m_nMarkBuje==G_W  )	return TRUE;
	if(pRec->m_nMarkBuje==G_H_L)	return TRUE;
	if(pRec->m_nMarkBuje==G_H_R)	return TRUE;
	if(pRec->m_nMarkBuje==V_ST)		return TRUE;
	if(pRec->m_nMarkBuje==V_J_ST)	return TRUE;

	if(pGOpt->GetSummaryAddSpliceWeight() == TRUE)
	{
		if(pRec->m_nMarkBuje==SP_U_U	)	return TRUE;
		if(pRec->m_nMarkBuje==SP_U_L	)	return TRUE;
		if(pRec->m_nMarkBuje==SP_U_BOLT )	return TRUE;
		if(pRec->m_nMarkBuje==SP_L_L	)	return TRUE;
		if(pRec->m_nMarkBuje==SP_L_U	)	return TRUE;
		if(pRec->m_nMarkBuje==SP_L_BOLT )	return TRUE;
		if(pRec->m_nMarkBuje==SP_W_MO	)	return TRUE;
		if(pRec->m_nMarkBuje==SP_W_SH	)	return TRUE;
		if(pRec->m_nMarkBuje==SP_W_BOLT )	return TRUE;
	}

	if(pGOpt->GetAssmUseCrossBeamBracketGirderOfBigAssm() == TRUE)
	{
		if(pRec->m_nMarkBuje==CP_BR_W	)	return TRUE;
		if(pRec->m_nMarkBuje==CP_BR_F_U	)	return TRUE;
		if(pRec->m_nMarkBuje==CP_BR_F_L )	return TRUE;
		if(pRec->m_nMarkBuje==CP_BR_GA	)	return TRUE;
		if(pRec->m_nMarkBuje==CP_BR_GA_FA)	return TRUE;
		if(pRec->m_nMarkBuje==CG_BR_W	 )	return TRUE;
		if(pRec->m_nMarkBuje==CG_BR_F_U	)	return TRUE;
		if(pRec->m_nMarkBuje==CG_BR_F_L	)	return TRUE;
		if(pRec->m_nMarkBuje==CG_BR_GA )	return TRUE;
		if(pRec->m_nMarkBuje==CG_BR_GA_FA)	return TRUE;
	}

	if(pRec->m_nMarkBuje==GI_SB	)		return TRUE;
	if(pRec->m_nMarkBuje==GI_SO )		return TRUE;
	if(pRec->m_nMarkBuje==GI_FA	)		return TRUE;
	if(pRec->m_nMarkBuje==GI_LP	)		return TRUE;
	if(pRec->m_nMarkBuje==GI_LP_R	)	return TRUE;
	if(pRec->m_nMarkBuje==GI_LP_PL	)	return TRUE;
	if(pRec->m_nMarkBuje==GI_JA_L	)	return TRUE;
	if(pRec->m_nMarkBuje==GI_JA_R	)	return TRUE;
	if(pRec->m_nMarkBuje==GI_JA_SA	)	return TRUE;

	return FALSE;
}



