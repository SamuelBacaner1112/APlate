// GlobarOption.cpp: implementation of the CGlobarOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobarOption.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobarOption::CGlobarOption()
{
	Init();
}

CGlobarOption::~CGlobarOption()
{

}

void CGlobarOption::Init()
{
	InitDesignBasic();
	InitDesignQty();
}

void CGlobarOption::InitDesignBasic()
{
	m_nDesignCondition							= 0;		// ��������
	m_nDesignBridgeType                         = 1;        // ��������    0 : �÷���Ʈ  1 : �Ҽ�����
	m_nDesignSlabType                           = 0;		// ����������  0 : RC        1 : PSC
	m_nDesignSlabConType                        = 1;        // ������ð����� 
	m_nDesignGirderConnect                      = 0;		// ������������ 0: ���������� 1: ������� 
	m_nDesignUseHBeamAtCr                       = 0;		// H�������	
	m_nDesignHeigtBaseUpper						= 0;		// �ʱⰪ..�Ҽ�����...��� ���
	m_nDesignHeigtBaseLower						= 1;		// �ʱ��..�Ҽ�����...�ϸ� �ϴ�  ...070202���� ����....KB...�Ҽ��ְŴ��� �������� ��ǥȸ ��...
	m_nDesignHBracingDirChange					= 0;		// ����극�̽� ���� ���� ���� �ɼ�
	//����
	m_nDesignSteelChangeByThick					= 2;		// �β��� ������ȯ ����	
	m_dDesignMinThickOfUDFlate					= 12.0;		// ��.���� �����β�    (mm)
	m_dDesignMinThickOfUpFlate					= 12.0;
	m_dDesignMinThickOfLoFlate					= 12.0;
	m_dDesignMinThickOfWebFlate					= 12.0;		// ������ �����β�    (mm)
	m_dDesignAllowMaxThick						= 8.0;		// �������� ���β� ����(mm)
	m_bDesignAllowDeflection					= TRUE;
	//��� 
	m_dDesignMaxWeightOfCarLoad					= 200000;	// �������� �ִ��߷�   (N)
	m_dDesignMaxLengthOfFieldSplice				= 13750;	// �������� �ִ����    (mm)
	//����
	m_nDesignDimensionOfFilletWeld				= 2;		// �ʷ� ���� ġ�� ����
	m_dDesignMinLenOfFilletWeld					= 80;		// �ʷ� ���� �ּұ���    (mm)
	m_dDesignThickWeldNoCut						=  0;
	m_dDesignThickXWeldApply					= 16;		// X ���� ���� ���� �β� : �Էµβ� ���� V���� ����
	//��Ʈ��ġ
	m_nDesignBoltHoleJigJaegOfHorPlate			= 1;		// ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
	m_nDesignBoltHoleJigJaegOfVerPlate			= 0;		// ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
	m_nDesignBoltHoleJigJaegOfOutCrossPlate		= 0;		// ��Ʈ��ġ ������� Ÿ�� �������κ� ��.����
	m_nDesignBoltHoleJigJaegOfOutBeamPlate		= 0;		// ��Ʈ��ġ ������� Ÿ�� ������ ��.����
	m_nDesignBoltHoleArrangeByunSectionType		= 0;		// ��Ʈ��ġ ������ ���ܸ� Ÿ��
	m_nDesignBoltHoleArrangeVerticalBeam		= 0;		// ��Ʈ��ġ ������ ���κ�
	m_nDesignBoltHoleArrangeCrossBeam			= 0;		// ��Ʈ��ġ ������ ���κ�
	m_nDesignBoltHoleArrangeOutCrossBeam		= 0;		// �������κ� ������ ��Ʈ ��ġ
	m_nDesignBoltHoleArrangeOutBeam				= 0;		// ��Ʈ��ġ ������ ������
	m_dDesignBoltJongVSlopApplyUp				= 3;		// ��Ʈ��ġ ������ ��� ��� ���� (%)
	m_dDesignBoltJongVSlopApplyDn				= 3;		// ��Ʈ��ġ ������ �Ϻ� ��� ���� (%)
	m_bBoltLengthAuto							= TRUE;		// ��� ��Ʈ ���� �ڵ� ���
	//���κ�
	m_nDesignCrossBeamBracketType				= 0;		// ���κ� ����� ��� Ÿ��
	m_nDesignCrossBeamHStiffAtBracket			= 1;		// ���κ� ���򺸰��� �������ġ ����	
	//����������
	m_dSttExpansionJointHeight					= 150;
	m_dSttExpansionJointWidth					= 300;
	m_dEndExpansionJointHeight					= 150;
	m_dEndExpansionJointWidth					= 300;
	//�����ȣ
	m_nDesignStartNumberOfGirder				= 0;		// �Ŵ����� ��ȣ
	m_nDesignStartNumberOfSplice				= 0;		// ���ö��̽� ���۹�ȣ
	//��������
	m_nDesignUnitTonfMeter						= 1;		// �߷�,�Ÿ�,��ǥ ���� - �߰� 2002/10/02
	m_nDesignPointSlope							= 3;		// ���� �� ���ܱ��� ǥ�� �ڸ��� - �߰� 2002/10/02
	m_nDesignByundanVSlopRoundPos				= 2;		// // ������ ������� ����(�ݿø� �ڸ���)
	//��Ÿ
	m_nDesignHBracingInstall					= 1;		//����극�̽� ��ġ Ÿ�� - �߰� 2003/03/25
	m_nDesignStringerJijumApply					= 1;		// ���κ� �ܺ� ������ ���� - �߰� 2002/08/12
	m_nDesignDanmyunCheck                       = 0;		// �ܸ����� �Ŵ�����(�����Ŵ�, �����Ŵ� ��������)
	m_nDesignGirderEndType						= 0;        // ���� ����Ÿ��

	m_nDegreeEarthQuake							= 0;		// ������� (1,2,Ư)
	m_nRegionEarthQuake							= 0;		// �������� (1,2)
	m_dCoefficientGround						= (long)1.0;
	m_dRiskCoefficient							= 1.4;		///< ���赵 ���
	m_dZoneCoefficient							= 0.11;		///< �������

	m_nDesignVStiffInstall						= 0;		// ���κ����̿� ���������� ��ġ	[��ġ]
	m_nDesignHStiffInstall						= 0;		// ���򺸰��� ��ġ				[��ġ]
	m_bInstallBeamAtCenter						= FALSE;	// �극�̽� ������ ������ ��ġ
	m_nDesignAllowCrackWidthEnv					= 1;		// ������ �νĿ� ���� ȯ������ (���տ���) - �߰� : �Ϲ�ȯ��			
	
	m_nSlabCentDesignMethod						= 0;		//�ٴ��� �߾Ӻ� ö�� ��� ���
	m_dAccelerationOfGravity					= 9.81;		// �߿����ӵ�(m/sec2) - ����
	m_dInsulationVStiffUF						= 0;		//TU�Ŵ��� ���� ������ ����÷��� �̰� �ɼ�

	for(int n = 0; n < OPT_CHKSHEET; n++)
	{
		m_bCalcDocCheck[n] = FALSE;
		m_nCalcDocSub[n] = 0;
		m_bCalcDocSub[n] = 0;
	}

	m_nSteelStandardYear						= APPLYYEAR2008;
}

