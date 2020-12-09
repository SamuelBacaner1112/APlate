   // WeldCreator.cpp: implementation of the CWeldCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "GangjeBase.h"
#include "WeldRecord.h"
#include "WeldCreator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
const double BOTHSIDE = 2;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWeldCreator::CWeldCreator(CDataManage* pMng)
{
	m_pDataManage = pMng;
	m_pDB = pMng->GetBridge();
	m_nWorkValue = 0;				// 0:생성, 1:tbl파일로드, 2:액셀파일로드
	m_nDimType	 = DIM_RECT;		// DimType
	m_nMakeFlag  = WELDMAKE_SANG;	// MakeFlag

	m_dwMakeMarkFlag = 0;
	m_bAlertWeld = TRUE;

/*	SetMakeMark(CMarkSerial::MARK_BUJE,TRUE);
	SetMakeMark(CMarkSerial::MARK_SECTION,TRUE);
	SetMakeMark(CMarkSerial::MARK_ASSEM,TRUE);
	SetMakeMark(CMarkSerial::MARK_ASSEM_MID,TRUE);
	SetMakeMark(CMarkSerial::MARK_ASSEM_BIG,TRUE);*/
}

CWeldCreator::~CWeldCreator()
{
	ClearListStandard();
	ClearListSang();
	ClearListAssembly();
	ClearListAssemblySang();
	ClearListAssemblyTotal();
	ClearListWeldType();
	ClearListWeldThick();
}

void CWeldCreator::ClearListStandard()
{
	AhPtrListDelete(m_ListStandard,(CWeldRecord*)0);
}

void CWeldCreator::ClearListAssembly()
{
	AhPtrListDelete(m_ListAssembly,(CWeldRecord*)0);
}
void CWeldCreator::ClearListSang()
{
	AhPtrListDelete(m_ListSang,(CWeldRecord*)0);
}
void CWeldCreator::ClearListAssemblySang()
{
	AhPtrListDelete(m_ListAssemblySang,(CWeldRecord*)0);
}
void CWeldCreator::ClearListAssemblyTotal()
{
	AhPtrListDelete(m_ListAssemblyTotal,(CWeldRecord*)0);
}

void CWeldCreator::ClearListWeldType()
{
	AhPtrListDelete(m_ListWeldType,(CWeldRecord*)0);
}

void CWeldCreator::ClearListWeldThick()
{
	AhPtrListDelete(m_ListWeldThick,(CWeldRecord*)0);
}

void CWeldCreator::AddFlangeUpper(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp	  *pGir  = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_STT_GIRDER|BX_UPPERFLANGE;

	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	long nMarkBuje	  = G_F_U;
	long nMarkBujeSec = G_F_U;
	double PreThick;

	while(pBx)
	{
// 		CPlateSplice *pSp = pBx->GetSplice();
// 
// 		if(pSp && pSp->m_bSpliceType)
// 		{
// 			pBx = Finder.GetBxNext();
// 			continue;	
// 		}

		if(pBx->IsState(BX_UPPERFLANGE))
		{
			CWeldRecord *pRec = AddWeldRecord();
			SetWeldRecordFact(pRec,pBx);
			pRec->SetStrSubEleName("상판");
			PreThick = pGir->GetThickJewon(nMarkBuje,Finder.GetBxFixPrev());
			pRec->m_dThickConnect = PreThick;
			SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);
			SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,PreThick);

			pRec->SetStrEleConnectName("일반");
			pRec->SetStrSubEleConnectName("상판");
			pRec->m_dLengthWeld = min(pGir->GetWidthOnStation(pBx->GetStation(),TRUE,TRUE),pGir->GetWidthOnStation(pBx->GetStation(),TRUE,FALSE))*pRec->m_nQty;
		}
		pBx = Finder.GetBxNext();
	}
}

void CWeldCreator::AddFlangeLower(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp	  *pGir  = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_STT_GIRDER|BX_LOWERFLANGE;

	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	long nMarkBuje	  = G_F_L;
	long nMarkBujeSec = G_F_L;
	double PreThick;

	while(pBx)
	{
// 		CPlateSplice *pSp = pBx->GetSplice();
// 
// 		if(pSp && pSp->m_bSpliceType)
// 		{
// 			pBx = Finder.GetBxNext();
// 			continue;	
// 		}

		if(pBx->IsState(BX_LOWERFLANGE))
		{
			CWeldRecord *pRec = AddWeldRecord();
			SetWeldRecordFact(pRec,pBx);
			pRec->SetStrSubEleName("하판");
			PreThick = pGir->GetThickJewon(nMarkBuje,Finder.GetBxFixPrev());
			pRec->m_dThickConnect = PreThick;
			SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);
			SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,PreThick);

			pRec->SetStrEleConnectName("일반");
			pRec->SetStrSubEleConnectName("하판");
			pRec->m_dLengthWeld = min(pGir->GetWidthOnStation(pBx->GetStation(),FALSE,TRUE),pGir->GetWidthOnStation(pBx->GetStation(),FALSE,FALSE))*pRec->m_nQty;
		}
		pBx = Finder.GetBxNext();
	}
}

// WEB - 상,하판 필렛용접
// WEB - 맞대기
void CWeldCreator::AddFlangeWeb(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER|BX_WEB;		             

	CPlateBasicIndex* pBx  = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	long nMarkBuje	  = G_W;
	long nMarkBujeSec = G_F_U;

	double sta1=0,sta2=0,FlgThick=0, dLength=0;
	while(pBx)
	{
// APLATE-698
// 현장이음 타입을 체크할 필요없음.. 
// 체크해서 continue하는 경우 S1을 제외하고는 복부판+상,하판 수량 누락
// 		CPlateSplice *pSp = pBx->GetSplice();
// 
// 		if(pSp && pSp->m_bSpliceType)
// 		{
// 			pBx = Finder.GetBxNext();
// 			continue;	
// 		}

		if(Finder.GetBxFixNext()==NULL) break;

		// 필렛용접 (상판)
		CWeldRecord *pRec = AddWeldRecord();
		SetWeldRecordFact(pRec,pBx);
		pRec->SetStrSubEleName("복부판");

		nMarkBujeSec = G_F_U;

		sta1 = pBx->GetStation();
		sta2 = Finder.GetBxFixNext()->GetStation();
		dLength = pGir->GetLine()->GetLengthByDis(sta1,sta2,pGir->m_dGirderCenter);;
		FlgThick = pGir->GetThickJewon(nMarkBujeSec,pBx);

		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,FlgThick);
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("상판");
		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld	  = dLength*nSide; // 내,외측
		pRec->m_dThickConnect = FlgThick;

		// 필렛용접 (하판)
		pRec = AddWeldRecord(pRec);
		nMarkBujeSec = G_F_L;
		FlgThick = pGir->GetThickJewon(nMarkBujeSec,pBx);
		
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,FlgThick);
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("하판");
		nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld	  = dLength*nSide; // 내,외측
		pRec->m_dThickConnect = FlgThick;

		// 공장이음 : 맞대기용접
		if(pBx->IsState(BX_WEB|BX_WEB))
		{
			pRec = AddWeldRecord(pRec);
			SetWeldRecordFact(pRec,pBx);
			nMarkBujeSec = nMarkBuje;
			FlgThick = pGir->GetThickJewon(nMarkBujeSec,Finder.GetBxFixPrev());
			pRec->m_dThickConnect = FlgThick;
			SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);
			SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,FlgThick);
			//
			nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
			pRec->m_dLengthWeld	= pGir->GetHeightGirderByStaAng(pBx->GetStation(),0,pBx->GetAngle())*nSide;
			pRec->SetStrEleConnectName("일반");
			pRec->SetStrSubEleConnectName("복부판");
		}

		pBx = Finder.GetBxNext();
	}
}

