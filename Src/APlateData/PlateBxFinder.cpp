// BxFinder.cpp: implementation of the CPlateBxFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateBxFinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateBxFinder::CPlateBxFinder(CPlateGirderApp* pGir)
{
	m_pGir = pGir;

	m_BxFindFlag = 0;
	m_psFixPrev = NULL;
	m_psFixNext = NULL;
	m_psHead = NULL;
	m_psTail = NULL;
	m_psCurrent = NULL;
	m_psRangeEnd = NULL;
	m_bNextState = TRUE;
	m_bIncludePOSRangeStt = TRUE;
	m_bIncludePOSRangeEnd = TRUE;
	m_nCount = 0;
	m_dwBothPos = 0;
}
CPlateBxFinder::CPlateBxFinder(CPlateBxFinder* pFinder)
{
	*this = *pFinder;
}
CPlateBxFinder& CPlateBxFinder::operator = (const CPlateBxFinder& obj)
{
	if(this == &obj) return *this;

	m_pGir = obj.m_pGir;
	m_BxFindFlag = obj.m_BxFindFlag;
	m_psFixPrev = obj.m_psFixPrev;
	m_psFixNext = obj.m_psFixNext;
	m_psHead = obj.m_psHead;
	m_psTail = obj.m_psTail;
	m_psCurrent = obj.m_psCurrent;
	m_psRangeEnd = obj.m_psRangeEnd;
	m_bNextState = obj.m_bNextState;
	m_bIncludePOSRangeStt = obj.m_bIncludePOSRangeStt;
	m_bIncludePOSRangeEnd = obj.m_bIncludePOSRangeEnd;
	m_nCount = obj.m_nCount;
	m_dwBothPos = obj.m_dwBothPos;
	return *this;
}

CPlateBxFinder::~CPlateBxFinder()
{

}

//입력 : 
//		 
//기능 : 
POS CPlateBxFinder::GetCurrentPOSToSearch(BOOL bStart) const
{
	// 끝에 다달은 경우
	if(m_psRangeEnd == m_psCurrent)
	{
		return bStart ? m_psCurrent : NULL; 
	} 

	// 시작Pos를 무조건 포함하는 경우
	if(bStart && (m_dwBothPos & 1))	return m_psCurrent;

	////////////////////////////////////////////////////////////////////////////////////////
	// position loop 시작
	POS pos = bStart ? m_psCurrent : m_pGir->GetPOSNext(m_psCurrent,m_bNextState ? 1 : -1);
	while(pos)
	{
		if(m_pGir->GetBx(pos)->IsState(m_BxFindFlag))
		{
			if(m_psRangeEnd == pos) 
				if(m_bIncludePOSRangeEnd == FALSE) // psRangeEnd 포함하지 않음
					return NULL;
			return pos;
		}
		// RangeEnd의 Flag가 현재 search하고 있는 flag와 다르면서 RangeEnd POS에 도착한 경우
		if(m_psRangeEnd == pos)
		{
			// 끝Pos를 무조건 포함하는 경우
			if(m_dwBothPos & 2)	return pos;
			return NULL; 
		}


		
		pos = m_pGir->GetPOSNext(pos,m_bNextState ? 1 : -1);
	}



	return NULL;
}


//입력 : 
//		 
//기능 : 
POS CPlateBxFinder::First(__int64 bxFlag,BOOL bNext,POS psStart,POS psEnd,
				   BOOL bIncludePOSRangeStt, BOOL bIncludePOSRangeEnd)
{
	if(bxFlag == 0)
		bxFlag = 0xFFFFFFFFFFFFFFFF;

	// Set m_BxFindFlag
	m_BxFindFlag = bxFlag;
	// Set m_psRangeEnd
	m_psRangeEnd = psEnd;
	// Set m_bNextState
	m_bNextState = bNext;

	m_psHead = NULL;
	m_psTail = NULL;



	if(m_dwBothPos & 1) 
	{
		bIncludePOSRangeStt = TRUE;
		m_psHead = psStart;
	}
	else// Set m_psHead
		for(POS ps = m_pGir->GetPOSHead(); ps ; ps = m_pGir->GetPOSNext(ps))
			if(m_pGir->GetBx(ps)->IsState(bxFlag))
			{	m_psHead = ps;	break; }

	if(m_dwBothPos & 2) 
	{
		bIncludePOSRangeEnd = TRUE;
		psEnd = psEnd;
	}
	else// Set m_psTail
		for(POS ps = m_pGir->GetPOSTail(); ps ; ps = m_pGir->GetPOSNext(ps,-1))
			if(m_pGir->GetBx(ps)->IsState(bxFlag))
			{	m_psTail = ps;	break; }



	// Set m_bIncludePOSRange
	m_bIncludePOSRangeStt = bIncludePOSRangeStt;
	m_bIncludePOSRangeEnd = bIncludePOSRangeEnd;

	//	Set m_psCurrent;
	if(psStart)
	{
		m_psCurrent = psStart;
//		if(bIncludePOSRangeStt == FALSE)
			m_psCurrent = GetCurrentPOSToSearch(bIncludePOSRangeStt);
	}
	else
	{
		m_psCurrent = bNext ? m_pGir->GetPOSHead() : m_pGir->GetPOSTail();
		m_psCurrent = GetCurrentPOSToSearch(TRUE);
	}

	// m_psFixPrev;
	m_psFixPrev = m_psCurrent;
	// m_psFixNext;
	m_psFixNext = GetCurrentPOSToSearch(FALSE);

	// m_nCount;
	m_nCount = 0;


	return m_psCurrent;
}