void CGlobarOption::InitDesignQty()
{
	//���輳��	
	m_nSummaryNumberOfRound						= 4;		// ����ǥ ���߷� �ڸ��� (����)
	m_nSummaryUnitNumberOfRound					= 4;		// ����ǥ �����߷� �ڸ��� (����)
	m_nSummaryApplyBaseOfDesign					= 0;		// ����ǥ ������� �����(��.����)
	m_nSummaryApplyBaseOfAssm					= 1;		// ����ǥ ������� ���ۿ�
	m_nSummaryAddBoltWeight						= 1;		// �߷� ���� ��Ʈ
	m_nSummaryAddStudWeight						= 1;		// �߷� ���� ���ܿ�����
	m_nSummaryAddSlabAnchorWeight				= 1;		// �߷� ���� ������ ��Ŀ
	m_nSummaryAddGoriWeight						= 1;		// �߷� ���� ���	
	m_nSummaryAddSpliceWeight					= 1;		// �߷� ���� ���	
	m_nAssmUseCrossBeamBracketGirderOfBigAssm   = 1;
}

void CGlobarOption::Serialize(CArchive& ar)
{
//	long nFlag = 14;	//090826-�ں���
	long nFlag = 15;	//����DB����
	long vLong = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nDesignCondition;
		ar << m_nDesignBridgeType;                        
		ar << m_nDesignSlabType;                          
		ar << m_nDesignSlabConType;                       
		ar << m_nDesignGirderConnect;                     
		ar << m_nDesignUseHBeamAtCr; 
		ar << m_nDesignHeigtBaseUpper;
		ar << m_nDesignHeigtBaseLower;                     
		ar << vLong;
		//����
		ar << m_nDesignSteelChangeByThick;			
		ar << m_dDesignMinThickOfUDFlate;
		ar << m_dDesignMinThickOfUpFlate;
		ar << m_dDesignMinThickOfLoFlate;
		ar << m_dDesignMinThickOfWebFlate;
		ar << m_dDesignAllowMaxThick;						
		//��� 
		ar << m_dDesignMaxWeightOfCarLoad;				
		ar << m_dDesignMaxLengthOfFieldSplice;			
		//����
		ar << m_nDesignDimensionOfFilletWeld;				
		ar << m_dDesignMinLenOfFilletWeld;				
		ar << m_dDesignThickWeldNoCut;					
		ar << m_dDesignThickXWeldApply;					
		//��Ʈ��ġ
		ar << m_nDesignBoltHoleJigJaegOfHorPlate;			
		ar << m_nDesignBoltHoleJigJaegOfVerPlate;			
		ar << m_nDesignBoltHoleJigJaegOfOutCrossPlate;	
		ar << m_nDesignBoltHoleJigJaegOfOutBeamPlate;		
		ar << m_nDesignBoltHoleArrangeByunSectionType;	
		ar << m_nDesignBoltHoleArrangeVerticalBeam;		
		ar << m_nDesignBoltHoleArrangeCrossBeam;			
		ar << m_nDesignBoltHoleArrangeOutCrossBeam;		
		ar << m_nDesignBoltHoleArrangeOutBeam;			
		ar << m_dDesignBoltJongVSlopApplyUp;		
		ar << m_dDesignBoltJongVSlopApplyDn;	
		//���κ�
		ar << m_nDesignCrossBeamBracketType;				
		ar << m_nDesignCrossBeamHStiffAtBracket;			
		ar << vLong;              
		//����������
		ar << m_dSttExpansionJointHeight;					
		ar << m_dSttExpansionJointWidth;					
		ar << m_dEndExpansionJointHeight;					
		ar << m_dEndExpansionJointWidth;					
		//�����ȣ
		ar << m_nDesignStartNumberOfGirder;				
		ar << m_nDesignStartNumberOfSplice;				
		//��������
		ar << m_nDesignUnitTonfMeter;						
		ar << m_nDesignPointSlope;						
		ar << m_nDesignByundanVSlopRoundPos;				
		//��Ÿ
		ar << m_nDesignHBracingInstall;					
		ar << m_nDesignGirderEndType;							
		ar << m_nDesignStringerJijumApply;		
		ar << m_nDesignDanmyunCheck;
		//���輳��	
		ar << m_nSummaryNumberOfRound;					
		ar << m_nSummaryUnitNumberOfRound;				
		ar << m_nSummaryApplyBaseOfDesign;				
		ar << m_nSummaryApplyBaseOfAssm;					
		ar << m_nSummaryAddBoltWeight;					
		ar << m_nSummaryAddStudWeight;					
		ar << m_nSummaryAddSlabAnchorWeight;				
		ar << m_nSummaryAddGoriWeight;					
		ar << m_nSummaryAddSpliceWeight;
		ar << m_nAssmUseCrossBeamBracketGirderOfBigAssm;
		//��������
		ar << m_nDegreeEarthQuake;
		ar << m_nRegionEarthQuake;
		ar << m_dCoefficientGround;
		ar << m_dRiskCoefficient;
		ar << m_dZoneCoefficient;

		ar << m_nDesignVStiffInstall;
		ar << m_nDesignHStiffInstall;
		ar << m_nDesignHBracingDirChange;
		ar << m_bBoltLengthAuto;
		ar << (long)OPT_CHKSHEET;
		for(int n = 0; n < OPT_CHKSHEET; n++)
		{
			ar << m_bCalcDocCheck[n];
			ar << m_nCalcDocSub[n];
			ar << m_bCalcDocSub[n];
		}
		ar << m_nSlabCentDesignMethod;
		ar << m_dAccelerationOfGravity;
		ar << m_bDesignAllowDeflection;
		ar << m_bInstallBeamAtCenter;
		ar << m_nDesignAllowCrackWidthEnv;
		ar << m_dInsulationVStiffUF;
		ar << m_nSteelStandardYear;
	}
	else
	{
		ar >> nFlag;
		ar >> m_nDesignCondition;
		ar >> m_nDesignBridgeType;                        
		ar >> m_nDesignSlabType;                          
		ar >> m_nDesignSlabConType;                       
		ar >> m_nDesignGirderConnect;                     
		ar >> m_nDesignUseHBeamAtCr;     
		if(nFlag>3)	
		{
			ar >> m_nDesignHeigtBaseUpper;
			ar >> m_nDesignHeigtBaseLower;
		}
		if(nFlag>0)
			ar >> vLong;		
		//����
		ar >> m_nDesignSteelChangeByThick;			
		ar >> m_dDesignMinThickOfUDFlate;
		if(nFlag>11)
		{
			ar >> m_dDesignMinThickOfUpFlate;
			ar >> m_dDesignMinThickOfLoFlate;
		}
		else
		{
			m_dDesignMinThickOfUpFlate = m_dDesignMinThickOfUDFlate;
			m_dDesignMinThickOfLoFlate = m_dDesignMinThickOfUDFlate;
		}
		ar >> m_dDesignMinThickOfWebFlate;				
		ar >> m_dDesignAllowMaxThick;						
		//��� 
		ar >> m_dDesignMaxWeightOfCarLoad;				
		ar >> m_dDesignMaxLengthOfFieldSplice;			
		//����
		ar >> m_nDesignDimensionOfFilletWeld;				
		ar >> m_dDesignMinLenOfFilletWeld;				
		ar >> m_dDesignThickWeldNoCut;					
		ar >> m_dDesignThickXWeldApply;					
		//��Ʈ��ġ
		ar >> m_nDesignBoltHoleJigJaegOfHorPlate;			
		ar >> m_nDesignBoltHoleJigJaegOfVerPlate;			
		ar >> m_nDesignBoltHoleJigJaegOfOutCrossPlate;	
		ar >> m_nDesignBoltHoleJigJaegOfOutBeamPlate;		
		ar >> m_nDesignBoltHoleArrangeByunSectionType;	
		ar >> m_nDesignBoltHoleArrangeVerticalBeam;		
		ar >> m_nDesignBoltHoleArrangeCrossBeam;			
		ar >> m_nDesignBoltHoleArrangeOutCrossBeam;		
		ar >> m_nDesignBoltHoleArrangeOutBeam;			
		ar >> m_dDesignBoltJongVSlopApplyUp;		
		ar >> m_dDesignBoltJongVSlopApplyDn;	
		//���κ�
		ar >> m_nDesignCrossBeamBracketType;				
		ar >> m_nDesignCrossBeamHStiffAtBracket;			
		ar >> vLong;              
		//����������
		ar >> m_dSttExpansionJointHeight;					
		ar >> m_dSttExpansionJointWidth;					
		ar >> m_dEndExpansionJointHeight;					
		ar >> m_dEndExpansionJointWidth;					
		//�����ȣ
		ar >> m_nDesignStartNumberOfGirder;				
		ar >> m_nDesignStartNumberOfSplice;				
		//��������
		ar >> m_nDesignUnitTonfMeter;						
		ar >> m_nDesignPointSlope;						
		ar >> m_nDesignByundanVSlopRoundPos;				
		//��Ÿ
		ar >> m_nDesignHBracingInstall;					
		ar >> m_nDesignGirderEndType;							
		ar >> m_nDesignStringerJijumApply;				
		if(nFlag>1)
			ar >> m_nDesignDanmyunCheck;
		//���輳��	
		ar >> m_nSummaryNumberOfRound;					
		ar >> m_nSummaryUnitNumberOfRound;				
		ar >> m_nSummaryApplyBaseOfDesign;				
		ar >> m_nSummaryApplyBaseOfAssm;					
		ar >> m_nSummaryAddBoltWeight;					
		ar >> m_nSummaryAddStudWeight;					
		ar >> m_nSummaryAddSlabAnchorWeight;				
		ar >> m_nSummaryAddGoriWeight;					
		ar >> m_nSummaryAddSpliceWeight;
		ar >> m_nAssmUseCrossBeamBracketGirderOfBigAssm;

		//��������
		ar >> m_nDegreeEarthQuake;
		ar >> m_nRegionEarthQuake;
		ar >> m_dCoefficientGround;
		ar >> m_dRiskCoefficient;
		ar >> m_dZoneCoefficient;

		if(nFlag >2)
		{
			ar >> m_nDesignVStiffInstall;
			ar >> m_nDesignHStiffInstall;
		}
		if(nFlag >3)
		{
			ar >> m_nDesignHBracingDirChange;
		}
		if(nFlag >4) ar >> m_bBoltLengthAuto;
		else	m_bBoltLengthAuto = FALSE;

		if(nFlag > 5)
		{
			long nSheet;
			ar >> nSheet;
			for(int n = 0; n < nSheet; n++)
			{
				ar >> m_bCalcDocCheck[n];
				ar >> m_nCalcDocSub[n];
				if (nFlag>13)
					ar >> m_bCalcDocSub[n];
			}
		}
		if(nFlag > 6)
			ar >> m_nSlabCentDesignMethod;
		ar >> m_dAccelerationOfGravity;

		if(nFlag>8)
			ar >> m_bDesignAllowDeflection;
		if(nFlag>9)
			ar >> m_bInstallBeamAtCenter;
		if(nFlag>10)
			ar >> m_nDesignAllowCrackWidthEnv;
		if(nFlag>12)
			ar >> m_dInsulationVStiffUF;
		if(nFlag > 14)
			ar >> m_nSteelStandardYear;
	}
}

