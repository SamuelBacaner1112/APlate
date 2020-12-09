// BMOption.cpp: implementation of the CBMOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "BMOption.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMOption::CBMOption()
{
	SetDataDefault();
}

CBMOption::~CBMOption()
{
	
}





void CBMOption::Serialize(CArchive &ar)
{
//	long nFlag	= 1;
	long nFlag	= 2;	// 080205 철근가공조립 구속콘크리트 추가 - lina
	long n		= 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_structPave.m_bApply;
		ar << m_structPave.m_dPaveThick;
		ar << m_structTackCoating.m_bApply;
		ar << m_structTackCoating.m_dTackCoating;
		ar << m_structBridgeName.m_bApply;
		ar << m_structBridgeName.m_bOneSide;
		ar << m_structBridgeName.m_bSttPoint;
		ar << m_structBridgeName.m_bEndPoint;
		ar << m_structBridgeNamePlate.m_bApply;
		ar << m_structBridgeNamePlate.m_bSttPoint;
		ar << m_structBridgeNamePlate.m_bEndPoint;
		ar << m_structExplainPlate.m_bApply;
		ar << m_structExplainPlate.m_bSttPoint;
		ar << m_structExplainPlate.m_bEndPoint;
		ar << m_structEstablishTBM.m_bApply;
		ar << m_structEstablishTBM.m_bSttPoint;
		ar << m_structEstablishTBM.m_bEndPoint;
		ar << m_structStyrofoam.m_bApply;
		ar << m_structStyrofoam.m_dThick;
		ar << m_structExpansionJoint.m_bApply;
		ar << m_structExpansionJoint.m_bUnitLength;
		for(n=0; n<BMOPT_EXPANSIONJOINT_MAX; n++)
		{
			ar << m_structExpansionJoint.m_nSttDiaIdx[n];
			ar << m_structExpansionJoint.m_nEndDiaIdx[n];
			ar << m_structExpansionJoint.m_dSttWeight[n];
			ar << m_structExpansionJoint.m_dEndWeight[n];
		}
		ar << m_structExpansionJoint.m_szSttName;
		ar << m_structExpansionJoint.m_szEndName;

		ar << m_structExpansionJointCover.m_bApply;
		for(n=0; n<7; n++)
		{
			ar << m_structExpansionJointCover.m_bSttPoint[n];
			ar << m_structExpansionJointCover.m_bEndPoint[n];
			ar << m_structExpansionJointCover.m_dHeight[n];
		}
		ar << m_structNotch.m_bApply;
		ar << m_structNotch.m_bLeft;
		ar << m_structNotch.m_bRight;
		ar << m_structGuardFence.m_bApply;
		ar << m_structGuardFence.m_dHeight;
		ar << m_structSoundProof.m_bApply;
		ar << m_structSoundProof.m_dLHeight;
		ar << m_structParapet.m_bApply;
		for(n=0; n<3; n++)
		{
			ar << m_structParapet.m_bLeft[n];
			ar << m_structParapet.m_bRight[n];
			ar << m_structParapet.m_dHeight[n];
		}
		ar << m_bConcreteBuy;
		ar << m_bConcretePlacing;
		ar << m_bMold;
		ar << m_bSpacer;
		ar << m_bRebarManufacture;
		ar << m_bDrainBridgeSurface;
		ar << m_bWaterDraw;
		ar << m_bShade;
		ar << m_bDroppingPrevent;
		ar << m_bSteelQuantity;
		ar << m_bPaint;
		ar << m_bDrainEstablish;
		ar << m_bShrinkageConcrete;	// 사용안함

		for(n=0; n<BMOPT_CHKSHEET; n++)
			ar << m_bChkSheet[n];						// ●구조계산서 출력 	
		for(n=0; n<BMOPT_CHKSH_2ND; n++)
			ar << m_bChk2ndSh_Model[n];					//	2장 하위항목
		for(n=0; n<BMOPT_CHKSH_3TH; n++)
			ar << m_bChk3thSh_Force[n];					//	3장 하위항목
		for(n=0; n<BMOPT_CHKSH_4TH; n++)
			ar << m_bChk4thSh_Section[n];				//	4장 하위항목
		for(n=0; n<BMOPT_CHKSH_5TH; n++)
			ar << m_bChk5thSh_Joint[n];					//	5장 하위항목		
		for(n=0; n<BMOPT_CHKSH_8TH; n++)
			ar << m_bChk8thSh_Diap[n];					//	8장 하위항목		
		for(n=0; n<BMOPT_CHKSH_9TH; n++)
			ar << m_bChk9thSh_Check[n];					//	9장 하위항목		
		
		ar << m_nSh2ndOptGir;							//		2장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)	
		ar << m_nSh4thOptGir;							//		4장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)
		ar << m_bChkSh9thPrecise;						//		9장 신축이음 정밀식
		ar << m_bChkSh9thSimple;						//		9장 신축이음 간편식
		
		ar << m_structWingWall.m_bApply;
		ar << m_structWingWall.m_dLengthLeftStt;
		ar << m_structWingWall.m_dLengthLeftEnd;
		ar << m_structWingWall.m_dLengthRighStt;
		ar << m_structWingWall.m_dLengthRighEnd;

		ar << m_structStyrofoam.m_bLeft;

		ar << m_nDrainEstablishType;
		ar << m_structRiverDrainEstablish.m_nWaterCollect;				// 집수구
		ar << m_structRiverDrainEstablish.m_dDrain;						// 배수구

		ar << m_structWalkWayDrainEstablish.m_nType;					// 타입
		ar << m_structWalkWayDrainEstablish.m_nWaterCollect;			// 집수구
		ar << m_structWalkWayDrainEstablish.m_nDrain;					// 배수구
		ar << m_structWalkWayDrainEstablish.m_nJoinDrain;				// 연결배수구
		for(n=0; n<2; n++)
		{
			ar << m_structWalkWayDrainEstablish.m_dStraightTube[n];		// 직관
			ar << m_structWalkWayDrainEstablish.m_nCurveTube[n];		// 곡관
			ar << m_structWalkWayDrainEstablish.m_nCleanTube[n];		// 청소구
			ar << m_structWalkWayDrainEstablish.m_nUpperFixed[n];		// 상부고정대
		}
		ar << m_structWalkWayDrainEstablish.m_nCollecting;				// 침전조
		ar << m_structWalkWayDrainEstablish.m_nJoint;					// 연결부
		ar << m_structWalkWayDrainEstablish.m_nJoinWaterCollect;		// 연결집수거

		ar << m_structTimber.m_bApply;
		ar << m_structTimber.m_nQuality;
		ar << m_structTimber.m_nPosDeckFinisher;
		ar << m_structTimber.m_dDeckFinisherWorkWidth;

		ar << m_structExpansionJoint.m_dMargin;
		ar << m_structExpansionJoint.m_nMarginsu;

		ar << m_structShrinkageConcrete.m_bApply;
		ar << m_structShrinkageConcrete.m_nSttQty;
		ar << m_structShrinkageConcrete.m_nEndQty;

		ar << m_structRebarManufacture.m_bApply;
		ar << m_structRebarManufacture.m_nSlabType;
		ar << m_structRebarManufacture.m_nGuardFenceType;
		ar << m_structRebarManufacture.m_nExpansionJointType;
		ar << m_structRebarManufacture.m_nBindConcType;

		ar << m_structBridgeName.m_nQty;
		ar << m_structBridgeNamePlate.m_nQty;
		ar << m_structExplainPlate.m_nQty;
		ar << m_structEstablishTBM.m_nQty;

		ar << m_structDroppingPrevent.m_bApply;
		ar << m_structDroppingPrevent.m_bIsOnlyHorizontal;
		ar << m_structDroppingPrevent.m_dHorSpaceHeight;
		ar << m_structDroppingPrevent.m_dVerSpaceHeight;

		ar << m_structNotch.m_szNotchType;

		for(n=0; n<7; n++)
			ar << m_structExpansionJointCover.m_nQty[n];

		ar << m_structParapet.m_dGuardLength[0];		// 방호벽용 좌측길이
		ar << m_structParapet.m_dGuardLength[1];		// 방호벽용 우측길이
		ar << m_structParapet.m_dCurbLength;			// 연석용 길이
		ar << m_structParapet.m_nCurbQty;				// 연석 적용갯수

		ar << m_structNotch.m_nApplyPos;				// Notch

		ar << m_structSoundProof.m_dRHeight;			// 방음판넬 우측
		ar << m_structElecWireHole.m_bApply;
		for(n=0; n<2; n++)
		{
			ar << m_structElecWireHole.m_nEA100[n];
			ar << m_structElecWireHole.m_nEA125[n];
			ar << m_structElecWireHole.m_nEA150[n];
		}

		ar << m_structExpansionJoint.m_bIncludeWalkLoad;

	}
	else
	{
		ar >> nFlag;
		ar >> m_structPave.m_bApply;
		ar >> m_structPave.m_dPaveThick;
		ar >> m_structTackCoating.m_bApply;
		ar >> m_structTackCoating.m_dTackCoating;
		ar >> m_structBridgeName.m_bApply;
		ar >> m_structBridgeName.m_bOneSide;
		ar >> m_structBridgeName.m_bSttPoint;
		ar >> m_structBridgeName.m_bEndPoint;
		ar >> m_structBridgeNamePlate.m_bApply;
		ar >> m_structBridgeNamePlate.m_bSttPoint;
		ar >> m_structBridgeNamePlate.m_bEndPoint;
		ar >> m_structExplainPlate.m_bApply;
		ar >> m_structExplainPlate.m_bSttPoint;
		ar >> m_structExplainPlate.m_bEndPoint;
		ar >> m_structEstablishTBM.m_bApply;
		ar >> m_structEstablishTBM.m_bSttPoint;
		ar >> m_structEstablishTBM.m_bEndPoint;
		ar >> m_structStyrofoam.m_bApply;
		ar >> m_structStyrofoam.m_dThick;
		ar >> m_structExpansionJoint.m_bApply;
		ar >> m_structExpansionJoint.m_bUnitLength;
		for(n=0; n<BMOPT_EXPANSIONJOINT_MAX; n++)
		{
			ar >> m_structExpansionJoint.m_nSttDiaIdx[n];
			ar >> m_structExpansionJoint.m_nEndDiaIdx[n];
			ar >> m_structExpansionJoint.m_dSttWeight[n];
			ar >> m_structExpansionJoint.m_dEndWeight[n];
		}
		ar >> m_structExpansionJoint.m_szSttName;
		ar >> m_structExpansionJoint.m_szEndName;

		ar >> m_structExpansionJointCover.m_bApply;
		for(n=0; n<7; n++)
		{
			ar >> m_structExpansionJointCover.m_bSttPoint[n];
			ar >> m_structExpansionJointCover.m_bEndPoint[n];
			ar >> m_structExpansionJointCover.m_dHeight[n];
		}
		ar >> m_structNotch.m_bApply;
		ar >> m_structNotch.m_bLeft;
		ar >> m_structNotch.m_bRight;
		ar >> m_structGuardFence.m_bApply;
		ar >> m_structGuardFence.m_dHeight;
		ar >> m_structSoundProof.m_bApply;
		ar >> m_structSoundProof.m_dLHeight;
		ar >> m_structParapet.m_bApply;
		for(n=0; n<3; n++)
		{
			ar >> m_structParapet.m_bLeft[n];
			ar >> m_structParapet.m_bRight[n];
			ar >> m_structParapet.m_dHeight[n];
		}
		ar >> m_bConcreteBuy;
		ar >> m_bConcretePlacing;
		ar >> m_bMold;
		ar >> m_bSpacer;
		ar >> m_bRebarManufacture;
		ar >> m_bDrainBridgeSurface;
		ar >> m_bWaterDraw;
		ar >> m_bShade;
		ar >> m_bDroppingPrevent;
		ar >> m_bSteelQuantity;
		ar >> m_bPaint;
		ar >> m_bDrainEstablish;
		ar >> m_bShrinkageConcrete;

		for(n=0; n<BMOPT_CHKSHEET; n++)
			ar >> m_bChkSheet[n];						// ●구조계산서 출력 	
		for(n=0; n<BMOPT_CHKSH_2ND; n++)
			ar >> m_bChk2ndSh_Model[n];					//	2장 하위항목
		for(n=0; n<BMOPT_CHKSH_3TH; n++)
			ar >> m_bChk3thSh_Force[n];					//	3장 하위항목
		for(n=0; n<BMOPT_CHKSH_4TH; n++)
			ar >> m_bChk4thSh_Section[n];				//	4장 하위항목
		for(n=0; n<BMOPT_CHKSH_5TH; n++)
			ar >> m_bChk5thSh_Joint[n];					//	5장 하위항목		
		for(n=0; n<BMOPT_CHKSH_8TH; n++)
			ar >> m_bChk8thSh_Diap[n];					//	8장 하위항목		
		for(n=0; n<BMOPT_CHKSH_9TH; n++)
			ar >> m_bChk9thSh_Check[n];					//	9장 하위항목		
		
		ar >> m_nSh2ndOptGir;							//		2장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)	
		ar >> m_nSh4thOptGir;							//		4장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)
		ar >> m_bChkSh9thPrecise;						//		9장 신축이음 정밀식
		ar >> m_bChkSh9thSimple;						//		9장 신축이음 간편식

		ar >> m_structWingWall.m_bApply;
		ar >> m_structWingWall.m_dLengthLeftStt;
		ar >> m_structWingWall.m_dLengthLeftEnd;
		ar >> m_structWingWall.m_dLengthRighStt;
		ar >> m_structWingWall.m_dLengthRighEnd;

		ar >> m_structStyrofoam.m_bLeft;

		ar >> m_nDrainEstablishType;
		ar >> m_structRiverDrainEstablish.m_nWaterCollect;				// 집수구
		ar >> m_structRiverDrainEstablish.m_dDrain;						// 배수구

		ar >> m_structWalkWayDrainEstablish.m_nType;					// 타입
		ar >> m_structWalkWayDrainEstablish.m_nWaterCollect;			// 집수구
		ar >> m_structWalkWayDrainEstablish.m_nDrain;					// 배수구
		ar >> m_structWalkWayDrainEstablish.m_nJoinDrain;				// 연결배수구
		for(n=0; n<2; n++)
		{
			ar >> m_structWalkWayDrainEstablish.m_dStraightTube[n];		// 직관
			ar >> m_structWalkWayDrainEstablish.m_nCurveTube[n];		// 곡관
			ar >> m_structWalkWayDrainEstablish.m_nCleanTube[n];		// 청소구
			ar >> m_structWalkWayDrainEstablish.m_nUpperFixed[n];		// 상부고정대
		}
		ar >> m_structWalkWayDrainEstablish.m_nCollecting;				// 침전조
		ar >> m_structWalkWayDrainEstablish.m_nJoint;					// 연결부
		ar >> m_structWalkWayDrainEstablish.m_nJoinWaterCollect;		// 연결집수거

		ar >> m_structTimber.m_bApply;
		ar >> m_structTimber.m_nQuality;
		ar >> m_structTimber.m_nPosDeckFinisher;
		ar >> m_structTimber.m_dDeckFinisherWorkWidth;

		ar >> m_structExpansionJoint.m_dMargin;
		ar >> m_structExpansionJoint.m_nMarginsu;

		ar >> m_structShrinkageConcrete.m_bApply;
		ar >> m_structShrinkageConcrete.m_nSttQty;
		ar >> m_structShrinkageConcrete.m_nEndQty;

		ar >> m_structRebarManufacture.m_bApply;
		ar >> m_structRebarManufacture.m_nSlabType;
		ar >> m_structRebarManufacture.m_nGuardFenceType;
		ar >> m_structRebarManufacture.m_nExpansionJointType;
		if(nFlag > 1)
			ar >> m_structRebarManufacture.m_nBindConcType;

		ar >> m_structBridgeName.m_nQty;
		ar >> m_structBridgeNamePlate.m_nQty;
		ar >> m_structExplainPlate.m_nQty;
		ar >> m_structEstablishTBM.m_nQty;

		ar >> m_structDroppingPrevent.m_bApply;
		ar >> m_structDroppingPrevent.m_bIsOnlyHorizontal;
		ar >> m_structDroppingPrevent.m_dHorSpaceHeight;
		ar >> m_structDroppingPrevent.m_dVerSpaceHeight;

		ar >> m_structNotch.m_szNotchType;
		for(n=0; n<7; n++)
			ar >> m_structExpansionJointCover.m_nQty[n];

		ar >> m_structParapet.m_dGuardLength[0];		// 방호벽용 좌측길이
		ar >> m_structParapet.m_dGuardLength[1];		// 방호벽용 우측길이
		ar >> m_structParapet.m_dCurbLength;			// 연석용 높이
		ar >> m_structParapet.m_nCurbQty;				// 연석 적용갯수
		ar >> m_structNotch.m_nApplyPos;				// Notch
		ar >> m_structSoundProof.m_dRHeight;
		ar >> m_structElecWireHole.m_bApply;
		for(n=0; n<2; n++)
		{
			ar >> m_structElecWireHole.m_nEA100[n];
			ar >> m_structElecWireHole.m_nEA125[n];
			ar >> m_structElecWireHole.m_nEA150[n];
		}

		if(nFlag > 0)
			ar >> m_structExpansionJoint.m_bIncludeWalkLoad;
	}
}