void CWeldCreator::SetJewonWeldRecord(CWeldRecord *pRec, CPlateBasicIndex *pBx, long nMarkBuje, long nMarkBujeSec,long nRow, long nCol, BOOL bFront, BOOL bLeftBeam)
{	
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption  *pGopt = m_pDataManage->GetGlobalOption();
	// Girder.....
	pRec->m_nGirder	         = pBx->GetNumberGirder();
	pRec->m_nMarkBuje        = nMarkBuje;
	pRec->m_nMarkBujeConnect = nMarkBujeSec;
	// 제원 SETTING
	CVector vJewon;
	vJewon.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
	vJewon.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
	vJewon.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
	pRec->m_vJewon = vJewon;

	// 용접 SETTING
	long nIndexWeld	 = pDB->GetIndexBaseWeld(nMarkBuje,nMarkBujeSec,pBx);
	if(nIndexWeld==-1)	return;

	CSteelWeld	   *pWeld     = m_pDataManage->GetSteelWeld();
	CSteelWeldData *pWeldData = pWeld->GetBaseWeldByIndex(nIndexWeld);
	if(pWeldData == NULL)
	{
		if(m_bAlertWeld)
		{
			AfxMessageBox(_T("용접설정 자료가 없습니다. \n[기초자료]-[설계정보]-[용접상세] 메뉴에서 기본값을 누르신후 진행해주세요"));
			m_bAlertWeld = FALSE;
		}
		return;
	}
	pRec->m_sWeldType	 = pWeld->GetWeldTypeByIndex(pWeldData->m_nWeldType);
	pRec->m_sImproveType = pWeld->GetImproveTypeByIndex(pWeldData->m_nImproveType);
	pRec->m_sLayType	 = pWeld->GetLayTypeByIndex(pWeldData->m_nLayType);
	pRec->m_nWeldType	 = pWeldData->m_nWeldType;
	pRec->m_nImproveType = pWeldData->m_nImproveType;
	pRec->m_nLayType	 = pWeldData->m_nLayType;
	pRec->m_nQty		 = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
	
	//개선형태 설정 (X개선 or V개선) 접속부재의 두께에 따라서 개선방법을 결정함
	//pRec->m_dThickConnect가 설정이 되어있어야함.............
	// CBaseWeldCtl::GetItemValue(CString Text)와 일치하여야 함

	if(pWeldData->m_nImproveType==5) // "두께에 따라 V형 또는 X형 개선"
	{
		double dMinThick   = min(pRec->m_dThickConnect,pRec->m_vJewon.y);
		double dXWeldThick = pGopt->GetDesignThickXWeldApply();
		if(dMinThick<=dXWeldThick)
			pRec->m_nImproveType = 3;
		else
			pRec->m_nImproveType = 4;
		pRec->m_sImproveType = pWeld->GetImproveTypeByIndex(pRec->m_nImproveType);
	}
}

void CWeldCreator::SetThickWeldByType(CWeldRecord *pRec, CPlateBasicIndex *pBx, long nMarkBuje,
										long nMarkBujeSec, double dThickSec)
{
	CPlateGirderApp	*pGir		= pBx->GetGirder();
	CPlateBridgeApp	*pDB		= m_pDB;
	CSteelWeld		*pWeld		= m_pDataManage->GetSteelWeld();
	CGangjeBase pBase;
	CString sWeldType = pRec->m_sWeldType;
	
	double Thick = 0;

	pRec->m_dThickWeld	  = Thick;
	pRec->m_dThickConnect = dThickSec;

	// 필렛용접
	if(sWeldType.Find("필렛")!=-1)
		pRec->m_dThickWeld = pGir->GetSizeWeldFillet(pBx, nMarkBuje, nMarkBujeSec);
		//pRec->m_dThickWeld = pBase.GetThickFilletWeld(pRec->m_vJewon.y,dThickSec);
	// 플레어용접 : 6mm로 고정함. 2009.09.24 이종원
	else if(sWeldType.Find("플래어")!=-1)
		pRec->m_dThickWeld = 6;//pBase.GetThickFilletWeld(pRec->m_vJewon.y,dThickSec);
	// 맞대기용접
	else if(sWeldType.Find("맞대기")!=-1)
	{
		pRec->m_dThickWeld = max(pRec->m_vJewon.y,dThickSec);
		// 용접 SETTING
		long nIndexWeld	 = pDB->GetIndexBaseWeld(nMarkBuje,nMarkBujeSec,pBx);
		if(nIndexWeld!=-1)	
		{		
			CSteelWeldData *pBaseWeld = pWeld->GetBaseWeldByIndex(nIndexWeld);
			if( pBaseWeld->m_nWeldType == 1)
			{
				if( ((nMarkBuje==CP_BR_F_U || nMarkBuje==CG_BR_F_U) && nMarkBujeSec==G_F_U) || 
					(nMarkBuje==ST_BR_F_U && (nMarkBujeSec==CP_F_U || nMarkBujeSec==CG_F_U)) ||
					((nMarkBuje==WI_P_BR_F_U || nMarkBuje==WI_G_BR_F_U) && nMarkBujeSec==G_F_U))
				{
					pRec->m_dThickWeld = min(pGir->GetThickJewon(nMarkBuje, pBx), pGir->GetThickJewon(nMarkBujeSec, pBx)) - 2;
				}
			}			
		}
		if((nMarkBuje==G_F_U || nMarkBuje==G_F_L ) && (nMarkBujeSec==G_F_U || nMarkBujeSec==G_F_L))
		{			
			pRec->m_dThickWeld = min(pBx->GetFactChain(nMarkBuje,FALSE)->m_dFactChainThick, pBx->GetFactChain(nMarkBuje,TRUE)->m_dFactChainThick);
		}
	}
	// T형 홈용접
	else if(sWeldType.Find("홈용접")!=-1)
		pRec->m_dThickWeld = long(RoundUp((pRec->m_vJewon.y-2)/2,0));
}

void CWeldCreator::AddStud(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{	
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER|BX_SPLICE,pBxStt,pBxEnd,TRUE,FALSE);

	if(pBxStt->GetGirder()->m_pSangse->m_StudHori_Type == 1) return;	// 전단연결재는 수량내지 않음

	long nMarkBujeSec = G_F_U;

	while(pBx)
	{
		CPlateGirderApp *pGir	= pBx->GetGirder();
		CPlateSangSeDo	*pSang	= pGir->m_pSangse;
		CWeldRecord		*pRec	= AddWeldRecord(NULL);
		SetWeldRecordFact(pRec, pBx);

		long nType = pSang->m_StudHori_Type;	// 0 앵커, 1 스터드, 2 각형
		long nMarkBuje;
		CString szBuje;
		if(nType == 0)
		{
			nMarkBuje = GI_SA;
			szBuje.Format("슬래브 앵커");
		}
		else
		{
			nMarkBuje = GI_SQ;
			szBuje.Format("전단연결재 각형");
		}

		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);
		pRec->SetStrSubEleName(szBuje);
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		long nSp = pGir->GetNumberSpliceByBx(pBx);
		if(pBx->GetSplice()) nSp += 1;
		// 전단연결재의 숫자 계산으로, 스터드 수만큼 곱해져 있다..
		pRec->m_nQty = (pBx->GetGirder()->GetStudBoltSu(nSp) / pGir->m_pSangse->m_StudHori_Qty) * nSide;
		if(nMarkBuje == GI_SA)		pRec->m_dLengthWeld	= pSang->m_SlabAnchor_B*pRec->m_nQty;
		else						pRec->m_dLengthWeld = pRec->m_vJewon.z*pRec->m_nQty;

		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec,pBx));
		pRec->SetStrEleConnectName(pRec->GetStrEleName());
		pRec->SetStrSubEleConnectName("상판");

		pBx = Finder.GetBxNext();
	}
}

