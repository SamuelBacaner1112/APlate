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

#define WM_USER_MAKEANALYSIS_BM (WM_APP + 1010) // ���� ��꼭

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
	CAPlateOutBMBrInfo	*m_pAPlateOutBMBrInfo;		// �ϴ� ���� 1���� ���� - ����
	CString			m_szReadFileName;		// �о �� dll ���� �̸�
	CString			m_szSaveFilePath;		// ���� ���� �̸�
	CString			m_szBridgeDir;			// ���� ���� ����
	CStringArray	m_szRebarCol;			// ��� Col String
	BOOL			m_bSubBridge;			// 2���̻��� ���� ���
	long			m_nBridgeDir;			// ���� ��������� ����
	long			m_nBridgeSP;			// ���� ���� ������ ���Ұ���
	long			m_nTotSheetSu;			// �� ��� Sheet ��
	long			m_nBMSheetIdx[255];		// ����� Sheet�� ID�� ���ʵ��� ����
	long			m_nRebarColIdx;			// ��� Col Index
	CXLControl		*m_pImageXL;			///< �׸� ����

	void TotalSummary(CXLControl *pXL, CStringArray &ArrDelSheet);			// �Ѱ� ����ǥ
	void RebarTotalSummary(CXLControl *pXL, CStringArray &ArrDelSheet);		// ö�� ���� �Ѱ� ����ǥ
	void RebarSummary(CXLControl *pXL, CStringArray &ArrDelSheet);			// ö�� ���� ����ǥ
	void RebarSummarySlab(CXLControl *pXL);									// ö�� ���� ������
	void RebarSummarySlabDivide(CXLControl *pXL);							// ö�� ���� ������(ö�ٺи�)
	void RebarSummaryGuardFence(CXLControl *pXL);							// ö�� ���� ��ȣ��
	void RebarSummaryGuardFenceDivide(CXLControl *pXL);						// ö�� ���� ��ȣ��(ö�ٺи�)
	void RebarSummaryWaterHole(CXLControl *pXL);							// ö�� ���� ����� ����ö��
	void RebarSummaryExpansionJoint(CXLControl *pXL);						// ö�� ���� ��������
	void RebarSummaryConfinement(CXLControl *pXL);							// ö�� ���� ���� ��ũ��Ʈ
	void RebarSummaryConfinementDivide(CXLControl *pXL);							// ö�� ���� ���� ��ũ��Ʈ

	void WriteRebarSummary(CXLControl *pXL, CMap <CString, LPCTSTR, double, double> &MapWeightTot, long &nSttRow, double dBrLength);
	void WriteSlabHeight(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight);
	void WriteSlabLength(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapLength);
	void WriteSlabSlop(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight, CMap <CString, LPCTSTR, CString, CString> &MapLength, CMap <CString, LPCTSTR, CString, CString> &MapSlop);
	void WriteSlabArea(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight, CMap <CString, LPCTSTR, CString, CString> &MapLength, CMap <CString, LPCTSTR, CString, CString> &MapArea);
	void WriteSlabEctLength(CXLControl *pXL, long &nSttRow);
	void WriteCrossBeam(CXLControl *pXL, long &nSttRow);
	void WriteSlabDimTable(CXLControl *pXL, long &nSttRow);

	void SlabSummary(CXLControl *pXL, CStringArray &ArrDelSheet);			// ������ ���� ����ǥ
	void SlabCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet);	// ����ٰ�(������)
	void GuardCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet);	// ����ٰ�(��ȣ��)nGuardsu
	void ConfinementConcrete(CXLControl *pXL, CStringArray &ArrDelSheet);	// ����ūũ��Ʈ

	void KillTheXL();
	void APlateOutBM(CXLControl	*pXL, long nOutIdx, CStringArray &ArrDelSheet);
	void GetOutBMList();
	void SetProgress(CString strText, long nlW);
	long GetDiaIdx(CString szDia);
	CString	GetNamePrintSheet(long nSheet);
	CAPlateOutBMBrInfo* GetBridgeInfo(long nBridgeIdx);
private:
	BOOL	m_bSlabRebar[2];			// m_bSlabRebar[0]				= TRUE : �����	m_bSlabRebar[1]				= TRUE : �������
	BOOL	m_bGuardFenceRebar[2];		// m_bGuardFenceRebar[0]		= TRUE : �����	m_bGuardFenceRebar[1]		= TRUE : �������
	BOOL	m_bExpansionJointRebar[2];	// m_bExpansionJointRebar[0]	= TRUE : �����	m_bExpansionJointRebar[1]	= TRUE : �������
	BOOL	m_bConfinementConc[2];		// m_bConfinementConc[0]		= TRUE : �����	m_bConfinementConc[1]		= TRUE : �������

	BOOL	IsSameGuardFence(long nGuardFence, long &nGuardMatch);
	CString	GetGuardFenceSheetName(long nGuard);
	void MakeRebarList(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar);
	void MakeRebarListGuard(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar);

	CMap <CString, LPCTSTR, CString, CString> m_MapGuardCell;

};

#endif // !defined(AFX_APLATEBMXL_H__5F27EFB4_FF8E_4468_92F8_A0F793C62D9E__INCLUDED_)