void CBMOption::SetDataDefault()
{
	m_structPave.m_bApply								= TRUE;				// 포장
	m_structPave.m_dPaveThick							= 80;				// 포장두께

	m_structTackCoating.m_bApply						= TRUE;				// 택코팅
	m_structTackCoating.m_dTackCoating					= 1.5;				// 1.5배

	m_structBridgeName.m_bApply							= FALSE;			// 교명주
	m_structBridgeName.m_bOneSide						= FALSE;			// 편측.양측
	m_structBridgeName.m_bSttPoint						= FALSE;			// 시점
	m_structBridgeName.m_bEndPoint						= FALSE;			// 종점

	m_structBridgeNamePlate.m_bApply					= FALSE;			// 교명판
	m_structBridgeNamePlate.m_bSttPoint					= FALSE;			// 시점
	m_structBridgeNamePlate.m_bEndPoint					= FALSE;			// 종점

	m_structExplainPlate.m_bApply						= FALSE;			// 설명판
	m_structExplainPlate.m_bSttPoint					= FALSE;			// 시점
	m_structExplainPlate.m_bEndPoint					= FALSE;			// 종점

	m_structEstablishTBM.m_bApply						= FALSE;			// TBM 설치
	m_structEstablishTBM.m_bSttPoint					= FALSE;			// 시점
	m_structEstablishTBM.m_bEndPoint					= FALSE;			// 종점

	m_structStyrofoam.m_bApply							= FALSE;			// 스티로폼
	m_structStyrofoam.m_dThick							= 20;				// 두께 10mm or 20mm
	m_structStyrofoam.m_bLeft							= TRUE;

	m_structExpansionJoint.m_bApply						= TRUE;				// 신축이음
	m_structExpansionJoint.m_bUnitLength				= FALSE;
	long n = 0;
	for(n= 0; n<BMOPT_EXPANSIONJOINT_MAX; n++)
	{
		m_structExpansionJoint.m_nSttDiaIdx[n]			= 0;
		m_structExpansionJoint.m_nEndDiaIdx[n]			= 0;
		m_structExpansionJoint.m_dSttWeight[n]			= 120.0;
		m_structExpansionJoint.m_dEndWeight[n]			= 120.0;
	}
	m_structExpansionJoint.m_nSttDiaIdx[0]				= 1;
	m_structExpansionJoint.m_nEndDiaIdx[0]				= 1;
	m_structExpansionJoint.m_nSttDiaIdx[1]				= 1;
	m_structExpansionJoint.m_nEndDiaIdx[1]				= 1;
	m_structExpansionJoint.m_szSttName					= _T("UCF-50 S");
	m_structExpansionJoint.m_szEndName					= _T("UCF-80 S");
	m_structExpansionJoint.m_dMargin					= 70.0;				// 여유치수
	m_structExpansionJoint.m_nMarginsu					= 1;				// 여유치수갯수
	m_structExpansionJoint.m_bIncludeWalkLoad			= FALSE;

	m_structExpansionJointCover.m_bApply				= TRUE;				// 신축이음 덮개
	for(n= 0; n<7; n++)
	{
		m_structExpansionJointCover.m_bSttPoint[n]		= FALSE;			// 시점 - 사용안함
		m_structExpansionJointCover.m_bEndPoint[n]		= FALSE;			// 종점 - 사용안함
		m_structExpansionJointCover.m_dHeight[n]		= 2000.0;			// 거리
		m_structExpansionJointCover.m_nQty[n]			= 1;				// 갯수
	}

	m_structNotch.m_bApply								= TRUE;				// Notch
	m_structNotch.m_bLeft								= TRUE;				// 좌측
	m_structNotch.m_bRight								= TRUE;				// 우측
	m_structNotch.m_nApplyPos							= 0;				// 좌측적용

	m_structGuardFence.m_bApply							= FALSE;			// GuardFence
	m_structGuardFence.m_dHeight						= 2000.0;

	m_structSoundProof.m_bApply							= FALSE;			// 방음판넬
	m_structSoundProof.m_dLHeight						= 2000.0;
	m_structSoundProof.m_dRHeight						= 2000.0;

	m_structParapet.m_bApply							= TRUE;				// 난간
	for(n=0; n<3; n++)
	{
		m_structParapet.m_bLeft[n]						= TRUE;
		m_structParapet.m_bRight[n]						= TRUE;
		m_structParapet.m_dHeight[n]					= 2000.0;
	}
	m_structParapet.m_dGuardLength[0]					= 0;
	m_structParapet.m_dGuardLength[1]					= 0;
	m_structParapet.m_dCurbLength						= 0;			// 연석용 높이
	m_structParapet.m_nCurbQty							= 1;			// 연석 적용갯수

	m_bConcreteBuy										= TRUE;			// 콘크리트 구입
	m_bConcretePlacing									= TRUE;			// 콘크리트 타설
	m_bMold												= TRUE;			// 거푸집
	m_bSpacer											= TRUE;			// 스페이셔
	m_bRebarManufacture									= TRUE;			// 철근 가공 조립	
	m_bDrainBridgeSurface								= TRUE;			// 교면방수
	m_bWaterDraw										= TRUE;			// 교면 물빼기
	m_bShade											= FALSE;		// 차광망
	m_bDroppingPrevent									= TRUE;			// 낙하물 방지공
	m_bSteelQuantity									= TRUE;			// 강재 수량
	m_bPaint											= TRUE;			// 강교 도장
	m_bDrainEstablish									= TRUE;			// 배수 시설
	m_bShrinkageConcrete								= TRUE;			// 무수축 콘크리트	- 사용안함

	m_structWingWall.m_bApply							= TRUE;			// 날개벽 적용
	m_structWingWall.m_dLengthLeftStt					= 6000;			// 날개벽 좌측 시점(mm)
	m_structWingWall.m_dLengthLeftEnd					= 6000;			// 날개벽 좌측 종점(mm)
	m_structWingWall.m_dLengthRighStt					= 6000;			// 날개벽 우측 시점(mm)
	m_structWingWall.m_dLengthRighEnd					= 6000;			// 날개벽 우측 종점(mm)

	m_nDrainEstablishType								= RIVER;
	m_structRiverDrainEstablish.m_nWaterCollect			= 0;			// 집수구
	m_structRiverDrainEstablish.m_dDrain				= 0;			// 배수구

	m_structWalkWayDrainEstablish.m_nType				= 0;
	m_structWalkWayDrainEstablish.m_nWaterCollect		= 0;			// 집수구
	m_structWalkWayDrainEstablish.m_nDrain				= 0;			// 배수구
	m_structWalkWayDrainEstablish.m_nJoinDrain			= 0;			// 연결배수구
	for(n=0; n<2; n++)
	{
		m_structWalkWayDrainEstablish.m_dStraightTube[n]= 0;			// 직관
		m_structWalkWayDrainEstablish.m_nCurveTube[n]	= 0;			// 곡관
		m_structWalkWayDrainEstablish.m_nCleanTube[n]	= 0;			// 청소구
		m_structWalkWayDrainEstablish.m_nUpperFixed[n]	= 0;			// 상부고정대
	}
	m_structWalkWayDrainEstablish.m_nCollecting			= 0;			// 침전조
	m_structWalkWayDrainEstablish.m_nJoint				= 0;			// 연결부
	m_structWalkWayDrainEstablish.m_nJoinWaterCollect	= 0;			// 연결집수거

	m_structTimber.m_bApply								= TRUE;			// 동바리
	m_structTimber.m_nQuality							= 0;			// 동바리 재질 - 목재
	m_structTimber.m_nPosDeckFinisher					= 2;			// 데크피니셔 위치
	m_structTimber.m_dDeckFinisherWorkWidth				= 585.0;		// 데크피니셔 작업폭

	m_structShrinkageConcrete.m_bApply					= TRUE;			// 무수축콘크리트
	m_structShrinkageConcrete.m_nSttQty					= 2;			// 시점부개수
	m_structShrinkageConcrete.m_nEndQty					= 2;			// 종점부개수

	m_structRebarManufacture.m_bApply					= TRUE;			// 철근가공조립
	m_structRebarManufacture.m_nSlabType				= 1;			// 슬래브 타입	- 복잡
	m_structRebarManufacture.m_nGuardFenceType			= 2;			// 방호벽 타입	- 보통
	m_structRebarManufacture.m_nExpansionJointType		= 3;			// 신축이음		- 간단
	m_structRebarManufacture.m_nBindConcType			= 2;			// 구속콘크리트 - 보통

	m_structBridgeName.m_nQty							= 0;			// 교명주 갯수
	m_structBridgeNamePlate.m_nQty						= 1;			// 교명판 갯수
	m_structExplainPlate.m_nQty							= 1;			// 설명판 갯수
	m_structEstablishTBM.m_nQty							= 1;			// TBM 갯수

	m_structDroppingPrevent.m_bApply					= TRUE;
	m_structDroppingPrevent.m_bIsOnlyHorizontal			= FALSE;
	m_structDroppingPrevent.m_dHorSpaceHeight			= 1000.0;
	m_structDroppingPrevent.m_dVerSpaceHeight			= 1000.0;

	m_structNotch.m_szNotchType							= _T("알루미늄판");

	m_structElecWireHole.m_bApply						= FALSE;
	m_structElecWireHole.m_nEA100[0]					= 1;
	m_structElecWireHole.m_nEA100[1]					= 1;
	m_structElecWireHole.m_nEA125[0]					= 1;
	m_structElecWireHole.m_nEA125[1]					= 1;
	m_structElecWireHole.m_nEA150[0]					= 0;
	m_structElecWireHole.m_nEA150[1]					= 0;	

	// 구조계산서 출력옵션	
	for(n=0; n<BMOPT_CHKSHEET; n++)
		m_bChkSheet[n]			= FALSE;			// ●구조계산서 출력 	
	for(n=0; n<BMOPT_CHKSH_2ND; n++)
		m_bChk2ndSh_Model[n]	= TRUE;				//	2장 하위항목
	for(n=0; n<BMOPT_CHKSH_3TH; n++)
		m_bChk3thSh_Force[n]	= TRUE;				//	3장 하위항목
	for(n=0; n<BMOPT_CHKSH_4TH; n++)
		m_bChk4thSh_Section[n]	= TRUE;				//	4장 하위항목
	for(n=0; n<BMOPT_CHKSH_5TH; n++)
		m_bChk5thSh_Joint[n]	= TRUE;				//	5장 하위항목		
	for(n=0; n<BMOPT_CHKSH_8TH; n++)
		m_bChk8thSh_Diap[n]		= TRUE;				//	8장 하위항목		
	for(n=0; n<BMOPT_CHKSH_9TH; n++)
		m_bChk9thSh_Check[n]	= TRUE;				//	9장 하위항목		
	
	m_nSh2ndOptGir				= 0;				//		2장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)	
	m_nSh4thOptGir				= 0;				//		4장 거더 구분 옵션 (0:거더구분않음, 1:모든거더구분)
	m_bChkSh9thPrecise			= TRUE;				//		9장 신축이음 정밀식
	m_bChkSh9thSimple			= FALSE;			//		9장 신축이음 간편식
}