CString	CGlobarOption::GetStrDesignCondition()			
{
	if(m_nDesignCondition == 0) return "���α���";
//	if(m_nDesignCondition == 1) return "����ö����";
//	if(m_nDesignCondition == 2) return "��ö����(LS-22)";
//	if(m_nDesignCondition == 3) return "��ö����(LS-18)";
//	if(m_nDesignCondition == 4) return "���ö������";
//	if(m_nDesignCondition == 5) return "����ö����";
	return "";
}

CString	CGlobarOption::GetStrDesignBridgeType()
{
	if(m_nDesignBridgeType == 0)	return "������";
	if(m_nDesignBridgeType == 1)	return "�Ҽ��ְŴ���";
	if(m_nDesignBridgeType == 2)	return "Turnover�Ŵ���";
	return "";
}

CString CGlobarOption::GetStrDesignAllowCrackWidthEnv()
{
	if(m_nDesignAllowCrackWidthEnv == 0) return	"������ ȯ��";
	if(m_nDesignAllowCrackWidthEnv == 1) return	"�Ϲ� ȯ��";
	if(m_nDesignAllowCrackWidthEnv == 2) return	"�νļ� ȯ��";
	if(m_nDesignAllowCrackWidthEnv == 3) return	"�ؽ��� �νļ� ȯ��";
	return "";
}

