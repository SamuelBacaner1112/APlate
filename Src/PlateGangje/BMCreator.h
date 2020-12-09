// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BMCreator.h: interface for the CBMCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMCREATOR_H__2C813849_F3E1_41CA_9673_27BB3A3273CB__INCLUDED_)
#define AFX_BMCREATOR_H__2C813849_F3E1_41CA_9673_27BB3A3273CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000

// 강제집계표 제작용    
const long MAKE_BM_SANG						=	0x00000001;	// [01]기본도의 강재집계표
const long MAKE_BM_STDANDARD_SANG			=	0x00000002;	// [02]표준집계표
const long MAKE_BM_STDANDARD_SHOP			=	0x00000004;	// [03]표준집계표(제작용)
const long MAKE_BM_STDANDARD_SHOP_TYPE2		=	0x00000008;	// [03]표준집계표(제작용)
const long MAKE_BM_THICK					=	0x00000010;	// [04]강종별 두께별집계표
const long MAKE_BM_THICK_SANG				=	0x00000020;	// [05]강종별 두께별집계표
const long MAKE_BM_BUJEMARK_SANG			=	0x00000040;	// [06]부재마크별 상세집계표
const long MAKE_BM_BUJEMARK_TOTAL			=	0x00000080; // [07]부재마크별 총괄집계표
const long MAKE_BM_BUJEMARK_DEDUCT			=	0x00000100; // [08]부재마크별 공제집계표
const long MAKE_BM_CAR						=	0x00000200;	// [09]차량적재 상세집계표
const long MAKE_BM_CARTOTAL					=	0x00000400;	// [10]차량적재 총괄집계표
const long MAKE_BM_GRAND_ASSEMBLY_SANG 		=	0x00000800;	// [11]대조립별 상세집계표
const long MAKE_BM_GRAND_ASSEMBLY_TOTAL		=	0x00001000;	// [12]대조립별 총괄집계표
const long MAKE_BM_MID_ASSEMBLY_SANG 		=	0x00002000;	// [13]중조립별 상세집계표
const long MAKE_BM_MID_ASSEMBLY_TOTAL		=	0x00004000;	// [14]중조립별 총괄집계표
const long MAKE_BM_ASSEMBLY_SANG			=	0x00008000;	// [15]소조립별 상세집계표
const long MAKE_BM_ASSEMBLY_TOTAL			=	0x00010000; // [16]소조립별 총괄집계표
const long MAKE_BM_ASSEMBLY_DEDUCT			=	0x00020000; // [17]소조립별 공제집계표
const long MAKE_BM_ASSEMBLY_FACT			=	0x00040000; // [18]소조립별 공장제작용집계표
const long MAKE_BM_ASSEMBLY_FIELD			=	0x00080000; // [19]소조립별 현장설치용집계표
const long MAKE_BM_SECTION_COMP				=	0x00100000; // [20]단면별 집계표(기본도정산용)
const long MAKE_BM_BUJE_COMP				=	0x00200000; // [21]부재별 집계표(기본도정산용)
const long MAKE_BM_BASIC_COMP				=	0x00400000;	// [22]기본도 정산용 집계표
// 강제집계표 설계용(????)
const long MAKE_BM_SECTIONTYPE				=	0x00800000;
const long MAKE_BM_BUJE						=	0x01000000;
const long MAKE_BM_BUJE_TOTAL				=	0x02000000;
const long MAKE_BM_DEDUCT_SANG				=	0x04000000; 
const long MAKE_BM_DEDUCT_BUJE				=	0x08000000; 
const long MAKE_BM_GIRDER_TOTAL				=	0x10000000; 
const long MAKE_BM_BUJEKIND_TOTAL			=	0x20000000; 

// TOTAL;
const long BM_SUMMARY_ASSEM		= 0x00000001;
const long BM_SUMMARY_ASSEM_MID	= 0x00000002;
const long BM_SUMMARY_ASSEM_BIG	= 0x00000004;
const long BM_SUMMARY_SECTION	= 0x00000008;

const DWORD WM_BM_PROGRESS	 =	WM_APP+2200;
class CDataManage;
class CBMRecord;
class CDataManage;
class CTStandard;


class AFX_EXT_CLASS CBMCreator  : public CThreadWork
{	
// Constructor
public:
	CBMCreator(CDataManage *pMng);
	virtual ~CBMCreator();

// Attributes
protected:
	CDataManage* m_pDataManage;
	CPlateBridgeApp* m_pDB;

	CPtrList m_BMStardardList;	// 표준리스트

