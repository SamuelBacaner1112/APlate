// BMRecord.cpp: implementation of the CBMRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GangjeBase.h" 
#include "BMRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CBMRecord::CBMRecord(CBMRecord* pBM /*= NULL*/)
{
	if(pBM)
		*this = *pBM;
	else
		InitData();
}

CBMRecord::~CBMRecord()
{

}

CBMRecord& CBMRecord::operator=(const CBMRecord &obj)
{
	if(this==&obj) return *this;

	m_dwSummaryFlag = obj.m_dwSummaryFlag;
	m_sSummaryMark  = obj.m_sSummaryMark;
	m_dwDisableFlag = obj.m_dwDisableFlag;
	m_dwFlag		= obj.m_dwFlag;
	m_nGirder		= obj.m_nGirder;
	m_nCountMake	= obj.m_nCountMake;
	m_nSplice		= obj.m_nSplice;
	m_nKindName		= obj.m_nKindName;
	m_nEleName		= obj.m_nEleName;
	m_nProcessBig	= obj.m_nProcessBig;
	m_nProcessMid	= obj.m_nProcessMid;
	m_nSectionNumber = obj.m_nSectionNumber;
	m_sKindName   	= obj.m_sKindName;
	m_nSubEleName	= obj.m_nSubEleName;
	m_nQty			= obj.m_nQty;
	m_vJewon		= obj.m_vJewon;
	m_vJewonAngle	= obj.m_vJewonAngle;
	m_vJewonBasic	= obj.m_vJewonBasic;
	m_dUWeight		= obj.m_dUWeight;
	m_dTWeight		= obj.m_dTWeight;
	m_dTWeightBasic = obj.m_dTWeightBasic;
	m_dAreaBuje		= obj.m_dAreaBuje;
	m_dStationRec	= obj.m_dStationRec;
	m_sMaterial		= obj.m_sMaterial;
	m_nMarkBuje		= obj.m_nMarkBuje;
	m_nFirstClass	= obj.m_nFirstClass;

	m_sRefDWG		= obj.m_sRefDWG;
	m_sUsage		= obj.m_sUsage;
	m_dFactorArea	= obj.m_dFactorArea;
	m_dFactorAreaBasic= obj.m_dFactorAreaBasic;
	m_dThickSecond	= obj.m_dThickSecond;
	m_dWidthSecond	= obj.m_dWidthSecond;
	m_nSizeType = obj.m_nSizeType;
	return *this;
}

void CBMRecord::InitData()
{
	m_dwDisableFlag = 0;
	m_sSummaryMark.Empty();
	m_dwSummaryFlag = 0;
	m_dwFlag		= 0;
	m_nGirder		= -1;
	m_nCountMake	= 0;
	m_nSplice		= -1;
	m_nKindName		= 0;
	m_nEleName		= 0;
	m_nSectionNumber = -1;
	m_nSubEleName	= 0;
	m_nQty			= 0;
	m_dUWeight		= 0;
	m_dTWeight		= 0;
	m_dTWeightBasic = 0;
	m_dAreaBuje		= 0;
	m_dStationRec	= 0;
	m_vJewon		= CVector(0,0,0);
	m_vJewonAngle	= CVector(1,1,1);
	m_vJewonBasic	= CVector(0,0,0);
	m_nMarkBuje		= -1;
	m_nFirstClass	= 100;
	m_nProcessMid	= -1;
	m_nProcessBig	= -1;

	m_sRefDWG.Empty();
	m_sUsage.Empty();
	m_sKindName.Empty();
	m_sMaterial.Empty();

	m_dFactorArea		= 1.0;
	m_dFactorAreaBasic	= 1.0;
	m_dThickSecond		= 0.0;
	m_dWidthSecond		= 0.0;
	m_nSizeType = 3;
	
	SetNormal(TRUE);
}

