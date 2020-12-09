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

#define OPT_CHKSHEET			9			// 전체 구조계산서 

class CDesignExpansionJoint;
class AFX_EXT_CLASS CGlobarOption  
{
public:
	CGlobarOption();
	virtual ~CGlobarOption();

	void Serialize(CArchive& ar);

private:
	// 기본조건(교량형태관련)
	long	m_nDesignCondition;							// 설계조건	
	long    m_nDesignBridgeType;                        // 교량형태    0 : 플레이트  1 : 소수주형
	long    m_nDesignSlabType;                          // 슬래브형태  0 : RC        1 : PSC
	long    m_nDesignSlabConType;                       // 슬래브시공형태 
	long    m_nDesignGirderConnect;                     // 주형이음형태0 : 현장이음판1 : 현장용접
	long    m_nDesignUseHBeamAtCr;                      // H형강사용	
	long	m_nDesignHeigtBaseUpper;					// 거더높이 기준 0 : 상면 상단 1 : 상면 하단
	long	m_nDesignHeigtBaseLower;					// 거더높이 기준 0 : 하면 상단 1 : 하면 하단
	long	m_nDesignHBracingDirChange;					// 수평브레이싱 형강 방향 변경 옵션
	//강종두께관련
	long	m_nDesignSteelChangeByThick;				// 두께별 강종변환 적용
	double  m_dDesignMinThickOfUDFlate;					// 상.하판 최저두께  (mm) - nFlag 12부터 분리
	double	m_dDesignMinThickOfUpFlate;					// 상판 최저두께 (mm)
	double	m_dDesignMinThickOfLoFlate;					// 하판 최저두께 (mm)
	double	m_dDesignMinThickOfWebFlate;				// 복부판  최저두께  (mm)
	double	m_dDesignAllowMaxThick;						// 구조계산시 허용두께 편차(mm)
	BOOL	m_bDesignAllowDeflection;					// 인접두께 편차 허용치 적용여부

