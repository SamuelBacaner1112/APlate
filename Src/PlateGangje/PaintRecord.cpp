// PaintRecord.cpp: implementation of the CPaintRecord class.
//
//////////////////////////////////////////////////////////////////////
  
#include "stdafx.h"
#include "GangjeBase.h"
#include "PaintRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaintRecord::CPaintRecord(CPaintRecord *pRec)
{
	if(pRec)
		*this = *pRec;
	else
	{
		m_nMarkBuje			= -1;	// 부재마크
		m_nMarkGrandAssem	= -1;	// 대조립마크
		m_nMarkMidAssem		= -1;	// 중조립마크
		m_nMarkAssem		= -1;	// 소조립마크
		m_nMarkSection		= -1;	// 단면마크
		m_nMarkSectionBasic	= -1;	// 단면마크
		m_nGirder			= -1;	// 거더번호 
		m_nSplice			= -1;	// 현장이음 번호 
		m_nSectionNumber	= -1;
		m_nProcessMid		= -1;
		m_nProcessBig		= -1;

		m_sMarkBuje			= _T("");
		m_sMarkGrandAssem	= _T("");
		m_sMarkMidAssem		= _T("");
		m_sMarkAssem		= _T("");
		m_sMarkSection		= _T("");
		m_sMarkSectionBasic	= _T("");
		m_sRefDWG			= _T("");
		m_sUsage			= _T("");
		m_sMaterial			= _T("");

		m_vJewon.SetNull();			// 제원
		m_vJewonAngle		= CVector(1,1,1);
		m_dPlateArea		= 0.0;	// 면적(W,L)
		m_dPerimeter		= 0.0;	// 둘레(W,L)
		m_dUWeight			= 0.0;
		m_dTWeight			= 0.0;
		for(long i=0; i<PAINT_ITEM_MAX; i++)
			m_dPaintArea[i] = 0;
		m_nQty				= 0;	// 수량
		m_nCountMake		= -1;
	}
}
 
CPaintRecord::~CPaintRecord()
{

}

int CPaintRecord::CompareAssembly(const CPaintRecord* pRec) const
{
	if(m_nMarkAssem != pRec->m_nMarkAssem)		return m_nMarkAssem  > pRec->m_nMarkAssem  ? 1 : -1;
	if(m_sMarkAssem != pRec->m_sMarkAssem)		return m_sMarkAssem  > pRec->m_sMarkAssem  ? 1 : -1;
	if(m_nMarkBuje  != pRec->m_nMarkBuje)		return m_nMarkBuje   > pRec->m_nMarkBuje   ? 1 : -1;
	if(m_sMarkBuje  != pRec->m_sMarkBuje)		return m_sMarkBuje   > pRec->m_sMarkBuje   ? 1 : -1;
	return 0;
}

int CPaintRecord::CompareAssemblyTotal(const CPaintRecord* pRec) const
{
	if(m_nMarkAssem != pRec->m_nMarkAssem)		return m_nMarkAssem  > pRec->m_nMarkAssem  ? 1 : -1;
	if(m_sMarkAssem != pRec->m_sMarkAssem)		return m_sMarkAssem  > pRec->m_sMarkAssem  ? 1 : -1;
	return 0;
}

int CPaintRecord::CompareGrandAssembly(const CPaintRecord* pRec) const
{
	if(m_nMarkGrandAssem  != pRec->m_nMarkGrandAssem)	return m_nMarkGrandAssem  > pRec->m_nMarkGrandAssem  ? 1 : -1;
	if(m_sMarkGrandAssem  != pRec->m_sMarkGrandAssem)	return m_sMarkGrandAssem  > pRec->m_sMarkGrandAssem  ? 1 : -1;
	if(m_nMarkMidAssem    != pRec->m_nMarkMidAssem)		return m_nMarkMidAssem    > pRec->m_nMarkMidAssem    ? 1 : -1;
	if(m_sMarkMidAssem    != pRec->m_sMarkMidAssem)		return m_sMarkMidAssem    > pRec->m_sMarkMidAssem    ? 1 : -1;
	if(m_nMarkAssem != pRec->m_nMarkAssem)		return m_nMarkAssem  > pRec->m_nMarkAssem  ? 1 : -1;
	if(m_sMarkAssem != pRec->m_sMarkAssem)		return m_sMarkAssem  > pRec->m_sMarkAssem  ? 1 : -1;
	if(m_nMarkBuje  != pRec->m_nMarkBuje)		return m_nMarkBuje   > pRec->m_nMarkBuje   ? 1 : -1;
	if(m_sMarkBuje  != pRec->m_sMarkBuje)		return m_sMarkBuje   > pRec->m_sMarkBuje   ? 1 : -1;
	return 0;
}