BOOL CBMRecord::IsSameRecord(const CBMRecord* pRec,DWORD exceptionFlag/*=0*/) const
{
	if(this == pRec) return TRUE;

	if((exceptionFlag&0x000001)==0)	if(m_nGirder			!= pRec->m_nGirder)				return FALSE;
	if((exceptionFlag&0x000002)==0)	if(m_nSplice			!= pRec->m_nSplice)				return FALSE;
	if((exceptionFlag&0x000004)==0)	if(m_nKindName			!= pRec->m_nKindName)			return FALSE;
	if((exceptionFlag&0x000008)==0)	if(m_nEleName			!= pRec->m_nEleName)			return FALSE;
	if((exceptionFlag&0x000010)==0)	if(m_nSectionNumber		!= pRec->m_nSectionNumber)		return FALSE;
	if((exceptionFlag&0x000020)==0)	if(m_nSubEleName		!= pRec->m_nSubEleName)			return FALSE;
	if((exceptionFlag&0x000040)==0) if(Round(m_vJewon.x,0)	!= Round(pRec->m_vJewon.x,0))	return FALSE;
	if((exceptionFlag&0x000080)==0)	if(Round(m_vJewon.y,0)	!= Round(pRec->m_vJewon.y,0))	return FALSE;
	if((exceptionFlag&0x000100)==0)	if(Round(m_vJewon.z,0)	!= Round(pRec->m_vJewon.z,0))	return FALSE;
	if((exceptionFlag&0x000200)==0)	if(m_dUWeight			!= pRec->m_dUWeight)			return FALSE;
	if((exceptionFlag&0x000400)==0)	if(m_dTWeight			!= pRec->m_dTWeight)			return FALSE;
	if((exceptionFlag&0x000800)==0)	if(m_sMaterial			!= pRec->m_sMaterial)			return FALSE;
	if((exceptionFlag&0x004000)==0)	if(m_nMarkBuje			!= pRec->m_nMarkBuje)			return FALSE;
	if((exceptionFlag&0x008000)==0)	if(m_nSizeType			!= pRec->m_nSizeType)			return FALSE;

	return TRUE;
}

int CBMRecord::CompareSang(const CBMRecord* pRec) const
{
	if(m_nGirder		!= pRec->m_nGirder)			return m_nGirder		> pRec->m_nGirder		 ? 1 : -1;
	if(m_nSplice		!= pRec->m_nSplice)			return m_nSplice		> pRec->m_nSplice		 ? 1 : -1;
	if(m_nEleName		!= pRec->m_nEleName)		return m_nEleName		> pRec->m_nEleName		 ? 1 : -1;
	if(m_nSectionNumber != pRec->m_nSectionNumber)	return m_nSectionNumber > pRec->m_nSectionNumber ? 1 : -1;
	if(m_nMarkBuje		!= pRec->m_nMarkBuje)		return m_nMarkBuje		> pRec->m_nMarkBuje		 ? 1 : -1;
	if(m_nSubEleName	!= pRec->m_nSubEleName)		return m_nSubEleName	> pRec->m_nSubEleName	 ? 1 : -1;
	if(m_vJewon.y		!= pRec->m_vJewon.y)		return m_vJewon.y		> pRec->m_vJewon.y		 ? 1 : -1;
	if(m_vJewon.x		!= pRec->m_vJewon.x)		return m_vJewon.x		> pRec->m_vJewon.x		 ? 1 : -1;
	if(m_vJewon.z		!= pRec->m_vJewon.z)		return m_vJewon.z		> pRec->m_vJewon.z		 ? 1 : -1;
	return 0;
}

int CBMRecord::CompareStandardSang(const CBMRecord* pRec) const
{
	return CompareSang(pRec);
}

// 대조립마크별 상세와 동일 (동운타입분류만 추가:1차분류)
int CBMRecord::CompareStandardShopType2(const CBMRecord* pRec) const
{
	if(m_nFirstClass != pRec->m_nFirstClass) return m_nFirstClass > pRec->m_nFirstClass	? 1 : -1;
	if(m_nGirder	 != pRec->m_nGirder)	 return m_nGirder	  > pRec->m_nGirder		? 1 : -1;
	if(m_nSplice	 != pRec->m_nSplice)	 return m_nSplice	  > pRec->m_nSplice		? 1 : -1;
	if(m_nMarkBuje	 != pRec->m_nMarkBuje)	 return m_nMarkBuje	  > pRec->m_nMarkBuje	? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)	 return m_vJewon.y	  > pRec->m_vJewon.y		? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)	 return m_vJewon.x	  > pRec->m_vJewon.x		? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)	 return m_vJewon.z	  > pRec->m_vJewon.z		? 1 : -1;
	return 0;
}

