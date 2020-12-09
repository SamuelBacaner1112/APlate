// WeldRecord.cpp: implementation of the CWeldRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "GangjeBase.h" 
#include "WeldRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeldRecord::CWeldRecord(CWeldRecord* pRec)
{
	if(pRec)
		*this = *pRec;
	else 
	{ 
		m_nKindName			= -1;
		m_nEleName			= -1;
		m_nSubEleName		= -1;
		m_nEleNameConnect	= -1;
		m_nSubEleNameConnect	= -1;
		m_nGirder			= -1;
		m_nSplice			= -1;
		m_nMarkGrandAssem	= -1;
		m_nMarkMidAssem		= -1;
		m_nMarkAssem		= -1;
		m_nMarkSection		= -1;
		m_nMarkSectionBasic	= -1;
		m_nMarkBuje			= -1;
		m_nSectionNumber	= -1;
		m_nSectionNumberConnect = -1;
		m_sMaterial			= _T("");
		m_sMarkGrandAssem	= _T("");
		m_sMarkMidAssem		= _T("");
		m_sMarkAssem		= _T("");
		m_sMarkSection		= _T("");
		m_sMarkSectionBasic	= _T("");
		m_sMarkBuje			= _T("");
		m_sRefDWG			= _T("");
		m_sUsage			= _T("");
		m_vJewon.SetNull();
		m_nQty				= 0;
		m_dLengthWeld		= 0;
		m_dThickWeld		= 0;
		m_sWeldType			= _T("");
		m_sImproveType		= _T("");
		m_sLayType			= _T("");
		m_nWeldType			= -1;
		m_nImproveType		= -1;
		m_nLayType			= -1;
		m_nMarkBujeConnect	= -1;
		m_dThickConnect		=  0;
	}
}

CWeldRecord::~CWeldRecord()
{

}

int CWeldRecord::CompareSang(const CWeldRecord* pRec) const
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

int CWeldRecord::CompareAssemblyTotal(const CWeldRecord* pRec) const 
{
	if(m_nMarkAssem    != pRec->m_nMarkAssem)	 return m_nMarkAssem    > pRec->m_nMarkAssem    ? 1 : -1;
	if(m_sMarkAssem    != pRec->m_sMarkAssem)	 return m_sMarkAssem    > pRec->m_sMarkAssem	? 1 : -1;

	if(m_nWeldType!=-1 && pRec->m_nWeldType!=-1)
	{
		if(m_nWeldType	  != pRec->m_nWeldType)		return m_nWeldType    > pRec->m_nWeldType  ? 1 : -1;
		if(m_nImproveType != pRec->m_nImproveType)		return m_nImproveType > pRec->m_nImproveType   ? 1 : -1;
		if(m_dThickWeld	  != pRec->m_dThickWeld)	return m_dThickWeld	  > pRec->m_dThickWeld ? 1 : -1;
		if(m_nLayType     != pRec->m_nLayType)		return m_nLayType     > pRec->m_nLayType   ? 1 : -1;
	}

	return 0;
}
int CWeldRecord::CompareAssembly(const CWeldRecord* pRec) const
{
	if(m_nMarkAssem    != pRec->m_nMarkAssem)	 return m_nMarkAssem    > pRec->m_nMarkAssem    ? 1 : -1;
	if(m_sMarkAssem    != pRec->m_sMarkAssem)	 return m_sMarkAssem    > pRec->m_sMarkAssem	? 1 : -1;
	if(m_nMarkBuje     != pRec->m_nMarkBuje)	 return m_nMarkBuje     > pRec->m_nMarkBuje     ? 1 : -1;
	if(m_sMarkBuje     != pRec->m_sMarkBuje)	 return m_sMarkBuje     > pRec->m_sMarkBuje	    ? 1 : -1;

	if(m_nWeldType!=-1 && pRec->m_nWeldType!=-1)
	{
		if(m_nWeldType	  != pRec->m_nWeldType)	  return m_nWeldType    > pRec->m_nWeldType ? 1 : -1;
		if(m_nImproveType != pRec->m_nImproveType)	  return m_nImproveType > pRec->m_nImproveType  ? 1 : -1;
		if(m_dThickWeld	  != pRec->m_dThickWeld)  return m_dThickWeld   > pRec->m_dThickWeld ? 1 : -1;
		if(m_nLayType	  != pRec->m_nLayType)	  return m_nLayType     > pRec->m_nLayType  ? 1 : -1;
	}

	return 0;
}

