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
	long nFlag	= 2;	// 080205 ö�ٰ������� ������ũ��Ʈ �߰� - lina
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
		ar << m_bShrinkageConcrete;	// ������

		for(n=0; n<BMOPT_CHKSHEET; n++)
			ar << m_bChkSheet[n];						// �ܱ�����꼭 ��� 	
		for(n=0; n<BMOPT_CHKSH_2ND; n++)
			ar << m_bChk2ndSh_Model[n];					//	2�� �����׸�
		for(n=0; n<BMOPT_CHKSH_3TH; n++)
			ar << m_bChk3thSh_Force[n];					//	3�� �����׸�
		for(n=0; n<BMOPT_CHKSH_4TH; n++)
			ar << m_bChk4thSh_Section[n];				//	4�� �����׸�
		for(n=0; n<BMOPT_CHKSH_5TH; n++)
			ar << m_bChk5thSh_Joint[n];					//	5�� �����׸�		
		for(n=0; n<BMOPT_CHKSH_8TH; n++)
			ar << m_bChk8thSh_Diap[n];					//	8�� �����׸�		
		for(n=0; n<BMOPT_CHKSH_9TH; n++)
			ar << m_bChk9thSh_Check[n];					//	9�� �����׸�		
		
		ar << m_nSh2ndOptGir;							//		2�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)	
		ar << m_nSh4thOptGir;							//		4�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)
		ar << m_bChkSh9thPrecise;						//		9�� �������� ���н�
		ar << m_bChkSh9thSimple;						//		9�� �������� �����
		
		ar << m_structWingWall.m_bApply;
		ar << m_structWingWall.m_dLengthLeftStt;
		ar << m_structWingWall.m_dLengthLeftEnd;
		ar << m_structWingWall.m_dLengthRighStt;
		ar << m_structWingWall.m_dLengthRighEnd;

		ar << m_structStyrofoam.m_bLeft;

		ar << m_nDrainEstablishType;
		ar << m_structRiverDrainEstablish.m_nWaterCollect;				// ������
		ar << m_structRiverDrainEstablish.m_dDrain;						// �����

		ar << m_structWalkWayDrainEstablish.m_nType;					// Ÿ��
		ar << m_structWalkWayDrainEstablish.m_nWaterCollect;			// ������
		ar << m_structWalkWayDrainEstablish.m_nDrain;					// �����
		ar << m_structWalkWayDrainEstablish.m_nJoinDrain;				// ��������
		for(n=0; n<2; n++)
		{
			ar << m_structWalkWayDrainEstablish.m_dStraightTube[n];		// ����
			ar << m_structWalkWayDrainEstablish.m_nCurveTube[n];		// ���
			ar << m_structWalkWayDrainEstablish.m_nCleanTube[n];		// û�ұ�
			ar << m_structWalkWayDrainEstablish.m_nUpperFixed[n];		// ��ΰ�����
		}
		ar << m_structWalkWayDrainEstablish.m_nCollecting;				// ħ����
		ar << m_structWalkWayDrainEstablish.m_nJoint;					// �����
		ar << m_structWalkWayDrainEstablish.m_nJoinWaterCollect;		// ����������

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

		ar << m_structParapet.m_dGuardLength[0];		// ��ȣ���� ��������
		ar << m_structParapet.m_dGuardLength[1];		// ��ȣ���� ��������
		ar << m_structParapet.m_dCurbLength;			// ������ ����
		ar << m_structParapet.m_nCurbQty;				// ���� ���밹��

		ar << m_structNotch.m_nApplyPos;				// Notch

		ar << m_structSoundProof.m_dRHeight;			// �����ǳ� ����
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
			ar >> m_bChkSheet[n];						// �ܱ�����꼭 ��� 	
		for(n=0; n<BMOPT_CHKSH_2ND; n++)
			ar >> m_bChk2ndSh_Model[n];					//	2�� �����׸�
		for(n=0; n<BMOPT_CHKSH_3TH; n++)
			ar >> m_bChk3thSh_Force[n];					//	3�� �����׸�
		for(n=0; n<BMOPT_CHKSH_4TH; n++)
			ar >> m_bChk4thSh_Section[n];				//	4�� �����׸�
		for(n=0; n<BMOPT_CHKSH_5TH; n++)
			ar >> m_bChk5thSh_Joint[n];					//	5�� �����׸�		
		for(n=0; n<BMOPT_CHKSH_8TH; n++)
			ar >> m_bChk8thSh_Diap[n];					//	8�� �����׸�		
		for(n=0; n<BMOPT_CHKSH_9TH; n++)
			ar >> m_bChk9thSh_Check[n];					//	9�� �����׸�		
		
		ar >> m_nSh2ndOptGir;							//		2�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)	
		ar >> m_nSh4thOptGir;							//		4�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)
		ar >> m_bChkSh9thPrecise;						//		9�� �������� ���н�
		ar >> m_bChkSh9thSimple;						//		9�� �������� �����

		ar >> m_structWingWall.m_bApply;
		ar >> m_structWingWall.m_dLengthLeftStt;
		ar >> m_structWingWall.m_dLengthLeftEnd;
		ar >> m_structWingWall.m_dLengthRighStt;
		ar >> m_structWingWall.m_dLengthRighEnd;

		ar >> m_structStyrofoam.m_bLeft;

		ar >> m_nDrainEstablishType;
		ar >> m_structRiverDrainEstablish.m_nWaterCollect;				// ������
		ar >> m_structRiverDrainEstablish.m_dDrain;						// �����

		ar >> m_structWalkWayDrainEstablish.m_nType;					// Ÿ��
		ar >> m_structWalkWayDrainEstablish.m_nWaterCollect;			// ������
		ar >> m_structWalkWayDrainEstablish.m_nDrain;					// �����
		ar >> m_structWalkWayDrainEstablish.m_nJoinDrain;				// ��������
		for(n=0; n<2; n++)
		{
			ar >> m_structWalkWayDrainEstablish.m_dStraightTube[n];		// ����
			ar >> m_structWalkWayDrainEstablish.m_nCurveTube[n];		// ���
			ar >> m_structWalkWayDrainEstablish.m_nCleanTube[n];		// û�ұ�
			ar >> m_structWalkWayDrainEstablish.m_nUpperFixed[n];		// ��ΰ�����
		}
		ar >> m_structWalkWayDrainEstablish.m_nCollecting;				// ħ����
		ar >> m_structWalkWayDrainEstablish.m_nJoint;					// �����
		ar >> m_structWalkWayDrainEstablish.m_nJoinWaterCollect;		// ����������

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

		ar >> m_structParapet.m_dGuardLength[0];		// ��ȣ���� ��������
		ar >> m_structParapet.m_dGuardLength[1];		// ��ȣ���� ��������
		ar >> m_structParapet.m_dCurbLength;			// ������ ����
		ar >> m_structParapet.m_nCurbQty;				// ���� ���밹��
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
	m_structPave.m_bApply								= TRUE;				// ����
	m_structPave.m_dPaveThick							= 80;				// ����β�

	m_structTackCoating.m_bApply						= TRUE;				// ������
	m_structTackCoating.m_dTackCoating					= 1.5;				// 1.5��

	m_structBridgeName.m_bApply							= FALSE;			// ������
	m_structBridgeName.m_bOneSide						= FALSE;			// ����.����
	m_structBridgeName.m_bSttPoint						= FALSE;			// ����
	m_structBridgeName.m_bEndPoint						= FALSE;			// ����

	m_structBridgeNamePlate.m_bApply					= FALSE;			// ������
	m_structBridgeNamePlate.m_bSttPoint					= FALSE;			// ����
	m_structBridgeNamePlate.m_bEndPoint					= FALSE;			// ����

	m_structExplainPlate.m_bApply						= FALSE;			// ������
	m_structExplainPlate.m_bSttPoint					= FALSE;			// ����
	m_structExplainPlate.m_bEndPoint					= FALSE;			// ����

	m_structEstablishTBM.m_bApply						= FALSE;			// TBM ��ġ
	m_structEstablishTBM.m_bSttPoint					= FALSE;			// ����
	m_structEstablishTBM.m_bEndPoint					= FALSE;			// ����

	m_structStyrofoam.m_bApply							= FALSE;			// ��Ƽ����
	m_structStyrofoam.m_dThick							= 20;				// �β� 10mm or 20mm
	m_structStyrofoam.m_bLeft							= TRUE;

	m_structExpansionJoint.m_bApply						= TRUE;				// ��������
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
	m_structExpansionJoint.m_dMargin					= 70.0;				// ����ġ��
	m_structExpansionJoint.m_nMarginsu					= 1;				// ����ġ������
	m_structExpansionJoint.m_bIncludeWalkLoad			= FALSE;

	m_structExpansionJointCover.m_bApply				= TRUE;				// �������� ����
	for(n= 0; n<7; n++)
	{
		m_structExpansionJointCover.m_bSttPoint[n]		= FALSE;			// ���� - ������
		m_structExpansionJointCover.m_bEndPoint[n]		= FALSE;			// ���� - ������
		m_structExpansionJointCover.m_dHeight[n]		= 2000.0;			// �Ÿ�
		m_structExpansionJointCover.m_nQty[n]			= 1;				// ����
	}

	m_structNotch.m_bApply								= TRUE;				// Notch
	m_structNotch.m_bLeft								= TRUE;				// ����
	m_structNotch.m_bRight								= TRUE;				// ����
	m_structNotch.m_nApplyPos							= 0;				// ��������

	m_structGuardFence.m_bApply							= FALSE;			// GuardFence
	m_structGuardFence.m_dHeight						= 2000.0;

	m_structSoundProof.m_bApply							= FALSE;			// �����ǳ�
	m_structSoundProof.m_dLHeight						= 2000.0;
	m_structSoundProof.m_dRHeight						= 2000.0;

	m_structParapet.m_bApply							= TRUE;				// ����
	for(n=0; n<3; n++)
	{
		m_structParapet.m_bLeft[n]						= TRUE;
		m_structParapet.m_bRight[n]						= TRUE;
		m_structParapet.m_dHeight[n]					= 2000.0;
	}
	m_structParapet.m_dGuardLength[0]					= 0;
	m_structParapet.m_dGuardLength[1]					= 0;
	m_structParapet.m_dCurbLength						= 0;			// ������ ����
	m_structParapet.m_nCurbQty							= 1;			// ���� ���밹��

	m_bConcreteBuy										= TRUE;			// ��ũ��Ʈ ����
	m_bConcretePlacing									= TRUE;			// ��ũ��Ʈ Ÿ��
	m_bMold												= TRUE;			// ��Ǫ��
	m_bSpacer											= TRUE;			// �����̼�
	m_bRebarManufacture									= TRUE;			// ö�� ���� ����	
	m_bDrainBridgeSurface								= TRUE;			// ������
	m_bWaterDraw										= TRUE;			// ���� ������
	m_bShade											= FALSE;		// ������
	m_bDroppingPrevent									= TRUE;			// ���Ϲ� ������
	m_bSteelQuantity									= TRUE;			// ���� ����
	m_bPaint											= TRUE;			// ���� ����
	m_bDrainEstablish									= TRUE;			// ��� �ü�
	m_bShrinkageConcrete								= TRUE;			// ������ ��ũ��Ʈ	- ������

	m_structWingWall.m_bApply							= TRUE;			// ������ ����
	m_structWingWall.m_dLengthLeftStt					= 6000;			// ������ ���� ����(mm)
	m_structWingWall.m_dLengthLeftEnd					= 6000;			// ������ ���� ����(mm)
	m_structWingWall.m_dLengthRighStt					= 6000;			// ������ ���� ����(mm)
	m_structWingWall.m_dLengthRighEnd					= 6000;			// ������ ���� ����(mm)

	m_nDrainEstablishType								= RIVER;
	m_structRiverDrainEstablish.m_nWaterCollect			= 0;			// ������
	m_structRiverDrainEstablish.m_dDrain				= 0;			// �����

	m_structWalkWayDrainEstablish.m_nType				= 0;
	m_structWalkWayDrainEstablish.m_nWaterCollect		= 0;			// ������
	m_structWalkWayDrainEstablish.m_nDrain				= 0;			// �����
	m_structWalkWayDrainEstablish.m_nJoinDrain			= 0;			// ��������
	for(n=0; n<2; n++)
	{
		m_structWalkWayDrainEstablish.m_dStraightTube[n]= 0;			// ����
		m_structWalkWayDrainEstablish.m_nCurveTube[n]	= 0;			// ���
		m_structWalkWayDrainEstablish.m_nCleanTube[n]	= 0;			// û�ұ�
		m_structWalkWayDrainEstablish.m_nUpperFixed[n]	= 0;			// ��ΰ�����
	}
	m_structWalkWayDrainEstablish.m_nCollecting			= 0;			// ħ����
	m_structWalkWayDrainEstablish.m_nJoint				= 0;			// �����
	m_structWalkWayDrainEstablish.m_nJoinWaterCollect	= 0;			// ����������

	m_structTimber.m_bApply								= TRUE;			// ���ٸ�
	m_structTimber.m_nQuality							= 0;			// ���ٸ� ���� - ����
	m_structTimber.m_nPosDeckFinisher					= 2;			// ��ũ�Ǵϼ� ��ġ
	m_structTimber.m_dDeckFinisherWorkWidth				= 585.0;		// ��ũ�Ǵϼ� �۾���

	m_structShrinkageConcrete.m_bApply					= TRUE;			// ��������ũ��Ʈ
	m_structShrinkageConcrete.m_nSttQty					= 2;			// �����ΰ���
	m_structShrinkageConcrete.m_nEndQty					= 2;			// �����ΰ���

	m_structRebarManufacture.m_bApply					= TRUE;			// ö�ٰ�������
	m_structRebarManufacture.m_nSlabType				= 1;			// ������ Ÿ��	- ����
	m_structRebarManufacture.m_nGuardFenceType			= 2;			// ��ȣ�� Ÿ��	- ����
	m_structRebarManufacture.m_nExpansionJointType		= 3;			// ��������		- ����
	m_structRebarManufacture.m_nBindConcType			= 2;			// ������ũ��Ʈ - ����

	m_structBridgeName.m_nQty							= 0;			// ������ ����
	m_structBridgeNamePlate.m_nQty						= 1;			// ������ ����
	m_structExplainPlate.m_nQty							= 1;			// ������ ����
	m_structEstablishTBM.m_nQty							= 1;			// TBM ����

	m_structDroppingPrevent.m_bApply					= TRUE;
	m_structDroppingPrevent.m_bIsOnlyHorizontal			= FALSE;
	m_structDroppingPrevent.m_dHorSpaceHeight			= 1000.0;
	m_structDroppingPrevent.m_dVerSpaceHeight			= 1000.0;

	m_structNotch.m_szNotchType							= _T("�˷�̴���");

	m_structElecWireHole.m_bApply						= FALSE;
	m_structElecWireHole.m_nEA100[0]					= 1;
	m_structElecWireHole.m_nEA100[1]					= 1;
	m_structElecWireHole.m_nEA125[0]					= 1;
	m_structElecWireHole.m_nEA125[1]					= 1;
	m_structElecWireHole.m_nEA150[0]					= 0;
	m_structElecWireHole.m_nEA150[1]					= 0;	

	// ������꼭 ��¿ɼ�	
	for(n=0; n<BMOPT_CHKSHEET; n++)
		m_bChkSheet[n]			= FALSE;			// �ܱ�����꼭 ��� 	
	for(n=0; n<BMOPT_CHKSH_2ND; n++)
		m_bChk2ndSh_Model[n]	= TRUE;				//	2�� �����׸�
	for(n=0; n<BMOPT_CHKSH_3TH; n++)
		m_bChk3thSh_Force[n]	= TRUE;				//	3�� �����׸�
	for(n=0; n<BMOPT_CHKSH_4TH; n++)
		m_bChk4thSh_Section[n]	= TRUE;				//	4�� �����׸�
	for(n=0; n<BMOPT_CHKSH_5TH; n++)
		m_bChk5thSh_Joint[n]	= TRUE;				//	5�� �����׸�		
	for(n=0; n<BMOPT_CHKSH_8TH; n++)
		m_bChk8thSh_Diap[n]		= TRUE;				//	8�� �����׸�		
	for(n=0; n<BMOPT_CHKSH_9TH; n++)
		m_bChk9thSh_Check[n]	= TRUE;				//	9�� �����׸�		
	
	m_nSh2ndOptGir				= 0;				//		2�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)	
	m_nSh4thOptGir				= 0;				//		4�� �Ŵ� ���� �ɼ� (0:�Ŵ����о���, 1:���Ŵ�����)
	m_bChkSh9thPrecise			= TRUE;				//		9�� �������� ���н�
	m_bChkSh9thSimple			= FALSE;			//		9�� �������� �����
}

