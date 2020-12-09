// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// FemStressCalc.h: interface for the CFemStressCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEMSTRESSCALC_H__502BF5DE_6071_40FC_B600_C70A2C3BFDEB__INCLUDED_)
#define AFX_FEMSTRESSCALC_H__502BF5DE_6071_40FC_B600_C70A2C3BFDEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  NOD_DISPLACE   0       // 처짐
#define  NOD_ROTATE     1       // 회전량
#define  NOD_REACTION   2       // 반력
#define  NOD_MOMENT     3       // 모멘트

#define  LC_PREV_S				0x00000001  // 합성전 사하중 (Steel)
#define  LC_PREV_CONF			0x00000002  // 합성전 사하중 (구속콘크리트)
#define  LC_PREV_TURN			0x00000004  // 합성전 사하중 (터닝)
#define  LC_PREV_C				0x00000008  // 합성전 사하중 (Concrete)
#define  LC_NEXT				0x00000010  // 합성후 사하중
#define  LC_LIVE_MAX			0x00000020  // 활하중 Max
#define  LC_LIVE_MIN			0x00000040  // 활하중 Min
#define  LC_ONLYLIVE_MAX		0x00000080  // 활하중 Max
#define  LC_ONLYLIVE_MIN		0x00000100  // 활하중 Min
#define  LC_DISP_MAX			0x00000200  // 지점 침하 Max
#define  LC_DISP_MIN			0x00000400  // 지점 침하 Min
#define  LC_SUM_MAX				0x00000800  // 합계 Max
#define  LC_SUM_MIN				0x00001000  // 합계 Min		///< GetElementForce()는 여기까지만 지원..
#define  LC_DB_MAX				0x00002000	// DB max
#define  LC_DB_MIN				0x00004000	// DB min
#define  LC_DL_MAX				0x00008000	// DL max
#define  LC_DL_MIN				0x00010000	// DL min
#define  LC_MTY_MAX				0x00020000	// MTY max
#define  LC_MTY_MIN				0x00040000	// MTY min
#define  LC_PEOPLE_MAX			0x00080000	// PEOPLE max
#define  LC_PEOPLE_MIN			0x00100000	// PEOPLE min
#define  LC_WIND_MAX			0x00200000	// WIND max
#define  LC_WIND_MIN			0x00400000	// WIND min
#define  LC_TANK_MAX			0x00800000	// TANK max
#define  LC_TANK_MIN			0x01000000	// TANK min
#define  LC_TRAILER_MAX			0x02000000	// TRAILER max
#define  LC_TRAILER_MIN			0x04000000	// TRAILER min
#define  LC_PREV_CONF2			0x08000000	// 합성전 구속콘
#define  LC_PREV_ST_CON			0x10000000	// 합성전 Steel + 구속콘

// 합성전 사하중
const int FRAME_PRE_DEADLOAD	= LC_PREV_S|LC_PREV_C|LC_PREV_CONF|LC_PREV_TURN;
const int FRAME_PRE_DEADLOAD_2	= LC_PREV_ST_CON|LC_PREV_C|LC_PREV_CONF;
const int FRAME_NEXT_DEADLOAD	= LC_NEXT;


class CFemModelingCalc;
class CPlateBasicIndex;

class AFX_EXT_CLASS CFemStressCalc 
{
public:
	CFemStressCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd);
	virtual ~CFemStressCalc();