int CBMRecord::CompareCar(const CBMRecord* pRec) const
{
	if(m_nGirder		!= pRec->m_nGirder)			return m_nGirder		> pRec->m_nGirder		 ? 1 : -1;
	if(m_nSplice		!= pRec->m_nSplice)			return m_nSplice		> pRec->m_nSplice		 ? 1 : -1;
	if(m_nEleName		!= pRec->m_nEleName)		return m_nEleName		> pRec->m_nEleName		 ? 1 : -1;
	if(m_nSectionNumber != pRec->m_nSectionNumber)	return m_nSectionNumber > pRec->m_nSectionNumber ? 1 : -1;
	if(m_nMarkBuje		!= pRec->m_nMarkBuje)		return m_nMarkBuje		> pRec->m_nMarkBuje		 ? 1 : -1;
	if(m_vJewon.y		!= pRec->m_vJewon.y)		return m_vJewon.y		> pRec->m_vJewon.y		 ? 1 : -1;
	if(m_vJewon.x		!= pRec->m_vJewon.x)		return m_vJewon.x		> pRec->m_vJewon.x		 ? 1 : -1;
	if(m_vJewon.z		!= pRec->m_vJewon.z)		return m_vJewon.z		> pRec->m_vJewon.z		 ? 1 : -1;
	return 0;
}

int CBMRecord::CompareCarSection(const CBMRecord* pRec) const
{
	if(m_nMarkBuje	  != pRec->m_nMarkBuje)		return m_nMarkBuje	  > pRec->m_nMarkBuje	 ? 1 : -1;
	if(m_vJewon.y	  != pRec->m_vJewon.y)		return m_vJewon.y	  > pRec->m_vJewon.y	 ? 1 : -1;
	if(m_vJewon.x	  != pRec->m_vJewon.x)		return m_vJewon.x	  > pRec->m_vJewon.x	 ? 1 : -1;
	if(m_vJewon.z	  != pRec->m_vJewon.z)		return m_vJewon.z	  > pRec->m_vJewon.z	 ? 1 : -1;
	return 0;
}

int CBMRecord::CompareCarAssembly(const CBMRecord* pRec) const
{
	if(m_nGirder		!= pRec->m_nGirder)			return m_nGirder		 > pRec->m_nGirder		   ? 1 : -1;
	if(m_nSplice		!= pRec->m_nSplice)			return m_nSplice		 > pRec->m_nSplice		   ? 1 : -1;
	if(m_nMarkBuje		!= pRec->m_nMarkBuje)		return m_nMarkBuje		 > pRec->m_nMarkBuje	   ? 1 : -1;
	if(m_vJewon.y		!= pRec->m_vJewon.y)		return m_vJewon.y		 > pRec->m_vJewon.y		   ? 1 : -1;
	if(m_vJewon.x		!= pRec->m_vJewon.x)		return m_vJewon.x		 > pRec->m_vJewon.x		   ? 1 : -1;
	if(m_vJewon.z		!= pRec->m_vJewon.z)		return m_vJewon.z		 > pRec->m_vJewon.z		   ? 1 : -1;
	return 0;}