//입력 : 
//		 
//기능 : 
POS CPlateBxFinder::Next(long nCount)
{
	ASSERT(nCount >= 0);
	if(nCount == 0) return m_psCurrent;
	if(nCount == 1) return NextOne();

	POS psResult;
	for(long n = 0; n < nCount; n++)
	{
		psResult = NextOne();
		if(!psResult) return NULL;
	}

	return psResult;
}

//입력 : 
//		 
//기능 : 
POS CPlateBxFinder::NextOne()
{
//	if(m_psFixNext == NULL)		return NULL;

	// m_psFixPrev;
	m_psFixPrev = m_psCurrent;

	//	Set m_psCurrent;
	m_psCurrent = m_psFixNext;

	// m_psFixNext;
	m_psFixNext = GetCurrentPOSToSearch(FALSE);

	m_nCount++;

	return m_psCurrent;
}

// POS
//입력 : 
//		 
//기능 : 
POS CPlateBxFinder::GetPOSFirst(__int64 bxFlag,POS psStart /*=NULL*/,POS psEnd /*= NULL*/,
						 BOOL bIncludePOSRangeStt /*= TRUE*/,BOOL bIncludePOSRangeEnd /*= TRUE*/)
{
	return First(bxFlag,TRUE,psStart,psEnd,bIncludePOSRangeStt,bIncludePOSRangeEnd);
}
//입력 : 
//		 
//기능 : 
POS CPlateBxFinder::GetPOSNext(long nCount /*= 1*/)// GetBxFirst() 실행후에 실행해야함
{
	return Next(nCount);
}
//입력 : 
//		 
//기능 : 
POS CPlateBxFinder::GetPOSLast(__int64 bxFlag,POS psStart /*=NULL*/,POS psEnd /*= NULL*/,
						BOOL bIncludePOSRangeStt /*= TRUE*/,BOOL bIncludePOSRangeEnd /*= TRUE*/)
{
	return First(bxFlag,FALSE,psStart,psEnd,bIncludePOSRangeStt,bIncludePOSRangeEnd);
}

// Bx
//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBxFirst(__int64 bxFlag,CPlateBasicIndex *pBxStart /*=NULL*/,CPlateBasicIndex *pBxEnd /*= NULL*/,
					BOOL bIncludePOSRangeStt /*= TRUE*/, BOOL bIncludePOSRangeEnd /*= TRUE*/)
{
	return m_pGir->GetBx( First(bxFlag,TRUE,
		pBxStart ? pBxStart->GetPOS() : NULL,
		pBxEnd ? pBxEnd->GetPOS() : NULL,
		bIncludePOSRangeStt,
		bIncludePOSRangeEnd) );
}

// GetBxFirst() 실행후에 실행해야함
//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBxNext(long nCount /*= 1*/)
{
	return m_pGir->GetBx( Next(nCount) );
}

//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBxLast(__int64 bxFlag,CPlateBasicIndex *pBxStart /*= NULL*/,CPlateBasicIndex *pBxEnd /*= NULL*/,
					BOOL bIncludePOSRangeStt /*= TRUE*/,	BOOL bIncludePOSRangeEnd /*= TRUE*/)
{
	return m_pGir->GetBx( First(bxFlag,FALSE,
		pBxStart ? pBxStart->GetPOS() : NULL,
		pBxEnd ? pBxEnd->GetPOS() : NULL,
		bIncludePOSRangeStt,
		bIncludePOSRangeEnd) );
}

//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBx() const
{
	return m_pGir->GetBx( GetPOS() );
}
//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBxFixPrev()
{
	return m_pGir->GetBx( GetPOSFixPrev() );
}
//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBxFixNext()
{
	return m_pGir->GetBx( GetPOSFixNext() );
}
//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBxFixHead() const
{
	return m_pGir->GetBx( GetPOSFixHead() );
}
//입력 : 
//		 
//기능 : 
CPlateBasicIndex* CPlateBxFinder::GetBxFixTail() const
{
	return m_pGir->GetBx( GetPOSFixTail() );
}
//입력 : 
//		 
//기능 : 
long CPlateBxFinder::GetCountByFlag(__int64 bxFlag) const
{
	if(bxFlag == 0)	bxFlag = 0xFFFFFFFFFFFFFFFF;
	long nResult = 0;
	POS pos = m_psCurrent;
	while(pos)
	{
		if(m_pGir->GetBx(pos)->IsState(bxFlag))
			nResult++;

		if(pos == m_psRangeEnd) break;
		pos = m_pGir->GetPOSNext(pos, m_bNextState ? 1 : -1);
		if(m_bIncludePOSRangeEnd==FALSE)
			if(pos == m_psRangeEnd) break;
	}
	return nResult;

}

void CPlateBxFinder::SetIncludeBothPos(BOOL bInclude)
{
	m_dwBothPos = bInclude ? 3 : 0;
}
void CPlateBxFinder::SetIncludeBothPos(BOOL bIncludeSttPos,BOOL bIncludeEndPos)
{
	m_dwBothPos = 0;
	if(bIncludeSttPos)
		m_dwBothPos |= 1;
	if(bIncludeSttPos)
		m_dwBothPos |= 2;
}