// ��� ������.... �ٵ� �� ã�ڳ�.....
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
	if(nIdx == GENERAL_GUARDFENCE)		szPos = _T("�Ϲݹ�ȣ��");
	if(nIdx == ABSOLUTE_GUARDFENCE)		szPos = _T("�����ȣ��");
	if(nIdx == GENERAL_SOUNDPROOF)		szPos = _T("�Ϲݹ�����");
	if(nIdx == ABSOLUTE_SOUNDPROOF)		szPos = _T("���������");
	if(nIdx == CENTER_GUARDFENCE)		szPos = _T("��   ��   ��");
	if(nIdx == PARAPET)					szPos = _T("��         ��");
	if(nIdx == CURB)					szPos = _T("��         ��");

	return szPos;
}

CString	CBMOption::GetParapetType(long nIdx)
{
	CString szParapetType	= _T("");

	if(nIdx == DRIVEWAY)			szParapetType = _T("��   ��   ��");
	if(nIdx == FOOTWAY)				szParapetType = _T("��   ��   ��");
	if(nIdx == DRIVE_FOOT_BOUNDARY)	szParapetType = _T("����������");

	return szParapetType;
}

CString	CBMOption::GetBMOptItem(long nIdx)
{
	CString	szBMOpt	= _T("");

	if(nIdx == BMOPT_CONCRETE_BYE)			szBMOpt	= _T("��ũ��Ʈ ����");
	if(nIdx == BMOPT_CONCRETE_PLACING)		szBMOpt	= _T("��ũ��Ʈ Ÿ��");
	if(nIdx == BMOPT_MOLD)					szBMOpt	= _T("��Ǫ��");
	if(nIdx == BMOPT_SPACER)				szBMOpt	= _T("�����̼�");
	if(nIdx == BMOPT_REBAR_MANUFACTURE)		szBMOpt	= _T("ö�� ���� ����");
	if(nIdx == BMOPT_PAVE)					szBMOpt	= _T("����");
	if(nIdx == BMOPT_TACKCOATING)			szBMOpt	= _T("������");
	if(nIdx == BMOPT_DRAINBRIDGESURFACE)	szBMOpt	= _T("������");
	if(nIdx == BMOPT_BRIDGENAME)			szBMOpt	= _T("������");
	if(nIdx == BMOPT_BRIDGENAME_PLATE)		szBMOpt	= _T("������");
	if(nIdx == BMOPT_EXPLAIN_PLATE)			szBMOpt	= _T("������");
	if(nIdx == BMOPT_ESTABLISH_TBM)			szBMOpt	= _T("TBM ��ġ");
	if(nIdx == BMOPT_STYROFOAM)				szBMOpt	= _T("��Ƽ����");
	if(nIdx == BMOPT_EXPANSIONJOINT)		szBMOpt	= _T("��������");
	if(nIdx == BMOPT_EXPANSIONJOINT_COVER)	szBMOpt	= _T("�������� ����");
	if(nIdx == BMOPT_SHRINKAGE_CONCRETE)	szBMOpt	= _T("������ ��ũ��Ʈ");
	if(nIdx == BMOPT_WATER_DRAW)			szBMOpt	= _T("���� ������");
	if(nIdx == BMOPT_NOTCH)					szBMOpt	= _T("NOTCH");
	if(nIdx == BMOPT_GUARD_FENCE)			szBMOpt	= _T("�����潺");
	if(nIdx == BMOPT_SOUND_PROOF)			szBMOpt	= _T("�����ǳ�");
	if(nIdx == BMOPT_PARAPET)				szBMOpt	= _T("����");
	if(nIdx == BMOPT_SHADE)					szBMOpt	= _T("������");
	if(nIdx == BMOPT_DROPPING_PREVENT)		szBMOpt	= _T("���Ϲ� ������");
	if(nIdx == BMOPT_STEEL_QUANTITY)		szBMOpt	= _T("���� ����");
	if(nIdx == BMOPT_PAINT)					szBMOpt	= _T("���� ����");
	if(nIdx == BMOPT_DRAIN_ESTABLISH)		szBMOpt	= _T("��� �ü�");
	if(nIdx == BMOPT_WING_WALL)				szBMOpt = _T("�� �� ��");
	if(nIdx == BMOPT_TIMBER)				szBMOpt = _T("�� �� ��");
	if(nIdx == BMOPT_ELECWIREHOLE)			szBMOpt = _T("�� �� ��");
	return szBMOpt;
}

