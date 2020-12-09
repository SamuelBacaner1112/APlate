// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#ifndef  __PLATEBASESTRU_H__
#define  __PLATEBASESTRU_H__

#define	SHARECONNECT_CROSS_MAX 20

#include "PlatePiecePoint.h"
#include "PlatePieceObject.h"


#define	STUD_HORI_SU			20
#define	STUD_JONG_SU			800		// 4.8 KBytes	//2000년 8월 8일 이전
#define	TON_INDEX				4
///////////////////////////////////////////////////////////
class AFX_EXT_CLASS CPlateSangSeDo : public CObject
{
// Constructor
public:
	CPlateSangSeDo(CPlateGirderApp *pGir);
	virtual ~CPlateSangSeDo();
// Attributes
public:
	double	m_WaterOut_W					;	// 결로집수구
	double	m_WaterOut_T					;
	double	m_WaterOut_L					;
	double	m_WaterOut_L2					;
	double	m_WaterOut_d1					;
	double	m_WaterOut_d2					;
	double	m_WaterOut_M					;
	double	m_WaterOut_R					;
	double	m_WaterOut_D					;
	double	m_WaterOut_DS1					; // 물끊음 장치 내경
	double	m_WaterOut_DS2					; // 물끊음 장치 외경
	double	m_WaterOut_ST					; // 물끊음 장치 길이
	double	m_JRib_A						;	//[종리브]
	BYTE	m_JRib_sUp						;	//종리브 상부 시종점부
	BYTE	m_JRib_sLow						;	//종리브 하부 시종점부
	double	m_HStiff_VStiff_B				;	//[수평보강재 부착]
	double	m_HStiff_VStiff_D				;	// 복부판측 직선길이
	double	m_HStiff_Splice_B				;
	double	m_Taper_W						;	//[단면 변화부 TAPER 처리 상세]
	double	m_Taper_H						;
	double	m_Scallop_R1					;	//[스캘럽 상세]
	double	m_Scallop_R2					;
	double	m_Chamfer_C1					;
	double	m_Chamfer_C2					;
	double	m_Scallop_JRib_R1				;	//[종리브 스캘럽상세]
	double	m_Scallop_JRib_D1				;	//압축부
	double	m_Scallop_JRib_R2				;	//인장부
	double	m_Scallop_JRib_D2				;	
	BYTE	m_ScallopType					;	//0=일반타입, 1=용접타입
	BYTE	m_ScallopBasePoint_Compression	;	//압축부 스캘럽 원중심 기준
	BYTE	m_ScallopBasePoint_Tension		;	//인장부 스캘럽 원중심 기준
	BYTE	m_ScallopRight					;	//압축부 스캘럽 우측, 좌측 타입
	double	m_Dongbari_A					;	//[동바리 고리 상세]
	double	m_Dongbari_B					;
	double	m_Dongbari_C					;
	double	m_Dongbari_D					;
	double	m_Dongbari_E					;
	double	m_Dongbari_F					;
	double	m_Dongbari_W					;
	double	m_Dongbari_T					;
	BYTE	m_Dongbari_Type					;	//0=일반타입, 1=플랜지 접착 타입
	double	m_SlabAnchor_A					;	//[슬래브앙커 고리 상세]
	double	m_SlabAnchor_B					;
	double	m_SlabAnchor_C					;
	double	m_SlabAnchor_T					;
	BYTE  m_SlabAnchorType				;	//0,1,2,3
	double	m_GassetPlate_S1				;	//[브라켓 보강판 상세]
	double	m_GassetPlate_S2				;
	double	m_GassetPlate_S3				;
	double	m_GassetPlate_W					;
	double	m_GassetPlate_T					;
	double	m_GassetPlate_dA				;
	double	m_GassetPlate_dB				;
	double	m_GassetPlate_dC				;
	double	m_GassetPlate_uA				;
	double	m_GassetPlate_uB				;
	double	m_GassetPlate_uC				;
	double	m_FanPlate_uW					;	//[환풍기 받침대 상세]
	double	m_FanPlate_uT					;
	double	m_FanPlate_uL					;
	double	m_FanPlate_uS					;
	double	m_FanPlate_dW					;
	double	m_FanPlate_dT					;
	double	m_FanPlate_dL					;
	double	m_FanPlate_dS1					;
	double	m_FanPlate_dS2					;
	double	m_Ladder_W						;	//[받침대 ROUND 상세]
	double	m_Ladder_T						;
	double	m_Ladder_L						;
	long	m_InOutGateType					;
	double	m_InOutGate_Stt_W				;	//[내부출입구 상세]
	double	m_InOutGate_Stt_H				;
	double	m_InOutGate_Stt_R				;	//내측 R
	double	m_InOutGate_End_W				;	//[내부출입구 상세]
	double	m_InOutGate_End_H				;
	double	m_InOutGate_End_R				;	//내측 R
	long	m_ManholeType					;
	double	m_Manhole_W						;	//[맨홀 상세]
	double	m_Manhole_H						;
	double	m_Manhole_R						;
	long	m_VentilatorType					;
	BYTE	m_Ventilator_POS				;	//[환기구 위치]
	double	m_Ventilator_H					;
	double	m_Ventilator_D					;	//[환기구 상세]
	double	m_Ventilator_sD					;	//OPEN하단 ~ 받침대상면
	double	m_Ventilator_uW					;	//[환기구받침대]
	double	m_Ventilator_uT					;
	double	m_Ventilator_uL					;
	double	m_Ventilator_dW					;
	double	m_Ventilator_dT					;
	double	m_Ventilator_dL					;
	double	m_Ventilator_dS1				;
	double	m_Ventilator_dS2				;
	//--- 들고리
	long	m_nTonIndex						;	//톤 종류수
	double	m_LiftLug_I[TON_INDEX]			;	//구분
	double	m_LiftLug_A[TON_INDEX]			;	//왼편 가로 
	double	m_LiftLug_B[TON_INDEX]			;	//오른편 가로 
	double	m_LiftLug_C[TON_INDEX]			;	//직선부 세로 
	double	m_LiftLug_D[TON_INDEX]			;	//구멍 중심까지 세로 
	double	m_LiftLug_d[TON_INDEX]			;	//구멍 지름
	double	m_LiftLug_R[TON_INDEX]			;	//상단 둥근부분 반지름
	double	m_LiftLug_T[TON_INDEX]			;	//두께
	double	m_LiftLug_iR[TON_INDEX]			;	//보강판 반지름
	double	m_LiftLug_sA[TON_INDEX]			;	//왼편 측보강판 두께 
	double	m_LiftLug_sB[TON_INDEX]			;	//오른편 측보강판판 두께 
	double	m_LiftLug_lT[TON_INDEX]			;	//보강판 왼편 두께
	double	m_LiftLug_rT[TON_INDEX]			;	//보강판 오른편 오께
	double	m_LiftLug_lW[TON_INDEX]			;	//측보강판 왼편 가로
	double	m_LiftLug_rW[TON_INDEX]			;	//측보강판 오른편 가로
	BOOL	m_LiftLug_bAddPlate[TON_INDEX]	;	//보강판
	BOOL	m_LiftLug_bSidePlate[TON_INDEX]	;	//편복부판

