#include "stdafx.h"
#include "APlateData.h"
#include "PlateBaseStru.h"

//<summary>
//������
//</summary>
//<return></return>
CPlateSangSeDo::CPlateSangSeDo(CPlateGirderApp *pGir)
{
	m_pGir = pGir;

	m_WaterOut_W					=  0;	// ���������
	m_WaterOut_T					=  0;
	m_WaterOut_L					=  0;
	m_WaterOut_L2					=  0;
	m_WaterOut_d1					=  0;
	m_WaterOut_d2					=  0;
	m_WaterOut_M					=  0;
	m_WaterOut_R					=  0;
	m_WaterOut_D					=  0;
	m_WaterOut_DS1					=  0;
	m_WaterOut_DS2					=  0;
	m_WaterOut_ST					=  0;
	m_JRib_A						=  0;	//[������]
	m_JRib_sUp						=  0;	//������ ��� ��������
	m_JRib_sLow						=  0;	//������ �Ϻ� ��������
	m_HStiff_VStiff_B				= 35;//[���򺸰��� ����]
	m_HStiff_VStiff_D				= 35;
	m_HStiff_Splice_B				= 35;
	m_Taper_W						=  0;	//[�ܸ� ��ȭ�� TAPER ó�� ��]
	m_Taper_H						=  0;
	m_Scallop_R1					=  0;	//[��Ķ�� ��]
	m_Scallop_R2					=  0;
	m_Chamfer_C1					=  0;
	m_Chamfer_C2					=  0;	
	m_Scallop_JRib_R1				=  0;	//[������ ��Ķ����]
	m_Scallop_JRib_D1				=  0;	//�����
	m_Scallop_JRib_R2				=  0;	//�����
	m_Scallop_JRib_D2				=  0;	
	m_ScallopType					=  0;	//0=�Ϲ�Ÿ��, 1=����Ÿ��
	m_ScallopBasePoint_Compression	=  0;	//����� ��Ķ�� ���߽� ����
	m_ScallopBasePoint_Tension		=  0;	//����� ��Ķ�� ���߽� ����
	m_ScallopRight					=  0;	//����� ��Ķ�� ����, ���� Ÿ��
	m_Dongbari_A					=  0;	//[���ٸ� �� ��]
	m_Dongbari_B					=  0;
	m_Dongbari_C					=  0;
	m_Dongbari_D					=  0;
	m_Dongbari_E					=  0;
	m_Dongbari_F					=  0;
	m_Dongbari_W					=  0;
	m_Dongbari_T					= 10;
	m_Dongbari_Type					=  0;
	m_GassetPlate_S1				=  0;	//[����� ������ ��]
	m_GassetPlate_S2				=  0;
	m_GassetPlate_S3				=  0;
	m_GassetPlate_W					=  0;
	m_GassetPlate_T					=  0;
	m_GassetPlate_dA				=  0;
	m_GassetPlate_dB				=  0;
	m_GassetPlate_dC				=  0;
	m_GassetPlate_uA				=  0;
	m_GassetPlate_uB				=  0;
	m_GassetPlate_uC				=  0;
	m_FanPlate_uW					=  0;	//[ȯǳ�� ��ħ�� ��]
	m_FanPlate_uT					=  0;
	m_FanPlate_uL					=  0;
	m_FanPlate_uS					=  0;
	m_FanPlate_dW					=  0;
	m_FanPlate_dT					=  0;
	m_FanPlate_dL					=  0;
	m_FanPlate_dS1					=  0;
	m_FanPlate_dS2					=  0;
	m_Ladder_W						=  0;	//[��ħ�� ROUND ��]
	m_Ladder_T						=  0;
	m_Ladder_L						=  0;
	m_InOutGateType					=  1;	//[�������Ա� ��]
	m_InOutGate_Stt_W				=  0;	//[�������Ա� ��]
	m_InOutGate_Stt_H				=  0;
	m_InOutGate_Stt_R				=  0;	//���� R
	m_InOutGate_End_W				=  0;	//[�������Ա� ��]
	m_InOutGate_End_H				=  0;
	m_InOutGate_End_R				=  0;	//���� R
	m_ManholeType					=  1;	//[��Ȧ ��]
	m_Manhole_W						=  0;	//[��Ȧ ��]
	m_Manhole_H						=  0;
	m_Manhole_R						=  0;
	m_VentilatorType				=  1;	// [ȯ�ⱸ ��ġ]
	m_Ventilator_POS				=  1;	// [ȯ�ⱸ ��ġ]
	m_Ventilator_H					=  0; 	
	m_Ventilator_D					=  0;	// [ȯ�ⱸ ��]
	m_Ventilator_uW					=  0;
	m_Ventilator_sD					=  0;
	m_Ventilator_uT					=  10;
	m_Ventilator_uW					=  0;
	m_Ventilator_uL					=  0;
	m_Ventilator_dW					=  0;
	m_Ventilator_dT					=  0;
	m_Ventilator_dL					=  0;
	m_Ventilator_dS1				=  0;
	m_Ventilator_dS2				=  0;
	m_StudHoriSQ_H					= 50;	// ���� ���ܿ�����
	m_StudHoriSQ_W					= 30;	// ���� ���ܿ�����
	m_StudHoriSQ_L					= 250;	// ���� ���ܿ�����
	m_StudHori_D					=  0;	//[���ܿ����� Ⱦ���� ��ġ]
	m_StudHori_H					=  0;
	m_StudHori_E					=  0;
	m_StudHori_Type					=  0;	//0=���ܿ�����, 1=������ ��Ŀ
	m_StudHori_Qty					=  0;	//���ܿ����� ����
	m_StudJong_MinLenFactor			=  0;	//[���ܿ����� ������ ��ġ]
	m_StudJong_MinDis				=  125;	//(125)
	m_StudJong_MidLenFactor			=  0;	//�߰����� ����(5000)
	m_StudJong_MidDis				=  250;	//�߰� ����(250)
	m_StudJong_MaxLen				=  0;	//�ִ� ����
	m_StudJong_MaxDis				=  500;	//�ִ� ����
	m_StudJong_GirderStart			=  0;	//�Ŵ� �������� ����
	m_StudJong_FactJoin				=  0;
	m_StudJong_SpliceCenter			=  0;
	long n = 0;
	for(n=0; n < STUD_HORI_SU; n++)
		m_StudHori_Dis[n] = 0;	//���̾����� ���� �����ϴ� ����

	for(n=0; n < STUD_JONG_SU; n++) 
	{ 
		m_StudJong_BaseTerm[n] = 0;	// BasicIndex�� �´� ���ܿ�����
		m_StudJong_BaseDis[n]  = 0; 
		m_StudJong_Studsu[n]   = 0;	// ���̺���
		m_StudJong_StudDis[n]  = 0;
	} 

	m_nTonIndex = 0;				//�� ������
	for(n=0; n<TON_INDEX; n++)
	{
		m_LiftLug_I[n]			= 0;		//����
		m_LiftLug_A[n]			= 0;		//���� ���� 
		m_LiftLug_B[n]			= 0;		//������ ���� 
		m_LiftLug_C[n]			= 0;		//������ ���� 
		m_LiftLug_D[n]			= 0;		//���� �߽ɱ��� ���� 
		m_LiftLug_d[n]			= 0;		//���� ����
		m_LiftLug_R[n]			= 0;		//��� �ձٺκ� ������
		m_LiftLug_T[n]			= 0;		//�β�
		m_LiftLug_iR[n]			= 0;		//������ ������
		m_LiftLug_sA[n]			= 0;		//���� �������� �β� 
		m_LiftLug_sB[n]			= 0;		//������ ���������� �β� 
		m_LiftLug_lT[n]			= 0;		//������ ���� �β�
		m_LiftLug_rT[n]			= 0;		//������ ������ ����
		m_LiftLug_lW[n]			= 0;		//�������� ���� ����
		m_LiftLug_rW[n]			= 0;		//�������� ������ ����
		m_LiftLug_bAddPlate[n]	= FALSE;	//������
		m_LiftLug_bSidePlate[n]	= FALSE;	//������
	}

	m_SlabAnchor_A	= 0;
	m_SlabAnchor_B	= 0;
	m_SlabAnchor_C	= 0;
	m_SlabAnchor_T	= 0;
	m_SlabAnchorType	= 0;
	m_nFacePlateUse_Stt = 1;
	m_nFacePlateUse_End = 1;
}
//<summary>
//�Ҹ���
//</summary>
//<return></return>
CPlateSangSeDo::~CPlateSangSeDo()
{
}