// 어딘가 있을듯.... 근데 못 찾겠네.....
CString CBMOption::GetRebarDia(long nRebarIdx, double dSigmaY)
{
	CString szDia = _T(""), szFy = _T("");

	szFy = GetCharRebarMark(dSigmaY);
		
	if(nRebarIdx == 0) szDia = _T(szFy + "13");
	if(nRebarIdx == 1) szDia = _T(szFy + "16");
	if(nRebarIdx == 2) szDia = _T(szFy + "19");
	if(nRebarIdx == 3) szDia = _T(szFy + "22");
	if(nRebarIdx == 4) szDia = _T(szFy + "25");
	if(nRebarIdx == 5) szDia = _T(szFy + "29");
	if(nRebarIdx == 6) szDia = _T(szFy + "32");
	if(nRebarIdx == 7) szDia = _T(szFy + "35");
	
	return szDia;
}

long CBMOption::GetRebarDiaIndex(CString szDia)
{
	long nDiaIdx = 0;

	if(szDia == _T("H13") || szDia == _T("D13") || szDia == _T("U13") || szDia == _T("S13"))	nDiaIdx = 0;
	if(szDia == _T("H16") || szDia == _T("D16") || szDia == _T("U16") || szDia == _T("S16"))	nDiaIdx = 1;
	if(szDia == _T("H19") || szDia == _T("D19") || szDia == _T("U19") || szDia == _T("S19"))	nDiaIdx = 2;
	if(szDia == _T("H22") || szDia == _T("D22") || szDia == _T("U22") || szDia == _T("S22"))	nDiaIdx = 3;
	if(szDia == _T("H25") || szDia == _T("D25") || szDia == _T("U25") || szDia == _T("S25"))	nDiaIdx = 4;
	if(szDia == _T("H29") || szDia == _T("D29") || szDia == _T("U29") || szDia == _T("S29"))	nDiaIdx = 5;
	if(szDia == _T("H32") || szDia == _T("D32") || szDia == _T("U32") || szDia == _T("S32"))	nDiaIdx = 6;
	if(szDia == _T("H35") || szDia == _T("D35") || szDia == _T("U35") || szDia == _T("S35"))	nDiaIdx = 7;

	return nDiaIdx;
}