	long	m_nWorkValue;	// 0:생성, 1:tbl파일로드, 2:액셀파일로드
	CString	m_sPath;		// 
	DIMTYPE	m_nDimType;		// DimType
	long	m_nMakeFlag;	// MakeFlag

	DWORD	m_dwMakeMarkFlag;	// Mark Flag
	BOOL	m_bUnitMode;

public:
	// 강제집계표 제작용
	CPtrList m_BMList_Sang;					// [01]기본도의 강재집계표
	CPtrList m_BMList_StandardSang;			// [02]표준집계표
	CPtrList m_BMList_StandardShop;			// [03]표준집계표(제작용)
	CPtrList m_BMList_StandardShopType2;	// [03]표준집계표(제작용동운)(2002.09.23)
	CPtrList m_BMList_Thick;				// [04]강종별 두께별집계표
	CPtrList m_BMList_ThickSang;			// [04]두께별 상세집계표 (2002.09.23)
	CPtrList m_BMList_BujeMarkSang;			// [05]부재마크별 상세집계표
	CPtrList m_BMList_BujeMarkTotal;		// [06]부재마크별 총괄집계표
	CPtrList m_BMList_BujeMarkDeduct;		// [07]부재마크별 공제집계표
	CPtrList m_BMList_Car;					// [08]차량적재 상세집계표
	CPtrList m_BMList_CarTotal;				// [09]차량적재 총괄집계표
	CPtrList m_BMList_GrandAssemblySang;	// [10]대조립별 상세집계표
	CPtrList m_BMList_GrandAssemblyTotal;	// [11]대조립별 총괄집계표
	CPtrList m_BMList_MidAssemblySang;		// [12]중조립별 상세집계표
	CPtrList m_BMList_MidAssemblyTotal;		// [13]중조립별 총괄집계표
	CPtrList m_BMList_AssemblySang;			// [14]소조립별 상세집계표
	CPtrList m_BMList_AssemblyTotal;		// [15]소조립별 총괄집계표
	CPtrList m_BMList_AssemblyDeduct;		// [16]소조립별 공제집계표
	CPtrList m_BMList_AssemblyFact;			// [17]소조립별 공장제작용집계표
	CPtrList m_BMList_AssemblyField;		// [18]소조립별 현장설치용집계표
	CPtrList m_BMList_SectionComp;			// [19]단면별 집계표(기본도정산용)
	CPtrList m_BMList_BujeComp;				// [20]부재별 집계표(기본도정산용)
	CPtrList m_BMList_BasicComp;			// [21]기본도 정산용 집계표
											
	// 강제집계표 설계용	
	CPtrList m_BMList_SectionType;			// 단면타입별 집계표
	CPtrList m_BMList_Buje;					// 부재별 집계표
	CPtrList m_BMList_BujeTotal;			// 부재별 총괄집계표
	CPtrList m_BMList_BujeKindTotal;		// 부재종류별 총괄집계표
	CPtrList m_BMList_DeductSang;			// 상세별공제
	CPtrList m_BMList_DeductBuje;			// 부재별공제
	CPtrList m_BMList_Girder;				// 거더별공제

	////////////////////////////////////////////////////////////////////////
	CPtrList m_BMUnitList;				// 단위재료표 생성을 위한 LIST
	CPtrList m_BMGangjeList;			// 단면별,SET별로 저장

// Implementation
protected:
	// 주판
	void DeductBMFactChain(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,long nMarkFactType);
	void DeductBMFlangeSkew(CPlateBasicIndex* pBx, BOOL bUpper);
	void DeductBMUpperFlangeBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx);
	void DeductBMLowerFlangeBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx);	
	void DeductBMWebTaper(CPlateBasicIndex* pBx);
	void DeductBMWebBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx);		
	// 수직보강재,수평보강재
	void DeductBMVStiffner(CPlateBasicIndex* pBx);
	void DeductBMHStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,BOOL bLeft);
	// 현장이음
	void DeductBMSpliceWeb(CPlateBasicIndex* pBx);
	void DeductBMSpliceUpperBoltHole(CPlateBasicIndex* pBx);
	void DeductBMSpliceLowerBoltHole(CPlateBasicIndex* pBx);
	void DeductBMSpliceWebBoltHole(CPlateBasicIndex* pBx);
	// 가로보
	void DeductBMCrossBeamMain(CPlateBasicIndex* pBx);
	void DeductBMCrossBeamBracket(CPlateBasicIndex* pBx, BOOL bLeft);
	void DeductBMCrossBeamBracketGasset(CPlateBasicIndex* pBx, BOOL bLeft);
	void DeductBMCrossBeamSplice(CPlateBasicIndex* pBx, BOOL bLeft);
	
