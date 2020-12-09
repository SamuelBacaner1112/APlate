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
	m_nDesignCondition							= 0;		// 설계조건
	m_nDesignBridgeType                         = 1;        // 교량형태    0 : 플레이트  1 : 소수주형
	m_nDesignSlabType                           = 0;		// 슬래브형태  0 : RC        1 : PSC
	m_nDesignSlabConType                        = 1;        // 슬래브시공형태 
	m_nDesignGirderConnect                      = 0;		// 주형이음형태 0: 현장이음판 1: 현장용접 
	m_nDesignUseHBeamAtCr                       = 0;		// H형강사용	
	m_nDesignHeigtBaseUpper						= 0;		// 초기값..소수주형...상면 상단
	m_nDesignHeigtBaseLower						= 1;		// 초기랎..소수주형...하면 하단  ...070202일자 변경....KB...소수주거더교 연구성과 발표회 후...
	m_nDesignHBracingDirChange					= 0;		// 수평브레이싱 형강 방향 변경 옵션
	//강종
	m_nDesignSteelChangeByThick					= 2;		// 두께별 강종변환 적용	
	m_dDesignMinThickOfUDFlate					= 12.0;		// 상.하판 최저두께    (mm)
	m_dDesignMinThickOfUpFlate					= 12.0;
	m_dDesignMinThickOfLoFlate					= 12.0;
	m_dDesignMinThickOfWebFlate					= 12.0;		// 복부판 최저두께    (mm)
	m_dDesignAllowMaxThick						= 8.0;		// 구조계산시 허용두께 편차(mm)
	m_bDesignAllowDeflection					= TRUE;
	//운반 
	m_dDesignMaxWeightOfCarLoad					= 200000;	// 차량적재 최대중량   (N)
	m_dDesignMaxLengthOfFieldSplice				= 13750;	// 현장이음 최대길이    (mm)
	//용접
	m_nDesignDimensionOfFilletWeld				= 2;		// 필렛 용접 치수 기준
	m_dDesignMinLenOfFilletWeld					= 80;		// 필렛 용접 최소길이    (mm)
	m_dDesignThickWeldNoCut						=  0;
	m_dDesignThickXWeldApply					= 16;		// X 개선 용접 적용 두께 : 입력두께 이하 V용접 적용
	//볼트설치
	m_nDesignBoltHoleJigJaegOfHorPlate			= 1;		// 볼트배치 지그재그 타입 가로보 상.하판
	m_nDesignBoltHoleJigJaegOfVerPlate			= 0;		// 볼트배치 지그재그 타입 세로보 상.하판
	m_nDesignBoltHoleJigJaegOfOutCrossPlate		= 0;		// 볼트배치 지그재그 타입 외측가로보 상.하판
	m_nDesignBoltHoleJigJaegOfOutBeamPlate		= 0;		// 볼트배치 지그재그 타입 외측빔 상.하판
	m_nDesignBoltHoleArrangeByunSectionType		= 0;		// 볼트배치 복부판 변단면 타입
	m_nDesignBoltHoleArrangeVerticalBeam		= 0;		// 볼트배치 복부판 세로보
	m_nDesignBoltHoleArrangeCrossBeam			= 0;		// 볼트배치 복부판 가로보
	m_nDesignBoltHoleArrangeOutCrossBeam		= 0;		// 외측가로보 이음판 볼트 배치
	m_nDesignBoltHoleArrangeOutBeam				= 0;		// 볼트배치 복부판 외측빔
	m_dDesignBoltJongVSlopApplyUp				= 3;		// 볼트배치 복부판 상부 경사 기준 (%)
	m_dDesignBoltJongVSlopApplyDn				= 3;		// 볼트배치 복부판 하부 경사 기준 (%)
	m_bBoltLengthAuto							= TRUE;		// 모든 볼트 길이 자동 계산
	//가로보
	m_nDesignCrossBeamBracketType				= 0;		// 가로보 브라켓 경사 타입
	m_nDesignCrossBeamHStiffAtBracket			= 1;		// 가로보 수평보강재 브라켓위치 적용	
	//신축이음부
	m_dSttExpansionJointHeight					= 150;
	m_dSttExpansionJointWidth					= 300;
	m_dEndExpansionJointHeight					= 150;
	m_dEndExpansionJointWidth					= 300;
	//도면번호
	m_nDesignStartNumberOfGirder				= 0;		// 거더시작 번호
	m_nDesignStartNumberOfSplice				= 0;		// 스플라이스 시작번호
	//단위관련
	m_nDesignUnitTonfMeter						= 1;		// 중량,거리,좌표 단위 - 추가 2002/10/02
	m_nDesignPointSlope							= 3;		// 편경사 및 종단기울기 표기 자리수 - 추가 2002/10/02
	m_nDesignByundanVSlopRoundPos				= 2;		// // 이음판 경사적용 단위(반올림 자리수)
	//기타
	m_nDesignHBracingInstall					= 1;		//수평브레이싱 설치 타입 - 추가 2003/03/25
	m_nDesignStringerJijumApply					= 1;		// 세로보 단부 지점부 적용 - 추가 2002/08/12
	m_nDesignDanmyunCheck                       = 0;		// 단면검토시 거더구분(외측거더, 내측거더 구분적용)
	m_nDesignGirderEndType						= 0;        // 주형 끝단타입

	m_nDegreeEarthQuake							= 0;		// 내진등급 (1,2,특)
	m_nRegionEarthQuake							= 0;		// 지진구역 (1,2)
	m_dCoefficientGround						= (long)1.0;
	m_dRiskCoefficient							= 1.4;		///< 위험도 계수
	m_dZoneCoefficient							= 0.11;		///< 구역계수

	m_nDesignVStiffInstall						= 0;		// 가로보사이에 수직보강재 설치	[설치]
	m_nDesignHStiffInstall						= 0;		// 수평보강재 설치				[설치]
	m_bInstallBeamAtCenter						= FALSE;	// 브레이싱 골조선 도심축 설치
	m_nDesignAllowCrackWidthEnv					= 1;		// 강재의 부식에 대한 환경조건 (허용균열폭) - 추가 : 일반환경			
	
	m_nSlabCentDesignMethod						= 0;		//바닥판 중앙부 철근 계산 방법
	m_dAccelerationOfGravity					= 9.81;		// 중열가속도(m/sec2) - 보류
	m_dInsulationVStiffUF						= 0;		//TU거더교 수직 보강재 상부플랜지 이격 옵션

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
	//집계설정	
	m_nSummaryNumberOfRound						= 4;		// 집계표 총중량 자리수 (단위)
	m_nSummaryUnitNumberOfRound					= 4;		// 집계표 단위중량 자리수 (단위)
	m_nSummaryApplyBaseOfDesign					= 0;		// 집계표 적용기준 설계용(상.하판)
	m_nSummaryApplyBaseOfAssm					= 1;		// 집계표 적용기준 제작용
	m_nSummaryAddBoltWeight						= 1;		// 중량 포함 볼트
	m_nSummaryAddStudWeight						= 1;		// 중량 포함 전단연결재
	m_nSummaryAddSlabAnchorWeight				= 1;		// 중량 포함 슬래브 앵커
	m_nSummaryAddGoriWeight						= 1;		// 중량 포함 들고리	
	m_nSummaryAddSpliceWeight					= 1;		// 중량 포함 들고리	
	m_nAssmUseCrossBeamBracketGirderOfBigAssm   = 1;
}

