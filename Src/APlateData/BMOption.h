// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BMOption.h: interface for the CBMOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMOPTION_H__4C22AC7B_964D_4445_8E36_7B65E64369D0__INCLUDED_)
#define AFX_BMOPTION_H__4C22AC7B_964D_4445_8E36_7B65E64369D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BRIDGE_STT_POINT			0			// ���� ����
#define BRIDGE_END_POINT			1			// ���� ����
#define GENERAL_GUARDFENCE			0			// �Ϲ� ��ȣ��
#define ABSOLUTE_GUARDFENCE			1			// ���� ��ȣ��
#define GENERAL_SOUNDPROOF			2			// �Ϲ� ������
#define ABSOLUTE_SOUNDPROOF			3			// ���� ������
#define CENTER_GUARDFENCE			4			// �ߺд�
#define PARAPET						5			// ��  ��
#define CURB						6			// ��  ��
#define DRIVEWAY					0			// ������
#define FOOTWAY						1			// ������
#define DRIVE_FOOT_BOUNDARY			2			// ������ ����
#define RIVER						0			// ��õ��
#define WALKWAY						1			// ������
#define RIVERWALKWAY				2			// ��õ������
#define TIMBER					    0			// ����
#define STEELTUBE					1			// ����

#define BMOPT_CONCRETE_BYE			1001		// ��ũ��Ʈ ����
#define BMOPT_CONCRETE_PLACING		1002		// ��ũ��Ʈ Ÿ��
#define BMOPT_MOLD					1003		// ��Ǫ��
#define BMOPT_SPACER				1004		// �����̼�
#define BMOPT_REBAR_MANUFACTURE		1005		// ö�� ���� ����
#define BMOPT_PAVE					1006		// ����
#define BMOPT_TACKCOATING			1007		// ������
#define BMOPT_DRAINBRIDGESURFACE	1008		// ������
#define BMOPT_BRIDGENAME			1009		// ������
#define BMOPT_BRIDGENAME_PLATE		1010		// ������
#define BMOPT_EXPLAIN_PLATE			1011		// ������
#define BMOPT_ESTABLISH_TBM			1012		// TBM ��ġ
#define BMOPT_STYROFOAM				1013		// ��Ƽ����
#define BMOPT_EXPANSIONJOINT		1014		// ��������
#define BMOPT_EXPANSIONJOINT_COVER	1015		// �������� ����
#define BMOPT_SHRINKAGE_CONCRETE	1016		// ������ ��ũ��Ʈ
#define BMOPT_WATER_DRAW			1017		// ���� ������
#define BMOPT_NOTCH					1018		// NOTCH
#define BMOPT_GUARD_FENCE			1019		// �����潺
#define BMOPT_SOUND_PROOF			1020		// �����ǳ�
#define BMOPT_PARAPET				1021		// ����
#define BMOPT_SHADE					1022		// ������
#define BMOPT_DROPPING_PREVENT		1023		// ���Ϲ� ������
#define BMOPT_STEEL_QUANTITY		1024		// ���� ����
#define BMOPT_PAINT					1025		// ���� ����
#define BMOPT_DRAIN_ESTABLISH		1026		// ��� �ü�
#define BMOPT_WING_WALL				1027		// ������
#define BMOPT_TIMBER				1028		// ���ٸ�
#define BMOPT_ELECWIREHOLE			1029		// ������

#define BMOPT_EXPANSIONJOINT_MAX	2			// ��������

#define BMOPT_CHKSHEET				9			// ��ü ������꼭 
#define BMOPT_CHKSH_2ND				4			//	2�� �����׸�
#define BMOPT_CHKSH_3TH				3			//	3�� �����׸�
#define BMOPT_CHKSH_4TH				3			//	4�� �����׸�
#define BMOPT_CHKSH_5TH				2			//	5�� �����׸�
#define BMOPT_CHKSH_8TH				2			//	8�� �����׸�
#define BMOPT_CHKSH_9TH				4			//	9�� �����׸�


class AFX_EXT_CLASS CBMOption  : public CObject
{
public:
	CBMOption();
	virtual ~CBMOption();

	void Serialize(CArchive& ar);
	void SetDataDefault();
	CString GetRebarDia(long nRebarIdx, double dSigmaY=4000.0);
	CString GetExpansionJointDia(BOOL bStt, long nIdx, double dSigmaY);
	CString GetExpansionJointCoverPos(long nIdx);
	CString	GetParapetType(long nIdx);
	CString	GetBMOptItem(long nIdx);
	long	GetRebarDiaIndex(CString szDia);
	CString GetDrainEstablishType();
	long	GetIdxDrainEstablishType(CString szType);
	long	GetIdxTimberQuality(CString szType);
	CString	GetTimberQuality(long nIdx);
	CString GetPosDeckFinisher(long nIdx);
	long	GetIdxDeckFinisher(CString szPos);
	CString	GetRebarManufactureType(long nType);
	long	GetIdxRebarManufactureType(CString szType);
	CString	GetDroppingPrevent(BOOL bApply);
	BOOL	GetDroppingPrevent(CString szApply);
	long	GetNotchPos(CString szPos);
	CString	GetNotchPos(long nPos);