void CWeldCreator::AddHBracing(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_HBRACINGGUSSET;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	
	while(pBx)
	{
		if(pBx == NULL) return;

		CSection *pSec = pBx->GetSection();

		AddHBracingGusset(pBx);	

		pBx = Finder.GetBxNext();
	}
}

void CWeldCreator::AddHBracingGusset(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CHBracingGusset *pHBr = pBx->GetHBracingGusset();
	if(!pHBr)	return;

	for(long nRow=0; nRow<2; nRow++)
	{
		if(pHBr->IsReal(nRow))
		{
			CWeldRecord *pRec = AddWeldRecord(NULL);
			SetWeldRecordFact(pRec,pBx);
			pRec->SetStrEleName("수평 브레이싱");
			pRec->SetStrSubEleName("거세트 플레이트");

			SetJewonWeldRecord(pRec, pBx, HB_GU,G_W, nRow, 0);
			SetThickWeldByType(pRec, pBx, HB_GU,G_W, pGir->GetThickJewon(G_W,pBx));
			pRec->SetStrEleConnectName(pRec->GetStrEleName());
			pRec->SetStrSubEleConnectName("복부판");
			long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
			
			pRec->m_nQty = nSide;
			pRec->m_dLengthWeld   = pRec->m_vJewon.x*nSide;
		}
	}
}

void CWeldCreator::AddVBracing(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	while(pBx)
	{
		if(pBx == NULL) return;

		CSection *pSec = pBx->GetSection();

		if(pSec && pSec->IsenDetType(SECDET_VBRACING))
			AddVBracingGusset(pBx);	

		pBx = Finder.GetBxNext();
	}
}

void CWeldCreator::AddVBracingGusset(CPlateBasicIndex* pBx)
{	
	CPlateGirderApp *pGir = pBx->GetGirder();
	CVBracing *pVr = pBx->GetSection()->GetVBracing();
	long nMarkBuje = VB_GU;

	for(long nCol=0; nCol<5; nCol++)
	{
		switch(nCol)
		{
		case 0:	// LT - 상현재, 사재(V형)
		case 2:	// RT - 상현재, 사재(V형)
			AddVBracingRecord(pBx, nCol, VB_UB);
			if(pVr->GetType() == TYPE_VBRACING)
				AddVBracingRecord(pBx, nCol, VB_CR);
			break;
		case 1:	// LD - 하현재, 사재(역V형)
		case 3:	// RD - 하현재, 사재(역V형)
			AddVBracingRecord(pBx, nCol, VB_LB);
			if(pVr->GetType() == TYPE_INVVBRACING)
				AddVBracingRecord(pBx, nCol, VB_CR);
			break;		
		case 4:	// CEN - 상(역V)or하(V)현재, 좌우 사재(용접길이 동일)
			AddVBracingRecord(pBx, nCol, TYPE_VBRACING ? VB_UB : VB_LB);
			AddVBracingRecord(pBx, nCol, VB_CR);
			AddVBracingRecord(pBx, nCol, VB_CR);
			break;
		}
	}
}

void CWeldCreator::AddVBracingRecord(CPlateBasicIndex *pBx, long nCol, long nMarkBujeSec)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CVBracing *pVr = pBx->GetSection()->GetVBracing();
	long nMarkBuje = VB_GU;

	if(pVr == NULL) return;

	CWeldRecord *pRec = AddWeldRecord(NULL);
	SetWeldRecordFact(pRec,pBx);
	pRec->SetStrEleName("수직 브레이싱");
	pRec->SetStrSubEleName("거세트 플레이트");

	SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec, 0, nCol);
	SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec,pBx));
	pRec->SetStrEleConnectName(pRec->GetStrEleName());
	pRec->m_nQty = 1;

	double dUH = pVr->GetSteelMaterialData(STEELMAT_UPPER)->m_dH;
	double dLH = pVr->GetSteelMaterialData(STEELMAT_LOWER)->m_dH;
	double dCH = pVr->GetSteelMaterialData(STEELMAT_DIAGONAL)->m_dH;

	switch(nMarkBujeSec)
	{	
	case VB_UB:	// nCol == 0, 2, 4
		pRec->SetStrSubEleConnectName("상현재");
		if(nCol == 0)	pRec->m_dLengthWeld = (dUH + pVr->m_dULG) * 2;
		if(nCol == 2)	pRec->m_dLengthWeld = (dUH + pVr->m_dURG) * 2;
		if(nCol == 4)	pRec->m_dLengthWeld = (dUH + pVr->m_dc3*2) * 2;
		break;
	case VB_LB: // nCol == 1, 3, 4
		pRec->SetStrSubEleConnectName("하현재");
		if(nCol == 1)	pRec->m_dLengthWeld = (dLH + pVr->m_dDLG) * 2;
		if(nCol == 3)	pRec->m_dLengthWeld = (dLH + pVr->m_dDRG) * 2;
		if(nCol == 4)	pRec->m_dLengthWeld = (dLH + pVr->m_dc3*2) * 2;
		break;
	case VB_CR:	// nCol == 0, 1, 2, 3, 4
		pRec->SetStrSubEleConnectName("사재");
		if(nCol < 2)		pRec->m_dLengthWeld = (dCH + pVr->m_dC1LG) * 2;
		else if(nCol < 4)	pRec->m_dLengthWeld = (dCH + pVr->m_dC2RG) * 2;
		else				pRec->m_dLengthWeld = (dCH + pVr->m_dC1RG) * 2;
		break;
	}
}


void CWeldCreator::AddCrossBeam(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp	*pDB	= m_pDB;
	CPlateGirderApp *pGir	= pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	while(pBx)
	{
		if(pBx == NULL) return;
		// 가로보 복부 - 가로보상판, 가로보하판, 
		// 브라켓 상판 - 거더상판or복부판, 브라켓복부판
		// 브라켓 하판 - 거더하판or복부판, 브라켓복부판
		// 제작타입일때 : 거더 복부판 - 브라켓복부판		

		if(pBx->IsCrossBeamRight() && pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
		{	
			AddCrossBeamMainFlange(pBx);
			AddCrossBeamMainHStiff(pBx);
			AddCrossBeamMainVStiff(pBx,TRUE);
			AddCrossBeamMainVStiff(pBx,FALSE);
			
			AddCrossBeamBrkUpperFlange(pBx, TRUE);
			AddCrossBeamBrkLowerFlange(pBx, TRUE);
			AddCrossBeamBrkUpperFlange(pBx, FALSE);
			AddCrossBeamBrkLowerFlange(pBx, FALSE);

			// 브라켓 보강판 설치시  - 거더복부판, 거더하판, 브라켓하판, 브라켓보강재
			if(pBx->GetCrossBeam()->m_cGasset_CR)
			{
				AddCrossBeamBrkSupportPlate(pBx, TRUE);
				AddCrossBeamBrkSupportPlatePan(pBx, TRUE);
				
				AddCrossBeamBrkSupportPlate(pBx, FALSE);
				AddCrossBeamBrkSupportPlatePan(pBx, FALSE);
			}

			AddCrossBeamAnchor(pBx);
		}

		pBx = Finder.GetBxNext();
	}
}

void CWeldCreator::AddCrossBeamMainFlange(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();

	long nMarkBuje     = CP_F_U;
	long nMarkBujeSec  = CP_W;

// APLATE-698
//  pGir->GetLengthCrossTaper(pBx,DIM_RECT)는 가로보 기준위치이므로 복부판길이와는 다름
// 	double dLengthWeld = pGir->GetLengthCrossTaper(pBx,DIM_RECT);

	if(pC->GetType()==CR_TYPE_HSTEEL)	return;

// 	if(m_pDataManage->GetGlobalOption()->GetDesignCrossBeamBracketType())// LEVEL TYPE
// 		dLengthWeld += (pC->m_uC1/2+pC->m_uB*pC->m_uC)/pBx->GetAngleSectionRight().y*BOTHSIDE;

	// 상플레이트
	CWeldRecord *pRec = AddWeldRecord(NULL);
	SetWeldRecordCrossBeam(pRec,pBx);
	pRec->SetStrSubEleName("상판");
	SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);

	long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
	pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
	SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T2_CR);
	pRec->SetStrEleConnectName(pRec->GetStrEleName());
	pRec->SetStrSubEleConnectName("복부판");

	// 하플레이트
	nMarkBuje = CP_F_L;
	pRec = AddWeldRecord(pRec);
	pRec->SetStrSubEleName("하판");
	SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);

	nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
	pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
	SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T1_CR);
	pRec->SetStrEleConnectName(pRec->GetStrEleName());
	pRec->SetStrSubEleConnectName("복부판");

	if(pC->GetType() == CR_TYPE_MANUFACTURE)
	{
		nMarkBuje		= CP_BR_W;
		nMarkBujeSec	= G_W;
		pRec = AddWeldRecord(pRec);
		pRec->SetStrSubEleName("브라켓 복부판");
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);

		nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_nQty		  = nSide;
		pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec, pBx));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("복부판");

		if(!pGir->m_pRight) return;
		CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
		pRec = AddWeldRecord(pRec);
		pRec->SetStrSubEleName("브라켓 복부판");
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec);

		nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_nQty		  = nSide;
		pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
		SetThickWeldByType(pRec,pBxR,nMarkBuje,nMarkBujeSec,pGir->m_pRight->GetThickJewon(nMarkBujeSec, pBxR));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("복부판");
	}
}