CString CBMOption::GetDrainEstablishType()
{
	CString szDrain = _T("");
	if(m_nDrainEstablishType == RIVER)			szDrain = _T("��õ��");
	if(m_nDrainEstablishType == WALKWAY)		szDrain = _T("������");
	if(m_nDrainEstablishType == RIVERWALKWAY)	szDrain = _T("��õ��������");

	return szDrain;
}

long CBMOption::GetIdxDrainEstablishType(CString szType)
{
	long nIdx = 0;
	if(szType == _T("��õ��"))			nIdx = RIVER;
	if(szType == _T("������"))			nIdx = WALKWAY;
	if(szType == _T("��õ��������"))	nIdx = RIVERWALKWAY;

	return nIdx;
}

long CBMOption::GetIdxTimberQuality(CString szType)
{
	long nQuality = 0;
	if(szType == _T("����"))	nQuality = 0;
	if(szType == _T("����"))	nQuality = 1;

	return nQuality;
}

CString	CBMOption::GetTimberQuality(long nIdx)
{
	CString szQuality = _T("");

	if(nIdx == 0)	szQuality = _T("����");
	if(nIdx == 1)	szQuality = _T("����");

	return szQuality;
}

CString CBMOption::GetPosDeckFinisher(long nIdx)
{
	CString szPos = _T("");
	if(nIdx == 0)	szPos = _T("������");
	if(nIdx == 1)	szPos = _T("��������");
	if(nIdx == 2)	szPos = _T("��������");
	if(nIdx == 3)	szPos = _T("��������");	

	return szPos;
}