void CGlobarOption::Serialize(CArchive& ar)
{
//	long nFlag = 14;	//090826-박병구
	long nFlag = 15;	//강재DB적용
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
		//강종
		ar << m_nDesignSteelChangeByThick;			
		ar << m_dDesignMinThickOfUDFlate;
		ar << m_dDesignMinThickOfUpFlate;
		ar << m_dDesignMinThickOfLoFlate;
		ar << m_dDesignMinThickOfWebFlate;
		ar << m_dDesignAllowMaxThick;						
		//운반 
		ar << m_dDesignMaxWeightOfCarLoad;				
		ar << m_dDesignMaxLengthOfFieldSplice;			
		//용접
		ar << m_nDesignDimensionOfFilletWeld;				
		ar << m_dDesignMinLenOfFilletWeld;				
		ar << m_dDesignThickWeldNoCut;					
		ar << m_dDesignThickXWeldApply;					
		//볼트설치
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
		//가로보
		ar << m_nDesignCrossBeamBracketType;				
		ar << m_nDesignCrossBeamHStiffAtBracket;			
		ar << vLong;              
		//신축이음부
		ar << m_dSttExpansionJointHeight;					
		ar << m_dSttExpansionJointWidth;					
		ar << m_dEndExpansionJointHeight;					
		ar << m_dEndExpansionJointWidth;					
		//도면번호
		ar << m_nDesignStartNumberOfGirder;				
		ar << m_nDesignStartNumberOfSplice;				
		//단위관련
		ar << m_nDesignUnitTonfMeter;						
		ar << m_nDesignPointSlope;						
		ar << m_nDesignByundanVSlopRoundPos;				
		//기타
		ar << m_nDesignHBracingInstall;					
		ar << m_nDesignGirderEndType;							
		ar << m_nDesignStringerJijumApply;		
		ar << m_nDesignDanmyunCheck;
		//집계설정	
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
		//지진하중
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
		//강종
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
		//운반 
		ar >> m_dDesignMaxWeightOfCarLoad;				
		ar >> m_dDesignMaxLengthOfFieldSplice;			
		//용접
		ar >> m_nDesignDimensionOfFilletWeld;				
		ar >> m_dDesignMinLenOfFilletWeld;				
		ar >> m_dDesignThickWeldNoCut;					
		ar >> m_dDesignThickXWeldApply;					
		//볼트설치
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
		//가로보
		ar >> m_nDesignCrossBeamBracketType;				
		ar >> m_nDesignCrossBeamHStiffAtBracket;			
		ar >> vLong;              
		//신축이음부
		ar >> m_dSttExpansionJointHeight;					
		ar >> m_dSttExpansionJointWidth;					
		ar >> m_dEndExpansionJointHeight;					
		ar >> m_dEndExpansionJointWidth;					
		//도면번호
		ar >> m_nDesignStartNumberOfGirder;				
		ar >> m_nDesignStartNumberOfSplice;				
		//단위관련
		ar >> m_nDesignUnitTonfMeter;						
		ar >> m_nDesignPointSlope;						
		ar >> m_nDesignByundanVSlopRoundPos;				
		//기타
		ar >> m_nDesignHBracingInstall;					
		ar >> m_nDesignGirderEndType;							
		ar >> m_nDesignStringerJijumApply;				
		if(nFlag>1)
			ar >> m_nDesignDanmyunCheck;
		//집계설정	
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

		//지진하중
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
	if(m_nDesignCondition == 0) return "도로교량";
//	if(m_nDesignCondition == 1) return "지하철교량";
//	if(m_nDesignCondition == 2) return "국철교량(LS-22)";
//	if(m_nDesignCondition == 3) return "국철교량(LS-18)";
//	if(m_nDesignCondition == 4) return "고속철도교량";
//	if(m_nDesignCondition == 5) return "경전철교량";
	return "";
}