public:
	enum { CURRENT, LEFT, RIGHT, BOTH };
	
	CFrameSection* GetFrameSectionByIndex(CFEMManage *pFEM, long nIndex);
	// 현재 단면의 I33값을 가져올때 스테이션의 위치
	double GetStationCheckPos(CPlateBasicIndex *pBx);
	// 현재 단면의 I33값을 가져올때 스테이션의 위치
	double GetStationCheckPos(long nG, double dSta);
	// 현재 단면의 넓이
	double GetAreaOfSectionByStation(long nG, double dSta);

	CDataManage      *m_pDataManage;
	CAPlateCalcStd   *m_pCalcStd;

	map<CString, double> m_MapSecStress;// GetSectionStress
	double GetMapValueSecStress(CString sz);	
	
	double GetOneNodForceSub(CFEMManage *pFEM, int nForceType, long nLC, int nNodIdx, char cAxis);
	double GetOneNodForce(int nForceType, int nLC, int nNodIdx, char cAxis, BOOL &bExist);

	///< nForceType, nLC에 의해서 요소(nEleIdx)의 길이비위치(dRatio)의	결과값을 구한다. 
	double GetOneEleForce(int nForceType, int nLC, int nEleIdx, double dRatio);

	///< nForceType, nLC에 의해서 해당 요소(nEleIdx)의 최대결과값을 구한다. 
	double GetOneEleMaxForce(int nForceType, int nLC, int nEleIdx);
	
	///< 거더(nG)의 스테이션(dSta)의 요소력을 구한다.	 
	///<   nResultType ==  0   지정된 값. (Girder와 Basic Index가 지정되어야 함.)
	///<               ==  1   최대값. (+로 제일 큰값)
	///<               ==  2   최소값. (-로 제일 큰값)
	///<               ==  3   최대 절대값. (절대값으로 제일 큰값. 단, 부호는 그대로 리턴됨.)
	///<               ==  4   합계 Max와 Min중 절대값이 큰값. (단, 부호는 그대로 리턴됨.)
	///<                       (이 경우는 LoadCase가 무시되고 무조건 합계 Max Min값으로 계산.)
	double GetElementForce (int nForceType, int nLC, long &nG, double &dSta, int nResultType);

	///< 거더(nG)의 스테이션(dStaStt)과 스테이션(dStaEnd)의 범위중의 요소력을 구한다.	 
	double GetElementForceTerm(int nForceType, int nLC, long nG, double &dStaStt, double dStaEnd, int nResultType);

	///< 상,하부 응력값 및 허용응력값 구하고 응력비를 리턴함.
	double GetMaxStressRatio(long nG, double dSta, double &Max_up, double &Max_lo, double &Comp_up, double &Comp_lo, double &Tau, double dStaSec=0, long nPosSec=0);

	///< nG거더의 dSta위치에서 상하부최대응력값과 허용응력, 최대전단응력과 허용전단응력을 구함
	void GetFlangeStress(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec=0, long nPosSec=0);
	void GetFlangeStress_TUGirder(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec=0, long nPosSec=0);
	void GetFlangeStress_GenGirder(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec=0, long nPosSec=0);

	///< 단면 검토를 위한 각종 응력값들 구하기
	void GetSectionStress(long nG, double dSta, BOOL bHAPSUNG=TRUE, double dStaSec=0, long nPosSec=0);
	void GetSectionStress_TUGirder(long nG, double dSta, BOOL bHAPSUNG=TRUE, double dStaSec=0, long nPosSec=0);
	void GetSectionStress_GenGirder(long nG, double dSta, BOOL bHAPSUNG=TRUE, double dStaSec=0, long nPosSec=0);
	double GetAbsMaxMinEleForce(long nForceType, long nElemIdx, double dRatio);

	///< 단면저항모멘트을 생성함
	void MakeDanmyunMomentData(long nG);
	void DeleteOnNotUsedExtSection(int nG);
	void SetDanmyunMoment(CDanMyunProp *pPreSect, CDanMyunProp *pCurSect, CDanMyunProp *pNextSect, int nType, double dBound);
	
	BOOL InsertMomentPos(long nG);
	BOOL IsMomentPos(long nG, double dSta);
	///< 단면검토구간을 정함.
	void SetSectionGugan();
	
	///< 일정구간(가로보 간격) 사이에 변단면 여부 구함.
	///< return  0 : 변단면 아님
	///<        -1 : ↙ 방향으로 기울어짐
	///<         1 : ↘ 방향으로 기울어짐
	///< pBxTerm = 변단면BX
	//long GetByunDanStyle(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CPlateBasicIndex *&pBxTerm);
	
	///< 일정구간(시작절점, 끝절점 간격) 사이에 변단면 여부 구함.
	long GetByunDanStyle(long nStt, long nEnd, double &dStaByun);
	
	///< pBx위치의 변단면 여부 구함
	long GetByunDanStyle(CPlateBasicIndex *pBx);
	
	///< 1. m_ArrSecSta의 단면을 설정하고 
	///< 2. m_dMatMinSpace(단면검토 최소 간격) 이하의 단면 변화에 대해서 무시(제거)
	///< 3. 같은 단면 번호 제거함
	//void SetSectionNameAndRemoveMinSpace();

	///< SetSectionNameAndRemoveMinSpace()에서 만들어진 단면을 비교하여 같은 단면은 같은 단면 번호로...
	//void SetSameSectionToOne();

	///< nG거더의 dSttSta, dEndSta사이가 변단면 형태인지?
	//int  IsByunDanStyle(long nG, double dSttSta, double dEndSta);

	///< 현재 중앙거더 스테이션이 지점인지?
	BOOL IsJijumByStation(double dSta);
	
	///< x1: 시작데이터위치
	///< y1: 시작데이터값
	///< x2: 끝데이터위치
	///< y2: 끝데이터값
	///< x : 보간할 위치값
	double Bogan(double x1, double x2, double y1, double y2, double x);
	
	///< CDWordArray값을 sort함
	void InsertionSort(CDWordArray &Data);

	///< 현장이음검토 위치를 설정함.
	void SetSectionSplice();

	// 가로보 검토위치를 설정함.
	void SetSectionCrossBeamPos();
	
	// 수직브레이싱의 검토위치를 설정함
	void SetSectionVBracingPos();

	// 수평브레이싱의 검토위치를 설정함
	void SetSectionHBracingPos();

	// 가로보 검토를 위한 영향선 종거값 및 합계 구하기
	double CalcInfluenceLineCross(double Length,double Distance,long nLoadType,long nForceType,CDDWordArray &DArrY);
	
	// 세로보 검토를 위한 영향선 종거값 및 합계 구하기
	double CalcInfluenceLineStg(double Length,double Distance,long nLoadType,long nForceType,CDDWordArray &DArrY);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 단면검토부 다이얼로그에 들어가는 도면생성(얼마 안되어 따로 클래스 생성 안함)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///< 단면저항모멘트 및 단면에 작용하는 모멘트를 그림.
	void OutputOfMomentDiagram(CDomyun *pDom, long nG, double dScaleMoment, double Scale = 300, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, long nML = 0, BOOL bXL = FALSE);
	///< 검토단면의 위치와 종단면의 형태를 그림
	void OutputOfSectionSummary(CDomyun *pDom, long nG, double GFactor, double scaleH, double scaleY = 300, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, long nML = 0);
	///< 상판, 하판, 복부판, 경간구성, 현장이음 등의 치수선을 그림
	void OutputOfDimSectionSummary(CDomyun *pDom, double BaseY, long nDan, CString sTitle, CDDWordArray &DimSta, CDDWordArray &LenSta, CDDWordArray &ArrThick);
	void OutputOfFatigueCheckPos(CDomyun *pDom, BOOL bTitle, double TextHeight=1);
	void OutputOfExtraSection(CDomyun *pDom, CDRect rect);
	// 변단면시 부재력 최대인 위치
	void GetAddMaxForceSect(CDWordArray &DArrnG, CDDWordArray &DArrSta);
	// 변단면시 부재력 최소인 위치
	void GetAddMinSect(CDWordArray &DArrnG, CDDWordArray &DArrSta);
	// 최소단면 중복 검사및 추가
	void CompareHeightAddSect(CDWordArray &DArnG, CDDWordArray &DArSta, CDWordArray &DArnG2, CDDWordArray &DArSta2, CDWordArray &DArType);
	void CompareMaterialRemoveDup(CDWordArray &DArnG, CDDWordArray &DArSta, CDWordArray &DArMat);
	// 구간사이에서 최소 웨브 높이의 위치 구하기
	double GetStationMinHeight(long nG, double dSttSta, double dEndSta);
	// 거더 상판, 하판, 웨브 두께 구하기
	double GetThickFlangeBySta(long nMarkBuje, long nG, double dSta, BOOL bSmall = TRUE);
	// 정모멘트의 여부판별
	BOOL IsPositive(int nForceType, long nG, double dSta);
	// 플랜지&복부판 전/후 구간의 동일제원 다른강종여부 판별
	BOOL IsSameSteelMaterial(CPlateBasicIndex *pBx);
	BOOL IsSameSteelMaterial(long nG, double dSta);
};

#endif // !defined(AFX_FEMSTRESSCALC_H__502BF5DE_6071_40FC_B600_C70A2C3BFDEB__INCLUDED_)
