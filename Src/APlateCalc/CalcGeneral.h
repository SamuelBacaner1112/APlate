// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// CalcGeneral.h: interface for the CCalcGeneral class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCGENERAL_H__5A53A0BF_E0FF_4ED1_9723_DEE2060F39A3__INCLUDED_)
#define AFX_CALCGENERAL_H__5A53A0BF_E0FF_4ED1_9723_DEE2060F39A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const static double _dRebarDia[]	= {13, 16, 19, 22, 25, 29, 32, 35};
const static double _dRebarDiadim[]	= {12.7, 15.9, 19.1, 22.2, 25.4, 28.6, 31.8, 34.9};
const static double _dRebarArea[]	= {126.7, 198.6, 286.5, 387.1, 506.7, 642.4, 794.2, 956.6};

class CDataManage;
class AFX_EXT_CLASS CCalcGeneral
{
public:
	CCalcGeneral(CDataManage *pDataManage);
	virtual ~CCalcGeneral();

	CDataManage *m_pDataManage;

public:
	void GetSectionWeld(long nType);

	double CalcMoveCoeff() const;

	double GetDBLOAD_Dis_CAR()			{ return 0.3;		};
	double GetDBLOAD_Dis_Tank()			{ return 0.35;		};
	double GetDBLOAD_Dis_Trailer()		{ return 0.4875;	};

	void   CalcMovingLoad_I(double L, double&i, double& ii) const;
	double CalcMovingLoad_E(double L) const;
	double CalcMovingLoad_Pr(double A, double I, double E) const;
	double CalcMovingLoad_ML(double Pr, double L) const;
	// 충돌하중 계산
	double CalcCollisionLoad_Co(double V) const;
	double CalcCollisionLoad_Mo(double Co, double H) const;
	// 풍하중 계산
	double CalcWindLoad_DB(double B, double D) const;
	double CalcWindLoad_DBPw(double B, double D) const;
	double CalcWindLoad_Pw(double B, double D) const;
	double CalcWindLoad_Mw(double Pw, double D) const;
	// 원심 하중
	double CalcCentriodLoad_CF(double V, double R) const;
	double CalcCentriodLoad_Pcf(double CF, double V) const;
	double CalcCentriodLoad_Mcf(double Pcf, double LL) const;
	// 철근량 산출	
	void   CalcReinRod_As(double Mu,double sigmaY,double sigmaCK,double d,double b, double &As, double &a) const;
	double CalcReinRod_Md(double BendingPi,double As,double sigmaY,double d,double a) const;
	double CalcReinRod_Pused(double UseAs,double b,double d) const;
	double CalcReinRod_Pmax(double sigmaCK,double sigmaY) const;
	double CalcReinRod_Pmin(double sigmaY) const;
	double CalcReinRod_a(double As, double sigmaY, double sigmaCK, double b) const;
	// 중앙부 사하중
	double CalcCentPart_L() const;
	double CalcCentPart_Md(double W, double L) const;
	double CalcCentPart_P(double P, double i) const;
	double CalcCentPart_Ml(double L, double i, double t, double M) const;

	double GetWidthBodoDeadLoad(BOOL bLeft, CPlateBasicIndex *pBx);
	void AscendSort(CDDWordArray &DArrWheight, CString DArrHeight[]);

	void SlabMinThick(long nPos);

	// 경험적 설계가능 여부
	BOOL CheckExperienceDesign(BOOL bLeft) const;
	// 허용응력을 위한 강종
	void GetMaterial(CPlateBasicIndex *pBx, long nMaterial, CString &szLeft, CString &szRight);
	CPlateBasicIndex* GetCorrectBxIfTaper(long nG, double& dSta);
	
	//Safe Factor
	double	GetScaceFactor(long nLane);
	// 시방서기준 차선수 구하기
	long	GetQtyLane(double dWidth);
};

#endif // !defined(AFX_CALCGENERAL_H__5A53A0BF_E0FF_4ED1_9723_DEE2060F39A3__INCLUDED_)