CString CBMOption::GetExpansionJointDia(BOOL bStt, long nIdx, double dSigmaY)
{
	CString szDia = _T("");
	if(bStt)	szDia = GetRebarDia(m_structExpansionJoint.m_nSttDiaIdx[nIdx], dSigmaY);
	else		szDia = GetRebarDia(m_structExpansionJoint.m_nEndDiaIdx[nIdx], dSigmaY);

	return szDia;
}

CString CBMOption::GetExpansionJointCoverPos(long nIdx)
{
	CString szPos = _T("");
	if(nIdx == GENERAL_GUARDFENCE)		szPos = _T("일반방호벽");
	if(nIdx == ABSOLUTE_GUARDFENCE)		szPos = _T("절대방호벽");
	if(nIdx == GENERAL_SOUNDPROOF)		szPos = _T("일반방음벽");
	if(nIdx == ABSOLUTE_SOUNDPROOF)		szPos = _T("절대방음벽");
	if(nIdx == CENTER_GUARDFENCE)		szPos = _T("중   분   대");
	if(nIdx == PARAPET)					szPos = _T("난         간");
	if(nIdx == CURB)					szPos = _T("연         석");

	return szPos;
}

CString	CBMOption::GetParapetType(long nIdx)
{
	CString szParapetType	= _T("");

	if(nIdx == DRIVEWAY)			szParapetType = _T("차   도   용");
	if(nIdx == FOOTWAY)				szParapetType = _T("보   도   용");
	if(nIdx == DRIVE_FOOT_BOUNDARY)	szParapetType = _T("보차도경계용");

	return szParapetType;
}