//<summary>
//����
//</summary>
//<param name="ar"></param>
void CPlateSangSeDo::SerializeSangse(CArchive& ar)
{
	long nFlag = 11;	// 071214 lina - ���ܿ����� ����
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_WaterOut_W						;	// ���������
		ar << m_WaterOut_T						;
		ar << m_WaterOut_L						;
		ar << m_WaterOut_L2						;
		ar << m_WaterOut_d1						;
		ar << m_WaterOut_d2						;
		ar << m_WaterOut_M						;
		ar << m_WaterOut_R						;
		ar << m_WaterOut_D						;
		ar << m_WaterOut_DS1					;
		ar << m_WaterOut_DS2					;
		ar << m_WaterOut_ST						;
		ar << m_JRib_A							;	//[������]
		ar << m_JRib_sUp						;	//������ ��� ��������
		ar << m_JRib_sLow						;	//������ �Ϻ� ��������
		ar << m_HStiff_VStiff_B					;	//[���򺸰��� ����]
		ar << m_HStiff_VStiff_D					;
		ar << m_HStiff_Splice_B					;
		ar << m_Taper_W							;	//[�ܸ� ��ȭ�� TAPER ó�� ��]
		ar << m_Taper_H							;
		ar << m_Scallop_R1						;	//[��Ķ�� ��]
		ar << m_Scallop_R2						;
		ar << m_Chamfer_C1						;
		ar << m_Chamfer_C2						;		
		ar << m_Scallop_JRib_R1					;	//[������ ��Ķ����]
		ar << m_Scallop_JRib_D1					;	//�����
		ar << m_Scallop_JRib_R2					;	//�����
		ar << m_Scallop_JRib_D2					;		
		ar << m_ScallopType						;	//0=�Ϲ�Ÿ��, 1=����Ÿ��
		ar << m_ScallopBasePoint_Compression	;	//����� ��Ķ�� ���߽� ����
		ar << m_ScallopBasePoint_Tension		;	//����� ��Ķ�� ���߽� ����
		ar << m_ScallopRight					;	//����� ��Ķ�� ����, ���� Ÿ��
		ar << m_Dongbari_A						;	//[���ٸ� �� ��]
		ar << m_Dongbari_B						;
		ar << m_Dongbari_C						;
		ar << m_Dongbari_D						;
		ar << m_Dongbari_E						;
		ar << m_Dongbari_F						;
		ar << m_Dongbari_W						;
		ar << m_Dongbari_T						;
		ar << m_Dongbari_Type					;
		ar << m_GassetPlate_S1					;	//[����� ������ ��]
		ar << m_GassetPlate_S2					;
		ar << m_GassetPlate_S3					;
		ar << m_GassetPlate_W					;
		ar << m_GassetPlate_T					;
		ar << m_GassetPlate_dA					;
		ar << m_GassetPlate_dB					;
		ar << m_GassetPlate_dC					;
		ar << m_GassetPlate_uA					;
		ar << m_GassetPlate_uB					;
		ar << m_GassetPlate_uC					;
		ar << m_FanPlate_uW						;	//[ȯǳ�� ��ħ�� ��]
		ar << m_FanPlate_uT						;
		ar << m_FanPlate_uL						;
		ar << m_FanPlate_uS						;
		ar << m_FanPlate_dW						;
		ar << m_FanPlate_dT						;
		ar << m_FanPlate_dL						;
		ar << m_FanPlate_dS1					;
		ar << m_FanPlate_dS2					;
		ar << m_Ladder_W						;	//[��ħ�� ROUND ��]
		ar << m_Ladder_T						;
		ar << m_Ladder_L						;
		ar << m_InOutGateType					;	//[�������Ա� ��]
		ar << m_InOutGate_Stt_W					;
		ar << m_InOutGate_Stt_H					;
		ar << m_InOutGate_Stt_R					;	//���� R
		ar << m_InOutGate_End_W					;	//[�������Ա� ��]
		ar << m_InOutGate_End_H					;
		ar << m_InOutGate_End_R					;	//���� R
		ar << m_ManholeType						;	//[��Ȧ ��]
		ar << m_Manhole_W						;
		ar << m_Manhole_H						;
		ar << m_Manhole_R						;
		ar << m_VentilatorType					;	//[ȯ�ⱸ ��]
		ar << m_Ventilator_POS					;
		ar << m_Ventilator_H					;
		ar << m_Ventilator_D					;
		ar << m_Ventilator_sD					;	// ADD 2002.2.20
		ar << m_Ventilator_uW					;	// ADD 2002.2.15
		ar << m_Ventilator_uT					;	// .
		ar << m_Ventilator_uL					;	// .
		ar << m_Ventilator_dW					;	// .
		ar << m_Ventilator_dT					;	// .
		ar << m_Ventilator_dL					;	// .
		ar << m_Ventilator_dS1					;	// .
		ar << m_Ventilator_dS2					;	// ADD 2002.2.15
		ar << m_StudHori_D						;	//[���ܿ����� Ⱦ���� ��ġ]
		ar << m_StudHori_H						;
		ar << m_StudHori_E						;
		ar << m_StudHori_Type					;	//0=���ܿ�����, 1=������ ��Ŀ
		ar << m_StudHori_Qty					;	//���ܿ����� ����
		ar << m_StudJong_MinLenFactor			;	//[���ܿ����� ������ ��ġ]
		ar << m_StudJong_MinDis					;	//(125)
		ar << m_StudJong_MidLenFactor			;	//�߰����� ����(5000)
		ar << m_StudJong_MidDis					;	//�߰� ����(250)
		ar << m_StudJong_MaxLen					;	//�ִ� ����
		ar << m_StudJong_MaxDis					;	//�ִ� ����
		ar << m_StudJong_GirderStart			;	//�Ŵ� �������� ����
		ar << m_StudJong_FactJoin				;
		ar << m_StudJong_SpliceCenter			;
		long n = 0;
		for(n=0; n < STUD_HORI_SU; n++)
			ar << m_StudHori_Dis[n];			//���̾����� ���� �����ϴ� ����

		for(n=0; n < STUD_JONG_SU; n++)
		{
			ar << m_StudJong_BaseDis[n]	;
			ar << m_StudJong_BaseTerm[n];
			ar << m_StudJong_Studsu[n]	;
			ar << m_StudJong_StudDis[n]	;
		}

		ar << m_nTonIndex;				//�� ������
		for(n=0; n<TON_INDEX; n++)
		{
			ar << m_LiftLug_I[n]			;	//����
			ar << m_LiftLug_A[n]			;	//���� ���� 
			ar << m_LiftLug_B[n]			;	//������ ���� 
			ar << m_LiftLug_C[n]			;	//������ ���� 
			ar << m_LiftLug_D[n]			;	//���� �߽ɱ��� ���� 
			ar << m_LiftLug_d[n]			;	//���� ����
			ar << m_LiftLug_R[n]			;	//��� �ձٺκ� ������
			ar << m_LiftLug_T[n]			;	//�β�
			ar << m_LiftLug_iR[n]			;	//������ ������
			ar << m_LiftLug_sA[n]			;	//���� �������� �β� 
			ar << m_LiftLug_sB[n]			;	//������ ���������� �β� 
			ar << m_LiftLug_lT[n]			;	//������ ���� �β�
			ar << m_LiftLug_rT[n]			;	//������ ������ ����
			ar << m_LiftLug_lW[n]			;	//�������� ���� ����
			ar << m_LiftLug_rW[n]			;	//�������� ������ ����
			ar << m_LiftLug_bAddPlate[n]	;	//������
			ar << m_LiftLug_bSidePlate[n]	;	//������
		}
		ar << m_SlabAnchor_A;
		ar << m_SlabAnchor_B;
		ar << m_SlabAnchor_C;
		ar << m_SlabAnchor_T;
		ar << m_SlabAnchorType;
		// ���Ա� ������ ��ġ����
		ar << m_nFacePlateUse_Stt;
		ar << m_nFacePlateUse_End;
		ar << m_StudHoriSQ_H;
		ar << m_StudHoriSQ_W;
		ar << m_StudHoriSQ_L;
	}
	else
	{
		ar >> nFlag								;
		ar >> m_WaterOut_W						;	// ���������
		ar >> m_WaterOut_T						;
		ar >> m_WaterOut_L						;
		ar >> m_WaterOut_L2						;
		ar >> m_WaterOut_d1						;
		ar >> m_WaterOut_d2						;
		ar >> m_WaterOut_M						;
		ar >> m_WaterOut_R						;
		ar >> m_WaterOut_D						;
		if(nFlag>4)
		{
			ar >> m_WaterOut_DS1				;
			ar >> m_WaterOut_DS2				;
			ar >> m_WaterOut_ST					;
		}
		ar >> m_JRib_A							;	//[������]
		ar >> m_JRib_sUp						;	//������ ��� ��������
		ar >> m_JRib_sLow						;	//������ �Ϻ� ��������
		ar >> m_HStiff_VStiff_B					;	//[���򺸰��� ����]
		ar >> m_HStiff_VStiff_D					;
		ar >> m_HStiff_Splice_B					;
		ar >> m_Taper_W							;	//[�ܸ� ��ȭ�� TAPER ó�� ��]
		ar >> m_Taper_H							;
		ar >> m_Scallop_R1						;	//[��Ķ�� ��]
		ar >> m_Scallop_R2						;
		if(nFlag>6)
		{
			ar >> m_Chamfer_C1					;
			ar >> m_Chamfer_C2					;
		}		
		ar >> m_Scallop_JRib_R1					;	//[������ ��Ķ����]
		ar >> m_Scallop_JRib_D1					;	//�����
		ar >> m_Scallop_JRib_R2					;	//�����
		ar >> m_Scallop_JRib_D2					;
		ar >> m_ScallopType						;	//0=�Ϲ�Ÿ��, 1=����Ÿ��
		ar >> m_ScallopBasePoint_Compression	;	//����� ��Ķ�� ���߽� ����
		ar >> m_ScallopBasePoint_Tension		;	//����� ��Ķ�� ���߽� ����
		ar >> m_ScallopRight					;	//����� ��Ķ�� ����, ���� Ÿ��
		ar >> m_Dongbari_A						;	//[���ٸ� �� ��]
		ar >> m_Dongbari_B						;
		ar >> m_Dongbari_C						;
		ar >> m_Dongbari_D						;
		ar >> m_Dongbari_E						;
		ar >> m_Dongbari_F						;
		ar >> m_Dongbari_W						;
//		if(nFlag > 0)
		ar >> m_Dongbari_T						;
		ar >> m_Dongbari_Type					;
		ar >> m_GassetPlate_S1					;	//[����� ������ ��]
		ar >> m_GassetPlate_S2					;
		ar >> m_GassetPlate_S3					;
		ar >> m_GassetPlate_W					;
		ar >> m_GassetPlate_T					;
		ar >> m_GassetPlate_dA					;
		ar >> m_GassetPlate_dB					;
		ar >> m_GassetPlate_dC					;
		ar >> m_GassetPlate_uA					;
		ar >> m_GassetPlate_uB					;
		ar >> m_GassetPlate_uC					;
		ar >> m_FanPlate_uW						;	//[ȯǳ�� ��ħ�� ��]
		ar >> m_FanPlate_uT						;
		ar >> m_FanPlate_uL						;
		ar >> m_FanPlate_uS						;
		ar >> m_FanPlate_dW						;
		ar >> m_FanPlate_dT						;
		ar >> m_FanPlate_dL						;
		ar >> m_FanPlate_dS1					;
		ar >> m_FanPlate_dS2					;
		ar >> m_Ladder_W						;	//[��ħ�� ROUND ��]
		ar >> m_Ladder_T						;
		ar >> m_Ladder_L						;
		if(nFlag>5)
			ar >> m_InOutGateType				;	//[�������Ա� ��]
		ar >> m_InOutGate_Stt_W					;
		ar >> m_InOutGate_Stt_H					;
		ar >> m_InOutGate_Stt_R					;	//���� R
		ar >> m_InOutGate_End_W					;	//[�������Ա� ��]
		ar >> m_InOutGate_End_H					;
		ar >> m_InOutGate_End_R					;	//���� R
		if(nFlag>5)
			ar >> m_ManholeType					;
		ar >> m_Manhole_W						;	//[��Ȧ ��]
		ar >> m_Manhole_H						;
		ar >> m_Manhole_R						;
		if(nFlag>5)
			ar >> m_VentilatorType				;
		ar >> m_Ventilator_POS					;
		ar >> m_Ventilator_H					;	//[ȯ�ⱸ ��]
		ar >> m_Ventilator_D					;
		if(nFlag>2)
		{
			if(nFlag>3)
				ar >> m_Ventilator_sD			;
			ar >> m_Ventilator_uW				;
			ar >> m_Ventilator_uT				;
			ar >> m_Ventilator_uL				;
			ar >> m_Ventilator_dW				;
			ar >> m_Ventilator_dT				;
			ar >> m_Ventilator_dL				;
			ar >> m_Ventilator_dS1				;
			ar >> m_Ventilator_dS2				;
		}
		ar >> m_StudHori_D						;	//[���ܿ����� Ⱦ���� ��ġ]
		ar >> m_StudHori_H						;
		ar >> m_StudHori_E						;
		ar >> m_StudHori_Type					;	//0=���ܿ�����, 1=������ ��Ŀ
		ar >> m_StudHori_Qty					;	//���ܿ����� ����
		ar >> m_StudJong_MinLenFactor			;	//[���ܿ����� ������ ��ġ]
		ar >> m_StudJong_MinDis					;	//(125)
		ar >> m_StudJong_MidLenFactor			;	//�߰����� ����(5000)
		ar >> m_StudJong_MidDis					;	//�߰� ����(250)
		ar >> m_StudJong_MaxLen					;	//�ִ� ����
		ar >> m_StudJong_MaxDis					;	//�ִ� ����
		ar >> m_StudJong_GirderStart			;	//�Ŵ� �������� ����
		ar >> m_StudJong_FactJoin				;
		ar >> m_StudJong_SpliceCenter			;
		long n = 0;
		for(n=0; n < STUD_HORI_SU; n++)
			ar >> m_StudHori_Dis[n];			//���̾����� ���� �����ϴ� ����
		for(n=0; n < STUD_JONG_SU; n++)
		{
			float dTemp;
			if(nFlag>10)
				ar >> m_StudJong_BaseDis[n]	;
			else
			{
				ar >> dTemp;
				m_StudJong_BaseDis[n] = (double)dTemp;
			}
			ar >> m_StudJong_BaseTerm[n];
			ar >> m_StudJong_Studsu[n]	;
			ar >> m_StudJong_StudDis[n]	;
		}
		if(nFlag > 1) 
		{
			ar >> m_nTonIndex;				//�� ������
			for(n=0; n<TON_INDEX; n++)
			{
				ar >> m_LiftLug_I[n]			;	//����
				ar >> m_LiftLug_A[n]			;	//���� ���� 
				ar >> m_LiftLug_B[n]			;	//������ ���� 
				ar >> m_LiftLug_C[n]			;	//������ ���� 
				ar >> m_LiftLug_D[n]			;	//���� �߽ɱ��� ���� 
				ar >> m_LiftLug_d[n]			;	//���� ����
				ar >> m_LiftLug_R[n]			;	//��� �ձٺκ� ������
				ar >> m_LiftLug_T[n]			;	//�β�
				ar >> m_LiftLug_iR[n]			;	//������ ������
				ar >> m_LiftLug_sA[n]			;	//���� �������� �β� 
				ar >> m_LiftLug_sB[n]			;	//������ ���������� �β� 
				ar >> m_LiftLug_lT[n]			;	//������ ���� �β�
				ar >> m_LiftLug_rT[n]			;	//������ ������ ����
				ar >> m_LiftLug_lW[n]			;	//�������� ���� ����
				ar >> m_LiftLug_rW[n]			;	//�������� ������ ����
				ar >> m_LiftLug_bAddPlate[n]	;	//������
				ar >> m_LiftLug_bSidePlate[n]	;	//������
			}
		}
		if(nFlag>7)
		{
			ar >> m_SlabAnchor_A;
			ar >> m_SlabAnchor_B;
			ar >> m_SlabAnchor_C;
			ar >> m_SlabAnchor_T;
			ar >> m_SlabAnchorType;
		}
		// ���Ա� ������ ��ġ����
		if(nFlag>8)	
		{
			ar >> m_nFacePlateUse_Stt;
			ar >> m_nFacePlateUse_End;
		}

		// TU - Girder �߰� ���� �ۼ�
		if(nFlag>10)
		{
			ar >> m_StudHoriSQ_H;
			ar >> m_StudHoriSQ_W;
			ar >> m_StudHoriSQ_L;
		}
	}
}

