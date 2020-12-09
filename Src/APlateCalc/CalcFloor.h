// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// CalcFloor.h: interface for the CCalcFloor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCFLOOR_H__475A4A3B_D02A_4489_BD1E_B6B1A49B33B4__INCLUDED_)
#define AFX_CALCFLOOR_H__475A4A3B_D02A_4489_BD1E_B6B1A49B33B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define REBAR_PART_GENERAL		0	// �Ϲݺ�
#define REBAR_PART_END			1	// �ܺ�
#define REBAR_PART_MIDDLE		2	// �߰�������

#define REBAR_TYPE_MAIN		0	// ��ö��
#define REBAR_TYPE_SUPPORT	1	// ���ö��

#define REBAR_CYCLE_1		0	// 1 Cycle
#define REBAR_CYCLE_2		1	// 2 Cycle

class CDataManage;
class CPlateBasicIndex;
class CAPlateCalcStd;
class CFEMManage;

class AFX_EXT_CLASS CCalcFloor
{
public:
	CCalcFloor(CDataManage *pDataManage);
	virtual ~CCalcFloor();

	CDataManage *m_pDataManage;
public:
	double  GetLengthArcBy3Point(CDPoint xyCen, CDPoint xyStt, CDPoint xyEnd);
	long    SetFloorStatus(long nHLL);
	long	GetGuardWallType(long nSide);

