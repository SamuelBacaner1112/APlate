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
	// 강선이 없는 경우
	///< 3.1 중앙지간부
	void DesignFloor_Center();
	void DesignExperience();			// 경험적 설계검토시
	///< 3.2 바닥판설계
	void DesignFloor(BOOL bLeft);		///< TRUE : 좌측 캔틸레버부		FALSE : 우측 캔틸레버부	
	///< 3.4 프리스트레스의 손실
	long LossOfPreStress();
	///< 3.5 바닥판 슬래브 응력검토
	void SlabStressCheck(long &nRow);
	///< 3.7 극한하중에 대한 단면검토
	void SectionCheckAboutUltimateLoad();
	///< 3.7 바닥판 보강 검토
	void CheckFloorReinforce(CString szSheetName);
	///< 3.4 내부지점부 바닥판 검토
	void CheckInnerJijum();
	// 지점캔틸레버부(신축이음부)
	void ExpansionJointUltimateLoad();
	///< 3.9 정착부 보강 설계
	void DesignFixingReinforce();

	void CheckFloorRebarCalc(FloorType eType, CStringArray &ArrDelSheet, BOOL bNewSheet = TRUE);

	//////////////////////////////////////////////////////////////////////////
	// 강선이 있는 경우
	///< 3. 바닥판 설계
	void TendonDesignFloor();
	///< 3.3 하중산정
	void TendonLoadSum();
	///< 3.4 단면력 집계(1)
	void TendonSectionSum();
	///< 3.5 프리스트레스의 손실(1)
	void TendonPreStressLose();
	///< 3.6 바닥판 슬래브 검토
	void TendonFloorStress();
	///< 3.7 극한하중에 대한 단면검토
	void TendonLimitStressCheck();
	///< 3.8 정착구 설계
	void TendonSettlement();
	///< 3.9 내부 지점부 주장률 검토
	void TendonInnerTension();
	///< 3.11 거더 단부 바닥판의 보강
	void TendonGirderReinforce();

protected:
	void Initialize();

	// 3.1 좌측캔틸레버부
	// 3.2 우측캔틸레버부
	// 1) 사하중
	void CalcDeadLoad();
	// 2) 활하중
	void CalcLiveLoad();
	// 3) 보도하중
	void CalcWalkLoad();
	// 4) 충돌하중
	void CalcCollisionLoad();
	// 5) 탱크하중
	void CalcTankLoad();
	// 6) 트레일러 하중
	void CalcTrailerLoad();
	// 7) 풍하중
	void CalcWindLoad();
	// 8) 원심하중
	void CalcCentriodLoad();
	// 9) 하중조합
	void CalcComposeLoad();
	// 10) 캔틸레버 단부설계
	void DesignOfCantilDanbu();
	// 11) 최소두계검토
	void CheckMinThick(long nFloorIndex);
	// 12) 단면검토
	void DesignOfSection(long nFloorIndex);
	void CalcReBarQuantity(long nFloorIndex);	// 철근량 산정 - 강도 설계
	void CaclRebarExperienceDesign();			// 철근량 산정 - 경험적 설계
	void CheckUsefully(long nFloorIndex);		// 사용성 검토
	
	// 1) 사하중
	void CalcDeadLoad_Center();
	// 2) 활하중
	void CalcLiveLoad_Center();
	// 3) 탱크 하중
	void CalcTankLoad_Center();
	// 4) 트레일러 하중
	void CalcTrailerLoad_Center();
	// 5) 원심하중
	void CalcCentriodLoad_Center();
	// 6) 하중조합 - 극한모멘트계산
	void CalcComposeLoad_Center(BOOL bCount = TRUE);
	// 7) 단면검토
	void DrawReBarQuantity(CDomyun *pDom, long nFloorIndex);
	void DrawCrackWidthCheck(CDomyun *pDom, long nType);
	void DimCrackWidthCheck(CDomyun *pDom, long nType);

	double GetAs(long AsNum, CString &Case_H);
	double DiaReturn(long AsNum);
	double AreaReturn(long AsNum);

	BOOL				m_bLeft;	// 좌측캔틸레버부, 우측켄틸레버부 구분
	CPlateBasicIndex	*m_pBx;
	CCentSeparation		*m_pSep;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	CStringArray		m_ArrayNumber;
	
	CStringArray		m_ArrayGuardWallH;
	CStringArray		m_ArrayGuardWallTopL;
	long				m_nSttRow;
	long				m_nNumber;
	long				m_nUltSttRow[3];	///< 하중조합 이후 Row Number
	long				m_nDanRow[6];		// 단부 좌, 우 참조용 Row
	double				m_dPreMd;
	double				m_dPostMd;
	BOOL				m_bComposeOnlyOne;

	void SetLines(int nLine);
	void DimGuardWall();

	long				m_nW3;//군중하중계산을 위해...

	// 플랜지 폭, 웹 두께
	void SeekFlange_Web(double& dFlange, double& dWeb);

	// 정착위치에 따른 강선위치 재설정
	void ReSetTendonDistance(CDPointArray& PosArr, long nPos, CDoubleArray& DisArr);

	// 거리구하기
	double GetDistance(CDPoint& p1, CDPoint& p2);

	// 위치별처짐구하기
	double GetDeflection(long nPos, CString szLoad);
	void GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr);


	// 도면표기
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