	//운반관련
	double  m_dDesignMaxWeightOfCarLoad;				// 차량적재 최대중량   (kg)
	double  m_dDesignMaxLengthOfFieldSplice;			// 현장이음 최대길이    (mm)
	//용접관련
	long	m_nDesignDimensionOfFilletWeld;				// 필렛 용접 치수 기준
	double	m_dDesignMinLenOfFilletWeld;				// 필렛 용접 최소길이    (mm)
	double	m_dDesignThickWeldNoCut;					// 무개선 용접 적용 두께차   (mm)
	double	m_dDesignThickXWeldApply;					// X 용접 적용 두께    (mm)
	//볼트설치관련
	long	m_nDesignBoltHoleJigJaegOfHorPlate;			// 볼트배치 지그재그 타입 가로보 상.하판
	long	m_nDesignBoltHoleJigJaegOfVerPlate;			// 볼트배치 지그재그 타입 세로보 상.하판
	long	m_nDesignBoltHoleJigJaegOfOutCrossPlate;	// 볼트배치 지그재그 타입 외측가로보 상.하판
	long	m_nDesignBoltHoleJigJaegOfOutBeamPlate;		// 볼트배치 지그재그 타입 외측빔 상.하판
	long	m_nDesignBoltHoleArrangeByunSectionType;	// 볼트배치 복부판 변단면 타입
	long	m_nDesignBoltHoleArrangeCrossBeam;			// 볼트배치 복부판 가로보
	long	m_nDesignBoltHoleArrangeVerticalBeam;		// 볼트배치 복부판 세로보
	long	m_nDesignBoltHoleArrangeOutCrossBeam;		// 볼트배치 복부판 외측가로보
	long	m_nDesignBoltHoleArrangeOutBeam;			// 볼트배치 복부판 외측빔
	double	m_dDesignBoltJongVSlopApplyUp;				// 볼트배치 복부판 상부경사 적용 (%)
	double	m_dDesignBoltJongVSlopApplyDn;				// 볼트배치 복부판 하부경사 적용 (%)
	BOOL	m_bBoltLengthAuto;							// 모든 볼트 길이 자동계산
	//가로보관련
	long	m_nDesignCrossBeamBracketType;				// 가로보 브라켓 경사 타입
	long	m_nDesignCrossBeamHStiffAtBracket;			// 가로보 수평보강재 브라켓위치 적용	
	//신축이음부관련
	double  m_dSttExpansionJointHeight;					// 시점 신축이음장치 높이   (mm)
	double  m_dSttExpansionJointWidth;					// 시점 신축이음장치 너비   (mm)
	double  m_dEndExpansionJointHeight;					// 종점 신축이음장치 높이   (mm)
	double  m_dEndExpansionJointWidth;					// 종점 신축이음장치 너비   (mm)
	//도면번호관련
	long	m_nDesignStartNumberOfGirder;				// 거더   시작번호
	long	m_nDesignStartNumberOfSplice;				// 이음판 시작번호
	//단위관련
	long	m_nDesignUnitTonfMeter;						// 중량,거리,좌표 단위
	long	m_nDesignPointSlope;						// 편경사 및 종단기울기 표기 자리수
	long	m_nDesignByundanVSlopRoundPos;				// 이음판 경사적용 단위(반올림 자리수) : 2
	//기타관련
	long	m_nDesignHBracingInstall;					// 수평보강재 설치 타입
	long    m_nDesignGirderEndType;                     // 주형 끝단타입
	long	m_nDesignStringerJijumApply;				// 세로보 단부 지점부 적용
	long    m_nDesignDanmyunCheck;						// 단면검토시 거더구분방법
	// 집계설정 
	long	m_nSummaryApplyBaseOfDesign;				// 집계표 적용기준 설계용(상.하판)
	long	m_nSummaryApplyBaseOfAssm;					// 집계표 적용기준 제작용
	long	m_nSummaryNumberOfRound;					// 집계표 총중량 자리수 (단위 Kg)
	long	m_nSummaryUnitNumberOfRound;				// 집계표 단위중량 자리수 (단위 Kg)
	long	m_nSummaryAddBoltWeight;					// 중량 포함 볼트
	long	m_nSummaryAddStudWeight;					// 중량 포함 전단연결재
	long	m_nSummaryAddSlabAnchorWeight;				// 중량 포함 슬래브 앵커
	long	m_nSummaryAddGoriWeight;					// 중량 포함 들고리
	long    m_nSummaryAddSpliceWeight;                  // 차량적재 산출시 현장이음 포함
	long	m_nAssmUseCrossBeamBracketGirderOfBigAssm;	// 대조립 마크 가로보 브라켓 박스에 사용

	long	m_nDegreeEarthQuake;						// 내진등급 (1,2,특)
	long	m_nRegionEarthQuake;						// 지진구역 (1,2)
	long	m_dCoefficientGround;
	double	m_dRiskCoefficient;							///< 위험도 계수
	double	m_dZoneCoefficient;							///< 구역계수
	
	long    m_nDesignVStiffInstall;						// 가로보사이에 수직보강재 설치
	long    m_nDesignHStiffInstall;						// 수평보강재 설치
	long	m_nSlabCentDesignMethod;					// 바닥판 중앙부 철근 계산방법
	double	m_dAccelerationOfGravity;					// 중력가속도(m/sec2)
	BOOL	m_bInstallBeamAtCenter;						// 브레이싱 골조선 도심축 설치
	long	m_nDesignAllowCrackWidthEnv;				// 강재의 부식에 대한 환경조건 (허용균열폭) - ARoad 추가 2005/05/11, APlate 추가 2007/04/19
	double	m_dInsulationVStiffUF;						// TU거더교 수직 보강재 상부플랜지 이격 옵션