	long	CalcSlabLeftDeadLoad(CStringArray& arr);
	long	CalcSlabRightDeadLoad(CStringArray& arr);
	long	CalcSlabCenterDeadLoad(CStringArray& arr);
	double	CalcCantileverPreDeadLoad(long nSide);	///< FLOOR_LEFT : ���� FLOOR_RIGHT : ���� - �����꿡 ���� ���Ʈ
	double	CalcCantileverPostDeadLoad(long nSide);	///< FLOOR_LEFT : ���� FLOOR_RIGHT : ���� - �����꿡 ���� ���Ʈ
	double	CalcPrestressLength(long nSide);
	double	CalcPrestressTotAngle(long nSide);
	double	GetFloorConcAreaCoefficient();
	double	GetFloorJujangStd();
	// ��ȣ��
	BOOL	GetCalcGuardRail(CPlateBasicIndex *pBx, double &dDist, double &dForce, BOOL bIsLeft);
	BOOL    GetCalcJungBunDe(CPlateBasicIndex *pBx, double &dForce);
protected:
	// ���� ��ȣ��
	void	CalcGuardLeftType0(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType1(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType2(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType3(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType4(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType5(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType6(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType7(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType8(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType99(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardLeftType100(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);

	// ���� ��ȣ��
	void	CalcGuardRightType0(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType1(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType2(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType3(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType4(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType5(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType6(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType7(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType8(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType99(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);
	void	CalcGuardRightType100(CPlateBasicIndex *pBx, CStringArray& arr, double LEN);

// bIsLeft : TRUE -> ���� ��ȣ�� FALSE -> ���� ��ȣ��
	void	GetCalcGuardType1(double &dDist, double &dForce, double LEN, BOOL bIsLeft);
	void	GetCalcGuardType2(double &dDist, double &dForce, double LEN, BOOL bIsLeft);
	void	GetCalcGuardType3(double &dDist, double &dForce, double LEN, BOOL bIsLeft);
	void	GetCalcGuardType4(double &dDist, double &dForce, double LEN, BOOL bIsLeft);
	void	GetCalcGuardType5(double &dDist, double &dForce, double LEN, BOOL bIsLeft);
	void	GetCalcGuardType6(double &dDist, double &dForce, double LEN, BOOL bIsLeft);
	void	GetCalcGuardType7(double &dDist, double &dForce, double LEN, BOOL bIsLeft);
	void	GetCalcGuardType8(double &dDist, double &dForce, double LEN, BOOL bIsLeft);

	void CalcSlabLeft(CPlateBasicIndex *pBx, CStringArray& arr, double LEN, double &dTotWeight, double &dTotMoment);			///< return : Sum Moment 
	void CalcSlabRight(CPlateBasicIndex *pBx, CStringArray& arr, double LEN, double &dTotWeight, double &dTotMoment);
	double	m_dPreDeadLoad[3];
	double	m_dPostDeadLoad[3];

	map<CString, double> m_MapPreStress;// GetSectionStress

	long m_nTypeHDan;
	double m_dAppConcreteEC;//���� ��ũ��Ʈ ���(ö��/ Ȥ�� ����)
	void GetAppConcreteEC(long nTypeHDan);

public:
	void SetSlabMinThick(long nSide);
	void SetLiveLoad(long nSide);
	void SetLiveLoadCenter();
	void SetWindLoad(long nSide);
	void SetCollisionLoad(long nSide);
	void SetCentriodLoad(long nSide);
	void NoneWalkLoad(long nSide);
	void WalkLoad(long nSide);
	void SetDataInitInnerSlab();
	void SetLoadComposite(long nSide);
	void SetLoadCompositeCantilever(long nSide);
	void SetLoadCompositeCenter();
	void SetCantileverRebar(long nSide, long nCheckType);
	void SetInnerJijumRebar();
	double GetCantileverDroop(long nSide);
	void CalcPreStress(CMap <CString, LPCTSTR, double, double>& Map, long nSide);		///< ������Ʈ���� �ս�
	void CalcPreStressLoss(CAPlateCalcStd* pCalc, CMap <CString, LPCTSTR, double, double>& Map, long nSide);	//<������Ʈ���� �ս�> 2009.07.20 �ں���
	void CalcPreStressCheck(CAPlateCalcStd* pCalc, CMap <CString, LPCTSTR, double, double>& Map, long nSide);	//<������Ʈ���� üũ> 2009.07.23 �ں���
	void SetCantileverCrack(CMap <CString, LPCTSTR, double, double>& Map, long nSide);			///< �տ� ����
	void SetFloorExperienceDesign();
	void SetCantileverUltimateLoad(long nSide, CMap<CString, LPCTSTR, double, double> &Map);		// �Ҽ����� �������߰���
	void CalcCantileverUltimateLoadCheck(CAPlateCalcStd* pCalc, CMap<CString, LPCTSTR, double, double> &Map, long nSide);	// �������߿� ���� �ܸ���� (2009.07.24)
	void CalcCantileverInnerPositionCheck(CMap<CString, LPCTSTR, double, double> &Map);		// ���� ������ ����� ���� (2009.07.27)
	void CalcCantileverFixedCheck(CMap<CString, LPCTSTR, double, double> &Map);	// ������ ���� ���� (2009.07.27)
	void DesignExpansionJointUltimateLoad(CMap<CString, LPCTSTR, double, double> &Map);
	void DesignFixingPlate(CMap<CString, LPCTSTR, double, double> &Map);

	long GetTangentCircle(CDPoint xyPoint, CDPoint xyCircleCen, double dCircleR, CDPoint& xyResult1, CDPoint& xyResult2);
	void GetTransPointDistance(CDPointArray& xyArr, CDoubleArray& AngArr);
	void SeekFlange_Web(double& dFlange, double& dWeb);
	double GetDistance(CDPoint& p1, CDPoint& p2);
	void ReSetTendonDistance(CDPointArray& PosArr, long nPos, CDoubleArray& DisArr);
	void GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr);
	double GetMemberForce(CFEMManage* pFEM, CString szLoad, long nForceType, long nLoadPos);

	void SetAnchorageRebar();
	double AreaReturn(long AsNum);

	CConcBasic *MakeConcBasicData(FloorType eType, long nPart);		// nPos = LEFT_CANTILEVER_FLOOR:����ĵƿ���� / RIGHT_CANTILEVER_FLOOR:����ĵƿ���� / CENTER_FLOOR:�߾Ӻ�, nPart = REBAR_PART_GENERAL:�Ϲݺ� / REBAR_PART_END:�ܺ� / REBAR_PART_MIDDEL:�߰�������
	void SetConcBasicDataDesign(CConcBasic *pConc);				// �������� ����
	void MakeCalculateOutputFile(FloorType eType);
	void MakeOutputResultFile(FloorType eType);

	enum CheckFileName{eFile_Design, eFile_Crack, eFile_Support, eFile_Deflection, eFile_LengthRate, eFile_EndPart, eFile_Experience, eFile_ExperienceEnd, eFile_ExperienceCondition};

	CString GetFileNameBySubject(CheckFileName eFile);
	CString GetFileNameResult();

	double GetRebarCTC(long nPos, long nPart, long nRebarType, long nCycle, BOOL bUpper);									// nPos = LEFT_CANTILEVER_FLOOR:����ĵƿ���� / RIGHT_CANTILEVER_FLOOR:����ĵƿ���� / CENTER_FLOOR:�߾Ӻ�, nPart = REBAR_PART_GENERAL:�Ϲݺ� / REBAR_PART_END:�ܺ� / REBAR_PART_MIDDEL:�߰�������, nRebarType =  REBAR_TYPE_MAIN:��ö�� / REBAR_TYPE_SUPPORT:���ö��
	double GetRebarDia(long nPos, long nPart, long nRebarType, long nCycle, BOOL bUpper);	// nPos = LEFT_CANTILEVER_FLOOR:����ĵƿ���� / RIGHT_CANTILEVER_FLOOR:����ĵƿ���� / CENTER_FLOOR:�߾Ӻ�, nPart = REBAR_PART_GENERAL:�Ϲݺ� / REBAR_PART_END:�ܺ� / REBAR_PART_MIDDEL:�߰�������, nRebarType =  REBAR_TYPE_MAIN:��ö�� / REBAR_TYPE_SUPPORT:���ö��, nCycle = REBAR_CYCLE_1:1Cycle / REBAR_CYCLE_2:2Cycle

	BOOL IsLaneLoad(long nSide);	// FLOOR_LEFT, FLOOR_RIGHT, FLOOR_MID
};

#endif // !defined(AFX_CALCFLOOR_H__475A4A3B_D02A_4489_BD1E_B6B1A49B33B4__INCLUDED_)