CString	CGlobarOption::GetStrDesignBridgeType()
{
	if(m_nDesignBridgeType == 0)	return "판형교";
	if(m_nDesignBridgeType == 1)	return "소수주거더교";
	if(m_nDesignBridgeType == 2)	return "Turnover거더교";
	return "";
}

CString CGlobarOption::GetStrDesignAllowCrackWidthEnv()
{
	if(m_nDesignAllowCrackWidthEnv == 0) return	"건조한 환경";
	if(m_nDesignAllowCrackWidthEnv == 1) return	"일반 환경";
	if(m_nDesignAllowCrackWidthEnv == 2) return	"부식성 환경";
	if(m_nDesignAllowCrackWidthEnv == 3) return	"극심한 부식성 환경";
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
	if(m_nDesignSlabConType == 0) return "데크플레이트";
	if(m_nDesignSlabConType== 1) return "거푸집";
	return "";
}

CString	CGlobarOption::GetStrDesignGirderConnect()
{
	if(m_nDesignGirderConnect == 0) return "현장이음";
	if(m_nDesignGirderConnect == 1) return "현장용접";

	return "";
}

CString CGlobarOption::GetStrDesignHBracingDirChange()
{
	if(m_nDesignHBracingDirChange== 0) return "하향기준";
	if(m_nDesignHBracingDirChange== 1) return "상향기준";
	return "";
}