	//강재관련
	long	m_nSteelStandardYear;						//강재기준
public:
	BOOL m_bCalcDocCheck[OPT_CHKSHEET];	//구조계산서 출력 (다이얼로그 옵션)
	long m_nCalcDocSub[OPT_CHKSHEET];	//구조계산서 출력 (다이얼로그 옵션)
	BOOL m_bCalcDocSub[OPT_CHKSHEET];	//구조계산서 출력 (다이얼로그 옵션)

public:
	void Init();
	void InitDesignBasic();  //기초입력(권고안)
	void InitDesignQty();    //집계표(권고안)
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 기본조건(교량형태관련)
	long	GetDesignCondition()				{ return	m_nDesignCondition;}   // 설계조건	
	long	GetDesignBridgeType()				{ return	m_nDesignBridgeType;}  // 교량형태 	
	long	GetDesignSlabType()					{ return	m_nDesignSlabType;}    // 슬래브형태
	long	GetDesignSlabConType()				{ return	m_nDesignSlabConType;} // 슬래브시공형태 
	long	GetDesignGirderConnect()			{ return	m_nDesignGirderConnect;} // 주형이음형태
	long	GetDesignUseHBeamAtCr()				{ return	m_nDesignUseHBeamAtCr; } // H형강사용	
	long	GetDesignHeigtBaseUpper()			{ return	m_nDesignHeigtBaseUpper; }
	long	GetDesignHeigtBaseLower()			{ return	m_nDesignHeigtBaseLower; }
	long	GetDesignHeigtBase(); // 거더높이 상하부 기준
	long	GetDesignHBracingDirChange()		{ return	m_nDesignHBracingDirChange; } // 수평브레이싱 형강 방향 변경 옵션
 
