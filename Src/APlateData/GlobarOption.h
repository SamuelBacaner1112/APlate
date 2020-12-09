// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// GlobarOption.h: interface for the CGlobarOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAROPTION_H__59D01DA1_A74D_485D_BE31_B60409D28442__INCLUDED_)
#define AFX_GLOBAROPTION_H__59D01DA1_A74D_485D_BE31_B60409D28442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OPT_CHKSHEET			9			// ��ü ������꼭 

class CDesignExpansionJoint;
class AFX_EXT_CLASS CGlobarOption  
{
public:
	CGlobarOption();
	virtual ~CGlobarOption();

	void Serialize(CArchive& ar);

private:
	// �⺻����(�������°���)
	long	m_nDesignCondition;							// ��������	
	long    m_nDesignBridgeType;                        // ��������    0 : �÷���Ʈ  1 : �Ҽ�����
	long    m_nDesignSlabType;                          // ����������  0 : RC        1 : PSC
	long    m_nDesignSlabConType;                       // ������ð����� 
	long    m_nDesignGirderConnect;                     // ������������0 : ����������1 : �������
	long    m_nDesignUseHBeamAtCr;                      // H�������	
	long	m_nDesignHeigtBaseUpper;					// �Ŵ����� ���� 0 : ��� ��� 1 : ��� �ϴ�
	long	m_nDesignHeigtBaseLower;					// �Ŵ����� ���� 0 : �ϸ� ��� 1 : �ϸ� �ϴ�
	long	m_nDesignHBracingDirChange;					// ����극�̽� ���� ���� ���� �ɼ�
	//�����β�����
	long	m_nDesignSteelChangeByThick;				// �β��� ������ȯ ����
	double  m_dDesignMinThickOfUDFlate;					// ��.���� �����β�  (mm) - nFlag 12���� �и�
	double	m_dDesignMinThickOfUpFlate;					// ���� �����β� (mm)
	double	m_dDesignMinThickOfLoFlate;					// ���� �����β� (mm)
	double	m_dDesignMinThickOfWebFlate;				// ������  �����β�  (mm)
	double	m_dDesignAllowMaxThick;						// �������� ���β� ����(mm)
	BOOL	m_bDesignAllowDeflection;					// �����β� ���� ���ġ ���뿩��

