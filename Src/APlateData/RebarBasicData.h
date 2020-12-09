// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RebarBasicData.h: interface for the CRebarBasicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REBARBASICDATA_H__38C78D61_1963_4C10_A5C7_B87FE005C512__INCLUDED_)
#define AFX_REBARBASICDATA_H__38C78D61_1963_4C10_A5C7_B87FE005C512__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define REBAR_WEIGHT_COL	4	// 수정시 Serialize()
#define REBAR_WEIGHT_ROW	7
#define REBAR_JOINT_COL		7
#define REBAR_JOINT_ROW		7
#define REBAR_SETTLE_COL	5
#define REBAR_SETTLE_ROW	7

// 단위중량 및 단면적
#define BASIC_WEIGHT_UNIT	0
#define BASIC_AREA_UNIT		1
#define BASIC_DIAMEATER		2
#define BASIC_GIRTH			3

// 정착장
#define SETTLE_TENS_BASIC	0
#define SETTLE_TENS_NORMAL	1
#define SETTLE_TENS_UPPER	2
#define SETTLE_COMP_BASIC	3
#define SETTLE_COMP_SPIRAL 	4

// 이음장
#define JOINT_TENS_NORMAL_A	0
#define JOINT_TENS_NORMAL_B	1
#define JOINT_TENS_UPPER_A	2
#define JOINT_TENS_UPPER_B	3
#define JOINT_COMP_NORMAL 	4
#define JOINT_COMP_BELT 	5
#define JOINT_COMP_SPIRAL 	6

//절곡 철근 
#define JOINT_BEBD_R 		0
#define JOINT_BEBD_C 		1

#define MAX_REBAR_DETAIL	45

#define MAX_UPPER_TYPE		6
#define MAX_PIER_TYPE		7
#define MAX_CONDITION_TYPE	4
#define MAX_CRACK_ENV_TYPE	4

#define MAX_BM_FORM_TYPE		8
#define MAX_BM_FORM_TYPE_NORMAL	6

typedef struct STR_JOINTCALC_FACT
{
	double m_dAlpha_UpRebar;
	double m_dAlpha_GenBar;
	double m_dBeta_Tomak1;
	double m_dBeta_Tomak2;
	double m_dBeta_GenBar;
	double m_dGamma_D22Up;
	double m_dGamma_D22Dn;
	double m_dIpSmallConc;
	double m_dIpGenConc;

} JOINT_FACTOR;

class CDataManage;
class AFX_EXT_CLASS CRebarBasicData  
{
public:
	CRebarBasicData(CDataManage* pManage = NULL);
	virtual ~CRebarBasicData();

	void Serialize(CArchive& ar);

	CDataManage* m_pManage;
	long	m_nTypeRebarConnect;	// 철근겹이음 방식 : 0 - 보정계수에 의한 방법 , 1 - 산식에 의한방법
	double  m_dLengthRebarMax;		// 사용철근의 길이
	BOOL	m_bRoundRebarConnect;	// 철근 겹이음, 정착장을 cm에서 반올림
	double   m_RebarWeight[REBAR_WEIGHT_ROW][REBAR_WEIGHT_COL];
	double   m_RebarJoint [REBAR_JOINT_ROW][REBAR_JOINT_COL];
	double   m_RebarSettle[REBAR_SETTLE_ROW][REBAR_SETTLE_COL];

	JOINT_FACTOR m_JointFactor;

	double GetValueBasic (long   nDia, long nType);
	double GetValueBasic (double nDia, long nType);
	double GetValueSettle(long   nDia, long nType);
	double GetValueSettle(double nDia, long nType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);
	double GetValueSettleByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp);
	double GetValueJoint (long   nDia, long nType);
	double GetRebarInfo(double dDia, long nType);
	double GetValueJointByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp);
	double GetValueJoint (double nDia, long nType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);
};


#endif // !defined(AFX_REBARBASICDATA_H__38C78D61_1963_4C10_A5C7_B87FE005C512__INCLUDED_)
