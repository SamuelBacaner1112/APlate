// RebarBasicData.cpp: implementation of the CRebarBasicData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RebarBasicData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRebarBasicData::CRebarBasicData(CDataManage* pManage)
{
	m_pManage = pManage;

	m_nTypeRebarConnect		=	1;  // 철근겹이음 방식 - 보정계수에 의한 방법
	m_dLengthRebarMax		= 8000;
	m_bRoundRebarConnect    = TRUE;	// 철근 겹이음, 정착장을 cm에서 반올림

	for(long i=0; i<REBAR_WEIGHT_ROW; i++)
		for(long j=0; j<REBAR_WEIGHT_COL; j++)
			m_RebarWeight[i][j] = 0;
	for(i=0; i<REBAR_JOINT_ROW; i++)
		for(long j=0; j<REBAR_JOINT_COL; j++)
			m_RebarJoint[i][j] = 0;
	for(i=0; i<REBAR_SETTLE_ROW; i++)
		for(long j=0; j<REBAR_SETTLE_COL; j++)
			m_RebarSettle[i][j] = 0;
		
	m_JointFactor.m_dAlpha_UpRebar = 1.3;
	m_JointFactor.m_dAlpha_GenBar = 1.0;
	m_JointFactor.m_dBeta_Tomak1 = 1.5;
	m_JointFactor.m_dBeta_Tomak2 = 1.2;
	m_JointFactor.m_dBeta_GenBar = 1.0;
	m_JointFactor.m_dGamma_D22Up = 1.0;
	m_JointFactor.m_dGamma_D22Dn = 0.8;
	m_JointFactor.m_dIpSmallConc = 1.3;
	m_JointFactor.m_dIpGenConc = 1.0;

}

CRebarBasicData::~CRebarBasicData()
{

}
void CRebarBasicData::Serialize(CArchive& ar)
{	
	long nFlag = 0;
	if(ar.IsStoring()) 
	{
		ar << nFlag;
		ar << m_nTypeRebarConnect;
		ar << m_dLengthRebarMax;
		ar << m_bRoundRebarConnect;

		for(long i=0; i<REBAR_WEIGHT_ROW; i++)
			for(long j=0; j<REBAR_WEIGHT_COL; j++)
				ar << m_RebarWeight[i][j];
		for(i=0; i<REBAR_JOINT_ROW; i++)
			for(long j=0; j<REBAR_JOINT_COL; j++)
				ar << m_RebarJoint[i][j];
		for(i=0; i<REBAR_SETTLE_ROW; i++)
			for(long j=0; j<REBAR_SETTLE_COL; j++)
				ar << m_RebarSettle[i][j];		

		ar << m_JointFactor.m_dAlpha_UpRebar;
		ar << m_JointFactor.m_dAlpha_GenBar;
		ar << m_JointFactor.m_dBeta_Tomak1;
		ar << m_JointFactor.m_dBeta_Tomak2;
		ar << m_JointFactor.m_dBeta_GenBar;
		ar << m_JointFactor.m_dGamma_D22Up;
		ar << m_JointFactor.m_dGamma_D22Dn;
		ar << m_JointFactor.m_dIpSmallConc;
		ar << m_JointFactor.m_dIpGenConc;
	}
	else
	{
		ar >> nFlag;
		ar >> m_nTypeRebarConnect;
		ar >> m_dLengthRebarMax;
		ar >> m_bRoundRebarConnect;

		for(long i=0; i<REBAR_WEIGHT_ROW; i++)
			for(long j=0; j<REBAR_WEIGHT_COL; j++)
				ar >> m_RebarWeight[i][j];
		for(i=0; i<REBAR_JOINT_ROW; i++)
			for(long j=0; j<REBAR_JOINT_COL; j++)
				ar >> m_RebarJoint[i][j];
		for(i=0; i<REBAR_SETTLE_ROW; i++)
			for(long j=0; j<REBAR_SETTLE_COL; j++)
				ar >> m_RebarSettle[i][j];		

		ar >> m_JointFactor.m_dAlpha_UpRebar;
		ar >> m_JointFactor.m_dAlpha_GenBar;
		ar >> m_JointFactor.m_dBeta_Tomak1;
		ar >> m_JointFactor.m_dBeta_Tomak2;
		ar >> m_JointFactor.m_dBeta_GenBar;
		ar >> m_JointFactor.m_dGamma_D22Up;
		ar >> m_JointFactor.m_dGamma_D22Dn;
		ar >> m_JointFactor.m_dIpSmallConc;
		ar >> m_JointFactor.m_dIpGenConc;
	}
}