int CWeldRecord::CompareAssemblySang(const CWeldRecord* pRec) const
{
	if(m_nMarkGrandAssem  != pRec->m_nMarkGrandAssem)	return m_nMarkGrandAssem  > pRec->m_nMarkGrandAssem  ? 1 : -1;
	if(m_sMarkGrandAssem  != pRec->m_sMarkGrandAssem)	return m_sMarkGrandAssem  > pRec->m_sMarkGrandAssem	? 1 : -1;
	if(m_nMarkMidAssem    != pRec->m_nMarkMidAssem)		return m_nMarkMidAssem    > pRec->m_nMarkMidAssem  ? 1 : -1;
	if(m_sMarkMidAssem    != pRec->m_sMarkMidAssem)		return m_sMarkMidAssem    > pRec->m_sMarkMidAssem	? 1 : -1;

	return CompareAssembly(pRec);
}

int CWeldRecord::CompareWeldThick(const CWeldRecord* pRec) const 
{
	if(m_nWeldType!=-1 && pRec->m_nWeldType!=-1)
	{
		if(m_nWeldType	  != pRec->m_nWeldType)		return m_nWeldType    > pRec->m_nWeldType	 ? 1 : -1;
		if(m_nImproveType != pRec->m_nImproveType)	return m_nImproveType > pRec->m_nImproveType ? 1 : -1;
		if(m_nLayType	  != pRec->m_nLayType)		return m_nLayType     > pRec->m_nLayType	 ? 1 : -1;
		if(m_dThickWeld	  != pRec->m_dThickWeld)	return m_dThickWeld   > pRec->m_dThickWeld	 ? 1 : -1;
	}
	//
	if(m_nMarkAssem    != pRec->m_nMarkAssem) return m_nMarkAssem > pRec->m_nMarkAssem ? 1 : -1;
	if(m_sMarkAssem    != pRec->m_sMarkAssem) return m_sMarkAssem > pRec->m_sMarkAssem ? 1 : -1;
	if(m_nMarkBuje     != pRec->m_nMarkBuje)  return m_nMarkBuje  > pRec->m_nMarkBuje  ? 1 : -1;
	if(m_sMarkBuje     != pRec->m_sMarkBuje)  return m_sMarkBuje  > pRec->m_sMarkBuje  ? 1 : -1;
	if(m_nGirder	   != pRec->m_nGirder)	  return m_nGirder	  > pRec->m_nGirder	   ? 1 : -1;
	if(m_nSplice	   != pRec->m_nSplice)	  return m_nSplice	  > pRec->m_nSplice	   ? 1 : -1;

	return 0;
}

int CWeldRecord::CompareWeldType(const CWeldRecord* pRec) const
{
	if(m_nWeldType!=-1 && pRec->m_nWeldType!=-1)
	{
		if(m_nWeldType	  != pRec->m_nWeldType)	  return m_nWeldType    > pRec->m_nWeldType  ? 1 : -1;
		if(m_nImproveType != pRec->m_nImproveType)	  return m_nImproveType > pRec->m_nImproveType   ? 1 : -1;
		if(m_nLayType	  != pRec->m_nLayType)	  return m_nLayType     > pRec->m_nLayType   ? 1 : -1;
		if(m_dThickWeld	  != pRec->m_dThickWeld)  return m_dThickWeld   > pRec->m_dThickWeld ? 1 : -1;
	}

	if(m_nEleName		!= pRec->m_nEleName)		return m_nEleName		> pRec->m_nEleName		 ? 1 : -1;
	if(m_nSectionNumber != pRec->m_nSectionNumber)	return m_nSectionNumber > pRec->m_nSectionNumber ? 1 : -1;
	if(m_nSubEleName    != pRec->m_nSubEleName)		return m_nSubEleName    > pRec->m_nSubEleName	 ? 1 : -1;

	return 0;
}