void CWeldCreator::AddCrossBeamAnchor(CPlateBasicIndex *pBx)
{
	CPlateGirderApp	*pGir	= pBx->GetGirder();
	CPlateBridgeApp	*pDB	= (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateCrossBeam	*pC		= pBx->GetCrossBeam();

	if(pC->m_nAnchorsu_CR==0)	return;


	long	nMarkBuje		= GI_SA;
	long	nMarkBujeSec	= pBx->IsJijum() ? CP_F_U : CG_F_U;

	CWeldRecord		*pRec	= AddWeldRecord(NULL);
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	pRec->SetStrEleName("지점부 가로보");

	SetJewonWeldRecord(pRec, pBx, nMarkBuje, nMarkBujeSec);
	pRec->SetStrSubEleName("슬래브 앵커");
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);

	long	nSide	= pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;

	pRec->m_nQty = pC->m_nAnchorsu_CR * nSide;
	pRec->m_dLengthWeld	= pGir->m_pSangse->m_SlabAnchor_B*pRec->m_nQty;

	SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec,pBx));
	pRec->SetStrEleConnectName(pRec->GetStrEleName());
	pRec->SetStrSubEleConnectName("상판");
}

void CWeldCreator::AddCrossBeamMainHStiff(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CGlobarOption   *pGlopt = m_pDataManage->GetGlobalOption();

	CDPoint Po[18];
	BOOL bIncludeGasset  = pGlopt->GetDesignCrossBeamHStiffAtBracket()==0 ? TRUE : FALSE;
	long nQty = pC->m_cVSide_CR==0 ? 2 : 1;	//양면수직보강재 이면 수평보강재는 2 그렇지 않으면 1
	long nV   = 0;
	BOOL bFront = pC->m_cVSide_CR==2 ? FALSE : TRUE;  
	if (bFront)
		nV = pGir->GetPointVStiffOnCrossBeamPrv(pBx, Po, FALSE, bIncludeGasset);
	else
		nV = pGir->GetPointVStiffOnCrossBeam(pBx, Po, FALSE, bIncludeGasset);

	long nH = pC->m_nHStiffUppersu_CR + pC->m_nHStiffLowersu_CR;
	long nColStt = bIncludeGasset&&pC->m_nLGirSpType  ? 0 : 1;
	long nColEnd = bIncludeGasset&&pC->m_nRGirSpType  ? nV-1 : nV-2;

	long nMarkBuje    = CP_H;
	long nMarkBujeSec = CP_W;

	for(long nRow=0; nRow<nH; nRow++) 
	{
		for(long nCol=nColStt; nCol<nColEnd; nCol++) 
		{
			CWeldRecord *pRec = AddWeldRecord(NULL);
			SetWeldRecordCrossBeam(pRec,pBx);
			pRec->SetStrSubEleName("수평보강재");
			SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol,bFront);

			long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
			pRec->m_nQty          = nQty*nSide;
			pRec->m_dLengthWeld   = pRec->m_vJewon.z*pRec->m_nQty;
			
			SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T3_CR);
			pRec->SetStrEleConnectName(pRec->GetStrEleName());
			pRec->SetStrSubEleConnectName("복부판");
			pRec->m_dThickConnect	= pC->m_T3_CR;
		}
	}
}

//	if(nSec==CP_W)		return  nCRO+13;
//	if(nSec==CP_F_U)	return  nCRO+14;
//	if(nSec==CP_F_L)	return  nCRO+15;
void CWeldCreator::AddCrossBeamMainVStiff(CPlateBasicIndex *pBx, BOOL bFront)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();

	if( bFront && pC->m_cVSide_CR==2) return;
	if(!bFront && pC->m_cVSide_CR==1) return;

	long nMarkBuje    = CP_V;
	long nMarkBujeSec = CP_W;
	long nRow = bFront ? 0 : 1;
	double Rad = atan(pGir->GetGiulFlangeUpperLowerCamber(pBx->GetStation()));

	for(long nCol=0; nCol<pC->m_nVStiffsu_CR; nCol++) 
	{
		// 가로보 복부
		nMarkBujeSec = CP_W;
		CWeldRecord *pRec = AddWeldRecord(NULL);
		pRec->SetStrSubEleName("수직보강재");
		SetWeldRecordCrossBeam(pRec,pBx);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol,bFront);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T3_CR);
		pRec->SetStrEleConnectName(pRec->GetStrEleName());
		pRec->SetStrSubEleConnectName("복부판");

		// 상 플레이트
		nMarkBujeSec = CP_F_U;
		pRec = AddWeldRecord(pRec);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol,bFront);

		nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld   = pRec->m_vJewon.x*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T2_CR);
		pRec->SetStrSubEleConnectName("상판");

		if(pC->m_H_CR>pC->m_vL_CR) continue;

		// 하 플레이트
		nMarkBujeSec = CP_F_L;
		pRec = AddWeldRecord(pRec);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol,bFront);
		nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld   = pRec->m_vJewon.x*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T1_CR);
		pRec->SetStrSubEleConnectName("하판");
	}
}