	//��ݰ���
	double  m_dDesignMaxWeightOfCarLoad;				// �������� �ִ��߷�   (kg)
	double  m_dDesignMaxLengthOfFieldSplice;			// �������� �ִ����    (mm)
	//��������
	long	m_nDesignDimensionOfFilletWeld;				// �ʷ� ���� ġ�� ����
	double	m_dDesignMinLenOfFilletWeld;				// �ʷ� ���� �ּұ���    (mm)
	double	m_dDesignThickWeldNoCut;					// ������ ���� ���� �β���   (mm)
	double	m_dDesignThickXWeldApply;					// X ���� ���� �β�    (mm)
	//��Ʈ��ġ����
	long	m_nDesignBoltHoleJigJaegOfHorPlate;			// ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
	long	m_nDesignBoltHoleJigJaegOfVerPlate;			// ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
	long	m_nDesignBoltHoleJigJaegOfOutCrossPlate;	// ��Ʈ��ġ ������� Ÿ�� �������κ� ��.����
	long	m_nDesignBoltHoleJigJaegOfOutBeamPlate;		// ��Ʈ��ġ ������� Ÿ�� ������ ��.����
	long	m_nDesignBoltHoleArrangeByunSectionType;	// ��Ʈ��ġ ������ ���ܸ� Ÿ��
	long	m_nDesignBoltHoleArrangeCrossBeam;			// ��Ʈ��ġ ������ ���κ�
	long	m_nDesignBoltHoleArrangeVerticalBeam;		// ��Ʈ��ġ ������ ���κ�
	long	m_nDesignBoltHoleArrangeOutCrossBeam;		// ��Ʈ��ġ ������ �������κ�
	long	m_nDesignBoltHoleArrangeOutBeam;			// ��Ʈ��ġ ������ ������
	double	m_dDesignBoltJongVSlopApplyUp;				// ��Ʈ��ġ ������ ��ΰ�� ���� (%)
	double	m_dDesignBoltJongVSlopApplyDn;				// ��Ʈ��ġ ������ �Ϻΰ�� ���� (%)
	BOOL	m_bBoltLengthAuto;							// ��� ��Ʈ ���� �ڵ����
	//���κ�����
	long	m_nDesignCrossBeamBracketType;				// ���κ� ����� ��� Ÿ��
	long	m_nDesignCrossBeamHStiffAtBracket;			// ���κ� ���򺸰��� �������ġ ����	
	//���������ΰ���
	double  m_dSttExpansionJointHeight;					// ���� ����������ġ ����   (mm)
	double  m_dSttExpansionJointWidth;					// ���� ����������ġ �ʺ�   (mm)
	double  m_dEndExpansionJointHeight;					// ���� ����������ġ ����   (mm)
	double  m_dEndExpansionJointWidth;					// ���� ����������ġ �ʺ�   (mm)
	//�����ȣ����
	long	m_nDesignStartNumberOfGirder;				// �Ŵ�   ���۹�ȣ
	long	m_nDesignStartNumberOfSplice;				// ������ ���۹�ȣ
	//��������
	long	m_nDesignUnitTonfMeter;						// �߷�,�Ÿ�,��ǥ ����
	long	m_nDesignPointSlope;						// ���� �� ���ܱ��� ǥ�� �ڸ���
	long	m_nDesignByundanVSlopRoundPos;				// ������ ������� ����(�ݿø� �ڸ���) : 2
	//��Ÿ����
	long	m_nDesignHBracingInstall;					// ���򺸰��� ��ġ Ÿ��
	long    m_nDesignGirderEndType;                     // ���� ����Ÿ��
	long	m_nDesignStringerJijumApply;				// ���κ� �ܺ� ������ ����
	long    m_nDesignDanmyunCheck;						// �ܸ����� �Ŵ����й��
	// ���輳�� 
	long	m_nSummaryApplyBaseOfDesign;				// ����ǥ ������� �����(��.����)
	long	m_nSummaryApplyBaseOfAssm;					// ����ǥ ������� ���ۿ�
	long	m_nSummaryNumberOfRound;					// ����ǥ ���߷� �ڸ��� (���� Kg)
	long	m_nSummaryUnitNumberOfRound;				// ����ǥ �����߷� �ڸ��� (���� Kg)
	long	m_nSummaryAddBoltWeight;					// �߷� ���� ��Ʈ
	long	m_nSummaryAddStudWeight;					// �߷� ���� ���ܿ�����
	long	m_nSummaryAddSlabAnchorWeight;				// �߷� ���� ������ ��Ŀ
	long	m_nSummaryAddGoriWeight;					// �߷� ���� ���
	long    m_nSummaryAddSpliceWeight;                  // �������� ����� �������� ����
	long	m_nAssmUseCrossBeamBracketGirderOfBigAssm;	// ������ ��ũ ���κ� ����� �ڽ��� ���

	long	m_nDegreeEarthQuake;						// ������� (1,2,Ư)
	long	m_nRegionEarthQuake;						// �������� (1,2)
	long	m_dCoefficientGround;
	double	m_dRiskCoefficient;							///< ���赵 ���
	double	m_dZoneCoefficient;							///< �������
	
	long    m_nDesignVStiffInstall;						// ���κ����̿� ���������� ��ġ
	long    m_nDesignHStiffInstall;						// ���򺸰��� ��ġ
	long	m_nSlabCentDesignMethod;					// �ٴ��� �߾Ӻ� ö�� �����
	double	m_dAccelerationOfGravity;					// �߷°��ӵ�(m/sec2)
	BOOL	m_bInstallBeamAtCenter;						// �극�̽� ������ ������ ��ġ
	long	m_nDesignAllowCrackWidthEnv;				// ������ �νĿ� ���� ȯ������ (���տ���) - ARoad �߰� 2005/05/11, APlate �߰� 2007/04/19
	double	m_dInsulationVStiffUF;						// TU�Ŵ��� ���� ������ ����÷��� �̰� �ɼ�

