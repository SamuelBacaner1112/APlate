// FemHLLCalc.cpp: implementation of the CFemHLLCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSectionJewon::CSectionJewon()
{
	m_nType	= 0;
	m_nIdx = -1;
	m_pBx	= NULL;
	m_dTu	= 0;
	m_dTl	= 0;
	m_dBu	= 0;
	m_dBl	= 0;
	m_dH	= 0;
	m_dTw	= 0;
	m_dI33	= 0;
	m_dI22	= 0;
	m_dA	= 0;
	m_dJ	= 0;
	m_bNeedDelete = FALSE;
	// 실제 단면으로 사용되었는지에 대한 여부
	m_bUsedCalc = FALSE;
}

CSectionJewon::~CSectionJewon()
{
}

bool CSectionJewon::operator==(const CSectionJewon& rhs) const
{
	if(this == &rhs) 
		return false;

	if(m_dTu    != rhs.m_dTu)   return false;
	if(m_dTl	!= rhs.m_dTl)	return false;
	if(m_dBu    != rhs.m_dBu)   return false;
	if(m_dBl	!= rhs.m_dBl)	return false;
	if(m_dTw	!= rhs.m_dTw)	return false;
	if((fabs(m_dH - rhs.m_dH) / m_dH) > 0.01)	return false;

	return true;	
}

CSectionJewon& CSectionJewon::operator=(const CSectionJewon& rhs)
{
	m_nType = rhs.m_nType;
	m_nIdx = rhs.m_nIdx;
	m_pBx = rhs.m_pBx;
	m_dTu = rhs.m_dTu;
	m_dTl = rhs.m_dTl;
	m_dBu = rhs.m_dBu;	
	m_dBl = rhs.m_dBl;	
	m_dH = rhs.m_dH;
	m_dTw = rhs.m_dTw;

	m_dI33 = rhs.m_dI33;	
	m_dI22 = rhs.m_dI22;	

	m_dA = rhs.m_dA;
	m_dJ = rhs.m_dJ;

	m_bNeedDelete = rhs.m_bNeedDelete;
	m_bUsedCalc = rhs.m_bUsedCalc;

	return *this;
}

CFemHLLCalc::CFemHLLCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd)
{
	m_pDataManage = pDataManage;
	m_pCalcStd    = pCalcStd;
}

CFemHLLCalc::~CFemHLLCalc()
{

}

///////////////////////////////////////////////////////////////////
///	고정단		0
///	양방향단    1
///	횡방향가동  2
///	종방향가동  3
///	고정단 45도 4	
///*횡단면에서 거더위치에 절점을 생성함
///////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeJointRestraint(CFEMManage *pFEM, CPlateBasicIndex *pBx)
{	
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	long   nIndex = 0;
	double dMinL  = 100000000;
	///< pBx에서 가장 가까운 곳의 지점을 찾는다.
	long n = 0;
	for(n=0; n<pDB->m_nQtyJigan+1; n++)
	{
		CPlateBasicIndex *pBxJijum = pDB->GetGirder(-1)->GetBxOnJijum(n);
		if(ABS(pBx->GetStation()-pBxJijum->GetStation()) < dMinL)
		{
			dMinL  = ABS(pBx->GetStation()-pBxJijum->GetStation());
			nIndex = n;
		}
	}
	long nJ = 0;
	for(nJ=0; nJ<pFEM->GetJointSize(); nJ++)
	{
		if(pFEM->GetJoint((unsigned short)nJ)->m_LongAttrib[JOINT_TYPE] & GIRDER_NODE) break;
	}
	for(n=0; n<pDB->GetGirdersu(); n++)
	{
		CShoe *pShoe = pDB->GetGirder(n)->GetShoe(nIndex); 
		CJoint *pJoint = pFEM->GetJoint((unsigned short)nJ+(unsigned short)n);
		switch(pShoe->m_cDirectionShoe)
		{
			case 0:	// 양방향 고정(X,Y고정) 
					pJoint->m_UX = 1;
					pJoint->m_UY = 1;
					pJoint->m_UZ = 1;
					break;
			case 1: // 전체 자유
					pJoint->m_UX = 0;
					pJoint->m_UY = 1;
					pJoint->m_UZ = 1;
					break;
			case 2: // 횡방향가동(Y고정)
					pJoint->m_UX = 0;
					pJoint->m_UY = 1;
					pJoint->m_UZ = 1;
					break;
			case 3: // 종방향가동(X고정)
					pJoint->m_UX = 1;
					pJoint->m_UY = 1;
					pJoint->m_UZ = 1;
					break;
		}
		pJoint->m_RX = 1;
		pJoint->m_RY = 0;
		pJoint->m_RZ = 1;
	}
}
//////////////////////////////////////////////////////////////////////
/// pBx위치에서 횡단에서 거더절점을 생성함
//////////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeJointOnBx(CFEMManage *pFEM, CPlateBasicIndex *pBx, BOOL bWind)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	
	long   nIdx   = 0;
	double dBaseX = 0;
	CJoint *pJ;
	long nG = 0;
	pJ = pFEM->AddJoint((unsigned short)nIdx++, CVector(dBaseX,0,0));	///<좌측슬래브끝단
	pJ->m_LongAttrib[JOINT_TYPE]	= LEFT_END_NODE;	

	dBaseX += toM(pDB->GetWidthSlabLeft());				
		
	pJ = pFEM->AddJoint((unsigned short)nIdx++, CVector(dBaseX,0,0));	///<첫번째 거더	
	pJ->m_LongAttrib[JOINT_TYPE] = bWind ? SLAB_NODE : GIRDER_NODE;	
	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		dBaseX += toM(pDB->GetGirder(nG)->GetDisGirderToGirder(pBx));				
		pJ = pFEM->AddJoint((unsigned short)nIdx++, CVector(dBaseX,0,0));///<다음 거더		
		pJ->m_LongAttrib[JOINT_TYPE] = bWind ? SLAB_NODE : GIRDER_NODE;	
	}	

	dBaseX += toM(pDB->GetWidthSlabRight());				
	pFEM->AddJoint((unsigned short)nIdx, CVector(dBaseX,0,0));	///<우측슬래브끝단		
	pFEM->GetJoint((unsigned short)nIdx++)->m_LongAttrib[JOINT_TYPE] = RIGHT_END_NODE;	

	if(bWind)
	{	
		// 거더의 모델링
		CPlateGirderApp *pGir  =pDB->GetGirder(nG);
		double dGirderHalfH = toM(pGir->GetHeightGirderByStaAng(pBx->GetStation())/2);
		dBaseX = toM(pDB->GetWidthSlabLeft());	
		pJ = pFEM->AddJoint((unsigned short)nIdx++, CVector(dBaseX,0,-dGirderHalfH));	///<첫번째 거더	
		pJ->m_LongAttrib[JOINT_TYPE] = GIRDER_NODE;	
		for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
		{
			dBaseX += toM(pDB->GetGirder(nG)->GetDisGirderToGirder(pBx));				
			pJ = pFEM->AddJoint((unsigned short)nIdx++, CVector(dBaseX,0,-dGirderHalfH));///<다음 거더		
			pJ->m_LongAttrib[JOINT_TYPE] = GIRDER_NODE;	
		}	
	}

	MakeJointRestraint(pFEM, pBx);
}

void CFemHLLCalc::MakeJointOnGir(CFEMManage *pFEM, long nG)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CVector vBase(0,0,0);
	double dElemL=0;
	CDoubleArray arrElemL;

	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBxFinder Finder2(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while(Finder.GetBxFixNext())
	{	
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		CPlateBasicIndex *pBxN = Finder2.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE, Finder.GetBxFixNext());
		while(!pBxN->IsJoint())
			pBxN = Finder2.GetBxNext();

		dElemL = ABS(~(pGir->GetXyGirderDis(pBx->GetStation())-pGir->GetXyGirderDis(pBxN->GetStation())));
		arrElemL.Add(dElemL);
		pBx = Finder.GetBxNext();
	}
	
	double dJointPos=0;
	long   nIdx     = 0;	
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while(pBx)
	{	
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}

		if(nIdx==0)
			pFEM->AddJoint((unsigned short)nIdx, vBase);
		else
		{
			dJointPos += arrElemL[nIdx-1];
			pFEM->AddJoint((unsigned short)nIdx, CVector(toM(dJointPos), 0, 0));
		}
//		pFEM->GetJoint(nIdx)->m_pBx = pBx;
		pFEM->GetJoint((unsigned short)nIdx)->m_LongAttrib[JOINT_GIR]	= pBx->GetNumberGirder();
		pFEM->GetJoint((unsigned short)nIdx)->m_vPointReal.x			= pBx->GetStation();
		if(pBx->IsJijum())
		{
			pFEM->GetJoint((unsigned short)nIdx)->m_UX = 1;
			pFEM->GetJoint((unsigned short)nIdx)->m_UY = 1;
			pFEM->GetJoint((unsigned short)nIdx)->m_UZ = 1;
			pFEM->GetJoint((unsigned short)nIdx)->m_RX = 1;
		}
		++nIdx;
		pBx = Finder.GetBxNext();
	}
}

void CFemHLLCalc::MakeElementOnGir(CFEMManage *pFEM, long nG)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	
	long nEIdx     =0;
	long nJointIdx =0;	
	/////////////////////////////////////////////////////////////////////////
	///<거더 요소
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while(Finder.GetBxFixNext())
	{	
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		
		CElement *pEle = pFEM->AddElement((unsigned short)nEIdx, (unsigned short)nJointIdx, (unsigned short)nJointIdx+1);
		pEle->m_LongAttrib[ELEMENT_TYPE] = FRAME_GIRDER_ELEMENT;
		nEIdx++;
		nJointIdx++;
		pBx = Finder.GetBxNext();
	}
	nJointIdx++;
}

void CFemHLLCalc::MakeElementDefineSectionOnGir(CFEMManage *pFEM, long nSection)
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CFemStressCalc StressCalc(m_pDataManage, m_pCalcStd);
	///********************************************************
	///<1단계(거더 중 요소내 단면이 동일한 경우, 가로보요소)
	///********************************************************
	
	double dSttJointI33=0, dEndJointI33=0, dElmI33=0;
	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pElm  = (CElement*)pFEM->m_ElementList.GetNext(pos);
		CJoint   *pJStt = pFEM->GetJoint(pElm->m_nJStt);
		CJoint   *pJEnd = pFEM->GetJoint(pElm->m_nJEnd);
		CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pJStt);
		CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pJEnd);

		long nG = pBxStt->GetNumberGirder();
		double	dStaStt	= pBxStt->GetStation();
		double	dStaEnd = pBxEnd->GetStation();

		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)///<거더일 경우
		{
			ModelCalc.GetSectionCoefficient(nG, dStaStt, FALSE, dStaStt, 1);
			dSttJointI33 = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
			ModelCalc.GetSectionCoefficient(nG, dStaEnd, FALSE, dStaEnd, -1);
			dEndJointI33 = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
			
			if(IsSameI33(dSttJointI33, dEndJointI33))
			{
				CFrameSection *pFrameSec = ModelCalc.GetFrameSectionByI33(pFEM, dSttJointI33);
				if(pFrameSec)
					pElm->m_szFrameSectionName = pFrameSec->m_szName;
			}
			else ///<단면이 서로 다를 경우
				continue;
		}	
		else if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_SHOE_ELEMENT)///< 슈 
		{			
			CFrameSection *pFrameSec = (CFrameSection*)pFEM->m_FrameSectionList.GetTail();
			if(pFrameSec)
				pElm->m_szFrameSectionName = pFrameSec->m_szName;
		}
		else if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)///< 가로보
		{
			ModelCalc.GetSectionCoefficientCR(pBxStt);
			dElmI33 = ModelCalc.m_MapSecCoeCR["I33"];
			CFrameSection *pFrameSec = ModelCalc.GetFrameSectionByI33(pFEM, dElmI33);
			if(pFrameSec)
				pElm->m_szFrameSectionName = pFrameSec->m_szName;
		}
	}
	///********************************************************
	///<2단계
	///<거더요소를 쭉 훓어가면서 요소가 변화되는 구간의 단면을 추가한다.
	///********************************************************
	CStringArray arrSec;///<4자리수 : 예) 0507 [시작단면5.종점단면7]
	CString szTotal = "";
	POSITION posFr = pFEM->m_ElementList.GetHeadPosition();
	while(posFr)
	{
		CElement *pElm  = (CElement*)pFEM->m_ElementList.GetNext(posFr);
		CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pFEM->GetJoint(pElm->m_nJStt));
		CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pFEM->GetJoint(pElm->m_nJEnd));

		long	nG		= pBxStt->GetNumberGirder();
		double	dStaStt	= pBxStt->GetStation();
		double	dStaEnd	= pBxEnd->GetStation();
		
		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			ModelCalc.GetSectionCoefficient(nG, dStaStt, FALSE, dStaStt, 1);
			dSttJointI33 = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS, pBxStt);
			ModelCalc.GetSectionCoefficient(nG, dStaEnd, FALSE, dStaEnd, -1);
			dEndJointI33 = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS, pBxEnd, FALSE);