CString	CGlobarOption::GetStrDesignUseHBeamAtCr()
{
	if(m_nDesignUseHBeamAtCr== 0) return "적용";
	if(m_nDesignUseHBeamAtCr== 1) return "적용안함";

	return "";
}

CString CGlobarOption::GetStrBoltLengthAuto()
{
	if(m_bBoltLengthAuto)	return "적용";
	else					return "적용안함";
}

CString CGlobarOption::GetStrSlabCentDesignMethod()
{
	if(m_nSlabCentDesignMethod == 0) return "강도 설계법";
	if(m_nSlabCentDesignMethod == 1) return "경험적 설계법";
	if(m_nSlabCentDesignMethod == 2) return "LB-Deck 공법";
	return "";
}

CString	CGlobarOption::GetStrDesignSteelChangeByThick	()			
{
	if(m_nDesignSteelChangeByThick == 0) return "두께별 강종 자동 변환";
	if(m_nDesignSteelChangeByThick == 1) return "두께별 520 적용";
	if(m_nDesignSteelChangeByThick == 2) return "두께별 강종 변환 없음";
	return "";
}
CString	CGlobarOption::GetStrDesignDimensionOfFilletWeld()
{	
	if(m_nDesignDimensionOfFilletWeld == 0) return "실제 치수 적용";
	if(m_nDesignDimensionOfFilletWeld == 1) return "거더별 최대 치수적용";	
	if(m_nDesignDimensionOfFilletWeld == 2) return "상하 측판별 최대치수";
	if(m_nDesignDimensionOfFilletWeld == 3) return "전체 8mm 적용";	
	if(m_nDesignDimensionOfFilletWeld == 4) return "홀수두께 올림(실제 치수)";	
	if(m_nDesignDimensionOfFilletWeld == 5) return "홀수두께 올림(박스별 최대)";	
	if(m_nDesignDimensionOfFilletWeld == 6) return "홀수두께 올림(중조립마크별)";	
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfHorPlate()
{
	if(m_nDesignBoltHoleJigJaegOfHorPlate == 0) return "지그재그 타입";
	if(m_nDesignBoltHoleJigJaegOfHorPlate == 1) return "일반타입";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfVerPlate()
{
	if(m_nDesignBoltHoleJigJaegOfVerPlate == 0) return "지그재그 타입";
	if(m_nDesignBoltHoleJigJaegOfVerPlate == 1) return "일반타입";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfBracketFlange()
{
	if(m_nDesignBoltHoleJigJaegOfOutCrossPlate == 0) return "지그재그 타입";
	if(m_nDesignBoltHoleJigJaegOfOutCrossPlate == 1) return "일반타입";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleJigJaegOfOutBeamPlate()
{
	if(m_nDesignBoltHoleJigJaegOfOutBeamPlate == 0) return "지그재그 타입";
	if(m_nDesignBoltHoleJigJaegOfOutBeamPlate == 1) return "일반타입";		
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleArrangeByunSectionType()
{ 
	if(m_nDesignBoltHoleArrangeByunSectionType == 0) return "볼트 균등배치";
	if(m_nDesignBoltHoleArrangeByunSectionType == 1) return "볼트 일반배치";	
	if(m_nDesignBoltHoleArrangeByunSectionType == 2) return "볼트 수평배치";	
	return "";
}
CString	CGlobarOption::GetStrDesignBoltHoleArrangeCrossBeam()
{ 
	if(m_nDesignBoltHoleArrangeCrossBeam == 0) return "볼트 균등배치";
	if(m_nDesignBoltHoleArrangeCrossBeam == 1) return "볼트 일반배치";	
	if(m_nDesignBoltHoleArrangeCrossBeam == 2) return "볼트 수평배치";	
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleArrangeVerticalBeam()
{ 
	if(m_nDesignBoltHoleArrangeVerticalBeam == 0) return "볼트 균등배치";
	if(m_nDesignBoltHoleArrangeVerticalBeam == 1) return "볼트 일반배치";	
	if(m_nDesignBoltHoleArrangeVerticalBeam == 2) return "볼트 수평배치";	
	return "";
}

CString	CGlobarOption::GetStrDesignBoltHoleArrangeBracket	()
{ 
	if(m_nDesignBoltHoleArrangeOutCrossBeam == 0) return "볼트 균등배치";
	if(m_nDesignBoltHoleArrangeOutCrossBeam == 1) return "볼트 일반배치";	
	if(m_nDesignBoltHoleArrangeOutCrossBeam == 2) return "볼트 수평배치";	
	return "";
}
CString	CGlobarOption::GetStrDesignBoltHoleArrangeOutBeam()
{ 
	if(m_nDesignBoltHoleArrangeOutBeam == 0) return "볼트 균등배치";
	if(m_nDesignBoltHoleArrangeOutBeam == 1) return "볼트 일반배치";	
	if(m_nDesignBoltHoleArrangeOutBeam == 2) return "볼트 수평배치";	
	return "";
}

CString	CGlobarOption::GetStrDesignCrossBeamBracketType()
{ 
	if(m_nDesignCrossBeamBracketType == 0) return "일반타입";
	if(m_nDesignCrossBeamBracketType == 1) return "레벨타입";		
	return "";
}

CString CGlobarOption::GetStrDesignCrossBeamHStiffAtBracket()
{
	if(m_nDesignCrossBeamHStiffAtBracket == 0) return "적용";
	if(m_nDesignCrossBeamHStiffAtBracket == 1) return "적용안함";	
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
	if(m_nDesignHBracingInstall == 0) return "상부 설치";
	if(m_nDesignHBracingInstall == 1) return "하부 설치";
	if(m_nDesignHBracingInstall == 2) return "상,하부 설치";
	return "";
}

CString CGlobarOption::GetStrDesignGirderEndType()
{
	if(m_nDesignGirderEndType == 0) return "중심축에 직각";
	if(m_nDesignGirderEndType== 1) return "사각방향과 평행";

	return "";
}

CString CGlobarOption::GetStrDesignStringerJijumApply()
{
	if(m_nDesignStringerJijumApply == 0) return "적용";
	if(m_nDesignStringerJijumApply == 1) return "적용안함";	
	return "";
}

CString CGlobarOption::GetStrDesignDanmyunCheck()
{
	if(m_nDesignDanmyunCheck == 0) return "외측거더,내측거더 구분적용";
	if(m_nDesignDanmyunCheck == 1) return "거더 구분하지 않음";	
	if(m_nDesignDanmyunCheck == 2) return "모든거더 구분적용";	

	return "";
}


CString CGlobarOption::GetStrSummaryApplyBaseOfDesign()
{
	if(m_nSummaryApplyBaseOfDesign == 0) return "평면치수 좌.우 기준";
	if(m_nSummaryApplyBaseOfDesign == 1) return "평면치수 거더 중앙 기준";	
	return "";		
}

CString CGlobarOption::GetStrSummaryApplyBaseOfAssm()
{
	if(m_nSummaryApplyBaseOfAssm == 0) return "전개치수 좌우 적용";
	if(m_nSummaryApplyBaseOfAssm == 1) return  "전개치수 직사각형 적용";
	if(m_nSummaryApplyBaseOfAssm == 2) return  "전개치수 수축량, 좌우 적용";
	if(m_nSummaryApplyBaseOfAssm == 3) return  "전개치수 수축량, 직사각형 적용";
	return "";
}

CString CGlobarOption::GetStrDegreeEarthQuake()
{
	if(m_nDegreeEarthQuake == 0) return "내진I 등급";
	if(m_nDegreeEarthQuake == 1) return "내진II등급";
	if(m_nDegreeEarthQuake == 2) return "내진특등급";
	return "";
}

void CGlobarOption::SetStrDegreeEarthQuake(CString sType)
{
	long nType = 0;
	if(sType == "내진I 등급") nType = 0;
	if(sType == "내진II등급") nType = 1;
	if(sType == "내진특등급") nType = 2;
	SetDegreeEarthQuake(nType);
}

CString CGlobarOption::GetStrRegionEarthQuake()
{
	if(m_nRegionEarthQuake== 0) return "I 구역";
	if(m_nRegionEarthQuake == 1) return "II구역";
	return "";
}


CString CGlobarOption::GetStrDesignVStiffInstall()
{
	if(m_nDesignVStiffInstall== 0) return "설치함";
	if(m_nDesignVStiffInstall == 1) return "설치안함";

	return "";
}


CString CGlobarOption::GetStrInstallBeamAtCenter()
{
	if(m_bInstallBeamAtCenter)
		return "적용";
	else
		return "적용안함";	
}

CString CGlobarOption::GetStrDesignHStiffInstall()
{
	if(m_nDesignHStiffInstall== 0) return "설치함";
	if(m_nDesignHStiffInstall == 1) return "설치안함";

	return "";
}


void CGlobarOption::SetStrRegionEarthQuake(CString sType)
{
	long nType = 0;
	if(sType == "I 구역") nType = 0;
	if(sType == "II구역") nType = 1;
	SetRegionEarthQuake(nType);
}

void CGlobarOption::SetStrDesignCondition(CString sType)
{
	long nType = 0;

	if(sType == "도로교량") nType = 0;
//	if(sType == "지하철교량") nType = 1;
//	if(sType == "국철교량(LS-22)") nType = 2;
//	if(sType == "국철교량(LS-18)") nType = 3;
//	if(sType == "고속철도교량") nType = 4;
//	if(sType == "경전철교량") nType = 5;
	
	SetDesignCondition(nType);
}

void CGlobarOption::SetStrDesignBridgeType(CString sType)
{
	long nType = 0;

	if(sType == "판형교")			nType = 0;
	if(sType == "소수주거더교")		nType = 1;
	if(sType == "Turnover거더교")	nType = 2;

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
	if(sType == "데크플레이트") nType = 0;
	if(sType == "거푸집") nType = 1;

	SetDesignSlabConType(nType);
}

void CGlobarOption::SetStrDesignGirderConnect(CString sType)
{
	long nType = 0;
	if(sType == "현장이음") nType = 0;
	if(sType == "현장용접") nType = 1;

	SetDesignGirderConnect(nType);
}

void CGlobarOption::SetStrDesignUseHBeamAtCr(CString sType)
{
	long nType = 0;
	if(sType == "적용") nType = 0;
	if(sType == "적용안함") nType = 1;

	SetDesignUseHBeamAtCr(nType);
}

void CGlobarOption::SetStrDesignSteelChangeByThick(CString sType)
{
	long nType = 0;
	if(sType == "두께별 강종 자동 변환") nType = 0;
	if(sType == "두께별 520 적용") nType = 1;
	if(sType == "두께별 강종 변환 없음") nType = 2;

	SetDesignSteelChangeByThick(nType);
}

void CGlobarOption::SetStrDesignDimensionOfFilletWeld(CString sType)
{
	long nType = 0;
	if(sType == "실제 치수 적용") nType = 0;
	if(sType == "거더별 최대 치수적용") nType = 1;	
	if(sType == "상하 측판별 최대치수") nType = 2;	
	if(sType == "전체 8mm 적용") nType = 3;	
	if(sType == "홀수두께 올림(실제 치수)") nType = 4;
	if(sType == "홀수두께 올림(거더별 최대)") nType = 5;
	if(sType == "홀수두께 올림(중조립마크별)") nType = 6;

	SetDesignDimensionOfFilletWeld(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfHorPlate(CString sType)
{
	long nType = 0;
	if(sType == "지그재그 타입") nType = 0;
	if(sType == "일반타입") nType = 1;		

	SetDesignBoltHoleJigJaegOfHorPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfVerPlate(CString sType)
{
	long nType = 0;
	if(sType == "지그재그 타입") nType = 0;
	if(sType == "일반타입") nType = 1;		

	SetDesignBoltHoleJigJaegOfVerPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfBracketFlange(CString sType)
{
	long nType = 0;
	if(sType == "지그재그 타입") nType = 0;
	if(sType == "일반타입") nType = 1;		

	SetDesignBoltHoleJigJaegOfOutCrossPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleJigJaegOfOutBeamPlate(CString sType)
{
	long nType = 0;
	if(sType == "지그재그 타입") nType = 0;
	if(sType == "일반타입") nType = 1;		

	SetDesignBoltHoleJigJaegOfOutBeamPlate(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeByunSectionType(CString sType)
{
	long nType = 0;
	if(sType == "볼트 균등배치") nType = 0;
	if(sType == "볼트 일반배치") nType = 1;	
	if(sType == "볼트 수평배치") nType = 2;	

	SetDesignBoltHoleArrangeByunSectionType(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeCrossBeam(CString sType)
{ 
	long nType = 0;
	if(sType == "볼트 균등배치") nType = 0;
	if(sType == "볼트 일반배치") nType = 1;	
	if(sType == "볼트 수평배치") nType = 2;	

	SetDesignBoltHoleArrangeCrossBeam(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeVerticalBeam(CString sType)
{ 
	long nType = 0;
	if(sType == "볼트 균등배치") nType = 0;
	if(sType == "볼트 일반배치") nType = 1;	
	if(sType == "볼트 수평배치") nType = 2;	

	SetDesignBoltHoleArrangeVerticalBeam(nType);
}

void CGlobarOption::SetStrDesignBoltHoleArrangeBracket(CString sType)
{
	long nType = 0;
	if(sType == "볼트 균등배치") nType = 0;
	if(sType == "볼트 수평배치") nType = 2;	
	if(sType == "볼트 일반배치") nType = 1;

	SetDesignBoltHoleArrangeOutCrossBeam(nType);
}

void CGlobarOption::SetStrDesignCrossBeamBracketType(CString sType)
{
	long nType = 0;
	if(sType == "일반타입") nType = 0;
	if(sType == "레벨타입") nType = 1;
	
	SetDesignCrossBeamBracketType(nType);
}

void CGlobarOption::SetStrDesignCrossBeamHStiffAtBracket(CString sType)
{
	long nType = 0;
	if(sType == "적용") nType = 0;
	if(sType == "적용안함") nType = 1;

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
	if(sType == "상부 설치") nType = 0;
	if(sType == "하부 설치") nType = 1;
	if(sType == "상,하부 설치") nType = 2;

	SetDesignHBracingInstall(nType);
}

void CGlobarOption::SetStrDesignGirderEndType(CString sType)
{
	long nType = 0;
	if(sType == "중심축에 직각") nType = 0;
	if(sType == "사각방향과 평행") nType = 1;

	SetDesignGirderEndType(nType);
}

void CGlobarOption::SetStrDesignStringerJijumApply(CString sType)
{
	long nType = 0;
	if(sType == "적용") nType = 0;
	if(sType == "적용안함") nType = 1;

	SetDesignStringerJijumApply(nType);
}

void CGlobarOption::SetStrDesignDanmyunCheck(CString sType)
{
	long nType = 0;

	if(sType == "외측거더,내측거더 구분적용")	nType = 0;
	if(sType == "거더 구분하지 않음")			nType = 1;
	if(sType == "모든거더 구분적용")			nType = 2;

	SetDesignDanmyunCheck(nType);
}

void CGlobarOption::SetStrSummaryApplyBaseOfDesign(CString sType)
{
	long nType = 0;
	if(sType == "평면치수 좌.우 기준") nType = 0;
	if(sType == "평면치수 거더 중앙 기준") nType = 1;

	SetSummaryApplyBaseOfDesign(nType);
}

void CGlobarOption::SetStrSummaryApplyBaseOfAssm(CString sType)
{
	long nType = 0;	
	if(sType == "전개치수 좌우 적용")		nType = 0;
	if(sType == "전개치수 직사각형 적용")	nType = 1;
	if(sType == "전개치수 수축량, 좌우 적용")	nType = 2;
	if(sType == "전개치수 수축량, 직사각형 적용")nType = 3;	

	SetSummaryApplyBaseOfAssm(nType);
}



void CGlobarOption::SetStrDesignVStiffInstall(CString sType)
{
	long nType = 0;	
	if(sType == "설치함")		nType = 0;
	if(sType == "설치안함")		nType = 1;

	SetDesignVStiffInstall(nType);
}



void CGlobarOption::SetStrDesignHStiffInstall(CString sType)
{
	long nType = 0;	
	if(sType == "설치함")		nType = 0;
	if(sType == "설치안함")		nType = 1;

	SetDesignHStiffInstall(nType);
}


void CGlobarOption::SetStrInstallBeamAtCenter(CString sType)
{
	BOOL bApply = FALSE;	
	if(sType == "적용")			bApply = TRUE;
	if(sType == "적용안함")		bApply = FALSE;

	SetInstallBeamAtCenter(bApply);
}

void CGlobarOption::SetStrDesignAllowCrackWidthEnv(CString sType)
{
	long nType = 0;
	if(sType == "건조한 환경")			nType = 0;
	if(sType == "일반 환경")			nType = 1;
	if(sType == "부식성 환경")			nType = 2;
	if(sType == "극심한 부식성 환경")	nType = 3;
	
	SetDesignAllowCrackWidthEnv(nType);
}

void CGlobarOption::SetStrDesignHBracingDirChange(CString sType)
{
	long nType = 0;	
	if(sType == "하향기준")		nType = 0;
	if(sType == "상향기준")		nType = 1;

	SetDesignHBracingDirChange(nType);
}

void CGlobarOption::SetStrBoltLengthAuto(CString sType)
{
	BOOL bBoltLengthAuto = TRUE;

	if(sType == "적용안함")
		bBoltLengthAuto = FALSE;
	
	SetBoltLengthAuto(bBoltLengthAuto);
}

void CGlobarOption::SetStrSlabCentDesignMethod(CString sType)
{
	long nType = 0;	
	if(sType == "강도 설계법")		nType = 0;
	if(sType == "경험적 설계법")	nType = 1;
	if(sType == "LB-Deck 공법")		nType = 2;

	SetSlabCentDesignMethod(nType);	
}


// 거더높이 상하부 기준
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

// 거더높이 상하부 기준
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

// 상.하판 최저 두께
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
	if(sApply=="적용")		m_bDesignAllowDeflection = TRUE;
	if(sApply=="적용안함")	m_bDesignAllowDeflection = FALSE;
}

// 도면 반복 치수
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
	
	if(sYear == "도로설계기준2008")					nYear = APPLYYEAR2008;
	else if(sYear == "도로설계기준2010")				nYear = APPLYYEAR2010;
	
	SetSteelStandardYear(nYear);
}

long CGlobarOption::GetSteelStandardYear(CString sYear)
{
	long nYear = 0;
	
	if(sYear == "도로설계기준2008")			nYear = APPLYYEAR2008;
	else if(sYear == "도로설계기준2010")		nYear = APPLYYEAR2010;
	
	return nYear;
}

long CGlobarOption::GetSteelStandardYearComboIdx(CString sYear)
{
	long nYear = 0;
	
	if(sYear == "도로설계기준2008")			nYear = 0;
	else if(sYear == "도로설계기준2010")		nYear = 1;
	
	return nYear;
}

CString CGlobarOption::GetStrSteelStandardYear()
{
	if(m_nSteelStandardYear == APPLYYEAR2008 )		return	_T("도로설계기준2008");
	else if(m_nSteelStandardYear == APPLYYEAR2010)	return	_T("도로설계기준2010");
	
	return _T("");
}

CString CGlobarOption::GetStrSteelStandardYear(long nApplyYear)
{
	if(nApplyYear == APPLYYEAR2008 )		return	_T("도로설계기준2008");
	else if(nApplyYear == APPLYYEAR2010)	return	_T("도로설계기준2010");
	
	return _T("");
}

// 엔진의 블럭제원 포인터에 APlate의 블럭제원 주소를 연결
void CGlobarOption::SetExpansionBlockJewon(CDesignExpansionJoint *pDesignExp)
{
	if(pDesignExp==NULL || pDesignExp->GetSize()<2)	return;

	CExpansionJointData *pExpDataStt	= pDesignExp->GetExpansionJointData(0);
	CExpansionJointData *pExpDataEnd	= pDesignExp->GetExpansionJointData(1);

	pExpDataStt->SetBlockJewon(POST_SECTION,	&m_dSttExpansionJointWidth, &m_dSttExpansionJointHeight);
	pExpDataEnd->SetBlockJewon(PRE_SECTION,		&m_dEndExpansionJointWidth, &m_dEndExpansionJointHeight);
}