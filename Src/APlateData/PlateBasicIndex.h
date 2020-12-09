// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateBasicIndex.h: interface for the CPlateBasicIndex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEBASICINDEX_H__8A1E846C_8F1D_40FC_8BF2_9AFF530C3562__INCLUDED_)
#define AFX_PLATEBASICINDEX_H__8A1E846C_8F1D_40FC_8BF2_9AFF530C3562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



const long	LINKOBJECTSU = 30;
class CPlateBxObject;
 
// General flag
const __int64 BX_STT_GIRDER					= 0x0000000000000001;
const __int64 BX_END_GIRDER					= 0x0000000000000002;
const __int64 BX_STT_JOINT					= 0x0000000000000004;
const __int64 BX_END_JOINT					= 0x0000000000000008;
const __int64 BX_STT_BENDING_UPPER			= 0x0000000000000010;
const __int64 BX_STT_BENDING_LOWER			= 0x0000000000000020;
const __int64 BX_END_BENDING_UPPER			= 0x0000000000000040;
const __int64 BX_END_BENDING_LOWER			= 0x0000000000000080;
const __int64 BX_STT_BYUN_UPPER				= 0x0000000000000100;
const __int64 BX_STT_BYUN_LOWER				= 0x0000000000000200;
const __int64 BX_END_BYUN_LOWER				= 0x0000000000000400;
const __int64 BX_END_BYUN_UPPER				= 0x0000000000000800;
const __int64 BX_STT_SLAB					= 0x0000000000001000;
const __int64 BX_END_SLAB					= 0x0000000000002000;
const __int64 BX_VSTIFF						= 0x0000000000004000;
const __int64 BX_CROSSBEAM_VBRACING			= 0x0000000000008000;
const __int64 BX_HBRACINGGUSSET				= 0x0000000000010000;
const __int64 BX_JIJUMSTIFF					= 0x0000000000020000;
const __int64 BX_JACKUP						= 0x0000000000040000;
const __int64 BX_UPPERFLANGE				= 0x0000000000080000;
const __int64 BX_LOWERFLANGE				= 0x0000000000100000;
const __int64 BX_WEB						= 0x0000000000200000;
const __int64 BX_SPLICE						= 0x0000000000400000;
const __int64 BX_JIJUM    					= 0x0000000000800000;
const __int64 BX_LINE_P						= 0x0000000001000000;
const __int64 BX_LINE_V						= 0x0000000002000000;
const __int64 BX_LINE_VPYUN					= 0x0000000004000000;

// Group flag
const __int64 BX_BENDING =		BX_STT_BENDING_LOWER	|
								BX_STT_BENDING_UPPER	|
								BX_END_BENDING_LOWER	|
								BX_END_BENDING_UPPER;

const __int64 BX_BYUN =			BX_STT_BYUN_UPPER	|
								BX_END_BYUN_UPPER	|
								BX_STT_BYUN_LOWER	|
								BX_END_BYUN_LOWER;

const __int64 BX_FLANGE =		BX_UPPERFLANGE	|
								BX_LOWERFLANGE	|		
								BX_WEB;	

const __int64 BX_LINE =			BX_LINE_P		|
								BX_LINE_V		|
								BX_LINE_VPYUN;

const __int64 BX_BENDING_UPPER =BX_STT_BENDING_UPPER |
								BX_END_BENDING_UPPER;

const __int64 BX_BENDING_LOWER =BX_STT_BENDING_LOWER|
								BX_END_BENDING_LOWER;

const __int64 BX_COMMON_PLATEGIRDER = BX_STT_GIRDER	|
									  BX_END_GIRDER	|
									  BX_JACKUP		|
									  BX_JIJUMSTIFF	|
									  BX_VSTIFF		|
									  BX_CROSSBEAM_VBRACING;

const __int64 BX_PLATEHSTIFF = BX_STT_GIRDER | 
							   BX_VSTIFF     | 
							   BX_JIJUMSTIFF |
							   BX_CROSSBEAM_VBRACING  | 															
							   BX_SPLICE     |
							   BX_JACKUP;	

const __int64 BX_UPPER = BX_COMMON_PLATEGIRDER	|	//상판 작업시(중앙)
						 BX_SPLICE				|
						 BX_UPPERFLANGE			|
						 BX_BENDING;							

