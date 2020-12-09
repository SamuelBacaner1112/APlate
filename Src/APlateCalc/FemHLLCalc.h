// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

//////////////////////////////////////////////////////////////////////////
// SectionJewon
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_SECTION_JEWON)
#define AFX_SECTION_JEWON

class AFX_EXT_CLASS CSectionJewon
{
public:
	CSectionJewon();
	virtual ~CSectionJewon();

	bool operator==(const CSectionJewon& rhs) const;
	CSectionJewon& operator=(const CSectionJewon& rhs);

	long m_nType;
	long m_nIdx;	// ���� �ε����� ���
	CPlateBasicIndex *m_pBx;
	double m_dTu;
	double m_dTl;
	double m_dBu;	
	double m_dBl;	
	double m_dH;
	double m_dTw;

	double m_dI33;	
	double m_dI22;	

	double m_dA;
	double m_dJ;

	BOOL m_bNeedDelete;
	BOOL m_bUsedCalc;
};
#endif	// !defined(AFX_SECTION_JEWON)

// FemHLLCalc.h: interface for the CFemHLLCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEMHLLCALC_H__BF83F3A0_7228_44C6_AEFB_E65513F6AE99__INCLUDED_)
#define AFX_FEMHLLCALC_H__BF83F3A0_7228_44C6_AEFB_E65513F6AE99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CFEMManage;
class CPlateBasicIndex;
class CCombo;
class CMatrial;

struct PEOPLELOAD
{
	long	m_nType;
	BOOL	m_bLeft;
	double	m_dSttLen;
	double	m_dEndLen;
};

inline BOOL IsDelete(CSectionJewon p)
{
	return p.m_bNeedDelete;
}

class AFX_EXT_CLASS CFemHLLCalc  
{
public:
	CFemHLLCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd);
	virtual ~CFemHLLCalc();

	CDataManage		*m_pDataManage;
	CAPlateCalcStd	*m_pCalcStd;
	CProgressCtrl	m_ctlProgress;

	void RunWindAnalysis();			// ǳ���߰�� ����
	void RunCrossAnalysis();		// Ⱦ�й� ��� ����
	void RunInfluenceAnalysis();	// ������ ��� ����
	void CalcHLiveLoadType();				// Ⱦ�й��� ����� ������
	double GetElemLength(CFEMManage *pFemMange, long nElm);			// ����� ���̸� ����	
	CCombo* GetComboByName(CFEMManage *pFEM, CString szName);		// ���������̸��� ���� CCombo�� ����.
	CCombo* GetComboByLCName(CFEMManage *pFEM, CString szLCName);	// LoadCase�� ���� CCombo�� ����.
	long GetLoadApplyMemberCount(CFEMManage *pFEM);

private:
	typedef enum { DB, DL, TANK, TRAILER } LIVETYPE;
	// CFEMManage �𵨸�
	void MakeJointOnBx(CFEMManage *pFEM, CPlateBasicIndex *pBx, BOOL bWind);
	long MakeFrameSectionOnGir(CFEMManage *pFEM, long nG);
	void MakeElementDefineSectionOnGir(CFEMManage *pFEM, long nSection);
	void MakeJointOnGir(CFEMManage *pFEM, long nG);
	void MakeElementOnGir(CFEMManage *pFEM, long nG);
	void MakeJointRestraint(CFEMManage *pFEM, CPlateBasicIndex *pBx);
	void MakeElement(CFEMManage *pFEM, BOOL bWind);
	void MakeForcePreDeadLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeForcePostDeadLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeForceLiveLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nHL, long &nLoadCase, LIVETYPE Type);
	void MakeForceLivePeople(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCase);
	void MakeForceLiveTank(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nHL, long &nLoadCase);
	void MakeForceLiveTrailer(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nHL, long &nLoadCase);
	void MakeForceWindLoad(CFEMManage *pFEM, long nJijum);
	void MakeForceInfK(CFEMManage *pFEM);
	void MakeMaterial(CFEMManage *pFEM);
	void MakeFrameSection(CFEMManage *pFEM);

	void AddUnitPointTankOrTrailor(CFEMManage *pFEM, BOOL bTank, double dRoadStt, double dRoadEnd, long nIndex, long nLoadCase);

	// ���� ��ġ���� �������� �β��� ����
	double GetSlabHeightOnJoint(CFEMManage *pFEM, long nIdx, CPlateBasicIndex *pBx);
	// �������� ������(dDistStt : ��ü����� ���������� �̰ݰŸ�)
	void AddTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, double dForceStt, double dForceEnd, long nLoadCase, CString szLoadName="");
	// �������� ����(dDist : ��ü����� ���������� �̰ݰŸ�)
	void AddPointForceAtLeft(CFEMManage *pFEM, double dDist, CVector vForce, long nLoadCase, CString szLoadName="");
	// ������������ ����(dDist : ��ü����� ���������� �̰ݰŸ�)
	BOOL AddUnitPointForceAtLeft(CFEMManage *pFEM, double dDist, long nLoadCase, CString szLoadName="");
	// ������������ ����(dDistStt : ��ü����� ���������� �̰ݰŸ�)
	BOOL AddUnitTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, long nLoadCase, CString szLoadName="");
	// ���������� �̰ݰŸ��� �̿��� ��Ҹ� ����.
	long GetElemNoByDist(double dDist, CFEMManage *pFEM);
	// ���� �߽��� �������� �¿����� �Ǻ�
	BOOL IsLeft(CPlateBasicIndex *pBx, double dDist);
	
	
	void GetHDanArrayForLiveLoad(CDPointArray &ptArrVal, long nHL);
	BOOL SetDBDLLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nG, double dSttRoadway, double dEndRoadway, long &nLoadCase, CHLLLaneUnit *pHLLLaneUnit, long nQtyLane, LIVETYPE Type);
	// Ⱦ������ �ܺο��� �Ŵ����� �Ÿ�
	double GetGirderPos(long nHL, long nG);
	void InsertPreLane(CFEMManage *pFEM, CHLLLane *pHLLLane, CHLLLaneUnit *pHLLLaneUnit, long nLane, long nLoadCase, LIVETYPE Type);
	//Ȱ������ �Ÿ��� ����
	void SetReDefineEdge(long nHL);	// �ٴ��� �����������Է¿��� �ӽ÷� ������ ��ȣ�� �� ������ ��Ȯ�� ��ġ(Ȯ�����)�� �������Ѵ�.
	// Ⱦ�й���� Ⱦ�й���ġbx�� ����(nHLType=1 : ���� ��ȭ, nHLType=2 : ������ ��ȭ, nHLType=3 : ����+������ ��ȭ, nHLType=4 : ������� Ⱦ�й�)
	void SetHLsu(long nHLType);
	void SetWindLoadForce(long nJijum);											// ǳ���� �𵨸� ������ ���
	double GetWindLoad(double dBridgeWidth, double dBridgeHeight, double dD);	// ǳ���� ũ�� ���ϱ�

	BOOL IsSameI33(double dI331, double dI332, double dError=0.0000001);
};

#endif // !defined(AFX_FEMHLLCALC_H__BF83F3A0_7228_44C6_AEFB_E65513F6AE99__INCLUDED_)