BOOL CWeldRecord::IsSameRecord(const CWeldRecord* pRec,DWORD exceptionFlag/*=0*/) const
{
	if(this == pRec) return TRUE;

	if((exceptionFlag&0x000001)==0)	if(m_nGirder			!= pRec->m_nGirder)				return FALSE;
	if((exceptionFlag&0x000002)==0)	if(m_nSplice			!= pRec->m_nSplice)				return FALSE;
	if((exceptionFlag&0x000004)==0)	if(m_nKindName			!= pRec->m_nKindName)			return FALSE;
	if((exceptionFlag&0x000008)==0)	if(m_nEleName			!= pRec->m_nEleName)			return FALSE;
	if((exceptionFlag&0x000010)==0)	if(m_nSectionNumber		!= pRec->m_nSectionNumber)		return FALSE;
	if((exceptionFlag&0x000020)==0)	if(m_nSubEleName		!= pRec->m_nSubEleName)			return FALSE;
	// 마크별 비교
	if((exceptionFlag&0x000040)==0)	if(m_sMarkGrandAssem	!= pRec->m_sMarkGrandAssem)		return FALSE;
	if((exceptionFlag&0x000080)==0)	if(m_sMarkMidAssem		!= pRec->m_sMarkMidAssem)		return FALSE;
	if((exceptionFlag&0x000100)==0)	if(m_sMarkAssem			!= pRec->m_sMarkAssem)			return FALSE;
	if((exceptionFlag&0x000200)==0)	if(m_sMarkBuje			!= pRec->m_sMarkBuje)			return FALSE;
	if((exceptionFlag&0x000400)==0)	if(m_sMarkSection		!= pRec->m_sMarkSection)		return FALSE;
	if((exceptionFlag&0x004000)==0)	if(m_nMarkBuje			!= pRec->m_nMarkBuje)			return FALSE;
	// 제원비교 
	if((exceptionFlag&0x000800)==0)	if(Round(m_vJewon.x,0)	!= Round(pRec->m_vJewon.x,0))	return FALSE;
	if((exceptionFlag&0x001000)==0)	if(Round(m_vJewon.y,0)	!= Round(pRec->m_vJewon.y,0))	return FALSE;
	if((exceptionFlag&0x002000)==0)	if(Round(m_vJewon.z,0)	!= Round(pRec->m_vJewon.z,0))	return FALSE;
	// 용접별 비교
	if((exceptionFlag&0x008000)==0)	if(m_sWeldType			!= pRec->m_sWeldType)			return FALSE;
	if((exceptionFlag&0x010000)==0)	if(m_sImproveType		!= pRec->m_sImproveType)		return FALSE;
	if((exceptionFlag&0x020000)==0)	if(m_sLayType			!= pRec->m_sLayType)			return FALSE;
	if((exceptionFlag&0x040000)==0)	if(m_dThickWeld			!= pRec->m_dThickWeld)			return FALSE;
	// 부재마크(연결)
	if((exceptionFlag&0x080000)==0)	if(m_nMarkBujeConnect		!= pRec->m_nMarkBujeConnect)		return FALSE;
	if((exceptionFlag&0x100000)==0)	if(m_nEleNameConnect		!= pRec->m_nEleNameConnect)			return FALSE;
	if((exceptionFlag&0x200000)==0)	if(m_nSubEleNameConnect		!= pRec->m_nSubEleNameConnect)		return FALSE;
	if((exceptionFlag&0x400000)==0)	if(m_nSectionNumberConnect	!= pRec->m_nSectionNumberConnect)	return FALSE;

	return TRUE;
}


//////////////////////////////////////////////////
// CBMRecord와 유사한 함수들
// 정리후 CGangjeBase에서 통합......................
CString CWeldRecord::GetStrGirder(long nStart/*=0*/) const
{
	CString sResult;
	sResult.Format("G%ld",m_nGirder+1+nStart);
	return sResult;
}

CString CWeldRecord::GetStrSplice(long nStart/*=0*/) const
{
	CString sResult;
	sResult.Format("S%ld",m_nSplice+1+nStart);
	return sResult;
}

CString CWeldRecord::GetStrSectionType() const
{
	CGangjeBase GangBase;
	return GangBase.m_sKindName[m_nKindName];
}

CString CWeldRecord::GetStrEleName() const
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

