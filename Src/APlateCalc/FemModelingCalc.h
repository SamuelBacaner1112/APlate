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
	///< �ռ��� ��������, �ռ��� ������ ����, �ռ��� �ϣO, Ȱ����, ����ħ��, ��ü����
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

	// ����Լ�////////////////////////////////////////////////////////////////////	
	BOOL IsLiveCombMaxLC(CString szLC);

	BOOL IsRestraintJoint(long nJoint);

	long GetShoeJointNoByBx(CPlateBasicIndex *pBx);
	
	CDPoint GetStdAngle();
	
	///< �κ������������ϱ�
	double CalcBonusRate();

	///< �ܸ���(�Ŵ�)
	void GetSectionCoefficient(long nG, double dSta, BOOL bEffectWidth=FALSE, double dStaEffect=0, long nPos=0);
	void GetSectionCoefficient(CPlateBasicIndex *pBx, BOOL bEffectWidth=FALSE, double dStaEffect=0, long nPos=0);
	double GetThickFlange(CPlateBasicIndex *pBx, long nMarkBuje, long nPos);
	double GetThickFlange(long nG, double dSta, long nMarkBuje, long nPos);
	double GetWidthFlange(CPlateBasicIndex *pBx, long nMarkBuje, long nPos);
	double GetWidthFlange(long nG, double dSta, long nMarkBuje, long nPos);
	double GetHeightWeb(CPlateBasicIndex *pBx, long nPos);

	///< �ܸ���(���κ�)
	void GetSectionCoefficientCR(CPlateBasicIndex *pBx);
	
	///< �÷��� ��ȿ�� ���ϱ�
	double GetEffectWidthFlange(long nG, double dSta, BOOL bUpper=TRUE);
	
	///< ������ ��ȿ�� ���ϱ�
	double GetEffectWidthSlab(long nG, double dSta);
	
	///< ��ȿ��(�÷���, ������)�� ���ϱ� ���� [���� ������]�� ���ϱ�
	double GetEffectWidthJiganLength(long nG, double dSta);
	
	///< ��ȿ��(�÷���, ������)�� ���ϱ� ���� [���� ������]�� ���ϱ� �����Լ�
	double GetEffectWidthJiganLength(long nG, long nK, BOOL bJijum); 
	
	///< ��ȿ��(�÷���, ������)�� ���ϱ� ���� Lambda ���ϱ�
	double GetEffectWidthLambda(double b, double l, BOOL bPier); 
	
	///< �߾Ӻ����� ���������� �Ǵ�	
	BOOL   IsPierEffectWidthSlab(long nG, double dSta);
	
	///< ���� Bx�� ������ȣ
	long   GetJointNoByBx(CPlateBasicIndex* pBx);
	
	///< ���κ� Bx�� �̿��Ͽ� ���κ��� ��Ī�Ǵ� ���
	double GetElemByCrossBx(CPlateBasicIndex* pBx);
	
	///<��ҳ� ������ġ������ �����̼�
	double GetStationByElmAndRatio(long nElem, double dRatio);
	
	///<�Ŵ��� �����̼��� �̿��Ͽ� K��
	double GetValueOfInfK(long nG, double dSta);
	
	///< �ش� JOINT�� �����̸�(�� : A1, A2, P1, P2...)
	BOOL   GetJijumNameOfJoint(long nIdx, CString& szName);
	
	///< �ش� JOINT�� ������ȣ
	long   GetJijumNoOfJoint(long nIdx);
	
	///< nG�Ŵ��� ���� JOINT�� ������ JOINT
	void   GetJointRangeByGirder(long nG, long& nStt, long& nEnd);
	
	///< nG�Ŵ��� ���� Elem�� ������ Elem
	///< ����: �Ŵ����� ��Ҽ� 
	double GetElemRangeByGirder(long nG, long& nStt, long& nEnd);
	
	///< ������ȣ�� �̿��Ͽ� �Ŵ���ȣ ��������	
	long   GetGirderNoByJointID(long nJoint);
	
	///< ��ҹ�ȣ�� �̿��Ͽ� �Ŵ���ȣ ��������
	long   GetGirderNoByElemID(long nElem);
	///< ����� ����
	double GetElemLength(long nElm, BOOL bApplyZ=TRUE);
	
	///< nG�Ŵ� �� ����� �Ǵ� ������ ��  �����ݷ��� ���� ū ���� 
	CJoint* GetJointMaxForceAtJijum(long nForceCase, BOOL bPier);

	///< nSttGirNum�Ŵ��� nEndGirNum�Ŵ����̿��� ���� ó���� ū ��
	double GetCalcMaxDisplace(long nSttGirNum, long nEndGirNum);
	
	///<  �� BX�� I33�� ������ ����(I33�� �����ϸ� �ܸ鵵 �����ϴٰ� ������)
	BOOL IsSameSectionBetweenTwoBx(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nAnalysisType);
	
	///< �ռ����� �Լ��ĸ� �����Ͽ� ���� BX�� �ܸ��̸��� ����
	CString GetSectionNameByBx(CFEMManage *pFemData, CPlateBasicIndex *pBx, long nAnalysisType);
	BOOL GetPositionBySectionName(CString szSecName, long &nG, double &dSta, long nType);
	
	///< I33���� �̿��Ͽ� �ܸ��� ������.I33
	CFrameSection* GetFrameSectionByI33(CFEMManage *pFemData, double dSttJointI33);

	///< Ⱦ�й踦 ������������ �� ��� ���� ����� ��ġ�� ��� Ⱦ�й豸���� �ִ��� ����
	long GetHLLNoByElemID(long nElm);
	long GetHLLNoByElemID(long nElm, BOOL bSttNode);
	BOOL IsTaperElement(long nElm, double &dRate);
	double GetStationTaper(long nElm, BOOL bStt);
	///< �Ŵ��� �����̼ǰ��� �̿��� �ش� ��ҹ�ȣ�� �����ϰ� ���̺���---[(�����̼���ġ-����)/��ұ���]

	long GetElemNoAndRatio(long nG, double dSta, double& dRatio);
	long GetElemNoAndRatioAtInfK(long nG, double dSta, double& dRatio);
	long GetElemNoShoeByJijumBx(CPlateBasicIndex *pBx);
	
	///< �Ŵ��� �����̼��� �̿��� ��� ��ġ�Ÿ�(m_StudJong_StudDis)�� ����	
	double GetUseBasePitchByStation(long nG, double dSta, BOOL bEndPos=FALSE);	
	
	///< �Ŵ��� �����̼��� �̿��� ��� ���ܿ�������� ����	
	long  GetUseStudSuByStation(long nG, double dSta, BOOL bPositive);

	///< nSecIndex �ܸ�� ������ �ܸ� �����߿��� �ִ밪�� �߻��� ���� ��ġ�� ����.
	long GetMaxForcePositionOfSection(long nSecIndex, long &nG, double &dSta, int nForceType, long nResultType=MAX_ABS, BOOL bSectionCheck=TRUE);
	
	///< �ִ��Ƿΰ� ��Ÿ���� �ܸ��� ã�´�.
	long GetMaxFatigueIndex(long nType, long &nEleIdx, double &dRatio, long nLC);

	void RemoveDupTypedArray(CTypedPtrArray < CObArray, CPlateBasicIndex* > &DArrBx);

	void GetBxArrayOfVBracing(CTypedPtrArray < CObArray, CPlateBasicIndex* > &DArrVBracingBx);

	// pBx�� ������ ���°� ���� ������ �����(ELE_MOMENT3)�� ���� ū ���κ�
	CPlateBasicIndex* GetBxCrossBeamMaxMomentByMatType(CPlateBasicIndex *pBx);

	// pBx�� ������ ���°� ���� �����극�̽��� �����(ELE_MOMENT3)�� ���� ū �����극�̽�
	CPlateBasicIndex* GetBxVBracingMaxMomentByMatType(CPlateBasicIndex *pBx);
	
	CPlateBasicIndex* GetBxCrossBeamMaxMoment(BOOL bJijum);

	///< ������ȣ�� �Է� �޾� ���ܿ����� ��ġ������ ����
	long GetShearChainSu(long nG, long nSttNodeNo, long nEndNodeNo);

	///< Ȱ������ ó�����並 ���� ó���� ����.
	double GetLiveLoadDisplace(int nLoadCase, int nNodIdx);
	double GetLiveLoadReaction(long nLoadType, long nLoadGir, long nLane, long nNodIdx, BOOL &bExist);
	
	///< Map�� �ӽ� ����� ��갪�� �ҷ����� �Լ�
	double GetMapValueSecCo(CString sz);
	double GetMapValueSecCoCR(CString sz);
	double GetMaxSlabWidth(long nG, double dSta);

	void GetGirderFrameSectionArray(vector <CSectionJewon> *pJewonArray, long nAnalysisType);	// �Ŵ� �ܸ�Array
	void GetCrossBeamFrameSectionArray(vector <CSectionJewon> *pJewonArray);					// ���κ� �ܸ�Array

	double GetSectionCoeffAs(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);
	double GetSectionCoeffI33(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);
	double GetSectionCoeffI22(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);
	double GetSectionCoeffK(long nAnalysisType, CPlateBasicIndex *pBx=NULL, BOOL bNext=TRUE);

	// �ܸ����� �ִ���Ʈ Station���� �ܸ����� Station�� ����
	double GetStationSecJewon(long nMatNo, long nG, double dSta, long &nPosSec);
	BOOL IsSameI33(double dI331, double dI332, double dError=0.0000001);
	long GetMatNo(long nG, double dSta);

	enum {DBL, DBR, DLL, DLR, TANK, TRAILER, PEOPLE, WIND, LWIND, RWIND};
};

#endif // !defined(AFX_FEMMODELINGCALC_H__8E73417B_A5FD_4183_A35A_57A441F3994E__INCLUDED_)