int CBMRecord::CompareSectionType(const CBMRecord* pRec) const
{
	if(m_nEleName		!= pRec->m_nEleName)		return m_nEleName > pRec->m_nEleName ? 1 : -1;
	if(m_nSectionNumber != pRec->m_nSectionNumber)	return m_nSectionNumber > pRec->m_nSectionNumber ? 1 : -1;
	if(m_nSubEleName	!= pRec->m_nSubEleName)		return m_nSubEleName > pRec->m_nSubEleName ? 1 : -1;
	if(m_nKindName		!= pRec->m_nKindName)		return m_nKindName > pRec->m_nKindName ? 1 : -1;

	if(m_vJewon.y		!= pRec->m_vJewon.y)	 return m_vJewon.y > pRec->m_vJewon.y		? 1 : -1;
	if(m_vJewon.x		!= pRec->m_vJewon.x)	 return m_vJewon.x > pRec->m_vJewon.x		? 1 : -1;
	if(m_vJewon.z		!= pRec->m_vJewon.z)	 return m_vJewon.z > pRec->m_vJewon.z		? 1 : -1;

	if(m_nCountMake		!= pRec->m_nCountMake)	return m_nCountMake > pRec->m_nCountMake ? 1 : -1;
	if(m_dStationRec	!= pRec->m_dStationRec) return m_dStationRec > pRec->m_dStationRec ? 1 : -1;
	return 0;
}


int CBMRecord::CompareThick(const CBMRecord* pRec) const
{
	if(m_nKindName  != pRec->m_nKindName) return m_nKindName > pRec->m_nKindName ? 1 : -1;
	if(m_sMaterial  != pRec->m_sMaterial)  return m_sMaterial  < pRec->m_sMaterial	? 1 : -1;
	if(m_vJewon.y   != pRec->m_vJewon.y)   return m_vJewon.y   < pRec->m_vJewon.y	? 1 : -1;

	return 0;
}

int CBMRecord::CompareThickSang(const CBMRecord* pRec) const
{
	if(m_nKindName		 != pRec->m_nKindName)			return m_nKindName		  > pRec->m_nKindName	? 1 : -1;
	if(m_sMaterial		 != pRec->m_sMaterial)			return m_sMaterial		  < pRec->m_sMaterial	? 1 : -1;
	if(m_vJewon.y		 != pRec->m_vJewon.y)			return m_vJewon.y		  < pRec->m_vJewon.y	? 1 : -1;
	if(m_nProcessBig	 != pRec->m_nProcessBig)		return m_nProcessBig	  > pRec->m_nProcessBig		? 1 : -1;

	if(m_nProcessBig==1 && pRec->m_nProcessBig==1 && m_nMarkBuje!=pRec->m_nMarkBuje)
		return m_nMarkBuje  > pRec->m_nMarkBuje ? 1 : -1;

	if(m_nProcessMid	 != pRec->m_nProcessMid)		return m_nProcessMid	  > pRec->m_nProcessMid		? 1 : -1;

	return 0;
}

int CBMRecord::CompareCarTotal(const CBMRecord* pRec) const
{
	if(m_nGirder != pRec->m_nGirder) return m_nGirder > pRec->m_nGirder ? 1 : -1;
	if(m_nSplice != pRec->m_nSplice) return m_nSplice > pRec->m_nSplice ? 1 : -1;
	return 0;
}
int CBMRecord::CompareBuje(const CBMRecord* pRec) const
{
	if(m_nSubEleName != pRec->m_nSubEleName) return m_nSubEleName	> pRec->m_nSubEleName	? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)	 return m_vJewon.y		> pRec->m_vJewon.y		? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)	 return m_vJewon.x		> pRec->m_vJewon.x		? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)	 return m_vJewon.z		> pRec->m_vJewon.z		? 1 : -1;
	if(m_nCountMake  != pRec->m_nCountMake)	 return m_nCountMake	> pRec->m_nCountMake	? 1 : -1;
	if(m_dStationRec != pRec->m_dStationRec) return m_dStationRec	> pRec->m_dStationRec	? 1 : -1;
	if(m_sMaterial	 != pRec->m_sMaterial)	 return m_sMaterial		< pRec->m_sMaterial		? 1 : -1;
	return 0;
} 

int CBMRecord::CompareAssemblyTotal(const CBMRecord* pRec) const
{
	if(m_nGirder	 != pRec->m_nGirder)		return m_nGirder	 > pRec->m_nGirder	   ? 1 : -1;
	if(m_nSplice	 != pRec->m_nSplice)		return m_nSplice	 > pRec->m_nSplice	   ? 1 : -1;
	return 0;
}

int CBMRecord::CompareAssemblyField(const CBMRecord* pRec) const
{
	return CompareAssemblyFact(pRec);
}