//			if(ModelCalc.IsSameSectionBetweenTwoBx(pBxStt, pBxEnd, AFTER_COMPOSITE_ANALYSIS))	continue;///<단면이 동일할 경우
			if(IsSameI33(dSttJointI33, dEndJointI33))
				continue;///<단면이 동일할 경우

			CString szSectionStt = ModelCalc.GetSectionNameByBx(pFEM, pBxStt, AFTER_COMPOSITE_ANALYSIS);
			CString szSectionEnd = ModelCalc.GetSectionNameByBx(pFEM, pBxEnd, AFTER_COMPOSITE_ANALYSIS);
			szTotal.Format("%2d%2d", atol(szSectionStt), atol(szSectionEnd));
			if(atol(szSectionStt)<10 && atol(szSectionEnd)<10)
				szTotal.Format("0%d0%d", atol(szSectionStt), atol(szSectionEnd));
			else if(atol(szSectionStt)>=10 && atol(szSectionEnd)< 10)
				szTotal.Format("%2d0%d", atol(szSectionStt), atol(szSectionEnd));
			else if(atol(szSectionStt)< 10 && atol(szSectionEnd)>=10)
				szTotal.Format("0%d%2d", atol(szSectionStt), atol(szSectionEnd));

			arrSec.Add(szTotal);
		}
	}
	CStringArray arrSecLast;///<중복되는 문자열은 제거하고 다시 생성
	long n = 0;
	for(n=0; n<arrSec.GetSize(); n++)///<범위를 좁혀가면서 중목된 문자는 제외함
	{
		CString szMain = arrSec.GetAt(n);
		long nEqual=0;
		for(long a=n; a<arrSec.GetSize(); a++)
		{
			if(szMain==arrSec.GetAt(a))
				nEqual++;
		}
		if(nEqual==1)	arrSecLast.Add(arrSec.GetAt(n));
	}
	for(n=0; n<arrSecLast.GetSize(); n++)///<arrSecLast를 이용하여 변단면구간을 정의함.
	{
		CString szSecChange = arrSecLast.GetAt(n);
		CString szStt = szSecChange.Left(2);  ///<"01"
		CString szEnd = szSecChange.Right(2); ///<"02"
		///<단면이름은 arrSecLast의 문자열을 바로 사용함
		pFEM->AddFrameSectionOfTaper(Comma(nSection), Comma(atol(szStt)), Comma(atol(szEnd)), 2, 1, "변화 단면", POS_EIVAR);
		nSection++;
	}
	///********************************************************
	///<3단계 : 요소의 시.종점위치의 단면이 상이할 경우
	///********************************************************
	pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pElm  = (CElement*)pFEM->m_ElementList.GetNext(pos);
		
		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pFEM->GetJoint(pElm->m_nJStt));
			CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pFEM->GetJoint(pElm->m_nJEnd));

			long nG = pBxStt->GetNumberGirder();
			double	dStaStt	= pBxStt->GetStation();
			double	dStaEnd = pBxEnd->GetStation();

			ModelCalc.GetSectionCoefficient(nG, dStaStt, FALSE, dStaEnd, 1);
			dSttJointI33 = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
			ModelCalc.GetSectionCoefficient(nG, dStaEnd, FALSE, dStaEnd, -1);
			dEndJointI33 = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);

			CString szSec1="", szSec2="";
			CString szElemSec = pElm->m_szFrameSectionName;

			if(IsSameI33(dSttJointI33, dEndJointI33))
				continue;
				
			CFrameSection *pFrameSecStt = ModelCalc.GetFrameSectionByI33(pFEM, dSttJointI33);
			CFrameSection *pFrameSecEnd = ModelCalc.GetFrameSectionByI33(pFEM, dEndJointI33);

			if(pFrameSecStt==NULL || pFrameSecEnd==NULL) continue;
			szSec1 = pFrameSecStt->m_szName;///<시작부의 I33이용하여 단면을 구해서 정의함
			szSec2 = pFrameSecEnd->m_szName;///<종점부의 I33이용하여 단면을 구해서 정의함

			POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
			while(pos)
			{
				CFrameSection *pFrameSec =  (CFrameSection*)pFEM->m_FrameSectionList.GetNext(pos);  
				///<현재단면의 시작단면과 종점단면이 szSec1, szSec2와 같으면 요소에 단면의 정의함
				if(szSec1==pFrameSec->m_szSecStt && szSec2==pFrameSec->m_szSecEnd)
				{
					pElm->m_szFrameSectionName = pFrameSec->m_szName;
					break;
				}
			}		
		}
	}
}

//////////////////////////////////////////////////////////////////////
/// 절점수-1만큼 요소를 생성함
///    -요소단면이름 : MSLAB
//////////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeElement(CFEMManage *pFEM, BOOL bWind)
{
	ASSERT(pFEM->GetJointSize());	
	CPlateBridgeApp *pDB  = m_pDataManage->GetBridge();
	
	long nEle=0;
	for(long n=0; n<pFEM->GetJointSize()-1; n++)
	{
		if(bWind)
		{
			if(pFEM->GetJoint((unsigned short)n+1)->m_LongAttrib[JOINT_TYPE] & GIRDER_NODE)
			{			
			}
			else
			{
				CElement *pEle = pFEM->AddElement((unsigned short)nEle++, (unsigned short)n, (unsigned short)n+1);
				pEle->m_szFrameSectionName = "MSLAB";
			}
		}
		else
		{		
			CElement *pEle = pFEM->AddElement((unsigned short)nEle++, (unsigned short)n, (unsigned short)n+1);
			pEle->m_szFrameSectionName = "MSLAB";
		}
	}

	if(bWind)
	{	
		for(long n=0; n<pFEM->GetJointSize()-1; n++)
		{		
			if(pFEM->GetJoint((unsigned short)n)->m_LongAttrib[JOINT_TYPE] & SLAB_NODE)
			{
				CElement *pEle = pFEM->AddElement((unsigned short)nEle++, (unsigned short)n, (unsigned short)n + (unsigned short)pDB->GetGirdersu()+1);
				pEle->m_szFrameSectionName = "MGIRDER";
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
/// 재질을 생성함
///    -콘크리트 : CONCRETE
//////////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeMaterial(CFEMManage *pFEM)
{
	CCalcData	*pCalcData	= m_pDataManage->GetCalcData();

	double	dEst	= tokNPM2(pCalcData->DESIGN_MATERIAL.m_dEst);
	double	dEc		= tokNPM2(pCalcData->DESIGN_MATERIAL.m_dEc);

	pFEM->AddMaterial("STEEL", dEst, 0.0, 'C', 0.0, 0.0, 0.0, 0.0);
	pFEM->AddMaterial("CONC", dEc, 0.0, 'C', 0.0, 0.0, 0.0, 0.0);
}

long CFemHLLCalc::MakeFrameSectionOnGir(CFEMManage *pFEM, long nG)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CPlateBasicIndex *pBx     = NULL;
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CFemStressCalc StressCalc(m_pDataManage, m_pCalcStd);
	CCalcGeneral		CalceGeneral(m_pDataManage);
	
	long nSection = 1;
	CString szSec = "";
	
	// 거더
	CPlateBxFinder Finder(pDB->GetGirder(nG));
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);

	vector <CSectionJewon> JewonArray;
	CSectionJewon vSec;
	
	while(pBx)
	{		
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		double	dSta	= pBx->GetStation();
		ModelCalc.GetSectionCoefficient(nG, dSta, FALSE, dSta, 0);

		vSec.m_pBx		= pBx;
		vSec.m_dA		= ModelCalc.GetSectionCoeffAs(AFTER_COMPOSITE_ANALYSIS);
		vSec.m_dI33		= ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
		vSec.m_dI22		= ModelCalc.GetSectionCoeffI22(AFTER_COMPOSITE_ANALYSIS);
		vSec.m_dJ		= ModelCalc.GetSectionCoeffK(AFTER_COMPOSITE_ANALYSIS);

		vSec.m_dBu		= ModelCalc.m_MapSecCoe["FLANGE_W_UP"];
		vSec.m_dTu		= ModelCalc.m_MapSecCoe["FLANGE_T_UP"];
		vSec.m_dBl		= ModelCalc.m_MapSecCoe["FLANGE_W_DN"];
		vSec.m_dTl		= ModelCalc.m_MapSecCoe["FLANGE_T_DN"];
		vSec.m_dH		= ModelCalc.m_MapSecCoe["WEB_H"];
		vSec.m_dTw		= ModelCalc.m_MapSecCoe["WEB_T"];

		JewonArray.push_back(vSec);

		pBx = Finder.GetBxNext();
	}		

	vector <CSectionJewon>::iterator iter, iterComp;
	for(iter=JewonArray.begin(); iter!=JewonArray.end(); iter++) iter->m_bNeedDelete = FALSE;
	for(iter=JewonArray.begin(); iter!=JewonArray.end(); iter++)
	{
		for(iterComp=iter+1; iterComp!=JewonArray.end(); iterComp++)
		{
			if(IsSameI33(iter->m_dI33, iterComp->m_dI33))
				iterComp->m_bNeedDelete = TRUE;
		}
	}
	JewonArray.erase(remove_if(JewonArray.begin(), JewonArray.end(), IsDelete), JewonArray.end());

	CString szUp, szLo, szWeb, szTemp;
	for(long nSec = 0; nSec <(long) JewonArray.size(); nSec++)
	{
		CSectionJewon	jewon = JewonArray[nSec];
		szSec.Format("%d", nSection);
		pFEM->AddFrameSection(szSec, "STEEL", toM2(JewonArray[nSec].m_dA), toM4(JewonArray[nSec].m_dI33), toM4(JewonArray[nSec].m_dI22), toM4(JewonArray[nSec].m_dJ), 0, "거더 단면", POS_GIRDER);
		CFrameSection	*pFrmSec	= pFEM->GetFrameSection(szSec);
		pFrmSec->m_dJewon[SEC_BU]	= JewonArray[nSec].m_dBu;
		pFrmSec->m_dJewon[SEC_TU]	= JewonArray[nSec].m_dTu;
		pFrmSec->m_dJewon[SEC_BL]	= JewonArray[nSec].m_dBl;
		pFrmSec->m_dJewon[SEC_TL]	= JewonArray[nSec].m_dTl;
		pFrmSec->m_dJewon[SEC_HW]	= JewonArray[nSec].m_dH;
		pFrmSec->m_dJewon[SEC_TW]	= JewonArray[nSec].m_dTw;
		CalceGeneral.GetMaterial(jewon.m_pBx, G_F_U, szUp, szTemp);
		CalceGeneral.GetMaterial(jewon.m_pBx, G_F_L, szLo, szTemp);
		CalceGeneral.GetMaterial(jewon.m_pBx, G_W,  szWeb, szTemp);
		pFrmSec->m_szJewon[FRM_UPPER]	= szUp;
		pFrmSec->m_szJewon[FRM_LOWER]	= szLo;
		pFrmSec->m_szJewon[FRM_WEB]		= szWeb;
		
		nSection++;
	}

	return nSection;
}

//////////////////////////////////////////////////////////////////////
/// 요소단면을 생성함
///    -슬래브 단면 :  MSLAB
///    -  거더 단면 :  MGIRDER
//////////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeFrameSection(CFEMManage *pFEM)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	double dT = pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;
	pFEM->AddFrameSection("MSLAB", "CONC", toM(dT), toM4(1000000000000.0), toM4(1000000000000.0), toM4(1000000000000.0), 0.0, "슬래브 단면", POS_SLAB);
	pFEM->AddFrameSection("MGIRDER", "STEEL", toM2(1000000.0), toM4(1000000000000.0), toM4(1000000000000.0), toM4(1000000000000.0), 0.0, "거더 단면", POS_GIRDER);
}

//////////////////////////////////////////////////////////////////////
/// Step 1 합성전 사하중
//////////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeForcePreDeadLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();	
	
	double  dLeftDist       = pDB->GetSlabLeft()->m_dGirderCenter;
	double  dRightDist      = pDB->GetSlabRight()->m_dGirderCenter;
	double  dUnitConc       = tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dChulgun);
	
	pFEM->m_szLCName.Add("BEFORE");	
	pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
