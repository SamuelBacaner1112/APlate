// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PaintCreator.h: interface for the CPaintCreator class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_PAINTCREATOR_H__5DF19FF3_5801_43E0_AE76_1690338B7132__INCLUDED_)
#define AFX_PAINTCREATOR_H__5DF19FF3_5801_43E0_AE76_1690338B7132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


const long PAINTMAKE_SANG					= 0x00000001;
const long PAINTMAKE_BUJE_SANG				= 0x00000002;
const long PAINTMAKE_BUJE_TOTAL				= 0x00000004;
const long PAINTMAKE_ASSEMBLY				= 0x00000008;
const long PAINTMAKE_ASSEMBLY_TOTAL			= 0x00000010;
const long PAINTMAKE_GRAND_ASSEMBLY			= 0x00000020;
const long PAINTMAKE_GRAND_ASSEMBLY_TOTAL	= 0x00000040;
const long PAINTMAKE_MAT_THICK				= 0x00000080;
const long PAINTMAKE_BOLT_SANG				= 0x00000100;
const long PAINTMAKE_BOLT_TOTAL				= 0x00000200;
const long PAINTMAKE_SUMMARY				= 0x00000400;
 
const DWORD WM_PAINT_PROGRESS = WM_APP+2201;

class CPaintRecord;
class CDataManage;
class CBMCreator;
class CTStandard;

class AFX_EXT_CLASS CPaintCreator : public CThreadWork  
{
public:
	CPaintCreator(CDataManage* pMng);
	virtual ~CPaintCreator();

	CDataManage *m_pDataManage;
	CPlateBridgeApp *m_pDB;
	CPtrList m_ListStandard;
	//
	CPtrList m_ListSang;
	CPtrList m_ListBujeSang;
	CPtrList m_ListBujeTotal;
	CPtrList m_ListAssembly;
	CPtrList m_ListAssemblyTotal;
	CPtrList m_ListGrandAssembly;
	CPtrList m_ListGrandAssemblyTotal;
	CPtrList m_ListMatAndThick;
	CPtrList m_ListBoltSang;
	CPtrList m_ListBoltTotal;
	//
	BOOL SameAndIncludeRecord(CPtrList* pList, const CPaintRecord* pRec, DWORD exceptionFlag=0);
	void SetMakeMark(long nMAKE_MARK,BOOL bApply);
	BOOL IsMakeMark(long nMAKE_MARK) const;

	void SetTStandard(CTStandard *pTStandard);
	CTStandard* GetTStandard();
protected:
	DWORD	m_dwMakeMarkFlag;	// Mark Flag
	long	m_nWorkValue;		// 0:생성, 1:tbl파일로드, 2:액셀파일로드
	CString	m_sPath;			// 
	DIMTYPE	m_nDimType;			// DimType
	long	m_nMakeFlag;		// MakeFlag

private:
	CTStandard* m_pTStandard;