int CBMRecord::CompareAssemblyFact(const CBMRecord* pRec) const
{
	if(m_nFirstClass != pRec->m_nFirstClass)	return m_nFirstClass > pRec->m_nFirstClass	? 1 : -1;
	if(m_nGirder	 != pRec->m_nGirder)		return m_nGirder	 > pRec->m_nGirder		? 1 : -1;
	if(m_nSplice	 != pRec->m_nSplice)		return m_nSplice	 > pRec->m_nSplice		? 1 : -1;
	//
	if(m_nMarkBuje   != pRec->m_nMarkBuje)		return m_nMarkBuje  > pRec->m_nMarkBuje		? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)		return m_vJewon.y > pRec->m_vJewon.y		? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)		return m_vJewon.x > pRec->m_vJewon.x		? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)		return m_vJewon.z > pRec->m_vJewon.z		? 1 : -1;
	return 0;
}

int CBMRecord::CompareSectionComp(const CBMRecord* pRec) const
{
	if(m_nEleName		!= pRec->m_nEleName)		return m_nEleName > pRec->m_nEleName ? 1 : -1;
	if(m_nSectionNumber != pRec->m_nSectionNumber)	return m_nSectionNumber > pRec->m_nSectionNumber ? 1 : -1;
	if(m_nSubEleName	!= pRec->m_nSubEleName)		return m_nSubEleName > pRec->m_nSubEleName ? 1 : -1;
	if(m_nMarkBuje != pRec->m_nMarkBuje) return m_nMarkBuje > pRec->m_nMarkBuje ? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)	 return m_vJewon.y > pRec->m_vJewon.y		? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)	 return m_vJewon.x > pRec->m_vJewon.x		? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)	 return m_vJewon.z > pRec->m_vJewon.z		? 1 : -1;
	//
	if(m_nMarkBuje   != pRec->m_nMarkBuje)		return m_nMarkBuje  > pRec->m_nMarkBuje		? 1 : -1;
	return 0;
}

int CBMRecord::CompareBujeComp(const CBMRecord* pRec) const
{
	if(m_nSubEleName	!= pRec->m_nSubEleName)		return m_nSubEleName > pRec->m_nSubEleName ? 1 : -1;
//	if(m_nMarkBuje != pRec->m_nMarkBuje) return m_nMarkBuje > pRec->m_nMarkBuje ? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)	 return m_vJewon.y > pRec->m_vJewon.y		? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)	 return m_vJewon.x > pRec->m_vJewon.x		? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)	 return m_vJewon.z > pRec->m_vJewon.z		? 1 : -1;
	//
	if(m_nMarkBuje   != pRec->m_nMarkBuje)		return m_nMarkBuje  > pRec->m_nMarkBuje		? 1 : -1;
	return 0;
}

int CBMRecord::CompareBasicComp(const CBMRecord* pRec) const
{
	return 0;
}

int CBMRecord::CompareBujeMarkSang(const CBMRecord* pRec) const
{
	if(m_nMarkBuje != pRec->m_nMarkBuje) return m_nMarkBuje > pRec->m_nMarkBuje ? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)	 return m_vJewon.y > pRec->m_vJewon.y		? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)	 return m_vJewon.x > pRec->m_vJewon.x		? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)	 return m_vJewon.z > pRec->m_vJewon.z		? 1 : -1;
	return 0;
}

int CBMRecord::CompareBujeMarkDeduct(const CBMRecord* pRec) const
{
	return CompareBujeMarkSang(pRec);
}

int CBMRecord::CompareGrandAssemblySang(const CBMRecord* pRec) const
{
	if(m_nMarkBuje != pRec->m_nMarkBuje)	return m_nMarkBuje  > pRec->m_nMarkBuje  ? 1 : -1;
	return 0;
}