	//�������
	long	m_nSteelStandardYear;						//�������
public:
	BOOL m_bCalcDocCheck[OPT_CHKSHEET];	//������꼭 ��� (���̾�α� �ɼ�)
	long m_nCalcDocSub[OPT_CHKSHEET];	//������꼭 ��� (���̾�α� �ɼ�)
	BOOL m_bCalcDocSub[OPT_CHKSHEET];	//������꼭 ��� (���̾�α� �ɼ�)

public:
	void Init();
	void InitDesignBasic();  //�����Է�(�ǰ��)
	void InitDesignQty();    //����ǥ(�ǰ��)
	///////////////////////////////////////////////////////////////////////////////////////////////
	// �⺻����(�������°���)
	long	GetDesignCondition()				{ return	m_nDesignCondition;}   // ��������	
	long	GetDesignBridgeType()				{ return	m_nDesignBridgeType;}  // �������� 	
	long	GetDesignSlabType()					{ return	m_nDesignSlabType;}    // ����������
	long	GetDesignSlabConType()				{ return	m_nDesignSlabConType;} // ������ð����� 
	long	GetDesignGirderConnect()			{ return	m_nDesignGirderConnect;} // ������������
	long	GetDesignUseHBeamAtCr()				{ return	m_nDesignUseHBeamAtCr; } // H�������	
	long	GetDesignHeigtBaseUpper()			{ return	m_nDesignHeigtBaseUpper; }
	long	GetDesignHeigtBaseLower()			{ return	m_nDesignHeigtBaseLower; }
	long	GetDesignHeigtBase(); // �Ŵ����� ���Ϻ� ����
	long	GetDesignHBracingDirChange()		{ return	m_nDesignHBracingDirChange; } // ����극�̽� ���� ���� ���� �ɼ�
 