const __int64 BX_LOWER = BX_COMMON_PLATEGIRDER	|	//상판 작업시(중앙)
						 BX_SPLICE				|
						 BX_LOWERFLANGE			|
						 BX_BYUN;							
// Old define
const DWORD BI_DANMYUN			= 0x4;  // BI 짝수만
const DWORD BI_NO_DANMYUN		= 0x8;  // BI 홀수만
const DWORD BI_NORMAL			= 0xC;  // BI 모두
const DWORD BI_TAPERSTT			= 0x10; // Taper 경사부 시작만
const DWORD BI_TAPEREND			= 0x20; // Taper 경사부 끝만
const DWORD BI_TAPER			= 0x30; // Taper 모두
const DWORD BI_JACKUP			= 0x40; // JackUp 보강재
const DWORD BI_BYUN_TERM		= 0x80; // 변단면 꺾이는곳
const DWORD BI_JONGDAN_TERM		= 0x100;

class CPlateGirderApp;
class CPlateFactChain;
class CJRib;
//class CStringer;
class CJackupStiff;
class CWebHStiff;
class CPlateSplice;

#define RETURN_ASSERT(x,bAssert,s)	\
{if(bAssert) ASSERT(x);	else if(!(x)) { CPlateBasicIndex::m_sBxError+=s;CPlateBasicIndex::m_sBxError+="\r\n"; return FALSE;}}


class CSection;
class CVBracing;
class CHBracingGusset;
class AFX_EXT_CLASS CPlateBasicIndex : public CObject  
{
public:
	CPlateBasicIndex(CPlateBasicIndex *pBx = NULL);	
	virtual ~CPlateBasicIndex();

public:
	void Clear();
	__int64	GetBxFlagState() const { return m_BxFlag; }
	double	GetStation() const	{ return m_dStation; }
	CDPoint	GetAngle() const	{return m_vAngle;}
	//CDPoint	GetAngleCenterBx() const	{return GetBxCenter()->m_vAngle;}
	double	GetStationByScInBx(double dDisGir) const;
	long	GetNumberGirder() const;
	CString	GetNameSectionType(BOOL bFullStr=TRUE) const;
	long	GetNumberSectionType() const;
	virtual CString	GetNumberSectionTypeForOut(BOOL bCross = FALSE) const;
	long	GetNumberCrossBeam() const;
	double	GetHeightGirderByBx(BOOL bReal=TRUE) const;
	long	GetCompressTypeByComplex() const;
	long	GetNumberGirderEx() const;
	long	GetNumberSpliceEx() const;
	long	GetNumberSplice() const;
	long	GetLinePTType() const;
	void	AddFlag(__int64 nFlag) { m_BxFlag |= nFlag; }

	CPlateBasicIndex* GetBxMatchByVert(CPlateGirderApp *pGir, __int64 bxFlag=NULL) const;
	//CPlateBasicIndex* GetBxCenter() const;
	CPlateBasicIndex* GetBxLeft() const;
	CPlateBasicIndex* GetBxRight() const;
	CPlateBasicIndex* GetBxMax() const;
	CPlateBasicIndex* GetBxMin() const;

	CPlateGirderApp	*GetGirder() const { return m_pGir; }
	CPlateSplice	*GetSplice() const;
	CPlateCrossBeam	*GetCrossBeam() const;
	CPlateCrossBeam	*GetCrossBeamLeft() const;
	CPlateFactChain	*GetFactChain(long nMarkBujeFact,BOOL bNext=TRUE) const;		
	CWebHStiff	*GetWebHStiff() const;
	CJackupStiff *GetJackupStiff() const;	
	BOOL	IsCompressByJRib(BOOL bUpper) const;
	BOOL	IsCompressByJRibBoth(BOOL bUpper) const;
	BOOL	IsAbut() const;
	BOOL	IsAbutStt() const;
	BOOL	IsAbutEnd() const;
	BOOL	IsDanbuJijumStt() const;
	BOOL	IsDanbuJijumEnd() const;
	BOOL	IsDanbuJijum() const;
	BOOL	IsPier() const;
	BOOL	IsJijum() const;
	BOOL	IsState(__int64 bxFlag) const	{	return (m_BxFlag & bxFlag) ? TRUE : FALSE; }
	BOOL	IsFiller(__int64 nFlag, BOOL &bNext) const;
	BOOL	IsCrossBeamRight() const;
	BOOL	IsCrossBeamLeft() const;
	BOOL	IsHybrid() const;
	CPlateBasicIndex* GetBxInLeftGirderSection() const;
	CPlateBasicIndex* GetBxInRighGirderSection() const;
	POS		GetPOSInLeftGirderSection() const { return m_psLeftCross; }
	POS		GetPOSInRightGirderSection() const { return m_psRightCross; }

// Construtor
public:
	static CString m_sBxError;
	
