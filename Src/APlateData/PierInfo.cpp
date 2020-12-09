// PierInfo.cpp: implementation of the CPierInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PierInfo.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPierInfo::CPierInfo(CPierInfo* pPierInfo /*= NULL*/)
{
	if(pPierInfo)
	{
		*this = *pPierInfo;
		return;
	}
	m_dwFlag = 0;
	m_nNumberPier = 0;
//	m_dWidth = 0;
//	m_dALeft = 0;
//	m_dARight = 0;

	m_daT1 = 300;
	m_daT2 = 500;
	m_daT3 = 250;
	m_daT4 = 1450;
	m_daH1 = 400;
	m_daH2 = 230;
	m_daH3 = 2000;
	m_daH4 = 2500;
	m_daH5 = 1100;
	m_daH6 = 880;
	m_daH7 = 2700;
	m_daH8 = 800;
	m_daB1 = 1600;
	m_daB2 = 1450;
	m_daB3 = 1000;
	m_daK1 = 100;
	m_daK2 = 100;

	m_daW = 4950;
	m_daW1 = 1500;
	m_daW2 = 3150;

	m_daWL = 10400;
	m_daWT = 500;

	m_dpB1 = 2750;
	m_dpB2 = 2750;
	m_dpB3 = 2750;
	m_dpB4 = 2750;
	m_dpD = 2500;
	m_dpP = 1250;		
	m_dpN = 1250;		
	m_dpW = 10400;
	m_dpH1 = 1100;
	m_dpH2 = 900;
	m_dpK1 = 100;
	m_dpK2 = 100;
	m_dpH3 = 10000;
	m_dpH4 = 2000;		

	m_dpR  = 0;
}

CPierInfo::~CPierInfo()
{

}

CPierInfo& CPierInfo::operator = (const CPierInfo& obj)
{
	if(this == &obj) return *this;

	m_dwFlag		= obj.m_dwFlag;
	m_nNumberPier	= obj.m_nNumberPier;
//	m_dWidth		= obj.m_dWidth;
//	m_dALeft		= obj.m_dALeft;
//	m_dARight		= obj.m_dARight;
	m_daT1			= obj.m_daT1;
	m_daT2			= obj.m_daT2;	
	m_daT3			= obj.m_daT3;
	m_daT4			= obj.m_daT4;
	m_daH1			= obj.m_daH1;
	m_daH2			= obj.m_daH2;
	m_daH3			= obj.m_daH3;
	m_daH4			= obj.m_daH4;
	m_daH5			= obj.m_daH5;
	m_daH6			= obj.m_daH6;
	m_daH7			= obj.m_daH7;
	m_daH8			= obj.m_daH8;
	m_daB1			= obj.m_daB1;
	m_daB2			= obj.m_daB2;
	m_daB3			= obj.m_daB3;
	m_daK1			= obj.m_daK1;
	m_daK2			= obj.m_daK2;

	m_daW			= obj.m_daW;
	m_daW1			= obj.m_daW1;
	m_daW2			= obj.m_daW2;

	m_daWL			= obj.m_daWL;
	m_daWT			= obj.m_daWT;

	m_dpB1			= obj.m_dpB1;
	m_dpB2			= obj.m_dpB2;
	m_dpB3			= obj.m_dpB3;
	m_dpB4			= obj.m_dpB4;
	m_dpD			= obj.m_dpD;
	m_dpP		= obj.m_dpP;
	m_dpN		= obj.m_dpN;
	m_dpW			= obj.m_dpW;
	m_dpH1			= obj.m_dpH1;
	m_dpH2			= obj.m_dpH2;
	m_dpK1			= obj.m_dpK1;
	m_dpK2			= obj.m_dpK2;
	m_dpH3			= obj.m_dpH3;
	m_dpH4			= obj.m_dpH4;

	m_dpR			= obj.m_dpR;
	return *this;
}


void CPierInfo::Serialize(CArchive& ar)
{
	long nFlag = 0;
	long vlong = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_dwFlag;
		ar << m_nNumberPier;
		ar << m_daT1;
		ar << m_daT2;
		ar << m_daT3;
		ar << m_daT4;
		ar << m_daH1;
		ar << m_daH2;
		ar << m_daH3;
		ar << m_daH4;
		ar << m_daH5;
		ar << m_daH6;
		ar << m_daH7;
		ar << m_daH8;
		ar << m_daB1;
		ar << m_daB2;
		ar << m_daB3;
		ar << m_daK1;
		ar << m_daK2;

		ar << m_daW;
		ar << m_daW1;
		ar << m_daW2;

		ar << m_daWL;
		ar << m_daWT;

		ar << m_dpB1;
		ar << m_dpB2;
		ar << m_dpB3;
		ar << m_dpB4;
		ar << m_dpD;
		ar << m_dpP;
		ar << m_dpN;
		ar << m_dpW;
		ar << m_dpH1;
		ar << m_dpH2;
		ar << m_dpK1;
		ar << m_dpK2;
		ar << m_dpH3;
		ar << m_dpH4;

		ar << m_dpR;

		ar << vlong;
		ar << vlong;
		ar << vlong;
		ar << vlong;
		ar << vlong;
	}
	else
	{
		ar >> nFlag;
		ar >> m_dwFlag;
		ar >> m_nNumberPier;
		ar >> m_daT1;
		ar >> m_daT2;
		ar >> m_daT3;
			
		ar >> m_daT4;
		ar >> m_daH1;
		ar >> m_daH2;
		ar >> m_daH3;
		ar >> m_daH4;
		ar >> m_daH5;
		ar >> m_daH6;
		ar >> m_daH7;
		ar >> m_daH8;
		ar >> m_daB1;
		ar >> m_daB2;
		ar >> m_daB3;
		ar >> m_daK1;
		ar >> m_daK2;

		ar >> m_daW;
		ar >> m_daW1;
		ar >> m_daW2;

		ar >> m_daWL;
		ar >> m_daWT;

		ar >> m_dpB1;
		ar >> m_dpB2;
		ar >> m_dpB3;
		ar >> m_dpB4;
		ar >> m_dpD;
		ar >> m_dpP;
		ar >> m_dpN;
		ar >> m_dpW;
		ar >> m_dpH1;
		ar >> m_dpH2;
		ar >> m_dpK1;
		ar >> m_dpK2;
		ar >> m_dpH3;
		ar >> m_dpH4;

		ar >> m_dpR;
		ar >> vlong;
		ar >> vlong;
		ar >> vlong;
		ar >> vlong;
		ar >> vlong;
	}

}

/*
void CPierInfo::SetLeftR(BOOL bUse)
{
	SetBitFlag32(m_dwFlag,1,bUse);
}

void CPierInfo::SetRightR(BOOL bUse)
{
	SetBitFlag32(m_dwFlag,2,bUse);
}

BOOL CPierInfo::GetLeftR() const
{
	return GetBitFlag32(m_dwFlag,1);
}

BOOL CPierInfo::GetRightR() const
{
	return GetBitFlag32(m_dwFlag,2);
}
*/
void CPierInfo::SetAbut(BOOL bAbut)
{
	SetBitFlag32(m_dwFlag,0,bAbut);
}

BOOL CPierInfo::IsAbut() const
{
	return GetBitFlag32(m_dwFlag,0);
}