double CGlobarOption::GetdCoeffDesignAllowCrackWidthEnv()
{
	double dCoeff = 0;
	if(m_nDesignAllowCrackWidthEnv == 0) dCoeff = 0.006;
	if(m_nDesignAllowCrackWidthEnv == 1) dCoeff = 0.005;
	if(m_nDesignAllowCrackWidthEnv == 2) dCoeff = 0.004;
	if(m_nDesignAllowCrackWidthEnv == 3) dCoeff = 0.0035;
	return dCoeff;
}

CString	CGlobarOption::GetStrDesignSlabType()
{
	if(m_nDesignSlabType== 0) return "RC Slab";
	if(m_nDesignSlabType == 1) return "PSC Slab";
	return "";
}

CString	CGlobarOption::GetStrDesignSlabConType()
{
	if(m_nDesignSlabConType == 0) return "��ũ�÷���Ʈ";
	if(m_nDesignSlabConType== 1) return "��Ǫ��";
	return "";
}

CString	CGlobarOption::GetStrDesignGirderConnect()
{
	if(m_nDesignGirderConnect == 0) return "��������";
	if(m_nDesignGirderConnect == 1) return "�������";

	return "";
}

CString CGlobarOption::GetStrDesignHBracingDirChange()
{
	if(m_nDesignHBracingDirChange== 0) return "�������";
	if(m_nDesignHBracingDirChange== 1) return "�������";
	return "";
}

CString	CGlobarOption::GetStrDesignUseHBeamAtCr()
{
	if(m_nDesignUseHBeamAtCr== 0) return "����";
	if(m_nDesignUseHBeamAtCr== 1) return "�������";

	return "";
}

CString CGlobarOption::GetStrBoltLengthAuto()
{
	if(m_bBoltLengthAuto)	return "����";
	else					return "�������";
}

CString CGlobarOption::GetStrSlabCentDesignMethod()
{
	if(m_nSlabCentDesignMethod == 0) return "���� �����";
	if(m_nSlabCentDesignMethod == 1) return "������ �����";
	if(m_nSlabCentDesignMethod == 2) return "LB-Deck ����";
	return "";
}