	//강종두께관련
	long	GetDesignSteelChangeByThick()		{ return	m_nDesignSteelChangeByThick; }// 두께별 강종변환 적용
	double	GetDesignUDFlangeMinThick(BOOL bUpper);	// 상.하판 최저 두께
	double	GetDesignWebFalngeMinThick()		{ return	m_dDesignMinThickOfWebFlate; }      // 복부판 플랜지 최저 두께
	double	GetDesignAllowMaxThick()			{ return	m_dDesignAllowMaxThick; }           // 구조계산시 허용두께 편차(mm)
	//운반관련
	double  GetDesignMaxWeightOfCarLoad()		{ return	m_dDesignMaxWeightOfCarLoad; }    // 차량적재 최대중량
	double	GetDesignMaxLengthOfFieldSplice()	{ return	m_dDesignMaxLengthOfFieldSplice; }// 현장이음 최대길이	
	//용접관련
	long	GetDesignDimensionOfFilletWeld()	{ return	m_nDesignDimensionOfFilletWeld; }// 필렛 용접 치수 기준
	double	GetDesignMinLenOfFilletWeld()		{ return	m_dDesignMinLenOfFilletWeld;	}// 필렛 용접 최소길이    (mm)
	double	GetDesignThickWeldNoCut()			{ return	m_dDesignThickWeldNoCut;	}    //무개선 용접 적용 두께차   (mm)
	double	GetDesignThickXWeldApply()			{ return	m_dDesignThickXWeldApply;	}    //X 용접 적용 두께    (mm)
	//볼트설치관련
	long	GetDesignBoltHoleJigJaegOfHorPlate()	 { return	m_nDesignBoltHoleJigJaegOfHorPlate; }      // 볼트배치 지그재그 타입 가로보 상.하판
	long	GetDesignBoltHoleJigJaegOfVerPlate()	 { return	m_nDesignBoltHoleJigJaegOfVerPlate; }      // 볼트배치 지그재그 타입 세로보 상.하판
	long	GetDesignBoltHoleJigJaegOfBracketFlange(){ return	m_nDesignBoltHoleJigJaegOfOutCrossPlate; } // 볼트배치 지그재그 타입 외측가로보 상.하판
	long	GetDesignBoltHoleJigJaegOfOutBeamPlate() { return   m_nDesignBoltHoleJigJaegOfOutBeamPlate; }// 볼트배치 지그재그 타입 외측빔 상.하판
	long	GetDesignBoltHoleArrangeByunSectionType(){ return	m_nDesignBoltHoleArrangeByunSectionType; } // 볼트배치 복부판 변단면 타입
	long	GetDesignBoltHoleArrangeCrossBeam()		 { return	m_nDesignBoltHoleArrangeCrossBeam; }       // 볼트배치 복부판 가로보
	long	GetDesignBoltHoleArrangeVerticalBeam()	 { return   m_nDesignBoltHoleArrangeVerticalBeam; }    // 볼트배치 복부판 세로보
	long	GetDesignBoltHoleArrangeBracket()		 { return	m_nDesignBoltHoleArrangeOutCrossBeam; }    // 볼트배치 복부판 외측가로보
	long	GetDesignBoltHoleArrangeOutBeam()		 { return	m_nDesignBoltHoleArrangeOutBeam; }         // 볼트배치 복부판 외측빔
	double	GetDesignBoltJongVSlopApplyUp()			 { return	m_dDesignBoltJongVSlopApplyUp;	}  // 볼트배치 복부판 상부경사 적용 (%)
	double	GetDesignBoltJongVSlopApplyDn()			 { return	m_dDesignBoltJongVSlopApplyDn;} // 볼트배치 복부판 하부경사 적용 (%)
	BOOL	GetBoltLengthAuto()						 { return	m_bBoltLengthAuto;}				// 모든 볼트 길이 자동계산
	//가로보관련
	long	GetDesignCrossBeamBracketType()		{ return	m_nDesignCrossBeamBracketType; }
	long	GetDesignCrossBeamHStiffAtBracket()	{ return	m_nDesignCrossBeamHStiffAtBracket; }	// 수평보강재 브라켓 위치 적용	
	//신축이음부관련
	double  GetSttExpansionJointHeight()        { return	m_dSttExpansionJointHeight;}
	double  GetSttExpansionJointWidth()	        { return	m_dSttExpansionJointWidth;}
	double  GetEndExpansionJointHeight()        { return	m_dEndExpansionJointHeight;}
	double  GetEndExpansionJointWidth()	        { return	m_dEndExpansionJointWidth;}
	//도면관련
	long	GetDesignStartNumberOfGirder()		{ return	m_nDesignStartNumberOfGirder; }
	long	GetDesignStartNumberOfSplice()		{ return	m_nDesignStartNumberOfSplice; }
	//단위관련
	long	GetDesignUnitTonfMeter()			{ return	m_nDesignUnitTonfMeter; }
	long	GetDesignPointSlope()				{ return	m_nDesignPointSlope; }
	long	GetDesignByundanVSlopRoundPos()	 	{ return	m_nDesignByundanVSlopRoundPos; }
	//기타관련	
	long	GetDesignHBracingInstall()			{ return	m_nDesignHBracingInstall; }
	long	GetDesignGirderEndType()			{ return	m_nDesignGirderEndType; }
	long	GetDesignStringerJijumApply()		{ return	m_nDesignStringerJijumApply; }
	long 	GetDesignDanmyunCheck()				{ return	m_nDesignDanmyunCheck;}
	//집계설정 
	long	GetSummaryApplyBaseOfDesign()		{ return	m_nSummaryApplyBaseOfDesign; }
	long	GetSummaryApplyBaseOfAssm()			{ return	m_nSummaryApplyBaseOfAssm; }
	long	GetSummaryAddBoltWeight()			{ return	m_nSummaryAddBoltWeight; }
	long	GetSummaryAddStudWeight()			{ return	m_nSummaryAddStudWeight; }	
	long	GetSummaryAddSlabAnchorWeight()		{ return	m_nSummaryAddSlabAnchorWeight; }
	long	GetSummaryAddGoriWeight()			{ return	m_nSummaryAddGoriWeight; }
	long	GetSummaryAddSpliceWeight()			{ return	m_nSummaryAddSpliceWeight; }
	long	GetAssmUseCrossBeamBracketGirderOfBigAssm(){return	m_nAssmUseCrossBeamBracketGirderOfBigAssm; }
	