//	if(nSec==G_F_U)		return  nCRO+5;
//	if(nSec==CP_BR_W)	return  nCRO+6;
void CWeldCreator::AddCrossBeamBrkUpperFlange(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CWeldRecord		*pRec = NULL;

	long nMarkBuje    = CP_BR_F_U;
	long nMarkBujeSec = G_F_U;
	double Rad = atan(pGir->GetGiulCrossGasset(pBx,TRUE,TRUE));

	long nRow = 0;
	long nCol = bLeft ? 0 : 1;

	CPlateGirderApp *pGirR = pGir->m_pRight;
	double WebT  = pGir->GetThickJewon(G_W,pBx);		                 
	double Sharp = bLeft ? pGir->GetLengthSharpOnBx(pBx, TRUE)
		                 : pGirR->GetLengthSharpOnBx(pBx, TRUE);
	CDPoint vAng = bLeft ? pBx->GetAngleSectionRight()
		                 : pBx->GetBxMatchByCrossBeam(pGirR)->GetAngleSectionLeft();
	double  Tsec = 0;

	if(pC->GetType() == CR_TYPE_HSTEEL) return;	// 형강타입에는 브라켓상판이 없음

	// 거더상판 or 거더 복부판
	if((bLeft && pC->m_dLH1 == 0) || (!bLeft && pC->m_dRH1 == 0))
	{
		nMarkBujeSec = G_F_U;
		pRec = AddWeldRecord();
		pRec->SetStrSubEleName("브라켓 상판");
		SetWeldRecordCrossBeam(pRec,pBx);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);
		pRec->m_dLengthWeld   = pRec->m_vJewon.z*2;
		Tsec = pGir->GetThickJewon(nMarkBujeSec,pBx);
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,Tsec);
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("상판");
	}
	else
	{
		nMarkBujeSec = G_W;
		pRec = AddWeldRecord();
		pRec->SetStrSubEleName("브라켓 상판");
		SetWeldRecordCrossBeam(pRec,pBx);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);
		long  nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld   = pRec->m_vJewon.z * nSide;
		Tsec = pGir->GetThickJewon(nMarkBujeSec,pBx);
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,Tsec);
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("복부판");
	}
	
	// 제작타입 - 브라켓 복부판
	if(pC->GetType() == CR_TYPE_MANUFACTURE)
	{
		pRec = AddWeldRecord(pRec);
		pRec->SetStrSubEleName("브라켓 복부판");
		SetWeldRecordCrossBeam(pRec,pBx);		
		nMarkBuje	 = CP_BR_W;
		nMarkBujeSec = CP_BR_F_U;
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_nQty		  = nSide;
		pRec->m_dLengthWeld   = (pC->m_P_CR/vAng.y)*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T2_CR);
		pRec->SetStrEleConnectName(pRec->GetStrEleName());
		pRec->SetStrSubEleConnectName("브라켓 상판");
	}

	// 수직보강재타입 - 수직보강재
	if(pC->GetType() == CR_TYPE_VSTIFF)
	{
		pRec = AddWeldRecord(pRec);
		pRec->SetStrSubEleName("브라켓 상판");
		SetWeldRecordCrossBeam(pRec,pBx);
		nMarkBujeSec = pBx->IsJijum() ? V_J_ST : V_ST;
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_nQty		  = 2*nSide;
		pRec->m_dLengthWeld   = (pRec->m_vJewon.x/vAng.y)*pRec->m_nQty;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T3_CR);
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName(pBx->IsJijum() ? "지점부보강재" : "수직보강재");		
	}
}

//	if(nSec==G_F_L)		return  nCRO+7;
//	if(nSec==CP_BR_W)	return  nCRO+8;
//  복부판 누락
void CWeldCreator::AddCrossBeamBrkLowerFlange(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CWeldRecord		*pRec = NULL;

	long nMarkBuje    = CP_BR_F_L;
	long nMarkBujeSec = G_F_L;	
	double Rad = atan(pGir->GetGiulCrossGasset(pBx,TRUE,TRUE));

	long nRow = 0;
	long nCol = bLeft ? 0 : 1;
	long nSide= 0;

	CPlateGirderApp *pGirR = pGir->m_pRight;
	double WebT  = pGir->GetThickJewon(G_W,pBx);		                 
	double Sharp = bLeft ? pGir->GetLengthSharpOnBx(pBx, TRUE)
		                 : pGirR->GetLengthSharpOnBx(pBx, TRUE);
	CDPoint vAng = bLeft ? pBx->GetAngleSectionRight()
		                 : pBx->GetBxMatchByCrossBeam(pGirR)->GetAngleSectionLeft();

	if(pC->GetType() == CR_TYPE_HSTEEL)	return;	// 형강타입에는 브라켓하판이 없음

	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();
	if(!bLeft)
	{
		CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
		pBrGu = pBxR ? pBxR->GetHBracingGusset() : NULL;
	}

	if(pBrGu && (bLeft&&pBrGu->IsReal(1)) || (!bLeft&&pBrGu->IsReal(0))) return;

	// 거더 하판 or 거더 복부판	
	CDPoint A[10], B[10];
	double dHeight   = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dBrkLoY, dLoFlangeUpY;
	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	if(bLeft)
	{
		dBrkLoY = A[0].y - pC->m_H_CR;
		dLoFlangeUpY = B[1].y - dHeight;
	}
	else
	{
		CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGirR);
		dHeight = pGirR->GetHeightGirderByStaAng(pBxR->GetStation());

		dBrkLoY = A[7].y - pC->m_H_CR;
		dLoFlangeUpY = B[8].y - dHeight;
	}

	pRec = AddWeldRecord();
	pRec->SetStrSubEleName("브라켓 하판");
	SetWeldRecordCrossBeam(pRec, pBx);
	if(dBrkLoY <= dLoFlangeUpY)	// 브라켓 하판 - 거더 하판
	{
		nMarkBujeSec = G_F_L;
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

		pRec->m_dLengthWeld = pRec->m_vJewon.z*2;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec,pBx));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("하판");
	}
	else						// 브라켓 하판 - 거더 복부판
	{
		nMarkBujeSec = G_W;
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);
		
		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld   = pRec->m_vJewon.z * nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec,pBx));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("복부판");
	}

	// 제작타입 - 브라켓 복부판
	if(pC->GetType() == CR_TYPE_MANUFACTURE)
	{
		pRec = AddWeldRecord(pRec);
		pRec->SetStrSubEleName("브라켓 복부판");
		nMarkBuje	 = CP_BR_W;
		nMarkBujeSec = CP_BR_F_L;		
		SetWeldRecordCrossBeam(pRec, pBx);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_nQty		  = nSide;
		pRec->m_dLengthWeld   = (pC->m_P_CR/vAng.y)*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T1_CR);
		pRec->SetStrEleConnectName(pRec->GetStrEleName());
		pRec->SetStrSubEleConnectName("브라켓 하판");
	}

	// 수직보강재타입 - 수직보강재
	if(pC->GetType() == CR_TYPE_VSTIFF)
	{
		pRec = AddWeldRecord(pRec);
		pRec->SetStrSubEleName("브라켓 하판");
		nMarkBujeSec = pBx->IsJijum() ? V_J_ST : V_ST;
		SetWeldRecordCrossBeam(pRec, pBx);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_nQty		  = 2*nSide;
		pRec->m_dLengthWeld   = (pRec->m_vJewon.x/vAng.y)*pRec->m_nQty;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T3_CR);
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName(pBx->IsJijum() ? "지점부보강재" : "수직보강재");
	}
}

