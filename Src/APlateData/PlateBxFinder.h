// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BxFinder.h: interface for the CPlateBxFinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEBXFINDER_H__20BA048D_013A_423C_9A19_1EA47EEFF67D__INCLUDED_)
#define AFX_PLATEBXFINDER_H__20BA048D_013A_423C_9A19_1EA47EEFF67D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateGirderApp;
class CPlateBasicIndex;

class AFX_EXT_CLASS CPlateBxFinder
{	
// Construtor
public:
	CPlateBxFinder(CPlateGirderApp* pGir);
	CPlateBxFinder(CPlateBxFinder* pFinder);
	virtual ~CPlateBxFinder();

// Attributes
private:
	CPlateGirderApp*	m_pGir;
	__int64		m_BxFindFlag;
	POS			m_psFixPrev;
	POS			m_psFixNext;
	POS			m_psHead;
	POS			m_psTail;
	POS			m_psCurrent;
	POS			m_psRangeEnd;
	BOOL		m_bNextState;
	BOOL		m_bIncludePOSRangeStt;
	BOOL		m_bIncludePOSRangeEnd;
	long		m_nCount;
	DWORD		m_dwBothPos;

// Implementation
private:
	POS GetCurrentPOSToSearch(BOOL bStart) const;	
	POS First(__int64 bxFlag,BOOL bNext,POS psStart,POS psEnd,BOOL bIncludePOSRangeStt,BOOL bIncludePOSRangeEnd);	
	POS Next(long nCount);	
	POS NextOne();	


public:
	POS GetPOSFixHead() const		{ return m_psHead; }
	POS GetPOSFixTail() const		{ return m_psTail; }
	POS GetPOSFixPrev() const		{ return m_psFixPrev; }
	POS GetPOSFixNext() const		{ return m_psFixNext; }
	POS GetPOSFirst(__int64 bxFlag,POS psStart = NULL,POS psEnd = NULL,
			BOOL bIncludePOSRangeStt = TRUE,BOOL bIncludePOSRangeEnd = TRUE);
	POS GetPOSNext(long nCount = 1);	
	POS GetPOSLast(__int64 bxFlag,POS psStart = NULL,POS psEnd = NULL,
			BOOL bIncludePOSRangeStt = TRUE,BOOL bIncludePOSRangeEnd = TRUE);
	POS GetPOS() const			{ return m_psCurrent; }


	CPlateBxFinder& operator = (const CPlateBxFinder& obj);
	__int64 GetFindFlag() const { return m_BxFindFlag; }

	CPlateBasicIndex* GetBxFixHead() const;
	CPlateBasicIndex* GetBxFixTail() const;
	CPlateBasicIndex* GetBxFixPrev();
	CPlateBasicIndex* GetBxFixNext();
	CPlateBasicIndex* GetBxFirst(__int64 bxFlag,CPlateBasicIndex *pBxStart = NULL,CPlateBasicIndex *pBxEnd = NULL,
			BOOL bIncludePOSRangeStt = TRUE, BOOL bIncludePOSRangeEnd = TRUE);
	CPlateBasicIndex* GetBxNext(long nCount = 1);
	CPlateBasicIndex* GetBxLast(__int64 bxFlag,CPlateBasicIndex *pBxStart = NULL,CPlateBasicIndex *pBxEnd = NULL,
			BOOL bIncludePOSRangeStt = TRUE, BOOL bIncludePOSRangeEnd = TRUE);	
	CPlateBasicIndex* GetBx() const;	

	long GetCountBx()	const		{ return GetCountByFlag(m_BxFindFlag); }
	long GetCountByFlag(__int64 bxFlag) const;

	long GetCurrentCount() const	{ return m_nCount; }
	void SetIncludeBothPos(BOOL bInclude);
	void SetIncludeBothPos(BOOL bIncludeSttPos,BOOL bIncludeEndPos);
};



#endif // !defined(AFX_PLATEBXFINDER_H__20BA048D_013A_423C_9A19_1EA47EEFF67D__INCLUDED_)
