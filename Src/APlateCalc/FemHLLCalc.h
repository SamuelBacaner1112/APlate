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
	long m_nIdx;	// 기초 인덱스로 사용
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

	void RunWindAnalysis();			// 풍하중계산 수행
	void RunCrossAnalysis();		// 횡분배 계산 수행
	void RunInfluenceAnalysis();	// 영향계수 계산 수행
	void CalcHLiveLoadType();				// 횡분배계산 방법을 결정함
	double GetElemLength(CFEMManage *pFemMange, long nElm);			// 요소의 길이를 구함	
	CCombo* GetComboByName(CFEMManage *pFEM, CString szName);		// 하중조합이름을 통해 CCombo를 구함.
	CCombo* GetComboByLCName(CFEMManage *pFEM, CString szLCName);	// LoadCase를 통해 CCombo를 구함.
	long GetLoadApplyMemberCount(CFEMManage *pFEM);

private:
	typedef enum { DB, DL, TANK, TRAILER } LIVETYPE;
	// CFEMManage 모델링
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

	// 절점 위치에서 슬래브의 두께를 구함
	double GetSlabHeightOnJoint(CFEMManage *pFEM, long nIdx, CPlateBasicIndex *pBx);
	// 분포하중 재하함(dDistStt : 전체요소의 시점에서의 이격거리)
	void AddTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, double dForceStt, double dForceEnd, long nLoadCase, CString szLoadName="");
	// 집중하중 재하(dDist : 전체요소의 시점에서의 이격거리)
	void AddPointForceAtLeft(CFEMManage *pFEM, double dDist, CVector vForce, long nLoadCase, CString szLoadName="");
	// 단위집중하중 재하(dDist : 전체요소의 시점에서의 이격거리)
	BOOL AddUnitPointForceAtLeft(CFEMManage *pFEM, double dDist, long nLoadCase, CString szLoadName="");
	// 단위분포하중 재하(dDistStt : 전체요소의 시점에서의 이격거리)
	BOOL AddUnitTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, long nLoadCase, CString szLoadName="");
	// 시점에서의 이격거리를 이용해 요소를 구함.
	long GetElemNoByDist(double dDist, CFEMManage *pFEM);
	// 교량 중심을 기준으로 좌우측을 판별
	BOOL IsLeft(CPlateBasicIndex *pBx, double dDist);
	
	
	void GetHDanArrayForLiveLoad(CDPointArray &ptArrVal, long nHL);
	BOOL SetDBDLLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nG, double dSttRoadway, double dEndRoadway, long &nLoadCase, CHLLLaneUnit *pHLLLaneUnit, long nQtyLane, LIVETYPE Type);
	// 횡단좌측 단부에서 거더까지 거리
	double GetGirderPos(long nHL, long nG);
	void InsertPreLane(CFEMManage *pFEM, CHLLLane *pHLLLane, CHLLLaneUnit *pHLLLaneUnit, long nLane, long nLoadCase, LIVETYPE Type);
	//활하중의 거리를 셋팅
	void SetReDefineEdge(long nHL);	// 바닥판 설계고려사항입력에서 임시로 생성한 방호벽 및 보도의 정확한 위치(확폭고려)를 재정의한다.
	// 횡분배수와 횡분배위치bx를 구함(nHLType=1 : 편경사 변화, nHLType=2 : 교량폭 변화, nHLType=3 : 편경사+교량폭 변화, nHLType=4 : 편경사실제 횡분배)
	void SetHLsu(long nHLType);
	void SetWindLoadForce(long nJijum);											// 풍하중 모델링 수행전 계산
	double GetWindLoad(double dBridgeWidth, double dBridgeHeight, double dD);	// 풍하중 크기 구하기

	BOOL IsSameI33(double dI331, double dI332, double dError=0.0000001);
};

#endif // !defined(AFX_FEMHLLCALC_H__BF83F3A0_7228_44C6_AEFB_E65513F6AE99__INCLUDED_)