void CPlateSangSeDo::SerializeForOldVersion(CArchive& ar)
{
	CString vString;
	BYTE vbyte;
	long vlong;
	double vdouble;
	short vshort;
	if(!ar.IsStoring())
	{
		long m_SCSu1, m_SCSu2;
		BYTE bSangse, bFlag = 0; // Sonar Qube Major Issue ó��.
		ar >> m_InOutGate_Stt_W;m_InOutGate_End_W = m_InOutGate_Stt_W;//m_Gate_W;
		ar >> m_InOutGate_Stt_H;m_InOutGate_End_H = m_InOutGate_Stt_H;//m_Gate_H;
		ar >> vString; //m_Gate_BoltSize;
		ar >> vString;//m_Gate_BoltMaterial;
		ar >> vdouble;//m_Gate_BoltLength;
		ar >> m_JRib_A; if(m_JRib_A > 0) m_JRib_sUp = m_JRib_sLow = 1;//m_JRib_A;	
		ar >> m_StudHori_D;//m_ShareConnect_D;
		ar >> m_StudHori_H;//m_ShareConnect_H;	
		ar >> m_StudHori_E;//m_ShareConnect_E;	
		ar >> vshort; m_StudHori_Qty = (long)vshort;//m_ScQty;
		ar >> bSangse;
		ar >> vbyte;
		long m = 0;
		for(m = 0;m < 20;m++) 
			ar >> m_StudHori_Dis[m];//m_ScDis[m];
		for(m=0;m < 200;m++) 
			ar >> vdouble;//m_ShareConnect_Dis[m];
		for(m=0;m < 200;m++) 
			ar >> vlong;//m_ShareConnect_Su[m];
		ar >> m_Scallop_R1;//m_Skarab_R1;	
		ar >> m_Scallop_R2;//m_Skarab_R2;	
		ar >> vdouble;//m_Manhole_R;		
		ar >> vdouble;//m_dW_SolePlate;		
		ar >> vdouble;//m_dH_SolePlate;	
		ar >> vdouble;//m_SolePlate_T1;	
		ar >> vdouble;//m_SolePlate_T2;
		ar >> m_HStiff_VStiff_B;//m_HStiff_B;
		ar >> m_HStiff_VStiff_D;//m_HStiff_D;
		ar >> m_WaterOut_W;//m_WaterOut_W;
		ar >> m_WaterOut_T;//m_WaterOut_T;
		ar >> m_WaterOut_L;//m_WaterOut_L;
		ar >> m_WaterOut_M;//m_WaterOut_M;
		ar >> m_WaterOut_R;//m_WaterOut_R;
		ar >> m_Taper_W;//m_Taper_W;
		ar >> m_Taper_H;//m_Taper_H;
		ar >> m_HStiff_Splice_B;//m_HStiffS_B;
		if( bSangse )
		{
			ar >> vdouble;//m_HStiffWidth;
			ar >> vdouble;//m_HStiffThick;
			ar >> vlong;//m_bGirderStart;
			ar >> vbyte;//m_ScType;
			ar >> bFlag;
			ar >> vbyte;
			ar >> vbyte;
			ar >> m_InOutGate_Stt_R;m_InOutGate_End_R = m_InOutGate_Stt_R;//m_R;
			ar >> m_StudJong_MinLenFactor;//m_MinLen;
			ar >> m_StudJong_MidLenFactor;//m_MidLen;
			ar >> m_StudJong_MidDis;//m_MidDis;
			ar >> m_Manhole_W;//m_Manhole_W;
			ar >> m_Manhole_H;//m_Manhole_H;
			ar >> m_Ventilator_H;//m_Ventilator_H;
			ar >> m_Ventilator_D;//m_Ventilator_D;
			ar >> m_SCSu1;
			ar >> m_SCSu2;

			m_Ventilator_sD = 125;
		}
		if( bFlag >= 1 )
		{
			for(m=0;m<200;m++) // 200
			{
				ar >> vlong;//m_SC_Idx[m];
				ar >> vdouble;//m_SC_Dis[m];
			}
			for(m=200;m < m_SCSu1;m++)
			{
				ar >> vdouble;//m_ShareConnect_Dis[m];
				ar >> vlong;//m_ShareConnect_Su[m];
			}
			for(m=200;m < m_SCSu2;m++)
			{
				ar >> vlong;//m_SC_Idx[m];
				ar >> vdouble;//m_SC_Dis[m];
			}
		}
		if( bFlag == 2 )
		{
			ar >> m_WaterOut_D;//m_WaterOut_D;if( m_WaterOut_D <= 0 ) m_WaterOut_D = 20;
			// REV_HA : 2000.7.3
			// 10->9 (W�� ���������� ����)
			ar >> m_WaterOut_L2;//m_WaterOut_L2;
			ar >> m_WaterOut_d1;//m_WaterOut_d1;
			// m_WaterOut_d1 == 0 �̸� ��������� ��ġ�Ұ�
			//if(m_WaterOut_d1 == 0)
			//	m_WaterOut_d1 = 50;
			ar >> m_WaterOut_d2;//m_WaterOut_d2;
			//
			for(long m=0;m<7;m++)
				ar >> vdouble;
		}
		// *************
		m_Ventilator_uT	=  10;
		m_Ventilator_uW	=  200;
		m_Ventilator_uL	=  400;

	}
}

