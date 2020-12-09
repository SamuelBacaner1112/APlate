// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PaintRecord.h: interface for the CPaintRecord class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_PAINTRECORD_H__E2672109_10EA_468B_B9F2_F269BAA294C1__INCLUDED_)
#define AFX_PAINTRECORD_H__E2672109_10EA_468B_B9F2_F269BAA294C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const long OUTSIDE_FACT			=	0;	// 외부도장 (공장)
const long OUTSIDE_FIELD		=	1;	// 외부도장 (현장)
//const long INSIDE_FACT			=	2;	// 내부도장 (공장)
const long SPLICE_FACT			=	2;	// 이음판도장
const long BOLT_FIELD_OUTSIDE	=	3;	// 외부볼트도장
//const long BOLT_FIELD_INSIDE	=	5;	// 내부볼트도장
const long PAVE_FACT			=	4;	// 포장면도장
const long PAINT_ITEM_MAX		=   5;

#define MAX_PAINT_COLUMN	40

class AFX_EXT_CLASS CPaintRecord  
{
protected:
		
	long m_nEleName;		// 종류 번호 
	long m_nSubEleName;		// 부재 번호 
	long m_nProcessBig;
	long m_nProcessMid;

public:
	double GetAreaAllSide();
	CPaintRecord(CPaintRecord *pRec=NULL);
	virtual ~CPaintRecord();

// DATA SET
	long m_nKindName;
	long m_nMarkBuje;			// 부재마크
	long m_nMarkGrandAssem;		// 대조립마크
	long m_nMarkMidAssem;		// 중조립마크
	long m_nMarkAssem;			// 소조립마크
	long m_nMarkSection;		// 단면마크
	long m_nMarkSectionBasic;	// 단면마크
	long m_nGirder;				// 거더번호 
	long m_nSplice;				// 현장이음 번호 
	long m_nSectionNumber;

	CString m_sMarkBuje;
	CString m_sMarkGrandAssem;
	CString m_sMarkMidAssem;
	CString m_sMarkAssem;
	CString m_sMarkSection;
	CString m_sMarkSectionBasic;
	//
	CString m_sMaterial;
	CString m_sRefDWG;
	CString m_sUsage;

	CVector m_vJewon;		// 제원
	CVector m_vJewonAngle;		// dB, dT2
	double  m_dPlateArea;	// 면적(W,L)
	double	m_dPerimeter;	// 둘레(W,L)
	double  m_dPaintArea[PAINT_ITEM_MAX];
	double  m_dUWeight;
	double  m_dTWeight;
	long	m_nQty;			// 수량
	long	m_nCountMake;

	/////////////////////////////////////
	int CompareSang(const CPaintRecord* pRec) const;
	int CompareBujeSang(const CPaintRecord* pRec) const;
	int CompareBujeTotal(const CPaintRecord* pRec) const;
	int CompareAssembly(const CPaintRecord* pRec) const;
	int CompareAssemblyTotal(const CPaintRecord* pRec) const;
	int CompareGrandAssembly(const CPaintRecord* pRec) const;
	int CompareGrandAssemblyTotal(const CPaintRecord* pRec) const;
	int CompareMatAndThick(const CPaintRecord* pRec) const;
	int CompareBoltSang(const CPaintRecord* pRec) const;
	int CompareBoltTotal(const CPaintRecord* pRec) const;
	BOOL IsSameRecord(const CPaintRecord* pRec,DWORD exceptionFlag = 0) const;

	/////////////////////////////////////
	CString GetStrGirder(long nStart=0) const;
	CString GetStrSplice(long nStart=0) const;
	CString GetStrEleName() const;
	CString GetStrSubEleName() const;
	CString GetStrSectionType() const;
	CString GetStrProcessBig() const;
	CString GetStrProcessMid() const;

	void SetStrEleName(const CString& sEleName);
	void SetStrEleName(const CString& sName,long nSecTypeNumber);
	void SetStrSubEleName(const CString& sName);
	void SetStrSectionType(const CString& sName);
	void SetStrProcessBig(const CString& sName);
	void SetStrProcessMid(const CString& sName);

	long GetDivideEleNameNum(const CString& sName) const;
	CString GetDivideEleNameStr(const CString& sName) const;
	CString GetStrWidth(long nRound=0) const;
	CString GetStrThick(long nRound=0) const;
	CString GetStrLength(long nRound=0) const;
};
 
#endif // !defined(AFX_PAINTRECORD_H__E2672109_10EA_468B_B9F2_F269BAA294C1__INCLUDED_)