	double	m_StudHoriSQ_H						;	// 각형 전단연결재
	double	m_StudHoriSQ_W						;	// 각형 전단연결재
	double	m_StudHoriSQ_L						;	// 각형 전단연결재
	double	m_StudHori_D						;	//[전단연결재 횡방향 위치]
	double	m_StudHori_H						;
	double	m_StudHori_E						;
	BYTE	m_StudHori_Type						;	//0=슬래브 앵커, 1=전단연결재, 2=각형
	long	m_StudHori_Qty						;	//전단연결재 수량
	double	m_StudHori_Dis[STUD_HORI_SU]		;	//다이아프램 위에 존재하는 전단
	double	m_StudJong_MinLenFactor				;	//[전단연결재 종방향 위치]
	double	m_StudJong_MinDis					;	//(125)
	double	m_StudJong_MidLenFactor				;	//중간기준 길이(5000)
	double	m_StudJong_MidDis					;	//중간 간격(250)
	double	m_StudJong_MaxLen					;	//최대 길이
	double	m_StudJong_MaxDis					;	//최대 간격
	BYTE	m_StudJong_GirderStart				;	//거더 시점부터 시작
	BYTE	m_StudJong_FactJoin					;
	BYTE	m_StudJong_SpliceCenter				;
	BYTE	m_StudJong_HBeamVStiff				;	// 가로보와 연결된 수직보강재 위의 전달연결재 배치 여부..
	double	m_StudJong_BaseTerm[STUD_JONG_SU]	;
	double	m_StudJong_BaseDis[STUD_JONG_SU]	;
	long	m_StudJong_Studsu[STUD_JONG_SU]		;
	double	m_StudJong_StudDis[STUD_JONG_SU]	;