CPlateSangSeDo& CPlateSangSeDo::operator=(const CPlateSangSeDo& Obj)
{
	if( this == &Obj ) return *this;
	m_WaterOut_W					= Obj.m_WaterOut_W					;	// ���������
	m_WaterOut_T					= Obj.m_WaterOut_T					;
	m_WaterOut_L					= Obj.m_WaterOut_L					;
	m_WaterOut_L2					= Obj.m_WaterOut_L2					;
	m_WaterOut_d1					= Obj.m_WaterOut_d1					;
	m_WaterOut_d2					= Obj.m_WaterOut_d2					;
	m_WaterOut_M					= Obj.m_WaterOut_M					;
	m_WaterOut_R					= Obj.m_WaterOut_R					;
	m_WaterOut_D					= Obj.m_WaterOut_D					;
	m_JRib_A						= Obj.m_JRib_A						;	//[������]
	m_JRib_sUp						= Obj.m_JRib_sUp					;	//������ ��� ��������
	m_JRib_sLow						= Obj.m_JRib_sLow					;	//������ �Ϻ� ��������
	m_HStiff_VStiff_B				= Obj.m_HStiff_VStiff_B				;	//[���򺸰��� ����]
	m_HStiff_VStiff_D				= Obj.m_HStiff_VStiff_D				;
	m_HStiff_Splice_B				= Obj.m_HStiff_Splice_B				;
	m_Taper_W						= Obj.m_Taper_W						;	//[�ܸ� ��ȭ�� TAPER ó�� ��]
	m_Taper_H						= Obj.m_Taper_H						;
	m_Scallop_R1					= Obj.m_Scallop_R1					;	//[��Ķ�� ��]
	m_Scallop_R2					= Obj.m_Scallop_R2					;
	m_Chamfer_C1					= Obj.m_Chamfer_C1					;	//[��Ķ�� ��]
	m_Chamfer_C2					= Obj.m_Chamfer_C2					;	
	m_Scallop_JRib_R1				= Obj.m_Scallop_JRib_R1				;	//[������ ��Ķ����]
	m_Scallop_JRib_D1				= Obj.m_Scallop_JRib_D1				;	//�����
	m_Scallop_JRib_R2				= Obj.m_Scallop_JRib_R2				;	//�����
	m_Scallop_JRib_D2				= Obj.m_Scallop_JRib_D2				;
	m_ScallopType					= Obj.m_ScallopType					;	//0=�Ϲ�Ÿ��, 1=����Ÿ��
	m_ScallopBasePoint_Compression	= Obj.m_ScallopBasePoint_Compression;	//����� ��Ķ�� ���߽� ����
	m_ScallopBasePoint_Tension		= Obj.m_ScallopBasePoint_Tension	;	//����� ��Ķ�� ���߽� ����
	m_ScallopRight					= Obj.m_ScallopRight				;	//����� ��Ķ�� ����, ���� Ÿ��
	m_Dongbari_A					= Obj.m_Dongbari_A					;	//[���ٸ� �� ��]
	m_Dongbari_B					= Obj.m_Dongbari_B					;
	m_Dongbari_C					= Obj.m_Dongbari_C					;
	m_Dongbari_D					= Obj.m_Dongbari_D					;
	m_Dongbari_E					= Obj.m_Dongbari_E					;
	m_Dongbari_F					= Obj.m_Dongbari_F					;
	m_Dongbari_W					= Obj.m_Dongbari_W					;
	m_Dongbari_T					= Obj.m_Dongbari_T					;
	m_Dongbari_Type					= Obj.m_Dongbari_Type				;
	m_GassetPlate_S1				= Obj.m_GassetPlate_S1				;	//[����� ������ ��]
	m_GassetPlate_S2				= Obj.m_GassetPlate_S2				;
	m_GassetPlate_S3				= Obj.m_GassetPlate_S3				;
	m_GassetPlate_W					= Obj.m_GassetPlate_W				;
	m_GassetPlate_T					= Obj.m_GassetPlate_T				;
	m_GassetPlate_dA				= Obj.m_GassetPlate_dA				;
	m_GassetPlate_dB				= Obj.m_GassetPlate_dB				;
	m_GassetPlate_dC				= Obj.m_GassetPlate_dC				;
	m_GassetPlate_uA				= Obj.m_GassetPlate_uA				;
	m_GassetPlate_uB				= Obj.m_GassetPlate_uB				;
	m_GassetPlate_uC				= Obj.m_GassetPlate_uC				;
	m_FanPlate_uW					= Obj.m_FanPlate_uW					;	//[ȯǳ�� ��ħ�� ��]
	m_FanPlate_uT					= Obj.m_FanPlate_uT					;
	m_FanPlate_uL					= Obj.m_FanPlate_uL					;
	m_FanPlate_uS					= Obj.m_FanPlate_uS					;
	m_FanPlate_dW					= Obj.m_FanPlate_dW					;
	m_FanPlate_dT					= Obj.m_FanPlate_dT					;
	m_FanPlate_dL					= Obj.m_FanPlate_dL					;
	m_FanPlate_dS1					= Obj.m_FanPlate_dS1				;
	m_FanPlate_dS2					= Obj.m_FanPlate_dS2				;
	m_Ladder_W						= Obj.m_Ladder_W					;	//[��ħ�� ROUND ��]
	m_Ladder_T						= Obj.m_Ladder_T					;
	m_Ladder_L						= Obj.m_Ladder_L					;
	m_InOutGate_Stt_W				= Obj.m_InOutGate_Stt_W				;	//[�������Ա� ��]
	m_InOutGate_Stt_H				= Obj.m_InOutGate_Stt_H				;
	m_InOutGate_Stt_R				= Obj.m_InOutGate_Stt_R				;	//���� R
	m_InOutGate_End_W				= Obj.m_InOutGate_End_W				;	//[�������Ա� ��]
	m_InOutGate_End_H				= Obj.m_InOutGate_End_H				;
	m_InOutGate_End_R				= Obj.m_InOutGate_End_R				;	//���� R
	m_Manhole_W						= Obj.m_Manhole_W					;	//[��Ȧ ��]
	m_Manhole_H						= Obj.m_Manhole_H					;
	m_Manhole_R						= Obj.m_Manhole_R					;
	m_Ventilator_H					= Obj.m_Ventilator_H				;	//[ȯ�ⱸ ��]
	m_Ventilator_D					= Obj.m_Ventilator_D				;
	m_StudHori_D					= Obj.m_StudHori_D					;	//[���ܿ����� Ⱦ���� ��ġ]
	m_StudHori_H					= Obj.m_StudHori_H					;
	m_StudHori_E					= Obj.m_StudHori_E					;
	m_StudHori_Type					= Obj.m_StudHori_Type				;	//0=���ܿ�����, 1=������ ��Ŀ
	m_StudHori_Qty					= Obj.m_StudHori_Qty				;	//���ܿ����� ����
	m_StudJong_MinLenFactor			= Obj.m_StudJong_MinLenFactor		;	//[���ܿ����� ������ ��ġ]
	m_StudJong_MinDis				= Obj.m_StudJong_MinDis				;	//(125)
	m_StudJong_MidLenFactor			= Obj.m_StudJong_MidLenFactor		;	//�߰����� ����(5000)
	m_StudJong_MidDis				= Obj.m_StudJong_MidDis				;	//�߰� ����(250)
	m_StudJong_MaxLen				= Obj.m_StudJong_MaxLen				;	//�ִ� ����
	m_StudJong_MaxDis				= Obj.m_StudJong_MaxDis				;	//�ִ� ����
	m_StudJong_GirderStart			= Obj.m_StudJong_GirderStart		;	//�Ŵ� �������� ����
	m_StudJong_FactJoin				= Obj.m_StudJong_FactJoin			;
	m_StudJong_SpliceCenter			= Obj.m_StudJong_SpliceCenter		;
	long n = 0;
	for(n=0; n < STUD_HORI_SU; n++)
		m_StudHori_Dis[n]			=Obj.m_StudHori_Dis[n];			//���̾����� ���� �����ϴ� ����
	for(n=0; n < STUD_JONG_SU; n++)
	{
		m_StudJong_BaseTerm[n] = Obj.m_StudJong_BaseTerm[n];
		m_StudJong_BaseDis[n]  = Obj.m_StudJong_BaseDis[n];
		m_StudJong_Studsu[n]   = Obj.m_StudJong_Studsu[n];
		m_StudJong_StudDis[n]  = Obj.m_StudJong_StudDis[n];
	}

	m_nTonIndex = Obj.m_nTonIndex;	
	for(n=0; n<TON_INDEX; n++)
	{
		m_LiftLug_I[n]  = Obj.m_LiftLug_I[n]					;
		m_LiftLug_A[n]  = Obj.m_LiftLug_A[n]					;
		m_LiftLug_B[n]  = Obj.m_LiftLug_B[n]					;
		m_LiftLug_C[n]  = Obj.m_LiftLug_C[n]					;
		m_LiftLug_D[n]  = Obj.m_LiftLug_D[n]					;
		m_LiftLug_d[n]  = Obj.m_LiftLug_d[n]					;
		m_LiftLug_R[n]  = Obj.m_LiftLug_R[n]					;
		m_LiftLug_T[n]  = Obj.m_LiftLug_T[n]					;
		m_LiftLug_iR[n] = Obj.m_LiftLug_iR[n]					;
		m_LiftLug_sA[n] = Obj.m_LiftLug_sA[n]					;
		m_LiftLug_sB[n] = Obj.m_LiftLug_sB[n]					;
		m_LiftLug_lT[n] = Obj.m_LiftLug_lT[n]					;
		m_LiftLug_rT[n] = Obj.m_LiftLug_rT[n]					;
		m_LiftLug_lW[n] = Obj.m_LiftLug_lW[n]					;
		m_LiftLug_rW[n] = Obj.m_LiftLug_rW[n]					;
		m_LiftLug_bAddPlate[n]  = Obj.m_LiftLug_bAddPlate[n]	;
		m_LiftLug_bSidePlate[n] = Obj.m_LiftLug_bSidePlate[n]	;
	}
	m_StudHoriSQ_H	= Obj.m_StudHoriSQ_H;
	m_StudHoriSQ_W	= Obj.m_StudHoriSQ_W;
	m_StudHoriSQ_L	= Obj.m_StudHoriSQ_L;

	return *this;
}