	void ClearAll();
	////////////////////////////////////////////////////////////
	void AddFlange(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddWeb(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);	
	void AddHStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);		
	void AddVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddFieldSplice(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeStt, BOOL bIncludeEnd);
	void AddCrossBeam(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);			
	void AddGitaBuje(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	// ADD
	void AddFacePlate(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	
	void RemoveTailPaintRecord();
	CPaintRecord* AddPaintRecord(CPaintRecord* pRec =NULL);
	CString GetMarkBx(long nMarkType,long nMark,CPlateBasicIndex* pBx,long nRow=0,long nCol=0, BOOL bFront=TRUE) const;

	// 수직 브레이싱
	void AddVBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddVBracingAngle(CPlateBasicIndex* pBx);
	void AddVBracingGusset(CPlateBasicIndex* pBx);
	void AddVBracingBolt(CPlateBasicIndex* pBx);
	void AddVBracingGirderVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	// 수평 브레이싱
	void AddHBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddHBracingAngle(CPlateBasicIndex* pBx);
	void AddHBracingGusset(CPlateBasicIndex* pBx);
	void AddHBracingBolt(CPlateBasicIndex* pBx);

	// 가로보
	void AddCrossBeamMain(CPlateBasicIndex *pBx);
	void AddCrossBeamGirderVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void SetPaintAreaVStiff(CPaintRecord* pRec);

private:	
	//현장이음 
	void AddSpliceUpper(CPlateBasicIndex *pBx);
	void AddSpliceLower(CPlateBasicIndex *pBx);
	void AddSpliceWeb(CPlateBasicIndex *pBx);
	//거더 	
	void AddBottomFlange(CPlateBasicIndex *pBx);
	void AddUpperFlange(CPlateBasicIndex *pBx);	
	void AddWebPlate(CPlateBasicIndex *pBx);
	//수평보강재 
	void AddHStiffSub(CPlateBasicIndex *pBx, long row, BOOL bLeft);
	// 수직보강재
	void AddVStiff(CPlateBasicIndex *pBx, BOOL bLeft);
	//가로보 
	void AddCrossBeamHSteel(CPlateBasicIndex *pBx);
	void AddCrossBeamWeb(CPlateBasicIndex *pBx);
	void AddCrossBeamFlange(CPlateBasicIndex *pBx, BOOL bUpper);
	void AddCrossBeamVStiff(CPlateBasicIndex *pBx);
	void AddCrossBeamHStiff(CPlateBasicIndex *pBx);
	void AddCrossBeamAnchor(CPlateBasicIndex *pBx);
	void AddCrossBeamSplice(CPlateBasicIndex *pBx, BOOL bLeft);
	void AddCrossBeamBrk(CPlateBasicIndex *pBx, BOOL bLeft);
	void AddCrossBeamBrkGusset(CPlateBasicIndex *pBx, BOOL bLeft);

	// 기타부재
	void AddGitaBujeSolePlate(CPlateBasicIndex *pBx);	
	void AddGitaBujeDongbari(CPlateBasicIndex* pBx);
	void AddGitaBujeShareConnect(CPlateBasicIndex* pBx);
	void AddGitaBujeJackUp(CPlateBasicIndex *pBx, BOOL bLeft);	
	void AddGitaBujeJiJumStiff(CPlateBasicIndex* pBx);
	void AddGitaBujeJackUpPlate(CPlateBasicIndex* pBx);

	void SetJewonPaintRecord(CPaintRecord *pRec, CPlateBasicIndex *pBx, long nMarkBuje, long nRow=0, long nCol=0, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);
	void SetPaintRecordFact(CPaintRecord *pRec, CPlateBasicIndex* pBx);
	void SetPaintRecordSplice(CPaintRecord *pRec, CPlateBasicIndex* pBx);	
	void SetRecStrEleNameCrossBeam(CPaintRecord *pRec, CPlateBasicIndex* pBx);		





	//Org PaintStandard	
public:
	UINT AddAllCreator();
	void ShellSortRecord(CPtrList* pList, int (CPaintRecord::*fcmp)(const CPaintRecord*) const) const;
	void MakeListSang();
	void MakeListBujeSang();
	void MakeListBujeTotal();
	void MakeListAssembly();
	void MakeListAssemblyTotal();
	void MakeListGrandAssembly();
	void MakeListGrandAssemblyTotal();
	void MakeListMatAndThick();
	void MakeListBoltSang();
	void MakeListBoltTotal();

protected:
	void AddPaintXLFileLine(const CStringArray& sArr);
	UINT MakeToLoadFileXL();

public:
	virtual UINT DoWork();
	void SetWorkValue(long nWorkValue,const CString sPath,DIMTYPE nDimType,long nMakeFlag);

private:
	void SetTotalPaintStandardList();
};

#endif // !defined(AFX_PAINTCREATOR_H__5DF19FF3_5801_43E0_AE76_1690338B7132__INCLUDED_)
