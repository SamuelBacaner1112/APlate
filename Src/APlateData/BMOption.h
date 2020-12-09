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

#define BRIDGE_STT_POINT			0			// 교량 시점
#define BRIDGE_END_POINT			1			// 교량 종점
#define GENERAL_GUARDFENCE			0			// 일반 방호벽
#define ABSOLUTE_GUARDFENCE			1			// 절대 방호벽
#define GENERAL_SOUNDPROOF			2			// 일반 방음벽
#define ABSOLUTE_SOUNDPROOF			3			// 절대 방음벽
#define CENTER_GUARDFENCE			4			// 중분대
#define PARAPET						5			// 난  간
#define CURB						6			// 연  석
#define DRIVEWAY					0			// 차도용
#define FOOTWAY						1			// 보도용
#define DRIVE_FOOT_BOUNDARY			2			// 보차도 경계용
#define RIVER						0			// 하천용
#define WALKWAY						1			// 육교용
#define RIVERWALKWAY				2			// 하천육교용
#define TIMBER					    0			// 목재
#define STEELTUBE					1			// 강관

#define BMOPT_CONCRETE_BYE			1001		// 콘크리트 구입
#define BMOPT_CONCRETE_PLACING		1002		// 콘크리트 타설
#define BMOPT_MOLD					1003		// 거푸집
#define BMOPT_SPACER				1004		// 스페이셔
#define BMOPT_REBAR_MANUFACTURE		1005		// 철근 가공 조립
#define BMOPT_PAVE					1006		// 포장
#define BMOPT_TACKCOATING			1007		// 택코팅
#define BMOPT_DRAINBRIDGESURFACE	1008		// 교면방수
#define BMOPT_BRIDGENAME			1009		// 교명주
#define BMOPT_BRIDGENAME_PLATE		1010		// 교명판
#define BMOPT_EXPLAIN_PLATE			1011		// 설명판
#define BMOPT_ESTABLISH_TBM			1012		// TBM 설치
#define BMOPT_STYROFOAM				1013		// 스티로폼
#define BMOPT_EXPANSIONJOINT		1014		// 신축이음
#define BMOPT_EXPANSIONJOINT_COVER	1015		// 신축이음 덮개
#define BMOPT_SHRINKAGE_CONCRETE	1016		// 무수축 콘크리트
#define BMOPT_WATER_DRAW			1017		// 교면 물빼기
#define BMOPT_NOTCH					1018		// NOTCH
#define BMOPT_GUARD_FENCE			1019		// 가드펜스
#define BMOPT_SOUND_PROOF			1020		// 방음판넬
#define BMOPT_PARAPET				1021		// 난간
#define BMOPT_SHADE					1022		// 차광망
#define BMOPT_DROPPING_PREVENT		1023		// 낙하물 방지공
#define BMOPT_STEEL_QUANTITY		1024		// 강재 수량
#define BMOPT_PAINT					1025		// 강교 도장
#define BMOPT_DRAIN_ESTABLISH		1026		// 배수 시설
#define BMOPT_WING_WALL				1027		// 날개벽
#define BMOPT_TIMBER				1028		// 동바리
#define BMOPT_ELECWIREHOLE			1029		// 전선관

#define BMOPT_EXPANSIONJOINT_MAX	2			// 신축이음