void CWeldCreator::AddCrossBeamBrkSupportPlate(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateBridgeApp* pBridge= (CPlateBridgeApp*)pGir->GetBridgeApp();

	if(!pC->m_cGasset_CR) return;

	long nMarkBuje    = CP_BR_GA;
	long nMarkBujeSec = G_W;
	double Rad = atan(pGir->GetGiulCrossGasset(pBx,TRUE,TRUE));

	long nRow = 0;
	long nCol = bLeft ? 0 : 1;

	CPlateGirderApp *pGirR = pGir->m_pRight;
	double WebT  = pGir->GetThickJewon(G_W,pBx);		                 
	double Sharp = bLeft ? pGir->GetLengthSharpOnBx(pBx, TRUE)
		                 : pGirR->GetLengthSharpOnBx(pBx, TRUE);
	CDPoint vAng = bLeft ? pBx->GetAngleSectionRight()
		                 : pBx->GetBxMatchByCrossBeam(pGirR)->GetAngleSectionLeft();
	// 박스 복부판
	CWeldRecord *pRec = AddWeldRecord(NULL);
	pRec->SetStrSubEleName("브라켓 보강판");
	SetWeldRecordCrossBeam(pRec,pBx);
	SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

	long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
	pRec->m_dLengthWeld   = (pBx->GetHeightGirderByBx()-pC->m_H_CR)*nSide;
	SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec,pBx));
	pRec->SetStrEleConnectName("일반");
	pRec->SetStrSubEleConnectName("복부판");

	long nID=0;
	CPlateCrossBeam* pC1 = pBx->GetSection()->GetCrossBeam();
	if((nCol==0 && pC1->m_nLGirSpType) || (nCol==1 && pC1->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC1->m_nLGirSpType) || (nCol==1 && !pC1->m_nRGirSpType))
		nID = 1;		//볼트이음

	// 박스하판
	double dS1 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S1);
	nMarkBujeSec = G_F_L;
	pRec = AddWeldRecord(pRec);
	SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

	nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
	pRec->m_dLengthWeld = (Sharp-dS1)/vAng.y*nSide;
	SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pGir->GetThickJewon(nMarkBujeSec,pBx));
	pRec->SetStrEleConnectName("일반");
	pRec->SetStrSubEleConnectName("하판");

	// 브라켓 하판
	if(pC->GetType() != CR_TYPE_HSTEEL)
	{
		nMarkBujeSec = CP_BR_F_L;
		pRec = AddWeldRecord(pRec);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

		nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld = (pC->m_P_CR-WebT)/vAng.y*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T1_CR);
		pRec->SetStrEleConnectName(pRec->GetStrEleName());
		pRec->SetStrSubEleConnectName("브라켓 하판");
	}
	else
	{
		nMarkBujeSec = CP_F_L;
		pRec = AddWeldRecord(pRec);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

		nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		pRec->m_dLengthWeld = (pC->m_P_CR-WebT)/vAng.y*nSide;
		SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pC->m_T1_CR);
		pRec->SetStrEleConnectName(pRec->GetStrEleName());
		pRec->SetStrSubEleConnectName("하판");
	}	
}

void CWeldCreator::AddCrossBeamBrkSupportPlatePan(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp	*pBridge = m_pDataManage->GetBridge();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();

	int nCol = bLeft?0:1;
	long nID=0;
	CPlateCrossBeam* pC1 = pBx->GetSection()->GetCrossBeam();
	if((nCol==0 && pC1->m_nLGirSpType) || (nCol==1 && pC1->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC1->m_nLGirSpType) || (nCol==1 && !pC1->m_nRGirSpType))
		nID = 1;		//볼트이음

	if(!pC->m_cGasset_CR) return;
	if(pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_T)==0)	return;
	if(pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_W)==0)	return;

	long nMarkBuje    = CP_BR_GA_FA;
	long nMarkBujeSec = CP_BR_GA;	

	long nRow = 0;
//	long nCol = bLeft ? 0 : 1;

	// 브라켓 보강판
	CWeldRecord *pRec = AddWeldRecord(NULL);
	pRec->SetStrSubEleName("브라켓 보강판");
	SetWeldRecordCrossBeam(pRec,pBx);
	SetJewonWeldRecord(pRec,pBx,nMarkBuje,nMarkBujeSec,nRow,nCol);

	long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
	pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
	SetThickWeldByType(pRec,pBx,nMarkBuje,nMarkBujeSec,pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_T));
	pRec->SetStrEleConnectName(pRec->GetStrEleName());
	pRec->SetStrSubEleConnectName("브라켓 보강재");
}

void CWeldCreator::AddVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	__int64 BxFlag = BX_VSTIFF | BX_JIJUMSTIFF;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd,TRUE,FALSE);

	while(pBx)
	{
		CSection* pSec = pBx->GetSection();
		if(pSec || pBx->IsState(BX_JIJUMSTIFF))
		{
			AddVStiff(pBx, TRUE, pBx->IsState(BX_JIJUMSTIFF));	//left
			AddVStiff(pBx, FALSE,pBx->IsState(BX_JIJUMSTIFF));	//right
		}		

		pBx = Finder.GetBxNext();
	}
}

void CWeldCreator::AddVStiff(CPlateBasicIndex *pBx, BOOL bLeft, BOOL bJijum)
{
	CPlateGirderApp *pGir	= pBx->GetGirder();
	long nCol		= bLeft ? 0 : 1;
	long nMarkBuje	= bJijum ? V_J_ST : V_ST;
	CString szSub	= bJijum ? _T("지점부보강재") : _T("수직보강재");	
	
	double dL = !bJijum ? pGir->GetLengthJewon(V_ST, pBx, 0, nCol)	: -1;
	double dW = !bJijum ? pGir->GetWidthJewon(V_ST, pBx, 0, nCol)	: -1;
	double dT = !bJijum ? pGir->GetThickJewon(V_ST, pBx, 0, nCol)	: -1;

//	if(dL == 0 || dW == 0 || dT == 0) return;
	CVStiff *pVStiff = bJijum ? pBx->GetJijumStiff() : pBx->GetSection()->GetVStiff();
	if(pVStiff->m_dWidth[nCol]==0 || pVStiff->m_dThick[nCol]==0) return;

	CWeldRecord *pRec = AddWeldRecord();
	SetWeldRecordFact(pRec,pBx);
	pRec->SetStrSubEleName(szSub);
	SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_W,0,nCol);

	// 수직보강재 위치에 제작타입 가로보가 설치되어 있을 경우
	if(pBx->IsState(BX_CROSSBEAM_VBRACING))
	{
		CPlateGirderApp *pGirCr	= NULL;
		if(bLeft && pGir->GetNumberGirder() > 0)
			pGirCr = pGir->m_pLeft;
		else if(!bLeft)
			pGirCr = pGir;
		CPlateBasicIndex *pBxCr = pGirCr ? pGir->GetBxMatchByCrossBeam(pBx, pGirCr) : NULL;
		CSection		*pSecCr	= pBxCr ? pBxCr->GetSection() : NULL;
		CPlateCrossBeam	*pC		= (pSecCr && pSecCr->IsenDetType(SECDET_CROSSBEAM)) ? pSecCr->GetCrossBeam() : NULL;
		if(pC && pC->GetType() == CR_TYPE_MANUFACTURE)
		{
			pRec->m_vJewon.z -= (pC->m_H_CR + pC->m_T1_CR);
			if((bLeft && pC->m_dRH1 > 0) || (!bLeft && pC->m_dLH1 > 0))
				pRec->m_vJewon.z -= pC->m_T2_CR;
		}
	}

	long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
	pRec->m_nQty = nSide;
	pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
	
	SetThickWeldByType(pRec,pBx,nMarkBuje,G_W,pGir->GetThickJewon(G_W,pBx));
	pRec->SetStrEleConnectName("일반");
	pRec->SetStrSubEleConnectName("복부판");

	if(pVStiff->m_dH[nCol][UP] == 0)
	{
		pRec = AddWeldRecord();
		SetWeldRecordFact(pRec,pBx);
		pRec->SetStrSubEleName(szSub);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_F_U, 0, nCol);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		double dWidthConnect = min((pGir->GetWidthJewon(G_F_U, pBx) - pGir->GetThickJewon(G_W, pBx) - 30)/2, pVStiff->m_dWidth[nCol]);
		pRec->m_dLengthWeld   = dWidthConnect*nSide;
		pRec->m_nQty = nSide;
		
		SetThickWeldByType(pRec,pBx,nMarkBuje,G_F_U,pGir->GetThickJewon(G_F_U,pBx));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("상판");
	}

	if(pVStiff->m_dH[nCol][DOWN] == 0)
	{
		pRec = AddWeldRecord();
		SetWeldRecordFact(pRec,pBx);
		pRec->SetStrSubEleName(szSub);
		SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_F_L, 0, nCol);

		long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
		double dWidthConnect = min((pGir->GetWidthJewon(G_F_L, pBx) - pGir->GetThickJewon(G_W, pBx) - 30)/2, pVStiff->m_dWidth[nCol]);
		pRec->m_dLengthWeld   = dWidthConnect*nSide;
		pRec->m_nQty = nSide;
		
		SetThickWeldByType(pRec,pBx,nMarkBuje,G_F_L,pGir->GetThickJewon(G_F_L,pBx));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("하판");
	}
}