//<summary>
//
//</summary>
//<param name="dArr"></param>
void CPlateSangSeDo::SetFillAssemVariable(CDoubleArray& dArr)
{
	dArr.Add(m_WaterOut_W);
	dArr.Add(m_WaterOut_T);
	dArr.Add(m_WaterOut_L);
	dArr.Add(m_WaterOut_L2);
	dArr.Add(m_WaterOut_d1);
	dArr.Add(m_WaterOut_M);
	dArr.Add(m_WaterOut_W);
	dArr.Add(m_WaterOut_R);
	dArr.Add(m_WaterOut_D);
}


//<summary>
//
//</summary>
//<param name="Obj"></param>
//<return></return>
BOOL CPlateSangSeDo::IsSameWaterOut(const CPlateSangSeDo& Obj)
{
	if(m_WaterOut_W  != Obj.m_WaterOut_W)	return FALSE;
	if(m_WaterOut_T  != Obj.m_WaterOut_T)	return FALSE;
	if(m_WaterOut_L  != Obj.m_WaterOut_L)	return FALSE;
	if(m_WaterOut_L2 != Obj.m_WaterOut_L2)	return FALSE;
	if(m_WaterOut_d1 != Obj.m_WaterOut_d1)	return FALSE;
	if(m_WaterOut_M  != Obj.m_WaterOut_M)	return FALSE;
	if(m_WaterOut_W  != Obj.m_WaterOut_W)	return FALSE;
	if(m_WaterOut_R  != Obj.m_WaterOut_R)	return FALSE;
	if(m_WaterOut_D  != Obj.m_WaterOut_D)	return FALSE;

	return TRUE;
}

//<summary>
//������� �����Լ� ����
//</summary>
//<param name="*pObj"></param>
void CPlateSangSeDo::CreatePieceObjLiftLug(CPlatePieceObject *pObj)
{
	if(pObj->m_nCol==0) CreatePieceObjLiftLugMainPlate(pObj);
	if(pObj->m_nCol==1) CreatePieceObjLiftLugAddPlate(pObj);
	if(pObj->m_nCol==2) CreatePieceObjLiftLugSidePlate(pObj);
}

//<summary>
//���������
// /   (1) ___ (2)
///    R   | | 
//(0)   |       |      (3)
//   |      __      |            H
//|      d |__|        |      D
//|        (7)         |   C  
//|__________*_________|
//(6)   A  (5)    B    (4)	
//(0,0)�� �ظ� �߾�
//</summary>
//<param name="*pObj"></param>
void CPlateSangSeDo::CreatePieceObjLiftLugMainPlate(CPlatePieceObject *pObj)
{
	pObj->SetLayer(HCAD_STLC);

	long nArrIndex = pObj->m_nRow;

	double A = m_LiftLug_A[nArrIndex];
	double B = m_LiftLug_B[nArrIndex];
	double C = m_LiftLug_C[nArrIndex];
	double D = m_LiftLug_D[nArrIndex];
	double d = m_LiftLug_d[nArrIndex];
	double R = m_LiftLug_R[nArrIndex];

	CDPoint xy, cen, left, right;
	
	xy.x = -A;	xy.y = C;
	pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(0)

	cen.x = 0;	cen.y = D; 
	GetTangentCircle(xy, cen, R, left, right); 
	pObj->AddElement(left, P_TYPE_LINE);				//(1)

	xy.x = B;	xy.y = C;
	GetTangentCircle(xy, cen, R, left, right);
	pObj->AddElement(right, R, -1, NULL, P_TYPE_ARC);	//(2)

	pObj->AddElement(xy, P_TYPE_LINE);	//(3)

	xy.x = B;	xy.y = 0;
	pObj->AddElement(xy, P_TYPE_LINE);	//(4)

	xy.x = 0;	xy.y = 0;
	pObj->AddElement(xy, P_TYPE_LINE);	//(5)

	xy.x = -A;	xy.y = 0;
	pObj->AddElement(xy, P_TYPE_LINE);	//(6)

	CPlatePieceObject *pChild = new CPlatePieceObject(pObj->GetGirder());

	pChild->AddCircle(cen, d/2);			//(7)

	xy.y = R;
	pChild->AddElement(xy, P_TYPE_DIMPOINT);	//(8) �ݰ�

	xy.x = d;
	pChild->AddElement(xy, P_TYPE_DIMPOINT);	//(9) ����

	pChild->SetSolid(FALSE);
	pObj->AddChildObj(pChild);

	pObj->SetFaceRange(0,3);
	pObj->SetFaceRange(2,4);
	pObj->SetFaceRange(4,6);
	pObj->SetFaceRange(6,7);
}
//<summary>
//���������
//             _
//           |   |
//         |   _    | (1)
//         |  |_|   |
//         |    (0) |
//		     | * |
//</summary>
//<param name="*pObj"></param>
void CPlateSangSeDo::CreatePieceObjLiftLugAddPlate(CPlatePieceObject *pObj)
{
	pObj->SetLayer(HCAD_STLC);
	long nArrIndex = pObj->m_nRow;

	double D  = m_LiftLug_D[nArrIndex];
	double iR = m_LiftLug_iR[nArrIndex];
	double d  = m_LiftLug_d[nArrIndex];

	CDPoint xy;

	xy.x = 0;	xy.y = 0;
	pObj->AddCircle(xy, iR);		//(1)
	CPlatePieceObject *pChild = new CPlatePieceObject(pObj->GetGirder());
	pChild->AddCircle(xy, d/2);		//(0)
				xy.y = D;
	pChild->AddElement(xy, P_TYPE_DIMPOINT);	//(2)
				xy.y = iR;
	pChild->AddElement(xy, P_TYPE_DIMPOINT);	//(3)

	pChild->SetSolid(FALSE);
	pObj->AddChildObj(pChild);

//	pObj->SetFaceRange(0,1);
}
//<summary>
//���������
// (0) _________________ (1)
//    |                 |  
//    |                 | C 
//    |                 | 
//    |________*________|    *(0,0)
// (3)    lW       rW    (2)
//</summary>
//<param name="*pObj"></param>
void CPlateSangSeDo::CreatePieceObjLiftLugSidePlate(CPlatePieceObject *pObj)
{
	pObj->SetLayer(HCAD_STLC);
	long nArrIndex = pObj->m_nRow;

	double lW = m_LiftLug_lW[nArrIndex];
	double rW = m_LiftLug_rW[nArrIndex];
	double C  = m_LiftLug_C[nArrIndex];

	CDPoint xy;

	xy.x = -lW;	xy.y = C;
	pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(0)

	xy.x = rW;	xy.y = C;
	pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(1)

	xy.x = rW;	xy.y = 0;
	pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(2)

	xy.x = -lW;	xy.y = 0;
	pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(3)

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,0);
}
//<summary>
//��� (�𵨸� �۾�)
//</summary>
//<param name="*pObj"></param>
//<param name="sta"></param>
//<param name="staDir"></param>
//<param name="bLeft"></param>
void CPlateSangSeDo::CreatePieceObjLiftLugMainPlate3D(CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft)
{
	CPlateGirderApp *pGir = m_pGir;
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	double  dLevel  = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0,1), TRUE);	//datum level ���ظ�
	double  tFlange = pGir->GetThickFactByStation(sta, G_F_U);
	CVector vNormal = pGir->GetVectorNormalOnFlange(sta, 0);

	CVector	vGir    = pGir->GetXyGirderDis(sta);
	CVector vGirDir = pGir->GetXyGirderDis(staDir);
	CVector vXYDir  = (vGirDir - vGir).Unit();	//�¿�� ��ġ�� ���� ���� ����

	CreatePieceObjLiftLugMainPlate(pObj);
	CPlatePiecePointArray& rVentLiftLugMainPlate = pObj->GetPiecePointArray();
	CPlatePiecePointArray& rVentLiftLugMainPlateHole = pObj->GetChild(0)->GetPiecePointArray();

	CVector	baseS, baseT, vX(1, 0, 0), vY(0, 1, 0);
	baseS = CVector(0, 0, 0);
	baseT = vGir;
	baseT.z = dLevel + tFlange;

	rVentLiftLugMainPlate.Transform(baseS, baseT, vY, vNormal, vX, vXYDir);
	rVentLiftLugMainPlateHole.Transform(baseS, baseT, vY, vNormal, vX, vXYDir);
}

//<summary>
//�����ħ�� �𵨸�
//</summary>
//<param name="*pObj"></param>
//<param name="sta"></param>
//<param name="staDir"></param>
//<param name="bLeft"></param>
//<param name="bFront"></param>
void CPlateSangSeDo::CreatePieceObjLiftLugAddPlate3D(CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft, BOOL bFront)
{
	CPlateGirderApp *pGir = m_pGir;
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	BOOL bSttPos = sta < staDir;

	double  dLevel  = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0,1), TRUE);	//datum level ���ظ�
	double  tFlange = pGir->GetThickFactByStation(sta, G_F_U);
	CVector vNormal = pGir->GetVectorNormalOnFlange(sta, 0);

	pObj->SetLayer(HCAD_STLC);
	long nArrIndex = pObj->m_nRow;

	double distD = m_LiftLug_D[nArrIndex] ;
	double tMain = m_LiftLug_T[nArrIndex] ;	//��ħ�� �β� - ������

	CVector	vGir    = pGir->GetXyGirderDis(sta);
	CVector vGirDir = pGir->GetXyGirderDis(staDir);
	CVector vXYDir  = (vGirDir - vGir).Unit();	//�¿�� ��ġ�� ���� ���� ����

	CreatePieceObjLiftLugAddPlate(pObj);
	CPlatePiecePointArray& rVentLiftLugAddPlate = pObj->GetPiecePointArray();
	CPlatePiecePointArray& rVentLiftLugAddPlateHole = pObj->GetChild(0)->GetPiecePointArray();

	CVector	baseS, baseT, vX(1, 0, 0), vY(0, 1, 0);
	baseS = CVector(0, -distD, (bFront ? 0 : tMain) * (bLeft ^ bSttPos ? -1 : 1));
	baseT = vGir;
	baseT.z = dLevel + tFlange;

	rVentLiftLugAddPlate.Transform(baseS, baseT, vY, vNormal, vX, vXYDir);
	rVentLiftLugAddPlateHole.Transform(baseS, baseT, vY, vNormal, vX, vXYDir);
}