#define BMOPT_CHKSHEET				9			// 전체 구조계산서 
#define BMOPT_CHKSH_2ND				4			//	2장 하위항목
#define BMOPT_CHKSH_3TH				3			//	3장 하위항목
#define BMOPT_CHKSH_4TH				3			//	4장 하위항목
#define BMOPT_CHKSH_5TH				2			//	5장 하위항목
#define BMOPT_CHKSH_8TH				2			//	8장 하위항목
#define BMOPT_CHKSH_9TH				4			//	9장 하위항목


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
	}	m_structPave;					// 포장

	struct _TACKCOATING
	{
		BOOL	m_bApply;
		double	m_dTackCoating;
	}	m_structTackCoating;			// 택 코팅

	struct _BRIDGENAME
	{
		BOOL	m_bApply;		
		BOOL	m_bOneSide;				// 편측.양측	- 사용안함
		BOOL	m_bSttPoint;			// 시점			- 사용안함
		BOOL	m_bEndPoint;			// 종점			- 사용안함
		long	m_nQty;					// 교명주 갯수
	}	m_structBridgeName;				// 교명주

	struct _BRIDGENAMEPLATE
	{
		BOOL	m_bApply;
		BOOL	m_bSttPoint;			// 시점	 - 사용안함
		BOOL	m_bEndPoint;			// 종점	 - 사용안함
		long	m_nQty;					// 교명판 갯수
	}	m_structBridgeNamePlate;		// 교명판

	struct	_EXPLAINPLATE
	{
		BOOL	m_bApply;				// 설명판
		BOOL	m_bSttPoint;			// 시점	- 사용안함
		BOOL	m_bEndPoint;			// 종점	- 사용안함
		long	m_nQty;					// 설명판 갯수
	}	m_structExplainPlate;			// 설명판

	struct _ESTABLISHTBM
	{
		BOOL	m_bApply;
		BOOL	m_bSttPoint;			// 시점	- 사용안함
		BOOL	m_bEndPoint;			// 종점	- 사용안함
		long	m_nQty;					// TBM설치 갯수
	}	m_structEstablishTBM;			// TBM 설치

	struct _STYROFOAM
	{
		BOOL	m_bApply;				// 스티로폼
		BOOL	m_bLeft;				// TRUE : 좌측적용     FALSE : 우측적용
		double	m_dThick;				// 두께 10mm or 20mm
	}	m_structStyrofoam;

	struct _EXPANSIONJOINT
	{
		BOOL	m_bApply;				// 신축이음
		BOOL	m_bUnitLength;			// TRUE : 단위 m당 FALSE : 지점별
		long	m_nSttDiaIdx[BMOPT_EXPANSIONJOINT_MAX];		// 시점부 직경
		long	m_nEndDiaIdx[BMOPT_EXPANSIONJOINT_MAX];		// 종점부 직경
		double	m_dSttWeight[BMOPT_EXPANSIONJOINT_MAX];		// 시점부 Kg/m
		double	m_dEndWeight[BMOPT_EXPANSIONJOINT_MAX];		// 종점부 Kg/m
		CString	m_szSttName;
		CString	m_szEndName;
		double	m_dMargin;									// 여유치수
		long	m_nMarginsu;								// 여유치적용수
		BOOL	m_bIncludeWalkLoad;							// 보도부 포함 여부
	}	m_structExpansionJoint;

	struct _EXPANSIONJOINTCOVER
	{
		BOOL	m_bApply;				// 신축이음 덮개
		BOOL	m_bSttPoint[7];			// 시점	- 사용안함
		BOOL	m_bEndPoint[7];			// 종점 - 사용안함
		long	m_nQty[7];				// 갯수
		double	m_dHeight[7];			// 거리
	}	m_structExpansionJointCover;
	
	struct _NOTCH
	{
		BOOL	m_bApply;				// Notch
		BOOL	m_bLeft;				// 좌측 - 사용안함
		BOOL	m_bRight;				// 우측 - 사용안함
		long	m_nApplyPos;			// 0 : 좌측적용		1 : 우측적용	2 : 양측적용
		CString	m_szNotchType;			// NOTCH 종류
	}	m_structNotch;

	struct _GUARDFENCE
	{
		BOOL	m_bApply;				// GuardFence
		double	m_dHeight;
	}	m_structGuardFence;

	struct _SOUNDPROOF
	{
		BOOL	m_bApply;				// 방음판넬
		double	m_dLHeight;
		double	m_dRHeight;
	}	m_structSoundProof;

	struct _PARAPET
	{
		BOOL	m_bApply;				// 난간
		BOOL	m_bLeft[3];				// 좌측적용 - 사용안함
		BOOL	m_bRight[3];			// 우측적용 - 사용안함
		double	m_dHeight[3];			// 난간높이 - 사용안함
		double	m_dGuardLength[2];		// 방호벽용 길이
		double	m_dCurbLength;			// 연석용 길이
		long	m_nCurbQty;				// 연석 적용갯수
	}	m_structParapet;

	struct _WINGWALL
	{
		BOOL	m_bApply;				// 날개벽
		double	m_dLengthLeftStt;		// 좌측 날개벽 길이 시점
		double	m_dLengthLeftEnd;		// 좌측 날개벽 길이 종점
		double	m_dLengthRighStt;		// 우측 날개벽 길이 시점
		double	m_dLengthRighEnd;		// 우측 날개벽 길이 종점
	}	m_structWingWall;

	struct _DRAINESTABLISHRIVER
	{
		long	m_nWaterCollect;		// 집수구
		double	m_dDrain;				// 배수구
	}	m_structRiverDrainEstablish;	// 배수시설 하천

	struct _DRAINESTABLISH_WALKWAY
	{
		long	m_nType;
		long	m_nWaterCollect;		// 집수구
		long	m_nDrain;				// 배수구
		long	m_nJoinDrain;			// 연결배수구
		double	m_dStraightTube[2];		// 직관
		long	m_nCurveTube[2];		// 곡관
		long	m_nCleanTube[2];		// 청소구
		long	m_nUpperFixed[2];		// 상부고정대
		long	m_nCollecting;			// 침전조
		long	m_nJoint;				// 연결부
		long	m_nJoinWaterCollect;	// 연결집수거
	
	}	m_structWalkWayDrainEstablish;	// 배수시설

	struct _TIMBER
	{
		BOOL	m_bApply;				// 적용
		long	m_nQuality;				// 재질
		long	m_nPosDeckFinisher;		// 데크피니셔위치		0 : 미적용  1 : 좌측적용   2 : 우측적용   3 : 양측적용
		double	m_dDeckFinisherWorkWidth;	// 데크피니셔작업폭
	}	m_structTimber;					// 동바리

	struct _SHRINKAGECONCRETE
	{
		BOOL	m_bApply;
		long	m_nSttQty;
		long	m_nEndQty;
	}	m_structShrinkageConcrete;		// 무수축콘크리트

	struct _REBARMANUFACTURE
	{
		BOOL	m_bApply;
		long	m_nSlabType;			// 슬래브		0 : 매우복잡	1 : 복잡	2 : 보통	3 : 간단	4 : 미적용
		long	m_nGuardFenceType;		// 방호벽		0 : 매우복잡	1 : 복잡	2 : 보통	3 : 간단	4 : 미적용
		long	m_nExpansionJointType;	// 신축이음		0 : 매우복잡	1 : 복잡	2 : 보통	3 : 간단	4 : 미적용
		long	m_nBindConcType;		// 구속콘크리트	0 : 매우복잡	1 : 복잡	2 : 보통	3 : 간단	4 : 미적용
	}	m_structRebarManufacture;		// 철근가공조립

	struct _DROPPINGPREVENT
	{
		BOOL	m_bApply;				// 적용
		BOOL	m_bIsOnlyHorizontal;	// 수평적용 or 수평수직적용
		double	m_dHorSpaceHeight;		// 수평여유높이
		double	m_dVerSpaceHeight;		// 수직여유높이
	}	m_structDroppingPrevent;

	struct _ELECWIREHOLE
	{
		BOOL	m_bApply;				// 적용
		long	m_nEA100[2];			// φ100 : 0-Left 1-Right
		long	m_nEA125[2];			// φ125 : 0-Left 1-Right
		long	m_nEA150[2];			// φ150 : 0-Left 1-Right
	}	m_structElecWireHole;			// 전선관

	BOOL	m_bConcreteBuy;				// 콘크리트 구입
	BOOL	m_bConcretePlacing;			// 콘크리트 타설
	BOOL	m_bMold;					// 거푸집
	BOOL	m_bSpacer;					// 스페이셔
	BOOL	m_bRebarManufacture;		// 철근 가공 조립	- 사용안함
	BOOL	m_bDrainBridgeSurface;		// 교면방수
	BOOL	m_bWaterDraw;				// 교면 물빼기
	BOOL	m_bShade;					// 차광망
	BOOL	m_bDroppingPrevent;			// 낙하물 방지공 - 사용안함
	BOOL	m_bSteelQuantity;			// 강재 수량
	BOOL	m_bPaint;					// 강교 도장
	BOOL	m_bDrainEstablish;			// 배수 시설
	long	m_nDrainEstablishType;		// 배수시설 타입
	BOOL	m_bShrinkageConcrete;		// 무수축 콘크리트	- 사용안함

	// 계산서 출력 옵션
	BOOL	m_bChkSheet[BMOPT_CHKSHEET];				// ●구조계산서 출력 
	BOOL	m_bChk2ndSh_Model[BMOPT_CHKSH_2ND];		//	2장 하위항목
	int		m_nSh2ndOptGir;				//		2장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)
	BOOL	m_bChk3thSh_Force[BMOPT_CHKSH_3TH];		//	3장 하위항목
	BOOL	m_bChk4thSh_Section[BMOPT_CHKSH_4TH];		//	4장 하위항목
	int		m_nSh4thOptGir;				//		4장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)
	BOOL	m_bChk5thSh_Joint[BMOPT_CHKSH_5TH];		//	5장 하위항목		
	BOOL	m_bChk8thSh_Diap[BMOPT_CHKSH_8TH];		//	8장 하위항목		
	BOOL	m_bChk9thSh_Check[BMOPT_CHKSH_9TH];		//	9장 하위항목		
	BOOL	m_bChkSh9thPrecise;			//		9장 신축이음 정밀식
	BOOL	m_bChkSh9thSimple;			//		9장 신축이음 간편식
};

#endif // !defined(AFX_BMOPTION_H__4C22AC7B_964D_4445_8E36_7B65E64369D0__INCLUDED_)