void CWeldCreator::AddJackUpStiff(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CJackupStiff *pJackUp = pBx->GetJackupStiff();

	if(pJackUp == NULL)
		return;

	CWeldRecord *pRec = NULL;
	long nSide = 0;
	double dWidth = 0;
	double dLength = 0;
	vector<long> nArrJackupIdx;

	if(pJackUp->m_vJewon[0].x > 0.0 && pJackUp->m_vJewon[0].y > 0.0)
		nArrJackupIdx.push_back(0);
	if(pJackUp->m_vJewon[1].x > 0.0 && pJackUp->m_vJewon[1].y > 0.0)
		nArrJackupIdx.push_back(1);
	
	long nMarkBuje;
	long nJackupSize = nArrJackupIdx.size();
	long nJackupPos = 0;
	for(long nIdx = 0; nIdx < nJackupSize; ++nIdx)
	{
		nJackupPos = nArrJackupIdx[nIdx];
		nMarkBuje = (nJackupPos == 0) ? GI_JA_L : GI_JA_R;

		//하판
		pRec = AddWeldRecord();
		SetWeldRecordFact(pRec, pBx);
		pRec->SetStrSubEleName("잭업보강재");
		SetJewonWeldRecord(pRec, pBx, nMarkBuje, G_F_L, 0, 0);
		pRec->m_dLengthWeld = pRec->m_vJewon.x * pRec->m_nQty;

		SetThickWeldByType(pRec, pBx, nMarkBuje, G_F_L, pGir->GetThickJewon(G_F_L, pBx));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("하판");

		//복부판
		pRec = AddWeldRecord();
		SetWeldRecordFact(pRec, pBx);
		pRec->SetStrSubEleName("잭업보강재");
		SetJewonWeldRecord(pRec, pBx, nMarkBuje, G_W, 0, 0);

		pRec->m_dLengthWeld = pRec->m_vJewon.z * pRec->m_nQty;
		SetThickWeldByType(pRec, pBx, nMarkBuje, G_W, pGir->GetThickJewon(G_W, pBx));
		pRec->SetStrEleConnectName("일반");
		pRec->SetStrSubEleConnectName("복부판");
	}
}

void CWeldCreator::AddCrossBeamGirderVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp *pGir	= pBxStt->GetGirder();
	CPlateGirderApp *pGirL	= pGir->GetNumberGirder() != 0 ? pGir->m_pLeft : NULL;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	long nMarkBuje =V_ST;

	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) && !pBx->IsJijum())	// 지점일때는 수직보강재에서
		{
			CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
			CVStiff *pVStiff = pC ? pC->m_pVStiff : NULL;
			if(pVStiff==NULL) continue;

			CPlateBasicIndex *pBxL	= pGirL ? pGir->GetBxMatchByCrossBeam(pBx, pGirL) : NULL;
			CSection		 *pSecL	= pBxL  ? pBxL->GetSection() : NULL;
			CPlateCrossBeam	 *pCL	= (pSecL && pSecL->IsenDetType(SECDET_CROSSBEAM)) ? pSecL->GetCrossBeam() : NULL;

			for(long nCol=0; nCol<2; nCol++)
			{
				if(pVStiff->m_dWidth[nCol] <= 0)	continue;

				CPlateCrossBeam *pCCur = nCol==0 ? pCL : pC;

				CWeldRecord *pRec = AddWeldRecord();
				SetWeldRecordFact(pRec,pBx);
				pRec->SetStrSubEleName("수직보강재");
				SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_W, 0, nCol);

				if(pCCur && pCCur->GetType() == CR_TYPE_MANUFACTURE)
				{
					pRec->m_vJewon.z -= (pCCur->m_H_CR + pCCur->m_T1_CR);
					if((nCol == 0 && pCCur->m_dRH1 > 0) || (nCol == 1 && pCCur->m_dLH1 > 0))
						pRec->m_vJewon.z -= pCCur->m_T2_CR;
				}

				long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
				pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
				pRec->m_nQty = nSide;
				
				SetThickWeldByType(pRec,pBx,nMarkBuje,G_W,pGir->GetThickJewon(G_W,pBx));
				pRec->SetStrEleConnectName("일반");
				pRec->SetStrSubEleConnectName("복부판");

				if(pVStiff->m_dH[nCol][UP] == 0)
				{
					pRec = AddWeldRecord();
					SetWeldRecordFact(pRec,pBx);
					pRec->SetStrSubEleName("수직보강재");
					SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_F_U, 0, nCol);

					long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
					double dWidthConnect = min((pGir->GetWidthJewon(G_F_U, pBx) - pGir->GetThickJewon(G_W, pBx) - 30)/2, pVStiff->m_dWidth[nCol]);
					pRec->m_dLengthWeld   = dWidthConnect*nSide;
					pRec->m_nQty = nSide;
					
					SetThickWeldByType(pRec,pBx,nMarkBuje,G_F_U,pGir->GetThickJewon(G_F_U,pBx));
					pRec->SetStrEleConnectName("일반");
					pRec->SetStrSubEleConnectName("상판");
				}

				if(pVStiff->m_dH[nCol][DOWN] == 0)
				{
					pRec = AddWeldRecord();
					SetWeldRecordFact(pRec,pBx);
					pRec->SetStrSubEleName("수직보강재");
					SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_F_L, 0, nCol);

					long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
					double dWidthConnect = min((pGir->GetWidthJewon(G_F_L, pBx) - pGir->GetThickJewon(G_W, pBx) - 30)/2, pVStiff->m_dWidth[nCol]);
					pRec->m_dLengthWeld   = dWidthConnect*nSide;
					pRec->m_nQty = nSide;
					
					SetThickWeldByType(pRec,pBx,nMarkBuje,G_F_L,pGir->GetThickJewon(G_F_L,pBx));
					pRec->SetStrEleConnectName("일반");
					pRec->SetStrSubEleConnectName("하판");
				}				
			}
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}
void CWeldCreator::AddVBracingGirderVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp *pGir	= pBxStt->GetGirder();
	CPlateGirderApp *pGirL	= pGir->GetNumberGirder() != 0 ? pGir->m_pLeft : NULL;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	long nMarkBuje = V_ST;

	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_VBRACING) && !pBx->IsJijum())	// 지점일때는 수직보강재에서
		{
			CVBracing *pVr = pBx->GetSection()->GetVBracing();
			CVStiff *pVStiff = pVr ? pVr->m_pVStiff : NULL;
			if(pVStiff==NULL) continue;

			CPlateBasicIndex *pBxL	= pGirL ? pGir->GetBxMatchByCrossBeam(pBx, pGirL) : NULL;
			CSection		 *pSecL	= pBxL  ? pBxL->GetSection() : NULL;
			CPlateCrossBeam	 *pCL	= (pSecL && pSecL->IsenDetType(SECDET_CROSSBEAM)) ? pSecL->GetCrossBeam() : NULL;			

			for(long nCol=0; nCol<2; nCol++)
			{
				if(pVStiff->m_dWidth[nCol] <= 0)	continue;

				CPlateCrossBeam *pCCur = nCol==0 ? pCL : NULL;

				CWeldRecord *pRec = AddWeldRecord();
				SetWeldRecordFact(pRec,pBx);
				pRec->SetStrSubEleName("수직보강재");
				SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_W, 0, nCol);

				if(nCol==0 && pCL && pCL->GetType() == CR_TYPE_MANUFACTURE)
				{
					pRec->m_vJewon.z -= (pCL->m_H_CR + pCL->m_T1_CR);
					if(pCL->m_dRH1 > 0)
						pRec->m_vJewon.z -= pCL->m_T2_CR;
				}

				long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
				pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
				pRec->m_nQty = nSide;
				
				SetThickWeldByType(pRec,pBx,nMarkBuje,G_W,pGir->GetThickJewon(G_W,pBx));
				pRec->SetStrEleConnectName("일반");
				pRec->SetStrSubEleConnectName("복부판");

				if(pVStiff->m_dH[nCol][UP] == 0)
				{
					pRec = AddWeldRecord();
					SetWeldRecordFact(pRec,pBx);
					pRec->SetStrSubEleName("수직보강재");
					SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_F_U, 0, nCol);

					long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
					double dWidthConnect = min((pGir->GetWidthJewon(G_F_U, pBx) - pGir->GetThickJewon(G_W, pBx) - 30)/2, pVStiff->m_dWidth[nCol]);
					pRec->m_dLengthWeld   = dWidthConnect*nSide;
					pRec->m_nQty = nSide;
					
					SetThickWeldByType(pRec,pBx,nMarkBuje,G_F_U,pGir->GetThickJewon(G_F_U,pBx));
					pRec->SetStrEleConnectName("일반");
					pRec->SetStrSubEleConnectName("상판");
				}

				if(pVStiff->m_dH[nCol][DOWN] == 0)
				{
					pRec = AddWeldRecord();
					SetWeldRecordFact(pRec,pBx);
					pRec->SetStrSubEleName("수직보강재");
					SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_F_L, 0, nCol);

					long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
					double dWidthConnect = min((pGir->GetWidthJewon(G_F_L, pBx) - pGir->GetThickJewon(G_W, pBx) - 30)/2, pVStiff->m_dWidth[nCol]);
					pRec->m_dLengthWeld   = dWidthConnect*nSide;
					pRec->m_nQty = nSide;
					
					SetThickWeldByType(pRec,pBx,nMarkBuje,G_F_L,pGir->GetThickJewon(G_F_L,pBx));
					pRec->SetStrEleConnectName("일반");
					pRec->SetStrSubEleConnectName("하판");
				}				
			}
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}