//	pFEM->AddCombo("BEFORE", "BEFORE", CCombo::ADD, CCombo::LOAD, 1, FALSE);
	nLoadCaseIndex++;

	double	dBaseDist = 0, dPreDist = 0, dPreThick=0, dSlabThick=0, dTaperThick=0;	
	CString	sText	= _T("");
	CDPoint A[10];

	if(!pDB->IsTUGir())
	{
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);						
			CPlateBasicIndex *pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGir);
			if(pBxMatch==NULL)	continue;
			
			pGir->GetHunchXyDom(pBxMatch, A, FALSE);
			if(nG==0) //좌측캔틸레버 사하중
			{			
				dTaperThick = pGir->GetHeightTaper(pBxMatch->GetStation());
				dPreThick  = pDB->m_dThickSlabLeft + dTaperThick;
				dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[0].x) - A[0].y) - pDB->m_dThickPave;
				if(pDB->m_dHunchArray.GetSize() > 0)
				{
					AddTrapForceAtLeft(pFEM, 0, toM(A[0].x-dLeftDist-pDB->m_dHunchArray[0])
									,  -toM(dPreThick)*dUnitConc, -toM(pDB->m_dThickSlabLeftExt)*dUnitConc, 0, "슬래브");
					AddTrapForceAtLeft(pFEM, toM(A[0].x-dLeftDist-pDB->m_dHunchArray[0]), toM(A[0].x - dLeftDist)
									,  -toM(pDB->m_dThickSlabLeftExt)*dUnitConc, -toM(dSlabThick)*dUnitConc, 0, "슬래브");
				}
				else
				{
					AddTrapForceAtLeft(pFEM, 0, toM(A[0].x-dLeftDist)
									,  -toM(dPreThick)*dUnitConc, -toM(dSlabThick)*dUnitConc, 0, "슬래브");

				}
				dBaseDist = A[0].x-dLeftDist;
				dPreDist  = A[0].x;	
				dPreThick = dSlabThick;
			}		
			long nHunchEnd = 6;
			if(nG==pDB->GetGirdersu()-1) nHunchEnd = 3;
			for(long n=1; n<=nHunchEnd; n++)
			{
				if(n==1 || n==2)	continue;

				// 도로중심 추가						
				if(dPreDist<0 && A[n].x>0)
				{
					dSlabThick = fabs(pDB->GetThickSlabAndPaveByDis(pBxMatch,0)) - pDB->m_dThickPave;
					AddTrapForceAtLeft(pFEM, toM(dBaseDist), toM(dBaseDist - dPreDist),
									  -toM(dPreThick) * dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");
					dBaseDist += - dPreDist;
					dPreThick = dSlabThick;
					dPreDist = 0;
				}						
				dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[n].x) - A[n].y) - pDB->m_dThickPave;


				if(dPreDist != A[n].x)
				{					
					AddTrapForceAtLeft(pFEM, toM(dBaseDist), toM(dBaseDist + (A[n].x - dPreDist)),
									  -toM(dPreThick) * dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");

					dBaseDist += (A[n].x - dPreDist);
					dPreThick = dSlabThick;
				}
				dPreDist = A[n].x;
			}					
			if(nG==pDB->GetGirdersu()-1) //우측캔틸레버 사하중
			{
				dTaperThick = pGir->GetHeightTaper(pBxMatch->GetStation());
				dPreThick  = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[3].x) - A[3].y) - pDB->m_dThickPave;
				dSlabThick = pDB->m_dThickSlabRight + dTaperThick;
				double dRightCatil = dRightDist-A[3].x;
				if(pDB->m_dHunchArray.GetSize() > 0)
				{
					AddTrapForceAtLeft(pFEM,  toM(pDB->GetWidthSlabDom(pBx)-dRightCatil), toM(pDB->GetWidthSlabDom(pBx)-dRightCatil+pDB->m_dHunchArray[pDB->m_dHunchArray.GetSize()-1])
									,  -toM(dPreThick) * dUnitConc, -toM(pDB->m_dThickSlabRightExt) * dUnitConc, 0, "슬래브");
					AddTrapForceAtLeft(pFEM,  toM(pDB->GetWidthSlabDom(pBx)-dRightCatil+pDB->m_dHunchArray[pDB->m_dHunchArray.GetSize()-1]), toM(pDB->GetWidthSlabDom(pBx))
									,  -toM(pDB->m_dThickSlabRightExt)*dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");
				}
				else
					AddTrapForceAtLeft(pFEM,  toM(pDB->GetWidthSlabDom(pBx)-dRightCatil), toM(pDB->GetWidthSlabDom(pBx)),
									   -toM(dPreThick) * dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");
			}
		}		
	}
	else
	{
		long nG = 0;
		for(nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);						
			CPlateBasicIndex *pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGir);
			if(pBxMatch==NULL)	continue;
			
			pGir->GetHunchXyDom(pBxMatch, A, FALSE, FALSE, TRUE);
			if(nG==0) //좌측캔틸레버 사하중
			{			
				dPreThick  = pDB->m_dThickSlabLeft;		
				dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[0].x) - A[0].y)
							- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));
				
				AddTrapForceAtLeft(pFEM, 0, toM(A[0].x-dLeftDist)
									,  -toM(dPreThick)*dUnitConc, -toM(dSlabThick)*dUnitConc, 0, "슬래브");
				
				dBaseDist = A[0].x-dLeftDist;
				dPreDist  = A[0].x;	
				dPreThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[1].x) - A[1].y)
							- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));
			}	
			
			dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[2].x) - A[2].y)
						- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));

			AddTrapForceAtLeft(pFEM, toM(dBaseDist), toM(dBaseDist + A[2].x - dPreDist),
								  -toM(dPreThick) * dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");

			dBaseDist += A[3].x-dPreDist;
			dPreDist  = A[3].x;	
			dPreThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[3].x) - A[3].y)
						- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));

			if(nG!=pDB->GetGirdersu()-1)
			{
				if(dPreDist<0 && A[6].x>0)
				{
					dSlabThick = fabs(pDB->GetThickSlabAndPaveByDis(pBxMatch,0))
							- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));
					AddTrapForceAtLeft(pFEM, toM(dBaseDist), toM(dBaseDist - dPreDist),
									  -toM(dPreThick) * dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");
					dBaseDist += - dPreDist;
					dPreThick = dSlabThick;
					dPreDist = 0;
				}			
				
				dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[6].x) - A[6].y)
							- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));

				AddTrapForceAtLeft(pFEM, toM(dBaseDist), toM(dBaseDist + A[6].x - dPreDist),
									  -toM(dPreThick) * dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");

				dBaseDist += A[6].x-dPreDist;
				dPreDist  = A[6].x;	
				dPreThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[7].x) - A[7].y)
							- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));
			}
			else
			{
				dPreThick  = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[3].x) - A[3].y)
							- (pDB->m_dThickPave+pGir->GetHeightTaper(pBxMatch->GetStation()));
				dSlabThick = pDB->m_dThickSlabRight;		
				double dRightCatil = dRightDist-A[3].x;
				
				AddTrapForceAtLeft(pFEM,  toM(pDB->GetWidthSlabDom(pBx)-dRightCatil), toM(pDB->GetWidthSlabDom(pBx)),
								   -toM(dPreThick) * dUnitConc, -toM(dSlabThick) * dUnitConc, 0, "슬래브");
			}
		}

		pFEM->m_szLCName.Add("BEF_TU");	
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		nLoadCaseIndex++;

		double	dWidthBindConc	= toM(pDB->m_BindConc.m_dWidth);
		double	dHeightBindConc	= toM(pDB->m_BindConc.m_dHeight);
		double	dForceBindConc	= dWidthBindConc * dHeightBindConc * dUnitConc;

		for(nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			double	dDisGir	= ABS(dLeftDist - pGir->m_dGirderCenter);
			sText.Format("구속콘크리트(G%d)", nG+1);

			AddPointForceAtLeft(pFEM, toM(dDisGir), CVector(0,0,-dForceBindConc), 1, sText);
		}
	}
}

BOOL CFemHLLCalc::IsLeft(CPlateBasicIndex *pBx, double dDist)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	double	dWidthBr		= toM(pDB->GetWidthSlabDom(pBx));
	if(dDist>dWidthBr/2)	return FALSE;
	else					return TRUE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
/// Step 2 합성후 사하중
//////////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeForcePostDeadLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CCalcData		*pData		= m_pDataManage->GetCalcData();
	CSteelUWeight	*pUWeight	= m_pDataManage->GetSteelUWeight();	

	double	dUnitAsphalt	= tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dAsphalt);
	double	dForceAsp		= toM(pDB->m_dThickPave) * dUnitAsphalt;//포장

	double dDisStt=0, dDisEnd=0, dDist=0, dForceGuard=0;

	// 분포하중(보도하중)
	long	nType		= -1;		// 횡단타입
	long	nHDanNode	= pDB->GetQtyHDanNode();
	CString	szName		= _T("");
	BOOL	bFirst		= TRUE;
	long	nFloorIdx	= -1;
	long	nGuardCnt	= 0;
	double	dSttLen		= 0;
	double	dEndLen		= 0;

	vector <PEOPLELOAD> vArrPeople;
	for(long nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		long	nType	= pDB->GetValueTypeHDan(nHDan, 1);
		double	dLen	= toM(pDB->GetLengthHDanDom(pBx, nHDan));
		double	dWidth	= toM(pDB->GetWidthSlabDom(pBx));
		BOOL	bLeft	= FALSE;
		dEndLen += dLen;
		
		if(Round(dSttLen,5) < Round(dWidth/2.0,5))
		{
			bLeft		= TRUE;
			nFloorIdx	= 0;
		}
		else
		{
			bLeft		= FALSE;
			nFloorIdx	= 1;
		}
		if(nType==HT_PEOPLEROAD && pData->DESIGN_FLOOR_DATA[nFloorIdx].m_bWalkLoad)
		{
			struct PEOPLELOAD vPeople;
			vPeople.m_nType		= nFloorIdx;
			vPeople.m_bLeft		= bLeft;
			vPeople.m_dSttLen	= dSttLen;
			vPeople.m_dEndLen	= dEndLen;
			vArrPeople.push_back(vPeople);
		}
		
		CCentSeparation *pGuard = NULL;
		if(pDB->IsTypeGuardFenceDom(nHDan))
		{
			pGuard = pDB->GetGuardFence(nGuardCnt);
			nGuardCnt++;
		}
		
		if(!pGuard)
		{
			dSttLen += dLen;
			continue;
		}
		
		if(nType == HT_PEOPLEROAD)
		{
			double	dStt	= dSttLen + toM(pGuard->m_D1);
			double	dEnd	= dEndLen;

			struct PEOPLELOAD vPeople;
			vPeople.m_nType		= nType;
			vPeople.m_bLeft		= bLeft;
			vPeople.m_dSttLen	= dStt;
			vPeople.m_dEndLen	= dEnd;
			vArrPeople.push_back(vPeople);
		}
		
		dSttLen += dLen;
	}

	if(vArrPeople.size()>0)
	{
		long k = 0;
		for(k=0; k<(long)vArrPeople.size()-1; k++)
		{
			PEOPLELOAD *pPeoplePre	= &vArrPeople.at(k);
			PEOPLELOAD *pPeopleNext	= &vArrPeople.at(k+1);
			
			if(pPeoplePre->m_dEndLen==pPeopleNext->m_dSttLen && pPeoplePre->m_bLeft==pPeopleNext->m_bLeft)
			{
				pPeopleNext->m_dSttLen	= pPeoplePre->m_dSttLen;
				vArrPeople.erase(vArrPeople.begin()+k);
				k--;
			}
		}

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, "WALK");
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "보도");
		pFEM->AddCombo("AFTER", "WALK", CCombo::ADD, CCombo::LOAD, 1, FALSE);

		for(k=0; k<(long)vArrPeople.size(); k++)
		{
			PEOPLELOAD People = vArrPeople.at(k);	
			long	nType	= People.m_nType;
			double	dSttLen	= People.m_dSttLen;
			double	dEndLen	= People.m_dEndLen;
			double	dForceWalkLoad1 = tokNPM2(pData->DESIGN_FLOOR_DATA[nType].m_dWalkLoad1);
 			double	dForceWalkLoad2 = tokNPM2(pData->DESIGN_FLOOR_DATA[nType].m_dWalkLoad2);
			AddTrapForceAtLeft(pFEM, dSttLen, dEndLen, -dForceWalkLoad1, -dForceWalkLoad2, nLoadCaseIndex, "보도하중");
		}
		nLoadCaseIndex++;
	}