	//�����β�����
	long	GetDesignSteelChangeByThick()		{ return	m_nDesignSteelChangeByThick; }// �β��� ������ȯ ����
	double	GetDesignUDFlangeMinThick(BOOL bUpper);	// ��.���� ���� �β�
	double	GetDesignWebFalngeMinThick()		{ return	m_dDesignMinThickOfWebFlate; }      // ������ �÷��� ���� �β�
	double	GetDesignAllowMaxThick()			{ return	m_dDesignAllowMaxThick; }           // �������� ���β� ����(mm)
	//��ݰ���
	double  GetDesignMaxWeightOfCarLoad()		{ return	m_dDesignMaxWeightOfCarLoad; }    // �������� �ִ��߷�
	double	GetDesignMaxLengthOfFieldSplice()	{ return	m_dDesignMaxLengthOfFieldSplice; }// �������� �ִ����	
	//��������
	long	GetDesignDimensionOfFilletWeld()	{ return	m_nDesignDimensionOfFilletWeld; }// �ʷ� ���� ġ�� ����
	double	GetDesignMinLenOfFilletWeld()		{ return	m_dDesignMinLenOfFilletWeld;	}// �ʷ� ���� �ּұ���    (mm)
	double	GetDesignThickWeldNoCut()			{ return	m_dDesignThickWeldNoCut;	}    //������ ���� ���� �β���   (mm)
	double	GetDesignThickXWeldApply()			{ return	m_dDesignThickXWeldApply;	}    //X ���� ���� �β�    (mm)
	//��Ʈ��ġ����
	long	GetDesignBoltHoleJigJaegOfHorPlate()	 { return	m_nDesignBoltHoleJigJaegOfHorPlate; }      // ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
	long	GetDesignBoltHoleJigJaegOfVerPlate()	 { return	m_nDesignBoltHoleJigJaegOfVerPlate; }      // ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
	long	GetDesignBoltHoleJigJaegOfBracketFlange(){ return	m_nDesignBoltHoleJigJaegOfOutCrossPlate; } // ��Ʈ��ġ ������� Ÿ�� �������κ� ��.����
	long	GetDesignBoltHoleJigJaegOfOutBeamPlate() { return   m_nDesignBoltHoleJigJaegOfOutBeamPlate; }// ��Ʈ��ġ ������� Ÿ�� ������ ��.����
	long	GetDesignBoltHoleArrangeByunSectionType(){ return	m_nDesignBoltHoleArrangeByunSectionType; } // ��Ʈ��ġ ������ ���ܸ� Ÿ��
	long	GetDesignBoltHoleArrangeCrossBeam()		 { return	m_nDesignBoltHoleArrangeCrossBeam; }       // ��Ʈ��ġ ������ ���κ�
	long	GetDesignBoltHoleArrangeVerticalBeam()	 { return   m_nDesignBoltHoleArrangeVerticalBeam; }    // ��Ʈ��ġ ������ ���κ�
	long	GetDesignBoltHoleArrangeBracket()		 { return	m_nDesignBoltHoleArrangeOutCrossBeam; }    // ��Ʈ��ġ ������ �������κ�
	long	GetDesignBoltHoleArrangeOutBeam()		 { return	m_nDesignBoltHoleArrangeOutBeam; }         // ��Ʈ��ġ ������ ������
	double	GetDesignBoltJongVSlopApplyUp()			 { return	m_dDesignBoltJongVSlopApplyUp;	}  // ��Ʈ��ġ ������ ��ΰ�� ���� (%)
	double	GetDesignBoltJongVSlopApplyDn()			 { return	m_dDesignBoltJongVSlopApplyDn;} // ��Ʈ��ġ ������ �Ϻΰ�� ���� (%)
	BOOL	GetBoltLengthAuto()						 { return	m_bBoltLengthAuto;}				// ��� ��Ʈ ���� �ڵ����
	//���κ�����
	long	GetDesignCrossBeamBracketType()		{ return	m_nDesignCrossBeamBracketType; }
	long	GetDesignCrossBeamHStiffAtBracket()	{ return	m_nDesignCrossBeamHStiffAtBracket; }	// ���򺸰��� ����� ��ġ ����	
	//���������ΰ���
	double  GetSttExpansionJointHeight()        { return	m_dSttExpansionJointHeight;}
	double  GetSttExpansionJointWidth()	        { return	m_dSttExpansionJointWidth;}
	double  GetEndExpansionJointHeight()        { return	m_dEndExpansionJointHeight;}
	double  GetEndExpansionJointWidth()	        { return	m_dEndExpansionJointWidth;}
	//�������
	long	GetDesignStartNumberOfGirder()		{ return	m_nDesignStartNumberOfGirder; }
	long	GetDesignStartNumberOfSplice()		{ return	m_nDesignStartNumberOfSplice; }
	//��������
	long	GetDesignUnitTonfMeter()			{ return	m_nDesignUnitTonfMeter; }
	long	GetDesignPointSlope()				{ return	m_nDesignPointSlope; }
	long	GetDesignByundanVSlopRoundPos()	 	{ return	m_nDesignByundanVSlopRoundPos; }
	//��Ÿ����	
	long	GetDesignHBracingInstall()			{ return	m_nDesignHBracingInstall; }
	long	GetDesignGirderEndType()			{ return	m_nDesignGirderEndType; }
	long	GetDesignStringerJijumApply()		{ return	m_nDesignStringerJijumApply; }
	long 	GetDesignDanmyunCheck()				{ return	m_nDesignDanmyunCheck;}
	//���輳�� 
	long	GetSummaryApplyBaseOfDesign()		{ return	m_nSummaryApplyBaseOfDesign; }
	long	GetSummaryApplyBaseOfAssm()			{ return	m_nSummaryApplyBaseOfAssm; }
	long	GetSummaryAddBoltWeight()			{ return	m_nSummaryAddBoltWeight; }
	long	GetSummaryAddStudWeight()			{ return	m_nSummaryAddStudWeight; }	
	long	GetSummaryAddSlabAnchorWeight()		{ return	m_nSummaryAddSlabAnchorWeight; }
	long	GetSummaryAddGoriWeight()			{ return	m_nSummaryAddGoriWeight; }
	long	GetSummaryAddSpliceWeight()			{ return	m_nSummaryAddSpliceWeight; }
	long	GetAssmUseCrossBeamBracketGirderOfBigAssm(){return	m_nAssmUseCrossBeamBracketGirderOfBigAssm; }
	