void CWeldCreator::AddHStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeftWeb)
{
	CPlateGirderApp*	pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);

	__int64 bxFlag  = BX_PLATEHSTIFF;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(bxFlag,pBxStt,pBxEnd,TRUE,FALSE);

	long nMarkBuje  = bLeftWeb ? G_H_L : G_H_R;
	double WebThick;

	while( pBx )
	{		
		CWebHStiff *pHs = pBx->GetWebHStiff();
		for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
		{
			if(pHs->IsEmpty(nDan, bLeftWeb))	continue;
			//
			CWeldRecord *pRec = AddWeldRecord();
			SetWeldRecordFact(pRec,pBx);
			pRec->SetStrSubEleName("수평보강재");
			SetJewonWeldRecord(pRec,pBx,nMarkBuje,G_W,nDan);

			long nSide = pRec->m_sLayType.Find("양면") != -1 ? 2 : 1;
			pRec->m_dLengthWeld   = pRec->m_vJewon.z*nSide;
			WebThick = pGir->GetThickJewon(G_W,pBx);
			SetThickWeldByType(pRec,pBx,nMarkBuje,G_W,WebThick);
			pRec->SetStrEleConnectName("일반");
			pRec->SetStrSubEleConnectName("복부판");
		}
		pBx = Finder.GetBxNext();
	}
}

BOOL CWeldCreator::SameAndIncludeRecord(CPtrList* pList, const CWeldRecord *pRec,
										DWORD exceptionFlag)
{
	BOOL bSameAndInclude = FALSE;

	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		CWeldRecord * pSouRec = (CWeldRecord *)pList->GetNext(pos);
		if( pRec->IsSameRecord( pSouRec,exceptionFlag ) )
		{
			pSouRec->m_nQty        += pRec->m_nQty;
			pSouRec->m_dLengthWeld += pRec->m_dLengthWeld;
			bSameAndInclude = TRUE;
			break;
		}
	}

	return bSameAndInclude;
}

CWeldRecord* CWeldCreator::AddWeldRecord(CWeldRecord *pRec)
{
	pRec = new CWeldRecord(pRec);

	m_ListStandard.AddTail(pRec);

	return pRec;
}


void CWeldCreator::SetWeldRecordFact(CWeldRecord *pRec, CPlateBasicIndex* pBx)
{
	CPlateGirderApp*	pGir = pBx->GetGirder();

	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
	pRec->SetStrEleName("일반");

}

void CWeldCreator::SetWeldRecordCrossBeam(CWeldRecord *pRec, CPlateBasicIndex* pBx)
{
	CPlateGirderApp*	pGir = pBx->GetGirder();

	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	if(pBx->IsJijum())
		pRec->SetStrEleName("지점부 가로보");
	else
		pRec->SetStrEleName("분배 가로보");
	pRec->SetStrSectionType("가로보");
}

CString CWeldCreator::GetMarkBx(long nMarkType,long nMark,CPlateBasicIndex* pBx,long nRow,long nCol, BOOL bFront) const
{
	if(!IsMakeMark(nMarkType)) return _T("");

/*	switch(nMarkType)
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
	}*/
	
	return _T("");
}

void CWeldCreator::SetMakeMark(long nMAKE_MARK,BOOL bApply)
{
	SetBitFlag32(m_dwMakeMarkFlag,nMAKE_MARK,bApply);
}

BOOL CWeldCreator::IsMakeMark(long nMAKE_MARK) const
{
	return GetBitFlag32(m_dwMakeMarkFlag,nMAKE_MARK);
}

// 세로보
void CWeldCreator::AddStringer(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{

	CPlateGirderApp *pGir = pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_CROSSBEAM_VBRACING;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	while(pBx)
	{
		if(pBx->IsCrossBeamRight())
		{
			for(long nIdxStr=0; nIdxStr<pBx->GetCrossBeam()->m_nQtyStringer_CR; nIdxStr++)
			{
/*
				CStringer *pStr = pBx->GetStringer(nIdxStr);
				pStr->SetStringerPointAll();

				AddStringerMainFlange(pStr);
				AddStringerMainHStiff(pStr);
				AddStringerMainVStiff(pStr);
				//
				AddStringerBrkWeb(pStr, TRUE);
				AddStringerBrkUpperFlange(pStr, TRUE);
				AddStringerBrkLowerFlange(pStr, TRUE);
				AddStringerBrkSupportPlate(pStr, TRUE);
				AddStringerBrkSupportPlatePan(pStr, TRUE);
				//
				AddStringerBrkWeb(pStr, FALSE);
				AddStringerBrkUpperFlange(pStr, FALSE);
				AddStringerBrkLowerFlange(pStr, FALSE);
				AddStringerBrkSupportPlate(pStr, FALSE);
				AddStringerBrkSupportPlatePan(pStr, FALSE);
*/
			}
		}
		//
		pBx = Finder.GetBxNext();
	}

}
