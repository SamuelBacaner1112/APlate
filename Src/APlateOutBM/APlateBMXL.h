// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateBMXL.h: interface for the CAPlateBMXL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEBMXL_H__5F27EFB4_FF8E_4468_92F8_A0F793C62D9E__INCLUDED_)
#define AFX_APLATEBMXL_H__5F27EFB4_FF8E_4468_92F8_A0F793C62D9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_MAKEANALYSIS_BM (WM_APP + 1010) // 물량 계산서

#define	DIA_H13		0
#define	DIA_H16		1
#define	DIA_H19		2
#define	DIA_H22		3
#define	DIA_H25		4
#define	DIA_H29		5
#define	DIA_H32		6
#define	DIA_H35		7
#define	DIA_D10		8
#define	DIA_D13		9
#define	DIA_D16		10
#define	DIA_D19		11
#define	DIA_D22		12
#define	DIA_D25		13
#define	DIA_D29		14
#define	DIA_D32		15
#define	DIA_D35		16

#define MAX_BRIDGE	16

class CAPlateOutBMBrInfo;

class CAPlateBMXL : public CThreadWork
{
public:
	CAPlateBMXL(CAPlateOutBMStd *pAPlateOutBMStd);
	virtual ~CAPlateBMXL();
	enum {SLAB_TOTAL_SUMMARY, REBAR_TOTAL_SUMMARY, SLAB_SUMMARY, REBAR_SUMMARY, SLAB_CALC_FOUNDATION, GUARDFENCE_CALC_FOUNDATION, CONFINEMENT_CONCRETE};

	virtual	UINT DoWork();
	CAPlateOutBMStd		*m_pAPlateOutBMStd;
	CAPlateOutBMBrInfo	*m_pAPlateOutBMBrInfo;		// 일단 교량 1개만 적용 - 수정
	CString			m_szReadFileName;		// 읽어서 쓸 dll 파일 이름
	CString			m_szSaveFilePath;		// 저장 파일 이름
	CString			m_szBridgeDir;			// 교량 진행 방향
	CStringArray	m_szRebarCol;			// 출력 Col String
	BOOL			m_bSubBridge;			// 2개이상의 교량 출력
	long			m_nBridgeDir;			// 교량 진행방향의 갯수
	long			m_nBridgeSP;			// 같은 진행 방향의 스팬갯수
	long			m_nTotSheetSu;			// 총 출력 Sheet 수
	long			m_nBMSheetIdx[255];		// 출력할 Sheet의 ID를 차례데로 저장
	long			m_nRebarColIdx;			// 출력 Col Index
	CXLControl		*m_pImageXL;			///< 그림 엑셀

	void TotalSummary(CXLControl *pXL, CStringArray &ArrDelSheet);			// 총괄 집계표
	void RebarTotalSummary(CXLControl *pXL, CStringArray &ArrDelSheet);		// 철근 수량 총괄 집계표
	void RebarSummary(CXLControl *pXL, CStringArray &ArrDelSheet);			// 철근 수량 집계표
	void RebarSummarySlab(CXLControl *pXL);									// 철근 수량 슬래브
	void RebarSummarySlabDivide(CXLControl *pXL);							// 철근 수량 슬래브(철근분리)
	void RebarSummaryGuardFence(CXLControl *pXL);							// 철근 수량 방호벽
	void RebarSummaryGuardFenceDivide(CXLControl *pXL);						// 철근 수량 방호벽(철근분리)
	void RebarSummaryWaterHole(CXLControl *pXL);							// 철근 수량 배수구 보강철근
	void RebarSummaryExpansionJoint(CXLControl *pXL);						// 철근 수량 신축이음
	void RebarSummaryConfinement(CXLControl *pXL);							// 철근 수량 구속 콘크리트
	void RebarSummaryConfinementDivide(CXLControl *pXL);							// 철근 수량 구속 콘크리트

	void WriteRebarSummary(CXLControl *pXL, CMap <CString, LPCTSTR, double, double> &MapWeightTot, long &nSttRow, double dBrLength);
	void WriteSlabHeight(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight);
	void WriteSlabLength(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapLength);
	void WriteSlabSlop(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight, CMap <CString, LPCTSTR, CString, CString> &MapLength, CMap <CString, LPCTSTR, CString, CString> &MapSlop);
	void WriteSlabArea(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight, CMap <CString, LPCTSTR, CString, CString> &MapLength, CMap <CString, LPCTSTR, CString, CString> &MapArea);
	void WriteSlabEctLength(CXLControl *pXL, long &nSttRow);
	void WriteCrossBeam(CXLControl *pXL, long &nSttRow);
	void WriteSlabDimTable(CXLControl *pXL, long &nSttRow);

	void SlabSummary(CXLControl *pXL, CStringArray &ArrDelSheet);			// 슬래브 수량 집계표
	void SlabCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet);	// 산출근거(슬래브)
	void GuardCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet);	// 산출근거(방호벽)nGuardsu
	void ConfinementConcrete(CXLControl *pXL, CStringArray &ArrDelSheet);	// 구속큰크리트

	void KillTheXL();
	void APlateOutBM(CXLControl	*pXL, long nOutIdx, CStringArray &ArrDelSheet);
	void GetOutBMList();
	void SetProgress(CString strText, long nlW);
	long GetDiaIdx(CString szDia);
	CString	GetNamePrintSheet(long nSheet);
	CAPlateOutBMBrInfo* GetBridgeInfo(long nBridgeIdx);
private:
	BOOL	m_bSlabRebar[2];			// m_bSlabRebar[0]				= TRUE : 고강사용	m_bSlabRebar[1]				= TRUE : 연강사용
	BOOL	m_bGuardFenceRebar[2];		// m_bGuardFenceRebar[0]		= TRUE : 고강사용	m_bGuardFenceRebar[1]		= TRUE : 연강사용
	BOOL	m_bExpansionJointRebar[2];	// m_bExpansionJointRebar[0]	= TRUE : 고강사용	m_bExpansionJointRebar[1]	= TRUE : 연강사용
	BOOL	m_bConfinementConc[2];		// m_bConfinementConc[0]		= TRUE : 고강사용	m_bConfinementConc[1]		= TRUE : 연강사용

	BOOL	IsSameGuardFence(long nGuardFence, long &nGuardMatch);
	CString	GetGuardFenceSheetName(long nGuard);
	void MakeRebarList(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar);
	void MakeRebarListGuard(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar);

	CMap <CString, LPCTSTR, CString, CString> m_MapGuardCell;

};

#endif // !defined(AFX_APLATEBMXL_H__5F27EFB4_FF8E_4468_92F8_A0F793C62D9E__INCLUDED_)