int CPaintRecord::CompareGrandAssemblyTotal(const CPaintRecord* pRec) const
{
	if(m_nMarkGrandAssem  != pRec->m_nMarkGrandAssem)	return m_nMarkGrandAssem  > pRec->m_nMarkGrandAssem  ? 1 : -1;
	if(m_sMarkGrandAssem  != pRec->m_sMarkGrandAssem)	return m_sMarkGrandAssem  > pRec->m_sMarkGrandAssem  ? 1 : -1;
	return 0;
}

int CPaintRecord::CompareSang(const CPaintRecord* pRec) const
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

int CPaintRecord::CompareBoltSang(const CPaintRecord* pRec) const
{
	if(m_nMarkGrandAssem != pRec->m_nMarkGrandAssem)	return m_nMarkGrandAssem > pRec->m_nMarkGrandAssem ? 1 : -1;
	if(m_sMarkGrandAssem != pRec->m_sMarkGrandAssem)	return m_sMarkGrandAssem > pRec->m_sMarkGrandAssem ? 1 : -1;
	if(m_nMarkBuje		 != pRec->m_nMarkBuje)			return m_nMarkBuje		 > pRec->m_nMarkBuje	   ? 1 : -1;
	if(m_sMarkBuje		 != pRec->m_sMarkBuje)			return m_sMarkBuje		 > pRec->m_sMarkBuje	   ? 1 : -1;
	if(m_sMaterial		 != pRec->m_sMaterial)			return m_sMaterial		 > pRec->m_sMaterial	   ? 1 : -1;
	if(m_vJewon.y		 != pRec->m_vJewon.y)			return m_vJewon.y		 > pRec->m_vJewon.y		   ? 1 : -1;
	if(m_vJewon.z		 != pRec->m_vJewon.z)			return m_vJewon.z		 > pRec->m_vJewon.z		   ? 1 : -1;
	return 0;
}

int CPaintRecord::CompareBoltTotal(const CPaintRecord* pRec) const
{
	if(m_sMaterial != pRec->m_sMaterial) return m_sMaterial > pRec->m_sMaterial ? 1 : -1;
	if(m_vJewon.y  != pRec->m_vJewon.y)	 return m_vJewon.y  > pRec->m_vJewon.y	? 1 : -1;
	if(m_vJewon.z  != pRec->m_vJewon.z)	 return m_vJewon.z  > pRec->m_vJewon.z	? 1 : -1;
	return 0;
}

int CPaintRecord::CompareMatAndThick(const CPaintRecord* pRec) const
{
//	if(m_nKindName		 != pRec->m_nKindName)			return m_nKindName		  > pRec->m_nKindName	? 1 : -1;
	if(m_sMaterial		 != pRec->m_sMaterial)			return m_sMaterial		  < pRec->m_sMaterial	? 1 : -1;
	if(m_vJewon.y		 != pRec->m_vJewon.y)			return m_vJewon.y		  < pRec->m_vJewon.y	? 1 : -1;
	if(m_nProcessBig	 != pRec->m_nProcessBig)		return m_nProcessBig	  > pRec->m_nProcessBig		? 1 : -1;

	if(m_nProcessBig==1 && pRec->m_nProcessBig==1 && m_nMarkBuje!=pRec->m_nMarkBuje)
		return m_nMarkBuje  > pRec->m_nMarkBuje ? 1 : -1;

	if(m_nProcessMid	 != pRec->m_nProcessMid)		return m_nProcessMid	  > pRec->m_nProcessMid		? 1 : -1;
	if(m_sMarkBuje		 != pRec->m_sMarkBuje)			return m_sMarkBuje		  > pRec->m_sMarkBuje		? 1 : -1;
	return 0;
}