// 
// 	for(long nH=0; nH<nHDanNode; nH++)
// 	{
// 		nType    = pDB->GetValueTypeHDan(nH, 1);
// 		dDisStt  = dDisEnd;
// 		dDisEnd  = dDisStt + toM(pDB->GetLengthHDanDom(pBx, nH));
// 
// 		if(nType==HT_PEOPLEROAD)
// 		{
// 			double dForceWalkLoad1 = 0;
// 			double dForceWalkLoad2 = 0;
// 			if(IsLeft(pBx, dDisEnd))
// 			{
// 				dForceWalkLoad1 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1);
// 				dForceWalkLoad2 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2);
// 			}
// 			else
// 			{
// 				dForceWalkLoad1 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2);
// 				dForceWalkLoad2 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1);
// 			}
// 			if(bFirst)
// 			{
// 				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
// 				pFEM->m_szLCName.SetAt(nLoadCaseIndex, "WALK");
// 				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "보도");
// 				pFEM->AddCombo("AFTER", "WALK", CCombo::ADD, CCombo::LOAD, 1, FALSE);
// 				bFirst = FALSE;
// 			}
// 			AddTrapForceAtLeft(pFEM, dDisStt, dDisEnd, -dForceWalkLoad1, -dForceWalkLoad2, nLoadCaseIndex, "보도하중");
// 		}
// 	}
//	if(!bFirst)	nLoadCaseIndex++;

	pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, "PAVE");	
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "포장");
	pFEM->AddCombo("AFTER", "PAVE", CCombo::ADD, CCombo::LOAD, 1, FALSE);
	dDisStt	 = 0;
	dDisEnd	= 0;
	long nH = 0;
	for(nH=0; nH<nHDanNode; nH++)
	{
		nType    = pDB->GetValueTypeHDan(nH, 1);
		dDisStt  = dDisEnd;
		dDisEnd  = dDisStt + toM(pDB->GetLengthHDanDom(pBx, nH));
		
//		if(nH==0 || nType==HT_GUARDFENCE || nType==HT_CENTERGUARDFENCE)
//			continue;
		if(nType==HT_NONE)
			AddTrapForceAtLeft(pFEM, dDisStt, dDisEnd, -dForceAsp, -dForceAsp, nLoadCaseIndex, "포장하중");
	}
	nLoadCaseIndex++;

	dDisStt	= 0;
	dDisEnd	= 0;

	long	nGuardIdx	= 0;
	double	dArea		= 0;
	double	dUnitWeight	= 0;
	CString	szGuard		= _T("");
	bFirst = TRUE;
	for(nH=0; nH<nHDanNode; nH++)
	{
		nType    = pDB->GetValueTypeHDan(nH, 1);

		if(nType==HT_NONE || nType==HT_PEOPLEROAD)
		{
			dDisStt  = dDisEnd;
			dDisEnd  = dDisStt + toM(pDB->GetLengthHDanDom(pBx, nH));
			continue;
		}
		CCentSeparation* pSep = pDB->GetGuardFence(nGuardIdx);
		if(pSep)
		{
			if(nType==HT_GUARDFENCE && pData->m_bMugun)						dUnitWeight	= pUWeight->m_dUWeightMugunCon;
			else if(nType==HT_GUARDFENCE && !pData->m_bMugun)				dUnitWeight	= pUWeight->m_dUWeightChulgunCon;
			else if(nType==HT_CENTERGUARDFENCE && pData->m_bJungbunMugun)	dUnitWeight	= pUWeight->m_dUWeightMugunCon;
			else if(nType==HT_CENTERGUARDFENCE && !pData->m_bJungbunMugun)	dUnitWeight	= pUWeight->m_dUWeightChulgunCon;
			dArea		= pSep->GetArea();
			dForceGuard	= dArea * dUnitWeight;
			dDist		= dDisEnd + toM(pSep->GetDistCentroid());
			szGuard.Format("방호벽자중-%d", nGuardIdx+1);

			if(bFirst)
			{
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, "GUARD");
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "방호벽");
				pFEM->AddCombo("AFTER", "GUARD", CCombo::ADD, CCombo::LOAD, 1, FALSE);
				bFirst = FALSE;
			}
			AddPointForceAtLeft(pFEM, dDist, CVector(0,0,-dForceGuard), nLoadCaseIndex, szGuard);
			nGuardIdx++;
		}
		dDisStt  = dDisEnd;
		dDisEnd  = dDisStt + toM(pDB->GetLengthHDanDom(pBx, nH));
	}
	if(!bFirst)
		nLoadCaseIndex++;

	double dForce=0;
	//////////////////////////////////////////////////////////////////////////////
	//방음벽(좌측)
	if(pData->DESIGN_FLOOR_DATA[0].m_bBangEm)
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, "SOUNDL");	
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "방음벽(좌측)");
		pFEM->AddCombo("AFTER", "SOUNDL", CCombo::ADD, CCombo::LOAD, 1, FALSE);

		CElement   *pE     = pFEM->GetElement(0);
		CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);				

		dForce = tokN(pData->DESIGN_FLOOR_DATA[0].m_dBangEmWeight);
		dDist  = toM(pData->DESIGN_FLOOR_DATA[0].m_dBangEmDis);
		pELoad->SetDistForce(dDist, CVector(0,0,-dForce), "SOUNDL", FALSE, "");
		nLoadCaseIndex++;
	}
	//////////////////////////////////////////////////////////////////////////////
	//방음벽(우측)
	if(pData->DESIGN_FLOOR_DATA[1].m_bBangEm)
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, "SOUNDR");	
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "방음벽(우측)");
		pFEM->AddCombo("AFTER", "SOUNDR", CCombo::ADD, CCombo::LOAD, 1, FALSE);

		CElement   *pE     = pFEM->GetElement((unsigned short)pFEM->GetElementSize()-1);
		CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);				
		CVector vStt = pFEM->GetJoint(pE->m_nJStt)->m_vPoint;
		CVector vEnd = pFEM->GetJoint(pE->m_nJEnd)->m_vPoint;

		dForce = tokN(pData->DESIGN_FLOOR_DATA[1].m_dBangEmWeight);
		dDist  = ~(vEnd-vStt)-toM(pData->DESIGN_FLOOR_DATA[1].m_dBangEmDis);
		pELoad->SetDistForce(dDist, CVector(0,0,-dForce), "SOUNDR", FALSE, "");
		nLoadCaseIndex++;
	}
	//////////////////////////////////////////////////////////////////////////////
	//난간(좌측)
	if(pData->DESIGN_FLOOR_DATA[0].m_bNanGan)
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, "HANDL");	
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "난간");
		pFEM->AddCombo("AFTER", "HANDL", CCombo::ADD, CCombo::LOAD, 1, FALSE);

		CElement   *pE     = pFEM->GetElement(0);
		CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);

		dForce = tokN(pData->DESIGN_FLOOR_DATA[0].m_dNanGanWeight);
		dDist  = toM(pData->DESIGN_FLOOR_DATA[0].m_dNanGanDis);
		pELoad->SetDistForce(dDist, CVector(0,0,-dForce), "HANDL", FALSE, "");
		nLoadCaseIndex++;
	}
	//////////////////////////////////////////////////////////////////////////////
	//난간(우측)
    if(pData->DESIGN_FLOOR_DATA[1].m_bNanGan)
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, "HANDR");	
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "난간");
		pFEM->AddCombo("AFTER", "HANDR", CCombo::ADD, CCombo::LOAD, 1, FALSE);

		CElement   *pE     = pFEM->GetElement((unsigned short)pFEM->GetElementSize()-1);
		CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);				

		CVector vStt = pFEM->GetJoint(pE->m_nJStt)->m_vPoint;
		CVector vEnd = pFEM->GetJoint(pE->m_nJEnd)->m_vPoint;
		dForce = tokN(pData->DESIGN_FLOOR_DATA[1].m_dNanGanWeight);
		dDist  = ~(vStt-vEnd)-toM(pData->DESIGN_FLOOR_DATA[1].m_dNanGanDis);
		pELoad->SetDistForce(dDist, CVector(0,0,-dForce), "HANDR", FALSE, "");
		nLoadCaseIndex++;
	}
	//////////////////////////////////////////////////////////////////////////////
	//기타하중
	if(pData->m_ELoadWeightArr.GetSize()>0 && pData->m_bExtraLoad)
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, "EXTRA");	
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "기타");
		pFEM->AddCombo("AFTER", "EXTRA", CCombo::ADD, CCombo::LOAD, 1, FALSE);

		for(long n=0; n<pData->m_ELoadWeightArr.GetSize(); n++)
		{
			dDist  = toM(pData->m_ELoadDisArr.GetAt(n));
			dForce = tokN(pData->m_ELoadWeightArr.GetAt(n));
			AddPointForceAtLeft(pFEM, dDist, CVector(0,0,-dForce), nLoadCaseIndex, "EXTRA");
		}
		nLoadCaseIndex++;
	}
}

//////////////////////////////////////////////////////////////////////
/// Step 3 활하중
/// 거더별 최대를 구하기 위해 구현중 좌우측으로 변경됨(이종원,심동윤)
//////////////////////////////////////////////////////////////////////
void CFemHLLCalc::MakeForceLiveLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nHL, long &nLoadCase, LIVETYPE Type)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();

	long	nG			= 0;
	long	nQtyLane	= 0;
	long	nHDanType	= 0;
	long	nHDanNode	= pDB->GetQtyHDanNode();
	long	nLaneCount	= 1;
	BOOL	bStt		= FALSE;
	BOOL	bInsertPre	= FALSE;
	double	dSttHDan	= 0;
	double	dEndHDan	= 0;
	double	dSttRoadway	= 0;
	double	dEndRoadway	= 0;
	double	dLenHDanDom	= 0;
	double	dEffWidth	= 0;
	CString	szName		= Type == DB ? _T("DB") : _T("DL");

	pData->m_DBWidth	= 1.8;               // DB 차폭
	pData->m_DBDis		= 1.2;               // DB 차간   거리
	pData->m_DLDis		= 3.0;               // DL 차선간 거리

	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
	CCalcGeneral		CalGeneral(m_pDataManage);
	CHLLLane HLLLane;

	dSttHDan	= 0;
	dEndHDan	= 0;
	dSttRoadway	= 0;
	dEndRoadway	= 0;
	nLaneCount	= 1;
	bInsertPre	= FALSE;

	long nLaneTemp = 0;
	long nHDan = 0;
	for(nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBxMatch, nHDan));
		dEndHDan	+= dLenHDanDom;
		if(!bStt && nHDanType==HT_NONE)
		{
			dSttRoadway	= dSttHDan;
			bStt		= TRUE;
		}
		long nInsCount  = 0;
		if((bStt && nHDanType!=HT_NONE) || (nHDan==nHDanNode-1 && nHDanType==HT_NONE))
		{
			if(nHDanType==HT_NONE)	dEndRoadway	= dEndHDan;
			else					dEndRoadway	= dEndHDan-dLenHDanDom;
			dEffWidth	= fabs(dEndRoadway-dSttRoadway);
			nQtyLane	= CalGeneral.GetQtyLane(dEffWidth);
			bStt		= FALSE;

			for(long nLane=0; nLane<nQtyLane; nLane++)
			{
				CHLLLaneUnit HLLLaneUnit;
				HLLLaneUnit.m_nLaneType	= Type;
				HLLLaneUnit.m_szLCName.Format("%sL-%d", szName, nLaneCount);
				HLLLaneUnit.m_szDescription.Format("%sL %d차선 재하시", szName, nLaneCount++);

				pFEM->m_szLCName.Add(HLLLaneUnit.m_szLCName);
				pFEM->MakeElementLoadForce(nLoadCase+1);

				if(SetDBDLLoad(pFEM, pBx, nG, dSttRoadway, dEndRoadway, nLoadCase, &HLLLaneUnit, nLane+1, Type))
				{
					if(bInsertPre)
						InsertPreLane(pFEM, &HLLLane, &HLLLaneUnit, nLaneTemp-1, nLoadCase, Type);
					HLLLane.m_vLaneUnit.push_back(HLLLaneUnit);

					nLoadCase++;
					nInsCount++;
				}
			}
			bInsertPre	= TRUE;
			nLaneTemp	+= nInsCount;
		}
		dSttHDan = dEndHDan;
	}

	HLLLane.m_vLaneUnit.clear();
	dSttHDan	= 0;
	dEndHDan	= 0;
	dSttRoadway	= 0;
	dEndRoadway	= 0;
	nLaneCount	= 1;
	bInsertPre	= FALSE;
	nLaneTemp	= 0;
	nG			= pDB->GetQtyGirder()-1;

	pGir		= pDB->GetGirder(nG);
	pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);

	double	dSlabWidth	= pDB->GetWidthSlabDom(pBxMatch);
	dSttHDan	= toM(dSlabWidth);
	dEndHDan	= toM(dSlabWidth);

	for(nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDanNode-nHDan-1, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBxMatch, nHDanNode-nHDan-1));
		dSttHDan	-= dLenHDanDom;
		if(!bStt && nHDanType==HT_NONE)
		{
			dEndRoadway	= dEndHDan;
			bStt		= TRUE;
		}

		long nInsCount  = 0;
		if((bStt && nHDanType!=HT_NONE) || (nHDan==nHDanNode-1 && nHDanType==HT_NONE))
		{
			dSttRoadway	= dEndHDan;
			dEffWidth	= fabs(dEndRoadway-dSttRoadway);
			nQtyLane	= CalGeneral.GetQtyLane(dEffWidth);
			bStt		= FALSE;

			for(long nLane=0; nLane<nQtyLane; nLane++)
			{
				CHLLLaneUnit HLLLaneUnit;
				HLLLaneUnit.m_nLaneType	= Type;
				HLLLaneUnit.m_szLCName.Format("%sR-%d", szName, nLaneCount);
				HLLLaneUnit.m_szDescription.Format("%sR %d차선 재하시", szName, nLaneCount++);

				pFEM->m_szLCName.Add(HLLLaneUnit.m_szLCName);
				pFEM->MakeElementLoadForce(nLoadCase+1);

				if(SetDBDLLoad(pFEM, pBx, nG, dSttRoadway, dEndRoadway, nLoadCase, &HLLLaneUnit, nLane+1, Type))
				{
					if(bInsertPre)
						InsertPreLane(pFEM, &HLLLane, &HLLLaneUnit, nLaneTemp-1, nLoadCase, Type);

					HLLLane.m_vLaneUnit.push_back(HLLLaneUnit);
					nLoadCase++;
					nInsCount++;
				}
			}
			bInsertPre	= TRUE;
			nLaneTemp	+= nInsCount;
		}
		dEndHDan = dSttHDan;
	}
}