CString	CBMOption::GetBMOptItem(long nIdx)
{
	CString	szBMOpt	= _T("");

	if(nIdx == BMOPT_CONCRETE_BYE)			szBMOpt	= _T("콘크리트 구입");
	if(nIdx == BMOPT_CONCRETE_PLACING)		szBMOpt	= _T("콘크리트 타설");
	if(nIdx == BMOPT_MOLD)					szBMOpt	= _T("거푸집");
	if(nIdx == BMOPT_SPACER)				szBMOpt	= _T("스페이셔");
	if(nIdx == BMOPT_REBAR_MANUFACTURE)		szBMOpt	= _T("철근 가공 조립");
	if(nIdx == BMOPT_PAVE)					szBMOpt	= _T("포장");
	if(nIdx == BMOPT_TACKCOATING)			szBMOpt	= _T("택코팅");
	if(nIdx == BMOPT_DRAINBRIDGESURFACE)	szBMOpt	= _T("교면방수");
	if(nIdx == BMOPT_BRIDGENAME)			szBMOpt	= _T("교명주");
	if(nIdx == BMOPT_BRIDGENAME_PLATE)		szBMOpt	= _T("교명판");
	if(nIdx == BMOPT_EXPLAIN_PLATE)			szBMOpt	= _T("설명판");
	if(nIdx == BMOPT_ESTABLISH_TBM)			szBMOpt	= _T("TBM 설치");
	if(nIdx == BMOPT_STYROFOAM)				szBMOpt	= _T("스티로폼");
	if(nIdx == BMOPT_EXPANSIONJOINT)		szBMOpt	= _T("신축이음");
	if(nIdx == BMOPT_EXPANSIONJOINT_COVER)	szBMOpt	= _T("신축이음 덮개");
	if(nIdx == BMOPT_SHRINKAGE_CONCRETE)	szBMOpt	= _T("무수축 콘크리트");
	if(nIdx == BMOPT_WATER_DRAW)			szBMOpt	= _T("교면 물빼기");
	if(nIdx == BMOPT_NOTCH)					szBMOpt	= _T("NOTCH");
	if(nIdx == BMOPT_GUARD_FENCE)			szBMOpt	= _T("가드펜스");
	if(nIdx == BMOPT_SOUND_PROOF)			szBMOpt	= _T("방음판넬");
	if(nIdx == BMOPT_PARAPET)				szBMOpt	= _T("난간");
	if(nIdx == BMOPT_SHADE)					szBMOpt	= _T("차광망");
	if(nIdx == BMOPT_DROPPING_PREVENT)		szBMOpt	= _T("낙하물 방지공");
	if(nIdx == BMOPT_STEEL_QUANTITY)		szBMOpt	= _T("강재 수량");
	if(nIdx == BMOPT_PAINT)					szBMOpt	= _T("강교 도장");
	if(nIdx == BMOPT_DRAIN_ESTABLISH)		szBMOpt	= _T("배수 시설");
	if(nIdx == BMOPT_WING_WALL)				szBMOpt = _T("날 개 벽");
	if(nIdx == BMOPT_TIMBER)				szBMOpt = _T("동 바 리");
	if(nIdx == BMOPT_ELECWIREHOLE)			szBMOpt = _T("전 선 관");
	return szBMOpt;
}