long CBMOption::GetIdxDeckFinisher(CString szPos)
{
	long nIdx = 0;

	if(szPos == _T("������"))	nIdx = 0;
	if(szPos == _T("��������"))	nIdx = 1;
	if(szPos == _T("��������"))	nIdx = 2;
	if(szPos == _T("��������"))	nIdx = 3;
	return nIdx;
}

long CBMOption::GetNotchPos(CString szPos)
{
	long nIdx = 0;
	if(szPos == _T("��������"))	nIdx = 0;
	if(szPos == _T("��������"))	nIdx = 1;
	if(szPos == _T("��������"))	nIdx = 2;

	return nIdx;
}

CString CBMOption::GetNotchPos(long nPos)
{
	CString szPos	= _T("");
	if(nPos == 0)	szPos = _T("��������");
	if(nPos == 1)	szPos = _T("��������");
	if(nPos == 2)	szPos = _T("��������");

	return szPos;
}

CString CBMOption::GetRebarManufactureType(long nType)
{
	CString szType = _T("");
	if(nType == 0)	szType = _T("�ſ캹��");
	if(nType == 1)	szType = _T("����");
	if(nType == 2)	szType = _T("����");
	if(nType == 3)	szType = _T("����");
	if(nType == 4)	szType = _T("������");

	return szType;
}

long CBMOption::GetIdxRebarManufactureType(CString szType)
{
	long nIdx = 0;
	if(szType == _T("�ſ캹��"))	nIdx = 0;
	if(szType == _T("����"))		nIdx = 1;
	if(szType == _T("����"))		nIdx = 2;
	if(szType == _T("����"))		nIdx = 3;
	if(szType == _T("������"))		nIdx = 4;
	return nIdx;
}

CString	CBMOption::GetDroppingPrevent(BOOL bApply)
{
	if(bApply)	return _T("��������");
	else		return _T("�����������");

	return _T("");
}

BOOL CBMOption::GetDroppingPrevent(CString szApply)
{
	if(szApply == _T("��������"))			return TRUE;
	else if(szApply == _T("�����������"))	return FALSE;

	return FALSE;
}