CString CBMRecord::GetStrGirder(long nStart/*=0*/) const
{
	CString sResult = _T("");
	if(m_nGirder<0)	return sResult;
	sResult.Format("G%ld",m_nGirder+1+nStart);
	return sResult;
}
CString CBMRecord::GetStrSplice(long nStart/*=0*/) const
{
	CString sResult = _T("");
	if(m_nSplice<0)	return sResult;
	sResult.Format("S%ld",m_nSplice+1+nStart);
	return sResult;
}
CString CBMRecord::GetStrSectionType() const
{
	CGangjeBase GangBase;
	return GangBase.m_sKindName[m_nKindName];
}

CString CBMRecord::GetStrEleName() const
{
	CGangjeBase GangBase;
	CString sResult = GangBase.m_sEleNameMain[m_nEleName];

	if(m_nSectionNumber > 0)
	{
		sResult.Format("%s%ld",GangBase.m_sEleNameMain[m_nEleName],
			           m_nSectionNumber);
	}

	return sResult;

}

CString CBMRecord::GetStrBujeKindName() const
{
	if(m_nKindName==MATERIAL_KIND_PLATE)			return "PLATE";
	else if(m_nKindName==MATERIAL_KIND_BOLT		)	return "볼트";
	else if(m_nKindName==MATERIAL_KIND_BOLT2NUT)	return "BOLT2NUT";	
	else if(m_nKindName==MATERIAL_KIND_ROUNDBAR)	return "원형강";		
	else if(m_nKindName==MATERIAL_KIND_STUDWELD)	return "전단연결재";	
	else if(m_nKindName==MATERIAL_KIND_STUDBOLT)	return "전단연결재";	
	else if(m_nKindName==MATERIAL_KIND_WIREMESH)	return "철망";	
	else if(m_nKindName==MATERIAL_KIND_PIPE		)	return "파이프";	
	else if(m_nKindName==MATERIAL_KIND_REBAR	)	return "철근";
	else if(m_nKindName==MATERIAL_KIND_ELBOW	)	return "ELBOW";
	else if(m_nKindName==MATERIAL_KIND_ELBOWEQUAL)	return "ELBOWEQUAL";
	else if(m_nKindName==MATERIAL_KIND_SLEEVEFORPIPE)	return "SLEEVE";
	else if(m_nKindName==MATERIAL_KIND_BEAM		)	return "형강";
	else if(m_nKindName==MATERIAL_KIND_AL		)	return "알루미늄";

	return "";
}

CString CBMRecord::GetStrSteelBujeKindName() const
{
	CString	szKindName	= _T("");
	if(m_nKindName==MATERIAL_KIND_BEAM)
	{
		if(GetStrSubEleName()==_T("등변 ㄱ 형강"))		szKindName	= _T("L");
		if(GetStrSubEleName()==_T("부등변 ㄱ 형강"))	szKindName	= _T("L");
		if(GetStrSubEleName()==_T("ㄷ 형강"))			szKindName	= _T("ㄷ");
		if(GetStrSubEleName()==_T("CT 형강"))			szKindName	= _T("CT");
		if(GetStrSubEleName()==_T("I 형강"))			szKindName	= _T("I");
		if(GetStrSubEleName()==_T("H 형강"))			szKindName	= _T("H");
	}

	return szKindName;
}


CString CBMRecord::GetStrSubEleName() const
{
	CGangjeBase GangBase;
	return GangBase.m_sEleNameSub[m_nSubEleName];
}

CString CBMRecord::GetStrQty() const
{
	CString sResult = _T("");
	if(m_nQty==0)	return sResult;

	sResult.Format("%ld",m_nQty);
	return sResult;
}

CString CBMRecord::GetStrWidth(long nRound) const
{
	CString sResult = _T("");
	if(m_vJewon.x==0)	return sResult;

	sResult = GetStringOutValue(m_vJewon.x,nRound);

	if(m_vJewonAngle.x !=1)
		sResult += "X" + GetStringOutValue(m_vJewonAngle.x,nRound);

	return sResult;
}
CString CBMRecord::GetStrThick(long nRound) const
{
	CString sResult = _T("");
	if(m_vJewon.y==0)	return sResult;

	sResult = GetStringOutValue(m_vJewon.y,nRound);

	if(m_vJewonAngle.y !=1)
	{
		if(GetStringOutValue(m_vJewonAngle.y, nRound)=="0")
			return sResult;
		else
			sResult += "X" + GetStringOutValue(m_vJewonAngle.y, nRound);
	}

	return sResult;
}
CString CBMRecord::GetStrLength(long nRound) const
{
	CString sResult = _T("");
	if(m_vJewon.z==0)	return sResult;

	sResult = GetStringOutValue(m_vJewon.z,nRound);

	return sResult;
}