//<summary>
//
//</summary>
//<param name="*pObj"></param>
//<param name="sta"></param>
//<param name="staDir"></param>
//<param name="bLeft"></param>
//<param name="bFront"></param>
void CPlateSangSeDo::CreatePieceObjLiftLugSidePlate3D(CPlatePieceObject *pObj, double sta, double staDir, BOOL bLeft, BOOL bFront)
{
	CPlateGirderApp *pGir = m_pGir;
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	BOOL bSttPos = sta < staDir;

 	double  dLevel  = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0,1), TRUE);	//datum level ���ظ�
	double  tFlange = pGir->GetThickFactByStation(sta, G_F_U);
	CVector vNormal = pGir->GetVectorNormalOnFlange(sta, 0);

	pObj->SetLayer(HCAD_STLC);
	long nArrIndex = pObj->m_nRow;

	double distA = m_LiftLug_A[nArrIndex] ;
	double distB = m_LiftLug_B[nArrIndex] ;
	double tMain = m_LiftLug_T[nArrIndex] ;	//��ħ�� �β� - ������

	CVector	vGir    = pGir->GetXyGirderDis(sta);
	CVector vGirDir = pGir->GetXyGirderDis(staDir);
	CVector vXYDir  = (vGirDir - vGir).Unit();	//�¿�� ��ġ�� ���� ���� ����

	CreatePieceObjLiftLugSidePlate(pObj);
	CPlatePiecePointArray& rVentLiftLugSidePlate = pObj->GetPiecePointArray();

	CVector	baseS, baseT, vY(0, 1, 0), vZ(0, 0, 1);
	baseS = CVector(tMain/2 * (bLeft ^ bSttPos ? 1 : -1), 0, bFront ? distA : -distB);
	baseT = vGir;
	baseT.z = dLevel + tFlange;

	rVentLiftLugSidePlate.Transform(baseS, baseT, vY, vNormal, vZ, vXYDir);
}

//<summary>
//���������� ���ٸ� �� ����
//</summary>
//<param name="*pObj"></param>
void CPlateSangSeDo::CreatePieceObjStagingLinkGeneral(CPlatePieceObject *pObj)
{
	CDPoint xy;

	//���� ����
	double dSpacing = m_Dongbari_F;	//�������� ��ο��� �Ÿ���
	double dHeight  = m_Dongbari_E;	//���� �߽ɱ��� ����
	double dWidth   = m_Dongbari_W;	//��
	//Ȧ ����
	double dRadius         = m_Dongbari_D;	//Ȧ ���� �ݰ�
	double dHoleStraight   = m_Dongbari_B;	//Ȧ �������� ����
	double dDistHoleCenter = m_Dongbari_A;	//�����Ǹ����� ���� Ȧ�߽��� ��ġ�� �Ÿ�
	//����� ����
	double dChamfer = m_Dongbari_C;	//����� ���� ����

	xy = CDPoint(0,0);

	xy.x = 0;
	xy.y = -dSpacing;
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.y -= dHeight*2;
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.x = -dWidth;
	pObj->AddElement(xy, P_TYPE_CHAMFER, -1, NULL, dChamfer);

	xy.y += dHeight*2;
	pObj->AddElement(xy, P_TYPE_CHAMFER, -1, NULL, dChamfer);

	CPlatePieceObject *pChild = new CPlatePieceObject(pObj->GetGirder());
	xy = CDPoint(0,0);

	//������
	xy.x = -(dDistHoleCenter+(dRadius/2));
	xy.y = -(dHeight-(dHoleStraight/2) + dSpacing);
	pChild->AddElement(xy, P_TYPE_LINE);

	xy.x += dRadius/2;
	xy.y += dRadius/2;
	pChild->AddElement(xy, dRadius/2);

	xy.x += dRadius/2;
	xy.y -= dRadius/2;
	pChild->AddElement(xy, dRadius/2);

	xy.y -= dHoleStraight;
	pChild->AddElement(xy, P_TYPE_LINE);

	xy.x -= dRadius/2;
	xy.y -= dRadius/2;
	pChild->AddElement(xy, dRadius/2);

	xy.x -= dRadius/2;
	xy.y += dRadius/2;
	pChild->AddElement(xy, dRadius/2);

	pChild->SetSolid(FALSE);
	pObj->AddChildObj(pChild);
	pObj->RegenVertex();

}

//<summary>
//���������� ���ٸ� �� �÷��� ������
//</summary>
//<param name="*pObj"></param>
void CPlateSangSeDo::CreatePieceObjStagingLinkFlangeBond(CPlatePieceObject *pObj)
{
	CDPoint xy;

	double dHeight   = m_Dongbari_B;	//����
	double dWidth    = m_Dongbari_W;	//��
	double dScallopR = m_Dongbari_A;	//��Ķ�� �ݰ�
	double dChamfer  = m_Dongbari_C;	//����� ���� ����

	xy = CDPoint(0,0);

	xy.x = 0;
	xy.y = 0;
	pObj->AddElement(xy, P_TYPE_SCALLOP, -1, NULL, dScallopR);

	xy.y -= dHeight;
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.x -= dWidth;
	pObj->AddElement(xy, P_TYPE_CHAMFER, -1, NULL, dChamfer);

	xy.y += dHeight;
	pObj->AddElement(xy, P_TYPE_LINE);

	//�Ϲ����� ���� Ÿ��->������ 0
	CPlatePieceObject *pChild = new CPlatePieceObject(pObj->GetGirder());
	pObj->AddChildObj(pChild);
	pObj->RegenVertex();
}

//<summary>
//���������� ���ٸ� �� �÷��� ������ �̵�
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
//<param name="bLeft"></param>
//<param name="bUpper"></param>
void CPlateSangSeDo::CreatePieceObjStagingLink3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bUpper)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	double dSta        = pBx->GetStation();
	double dFlangeT    = pGir->GetThickJewon(G_W, pBx);		//������ �β�
	double dGirW       = bLeft ? -dFlangeT : dFlangeT;
	double dDatumEL    = pGir->GetElevationFlangeCamber(dSta, dGirW, CDPoint(0,1), bUpper);	//datum level ���ظ�

	if(m_Dongbari_Type == 0)
		CreatePieceObjStagingLinkGeneral(pObj);
	else
		CreatePieceObjStagingLinkFlangeBond(pObj);

	CPlatePiecePointArray& rStagingLinkPlate = pObj->GetPiecePointArray();
	CPlatePiecePointArray& rStagingLinkOpen  = pObj->GetChild(0)->GetPiecePointArray();

	CVector vGir, vDirX(0, 0, 1), vDirY;
	vGir  = pGir->GetLine()->GetAngleAzimuth(dSta);
	vDirY = vGir.RotateInv90();

	CVector vBaseS, vBaseT;
	vBaseS   = CVector(0, 0, 0);
	vBaseT   = pGir->GetXyGirderDis(dSta, dGirW);
	vBaseT.z = dDatumEL;

	if(!bUpper)
		vDirX *= -1;
	if(!bLeft)
		vDirY *= -1;

	rStagingLinkPlate.Transform(vBaseS, vBaseT, CDPoint(0,1), vDirX, CDPoint(1,0), vDirY);
	rStagingLinkOpen.Transform( vBaseS, vBaseT, CDPoint(0,1), vDirX, CDPoint(1,0), vDirY);
}
//<summary>
//���Ա� �÷���Ʈ
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	CDPoint xy(0, 0);  /*80 �� 100�� ���� ����������. ���� �����߰�*/
	double R = m_InOutGate_Stt_R+80;
	double H = m_InOutGate_Stt_H+100;
	double W = m_InOutGate_Stt_W+100;

	xy.x = W/2;
	xy.y = H/2;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,R);

	xy.y -= H;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,R);
	
	xy.x -= W;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,R);
	
	xy.y += H;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,R);

	pObj->RegenVertex();
}
//<summary>
//���Ա� �÷���Ʈ��3D: �����ο� �����ο� ���� ���Թ��� ��ġ�� ���� mirror���¸�  �̷��.
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CDiaphragm *pDia = pBx->GetDiaphragm();
	BOOL bStart = FALSE;
	bStart      = pBx->IsAbutStt();
	// Create 2D
	CreatePieceObjAbutDoorPlate(pObj, pBx);
	// ��ǥ ��ȯ
	CPlatePiecePointArray& va = pObj->GetPiecePointArray();
	CVector Base2D(0,0,0);
	CVector Base3D(0,0,0);
	double sta  = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	CDPoint vXYDir = vGir.Rotate(vAng);
	Base3D   = pGir->GetXyGirderDis(sta, -pGir->m_dWidth/2+pDia->m_G_DpDp, vAng);

	Base3D  += (bStart?1:-1)*vXYDir.RotateInv90() * pDia->m_dT_DpDp/2;  
	Base3D.z = pGir->GetElevationFlangeCamber(sta, -pGir->m_dWidth/2+pDia->m_G_DpDp, vAng, FALSE) + pDia->m_F_DpDp + m_InOutGate_Stt_H/2;

	va.TransformForVert(Base2D,Base3D,vXYDir);
	*/
}	
//<summary>
//���Ա� L����2D size: 100*100*10
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorL(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	CVectorArray va;

	double L = 100;
	double W = 100;
	va.Add(CVector(-W/2, L, 0));
	va.Add(CVector( W/2, L, 0));
	va.Add(CVector(-W/2, 0, 0));
	va.Add(CVector( W/2, 0, 0));
	va.Add(CVector(-W/2, 0, L));
	va.Add(CVector( W/2, 0, L));

	pObj->GetPiecePointArray() = va;

}

//<summary>
//���Ա� L����3D: �����ο� �����ο� ���� ���Թ��� ��ġ�� ���� mirror���¸�  �̷��.	
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
//<param name="nRow"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorL3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CDiaphragm *pDia = pBx->GetDiaphragm();
	BOOL bStart = FALSE;
	bStart      = pBx->IsAbutStt();
	
	double HGap = 120;
	double VGap = 110;
	// Create 2D
	CreatePieceObjAbutDoorL(pObj, pBx);
	double L = 100;
	double T = 10;
	// ��ǥ ��ȯ
	CPlatePiecePointArray& va = pObj->GetPiecePointArray();
	CVector Base2D(0,0,0);
	CVector Base3D(0,0,0);
	double sta  = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	CDPoint vXYDir = vGir.Rotate(vAng);

	//���Ա��߾ӿ��� L�����߾ӱ��� ����Ÿ�
	double dis = -pGir->m_dWidth/2 + pDia->m_G_DpDp - pDia->m_X_DpDp/2 - HGap;
	Base3D = pGir->GetXyGirderDis(sta, dis, vAng);
	Base3D +=  (bStart?1:-1)*vXYDir.RotateInv90() * pDia->m_dT_DpDp/2;          //���̾������� �β� ���
	Base3D.z = pGir->GetElevationFlangeCamber(sta, dis, vAng, FALSE) + pDia->m_F_DpDp + m_InOutGate_Stt_H/2 + VGap * (nRow==1 ? 1 : -1);

	va.TransformForVert(Base2D,Base3D, (bStart?1:-1)*vXYDir);
	if(nRow==0)
		va.RotateAsAxis(Base3D, vXYDir.RotateInv90(), CDPoint(-1, 0));
		*/
}