	long	GetPointValueOfGangJae()			{ return	m_nSummaryNumberOfRound;}		// 집계표 총중량 자리수 (단위 Kg)
	long	GetPointValueOfUnitGangJae()		{ return	m_nSummaryUnitNumberOfRound;}	// 집계표 단위중량 자리수 (단위 Kg)
	// 지진
	long	GetIdxDegreeEarthQuake()				{ return m_nDegreeEarthQuake;	};
	long	GetIdxRegionEarthQuake()				{ return m_nRegionEarthQuake;	};
	CString GetStrRegionEarthQuake();
	CString GetStrDegreeEarthQuake();
	double GetCoefficientGround()					{ return m_dCoefficientGround; }
	double GetRiskCoefficient()						{ return m_dRiskCoefficient; }
	double GetZoneCoefficient()						{ return m_dZoneCoefficient; }
	long	GetDesignVStiffInstall()				{ return m_nDesignVStiffInstall; }
	long	GetDesignHStiffInstall()				{ return m_nDesignHStiffInstall; }
	long	GetSlabCentDesignMethod()				{ return m_nSlabCentDesignMethod;}//바닥판 중앙부 철근 계산방법
	BOOL	IsLBDeck()								{ return m_nSlabCentDesignMethod==2 ? TRUE : FALSE; }
	double	GetAccelerationOfGravity()				{ return m_dAccelerationOfGravity;}	
	BOOL    GetInstallBeamAtCenter()				{ return m_bInstallBeamAtCenter;}
	long	GetDesignAllowCrackWidthEnv()			{ return m_nDesignAllowCrackWidthEnv;}	
	double  GetInsulationVStiffUF()					{ return m_dInsulationVStiffUF;}
	long	GetSteelStandardYear()					{return m_nSteelStandardYear;}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 기본조건(교량형태관련)
	void SetDesignCondition(long nVal)				{ m_nDesignCondition = nVal;}
	void SetDesignBridgeType(long nVal)				{ m_nDesignBridgeType = nVal;}  // 교량형태 	
	void SetDesignSlabType(long nVal)				{ m_nDesignSlabType = nVal;}    // 슬래브형태
	void SetDesignSlabConType(long nVal)			{ m_nDesignSlabConType = nVal;} // 슬래브시공형태 
	void SetDesignGirderConnect(long nVal)			{ m_nDesignGirderConnect = nVal;} // 주형이음형태
	void SetDesignUseHBeamAtCr(long nVal)			{ m_nDesignUseHBeamAtCr = nVal;}  // H형강사용	
	void SetDesignHeigtBaseUpper(long nVal)			{ m_nDesignHeigtBaseUpper = nVal; }
	void SetDesignHeigtBaseLower(long nVal)			{ m_nDesignHeigtBaseLower = nVal; }
	void SetDesignHeigtBase(long nUpperLowerVal);
	void SetDesignHBracingDirChange(long nVal)		{ m_nDesignHBracingDirChange = nVal; } // 수평브레이싱 형강 방향 변경 옵션
	//강종두께관련
	void SetDesignSteelChangeByThick(long nVal)		{ m_nDesignSteelChangeByThick = nVal; }// 두께별 강종변환 적용
	void SetDesignMinThickOfUDFlate(double dVal, BOOL bUpper);	// 상.하판 최저 두께
	void SetDesignMinThickOfWebFlate(double dVal)	{ m_dDesignMinThickOfWebFlate = dVal; }// 복부판 플랜지 최저 두께
	void SetDesignAllowMaxThick(double dVal)		{ m_dDesignAllowMaxThick = dVal; }
	//운반관련
	void SetDesignMaxWeightOfCarLoad(double dVal)	{ m_dDesignMaxWeightOfCarLoad = dVal; }// 차량적재 최대중량
	void SetDesignMaxLengthOfFieldSplice(double dVal){ m_dDesignMaxLengthOfFieldSplice = dVal; }// 현장이음 최대길이		
	//용접관련
	void SetDesignDimensionOfFilletWeld(long nD)	{ m_nDesignDimensionOfFilletWeld = nD; }	
	void SetDesignMinLenOfFilletWeld(double dVal)	{ m_dDesignMinLenOfFilletWeld = dVal;	}// 필렛 용접 최소두께
	void SetDesignThickWeldNoCut(double dVal)		{ m_dDesignThickWeldNoCut = dVal;	}// 필렛 용접 최소두께
	void SetDesignThickXWeldApply(double dVal)		{ m_dDesignThickXWeldApply = dVal;	}// 필렛 용접 최소두께
	//볼트설치관련
	void SetDesignBoltHoleJigJaegOfHorPlate(long nVal){ m_nDesignBoltHoleJigJaegOfHorPlate = nVal; }
	void SetDesignBoltHoleJigJaegOfVerPlate(long nVal){ m_nDesignBoltHoleJigJaegOfVerPlate = nVal; }
	void SetDesignBoltHoleJigJaegOfOutCrossPlate(long nVal)	{ m_nDesignBoltHoleJigJaegOfOutCrossPlate = nVal; }
	void SetDesignBoltHoleJigJaegOfOutBeamPlate(long nVal)  { m_nDesignBoltHoleJigJaegOfOutBeamPlate = nVal; }
	void SetDesignBoltHoleArrangeByunSectionType(long nVal)	{ m_nDesignBoltHoleArrangeByunSectionType = nVal; }// 복부판 이음 하판 변단면 볼트 배치
	void SetDesignBoltHoleArrangeCrossBeam(long nVal)	 { m_nDesignBoltHoleArrangeCrossBeam = nVal; }
	void SetDesignBoltHoleArrangeVerticalBeam(long nVal) { m_nDesignBoltHoleArrangeVerticalBeam = nVal; }
	void SetDesignBoltHoleArrangeOutCrossBeam(long nVal) { m_nDesignBoltHoleArrangeOutCrossBeam = nVal; }// 외측가로보 이음판 볼트 배치
	void SetDesignBoltHoleArrangeOutBeam(long nVal)	{ m_nDesignBoltHoleArrangeOutBeam = nVal; }
	void SetDesignBoltJongVSlopApplyUp(double dVal)	{ m_dDesignBoltJongVSlopApplyUp = dVal;	}// 복부판 이음판 상부경사 기준 (3%)  
	void SetDesignBoltJongVSlopApplyDn(double dVal)	{ m_dDesignBoltJongVSlopApplyDn = dVal; } // 복부판 이음판 하부경사 기준 (3%)
	void SetBoltLengthAuto(BOOL bD)				    { m_bBoltLengthAuto = bD;	}// 모든 볼트 길이 자동계산
	//가로보관련 
	void SetDesignCrossBeamBracketType(long nD)		 { m_nDesignCrossBeamBracketType = nD; }	
	void SetDesignCrossBeamHStiffAtBracket(long nVal){ m_nDesignCrossBeamHStiffAtBracket = nVal; }	// 수평보강재 브라켓 위치 적용	
	//신축이음부관련
	void SetSttExpansionJointHeight(double dVal)    { m_dSttExpansionJointHeight = dVal;}
	void SetSttExpansionJointWidth(double dVal)	    { m_dSttExpansionJointWidth = dVal;}
	void SetEndExpansionJointHeight(double dVal)    { m_dEndExpansionJointHeight = dVal;}
	void SetEndExpansionJointWidth(double dVal)	    { m_dEndExpansionJointWidth = dVal;}
	//도면관련
	void SetDesignStartNumberOfGirder(long nVal)	{ m_nDesignStartNumberOfGirder = nVal; }
	void SetDesignStartNumberOfSplice(long nVal)	{ m_nDesignStartNumberOfSplice = nVal; }
	//단위관련
	void SetDesignUnitTonfMeter(long nVal)			{ m_nDesignUnitTonfMeter = nVal; }
	void SetDesignPointSlope(long nVal)				{ m_nDesignPointSlope = nVal; }
	void SetDesignByundanVSlopRoundPos(long nVal)	{ m_nDesignByundanVSlopRoundPos = nVal; }
	//기타관련
	void SetDesignHBracingInstall(long nVal)		{ m_nDesignHBracingInstall = nVal; }
	void SetDesignGirderEndType(long nVal)          { m_nDesignGirderEndType = nVal;}
	void SetDesignStringerJijumApply(long nVal)		{ m_nDesignStringerJijumApply = nVal; }
	void SetDesignDanmyunCheck(long nVal)			{ m_nDesignDanmyunCheck       = nVal;}
	//집계설정
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
	CString GetStrBoltLengthAuto();//모든 볼트길이 자동계산
	CString GetStrSlabCentDesignMethod();//바닥판 중앙부 철근 계산방법
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