CString CBMRecord::GetStrTWeight(BOOL bUnitTon, long nRound) const
{
	CGangjeBase GangBase;
	CString sResult = _T("");

	if(m_dTWeight==0)	return sResult;

	double Weight = bUnitTon ? toTon(m_dTWeight) : m_dTWeight;

	sResult = GangBase.GetStringWeight(Weight,nRound);

	return sResult;
}


CString CBMRecord::GetStrWidthBasic(long nRound) const
{
	CString sResult = _T("");
	if(m_vJewonBasic.x==0)	return sResult;

	sResult = GetStringOutValue(m_vJewonBasic.x,nRound);

	return sResult;
}

CString CBMRecord::GetStrThickBasic(long nRound) const
{
	CString sResult = _T("");
	if(m_vJewonBasic.y==0)	return sResult;

	sResult = GetStringOutValue(m_vJewonBasic.y,nRound);

	return sResult;
}
CString CBMRecord::GetStrLengthBasic(long nRound) const
{
	CString sResult = _T("");
	if(m_vJewonBasic.z==0)	return sResult;

	sResult = GetStringOutValue(m_vJewonBasic.z,nRound);

	return sResult;
}

CString CBMRecord::GetStrTWeightBasic(BOOL bUnitTon, long nRound) const
{
	CGangjeBase GangBase;
	CString sResult = _T("");

	if(m_dTWeightBasic==0)	return sResult;

	double Weight = bUnitTon ? toTon(m_dTWeightBasic) : m_dTWeightBasic;

	sResult = GangBase.GetStringWeight(Weight,nRound);

	return sResult;
}


// kg/m^3 단위로 리턴
CString CBMRecord::GetStrUWeight(long nRound) const
{
	CString sResult = _T("");

	if(m_dUWeight==0) return sResult;

	long nRoundApply = nRound;
	if(m_dUWeight>1000.0)
		nRoundApply = max(0,nRound-4);

	if(m_nKindName==MATERIAL_KIND_BEAM)		// 형강
		sResult.Format("%.2lf", m_dUWeight);	// 2008.10.30 simdongyoun
//		sResult.Format("%.4lf", m_dUWeight);
	else if(m_nKindName==MATERIAL_KIND_BOLT)// 볼트
		sResult.Format("%.4lf", m_dUWeight);
	else if(m_nKindName==MATERIAL_KIND_STUDWELD)// 전단연결재
		sResult.Format("%.4lf", m_dUWeight);
	else if(m_nKindName==MATERIAL_KIND_REBAR)	// 철근(슬래브앵커)
		sResult.Format("%.4lf", m_dUWeight);
	else										// plate		
		sResult = GetStringOutValue(m_dUWeight,nRoundApply);

	return sResult;
}

CString CBMRecord::GetStrMaterial() const
{
	return m_sMaterial;
}

void CBMRecord::SetNormal(BOOL bNormal)
{
	SetBitFlag32(m_dwFlag,1,bNormal);
}

BOOL CBMRecord::IsNormal() const
{
	return GetBitFlag32(m_dwFlag,1);
}

void CBMRecord::SetStrSectionType(const CString& sName)
{
	CGangjeBase GangBase;
	long i = 0;
	while(1)
	{
		if(GangBase.m_sKindName[i].IsEmpty()) break;
		if(GangBase.m_sKindName[i] == sName)
		{
			m_nKindName = i;
			return;
		}
		i++;
	}

//	ASSERT(FALSE);
}

long CBMRecord::GetDivideEleNameNum(const CString& sName) const
{
	CString sDM[10] = {"A","P","CU","CX","CL","B","D","E","F","G"};
	for(long n = 0; n < 10; n++)
	{
		long nFix = sDM[n].GetLength(); // 1 or 2
		if(sName.GetLength() < nFix) continue;
		CString sFix = sName.Left(nFix);
		if(sFix == sDM[n])
			return atol(sName.Mid(nFix));
	}

	return -1;
}