void CFemHLLCalc::InsertPreLane(CFEMManage *pFEM, CHLLLane *pHLLLane, CHLLLaneUnit *pHLLLaneUnit, long nLane, long nLoadCase, LIVETYPE Type)
{
	if(pHLLLane->m_vLaneUnit.size()==0)	return;

	CHLLLaneUnit HLLLaneUnitLast = pHLLLane->m_vLaneUnit.at(nLane);

	for(long n=0; n<(long)HLLLaneUnitLast.m_vLaneElem.size(); n++)
	{
		CHLLLaneElement Elem1	= HLLLaneUnitLast.m_vLaneElem.at(n);
		CHLLLaneElement Elem2(Elem1);
		pHLLLaneUnit->m_vLaneElem.push_back(Elem2);
		if(Type==DB)
		{
			AddUnitPointForceAtLeft(pFEM, Elem2.m_dSttHDanPos, nLoadCase);
			AddUnitPointForceAtLeft(pFEM, Elem2.m_dEndHDanPos, nLoadCase);
		}
		else
		{
			AddUnitTrapForceAtLeft(pFEM, Elem2.m_dSttHDanPos, Elem2.m_dEndHDanPos, nLoadCase);
		}
	}
}

BOOL CFemHLLCalc::SetDBDLLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nG, double dSttRoadway, double dEndRoadway, long &nLoadCase, CHLLLaneUnit *pHLLLaneUnit, long nQtyLane, LIVETYPE Type)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pData		= m_pDataManage->GetCalcData();
	CCalcGeneral		CalceGeneral(m_pDataManage);

	double	dDBWidth			= Type==DB ? pData->m_DBWidth : pData->m_DLDis;
	double	dEdge				= Type==DB ? pData->m_DBFromEdge : 0;
	double	dLaneWidth			= dEndRoadway-dSttRoadway;
	long	nQtyDesignLane		= CalceGeneral.GetQtyLane(dLaneWidth);
	double	dDesignLaneWidth	= MIN(Round(dLaneWidth/nQtyDesignLane,3), 3.6);

	if(pData->m_nHLiveLoadType==0)
		dDesignLaneWidth = 3.0;
	if(pData->m_nHLiveLoadType==2)
		dDesignLaneWidth = pData->m_dUserLaneWidth;

	if(nQtyDesignLane==0)
		return FALSE;

	CHLLLaneUnit LaneUnit;

	for(long nLane=0; nLane<nQtyLane; nLane++)
	{
		CHLLLaneElement	HLLLaneElem;

		if(nLane==0)
		{
			if(nG==0)
			{
				HLLLaneElem.m_dSttHDanPos = dSttRoadway+dEdge;
				HLLLaneElem.m_dEndHDanPos = HLLLaneElem.m_dSttHDanPos+dDBWidth;
			}
			else
			{
				HLLLaneElem.m_dEndHDanPos = dEndRoadway-dEdge;
				HLLLaneElem.m_dSttHDanPos = HLLLaneElem.m_dEndHDanPos-dDBWidth;
			}
		}
		else
		{
			CHLLLaneElement	HLLLaneElemLeft		= LaneUnit.GetElementSide(TRUE);
			CHLLLaneElement	HLLLaneElemRight	= LaneUnit.GetElementSide(FALSE);
 
			if(HLLLaneElemLeft.m_dSttHDanPos-dSttRoadway>dEdge+dDBWidth+(dDesignLaneWidth-dDBWidth))
			{
				HLLLaneElem.m_dEndHDanPos	= HLLLaneElemLeft.m_dSttHDanPos-(dDesignLaneWidth-dDBWidth);
				HLLLaneElem.m_dSttHDanPos	= HLLLaneElem.m_dEndHDanPos-dDBWidth;
			}

			if(dEndRoadway-HLLLaneElemRight.m_dEndHDanPos>dEdge+dDBWidth+(dDesignLaneWidth-dDBWidth))
			{
				HLLLaneElem.m_dSttHDanPos	= HLLLaneElemRight.m_dEndHDanPos+(dDesignLaneWidth-dDBWidth);
				HLLLaneElem.m_dEndHDanPos	= HLLLaneElem.m_dSttHDanPos+dDBWidth;
			}
		}

		if(HLLLaneElem.m_dEndHDanPos>0)
			LaneUnit.m_vLaneElem.push_back(HLLLaneElem);
	}

	if(LaneUnit.m_vLaneElem.size()==nQtyLane)
	{
		for(long n=0; n<(long)LaneUnit.m_vLaneElem.size(); n++)
		{
			CHLLLaneElement	HLLLaneElem(LaneUnit.m_vLaneElem.at(n));

			pHLLLaneUnit->m_vLaneElem.push_back(HLLLaneElem);
			if(Type==DB)
			{
				AddUnitPointForceAtLeft(pFEM, HLLLaneElem.m_dSttHDanPos, nLoadCase);
				AddUnitPointForceAtLeft(pFEM, HLLLaneElem.m_dEndHDanPos, nLoadCase);
			}
			else
			{
				AddUnitTrapForceAtLeft(pFEM, HLLLaneElem.m_dSttHDanPos, HLLLaneElem.m_dEndHDanPos, nLoadCase);
			}
		}

		return TRUE;
	}
	return FALSE;
}

void CFemHLLCalc::MakeForceLivePeople(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCase)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	//군중하중
	long	nIdx = 0;
	long   nType=0;
	double dSumDis=0, dSttGugan=0, dEndGugan=0, dDisStt=0;	
	CString szName("");

	for(long nHdan=0; nHdan<pDB->GetQtyHDanNode(); nHdan++)
	{
		nType	= pDB->GetValueTypeHDan(nHdan, 1);
		dDisStt	= toM(pDB->GetLengthHDanDom(pBx, nHdan));
		dSumDis	+= dDisStt;
		dSttGugan = dSumDis-dDisStt;
		dEndGugan = dSumDis;

		if(nType==HT_PEOPLEROAD) //보도
		{
			double dL = 3.5;

			szName.Format("군중하중-%d", nIdx+1);
//			pFEM->m_szTitle.Add(szName);
			szName.Format("PEOPLE-%d", nIdx+1);
			pFEM->m_szLCName.Add(szName);
			pFEM->MakeElementLoadForce(nLoadCase+1);
//			pFEM->AddCombo(szName, szName, CCombo::ADD, CCombo::LOAD, 1, FALSE);

			AddTrapForceAtLeft(pFEM, dSttGugan, dEndGugan, -dL , -dL, nLoadCase);
			nLoadCase++;
			nIdx++;
		}	
	}
}

void CFemHLLCalc::MakeForceLiveTank(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nHL, long &nLoadCase)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();

	if(!pData->m_bTank)	return;

	double	dLenHDanDom		= 0;
	double	dLenHDanCurrent	= 0;
	double	dLenHDanBefore	= 0;
	double	dSttRoadway		= 0;
	double	dEndRoadway		= 0;
	long	nHDanType		= 0;
	long	nIndex			= 0;
	BOOL	isBeforeRoad	= FALSE;
	BOOL	isCurrentRoad	= FALSE;

	// 2015. 04. 09 김형준, #APLATE-668, 슬래브 제원 상황에 따라 탱크 하중이 바뀌도록 수정한다.
	// 여기에서 "도로" 는 CentSeparaion.h 의 HT_NONE를 의미한다.

	// 1) 이전 단(dan)도 도로가 아니고 + 지금 도로가 아닌 경우 :: X -> X, 아무것도 하지 않는다.
	// 2) 이전 단(dan)도 도로가 아니고 + 지금 도로 위에 있다면 :: X -> O, Start 지점과 End 지점을 저장한다.
	// 3) 이전 단(dan)이 도로이고	   + 지금 도로가 아닌 경우 :: O -> X, Start 지점과 End 지점으로 하중을 등록한다.
	// 4) 이전 단(dan)이 도로이고	   + 지금 도로 위에 있다면 :: O -> O, End 지점을 갱신한다.

	long nQtyHDanNode = pDB->GetQtyHDanNode();
	for(long nHDan = 0; nHDan < nQtyHDanNode; nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBx, nHDan));

		dLenHDanBefore = dLenHDanCurrent;
		dLenHDanCurrent += dLenHDanDom;

		isCurrentRoad = (nHDanType == HT_NONE);

		if(isBeforeRoad == FALSE && isCurrentRoad == TRUE) // 2번 상황
		{
			dSttRoadway = dLenHDanBefore;
			dEndRoadway = dLenHDanCurrent;
		}
		else if(isBeforeRoad == TRUE && isCurrentRoad == FALSE) // 3번 상황
		{
			AddUnitPointTankOrTrailor(pFEM, TRUE, dSttRoadway, dEndRoadway, nIndex++, nLoadCase++);
		}
		else if(isBeforeRoad == TRUE && isCurrentRoad == TRUE) // 4번 상황
		{
			dEndRoadway = dLenHDanCurrent;
		}

		// 단, 오른쪽 끝에서 도로가 끝난 경우에는 예외처리로 등록을 해주어야 한다.
		if(isCurrentRoad == TRUE && nHDan == nQtyHDanNode - 1)
			AddUnitPointTankOrTrailor(pFEM, TRUE, dSttRoadway, dEndRoadway, nIndex++, nLoadCase++);

		isBeforeRoad = isCurrentRoad;
	}
}

void CFemHLLCalc::MakeForceLiveTrailer(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nHL, long &nLoadCase)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();

	if(!pData->m_bTrailer)	return;

	double	dLenHDanDom		= 0;
	double	dLenHDanCurrent	= 0;
	double	dLenHDanBefore	= 0;
	double	dSttRoadway		= 0;
	double	dEndRoadway		= 0;
	long	nHDanType		= 0;
	long	nIndex			= 0;
	BOOL	isBeforeRoad	= FALSE;
	BOOL	isCurrentRoad	= FALSE;

	// 2015. 04. 09 김형준, #APLATE-668, 슬래브 제원 상황에 따라 트레일러 하중이 바뀌도록 수정한다.
	// 여기에서 "도로" 는 CentSeparaion.h 의 HT_NONE를 의미한다.

	// 1) 이전 단(dan)도 도로가 아니고 + 지금 도로가 아닌 경우 :: X -> X, 아무것도 하지 않는다.
	// 2) 이전 단(dan)도 도로가 아니고 + 지금 도로 위에 있다면 :: X -> O, Start 지점과 End 지점을 저장한다.
	// 3) 이전 단(dan)이 도로이고	   + 지금 도로가 아닌 경우 :: O -> X, Start 지점과 End 지점으로 하중을 등록한다.
	// 4) 이전 단(dan)이 도로이고	   + 지금 도로 위에 있다면 :: O -> O, End 지점을 갱신한다.

	long nQtyHDanNode = pDB->GetQtyHDanNode();
	for(long nHDan = 0; nHDan < nQtyHDanNode; nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBx, nHDan));

		dLenHDanBefore = dLenHDanCurrent;
		dLenHDanCurrent += dLenHDanDom;

		isCurrentRoad = (nHDanType == HT_NONE);

		if(isBeforeRoad == FALSE && isCurrentRoad == TRUE) // 2번 상황
		{
			dSttRoadway = dLenHDanBefore;
			dEndRoadway = dLenHDanCurrent;
		}
		else if(isBeforeRoad == TRUE && isCurrentRoad == FALSE) // 3번 상황
		{
			AddUnitPointTankOrTrailor(pFEM, FALSE, dSttRoadway, dEndRoadway, nIndex++, nLoadCase++);
		}
		else if(isBeforeRoad == TRUE && isCurrentRoad == TRUE) // 4번 상황
		{
			dEndRoadway = dLenHDanCurrent;
		}

		// 단, 오른쪽 끝에서 도로가 끝난 경우에는 예외처리로 등록을 해주어야 한다.
		if(isCurrentRoad == TRUE && nHDan == nQtyHDanNode - 1)
			AddUnitPointTankOrTrailor(pFEM, FALSE, dSttRoadway, dEndRoadway, nIndex++, nLoadCase++);

		isBeforeRoad = isCurrentRoad;
	}
}