// Implementation
protected:
	void AddBMFactChain(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,long nMarkFactType);	
	void AddBMHStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,BOOL bLeft);
	void AddBMSplice(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, BOOL bIncludeStt=TRUE, BOOL bIncludeEnd=TRUE);
	void AddBMSpliceUpper(CPlateBasicIndex* pBx);
	void AddBMSpliceLower(CPlateBasicIndex* pBx);
	void AddBMSpliceWeb(CPlateBasicIndex* pBx);
	
	// 수직 브레이싱
	void AddBMVBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMVBracingAngle(CPlateBasicIndex* pBx);
	void AddBMVBracingGusset(CPlateBasicIndex* pBx);
	void AddBMVBracingBolt(CPlateBasicIndex* pBx);
	// 수평 브레이싱
	void AddBMHBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMHBracingAngle(CPlateBasicIndex* pBx);
	void AddBMHBracingGusset(CPlateBasicIndex* pBx);
	void AddBMHBracingBolt(CPlateBasicIndex* pBx);

	// 수직 보강재
	void AddBMVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	
	// 가로보
	void AddBMCrossBeam(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMCrossBeamMain(CPlateBasicIndex* pBx);
	void AddBMCrossBeamBracket(CPlateBasicIndex* pBx, BOOL bLeft);
	void AddBMCrossBeamBracketGusset(CPlateBasicIndex* pBx, BOOL bLeft);
	void AddBMCrossBeamGirderVStiff(CPlateBasicIndex* pBx, CPlateBasicIndex* pBxEnd);
	void AddBMCrossBeamSplice(CPlateBasicIndex* pBx, BOOL bLeft);
	void AddBMVBracingGirderVStiff(CPlateBasicIndex* pBx, CPlateBasicIndex* pBxEnd);
	//
	void AddBMGitaBuje(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMGitaBuje_Dongbari(CPlateBasicIndex* pBx);
	void AddBMGitaBuje_JackUpPlate(CPlateBasicIndex* pBx);
	void AddBMGitaBuje_SolePlate(CPlateBasicIndex* pBx);
	void AddBMWeldStud(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	// 주판에 속함
	void AddBMJackUpStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, __int64 BxFlag=BX_JACKUP);
	void AddBMJiJumStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, __int64 BxFlag=BX_JIJUMSTIFF);
	void AddBMJackUpStiffSub(CPlateBasicIndex* pBx);	
	void AddBMFacePlate(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);

	void ClearBase();
	void ClearAll();
	BOOL SameAndIncludeRecord(CPtrList* pList, const CBMRecord* pRec,DWORD exceptionFlag=0);

	CBMRecord* AddBMRecord(CBMRecord* pRec =NULL);
	CBMRecord* AddSimpleBMRecord(CBMRecord* pRec,long nMarkPiece,CPlateBasicIndex* pBx,const CString& sSubEleName);
	CString GetMarkBx(long nMarkType,long nMark,CPlateBasicIndex* pBx,long nRow=0,long nCol=0, BOOL bFront=TRUE) const;

public:
	void SetMakeMark(long nMAKE_MARK,BOOL bApply);
	BOOL IsMakeMark(long nMAKE_MARK) const;

private:
	void SetBMSpliceRecord(CBMRecord *pRec, CPlateBasicIndex* pBx);	
	void SetBMCrossBeamRecord(CBMRecord *pRec, CPlateBasicIndex* pBx);	
	void SetBMVStiffRecord(CBMRecord *pRec, CPlateBasicIndex* pBx);
	void SetJewonBMRecord(CBMRecord *pRec, CPlateBasicIndex* pBx, long nMarkBuje, long nRow=0, long nCol=0, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE, CVector *pJewon=NULL, CVector *pJewonBasic=NULL);
	void SetDeductRecord(CBMRecord *pRec, CPlateBasicIndex* pBx, long nMarkBuje, long nRow=0, long nCol=0, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);

	/////////////////////////////////////////////////////////
	// BMStandard
public:
	CTStandard* m_pTStandard;
 
	// AROAD
	void GetBMListBoxGangje(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bSpaceTerm=FALSE);
	void GetBMListCrossBeam(CPtrList* pList, CPlateBasicIndex *pBx);	
	void GetBMListVBracing(CPtrList* pList, CPlateBasicIndex *pBx);
	void GetBMListByBxAndFlagSang(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 BxFlag, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bCrossBeam=FALSE);
	// ASHOP
	void GetBMListGangjeAllGirder(CPtrList* pList,  __int64 BxFlag, long nMarkMidAssem, long nMarkAssem, long nMarkSection);
	void GetBMListByBxAndFlag(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 BxFlag, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bCrossBeam=FALSE);
	void GetBMListAssemblyBox(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bSpaceTerm=FALSE);
	void GetBMListAssemblySection(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, __int64 BxFlag=0x00);
	void GetBMListAssemblyCrossBeam(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUnitWeightBracket, BOOL bUnitWeightSplice);
	void GetBMListAssemblyCrossBeamGasset(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft=TRUE);
	void GetBMListAssemblyCrossBeamSplice(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void GetBMListAssemblySerobo(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void GetBMListAssemblySeroboGasset(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bStt=TRUE);
	void GetBMListAssemblySeroboSplice(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bStt=TRUE);
	// 가로보상세도면 제원용	
	void MakeListCrossBeamJewon();
	CPtrList m_BMCrossBeamList;

// Implementation
protected:
	UINT AddAllCreator();
	UINT MakeToLoadFileXL();

	void ShellSortBMRecord(CPtrList* pList, int (CBMRecord::*fcmp)(const CBMRecord*) const) const;
	void MakeUnitList(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 BxFlag, BOOL bIncludeSttBx, BOOL bIncludeEndBx, BOOL bCrossBeam);

	// 강제집계표 제작용
	void MakeListSang();				// [01]기본도의 강재집계표 : 2002.06.10
	void MakeListStandardSang();		// [02]표준집계표
	void MakeListStandardShop();		// [03]표준집계표(제작용)
	void MakeListStandardShopType2();	// [03]표준집계표(제작용) - 2002.09.23
	void MakeListThick();				// [04]강종별 두께별집계표
	void MakeListThickSang();			// [04]두께별 상세집계표  - 2002.09.23
	void MakeListBujeMarkSang();		// [05]부재마크별 상세집계표
	void MakeListBujeMarkTotal();		// [06]부재마크별 총괄집계표
	void MakeListBujeMarkDeduct();		// [07]부재마크별 공제집계표
	void MakeListCar();					// [08]차량적재 상세집계표
	void MakeListCarTotal();			// [09]차량적재 총괄집계표
	void MakeListGrandAssemblySang();	// [10]대조립별 상세집계표
	void MakeListGrandAssemblyTotal();	// [11]대조립별 총괄집계표
	void MakeListMidAssemblySang();		// [12]중조립별 상세집계표
	void MakeListMidAssemblyTotal();	// [13]중조립별 총괄집계표
	void MakeListAssemblySang();		// [14]소조립별 상세집계표
	void MakeListAssemblyTotal();		// [15]소조립별 총괄집계표
	void MakeListAssemblyDeduct();		// [16]소조립별 공제집계표
	void MakeListAssemblyFact();		// [17]소조립별 공장제작용집계표
	void MakeListAssemblyField();		// [18]소조립별 현장설치용집계표
	void MakeListSectionComp();			// [19]단면별 집계표(기본도정산용)
	void MakeListBujeComp();			// [20]부재별 집계표(기본도정산용)
	void MakeListBasicComp();			// [21]기본도 정산용 집계표

	// 강제집계표 설계용
	void MakeListSectionType();		// 단면타입별 집계표
	void MakeListBuje();			// 부재별 집계표
	void MakeListBujeTotal();		// 부재별 총괄집계표
	void MakeListBujeKindTotal();	// 부재종류별 총괄집계표
	void MakeListDeductSang();		// 상세별공제
	void MakeListDeductBuje();		// 부재별공제
	void MakeListGirder();			// 거더별 집계표

	// 강제리스트 생성
	void MakeListGangje();
	void AddBMXLFileLine(const CStringArray& sArr);

public:
	virtual UINT DoWork();
	double GetTotalWeight() {return m_TotalWeight;}
	void SetWorkValue(long nWorkValue,const CString sPath,DIMTYPE nDimType,long nMakeFlag);
	void SetDimType(DIMTYPE nDimType) {m_nDimType = nDimType;}	
	BOOL IsCarBuje(CBMRecord *pRec);

private:
	double m_TotalWeight;	
	void SetPlateTypeInStandardList();



};

#endif // !defined(AFX_BMCREATOR_H__2C813849_F3E1_41CA_9673_27BB3A3273CB__INCLUDED_)