	BOOL IsShift() const { return m_bShift; }
	BOOL IsJoint() const;
	void SetShift(BOOL bShift) { m_bShift = bShift; }

// Attributes
protected:
	long	m_nObjLinkSeq[LINKOBJECTSU];
	DWORD	m_dwFlag;
	POS		m_psSelf;
	POS		m_psLeftCross;
	POS		m_psRightCross;
	POS		m_psLeft;
	POS		m_psRight;
	//POS		m_psCenter;
	__int64 m_BxFlag;
	long	m_nMatchCol;		//protected
	CPlateGirderApp	*m_pGir;
	double	m_dStation;			//protected	
	CDPoint m_vAngle;			//protected
	BOOL m_bShift;				// 교번부지정 체크위치

public:


// Access method
public:
	long GetMatchCol() const { return m_nMatchCol; }

	void SetStation(double sta);
	void SetAngle(const CDPoint& vAng);
	void SetMatchCol(long nMatchCol);
	void SetPOSLeft(POS ps);
	void SetPOSRight(POS ps);
	//void SetPOSCenter(POS ps);

	void SetJustModified(BOOL bModify = TRUE);
	BOOL IsJustModified() const;
	void SetModify(BOOL bModify = TRUE);
	BOOL IsModify() const;
	void SetDataFilled(BOOL bFill = TRUE);
	BOOL IsDataFilled() const;



//세팅 시에만 사용(거더, 다이아프램 위치입력 등)
public:
	static CString GetErrorBxString();
	long	GetObjLinkSeq(long n) const;
	void	CopyLinkVariable(CPlateBasicIndex *pBx);
	BOOL	CheckSelfData(BOOL bAssertUse);
	void	ContextOutBx(CString& sContext) const;
	void	SetPOSSelf(POS psSelf);
	POS		GetPOS() const { return m_psSelf; }
	//POS		GetPOSCenter() const { return m_psCenter; }
	void	SetObjLinkSeq(long lkType,long nSeq);
	void	Create(CPlateGirderApp* pGir);
	CPlateGirderApp* GetCreate() const;
	CPlateBxObject	*GetObjectLink(long enumLK) const;
	void	SetState(__int64 bxFlag, BOOL bApply);
	void	Serialize(CArchive& ar);	
	CPlateBasicIndex& operator=(const CPlateBasicIndex& obj);

public:
	CBracing *GetBracing() const;
	CSection *GetSection() const;	
	CHBracingGusset *GetHBracingGusset() const;
	CVStiff *GetJijumStiff() const;


public:
	double GetOffsetFromLowerFlange();
	long GetNumberJijum();
	CString GetMarkString();
	CPlateBasicIndex* GetBxNext(__int64 bxFlag, long nCheckMatchCr=0);
	CPlateBasicIndex* GetBxPrev(__int64 bxFlag, long nCheckMatchCr=0);

	void SetPOSLeftCross(POS ps);
	void SetPOSRightCross(POS ps);
	
	CPlateBasicIndex* GetBxMatchByCrossBeam(CPlateGirderApp *pGir, long nReturnType=0) const;
	CPlateBasicIndex* GetBxMatchByCrossBeamByDir(CPlateGirderApp *pGir, long nDir) const;
	CPlateBasicIndex* GetBxMatchByVStiff(CPlateGirderApp *pGir) const;
	CPlateBasicIndex* GetBxMatchBySection(CPlateGirderApp *pGir) const;
	CPlateBasicIndex* GetBxCrossBeamByHGuBx(BOOL bLeftMatch, BOOL bPrevBx);
	CDPoint GetAngleSectionLeft() const;
	CDPoint GetAngleSectionRight() const;
	CDPoint GetAngleSection() const;
	CDPoint GetAngleSectionCrossVBracing() const;
	CSection* GetSectionLeft() const;		
};

#endif // !defined(AFX_PLATEBASICINDEX_H__8A1E846C_8F1D_40FC_8BF2_9AFF530C3562__INCLUDED_)