CString CBMOption::GetDrainEstablishType()
{
	CString szDrain = _T("");
	if(m_nDrainEstablishType == RIVER)			szDrain = _T("하천용");
	if(m_nDrainEstablishType == WALKWAY)		szDrain = _T("육교용");
	if(m_nDrainEstablishType == RIVERWALKWAY)	szDrain = _T("하천및육교용");

	return szDrain;
}

long CBMOption::GetIdxDrainEstablishType(CString szType)
{
	long nIdx = 0;
	if(szType == _T("하천용"))			nIdx = RIVER;
	if(szType == _T("육교용"))			nIdx = WALKWAY;
	if(szType == _T("하천및육교용"))	nIdx = RIVERWALKWAY;

	return nIdx;
}

long CBMOption::GetIdxTimberQuality(CString szType)
{
	long nQuality = 0;
	if(szType == _T("목재"))	nQuality = 0;
	if(szType == _T("강관"))	nQuality = 1;

	return nQuality;
}

CString	CBMOption::GetTimberQuality(long nIdx)
{
	CString szQuality = _T("");

	if(nIdx == 0)	szQuality = _T("목재");
	if(nIdx == 1)	szQuality = _T("강관");

	return szQuality;
}

CString CBMOption::GetPosDeckFinisher(long nIdx)
{
	CString szPos = _T("");
	if(nIdx == 0)	szPos = _T("미적용");
	if(nIdx == 1)	szPos = _T("좌측적용");
	if(nIdx == 2)	szPos = _T("우측적용");
	if(nIdx == 3)	szPos = _T("양측적용");	

	return szPos;
}