int CPaintRecord::CompareBujeSang(const CPaintRecord* pRec) const
{
	if(m_nSubEleName != pRec->m_nSubEleName) return m_nSubEleName > pRec->m_nSubEleName ? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)	 return m_vJewon.y    > pRec->m_vJewon.y	? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)	 return m_vJewon.x    > pRec->m_vJewon.x	? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)	 return m_vJewon.z    > pRec->m_vJewon.z	? 1 : -1;
	return 0;
}

int CPaintRecord::CompareBujeTotal(const CPaintRecord* pRec) const
{
	if(m_nSubEleName != pRec->m_nSubEleName) return m_nSubEleName > pRec->m_nSubEleName ? 1 : -1;
	if(m_vJewon.y	 != pRec->m_vJewon.y)	 return m_vJewon.y    > pRec->m_vJewon.y	? 1 : -1;
	if(m_vJewon.x	 != pRec->m_vJewon.x)	 return m_vJewon.x    > pRec->m_vJewon.x	? 1 : -1;
	if(m_vJewon.z	 != pRec->m_vJewon.z)	 return m_vJewon.z    > pRec->m_vJewon.z	? 1 : -1;
	return 0;
}


BOOL CPaintRecord::IsSameRecord(const CPaintRecord* pRec,DWORD exceptionFlag/*=0*/) const
{
	if(this == pRec) return TRUE;

	if((exceptionFlag & 0x000001) == 0)	if(m_nGirder			!= pRec->m_nGirder)				return FALSE;
	if((exceptionFlag & 0x000002) == 0)	if(m_nSplice			!= pRec->m_nSplice)				return FALSE;
	if((exceptionFlag & 0x000004) == 0)	if(m_nKindName			!= pRec->m_nKindName)			return FALSE;
	if((exceptionFlag & 0x000008) == 0)	if(m_nEleName			!= pRec->m_nEleName)			return FALSE;
	if((exceptionFlag & 0x000010) == 0)	if(m_nSectionNumber		!= pRec->m_nSectionNumber)		return FALSE;
	if((exceptionFlag & 0x000020) == 0)	if(m_nSubEleName		!= pRec->m_nSubEleName)			return FALSE;
	if((exceptionFlag & 0x000040) == 0)	if(Round(m_vJewon.x,0)	!= Round(pRec->m_vJewon.x,0))	return FALSE;
	if((exceptionFlag & 0x000080) == 0)	if(Round(m_vJewon.y,0)	!= Round(pRec->m_vJewon.y,0))	return FALSE;
	if((exceptionFlag & 0x000100) == 0)	if(Round(m_vJewon.z,0)	!= Round(pRec->m_vJewon.z,0))	return FALSE;
	if((exceptionFlag & 0x000200) == 0)	if(m_nMarkBuje			!= pRec->m_nMarkBuje)			return FALSE;
	if((exceptionFlag & 0x000400) == 0)	if(m_sMarkAssem			!= pRec->m_sMarkAssem)			return FALSE;
	if((exceptionFlag & 0x000800) == 0)	if(m_sMarkSection		!= pRec->m_sMarkSection)		return FALSE;
	if((exceptionFlag & 0x001000) == 0)	if(m_sMarkBuje			!= pRec->m_sMarkBuje)			return FALSE;
	if((exceptionFlag & 0x002000) == 0)	if(m_nMarkGrandAssem	!= pRec->m_nMarkGrandAssem)		return FALSE;
	if((exceptionFlag & 0x004000) == 0)	if(m_sMarkGrandAssem	!= pRec->m_sMarkGrandAssem)		return FALSE;
	if((exceptionFlag & 0x008000) == 0)	if(m_sMaterial			!= pRec->m_sMaterial)			return FALSE;
	if((exceptionFlag & 0x010000) == 0)	if(m_nMarkAssem			!= pRec->m_nMarkAssem)			return FALSE;
	if((exceptionFlag & 0x020000) == 0)	if(m_nMarkMidAssem		!= pRec->m_nMarkMidAssem)		return FALSE;
	if((exceptionFlag & 0x040000) == 0)	if(m_sMarkMidAssem		!= pRec->m_sMarkMidAssem)		return FALSE;

	return TRUE;
}