double CRebarBasicData::GetValueJoint(double nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	if(m_nTypeRebarConnect==0 || dCover == 0 || dRebarSpace == 0)
		return m_bRoundRebarConnect ? Round(GetValueJoint((long)nDia,nType), -1) : GetValueJoint((long)nDia,nType);

	return m_bRoundRebarConnect ? Round(GetValueJointByCalc((long)nDia,nType,dCover,dRebarSpace,nBeta,nIp), -1) : GetValueJointByCalc((long)nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

double CRebarBasicData::GetValueJoint(long nDia, long nType)
{
	if(nType<0 || nType>JOINT_COMP_SPIRAL) return 0;
	double dValue = 0;

	if(nDia==13) dValue = m_RebarJoint[0][nType];
	if(nDia==16) dValue = m_RebarJoint[1][nType];
	if(nDia==19) dValue = m_RebarJoint[2][nType];
	if(nDia==22) dValue = m_RebarJoint[3][nType];
	if(nDia==25) dValue = m_RebarJoint[4][nType];
	if(nDia==29) dValue = m_RebarJoint[5][nType];
	if(nDia==32) dValue = m_RebarJoint[6][nType];

	return dValue;
}

double CRebarBasicData::GetRebarInfo(double dDia, long nType)
{
	double dArea = 0;
	long nDia = (long)dDia;
	switch(nDia)
	{
	case 13:		
		dArea = m_RebarWeight[0][nType];
		break;
	case 16:		
		dArea = m_RebarWeight[1][nType];
		break;
	case 19:
		dArea = m_RebarWeight[2][nType];
		break;
	case 22:
		dArea = m_RebarWeight[3][nType];
		break;
	case 25:		
		dArea = m_RebarWeight[4][nType];
		break;
	case 29:		
		dArea = m_RebarWeight[5][nType];
		break;
	case 32:		
		dArea = m_RebarWeight[6][nType];
		break;
	}

	return dArea;
}

// nType : 0 - 일반 - A급, 1 - 일반 B급, 2 - 상부 A급, 3 - 상부 B급, 4 - 일반, 5 - 폐합 띠철근, 6 - 나선철근
double CRebarBasicData::GetValueJointByCalc(long nDia, long nType, double dCover, double dRebarSpace,
										  long nBeta, long nIp)
{
	CCalcData       *pData = m_pManage->GetCalcData();
	// 인장이형철근
	double dValue = 0;
	double Fy	  = pData->DESIGN_MATERIAL.m_dSigmaY;
	double Fck	  = pData->DESIGN_MATERIAL.m_dSigmaCK;			
	double dDia, dArea, dGamma, dC, dfv, dCd, dR, dAl, dBe, dIp, dFactor;
	
	switch(nDia)
	{
	case 13: 
		dDia  = m_RebarWeight[0][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[0][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Dn;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 16:
		dDia  = m_RebarWeight[1][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[1][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Dn;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 19:
		dDia  = m_RebarWeight[2][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[2][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Dn;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 22:
		dDia  = m_RebarWeight[3][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[3][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 25:
		dDia  = m_RebarWeight[4][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[4][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 29:
		dDia  = m_RebarWeight[5][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[5][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 32:
		dDia  = m_RebarWeight[6][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[6][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	}

	dfv = 0.285*dDia*Fy/pow(Fck,0.5);
	dCd = (dC / dDia) > 2.5 ? 2.5 : dC / dDia;
	dR = dGamma / dCd;	
	dAl = (nType==2 || nType==3) ? m_JointFactor.m_dAlpha_UpRebar : m_JointFactor.m_dAlpha_GenBar;
	dBe = (nBeta==0) ? m_JointFactor.m_dBeta_Tomak1 : (nBeta==1) ? m_JointFactor.m_dBeta_Tomak2 : m_JointFactor.m_dBeta_GenBar;
	dIp = (nIp==0) ? m_JointFactor.m_dIpSmallConc : m_JointFactor.m_dIpGenConc;
	dFactor = dAl * dBe * dIp;
	switch(nType)
	{
	case 0:	
		dValue = Round(dfv*dR*dFactor*10,0)>=300 ? Round(dfv*dR*dFactor*10,0) : 300;
		break;
	case 1:	
		dValue = Round(1.3*dfv*dR*dFactor*10,0)>=300 ? Round(1.3*dfv*dR*dFactor*10,0) : 300;
		break;
	case 2:	
		dValue = Round(dfv*dR*dFactor*10,0)>=300 ? Round(dfv*dR*dFactor*10,0) : 300;
		break;
	case 3:	
		dValue = Round(1.3*dfv*dR*dFactor*10,0)>=300 ? Round(1.3*dfv*dR*dFactor*10,0) : 300;
		break;
	case 4:			
		dValue = Round(0.0072*Fy*dDia*10,0)<300 ? 300 : Round(0.0072*Fy*dDia*10,0);
		break;	
	case 5:
		dValue = Round(0.83*0.0072*Fy*dDia*10,0)<300 ? 300 : Round(0.83*0.0072*Fy*dDia*10,0);		
		break;	
	case 6:
		dValue = Round(0.75*0.0072*Fy*dDia*10,0)<300 ? 300 : Round(0.75*0.0072*Fy*dDia*10,0);		
		break;
	}
	
	return dValue;
}

double CRebarBasicData::GetValueSettle(double nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	if(m_nTypeRebarConnect==0 || dCover == 0 || dRebarSpace == 0)
		return m_bRoundRebarConnect ? Round(GetValueSettle((long)nDia,nType), -1) : GetValueSettle((long)nDia,nType);

	return m_bRoundRebarConnect ? Round(GetValueSettleByCalc((long)nDia,nType,dCover,dRebarSpace,nBeta,nIp), -1) : GetValueSettleByCalc((long)nDia,nType,dCover,dRebarSpace,nBeta,nIp);	
}

double CRebarBasicData::GetValueSettle(long nDia, long nType)
{
	if(nType<0 || nType>SETTLE_COMP_SPIRAL) return 0;
	double dValue = 0;

	if(nDia==13) dValue = m_RebarSettle[0][nType];
	if(nDia==16) dValue = m_RebarSettle[1][nType];
	if(nDia==19) dValue = m_RebarSettle[2][nType];
	if(nDia==22) dValue = m_RebarSettle[3][nType];
	if(nDia==25) dValue = m_RebarSettle[4][nType];
	if(nDia==29) dValue = m_RebarSettle[5][nType];
	if(nDia==32) dValue = m_RebarSettle[6][nType];

	return dValue;
}

double CRebarBasicData::GetValueSettleByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{	
	CCalcData       *pData = m_pManage->GetCalcData();
	// 인장이형철근
	double dValue = 0;
	double Fy	  = pData->DESIGN_MATERIAL.m_dSigmaY;
	double Fck	  = pData->DESIGN_MATERIAL.m_dSigmaCK;			
	double dDia, dArea, dGamma, dC, dfv, dCd, dR, d4df, dAl, dBe, dIp, dFactor;
	
	switch(nDia)
	{
	case 13:
		dDia  = m_RebarWeight[0][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[0][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Dn;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 16:
		dDia  = m_RebarWeight[1][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[1][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Dn;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 19:
		dDia  = m_RebarWeight[2][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[2][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Dn;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 22:
		dDia  = m_RebarWeight[3][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[3][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 25:
		dDia  = m_RebarWeight[4][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[4][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 29:
		dDia  = m_RebarWeight[5][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[5][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	case 32:
		dDia  = m_RebarWeight[6][BASIC_DIAMEATER]/10.0;
		dArea = m_RebarWeight[6][BASIC_AREA_UNIT];
		dGamma = m_JointFactor.m_dGamma_D22Up;
		dC = min(dCover, dRebarSpace/2);
		break;
	}

	dfv = 0.285*dDia*Fy/pow(Fck,0.5);
	dCd = (dC / dDia) > 2.5 ? 2.5 : dC / dDia;
	dR = dGamma / dCd;	
	dAl = (nType==2) ? m_JointFactor.m_dAlpha_UpRebar : m_JointFactor.m_dAlpha_GenBar;
	dBe = (nBeta==0) ? m_JointFactor.m_dBeta_Tomak1 : (nBeta==1) ? m_JointFactor.m_dBeta_Tomak2 : m_JointFactor.m_dBeta_GenBar;
	dIp = (nIp==0) ? m_JointFactor.m_dIpSmallConc : m_JointFactor.m_dIpGenConc;
	dFactor = dAl * dBe * dIp;
	d4df = 0.004 * dDia * Fy;
	switch(nType)
	{
	case 0:	// 기본		
		dValue = Round(dfv*dR*dFactor*10,0)>=300 ? Round(dfv*dR*dFactor*10,0) : 300;
		break;
	case 1:	// 일반 - 인장부
		dValue = Round(dfv*dR*dFactor*10,0)>=300 ? Round(dfv*dR*dFactor*10,0) : 300;
		break;
	case 2:	// 상부 - 인장부
		dValue = Round(dfv*dR*dFactor*10,0)>=300 ? Round(dfv*dR*dFactor*10,0) : 300;
		break;
	case 3:	// 기본 - 압축부
		dValue = Round(0.08*Fy*dDia/sqrt(Fck)*10,0) < max(20,d4df) ? max(20,d4df) : Round(0.08*Fy*dDia/sqrt(Fck)*10,0);
		break;
	case 4:	// 나선 - 압축부		
		dValue = Round(0.75*0.0072*Fy*dDia*10,0)<300 ? 300 : Round(0.75*0.0072*Fy*dDia*10,0);
		break;	
	}
	
	return dValue;
}

double CRebarBasicData::GetValueBasic(double nDia, long nType)
{
	return GetValueBasic((long)nDia,nType);
}

double CRebarBasicData::GetValueBasic(long nDia, long nType)
{
	if(nType<0 || nType>BASIC_GIRTH) return 0;
	double dValue = 0;

	if(nDia==13) dValue = m_RebarWeight[0][nType];
	if(nDia==16) dValue = m_RebarWeight[1][nType];
	if(nDia==19) dValue = m_RebarWeight[2][nType];
	if(nDia==22) dValue = m_RebarWeight[3][nType];
	if(nDia==25) dValue = m_RebarWeight[4][nType];
	if(nDia==29) dValue = m_RebarWeight[5][nType];
	if(nDia==32) dValue = m_RebarWeight[6][nType];

	return dValue;
}