CString	CGlobarOption::GetStrDesignSteelChangeByThick	()			
{
	if(m_nDesignSteelChangeByThick == 0) return "�β��� ���� �ڵ� ��ȯ";
	if(m_nDesignSteelChangeByThick == 1) return "�β��� 520 ����";
	if(m_nDesignSteelChangeByThick == 2) return "�β��� ���� ��ȯ ����";
	return "";
}
CString	CGlobarOption::GetStrDesignDimensionOfFilletWeld()
{	
	if(m_nDesignDimensionOfFilletWeld == 0) return "���� ġ�� ����";
	if(m_nDesignDimensionOfFilletWeld == 1) return "�Ŵ��� �ִ� ġ������";	
	if(m_nDesignDimensionOfFilletWeld == 2) return "���� ���Ǻ� �ִ�ġ��";
	if(m_nDesignDimensionOfFilletWeld == 3) return "��ü 8mm ����";	
	if(m_nDesignDimensionOfFilletWeld == 4) return "Ȧ���β� �ø�(���� ġ��)";	
	if(m_nDesignDimensionOfFilletWeld == 5) return "Ȧ���β� �ø�(�ڽ��� �ִ�)";	
	if(m_nDesignDimensionOfFilletWeld == 6) return "Ȧ���β� �ø�(��������ũ��)";	
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfHorPlate()
{
	if(m_nDesignBoltHoleJigJaegOfHorPlate == 0) return "������� Ÿ��";
	if(m_nDesignBoltHoleJigJaegOfHorPlate == 1) return "�Ϲ�Ÿ��";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfVerPlate()
{
	if(m_nDesignBoltHoleJigJaegOfVerPlate == 0) return "������� Ÿ��";
	if(m_nDesignBoltHoleJigJaegOfVerPlate == 1) return "�Ϲ�Ÿ��";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfBracketFlange()
{
	if(m_nDesignBoltHoleJigJaegOfOutCrossPlate == 0) return "������� Ÿ��";
	if(m_nDesignBoltHoleJigJaegOfOutCrossPlate == 1) return "�Ϲ�Ÿ��";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfOutBeamPlate()
{
	if(m_nDesignBoltHoleJigJaegOfOutBeamPlate == 0) return "������� Ÿ��";
	if(m_nDesignBoltHoleJigJaegOfOutBeamPlate == 1) return "�Ϲ�Ÿ��";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleArrangeByunSectionType()
{ 
	if(m_nDesignBoltHoleArrangeByunSectionType == 0) return "��Ʈ �յ��ġ";
	if(m_nDesignBoltHoleArrangeByunSectionType == 1) return "��Ʈ �Ϲݹ�ġ";	
	if(m_nDesignBoltHoleArrangeByunSectionType == 2) return "��Ʈ �����ġ";	
	return "";
}
CString	CGlobarOption::GetStrDesignBoltHoleArrangeCrossBeam()
{ 
	if(m_nDesignBoltHoleArrangeCrossBeam == 0) return "��Ʈ �յ��ġ";
	if(m_nDesignBoltHoleArrangeCrossBeam == 1) return "��Ʈ �Ϲݹ�ġ";	
	if(m_nDesignBoltHoleArrangeCrossBeam == 2) return "��Ʈ �����ġ";	
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleArrangeVerticalBeam()
{ 
	if(m_nDesignBoltHoleArrangeVerticalBeam == 0) return "��Ʈ �յ��ġ";
	if(m_nDesignBoltHoleArrangeVerticalBeam == 1) return "��Ʈ �Ϲݹ�ġ";	
	if(m_nDesignBoltHoleArrangeVerticalBeam == 2) return "��Ʈ �����ġ";	
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleArrangeBracket	()
{ 
	if(m_nDesignBoltHoleArrangeOutCrossBeam == 0) return "��Ʈ �յ��ġ";
	if(m_nDesignBoltHoleArrangeOutCrossBeam == 1) return "��Ʈ �Ϲݹ�ġ";	
	if(m_nDesignBoltHoleArrangeOutCrossBeam == 2) return "��Ʈ �����ġ";	
	return "";
}
CString	CGlobarOption::GetStrDesignBoltHoleArrangeOutBeam()
{ 
	if(m_nDesignBoltHoleArrangeOutBeam == 0) return "��Ʈ �յ��ġ";
	if(m_nDesignBoltHoleArrangeOutBeam == 1) return "��Ʈ �Ϲݹ�ġ";	
	if(m_nDesignBoltHoleArrangeOutBeam == 2) return "��Ʈ �����ġ";	
	return "";
}

CString	CGlobarOption::GetStrDesignCrossBeamBracketType()
{ 
	if(m_nDesignCrossBeamBracketType == 0) return "�Ϲ�Ÿ��";
	if(m_nDesignCrossBeamBracketType == 1) return "����Ÿ��";		
	return "";
}

CString CGlobarOption::GetStrDesignCrossBeamHStiffAtBracket()
{
	if(m_nDesignCrossBeamHStiffAtBracket == 0) return "����";
	if(m_nDesignCrossBeamHStiffAtBracket == 1) return "�������";	
	return "";	
}

CString CGlobarOption::GetStrUnitTonfMeter()
{
	if(m_nDesignUnitTonfMeter == 0) return "Kgf, mm";
	if(m_nDesignUnitTonfMeter == 1) return "Tonf, M";
	return "";	
}


CString CGlobarOption::GetStrDesignHBracingInstall()
{
	if(m_nDesignHBracingInstall == 0) return "��� ��ġ";
	if(m_nDesignHBracingInstall == 1) return "�Ϻ� ��ġ";
	if(m_nDesignHBracingInstall == 2) return "��,�Ϻ� ��ġ";
	return "";
}

CString CGlobarOption::GetStrDesignGirderEndType()
{
	if(m_nDesignGirderEndType == 0) return "�߽��࿡ ����";
	if(m_nDesignGirderEndType== 1) return "�簢����� ����";

	return "";
}

CString CGlobarOption::GetStrDesignStringerJijumApply()
{
	if(m_nDesignStringerJijumApply == 0) return "����";
	if(m_nDesignStringerJijumApply == 1) return "�������";	
	return "";
}

CString CGlobarOption::GetStrDesignDanmyunCheck()
{
	if(m_nDesignDanmyunCheck == 0) return "�����Ŵ�,�����Ŵ� ��������";
	if(m_nDesignDanmyunCheck == 1) return "�Ŵ� �������� ����";	
	if(m_nDesignDanmyunCheck == 2) return "���Ŵ� ��������";	

	return "";
}


CString CGlobarOption::GetStrSummaryApplyBaseOfDesign()
{
	if(m_nSummaryApplyBaseOfDesign == 0) return "���ġ�� ��.�� ����";
	if(m_nSummaryApplyBaseOfDesign == 1) return "���ġ�� �Ŵ� �߾� ����";	
	return "";		
}

CString CGlobarOption::GetStrSummaryApplyBaseOfAssm()
{
	if(m_nSummaryApplyBaseOfAssm == 0) return "����ġ�� �¿� ����";
	if(m_nSummaryApplyBaseOfAssm == 1) return  "����ġ�� ���簢�� ����";
	if(m_nSummaryApplyBaseOfAssm == 2) return  "����ġ�� ���෮, �¿� ����";
	if(m_nSummaryApplyBaseOfAssm == 3) return  "����ġ�� ���෮, ���簢�� ����";
	return "";
}

CString CGlobarOption::GetStrDegreeEarthQuake()
{
	if(m_nDegreeEarthQuake == 0) return "����I ���";
	if(m_nDegreeEarthQuake == 1) return "����II���";
	if(m_nDegreeEarthQuake == 2) return "����Ư���";
	return "";
}

void CGlobarOption::SetStrDegreeEarthQuake(CString sType)
{
	long nType = 0;
	if(sType == "����I ���") nType = 0;
	if(sType == "����II���") nType = 1;
	if(sType == "����Ư���") nType = 2;
	SetDegreeEarthQuake(nType);
}

CString CGlobarOption::GetStrRegionEarthQuake()
{
	if(m_nRegionEarthQuake== 0) return "I ����";
	if(m_nRegionEarthQuake == 1) return "II����";
	return "";
}


CString CGlobarOption::GetStrDesignVStiffInstall()
{
	if(m_nDesignVStiffInstall== 0) return "��ġ��";
	if(m_nDesignVStiffInstall == 1) return "��ġ����";

	return "";
}


CString CGlobarOption::GetStrInstallBeamAtCenter()
{
	if(m_bInstallBeamAtCenter)
		return "����";
	else
		return "�������";	
}

CString CGlobarOption::GetStrDesignHStiffInstall()
{
	if(m_nDesignHStiffInstall== 0) return "��ġ��";
	if(m_nDesignHStiffInstall == 1) return "��ġ����";

	return "";
}


void CGlobarOption::SetStrRegionEarthQuake(CString sType)
{
	long nType = 0;
	if(sType == "I ����") nType = 0;
	if(sType == "II����") nType = 1;
	SetRegionEarthQuake(nType);
}

void CGlobarOption::SetStrDesignCondition(CString sType)
{
	long nType = 0;

	if(sType == "���α���") nType = 0;
//	if(sType == "����ö����") nType = 1;
//	if(sType == "��ö����(LS-22)") nType = 2;
//	if(sType == "��ö����(LS-18)") nType = 3;
//	if(sType == "���ö������") nType = 4;
//	if(sType == "����ö����") nType = 5;
	
	SetDesignCondition(nType);
}

void CGlobarOption::SetStrDesignBridgeType(CString sType)
{
	long nType = 0;

	if(sType == "������")			nType = 0;
	if(sType == "�Ҽ��ְŴ���")		nType = 1;
	if(sType == "Turnover�Ŵ���")	nType = 2;

	SetDesignBridgeType(nType);
}

void CGlobarOption::SetStrDesignSlabType(CString sType)
{
	long nType = 0;
	if(sType == "RC Slab") nType = 0;
	if(sType == "PSC Slab") nType = 1;

	SetDesignSlabType(nType);
}

void CGlobarOption::SetStrDesignSlabConType(CString sType)
{
	long nType = 0;
	if(sType == "��ũ�÷���Ʈ") nType = 0;
	if(sType == "��Ǫ��") nType = 1;

	SetDesignSlabConType(nType);
}

void CGlobarOption::SetStrDesignGirderConnect(CString sType)
{
	long nType = 0;
	if(sType == "��������") nType = 0;
	if(sType == "�������") nType = 1;

	SetDesignGirderConnect(nType);
}

void CGlobarOption::SetStrDesignUseHBeamAtCr(CString sType)
{
	long nType = 0;
	if(sType == "����") nType = 0;
	if(sType == "�������") nType = 1;

	SetDesignUseHBeamAtCr(nType);
}

void CGlobarOption::SetStrDesignSteelChangeByThick(CString sType)
{
	long nType = 0;
	if(sType == "�β��� ���� �ڵ� ��ȯ") nType = 0;
	if(sType == "�β��� 520 ����") nType = 1;
	if(sType == "�β��� ���� ��ȯ ����") nType = 2;

	SetDesignSteelChangeByThick(nType);
}

void CGlobarOption::SetStrDesignDimensionOfFilletWeld(CString sType)
{
	long nType = 0;
	if(sType == "���� ġ�� ����") nType = 0;
	if(sType == "�Ŵ��� �ִ� ġ������") nType = 1;	
	if(sType == "���� ���Ǻ� �ִ�ġ��") nType = 2;	
	if(sType == "��ü 8mm ����") nType = 3;	
	if(sType == "Ȧ���β� �ø�(���� ġ��)") nType = 4;
	if(sType == "Ȧ���β� �ø�(�Ŵ��� �ִ�)") nType = 5;
	if(sType == "Ȧ���β� �ø�(��������ũ��)") nType = 6;

	SetDesignDimensionOfFilletWeld(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfHorPlate(CString sType)
{
	long nType = 0;
	if(sType == "������� Ÿ��") nType = 0;
	if(sType == "�Ϲ�Ÿ��") nType = 1;		

	SetDesignBoltHoleJigJaegOfHorPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfVerPlate(CString sType)
{
	long nType = 0;
	if(sType == "������� Ÿ��") nType = 0;
	if(sType == "�Ϲ�Ÿ��") nType = 1;		

	SetDesignBoltHoleJigJaegOfVerPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfBracketFlange(CString sType)
{
	long nType = 0;
	if(sType == "������� Ÿ��") nType = 0;
	if(sType == "�Ϲ�Ÿ��") nType = 1;		

	SetDesignBoltHoleJigJaegOfOutCrossPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfOutBeamPlate(CString sType)
{
	long nType = 0;
	if(sType == "������� Ÿ��") nType = 0;
	if(sType == "�Ϲ�Ÿ��") nType = 1;		

	SetDesignBoltHoleJigJaegOfOutBeamPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeByunSectionType(CString sType)
{
	long nType = 0;
	if(sType == "��Ʈ �յ��ġ") nType = 0;
	if(sType == "��Ʈ �Ϲݹ�ġ") nType = 1;	
	if(sType == "��Ʈ �����ġ") nType = 2;	

	SetDesignBoltHoleArrangeByunSectionType(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeCrossBeam(CString sType)
{ 
	long nType = 0;
	if(sType == "��Ʈ �յ��ġ") nType = 0;
	if(sType == "��Ʈ �Ϲݹ�ġ") nType = 1;	
	if(sType == "��Ʈ �����ġ") nType = 2;	

	SetDesignBoltHoleArrangeCrossBeam(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeVerticalBeam(CString sType)
{ 
	long nType = 0;
	if(sType == "��Ʈ �յ��ġ") nType = 0;
	if(sType == "��Ʈ �Ϲݹ�ġ") nType = 1;	
	if(sType == "��Ʈ �����ġ") nType = 2;	

	SetDesignBoltHoleArrangeVerticalBeam(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeBracket(CString sType)
{
	long nType = 0;
	if(sType == "��Ʈ �յ��ġ") nType = 0;
	if(sType == "��Ʈ �����ġ") nType = 2;	
	if(sType == "��Ʈ �Ϲݹ�ġ") nType = 1;

	SetDesignBoltHoleArrangeOutCrossBeam(nType);
}

void CGlobarOption::SetStrDesignCrossBeamBracketType(CString sType)
{
	long nType = 0;
	if(sType == "�Ϲ�Ÿ��") nType = 0;
	if(sType == "����Ÿ��") nType = 1;
	
	SetDesignCrossBeamBracketType(nType);
}

void CGlobarOption::SetStrDesignCrossBeamHStiffAtBracket(CString sType)
{
	long nType = 0;
	if(sType == "����") nType = 0;
	if(sType == "�������") nType = 1;

	SetDesignCrossBeamHStiffAtBracket(nType);
}

void CGlobarOption::SetStrUnitTonfMeter(CString sType)
{
	long nType = 0;
	if(sType == "Kgf, mm")	nType = 0;
	if(sType == "Tonf, M")	nType = 1;

	SetDesignUnitTonfMeter(nType);
}

void CGlobarOption::SetStrDesignHBracingInstall(CString sType)
{
	long nType = 0;
	if(sType == "��� ��ġ") nType = 0;
	if(sType == "�Ϻ� ��ġ") nType = 1;
	if(sType == "��,�Ϻ� ��ġ") nType = 2;

	SetDesignHBracingInstall(nType);
}

void CGlobarOption::SetStrDesignGirderEndType(CString sType)
{
	long nType = 0;
	if(sType == "�߽��࿡ ����") nType = 0;
	if(sType == "�簢����� ����") nType = 1;

	SetDesignGirderEndType(nType);
}

void CGlobarOption::SetStrDesignStringerJijumApply(CString sType)
{
	long nType = 0;
	if(sType == "����") nType = 0;
	if(sType == "�������") nType = 1;

	SetDesignStringerJijumApply(nType);
}

void CGlobarOption::SetStrDesignDanmyunCheck(CString sType)
{
	long nType = 0;

	if(sType == "�����Ŵ�,�����Ŵ� ��������")	nType = 0;
	if(sType == "�Ŵ� �������� ����")			nType = 1;
	if(sType == "���Ŵ� ��������")			nType = 2;

	SetDesignDanmyunCheck(nType);
}

void CGlobarOption::SetStrSummaryApplyBaseOfDesign(CString sType)
{
	long nType = 0;
	if(sType == "���ġ�� ��.�� ����") nType = 0;
	if(sType == "���ġ�� �Ŵ� �߾� ����") nType = 1;

	SetSummaryApplyBaseOfDesign(nType);
}

void CGlobarOption::SetStrSummaryApplyBaseOfAssm(CString sType)
{
	long nType = 0;	
	if(sType == "����ġ�� �¿� ����")		nType = 0;
	if(sType == "����ġ�� ���簢�� ����")	nType = 1;
	if(sType == "����ġ�� ���෮, �¿� ����")	nType = 2;
	if(sType == "����ġ�� ���෮, ���簢�� ����")nType = 3;	

	SetSummaryApplyBaseOfAssm(nType);
}



void CGlobarOption::SetStrDesignVStiffInstall(CString sType)
{
	long nType = 0;	
	if(sType == "��ġ��")		nType = 0;
	if(sType == "��ġ����")		nType = 1;

	SetDesignVStiffInstall(nType);
}



void CGlobarOption::SetStrDesignHStiffInstall(CString sType)
{
	long nType = 0;	
	if(sType == "��ġ��")		nType = 0;
	if(sType == "��ġ����")		nType = 1;

	SetDesignHStiffInstall(nType);
}


void CGlobarOption::SetStrInstallBeamAtCenter(CString sType)
{
	BOOL bApply = FALSE;	
	if(sType == "����")			bApply = TRUE;
	if(sType == "�������")		bApply = FALSE;

	SetInstallBeamAtCenter(bApply);
}

void CGlobarOption::SetStrDesignAllowCrackWidthEnv(CString sType)
{
	long nType = 0;
	if(sType == "������ ȯ��")			nType = 0;
	if(sType == "�Ϲ� ȯ��")			nType = 1;
	if(sType == "�νļ� ȯ��")			nType = 2;
	if(sType == "�ؽ��� �νļ� ȯ��")	nType = 3;
	
	SetDesignAllowCrackWidthEnv(nType);
}

void CGlobarOption::SetStrDesignHBracingDirChange(CString sType)
{
	long nType = 0;	
	if(sType == "�������")		nType = 0;
	if(sType == "�������")		nType = 1;

	SetDesignHBracingDirChange(nType);
}

void CGlobarOption::SetStrBoltLengthAuto(CString sType)
{
	BOOL bBoltLengthAuto = TRUE;

	if(sType == "�������")
		bBoltLengthAuto = FALSE;
	
	SetBoltLengthAuto(bBoltLengthAuto);
}

void CGlobarOption::SetStrSlabCentDesignMethod(CString sType)
{
	long nType = 0;	
	if(sType == "���� �����")		nType = 0;
	if(sType == "������ �����")	nType = 1;
	if(sType == "LB-Deck ����")		nType = 2;

	SetSlabCentDesignMethod(nType);	
}


// �Ŵ����� ���Ϻ� ����
long CGlobarOption::GetDesignHeigtBase() 
{
	if(m_nDesignHeigtBaseUpper == 1 && m_nDesignHeigtBaseLower == 0)
		return 0;
	else if(m_nDesignHeigtBaseUpper == 0 && m_nDesignHeigtBaseLower == 1)
		return 1;
	else if(m_nDesignHeigtBaseUpper == 1 && m_nDesignHeigtBaseLower == 1)
		return 2;
	else if(m_nDesignHeigtBaseUpper == 0 && m_nDesignHeigtBaseLower == 0)
		return 3;

	return -1;
}

// �Ŵ����� ���Ϻ� ����
void CGlobarOption::SetDesignHeigtBase(long nUpperLowerVal)
{
	switch(nUpperLowerVal)
	{
	case 0 :
		m_nDesignHeigtBaseUpper = 1;
		m_nDesignHeigtBaseLower = 0;
		break;
	case 1:
		m_nDesignHeigtBaseUpper = 0;
		m_nDesignHeigtBaseLower = 1;
		break;
	case 2:
		m_nDesignHeigtBaseUpper = 1;
		m_nDesignHeigtBaseLower = 1;
		break;
	default:
		m_nDesignHeigtBaseUpper = 0;
		m_nDesignHeigtBaseLower = 0;
		break;
	}
}

// ��.���� ���� �β�
void CGlobarOption::SetDesignMinThickOfUDFlate(double dVal, BOOL bUpper)
{ 
	if(bUpper)
		m_dDesignMinThickOfUpFlate = dVal;
	else
		m_dDesignMinThickOfLoFlate = dVal;
}

double CGlobarOption::GetDesignUDFlangeMinThick(BOOL bUpper)
{
	if(bUpper)
		return m_dDesignMinThickOfUpFlate;
	else
		return m_dDesignMinThickOfLoFlate;
}

BOOL CGlobarOption::GetDesignAllowDeflection()
{
	return m_bDesignAllowDeflection;
}

void CGlobarOption::SetDesignAllowDeflection(CString sApply)
{
	if(sApply=="����")		m_bDesignAllowDeflection = TRUE;
	if(sApply=="�������")	m_bDesignAllowDeflection = FALSE;
}

// ���� �ݺ� ġ��
CString CGlobarOption::GetStrRepeat(long nQty, double dDis, BOOL bCalc)
{
	CString szText = _T("");

	switch(nQty)
	{
	case 0: break;
	case 1: szText.Format("%s", COMMA(dDis));	break;
	default:
		if(bCalc)
			szText.Format("%d@%s = %s", nQty, COMMA(dDis), COMMA(nQty*dDis));
		else
			szText.Format("%d@%s", nQty, COMMA(dDis));
		break;
	}

	return szText;
}

void CGlobarOption::SetStrSteelStandardYear(CString sYear)
{
	long nYear = 0;
	
	if(sYear == "���μ������2008")					nYear = APPLYYEAR2008;
	else if(sYear == "���μ������2010")				nYear = APPLYYEAR2010;
	
	SetSteelStandardYear(nYear);
}

long CGlobarOption::GetSteelStandardYear(CString sYear)
{
	long nYear = 0;
	
	if(sYear == "���μ������2008")			nYear = APPLYYEAR2008;
	else if(sYear == "���μ������2010")		nYear = APPLYYEAR2010;
	
	return nYear;
}

long CGlobarOption::GetSteelStandardYearComboIdx(CString sYear)
{
	long nYear = 0;
	
	if(sYear == "���μ������2008")			nYear = 0;
	else if(sYear == "���μ������2010")		nYear = 1;
	
	return nYear;
}

CString CGlobarOption::GetStrSteelStandardYear()
{
	if(m_nSteelStandardYear == APPLYYEAR2008 )		return	_T("���μ������2008");
	else if(m_nSteelStandardYear == APPLYYEAR2010)	return	_T("���μ������2010");
	
	return _T("");
}

CString CGlobarOption::GetStrSteelStandardYear(long nApplyYear)
{
	if(nApplyYear == APPLYYEAR2008 )		return	_T("���μ������2008");
	else if(nApplyYear == APPLYYEAR2010)	return	_T("���μ������2010");
	
	return _T("");
}

// ������ ������ �����Ϳ� APlate�� ������ �ּҸ� ����
void CGlobarOption::SetExpansionBlockJewon(CDesignExpansionJoint *pDesignExp)
{
	if(pDesignExp==NULL || pDesignExp->GetSize()<2)	return;

	CExpansionJointData *pExpDataStt	= pDesignExp->GetExpansionJointData(0);
	CExpansionJointData *pExpDataEnd	= pDesignExp->GetExpansionJointData(1);

	pExpDataStt->SetBlockJewon(POST_SECTION,	&m_dSttExpansionJointWidth, &m_dSttExpansionJointHeight);
	pExpDataEnd->SetBlockJewon(PRE_SECTION,		&m_dEndExpansionJointWidth, &m_dEndExpansionJointHeight);
}