CString CWeldRecord::GetStrSubEleName() const
{
	CGangjeBase GangBase;
	return GangBase.m_sEleNameSub[m_nSubEleName];
}

CString CWeldRecord::GetStrWidth(long nRound) const
{
	CString sResult;
	if(nRound==0)	sResult.Format("%4.0lf",m_vJewon.x);
	if(nRound==1)	sResult.Format("%4.1lf",m_vJewon.x);
	if(nRound==2)	sResult.Format("%4.2lf",m_vJewon.x);
	if(nRound==3)	sResult.Format("%4.3lf",m_vJewon.x);
	if(nRound>=4)	sResult.Format("%4.4lf",m_vJewon.x);
	return sResult;
}

CString CWeldRecord::GetStrThick(long nRound) const
{
	CString sResult;
	if(nRound==0)	sResult.Format("%2.0lf",m_vJewon.y);
	if(nRound==1)	sResult.Format("%2.1lf",m_vJewon.y);
	if(nRound==2)	sResult.Format("%2.2lf",m_vJewon.y);
	if(nRound==3)	sResult.Format("%2.3lf",m_vJewon.y);
	if(nRound>=4)	sResult.Format("%2.4lf",m_vJewon.y);
	return sResult;
}

CString CWeldRecord::GetStrLength(long nRound) const
{
	CString sResult;
	if(nRound==0)	sResult.Format("%5.0lf",m_vJewon.z);
	if(nRound==1)	sResult.Format("%5.1lf",m_vJewon.z);
	if(nRound==2)	sResult.Format("%5.2lf",m_vJewon.z);
	if(nRound==3)	sResult.Format("%5.3lf",m_vJewon.z);
	if(nRound>=4)	sResult.Format("%5.4lf",m_vJewon.z);
	return sResult;
}



void CWeldRecord::SetStrSectionType(const CString& sName)
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

	ASSERT(FALSE);
}

long CWeldRecord::GetDivideEleNameNum(const CString& sName) const
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

CString CWeldRecord::GetDivideEleNameStr(const CString& sName) const
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

void CWeldRecord::SetStrEleName(const CString& sEleName,long nSecTypeNumber)
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

void CWeldRecord::SetStrEleName(const CString& sEleName)
{
	CGangjeBase GangBase;

//	CString sName = GetDivideEleNameStr(sEleName);
//	long nNum = GetDivideEleNameNum(sEleName);

	long i = 0;
	while(1)
	{
		if(GangBase.m_sEleNameMain[i].IsEmpty()) break;
		if(GangBase.m_sEleNameMain[i] == sEleName/*sName*/)
		{
			m_nEleName = i;
//			m_nSectionNumber = nNum;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

void CWeldRecord::SetStrSubEleName(const CString& sName)
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

void CWeldRecord::SetStrEleConnectName(const CString& sEleName)
{
	CGangjeBase GangBase;

//	CString sName = GetDivideEleNameStr(sEleName);
//	long nNum = GetDivideEleNameNum(sEleName);

	long i = 0;
	while(1)
	{
		if(GangBase.m_sEleNameMain[i].IsEmpty()) break;
		if(GangBase.m_sEleNameMain[i] == sEleName/*sName*/)
		{
			m_nEleNameConnect = i;
//			m_nSectionNumberConnect = nNum;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

void CWeldRecord::SetStrSubEleConnectName(const CString& sName)
{
	CGangjeBase GangBase;
	long i = 0;
	while(1)
	{ 
		if(GangBase.m_sEleNameSub[i].IsEmpty()) break;
		if(GangBase.m_sEleNameSub[i] == sName)
		{
			m_nSubEleNameConnect = i;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

CString CWeldRecord::GetStrEleConnectName() const
{
	CGangjeBase GangBase;
	CString sResult = GangBase.m_sEleNameMain[m_nEleNameConnect];

	if(m_nSectionNumberConnect > 0)
	{
		sResult.Format("%s%ld",GangBase.m_sEleNameMain[m_nEleNameConnect],
			           m_nSectionNumberConnect);
	}

	return sResult;

}

CString CWeldRecord::GetStrSubEleConnectName() const
{
	CGangjeBase GangBase;
	return GangBase.m_sEleNameSub[m_nSubEleNameConnect];
}
