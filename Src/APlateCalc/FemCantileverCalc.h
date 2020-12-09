// FemCantileverCalc.h: interface for the CFemCantileverCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEMCANTILEVERCALC_H__9C1D6A71_0F53_40C8_BC17_FA20221402ED__INCLUDED_)
#define AFX_FEMCANTILEVERCALC_H__9C1D6A71_0F53_40C8_BC17_FA20221402ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFrameSection;

#define PRE_LOADCASE_WEIGHT		"WEIGHT"

#define MAKE_ALL	0
#define MAKE_COMBO	1
#define MAKE_APPLY	2

class AFX_EXT_CLASS CFemCantileverCalc  
{
public:
	CFemCantileverCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd);
	virtual ~CFemCantileverCalc();

	void MakeCantileverModeling(long nMakeType);
	double GetHDanArea(CPlateBasicIndex *pBx);
	double GetHDanAreaLength(CPlateBasicIndex *pBx);

	enum {JOINT_TYPE, GIRDER_NUM, JOINT_SEC};
	enum {JOINT_SLAB, JOINT_GIR_UPPERSIDE, JOINT_GIR_LOWERSIDE, JOINT_GIR_CENT, JOINT_CROSS};
	enum {ELEM_TYPE};
	enum {ELEM_SLAB, ELEM_UPPER_FLANGE, ELEM_LOWER_FLANGE, ELEM_WEB, ELEM_CROSS};
	enum {JOINT_LEFT_CANTILEVER, JOINT_RIGHT_CANTILEVER, JOINT_SLAB_CENT, JOINT_SLAB_DANBU};

private:
	CDataManage		*m_pDataManage;
	CAPlateCalcStd	*m_pCalcStd;
	CProgressCtrl	m_ctlProgress;

	CPlateBasicIndex *GetModelingPosition();
	BOOL IsTaper(CPlateBasicIndex *pBx);
	CDPoint GetXyTensionBar(CPlateBasicIndex *pBx, BOOL bLeft);
	CJoint *AddJointData(CFEMManage *pFEM, long &nIndex, CVector vJoint, CVector vMove, long nJointType, long nGirderNum=-1, long nNodeSec=-1);
	CFrameSection *GetFrameSectionByHeight(CFEMManage *pFEM, double dHeightStt, double dHeightEnd);
	void GetSlabElemNum(CFEMManage *pFEM, long &nStt, long &nEnd);

	void MakeJointData(CFEMManage *pFEM, CPlateBasicIndex *pBx);
	void MakeElementData(CFEMManage *pFEM, CPlateBasicIndex *pBx);
	void MakeGroupData(CFEMManage *pFEM);
	void MakeMaterialData(CFEMManage *pFEM);
	void MakeFrameSectionData(CFEMManage *pFEM, CPlateBasicIndex *pBx);
	void MakeTDMData(CFEMManage *pFEM);
	void MakeTDNData(CFEMManage *pFEM, CPlateBasicIndex *pBx);
	void MakeSlabSelfLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);

	void MakePostSelfLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeLiveLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakePeopleLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeTankLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeTrailerLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeWindLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeCrashLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeCentrifugalLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex);
	void MakeTendenLoad(CFEMManage *pFEM);
	void MakeContructionStage(CFEMManage *pFEM);

	// 집중하중 재하(dDist : 전체요소의 시점에서의 이격거리)
	void AddPointForceAtLeft(CFEMManage *pFEM, double dDist, CVector vForce, long nLoadCase, CString szLoadName="", CString szGroupName="");
	// 교량 중심을 기준으로 좌우측을 판별
	BOOL IsLeft(CPlateBasicIndex *pBx, double dDist);
	// 분포하중 재하함(dDistStt : 전체요소의 시점에서의 이격거리)
	void AddTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, double dForceStt, double dForceEnd, long nLoadCase, CString szLoadName="", CString szGroupName="", BOOL bRatio=FALSE);
	long GetLoadApplyMemberCount(CFEMManage *pFEM);
	BOOL SetDBLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nG, double dSttRoadway, double dEndRoadway, 
		long &nLoadCase, CHLLLaneUnit *pHLLLaneUnit, long nQtyLane, CString szLoadName="", CString szGroupName="");
	void InsertPreLane(CFEMManage *pFEM, CHLLLane *pHLLLane, CHLLLaneUnit *pHLLLaneUnit, long nLane, long nLoadCase, CString szLoadName="", CString szGroupName="");
	void InsertPreLaneCentri(CFEMManage *pFEM, CHLLLane *pHLLLane, CHLLLaneUnit *pHLLLaneUnit, long nLane, long nLoadCase, double dDis[3], double dForce[3], double dMoment[3], CString szLoadName="", CString szGroupName="");
	// 단위집중하중 재하(dDist : 전체요소의 시점에서의 이격거리)
	BOOL AddUnitPointForceAtLeft(CFEMManage *pFEM, double dDist, long nLoadCase, CString szLoadName="", CString szGroupName="", double dValue=1);
	// 단위분포하중 재하(dDistStt : 전체요소의 시점에서의 이격거리)
	BOOL AddUnitTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, long nLoadCase, CString szLoadName="", CString szGroupName="");
	void SetWindLoadForce(long nJijum);											// 풍하중 모델링 수행전 계산
	double GetWindLoad(double dBridgeWidth, double dBridgeHeight, double dD);	// 풍하중 크기 구하기
	double GetElemLength(CFEMManage *pFemMange, long nElm);						// 요소의 길이를 구함
	double GetRadius(long nPosition);															// 곡률반경 구함
	void GetUnitCentrifugalLoad(double dLoad[3], double dDist[3]);
	// 플랜지 폭, 웹 두께
	void SeekFlange_Web(double& dFlange, double& dWeb);
	BOOL SetCentrifugalLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nG, double dSttRoadway, 
		double dEndRoadway, long &nLoadCase, CHLLLaneUnit *pHLLLaneUnit, long nQtyLane, 
		double dCantilDist[3], double dForce[3], double dMoment[3], CString szLoad="", CString szGroup="");
	BOOL AddPointForceMomentAtLeft(CFEMManage *pFEM, double dDist, long nLoadCase, CVector vForce, CVector vMoment, CString szLoadName="", CString szGroupName="");
	void CalcCarLoad(double dValue[3], double dDis[2]);
	long GetElementIndexFromDistance(CFEMManage *pFEM, double dDis);	
};

#endif // !defined(AFX_FEMCANTILEVERCALC_H__9C1D6A71_0F53_40C8_BC17_FA20221402ED__INCLUDED_)