	struct _PAVE
	{
		BOOL	m_bApply;
		double	m_dPaveThick;
	}	m_structPave;					// ����

	struct _TACKCOATING
	{
		BOOL	m_bApply;
		double	m_dTackCoating;
	}	m_structTackCoating;			// �� ����

	struct _BRIDGENAME
	{
		BOOL	m_bApply;		
		BOOL	m_bOneSide;				// ����.����	- ������
		BOOL	m_bSttPoint;			// ����			- ������
		BOOL	m_bEndPoint;			// ����			- ������
		long	m_nQty;					// ������ ����
	}	m_structBridgeName;				// ������

	struct _BRIDGENAMEPLATE
	{
		BOOL	m_bApply;
		BOOL	m_bSttPoint;			// ����	 - ������
		BOOL	m_bEndPoint;			// ����	 - ������
		long	m_nQty;					// ������ ����
	}	m_structBridgeNamePlate;		// ������

	struct	_EXPLAINPLATE
	{
		BOOL	m_bApply;				// ������
		BOOL	m_bSttPoint;			// ����	- ������
		BOOL	m_bEndPoint;			// ����	- ������
		long	m_nQty;					// ������ ����
	}	m_structExplainPlate;			// ������

	struct _ESTABLISHTBM
	{
		BOOL	m_bApply;
		BOOL	m_bSttPoint;			// ����	- ������
		BOOL	m_bEndPoint;			// ����	- ������
		long	m_nQty;					// TBM��ġ ����
	}	m_structEstablishTBM;			// TBM ��ġ

	struct _STYROFOAM
	{
		BOOL	m_bApply;				// ��Ƽ����
		BOOL	m_bLeft;				// TRUE : ��������     FALSE : ��������
		double	m_dThick;				// �β� 10mm or 20mm
	}	m_structStyrofoam;

	struct _EXPANSIONJOINT
	{
		BOOL	m_bApply;				// ��������
		BOOL	m_bUnitLength;			// TRUE : ���� m�� FALSE : ������
		long	m_nSttDiaIdx[BMOPT_EXPANSIONJOINT_MAX];		// ������ ����
		long	m_nEndDiaIdx[BMOPT_EXPANSIONJOINT_MAX];		// ������ ����
		double	m_dSttWeight[BMOPT_EXPANSIONJOINT_MAX];		// ������ Kg/m
		double	m_dEndWeight[BMOPT_EXPANSIONJOINT_MAX];		// ������ Kg/m
		CString	m_szSttName;
		CString	m_szEndName;
		double	m_dMargin;									// ����ġ��
		long	m_nMarginsu;								// ����ġ�����
		BOOL	m_bIncludeWalkLoad;							// ������ ���� ����
	}	m_structExpansionJoint;

	struct _EXPANSIONJOINTCOVER
	{
		BOOL	m_bApply;				// �������� ����
		BOOL	m_bSttPoint[7];			// ����	- ������
		BOOL	m_bEndPoint[7];			// ���� - ������
		long	m_nQty[7];				// ����
		double	m_dHeight[7];			// �Ÿ�
	}	m_structExpansionJointCover;
	
	struct _NOTCH
	{
		BOOL	m_bApply;				// Notch
		BOOL	m_bLeft;				// ���� - ������
		BOOL	m_bRight;				// ���� - ������
		long	m_nApplyPos;			// 0 : ��������		1 : ��������	2 : ��������
		CString	m_szNotchType;			// NOTCH ����
	}	m_structNotch;

	struct _GUARDFENCE
	{
		BOOL	m_bApply;				// GuardFence
		double	m_dHeight;
	}	m_structGuardFence;

	struct _SOUNDPROOF
	{
		BOOL	m_bApply;				// �����ǳ�
		double	m_dLHeight;
		double	m_dRHeight;
	}	m_structSoundProof;

	struct _PARAPET
	{
		BOOL	m_bApply;				// ����
		BOOL	m_bLeft[3];				// �������� - ������
		BOOL	m_bRight[3];			// �������� - ������
		double	m_dHeight[3];			// �������� - ������
		double	m_dGuardLength[2];		// ��ȣ���� ����
		double	m_dCurbLength;			// ������ ����
		long	m_nCurbQty;				// ���� ���밹��
	}	m_structParapet;

	struct _WINGWALL
	{
		BOOL	m_bApply;				// ������
		double	m_dLengthLeftStt;		// ���� ������ ���� ����
		double	m_dLengthLeftEnd;		// ���� ������ ���� ����
		double	m_dLengthRighStt;		// ���� ������ ���� ����
		double	m_dLengthRighEnd;		// ���� ������ ���� ����
	}	m_structWingWall;

	struct _DRAINESTABLISHRIVER
	{
		long	m_nWaterCollect;		// ������
		double	m_dDrain;				// �����
	}	m_structRiverDrainEstablish;	// ����ü� ��õ