	long	GetPointValueOfGangJae()			{ return	m_nSummaryNumberOfRound;}		// ����ǥ ���߷� �ڸ��� (���� Kg)
	long	GetPointValueOfUnitGangJae()		{ return	m_nSummaryUnitNumberOfRound;}	// ����ǥ �����߷� �ڸ��� (���� Kg)
	// ����
	long	GetIdxDegreeEarthQuake()				{ return m_nDegreeEarthQuake;	};
	long	GetIdxRegionEarthQuake()				{ return m_nRegionEarthQuake;	};
	CString GetStrRegionEarthQuake();
	CString GetStrDegreeEarthQuake();
	double GetCoefficientGround()					{ return m_dCoefficientGround; }
	double GetRiskCoefficient()						{ return m_dRiskCoefficient; }
	double GetZoneCoefficient()						{ return m_dZoneCoefficient; }
	long	GetDesignVStiffInstall()				{ return m_nDesignVStiffInstall; }
	long	GetDesignHStiffInstall()				{ return m_nDesignHStiffInstall; }
	long	GetSlabCentDesignMethod()				{ return m_nSlabCentDesignMethod;}//�ٴ��� �߾Ӻ� ö�� �����
	BOOL	IsLBDeck()								{ return m_nSlabCentDesignMethod==2 ? TRUE : FALSE; }
	double	GetAccelerationOfGravity()				{ return m_dAccelerationOfGravity;}	
	BOOL    GetInstallBeamAtCenter()				{ return m_bInstallBeamAtCenter;}
	long	GetDesignAllowCrackWidthEnv()			{ return m_nDesignAllowCrackWidthEnv;}	
	double  GetInsulationVStiffUF()					{ return m_dInsulationVStiffUF;}
	long	GetSteelStandardYear()					{return m_nSteelStandardYear;}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// �⺻����(�������°���)
	void SetDesignCondition(long nVal)				{ m_nDesignCondition = nVal;}
	void SetDesignBridgeType(long nVal)				{ m_nDesignBridgeType = nVal;}  // �������� 	
	void SetDesignSlabType(long nVal)				{ m_nDesignSlabType = nVal;}    // ����������
	void SetDesignSlabConType(long nVal)			{ m_nDesignSlabConType = nVal;} // ������ð����� 
	void SetDesignGirderConnect(long nVal)			{ m_nDesignGirderConnect = nVal;} // ������������
	void SetDesignUseHBeamAtCr(long nVal)			{ m_nDesignUseHBeamAtCr = nVal;}  // H�������	
	void SetDesignHeigtBaseUpper(long nVal)			{ m_nDesignHeigtBaseUpper = nVal; }
	void SetDesignHeigtBaseLower(long nVal)			{ m_nDesignHeigtBaseLower = nVal; }
	void SetDesignHeigtBase(long nUpperLowerVal);
	void SetDesignHBracingDirChange(long nVal)		{ m_nDesignHBracingDirChange = nVal; } // ����극�̽� ���� ���� ���� �ɼ�
	//�����β�����
	void SetDesignSteelChangeByThick(long nVal)		{ m_nDesignSteelChangeByThick = nVal; }// �β��� ������ȯ ����
	void SetDesignMinThickOfUDFlate(double dVal, BOOL bUpper);	// ��.���� ���� �β�
	void SetDesignMinThickOfWebFlate(double dVal)	{ m_dDesignMinThickOfWebFlate = dVal; }// ������ �÷��� ���� �β�
	void SetDesignAllowMaxThick(double dVal)		{ m_dDesignAllowMaxThick = dVal; }
	//��ݰ���
	void SetDesignMaxWeightOfCarLoad(double dVal)	{ m_dDesignMaxWeightOfCarLoad = dVal; }// �������� �ִ��߷�
	void SetDesignMaxLengthOfFieldSplice(double dVal){ m_dDesignMaxLengthOfFieldSplice = dVal; }// �������� �ִ����		
	//��������
	void SetDesignDimensionOfFilletWeld(long nD)	{ m_nDesignDimensionOfFilletWeld = nD; }	
	void SetDesignMinLenOfFilletWeld(double dVal)	{ m_dDesignMinLenOfFilletWeld = dVal;	}// �ʷ� ���� �ּҵβ�
	void SetDesignThickWeldNoCut(double dVal)		{ m_dDesignThickWeldNoCut = dVal;	}// �ʷ� ���� �ּҵβ�
	void SetDesignThickXWeldApply(double dVal)		{ m_dDesignThickXWeldApply = dVal;	}// �ʷ� ���� �ּҵβ�
	//��Ʈ��ġ����
	void SetDesignBoltHoleJigJaegOfHorPlate(long nVal){ m_nDesignBoltHoleJigJaegOfHorPlate = nVal; }
	void SetDesignBoltHoleJigJaegOfVerPlate(long nVal){ m_nDesignBoltHoleJigJaegOfVerPlate = nVal; }
	void SetDesignBoltHoleJigJaegOfOutCrossPlate(long nVal)	{ m_nDesignBoltHoleJigJaegOfOutCrossPlate = nVal; }
	void SetDesignBoltHoleJigJaegOfOutBeamPlate(long nVal)  { m_nDesignBoltHoleJigJaegOfOutBeamPlate = nVal; }
	void SetDesignBoltHoleArrangeByunSectionType(long nVal)	{ m_nDesignBoltHoleArrangeByunSectionType = nVal; }// ������ ���� ���� ���ܸ� ��Ʈ ��ġ
	void SetDesignBoltHoleArrangeCrossBeam(long nVal)	 { m_nDesignBoltHoleArrangeCrossBeam = nVal; }
	void SetDesignBoltHoleArrangeVerticalBeam(long nVal) { m_nDesignBoltHoleArrangeVerticalBeam = nVal; }
	void SetDesignBoltHoleArrangeOutCrossBeam(long nVal) { m_nDesignBoltHoleArrangeOutCrossBeam = nVal; }// �������κ� ������ ��Ʈ ��ġ
	void SetDesignBoltHoleArrangeOutBeam(long nVal)	{ m_nDesignBoltHoleArrangeOutBeam = nVal; }
	void SetDesignBoltJongVSlopApplyUp(double dVal)	{ m_dDesignBoltJongVSlopApplyUp = dVal;	}// ������ ������ ��ΰ�� ���� (3%)  
	void SetDesignBoltJongVSlopApplyDn(double dVal)	{ m_dDesignBoltJongVSlopApplyDn = dVal; } // ������ ������ �Ϻΰ�� ���� (3%)
	void SetBoltLengthAuto(BOOL bD)				    { m_bBoltLengthAuto = bD;	}// ��� ��Ʈ ���� �ڵ����
	//���κ����� 
	void SetDesignCrossBeamBracketType(long nD)		 { m_nDesignCrossBeamBracketType = nD; }	
	void SetDesignCrossBeamHStiffAtBracket(long nVal){ m_nDesignCrossBeamHStiffAtBracket = nVal; }	// ���򺸰��� ����� ��ġ ����	
	//���������ΰ���
	void SetSttExpansionJointHeight(double dVal)    { m_dSttExpansionJointHeight = dVal;}
	void SetSttExpansionJointWidth(double dVal)	    { m_dSttExpansionJointWidth = dVal;}
	void SetEndExpansionJointHeight(double dVal)    { m_dEndExpansionJointHeight = dVal;}
	void SetEndExpansionJointWidth(double dVal)	    { m_dEndExpansionJointWidth = dVal;}
	//�������
	void SetDesignStartNumberOfGirder(long nVal)	{ m_nDesignStartNumberOfGirder = nVal; }
	void SetDesignStartNumberOfSplice(long nVal)	{ m_nDesignStartNumberOfSplice = nVal; }
	//��������
	void SetDesignUnitTonfMeter(long nVal)			{ m_nDesignUnitTonfMeter = nVal; }
	void SetDesignPointSlope(long nVal)				{ m_nDesignPointSlope = nVal; }
	void SetDesignByundanVSlopRoundPos(long nVal)	{ m_nDesignByundanVSlopRoundPos = nVal; }
	//��Ÿ����
	void SetDesignHBracingInstall(long nVal)		{ m_nDesignHBracingInstall = nVal; }
	void SetDesignGirderEndType(long nVal)          { m_nDesignGirderEndType = nVal;}
	void SetDesignStringerJijumApply(long nVal)		{ m_nDesignStringerJijumApply = nVal; }
	void SetDesignDanmyunCheck(long nVal)			{ m_nDesignDanmyunCheck       = nVal;}
	//���輳��
	void SetSummaryApplyBaseOfDesign(long nD)		{ m_nSummaryApplyBaseOfDesign = nD; }
	void SetSummaryApplyBaseOfAssm(long nD)			{ m_nSummaryApplyBaseOfAssm = nD; }
	void SetSummaryAddBoltWeight(long nD)			{ m_nSummaryAddBoltWeight = nD; }
	void SetSummaryAddStudWeight(long nD)			{ m_nSummaryAddStudWeight = nD; }
	void SetSummaryAddSlabAnchorWeight(long nD)		{ m_nSummaryAddSlabAnchorWeight = nD; }
	void SetSummaryAddGoriWeight(long nD)			{ m_nSummaryAddGoriWeight = nD;}
	void SetSummaryAddSpliceWeight(long nD)			{ m_nSummaryAddSpliceWeight = nD;}
	void SetAssmUseCrossBeamBracketBoxOfBigAssm(long nD) { m_nAssmUseCrossBeamBracketGirderOfBigAssm = nD; }

