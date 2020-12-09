// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BMRecord.h: interface for the CBMRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMRECORD_H__BC968069_2BE4_4B54_B1C8_34C97869A803__INCLUDED_)
#define AFX_BMRECORD_H__BC968069_2BE4_4B54_B1C8_34C97869A803__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#define MAX_BM_COLUMN		45
#define MAX_COUNT_ELEMAIN	24
#define MAX_COUNT_ELESUB	63
#define MAX_COUNT_KINDNAME	25

class AFX_EXT_CLASS CBMRecord 
{
// Constructor 
public:
	CBMRecord(CBMRecord* pBM = NULL);
	virtual ~CBMRecord();

// Attributes
protected:
	long m_nEleName;			// 종류 번호(일반, 가로보...)
	long m_nSubEleName;			// 부재명   번호(상부플랜지, 하부플랜지...)
	long m_nProcessBig;			// 대조립 번호
	long m_nProcessMid;			// 중조립 번호

public:
	DWORD	m_dwSummaryFlag;	// 단위 RECORD가 아닌 합계 DATA (소조립,중조립,단면타입)
	CString m_sSummaryMark;		// 제작도의 집계를 위한 구분마크
	DWORD m_dwFlag;				//
	DWORD m_dwDisableFlag;		//
	long m_nKindName;			// 부재별 종류(MATERIAL_KIND_PLATE=0, MATERIAL_KIND_BOLT=1, MATERIAL_KIND_BOLT2NUT=2 등
	long m_nMarkBuje;			// 부재마크
	long m_nCountMake;			//
	long m_nGirder;				// 거더번호
	long m_nSplice;				// 현장이음번호
	long m_nQty;				// 갯수
	long m_nFirstClass;			// 재료표상의 1차분류 (대우타입의 재료표에서 사용... -> 삭제검토)
	long m_nSectionNumber;		// 
	
	CVector m_vJewon;			// 부재제원(빔제원 시 dH, dT1, dT2)
	CVector m_vJewonAngle;		// dB, dT2
	CVector m_vJewonBasic;		//
	double	m_dAreaBuje;		// 부재면적
	double  m_dUWeight;			// 단위중량
	double  m_dTWeight;			// 전체무게
	double  m_dTWeightBasic;	//
	double	m_dStationRec;		//	

	CString m_sMaterial;		// 부재재질
	// NEW ADD
	CString m_sKindName;		// 부재별 종류
	CString m_sRefDWG;			// 참조도면명
	CString m_sUsage;			// 사용용도
	double  m_dFactorArea;		//
	double  m_dFactorAreaBasic; //
	double  m_dThickSecond;		//
	double  m_dWidthSecond;		//
	long m_nSizeType;			// 사이즈 종류 : 1:대형,2:소형,3:기타 2006.5.24

// Operation
public:
	CBMRecord& operator=(const CBMRecord &obj);

// Implementation
public:
	CString GetStrGirder(long nStart=0) const;	// 거더이름
	CString GetStrSplice(long nStart=0) const;	// 현장이음이름
	CString GetStrSectionType() const;			// 단면이름
	CString GetStrEleName() const;				// 종류이름
	CString GetStrSubEleName() const;			// 부재이름
	CString GetStrProcessBig() const;			//
	CString GetStrProcessMid() const;			//

	CString GetStrQty() const;					// 갯수명칭
	CString GetStrWidth(long nRound=0) const;	// 폭
	CString GetStrThick(long nRound=0) const;	// 두께
	CString GetStrLength(long nRound=0) const;	// 길이
	CString GetStrUWeight(long nRound=3) const;		// 단위중량
	CString GetStrTWeight(BOOL bUnitTon=FALSE, long nRound=3) const;	// 전체중량
	CString GetStrBujeKindName() const;			// 부재별 종류이름	
	CString GetStrWidthBasic(long nRound=0) const;	//	
	CString GetStrThickBasic(long nRound=0) const;	//
	CString GetStrLengthBasic(long nRound=0) const;	//
	CString GetStrTWeightBasic(BOOL bUnitTon=FALSE, long nRound=3) const;	//
	//
	CString GetStrMaterial() const;		// 재질명칭
	
	void SetStrSectionType(const CString& sName);	// 단면이름 설정
	void SetStrEleName(const CString& sEleName,long nSecTypeNumber);	// 	종류설정
	void SetStrEleName(const CString& sEleName);						// 종류설정
	void SetStrSubEleName(const CString& sName);	// 부재명설정
	void SetStrProcessBig(const CString& sName);	// 
	void SetStrProcessMid(const CString& sName);	//

	void SetNormal(BOOL bNormal);
	BOOL IsNormal() const;
	BOOL IsSameRecord(const CBMRecord* pRec,DWORD exceptionFlag = 0) const;
	void InitData();

	int CompareSang(const CBMRecord* pRec) const;				// [01]기본도의 강재집계표
	int CompareStandardSang(const CBMRecord* pRec) const;		// [02]표준집계표	
	int CompareStandardShopType2(const CBMRecord* pRec) const;	// [03]표준집계표(제작용)
	int CompareThick(const CBMRecord* pRec) const;				// [04]강종별 두께별집계표
	int CompareThickSang(const CBMRecord* pRec) const;			// [04]강종별 두께별집계표
	int CompareBujeMarkSang(const CBMRecord* pRec) const;		// [05]부재마크별 상세집계표	
	int CompareBujeMarkDeduct(const CBMRecord* pRec) const;		// [07]부재마크별 공제집계표
	int CompareCar(const CBMRecord* pRec) const;				// [08]차량적재 상세집계표
	int CompareCarSection(const CBMRecord* pRec) const;			// [08]차량적재 상세집계표
	int CompareCarAssembly(const CBMRecord* pRec) const;		// [08]차량적재 상세집계표
	int CompareCarTotal(const CBMRecord* pRec) const;			// [09]차량적재 총괄집계표
	int CompareGrandAssemblySang(const CBMRecord* pRec) const;	// [10]대조립별 상세집계표			
	int CompareAssemblyTotal(const CBMRecord* pRec) const;		// [15]소조립별 총괄집계표	
	int CompareAssemblyFact(const CBMRecord* pRec) const;		// [17]소조립별 공장제작용집계표
	int CompareAssemblyField(const CBMRecord* pRec) const;		// [18]소조립별 현장설치용집계표
	int CompareSectionComp(const CBMRecord* pRec) const;		// [19]단면별 집계표(기본도정산용)
	int CompareBujeComp(const CBMRecord* pRec) const;			// [20]부재별 집계표(기본도정산용)
	int CompareBasicComp(const CBMRecord* pRec) const;			// [21]기본도 정산용 집계표
	//
	int CompareSectionType(const CBMRecord* pRec) const;
	int CompareBuje(const CBMRecord* pRec) const;
	int CompareSizeType(const CBMRecord* pRec) const;

	long GetDivideEleNameNum(const CString& sName) const;
	CString GetDivideEleNameStr(const CString& sName) const;
	CString GetStrSteelBujeKindName()	const;					// 형강타입 부재이름 리턴

};

#endif // !defined(AFX_BMRECORD_H__BC968069_2BE4_4B54_B1C8_34C97869A803__INCLUDED_)