void CFemHLLCalc::MakeForceWindLoad(CFEMManage *pFEM, long nJijum)
{
	CCalcData			*pData	= m_pDataManage->GetCalcData();

	SetWindLoadForce(nJijum);

	long	nLoadCase	= 0;
	long	nApplyElem	= GetLoadApplyMemberCount(pFEM);
	CString szLoadCaseTitle="";
	CJoint *pJointStt = pFEM->GetJoint(0);
	CJoint *pJointEnd = pFEM->GetJoint((unsigned short)nApplyElem);
	BOOL bBangEmL = pData->DESIGN_FLOOR_DATA[0].m_bBangEm;
	BOOL bBangEmR = pData->DESIGN_FLOOR_DATA[1].m_bBangEm;

	if(bBangEmL && !bBangEmR) ///< 좌측방음벽망
	{
		pFEM->m_szLCName.Add("LLWIND");	
		pFEM->MakeElementLoadForce(nLoadCase+1);
		nLoadCase++;
		CLoadForceJoint	*pLoadForceJoint1	= pJointStt->GetLoadForce(0);
		pLoadForceJoint1->SetLoadForce("LLWIND", CVector(pData->m_dLeftWindFroceFromL[nJijum], 0,0), CVector(0,pData->m_dLeftWindMomentFromL[nJijum],0));

		pFEM->m_szLCName.Add("LRWIND");	
		pFEM->MakeElementLoadForce(nLoadCase+1);
		CLoadForceJoint	*pLoadForceJoint2	= pJointStt->GetLoadForce(1);
		pLoadForceJoint2->SetLoadForce("LRWIND", CVector(pData->m_dLeftWindFroceFromR[nJijum], 0,0), CVector(0,pData->m_dLeftWindMomentFromR[nJijum],0));
	}
	else if(!bBangEmL && bBangEmR) ///< 우측방음벽망
	{
		pFEM->m_szLCName.Add("RRWIND");	
		pFEM->MakeElementLoadForce(nLoadCase+1);
		nLoadCase++;
		CLoadForceJoint *pLoadForceJoint1	= pJointEnd->GetLoadForce(0);
		pLoadForceJoint1->SetLoadForce("RRWIND", CVector(pData->m_dRightWindFroceFromR[nJijum], 0, 0), CVector(0, pData->m_dRightWindMomentFromR[nJijum], 0));

		pFEM->m_szLCName.Add("RLWIND");	
		pFEM->MakeElementLoadForce(nLoadCase+1);
		CLoadForceJoint *pLoadForceJoint2 = pJointEnd->GetLoadForce(1);
		pLoadForceJoint2->SetLoadForce("RLWIND", CVector(pData->m_dRightWindFroceFromL[nJijum], 0, 0), CVector(0, pData->m_dRightWindMomentFromL[nJijum], 0));
	}
	else if(bBangEmL && bBangEmR) ///< 양쪽 다
	{
		pFEM->m_szLCName.Add("LLWIND");	
		pFEM->MakeElementLoadForce(nLoadCase+1);
		nLoadCase++;
		CLoadForceJoint *pLoadForceJoint1 = pJointStt->GetLoadForce(0);
		pLoadForceJoint1->SetLoadForce("LLWIND", CVector(pData->m_dLeftWindFroceFromL[nJijum], 0, 0), CVector(0, -pData->m_dLeftWindMomentFromL[nJijum], 0));

		pFEM->m_szLCName.Add("RRWIND");	
		pFEM->MakeElementLoadForce(nLoadCase+1);
		CLoadForceJoint *pLoadForceJoint2 = pJointEnd->GetLoadForce(1);
		pLoadForceJoint2->SetLoadForce("RRWIND", CVector(pData->m_dRightWindFroceFromR[nJijum], 0, 0), CVector(0, -pData->m_dRightWindMomentFromR[nJijum], 0));
	}
}

void CFemHLLCalc::AddTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, double dForceStt, double dForceEnd, long nLoadCase, CString szLoadName)
{
	for(unsigned short n=0; n<pFEM->GetElementSize(); n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);

		if(dDistStt >= (pJ_I->m_vPoint.x) && dDistStt < (pJ_J->m_vPoint.x))
		{
			double dForceMiddle = dForceStt + (dForceEnd - dForceStt) / (dDistEnd - dDistStt) * min(dDistEnd - dDistStt, (pJ_J->m_vPoint.x) - dDistStt);
			if(dDistEnd <= (pJ_J->m_vPoint.x))
			{
				double dStt = dDistStt - (pJ_I->m_vPoint.x);
				double dEnd = dDistEnd - (pJ_I->m_vPoint.x);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dStt, dEnd, CVector(0,0,dForceStt), CVector(0,0,dForceEnd), CVector(0,0,0), CVector(0,0,0), szLoadName, CVector(0,0,0), FALSE);
//				pELoad->SetTrapForce(dStt, dEnd, CVector(0, 0, dForceStt),  CVector(0, 0, dForceEnd), szLoadName);		
				break;
			}		
			else
			{
				double dStt = dDistStt - (pJ_I->m_vPoint.x);
				double dEnd = (pJ_J->m_vPoint.x) - (pJ_I->m_vPoint.x);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dStt, dEnd, CVector(0,0,dForceStt), CVector(0,0,dForceMiddle), CVector(0,0,0), CVector(0,0,0), szLoadName, CVector(0,0,0), FALSE);
//				pELoad->SetLoadForce(dStt, dEnd, CVector(0,0,dForceStt), CVector(0,0,dForceEnd), CVector(0,0,0), CVector(0,0,0), szLoadName, CVector(0,0,0), FALSE);
//				pELoad->SetTrapForce(dStt, dEnd, CVector(0, 0, dForceStt),  CVector(0, 0, dForceMiddle), szLoadName);				
				dForceStt = dForceMiddle;
//				dForceStt = dForceEnd;
				dDistStt = (pJ_J->m_vPoint.x);			
			}			
		}
	}
}


void CFemHLLCalc::AddPointForceAtLeft(CFEMManage *pFEM, double dDist, CVector vForce, long nLoadCase, CString szLoadName)
{
	for(unsigned short n=0; n<pFEM->GetElementSize(); n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);
		if(dDist >= (pJ_I->m_vPoint.x) && dDist < (pJ_J->m_vPoint.x))
		{			
			double dDistLocal = dDist - (pJ_I->m_vPoint.x);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
			pELoad->SetDistForce(dDistLocal, vForce, szLoadName, FALSE, "");
//			pELoad->SetDistForce(dDistLocal, vForce, szLoadName);
			break;
		}
	}
}

BOOL CFemHLLCalc::AddUnitPointForceAtLeft(CFEMManage *pFEM, double dDist, long nLoadCase, CString szLoadName)
{
	long icElement = pFEM->GetElementSize();
	for(unsigned short n=0; n<pFEM->GetElementSize(); n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);
		if((dDist >= pJ_I->m_vPoint.x && dDist < pJ_J->m_vPoint.x) || (n == icElement-1 && pJ_J->m_vPoint.x == dDist))
		{			
			double dDistLocal = dDist - (pJ_I->m_vPoint.x);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
			pELoad->SetDistForce(dDistLocal, CVector(0,0,-1), szLoadName, FALSE, "");
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFemHLLCalc::AddUnitTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, long nLoadCase, CString szLoadName)
{
	for(unsigned short n=0; n<pFEM->GetElementSize(); n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);

		if(dDistStt >= (pJ_I->m_vPoint.x) && dDistStt < (pJ_J->m_vPoint.x))
		{			
			if(dDistEnd <= (pJ_J->m_vPoint.x))
			{
				double dStt = dDistStt - (pJ_I->m_vPoint.x);
				double dEnd = dDistEnd - (pJ_I->m_vPoint.x);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dStt, dEnd, CVector(0,0,-1), CVector(0,0,-1), CVector(0,0,0), CVector(0,0,0), szLoadName, CVector(0,0,0), FALSE);
//				pELoad->SetUnitTrapForce(dStt, dEnd, szLoadName);
			}		
			else
			{
				double dStt = dDistStt - (pJ_I->m_vPoint.x);
				double dEnd = (pJ_J->m_vPoint.x) - (pJ_I->m_vPoint.x);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dStt, dEnd, CVector(0,0,-1), CVector(0,0,-1), CVector(0,0,0), CVector(0,0,0), szLoadName, CVector(0,0,0), FALSE);
//				pELoad->SetUnitTrapForce(dStt, dEnd, szLoadName);								
				dDistStt = (pJ_J->m_vPoint.x);
			}			
		}
	}
	return FALSE;
}

long CFemHLLCalc::GetElemNoByDist(double dDist, CFEMManage *pFEM)
{
	long nElem=0;
	for(long n=0; n<pFEM->GetElementSize(); n++)
	{
		CElement *pE = pFEM->GetElement((unsigned short)n);
		CVector vStt = pFEM->GetJoint(pE->m_nJStt)->m_vPoint;
		CVector vEnd = pFEM->GetJoint(pE->m_nJEnd)->m_vPoint;
		if(vStt.x<=dDist && vEnd.x>dDist)		
			break;
		else
			nElem++;
	}
	
	if(nElem >= pFEM->GetElementSize())
		AfxMessageBox("기타하중재하위치가 슬래브를 벗어납니다.");

	return nElem;
}

void CFemHLLCalc::CalcHLiveLoadType()
{
	CPlateBridgeApp *pDB  = m_pDataManage->GetBridge();
	CCalcData       *pData= m_pDataManage->GetCalcData();
	CLineInfo       *pLine= pDB->m_pLine;

	double    Sta = 0;
	double SttSta = toM(pDB->GetGirder(-1)->GetStationStt());
	double EndSta = toM(pDB->GetGirder(-1)->GetStationEnd());
	long nLineType = 0;

	if(pData->m_nHLiveLoadType == -1) // 처음 실행일 경우 --> 스스로 판단
	{
		for(Sta=SttSta; Sta<=EndSta; Sta+=5)
		{
			nLineType = pLine->GetTypeLine(Sta);
			if(nLineType != 0) // 직선이 아니면
			{
				pData->m_nHLiveLoadType = 0; // 곡선교->표준점유폭(3m적용)
				break;
			}
		}
		if(pData->m_nHLiveLoadType == -1) 
			pData->m_nHLiveLoadType = 1;
	}
}


//////////////////////////////////////////////////////////////////////
/// 절점 위치에서 슬래브의 두께를 구함
//////////////////////////////////////////////////////////////////////
double CFemHLLCalc::GetSlabHeightOnJoint(CFEMManage *pFEM, long nIdx, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	
	double dPreDist = 0, dSlabThick = 0;
	long   cmpIdx   = 0;
	CDPoint A[10];
		
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp  *pGir     = pDB->GetGirder(nG);						
		CPlateBasicIndex *pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGir);
		pGir->GetHunchXyDom(pBxMatch, A, TRUE, FALSE, pDB->IsTUGir());
		if(nG==0) 
		{			
			dSlabThick = pDB->m_dThickSlabLeft;
			if(nIdx==cmpIdx) return dSlabThick;
			++cmpIdx;

			dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[0].x) - A[0].y);
			if(nIdx==cmpIdx) return dSlabThick;
			++cmpIdx;	
			dPreDist = A[0].x;			
		}
		if(nG==pDB->GetGirdersu()-1) 
		{	
			for(long n=0; n<=3; n++)
			{
				if(dPreDist != A[n].x)
				{
					dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[n].x) - A[n].y);
					if(nIdx==cmpIdx) return dSlabThick;
					++cmpIdx;						
				}
				dPreDist = A[n].x;
			}
			dSlabThick = pDB->m_dThickSlabRight;
			if(nIdx==cmpIdx) return dSlabThick;
			++cmpIdx;
			break;
		}
		for(long n=1; n<=6; n++)
		{
			if(dPreDist != A[n].x)
			{					
				dSlabThick = fabs(pDB->GetHeightSlabFromRoadCenterDom(pBxMatch,A[n].x) - A[n].y);
				if(nIdx==cmpIdx) return dSlabThick;
				++cmpIdx;	
			}
			dPreDist = A[n].x;
		}			
	}	
	return dSlabThick;
}

