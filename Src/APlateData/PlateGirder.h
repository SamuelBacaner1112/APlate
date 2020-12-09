// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#ifndef  __PLATEGIRDER_H__
#define  __PLATEGIRDER_H__

typedef CTypedPtrArray<CObArray, CPlateBxObject*> CPlateLinkToBxArray;

#include "Shoe.h"

class CCamber;
class CPlateFactChain;
class CPlateCrossBeam;
class CWebHStiff;
class CJackupStiff;
class CJRib;
class CPlateSplice;
class CBracing;
class CSection;
class CHBracingGusset;
class CVStiff;
class CPlateFactChain;
class CRangePress;
///////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS  CPlateGirder : public CObject
{
// Constructor
public:
	CPlateGirder();
	CPlateGirder(CPlateBridge* pDB,CLineInfo* pLine,CPlateGirderApp* pLeft = NULL);
	virtual ~CPlateGirder();
	friend class CPlateBasicIndex;
	friend class CPlateBridge;

public:	
	enum SP_TYPE { SPLICE=0, WELD=1, TOTAL=2};	

	CLineInfo	    *GetLine()      const { return m_pLine; }
	CPlateBridge    *GetBridgeApp() const{ 	return m_pBridgeApp; }

	BOOL IsJustModified() const;
	BOOL IsModify() const;
	BOOL IsDataFilled() const;
	void SetJustModified(BOOL bModify);
	void SetEleLevel(BOOL bVal)	{ m_bEleLevel = bVal; }
	BOOL GetEleLevel()	{ return m_bEleLevel; }
	BOOL IsLineSameLeftOrRight() const ;

	void ReStandVStiff();

// Attributes
public:
	CPlateBridge *m_pBridgeApp;
	CLineInfo    *m_pLine;
	long m_nBoundLineInfo;
	BOOL m_bEleLevel;

	long m_SectSu[10];			
	//////////////////////////////////////////
	CTypedPtrArray <CObArray, CBracing*>		m_BracingArr;		// 수평브레이싱 
	CTypedPtrArray <CObArray, CSection*>		m_VStiffSectionArr;	// 단면 중 수직보강재 위치
	CTypedPtrArray <CObArray, CSection*>		m_CrossSectionArr;	// 단면 중 가로보, 수직브레이싱 위치
	CTypedPtrArray <CObArray, CHBracingGusset*>	m_HGussetArr;		// 수평브레이싱 이음판
	CTypedPtrArray <CObArray, CVStiff*>			m_JiJumVStiff;		// 지점보강재	
	CTypedPtrArray <CObArray, CWebHStiff*>		m_aWebHStiffArr;	// 거더 수평보강재
	CTypedPtrArray <CObArray, CJackupStiff*>	m_aJackupArr;		// 잭업보강재
	CTypedPtrArray <CObArray, CPlateFactChain*>	m_aFactChain[3];	// 공장이음
	CTypedPtrArray <CObArray, CCamber*>			m_pCamber;			// 캠버
	CTypedPtrArray <CObArray, CPlateSplice*>	m_pSplice;			// 현장이음판
	//	
	CShoe m_Shoe[JIJUMSUMAX];										// 슈
	
	CPtrList	m_BxList;	

protected:
	double	m_dHeightOnJijum[JIJUMSUMAX]	;	// 거더높이

public:
	CRangePress		*m_pRangePress;		// 압축, 인장구간
	CPlateGirderApp *m_pLeft;			// 현재거더기준 좌측거더
	CPlateGirderApp *m_pRight;			// 현재거더기준 우측거더
	CPlateSangSeDo  *m_pSangse;			// 상세도에 필요한 변수 및 피이스
	
	double	m_dGirderCenter					;	// 거더중심위치(선형기준)	
	long	m_nLineNum						;	// 선형번호
	long    m_nInstallVStiff;					// 양측, 좌측, 우측

// Implementation
	void SetPOSAndEachPointer();
protected:
	void SetLKGirderPointAndSelfCount(void* pTPA);
	void SetLKSideFactChain();		
	void SetEnTypeNumSplice();

public:
	CPlateGirder& operator=(const CPlateGirder& obj);

public:	
	virtual void Serialize(CArchive& ar);		
		//세팅 시에만 사용
	CPlateLinkToBxArray * GetLKArr(long nLK) const;
	void SetLKEndPOS(void* pTPA);
	void SetLKClearPOS(long nLK);
	BOOL CheckSelfData(BOOL bAssertUse);
	void SetBxPosLeftRight(__int64 bxLeftFlag); 
	CPlateBasicIndex* GetBx(POS pos) const{ if(!pos) return NULL; return (CPlateBasicIndex*)m_BxList.GetAt(pos);}	
	CPtrList& GetBxList()  { return m_BxList; }
	virtual CPlateBasicIndex* newBx(CPlateBasicIndex *pBx = NULL);
	void SetLine(CLineInfo* pLine) { m_pLine = pLine; }	
	void SetRightGirder(CPlateGirderApp* pRight) {m_pRight = pRight;}	
};

#endif  // __PLATEGIRDER_H__