long CBMOption::GetIdxDeckFinisher(CString szPos)
{
	long nIdx = 0;

	if(szPos == _T("미적용"))	nIdx = 0;
	if(szPos == _T("좌측적용"))	nIdx = 1;
	if(szPos == _T("우측적용"))	nIdx = 2;
	if(szPos == _T("양측적용"))	nIdx = 3;
	return nIdx;
}

long CBMOption::GetNotchPos(CString szPos)
{
	long nIdx = 0;
	if(szPos == _T("좌측적용"))	nIdx = 0;
	if(szPos == _T("우측적용"))	nIdx = 1;
	if(szPos == _T("양측적용"))	nIdx = 2;

	return nIdx;
}

CString CBMOption::GetNotchPos(long nPos)
{
	CString szPos	= _T("");
	if(nPos == 0)	szPos = _T("좌측적용");
	if(nPos == 1)	szPos = _T("우측적용");
	if(nPos == 2)	szPos = _T("양측적용");

	return szPos;
}

CString CBMOption::GetRebarManufactureType(long nType)
{
	CString szType = _T("");
	if(nType == 0)	szType = _T("매우복잡");
	if(nType == 1)	szType = _T("복잡");
	if(nType == 2)	szType = _T("보통");
	if(nType == 3)	szType = _T("간단");
	if(nType == 4)	szType = _T("미적용");

	return szType;
}

long CBMOption::GetIdxRebarManufactureType(CString szType)
{
	long nIdx = 0;
	if(szType == _T("매우복잡"))	nIdx = 0;
	if(szType == _T("복잡"))		nIdx = 1;
	if(szType == _T("보통"))		nIdx = 2;
	if(szType == _T("간단"))		nIdx = 3;
	if(szType == _T("미적용"))		nIdx = 4;
	return nIdx;
}

CString	CBMOption::GetDroppingPrevent(BOOL bApply)
{
	if(bApply)	return _T("수평적용");
	else		return _T("수평수직적용");

	return _T("");
}

BOOL CBMOption::GetDroppingPrevent(CString szApply)
{
	if(szApply == _T("수평적용"))			return TRUE;
	else if(szApply == _T("수평수직적용"))	return FALSE;

	return FALSE;
}