void CFemHLLCalc::SetWindLoadForce(long nJijum)
{
	CPlateBridgeApp* pDB= m_pDataManage->GetBridge();
	CPlateGirderApp* pGirCR = pDB->GetGirder(-1);
	CPlateGirderApp* pGirL  = pDB->GetGirder(0);
	CPlateGirderApp* pGirR  = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBx   = pDB->GetGirder(-1)->GetBxOnJijum(nJijum);
	CCalcData*       pData  = m_pDataManage->GetCalcData();

	BOOL bTwoBangEm = FALSE;

	///< 방음벽이 없을 경우는 리턴함
	if(pData->DESIGN_FLOOR_DATA[0].m_bBangEm==FALSE
	&& pData->DESIGN_FLOOR_DATA[1].m_bBangEm==FALSE) return;

	if(pData->DESIGN_FLOOR_DATA[0].m_bBangEm
	&& pData->DESIGN_FLOOR_DATA[1].m_bBangEm) bTwoBangEm = TRUE;

	// 방음벽 높이(방호벽 높이 포함)
	double dHBangEmL = toM(pData->DESIGN_FLOOR_DATA[0].m_dBangEmHeight);
	double dHBangEmR = toM(pData->DESIGN_FLOOR_DATA[1].m_dBangEmHeight);
	// 교량 총 폭(나중에 수정요함)
	double dSlabWidth= toM(pDB->GetWidthAct(pBx->GetStation()));
	// 교량 총 높이
	double dTotalHBriL = 0, dTotalHBriR = 0;
	// 슬래브 윗부분 = 방호벽 + 방음벽
	double dUpperHBriL = 0, dUpperHBriR = 0;
	// 풍하중 
	double dForceWindL = 0;
	double dForceWindR = 0;
	// 풍하중 모멘트 팔(거리)
	double dMomentDisL = 0;
	double dMomentDisR = 0;
	//////////////////////////////////////////////////////////
	// 교량 왼쪽
	if(pData->DESIGN_FLOOR_DATA[0].m_bBangEm)  // 벽형 강성 방호책
	{
		dTotalHBriL = toM(pGirL->GetHeightGirderByJijum(nJijum)) + toM(pDB->m_dThickSlabBase) + dHBangEmL;
		dUpperHBriL = dHBangEmL + toM(pDB->m_dThickSlabBase) / 2.0;

		dForceWindL = GetWindLoad(dSlabWidth, dTotalHBriL, dTotalHBriL);		
		dMomentDisL = dUpperHBriL - dTotalHBriL/2.0;

		pData->m_dLeftWindFroceFromL[nJijum]  = dForceWindL;				// P
		pData->m_dLeftWindMomentFromL[nJijum] = dForceWindL * dMomentDisL; // M
	
		if(!bTwoBangEm)///< 바람이 안쪽에서 불어올때
		{
			dForceWindL = GetWindLoad(dSlabWidth,dTotalHBriL,dHBangEmL);		
			dMomentDisL = dHBangEmL / 2.0  + toM(pDB->m_dThickSlabBase) / 2.0; 
			pData->m_dLeftWindFroceFromR[nJijum]  = -dForceWindL;				// P
			pData->m_dLeftWindMomentFromR[nJijum] = -dForceWindL * dMomentDisL; // M
		}
	}	
	//////////////////////////////////////////////////////////
	if(pData->DESIGN_FLOOR_DATA[1].m_bBangEm)  // 벽형 강성 방호책
	{
		dTotalHBriR = toM(pGirR->GetHeightGirderByJijum(nJijum)) + toM(pDB->m_dThickSlabBase) + dHBangEmR;
		dUpperHBriR = dHBangEmR + toM(pDB->m_dThickSlabBase) / 2.0;

		dForceWindR = GetWindLoad(dSlabWidth,dTotalHBriR,dTotalHBriR);		
		dMomentDisR = dUpperHBriR - dTotalHBriR/2.0;

		pData->m_dRightWindFroceFromR[nJijum]  = -dForceWindR; // P
		pData->m_dRightWindMomentFromR[nJijum] = -dForceWindR * dMomentDisR; // M

		if(!bTwoBangEm)
		{
			dForceWindR = GetWindLoad(dSlabWidth,dTotalHBriR,dHBangEmR);		
			dMomentDisR = dHBangEmR / 2.0 + toM(pDB->m_dThickSlabBase) / 2.0; 
			pData->m_dRightWindFroceFromL[nJijum]  = dForceWindR; // P
			pData->m_dRightWindMomentFromL[nJijum] = dForceWindR * dMomentDisR; // M
		}
	}
}


long CFemHLLCalc::GetLoadApplyMemberCount(CFEMManage *pFEM)
{
	long nCnt = 0;
	
	for (long nE=0; nE<pFEM->GetElementSize(); nE++)
	{
		CElement* pElem = pFEM->GetElement((unsigned short)nE);
		CVector vStt    = pFEM->GetJoint(pElem->m_nJStt)->m_vPoint;
		CVector vEnd    = pFEM->GetJoint(pElem->m_nJEnd)->m_vPoint;
		
		if (vStt.z==0 && vEnd.z==0) nCnt++; 
	}
	
	return nCnt;
}

// 풍하중 크기 구하기
double CFemHLLCalc::GetWindLoad(double dBridgeWidth, double dBridgeHeight, double dD)
{
	double Ratio = dBridgeWidth / dBridgeHeight;
	double WindLoad = 0;
	
	if(Ratio && Ratio < 8) 
		WindLoad = (4 - 0.2 * Ratio) * dD;

	if(8 <= Ratio)
		WindLoad = 2.4 * dD;

	if(WindLoad < 6) WindLoad = 6;

	return WindLoad;
}


void CFemHLLCalc::RunCrossAnalysis()
{
	CCalcData			*pData	= m_pDataManage->GetCalcData();
	CPlateBasicIndex	*pBx	= NULL;
	CTStandard			*pTS	= m_pDataManage->GetBMStandard()->m_pTStandard;

	if(pData->m_bRunCross) return;
	pData->m_bRunCross = TRUE;

	CRect rect = CRect(200, 3, 400, 18);
	if(!m_ctlProgress.GetSafeHwnd())
		m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_ctlProgress.SetRange(0, 100);
	m_ctlProgress.ShowWindow(SW_SHOW);	
	pTS->SetPaneText(0,"횡분배 해석중입니다...",TRUE);
	m_ctlProgress.SetPos(10);

	if(pData->m_HLType<0) pData->m_HLType = 0;
	SetHLsu(pData->m_HLType);	// 횡분배위치Bx를 생성함.

	m_pCalcStd->RemoveHLLFemData();
	m_pCalcStd->MakeHLLFemData(pData->m_HLsu);

	m_ctlProgress.SetPos(50);

	for(long nHL=0; nHL<pData->m_HLsu; nHL++)
	{
		long nLoadCaseIndex = 0;
		pBx = pData->m_DArrHLBasicIndexRun.GetAt(nHL);
		//기초설정
		CFEMManage *pFemManage = m_pCalcStd->GetHLLFemData(nHL);
		pFemManage->RemoveAllData();

		pFemManage->SYSTEM_VAR.m_szUnitType = "KN";
		pFemManage->SYSTEM_VAR.m_UX = pFemManage->SYSTEM_VAR.m_UY = pFemManage->SYSTEM_VAR.m_UZ = 0;
		pFemManage->SYSTEM_VAR.m_RX = pFemManage->SYSTEM_VAR.m_RY = pFemManage->SYSTEM_VAR.m_RZ = 0;	
		pFemManage->m_nExtraInfo = CROSS_ANALYSIS;
		pFemManage->m_szFEMName  =  "HORIZONTAL DISTRIBUTE";
		pFemManage->m_szLCName.RemoveAll();
		pFemManage->m_szTitle.RemoveAll();
		// 모델링
		MakeJointOnBx(pFemManage, pBx,FALSE);
		MakeElement(pFemManage,FALSE);
		MakeForcePreDeadLoad(pFemManage, pBx, nLoadCaseIndex);
		MakeForcePostDeadLoad(pFemManage, pBx, nLoadCaseIndex);
		MakeForceLiveLoad(pFemManage, pBx, nHL, nLoadCaseIndex, DB);
		MakeForceLiveLoad(pFemManage, pBx, nHL, nLoadCaseIndex, DL);
		MakeForceLivePeople(pFemManage, pBx, nLoadCaseIndex);
		MakeForceLiveTank(pFemManage, pBx, nHL, nLoadCaseIndex);
		MakeForceLiveTrailer(pFemManage, pBx, nHL, nLoadCaseIndex);

		MakeMaterial(pFemManage);
		MakeFrameSection(pFemManage);
		// 해석

		CFEMAnalysis Analysis(NULL, pFemManage);
		Analysis.SetWriteM3t(m_pCalcStd->m_bMasterLock);	
		if(!Analysis.RunFemAnalysisOnASolver())
		{
			AfxMessageBox("계산 수행 중 오류가 있습니다 !");
			return;
		}
	}
	m_ctlProgress.ShowWindow(SW_HIDE);	
	//pTS->SetPaneText(0,"",TRUE);
}

void CFemHLLCalc::RunWindAnalysis()
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CPlateBasicIndex *pBx     = NULL;
	CTStandard       *pTS     = m_pDataManage->GetBMStandard()->m_pTStandard;

	if(pData->m_bRunCrossWind) return;
	pData->m_bRunCrossWind = TRUE;

	CRect rect = CRect(200, 3, 400, 18);
	if(!m_ctlProgress.GetSafeHwnd())
		m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_ctlProgress.SetRange(0, 100);
	m_ctlProgress.ShowWindow(SW_SHOW);	
	
		
	pTS->SetPaneText(0,"풍하중 해석중입니다...",TRUE);
	m_ctlProgress.SetPos(10);

	long nFemData = pDB->m_nQtyJigan+1;	// 지점마다 모델링을 하여 보간함
	if(pDB->IsByunDanInBridge()==FALSE)
		nFemData = 1;
	m_pCalcStd->RemoveWindFemData();
	m_pCalcStd->MakeWindFemData(nFemData);	

	for(long n=0; n<nFemData; n++)
	{
		pBx = pDB->GetGirder(-1)->GetBxOnJijum(n);
		//기초설정
		CFEMManage *pFemManage = m_pCalcStd->GetWindFemData(n);
		pFemManage->RemoveAllData();
	
		pFemManage->SYSTEM_VAR.m_szUnitType = "KN";
		pFemManage->SYSTEM_VAR.m_UX = pFemManage->SYSTEM_VAR.m_UY = pFemManage->SYSTEM_VAR.m_UZ = 0;
		pFemManage->SYSTEM_VAR.m_RX = pFemManage->SYSTEM_VAR.m_RY = pFemManage->SYSTEM_VAR.m_RZ = 0;
		pFemManage->m_nExtraInfo	= HORIZONTAL_LOAD_ANALYSIS;
		pFemManage->m_szFEMName		=  "WIND DISTRIBUTE";
		pFemManage->m_szLCName.RemoveAll();
		pFemManage->m_szTitle.RemoveAll();
		// 모델링
		MakeJointOnBx(pFemManage, pBx,TRUE);
		MakeElement(pFemManage,TRUE);
		MakeForceWindLoad(pFemManage, n);
		MakeMaterial(pFemManage);
		MakeFrameSection(pFemManage);

		// 해석
		CFEMAnalysis Analysis(NULL, pFemManage);
		Analysis.SetWriteM3t(m_pCalcStd->m_bMasterLock);	
		if(!Analysis.RunFemAnalysisOnASolver())
		{
			AfxMessageBox("계산 수행 중 오류가 있습니다 !");
			return;
		}
		m_ctlProgress.SetPos((80/nFemData)*n);
	}

	m_ctlProgress.SetPos(100);
	m_ctlProgress.ShowWindow(SW_HIDE);	
	pTS->SetPaneText(0,"",TRUE);
}


void CFemHLLCalc::RunInfluenceAnalysis()
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();
	CTStandard      *pTS     = m_pDataManage->GetBMStandard()->m_pTStandard;

	if(pData->m_bRunInfluence) return;
	pData->m_bRunInfluence = TRUE;

	CRect rect = CRect(200, 3, 400, 18);
	if(!m_ctlProgress.GetSafeHwnd())
		m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_ctlProgress.SetRange(0, (short)pDB->GetGirdersu());
	m_ctlProgress.ShowWindow(SW_SHOW);	
	pTS->SetPaneText(0,"영향계수 해석중입니다...",TRUE);
	m_ctlProgress.SetPos(0);
	m_ctlProgress.SetPos(10);

	// 기존에 생성된 프레임 모델링 제거
	m_pCalcStd->RemoveInfluenceFemData();
	m_pCalcStd->MakeInfluenceFemData(pDB->GetGirdersu());
	CString str;

	for(long n=0; n<pDB->GetGirdersu(); n++)	
	{
		m_ctlProgress.SetPos(n+1);
		str.Format("영향선 해석 : GIRDER - %d", n+1);
		//기초설정
		CFEMManage *pFemManage = m_pCalcStd->GetInfluenceFemData(n);
		pFemManage->RemoveAllData();

		pFemManage->SYSTEM_VAR.m_szUnitType = "KN";
		pFemManage->SYSTEM_VAR.m_UX = pFemManage->SYSTEM_VAR.m_UY = pFemManage->SYSTEM_VAR.m_UZ = 0;
		pFemManage->SYSTEM_VAR.m_RX = pFemManage->SYSTEM_VAR.m_RY = pFemManage->SYSTEM_VAR.m_RZ = 0;	
		pFemManage->m_nExtraInfo = INFLUENCE_ANALYSIS;
		pFemManage->m_szFEMName  = str;
		pFemManage->m_szLCName.RemoveAll();
		pFemManage->m_szTitle.RemoveAll();
		// 모델링
		MakeJointOnGir(pFemManage, n);	
		MakeElementOnGir(pFemManage, n);	
		MakeMaterial(pFemManage);	
		long nSection = MakeFrameSectionOnGir(pFemManage, n);	
		MakeElementDefineSectionOnGir(pFemManage, nSection);
		MakeForceInfK(pFemManage);

		// 해석
		CFEMAnalysis	Analysis(NULL, pFemManage);
		Analysis.SetWriteM3t(m_pCalcStd->m_bMasterLock);
	
		if(!Analysis.RunFemAnalysisOnASolver())
		{
			AfxMessageBox("계산 수행 중 오류가 있습니다 !");
			return;
		}
	}

	m_ctlProgress.SetPos(100);
	m_ctlProgress.ShowWindow(SW_HIDE);	
	pTS->SetPaneText(0,"",TRUE);
}


