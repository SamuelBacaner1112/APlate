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
#define	STUD_JONG_SU			800		// 4.8 KBytes	//2000�� 8�� 8�� ����
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
	double	m_WaterOut_W					;	// ���������
	double	m_WaterOut_T					;
	double	m_WaterOut_L					;
	double	m_WaterOut_L2					;
	double	m_WaterOut_d1					;
	double	m_WaterOut_d2					;
	double	m_WaterOut_M					;
	double	m_WaterOut_R					;
	double	m_WaterOut_D					;
	double	m_WaterOut_DS1					; // ������ ��ġ ����
	double	m_WaterOut_DS2					; // ������ ��ġ �ܰ�
	double	m_WaterOut_ST					; // ������ ��ġ ����
	double	m_JRib_A						;	//[������]
	BYTE	m_JRib_sUp						;	//������ ��� ��������
	BYTE	m_JRib_sLow						;	//������ �Ϻ� ��������
	double	m_HStiff_VStiff_B				;	//[���򺸰��� ����]
	double	m_HStiff_VStiff_D				;	// �������� ��������
	double	m_HStiff_Splice_B				;
	double	m_Taper_W						;	//[�ܸ� ��ȭ�� TAPER ó�� ��]
	double	m_Taper_H						;
	double	m_Scallop_R1					;	//[��Ķ�� ��]
	double	m_Scallop_R2					;
	double	m_Chamfer_C1					;
	double	m_Chamfer_C2					;
	double	m_Scallop_JRib_R1				;	//[������ ��Ķ����]
	double	m_Scallop_JRib_D1				;	//�����
	double	m_Scallop_JRib_R2				;	//�����
	double	m_Scallop_JRib_D2				;	
	BYTE	m_ScallopType					;	//0=�Ϲ�Ÿ��, 1=����Ÿ��
	BYTE	m_ScallopBasePoint_Compression	;	//����� ��Ķ�� ���߽� ����
	BYTE	m_ScallopBasePoint_Tension		;	//����� ��Ķ�� ���߽� ����
	BYTE	m_ScallopRight					;	//����� ��Ķ�� ����, ���� Ÿ��
	double	m_Dongbari_A					;	//[���ٸ� �� ��]
	double	m_Dongbari_B					;
	double	m_Dongbari_C					;
	double	m_Dongbari_D					;
	double	m_Dongbari_E					;
	double	m_Dongbari_F					;
	double	m_Dongbari_W					;
	double	m_Dongbari_T					;
	BYTE	m_Dongbari_Type					;	//0=�Ϲ�Ÿ��, 1=�÷��� ���� Ÿ��
	double	m_SlabAnchor_A					;	//[�������Ŀ �� ��]
	double	m_SlabAnchor_B					;
	double	m_SlabAnchor_C					;
	double	m_SlabAnchor_T					;
	BYTE  m_SlabAnchorType				;	//0,1,2,3
	double	m_GassetPlate_S1				;	//[����� ������ ��]
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
	double	m_FanPlate_uW					;	//[ȯǳ�� ��ħ�� ��]
	double	m_FanPlate_uT					;
	double	m_FanPlate_uL					;
	double	m_FanPlate_uS					;
	double	m_FanPlate_dW					;
	double	m_FanPlate_dT					;
	double	m_FanPlate_dL					;
	double	m_FanPlate_dS1					;
	double	m_FanPlate_dS2					;
	double	m_Ladder_W						;	//[��ħ�� ROUND ��]
	double	m_Ladder_T						;
	double	m_Ladder_L						;
	long	m_InOutGateType					;
	double	m_InOutGate_Stt_W				;	//[�������Ա� ��]
	double	m_InOutGate_Stt_H				;
	double	m_InOutGate_Stt_R				;	//���� R
	double	m_InOutGate_End_W				;	//[�������Ա� ��]
	double	m_InOutGate_End_H				;
	double	m_InOutGate_End_R				;	//���� R
	long	m_ManholeType					;
	double	m_Manhole_W						;	//[��Ȧ ��]
	double	m_Manhole_H						;
	double	m_Manhole_R						;
	long	m_VentilatorType					;
	BYTE	m_Ventilator_POS				;	//[ȯ�ⱸ ��ġ]
	double	m_Ventilator_H					;
	double	m_Ventilator_D					;	//[ȯ�ⱸ ��]
	double	m_Ventilator_sD					;	//OPEN�ϴ� ~ ��ħ����
	double	m_Ventilator_uW					;	//[ȯ�ⱸ��ħ��]
	double	m_Ventilator_uT					;
	double	m_Ventilator_uL					;
	double	m_Ventilator_dW					;
	double	m_Ventilator_dT					;
	double	m_Ventilator_dL					;
	double	m_Ventilator_dS1				;
	double	m_Ventilator_dS2				;
	//--- ���
	long	m_nTonIndex						;	//�� ������
	double	m_LiftLug_I[TON_INDEX]			;	//����
	double	m_LiftLug_A[TON_INDEX]			;	//���� ���� 
	double	m_LiftLug_B[TON_INDEX]			;	//������ ���� 
	double	m_LiftLug_C[TON_INDEX]			;	//������ ���� 
	double	m_LiftLug_D[TON_INDEX]			;	//���� �߽ɱ��� ���� 
	double	m_LiftLug_d[TON_INDEX]			;	//���� ����
	double	m_LiftLug_R[TON_INDEX]			;	//��� �ձٺκ� ������
	double	m_LiftLug_T[TON_INDEX]			;	//�β�
	double	m_LiftLug_iR[TON_INDEX]			;	//������ ������
	double	m_LiftLug_sA[TON_INDEX]			;	//���� �������� �β� 
	double	m_LiftLug_sB[TON_INDEX]			;	//������ ���������� �β� 
	double	m_LiftLug_lT[TON_INDEX]			;	//������ ���� �β�
	double	m_LiftLug_rT[TON_INDEX]			;	//������ ������ ����
	double	m_LiftLug_lW[TON_INDEX]			;	//�������� ���� ����
	double	m_LiftLug_rW[TON_INDEX]			;	//�������� ������ ����
	BOOL	m_LiftLug_bAddPlate[TON_INDEX]	;	//������
	BOOL	m_LiftLug_bSidePlate[TON_INDEX]	;	//������

	double	m_StudHoriSQ_H						;	// ���� ���ܿ�����
	double	m_StudHoriSQ_W						;	// ���� ���ܿ�����
	double	m_StudHoriSQ_L						;	// ���� ���ܿ�����
	double	m_StudHori_D						;	//[���ܿ����� Ⱦ���� ��ġ]
	double	m_StudHori_H						;
	double	m_StudHori_E						;
	BYTE	m_StudHori_Type						;	//0=������ ��Ŀ, 1=���ܿ�����, 2=����
	long	m_StudHori_Qty						;	//���ܿ����� ����
	double	m_StudHori_Dis[STUD_HORI_SU]		;	//���̾����� ���� �����ϴ� ����
	double	m_StudJong_MinLenFactor				;	//[���ܿ����� ������ ��ġ]
	double	m_StudJong_MinDis					;	//(125)
	double	m_StudJong_MidLenFactor				;	//�߰����� ����(5000)
	double	m_StudJong_MidDis					;	//�߰� ����(250)
	double	m_StudJong_MaxLen					;	//�ִ� ����
	double	m_StudJong_MaxDis					;	//�ִ� ����
	BYTE	m_StudJong_GirderStart				;	//�Ŵ� �������� ����
	BYTE	m_StudJong_FactJoin					;
	BYTE	m_StudJong_SpliceCenter				;
	BYTE	m_StudJong_HBeamVStiff				;	// ���κ��� ����� ���������� ���� ���޿����� ��ġ ����..
	double	m_StudJong_BaseTerm[STUD_JONG_SU]	;
	double	m_StudJong_BaseDis[STUD_JONG_SU]	;
	long	m_StudJong_Studsu[STUD_JONG_SU]		;
	double	m_StudJong_StudDis[STUD_JONG_SU]	;

	CTypedPtrArray <CObArray, long*> m_StudJong_StudQty;

	// ���Ա� ������ ��ġ����
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
	//��� ��ü ����
	void CreatePieceObjLiftLug(CPlatePieceObject *pObj);	
	//���3D
	void CreatePieceObjLiftLugMainPlate3D(CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft); //���
	void CreatePieceObjLiftLugAddPlate3D( CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft, BOOL bFront); //������
	void CreatePieceObjLiftLugSidePlate3D(CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft, BOOL bFront); //������
	///////////////////////////////////////////////////////////////////
	//ȯǳ�� ��ħ�� : 20011027JSH
	//���̾����� ������
	void CreatePieceObjVentDiaHPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentDiaHPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bFront);
	//���̾����� ������
	void CreatePieceObjVentDiaVPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bFront);
	void CreatePieceObjVentDiaVPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bFront, BOOL bLeft);
	//������ ������
	void CreatePieceObjVentWebHPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentWebHPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft);
	//������ ������
	void CreatePieceObjVentWebVPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentWebVPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft);
	//ȯ�ⱸ �÷���Ʈ
	void CreatePieceObjVentWebPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjVentWebPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft);
	///////////////////////////////////////////////////////////////////
	//���������� ���ٸ� �� : 20011208JSH
	//�Ϲ���
	void CreatePieceObjStagingLinkGeneral(CPlatePieceObject *pObj);
	//�÷��� ������
	void CreatePieceObjStagingLinkFlangeBond(CPlatePieceObject *pObj);
	//3D
	void CreatePieceObjStagingLink3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bUpper);
	//���Ա�������
	void CreatePieceObjAbutDoorPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorL3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjAbutDoorL(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorRebar3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjAbutDoorRebar(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	void CreatePieceObjAbutDoorPlateSup3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow);
	void CreatePieceObjAbutDoorPlateSup(CPlatePieceObject *pObj, CPlateBasicIndex *pBx);
	//��Ȧ�� ����
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