//<summary>
//���Ա� ���� ����
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorRebar(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	double phi =  13;
	double W   = 200;
	double H   =  65;
	CDPoint xy(0, 0);

	xy.x +=W/2+phi/2; 
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.y -=(H+phi/2);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/2);

	xy.x -=(W+phi);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/2);

	xy.y +=(H+phi/2);
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.x +=phi;
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.y -=(H-phi/2);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/3);

	xy.x +=(W-phi);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/3);

	xy.y +=(H-phi/2);
	pObj->AddElement(xy,P_TYPE_LINE);

	pObj->RegenVertex();

}

//<summary>
//���Ա� ���� �̵�
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
//<param name="nRow"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorRebar3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CDiaphragm *pDia = pBx->GetDiaphragm();
	BOOL bStart = FALSE;
	bStart      = pBx->IsAbutStt();
	double HGap = 120;  
	double VGap = 110;
	double phi  =  13;
	double W    = 200;
	double H    =  65;
	double PlateT = 10;
	double disH = 125 + W/2 + phi/2;   //���Ա��������� ������ �߽ɱ���. 

	// Create 2D
	CreatePieceObjAbutDoorRebar(pObj, pBx);
	// ��ǥ ��ȯ
	CPlatePiecePointArray& va = pObj->GetPiecePointArray();
	CVector Base2D(0,0,0);
	CVector Base3D(0,0,0);
	double sta  = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	CDPoint vXYDir = vGir.Rotate(vAng);

	double dis = -pGir->m_dWidth/2 + pDia->m_G_DpDp + pDia->m_X_DpDp/2 - disH;
	Base3D   = pGir->GetXyGirderDis(sta, dis, vAng);
	if(bStart)
		Base3D  += vXYDir.RotateInv90()*pDia->m_dT_DpDp/2 
		      + (nRow==1 ? 1 : 0)*vXYDir.RotateInv90() * PlateT ;
	else
		Base3D  += -vXYDir.RotateInv90()*pDia->m_dT_DpDp/2 
		      - (nRow==0 ? 1 : 0)*vXYDir.RotateInv90() * PlateT ;

	Base3D.z = pGir->GetElevationFlangeCamber(sta, dis, vAng, FALSE) + pDia->m_F_DpDp + m_InOutGate_Stt_H/2;

	va.TransformForHori(Base2D,Base3D,vXYDir*(nRow==1 ? 1:-1));
	*/
}

//<summary>
//���Ա� �����÷���Ʈ ����
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorPlateSup(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	/*
	BOOL bStart = FALSE;
	bStart      = pBx->IsAbutStt();
	double R = 40;

	CDPoint xy(40, 40*(bStart?1:-1));  
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,R);

	xy.x -=110; 
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.y +=10*(bStart?1:-1); 
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.x -=330; 
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.y -=25*(bStart?1:-1); 
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.x +=250;	    xy.y -=75*(bStart?1:-1);
	pObj->AddElement(xy, P_TYPE_LINE);

	xy.x +=190;  
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,R);

	pObj->RegenVertex();
	*/
}

//<summary>
//���Ա� ���� �÷���Ʈ �̵�
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
//<param name="nRow"></param>
void CPlateSangSeDo::CreatePieceObjAbutDoorPlateSup3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CDiaphragm *pDia = pBx->GetDiaphragm();
	BOOL bStart = FALSE;
	bStart      = pBx->IsAbutStt();
	double HGap = 120;
	double VGap = 110;
	double Fr_Dia_To_PinCen=60;
	// Create 2D
	CreatePieceObjAbutDoorPlateSup(pObj, pBx);
	// ��ǥ ��ȯ
	CPlatePiecePointArray& va = pObj->GetPiecePointArray();
	CVector Base2D(0,0,0);
	CVector Base3D(0,0,0);
	double sta  = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	CDPoint vXYDir = vGir.Rotate(vAng);

	//���Ա��߾ӿ��� L�����߾ӱ��� ����Ÿ�
	double dis = -pGir->m_dWidth/2 + pDia->m_G_DpDp - pDia->m_X_DpDp/2 - HGap;
	Base3D = pGir->GetXyGirderDis(sta, dis, vAng);
	Base3D += (bStart?1:-1)*vXYDir.RotateInv90() * (pDia->m_dT_DpDp/2 + Fr_Dia_To_PinCen);
	Base3D.z = pGir->GetElevationFlangeCamber(sta, dis, vAng, FALSE) + pDia->m_F_DpDp + m_InOutGate_Stt_H/2 + VGap * (nRow?1:-1);

	va.TransformForHori(Base2D, Base3D, vXYDir);
	*/
}

//<summary>
//��Ȧ �÷���Ʈ ����
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlate(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	CDPoint xy(0, 0); 
	
	double InnerW = m_Manhole_W;
	double InnerH = m_Manhole_H;
	double InnerR = InnerW/2;
	double width = 70;
	double OuterW = InnerW + width*2;
	double OuterH = InnerH + width*2;
	double OuterR = OuterW/2;

	//�ܺ���
	xy.x = OuterW/2;
	xy.y = OuterH/2;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,OuterR);

	xy.x -= OuterW;	
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,OuterR);
	
	xy.y -= OuterH;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,OuterR);
	
	xy.x += OuterW;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,OuterR);

	pObj->RegenVertex();
	//������
	CPlatePieceObject *pChild = new CPlatePieceObject(pObj->GetGirder());
	xy.x = InnerW/2;
	xy.y = InnerH/2;
	pChild->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);

	xy.x -= InnerW;	
	pChild->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);
	
	xy.y -= InnerH;
	pChild->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);
	
	xy.x += InnerW;
	pChild->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);

	pChild->RegenVertex();
	pChild->SetSolid(FALSE);

	pObj->AddChildObj(pChild);
}

//<summary>
//��Ȧ�÷���Ʈ �̵�
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlate3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();

	double sta               = pBx->GetStation();
	double distToCenter      = 0;
	double verticalPosition  = sta;		        //������ ��ġ
	double crossPosition     = distToCenter;	//Ⱦ���� ��ġ	

	CJRib *pRib = pBx->GetJRib(FALSE);
	if(pRib->Getsu()%2)
		crossPosition += pRib->GetDisBetweenJRib(long(pRib->Getsu()/2))/2;

	CVector	basePOS, baseS, baseT;
	CVector vGir, vDir;
	
	CreatePieceObjManholeDoorPlate(pObj, pBx);
	CPlatePiecePointArray& rManhole      = pObj->GetPiecePointArray();
	CPlatePiecePointArray& rManholeInner = pObj->GetChild(0)->GetPiecePointArray();
	
	vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	vGir.z = pGir->GetGiulFlangeUpperLowerCamber(sta, crossPosition, FALSE);
	vGir = vGir.Unit();
	vDir = pGir->GetLine()->GetAngleAzimuth(sta).Rotate90();
	vDir.z = -pGir->GetGiulSection(pBx, 0, FALSE);
	vDir = vDir.Unit();

	basePOS = pGir->GetXyGirderDis(verticalPosition, crossPosition);	//��Ȧ�߽�
	baseS   = CVector(0, 0, 0);
	baseT   = basePOS;
	baseT.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), FALSE);

	rManhole.Transform(baseS, baseT, CDPoint(0,1), vGir, CDPoint(1,0), vDir);
	rManholeInner.Transform(baseS, baseT, CDPoint(0,1), vGir, CDPoint(1,0), vDir);
	*/
}


//<summary>
//��Ȧ �˹̴� �÷���Ʈ  ����
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlateAL(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	CDPoint xy(0, 0); 
	double space = 10;
	double InnerW = m_Manhole_W - space; 
	double InnerH = m_Manhole_H - space;
	double InnerR = m_Manhole_R - space;
	InnerR = InnerW/2;
	//�ܺ���
	xy.x = InnerW/2;
	xy.y = InnerH/2;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);

	xy.x -= InnerW;	
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);
	
	xy.y -= InnerH;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);
	
	xy.x += InnerW;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,InnerR);

	pObj->RegenVertex();
}

//<summary>
//��Ȧ �˹̴� �÷���Ʈ �̵�
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlateAL3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();

	double sta               = pBx->GetStation();
	double distToCenter      = 0;
	double verticalPosition  = sta;		        //������ ��ġ
	double crossPosition     = distToCenter;	//Ⱦ���� ��ġ	

	CJRib *pRib = pBx->GetJRib(FALSE);
	if(pRib->Getsu()%2)
		crossPosition += pRib->GetDisBetweenJRib(long(pRib->Getsu()/2))/2;

	CVector	basePOS, baseS, baseT;
	CVector vGir, vDir;
	
	CreatePieceObjManholeDoorPlateAL(pObj, pBx);
	CPlatePiecePointArray& rManholeAL = pObj->GetPiecePointArray();
	
	vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	vGir.z = pGir->GetGiulFlangeUpperLowerCamber(sta, crossPosition, FALSE);
	vGir = vGir.Unit();
	vDir = pGir->GetLine()->GetAngleAzimuth(sta).Rotate90();
	vDir.z = -pGir->GetGiulSection(pBx, 0, FALSE);
	vDir = vDir.Unit();

	basePOS = pGir->GetXyGirderDis(verticalPosition, crossPosition);	//��Ȧ�߽�
	baseS   = CVector(0, 0, 0);
	baseT   = basePOS;
	baseT.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), FALSE);

	rManholeAL.Transform(baseS, baseT, CDPoint(0,1), vGir, CDPoint(1,0), vDir);
	*/
}