/////////////////////////////////////////////////////////////////////////////
///바닥판 설계고려사항입력에서 임시로 생성한 방호벽 및 보도의 정확한 위치
///  (확폭고려)를 재정의한다.
/////////////////////////////////////////////////////////////////////////////
void CFemHLLCalc::SetReDefineEdge(long nHL)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CCalcData			*pData	= m_pDataManage->GetCalcData();
	CPlateBasicIndex	*pBx	= pData->m_DArrHLBasicIndexRun.GetAt(nHL);

	vector<CHLLGuardFence> vHLLGuardFence;

	long	nHdanSize	= pDB->GetQtyHDanNode();
	long	nType		= 0;
	long	nGuardIdx	= 0;
	double	dDisStt		= 0;
	double	dSumDis		= 0;

	for(long nHDan = 0; nHDan<nHdanSize; nHDan++)
	{
		nType	= pDB->GetValueTypeHDan(nHDan, 1);
		dDisStt	= pDB->GetLengthHDanDom(pBx, nHDan);
		dSumDis	+= dDisStt;

		if(nType==HT_GUARDFENCE || nType==HT_CENTERGUARDFENCE)
		{
			CCentSeparation	*pSep = pDB->GetGuardFence(nGuardIdx);
			if(!pSep) continue;
			CHLLGuardFence	HLLGuardFence;
			HLLGuardFence.m_nGuardIndex	= nGuardIdx;
			HLLGuardFence.m_dSttHDanPos	= dSumDis-dDisStt;
			HLLGuardFence.m_dEndHDanPos	= dSumDis;
			vHLLGuardFence.push_back(HLLGuardFence);

			nGuardIdx++;
		}
	}
}



/////////////////////////////////////////////////////////////////////////////
///횡분배수와 횡분배위치bx를 구한다.
/// 1. m_DArrHLBasicIndexPyun 편경사 변화위치(가로보위치중에서)
/// 2. m_DArrHLBasicIndexWidth 교량폭 변화위치(가로보위치중에서)
/// 3. m_DArrHLBasicIndexPyunWidth 편경사+교량폭 변화위치(가로보위치중에서)
/// 4. m_DArrHLBasicIndexRun 실제 횡분배를 수행할 위치
/// <기본적으로 1,2,3에서 첫번째 가로보위치BX는 모두 포함됨.>
/////////////////////////////////////////////////////////////////////////////
void CFemHLLCalc::SetHLsu(long nHLType)
{
	CPlateBridgeApp	*pDB    = m_pDataManage->GetBridge();
	CPlateGirderApp	*pGirCR = pDB->GetGirder(-1);
	CCalcData       *pData  = m_pDataManage->GetCalcData();

	BOOL bAddIdx=FALSE;
	BOOL bAddCrossBeam =FALSE;
	long nGsu   = pDB->GetGirdersu();
	long nCount = 0;
	CString str = "";
	long    n   = 0;

	pData->m_DArrHLBasicIndexPyun.RemoveAll();
	pData->m_DArrHLBasicIndexWidth.RemoveAll();
	pData->m_DArrHLBasicIndexPyunWidth.RemoveAll();
	pData->m_DArrHLBasicIndexRun.RemoveAll();
	pData->m_dStationHLLPos.RemoveAll();

	// 횡분배 수 구하기(가로보위치)
	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	CPlateBasicIndex *pBxMatch = NULL;
	CPlateBasicIndex *pBxpre = pBx;
	
	while(pBx)
	{
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}

		bAddIdx = TRUE;
		///<동일 횡단내 가로보가 모두 존재하는지를 검사
		for(long nG = 0; nG < nGsu; nG++)
		{
			CPlateGirderApp  *pGir = pDB->GetGirder(nG);
			pBxMatch = pGirCR->GetBxMatchByCrossBeam(pBx, pGir);
			if(pBxMatch == NULL || (nG != nGsu-1 && pBxMatch->GetSection() == NULL)) 
			{
				bAddIdx = FALSE;
				break;
			}
		}

		if(bAddIdx)
		{
			if(nHLType == 0)
			{
				double dHeightTaper = pBx->GetGirder()->GetHeightTaper(pBx->GetStation());
				BOOL bSttEndJijum = pBx->IsDanbuJijum();
				if(bSttEndJijum || (bAddCrossBeam==FALSE && dHeightTaper == 0))
				{
					pData->m_DArrHLBasicIndexPyun.Add(pBx);
					if(bSttEndJijum==FALSE && bAddCrossBeam == FALSE)
					{
						bAddCrossBeam = TRUE;
					}
				}
			}
			else // 모든 가로보 검토
			{
				pData->m_DArrHLBasicIndexPyun.Add(pBx);
			}
		}

		pBxpre = pBx;
		pBx = Finder.GetBxNext();
		nCount++;
	}

	pData->m_HLsu = pData->m_DArrHLBasicIndexPyun.GetSize();
	for(long n=0; n<pData->m_HLsu; n++)
	{
		pData->m_DArrHLBasicIndexRun.Add(pData->m_DArrHLBasicIndexPyun.GetAt(n));
	}

	for(n=0; n<pData->m_DArrHLBasicIndexRun.GetSize(); n++)
	{
		CPlateBasicIndex *pBx = pData->m_DArrHLBasicIndexRun.GetAt(n);
		pData->m_dStationHLLPos.Add(pBx->GetStation());
	}
}

void CFemHLLCalc::MakeForceInfK(CFEMManage *pFEM)
{
	long	nLoadCase	= 0;
	pFEM->MakeElementLoadForce(nLoadCase+1);
	pFEM->m_szLCName.SetAt(nLoadCase, "LC1");
	pFEM->m_szLCComment.SetAt(nLoadCase, "LC1");

	CLoadForceJoint	*pLoad1 = pFEM->GetJoint(0)->GetLoadForce(0);
	CLoadForceJoint	*pLoad2 = pFEM->GetJoint((unsigned short)pFEM->GetJointSize()-1)->GetLoadForce(0);

	pLoad1->SetLoadForce("",CVector(0,0,0),CVector(0, 1,0));
	pLoad2->SetLoadForce("",CVector(0,0,0),CVector(0, -1,0));
}

double CFemHLLCalc::GetElemLength(CFEMManage *pFemMange, long nElm)
{
	CElement *pElm  = pFemMange->GetElement((unsigned short)nElm);
	CJoint   *pJStt = pFemMange->GetJoint(pElm->m_nJStt);
	CJoint   *pJEnd = pFemMange->GetJoint(pElm->m_nJEnd);

	double dLengthElm = ~(pJStt->m_vPoint-pJEnd->m_vPoint);

	return dLengthElm;
}

CCombo* CFemHLLCalc::GetComboByName(CFEMManage *pFEM, CString szName)
{
	if(pFEM->m_ComboList.GetCount()==0) return NULL;

	POSITION pos = pFEM->m_ComboList.GetHeadPosition();
	while(pos)
	{
		CCombo *pComb = (CCombo *)pFEM->m_ComboList.GetNext(pos);
		if(szName==pComb->m_szName)
			return pComb;
	}
	return NULL;
}

CCombo* CFemHLLCalc::GetComboByLCName(CFEMManage *pFEM, CString szLCName)
{
	if(szLCName.Find("MAX") != -1 || szLCName.Find("MIN") != -1)
		szLCName.Delete(szLCName.GetLength()-4, 4);	// Max, Min삭제

	return GetComboByName(pFEM, szLCName);
}

void CFemHLLCalc::GetHDanArrayForLiveLoad(CDPointArray &ptArrVal, long nHL)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CCalcData			*pData	= m_pDataManage->GetCalcData();
	CPlateBasicIndex	*pBx	= pData->m_DArrHLBasicIndexRun.GetAt(nHL);

	CDPoint ptVal(0,0);
	double dLen=0;
	for(long n = 0; n < pDB->GetQtyHDanNode(); n++)
	{
		if(pDB->GetValueTypeHDan(n, 1)==HT_NONE)
		{
			for(long nn=n; nn < pDB->GetQtyHDanNode(); nn++)
			{
				if(pDB->GetValueTypeHDan(nn, 1)!=HT_NONE)
				{
					ptVal.y = dLen;
					ptArrVal.Add(ptVal);
					dLen += toM(pDB->GetLengthHDanAct(pBx->GetStation(),nn));
					ptVal.x = dLen;
					n = nn;
					break;
				}
				dLen += toM(pDB->GetLengthHDanAct(pBx->GetStation(), nn));
				n = nn;
			}
		}		
		else
		{
			dLen += toM(pDB->GetLengthHDanAct(pBx->GetStation(), n));
			ptVal.x = dLen;
		}
	}
	if(pDB->GetValueTypeHDan(pDB->GetQtyHDanNode()-1, 1)==HT_NONE)
	{
		ptVal.y = dLen;
		ptArrVal.Add(ptVal);
	}
}

// 슬래브 좌측끝에서 거더 중심까지의 거리
double CFemHLLCalc::GetGirderPos(long nHL, long nG)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CCalcData			*pData	= m_pDataManage->GetCalcData();
	CPlateBasicIndex	*pBx	= pData->m_DArrHLBasicIndexRun.GetAt(nHL);

	CPlateGirderApp		*pGir	= pBx->GetGirder();
	CPlateGirderApp		*pGirMat= pDB->GetGirder(0);
	CPlateBasicIndex	*pBxMat	= pBx->GetBxMatchByCrossBeam(pGirMat);

	CDPoint A[HDANLINESU]; // 포장상단의 포인트
	CDPoint B[10]; // 거더의 포인트

	pDB->GetXyPaveUpperDom(pBxMat, A);
	pGirMat->GetHunchXyDom(pBxMat, B);

	double	dGirCenFir	= (B[1].x + B[2].x)/2;
	double	dValue		= dGirCenFir-A[0].x;
	if(nG==0)	return dValue;

	CPlateGirderApp		*pGirMat2	= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxMat2	= pBxMat->GetBxMatchByCrossBeam(pGirMat2);
	pGirMat2->GetHunchXyDom(pBxMat2, B);

	dValue	= (B[1].x + B[2].x)/2 - A[0].x;

	return dValue;
}

BOOL CFemHLLCalc::IsSameI33(double dI331, double dI332, double dError)
{
	double	dI331m4	= toM4(dI331);
	double	dI332m4	= toM4(dI332);

	BOOL	bSame	= FALSE;
	if(fabs(dI331m4-dI332m4)<dError)
		bSame = TRUE;
	return bSame;
}

void CFemHLLCalc::AddUnitPointTankOrTrailor( CFEMManage *pFEM, BOOL bTank, double dRoadStt, double dRoadEnd, long nIndex, long nLoadCase )
{
	CCalcData *pData = m_pDataManage->GetCalcData();
	if(pData == NULL || pFEM == NULL)
		return ;
	if(fabs(dRoadStt - dRoadEnd) < 0.001)
		return ;
	if(dRoadStt > dRoadEnd)
		return ;

	double dTankWidth	 = pData->m_TankWidth;
	double dTrailerWidth = pData->m_TrailWidth;
	double dWidth = (bTank == TRUE) ? dTankWidth : dTrailerWidth ;

	if(dRoadEnd - dRoadStt <= dWidth)
		return ;
	
	CString szLoadCaseTitle = NOTRANS("");
	CString szTitle = NOTRANS("");

	if(bTank == TRUE) {
		szLoadCaseTitle.Format("TANK-%d", nIndex+1);
		szTitle.Format("탱크 재하시 - %d", nIndex+1);
	}
	else {
		szLoadCaseTitle.Format("TRAIL-%d", nIndex+1);
		szTitle.Format("트레일러 재하시 - %d", nIndex+1);
	}

	pFEM->m_szLCName.Add(szLoadCaseTitle);
	pFEM->MakeElementLoadForce(nLoadCase+1);

	double dDisL = (dRoadStt + dRoadEnd) / 2 - dWidth/2;
	double dDisR = (dRoadStt + dRoadEnd) / 2 + dWidth/2;
	AddUnitPointForceAtLeft(pFEM, dDisL, nLoadCase);
	AddUnitPointForceAtLeft(pFEM, dDisR, nLoadCase);
}