CString CPaintRecord::GetStrGirder(long nStart/*=0*/) const
{
	CString sResult;
	sResult.Format("G%ld",m_nGirder+1+nStart);
	return sResult;
}

CString CPaintRecord::GetStrSplice(long nStart/*=0*/) const
{
	CString sResult;
	sResult.Format("S%ld",m_nSplice+1+nStart);
	return sResult;
}


//////////////////////////////////////////////////////////////////
CString CPaintRecord::GetStrEleName() const
{
	CGangjeBase GangBase;
	CString sResult = GangBase.m_sEleNameMain[m_nEleName];

	if(m_nSectionNumber > 0)
//	if(m_nSectionNumber >= 0)
	{
		sResult.Format("%s%ld",GangBase.m_sEleNameMain[m_nEleName],m_nSectionNumber);
	}

	return sResult;

}

CString CPaintRecord::GetStrSubEleName() const
{
	CGangjeBase GangBase;
	return GangBase.m_sEleNameSub[m_nSubEleName];
}

void CPaintRecord::SetStrEleName(const CString& sName,long nSecTypeNumber /*= -1*/)
{
	CGangjeBase GangBase;
	long i = 0;
	while(1)
	{
		if(GangBase.m_sEleNameMain[i].IsEmpty()) break;
		if(GangBase.m_sEleNameMain[i] == sName)
		{
			m_nEleName = i;
			m_nSectionNumber = nSecTypeNumber;
			return;
		}
		i++;
	}

	ASSERT(FALSE);
}

void CPaintRecord::SetStrEleName(const CString& sEleName)
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

void CPaintRecord::SetStrSubEleName(const CString& sName)
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

long CPaintRecord::GetDivideEleNameNum(const CString& sName) const
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

CString CPaintRecord::GetDivideEleNameStr(const CString& sName) const
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

void CPaintRecord::SetStrSectionType(const CString& sName)
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

CString CPaintRecord::GetStrSectionType() const
{
	CGangjeBase GangBase;
	return GangBase.m_sKindName[m_nKindName];
}



CString CPaintRecord::GetStrWidth(long nRound) const
{
	if(m_vJewon.x==0) return _T("");

	CString sResult;
	sResult = GetStringOutValue(m_vJewon.x,nRound);

	if(m_vJewonAngle.x !=1)
		sResult += "X" + GetStringOutValue(m_vJewonAngle.x,nRound);

	return sResult;
}
CString CPaintRecord::GetStrThick(long nRound) const
{
	if(m_vJewon.y==0) return _T("");

	CString sResult;
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
CString CPaintRecord::GetStrLength(long nRound) const
{
	if(m_vJewon.z==0) return _T("");

	CString sResult;
	sResult = GetStringOutValue(m_vJewon.z,nRound);
	return sResult;
}

double CPaintRecord::GetAreaAllSide()
{
	double W = m_vJewon.x;
	double T = m_vJewon.y;
	double L = m_vJewon.z;

	return W*L*2 + (W+L)*2*T;
}

CString CPaintRecord::GetStrProcessBig() const
{
//	if(m_nProcessBig==1)
//		return m_sMarkGrandAssem;

	CGangjeBase GangBase;
	return GangBase.m_sProcessBig[m_nProcessBig];
}

CString CPaintRecord::GetStrProcessMid() const
{
	CGangjeBase GangBase;
	return GangBase.m_sProcessMid[m_nProcessMid];
}

void CPaintRecord::SetStrProcessBig(const CString& sName)
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

void CPaintRecord::SetStrProcessMid(const CString& sName)
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