//<summary>
//��Ȧ ���� ����
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlateXP13(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	//xp13, xp16, xp22  =>xp13, xp22�� �켱 �׸�.
	//xp13
	CDPoint xy(30, 30); 
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,30);

	xy.x -= 30; 
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,30);

	xy.x -= 320;
	xy.y -= 5;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,10);
	
	xy.y -= 65;
	pObj->AddElement(xy,P_TYPE_LINE);
	
	xy.x += 90;
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.y += 10;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,10);
	
	xy.x += 260;
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,30);

	pObj->RegenVertex();
}

//<summary>
//��Ȧ���� XP13 �̵�
///summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
//<param name="nRow"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlateXP133D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();
	double InnerW = m_Manhole_W;
	double InnerH = m_Manhole_H;
	double InnerR = InnerW/2;
	double width = 70;
	double OuterW = InnerW + width*2;
	double OuterH = InnerH + width*2;
	double OuterR = OuterW/2;

	double sta               = pBx->GetStation();
	double distToCenter      = 0;
	double verticalPosition  = sta;		        //������ ��ġ
	double crossPosition     = distToCenter;	//Ⱦ���� ��ġ	

	CJRib *pRib = pBx->GetJRib(FALSE);
	if(pRib->Getsu()%2)
		crossPosition += pRib->GetDisBetweenJRib(long(pRib->Getsu()/2))/2;

	CVector	basePOS, baseS, baseT;
	CVector vGir, vDir;
	
	CreatePieceObjManholeDoorPlateXP13(pObj, pBx);
	CPlatePiecePointArray& rManholeP = pObj->GetPiecePointArray();
	
	vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	vGir.z = pGir->GetGiulFlangeUpperLowerCamber(sta, crossPosition, FALSE);
	vGir = vGir.Unit();
	vDir = pGir->GetLine()->GetAngleAzimuth(sta).Rotate90();
	vDir.z = -pGir->GetGiulSection(pBx, 0, FALSE);
	vDir = vDir.Unit();

	basePOS = pGir->GetXyGirderDis(verticalPosition, crossPosition);	//��Ȧ�߽�
	baseS   = CVector(-(OuterR-35), -60, (nRow==1? 70: -70));
	baseT   = basePOS;
	baseT.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), FALSE);
	rManholeP.Transform(baseS, baseT, CVector(1,0,0), -vDir, CVector(0,0,1), -vGir);
	*/
}

//<summary>
//��Ȧ ���� XP22
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlateXP22(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	//xp13, xp16, xp22  =>xp13, xp22�� �켱 �׸�.
	//xp22

	CDPoint xy(80, 60); 
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.x -= 160; 
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.y -= 120;
	pObj->AddElement(xy,P_TYPE_LINE);
	
	xy.x += 160;
	pObj->AddElement(xy,P_TYPE_LINE);

	//������
/*	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pChild = new CPlatePieceObject(pObj->GetGirder());
	double R = 10;
	xy.x = 30;
	xy.y = 30;
	pBolt->AddCircle(xy, R, pBx);

	xy.x = -30;
	xy.y = 30;
	pBolt->AddCircle(xy, R, pBx);
	
	xy.x = -30;
	xy.y = -30;
	pBolt->AddCircle(xy, R, pBx);
	
	xy.x = 30;
	xy.y = -30;
	pBolt->AddCircle(xy, R, pBx);
	pObj->AddChildObj(pBolt);*/
}

//<summary>
//��Ȧ ���� XP22 �̵�
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
//<param name="nRow"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorPlateXP223D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();

	double sta               = pBx->GetStation();
	double distToCenter      = 0;
	double verticalPosition  = sta;		        //������ ��ġ
	double crossPosition     = distToCenter;	//Ⱦ���� ��ġ	

	CJRib *pRib = pBx->GetJRib(FALSE);
	if(pRib->Getsu()%2)
		crossPosition += pRib->GetDisBetweenJRib(long(pRib->Getsu()/2))/2;

	CVector	basePOS, baseS, baseT;
	CVector vGir, vDir;
	
	CreatePieceObjManholeDoorPlateXP22(pObj, pBx);
	CPlatePiecePointArray& rManhole22 = pObj->GetPiecePointArray();
//	CPlatePiecePointArray& rManhole22Bolt = pObj->GetChild(0)->GetPiecePointArray();	
	vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	vGir.z = pGir->GetGiulFlangeUpperLowerCamber(sta, crossPosition, FALSE);
	vGir = vGir.Unit();
	vDir = pGir->GetLine()->GetAngleAzimuth(sta).Rotate90();
	vDir.z = -pGir->GetGiulSection(pBx, 0, FALSE);
	vDir = vDir.Unit();

	basePOS = pGir->GetXyGirderDis(verticalPosition, crossPosition);	//��Ȧ�߽�
	baseS   = CVector(0, -20, (nRow==1?-10:0));
	baseT   = basePOS;
	baseT.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), FALSE);

	rManhole22.Transform(baseS, baseT, CDPoint(1,0), vGir, CDPoint(0,1), vDir);
//	rManhole22Bolt.Transform(baseS, baseT, CDPoint(1,0), vGir, CDPoint(0,1), vDir);
*/
}

//<summary>
//��Ȧ���� L����
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorL(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	CVectorArray va;

	double L = 90;
	double W = 60;
	va.Add(CVector(-W/2, L, 0));
	va.Add(CVector( W/2, L, 0));
	va.Add(CVector(-W/2, 0, 0));
	va.Add(CVector( W/2, 0, 0));
	va.Add(CVector(-W/2, 0, L));
	va.Add(CVector( W/2, 0, L));

	pObj->GetPiecePointArray() = va;


}

//<summary>
//��Ȧ ���� L���� �̵�
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
//<param name="nRow"></param>
void CPlateSangSeDo::CreatePieceObjManholeDoorL3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx, long nRow)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();
	double InnerW = m_Manhole_W;
	double width = 70;
	double OuterW = InnerW + width*2;
	double OuterR = OuterW/2;

	double sta               = pBx->GetStation();
	double distToCenter      = 0;
	double verticalPosition  = sta;		        //������ ��ġ
	double crossPosition     = distToCenter;	//Ⱦ���� ��ġ	

	CJRib *pRib = pBx->GetJRib(FALSE);
	if(pRib->Getsu()%2)
		crossPosition += pRib->GetDisBetweenJRib(long(pRib->Getsu()/2))/2;

	CVector	basePOS, baseS, baseT;
	CVector vGir, vDir;
	
	CreatePieceObjManholeDoorL(pObj, pBx);
	CPlatePiecePointArray& rManholeL = pObj->GetPiecePointArray();
	
	vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	vGir.z = pGir->GetGiulFlangeUpperLowerCamber(sta, crossPosition, FALSE);
	vGir = vGir.Unit();
	vDir = pGir->GetLine()->GetAngleAzimuth(sta).Rotate90();
	vDir.z = -pGir->GetGiulSection(pBx, 0, FALSE);
	vDir = vDir.Unit();

	basePOS = pGir->GetXyGirderDis(verticalPosition, crossPosition);	//��Ȧ�߽�
	baseS   = CVector((OuterR - 35) * (nRow? -1 : 1), -75, -10);
	baseT   = basePOS;
	baseT.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), FALSE);
	rManholeL.Transform(baseS, baseT, CVector(1,0,0), vDir*(nRow==1?-1:1), CVector(0,1,0), vGir*(nRow==1?1:-1));
	*/
}


//<summary>
//��Ȧ ���� ���� �̵� 
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo:: CreatePieceObjManholeDoorRebar3D(CPlatePieceObject *pObj, CPlateBasicIndex *pBx)
{
	/*
	CPlateGirderApp *pGir  = pBx->GetGirder();
	double InnerW = m_Manhole_W;
	double width = 70;
	double OuterW = InnerW + width*2;
	double OuterR = OuterW/2;

	double sta               = pBx->GetStation();
	double distToCenter      = 0;
	double verticalPosition  = sta;		        //������ ��ġ
	double crossPosition     = distToCenter;	//Ⱦ���� ��ġ	

	CJRib *pRib = pBx->GetJRib(FALSE);
	if(pRib->Getsu()%2)
		crossPosition += pRib->GetDisBetweenJRib(long(pRib->Getsu()/2))/2;

	CVector	basePOS, baseS, baseT;
	CVector vGir, vDir, vVert;
	
	CreatePieceObjManholeDoorRebar(pObj, pBx);
	CPlatePiecePointArray& rManholeL = pObj->GetPiecePointArray();
	
	vGir = pGir->GetLine()->GetAngleAzimuth(sta);
	vGir.z = pGir->GetGiulFlangeUpperLowerCamber(sta, crossPosition, FALSE);
	vGir = vGir.Unit();
	vDir = pGir->GetLine()->GetAngleAzimuth(sta).Rotate90();
	vDir.z = -pGir->GetGiulSection(pBx, 0, FALSE);
	vDir = vDir.Unit();
	vVert = (vGir * vDir).Unit();

	basePOS = pGir->GetXyGirderDis(verticalPosition, crossPosition);	//��Ȧ�߽�
	baseS   = CVector(0, 10, 20);
	baseT   = basePOS;
	baseT.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), FALSE);
	rManholeL.Transform(baseS, baseT, CVector(1,0,0), vGir, CVector(0,0,1), -vDir);
	*/
}


//<summary>
//��Ȧ ���� ���� ����
//</summary>
//<param name="*pObj"></param>
//<param name="*pBx"></param>
void CPlateSangSeDo:: CreatePieceObjManholeDoorRebar(CPlatePieceObject  *pObj, CPlateBasicIndex *pBx)
{
	double phi =  16;
	double W   = 100;
	double H   =  50;
	CDPoint xy(0, 0);

	xy.x +=W/2+phi/2; 
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.y -=(H+phi/2);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/2);

	xy.x -=(W+phi);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/2);

	xy.y +=(H+phi/2);
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.x +=phi;
	pObj->AddElement(xy,P_TYPE_LINE);

	xy.y -=(H-phi/2);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/3);

	xy.x +=(W-phi);
	pObj->AddElement(xy,P_TYPE_FILLET,-1,pBx,phi/3);

	xy.y +=(H-phi/2);
	pObj->AddElement(xy,P_TYPE_LINE);

	pObj->RegenVertex();
}