CString CBMRecord::GetDivideEleNameStr(const CString& sName) const
{
	CString sDM[10] = {"A","P","CU","CX","CL","B","D","E","F","G"};
	for(long n = 0; n < 10; n++)
	{
		long nFix = sDM[n].GetLength(); // 1 or 2
		if(sName.GetLength() < nFix) continue;
		CString sFix = sName.Left(nFix);
		if(sFix == sDM[n])
			return sFix;
	}

	return sName;
}

void CBMRecord::SetStrEleName(const CString& sEleName,long nSecTypeNumber)
{
	CGangjeBase GangBase;
	long i = 0;
	while(1)
	{
		if(GangBase.m_sEleNameMain[i].IsEmpty()) break;
		if(GangBase.m_sEleNameMain[i] == sEleName)
		{
			m_nEleName = i;
			m_nSectionNumber = nSecTypeNumber;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

void CBMRecord::SetStrEleName(const CString& sEleName)
{
	CGangjeBase GangBase;

	CString sName = GetDivideEleNameStr(sEleName);
	long nNum = GetDivideEleNameNum(sEleName);

	long i = 0;
	while(1)
	{
		if(GangBase.m_sEleNameMain[i].IsEmpty()) break;
		if(GangBase.m_sEleNameMain[i] == sName)
		{
			m_nEleName = i;
			m_nSectionNumber = nNum;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

void CBMRecord::SetStrSubEleName(const CString& sName)
{
	CGangjeBase GangBase;
	long i = 0;
	while(1)
	{ 
		if(GangBase.m_sEleNameSub[i].IsEmpty()) break;
		if(GangBase.m_sEleNameSub[i] == sName)
		{
			m_nSubEleName = i;
			return;
		}
		i++;
	}

		ASSERT(FALSE);
}

void CBMRecord::SetStrProcessBig(const CString& sName)
{
	CGangjeBase GangBase;
	long i = 0;
	while(1)
	{ 
		if(GangBase.m_sProcessBig[i].IsEmpty()) break;
		if(GangBase.m_sProcessBig[i] == sName)
		{
			m_nProcessBig = i;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

void CBMRecord::SetStrProcessMid(const CString& sName)
{
	CGangjeBase GangBase;
	long i = 0;
	while(1)
	{ 
		if(GangBase.m_sProcessMid[i].IsEmpty()) break;
		if(GangBase.m_sProcessMid[i] == sName)
		{
			m_nProcessMid = i;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

CString CBMRecord::GetStrProcessBig() const
{
//	if(m_nProcessBig==1)
//		return m_sMarkGrandAssem;

	CGangjeBase GangBase;
	return GangBase.m_sProcessBig[m_nProcessBig];
}

CString CBMRecord::GetStrProcessMid() const
{
	CGangjeBase GangBase;
	return GangBase.m_sProcessMid[m_nProcessMid];
}


int CBMRecord::CompareSizeType(const CBMRecord* pRec) const
{
	if(m_nSizeType		!= pRec->m_nSizeType)		return m_nSizeType		> pRec->m_nSizeType		 ? 1 : -1;
	if(m_nSubEleName != pRec->m_nSubEleName)		return m_nSubEleName > pRec->m_nSubEleName ? 1 : -1;
	if(m_sMaterial   != pRec->m_sMaterial)			return m_sMaterial	  < pRec->m_sMaterial	? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)			return m_vJewon.y    > pRec->m_vJewon.y	? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)			return m_vJewon.x	  > pRec->m_vJewon.x	? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)			return m_vJewon.z    > pRec->m_vJewon.z	? 1 : -1;
	if(m_nCountMake  != pRec->m_nCountMake)			return m_nCountMake  > pRec->m_nCountMake	? 1 : -1;
	if(m_dStationRec != pRec->m_dStationRec)		return m_dStationRec > pRec->m_dStationRec ? 1 : -1;	
	return 0;
}