	struct _DRAINESTABLISH_WALKWAY
	{
		long	m_nType;
		long	m_nWaterCollect;		// ������
		long	m_nDrain;				// �����
		long	m_nJoinDrain;			// ��������
		double	m_dStraightTube[2];		// ����
		long	m_nCurveTube[2];		// ���
		long	m_nCleanTube[2];		// û�ұ�
		long	m_nUpperFixed[2];		// ��ΰ�����
		long	m_nCollecting;			// ħ����
		long	m_nJoint;				// �����
		long	m_nJoinWaterCollect;	// ����������
	
	}	m_structWalkWayDrainEstablish;	// ����ü�

	struct _TIMBER
	{
		BOOL	m_bApply;				// ����
		long	m_nQuality;				// ����
		long	m_nPosDeckFinisher;		// ��ũ�Ǵϼ���ġ		0 : ������  1 : ��������   2 : ��������   3 : ��������
		double	m_dDeckFinisherWorkWidth;	// ��ũ�Ǵϼ��۾���
	}	m_structTimber;					// ���ٸ�

	struct _SHRINKAGECONCRETE
	{
		BOOL	m_bApply;
		long	m_nSttQty;
		long	m_nEndQty;
	}	m_structShrinkageConcrete;		// ��������ũ��Ʈ

	struct _REBARMANUFACTURE
	{
		BOOL	m_bApply;
		long	m_nSlabType;			// ������		0 : �ſ캹��	1 : ����	2 : ����	3 : ����	4 : ������
		long	m_nGuardFenceType;		// ��ȣ��		0 : �ſ캹��	1 : ����	2 : ����	3 : ����	4 : ������
		long	m_nExpansionJointType;	// ��������		0 : �ſ캹��	1 : ����	2 : ����	3 : ����	4 : ������
		long	m_nBindConcType;		// ������ũ��Ʈ	0 : �ſ캹��	1 : ����	2 : ����	3 : ����	4 : ������
	}	m_structRebarManufacture;		// ö�ٰ�������

	struct _DROPPINGPREVENT
	{
		BOOL	m_bApply;				// ����
		BOOL	m_bIsOnlyHorizontal;	// �������� or �����������
		double	m_dHorSpaceHeight;		// ����������
		double	m_dVerSpaceHeight;		// ������������
	}	m_structDroppingPrevent;

	struct _ELECWIREHOLE
	{
		BOOL	m_bApply;				// ����
		long	m_nEA100[2];			// ��100 : 0-Left 1-Right
		long	m_nEA125[2];			// ��125 : 0-Left 1-Right
		long	m_nEA150[2];			// ��150 : 0-Left 1-Right
	}	m_structElecWireHole;			// ������

	BOOL	m_bConcreteBuy;				// ��ũ��Ʈ ����
	BOOL	m_bConcretePlacing;			// ��ũ��Ʈ Ÿ��
	BOOL	m_bMold;					// ��Ǫ��
	BOOL	m_bSpacer;					// �����̼�
	BOOL	m_bRebarManufacture;		// ö�� ���� ����	- ������
	BOOL	m_bDrainBridgeSurface;		// ������
	BOOL	m_bWaterDraw;				// ���� ������
	BOOL	m_bShade;					// ������
	BOOL	m_bDroppingPrevent;			// ���Ϲ� ������ - ������
	BOOL	m_bSteelQuantity;			// ���� ����
	BOOL	m_bPaint;					// ���� ����
	BOOL	m_bDrainEstablish;			// ��� �ü�
	long	m_nDrainEstablishType;		// ����ü� Ÿ��
	BOOL	m_bShrinkageConcrete;		// ������ ��ũ��Ʈ	- ������

	// ��꼭 ��� �ɼ�
	BOOL	m_bChkSheet[BMOPT_CHKSHEET];				// �ܱ�����꼭 ��� 
	BOOL	m_bChk2ndSh_Model[BMOPT_CHKSH_2ND];		//	2�� �����׸�
	int		m_nSh2ndOptGir;				//		2�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)
	BOOL	m_bChk3thSh_Force[BMOPT_CHKSH_3TH];		//	3�� �����׸�
	BOOL	m_bChk4thSh_Section[BMOPT_CHKSH_4TH];		//	4�� �����׸�
	int		m_nSh4thOptGir;				//		4�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)
	BOOL	m_bChk5thSh_Joint[BMOPT_CHKSH_5TH];		//	5�� �����׸�		
	BOOL	m_bChk8thSh_Diap[BMOPT_CHKSH_8TH];		//	8�� �����׸�		
	BOOL	m_bChk9thSh_Check[BMOPT_CHKSH_9TH];		//	9�� �����׸�		
	BOOL	m_bChkSh9thPrecise;			//		9�� �������� ���н�
	BOOL	m_bChkSh9thSimple;			//		9�� �������� �����
};

#endif // !defined(AFX_BMOPTION_H__4C22AC7B_964D_4445_8E36_7B65E64369D0__INCLUDED_)