	CTypedPtrArray <CObArray, long*> m_StudJong_StudQty;

	// 인입구 보강재 설치여부
	long	m_nFacePlateUse_Stt;
	long	m_nFacePlateUse_End;

	CPlateGirderApp *m_pGir;

// Implementation
public:
	BOOL IsSameWaterOut(const CPlateSangSeDo& Obj);
	void SerializeSangse(CArchive& ar);
	void SerializeForOldVersion(CArchive& ar);
	CPlateSangSeDo& operator=(const CPlateSangSeDo& Obj);

public:
	void SetFillAssemVariable(CDoubleArray& dArr);
	//들고리 개체 생성
	void CreatePieceObjLiftLug(CPlatePieceObject *pObj);	
	//들고리3D
	void CreatePieceObjLiftLugMainPlate3D(CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft); //들고리
	void CreatePieceObjLiftLugAddPlate3D( CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft, BOOL bFront); //보강판
	void CreatePieceObjLiftLugSidePlate3D(CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft, BOOL bFront); //측면판
	///////////////////////////////////////////////////////////////////
	//환풍기 받침대 : 20011027JSH
	//다이아프램 수평판
	void CreatePieceObjVentDiaHPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentDiaHPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bFront);
	//다이아프램 수직판
	void CreatePieceObjVentDiaVPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bFront);
	void CreatePieceObjVentDiaVPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bFront, BOOL bLeft);
	//복부판 수평판
	void CreatePieceObjVentWebHPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentWebHPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft);
	//복부판 수직판
	void CreatePieceObjVentWebVPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentWebVPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft);
	//환기구 플레이트
	void CreatePieceObjVentWebPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentWebPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft);
	///////////////////////////////////////////////////////////////////
	//유지보수용 동바리 고리 : 20011208JSH
	//일반형
	void CreatePieceObjStagingLinkGeneral(CPlatePieceObject *pObj);
	//플랜지 접합형
	void CreatePieceObjStagingLinkFlangeBond(CPlatePieceObject *pObj);
	//3D
	void CreatePieceObjStagingLink3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bUpper);
	//출입구문생성
	void CreatePieceObjAbutDoorPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorL3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjAbutDoorL(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorRebar3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjAbutDoorRebar(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorPlateSup3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjAbutDoorPlateSup(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	//맨홀문 생성
	void CreatePieceObjManholeDoorPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorPlate(CPlatePieceObject  *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorPlateAL3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorPlateAL(CPlatePieceObject  *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorPlateXP133D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjManholeDoorPlateXP13(CPlatePieceObject  *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorPlateXP223D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjManholeDoorPlateXP22(CPlatePieceObject  *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorL3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjManholeDoorL(CPlatePieceObject  *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorRebar3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjManholeDoorRebar(CPlatePieceObject  *pObj, CPlateBasicIndex *pBx);

private:
	void CreatePieceObjLiftLugMainPlate(CPlatePieceObject *pObj);
	void CreatePieceObjLiftLugAddPlate(CPlatePieceObject *pObj);
	void CreatePieceObjLiftLugSidePlate(CPlatePieceObject *pObj);
};


#endif __PLATEBASESTRU_H__


