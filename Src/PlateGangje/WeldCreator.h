// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WeldCreator.h: interface for the CWeldCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WELDCREATOR_H__63358313_2087_4D76_B366_7D10F4E434D8__INCLUDED_)
#define AFX_WELDCREATOR_H__63358313_2087_4D76_B366_7D10F4E434D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const long WELDMAKE_SANG			= 0x00000001;
const long WELDMAKE_ASSEMBLY_SANG	= 0x00000002;
const long WELDMAKE_ASSEMBLY		= 0x00000004;
const long WELDMAKE_ASSEMBLY_TOTAL	= 0x00000008;
const long WELDMAKE_WELDTYPE		= 0x00000010;
const long WELDMAKE_WELDTHICK		= 0x00000020;

const DWORD WM_WELD_PROGRESS = WM_APP+2203;

#include "WeldRecord.h"
class AFX_EXT_CLASS CWeldCreator : public CThreadWork
{
public:
	CWeldCreator(CDataManage* pMng);
	virtual ~CWeldCreator();

	CDataManage *m_pDataManage;
	CPlateBridgeApp *m_pDB;
	BOOL m_bAlertWeld;

	CPtrList m_ListStandard;
	CPtrList m_ListSang;
	CPtrList m_ListAssembly;
	CPtrList m_ListAssemblySang;
	CPtrList m_ListAssemblyTotal;
	CPtrList m_ListWeldType;
	CPtrList m_ListWeldThick;

	void ClearListStandard();
	void ClearListSang();
	void ClearListAssembly();
	void ClearListAssemblySang();
	void ClearListAssemblyTotal();
	void ClearListWeldType();
	void ClearListWeldThick();
	void SetMakeMark(long nMAKE_MARK,BOOL bApply);
	BOOL IsMakeMark(long nMAKE_MARK) const;

protected:
	DWORD	m_dwMakeMarkFlag;	// Mark Flag
	long	m_nWorkValue;	// 0:생성, 1:tbl파일로드, 2:액셀파일로드
	CString	m_sPath;		// 
	DIMTYPE	m_nDimType;		// DimType
	long	m_nMakeFlag;	// MakeFlag

	CDoubleArray m_dStudDisArr;
	//
	void AddFlangeUpper(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddFlangeLower(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddFlangeWeb(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddCrossBeam(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddVBracing(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddStringer(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddHStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeftWeb);
	void AddVStiff(CPlateBasicIndex *pBx, BOOL bLeft, BOOL bJijum);
	void AddCrossBeamGirderVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddVBracingGirderVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddJackUpStiff(CPlateBasicIndex *pBx);
	BOOL SameAndIncludeRecord(CPtrList* pList, const CWeldRecord *pRec,DWORD exceptionFlag=0);
	CString GetMarkBx(long nMarkType,long nMark,CPlateBasicIndex* pBx,long nRow=0,long nCol=0, BOOL bFront=TRUE) const;
	CWeldRecord* AddWeldRecord(CWeldRecord* pRec=NULL);

	// 전단연결재
	void AddStud(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);

	// 수평브레이싱
	void AddHBracing(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddHBracingGusset(CPlateBasicIndex *pBx);
private:
	// 가로보
	void AddCrossBeamMainFlange(CPlateBasicIndex *pBx);
	void AddCrossBeamMainHStiff(CPlateBasicIndex *pBx);
	void AddCrossBeamMainVStiff(CPlateBasicIndex *pBx, BOOL bFront);
	void AddCrossBeamBrkUpperFlange(CPlateBasicIndex *pBx, BOOL bLeft);
	void AddCrossBeamBrkLowerFlange(CPlateBasicIndex *pBx, BOOL bLeft);
	void AddCrossBeamBrkSupportPlate(CPlateBasicIndex *pBx, BOOL bLeft);
	void AddCrossBeamBrkSupportPlatePan(CPlateBasicIndex *pBx, BOOL bLeft);
	void AddCrossBeamAnchor(CPlateBasicIndex *pBx);

	void AddVBracingGusset(CPlateBasicIndex *pBx);
	void AddVBracingRecord(CPlateBasicIndex *pBx, long nCol, long nMarkBujeSec);
	
	//////////////////////////////////////////////////////////
	void SetJewonWeldRecord(CWeldRecord *pRec, CPlateBasicIndex *pBx, long nMarkBuje, long nMarkBujeSec, long nRow=0, long nCol=0, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);
	void SetThickWeldByType(CWeldRecord *pRec, CPlateBasicIndex *pBx, long nMarkBuje, long nMarkBujeSec, double dThickSec);
	//
	void SetWeldRecordFact(CWeldRecord *pRec, CPlateBasicIndex* pBx);	
	void SetWeldRecordCrossBeam(CWeldRecord *pRec, CPlateBasicIndex* pBx);
	void SetWeldRecordVStiff(CWeldRecord *pRec, CPlateBasicIndex* pBx, BOOL bLeft);
	void SetWeldRecordTRib(CWeldRecord *pRec, CPlateBasicIndex* pBx, BOOL bUpper);

};

#endif // !defined(AFX_WELDCREATOR_H__63358313_2087_4D76_B366_7D10F4E434D8__INCLUDED_)
