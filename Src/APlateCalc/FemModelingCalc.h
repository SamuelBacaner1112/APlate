// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// FemModelingCalc.h: interface for the CFemModelingCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEMMODELINGCALC_H__8E73417B_A5FD_4183_A35A_57A441F3994E__INCLUDED_)
#define AFX_FEMMODELINGCALC_H__8E73417B_A5FD_4183_A35A_57A441F3994E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBasicIndex;
class CDomyun;
class CSectionJewon;
class CFrameSection;

class AFX_EXT_CLASS CFemModelingCalc  
{
public:
	CFemModelingCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd);
	virtual ~CFemModelingCalc();

public:
	CDataManage *m_pDataManage;
	CAPlateCalcStd *m_pCalcStd;
	CProgressCtrl m_ctlProgress;

	CMap<CString, LPCTSTR, double, double> m_MapSecCoe;		// GetSectionCoefficient
	CMap<CString, LPCTSTR, double, double> m_MapSecCoeCR;	// GetSectionCoefficientCR

public:
	long GetFrameAnalysisSize();
	double GetWidthSlab(long nG, double dSta);
	CFrameSection* GetFrameSectionByPrpty(CFEMManage *pFemData, CDPoint* Prpty, CString* Material=NULL);
	void MakeFrameModeling();
	long GetJiganNoByJointID(long nJoint);
	long GetJointNoByStation(long nG, double dSta, double& dRatio);
	///< 합성전 강재자중, 합성전 슬래브 자중, 합성후 하즁, 활하중, 지점침하, 전체하중
	enum {SELF_W=0, SLAB_W=1, POST_W=2, LIVE_W=3, SETTLE_W=4, TOTAL_W=5};
	enum {MAX_POSITIVE=0, MAX_NEGATIVE=1, MAX_ABS=2};

	void MakeResultData();
	void MakeSummaryOfElementForce();
	void MakeSummaryOfJointForce();
	void MakeMaxMinForceDB();
	void MakeMaxMinForceDL();
	void MakeMaxMinForceMTY();
	void MakeMaxMinForcePeople();
	void MakeMaxMinForceWind();

	void MakeJoint(CFEMManage *pFemData);
	void MakeElement(CFEMManage *pFemData);	
	void MakeElementDefineSection(CFEMManage *pFemData, long nSection, long nAnalysisType);
	long MakeFrameSection(CFEMManage *pFemData, long nAnalysisType);	
	void MakeForceLive(CFEMManage *pFemData, long &nLoadCase);
	void MakeForceWind(CFEMManage *pFemData, long &nLoadCase);
	void MakeForcePreDead(CFEMManage *pFemData, long nAnalysisType, long &nLoadCase);
	void MakeForcePreDead_Turn(CFEMManage *pFemData, long nAnalysisType, long &nLoadCase);
	void MakeForcePostDead(CFEMManage *pFemData, long &nLoadCase);
	void MakeMaterial(CFEMManage *pFemData);
	void MakeLane(CFEMManage *pFemData);
	void MakeVehicleClass(CFEMManage *pFemData);
	void MakeVehicle(CFEMManage *pFemData);
	void MakeMovingLoad(CFEMManage *pFemData);
	void MakeMovingLoadCombo(CFEMManage *pFemData);
	void MakeUZPLoad(CFEMManage *pFemData);
	void MakeUZPCombo(CFEMManage *pFemData);
	void MakeCombinationIndex(long nCnt, long nTotal, CLongArray& IndexArr);

	// 계산함수////////////////////////////////////////////////////////////////////	
	BOOL IsLiveCombMaxLC(CString szLC);

	BOOL IsRestraintJoint(long nJoint);

	long GetShoeJointNoByBx(CPlateBasicIndex *pBx);
	
	CDPoint GetStdAngle();
	
	///< 부부재할증률구하기
	double CalcBonusRate();

	///< 단면계수(거더)
	void GetSectionCoefficient(long nG, double dSta, BOOL bEffectWidth=FALSE, double dStaEffect=0, long nPos=0);
	void GetSectionCoefficient(CPlateBasicIndex *pBx, BOOL bEffectWidth=FALSE, double dStaEffect=0, long nPos=0);
	double GetThickFlange(CPlateBasicIndex *pBx, long nMarkBuje, long nPos);
	double GetThickFlange(long nG, double dSta, long nMarkBuje, long nPos);
	double GetWidthFlange(CPlateBasicIndex *pBx, long nMarkBuje, long nPos);
	double GetWidthFlange(long nG, double dSta, long nMarkBuje, long nPos);
	double GetHeightWeb(CPlateBasicIndex *pBx, long nPos);

	///< 단면계수(가로보)
	void GetSectionCoefficientCR(CPlateBasicIndex *pBx);
	
	///< 플랜지 유효폭 구하기
	double GetEffectWidthFlange(long nG, double dSta, BOOL bUpper=TRUE);
	
	///< 슬래브 유효폭 구하기
	double GetEffectWidthSlab(long nG, double dSta);
	
	///< 유효폭(플랜지, 슬래브)을 구하기 위한 [적용 지간장]을 구하기
	double GetEffectWidthJiganLength(long nG, double dSta);
	
	///< 유효폭(플랜지, 슬래브)을 구하기 위한 [적용 지간장]을 구하기 서브함수
	double GetEffectWidthJiganLength(long nG, long nK, BOOL bJijum); 
	
	///< 유효폭(플랜지, 슬래브)을 구하기 위한 Lambda 구하기
	double GetEffectWidthLambda(double b, double l, BOOL bPier); 
	
	///< 중앙부인지 지점부인지 판단	
	BOOL   IsPierEffectWidthSlab(long nG, double dSta);
	
	///< 현재 Bx의 절점번호
	long   GetJointNoByBx(CPlateBasicIndex* pBx);
	
	///< 가로보 Bx를 이용하여 가로보에 대칭되는 요소
	double GetElemByCrossBx(CPlateBasicIndex* pBx);
	
	///<요소내 비율위치에서의 스테이션
	double GetStationByElmAndRatio(long nElem, double dRatio);
	
	///<거더와 스테이션을 이용하여 K값
	double GetValueOfInfK(long nG, double dSta);
	
	///< 해당 JOINT의 지점이름(예 : A1, A2, P1, P2...)
	BOOL   GetJijumNameOfJoint(long nIdx, CString& szName);
	
	///< 해당 JOINT의 지점번호
	long   GetJijumNoOfJoint(long nIdx);
	
	///< nG거더의 시작 JOINT와 마지막 JOINT
	void   GetJointRangeByGirder(long nG, long& nStt, long& nEnd);
	
	///< nG거더의 시작 Elem와 마지막 Elem
	///< 리턴: 거더안의 요소수 
	double GetElemRangeByGirder(long nG, long& nStt, long& nEnd);
	
	///< 절점번호를 이용하여 거더번호 가져오기	
	long   GetGirderNoByJointID(long nJoint);
	
	///< 요소번호를 이용하여 거더번호 가져오기
	long   GetGirderNoByElemID(long nElem);
	///< 요소의 길이
	double GetElemLength(long nElm, BOOL bApplyZ=TRUE);
	
	///< nG거더 중 교대부 또는 교각부 중  지점반력이 가장 큰 절점 
	CJoint* GetJointMaxForceAtJijum(long nForceCase, BOOL bPier);

	///< nSttGirNum거더와 nEndGirNum거더사이에서 가장 처짐이 큰 값
	double GetCalcMaxDisplace(long nSttGirNum, long nEndGirNum);
	
	///<  두 BX의 I33이 같은지 여부(I33이 동일하면 단면도 동일하다고 가정함)
	BOOL IsSameSectionBetweenTwoBx(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nAnalysisType);
	
	///< 합성전과 함성후를 구분하여 현재 BX의 단면이름을 리턴
	CString GetSectionNameByBx(CFEMManage *pFemData, CPlateBasicIndex *pBx, long nAnalysisType);
	BOOL GetPositionBySectionName(CString szSecName, long &nG, double &dSta, long nType);
	
	///< I33값을 이용하여 단면을 리턴함.I33
	CFrameSection* GetFrameSectionByI33(CFEMManage *pFemData, double dSttJointI33);

	///< 횡분배를 여러구간에서 할 경우 현재 요소의 위치가 어느 횡분배구간에 있는지 결정
	long GetHLLNoByElemID(long nElm);
	long GetHLLNoByElemID(long nElm, BOOL bSttNode);
	BOOL IsTaperElement(long nElm, double &dRate);
	double GetStationTaper(long nElm, BOOL bStt);
	///< 거더와 스테이션값을 이용해 해당 요소번호를 참조하고 길이비율---[(스테이션위치-시점)/요소길이]

	long GetElemNoAndRatio(long nG, double dSta, double& dRatio);
	long GetElemNoAndRatioAtInfK(long nG, double dSta, double& dRatio);
	long GetElemNoShoeByJijumBx(CPlateBasicIndex *pBx);
	
	///< 거더와 스테이션을 이용해 사용 피치거리(m_StudJong_StudDis)를 구함	
	double GetUseBasePitchByStation(long nG, double dSta, BOOL bEndPos=FALSE);	
	
	///< 거더와 스테이션을 이용해 사용 전단연결재수를 구함	
	long  GetUseStudSuByStation(long nG, double dSta, BOOL bPositive);

	///< nSecIndex 단면과 동일한 단면 구간중에서 최대값이 발생한 곳의 위치를 구함.
	long GetMaxForcePositionOfSection(long nSecIndex, long &nG, double &dSta, int nForceType, long nResultType=MAX_ABS, BOOL bSectionCheck=TRUE);
	
	///< 최대피로가 나타나는 단면을 찾는다.
	long GetMaxFatigueIndex(long nType, long &nEleIdx, double &dRatio, long nLC);

	void RemoveDupTypedArray(CTypedPtrArray < CObArray, CPlateBasicIndex* > &DArrBx);

	void GetBxArrayOfVBracing(CTypedPtrArray < CObArray, CPlateBasicIndex* > &DArrVBracingBx);

	// pBx와 이음판 형태가 같은 가로중 부재력(ELE_MOMENT3)이 가장 큰 가로보
	CPlateBasicIndex* GetBxCrossBeamMaxMomentByMatType(CPlateBasicIndex *pBx);

	// pBx와 이음판 형태가 같은 수직브레이싱중 부재력(ELE_MOMENT3)이 가장 큰 수직브레이싱
	CPlateBasicIndex* GetBxVBracingMaxMomentByMatType(CPlateBasicIndex *pBx);
	
	CPlateBasicIndex* GetBxCrossBeamMaxMoment(BOOL bJijum);

	///< 절점번호를 입력 받아 전단연결재 설치수량을 구함
	long GetShearChainSu(long nG, long nSttNodeNo, long nEndNodeNo);

	///< 활하중의 처짐검토를 위한 처짐값 구함.
	double GetLiveLoadDisplace(int nLoadCase, int nNodIdx);
	double GetLiveLoadReaction(long nLoadType, long nLoadGir, long nLane, long nNodIdx, BOOL &bExist);
	
	///< Map에 임시 저장된 계산값을 불러오는 함수
	double GetMapValueSecCo(CString sz);
	double GetMapValueSecCoCR(CString sz);
	double GetMaxSlabWidth(long nG, double dSta);

	void GetGirderFrameSectionArray(vector <CSectionJewon> *pJewonArray, long nAnalysisType);	// 거더 단면Array
	void GetCrossBeamFrameSectionArray(vector <CSectionJewon> *pJewonArray);					// 가로보 단면Array

	double GetSectionCoeffAs(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);
	double GetSectionCoeffI33(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);
	double GetSectionCoeffI22(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);
	double GetSectionCoeffK(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);

	// 단면검토시 최대모멘트 Station으로 단면제원 Station을 구함
	double GetStationSecJewon(long nMatNo, long nG, double dSta, long &nPosSec);
	BOOL IsSameI33(double dI331, double dI332, double dError=0.0000001);
	long GetMatNo(long nG, double dSta);

	enum {DBL, DBR, DLL, DLR, TANK, TRAILER, PEOPLE, WIND, LWIND, RWIND};
};

#endif // !defined(AFX_FEMMODELINGCALC_H__8E73417B_A5FD_4183_A35A_57A441F3994E__INCLUDED_)
