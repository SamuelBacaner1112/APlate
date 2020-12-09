// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignFloor.h: interface for the CXLDesignFloor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNFLOOR_H__CE918F51_8262_410D_85C4_CA7E17B64CF9__INCLUDED_)
#define AFX_XLDESIGNFLOOR_H__CE918F51_8262_410D_85C4_CA7E17B64CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBridgeApp;
class CDataMange;
class CCalcData;
class CAPlateCalcStd;
class CAPlateDrawCalc;
class CAPlateDrawDanmyun;
class CPlateBasicIndex;
class CCentSeparation;
class CCalcFloor;

class CXLDesignFloor : public CAPlateOutCalcXL
{
public:
	CXLDesignFloor(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignFloor();

	CXLControl			*m_pXL;
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	
	//////////////////////////////////////////////////////////////////////////
	// ������ ���� ���
	///< 3.1 �߾�������
	void DesignFloor_Center();
	void DesignExperience();			// ������ ��������
	///< 3.2 �ٴ��Ǽ���
	void DesignFloor(BOOL bLeft);		///< TRUE : ���� ĵƿ������		FALSE : ���� ĵƿ������	
	///< 3.4 ������Ʈ������ �ս�
	long LossOfPreStress();
	///< 3.5 �ٴ��� ������ ���°���
	void SlabStressCheck(long &nRow);
	///< 3.7 �������߿� ���� �ܸ����
	void SectionCheckAboutUltimateLoad();
	///< 3.7 �ٴ��� ���� ����
	void CheckFloorReinforce(CString szSheetName);
	///< 3.4 ���������� �ٴ��� ����
	void CheckInnerJijum();
	// ����ĵƿ������(����������)
	void ExpansionJointUltimateLoad();
	///< 3.9 ������ ���� ����
	void DesignFixingReinforce();

	void CheckFloorRebarCalc(FloorType eType, CStringArray &ArrDelSheet, BOOL bNewSheet = TRUE);

	//////////////////////////////////////////////////////////////////////////
	// ������ �ִ� ���
	///< 3. �ٴ��� ����
	void TendonDesignFloor();
	///< 3.3 ���߻���
	void TendonLoadSum();
	///< 3.4 �ܸ�� ����(1)
	void TendonSectionSum();
	///< 3.5 ������Ʈ������ �ս�(1)
	void TendonPreStressLose();
	///< 3.6 �ٴ��� ������ ����
	void TendonFloorStress();
	///< 3.7 �������߿� ���� �ܸ����
	void TendonLimitStressCheck();
	///< 3.8 ������ ����
	void TendonSettlement();
	///< 3.9 ���� ������ ����� ����
	void TendonInnerTension();
	///< 3.11 �Ŵ� �ܺ� �ٴ����� ����
	void TendonGirderReinforce();

protected:
	void Initialize();

	// 3.1 ����ĵƿ������
	// 3.2 ����ĵƿ������
	// 1) ������
	void CalcDeadLoad();
	// 2) Ȱ����
	void CalcLiveLoad();
	// 3) ��������
	void CalcWalkLoad();
	// 4) �浹����
	void CalcCollisionLoad();
	// 5) ��ũ����
	void CalcTankLoad();
	// 6) Ʈ���Ϸ� ����
	void CalcTrailerLoad();
	// 7) ǳ����
	void CalcWindLoad();
	// 8) ��������
	void CalcCentriodLoad();
	// 9) ��������
	void CalcComposeLoad();
	// 10) ĵƿ���� �ܺμ���
	void DesignOfCantilDanbu();
	// 11) �ּҵΰ����
	void CheckMinThick(long nFloorIndex);
	// 12) �ܸ����
	void DesignOfSection(long nFloorIndex);
	void CalcReBarQuantity(long nFloorIndex);	// ö�ٷ� ���� - ���� ����
	void CaclRebarExperienceDesign();			// ö�ٷ� ���� - ������ ����
	void CheckUsefully(long nFloorIndex);		// ��뼺 ����
	
	// 1) ������
	void CalcDeadLoad_Center();
	// 2) Ȱ����
	void CalcLiveLoad_Center();
	// 3) ��ũ ����
	void CalcTankLoad_Center();
	// 4) Ʈ���Ϸ� ����
	void CalcTrailerLoad_Center();
	// 5) ��������
	void CalcCentriodLoad_Center();
	// 6) �������� - ���Ѹ��Ʈ���
	void CalcComposeLoad_Center(BOOL bCount = TRUE);
	// 7) �ܸ����
	void DrawReBarQuantity(CDomyun *pDom, long nFloorIndex);
	void DrawCrackWidthCheck(CDomyun *pDom, long nType);
	void DimCrackWidthCheck(CDomyun *pDom, long nType);

	double GetAs(long AsNum, CString &Case_H);
	double DiaReturn(long AsNum);
	double AreaReturn(long AsNum);

	BOOL				m_bLeft;	// ����ĵƿ������, ������ƿ������ ����
	CPlateBasicIndex	*m_pBx;
	CCentSeparation		*m_pSep;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	CStringArray		m_ArrayNumber;
	
	CStringArray		m_ArrayGuardWallH;
	CStringArray		m_ArrayGuardWallTopL;
	long				m_nSttRow;
	long				m_nNumber;
	long				m_nUltSttRow[3];	///< �������� ���� Row Number
	long				m_nDanRow[6];		// �ܺ� ��, �� ������ Row
	double				m_dPreMd;
	double				m_dPostMd;
	BOOL				m_bComposeOnlyOne;

	void SetLines(int nLine);
	void DimGuardWall();

	long				m_nW3;//�������߰���� ����...

	// �÷��� ��, �� �β�
	void SeekFlange_Web(double& dFlange, double& dWeb);

	// ������ġ�� ���� ������ġ �缳��
	void ReSetTendonDistance(CDPointArray& PosArr, long nPos, CDoubleArray& DisArr);

	// �Ÿ����ϱ�
	double GetDistance(CDPoint& p1, CDPoint& p2);

	// ��ġ��ó�����ϱ�
	double GetDeflection(long nPos, CString szLoad);
	void GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr);


	// ����ǥ��
	CDPointArray GetWallGuardPoint(CDPoint* pPoint);
	void DrawWallGuard(CDomyun *pDom, double dTextHeight=1);
	void DrawDimJoint(CDomyun *pDom, CFEMManage *pFEM, double dTextHeight=1, BOOL bElemNum=TRUE);
	void DrawDimLoadCase(CDomyun *pDom, CFEMManage *pFEM, long nCase, double dTextHeight=1);
	long GetLoadApplyMemberCount(CFEMManage* pFEM);
	long GetLoadCase(CString szLoad);
	long GetLoadCaseQty(CString szLoad);
	void DrawForceDiaghram(CDomyun *pDom, CFEMManage *pFem, long nCase, BOOL bTopDim, BOOL bBotDim);
};

#endif // !defined(AFX_XLDESIGNFLOOR_H__CE918F51_8262_410D_85C4_CA7E17B64CF9__INCLUDED_)