	void SetPointValueOfGangJae(long nD)			{ m_nSummaryNumberOfRound = nD;}
	void SetPointValueOfUnitGangJae(long nD)		{ m_nSummaryUnitNumberOfRound = nD;}
	void SetStrDegreeEarthQuake(CString sType);
	void SetStrRegionEarthQuake(CString sType);
	void SetDegreeEarthQuake(long nD)				{ m_nDegreeEarthQuake = nD; }
	void SetRegionEarthQuake(long nD)				{ m_nRegionEarthQuake = nD; }
	void SetCoefficientGround(double dCoefficientGround)	{ m_dCoefficientGround = (long)dCoefficientGround;}
	void SetRiskCoefficient(double dRiskCoefficient)		{ m_dRiskCoefficient = dRiskCoefficient;}
	void SetZoneCoefficient(double dZoneCoefficient)		{ m_dZoneCoefficient = dZoneCoefficient;}
	void SetDesignVStiffInstall(long nDesignVStiffInstall)		{ m_nDesignVStiffInstall = nDesignVStiffInstall;}
	void SetDesignHStiffInstall(long nDesignHStiffInstall)		{ m_nDesignHStiffInstall = nDesignHStiffInstall;}
	void SetInstallBeamAtCenter(BOOL bInstallBeamAtCenter)		{ m_bInstallBeamAtCenter = bInstallBeamAtCenter;}
	void SetDesignAllowCrackWidthEnv(long nD)					{ m_nDesignAllowCrackWidthEnv = nD;	}
	void SetInsulationVStiffUF(double dInsulationVStiffUF)		{ m_dInsulationVStiffUF = dInsulationVStiffUF;}
	void	SetSteelStandardYear(long nYear)					{ m_nSteelStandardYear = nYear;}
	///////////////////////////////////////////////////////////////////////////////////////////////
	CString GetStrRepeat(long nQty, double dDis, BOOL bCalc=FALSE);
	CString	GetStrDesignCondition();	
	CString	GetStrDesignBridgeType();
	CString	GetStrDesignSlabType();
	CString	GetStrDesignSlabConType();
	CString	GetStrDesignGirderConnect();
	CString	GetStrDesignUseHBeamAtCr();
	CString	GetStrDesignSlabStt();
	CString GetStrDesignHBracingDirChange();
	//
	CString	GetStrDesignSteelChangeByThick();					
	//
	CString	GetStrDesignDimensionOfFilletWeld();		
	//
	CString	GetStrDesignBoltHoleJigJaegOfHorPlate();
	CString	GetStrDesignBoltHoleJigJaegOfVerPlate();
	CString	GetStrDesignBoltHoleJigJaegOfBracketFlange();
	CString	GetStrDesignBoltHoleJigJaegOfOutBeamPlate();
	CString	GetStrDesignBoltHoleArrangeByunSectionType ();
	CString	GetStrDesignBoltHoleArrangeCrossBeam();
	CString	GetStrDesignBoltHoleArrangeVerticalBeam();
	CString	GetStrDesignBoltHoleArrangeBracket();
	CString	GetStrDesignBoltHoleArrangeOutBeam();
	//
	CString	GetStrDesignCrossBeamBracketType();
	CString	GetStrDesignCrossBeamHStiffAtBracket();
	CString GetStrDesignCrossBeamHStiffSlope();
	//
	CString GetStrUnitTonfMeter();
	//
	CString	GetStrDesignHBracingInstall();
	CString	GetStrDesignGirderEndType();
	CString	GetStrDesignStringerJijumApply();
	CString GetStrDesignDanmyunCheck();
	CString GetStrDesignVStiffInstall();
	CString GetStrDesignHStiffInstall();
	//
	CString	GetStrSummaryApplyBaseOfDesign();
	CString	GetStrSummaryApplyBaseOfAssm();
	//
	CString GetStrBoltLengthAuto();//��� ��Ʈ���� �ڵ����
	CString GetStrSlabCentDesignMethod();//�ٴ��� �߾Ӻ� ö�� �����
	CString GetStrInstallBeamAtCenter();	
	CString GetStrDesignAllowCrackWidthEnv();
	double	GetdCoeffDesignAllowCrackWidthEnv();
	///////////////////////////////////////////////////////////////////////////////////////////////
	void	SetStrDesignCondition(CString sType);		
	void	SetStrDesignBridgeType(CString sType);	
	void	SetStrDesignSlabType(CString sType);	
	void	SetStrDesignSlabConType(CString sType);	
	void	SetStrDesignGirderConnect(CString sType);	
	void	SetStrDesignUseHBeamAtCr(CString sType);		
	//
	void	SetStrDesignSteelChangeByThick(CString sType);						
	//
	void	SetStrDesignDimensionOfFilletWeld(CString sType);			
	//
	void	SetStrDesignBoltHoleJigJaegOfHorPlate(CString sType);	
	void	SetStrDesignBoltHoleJigJaegOfVerPlate(CString sType);	
	void	SetStrDesignBoltHoleJigJaegOfBracketFlange(CString sType);	
	void	SetStrDesignBoltHoleJigJaegOfOutBeamPlate(CString sType);	
	void	SetStrDesignBoltHoleArrangeByunSectionType(CString sType);	
	void	SetStrDesignBoltHoleArrangeCrossBeam(CString sType);	
	void	SetStrDesignBoltHoleArrangeVerticalBeam(CString sType);	
	void	SetStrDesignBoltHoleArrangeBracket(CString sType);	
	void	SetStrDesignBoltHoleArrangeOutBeam(CString sType);	
	//
	void	SetStrDesignCrossBeamBracketType(CString sType);	
	void	SetStrDesignCrossBeamHStiffAtBracket(CString sType);		
	//
	void    SetStrUnitTonfMeter(CString sType);	
	//
	void	SetStrDesignHBracingInstall(CString sType);	
	void	SetStrDesignGirderEndType(CString sType);	
	void	SetStrDesignStringerJijumApply(CString sType);	
	void    SetStrDesignDanmyunCheck(CString sType);
	//
	void	SetStrSummaryApplyBaseOfDesign(CString sType);
	void	SetStrSummaryApplyBaseOfAssm(CString sType);
	void	SetStrDesignVStiffInstall(CString sType);
	void	SetStrDesignHStiffInstall(CString sType);
	void    SetStrDesignHBracingDirChange(CString sType);
	void	SetStrBoltLengthAuto(CString sType);
	void	SetStrSlabCentDesignMethod(CString sType);
	void	SetSlabCentDesignMethod(long nData)			{ m_nSlabCentDesignMethod = nData;}
	void	SetAccelerationOfGravity(double dData)		{ m_dAccelerationOfGravity = dData;}
	void    SetStrInstallBeamAtCenter(CString sType);
	void    SetStrDesignAllowCrackWidthEnv(CString sType);

	BOOL	GetDesignAllowDeflection();
	void	SetDesignAllowDeflection(CString sApply);

	void	SetStrSteelStandardYear(CString sYear);
	long	GetSteelStandardYear(CString sYear);
	long	GetSteelStandardYearComboIdx(CString sYear);
	CString	GetStrSteelStandardYear();
	CString GetStrSteelStandardYear(long nApplyYear);

	void	SetExpansionBlockJewon(CDesignExpansionJoint *pDesignExp);
};

#endif // !defined(AFX_GLOBAROPTION_H__59D01DA1_A74D_485D_BE31_B60409D28442__INCLUDED_)
